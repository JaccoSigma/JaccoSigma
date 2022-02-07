#include "T_IPC.H"
#include "T_NUC.H"
#include "COMINTFC.H"


//#define ACCESS_DENIED 0x80000010
#define MAX_COMDATA 1024 // obechr 300
#define PROJECT_5920 // Änderungen (Franz Aschl + Ronald Roiter)

_UBYTE directdata[MAX_COMDATA];
#ifdef PROJECT_5920
static _UDWORD TimeDelayOfflineWindow = 0;
#endif

void SetTimeDelayOfflineWindow(_UDWORD time_ms)
{
 #ifdef PROJECT_5920
  TimeDelayOfflineWindow = time_ms;
 #endif 
}

_UDWORD PlcLogin(_COMPORT *pcp)
{
  _COMDEF *pcd = &pcp->comdef;

 #ifdef UC_EXOONLINE
  _DOIT doit = ExoLogin(pcd);
//  if(*(_UDWORD*)&pcd->handle == UC_EXOONLINE)
  if(doit == _IDIDIT)
    return 0;
  else if(doit == _IFAILED)
    return 1;
 #endif

  //pcd->port = 1960;

  _UDWORD retcode = LOGIN(pcd);

  if(retcode == 0)
  {
    pcp->loader_version = 0;
    GetLoaderVersion(pcp);
  }

  return retcode; 
}

#ifdef CANOPEN
_SVRTYP get_SVRTYP(_UWORD format, _VARTYPE vartype)
{
  switch(vartype)
  {
    case BINARY : switch(format)
                  {
                    case SIZE_BYTE       : return(0);
                    case SIZE_WORD       : return(1);
                    case SIZE_DWORD      : return(2);
                    case SIZE_UBYTE      : return(3);
                    case SIZE_UWORD      : return(4);
                    case SIZE_UDWORD     : return(5);
                    case SIZE_HBYTE      : return(6);
                    case SIZE_HWORD      : return(7);
                    case SIZE_HDWORD     : return(8);
                    case SIZE_BBYTE      : return(9);
                    case SIZE_BWORD      : return(10);
                    case SIZE_BDWORD     : return(11);
                    case SIZE_REAL       : return(12);
                  }
                  break;

    case FSTRING :                         return(13);

    case FTIME  : switch(format)
                  {
                    case SIZE_HHMM       : return(14);
                    case SIZE_HHMMSS     : return(15);
                  }
                  break;

    case FDATE  : switch(format)
                  {
                    case SIZE_DDMM       : return(16);
                    case SIZE_DDMMYY     : return(17);
                    case SIZE_WWDDMM     : return(18);
                    case SIZE_WWDDMMYY   : return(19);
                    case SIZE_DDMMYYYY   : return(20);
                    case SIZE_WWDDMMYYYY : return(21);
                    case SIZE_YYYYMMDD   : return(23);
                    case SIZE_YYMMDD     : return(24);
                    case SIZE_YYMM       : return(25);
                  }
                  break;

	case BIT	    : return(22);

//                 BIT,           // bit datatype
//                 COMPLEX,       // structure, complex
//                 FCONSTANT,     // constant type
//                 OBJECT

  }

  return(DEFSVRTYP);
}

_SVRTYP get_SVRTYP_from_LSEVARINFO(_LSEVARINFO *pi)
{
  if(pi == NULL)
    return DEFSVRTYP;
    
  _SVRTYP retcode = get_SVRTYP(pi->format, pi->vartype);
 #ifdef USER_LISEC
  if(retcode == 12) // REAL
  {
    if(pi->statistic.parameter & 0x8000)
      retcode = 23; // LISEC_REAL
  }
 #endif
  return(retcode);
}

_SVRTYP get_SVRTYP_from_VARINFO(_VARINFO *pi)
{
  _SVRTYP retcode = get_SVRTYP(pi->format, pi->vartype);
 #ifdef USER_LISEC
  if(retcode == 12)  // REAL
  {
    if(pi->statistic.parameter & 0x8000)
      retcode = 23; // LISEC_REAL
  }
 #endif
  return(retcode);
}
#endif

_UDWORD PlcGetLasalId(_COMDEF *pcd, _ASCII *label, _LSEVARINFO *plvi)
{
  _UBYTE  *result = NULL;
  _UDWORD id, chmode;
  _UDWORD retry = 0;
  
 #ifdef UC_EXOONLINE
  if(IsExoOnline(pcd))
  {
    if(plvi == NULL)
      return ExoGetLasalId(&id, pcd, label);
    return ExoGetLasalId(&plvi->exoinfo, pcd, label);
  }
 #endif
  
 PERFORM_RETRY:
  
 #ifdef CANOPEN
  if(TxCommandIntern(I_GET_OBJ, StrLen(label, sizeof(_ASCII))+1, label, pcd, (void**)&result, get_SVRTYP_from_LSEVARINFO(plvi)) == DONE)
 #else
  if(TxCommandIntern(I_GET_OBJ, StrLen(label, sizeof(_ASCII))+1, label, pcd, (void**)&result, DEFSVRTYP) == DONE)
 #endif
  {
    if(result == NULL)
    {
      if(retry < 2)
      {
        retry += 1;
        goto PERFORM_RETRY;
      }
      
      return 0;
    }
  
  
//    length    = *(_UWORD*)result;
    result  += 2;
    id       = *(_UDWORD*)result;
    result  += 8;
    chmode   = *(_UDWORD*)result;

    if(chmode < 3) // just server-channel is accessible
    {
      return id;
    }
   #ifdef PGTEST
    else if(chmode < 7)
    {
      _ASCII txt[120];
      StrCpy(txt, sizeof(_ASCII), "access to client denied:", sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), label, sizeof(_ASCII));
      Annahme(txt, 0); // just server-channels are accessible
    }
   #endif
  }
  return 0;
}

_DWORD TxCommandIntern(_UDWORD bef, _UDWORD len, void *pdata, _COMDEF *pcd, void **presult, _SVRTYP svrtyp)
{
 #ifdef UC_EXOONLINE
  if(IsExoOnline(pcd))
  {
    return ExoTxCommand(bef, len, pdata, pcd, presult, svrtyp);
  }
 #endif

  return LseTxCommand(bef, len, pdata, pcd, presult, svrtyp);
}




/*
_UDWORD TxCommandIntern(_UDWORD bef, _UDWORD len, void *pdata, _COMDEF *pc, void **presult, _SVRTYP svrtyp)
{
 #ifdef CANOPEN
  return(TXCOMMAND(bef, len, pdata, pc, presult, svrtyp));
 #else
  return(ST_TxCommandIntern(bef, len, pdata, pc, presult));
 #endif
}
*/

_BOOL send_bdint(_UDWORD, _VARINFO*);

#define MIN_INDEXDAT   1300
#define MIN_REFDAT     16000

#ifdef OLDSTYLE_CAN
_UWORD oldstylecount;
#endif
_BOOL is_online(_COMDEF *pc)
{
  // im Kernel intern nicht mehr verwenden, ist nur wegen Kompatibilität zu älterer Version vorhanden.
  _BOOL  retcode = _FALSE;
  _UBYTE *result = NULL;

  if(TxCommandIntern(I_GET_VERSION, 0, NULL, pc, (void**)&result, DEFSVRTYP) == DONE)
  {
    if(result != NULL)
    {
      if(*(_UWORD*)result == 4)
      {
        retcode = _TRUE;
      }
    }
  }

 #ifdef OLDSTYLE_CAN
  #ifndef CANOPEN
  if(retcode == _TRUE)
  {
    oldstylecount = 0;
  }
  else if(oldstylecount < 2)
  {
    oldstylecount += 1;
    retcode = _TRUE;
  }
  #endif
 #endif

  return(retcode);
}

#ifdef PROJECT_5920
extern "C" unsigned long ThreadOnlineState(_COMPORT *pcp)
{
  _UBYTE  *result = NULL;
  _COMDEF *pcd = &pcp->comdef;

  if(pcp->async_onltimex == 0)
  {
    pcp->async_onlstate = 255; // OFFLINE Window
    return 0; // Thread beenden
  }

  if(TxCommandIntern(I_GET_VERSION, 0, NULL, pcd, (void**)&result, DEFSVRTYP) == DONE)
  {
    if(result != NULL)
    {
      if(*(_UWORD*)result == 4)
      {
        pcp->async_onlstate = 0;
        return 0; // wieder online
      }
    }
  }

  if((lib_tabsolute_nonzero() - pcp->async_onltimex) >= TimeDelayOfflineWindow)
  {
    pcp->async_onlstate = 255; // OFFLINE Window
    return 0; // Thread beenden
  }
  
  return 1; // Thread weiter aufrufen
}
#endif

_BOOL _Batch::IsOnline(_COMPORT *pcp)
{
  _UBYTE  *result = NULL;
  _COMDEF *pcd    = &pcp->comdef;

 #ifdef UC_EXOONLINE
  if(IsExoOnline(pcd))
  {
    return ExoChkOnline(pcd);
  }
 #endif

 #ifdef PROJECT_5920
  if(pcp->async_onlstate > 1)
  {
    if(pcp->async_onlstate == 255)
    {
      pcp->async_onlstate = 1;
      return _FALSE;
    }
  
    return _TRUE;
  }
 #endif

  if(TxCommandIntern(I_GET_VERSION, 0, NULL, pcd, (void**)&result, DEFSVRTYP) == DONE)
  {
    if(result != NULL)
    {
      if(*(_UWORD*)result == 4)
      {
        pcp->offline_cnt = 0;
        return _TRUE;
      }
    }
  }
  
  if((pcd->typ == _COM_CAN1)||
     (pcd->typ == _COM_CAN2)||
     (pcd->typ == _COM_CAN20)||
     (pcd->typ == _COM_CAN21)||
     (pcd->typ == _COM_CAN22)||
     (pcd->typ == _COM_CAN23)||
     (pcd->typ == _COM_CAN24)||
     (pcd->typ == _COM_CAN25)||
     (pcd->typ == _COM_CAN26)||
     (pcd->typ == _COM_CAN27)||
     (pcd->typ == _COM_CAN28)||
     (pcd->typ == _COM_CAN29)
    )
  {
    pcp->offline_cnt += 1;
    if(pcp->offline_cnt < 3) // bei can-bus mehrere retry (3)
    {
      DoCheck = _TRUE; // ab jetzt alle 500ms online überprüfen
      return _TRUE;
    }
    else
    {
      pcp->offline_cnt = 0;
    }
  }
 #ifdef PROJECT_5920
  else 
  {
    if((pcp->async_onlstate == 0) && (TimeDelayOfflineWindow > 1000))
    {
      pcp->async_onltimex = lib_tabsolute_nonzero();
      pcp->async_onlstate = 1;
      if(lib_thread_create2((void*)ThreadOnlineState, 11, 100000, pcp, NULL) != 0)
      {
        pcp->async_onlstate = 2;
        return _TRUE;
      }
    }
  }
 #endif

  return _FALSE;
}

#define SA32155

#ifdef SA32155 // *********************************************************************************

typedef struct
{
  _UDWORD ipadress;
  _UWORD  index;
}
_AsyncPingData;

extern "C" void AsyncPing(void* pdata, _UDWORD length)
{
  _AsyncPingData *papd = (_AsyncPingData*)pdata;
  _CHAR          tmp[100];
  _ASCII         iptxt[100];
  _COMPORT       *pcp;
 
  format_ipaddress(tmp, papd->ipadress);
  StrCpy(iptxt, 1, tmp, sizeof(_CHAR));
  pcp = Batch.GetComportDirect(papd->index);
  if(pcp != NULL)
  {
    if(ExternPing(iptxt) == 2)
    {
      // keine antwort, somit immernoch offline
      pcp->async_pingstate = ASYNCPING_OFFLINE;
    }
    else
    {
      // antwort bekommen, somit online
      pcp->async_pingstate = ASYNCPING_ONLINE;
    }
    
 //   pcp->async_plcstate ++;
  }
}

_BOOL PingIp(_COMPORT *pcp, _UWORD index)
{
  _AsyncPingData AsyncPingData;
 
 #ifdef UC_EXOONLINE
  if(IsExoOnline(&pcp->comdef))
    return ExoPing(&pcp->comdef);
 #endif
 
  if((pcp->comdef.typ == _COM_TCPIP1)||(pcp->comdef.typ == _COM_TCPIP1_P))
  {
    if(pcp->async_pingstate == ASYNCPING_OFFLINE)
    {
      pcp->async_pingstate = ASYNCPING_BUSY;
      AsyncPingData.ipadress = pcp->comdef.ipadress;
      AsyncPingData.index    = index;
      if(Thread2_AddAsyncMethod(AsyncPing, NULL, &AsyncPingData, sizeof(_AsyncPingData)) == _FALSE)
      {
        pcp->async_pingstate = ASYNCPING_OFFLINE;
      }
    }
   
    if(pcp->async_pingstate == ASYNCPING_ONLINE)
    {
      pcp->async_pingstate = ASYNCPING_OFFLINE;
      return _TRUE;
    }
   
    return _FALSE;
  }
 
  return _TRUE;
}
#else // SA32155 **********************************************************************************
_BOOL PingIp(_COMPORT *pcp, _UWORD index)
{
  _CHAR tmp[100];
  _ASCII iptxt[100];
 
 #ifdef UC_EXOONLINE
  if(IsExoOnline(&pcp->comdef))
    return ExoPing(&pcp->comdef);
 #endif
 
  if((pcp->comdef.typ == _COM_TCPIP1)||(pcp->comdef.typ == _COM_TCPIP1_P))
  {
    format_ipaddress(tmp, pcp->comdef.ipadress);
    StrCpy(iptxt, 1, tmp, sizeof(_CHAR));
    if(ExternPing(iptxt) == 2)
      return _FALSE; // broken connection
  }
  
  return _TRUE;
}
#endif // SA32155 *********************************************************************************


_BOOL _Batch::TryToGetOnline(_COMPORT *pcp, _UWORD index)
{
  _BOOL  retcode = _FALSE;

  if(PingIp(pcp, index) == _TRUE)
  {
    pcp->plcstate = GetPLCState(pcp); // cpu-status ermitteln
    if(PlcLogin(pcp) == 0)
    {
      // unbedingt auf init setzen, ansonsten wird funktion do_login nicht ausgeführt.
      // LOGOUT ist nicht notwendig (erwin pranz) -> loader weiss was zu tun ist.
      pcp->reflist.login = _LOGIN_INIT;
      pcp->offline       = _FALSE;
      retcode            = _TRUE;

     #ifndef CUTOFF_USER
      _FVOID_UWORD ptr;
      _UWORD       st, anz, stations[256];

      ptr = (_FVOID_UWORD)find_lasal_method(LseThisPointer, METHOD_LSE_ONLINE);
      if(ptr != NULL)
      {
        anz = GetStationViaComportIndex(stations, index);
        while(anz)
        {
          anz -= 1;
          st = stations[anz];
          load_this(LseThisPointer);
          ptr(st);
        }
      }
     #endif
    }
  }

  return(retcode);
}

_BOOL _Batch::WatchOffline(void)
{
  // automatisches online während offlinefenster
  _COMPORT *pcp;
  _UWORD   no;
  _BOOL    retcode = _FALSE;

  no = GetNo_COMPORT();
  while(no)
  {
    no -= 1;
    pcp = GetComportDirect(no);
    // observe eingeschaltet und offline bereits aufgetreten
    if((pcp->observe == _TRUE)&&(pcp->offline == _TRUE))
    {
      if(TryToGetOnline(pcp, no) == _TRUE)
        retcode = _TRUE;
    }
  }

  return(retcode);
}

_BOOL _Batch::WatchOnlineSub(_BOOL state)
{
  // state ist _TRUE wenn nur die offline-retry überprüft werden
  _UWORD   no, index = 0;
  _COMPORT *pcp;
  _BOOL    retcode = _FALSE;

  DoCheck = _FALSE;

  no  = IpcIni.ComSet.no;
  pcp = IpcIni.ComSet.ptr;

  while(no)
  {
    if((pcp->observe == _TRUE)&&(pcp->offline == _FALSE))
    {
      if((pcp->reflist.login == _LOGIN_TRUE)&&((state == _TRUE)||(pcp->offline_cnt != 0)))
      {
        // nur wenn login bereits ausgeführt und noch nicht offline
        if(IsOnline(pcp) == _FALSE)
        {
          pcp->offline = _TRUE; // offline aufgetreten
          retcode      = _TRUE;

         #ifndef CUTOFF_USER
          _FVOID_UWORD ptr;
          _UWORD       st, anz, stations[256];

          ptr = (_FVOID_UWORD)find_lasal_method(LseThisPointer, METHOD_LSE_OFFLINE);
          if(ptr != NULL)
          {
            anz = GetStationViaComportIndex(stations, index);
            while(anz)
            {
              anz -= 1;
              st = stations[anz];
              load_this(LseThisPointer);
              ptr(st);
            }
          }
         #endif
        }
      }
      else if(pcp->reflist.login == _LOGIN_FALSE)
      {
        pcp->offline = _TRUE;
        if(get_noexterncom() == _FALSE)
          retcode = _TRUE;
      }
    }

    index += 1;
    no    -= 1;
    pcp   += 1;
  }

  return(retcode);
}

void _Batch::WatchOnline(void)
{
  _BOOL    state = _FALSE;
  _UDWORD  diff  = timediff(WatchOnlineTimeDelay);

  if(diff > 3000)
  {
    WatchOnlineTimeDelay = get_abstime();
    state = WatchOnlineSub(_TRUE);
  }
  else if(DoCheck == _TRUE) // zb: Can-Retry
  {
    if(diff > 626) // beliebig gewählter wert
    {
      WatchOnlineTimeDelay = get_abstime();
      state = WatchOnlineSub(_FALSE);
    }
  }

  if(state == _TRUE)
  {
    SetLseServer(LSE_OFFLINEMODE, 1);
    Root.FindTop()->AddCmd(CMD_NEWWINDOW, UC_OFFLINEWINDOW, 0, _FALSE); // open offlinewindow
  }
}

_UBYTE *make_address(_COMPORT *cp, _INDEX *pi)
{
  return(((_UBYTE*)cp->reflist.refdat.data) + pi->offset);
}

_BOOL get_crc_from_plc(_COMPORT *cp)
{
  _UBYTE *ptr = NULL;

  if(cp->reflist.login == _LOGIN_TRUE)
  {
   #ifdef UC_EXOONLINE
    if(IsExoOnline(&cp->comdef))
    {
      cp->crc = ExoGetDescriptorCrc(&cp->comdef);
      return (cp->crc != DEFAULTCRC)? _TRUE : _FALSE;
    }
   #endif
    
    if(TxCommandIntern(I_GET_DESC_CRC, 0, NULL, &cp->comdef, (void**)&ptr, DEFSVRTYP) == DONE)
    {
      if(ptr != NULL)
      {
        ptr    += 2;
        cp->crc = *(_UDWORD*)ptr;
      }
    }
  }

  return((cp->crc != DEFAULTCRC)? _TRUE : _FALSE);
}

typedef struct { void *callbackfkt;
                 void *handle;
                 void *pthis;
               }
_SCALLBACK;

_BOOL do_login(_COMPORT *cp)
{
  _BOOL      retcode = _FALSE;
  _UDWORD    lov;
  _SCALLBACK cbp;

  cp->reflist.login = _LOGIN_FALSE;
  cp->offline       = _FALSE; // merker für offlinefenster, nicht auf _TRUE setzen !

//if(PingIp(cp) == _TRUE)
//{
  if(PlcLogin(cp) == 0)
  {
    cp->reflist.indexdat.no_max = 600;  // maximale anzahl der einträge in refreshliste
    cp->reflist.login = _LOGIN_TRUE;
    lov = get_loaderversion();

    if(lov >= 0x20001)
//    if(((lov & 0xFFFF) >= 1) && (((lov >> 16) & 0xFFFF) >= 2))
    {
      // neue callback-funktion, loader erkennt intern ob übergabepointer
      // auf daten oder code zeigt. in diesem fall daten (stack)
      
     #ifdef UC_EXOONLINE
      if(!IsExoOnline(&cp->comdef))
     #endif
      {
        cbp.callbackfkt = (void*)irq_PLC;
        cbp.handle      = (void*)cp->comdef.handle;
        cbp.pthis       = NULL;
        INSTALLCALLBACK((void*)&cbp);
      }
    }

    get_crc_from_plc(cp);
    retcode = _TRUE;
  }
  else
  {
    cp->comdef.handle = NULL;
  }
//}

  return(retcode);
}

_BOOL retry_login_with_timeout(_COMPORT *cp, _UWORD station)
{
  _UWORD  sb;
  _UDWORD begin, part, diff;
  _BOOL   retcode = _FALSE;
  _ASCII  txt[50];
  _UDWORD logintimeout = Batch.GetLoginDelayTime();

  if(logintimeout > 0)
  {
    begin = get_abstime();
    StrCpy(txt, sizeof(_ASCII), "Login Station ", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], station & 0xFF, 0x0800, sizeof(_ASCII));
   #ifdef UC_MULTICPU
    if(station & 0xFF00)
    {
      StrCat(txt, sizeof(_ASCII), "[", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], (station >> 8) & 0xFF, 0x0800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), "]", sizeof(_ASCII));
    }
   #endif
    StrCat(txt, sizeof(_ASCII), ":", sizeof(_ASCII));

    sb    = ScanBar.Add(txt, logintimeout);
    part  = 0;
    do
    {
      diff = timediff(begin);
      if((diff-part) > 20)
      {
        part = diff;
        if(diff < logintimeout)
          ScanBar.Run(sb, diff);
        if(do_login(cp) == _TRUE)
        {
          diff = logintimeout;
          retcode = _TRUE;
        }
      }
    }
    while(diff < logintimeout);

    ScanBar.End(sb);

    if(retcode == _FALSE)
    {
      if(do_login(cp) == _TRUE)
      {
        retcode = _TRUE;
      }
      else
      {
        StrCat(txt, sizeof(_ASCII), " FAILED !!", sizeof(_ASCII));
        FatalError(txt);
      }
    }
  }

  return(retcode);
}

_BOOL login_PLC_Sub(_COMPORT *cp, _UWORD station)
{
  if(cp->reflist.login == _LOGIN_INIT)
  {
    // bei externer station muss server _Lse::ExternCom abgefragt werden
    if((cp->comdef.typ == _COM_INTERN)||(get_noexterncom() == _FALSE))
    {
      if(do_login(cp) == _FALSE)
        retry_login_with_timeout(cp, station);
    }
    else
    {
      cp->reflist.login = _LOGIN_FALSE;
      cp->offline = _TRUE;
    }
  }

  return((cp->reflist.login == _LOGIN_TRUE)? _TRUE : _FALSE);
}

_COMDEF* login_PLC(_UWORD station)
{
  _COMPORT *cp;
  _COMDEF  *retcode = NULL;

  cp = Batch.Get_COMPORT(station);
  if(cp != NULL)
  {
    if(cp->reflist.login == _LOGIN_INIT)
    {
      login_PLC_Sub(cp, station);

     #ifdef UC_NEW_ALARMSYSTEM
      if(cp->reflist.login == _LOGIN_TRUE)
      {
        setupAlarmObject(cp);
      }
     #endif
    }
    retcode = (cp->reflist.login == _LOGIN_TRUE)? &cp->comdef : NULL;
  }

  return(retcode);
}

_BOOL GetAddressObj(_ASCII *label, _LASALID *p, _COMDEF *pcd, _LSEVARINFO *plvi)
{
  _UDWORD id = PlcGetLasalId(pcd, label, plvi);
  if(id != 0)
  {
    p->id    = id;
    p->vtype = (plvi->vartype == FSTRING)? TYP_EXTENDED : TYP_STANDARD;
    return _TRUE;
  }
  return _FALSE;
}

_BOOL GetAddressGlobVar(_ASCII *label, _LASALID *p, _COMDEF *pcd, _LSEVARINFO *plvi)
{
 #ifdef UC_EXOONLINE
  if(IsExoOnline(pcd))
  {
    _UDWORD id = ExoGetLasalId(&plvi->exoinfo, pcd, label);
    if(id != 0)
    {
      p->id    = id;
      p->vtype = TYP_STANDARD;
      return _TRUE;
    }
    return _FALSE;
  }
 #endif

//  if((get_loaderversion() & 0x0000FFFF) >= 0x00000039)  // 0x00220039
  {
    _UBYTE *result;

    if(TxCommandIntern(I_GET_GLOBAL_ADDR, StrLen(label, sizeof(_ASCII))+1, label, pcd, (void**)&result, DEFSVRTYP) == DONE)
    {
      result += 2;

      if(*(_UDWORD*)result != 0)
      {
        p->id = *(_UDWORD*)result;
        p->vtype = TYP_STANDARD;


       #ifdef UC_LINKVL
        _UBYTE no;
        if((plvi->memlinkvl.no > 0) && (plvi->memlinkvl.ptr != NULL))
        {
          no = plvi->memlinkvl.no - 1;
          if((plvi->memlinkvl.ptr[no].info == VL_STRUCTUREOFFSET) && (plvi->memlinkvl.ptr[no].link.no == 1) && (plvi->memlinkvl.ptr[no].link.info[0].state == CONST_VAL))
          {
            p->id += plvi->memlinkvl.ptr[no].link.info[0].value;
          }
        }
       #endif
        return _TRUE;
      }
    }
  }
 
  return _FALSE;
}

_BOOL GetAddressListObj(_ASCII *label, _UWORD len, _LASALID *p, _UWORD no, _COMDEF *pc)
{
  _BOOL    retcode = _FALSE;
  _UBYTE   *result = NULL;
  _UDWORD  id;
  _UWORD   chmode;
  _UWORD   length;

 #ifdef CANOPEN
  return _FALSE;
 #else
  if(TxCommandIntern(I_GET_OBJ_LIST, len, label, pc, (void**)&result, DEFSVRTYP) == DONE)
 #endif
  {
    length = *(_UWORD*)result;
    length -= 2; // incl. 2 byte length
    length /= 6; // 6byte pro server
    result += 2;

    if (length == no) // erwartete und bekommene anzahl ist unterschiedlich
    {
      retcode = _TRUE;
      
      while(length)
      {
        init_LASALID(p);
      
        id      = *(_UDWORD*)result;
        result += 4;
        chmode  = *(_UWORD*)result;
        result += 2;

        if(chmode < 3) // just server-channel is accessible
        {
          p->id    = id;
          p->vtype = TYP_STANDARD;
        }
//       #ifdef PGTEST
//        else if(chmode < 7)
//        {
//          Annahme("access to client denied:", 0); // just server-channels are accessible
//        }
//       #endif
     
        p ++;
        length --;
      }
    }
  }

  return(retcode);
}

_BOOL get_lasal_id(_LASALID *p, _ASCII *label, _LSEVARINFO *pi)
{
  _BOOL    retcode = _FALSE;
  _COMPORT *cp;

  init_LASALID(p);

  cp = Batch.Get_COMPORT(pi->station);
  if(cp != NULL)
  {
    if((pi->POWPRUIC & (VARBIT_VARGLOBAL | VARBIT_VARMEMBER)) == 0) // wenn keine membervariable & keine globale variable
    {
      if(GetAddressObj(label, p, &cp->comdef, pi) == _TRUE)
      {
        retcode = _TRUE;

       #ifdef OLDSTYLE_CAN
       #ifndef CANOPEN
        if(cp->comdef.typ != _COM_INTERN)
        {
          if(pi->memlinkvl.no > 0)
          {
            // p->id = 1; //pi->memlinkvl.ptr[0].link.info[0].value;
            // CAN_OPEN p->id = pi->memlinkvl.ptr[0].link.info[0].value;
          }
          else
          {
            retcode = _FALSE;
          }
        }
       #endif
       #endif
      }
    }
   #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
    else if(pi->POWPRUIC & VARBIT_VARGLOBAL) // wenn globale variable
    {
      if(GetAddressGlobVar(label, p, &cp->comdef, pi) == _TRUE)
      {
        retcode = _TRUE;
      }
    }
   #endif
  }

  return(retcode);
}

#if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
void delete_element_global(_ASCII *label)
{
  _UWORD len, i;
  _ASCII *chr;


  len = StrLen(label, sizeof(_ASCII));
  chr = label;

  for(i=0; i<len; i++)
  {
    if(*chr == '.')
    {
      *chr = 0;
      break;
    }

    chr++;
  }

}
#endif

#ifdef UC_MULTICPU
void _VarList::ChangeMultiID(_UWORD station, _UWORD substation)
{
  _UDWORD     i, anzahl = Data.MemLseVarInfo.no;
  _LSEVARINFO *p;

  if((station <= 255)&&(substation < UC_MULTICPU))
  {
    for(i=0; i<anzahl; i++)
    {
      if(GetVarInfo(&p, i) == _TRUE)
      {
        if(p->station == station)
          p->lasalid.id = p->multiid[substation];
      }
    }
  }
}
#endif

#if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
_BOOL calculate_global_id(_LASALID *pid, _LSEVARINFO *p, _LSEVARINFO *root)
{
  _BOOL retcode = _FALSE;
#ifdef UC_LINKVL
  _UBYTE no;

  if((root->memlinkvl.no > 0) && (root->memlinkvl.ptr != NULL))
  {
    no = root->memlinkvl.no - 1;
    if((root->memlinkvl.ptr[no].info == VL_STRUCTUREOFFSET) && (root->memlinkvl.ptr[no].link.no == 1) && (root->memlinkvl.ptr[no].link.info[0].state == CONST_VAL))
    {
      if(root->lasalid.id != DEFLASALID)
      {

        if((p->memlinkvl.no > 0) && (p->memlinkvl.ptr != NULL))
        {
          no = p->memlinkvl.no - 1;
          if((p->memlinkvl.ptr[no].info == VL_STRUCTUREOFFSET) && (p->memlinkvl.ptr[no].link.no == 1) && (p->memlinkvl.ptr[no].link.info[0].state == CONST_VAL))
          {
            pid->id = root->lasalid.id + p->memlinkvl.ptr[no].link.info[0].value;
           #ifdef UC_MULTICPU
            _UBYTE i;
            for(i=0; i<UC_MULTICPU; i++)
            {
              if(root->multiid[i] != DEFLASALID)
                p->multiid[i] = root->multiid[i] + p->memlinkvl.ptr[no].link.info[0].value;
            }
           #endif
            retcode = _TRUE;
          }
        }
      }
    }
  }

#endif

  return(retcode);
}
#endif

#if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
void calculate_all_global_ids(void)
{
  _UDWORD no, i;
  _LSEVARINFO *pvi, *root;
  _UBYTE varobj;
  _ASCII      label[MAX_LABELLEN];


  no = VarList.GetNo();

  for(i=0; i<no; i++)
  {
    if(VarList.GetVarInfo(&pvi, i) == _TRUE)
    {
      if(pvi->POWPRUIC & VARBIT_VARGLOBAL)
      {
        varobj = 0;

        if(VarList.GetLabel(label, &varobj, i) == _TRUE)
        {
          if(VarList.GetRootParent(&root, label, pvi->station) == _TRUE)
          {
            calculate_global_id(&pvi->lasalid, pvi, root);
          }
        }
      }
    }

  }

}
#endif

void _VarList::DoPermanentServer(_UDWORD anzahl)
{
  _UDWORD     i;
  _LSEVARINFO *p;

  // add entry to permanent list
  for(i=0; i<anzahl; i++)
  {
    if(GetVarInfo(&p, i) == _TRUE)
    {
      if(p->POWPRUIC & VARBIT_PERMANENT)
      {
        if(chk_LASALID(&p->lasalid) == _TRUE)
        {
          add_PLC(p, _TRUE, _TRUE);
        }
      }
    }
  }
}

#ifdef UC_MULTICPU
void _VarList::GetIDSub(_UWORD station, _UWORD substation, _UDWORD anzahl, _UWORD sb)
#else
void _VarList::GetIDSub(_UDWORD anzahl, _UWORD sb)
#endif
{
  _ASCII      label[MAX_LABELLEN];
  _LSEVARINFO *p, *pvi;
  _VARSYMBOL  *pvs;
  _UDWORD     i;
  _UBYTE      varobj;
  _LASALID    id;

  ScanBar.Reset(sb, anzahl, _FALSE);

  // get lasalid
  for(i=0; i<anzahl; i++)
  {
    ScanBar.Run(sb, i);

    if(GetVarInfo(&p, i) == _TRUE)
    {
     #ifdef UC_MULTICPU
      if((station == 0xFFFF)||(station == p->station))
     #endif
      {
        init_LASALID(&id);
        varobj       = 0;

        if((p->vartype != FCONSTANT)&&(p->vartype != FMERKER)&&(p->vartype != FENUMITEM))
        {
          if(login_PLC(p->station) != NULL)
          {
            if((p->POWPRUIC & VARBIT_VARMEMBER) == 0) // nur wenn keine membervariable
            {
              if(GetLabel(label, &varobj, i) == _TRUE)
              {
               #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
                if(p->POWPRUIC & VARBIT_VARGLOBAL)
                {
                  _LSEVARINFO *root;
                  if(GetRootParent(&root, label, p->station) == _TRUE)
                  {
                    if(calculate_global_id(&id, p, root) == _FALSE)
                      delete_element_global(label);
                  }
                  else
                  {
                    delete_element_global(label);
                  }
                }
                else
               #endif
                {
                  if(p->vartype == BIT)
                    delete_bit(label);

                 #ifdef UC_COMPLEX_SERVER
                  else if((p->POWPRUIC & VARBIT_STRUCTELEMENT) || (p->vartype == COMPLEX))
                    delete_element(label);
                 #endif

                }

               #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
                if(id.id == DEFLASALID)
               #endif
                  get_lasal_id(&id, label, p);
              }
            }
          } 
        }

       #ifdef UC_MULTICPU
        if(station == 0xFFFF)
        {
          p->lasalid    = id;
          p->multiid[0] = id.id;
        }
        else
        {
          p->multiid[substation] = id.id;
        }
       #else
        p->lasalid = id;
       #endif
      }
    }
  }
  
//  ExoFinalizeId();
  
  for(i=0; i<anzahl; i++)
  {
    if(GetVarInfo(&p, i) == _TRUE)
    {
     #ifdef UC_MULTICPU
      if((station == 0xFFFF)||(p->station == station))
     #endif
      {
        // id aller bitserver ermitteln wenn lasalid ungültig und scope auf
        // bit datentyp (zB.: BINT etc.) zeigt.
        if(p->vartype == BIT)                               // server buss BIT sein
        {
          if(chk_LASALID(&p->lasalid) == _FALSE)        // lasalid muss _false sein
          {
            if(GetVarSymbol(&pvs, i) == _TRUE)                   // scope ermitteln
            {
              if(GetVarInfo(&pvi, pvs->scope) == _TRUE)  // varinfo von scope holen
              {
                if(pvi->vartype == BINARY)               // vartyp muss binary sein
                {
                  if((pvi->format == SIZE_BDWORD)||(pvi->format == SIZE_BWORD)||(pvi->format == SIZE_BBYTE))
                  {
                    p->lasalid = pvi->lasalid;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

#ifdef UC_MEMBERVARIABLE


_UDWORD _VarList::GetMemberVariableAddress(_UDWORD varno)
{
  _LSEVARINFO *pvi;
  _VARSYMBOL  *pvs; 
  _UDWORD     tmpaddress;

  if((GetVarInfo(&pvi, varno) == _TRUE) && (GetVarSymbol(&pvs, varno) == _TRUE))
  {
    if(pvi->memberoffset < 0)
    {
      return pvi->lasalid.id;
    }
    
    tmpaddress = GetMemberVariableAddress(pvs->scope);
    if(tmpaddress != 0)
    {
      return GetMemberVariableAddress(pvs->scope) + pvi->memberoffset;
    }
  }
  
  return 0;
}

_DWORD _VarList::GetMemberOffsetFromParent(_UDWORD varno)
{
  _LSEVARINFO *pvi;
  _VARSYMBOL  *pvs;

  if(GetVarSymbol(&pvs, varno) == _TRUE)                   // scope ermitteln
  {
    if(GetVarInfo(&pvi, pvs->scope) == _TRUE)
    {
      return pvi->memberoffset;
    }
  }
  
  return -1;
}


_UDWORD _VarList::FindMemberVariables(void)
{
  _UDWORD     i, retcode = 0;
  _LSEVARINFO *pvi;
    
  for(i=0; i<Data.MemLseVarInfo.no; i++)
  {
    if(GetVarInfo(&pvi, i) == _TRUE)
    {
      if(pvi->vartype != FENUMITEM)
      {
        if(pvi->vartype == BIT)
        {
          if(GetMemberOffsetFromParent(i) >= 0)
          {
            pvi->memberoffset = 0; // memberoffset bei bit eintragen
          }
        }
        
        if((pvi->memberoffset >= 0) && ((pvi->POWPRUIC & VARBIT_VARGLOBAL) == 0)) // keine globale variable aber memberoffset vorhanden
        {
          pvi->POWPRUIC |= VARBIT_VARMEMBER;      // membervariable
          pvi->POWPRUIC &= ~VARBIT_STRUCTELEMENT; // bit löschen
          if(pvi->vartype != BIT)
          {
            pvi->reftime_bitno &= 0xE0;                  // den bitoffset löschen
          }
          retcode ++;
        }
      }
    }
  }

  MemberVariablesCount = retcode;
  
  return retcode;
}

_BOOL _VarList::DoMemberVariables(void)
{
  _UDWORD     i, anzahl = Data.MemLseVarInfo.no;
  _UWORD      sb = ScanBar.Add("Member ID:", anzahl);
  _LSEVARINFO *pvi;
    
  for(i=0; i<anzahl; i++)
  {
    if(GetVarInfo(&pvi, i) == _TRUE)
    {
      if((pvi->POWPRUIC & VARBIT_VARMEMBER) != 0) // es ist eine membervariable
      {
        pvi->lasalid.id    = GetMemberVariableAddress(i);
        pvi->lasalid.vtype = TYP_STANDARD;
      }
      ScanBar.Run(sb, i);
    }
  }
    
  ScanBar.End(sb);

  return _TRUE;
}




#endif //UC_MEMBERVARIABLE

/*
void CountLasalId(const char *dpne, _UDWORD lasalid, _UDWORD timex) // todo: just test
{
  _UDWORD retcode = 0;
//  _UDWORD crc = 0;
  _UDWORD anz = VarList.GetNo();
  char label[256];
  char line[512];
  _UDWORD lid_sh = 0;
  _UDWORD lid_ok = 0;
  
  _File hdl;
  hdl.Open(SAVE, dpne);
  
  lib_sprintf(label, "Duration=%ums", timex);
  hdl.Write(label, StrLen(label, 1));
  label[0] = 13;
  label[1] = 10;
  hdl.Write(label, 2);
  
  for(_UDWORD i=0; i<anz; i++)
  {
    _LSEVARINFO *pvi;
    if(VarList.GetVarInfo(&pvi, i) == _TRUE)
    {
//      if((pvi->lasalid.id == lasalid)) // && (pvi->vartype != FENUMITEM) && (pvi->vartype != FCONSTANT))
      {
        retcode ++;
//        _UDWORD tmp = (crc & 0x80000000)? 1 : 0;
//        crc = ((crc << 1) ^ i) | tmp;
 
        _UBYTE varobj;
//        DToA(label, i, 0x800, 1);
//        hdl.Write(label, StrLen(label, 1));
//        hdl.Write(" ", 1);
        
//        DToA(label, pvi->lasalid.vtype, 0x800, 1);
//        hdl.Write(label, StrLen(label, 1));
//        hdl.Write(" ", 1);
        
        if(VarList.GetLabel(label, &varobj, i) == _TRUE)
        {
          if(pvi->lasalid.id != 0)
          {
            lid_ok ++;
          }
          else
          {
            lid_sh ++;
          }
        
          lib_sprintf(line, "%u::%u 0x%08X %s %i", pvi->station, i, (pvi->lasalid.id != 0)? 0xCAFEBABE : 0, label, pvi->lasalid.vtype);
//          hdl.Write(line, StrLen(line, 1));
          
//          hdl.Write(label, StrLen(label, 1));
        }
        else
        {
          lib_sprintf(line, "ERROR: kein Label (%u::%u)", pvi->station, i);
        }
        
        hdl.Write(line, StrLen(line, 1));
        
        label[0] = 13;
        label[1] = 10;
        hdl.Write(label, 2);
        
      }
    }
  }
  
  lib_sprintf(line, "Used=%u, unused=%u", lid_ok, lid_sh);
  hdl.Write(line, StrLen(line, 1));
  label[0] = 13;
  label[1] = 10;
  hdl.Write(label, 2);
  
  hdl.Close();
}
*/

void _VarList::GetID(_BOOL bar)
{
  _UWORD  sb;
  _UDWORD anzahl = Data.MemLseVarInfo.no;
 #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
  _BOOL calculate = _FALSE;
 #endif

  sb = 0xFFFF;
  if(bar == _TRUE)
    sb = ScanBar.Add("Get ID:", anzahl);

 #ifdef UC_MEMBERVARIABLE
  _UDWORD membervars = FindMemberVariables();
 #endif // UC_MEMBERVARIABLE

 #ifndef OLDSTYLE_CAN
 #ifdef OK_CHECK_CRCOFID

  _UDWORD zi;
  for(zi=0; zi<256; zi++)
  {
    if(login_PLC(zi) != NULL)
    {
     #ifndef CUTOFF_USER
      _FVOID_UWORD fptr = (_FVOID_UWORD)find_lasal_method(LseThisPointer, METHOD_LSE_ONLINE);
      if(fptr != NULL)
      {
        load_this(LseThisPointer);
        fptr(zi);
      }
     #endif
    }
  }
  
 #ifdef UC_MULTICPU
  Batch.LoginAllMultiStations();
 #endif

 #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
  calculate = _TRUE;
 #endif

  if(LookForIdCrc() == _FALSE)
 #endif
 #endif
  {
    VarIdChanged = _TRUE;
   #ifdef UC_MULTICPU
    GetIDSub(0xFFFF, 0, anzahl, sb);
    Batch.GetMultiID(bar);
   #else

//_UDWORD tix = lib_tabsolute(); // todo: just test
   
    #ifdef UC_GETLASALID_DATALINK   
     GetLasalIdUsingDataLink(sb, bar);
    #endif
   
    if(GetIDFast(sb, bar) == _FALSE)
    {
      GetIDSub(anzahl, sb);
    }
    
    #ifdef UC_GETLASALID_DATALINK   
     FixLasalIdUsingDataLink();
    #endif
    
//tix = lib_tabsolute() - tix; // todo: just test
//CountLasalId("C:\\LidoOrigNew.txt", 0, tix); // todo: just test
   #endif

   #ifdef UC_MEMBERVARIABLE
    if(membervars > 0) // wenn membervariablen vorhanden
    {
      DoMemberVariables();
    }
   #endif

   #ifndef OLDSTYLE_CAN
   #ifdef OK_CHECK_CRCOFID
    SaveIdCrc();
   #endif
   #endif

   #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
    calculate = _FALSE;
   #endif
  }

  SendVisuReady();  

  DoPermanentServer(anzahl);

 #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
  if(calculate == _TRUE)
  {
    calculate_all_global_ids();
  }
 #endif

 #ifdef OLDSTYLE_CAN
 #ifndef CANOPEN
  VarList.CalculatePgAddresses();
 #endif
 #endif

//  VarList.MakeMapLasalID();

  ScanBar.End(sb);
}

void _VarList::ReGetID(_BOOL bar)
{
  _UWORD i, no;
  _COMPORT *pcp;

  no = Batch.GetNo_COMPORT();

  for(i=0; i<no; i++)
  {
    pcp = Batch.GetComportDirect(i);
    if(pcp != NULL)
    {
      if(pcp->reflist.login == _LOGIN_INIT)
        pcp->reflist.login = _LOGIN_FALSE;
    }
  }

  GetID(bar);
}
/*
_UWORD calcul_index(_REFLIST *p, _UWORD index, _BOOL state)
{
  _UWORD retcode = index;

  if(state == _TRUE)
  {
    if(index >= p->indexdat.permanent)
    {
      retcode = index + 1000 - p->indexdat.permanent;
    }
  }
  else
  {
    if(index >= p->indexdat.permanent)
    {
     #ifdef PGTEST
      if(index < 1000)
        Annahme("T_COM.CPP", __LINE__);
     #endif

      retcode = index - 1000 + p->indexdat.permanent;
    }
  }

  return(retcode);
}
*/

_UWORD calcul_index(_REFLIST *p, _UWORD index, _BOOL state)
{
  if(index >= p->indexdat.permanent)
  {
    if(state == _TRUE)
    {
      return (index + 1000 - p->indexdat.permanent);
    }
    
    return (index - 1000 + p->indexdat.permanent);
  }

  return index;
}

_INDEX* get_destination(_COMPORT *cp, _UWORD index)
{
  _REFLIST *pr;

  if(cp != NULL)
  {
    pr    = &cp->reflist;
    index = calcul_index(pr, index, _FALSE);

    if(index < pr->indexdat.used)
      return(&pr->indexdat.data[index]);
  }

  return(NULL);
}

_UWORD get_extendeddataof(_UBYTE *dst, _UWORD offset, _UDWORD lasalid, _COMDEF *cd, _UDWORD restbytelen)
{
  _UWORD retcode = 0;
  _UBYTE cmd[20];
  _UBYTE *result;
  _UWORD tmplen;
        
  *(_UDWORD*)&cmd[0] = lasalid;
  *(_UWORD*)&cmd[4]  = 3;
  *(_UBYTE*)&cmd[6]  = P_IMMED;      
  *(_UDWORD*)&cmd[7] = offset;
  *(_UBYTE*)&cmd[11] = P_EOL;
        
  if(TxCommandIntern(I_CMD, 12, cmd, cd, (void**)&result, DEFSVRTYP) == DONE) // length 10 !!
  {
    tmplen = *(_UWORD*)result;
    if(tmplen > 2)
    {
      tmplen -= 2;
      result += 2;

      if(tmplen > restbytelen)
      {
        tmplen = restbytelen;
      }
      
      if(tmplen > 250)
      {
        MemCpy(dst, result, 250);
        retcode = 250 + get_extendeddataof(dst+250, offset+250, lasalid, cd, restbytelen - 250); //tmplen);
      }
      else
      {
        MemCpy(dst, result, tmplen);
        retcode = tmplen;
      }
    }
  }
  
  return retcode;
}

_BOOL get_direct_PLC_sub(void *dest, _LASALID *pid, _COMPORT *cp, _UWORD size, _SVRTYP svrtyp)
{
  _BOOL   retcode = _FALSE;
  _UBYTE  tmp[MAX_COMDATA];
  _UBYTE  cmd[128];
  _UBYTE  *result;
  _UDWORD length;

  if(pid->vtype == TYP_STANDARD)  // standard data
  {
   #ifdef UC_EXOONLINE
    if(IsExoOnline(&cp->comdef))
    {
      return ExoGetValue(&cp->comdef, dest, pid->id, (_UDWORD)svrtyp, size);
    }
   #endif
  
    *(_UBYTE*)&cmd[0]  = P_VARIA;
    *(_UDWORD*)&cmd[1] = pid->id;
    *(_UBYTE*)&cmd[5]  = P_EOL;

    if(TxCommandIntern(I_READ, 8, cmd, &cp->comdef, (void**)&result, svrtyp) == DONE)
    {
      result += 2;
      MemCpy(dest, result, size);

      retcode = _TRUE;
    }
  }
  else // extended data
  {
  
   #ifdef UC_EXOONLINE
    if(IsExoOnline(&cp->comdef))
    {
      if(ExoGetString(&cp->comdef, (_ASCII*)tmp, pid->id, (_UDWORD)svrtyp, sizeof(tmp)) == _TRUE)
      {
         StrCpy(dest, sizeof(_CHAR), tmp, sizeof(_ASCII));
         return _TRUE;
      }
      return _FALSE;
    }
   #endif
  
    *(_UDWORD*)&cmd[0] = pid->id;
    *(_UWORD*)&cmd[4]  = 1;
    *(_UBYTE*)&cmd[6]  = P_EOL;

   #ifdef OLDSTYLE_CAN
    *(_UWORD*)&cmd[7]  = size + 2; //VarList.GetVariableSize(p->vartype, p->format);
   #endif

    if(TxCommandIntern(I_CMD, 9, cmd, &cp->comdef, (void**)&result, DEFSVRTYP) == DONE) // length 10 !!
    {
      result += 2;
      length = *(_UDWORD*)result;
      result += 4;
      // result zeigt auf daten (vorsicht string ohne final zero)
      // wenn length > 1 und die ersten 2 byte sind 0x00 und 0x02 dann ist
      // string im unicode format !!

      if(length > 250)
      {
        MemCpy(tmp, result, 250);
        length = 250 + get_extendeddataof(&tmp[250], 250, pid->id, &cp->comdef, sizeof(tmp)-252);
        result = tmp;
      }

     #ifdef UC_UNICODE
      if((length > 1)&&(result[0] == 0)&&(result[1] == 2))
      {
        result  += 2;
        length  -= 2;

        MemSet(dest, 0, size + sizeof(_CHAR));
        size = (length > size)? size : length;
        MemCpy(dest, result, size);
      }
      else
      {
        MemSet(dest, 0, size + sizeof(_CHAR));
        size = (length > (size/2))? (size/2) : length;
        if(result != &tmp[0]) // könnte sein wenn daten > 250 byte
          MemCpy(tmp, result, size);
        tmp[size] = 0;
        StrCpy(dest, sizeof(_UNI), tmp, sizeof(_ASCII));
      }
     #else
      MemSet(dest, 0, size + sizeof(_CHAR));
      size = (length > size)? size : length;
      MemCpy(dest, result, size);
     #endif
      retcode = _TRUE;
    }
  }

  return(retcode);
}

_BOOL get_direct_PLC(_LASALID *pid, _COMPORT *cp, _UWORD index, _SVRTYP svrtyp)
{
  _BOOL   retcode = _FALSE;
  _INDEX  *pi;
  _UBYTE  *dest;

  if(chk_LASALID(pid) == _TRUE)
  {
    pi = get_destination(cp, index);
    if(pi != NULL)
    {
      if(pid->vtype == TYP_STANDARD)  // standard data
      {
        retcode = get_direct_PLC_sub(make_address(cp, pi), pid, cp, pi->size, svrtyp);
      }
      else // extended data
      {
        dest    = make_address(cp, pi);
//+        retcode = get_direct_PLC_sub(dest+4, pid, cp, pi->size-4, svrtyp);
        retcode = get_direct_PLC_sub(dest+4, pid, cp, pi->size-4-sizeof(_CHAR), svrtyp);
      }

      if(retcode == _TRUE)
        pi->valid = 2;
    }
  }

  return(retcode);
}

_BOOL make_INDEXDAT(_INDEXDAT *p, _UWORD no)
{
  _BOOL   retcode = _TRUE;
  _UWORD  i;
  _UDWORD size;

  if(p->no < no)
  {
    if(no < MIN_INDEXDAT)
      no = MIN_INDEXDAT;

    size = no * sizeof(_INDEX);
    if(Memory2((void**)&p->data, size) != 0)
    {
      for(i=p->no; i<no; i++)
        init_INDEX(&p->data[i]);                   // initialize all new index

      p->no = no;                                           // set new account
    }
    else
    {
      retcode = _FALSE;
    }
  }

  return(retcode);
}

_BOOL make_REFDAT(_REFDAT *p, _UDWORD size)
{
  _BOOL retcode = _TRUE;

  if(p->size < size)
  {
    if(size < MIN_REFDAT)
      size = MIN_REFDAT;

    Mutex(_TRUE, MUTEX_ReallocREFLIST);
  
    if(Memory2((void**)&p->data, size) != 0)
    {
      MemSet(&p->data[p->size], 0, size-p->size);             // init new size
      p->size = size;                                       // set new account
    }
    else
    {
      retcode = _FALSE;
    }
  
    Mutex(_FALSE, MUTEX_ReallocREFLIST);
  }

  return(retcode);
}

_UWORD add_REFLIST(_COMPORT *cp, _LASALID *pid, _LSEVARINFO *plvi, _UWORD len, _BOOL permanent)
{
  _DWORD        offset, size;
  _UWORD        retcode = 0xFFFF;
//vo  _UDWORD            st;
  _REFLIST      *p;
  _COMDEF       *cd;
  _LSLCOMREGDAT reg;
  _UWORD        time    = get_reftime(plvi);
  _UWORD        station = plvi->station;
//vo  _UBYTE            varobj  = plvi->varobj;

  // retcode .... index to _REFLIST

  if(chk_LASALID(pid) == _TRUE)
  {
    p = &cp->reflist;

    // create memory for index
    if(make_INDEXDAT(&p->indexdat, p->indexdat.used + 1) == _TRUE)
    {
      cd   = &cp->comdef;
      size = len;

      // create memory for data
      if(make_REFDAT(&p->refdat, p->refdat.used + size) == _TRUE)
      {
        if(permanent == _TRUE)
        {
         #ifndef USER_KM
          if(p->permanent_enable == _FALSE)
            permanent = _FALSE;
         #endif
        }
        else
        {
          p->permanent_enable = _FALSE;
        }


        offset  = p->refdat.used;
        retcode = p->indexdat.used;                               // set index
//        p->indexdat.data[retcode].pdata   = &p->refdat.data[offset];
        _INDEX *pindex = &p->indexdat.data[retcode];
        pindex->offset  = offset;
        pindex->lasalid = *pid;
        pindex->ftime   = time;
        pindex->size    = len;

//        *(_UWORD*)&p->refdat.data[offset] = retcode;

        p->indexdat.used ++;
        p->refdat.used   += size;
        if(permanent == _TRUE)
        {
          p->indexdat.permanent = p->indexdat.used;
          p->refdat.permanent   = p->refdat.used;
        }

        reg.id        = pid->id;
        reg.channel   = (permanent == _TRUE)? 0 : 1;
        reg.xtime     = time;
        reg.varpos    = calcul_index(p, retcode, _TRUE);
        reg.varlistid = ((retcode << 8) & 0xFFFF0000) | station;

        if(plvi->POWPRUIC & (VARBIT_VARGLOBAL|VARBIT_VARMEMBER))
        {
          reg.xtime |= 0x4000;
        }
        else if(pid->vtype == TYP_EXTENDED)
        {
          // bei string muss msb bei time gesetzt werden, ansonsten kein irq
          reg.xtime |= 0x8000;
        }

        // attention: handling of intern variables is absolut fast
        //            irq (intern plc) occures immediately !!

       #ifdef UC_EXOONLINE
        if(IsExoOnline(cd))
        {
          p->indexdat.no_sent ++;
          ExoAddRefList(cd, &reg, plvi->exoinfo);
          ExoStartRefList(cd);
          return retcode;
        }
       #endif

       
       #ifdef CANOPEN
        TXUPD(&reg, cd, get_SVRTYP_from_LSEVARINFO(plvi));
       #else
        TXUPD(&reg, cd);
       #endif
        
        p->indexdat.no_sent ++;

        if(permanent == _TRUE)
        {
          Batch.StartRefreshList(cp, p->indexdat.permanent, 0);
        }
        else
        {
          Batch.StartRefreshList(cp, p->indexdat.no_sent - p->indexdat.permanent, 1);
        }
      }
    }
  }

  return(retcode);
}

_UWORD single_com_size(_LSEVARINFO *pi)
{
  _UWORD retcode;


  retcode = VarList.GetVariableSize(pi->vartype, pi->format);

  switch(pi->vartype)
  {
//    case COMPLEX  :
	 case FSTRING  :
		retcode += 4;
		retcode += sizeof(_CHAR);
		break;

	 default       :
		break;
  }

  if(retcode < 4)
	 retcode = 4;

  return(retcode);
}
/*
_BOOL update_STRING(_VARINFO *pi)
{
  _UWORD   index;
  _BOOL    retcode = _FALSE;
  _INDEX   *pind;
  _COMPORT *cp;

  if(pi->xadress.no > 0)
  {
	 if(pi->xadress.info[0].state == REFINDEX_VAL)
	 {
		index = pi->xadress.info[0].value;
		cp    = Batch.Get_COMPORT(pi->station);
		index = calcul_index(&cp->reflist, index, _TRUE);
		pind  = get_destination(cp, index);
		if(pind != NULL)
		{
        retcode = _TRUE;
        if(pind->valid == 1)
          retcode = update_PLC(pi);
      }
    }
  }

  return(retcode);
}
*/

_BOOL update_PLC(_VARINFO *pvi)
{
  _UWORD   index;
  _COMPORT *cp;
  _BOOL    retcode = _FALSE;

  if(pvi->xadress.no > 0)
  {
    if(pvi->xadress.info[0].state == REFINDEX_VAL)
    {
      index   = pvi->xadress.info[0].value;
      cp      = Batch.Get_COMPORT(pvi->station);
      index   = calcul_index(&cp->reflist, index, _TRUE);
     #ifdef CANOPEN
      retcode = get_direct_PLC(&pvi->lasalid, cp, index, get_SVRTYP_from_VARINFO(pvi));
     #else
      #ifdef UC_EXOONLINE
       retcode = get_direct_PLC(&pvi->lasalid, cp, index, (_SVRTYP)pvi->exoinfo);
      #else
       retcode = get_direct_PLC(&pvi->lasalid, cp, index, DEFSVRTYP);
      #endif
     #endif
    }
  }

  return(retcode);
}

void new_REFLIST(_COMPORT *pc)
{
  _UWORD  i;
  _UDWORD size;

  if(pc != NULL)
  {
    if(pc->comdef.handle != NULL)
    {
     #ifdef UC_EXOONLINE
      if(IsExoOnline(&pc->comdef))
      {
        ExoClearRefList(&pc->comdef);
      }
      else
     #endif
      {
        StartStopRefresh(&pc->comdef, 0, 1);
      }
      
      for(i=pc->reflist.indexdat.permanent; i<pc->reflist.indexdat.used; i++)
      {
        init_INDEX(&pc->reflist.indexdat.data[i]);
      }

      pc->reflist.indexdat.used    = pc->reflist.indexdat.permanent;
      pc->reflist.indexdat.no_sent = pc->reflist.indexdat.permanent;

      if(pc->reflist.refdat.data != NULL)
      {
        size = pc->reflist.refdat.used - pc->reflist.refdat.permanent;
        MemSet(&pc->reflist.refdat.data[pc->reflist.refdat.permanent], 0, size);
      }

      pc->reflist.refdat.used = pc->reflist.refdat.permanent;
    }
  }
}

#ifdef UC_DELETE_REFLIST
void chk_refreshbuffer(_COMPORT *cp, _UWORD station)
{
  _UBYTE stno[256];
  _UWORD no;

  if((cp->reflist.indexdat.no_sent - cp->reflist.indexdat.permanent) >= cp->reflist.indexdat.no_max)
  {
    if(Root.FindTop()->SystemError(REINITIALIZE_REFRESHLIST, station, NULL) == _TRUE)
    {
      no = Batch.GetAllStationNumbersToSameComport(stno, station);
      while(no)
      {
        no -= 1;
        VarList.InitAdressesOnStationNumber(stno[no]);
      }
      new_REFLIST(cp);
    }
  }
}

void DeleteRefreshlistOnRequest(_BOOL st)
{
  ChkForDeleteRefreshlist = st;
}

#endif

_UWORD add_PLC(_LSEVARINFO *plvi, _BOOL permanent, _BOOL wait4plc)
{
  _UWORD   index = 0xFFFF;
  _COMPORT *cp;
  _UWORD   len;
  _LASALID *pid;

  cp = Batch.Get_COMPORT(plvi->station);
  if(cp != NULL)
  {
   #ifdef UC_DELETE_REFLIST
    if(ChkForDeleteRefreshlist == _TRUE)
      chk_refreshbuffer(cp, plvi->station);
   #endif

    pid   = &plvi->lasalid;
    len   = single_com_size(plvi);
    index = add_REFLIST(cp, pid, plvi, len, permanent);
//    index = add_REFLIST(cp, pid, plvi->reftime, len, plvi->varobj, permanent, plvi->station);

    if(index != 0xFFFF)
    {
      plvi->iaddress = index;
      if(wait4plc == _TRUE)
      {
        index = calcul_index(&cp->reflist, index, _TRUE);
       #ifdef CANOPEN
        get_direct_PLC(pid, cp, index, get_SVRTYP_from_LSEVARINFO(plvi));
       #else
        #ifdef UC_EXOONLINE
         get_direct_PLC(pid, cp, index, (_SVRTYP)plvi->exoinfo);
        #else
         get_direct_PLC(pid, cp, index, DEFSVRTYP);
        #endif
       #endif
      }
    }
  }

  return(index);
}

void _VarList::InitAdresses(void)
{
  _LSEVARINFO *p;
  _UDWORD     i;

  p = Data.MemLseVarInfo.ptr;
  i = Data.MemLseVarInfo.no;
  while(i)
  {
    if(p->iaddress != DEF_IADDRESS) // geschwindigkeitsoptimierung !!
    {
      if(!(p->POWPRUIC & VARBIT_PERMANENT)) // nur wenn permanent-flag = false
        p->iaddress = DEF_IADDRESS;
    }

    i -= 1;
    p += 1;
  }
}

#ifdef UC_DELETE_REFLIST
void _VarList::InitAdressesOnStationNumber(_UWORD station)
{
  _LSEVARINFO *p;
  _UDWORD     i;

  for(i=0; i<Data.MemLseVarInfo.no; i++)
  {
    if(GetVarInfo(&p, i) == _TRUE)
    {
      if(p->station == station)
      {
        if((p->POWPRUIC & VARBIT_PERMANENT) == 0)
        {
          p->iaddress = DEF_IADDRESS;
        }
      }
    }
  }
}
#endif

void new_PLC(void)
{
  _UWORD   i, no;
  _COMPORT *cp;

  VarList.InitAdresses();
  no = Batch.GetNo_COMPORT();

  for(i=0; i<no; i++)
  {
    cp = Batch.GetComportDirect(i);
    if(cp != NULL)
      new_REFLIST(cp);
  }
}

void *get_PLC(_VARINFO *pvi, _UWORD index)
{
  // --> stationnumber
  // --> index to REFLIST
  // <-- pointer to data (refreshed by plc)
  void *retcode = NULL;
  _COMPORT *cp;
  _REFLIST *p;
  _INDEX   *pi;

  cp = Batch.Get_COMPORT(pvi->station);
  if(cp != NULL)
  {
    p = &cp->reflist;

    if(index < p->indexdat.used)
    {
      pi = &p->indexdat.data[index];
      if(pi->valid != 0)
      {
        retcode = make_address(cp, pi);

        if(pvi->lasalid.vtype == TYP_EXTENDED)
        {
          retcode = ((_UBYTE*)retcode) + 4;
          if(pi->valid == 1)
          {
            index = calcul_index(&cp->reflist, index, _TRUE);
           #ifdef CANOPEN
            if(get_direct_PLC(&pvi->lasalid, cp, index, get_SVRTYP_from_VARINFO(pvi)) == _FALSE)
           #else
            #ifdef UC_EXOONLINE
            if(get_direct_PLC(&pvi->lasalid, cp, index, (_SVRTYP)pvi->exoinfo) == _FALSE)
            #else
            if(get_direct_PLC(&pvi->lasalid, cp, index, DEFSVRTYP) == _FALSE)
            #endif
           #endif
            {
              retcode = NULL;
            }
          }
        }

        if(retcode != NULL)
          pi->valid = 2;
      }
    }
  }

  return(retcode);
}

_BOOL wait_for_PLC(_UWORD index, _VARINFO *pvi, _COMPORT *cp)
{
  _BOOL     retcode = _FALSE;
//  _COMPORT  *cp;
  _INDEXDAT *pi;
  _UDWORD tmpindex;


//  cp = Batch.Get_COMPORT(pvi->station);
  if(cp != NULL)
  {
    pi = &cp->reflist.indexdat;

    tmpindex = calcul_index(&cp->reflist, index, _FALSE);

    if(tmpindex < pi->no)
    {
      retcode = _TRUE;
      if(pi->data[index].valid == 0)
      {
       #ifdef CANOPEN
        retcode = get_direct_PLC(&pi->data[tmpindex].lasalid, cp, index, get_SVRTYP_from_VARINFO(pvi));
       #else
        #ifdef UC_EXOONLINE
         retcode = get_direct_PLC(&pi->data[tmpindex].lasalid, cp, index, (_SVRTYP)pvi->exoinfo);
        #else
         retcode = get_direct_PLC(&pi->data[tmpindex].lasalid, cp, index, DEFSVRTYP);
        #endif
       #endif
      }
    }
  }

  return(retcode);
}

extern _BOOL wait_for_PLC_global(_UWORD index, _VARINFO *pvi, _VARLABEL *pvl)
{
  _BOOL retcode = _FALSE;
  _COMPORT *pcp;
  _INDEXDAT *pid;
  _INDEX *pi;
  _UBYTE *dest;
  _UWORD  tmpindex;


  pcp = Batch.Get_COMPORT(pvi->station);
  if(pcp != NULL)
  {
    pid = &pcp->reflist.indexdat;
    tmpindex = calcul_index(&pcp->reflist, index, _FALSE);

    if(tmpindex < pid->no)
    {
      if(pid->data[tmpindex].valid == 0)
      {
        pi = get_destination(pcp, index);
        if(pi != NULL)
        {
          if(pid->data[tmpindex].lasalid.vtype == TYP_STANDARD)
          {
            dest = make_address(pcp, pi);

            if(get_global_data(dest, pvi) == _TRUE)
//            if(xget_dat(&result, pvl->info[0].value, NOCARE_DEFAULT) == ERROR_CAUSED_BY_NO_ERROR)
            {
//              *dest = result.value;
              pid->data[tmpindex].valid = 2;
              retcode = _TRUE;
            }
          }
        }
      }
    }
  }


  return(retcode);
}

_BOOL send_PLC_string(_UBYTE *dat, _UBYTE *p, _UWORD len, _VARINFO *pvi, _COMDEF *cd)
{
  _UDWORD size, part;
  _UDWORD offset = 0;
  _UBYTE  *ps, *presult = NULL;
  _UBYTE  sizeofchar;
 #ifdef UC_UNICODE
  _UBYTE  tmp[800];
 #endif

  size = StrLen((_CHAR*)p, sizeof(_CHAR));
  size = (size > len)? len : size;
  size++;
//  size = (size + 1) * sizeof(_CHAR);

 #ifdef UC_UNICODE
  if(VarList.GetOnlyAscii(pvi) == _TRUE) // Unicode String soll als Ascii String geschrieben werden
  {
    sizeofchar = sizeof(_ASCII);
    StrCpy(tmp, sizeof(_ASCII), p, sizeof(_UNI));
  }
  else
  {
    sizeofchar = sizeof(_UNI);
    size *= sizeof(_UNI);
    MemCpy(&tmp[0], p, size);
  }
  p = tmp;
 #else
  sizeofchar = sizeof(_CHAR);
 #endif

  do
  {
    *(_UDWORD*)&dat[0]  = pvi->lasalid.id;
    *(_UWORD*)&dat[4]   = 4;
    *(_UBYTE*)&dat[6]   = P_USER_STREAM;
    part                = (size > 50)? 50 : size;
    *(_UDWORD*)&dat[7]  = part;
    *(_UDWORD*)&dat[11] = offset;

   #ifdef UC_UNICODE
    if((offset > 0)&&(sizeofchar > 1))
    {
      // bei unicode beginnt der eigentliche string auf startadresse + 2 byte.
      // deshalb muss beim anhängen des weiteren stringteils dies berücksichtigt werden.
      // -- tribut an erfinder der klassen String und StringRAM --
      *(_UDWORD*)&dat[11] += 2;
    }
   #endif

    ps = &dat[15];
    if(offset == 0)
    {
      *(_UDWORD*)&dat[7] += 2;
      *(_UBYTE*)&dat[15]  = 0;
      *(_UBYTE*)&dat[16]  = sizeofchar;
      ps = &dat[17];
    }

    MemCpy(ps, &p[offset], part);
    ps[part] = P_EOL;

    if(TxCommandIntern(I_CMD, (*(_UDWORD*)&dat[7])+16, dat, cd, (void**)&presult, DEFSVRTYP) != DONE)
      return(_FALSE);

    offset += part;
    size   -= part;
  }
  while(size);

  return(_TRUE);
}

_BOOL send_PLC(void *pdata, _VARINFO *pvi)
{
  _BOOL    retcode = _FALSE;
  _UWORD   len;
  _UDWORD  bit, value;
  _UBYTE   dat[400];
  _UBYTE   *presult = NULL;
  _UDWORD  result;
  _COMPORT *cp;


  if(chk_LASALID(&pvi->lasalid) == _TRUE)
  {
    cp = Batch.Get_COMPORT(pvi->station);
  	if(cp != NULL)
    {
      len = VarList.GetVariableSize(pvi->vartype, pvi->format);

     #ifdef UC_EXOONLINE
      if(IsExoOnline(&cp->comdef))
      {
        if(pvi->vartype == FSTRING)
          return ExoSetString(&cp->comdef, (_ASCII*)pdata, pvi->lasalid.id, pvi->exoinfo, len);
          
        return ExoSetValue(&cp->comdef, pdata, pvi->lasalid.id, pvi->exoinfo, len);
      }
     #endif

		  //[4] ... id
		  //[1] ... prfix P_IMMED
		  //[4] ... value
		  //[1] ... prefix P_EOL

		  *(_UDWORD*)dat = pvi->lasalid.id;
  		dat[4] = P_IMMED;
	  	dat[9] = P_EOL;

      switch(pvi->vartype)
      {
        //case RADIO   :
        case BIT:
          value = 0;
          MemCpy(&value, pdata, len);
          bit  = (1 << pvi->bitno);
          bit |= 0x80000000;
          if(value == 0)
          bit = bit & 0x7FFFFFFF;
          *(_UDWORD*)&dat[5] = bit;
        break;

        case FSTRING:
          return(send_PLC_string((_UBYTE*)dat, (_UBYTE*)pdata, len, pvi, &cp->comdef));

        default:
         #ifdef UC_COMPLEX_SERVER
          _UDWORD  tmp;
          _UDWORD  data;
          if(pvi->POWPRUIC & VARBIT_STRUCTELEMENT)
          {
           #ifdef UC_EXOONLINE
            if(get_direct_PLC_sub(&data, &pvi->lasalid, cp, sizeof(_IVALUE), (_SVRTYP)pvi->exoinfo) == _TRUE)
           #else
            if(get_direct_PLC_sub(&data, &pvi->lasalid, cp, sizeof(_IVALUE), DEFSVRTYP) == _TRUE)
           #endif
            {
              switch(VarList.GetVariableSize(pvi->vartype, pvi->format))
              {
                case 1: tmp = 0xFF; break;
                case 2: tmp = 0xFFFF; break;
                case 4: tmp = 0xFFFFFFFF; break;
              }

              data &= ((tmp << (pvi->bitno*8)) ^ 0xFFFFFFFF);
              *(_IVALUE*)pdata  &= tmp;
              data |= *(_IVALUE*)pdata << (pvi->bitno*8);

              MemCpy(&dat[5], &data, sizeof(_IVALUE));
            }
          }
          else
         #endif

          {
           #ifdef OLDSTYLE_CAN
            if(cp->comdef.typ == _COM_INTERN)
           #endif
            if(pvi->vartype == BINARY)
            {
            // BDINT, BINT und BSINT wird auf 32/16/8 einzelzugriffe aufgeteilt (melkus 09.05.03)
              switch(pvi->format)
              {
                case SIZE_BBYTE  : return(send_bdint(*(_UBYTE*)pdata,  pvi));
                case SIZE_BWORD  : return(send_bdint(*(_UWORD*)pdata,  pvi));
                case SIZE_BDWORD : return(send_bdint(*(_UDWORD*)pdata, pvi));
              }
            }

            MemCpy(&dat[5], pdata, len);
            break;
          }

      }

     #ifdef OLDSTYLE_CAN
      dat[10] = VarList.GetVariableSize(pvi->vartype, pvi->format);
     #endif

     #ifdef CANOPEN
      if(TxCommandIntern(I_WRITE, 10, dat, &cp->comdef, (void**)&presult, get_SVRTYP_from_VARINFO(pvi)) == DONE)
     #else
      if(TxCommandIntern(I_WRITE, 10, dat, &cp->comdef, (void**)&presult, DEFSVRTYP) == DONE)
     #endif
      {
        retcode = _TRUE;
        if(presult != NULL)
        {
          presult += 2;
          result = *(_UDWORD*)presult;
          if(result == ACCESS_DENIED) //||(result == 604117360L))
          {
            update_PLC(pvi);
			      retcode = _FALSE;
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL send_bdint(_UDWORD value, _VARINFO *p)
{
  _UWORD   no;
  _VARINFO tmp;
  _BOOL    retcode = _FALSE;
  _UDWORD  data;

  no  = 0;
  tmp = *p;

  switch(tmp.format)
  {
    case SIZE_BBYTE  : no =  8; tmp.format = SIZE_USINT; break;
    case SIZE_BWORD  : no = 16; tmp.format = SIZE_UINT;  break;
    case SIZE_BDWORD : no = 32; tmp.format = SIZE_UDINT; break;
  }

  if(no)
  {
    retcode     = _TRUE;
    tmp.vartype = BIT;   // bit datentype
    for(tmp.bitno=0; tmp.bitno<no; tmp.bitno++)
    {
      data = (value >> tmp.bitno) & 1;
      if(send_PLC(&data, &tmp) == _FALSE)
        retcode = _FALSE;
    }
  }

  return(retcode);
}

extern "C" void irq_PLC(void *p0, _UBYTE *pd)
{
  // if a value has changed this function is called by irq
  // cd ... --> to information who sent data
  // pd ... --> to data (2 byte position in reflist + data)

  // pd ... 4 byte varlistid, 2 byte offset, data

  // valid == 0 .... ungültig
  // valid == 1 .... neu gesendet
  // valid == 2 .... bereits von kernel aus combuffer ausgelesen

//  _UWORD   size;
  _INDEX   *pi;
  _COMDEF  *cd = (_COMDEF*)p0;
  _COMPORT *cp;

//  cp  = Batch.Get_COMPORT(*(_UWORD*)pd);
//  pd += 2;
//  if(cp != NULL)
//  {
//    pi   = &cp->reflist.indexdat.data[*(_UWORD*)pd];
//    size = (pi->size <= 4)? pi->size : 4;
//    MemCpy(pi->pdata, pd+2+2, size);
//    pi->valid = 1;                                     // mark as valid data
//  }

  Mutex(_TRUE, MUTEX_ReallocREFLIST);

  pd = pd + 4;
  cp = Batch.Seek_COMPORT(cd);                        // find index to reflist
  pi = get_destination(cp, *(_UWORD*)pd);
  if(pi != NULL)
  {
    void *dst = make_address(cp, pi);
    void *src = pd+2;
  
    switch(pi->size)
    {
      case  0: break;
      case  1: *(_UBYTE*)dst = *(_UBYTE*)src;
               break;
      case  2: *(_UWORD*)dst = *(_UWORD*)src;
               break;
      case  3: break;
      default: *(_UDWORD*)dst = *(_UDWORD*)src;
               break;
    }
  
//    _UWORD size = (pi->size <= 4)? pi->size : 4;
//    MemCpy(make_address(cp, pi), pd+2, size);
    
    pi->valid = 1;                                     // mark as valid data

    GlobalChangeIrq ++;
  }
  
  Mutex(_FALSE, MUTEX_ReallocREFLIST);
}

// ***************************************************************************
// ***************************************************************************
// ***************************************************************************

#ifdef PLC_SIMULATION
void simulate_PLC(void)
{
  _UWORD    i, j;
  _REFLIST  *pr;
  _INDEXDAT *pi;
  _INDEX    *pd;
  _UBYTE    *pb;
  _UWORD    *pw;
  _UDWORD   *pu;

  for(i=0; i<NO_COMSTATION; i++)
  {
    pr = &communication.reflist[i];
    if(pr->login == _LOGIN_TRUE)
    {
      pi = &pr->indexdat;
      for(j=0; j<pi->used; j++)
      {
        pd = &pi->data[j];

        switch(pd->size)
        {
          case 0 : break;
          case 2 : pw = (_UWORD*)pd->pdata;  pw[0] ++; pd->valid = 1; break;
          case 4 : pu = (_UDWORD*)pd->pdata; pu[0] ++; pd->valid = 1; break;
          default: pb = (_UBYTE*)pd->pdata;  pb[0] ++; pd->valid = 1; break;
        }
      }
    }
  }
}
#endif

/*
typedef struct { _UBYTE opChg;
                 _UBYTE mode;
                 _UBYTE alano;
               }
_ALACONF;
*/

_BOOL setup_SINGLE_ALARM(_SINGLE_ALARM *pa, _UDWORD config, _UWORD no, _UDWORD *ppara, _UWORD station)
{
  _BOOL     retcode = _FALSE;
  _DWORD    size;
  _UWORD    i;
  _UDWORD   varno;
  _VARLABEL *pvl;

  size  = no * sizeof(_VARLABEL);

  if(Memory2((void**)&pa->parameter.ptr, size) != 0)
  {
   #ifdef UC_NEW_ALARMSYSTEM
    pa->alano        = (_UWORD)(config & 0xFFFF);
    pa->mode         = (_UWORD)((config >> 16) & 0xFFFF);
   #else
    pa->alano        = (_UWORD)((config >> 16) & 0xFFFF);
    pa->mode         = (_UWORD)(config & 0xFFFF);
   #endif
    pa->parameter.no = no;

    for(i=0; i<no; i++)
    {
      pvl = &pa->parameter.ptr[i];
      init_VARLABEL(pvl);
      varno   = VarList.GetVarNoByID(ppara[i], station, _TRUE);
      pvl->no = 1;
      if(varno < VarList.GetNo())
      {
        pvl->info[0].state = PARA_VAL; // VAR_VAL;
        pvl->info[0].value = varno;
      }
      else
      {
        pvl->info[0].state = PARA_CONST; // VAR_VAL;
        pvl->info[0].value = DEFSCOPE;
      }
    }

    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL config_SINGLE_ALARM(_SINGLE_ALARM *pa, _BOOL reconfig)
{
  // result TXCOMMAND
  // [][] ....... length of result (incl. length)
  // [] ......... opChg
  // [] ......... mode
  // [][] ....... alano
  // [][][][] ... parameter 1 (lasalid) depends on length
  // [][][][] ... parameter 2 (lasalid) depends on length
  // [][][][] ... parameter 3 (lasalid) depends on length
  // [][][][] ... parameter 4 (lasalid) depends on length

  // übergabe TXCOMMAND
  // [][][][] ... id
  // [][] ....... commando to class 0 (constatnt)
  // [] ......... prefix (eol)

  _UBYTE      tmp[20];
  _UBYTE      *presult;
  _UWORD      len, no;
  _BOOL       retcode = _FALSE;
  _LSEVARINFO *pi;
  _UWORD      station;
  _COMPORT    *cp;


  if(pa->link.no > 0)
  {
    if((pa->link.info[0].state == VAR_VAL)&&(VarList.GetVarInfo(&pi, pa->link.info[0].value) == _TRUE))
    {
      if(pi->lasalid.id != 0)
      {
        station            = pi->station;
        cp = Batch.Get_COMPORT(station);
        if(cp != NULL)
        {
         #ifdef UC_NEW_ALARMSYSTEM
          if(cp->alarmObject != 0)
          {
            *(_UDWORD*)&tmp[0]  = cp->alarmObject;
            *(_UWORD*) &tmp[4]  = (reconfig == _FALSE)? COMMAND_GETALARMCONFIG : COMMAND_GETALARMCONFIGUNREG;
            *(_UBYTE*) &tmp[6]  = P_IMMED;
            *(_UDWORD*)&tmp[7]  = pi->lasalid.id;
            *(_UBYTE*) &tmp[11] = P_EOL;
            len = 12;
            
            if(TxCommandIntern(I_CMD, len, tmp, &cp->comdef, (void**)&presult, DEFSVRTYP) == DONE)
            {
              if(presult != NULL)
              {
                len = *(_UWORD*)&presult[0];
                if(len > 6)
                {
                  retcode = _TRUE;
                  // Debug(*(_UDWORD*)&presult[2],  1); // config (mode + alano)
                  // Debug(*(_UBYTE*)&presult[6],   2); // anzahl parameter
                  // Debug(*(_UDWORD*)&presult[7],  3); // parameter 1
                  // Debug(*(_UDWORD*)&presult[11], 4); // parameter 2
                  // Debug(*(_UDWORD*)&presult[15], 5); // parameter 3
              
                  no = *(_UBYTE*)&presult[6];
                  setup_SINGLE_ALARM(pa, *(_UDWORD*)&presult[2], no, (_UDWORD*)&presult[7], station);
                }
              }
            }
          }
         #else
          {
            *(_UDWORD*)&tmp[0] = pi->lasalid.id;
            *(_UWORD*)&tmp[4]  = 0;
            tmp[6]             = P_EOL;
            len = 7;

            if(TxCommandIntern(I_CMD, len, tmp, &cp->comdef, (void**)&presult, DEFSVRTYP) == DONE)
            {
              if(presult != NULL)
              {
                retcode = _TRUE;
                len     = *(_UWORD*)&presult[0];    // size of returnstream

                if(len >= 6)
                {
                  no = (len - 6) / 4;            // no of connected parameter
                  setup_SINGLE_ALARM(pa, *(_UDWORD*)&presult[2], no, (_UDWORD*)&presult[6], station);
                }
              }
            }
          }
         #endif
        }
      }
    }
  }

  return(retcode);
}

_BOOL send_command0(_UDWORD objno, _UWORD command, _UWORD no, _DWORD *para, void **result)
{
  _UDWORD     i, size;
  _COMDEF     *cd;
  _LSEVARINFO *pvi;
  _VARSYMBOL  *pvs;
  _UBYTE      tmp[130], *ptr;
  _BOOL       retcode = _FALSE;
  _BOOL       run;
  _UBYTE      *tmpresult = NULL;
//  _UWORD     scope;

  do
  {
    run = _FALSE;
    if(VarList.GetVarInfo(&pvi, objno) == _TRUE)
    {
      if(pvi->vartype != OBJECT)
      {
        VarList.GetVarSymbol(&pvs, objno);
        objno = pvs->scope;
        run   = _TRUE;
      }
    }
  }
  while(run == _TRUE);

//  if(VarList.GetVarSymbol(&pvs, objno) == _TRUE)
//  {
//    scope = pvs->scope;
//    while(scope != DEFSCOPE)
//    {
//      objno = pvs->scope;
//      if(VarList.GetVarSymbol(&pvs, objno) == _FALSE)
//        scope = DEFSCOPE;
//    }
//  }

  if(VarList.GetVarInfo(&pvi, objno) == _TRUE)
  {
    cd = login_PLC(pvi->station);
    if(cd != NULL)
    {
      if((pvi->vartype == OBJECT)&&(chk_LASALID(&pvi->lasalid) == _TRUE))
      {
        ptr  = &tmp[0];
        size = 0;

        *(_UDWORD*)ptr = pvi->lasalid.id;  ptr += 4; size += 4;
        *(_UWORD*)ptr  = command;          ptr += 2; size += 2;

        if((no > 0)&&(para != NULL))
        {
          for(i=0; i<no; i++)
          {
            *(_UBYTE*)ptr  = P_IMMED;  ptr += 1; size += 1;
            *(_UDWORD*)ptr = para[i];  ptr += 4; size += 4;
            *(_UBYTE*)ptr  = P_COMMA;  ptr += 1; size += 1;
          }
          ptr -= 1;      // no comma at last parameter
        }

        *(_UBYTE*)ptr = P_EOL; size += 1;

        if(TxCommandIntern(I_CMD, size+3, tmp, cd, (void**)&tmpresult, DEFSVRTYP) == DONE)
          retcode = _TRUE;
      }
    }
  }

  if(result != NULL)
    *result = tmpresult;

  return(retcode);
}

_UDWORD send_command1_sub(_ASCII *object, _UWORD station)
{
  _LSEVARINFO *pvi;
  _UDWORD     retcode = DEFSCOPE;
  _UDWORD     objno   = VarList.GetVarNo(object, station);

  if(VarList.GetVarInfo(&pvi, objno) == _TRUE)
  {
    if(pvi->station == station)
    {
      retcode = objno;
    }
  }

  return(retcode);
}

_BOOL send_command1(_ASCII *object, _UWORD station, _UWORD command, _UWORD no, _DWORD *para)
{
  return(send_command3(object, station, command, no, para, NULL));
}

_BOOL send_command2(_ASCII *object, _UWORD station, _UWORD command, _UWORD len, _UBYTE *src, _UDWORD timeout)
{
  _UDWORD objno;
  _UBYTE  data[200];
  _UBYTE  part;
  _UWORD  run, offset = 0;
  _UDWORD time;

  // format
  // [2]... offset
  // [1]... anzahl der nutzdaten daten
  // [1]... <> 0 ... letztes paket
  // [n]... nutzdaten

  if(len > 0)
  {
    objno = send_command1_sub(object, station);

    if(objno != DEFSCOPE)
    {
      while(len)
      {
        part = (len < 40)? len : 40;
        len -= part;
        *(_UWORD*)&data[0] = offset;
        *(_UBYTE*)&data[2] = part;
        *(_UBYTE*)&data[3] = (len == 0)? 1 : 0;
        MemCpy(&data[4], src, part);
        src += part;

        time = get_abstime();
        run  = 1;
        do
        {
          if(send_command0(objno, command, 1+(part+3)/4, (_DWORD*)data, NULL) == _FALSE)
          {
            if(timediff(time) >= timeout)
              return(_FALSE);
          }
          else
          {
            run = 0;
          }
        }
        while(run);
      }
    }
  }

  return(_TRUE);
}

_BOOL send_command3(_ASCII *object, _UWORD station, _UWORD command, _UWORD no, _DWORD *para, void **result)
{
  _BOOL   retcode = _FALSE;
  _UDWORD objno   = send_command1_sub(object, station);

  if(objno != DEFSCOPE)
    retcode = send_command0(objno, command, no, para, result);

  return(retcode);
}

_UWORD xset_dat(_RESULT *res, _UDWORD varno, _UWORD care_state)
{
  _VARLABEL vl;
  _VARIABLE variable;
  _UWORD    retcode = ERROR_CAUSED_BY_NO_ERROR;

  init_VARIABLE_Makro(&variable);
  init_VARLABEL_Makro(&vl);
  vl.no = 1;
  vl.info[0].state = VAR_VAL;
  vl.info[0].value = varno;

  if((care_state & NOCARE_LIMITS) == 0)
  {
    retcode = ERROR_CAUSED_BY_LIMITS;
    if(res->ftype == RES_REAL)
    {
     #ifndef UC_IPC
      _BOOL tmp0 = _FALSE;
      _BOOL tmp1 = _FALSE;
      if(RealComp((_REAL*)&res->value, (_REAL*)&res->rmax, IOP_SMALLEREQUAL) == _TRUE)
        tmp0 = _TRUE;
      if(RealComp((_REAL*)&res->value, (_REAL*)&res->rmin, IOP_BIGGEREQUAL) == _TRUE)
        tmp1 = _TRUE;
      if((tmp0 == _TRUE) && (tmp1 == _TRUE))
     #else
      _REAL rval, rmax, rmin;
      move_pREAL_pREAL(&rval, (_REAL*)&res->value);
      move_pREAL_pREAL(&rmax, (_REAL*)&res->rmax);
      move_pREAL_pREAL(&rmin, (_REAL*)&res->rmin);
      if((rval <= rmax)&&(rval >= rmin))
     #endif
        retcode = ERROR_CAUSED_BY_NO_ERROR;
    }
    else
    {
      if((res->value <= res->rmax)&&(res->value >= res->rmin))
        retcode = ERROR_CAUSED_BY_NO_ERROR;
    }
  }

  if(retcode == ERROR_CAUSED_BY_NO_ERROR)
  {
    if(VarList.SetSystemDataSub(res, &variable, &vl, _TRUE, care_state) == _FALSE)
      retcode = ERROR_CAUSED_BY_NO_IDEA;
  }

  free_VARIABLE(&variable);

  return(retcode);
}

_BOOL set_dat(_RESULT *res, _UDWORD varno)
{
  _BOOL retcode = _FALSE;

  if(xset_dat(res, varno, NOCARE_DEFAULT) == ERROR_CAUSED_BY_NO_ERROR)
    retcode = _TRUE;

  return(retcode);
}




_BOOL get_minmaxsub(_IVALUE *pm, _VARLABEL *vl, _UWORD care_state, _RESULT_TYPE ftype)
{
  _RESULT res;
  _BOOL   retcode = _FALSE;

  res.ftype = RES_NUMERIC;

  if(vl->info[0].state == CONST_VAL)
  {
    retcode = _TRUE;
    *pm = vl->info[0].value;
  }
  else if(vl->info[0].state == VAR_VAL)
  {
    if(xget_dat(&res, vl->info[0].value, care_state) == ERROR_CAUSED_BY_NO_ERROR)
    {
      retcode = _TRUE;
      *pm = res.value;
    }
  }

  if((ftype == RES_REAL)&&(res.ftype != RES_REAL))
  {
   #ifndef UC_IPC
    dword2real((_REAL*)pm, (_DWORD)*pm);
   #else
    move_pREAL_DWORD((_REAL*)pm, *(_DWORD*)pm);
   #endif
  }

  return(retcode);
}

_BOOL get_minmax(_RESULT *res, _VARIABLE *p, _UWORD care_state)
{
  VarList.GetDefaultMinMax(res, p);
  get_minmaxsub(&res->rmin, &p->info.lo_limit, care_state, res->ftype);
  get_minmaxsub(&res->rmax, &p->info.hi_limit, care_state, res->ftype);
  return _TRUE;
}



/*
_BOOL get_minmax(_RESULT *res, _VARIABLE *p, _UWORD care_state)
{
  _BOOL retcode = _FALSE;

  if(VarList.GetDefaultMinMax(res, p) == _TRUE)
  {
    get_minmaxsub(&res->rmin, &p->info.lo_limit, care_state, res->ftype);
    get_minmaxsub(&res->rmax, &p->info.hi_limit, care_state, res->ftype);
    retcode = _TRUE;
  }

  return(retcode);
}
*/

_BOOL get_global_data(void *ret, _VARINFO *pvi)
{
  _BOOL retcode = _FALSE;
  _UBYTE tmp[50];
  _UBYTE size;
  _COMPORT *cp;
  _UBYTE *result;

  if(pvi->lasalid.id != 0)
  {
    cp = Batch.Get_COMPORT(pvi->station);
    size = VarList.GetVariableSize(pvi->vartype, pvi->format);

    if(pvi->lasalid.id != DEFLASALID)
    {
      *(_UDWORD*)&tmp[0] = pvi->lasalid.id;
      *(_UDWORD*)&tmp[4] = size;

      if(TxCommandIntern(I_GET_DATA, 8, tmp, &cp->comdef, (void**)&result, DEFSVRTYP) == DONE)
      {
        result += 2;
        MemCpy(ret, result, size);
        retcode = _TRUE;
      }
    }
  }

  return(retcode);
}

_UWORD xget_dat(_RESULT *res, _UDWORD varno, _UWORD care_state)
{
  _UWORD    retcode = ERROR_CAUSED_BY_NO_IDEA;
  _UWORD    size;
  _VARIABLE variable;
  _VARINFO  *pvi;
  _COMPORT  *cp;
  void      *refptr;
  _BOOL     state;

  if(VarList.GetSingleVariable(&variable, varno) == _TRUE)
  {
    retcode = ERROR_CAUSED_BY_CHKBIT;
    if((care_state & NOCARE_CHKBIT)||(chk_XCHKBIT(&variable.info.xchkbit) == _TRUE))
    {
      retcode = ERROR_CAUSED_INVALID_SERVER;
      if(chk_LASALID(&variable.info.lasalid) == _TRUE)
      {
        retcode = ERROR_CAUSED_BY_STATESCHEME;
        if(VarList.GetStateScheme(varno, care_state) != STATE_INVISIBLE)
        {
          pvi  = &variable.info;

          cp   = Batch.Get_COMPORT(pvi->station);
          size = VarList.GetVariableSize(pvi->vartype, pvi->format);

         #ifdef UC_COMPLEX_SERVER
          if(pvi->POWPRUIC & VARBIT_STRUCTELEMENT)
          {
            if(size < 4)
              size = 4;
          }
         #endif

          if(size <= sizeof(directdata))
          {
            state = _FALSE;

            if(pvi->POWPRUIC & (VARBIT_VARGLOBAL | VARBIT_VARMEMBER))
            {
              if(get_global_data(directdata, pvi) == _TRUE)
                state = _TRUE;
            }
            else
            {

             #ifdef CANOPEN
              if(get_direct_PLC_sub(directdata, &pvi->lasalid, cp, size, get_SVRTYP_from_VARINFO(pvi)) == _TRUE)
                state = _TRUE;
             #else
              #ifdef UC_EXOONLINE
               if(get_direct_PLC_sub(directdata, &pvi->lasalid, cp, size, (_SVRTYP)pvi->exoinfo) == _TRUE)
                 state = _TRUE;
              #else
               if(get_direct_PLC_sub(directdata, &pvi->lasalid, cp, size, DEFSVRTYP) == _TRUE)
                 state = _TRUE;
              #endif
             #endif
            }

            if(state == _TRUE)
            {
              state = _FALSE;
              switch(pvi->vartype)
              {
                case FENUM    :
                case BINARY   : state = format_BINARY(res, pvi, (void*)directdata); break;
                case FTIME    : state = format_FTIME(res, (void*)directdata); break;
                case FDATE    : state = format_FDATE(res, (void*)directdata); break;
                case FSTRING  : state = format_FSTRING(res, (void*)directdata); break;
                case BIT      : state = format_BIT(res, pvi, (void*)directdata); break;
              }

              retcode = (state == _TRUE)? ERROR_CAUSED_BY_NO_ERROR : ERROR_CAUSED_BY_NO_IDEA;

              if(VarList.GetPointerToData(&refptr, pvi, NULL) == _TRUE)
              {
                switch(pvi->vartype)
                {
                  case FENUM  :
                  case BINARY : set_format_BINARY(res, pvi, refptr); break;
                  case FTIME  : set_format_FTIME(res, refptr); break;
                  case FDATE  : set_format_FDATE(res, refptr); break;
                }
              }

              if((retcode == ERROR_CAUSED_BY_NO_ERROR)&&((care_state & NOCARE_LIMITS) == 0))
                get_minmax(res, &variable, care_state|NOCARE_LIMITS);
            }
          }
        }
      }
    }
  }

  if(retcode != ERROR_CAUSED_BY_NO_ERROR)
    init_RESULT(res);

  return(retcode);
}

_BOOL get_dat(_RESULT *res, _UDWORD varno)
{
  _BOOL retcode = _FALSE;

  if(xget_dat(res, varno, NOCARE_DEFAULT) == ERROR_CAUSED_BY_NO_ERROR)
    retcode = _TRUE;

  return(retcode);
}

_BOOL _Batch::ChangeRemoteAddress(_UWORD x_station, _INTERFACE typ, _UDWORD address, _BOOL observe)
{
  _BOOL    retcode = _FALSE;
  _COMPORT *pcp;
  _COMAP   *pcm;
  _COMDEF  comdef;
  _UWORD   station;
 #ifdef UC_MULTICPU
  _UWORD   multi_map_no = (x_station >> 8) & 0xFF;
  if(multi_map_no >= UC_MULTICPU)
    return(_FALSE);
 #endif

  station = x_station & 0xFF;
  pcp     = Get_COMPORT(station);

  if(pcp != NULL)
  {
    // bestehende verbindung versorgen
    new_REFLIST(pcp);

    if(IsComdefMultipleUsed(station) == _FALSE)
    {
//      LOGOUT(&pcp->comdef);
      free_COMPORT(pcp);
    }
    else
    {
      free_REFLIST(&pcp->reflist);
    }

    // neue verbindung aufbauen
    init_COMDEF(&comdef);
    comdef.typ = typ;

    if((typ == _COM_TCPIP1) || (typ == _COM_TCPIP1_P) || (typ == _COM_TCPIP2_P))
    {
      comdef.ipadress = address;
    }
    else if((typ == _COM_CAN1)||
            (typ == _COM_CAN2)||
            (typ == _COM_CAN20)||
            (typ == _COM_CAN21)||
            (typ == _COM_CAN22)||
            (typ == _COM_CAN23)||
            (typ == _COM_CAN24)||
            (typ == _COM_CAN25)||
            (typ == _COM_CAN26)||
            (typ == _COM_CAN27)||
            (typ == _COM_CAN28)||
            (typ == _COM_CAN29))
    {
      comdef.adress = address;
    }
    
    pcm = &IpcIni.ComSet.mapper[station];
   #ifdef UC_MULTICPU
    if(multi_map_no == 0)
      pcm->aktmap = 0xFFFF;
    pcm->multimap[multi_map_no] = 0xFFFF;
   #else
    pcm->aktmap = 0xFFFF;
   #endif

   #ifdef UC_MULTICPU
    Insert_COMPORT(&comdef, station, observe, multi_map_no);
   #else
    Insert_COMPORT(&comdef, station, observe, 0);
   #endif

    Root.FindTop()->AddCmd(CMD_RECONFIGVAR, 0, 0, _FALSE);
  }

  return(retcode);
}

//****************************************************************************
//  LASAL Interface
//****************************************************************************
//****************************************************************************
//****************************************************************************
extern "C" void VarList_ReGetID(_BOOL bar)
{
  VarList.ReGetID(bar);
}

extern "C" _BOOL ChangeRemoteAddress(_UWORD station, _INTERFACE typ, _UDWORD address, _BOOL observe)
{
  return(Batch.ChangeRemoteAddress(station, typ, address, observe));
}

extern "C" _BOOL is_new_online(_COMPORT *pcp)
{
  return(Batch.IsOnline(pcp));
}

void SoftReset(void)
{
  TermAlarm.CacheFinalize();

  _FVOID_VOID ptr = (_FVOID_VOID)find_lasal_method(LseThisPointer, METHOD_LSE_SOFTRESET);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    ptr();
  }
}

