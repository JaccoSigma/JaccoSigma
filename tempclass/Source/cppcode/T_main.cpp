#include "T_IPC.H"
#include "T_NUC.H"
#include "COMINTFC.H"

#ifdef UC_IPC
_UDWORD microdiff(_UDWORD act)
{
  return(get_microtime() - act);
}

void WatchStart(void)
{
  LseTime.time = get_microtime();
}

_UDWORD WatchSleep(void)
{
  return(get_microtime());
}

void WatchAwake(_UDWORD time)
{
  LseTime.time += (get_microtime() - time);
}

void WatchEnd(void)
{
  _UDWORD diff, timer;


  if(LseTime.ref == 0)
  {
    LseTime.sum = 0;
    LseTime.average = 0;
    LseTime.count = 0;
    LseTime.max = 0;
    LseTime.min = 0xFFFFFFFF;
    timer = LseTime.timebase;
  }
  else
  {
    timer = microdiff(LseTime.ref);
  }

  diff = microdiff(LseTime.time);

  LseTime.sum += diff;
  LseTime.count += 1;
  LseTime.average = LseTime.sum / LseTime.count;

  if(diff > LseTime.max)
    LseTime.max = diff;
  if(diff < LseTime.min)
    LseTime.min = diff;

  if(timer >= LseTime.timebase)
  {
    LseTime.timemin = LseTime.min;
    LseTime.timemax = LseTime.max;
    LseTime.timeaverage = LseTime.average;

/*
    StrCpy(tmp, 1, "Max:", 1);
    DToA(&tmp[4], LseTime.max, 0x830, 1);
    StrCat(tmp, 1, "    ", 1);
    Debug(tmp, 1, 10);

    StrCpy(tmp, 1, "Min:", 1);
    DToA(&tmp[4], LseTime.min, 0x830, 1);
    StrCat(tmp, 1, "    ", 1);
    Debug(tmp, 1, 11);

    StrCpy(tmp, 1, "Ave:", 1);
    DToA(&tmp[4], LseTime.average, 0x830, 1);
    StrCat(tmp, 1, "    ", 1);
    Debug(tmp, 1, 12);
*/

    LseTime.sum = 0;
    LseTime.average = 0;
    LseTime.count = 0;
    LseTime.max = 0;
    LseTime.min = 0xFFFFFFFF;
    LseTime.ref = get_microtime();
  }
}
#endif

void shut_down(void);
/*
//_UDWORD blinktime = 0;
_UBYTE  blink     = 0;
_BOOL blinkstatus(void)
{
  return((blink == 1)? _TRUE : _FALSE);
}
*/

void *opsys_cilget(_ASCII *name)
{
  void *retcode = NULL;

  if(OS_CILGet(name, (void**)&retcode) != SYS_ERR_NONE)
    retcode = NULL;

  return(retcode);
}

_UDWORD GetUniquePlcId(void)
{
  typedef struct 
  { 
    void      *udVersion;
    void      *Reserved;
    void      *SernumGetPLC;
    void      *SernumGetPLCDrive;
    _FPT_VOID SernumGetPLCInfo;
  } _PLC_SYS;

  typedef struct 
  { 
    _UDWORD udVersion;
    _ASCII  BiosVersion[16];
    _ASCII  SerialNumber[24];
    _ASCII  szApplication[128];
  } _PLC_INFO;

  _PLC_SYS  *psys;
  _PLC_INFO *pinfo;
  _UDWORD   retcode = 255;
  _UWORD    len;
        
  psys = (_PLC_SYS*)opsys_cilget((_ASCII*)"ISYSSERNUM");
  if(psys != NULL)
  {
    pinfo = (_PLC_INFO*)psys->SernumGetPLCInfo();
    if(pinfo != NULL)
    {
      len = StrLen(pinfo->SerialNumber, sizeof(_ASCII));
      if(len > 0)
      {
        if(len < 10)
          retcode = AToD(pinfo->SerialNumber, sizeof(_ASCII));
        else
          retcode = MakeCrc32(0, pinfo->SerialNumber, len+1);
      
        return retcode;
      }
    }
  }
  
  _File file;
  _ASCII dpne[_MAX_PATHLENGTH];
  
  StrCpy(dpne, sizeof(_ASCII), "RAM@", sizeof(_ASCII));
  StrCat(dpne, sizeof(_ASCII), Project.GetPrjPath(), sizeof(_ASCII));
  StrCat(dpne, sizeof(_ASCII), "UniqueId.bin", sizeof(_ASCII));
  
  if(file.Open(LOAD, dpne) == _TRUE)
  {
    file.Read(&retcode, 4);
    file.Close();
  }
  
  if(retcode == 255)
  {
    while(retcode == 255)
      retcode = Random(0xFFFFFFFF);
      
    if(file.Open(SAVE, dpne) == _TRUE)
    {
      file.Write(&retcode, 4);
      file.Close();
    }
  }
  
  return(retcode);
}  


_BOOL RTK_Init(void *pthis, _BOOL urinit)
{
//  _BOOL  retcode = _FALSE;
//  chk_function();

 #ifdef __TURBOC__
	InitGraph();
 #endif
  init_MAIN(urinit);
  LseThisPointer = pthis;

 #ifndef UC_RESETRECONNECT
  SYSMAE_SetExternFunction(NULL, (_FVOID_VOID)shut_down);
 #endif
 
  INSTALLCALLBACK((void*)irq_PLC);

  // OS-Interface ermitteln
  IKeyPad = (IKEYPAD*)opsys_cilget((_ASCII*)"KEYPAD");

 #ifdef XUSER_KM
  Batch.Read();                                                  // load batch
  AllSoftkeys.Scan();
 #endif

  init_LSE_TIME(&LseTime);

  return(_TRUE);
}

_BOOL RTK_Ready(void)
{
  _BOOL  retcode = _FALSE;
  _UWORD startpic;

  _UDWORD startimex = get_abstime();

  Palette.AskSystem();
  Palette.Set();
  ClrScr(BLACK);                                              // delete screen
 #ifndef UC_ROTATE
  if(OpSysIsSalamander())
  {
    // salamander funktioniert nicht ohne UC_ROTATE
    _ROOM room;
    GetScreenSize(&room);
    Annahme("Program will not run without #define UC_ROTATE", 0);
    RefreshRectangle(&room);
    while(1);
  }
 #else 
  _ROOM room;
  GetScreenSize(&room);
  RefreshRectangle(&room);
 #endif

 #ifndef XUSER_KM
  Batch.Read();                                                  // load batch
 #endif

  // get this pointer from lasal
//  get_TOOLDATA()->Pthis = get_thispointer(0);                       // ToolCat

 #ifdef OK_OFFSCREEN
  if(UseOffscreen == _TRUE)
  {
    _ROOM screen;
    GetScreenSize(&screen);
    CreateOffScreen(&OffScreen, screen.xy2.x+1-screen.xy1.x, screen.xy2.y+1-screen.xy1.y);
  }
 #endif

 #ifdef UC_IPC
  _BMP   ip;
  _ROOM  iproom;
  init_BMP_Makro(&ip);
  if(load_gfx(&ip, Batch.GetStartGfxDPNE(), NULL, NULL) == _TRUE)
  {
    GetScreenSize(&iproom);
    iproom.xy1.x = (iproom.xy1.x + iproom.xy2.x + 1 - ip.width) / 2;
    iproom.xy1.y = (iproom.xy1.y + iproom.xy2.y + 1 - ip.height) / 2;
    Bitmap(iproom.xy1.x, iproom.xy1.y, &ip, T_COPY|T_SOLID, 0);

   #ifdef UC_ROTATE
    iproom.xy2.x = iproom.xy1.x + ip.width;
    iproom.xy2.y = iproom.xy1.y + ip.height;
    RefreshRectangle(&iproom);
   #endif
    free_BMP(&ip);
  }
//  flash_gfx(0, 0, Batch.GetStartGfxDPNE());
// #ifdef UC_ROTATE
//  _ROOM screen1;
//  GetScreenSize(&screen1);
//  RefreshRectangle(&screen1);
// #endif
 #endif

  if(Project.Load(Batch.GetProjectDPNE()) == _TRUE)
  {
    startpic = Batch.GetStartPicture();

    VarList.GetID(_TRUE);                              // get all lasal id
    if(Project.GetPictureAccess(NULL, &startpic, _TRUE, NULL) == _TRUE)
    {
      Keyboard.Install();
      Alarm.Config();                                        // alarm configuration
      Alarm.GetTempBuffer();                // get all alarmrecords from tmp-buffer
      if(VarList.CheckHostname(_TRUE, _TRUE, _TRUE) == _TRUE) // check all hostname
      {
       #ifdef UC_MOVING_WINDOW
        WindowMoveProjectReady();
       #endif
       #ifdef XUSER_KM
//        AllSoftkeys.Scan();
        AllSoftkeys.AssignSoftInfo();
       #endif
       
       #ifdef UC_JAPANINPUT 
        Japan_cTor();
       #endif
       
       #ifdef UC_IPC
        ScanBar.Message("Startup: OK");
        //ScanBar.End(ScanBar.Add("Startup: OK", 0));
       #endif
       
        SetUp.StartupTimeLseProject = timediff(startimex);

        // kommando zum zeichnen des startbildschirms absetzen
       #ifdef NINETEEN
        DashBoard.SetReturn(&Root);
        Root.AddCmd(CMD_NEWPICTURE, NINETEEN, 0, _FALSE);
       #endif
        Root.AddCmd(CMD_NEWPICTURE, startpic, 0, _FALSE);
       
        if(Project.Ready(_TRUE) == _TRUE)          // interface: project ready
        {
          retcode = _TRUE;
          ClrScr(SetUp.GetBackgroundColor());
          Batch.ResetWatchOnlineTimeDelay();
          Keyboard.KillAllTouchEvents(); // SA38307
        }
        else
        {
          Root.DeleteAllCmd(); // alle kommandos entfernen
          ScanBar.Assert("BREAK: USER-INTERFACE...");
          //ScanBar.End(ScanBar.Add("BREAK: USER-INTERFACE...", 0));
        }
      }
      else
      {
        ScanBar.Assert("ERROR: BAD HOSTNAME...");
        //ScanBar.End(ScanBar.Add("ERROR: BAD HOSTNAME...", 0));
      }
    }
    else
    {
      ScanBar.Assert("ERROR: CAN'T FIND STARTPICTURE... (check ipc.ini)");
      //ScanBar.End(ScanBar.Add("ERROR: CAN'T FIND STARTPICTURE... (check ipc.ini)", 0));
    }
  }

//  set_keymode(53, _TIPSINGLE);

 #ifdef UC_INVISIBLE_PROGRESS
  ScanBar.Free();
 #endif

  return(retcode);
}

_BOOL RTK_Run(void)
{
  _BOOL retcode;

 #ifdef UC_IPC
  if(LseTime.valid == _TRUE)
    WatchStart();
 #endif

  TermAlarm.Run();                                   // ifnec collect alarms
 #ifndef CUTOFF_USER
  ServerWatch.Run();                                       // observe server
 #endif

 #ifdef UC_MOVING_WINDOW 
  retcode = _TRUE;
  if(WindowMoveRun() == _FALSE)
  {
    if(Root.RunIntern() != RUN_NOP)
    {
      retcode = _FALSE;
    }
  }
 #else // UC_MOVING_WINDOW
  retcode = (Root.RunIntern() == RUN_NOP)? _TRUE : _FALSE;
 #endif // UC_MOVING_WINDOW
  
  ToolCatalogue.Run();

  if(System.write == _TRUE)
  {
    IpcSettings.Save();
    System.write = _FALSE;
  }

 #ifdef UC_EXOONLINE
  ExoRun();
 #endif

 #ifdef UC_IPC
  if(LseTime.valid == _TRUE)
    WatchEnd();
 #endif

  Batch.StartRefreshList();



  return(retcode);
}

_BOOL RTK_End(_BOOL free_memory)
{

 #ifdef UC_RESETRECONNECT
  Lse_Reset(_TRUE);
 #endif

 #ifdef OK_OFFSCREEN
  if(UseOffscreen == _TRUE)
  {
    ActivateScreen(&OnScreen);               // switch back to original screen
  }
 #endif

//  IpcSettings.Save();
  TermAlarm.Save();                                   // save terminal alarm

  Root.EndIntern(_FALSE);
  Root.Free();

//  ClrScr(BLACK);                                              // delete screen
  Font.Set(255, NULL);
//  OutTextXY(10, 10, "APPLICATION END...", T_COPY|T_SOLID, MakeColor_Makro(BLACK, WHITE), sizeof(_ASCII));



//  IpcMemory.Save();                                     // save local data
//  Keyboard.DeInstall();
  Project.End();                                   // interface: project end
  Project.Free();
//  IpcMemory.Free();

  if(free_memory == _TRUE)
    free_MAIN();    // in rtk os not necessary

//  CloseGraph();

  free_LSE_TIME(&LseTime);

  return(_TRUE);
}

void _Window::PowerDown(void)
{
  if(P_Window != NULL)
  {
    P_Window->PowerDown();
    Memory2((void**)&P_Window, 0);
    Root.MakeRegion();
  }

  EndIntern(_FALSE);
  Free();
}

void shut_down(void)
{

  // function is called by irq (lasal reset or restart)

 #ifdef OK_OFFSCREEN
  if(UseOffscreen == _TRUE)
  {
    ActivateScreen(&OnScreen);
  }
 #endif

 #ifndef CUTOFF_USER
  _FVOID_VOID ptr;

  ptr = (_FVOID_VOID)find_lasal_method(LseThisPointer, METHOD_LSE_POWERDOWN);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    ptr();
  }
 #endif

  Root.PowerDown();
  RTK_End(_FALSE);
}

_BOOL PushMemColor(_UWORD idx)
{
  _BOOL retcode = _FALSE;

  if(MemoryIdx.cnt == 0)
  {
    MemoryIdx.psh = MemoryIdx.act;
    MemoryIdx.act = idx;
    retcode       = _TRUE;
  }

  MemoryIdx.cnt += 1;

  return(retcode);
}

_BOOL PopMemColor(void)
{
  _BOOL retcode = _FALSE;

  MemoryIdx.cnt -= 1;
  if(MemoryIdx.cnt == 0)
  {
    MemoryIdx.act = MemoryIdx.psh;
    retcode = _TRUE;
  }

  return(retcode);
}

/*
_UWORD Memory2x(void **dest, _UDWORD size)
{
//  _FUWORD_PT_UDWORD_UBYTE ptr = (_FUWORD_PT_UDWORD_UBYTE)lsl_pOS->piLSE->pMemoryV1;
//  return(lsl_pOS->piLSE->pMemoryV1(dest, size, 1));

//typedef _UWORD (*_FUWORD_PT_UDWORD_UBYTE) (void**, _UDWORD, _UBYTE);
//  _FUWORD_PT_UDWORD_UBYTE ptr = (_FUWORD_PT_UDWORD_UBYTE)*(_UDWORD*)(((_UBYTE*)(&lsl_pOS->piLSE->pOS_FindNextEx)) + 4);
//  return(ptr(dest, size, MemoryIdx));

  if(size == 0)
  {
   #ifdef _LSL_TARGETARCH_ARM
    if((void*)ReadU4((_UBYTE*)dest) == NULL)
   #else
    if(*dest == NULL)
   #endif
    {
      return(1); // schneller (böse)
    }
  }

  if(RTOSVERSION >= 0x111E)   // nur wenn opsys >= 01.01.030
    size |= (MemoryIdx.act << 28);

  if(Memory(dest, size) == 0)
  {
    SetTooLessMemory();
    return(0);
  }

  return(1);
}
*/
/*
typedef struct __MEMBLK
               { _UDWORD  size;
                 __MEMBLK *p_prev;
                 __MEMBLK *p_next;
                 _UDWORD  state;
               }
_MEMBLK;

class _MyHeap
{

  #define MEMUSED  1
  #define MEMFREE  0

  private :
    _MEMBLK  *Data;
    _UDWORD  DataSize;

    _UDWORD  MakeSize(_UDWORD);
    _MEMBLK *MakePb(void*);
    _MEMBLK *ScanForFreeBlock(_UDWORD);
    void     SplitBlock(_MEMBLK*, _UDWORD);
    void     MergeBlock(_MEMBLK*);
    void     LookForMergeNext(_MEMBLK*);
    void     LookForMergePrev(_MEMBLK*);
    void    *Malloc(_UDWORD);
    void     Free(void*);
    void    *Realloc(void*, _UDWORD);

  public:
    void     Free(void);
    void     Init(void);
    _BOOL    IsUsed(void*);
    _BOOL    MyMemory(void**, _UDWORD);
};

_UDWORD _MyHeap::MakeSize(_UDWORD size)
{
  return((size + 3) & 0xFFFFFFFC);
}


_MEMBLK* _MyHeap::MakePb(void *ptr)
{
  _MEMBLK *hp = (_MEMBLK*)((_UBYTE*)Data + DataSize);

  if(ptr < Data)
    return(NULL);
  if(ptr >= hp)
    return(NULL);

  return((_MEMBLK*)(((_UBYTE*)ptr) - sizeof(_MEMBLK)));
}

_MEMBLK* _MyHeap::ScanForFreeBlock(_UDWORD size)
{
  _MEMBLK *retcode = Data;

  while(retcode != NULL)
  {
    if((retcode->state == MEMFREE)&&(retcode->size >= size))
      return(retcode);
    retcode = retcode->p_next;
  }

  return(NULL);
}

void _MyHeap::SplitBlock(_MEMBLK *pb, _UDWORD size)
{
  _MEMBLK *pn;

  if(pb->size > (size + sizeof(_MEMBLK) + 4))
  {
    pn         = (_MEMBLK*)(((_UBYTE*)pb) + sizeof(_MEMBLK) + size);

    pn->size   = pb->size - sizeof(_MEMBLK) - size;
    pn->p_next = pb->p_next;
    pn->p_prev = pb;
    pn->state  = MEMFREE;

    if(pn->p_next != NULL)
      pn->p_next->p_prev = pn;

    pb->size   = size;
    pb->p_next = pn;
  }
}

void _MyHeap::MergeBlock(_MEMBLK *pb)
{
  pb->size  += pb->p_next->size + sizeof(_MEMBLK);
  pb->p_next = pb->p_next->p_next;
  if(pb->p_next != NULL)
    pb->p_next->p_prev = pb;
}

void _MyHeap::LookForMergeNext(_MEMBLK *pb)
{
  _MEMBLK *ph;

  if(pb->p_next != NULL)
  {
    if(pb->p_next->state == MEMFREE)
      MergeBlock(pb);
  }
}

void _MyHeap::LookForMergePrev(_MEMBLK *pb)
{
  _MEMBLK *ph;

  if(pb->p_prev != NULL)
  {
    if(pb->p_prev->state == MEMFREE)
      MergeBlock(pb->p_prev);
  }
}

void* _MyHeap::Malloc(_UDWORD size)
{
  _MEMBLK *pb;

  size = MakeSize(size);
  if(size > 0)
  {
    pb = ScanForFreeBlock(size);
    if(pb != NULL)
    {
      SplitBlock(pb, size);
      pb->state = MEMUSED;
      return((void*)((_UBYTE*)pb + sizeof(_MEMBLK)));
    }
  }
  return(NULL);
}

void _MyHeap::Free(void *ptr)
{
  _MEMBLK *pb = MakePb(ptr);

  if(pb != NULL)
  {
    if(pb->state == MEMUSED)
    {
      pb->state = MEMFREE;
      LookForMergeNext(pb);
      LookForMergePrev(pb);
    }
  }
}

void *_MyHeap::Realloc(void *ptr, _UDWORD size)
{
  _MEMBLK *pb = MakePb(ptr);
  void    *ph;
  _UDWORD sz;

  if(pb != NULL)
  {
    size = MakeSize(size);
    if(pb->state == MEMUSED)
    {
      if(size == pb->size)     // speicher bleibt gleich
      {
        return(ptr);
      }
      else if(size < pb->size) // speicher verkleinern
      {
        SplitBlock(pb, size);
        return(ptr);
      }
      else // speicher vergrössern
      {
        if(pb->p_next != NULL)
        {
          if((pb->p_next->state == MEMFREE)&&(size <= (pb->size + sizeof(_MEMBLK) + pb->p_next->size)))
          {
            // nächster block wird adaptiert
            MergeBlock(pb);
            SplitBlock(pb, size);
            return(ptr);
          }
        }

        if(pb->p_prev != NULL)
        {
          if((pb->p_prev->state == MEMFREE)&&(size <= (pb->size + sizeof(_MEMBLK) + pb->p_prev->size)))
          {
            // vorgänger block wird adaptiert
            sz = pb->size;
            pb = pb->p_prev;
            MergeBlock(pb);
            pb->state = MEMUSED;
            ph = (void*)(((_UBYTE*)pb) + sizeof(_MEMBLK));
            MemMove(ph, ptr, sz);
            SplitBlock(pb, size);
            return(ph);
          }
        }

        // neuer block wird erstellt
        ph = Malloc(size);
        if(ph != NULL)
        {
          MemCpy(ph, ptr, pb->size);
          Free(ptr);
          return(ph);
        }
      }
    }
  }

  return(NULL);
}

void _MyHeap::Free(void)
{
  Memory((void**)&Data, 0);
  Data     = NULL;
  DataSize = 0;
}

void _MyHeap::Init(void)
{
  #define MYMEMSIZE 40000000L
  Data     = NULL;
  DataSize = 0;
  if(Memory((void**)&Data, MYMEMSIZE) != 0)
  {
    DataSize     = MYMEMSIZE;
    Data->size   = MYMEMSIZE - sizeof(_MEMBLK);
    Data->p_next = NULL;
    Data->p_prev = NULL;
    Data->state  = MEMFREE;
  }
}

_BOOL _MyHeap::IsUsed(void *ptr)
{
  _UBYTE *hp;

  if(Data != NULL)
  {
    if(ptr == NULL)
      return(_FALSE);

    hp = ((_UBYTE*)Data + DataSize);
    if((ptr > Data)&&(ptr < hp))
      return(_TRUE);
  }

  return(_FALSE);
}

_BOOL _MyHeap::MyMemory(void **pptr, _UDWORD size)
{
  void *ptr;

  if(pptr != NULL)
  {
    ptr = *pptr;

    if(size > 0)
    {
      if(ptr == NULL)
        *pptr = Malloc(size);
      else
        *pptr = Realloc(ptr, size);

      if(*pptr != NULL)
        return(_TRUE);
      return(_FALSE);
    }
    else if(ptr != NULL)
    {
      Free(ptr);
      return(_TRUE);
    }
  }

  if(size == 0)
    return(_TRUE);

  return(_FALSE);
}
*/




