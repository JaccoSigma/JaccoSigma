#include "T_IPC.H"
#include "T_NUC.H"

_BOOL _SubPicture::MakeStandardWindow(_UDWORD no)
{
  _BOOL retcode = _FALSE;

  if(no == UC_CONFIRMWINDOW)
  {
    retcode = _TRUE;
    
    _ROOM sroom;
    MyGetScreenSize(&sroom);
    
    _KOORD dx = sroom.xy2.x - sroom.xy1.x + 1;
    _KOORD dy = sroom.xy2.y - sroom.xy1.y + 1;
    _KOORD xx = (MessageBox.WidthBox > dx)? dx : MessageBox.WidthBox;
    _KOORD yy = (MessageBox.HeightBox > dy)? dy : MessageBox.HeightBox; 

    sroom.xy1.x = (dx - xx) / 2;
    sroom.xy1.y = (dy / 3) - (yy / 2);
    sroom.xy2.x = sroom.xy1.x + xx;
    sroom.xy2.y = sroom.xy1.y + yy;
      
    Picture.mask.room = sroom;  
  }
 #ifdef UC_BUBBLE_HELP 
  else if(no == UC_NEWBUBBLEWINDOW)
  {
    retcode = NewBubbleHelp.DoSize(&Picture);
  }
 #endif
  return retcode;
}

_UDWORD _MessageBox::GetButtonNo(_MESSAGEBOX *pbox)
{
  _UDWORD no = 0;
  if(pbox->MemButton[0].Result != MESSAGEBOXRESULT_NONE)
  {
    no += 1;
    if(pbox->MemButton[1].Result != MESSAGEBOXRESULT_NONE)
      no += 1;
  }
  
  return no;
}

_BOOL _MessageBox::ButtonPosition(_ROOM *pr, _MESSAGEBOX *pbox)
{
  _UDWORD no = GetButtonNo(pbox);
  _KOORD  xx = (pr->xy2.x - pr->xy1.x + 1 - (no * MessageBox.WidthButton)) / (no + 1);
  _KOORD  x0 = pr->xy1.x + xx;

  _MESSAGEBOXBUTTON *pmb = pbox->MemButton;

  while(no)
  {
    pmb->Space.xy1.x = x0; 
    pmb->Space.xy2.x = x0 + MessageBox.WidthButton;
    pmb->Space.xy1.y = pr->xy2.y - (20 + MessageBox.HeightButton);
    pmb->Space.xy2.y = pmb->Space.xy1.y + MessageBox.HeightButton;
  
    x0 += MessageBox.WidthButton + xx;
  
    no --;
    pmb ++;
  }

  return _TRUE;
}

void _MessageBox::DrawButtonSingle(_MESSAGEBOXBUTTON *pmb, _BOOL state)
{
  _ROOM   room = pmb->Space;
  _ATTRIB att = T_MIDBOUND|T_CENTERBOUND|T_COPY|T_SOLID|T_PROP;
  
  if(state == _TRUE)
    att |= T_INVRS;
  
  OutMultiTextAlign(pmb->Text, 0, &room, pmb->Font, att, pmb->FrameType, pmb->ColorText, pmb->ColorBack, pmb->ColorFrame, pmb->ChrSize);
  
 #ifdef UC_ROTATE
  RefreshRectangle(&pmb->Space);
 #endif  
}

void _MessageBox::DrawButtonAll(_ROOM *pr, _MESSAGEBOX *pbox)
{
  if(ButtonPosition(pr, pbox) == _TRUE)
  {
    _MESSAGEBOXBUTTON *pmb = pbox->MemButton;
    _UDWORD no = GetButtonNo(pbox);
    for(_UDWORD i=0; i<no; i++, pmb++)
      DrawButtonSingle(pmb, _FALSE);
  }
}

_BOOL _MessageBox::ButtonPress(_MESSAGEBOXBUTTON *pmb)
{
  DrawButtonSingle(pmb, _TRUE);

  if(pmb->pFkt != NULL)
  {
    _MESSAGEBOXRESULT result = pmb->Result;
    _FVOID_MESSAGEBOXRESULT pfkt = pmb->pFkt;
    void *sonst_compilererror = pmb->pObj;
    load_this(sonst_compilererror);
    pfkt(result);
  }
  return _TRUE;
}

_BOOL _MessageBox::ButtonHid(_MESSAGEBOX *pbox, _DOT *pxy)
{
  _UDWORD no = GetButtonNo(pbox);
  _MESSAGEBOXBUTTON *pmb = pbox->MemButton;

  while(no)
  {
    if((pxy->x >= pmb->Space.xy1.x) && (pxy->x <= pmb->Space.xy2.x) && (pxy->y >= pmb->Space.xy1.y) && (pxy->y <= pmb->Space.xy2.y))
      return ButtonPress(pmb);
      
    no --;
    pmb ++;
  }
  
  return _FALSE;
}

_BOOL _MessageBox::ButtonKey(_MESSAGEBOX *pbox, _UDWORD scancode)
{
  _UDWORD no = GetButtonNo(pbox);
  _MESSAGEBOXBUTTON *pmb = pbox->MemButton;

  while(no)
  {
    if((pmb->Scancode != 0) && (scancode == pmb->Scancode))
      return ButtonPress(pmb);
      
    no --;
    pmb ++;
  }
  
  return _FALSE;
}

void _MessageBox::Run(_MY_PICTURE *mp, _EVENT *pe, _UDWORD messboxindex)
{
  _MESSAGEBOX *pbox = Get(messboxindex);
  if(pbox != NULL)
  {
    if(mp->JustProgram == _TRUE)
    {
      _BOOL close = _FALSE;
      
      if(pe->ftype == _EVENT_KEYPRESS)
      {
        close = ButtonKey(pbox, pe->scancode);
        if(close == _TRUE)
          pe->ftype = _EVENT_NONE;
      }
      else if(pe->ftype == _EVENT_HIDPRESS)
      {
        if(pbox->FrameType >= _THEMEFRAME)
        {
          _ROOM room = mp->Room;
          if(is_THEME_cancle(&pe->dot, pbox->FrameType, &room, NULL, NULL) == _TRUE)
          {
            found_something_by_hid(pe, FOUND_CANCLE);
            draw_THEME_cancle(&room, pbox->FrameType, _TRUE);
            close = _TRUE;
          }
        }
    
        if(ButtonHid(pbox, &pe->dot) == _TRUE)
        {
          found_something_by_hid(pe, FOUND_BUTTON);
          close = _TRUE;
        }
      }
      
      if(close == _TRUE)
      {
        InsertCmd(CMD_RETURN, 0, 0);
      }
    }
  }  
}

void _MessageBox::Ready(_MY_PICTURE *mp, _UDWORD messboxindex)
{
  _MESSAGEBOX *pbox = Get(messboxindex);
  if(pbox != NULL)
  {
    if(mp->JustProgram == _TRUE)
    {
      // hintergrund zeichnen
      _ROOM tmpr = mp->Room;
      ::DrawButton(&tmpr, T_COPY | T_SOLID, pbox->FrameType, pbox->ColorBack, pbox->ColorFrame);
    
      _ROOM room;
      GetTextRoom(&room, &tmpr, T_COPY | T_SOLID, pbox->FrameType, _TRUE);
    
      //room.xy1.x += 20;
      //room.xy1.y += 20;
      //room.xy2.x -= 20;
      room.xy2.y -= (MessageBox.HeightButton + 20);
    
      // text ausgeben
      if(pbox->Text[0] != 0)
        OutMultiTextAlign(pbox->Text, 0, &room, pbox->Font, T_MIDBOUND|T_CENTERBOUND|T_COPY|T_SOLID|T_PROP, _DEFFRAME, pbox->ColorText, pbox->ColorBack, pbox->ColorFrame, sizeof(_CHAR));
    
      // buttons zeichnen
      DrawButtonAll(&mp->Room, pbox);
    
     #ifdef UC_ROTATE
      RefreshRectangle(&mp->Room);
     #endif  
    }
  }
}

void _MessageBox::Init(void)
{
  LastUsedIndex = 0xFFFFFFFF;
  for(_UDWORD i=0; i<MESSAGEBOXNO; i++)
    Init_MESSAGEBOX(&MessBox[i], NULL, 0, 0, MESSAGEBOXTYP_OK, 0xFFFFFFFF, NULL, NULL);

 #ifdef UC_THEME
  FrameTypeBox          =  FRAME_METAL_CANCEL;
  FrameTypeButton       =  FRAME_PLASTIC;
 #else
  FrameTypeBox          =  _3DFRAME;
  FrameTypeButton       =  _3DFRAME;
 #endif 
  WidthBox              = 450;
  HeightBox             = 200;
  WidthButton           = 120;
  HeightButton          =  46;
  ColorBoxBackground    = MakeColor_Makro(WHITE, WHITE);
  ColorBoxText          = MakeColor_Makro(WHITE, BLACK);
  ColorBoxFrame         = MakeColor_Makro(LIGHTGRAY, DARKGRAY);
  ColorButtonBackground = MakeColor_Makro(LIGHTGRAY, LIGHTGRAY);
  ColorButtonText       = MakeColor_Makro(LIGHTGRAY, BLACK);
  ColorButtonFrame      = MakeColor_Makro(WHITE, DARKGRAY);
  FontScheme            = 0xFF;
}

void _MessageBox::Free(void)
{
  Init();
}

_MESSAGEBOX *_MessageBox::Get(_UDWORD idx)
{
  if(idx < MESSAGEBOXNO)
    return &MessBox[idx];
  return NULL;
}

_UDWORD _MessageBox::GetUnused(void)
{
  _UBYTE midx[MESSAGEBOXNO];
  MemSet(midx, 0, MESSAGEBOXNO);
  Root.GetUsedMessBoxIndex(midx);
  
  _UDWORD no = MESSAGEBOXNO;
  _UDWORD i  = LastUsedIndex;
  
  while(no)
  {
    i ++;
    if(i > MESSAGEBOXNO)
      i = 0;
     
    if(midx[i] == 0)
    {
      LastUsedIndex = i;
      return i;
    } 
    
    no --;
  }
  
  return 0xFFFFFFFF;
}

void _MessageBox::Init_MESSAGEBOXBUTTON(_MESSAGEBOXBUTTON *p, _FONT font, _MESSAGEBOXRESULT result, void *pthis, _FVOID_MESSAGEBOXRESULT pfkt)
{

  p->Text    = NULL;
  p->ChrSize = sizeof(_ASCII);

  switch(result)
  {
    case MESSAGEBOXRESULT_OK     : Language.GetStandardText((void**)&p->Text, &p->ChrSize, TxtID_OK);     break;
    case MESSAGEBOXRESULT_CANCLE : Language.GetStandardText((void**)&p->Text, &p->ChrSize, TxtID_CANCLE); break;
    case MESSAGEBOXRESULT_YES    : Language.GetStandardText((void**)&p->Text, &p->ChrSize, TxtID_YES);    break;
    case MESSAGEBOXRESULT_NO     : Language.GetStandardText((void**)&p->Text, &p->ChrSize, TxtID_NO);     break;
  }
  
  p->ColorText  = ColorButtonText;
  p->ColorBack  = ColorButtonBackground;
  p->ColorFrame = ColorButtonFrame;
  p->FrameType  = GetFrameType(FrameTypeButton, FRAME_STAGE_LIGHT, FRAME_STEALTH, FRAME_3D);
  p->Font       = font;
  init_ROOM_Makro(&p->Space);
  p->Scancode   = 0;
  p->Result     = result;
  p->pObj       = pthis;
  p->pFkt       = pfkt;
  
  switch(result)
  {
    case MESSAGEBOXRESULT_OK     :
    case MESSAGEBOXRESULT_YES    : p->Scancode = _ENTER;
                                   break;    
    case MESSAGEBOXRESULT_NO     :
    case MESSAGEBOXRESULT_CANCLE : p->Scancode = _ESC;
                                   break;
  }
}


void _MessageBox::Init_MESSAGEBOX(_MESSAGEBOX *p, void *txt, _UWORD chrsize, _FONT font, _MESSAGEBOXTYP typ, _UDWORD imageno, void *pthis, _FVOID_MESSAGEBOXRESULT pfkt)
{
  p->Text[0] = 0;
  if(txt != NULL)
    StrCpy(p->Text, sizeof(_CHAR), txt, chrsize);
  p->ColorText  = ColorBoxText;
  p->ColorBack  = ColorBoxBackground;
  p->ColorFrame = ColorBoxFrame;
  p->ImageNo    = imageno;
  p->FrameType  = GetFrameType(FrameTypeBox, 78, 26, _3DFRAME);
  p->Font       = font;
  p->Typ        = typ;
  
  switch(typ)
  {
    case MESSAGEBOXTYP_OK_CANCLE :
      Init_MESSAGEBOXBUTTON(&p->MemButton[0], font, MESSAGEBOXRESULT_OK,     pthis, pfkt);
      Init_MESSAGEBOXBUTTON(&p->MemButton[1], font, MESSAGEBOXRESULT_CANCLE, pthis, pfkt);
      break;
    
    case MESSAGEBOXTYP_YES_NO :
      Init_MESSAGEBOXBUTTON(&p->MemButton[0], font, MESSAGEBOXRESULT_YES, pthis, pfkt);
      Init_MESSAGEBOXBUTTON(&p->MemButton[1], font, MESSAGEBOXRESULT_NO,  pthis, pfkt);
      break;
    
    default :
      Init_MESSAGEBOXBUTTON(&p->MemButton[0], font, MESSAGEBOXRESULT_OK,   pthis, pfkt);
      Init_MESSAGEBOXBUTTON(&p->MemButton[1], font, MESSAGEBOXRESULT_NONE, pthis, pfkt);
      break;
  }
}

_BOOL _MessageBox::Call(void *text, _UWORD chrsize, _FONT font, _MESSAGEBOXTYP typ, void *pthis, _FVOID_MESSAGEBOXRESULT pfkt)
{
  _UDWORD mesboxidx = GetUnused();
  if(mesboxidx < MESSAGEBOXNO)
  {
    Init_MESSAGEBOX(&MessBox[mesboxidx], text, chrsize, font, typ, 0xFFFFFFFF, pthis, pfkt);
    InsertCmd(CMD_MESSAGEBOX, UC_CONFIRMWINDOW, mesboxidx);
    return _TRUE;
  }
  
  return _FALSE;
}

// extern fubnctions *************************************************************************

void MessageBoxSetColorBox(_COLOR back, _COLOR text, _COLOR frame)
{
  MessageBox.ColorBoxBackground = back;
  MessageBox.ColorBoxText       = text;
  MessageBox.ColorBoxFrame      = frame;
}

void MessageBoxSetColorButton(_COLOR back, _COLOR text, _COLOR frame)
{
  MessageBox.ColorButtonBackground = back;
  MessageBox.ColorButtonText       = text;
  MessageBox.ColorButtonFrame      = frame;
}

void MessageBoxSetFrame(_FRAMETYPE box, _FRAMETYPE button)
{
  MessageBox.FrameTypeBox    = box;
  MessageBox.FrameTypeButton = button;
}

void MessageBoxSetSizeBox(_UDWORD width, _UDWORD height)
{
  MessageBox.WidthBox  = width;
  MessageBox.HeightBox = height;
}

void MessageBoxSetSizeButton(_UDWORD width, _UDWORD height)
{
  MessageBox.WidthButton  = width;
  MessageBox.HeightButton = height;
}

void MessageBoxSetFontScheme(_ASCII *fontschemename)
{
  MessageBox.FontScheme = (_FONT)FontScheme.GetIndexByLabel(fontschemename);
}

_BOOL MessageBoxCall(void *text, _UWORD chrsize, _FONT font, _MESSAGEBOXTYP typ, void *pthis, _FVOID_MESSAGEBOXRESULT pfkt)
{
  return MessageBox.Call(text, chrsize, font, typ, pthis, pfkt);
}

_BOOL MessageBoxCallEx(void *text, _UWORD chrsize, _MESSAGEBOXTYP typ, void *pthis, _FVOID_MESSAGEBOXRESULT pfkt)
{
  if(MessageBox.FontScheme >= FontScheme.GetNo())
  {
    MessageBox.FontScheme = (_FONT)FontScheme.GetIndexByLabel((_ASCII*)"_@Standard");
  }
  
  return MessageBox.Call(text, chrsize, MessageBox.FontScheme, typ, pthis, pfkt);
}


