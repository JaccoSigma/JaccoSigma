#include "T_IPC.H"
#include "T_NUC.H"

void _SetUp::Init(void)
{
  init_EDITCOLOR(&ActiveEditColor);
  init_EDITCOLOR(&ChangeEditColor);
  init_EDITCOLOR(&SelectGroupColor);
  init_BUBBLEINFO(&BubbleInfo);
  init_CURSORCOLOR(&SelectCursorColor);
  init_CURSORCOLOR(&ActiveCursorColor);
  init_SCREENSAVER(&ScrSaver);
  init_SOFTKEYSETUP(&SoftkeySetup);
  init_DEFKEYPAD(&DefKeyPad);

  TextFrameDistance       = 0;
  KeepObjectOffset        = _FALSE;
  ZoomImageList           = _TRUE;
  DeleteBackground        = _TRUE;
  BackgroundColor         = MakeColor_Makro(BLACK, BLACK);
  AutomaticSeekFirstInput = _FALSE;
  DeleteEventBuffer       = _FALSE;
  DirectionOnEnter        = _EP_NONE;
 #ifdef UC_TOUCH
  UseTouchEditor          = _TRUE;
 #else
  UseTouchEditor          = _FALSE;
 #endif
  init_DATA(&TargetNotice);
  EditorIncDec            = _TRUE;
  HotkeysInMenu           = _FALSE;
  UseShuttleToEdit        = _FALSE;
  UseHostName             = _FALSE;
  WordWrap                = _FALSE;
  DisableAltEsc           = _FALSE;
  DisplayPassword         = _FALSE;
  DoubleClickOption       = _FALSE;
  StartupTimeLseProject   = 0;
  CheckBoxImage[0]        = DEFSCOPE;
  CheckBoxImage[1]        = DEFSCOPE;
  RadioBoxImage[0]        = DEFSCOPE;
  RadioBoxImage[1]        = DEFSCOPE;
  PossiblyLocalTextInUse  = _TRUE; // sa38992
}

void _SetUp::Free(void)
{
  free_EDITCOLOR(&ActiveEditColor);
  free_EDITCOLOR(&ChangeEditColor);
  free_EDITCOLOR(&SelectGroupColor);
  free_BUBBLEINFO(&BubbleInfo);
  free_CURSORCOLOR(&SelectCursorColor);
  free_CURSORCOLOR(&ActiveCursorColor);
  free_SCREENSAVER(&ScrSaver);
  free_SOFTKEYSETUP(&SoftkeySetup);
  free_DATA(&TargetNotice);
  free_DEFKEYPAD(&DefKeyPad);

  Init();
}

_UWORD _SetUp::GetNo(void)
{
  _UWORD retcode = 6;

 #if COMPILER_VERSION >= 22
  retcode += 1;
 #endif
 #if COMPILER_VERSION >= 24
  retcode += 3;
 #endif
 #if COMPILER_VERSION >= 27
  retcode += 1;
 #endif
 #if COMPILER_VERSION >= 28
  retcode += 1;
 #endif
 #if COMPILER_VERSION >= 31
  retcode += 9;
 #endif

  return(retcode);
}

_BOOL _SetUp::Read(_File *File)
{
  _BOOL   retcode = _TRUE;
  _UBYTE  tmp =  0;
 #if COMPILER_VERSION >= 31
  _UWORD no;
 #endif

  Init();

  if(File->GetError() == _FALSE)
  {
    File->Read_BUBBLEINFO(&BubbleInfo);
    File->Read_EDITCOLOR(&ActiveEditColor);
    File->Read_EDITCOLOR(&ChangeEditColor);
    File->Read_EDITCOLOR(&SelectGroupColor);
    File->Read(&TextFrameDistance, sizeof(_UWORD));
    File->Read(&tmp, sizeof(_UBYTE)); KeepObjectOffset = (tmp == 0)? _FALSE : _TRUE;
    File->Read(&tmp, sizeof(_UBYTE)); ZoomImageList    = (tmp == 0)? _FALSE : _TRUE;
    File->Read(&tmp, sizeof(_UBYTE)); DeleteBackground = (tmp == 0)? _FALSE : _TRUE;
    File->Read_COLOR(&BackgroundColor);
    File->Read_CURSORCOLOR(&SelectCursorColor);
    File->Read_CURSORCOLOR(&ActiveCursorColor);

   #if COMPILER_VERSION >= 22
    File->Read(&ColorsNo, sizeof(_UDWORD));           // no of colors
    File->Read(&SoftkeySetup.height, sizeof(_UBYTE));   // softkeyheight
   #endif
   #if COMPILER_VERSION >= 24
    File->Read(&tmp, sizeof(_UBYTE)); DeleteEventBuffer = (tmp == 0)? _FALSE : _TRUE;
    File->Read(&tmp, sizeof(_UBYTE)); AutomaticSeekFirstInput = (tmp == 0)? _FALSE : _TRUE;
    File->Read_SCREENSAVER(&ScrSaver);
   #endif
   #if COMPILER_VERSION >= 27
    DirectionOnEnter = _EP_NONE;
    File->Read(&tmp, sizeof(_UBYTE));

    switch(tmp)
    {
      case 0 : DirectionOnEnter = _EP_LEFT;  break;
      case 1 : DirectionOnEnter = _EP_RIGHT; break;
      case 2 : DirectionOnEnter = _EP_UP;    break;
      case 3 : DirectionOnEnter = _EP_DOWN;  break;
    }
   #endif
   #if COMPILER_VERSION >= 28
    File->Read(&SoftkeySetup.width, sizeof(_UBYTE));   // softkeywidth
    File->Read(&SoftkeySetup.no,    sizeof(_UBYTE));   // softkeyanzahl
    File->Read(&tmp, sizeof(_UBYTE));   // direction
    SoftkeySetup.horizontal = (tmp == 0)? _TRUE : _FALSE;
    SoftkeySetup.align      = 0;
    File->Read(&tmp, sizeof(_UBYTE));   // horizontal align
    switch(tmp)
    {
      case  0 : SoftkeySetup.align |= T_LEFTBOUND;   break;
      case  1 : SoftkeySetup.align |= T_CENTERBOUND; break;
      default : SoftkeySetup.align |= T_RIGHTBOUND;  break;
    }
    File->Read(&tmp, sizeof(_UBYTE));   // vertical align
    switch(tmp)
    {
      case  0 : SoftkeySetup.align |= T_UPBOUND;   break;
      case  1 : SoftkeySetup.align |= T_MIDBOUND;  break;
      default : SoftkeySetup.align |= T_DOWNBOUND; break;
    }
   #endif

   #if COMPILER_VERSION >= 31
    File->Read(&tmp, sizeof(_UBYTE)); UseTouchEditor = (tmp == 0)? _FALSE : _TRUE;

    File->Read(&no, sizeof(_UWORD));

   #if COMPILER_VERSION >= 40
    // Im LSE wurde immer fix Länge 0 geschrieben - und kein Text
    if(no > 0)
    {
      if(Memory2((void**)&TargetNotice.ptr, no) != 0)
      {
        TargetNotice.no = no;
        File->Read(TargetNotice.ptr, no);
      }
    }
   #endif

    File->Read(&tmp, sizeof(_UBYTE));

    EditorIncDec = (tmp & 0x0001)? _TRUE : _FALSE;

    HotkeysInMenu = (tmp & 0x0002)? _TRUE : _FALSE;

    UseShuttleToEdit = (tmp & 0x0004)? _TRUE : _FALSE;

    UseHostName = (tmp & 0x0008)? _TRUE : _FALSE;

    WordWrap = (tmp & 0x0010)? _TRUE : _FALSE;

    DisableAltEsc = (tmp & 0x0020)? _TRUE : _FALSE;

    DisplayPassword = (tmp & 0x0040)? _TRUE : _FALSE;
    #ifdef USER_HTC
      DisplayPassword = _TRUE;
    #endif

    #ifndef USER_KM // km will das nicht
//    DoubleClickOption = (tmp & 0x0080)? _TRUE : _FALSE; // noch nicht in LSE integriert
    #endif

   #endif

   #if COMPILER_VERSION >= 58
    _UWORD dtmp;
    File->Read(&dtmp, 2);
    DefKeyPad.ButtonColorReleased = dtmp;
    File->Read(&dtmp, 2);
    DefKeyPad.ButtonColorPressed = dtmp;
    File->Read(&dtmp, 2);
    DefKeyPad.ButtonColorText = dtmp;
    File->Read(&dtmp, 2);
    DefKeyPad.TitleColorText = dtmp;
    File->Read(&dtmp, 2);
    DefKeyPad.LimitColorText = dtmp;
    File->Read(&dtmp, 2);
    DefKeyPad.WindowColorBack = dtmp;
    File->Read(&dtmp, 2);
    DefKeyPad.ButtonFrame = (_FRAMETYPE)dtmp;
    File->Read(&dtmp, 2);
    DefKeyPad.WindowFrame = (_FRAMETYPE)dtmp;
    File->Read(&dtmp, 2);
    DefKeyPad.StandardFontSchemeNo = dtmp;
    File->Read(&dtmp, 2);
    DefKeyPad.SmallFontSchemeNo = dtmp;
    
    File->Read(&dtmp, 2); // reserve1
    DefKeyPad.TitleBarColor = dtmp; // sa32226
    
    File->Read(&tmp, 1);
    PossiblyLocalTextInUse = (tmp & 0x01)? _TRUE : _FALSE; // sa38992
    
    File->Read(&tmp,  1); // reserve2 mit 0xFF initialisiert
    File->Read(&dtmp, 2); // reserve3 mit 0xFFFF initialisiert
    File->Read(&dtmp, 2); // reserve4 mit 0xFFFF initialisiert
   #endif

  }

  if(File->GetError() == _TRUE)
  {
    Free();
    retcode = _FALSE;
  }

  return(retcode);
}

_BOOL _SetUp::Load(_ASCII *dpne)
{
  _File File;
  _BOOL retcode = _FALSE;

  File.Open(LOAD, dpne);
  if(File.ReadHeader() == _TRUE)
    retcode = Read(&File);

  File.Close();

  return(retcode);
}

// ***

_COLOR _SetUp::IfNecChangeColor(_COLOR editcol, _COLOR iocol)
{
  _COLOR p, b;

  p = GetPenColor_Makro(editcol);
  if(p == INVISIBLE)
    p = GetPenColor_Makro(iocol);
  b = GetBackColor_Makro(editcol);
  if(b == INVISIBLE)
    b = GetBackColor_Makro(iocol);

  return(MakeColor_Makro(b, p));
}

_UWORD _SetUp::GetFrameSpace(void)
{
  return(TextFrameDistance);
}

_COLOR _SetUp::GetBackgroundColor(void)
{
  return(BackgroundColor);
}

_BOOL _SetUp::GetDeleteBackground(void)
{
  return(DeleteBackground);
}

_BOOL _SetUp::GetPossiblyLocalTextInUse(void)
{
  return PossiblyLocalTextInUse; // sa38992
}

_EDITCOLOR *_SetUp::Get_EDITCOLOR(_EDITSTATE state)
{

  switch(state)
  {
    case _EDITACTIVE  : return(&ActiveEditColor);
    case _EDITCHANGE  : return(&ChangeEditColor);
    case _EDITPASSIVE : break;
    case _SELECTGROUP : break;
  }

  return(&SelectGroupColor);
}

_COLOR _SetUp::GetEditColorBack(_EDITSTATE state, _COLOR iocol)
{
  _UDWORD    colhi, collo;
  _EDITCOLOR *p = Get_EDITCOLOR(state);
  _COLOR     retcode;

  retcode = p->colback;

  if(ColorScheme.Call(&colhi, &p->dyncolback, GetBackColor_Makro(p->colback)) == _TRUE)
  {
    if(ColorScheme.Call(&collo, &p->dyncolfront, GetPenColor_Makro(p->colback)) == _TRUE)
    {
      retcode = MakeColor_Makro((_UWORD)colhi, (_UWORD)collo);
    }
  }

  return(IfNecChangeColor(retcode, iocol));
}

_COLOR _SetUp::GetEditColorFrame(_EDITSTATE state, _COLOR iocol)
{
  _UDWORD    colhi, collo;
  _EDITCOLOR *p = Get_EDITCOLOR(state);
  _COLOR     retcode;

  retcode = p->colframe;

  if(ColorScheme.Call(&colhi, &p->dyncoltl, GetBackColor_Makro(p->colframe)) == _TRUE)
  {
    if(ColorScheme.Call(&collo, &p->dyncolbr, GetPenColor_Makro(p->colframe)) == _TRUE)
    {
      retcode = MakeColor_Makro((_UWORD)colhi, (_UWORD)collo);
    }
  }

  return(IfNecChangeColor(retcode, iocol));
}

_COLOR _SetUp::GetEditColorTxt(_EDITSTATE state, _COLOR iocol)
{
  _UDWORD    col;
  _EDITCOLOR *p = Get_EDITCOLOR(state);
  _COLOR     retcode;

  retcode = p->coltxt;

  if(ColorScheme.Call(&col, &p->dyncoltxt, GetBackColor_Makro(p->coltxt)) == _TRUE)
  {
    retcode = MakeColor_Makro((_UWORD)col, (_UWORD)col);
  }

  return(IfNecChangeColor(retcode, iocol));
}

_COLOR _SetUp::GetEditColorCursor(_EDITSTATE state)
{
  _UDWORD col;
  _COLOR  retcode;

  if(state == _EDITACTIVE)
  {
    retcode = SelectCursorColor.color;
    if(ColorScheme.Call(&col, &SelectCursorColor.dyncol, retcode) == _TRUE)
    {
      retcode = MakeColor_Makro((_UWORD)col, (_UWORD)col);
    }
  }
  else if(state == _EDITCHANGE)
  {
    retcode = ActiveCursorColor.color;
    if(ColorScheme.Call(&col, &ActiveCursorColor.dyncol, retcode) == _TRUE)
    {
      retcode = MakeColor_Makro((_UWORD)col, (_UWORD)col);
    }
  }

  return(retcode);
}

#ifdef UC_BUBBLE_HELP
_BUBBLEINFO *_SetUp::Get_BUBBLEINFO(void)
{
  return(&BubbleInfo);
}

_COLOR _SetUp::GetBubbleColorBack(void)
{
  _UDWORD    colhi, collo;
  _COLOR     retcode;
  _EDITCOLOR *p = &BubbleInfo.editcolor;

  retcode = p->colback;

  if(ColorScheme.Call(&colhi, &p->dyncolback, GetBackColor_Makro(p->colback)) == _TRUE)
  {
    if(ColorScheme.Call(&collo, &p->dyncolfront, GetPenColor_Makro(p->colback)) == _TRUE)
    {
      retcode = MakeColor_Makro((_UWORD)colhi, (_UWORD)collo);
    }
  }

  return(retcode);
}

_COLOR _SetUp::GetBubbleColorFrame(void)
{
  _UDWORD    colhi, collo;
  _COLOR     retcode;
  _EDITCOLOR *p = &BubbleInfo.editcolor;

  retcode = p->colframe;

  if(ColorScheme.Call(&colhi, &p->dyncoltl, GetBackColor_Makro(p->colframe)) == _TRUE)
  {
    if(ColorScheme.Call(&collo, &p->dyncolbr, GetPenColor_Makro(p->colframe)) == _TRUE)
    {
      retcode = MakeColor_Makro((_UWORD)colhi, (_UWORD)collo);
    }
  }

  return(retcode);
}

_COLOR _SetUp::GetBubbleColorTxt(void)
{
  _UDWORD    col;
  _COLOR     retcode;
  _EDITCOLOR *p = &BubbleInfo.editcolor;

  retcode = p->coltxt;

  if(ColorScheme.Call(&col, &p->dyncoltxt, GetBackColor_Makro(p->coltxt)) == _TRUE)
  {
    retcode = MakeColor_Makro((_UWORD)col, (_UWORD)col);
  }

  return(retcode);
}

void _SetUp::Set_BUBBLEINFO_delaytime(_DWORD delaytime)
{

  BubbleInfo.delaytime = delaytime;
}

void _SetUp::Set_BUBBLEINFO_remaintime(_DWORD remaintime)
{

  BubbleInfo.remaintime = remaintime;
}
#endif

_SCREENSAVER *_SetUp::Get_SCREENSAVER(void)
{
  return(&ScrSaver);
}

_BOOL _SetUp::GetAutomaticSeekFirstInput(void)
{
  return(AutomaticSeekFirstInput);
}

_BOOL _SetUp::GetDeleteEventBuffer(void)
{
  return(DeleteEventBuffer);
}

_UWORD _SetUp::GetSoftkeyHeight(void)
{
  return(SoftkeySetup.height);
}

_SOFTKEYSETUP *_SetUp::Get_SOFTKEYSETUP(void)
{
  return(&SoftkeySetup); 
}

_DEFKEYPAD *_SetUp::Get_DEFKEYPAD(void)
{
  _ASCII *label;

  if(DefKeyPad.StandardFontSchemeName[0] == 0)
  {
    StrCpy(DefKeyPad.StandardFontSchemeName, sizeof(_ASCII), "_@Standard", sizeof(_ASCII));
    label = FontScheme.GetLabelByIndex(DefKeyPad.StandardFontSchemeNo);
    if(label != NULL)
    {
      if(label[0] != 0)
      {
        if(StrLen(label, sizeof(_ASCII)) < sizeof(DefKeyPad.StandardFontSchemeName))
        {
          StrCpy(DefKeyPad.StandardFontSchemeName, sizeof(_ASCII), label, sizeof(_ASCII));
        }
      }
    }
  }

  if(DefKeyPad.SmallFontSchemeName[0] == 0)
  {
    StrCpy(DefKeyPad.SmallFontSchemeName, sizeof(_ASCII), "_@Default", sizeof(_ASCII));
    label = FontScheme.GetLabelByIndex(DefKeyPad.SmallFontSchemeNo);
    if(label != NULL)
    {
      if(label[0] != 0)
      {
        if(StrLen(label, sizeof(_ASCII)) < sizeof(DefKeyPad.SmallFontSchemeName))
        {
          StrCpy(DefKeyPad.SmallFontSchemeName, sizeof(_ASCII), label, sizeof(_ASCII));
        }
      }
    }
  }
  
  return &DefKeyPad;
}


_ENTER_PLUS _SetUp::GetDirectionOnEnter(void)
{
  _ENTER_PLUS retcode = _EP_NONE;

 // vorsicht: bei verwendung von shuttle kein direction on enter, weil taste enter die
 // eigentliche eingabe einleitet
  if(GetUseShuttleForEdit() == _FALSE)
  {
    if(get_directiononenter() == _TRUE)
      retcode = DirectionOnEnter;
  }

  return(retcode);
}

_BOOL _SetUp:: GetKeepObjectOffset(void)
{
  return(KeepObjectOffset);
}

_BOOL _SetUp::GetZoomImageList(void)
{
  return(ZoomImageList);
}

_UDWORD _SetUp::GetColorsNo(void)
{
  return(ColorsNo);
}

_BOOL _SetUp::GetUseTouchEditor(void)
{
  return(UseTouchEditor);
}

_DATA* _SetUp::GetTargetNotice(void)
{
  return(&TargetNotice);
}

_BOOL _SetUp::GetEditorIncDec(void)
{
 #ifndef ET_261
  // diese änderung wurde am 20.05.05 eingebaut (sa 5585)
  if(GetUseShuttleForEdit() == _TRUE)
  {
    // bei verwendung shuttle wird wert verändert und erst nach taste enter übernommen
    return(_FALSE);
  }
  else
 #endif
  {
    return(EditorIncDec);
  }
}

_BOOL _SetUp::GetHotkeysInMenu(void)
{
  return(HotkeysInMenu);
}

_BOOL _SetUp::GetUseShuttleForEdit(void)
{
  return(UseShuttleToEdit);
}

_BOOL _SetUp::GetUseHostName(void)
{
  return(UseHostName);
}

_BOOL _SetUp::GetWordWrap(void)
{
  return(WordWrap);
}

_BOOL _SetUp::SetWordWrap(_BOOL newstate)
{
  _BOOL retcode = WordWrap;
  WordWrap = newstate;
  return retcode;
}

_BOOL _SetUp::GetDisableAltEsc(void)
{
  return(DisableAltEsc);
}

_BOOL _SetUp::GetDisplayPassword(void)
{
  return(DisplayPassword);
}

void _SetUp::SetDoubleClickOption(_BOOL state)
{
  DoubleClickOption = state;
}

_BOOL _SetUp::GetDoubleClickOption(void)
{
//  return(WordWrap); // nur zum test
  return(DoubleClickOption);
}

void _SetUp::SetChkBoxImages(_UDWORD unchecked, _UDWORD checked)
{
  CheckBoxImage[0] = unchecked;
  CheckBoxImage[1] = checked;
}

void _SetUp::SetRadioBoxImages(_UDWORD unchecked, _UDWORD checked)
{
  RadioBoxImage[0] = unchecked;
  RadioBoxImage[1] = checked;
}


_BMP* _SetUp::GetChkBoxImage(_DWORD value)
{
  _UDWORD imgno = CheckBoxImage[(value == 0)? 0 : 1];
  if(imgno != DEFSCOPE)
    return ImageList.Get(imgno);
  return NULL;
}

_BMP* _SetUp::GetRadioBoxImage(_DWORD value)
{
  _UDWORD imgno = RadioBoxImage[(value == 0)? 0 : 1];
  if(imgno != DEFSCOPE)
    return ImageList.Get(imgno);
  return NULL;
}

//****************************************************************************
//  LASAL Interface
//****************************************************************************
//****************************************************************************
//****************************************************************************

_COLOR GetEditColorBack(_EDITSTATE state)
{
  return(SetUp.GetEditColorBack(state, MakeColor_Makro(BLACK, WHITE)));
}

_COLOR GetEditColorFrame(_EDITSTATE state)
{
  return(SetUp.GetEditColorFrame(state, MakeColor_Makro(BLACK, WHITE)));
}

_COLOR GetEditColorTxt(_EDITSTATE state)
{
  return(SetUp.GetEditColorTxt(state, MakeColor_Makro(BLACK, WHITE)));
}

_COLOR GetEditColorCursor(_EDITSTATE state)
{
  return(SetUp.GetEditColorCursor(state));
}

_DATA* GetTargetNotice(void)
{
  return(SetUp.GetTargetNotice());
}

_BOOL GetEditorIncDec(void)
{
  return(SetUp.GetEditorIncDec());
}

_BOOL GetHotkeysInMenu(void)
{
  return(SetUp.GetHotkeysInMenu());
}

_SOFTKEYSETUP* Get_SOFTKEYSETUP(void)
{
  return(SetUp.Get_SOFTKEYSETUP());
}

_SCREENSAVER* Get_SCREENSAVER(void)
{
  return(SetUp.Get_SCREENSAVER());
}

_DEFKEYPAD* Get_DEFKEYPAD(void)
{
  return SetUp.Get_DEFKEYPAD();
}

void SetDoubleClickOption(_BOOL state)
{
  SetUp.SetDoubleClickOption(state);
}

_BOOL GetDoubleClickOption(void)
{
  return(SetUp.GetDoubleClickOption());
}

#ifdef UC_BUBBLE_HELP
_BUBBLEINFO* Get_BUBBLEINFO(void)
{
  return(SetUp.Get_BUBBLEINFO());
}

void Set_BUBBLEINFO_delaytime(_DWORD delaytime)
{
  SetUp.Set_BUBBLEINFO_delaytime(delaytime);
}

void Set_BUBBLEINFO_remaintime(_DWORD remaintime)
{
  SetUp.Set_BUBBLEINFO_remaintime(remaintime);
}
#endif

void SetChkBoxImages(_UDWORD unchecked, _UDWORD checked)
{
  SetUp.SetChkBoxImages(unchecked, checked);
}

void SetRadioBoxImages(_UDWORD unchecked, _UDWORD checked)
{
  SetUp.SetRadioBoxImages(unchecked, checked);
}

extern "C" _BOOL SetWordWrap(_BOOL newstate)
{
  return SetUp.SetWordWrap(newstate);
}



