//<xxSigmatekCFileOptimize/>
#include "T_IPC.H"
#include "T_NUC.H"

#define FAQX 8192 // faktorierung für bitmapzoom, durch 8 teilbar somit keine multiplikation oder division (nur shl und shr)

void   colorCorrection256(_BMP*, _RGB256*, _UWORD);
_BOOL  load_gfx(_BMP *bmp, _ASCII *dpne, _UWORD *p_errorcode, _FUDWORD_UDWORD_UDWORD fp_callback);
_BOOL	 flash_gfx(_KOORD, _KOORD, _ASCII*);
_UWORD MakeIndexColor(_UWORD, _UWORD, _UBYTE*);
_BOOL  zoom_BMP(_BMP*, _UWORD, _UWORD);

_UWORD mic_color = 0;
_UWORD mic_index = 0;
_UWORD MakeIndexColor(_UWORD color, _UWORD TargetCol, _UBYTE *palette)
{
 #ifdef UC_HICOLOR
  _RGB    rgb;
	_UWORD  i, retcode = 0;
	_UDWORD l, DiffSquare, NearestSquare;

  if(mic_color == color)
    return(mic_index);

  mic_color = color;
	rgb.cred   = ((((color >> 11) & 0x001F) * 255) / 31);
	rgb.cgreen = ((((color >> 5) & 0x003F) * 255) / 63);
	rgb.cblue  = (((color & 0x001F) * 255) / 31);

	// hunt for nearest squared difference in array
	NearestSquare = 0xFFFFFFFFL;

  i = 0;
  while(TargetCol)
  {
    TargetCol--;

		l = (_UDWORD)absolut(rgb.cred - palette[i]);
		DiffSquare = l * l;
    i ++;
		l = (_UDWORD)absolut(rgb.cgreen - palette[i]);
		DiffSquare += l * l;
    i ++;
		l = (_UDWORD)absolut(rgb.cblue - palette[i]);
		DiffSquare += l * l;
    i ++;
		if(DiffSquare < NearestSquare)
		{
			NearestSquare = DiffSquare;
			retcode  = (i/3) - 1;
		}
	}

  mic_index = retcode;
	return(retcode);
 #else
  return(color);
 #endif
}

_BOOL load_gfx(_BMP *bmp, _ASCII *dpne, _UWORD *p_errorcode, _FUDWORD_UDWORD_UDWORD fp_callback)
{
  _BOOL retcode = _FALSE;

  gfxerror_set(p_errorcode, GFXERROR_NONE);

 #ifdef UC_IPC
  switch(GetFiletype(dpne))
  {
    case FILEXT_BMO : retcode = load_bmo(bmp, dpne, p_errorcode); break;
    case FILEXT_BMP : retcode = load_bmp(bmp, dpne, p_errorcode, fp_callback); break;
    case FILEXT_GIF : retcode = load_gif(bmp, dpne, p_errorcode); break;
    //case FILEXT_ICO : retcode = load_ico(bmp, dpne, p_errorcode); break;
    case FILEXT_JPG : retcode = load_jpg(bmp, dpne, p_errorcode, fp_callback); break;
    default         : gfxerror_set(p_errorcode, GFXERROR_UNKNOWNFILE); break;
  }
 #endif

  if(retcode == _FALSE)
    gfxerror_set(p_errorcode, GFXERROR_UNKNOWN);
    
 #ifdef UC_GRAYSCALE
  if(retcode == _TRUE)
    transform_BMP_to_gray(bmp);
 #endif

  return(retcode);
}

_BOOL flash_gfx(_KOORD x, _KOORD y, _ASCII *dpne, _UWORD *p_errorcode, _FUDWORD_UDWORD_UDWORD fp_callback)
{
  _BOOL retcode = _FALSE;
  _BMP  bmp;

  init_BMP_Makro(&bmp);
  if(load_gfx(&bmp, dpne, p_errorcode, fp_callback) == _TRUE)
  {
    retcode = _TRUE;
   #ifdef UC_MOUSE
    HideMouseCursor();
   #endif

		Bitmap(x, y, &bmp, T_COPY|T_SOLID, 0);
   #ifdef UC_ROTATE
    _ROOM room;
    room.xy1.x = x;
    room.xy1.y = y;
    room.xy2.x = x + bmp.width - 1;
    room.xy2.y = y + bmp.height - 1;
    RefreshRectangle(&room);
   #endif

   #ifdef UC_MOUSE
    ShowMouseCursor();
   #endif
  }
  free_BMP(&bmp);

  return(retcode);
}

_BOOL save_gfx_offscreen(_ASCII *dpne, _KOORD x1, _KOORD y1, _KOORD x2, _KOORD y2, _UDWORD colno, _NEWSCREEN *offscreen)
{
  _BOOL retcode;
  _NEWSCREEN act;


  GetScreen(&act);
  WritePixel(0, 0, T_COPY|T_SOLID, ReadPixel(0, 0)); // set copy attribute to destination screen
 #ifdef OK_OFFSCREEN
  if(UseOffscreen == _TRUE)
  {
    ActivateScreen(offscreen);
  }
 #endif

  retcode = save_gfx(dpne, x1, y1, x2, y2, colno);

  ActivateScreen(&act);

  return(retcode);
}

_BOOL save_gfx(_ASCII *dpne, _KOORD x1, _KOORD y1, _KOORD x2, _KOORD y2, _UDWORD colno)
{
  _BOOL retcode = _FALSE;

  if((x1 == DEFKOORD)&&(y1 == DEFKOORD)&&(x2 == DEFKOORD)&&(y2 == DEFKOORD))
  {
    _ROOM room;
    GetScreenSize(&room);
    x1 = room.xy1.x;
    y1 = room.xy1.y;
    x2 = room.xy2.x;
    y2 = room.xy2.y;
  }

 #ifdef UC_IPC
  switch(GetFiletype(dpne))
  {
    case FILEXT_BMO : retcode = save_bmo(dpne, x1, y1, x2, y2); break;
    case FILEXT_BMP : retcode = save_bmp(dpne, x1, y1, x2, y2, (colno==256)? _TRUE:_FALSE); break;
    case FILEXT_GIF : retcode = save_gif(dpne, x1, y1, x2, y2); break;
    case FILEXT_JPG : retcode = save_jpg(dpne, x1, y1, x2, y2, colno, _FALSE); break;
    default         : break;
  }
 #endif

  return(retcode);
}

_BOOL save_gfx_BMP(_ASCII *dpne, _BMP *bmp, _UDWORD colno)
{
  _BOOL retcode = _FALSE;

 #ifdef UC_IPC
  switch(GetFiletype(dpne))
  {
//    case FILEXT_BMO : retcode = save_bmo_BMP(dpne, bmp); break;
//    case FILEXT_BMP : retcode = save_bmp_BMP(dpne, bmp (colno==256)? _TRUE:_FALSE); break;
    case FILEXT_GIF : retcode = save_gif_BMP(dpne, bmp); break;
    case FILEXT_JPG : retcode = save_jpg_BMP(dpne, bmp, colno, _FALSE); break;
    default         : break;
  }
 #endif

  return(retcode);
}

#ifdef UC_IPC

void init_GFXBMP(_GFXBMP *gfxbmp)
{
  gfxbmp->dpne[0] = 0;
  init_BMP_Makro(&gfxbmp->bmp);
}

void free_GFXBMP(_GFXBMP *gfxbmp)
{
  free_BMP(&gfxbmp->bmp);
  init_GFXBMP(gfxbmp);
}

void _Gfx::Init(void)
{
  No  = 0;
  Ptr = NULL;
}

void _Gfx::Free(void)
{
  _UWORD  i;

  for(i=0; i<No; i++)
    free_GFXBMP(&Ptr[i]);

  Memory2((void**)&Ptr, 0);

  Init();
}

_UDWORD _Gfx::Scan4File(_ASCII *dpne)
{
  _UWORD i;

  for(i=0; i<No; i++)
  {
	 if(StrXmp(&Ptr[i].dpne[0], dpne, sizeof(_ASCII)) == 0)
		return(i);
  }

  return(0xFFFF);
}

_BOOL  _Gfx::AddToMem(_GFXBMP *tmp)
{
  _BOOL retcode = _FALSE;

  if(Memory2((void**)&Ptr, ((No+1)*sizeof(_GFXBMP))) != 0)
  {
    init_GFXBMP(&Ptr[No]);
    Ptr[No]  = *tmp;
    No      += 1;
    retcode  = _TRUE;
  }

  return(retcode);
}

_BOOL _Gfx::Load(_ASCII *dpne)
{
  _BOOL   retcode = _TRUE;
  _GFXBMP gfxbmp;

  if(Scan4File(dpne) >= No)
  {
    retcode = _FALSE;
    init_GFXBMP(&gfxbmp);

    if(load_gfx(&gfxbmp.bmp, dpne, NULL, NULL) == _TRUE)
    {
      StrCpy(gfxbmp.dpne, sizeof(_ASCII), dpne, sizeof(_ASCII));
      retcode = AddToMem(&gfxbmp);
      init_GFXBMP(&gfxbmp);
    }

    free_GFXBMP(&gfxbmp);
  }

  return(retcode);
}

_BOOL _Gfx::Out(_ROOM *pr, _ASCII *dpne, _ATTRIB attrib, _BOOL zoom)
{
  _BOOL   retcode = _FALSE;
  _BMP    *bmp;
  _KOORD  x, y;
  _UWORD  width, height;
  _REGION actreg, ioreg=NULL, newreg=NULL;

  if(Load(dpne) == _TRUE)
  {
	 bmp = Get(dpne);
	 if(bmp != NULL)
	 {
    if(zoom == _TRUE)
    {
      out_zoom_BMP(pr, bmp, attrib);
    }
    else
    {
		  width  = bmp->width * (GetXSize_Makro(attrib) + 1);
		  height = bmp->height * (GetYSize_Makro(attrib) + 1);

		  switch(attrib & T_VERTBOUND)
		  {
		    case T_MIDBOUND   : y = (pr->xy2.y + pr->xy1.y - height)/2; break;
		    case T_DOWNBOUND  : y = (pr->xy2.y - height);               break;
		    default           : y = pr->xy1.y;                          break;
		  }

		  switch(attrib & T_HORIZBOUND)
		  {
		    case T_CENTERBOUND : x = (pr->xy2.x + pr->xy1.x - width)/2; break;
		    case T_RIGHTBOUND  : x = (pr->xy2.x - width);               break;
		    default            : x = pr->xy1.x;                         break;
		  }

//		  actreg = ActivateAndRegion(&ioreg, &newreg, pr, attrib);
		  actreg = ActivateAndRegion(&ioreg, &newreg, pr);
	   #ifdef UC_MOUSE
		  _UWORD hide = IfNecHideMouseCursor(pr);
	   #endif
		  Bitmap(x, y, bmp, T_SOLID|T_COPY, DEFCOLOR);
	   #ifdef UC_MOUSE
      IfNecShowMouseCursor(hide);
	   #endif
      DeactivateAndRegion(actreg, &ioreg, &newreg);
    }
		retcode = _TRUE;
	 }
  }

  return(retcode);
}

_BMP* _Gfx::Get(_ASCII *dpne)
{
  _UDWORD index = Scan4File(dpne);
  _BMP    *bmp = NULL;

  if(index < No)
	 bmp = &Ptr[index].bmp;

  return(bmp);
}

// *****************
// *****************

_BOOL _MakeBMP::Start(_UWORD width, _UWORD height)
{
  _DWORD size;
  _BOOL  retcode = _FALSE;

  init_BMP_Makro(&Bmp);

  PosX        = 0;
  MaxSize     = 0;
  State       = _FALSE;
  Recalculate = _FALSE;

  Bmp.width  = width;
  Bmp.height = height;
  Bmp.datano = 0;
  size = (width * height) * sizeof(_BMPDAT);
  if(Memory2((void**)&Bmp.ptr, size) != 0)
  {
    MemSet(Bmp.ptr, 0, size);
    retcode = _TRUE;
    MaxSize = width * height;
    State   = _TRUE;
  }

  return(retcode);
}

_BOOL _MakeBMP::Add(_KOORD x, _KOORD y, _UWORD col)
{
  _BMPDAT *pd;
  _UDWORD offset;

  if(State == _TRUE)
  {
    offset = (y * Bmp.width) + x;
    if(offset < MaxSize)
    {
      pd          = &Bmp.ptr[offset];
      pd->no      = 1;
      pd->col     = col;
      Bmp.datano  = MaxSize;
      Recalculate = _TRUE;
      return _TRUE;
    }
  }

  return _FALSE;
}
/*
_BOOL _MakeBMP::Add2(_KOORD x, _KOORD y, _UWORD col)
{
  _BMPDAT *pd;
  _UDWORD offset;

  if(State == _TRUE)
  {
    offset = Bmp.datano;
    if(offset < MaxSize)
    {
      if((x > 0)&&(offset > 0))
      {
        pd = &Bmp.ptr[offset-1];
        if((pd->col == col)&&(pd->no < 255))
        {
          pd->no += 1;
          return(_TRUE);
        }  
      }  
        
      pd          = &Bmp.ptr[offset];
      pd->no      = 1;
      pd->col     = col;
      Bmp.datano += 1;
      Recalculate = _FALSE;
      return(_TRUE);
    }
  }

  return(_FALSE);
}
*/
_BOOL _MakeBMP::Add(_UWORD col)
{
  _BMPDAT *pd;
  _BOOL   retcode = _FALSE;

  if(State == _TRUE)
  {
    if(PosX < Bmp.width)
    {
      if(Bmp.datano > 0)
      {
        pd = &Bmp.ptr[Bmp.datano-1];
        if((pd->col == col)&&(pd->no < 255))
        {
          pd->no ++;
          retcode = _TRUE;
        }
      }
      PosX ++;
    }
    else
    {
      PosX = 1;
    }

    if((retcode == _FALSE)&&(Bmp.datano < MaxSize))
    {
      pd      = &Bmp.ptr[Bmp.datano];
      pd->no  = 1;
      pd->col = col;
      Bmp.datano ++;
      retcode = _TRUE;
    }
  }
  return(retcode);
}

_BMP *_MakeBMP::Stop(void)
{
  if(Recalculate == _TRUE)
  {
    compress_BMP(&Bmp);
  }
  else if(State == _TRUE)
  {
    Memory2((void**)&Bmp.ptr, Bmp.datano * sizeof(_BMPDAT));
  }
  
  return(&Bmp);
}

void colorCorrection256(_BMP *b, _RGB256 *rgb, _UWORD no)
{
  _UDWORD i;
  _UWORD  index[256];

  if((rgb != NULL)&&(no <= 256))
  {
    for(i=0; i<no; i++)
    {
      index[i] = MakeTrueColor_Makro(rgb[i].cred, rgb[i].cgreen, rgb[i].cblue);
    }

    for(i=0; i<b->datano; i++)
    {
      b->ptr[i].col = index[b->ptr[i].col];
    }
  }
}

void flash_memorize_zoom_gfx(_ROOM *pr, _ASCII *dpne, _BOOL zoom)
{
  Gfx.Out(pr, dpne, T_SOLID|T_COPY, zoom);
}

void flash_memorize_gfx(_KOORD x, _KOORD y, _ASCII *dpne)
{
  _ROOM room;

  MyGetScreenSize(&room);

  room.xy1.x = x;
  room.xy1.y = y;
  Gfx.Out(&room, dpne, T_SOLID|T_COPY, _FALSE);
}

#endif

_BOOL shrink_y16Bit(_BMP *bmp, _UWORD height)
{
  _UDWORD faktor, datano, no, anz, i, begin;
  _UWORD  width;
  _BMPDAT *src;

  faktor = (((_UDWORD)height * 10000)/bmp->height);
  datano = bmp->datano;
  width  = 0;
  begin  = 0;
  src    = bmp->ptr;
  anz    = 10000;

  bmp->datano = 0;
  bmp->height = 0;

  for(i=0; i<datano; i++)
  {
    width += src->no;
    if(width >= bmp->width)
    {
      width = 0;

      if(anz >= 10000)
      {
        anz -= 10000;          // malen
        no   = i - begin + 1;

        if(bmp->datano != begin)
          MemCpy(&bmp->ptr[bmp->datano], &bmp->ptr[begin], no*sizeof(_BMPDAT));
        bmp->datano += no;

        bmp->height ++;
      }

      begin = i + 1;
      anz  += faktor;
    }

    src += 1;
  }

  if(datano != bmp->datano)
    Memory2((void**)&bmp->ptr, bmp->datano * sizeof(_BMPDAT));

  return(_TRUE);
}

_BOOL enlarge_y16Bit(_BMP *bmp, _UWORD height)
{
  _UDWORD i, begin, datano, anz, faktor;
  _UWORD  lastline, cut, width, add;
  _BMPDAT *src;
  _UDWORD maxd, aktd, akth, no;
  _BOOL   retcode = _FALSE;
  _BMPDAT *tmp = NULL;

  faktor = (((_UDWORD)height * 10000)/bmp->height) - 10000;
  datano = bmp->datano;
  width  = 0;
  begin  = 0;
  src    = bmp->ptr;
  anz    = faktor;

  akth = 0;
  maxd = ((datano * height)/ bmp->height);
  aktd = 0;
  if(Memory2((void**)&tmp, maxd * sizeof(_BMPDAT)) != 0)
  {
    lastline = bmp->height;
    for(i=0; i<datano; i++)
    {
      width += src->no;
      if(width >= bmp->width)
      {
        lastline -= 1;
        add       = 0;
        width     = 0;
        if(anz >= 10000)
        {
          add  = (anz / 10000);
          anz -= (add * 10000);
          // duplicate lines
//          for(n=begin; n<=i; n++)
//            bmp->ptr[n].col = LIGHTRED;
        }

        add ++;

        akth += add;

        if(lastline == 0)
        {
          if(akth < height)
          {
            cut   = height - akth;
            add  += cut;
            akth += cut;
          }
        }

        while(add)
        {
          no = (i - begin + 1);
          if((aktd + no) > maxd)
          {
            if(Memory2((void**)&tmp, (aktd + no)*sizeof(_BMPDAT)) == 0)
            {
              Memory2((void**)&tmp, 0);
              return(_FALSE);
            }
            maxd = aktd + no;
          }

          MemCpy(&tmp[aktd], &bmp->ptr[begin], no*sizeof(_BMPDAT));
          aktd += no;
          add  --;
        }

        begin = i + 1;
        anz  += faktor;
      }

      src += 1;
    }

    Memory2((void**)&tmp, aktd*sizeof(_BMPDAT));
    Memory2((void**)&bmp->ptr, 0);
    bmp->ptr    = tmp;
    bmp->datano = aktd;
    bmp->height = akth;

    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL zoom_BMP16Bit(_BMP *dst, _UWORD width, _UWORD height)
{
  _BOOL   retcode = _TRUE;
  _UDWORD faktor, anz;
  _UWORD  y, x, no, owidth, oheight, add, tmp;
  _UWORD  nwidth;
  _BMPDAT *data;
  _UDWORD i, size;

  if(height < dst->height)
    shrink_y16Bit(dst, height);

  owidth  = dst->width;
  oheight = dst->height;
  data    = dst->ptr;

  if(width < owidth) // del pixel
  {
    faktor = (((_UDWORD)width * FAQX) / owidth);
    _BMPDAT *ptx = data;
    _UDWORD nanz = 0;
    
    for(y=0; y<oheight; y++)
    {
      _UDWORD fehler = 0;
      _UDWORD myx = 0;
      nwidth = 0;
      while(nwidth < owidth)    
      {
        nwidth  += data->no;
        anz      = (data->no * faktor) + fehler;
        data->no = (anz / FAQX);
        fehler   = anz - (data->no * FAQX);
        
        if(data->no)
        {
          myx += data->no;
          if(ptx != data)
            *ptx = *data;
          ptx ++;
          nanz ++;
        }
        
        data ++;
      }
      
      if(myx < width)
      {
        ptx--;
        ptx->no += width - myx;
        ptx++;
      }
    }
    
    dst->width = width;
    dst->datano = nanz;
  }
  else if(width > owidth)  // ins pixel
  {
    faktor = (((_UDWORD)width * 10000)/owidth) - 10000;
    i      = 0;
    nwidth = 0;
    
    for(y=0; y<oheight; y++)
    {
      x      = 0;
      anz    = faktor;
      nwidth = owidth;
//      max    = owidth - 1;

      while(x < owidth)
      {
        no  = data->no;
        add = 0;

        while(no)
        {
          no--;
          if(anz >= 10000)
          {
            add += (anz / 10000);
            anz -= ((anz / 10000) * 10000);
//            WritePixel(x, y, T_COPY|T_SOLID, LIGHTRED);
          }
//          else
//          {
//            WritePixel(x, y, T_COPY|T_SOLID, data->col);
//          }
          anz += faktor;
          x ++;
        }

        if(x == owidth)
        {
          tmp = nwidth + add;
          if(tmp < width)
          {
            add += width - tmp;
          }
        }

        if(add)
        {
          nwidth += add;

          if((data->no + add) <= 255)
          {
            data->no += add;
          }
          else
          {
            // servis g'scheft, schen dong
            if(data->no < 255)
            {
              add     -= (255-data->no);
              data->no = 255;
            }

            while(add)
            {
              size = (dst->datano + 1) * sizeof(_BMPDAT);
              if(Memory2((void**)&dst->ptr, size) != 0)
              {
                // davor einen neuen block einfuegen
                size = (dst->datano-i) * sizeof(_BMPDAT);
                MemMove(&dst->ptr[i+1], &dst->ptr[i], size);

                if(add > 255)
                {
                  dst->ptr[i].no = 255;
                  add           -= 255;
                }
                else
                {
                  dst->ptr[i].no = add;
                  add            = 0;
                }

                i           += 1;
                dst->datano += 1;
                data         = &dst->ptr[i];
              }
              else
              {
                retcode = _FALSE;  // bitte funktion verlassen !!!
                goto STRETCH_ERROR;
              }
            }
          }
        }

        data += 1;
        i    += 1;
      }
    }

    dst->width = nwidth;
  }
  
  if(height > dst->height)
    enlarge_y16Bit(dst, height);
 
  STRETCH_ERROR:

  if(retcode == _FALSE)
    free_BMP(dst);

  return(retcode);
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
#ifdef UC_FIXMEMORY
void fixshrink_y16Bit(_BMP *bmp, _UWORD height)
{
  _UDWORD faktor, datano, no, anz, i, begin;
  _UWORD  width;
  _BMPDAT *src;

  faktor = (((_UDWORD)height * 10000)/bmp->height);
  datano = bmp->datano;
  width  = 0;
  begin  = 0;
  src    = bmp->ptr;
  anz    = 10000;

  bmp->datano = 0;
  bmp->height = 0;

  for(i=0; i<datano; i++)
  {
    width += src->no;
    if(width >= bmp->width)
    {
      width = 0;

      if(anz >= 10000)
      {
        anz -= 10000;          // malen
        no   = i - begin + 1;

        if(bmp->datano != begin)
          MemCpy(&bmp->ptr[bmp->datano], &bmp->ptr[begin], no*sizeof(_BMPDAT));
        bmp->datano += no;

        bmp->height ++;
      }

      begin = i + 1;
      anz  += faktor;
    }

    src += 1;
  }
}

void fixenlarge_y16Bit(_BMP *bmp, _UWORD height)
{
  _UDWORD i, begin, datano, anz, faktor;
  _UWORD  lastline, cut, width, add;
  _BMPDAT *src;
  _UDWORD maxd, aktd, akth, no;
  _BMPDAT *tmp = (_BMPDAT*)GetFixMemory();

  faktor = (((_UDWORD)height * 10000)/bmp->height) - 10000;
  datano = bmp->datano;
  width  = 0;
  begin  = 0;
  src    = bmp->ptr;
  anz    = faktor;

  akth = 0;
  maxd = ((datano * height)/ bmp->height);
  aktd = 0;
  
  lastline = bmp->height;
  for(i=0; i<datano; i++)
  {
      width += src->no;
      if(width >= bmp->width)
      {
        lastline -= 1;
        add       = 0;
        width     = 0;
        if(anz >= 10000)
        {
          add  = (anz / 10000);
          anz -= (add * 10000);
          // duplicate lines
//          for(n=begin; n<=i; n++)
//            bmp->ptr[n].col = LIGHTRED;
        }

        add ++;

        akth += add;

        if(lastline == 0)
        {
          if(akth < height)
          {
            cut   = height - akth;
            add  += cut;
            akth += cut;
          }
        }

        no = (i - begin + 1);
        _UDWORD tsz = no*sizeof(_BMPDAT);
        while(add)
        {
          if((aktd + no) > maxd)
            maxd = aktd + no;

          MemCpy(&tmp[aktd], &bmp->ptr[begin], tsz);
          aktd += no;
          add  --;
        }

        begin = i + 1;
        anz  += faktor;
      }

      src += 1;
  }
  
  bmp->ptr    = tmp;
  bmp->datano = aktd;
  bmp->height = akth;
}

_BOOL fixzoom_BMP16Bit(_BMP *dst, _UWORD width, _UWORD height)
{
  _UDWORD faktor, anz;
  _UWORD  y, x, no, owidth, oheight, add, tmp;
  _UWORD  nwidth;
  _BMPDAT *data;
  _UDWORD i, size;

  if(height < dst->height)
    fixshrink_y16Bit(dst, height);

  owidth  = dst->width;
  oheight = dst->height;
  data    = dst->ptr;

  if(width < owidth) // del pixel
  {
    if((dst->datano == dst->height) && (width < 256))
    {
      // spezialfall, in jeder pixelreihe sind alle farben gleich
      for(y=0; y<dst->height; y++)
        dst->ptr[y].no = width;
      dst->width = width;
    }
    else
    {
      faktor = (((_UDWORD)width * FAQX)/owidth);
      _BMPDAT *ptx = data;
      _UDWORD nanz = 0;
    
      for(y=0; y<oheight; y++)
      {
        _UDWORD fehler = 0;
        _UDWORD myx = 0;
        nwidth = 0;
        while(nwidth < owidth)    
        {
          nwidth  += data->no;
          anz      = (data->no * faktor) + fehler;
          data->no = (anz / FAQX);
          fehler   = anz - (data->no * FAQX);
        
          if(data->no)
          {
            myx += data->no;
            if(ptx != data)
              *ptx = *data;
            ptx ++;
            nanz ++;
          }
        
          data ++;
        }
      
        if(myx < width)
        {
          ptx--;
          ptx->no += width - myx;
          ptx++;
        }
      }
    
      dst->width = width;
      dst->datano = nanz;
    }
  }
  else if(width > owidth)  // ins pixel
  {
    faktor = (((_UDWORD)width * 10000)/owidth) - 10000;
    i      = 0;
    nwidth = 0;

    for(y=0; y<oheight; y++)
    {
      x      = 0;
      anz    = faktor;
      nwidth = owidth;
//      max    = owidth - 1;

      while(x < owidth)
      {
        no  = data->no;
        add = 0;

        while(no)
        {
          no--;
          if(anz >= 10000)
          {
            add += (anz / 10000);
            anz -= ((anz / 10000) * 10000);
//            WritePixel(x, y, T_COPY|T_SOLID, LIGHTRED);
          }
//          else
//          {
//            WritePixel(x, y, T_COPY|T_SOLID, data->col);
//          }
          anz += faktor;
          x ++;
        }

        if(x == owidth)
        {
          tmp = nwidth + add;
          if(tmp < width)
          {
            add += width - tmp;
          }
        }

        if(add)
        {
          nwidth += add;

          if((data->no + add) <= 255)
          {
            data->no += add;
          }
          else
          {
            // servis g'scheft, schen dong
            if(data->no < 255)
            {
              add     -= (255-data->no);
              data->no = 255;
            }

            while(add)
            {
              // davor einen neuen block einfuegen
              size = (dst->datano-i) * sizeof(_BMPDAT);
              MemMove(&dst->ptr[i+1], &dst->ptr[i], size);

              if(add > 255)
              {
                dst->ptr[i].no = 255;
                add           -= 255;
              }
              else
              {
                dst->ptr[i].no = add;
                add            = 0;
              }

              i           += 1;
              dst->datano += 1;
              data         = &dst->ptr[i];
            }
          }
        }

        data += 1;
        i    += 1;
      }
    }

    dst->width = nwidth;
  }
  
  if(height > dst->height)
  {
    fixenlarge_y16Bit(dst, height);
  }

  return _TRUE;
}
#endif
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************


#pragma pack(push, 1)
typedef struct { _UBYTE no;
                 _UBYTE col;
               }
_XBMPDAT256;
#pragma pack(pop)

_BOOL shrink_y8Bit(_BMP *bmp, _UWORD height)
{
  _UDWORD     faktor, datano, no, anz, i, begin;
  _UWORD      width;
  _XBMPDAT256 *src;
  _XBMPDAT256 *ptr;

  faktor = (((_UDWORD)height * 10000)/bmp->height);
  datano = bmp->datano;
  width  = 0;
  begin  = 0;
  src    = (_XBMPDAT256*)bmp->ptr;
  ptr    = src;
  anz    = 10000;

  bmp->datano = 0;
  bmp->height = 0;

  for(i=0; i<datano; i++)
  {
    width += src->no;
    if(width >= bmp->width)
    {
      width = 0;

      if(anz >= 10000)
      {
        anz -= 10000;          // malen
        no   = i - begin + 1;

        if(bmp->datano != begin)
          MemCpy(&ptr[bmp->datano], &ptr[begin], no*sizeof(_XBMPDAT256));
        bmp->datano += no;

        bmp->height ++;
      }

      begin = i + 1;
      anz  += faktor;
    }

    src += 1;
  }

  if(datano != bmp->datano)
    Memory2((void**)&bmp->ptr, bmp->datano * sizeof(_XBMPDAT256));

  return(_TRUE);
}

_BOOL enlarge_y8Bit(_BMP *bmp, _UWORD height)
{
  _UDWORD    i, begin, datano, anz, faktor;
  _UWORD     lastline, cut, width, add;
  _XBMPDAT256 *src, *ptr;
  _UDWORD    maxd, aktd, akth, no;
  _BOOL      retcode = _FALSE;
  _XBMPDAT256 *tmp = NULL;

  faktor = (((_UDWORD)height * 10000)/bmp->height) - 10000;
  datano = bmp->datano;
  width  = 0;
  begin  = 0;
  src    = (_XBMPDAT256*)bmp->ptr;
  anz    = faktor;

  akth = 0;
  maxd = ((datano * height)/ bmp->height);
  aktd = 0;
  if(Memory2((void**)&tmp, maxd * sizeof(_XBMPDAT256)) != 0)
  {
    lastline = bmp->height;
    for(i=0; i<datano; i++)
    {
      width += src->no;
      if(width >= bmp->width)
      {
        add   = 0;
        width = 0;
        if(anz >= 10000)
        {
          add  = (anz / 10000);
          anz -= (add * 10000);
          // duplicate lines
//          for(n=begin; n<=i; n++)
//            bmp->ptr[n].col = LIGHTRED;
        }

        add ++;

        if(lastline == 0)
        {
          if(akth < height)
          {
            cut   = height - akth;
            add  += cut;
            akth += cut;
          }
        }

        akth += add;
        while(add)
        {
          no = (i - begin + 1);
          if((aktd + no) > maxd)
          {
            if(Memory2((void**)&tmp, (aktd + no)*sizeof(_XBMPDAT256)) == 0)
            {
              Memory2((void**)&tmp, 0);
              return(_FALSE);
            }
            maxd = aktd + no;
          }

          ptr = (_XBMPDAT256*)bmp->ptr;
          MemCpy(&tmp[aktd], &ptr[begin], no*sizeof(_XBMPDAT256));
          aktd += no;
          add  --;
        }

        begin = i + 1;
        anz  += faktor;
      }

      src += 1;
    }

    Memory2((void**)&tmp, aktd*sizeof(_XBMPDAT256));
    Memory2((void**)&bmp->ptr, 0);
    bmp->ptr    = (_BMPDAT*)tmp;
    bmp->datano = aktd;
    bmp->height = akth;

    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL zoom_BMP8Bit(_BMP *dst, _UWORD width, _UWORD height)
{
  _BOOL      retcode = _TRUE;
  _UDWORD    faktor, anz;
  _UWORD     y, x, no, owidth, oheight, add, tmp;
  _UWORD     nwidth = 0;
  _XBMPDAT256 *data, *ptr;
  _UDWORD    i, size;

  if(height < dst->height)
  {
    shrink_y8Bit(dst, height);
  }
  else if(height > dst->height)
  {
    enlarge_y8Bit(dst, height);
  }

  owidth  = dst->width;
  oheight = dst->height;
  data    = (_XBMPDAT256*)dst->ptr;

  if(width < owidth) // del pixel
  {
    faktor = (((_UDWORD)width * 10000)/owidth);

    for(y=0; y<oheight; y++)
    {
      x      = 0;
      anz    = 10000;
      nwidth = owidth;

      while(x < owidth)
      {
        no = data->no;
        while(no)
        {
          no--;
          if(anz >= 10000)
          {
            anz -= 10000;
//            WritePixel(x, y, T_COPY|T_SOLID, data->col);
          }
          else if(data->no > 0)
          {
            data->no --;
            nwidth --;
//            WritePixel(x, y, T_COPY|T_SOLID, LIGHTRED);
          }
          else
          {
            retcode = _FALSE;
            goto STRETCH_ERROR;
          }
          anz += faktor;
          x ++;
        }
        data += 1;
      }
    }

    dst->width = nwidth;
  }

  else if(width > owidth)  // ins pixel
  {
    faktor = (((_UDWORD)width * 10000)/owidth) - 10000;
    i      = 0;

    for(y=0; y<oheight; y++)
    {
      x      = 0;
      anz    = faktor;
      nwidth = owidth;

      while(x < owidth)
      {
        no  = data->no;
        add = 0;

        while(no)
        {
          no--;
          if(anz >= 10000)
          {
            add += (anz / 10000);
            anz -= ((anz / 10000) * 10000);
//            WritePixel(x, y, T_COPY|T_SOLID, LIGHTRED);
          }
//          else
//          {
//            WritePixel(x, y, T_COPY|T_SOLID, data->col);
//          }
          anz += faktor;
          x ++;
        }

        if(x == owidth)
        {
          tmp = nwidth + add;
          if(tmp < width)
          {
            add += width - tmp;
          }
        }

        if(add)
        {
          nwidth += add;
          if((data->no + add) <= 255)
          {
            data->no += add;
          }
          else
          {
            // servis g'scheft, schen dong
            if(data->no < 255)
            {
              add     -= (255-data->no);
              data->no = 255;
            }

            while(add)
            {
              size = (dst->datano + 1) * sizeof(_XBMPDAT256);
              if(Memory2((void**)&dst->ptr, size) != 0)
              {
                // davor einen neuen block einfuegen
                size = (dst->datano-i) * sizeof(_XBMPDAT256);
                ptr  = (_XBMPDAT256*)dst->ptr;
                MemMove(&ptr[i+1], &ptr[i], size);

                if(add > 255)
                {
                  ptr[i].no = 255;
                  add      -= 255;
                }
                else
                {
                  ptr[i].no = add;
                  add       = 0;
                }

                i           += 1;
                dst->datano += 1;
                data         = &ptr[i];
              }
              else
              {
                retcode = _FALSE;  // bitte funktion verlassen !!!
                goto STRETCH_ERROR;
              }
            }
          }
        }

        data += 1;
        i    += 1;
      }
    }

    dst->width = nwidth;
  }

  STRETCH_ERROR:

  if(retcode == _FALSE)
    free_BMP(dst);

  return(retcode);
}


_BOOL zoom_BMP(_BMP *dst, _UWORD width, _UWORD height)
{
  _BOOL retcode = _TRUE;

  if(dst != NULL)
  {
    if((width > 0) && (height > 0) && (dst->width > 0) && (dst->height > 0))
    {
      if((dst->state & 8) == 0) //BMP_256COLOR) == 0)
        retcode = zoom_BMP16Bit(dst, width, height);
      else
        retcode = zoom_BMP8Bit(dst, width, height);
    }
    else
    {
      free_BMP(dst);
    }
  }

  return(retcode);
}

void compress_BMP(_BMP *bmp)
{
  _BMPDAT *ps, *pd;
  _UDWORD x, y;
  _UDWORD datano = 0;

  if((bmp->datano > 0) && (bmp->width > 1))
  {
    pd =
    ps = &bmp->ptr[0];

    //for(y=0; y<bmp->height; y++)
    y = bmp->height;
    while(y)
    {
      y --;
      *pd    = *ps;
      datano ++;
      ps     ++;

      //for(x=1; x<bmp->width; x++)
      x = bmp->width - 1;
      while(x)
      {
        x --;
        if(pd->col == ps->col)
        {
          //no = pd->no + ps->no;
          if(pd->no < 255)
          {
            pd->no ++; //= no;
          }
          else
          {
            datano ++;
            pd ++;
            *pd = *ps;
          }
        }
        else
        {
          datano ++;
          pd ++;
          *pd = *ps;
        }

        ps++;
      }
      pd++;
    }

    bmp->datano = datano;
    Memory2((void**)&bmp->ptr, datano * sizeof(_BMPDAT));
  }
}

/*
void compress_BMP(_BMP *bmp)
{
  _BMPDAT *pr, *pw;
	_UWORD  no, x, y;
	_UDWORD datano = 0;
	_BOOL   state;

	if(bmp->datano > 0)
	{
		pw =
		pr = &bmp->ptr[0];

		for(y=0; y<bmp->height; y++)
		{
			datano++;
			for(x=0; x<bmp->width; x++)
			{
				if(x > 0)
				{
					state = _FALSE;
					if(pw->col == pr->col)
					{
						no = pw->no + pr->no;
						if(no <= 255)
						{
							pw->no = no;
							state  = _TRUE;
						}
					}

					if(state == _FALSE)
					{
						pw++;
						*pw = *pr;
						datano++;
					}
				}
				pr++;
			}
			pw++;
			*pw = *pr;
		}

    bmp->datano = datano;
    Memory2((void**)&bmp->ptr, datano * sizeof(_BMPDAT));
  }
}
*/

_BOOL raw_to_BMP(_BMP *bmp, _UBYTE *raw, _UWORD xx, _UWORD yy)
{
  _BOOL retcode = _FALSE;
  _UWORD x, y, r, g, b;
  _BMP dest;


  free_BMP(bmp);

  if(Memory2((void**)&bmp->ptr, xx * yy * sizeof(_BMPDAT)) != 0)
  {
    retcode = _TRUE;

    dest = *bmp;
    for(x=0; x<xx; x++)
    {
      for(y=0; y<yy; y++)
      {
        r = (raw[0] >> 3);
        g = (raw[1] >> 2);
	      b = (raw[2] >> 3);
        dest.ptr->no = 1;
        dest.ptr->col = (r<<11)|(g<<5)|b;
        dest.ptr++;
        raw += 3;
      }
    }

    bmp->datano = xx * yy;
    bmp->width  = xx;
    bmp->height = yy;
    bmp->state  = (BMP_HICOLOR | BMP_NO_TRANSPARENT);

    compress_BMP(bmp);
  }

  return(retcode);
}

_UWORD get_pixel(_BMP *bmp, _UWORD x, _UWORD y)
{
  _UWORD retcode = 0;
  _UDWORD pixel;
  _UDWORD pixelno;
  _UDWORD i;
  _BMPDAT *bmpdat;


  bmpdat = bmp->ptr;

  pixelno = 0;

  if((bmp->state & BMP_TURN) == BMP_TURN)
    y = (bmp->height - 1) - y;

  pixel = (bmp->width * y) + x;

  if((x < bmp->width) && (y < bmp->height))
  {
    for(i=0; i<bmp->datano; i++)
    {
      pixelno += bmpdat->no;
      if(pixelno > pixel)
      {
        retcode = bmpdat->col;
        break;
      }

      bmpdat++;
    }
  }

  return(retcode);
}

_BOOL get_pixel_line(_UWORD *buffer, _BMP *bmp, _UWORD y)
{
  _BOOL   retcode = _FALSE;
  _UWORD  retno, j;
  _UDWORD pixel;
  _UDWORD pixelno;
  _UDWORD i;
  _BMPDAT *bmpdat;


  bmpdat = bmp->ptr;

  retno = 0;
  pixelno = 0;

  if((bmp->state & BMP_TURN) == BMP_TURN)
    y = (bmp->height - 1) - y;

  pixel = bmp->width * y;

  if(y < bmp->height)
  {
    for(i=0; (i<bmp->datano)&&(retcode==_FALSE); i++)
    {
      pixelno += bmpdat->no;
      if(pixelno > pixel)
      {
        for(j=0; j<bmpdat->no; j++)
        {
          buffer[retno] = bmpdat->col;
          retno++;
          if(retno >= bmp->width)
          {
            retcode = _TRUE;
            break;
          }
        }
      }

      bmpdat++;
    }
  }

  return(retcode);
}

_BOOL gfxerror_set(_UWORD *p_errorcode, _UWORD code)
{
  if(p_errorcode != NULL)
  {
    if((*p_errorcode == GFXERROR_NONE)||(code == GFXERROR_NONE))
      *p_errorcode = code;
  }

  return(_FALSE);
}

extern "C" void transparent_BMP(_BMP *bmp, _UBYTE r, _UBYTE g, _UBYTE b)
{
  bmp->transparent = MakeTrueColor(r, g, b);
  bmp->state       = bmp->state & (~BMP_NO_TRANSPARENT);
}

extern "C" void invert_BMP(_BMP *bmp)
{
  _BMPDAT *pc;
  _UDWORD i;
  _UBYTE cr, cg, cb;

  if((bmp->state & BMP_NO_TRANSPARENT) == 0)
  {
    cr               = 31 - ((bmp->transparent >> 11) & 0x001F);
    cg               = 63 - ((bmp->transparent >> 5) & 0x003F);
    cb               = 31 - (bmp->transparent & 0x001F) ;
    bmp->transparent = (cb << 11) | (cg << 5) | cr;
  }
  
  for(i=0, pc=bmp->ptr; i<bmp->datano; i++, pc++)
  {
    cr = 31 - ((pc->col >> 11) & 0x001F);
    cg = 63 - ((pc->col >> 5) & 0x003F);
    cb = 31 - (pc->col & 0x001F) ;
    pc->col = (cb << 11) | (cg << 5) | cr;
  }
}

_BOOL grab_bmp(_BMP *pd, _KOORD x0, _KOORD y0, _KOORD x1, _KOORD y1)
{
  if(x1 < x0) 
  { 
    _KOORD memo = x1;
    x1 = x0;
    x0 = memo;
  }
  if(y1 < y0)
  {
    _KOORD memo = y1;
    y1 = y0;
    y0 = memo;
  }
  
  unsigned long nox = (x1 - x0 + 1) * (y1 - y0 + 1);
  _BMPDAT *pix = NULL;

  if(Memory2((void**)&pix, nox * sizeof(_BMPDAT)) != 0)
  {
    _BMPDAT *ph = pix; 
   #ifdef UC_MOUSE
    HideMouseCursor();
   #endif
    ReadPixelStart(x0, y0, x1, y1);

    for(_KOORD y=y0; y<=y1; y++)
    {
      for(_KOORD x=x0; x<=x1; x++)
      {
        ph->col = ReadPixel(x, y);
        ph->no  = 1;
        ph++;
      }
    }

    ReadPixelStop();
   #ifdef UC_MOUSE
    ShowMouseCursor();
   #endif

    Memory2((void**)&pd->ptr, 0);
    pd->width       = x1 - x0 + 1;
    pd->height      = y1 - y0 + 1;
    pd->transparent = INVISIBLE;
    pd->state       = 4;
    pd->datano      = nox;
    pd->ptr         = pix;

    return _TRUE;
  }

  return _FALSE;
}

