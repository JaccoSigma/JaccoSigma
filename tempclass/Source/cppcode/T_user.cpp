#include "T_IPC.H"
#include "T_NUC.H"

//#define OLDCALCULATOR





//#define hebrew
//#define CHANGE // punkt tauschen
/*
_UWORD errcnt;

void cerror(_UDWORD line, _ASCII *str)
{
  Debug(str, 1, 1);
  Debug(line, 2);
  errcnt += 1;
  Debug(errcnt, 3);
}

void change201Eto2022(_UNI *l, _UNI *ref, _UWORD len)
{
  _UWORD i;

  for(i=0; i<len; i++)
  {
    if((l[i] == 0x201E)&&(ref[i] == 0x2022))
    {
	  l[i] = 0x2022;
    }
  }
}

_BOOL ccompare(_UNI *l1, _UNI *l2, _UWORD len, _UNI chr)
{
  _UWORD i;

  for(i=0; i<len; i++)
  {
    if((l1[i] == chr)&&(l2[i] != chr))
    {
      Debug(l1[i], 10);
      Debug(l2[i], 11);
      Debug(i, 12);
      return(_FALSE);
    }
  }

  return(_TRUE);
}

_BOOL lcompare(_UDWORD line, _UNI *l1, _UNI *l2)
{
  _UWORD len1, len2;

  if(StrCmp(l1, 2, l2, 2) != 0)
  {
    if((l1[0] == '{')||(l2[0] == '{'))
    {
      if(MemCmp(l1, l2, 38*2) != 0)
      {
        cerror(line, "ungleiche guid...");
        return(_FALSE);
      }
    }

    len1 = StrLen(l1, 2);
    len2 = StrLen(l2, 2);

    if(len1 != len2)
    {
      cerror(line, "längenunterschied...");
      return(_FALSE);
    }

    if(ccompare(l1, l2, len1, ' ') == _FALSE)
    {
      cerror(line, "space passen nit...");
      return(_FALSE);
    }
    if(ccompare(l2, l1, len1, ' ') == _FALSE)
    {
      cerror(line, "space passen nit...");
      return(_FALSE);
    }
    if(ccompare(l1, l2, len1, '*') == _FALSE)
    {
      cerror(line, "* passen nit...");
      return(_FALSE);
    }
    if(ccompare(l2, l1, len1, '*') == _FALSE)
    {
      cerror(line, "* passen nit...");
      return(_FALSE);
    }
    if(ccompare(l1, l2, len1, 124) == _FALSE)
    {
      cerror(line, "pipe passen nit...");
      return(_FALSE);
    }
    if(ccompare(l2, l1, len1, 124) == _FALSE)
    {
      cerror(line, "pipe passen nit...");
      return(_FALSE);
    }

    
#ifdef CHANGE
    change201Eto2022(l1, l2, len1);
#endif

    if(ccompare(l1, l2, len1, 0x2022) == _FALSE)
    {
      cerror(line, "2022 passen nit...");
      return(_FALSE);
    }
    if(ccompare(l2, l1, len1, 0x2022) == _FALSE)
    {
      cerror(line, "2022 passen nit...");
      return(_FALSE);
    }
  }

  return(_TRUE);
}

void compare(_ASCII *d1, _ASCII *d2)
{
  _UNI    tmp1[1000];
  _UNI    tmp2[1000];
  _File   File1, File2;
  _BOOL   st1, st2;
  _UWORD  run = 1;
  _UDWORD line = 0;

  errcnt = 0;

#ifdef hebrew
  _File File;
  File.Open(SAVE, "RAM@C:\\test.txt");
#endif

  if(File1.Open(LOAD, d1) == _TRUE)
  {
    if(File2.Open(LOAD, d2) == _TRUE)
    {
      File1.Read(tmp1, 2);
      File2.Read(tmp2, 2);
#ifdef hebrew
      File.Write(tmp1, 2);
#endif
      run = 1;

      do
      {
        line += 1;
        st1 = File1.ReadCrLf(tmp1, 999, 2);
        st2 = File2.ReadCrLf(tmp2, 999, 2);

        if((st1 == _FALSE)||(st2 == _FALSE))
        {
          run = 0;
          if(st1 != st2)
            cerror(line, "Ungleiches Fileende...");
        }
        else
        {
#ifndef hebrew
          lcompare(line, tmp1, tmp2);
#else
          if(lcompare(line, tmp1, tmp2) == _TRUE)
            File.Write(tmp1, StrLen(tmp1, 2) * 2);
          else
            File.Write(tmp2, StrLen(tmp2, 2) * 2);

          tmp1[0] = 13;
          tmp1[1] = 10;
          File.Write(tmp1, 4);
#endif
        }
      }
      while(run == 1);


      File2.Close();
    }
    File1.Close();
  }


#ifdef hebrew
  File.Close();
#endif

  cerror(line, "--------------- FINITO ---------------");
}
*/

//#define LCD_GETVALUE 0
//#define LCD_DECREASE 1
//#define LCD_INCREASE 2
//#define LCD_STANDARD 3
//#define LCD_SETVALUE 4
//_DWORD LCD_Contrast(_UWORD cmd, _UWORD val);
//_DWORD LCD_BRIGHTNESS(_UWORD cmd, _UWORD val);
//falls funktion nicht vorhanden, ist retcode auf -1 gesetzt

_UDWORD ScrSvrLastEvent;
_UDWORD ScrSvrOldTime;

/* ************************************************************************* */
/* * this function is called once in lifetime of application               * */
/* * if you change the arrangement, or add a new global structure, your    * */
/* * application has changed and therefore this function is called at      * */
/* * once for another time (new application)                               * */
/* * note: the project is not ready loaded                                 * */
/* ************************************************************************* */
void urinit(void)
{
 #ifndef CUTOFF_USER
  _FVOID_VOID ptr;

  ptr = (_FVOID_VOID)find_lasal_method(LseThisPointer, METHOD_LSE_URINIT);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    ptr();
  }
 #endif
}

/* ************************************************************************* */
/* * this function is called every time when you start your application    * */
/* * note: the project is not ready loaded                                 * */
/* ************************************************************************* */
void init(void)
{
}

/* ************************************************************************* */
/* * function is called before loading project                             * */
/* * graphic is already initialized,                                       * */
/* * no picture is loaded and nothing is on the screen (blank screen)      * */
/* * the initfunctions (IpcUrinit, IpcInit) are well done                  * */
/* ************************************************************************* */
void _Project::Start(void)
{
  MethodLseWindowRun           = (_FVOID_PT_PT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_RUN);
  MethodLseProjectRun          = (_FVOID_PT)find_lasal_method(LseThisPointer, METHOD_LSE_PROJECTRUN);
  MethodLseRecalculate         = (_FBOOL_PT_PT_BOOL)find_lasal_method(LseThisPointer, METHOD_LSE_RECALCULATE);
  MethodLseUserCall            = (_FVOID_DWORD_DWORD)find_lasal_method(LseThisPointer, METHOD_LSE_FCAWL);
  MethodLseAccessButton        = (_FBOOL_PT)find_lasal_method(LseThisPointer, METHOD_LSE_ACCESSBUTTON);
  MethodLseAccessInput         = (_FBOOL_PT)find_lasal_method(LseThisPointer, METHOD_LSE_ACCESSINPUT);
  MethodLseCheckEnable         = (_FBOOL_DWORD)find_lasal_method(LseThisPointer, METHOD_LSE_CHKENABLE);
  MethodLseTryToAccess         = (_FBOOL_PT_UWORD_FOUNDHID_BOOL)find_lasal_method(LseThisPointer, METHOD_LSE_TRY_TO_ACCESS);
  MethodLseFoundSomethingByHid = (_FVOID_FOUNDHID)find_lasal_method(LseThisPointer, METHOD_LSE_FOUNDHID);

 #ifndef CUTOFF_USER
  _FVOID_VOID ptr;

  ptr = (_FVOID_VOID)find_lasal_method(LseThisPointer, METHOD_LSE_PROJECTSTART);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    ptr();
  }
 #endif
}

/* ************************************************************************* */
/* * function is called after loading project                              * */
/* * graphic is already initialized,                                       * */
/* * no picture is loaded and nothing is on the screen (blank screen)      * */
/* ************************************************************************* */
/* * state .... _TRUE successful loading project either _FALSE             * */
/* ************************************************************************* */
/* * return _FALSE if you don't want to start this application             * */
/* ************************************************************************* */
_BOOL _Project::Ready(_BOOL state)
{
 #ifndef CUTOFF_USER
  _FBOOL_BOOL ptr;

  ptr = (_FBOOL_BOOL)find_lasal_method(LseThisPointer, METHOD_LSE_PROJECTREADY);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    state = ptr(state);
  }
 #endif

  return(state);
}

/* ************************************************************************* */
/* * function is called cyclic                                             * */
/* ************************************************************************* */
/* * event ..... --> to event (hid, keybrd)                                * */
/* ************************************************************************* */
void _Project::Run(_EVENT *pe)
{
  if(SystemMessage != EMTY_TEXT)
  {
    if(pe->ftype != _EVENT_NONE)
    {
      if((pe->ftype == _EVENT_KEYPRESS)||(pe->ftype == _EVENT_HIDPRESS)||(pe->ftype == _EVENT_SHUTTLEINC)||(pe->ftype == _EVENT_SHUTTLEDEC))
      {
        SystemError(EMTY_TEXT, 0, NULL);
      }
    }
  }
  
 #ifndef CUTOFF_USER
  if(MethodLseProjectRun != NULL)
  {
    load_this(LseThisPointer);
    MethodLseProjectRun((void*)pe);
  }
 #endif
}

/* ************************************************************************* */
/* * function is called before closing application                         * */
/* * graphic is still initialized                                          * */
/* * and the projectfiles are still accessible                             * */
/* ************************************************************************* */
void _Project::End(void)
{
 #ifndef CUTOFF_USER
  _FVOID_VOID ptr;

  ptr = (_FVOID_VOID)find_lasal_method(LseThisPointer, METHOD_LSE_PROJECTEND);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    ptr();
  }
 #endif
}

void pos_2button(_ROOM *p, _ROOM *pr, _BOOL left)
{
  if((pr->xy2.x - pr->xy1.x) >= 400)
  {
    p->xy1.y = pr->xy2.y - 50;
    p->xy2.y = pr->xy2.y - 10;

    if(left == _TRUE)
    {
      p->xy1.x = pr->xy1.x + 10;
      p->xy2.x = ((pr->xy1.x + pr->xy2.x) / 2) - 5;
    }
    else
    {
      p->xy1.x = ((pr->xy1.x + pr->xy2.x) / 2) + 5;
      p->xy2.x = pr->xy2.x - 10;
    }
  }
  else
  {
    p->xy1.x = pr->xy1.x + 10;
    p->xy2.x = pr->xy2.x - 10;
    if(left == _TRUE)
    {
      p->xy1.y = pr->xy1.y + 10;
      p->xy2.y = pr->xy1.y + 50;
    }
    else
    {
      p->xy1.y = pr->xy2.y - 50;
      p->xy2.y = pr->xy2.y - 10;
    }
  }
}

static void make_resetwindow_hid(_MY_PICTURE *mp, _EVENT *pe)
{
  _ROOM  room;
  _DOT   dot;
  _EVENT evnt;

  if(pe->ftype == _EVENT_HIDPRESS)
  {
    dot = pe->dot;
    pos_2button(&room, &mp->Room, _TRUE);
    if((dot.x >= room.xy1.x) && (dot.x <= room.xy2.x) && (dot.y >= room.xy1.y) && (dot.y <= room.xy2.y))
    {
      init_EVENT_Makro(&evnt);
      evnt.ftype    = _EVENT_KEYPRESS;
      evnt.scancode = _ENTER;
      AddEvent(&evnt);
      //pe->ftype    = _EVENT_KEYPRESS;
      //pe->scancode = _ENTER;
      //done = _TRUE;
      free_EVENT(pe);
    }
    pos_2button(&room, &mp->Room, _FALSE);
    if((dot.x >= room.xy1.x) && (dot.x <= room.xy2.x) && (dot.y >= room.xy1.y) && (dot.y <= room.xy2.y))
    {
      init_EVENT_Makro(&evnt);
      evnt.ftype    = _EVENT_KEYPRESS;
      evnt.scancode = _ESC;
      AddEvent(&evnt);
      //pe->ftype    = _EVENT_KEYPRESS;
      //pe->scancode = _ESC;
      //done = _TRUE;
      free_EVENT(pe);
    }
  }
}


void resetwindow_ready(_MY_PICTURE *mp)
{
  _ROOM room;
  
  if(mp->JustProgram == _TRUE)
  {
    _FONT fnt = (_FONT)FontScheme.GetIndexByLabel((_ASCII*)"_@Standard");
    room = mp->Room;
   #ifdef HID
    OutMultiTextAlign((void*)"ATTENTION !!|Reset CPU", 0, &room, fnt, T_MIDBOUND|T_CENTERBOUND|T_COPY|T_SOLID|T_PROP, FRAME_STEALTH, WHITE, MakeColor_Makro(LIGHTRED, LIGHTRED), MakeColor_Makro(WHITE, RED), sizeof(_ASCII));
    pos_2button(&room, &mp->Room, _TRUE);
    OutMultiTextAlign((void*)"<ENTER>...reset", 0, &room, fnt, T_MIDBOUND|T_CENTERBOUND|T_COPY|T_SOLID|T_PROP, FRAME_PLASTIC, WHITE, MakeColor_Makro(LIGHTRED, LIGHTRED), MakeColor_Makro(WHITE, RED), sizeof(_ASCII));
    pos_2button(&room, &mp->Room, _FALSE);
    OutMultiTextAlign((void*)"<ESC>...cancel", 0, &room, fnt, T_MIDBOUND|T_CENTERBOUND|T_COPY|T_SOLID|T_PROP, FRAME_PLASTIC, WHITE, MakeColor_Makro(LIGHTRED, LIGHTRED), MakeColor_Makro(WHITE, RED), sizeof(_ASCII));
   #else
    OutMultiTextAlign((void*)"ATTENTION !!|Reset CPU|Press <ENTER>", 0, &room, fnt, T_MIDBOUND|T_CENTERBOUND|T_COPY|T_SOLID|T_PROP, FRAME_STEALTH, WHITE, MakeColor_Makro(LIGHTRED, LIGHTRED), MakeColor_Makro(WHITE, RED), sizeof(_ASCII));
   #endif
  }

 #ifdef UC_ROTATE
  RefreshRectangle(&mp->Room);
 #endif  
}

void resetwindow_run(_MY_PICTURE *mp, _EVENT *pe)
{
  if(mp->JustProgram == _TRUE)
  {
    make_resetwindow_hid(mp, pe);
    if((pe->ftype == _EVENT_KEYPRESS)&&(pe->scancode == _ENTER))
    {
      TermAlarm.CacheFinalize();
      Lse_Reset(_FALSE);
    }
  }
}

/* ************************************************************************* */
/* * function is called before drawing a new screen (picture,window,popup) * */
/* ************************************************************************* */
void _Window::Start(_BOOL firsttime)
{
  _WHOAMI     *pwai = &WhoAmI;
 #ifndef CUTOFF_USER
  _FVOID_PT_PT_BOOL ptr;
  void        *para;
  
  if(firsttime == _TRUE)
    DoFlipScreen();

  ptr = (_FVOID_PT_PT_BOOL)find_lasal_method(LseThisPointer, METHOD_LSE_START);
  if(ptr != NULL)
  {
    para = GetContentPointer();
    load_this(LseThisPointer);
    ptr((void*)pwai, para, firsttime);
  }
 #endif

 #ifdef UC_DIRECTORY
  if((pwai->ima == IMA_WINDOW)&&(pwai->no == UC_DIRECTORY))
    Directory.Call(STATE_START, NULL, firsttime, &P_Picture->GetMyPicture()->Room);
 #endif

  if((pwai->ima == IMA_WINDOW)&&(pwai->no == UC_OFFLINEWINDOW))
  {
    if(P_Picture != NULL)
    {
      offlinewindow_start(P_Picture->GetMyPicture(), firsttime);
    }
  }

  ScrSvrLastEvent = get_abstime();
  ScrSvrOldTime = 0;
}

/* ************************************************************************* */
/* * function is called after drawing a new screen                         * */
/* * IO's will be drawn at a later time !                                  * */
/* ************************************************************************* */
/* * firsttime .... _TRUE if screen is drawn for the firsttime,            * */
/* *                on the other hand (_FALSE) it acts in backgrndrestore  * */
/* ************************************************************************* */
void _Window::Ready(_BOOL firsttime)
{
  _WHOAMI *pwai = &WhoAmI;

  if(SetUp.GetDeleteEventBuffer() == _TRUE)
    Keyboard.DelBuffer();

 #ifndef CUTOFF_USER
  _FVOID_PT_PT_BOOL ptr;
  void    *para;

  ptr = (_FVOID_PT_PT_BOOL)find_lasal_method(LseThisPointer, METHOD_LSE_READY);
  if(ptr != NULL)
  {
    para = GetContentPointer();
    load_this(LseThisPointer);
    ptr((void*)pwai, para, firsttime);
  }
 #endif

  if(pwai->ima == IMA_SCREENSAVER)
    ScreenSaver.Ready();

  if(pwai->ima == IMA_WINDOW)
  {
   #ifdef UC_DIRECTORY
    if(pwai->no == UC_DIRECTORY)
      Directory.Call(STATE_READY, NULL, firsttime, &P_Picture->GetMyPicture()->Room);
   #endif

    if(P_Picture != NULL)
    {
      if(pwai->no == UC_OFFLINEWINDOW)
      {
        offlinewindow_ready(P_Picture->GetMyPicture());
      }
      else if(pwai->no == UC_RESETWINDOW)
      {
        resetwindow_ready(P_Picture->GetMyPicture());
      }
      else if(pwai->no == UC_CONFIRMWINDOW)
      {
        MessageBox.Ready(P_Picture->GetMyPicture(), MessBoxIndex);
      }
     #ifdef UC_BUBBLE_HELP 
      else if(pwai->no == UC_NEWBUBBLEWINDOW)
      {
        NewBubbleHelp.Ready(P_Picture->GetMyPicture());
      }
     #endif
    }
  }
}

/* ************************************************************************* */
/* * function is called (cyclic) during a screen is open                   * */
/* * ...don't mind the clipping                                            * */
/* ************************************************************************* */

_BOOL _Window::CallScreensaverEnd(_EVENT *pe)
{
  _BOOL  retcode = _TRUE;
  _EVENT tmpevnt;
  
  if(ScreenSaverEndCnt == 0)
  {
    if(pe == NULL)
    {
      pe = &tmpevnt;
      init_EVENT(pe);
    }
  
   #ifndef CUTOFF_USER
    _FBOOL_PT pte = (_FBOOL_PT)find_lasal_method(LseThisPointer, METHOD_LSE_SCRSVR_END);
    if(pte != NULL)
    {
      load_this(LseThisPointer);
      retcode = pte(pe);
    }
   #endif
  }
  
  if(retcode == _TRUE)
    ScreenSaverEndCnt += 1;
    
  return retcode;
}

void _Window::Run(_EVENT *pe)
{
  _WHOAMI *pwai = &WhoAmI;

 #ifndef CUTOFF_USER
  _FBOOL_VOID     ptx;
//  _FBOOL_PT       pte;
  _BOOL           statx;
  void            *para;
    
  if(MethodLseWindowRun != NULL)
  {
    para = GetContentPointer(); 
    load_this(LseThisPointer);
    MethodLseWindowRun((void*)pwai, para, (void*)pe);
  }
 #endif

 if(pwai->ima != IMA_NOTHING)
 {
   #ifdef UC_DIRECTORY
    if((pwai->ima == IMA_WINDOW)&&(pwai->no == UC_DIRECTORY))
    {
      if(Directory.Call(STATE_RUN, pe, _TRUE, &P_Picture->GetMyPicture()->Room) == _FALSE)
      {
        AddCmd(CMD_RETURN, 0, 0, _FALSE);
      }
    }
   #endif

    if(pwai->ima == IMA_SCREENSAVER)
    {
      ScreenSaver.Run();

      if(pe->ftype != _EVENT_NONE)
      {
        if(CallScreensaverEnd(pe) == _TRUE)
          AddCmd(CMD_RETURNQUEUE, 0, 0, _FALSE);
/*          
       #ifndef CUTOFF_USER
        statx = _TRUE;
        pte   = (_FBOOL_PT)find_lasal_method(LseThisPointer, METHOD_LSE_SCRSVR_END);
        if(pte != NULL)
        {
          load_this(LseThisPointer);
          statx = pte(pe);
        }
        if(statx == _TRUE)
       #endif
        {
          AddCmd(CMD_RETURNQUEUE, 0, 0, _FALSE);
        }
*/
        pe->ftype = _EVENT_NONE;  // eat event
      }
    }
    else if(P_Window == NULL)
    {
      if(pe->ftype != _EVENT_NONE)
        ScrSvrLastEvent = get_abstime();
      else
      {
        if(get_delay_screensave_Makro() > 1000)
        {
          if(ScrSvrOldTime != 0)
          {
            if(timediff(ScrSvrLastEvent) > get_delay_screensave_Makro())
            {
             #ifndef CUTOFF_USER
              statx = _TRUE;
              ptx   = (_FBOOL_VOID)find_lasal_method(LseThisPointer, METHOD_LSE_SCRSVR_START);
              if(ptx != NULL)
              {
                load_this(LseThisPointer);
                statx = ptx();
              }
              if(statx == _TRUE)
             #endif
              {
               #ifndef USER_DEKEMA
               #ifndef USER_BILLION
                if((pwai->ima == IMA_TOUCHEDITOR)||(pwai->ima == IMA_POPUP))
                {
                  // reihenfolge (ret, kill) unbedingt beibehalten, ansonsten ist screen eingefroren...
                  AddCmd(CMD_RETURN, 0, 0, _FALSE);
                }
                AddCmd(CMD_KILLEDITOR, 0, 0, _FALSE);
               #endif
               #endif

                AddCmd(CMD_SCREENSAVE, 0, 0, _FALSE);
              }
             #ifndef CUTOFF_USER
              else
             #endif 
              {
                // screensaver zurücksetzen
                ScrSvrLastEvent = get_abstime();
              }
            }
          }
          else
          {
            ScrSvrOldTime = get_delay_screensave_Makro();
            ScrSvrLastEvent = get_abstime();
          }
        }
      }
    }

    if(pwai->ima == IMA_WINDOW)
    {
      if(P_Picture != NULL)
      {
        if(pwai->no == UC_OFFLINEWINDOW)
        {
          offlinewindow_run(P_Picture->GetMyPicture(), pe);
        }
        else if(pwai->no == UC_RESETWINDOW)
        {
          resetwindow_run(P_Picture->GetMyPicture(), pe);
        }
        else if(pwai->no == UC_CONFIRMWINDOW)
        {
          MessageBox.Run(P_Picture->GetMyPicture(), pe, MessBoxIndex);
        }
       #ifdef UC_BUBBLE_HELP 
        else if(pwai->no == UC_NEWBUBBLEWINDOW)
        {
          NewBubbleHelp.Run(P_Picture->GetMyPicture(), pe);
        }
       #endif
      }
    }
  }

  if(P_Window == NULL)
    Batch.WatchOnline();
}

/* ************************************************************************* */
/* * function is called at the end of a screen                             * */
/* ************************************************************************* */
void _Window::End(void)
{
  _WHOAMI *pwai = &WhoAmI;

 #ifndef CUTOFF_USER
  _FVOID_PT_PT ptr;
  void    *para;

  ptr = (_FVOID_PT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_END);
  if(ptr != NULL)
  {
    para = GetContentPointer();
    load_this(LseThisPointer);
    ptr((void*)pwai, para);
  }
 #endif

 #ifdef UC_DIRECTORY
  if((pwai->ima == IMA_WINDOW)&&(pwai->no == UC_DIRECTORY))
    Directory.Call(STATE_END, NULL, _TRUE, &P_Picture->GetMyPicture()->Room);
 #endif

  if(pwai->ima == IMA_SCREENSAVER)
    ScreenSaver.Free();
}

/* ************************************************************************* */
/* * call an userdefined function                                          * */
/* ************************************************************************* */
/* * nr ...... number of function                                          * */
/* * op ...... parameter for function                                      * */
/* ************************************************************************* */
_BOOL save_bmpgif(_ASCII *dpne, _UDWORD color)
{
  _ROOM room;
  GetScreenSize(&room);
  return(save_gfx(dpne, room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, color));
}

#ifdef UC_DIRECTORY
_UDWORD globop;
#endif

void _Window::UserCall(_DWORD no, _DWORD op)
{
  _ASCII dp[50], ne[20], tmp[10];
  _UDWORD color;

  switch(no)
  {
    case 100:
				 #ifdef UC_DIRECTORY
				  globop = op;
				  NewDirectorySetup(SAVE, (_ASCII*)"*.TXT", (_FBOOL_PT_PT)save_toolcat, &globop, NULL);
				 #else
				  save_toolcat("C:\\TOOLCAT.TXT", (_UDWORD*)&op);
				 #endif
				  break;
    case 101:
				 #ifdef UC_DIRECTORY
				  globop = op;
				  NewDirectorySetup(LOAD, (_ASCII*)"A:\\*.TXT", (_FBOOL_PT_PT)load_toolcat, &globop, NULL);
				 #else
				  load_toolcat("C:\\TOOLCAT.TXT", (_UDWORD*)&op);
				 #endif
				  break;
    case 102: save_toolcat("C:\\TOOLCAT.TXT", (_UDWORD*)&op);
              break;
    case 103: load_toolcat("C:\\TOOLCAT.TXT", (_UDWORD*)&op);
              break;
    case 104:
    case 105:
    case 106: color = 65536L;
              StrCpy(dp, sizeof(_ASCII), "C:\\", sizeof(_ASCII));
              if(no == 104)
              {
                StrCpy(ne, sizeof(_ASCII), "P00000??.BMP", sizeof(_ASCII));
              }
              else if(no == 105)
              {
                StrCpy(ne, sizeof(_ASCII), "P00000??.GIF", sizeof(_ASCII));
              }
              else
              {
                if(JpgFunctSave != NULL)
                {
                  StrCpy(ne, sizeof(_ASCII), "P00000??.JPG", sizeof(_ASCII));
                  color = 100; // kompressionsrate
                }
                else
                {
                  StrCpy(ne, sizeof(_ASCII), "P00000??.BMO", sizeof(_ASCII));
                }
              }

              DToA(tmp, WhoAmI.no, 0x0105, sizeof(_ASCII));
              MemCpy(&ne[1], tmp, 5);
              if(create_filename(dp, ne) == _TRUE)
              {
                StrCat(dp, sizeof(_ASCII), ne, sizeof(_ASCII));
                save_bmpgif(dp, color);
              }
              break;
    case 107:
    case 108: ConfigGraphicSystem((no == 108)? GRC_SET_ORIGINCOLOR : GRC_SET_NAVYCOLOR, 0, NULL);
              Root.RedrawScreen(NULL);
              Root.FindTop()->AddCmd(CMD_OUTSOFTKEY, 0, 0, _FALSE);    // draw softkeys
              break;

   #ifndef CUTOFF_USER
    default : if(MethodLseUserCall != NULL)
              {
                load_this(LseThisPointer);
                MethodLseUserCall(no, op);
              }
              break;
   #endif
  }
}

/* ************************************************************************* */
/* * out io                                                                * */
/* ************************************************************************* */
/* * pio ..... --> to ioinformation                                        * */
/* * pr ...... --> to result (value)                                       * */
/* * pv ...... --> to variable                                             * */
/* * input ... _TRUE if io is an input, on the other hand it is an output  * */
/* ************************************************************************* */
/* * return _FALSE if the io should not be drawn by the kernel             * */
/* ************************************************************************* */
_BOOL _SubPicture::OutIO(_IO *pio, _RESULT *pr, _VARIABLE *pv, _BOOL input)
{
  _BOOL retcode = _TRUE;

 #ifdef OK_ENCLOSE_NAME
  if(pio->virtname.pthis != NULL)
  {
    Font.Set(pio->font, &pio->attrib);
    switch(call_enclosed_name_method(METHOD_MYIO_DRAW, pio, pv, pr, NULL, NULL, input))
    {
      case _IDIDIT  :
      case _IFAILED : retcode = _FALSE; break;
      case _IDLE    : break;
    }
  }
 #endif

  return(retcode);
}


/* ************************************************************************* */
/* * check if input is accessible for editor                               * */
/* ************************************************************************* */
/* * pi ...... --> to inputinformation                                     * */
/* ************************************************************************* */
/* * return _TRUE if input is accessible, on the other hand _FALSE         * */
/* ************************************************************************* */
_BOOL access_input(_INPUT *pi, _ACCESSFAILED *paf)
{
  _BOOL   retcode = _FALSE;
  _UDWORD actin;

  *paf = CHKBIT_FAILED;

  if(chk_CHKBIT(&pi->io.chkbit) == _TRUE)
  {
    *paf = STATESCHEME_FAILED;

    actin  = STATE_ACTIVE;
    if(StateScheme.Call(&actin, &pi->io.stscheme, NULL, STATE_ACTIVE) == _FALSE)
      actin = STATESCHEME_PRESELECTION;

    if((actin != STATE_INVISIBLE)&&(actin != STATE_INACTIVE))
    {
//      if(chk_ACCESS(pi->access))
      {
        if(VarList.GetStateScheme(pi->io.variable.info[0].value, NOCARE_DEFAULT) == STATE_ACTIVE)
        {
          retcode = _TRUE;
          *paf    = NOTHING_FAILED;

          if(SuperMode == _FALSE)
          {
           #ifndef CUTOFF_USER
            if(MethodLseAccessInput != NULL)
            {
              load_this(LseThisPointer);
              retcode = MethodLseAccessInput((void*)pi);
              if(retcode == _FALSE)
                *paf = USERIF_FAILED;
            }
           #endif
          }
        }
      }
    }
  }

  return(retcode);
}

/* ************************************************************************* */
/* * check userlevel                                                       * */
/* ************************************************************************* */
/* * ena ..... required userlevel (access information)                     * */
/* ************************************************************************* */
/* * return _TRUE if required userlevel fits, on the other hand _FALSE     * */
/* ************************************************************************* */
_BOOL chk_ACCESS(_UWORD access, _BOOL system)
{
  _BOOL        retcode = _TRUE;
  _DWORD       tmp;

  if((access != DEFACCESS)&&(SuperMode == _FALSE))
  {
    tmp = access;
   #ifdef USER_DEMAG
    if(system == _TRUE)
      tmp |= 0x10000L;
   #endif

   #ifndef CUTOFF_USER
    if(MethodLseCheckEnable != NULL)
    {
      load_this(LseThisPointer);
      retcode = MethodLseCheckEnable(tmp);
    }
   #endif
  }

  return(retcode);
}

/* ************************************************************************* */
/* * system error                                                          * */
/* ************************************************************************* */
/* * err ..... errorcode                                                   * */
/* * op ...... operand                                                     * */
/* * txt ..... individual text                                             * */
/* ************************************************************************* */
/* * return _FALSE if the application should stop immediately              * */
/* ************************************************************************* */
_BOOL _Window::SystemError(_SYSERROR err, _DWORD op, _CHAR *txt)
{
  _BOOL retcode = _TRUE;

  SystemMessage = err;

 #ifndef CUTOFF_USER
  _FBOOL_DWORD_DWORD_PT ptr;

  ptr = (_FBOOL_DWORD_DWORD_PT)find_lasal_method(LseThisPointer, METHOD_LSE_SYSTEMERROR);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    retcode = ptr(err, op, (void*)txt);
  }
 #endif

//  _DWORD winno;
//  if((VarList.SetValue(VarList.GetVarNo("System.Error.No"), err) == _TRUE) && // set errorno
//     (VarList.SetValue(VarList.GetVarNo("System.Error.Op"), op) == _TRUE) && // set parameter
//     (VarList.GetValue(VarList.GetVarNo("System.Error.Window"), &winno) == _TRUE))
//  {
//    AddCmd(CMD_NEWWINDOW, winno, 0);   // add command
//  }

  return(retcode);
}


/* ************************************************************************* */
/* * check if button is accessible                                         * */
/* ************************************************************************* */
/* * pb ...... --> to buttoninformation                                    * */
/* ************************************************************************* */
/* * return _TRUE if button is accessible, on the other hand _FALSE        * */
/* ************************************************************************* */

_BOOL access_button(_BUTTON *pb, _BOOL system, _SELECTOR selector)
{
  _BOOL   retcode = _FALSE;
  _UDWORD tmp1, tmp;
 #ifdef USER_BILLION
  #define DEF_TRYTOACCESS
 #endif
 #ifdef USER_DEKEMA
  #define DEF_TRYTOACCESS
 #endif
 #ifdef DEF_TRYTOACCESS
  _ACCESSFAILED  failed = CHKBIT_FAILED;
 #endif

  if((valid_SELECTOR(selector, pb->selector) == _TRUE)&&(chk_CHKBIT(&pb->chkbit) == _TRUE))
  {
   #ifdef DEF_TRYTOACCESS
    failed = STATESCHEME_FAILED;
   #endif

    if(StateScheme.Call(&tmp, &pb->stscheme, NULL, STATE_ACTIVE) == _FALSE)
      tmp = STATESCHEME_PRESELECTION;

    if(is_server_defined(&pb->varlabel) == _TRUE)
    {
      tmp1 = VarList.GetStateScheme(pb->varlabel.info[0].value, NOCARE_DEFAULT);
      if(tmp1 > tmp)
        tmp = tmp1;
    }

    if(tmp == STATE_ACTIVE)
    {
     #ifdef DEF_TRYTOACCESS
      failed = ACCESS_FAILED;
     #endif

      if(chk_ACCESS(pb->access, system) == _TRUE)
      {
       #ifdef DEF_TRYTOACCESS
        failed  = NOTHING_FAILED;
       #endif
        retcode = _TRUE;

       #ifndef CUTOFF_USER
        if(MethodLseAccessButton != NULL)
        {
          load_this(LseThisPointer);
          retcode = MethodLseAccessButton((void*)pb);
         #ifdef DEF_TRYTOACCESS
          if(retcode == _FALSE)
            failed = USERIF_FAILED;
         #endif
        }
       #endif
      }
    }
  }

#ifdef DEF_TRYTOACCESS
 #ifndef CUTOFF_USER
  if(MethodLseTryToAccess != NULL)
  {
    load_this(LseThisPointer);
    retcode = MethodLseTryToAccess((void*)pb, failed, FOUND_BUTTON, retcode);
  }
 #endif
 #undef DEF_TRYTOACCESS
#endif

  return(retcode);
}

void Beep(_WHOBEEP whobeep)
{
  _FVOID_WHOBEEP ptr = (_FVOID_WHOBEEP)find_lasal_method(LseThisPointer, METHOD_LSE_BEEP);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    ptr(whobeep);
  }
}

_DWORD MulDiv(_DWORD a, _DWORD b)
{
  if(b != 0)
  {
    float af = a;
    float bf = b;
    float rf = af / bf;
    return (_DWORD)((rf < 0)? (rf-0.5) : (rf+0.5));
  
//    if(b < 0)
//    {
//      a = -a;
//      b = -b;
//    }
//    
//    if(a >= 0)
//      return((a+(b/2))/b);
//    return((a-(b/2))/b);
  }
  return 0x7FFFFFFF;
}

#ifdef UC_REAL
_BOOL make_same_format(_RESULT *p1, _RESULT *p2)
{
  _BOOL retcode = _FALSE;

  if((p1->ftype == RES_REAL)||(p2->ftype == RES_REAL))
  {
    if((p1->ftype == RES_CONSTANT)||(p1->ftype == RES_NUMERIC))
    {
     #ifdef UC_IPC
      move_pREAL_DWORD((_REAL*)&p1->value, p1->value);
     #else
      dword2real((_REAL*)&p1->value, p1->value);
     #endif
      p1->ftype = RES_REAL;
    }

    if((p2->ftype == RES_CONSTANT)||(p2->ftype == RES_NUMERIC))
    {
     #ifdef UC_IPC
      move_pREAL_DWORD((_REAL*)&p2->value, p2->value);
     #else
      dword2real((_REAL*)&p2->value, p2->value);
     #endif
      p2->ftype = RES_REAL;
    }
  }

  if(p1->ftype == p2->ftype)
    retcode = _TRUE;

  return(retcode);
}
#endif

_BOOL formula_01(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  _RESULT   res;
  _VARIABLE var;
  _BOOL     retcode = _FALSE;

  if(VarList.GetSystemData(&res, &var, &pv->info.parameter, _FALSE, _TRUE) == _TRUE)
  {
   #ifdef UC_REAL
    if((pr->ftype == RES_REAL)||(res.ftype == RES_REAL))
    {
      _RESULT rtmp = *pr;

      make_same_format(&rtmp, &res);
      if(direction == _TRUE)
      {
       #ifdef UC_IPC
        move_pREAL_REAL((_REAL*)&rtmp.value, get_pREAL((_REAL*)&rtmp.value) * get_pREAL((_REAL*)&res.value));
//        *(_REAL*)&rtmp.value = *(_REAL*)&rtmp.value * (*(_REAL*)&res.value);
       #else
        RealCalc((_REAL*)&rtmp.value, (_REAL*)&rtmp.value, (_REAL*)&res.value, IOP_A_MULTI);
       #endif
        retcode = _TRUE;
      }
      else if(res.value != 0)
      {
       #ifdef UC_IPC
        move_pREAL_REAL((_REAL*)&rtmp.value, get_pREAL((_REAL*)&rtmp.value) / get_pREAL((_REAL*)&res.value));
//        *(_REAL*)&rtmp.value = *(_REAL*)&rtmp.value / (*(_REAL*)&res.value);
       #else
        RealCalc((_REAL*)&rtmp.value, (_REAL*)&rtmp.value, (_REAL*)&res.value, IOP_A_DIV);
       #endif
        retcode = _TRUE;
      }

      if(retcode == _TRUE)
      {
        if(pr->ftype == RES_REAL)
        {
//          *(_REAL*)&pr->value = *(_REAL*)&rtmp.value;
          move_pREAL_pREAL((_REAL*)&pr->value, (_REAL*)&rtmp.value);
        }
        else
        {
         #ifdef UC_IPC
          pr->value = FTOL(get_pREAL((_REAL*)&rtmp.value));
         #else
          pr->value = ftol_((_REAL*)&rtmp.value);
         #endif
        }
      }

      return(_TRUE);
    }
   #endif

    if((res.ftype == RES_CONSTANT) || (res.ftype == RES_NUMERIC))
    {
      if(direction == _TRUE)
      {
        pr->value = pr->value * res.value;
        retcode   = _TRUE;
      }
      else if(res.value != 0)
      {
        pr->value = MulDiv(pr->value, res.value);
        retcode   = _TRUE;
      }
    }
  }

  return(retcode);
}

_BOOL formula_02(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  return(formula_01(pr, pv, (direction==_TRUE)? _FALSE:_TRUE));
}
#ifdef OLDCALCULATOR
_BOOL formula_stroke2mm(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  // lasal grundeinheit : 10E-6 m
  _DWORD para = 1000;

 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    if(direction == _TRUE) // stroke -> mm
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) / 1000);
    else  // inch -> stroke
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) * 1000);
   #else
    _REAL tmp = 1000.0;
    if(direction == _TRUE) // stroke -> mm
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_DIV);
    else  // inch -> stroke
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_MULTI);
   #endif

    return(_TRUE);
  }
 #endif

  switch(pv->info.posdezpt)
  {
    case 1 : para = 100; break;
    case 2 : para =  10; break;
    case 3 : para =   1; break;
  }

  if(direction == _TRUE) // stroke -> mm
    pr->value = MulDiv(pr->value, para);
  else
    pr->value = pr->value * para;

  return(_TRUE);
}

_BOOL formula_stroke2inch(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  // lasal grundeinheit : 10E-6 m
  _DWORD mul = 25400;
  _DWORD div = 0;

 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    if(direction == _TRUE) // stroke -> inch
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) / 25400.0);
    else  // inch -> stroke
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) * 25400.0);
   #else
    _REAL tmp = 25400.0;

    if(direction == _TRUE) // stroke -> inch
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_DIV);
    else  // inch -> stroke
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_MULTI);
   #endif

    return(_TRUE);
  }
 #endif

  switch(pv->info.posdezpt)
  {
    case 1 : div =  0; mul = 2540; break;
    case 2 : div =  0; mul =  254; break;
    case 3 : div = 10; mul =  254; break;
  }

  if(direction == _TRUE) // stroke -> inch
  {
    if(div != 0)
      pr->value = pr->value * 10;
    pr->value = MulDiv(pr->value, mul);
  }
  else // inch -> stroke
  {
    pr->value = (pr->value * mul);
    if(div != 0)
      pr->value = pr->value / div;
  }

  return(_TRUE);
}

_BOOL formula_temp2fahrenheit(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  // lasal grundeinheit : 10E-1 Grad C
  // [C] = ([F]-32)*5/9
  _DWORD value;

 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    _REAL rval = get_pREAL((_REAL*)&pr->value);
    if(direction == _TRUE)
      move_pREAL_REAL((_REAL*)&pr->value, 32.0 + ((rval * 9.0) / 50.0));
    else
      move_pREAL_REAL((_REAL*)&pr->value, ((rval-32.0)*50.0) / 9.0);
   #else
    _REAL tmp0, tmp1;
    if(direction == _TRUE)
    {
      tmp0 = 9.0;
      RealCalc(&tmp1, (_REAL*)&pr->value, &tmp0, IOP_A_MULTI);
      tmp0 = 50.0;
      RealCalc(&tmp1, &tmp1, &tmp0, IOP_A_DIV);
      tmp0 = 32.0;
      RealCalc((_REAL*)&pr->value, &tmp1, &tmp0, IOP_A_PLUS);
    }
    else
    {
      tmp0 = 32.0;
      RealCalc(&tmp1, (_REAL*)&pr->value, &tmp0, IOP_A_MINUS);
      tmp0 = 50.0;
      RealCalc(&tmp1, &tmp1, &tmp0, IOP_A_MULTI);
      tmp0 = 9.0;
      RealCalc((_REAL*)&pr->value, &tmp1, &tmp0, IOP_A_DIV);
    }
   #endif

    return(_TRUE);
  }
 #endif

  value = pr->value;

  if(direction == _TRUE)
  {
    value = MulDiv(value * 90, 50) + 320;

    if(pv->info.posdezpt == 0)
      value = MulDiv(value, 10);
  }
  else
  {
    if(pv->info.posdezpt == 0)
      value = (value - 32) * 10;
    else
      value = value - 320;

    value = MulDiv(value * 50, 90);
  }

  pr->value = value;

  return(_TRUE);
}

_BOOL formula_temp2celsius(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  // lasal grundeinheit : 10E-1 Grad C

 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    if(direction == _TRUE)
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) / 10.0);
    else
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL(((_REAL*)&pr->value) * 10.0);
   #else
    _REAL tmp = 10.0;
    if(direction == _TRUE)
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_DIV);
    else
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_MULTI);
   #endif

    return(_TRUE);
  }
 #endif

  if(direction == _TRUE)
  {
    switch(pv->info.posdezpt)
    {
      case 0 : pr->value = MulDiv(pr->value, 10); break;
      case 2 : pr->value = pr->value * 10; break;
    }
  }
  else
  {
    switch(pv->info.posdezpt)
    {
      case 0 : pr->value = pr->value * 10; break;
      case 2 : pr->value = MulDiv(pr->value, 10); break;
    }
  }

  return(_TRUE);
}

_BOOL formula_time2second(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  // lasal grundeinheit : 10E-3 s

  _DWORD teiler;

 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    if(direction == _TRUE)
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) / 1000.0);
    else
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) * 1000.0);
   #else
    _REAL tmp = 1000.0;
    if(direction == _TRUE)
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_DIV);
    else
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_MULTI);
   #endif
    return(_TRUE);
  }
 #endif

  if(direction == _TRUE)
  {
    switch(pv->info.posdezpt)
    {
      case  0 : teiler = 1000; break;
      case  1 : teiler = 100; break;
      case  2 : teiler = 10; break;
      default : teiler = 1; break;
    }

    pr->value = MulDiv(pr->value, teiler);
  }
  else
  {
    switch(pv->info.posdezpt)
    {
      case  0 : teiler = 1000; break;
      case  1 : teiler = 100; break;
      case  2 : teiler = 10; break;
      default : teiler = 1; break;
    }

    pr->value = pr->value * teiler;
  }

  return(_TRUE);
}

_BOOL formula_time2minute(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  _DWORD teiler;

  // lasal grundeinheit : 10E-3 s
 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    if(direction == _TRUE)
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL(_REAL*)&pr->value) / 60000.0);
    else
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) * 60000.0);
   #else
    _REAL tmp = 60000.0;
    if(direction == _TRUE)
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_DIV);
    else
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_MULTI);
   #endif
    return(_TRUE);
  }
 #endif

  if(direction == _TRUE)
  {
    switch(pv->info.posdezpt)
    {
      case  0 : teiler = 60000; break;
      case  1 : teiler = 6000; break;
      case  2 : teiler = 600; break;
      default : teiler = 60; break;
    }

    pr->value = MulDiv(pr->value, teiler);
  }
  else
  {
    switch(pv->info.posdezpt)
    {
      case  0 : teiler = 60000; break;
      case  1 : teiler = 6000; break;
      case  2 : teiler = 600; break;
      default : teiler = 60; break;
    }

    pr->value = pr->value * teiler;
  }

  return(_TRUE);
}

_BOOL formula_time2hour(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  _DWORD teiler;

  // lasal grundeinheit : 10E-3 s

 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    if(direction == _TRUE)
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) / 3600000.0);
    else
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) * 3600000.0);
   #else
    _REAL tmp = 3600000.0;
    if(direction == _TRUE)
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_DIV);
    else
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_MULTI);
   #endif

    return(_TRUE);
  }
 #endif

  if(direction == _TRUE)
  {
    switch(pv->info.posdezpt)
    {
      case  0 : teiler = 3600000; break;
      case  1 : teiler = 360000; break;
      case  2 : teiler = 36000; break;
      default : teiler = 3600; break;
    }

    pr->value = MulDiv(pr->value, teiler);
  }
  else
  {
    switch(pv->info.posdezpt)
    {
      case  0 : teiler = 3600000; break;
      case  1 : teiler = 360000; break;
      case  2 : teiler = 36000; break;
      default : teiler = 3600; break;
    }

    pr->value = pr->value * teiler;
  }

  return(_TRUE);
}

_BOOL formula_force2kN(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  // lasal grundeinheit : 10E0 N (Newton)

  _DWORD teiler;

 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    if(direction == _TRUE)
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) / 1000.0);
    else
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) * 1000.0);
   #else
    _REAL tmp = 1000.0;
    if(direction == _TRUE)
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp,  IOP_A_DIV);
    else
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp,  IOP_A_MULTI);
   #endif
    return(_TRUE);
  }
 #endif

  if(direction == _TRUE)
  {
    switch(pv->info.posdezpt)
    {
      case  0 : teiler = 1000; break;
      case  1 : teiler = 100; break;
      case  2 : teiler = 10; break;
      default : teiler = 1; break;
    }

    pr->value = MulDiv(pr->value, teiler);
  }
  else
  {
    switch(pv->info.posdezpt)
    {
      case  0 : teiler = 1000; break;
      case  1 : teiler = 100; break;
      case  2 : teiler = 10; break;
      default : teiler = 1; break;
    }

    pr->value = pr->value * teiler;
  }

  return(_TRUE);
}

_BOOL formula_weight2kilogram(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  // lasal grundeinheit : 10E-1 gramm

  _DWORD div = 1;

 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    if(direction == _TRUE)
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) / 10000.0);
    else
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) * 10000.0);
   #else
    _REAL tmp = 10000.0;
    if(direction == _TRUE)
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp,  IOP_A_DIV);
    else
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp,  IOP_A_MULTI);
   #endif
    return(_TRUE);
  }
 #endif

  switch(pv->info.posdezpt)
  {
    case 0 : div = 10000; break;
    case 1 : div =  1000; break;
    case 2 : div =   100; break;
    case 3 : div =    10; break;
  }

  if(direction == _TRUE)
  {
    pr->value = MulDiv(pr->value, div);
  }
  else
  {
    pr->value = pr->value * div;
  }

  return(_TRUE);
}

_BOOL formula_weight2pound(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  // lasal grundeinheit : 10E-1 gramm

  _DWORD div = 1;
  _DWORD value;

 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    _REAL rval = get_pREAL((_REAL*)&pr->value);
    if(direction == _TRUE)
      move_pREAL_REAL((_REAL*)&pr->value, (rval * 2.205) / 10000.0);
    else
      move_pREAL_REAL((_REAL*)&pr->value, (rval * 10000.0) /  2.205);
   #else
    _REAL tmp0, tmp1;
    if(direction == _TRUE)
    {
      tmp0 = 2.205;
      RealCalc(&tmp1, (_REAL*)&pr->value, &tmp0, IOP_A_MULTI);
      tmp0 = 10000.0;
      RealCalc((_REAL*)&pr->value, &tmp1, &tmp0, IOP_A_DIV);
    }
    else
    {
      tmp0 = 10000.0;
      RealCalc(&tmp1, (_REAL*)&pr->value, &tmp0, IOP_A_MULTI);
      tmp0 = 2.205;
      RealCalc((_REAL*)&pr->value, &tmp1, &tmp0, IOP_A_DIV);
    }
   #endif
    return(_TRUE);
  }
 #endif

  switch(pv->info.posdezpt)
  {
    case 0 : div = 10000; break;
    case 1 : div =  1000; break;
    case 2 : div =   100; break;
    case 3 : div =    10; break;
  }

  if(direction == _TRUE)
  {
    value = MulDiv(pr->value * 2205, 1000);
    pr->value = MulDiv(value, div);
  }
  else
  {
    pr->value = MulDiv(pr->value * 1000, 2205) * div;
  }

  return(_TRUE);
}

_BOOL formula_pressure2bar(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  // lasal grundeinheit : 10E-1 bar

  _DWORD para = 1;

 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    if(direction == _TRUE)
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) / 10.0);
    else
      move_pREAL_REAL((_REAL*)&pr->value, get_pREAL((_REAL*)&pr->value) * 10.0);
   #else
    _REAL tmp = 10.0;
    if(direction == _TRUE)
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_DIV);
    else
      RealCalc((_REAL*)&pr->value, (_REAL*)&pr->value, &tmp, IOP_A_MULTI);
   #endif
    return(_TRUE);
  }
 #endif

  if(pv->info.posdezpt == 0)
    para = 10;

  if(direction == _TRUE)
  {
    pr->value = pr->value * para;
  }
  else
  {
    pr->value = MulDiv(pr->value, para);
  }

  return(_TRUE);
}

_BOOL formula_pressure2psi(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  // lasal grundeinheit : 10E-1 bar
  // 1psi = 0.0689476 bar
  // 1bar = 14.50376808 psi
  _DWORD para;

 #ifdef UC_REAL
  if(pr->ftype == RES_REAL)
  {
   #ifdef UC_IPC
    _REAL rval = get_pREAL((_REAL*)&pr->value);
    if(direction == _TRUE)
    {
      move_pREAL_REAL((_REAL*)&pr->value, (rval * 14.50376808) / 10.0);
    }
    else
    {
      move_pREAL_REAL((_REAL*)&pr->value, (rval * 10.0) / 14.50376808);
    }
   #else
    _REAL tmp0, tmp1;
    if(direction == _TRUE)
    {
      tmp0 = 14.50376808;
      RealCalc(&tmp1, (_REAL*)&pr->value, &tmp0, IOP_A_MULTI);
      tmp0 = 10.0;
      RealCalc((_REAL*)&pr->value, &tmp1, &tmp0, IOP_A_DIV);
    }
    else
    {
      tmp0 = 10.0;
      RealCalc(&tmp1, (_REAL*)&pr->value, &tmp0, IOP_A_MULTI);
      tmp0 = 14.50376808;
      RealCalc((_REAL*)&pr->value, &tmp1, &tmp0, IOP_A_DIV);
    }
   #endif
    return(_TRUE);
  }
 #endif

  para = 1000;
  if(pv->info.posdezpt == 0)
    para = 10000;

  if(direction == _TRUE)
  {
    pr->value = MulDiv(pr->value * 14504, para);
  }
  else
  {
    pr->value = MulDiv(pr->value * para, 14504);
  }

  return(_TRUE);
}
#endif


/* ************************************************************************* */
/* * recalculate (formula)                                                 * */
/* ************************************************************************* */
/* * pr ......... --> to value                                             * */
/* * pv ......... --> to variable                                          * */
/* * direction .. direction (_TRUE Y=kX+d, _FALSE X=(Y+d)/k)               * */
/* ************************************************************************* */
_BOOL recalculate(_RESULT *pr, _VARIABLE *pv, _BOOL direction)
{
  _BOOL retcode = _TRUE;

  if(pv->info.formula != DEFFORMULA)
  {
    if((pr->ftype == RES_CONSTANT)||(pr->ftype == RES_NUMERIC)||(pr->ftype == RES_REAL))
    {
      if(MethodLseRecalculate != NULL)
      {
        load_this(LseThisPointer);
        retcode = MethodLseRecalculate((void*)pr, (void*)pv, direction);
      }

      // 1,2

      if(retcode == _TRUE)
      {
        switch(pv->info.formula)
        {
          case  1 : retcode = formula_01(pr, pv, direction); break; // mul
          case  2 : retcode = formula_02(pr, pv, direction); break; // div
         #ifdef OLDCALCULATOR
          case  3 : retcode = formula_stroke2mm(pr, pv, direction); break; // stroke <-> mm
          case  4 : retcode = formula_stroke2inch(pr, pv, direction); break; // stroke <-> inch
          case  5 : retcode = formula_temp2celsius(pr, pv, direction); break; // temp <-> celsius
          case  6 : retcode = formula_temp2fahrenheit(pr, pv, direction); break; // temp <-> fahrenheit
          case  7 : retcode = formula_weight2kilogram(pr, pv, direction); break; // weight <-> kilogram
          case  8 : retcode = formula_weight2pound(pr, pv, direction); break; // weight <-> pound
          case  9 : retcode = formula_pressure2bar(pr, pv, direction); break; // pressure <-> bar
          case 10 : retcode = formula_pressure2psi(pr, pv, direction); break; // pressure <-> psi
          case 11 : retcode = formula_time2second(pr, pv, direction); break; // time <-> second
          case 12 : retcode = formula_time2minute(pr, pv, direction); break; // time <-> minute
          case 13 : retcode = formula_time2hour(pr, pv, direction); break; // time <-> hour
          case 14 : retcode = formula_force2kN(pr, pv, direction); break; // force <-> kN
         #endif
        }
      }
      else
      {
        retcode = _TRUE;
      }
    }
  }

  return(retcode);
}

/* ************************************************************************* */
/* * pe ......... --> to editor (including bubblehelp-area)                * */
/* ************************************************************************* */
/* * return _FALSE if bubblehelp-window should not be drawn by the kernel  * */
/* ************************************************************************* */
_BOOL DrawBubbleHelp(_EDITOR *pe)
{
  _BOOL retcode = _TRUE;

 #ifndef CUTOFF_USER
  _FBOOL_PT ptr;

  ptr = (_FBOOL_PT)find_lasal_method(LseThisPointer, METHOD_LSE_BUBBLEHELP);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    retcode = ptr((void*)pe);
  }
 #endif

  return(retcode);
}

/* ************************************************************************* */
/* * Pm ......... --> to structure _MENU                                   * */
/* * no ......... --> to number of popup-menu                              * */
/* ************************************************************************* */
/* * return _TRUE if initialisation is done by user (else _FALSE)          * */
/* ************************************************************************* */
_BOOL popup_call(_MENU *Pm, _UWORD *no)
{
  _BOOL retcode = _FALSE;

 #ifndef CUTOFF_USER
  _FBOOL_PT_PT ptr;

  ptr = (_FBOOL_PT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_CALLPOPUP);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    retcode = ptr((void*)Pm, no);
  }
 #endif

  return(retcode);
}

/* ************************************************************************* */
/* * this function is called after a succesfull writing of an input        * */
/* * pi ......... --> to saved _INPUT                                      * */
/* * pr ......... --> to _RESULT                                           * */
/* ************************************************************************* */
void input_is_saved(_INPUT *pi, _RESULT *pr)
{
 #ifndef CUTOFF_USER
  _FVOID_PT_PT ptr;

  ptr = (_FVOID_PT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_INPUTSAVED);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    ptr(pi, pr);
  }
 #endif

}

void found_something_by_hid(_EVENT *pe, _FOUNDHID hp)
{
  pe->already_used++;
  
 #ifndef CUTOFF_USER
  if(MethodLseFoundSomethingByHid != NULL)
  {
    load_this(LseThisPointer);
    MethodLseFoundSomethingByHid(hp);
  }
 #endif
}

void zero_screensaver(void)
{
  ScrSvrLastEvent = get_abstime();
}

void reset_screensaver(void)
{
  if(Root.FindTop()->GetWhoAmI().ima == IMA_SCREENSAVER)
    Root.FindTop()->AddCmd(CMD_RETURNQUEUE, 0, 0, _FALSE);

  zero_screensaver();
}



_BOOL user_station(_UWORD no, _COMDEF *cd, _BOOL *online_watch)
{
 #ifndef CUTOFF_USER
  _BOOL retcode = _TRUE;
  _FBOOL_UWORD_PT_PT ptr;
  ptr = (_FBOOL_UWORD_PT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_USERSTATION);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    retcode = ptr(no, cd, online_watch);
  }
  return retcode;
 #else
  return _TRUE;
 #endif
}



/*
_BOOL read_files(_ASCII_BUFFER *dest, _ASCII *dp, _ASCII *ne)
{
  _DDE_INFO dde;
  _DWORD    hand;
  _ASCII    name[50];
  _UWORD    i, len;
  _WORD     run = -1;

  free_ASCII_BUFFER(dest);

  hand = FindFirst(dp, ne, &dde, 0, 0);
  if(hand >= 0)
    run = 0;

  while(run >= 0)
  {
    if((dde.FileName[0] != 0) &&
       (dde.Attributes & _DDE_ARCHIV) &&
       ((dde.Attributes & _DDE_HIDDEN) == 0) &&
       ((dde.Attributes & _DDE_SYSTEM) == 0) &&
       ((dde.Attributes &_DDE_VOLUME) == 0)
      )
    {
      // Datum eintragen
      name[0] = '[';
      DToA(&name[1], dde.DateTime.Day, 0x0102, sizeof(_ASCII));
      name[3] = '.';
      DToA(&name[4], dde.DateTime.Month, 0x0102, sizeof(_ASCII));
      name[6] = '.';
      DToA(&name[7], dde.DateTime.Year % 100, 0x0102, sizeof(_ASCII));
      name[9]  = ']';
      name[10] = ' ';

      // Filename eintragen
      MemCpy(&name[11], dde.FileName, 8);
      name[19] = 0;

      for(i=11; i<19; i++)
      {
        if(name[i] == ' ')
        {
          name[i] = 0;
          break;
        }
      }

      // Extension eintragen
      if((dde.Extension[0] != 0)&&(dde.Extension[0] != ' '))
      {
        StrCat(name, sizeof(_ASCII), ".", sizeof(_ASCII));
        len = StrLen(name, sizeof(_ASCII));
        MemCpy(&name[len], dde.Extension, 3);
        name[len+3] = 0;
      }

      // kompletten Namen in Puffer eintragen
      len = StrLen(name, sizeof(_ASCII)) + 1;
      if(Memory2((void**)&dest->ptr, dest->size + len) != 0)
      {
        MemCpy(&dest->ptr[dest->size], name, len);  // --> auf ASCII-0-Texte
        dest->size += len;  // anzahl der ASCII-Zeichen
        dest->no   += 1;    // anzahl der ASCII-0-Texte
      }
    }

    run = FindNext(hand, &dde);
  }

  FindClose(hand);

  return(_TRUE);
}
*/


