#include "T_IPC.H"
#include "T_NUC.H"

#include "COMINTFC.H"

//#define MENUE_CURSOR_ALT
//#define CURSOR_COLOR_MENU   MakeColor(LIGHTRED, WHITE)
#ifdef UC_MENU_NEWSCROLL
  #define     MENU_SCROLLDECEL    3
  #define     MENU_INITMOVEDELAY  100
  #define     MENU_MAXMOVEDELAY   150
  #define     MENU_TIMESCROLL     40
  void        popup_level_draw(_LEVEL*, _BOOL, _BMP*, _FRAMETYPE);
  void        popup_level_run(_LEVEL*, _DWORD, _POSITION, _BMP*, _FRAMETYPE, _COLOR prColor);
  _ROOM       GetRoomWithoutButtons(_ROOM, _BOOL);
  extern void GetSlimSliderRoomWidth(_ROOM*, _ROOM*, _BOOL);
#endif

void  draw_window(_MENU*, _CHAR*, _BOOL);
void  calculate_window(_MENUCALCUL*, _MENUINFO*, _POSITION*, _MENUPOINTER*);

_BOOL _Window::CallInputIsSavedOnSign(_VARLABEL *value, _BOOL lrm)
{
  _BOOL       retcode = _FALSE;
  _RESULT     result;
  _VARIABLE   variable;
  _MY_PICTURE *pmp;

  if(Sign == _TRUE)
  {
    if(P_Picture != NULL)
    {
      if(P_Picture->IsEditorOpen() == _TRUE)
      {
        init_RESULT_Makro(&result);
        init_VARIABLE_Makro(&variable);
        set_lrm(&result, lrm);
        if(VarList.GetSystemData(&result, &variable, value, _TRUE, _TRUE) == _TRUE)
        {
          pmp = P_Picture->GetMyPicture();
          input_is_saved(&pmp->Editor.input, &result);
          retcode = _TRUE;
        }
        free_VARIABLE(&variable);
//        free_RESULT(&result);
      }
    }
  }
  else if(P_Background != NULL)
  {
    P_Background->CallInputIsSavedOnSign(value, lrm);
  }

  return(retcode);
}

#ifdef PATRICK
_BOOL _Window::CheckForClose(void)
{
  if(P_Picture != NULL)
    return(P_Picture->CheckForClose());

  if(P_Background != NULL)
    return(P_Background->CheckForClose());

  return(_FALSE);
}
#endif

void popup_mae_run(_Window *Pw, _PopUp *Pp, _EVENT *pe)
{
  _MENU       *pm;
  _LINEINFO   *pli;
  _VARLABEL   *pv;
  _STACK_LINE sl;
  _BOOL       state, doret, saval;

 #ifdef PATRICK
  pv = Pp->GetInput();
  if(chk_VARLABEL(pv) == _TRUE)
  {
    if(Pw->CheckForClose() == _TRUE) // überprüfen ob eingabeberechtigung noch besteht, ansonsten menue schliessen
    {
      init_STACK_LINE_Makro(&sl);
      sl.cmd = CMD_RETURN_TO_SIGN;
      sl.op1.no = 1;
      sl.op1.info[0].state = CONST_VAL;
      sl.op1.info[0].value = _ESC;
      Pw->AddLine(&sl, _FALSE);                                      // exit
      free_STACK_LINE(&sl);
    }
  }
 #endif

  if(pe->ftype == _EVENT_KEYPRESS)
  {
    if((pe->scancode == _ENTER)||(pe->scancode == _RIGHT))
    {
      pm  = Pp->GetMenu();
      pli = MenuList.Get_LINEINFO(pm->pointer.Ptr_LSEMENU, pm->position.pos);

      if(pli != NULL)
      {
        if(state_LINEINFO(pli) == STATE_ACTIVE)
        {
          if(pe->scancode == _RIGHT)
          {
            if(lookup_LSEFUNCT(&pli->gfunct.pos_fl, CMD_NEWPOPUP) == _TRUE)
            {
              Pw->Add_LSEFUNCT(&pli->gfunct.pos_fl, pe->lrm);
            }
          }
          else if(pe->scancode == _ENTER)
          {
            // standard funktionalität ausführen
            saval = _TRUE; // save editor
            doret = _TRUE; // direction on enter

            if(InterBoxList.Chk_LSEFUNCT(&pli->gfunct.pos_fl) == _TRUE)
            {
              // user hat eigene funktion definiert
              saval = lookup_LSEFUNCT(&pli->gfunct.pos_fl, CMD_SAVEEDITOR);
              doret = lookup_LSEFUNCT(&pli->gfunct.pos_fl, CMD_RETURN);
              Pw->Add_LSEFUNCT(&pli->gfunct.pos_fl, pe->lrm);    // andere funktionalität in queue
            }

            state = _FALSE;
            if(saval == _TRUE)
            {
              pv = Pp->GetInput();
              
//             #ifdef UC_JAPANINPUT 
//              if(MenuList.IsDictionaryMenue(pm->pointer.Ptr_LSEMENU) == _TRUE)
//              {
//                if(Pw != NULL)
//                {
//                  _Window *pb = Pw->GetP_Background();
//                  if(pb != NULL)
//                  {
//                    _WHOAMI wai = pb->GetWhoAmI();
//                    if(wai.ima == IMA_TOUCHEDITOR)
//                    {
//                      _UDWORD stscheme_result;
//                      _CHAR  *txt = MenuList.GetLineText(&stscheme_result, pm->pointer.Ptr_LSEMENU, pm->position.pos);
//                      if(txt != NULL)
//                      {
//                        pb->InjectEditorText(txt); // ausgewähltes wort in Editor übernehmen
//                      }
//                    }
//                  }
//                }
//              }
//              else
//             #endif
              if(chk_VARLABEL(pv) == _TRUE)
              {
                
                _DOIT doit = _IDLE;
                if(Pp->GetP_MyIo() != NULL)
                {
                  _RESULT   m_result;
                  _VARIABLE m_var;
                  if(VarList.GetSystemData(&m_result, &m_var, &pli->value, _TRUE, _TRUE) == _TRUE)
                  {
                    doit = CallMethod_MyIoSetValue(&m_result, pv, Pp->GetP_MyIo());
                  }
                }
                
                if(doit == _IDLE)
                {
                  init_STACK_LINE_Makro(&sl);
                  sl.cmd = CMD_SET;
                  sl.op1 = *pv;
                  sl.op2 = pli->value;
                  state = do_CMD_SET(&sl, pe->lrm);   // save
                  if(state == _TRUE)
                    doit = _IDIDIT;
                  free_STACK_LINE(&sl);
                }
                
                if(doit == _IDIDIT)
                  Pw->CallInputIsSavedOnSign(&pli->value, pe->lrm); // call interface input is saved
              }
            }

            if(doret == _TRUE)
            {
              init_STACK_LINE_Makro(&sl);
              sl.cmd = CMD_RETURN_TO_SIGN;
              sl.op1.no = 1;
              sl.op1.info[0].state = CONST_VAL;
              sl.op1.info[0].value = (state == _TRUE)? _ENTER : _ESC;   // call direction on enter
              // sl.op2 = pli->value;
              Pw->AddLine(&sl, pe->lrm);                                      // exit
              free_STACK_LINE(&sl);
            }
          }
        }
      }
    }
    else if(pe->scancode == _LEFT)
    {
      pe->scancode = _ESC;
    }
  }
}

void popup_mae_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ROOM   room;
  _CHAR   *txt;
  _MENU   *pm;
  _COLOR  c;
  _UDWORD stscheme_result;

  room = *r;
  room.xy1.x += 2;

  pm = Pp->GetMenu();

  if(pm->info.frametype >= _THEMEFRAME)
    room.xy1.x += 4;

  if(background == _TRUE)
  {
    if(pm->info.frametype < _THEMEFRAME)
    {
      Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));
    }
    else
    {
      c = GetBackColor_Makro(col);
      c = MakeColor_Makro(c, c);
      DrawButton(&pm->calcul.out_room, pm->info.attrib, pm->info.frametype, c, c); // T_COPY|T_SOLID
    }
  }

  txt = MenuList.GetLineText(&stscheme_result, pm->pointer.Ptr_LSEMENU, no);
  if(txt != NULL)
  {
    _KOORD x, y;
    _ATTRIB att = pm->info.attrib;
    //_ATTRIB att = T_COPY | T_SOLID | T_PROP | T_CENTERBOUND | T_DOWNBOUND; // test
    switch(att & T_HORIZBOUND)
    {
      case T_CENTERBOUND:
        x = (room.xy1.x + room.xy2.x) / 2;
        break;
      case T_RIGHTBOUND:
        x = room.xy2.x;
        break;
      default:
        x = room.xy1.x;
        break;
    }
    
    switch(att & T_VERTBOUND)
    {
      case T_MIDBOUND:
        y = (room.xy1.y + room.xy2.y) / 2;
        break;
      case T_DOWNBOUND:
        y = room.xy2.y;
        break;
      default:
        y = room.xy1.y;
        break;
    }
        
    if(stscheme_result == STATE_INACTIVE)    
        col = pm->info.inactive_text_color;
        
    col = GetPenColor_Makro(col);
    if(col != INVISIBLE)
      OutMultiTextIconXY(x, y, txt, att, MakeColor_Makro(INVISIBLE, col), sizeof(_CHAR), pm->info.font);
  }

 #ifdef UC_ROTATE
  RefreshRectangle((background == _TRUE)? r:&room);
 #endif
  
}

void _PopUp::Free(void)
{
  Cursor.Free();
  free_REGION(&Region);
  free_MENU(&Menu);
  free_VARLABEL(&Input);
  Init();
}

void _PopUp::Init(void)
{
  P_Window = NULL;
  Cursor.Init();
  init_REGION(&Region);
  init_MENU(&Menu);
  init_VARLABEL_Makro(&Input);
  init_VARLABEL_Makro(&ImOverloaded);
  pUser = NULL;
  P_MyIo = NULL;
}

void _PopUp::Calculate(void)
{
  calculate_window(&Menu.calcul, &Menu.info, &Menu.position, &Menu.pointer);
}

void _PopUp::GetLinePosition(_UWORD y, _ROOM *pr)
{
  *pr        = Menu.calcul.scroll_room;
  pr->xy1.y += y * Menu.info.line_height;
  pr->xy2.y  = pr->xy1.y + Menu.info.line_height - 1;
}

void _PopUp::Beam(_UWORD y, _UWORD no, _BOOL st, _BOOL do_refresh)
{
  _COLOR col, cb, cf;

  if(st == _TRUE)
  {
   #ifdef MENUE_CURSOR_ALT
    col = Menu.info.bar_color;
   #else
    cb  = SetUp.GetEditColorBack(_EDITACTIVE, GetBackColor_Makro(Menu.info.text_color));
    cf  = SetUp.GetEditColorTxt(_EDITACTIVE, GetPenColor_Makro(Menu.info.text_color));
    col = MakeColor_Makro(cb, cf);
   #endif
   
   #ifdef CURSOR_COLOR_MENU
    col = CURSOR_COLOR_MENU;
   #endif
  }
  else
  {
   #ifdef UC_MENU_TWOCOLOR
    if (no & 1)
    {
      cb  = GetBackColor(Menu.info.text_color);
    }
    else
    {
      cb  = GetPenColor(Menu.info.text_color);
    }
    cf  = GetPenColor(Menu.info.bar_color);
    col = MakeColor_Makro(cb, cf);
   #else
    col = Menu.info.text_color;
   #endif
  }

  Cursor.End();
  OutLine(y, no, col, st, _TRUE, do_refresh);
  set_MENCURSOR(&Menu.mencursor);
}

void _PopUp::OutPage(_UWORD actpos, _BOOL do_refresh)
{
  _UWORD  i, j;
  _REGION actreg, ioreg=NULL, newreg=NULL;
  _COLOR  color, c;
 #ifdef UC_MENU_TWOCOLOR
  _COLOR  cb, cf;
 #endif
 
  Cursor.End();

//  actreg = ActivateAndRegion(&ioreg, &newreg, &Menu.calcul.scroll_room, T_SOLID|T_COPY);
  actreg = ActivateAndRegion(&ioreg, &newreg, &Menu.calcul.scroll_room);
  color  = GetBackColor_Makro(Menu.info.text_color);
  if(Menu.info.frametype < _THEMEFRAME)
  {
    Bar(Menu.calcul.scroll_room.xy1.x, Menu.calcul.scroll_room.xy1.y, Menu.calcul.scroll_room.xy2.x, Menu.calcul.scroll_room.xy2.y, Menu.info.attrib, MakeColor_Makro(color, color));
  }
  else
  {
    c = MakeColor_Makro(color, color);
    DrawButton(&Menu.calcul.out_room, Menu.info.attrib, Menu.info.frametype, c, c);
  }

//  DrawBackground(_FALSE);
  DeactivateAndRegion(actreg, &ioreg, &newreg);

  if(Menu.position.no > 0)
  {
    j = actpos - Menu.position.begin;
    if(j < Menu.position.height)
      Beam(j, actpos, _TRUE, _FALSE);
  }
  else
  {
    actpos = 0xFFFF;
  }

  j = Menu.position.begin;
  for(i=0; i<Menu.position.height; i++, j++)
  {
    if(j != actpos)
    {
     #ifdef UC_MENU_TWOCOLOR
      if (j & 1)
      {
        cb  = GetBackColor(Menu.info.text_color);
      }
      else
      {
        cb  = GetPenColor(Menu.info.text_color);
      }
      cf = GetPenColor(Menu.info.bar_color);
      color = MakeColor_Makro(cb, cf);
      OutLine(i, j, color, _FALSE, _TRUE, _FALSE);
     #else     
      OutLine(i, j, Menu.info.text_color, _FALSE, _FALSE, _FALSE);
     #endif
    }
  }

 #ifdef UC_ROTATE
  if(do_refresh == _TRUE)
    RefreshRectangle(&Menu.calcul.scroll_room);
 #endif
}

void _PopUp::DrawBackground(_BOOL shadowstate)
{
  _CHAR *title = NULL;

//  if(Menu.info.Title.GetNo() > 0)
//    title = Menu.info.Title.GetText(0);

  draw_window(&Menu, title, (shadowstate == _TRUE)? P_Window->GetShadow():_FALSE);
}

void _PopUp::Draw(void)
{
  DrawBackground(_TRUE);
  OutPage(Menu.position.pos, _FALSE);
 #ifdef UC_ROTATE
  RefreshRectangle(&Menu.calcul.out_room);
 #endif


}

void _PopUp::DoPositions(_EVENT *pe)
{
  _UWORD     oldpos   = Menu.position.oldpos;
  _UWORD     oldbegin = Menu.position.oldbegin;
  _POSRESULT st;

  if(Menu.position.no > 0)
  {
   #ifdef UC_MENU_NEWSCROLL
    st = calculate_PopUpPosition(&Menu.position, pe, Menu.position.height);
   #else
    st = calculate_position(&Menu.position, NULL, pe, NULL);
   #endif
    if(st != P_NOP)
    {
     #ifdef UC_HICOLOR
      switch(st)
      {
        case P_SCROLLUP   :
        case P_SCROLLDOWN :
        case P_REDRAW     : OutPage(Menu.position.pos, _TRUE);
                            break;
        default           : Beam(oldpos-oldbegin, oldpos, _FALSE, _TRUE); // off
                            Beam(Menu.position.pos-Menu.position.begin, Menu.position.pos, _TRUE, _TRUE); // on
                            break;
      }
     #else
      switch(st)
      {
        case P_SCROLLUP   : Beam(oldpos-oldbegin, oldpos, _FALSE, _TRUE); // off
                            ScrollUp();
                            Beam(Menu.position.pos-Menu.position.begin, Menu.position.pos, _TRUE, _TRUE); // on
                            break;
        case P_SCROLLDOWN : Beam(oldpos-oldbegin, oldpos, _FALSE, _TRUE); // off
                            ScrollDown();
                            Beam(Menu.position.pos-Menu.position.begin, Menu.position.pos, _TRUE, _TRUE); // on
                            break;
        case P_REDRAW     : OutPage(Menu.position.pos, _TRUE);
                            break;
        default           : Beam(oldpos-oldbegin, oldpos, _FALSE, _TRUE); // off
                            Beam(Menu.position.pos-Menu.position.begin, Menu.position.pos, _TRUE, _TRUE); // on
                            break;
      }
     #endif
    }
  }


}

void _PopUp::ScrollUp(void)
{
  _ROOM  tmp, room;
  _KOORD h;
  _COLOR color;

  GetLinePosition(0, &tmp);
  h           = (tmp.xy2.y - tmp.xy1.y) + 1;
  room        = Menu.calcul.scroll_room;
  room.xy1.y += h;

  color = MakeColor_Makro(GetBackColor_Makro(Menu.info.text_color), GetBackColor_Makro(Menu.info.text_color));
 #ifdef UC_MOUSE
  _UWORD hide = IfNecHideMouseCursor(&Menu.calcul.scroll_room);
 #endif
  Scroll(room.xy1.x, room.xy1.y, room.xy2.x+1, room.xy2.y+1, 0, -h ,color);
 #ifdef UC_MOUSE
  IfNecShowMouseCursor(hide);
 #endif
}

void _PopUp::ScrollDown(void)
{
  _ROOM  tmp, room;
  _KOORD h;
  _COLOR color;

  GetLinePosition(0, &tmp);
  h           = (tmp.xy2.y - tmp.xy1.y);
  room        = Menu.calcul.scroll_room;
  room.xy2.y -= h;
  color = MakeColor_Makro(GetBackColor_Makro(Menu.info.text_color), GetBackColor_Makro(Menu.info.text_color));
 #ifdef UC_MOUSE
  _UWORD hide = IfNecHideMouseCursor(&Menu.calcul.scroll_room);
 #endif
  Scroll(room.xy1.x, room.xy1.y, room.xy2.x+1, room.xy2.y, 0, h+1 ,color);
 #ifdef UC_MOUSE
  IfNecShowMouseCursor(hide);
 #endif
}

_BOOL _PopUp::Open(_UWORD no, _Window* pw, _KOORD x, _KOORD y, _VARLABEL *pv, _VARLABEL *ovl, void *pmyio)
{
  _BOOL retcode = _FALSE;

  if(Load(no, pw, pv, ovl, pmyio) == _TRUE)
  {
    Menu.info.xy.x = x;
	  Menu.info.xy.y = y;

	  Calculate();

	  if(pv != NULL)
    {
		  Input = *pv;
    }

	  retcode = _TRUE;
  }

  return(retcode);
}

_BOOL _PopUp::Load(_UWORD no, _Window* pw, _VARLABEL *vl, _VARLABEL *ovl, void *pmyio)
{
  _BOOL     retcode = _FALSE;
  _RESULT   result;
  _VARIABLE variable;
  _LSEMENU  *plsemenu;

  Init();
  P_Window = pw;
  P_MyIo   = pmyio;

  if(popup_call(&Menu, &no) == _FALSE)           // user interface open popup
  {
    plsemenu = MenuList.Get_LSEMENU(no);
    if(plsemenu != NULL)
    {
      if(Memory2((void**)&Menu.pointer.Ptr_LSEMENU, sizeof(_LSEMENU)) != 0)
      {
        init_LSEMENU(Menu.pointer.Ptr_LSEMENU);
        
        if(copy_MENUINFO(&Menu.info, &plsemenu->info) == _TRUE)
        {
          if(copy_LSEMENU(Menu.pointer.Ptr_LSEMENU, plsemenu) == _TRUE)
          {
            Menu.pointer.PtrLine  = popup_mae_line; // --> to linedraw function
            Menu.pointer.PtrRun   = popup_mae_run;       // --> to run function

            if(ovl != NULL)
            {
              if(valid_VARLABEL(ovl) == _TRUE)
              {
                overload_LSEMENU(Menu.pointer.Ptr_LSEMENU, ovl);
                ImOverloaded = *ovl;
              }
            }

            Menu.position.no = MenuList.CalculateLines(Menu.pointer.Ptr_LSEMENU);

            if(vl != NULL)
            {
              if(VarList.GetSystemData(&result, &variable, vl, _TRUE, _TRUE) == _TRUE)
                Menu.position.pos = MenuList.AktLineNo(result.value, Menu.pointer.Ptr_LSEMENU);
            }
            else if((vl == NULL) && (chk_VARLABEL(&Menu.info.server)))
            {
              if(VarList.GetSystemData(&result, &variable, &Menu.info.server, _TRUE, _TRUE) == _TRUE)
              {
                Menu.position.pos = MenuList.AktLineNo(result.value, Menu.pointer.Ptr_LSEMENU);
                copy_VARLABEL(&Input, &Menu.info.server);
              }
            }

            retcode = _TRUE;
          }
        }
      }
    }
   #ifdef OK_DIAGNOSTIC
    else
    {
      retcode = Diagnostic(&Menu, vl, no);
    }
   #endif
  }
  else
  {
    retcode = _TRUE;
  }

  if(retcode == _TRUE)
  {
    Calculate();
  }
  else
  {
    Free();
  }

  return(retcode);
}

void _PopUp::PopUpAddRegion(void)
{
  _ROOM room;

 #ifdef UC_THEME
  if(Menu.info.frametype >= _THEMEFRAME)
    clip_THEME(&Menu.calcul.out_room, Menu.info.frametype);
  else
 #endif
    AddRegion(&Menu.calcul.out_room);

  if(P_Window->GetShadow() == _TRUE)
  {
    room = Menu.calcul.out_room;
    GetShadowRoom(&room);
   #ifdef UC_THEME
    if(Menu.info.frametype >= _THEMEFRAME)
      clip_THEME(&room, Menu.info.frametype);
    else
   #endif
    AddRegion(&room);
  }
}

void _PopUp::StartIntern(_BOOL firsttime)
{
  if((Menu.pointer.PtrStart != NULL) && (P_Window != NULL))
	 Menu.pointer.PtrStart(P_Window, this, firsttime);
   
   #ifdef UC_MENU_NEWSCROLL
     uScrollState = 1;
     bIsFirstStart = _TRUE;
     dActMoveDelay = 0;
     bPressOK = _TRUE;
     prDot.x = -1;
     prDot.y = -1;
    
     if(Menu.position.pos < Menu.position.begin)
       udOldBegin = Menu.position.begin = Menu.position.oldbegin = Menu.position.pos;

     if(Menu.position.pos >= (Menu.position.begin + Menu.position.height))
       udOldBegin = Menu.position.begin = Menu.position.oldbegin = (Menu.position.pos - (Menu.position.height /2));   
   #endif
}

void _PopUp::ReadyIntern(_BOOL firsttime)
{
//  if(P_Window != NULL)
//    P_Window->Ready(firsttime); 

  if((Menu.pointer.PtrReady != NULL) && (P_Window != NULL))
    Menu.pointer.PtrReady(P_Window, this, firsttime); 
    
    
}

void _PopUp::End(void)
{
  Cursor.End();
  if((Menu.pointer.PtrEnd != NULL) && (P_Window != NULL))
	  Menu.pointer.PtrEnd(P_Window, this);
 #ifdef UC_MENU_NEWSCROLL
  bPressOK = _FALSE;
 #endif    
}

void _PopUp::Run(_EVENT *pe)
{
  _ROOM       room;
  _Picture    *pPic = NULL;
  _Window     *pWin;
  _MY_PICTURE *pMyPic;
 #ifdef UC_MENU_NEWSCROLL 
  _DWORD      dtmpDif = 0;
 #endif
 
  if(SetUp.GetHotkeysInMenu() == _TRUE)
  {
    if((pe->ftype == _EVENT_KEYPRESS) || (pe->ftype == _EVENT_KEYRELEASE))
    {
      if(P_Window != NULL)
      {
        pWin = P_Window;

        while((pPic == NULL) && (pWin != NULL))
        {
          pPic = pWin->GetP_Picture();
          if(pPic == NULL)
            pWin = pWin->GetP_Background();
        }

        if(pPic != NULL)
          pMyPic = pPic->GetMyPicture();

        if(pMyPic != NULL)
        {
          if(pMyPic->No > 0)
            pMyPic->Ptr[pMyPic->No-1].DoKeyboard(pe, pWin, pMyPic->GlobalHotkeys);

        }
      }
    }
  }


 #ifdef HID
 #ifdef UC_MENU_NEWSCROLL
  if (do_POPUP_SCROLL(pe, &Menu.calcul.level, &Menu.position, &Menu.calcul.scroll_room, Menu.info.line_height) == _FALSE)
  {
 #endif
  track_HID_SCROLL(pe, &Menu.calcul.level, &Menu.position, &Menu.calcul.scroll_room, Menu.info.line_height);
 #ifdef UC_MENU_NEWSCROLL
  }
 #endif
 #endif
 
 #ifdef OK_SUBLIST
  if((Menu.pointer.PtrSubList != NULL)&&(P_Window != NULL))
    sublist_run(P_Window, this, pe);
  else
 #endif

  if((Menu.pointer.PtrRun != NULL) && (P_Window != NULL))
    Menu.pointer.PtrRun(P_Window, this, pe);

  if(ask_MENCURSOR(&Menu.mencursor) == _TRUE)
  {
    GetLinePosition(Menu.position.pos- Menu.position.begin, &room);
    Cursor.Set(&room, SetUp.GetEditColorCursor(_EDITACTIVE), 0);
  }

  DoPositions(pe);

 #ifdef UC_MENU_NEWSCROLL
  if (Menu.position.height < Menu.position.no)
  {
    popup_level_run(&Menu.calcul.level, (Menu.position.no - Menu.position.height+1), Menu.position, NULL, Menu.info.frametype, InvertColor_Makro(Menu.info.bar_color));
  }
  else
  {
    popup_level_run(&Menu.calcul.level, 0, Menu.position, NULL, Menu.info.frametype, InvertColor_Makro(Menu.info.bar_color));
  }
  
  //--- scroll follow-up movement
  switch (uScrollState)
  {
    case 0: // idle state
      udTimeLastMove = udTimeLastMove = get_abstime();
      udOldBegin = Menu.position.begin;
      dActMoveDelay = 0;
      break;
    
    case 1: // listen state (user is scrolling)
      // check scroll movement since last timer
      dtmpDif = Menu.position.begin - udOldBegin;
      
      if (dtmpDif != 0)
      {
        dActMoveDelay = (MENU_INITMOVEDELAY * Menu.info.line_height) / (dtmpDif * 10);
        
        if ((get_abstime() - udTimeLastMove) > (MENU_TIMESCROLL * Menu.info.line_height / 10))
        { 
          udTimeLastMove = get_abstime();
          udOldBegin = Menu.position.begin;
        }
      }
      else
      {
        if ((bIsRunning = _FALSE) || ((get_abstime() - udTimeLastMove) > MENU_TIMESCROLL))
        {
          dActMoveDelay = 0;
        }
      }
      break;
      
    case 2: // user finished scrolling - keep moving and slowly decelerate
      if (dActMoveDelay != 0)
      {
        bIsRunning = _TRUE;
        // check direction
        if (dActMoveDelay > 0)
        {
          if (get_abstime() - udTimeLastScroll >= dActMoveDelay)
          {
            Menu.position.begin ++;
            
            // slow down
            udTimeLastScroll = get_abstime();
            dActMoveDelay += MENU_SCROLLDECEL;
            
            // overscroll protection
            if ((Menu.position.begin > (Menu.position.no - Menu.position.height + 1))
            || (dActMoveDelay >= MENU_MAXMOVEDELAY)
            || (Menu.position.height >= Menu.position.no))
            {
              bIsRunning = _FALSE;
              uScrollState = 0;
            }
          }
        }
        else 
        {
          if (get_abstime() - udTimeLastScroll >= (-dActMoveDelay))
          {
            Menu.position.begin --;
            
            // slow down
            udTimeLastScroll = get_abstime();
            dActMoveDelay -= MENU_SCROLLDECEL;
            
            // overscroll protection
            if ((Menu.position.begin <= 0)
            || (absolut(dActMoveDelay) >= MENU_MAXMOVEDELAY))
            {
              if (Menu.position.begin <= 0)
                Menu.position.begin = 0;
              
              bIsRunning = _FALSE;
              uScrollState = 0;
            }
          }
        }
      }
      else
      {
        uScrollState = 0;
      }
        
  }
 #else
  level_run(&Menu.calcul.level, Menu.position.no, Menu.position.pos, _FALSE, NULL, Menu.info.frametype);
 #endif
}

void _PopUp::OutLine(_UWORD y, _UWORD no, _COLOR color, _BOOL beam, _BOOL background, _BOOL do_refresh)
{
  _ROOM   r;
  _REGION actreg, ioreg=NULL, newreg=NULL;

 #ifdef UC_MENU_NEWSCROLL
  if ((no >= Menu.position.begin) && (no < (Menu.position.begin + Menu.position.height)))
  {
 #endif

  Font.Set(Menu.info.font, &Menu.info.attrib);
  GetLinePosition(y, &r);

//  actreg = ActivateAndRegion(&ioreg, &newreg, &r, T_COPY|T_SOLID);
  actreg = ActivateAndRegion(&ioreg, &newreg, &r);

 #ifdef UC_MOUSE
  _UWORD hide = IfNecHideMouseCursor(&r);
 #endif

  Cursor.Hide();
 #ifdef OK_SUBLIST
  if((Menu.pointer.PtrSubList != NULL) && (P_Window != NULL))
    sublist_line(P_Window, this, no, &r, color, background);
  else
 #endif
  if((Menu.pointer.PtrLine != NULL) && (P_Window != NULL))
    Menu.pointer.PtrLine(P_Window, this, no, &r, color, background);
  else
    Bar(r.xy1.x, r.xy1.y, r.xy2.x, r.xy2.y, T_COPY|T_SOLID, InvertColor_Makro(color));

 #ifdef UC_ROTATE
  if(do_refresh == _TRUE)
    RefreshRectangle(&r);
 #endif
 #ifdef UC_MOUSE
  IfNecShowMouseCursor(hide);
 #endif

  DeactivateAndRegion(actreg, &ioreg, &newreg);

 #ifdef UC_MENU_NEWSCROLL
  }
 #endif
  
}

void _PopUp::GetSubPosition(_KOORD *x, _KOORD *y)
{
  _ROOM room;

  // berechne position für sub-popup-menue

  GetLinePosition(Menu.position.pos-Menu.position.begin, &room);
  *y = room.xy1.y;
  *x = Menu.calcul.out_room.xy2.x + 1;
}

void calculate_window(_MENUCALCUL *pc, _MENUINFO *pm, _POSITION *pp, _MENUPOINTER *poi)
{
  _ROOM  room;
  _KOORD corr;
  _UWORD title;
//  _CHAR  *txt;
  _COLOR tcol;

  free_MENUCALCUL(pc);
  pp->height         = pm->height;
  pp->width          = pm->width;
  pc->out_room.xy1.x = pm->xy.x;
  pc->out_room.xy1.y = pm->xy.y;

	title = 0;
//  if(pm->Title.GetNo() > 0)
//  {
//    txt = pm->Title.GetText(0);
//    if(txt != NULL)
//    {
//      if(StrLen(txt, sizeof(_CHAR)) != 0)
//      {
//        title = pm->line_height + 2;
//      }
//    }
//  }

  if(pm->frametype < _THEMEFRAME)
  {
	  pc->scroll_room.xy1.x = pc->out_room.xy1.x + 4;
	  pc->scroll_room.xy1.y = pc->out_room.xy1.y + 4 + title;
	  pc->scroll_room.xy2.x = pc->scroll_room.xy1.x + pm->width;
	  pc->scroll_room.xy2.y = pc->scroll_room.xy1.y + pm->height * pm->line_height - 1;

	  pc->level.room.xy1.x  = pc->scroll_room.xy2.x + 1;
	  pc->level.room.xy1.y  = pc->scroll_room.xy1.y - 1;
	  pc->level.room.xy2.x  = pc->level.room.xy1.x + LEVELBUTTON_WIDTH;
	  pc->level.room.xy2.y  = pc->scroll_room.xy2.y + 1;
  }
  else
  {
	  pc->scroll_room.xy1.x = pc->out_room.xy1.x;
	  pc->scroll_room.xy1.y = pc->out_room.xy1.y + title;
	  pc->scroll_room.xy2.x = pc->scroll_room.xy1.x + pm->width;
	  pc->scroll_room.xy2.y = pc->scroll_room.xy1.y + pm->height * pm->line_height - 1;

	  pc->level.room.xy1.x  = pc->scroll_room.xy2.x + 1;
	  pc->level.room.xy1.y  = pc->scroll_room.xy1.y;
	  pc->level.room.xy2.x  = pc->level.room.xy1.x + LEVELBUTTON_WIDTH;
	  pc->level.room.xy2.y  = pc->scroll_room.xy2.y;
  }

  tcol                  = GetBackColor_Makro(pm->text_color);
	pc->level.color       = MakeColor_Makro(tcol, tcol); //pm->text_color;
	pc->level.frame_color = pm->frame_color;

	if((pp->no > pm->height)||(poi->PtrSubList != NULL)||(poi->Level == _TRUE))
	{
    pc->level.active    = _TRUE;
	}
	else
	{
    pc->level.active      = _FALSE;
    pc->level.room.xy2.x  = pc->level.room.xy1.x;
	}

  if(pm->frametype < _THEMEFRAME)
  {
	  pc->out_room.xy2.x = pc->level.room.xy2.x + 3;
	  pc->out_room.xy2.y = pc->level.room.xy2.y + 3;
  }
  else
  {
	  pc->out_room.xy2.x = pc->level.room.xy2.x;
	  pc->out_room.xy2.y = pc->level.room.xy2.y;
  }

  pc->out_room.xy2.y += pm->basement;

	MyGetScreenSize(&room);
#ifdef ZOOM_KM
 room.xy2.y -= SOFTKEY_NEG_OFF_X;
#endif
	room.xy2.y -= (SetUp.GetSoftkeyHeight() + POPUP_SHADOW);
	room.xy2.x -= POPUP_SHADOW;

	if(pc->out_room.xy2.x > room.xy2.x)
	{
    corr = pc->out_room.xy2.x - room.xy2.x;
    pc->out_room.xy1.x    -= corr;
    pc->out_room.xy2.x    -= corr;
    pc->scroll_room.xy1.x -= corr;
    pc->scroll_room.xy2.x -= corr;
    pc->level.room.xy1.x  -= corr;
    pc->level.room.xy2.x  -= corr;
    pm->xy.x              -= corr;
  }

  if(pc->out_room.xy2.y > room.xy2.y)
  {
    corr = pc->out_room.xy2.y - room.xy2.y;
    pc->out_room.xy1.y    -= corr;
    pc->out_room.xy2.y    -= corr;
    pc->scroll_room.xy1.y -= corr;
    pc->scroll_room.xy2.y -= corr;
    pc->level.room.xy1.y  -= corr;
    pc->level.room.xy2.y  -= corr;
    pm->xy.y              -= corr;
  }
}

void draw_window(_MENU *pm, _CHAR *title, _BOOL shadow)
{
  _COLOR      coltxt, colframe, colback;
  _ROOM       room;
  _MENUCALCUL *pc = &pm->calcul;
  _COLOR      col = InvertColor_Makro(pm->info.text_color);

 #ifdef UC_MOUSE
  _UWORD hide = IfNecHideMouseCursor(&pc->out_room);
 #endif

  if(shadow == _TRUE)
    DrawShadow(pc->out_room, pm->info.frametype);

  DrawButton(&pc->out_room, pm->info.attrib, (pm->info.frametype < _THEMEFRAME)? _3DFRAME : pm->info.frametype, col, InvertColor_Makro(pm->info.frame_color));
 #ifdef UC_ROTATE
  RefreshRectangle(&pc->out_room);
 #endif


  room = pc->scroll_room;
  room.xy1.x --;
  room.xy1.y --;
  room.xy2.x ++;
  room.xy2.y ++;

  if(pm->info.frametype < _THEMEFRAME)
    DrawButton(&room, T_SOLID|T_FILL|T_COPY, _3DFRAME, col, pm->info.frame_color);

  if(DrawBackGround == 0)
  {
   #ifndef UC_MENU_NEWSCROLL  
    level_draw(&pc->level, _FALSE, _TRUE, NULL, pm->info.frametype);
    level_run(&pc->level, 0, 0, _FALSE, NULL, pm->info.frametype);
   #else
    popup_level_draw(&pc->level, _TRUE, NULL, pm->info.frametype);
    popup_level_run(&pc->level, 0, pm->position, NULL, pm->info.frametype, InvertColor_Makro(pm->info.bar_color));
   #endif
  }
  else
  {
   #ifndef UC_MENU_NEWSCROLL
    level_draw(&pc->level, _FALSE, _FALSE, NULL, pm->info.frametype);
    level_run(&pc->level, pm->position.no, pm->position.pos, _FALSE, NULL, pm->info.frametype);
   #else
    popup_level_draw(&pc->level, _FALSE, NULL, pm->info.frametype);
    popup_level_run(&pc->level, pm->position.no, pm->position, NULL, pm->info.frametype, InvertColor_Makro(pm->info.bar_color));
   #endif
  }

  if(title != NULL)
  {
    if(StrLen(title, sizeof(_CHAR)) != 0)
    {
      room.xy1.x = pc->out_room.xy1.x + 1;
      room.xy2.x = pc->out_room.xy2.x - 1;
      room.xy1.y = pc->out_room.xy1.y + 1;
      room.xy2.x = pc->scroll_room.xy1.x - 2;

      coltxt   = MakeColor_Makro(INVISIBLE, pm->info.text_color);
      colback  = MakeColor_Makro(GetBackColor_Makro(pm->info.text_color), GetBackColor_Makro(pm->info.text_color));
      colframe = MakeColor_Makro(INVISIBLE, INVISIBLE);
      OutMultiTextAlign(title, 0, &room, pm->info.font, T_COPY|T_SOLID|T_MIDBOUND|T_CENTERBOUND, _DEFFRAME, coltxt, colback, colframe, sizeof(_CHAR));
    }
  }

 #ifdef UC_MOUSE
  IfNecShowMouseCursor(hide);
 #endif
}

_MENU* _PopUp::GetMenu(void)
{
  return(&Menu);
}

void _PopUp::GetSize(_ROOM *pr)
{
  *pr = Menu.calcul.out_room;
}

_VARLABEL* _PopUp::GetImOverloaded(void)
{
  return(&ImOverloaded);
}

_VARLABEL* _PopUp::GetInput(void)
{
  return(&Input);
}

void* _PopUp::GetP_MyIo(void)
{
  return P_MyIo;
}

void _PopUp::OutBasement(_KOORD x, _UWORD width, _ASCII *txt, _COLOR col)
{
  _ROOM room;

  if(Menu.info.basement > 0)
  {
    room.xy1.x = Menu.calcul.scroll_room.xy1.x;
    room.xy2.x = Menu.calcul.scroll_room.xy2.x;
    room.xy1.y = Menu.calcul.scroll_room.xy2.y + 3;
    room.xy2.y = Menu.calcul.out_room.xy2.y - 2;
    room.xy1.x += x;
	if(width > 1)
      room.xy2.x = room.xy1.x + width - 1;
    OutMultiTextAlign(txt, 0, &room, Menu.info.font, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, _DEFFRAME, GetPenColor_Makro(col), GetBackColor_Makro(col), MakeColor_Makro(INVISIBLE, INVISIBLE), sizeof(_ASCII));
   #ifdef UC_ROTATE
    RefreshRectangle(&room);
   #endif
  }
}

#ifdef UC_MENU_NEWSCROLL
_BOOL _PopUp::do_POPUP_SCROLL(_EVENT *pe, _LEVEL *pl, _POSITION *pp, _ROOM *scroll_room, _UWORD lineheight)
{
  _UDWORD h, y;
  _WORD thumbHeight;

  // in some cases, hardware does not provide a HIDPRESS,
  // but directly sends HIDMOVE events, so we have to simulate HIDPRESS
  switch(pe->ftype)
  {
    case _EVENT_HIDMOVE:
      if (bPressOK == _FALSE)
      {
        pe->ftype = _EVENT_HIDPRESS;
        prDot = pe->dot;
        bPressOK = _TRUE;
      }
      break;

    case _EVENT_HIDPRESS:
      if(bPressOK == _FALSE)
      {  
        prDot = pe->dot;
        bPressOK = _TRUE;
      }
      else
      {
        // we already sent a "fake" press event
        return _TRUE;
      }
      break;
    case _EVENT_HIDRELEASE:
      bPressOK = _FALSE;
      break;
  }
    

  // hid in vertical level
  if((pl != NULL) && (pp != NULL) &&
     (((pl->active == _TRUE) &&
     (pe->dot.x >= pl->room.xy1.x) && (pe->dot.x <= pl->room.xy2.x) &&
     (pe->dot.y >= pl->room.xy1.y) && (pe->dot.y <= pl->room.xy2.y) &&
     (pe->ftype != _EVENT_NONE)) || (pl->drag == _TRUE)))
  {
    if(pp->no > 0)
    {
      thumbHeight = pp->height * (pl->room.xy2.y - pl->room.xy1.y) / pp->no;
      if (thumbHeight < LEVELBUTTON_HEIGHT)
        thumbHeight = LEVELBUTTON_HEIGHT;
  
      if((pe->ftype == _EVENT_HIDPRESS) || (pe->ftype == _EVENT_HIDRELEASE))
      {
        if (pe->ftype == _EVENT_HIDRELEASE)
        {
          pl->drag = _FALSE;
          pl->old_pos = -1; // cause redraw of thumb
          return _TRUE;
        }
          
        if(pe->dot.y <= (pl->room.xy1.y))
        {
          return _FALSE;
        }
        else if(pe->dot.y >= (pl->room.xy2.y))
        {
          return _FALSE;
        }
        else if(pe->dot.y <= (pl->room.xy1.y + pl->old_pos))
        {
          if(pe->ftype == _EVENT_HIDPRESS)
            pp->begin -= pp->height;
          return _TRUE;
        }
        else if(pe->dot.y >= (pl->room.xy1.y + thumbHeight + pl->old_pos))
        {
          if(pe->ftype == _EVENT_HIDPRESS)
            pp->begin += pp->height;
          return _TRUE;
        }
        else // hidpress on level
        {
          pl->drag = _TRUE;
        }

      }

      if((pl->drag == _TRUE) && (pe->ftype != _EVENT_NONE))  // level drag and drop
      {
        if(pe->dot.y < (pl->room.xy1.y + (thumbHeight/2)))
        {
          pp->begin = 0;
          return _TRUE;
        }
        else if(pe->dot.y > (pl->room.xy2.y - (thumbHeight/2)))
        {
          pp->begin = pp->no - 1;
          return _TRUE;
        }
        else
        {
          h = (pl->room.xy2.y - pl->room.xy1.y + 1) - (thumbHeight);
          y = pe->dot.y - (pl->room.xy1.y + thumbHeight/2);

          if(pp->lineheight == NULL)
          {
            y = AMBDC(y, pp->no, h);
            if(y < pp->no)
            {
              if (pe->ftype != _EVENT_HIDRELEASE)
              {
                pp->begin = y;
              }
              else
              {
                pl->drag = _FALSE;
              }
              return _TRUE;
            }
          }
          else
          {
            y = AMBDC(y, pp->totalheight, h);
            if(y < pp->no)
              pp->begin = y;
            return _TRUE;
          }
        }
      }
    }
  }  
  
  
  // hid in scroll area or outside 
  switch(pe->ftype)
  {
    case _EVENT_HIDPRESS:   
      uScrollState = 0;
      return _TRUE;
      break;
    
    case _EVENT_HIDMOVE:
      // vertical movement - check direction
      // only if we have valid coords
      if ((prDot.x != -1) && (prDot.y != -1))
      {
        if (pe->dot.y < prDot.y)
        {
          if ((prDot.y - pe->dot.y) > lineheight)
          {
            if (pp->height < pp->no)
            {
              pp->begin ++;
              uScrollState = 1;
            }
            prDot = pe->dot;
            return _TRUE;
          }        
        }
        else
        {
          if ((pe->dot.y - prDot.y) > lineheight)
          {
            pp->begin --;
            prDot = pe->dot;
            uScrollState = 1;
            return _TRUE;
          }
        }
      }
      break;

      
    case _EVENT_HIDRELEASE:
      if (uScrollState == 0)
      {
        if (bIsRunning == _FALSE)
        {
          pe->ftype = _EVENT_HIDPRESS;
          uScrollState = 0;
          return _FALSE;
          break;
        }
        else
        {
          bIsRunning = _FALSE;
          return _TRUE;
          break;
        }
      }
      else 
      {
        if (bIsFirstStart == _TRUE)
        {
          bIsFirstStart = _FALSE;
          uScrollState = 0;
          return _TRUE;
        }
        else
        {
          uScrollState = 2;
          return _TRUE;
        }
      }
      break;
  }    

  return _FALSE;
}
#endif

#ifdef UC_MENU_NEWSCROLL
_POSRESULT _PopUp::calculate_PopUpPosition(_POSITION *pos, _EVENT *pe, _UWORD step)
{
  _POSRESULT retcode = P_NOP;

  if((pe != NULL)&&(pos != NULL))
  {
    if(pos->no > 0)
    {
      if(pe->ftype == _EVENT_KEYPRESS)
      {
        switch(pe->scancode)
        { 
         #ifdef UC_MENU_NEWSCROLL
          case _UP     : pos->begin --;
         #else
          case _UP     : pos->pos --;
         #endif
                         break;
         #ifdef UC_MENU_NEWSCROLL
          case _DOWN   : pos->begin ++;
         #else
          case _DOWN   : pos->pos ++;
         #endif
                         break;
          case _PGUP   : pos->pos   -= step;
                         pos->begin -= step;
                         break;
          case _PGDOWN : // dr856
		                 // alt
		                 //pos->pos += step;
                         //pos->begin += step;
                         // neu
		                 if((pos->pos + step) < pos->no)
		                   pos->pos += step;
                         else
						   pos->pos = pos->no - 1;

                         if(pos->pos >= (pos->begin + pos->height))
                           pos->begin += step;
                         // dr856 ende
                         break;
        }
      }
      else if(pe->ftype == _EVENT_SHUTTLEINC)
      {
        pos->pos += pe->scancode;
      }
      else if(pe->ftype == _EVENT_SHUTTLEDEC)
      {
        pos->pos -= pe->scancode;
      }

      if((pos->oldpos != pos->pos)||(pos->oldbegin != pos->begin))
      {
        if(pos->pos < 0)
          pos->pos = 0;

        if(pos->begin < 0)
          pos->begin = 0;

        if(pos->pos >= pos->no)
        {
          if(pos->no > 0)
            pos->pos = pos->no - 1;
          else if((pos->oldpos > 0) && (pos->oldpos < pos->no))
            pos->pos = pos->oldpos;
          else
            pos->pos = 0;
        }
        
        // HubChr: was, wenn wir nicht von einem Menü aus aufgerufen werden?    
        if(pos->no > pos->height)
        {
          if(pos->begin > (pos->no - pos->height))
          {
            pos->begin = pos->no - pos->height;
          }
        }

        if((pos->oldpos != pos->pos)||(pos->oldbegin != pos->begin))
        {
          retcode = P_CURSOR;

          if(pos->oldbegin != pos->begin)
          {
            retcode = P_REDRAW;

            if(pos->begin == (pos->oldbegin + 1))
              retcode = P_SCROLLUP;
            else if(pos->begin == (pos->oldbegin - 1))
              retcode = P_SCROLLDOWN;
          }
        }

        pos->oldpos   = pos->pos;
        pos->oldbegin = pos->begin;

      }
    }
  }

  return(retcode);
}
#endif

#ifdef UC_MENU_NEWSCROLL
void popup_level_draw(_LEVEL *p, _BOOL firsttime, _BMP *pbmp, _FRAMETYPE frame)
{
  _ATTRIB att = T_SOLID|T_COPY|T_FILL;
  _ROOM   room;
  _COLOR  tmp, color = InvertColor_Makro(p->color);
  _BOOL   isSlim = IsSlimDesign(frame);

  if(p->active == _TRUE)
  {
   #ifdef UC_MOUSE
    _UWORD hide = IfNecHideMouseCursor(&p->room);
   #endif
   
    //Scrollbereiche zeichnen
    if(frame < _THEMEFRAME)
    {
     #ifdef UC_MENU_TWOCOLOR
       tmp = GetPenColor_Makro(p->frame_color);
       color = MakeColor_Makro(tmp, tmp);
      DrawButton(&p->room, att, _3DFRAME, color, color);
     #else
      DrawButton(&p->room, att, _3DFRAME, p->color, p->frame_color);
     #endif
    }
    else
    {
      DrawButton(&p->room, att|T_INVERTTHEME, frame, p->color, p->frame_color);

      tmp = GetPenColor_Makro(p->frame_color);
      if(tmp != INVISIBLE)
        color = MakeColor_Makro(tmp, tmp);  
    }


    if (isSlim == _TRUE)
    {
      //schmalen Scrollbarschiebebereich zeichnen
      GetSlimSliderDecentRoom(&room, &p->room, _FALSE, p->pguppgdown);
      room = GetRoomWithoutButtons(room, isSlim);
      DrawButton(&room, att, frame, GetBackColor_Makro(p->frame_color), p->color);
    }

   #ifdef UC_MOUSE
    IfNecShowMouseCursor(hide);
   #endif

    if(firsttime == _TRUE)
      p->old_pos = -1;
    else
      p->old_pos = 0;

  }
}
#endif

#ifdef UC_MENU_NEWSCROLL
void popup_level_run(_LEVEL *p, _DWORD no, _POSITION pos, _BMP *pbmp, _FRAMETYPE frame, _COLOR prColor)
{
  _KOORD  y, y1, y2, h;
  _ROOM   room;
  _ATTRIB att = T_COPY|T_SOLID|T_FILL;
  _COLOR  color;
  _BOOL   isSlim = IsSlimDesign(frame); 
  _WORD   thumbHeight;
  
 #ifdef UC_MOUSE
  _UWORD hide;
 #endif

  if(p->active == _TRUE)
  {
    if (isSlim == _FALSE)
    {
      y1 = p->room.xy1.y + 2;
      y2 = p->room.xy2.y - 2;
    }
    else
    {
      y1 = p->room.xy1.y + LEVELBUTTON_HEIGHT/2 + 2;
      y2 = p->room.xy2.y - LEVELBUTTON_HEIGHT/2 - 2;
    }
    thumbHeight = pos.height * (y2 - y1) / pos.no;
    if (thumbHeight < LEVELBUTTON_HEIGHT)
      thumbHeight = LEVELBUTTON_HEIGHT;
    h = y2 - y1 - thumbHeight;

    if(h > 0)
    {
      if((pos.begin == 0)||(no < 2))
      {
        y = 0;
      }
      else if(pos.begin >= (no-1))
      {
        y = h;
      }
      else
      {
        y = (h * pos.begin)/(no-1);
      }

      if((y != p->old_pos)||(p->old_pos == -1))
      {
        room.xy1.x = p->room.xy1.x + 1;
        room.xy2.x = p->room.xy2.x - 1;

        if(p->old_pos >= 0)
        {
          if(is_invisible(p->color, att) == _FALSE)
          {
            //Alten Scrollbarschieber löschen
            room.xy1.y = y1 + p->old_pos;
            room.xy2.y = room.xy1.y + thumbHeight; 

           #ifdef UC_MOUSE
            hide = IfNecHideMouseCursor(&room);
           #endif
           #ifdef UC_THEME
            if(frame < _THEMEFRAME)
           #endif
            {
             #ifdef UC_MENU_TWOCOLOR
              Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, att, p->frame_color);
             #else
              Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, att, p->color); //p->color);
             #endif
            }
           #ifdef UC_THEME
            else
            {
              if (isSlim == _TRUE)
              {
                GetSlimSliderDecentRoom(&room, &p->room, _FALSE, p->pguppgdown);
                room = GetRoomWithoutButtons(room, isSlim);
                draw_THEME(&room, frame, T_COPY|T_SOLID|T_INVERTTHEME, GetBackColor_Makro(p->frame_color), _TRUE, _FALSE);
              }
              else
              {
                room.xy1.x -= 1;
                room.xy2.x += 1;
                draw_THEME_part(&room, frame, T_COPY|T_SOLID, p->color, _TRUE, _FALSE);
              }
            }
           #endif
           #ifdef UC_ROTATE
            RefreshRectangle(&room);
           #endif
           #ifdef UC_MOUSE
            IfNecShowMouseCursor(hide);
           #endif
          }
        }
        //Neuen Scrollbarschieber zeichnen
        p->old_pos = y;
        room.xy1.y = y1 + y;
        room.xy2.y = room.xy1.y + thumbHeight;
       #ifdef UC_MOUSE
        hide = IfNecHideMouseCursor(&room);
       #endif
        if(pbmp == NULL)
        {
          if(frame < _THEMEFRAME)
          {
           #ifdef UC_MENU_TWOCOLOR
            if (p->drag == _TRUE)
              color = prColor;
            else
              color = InvertColor_Makro(p->frame_color);
              
            DrawButton(&room, att, _DEFFRAME, color, color);
           #else
            DrawButton(&room, att, _3DFRAME, InvertColor_Makro(p->color), InvertColor_Makro(p->frame_color));
           #endif            
          }
          else
          {
            color = GetPenColor_Makro(p->frame_color);
            if(color != INVISIBLE)
              color = MakeColor_Makro(color, color);
            else
            {
              color = GetBackColor_Makro(p->color);
            }
              
            room.xy1.x = p->room.xy1.x + 1;
            room.xy2.x = p->room.xy2.x;
                        
            if(isSlim == _TRUE)
            {
              GetSlimSliderRoomWidth(&room, &p->room, _FALSE);
            }
            DrawButton(&room, att, frame, color, color);
          }
        }
        else
        {
          out_zoom_BMP(&room, pbmp, T_SOLID);
        }

       #ifdef UC_ROTATE
        RefreshRectangle(&room);
       #endif
       #ifdef UC_MOUSE
        IfNecShowMouseCursor(hide);
       #endif
      }
    }
  }
}
#endif

#ifdef UC_MENU_NEWSCROLL
_ROOM  GetRoomWithoutButtons(_ROOM room, _BOOL isSlim)
{
  if (isSlim == _TRUE)
  {
    room.xy1.y -= LEVELBUTTON_HEIGHT/2;
    room.xy2.y += LEVELBUTTON_HEIGHT/2;
  }
  else
  {
    room.xy1.y -= LEVELBUTTON_HEIGHT;
    room.xy2.y += LEVELBUTTON_HEIGHT;
  }
  
  return room;
  
}
#endif