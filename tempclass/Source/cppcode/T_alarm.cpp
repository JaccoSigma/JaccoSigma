#include "T_IPC.H"
#include "T_NUC.H"
#include "COMINTFC.H"

#define HISTORYFILENAME    "LOGFILE.BIN"
#ifdef SA34706
#define HISTORYFILEID      "HISTORY"
#else
#define HISTORYFILEID      "LOGFILE"
#endif
#define HISTORYFILEVERSION 0

#define SA33569
#ifndef USER_DEMAG // 28.02.2017: user demag will das nicht
#ifndef USER_SMT // 19.03.2018: user SMT will das nicht (SA40534)
#define SA35201 // multiple alarmconfig
#endif
#endif

#define _8D325 // 8D-325: AlarmConfignummern stationsbezogen machen


#ifdef UC_HISTORY_FILECACHE // 8D-3231
#define HISTORY_CACHE_SIZE   20 // max Recordanzahl im Cache
#define HISTORY_CACHE_TIME 5000 // Timedelay in ms
typedef struct
{
  _UDWORD no;     // Anzahl gepufferter Records
  _UDWORD offset; // Beginn Fileoffset
  _UDWORD xtime;  // Zeitstempel letzte Aktivität (8D-3231: Wunsch von ObeChr, Freigabe SeiHan !)
  _UBYTE  data[HISTORY_CACHE_SIZE*512]; // Records
} _HISTORY_CACHE; // 8D-3231

_HISTORY_CACHE HistoryCache;
#endif //UC_HISTORY_FILECACHE


void _TermAlarm::CacheReset(void)
{
 #ifdef UC_HISTORY_FILECACHE
  HistoryCache.no     = 0;
  HistoryCache.offset = 0;
  HistoryCache.xtime  = 0;
 #endif //UC_HISTORY_FILECACHE
}

void _TermAlarm::CacheRun(void)
{
 #ifdef UC_HISTORY_FILECACHE
  if(HistoryCache.no > 0)
  {
    if((lib_tabsolute() - HistoryCache.xtime) > HISTORY_CACHE_TIME)
    {
      CacheFinalize();
    }
  }
 #endif //UC_HISTORY_FILECACHE
}

_BOOL _TermAlarm::CacheFinalize(void)
{
  _BOOL retcode = _TRUE;
  
 #ifdef UC_HISTORY_FILECACHE
  if(HistoryCache.no > 0)
  {
    if(History.File.Seek(HistoryCache.offset, SEEK_SET) == _TRUE)
    {
      _UWORD bytesize = HistoryCache.no * 512;
      retcode = History.File.Write(HistoryCache.data, bytesize);
    }
    
    CacheReset();
  }
 #endif //UC_HISTORY_FILECACHE
  
  return retcode;
}

_BOOL _TermAlarm::CacheAdd(_SINGLEALARM *psa, _UDWORD offset)
{
  _BOOL retcode = _FALSE;
  
 #ifdef UC_HISTORY_FILECACHE
  if(History.File.GetError() == _FALSE)
  {
    if(HistoryCache.no > 0)
    {
      _UDWORD isoff = HistoryCache.offset + (HistoryCache.no * 512);
      if((HistoryCache.no >= HISTORY_CACHE_SIZE) || (isoff != offset))
      {
        CacheFinalize();
      }
    }

    _UDWORD tmpoff = HistoryCache.no * 512;
    if(History.File.Memory_SINGLEALARM(&HistoryCache.data[tmpoff], psa, HISTORYFILEVERSION) == _TRUE)
    {
      if(HistoryCache.no == 0)
      {
        HistoryCache.offset = offset;
      }
    
      // if(HistoryCache.no == 0) // unerwünscht ?!
      // {
      HistoryCache.xtime = lib_tabsolute();
      // }
      HistoryCache.no   += 1;
      retcode = _TRUE;
    }
  }
 #endif //UC_HISTORY_FILECACHE
  
  return retcode;
}

void getAlarmsFrom(_COMPORT *pcp, _UDWORD start, _UDWORD anz, _MEM_SINGLEALARM *ptemp);


void _Alarm::Init(void)
{
  LseLabel.Init();
  Valid = _FALSE;
  init_ALARM(&Data);
  LseCrc32 = DEFAULTCRC;
}

void _Alarm::Free(void)
{
  LseLabel.Free();
  free_ALARM(&Data);

  Init();
}

_CRC32 _Alarm::GetLseCrc32(void)
{
  return(LseCrc32);
}

_BOOL _Alarm::Load(_ASCII *dpne)
{
  _File File;
  _BOOL retcode = _FALSE;

  Free();

  File.Open(LOAD, dpne);
  if(File.ReadHeader() == _TRUE)
  {
    LseCrc32 = File.GetLseCrc32();
    retcode  = File.Read_ALARM(&Data);
   #if COMPILER_VERSION >= 33
    LseLabel.Read(&File, _FALSE);
   #endif
  }
  File.Close();

  return(retcode);
}

#ifdef SA35201
_BOOL _Alarm::ScanDoubleConfigNo(_UDWORD nox, _SINGLE_ALARM *psrc)
{
  _SINGLE_ALARM *psa = Data.ptr;
  _ASCII *ph, *ptxt = (_ASCII*)get_temptext();

  while(nox--)
  { 
    if(psrc->alano == psa->alano)
    {
      _UBYTE varobj = 0;
      StrCpy(ptxt, 1, "Same Alarmconfig at Server: ", 1);
      ph = ptxt + StrLen(ptxt, 1);
      VarList.GetLabel(ph, &varobj, psa->link.info[0].value);
      StrCat(ptxt, 1, " and ", 1);
      ph = ptxt + StrLen(ptxt, 1);
      VarList.GetLabel(ph, &varobj, psrc->link.info[0].value);
      Trace(ptxt);
      return _TRUE;
    }
    psa++;
  }
  
  return _FALSE;
}
#endif

_BOOL _Alarm::Config(void)
{
  _UWORD        i, bs;
  _SINGLE_ALARM *psa;
 #ifdef SA35201 
  _UDWORD       doubleconfig = 0;
 #endif // SA35201

 #ifndef UC_UNREGISTERED_ALARM
  if(Data.no > 0)
 #endif 
  {
    bs = ScanBar.Add("AlarmConfig.:", (Data.no)? Data.no : 1);

   #ifdef OK_CHECK_CRCOFID
//    if(LookForAlarmCrc() == _FALSE)
   #endif
    {
      psa = Data.ptr;
      for(i=0; i<Data.no; i++)
      {
        ScanBar.Run(bs, i);
        if(config_SINGLE_ALARM(psa, _FALSE) == _TRUE)
        {
         #ifdef SA35201
          if(ScanDoubleConfigNo(i, psa) == _TRUE)
          {
            doubleconfig++;
          }
         #endif // SA35201 
        }
        else
        {
          init_MEM_VARLABEL(&psa->parameter);
        }
        psa += 1;
      }
    }
    
   #ifdef OK_CHECK_CRCOFID
//    SaveAlarmCrc();
   #endif

    Valid = _TRUE;

   #ifdef USER_DEMAG
    TermAlarm.ReConfig(_TRUE);    // es wird GetLostAlarms aufgerufen - nicht mehr nötig
    TermAlarm.ReConfig(_FALSE);
   #endif
   #ifdef USER_HTC
    TermAlarm.ReConfig(_TRUE);    // es wird GetLostAlarms aufgerufen - nicht mehr nötig
    TermAlarm.ReConfig(_FALSE);
   #endif

    TermAlarm.LookForReConfig(_TRUE);
    TermAlarm.LookForReConfig(_FALSE);
    
   #ifdef UC_NEW_ALARMSYSTEM
   #ifdef UC_UNREGISTERED_ALARM
    // create alarms which are not ready done in lse-alarmdialog
    DoUnregisteredAlarms(bs);
   #endif
   #endif

    ScanBar.End(bs);
    
   #ifdef SA35201 
    if(doubleconfig != 0)
    {
      ScanBar.Assert("WARNING: AlarmConfigNumber multiple used, check Debugger Trace for details."); 
      //End(ScanBar.Add("WARNING: AlarmConfigNumber multiple used, check Debugger Trace for details.", 0, MakeColor(BLACK, YELLOW)));
    }
   #endif // SA35201 
  }

  return(_TRUE);
}

void _Alarm::GetTempBuffer(void)
{
 #ifdef UC_NEW_ALARMSYSTEM
  _UDWORD  tmpstart, tmpanz;
  _COMPORT *pcp;
  _UWORD   i, end = Batch.GetNo_COMPORT();

  // sa19670
  // es weden alle einträge aus dem alarmXbuffer aus der sps ausgelesen
  // funktion beinhaltet eine gewisse unsicherheit (überlauf in sps-puffer)
  // es werden alle im sps-buffer eingetragenen records neu gelesen. dies entspricht nicht einer liste aller aktiven alarme!
  // -> mit appli und support besprochen, hlawna und hötzenauer fordern diese funktionalität so wie sie ist als standard.

  for(i=0; i<end; i++) 
  {
    pcp = Batch.GetDirect_COMPORT(i);
  
    if(pcp != NULL) 
    {
      if(pcp->reflist.login == _LOGIN_TRUE)
      {
        if((pcp->alarmAnzahl > 0) && (pcp->alarmValidIndex > 0))
        {
          tmpanz   = pcp->alarmValidIndex;
          tmpstart = 0;
          if(pcp->alarmValidIndex > pcp->alarmXBufferSize)
          {
            tmpanz   = pcp->alarmXBufferSize;
            tmpstart = pcp->alarmValidIndex - pcp->alarmXBufferSize;
          }
                
          getAlarmsFrom(pcp, tmpstart, tmpanz, TermAlarm.Get_MEM_SINGLEALARM(_FALSE));
        }
      }
    }
  }
 #endif 
}

_UWORD _Alarm::GetIndex(_UWORD config_no, _COMPORT *pcp)
{
  _UWORD retcode = 0xFFFF;

  _SINGLE_ALARM *pa = Data.ptr;
  for(_UWORD i=0; i<Data.no; i++)
  {
    if(pa->alano == config_no)
    {
     #ifdef _8D325 // 8D-325:
      if(pcp != NULL)
      {
        if(retcode == 0xFFFF)
        {
          retcode = i;
        }
        _LSEVARINFO *pvar = VarList.Get(&pa->link);
        if(pvar != NULL)
        {
          _COMPORT *pcv = Batch.Get_COMPORT(pvar->station);
          if(pcv == pcp)
          {
            return i;
          }
        }
      }
      else
     #endif
      {
        return i;
      }  
    }
    pa += 1;
  }

  return retcode;
}

_UWORD _Alarm::GetIndexByGroup(_UWORD group)
{
  _UWORD retcode = 0xFFFF;
  _UWORD i;
  _SINGLE_ALARM *pa;


  pa = Data.ptr;
  for(i=0; i<Data.no; i++)
  {
    if(pa->group == group)
    {
      retcode = i;
      break;
    }

    pa++;
  }

  return(retcode);
}

_SINGLE_ALARM *_Alarm::Get_SINGLE_ALARM(_UWORD no)
{
  _SINGLE_ALARM *retcode = NULL;

  if(no < Data.no)
    retcode = &Data.ptr[no];

  return(retcode);
}

_ALARM *_Alarm::GetAlarm(void)
{
  return((Valid == _TRUE)? &Data : NULL);
}

_ALARM *_Alarm::GetAlarmDirect(void)
{
  return(&Data);
}

void merge_label(_ASCII *o, _ASCII *s)
{
  _UWORD i, len;
  _ASCII *ps;

  len = StrLen(s, sizeof(_ASCII));
  for(i=0,ps=s; i<=len; i++)
  {
    switch(*ps)
    {
      case  0 :
      case 92 :
      case 46 : StrCat(o, sizeof(_ASCII), ps, sizeof(_ASCII)); return;
    }
    ps++;
  }
}

_SINGLE_ALARM *_Alarm::LookUp(_UDWORD varno)
{
  _LSEVARINFO   *pi;
  _UDWORD       i, srt;
  _SINGLE_ALARM *ps;
  _ASCII        objlabel[150];
  _ASCII        alarmlabel[150];
  _UBYTE        obj = 0;
  _VARLABEL     tmp;

  pi = VarList.GetRootInfo(varno);
  if(pi != NULL)
  {
    if(VarList.GetLabel(alarmlabel, &obj, varno) == _TRUE)
    {
      ps = Data.ptr;
      for(i=0; i<Data.no; i++)
      {
        srt = VarList.LookUpIfInRootIsSameType(ps->link.info[0].value, pi->classid);

        if(VarList.GetLabel(objlabel, &obj, srt) == _TRUE)
        {
          merge_label(objlabel, alarmlabel);

          tmp.no = 1;
          tmp.info[0].state = VAR_VAL;
          tmp.info[0].value = VarList.GetVarNo(objlabel, pi->station);

          if(cmp_VARLABEL(&ps->link, &tmp) == 0)
            return(ps);     // bingo
        }
        ps += 1;
      }
    }
  }

  return(NULL);
}

_UWORD _Alarm::GetNo(void)
{
  _UDWORD retcode = 0;
  _ALARM  *pa = GetAlarm();

  if(pa != NULL)
    retcode = pa->no;

  return(retcode);
}

_BOOL _Alarm::Add(_UDWORD varno, _BOOL unregistered)
{
  _BOOL         retcode = _FALSE;
  _SINGLE_ALARM *ps, *pd;
  _ASCII        tmpnam[64];

  if(Memory2((void**)&Data.ptr, (Data.no + 1) * sizeof(_SINGLE_ALARM)) == _TRUE)
  {
    retcode = _TRUE;
    
    tmpnam[0] = '@';
    tmpnam[1] = 'u';
    tmpnam[2] = 'n';
    tmpnam[3] = 'r';
    tmpnam[4] = 'e';
    tmpnam[5] = 'g';
    DToA(&tmpnam[6], Data.no, 0x800, 1);
    
    pd      = &Data.ptr[Data.no];
    init_SINGLE_ALARM(pd);
    Data.no += 1;
    
    ps = (unregistered == _TRUE)? NULL : LookUp(varno);
    if(ps != NULL)
    {
      if(copy_SINGLE_ALARM(&retcode, pd, ps) == _TRUE)
      {
        pd->link.info[0].value = varno;
        init_MEM_VARLABEL(&pd->parameter);
      }
    }
    else
    {
      if(Data.no > 1)
      {
        // set colors taken from first entry
        pd->active       = Data.ptr->active;
        pd->passive      = Data.ptr->passive;
        pd->quit_active  = Data.ptr->quit_active;
        pd->quit_passive = Data.ptr->quit_passive;
      }
      else
      {
        // set defaultcolors
        pd->active       = MakeColor_Makro(MakeColorFromIndex(12), MakeColorFromIndex(15));
        pd->passive      = MakeColor_Makro(MakeColorFromIndex(13), MakeColorFromIndex(15));
        pd->quit_active  = MakeColor_Makro(MakeColorFromIndex(12), MakeColorFromIndex(7));
        pd->quit_passive = MakeColor_Makro(MakeColorFromIndex(7), MakeColorFromIndex(13));
      }
      
      pd->link.no = 1;
      pd->link.info[0].state = VAR_VAL;
      pd->link.info[0].value = varno;
    }

    pd->text.anything.list     = VARTXT1; // SA34586
    pd->text.anything.no       = varno;
    pd->helptext.anything.list = VARTXT2;
    pd->helptext.anything.no   = varno;

    LseLabel.Add(tmpnam);

    config_SINGLE_ALARM(pd, unregistered); // == _FALSE)
//    if(config_SINGLE_ALARM(pd, _FALSE) == _FALSE)
//      free_SINGLE_ALARM(pd);

  }

  return(retcode);
}

#if HISTORYFILEVERSION >= 1
_BOOL _Alarm::InsertAlarmIdByIndex(_SINGLEALARM *p)
{
  _BOOL         retcode = _TRUE;
  _SINGLE_ALARM *psa;
  _LSEVARINFO   *pvi;
  _UDWORD       i;

  if(p->entrytype == 0xFFFF)
  {
    retcode = _FALSE;
//    if(Valid == _TRUE)
    {
      if(p->anything.list == ALARMLIST)
      {
        psa = Get_SINGLE_ALARM(p->anything.no);
        if(psa != NULL)
        {
          if((psa->link.no > 0)&&(psa->link.info[0].state == VAR_VAL))
          {
            if(VarList.GetVarInfo(&pvi, psa->link.info[0].value) == _TRUE)
            {
              p->alarmid = pvi->crc32;
              retcode    = _TRUE;
            }
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL _Alarm::InsertIndexByAlarmId(_SINGLEALARM *p)
{
  _BOOL         retcode = _TRUE;
  _SINGLE_ALARM *psa;
  _LSEVARINFO   *pvi;
  _UDWORD       i;

  if(p->entrytype == 0xFFFF)
  {
    retcode = _FALSE;
//    if(Valid == _TRUE)
    {
      psa = Data.ptr;
      for(i=0; i<Data.no; i++)
      {
        if(chk_CHKBIT(&psa->chkbit) == _TRUE)
        {
          if((psa->link.no > 0)&&(psa->link.info[0].state == VAR_VAL))
          {
            if(VarList.GetVarInfo(&pvi, psa->link.info[0].value) == _TRUE)
            {
              if(p->alarmid == pvi->crc32)
              {
                p->anything.list = ALARMLIST;
                p->anything.no   = i;
                return(_TRUE);
              }
            }
          }
        }

        psa += 1;
      }
    }
  }

  return(retcode);
}
#endif

// ***************************************************************************
// ***************************************************************************
// ***************************************************************************
// ***************************************************************************

#ifndef UC_NEW_ALARMSYSTEM
_BOOL acknowledge_alarm_sub(_SINGLEALARM *p, _UWORD station)
{
  _SINGLE_ALARM *psa;
  _BOOL         retcode = _FALSE;
  _RESULT       result;
  _BOOL         quit = _TRUE;
  _LSEVARINFO   *pvi = NULL;
  _VARLABEL     link;

  if(p->acknowledge == 0)
  {
    // Wird wegen Kompatibilität schon hier quittiert
    if(station == 0xFFFF)
      p->acknowledge = 1;

    if(p->anything.list == ALARMLIST)
    {
      psa = Alarm.Get_SINGLE_ALARM(p->anything.no);
      if(psa != NULL)
      {
       #ifdef UC_MULTICPU
        _LSEVARINFO *plvi = NULL;
        _UDWORD     mlid;
        _UWORD      maktmap;
        if(VarList.GetVarInfo(&plvi, psa->link.info[0].value) == _TRUE)
        {
          mlid = plvi->lasalid.id;
          if(p->multicpuno < UC_MULTICPU)
          {
            plvi->lasalid.id = plvi->multiid[p->multicpuno];
            maktmap          = Batch.ChangeAktMap(plvi->station, p->multicpuno);
          }
          else
          {
            plvi = NULL;
          }
        }
       #endif

       #ifdef UC_ALARMGROUPS
        init_VARLABEL(&link);
        if(p->no_para & 0x80)
        {
          if(VarList.GetVarInfo(&pvi, psa->link.info[0].value) == _TRUE)
          {
            link.no = 1;
            link.info[0].state = VAR_VAL;
            link.info[0].value = VarList.GetVarNoByCRC(p->para[4]);
          }
        }
        else
       #endif
          copy_VARLABEL(&link, &psa->link);

        if(valid_VARLABEL(&link) == _TRUE)
        {
          if(station != 0xFFFF)
          {
            quit = _FALSE;
            if(VarList.GetVarInfo(&pvi, link.info[0].value) == _TRUE)
            {
              if(pvi->station == station)
              {
                p->acknowledge = 1;
                quit = _TRUE;
              }
            }
          }

          if(quit == _TRUE)
          {
            init_RESULT_Makro(&result);

            result.rmax  =
            result.rmin  =
            result.value = 0xFF;
            result.ftype = RES_CONSTANT;
            result.ptr   = &result.value;

            if(xset_dat(&result, link.info[0].value, NOCARE_ALL) == ERROR_CAUSED_BY_NO_ERROR)
              retcode = _TRUE;
          }
        }
       #ifdef UC_MULTICPU
        if(plvi != NULL)
        {
          plvi->lasalid.id = mlid;
          Batch.SetAktMap(plvi->station, maktmap);
        }
       #endif
      }
    }
  }

  return(retcode);
}
#endif

_BOOL acknowledge_alarm(_MEM_SINGLEALARM *p, _DWORD pos)
{

 #ifdef UC_NEW_ALARMSYSTEM
  if(pos < 0)
  {
    if(pos == -2)
    {
      quitAlarm(0xFFFF, 0xFFFFFFFE); // QuitEx bei allen Alarme auf allen Stationen
    }
    else
    {
      quitAlarm(0xFFFF, 0xFFFFFFFF); // alle Alarme auf allen Stationen
    }
#ifdef USER_KM
    _UDWORD no = no_RINGBUFFER(&p->ringbuffer);
    while(no != 0)
    {
      no -= 1;
      _SINGLEALARM *psa = &p->ptr[rpos_RINGBUFFER(&p->ringbuffer, no)];
      if(psa->anything.list == ALARMLIST)
        psa->acknowledge = 1;
    }
#endif    
  }
  else if(p != NULL)
  {
    if(pos < no_RINGBUFFER(&p->ringbuffer))
    {
      _SINGLEALARM *psa = &p->ptr[rpos_RINGBUFFER(&p->ringbuffer, pos)];
      if(psa->anything.list == ALARMLIST)
      {
        _SINGLE_ALARM *psal = Alarm.Get_SINGLE_ALARM(psa->anything.no);
        if(psal != NULL)
        {
          if(valid_VARLABEL(&psal->link) == _TRUE)
          {
            _LSEVARINFO *pvi;
            if(VarList.GetVarInfo(&pvi, psal->link.info[0].value) == _TRUE)
            {
              quitAlarm(pvi->station, psa->no); //psal->alano);
            }
          }
        }
      }
    }
  }
  return(_TRUE);
 #else
  _UDWORD i, no;
  _BOOL   retcode = _FALSE;

  if(p != NULL)
  {
    no = no_RINGBUFFER(&p->ringbuffer);

    if(pos == -1)
    {
      for(i=0; i<no; i++)
      {
        if(acknowledge_alarm_sub(&p->ptr[rpos_RINGBUFFER(&p->ringbuffer, i)], 0xFFFF) == _TRUE)
          retcode = _TRUE;
      }
    }
    else if(pos < no)
    {
      if(acknowledge_alarm_sub(&p->ptr[rpos_RINGBUFFER(&p->ringbuffer, pos)], 0xFFFF) == _TRUE)
        retcode = _TRUE;
    }

    if(retcode == _TRUE)
      p->xchange += 1;                               // redraw whole page
  }
  return(retcode);
 #endif

}

_BOOL acknowledge_alarm_station(_UWORD station, _BOOL history)
{
 #ifdef UC_NEW_ALARMSYSTEM
  return(quitAlarm(station, 0xFFFFFFFF));
 #else
  _BOOL retcode = _FALSE;
  _MEM_SINGLEALARM *pmsa;
  _UDWORD      i, no;


  pmsa = get_MEM_SINGLEALARM(history);

  if(pmsa != NULL)
  {
    no = no_RINGBUFFER(&pmsa->ringbuffer);

    for(i=0; i<no; i++)
    {
      if(acknowledge_alarm_sub(&pmsa->ptr[rpos_RINGBUFFER(&pmsa->ringbuffer, i)], station) == _TRUE)
        retcode = _TRUE;
    }

    if(retcode == _TRUE)
      pmsa->xchange += 1;                               // redraw whole page
  }

  return(retcode);
 #endif
}

_BOOL NoDelActiveAnd2Line(_UBYTE filter)
{
  // sa7891
  if((filter & (BIT_ONOFF|BIT_DELQUIT)) == BIT_DELQUIT)
    return(_TRUE);

  return(_FALSE);
}

_BOOL keep_alarm(_MEM_SINGLEALARM *p, _SINGLEALARM *ptr, _DWORD pos, _UDWORD i, _UWORD station)
{
  _BOOL retcode = _TRUE;
  _SINGLE_ALARM *psa;
  _LSEVARINFO *pvi;

  if(pos == -2) // sa39401
  {
    retcode = _FALSE;
  }
  else if((i == pos)||(pos == -1))
  {
    retcode = _FALSE;

    if(p->filter & BIT_DELQUIT)
    {
      if((NoDelActiveAnd2Line(p->filter) == _TRUE) && ((ptr->state & 3) == 3))
      {
        // sa7891
        retcode = _FALSE;
      }
      else if((ptr->state & 1) == 1) // aktiver alarm
      {
        acknowledge_alarm(p, i); // aktiven alarm jetzt quittieren
        retcode = _TRUE;         // und doch behalten
      }
    }
    else if(ptr->acknowledge == 0)
    {
      retcode = _TRUE;
    }
    else if(station != 0xFFFF)
    {
      psa = Alarm.Get_SINGLE_ALARM(ptr->anything.no);
      if(psa != NULL)
      {
        if(VarList.GetVarInfo(&pvi, psa->link.info[0].value) == _TRUE)
        {
          if(pvi->station == station)
          {
            retcode = _FALSE;
          }
        }
      }
    }
  }

  return(retcode);
}

void save_all_file_entry(_MEM_SINGLEALARM *p, _ASCII *txt)
{
  _UDWORD      no, i;
  _BOOL        state = _TRUE;
  _SINGLEALARM sa, *psa;
  _UWORD       bs;

  if(p->locked == _FALSE)
  {
    if(txt != NULL)
      bs = ScanBar.Add(txt, p->ringbuffer.no);

    TermAlarm.CacheFinalize();

    if(p->File.GetError() == _FALSE)
    {
      init_SINGLEALARM(&sa);
      no = no_RINGBUFFER(&p->ringbuffer);

      if(p->File.Seek(512, SEEK_SET) == _TRUE)
      {
        p->seqno = 0; // alle neuen alarme beginnen mit 0
        for(i=0; i<p->ringbuffer.no; i++)
        {
          if(txt != NULL)
            ScanBar.Run(bs, i);

          if(state == _TRUE)
          {
            psa = &sa;
            if(i < no)
            {
              psa = &p->ptr[rpos_RINGBUFFER(&p->ringbuffer, i)];
              psa->seqno = i;
              p->seqno   = i+1;
            }

            state = p->File.Write_SINGLEALARM(psa, HISTORYFILEVERSION);
          }
        }
      }
    }

    if(txt != NULL)
      ScanBar.End(bs);

  }
}

_BOOL delete_alarm(_MEM_SINGLEALARM *p, _DWORD pos)
{
  _BOOL        retcode = _FALSE;
  _UDWORD      i, no;
  _SINGLEALARM *ptr, *pd;
  _RINGBUFFER  tmp;

  if(p != NULL)
  {
    no       = no_RINGBUFFER(&p->ringbuffer);
    tmp      = p->ringbuffer;
    tmp.used = _FALSE;
    tmp.pos  = tmp.begin;

    if(p->filter & BIT_POWER_ON)
    {
      if(pos == -3) // spezielles löschen, alles incl. letzter POWER ON zeile löschen (sa39254)
      {
        _UDWORD end = 0xFFFFFFFFUL;
        for(i=0; i<no; i++)
        {
          ptr = &p->ptr[rpos_RINGBUFFER(&p->ringbuffer, i)];
          if(ptr->entrytype == 0xFFFC)
          {
            end = i;
          }
        }
        
        if(end < no)
        {
          for(i=0; i<no; i++)
          {
            ptr = &p->ptr[rpos_RINGBUFFER(&p->ringbuffer, i)];

            if(i > end)
            {
              pd = &p->ptr[wpos_RINGBUFFER(&tmp)];
              if(pd != ptr)
              {
                *pd = *ptr;
                init_SINGLEALARM(ptr);
              }
            }
            else
            {
              free_SINGLEALARM(ptr);
              retcode = _TRUE;
            }
          }
        }
//        else
//        {
//          pos = -2; // alle löschen
//        }
      } 
    }
   
    
    if(pos > -3)
    {
      for(i=0; i<no; i++)
      {
        ptr = &p->ptr[rpos_RINGBUFFER(&p->ringbuffer, i)];

//      if((ptr->acknowledge == 0)||((i != pos)&&(pos != -1)))
        if(keep_alarm(p, ptr, pos, i, 0xFFFF) == _TRUE)
        {
          pd = &p->ptr[wpos_RINGBUFFER(&tmp)];
          if(pd != ptr)
          {
            *pd = *ptr;
            init_SINGLEALARM(ptr);
          }
        }
        else
        {
          free_SINGLEALARM(ptr);
          retcode = _TRUE;
        }
      }
    }
    

    if(retcode == _TRUE)
    {
      if(no != no_RINGBUFFER(&tmp)) //p->ringbuffer))
      {
        p->ringbuffer = tmp;
        save_all_file_entry(p, NULL);  // ggf. alle einträge im file als gelöscht markieren
      }
    }
    
    active_MEM_SINGLEALARM(p);

    if(retcode == _TRUE)
    {
      p->xchange += 1;                               // redraw whole page
    }
  }

  return(retcode);
}

_BOOL delete_alarm_station(_UWORD station, _BOOL history)
{
  _BOOL        retcode = _FALSE;
  _UDWORD      i, no;
  _SINGLEALARM *ptr, *pd;
  _RINGBUFFER  tmp;
  _MEM_SINGLEALARM *p;


  p = get_MEM_SINGLEALARM(history);

  if(p != NULL)
  {
    no       = no_RINGBUFFER(&p->ringbuffer);
    tmp      = p->ringbuffer;
    tmp.used = _FALSE;
    tmp.pos  = tmp.begin;

    for(i=0; i<no; i++)
    {
      ptr = &p->ptr[rpos_RINGBUFFER(&p->ringbuffer, i)];

//      if((ptr->acknowledge == 0)||((i != pos)&&(pos != -1)))
      if(keep_alarm(p, ptr, -1, i, station) == _TRUE)
      {
        pd = &p->ptr[wpos_RINGBUFFER(&tmp)];
        if(pd != ptr)
          *pd = *ptr;
      }
      else
      {
        free_SINGLEALARM(ptr);
        retcode = _TRUE;
      }
    }

    p->ringbuffer = tmp;

    if(no != no_RINGBUFFER(&p->ringbuffer))
      save_all_file_entry(p, NULL);  // ggf. alle einträge im file als gelöscht markieren

    active_MEM_SINGLEALARM(p);

    if(retcode == _TRUE)
      p->xchange += 1;                               // redraw whole page
  }

  return(retcode);
}


// neu ***********************************************************************
// neu ***********************************************************************
// neu ***********************************************************************
// neu ***********************************************************************
// neu ***********************************************************************
// neu ***********************************************************************
// neu ***********************************************************************

void _TermAlarm::Init(_BOOL urinit)
{
  _UWORD i;

  CacheReset();

  HistoryDPNE[0] = 0;
  Queue = _FALSE;

  if(urinit == _TRUE)
  {
    init_MEM_SINGLEALARM(&History);
    init_MEM_SINGLEALARM(&Temporary);
  }

  for(i=0; i<TERMALARMPUFFER; i++)
    init_SINGLEALARM(&Puffer[i]);

  MaxDateTime = 0;
}

void _TermAlarm::Free(void)
{
  free_MEM_SINGLEALARM(&History);
  free_MEM_SINGLEALARM(&Temporary);
  Init(_FALSE);
}

void _TermAlarm::LookForReConfig(_BOOL st)
{
  _MEM_SINGLEALARM *p;

  p   = (st == _TRUE)? &Temporary : &History;

  if(p->filter & BIT_RESETACTIVE)
    ReConfig(st);
}

void _TermAlarm::ReConfig(_BOOL st)
{
  _UDWORD          no, i;
  _SINGLEALARM     *ptr;
  _MEM_SINGLEALARM *p;
  _DATE            date;
  _TIME            time;

  p   = (st == _TRUE)? &Temporary : &History;
  no  = no_RINGBUFFER(&p->ringbuffer);
  for(i=0; i<no; i++)
  {
    ptr = &p->ptr[rpos_RINGBUFFER(&p->ringbuffer, i)];
    if(ptr->anything.list == ALARMLIST)
    {
      ptr->anything.no = Alarm.GetIndex(ptr->no, NULL);

      //if(p->filter & BIT_ONOFF)
      if(p->filter & (BIT_ONOFF|BIT_USECOUNTER)) // SA24407 BIT_USECOUNTER
      {
        // AlarmXPara does not send the alarm when the alarm state is changing
        // to 0 while the IPC is switched off
        if((ptr->state & 1) != 0)
        {
          ptr->state &= 0xFE;
          get_DATE(&date);
          get_TIME(&time);
          DATE_to_lasal(&ptr->date_off, &date);
          TIME_to_lasal(&ptr->time_off, &time);
          Insert_SINGLEALARM(p, ptr); // bestehenden eintrag in ringfile ändern
        }
      }
    }
  }
}

_BOOL _TermAlarm::LockHistory(_BOOL state)
{
//  _ASCII       *hptr;
  _BOOL        retcode = _FALSE;
  _UDWORD      no, i;
  _SINGLEALARM *ptr;

  if(state == _TRUE)
  {
    CacheFinalize();
    if(History.locked == _FALSE)
    {
      History.locked = _TRUE;
      History.File.Close();
    }
    retcode = _TRUE;
  }
  else
  {
    if(History.locked == _TRUE)
    {
      if(History.File.Open(LOADSAVE, HistoryDPNE) == _TRUE)
      {
        History.locked = _FALSE;

        no = no_RINGBUFFER(&History.ringbuffer);
        for(i=0; i<no; i++)
        {
          ptr = &History.ptr[rpos_RINGBUFFER(&History.ringbuffer, i)];
          if(ptr->was_locked == _TRUE)
          {
            ptr->was_locked = _FALSE;
            Insert_SINGLEALARM(&History, ptr);
          }
        }
        retcode = _TRUE;
      }
    }
  }
  return(retcode);
}

_BOOL _TermAlarm::Insert_SINGLEALARM(_MEM_SINGLEALARM *p, _SINGLEALARM *ptr)
{
  _BOOL retcode = _FALSE;

  if(p->locked == _FALSE)
  {
    if(p->File.GetError() == _FALSE)
    {
      if(ptr->seqno == 0xFFFFFFFFL)
      {
        ptr->seqno = p->seqno;
        p->seqno  += 1;
      }

      _UDWORD pos = (ptr->seqno % p->ringbuffer.no);
      pos = (pos * 512) + 512;
      
      if(CacheAdd(ptr, pos) == _FALSE) // ggf. in Cache schreiben
      {
        if(p->File.Seek(pos, SEEK_SET) == _TRUE)
        {
          p->File.Write_SINGLEALARM(ptr, HISTORYFILEVERSION);
        }
      }
      retcode = _TRUE;
    }
  }
  else
  {
    ptr->was_locked = _TRUE;
  }

  return(retcode);
}

#ifdef SA33569
_BOOL _TermAlarm::CheckParameterFilter(_MEM_SINGLEALARM *p, _SINGLEALARM *dst, _SINGLEALARM *src)
{
  if(p->para_filter != 0)
  {
    _UDWORD nox = (dst->no_para < src->no_para)? dst->no_para : src->no_para; // anzahl der parameter muss eigentlich gleich sein-> sicherheit
    _UDWORD bit = 1;
    for(_UDWORD i=0; i<nox; i++)
    {
      if(p->para_filter & bit)
      {
        if(src->para[i] != dst->para[i])
        {
          return _FALSE;
        }
      }
      bit <<= 1;
    }
  }
  return _TRUE;
}
#endif

_BOOL _TermAlarm::Insert_MEM_SINGLEALARM(_MEM_SINGLEALARM *p, _SINGLEALARM *s)
{
  _BOOL        retcode = _FALSE;
  _UDWORD      no, i;
  _SINGLEALARM *ptr;

  if(p->ringbuffer.no > 0)
  {
    if(s->entrytype == 0xFFFF)
    {
      if(p->filter & (BIT_ONOFF|BIT_USECOUNTER))
      {
        no = no_RINGBUFFER(&p->ringbuffer);
        for(i=0; i<no; i++)
        {
          ptr = &p->ptr[rpos_RINGBUFFER(&p->ringbuffer, i)];
         #ifdef UC_MULTICPU
          if((ptr->no == s->no)&&(ptr->multicpuno == s->multicpuno))
         #else
          if(ptr->no == s->no)
         #endif
          {
           #ifdef SA33569
            if(CheckParameterFilter(p, s, ptr) == _TRUE)
           #endif 
            {
//            if(((s->state & 1) == 0)&&((ptr->state & 1) != 0))
              if(((s->state & 1) == 0)&&((ptr->state & 1) != 0)&&(s->entrytype == ptr->entrytype)) // wunsch applikation (friher)
              {
                p->xchange   += 1;
                ptr->state   &= 0xFE;
                ptr->time_off = s->time_off;
                ptr->date_off = s->date_off;
                retcode       = _TRUE;
                Insert_SINGLEALARM(p, ptr); // bestehenden alarm in ramfile verändern
                break;
              }
// bei km kann alarm mehrmals aktiv werden ohne je passiv zu sein (rauand)
//            else if((s->state != 0)&&(ptr->state == 0))
              else if((s->state & 1) != 0)
              {
                if(p->filter & BIT_USECOUNTER)
                {
                  p->xchange      += 1;
                  ptr->state      |= 1;
                  ptr->cnt        += 1;
                  ptr->acknowledge = 0;
                  retcode          = _TRUE;
                  Insert_SINGLEALARM(p, ptr); // bestehenden alarm in ramfile verändern
                }
                break;
              }
            }
          }
        }

        if((retcode == _FALSE)&&((s->state & 1) == 0))
        {
          // inaktiven alarm nicht eintragen, wenn kein passender aktiver gefunden
          return(_TRUE);
        }
      }
      else if(NoDelActiveAnd2Line(p->filter) == _TRUE)
      {
        {// sa7891: "2 zeilen modus" und "can't del avtive alarm" bits sind gesetzt
          if((s->state & 1) == 0) // alarm ist inaktiv geworden
          {
            // aktive zeile suchen und status auf "doch löschen" setzen
            no = no_RINGBUFFER(&p->ringbuffer);
            for(i=0; i<no; i++)
            {
              ptr = &p->ptr[rpos_RINGBUFFER(&p->ringbuffer, i)];
              if(ptr->no == s->no)
              {
                if((ptr->state & 3) == 1)
                {
                  ptr->state |= 2;  // status auf "aktiv, kann jedoch gelöscht werden" setzen
                  Insert_SINGLEALARM(p, ptr);  // ggf. änderung in file schreiben
                }
              }
            }
          }
        }
      }
    }

    if(retcode == _FALSE)
    {
      // alarm wird in liste neu eingetragen
      i           = wpos_RINGBUFFER(&p->ringbuffer);
      p->xchange += 1;
      ptr         = &p->ptr[i];
      free_SINGLEALARM(ptr);
      *ptr        = *s;
      
      if(ptr->entrytype != 0xFFFE)
        ptr->cnt  = 1;
        
      retcode     = _TRUE;

      // alarm in ringfile neu eintragen
      ptr->seqno  = 0xFFFFFFFFL;
      Insert_SINGLEALARM(p, ptr);
    }

    if((retcode == _TRUE)&&(s->entrytype == 0xFFFF))
      active_MEM_SINGLEALARM(p);
  }

  return(retcode);
}

_BOOL _TermAlarm::Save(void)
{
  CacheFinalize();
  HistoryDPNE[0] = 0; // merker für filenamen zerstören
  return(History.File.Close());
}

_BOOL _TermAlarm::Load(void)
{
  _BOOL        retcode = _FALSE;
  _UBYTE       data[512];
  _UDWORD      version, i, anzahl, seqmin, seqmax, begin;
  _SINGLEALARM *psa;
  _File        XFile;
  _ASCII       *hptr;
  _ASCII       dpne[_MAX_PATHLENGTH];
  _ASCII       xdpne[_MAX_PATHLENGTH];
  _UWORD       bs;
 #if HISTORYFILEVERSION >= 1
  _CRC32       crc;
  _UWORD       convert;
 #endif
  _UWORD       last_error = 0;

  if(Project.MakeConfigDPNE(dpne, HISTORYFILENAME, _FALSE) == _TRUE)
  {
    if((History.ptr != NULL)&&(History.ringbuffer.no > 0))
    {
      hptr = AlarmDPNE(&History, dpne);
      if(hptr != dpne)
        StrCpy(dpne, sizeof(_ASCII), hptr, sizeof(_ASCII));

      StrCpy(xdpne, sizeof(_ASCII), "RAM@", sizeof(_ASCII));
      StrCat(xdpne, sizeof(_ASCII), dpne, sizeof(_ASCII));

     #if HISTORYFILEVERSION >= 1
      convert = 0;
     #endif

      if(XFile.Open(LOAD, xdpne) == _TRUE) // kontrolle ob file bereits vorhanden
      {
        // file bereits vorhanden
        if((XFile.Read_HEADER(&version, HISTORYFILEID, _TRUE) == _TRUE)&&(version <= HISTORYFILEVERSION))
        {
          if(XFile.Read(data, 512-XFile.GetHeaderSize()) == _TRUE)
          {
            // startindex und blockgrösse überprüfen
            if((*(_UDWORD*)&data[4] == 512)&&(*(_UDWORD*)&data[8] == 0))
            {
              // anzahl einträge im ringpuffer und file überprüfen
              if(History.ringbuffer.no == *(_UDWORD*)&data[0])
              {
               #if HISTORYFILEVERSION >= 1
                crc = *(_UDWORD*)&data[12];
                if(version != HISTORYFILEVERSION)
                  convert |= 0x0001;
                else if(crc != Alarm.GetLseCrc32())
                  convert |= 0x0002;
               #endif

                bs      = ScanBar.Add("Load History:", History.ringbuffer.no);
                retcode = _TRUE;
                seqmin  = 0xFFFFFFFFL;
                seqmax  = 0;
                anzahl  = 0;
                begin   = 0;
                psa     = History.ptr;
                for(i=0; i<History.ringbuffer.no; i++)
                {
                  ScanBar.Run(bs, i);

                  if(XFile.Read_SINGLEALARM(psa, version) == _TRUE)
                  {
                   #if HISTORYFILEVERSION >= 1
                    if(convert & 0x0001)
                    {
                      Alarm.InsertAlarmIdByIndex(psa);
                    }
                    else if(convert & 0x0002)
                    {
                      Alarm.InsertIndexByAlarmId(psa);
                    }
                   #endif
                    if(psa->seqno != 0xFFFFFFFFL)
                    {
                      anzahl += 1;
                      if(psa->seqno < seqmin)
                      {
                        seqmin = psa->seqno;
                      }
                      if(psa->seqno > seqmax)
                      {
                        seqmax = psa->seqno;
					              begin  = i + 1;
                      }
                      psa += 1;
                    }
                  }
                  else
                  {
                    retcode = _FALSE;
                    anzahl  = 0;
                    last_error = 14; // unbekannter fehler im file !!
                    break;
                  }
//                  psa += 1;
                }
                ScanBar.End(bs);

                if(anzahl > 0)
                {
                  History.seqno            = seqmax + 1;
                  History.ringbuffer.used  = _TRUE;
                  if(begin >= anzahl)
                    begin = 0;
                  History.ringbuffer.begin = begin;
                  History.ringbuffer.pos   = (History.ringbuffer.begin + anzahl) % History.ringbuffer.no;
                }
                else
                {
                  History.seqno            = 0;
                  History.ringbuffer.used  = _FALSE;
                  History.ringbuffer.begin = 0;
                  History.ringbuffer.pos   = 0;
                }
              }
            }
          }
        }

        XFile.Close();
      }

     #if HISTORYFILEVERSION >= 1
      START_OPEN_HISTFILE:
     #endif

      if(retcode == _FALSE) // file noch nicht vorhanden
      {
       #if HISTORYFILEVERSION >= 1
        convert = 0;
       #endif
        retcode = XFile.Create_RINGFILE(xdpne, History.ringbuffer.no, HISTORYFILEID, HISTORYFILEVERSION, Alarm.GetLseCrc32());
      }

      if(retcode == _TRUE) // vorhandenes file für weitere bearbeitung öffnen
        retcode = History.File.Open(LOADSAVE, dpne);

      if(retcode == _TRUE) // filname für vorhandenes file merken
      {
       #if HISTORYFILEVERSION >= 1
        if(convert != 0)
        {
          // wegen konvertierung alle alarme neu speichern
          // header speichern
          History.File.Write_RINGFILEHEADER(History.ringbuffer.no, HISTORYFILEID, HISTORYFILEVERSION, Alarm.GetLseCrc32());
          // einträge speichern
          save_all_file_entry(&History, "Update History:");

          if(History.File.GetError() == _TRUE)
          {
            // shit happens
            History.File.Close();
            retcode = _FALSE;
            goto START_OPEN_HISTFILE;
          }
        }
       #endif
        StrCpy(HistoryDPNE, sizeof(_ASCII), dpne, sizeof(_ASCII));
      }
    }
  }

  if(History.filter & BIT_POWER_ON) // sa39254
  {  
    if(History.ptr != NULL)
    {
      _SINGLEALARM sa;
      init_SINGLEALARM(&sa);
      sa.entrytype = 0xFFFC;
      
      _UDWORD tim, dat;
      
      GetActDateTime(&dat, &tim);
      sa.date_on = dat;
      sa.time_on = tim;
      
      add_SINGLEALARM(&History, &sa);
    }
  }  

  if(last_error != 0)
  {
    if(History.ptr != NULL)
    {
      _SINGLEALARM sa;
      init_SINGLEALARM(&sa);
      sa.entrytype = 0xFFFE;
      sa.cnt = last_error;
      
      _UDWORD tim, dat;
      
      GetActDateTime(&dat, &tim);
      sa.date_on = dat;
      sa.time_on = tim;
      
      add_SINGLEALARM(&History, &sa);
    }
  }

  return(retcode);
}

_CHAR *create_alarmtext(_SINGLEALARM *p, _BOOL longtext, _COLOR *col)
{
  _CHAR         *retcode = NULL;
  _SINGLE_ALARM *psa;
  _VARLABEL     data[10];
  _MEM_VARLABEL mvl;
  _UWORD        i;
  _ANYPARA      *ap;
  _MEM_VARLABEL *pmv;
  _UBYTE        no_para;


  no_para = p->no_para & 0x7F;

  if(p->anything.list == ALARMLIST)
  {
    psa = (_SINGLE_ALARM*)get_ANYTHING(&p->anything, NULL);
    if(psa != NULL)
    {
      pmv    = &psa->parameter;
      ap     = (longtext == _TRUE)? &psa->helptext : &psa->text;
      mvl.no = (no_para > pmv->no)? pmv->no : no_para;

      for(i=0; i<mvl.no; i++)
      {
        if(pmv->ptr[i].info[0].state == PARA_VAL)
        {
          data[i].no = 2;
          data[i].info[0].state = PARA_VAL;
          data[i].info[0].value = pmv->ptr[i].info[0].value;
          data[i].info[1].state = CONST_VAL;
          data[i].info[1].value = p->para[i];
        }
        else
        {
          data[i].no = 1;
          data[i].info[0].state = CONST_VAL;
          data[i].info[0].value = p->para[i];
        }
      }

      mvl.ptr = data;
//      retcode = get_text(&ap->anything, NULL, &mvl, _TRUE);

      if((psa->link.no > 0)&&(psa->link.info[0].state == VAR_VAL))
      {
        // short oder longtext definiert bei alarm im lse
        retcode = get_text_sub(&ap->anything, NULL, &ap->memvarlabel, _TRUE, psa->link.info[0].value);
        if(retcode == NULL)
        {
          // text1 von variable welche alarm definiert
          _ANYTHING vtx1;
          init_ANYTHING_Makro(&vtx1);
          vtx1.list = VARTXT1_OBJNAME;
          vtx1.no   = psa->link.info[0].value;
          retcode = get_text(&vtx1, NULL, &ap->memvarlabel, _TRUE);
        }
      }
      else
      {
        // short oder longtext definiert bei alarm im lse
        retcode = get_text(&ap->anything, NULL, &ap->memvarlabel, _TRUE);
      }

      if(retcode != NULL)
      {
        retcode = make_parameterstring(retcode, &mvl, _FALSE);
      }

      if(col != NULL)
      {
        if((p->state & 1) == 0)
          *col = (p->acknowledge == 0)? psa->passive : psa->quit_passive;
        else
          *col = (p->acknowledge == 0)? psa->active : psa->quit_active;
      }
    }
  }

  return(retcode);
}

void out_standard_alarmline(_SINGLEALARM *p, _ROOM *pr, _COLOR color, _UWORD filter, _SCROLL *ps)
{
  _CHAR  *ptr, *ptx, txt[300];
  _TIME  time;
  _DATE  date;
  _COLOR sug, bg, fg;
  _BOOL  backstate;

  ptr = txt;
  bg  = INVISIBLE;

  if(p->entrytype == 0xFFFC)
  {
    fg = GetPenColor_Makro(color);
    bg = GetBackColor_Makro(color);
    if(bg != INVISIBLE)
    {
      bg = LIGHTGREEN;
    }
    
    StrCpy(txt, sizeof(_CHAR), "POWER ON... ", sizeof(_ASCII));
    
    ptr = &txt[StrLen(txt, sizeof(_CHAR))];
    lasal_to_DATE(&date, p->date_on);
    format_date(ptr, &date, SIZE_DDMMYY);                         // date
    ptr += StrLen(ptr, sizeof(_CHAR));

    ptr[0] = ' '; ptr ++;
    lasal_to_TIME(&time, p->time_on);
    format_time(ptr, &time, SIZE_HHMMSS);                         // time
    ptr += StrLen(ptr, sizeof(_CHAR));
    ptr[0] = 0;
  }
  else if(p->entrytype == 0xFFFF)
  {
    DToA(ptr, p->no, 0x0800, sizeof(_CHAR));               // alarmnummer
    ptr += StrLen(ptr, sizeof(_CHAR));

   #ifdef UC_ALARMGROUPS                                    // gruppennummer
    _SINGLE_ALARM *psa;
    psa = Alarm.Get_SINGLE_ALARM(p->anything.no);
    if(psa != NULL)
    {
      if(p->para[3] != 0)
      {
        ptr[0] = ' ';
        ptr++;
        DToA(ptr, psa->group, 0x0800, sizeof(_CHAR));
        ptr += StrLen(ptr, sizeof(_CHAR));
      }
    }
   #endif

    if(filter & BIT_USECOUNTER)
    {
      ptr[0] = ' '; ptr ++;
      DToA(ptr, p->cnt, 0x0800, sizeof(_CHAR));                // counter
      ptr += StrLen(ptr, sizeof(_CHAR));
    }

    if(p->anything.no == 0xFFFF)
    {
      // unbekannter alarm wird erneut gesucht. problem wenn cpu später als
      // terminal hochläuft, werden bereits alarme geschickt bevor alarme im
      // terminal konfiguriert (bekannt) wurden (sa5168).
      p->anything.no = Alarm.GetIndex(p->no, NULL);
    }

    ptx = create_alarmtext(p, (filter & BIT_LONGTEXT)? _TRUE : _FALSE, &sug);

   #ifdef UC_MULTICPU
    if(p->multicpuno != 0)
    {
      ptr[0] = ' '; ptr += 1;
      ptr[0] = '['; ptr += 1;
      NumToStr(ptr, p->multicpuno, sizeof(_CHAR)); ptr += StrLen(ptr, sizeof(_CHAR));
      ptr[0] = ']'; ptr += 1;
    }
   #endif

    if(ptx != NULL)
    {
      ptr[0] = ' '; ptr += 1;
      StrCpy(ptr, sizeof(_CHAR), ptx, sizeof(_CHAR));        // alarmtext
      ptr += StrLen(ptr, sizeof(_CHAR));
    }
    else
    {
      StrCpy(ptr, sizeof(_CHAR), " unknown", sizeof(_ASCII));        // alarmtext
      
      sug = MakeColor_Makro(BLACK, WHITE);
     #ifndef UC_ALARMGROUPS                                    // gruppennummer
      _SINGLE_ALARM *psa;
      psa = Alarm.Get_SINGLE_ALARM(p->anything.no);
     #endif
      if(psa != NULL)
      {
        if(is_server_defined(&psa->link) == _TRUE)
        {
          _ANYTHING xany;
          init_ANYTHING_Makro(&xany);
          xany.list = VARTXT1;
          xany.no = psa->link.info[0].value;
          _CHAR *ptxt = get_text(&xany, NULL, NULL, _FALSE);
          
          if(ptxt != NULL)
          {
            *ptr = ' ';
            StrCpy(ptr+1, sizeof(_CHAR), ptxt, sizeof(_CHAR));  // servername
            StrCat(ptr, sizeof(_CHAR), (p->state == 0)? " (lo)":" (hi)" , sizeof(_ASCII));
          }
          
//          _ASCII arrunk[256];
//          arrunk[0] = ' ';
//          _UBYTE varobj = 0;
//          
//          if(VarList.GetLabel(&arrunk[1], &varobj, psa->link.info[0].value) == _TRUE)      // get name of variable
//          {
//            StrCpy(ptr, sizeof(_CHAR), arrunk, sizeof(_ASCII));  // servername
//            StrCat(ptr, sizeof(_CHAR), (p->state == 0)? " (lo)":" (hi)" , sizeof(_ASCII));
//          }
        }
      }
      
      ptr += StrLen(ptr, sizeof(_CHAR));
    }

    ptr[0] = ' '; ptr ++;
    lasal_to_DATE(&date, p->date_on);
    format_date(ptr, &date, SIZE_DDMMYY);                         // date
    ptr += StrLen(ptr, sizeof(_CHAR));

    ptr[0] = ' '; ptr ++;
    lasal_to_TIME(&time, p->time_on);
    format_time(ptr, &time, SIZE_HHMMSS);                         // time
    ptr += StrLen(ptr, sizeof(_CHAR));

    if(filter & BIT_ONOFF)
    {
      if((p->state & 1) == 0)
      {
        ptr[0] = ' '; ptr ++;
        lasal_to_DATE(&date, p->date_off);
        format_date(ptr, &date, SIZE_DDMMYY);                       // date
        ptr += StrLen(ptr, sizeof(_CHAR));

        ptr[0] = ' '; ptr ++;
        lasal_to_TIME(&time, p->time_off);
        format_time(ptr, &time, SIZE_HHMMSS);                       // time
//      ptr += StrLen(ptr, sizeof(_CHAR));
      }
      else
      {
        StrCpy(ptr, sizeof(_CHAR), " --.--.-- --:--:--", sizeof(_ASCII));
      }
    }

    bg = GetBackColor_Makro(color);
    fg = GetPenColor_Makro(color);

    if(fg != INVISIBLE)
      fg = GetPenColor_Makro(sug);

    if(bg != INVISIBLE)
    {
      bg = GetBackColor_Makro(sug);
//      if(bg != INVISIBLE)
//      {
//        backstate = _FALSE;
//
//        if(ps != NULL)
//        {
//          if(ps->frametype >= _THEMEFRAME)
//          {
//            backstate = _TRUE;
//            DrawButton(&ps->out_room, T_FILL|T_COPY|T_SOLID|((ps->attrib_activ & T_INVERTTHEME)?T_INVRS:0), ps->frametype, MakeColor_Makro(bg, bg), MakeColor_Makro(bg, bg));
//          }
//        }
//
//        if(backstate == _FALSE)
//          Bar(pr->xy1.x, pr->xy1.y, pr->xy2.x, pr->xy2.y, T_COPY|T_SOLID, bg);
//      }
    }
  }
  else
  {
    StrCpy(ptr, sizeof(_CHAR), "unknown line-format", sizeof(_ASCII));
    fg = GetPenColor_Makro(color);
    
    if(p->entrytype == 0xFFFE)
    {
      lasal_to_DATE(&date, p->date_on);
      format_date(ptr, &date, SIZE_DDMMYY); // date
      ptr += StrLen(ptr, sizeof(_CHAR));
      ptr[0] = ' '; ptr ++;
      
      lasal_to_TIME(&time, p->time_on);
      format_time(ptr, &time, SIZE_HHMMSS); // time
      ptr += StrLen(ptr, sizeof(_CHAR));
      ptr[0] = ' '; ptr ++;
    
      switch(p->cnt)
      {
        case 13 : StrCpy(ptr, sizeof(_CHAR), "ERROR: corrupt logfileentry detected", sizeof(_ASCII));
                  break;
        case 14 : StrCpy(ptr, sizeof(_CHAR), "ERROR: corrupt logfile detected", sizeof(_ASCII));
                  break;
        default : StrCpy(ptr, sizeof(_CHAR), "ERROR: unknown", sizeof(_ASCII));
                  break;
      }
    }
  }


      if(bg != INVISIBLE)
      {
        backstate = _FALSE;

        if(ps != NULL)
        {
          if(ps->frametype >= _THEMEFRAME)
          {
            backstate = _TRUE;
            DrawButton(&ps->out_room, T_FILL|T_COPY|T_SOLID|((ps->attrib_activ & T_INVERTTHEME)?T_INVRS:0), ps->frametype, MakeColor_Makro(bg, bg), MakeColor_Makro(bg, bg));
          }
        }

        if(backstate == _FALSE)
        {
          Bar(pr->xy1.x, pr->xy1.y, pr->xy2.x, pr->xy2.y, T_COPY|T_SOLID, bg);
        }
      }


  OutTextXY(pr->xy1.x+3, pr->xy1.y, txt, T_COPY|T_SOLID|T_PROP, MakeColor_Makro(INVISIBLE, fg), sizeof(_CHAR));
}

_MEM_SINGLEALARM* _TermAlarm::Get_MEM_SINGLEALARM(_BOOL st)
{
  return((st == _TRUE)? &History : &Temporary);
}

#ifndef UC_NEW_ALARMSYSTEM
void decode_timedate(_IVALUE *time, _IVALUE *date, _UDWORD dat)
{
  _DATIM datim;


  init_DATIM(&datim);

  datim.fdate.year = 2000;

  datim.fdate.month = dat/(100*32*86400);
  dat -= (datim.fdate.month*100*32*86400);
  datim.fdate.day = dat/(100*86400);
  dat -= (datim.fdate.day*100*86400);
  datim.ftime.hour = dat/(100*60*60);
  dat -= (datim.ftime.hour*100*60*60);
  datim.ftime.minute = dat/(100*60);
  dat -= (datim.ftime.minute*100*60);
  datim.ftime.second = dat / 100;
  dat -= (datim.ftime.second*100);

  DATE_to_lasal(date, &datim.fdate);
  TIME_to_lasal(time, &datim.ftime);
  *time |= dat & 0x00FF;
}
#endif

#ifndef UC_NEW_ALARMSYSTEM
_SINGLEALARM XXPuffer[TERMALARMPUFFER];
#endif

void _TermAlarm::Run(void)
{
  _UWORD i;

 #ifdef UC_NEW_ALARMSYSTEM
  if(timediff(AlaTimer) > 1013)
  {
    AlaTimer = get_abstime();
    _COMSET *pcs = Batch.GetComSet();
    if(pcs != NULL)
    {
      for(i=0; i<pcs->no; i++)
      {
        getAlarms(&pcs->ptr[i], &Temporary, &History);
      }
    }
  }
 #else
  _DATE        date;
  _UWORD       data[20];
  _SINGLEALARM *tmp;
  _SINGLEALARM res, res1;

  if(Queue == _TRUE)
  {
    Mutex(_TRUE, MUTEX_ALARM);
    MemCpy(XXPuffer, Puffer, sizeof(Puffer));
    Queue    = _FALSE;
    for(i=0; i<TERMALARMPUFFER; i++)
    {
      if(Puffer[i].acknowledge == 2)
      {
        Puffer[i].acknowledge = 0;
      }
    }
    Mutex(_FALSE, MUTEX_ALARM);

    Sys_GetTime(data);
    tmp = XXPuffer;

    for(i=0; i<TERMALARMPUFFER; i++)
    {
      if(tmp->acknowledge == 2)
      {
        res = *tmp;
        init_SINGLEALARM(tmp);

        decode_timedate(&res.time_on, &res.date_on, res.time_on); // neu
        res.time_off = res.time_on; // neu

        lasal_to_DATE(&date, res.date_on);
        date.year = data[0];                        // insert year
        DATE_to_lasal(&res.date_on, &date);
        res.date_off = res.date_on;

        res.acknowledge   = 0;
        res.anything.list = ALARMLIST;

       #ifdef UC_ALARMGROUPS
        if((res.no_para & 0x80) && (res.para[3] != 0))
        {
          res.anything.no   = Alarm.GetIndexByGroup(res.para[3]);
        }
        else
       #endif

        res.anything.no   = Alarm.GetIndex(res.no, NULL);
       #if HISTORYFILEVERSION >= 1
        res.alarmid       = Alarm.InsertAlarmIdByIndex(&res);
       #endif
        res.entrytype     = 0xFFFF;

        res1 = res;

        AlarmDisplayAddEntry(&History, &res);
        AlarmDisplayAddEntry(&Temporary, &res1);
      }
      tmp += 1;
    }
  }
 #endif

  CacheRun();
}

#ifdef UC_MULTICPU
_SINGLEALARM* _TermAlarm::SeekPuffer(_DWORD no, _UWORD multicpuno)
#else
_SINGLEALARM* _TermAlarm::SeekPuffer(_DWORD no)
#endif
{
  _UWORD       i;
  _SINGLEALARM *ptr;

  ptr = Puffer;
  if(no != -1)
  {
    for(i=0; i<TERMALARMPUFFER; i++)
    {
     #ifdef UC_MULTICPU
      if((ptr->acknowledge == 1)&&(ptr->no == no)&&(ptr->multicpuno == multicpuno))
        return(ptr);
     #else
      if((ptr->acknowledge == 1)&&(ptr->no == no))
        return(ptr);
     #endif
      ptr += 1;
    }
  }
  else
  {
    for(i=0; i<TERMALARMPUFFER; i++)
    {
      if(ptr->acknowledge == 0)
        return(ptr);
      ptr += 1;
    }
  }

  return(NULL);
}

_BOOL _TermAlarm::Store_ALACELL(_ALACELL *p)
{
  _SINGLEALARM *tmp;
  _UWORD       i, ino;
  _BOOL        retcode = _FALSE;
 #ifdef UC_MULTICPU
  _UBYTE       multicpuno;
 #endif

  Mutex(_TRUE, MUTEX_ALARM);

  ino   = (p->index >> 4) & 0x0F;    // number of frames (index) 0 = 1 frame
  i     = (p->index & 0x0F);         // actual frame
 #ifdef UC_MULTICPU
  multicpuno = (p->kennung >> 2) & 0x3F;
 #endif

  if(i == 0) // new alarm
  {
   #ifdef UC_MULTICPU
    tmp = SeekPuffer(-1, multicpuno);
   #else
    tmp = SeekPuffer(-1);
   #endif
    if(tmp != NULL)
    {
      init_SINGLEALARM(tmp);
      tmp->no          = p->no;
      tmp->time_on     = p->info;
      tmp->state      |= p->kennung & 1;
     #ifdef UC_MULTICPU
      tmp->multicpuno  = multicpuno;
     #endif
      tmp->acknowledge = 1;
    }
  }
  else
  {
   #ifdef UC_MULTICPU
    tmp = SeekPuffer(p->no, multicpuno);
   #else
    tmp = SeekPuffer(p->no);
   #endif
    if(tmp != NULL)
    {
      if((i<=ino)&&(i<=6))
      {
        tmp->para[i-1] = p->info;
        if(tmp->no_para < i)
          tmp->no_para = i;
      }
    }
  }

  if(tmp != NULL)
  {
    retcode = _TRUE;
    if(i == ino)
    {
      tmp->acknowledge = 2;
      Queue = _TRUE;

     #ifdef UC_ALARMGROUPS
      if(tmp->no_para >= 5)
      {
        tmp->no_para |= 0x80;

        _SINGLE_ALARM *psa;
        _LSEVARINFO *pvi;
        _UDWORD varno;
        _ASCII label[MAX_LABELLEN];
        _BOOL obj;

        psa = Alarm.Get_SINGLE_ALARM(Alarm.GetIndexByGroup(tmp->para[3]));
        if(psa != NULL)
        {
          if(VarList.GetVarInfo(&pvi, psa->link.info[0].value) == _TRUE)
          {
            varno = VarList.GetVarNoByID(tmp->para[4], pvi->station, _TRUE);
            if(varno != 0xFFFFFFFF)
            {
              obj = _FALSE;
              if(VarList.GetLabel(label, &obj, varno) == _TRUE)
              {
                tmp->para[4] = MakeCrc32(pvi->station, label, StrLen(label, sizeof(_ASCII)));
              }
            }
          }
        }
      }

     #endif
    }
  }

  Mutex(_FALSE, MUTEX_ALARM);

  return(retcode);
}

_BOOL add_SINGLEALARM(_MEM_SINGLEALARM *p, _SINGLEALARM *s)
{
  _BOOL retcode = _FALSE;

  if((p != NULL)&&(s != NULL))
  {
    retcode = TermAlarm.Insert_MEM_SINGLEALARM(p, s);
  }

  return(retcode);
}

_UDWORD active_MEM_SINGLEALARM(_MEM_SINGLEALARM *p)
{
/*
  // schneller
  _UDWORD      retcode = 0;
  _UDWORD      begin, anz1, anz2, i;
  _SINGLEALARM *ptr;

  begin = teiler_RINGBUFFER(&p->ringbuffer, &anz1, &anz2);

  ptr = p->ptr;
  for(i=0;i<anz1; i++, ptr++)
  {
    if((ptr->state & 1) != 0)
      retcode += 1;
  }
  ptr = &p->ptr[begin];
  for(i=0;i<anz2; i++, ptr++)
  {
    if((ptr->state & 1) != 0)
      retcode += 1;
  }

  return(retcode);
*/

  _UDWORD no, i, retcode = 0;

  no = no_RINGBUFFER(&p->ringbuffer);
  for(i=0; i<no; i++)
  {
    if((p->ptr[rpos_RINGBUFFER(&p->ringbuffer, i)].state & 1) != 0)
      retcode += 1;
  }

  p->noactive = retcode;

  return(retcode);

}

_BOOL alarm_save(void)
{
  return(TermAlarm.Save());
}


#ifdef UC_NEW_ALARMSYSTEM
_UBYTE NewAlaPuffer[5000];
_UBYTE *getNewInstData(_UWORD *plen, _UBYTE *rx)
{
  _UBYTE *retcode = NULL;
  _UWORD len;

  len = *(_UWORD*)rx;

  if((len > 1) && (len != 0xFFFF))
	{
		len -= 2;
    MemCpy(NewAlaPuffer, rx+2, len);
		retcode = NewAlaPuffer;
    if(plen != NULL)
      *plen = len;
	}

  return(retcode);
}
#ifdef UC_UNREGISTERED_ALARM

_UDWORD _Alarm::GetUnreg(_COMPORT *pcp, _UWORD station, _UDWORD startindex, _UWORD scanbar)
{
  _UDWORD retcode = 0;
  _UBYTE  tx[20], *rx, *data;
  _UWORD  length;
  _UDWORD anz, varno;
  _UDWORD *plid;

  if(pcp->alarmVersion >= 2)
  {
    *(_UDWORD*)&tx[0]  = pcp->alarmObject;
    *(_UWORD*) &tx[4]  = COMMAND_GETUNREGID;
    *(_UBYTE*) &tx[6]  = P_IMMED;
    *(_UDWORD*)&tx[7]  = pcp->alarmHandle;
    *(_UBYTE*) &tx[11] = P_COMMA;
    *(_UBYTE*) &tx[12] = P_IMMED;
    *(_UDWORD*)&tx[13] = startindex;
    *(_UBYTE*) &tx[17] = P_EOL;

    if(TxCommandIntern(I_CMD, 18, tx, &pcp->comdef, (void**)&rx, DEFSVRTYP) == DONE)
    {
      data = getNewInstData(&length, rx);
      if((data != NULL) && (length >= 9))
      {
        anz        = data[4];
        plid       = (_UDWORD*)&data[5];
        startindex = *(_UDWORD*)&data[0];
        
        if(anz > 0)
        {
          ScanBar.Reset(scanbar, anz, _TRUE);
          for(_UDWORD i=0; i<anz; i++, plid++)
          {
            ScanBar.Run(scanbar, i);
            varno = VarList.GetVarNoByID(*plid, station, _TRUE);    // get varno by lasalid
            if(varno != DEFSCOPE)
            {
              // neuen eintrag zu alarmliste von lse und diesen neuen alarm auch configurieren
              // vorsicht COMMAND_GETALARMCONFIGEX anstelle COMMAND_GETALARMCONFIG verwenden
              if(Add(varno, _TRUE) == _TRUE)
                retcode += 1;
            }
          }
        }
        
        if(startindex != 0xFFFFFFFF)
          retcode += GetUnreg(pcp, station, startindex, scanbar);
      }
    }
  }
  
  return retcode;
}

void _Alarm::DoUnregisteredAlarms(_UWORD scanbar)
{
  _COMPORT *pcp;
  _UWORD   station;
  
  for(station=0; station<256; station++)
  {
    pcp = Batch.Get_COMPORT(station);
    if(pcp != NULL)
      GetUnreg(pcp, station, 0, scanbar);
  }
}
#endif

_BOOL setupAlarmObject(_COMPORT *pcp)
{
  _BOOL   retcode = _FALSE;
  _UBYTE  tx[20], *rx, *data;
  _UWORD  length;
  _UDWORD pobj = 0;
  
  if(pcp->alarm_disable == _FALSE)
  {
    pobj = PlcGetLasalId(&pcp->comdef, (_ASCII*)ALARMBUFFERNAME, NULL);
  }
  
  if(pobj != 0)
  {
    *(_UDWORD*)&tx[0]  = pobj;
    *(_UWORD*) &tx[4]  = COMMAND_REGISTERHMI; //COMMAND_SETID;
    *(_UBYTE*) &tx[6]  = P_IMMED;
    *(_UDWORD*)&tx[7]  = GetUniquePlcId(); //UC_NEW_ALARMSYSTEM;
    *(_UBYTE*) &tx[11] = P_COMMA;
    *(_UBYTE*) &tx[12] = P_IMMED;
    *(_UDWORD*)&tx[13] = 0;
    *(_UBYTE*) &tx[17] = P_EOL;
       

    // in:   [4] ... unique id of hmi
    //       [4] ... hmi style (1 = seperate set of index counter)
        
    // out:  [4] ... handle to get access (error = 16#FFFFFFFF)
    //       [4] ... index of last validated element
    //       [4] ... number of buffered elements
    //       [4] ... size of single element in byte
    //       [4] ... max. count of entries in buffer

    if(TxCommandIntern(I_CMD, 18, tx, &pcp->comdef, (void**)&rx, DEFSVRTYP) == DONE)
    {
      data = getNewInstData(&length, rx);
      if((data != NULL) && (length >= 20))
      {
        if(*(_UDWORD*)&data[0] != 0xFFFFFFFF)
        {
          pcp->alarmObject = pobj;
          pcp->alarmHandle  = *(_UDWORD*)&data[0];
          pcp->alarmValidIndex = *(_UDWORD*)&data[4];
          pcp->alarmAnzahl = *(_UDWORD*)&data[8];
          pcp->alarmRecordSize = *(_UDWORD*)&data[12];
          pcp->alarmXBufferSize = *(_UDWORD*)&data[16];
           
           
          // get version of plc - _AlarmXBuffer
          *(_UDWORD*)&tx[0]  = pobj;
          *(_UWORD*) &tx[4]  = COMMAND_GETVERSION;
          *(_UBYTE*) &tx[6]  = P_EOL;              
          if(TxCommandIntern(I_CMD, 7, tx, &pcp->comdef, (void**)&rx, DEFSVRTYP) == DONE)
          {
            pcp->alarmVersion = *(_UDWORD*)&rx[2];
          }
              
          retcode = _TRUE;
        }
      }
    }
  }

  return(retcode);
}

void convert_date_time(_DWORD *date, _DWORD *time, _UDWORD datetime)
{
// YY MM DD HH MM SS
//  6  4  5  5  6  6

// JJJJJJJJJJJJJJJJ MMMMTTTTTTTTWWWW
// HHHHHHHHMMMMMMMM SSSSSSSS00000000

  if(date != NULL)
  {
    *date = (((datetime >> 26) & 0x3F) + 2000) << 16;
    *date |= ((datetime >> 22) & 0x0F) << 12;
    *date |= ((datetime >> 17) & 0x1F) << 4;
//    *date |= 1; // hf
  }

  if(time != NULL)
  {
    *time = 0;
    *time |= ((datetime >> 12) & 0x1F) << 24;
    *time |= ((datetime >> 6) & 0x3F) << 16;
    *time |= (datetime & 0x3F) << 8;
  }
}

_BOOL check_newalarm(_IVALUE *dest, _COMPORT *pcp)
{
  _UBYTE tmp[100];
  _UBYTE *result;

  if(pcp != NULL)
  {
    if(pcp->alarmObject != NULL)
    {
      *(_UBYTE*)&tmp[0]  = P_VARIA;
      *(_UDWORD*)&tmp[1] = pcp->alarmObject;
      *(_UBYTE*)&tmp[5]  = P_EOL;

      if(TxCommandIntern(I_READ, 8, tmp, &pcp->comdef, (void**)&result, DEFSVRTYP) == DONE)
      {
        result += 2;
        *(_UDWORD*)dest = *(_UDWORD*)result; //MemCpy(dest, result, 4);
        return(_TRUE);
      }
    }
  }
  return(_FALSE);
}

void AlarmDisplayQuitEntry(_MEM_SINGLEALARM *pdst, _SINGLEALARM *psa, _BOOL history)
{
  _UDWORD      no, i;
  _SINGLEALARM *ps;

  no = no_RINGBUFFER(&pdst->ringbuffer);

  for(i=0; i<no; i++)
  {
    ps = &pdst->ptr[rpos_RINGBUFFER(&pdst->ringbuffer, i)];
    if((ps->no == psa->no)&&(ps->acknowledge == 0))
    {
      ps->acknowledge = 1;
      if(history == _TRUE)
        TermAlarm.Insert_SINGLEALARM(pdst, ps);  
      pdst->xchange  += 1;                               // redraw whole page
    }
  }
}


static void ProcessAlarmRecords(_COMPORT *pcp, _UBYTE *data, _UDWORD anzahl, _MEM_SINGLEALARM *ptemp, _MEM_SINGLEALARM *phist)
{
// _8D325
  _SINGLEALARM sa, sa1;
  _UBYTE       tmp;

  while(anzahl)
  {
    anzahl -= 1;
    init_SINGLEALARM(&sa);
   #ifdef SA34706
    sa.no = *(_UDWORD*)data; data += 4; // 32bit alarmnummer
   #else
    sa.no = *(_UWORD*)data; data += 4; // nur das untere word beinhaltet die alarmnummer, SA23599: auf 32 bit erweitern
   #endif 
    convert_date_time(&sa.date_on, &sa.time_on, *(_UDWORD*)data); data += 4;
    sa.date_off = sa.date_on;
    sa.time_off = sa.time_on;

    tmp = *(_UBYTE*)data; data += 1;
    if(tmp == 255)
    {
      sa.state   = 255;
      sa.no_para = 0; // alarm wurde quittiert
    }
    else
    {
      sa.state   = tmp & 1;
      sa.no_para = (tmp & 0x7E) >> 1;

      if(tmp & 0x80)
      {
       #ifdef UC_ALARMGROUPS
        _SINGLE_ALARM *psa;
        _LSEVARINFO *pvi;
        _UDWORD varno;
        _ASCII label[MAX_LABELLEN];
        _BOOL obj;

        sa.para[3] = *(_UDWORD*)data;
        data   += 4;

        psa = Alarm.Get_SINGLE_ALARM(Alarm.GetIndexByGroup(sa.para[3]));
        if(psa != NULL)
        {
          if(VarList.GetVarInfo(&pvi, psa->link.info[0].value) == _TRUE)
          {
            varno = VarList.GetVarNoByID(*(_UDWORD*)data, pvi->station, _TRUE);
            if(varno != 0xFFFFFFFF)
            {
              obj = _FALSE;
              if(VarList.GetLabel(label, &obj, varno) == _TRUE)
              {
                sa.para[4] = MakeCrc32(pvi->station, label, StrLen(label, sizeof(_ASCII)));
              }
            }
          }
        }

        data += 4;
       #else
        data += 8;
       #endif
      }
    }

    for(tmp=0; tmp<sa.no_para; tmp++)
    {
      sa.para[tmp] = *(_UDWORD*)data;
      data += 4;
    }

    sa.acknowledge = 0;

   #ifdef UC_ALARMGROUPS
    if(sa.para[3] != 0)
    {
      sa.no_para |= 0x80;
      sa.anything.no = Alarm.GetIndexByGroup(sa.para[3]);
    }
    else
    {
      sa.anything.no = Alarm.GetIndex(sa.no, pcp);
    }
   #else
    sa.anything.no = Alarm.GetIndex(sa.no, pcp);
   #endif

   #if HISTORYFILEVERSION >= 1
    sa.alarmid = Alarm.InsertAlarmIdByIndex(&sa);
   #endif
    sa.entrytype = 0xFFFF;
    sa.anything.list = ALARMLIST;

    sa1 = sa;
    if(sa.state == 255)
    {
      // alarm wurde quittiert
      if(ptemp != NULL)
        AlarmDisplayQuitEntry(ptemp, &sa, _FALSE);
      if(phist != NULL)
        AlarmDisplayQuitEntry(phist, &sa1, _TRUE);
    }
    else
    {
      if(ptemp != NULL)
        AlarmDisplayAddEntry(ptemp, &sa);
      if(phist != NULL)
        AlarmDisplayAddEntry(phist, &sa1);
    }
  }
}

static void ProcessAlarmRecords202(_COMPORT *pcp, _UBYTE *data, _UDWORD anzahl, _MEM_SINGLEALARM *ptemp, _MEM_SINGLEALARM *phist, _UDWORD recordsize)
{
  // _8D325
  _SINGLEALARM sa, sa1;
  _UBYTE       tmp;
  _UBYTE       *ph = data;

  while(anzahl)
  {
    anzahl -= 1;
    init_SINGLEALARM(&sa);
    sa.no = *(_UWORD*)data; data += 4; // nur das untere word beinhaltet die alarmnummer
    convert_date_time(&sa.date_on, &sa.time_on, *(_UDWORD*)data); data += 4;
    sa.date_off = sa.date_on;
    sa.time_off = sa.time_on;

    tmp = *(_UBYTE*)data; data += 1;
    if(tmp == 255)
    {
      sa.state   = 255;
      sa.no_para = 0; // alarm wurde quittiert
    }
    else
    {
      sa.state   = tmp & 1;
      sa.no_para = (tmp & 0x7E) >> 1;

      if(tmp & 0x80)
      {
       #ifdef UC_ALARMGROUPS
        _SINGLE_ALARM *psa;
        _LSEVARINFO *pvi;
        _UDWORD varno;
        _ASCII label[MAX_LABELLEN];
        _BOOL obj;

        sa.para[3] = *(_UDWORD*)data;
        data   += 4;

        psa = Alarm.Get_SINGLE_ALARM(Alarm.GetIndexByGroup(sa.para[3]));
        if(psa != NULL)
        {
          if(VarList.GetVarInfo(&pvi, psa->link.info[0].value) == _TRUE)
          {
            varno = VarList.GetVarNoByID(*(_UDWORD*)data, pvi->station, _TRUE);
            if(varno != 0xFFFFFFFF)
            {
              obj = _FALSE;
              if(VarList.GetLabel(label, &obj, varno) == _TRUE)
              {
                sa.para[4] = MakeCrc32(pvi->station, label, StrLen(label, sizeof(_ASCII)));
              }
            }
          }
        }

        data -= 4;
       #endif
      }
      
      data += 8;
      
      for(tmp=0; tmp<sa.no_para; tmp++)
      {
        sa.para[tmp] = *(_UDWORD*)data;
        data += 4;
      }
    }

    sa.acknowledge = 0;

   #ifdef UC_ALARMGROUPS
    if(sa.para[3] != 0)
    {
      sa.no_para |= 0x80;
      sa.anything.no = Alarm.GetIndexByGroup(sa.para[3]);
    }
    else
    {
      sa.anything.no = Alarm.GetIndex(sa.no, pcp);
    }
   #else
    sa.anything.no = Alarm.GetIndex(sa.no, pcp);
   #endif

   #if HISTORYFILEVERSION >= 1
    sa.alarmid = Alarm.InsertAlarmIdByIndex(&sa);
   #endif
    sa.entrytype = 0xFFFF;
    sa.anything.list = ALARMLIST;

    sa1 = sa;
    if(sa.state == 255)
    {
      // alarm wurde quittiert
      if(ptemp != NULL)
        AlarmDisplayQuitEntry(ptemp, &sa, _FALSE);
      if(phist != NULL)
        AlarmDisplayQuitEntry(phist, &sa1, _TRUE);
    }
    else
    {
      if(ptemp != NULL)
        AlarmDisplayAddEntry(ptemp, &sa);
      if(phist != NULL)
        AlarmDisplayAddEntry(phist, &sa1);
    }
    
    ph  += recordsize;
    data = ph;
  }
}


void getAlarmsFrom(_COMPORT *pcp, _UDWORD start, _UDWORD anz, _MEM_SINGLEALARM *ptemp)
{
  _UBYTE  tx[30], *rx;
  _UBYTE  *data;
  _UDWORD tmpanz;
  _UWORD  sb;
  _BOOL   sbstate = _FALSE;
  _UDWORD zerocnt = 0;

  if(anz > 20)
  {
    sb = ScanBar.Add("AlarmGetTemp.:", anz); 
    sbstate = _TRUE;
  }

  while(anz)
  {
    *(_UDWORD*)&tx[0]  = pcp->alarmObject;
    *(_UWORD*) &tx[4]  = COMMAND_GETENTRY;
    *(_UBYTE*) &tx[6]  = P_IMMED;
    *(_UDWORD*)&tx[7]  = pcp->alarmHandle;
    *(_UBYTE*) &tx[11] = P_COMMA;
    *(_UBYTE*) &tx[12] = P_IMMED;
    *(_UDWORD*)&tx[13] = start;
    *(_UBYTE*) &tx[17] = P_COMMA;
    *(_UBYTE*) &tx[18] = P_IMMED;
    *(_UDWORD*)&tx[19] = anz;
    *(_UBYTE*) &tx[23] = P_EOL;

    if(TxCommandIntern(I_CMD, 24, tx, &pcp->comdef, (void**)&rx, DEFSVRTYP) == DONE)
    {
      data = getNewInstData(NULL, rx);
      if(data != NULL)
      {
        //idx    = *(_UDWORD*)&data[0];
        tmpanz = (*(_UDWORD*)&data[4]) & 0x7FFFFFFF;
        data  += 8;
        
        if(tmpanz == 0)
        {
          zerocnt += 1;
        }
        else
        {
          zerocnt = 0;
        }
        
        ProcessAlarmRecords202(pcp, data, tmpanz, ptemp, NULL, pcp->alarmRecordSize);  
        anz -= tmpanz;
        start += tmpanz;

        if(zerocnt > 9)
        {
          anz = 0; // abbruch
        }
        
        if(sbstate == _TRUE)
          ScanBar.Run(sb, start);
      }
      else
      {
        anz = 0;
      }
    }
    else
    {
      anz = 0;
    }
  }
  
  if(sbstate == _TRUE)
    ScanBar.End(sb);  
  
}

void getAlarms(_COMPORT *pcp, _MEM_SINGLEALARM *ptemp, _MEM_SINGLEALARM *phist)
{
  _UBYTE tx[20], *rx;
  _UBYTE *data;
  _UWORD anzahl;
  _IVALUE value;

  if(check_newalarm(&value, pcp) == _TRUE)
  {
    while(value != pcp->oldValue)
    {
      *(_UDWORD*)&tx[0]  = pcp->alarmObject;
      *(_UWORD*) &tx[4]  = COMMAND_GETALARMS;
      *(_UBYTE*) &tx[6]  = P_IMMED;
      *(_UDWORD*)&tx[7]  = pcp->alarmHandle;
      *(_UBYTE*) &tx[11] = P_EOL;

      if(TxCommandIntern(I_CMD, 12, tx, &pcp->comdef, (void**)&rx, DEFSVRTYP) == DONE)
      {
        data = getNewInstData(NULL, rx);
        if(data != NULL)
        {
          anzahl = data[0];
          data  += 1;

          if(anzahl == 0) // kennung dass keine weiteren einträge in cpu existieren
          {
            pcp->oldValue = value;
          }
          else // einträge auswerten
          {
            pcp->oldValue += anzahl;
            ProcessAlarmRecords(pcp, data, anzahl, ptemp, phist);
          }
        }
        else
        {
          value = pcp->oldValue; // problem: keine receive daten
        }
      }
      else
      {
        value = pcp->oldValue; // problem: alarme konnten nicht abgeholt werden
      }
    }
  }
}

_BOOL quitAlarmStation(_COMPORT *pcp, _UDWORD alano)
{
  _BOOL  retcode = _TRUE;
  _UBYTE tx[20];
  _UBYTE *rx, *data;
  _UWORD length;

  if(pcp != NULL)
  {
    if(pcp->alarmObject != 0)
    {
      retcode = _FALSE;
      
      *(_UDWORD*)&tx[0]  = pcp->alarmObject;
      *(_UWORD*) &tx[4]  = COMMAND_QUIT;
      *(_UBYTE*) &tx[6]  = P_IMMED;
      *(_UDWORD*)&tx[7]  = pcp->alarmHandle; //UC_NEW_ALARMSYSTEM;
      *(_UBYTE*) &tx[11] = P_COMMA;
      *(_UBYTE*) &tx[12] = P_IMMED;
      *(_UDWORD*)&tx[13] = alano;
      *(_UBYTE*) &tx[17] = P_EOL;

      if(TxCommandIntern(I_CMD, 18, tx, &pcp->comdef, (void**)&rx, DEFSVRTYP) == DONE)
      {
        data = getNewInstData(&length, rx);
        if((data != NULL) && (length == 1))
        {
          if(*data == 1)
          {
            retcode = _TRUE;
          }
        }
      }
    }
  }
  
  return(retcode);
}

_BOOL quitAlarm(_UWORD station, _UDWORD alano)
{
  _BOOL   retcode = _FALSE;
  _COMSET *pcs;
  _UWORD  i;
  
  if(station == 0xFFFF)
  {
    // Alarm(e) auf allen Stationen quittieren
    pcs = Batch.GetComSet();
    if(pcs != NULL)
    {
      for(i=0; i<pcs->no; i++)
      {
        if(quitAlarmStation(&pcs->ptr[i], alano) == _FALSE)
          retcode = _FALSE;
      }
    }
  }
  else
  {
    // Alarm(e) auf nur einer Station quittieren
    if(quitAlarmStation(Batch.Get_COMPORT(station), alano) == _FALSE)
      retcode = _FALSE;
  }
  
  TermAlarm.ResetTimer();
  
  return(retcode);
}

void _TermAlarm::ResetTimer(void)
{
  AlaTimer = 0; // alarme so schnell wie möglich wieder abholen
}

#endif

// lost alarms ***************************************************************
// lost alarms ***************************************************************
// lost alarms ***************************************************************
// lost alarms ***************************************************************
// lost alarms ***************************************************************
// lost alarms ***************************************************************
// lost alarms ***************************************************************

#ifndef UC_NEW_ALARMSYSTEM
_BOOL _TermAlarm::CheckLostAlarm(_ALACELL *pac)
{
  _BOOL retcode = _TRUE;
  _BOOL newalarm = _FALSE;
  _UWORD data[20];
  _IVALUE date, time;
  _DATE d;

  if(pac->info != 0)
  {
    decode_timedate(&time, &date, pac->info);
    lasal_to_DATE(&d, date);
    Sys_GetTime(data);
    d.year = data[0];
    DATE_to_lasal(&date, &d);


    if(date + time > MaxDateTime)
      newalarm = _TRUE;

    retcode = newalarm;
  }

  return(retcode);
}
#endif


// lasal interface ***********************************************************
// lasal interface ***********************************************************
// lasal interface ***********************************************************
// lasal interface ***********************************************************
// lasal interface ***********************************************************
// lasal interface ***********************************************************
// lasal interface ***********************************************************

extern "C" _ALARM *Lse_GetAlarmPointer(void)
{
  return(Alarm.GetAlarm());
}

extern "C" _BOOL store_ALACELL(_ALACELL *p)
{
  return(TermAlarm.Store_ALACELL(p));
}

extern "C" _MEM_SINGLEALARM* get_MEM_SINGLEALARM(_BOOL st)
{
  return(TermAlarm.Get_MEM_SINGLEALARM(st));
}

extern "C" _BOOL CheckLostAlarm(_ALACELL *pac)
{
 #ifdef UC_NEW_ALARMSYSTEM
  return((pac != NULL)? _TRUE:_FALSE); // nur wegen warnung ist abfrage != NULL
 #else
  return(TermAlarm.CheckLostAlarm(pac));
 #endif
}

extern "C" _UDWORD Lse_GetAlarmCrc(void)
{
  return Alarm.GetLseCrc32();
}

extern "C" _SINGLE_ALARM *LSE_GetAlarmconfig(_UWORD alaconfigno)
{
  return Alarm.Get_SINGLE_ALARM(Alarm.GetIndex(alaconfigno, NULL));
}

extern "C" _UDWORD LSE_GetAlarmConfigIndex(_UWORD alaconfigno)
{
  _UDWORD retcode = Alarm.GetIndex(alaconfigno, NULL);
  if(retcode == 0xFFFF)
    retcode = 0xFFFFFFFF;
    
  return retcode;
}

extern "C" _BOOL UpdateAlarmHistoryRecord(_SINGLEALARM *ptr)
{
  if(ptr != NULL)
  {
    _MEM_SINGLEALARM *pms = TermAlarm.Get_MEM_SINGLEALARM(_TRUE);
    if(pms != NULL)
    {
      return TermAlarm.Insert_SINGLEALARM(pms, ptr);
    }
  }
  return _FALSE;
}

