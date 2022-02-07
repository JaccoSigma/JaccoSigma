#include "T_IPC.H"
#include "T_NUC.H"


_BOOL add_zorder(_PICTUREMASK *pm, _UWORD ztype, _UWORD index)
{
  _BOOL retcode = _FALSE;
  
  _UDWORD size = (pm->memzorder.no + 1) * sizeof(_ZORDER);
  if(Memory2((void**)&pm->memzorder.ptr, size) != 0)
  {
    _ZORDER *pzo = &pm->memzorder.ptr[pm->memzorder.no];
    init_ZORDER(pzo);
    pzo->ftype = ztype;
    pzo->no    = index;

    pm->memzorder.no += 1;
    retcode = _TRUE;
  }
  
  return retcode;
}


_BOOL add_myobject(_PICTUREMASK *pm, _UWORD no, _ROOM *pr, _UDWORD overload)
{
  _BOOL       retcode = _FALSE;
  _SubPicture *Obj;
  _ROOM       space, room;
  _OBJECT     *pdest;
  _UDWORD     size;
  _DWORD      xx, nn;

  Obj = ObjectList.Get(no);

  if(Obj != NULL)
  {
    room  = Obj->GetPICTURE()->mask.room;
    space = *pr;

    size = (pm->memobject.no + 1) * sizeof(_OBJECT);
    if(Memory2((void**)&pm->memobject.ptr, size) != 0)
    {
      linear_coordinates(&space.xy1.x, &space.xy1.y, &space.xy2.x, &space.xy2.y);
      linear_coordinates(&room.xy1.x, &room.xy1.y, &room.xy2.x, &room.xy2.y);

      pdest = &pm->memobject.ptr[pm->memobject.no];
      init_OBJECT(pdest);

      pdest->xy = space.xy1;
      pdest->no = no;

      xx = (room.xy2.x  - room.xy1.x  + 1);
      nn = (space.xy2.x - space.xy1.x + 1);
      pdest->zoom.x = (_WORD)((nn * 1000)/xx);

      xx = (room.xy2.y  - room.xy1.y  + 1);
      nn = (space.xy2.y - space.xy1.y + 1);
      pdest->zoom.y = (_WORD)((nn * 1000)/xx);

      if(overload < VarList.GetNo())
      {
        pdest->loader.no = 1;
        pdest->loader.info[0].state = VAR_VAL;
        pdest->loader.info[0].value = overload;
      }

      if(add_zorder(pm, ZO_OBJECT, pm->memobject.no) == _TRUE)
      {
        pm->memobject.no += 1;
        retcode           = _TRUE;
      }

//      size = (pm->memzorder.no + 1) * sizeof(_ZORDER);
//      if(Memory2((void**)&pm->memzorder.ptr, size) != 0)
//      {
//        pzo = &pm->memzorder.ptr[pm->memzorder.no];
//        init_ZORDER(pzo);
//        pzo->ftype = ZO_OBJECT;
//        pzo->no    = pm->memobject.no;
//        pm->memobject.no += 1;
//        pm->memzorder.no += 1;
//        retcode           = _TRUE;
//      }
    }
  }

  return(retcode);
}

_BOOL add_COL_PICTXT(_PICTUREMASK *pm, _COL_PICTXT *pctxt)
{
  _UDWORD size = (pm->colpictxt.no + 1) * sizeof(_COL_PICTXT);
  
  if(Memory2((void**)&pm->colpictxt.ptr, size) != 0)
  {
    if(add_zorder(pm, ZO_COLTEXT, pm->colpictxt.no) == _TRUE)
    {
      pm->colpictxt.ptr[pm->colpictxt.no] = *pctxt;
      pm->colpictxt.no += 1;
      return _TRUE;
    }
  }
  return _FALSE;
}

_BOOL add_PICTEXT(_PICTUREMASK *pm, _PICTEXT *ptxt)
{
  _UDWORD size = (pm->mempictxt.no + 1) * sizeof(_PICTEXT);
  
  if(Memory2((void**)&pm->mempictxt.ptr, size) != 0)
  {
    if(add_zorder(pm, ZO_TEXT, pm->mempictxt.no) == _TRUE)
    {
      pm->mempictxt.ptr[pm->mempictxt.no] = *ptxt;
      pm->mempictxt.no += 1;
      return _TRUE;
    }
  }
  return _FALSE;
}    

_BOOL add_RECTANGLE(_PICTUREMASK *pm, _RECTANGLE *prec)
{
  _UDWORD size = (pm->memrectangle.no + 1) * sizeof(_RECTANGLE);
  
  if(Memory2((void**)&pm->memrectangle.ptr, size) != 0)
  {
    if(add_zorder(pm, ZO_RECTANGLE, pm->memrectangle.no) == _TRUE)
    {
      pm->memrectangle.ptr[pm->memrectangle.no] = *prec;
      pm->memrectangle.no += 1;
      return _TRUE;
    }
  }
  return _FALSE;
}

_BOOL add_COL_RECT(_PICTUREMASK *pm, _COL_RECT *prec)
{
  _UDWORD size = (pm->colrect.no + 1) * sizeof(_COL_RECT);
  
  if(Memory2((void**)&pm->colrect.ptr, size) != 0)
  {
    if(add_zorder(pm, ZO_COLRECTANGLE, pm->colrect.no) == _TRUE)
    {
      pm->colrect.ptr[pm->colrect.no] = *prec;
      pm->colrect.no += 1;
      return _TRUE;
    }
  }
  return _FALSE;
}

_BOOL add_BUTTON(_PICTUREMASK *pm, _BUTTON *pbut)
{
  _UDWORD size = (pm->membutton.no + 1) * sizeof(_BUTTON);
  
  if(Memory2((void**)&pm->membutton.ptr, size) != 0)
  {
    if(add_zorder(pm, ZO_BUTTON, pm->membutton.no) == _TRUE)
    {
      pm->membutton.ptr[pm->membutton.no] = *pbut;
      pm->membutton.no += 1;
      return _TRUE;
    }
  }
  return _FALSE;
}    

_BOOL add_IO(_PICTUREMASK *pm, _IO *pio)
{
  _UDWORD size = (pm->memoutput.no + 1) * sizeof(_IO);

  if(Memory2((void**)&pm->memoutput.ptr, size) != 0)
  {
    if(add_zorder(pm, ZO_OUTPUT, pm->memoutput.no) == _TRUE)
    {
      pm->memoutput.ptr[pm->memoutput.no] = *pio;
      pm->memoutput.no += 1;
      return _TRUE;
    }
  }
  return _FALSE;
}

_BOOL add_INPUT(_PICTUREMASK *pm, _INPUT *pio)
{
  _UDWORD size = (pm->meminput.no + 1) * sizeof(_INPUT);

  if(Memory2((void**)&pm->meminput.ptr, size) != 0)
  {
    if(add_zorder(pm, ZO_INPUT, pm->meminput.no) == _TRUE)
    {
      pm->meminput.ptr[pm->meminput.no] = *pio;
      pm->meminput.no += 1;
      return _TRUE;
    }
  }
  return _FALSE;
}

_BOOL add_BITMAP(_PICTUREMASK *pm, _BITMAP *bmp)
{
  _UDWORD size = (pm->membitmap.no + 1) * sizeof(_BITMAP);

  if(Memory2((void**)&pm->membitmap.ptr, size) != 0)
  {
    if(add_zorder(pm, ZO_IMAGE, pm->membitmap.no) == _TRUE)
    {
      pm->membitmap.ptr[pm->membitmap.no] = *bmp;
      pm->membitmap.no += 1;
      return _TRUE;
    }
  }
  return _FALSE;
}


// ****************************************************************
// ** class _SubPicture
// ****************************************************************

_DWORD _SubPicture::Add(_MOTION *pm)
{
  _DWORD retcode = -1;
  _UDWORD size   = (Picture.mask.memmotion.no + 1) * sizeof(_MOTION);

  if(Memory2((void**)&Picture.mask.memmotion.ptr, size) != 0)
  {
    if(add_zorder(&Picture.mask, ZO_MOTION, Picture.mask.memmotion.no) == _TRUE)
    {
      Picture.mask.memmotion.ptr[Picture.mask.memmotion.no] = *pm;
      retcode = Picture.mask.memmotion.no;
      Picture.mask.memmotion.no += 1;
    }
  }

  return(retcode);
}

_BOOL _SubPicture::AddObject(_UWORD no, _ROOM *pr, _UDWORD varno)
{
  return(add_myobject(&Picture.mask, no, pr, varno));
}

_BOOL _SubPicture::Add(_COL_PICTXT *pctxt)
{
  return(add_COL_PICTXT(&Picture.mask, pctxt));
}

_BOOL _SubPicture::Add(_PICTEXT *ptxt)
{
  return(add_PICTEXT(&Picture.mask, ptxt));
}

_BOOL _SubPicture::Add(_RECTANGLE *prec)
{
  return(add_RECTANGLE(&Picture.mask, prec));
}

_BOOL _SubPicture::Add(_COL_RECT *pcrec)
{
  return(add_COL_RECT(&Picture.mask, pcrec));
}

_BOOL _SubPicture::Add(_BUTTON *pbut)
{
  return(add_BUTTON(&Picture.mask, pbut));
}

_BOOL _SubPicture::Add(_IO *pio)
{
  return(add_IO(&Picture.mask, pio));
}

_BOOL _SubPicture::Add(_INPUT *pio)
{
  return(add_INPUT(&Picture.mask, pio));
}

_BOOL _SubPicture::Add(_BITMAP *bmp)
{
  return(add_BITMAP(&Picture.mask, bmp));
}

// ****************************************************************
// ** st-interface funktionen
// ****************************************************************

_BOOL check_mp(_MY_PICTURE *mp)
{
  if(mp != NULL)
  {
    if(mp->No >= 1)
    {
      return _TRUE;
    }
  }
  return _FALSE;
}

_BOOL check_varno(_UDWORD varno)
{
  if(varno != DEFSCOPE)
  {
    if((varno < VarList.GetNo()) || (varno >= VARNO_JAPANESE_INPUT))
    {
      return _TRUE;
    }
  }
  return _FALSE;
}


extern "C" _DWORD add_carousel(_MY_PICTURE *mp, _ROOM *room, _UWORD cino, _CAROITEM *ci, _UDWORD varno_pos, _BOOL horizontal, _BOOL infinite, _UWORD _dx)
{
  // cino ......... anzahl der _CAROITEM hinter *ci
  // varno_xpos ... variablenummer der xpositionsvariable
  // dx ........... abstand zwischen den einzelnen objekten
  
  
  _DWORD retcode = -1;
  
  if(check_mp(mp) == _TRUE)
  {
    _MOTION carousel;
    _UWORD  i; 
  
    init_MOTION(&carousel);
    carousel.attrib &= ~T_BOUND;
    carousel.attrib |= (T_LEFTBOUND | T_UPBOUND);
      
    if(Memory2((void**)&carousel.memkaru.ptr, cino * sizeof(_KARU)) != 0)
    {
      carousel.memkaru.no = cino;
      _KARU *pkaru = carousel.memkaru.ptr;
      for(i=0; i<cino; i++)
      {
        init_KARU(pkaru);
        pkaru->objno = ci[i].objno;
        copy_VARLABEL(&pkaru->loader, &ci[i].loader);
        pkaru += 1;
      }
    
      carousel.gap        = _dx;
      carousel.infinite   = infinite;
      carousel.horizontal = horizontal;
      carousel.room       = *room;
      carousel.wiperoom   = *room;
      //carousel.room.xy2.x = carousel.room.xy1.x + width;
      //carousel.room.xy2.y = carousel.room.xy1.y;
    
      carousel.varx.no = 1;
      carousel.varx.info[0].state = CONST_VAL;
      carousel.varx.info[0].value = 0;
      carousel.vary = carousel.varx;
    
      _IVALUE val;
      if(VarList.GetValueByNo(varno_pos, &val, _TRUE) == _TRUE)
      {
        if(horizontal == _TRUE)
        {
          carousel.varx.no = 1;
          carousel.varx.info[0].state = VAR_VAL;
          carousel.varx.info[0].value = varno_pos;
        }
        else
        {
          carousel.vary.no = 1;
          carousel.vary.info[0].state = VAR_VAL;
          carousel.vary.info[0].value = varno_pos;
        }
        carousel.usermove = _TRUE;
      }
        
      retcode = mp->Ptr[mp->No-1].Add(&carousel);
    }    
  }
  
  return retcode;
}

_BOOL add_carousel_scrollbar(_MY_PICTURE *mp, _DWORD cno, _FRAMETYPE frame, _COLOR col_back, _COLOR col_front, _COLOR col_symbol, _COLOR col_topleft, _COLOR col_bottomright)
{
  _MOTION *p = get_carousel(mp, cno);
  
  if(p != NULL)
  {
    p->scrbar.valid     = _TRUE;
    
    p->scrbar.col_back  = col_back;
    p->scrbar.col_front = col_front;
    p->scrbar.col_frame = MakeColor(col_topleft, col_bottomright);
    p->scrbar.col_text  = col_symbol;
    
    if(p->horizontal == _TRUE)
    {
      p->scrbar.horizontal = _TRUE;
      p->wiperoom.xy2.y   -= (LEVELBUTTON_HEIGHT+1);
      p->scrbar.room       = p->room;
      p->scrbar.room.xy1.y = p->wiperoom.xy2.y + 1;
    }
    else
    {
      p->scrbar.horizontal = _FALSE;
      p->wiperoom.xy2.x   -= LEVELBUTTON_WIDTH;
      p->scrbar.room       = p->room;
      p->scrbar.room.xy1.x = p->wiperoom.xy2.x + 1;
    }
      
    p->scrbar.frametype = frame;
    
    return _TRUE;
  }
  
  return _FALSE;
}

extern "C" _MOTION* get_carousel(_MY_PICTURE *mp, _DWORD index)
{
  if(check_mp(mp) == _TRUE)
  {
    return mp->Ptr[mp->No-1].Get_MOTION(index);
  }
  
  return NULL;
}

void correct_room(_ROOM *pr, _MY_PICTURE *mp)
{
  pr->xy1.x += mp->Room.xy1.x;
  pr->xy1.y += mp->Room.xy1.y;
  pr->xy2.x += mp->Room.xy1.x;
  pr->xy2.y += mp->Room.xy1.y;
}

extern "C" _BOOL add_object(_MY_PICTURE *mp, _UWORD no, _ROOM *pr, _UDWORD overload)
{
  if(check_mp(mp) == _TRUE)
  {
    _ROOM room = *pr;
    correct_room(&room, mp);
    return mp->Ptr[mp->No-1].AddObject(no, &room, overload);
  }

  return _FALSE;
}

extern "C" _BOOL add_rectangle(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD overload)
{
  _COL_RECT crec;
  _BOOL     retcode = _FALSE;
  _BOOL     isdyncol = _FALSE;
  
  if(check_mp(mp) == _TRUE)
  {
    retcode = _TRUE;
    init_COL_RECT(&crec);
    
    crec.rectangle.attrib     = T_SOLID | T_COPY | (pstyle->contextalign & T_INVERTTHEME);
    
    // position
    crec.rectangle.room       = *pr;
    correct_room(&crec.rectangle.room, mp);
    
    crec.rectangle.color      = MakeColor(pstyle->colhatch, pstyle->colback);
    crec.rectangle.colorframe = MakeColor(pstyle->coltopleft, pstyle->colbottomright); //pstyle->colback;
    crec.rectangle.frametype  = pstyle->frametype;
    
    linear_coordinates(&crec.rectangle.room.xy1.x, &crec.rectangle.room.xy1.y, &crec.rectangle.room.xy2.x, &crec.rectangle.room.xy2.y);
  
    if(pstyle->pdyncolback != NULL)
    {
      copy_LINKSCHEME(&retcode, &crec.dyncolfront, pstyle->pdyncolback);
      isdyncol = _TRUE;
     // copy_LINKSCHEME(&retcode, &crec.dyncolback, pstyle->pdyncolback);
    } 
          
    if(pstyle->pdyncoltopleft != NULL)
    {
      copy_LINKSCHEME(&retcode, &crec.dyncoltl, pstyle->pdyncoltopleft);
      isdyncol = _TRUE;
    }
    
    if(pstyle->pdyncolbottomright != NULL)
    {
      copy_LINKSCHEME(&retcode, &crec.dyncolbr, pstyle->pdyncolbottomright);
      isdyncol = _TRUE;
    }
          
    if(overload < VarList.GetNo())
    {
      _VARLABEL vl;
      init_VARLABEL(&vl);
      vl.no            = 1;
      vl.info[0].value = overload;
      vl.info[0].state = VAR_VAL;
      overload_COL_RECT(&crec, &vl);
      free_VARLABEL(&vl);
    }
        
    if(isdyncol == _TRUE)
      retcode = mp->Ptr[mp->No-1].Add(&crec);
    else
      retcode = mp->Ptr[mp->No-1].Add(&crec.rectangle);
  }
  
  return retcode;
}

extern "C" _BOOL add_text(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD stschemeno, _ANYPARA *pany, void *txt, _UWORD chrsize, _UDWORD overload)
{
  _COL_PICTXT ctxt;
  _BOOL       retcode = _FALSE;
  _BOOL       isdyncol = _FALSE;
  
  if(check_mp(mp) == _TRUE)
  {
    retcode = _TRUE;
      
    init_COL_PICTXT(&ctxt);

    ctxt.btx.attrib    = T_SOLID | T_COPY | T_PROP | (pstyle->contextalign & (T_HORIZBOUND|T_VERTBOUND|T_INVERTTHEME));
    ctxt.btx.colback   = MakeColor(pstyle->colhatch, pstyle->colback);
    ctxt.btx.coltxt    = pstyle->colfront;
    ctxt.btx.colframe  = MakeColor(pstyle->coltopleft, pstyle->colbottomright);
    ctxt.btx.font      = pstyle->fontscheme;
    ctxt.btx.frametype = pstyle->frametype;
    ctxt.btx.room      = *pr;
    correct_room(&ctxt.btx.room, mp);
    //ctxt.btx.tip       = TIP_SINGLESHOT;
    
    if(stschemeno < StateScheme.GetNo())
      ctxt.btx.stscheme.schemeno = stschemeno;
    
    linear_coordinates(&ctxt.btx.room.xy1.x, &ctxt.btx.room.xy1.y, &ctxt.btx.room.xy2.x, &ctxt.btx.room.xy2.y);

    if(pany != NULL)
      retcode = copy_ANYPARA(&retcode, &ctxt.btx.anypara, pany);

    if(retcode == _TRUE)
    {
      if(setup_USERTEXT(&ctxt.btx.usertxt, txt, chrsize) == _TRUE)
      {
        if(pstyle->pdyncolback != NULL)
        {
          copy_LINKSCHEME(&retcode, &ctxt.dyncolfront, pstyle->pdyncolback);
          isdyncol = _TRUE;
        }
          
        if(pstyle->pdyncolfront != NULL)
        {
          copy_LINKSCHEME(&retcode, &ctxt.dyncoltxt, pstyle->pdyncolfront);
          isdyncol = _TRUE;
        }
          
        if(pstyle->pdyncoltopleft != NULL)
        {
          copy_LINKSCHEME(&retcode, &ctxt.dyncoltl, pstyle->pdyncoltopleft);
          isdyncol = _TRUE;
        }
          
        if(pstyle->pdyncolbottomright != NULL)
        {
          copy_LINKSCHEME(&retcode, &ctxt.dyncolbr, pstyle->pdyncolbottomright);
          isdyncol = _TRUE;
        }
      
        if(overload < VarList.GetNo())
        {
          _VARLABEL vl;
          init_VARLABEL(&vl);
          vl.no            = 1;
          vl.info[0].value = overload;
          vl.info[0].state = VAR_VAL;
          overload_COL_PICTXT(&ctxt, &vl);
          free_VARLABEL(&vl);
        }
        
        if(isdyncol == _TRUE)
          retcode = mp->Ptr[mp->No-1].Add(&ctxt);
        else
          retcode = mp->Ptr[mp->No-1].Add(&ctxt.btx);
      }
    }
  }

  return retcode;
}

_BOOL add_button_intern(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD stschemeno, _UDWORD fktno_press, _UDWORD fktno_release, _UDWORD varno, _UDWORD imageno_press, _UDWORD imageno_release, void *text_press, void *text_release, _UWORD chrsize, _BOOL act_momentary, _UDWORD accessno, _UDWORD hotkeycode, _UDWORD overload, _DWORD setval, _DWORD resetval, _ATTRIB imagealign, _UWORD keycode, _UWORD keycode_shift, _UWORD keycode_altgr, _UWORD keycode_altgr_shift,_TIP tip, _UBYTE selector)
{
  _BUTTON but;
  _BOOL   retcode = _FALSE;

  if(check_mp(mp) == _TRUE)
  {
    retcode = _TRUE;
    init_BUTTON(&but);
    
    but.tip        = tip;
    but.attrib     = T_SOLID | T_COPY | T_PROP | (pstyle->contextalign & (T_HORIZBOUND|T_VERTBOUND));
    but.col        = MakeColor(pstyle->colhatch, pstyle->colback);
    but.coltxt     = pstyle->colfront;
    but.framecol   = MakeColor(pstyle->coltopleft, pstyle->colbottomright);
    but.font       = pstyle->fontscheme;
    but.frametype  = pstyle->frametype;
    but.room       = *pr;
    but.keycode[0] = keycode;
    but.keycode[1] = keycode_shift;
    but.keycode[2] = keycode_altgr;
    but.keycode[3] = keycode_altgr_shift;
    but.selector   = selector;
    
    correct_room(&but.room, mp);
    
    linear_coordinates(&but.room.xy1.x, &but.room.xy1.y, &but.room.xy2.x, &but.room.xy2.y);
    
    but.wose      = make_WOSE((imagealign == 0xFFFFFFFF)? pstyle->contextalign : imagealign);
    
    if(stschemeno < StateScheme.GetNo())
      but.stscheme.schemeno = stschemeno;
    
    if(fktno_press < InterBoxList.GetNo())
      but.gfunct.pos_fl.no = fktno_press;
    if(fktno_release < InterBoxList.GetNo())
      but.gfunct.neg_fl.no = fktno_release;
    
    if(check_varno(varno) == _TRUE)
    {
      but.varlabel.no = 1;
      but.varlabel.info[0].value = varno;
      but.varlabel.info[0].state = VAR_VAL;
      but.set_val = setval;
      but.reset_val = resetval;
    } 
    
    if(imageno_press < ImageList.GetNo())
    {
      but.buttonimages.image_press = imageno_press;
      but.buttonimages.image_in_press = imageno_press;
      but.buttonimages.image_sel_press = imageno_press;
    }
    if(imageno_release < ImageList.GetNo())
    {
      but.buttonimages.image_release = imageno_release;
      but.buttonimages.image_in_release = imageno_release;
      but.buttonimages.image_sel_release = imageno_release;
    }
    but.buttonimages.zoom_images = _FALSE; // zoom images to buttonsize

    but.act_momentary = act_momentary;
    but.access        = accessno;
    but.hotkeycode    = hotkeycode;
   
    if(pstyle->pdyncolback != NULL)
    {
       copy_LINKSCHEME(&retcode, &but.dyncolfront, pstyle->pdyncolback); // released farbe
       if(pstyle->pdyncolhatch == NULL)
         copy_LINKSCHEME(&retcode, &but.dyncolback, pstyle->pdyncolback); // pressed farbe
    }

    if(pstyle->pdyncolhatch != NULL)
    {
      copy_LINKSCHEME(&retcode, &but.dyncolback, pstyle->pdyncolhatch); // pressed farbe
    }
    
    if(pstyle->pdyncolfront != NULL)
    {
      copy_LINKSCHEME(&retcode, &but.dyncoltxt, pstyle->pdyncolfront);
    }
    
    if(pstyle->pdyncoltopleft != NULL)
    {
      copy_LINKSCHEME(&retcode, &but.dyncoltl, pstyle->pdyncoltopleft);
    }

    if(pstyle->pdyncolbottomright != NULL)
    {
      copy_LINKSCHEME(&retcode, &but.dyncolbr, pstyle->pdyncolbottomright);
    }

    if(retcode == _TRUE)
    {
      if(setup_USERTEXT(&but.usertext, text_release, chrsize) == _TRUE)
      {
        if(setup_USERTEXT(&but.usertext1, text_press, chrsize) == _TRUE)
        {
          if(overload < VarList.GetNo())
          {
            _VARLABEL vl;
            init_VARLABEL(&vl);
            vl.no            = 1;
            vl.info[0].value = overload;
            vl.info[0].state = VAR_VAL;
            overload_BUTTON(&but, &vl);
            free_VARLABEL(&vl);
          }
        
          return mp->Ptr[mp->No-1].Add(&but);
        }
      }
    }
  }
  
  return _FALSE;
}

extern "C" _BOOL add_button(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD stschemeno, _UDWORD fktno_press, _UDWORD fktno_release, _UDWORD varno, _UDWORD imageno_press, _UDWORD imageno_release, void *text_press, void *text_release, _UWORD chrsize, _BOOL act_momentary, _UDWORD accessno, _UDWORD hotkeycode, _UDWORD overload, _DWORD setval, _DWORD resetval, _ATTRIB imagealign)
{
  return add_button_intern(mp, pstyle, pr, stschemeno, fktno_press, fktno_release, varno, imageno_press, imageno_release, text_press, text_release, chrsize, act_momentary, accessno, hotkeycode, overload, setval, resetval, imagealign, _NOKEY, _NOKEY, _NOKEY, _NOKEY, _TIPSINGLE, _SELECTOR_ONLY_HID);
}

extern "C" _BOOL add_key(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD imageno, void *text, _UWORD chrsize, _ATTRIB imagealign, _UWORD keycode, _UWORD keycode_shift, _UWORD keycode_altgr, _UWORD keycode_altgr_shift)
{
  return add_button_intern(mp, pstyle, pr, DEFSCOPE, DEFSCOPE, DEFSCOPE, DEFSCOPE, imageno, imageno, text, text, chrsize, _TRUE, 0, 0, DEFSCOPE, 1, 0, imagealign, keycode, keycode_shift, keycode_altgr, keycode_altgr_shift, _TIPCOMBI, _SELECTOR_ONLY_HID);
}

_BOOL setup_IO(_IO *pio, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD stschemeno, _UDWORD varno, _IO_TYPE iotype, _UDWORD overload)
{
  _BOOL retcode = _FALSE;

  if(check_varno(varno) == _TRUE)
  {
    retcode = _TRUE;
      
    pio->variable.no = 1;
    pio->variable.info[0].value = varno;
    pio->variable.info[0].state = VAR_VAL;
    
        //_IOINFO      info;        // special information (depends on typ)
        //_PICFORMAT   picformat;   // overloaded format
    
    pio->typ = iotype;
    
    pio->attrib    = T_SOLID | T_COPY | T_PROP | (pstyle->contextalign & (T_HORIZBOUND|T_VERTBOUND|T_INVERTTHEME));
    pio->room      = *pr;
    linear_coordinates(&pio->room.xy1.x, &pio->room.xy1.y, &pio->room.xy2.x, &pio->room.xy2.y);
    
    pio->font      = pstyle->fontscheme;
    pio->frametype = pstyle->frametype;
    pio->colback   = MakeColor(pstyle->colhatch, pstyle->colback);
    pio->coltxt    = pstyle->colfront;
    pio->colframe  = MakeColor(pstyle->coltopleft, pstyle->colbottomright);

    if(stschemeno < StateScheme.GetNo())
      pio->stscheme.schemeno = stschemeno;
    
    if(pstyle->pdyncolback != NULL)
    {
      copy_LINKSCHEME(&retcode, &pio->dyncolfront, pstyle->pdyncolback); // released farbe
      copy_LINKSCHEME(&retcode, &pio->dyncolback, pstyle->pdyncolback); // pressed farbe
      pio->is_dyncolor = _TRUE;
    }
    
    if(pstyle->pdyncolfront != NULL)
    {
      copy_LINKSCHEME(&retcode, &pio->dyncoltxt, pstyle->pdyncolfront);
      pio->is_dyncolor = _TRUE;
    }
    
    if(pstyle->pdyncoltopleft != NULL)
    {
      copy_LINKSCHEME(&retcode, &pio->dyncoltl, pstyle->pdyncoltopleft);
      pio->is_dyncolor = _TRUE;
    }

    if(pstyle->pdyncolbottomright != NULL)
    {
      copy_LINKSCHEME(&retcode, &pio->dyncolbr, pstyle->pdyncolbottomright);
      pio->is_dyncolor = _TRUE;
    }
    
  }
  
  return retcode;
}

extern "C" _BOOL add_output(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD stschemeno, _UDWORD varno, _IO_TYPE iotype, _UDWORD overload)
{
  _IO io;

  if(check_mp(mp) == _TRUE)
  {
    if((iotype == AS_NUMERIC)||(iotype == AS_NUMERIC_HIDE)||(iotype == AS_ASCII)||(iotype == AS_ASCII_HIDE)||(iotype == AS_CHKBOX)||(iotype == AS_RADIO)||(iotype == AS_DATE)||(iotype == AS_TIME)||(iotype == AS_IPADDRESS)||(iotype == AS_ENUMERATION))
    {
      init_IO_Makro(&io);
      _ROOM room = *pr;
      correct_room(&room, mp);
      if(setup_IO(&io, pstyle, &room, stschemeno, varno, iotype, overload) == _TRUE)
      {
        return mp->Ptr[mp->No-1].Add(&io);
      }
    }
  }

  return _FALSE;
}

extern "C" _BOOL add_myio(_MY_PICTURE *mp, _ROOM *pr, _UDWORD stschemeno, _UDWORD varno, _BOOL input, _ASCII *objectname, _UDWORD overload)
{
  _INPUT ipt;
  _STYLESHEET style;

  if(check_mp(mp) == _TRUE)
  {
    init_INPUT_Makro(&ipt);
    _ROOM room = *pr;
    correct_room(&room, mp);
    init_STYLESHEET(&style);
    
    ipt.io.virtname.pthis = get_this_pointer(objectname);
    if(ipt.io.virtname.pthis != NULL)
    {
      if(setup_IO(&ipt.io, &style, &room, stschemeno, varno, AS_NUMERIC, overload) == _TRUE)
      {
        _BOOL retcode = _FALSE;
        
        if(input == _TRUE)
          retcode = mp->Ptr[mp->No-1].Add(&ipt);
        else   
          retcode = mp->Ptr[mp->No-1].Add(&ipt.io);
         
        if(retcode == _TRUE)
        {
         #ifndef CUTOFF_USER
          void *ptr = find_lasal_method(ipt.io.virtname.pthis, METHOD_MYIO_START);
          if(ptr != NULL)
          {
            void *tmp = ipt.io.virtname.pthis;
            _IO *pio = &ipt.io;
            load_this(tmp);
            ((_FVOID_PT_BOOL)ptr)(pio, _TRUE);            // call start methode
          }
         #endif 
          return _TRUE;
        }
      }
    }
  }

  return _FALSE;
}

extern "C" _BOOL add_input(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD stschemeno, _UDWORD varno, _IO_TYPE iotype, _UDWORD overload)
{
  _INPUT inpt;

  if(check_mp(mp) == _TRUE)
  {
    if((iotype == AS_NUMERIC)||(iotype == AS_NUMERIC_HIDE)||(iotype == AS_ASCII)||(iotype == AS_ASCII_HIDE)||(iotype == AS_CHKBOX)||(iotype == AS_RADIO)||(iotype == AS_DATE)||(iotype == AS_TIME)||(iotype == AS_IPADDRESS)||(iotype == AS_ENUMERATION))
    {
      init_INPUT_Makro(&inpt);
      _ROOM room = *pr;
      correct_room(&room, mp);
      if(setup_IO(&inpt.io, pstyle, &room, stschemeno, varno, iotype, overload) == _TRUE)
      {
        return mp->Ptr[mp->No-1].Add(&inpt);
      }
    }
  }
  
  return _FALSE;
}

extern "C" _BOOL add_imagescheme(_MY_PICTURE *mp, _ROOM *pr, _UDWORD imageschemeno, _UDWORD stschemeno, _UDWORD varno, _UDWORD overload)
{
  _IO io;

  if(check_mp(mp) == _TRUE)
  {
    init_IO_Makro(&io);
    _ROOM room = *pr;
    correct_room(&room, mp);
    
    _STYLESHEET style;
    init_STYLESHEET(&style);
    
    io.info.tbo.list          = IMAGESCHEME;
    io.info.myscheme.schemeno = imageschemeno;
    if(setup_IO(&io, &style, &room, stschemeno, varno, AS_TBO, overload) == _TRUE)
    {
      return mp->Ptr[mp->No-1].Add(&io);
    }
  }
  
  return _FALSE;
}

extern "C" _BOOL add_image(_MY_PICTURE *mp, _ROOM *pr, _UDWORD imageno, _BMP *pbmp)
{
  _BITMAP bmp;
  if(check_mp(mp) == _TRUE)
  {
    init_BITMAP(&bmp);

    bmp.attrib = T_SOLID | T_COPY;
    bmp.room   = *pr;
    correct_room(&bmp.room, mp);
    
    linear_coordinates(&bmp.room.xy1.x, &bmp.room.xy1.y, &bmp.room.xy2.x, &bmp.room.xy2.y);
    
    if(imageno < ImageList.GetNo())
    {
      bmp.no = imageno;
      return mp->Ptr[mp->No-1].Add(&bmp);
    }
    else if(pbmp != NULL)
    {
      if(Memory2((void**)&bmp.userbmp, sizeof(_BMP)) != 0)
      {
        init_BMP(bmp.userbmp);
        if(copy_BMP(bmp.userbmp, pbmp) == _TRUE)
        {
          if(zoom_BMP(bmp.userbmp, bmp.room.xy2.x - bmp.room.xy1.x + 1, bmp.room.xy2.y - bmp.room.xy1.y + 1) == _TRUE)
            return mp->Ptr[mp->No-1].Add(&bmp);
        }
      }
    }
      
    free_BITMAP(&bmp);
  }
  
  return _FALSE;
}