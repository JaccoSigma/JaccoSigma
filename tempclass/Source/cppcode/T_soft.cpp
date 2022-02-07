#include "T_IPC.H"
#include "T_NUC.H"

#define SA34115

_BOOL chk_picture_access(_LSEFUNCT *p, _BOOL system)
{
  _BOOL       retcode = _TRUE;
  _STACK      *ps = NULL;
  _STACK_LINE *pl;
  _RESULT     op;
  _VARIABLE   var;
  _UWORD      no, i;

  if(p->no != 0xFFFF)
  {
    ps = InterBoxList.Get(p->no)->GetStack();
  }
  else if(p->overload.no > 0)
  {
    ps = &p->overload;
  }

  if(ps != NULL)
  {
    pl = ps->ptr;
    for(i=0; i<ps->no; i++)
    {
      switch(pl->cmd)
      {
        case CMD_NEWWINDOW  :
          retcode = _FALSE;
          if(VarList.GetSystemData(&op, &var, &pl->op1, _TRUE, _TRUE) == _TRUE)
          {
            no = (_UWORD)op.value;
            retcode = Project.GetWindowAccess(NULL, &no, system, NULL);
          }
          break;
        case CMD_NEWPICTURE :
          retcode = _FALSE;
          if(VarList.GetSystemData(&op, &var, &pl->op1, _TRUE, _TRUE) == _TRUE)
          {
            no = (_UWORD)op.value;
            retcode = Project.GetPictureAccess(NULL, &no, system, NULL);
          }
          break;
        default :
          break;
      }

      pl++;
    }
  }

  return(retcode);
}

void _Softkey::Init(void)
{
  SoftkeyText.Init();
  ActFloor       = 0;
  NoFloor        = 0;
  NoSoftkeys     = 8;
  Ptr            = NULL;
  CopyOfPrevious = _FALSE;
 #ifdef XUSER_KM
  AssignedImage  = 0xFFFF;
  PicNoMemo      = 0;
 #endif
 #ifdef UC_DYN_SOFTKEYS
  RunCount = UC_DYN_SOFTKEYS;
 #endif
}

void _Softkey::SetCopyOfPrevious(_BOOL state)
{
  CopyOfPrevious = state;
}

_BOOL _Softkey::GetCopyOfPrevious(void)
{
  return(CopyOfPrevious);
}


void _Softkey::Free(void)
{
  _UWORD i;

  for(i=0; i<NoFloor; i++)
    free_SOFTFLOOR(&Ptr[i]);

  Memory2((void**)&Ptr, 0);

  SoftkeyText.Free();
  Init();
}

_BOOL _Softkey::Copy(_Softkey *Ps)
{
  _BOOL       retcode = _FALSE;
  _SOFTFLOOR  *sf;
  _UWORD      no, i;
  _DWORD      size;

  Free();

  no   = Ps->GetNoFloor();      // no of floors
  sf   = Ps->GetSoftFloor();    //
  size = no * sizeof(_SOFTFLOOR);

  if(Memory2((void**)&Ptr, size) != 0)
  {
    retcode = _TRUE;
    for(i=0; i<no; i++)
    {
      init_SOFTFLOOR(&Ptr[i]);
      copy_SOFTFLOOR(&retcode, &Ptr[i], &sf[i]);
    }

    NoFloor       = no;
    NoSoftkeys    = Ps->GetNoSoftkeys();
   #ifdef XUSER_KM
    AssignedImage = Ps->GetAssignedImage();
   #endif

    if(retcode == _TRUE)
      retcode = SoftkeyText.Copy(Ps->GetTextPuffer());
  }

  if(retcode == _FALSE)
    Free();

  return(retcode);
}

void _Softkey::GetSize(_ROOM *room)
{
//old  GetScreenSize(room);
//old  room->xy1.y = room->xy2.y - SetUp.GetSoftkeyHeight() + 1;

  _ROOM         r, screen;
  _SOFTKEYSETUP *pss;

  MyGetScreenSize(&screen);
 #ifdef ZOOM_KM
  screen.xy2.y -= SOFTKEY_NEG_OFF_X;
 #endif
  pss = SetUp.Get_SOFTKEYSETUP();

  if(pss->horizontal == _TRUE)
  {
    switch(pss->align & T_HORIZBOUND)
    {
      case T_LEFTBOUND   : r.xy1.x = screen.xy1.x; break;
      case T_CENTERBOUND : r.xy1.x = ((screen.xy1.x+screen.xy2.x+1)-(pss->no*pss->width))/2; break;
      default            : r.xy1.x = screen.xy2.x+1-(pss->no*pss->width); break;
    }
    r.xy2.x = r.xy1.x + (pss->no * pss->width) - 1;

    switch(pss->align & T_VERTBOUND)
    {
      case T_UPBOUND  : r.xy1.y = screen.xy1.y; break;
      case T_MIDBOUND : r.xy1.y = (screen.xy1.y+screen.xy2.y+1-pss->height)/2; break;
      default         : r.xy1.y = screen.xy2.y - pss->height + 1; break;
    }
    r.xy2.y = r.xy1.y + pss->height - 1;
  }
  else
  {
    switch(pss->align & T_HORIZBOUND)
    {
      case T_LEFTBOUND   : r.xy1.x = screen.xy1.x; break;
      case T_CENTERBOUND : r.xy1.x = (screen.xy1.x+screen.xy2.x+1-pss->width)/2; break;
      default            : r.xy1.x = screen.xy2.x-pss->width; break;
    }
    r.xy2.x = r.xy1.x + pss->width - 1;

    switch(pss->align & T_VERTBOUND)
    {
      case T_UPBOUND  : r.xy1.y = screen.xy1.y; break;
      case T_MIDBOUND : r.xy1.y = ((screen.xy1.y+screen.xy2.y+1)-(pss->no*pss->height))/2; break;
      default         : r.xy1.y = screen.xy2.y+1-(pss->no*pss->height); break;
    }
    r.xy2.y = r.xy1.y + (pss->no * pss->height) - 1;
  }

  *room = r;
}

void _Softkey::GetSingleSize(_ROOM *p, _UWORD no)
{
//old  _KOORD sdx, width, offset;
//old  _ROOM  room;
//old  GetScreenSize(&room);
//old  sdx    = room.xy2.x + 1 - room.xy1.x;
//old  width  = sdx / NoSoftkeys;
//old  offset = (sdx - (NoSoftkeys * width)) / 2;
//old  p->xy1.x = offset + width * no;
//old  p->xy2.x = p->xy1.x + width - 1;
//old  p->xy1.y = room.xy2.y - SetUp.GetSoftkeyHeight() + 1;
//old  p->xy2.y = room.xy2.y;

  _SOFTKEYSETUP *pss;
  _ROOM r;

  pss = SetUp.Get_SOFTKEYSETUP();
  GetSize(&r);

  if(pss->horizontal == _TRUE)
  {
    r.xy1.x += pss->width * no;
    r.xy2.x = r.xy1.x + pss->width - 1;
  }
  else
  {
    r.xy1.y += pss->height * no;
    r.xy2.y = r.xy1.y + pss->height - 1;
  }

  *p = r;
}

_BOOL _Softkey::KeymodeSingleSoftkey(_UWORD sno, _TIP tip)
{
  _UWORD kc = 0;

  switch(sno)
  {
    case  0 : kc = _F1; break;
    case  1 : kc = _F2; break;
    case  2 : kc = _F3; break;
    case  3 : kc = _F4; break;
    case  4 : kc = _F5; break;
    case  5 : kc = _F6; break;
    case  6 : kc = _F7; break;
    case  7 : kc = _F8; break;
    case  8 : kc = _F9; break;
    case  9 : kc = _F10; break;
    case 10 : kc = _F11; break;
    case 11 : kc = _F12; break;
  }

  if(kc != 0)
  {
    set_keymode(kc, tip);
    return(_TRUE);
  }

  return(_FALSE);
}

_BOOL _Softkey::Draw(void)
{
 #ifdef UC_SOFTKEY

 #ifdef UC_DYN_SOFTKEYS
  ScreenStop();
  _BOOL retcode = Refresh(_TRUE);
  ScreenStart();
  #ifdef UC_ROTATE
  _ROOM sroom;
  GetSize(&sroom);  
  RefreshRectangle(&sroom);
  #endif  
  return retcode;
 #else
  _UWORD      i;
  _BOOL       retcode = _FALSE;
  _SINGLESOFT *pss;
  if(ActFloor < NoFloor)
  {
    pss = &Ptr[ActFloor].floor[0];
    for(i=0; i<NoSoftkeys; i++)
    {
//			DrawSingleSoftkey(&Ptr[ActFloor].floor[i], i, chk_CHKBIT(&Ptr[ActFloor].floor[i].chkbit));
//      KeymodeSingleSoftkey(i, Ptr[ActFloor].floor[i].tip);
      pss->state = _FALSE; // beim ausgeben der gesamten leiste, alle softkeys ungedrückt malen
			DrawSingleSoftkey(pss, i, chk_CHKBIT(&pss->chkbit), 0);
      KeymodeSingleSoftkey(i, pss->tip);
      pss ++;
    }

    retcode = _TRUE;
  }
  else
  {
    for(i=0; i<NoSoftkeys; i++)
    {
      DrawSingleSoftkey(NULL, i, _TRUE, 0);
    }
  }
  return(retcode);
 #endif

 #else
  return(_TRUE);
 #endif
}

_DWORD _Softkey::Set(_DWORD floorno)
{
  _DWORD retcode = -1;

  if(floorno < 0)
  {
    floorno = ActFloor + 1;
    if(floorno >= NoFloor)
      floorno = 0;
  }

  if(floorno < NoFloor)
	{
    if(floorno != ActFloor)
    {
      ActFloor = floorno;
      DoAllNegEdge(_FALSE);
    }

    retcode = ActFloor;
  }

  return(retcode);
}

_BOOL _Softkey::ChkAccess(_SINGLESOFT *ps, _BOOL system)
{
  _BOOL retcode = _FALSE;

 #ifdef XUSER_KM
  if(chk_ACCESS(ps->access, system) == _TRUE)
 #else
  if((chk_ACCESS(ps->access, system) == _TRUE) &&
     (chk_picture_access(&ps->gfunct.pos_fl, system) == _TRUE) &&
     (chk_picture_access(&ps->gfunct.neg_fl, system) == _TRUE))
 #endif
  {
    retcode = _TRUE;
  }

  return(retcode);
}


#ifdef UC_DYN_SOFTKEYS


#define rotate_crc_makro(_p) ((_p)&0x80000000)? ((_p) = ((_p) << 1)|1) : ((_p) <<= 1) 


//void rotate_crc(_CRC32 *crc)
//{
//  if(*crc & 0x80000000)
//    *crc = ((*crc << 1) | 1);
//  else
//    *crc <<= 1;
//}

_BOOL crc_SINGLESOFT(_SINGLESOFT *pss, _BOOL state, _BOOL init)
{
  _BOOL retcode = _FALSE;
//  _RESULT result;
//  _VARIABLE variable;
  _CRC32 crc;
  _COLOR col0, col1;
  _CHAR *txt;
  _BOOL sst = _FALSE;

  if(pss->act_momentary == _FALSE)
    get_stationary_state(&sst, &pss->varlabel, pss->set_val, pss->reset_val);  
  
  //if(VarList.GetSystemData(&result, &variable, &pss->varlabel, _FALSE, _TRUE) == _TRUE)
  {
    crc = (sst == _TRUE)? 1 : 0; //crc_numeric(&result, &variable);
    if(ColorScheme.Call(&col0, &pss->dyncolback, GetBackColor_Makro(pss->color)) == _TRUE)
    {
      if(ColorScheme.Call(&col1, &pss->dyncolfront, GetPenColor_Makro(pss->color)) == _TRUE)
      {
        pss->color = MakeColor_Makro(col0, col1);

        rotate_crc_makro(crc);
        crc ^= pss->color;
      }
    }

    if(ColorScheme.Call(&col0, &pss->dyncoltl, GetBackColor_Makro(pss->frame)) == _TRUE)
    {
      if(ColorScheme.Call(&col1, &pss->dyncolbr, GetPenColor_Makro(pss->frame)) == _TRUE)
      {
        pss->frame = MakeColor_Makro(col0, col1);

        rotate_crc_makro(crc);
        crc ^= pss->frame;
      }
    }

    if(ColorScheme.Call(&col0, &pss->dyncoltxt, GetPenColor_Makro(pss->text)) == _TRUE)
    {
      pss->text = MakeColor_Makro(INVISIBLE, col0);

      rotate_crc_makro(crc);
      crc ^= pss->text;
    }

    txt = get_text(&pss->anypara.anything, NULL, &pss->anypara.memvarlabel, _TRUE);
    if(txt != NULL)
    {
      rotate_crc_makro(crc);
      crc ^= crc_string(txt, sizeof(_CHAR));
    }

    rotate_crc_makro(crc);
    crc ^= state;

  }


  if(((_CRC32)pss->cstate.value != crc) || (init == _TRUE))// || (pss->cstate.init == _TRUE))
  {
    pss->cstate.value = crc;
    pss->cstate.init = _FALSE;
    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL _Softkey::Refresh(_BOOL init)
{
  _BOOL retcode = _FALSE;
  _SINGLESOFT *pss;
  _UWORD i;
  _BOOL state;
  _UDWORD tmp;


  if((RunCount >= UC_DYN_SOFTKEYS) || (init == _TRUE))
  {
    RunCount = 0;

    if(ActFloor < NoFloor)
    {
      retcode = _TRUE;

      pss = &Ptr[ActFloor].floor[0];

      for(i=0; i<NoSoftkeys; i++,pss++)
      {
        state = _TRUE;

        if((chk_CHKBIT(&pss->chkbit) == _FALSE) || (StateScheme.Call(&tmp, &pss->stscheme, NULL, STATE_ACTIVE) == _FALSE) || (tmp != STATE_ACTIVE))
          state = _FALSE;

        if(crc_SINGLESOFT(pss, state, init) == _TRUE)
          DrawSingleSoftkey(pss, i, state, tmp);

      }
    }
  }

  RunCount++;

  return(retcode);
}
#endif

#if COMPILER_VERSION >= 28

void softkey_image(_ROOM *pr, _UBYTE wose, _UWORD imageno)
{
  _BMP   *bmp;
  _WORD  width, height;
  _ROOM  room;
  _DWORD fakt, faktx, fakty;
  _KOORD x, y;

  bmp = ImageList.Get(imageno);
  if(bmp != NULL)
  {
    width  = pr->xy2.x - pr->xy1.x + 1;
    height = pr->xy2.y - pr->xy1.y + 1;

    if(((bmp->width <= width) && (bmp->height <= height)) || (width <= 1) || (height <= 1))
    {
      // sa10634 begin
      switch((wose >> 2) & 0x03)
      {
        case 0x01 : x = pr->xy1.x; break;
        case 0x02 : x = pr->xy2.x - bmp->width + 1; break;
        default   : x = pr->xy1.x + (width - bmp->width) / 2; break;
      }
      switch(wose & 0x03)
      {
        case 0x01 : y = pr->xy1.y; break;
        case 0x02 : y = pr->xy2.y - bmp->height + 1; break;
        default   : y = pr->xy1.y + (height - bmp->height) / 2; break;
      }
      // sa10634 end
      Bitmap(x, y, bmp, T_SOLID|T_COPY, 0);
    }
    else
    {
      room  = *pr;
      faktx = (width * 1000) / bmp->width;
      fakty = (height * 1000) / bmp->height;
      fakt  = (faktx < fakty)? faktx : fakty;

      width  = (bmp->width * fakt) / 1000;
      height = (bmp->height * fakt) / 1000;
      x      = (room.xy1.x + room.xy2.x + 1) / 2;
      y      = (room.xy1.y + room.xy2.y + 1) / 2;

      room.xy1.x = x - width / 2;
      room.xy1.y = y - height / 2;
      room.xy2.x = room.xy1.x + width - 1;
      room.xy2.y = room.xy1.y + height - 1;
      out_zoom_BMP(&room, bmp, T_COPY|T_SOLID);
    }
  }
}

void _Softkey::DrawSingleSoftkey(_SINGLESOFT *ps, _UWORD no, _BOOL chkbit, _UDWORD state_scheme_result)
{
#ifdef UC_SOFTKEY
  _ATTRIB att;
  _COLOR  colframe, invisible;
  _ROOM   room, inlet;
//  _UWORD  thick;
  _CHAR   *txt;
  _BOOL   selected = _FALSE;
//  _IVALUE value;
  _BOOL   state;

  GetSingleSize(&room, no);

 #ifdef UC_MOUSE
  _UWORD hide = IfNecHideMouseCursor(&room);
 #endif

  if(ps != NULL)
  {
    if(ps->valid)
    {
      invisible = MakeColor_Makro(INVISIBLE, INVISIBLE);

      colframe = ps->frame;

      state = ps->state;
      
      if(ps->act_momentary == _FALSE)
        get_stationary_state(&state, &ps->varlabel, ps->set_val, ps->reset_val);
      //if(VarList.GetValue(&ps->varlabel, &value, _TRUE) == _TRUE)
      //  state = (value == 0)? _FALSE : _TRUE;

      if(ps->attrib & T_INVERTTHEME)
        state = (state == _TRUE)? _FALSE : _TRUE;

      if(state == _TRUE)
        colframe = MakeColor_Makro(GetPenColor_Makro(colframe), GetBackColor_Makro(colframe));

      att = ((ps->attrib & (T_XSIZE|T_YSIZE))|T_COPY|T_SOLID|T_FILL);

     #ifdef UC_THEME
      if((ps->frametype & 0x7F) >= _THEMEFRAME)
      {
        _COLOR tcl = ps->color;
        if(state == _TRUE)
          tcl = MakeColor_Makro(GetPenColor_Makro(tcl), GetBackColor_Makro(tcl));
        draw_THEME(&room, ps->frametype, att, tcl, state, _FALSE);
       #ifdef ZOOM_KM
        inlet = room;
       #endif
      }
      else
     #endif
      {
        DrawButton(&room, att, ps->frametype, ps->color, colframe);
       #ifdef ZOOM_KM
        GetTextRoom(&inlet, &room, ps->attrib, ps->frametype, _FALSE);
       #endif
      }

     #ifndef ZOOM_KM
      GetTextRoom(&inlet, &room, ps->attrib, ps->frametype, _FALSE);
     #endif

      if(selected == _TRUE)
      {
        softkey_image(&inlet, ps->wose, (state == _TRUE)? ps->buttonimages.image_sel_press : ps->buttonimages.image_sel_release);
      }
      else
      {
        if(state_scheme_result == 0)
          softkey_image(&inlet, ps->wose, (state == _TRUE)? ps->buttonimages.image_press : ps->buttonimages.image_release);
        else
          softkey_image(&inlet, ps->wose, (state == _TRUE)? ps->buttonimages.image_in_press : ps->buttonimages.image_in_release);
        
      }

      if((ChkAccess(ps, _TRUE) == _TRUE)&&(chkbit == _TRUE))
      {
       #ifdef XUSER_KM
        _BOOL  original_image_valid = _TRUE;
        _UWORD imageno = AllSoftkeys.GetImageNo(ps, no, &original_image_valid, GetAssignedImage());
        if((imageno > ImageList.GetNo())&&(original_image_valid == _TRUE))
          imageno = ps->buttonimages.image_describe;
        softkey_image(&inlet, ps->wose, imageno);
       #else
        // describing image
        softkey_image(&inlet, ps->wose, ps->buttonimages.image_describe);
       #endif

        // text
        txt = get_text(&ps->anypara.anything, &SoftkeyText, &ps->anypara.memvarlabel, _TRUE);
        if(txt != NULL)
        {
         #ifdef UC_THEME
          if((ps->frametype & 0x7F) >= _THEMEFRAME) // wegen kompatibilität zu früher
          {
            _REGION actreg, ioreg=NULL, newreg=NULL;
            actreg = ActivateAndRegion(&ioreg, &newreg, &room);
            OutMultiTextAlign(txt, 0, &inlet, ps->font, ps->attrib, _DEFFRAME_NOCAREREGION, MakeColor_Makro(INVISIBLE, GetPenColor_Makro(ps->text)), invisible, invisible, sizeof(_CHAR));
            DeactivateAndRegion(actreg, &ioreg, &newreg);
          }
          else
         #endif
          {
            OutMultiTextAlign(txt, 0, &room, ps->font, ps->attrib, _DEFFRAME, MakeColor_Makro(INVISIBLE, GetPenColor_Makro(ps->text)), invisible, invisible, sizeof(_CHAR));
          }
        }
      }
    }
   #ifndef SA34115 // rauhofer will das nicht
    else
    {
      Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY|T_SOLID, SetUp.GetBackgroundColor());
    }
   #endif
  }

 #ifdef UC_ROTATE
  RefreshRectangle(&room);
 #endif

 #ifdef UC_MOUSE
	IfNecShowMouseCursor(hide);
 #endif
#endif
}

#else
void _Softkey::DrawSingleSoftkey(_SINGLESOFT *ps, _UWORD no, _BOOL chkbit, _UDWORD state_scheme_result)
{
#ifdef UC_SOFTKEY
  _ATTRIB att;
  _COLOR  col, coltxt, colframe, colback;
  _ROOM   room;
  _REGION actreg, ioreg=NULL, newreg=NULL;
  _WORD   x, y, width, height;
  _DWORD  fakt, faktx, fakty;
  _BMP    *bmp;
  _UWORD  thick;
  _CHAR   *txt;

  GetSingleSize(&room, no);

 #ifdef UC_MOUSE
  _UWORD hide = IfNecHideMouseCursor(&room);
 #endif

//  if(ps == NULL)
//  {
//    att = T_DUPX|T_DUPY|T_COPY|T_DOTTED|T_INVRS|T_FILL;
//    DrawButton(&room, att, _3DFRAME, MakeColor_Makro(BLACK, LIGHTGRAY), MakeColor_Makro(WHITE, DARKGRAY));
//  }
//  else
  if(ps != NULL)
  {
    col = ps->frame;
    if(ps->state == _TRUE)
      col = MakeColor_Makro(GetPenColor_Makro(col), GetBackColor_Makro(col));

    att = ((ps->attrib & (T_XSIZE|T_YSIZE))|T_COPY|T_SOLID|T_FILL);
//    att = T_DUPX|T_DUPY|T_COPY|T_SOLID|T_FILL;
    DrawButton(&room, att, ps->frametype, ps->color, col);
//    Button(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, att, ps->color, col);

    if(ChkAccess(ps) == _TRUE)
    {
      thick = GetXSize_Makro(att);
      if(thick == 0)
        thick = 1;

      room.xy1.x += thick;
      room.xy1.y += thick;
      room.xy2.x -= thick;
      room.xy2.y -= thick;
//      room.xy2.x ++; //= thick;
//      room.xy2.y ++; //= thick;
//      actreg = ActivateAndRegion(&ioreg, &newreg, &room, T_COPY|T_SOLID);
      actreg = ActivateAndRegion(&ioreg, &newreg, &room);
      att = (ps->attrib & (T_ITALIC|T_BOLD|T_STRIKEOUT|T_UNDERLINE|T_PROP)) | T_MIDBOUND | T_CENTERBOUND | T_SOLID | T_COPY;
      col = MakeColor_Makro(GetPenColor_Makro(ps->color), GetPenColor_Makro(ps->text));
      coltxt   = MakeColor_Makro(INVISIBLE, col);
      colback  = MakeColor_Makro(GetBackColor_Makro(col), GetBackColor_Makro(col));
      colframe = MakeColor_Makro(INVISIBLE, INVISIBLE);

      if(ps->anypara.anything.list == ICONLST)
      {
        bmp = ImageList.Get(ps->anypara.anything.no);
        if(bmp != NULL)
        {
          width  = room.xy2.x - room.xy1.x + 1;
          height = room.xy2.y - room.xy1.y + 1;

          if((bmp->width <= width) && (bmp->height <= height))
          {
            x = room.xy1.x + (width - bmp->width) / 2;
            y = room.xy1.y + (height - bmp->height) / 2;
            Bitmap(x, y, bmp, T_SOLID|T_COPY, 0);
          }
          else
          {
            faktx = (width * 1000) / bmp->width;
            fakty = (height * 1000) / bmp->height;
            fakt  = (faktx < fakty)? faktx : fakty;

            width  = (bmp->width * fakt) / 1000;
            height = (bmp->height * fakt) / 1000;
            x      = (room.xy1.x + room.xy2.x + 1) / 2;
            y      = (room.xy1.y + room.xy2.y + 1) / 2;

            room.xy1.x = x - width / 2;
            room.xy1.y = y - height / 2;
            room.xy2.x = room.xy1.x + width - 1;
            room.xy2.y = room.xy1.y + height - 1;
            out_zoom_BMP(&room, bmp, T_COPY|T_SOLID);
          }
        }
      }
      else
      {
        txt = get_ANYPARA(&ps->anypara, &SoftkeyText);
        if(txt != NULL)
          OutMultiTextAlign(txt, 0, &room, ps->font, att, ps->frametype, coltxt, colback, colframe, sizeof(_CHAR));
      }
      DeactivateAndRegion(actreg, &ioreg, &newreg);
    }
  }

 #ifdef UC_MOUSE
	IfNecShowMouseCursor(hide);
 #endif
#endif
}
#endif

_BOOL _Softkey::Do(_UWORD no, _Window *Window, _EVENT* pe)
{
  _BOOL       retcode = _FALSE;
 #ifdef UC_SOFTKEY
	_SINGLESOFT *ps;

	if((Ptr != NULL) && (no < NoSoftkeys) && (ActFloor < NoFloor))
	{
		ps = &Ptr[ActFloor].floor[no];

		if(chk_CHKBIT(&ps->chkbit) == _TRUE)
		{
     #ifdef UC_DYN_SOFTKEYS
      _UDWORD tmp;
      if(StateScheme.Call(&tmp, &ps->stscheme, NULL, STATE_ACTIVE) == _FALSE)
        tmp = STATESCHEME_PRESELECTION;
      if(tmp == STATE_ACTIVE)
     #endif

      {
			  switch(pe->ftype)
			  {
				  case _EVENT_KEYPRESS   : 
                                  #ifdef UC_BUBBLE_HELP
                                   if(GetLseServer(LSE_ACTIVATEBUBBLEHELP))
                                   {
                                     _ROOM xroom;    
                                     GetSingleSize(&xroom, no);
                                     NewBubbleHelp.Call(xroom.xy1.x, xroom.xy1.y, &ps->newbubbletext, &ps->varlabel);
                                     return _FALSE;
                                   }        
                                  #endif
                                  
                                   ps->state = _TRUE;
                                   if(ChkAccess(ps, _FALSE) == _TRUE)
                                   {
                                     if(pe->selfmade != 2)
                                       ps->state = button_press(&ps->varlabel, ps->act_momentary, ps->set_val, ps->reset_val, pe->lrm);
                                    #ifdef XUSER_KM
                                     AllSoftkeys.DoKey(ps, no, Window, _FALSE, pe->lrm);
                                    #else
                                     do_LSEFUNCT_lrm(&ps->gfunct.pos_fl, Window, pe->lrm);
                                    #endif
                                   }
                                   
                                   RefreshSingleSoftkey(ps, no);
                                   retcode = _TRUE;
                                   break;
                                   
          case _EVENT_KEYRELEASE : ps->state = _FALSE;
                                   if(ChkAccess(ps, _FALSE) == _TRUE)
                                   {
                                     button_release(&ps->varlabel, ps->act_momentary, ps->set_val, ps->reset_val, pe->lrm);
                                     do_LSEFUNCT_lrm(&ps->gfunct.neg_fl, Window, pe->lrm);
                                   }
                                   RefreshSingleSoftkey(ps, no);
                                   retcode = _TRUE;
                                   break;
                                   
          default                : break;
        }
		  }
		}
	}
 #endif

 return(retcode);
}

_BOOL _Softkey::Read(_File *File)
{
  // [ .... softtext
  // [][] .. no softkeys / floor
  // [][] .. no floor
  // [ ..... information

  _BOOL   retcode = _FALSE; 
  _UWORD  i;
  _UWORD  sno, fno;
  _UDWORD size; 

  if(File->GetError() == _FALSE) 
  {
    Free(); 

    SoftkeyText.ReadLanguage(File, get_actual_language());

    if((File->Read(&sno, sizeof(_UWORD)) == _TRUE) && (File->Read(&fno, sizeof(_UWORD)) == _TRUE))
    {
      size = fno * sizeof(_SOFTFLOOR);
      if(File->FileMemory((void**)&Ptr, size) == _TRUE)
      {
        NoFloor    = fno; 
        NoSoftkeys = sno; 
        for(i=0, retcode=_TRUE; i<fno; i++)
        {
          init_SOFTFLOOR(&Ptr[i]); 
          retcode = File->Read_SOFTFLOOR(&Ptr[i], sno);
        }
      }
    }
//    NoSoftkeys = 10;
    if(retcode == _FALSE)
      Free(); 
  }

  return(retcode); 
}

void _Softkey::RefreshSingleSoftkey(_SINGLESOFT *ps, _UWORD no)
{
 #ifdef UC_SOFTKEY
  if(ps->state != ps->oldstate)
  {
		DrawSingleSoftkey(ps, no, _TRUE, 0);
    ps->oldstate = ps->state;
  }
 #endif
}

void _Softkey::DoAllNegEdge(_BOOL redraw)
{
 #ifdef UC_SOFTKEY
  _SINGLESOFT *ps; 
  _UWORD      i; 

  if(ActFloor < NoFloor)
  {
    for(i=0; i<NoSoftkeys; i++)
    {
      ps = &Ptr[ActFloor].floor[i]; 
      if(ps->state == _TRUE)
      {
        ps->state = _FALSE; 
//        Stack->AddStack(InterBoxList.Get(ps->gfunct.neg_fl)); 
        if(redraw == _TRUE)
          RefreshSingleSoftkey(ps, i);
        ps->oldstate = _FALSE; 
      }
    }
  }
 #endif
}

_TextPuffer* _Softkey::GetTextPuffer(void)          
{
  return(&SoftkeyText);
}
_SOFTFLOOR* _Softkey::GetSoftFloor(void)                    
{
  return(Ptr);
}
_UWORD _Softkey::GetNoFloor(void)
{
  return(NoFloor);
}
_UWORD _Softkey::GetActFloor(void)
{
  return(ActFloor);
}
_UWORD _Softkey::GetNoSoftkeys(void)
{
  return(NoSoftkeys);
}


#ifdef HID
_UWORD _Softkey::GetSoftCode(_UWORD no)
{
  _UWORD retcode = _NOKEY;

  switch(no)
  {
    case  0: retcode = _F1;   break;
    case  1: retcode = _F2;   break;
    case  2: retcode = _F3;   break;
    case  3: retcode = _F4;   break;
    case  4: retcode = _F5;   break;
    case  5: retcode = _F6;   break;
    case  6: retcode = _F7;   break;
    case  7: retcode = _F8;   break;
    case  8: retcode = _F9;   break;
    case  9: retcode = _F10;  break;
    case 10: retcode = _F11;  break;
    case 11: retcode = _F12;  break;
  }

  return(retcode);
}

_BOOL _Softkey::DoHID(_EVENT *p)
{
 #ifdef UC_SOFTKEY
  _UWORD i, sc;
  _ROOM  room;
  _KOORD x, y;

  if((ActFloor < NoFloor) && (p->button == _HID_LEFT))
  {
    if(p->ftype == _EVENT_HIDRELEASE)
    {
      x = p->dot.x;
      y = p->dot.y;
      for(i=0; i<NoSoftkeys; i++)
      {
        GetSingleSize(&room, i);
        if((x >= room.xy1.x)&&(x <= room.xy2.x)&&(y >= room.xy1.y)&&(y <= room.xy2.y)) // sa31920
        {
          if(Ptr[ActFloor].floor[i].state == _TRUE)
          {
            sc = GetSoftCode(i);
            if(sc != _NOKEY)
            {
              p->ftype    = _EVENT_KEYRELEASE;
              p->selfmade = 1;
              p->scancode = sc;
              return(_TRUE);
            }
          }
        }
      }
    }
    else if(p->ftype == _EVENT_HIDPRESS)
    {
      x = p->dot.x;
      y = p->dot.y;
      for(i=0; i<NoSoftkeys; i++)
      {
        GetSingleSize(&room, i);
        if((x >= room.xy1.x)&&(x <= room.xy2.x)&&(y >= room.xy1.y)&&(y <= room.xy2.y))
        {
          Beep(BEEP_SOFTKEY);
          found_something_by_hid(p, FOUND_SOFTKEY);

          sc = GetSoftCode(i);
          if(sc != _NOKEY)
          {
            Keyboard.TriggerEventStart(p, Ptr[ActFloor].floor[i].tip);
            
            p->ftype = _EVENT_KEYPRESS;
            p->selfmade = 1;
            p->scancode = sc;
            return(_TRUE);
          }
        }
      }
    }
    else if(p->ftype == _EVENT_TRIGGER)
    {
      x = p->dot.x;
      y = p->dot.y;
      for(i=0; i<NoSoftkeys; i++)
      {
        GetSingleSize(&room, i);
        if((x >= room.xy1.x)&&(x <= room.xy2.x)&&(y >= room.xy1.y)&&(y <= room.xy2.y)) // sa31920
        {
          if(Ptr[ActFloor].floor[i].state == _TRUE)
          {
            sc = GetSoftCode(i);
            if(sc != _NOKEY)
            {
              p->ftype    = _EVENT_KEYPRESS;
              p->selfmade = 2;
              p->scancode = sc;
              return(_TRUE);
            }
          }
        }
      }
    }
  }
 #endif

  return(_FALSE);
}

#endif


#ifdef XUSER_KM

// nachstehende funktionalität wurde "so wie sie ist" vom kunden beschrieben.
// sigmatek weist kunden auf verminderte funktionalität hin --> kunde akzeptiert.
// * es wird nur mehr rücksicht auf funktion "bildumschalten" + "if then else" und "screenschama" genommen. andere funktionen werden nicht beachtet oder ausgeführt
// * hochlaufzeit wird verlängert (momentan ~3 sekunden)
// * bildaufschlagen dauert, wegen _Softkey::AssignedImage suchen länger
// * ausgabe der softkeys dauert länger
// * voraussetzung dass bei jedem bild ein bild-image definiert ist
// * funktionalität ist weiters ausschliesslich für softkeys gedacht (nicht für buttons)
// anmerkung: die darstellung eines softkeys wird jetzt sehr stark von der software beeinflusst.
// dies könnte unter umständen vom anwender als fehler interpretiert werden.

void _Softkey::PushScreenNo(_UWORD picno)
{
  PicNoMemo = picno;
}

_UWORD _Softkey::PopScreenNo(void)
{
  return(PicNoMemo);
}

_SINGLESOFT* _Softkey::Get_SINGLESOFT(_UWORD floorno, _UWORD softno)
{
  _SINGLESOFT *retcode = NULL;

  if((floorno < NoFloor)&&(softno < NoSoftkeys))
    retcode = &Ptr[floorno].floor[softno];
  return(retcode);
}

void _Softkey::SetAssignedImage(_UWORD ino)
{
  AssignedImage = ino;
}

_UWORD _Softkey::GetAssignedImage(void)
{
  return(AssignedImage);
}

void _AllSoftkeys::Init(void)
{
  Ptr = NULL;
  No = 0;
}

void _AllSoftkeys::Free(void)
{
  _UWORD i;

  for(i=0; i<No; i++)
    Ptr[i].Free();
  Memory2((void**)&Ptr, 0);
  Init();
}

_Softkey* _AllSoftkeys::GetSoftkey(_UWORD picno)
{
  _Softkey   *retcode = NULL;
  _PRJ_ENTRY *prje;

  prje = Project.GetEntryByNo(picno, _PE_PICTURE);
  if(prje != NULL)
  {
    if(prje->softinfo < No)
      retcode = &Ptr[prje->softinfo];
  }

  return(retcode);
}

void _AllSoftkeys::AssignSoftInfo(void)
{
  _PRJ_ENTRY *prje;
  _UDWORD    i;
  _UWORD     picno;

  _UDWORD time = get_abstime(); 
   
  for(i=0; i<No; i++)
  {
    picno = Ptr[i].PopScreenNo();

    prje = Project.GetEntryByNo(picno, _PE_PICTURE);
    if(prje != NULL)
      prje->softinfo = i;
  }

//  Debug(TimeDelay, 1);
  TimeDelay += timediff(time);
//  Debug(TimeDelay, 2);
}


void _AllSoftkeys::Scan(void)
{
  _PRJ_ENTRY *ptr;
  _UDWORD    anz, i, no;
  _Picture   Picture;
  _ASCII     dpne[_MAX_PATHLENGTH];
  _UDWORD    size;
  _Softkey   *ps;
  _Project   TmpPrj;
//  _UWORD     bs;

  _UDWORD time = get_abstime();

  PushMemColor(MEMORY_PRG);

  TmpPrj.Init();
  if(TmpPrj.JustLoad(Batch.GetProjectDPNE()) == _TRUE)
  {
    no = TmpPrj.GetNo();

    for(i=0,anz=0; i<no; i++)
    {
      ptr = TmpPrj.GetEntry(i);
      if(ptr != NULL)
        if(ptr->ftype == _PE_PICTURE)
          anz ++;
    }

    Picture.Init();
//    bs = ScanBar.Add("Scan Pictures:", anz);

    size = anz * sizeof(_Softkey);
    if(Memory2((void**)&Ptr, size) != 0)
    {
//      for(i=0,anz=0; i<no; i++)
      for(i=0; i<no; i++)
      {
        ptr = TmpPrj.GetEntry(i);
        if(ptr != NULL)
        {
          if(ptr->ftype == _PE_PICTURE)
          {
//            ScanBar.Run(bs, anz++);
            StrCpy(dpne, sizeof(_ASCII), TmpPrj.GetPrjPath(), sizeof(_ASCII));
            StrCat(dpne, sizeof(_ASCII), ptr->name, sizeof(_ASCII));
//o            size = (No + 1) * sizeof(_Softkey);
//o            if(Memory2((void**)&Ptr, size) != 0)
            {
              ps = &Ptr[No];
              ps->Init();

              if(Picture.Load(dpne, ps, _FALSE, _TRUE, 0) == _TRUE)
              {
                // ptr->softinfo = No;
                ps->PushScreenNo(ptr->no);
                No += 1;
              }
            }

            Picture.Free();
          }
        }
      }
    }

//    ScanBar.End(bs);
  }

  TmpPrj.Free();
  PopMemColor();

  TimeDelay = timediff(time);
}

#ifndef COLOR_SCREENSOFTKEY
 #define COLOR_SCREENSOFTKEY   0x7FF
#endif
_BOOL _AllSoftkeys::IsScreenSoftkey(_SINGLESOFT *ps)
{
  _BOOL retcode = _FALSE;

  if(ps != NULL)
  {
    if(GetPenColor_Makro(ps->color) == COLOR_SCREENSOFTKEY) // km-farbe türkies bei softkey
      retcode = _TRUE;
  }

  return(retcode);
}

_SINGLESOFT* _AllSoftkeys::Get_SINGLESOFT(_PRJ_ENTRY *prje, _UWORD softno, _UWORD floorno)
{
  _SINGLESOFT *retcode = NULL;

  if(prje->softinfo < No)
    retcode = Ptr[prje->softinfo].Get_SINGLESOFT(floorno, softno);

  return(retcode);
}

_UWORD _AllSoftkeys::SeekNextPictureSub(_UWORD screenno, _UWORD softno, _UWORD floorno, _BOOL system)
{
  _UWORD      retcode = 0xFFFF;
  _PRJ_ENTRY  *prje;
  _SINGLESOFT *pss;
  _UDWORD     i, max;

  prje = Project.GetEntryByNo(screenno, _PE_PICTURE);
  if(prje != NULL)
  {
    if((prje->usedintern & 0x02) == 0) // auf rekursion prüfen
    {
      prje->usedintern |= 0x02; // wegen rekursion setzen

      if(Project.Access_PRJ_ENTRY(prje, system) == _TRUE)
      {
        // zugriff auf bild ist erlaubt
        retcode = screenno;
      }
      else
      {
        // bildzugehörigen softkey suchen und softno neu zuweisen
        if(prje->softinfo < No)
        {
          max = Ptr[prje->softinfo].GetNoSoftkeys();
          for(i=0; i<max; i++)
          {
            if(IsScreenSoftkey(Get_SINGLESOFT(prje, i, floorno)) == _TRUE)
            {
              softno = i;
              break;
            }
          }
        }

        if(prje->nextno != 0xFFFF)
        {
          // falls nexscreenno angegeben ist dann soll diese auch verwendet werden
          retcode = SeekNextPictureSub(prje->nextno, softno, 0, system);
        }
        else if(softno < NO_SOFT_FLOOR) // bei hotkey ist softno auf 0xFFFF gesetzt
        {
          // keine nextno angegeben, also wird softkey gescannt
          pss = Get_SINGLESOFT(prje, softno, floorno);
          retcode = SeekNextPicture(pss, softno, &floorno, system, NULL);
        }
      }
    }
  }

  return(retcode);
}

_UWORD _AllSoftkeys::SeekNextPicture(_SINGLESOFT *ps, _UWORD softno, _UWORD *floorno, _BOOL system, _BOOL *original_image_valid)
{
  _BOOL  state;
  _UWORD retcode = 0xFFFF;
  _UWORD screenno;
  _Stack TmpStack;

  if(ps != NULL)
  {
    // überprüfen ob softkey überhaupt gültig
    if((ps->valid)&&(chk_ACCESS(ps->access, system) == _TRUE)&&(chk_CHKBIT(&ps->chkbit) == _TRUE))
    {
      // weiterschaltbedingung auf bild suchen
      TmpStack.Init();
      state = TmpStack.GetScreenNo(InterBoxList.Get(ps->gfunct.pos_fl.no), &screenno, floorno);
      TmpStack.Free();

      if(state == _TRUE)
      {
        if(original_image_valid != NULL)
          *original_image_valid = _FALSE;
        retcode = SeekNextPictureSub(screenno, softno, *floorno, system);
      }
    }
  }

  return(retcode);
}

_UWORD _AllSoftkeys::GetPictureNo(_SINGLESOFT *ps, _UWORD softno, _UWORD *floorno, _BOOL system, _BOOL *original_image_valid)
{
  if(original_image_valid != NULL)
    *original_image_valid = _TRUE;

  Project.InitUsedIntern(0xFFFD);
  return(SeekNextPicture(ps, softno, floorno, system, original_image_valid));
}

_UWORD _AllSoftkeys::GetImageNo(_SINGLESOFT *ps, _UWORD softno, _BOOL *original_image_valid, _UWORD assigned_image)
{
  _UWORD     retcode = 0xFFFF;
  _PRJ_ENTRY *prje;
  _UWORD     floorno;
  _UWORD     picno;
  _Stack     TmpStack;
  _UWORD     sno, fno;
  _BOOL      oiv = _TRUE;
  _BOOL      state;

  if(IsScreenSoftkey(ps) == _TRUE) // überprüfen der zum bild gehörende softkey
  {
    // beim bildzugehörenden softkey wird angegebenes image ausgegeben
    retcode = assigned_image;
    oiv     = _FALSE;
  }
  else
  {
    // überprüfen ob softkey eine bildweiterschaltbedingung enthält, wenn ja
    // wird softkeyimage mit image von weitergeschaltetem bild verglichen. sind
    // diese unterschiedlich, so wird das im softkey angegebene image ausgegeben.
    // ansonsten mit algorithmus neues image suchen
    TmpStack.Init();
    state = TmpStack.GetScreenNo(InterBoxList.Get(ps->gfunct.pos_fl.no), &sno, &fno);
    TmpStack.Free();
    if(state == _TRUE)
    {
      prje = Project.GetEntryByNo(sno, _PE_PICTURE);
      if(prje != NULL)
      {
        state = _FALSE;
        if((prje->imageno != ps->buttonimages.image_describe)&&(ps->buttonimages.image_describe < ImageList.GetNo()))
        {
          // softkeyimage ist gültig und ungleich bildimage
          if(GetPictureNo(ps, softno, &floorno, _TRUE, &oiv) != 0xFFFF)
          {
            // dahinterliegendes bild ist anwählbar
            retcode = ps->buttonimages.image_describe;
          }
          state = _TRUE;
        }
      }
    }

    if(state == _FALSE)
    {
      // falls noch kein image ermittelt wurde, wird dies mittels algorithmus berechnet
      if(retcode > ImageList.GetNo())
      {
        picno = GetPictureNo(ps, softno, &floorno, _TRUE, &oiv);
        if(picno != 0xFFFF)
        {
          prje = Project.GetEntryByNo(picno, _PE_PICTURE);
          if(prje != NULL)
            retcode = prje->imageno;
        }
      }
    }
  }
  if(original_image_valid != NULL)
    *original_image_valid = oiv;

  return(retcode);
}

void _AllSoftkeys::DoKey(_SINGLESOFT *ps, _UWORD no, _Window *PWin, _BOOL hotkey, _BOOL lrm)
{
  _UWORD      picno, floorno;
  _STACK_LINE stackline;
  _BOOL       do_function = _TRUE;

  picno = AllSoftkeys.GetPictureNo(ps, no, &floorno, _FALSE, &do_function);
  if(picno != 0xFFFF)
  {
    init_STACK_LINE_Makro(&stackline);
    stackline.cmd = CMD_NEWPICTURE;
    stackline.op1.no = 1;
    stackline.op1.info[0].state = CONST_VAL;
    stackline.op1.info[0].value = picno;
    stackline.op2.no = 1;
    stackline.op2.info[0].state = CONST_VAL;
    stackline.op2.info[0].value = floorno;
    PWin->AddPart(&stackline, 1, lrm);
    free_STACK_LINE(&stackline);
  }
  else if((do_function == _TRUE)||(hotkey == _TRUE))
  {
    do_LSEFUNCT_lrm(&ps->gfunct.pos_fl, PWin, lrm);
  }
}

#ifndef Hotkey01
 #define Hotkey01  24064
#endif
#ifndef Hotkey02
 #define Hotkey02  24320
#endif
#ifndef Hotkey03
 #define Hotkey03  24576
#endif
#ifndef Hotkey04
 #define Hotkey04  24832
#endif
#ifndef Hotkey05
 #define Hotkey05  25344
#endif
#ifndef Hotkey06
 #define Hotkey06  26624
#endif
#ifndef Hotkey07
 #define Hotkey07  26880
#endif
#ifndef Hotkey08
 #define Hotkey08  27136
#endif
#ifndef Hotkey09
 #define Hotkey09  27392
#endif
#ifndef Hotkey10
 #define Hotkey10  27904
#endif
#ifndef Hotkey11
 #define Hotkey11  28160
#endif
#ifndef Hotkey12
 #define Hotkey12  28416
#endif
#ifndef Hotkey13
 #define Hotkey13  28672
#endif
#ifndef Hotkey14
 #define Hotkey14  28928
#endif
#ifndef Hotkey15
 #define Hotkey15  33792
#endif

_BOOL _AllSoftkeys::IsHotkey(_UWORD scancode)
{
  if((scancode == Hotkey01)||(scancode == Hotkey02)||(scancode == Hotkey03)||
     (scancode == Hotkey04)||(scancode == Hotkey05)||(scancode == Hotkey06)||
     (scancode == Hotkey07)||(scancode == Hotkey08)||(scancode == Hotkey09)||
     (scancode == Hotkey10)||(scancode == Hotkey11)||(scancode == Hotkey12)||
     (scancode == Hotkey13)||(scancode == Hotkey14)||(scancode == Hotkey15)
    #ifdef Hotkey16
     ||(scancode == Hotkey16)
    #endif
    #ifdef Hotkey17
     ||(scancode == Hotkey17)
    #endif
    #ifdef Hotkey18
     ||(scancode == Hotkey18)
    #endif
    #ifdef Hotkey19
     ||(scancode == Hotkey19)
    #endif
    #ifdef Hotkey20
     ||(scancode == Hotkey20)
    #endif
    )
  {
    return(_TRUE);
  }

  return(_FALSE);
}

_SINGLESOFT *_AllSoftkeys::Get_SINGLESOFT(_IMA ima, _UWORD no, _UWORD floorno, _UWORD softno)
{
  _ASCII       dpne[_MAX_PATHLENGTH];
  _PRJ_ENTRY  *pre    = NULL;
  _SINGLESOFT *retcode = NULL;
  _UWORD       anz, tmpno = no;
  _SINGLESOFT *pso;

  if((ima == IMA_PICTURE)&&(Ptr != NULL))
  {
    if(Project.GetPictureAccess(dpne, &tmpno, _TRUE, &pre) == _TRUE)
    {
      if(tmpno == no)
      {
        anz = Ptr[0].GetNoSoftkeys();
        if(softno < anz)
        {
          retcode = Get_SINGLESOFT(pre, softno, floorno);
        }
        else
        {
          while(anz)
          {
            anz -= 1;
            pso = Get_SINGLESOFT(pre, anz, floorno);
            if(IsScreenSoftkey(pso) == _TRUE)
            {
              anz     = 0;
              retcode = pso;
            }
          }
        }
      }
    }
  }

  return(retcode);
}

_SINGLESOFT *Get_SINGLESOFT(_IMA ima, _UWORD no, _UWORD floorno, _UWORD softno)
{
  return(AllSoftkeys.Get_SINGLESOFT(ima, no, floorno, softno));
}

#endif




