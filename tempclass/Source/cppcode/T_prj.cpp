#include "T_IPC.H"
#include "T_NUC.H"

void _Project::Init(void)
{
  init_PROJECT(&Project);
  LseProjectCrc = 0;
}

void _Project::Free(void)
{
  free_PROJECT(&Project);
}

_BOOL _Project::Read(_File *File)
{
  _UWORD  i, tmp;
  _BOOL   retcode = _FALSE;
  _UDWORD size;
  _UNI    tmpname[128];

  Init();

  if(File->GetError() == _FALSE)
  {
    File->Read(&tmp, sizeof(_UWORD));
    size = tmp * sizeof(_PRJ_ENTRY);

    if(File->FileMemory((void**)&Project.ptr, size) == _TRUE)
    {
      Project.no        = tmp;
      Project.no_static = 15;

      if(File->Read(&tmp, sizeof(_UWORD)) == _TRUE)
        Project.no_static = tmp;

      for(i=0; i<Project.no; i++)
        retcode = File->Read_PRJ_ENTRY(&Project.ptr[i]);

      Language.Read_LANGUAGE_INFO(File);
      
     #if COMPILER_VERSION >= 58
      // revisionsnumber
      File->Read(&Project.RevisionNumber, 4);
      
      // projectname
      if(File->Read(&i, 2) == _TRUE)
      {
        if(i < 256)
        {
          if(File->Read(tmpname, i) == _TRUE)
          {
            tmpname[i/2] = 0;
            StrCpy(Project.ProjectName, sizeof(_CHAR), tmpname, sizeof(_UNI));
          }
        }
      }
      
     #endif
    }
  }

  if(File->GetError() == _TRUE)
  {
    Free();
    retcode = _FALSE;
  }

  return(retcode);
}

_PROJECT *_Project::Get_PROJECT(void)
{
  return &Project;
}

_BOOL _Project::GetPictureAccess2(_ASCII*dpne, _UWORD *pno, _BOOL system, _PRJ_ENTRY **ppe, _BOOL sa51941)
{
  _BOOL  retcode = _TRUE;
  _UWORD nox = *pno;

  if(*pno < 60000)
  {
    InitUsedIntern(0xFFFE);
    retcode = GetPictureAccessSub(dpne, pno, system, ppe, sa51941);
    
    if(retcode == _FALSE)
    {
      // wenn kein access und kein projectentry vorhanden 
      // --> nachsehen ob eine entsprechende instanz der klasse _Screen vorhanden ist (zB.KeyPads)
      if(GetEntryByNo(nox, _PE_PICTURE) == NULL)
      {
        _WHOAMI twai;
        setup_WHOAMI(&twai, IMA_PICTURE, nox);        
        retcode = _Screen_GetAccess(&twai);
      }
    }
  }

  return(retcode);
}

_BOOL _Project::GetPictureAccess(_ASCII*dpne, _UWORD *pno, _BOOL system, _PRJ_ENTRY **ppe)
{
  return GetPictureAccess2(dpne, pno, system, ppe, _FALSE);
}

_BOOL _Project::GetPictureAccessSub(_ASCII*dpne, _UWORD *pno, _BOOL system, _PRJ_ENTRY **ppe, _BOOL sa51914)
{
  _BOOL      retcode = _FALSE;
  _UWORD     i;
  _PRJ_ENTRY *p;
  _UWORD     no = *pno;

  p = &Project.ptr[Project.no_static];
  for(i=Project.no_static; i<Project.no; i++)
  {
    if((p->no == no) && (p->ftype == _PE_PICTURE) && ((p->usedintern & 0x01) == 0))
    {
      p->usedintern |= 0x01;
      retcode = _TRUE;
      if(sa51914 == _FALSE)
      {
        retcode = Access_PRJ_ENTRY(p, system);
      }
//         retcode       = Access_PRJ_ENTRY(p, system);
//      if((chk_CHKBIT(&p->chkbit) == _TRUE)||(SuperMode == _TRUE))
//        retcode = chk_ACCESS(p->access);

      if(retcode == _FALSE)
      {
        if((p->nextno != 0xFFFF)&&(p->nextno != no))
        {
          *pno = p->nextno;
          retcode = GetPictureAccessSub(dpne, pno, system, ppe, sa51914);
        }
      }
      else if(dpne != NULL)
      {
        if(ppe != NULL)
          *ppe = p;
        StrCpy(dpne, sizeof(_ASCII), Project.prjdp, sizeof(_ASCII));
        StrCat(dpne, sizeof(_ASCII), p->name, sizeof(_ASCII));
      }

      break;
    }

    p++;
  }

  if(retcode == _FALSE)
    *pno = no;

  return(retcode);
}

_BOOL _Project::Access_PRJ_ENTRY(_PRJ_ENTRY *p, _BOOL system)
{
  _BOOL retcode = _TRUE;

  if(SuperMode == _FALSE)
  {
    retcode = _FALSE;
    if(chk_CHKBIT(&p->chkbit) == _TRUE)
      retcode = chk_ACCESS(p->access, system);
  }

  return(retcode);
}

void _Project::InitUsedIntern(_UWORD mask)
{
  _UWORD      i;
  _PRJ_ENTRY *p;

  p = Project.ptr;
  for(i=0; i<Project.no; i++,p++)
    p->usedintern &= mask;
}

_BOOL _Project::GetWindowAccess(_ASCII *dpne, _UWORD *pno, _BOOL system, _PRJ_ENTRY **ppe)
{
  _BOOL retcode = _TRUE;

  if(*pno < 60000)
  {
    _UWORD nox = *pno;
    InitUsedIntern(0xFFFE);
    retcode = GetWindowAccessSub(dpne, pno, system, ppe);

    if(retcode == _FALSE)
    {
      // wenn kein access und kein projectentry vorhanden 
      // --> nachsehen ob eine entsprechende instanz der klasse _Screen vorhanden ist (zB.KeyPads)
      if(GetEntryByNo(nox, _PE_WINDOW) == NULL)
      {
        _WHOAMI twai;
        setup_WHOAMI(&twai, IMA_WINDOW, nox);        
        retcode = _Screen_GetAccess(&twai);
      }
    }
  }

  if(retcode == _FALSE)
  {
    switch(*pno)
    {
      case UC_NEWBUBBLEWINDOW :
      case UC_CONFIRMWINDOW :
      case UC_OFFLINEWINDOW :
      case UC_RESETWINDOW   :  retcode = _TRUE; break;
    }
  }

  return(retcode);
}

_BOOL _Project::GetWindowAccessSub(_ASCII *dpne, _UWORD *pno, _BOOL system, _PRJ_ENTRY **ppe)
{
  _BOOL      retcode = _FALSE;
  _UWORD     i;
  _PRJ_ENTRY *p;
  _UWORD     no = *pno;

  p = &Project.ptr[Project.no_static];
  for(i=Project.no_static; i<Project.no; i++)
  {
    if((p->no == no) && (p->ftype == _PE_WINDOW) && ((p->usedintern & 0x01) == 0))
    {
      p->usedintern |= 0x01;
      retcode = Access_PRJ_ENTRY(p, system);
//      if(chk_CHKBIT(&p->chkbit) == _TRUE)
//        retcode = chk_ACCESS(p->access);

      if(retcode == _FALSE)
      {
        if((p->nextno != 0xFFFF)&&(p->nextno != no))
        {
          *pno = p->nextno;
          retcode = GetWindowAccessSub(dpne, pno, system, ppe);
        }
      }
      else if(dpne != NULL)
      {
        if(ppe != NULL)
          *ppe = p;
        StrCpy(dpne, sizeof(_ASCII), Project.prjdp, sizeof(_ASCII));
        StrCat(dpne, sizeof(_ASCII), p->name, sizeof(_ASCII));
      }
      break;
    }
    p++;
  }

  if(retcode == _FALSE)
    *pno = no;

  return(retcode);
}

_WORD sort_PRJ_ENTRY(_PRJ_ENTRY *p1, _PRJ_ENTRY *p2)
{
  _ENTRYTYPE t1, t2;

  t1 = p1->ftype;
  t2 = p2->ftype;

  if(t1 == _PE_WINDOW)
  {
    if(t2 != _PE_WINDOW)
      return(1);
    return((p2->no < p1->no)? 1 : -1);
  }

  if(t1 == _PE_PICTURE)
  {
    if(t2 == _PE_PICTURE)
      return((p2->no < p1->no)? 1 : -1);
    if(t2 != _PE_WINDOW)
      return(1);
  }

  return(-1);
}

#ifdef XUSER_KM
_BOOL _Project::JustLoad(_ASCII *dpne)
{
  _BOOL  retcode;
  _File  File;
  _UWORD pos;

  File.Open(LOAD, dpne);
  if(File.ReadHeader() == _TRUE)
  {
    if(Read(&File) == _TRUE)
    {
      pos = offset_dpne(dpne, 1);
      MemCpy(Project.prjdp, dpne, pos);
      Project.prjdp[pos] = 0;
      retcode = _TRUE;
    }
  }

  return(retcode);
}
#endif


#ifdef NINETEEN
_BOOL load_dashboard(void)
{
  _BOOL      retcode = _FALSE;
  _Softkey   Softkey;
  _PRJ_ENTRY *pre;
  _ASCII     dpne[_MAX_PATHLENGTH];
  _ROOM      room;

  DashBoard.Free();

  pre = Project.GetEntryByNo(NINETEEN, _PE_PICTURE);
  if(pre != NULL)
  {
    StrCpy(dpne, sizeof(_ASCII), Project.GetPrjPath(), sizeof(_ASCII));
    StrCat(dpne, sizeof(_ASCII), pre->name, sizeof(_ASCII));

    Softkey.Init();

    if(DashBoard.Load(dpne, &Softkey, _TRUE, _FALSE, NINETEEN) == _TRUE)
    {
      MyGetScreenSize(&room);
      move_MY_PICTURE(DashBoard.GetMyPicture(), 0, room.xy2.y + 1, T_LEFTBOUND|T_UPBOUND);
      retcode = _TRUE;
    }

    Softkey.Free();
    if(retcode == _FALSE)
      DashBoard.Free();
  }

  return(retcode);
}
#endif

_UDWORD _Project::GetLseProjectCrc(void)
{
  return LseProjectCrc;
}

_UDWORD _Project::GetLseProjectCrcSub(void)
{
  _ASCII dpne[_MAX_PATHLENGTH];
  _File  File;
  _DWORD size;
  _UDWORD retcode = 0;
  void *data = NULL;
  
  StrCpy(dpne, sizeof(_ASCII), Project.prjdp, sizeof(_ASCII));
  StrCat(dpne, sizeof(_ASCII), "LSEMPC.CRC", sizeof(_ASCII));
  
  File.Open(LOAD, dpne, 0, _TRUE);
  size = File.Length();
  
  if(size > 0)
  {
    if(Memory2((void**)&data, size) != 0)
    {
      if(File.Read(data, size) == _TRUE)
      {
        retcode = MakeCrc32(0, data, size);
      }
      Memory2((void**)&data, 0);
    }
  }
  
  File.Close();
  
  return(retcode);
}


_BOOL _Project::Load(_ASCII *dpne)
{
  _File  File;
  _BOOL  retcode = _FALSE;
  _UWORD i, pos, sbi;
  _UWORD bs;
  _ASCII name[_MAX_PATHLENGTH];
  _BOOL  fst, error = _FALSE;

  Start();
  File.Open(LOAD, MakeRamFile(name, dpne));

  if(File.ReadHeader() == _TRUE)
  {
    SetLseServer(LSE_LSEVERSION, File.GetLseVersion());
    SetLseServer(LSE_COMPILERVERSION, File.GetMpcVersion());

    if(RTOSVERSION >= 0x050B)
      ConfigGraphicSystem((File.GetMpcVersion() >= 20)? GRC_SET_PIXELCORRECTION : GRC_RESET_PIXELCORRECTION, 0, NULL);

    retcode = Read(&File);
  }
  else
  {
    if(File.GetErrorState() == H_WRONGLSE)
    {
      FatalError("ERROR: WRONG LSE-COMPILER-VERSION ...");
      i = File.GetLseVersion();
      if(i != 617)
      {
        StrCpy(name, sizeof(_ASCII), "  PROJECT DONE WITH LSE: ", sizeof(_ASCII));
        make_version_text(&name[StrLen(name, sizeof(_ASCII))], i, 0x0800);
        FatalError(name);
      }

      StrCpy(name, sizeof(_ASCII), "  PROJECT COMPILER-VERSION: ", sizeof(_ASCII));
      DToA(&name[StrLen(name, sizeof(_ASCII))], File.GetMpcVersion(), 0x0800, sizeof(_ASCII));
      FatalError(name);
      StrCpy(name, sizeof(_ASCII), "  NEEDED COMPILER-VERSION: ", sizeof(_ASCII));
      DToA(&name[StrLen(name, sizeof(_ASCII))], File.GetCompilerVersion(), 0x0800, sizeof(_ASCII));
      FatalError(name);
      error = _TRUE;
    }
  }
  File.Close();


  if(retcode == _TRUE)
  {
    SetLseServer(LSE_LSEREVISION, Project.RevisionNumber);
  
    qsort(Project.ptr, Project.no, sizeof(_PRJ_ENTRY), (_QSORTFUNCT)sort_PRJ_ENTRY);

    bs  = ScanBar.Add("Load Project:", Project.no_static + 3); // +3 wegen 3-geteilter Varlist
    Project.crc32dpne = MakeCrc32(0, dpne, StrLen(dpne, sizeof(_ASCII)));
    pos = offset_dpne(dpne, 1);
    MemCpy(Project.prjdp, dpne, pos);
    Project.prjdp[pos] = 0;

    name[0] = 'R';
    name[1] = 'A';
    name[2] = 'M';
    name[3] = '@';
    StrCpy(&name[4], sizeof(_ASCII), Project.prjdp, sizeof(_ASCII));
    pos += 4;


    Alarm.Free();
    VarList.Free();
    TextList.Free(_TRUE);
    ObjectList.Free();
    ImageList.Free();
    InterBoxList.Free();
    MenuList.Free();
    ColorScheme.Free();
    UnitScheme.Free();
    ServerScheme.Free();
    FontScheme.Free();
    ScreenScheme.Free();
    ImageScheme.Free();
    StateScheme.Free();
    TextScheme.Free();
    Font.Free();
    Sequencer.Free();
    SetUp.Free();
    IpcSettings.Free();
    Hotkey.Free();

    IpcSettings.Load();                                   // read settings

   #ifdef UC_NEWCOLORPALETTE
    _PRJ_ENTRY *pec = GetEntryByNo(0xFFFF, _PE_PALETTE);
    if(pec != NULL)
    {
      name[pos] = 0;
      StrCat(name, sizeof(_ASCII), pec->name, sizeof(_ASCII));
      Palette.Load(name); // Palette als erstes laden weil ggf.farben beim laden nachfolgender files ausgetauscht werden
    }
   #endif
   
    _DWORD bs_step = 0;
    for(i=0, sbi=0; i<Project.no_static; i++, sbi++, bs_step++)
    {
      ScanBar.Run(bs, bs_step);

      if(i < Project.no)
      {
        name[pos] = 0;
        StrCat(name, sizeof(_ASCII), Project.ptr[i].name, sizeof(_ASCII));

        fst = _TRUE;

        switch(Project.ptr[i].ftype)
        {
          case _PE_ALARMLIST    : fst = Alarm.Load(name);
                                  break;
          case _PE_VARLIST      : fst = VarList.Load(name, bs, &sbi);
                                 #ifdef UC_INVISIBLE_PROGRESS
                                  bs_step = ScanBar.Get(bs);
                                 #endif 
                                  break;
          case _PE_LANGUAGE     : fst = Language.LoadDirectory(name);
                                  break;
          case _PE_TEXTLIST     : fst = TextList.Load(name, Project.ptr[i].no, get_actual_language());
                                  break;
          case _PE_OBJECTLIST   : fst = ObjectList.Load(name);
                                  break;
          case _PE_BMPLIST      : fst = ImageList.Load(name);
                                  break;
          case _PE_FUNCTIONLIST : fst = InterBoxList.Load(name);
                                  break;
          case _PE_MENULIST     : fst = MenuList.Load(name);
                                  break;
          case _PE_COLORSCHEME  : fst = ColorScheme.Load(name, _TRUE, _FALSE);
                                 #ifdef UC_GRAYSCALE
                                  ColorScheme.TransformToGray();
                                 #endif
                                  break;
          case _PE_UNITSCHEME   : fst = UnitScheme.Load(name, _TRUE, _FALSE);
                                  break;
          case _PE_FONTSCHEME   :
                                 #if COMPILER_VERSION < 36
                                  fst = FontScheme.Load(name, _FALSE, _FALSE);
                                  #if COMPILER_VERSION < 35
                                   FontScheme.Change(SOP_SMALLEQUAL, SOP_EQUAL);
                                  #endif
                                 #endif
                                  break;
          case _PE_SCRNSCHEME   : fst = ScreenScheme.Load(name, _TRUE, _FALSE);
                                  break;
          case _PE_IMAGESCHEME  : fst = ImageScheme.Load(name, _FALSE, _FALSE);
                                  break;
          case _PE_STATESCHEME  : fst = StateScheme.Load(name, _TRUE, _FALSE);
                                  break;
          case _PE_TEXTSCHEME   : fst = TextScheme.Load(name);
                                  break;
          case _PE_OBJECTSCHEME : fst = ObjectScheme.Load(name, _FALSE, _FALSE);
                                  break;
          case _PE_FONTLIST     : fst = Font.Load(ClearRamFile(name));
                                  break;
          case _PE_SEQUENCELIST : fst = Sequencer.Load(name);
                                  break;
          case _PE_SETUP        : fst = SetUp.Load(name);
                                  break;
         #ifndef UC_NEWCOLORPALETTE
          case _PE_PALETTE      : fst = Palette.Load(name);
                                  break;
         #endif
          case _PE_HOTKEYLIST   : fst = Hotkey.Load(name);
                                  break;
          case _PE_SERVERSCHEME : fst = ServerScheme.Load(name, _TRUE, _TRUE);
                                  ServerScheme.SetVariableResult(); // ergebnis ist eine variablennummer (overload benötigt dies)
                                  break;
         #ifdef UC_THEME
          case _PE_THEME        : fst = Theme.Load(name);
                                  break;
         #endif
          default               : break;
        }

        if(fst == _FALSE)
        {
          ScanBar.Assert("ERROR: CAN'T LOAD PROJECTENTRY...");
//          ScanBar.End(ScanBar.Add("ERROR: CAN'T LOAD PROJECTENTRY...", 0));
          if(GetTooLessMemory() == _TRUE)
          {
            ScanBar.Assert("ERROR: TOO LESS MEMORY...");
//            ScanBar.End(ScanBar.Add("ERROR: TOO LESS MEMORY...", 0));
          }
          return(_FALSE); // can't load projectentry
        }
      }
    }

   #if COMPILER_VERSION >= 36
    Language.MakeLanguageInfo();            // Sprachinformation erstellen
    Language.LoadLanguage(get_actual_language());      // Textlisten laden
   #endif
    Language.SetTextRenderEngine(get_actual_language());

   #ifdef NINETEEN
    load_dashboard();
   #endif

    ScanBar.End(bs);

    TermAlarm.Load();                              // read terminal alarms
  }

  LseProjectCrc = GetLseProjectCrcSub();

  if((retcode == _FALSE)&&(error == _FALSE))
  {
//    ScanBar.End(ScanBar.Add("ERROR: UNKNOWN PROJECT-FILE ...", 0));
    ScanBar.Assert("ERROR: UNKNOWN PROJECT-FILE ...");
  }

  return(retcode);
}


void _Project::ReloadText(void)
{
  _UWORD pos, i;
  _ASCII name[_MAX_PATHLENGTH];

  pos = StrLen(Project.prjdp, sizeof(_ASCII));
  StrCpy(name, sizeof(_ASCII), Project.prjdp, sizeof(_ASCII));

  TextList.Free(_FALSE);
 #ifndef USER_KM
 #ifndef USER_DEMAG
 #ifndef USER_BILLION
 #ifndef USER_BOY
 #ifndef USER_HIRSCH // sa38992
  _BOOL loctext = SetUp.GetPossiblyLocalTextInUse(); // sa38992
  if(loctext == _TRUE) // sa38992
  {
    ObjectList.Free();
  }
 #endif
 #endif
 #endif
 #endif
 #endif
 #if COMPILER_VERSION < 22
  MenuList.Free();
 #endif

  for(i=0; i<Project.no_static; i++)
  {
    if(i < Project.no)
    {
      name[pos] = 0;
      StrCat(name, sizeof(_ASCII), Project.ptr[i].name, sizeof(_ASCII));
      switch(Project.ptr[i].ftype)
      {
        case _PE_TEXTLIST   : TextList.Load(name, Project.ptr[i].no, get_actual_language());
                              break;
       #ifndef USER_KM
       #ifndef USER_DEMAG
       #ifndef USER_BILLION
       #ifndef USER_BOY
       #ifndef USER_HIRSCH // sa38992
        case _PE_OBJECTLIST : if(loctext == _TRUE) // sa38992
                              {
                                ObjectList.Load(name);
                              }
                              break;
       #endif
       #endif
       #endif
       #endif
       #endif
       
       #if COMPILER_VERSION < 22
        case _PE_MENULIST   : MenuList.Load(name);
                              break;
       #endif
        default             : break;
      }
    }
  }

 #if COMPILER_VERSION >= 36
  Language.LoadLanguage(get_actual_language());
 #endif
}

_PRJ_ENTRY *_Project::GetEntry(_UWORD i)
{
  if(i < Project.no)
    return(&Project.ptr[i]);

  return(NULL);
}

_BOOL _Project::GetNameFromFile(_ASCII *dst, _ASCII *ne)
{
  _BOOL  retcode;
  _File  File;
  _ASCII dpne[_MAX_PATHLENGTH]; 
  
  StrCpy(dpne, sizeof(_ASCII), Project.prjdp, sizeof(_ASCII));
  StrCat(dpne, sizeof(_ASCII), ne, sizeof(_ASCII));
  
  File.Open(LOAD, dpne);
  retcode = File.ReadHeaderName(dst);
  File.Close();
  
  return retcode;
}

_ASCII *_Project::GetNameOfEntry(_PRJ_ENTRY *pe)
{
  if((pe->ftype == _PE_WINDOW) || (pe->ftype == _PE_PICTURE))
  {
    if(pe->label[0] == 0)
      GetNameFromFile(pe->label, pe->name);
    
    if(pe->label[0] != 0)
      return pe->label;
  }
  
  return NULL;
}

_PRJ_ENTRY *_Project::GetEntryByLabel(_ASCII *label, _ENTRYTYPE ftype)
{
  _UWORD     i;
  _PRJ_ENTRY *p;
  _ASCII     *pnam;

  if(Project.no > 0)
  {
    if(ftype == _PE_WINDOW)
    {
      for(i=0, p=&Project.ptr[Project.no - 1]; i<Project.no; i++, p--)
      {
        if(p->ftype == _PE_WINDOW)
        {
          pnam = GetNameOfEntry(p);
          if(pnam != NULL)
          {
            if(StrXmp(pnam, label, sizeof(_ASCII)) == 0)
            {  
              return(p);
            }
          }
        }
      }
    }
    else if(ftype == _PE_PICTURE)
    {
      for(i=0, p=Project.ptr; i<Project.no; i++, p++)
      {
        if(p->ftype == _PE_PICTURE)
        {
          pnam = GetNameOfEntry(p);
          if(pnam != NULL)
          {
            if(StrXmp(pnam, label, sizeof(_ASCII)) == 0)
            {  
              return(p);
            }
          }
        }
      }
    }
  }

  return(NULL);
}

_PRJ_ENTRY *_Project::GetEntryByNo(_UWORD no, _ENTRYTYPE ftype)
{
  _UWORD     i;
  _PRJ_ENTRY *p;

  if(Project.no > 0)
  {
    if(ftype == _PE_WINDOW)
    {
      for(i=0, p=&Project.ptr[Project.no - 1]; i<Project.no; i++, p--)
      {
        if((p->no == no) && (p->ftype == _PE_WINDOW))
          return(p);
      }
    }
    else if(ftype == _PE_PALETTE)
    {
      for(i=0, p=Project.ptr; i<Project.no; i++, p++)
      {
        if(p->ftype == ftype)
          return(p);
      }
    }
    else
    {
      for(i=0, p=Project.ptr; i<Project.no; i++, p++)
      {
        if((p->no == no) && (p->ftype == ftype))
          return(p);
      }
    }
  }

  return(NULL);
}

_UWORD _Project::GetNo(void)
{
  return(Project.no);
}

_UWORD _Project::GetNoStatic(void)
{
  return(Project.no_static);
}

_UWORD _Project::GetLanguageNo(void)
{
  return(Language.GetNo());
}

_UDWORD _Project::GetCrc32(void)
{
  return(Project.crc32dpne);
}

_UWORD _Project::GetNextPictureNo(_UWORD actual)
{
  _UWORD      i;
  _PRJ_ENTRY *pe;
  _UWORD     retcode = 0xFFFF;

  for(i=0; i<Project.no; i++)
  {
    pe = &Project.ptr[i];

    if(pe->ftype == _PE_PICTURE)
    {
      if((pe->no > actual)&&(pe->no < retcode))
        retcode = pe->no;
    }
  }

  if(retcode == 0xFFFF)
  {
    for(i=0; i<Project.no; i++)
    {
      pe = &Project.ptr[i];

      if(pe->ftype == _PE_PICTURE)
      {
        if(pe->no < retcode)
          retcode = pe->no;
      }
    }
  }

  return(retcode);
}

_BOOL _Project::MakeConfigDPNE(_ASCII *dp, const _ASCII *ne, _BOOL ramfile)
{
  _DDE_INFO dde;
  _BOOL     retcode = _FALSE;
  _DWORD    handle;

  if(Project.prjdp[0] != 0)
  {
    retcode = _TRUE;

    StrCpy(dp, sizeof(_ASCII), Project.prjdp, sizeof(_ASCII));
    handle = FindFirst(dp, (char*)"config", &dde, _DDE_DIR, 0);
    StrCat(dp, sizeof(_ASCII), "config", sizeof(_ASCII));

    if(handle < 0)
    {
      retcode = _FALSE;
      if(CreateDir(dp) >= 0)
        retcode = _TRUE;
    }

    StrCat(dp, sizeof(_ASCII), "\\", sizeof(_ASCII));
    FindClose(handle);

    if(ne != NULL)
      StrCat(dp, sizeof(_ASCII), ne, sizeof(_ASCII));

    if((retcode == _TRUE)&&(ramfile == _TRUE))
    {
      MemMove(&dp[4], dp, StrLen(dp, sizeof(_ASCII)) + 1);
      MemCpy(dp, "RAM@", 4);
    }
  }

  return(retcode);
}

_ASCII* _Project::GetPrjPath(void)
{
  return(Project.prjdp);
}

_BOOL _Project::GetDPNE(_ASCII *dpne, _ENTRYTYPE ftype)
{
  _PRJ_ENTRY *p;
  _UDWORD    i;

  p = Project.ptr;
  for(i=0; i<Project.no; i++, p++)
  {
    if(p->ftype == ftype)
    {
      StrCpy(dpne, sizeof(_ASCII), GetPrjPath(), sizeof(_ASCII));
      StrCat(dpne, sizeof(_ASCII), p->name, sizeof(_ASCII));
      return(_TRUE);
    }
  }

  return(_FALSE);
}

_BOOL _Project::SetTime(_UWORD no, _ENTRYTYPE type, _UDWORD time)
{
  _PRJ_ENTRY *pe = GetEntryByNo(no, type);
  if(pe != NULL)
  {
    pe->time_last = time;
    if(pe->time_last > pe->time_max)
      pe->time_max = pe->time_last;
    return(_TRUE);
  }

  return(_FALSE);
}


_ASCII_BUFFER *_Project::GetLanguages(void)
{
  _LANGUAGE_INFO *pli;
  pli = Language.Get_LANGUAGE_INFO();
  return(&pli->winname);
}

_LANGUAGE_INFO *_Project::Get_LANGUAGE_INFO(void)
{
  return(Language.Get_LANGUAGE_INFO());
}

void _TextList::Init(void)
{
  MemTextPufferIndex = 0;
  
  _UDWORD nox = ARRAY_SIZE(MemTextPuffer);
  while(nox--)
  {
    init_MEM_TextPuffer(&MemTextPuffer[nox]);
    HostLanguageId[nox] = 0xFFFFFFFF;  
  }
  
  MemSet(HostStack, 0, sizeof(HostStack));
  HostStackNo = 0;
  HostStackError = 0;
  
  Unicode = _FALSE;
}

void _TextList::Free(_BOOL free_hostlanguage_as_well)
{
  free_MEM_TextPuffer(&MemTextPuffer[0]);
  init_MEM_TextPuffer(&MemTextPuffer[0]);
  
  if(free_hostlanguage_as_well == _TRUE) // wunsch appli: 10.05.2017
  {
    for(_UDWORD i=1; i<ARRAY_SIZE(MemTextPuffer); i++)
    {
      free_MEM_TextPuffer(&MemTextPuffer[i]);
      init_MEM_TextPuffer(&MemTextPuffer[i]);
      HostLanguageId[i] = 0xFFFFFFFF;  
    }
    MemTextPufferIndex = 0;
    MemSet(HostStack, 0, sizeof(HostStack));
    HostStackNo = 0;
    HostStackError = 0;
  }
}

_MEM_TextPuffer* _TextList::GetPtm(void)
{
  if(MemTextPufferIndex < ARRAY_SIZE(MemTextPuffer))
  {
    return(&MemTextPuffer[MemTextPufferIndex]);
  }
  
  return(&MemTextPuffer[0]);
}

_BOOL _TextList::SwapLanguage(_BOOL state, _UWORD language)
{
  _BOOL retcode = _FALSE;

  if(state == _TRUE)
  {
    if(HostLanguageSet(1, language) == _TRUE)
    {
      MemTextPufferIndex = 1;
      retcode = _TRUE;
    }
  }
  else
  {
    MemTextPufferIndex = 0;
    retcode = _TRUE;
  }
  
  return retcode;
}

_UDWORD _TextList::HostLanguageGet(void)
{
  return HostLanguageGetEx(MemTextPufferIndex);
}

_UDWORD _TextList::HostLanguageGetEx(_UDWORD idx)
{
  if(idx < ARRAY_SIZE(MemTextPuffer))
  {
    return Language.GetIDByIndex(HostLanguageId[idx]);
  }
  return Language.GetActualID();
}

_BOOL _TextList::HostLanguageSet(_UDWORD idx, _UDWORD language)
{
  _BOOL retcode = _FALSE;
  
  if(idx < ARRAY_SIZE(MemTextPuffer))
  {
    _UDWORD anz = Project.GetLanguageNo(); // anzahl der sprachen ermitteln
    
    if(language >= anz)
    {
      language = Language.GetIndexByID(language); // ggf ist dies eine language_id
    }

    if(language < anz)
    {
      if(language == HostLanguageId[idx])
      {
        retcode = _TRUE; // hostlanguage ist bereits gesetzt
      }
      else
      {
        HostLanguageId[idx] = 0xFFFFFFFF;
        free_MEM_TextPuffer(&MemTextPuffer[idx]);
        PushMemColor(MEMORY_PRG);
        _UDWORD memo = MemTextPufferIndex;
        MemTextPufferIndex = idx;
        if(Language.LoadLanguage(language) == _TRUE)
        {
          HostLanguageId[idx] = language;
          retcode = _TRUE;
        }
        MemTextPufferIndex = memo;
        PopMemColor();
      }
    }
  }
  
  return retcode;
}

_BOOL _TextList::HostLanguagePopEx(void)
{
  if(HostStackError > 0)
  {
    HostStackError--;
    return _FALSE;
  }
  
  if(HostStackNo > 0)
  {
    HostStackNo--;
    MemTextPufferIndex = HostStack[HostStackNo];
    return _TRUE;
  }

  MemTextPufferIndex = 0;

  return _FALSE;
}

_BOOL _TextList::HostLanguagePushEx(_UDWORD idx)
{
  _BOOL retcode = _TRUE;

  if(idx >= ARRAY_SIZE(MemTextPuffer))
  {
    retcode = _FALSE;
    idx = 0; // ungültiger index
  }

  if(HostLanguageId[idx] >= Project.GetLanguageNo()) // nur wenn hostlanguage mit index bereits gesetzt ist
  {
    retcode = _FALSE; // hostlanguage noch nicht gesetzt
    idx = 0;
  }
  
  if(HostStackNo < ARRAY_SIZE(HostStack))
  {
    HostStack[HostStackNo] = (_UBYTE)MemTextPufferIndex;
    HostStackNo++;
    MemTextPufferIndex = idx;
  }
  else
  {
    HostStackError++;
    retcode = _FALSE;
  }
  
  return retcode;
}

_BOOL _TextList::GetUnicode(void)
{

  return(Unicode);
}

_BOOL _TextList::Load(_ASCII *dpne, _UWORD no, _UWORD language)
{
  _UWORD          i;
  _BOOL           retcode = _FALSE;
  _MEM_TextPuffer *ptm = GetPtm();

  if(no >= ptm->no)
  {
    if(Memory2((void**)&ptm->ptr, sizeof(_TextPuffer) * (no + 1)) != 0)
    {
      for(i=ptm->no; i<=no; i++)
        ptm->ptr[i].Init();
      ptm->no = no + 1;
    }
  }

  if(no < ptm->no)
  {
    ptm->ptr[no].Free();
    retcode = ptm->ptr[no].Load(dpne, language);
  }

  return(retcode);
}

_CHAR *_TextList::Get(_UWORD listno, _UWORD txtno)
{
  _CHAR           *retcode = NULL;
  _MEM_TextPuffer *ptm = GetPtm();


  if(listno < ptm->no)
  {
    if(txtno > 0xFF00)
    {
      if(listno == 1) // es handelt sich um die systemtextliste
      {
        char *pa = NULL;
        switch(txtno)
        {
          case TxtID_OFFLINE_ENTER : 
            pa = (char*)"<ENTER>...restart";
            break;
            
          case TxtID_OFFLINE_ESC : 
            pa = (char*)"<ESC>...offlinemode";
            break;
            
          case TxtID_OFFLINE_TEXT : 
            return Batch.MakeOfflineText(get_temptext(), _FALSE);
        }
        if(pa != NULL)
        {
          retcode = get_temptext();
          StrCpy(retcode, sizeof(_CHAR), pa, 1);
          return retcode;
        }
      }
    }
  
    retcode = ptm->ptr[listno].GetText(txtno);
  }

  return(retcode);
}

_TextPuffer *_TextList::GetTextPuffer(_UWORD listno)
{
  _TextPuffer     *retcode = NULL;
  _MEM_TextPuffer *ptm = GetPtm();

  if(listno < ptm->no)
    retcode = &ptm->ptr[listno];

  return(retcode);
}

_UDWORD _TextList::GetSize(_UWORD listno)
{
  _UDWORD         retcode = 0;
  _MEM_TextPuffer *ptm = GetPtm();

  if(listno < ptm->no)
    retcode = ptm->ptr[listno].GetSize();

  return(retcode);
}

_UDWORD _TextList::GetNo(_UWORD listno)
{
  _UDWORD         retcode = 0;
  _MEM_TextPuffer *ptm = GetPtm();

  if(listno < ptm->no)
    retcode = ptm->ptr[listno].GetNo();

  return(retcode);
}

_CHAR *_TextList::Get(_ANYLST anylst, _UWORD txtno)
{
  _UWORD no = (_UWORD)anylst - (_UWORD)TXTLIST_VTX;
  return(Get(no, txtno));
}

_UDWORD _TextList::GetSize(_ANYLST anylst)
{
  _UWORD no = (_UWORD)anylst - (_UWORD)TXTLIST_VTX;
  return(GetSize(no));
}

_UDWORD _TextList::GetNo(_ANYLST anylst)
{
  _UWORD no = (_UWORD)anylst - (_UWORD)TXTLIST_VTX;
  return(GetNo(no));
}

_BOOL _TextList::LoadCompiler36(_ASCII *dpne)
{
  _BOOL           retcode = _FALSE;
  _File           File;
  _UWORD          tmp, i;
  _UDWORD         offset;
  _MEM_TextPuffer *ptm = GetPtm();


  // header
  // [4] ... absoluter offset auf sprachinformation
  // [2] ... anzahl textlisten
  // [ ..... textlisten
  // [ ..... sprachinformation

  free_MEM_TextPuffer(ptm);

  File.Open(LOAD, dpne);
  if(File.ReadHeader() == _TRUE)
  {
    File.Read(&offset, sizeof(_UDWORD));

    if(File.Read(&tmp, sizeof(_UWORD)) == _TRUE)
    {
      if(Memory2((void**)&ptm->ptr, sizeof(_TextPuffer) * tmp) != 0)
      {
        retcode = _TRUE;
        ptm->no = tmp;

        for(i=0; i<tmp; i++)
        {
          ptm->ptr[i].Init();
          if(retcode == _TRUE)
          {
            retcode = ptm->ptr[i].Read(&File, &Unicode);
          }
        }
      }
    }
  }

  File.Close();

  return(retcode);
}

_UWORD _TextList::GetNo(void)
{
  _MEM_TextPuffer *ptm = GetPtm();

  return(ptm->no);
}

_UDWORD memsize_MEM_TextPuffer(_MEM_TextPuffer *pmtp)
{
  _UDWORD retcode = 0;
  _UWORD  i;

  for(i=0; i<pmtp->no; i++)
    retcode += (sizeof(_TextPuffer) + pmtp->ptr[i].GetSize());

  return(retcode);
}

#ifdef UC_IPC
_UDWORD _TextList::MemSize(_MEMSIZE *pms)
{
  _LANGUAGE_INFO *pli;

  pms->label = 0;
  pms->data  = sizeof(_TextList);
  
  for(_UDWORD i=0; i<ARRAY_SIZE(MemTextPuffer); i++)
  {
    pms->data += memsize_MEM_TextPuffer(&MemTextPuffer[i]);
  }
  
  pli = Language.Get_LANGUAGE_INFO();

  if(pli != NULL)
  {
    pms->label = sizeof(_LANGUAGE_INFO)
                 + pli->winid.no * sizeof(_DWORD)
                 + pli->flag.no * sizeof(_DWORD)
                 + pli->name.size 
                 + pli->winname.size;
  }

  return(pms->label + pms->data);
}
#endif

//****************************************************************************
//  LASAL Interface
//****************************************************************************
//****************************************************************************
//****************************************************************************

extern "C" _UWORD Prj_GetNo(void)
{
  return(Project.GetNo());
}

extern "C" _PRJ_ENTRY *Prj_GetEntryByNo(_UWORD no, _ENTRYTYPE ftype)
{

  return(Project.GetEntryByNo(no, ftype));
}

extern "C" _PRJ_ENTRY *Prj_GetEntry(_UWORD no)
{
  return(Project.GetEntry(no));
}

extern "C" _UWORD Prj_GetLanguageNo(void)
{
  return(Project.GetLanguageNo());
}

extern "C" _ASCII_BUFFER *Prj_GetLanguages(void)
{
  return(Project.GetLanguages());
}

extern "C" _LANGUAGE_INFO *Prj_Get_LANGUAGE_INFO(void)
{
  return(Project.Get_LANGUAGE_INFO());
}

extern "C" _BOOL Prj_SwapLanguage(_BOOL state, _UWORD language_no)
{
  return(TextList.SwapLanguage(state, language_no));
}

extern "C" _BOOL Prj_HostLanguagePush(void)
{
  return TextList.HostLanguagePushEx(1);
}

extern "C" _BOOL Prj_HostLanguagePop(void)
{
  return TextList.HostLanguagePopEx();
}

extern "C" _BOOL Prj_HostLanguagePushEx(_UDWORD idx)
{
  return TextList.HostLanguagePushEx(idx);
}

extern "C" _BOOL Prj_HostLanguagePopEx(void)
{
  return TextList.HostLanguagePopEx();
}

extern "C" _BOOL Prj_HostLanguageSetEx(_UDWORD idx, _UDWORD language)
{
  if(idx > 0)
  {
    return TextList.HostLanguageSet(idx, language);
  }
  return _FALSE;
}

extern "C" _UDWORD Prj_HostLanguageGet(void)
{
  return TextList.HostLanguageGet();
}

extern "C" _UDWORD Prj_HostLanguageGetEx(_UDWORD idx)
{
  return TextList.HostLanguageGetEx(idx);
}

extern "C" _BOOL Prj_HostLanguageSet(_UDWORD language)
{
  return Prj_HostLanguageSetEx(1, language);
}

extern "C" _UDWORD Prj_GetLseProjectCrc(void)
{
  return(Project.GetLseProjectCrc());
}

extern "C" _PROJECT *Prj_GetProjectInfo(void)
{
  return Project.Get_PROJECT();
}

extern "C" char *Prj_GetProjectEntryName(_PRJ_ENTRY *pe)
{
  char *retcode = NULL;
  if(pe != NULL)
  {
    retcode = Project.GetNameOfEntry(pe);
  
    if(retcode == NULL)
    {
      retcode = pe->label;
    }
  }
  return retcode;
}