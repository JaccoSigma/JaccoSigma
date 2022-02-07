//<SigmatekCFileOptimize/>
#include "T_IPC.H"
#include "T_NUC.H"

#ifdef UC_THEME

#define SA41991

void _Theme::Init(void)
{
  IsAction = _FALSE;
  init_MEM_THEME(&MemTheme);
  MemSet(GuidPost, 0xFF, sizeof(GuidPost));
}

void _Theme::Free(void)
{
  free_MEM_THEME(&MemTheme);
  Init();
}

_BOOL _Theme::Load(_ASCII *dpne)
{
  _File File;
  _BOOL retcode = _FALSE;

  File.Open(LOAD, dpne);
  if(File.ReadHeader() == _TRUE)
  {
    retcode = Read(&File);
  }
  File.Close();

  return(retcode);
}

void _Theme::MakeGuidPost(void)
{
  _THEME *ptr;
  _UWORD i;

  MemSet(GuidPost, 0xFF, sizeof(GuidPost));

  for(i=0,ptr=MemTheme.ptr; i<MemTheme.no; i++,ptr++)
  {
    if(ptr->id < 256)
      GuidPost[ptr->id] = i;
  }
}

_BOOL is_cancle(_THEME *pth)
{
  _BOOL retcode = _FALSE;

  if(pth != NULL)
  {
    if((pth->xpos_cx != -1)&&(pth->ypos_cx != -1))
      retcode = _TRUE;
  }

  return(retcode);
}

_BOOL _Theme::Read(_File *File)
{
  _BOOL   retcode = _FALSE;
  _UDWORD i;
  _THEME  *pth;

  Init();

  if(File->GetError() == _FALSE)
  {
    retcode = _TRUE;
    File->Read_MEM_THEME(&MemTheme);

    MakeGuidPost();
  }

  if(File->GetError() == _TRUE)
  {
    Free();
    retcode = _FALSE;
  }
  else
  {
    // laden von label aus geschwindigkeitsgründen eingespart
    // _AsciiPuffer LseLabel;
    // LseLabel.Init();
    // LseLabel.Read(File, _FALSE);
    // LseLabel.Free();
  
    pth = MemTheme.ptr;
    for(i=0; i<MemTheme.no; i++)
    {
      if(is_cancle(pth) == _TRUE)
        IsAction = _TRUE;
      pth += 1;
    }
  }

  return(retcode);
}

_UDWORD _Theme::GetNo(void)
{
  return(MemTheme.no);
}

#ifdef UC_IPC
_UDWORD memsize_FRAMEWORK(_FRAMEWORK *p)
{
  return(memsize_BMP(&p->bmp_tl)
       + memsize_BMP(&p->bmp_tr)
       + memsize_BMP(&p->bmp_bl)
       + memsize_BMP(&p->bmp_br)
       + memsize_BMP(&p->bmp_l)
       + memsize_BMP(&p->bmp_r)
       + memsize_BMP(&p->bmp_t)
       + memsize_BMP(&p->bmp_b)
       + memsize_BMP(&p->bmp_c));
}

_UDWORD _Theme::MemSize(_MEMSIZE *p)
{
  _UDWORD i, size = 0;
  _THEME *pth;

  pth  = MemTheme.ptr;
  for(i=0; i<MemTheme.no; i++)
  {
    size += (sizeof(_THEME) + memsize_FRAMEWORK(&pth->frame0) + memsize_FRAMEWORK(&pth->frame1));
    pth  += 1;
  }

  p->data  = size;
  p->label = 0;

  return(p->data);
}
#endif

_THEME *_Theme::Get(_UWORD no)
{
  if((no != _DEFTHEME)&&(no < 256))
  {
    no = GuidPost[no];
    if(no < MemTheme.no)
    {
      return(&MemTheme.ptr[no]);
    }
  }

  return(NULL);
}

_THEME *_Theme::GetByIndex(_UWORD no)
{

  if((no < 256) && (no < MemTheme.no))
  {
    return(&MemTheme.ptr[no]);
  }

  return(NULL);
}

#define KEEPSIZE_BMP 10000
_BOOL copy_keep_BMP(_BMP *dst, _BMP *src)
{
  if(dst != src)
  {
   #ifndef UC_FIXMEMORY
    _UDWORD size;
    _BMPDAT *ph;
  
    if((src == NULL)||(dst == NULL))
      return _FALSE;
    
    size = src->datano * sizeof(_BMPDAT);
    if(size > KEEPSIZE_BMP)
      return copy_BMP(dst, src);
      
    if(dst->ptr == NULL)  
    {
      if(Memory2((void**)&dst->ptr, KEEPSIZE_BMP) == 0)
        return _FALSE;
    }
    
    ph = dst->ptr;
    MemCpy(ph, src->ptr, size);
    *dst = *src;
    dst->ptr = ph;
   #else
    *dst = *src;
    dst->ptr = (_BMPDAT*)GetFixMemory();
    MemCpy(dst->ptr, src->ptr, src->datano * sizeof(_BMPDAT));
   #endif
  }
  return _TRUE;
}

_BMP* prepaire_bmp(_BMP *dst, _BMP *src, _COLOR col)
{
  register _UWORD  c;
  _UWORD  dr, dg, db, r, g, b;
  _UDWORD i, end;
  _BMPDAT *pd;
  _COLOR  xol;
  _UWORD  zr, zg, zb, xr, xg, xb, xr_31, xg_63, xb_31;

  if(src->ptr != NULL)
  {
    if(copy_keep_BMP(dst, src) == _TRUE)
    {
      dr = ((col >> 11) & 0x1F);
      dg = ((col >>  5) & 0x3F);
      db = ((col      ) & 0x1F);

      xol = GetBackColor_Makro(col);
      xr = ((xol >> 11) & 0x1F);
      xg = ((xol >>  5) & 0x3F);
      xb = ((xol      ) & 0x1F);

      xr_31 = (31-xr) * 2; // speed
      xg_63 = (63-xg) * 2; // speed
      xb_31 = (31-xb) * 2; // speed

      pd  = dst->ptr;
      end = dst->datano;
      for(i=0; i<end; i++, pd++)
      {
        if(pd->col != dst->transparent)
        {
          c = pd->col;
          r = (c >> 11); // speed & 0x1F;
          b = (c & 0x1F);

          if(r == b)
          {
            g = (c >> 5) & 0x3F;
            if((g >> 1) == b) // graue pixel (schwarz --> farbe)
            {
              r = ((r * dr) + 15) / 31;
              g = ((g * dg) + 31) / 63;
              b = ((b * db) + 15) / 31;
              pd->col = (r << 11) | (g << 5) | (b);
              if(pd->col == dst->transparent)
                pd->col ^= 0x20;
            }
            else if(g == 0) // pinke pixel (schwarz --> farbe --> weiss)
            {
              if(c == 0x8010)
              {
                // entspricht genau dem treashold, r=128, g=0, b=128
                pd->col = xol;
              }
              else
              {
                g = (r << 1);

                if(g > 32)
                {
                  zr = (r-16) * xr_31; //speed (31-xr);
                  zg = (g-32) * xg_63; //speed (63-xg);
                  zb = (b-16) * xb_31; //speed (31-xb);
                  //speed zr = ((r*2)-32)*(31-xr);
                  //speed zg = ((g*2)-64)*(63-xg);
                  //speed zb = ((b*2)-32)*(31-xb);
                  
                  r = xr + ((zr + 15) / 31);
                  g = xg + ((zg + 31) / 63);
                  b = xb + ((zb + 15) / 31);
                }
                else
                {
                  r = ((r * xr)+ 7) / 15;
                  g = ((g * xg)+15) / 31;
                  b = ((b * xb)+ 7) / 15;
                }

                if(r > 0x1F) r = 0x1F;
                if(g > 0x3F) g = 0x3F;
                if(b > 0x1F) b = 0x1F;
                pd->col = (r << 11) | (g << 5) | (b);
              }
              if(pd->col == dst->transparent)
                pd->col ^= 0x20;
            }
          }
        }
      }
      return(dst);
    }
  }

  return(src);
}

_BMP* prepaire_bmp_treashold(_BMP *dst, _BMP *src, _COLOR col0)
{
  register _UWORD c;
  _UWORD  dr, dg, db, r, g, b, zr, zg, zb, dr_31, dg_63, db_31;
  _UWORD  xr, xg, xb, xr_31, xg_63, xb_31;
  _UDWORD i, end;
  _BMPDAT *pd;
  _COLOR col, xol;

  if(src->ptr != NULL)
  {
    if(copy_keep_BMP(dst, src) == _TRUE)
    {
      col = GetPenColor_Makro(col0);
      dr = ((col >> 11) & 0x1F);
      dg = ((col >> 5) & 0x3F);
      db = ((col & 0x1F));
      
      dr_31 = (31-dr) * 2; // speed
      dg_63 = (63-dg) * 2; // speed
      db_31 = (31-db) * 2; // speed

      xol = GetBackColor_Makro(col0);
      xr = ((xol >> 11) & 0x1F);
      xg = ((xol >>  5) & 0x3F);
      xb = ((xol      ) & 0x1F);

      xr_31 = (31-xr) * 2; // speed
      xg_63 = (63-xg) * 2; // speed
      xb_31 = (31-xb) * 2; // speed

      if(col == dst->transparent)
        col ^= 0x20; // nicht mehr transparent

      pd  = dst->ptr;

      end = dst->datano;
      for(i=0; i<end; i++, pd++)
      {
        if(pd->col != dst->transparent)
        {
          c = pd->col;
          if(c == 0x8410)
          {
            // entspricht genau dem treashold, r=128, g=128, b=128
            pd->col = col;
          }
          else
          {
            r = (c >> 11); //speed & 0x1F;
            b = (c & 0x1F);

            if(r == b)
            {
              g = (c >> 5) & 0x3F;
              if(((g >> 1)==b))
              {
                if(g > 32)
                {
                  //speed zr = ((r*2)-32)*(31-dr);
                  //speed zg = ((g*2)-64)*(63-dg);
                  //speed zb = ((b*2)-32)*(31-db);
                  zr = (r-16) * dr_31;
                  zg = (g-32) * dg_63;
                  zb = (b-16) * db_31;

                  r = dr + ((zr + 15) / 31);
                  g = dg + ((zg + 31) / 63);
                  b = db + ((zb + 15) / 31);
                }
                else
                {
                  r = ((r * dr)+ 7) / 15;
                  g = ((g * dg)+15) / 31;
                  b = ((b * db)+ 7) / 15;
                }

                if(r > 0x1F) r = 0x1F;
                if(g > 0x3F) g = 0x3F;
                if(b > 0x1F) b = 0x1F;

                pd->col = (r << 11) | (g << 5) | (b);

                if(pd->col == dst->transparent)
                  pd->col ^= 0x20;
              }




            else if(g == 0) // pinke pixel (schwarz --> farbe --> weiss)
            {
              if(c == 0x8010)
              {
                // entspricht genau dem treashold, r=128, g=0, b=128
                pd->col = xol;
              }
              else
              {
                g = (r << 1);

                if(g > 32)
                {
                  zr = (r-16) * xr_31; //speed (31-xr);
                  zg = (g-32) * xg_63; //speed (63-xg);
                  zb = (b-16) * xb_31; //speed (31-xb);
                  //speed zr = ((r*2)-32)*(31-xr);
                  //speed zg = ((g*2)-64)*(63-xg);
                  //speed zb = ((b*2)-32)*(31-xb);
                  
                  r = xr + ((zr + 15) / 31);
                  g = xg + ((zg + 31) / 63);
                  b = xb + ((zb + 15) / 31);
                }
                else
                {
                  r = ((r * xr)+ 7) / 15;
                  g = ((g * xg)+15) / 31;
                  b = ((b * xb)+ 7) / 15;
                }

                if(r > 0x1F) r = 0x1F;
                if(g > 0x3F) g = 0x3F;
                if(b > 0x1F) b = 0x1F;
                pd->col = (r << 11) | (g << 5) | (b);
              }
              if(pd->col == dst->transparent)
                pd->col ^= 0x20;
            }


              
              
              
              
            }
          }
        }
      }
      return(dst);
    }
  }

  return(src);
}

_BMP* black_bmp(_BMP *dst, _BMP *src)
{
  _UDWORD i, end;
  _BMPDAT *pd;
  _UWORD  col;

  if(src->ptr != NULL)
  {
    if(copy_keep_BMP(dst, src) == _TRUE)
    {
      col = BLACK;
      if(col == dst->transparent)
        col = BLACK + 1;

      pd  = dst->ptr;
      end = dst->datano;
      for(i=0; i<end; i++, pd++)
      {
        if(pd->col != dst->transparent)
          pd->col = col;
      }
      return(dst);
    }
  }

  return(src);
}

_BOOL inlet_THEME(_ROOM *dst, _ROOM *src, _FRAMETYPE frametype)
{
  _THEME *pth;
  _BOOL  retcode = _FALSE;

  pth = Theme.Get(frametype);

  if(pth != NULL)
  {
    retcode = _TRUE;
    *dst = *src;
    dst->xy1.x += pth->delta_l;
    dst->xy1.y += pth->delta_t;
    dst->xy2.x += pth->delta_r;
    dst->xy2.y += pth->delta_b;
  }
  else
  {
    *dst = *src;
  }

  return(retcode);
}

void position_cancle(_DOT *pd, _ROOM *pr, _THEME *pth)
{
  if(pth->xpos_cx < 0)
  {
   #ifdef SA41991
    pd->x = pr->xy2.x + pth->xpos_cx + 1;
   #else
    pd->x = pr->xy2.x + pth->xpos_cx;
   #endif   
  }
  else
  {
    pd->x = pr->xy1.x + pth->xpos_cx;
  }
  
  if(pth->ypos_cx < 0)
  {
   #ifdef SA41991
    pd->y = pr->xy2.y + pth->ypos_cx + 1;
   #else
    pd->y = pr->xy2.y + pth->ypos_cx;
   #endif
  }
  else
  {
    pd->y = pr->xy1.y + pth->ypos_cx;
  }
}

#ifdef UC_ROTATE
void draw_cancle(_ROOM *pr, _THEME *pth, _BOOL pressed, _BOOL refresh)
{
  _ROOM room;
  _BMP  *bmp = (pressed == _TRUE)? &pth->frame1.bmp_cx : &pth->frame0.bmp_cx;

  position_cancle(&room.xy1, pr, pth);
  Bitmap(room.xy1.x, room.xy1.y, bmp, T_COPY|T_SOLID, 0);
  if(refresh == _TRUE)
  {
    room.xy2.x = room.xy1.x + bmp->width - 1;
    room.xy2.y = room.xy1.y + bmp->height - 1;
    RefreshRectangle(&room);
  }
}
#else
void draw_cancle(_ROOM *pr, _THEME *pth, _BOOL pressed)
{
  _DOT xy;
  position_cancle(&xy, pr, pth);
  Bitmap(xy.x, xy.y, (pressed == _TRUE)? &pth->frame1.bmp_cx : &pth->frame0.bmp_cx, T_COPY|T_SOLID, 0);
}
#endif

_BOOL is_THEME_cancle(_DOT *pd, _FRAMETYPE frametype, _ROOM *pr, _CHKBIT *pchk, _MYSCHEME *pst)
{
  _ROOM   room;
  _UDWORD tmp;
  _BOOL   retcode = _FALSE;
  _THEME  *pth = Theme.Get(frametype);

  if(pth != NULL)
  {
    if(is_cancle(pth) == _TRUE)
    {
      position_cancle(&room.xy1, pr, pth);
      
     #ifdef USER_DEMAG
      room.xy1.x -= 10;
      room.xy1.y -= 10;
     #endif
     
      if((pd->x >= room.xy1.x)&&(pd->y >= room.xy1.y))
      {
        room.xy2.x = room.xy1.x + pth->frame0.bmp_cx.width - 1;
        room.xy2.y = room.xy1.y + pth->frame0.bmp_cx.height - 1;
        
       #ifdef USER_DEMAG
        room.xy2.x += 2 * 10;
        room.xy2.y += 2 * 10;
       #endif
        
        if((pd->x <= room.xy2.x)&&(pd->y <= room.xy2.y))
        {
          // treffer, hid ist auf canclebutton
          retcode = _TRUE;

          if(pchk != NULL) // chkbit überprüfen
          {
            if(chk_CHKBIT(pchk) == _FALSE)
              retcode = _FALSE;
          }

          if((retcode == _TRUE)&&(pst != NULL)) // stateschema überprüfen
          {
            if(StateScheme.Call(&tmp, pst, NULL, STATE_ACTIVE) == _FALSE)
              tmp = STATESCHEME_PRESELECTION;
            if(tmp != STATE_ACTIVE)
              retcode = _FALSE;
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL draw_THEME_cancle(_ROOM *pr, _FRAMETYPE frametype, _BOOL pressed)
{
  _THEME *pth = Theme.Get(frametype);
  _BOOL  retcode = _FALSE;

  if(is_cancle(pth) == _TRUE)
  {
   #ifdef UC_ROTATE
    draw_cancle(pr, pth, pressed, _TRUE);
   #else
    draw_cancle(pr, pth, pressed);
   #endif
    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL draw_THEME(_ROOM *pr, _FRAMETYPE frametype, _ATTRIB attrib, _COLOR colback, _BOOL pressed, _BOOL original_color)
{
  _THEME     *pth;
  _FRAMEWORK *fw;
  _KOORD     x, xt1, xt2, xb1, xb2, xm;
  _KOORD     y, yl1, yl2, yr1, yr2, ym;
  _ROOM      rmt, rmb, rml, rmr, room;
  _BOOL      retcode = _FALSE;
  _BMP       bmp;

  pth = Theme.Get(frametype);

  if(pth != NULL)
  {
    retcode = _TRUE;

    //    x   xt1      xt2
    //
    //y   +--+         +-+
    //    |  |         +-+
    //    +--+             yr1
    //yl1
    //             +-----+ yr2
    //yl2 +--+     |     |
    //    +--+     +-----+
    //
    //        xb1  xb2
//_ROOM ir;
//inlet_THEME(&ir, pr, pth->id);
    room = *pr;

    if(attrib & T_INVRS)
    {
      colback = MakeColor_Makro(GetPenColor_Makro(colback), GetBackColor_Makro(colback));
      attrib = attrib ^ T_INVRS;
    }

    if(attrib & T_INVERTTHEME) // T_BLINK
    {
      if(pressed == _TRUE)
        pressed = _FALSE;
      else
        pressed = _TRUE;

    }

    if(pressed == _TRUE)
    {
      fw = &pth->frame1;
    }
    else
    {
      fw = &pth->frame0;
    }

//    if(selected == _TRUE)
//    {
//      // editorfarbe holen
//      colback = GetPenColor_Makro(SetUp.GetEditColorBack(_EDITACTIVE, colback));
//    }

    x    = room.xy1.x;
    y    = room.xy1.y;
    xt1  = room.xy1.x + fw->bmp_tl.width;
    xt2  = room.xy2.x - fw->bmp_tr.width + 1;
    xb1  = room.xy1.x + fw->bmp_bl.width;
    xb2  = room.xy2.x - fw->bmp_br.width + 1;
    yl1  = room.xy1.y + fw->bmp_tl.height;
    yl2  = room.xy2.y - fw->bmp_bl.height + 1;
    yr1  = room.xy1.y + fw->bmp_tr.height;
    yr2  = room.xy2.y - fw->bmp_br.height + 1;
    xm   = ((room.xy1.x + room.xy2.x - fw->bmp_c.width) / 2);
    ym   = ((room.xy1.y + room.xy2.y - fw->bmp_c.height) / 2);

    rmt.xy1.x = xt1;
    rmt.xy2.x = xt2-1;
    rmt.xy1.y = y;
    rmt.xy2.y = y + fw->bmp_t.height - 1;
    rml.xy1.x = x;
    rml.xy2.x = x + fw->bmp_l.width - 1;
    rml.xy1.y = yl1;
    rml.xy2.y = yl2-1;
    rmr.xy1.x = room.xy2.x - fw->bmp_r.width + 1;
    rmr.xy2.x = room.xy2.x;
    rmr.xy1.y = yr1;
    rmr.xy2.y = yr2-1;
    rmb.xy1.x = xb1;
    rmb.xy2.x = xb2-1;
    rmb.xy1.y = room.xy2.y - fw->bmp_b.height + 1;
    rmb.xy2.y = room.xy2.y;

    if(is_invisible(colback, attrib) == _FALSE)
      Bar(room.xy1.x + pth->delta_l, room.xy1.y + pth->delta_t, room.xy2.x + pth->delta_r, room.xy2.y + pth->delta_b, attrib, colback);
    else if(original_color == _FALSE)
      return retcode;


    if((pth->colorcourse)&&(original_color == _FALSE))
    {
      init_BMP_Makro(&bmp);

      if(pth->colorcourse & 2)
      {
//        colback = GetPenColor_Makro(colback);
        // berechne farben von bmp samt treashold
        Bitmap(x  , y  , prepaire_bmp_treashold(&bmp, &fw->bmp_tl, colback), T_COPY|T_SOLID, 0);
        Bitmap(x  , yl2, prepaire_bmp_treashold(&bmp, &fw->bmp_bl, colback), T_COPY|T_SOLID, 0);
        Bitmap(xt2, y  , prepaire_bmp_treashold(&bmp, &fw->bmp_tr, colback), T_COPY|T_SOLID, 0);
        Bitmap(xb2, yr2, prepaire_bmp_treashold(&bmp, &fw->bmp_br, colback), T_COPY|T_SOLID, 0);
        out_zoom_BMP(&rmt, prepaire_bmp_treashold(&bmp, &fw->bmp_t, colback), T_COPY|T_SOLID);
        out_zoom_BMP(&rml, prepaire_bmp_treashold(&bmp, &fw->bmp_l, colback), T_COPY|T_SOLID);
        out_zoom_BMP(&rmr, prepaire_bmp_treashold(&bmp, &fw->bmp_r, colback), T_COPY|T_SOLID);
        out_zoom_BMP(&rmb, prepaire_bmp_treashold(&bmp, &fw->bmp_b, colback), T_COPY|T_SOLID);
        if(fw->bmp_c.ptr != NULL)
          Bitmap(xm,  ym,  prepaire_bmp_treashold(&bmp, &fw->bmp_c,  colback), T_COPY|T_SOLID, 0);
      }
      else
      {
        // berechne farben von bmp
        Bitmap(x  , y  , prepaire_bmp(&bmp, &fw->bmp_tl, colback), T_COPY|T_SOLID, 0);
        Bitmap(x  , yl2, prepaire_bmp(&bmp, &fw->bmp_bl, colback), T_COPY|T_SOLID, 0);
        Bitmap(xt2, y  , prepaire_bmp(&bmp, &fw->bmp_tr, colback), T_COPY|T_SOLID, 0);
        Bitmap(xb2, yr2, prepaire_bmp(&bmp, &fw->bmp_br, colback), T_COPY|T_SOLID, 0);
        out_zoom_BMP(&rmt, prepaire_bmp(&bmp, &fw->bmp_t, colback), T_COPY|T_SOLID);
        out_zoom_BMP(&rml, prepaire_bmp(&bmp, &fw->bmp_l, colback), T_COPY|T_SOLID);
        out_zoom_BMP(&rmr, prepaire_bmp(&bmp, &fw->bmp_r, colback), T_COPY|T_SOLID);
        out_zoom_BMP(&rmb, prepaire_bmp(&bmp, &fw->bmp_b, colback), T_COPY|T_SOLID);
        if(fw->bmp_c.ptr != NULL)
          Bitmap(xm,  ym,  prepaire_bmp(&bmp, &fw->bmp_c,  colback), T_COPY|T_SOLID, 0);
      }

     #ifndef UC_FIXMEMORY
      free_BMP(&bmp);
     #endif
    }
    else
    {
      // bmp ausgeben wie es ist
      Bitmap(x  , y  , &fw->bmp_tl, T_COPY|T_SOLID, 0);
      Bitmap(x  , yl2, &fw->bmp_bl, T_COPY|T_SOLID, 0);
      Bitmap(xt2, y  , &fw->bmp_tr, T_COPY|T_SOLID, 0);
      Bitmap(xb2, yr2, &fw->bmp_br, T_COPY|T_SOLID, 0);
      out_zoom_BMP(&rmt, &fw->bmp_t, T_COPY|T_SOLID);
      out_zoom_BMP(&rml, &fw->bmp_l, T_COPY|T_SOLID);
      out_zoom_BMP(&rmr, &fw->bmp_r, T_COPY|T_SOLID);
      out_zoom_BMP(&rmb, &fw->bmp_b, T_COPY|T_SOLID);
      if(fw->bmp_c.ptr != NULL)
        Bitmap(xm,  ym,  &fw->bmp_c,  T_COPY|T_SOLID, 0);
    }

    if(is_cancle(pth) == _TRUE)
    {
	 #ifdef UC_ROTATE
      draw_cancle(pr, pth, pressed, _FALSE);
	 #else
      draw_cancle(pr, pth, pressed);
     #endif
    }
//_ROOM ir;
//inlet_THEME(&ir, pr, frametype);
//Rectangle(ir.xy1.x, ir.xy1.y, ir.xy2.x, ir.xy2.y, T_COPY|T_DOTTED, MakeColor_Makro(INVISIBLE, LIGHTGREEN));
  }
//  else
//  {
//    todo: auf Freigabe Entwicklungsleitung warten
//    Bar(pr->xy1.x, pr->xy1.y, pr->xy2.x, pr->xy2.y, attrib, colback);
//  }


  return(retcode);
}



void draw_THEME_part_sub(_COLOR colback, _ATTRIB attrib, _UWORD colorcourse, _ROOM *room, _ROOM *room1, _BMP *bmp1, _ROOM *room2, _BMP *bmp2)
{
  _BMP bmp;

  if(is_invisible(colback, attrib) == _FALSE)
    Bar(room->xy1.x, room->xy1.y, room->xy2.x, room->xy2.y, attrib, colback);

  if(colorcourse)
  {
    colback = GetPenColor_Makro(colback);
    init_BMP_Makro(&bmp);

    if(colorcourse & 2)
    {
      // berechne farben von bmp samt treashold
      out_zoom_BMP(room1, prepaire_bmp_treashold(&bmp, bmp1, colback), T_COPY|T_SOLID);
      out_zoom_BMP(room2, prepaire_bmp_treashold(&bmp, bmp2, colback), T_COPY|T_SOLID);
    }
    else
    {
      // berechne farben von bmp
      out_zoom_BMP(room1, prepaire_bmp(&bmp, bmp1, colback), T_COPY|T_SOLID);
      out_zoom_BMP(room2, prepaire_bmp(&bmp, bmp2, colback), T_COPY|T_SOLID);
    }

   #ifndef UC_FIXMEMORY
    free_BMP(&bmp);
   #endif
  }
  else
  {
    // bmp ausgeben wie es ist
    out_zoom_BMP(room1, bmp1, T_COPY|T_SOLID);
    out_zoom_BMP(room2, bmp2, T_COPY|T_SOLID);
  }
}

_BOOL draw_THEME_part(_ROOM *pr, _FRAMETYPE frametype, _ATTRIB attrib, _COLOR colback, _BOOL pressed, _BOOL horizontal)
{
  _THEME     *pth;
  _FRAMEWORK *fw;
  _ROOM      rmt, rmb, rml, rmr, room;
  _BOOL      retcode = _FALSE;

  pth = Theme.Get(frametype);

  if(pth != NULL)
  {
    retcode = _TRUE;
    room    = *pr;

    if(attrib & T_INVRS)
    {
      colback = MakeColor_Makro(GetPenColor_Makro(colback), GetBackColor_Makro(colback));
      attrib = attrib ^ T_INVRS;
    }

    if(attrib & T_INVERTTHEME) // T_BLINK
    {
      if(pressed == _TRUE)
        pressed = _FALSE;
      else
        pressed = _TRUE;
    }

    if(pressed == _TRUE)
    {
      fw = &pth->frame1;
    }
    else
    {
      fw = &pth->frame0;
    }

    if(horizontal == _TRUE)
    {
      rmt.xy1.x = room.xy1.x;
      rmt.xy2.x = room.xy2.x;
      rmt.xy1.y = room.xy1.y;
      rmt.xy2.y = room.xy1.y + fw->bmp_t.height - 1;
      rmb.xy1.x = room.xy1.x;
      rmb.xy2.x = room.xy2.x;
      rmb.xy1.y = room.xy2.y - fw->bmp_b.height + 1;
      rmb.xy2.y = room.xy2.y;

      draw_THEME_part_sub(colback, attrib, pth->colorcourse, &room, &rmt, &fw->bmp_t, &rmb, &fw->bmp_b);
    }
    else
    {
      rml.xy1.x = room.xy1.x;
      rml.xy2.x = room.xy1.x + fw->bmp_l.width - 1;
      rml.xy1.y = room.xy1.y;
      rml.xy2.y = room.xy2.y;
      rmr.xy1.x = room.xy2.x - fw->bmp_r.width + 1;
      rmr.xy2.x = room.xy2.x;
      rmr.xy1.y = room.xy1.y;
      rmr.xy2.y = room.xy2.y;

      draw_THEME_part_sub(colback, attrib, pth->colorcourse, &room, &rmr, &fw->bmp_r, &rml, &fw->bmp_l);
    }
  }

  return(retcode);
}


void draw_BMP_partial(_COLOR colback, _UWORD colorcourse, _ROOM *room, _BMP *bmp)
{
  _BMP tmp;

  if(colorcourse)
  {
    colback = GetPenColor_Makro(colback);
    init_BMP_Makro(&tmp);

    if(colorcourse & 2)
    {
      // berechne farben von bmp samt treashold
      out_zoom_BMP(room, prepaire_bmp_treashold(&tmp, bmp, colback), T_COPY|T_SOLID);
    }
    else
    {
      // berechne farben von bmp
      out_zoom_BMP(room, prepaire_bmp(&tmp, bmp, colback), T_COPY|T_SOLID);
    }

   #ifndef UC_FIXMEMORY
    free_BMP(&tmp);
   #endif
  }
  else
  {
    // bmp ausgeben wie es ist
    out_zoom_BMP(room, bmp, T_COPY|T_SOLID);
  }
}

void draw_THEME_partial(_ROOM *out, _ROOM *part, _FRAMETYPE frametype, _ATTRIB attrib, _COLOR colback)
{
  _THEME     *pth;
  _FRAMEWORK *fw;
  _ROOM      tr, room;
  _BOOL      retcode = _FALSE;
  _KOORD     tx1, tx2, bx1, bx2;
  _KOORD     ly1, ly2, ry1, ry2;
  
  if(is_invisible(colback, attrib) == _FALSE)
  {
  
    pth = Theme.Get(frametype);

    if(pth != NULL)
    {
      retcode = _TRUE;
      room    = *out;

      if(attrib & T_INVRS)
      {
        colback = MakeColor_Makro(GetPenColor_Makro(colback), GetBackColor_Makro(colback));
        attrib = attrib ^ T_INVRS;
      }
    
      fw = &pth->frame0;
      if(attrib & T_INVERTTHEME) // T_BLINK
        fw = &pth->frame1;
    
      tr.xy1.x = room.xy1.x + pth->delta_l;
      tr.xy1.y = room.xy1.y + pth->delta_t;
      tr.xy2.x = room.xy2.x + pth->delta_r;
      tr.xy2.y = room.xy2.y + pth->delta_b;
      Bar(tr.xy1.x, tr.xy1.y, tr.xy2.x, tr.xy2.y, attrib, colback);


      tx1 = room.xy1.x;
      tx2 = room.xy2.x;
      ly1 = room.xy1.y;
      ly2 = room.xy2.y;
    
      bx1 = room.xy1.x;
      bx2 = room.xy2.x;
      ry1 = room.xy1.y;
      ry2 = room.xy2.y;

      // leftpart
      if(part->xy1.x < (room.xy1.x + fw->bmp_tl.width))
      {
        if(part->xy1.y < (room.xy1.y + fw->bmp_tl.height))
        {
          tr.xy1 = room.xy1;
          tr.xy2.x = room.xy1.x + fw->bmp_tl.width - 1;
          tr.xy2.y = room.xy1.y + fw->bmp_tl.height - 1;
          draw_BMP_partial(colback, pth->colorcourse, &tr, &fw->bmp_tl);
          ly1 += fw->bmp_tl.height;
          tx1 += fw->bmp_tl.width;
        }
      }
    
      if(part->xy1.x < (room.xy1.x + fw->bmp_bl.width))
      {
        if(part->xy2.y > (room.xy2.y - fw->bmp_bl.height))
        {
          tr.xy1.x = room.xy1.x;
          tr.xy1.y = room.xy2.y - fw->bmp_bl.height + 1;
          tr.xy2.x = room.xy1.x + fw->bmp_bl.width - 1;
          tr.xy2.y = room.xy2.y;
          draw_BMP_partial(colback, pth->colorcourse, &tr, &fw->bmp_bl);
          ly2 -= fw->bmp_bl.height;
          bx1 += fw->bmp_bl.width;
        }
      }

      if(part->xy1.x < room.xy1.x + fw->bmp_l.width)
      {
        if((part->xy1.y <= ly2) && (part->xy2.y >= ly1))
        {
          tr.xy1.x = room.xy1.x;
          tr.xy1.y = ly1;
          tr.xy2.x = room.xy1.x + fw->bmp_l.width - 1;
          tr.xy2.y = ly2;
          draw_BMP_partial(colback, pth->colorcourse, &tr, &fw->bmp_l);
        }
      }
    
    
      // rightpart
      if(part->xy2.x >= (room.xy2.x - fw->bmp_tr.width))
      {
        if(part->xy1.y < (room.xy1.y + fw->bmp_tr.height))
        {
          tr.xy1.x = room.xy2.x - fw->bmp_tr.width + 1;
          tr.xy1.y = room.xy1.y;
          tr.xy2.x = room.xy2.x;
          tr.xy2.y = room.xy1.y + fw->bmp_tr.height - 1;
          draw_BMP_partial(colback, pth->colorcourse, &tr, &fw->bmp_tr);
          ry1 += fw->bmp_tr.height;
          tx2 -= fw->bmp_tr.width;
        }
      }
    
      if(part->xy2.x >= (room.xy2.x - fw->bmp_br.width))
      {
        if(part->xy2.y > (room.xy2.y - fw->bmp_bl.height))
        {
          tr.xy1.x = room.xy2.x - fw->bmp_br.width + 1;
          tr.xy1.y = room.xy2.y - fw->bmp_bl.height + 1;
          tr.xy2.x = room.xy2.x;
          tr.xy2.y = room.xy2.y;
          draw_BMP_partial(colback, pth->colorcourse, &tr, &fw->bmp_br);
          ry2 -= fw->bmp_br.height;
          bx2 -= fw->bmp_br.width;
        }
      }
    
      if(part->xy2.x >= (room.xy2.x - fw->bmp_r.width))
      {
        if((part->xy1.y <= ry2) && (part->xy2.y >= ry1))
        {
          tr.xy1.x = room.xy2.x - fw->bmp_r.width + 1;
          tr.xy1.y = ry1;
          tr.xy2.x = room.xy2.x;
          tr.xy2.y = ry2;
          draw_BMP_partial(colback, pth->colorcourse, &tr, &fw->bmp_r);
        }
      }
    
      // toppart
      if(part->xy1.y < (room.xy1.y + fw->bmp_t.height))
      {
        if((part->xy1.x <= tx2) && (part->xy2.x >= tx1))
        {
          tr.xy1.x = tx1;
          tr.xy1.y = room.xy1.y;
          tr.xy2.x = tx2;
          tr.xy2.y = room.xy1.y + fw->bmp_t.height - 1;
          draw_BMP_partial(colback, pth->colorcourse, &tr, &fw->bmp_t);
        }
      }

      // bottompart
      if(part->xy2.y > (room.xy2.y - fw->bmp_b.height))
      {
        if((part->xy1.x <= bx2) && (part->xy2.x >= bx1))
        {
          tr.xy1.x = bx1;
          tr.xy1.y = room.xy2.y - + fw->bmp_b.height + 1;
          tr.xy2.x = bx2;
          tr.xy2.y = room.xy2.y;
          draw_BMP_partial(colback, pth->colorcourse, &tr, &fw->bmp_b);
        }
      }
    }
  }
}


_BOOL shadow_THEME(_ROOM *pr, _FRAMETYPE frametype)
{
  _THEME     *pth;
  _FRAMEWORK *fw;
  _KOORD     x, xt1, xt2, xb1, xb2;
  _KOORD     y, yl1, yl2, yr1, yr2;
  _ROOM      rmt, rmb, rml, rmr, room;
  _BOOL      retcode = _FALSE;
  _BMP       bmp;

  pth = Theme.Get(frametype);

  if(pth != NULL)
  {
    retcode = _TRUE;

    room = *pr;
    fw   = &pth->frame1;

    x    = room.xy1.x;
    y    = room.xy1.y;
    xt1  = room.xy1.x + fw->bmp_tl.width;
    xt2  = room.xy2.x - fw->bmp_tr.width + 1;
    xb1  = room.xy1.x + fw->bmp_bl.width;
    xb2  = room.xy2.x - fw->bmp_br.width + 1;
    yl1  = room.xy1.y + fw->bmp_tl.height;
    yl2  = room.xy2.y - fw->bmp_bl.height + 1;
    yr1  = room.xy1.y + fw->bmp_tr.height;
    yr2  = room.xy2.y - fw->bmp_br.height + 1;

    rmt.xy1.x = xt1;
    rmt.xy2.x = xt2-1;
    rmt.xy1.y = y;
    rmt.xy2.y = y + fw->bmp_t.height - 1;
    rml.xy1.x = x;
    rml.xy2.x = x + fw->bmp_l.width - 1;
    rml.xy1.y = yl1;
    rml.xy2.y = yl2-1;
    rmr.xy1.x = room.xy2.x - fw->bmp_r.width + 1;
    rmr.xy2.x = room.xy2.x;
    rmr.xy1.y = yr1;
    rmr.xy2.y = yr2-1;
    rmb.xy1.x = xb1;
    rmb.xy2.x = xb2-1;
    rmb.xy1.y = room.xy2.y - fw->bmp_b.height + 1;
    rmb.xy2.y = room.xy2.y;

    Bar(room.xy1.x + pth->delta_l, room.xy1.y + pth->delta_t, room.xy2.x + pth->delta_r, room.xy2.y + pth->delta_b, T_COPY|T_SOLID, BLACK);

    init_BMP_Makro(&bmp);
    Bitmap(x  , y  , black_bmp(&bmp, &fw->bmp_tl), T_COPY|T_SOLID, 0);
    Bitmap(x  , yl2, black_bmp(&bmp, &fw->bmp_bl), T_COPY|T_SOLID, 0);
    Bitmap(xt2, y  , black_bmp(&bmp, &fw->bmp_tr), T_COPY|T_SOLID, 0);
    Bitmap(xb2, yr2, black_bmp(&bmp, &fw->bmp_br), T_COPY|T_SOLID, 0);
    out_zoom_BMP(&rmt, black_bmp(&bmp, &fw->bmp_t), T_COPY|T_SOLID);
    out_zoom_BMP(&rml, black_bmp(&bmp, &fw->bmp_l), T_COPY|T_SOLID);
    out_zoom_BMP(&rmr, black_bmp(&bmp, &fw->bmp_r), T_COPY|T_SOLID);
    out_zoom_BMP(&rmb, black_bmp(&bmp, &fw->bmp_b), T_COPY|T_SOLID);
   #ifndef UC_FIXMEMORY
    free_BMP(&bmp);
   #endif
  }

  return(retcode);
}


void clip_BMP(_KOORD x, _KOORD y, _BMP *bmp)
{
  _UWORD  i, xx, yy, tra, start, end, ix, fun;
  _KOORD  iy;
  _BMPDAT *rgb;
  _ROOM   room;

  if(bmp != NULL)
  {
    if((bmp->height > 0)&&(bmp->width > 0))
    {
      if((!(bmp->state & BMP_NO_TRANSPARENT))&&(!(bmp->state & 0x0008))) //BMP_256COLOR)))
      {
        tra = bmp->transparent;
        xx  = bmp->width;
        yy  = bmp->height;
        rgb = bmp->ptr;

        for(i=0, iy=y+bmp->height-1; i<yy; i++, iy--)
        {
          start = 0;
          end   = 0;
          ix    = 0;
          fun   = 0;
          while(ix < xx)
          {
            ix += rgb->no;
            if(rgb->col == tra)
            {
              if(fun == 0)
                start += rgb->no;
              else
                end += rgb->no;
            }
            else
            {
              fun = 1;
              end = 0;
            }
            rgb += 1;
          }

          if((start + end) < xx)
          {
            room.xy1.x = x+start;
            room.xy1.y = iy;
            room.xy2.x = x+bmp->width-1-end;
            room.xy2.y = iy;
            AddRegion(&room);
          }
        }
      }
      else
      {
        room.xy1.x = x;
        room.xy1.y = y;
        room.xy2.x = x + bmp->width - 1;
        room.xy2.y = y + bmp->height - 1;
        AddRegion(&room);
      }
    }
  }
}

_BOOL clip_THEME(_ROOM *pr, _FRAMETYPE frametype)
{
  _BOOL      retcode = _FALSE;
  _THEME     *pth;
  _FRAMEWORK *fw;
  _ROOM      room;

  pth = Theme.Get(frametype);

  if(pth != NULL)
  {
    retcode = _TRUE;
    fw = &pth->frame1;

    room.xy1.x = pr->xy1.x + pth->delta_l;
    room.xy1.y = pr->xy1.y + pth->delta_t;
    room.xy2.x = pr->xy2.x + pth->delta_r;
    room.xy2.y = pr->xy2.y + pth->delta_b;
    AddRegion(&room);

    room.xy1.x = pr->xy1.x;
    room.xy2.x = pr->xy1.x + pth->delta_l - 1;
    room.xy1.y = pr->xy1.y + fw->bmp_tl.height;
    room.xy2.y = pr->xy2.y - fw->bmp_bl.height;
    AddRegion(&room);

    room.xy1.x = pr->xy2.x + pth->delta_r + 1;
    room.xy2.x = pr->xy2.x;
    room.xy1.y = pr->xy1.y + fw->bmp_tr.height;
    room.xy2.y = pr->xy2.y - fw->bmp_br.height;
    AddRegion(&room);

    room.xy1.x = pr->xy1.x + fw->bmp_tl.width;
    room.xy2.x = pr->xy2.x - fw->bmp_tr.width;
    room.xy1.y = pr->xy1.y;
    room.xy2.y = pr->xy1.y + pth->delta_t - 1;
    AddRegion(&room);

    room.xy1.x = pr->xy1.x + fw->bmp_bl.width;
    room.xy2.x = pr->xy2.x - fw->bmp_br.width;
    room.xy1.y = pr->xy2.y + pth->delta_b + 1;
    room.xy2.y = pr->xy2.y;
    AddRegion(&room);

    clip_BMP(pr->xy1.x,                    pr->xy1.y,                     &fw->bmp_tl);
    clip_BMP(pr->xy1.x,                    pr->xy2.y-fw->bmp_bl.height+1, &fw->bmp_bl);
    clip_BMP(pr->xy2.x-fw->bmp_tr.width+1, pr->xy1.y,                     &fw->bmp_tr);
    clip_BMP(pr->xy2.x-fw->bmp_br.width+1, pr->xy2.y-fw->bmp_br.height+1, &fw->bmp_br);
  }

  return(retcode);
}

void theme_minsize(_FRAMETYPE frametype, _ATTRIB attrib, _UWORD *xx, _UWORD *yy)
{
  _FRAMEWORK *fw;
  _THEME *pth;
  _UWORD xl, xr, yt, yb;

  *xx = 1;
  *yy = 1;

  switch(frametype)
  {
    case 0: 
      return;
    case 1: // single
      *xx = 2;
      *yy = 2;
      return;
    case 2: // 3D
      *xx = 2;
      *yy = 2;
      return;
    case 3: // overlapped
      *xx = 4;
      *yy = 4;
      return;
    case 4: // edge
      *xx = 6;
      *yy = 6;
      return;
    default: 
      pth = Theme.Get(frametype);
      if(pth != NULL)
      {
        fw = &pth->frame0;
        if(attrib & T_INVERTTHEME) // T_BLINK
          fw = &pth->frame1;
      
      
        xl = 0;
        if(fw->bmp_tl.width > xl)
          xl = fw->bmp_tl.width;
        if(fw->bmp_bl.width > xl)
          xl = fw->bmp_bl.width;
        if(fw->bmp_l.width > xl)
          xl = fw->bmp_l.width;
      
        xr = 0;  
        if(fw->bmp_tr.width > xr)
          xr = fw->bmp_tr.width;
        if(fw->bmp_br.width > xr)
          xr = fw->bmp_br.width;
        if(fw->bmp_r.width > xr)
          xr = fw->bmp_r.width;

        yt = 0;  
        if(fw->bmp_tl.height > yt)
          yt = fw->bmp_tl.height;
        if(fw->bmp_tr.height > yt)
          yt = fw->bmp_tr.height;
        if(fw->bmp_t.height > yt)
          yt = fw->bmp_t.height;
      
        yb = 0;  
        if(fw->bmp_bl.height > yb)
          yb = fw->bmp_bl.height;
        if(fw->bmp_br.height > yb)
          yb = fw->bmp_br.height;
        if(fw->bmp_b.height > yb)
          yb = fw->bmp_b.height;

        *xx = xl + xr;
        *yy = yt + yb;
      }
      break;
  }
}

#endif

_BOOL has_theme_cancel(_UDWORD frameid)
{
  _THEME *pth = Theme.Get(frameid);

  if(pth != NULL)
    return is_cancle(pth);
    
  return _FALSE;
}

_FRAMETYPE GetFrameType(_FRAMETYPE frame1, _FRAMETYPE frame2, _FRAMETYPE frame3, _FRAMETYPE framedefault)
{
 #ifdef UC_THEME
  if(Theme.Get(frame1) != NULL)
    return frame1;
  if(Theme.Get(frame2) != NULL)
    return frame2;
  if(Theme.Get(frame3) != NULL)
    return frame3;
 #endif
  return framedefault;
  //return _3DFRAME;
}

