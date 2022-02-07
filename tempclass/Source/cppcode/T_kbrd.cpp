#include "T_IPC.H"
#include "T_NUC.H"

#define SA31568  // Multitouchhardware im einsatz, verhalten aber wie singletouch (UC_MULTITOUCH deaktiviert) gewünscht

//#define UC_MULTIDISPLAY 2 // todo: wieder raus
#ifndef UC_MULTIDISPLAY
 #define UC_MULTIDISPLAY 1
#endif

#define DEVSW275 // wunsch applikation 19.10.2017

#define event_NONE    0x00 // no events pending
#define event_MOVE    0x01 // input movement
#define event_PRESS   0x02 // button press
#define event_RELEASE 0x04 // button release
#define event_KEYDN   0x08 // key press
#define event_KEYUP   0x10 // key release
#define event_USER1   0x20 // user program event 1
#define event_USER2   0x40 // user program event 2
#define event_LASALOS 0x80 // lasalos event

#ifdef UC_MULTITOUCH
#ifdef UC_FLICK_ZOOM
void init_MULTIEVENT(_MULTIEVENT *p)
{
  p->dotindex = 0;
  p->timestamp = 0;
}
#endif
#endif

void _Keyboard::Init(void)
{
 #ifdef UC_LOGEVENT 
  LogCnt = 0;
  LogNoneCnt = 0;
  LogEvent(NULL);  
 #endif

  HideEventDueWindowClosed = 0;
  init_ROOM_Makro(&HidPrePressRoom);
  HidPrePressValid = _FALSE;
  LasalOsCnt = 0;

  MouseAnzahl = 0;
  SecretTime = 0;
  KillMask = 0;
  mTouchDeviceLock = 0;
  PtMethodSetEvent = NULL;
  PtMethodGetEvent = NULL;
  UnderCover.Init();
  init_EVENT_Makro(&Event);
  
  for(_UDWORD i=0; i<MAX_MULTIFINGER; i++)
    init_EVENTMEMO(&EventMemo[i]);
  
  init_EVENTMEMO(&EventMemoLrm);
  
  TriggerEventValid = 0;
  
  NextEvent.ftype = _EVENT_NONE;
 #ifdef UC_MULTITOUCH
 #ifdef UC_FLICK_ZOOM
  for(_UDWORD i=0; i<MAX_MULTIFINGER; i++)
    init_MULTIEVENT(&MultiEvent[i]);
 #endif
 #endif 
 
  init_EVENT_Makro(&HidPrePressEvent);
  
 #ifdef UC_RELEASE_PRESS_FILTER 
  RP_DelayTime = 0;
  MemSet(&RP_Memo, 0, sizeof(_OSEVENT));
  RP_Memo.eventType = event_NONE;
  SetRP_DelayTime(UC_RELEASE_PRESS_FILTER);
 #endif
}

_UDWORD _Keyboard::SetRP_DelayTime(_UDWORD ms_time)
{
 #ifdef UC_RELEASE_PRESS_FILTER 
 
  RP_DelayTime = ms_time;
  if(RP_DelayTime == 0)
    RP_Memo.eventType = event_NONE;
 #endif
 return ms_time;
}

void _Keyboard::SetHidPrePress(void)
{
 #ifndef USER_HERZ
//  _BOOL st = Root.IsThereAnyCarousel();
//  if(HidPrePressValid != st)
//  {
//    init_EVENT_Makro(&HidPrePressEvent);
//    init_ROOM_Makro(&HidPrePressRoom);
//    HidPrePressValid = st;
//    
//    if(st == _TRUE)
//    {
//      Root.GetCarouselRoom(&HidPrePressRoom);
//    }
//  }
  
  _BOOL st = Root.IsThereAnyCarousel();
  init_EVENT_Makro(&HidPrePressEvent);
  init_ROOM_Makro(&HidPrePressRoom);
  HidPrePressValid = st;
  
  if(st == _TRUE)
  {
    Root.GetCarouselRoom(&HidPrePressRoom);
  }
  
 #endif 
}


void _Keyboard::Free(void)
{
  for(_UDWORD i=0; i<MAX_MULTIFINGER; i++)
    free_EVENTMEMO(&EventMemo[i]);

  free_EVENTMEMO(&EventMemoLrm);

  free_EVENT(&Event);
  Init();
}

void _Keyboard::Install(void)
{
  _UBYTE      para = event_KEYDN|event_KEYUP;
  _DOIT       doit = _IDLE;
  _FDOIT_VOID ptr;

  ptr = (_FDOIT_VOID)find_lasal_method(LseThisPointer, METHOD_LSE_STARTEVENT);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    doit = ptr();

    if(doit == _IDIDIT)
    {
      PtMethodGetEvent = (_FDOIT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_GETEVENT);
      PtMethodSetEvent = (_FDOIT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_SETEVENT);
    }
  }

  if(doit == _IDLE)
  {
   #ifdef HID
   {
//     _ROOM r;
//     GetScreenSize(&r);
//     OS_MoveCursor((r.xy1.x+r.xy2.x)/2, (r.xy1.y+r.xy2.y)/2);
     #ifdef UC_MOUSE
     {
      #ifndef UC_MOUSE_AUTODETECT
//       OS_ShowCursor();
       ShowMouse();
      #endif
       para |= (event_PRESS|event_RELEASE);
     }
     #endif
     #ifdef UC_TOUCH
     {
       para |= (event_PRESS|event_RELEASE);
     }
     #endif
     para |= event_MOVE;
   }
   #endif

   #ifndef UC_IPC
    para |= (event_PRESS|event_RELEASE|event_MOVE);
   #endif

    para |= (event_USER1|event_USER2|event_LASALOS);

    OS_StartEvent(para); // enable keyboard
  }

}

extern "C" void CStartEvent(void)
{
  OS_StartEvent(event_KEYDN|event_KEYUP|event_USER1|event_USER2|event_LASALOS|event_PRESS|event_RELEASE|event_MOVE); // enable keyboard
}
extern "C" void CStopEvent(void)
{
  OS_StopEvent(); // disable keyboard
}
extern "C" _UDWORD CKeyEvent(void *ptr)
{
  return(OS_KeyEventEx(0, ptr, sizeof(_OSEVENT)));
}
extern "C" _UDWORD CStoreEvent(void *ptr)
{
  return(OS_StoreEventEx(ptr, sizeof(_OSEVENT)));
}

void _Keyboard::DeInstall(void)
{
  _DOIT       doit = _IDLE;
  _FDOIT_VOID ptr;

  ptr = (_FDOIT_VOID)find_lasal_method(LseThisPointer, METHOD_LSE_STOPEVENT);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    doit = ptr();
  }

  if(doit == _IDLE)
  {
   #ifdef HID
    OS_StopMouse();
   #endif
    OS_StopEvent();                           // stop keyboard and mouse event
  }
}

_BOOL _Keyboard::AddEventEx(_EVENT *evnt, _BOOL keep_timestamp)
{
  _BOOL    retcode = _TRUE;
  _OSEVENT osevent;
  _DOIT    doit = _IDLE;

  MemSet(&osevent, 0, sizeof(_OSEVENT));
  osevent.eventSourceID = (Event.lrm == _FALSE)? 0xFFFFL : 0xFFFEL;
  osevent.eventTime = (_UWORD)get_abstime();
  
  if(keep_timestamp == _TRUE)
  {
    osevent.eventTime = evnt->timestamp;
  }
  
  switch(evnt->ftype)
  {
    case _EVENT_KEYPRESS   :
    case _EVENT_KEYRELEASE : osevent.eventType    = (evnt->ftype == _EVENT_KEYPRESS)? event_KEYDN : event_KEYUP;
                             osevent.eventChar    = (_UBYTE)evnt->scancode;
                             if(evnt->scancode & 0xFF00)
                             {
                               osevent.eventScan  = (_UBYTE)(evnt->scancode >> 8);
                               //if(evnt->ftype == _EVENT_KEYRELEASE)
                               //  osevent.eventScan += 0x80;
                             }
                            #ifdef USER_KM
                            #ifdef NINETEEN
                             if(evnt->user[0] == 0xA5A5)
                             {
                               if(evnt->ftype == _EVENT_KEYRELEASE)
                                 osevent.eventScan += 0x80;
                             }
                             else
                            #endif
                            #endif
                             osevent.eventUser[0] = 0x2402;
                             break;
//   #ifdef HID
    case _EVENT_HIDMOVE    : osevent.eventType    = event_MOVE;
                             osevent.eventX       = evnt->dot.x;
                             osevent.eventY       = evnt->dot.y;
                             break;
    case _EVENT_HIDRELEASE :
    case _EVENT_HIDPRESS   : osevent.eventType    = (evnt->ftype == _EVENT_HIDPRESS)? event_PRESS : event_RELEASE;
                             osevent.eventX       = evnt->dot.x;
                             osevent.eventY       = evnt->dot.y;
                             osevent.eventButtons = 1;
                             if(evnt->button == _HID_RIGHT)
                               osevent.eventButtons = 2;
                             else if(evnt->button == _HID_MIDDLE)
                               osevent.eventButtons = 4;
                             break;
//   #endif
    case _EVENT_USER1      :
    case _EVENT_USER2      : osevent.eventType    = (evnt->ftype == _EVENT_USER1)? event_USER1 : event_USER2;
                             osevent.eventX       = evnt->dot.x;
                             osevent.eventY       = evnt->dot.y;
                             osevent.eventUser[0] = evnt->user[0];
                             osevent.eventUser[1] = evnt->user[1];
                             osevent.eventChar    = (_UBYTE)evnt->scancode;
                             if(evnt->scancode & 0xFF00)
                               osevent.eventScan  = (_UBYTE)(evnt->scancode >> 8);
                             break;
                             
    case _EVENT_LASALOS    : osevent.eventType    = event_LASALOS; // SA40475
                             osevent.eventX       = Event.dot.x;
                             osevent.eventY       = Event.dot.y;
                             osevent.eventUser[1] = Event.user[0];
                             osevent.eventUser[2] = Event.user[1];
                             osevent.eventButtons = Event.button;
                             osevent.eventScan    = Event.scancode;
                             osevent.eventSource  = Event.source;
                             osevent.eventChar    = Event.modifier;
                             osevent.eventState   = Event.state;
                             break;
                             
    default                : retcode = _FALSE;
                             break;
  }

  if(PtMethodSetEvent != NULL)
  {
    load_this(LseThisPointer);
    doit = PtMethodSetEvent(&osevent);
  }

  if(doit == _IDLE)
  {
    if(retcode == _TRUE)
    {
      if(RTOSVERSION >= 0x1164)
      {
        retcode = ((OS_StoreEventEx(&osevent, sizeof(_OSEVENT)) == 0)? _FALSE : _TRUE);
      }
      else
      {
        retcode = ((OS_StoreEvent(&osevent) == 0)? _FALSE : _TRUE);
      }
    }
  }

  return(retcode);
}

_BOOL _Keyboard::AddEvent(_EVENTTYPE ftype, _UWORD scancode)
{
  _EVENT evnt;
  init_EVENT_Makro(&evnt);
  evnt.ftype    = ftype;
  evnt.scancode = scancode;
  return AddEventEx(&evnt, _FALSE);
}

#ifdef ET_261
_UWORD shuttle_last;
#endif

#ifdef UC_MOUSE_AUTODETECT
 #define USB_EVENTX_ALTE_UNBEKANNT     0x5112
 #define USB_KEYBOARD_EVENTX           0x60B
 #define USB_KEYBOARD_EVENTY           0x2101
 #define VGA_ANSCHLUSS                 0x7DD 
#endif

void _Keyboard::DoOsEvent(_OSEVENT *p)
{
 #ifdef UC_MOUSE_AUTODETECT
  if((p->eventScan == 3)&&(p->eventButtons == 2))
  {
    // es ist ein zeigergerät (maus oder touch)
    //if(((p->eventX != 0x5112)&&(p->eventX != 0x07DD))||(p->eventY != 1))
    if(((p->eventX != USB_EVENTX_ALTE_UNBEKANNT)&&(p->eventX != VGA_ANSCHLUSS)&&(p->eventX != USB_KEYBOARD_EVENTX))||((p->eventY != 1)&&(p->eventY != USB_KEYBOARD_EVENTY)))
    {
      // es ist kein sigmatek oder hampshire-touch
      if(p->eventChar == 1) // angesteckt
      {
        //OS_ShowCursor();
        if(MouseAnzahl == 0)
          ShowMouse();
        MouseAnzahl ++;
      }
      else if(p->eventChar == 2) // abgesteckt
      {
        //OS_ShowCursor();
        if(MouseAnzahl > 0)
          MouseAnzahl --;
        
        if(MouseAnzahl == 0)
          HideMouse();
      }
    }
  }
 #endif

  switch(p->eventSource)
  {
    case seSrcUnknown : break;
    case seSrcShuttle : if(p->eventChar == seActionInc)
                        {
                          Event.ftype    = _EVENT_SHUTTLEINC;
                          Event.scancode = p->eventScan;
                        }
                        else if(p->eventChar == seActionDec)
                        {
                          Event.ftype    = _EVENT_SHUTTLEDEC;
                          Event.scancode = p->eventScan;
                        }
                        break;
    default :           Event.ftype    = _EVENT_LASALOS;
                        Event.dot.x    = p->eventX;
                        Event.dot.y    = p->eventY;
                        Event.user[0]  = p->eventUser[1];
                        Event.user[1]  = p->eventUser[2];
                        *(_UWORD*)&Event.button   = p->eventButtons;
                        Event.scancode = p->eventScan;
                        Event.source   = p->eventSource;
                        Event.modifier = p->eventChar;
                        Event.state    = p->eventState;
                        LasalOsCnt    += 1;
                        break;
  }
}

#ifdef UC_RELEASE_PRESS_FILTER

//#define UC_VISUALIZE_OSEVENT
#ifdef UC_VISUALIZE_OSEVENT
void Show_OSEVENT(_OSEVENT *pose, _UDWORD st)
{
  _ROOM  room;
  _KOORD dxy;
  
  if(pose->eventType == event_PRESS)
  {
    dxy = (st == 0)? 6 : 3;
    room.xy1.x = pose->eventX-dxy;
    room.xy1.y = pose->eventY-dxy;
    room.xy2.x = pose->eventX+dxy;
    room.xy2.y = pose->eventY+dxy;
    Oval(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, 0, 3600, T_COPY|T_SOLID|T_FILL, (st == 0)? LIGHTGRAY : LIGHTRED);
    RefreshRectangle(&room);
  }
  else if(pose->eventType == event_MOVE)
  {
    dxy = (st == 0)? 1 : 0;
    room.xy1.x = pose->eventX-dxy;
    room.xy1.y = pose->eventY-dxy;
    room.xy2.x = pose->eventX+dxy;
    room.xy2.y = pose->eventY+dxy;
    Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY|T_SOLID|T_FILL, (st == 0)? DARKGRAY : YELLOW);
    RefreshRectangle(&room);
  }
  else if(pose->eventType == event_RELEASE)
  {
    dxy = (st == 0)? 6 : 3;
    room.xy1.x = pose->eventX-dxy;
    room.xy1.y = pose->eventY-dxy;
    room.xy2.x = pose->eventX+dxy;
    room.xy2.y = pose->eventY+dxy;
    Oval(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, 0, 3600, T_COPY|T_SOLID|T_FILL, (st == 0)? DARKGRAY : LIGHTGREEN);
    RefreshRectangle(&room);
  }
}
#endif

_DOIT _Keyboard::RP_Filter(_DOIT doit, _OSEVENT *pose)
{
  // mit dieser funktion wird die kurze aufeinanderfolge von touch_release und touch_press entfernt
  // die zeit welche die "kurze aufeinanderfolge" definiert ist mit UC_RELEASE_PRESS_FILTER in ms angegeben
  // opsys: ______P--------R_P----R__________
  // appli: ______P-------------------R______
  // vorsicht: diese funktion verzögert jedes touch_release um die zeit UC_RELEASE_PRESS_FILTER
  
  _OSEVENT tmp;

  if(doit == _IDLE)
  {
    pose->eventType = event_NONE;
  }

  if(pose->eventType != event_NONE)
  {
    if(RTOSVERSION < 0x12E0)
    {
      // bei opsys kleiner als 1.2.224 ist kein bzw.falscher timestamp auf event --> deshalb korrektur
      pose->eventTime = (_UWORD)(get_abstime() & 0xFFFF);
    }
  }
  
  if(pose->eventType == event_RELEASE)
  {
   #ifdef UC_VISUALIZE_OSEVENT
    Show_OSEVENT(pose, 0);
   #endif      

    // event ins memo schreiben ******************************************************************* 
    if(RP_Memo.eventType != event_NONE) 
    {
      // falls im memo bereits etwas steht --> muss dies an applikation weitergeleitet werden
      tmp     = RP_Memo;
      RP_Memo = *pose;
      *pose   = tmp;
      doit    = _IDIDIT;
      
    }
    else
    {
      // RELEASE in memo sichern und "nichts" an applikation weiterleiten
      RP_Memo         = *pose;
      pose->eventType = event_NONE;
      doit            = _IDLE;
    }
  }
  else
  {
    if(RP_Memo.eventType != event_NONE)
    {
      if(RP_Memo.eventType == event_RELEASE)
      {
        _UWORD tux = (_UWORD)((get_abstime() & 0xFFFF) - RP_Memo.eventTime);
        if(tux > RP_DelayTime) // timeout überprüfen
        {
          // zeit ist abgelaufen --> memo an applikation senden ***********************************
          if((pose->eventType == event_NONE) || (pose->eventType == event_MOVE))
          {
            // + memo löschen
            *pose             = RP_Memo;
            doit              = _IDIDIT;
            RP_Memo.eventType = event_NONE;
          }
          else
          {
            // + in diesem fall muss das aktuelle event ins memo kopiert werden
            tmp     = *pose;
            *pose   = RP_Memo;
            RP_Memo = tmp;
          }
        }
        else
        {
          // zeit ist noch nicht abgelaufen *******************************************************
          if((pose->eventType != event_NONE) && (pose->eventType != event_MOVE))
          {
            if(pose->eventType == event_PRESS)
            {
              // lösche memo
              // lösche event
             #ifdef UC_VISUALIZE_OSEVENT
              Show_OSEVENT(pose, 0);
             #endif      
             
              RP_Memo.eventType = event_NONE;
              pose->eventType   = event_NONE;
              doit              = _IDLE;
            }
            else
            {
              // memo an applikation weiterleiten
              // aktuelles event ins memo kopieren
              tmp     = RP_Memo;
              RP_Memo = *pose;
              *pose   = tmp;
              doit    = _IDIDIT;
            }
          }  
        }
      }
      else
      {
        // memo an applikation weiterleiten
        // ggf. aktuelles event in memo 
        tmp     = *pose;
        *pose   = RP_Memo;
        RP_Memo = tmp;
        doit = _IDIDIT;
      }
    }
  }
  
 #ifdef UC_VISUALIZE_OSEVENT
  Show_OSEVENT(pose, 1);
 #endif      
  
  return doit;
}

/*
_DOIT _Keyboard::RP_Filter(_DOIT doit, _OSEVENT *pose)
{
  // mit dieser funktion wird die kurze aufeinanderfolge von touch_release und touch_press entfernt
  // die zeit welche die "kurze aufeinanderfolge" definiert ist mit UC_RELEASE_PRESS_FILTER in ms angegeben
  // opsys: ______P--------R_P----R__________
  // appli: ______P-------------------R______
  // vorsicht: diese funktion verzögert jedes touch_release um die zeit UC_RELEASE_PRESS_FILTER
  if(doit == _IDIDIT)
  {
    if(RTOSVERSION < 0x12E0)
    {
      // bei opsys kleiner als 1.2.224 ist kein bzw.falscher timestamp auf event --> deshalb korrektur
      pose->eventTime = (_UWORD)(get_abstime() & 0xFFFF);
    }
    
   #ifdef UC_VISUALIZE_OSEVENT
    Show_OSEVENT(pose, 0);
   #endif
  }
  
  if(RP_Memo.eventType != event_NONE)
  {
    if(RP_Memo.eventType == event_RELEASE)
    {
      _UWORD tux = (_UWORD)((get_abstime() & 0xFFFF) - RP_Memo.eventTime);
      if(tux > RP_DelayTime) // timeout überprüfen
      {
        if(doit != _IDIDIT)
        {
          // memo an applikation weiterleiten
          *pose = RP_Memo;
          RP_Memo.eventType = event_NONE;
          doit = _IDIDIT;
        }
        else
        {
          _OSEVENT maxi = *pose;
          *pose = RP_Memo;
          RP_Memo = maxi;
        }
      }
      else if(pose->eventType == event_PRESS)
      {
        // release im memo und press im osevent ignorieren
        pose->eventType = event_NONE; 
        RP_Memo.eventType = event_NONE;
        doit = _IDLE;
      }
      else if(doit == _IDIDIT)
      {
        // zuerst memo an appli weiterleiten und osevent in memo puffern
        _OSEVENT maxi = *pose;
        *pose = RP_Memo;
        RP_Memo = maxi;
      }
    }
    else
    {
      if(doit != _IDIDIT)
      {
        *pose = RP_Memo;
        RP_Memo.eventType = event_NONE;
        doit = _IDIDIT;
      } 
      else
      {
        _OSEVENT maxi = *pose;
        *pose = RP_Memo;
        RP_Memo = maxi;
      }
    }
  }
  else if(doit == _IDIDIT)
  {
    if(pose->eventType == event_RELEASE)
    {
      RP_Memo = *pose;
      pose->eventType = event_NONE;
      doit = _IDLE;
    }
  }
  
 #ifdef UC_VISUALIZE_OSEVENT
  if(doit == _IDIDIT)
    Show_OSEVENT(pose, 1);
 #endif
 
  return doit;
}
*/
#endif

void _Keyboard::MakeScancode(_EVENT *pe, _OSEVENT *posevent)
{
  pe->modifier = posevent->eventState;
  
 #ifdef UC_UNICODE
  if(pe->selfmade)
  {
    pe->scancode = (posevent->eventScan << 8);
        
    if(posevent->eventChar & 0xFF)
    {
      pe->scancode |= posevent->eventChar;
      pe->is_character = _TRUE;
    }
  }
  else
 #endif
  if(posevent->eventChar & 0xFF)
  {
    pe->is_character = _TRUE;
    if((posevent->eventChar == 0xE0) && (posevent->eventScan > 0x40))
    {
      pe->scancode = ((posevent->eventScan) << 8);
    }
    else
    {
      pe->scancode = posevent->eventChar;
    }
  }
  else
  {
    if(pe->ftype == _EVENT_KEYRELEASE)
    {
      pe->scancode = ((posevent->eventScan - 0x80) << 8);
    }
    else
    {
      pe->scancode = ((posevent->eventScan) << 8);
    }
  }
  
  // tastatur-berichtigung für km-terminal mit rtk32
  if(pe->scancode != 0)
  {
    if(pe->modifier & _ALT)        // ALT
    {
      switch(pe->scancode)
      {
        case 0x3B00 : pe->scancode = 0x6800; break;  // ALT F1
        case 0x3C00 : pe->scancode = 0x6900; break;  // ALT F2
        case 0x3D00 : pe->scancode = 0x6A00; break;  // ALT F3
        case 0x3E00 : pe->scancode = 0x6B00; break;  // ALT F4
        case 0x3F00 : pe->scancode = 0x6C00; break;  // ALT F5
        case 0x4000 : pe->scancode = 0x6D00; break;  // ALT F6
        case 0x4100 : pe->scancode = 0x6E00; break;  // ALT F7
        case 0x4200 : pe->scancode = 0x6F00; break;  // ALT F8
        case 0x4300 : pe->scancode = 0x7000; break;  // ALT F9
        case 0x4400 : pe->scancode = 0x7100; break;  // ALT F10
        case 0x5700 : pe->scancode = 0x8400; break;  // ALT F11
        case 0x5800 : pe->scancode = 0x8500; break;  // ALT F12
        case 64     : // '@'
        case 91     : // '['
        case 92     : // '\'
        case 93     : // ']'
        case 123    : // '{'
        case 125    : break; // '}'
        default     : if((pe->scancode & 0xFF00) == 0)
                      {
                        pe->scancode |= (_ALT << 8);
                      }
                      break;
      }

      if(pe->ftype == _EVENT_KEYPRESS)
      {
        LookUpDiagnostic(pe);
      }
    }
    else if(pe->modifier & _CTRL)   // CTRL
    {
      switch(pe->scancode)
      {
        case 0x3B00 : pe->scancode = 0x5E00; break;  // CTRL F1
        case 0x3C00 : pe->scancode = 0x5F00; break;  // CTRL F2
        case 0x3D00 : pe->scancode = 0x6000; break;  // CTRL F3
        case 0x3E00 : pe->scancode = 0x6100; break;  // CTRL F4
        case 0x3F00 : pe->scancode = 0x6200; break;  // CTRL F5
        case 0x4000 : pe->scancode = 0x6300; break;  // CTRL F6
        case 0x4100 : pe->scancode = 0x6400; break;  // CTRL F7
        case 0x4200 : pe->scancode = 0x6500; break;  // CTRL F8
        case 0x4300 : pe->scancode = 0x6600; break;  // CTRL F9
        case 0x4400 : pe->scancode = 0x6700; break;  // CTRL F10
        case 0x5700 : pe->scancode = 0x7A00; break;  // CTRL F11
        case 0x5800 : pe->scancode = 0x7B00; break;  // CTRL F12
        default     : if((pe->scancode & 0xFF00) == 0)
                      {
                        pe->scancode |= (_CTRL << 8);
                      }
                      break;
      }
    }
  }  
}


_UBYTE _Keyboard::MakeMultiDisplayIndex(_UBYTE midx)
{
 #ifdef UC_MULTITOUCH
 #if UC_MULTIDISPLAY > 1
   #define TMP_MUTODI
 #endif
 #endif
 
 #ifdef TMP_MUTODI
  // wenn 2 oder mehr displays angeschlossen sind muss dies auch bei multindex berücksichtigt werden
  // UC_MULTIDISPLAY beinhaltet die anzahl der angeschlossenen displays
  _UBYTE hi = (midx >> 4) & 0x0F;
  _UBYTE lo = (midx     ) & 0x0F;

  if(lo < (MAX_MULTIFINGER/UC_MULTIDISPLAY))
  {
    if((mTouchDeviceLock & (1<<hi)) == 0)
    {
      return (lo + hi * (MAX_MULTIFINGER/UC_MULTIDISPLAY));
    }
  }
  return 0xFF;
  #undef TMP_MUTODI
 #else 
  return midx;
 #endif
}

void _Keyboard::TouchDeviceLock(_UDWORD device, _BOOL state)
{
  if(device < 16)
  {
    device = 1 << device;  
    if(state == _TRUE)
    {
      mTouchDeviceLock |= device;
    }
    else
    {
      mTouchDeviceLock &= ~device;
    }
  }
}

_BOOL _Keyboard::DoAbsolutKeyboard(void)
{
  _OSEVENT osevent;
  _BOOL    retcode = _FALSE;
  _DOIT    doit    = _IDLE;
  _UDWORD  midx; 
  
  Event.ftype        = _EVENT_NONE;
  Event.scancode     = 0;
  Event.selfmade     = 0;
  Event.modifier     = 0;
  Event.is_character = _FALSE;
  Event.multindex    = 0;

  if(PtMethodGetEvent != NULL)
  {
    load_this(LseThisPointer);
    doit = PtMethodGetEvent(&osevent);
  }

  if(doit == _IDLE)
  {
    if(RTOSVERSION >= 0x1164)
    {
      if(OS_KeyEventEx(0, &osevent, sizeof(_OSEVENT)))
      {
        doit = _IDIDIT;
      }
    }
    else
    {
      if(OS_KeyEvent(0, &osevent))
      {
        doit = _IDIDIT;
        osevent.eventSourceID = 1;
      }
    }
  }

 #ifdef UC_RELEASE_PRESS_FILTER
  if(RP_DelayTime != 0)
  {
    doit = RP_Filter(doit, &osevent);
  }
 #endif

  if(doit == _IDIDIT)
  {
    Event.lrm = (osevent.eventSourceID & 1)? _FALSE : _TRUE;

  #ifdef USER_KM
   #ifdef NINETEEN
    if(Event.lrm == _TRUE)
    {
      switch(osevent.eventType)
      {
        case event_MOVE    :
        case event_RELEASE :
        case event_PRESS   :
        if(osevent.eventY >= 768)
        {
          return(_FALSE);
        }
      }
    }
   #endif
  #endif

    Event.timestamp = osevent.eventTime;

    switch(osevent.eventType)
    {
      case event_KEYUP   : Event.ftype = _EVENT_KEYRELEASE;
                           if(osevent.eventUser[0] == 0x2402)
                           {
                             Event.selfmade = 1;
                           }
                           MakeScancode(&Event, &osevent);
                           retcode = _TRUE;
                           break;
                           
      case event_KEYDN   : Event.ftype = _EVENT_KEYPRESS;
                          #ifdef USER_DEMAG 
                           if(osevent.eventChar == 13)
                           {
                             // es wird die ggf.aktive editorposition bei ENTER eingetragen
                             _Window *pw = Root.FindTop();
                             if(pw != NULL)
                             {
                                _Picture *pp = pw->GetP_Picture();
                                if(pp != NULL)
                                {
                                  _MY_PICTURE *pmp = pp->GetMyPicture();
                                  if(pmp->Editor.actno != 0xFFFF)
                                  {
                                    Event.dot.x = (pmp->Editor.input.io.space.xy1.x + pmp->Editor.input.io.space.xy2.x) / 2;
                                    Event.dot.y = (pmp->Editor.input.io.space.xy1.y + pmp->Editor.input.io.space.xy2.y) / 2;
                                  }
                                  else
                                  {
                                    Event.dot.x = -1;
                                    Event.dot.y = -1;
                                  }
                                }
                             }
                           }
                          #endif 
                           if(osevent.eventUser[0] == 0x2402)
                           {
                             Event.selfmade = 1;
                           }  
                           MakeScancode(&Event, &osevent);
                           retcode = _TRUE;
                           break;
                           
      case event_MOVE    : Event.dot.x = osevent.eventX;
                           Event.dot.y = osevent.eventY;
                           Event.raw.x = osevent.eventX;
                           Event.raw.y = osevent.eventY;
                           if(Event.lrm == _TRUE)
                           {
                             Event.ftype = _EVENT_HIDMOVE;
                           }
                           else
                           {
                             midx = osevent.eventChar;
                            
                            #ifdef UC_MULTITOUCH
                            #if UC_MULTIDISPLAY > 1
                             midx = MakeMultiDisplayIndex(midx);
                            #endif
                            #endif
                            
                             if(midx >= MAX_MULTIFINGER)
                             {
                               midx = 0;
                              #ifdef SA31568
                               if(OpSysIsSalamander()) 
                               {
                                 midx = MAX_MULTIFINGER;  // zu viele finger auf touch
                               }
                              #endif // SA31568
                             }
                             
                             if(midx < MAX_MULTIFINGER)
                             {
                               Event.multindex = midx;
                               Event.ftype     = _EVENT_HIDMOVE;
                             }
                           }
                           retcode = _TRUE;
                           break;
                           
      case event_RELEASE : Event.raw.x = osevent.eventX;
                           Event.raw.y = osevent.eventY;
                           Event.button = _HID_LEFT;
                           
                           if(Event.lrm == _TRUE)
                           {
                             Event.ftype  = _EVENT_HIDRELEASE;
                             if(osevent.eventButtons > 1) 
                             {
                               Event.dot = EventMemoLrm.HidMemoRight; // right or middle button
                               EventMemoLrm.PressMemoRight = _FALSE;
                               Event.button = _HID_RIGHT;
                             }
                             else
                             {
                               Event.dot = EventMemoLrm.HidMemoLeft;
                               EventMemoLrm.PressMemoLeft = _FALSE;
                             }
                           }
                           else
                           {
                             midx = osevent.eventChar;
                             
                            #ifdef UC_MULTITOUCH
                            #if UC_MULTIDISPLAY > 1
                             midx = MakeMultiDisplayIndex(midx);
                            #endif
                            #endif
                             
                             if(midx >= MAX_MULTIFINGER)
                             {
                               midx = 0;
                              #ifdef SA31568
                               if(OpSysIsSalamander()) 
                               {
                                 midx = MAX_MULTIFINGER;  // zu viele finger auf touch
                               }
                              #endif // SA31568
                             }
                             
                             if(midx < MAX_MULTIFINGER)
                             {
                               Event.multindex = midx;
                               Event.ftype     = _EVENT_HIDRELEASE;
                          
                               if(osevent.eventButtons > 1) // right or middle button
                               {
                                 Event.dot = EventMemo[midx].HidMemoRight; 
                                 EventMemo[midx].PressMemoRight = _FALSE;
                                 Event.button = _HID_RIGHT;
                               }
                               else
                               {
                                 Event.dot = EventMemo[midx].HidMemoLeft;
                                 EventMemo[midx].PressMemoLeft = _FALSE;
                               }
                             }
                           }
                           retcode = _TRUE;
                           break;
                           
      case event_PRESS   : Event.dot.x = osevent.eventX;
                           Event.dot.y = osevent.eventY;
                           Event.raw.x = osevent.eventX;
                           Event.raw.y = osevent.eventY;
                           Event.button = _HID_LEFT;
                           
                           if(Event.lrm == _TRUE)
                           {
                             Event.ftype = _EVENT_HIDPRESS;
                             if(osevent.eventButtons > 1)  // right or middle button
                             {
                               EventMemoLrm.HidMemoRight   = Event.dot; 
                               EventMemoLrm.PressMemoRight = _TRUE;
                               Event.button                = _HID_RIGHT;
                             }
                             else
                             {
                               EventMemoLrm.HidMemoLeft    = Event.dot;
                               EventMemoLrm.PressMemoLeft  = _TRUE;
                             }
                           }
                           else
                           {
                             midx = osevent.eventChar;
                             
                            #ifdef UC_MULTITOUCH
                            #if UC_MULTIDISPLAY > 1
                             midx = MakeMultiDisplayIndex(midx);
                            #endif
                            #endif
                             
                             if(midx >= MAX_MULTIFINGER)
                             {
                               midx = 0;
                              #ifdef SA31568
                               if(OpSysIsSalamander()) 
                               {
                                 midx = MAX_MULTIFINGER;  // zu viele finger auf touch
                               }
                              #endif // SA31568
                             }
                           
                             if(midx < MAX_MULTIFINGER)
                             {
                               Event.multindex = midx;
                               Event.ftype     = _EVENT_HIDPRESS;
                               if(osevent.eventButtons > 1) // right or middle button
                               {
                                 EventMemo[midx].HidMemoRight   = Event.dot;
                                 EventMemo[midx].PressMemoRight = _TRUE;
                                 Event.button                   = _HID_RIGHT;
                               }
                               else
                               {
                                 EventMemo[midx].HidMemoLeft   = Event.dot;
                                 EventMemo[midx].PressMemoLeft = _TRUE;
                               }
                             }
                           }
                           retcode = _TRUE;
                           break;
                           
      case event_LASALOS : DoOsEvent(&osevent);
                           retcode = _TRUE; // SA40475
                           break;
    }
  }
  
 #ifdef ET_261
  switch(Event.ftype)
  {
    case _EVENT_KEYPRESS   :
    case _EVENT_KEYRELEASE : if(osevent.eventScan == -1)
                             {
                               switch(osevent.eventChar)
                               {
                                 case  1 : Event.scancode = _UP;     break;
                                 case  2 : Event.scancode = _DOWN;   break;
                                 case  9 : Event.scancode = _PGUP;   break;
                                 case 10 : Event.scancode = _PGDOWN; break;
                                 case 17 : Event.scancode = _LEFT;   break;
                                 case 18 : Event.scancode = _RIGHT;  break;
                                 case  7 : Event.scancode = _ESC;    break;
                                 case  8 : Event.scancode = _ENTER;  break;

                                 case 25 : Event.scancode = _F1;     break;
                                 case 26 : Event.scancode = _F2;     break;
                                 case 27 : Event.scancode = _F3;     break;
                                 case 28 : Event.scancode = _F4;     break;
                                 case 29 : Event.scancode = _F5;     break;
                                 case 30 : Event.scancode = _F6;     break;

                                 case 14 : Event.scancode = 0x30;    break;
                                 case  5 : Event.scancode = 0x31;    break;
                                 case 13 : Event.scancode = 0x32;    break;
                                 case 21 : Event.scancode = 0x33;    break;
                                 case  4 : Event.scancode = 0x34;    break;
                                 case 12 : Event.scancode = 0x35;    break;
                                 case 20 : Event.scancode = 0x36;    break;
                                 case  3 : Event.scancode = 0x37;    break;
                                 case 11 : Event.scancode = 0x38;    break;
                                 case 19 : Event.scancode = 0x39;    break;
                                 case  6 : Event.scancode = '-';     break;
                                 case 22 : Event.scancode = 46;      break;
                               }
                             }
                             break;
    case _EVENT_HIDPRESS   : shuttle_last = Event.dot.x;
                             Event.ftype  = _EVENT_NONE;
                             break;
    case _EVENT_HIDRELEASE :
    case _EVENT_HIDMOVE    : if((_WORD)Event.dot.x > (_WORD)shuttle_last)
                             {
                               Event.ftype    = _EVENT_SHUTTLEINC;
                               Event.scancode = (_WORD)Event.dot.x - (_WORD)shuttle_last;
                             }
                             else
                             {
                               Event.ftype    = _EVENT_SHUTTLEDEC;
                               Event.scancode = (_WORD)shuttle_last - (_WORD)Event.dot.x;
                             }
                             shuttle_last = Event.dot.x;
                             break;
  }
 #endif

  if(Event.ftype != _EVENT_NONE)
  {
    if((KillMask != 0) && (Event.lrm == _FALSE) && (Event.selfmade == 0))
    {
      _WHOAMI whoami = Root.FindTop()->GetWhoAmI();
      if((whoami.ima != IMA_POPUP)||(whoami.no < UC_PRJ_POPUP))
      {
        if((Event.ftype == _EVENT_HIDPRESS) || (Event.ftype == _EVENT_HIDRELEASE) || (Event.ftype == _EVENT_HIDMOVE))
        {
          if(KillMask & EVENT_LOCAL_TOUCH)
          {
            free_EVENT(&Event);
            retcode = _FALSE;
          }
        }
        else if((Event.ftype == _EVENT_KEYPRESS) || (Event.ftype == _EVENT_KEYRELEASE))
        {
          if(KillMask & EVENT_LOCAL_KEYBOARD)
          {
            free_EVENT(&Event);
            retcode = _FALSE;
          }
        }
        else if((Event.ftype == _EVENT_USER1) || (Event.ftype == _EVENT_USER2))
        {
          if(KillMask & EVENT_LOCAL_USER)
          {
            free_EVENT(&Event);
            retcode = _FALSE;
          }
        }
        else if((Event.ftype == _EVENT_SHUTTLEINC) || (Event.ftype == _EVENT_SHUTTLEDEC))
        {
          if(KillMask & EVENT_LOCAL_SHUTTLE)
          {
            free_EVENT(&Event);
            retcode = _FALSE;
          }
        }
        else if(Event.ftype == _EVENT_LASALOS)
        {
          if(KillMask & EVENT_LOCAL_LASALOS)
          {
            free_EVENT(&Event);
            retcode = _FALSE;
          }
        }
      }
    }

    if((Event.ftype == _EVENT_KEYPRESS)||(Event.ftype == _EVENT_KEYRELEASE))
    {
      if(Event.scancode == 0xFFFF)
      {
        init_EVENT_Makro(&Event); // wird verworfen, weil dies der defaultwert für den scancode ist (defaulthotkeycode bei button)
      }
    }
  }

  return(retcode);
}

void _Keyboard::KillEvent(void)
{
  init_EVENT_Makro(&Event);
}


#ifdef UC_MULTITOUCH
#ifdef UC_FLICK_ZOOM
_DWORD vektor(_DOT *d1, _DOT *d2)
{
  _DWORD xx = d2->x - d1->x;
  _DWORD yy = d2->y - d1->y;
  
  return xx*xx + yy*yy;
}

_BOOL _Keyboard::FlickZoom(_UDWORD timestamp)
{
  if((MultiEvent[1].dotindex != 0) && (MultiEvent[0].dotindex != 0))
  {
    // nur wenn wirklich 2 finger am screen

    // auf scancode steht eine relative änderung
    // auf raw steht eine absolute prozentuelle änderung (100% = 1000)
    // auf dot steht die mittelposition der beiden initialen finger

    init_EVENT_Makro(&NextEvent);
    
    _DWORD dx0, dx1, dy0, dy1;
    
    dx0 = MultiEvent[0].dot[0].x - MultiEvent[1].dot[0].x;
    dy0 = MultiEvent[0].dot[0].y - MultiEvent[1].dot[0].y;
    if(dx0 < 0)  dx0 = -dx0;
    if(dx0 == 0) dx0 = 1;
    if(dy0 < 0)  dy0 = -dy0;
    if(dy0 == 0) dy0 = 1;
    
    dx1 = MultiEvent[0].dot[MultiEvent[0].dotindex-1].x - MultiEvent[1].dot[MultiEvent[1].dotindex-1].x;
    dy1 = MultiEvent[0].dot[MultiEvent[0].dotindex-1].y - MultiEvent[1].dot[MultiEvent[1].dotindex-1].y;
    if(dx1 < 0)  dx1 = -dx1;
    if(dx1 == 0) dx1 = 1;
    if(dy1 < 0)  dy1 = -dy1;
    if(dy1 == 0) dy1 = 1;
   
    NextEvent.dot.x = (MultiEvent[0].dot[0].x + MultiEvent[1].dot[0].x + 1) / 2;
    NextEvent.dot.y = (MultiEvent[0].dot[0].y + MultiEvent[1].dot[0].y + 1) / 2;
   
    NextEvent.raw.x = 1000;
    if(dx0 > 30)
    {
      dx0 = ((dx1 * 1000) + (dx0/2)) / dx0;
      NextEvent.raw.x = (_KOORD)dx0;
    }
    
    NextEvent.raw.y = 1000;
    if(dy0 > 30)
    {
      dy0 = ((dy1 * 1000) + (dy0/2)) / dy0;
      NextEvent.raw.y = (_KOORD)dy0;
    }
    
    _UDWORD ref = (Event.multindex == 0)? 1 : 0;
    
    _DOT *refdot = &MultiEvent[ref].dot[MultiEvent[ref].dotindex - 1];
    _DOT *actdot = &MultiEvent[Event.multindex].dot[MultiEvent[Event.multindex].dotindex - 1];
    _DOT *lstdot = &MultiEvent[Event.multindex].dot[MultiEvent[Event.multindex].dotindex - 2];
    
    _DWORD v0 = vektor(refdot, lstdot);
    _DWORD v1 = vektor(refdot, actdot);
  
//    NextEvent.dot = Event.dot;
#ifdef DEVSW275
    NextEvent.timestamp = timestamp;
#else
    NextEvent.timestamp = get_abstime();
#endif

    if(v0 > v1)
    {
      NextEvent.ftype    = _EVENT_ZOOM;
      //NextEvent.scancode = 2;
      NextEvent.scancode = (sqroot(v0) - sqroot(v1)) / 2;
      return _TRUE;
    }
    else if(v0 < v1)
    {
      NextEvent.ftype    = _EVENT_ZOOM;
      //NextEvent.scancode = -2;
      NextEvent.scancode = (sqroot(v0) - sqroot(v1)) / 2;
      return _TRUE;
    }
  }

  return _FALSE;
}

_UDWORD IsDotAround(_DOT *d1, _DOT *d2)
{
  _DWORD xx = d2->x - d1->x;
  _DWORD yy = d2->y - d1->y;
 
  xx = (xx < 0)? -xx : xx;
  yy = (yy < 0)? -yy : yy;
  
  if((xx < 3) && (yy < 3))
    return 1;
    
  return 0;
}

#endif
#endif


#ifdef UC_LOGEVENT 

#define LOGEVENT_DPNE "RAM@C:\\EQUEUE.TXT"


void WriteCRLF(_File *file)
{
  _ASCII dat[2];
  
  dat[0] = 13;
  dat[1] = 10;
  file->Write(dat, 2);
}


void WriteSaved(_File *file)
{
  _ASCII dat[300];

  DToA(dat, get_abstime(), 0x107, 1);
  StrCat(dat, 1, " [ ] ...file saved", 1); 
  file->Write(dat, StrLen(dat, 1));
  WriteCRLF(file);
}

void WriteEvent(_File *file, _EVENT *pe)
{
  _ASCII dat[500];

  if(pe->timestamp == 0)
  {
    dat[0] = '-';
    dat[1] = '-';
    dat[2] = '-';
    dat[3] = '-';
    dat[4] = '-';
    dat[5] = '-';
    dat[6] = '-';
    dat[7] = 0;
  } 
  else
  {
    DToA(dat, pe->timestamp, 0x107, 1);
  }
  
  StrCat(dat, 1, " [", 1); 
  DToA(&dat[StrLen(dat, 1)], pe->multindex, 0x800, 1);
  StrCat(dat, 1, "]", 1); 
  
  switch(pe->ftype)
  {
    case _EVENT_NONE:       
      StrCat(dat, 1, " NONE: (", 1);
      DToA(&dat[StrLen(dat, 1)], pe->scancode, 0x800, 1);
      StrCat(dat, 1, ")", 1); 
      break;
    case _EVENT_KEYPRESS:   
      StrCat(dat, 1, " KEY_PRESS: (", 1); 
      DToA(&dat[StrLen(dat, 1)], pe->scancode, 0x800, 1);
      StrCat(dat, 1, ") --> ***", 1); 
      break;
    case _EVENT_KEYRELEASE: 
      StrCat(dat, 1, " KEY_RELEASE: (", 1); 
      DToA(&dat[StrLen(dat, 1)], pe->scancode, 0x800, 1);
      StrCat(dat, 1, ") <-- ***", 1); 
      break;
    case _EVENT_PREHIDPRESS:      // mouse, touch, lightpen pressed
      StrCat(dat, 1, " PREHID_PRESS: (", 1);
      DToA(&dat[StrLen(dat, 1)], pe->dot.x, 0x800, 1);
      StrCat(dat, 1, ",", 1); 
      DToA(&dat[StrLen(dat, 1)], pe->dot.y, 0x800, 1);
      StrCat(dat, 1, ") --> +++", 1); 
      break;
    case _EVENT_HIDPRESS:      // mouse, touch, lightpen pressed
      StrCat(dat, 1, " HID_PRESS: (", 1);
      DToA(&dat[StrLen(dat, 1)], pe->dot.x, 0x800, 1);
      StrCat(dat, 1, ",", 1); 
      DToA(&dat[StrLen(dat, 1)], pe->dot.y, 0x800, 1);
      StrCat(dat, 1, ") --> ###", 1);
      break;
    case _EVENT_HIDMOVE:    
      StrCat(dat, 1, " HID_MOV: (", 1); 
      DToA(&dat[StrLen(dat, 1)], pe->dot.x, 0x800, 1);
      StrCat(dat, 1, ",", 1); 
      DToA(&dat[StrLen(dat, 1)], pe->dot.y, 0x800, 1);
      StrCat(dat, 1, ")", 1); 
      break;
    case _EVENT_HIDRELEASE:    // mouse, touch, lightpen released
      StrCat(dat, 1, " HID_RELEASE: (", 1);
      DToA(&dat[StrLen(dat, 1)], pe->dot.x, 0x800, 1);
      StrCat(dat, 1, ",", 1); 
      DToA(&dat[StrLen(dat, 1)], pe->dot.y, 0x800, 1);
      StrCat(dat, 1, ") <-- ###", 1); 
      break;
    case _EVENT_TRIGGER:    
      StrCat(dat, 1, " TRIGGER: (", 1);
      DToA(&dat[StrLen(dat, 1)], pe->dot.x, 0x800, 1);
      StrCat(dat, 1, ",", 1); 
      DToA(&dat[StrLen(dat, 1)], pe->dot.y, 0x800, 1);
      StrCat(dat, 1, ")", 1); 
      break;
    case _EVENT_USER1:         // userdefined
      StrCat(dat, 1, " USER1:", 1); 
      break;
    case _EVENT_USER2:         // userdefined
      StrCat(dat, 1, " USER2:", 1); 
      break;
    case _EVENT_LASALOS:       // lasalos event
      StrCat(dat, 1, " LASALOS:", 1); 
      break;
    default :
      StrCat(dat, 1, " ???:", 1); 
      break;
  }

  file->Write(dat, StrLen(dat, 1));
  WriteCRLF(file);
}

void _Keyboard::LogEvent(_EVENT *p)
{
  _File file;
  
  if(p == NULL)
  {
    file.Open(SAVE, LOGEVENT_DPNE);
    const _ASCII *ptr = "TIME [MULTIINDEX] EVENTTYPE: (DATA)";
    file.Write(ptr, StrLen(ptr, 1));
    WriteCRLF(&file);
    WriteCRLF(&file);
    file.Close();
    LogTimex = get_abstime();
  }
  else
  {
    _BOOL filesave = _FALSE;
  
    if(p->ftype == _EVENT_NONE)
    {
      LogNoneCnt++;
      if(LogCnt > 0)
      {
        //if(LogNoneCnt == 500)
        if(timediff(LogTimex) > 3000)
        {
          filesave = _TRUE;
        }
      }
    }
    else
    {
      if(LogNoneCnt > 0)
      {
        init_EVENT_Makro(&LogBuf[LogCnt]);
        LogBuf[LogCnt++].scancode = LogNoneCnt;
      }
      LogNoneCnt = 0;
      LogTimex = get_abstime();
      LogBuf[LogCnt++] = *p;
    }
  
    if((filesave == _TRUE)||(LogCnt >= (UC_LOGEVENT-2)))
    {
      if(LogCnt > 0)
      {
        file.Open(LOADSAVE, LOGEVENT_DPNE);
        file.Seek(0, SEEK_END);
    
        _UDWORD i;
        for(i=0; i<LogCnt; i++)
          WriteEvent(&file, &LogBuf[i]);

        WriteSaved(&file);
   
        file.Close();
        LogCnt = 0;
      }
    }
  }
}
#endif


//_EVENT  TriggerEvent;
//_BOOL   TriggerEventState = 0;
//_UDWORD TriggerEventTimex;
//_UDWORD TriggerEventTimexDuration;

_EVENTMEMO *_Keyboard::GetEventMemo(_UDWORD index)
{
  if(index < MAX_MULTIFINGER)
    return &EventMemo[index];
  return NULL;
}

_BOOL _Keyboard::TriggerEventStart(_EVENT *pev, _TIP tip)
{
  _EVENTMEMO *pem = GetEventMemo(pev->multindex);

  if(pem != NULL)
  {
    TriggerEventStop(pev);
    
    if(tip != _TIPSINGLE)
    {
      pem->TriggerEventState     = _TRUE;
      pem->TriggerEventTimex     = get_abstime();
      pem->TriggerEvent          = *pev;
      pem->TriggerEvent.ftype    = _EVENT_TRIGGER;
      pem->TriggerEvent.selfmade = 1;
  
      switch(tip)
      {
        case _TIPCOMBI  : pem->TriggerEventTimexDuration = 500;  // autorepeat 0.5, 0.1
                          break;
        case _TIPCONTI  : pem->TriggerEventTimexDuration =  20;  // continuous rating
                          break;
        case _TIPAUTO   : pem->TriggerEventTimexDuration = 100;  // autorepeat 0.1, 0.1
                          break;
      }
      
      if(TriggerEventValid <= pev->multindex)
        TriggerEventValid = pev->multindex + 1;
        
      return _TRUE;
    }
  }
  
  return _FALSE;
}

#define SA28723

void _Keyboard::TriggerEventRun(_EVENT *pe)
{
//  Debug(TriggerEventValid, 0);

  if(TriggerEventValid > 0)
  {
    if(pe->ftype == _EVENT_HIDRELEASE)
    {
      TriggerEventStop(pe);
    }
   #ifdef SA28723
    else if((pe->ftype == _EVENT_NONE) || (pe->ftype == _EVENT_HIDMOVE)) 
   #else
    else if(pe->ftype == _EVENT_NONE) 
   #endif 
    {
     #if MAX_MULTIFINGER > 0
      for(_UDWORD TEVidx=0; TEVidx<TriggerEventValid; TEVidx++)
     #else
      #define TEVidx 0
     #endif
      {
        _EVENTMEMO *pem = GetEventMemo(TEVidx);
     
        if(pem != NULL)
        {
          if(pem->TriggerEventState == _TRUE)
          {
            _UDWORD tact = get_abstime();
	  	      if((tact - pem->TriggerEventTimex) >= pem->TriggerEventTimexDuration)
	          {
              pem->TriggerEventTimex = tact;
              if(pem->TriggerEventTimexDuration == 500)
                pem->TriggerEventTimexDuration = 100;
          
              *pe = pem->TriggerEvent;
              
              return;
            }
          }
        }
      }
    }
  }
}

void _Keyboard::TriggerEventStop(_EVENT *pev)
{
  _EVENTMEMO *pem;
  _UDWORD    i;

  TriggerEventValid = 0;

  if(pev == NULL)
  {
    for(i=0; i<MAX_MULTIFINGER; i++)
    {
      pem = GetEventMemo(i);
      pem->TriggerEventState = _FALSE;
    }
  }
  else
  {
    pem = GetEventMemo(pev->multindex);
    if(pem != NULL)
      pem->TriggerEventState = _FALSE;
    
    i = MAX_MULTIFINGER;
    while(i)
    {
      i -= 1;
      pem = GetEventMemo(i);
      if(pem->TriggerEventState == _TRUE)
      {
        TriggerEventValid = i+1;
        i = 0; // beenden
      }
    }
  }
}


void _Keyboard::HidPrePressRun(void)
{
  if(Event.ftype == _EVENT_HIDPRESS)
  {
    if(HidPrePressRoom.xy1.x != HidPrePressRoom.xy2.x)
    {
      if((Event.dot.x < HidPrePressRoom.xy1.x)||(Event.dot.x > HidPrePressRoom.xy2.x)||(Event.dot.y < HidPrePressRoom.xy1.y)||(Event.dot.y > HidPrePressRoom.xy2.y))
      {
        return; // event ausserhalb carousel
      }
    }
  }

//  if(Event.ftype == _EVENT_HIDMOVE)
//  {
//    TimexMove = get_abstime();
//  }
//  else if(Event.ftype == _EVENT_HIDRELEASE)
//  {
//    if(timediff(TimexMove) < 300)
//  }

  if(HidPrePressEvent.ftype == _EVENT_HIDPRESS)
  {
    if(Event.ftype == _EVENT_HIDMOVE)
    {
      _KOORD dx = HidPrePressEvent.dot.x - Event.dot.x;
      if((dx < -20)||(dx > 20))
      {
        HidPrePressEvent.ftype = _EVENT_NONE; // eat prepress
        return;
      } 
      _KOORD dy = HidPrePressEvent.dot.y - Event.dot.y;
      if((dy < -20)||(dy > 20))
      {
        HidPrePressEvent.ftype = _EVENT_NONE; // eat prepress
        return;
      } 
    }
    else if(Event.ftype != _EVENT_NONE)
    {
      NextEvent = Event;
      Event     = HidPrePressEvent;
      HidPrePressEvent.ftype = _EVENT_NONE; // eat prepress
      return;
    }
    
    _UWORD tix = (_UWORD)get_abstime();
    if((tix - HidPrePressEvent.modifier) > UC_FLICK_HID_DELAY)
    {
      NextEvent = Event;
      Event     = HidPrePressEvent;
      HidPrePressEvent.ftype = _EVENT_NONE; // eat prepress
      return;
    }
  }
  else if(Event.ftype == _EVENT_HIDPRESS)
  {
    HidPrePressEvent = Event;
    HidPrePressEvent.modifier = (_UWORD)get_abstime();
    Event.ftype = _EVENT_PREHIDPRESS;
  }
}

void _Keyboard::GetEvent(_EVENT *p)
{
  if(NextEvent.ftype != _EVENT_NONE)
  {
    Event = NextEvent;
    NextEvent.ftype = _EVENT_NONE;
  }
  else
  {
    DoAbsolutKeyboard();
  }
  
  if(TriggerEventValid > 0)
    TriggerEventRun(&Event); // ggf.erzeuge triggersignal für reperaterfunct bei buttons, softkeys und scrollbars

  if(HidPrePressValid != _FALSE)
    HidPrePressRun();

 #ifdef UC_LOGEVENT
  LogEvent(&Event);
 #endif  

#ifdef UC_MULTITOUCH
#ifdef UC_FLICK_ZOOM
  if(Event.ftype == _EVENT_HIDPRESS)
  {
    MultiEvent[Event.multindex].dot[0]    = Event.dot;
    MultiEvent[Event.multindex].timestamp = Event.timestamp;
    MultiEvent[Event.multindex].dotindex  = 1;
  }
  else if(Event.ftype == _EVENT_HIDMOVE)
  {
    if(MultiEvent[Event.multindex].dotindex != 0)
    {
      _UDWORD i = MultiEvent[Event.multindex].dotindex;
      _DOT *lastdot = &MultiEvent[Event.multindex].dot[i-1];
      
      if(!IsDotAround(lastdot, &Event.dot))
      {
        if(i == TOUCH_BUFFER_SIZE)
        {
          MemMove(&MultiEvent[Event.multindex].dot[1], &MultiEvent[Event.multindex].dot[2], (TOUCH_BUFFER_SIZE-2) * sizeof(_DOT));
          MultiEvent[Event.multindex].dot[TOUCH_BUFFER_SIZE-1] = Event.dot;
        }
        else
        {
          MultiEvent[Event.multindex].dot[i] = Event.dot;
          MultiEvent[Event.multindex].dotindex += 1;
        }
        
        if(Event.multindex < 2) // funktioniert nur mit den ersten 2 fingern
        {
          FlickZoom(Event.timestamp);
        }
      }
    }
  }
  else if(Event.ftype == _EVENT_HIDRELEASE)
  {
    if(MultiEvent[Event.multindex].dotindex != 0)
    {
   //   MultiEvent[Event.multindex].dot[MultiEvent[Event.multindex].dotindex] = Event.dot;
   //   MultiEvent[Event.multindex].timestamp = Event.timestamp;
   //   FlickPgUpDown();
      MultiEvent[Event.multindex].dotindex = 0;
    }
  }
#endif  
#endif
  
  
  if(HideEventDueWindowClosed != 0)     
  {
    // sa28695
    if((Event.ftype != _EVENT_HIDMOVE) && (Event.ftype != _EVENT_NONE))
    {
      HideEventDueWindowClosed = 0;
      if(Event.ftype == _EVENT_HIDRELEASE)
      {
        Event.ftype = _EVENT_NONE; // event not valid
      }
    }
  }
  
  
  *p = Event;
}

#ifdef USER_LISEC
 #define XXX_DELBUFF
#endif
#ifdef USER_KM
 #define XXX_DELBUFF
#endif
void _Keyboard::DelBuffer(void)
{
  TriggerEventStop(NULL);
  
 #ifdef UC_RELEASE_PRESS_FILTER
  RP_Memo.eventType = event_NONE;
 #endif  
  
 #ifdef XXX_DELBUFF
  _EVENT tevent;
	_BOOL  state = _FALSE;

  while(DoAbsolutKeyboard())
	{
   #ifdef USER_KM
    if((Event.ftype == _EVENT_KEYRELEASE)||(Event.ftype == _EVENT_HIDRELEASE))
   #else
    if(Event.ftype == _EVENT_KEYRELEASE)
   #endif
		{
		  tevent = Event;
			state = _TRUE;
    }
  }

  if(state == _TRUE)
	{
		AddEventEx(&tevent, _TRUE);
	}
 #else
  _EVENT stack[40];
  _UWORD i, cnt = 0;
  while(DoAbsolutKeyboard())
  {
    if(Event.ftype == _EVENT_LASALOS)
    {
      if(cnt < 40)
      {
        stack[cnt] = Event;
        cnt ++;
      }
    }
  }
  
  for(i=0; i<cnt; i++)
  {
		AddEventEx(&stack[i], _TRUE);
  }
  
 #endif
}

_UDWORD _Keyboard::KillAllTouchEvents(void)
{
  _EVENT stack[50];
  _UWORD i, retcode;
  
  retcode = 0;
  
  TriggerEventStop(NULL);
  
 #ifdef UC_RELEASE_PRESS_FILTER
  RP_Memo.eventType = event_NONE;
 #endif  
  
  while(DoAbsolutKeyboard())
  {
    if((Event.ftype != _EVENT_HIDPRESS) && (Event.ftype != _EVENT_HIDMOVE) && (Event.ftype != _EVENT_HIDRELEASE))
    {
      if(retcode < ARRAY_SIZE(stack))
      {
        stack[retcode] = Event;
        retcode ++;
      }
    }
  }
  
  for(i=0; i<retcode; i++)
  {
		AddEventEx(&stack[i], _TRUE);
  }
  
  return retcode;
}

void _Keyboard::StartStopEvent(_BOOL start, _UWORD mask)
{

  if(start == _TRUE)
  {
    KillMask &= mask ^ 0xFFFF;
  }
  else
  {
    KillMask |= mask;
  }

}

void _Keyboard::LookUpDiagnostic(_EVENT *pe)
{
  _BOOL st = _FALSE;

  if(SecretTime != 0)
  {
    if(timediff(SecretTime) > 2000)
    {
      SecretTime = 0;
    }
    else
    {
      st = UnderCover.Run(pe->scancode & 0xFF);
      SecretTime = get_abstime();
    }
  }

  if(pe->scancode == (_ESC|(_ALT<<8)))
  {
    st = _TRUE;
    if(SetUp.GetDisableAltEsc() == _TRUE)
    {
      st = _FALSE;
      SecretTime = get_abstime();
      UnderCover.Init();
    }
  }

  if(st == _TRUE)
  {
    Root.FindTop()->AddCmd(CMD_DIAGNOSTIC, 0, 0, pe->lrm);
    pe->ftype  = _EVENT_NONE;                               // eat event
    SecretTime = 0;
  }
}

// interface for st *********************************************************

_BOOL AddEvent(_EVENT *pe)
{
  return Keyboard.AddEventEx(pe, _FALSE);
}

extern "C" void  StartStopEvent(_BOOL start, _UWORD mask)
{
  Keyboard.StartStopEvent(start, mask);
}

_UDWORD SetRP_DelayTime(_UDWORD ms_time)
{
  return Keyboard.SetRP_DelayTime(ms_time);
}

extern "C" _BOOL IsCtrlKeyPress(_EVENT *pev, _UBYTE character)
{
  if((pev->modifier & _CTRL) && (pev->ftype == _EVENT_KEYPRESS))
  {
    if(OpSysIsSalamander())
    {
      if(character == *(_UBYTE*)&pev->scancode)
        return _TRUE;
    }
    else
    {
      switch(character)
      {
        case 'c': return (pev->scancode == 1027)? _TRUE : _FALSE;
        case 'v': return (pev->scancode == 1046)? _TRUE : _FALSE;
     //   case 'x': return (pev->scancode == 1048)? _TRUE : _FALSE;
      }
    }
  }
  return _FALSE;
}

extern "C" _BOOL GetPossibleHidReleaseEvent(_EVENT *pev, _HID_BUTTON button, _UDWORD multindex)
{
  _EVENTMEMO *pem = Keyboard.GetEventMemo(multindex);
  _BOOL      retcode = _FALSE;
  
  if(pem != NULL)
  {
    if(((pem->PressMemoLeft == _TRUE) && (button == _HID_LEFT)) ||
       ((pem->PressMemoRight == _TRUE) && (button == _HID_RIGHT)))
    {
      if(pev != NULL)
      {
        init_EVENT(pev);
        pev->ftype     = _EVENT_HIDRELEASE;
        pev->multindex = multindex;
        pev->timestamp = get_abstime();
        pev->button    = button;
        pev->dot       = (button == _HID_LEFT)? pem->HidMemoLeft : pem->HidMemoRight;
        //pev->user      = pem->TriggerEvent.user;
        //pev->selfmade  = pem->TriggerEvent.selfmade;
        //pev->source    = pem->TriggerEvent.source;
      }
      retcode = _TRUE;
    }
  }
  
  return retcode;
}


extern "C" void TouchDeviceLock(_UDWORD device, _BOOL state)
{
  Keyboard.TouchDeviceLock(device, state);
}