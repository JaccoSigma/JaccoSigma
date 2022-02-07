#include "T_IPC.H"
#include "T_NUC.H"

#ifdef UC_IPC
_BOOL load_bmo(_BMP*, _ASCII*, _UWORD*);
_BOOL save_bmo(_ASCII*, _KOORD, _KOORD, _KOORD, _KOORD);
_BOOL load_bmp(_BMP*, _ASCII*, _UWORD*, _FUDWORD_UDWORD_UDWORD);

_BOOL save_bmp(_ASCII *dpne, _KOORD x1, _KOORD y1, _KOORD x2, _KOORD y2, _BOOL hito256)
{
  _File    File;
  _BOOL    retcode = _FALSE;
  _BMPINFO bmpinfo;
  _BMPHEAD bmphead;
  _UWORD   x, y, i, nocol;
  _RGB256  pal[256];
  _UDWORD  linesize;
  _UBYTE   *pld, *paldat = NULL;
  _UBYTE   *pixdat = NULL;
  _COLOR   col;
  _UBYTE   dummy;
  _ASCII   rez[_MAX_PATHLENGTH];

  nocol = GetNoColors() + 1;

  bmphead.label  = 19778;                                    // 'BM' fix
  bmphead.nc1    = 0;                                           // 0 fix
  bmphead.nc2    = 0;                                           // 0 fix
  bmphead.offs   = 54; //sizeof(_BMPINFO) + sizeof(_BMPHEAD);  // off. pixelarea (ref. filestart)
  bmpinfo.bitcnt = 24;                       // number of bits per pixel

 #ifdef UC_HICOLOR
  if(hito256 == _TRUE)
    nocol = 256;
 #endif

	if(nocol == 256)
	{
		bmphead.offs   += 256 * 4;                      // adding palette data
		bmpinfo.bitcnt  = 8;                       // number of bits per pixel
		bmpinfo.nc5     = 256;
	}
	else
  {
		bmpinfo.nc5 = 0;
  }

	bmpinfo.width  = x2 - x1 + 1;      // bitmapwidth in pixel
	bmpinfo.height = y2 - y1 + 1;      // bitmapheight in pixel
	linesize       = (((_UDWORD)bmpinfo.width * (_UDWORD)bmpinfo.bitcnt + 31) / 32) * 4;
	bmphead.fsize  = linesize * bmpinfo.height + bmphead.offs;         // filesize
	bmpinfo.size   = 40;                       // lenght of bitmapheaderinfo
	bmpinfo.planes = 1;                                             // 1 fix
	bmpinfo.compr  = 0;                           // (0) type of compression
	bmpinfo.sizeim = 0;                                 // pic.size in bytes
	bmpinfo.nc3    = 0;                                             // 0 fix
	bmpinfo.nc4    = 0;                                             // 0 fix
	bmpinfo.nc6    = 0;                                             // 0 fix

  if(File.Open(SAVE, MakeRamFile(rez, dpne), bmphead.fsize) == _TRUE)
  {
    File.Write(&bmphead.label,  sizeof(_UWORD));    // ('BM')
    File.Write(&bmphead.fsize,  sizeof(_UDWORD));   // filesize
    File.Write(&bmphead.nc1,    sizeof(_UWORD));    // (0)
    File.Write(&bmphead.nc2,    sizeof(_UWORD));    // (0)
    File.Write(&bmphead.offs,   sizeof(_UDWORD));   // offset of bitpattern (reference filestart)

    File.Write(&bmpinfo.size,   sizeof(_UDWORD));   // length of bitpatterninfo
    File.Write(&bmpinfo.width,  sizeof(_UDWORD));   // Bitmapbreite in Pixel
    File.Write(&bmpinfo.height, sizeof(_UDWORD));   // Bitmaphoehe in Pixel
    File.Write(&bmpinfo.planes, sizeof(_UWORD));    // (1)
    File.Write(&bmpinfo.bitcnt, sizeof(_UWORD));    // (4) Zahl der Bits pro Pixel
    File.Write(&bmpinfo.compr,  sizeof(_UDWORD));   // (0) Art der komprimierung
    File.Write(&bmpinfo.sizeim, sizeof(_UDWORD));   // Bildgroesse in Bytes
    File.Write(&bmpinfo.nc3,    sizeof(_UDWORD));
    File.Write(&bmpinfo.nc4,    sizeof(_UDWORD));
    File.Write(&bmpinfo.nc5,    sizeof(_UDWORD));
    File.Write(&bmpinfo.nc6,    sizeof(_UDWORD));

	 #ifdef UC_MOUSE
    HideMouseCursor();
	 #endif
    ReadPixelStart(x1, y1, x2, y2);

    // data
    y = bmpinfo.height;
    if(nocol == 256)
    {
      dummy = 0;
      Palette.Get((_UBYTE*)pal, 0, 256);
      for(i=0; i<256; i++)
      {
        File.Write(&pal[i].cblue,  sizeof(_UBYTE));
        File.Write(&pal[i].cgreen, sizeof(_UBYTE));
        File.Write(&pal[i].cred,   sizeof(_UBYTE));
        File.Write(&dummy,         sizeof(_UBYTE));
      }

			if(Memory2((void**)&pixdat, linesize) != 0)
			{
				retcode = _TRUE;
				while(y > 0)  // bottom -> top
				{
					y--;
					MemSet(pixdat, 0, linesize);
				 #ifdef UC_HICOLOR
					if(hito256 == _TRUE)
					{
						for(x=0; x<bmpinfo.width; x++)  // left -> right
							pixdat[x] = MakeIndexColor(ReadPixel(x1 + x, y1 + y), 256, (_UBYTE*)pal);
					}
					else
				 #endif
					{
						for(x=0; x<bmpinfo.width; x++)  // left -> right
							pixdat[x] = (_UBYTE)ReadPixel(x1 + x, y1 + y);
					}

					File.Write(pixdat, linesize);
				}

				Memory2((void**)&pixdat, 0);                             // free mem
			}
		}
	 #ifdef UC_HICOLOR
		else  // 24 bit
		{
      if(Memory2((void**)&paldat, linesize) != 0)
      {
        MemSet(paldat, 0, linesize);
        retcode = _TRUE;
        while(y > 0) // bottom to top
        {
          y--;
          pld = paldat;
          for(x=0; x<bmpinfo.width; x++)                    // left to right
          {
            col = ReadPixel(x1 + x, y1 + y);
            *pld = ((col & 0x1F) << 3);         pld++;
            *pld = (((col >> 5) & 0x3F) << 2);  pld++;
            *pld = (((col >> 11) & 0x1F) << 3); pld++;
          }
          File.Write(paldat, linesize);
				}
			}
			Memory2((void**)&paldat, 0);                             // free mem
		}
	 #endif

    ReadPixelStop();
	 #ifdef UC_MOUSE
		ShowMouseCursor();
   #endif

    if(File.GetError() == _TRUE)
      retcode = _FALSE;

    if(File.Close() == _FALSE)
      retcode = _FALSE;
  }

  return(retcode);
}

_UDWORD convBMPRLE32(_BMPDAT *dst, _RGB32 *src, _UWORD pix)
{
  _UDWORD rlecnt  = 0, i;
  _UWORD  col;

  dst[rlecnt].no  = 1;
  dst[rlecnt].col = MakeTrueColor_Makro(src->cred, src->cgreen, src->cblue);

  for(i=1; i<pix; i++)
  {
    src += 1;
    col = MakeTrueColor_Makro(src->cred, src->cgreen, src->cblue);

    if(col != dst[rlecnt].col)
    {
       rlecnt         += 1;
       dst[rlecnt].no  = 1;
       dst[rlecnt].col = col;
    }
    else
    {
      if(dst[rlecnt].no == 255)
      {
        rlecnt         += 1;
        dst[rlecnt].no  = 0;
        dst[rlecnt].col = col;
      }
      dst[rlecnt].no += 1;
    }
  }

  rlecnt += 1;

  return(rlecnt);
}

_UDWORD convBMPRLE24(_BMPDAT *dst, _RGB24 *src, _UWORD pix)
{
  _UDWORD rlecnt  = 0, i;
  _UWORD  col;

  dst[rlecnt].no  = 1;
  dst[rlecnt].col = MakeTrueColor_Makro(src->cred, src->cgreen, src->cblue);

  for(i=1; i<pix; i++)
  {
    src += 1;
    col = MakeTrueColor_Makro(src->cred, src->cgreen, src->cblue);

    if(col != dst[rlecnt].col)
    {
       rlecnt         += 1;
       dst[rlecnt].no  = 1;
       dst[rlecnt].col = col;
    }
    else
    {
      if(dst[rlecnt].no == 255)
      {
        rlecnt         += 1;
        dst[rlecnt].no  = 0;
        dst[rlecnt].col = col;
      }
      dst[rlecnt].no += 1;
    }
  }

  rlecnt += 1;

  return(rlecnt);
}

_UDWORD convBMPRLE16(_BMPDAT *dst, _UWORD *src, _UWORD pix)
{
  _UDWORD rlecnt  = 0, i;
  _UWORD  col;

  // 0x7C00 ... Rot
  // 0x03E0 ... Grün
  // 0x001F ... Blau

  dst[rlecnt].no  = 1;
  dst[rlecnt].col = ((*src<<1) & 0xFFE0) | (*src & 0x1F);

  for(i=1; i<pix; i++)
  {
    src += 1;
    col = ((*src<<1) & 0xFFE0) | (*src & 0x1F);

    if(col != dst[rlecnt].col)
    {
       rlecnt         += 1;
       dst[rlecnt].no  = 1;
       dst[rlecnt].col = col;
    }
    else
    {
      if(dst[rlecnt].no == 255)
      {
        rlecnt         += 1;
        dst[rlecnt].no  = 0;
        dst[rlecnt].col = col;
      }
      dst[rlecnt].no += 1;
    }
  }

  rlecnt += 1;

  return(rlecnt);
}

_UDWORD convBMPRLE8(_BMPDAT *dst, _UBYTE *src, _UWORD pix)
{
  _UDWORD rlecnt  = 0, i;


  dst[rlecnt].no  =  1;
  dst[rlecnt].col =  *src;

  for(i=1; i<pix; i++)
  {
    if(src[i]!=dst[rlecnt].col)
    {
       rlecnt          += 1;
       dst[rlecnt].no  =  1;
       dst[rlecnt].col =  src[i];
    }
    else
    {
      if(dst[rlecnt].no == 255)
      {
        rlecnt += 1;
        dst[rlecnt].no  =  0;
        dst[rlecnt].col =  src[i];
      }
      dst[rlecnt].no += 1;
    }
  }
  rlecnt += 1;
  return(rlecnt);
}

_UDWORD convBMPRLE4(_BMPDAT *dst, _UBYTE *src, _UWORD pix)
{
  _UDWORD i , rlecnt = 0;
  _UBYTE  xx;

  dst[rlecnt].no  =  1;
  dst[rlecnt].col =  (*src) & 0x0F;

  for(i=1; i<pix; i++)
  {
    xx = ((i & 1) == 0)? ((src[(i/2)] & 0xF0) >> 4) : (src[(i/2)] & 0x0F);

    if(xx != dst[rlecnt].col)
    {
      rlecnt          += 1;
      dst[rlecnt].no  =  1;
      dst[rlecnt].col =  xx;
    }
    else
    {
      if(dst[rlecnt].no == 255)
      {
        rlecnt += 1;
        dst[rlecnt].no  =  0;
        dst[rlecnt].col =  xx;
      }
      dst[rlecnt].no += 1;
    }
  }
  rlecnt += 1;
  return(rlecnt);
}

_UDWORD convBMPRLE1(_BMPDAT *dst, _UBYTE *src, _UWORD pix)
{
  _UDWORD i, rlecnt = 1;
  _UBYTE  col;
  _UWORD  bit;

  dst->no  =  1;
  dst->col =  ((*src) & 128)? 1 : 0;
  pix     -= 1;

  for(i=0,bit=0; i<pix; i++)
  {
    bit += 1;
    if(bit == 8)
    {
      src += 1;
      bit =  0;
    }

    switch(bit)
    {
      case 0 : col = ((*src) & 128)? 1 : 0; break;
      case 1 : col = ((*src) &  64)? 1 : 0; break;
      case 2 : col = ((*src) &  32)? 1 : 0; break;
      case 3 : col = ((*src) &  16)? 1 : 0; break;
      case 4 : col = ((*src) &   8)? 1 : 0; break;
      case 5 : col = ((*src) &   4)? 1 : 0; break;
      case 6 : col = ((*src) &   2)? 1 : 0; break;
      default: col = ((*src) &   1)? 1 : 0; break;
    }

    if(col != dst->col)
    {
      rlecnt  += 1;
      dst     += 1;
      dst->no  = 1;
      dst->col = col;
    }
    else
    {
      if(dst->no == 255)
      {
        rlecnt  += 1;
        dst     += 1;
        dst->no  = 0;
        dst->col = col;
      }

      dst->no += 1;
    }
  }

  return(rlecnt);
}

void colorCorrection(_BMP *b, _RGBBMP *bmprgb, _UWORD no)
{
  _UDWORD i;
  _UWORD  index[256];

  if((bmprgb != NULL) && (no > 0) && (no <= 256))
  {
    for(i=0; i<no; i++)
    {
      index[i] = MakeTrueColor_Makro(bmprgb[i].cred, bmprgb[i].cgreen, bmprgb[i].cblue);
    }
    
    for(i=0; i<b->datano; i++)
    {
      b->ptr[i].col = index[b->ptr[i].col];
    }
  }
}

_BOOL load_bmp(_BMP *bmp, _ASCII *dpne, _UWORD *p_errorcode, _FUDWORD_UDWORD_UDWORD fp_callback)
{
  _File    File;
  _BOOL    retcode = _FALSE;
  _BMPHEAD bmphead;
  _BMPINFO bmpinfo;
  _UDWORD  size, line, rlecnt = 0;
  _UWORD   i;
  _UBYTE   *src = NULL;
  _RGBBMP  *bmprgb = NULL;
  _UWORD   rgbno;
  _FUDWORD_PT_PT_UWORD fptr;

  init_BMP_Makro(bmp);

  gfxerror_set(p_errorcode, GFXERROR_NONE);

  if(File.Open(LOAD, dpne)==_TRUE)
  {
    if(File.Read((void*)&bmphead, sizeof(_BMPHEAD)) == _TRUE)
    {
      if((MemCmp(&bmphead.label, "BM", 2) == 0) && (bmphead.nc1 == 0) && (bmphead.nc2 == 0))
      {
        if(File.Read((void*)&bmpinfo, sizeof(_BMPINFO)) == _TRUE)
        {
          if((bmpinfo.compr == 0) &&
             ((bmpinfo.bitcnt==1)||(bmpinfo.bitcnt==4)||(bmpinfo.bitcnt==8)||(bmpinfo.bitcnt==16)||(bmpinfo.bitcnt==24)||(bmpinfo.bitcnt==32)))
          {
            bmp->state  = BMP_TURN|BMP_NO_TRANSPARENT;
           #ifdef HICOLOR
            bmp->state |= BMP_HICOLOR;
           #endif
            bmp->width  = bmpinfo.width;
            bmp->height = bmpinfo.height;
            size        = bmp->width * bmp->height * sizeof(_BMPDAT);

            if(Memory2((void**)&bmp->ptr, size) != 0)
            {
              line = ((bmp->width * bmpinfo.bitcnt) + 31) / 32 * 4;       // byte
              if(Memory2((void**)&src, line) != 0)
              {
                switch(bmpinfo.bitcnt)
                {
                  case  1: size =   2 * sizeof(_RGBBMP); rgbno =   2; break;
                  case  4: size =  16 * sizeof(_RGBBMP); rgbno =  16; break;
                  case  8: size = 256 * sizeof(_RGBBMP); rgbno = 256; break;
                  case 16: size =   0; rgbno = 0; break;
                  case 32: size =   0; rgbno = 0; break;
                  default: size =   0; rgbno = 0; break;
                }

                if(Memory2((void**)&bmprgb, size) != 0)
                {
                  if(File.Read((void*)bmprgb, size)==_TRUE)
                  {
                    retcode = _TRUE;
                    switch(bmpinfo.bitcnt)
                    {
                      case  1: fptr = (_FUDWORD_PT_PT_UWORD)convBMPRLE1;  break;
                      case  4: fptr = (_FUDWORD_PT_PT_UWORD)convBMPRLE4;  break;
                      case  8: fptr = (_FUDWORD_PT_PT_UWORD)convBMPRLE8;  break;
                      case 16: fptr = (_FUDWORD_PT_PT_UWORD)convBMPRLE16; break;
                      case 32: fptr = (_FUDWORD_PT_PT_UWORD)convBMPRLE32; 
                               File.Seek(bmphead.offs, SEEK_SET); // ggf.sinnlose info überspringen
                               break;
                      default: fptr = (_FUDWORD_PT_PT_UWORD)convBMPRLE24; break;
                    }

                    if(fp_callback == NULL)
                    {
                      for(i=0; i<bmp->height; i++)
                      {
                        if(File.Read((void*)src, line)==_TRUE)
                        {
                          rlecnt += fptr(&bmp->ptr[rlecnt], src, bmp->width);
                        }
                        else
                        {
                          retcode = _FALSE;
                          break;
                        }
                      }
                    }
                    else
                    {
                      for(i=0; i<bmp->height; i++)
                      {
                        if(File.Read((void*)src, line)==_TRUE)
                        {
                          rlecnt += fptr(&bmp->ptr[rlecnt], src, bmp->width);
                        }
                        else
                        {
                          retcode = _FALSE;
                          break;
                        }

                        if(fp_callback(i, bmp->height) == 0)
                        {
                          retcode = _FALSE;
                          gfxerror_set(p_errorcode, GFXERROR_USERBREAK);
                          break;
                        }
                      }
                    }

                    if(retcode==_TRUE)
                    {
                      bmp->datano = rlecnt;
                      size = rlecnt * sizeof(_BMPDAT);
                      if(Memory2((void**)&bmp->ptr, size) != 0)     // cut RLE memory
                      {
                        if(bmpinfo.bitcnt <= 8)
                          colorCorrection(bmp, bmprgb, rgbno);
                      }
                    }
                  }
                }
                else
                {
                  gfxerror_set(p_errorcode, GFXERROR_TOOLESSMEMORY);
                }
              }
              else
              {
                gfxerror_set(p_errorcode, GFXERROR_TOOLESSMEMORY);
              }
            }
          }
        }
      }
    }

    if(File.GetError() == _TRUE)
    {
      gfxerror_set(p_errorcode, GFXERROR_INVALIDFILE);
    }
  }
  else
  {
    gfxerror_set(p_errorcode, GFXERROR_CANTOPEN);
  }

  File.Close();

  if(retcode == _FALSE)
  {
    free_BMP(bmp);
  }

  Memory2((void**)&bmprgb, 0);
  Memory2((void**)&src, 0);

  return(retcode);
}

_BOOL save_bmo(_ASCII *dpne, _KOORD x1, _KOORD y1, _KOORD x2, _KOORD y2)
{
  // [][] ........ id "MP"
  // [][][][] .... no of data
  // [][] ........ width
  // [][] ........ height
  // [][] ........ transparent
  // [][] ........ state
  // [ ........... [],[][] ... no, color

  _BOOL   retcode = _FALSE;
  _BMP    bmp;
  _BMPDAT dat;
  _UWORD  x, y, yy, col;
  _File   File;
 #ifndef UC_HICOLOR
  _RGB256 pal[256];
  _UWORD  rgb[256];
 #endif
  _ASCII  rez[_MAX_PATHLENGTH];

  if(File.Open(SAVE, MakeRamFile(rez, dpne)) == _TRUE)
  {
    init_BMP_Makro(&bmp);

   #ifndef UC_HICOLOR
    Palette.Get((_UBYTE*)pal, 0, 256);
    for(x=0; x<256; x++)
      rgb[x] = MakeTrueColor(pal[x].cred, pal[x].cgreen, pal[x].cblue);
   #endif

    bmp.datano = 0;
    bmp.width  = x2 - x1 + 1;
    bmp.height = y2 - y1 + 1;
    bmp.state  = BMP_NO_TRANSPARENT|BMP_TURN;

    x = 'M';
    File.Write(&x, sizeof(_UBYTE));
    x = 'P';
    File.Write(&x, sizeof(_UBYTE));

    File.Write(&bmp.datano, sizeof(_UDWORD));
    File.Write(&bmp.width, sizeof(_UWORD));
    File.Write(&bmp.height, sizeof(_UWORD));
    File.Write(&bmp.transparent, sizeof(_UWORD));
    File.Write(&bmp.state, sizeof(_UWORD));

	 #ifdef UC_MOUSE
		HideMouseCursor();
	 #endif
    ReadPixelStart(x1, y1, x2, y2);

    y = y2;
    for(yy=y1; yy<=y2; yy++)
    {
      dat.col = ReadPixel(x1, y);
      dat.no  = 1;

      for(x=x1+1; x<=x2; x++)
      {
        col = ReadPixel(x, y);
        if((col == dat.col)&&(dat.no < 255))
        {
          dat.no ++;
        }
        else
        {
          File.Write(&dat.no, sizeof(_UBYTE));
         #ifdef UC_HICOLOR
          File.Write(&dat.col, sizeof(_UWORD));
         #else
          File.Write(&rgb[dat.col], sizeof(_UWORD));
         #endif
          dat.col    = col;
          dat.no     = 1;
          bmp.datano ++;
        }
      }
      File.Write(&dat.no, sizeof(_UBYTE));
     #ifdef UC_HICOLOR
      File.Write(&dat.col, sizeof(_UWORD));
     #else
      File.Write(&rgb[dat.col], sizeof(_UWORD));
     #endif
      bmp.datano ++;
      y--;
    }
    
    ReadPixelStop();
	 #ifdef UC_MOUSE
		ShowMouseCursor();
	 #endif

    File.Seek(2, SEEK_SET);
    File.Write(&bmp.datano, sizeof(_UDWORD));

    if(File.GetError() == _FALSE)
      retcode = _TRUE;

    if(File.Close() == _FALSE)
      retcode = _FALSE;
  }

  return(retcode);
}

_BOOL load_bmo(_BMP *bmp, _ASCII *dpne, _UWORD *p_errorcode)
{
  _File  File;
  _BOOL  retcode = _FALSE;
  _UBYTE tmp[2];

  gfxerror_set(p_errorcode, GFXERROR_NONE);

  init_BMP_Makro(bmp);

  if(File.Open(LOAD, dpne)==_TRUE)
  {
    File.Read(tmp, sizeof(_UWORD));
    if((tmp[0] == 'M')&&(tmp[1] == 'P'))
      retcode = File.Read_BMP(bmp);
    else
      gfxerror_set(p_errorcode, GFXERROR_INVALIDFILE);
  }
  else
  {
    gfxerror_set(p_errorcode, GFXERROR_CANTOPEN);
  }

  if(File.GetError() == _TRUE)
    gfxerror_set(p_errorcode, GFXERROR_INVALIDFILE);

  File.Close();

  if(retcode == _FALSE)
    free_BMP(bmp);

  return(retcode);
}
#endif
