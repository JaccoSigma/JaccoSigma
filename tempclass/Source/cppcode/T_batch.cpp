#include "T_IPC.H"
#include "T_NUC.H"

#ifdef UC_MULTICPU
void _Batch::GetMultiID(_BOOL bar)
{
  _COMAP   *pcm;
  _UWORD   sb, i, n;
  _UDWORD  anzahl = VarList.GetNo();
  _ASCII   txt[100];
  _BOOL    switched;

  pcm = IpcIni.ComSet.mapper;
  for(i=0; i<256; i++)
  {
    if(pcm->used == _TRUE)
    {
      switched = _FALSE;
      for(n=1; n<UC_MULTICPU; n++)
      {
        if(pcm->multimap[n] != 0xFFFF)
        {
          sb = 0xFFFF;
          if(bar == _TRUE)
          {
            StrCpy(txt, sizeof(_ASCII), "Get ID (", sizeof(_ASCII));
            DToA(&txt[StrLen(txt, sizeof(_ASCII))], i, 0x0800, sizeof(_ASCII));
            StrCat(txt, sizeof(_ASCII), "[", sizeof(_ASCII));
            DToA(&txt[StrLen(txt, sizeof(_ASCII))], n, 0x0800, sizeof(_ASCII));
            StrCat(txt, sizeof(_ASCII), "]):", sizeof(_ASCII));
            sb = ScanBar.Add(txt, anzahl);
          }

          switched = _TRUE;
          SwitchMultiStation(i, n, _FALSE);
          VarList.GetIDSub(i, n, anzahl, sb);
          ScanBar.End(sb);
        }
      }

      if(switched == _TRUE)
        SwitchMultiStation(i, 0, _FALSE);
    }

    pcm += 1;
  }
}

_BOOL _Batch::SwitchMultiStationSub(_UWORD station, _UWORD multi_map_no, _BOOL call_interface)
{
  _COMAP *pcm;
  _UWORD comportno;
  _BOOL  retcode = _FALSE;

  pcm       = &IpcIni.ComSet.mapper[station];
  comportno = pcm->multimap[multi_map_no];

  if(pcm->aktmap != comportno) // nur wenn unterschied
  {
    retcode = _TRUE;
    pcm->aktmap = comportno;

   #ifndef CUTOFF_USER
    if(call_interface == _TRUE)
    {
      _FVOID_UWORD ptr;
      _UWORD       para = (multi_map_no << 8)|(station & 0xFF);
      ptr = (_FVOID_UWORD)find_lasal_method(LseThisPointer, METHOD_LSE_SWITCH_MULTICPU);
      if(ptr != NULL)
      {
        load_this(LseThisPointer);
        ptr(para);
      }
    }
   #endif
  }

  return(retcode);
}

void _Batch::MapAllMultiStation(_UWORD multi_map_no)
{
  _UWORD i;

  if(multi_map_no < UC_MULTICPU)
  {
    for(i=0; i<=255; i++)
      SwitchMultiStationSub(i, multi_map_no, _TRUE);
  }
}

_BOOL _Batch::SwitchMultiStation(_UWORD station, _UWORD multi_map_no, _BOOL change_lasalid)
{
  _BOOL retcode = _FALSE;

  if((station <= 255)&&(multi_map_no < UC_MULTICPU))
  {
    retcode = _TRUE;

    // es kann auch auf ein ungültiges comport umgeschaltet werden, anwender trägt verantwortung !!
    if(SwitchMultiStationSub(station, multi_map_no, change_lasalid) == _TRUE)
    {
      if(change_lasalid == _TRUE)
        VarList.ChangeMultiID(station, multi_map_no);
      new_PLC();
    }
  }

  return(retcode);
}

_BOOL _Batch::SetAktMap(_UWORD station, _UWORD mapno)
{
  _BOOL  retcode = _FALSE;
  _COMAP *pcm;

  if((station <= 255)&&(mapno < GetNo_COMPORT()))
  {
    pcm         = &IpcIni.ComSet.mapper[station];
    pcm->aktmap = mapno;
    retcode     = _TRUE;
  }

  return(retcode);
}

_UWORD _Batch::ChangeAktMap(_UWORD station, _UWORD multi_map_no)
{
  _COMAP *pcm;
  _UWORD comportno;
  _UWORD retcode = 0xFFFF;

  if((station <= 255)&&(multi_map_no < UC_MULTICPU))
  {
    pcm         = &IpcIni.ComSet.mapper[station];
    comportno   = pcm->multimap[multi_map_no];
    retcode     = pcm->aktmap;
    pcm->aktmap = comportno;
  }

  return(retcode);
}

void _Batch::LoginAllMultiStations(void)
{
  _UWORD   i, n;
  _COMPORT *pcp;
  _COMAP   *pcm;

  pcm = IpcIni.ComSet.mapper;
  for(i=0; i<256; i++)
  {
    if(pcm->used == _TRUE)
    {
      for(n=1; n<UC_MULTICPU; n++)
      {
        pcp = GetComportDirect(pcm->multimap[n]);
        if(pcp != NULL)
          login_PLC_Sub(pcp, (i | (n<<8)));
      }
    }
    pcm += 1;
  }
}
#endif

_UWORD _Batch::Insert_COMPORT_Sub(_COMDEF *cd, _BOOL observe)
{
  _UWORD   retcode = 0xFFFF;
  _UWORD   i, freeindex = 0xFFFF;
  _COMPORT *p;
  _DWORD   size;

  for(i=0; i<IpcIni.ComSet.no; i++)
  {
    p = &IpcIni.ComSet.ptr[i];
    if((p->comdef.typ == cd->typ)&&(p->comdef.adress == cd->adress)&&(p->comdef.ipadress == cd->ipadress)&&(p->comdef.port == cd->port))
    {
      if(observe == _TRUE)
      {
        p->observe = _TRUE;
      }
      return(i);
    }

    if(p->used == _FALSE)
    {
      freeindex = i;
    }
  }

  p = NULL;
  if(freeindex < IpcIni.ComSet.no)
  {
    p       = &IpcIni.ComSet.ptr[freeindex];
    retcode = freeindex;
  }
  else
  {
    size = (IpcIni.ComSet.no + 1) * sizeof(_COMPORT);
    if(Memory2((void**)&IpcIni.ComSet.ptr, size) != 0)
    {
      p       = &IpcIni.ComSet.ptr[IpcIni.ComSet.no];
      retcode = IpcIni.ComSet.no;
      IpcIni.ComSet.no ++;
    }
  }

  if(p != NULL)
  {
    init_COMPORT(p);
    p->comdef  = *cd;
    p->observe = observe;
    p->used    = _TRUE;
  }

  return(retcode);
}

_BOOL _Batch::Insert_COMPORT(_COMDEF *cd, _UWORD station, _BOOL observe, _UWORD multi_map_no)
{
  _UWORD  comsetno;
  _BOOL   retcode = _FALSE;
  _COMAP  *pcm;

  if(station < 256)
  {
    comsetno = 0xFFFF;
    pcm      = &IpcIni.ComSet.mapper[station];

   #ifndef UC_MULTICPU
    multi_map_no = 0;
   #endif

    if((pcm->aktmap == 0xFFFF)&&(multi_map_no == 0))
      comsetno = Insert_COMPORT_Sub(cd, observe);
   #ifdef UC_MULTICPU
    else if((multi_map_no > 0)&&(multi_map_no < UC_MULTICPU))
      comsetno = Insert_COMPORT_Sub(cd, observe);
   #endif

    if(comsetno != 0xFFFF)
    {
      retcode = _TRUE;
      if(multi_map_no == 0)
        pcm->aktmap = comsetno;
     #ifdef UC_MULTICPU
      pcm->used = _TRUE;
      pcm->multimap[multi_map_no] = comsetno;
     #endif
    }
  }

  return(retcode);
}

void _Batch::Init(void)
{
  RefListChangedCnt = 0;
  DoCheck = _FALSE;
  init_IPCINI(&IpcIni);
  ResetWatchOnlineTimeDelay();
}

void _Batch::Free(void)
{
  free_IPCINI(&IpcIni);
  Init();
}

_UDWORD _Batch::GetIpAdress(_ASCII **pp)
{
  _UDWORD retcode = 0;
  _UDWORD part;
  _UWORD  i;
  _ASCII  *p = *pp;

  for(i=0; i<4; i++)
  {
    part = AsciiToNo(&p) & 255;
    p += 1;
    retcode |= (part << (i*8));
  }

  *pp = p-1;

  return(retcode);
}

_UDWORD GetIpPort(_ASCII *p)
{
  _UWORD  pos = StrChr(p, ':', 1);
  
  if(pos < StrLen(p, 1))
  {
    p = &p[pos+1];
    return AsciiToNo(&p);
  }

  return 0; // defaultwert muss 0 sein (wegen loader)
}


#ifdef UC_IPC
void make_tcpiptext(_CHAR *txt, const _ASCII *name, _UDWORD adress)
{
  _UWORD i;

  StrCpy(txt, sizeof(_CHAR), name, sizeof(_ASCII));
  for(i=0; i<4; i++)
  {
    if(i)
      StrCat(txt, sizeof(_CHAR), ".", sizeof(_ASCII));
    NumToStr(&txt[StrLen(txt, sizeof(_CHAR))], adress&0xFF, sizeof(_CHAR));
    adress >>= 8;
  }
}

void make_cantext(_CHAR *txt, _UDWORD adress, _INTERFACE intfc)
{
  _UWORD can;

  switch(intfc)
  {
    case _COM_CAN2  : can =  2; break;
    case _COM_CAN20 : can = 20; break;
    case _COM_CAN21 : can = 21; break;
    case _COM_CAN22 : can = 22; break;
    case _COM_CAN23 : can = 23; break;
    case _COM_CAN24 : can = 24; break;
    case _COM_CAN25 : can = 25; break;
    case _COM_CAN26 : can = 26; break;
    case _COM_CAN27 : can = 27; break;
    case _COM_CAN28 : can = 28; break;
    case _COM_CAN29 : can = 29; break;
    default         : can =  1; break;
  }

  StrCpy(txt, sizeof(_CHAR), ": CAN", sizeof(_ASCII));
  NumToStr(&txt[StrLen(txt, sizeof(_CHAR))], can, sizeof(_CHAR));
  StrCat(txt, sizeof(_CHAR), " ", sizeof(_ASCII));
  NumToStr(&txt[StrLen(txt, sizeof(_CHAR))], adress, sizeof(_CHAR));
}

void make_rs232text(_CHAR *txt, _INTERFACE intfc)
{
  _UWORD com;

  switch(intfc)
  {
    case _COM_COM2  : com =  2; break;
    case _COM_COM3  : com =  3; break;
    case _COM_COM4  : com =  4; break;
    case _COM_COM5  : com =  5; break;
    case _COM_COM6  : com =  6; break;
    case _COM_COM7  : com =  7; break;
    case _COM_COM8  : com =  8; break;
    case _COM_COM9  : com =  9; break;
    case _COM_COM10 : com = 10; break;
    default         : com =  1; break;
  }

  StrCpy(txt, sizeof(_CHAR), ": COM", sizeof(_ASCII));
  NumToStr(&txt[StrLen(txt, sizeof(_CHAR))], com, sizeof(_CHAR));
  StrCat(txt, sizeof(_CHAR), " ", sizeof(_ASCII));
}

void _Batch::MakeOfflineTextSub(_CHAR *txt, _COMPORT *pc)
{
  if(pc != NULL)
  {
    switch(pc->comdef.typ)
    {
      case _COM_INTERN   : StrCpy(txt, sizeof(_CHAR), ": INTERN", sizeof(_ASCII));
                           break;
      case _COM_TCPIP2_P :
      case _COM_TCPIP1   :
      case _COM_TCPIP1_P :
                           make_tcpiptext(&txt[StrLen(txt, sizeof(_CHAR))], ": TCPIP ", pc->comdef.ipadress);
                           break;
//      case _COM_TCPIP2_P : make_tcpiptext(&txt[StrLen(txt, sizeof(_CHAR))], ": TCPIP2 ", pc->comdef.ipadress); // SA10817
//                           break;
      case _COM_CAN1     :
      case _COM_CAN2     :
      case _COM_CAN20    :
      case _COM_CAN21    :
      case _COM_CAN22    :
      case _COM_CAN23    :
      case _COM_CAN24    :
      case _COM_CAN25    :
      case _COM_CAN26    :
      case _COM_CAN27    :
      case _COM_CAN28    :
      case _COM_CAN29    : make_cantext(&txt[StrLen(txt, sizeof(_CHAR))], pc->comdef.adress, pc->comdef.typ);
                           break;
      case _COM_COM1     :
      case _COM_COM2     :
      case _COM_COM3     :
      case _COM_COM4     :
      case _COM_COM5     :
      case _COM_COM6     :
      case _COM_COM7     :
      case _COM_COM8     :
      case _COM_COM9     :
      case _COM_COM10    : make_rs232text(&txt[StrLen(txt, sizeof(_CHAR))], pc->comdef.typ);
                           break;
      default            : break;
    }
  }
}
#endif

_CHAR* _Batch::MakeOfflineText(_CHAR *txt, _BOOL state)
{
  _COMPORT *pcp;
  _UWORD   no, i, anz, stations[256];
 #ifndef UC_IPC
  _BOOL    comma = _FALSE;
 #endif

  StrCpy(txt, sizeof(_CHAR), "OFFLINE STATION: ", sizeof(_ASCII));

  no = GetNo_COMPORT();
  while(no)
  {
    no -= 1;
    pcp = GetComportDirect(no);
    if((pcp->observe == _TRUE)&&(pcp->offline == _TRUE))
    {
      anz = GetStationViaComportIndex(stations, no);
     #ifndef UC_IPC
      for(i=0; i<anz; i++)
      {
        if(comma == _TRUE)
          StrCat(txt, sizeof(_CHAR), ",", sizeof(_ASCII));
        NumToStr(&txt[StrLen(txt, sizeof(_CHAR))], stations[i]&0xFF, sizeof(_CHAR));
        comma = _TRUE;
      }
     #else
      for(i=0; i<anz; i++)
      {
        StrCat(txt, sizeof(_CHAR), "|", sizeof(_ASCII));
        NumToStr(&txt[StrLen(txt, sizeof(_CHAR))], stations[i]&0xFF, sizeof(_CHAR));
       #ifdef UC_MULTICPU
        if(stations[i] & 0xFF00)
        {
          StrCat(txt, sizeof(_CHAR), "[", sizeof(_ASCII));
          NumToStr(&txt[StrLen(txt, sizeof(_CHAR))], (stations[i]>>8)&0xFF, sizeof(_CHAR));
          StrCat(txt, sizeof(_CHAR), "]", sizeof(_ASCII));
        }
       #else
        _ASCII *pa = GetPLCStateText(pcp->plcstate);
        if(pa != NULL)
        {
          StrCat(txt, sizeof(_CHAR), " [", sizeof(_ASCII));
          StrCat(txt, sizeof(_CHAR), pa, sizeof(_ASCII));
          StrCat(txt, sizeof(_CHAR), "]", sizeof(_ASCII));
        }
       #endif
        MakeOfflineTextSub(&txt[StrLen(txt, sizeof(_CHAR))], pcp);
      }
     #endif
    }
  }

  if(state == _TRUE)
    StrCat(txt, sizeof(_CHAR), "|<ESC>...offlinemode|<ENTER>...restart", sizeof(_ASCII));

  return(txt);
}

void _Batch::ResetWatchOnlineTimeDelay(void)
{
  WatchOnlineTimeDelay = get_abstime();
}

void _Batch::ResetWatchOnline(void)
{
  _UWORD   no;
  _COMPORT *pcp;

  no = GetNo_COMPORT();
  while(no)
  {
    no -= 1;
    pcp = GetComportDirect(no);

    if(pcp != NULL)
    {
      if(pcp->observe == _TRUE)
      {
        if(pcp->offline == _TRUE)
        {
          pcp->offline_cnt   = 0;
          pcp->offline       = _FALSE;
          pcp->reflist.login = _LOGIN_INIT;
        }
      }
    }
  }

  ResetWatchOnlineTimeDelay();
}

_ASCII *GetDNS(_ASCII *dst, _ASCII **ppsrc)
{
  _ASCII *retcode = dst;
  _ASCII *src = *ppsrc;
  
  *dst = 0;
  while(1)
  {
    switch(*src)
    {
      case ':' :
      case 0   : *dst = 0;
                 return retcode;
    }
    
    if(MemCmp(src, "OBSERVE", 7) == 0)
    {
      *dst = 0;
      *ppsrc = src;
      return retcode;
    }
    
    *dst = *src;
    src ++;
    dst ++;
  }
  
  return retcode;
}

_BOOL _Batch::DoStation(_ASCII *n)
{
  _UWORD  no;
  _BOOL   retcode = _FALSE;
  _COMDEF cd;
  _BOOL   online_watch;
  _UWORD  multi_map_no = 0;
  _UDWORD my_ipadress;
  _ASCII  dnsname[256];

  init_COMDEF(&cd);
  no = AsciiToNo(&n);                                  // station number of mae !!

 // n = "=DNSmail.sigmatek.at:1961";
 // n = "=DNSwww.google.com:1961";
  
  if(n[0] == '[')
  {
    n += 1;
    multi_map_no = AsciiToNo(&n);
    n += 1;
  }

  if(n[0] == '=')
  {
    retcode = _TRUE;
    n       = &n[1];

    if(MemCmp(n, "INTERN", 6) == 0)
    {
      cd.typ    = _COM_INTERN;
      n        += 6;
    }
    else if((MemCmp(n, "TCPIP1", 6) == 0)||(MemCmp(n, "TCPIP2", 6) == 0))
    {
      // SA11817: TCPIP2 wird auf TCPIP1 gemapped
      n = &n[6];
      cd.port     = GetIpPort(n);
      cd.ipadress = GetIpAdress(&n);
      cd.typ      = _COM_TCPIP1_P;
      
      if(GetTCPIPAddress(1, &my_ipadress, NULL, NULL) == _TRUE)
      {
        if(my_ipadress == cd.ipadress)
          cd.typ = _COM_INTERN;
      }
    }
    else if(MemCmp(n, "DNS", 3) == 0)
    {
      // sytax "name":port
      n = &n[3];
      cd.port     = GetIpPort(n);
      cd.ipadress = GetTCPIPAddressFromDNS(GetDNS(dnsname, &n));
      cd.typ      = _COM_TCPIP1_P;
      
      if(GetTCPIPAddress(1, &my_ipadress, NULL, NULL) == _TRUE)
      {
        if(my_ipadress == cd.ipadress)
          cd.typ = _COM_INTERN;
      }
    }
    else if(MemCmp(n, "CAN", 3) == 0)
    {
      switch((n[3]-'0') * 10 + (n[4]-'0'))
      {
        case 1  : cd.typ = _COM_CAN1;  break;
        case 2  : cd.typ = _COM_CAN2;  break;
        case 20 : cd.typ = _COM_CAN20; break;
        case 21 : cd.typ = _COM_CAN21; break;
        case 22 : cd.typ = _COM_CAN22; break;
        case 23 : cd.typ = _COM_CAN23; break;
        case 24 : cd.typ = _COM_CAN24; break;
        case 25 : cd.typ = _COM_CAN25; break;
        case 26 : cd.typ = _COM_CAN26; break;
        case 27 : cd.typ = _COM_CAN27; break;
        case 28 : cd.typ = _COM_CAN28; break;
        default : cd.typ = _COM_CAN29; break;
      }

      n         = &n[5];
      cd.adress = AsciiToNo(&n);
    }
/*
    else if(MemCmp(n, "CAN1", 4) == 0)
    {
      n = &n[4];
      cd.typ    = _COM_CAN1;
      cd.adress = AsciiToNo(&n);
    }
    else if(MemCmp(n, "CAN2", 4) == 0)
    {
      n = &n[4];
      cd.typ    = _COM_CAN2;
      cd.adress = AsciiToNo(&n);
    }
*/
    else if(MemCmp(n, "COM", 3) == 0)
    {
      n = &n[3];
      cd.adress = 0;
      switch(AsciiToNo(&n))
      {
        case  2 : cd.typ = _COM_COM2; break;
        case  3 : cd.typ = _COM_COM3; break;
        case  4 : cd.typ = _COM_COM4; break;
        case  5 : cd.typ = _COM_COM5; break;
        case  6 : cd.typ = _COM_COM6; break;
        case  7 : cd.typ = _COM_COM7; break;
        case  8 : cd.typ = _COM_COM8; break;
        case  9 : cd.typ = _COM_COM9; break;
        case  10: cd.typ = _COM_COM10; break;
        default : cd.typ = _COM_COM1; break;
      }
    }
    else if(MemCmp(n, "LOCAL", 5) == 0)
    {
      cd.typ    = _COM_LOCAL;
      n        += 5;
    }
    else
    {
      retcode = _FALSE;
    }

    if(retcode == _TRUE)
    {
      online_watch = _FALSE;
      if(MemCmp(n, "OBSERVE", 7) == 0)
        online_watch = _TRUE;

      retcode = _FALSE;
      if(user_station(no, &cd, &online_watch) == _TRUE)
        retcode = Insert_COMPORT(&cd, no, online_watch, multi_map_no);
    }
  }

  return(retcode);
}

_BOOL IsNum(_ASCII chr)
{
  if((chr >= '0') && (chr <= '9'))
    return(_TRUE);
  return(_FALSE);
}

void PrepaireCAN(_ASCII *line)
{
  _ASCII *pa = line;

  while(pa[0] != 0)
  {
    if((pa[0] == 'C')&&(pa[1] == 'A')&&(pa[2] == 'N'))
    {
      if(IsNum(pa[3]) == _TRUE)
      {
        if(IsNum(pa[4]) == _FALSE)
        {
          MemMove(&pa[4], &pa[3], StrLen(&pa[3], sizeof(_ASCII)) + 1);
          pa[3] = '0';
		  return;
        }
      }
    }

	pa += 1;
  }
}

void _Batch::Read(void)
{
  _ASCII *p, line[300];
  _File  File;
  _BOOL  run = _TRUE;

  // PROJECT .......... dpne for project
  // DATA ............. dpne for data
  // INITPIC .......... dpne for startbmp (gfx)
  // SIMULATION ....... dpne for serversimulation file
  // STATION .......... assignment for communication
  //     STATION  1 = LOCAL
  //     STATION  0 = INTERN
  //     STATION 20 = CAN1 10
  //     STATION 20 = CAN2 10
  //     STATION 20 = COM1 ; COM2, COM3, COM4

  if(File.Open(LOAD, PROJECTINIFILE) == _TRUE)     // open inifile (read-only)
  {
    do
    {
      if(File.Read_CRLF(line, 300) == _TRUE) // read single cmd-line from file
      {
        StrUp(line, sizeof(_ASCII), 0);      // convert letters to big letters
        DelComment(line, sizeof(_ASCII));     // ifnec. delete present comment
        PrepaireCAN(line);
        DelSpace(line, sizeof(_ASCII));        // delete all space in cmd-line

        if(MemCmp(line, "PROJECT", 7) == 0)
        {
          StrCpy(IpcIni.ProjectDPNE, sizeof(_ASCII), &line[7], sizeof(_ASCII));              // drive path of project
        }
        else if(MemCmp(line, "DATA", 4) == 0)
        {
          StrCpy(IpcIni.DataDPNE, sizeof(_ASCII), &line[4], sizeof(_ASCII));                    // drive path of data
        }
        else if(MemCmp(line, "INITPIC", 7) == 0)
        {
          StrCpy(IpcIni.StartGfxDPNE, sizeof(_ASCII), &line[7], sizeof(_ASCII));        // drive path of startpicture
        }
        else if(MemCmp(line, "STARTPIC", 8) == 0)
        {
          p = &line[8];
          IpcIni.StartPicture = AsciiToNo(&p);                      // no of startpicture
        }
        else if(MemCmp(line, "STATION", 7) == 0)
        {
          DoStation(&line[7]);
        }
        else if(MemCmp(line, "LOGINDELAY", 10) == 0)
        {
          p = &line[10];
          IpcIni.LoginDelayTime = AsciiToNo(&p);
        }
      }
      else
      {
        run = _FALSE;
      }
    }
    while(run == _TRUE);
  }
  File.Close();
  
 #ifndef CUTOFF_USER
  _FVOID_PT ptr;
  void *pini = (void*)&IpcIni;

  ptr = (_FVOID_PT)find_lasal_method(LseThisPointer, METHOD_LSE_IPCINI);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    ptr(pini);
  }
 #endif
}

_COMPORT *_Batch::GetComportDirect(_UWORD index)
{
  if(index < IpcIni.ComSet.no)
    return(&IpcIni.ComSet.ptr[index]);
  return(NULL);
}

_COMPORT *_Batch::Get_COMPORT(_UWORD station)
{
  if(station < 256)
    return(GetComportDirect(IpcIni.ComSet.mapper[station].aktmap));
 #ifdef UC_MULTICPU
  else
  {
    _UWORD multi_map_no = ((station >> 8) & 0xFF);
    station      = station & 0xFF;
    if(multi_map_no < UC_MULTICPU)
      return(GetComportDirect(IpcIni.ComSet.mapper[station].multimap[multi_map_no]));
  }
 #endif

  return(NULL);
}

#ifdef UC_DELETE_REFLIST
_UWORD _Batch::GetAllStationNumbersToSameComport(_UBYTE *dst, _UWORD station)
{
  _UWORD retcode = 0;
  _UWORD i, index;

  if(station < 256)
  {
    index = IpcIni.ComSet.mapper[station].aktmap;
    if((index != 0xFFFF)&&(index < IpcIni.ComSet.no))
    {
      for(i=0; i<256; i++)
      {
        if(IpcIni.ComSet.mapper[i].aktmap == index)
        {
          retcode += 1;
          *dst = i;
          dst += 1;
        }
      }
    }
  }

  return(retcode);
}
#endif

_UWORD _Batch::GetStation(_UWORD index)
{
  _UWORD retcode = 0xFFFF;
  _UWORD i;


  for(i=0; i<256; i++)
  {
    if(IpcIni.ComSet.mapper[i].aktmap == index)
    {
      retcode = i;
      break;
    }
  }

  return(retcode);
}

_BOOL _Batch::GetObserve(_UWORD station)
{
  _BOOL    retcode = _FALSE;
  _COMPORT *pcp;

  pcp = Get_COMPORT(station);
  if(pcp != NULL)
    retcode = pcp->observe;

  return(retcode);
}

_BOOL _Batch::SetObserve(_UWORD station, _BOOL observe)
{
  _BOOL    retcode = _FALSE;
  _COMPORT *pcp;

  pcp = Get_COMPORT(station);
  if(pcp != NULL)
  {
    pcp->observe = observe;
    retcode      = _TRUE;
  }

  return(retcode);
}

_COMPORT* _Batch::Seek_COMPORT(_COMDEF *p)
{
  _UWORD  no = IpcIni.ComSet.no;
  _COMPORT *pcp;

  pcp = IpcIni.ComSet.ptr;
  while(no)
  {
    if(pcp->comdef.handle == p->handle)
      return(pcp);
    pcp += 1;
    no  -= 1;
  }

  return(NULL);
/*
  _UWORD  i;
  _COMPORT *pcp;

  pcp = ComSet.ptr;
  for(i=0; i<ComSet.no; i++)
  {
    if((pcp->comdef.typ == p->typ) && (pcp->comdef.adress == p->adress))
      return(pcp);
    pcp += 1;
  }

  return(NULL);
*/
}

_BOOL _Batch::IsComdefMultipleUsed(_UWORD station)
{
  _BOOL  retcode = _FALSE;
  _UWORD i, index;
  _COMAP *pcm;

  if(station < 256)
  {
    index = IpcIni.ComSet.mapper[station].aktmap;

    if((index != 0xFFFF) && (index < IpcIni.ComSet.no))
    {
      pcm = IpcIni.ComSet.mapper;
      for(i=0; i<256; i++)
      {
        if((pcm->aktmap == index) && (station != i))
          return(_TRUE);
       #ifdef UC_MULTICPU
        _UWORD n;
        if(pcm->used == _TRUE)
        {
          for(n=1; n<UC_MULTICPU; n++)
          {
            if(pcm->multimap[n] == index)
              return(_TRUE);
          }
        }
       #endif
        pcm += 1;
      }
    }
  }

  return(retcode);
}

_UWORD _Batch::GetStartPicture(void)
{
  return(IpcIni.StartPicture);
}
_ASCII* _Batch::GetProjectDPNE(void)
{
  return(IpcIni.ProjectDPNE);
}
_ASCII* _Batch::GetDataDPNE(void)
{
  return(IpcIni.DataDPNE);
}
_ASCII* _Batch::GetStartGfxDPNE(void)
{
  return(IpcIni.StartGfxDPNE);
}
_ASCII* _Batch::GetSimulationDPNE(void)
{
  return(IpcIni.SimulationDPNE);
}

_UWORD _Batch::GetNo_COMPORT(void)
{
  return(IpcIni.ComSet.no);
}

_COMPORT* _Batch::GetDirect_COMPORT(_UWORD i)
{
  if(i<IpcIni.ComSet.no)
    return(&IpcIni.ComSet.ptr[i]);
  return(NULL);
}

_UDWORD  _Batch::GetLoginDelayTime(void)
{
  return(IpcIni.LoginDelayTime);
}

#ifdef OK_DIAGNOSTIC
_BOOL _Batch::Read(_AsciiPuffer *Pap, const _ASCII *dpne)
{
  _File  File;
  _BOOL  retcode = _FALSE;
  _ASCII line[300];
  _BOOL  run = _TRUE;

  Pap->Free();

  if(File.Open(LOAD, MakeRamFile(line, (char*)dpne)) == _TRUE)     // open inifile (read-only)
  {
    Pap->MakeMemory(File.Length());
    retcode = _TRUE;
    do
    {
      if(File.Read_CRLF(line, 300) == _TRUE) // read single line from file
        retcode = Pap->Add(line);
      else
        run = _FALSE;

      if(retcode == _FALSE)
        run = _FALSE;
    }
    while(run == _TRUE);

    Pap->FinishMemory();
    File.Close();
  }

  if(retcode == _FALSE)
    Pap->Free();

  return(retcode);
}
#endif

_COMSET  *_Batch::GetComSet(void)
{
  return(&IpcIni.ComSet);
}

void insert_station(_UWORD *pst, _UWORD *panz, _UWORD st)
{
  _UWORD no = *panz;

  while(no)
  {
    if(*pst == st)
      return;
    pst += 1;
    no  -= 1;
  }

  *pst = st;
  (*panz) += 1;
}

_UWORD _Batch::GetStationViaComportIndex(_UWORD *pstations, _UWORD index)
{
  _UWORD i, retcode = 0;
  _COMAP *pcm;

 #ifdef UC_MULTICPU
  pcm = IpcIni.ComSet.mapper;
  for(i=0; i<256; i++)
  {
    _UWORD n;
    if(pcm->used == _TRUE)
    {
      for(n=0; n<UC_MULTICPU; n++)
      {
        if(pcm->multimap[n] == index)
          insert_station(pstations, &retcode, (n<<8) | i);
      }
    }
    pcm += 1;
  }
 #else
  pcm = IpcIni.ComSet.mapper;
  for(i=0; i<256; i++)
  {
    if(pcm->aktmap == index)
      insert_station(pstations, &retcode, i);
    pcm += 1;
  }
 #endif

  return(retcode);
}

void _Batch::StartRefreshList(void)
{
  if(RefListChangedCnt != 0)
  {
    RefListChangedCnt = 0;

    _UDWORD nox, no  = IpcIni.ComSet.no;
    _COMPORT *pc = IpcIni.ComSet.ptr;

    while(no--)
    {
      Mutex(_TRUE, MUTEX_StartREFLIST);
      nox = pc->add_reflist[0];
      pc->add_reflist[0] = 0;
      Mutex(_FALSE, MUTEX_StartREFLIST);

      if(nox != 0)
      {
        StartStopRefresh(&pc->comdef, nox, 0);
      }

      Mutex(_TRUE, MUTEX_StartREFLIST);
      nox = pc->add_reflist[1];
      pc->add_reflist[1] = 0;
      Mutex(_FALSE, MUTEX_StartREFLIST);

      if(nox != 0)
      {
        StartStopRefresh(&pc->comdef, nox, 1);
      }

      pc ++;
    }
  }
}

void _Batch::StartRefreshList(_COMPORT *cp, _UDWORD no, _UDWORD idx)
{
  Mutex(_TRUE, MUTEX_StartREFLIST);
  cp->add_reflist[idx] = no;
  RefListChangedCnt++;  
  Mutex(_FALSE, MUTEX_StartREFLIST);
//  StartStopRefresh(&cp->comdef, nox, idx);
}

//****************************************************************************
//  LASAL Interface
//****************************************************************************
//****************************************************************************
//****************************************************************************

extern "C" _COMSET *Lse_GetComSet(void)
{
  return(Batch.GetComSet());
}

extern "C" _COMPORT* Get_COMPORT(_UWORD station)
{
  return(Batch.Get_COMPORT(station));
}

extern "C" _UWORD GetStartPicture(void)
{
  return(Batch.GetStartPicture());
}

extern "C" void MakeOfflineText(_CHAR *txt, _BOOL state)
{
  Batch.MakeOfflineText(txt, state);
}

extern "C" _BOOL WatchOffline(void)
{
  return(Batch.WatchOffline());
}

