#include "T_IPC.H"
#include "T_NUC.H"

#define CNT_WITHOUT_PAUSE 13

void _Picture::Init(void)
{
  Cursor.Init();
  RefCyc = 0;
  init_MY_PICTURE(&Pic);
  DoAutomaticSeekInput = _FALSE;
  AutomaticSeekTimer = 0;
 #ifdef UC_TOUCHBUBBLE_BY_SELECTION
  Init_BubbleState();
 #endif
  RecalcIoID = _FALSE;
  init_DOT(&RecalcIoIDot);
}

void _Picture::Free(void)
{
  Keyboard.TriggerEventStop(NULL); // ggf alle gestarteten trigger beenden
  free_MY_PICTURE(&Pic);
  Cursor.Free();
  RefCyc = 0;
  DoAutomaticSeekInput = _FALSE;
  AutomaticSeekTimer = 0;
 #ifdef UC_TOUCHBUBBLE_BY_SELECTION
  Init_BubbleState();
 #endif
  RecalcIoID = _FALSE;
  init_DOT(&RecalcIoIDot);
//  Init(); speed
}

void _Picture::SetRecalcIoID(_BOOL st, _KOORD x, _KOORD y)
{
  RecalcIoID     = st;
  RecalcIoIDot.x = x;
  RecalcIoIDot.y = y;
}

#ifdef UC_TOUCHBUBBLE_BY_SELECTION
void _Picture::Init_BubbleState(void)
{
  BubbleState = 0;
  BubbleInput = 0xFFFF;
  TimeDiff = 0;
}
#endif

void _Picture::AccessChanged(void)
{
 #ifdef FIRSTINPUT_ALT
  if(IsEditorOpen() == _FALSE)
    AutomaticSeekInput(_FALSE);
 #endif // FIRSTINPUT_ALT
}

_BOOL _Picture::AutomaticSeekInput(_BOOL just_welcome_austria)
{
  _BOOL retcode = _FALSE;

  if((Pic.FirstInput.x >= 0) && (Pic.FirstInput.y >= 0)) // first input abfragen
  {
    retcode = SeekHid(&Pic.FirstInput, _TRUE, _FALSE, _SELECTOR_DEFAULT, NULL);
  }

  if((retcode == _FALSE) && (just_welcome_austria == _FALSE))
  {
    retcode = _TRUE;
    if(SetUp.GetAutomaticSeekFirstInput() == _TRUE)
    {
      _EVENT event;
      init_EVENT_Makro(&event);
      event.ftype    = _EVENT_KEYPRESS;
      event.scancode = _RIGHT;

      // suche von automatischem First Input ist wie suche mit Cursortaste
      if(Seek(&event) == _FALSE)
      {
        event.scancode = _DOWN;
        Seek(&event);
      }
    }
  }

  return(retcode);
}



void _Picture::ReadyIntern(_BOOL firstdraw)
{

 #ifdef OK_PICMEMO
  _UWORD  no;
  _WHOAMI wai;
  if((Pic.P_Window != NULL)&&(firstdraw == _TRUE))
  {
//    if(MemoPool.GetLastInput(&GetWhoAmI(), &no) == _TRUE)
    wai = GetWhoAmI();
    if(MemoPool.GetLastInput(&wai, &no) == _TRUE)
      Pic.P_Window->AddCmd(CMD_EDITNO, no, -1, _FALSE);
  }
 #endif

  // kontrolle ob FirstInput oder Setup.AutoSeekInput aktiviert ist
 #ifdef FIRSTINPUT_ALT
  if((Pic.P_Window != NULL)&&(IsEditorTrue(Pic.Editor.actno) == _FALSE))
  {
    DoAutomaticSeekInput = _TRUE;
    AutomaticSeekTimer   = get_abstime();
  }
 #else // FIRSTINPUT_ALT
  DoAutomaticSeekInput = SetUp.GetAutomaticSeekFirstInput();
  if((Pic.FirstInput.x >= 0) && (Pic.FirstInput.y >= 0))
  {
    DoAutomaticSeekInput = _TRUE;
  }
 #endif // FIRSTINPUT_ALT

  if(firstdraw == _TRUE)
  {
    do_LSEFUNCT_lrm(&Pic.StartFunct, Pic.P_Window, _FALSE);
  }
}

void _Picture::EndIntern(_Window *Pw)
{

  do_LSEFUNCT_lrm(&Pic.EndFunct, Pw, _FALSE); //Pic.P_Window);
}


_FRAMETYPE _Picture::GetBackgroundTheme(_ROOM *proom)
{
  if(Pic.Ptr != NULL)
    return Pic.Ptr->GetBackgroundTheme(&Pic.Room);

  return _DEFFRAME;
}

void _Picture::Draw(_DOT *offset, _BOOL draw_print)
{
  _UWORD i;
  _ROOM  room;
  _ZOOM  zoom;

  init_ZOOM(&zoom);
  room = Pic.Room;
  Zoom_ROOM(&room, offset, &zoom);

 #ifdef UC_MOUSE
  HideMouseCursor();
 #endif

  ScreenStop();  // bildschirm ausschalten

 #ifdef THR
  if(Pic.P_Window->GetShadow() == _FALSE)
    Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY|T_SOLID, MakeColor_Makro(BLACK, BLACK));
 #else
  if(SetUp.GetDeleteBackground() == _TRUE)
  {
   #ifndef UC_DELETE_WINDOWBACKGROUND
    _WHOAMI whoami = GetWhoAmI();
    if((whoami.ima != IMA_WINDOW) &&
       (whoami.ima != IMA_POPUP)  &&
       (whoami.ima != IMA_TOUCHEDITOR) &&
       (whoami.ima != IMA_OFF_PICTURE) &&
       (whoami.ima != IMA_OFF_WINDOW))
   #endif
    {
      Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY|T_SOLID, SetUp.GetBackgroundColor());
    }
  }

  if(Pic.P_Window != NULL)
  {
    if(Pic.P_Window->GetShadow() == _TRUE)
    {
      DrawShadow(room, Pic.P_Window->GetBackgroundTheme(&Pic.Room));
    
//      _FRAMETYPE sframetype = _DEFFRAME;
//     #ifdef UC_THEME
//      if(Pic.Ptr != NULL)
//	      sframetype = Pic.Ptr->GetBackgroundTheme(&Pic.Room);
//     #endif
//      DrawShadow(room, sframetype);
    }
  }
 #endif

// #ifdef UC_ROTATE
//  RefreshRectangle(&room);
// #endif

 #ifndef NINETEEN
  if(draw_print == _FALSE)
  {
    set_keymode(0xFFFFFFFF, _TIPCOMBI);            // set all keymodes to default
    Hotkey.SetKeyMode();                                   // set global keymodes
  }
 #endif

  if((DrawBackGround == 0)&&(draw_print == _FALSE))
  {
    DrawXPicture += 1;
    for(i=0; i<Pic.No; i++)
    {
      Pic.Ptr[i].Draw(offset, &Pic.PicZoom, NULL);
    }
    DrawXPicture -= 1;
    lookup_SEEKELEMENT(&Pic, 0, 0);
    ShowEditor();
  }
  else
  {
    for(i=0; i<Pic.No; i++)
      Pic.Ptr[i].Draw(offset, &Pic.PicZoom, NULL);
  }
  
  Pic.Actual.actrefresh_cnt = CNT_WITHOUT_PAUSE; // nach bildaufbau "längere zeit" keine verschnaufpause einlegen
  ScreenStart();  // bildschirm einschalten

 #ifdef UC_ROTATE
  RefreshRectangle(&room);
 #endif

 #ifdef UC_MOUSE
  ShowMouseCursor();
 #endif
}

_BOOL _Picture::Load(_ASCII *dpne, _Softkey *Softkey, _BOOL full, _BOOL loadjustsoftkey, _UWORD picnox)
{
  _File  File;
  _BOOL  retcode = _FALSE;

  File.Open(LOAD, dpne, 0, _TRUE);
  if(File.ReadHeader() == _TRUE)
  {
   #if COMPILER_VERSION >= 22
    _ROOM scrroom;
    
    File.Read_DOT(&Pic.FirstInput);
    MyGetScreenSize(&scrroom);
    if((Pic.FirstInput.x < scrroom.xy1.x) || (Pic.FirstInput.x >= scrroom.xy2.x) ||
       (Pic.FirstInput.y < scrroom.xy1.y) || (Pic.FirstInput.y >= scrroom.xy2.y))
    {
      Pic.FirstInput.x = -1;
      Pic.FirstInput.y = -1;
    }
   #endif

   #if COMPILER_VERSION >= 24
    Pic.TouchEditorNumeric = (_BOOL)File.Read_ENUM();
    Pic.TouchEditorAscii   = (_BOOL)File.Read_ENUM();
    Pic.DrawShadow         = (_BOOL)File.Read_ENUM();
   #endif

   #if COMPILER_VERSION >= 28
    File.Read_LSEFUNCT(&Pic.StartFunct);
    File.Read_LSEFUNCT(&Pic.EndFunct);
   #endif

   #if COMPILER_VERSION >= 33
    Pic.GlobalHotkeys = (_BOOL)File.Read_ENUM();
   #endif

    if(File.GetError() == _FALSE)
    {
      if(Softkey->Read(&File) == _TRUE)
      {
        if(loadjustsoftkey == _TRUE)
        {
          retcode = _TRUE;
        }
        else
        {
          retcode = Read(&File, full, picnox);
        }
      }
    }
  }
  File.Close();

  return(retcode);
}

_BOOL _Picture::LoadPicture(_ASCII *dpne, _UWORD no, _Window *Pw, _BOOL loadjustsoftkeys, _UWORD picnox)
{
  _BOOL  retcode = _FALSE;

//  Init();

  if(no >= 60000)
  {
    if(Memory2((void**)&Pic.Ptr, sizeof(_SubPicture)) != 0)
    {
      Pic.No = 1;
      Pic.Ptr[0].Init();
      //retcode = _TRUE; //sa20810: no errormessage, system goes on like nothing happens
      return _FALSE; //sa20810: no errormessage, system goes on like nothing happens
    }
  }
  else
  {
    retcode = Load(dpne, Pw->GetSoftkey(), _TRUE, loadjustsoftkeys, picnox);
  
    if(retcode == _FALSE)
    {
      _WHOAMI        twai;
      _NEWSCREENPROP nsp;
        
      init_NEWSCREENPROP(&nsp);
      setup_WHOAMI(&twai, IMA_PICTURE, no);
      
      MyGetScreenSize(&nsp.Position);
      if(_Screen_Create(&nsp, &twai) == _TRUE)
      {
        if(Memory2((void**)&Pic.Ptr, sizeof(_SubPicture)) != 0)
        {
          Pic.No = 1;
          Pic.Ptr[0].Init();
          MyGetScreenSize(&nsp.Position);
          
          _PICTURE *pp           = Pic.Ptr[0].GetPICTURE();
          pp->mask.room          = nsp.Position;
          Pic.Room               = nsp.Position;
          Pic.FirstInput         = nsp.FirstInput;
          Pic.TouchEditorNumeric = nsp.TouchEditorNumeric;
          Pic.TouchEditorAscii   = nsp.TouchEditorAscii;
          Pic.DrawShadow         = _FALSE;
          Pic.GlobalHotkeys      = _TRUE;
          
          _STYLESHEET stylesheet;
          init_STYLESHEET(&stylesheet);
          setup_STYLESHEET(&stylesheet, NULL, nsp.FrameId, T_LEFTBOUND | T_UPBOUND, nsp.ColorBack, nsp.ColorBack, nsp.ColorHatch, nsp.ColorTopLeft, nsp.ColorBottomRight, NULL, NULL, NULL, NULL, NULL);
          add_rectangle(&Pic, &stylesheet, &nsp.Position, DEFSCOPE);
          retcode = _TRUE;
        }
      }
    }
  }

  if(retcode == _TRUE)
  {
    Pic.P_Window = Pw;
   #ifdef XUSER_KM
    _PRJ_ENTRY *prje = Project.GetEntryByNo(no, _PE_PICTURE);
    if(prje != NULL)
      Pw->GetSoftkey()->SetAssignedImage(prje->imageno);
   #endif
  }
  else
  {
    SystemError(CANT_LOAD_PICTURE, no);
  }

  return(retcode);
}

_BOOL _Picture::LoadWindow(_ASCII *dpne, _UWORD no, _Window *Pw)
{
  _BOOL    retcode = _FALSE;
  _BOOL    stdwin = _FALSE;
  _PICTURE *pp;
  _ROOM    sroom;
  _KOORD   xx, yy;

  if(no < 60000)
  {
    retcode = Load(dpne, Pw->GetSoftkey(), _FALSE, _FALSE, 0);
    if(retcode == _FALSE)
    {
      switch(no)
      {
        case UC_NEWBUBBLEWINDOW :
        case UC_CONFIRMWINDOW : 
        case UC_OFFLINEWINDOW :
        case UC_RESETWINDOW   : stdwin = _TRUE; break;
      }
      
      if(stdwin == _FALSE)
      {
        _WHOAMI        twai;
        _NEWSCREENPROP nsp;
        
        init_NEWSCREENPROP(&nsp);
        setup_WHOAMI(&twai, IMA_WINDOW, no);
        if(_Screen_Create(&nsp, &twai) == _TRUE)
        {
          if(Memory2((void**)&Pic.Ptr, sizeof(_SubPicture)) != 0)
          {
            Pic.No = 1;
            Pic.Ptr[0].Init();
          
            pp                     = Pic.Ptr[0].GetPICTURE();
            pp->mask.room          = nsp.Position;
            Pic.Room               = nsp.Position;
            Pic.FirstInput         = nsp.FirstInput;
            Pic.TouchEditorNumeric = nsp.TouchEditorNumeric;
            Pic.TouchEditorAscii   = nsp.TouchEditorAscii;
            Pic.DrawShadow         = nsp.DrawShadow;
            Pic.GlobalHotkeys      = nsp.GlobalHotkeys;
          
            _STYLESHEET stylesheet;
            init_STYLESHEET(&stylesheet);
            
            setup_STYLESHEET(&stylesheet, NULL, nsp.FrameId, T_LEFTBOUND | T_UPBOUND, nsp.ColorBack, nsp.ColorBack, nsp.ColorHatch, nsp.ColorTopLeft, nsp.ColorBottomRight, NULL, NULL, NULL, NULL, NULL);
            
            sroom.xy1.x = 0;
            sroom.xy1.y = 0;
            sroom.xy2.x = nsp.Position.xy2.x - nsp.Position.xy1.x;
            sroom.xy2.y = nsp.Position.xy2.y - nsp.Position.xy1.y;
            
            add_rectangle(&Pic, &stylesheet, &sroom, DEFSCOPE);
            retcode = _TRUE;
          }
        }
      }
    }
  }

  if((no >= 60000) || (stdwin == _TRUE))
  {
    if(Memory2((void**)&Pic.Ptr, sizeof(_SubPicture)) != 0)
    {
      Pic.No = 1;
      Pic.Ptr[0].Init();

      if(Pic.Ptr[0].MakeStandardWindow(no) == _TRUE)
      {
        //Pic.DrawShadow  = _FALSE;
        Pic.JustProgram = _TRUE;
        Pic.Room        = Pic.Ptr[0].GetPICTURE()->mask.room;
        retcode         = _TRUE;
      }

      if(retcode == _FALSE)
      {
        if(stdwin == _TRUE)
        {
          retcode = _TRUE;
        
          MyGetScreenSize(&sroom);
          xx = sroom.xy2.x - sroom.xy1.x + 1;
          yy = sroom.xy2.y - sroom.xy1.y + 1;

          if(xx <= 320)
            xx = xx / 9;
          else
            xx = xx / 6;

          if(yy <= 240)
            yy = yy / 5;
          else
            yy = yy / 3;

          sroom.xy1.x += xx;
          sroom.xy1.y += yy;
          sroom.xy2.x -= xx;
          sroom.xy2.y -= yy;

          Pic.JustProgram = _TRUE;

          pp = Pic.Ptr[0].GetPICTURE();
          pp->mask.room = sroom;
          Pic.Room      = sroom;
        }
        else
        {
          return _FALSE; // sa20810: no errormessage, system goes on like nothing happens
        }
      }
    }
  }

  if(retcode == _TRUE)
  {
    Pic.P_Window = Pw;
   #ifdef XUSER_KM
    _PRJ_ENTRY *prje = Project.GetEntryByNo(no, _PE_WINDOW);
    if(prje != NULL)
      Pw->GetSoftkey()->SetAssignedImage(prje->imageno);
   #endif
  }
  else
  {
    SystemError(CANT_LOAD_WINDOW, no);
  }

  return(retcode);
}

_BOOL _Picture::Read(_File *File, _BOOL full, _UWORD picnox)
{
  _BOOL   retcode = _FALSE;
  _UDWORD size;
  _UWORD  i, tmp;

//  Init();

  if(File->Read(&tmp, sizeof(_UWORD)) == _TRUE)      // load no of subpictures
  {
    size = tmp * sizeof(_SubPicture);
    if(File->FileMemory((void**)&Pic.Ptr, size) == _TRUE)
    {
      Pic.No  = tmp;
      retcode = _TRUE;

     #ifdef ZOOM_KM
      _UDWORD oz = DoZoomKM;
     #endif

      for(i=0; i<Pic.No; i++)
      {
        tmp --;
        Pic.Ptr[tmp].Init();
        if(retcode == _TRUE)
        {
         #ifdef ZOOM_KM
          if((tmp == 0)&&(Pic.No > 1))
            DoZoomKM += 1;
          #ifdef NINETEEN
           if(picnox == NINETEEN)
             DoZoomKM = 1;
          #endif
         #endif

          if(Pic.Ptr[tmp].Read(File) == _FALSE)
          {
            retcode = _FALSE;
            Pic.Ptr[tmp].Free();
          }
        }
      }

     #ifdef ZOOM_KM
      DoZoomKM = oz;
     #endif
    }
  }

  if(retcode == _FALSE)
  {
    Free();
  }
  else
  {
    CalculateSize(full);
    retcode = _TRUE;
  }

  return(retcode); 
}

void _Picture::CalculateSize(_BOOL full)
{
  _ROOM  tmp; 
  _UWORD i; 

  MyGetScreenSize(&Pic.Room);

  linear_coordinates(&Pic.Room.xy1.x, &Pic.Room.xy1.y, &Pic.Room.xy2.x, &Pic.Room.xy2.y);

  if(full == _FALSE)
  {
    tmp.xy1      = Pic.Room.xy1; 
    Pic.Room.xy1 = Pic.Room.xy2;  
    Pic.Room.xy2 = tmp.xy1;  

    for(i=0; i<Pic.No; i++)
    {
      Pic.Ptr[i].GetSize(&tmp); 
      linear_coordinates(&tmp.xy1.x, &tmp.xy1.y, &tmp.xy2.x, &tmp.xy2.y); 
      if(tmp.xy1.x < Pic.Room.xy1.x) Pic.Room.xy1.x = tmp.xy1.x;
      if(tmp.xy1.y < Pic.Room.xy1.y) Pic.Room.xy1.y = tmp.xy1.y;
      if(tmp.xy2.x > Pic.Room.xy2.x) Pic.Room.xy2.x = tmp.xy2.x; 
      if(tmp.xy2.y > Pic.Room.xy2.y) Pic.Room.xy2.y = tmp.xy2.y; 
    }
  }
}

void _Picture::GetSize(_ROOM *room, _DOT *offset)
{
  room->xy1.x = Zoom(Pic.PicZoom.x, Pic.Room.xy1.x) + offset->x; 
  room->xy1.y = Zoom(Pic.PicZoom.y, Pic.Room.xy1.y) + offset->y; 
  room->xy2.x = Zoom(Pic.PicZoom.x, Pic.Room.xy2.x) + offset->x;
  room->xy2.y = Zoom(Pic.PicZoom.y, Pic.Room.xy2.y) + offset->y;
}

void _Picture::PictureRefresh(_DOT *offset)
{
  if(Pic.Actual.actrefresh_cnt == 0)
  {
    // verschnaufpause
    Pic.Actual.actrefresh_cnt = 3; 
  }
  else
  {
    // note dekrementieren, weil beim bildaufschlagen dieser wert auf CNT_WITHOUT_PAUSE gesetz 
    // wird um anschliessend "längere zeit" keine verschnaufpause zu erwirken
    Pic.Actual.actrefresh_cnt--; 
    
   #ifdef _CHANGELOOKUP
    if(GlobalChangeSys)
    {
      for(Pic.Actual.actlayer=0; Pic.Actual.actlayer<Pic.No; Pic.Actual.actlayer++)
      {
        Pic.Ptr[Pic.Actual.actlayer].Refresh(Pic.Editor.actno, offset, &Pic.PicZoom, this);
      }
    }
    else
   #endif
    {
      if(Pic.Actual.actlayer >= Pic.No)
      {
        Pic.Actual.actlayer = 0;
      }
      
      if(Pic.Actual.actlayer < Pic.No)
      {
        Pic.Ptr[Pic.Actual.actlayer].Refresh(Pic.Editor.actno, offset, &Pic.PicZoom, this);
      }

      Pic.Actual.actlayer ++;
    }
  }
}

_BOOL _Picture::Seek(_EVENT *pe)
{
  _BOOL     retcode = _FALSE;
  _UWORD    no_i, no_s, i;
  _DOT      offset;
  _ZOOM     zoom;
  _SELECTOR selector = _SELECTOR_ONLY_KBRD;

 #ifdef OK_SEEK_BUTTON
  // wenn editor auf einem momentary Button steht und dieser gedrückt ist darf der cursor nicht versetzt werden
  _INPUT *pi;
  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
    if((Pic.Editor.input.io.typ == AS_INPUTBUTTON)||(Pic.Editor.input.io.typ == AS_INPUTBUTTON_WITOUT_CURSOR))
    {
      pi = p_input_SEEKELEMENT(&Pic, Pic.Editor.actno, _TRUE);
      if(pi != NULL)
      {
        if(pi->io.xuser == 1)
        {
          return(_FALSE);
        }
      }
    }
  }
 #endif

  if(pe->ftype == _EVENT_KEYPRESS)
  {
    if(Pic.Editor.actlayer < Pic.No)
    {
      no_s = Pic.Editor.actlayer;
      no_i = Pic.Editor.actno;

      if(IsEditorTrue(no_i) == _FALSE)
      {
        i = Pic.No;
        while(i)
        {
          i --;
          if(is_any_input_accessible(&Pic, _SELECTOR_ONLY_KBRD) == _TRUE)
          {
            no_s = i;
            i = 0;
          }
        }
      }

      init_ZOOM(&zoom);
      if(Pic.P_Window != NULL)
      {
        offset = Pic.P_Window->GetPicOffset();
      }
      else
      {
        init_DOT(&offset);
        offset.x = 0;
        offset.y = 0;
      }

      switch(pe->scancode)
      {
#ifndef UC_NO_TAB_CURSORNAVIGATION      
        case _TAB   : retcode = SeekTab(&no_s, &no_i, get_seek_algorithm(), &offset, &zoom, selector);
                      break;
#endif
        case _LEFT  : retcode = seek_horizontal(&no_i, &Pic, no_s, _FALSE, no_i, get_seek_algorithm(), &offset, &zoom, selector);
                      break;
        case _RIGHT : retcode = seek_horizontal(&no_i, &Pic, no_s, _TRUE, no_i, get_seek_algorithm(), &offset, &zoom, selector);
                      break;
        case _UP    : retcode = seek_vertical(&no_i, &Pic, no_s, _FALSE, no_i, get_seek_algorithm(), &offset, &zoom, selector);
                      break;
        case _DOWN  : retcode = seek_vertical(&no_i, &Pic, no_s, _TRUE, no_i, get_seek_algorithm(), &offset, &zoom, selector);
                      break;
      }
    }
  }
  else if(pe->ftype == _EVENT_SHUTTLEINC)
  {
    no_i = Pic.Editor.actno;
    retcode = seek_shuttle(&no_i, &Pic, _TRUE, no_i, &offset, &zoom, selector);
  }
  else if(pe->ftype == _EVENT_SHUTTLEDEC)
  {
    no_i = Pic.Editor.actno;
    retcode = seek_shuttle(&no_i, &Pic, _FALSE, no_i, &offset, &zoom, selector);
  }

  if(retcode == _TRUE)
  {
    Pic.P_Window->AddCmd(CMD_EDITNO, no_i, -1, pe->lrm);
  }

  return(retcode);
}

#ifdef HID
#ifdef UC_THEME
_BOOL _Picture::SeekThemeCancle(_DOT *pd, _DOT offset, _ZOOM *zoom, _BOOL press)
{
  _UWORD i;
  _BOOL  retcode = _FALSE;

  for(i=0; i<Pic.No; i++)
  {
    if(Pic.Ptr[i].SeekThemeCancle(pd, offset, zoom, press) == _TRUE)
    {
      retcode = _TRUE;
      break;
    }
  }

  return(retcode);
}
#endif
#endif

_BOOL _Picture::SeekHid(_DOT *pd, _BOOL welcome, _BOOL lrm, _SELECTOR selector, _EVENT *pev)
{
  _BOOL        retcode = _FALSE;
  _UWORD       old_actno, i;
  _ROOM        room;
  _DOT         offset;
  _ZOOM        zoom;
  _INPUT       *pi;
  _LSEVARINFO  *pvi;
  _VARINFO     varinfo;
  _BOOL        state;


  for(i=0; i<Pic.memseekelement.no; i++)
  {
    init_ZOOM(&zoom);
    offset = Pic.P_Window->GetPicOffset();

//old_actno = Pic.Editor.actno;

    if(position_SEEKELEMENT(&room, &Pic, i, &offset, &zoom) == _TRUE)
    {
      if((pd->x >= room.xy1.x)&&(pd->x <= room.xy2.x)&&(pd->y >= room.xy1.y)&&(pd->y <= room.xy2.y))
      {
        if(access_SEEKELEMENT(&Pic, i, _FALSE, selector) == _TRUE)
        {
        
         #ifdef UC_BUBBLE_HELP 
          if(GetLseServer(LSE_ACTIVATEBUBBLEHELP))
          {
            pi = p_input_SEEKELEMENT(&Pic, i, _FALSE);
            if(pi != NULL)
            {
              NewBubbleHelp.Call(pi->io.space.xy1.x, pi->io.space.xy1.y, &pi->io.newbubbletext, &pi->io.variable);
              return _FALSE;
            }        
          }        
         #endif // UC_BUBBLE_HELP
//if(old_actno != i) // nur wenn neue eingabe selektiert wurde, für demo "ZoomableImage"
//{
          old_actno = Pic.Editor.actno;

         #ifndef USER_BILLION
          Pic.P_Window->AddCmd(CMD_KILLEDITOR, 0, 0, lrm);
         #endif
          if(welcome == _TRUE)
          {
            Pic.P_Window->AddCmd(CMD_EDITNO, i, old_actno, lrm);
          }
         #ifdef UC_TOUCHBUBBLE_BY_SELECTION
          else
          {
            pi = p_input_SEEKELEMENT(&Pic, i, _FALSE);
            if(pi != NULL)
            {
              //if(chk_VARLABEL_Makro(&pi->io.variable) == _TRUE)
              if(chk_VARLABEL_Makro(&pi->io.variable)) // Makro, speed: nicht auf true oder false abfragen
              {
                if(VarList.GetVarInfo(&pvi, pi->io.variable.info[0].value) == _TRUE)
                {
                  if((pvi->xmemtext.xxno == 4) && (pvi->xmemtext.xxptr[3].anything.no != 0xFFFF))
                  {
                    Pic.P_Window->AddCmd(CMD_EDITNO, i, old_actno, lrm);
                    BubbleState = 1;
                    BubbleInput = i;
                    TimeDiff = get_abstime();
                  }
                  else
                  {
                    Pic.P_Window->AddCmd(CMD_EDITNO_HID, i, old_actno, lrm);
                  }
                }
              }
            }
          }
         #else
          else
          {
            _VARLABEL vl1, vl2;
            init_VARLABEL(&vl1);
            init_VARLABEL(&vl2);
            vl1.no = 1;
            vl1.info[0].state = CONST_VAL;
            vl1.info[0].value = i;
            vl2.no = 1;
            vl2.info[0].state = CONST_VAL;
            vl2.info[0].value = old_actno;
            
            if(pev != NULL)
            {
              if((pev->ftype == _EVENT_HIDPRESS) && (old_actno != i))
              {
                // information wird für myio benötigt
                // es wird nach _MyIo::GetFocus() zusätzlich _MyIo::GetEvent() mit dem _EVENT_HIDPRESS aufgerufen
                // _MyIo::GetEvent() wird nicht mit anderen events zb._EVENT_KEYPRESS aufgerufen (kolott, ganber) 
                vl1.no = 3;
                vl1.info[1].state = CONST_VAL;
                vl1.info[1].value = *(_IVALUE*)&pev->dot;
                vl1.info[2].state = CONST_VAL;
                vl1.info[2].value = pev->timestamp;
                
                vl2.no = 3;
                vl2.info[1].state = CONST_VAL;
                vl2.info[1].value = pev->multindex;
                vl2.info[2].state = CONST_VAL;
                vl2.info[2].value = (_DWORD)pev->button;
              }
            }
            
            Pic.P_Window->AddCommand(CMD_EDITNO_HID, &vl1, &vl2, lrm);
            //Pic.P_Window->AddCmd(CMD_EDITNO_HID, i, old_actno, lrm);
            Beep(BEEP_INPUT);
          }
         #endif
          retcode = _TRUE;
          break;
//}          
        }
        else
        {
          pi = p_input_SEEKELEMENT(&Pic, i, _FALSE);
          if(pi != NULL)
          {
            //if(chk_VARLABEL(&pi->io.variable) == _TRUE)
            if(chk_VARLABEL_Makro(&pi->io.variable)) // Makro, speed: nicht auf true oder false abfragen
            {
              if(VarList.GetVarInfo(&pvi, pi->io.variable.info[0].value) == _TRUE)
              {
                state = _TRUE;
                init_VARINFO(&varinfo);
                if(copy_LSE_VARINFO(&state, &varinfo, pvi, _TRUE) == _TRUE)
                {
                 #ifndef CUTOFF_USER
                  _FVOID_PT_PT ptr;
                  _VARINFO     *pv;

                  pv  = &varinfo;
                  ptr = (_FVOID_PT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_INPUTNOACCESS);
                  if(ptr != NULL)
                  {
                    // eingabe wurde mit touch eindeutig angewählt, access jedoch nicht erlaubt
                    // zb.: für automatisches einblenden eines passwortfensters
                    load_this(LseThisPointer);
                    ptr(pi, pv);
                  }
                 #endif
                }
                free_VARINFO(&varinfo);
              }
            }
          }
        }
      }
    }
  }

  return(retcode);
}

_WHOAMI _Picture::GetWhoAmI(void)
{
  _WHOAMI whoami;

  if(Pic.P_Window != NULL)
    return(Pic.P_Window->GetWhoAmI());
  init_WHOAMI(&whoami);
  whoami.ima = IMA_NOTHING;
  return(whoami);
}

void _Picture::AddRegion_MOTION(_REGION referenz)
{
  _UWORD i;

  for(i=0; i<Pic.No; i++)
    Pic.Ptr[i].AddRegion_MOTION(referenz); 
}

_BOOL _Picture::PermitSearch(void)
{
  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
    if(Pic.Editor.chredit.change == _TRUE)
    {
      if(Pic.Editor.variable.info.vartype == FSTRING)
      {
        return _FALSE;
      }
      else if(Pic.memseekelement.no == 1)
      {
        // ist nur eine einzige eingabe vorhanden, so werden cursortasten für editor (nicht zum suchen) verwendet
        return _FALSE;
      }
    }
    
   #ifdef UC_TOUCH
    _WHOAMI wai = Root.FindTop()->GetWhoAmI();
    if(wai.ima == IMA_TOUCHEDITOR)
    {
      return _FALSE; // im toucheditor keine cursornavigation
    }
   #endif
  }

  return _TRUE;
}

void _Picture::SetReturn(_Window *p)
{
  Pic.P_Window = p;
}

void _Picture::PictureAddRegion(_DOT *offset)
{
  _ROOM r;
  _ZOOM zoom;
 #ifdef UC_THEME
  _FRAMETYPE frametype = _DEFFRAME;
 #endif

  init_ZOOM(&zoom);
  r = Pic.Room;

 #ifdef UC_THEME
  if(Pic.Ptr != NULL)
    frametype = Pic.Ptr->GetBackgroundTheme(&r);
 #endif

  Zoom_ROOM(&r, offset, &zoom);
 #ifdef UC_THEME
  if(frametype >= _THEMEFRAME)
    clip_THEME(&r, frametype);
  else
 #endif
    AddRegion(&r);

  if(Pic.P_Window != NULL)
  {
    if(Pic.P_Window->GetShadow() == _TRUE)
    {
      GetShadowRoom(&r);
     #ifdef UC_THEME
      if(frametype >= _THEMEFRAME)
        clip_THEME(&r, frametype);
      else
     #endif
        AddRegion(&r);
    }
  }
}

_MY_PICTURE *_Picture::GetMyPicture(void)
{
  return(&Pic);
}

void _Picture::SubEditorRegion(_REGION *region)
{
  _REGION subreg, newreg; 

  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
    if(Pic.Editor.bubblehelp.xstate == BUBBLE_ONSCREEN)
    {
      StartRegion();
      AddRegion(&Pic.Editor.bubblehelp.room);
      subreg = EndRegion();
      newreg = SubRegion(*region, subreg);
      FreeRegion(region);
      FreeRegion(&subreg);
      *region = newreg;
    }
  }
}

_BOOL is_navigation_key(_UDWORD scancode)
{
  switch(scancode)
  {
      case _TAB   : 
      case _UP    :
      case _DOWN  :
      case _RIGHT :
      case _LEFT  : return _TRUE;
  }
  return _FALSE;
}

void _Picture::PictureRunIntern(_Window *PWin, _EVENT *pe, _DOT *po)
{
  _UWORD      i;
 #ifdef HID
  _DOT        offset;
 #endif

 #ifdef OK_ENCLOSE_NAME
  if(GetEventEnclosedName(pe) == _IDIDIT)
    return;
 #endif

  if(Pic.Editor.input.io.typ == AS_THEURGE)
  {
    if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
    {
      if(getevent_THEURGE(pe, &Pic.Editor) == _IDIDIT)
      {
        return;
      }
    }
  }
  
  if(pe->ftype == _EVENT_NONE)
  {
    PictureRefresh(po);                                           // refresh IO-list
   #ifdef FIRSTINPUT_ALT
    RunEditor(PWin, pe, _FALSE);
   #else // FIRSTINPUT_ALT
    if(RunEditor(PWin, pe, _FALSE) == _FALSE) // es ist kein editor offen
    {
      if(DoAutomaticSeekInput == _TRUE) // FirstInput soll ausgeführt werden
      {
        if(Root.FindTop()->GetP_Picture() == this) // nur wenn top-window
        {
          if(++AutomaticSeekTimer > 5) // erst nach 5 durchlaufen...
          {
            AutomaticSeekInput(_FALSE);
            AutomaticSeekTimer = 0;
          }
        }
      }    
    }
    else
    {
      AutomaticSeekTimer = 0;
    }
   #endif // FIRSTINPUT_ALT
  }
  else
  {
    if((pe->ftype == _EVENT_KEYPRESS)||(pe->ftype == _EVENT_KEYRELEASE))
    {
      i = Pic.No;
      while(i)
      {
        i--;
        Pic.Ptr[i].DoKeyboard(pe, Pic.P_Window, Pic.GlobalHotkeys);
      }

      if(pe->ftype == _EVENT_KEYPRESS)
      {
        switch(pe->scancode)
        {
          case _TAB   : 
           #ifdef UC_TOUCHBUBBLE_BY_SELECTION
            Init_BubbleState();
           #endif
            Seek(pe);
            break;
          
          case _UP    :
          case _DOWN  :
           #ifdef UC_TOUCHBUBBLE_BY_SELECTION
            Init_BubbleState();
           #endif
            Seek(pe);
            break;
          
          case _RIGHT :
          case _LEFT  : 
            if(PermitSearch() == _TRUE)
            {
             #ifdef UC_TOUCHBUBBLE_BY_SELECTION
              Init_BubbleState();
             #endif
              Seek(pe);
            }
            else
            {
              RunEditor(PWin, pe, _FALSE);   // ifnec. run editor
            }
            break;
          
          default     : 
            RunEditor(PWin, pe, _FALSE);     // ifnec. run editor
            break;
        }
      }
      else
      {
        RunEditor(PWin, pe, _FALSE);
      }
    }
   #ifdef HID
    else if((pe->ftype == _EVENT_HIDPRESS)||(pe->ftype == _EVENT_HIDRELEASE)||(pe->ftype == _EVENT_HIDMOVE)||(pe->ftype == _EVENT_TRIGGER)||(pe->ftype == _EVENT_PREHIDPRESS))
    {
      _BOOL state = _FALSE;
      _ZOOM zoom;

      // look for buttons and action in carousel
      offset = *po;
      init_ZOOM(&zoom);
      for(i=0; i<Pic.No; i++)
      {
        if(Pic.Ptr[i].DoHID(pe, Pic.P_Window, offset, &zoom) == _TRUE)
        {
          found_something_by_hid(pe, FOUND_BUTTON);
          state = _TRUE;
          break;
        }
      }

      // look for input
      if((pe->ftype == _EVENT_HIDPRESS) && (state == _FALSE))
      {
        if(SeekHid(&pe->dot, _FALSE, pe->lrm, _SELECTOR_ONLY_HID, pe) == _TRUE)
        {
          found_something_by_hid(pe, FOUND_INPUT);
          state = _TRUE;
        }
      }

     #ifdef UC_THEME
      // look for action in theme
      if(((pe->ftype == _EVENT_HIDPRESS)||(pe->ftype == _EVENT_HIDRELEASE))&&(state == _FALSE))
      {
        if(Theme.IsAction == _TRUE) // test ob überhaupt ein thema mit actionbutton vorhanden ist
        {
          if(SeekThemeCancle(&pe->dot, offset, &zoom, (pe->ftype == _EVENT_HIDPRESS)? _TRUE : _FALSE) == _TRUE)
          {
            found_something_by_hid(pe, FOUND_CANCLE);
            InsertCmd(CMD_RETURN, 0, 0);
            init_EVENT_Makro(pe);
          }
        }
      }
     #endif

     #ifdef UC_TOUCHBUBBLE_BY_SELECTION
      if((pe->ftype == _EVENT_HIDRELEASE) && (BubbleState == 1))
      {
        BubbleState = 2;
        if(timediff(TimeDiff) < SetUp.Get_BUBBLEINFO()->delaytime)
        {
          Pic.P_Window->AddCmd(CMD_EDITNO_HID, BubbleInput, -1, pe->lrm);
          Init_BubbleState();
        }
      }
     #endif
    }
   #endif
    else if((pe->ftype == _EVENT_SHUTTLEINC)||(pe->ftype == _EVENT_SHUTTLEDEC))
    {
      if((IsEditorTrue(Pic.Editor.actno) == _TRUE)&&(Pic.Editor.chredit.change == _TRUE))
        RunEditor(PWin, pe, _FALSE);
      else
        Seek(pe);
    }
    else
    {
      PictureRefresh(po);                                           // refresh IO-list
      RunEditor(PWin, pe, _FALSE);
    }  
  }
  
  if(RecalcIoID == _TRUE)
  {
    if(Pic.P_Window != NULL)
      Pic.P_Window->AddCmd(CMD_RECALC_IOID, RecalcIoIDot.x, RecalcIoIDot.y, _FALSE);
    SetRecalcIoID(_FALSE, 0, 0);
  }

 #ifdef FIRSTINPUT_ALT
  if(DoAutomaticSeekInput == _TRUE)
  {
    if(timediff(AutomaticSeekTimer) < 1500)
    {
      if(AutomaticSeekInput(_TRUE) == _TRUE)
      {
        DoAutomaticSeekInput = _FALSE;
      }
    }
    else
    {
      DoAutomaticSeekInput = _FALSE;
      AutomaticSeekInput(_FALSE);
    }
  }
 #endif // FIRSTINPUT_ALT
}

void _Picture::DoAllNegEdge(_BOOL redraw)
{
  _UWORD i;

  KillAllSliderFocus();  

  for(i=0; i<Pic.No; i++)
    Pic.Ptr[i].DoAllNegEdge(redraw);
}

_BOOL _Picture::CopyEditor(_EDITOR *pe, _UWORD layer)
{
  _BOOL retcode = _FALSE;

  if(layer < Pic.No)
    retcode = Pic.Ptr[layer].CopyEditor(pe);

  return(retcode);
}

_EDITOR* _Picture::GetEditor(void)
{
  return(&Pic.Editor);
}
#ifndef UC_NO_TAB_CURSORNAVIGATION
_BOOL _Picture::SeekTab(_UWORD *sno, _UWORD *ino, _BOOL tab, _DOT *offset, _ZOOM *zoom, _SELECTOR selector)
{
  _BOOL  retcode = _FALSE;
  _WORD  layer;
  _UWORD no;

  if(Pic.No > 0)
  {
    layer = Pic.Editor.actlayer;
    no    = Pic.No;

    while(no)
    {
      no --;

      if(tab == _TRUE)
        layer += 1;
      else
        layer -= 1;

      if(layer < 0)
        layer = Pic.No - 1;
      if(layer >= Pic.No)
        layer = 0;

      if(is_SEEKELEMENT_available(&Pic, layer, selector) == _TRUE)
      {
        retcode = _TRUE;
        break;
      }
    }

    if(retcode == _TRUE)
    {
      no = Pic.Ptr[layer].GetLastEditor();
      if(access_SEEKELEMENT(&Pic, no, _FALSE, selector) == _FALSE)
        retcode = seek_horizontal(&no, &Pic, layer, _TRUE, 65535, get_seek_algorithm(), offset, zoom, selector);
      
      if(retcode == _TRUE)
      {
        *sno = layer;
        *ino = no;
      }
    }
  }

  return(retcode);
}
#endif
_BOOL _Picture::ReloadText(_Softkey *ps, _WHOAMI *whoami)
{
  _BOOL  retcode = _FALSE;
  _ASCII dpne[_MAX_PATHLENGTH];
  _UWORD no = (_UWORD)whoami->no;

  if(whoami->ima == IMA_PICTURE)
  {
    if(Project.GetPictureAccess(dpne, &no, _TRUE, NULL) == _TRUE)
    {
      retcode = Load(dpne, ps, _TRUE, _FALSE, whoami->no);
     #ifdef XUSER_KM
      if(retcode == _TRUE)
      {
        _PRJ_ENTRY *prje = Project.GetEntryByNo(no, _PE_PICTURE);
        if(prje != NULL)
          ps->SetAssignedImage(prje->imageno);
      }
     #endif
    }
  }
  else if(whoami->ima == IMA_WINDOW)
  {
    if(Project.GetWindowAccess(dpne, &no, _TRUE, NULL) == _TRUE)
    {
      retcode = Load(dpne, ps, _FALSE, _FALSE, 0);
     #ifdef XUSER_KM
      if(retcode == _TRUE)
      {
        _PRJ_ENTRY *prje = Project.GetEntryByNo(no, _PE_WINDOW);
        if(prje != NULL)
          ps->SetAssignedImage(prje->imageno);
      }
     #endif
    }
  }

  if(retcode == _TRUE)
    whoami->no = no;

  return(retcode);
}

_BOOL _Picture::Overload(_VARLABEL *ov, _BOOL change_ctrl_type)
{
  _UWORD i;

  if(valid_VARLABEL(ov) == _TRUE)
  {
    for(i=0; i<Pic.No; i++)
      Pic.Ptr[i].Overload(ov, change_ctrl_type);
      
    return _TRUE;
  }

  return _FALSE;
}

_BOOL _Picture::ChangeOverload(_VARLABEL *ovist, _VARLABEL *ovsoll)
{
  _UWORD i;
  _DOT   offset;

  if(valid_VARLABEL(ovsoll) == _TRUE)
  {
    init_DOT(&offset);

    for(i=0; i<Pic.No; i++)
      Pic.Ptr[i].ChangeOverload(ovist, ovsoll);

//    ReDraw();
    return _TRUE;
  }

  return _FALSE;
}

_BOOL _Picture::SummaryOverload(_UWORD summary, _VARLABEL *ovsoll)
{
  _UWORD i;
  _DOT   offset;

  if(valid_VARLABEL(ovsoll) == _TRUE)
  {
    init_DOT(&offset);

    for(i=0; i<Pic.No; i++)
      Pic.Ptr[i].SummaryOverload(summary, ovsoll);

//    ReDraw();
    return _TRUE;
  }

  return _FALSE;
}

void _Picture::ReDraw(void)
{
  _UWORD i;
  _DOT   offset;

  init_DOT(&offset);

  for(i=0; i<Pic.No; i++)
    Pic.Ptr[i].ReDraw(&offset, &Pic.PicZoom);

  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
    if(ShowEditor() == _TRUE)
   #ifdef OK_SUMMARY
    summary_SEEKELEMENT(&Pic, Pic.Editor.actno, _TRUE, this);
   #endif
  }
}

_BOOL _Picture::FindInputByVarNo(_UWORD *erg, _UDWORD varno)
{
  return(find_input_by_varno(erg, &Pic, varno));
}

void _Picture::KillBackGroundImage(void)
{
  _UWORD i;

  for(i=0; i<Pic.No; i++)
    Pic.Ptr[i].KillBackGroundImage();
}

_BOOL _Picture::DoHotkey(_EVENT *pe)
{
  if((pe->ftype == _EVENT_KEYPRESS)||(pe->ftype == _EVENT_KEYRELEASE))
    return hotkey_SEEKELEMENT(&Pic, Pic.P_Window, (pe->ftype == _EVENT_KEYPRESS)? _TRUE:_FALSE, pe);

  return _FALSE;
}

_UDWORD _Picture::GetCarouselRoom(_ROOM *p)
{
  _UDWORD retcode = 0;
  
  for(_UWORD i=0; i<Pic.No; i++)
    retcode += Pic.Ptr[i].GetCarouselRoom(p);
  
  return retcode;
}

_BOOL _Picture::IsThereAnyCarousel(void)
{
  for(_UWORD i=0; i<Pic.No; i++)
  {
    if(Pic.Ptr[i].IsThereAnyCarousel() == _TRUE)
      return _TRUE;
  }
  
  return _FALSE;
}

#ifdef NINETEEN
_DWORD _Picture::CyclicButton(_EVENT *pe, _UDWORD globalscr)
{
  _DWORD no = -1;
  _ZOOM  zoom;
  _DOT   offset;

  if(Pic.Ptr != NULL)
  {
    init_DOT(&offset);
    init_ZOOM(&zoom);

    if((Pic.No >= 1)||(globalscr != 0))
      no = Pic.Ptr[0].CyclicButton(pe, &offset, &zoom);
    if((no == -1)&&(Pic.No >= 2))
      no = Pic.Ptr[1].CyclicButton(pe, &offset, &zoom);
  }

  return(no);
}

extern "C" _DWORD CyclicButton(_EVENT *pe, _UDWORD dashbrd, _UDWORD globscr)
{
  _Picture *pPic;
  
  if(dashbrd == 0)
  {
    pPic = Root.GetP_Picture();
    if(pPic != NULL)
      return(pPic->CyclicButton(pe, globscr));
  }
  
  return(DashBoard.CyclicButton(pe, 0));
}
#endif

void _Picture::FinishCarouselSwim(_DOT *po)
{
  for(_UDWORD i=0; i<Pic.No; i++)
  {
    Pic.Ptr[i].FinishCarouselSwim(Pic.Editor.actno, po, &Pic.PicZoom, this);
  }
}


//void open_first_input(void)
//{
//  _Window *pWin = Root.FindTop();
//  if(pWin != NULL)
//  {
//    _Picture *pPic = pWin->GetP_Picture();
//    if(pPic != NULL)
//    {
//      _MY_PICTURE *pmp = pPic->GetMyPicture();
//      if(pmp != NULL)
//      {
//        if((pmp->FirstInput.x >= 0) && (pmp->FirstInput.y >= 0))
//        {
//          // es ist ein firstinput gesetzt
//          if(pPic->IsEditorTrue(pmp->Editor.actno) == _FALSE)  
//          {
//            // kein editor offen
//            pWin->InsertCmd(CMD_AUTOSEEKINPUT, 0, 0, _FALSE);
//          }
//        }
//      }
//    }
//  }
//}


