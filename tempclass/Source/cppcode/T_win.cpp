#include "T_IPC.H"
#include "T_NUC.H"


static _UDWORD RunCloseExData;

void _PicMemo::Init(void)
{
  Index = 0;
}

void _PicMemo::Free(void)
{
  Init();
}

_UWORD _PicMemo::GetNo(void)
{
  return(Index);
}

void _PicMemo::Add(_UWORD no, _UWORD skl)
{
  _UWORD size;

  if(Index < NO_PICMEMO)
  {
    Memo[Index].no = no;
    Memo[Index].skl = skl;
    Index ++;
  }
  else
  {
    size = (NO_PICMEMO - 1) * sizeof(_PICMEMO_SKL);
    MemMove(&Memo[0], &Memo[1], size);
    Memo[NO_PICMEMO-1].no = no;
    Memo[NO_PICMEMO-1].skl = skl;
  }
}

_PICMEMO_SKL *_PicMemo::Get(void)
{
  _PICMEMO_SKL *retcode = NULL;

  if(Index > 1)
  {
    Index --; 
    retcode = &Memo[Index-1]; 
  }

  return(retcode); 
}

void _Window::Init(void)
{
  _Stack::Init();
  P_Window     = NULL;
  P_Background = NULL;
  P_Picture    = NULL;
  P_PopUp      = NULL;
  MessBoxIndex = 0xFFFFFFFF;
  init_REGION(&Region);
  Softkey.Init();
  init_WHOAMI(&WhoAmI);
  init_DOT(&PicOffset);
  IsReady      = _FALSE;
  Shadow       = _FALSE;
  Sign         = _FALSE;
  TouchEditorFlag = _TRUE;
  DisableEvents = _FALSE;
  EventBreather = 0; // verschnaufpause für refresh schaffen
  ScreenSaverEndCnt = 0;
  EditServerNo = DEFSCOPE;
}

void _Window::Free(void)
{
  if(P_Window != NULL)
  {
    P_Window->Free();
    Memory2((void**)&P_Window, 0);
  }

  if(P_Picture != NULL)
  {
    P_Picture->Free();
    Memory2((void**)&P_Picture, 0);
  }

  if(P_PopUp != NULL)
  {
    P_PopUp->Free();
    Memory2((void**)&P_PopUp, 0);
  }

//  DoCommands();

  _Stack::Free();
  Softkey.Free();
  free_REGION(&Region);

  free_WHOAMI(&WhoAmI);

  Init();
}

_BOOL _Window::GetEvent(_EVENT *p)
{
  if((P_Window == NULL) && ((DisableEvents == _FALSE)))
  {
    // nur wenn topwindow und events sind nicht disabled
    EventBreather += 1;
    if(EventBreather < 10) // verschnaufpause für refresh realisieren
    {
      Keyboard.GetEvent(p);
      switch(p->ftype)
      {
        case _EVENT_NONE       : EventBreather = 0; // das ist eine verschnaufpause
                                 break;
     #ifdef HID
        case _EVENT_TRIGGER    :
        case _EVENT_HIDPRESS   :
        case _EVENT_HIDRELEASE : Softkey.DoHID(p);
                                 break;
     #endif
        default                : break;
      }
   
     return _TRUE;
    }
    EventBreather = 0;
  }
  
  init_EVENT_Makro(p);
  return _FALSE;
}

_Window *_Window::FindTop(void)
{
  if(P_Window != NULL)
    return P_Window->FindTop();
    
  return this;
}

void _Window::GetUsedMessBoxIndex(_UBYTE *pmidx)
{
  if(MessBoxIndex < MESSAGEBOXNO)
    pmidx[MessBoxIndex] = 1;
    
  if(P_Window != NULL)
    P_Window->GetUsedMessBoxIndex(pmidx);
}

_Window *_Window::FindRoot(void)
{
  if(P_Background != NULL)
    return P_Background->FindRoot(); 
    
  return this; 
}

void _Window::SubtractRegion(_REGION *pr)
{
  _REGION tmp;

  if(P_Window != NULL)
  {
    P_Window->SubtractRegion(pr);
    tmp = SubRegion(*pr, P_Window->GetRegion());
    FreeRegion(pr);
    *pr = tmp;
  }
}

_BOOL _Window::AddCommand(_PIC_COMMAND cmd, _VARLABEL *op1, _VARLABEL *op2, _BOOL lrm)
{
  _BOOL retcode;

  if((op1 == NULL) && (op2 == NULL))
  {
    retcode = AddCmd(cmd, 0, 0, lrm);
  }
  else
  {
    switch(cmd)
    {
      case CMD_EXITPROGRAM : retcode = FindTop()->InsCmd(cmd, op1, op2, lrm);
                             break;
     #ifdef _WIN_SERVERHELP
      case CMD_SERVERHELP  :
     #endif
      case CMD_NEWTOUCHEDITOR :
      case CMD_NEWPOPUP    :
      case CMD_IOPOPUP_HID :
      case CMD_IOPOPUP     :
      case CMD_NEWWINDOW   : retcode = FindTop()->InsCmd(cmd, op1, op2, lrm);
                             break;
      case CMD_NEWPICRET   :
      case CMD_NEWPICTURE  : retcode = FindTop()->InsCmd(cmd, op1, op2, lrm);
                             break;
      default              : retcode = InsCmd(cmd, op1, op2, lrm);
                             break;
    }
  }

  return(retcode);
}

void _Window::DeleteAllCmd(void)
{
  FindTop()->DeleteAllCmd();
}

_BOOL _Window::AddCmd(_PIC_COMMAND cmd, _DWORD v1, _DWORD v2, _BOOL lrm)
{
  _VARLABEL op1, op2;

  init_VARLABEL_Makro(&op1);
  init_VARLABEL_Makro(&op2);

  op1.no = 1;
  op1.info[0].state = CONST_VAL;
  op1.info[0].value = v1;

  op2.no = 1;
  op2.info[0].state = CONST_VAL;
  op2.info[0].value = v2;

  return(AddCommand(cmd, &op1, &op2, lrm));
}

void _Window::DrawBackground(_REGION *reg)
{
  if(P_Background != NULL)
  {
   #ifndef UC_REDRAW_ALL
    DrawBackGround ++;
    P_Background->Draw(_FALSE, _FALSE);
    _REGION region = P_Background->GetRegion();
    if(region != NULL)
    {
      _REGION newreg = SubRegion(*reg, region);
      DeactivateRegion();
      FreeRegion(reg);
      *reg = newreg;
      ActivateRegion(newreg);
    }
    P_Background->DrawBackground(reg);
    DrawBackGround --;

   #else
    DrawBackGround ++;
    P_Background->DrawBackground(reg);
    P_Background->Draw(_FALSE, _FALSE);
    DrawBackGround --;
   #endif
  }
}

_BOOL _Window::LoadPicWin(_UWORD no, _IMA ima, _Window *back)
{
  _Picture   *tmp = NULL;
  _BOOL      retcode = _FALSE;
  _ASCII     dpne[_MAX_PATHLENGTH];
  _PRJ_ENTRY *pe = NULL;

  dpne[0] = 0;

  if(ima == IMA_PICTURE)
  {
    if(Project.GetPictureAccess(dpne, &no, _FALSE, &pe) == _TRUE)
    {
      if(Memory2((void**)&tmp, sizeof(_Picture)) != 0)
      {
        tmp->Init();
        retcode = tmp->LoadPicture(dpne, no, this, _FALSE, no);
        if(retcode == _FALSE)
        {
          Memory2((void**)&tmp, 0);
        }
      }
    }
    else
    {
      SystemError(NO_ACCESS, no, NULL);
    }

    if(retcode == _TRUE)
    {
      if(pe != NULL)
        tmp->RefCyc = pe->refcyc;

      EndIntern(_FALSE);
      P_Picture = tmp;
      tmp       = NULL;
      P_Picture->SetReturn(this);
      IAm(ima, no);
    }
  }
  else if((ima == IMA_WINDOW)||(ima == IMA_TOUCHEDITOR))
  {
    if(Project.GetWindowAccess(dpne, &no, _FALSE, &pe) == _TRUE)
    {
      if(Memory2((void**)&tmp, sizeof(_Picture)) != 0)
      {
        tmp->Init();
        retcode = tmp->LoadWindow(dpne, no, this);
        P_Background = back;

        if(retcode == _TRUE)
        {
          SetShadow(tmp->GetMyPicture()->DrawShadow);
          if(P_Picture != NULL)
          {
            P_Picture->Free();
            Memory2((void**)&P_Picture, 0);
          }

          if(pe != NULL)
            tmp->RefCyc = pe->refcyc;

          P_Picture = tmp;
          tmp       = NULL;
          P_Picture->SetReturn(this);
          IAm(ima, no);
        }
        else
        {
          Memory2((void**)&tmp, 0);
        }
      }
    }
    else
    {
      SystemError(NO_ACCESS, no, NULL);
    }
  }
  else if(ima == IMA_SCREENSAVER)
  {
    if(Memory2((void**)&tmp, sizeof(_Picture)) != 0)
    {
      tmp->Init();
      P_Background = back;
      retcode      = _TRUE;
      MyGetScreenSize(&tmp->GetMyPicture()->Room);

      if(P_Picture != NULL)
      {
        P_Picture->Free();
        Memory2((void**)&P_Picture, 0);
      }

      P_Picture = tmp;
      P_Picture->SetReturn(this);
      IAm(ima, no);
    }
  }

  Keyboard.SetHidPrePress();

  return(retcode);
}

_BOOL _Window::LoadPopUp(_UWORD no, _Window *back, _KOORD x, _KOORD y, _VARLABEL *vl, _VARLABEL *ovl, void *pmyio)
{
  _PopUp *Pp = NULL;
  _BOOL  retcode = _FALSE;

  if(Memory2((void**)&Pp, sizeof(_PopUp)) != 0)
  {
    Pp->Init();

    if((vl == NULL) && (x == -1) && (y == -1))
      retcode = Pp->Load(no, this, vl, ovl); // menue wurde geöffnet
    else
      retcode = Pp->Open(no, this, x, y, vl, ovl, pmyio); // menue wurde von editor geöffnet

    P_Background = back;

    if(retcode == _FALSE)
    {
      Pp->Free();
      Memory2((void**)&Pp, 0);
    }
    else
    {
      if(P_PopUp != NULL)
      {
        P_PopUp->Free();
        Memory2((void**)&P_PopUp, 0);
      }

      P_PopUp = Pp;
      IAm(IMA_POPUP, no);
    }
  }

  return(retcode);
}

_BOOL _Window::GetReady(void)
{
  return(IsReady);
}

_BOOL _Window::NewWindowBasics(_PIC_COMMAND cmd , _DWORD nr)
{
  _BOOL retcode = _FALSE;

  if(P_Window == NULL)
  {
   #ifdef UC_BUBBLE_HELP
    CMD_CloseBubbleHelp();
   #endif
    if(Memory2((void**)&P_Window, sizeof(_Window)) != 0)
    {
      if(P_Picture != NULL)
        P_Picture->Cursor.Hide();
      if(P_PopUp != NULL)
        P_PopUp->Cursor.Hide();

      P_Window->Init();
      retcode = _TRUE;
    }
  }
  else
  {
    // shift command to next window
    P_Window->AddCmd(cmd, nr, 0, _FALSE);
  }

  return(retcode);
}

_BOOL _Window::CMD_NewWindow(_PIC_COMMAND cmd, _DWORD nr)
{
  _BOOL retcode = _FALSE;

  if(NewWindowBasics(cmd, nr) == _TRUE)
  {
    if(P_Window->LoadPicWin(nr, (cmd == CMD_NEWWINDOW)? IMA_WINDOW : IMA_TOUCHEDITOR, this) == _TRUE)
    {
      if((nr != UC_NEWBUBBLEWINDOW) && (nr != UC_CONFIRMWINDOW))
      {
        P_Window->IfNecCopySoftkey(&Softkey);
      }
//      shadow = _TRUE;
//      if(P_Picture != NULL)
//        shadow = P_Picture->GetMyPicture()->DrawShadow;
//      P_Window->SetShadow(shadow);
      retcode = _TRUE;
    }

    if(retcode == _FALSE)
    {
      P_Window->Free();
      Memory2((void**)&P_Window, 0);
    }

  }

  return(retcode);
}

_BOOL _Window::CMD_ScreenSave(void)
{
  _BOOL retcode = _FALSE;

  if(NewWindowBasics(CMD_SCREENSAVE, 0) == _TRUE)
    retcode = P_Window->LoadPicWin(0, IMA_SCREENSAVER, this);

  return(retcode);
}

_BOOL _Window::IfNecCopySoftkey(_Softkey *Ps)
{
  _BOOL retcode = _TRUE;


  if(Softkey.GetNoFloor() == 0)
  {
    retcode = Softkey.Copy(Ps);
    if(retcode == _TRUE)
      Softkey.SetCopyOfPrevious(_TRUE);
  }

  return(retcode);
}

_BOOL _Window::CMD_NewPopUp(_DWORD nr, _VARLABEL *ovl, _KOORD x, _KOORD y, _VARLABEL *vl, void *pmyio)
{
  _VARLABEL *pv;
  _BOOL     retcode = _FALSE;

  if(P_Window == NULL)
  {
   #ifdef UC_BUBBLE_HELP
    CMD_CloseBubbleHelp();
   #endif

    if(Memory2((void**)&P_Window, sizeof(_Window)) != 0)
    {
      if(P_PopUp != NULL)
      {
        P_PopUp->GetSubPosition(&x, &y);                // position sub-popup
        pv = P_PopUp->GetInput();
        if(chk_VARLABEL(pv) == _TRUE)
        {
          // falls popup einer eingabe bereits offen ist muss dessen varlabel
          // weitergegeben (io) werden, ausserdem position beachten !!
          vl = pv;
        }
        pv = P_PopUp->GetImOverloaded();
        if(chk_VARLABEL(pv) == _TRUE)
        {
          // falls popup einer eingabe bereits offen ist und diese überladen
          // wurde, muss overload varlabel weitergegeben (io) werden !!
          ovl = pv;
        }
      }

      P_Window->Init();
      if(P_Window->LoadPopUp(nr, this, x, y, vl, ovl, pmyio) == _TRUE)
      {
        retcode = _TRUE;
        P_Window->IfNecCopySoftkey(&Softkey);

//        _BOOL  shadow = _TRUE;
//        _PopUp *ppop = P_Window->GetP_PopUp();
//        if(ppop != NULL)
//        {
//          if(ppop->GetMenu()->info.frametype >= _THEMEFRAME)
//          {
//            shadow = _FALSE;
//          }
//        }
//        P_Window->SetShadow(shadow);

        P_Window->SetShadow(_TRUE);
//       #ifdef SA34116 
        _PopUp *pup = P_Window->GetP_PopUp(); // SA34116
        if(pup != NULL)
        {
          _MENU *pm = pup->GetMenu();
          if(pm != NULL)
          {
            P_Window->SetShadow(pm->info.draw_shadow);
          }
        }
//       #endif
      }

      if(retcode == _FALSE)
      {
        P_Window->Free();
        Memory2((void**)&P_Window, 0);
      }
    }
  }
  else
  {
    // shift command to next window
    P_Window->AddCmd(CMD_NEWPOPUP, nr, 0, _FALSE);
  }

  return(retcode);
}

_BOOL _Window::CMD_OpenBubbleHelp(void)
{
  _BOOL retcode = _FALSE;

 #ifdef UC_BUBBLE_HELP
  if(P_Picture != NULL)
    retcode = P_Picture->OpenBubbleHelp();
 #endif

  return(retcode);
}

_BOOL _Window::CMD_CloseBubbleHelp(void)
{
  _BOOL retcode = _FALSE;

 #ifdef UC_BUBBLE_HELP
  if(P_Picture != NULL)
  {
    retcode = P_Picture->CloseBubbleHelp();
    if(retcode == _TRUE)
      AddCmd(CMD_REGION, 0, 0, _FALSE);
  }
 #endif

  return(retcode);
}

_BOOL _Window::CMD_CloseEditor(void)
{
  _BOOL retcode = _FALSE;

  if(P_Picture != NULL)
  {
    _REGION actreg = GetActiveRegion();
    ActivateRegion(Region);
    retcode = P_Picture->CloseEditor();
    DeactivateRegion();
    ActivateRegion(actreg);
  }
  else if(P_PopUp != NULL)
  {
    if(valid_VARLABEL(P_PopUp->GetInput()) == _TRUE)
    {
      InsertCmd(CMD_CLOSE, 0, 0, _FALSE);
    }
  }

  return(retcode);
}

_BOOL _Window::CMD_KillEditor(void)
{
  if(CMD_CloseBubbleHelp() == _TRUE)
    FindRoot()->MakeRegion();                 // do complete new regions

  return(CMD_CloseEditor());
}

_BOOL _Window::CMD_SaveEditor(_BOOL lrm)
{
  _BOOL retcode = _FALSE;

  if(P_Picture != NULL)
  {
    ActivateRegion(Region);
    P_Picture->SetLrm(lrm);
    retcode = P_Picture->SaveEditor(_TRUE);
    DeactivateRegion();

   #ifdef USER_DEMAG
    if(WhoAmI.ima == IMA_TOUCHEDITOR)
    {
      if(retcode == _TRUE)
      {
        // dr859: editor wird automatisch geschlossen
        AddCmd(CMD_RETURNEDITOR, 0, 0, lrm);
      }
      else
      {
        // dr859: editor wird bei grenzwertverletzung nicht geschlossen,
        // editor erscheint als wäre er gerade erst geöffnet worden
        AddCmd(CMD_EDITNO, 0, -1, lrm);
      }
    }
   #endif
  }

  return(retcode);
}

#ifdef UC_TOUCH
_UWORD interface_OpenTouchEditor(_EDITOR *pe, _UWORD retcode)
{
 #ifndef CUTOFF_USER
  _FUWORD_PT_UWORD ptr;

  ptr = (_FUWORD_PT_UWORD)find_lasal_method(LseThisPointer, METHOD_LSE_OPENTOUCHEDITOR);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    retcode = ptr(pe, retcode);
  }
 #endif

  return(retcode);
}
#endif

#ifdef UC_TOUCH
_UWORD TouchEditorForMyIO(_EDITOR *pe, _UWORD retcode)
{
 #ifndef CUTOFF_USER
  _FUWORD_PT_UWORD ptr;
  void *tmp;

  tmp = pe->input.io.virtname.pthis;

  ptr = (_FUWORD_PT_UWORD)find_lasal_method(pe->input.io.virtname.pthis, METHOD_MYIO_OPENTOUCHEDITOR);

  if(ptr != NULL)
  {
    load_this(tmp);
    retcode = ptr(pe, retcode);
  }
 #endif

  return(retcode);
}
#endif

#ifdef HID
_BOOL _Window::CMD_OpenServer(_UDWORD serverno, _UDWORD winno)
{
  _BOOL       retcode = _FALSE;
  _LSEVARINFO *pvi;
  _EDITOR     tmpeditor;
  _UWORD      no;
  _VARLABEL   vl;

  ServerToEdit = DEFSCOPE;

  if(P_Picture != NULL)
  {
    if((SetUp.GetUseTouchEditor() == _TRUE)&&(WhoAmI.ima != IMA_TOUCHEDITOR))
    {
      if(VarList.GetVarInfo(&pvi, serverno) == _TRUE)
      {
        init_VARLABEL_Makro(&vl);
        init_EDITOR(&tmpeditor);

        vl.no = 1;
        vl.info[0].state = VAR_VAL;
        vl.info[0].value = serverno;

        if(VarList.GetSystemData(&tmpeditor.result, &tmpeditor.variable, &vl, _TRUE, _TRUE) == _TRUE)
        {
          switch(pvi->vartype)
          {
            case BIT     :
            case FTIME   :
            case FDATE   :
            case BINARY  :
              if(P_Picture->GetMyPicture()->TouchEditorNumeric == _TRUE)
              {
                if(winno == DEFSCOPE)
                {
                 #ifdef UC_TOUCH
                  no = interface_OpenTouchEditor(&tmpeditor, _WIN_EDITOR_NUMERIC);
                 #else
                  no = _WIN_EDITOR_NUMERIC;
                 #endif
                }
                else
                {
                  no = (_UWORD)winno;
                }
                
                if(no != 0xFFFF)
                {
                  ServerToEdit = serverno;
                  AddCmd(CMD_NEWTOUCHEDITOR, no, Softkey.GetActFloor(), _FALSE);
                  retcode = _TRUE;
                }
              }
              break;
            case FSTRING :
              if(P_Picture->GetMyPicture()->TouchEditorAscii == _TRUE)
              {
                if(winno == DEFSCOPE)
                {
                 #ifdef UC_TOUCH
                  no = interface_OpenTouchEditor(&tmpeditor, _WIN_EDITOR_ANUMERIC);
                 #else
                  no = _WIN_EDITOR_ANUMERIC;
                 #endif
                }
                else
                {
                  no = (_UWORD)winno;
                }
                
                if(no != 0xFFFF)
                {
                  ServerToEdit = serverno;
                  AddCmd(CMD_NEWTOUCHEDITOR, no, Softkey.GetActFloor(), _FALSE);
                  retcode = _TRUE;
                }
              }
              break;
          }
        }

        free_EDITOR(&tmpeditor);
      }
    }
  }

  return(retcode);
}
#endif

_BOOL _Window::CMD_OpenEditor(_STACK_LINE *stackline, _BOOL hid, _BOOL lrm, _EVENT *pev)
{
  _BOOL     retcode = _FALSE;
  _RESULT   op1;
  _VARIABLE var;
 #ifdef UC_TOUCH
  _UWORD    no;
  _EVENT    tmpevent;
 #endif

  ServerToEdit = DEFSCOPE;

  if((P_Picture != NULL)&&(VarList.GetSystemData(&op1, &var, &stackline->op1, _FALSE, _TRUE) == _TRUE))
  {
    ActivateRegion(Region);
    retcode = P_Picture->OpenEditor(op1.value, pev);
    DeactivateRegion();

   #ifdef UC_TOUCH
    if((retcode == _TRUE) && (hid == _TRUE))
    {
      _EDITOR *pe = P_Picture->GetEditor();

      if((SetUp.GetUseTouchEditor() == _TRUE)&&(WhoAmI.ima != IMA_TOUCHEDITOR))
      {
        switch(pe->input.io.typ)
        {
          case AS_IPADDRESS :
          case AS_DATE      :
          case AS_TIME      :
          case AS_NUMERIC   :
          case AS_NUMERIC_HIDE :
            if(P_Picture->GetMyPicture()->TouchEditorNumeric == _TRUE)
            {
              if(pe->input.io.virtname.pthis == NULL)
                no = interface_OpenTouchEditor(pe, _WIN_EDITOR_NUMERIC);
              else
                no = TouchEditorForMyIO(pe, 0xFFFF);

              if(no != 0xFFFF)
                AddCmd(CMD_NEWTOUCHEDITOR, no, Softkey.GetActFloor(), _FALSE);
            }
            break;

          case AS_ASCII   :
          case AS_ASCII_HIDE :
            if(P_Picture->GetMyPicture()->TouchEditorAscii == _TRUE)
            {
              if(pe->input.io.virtname.pthis == NULL)
                no = interface_OpenTouchEditor(pe, _WIN_EDITOR_ANUMERIC);
              else
                no = TouchEditorForMyIO(pe, 0xFFFF);

              if(no != 0xFFFF)
                AddCmd(CMD_NEWTOUCHEDITOR, no, Softkey.GetActFloor(), _FALSE);
            }
            break;

          case AS_THEURGE :
            if(pe->input.io.virtname.pthis != NULL)
            {
              no = TouchEditorForMyIO(pe, 0xFFFF);
              if(no != 0xffff)
                AddCmd(CMD_NEWTOUCHEDITOR, no, Softkey.GetActFloor(), _FALSE);
            }
            break;

          default :
            break;
        }
      }

      switch(pe->input.io.typ)
      {
        case AS_RADIO   :
        case AS_CHKBOX  :
                         #ifdef SELECT_AND_CHANGE_CHKBOX
                          // editor erst anwählen und beim nächsten touch ändern.
                          // auf op2 liegt die indexnummer des "alten" editors
                          if((stackline->op2.info[0].value == P_Picture->GetEditor()->actno)||(SetUp.GetDoubleClickOption() == _TRUE))
                         #endif
                          {
                            init_EVENT_Makro(&tmpevent);
                            tmpevent.ftype    = _EVENT_KEYPRESS;
                            tmpevent.scancode = _ENTER;
                            tmpevent.lrm      = lrm;
                            P_Picture->RunEditor(this, &tmpevent, hid);
                           #ifndef SELECT_AND_CHANGE_CHKBOX
                            AddCmd(CMD_KILLEDITOR, 0, 0, _FALSE);
                           #endif
                          }
                          break;
        case AS_ENUMERATION :
        case AS_MENU    : init_EVENT_Makro(&tmpevent);
                          tmpevent.ftype    = _EVENT_KEYPRESS;
                          tmpevent.scancode = _ENTER;
                          P_Picture->RunEditor(this, &tmpevent, hid);
                          break;
        default         : break;
      }
    }

   #endif

  }

  return(retcode);
}

_BOOL _Window::ReloadText(void)
{
  _BOOL retcode = _FALSE;
  _BOOL state = _FALSE;

  if(P_Picture != NULL)
  {
    if(WhoAmI.ima == IMA_WINDOW)
    {
      state = _TRUE;
      if((WhoAmI.no == UC_RESETWINDOW)||(WhoAmI.no == UC_OFFLINEWINDOW))
        state = _FALSE;
    }
    else if(WhoAmI.ima == IMA_PICTURE)
    {
      state = _TRUE;
    }

    if(state == _TRUE)
    {
      P_Picture->Free();
      Softkey.Free();
      retcode = P_Picture->ReloadText(&Softkey, &WhoAmI);
      P_Picture->GetMyPicture()->P_Window = this;
//    Draw(_TRUE);
    }
    else
    {
      // einfach so lassen wie es ist (screensaver, editorwindow, offline-fenster, ...)
      retcode = _TRUE;
    }
  }
  else // if(P_PopUp != NULL) auskommentiert wegen dr1025
  {
    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL _Window::ChangeLanguage(_DWORD language)
{
  _BOOL retcode = _TRUE;
  _Window *pp;
  _REGION reg;
  _UWORD  old, act_language;
  _UDWORD toplevel;
 #ifndef CUTOFF_USER
  _FVOID_UWORD_UWORD ptr;
 #endif

  old          =
  act_language = get_actual_language();

  if(language == -1)
  {
    // nächste sprache
    act_language ++;
  }
  else if (language > Project.GetLanguageNo())
  {
    // sprache wird mittels language_id selektiert
    act_language = Language.GetIndexByID(language);
  }
  else
  {
    // sprache wird direkt als index angegeben
    act_language = language;
  }

  if(act_language >= Project.GetLanguageNo())
    act_language = 0;

  if(old != act_language)
  {
    set_actual_language(act_language);
    SetLseServer(LSE_LANGUAGE, act_language);
    Language.SetTextRenderEngine(get_actual_language());

   #ifndef CUTOFF_USER
    ptr = (_FVOID_UWORD_UWORD)find_lasal_method(LseThisPointer, METHOD_LSE_LANGUAGEWILLCHANGE);

    if(ptr != NULL)
    {
      load_this(LseThisPointer);
      ptr(act_language, old);
    }
   #endif

    toplevel = Softkey.GetActFloor(); // get number of top softkeylevel
    
    Project.ReloadText();           // load language textlist, objlist, menulist

    pp = FindRoot();

    reg = GetActiveRegion();
    ActivateRegion(NULL);   // whole screen

// ScreenStop(); //warten auf toni (arno schmied)

    do
    {
//      ActivateRegion(pp->GetRegion()); // nein, tot fuer durchsichtigen hintergrund

      if(pp->ReloadText() == _TRUE)
      {
        pp->Draw(_TRUE, _FALSE);

        if(pp->P_Background != NULL)
          pp->IfNecCopySoftkey(&pp->P_Background->Softkey);

        pp = pp->P_Window;
      }
      else
      {
        // dieser else Zweig ist notwendig, wenn während der Sprachumschaltung die Berechtigung (Checkbit, Accesslevel, etc..)
        // für einen momentan aktiven Screen/Window erlischt (Kategorie: "böser" Anwender).
        _Window *tmp;

        tmp = pp->P_Background;
        pp->Free();

        if(tmp != NULL)
        {
          Memory2((void**)&tmp->P_Window, 0);
          tmp->P_Window = NULL;
        }
        else
        {
          retcode = _FALSE;
        }
        break;
      }

    }
    while(pp != NULL);

//ScreenStart();
//_ROOM roz;
//GetScreenSize(&roz);
//RefreshRectangle(&roz); // warten auf toni (arno schmied)

    ActivateRegion(reg);
    FindTop()->AddCmd(CMD_OUTSOFTKEY, toplevel, 0, _FALSE); // redraw softkey

   #ifndef CUTOFF_USER
    ptr = (_FVOID_UWORD_UWORD)find_lasal_method(LseThisPointer, METHOD_LSE_LANGUAGEHASCHANGED);

    if(ptr != NULL)
    {
      load_this(LseThisPointer);
      ptr(act_language, old);
    }
   #endif
  }

  // Attention - retcode = _FALSE finish the application !!!
  return(retcode);
}


#ifdef ET_261
 #pragma pack (push, 1)
  typedef struct
  {
    unsigned long ShuttleKeyVersion;
    void (*_SetRepeatKey)(unsigned long pressdelayms0, unsigned long charpersec0);
    void (*_SetShuttle)(unsigned long eventspersecond0, unsigned long releasetimeout0);
    unsigned long (*_SetShuttleEnabled)(unsigned long enabled0);
    long (*_GetShuttleWheel)(void);
    long (*_SetLed)(unsigned long ledcode0, unsigned long op0);
    void (*_Pfeiferl)(unsigned long delayms0);
  }ET261_INTERFACETYPE;

 #pragma pack (pop)
#endif

_BOOL _Window::CMD_OpSystem(_PIC_COMMAND cmd, _IVALUE op1, _IVALUE op2)
{
  _BOOL retcode = _FALSE;
  #ifdef LASAL
   #ifdef ET_261
    ET261_INTERFACETYPE *et261api;
    if(OS_CILGet((char*)"ET261API", (void**)&et261api) == 0)
    {
      switch(cmd)
      {
        case CMD_LED :
          if(et261api->_SetLed != NULL)
          {
            et261api->_SetLed(op1, op2);
            retcode = _TRUE;
          }
          break;

        case CMD_BEEP :
          if(et261api->_Pfeiferl != NULL)
          {
            et261api->_Pfeiferl(op1);
            retcode = _TRUE;
          }
          break;
        default : break;
      }
    }
   #else
    if(RTOSVERSION >= 0x0546)       // ab version 5.70
    {
      switch(cmd)
      {
        case CMD_LED :
          if(OS_IMAE_SetLed != NULL)
          {
            OS_IMAE_SetLed(op1, op2);
            retcode = _TRUE;
          }
          break;
        default : break;
      }
    }
   #endif
  #endif

  return(retcode);
}

_BOOL set_keytable(void *ptr)
{
  // tabelle zum ändern der tastencodes für "xilinx tastatur"
  // aufbau einzelner eintrag:
  // [][].... "xilinx code"
  // [][].... tasten code
  // abschliessend muss am tabellenende immer 0x00000000 stehen

  _BOOL retcode = _FALSE;

  if((ptr != NULL)&&(IKeyPad != NULL)&&(IKeyPad->SetKeyTable != NULL))
  {
    IKeyPad->SetKeyTableCharCode(ptr);
    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL set_keymode(_UDWORD no, _TIP state)
{
  _BOOL  retcode = _FALSE;
//  _UWORD i;

  if((IKeyPad != NULL)&&(IKeyPad->SetKeyMode != NULL))
  {
    if(no == 0xFFFFFFFF)
    {
      IKeyPad->SetKeyMode(0, (_UWORD)state);
//      for(i=0; i<0x3F; i++)
//        IKeyPad->SetKeyMode(i, (_UWORD)state);
    }
    else
    {
      IKeyPad->SetKeyMode(no, (_UWORD)state);
    }

    retcode = _TRUE;
  }

  return(retcode);
}
/*
void _Window::CMD_ReConfigVar(_IVALUE value)
{
  _COMPORT *pcp;
  _UWORD i;
  _ROOM room;
  _CHAR txt[200];

 #ifdef UC_MULTICPU
  Batch.MapAllMultiStation(0); // alle multistationen auf 0 setzen
 #endif

  if(value != 0)
  {
    // bei allen stationen neu online gehen
    for(i=0; i<256; i++)
    {
      pcp = Batch.Get_COMPORT(i);
      if(pcp != NULL)
      {
        pcp->reflist.login = _LOGIN_INIT;
        pcp->crc           = 0;
      }
    }
  }

  ScanBar.SetPos(0);
  MyGetScreenSize(&room);
  OutMultiTextAlign(Batch.MakeOfflineText(txt, _FALSE), 0, &room, 0, T_PAT09|T_MIDBOUND|T_CENTERBOUND|T_COPY|T_SOLID|T_PROP, _DEFFRAME, WHITE, MakeColor_Makro(INVISIBLE, BLACK), MakeColor_Makro(WHITE, RED), sizeof(_CHAR));
 #ifdef UC_ROTATE
  RefreshRectangle(&room);
 #endif

  new_PLC();                          // del present kommunicationbuffer
  Batch.ResetWatchOnline();
  VarList.GetID(_TRUE);                              // get all lasal id
  Alarm.Config();                                 // alarm configuration
  SetLseServer(LSE_OFFLINEMODE, 0);                  // reset Lse Server

  if(WhoAmI.ima == IMA_PICTURE)
    AddCmd(CMD_NEWPICTURE, WhoAmI.no, Softkey.GetActFloor(), _FALSE);
}
*/
_BOOL do_CMD_SET(_STACK_LINE *sl, _BOOL lrm)
{
  _RESULT   op1, op2;
  _VARIABLE var;
  _BOOL     retcode = _FALSE;

  init_RESULT_Makro(&op1);
  init_RESULT_Makro(&op2);

  if((VarList.GetSystemData(&op1, &var, &sl->op1, _TRUE, _TRUE) == _TRUE) &&
     (VarList.GetSystemData(&op2, &var, &sl->op2, _TRUE, _TRUE) == _TRUE))
  {
    op1.value = 0; // wert auf 0 setzen und anschliessend operator + ausführen.
    if(result_addsub(&op1.value, op1.ftype, &op2.value, op2.ftype, '+', &op1.rmin, &op1.rmax) == _TRUE)
    {
      set_lrm(&op1, lrm);
      retcode = VarList.SetSystemData(&op1, &var, &sl->op1, _TRUE);
    }

//    if((op2.value <= op1.rmax) && (op2.value >= op1.rmin))
//    {
//      set_lrm(&op2, lrm);
//      retcode = VarList.SetSystemData(&op2, &var, &sl->op1, _TRUE);
//    }
  }
  return(retcode);
}

/*
void cmd_watch(_STACK_LINE *p)
{
  _File  File;
  _ASCII *pt;
  _UWORD crlf;
  _ASCII txt[50];

  switch(p->cmd)
  {
    case CMD_NOP              : pt = "CMD_NOP"; break; // nothing (0)
    case CMD_START            : pt = "CMD_START"; break; // start with first picture (0)
    case CMD_NEWPICRET        : pt = "CMD_NEWPICRET"; break; // open new (previous) picture (0)
    case CMD_EXIT             : pt = "CMD_EXT"; break; // exit window without backgroundrefresh (0)
    case CMD_CLOSE            : pt = "CMD_CLOSE"; break; // close picture or window (0)
    case CMD_EDITNO           : pt = "CMD_EDITNO"; break; // open editor with input[no] (2)
    case CMD_REDRAW           : pt = "CMD_REDRAW"; break; // redraw picture or window (0)
    case CMD_OUTSOFTKEY       : pt = "CMD_OUTSOFTKEY"; break; // redraw softkey (0)
    case CMD_NEWPICTURE       : pt = "CMD_NEWPICTURE"; break; // open new picture (2)
    case CMD_NEWWINDOW        : pt = "CMD_NEWWINDOW"; break; // open new window (2)
    case CMD_NEWSOFTKEY       : pt = "CMD_NEWSOFTKEY"; break; // change to defined floor and draw (1)
    case CMD_NEWLANGUAGE      : pt = "CMD_NEWLANGUAGE"; break; // change to other language (1)
    case CMD_EDIT             : pt = "CMD_EDIT"; break; // seek input and open editor (1)
    case CMD_KILLEDITOR       : pt = "CMD_KILLEDITOR"; break; // kill editor without save (0)
    case CMD_SAVEEDITOR       : pt = "CMD_SAVEEDITOR"; break; // save editor (0)
    case CMD_RETURN           : pt = "CMD_RETURN"; break; // return to previous (0)
    case CMD_SCANCODE         : pt = "CMD_SCANCODE"; break; // set scancode for keyboard (1)
    case CMD_CALL             : pt = "CMD_CALL"; break; // call individual userfunction (2)
    case CMD_BEEP             : pt = "CMD_BEEP"; break; // signal beep (1)
    case CMD_ADD              : pt = "CMD_ADD"; break; // add value to variable
    case CMD_SUB              : pt = "CMD_SUB"; break; // sub value from variable
    case CMD_INC              : pt = "CMD_INC"; break; // value ++
    case CMD_DEC              : pt = "CMD_DEC"; break; // value --
    case CMD_SET              : pt = "CMD_SET"; break; // set variable
    case CMD_TOGGLE           : pt = "CMD_TOGGLE"; break; // 0-1-0
    case CMD_IFE              : pt = "CMD_IFE"; break; // if ==
    case CMD_IFG              : pt = "CMD_IFG"; break; // if >
    case CMD_IFS              : pt = "CMD_IFS"; break; // if <
    case CMD_ELSE             : pt = "CMD_ELSE"; break; // else
    case CMD_END              : pt = "CMD_END"; break; // end
    case CMD_EXITPROGRAM      : pt = "CMD_EXITPROGRAM"; break; // exit application
    case CMD_NEWPOPUP         : pt = "CMD_NEWPOPUP"; break; // open popupmenu
    case CMD_CALL_ALARM       : pt = "CMD_CALL_ALARM"; break; // open alarmwindow
    case CMD_OVERLOAD         : pt = "CMD_OVERLOAD"; break; // overload (1)
    case CMD_REDRAWTEXT       : pt = "CMD_REDRAWTEXT"; break; // redraw all text and io's
    case CMD_SCRNSCHEME       : pt = "CMD_SCRNSCHEME"; break; // open screen via scheme
    case CMD_DIAGNOSTIC       : pt = "CMD_DIAGNOSTIC"; break; // open diagnostic window (0)
    case CMD_OVERLOAD_OBJ     : pt = "CMD_OVERLOAD_OBJ"; break; // overload just defined object (2)
    case CMD_OVERLOAD_SUMMARY : pt = "CMD_OVERLOAD_SUMMARY"; break; // overload object with defined summary (2)
    case CMD_LED              : pt = "CMD_LED"; break; // switch led on/off/blink (2)
    case CMD_FUNCTIONBLOCK    : pt = "CMD_FUNCTIONBLOCK"; break; // call functionblock (1)
    case CMD_EDITSERVER_HID   : pt = "CMD_EDITSERVER_HID"; break; // call toucheditorwindow to edit spezified server (2)
    case CMD_SCREENSAVE       : pt = "CMD_SCREENSAVE"; break; // open screensaver (0)
    case CMD_EDITNO_HID       : pt = "CMD_EDITNO_HID"; break; // open editor with input[no] and open HID-window (2)
    case CMD_NEXTPICTURE      : pt = "CMD_NEXTPICTURE"; break; // open next picture
    case CMD_USERPOPUP        : pt = "CMD_USERPOPUP"; break; // open userdefined popupmenu
    case CMD_IOPOPUP          : pt = "CMD_IOPOPUP"; break; // open popupmenu for input
    case CMD_REGION           : pt = "CMD_REGION"; break; // create all regions (0)
    case CMD_FIRSTDRAW        : pt = "CMD_FIRSTDRAW"; break; // first time draw picture (0)
    case CMD_RETURN_TO_SIGN   : pt = "CMD_RETURN_TO_SIGN"; break; // return to sign, previous (dumpling) (0)
    case CMD_REEDIT           : pt = "CMD_REEDIT"; break; // open editor for a second time
    case CMD_OPENMINMAX       : pt = "CMD_OPENMINMAX"; break; // open minmaxwindow (bubble-help)
    case CMD_CLOSEMINMAX      : pt = "CMD_CLOSEMINMAX"; break; // close minmaxwindow (bubble-help)
    case CMD_RECALC_IOID      : pt = "CMD_RECALC_IOID"; break; // recalculate inputs in objectlist for editor (1)
    case CMD_RETURNQUEUE      : pt = "CMD_RETURNQUEUE"; break; // return and keep (copy) all commands
    case CMD_AUTOSEEKINPUT    : pt = "CMD_AUTOSEEKINPUT"; break; // automatic seek first input
    case CMD_EDIT_HID         : pt = "CMD_EDIT_HID"; break;
    case CMD_RECONFIGVAR      : pt = "CMD_RECONFIGVAR"; break; // reconfig server + alarm and redraw screen (1)
    case CMD_REFRESHSOFTKEY   : pt = "CMD_REFRESHSOFTKEY"; break; // redraw softkey without option to change floor (0)
    case CMD_ACCESSCHANGED    : pt = "CMD_ACCESSCHANGED"; break; // accesslevel has cahanged (0)
    case CMD_NEWTOUCHEDITOR   : pt = "CMD_NEWTOUCHEDITOR"; break; // open new toucheditorwindow (2)
    case CMD_LOCKHISTORY      : pt = "CMD_LOCKHISTORY"; break; // lock/unlock historyfile (1)
    case CMD_SERVERHELP       : pt = "CMD_SERVERHELP"; break; // open server-helpwindow (1)
    case CMD_IOPOPUP_HID      : pt = "CMD_IOPOPUP_HID"; break; // open popupmenu for input with hid
    case CMD_ENTERPLUS        : pt = "CMD_ENTERPLUS"; break; // do direction on enter (1)
    case CMD_FLIPSCREEN       : pt = "CMD_FLIPSCREEN"; break; // flip screen (2, op1=dx, op2=(yo<<16)|yu)
    case CMD_RETURNEDITOR     : pt = "CMD_RETURNEDITOR"; break; // close just editorwindow (0)
    default                   : pt = "ERROR"; break;
  }

  File.Open(LOADSAVE, "C:\\QUEUE.TXT");
  File.Seek(File.Length(), SEEK_SET);

  DToA(&txt[0], get_abstime(), 0x0106, 1);
  File.Write(txt, StrLen(txt, sizeof(_ASCII)));
  txt[0] = ':';
  File.Write(txt, 1);
  File.Write(pt, StrLen(pt, sizeof(_ASCII)));
  crlf = 0x0D0A;
  File.Write(&crlf, 2);

  File.Close();
}
*/

#ifdef USER_DEKEMA
_BOOL do_cmd_interface(_PIC_COMMAND cmd, _DWORD op1, _DWORD op2)
{
  _BOOL retcode = _TRUE;
  _FBOOL_CMD_DWORD_DWORD_PT ptr;

  ptr = (_FBOOL_CMD_DWORD_DWORD_PT)find_lasal_method(LseThisPointer, METHOD_LSE_DOCMDINTERFACE);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    retcode = ptr(cmd, op1, op2, NULL);
  }

  return(retcode);
}
#endif


void _Window::CMD_FirstDraw(void)
{
 #ifdef UC_MOUSE
  HideMouseCursor();
 #endif
  Draw(_TRUE, _FALSE);
 #ifdef UC_MOUSE
  ShowMouseCursor();
 #endif
}

void _Window::CMD_Region(void)
{
  FindRoot()->MakeRegion();
}

void _Window::CMD_OutSoftkey(_DWORD value)
{
  if(P_Picture != NULL)
  {
    P_Picture->GetMyPicture()->Actual.soft_floor = Softkey.Set(value);
  }
  else
    Softkey.Set(value);

  if(P_Window == NULL)
  {
   #ifdef UC_MOUSE
    HideMouseCursor();
   #endif
    Softkey.Draw();
   #ifdef UC_MOUSE
    ShowMouseCursor();
   #endif
  }
}

_BOOL _Window::OpenComboBox(_UDWORD menunumber, _VARLABEL *vl, _VARLABEL *ovl, _KOORD x, _KOORD y, _BOOL killeditorafterwards, _BOOL lrm, void *pmyio)
{
  _BOOL retcode = _FALSE;
  
  if(CMD_NewPopUp(menunumber, ovl, x, y, vl, pmyio) == _TRUE)
  {
    SetSign(_TRUE);                              // set sign (dumpling)
    if(killeditorafterwards == _FALSE)
      P_Window->SetTouchEditorFlag(_FALSE);      // nach schliessen von fenster soll editor nicht geschlossen werden
    P_Window->AddCmd(CMD_FIRSTDRAW, 0, 0, lrm);  // draw new window
    AddCmd(CMD_REGION, 0, 0, lrm);               // calculate region
    P_Window->AddCmd(CMD_OUTSOFTKEY, Softkey.GetActFloor(), 0, lrm); // draw softkeys
    retcode = _TRUE;
  }
  else
  {
    ::SystemError(CANT_LOAD_POPUP, menunumber);
  }
  
  return retcode;
}

void _Window::FinishCarouselSwim(void)  
{
  if(P_Picture != NULL)
  {
     P_Picture->FinishCarouselSwim(&PicOffset);
  }
}

_RUN_STATE _Window::DoCommands(void)
{
  _RESULT     op1, op2;
  _RUN_STATE  retcode = RUN_NOP;
  _STACK_LINE stack_line;
  _VARIABLE   var;
  _KOORD      x, y;
  _UWORD      erg;
  _UDWORD     shm, timex;
  _WHOAMI     wai;
  _MYSCHEME   myscheme;
  _BOOL       st;
  _LSEVARINFO *pvi;

  while(GetCommand(&stack_line) == _TRUE)
  {
//    cmd_watch(&stack_line);

   SORRY_IMPLIZIT_NECESSARY :
    switch(stack_line.cmd)
    {
      case CMD_EXITPROGRAM :
        if(P_Background != NULL)        // we have to close all windows
        {
          P_Background->InsCmd(stack_line.cmd, &stack_line.op1, &stack_line.op2, stack_line.lrm);
//          AddCmd(CMD_CLOSE, 0, 0);                           // close window
        }
//        else
//        {
          AddCmd(CMD_EXIT, 0, 0, stack_line.lrm);                       // close application
//        }
        break;

      case CMD_CLOSE :
        if(CMD_CloseBubbleHelp() == _TRUE)
          FindRoot()->MakeRegion();                 // do complete new regions
        if(P_Background != NULL)
        {
          P_Background->MoveStackWithoutCompromise(GetStack());
          retcode = RUN_CLOSE;
        }
       #ifdef USER_DEKEMA
        do_cmd_interface(stack_line.cmd, 0, 0);
       #endif
        break;

      case CMD_RECONFIGVAR :
        SoftReset();
  /*
        if(CMD_CloseBubbleHelp() == _TRUE)
          FindRoot()->MakeRegion();                 // do complete new regions
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _FALSE, _TRUE) == _TRUE)
        {
          if(P_Background != NULL)
          {
            P_Background->AddCmd(CMD_RECONFIGVAR, op1.value, 0, stack_line.lrm);
            P_Background->MoveStackWithoutCompromise(GetStack());
            retcode = RUN_CLOSE;
          }
          else
          {
            CMD_ReConfigVar(op1.value);
          }
        }
  */      
        break;

      case CMD_RETURN_TO_SIGN :
        if(P_Window != NULL)
        {
          FindTop()->AddCmd(CMD_RETURN_TO_SIGN, 0, 0, stack_line.lrm);
        }
        else if(Sign == _TRUE)
        {
          SetSign(_FALSE);

          // nur für menu-input damit direction on enter aufgerufen wird
          if((stack_line.op1.info[0].value == _ENTER)&&(P_Picture != NULL))
          {
            if(P_Picture->IsEditorOpen() == _TRUE)
            {
              P_Picture->DirectionOnEnter(_EP_NONE);
//              if(VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE) == _TRUE)
//              {
//                _MY_PICTURE *pmp = P_Picture->GetMyPicture();
//                input_is_saved(&pmp->Editor.input, &op2);
//                P_Picture->DirectionOnEnter(_EP_NONE);
//              }
            }
          }
        }
        else
        {
          if(P_Background != NULL)
          {
            P_Background->AddLine(&stack_line, _FALSE);
            retcode = RUN_CLOSE;
          }
        }
        break;

      case CMD_EXIT :
        retcode = RUN_EXIT;
        break;

      case CMD_SCANCODE :
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _FALSE, _TRUE) == _TRUE)
        {
          Keyboard.AddEvent(_EVENT_KEYPRESS, op1.value);
          Keyboard.AddEvent(_EVENT_KEYRELEASE, op1.value);
        }
        break;

      case CMD_REEDIT :
        if(P_Picture != NULL)
          P_Picture->ReOpenEditor();
        break;

      case CMD_REGION :
        CMD_Region();
        break;

      case CMD_RETURNQUEUE_EX :
        if(P_Background != NULL)
        {
          P_Background->MoveStackWithoutCompromise(GetStack());
          RunCloseExData = stack_line.op1.info[0].value;
          retcode = RUN_CLOSE_EX;
        }
        break;
       
      case CMD_RETURNQUEUE :
        if(P_Window != NULL)
        {
          FindTop()->AddCmd(CMD_RETURNQUEUE, 0, 0, stack_line.lrm);
        }
        else if(P_Background != NULL)
        {
          P_Background->MoveStackWithoutCompromise(GetStack());
          retcode = RUN_CLOSE;
        }
        break;

      case CMD_RETURNEDITOR :
        if(P_Window != NULL)
        {
          FindTop()->AddCmd(CMD_RETURNEDITOR, 0, 0, stack_line.lrm);
        }
        else if(WhoAmI.ima == IMA_TOUCHEDITOR)
        {
          free_STACK_LINE(&stack_line);
          stack_line.cmd = CMD_CLOSE;
          goto SORRY_IMPLIZIT_NECESSARY;
        }
        break;

      case CMD_RETURN :
        SetLseServer(LSE_ACTIVATEBUBBLEHELP, 0);
        if((P_Window != NULL) || (P_Background != NULL))
        {
          if(FindTop() != this)
          {
            FindTop()->AddCmd(CMD_CLOSE, 0, 0, stack_line.lrm);
            FindTop()->MoveStackWithoutCompromise(GetStack());
          }
          else
          {
            free_STACK_LINE(&stack_line);
            stack_line.cmd = CMD_CLOSE;
            goto SORRY_IMPLIZIT_NECESSARY;
          }
        }
        else
        {
          _PICMEMO_SKL *pmskl = PicMemo.Get();
          if(pmskl != NULL)
          {
            free_STACK_LINE(&stack_line);
            stack_line.cmd = CMD_NEWPICRET;
            stack_line.op1.no = 1;
            stack_line.op1.info[0].state = CONST_VAL;
            stack_line.op1.info[0].value = pmskl->no;
            stack_line.op2.no = 1;
            stack_line.op2.info[0].state = CONST_VAL;
            stack_line.op2.info[0].value = pmskl->skl;
                        
            goto SORRY_IMPLIZIT_NECESSARY;
          }
        }
        break;

      case CMD_SCRNSCHEME :
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          init_MYSCHEME_Makro(&myscheme);
          myscheme.schemeno = op1.value;
          if(ScreenScheme.Call(&shm, &myscheme, NULL, 0xFFFF) == _TRUE)
            if(shm != 0xFFFF)
              AddCmd(CMD_NEWPICTURE, shm, 0, stack_line.lrm);
        }
        break;

      case CMD_NEWPICRET :
      case CMD_NEWPICTURE :
        SetLseServer(LSE_ACTIVATEBUBBLEHELP, 0);
        if(P_Background != NULL)               // we have to close all windows
        {
          P_Background->InsCmd(stack_line.cmd, &stack_line.op1, &stack_line.op2, stack_line.lrm);
          P_Background->MoveStackWithoutCompromise(GetStack());
//          if(WhoAmI.ima != IMA_SCREENSAVER)
            retcode = RUN_CLOSE;
//            AddCmd(CMD_EXIT, 0, 0, stack_line.lrm);
        }
        else if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          timex = get_abstime();
         #ifdef NINETEEN
          if((stack_line.cmd == CMD_NEWPICTURE)&&(op1.value == NINETEEN))
          {
            _DOT offx;
            init_DOT(&offx);
            offx.x = 0;
            offx.y = 0;
            DashBoard.Draw(&offx, _FALSE);
            break;
          }
         #endif
         
          if(LoadPicWin(op1.value, IMA_PICTURE, NULL) == _TRUE)
          {
            if(stack_line.cmd == CMD_NEWPICTURE)
            {
             #ifdef USER_DEKEMA
              do_cmd_interface(stack_line.cmd, op1.value, 0);
             #endif
             
              if(VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE) == _FALSE)
                op2.value = 0;
              PicMemo.Add(op1.value, op2.value);    // remember picture-number for return
            }

            new_PLC();                              // init refreshlist
            
            // begin: sa26882
            if(stack_line.cmd == CMD_NEWPICTURE)
            {
              if(valid_VARLABEL(&stack_line.op2) == _TRUE)
              {
                if(VarList.GetVarInfo(&pvi, stack_line.op2.info[0].value) == _TRUE)
                {
                  if(pvi->vartype == OBJECT)
                  {
                    Overload(&stack_line.op2);
                    free_VARLABEL(&stack_line.op2);
                  }
                }
              }
            }
            // end: sa26882
            
            CMD_FirstDraw();  // draw new picture
            //AddCmd(CMD_FIRSTDRAW, 0, 0, stack_line.lrm);                  // draw new picture
            Batch.StartRefreshList();

            if(VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE) == _FALSE)
              op2.value = 0;

            CMD_OutSoftkey(op2.value);
            
            Project.SetTime(op1.value, _PE_PICTURE, get_abstime() - timex);
           
            CMD_Region();
            //AddCmd(CMD_OUTSOFTKEY, op2.value, 0, stack_line.lrm);            // draw softkeys
            //AddCmd(CMD_REGION, 0, 0, stack_line.lrm);
          }
          else
          {
            ::SystemError(CANT_LOAD_PICTURE, op1.value);
          }
        }
        break;

      case CMD_SCREENSAVE :
        SetLseServer(LSE_ACTIVATEBUBBLEHELP, 0);
        if(CMD_ScreenSave() == _TRUE)
        {
          P_Window->AddCmd(CMD_FIRSTDRAW, 0, 0, stack_line.lrm);         // draw new window
          P_Window->AddCmd(CMD_REGION, 0, 0, stack_line.lrm);           // calculate region
        }
        break;

     #ifdef _WIN_SERVERHELP
      case CMD_SERVERHELP :
        if(valid_VARLABEL(&stack_line.op1) == _TRUE)
        {
          FinishCarouselSwim(); // sa53607, DEVVIS-20
          if(CMD_NewWindow(CMD_NEWWINDOW, _WIN_SERVERHELP) == _TRUE)
          {
            P_Window->MoveStackWithoutCompromise(GetStack());  // move rest of stack into window
            P_Window->AddCmd(CMD_FIRSTDRAW, 0, 0, stack_line.lrm);         // draw new window
            P_Window->AddCmd(CMD_REGION, 0, 0, stack_line.lrm);           // calculate region
            P_Window->AddCmd(CMD_OUTSOFTKEY, Softkey.GetActFloor(), 0, stack_line.lrm);   // draw softkeys
            P_Window->Overload(&stack_line.op1);
            P_Window->SetTouchEditorFlag(_FALSE); // nach schliessen von fenster soll editor nicht geschlossen werden
          }
        }
        break;
     #endif

      case CMD_NEWTOUCHEDITOR :
      case CMD_NEWWINDOW :
      case CMD_MESSAGEBOX :
        FinishCarouselSwim(); // sa53607, DEVVIS-20
        SetLseServer(LSE_ACTIVATEBUBBLEHELP, 0);
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          _UDWORD messboxidx = 0xFFFFFFFF;
          if(stack_line.cmd == CMD_MESSAGEBOX)
          {
            if(chk_VARLABEL(&stack_line.op2) == _FALSE)
              break;
              
            messboxidx = *(_UDWORD*)&stack_line.op2.info[0].value;
            free_VARLABEL(&stack_line.op2);
            stack_line.cmd = CMD_NEWWINDOW;
          }
        
          timex = get_abstime();
          if(CMD_NewWindow(stack_line.cmd, op1.value) == _TRUE)
          {
            P_Window->MessBoxIndex = messboxidx;
            
            // begin: sa26882
            if(stack_line.cmd == CMD_NEWWINDOW) 
            {
              if(valid_VARLABEL(&stack_line.op2) == _TRUE)
              {
                if(VarList.GetVarInfo(&pvi, stack_line.op2.info[0].value) == _TRUE)
                {
                  if(pvi->vartype == OBJECT)
                  {
                    P_Window->AddCommand(CMD_OVERLOAD, &stack_line.op2, NULL, stack_line.lrm);  // overload window
                    free_VARLABEL(&stack_line.op2);
                  }
                }
              }
            }
            // end: sa26882
            
           #ifdef USER_DEKEMA
            do_cmd_interface(stack_line.cmd, op1.value, 0);
           #endif
//            P_Window->MoveStackWithoutCompromise(GetStack());  // move rest of stack into window
            P_Window->AddCmd(CMD_FIRSTDRAW, 0, 0, stack_line.lrm);
            P_Window->AddCmd(CMD_REGION, 0, 0, stack_line.lrm);

            op2.value = Softkey.GetActFloor();
            if(chk_VARLABEL(&stack_line.op2) == _TRUE)
              if(VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE) == _FALSE)
                op2.value = Softkey.GetActFloor();

            P_Window->AddCmd(CMD_OUTSOFTKEY, op2.value, 0, stack_line.lrm);
            
            P_Window->DoCommands();
            
            Project.SetTime(op1.value, _PE_WINDOW, get_abstime() - timex);
            
            P_Window->MoveStackWithoutCompromise(GetStack());  // move rest of stack into window
            // stack kopieren gehört ans ende - zwei fenster hintereinander (in einem Durchlauf)
            // funktioniert nicht (regionen)
          }
          else
          {
            ::SystemError(CANT_LOAD_WINDOW, op1.value);
          }
        }
        break;

      case CMD_UNIQUEWINDOW : // close all windows and open new window
        SetLseServer(LSE_ACTIVATEBUBBLEHELP, 0);
        if(FindTop() != this)
        {
          // move stack to top window
          FindTop()->InsCmd(stack_line.cmd, &stack_line.op1, &stack_line.op2, stack_line.lrm);
          FindTop()->MoveStackWithoutCompromise(GetStack());
        }
        else
        {
          if(CMD_CloseBubbleHelp() == _TRUE) // close bubblehelp
            FindRoot()->MakeRegion(); // do complete new regions
            
          if(P_Background != NULL)
          {
            P_Background->MoveStackWithoutCompromise(GetStack());
            P_Background->InsCmd(stack_line.cmd, &stack_line.op1, &stack_line.op2, stack_line.lrm);
            retcode = RUN_CLOSE; // close window
          }
          else
          {
            stack_line.cmd = CMD_NEWWINDOW; // open new window
            goto SORRY_IMPLIZIT_NECESSARY;
          }
        }
        break;

      case CMD_IOPOPUP_HID :
      case CMD_IOPOPUP :
        // attention !! rape (violation) of parameter op1
        // op1 ... popuplistennummer, position x und position y
        // op2 ... varlabel input und varlabel overloaded
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          x = stack_line.op1.info[1].value;
          y = stack_line.op1.info[2].value;

          free_VARLABEL(&stack_line.op1);
          if(stack_line.op2.no <= VL_MAXINDEX)
          {
            // varlabel von overload ermitteln
            if(stack_line.op2.info[VL_MAXINDEX].state == VAR_VAL)
            {
              stack_line.op1.no = 1;
              stack_line.op1.info[0] = stack_line.op2.info[VL_MAXINDEX];

              stack_line.op2.info[VL_MAXINDEX].state = DEF_VAL;
              stack_line.op2.info[VL_MAXINDEX].value = 0;
            }
          }
          
          OpenComboBox(op1.value, &stack_line.op2, &stack_line.op1, x, y, (stack_line.cmd == CMD_IOPOPUP)? _FALSE : _TRUE, stack_line.lrm, NULL);
/*
          if(CMD_NewPopUp(op1.value, &stack_line.op1, x, y, &stack_line.op2) == _TRUE)
          {
            SetSign(_TRUE);                             // set sign (dumpling)
            if(stack_line.cmd == CMD_IOPOPUP)
              P_Window->SetTouchEditorFlag(_FALSE); // nach schliessen von fenster soll editor nicht geschlossen werden
            P_Window->AddCmd(CMD_FIRSTDRAW, 0, 0, stack_line.lrm);          // draw new window
            AddCmd(CMD_REGION, 0, 0, stack_line.lrm);                      // calculate region
            P_Window->AddCmd(CMD_OUTSOFTKEY, Softkey.GetActFloor(), 0, stack_line.lrm); // draw softkeys
          }
          else
          {
            ::SystemError(CANT_LOAD_POPUP, op1.value);
          }
*/          
        }
        break;

      case CMD_NEWPOPUP :
        FinishCarouselSwim(); // sa53607, DEVVIS-20
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          if(CMD_NewPopUp(op1.value, NULL) == _TRUE)
          {
//            AddCmd(CMD_REGION, 0, 0);                      // calculate region
            P_Window->AddCmd(CMD_FIRSTDRAW, 0, 0, stack_line.lrm);          // draw new window
            AddCmd(CMD_REGION, 0, 0, stack_line.lrm);                      // calculate region
            P_Window->AddCmd(CMD_OUTSOFTKEY, Softkey.GetActFloor(), 0, stack_line.lrm); // draw softkeys
          }
          else
          {
            ::SystemError(CANT_LOAD_POPUP, op1.value);
          }
        }
        break;

      case CMD_FIRSTDRAW :
        CMD_FirstDraw();
        break;

      case CMD_REDRAW :
       #ifdef UC_MOUSE
        HideMouseCursor();
       #endif
        Draw(_FALSE, _FALSE);
        if(P_Picture != NULL)
          P_Picture->ShowEditor();
       #ifdef UC_MOUSE
        ShowMouseCursor();
       #endif
        break;

      case CMD_ACCESSCHANGED :
        if(P_Window != NULL)
        {
          FindTop()->AddCmd(CMD_ACCESSCHANGED, 0, 0, stack_line.lrm);
        }
        else
        {
          if(P_Picture != NULL)
            P_Picture->AccessChanged();
          AddCmd(CMD_REFRESHSOFTKEY, 0, 0, stack_line.lrm);
        }
        break;

      case CMD_REFRESHSOFTKEY :
        if(P_Window == NULL)
        {
         #ifdef UC_MOUSE
          HideMouseCursor();
         #endif
          Softkey.Draw();
         #ifdef UC_MOUSE
          ShowMouseCursor();
         #endif
        }
        break;

      case CMD_NEWSOFTKEY :
      case CMD_OUTSOFTKEY :
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          CMD_OutSoftkey(op1.value);
        }
        break;

      case CMD_EDIT :
        if(P_Picture != NULL)
        {
          stack_line.op1.info[0].state = VAR_VAL;
          if(valid_VARLABEL(&stack_line.op1) == _TRUE)
          {
            if(P_Picture->FindInputByVarNo(&erg, stack_line.op1.info[0].value) == _TRUE)
            {
             #ifdef HID
              AddCmd(CMD_EDITNO_HID, erg, -1, stack_line.lrm);
             #else
              AddCmd(CMD_EDITNO, erg, -1, stack_line.lrm);
             #endif
            }
          }
        }
        break;

      case CMD_EDITNO :
       #ifdef USER_BILLION
        if(CMD_SaveEditor(stack_line.lrm) == _TRUE)
       #endif
        {
          if(CMD_CloseBubbleHelp() == _TRUE)
            FindRoot()->MakeRegion();                 // do complete new regions
          CMD_OpenEditor(&stack_line, _FALSE, stack_line.lrm, NULL);
        }
        break;

     #ifdef HID                                   // Für Willkommen Österreich benötigt
      case CMD_EDIT_HID :
        if(P_Picture != NULL)
        {
          stack_line.op1.info[0].state = VAR_VAL;
          if(valid_VARLABEL(&stack_line.op1) == _TRUE)
            if(P_Picture->FindInputByVarNo(&erg, stack_line.op1.info[0].value) == _TRUE)
              AddCmd(CMD_EDITNO_HID, erg, -1, stack_line.lrm);
        }
        break;

      case CMD_EDITNO_HID :
       #ifdef USER_BILLION
        if(CMD_SaveEditor(stack_line.lrm) == _TRUE)
       #endif
        {
          if(CMD_CloseBubbleHelp() == _TRUE)
            FindRoot()->MakeRegion();                 // do complete new regions
          
          _EVENT *pev = NULL;
          if(stack_line.op1.no >= 3)
          {
            // event für aufruf von _MyIo::GetEvent() zusammenbauen
            _EVENT event;
            init_EVENT(&event);
            event.ftype     = _EVENT_HIDPRESS;
            event.dot       = *(_DOT*)&stack_line.op1.info[1].value;
            event.timestamp = stack_line.op1.info[2].value;
            event.lrm       = stack_line.lrm;

            if(stack_line.op2.no >= 3)
            {
              event.multindex = stack_line.op2.info[1].value;
              event.button    = (_HID_BUTTON)stack_line.op2.info[2].value;
            }
            pev = &event;
            
            stack_line.op1.no = 1;
            stack_line.op2.no = 1;
          }
            
          CMD_OpenEditor(&stack_line, _TRUE, stack_line.lrm, pev);
        }
        break;

      case CMD_EDITSERVER_HID_WIN_EDITOR:
        if(VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE) == _TRUE)
        {
          if(CMD_CloseBubbleHelp() == _TRUE)
            FindRoot()->MakeRegion();                 // do complete new regions
          CMD_OpenServer(stack_line.op1.info[0].value, op2.value);
        }
        break;
      
      case CMD_EDITSERVER_HID:
        if(CMD_CloseBubbleHelp() == _TRUE)
          FindRoot()->MakeRegion();                 // do complete new regions
        CMD_OpenServer(stack_line.op1.info[0].value, DEFSCOPE);
        break;
     #endif

     #ifdef UC_BUBBLE_HELP
      case CMD_OPENMINMAX :
        if(CMD_OpenBubbleHelp() == _TRUE)
          FindRoot()->MakeRegion();                 // do complete new regions
        break;

      case CMD_CLOSEMINMAX :
        CMD_CloseBubbleHelp();
        break;
     #endif

      case CMD_KILLEDITOR :
        CMD_KillEditor();
        break;

      case CMD_AUTOSEEKINPUT :
        if(P_Picture != NULL)
         P_Picture->AutomaticSeekInput(_FALSE);
        break;

      case CMD_SAVEEDITOR :
        if(CMD_CloseBubbleHelp() == _TRUE)
          FindRoot()->MakeRegion();                 // do complete new regions
        CMD_SaveEditor(stack_line.lrm);
        break;

      case CMD_CALL :
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          st = _FALSE;
          if(valid_VARLABEL(&stack_line.op2) == _TRUE)
          {
            if(VarList.GetVarInfo(&pvi, stack_line.op2.info[0].value) == _TRUE)
            {
              if(pvi->vartype == OBJECT)
              {
                op2.value = stack_line.op2.info[0].value;
                st        = _TRUE;
              }
              else
              {
                st = VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE);
              }
            }
          }
          else
          {
            st = VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE);
          }

          if(st == _TRUE)
          {
            ActivateRegion(Region);
            UserCall(op1.value, op2.value);
            DeactivateRegion();
          }
        }
        break;

      case CMD_FUNCTIONBLOCK :
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
#ifdef UC_OVERLOAD_FCB_SUB        
          if(chk_VARLABEL(&stack_line.op2) == _TRUE)
          {
            if(stack_line.op2.info[0].state == VAR_VAL)
            {
              _Stack *pSrcStk = InterBoxList.Get(op1.value);
              if(pSrcStk != NULL)
              {
                _Stack TmpStk;
                TmpStk.Init();
                TmpStk.AddStack(pSrcStk, _FALSE); // stack kopieren
                TmpStk.Overload(&stack_line.op2); // kopie überladen
                InsertStack(&TmpStk, stack_line.lrm); // uberladene kopie zur bearbeitung übernehmen
                TmpStk.Free(); // kopie freigeben
              }
              break;
            }
          }
#endif // UC_OVERLOAD_FCB_SUB        
          InsertStack(InterBoxList.Get(op1.value), stack_line.lrm);
        }
        break;

      case CMD_ADD :
        if((VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE) &&
           (VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE) == _TRUE))
        {
          if(result_addsub(&op1.value, op1.ftype, &op2.value, op2.ftype, '+', &op1.rmin, &op1.rmax) == _TRUE)
          {
            set_lrm(&op1, stack_line.lrm);
            VarList.SetSystemData(&op1, &var, &stack_line.op1, _TRUE);
          }
        }
        break;

      case CMD_SUB :
        if((VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE) &&
           (VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE) == _TRUE))
        {
          if(result_addsub(&op1.value, op1.ftype, &op2.value, op2.ftype, '-', &op1.rmin, &op1.rmax) == _TRUE)
          {
            set_lrm(&op1, stack_line.lrm);
            VarList.SetSystemData(&op1, &var, &stack_line.op1, _TRUE);
          }
        }
        break;

      case CMD_INC :
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          op2.value = 1;
          if(result_addsub(&op1.value, op1.ftype, &op2.value, RES_NUMERIC, '+', &op1.rmin, &op1.rmax) == _TRUE)
          {
            set_lrm(&op1, stack_line.lrm);
            VarList.SetSystemData(&op1, &var, &stack_line.op1, _TRUE);
          }
        }
        break;

      case CMD_DEC :
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          op2.value = 1;
          if(result_addsub(&op1.value, op1.ftype, &op2.value, RES_NUMERIC, '-', &op1.rmin, &op1.rmax) == _TRUE)
          {
            set_lrm(&op1, stack_line.lrm);
            VarList.SetSystemData(&op1, &var, &stack_line.op1, _TRUE);
          }
        }
        break;

      case CMD_TOGGLE :
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          op1.value = (op1.value == 0)? 1 : 0;
          if((op1.value <= op1.rmax) && (op1.value >= op1.rmin))
          {
            set_lrm(&op1, stack_line.lrm);
            VarList.SetSystemData(&op1, &var, &stack_line.op1, _TRUE);
          }
        }
        break;


      case CMD_SET :
        do_CMD_SET(&stack_line, stack_line.lrm);
//        if((VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE) &&
//           (VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE) == _TRUE))
//        {
//          if((op2.value <= op1.rmax) && (op2.value >= op1.rmin))
//            VarList.SetSystemData(&op2, &var, &stack_line.op1, _TRUE);
//        }
        break;

      case CMD_LED:
        if((VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE) &&
           (VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE) == _TRUE))
        {
          CMD_OpSystem(stack_line.cmd, op1.value, op2.value);
        }
        break;

      case CMD_BEEP :
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          CMD_OpSystem(stack_line.cmd, op1.value, 0);
        }
        break;

      case CMD_NEWLANGUAGE:
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          if(op1.value != get_actual_language())
          {
            CMD_CloseBubbleHelp();
            CMD_CloseEditor();
            if(ChangeLanguage(op1.value) == _FALSE)
              retcode = RUN_CLOSE;
          }
        }
        break;

      case CMD_NEXTPICTURE: // open next picture
        wai = FindRoot()->GetWhoAmI();
        AddCmd(CMD_NEWPICTURE, Project.GetNextPictureNo(wai.no), 0, stack_line.lrm);
        break;

      case CMD_OVERLOAD:
        Overload(&stack_line.op1);
//        if(P_Picture != NULL)
//          P_Picture->Overload(&stack_line.op1);
        break;

      case CMD_OVERLOAD_OBJ:
        if(P_Picture != NULL)
          P_Picture->ChangeOverload(&stack_line.op1, &stack_line.op2);
        break;

      case CMD_OVERLOAD_SUMMARY:
        if(P_Picture != NULL)
        {
          if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
            P_Picture->SummaryOverload(op1.value, &stack_line.op2);
        }
        break;

      case CMD_REDRAWTEXT:
        if(P_Picture != NULL)
          P_Picture->ReDraw();
        break;

      case CMD_ENTERPLUS:
        if(P_Picture != NULL)
        {
          if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
          {
            P_Picture->DirectionOnEnter((_ENTER_PLUS)op1.value);
          }
        }
        break;

      case CMD_DIAGNOSTIC:
        AddCmd(CMD_NEWPOPUP, UC_PRJ_POPUP, 0, stack_line.lrm);
        break;

      case CMD_FLIPSCREEN:
        if(P_Background != NULL)
        {
          P_Background->InsCmd(stack_line.cmd, &stack_line.op1, &stack_line.op2, stack_line.lrm);
        }
        else
        {
          if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
          {
            if(VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE) == _TRUE)
            {
              CMD_FlipScreen(op1.value, (_KOORD)((((_UDWORD)op2.value)>>16)&0xFFFF), (_KOORD)(((_UWORD)op2.value)&0xFFFF), stack_line.lrm);
            }
          }
        }
        break;

      case CMD_RECALC_IOID:
//        if(P_Background != NULL)
//          P_Background->InsCmd(stack_line.cmd, &stack_line.op1, &stack_line.op2);
        if(P_Picture != NULL)
        {
          if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
          {
            if(VarList.GetSystemData(&op2, &var, &stack_line.op2, _TRUE, _TRUE) == _TRUE)
            {
              lookup_SEEKELEMENT(P_Picture->GetMyPicture(), op1.value, op2.value);
            }
          }

//        {
//          if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE) == _TRUE)
//            new_IO_SEEKELEMENT(P_Picture->GetMyPicture(), (_UWORD)op1.value);
//        }
        }
        break;

      case CMD_LOCKHISTORY:
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          TermAlarm.LockHistory((op1.value == 0)? _FALSE : _TRUE);
        }
        break;

      case CMD_SWITCH_MULTICPU:
       #ifdef UC_MULTICPU
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          CMD_KillEditor(); // editor muss geschlossen werden
          Batch.SwitchMultiStation((_UWORD)op1.value&0xFF, ((_UWORD)op1.value>>8)&0xFF, _TRUE);
         #ifdef FIRSTINPUT_ALT 
          AddCmd(CMD_AUTOSEEKINPUT, 0, 0, stack_line.lrm);
         #endif // FIRSTINPUT_ALT
        }
       #endif
        break;

      case CMD_ACKNOWLEDGEFLAG:
        if(VarList.GetSystemData(&op1, &var, &stack_line.op1, _TRUE, _TRUE) == _TRUE)
        {
          AcknowledgeFlag(op1.value);
        }
        break;

      case CMD_RESET_SCREENSAVER:
        reset_screensaver();
        break;
 
      default : break;
    }
    free_STACK_LINE(&stack_line);
  }

  return(retcode);
}

_BOOL _Window::GetSize(_ROOM *pr)
{
  if(P_Picture != NULL)
  {
    P_Picture->GetSize(pr, &PicOffset);
    return _TRUE;
  }
  else if(P_PopUp != NULL)
  {
    P_PopUp->GetSize(pr);
    return _TRUE;
  }

  return _FALSE;
}

_RUN_STATE _Window::RunIntern(void)
{
  _EVENT     event;
  _Softkey   *Ps;
 #ifdef UC_TOUCH
  _BOOL      te;
//  _IMA       wima;
  _WHOAMI    wai;
 #endif

  if(P_Window != NULL)                                  // at first do window
  {
    _RUN_STATE runstate = P_Window->RunIntern();
    switch(runstate)
    {
      case RUN_NOP   :                                       // nothing to do
        break;
        
      case RUN_CLOSE_EX :
      case RUN_CLOSE :                 // close window and restore background
        Keyboard.KillEvent();
        Keyboard.HideEventDueWindowClosed = 1;
        Ps = P_Window->GetSoftkey();
        if(Ps != NULL)
        {
          // softkeyebene von window übernehmen (nur wenn im window softkeykopie von bild)
          if(Ps->GetCopyOfPrevious() == _TRUE)
            Softkey.Set(Ps->GetActFloor());
        }

       #ifdef UC_TOUCH
        te  = P_Window->GetTouchEditorFlag();
        wai = P_Window->GetWhoAmI();
       #endif

        P_Window->EndIntern(_TRUE);

        P_Window->Free();
        Memory2((void**)&P_Window, 0);
        Keyboard.SetHidPrePress();
        AddCmd(CMD_REGION, 0, 0, _FALSE);
        
        if(runstate == RUN_CLOSE)
        {  
         #ifdef UC_TOUCH
          if((SetUp.GetUseTouchEditor() == _TRUE)&&(te == _TRUE))
          {
            // 'te' ist bei serverhelp und menueingabe mit taste <enter> geöffnet auf _FALSE und bedeutet,
            // dass eingabefokus nicht verschwinden soll...

            if(SetUp.GetAutomaticSeekFirstInput() == _TRUE)
            {
              AddCmd(CMD_REEDIT, 0, 0, _FALSE);
            }
            else if((wai.ima == IMA_TOUCHEDITOR)||(wai.ima == IMA_POPUP))
            {
              // editor wird nur mehr in diesen 2 fällen geschlossen (sa8154), früher immer
              if(MenuList.IsDictionaryMenue(&wai) == _FALSE)
              {
                AddCmd(CMD_KILLEDITOR, 0, 0, _FALSE);
              }
            }
          }
          else
         #endif
          {
            AddCmd(CMD_REEDIT, 0, 0, _FALSE);
          }
        }
       
        if((LookUpCommand(CMD_NEWSOFTKEY) == _FALSE)&&(LookUpCommand(CMD_OUTSOFTKEY) == _FALSE))
        {
          // wenn noch kein softkeykommando abgesetzt wurde dann folgendes kommando eintragen.
          // problem ansonsten: window_start und window_ready wurden bereits aufgerufen und der
          // user könnte in diesen schnittstellen bereits eine softkeyebene definiert haben.
          AddCmd(CMD_NEWSOFTKEY, Softkey.GetActFloor(), 0, _FALSE);
        }
        
        if(runstate == RUN_CLOSE_EX)
        {
          if(P_Picture != NULL)
          {
            _EDITOR *pedi = P_Picture->GetEditor();
            if(pedi != NULL)
            {
              pedi->disable_direction_enter = 1; 
      
              if(RunCloseExData != DEFSCOPE)
              {
                // es muss der selbe editor wieder mit dem entsprechenden server geöffnet werden
                InsertCmd(CMD_EDITSERVER_HID_WIN_EDITOR, (RunCloseExData & 0xFFFF), (RunCloseExData >> 16) & 0xFFFF, _FALSE);
                RunCloseExData = DEFSCOPE;
              }
              else if(P_Picture->IsEditorTrue(pedi->actno) == _TRUE)
              {
                // editor für aktuelle eingabe wieder öffnen
                _DOT dot;
                dot.x = (pedi->input.io.space.xy1.x + pedi->input.io.space.xy2.x) / 2;
                dot.y = (pedi->input.io.space.xy1.y + pedi->input.io.space.xy2.y) / 2;
        
                _VARLABEL vl1, vl2;
                init_VARLABEL(&vl1);
                init_VARLABEL(&vl2);
        
                vl1.no = 3;
                vl1.info[0].value = pedi->actno;
                vl1.info[0].state = CONST_VAL;
        
                vl1.info[1].value = *(_IVALUE*)&dot;
                vl1.info[1].state = CONST_VAL;
        
                vl1.info[2].value = (_IVALUE)get_abstime();
                vl1.info[2].state = CONST_VAL;
        
                InsCmd(CMD_EDITNO_HID, &vl1, &vl2, _FALSE);
              }
            }
          }
        }
        
//        AddCmd(CMD_OUTSOFTKEY, Softkey.GetActFloor(), 0, _FALSE);
        return(DoCommands());

      case RUN_EXIT  :              // close window without backgroundrestore
        Keyboard.KillEvent();
        P_Window->EndIntern(_FALSE);
        P_Window->Free();
        Memory2((void**)&P_Window, 0);
        AddCmd(CMD_REGION, 0, 0, _FALSE);
        break;
    }
  }

  if(IsReady == _TRUE)
  {
    event.ftype = _EVENT_NONE;
    
    if(GlobalChangeSys != PRINTSCREEN_IN_USE)
    {    
      GetEvent(&event);                               // catch event from queue

     #ifdef _CHANGELOOKUP
      if((P_Window == NULL) && (event.ftype == _EVENT_NONE))
      {
        // nur bei top-window und wenn kein event vorhanden ist wird GlobalChangeIrq berücksichtigt
        _UDWORD memo = GlobalChangeIrq;
        GlobalChangeSys = (memo != GlobalChangeIrqMemo)? 1 : 0;
        GlobalChangeIrqMemo = memo;
      }
     #endif
    }

    ActivateRegion(Region);

    if(P_Window == NULL)
      Project.Run(&event);                           // project run interface

    Run(&event);                                            // interface: run

    if(P_Picture != NULL)
    {
      P_Picture->PictureRunIntern(this, &event, &PicOffset);    // do picture
      if(P_Window == NULL)
        P_Picture->Cursor.Run();
      else
        P_Picture->Cursor.Hide();
    }
    else if(P_PopUp != NULL)
    {
      P_PopUp->Run(&event);                                       // do popup
      if(P_Window == NULL)
        P_PopUp->Cursor.Run();
      else
        P_PopUp->Cursor.Hide();
    }

    DeactivateRegion();

  #ifdef HID
   #ifdef UC_HIDCLOSEWINDOW
    _ROOM room;
    if((event.ftype == _EVENT_HIDPRESS)&&(P_Background != NULL))
    {
     #ifdef NINETEEN
      _ROOM scr;
      MyGetScreenSize(&scr);
      if((event.dot.x >= scr.xy1.x) && (event.dot.x <= scr.xy2.x) &&
         (event.dot.y >= scr.xy1.y) && (event.dot.y <= scr.xy2.y))
     #endif
      {
        GetSize(&room);
        if((event.dot.x < room.xy1.x) || (event.dot.x > room.xy2.x) ||
            (event.dot.y < room.xy1.y) || (event.dot.y > room.xy2.y))
        {
          if(MessBoxIndex == 0xFFFFFFFF) // nur wenn es keine messagebox ist !!
          {
            // hid out of window
            Beep(BEEP_ESC);
            found_something_by_hid(&event, FOUND_ESC);
            AddCmd(CMD_CLOSE, 1, 1, event.lrm);
          }
        }
      }
    }
   #endif
  #endif

   #ifdef NINETEEN
    _ROOM nscr;
    _BOOL nst = _FALSE;
    if(event.ftype == _EVENT_NONE)
    {
      nst = _TRUE;
    }
    else if((event.ftype == _EVENT_HIDPRESS)||(event.ftype == _EVENT_HIDMOVE)||(event.ftype == _EVENT_HIDRELEASE))
    {
      MyGetScreenSize(&nscr);
      if(event.dot.y > nscr.xy2.y)
        nst = _TRUE;
    }

    if(nst == _TRUE)
    {
      DashBoard.PictureRunIntern(this, &event, &PicOffset);
    }
   #endif

    if(event.ftype == _EVENT_KEYPRESS)
    {
      if(event.scancode == _ESC)
      {
        _BOOL done = _FALSE;
        if(P_Picture != NULL)
        {
          if(P_Picture->IsEditorOpen() == _TRUE)
          {
            AddCmd(CMD_KILLEDITOR, 0, 0, event.lrm);
           #ifdef FIRSTINPUT_ALT 
            AddCmd(CMD_AUTOSEEKINPUT, 0, 0, event.lrm);
           #endif // FIRSTINPUT_ALT
            done = _TRUE;
          }
        }

        if(done == _FALSE)
        {
          if(P_Background != NULL)
          {
            AddCmd(CMD_RETURN, 1, 1, event.lrm);
          }
        }
      }
    }

    DoSoftkeyPress(&event);                        // if nec. do softkeypress

   #ifdef UC_DYN_SOFTKEYS
    if(P_Window == NULL)
      Softkey.Refresh(_FALSE);
   #endif

   #ifdef UC_MOVING_WINDOW
    if((event.ftype == _EVENT_HIDPRESS) && (event.multindex == 0) && (event.already_used == 0))
    {
      if(((WhoAmI.ima == IMA_WINDOW) || (WhoAmI.ima == IMA_TOUCHEDITOR)) && (P_Picture != NULL))
      {
        _MY_PICTURE *ppic = P_Picture->GetMyPicture();
        WindowMoveStart(this, &event, &ppic->Room);
      }
    }
   #endif // UC_MOVING_WINDOW

  }

  return DoCommands();                        // do all collected commands
}

_UDWORD _Window::GetMessBoxIndex(void)
{
  return MessBoxIndex;
}

_UDWORD _Window::GetEditServerNo(void)
{
  return EditServerNo;
}

void _Window::RefreshJustTopWindow(_BOOL justtop)
{

  if(P_Window != NULL)
  {
    if((justtop == _TRUE) && (IsReady == _TRUE))
      IsReady = _FALSE;
    else
      IsReady = _TRUE;

    P_Window->RefreshJustTopWindow(justtop);
  }

}

_UDWORD _Window::GetCarouselRoom(_ROOM *p)
{
  if(P_Window != NULL)
    return P_Window->GetCarouselRoom(p);
  
  if(P_Picture != NULL)
    return P_Picture->GetCarouselRoom(p);
    
  return 0;
}

_BOOL _Window::IsThereAnyCarousel(void)
{
  if(P_Window != NULL)
    return P_Window->IsThereAnyCarousel();
  
  if(P_Picture != NULL)
    return P_Picture->IsThereAnyCarousel();
  
  return _FALSE;
}

/*
void _Window::EndInternSub(void)
{
  if(P_Picture != NULL)
  {
    P_Picture->End();
  }
  else if(P_PopUp != NULL)
  {
    P_PopUp->End(); 
  }
}

void _Window::EndIntern(_BOOL redraw)
{
  _REGION act_region, region;

  if(P_Window != NULL)
  {
    if(redraw == _TRUE)
    {
      StartRegion();
      AddRegion_MOTION(P_Window->GetRegion());
      region = EndRegion();

      act_region = OrRegion(P_Window->GetRegion(), region);
      ActivateRegion(act_region);
//      ActivateRegion(P_Window->GetRegion());
      DrawBackground();
      DeactivateRegion();
      FreeRegion(&region);
      FreeRegion(&act_region);
    }

    if(P_Picture != NULL)
      P_Picture->ReOpenEditor(&PicOffset);

    P_Window->EndInternSub();

    P_Window->Free();
    Memory2((void**)&P_Window, 0);
  }
}
*/

_Window *_Window::SetP_Window(_Window *p)
{
  _Window *retcode = P_Window;
  P_Window = p;
  return(retcode);
}

void _Window::ReDrawBackground(_BOOL redraw)
{
  if(redraw == _TRUE)
  {
   #ifdef THR
    // THR: See if closing a Window, if so then
    // add on the offset.
    _ROOM       troom;
    _MY_PICTURE *pic;

    pic = P_Picture->GetMyPicture();
    if(pic != NULL)
    {
      // Calc the Window region
      // (Cannot use AddRegion_MOTION: when I do this I do not get
      //  the correct area with the offset redrawn!)
      //AddRegion_MOTION(GetRegion());
      StartRegion();
      troom = pic->Room;
      troom.xy1.x += PicOffset.x;
      troom.xy1.y += PicOffset.y;
      troom.xy2.x += PicOffset.x;
      troom.xy2.y += PicOffset.y;

      // Add the main region
      AddRegion(&troom);

      // Calc and add the shadow region
      GetShadowRoom(&troom);
      AddRegion(&troom);
      _REGION thr_region = EndRegion();

    // Set the region and redraw the background!
      ActivateRegion(thr_region);
      DrawBackground(&thr_region);
      DeactivateRegion();
      FreeRegion(&thr_region);
    }
    else
   #endif
    {
     #ifndef UC_REDRAW_ALL
      StartRegion();
      AddRegion_MOTION(GetRegion());
      _REGION region = EndRegion();

      _REGION act_region = OrRegion(GetRegion(), region);
      ActivateRegion(act_region);

      if(P_Background != NULL)
      {
        // calculate region without top-window
        _Window *pb = P_Background->SetP_Window(NULL);
        Root.MakeRegion();
        P_Background->SetP_Window(pb);
      }

      DrawBackground(&act_region);
      DeactivateRegion();
      FreeRegion(&region);
      FreeRegion(&act_region);
     #else

      ScreenStop();
      DrawBackground(NULL);
      ScreenStart();
     #endif
    }
  }
}

void _Window::EndIntern(_BOOL redraw)
{
  if(P_Window != NULL)
  {
    P_Window->EndIntern(redraw);
    P_Window->Free();
    Memory2((void**)&P_Window, 0);
  }

  if(WhoAmI.ima == IMA_SCREENSAVER)
  {
    CallScreensaverEnd(NULL);
  }

  if(P_Picture != NULL)
  {
    P_Picture->EndIntern((P_Background != NULL)? P_Background : this);

    P_Picture->CloseEditor();
    ReDrawBackground(redraw);
//    P_Picture->ReOpenEditor(&PicOffset);
    P_Picture->Free();
    Memory2((void**)&P_Picture, 0);
  }
  else if(P_PopUp != NULL)
  {
    P_PopUp->End();
    ReDrawBackground(redraw);
    P_PopUp->Free();
    Memory2((void**)&P_PopUp, 0);
  }

  End();

  if(P_Background != NULL)
    P_Background->MoveStackWithoutCompromise(GetStack()); // alle cmd welche in End-Interface eingetragen wurden an darunterliegende _Window weiterleiten

  IsReady = _FALSE;
}

void _Window::StartIntern(_BOOL firsttime)
{
  _WHOAMI   *pwai = &WhoAmI;
  _Picture  *PPic;
  _EDITOR   edit;
  _VARLABEL vl;
 #ifdef UC_DYN_EDIT_POSITION
  _ROOM     screen, place;
 #endif

  EditServerNo = DEFSCOPE;

  if(firsttime == _TRUE)
  { 
    if(pwai->ima == IMA_TOUCHEDITOR)
    {
      if(SetUp.GetUseTouchEditor() == _TRUE)
      {
        if(ServerToEdit != DEFSCOPE)
        {
          Start(firsttime);
          
          init_VARLABEL(&vl);
          vl.no            = 1;
          vl.info[0].value = ServerToEdit;
          vl.info[0].state = VAR_VAL;
          EditServerNo = ServerToEdit;
          
          // eingabe, vartexte, etc. überladen
          if(P_Picture->Overload(&vl, _TRUE) == _TRUE)
          {
            AddCmd(CMD_EDITNO, 0, -1, _FALSE);
          }
          ServerToEdit = DEFSCOPE;
        }
        else
        {
          PPic = P_Background->GetP_Picture();
          
#ifdef SA32402 
          if(PPic != NULL)
          {
            edit = *PPic->GetEditor();
            if(PPic->IsEditorTrue(edit.actno) == _TRUE)
            {
              _VARLABEL *ptvl = &edit.input.io.variable;
              if(chk_VARLABEL_Makro(ptvl))
              {
                ServerToEdit = ptvl->info[0].value; // bei KeyPadNum::S_Start wird die globale variable ServerToEdit benötigt
              }
            }
          }
#endif

          Start(firsttime);
          
#ifdef SA32402 
          ServerToEdit = DEFSCOPE;
#endif          
          if(PPic != NULL)
          {
#ifndef SA32402 
            edit = *PPic->GetEditor();
#endif
            if(PPic->IsEditorTrue(edit.actno) == _TRUE)
            {
              PPic = GetP_Picture();
              if((PPic != NULL)&&(P_Picture != NULL))
              {
                if(PPic->CopyEditor(&edit, 0) == _TRUE)
                {
                 #ifdef UC_DYN_EDIT_POSITION
                  // berechne position für touch-editorfenster
                  GetPictureSize(&screen); // grösse bildschirm ermitteln + softkeys berücksichtigen
                 #ifdef ZOOM_KM
                  screen.xy2.y -= SOFTKEY_NEG_OFF_X;
                 #endif
                  place = PPic->GetMyPicture()->Room;
                  if(find_ROOM(&place, &edit.input.io.space, &screen, Shadow) == _TRUE)
                    move_MY_PICTURE(PPic->GetMyPicture(), place.xy1.x, place.xy1.y, T_LEFTBOUND|T_UPBOUND);
                 #endif
                  // eingabe, vartexte, etc. überladen
                  if(P_Picture->Overload(&edit.input.io.variable) == _TRUE)
                  {  

                   #ifndef CUTOFF_USER
                    _FVOID_PT_PT_PT ptr;
                    _PICTUREMASK    *pm;

                    if(P_Picture->GetMyPicture()->Ptr != NULL)
                    {
                      pm = &P_Picture->GetMyPicture()->Ptr[0].GetPICTURE()->mask;

                      ptr = (_FVOID_PT_PT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_OVERLOADEDITOR);
                      if(ptr != NULL)
                      {
                        load_this(LseThisPointer);
                        ptr(pm, pwai, &edit.input.io.variable);
                      }
                    }
                   #endif

                    AddCmd(CMD_EDITNO, 0, -1, _FALSE);
                  }
                }
              }
            }
          }
        }
        return; // weil funktion Start() bereits aufgerufen wurde
      }
    }
   #ifdef HID
    else if(pwai->ima == IMA_POPUP)
    {
      if(pwai->no >= 60000)
      {
        if(P_PopUp != NULL)
        {
          // bei diagnostic in scrollbar zusätzlich PgUp und PgDown anzeigen
          P_PopUp->GetMenu()->calcul.level.pguppgdown = _TRUE;
        }
      }
    }
   #endif
   
  }

  Start(firsttime);
}

void _Window::ReadyIntern(_BOOL firsttime)
{
  if((firsttime == _TRUE) && (P_Background != NULL))
    P_Background->DoAllNegEdge(_FALSE);

  Ready(firsttime);

  IsReady = _TRUE;
}

_FRAMETYPE _Window::GetBackgroundTheme(_ROOM *proom)
{
  _MESSAGEBOX *pbox = MessageBox.Get(MessBoxIndex);

  if(pbox != NULL)
    return pbox->FrameType;

 #ifdef UC_BUBBLE_HELP
  if((WhoAmI.ima == IMA_WINDOW) && (WhoAmI.no == UC_NEWBUBBLEWINDOW))
    return NewBubbleHelp.GetFrame();
 #endif

  if(P_Picture != NULL)
    return P_Picture->GetBackgroundTheme(proom);
    
  return _DEFFRAME;
}

void _Window::Draw(_BOOL firsttime, _BOOL DrawPrint)
{
  _Window *tmp = P_Window;
  
  P_Window = NULL;

 #ifdef _STATICCHKBIT
  Sequencer.InitStatic();
 #endif

  StartIntern(firsttime);

  if(P_Picture != NULL)
  {
   #ifdef UC_MOVING_WINDOW 
    if(firsttime == _TRUE)
    {
      long dx, dy;
      if(WindowMoveGetOffset(&dx, &dy, WhoAmI.ima, WhoAmI.no) == _TRUE)
      {
        if((dx != 0) || (dy != 0))
        {
          add_MY_PICTURE(P_Picture->GetMyPicture(), dx, dy);
        }
      }
    }
   #endif // UC_MOVING_WINDOW 
   
//    P_Picture->StartIntern(firsttime);
    P_Picture->Draw(&PicOffset, DrawPrint);
    P_Picture->ShowEditor();
    P_Picture->ReadyIntern(firsttime);
//   #ifdef NINETEEN  // dashboard jedesmal beim bildaufbau ausgeben
//    if(P_Background == NULL) // is nit mehr notwendig
//    {
//      _DOT soffset;
//      init_DOT(&soffset);
//      soffset.x = 0;
//      soffset.y = 0;
//     DashBoard.Draw(&soffset, _FALSE);
//    }
//   #endif
  }
  else if(P_PopUp != NULL)
  {
    P_PopUp->StartIntern(firsttime);
    P_PopUp->Draw();
    P_PopUp->ReadyIntern(firsttime);
  }

  ReadyIntern(firsttime);
  P_Window = tmp; 
}

void _Window::MakeRegion(void)
{
  if(P_Window != NULL)
    P_Window->MakeRegion();

  FreeRegion(&Region);

  if(P_Picture != NULL)
  {
    StartRegion();
    P_Picture->PictureAddRegion(&PicOffset);
    Region = EndRegion();
    P_Picture->SubEditorRegion(&Region);
    SubtractRegion(&Region);
  }
  else if(P_PopUp != NULL)
  {
    StartRegion();
    P_PopUp->PopUpAddRegion();
    Region = EndRegion();
    SubtractRegion(&Region);
  }
}

void _Window::AddRegion_MOTION(_REGION referenz)
{
  if(P_Background != NULL)
    P_Background->AddRegion_MOTION(referenz);

  if(P_Picture != NULL)
    P_Picture->AddRegion_MOTION(referenz);
}


void _Window::DoSoftkeyPress(_EVENT *pe)
{
  if((pe->ftype == _EVENT_KEYPRESS)||(pe->ftype == _EVENT_KEYRELEASE))
  {
    switch(pe->scancode)
    {
      case _F1  : Softkey.Do( 0, this, pe); break;
      case _F2  : Softkey.Do( 1, this, pe); break;
      case _F3  : Softkey.Do( 2, this, pe); break;
      case _F4  : Softkey.Do( 3, this, pe); break;
      case _F5  : Softkey.Do( 4, this, pe); break;
      case _F6  : Softkey.Do( 5, this, pe); break;
      case _F7  : Softkey.Do( 6, this, pe); break;
      case _F8  : Softkey.Do( 7, this, pe); break;
      case _F9  : Softkey.Do( 8, this, pe); break;
      case _F10 : Softkey.Do( 9, this, pe); break;
      case _F11 : Softkey.Do(10, this, pe); break;
      case _F12 : Softkey.Do(11, this, pe); break;
    }

    if(P_Picture != NULL)
      P_Picture->DoHotkey(pe);
  }
}

void _Window::DoAllNegEdge(_BOOL redraw)
{
  Softkey.DoAllNegEdge(redraw);

// #ifdef HID 
  if(P_Picture != NULL)
    P_Picture->DoAllNegEdge(redraw);
// #endif
}

void _Window::KillBackGroundImage(void)
{
  if(P_Window != NULL)
    P_Window->KillBackGroundImage();

  if(P_Picture != NULL)
    P_Picture->KillBackGroundImage();
}

void _Window::RedrawScreenSub(void)
{
  Draw(_FALSE, _FALSE);

  if(P_Window != NULL)
    P_Window->RedrawScreenSub();
}

void _Window::RedrawScreen(_ROOM *pr)
{
  _REGION actreg, newreg;
  _ROOM   tmp;

  if(pr == NULL)
  {
    MyGetScreenSize(&tmp);
    pr = &tmp;
    FindRoot()->KillBackGroundImage();
  }

 #ifdef UC_MOUSE
  _UWORD hide = IfNecHideMouseCursor(pr);
 #endif

  actreg = GetActiveRegion();
  DeactivateRegion();

  StartRegion();
  AddRegion(pr);
  newreg = EndRegion();
  ActivateRegion(newreg);

  FindRoot()->RedrawScreenSub();

  FreeRegion(&newreg);
  ActivateRegion(actreg);

 #ifdef UC_MOUSE
  IfNecShowMouseCursor(hide);
 #endif

}

void _Window::Overload(_VARLABEL *pvl)
{
  if(P_Picture != NULL)
    P_Picture->Overload(pvl);
}

void *_Window::GetContentPointer(void)
{
  if(WhoAmI.ima == IMA_POPUP)
  {
    if(P_PopUp != NULL)
      return P_PopUp->GetMenu();
  }
  else if(P_Picture != NULL)
  {
    return P_Picture->GetMyPicture();
  }
  
  return NULL;
}

_Softkey* _Window::GetSoftkey(void)
{
  return(&Softkey);
}
_WHOAMI _Window::GetWhoAmI(void)                               
{ 
  return(WhoAmI);
}
_REGION _Window::GetRegion(void)                               
{ 
  return(Region); 
}
_PopUp* _Window::GetP_PopUp(void)                            
{ 
  return(P_PopUp); 
}
_Picture* _Window::GetP_Picture(void)
{
  return(P_Picture);
}
_Window* _Window::GetP_Window(void)
{
  return(P_Window);
}
_Window* _Window::GetP_Background(void)
{
  return(P_Background);
}
_BOOL _Window::GetShadow(void)
{
  return(Shadow);
}
void _Window::SetShadow(_BOOL st)
{
  Shadow = st;
}
void _Window::SetSign(_BOOL st)
{
  Sign = st;
}

void _Window::IAm(_IMA t, _UDWORD no)
{
  WhoAmI.ima = t;
  WhoAmI.no  = no;
}

_DOT _Window::GetPicOffset(void)
{
  return(PicOffset);
}

void _Window::SetPicOffset(_DOT dot)
{
  PicOffset = dot;
}

_Cursor *_Window::GetCursor(void)
{
  _Cursor *retcode = NULL;

  if(P_Picture != NULL)
    retcode = &P_Picture->Cursor;
  else if(P_PopUp != NULL)
    retcode = &P_PopUp->Cursor;

  return(retcode);
}

_BOOL _Window::GetTouchEditorFlag(void)
{
  return(TouchEditorFlag);
}

void _Window::SetTouchEditorFlag(_BOOL st)
{
  TouchEditorFlag = st;
}

void _Window::DoFlipScreen(void)
{
  if(FlipScreen.dx != 0)
  {
    if((WhoAmI.ima == IMA_PICTURE)&&(P_Picture != NULL))
    {
      flip_MY_PICTURE(P_Picture->GetMyPicture(), FlipScreen.dx, FlipScreen.yo, FlipScreen.yu);
    }
  }
}

void _Window::CMD_FlipScreen(_KOORD dx, _KOORD yo, _KOORD yu, _BOOL lrm)
{

  if((dx != FlipScreen.dx)||(yo != FlipScreen.yo)||(FlipScreen.yu != yu))
  {
    FlipScreen.dx = dx;
    FlipScreen.yo = yo;
    FlipScreen.yu = yu;

    IpcSettings.Save();

    if(WhoAmI.ima == IMA_PICTURE)
      InsertCmd(CMD_NEWPICTURE, WhoAmI.no, GetSoftkey()->GetActFloor(), lrm);
  }
}

void _Window::DoStart(void)
{

  _STACK *ps, tmp_stack;

  ps        = GetStack();
  tmp_stack = *ps;
  init_STACK_Makro(ps);
  Free();
  AddCmd(CMD_NEWPICTURE, Batch.GetStartPicture(), 0, _FALSE);
  DoCommands();
  free_STACK(ps);
  *ps = tmp_stack;
}

void do_start(void)
{
  Root.DoStart();
}


/*
_EDITOR *_Window::GetEditor(void)
{
  _EDITOR *retcode = NULL;

  if(P_Picture != NULL)
  {
    if(P_Picture->IsEditorOpen() == _TRUE)
  {
    retcode = &P_Picture->GetMyPicture()->Editor;
  }
  }

  return(retcode);
}
_EDITOR *_Window::CallInterface_If_InputIsSaved_sub(_Window *Pw, _UDWORD varno)
{
  _EDITOR *retcode = NULL;

  if(Pw != NULL)
  {
    retcode = Pw->GetEditor();

    if(retcode != NULL)
  {
    if(retcode->variable.no != varno)
      retcode = NULL;
  }

  if((retcode == NULL)&&(P_Background != NULL))
      retcode = CallInterface_If_InputIsSaved_sub(P_Background, varno);
  }

  return(retcode);
}
_BOOL _Window::CallInterface_If_InputIsSaved(_UDWORD varno, _RESULT *pr)
{
  _BOOL   retcode = _FALSE;
  _EDITOR *pe;

  pe = CallInterface_If_InputIsSaved_sub(this, varno);

  if(pe != NULL)
  {
    input_is_saved(&pe->input, pr);
    retcode = _TRUE;
  }

  return(retcode);
}
*/

_BOOL _Window::DrawPicWin(_IMA ima, _UWORD no, _UWORD run_no, _DOT *poff)
{
  _BOOL       retcode = _FALSE;
  _ASCII      dpne[_MAX_PATHLENGTH];
  _UWORD      i;
  _MY_PICTURE *pmp;

  dpne[0] = 0;

  if(ima == IMA_PICTURE)
  {
   #ifdef USER_DEMAG // SA51914
    if(Project.GetPictureAccess2(dpne, &no, _FALSE, NULL, _TRUE) == _TRUE)
   #else
    if(Project.GetPictureAccess(dpne, &no, _FALSE, NULL) == _TRUE)
   #endif 
    {
      if(Memory2((void**)&P_Picture, sizeof(_Picture)) != 0)
      {
        P_Picture->Init();
        if(P_Picture->LoadPicture(dpne, no, this, _FALSE, no) == _TRUE)
        {
          IAm(IMA_PICTURE, no);
          retcode = _TRUE;
        }
      }
    }
    else
    {
      SystemError(NO_ACCESS, no, NULL);
    }
  }
  else if((ima == IMA_WINDOW)||(ima == IMA_TOUCHEDITOR))
  {
    if(Project.GetWindowAccess(dpne, &no, _FALSE, NULL) == _TRUE)
    {
      if(Memory2((void**)&P_Picture, sizeof(_Picture)) != 0)
      {
        P_Picture->Init();
        if(P_Picture->LoadWindow(dpne, no, this) == _TRUE)
        {
          SetShadow(P_Picture->GetMyPicture()->DrawShadow);
          IAm(ima, no);
          retcode = _TRUE;
        }
      }
    }
    else
    {
      SystemError(NO_ACCESS, no, NULL);
    }
  }

  if(retcode == _TRUE)
  {
   #ifdef UC_MOUSE
    HideMouseCursor();
   #endif

    if(poff != NULL)
    {
      if((poff->x != 0)||(poff->y != 0))
      {
        if(P_Picture != NULL)
        {
          pmp = P_Picture->GetMyPicture();
          if(pmp != NULL)
          {
            move_MY_PICTURE(pmp, poff->x, poff->y, T_LEFTBOUND | T_UPBOUND);
          }
        }
      }
    }
    _UDWORD memo = GlobalChangeSys;
    GlobalChangeSys = PRINTSCREEN_IN_USE;
    
    MakeRegion();
    Draw(_TRUE, _TRUE);

    P_Picture->ShowEditor();
    Softkey.Draw();

    DisableEvents = _TRUE;

    for(i=0; i<run_no; i++)
    {
      GlobalChangeSys = PRINTSCREEN_IN_USE;
      RunIntern();
    }
    DisableEvents = _FALSE;
    GlobalChangeSys = memo;

   #ifdef UC_MOUSE
    ShowMouseCursor();
   #endif
  }

  return(retcode);
}

extern "C" _BOOL InsertCmd(_PIC_COMMAND cmd, _DWORD op1, _DWORD op2)
{
  return(Root.FindTop()->InsertCmd(cmd, op1, op2, _FALSE));           // add command
}

_BOOL AddCmd(_PIC_COMMAND cmd, _VARLABEL *op1, _VARLABEL *op2)
{
  return(Root.FindTop()->AddCommand(cmd, op1, op2, _FALSE));
}

_BOOL SaveEditor(void)
{
  return(Root.FindTop()->CMD_SaveEditor(_FALSE));
}

_BOOL CloseEditor(void)
{
  return(Root.FindTop()->CMD_CloseEditor());
}

void picmemo_free(void)
{
  PicMemo.Free();
}

_UWORD PicMemo_GetNo(void)
{
  return(PicMemo.GetNo());
}

_UWORD PicMemo_Get(void)
{
  _PICMEMO_SKL *p = PicMemo.Get();
  if(p != NULL)
    return(p->no);
  return(0);
}

void RefreshJustTopWindow(_BOOL justtop)
{
  Root.RefreshJustTopWindow(justtop);
}

_BOOL DrawPicWinOffset(_IMA ima, _UWORD no, _UWORD run_no, _DOT *poff)
{
  _BOOL   retcode;
  _Window tmp;
  _REGION region;

  region = GetActiveRegion();
  DeactivateRegion();

  tmp = Root;
  Root.Init();

  // festlegung: stättner, koller
  // keine _EVENTS auslesen
  // keine TipFunktionen setzen
  // Editor wird nur initialisiert, nicht geladen und kein FirstInput berücksichtigt
  // DrawBackgroung wird auf _FALSE gesetzt

  retcode = Root.DrawPicWin(ima, no, run_no, poff);
  Root.Free();
  Root = tmp;

  ActivateRegion(region);

  return(retcode);
}

_BOOL DrawPicWin(_IMA ima, _UWORD no, _UWORD run_no)
{
  _DOT offset;

  init_DOT(&offset);
  offset.x = 0;
  offset.y = 0;
  return(DrawPicWinOffset(ima, no, run_no, &offset));
}

