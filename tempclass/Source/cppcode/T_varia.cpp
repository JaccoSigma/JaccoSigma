#include "T_IPC.H"
#include "T_NUC.H"
#include "COMINTFC.H"

#define REAL_ROUND  0.5
#define UC_FASTBIT 
#define SA26785

#ifdef UC_LINKVL_FAST
void _LinkVlBuffer::Init(void)
{
  Sealed = _FALSE;
  No = 0;
  Used = 0;
  Ptr = NULL;
}

void _LinkVlBuffer::Free(void)
{
  Memory2((void**)&Ptr, 0);
  Init();
}

_LINKVL* _LinkVlBuffer::Get(_UDWORD anzahl, _UDWORD *rape)
{
  if(Sealed == _FALSE)
  {
    if(anzahl > 0)
    {
      if((Used + anzahl) <= No)
      {
        _LINKVL *retcode = &Ptr[Used];
        *rape = Used | 0x80000000;
        Used += anzahl;
        return(retcode);
      }
      else
      {
        if(Memory2((void**)&Ptr, (No + 1000)*sizeof(_LINKVL)) != 0)
        {
          No = No + 1000;
          return(Get(anzahl, rape));
        }
      }
    }
  }
  *rape = 0;
  return NULL;
}

void _LinkVlBuffer::Finalize(_LSEVARINFO *pvi)
{
  if(Used < No)
  {
    No = Used;
    Memory2((void**)&Ptr, Used*sizeof(_LINKVL));
  }
  
  Sealed = _TRUE;
  _UDWORD rape = *(_UDWORD*)&pvi->memlinkvl.ptr;
  if(rape & 0x80000000)    
  {
    rape = rape & 0x7FFFFFFF;
    if(rape < Used)
    {
      pvi->memlinkvl.ptr = &Ptr[rape];
    }
    else
    {
      pvi->memlinkvl.ptr = NULL;
      pvi->memlinkvl.no = 0;
    }
  }
}
#endif

#ifdef OLDSTYLE_CAN
_BOOL insert_size(_UDWORD *psize, _UDWORD size, _UDWORD varno, _MEM_VARSYMBOL *p)
{
  _BOOL retcode = _FALSE;

  if(varno < p->no)
  {
    retcode = _TRUE;
    if(size != 0)
    {
      psize[varno] += size;
      insert_size(psize, size, p->ptr[varno].scope, p);
    }
  }

  return(retcode);
}

_UDWORD varsize(_LSEVARINFO *p)
{
  _UDWORD retcode = 0;

  switch(p->vartype)
  {
    case FENUM     :
    case BINARY    : // binary
                     switch(p->format)
                     {
                       case SIZE_BYTE   : retcode = 1; break;
                       case SIZE_WORD   : retcode = 2; break;
                       case SIZE_DWORD  : retcode = 4; break;
                       case SIZE_UBYTE  : retcode = 1; break;
                       case SIZE_UWORD  : retcode = 2; break;
                       case SIZE_UDWORD : retcode = 4; break;
                       case SIZE_HBYTE  : retcode = 1; break;
                       case SIZE_HWORD  : retcode = 2; break;
                       case SIZE_HDWORD : retcode = 4; break;
                       case SIZE_BBYTE  : retcode = 1; break;
                       case SIZE_BWORD  : retcode = 2; break;
                       case SIZE_BDWORD : retcode = 4; break;
                       case SIZE_REAL   : retcode = 4; break;
                       case SIZE_LREAL  : retcode = 8; break;
                     }
                     break;
    case FSTRING   : retcode = p->format;
                     break;
    case FTIME     : // time
    case FDATE     : // date
                     retcode = 4;
                     break;
    case BIT       : // bit datatype
                     retcode = 0;
                     break;

//    case XRADIO    : // nc
//    case XBITMASK  : // nc
//    case COMPLEX   : // structure, complex
//    case FREE      : // nc
//    case FCONSTANT : // constant type
//    case UNUSED    : // nc
//    case OBJECT    : // objecttype
  }

  return(retcode);
}

void insert_offset(_UDWORD *psize, _UDWORD scope, _VARBLOCK *p)
{
  _LSEVARINFO *pvi;
  _VARSYMBOL  *pvs;
  _UDWORD     size = 0;
  _UDWORD     i;

  if(scope < p->MemVarSymbol.no)
  {
    if((psize[scope] & 0x80000000L) == 0)
    {
      psize[scope] |= 0x80000000L;

      pvi = p->MemLseVarInfo.ptr;
      pvs = p->MemVarSymbol.ptr;

      for(i=0; i<p->MemLseVarInfo.no; i++, pvi++, pvs++, psize++)
      {
        if(pvs->scope == scope)
        {
          pvi->uimenuindex = size;
          size            += (*psize) & 0x00FFFFFFL;
        }
      }
    }
  }
}

_UDWORD calculate_address_sub(_VARBLOCK *p, _UDWORD i, _UDWORD retcode)
{
  _VARSYMBOL  *pvs;
  _LSEVARINFO *pvi;

  pvs = &p->MemVarSymbol.ptr[i];
  pvi = &p->MemLseVarInfo.ptr[i];

  if((pvs->scope != DEFSCOPE)&&(pvs->scope < p->MemVarSymbol.no))
  {
    retcode += calculate_address_sub(p, pvs->scope, pvi->uimenuindex);
  }
  else if(pvi->memlinkvl.no > 0)
  {
    if(pvi->memlinkvl.ptr[0].link.info[0].state == CONST_VAL)
      retcode += pvi->memlinkvl.ptr[0].link.info[0].value;
  }

  return(retcode);
}

void calculate_address(_VARBLOCK *p)
{
  _UDWORD     i;
  _LSEVARINFO *pvi;
  _COMPORT    *cp;

  pvi = p->MemLseVarInfo.ptr;
  for(i=0; i<p->no; i++, pvi++)
  {
    cp = Batch.Get_COMPORT(pvi->station);
    if(cp != NULL)
    {
      if(cp->comdef.typ != _COM_INTERN)
      {
        pvi->lasalid.id = calculate_address_sub(p, i, 0); //pvi->uimenuindex);
      }
    }
  }
}

_BOOL change_baseaddress(_VARBLOCK *p, _UDWORD varno, _UDWORD address)
{
  _LSEVARINFO *pvi;
  _VARSYMBOL  *pvs;
  _BOOL       retcode = _FALSE;

  if(varno < p->no)
  {
    pvs = &p->MemVarSymbol.ptr[varno];
    if(pvs->scope != DEFSCOPE)
    {
      retcode = change_baseaddress(p, pvs->scope, address);
    }
    else
    {
      pvi = &p->MemLseVarInfo.ptr[varno];
      if(pvi->memlinkvl.no > 0)
      {
        if(pvi->memlinkvl.ptr[0].link.info[0].state == CONST_VAL)
        {
          pvi->memlinkvl.ptr[0].link.info[0].value = address;
          retcode = _TRUE;
        }
      }
    }
  }

  return(retcode);
}

void compile(_VARBLOCK *p)
{
  _UDWORD     size, i;
  _LSEVARINFO *pvi;
  _VARSYMBOL  *pvs;
  _UDWORD     *psize = NULL;

  size = p->no*sizeof(_UDWORD);
  if(Memory2((void**)&psize, size) != 0)
  {
    MemSet(psize, 0, size);
    pvi = p->MemLseVarInfo.ptr;
    for(i=0; i<p->no; i++, pvi++)
      insert_size(psize, varsize(pvi), i, &p->MemVarSymbol);

    pvi = p->MemLseVarInfo.ptr;
    pvs = p->MemVarSymbol.ptr;
    for(i=0; i<p->no; i++, pvi++, pvs++)
    {
      if(pvs->scope < p->no)
        insert_offset(psize, pvs->scope, p);
    }

//    calculate_address(p);

    Memory2((void**)&psize, 0);
  }
}

#endif

_BOOL format_BINARY(_RESULT*, _VARINFO*, void*);
_BOOL format_FDATE(_RESULT*, void*);
_BOOL format_FTIME(_RESULT*, void*);
_BOOL format_FSTRING(_RESULT*, void*);
_BOOL format_BIT(_RESULT*, _VARINFO*, void*);

_BOOL set_format_BINARY(_RESULT*, _VARINFO*, void*);
_BOOL set_format_FDATE(_RESULT*, void*);
_BOOL set_format_FTIME(_RESULT*, void*);


_BOOL set_format_BINARY(_RESULT *res, _VARINFO *p, void *ptr)
{
  _BOOL retcode = _FALSE;


  if((res->ftype == RES_NUMERIC)||(res->ftype == RES_CONSTANT)||(res->ftype == RES_REAL))
  {
    retcode = _TRUE;
    switch(p->format)
    {
      case SIZE_BYTE   : *(_BYTE*)ptr  = (_BYTE)res->value;    break;
      case SIZE_WORD   : *(_WORD*)ptr  = (_WORD)res->value;    break;
      case SIZE_DWORD  : *(_DWORD*)ptr = (_DWORD)res->value;   break;
      case SIZE_BBYTE  :
      case SIZE_HBYTE  :
      case SIZE_UBYTE  : *(_UBYTE*)ptr = (_UBYTE)res->value;   break;
      case SIZE_BWORD  :
      case SIZE_HWORD  :
      case SIZE_UWORD  : *(_UWORD*)ptr = (_UWORD)res->value;   break;
      case SIZE_BDWORD :
      case SIZE_HDWORD :
      case SIZE_UDWORD : *(_DWORD*)ptr = (_DWORD)res->value;   break;
     #ifdef UC_REAL
      #ifdef UC_IPC
       case SIZE_REAL  : move_pREAL_pREAL((_REAL*)ptr, (_REAL*)&res->value); break;
      #else
       case SIZE_REAL  : RealCalc((_REAL*)ptr, (_REAL*)&res->value, NULL, IOP_A_EQUAL); break;
//       case SIZE_REAL  : MemCpy(ptr, &res->value, sizeof(_REAL)); break;
      #endif
     #endif
      default          : retcode = _FALSE;                     break;
    }
  }

  return(retcode);
}

_BOOL set_format_FDATE(_RESULT *res, void *ptr)
{
  _IVALUE value;
  _BOOL   retcode = _FALSE;

  if(res->ftype == RES_DATE)
  {
    retcode = _TRUE;
    DATE_to_lasal(&value, &res->datim.fdate);
    *(_IVALUE*)ptr = value;
  }

  return(retcode);
}

_BOOL set_format_FTIME(_RESULT *res, void *ptr)
{
  _IVALUE value;
  _BOOL   retcode = _FALSE;

  if(res->ftype == RES_TIME)
  {
    retcode = _TRUE;
    TIME_to_lasal(&value, &res->datim.ftime);
    *(_IVALUE*)ptr = value;
  }

  return(retcode);
}

_BOOL format_BINARY(_RESULT *res, _VARINFO *p, void *ptr)
{
  _BOOL retcode = _TRUE;


 #ifdef UC_COMPLEX_SERVER
  if(p->POWPRUIC & VARBIT_STRUCTELEMENT)
  {
    if(format_ELEMENT(p, ptr) == _FALSE)
      return(_FALSE);
  }
 #endif

  switch(p->format)
  {
    case SIZE_BYTE   : res->value = *(_BYTE*)ptr;            break;
    case SIZE_WORD   : res->value = *(_WORD*)ptr;            break;
    case SIZE_DWORD  : res->value = *(_DWORD*)ptr;           break;
    case SIZE_BBYTE  :
    case SIZE_HBYTE  :
    case SIZE_UBYTE  : res->value = *(_UBYTE*)ptr;           break;
    case SIZE_BWORD  :
    case SIZE_HWORD  :
    case SIZE_UWORD  : res->value = *(_UWORD*)ptr;           break;
    case SIZE_BDWORD :
    case SIZE_HDWORD :
    case SIZE_UDWORD : res->value = *(_DWORD*)ptr;           break;
   #ifdef UC_REAL
    #ifdef UC_IPC
     case SIZE_REAL  : MemCpy(&res->value, ptr, sizeof(_REAL));
    #else
     case SIZE_REAL  : RealCalc((_REAL*)&res->value, (_REAL*)ptr, NULL, IOP_A_EQUAL);
//     case SIZE_REAL  : MemCpy(&res->value, ptr, sizeof(_REAL));
    #endif
                       res->ftype   = RES_REAL;
                       res->ptr     = &res->value;
                       return(retcode);
   #endif
    default          : retcode = _FALSE;                     break;
  }

  if(retcode == _TRUE)
  {
    res->ftype = RES_NUMERIC;
    res->ptr   = &res->value;
  }
  else
  {
    init_RESULT_Makro(res);
  }

  return(retcode);
}

#ifdef UC_COMPLEX_SERVER
_BOOL format_ELEMENT(_VARINFO *pvi, void *ptr)
{
  _BOOL retcode = _TRUE;


  switch(VarList.GetVariableSize(pvi->vartype, pvi->format))
  {
    case 1:
      *(_UBYTE*)ptr = *(_UBYTE*)((_UBYTE*)ptr+pvi->bitno);
      break;
    case 2:
      *(_UWORD*)ptr = *(_UWORD*)((_UBYTE*)ptr+pvi->bitno);
      break;
    case 4:
      *(_UDWORD*)ptr = *(_UDWORD*)((_UBYTE*)ptr+pvi->bitno);
      break;
    default:
      retcode = _FALSE;
      break;
  }

  return(retcode);
}
#endif

_BOOL format_FDATE(_RESULT *res, void *ptr)
{
  res->value = *(_IVALUE*)ptr;
  lasal_to_DATE(&res->datim.fdate, res->value);

  res->ftype = RES_DATE;
  res->ptr   = &res->datim.fdate;

  return(_TRUE);
}

_BOOL format_FTIME(_RESULT *res, void *ptr)
{
  res->value = *(_IVALUE*)ptr;
  lasal_to_TIME(&res->datim.ftime, res->value);

  res->ftype = RES_TIME;
  res->ptr  = &res->datim.ftime;

  return(_TRUE);
}

_BOOL format_FSTRING(_RESULT *res, void *ptr)
{
  res->ptr = ptr;
  res->ftype = RES_ANUMERIC;

  return(_TRUE);
}

_BOOL format_BIT(_RESULT *res, _VARINFO *p, void *ptr)
{
  _UDWORD tmp;

  switch(VarList.GetVariableSize(p->vartype, p->format))
  {
    case 1 : tmp = *(_UBYTE*)ptr;  break;
    case 2 : tmp = *(_UWORD*)ptr;  break;
    case 4 : tmp = *(_UDWORD*)ptr; break;
    default : init_RESULT_Makro(res);
              return _FALSE;
  }

  tmp = ((tmp >> p->bitno) & 1);
  if(VarList.GetInvert(p->POWPRUIC) == _TRUE)
      tmp = tmp ^ 1;

  res->value = tmp;
  res->ptr   = &res->value;
  res->ftype = RES_NUMERIC;

  return _TRUE;
}



_UDWORD find_root(_MEM_VARSYMBOL *p, _UDWORD i)
{
  // keep hands off, kinky recursive
  _UDWORD scope, retcode = i;

  if(i < p->no)
  {
    scope = p->ptr[i].scope;
    if(scope < p->no)
      retcode = find_root(p, scope);
  }

  return(retcode);
}

void _VarList::RepairStation(void)
{
  _VARSYMBOL  *pvs;
  _LSEVARINFO *pvi;
  _UDWORD     i, root;

  pvi = Data.MemLseVarInfo.ptr;
  pvs = Data.MemVarSymbol.ptr;
  for(i=0; i<Data.MemVarSymbol.no; i++)
  {
    root = find_root(&Data.MemVarSymbol, pvs->scope);
    if(root < Data.MemLseVarInfo.no)
      Data.MemLseVarInfo.ptr[i].station = Data.MemLseVarInfo.ptr[root].station;

    Stations[pvi->station] = 1;
    pvs += 1;
    pvi += 1;
  }
}

void _VarList::VarDummyInit(void)
{
  VarDummyData[0] = 0;
  VarDummyData[1] = 0;
  VarDummyData[2] = 0;
  VarDummyData[3] = 0; // VARNO_HIRAGANA_KATAGANA
  VarDummyData[4] = 0; // VARNO_JAPANESE_INPUT
  
}

_DWORD abstime(void)
{
  _UDWORD timex = get_abstime();
  return (_DWORD)(timex % 1000000);
}

_DWORD _VarList::VarDummyGet(_UDWORD varno)
{
  switch(varno)
  {
    case VARNO_ALTGRFLAG_LOCK :
    case VARNO_ALTGRFLAG :
      return VarDummyData[2] & 3;
      
    case VARNO_SHIFTFLAG :
      return VarDummyData[1];
        
    case VARNO_INSERTFLAG :
      return VarDummyData[0];

    case VARNO_HIRAGANA_KATAGANA :
      return VarDummyData[3];
      
    case VARNO_JAPANESE_INPUT :
      return VarDummyData[4];
      
    case VARNO_TIMEX_100 :
      return abstime() / 100;
  }
  
  return 0;
}

_BOOL _VarList::VarDummyGet(_RESULT *res, _VARLABEL *vl)
{
  if(chk_VARLABEL_Makro(vl))
  {
    if(vl->info[0].state == VAR_VAL)
    {
      if(vl->info[0].value >= VARNO_JAPANESE_INPUT)
      {
        switch(vl->info[0].value)
        {
          case VARNO_ALTGRFLAG_LOCK:
          case VARNO_ALTGRFLAG:
            res->ftype = RES_NUMERIC;
            res->value = VarDummyData[2] & 3;
            res->rmin  = 0;
            res->rmax  = 3; // wegen VARNO_ALTGRFLAG_LOCK
            return _TRUE;
        
          case VARNO_SHIFTFLAG:
            res->ftype = RES_NUMERIC;
            res->value = VarDummyData[1];
            res->rmin  = 0;
            res->rmax  = 1;
            return _TRUE;
            
          case VARNO_INSERTFLAG:
            res->ftype = RES_NUMERIC;
            res->value = VarDummyData[0];
            res->rmin  = 0;
            res->rmax  = 1;
            return _TRUE;
      
          case VARNO_TIMEX_100 :
            res->ftype = RES_NUMERIC;
            res->value = abstime() / 100; 
            res->rmin  = res->value;
            res->rmax  = res->value;
            return _TRUE;

          case VARNO_HIRAGANA_KATAGANA:
            res->ftype = RES_NUMERIC;
            res->value = VarDummyData[3];
            res->rmin  = 0;
            res->rmax  = 1;
            return _TRUE;

          case VARNO_JAPANESE_INPUT:
            res->ftype = RES_NUMERIC;
            res->value = VarDummyData[4];
            res->rmin  = 0;
            res->rmax  = 1;
            return _TRUE;

        }
      }
    }
  }

  
  return _FALSE;
}

static void VarDummySetBit(_UDWORD *pval, unsigned long bitwert, _DWORD val)
{
  if(val != 0)
  {
    *pval = *pval | bitwert; // entsprechendes Bit setzen
  }
  else
  {
    *pval = *pval & ~bitwert; // entsprechendes Bit setzen
  }
}

_BOOL _VarList::VarDummySet(_UDWORD varno, _DWORD value)
{
  switch(varno)
  {
    case VARNO_ALTGRFLAG_LOCK:
      VarDummySetBit(&VarDummyData[2], 2, value);
//      if(value != 0)
//      {
//        if(Language.GetActualLocal() == LOCAL_JAPANESE)
//        {
//          VarDummySet(VARNO_JAPANESE_INPUT, 1);
//        }
//      }
      return _TRUE;
      
    case VARNO_ALTGRFLAG : 
      VarDummySetBit(&VarDummyData[2], 1, value);
      return _TRUE;
      
    case VARNO_SHIFTFLAG : 
      VarDummyData[1] = value;
      return _TRUE;
        
    case VARNO_INSERTFLAG :
      VarDummyData[0] = value;
      return _TRUE;

    case VARNO_HIRAGANA_KATAGANA :
      VarDummyData[3] = value;
      return _TRUE;
      
    case VARNO_JAPANESE_INPUT :
      VarDummyData[4] = value;
      return _TRUE;
  }
  
  return _FALSE;
}

_BOOL _VarList::VarDummySet(_VARLABEL *vl, _RESULT *res)
{
  if(chk_VARLABEL_Makro(vl))
  {
    if((vl->info[0].state = VAR_VAL) && (vl->info[0].value >= VARNO_JAPANESE_INPUT))
    {
      if((res->ftype == RES_NUMERIC) || (res->ftype == RES_CONSTANT))
      {
        VarDummySet(vl->info[0].value, res->value);
      }
    }
  }
  
  return _FALSE;
}

#ifdef UC_ALARMID_FAST
void _VarList::init_MEM_LINDEX(_MEM_LINDEX *p)
{
  p->ptr  = NULL;
  p->no   = 0;
  p->done = 0;
}

void _VarList::free_MEM_LINDEX(_MEM_LINDEX *p)
{
  if(p->ptr != NULL)
  {
    Memory2((void**)&p->ptr, 0);
  }
  init_MEM_LINDEX(p);
}
#endif

void _VarList::Init(void)
{
  VarDummyInit();
  
 #ifdef UC_LINKVL_FAST
  LinkVlBuffer.Init();
 #endif
  init_VARBLOCK(&Data);
  init_MEM_UNIT(&MemUnit);
//  VarText.Init();
  LabelUnit.Init();
  LabelGroup.Init();
  init_MEM_MAP(&MapCRC);
  init_NAME_INDEX(&HostName);
//  init_MEM_MAP(&MapLasalID);
  VarIdChanged = _FALSE;
  FileOffsetHostName = -1;
  VarListDPNE[0] = 0;
  MemSet(Stations, 0, sizeof(Stations));
  MemberVariablesCount = 0;
  
 #ifdef UC_ALARMID_FAST
  for(_UDWORD i=0; i<lib_arraysize(MlStation); i++)
  {
    init_MEM_LINDEX(&MlStation[i]);
  }
 #endif
  
}

void _VarList::Free(void)
{
  free_VARBLOCK(&Data);
  free_MEM_UNIT(&MemUnit);
//  VarText.Free();
  LabelUnit.Free();
  LabelGroup.Free();
  free_MEM_MAP(&MapCRC);
  free_NAME_INDEX(&HostName);
//  free_MEM_MAP(&MapLasalID);

 #ifdef UC_ALARMID_FAST
  for(_UDWORD i=0; i<lib_arraysize(MlStation); i++)
  {
    free_MEM_LINDEX(&MlStation[i]);
  }
 #endif

 #ifdef UC_LINKVL_FAST
  LinkVlBuffer.Free();
 #endif

  Init();
}

_BOOL _VarList::IsStation(_UWORD station)
{
  if(station < 256)
    return((Stations[station] == 0)? _FALSE : _TRUE);
  return(_FALSE);
}

_BOOL _VarList::Load(_ASCII *dpne, _UWORD sb, _UWORD *sbi)
{
  _File File;
  _BOOL retcode = _FALSE;

  Free();

  File.Open(LOAD, dpne);
  if(File.ReadHeader() == _TRUE)
  {
    if(IsRamFile(dpne) == _TRUE)
      StrCpy(VarListDPNE, sizeof(_ASCII), &dpne[4], sizeof(_ASCII));
    else
      StrCpy(VarListDPNE, sizeof(_ASCII), dpne, sizeof(_ASCII));

    retcode = Read(&File, sb, sbi);
  }
  File.Close();

  if(retcode == _TRUE)
  {
//    RepairStation(&Data);
    RepairStation();

   #ifdef OLDSTYLE_CAN
    compile(&Data);
//    compile(&Types);
   #endif

   #ifdef _FIXED_STATESCHEME
    PrepaireStatescheme();
   #endif
  }

  return(retcode);
}

_BOOL _VarList::Read(_File *File, _UWORD sb, _UWORD *sbi)
{
  // [ ......... all text (variabletext)
  // [ ......... all varinfo (infotable + symboltable + labels)
  // [ ......... all typinfo (infotable + symboltable + labels)
  // [ ......... unitinfo
  // [ ......... grouplabel

  _BOOL     retcode = _TRUE;
  _VARBLOCK typedummy;

  Init();

  if(File->GetError() == _FALSE)
  {
    // load variabletextlist
//    VarText.ReadLanguage(File, language);  // weg ab COMPILER_VERSION 20

    // load variables and objects
    File->Read_VARBLOCK(&Data);
    *sbi += 1;
    ScanBar.Run(sb, *sbi);

    // load types and classes
    init_VARBLOCK(&typedummy);
    File->Read_VARBLOCK(&typedummy);
    free_VARBLOCK(&typedummy);
    *sbi += 1;
    ScanBar.Run(sb, *sbi);

    // unitinfo
    File->Read_MEM_UNIT(&MemUnit);

    // unitlabel
    LabelUnit.Read(File, _FALSE);
    *sbi += 1;
    ScanBar.Run(sb, *sbi);

    // grouplabel
    LabelGroup.Read(File, _FALSE);

   #if COMPILER_VERSION >= 30
    File->Read_MEM_MAP(&MapCRC);
   #endif

   #if COMPILER_VERSION >= 35
    FileOffsetHostName = File->Tell();
    File->Read_NAME_INDEX(&HostName);
   #endif
  }

  if(File->GetError() == _TRUE)
  {
    Free();
    retcode = _FALSE;
  }

//  else
//  {
//    Data.MemVarInfo.ptr[25].stscheme = 0;
//  }

  return(retcode);
}

_BOOL _VarList::ReloadHostName(void)
{
  _File File;
  _BOOL retcode = _FALSE;

  if((FileOffsetHostName != -1)&&(VarListDPNE[0] != 0))
  {
    free_NAME_INDEX(&HostName);
    File.Open(LOAD, VarListDPNE);
    File.Seek(FileOffsetHostName, SEEK_SET);
    File.Read_NAME_INDEX(&HostName);
    retcode = (File.GetError() == _TRUE)? _FALSE : _TRUE; 
    File.Close();
  }

  return(retcode);
}

_LSEVARINFO *_VarList::Get(_VARLABEL *pvl)
{
  _LSEVARINFO *pvi = NULL;

  if(pvl->no > 0)
  {
    if(pvl->info[0].state == VAR_VAL)
    {
      GetVarInfo(&pvi, *(_UDWORD*)&pvl->info[0].value);
    }
  }
  
  return pvi;
}

_BOOL _VarList::GetVarInfo(_LSEVARINFO **p, _UDWORD no)
{
  if(no < Data.MemLseVarInfo.no)
  {
    *p = &Data.MemLseVarInfo.ptr[no];
    return(_TRUE);
  }

  return(_FALSE);
}

_BOOL _VarList::ConvertVarlabel(_VARLABEL *vl)
{
  _BOOL     retcode = _FALSE;
  _RESULT   res;
  _VARIABLE var;

  if(GetSystemData(&res, &var, vl, _FALSE, _TRUE) == _TRUE)
  {
    switch(res.ftype)
    {
      case RES_NUMERIC  :
      case RES_CONSTANT : vl->no            = 1;
                          vl->info[0].state = CONST_VAL;
                          vl->info[0].value = res.value;
                          retcode           = _TRUE;
                          break;
      case RES_DEFAULT  : retcode           = _TRUE;
                          break;
      case RES_ANUMERIC : break;
      default           : break;
    }
  }

  return(retcode);
}

_UWORD _VarList::GetUnitNo(_ASCII *txt)
{
  _UWORD  retcode = 0xFFFF;
  _UDWORD no;

  if(LabelUnit.Seek(&no, txt) == _TRUE)
    retcode = no;

	return(retcode);
}

_UNIT *_VarList::GetUnitInfo(_UWORD unitno)
{
  if(unitno < MemUnit.no)
    return(&MemUnit.ptr[unitno]);
  return(NULL);
}

_UWORD _VarList::GetNoUnit(void)
{
  return(MemUnit.no);
}

_UBYTE _VarList::CalculDigits(_VARINFO *var, _UWORD no)
{
  _UBYTE retcode = no;

  if(no >= 0xFF)
  {
    switch(var->vartype)
    {
      case BINARY : switch(var->format)
                    {
                      case SIZE_BYTE   : retcode =  3; break;
                      case SIZE_WORD   : retcode =  5; break;
                      case SIZE_DWORD  : retcode = 10; break;
                      case SIZE_UBYTE  : retcode =  3; break;
                      case SIZE_UWORD  : retcode =  5; break;
                      case SIZE_UDWORD : retcode = 10; break;
                      case SIZE_HBYTE  : retcode =  2; break;
                      case SIZE_HWORD  : retcode =  4; break;
                      case SIZE_HDWORD : retcode =  8; break;
                      case SIZE_BBYTE  : retcode =  8; break;
                      case SIZE_BWORD  : retcode = 16; break;
                      case SIZE_BDWORD : retcode = 32; break;
                      default          : retcode =  8; break;
                    }
                    break;
      case FSTRING : retcode = var->format;
                    break;
      case FTIME  : switch(var->format)
                    {
                      case SIZE_MMSS   : retcode = 5; break;
                      case SIZE_HHMM   : retcode = 5; break;
                      default          : retcode = 8; break;
                    }
                    if(var->format != SIZE_MMSS)
                    {
                      if(get_americantimeformat() == _TRUE)
                      {
                        retcode += 3;
                      }
                    }
                    break;
      case FDATE  : switch(var->format)
                    {
                      case SIZE_DDMM     : retcode =  5; break;
                      case SIZE_DDMMYY   : retcode =  8; break;
                      case SIZE_WWDDMM   : retcode =  8; break;
                      case SIZE_WWDDMMYY : retcode = 11; break;
                      case SIZE_DDMMYYYY : retcode = 10; break;
                      case SIZE_YYYYMMDD : retcode = 10; break;
                      case SIZE_YYMMDD   : retcode =  8; break;
                      case SIZE_YYMM     : retcode =  5; break;
                      default            : retcode = 13; break;
                    }
                    break;
//      case BITMASK: retcode = 10; // ((var->Size + 1) * 8) - var->BitNr;
//                    break;
      default     : retcode = 10;
                    break;
    }
  }

  return(retcode);
}

_UDWORD _VarList::GetUnitIndex(_UDWORD varno) // sa36634
{
  _UDWORD retcode = 0xFFFFFFFF;
  
  if(varno < Data.MemLseVarInfo.no)
  {  
    _LSEVARINFO *pvi = &Data.MemLseVarInfo.ptr[varno];
    _MYSCHEME tmpscm;
    _UDWORD   unitno;

    init_MYSCHEME_Makro(&tmpscm);
    tmpscm.schemeno = pvi->funitscheme;

    if(UnitScheme.Call(&unitno, &tmpscm, NULL, pvi->funit) == _TRUE)
    {
      retcode = unitno;
    } 
    else
    {
      retcode = pvi->funit;
    }

    if(retcode >= MemUnit.no)
    {
      retcode = 0xFFFFFFFF;
    }
  }
  
  return retcode;    
}

_ASCII *_VarList::GetUnitLabel(_UDWORD unitindex) // sa36634
{
  return LabelUnit.GetText((_UWORD)unitindex);
}

_BOOL _VarList::InsertUnitInfo(_VARINFO *p)
{
  _RESULT   res;
  _VARIABLE var;
  _UNIT     *pu;
  _BOOL     retcode = _TRUE;
  _UDWORD   unitno;
  _MYSCHEME myscheme;

  if((p->vartype == BINARY) || (p->vartype == FCONSTANT))
  {
    retcode = _FALSE;

    init_MYSCHEME_Makro(&myscheme);
    myscheme.schemeno = p->funitscheme;

    if(UnitScheme.Call(&unitno, &myscheme, NULL, p->funit) == _TRUE)
      pu = GetUnitInfo((_UWORD)unitno);
    else
      pu = GetUnitInfo(p->funit);

    if(pu != NULL)
    {
      p->formula   = pu->formula;
      p->parameter = pu->parameter;

      if(GetSystemData(&res, &var, &pu->posdezpt, _TRUE, _TRUE, _FALSE) == _TRUE)
      {
        if(res.ftype == RES_REAL)
        {
          res.value = FTOL(get_pREAL((_REAL*)&res.value) + 0.5); // dr1850, sa23917
        }

        p->posdezpt = (res.value > 0)? (_UBYTE)res.value : 0; // dr2486: es gibt keinen negativen wert

        if(GetSystemData(&res, &var, &pu->xdigits, _TRUE, _FALSE, _FALSE) == _TRUE)
        {
          if(res.ftype == RES_REAL)
          { // dr1850, sa23917
            res.value = FTOL(get_pREAL((_REAL*)&res.value) + 0.5);
            res.ftype = RES_NUMERIC;
          }
        
          p->digits = CalculDigits(p, res.value);
          retcode = _TRUE;
        }
      }

      if(retcode == _FALSE)
        p->digits = CalculDigits(p, 0xFFFF);

      if(pu->fixdp != 0)
        p->POWPRUIC |= VARBIT_FIXDEZPT;
      else
        p->POWPRUIC &= ~VARBIT_FIXDEZPT;
        
     #ifdef SA26785
      if(pu->leading0 != 0) 
      {
        if(pu->leading0 == 1)
        {
          p->POWPRUIC |= VARBIT_PREZERO;
        }
        else
        {
          p->POWPRUIC &= ~VARBIT_PREZERO; 
        }
      }
     #endif
    }
    else
    {
      p->digits    = 0; // 10
      p->posdezpt  = 0;
      p->formula   = DEFFORMULA;
      init_VARLABEL_Makro(&p->parameter);
      retcode      = _TRUE;
    }
  }

  return(retcode);
}

_BOOL _VarList::GetSingleVariable(_VARIABLE *p, _UDWORD no)
{
   _LSEVARINFO *vi;
   
  if(GetVarInfo(&vi, no) == _TRUE)
  {
    _BOOL retcode = _TRUE;
    p->no = no;
//    if(copy_LSE_VARINFO(&retcode, &p->info, vi, _FALSE) == _TRUE)
//      retcode = InsertUnitInfo(&p->info);

    retcode = copy_LSE_VARINFO(&retcode, &p->info, vi, _FALSE);

    if(retcode == _FALSE)
      free_VARIABLE(p);
    
    return retcode;
  }
  
  init_VARIABLE_Makro(p);
  return _FALSE;
}

_BOOL _VarList::ShiftVarlabel(_VARLABEL *vl)
{
  _UDWORD size;
  _BOOL   retcode = _FALSE;

  if(vl->no > 1)
  {
    size = (VL_MAXINDEX - 1) * sizeof(_VARSINGLE);
    MemMove(&vl->info[1], &vl->info[2], size);
    vl->no --;
  }

  return(retcode);
}

_BOOL _VarList::GetVariable(_VARIABLE *p, _VARLABEL *vl)
{
  if(vl->no > 0)
  {
    if((vl->info[0].state == VAR_VAL) || (vl->info[0].state == PARA_VAL))
    {
      return GetSingleVariable(p, vl->info[0].value);
    }
    else if(vl->info[0].state == CRC_VAL)
    {
      return GetSingleVariable(p, GetVarNoByCRC(vl->info[0].value));
    }
  }

  return _FALSE;
}

_DWORD _VarList::GetIndexValue(_VARLABEL *vl)
{
  return(vl->info[0].value);
}

_BOOL _VarList::GetTime(_RESULT *res, _VARINFO *p, _VARLABEL *pvl)
{
  void  *ptr;

  init_TIME_Makro(&res->datim.ftime);

  if(GetPointerToData((void**)&ptr, p, pvl) == _TRUE)
    return format_FTIME(res, ptr);

  return _FALSE;
}

_BOOL _VarList::GetConstant(_RESULT *res, _VARINFO *p)
{
  _VARIABLE var;
  
  // constant value is stored in all limits !!
  return(GetSystemData(res, &var, &p->lo_limit, _FALSE, _TRUE));
}

_BOOL _VarList::GetMerker(_RESULT *res, _UDWORD varno)
{
  _BOOL retcode = _FALSE;
  
  if(varno < Data.MemLseVarInfo.no)
  {  
    res->ftype = RES_NUMERIC;
    res->value = *(_DWORD*)&Data.MemLseVarInfo.ptr[varno].lasalid.id;
    res->ptr   = &res->value;
    res->rmin  = -2147483647L;
    res->rmax  = 2147483647L;
    retcode = _TRUE;
  }
  
  return(_TRUE);
}

_BOOL _VarList::SetMerker(_RESULT *res, _UDWORD varno)
{
  _BOOL retcode = _FALSE;
  
  if(varno < Data.MemLseVarInfo.no)
  {  
    Data.MemLseVarInfo.ptr[varno].lasalid.id = *(_UDWORD*)&res->value;
    retcode = _TRUE;
  }
  
  return(retcode);
}

_BOOL _VarList::GetBit(_RESULT *res, _VARINFO *p, _VARLABEL *pvl)
{
  void    *ptr;
  _BOOL   retcode = _FALSE;

  init_RESULT_Makro(res);

  if(GetPointerToData((void**)&ptr, p, pvl) == _TRUE)
    retcode = format_BIT(res, p, ptr);

  return(retcode);
}

_BOOL _VarList::GetString(_RESULT *res, _VARINFO *p, _VARLABEL *pvl)
{
  void *ptr;
  _BOOL retcode = _FALSE;

  init_RESULT_Makro(res);

  if(GetPointerToData((void**)&ptr, p, pvl) == _TRUE)
    retcode = format_FSTRING(res, ptr);

  return(retcode);
}

_BOOL _VarList::GetDate(_RESULT *res, _VARINFO *p, _VARLABEL *pvl)
{
  _BOOL retcode = _FALSE;
  void  *ptr;

  init_DATE_Makro(&res->datim.fdate);

  if(GetPointerToData((void**)&ptr, p, pvl) == _TRUE)
    retcode = format_FDATE(res, ptr);

  return(retcode);
}


_BOOL _VarList::GetBinary(_RESULT *res, _VARINFO *p, _VARLABEL *pvl)
{
  _BOOL  retcode = _FALSE;
  void   *ptr;

  if(GetPointerToData((void**)&ptr, p, pvl) == _TRUE)
    retcode = format_BINARY(res, p, ptr);

  return(retcode);
}

void _VarList::GetDefaultMinMax(_RESULT *res, _VARIABLE *p)
{
  switch(p->info.vartype)
  {
    case FENUM   :
    case BINARY  :
                   switch(p->info.format)
                   {
                     case SIZE_BYTE   : res->rmin = -128;
                                        res->rmax = 127;
                                        break;
                     case SIZE_WORD   : res->rmin = -32768L;
                                        res->rmax = 32767L;
                                        break;
                     case SIZE_DWORD  : res->rmin = 0x80000000;  //GNU  //-2147483648L;
                                        res->rmax = 2147483647L;
                                        break;
                     case SIZE_BBYTE  :
                     case SIZE_HBYTE  :
                     case SIZE_UBYTE  : res->rmin = 0;
                                        res->rmax = 255;
                                        break;
                     case SIZE_BWORD  :
                     case SIZE_HWORD  :
                     case SIZE_UWORD  : res->rmin = 0;
                                        res->rmax = 65535L;
                                        break;
                     case SIZE_BDWORD :
                     case SIZE_HDWORD :
                     case SIZE_UDWORD : res->rmin = 0;
                                        res->rmax = 2147483647L;
//                                        res->rmax = 4294967295.0;
                                        break;
                    #ifdef UC_REAL
                     case SIZE_REAL   : move_pREAL_REAL((_REAL*)&res->rmin, -2147483647.0);
                                        move_pREAL_REAL((_REAL*)&res->rmax, 2147483647.0);
                                        break;
                    #endif
                     default          : res->rmin = -2147483647L;
                                        res->rmax = 2147483647L;
                                        break;
                   }
                   break;
    case BIT     : res->rmin = 0;
                   res->rmax = 1;
                   break;
                   
    case FMERKER : res->rmin = -2147483647L;
                   res->rmax = 2147483647L;
                   break;
//    case RADIO   : res->rmin = 0;
//                   res->rmax = 1;
//                   break;
//    case BITMASK :
    default      : res->rmin = res->value;
                   res->rmax = res->value;
                   break;
  }
}

#ifdef UC_IPC
void MakeMinMax(_RESULT *d, _RESULT *s, _BOOL min)
{
  // vorsicht: diese funktion existiert auch in st.
  // compilerschalter UC_IPC entscheidet welche verwendet wird
  _REAL  rval;
  _DWORD dval;

 #ifdef UC_REAL
  if(d->ftype == RES_REAL)
  {
    if((s->ftype == RES_CONSTANT)||(s->ftype == RES_NUMERIC))
      rval = s->value;
    else if(s->ftype == RES_REAL)
      rval = get_pREAL((_REAL*)&s->value);

    if(min == _TRUE)
    {
      if(rval > get_pREAL((_REAL*)&d->rmin))
        move_pREAL_REAL((_REAL*)&d->rmin, rval);
    }
    else
    {
      if(rval < get_pREAL((_REAL*)&d->rmax))
        move_pREAL_REAL((_REAL*)&d->rmax, rval);
    }
  }
  else
 #endif
  {
    if((s->ftype == RES_CONSTANT)||(s->ftype == RES_NUMERIC))
      dval = s->value;
    else
      dval = FTOL((_REAL)s->value);

    if(min == _TRUE)
    {
      if(dval > d->rmin)
        d->rmin = dval;
    }
    else
    {
      if(dval < d->rmax)
        d->rmax = dval;
    }
  }
}
#endif

_BOOL _VarList::GetMinMax(_RESULT *res, _VARIABLE *p, _VARLABEL *pvl, _BOOL wait4plc, _BOOL dolimit, _BOOL cheat)
{
  _RESULT   mm;
  _VARIABLE tmp;
  _VARLABEL *vl;
 #ifdef USER_DEMAG
  _REAL     r;
  _IVALUE   lim;
 #endif

  if(GlobalChangeSys == PRINTSCREEN_IN_USE) // sa34868
  {
    wait4plc = _TRUE;
  }

 #ifndef UC_MEMORIZE_PLCDATA
  wait4plc = _TRUE;
  cheat    = _FALSE;
 #endif

//  if(GetDefaultMinMax(res, p) == _TRUE)
  GetDefaultMinMax(res, p);
  if(dolimit == _TRUE)
  {
    if(pvl->info[0].state != PARA_VAL)  
    {
      vl = &p->info.lo_limit;
      if(vl->no > 0)
      {
        if(vl->info[0].state != DEF_VAL)
        {
          if(GetSystemData(&mm, &tmp, vl, wait4plc, _FALSE, cheat) == _TRUE)
          {
            MakeMinMax(res, &mm, _TRUE);
          }
        }
      }

      vl = &p->info.hi_limit;
      if(vl->no > 0)
      {
        if(vl->info[0].state != DEF_VAL)
        {
          if(GetSystemData(&mm, &tmp, vl, wait4plc, _FALSE, cheat) == _TRUE)
          {
            MakeMinMax(res, &mm, _FALSE);
          }
        }
      }

     #ifdef USER_DEMAG
      _UWORD digs = p->info.digits;

      if(GetFixDezpt(&p->info) == _FALSE)
        digs += p->info.posdezpt;
      switch(digs)
      {
        case 1  : lim = 9L; break;
        case 2  : lim = 99L; break;
        case 3  : lim = 999L; break;
        case 4  : lim = 9999L; break;
        case 5  : lim = 99999L; break;
        case 6  : lim = 999999L; break;
        case 7  : lim = 9999999L; break;
        case 8  : lim = 99999999L; break;
        case 9  : lim = 999999999L; break;
//        case 10 : lim = 9999999999L; break;
        default : lim = 0; break;
      }

      if(lim != 0)
      {
        init_RESULT_Makro(&mm);

        if(res->ftype == RES_REAL)
        {
          r = lim;

          switch(p->info.posdezpt)
          {
            case 1 : r = r / 10; break;
            case 2 : r = r / 100; break;
            case 3 : r = r / 1000; break;
            case 4 : r = r / 10000; break;
          }

          mm.value = *(_DWORD*)&r;
          mm.rmin  = mm.value;
          mm.rmax  = mm.value;
          mm.ftype = RES_REAL;
          recalculate(&mm, p, _FALSE);
          MakeMinMax(res, &mm, _FALSE);

          r = -r;
          mm.value = *(_DWORD*)&r;
          mm.rmin  = mm.value;
          mm.rmax  = mm.value;
          mm.ftype = RES_REAL;
          recalculate(&mm, p, _FALSE);
          MakeMinMax(res, &mm, _TRUE);
        }
        else
        {
          mm.value = lim;
          mm.rmin  = mm.value;
          mm.rmax  = mm.value;
          mm.ftype = RES_NUMERIC;
          recalculate(&mm, p, _FALSE);
          MakeMinMax(res, &mm, _FALSE);

          mm.value = -lim;
          mm.rmin  = mm.value;
          mm.rmax  = mm.value;
          mm.ftype = RES_NUMERIC;
          recalculate(&mm, p, _FALSE);
          MakeMinMax(res, &mm, _TRUE);
        }
      }
     #endif
    }
  }

  return _TRUE;
}

_UWORD _VarList::GetStateScheme(_UDWORD varno, _UWORD care_state)
{
  _UDWORD     ftemp;
  _MYSCHEME   fmyscheme;
  _UWORD      sno;
  _LSEVARINFO *pv;
  _VARSYMBOL  *ps;
  _UWORD      retcode = STATE_ACTIVE;
  _VARLABEL   *pvl = NULL;
 #ifdef UC_LINKVL
  _UWORD      i;
 #endif

  if(GetVarInfo(&pv, varno) == _TRUE)
  {
  #ifdef _FIXED_STATESCHEME
   if(pv->stscheme != NOSTSCHEME)
  #endif
   {
    if(GetVarSymbol(&ps, varno) == _TRUE)
    {
      sno = pv->stscheme;

      if(care_state & NOCARE_STATESCHEME)
        sno = DEFAULTSCHEME;               // don't take care about statescheme

      if(sno != DEFAULTSCHEME)
      {
        init_MYSCHEME_Makro(&fmyscheme);
        fmyscheme.schemeno = sno;
       #ifdef UC_LINKVL
        _LINKVL *plvl;
        for(i=0,plvl=pv->memlinkvl.ptr; i<pv->memlinkvl.no; i++,plvl++)
        {
          if(plvl->info == VL_STATEOVERLOAD)
          {
            pvl = &plvl->link;
            break;
          }
        }
       #endif
        if(StateScheme.Call(&ftemp, &fmyscheme, pvl, STATE_ACTIVE) == _FALSE)
          ftemp = STATESCHEME_PRESELECTION;
        retcode = (_UWORD)ftemp;
      }

      if((care_state & NOCARE_CHKBIT) == 0)
      {

        if(chk_XXCHKBIT(pv->xxchkbit) == _FALSE)
          retcode = STATE_INVISIBLE;

      }

      if(retcode != STATE_INACTIVE)
      {
        if(ps->scope != DEFSCOPE)
        {
          switch(GetStateScheme(ps->scope, care_state))
          {
            case STATE_INACTIVE  : retcode = STATE_INACTIVE;
                                   break;
            case STATE_INVISIBLE : if(retcode != STATE_INACTIVE)
                                     retcode = STATE_INVISIBLE;
                                   break;
          }
        }
      }
    }
   }
  }

  return(retcode);
}

//#define DIRECTCALL_LOCAL_READMETHOD

#ifdef DIRECTCALL_LOCAL_READMETHOD
typedef _DWORD  (*_FDWORD_VOID) (void);
_DWORD DummyRead(_UDWORD lasalid)
{
  _FDWORD_VOID prd = **(_FDWORD_VOID**)lasalid;
  load_this(lasalid);
  return prd();
//  return get_abstime();
}
#endif

#ifdef UC_MEMORIZE_PLCDATA
_BOOL PlcMemorizeSet(_LSEVARINFO *plvi, _RESULT *res)
{
  _BOOL retcode = _FALSE;

  switch(plvi->vartype)
  {
    case FENUM     :
    case BINARY    :
//    case FENUMITEM :
//    case FCONSTANT :
//    case FMERKER   : 
                     plvi->plc_value_ftype = (res->ftype == RES_CONSTANT)? RES_NUMERIC : res->ftype;
                     plvi->plc_value       = res->value;
                     retcode               = _TRUE;
                     break;
    
    case FTIME     : if(res->ftype == RES_TIME)
                     {
                       plvi->plc_value_ftype = RES_TIME;
                       TIME_to_lasal(&plvi->plc_value, &res->datim.ftime);
                       retcode               = _TRUE;
                     }
                     break;
                     
    case FDATE     : if(res->ftype == RES_DATE)
                     {
                       plvi->plc_value_ftype = RES_DATE;
                       DATE_to_lasal(&plvi->plc_value, &res->datim.fdate);
                       retcode               = _TRUE;
                     }
                     break;
                     
    case BIT       : if((res->ftype == RES_NUMERIC) || (res->ftype == RES_CONSTANT)) 
                     {
                       if((res->value >= 0) && (res->value <= 1))
                       {
                         plvi->plc_value_ftype = RES_NUMERIC;  // res_constant ist's wenn button gedrückt wird
                         plvi->plc_value       = res->value;
                         retcode = _TRUE;
                       }
                     }
                     break;
                     
    case FSTRING   : if(res->ftype == RES_ANUMERIC)
                     {
                       retcode = _TRUE;
                       plvi->plc_value_ftype = RES_ANUMERIC;
                     }
                     break;
  }
  
  return retcode;
}

_BOOL PlcMemorizeGet(_RESULT *res, _LSEVARINFO *plvi)
{
  _BOOL retcode = _FALSE;
  
  if(plvi->plc_value_ftype != RES_DEFAULT)
  {
    switch(plvi->vartype)
    {
      case FENUM     :
      case BINARY    :
//      case FENUMITEM :
//      case FCONSTANT :
//      case FMERKER   : 
                       res->value = plvi->plc_value;
                       res->ftype = plvi->plc_value_ftype;
                       retcode    = _TRUE;
                       break;
                       
      case FTIME     : if(plvi->plc_value_ftype == RES_TIME)
                       {
                         lasal_to_TIME(&res->datim.ftime, plvi->plc_value);
                         res->ftype = RES_TIME;
                         retcode    = _TRUE;
                       }
                       break;
                       
      case FDATE     : if(plvi->plc_value_ftype == RES_DATE)
                       {
                         lasal_to_DATE(&res->datim.fdate, plvi->plc_value);
                         res->ftype = RES_DATE;
                         retcode    = _TRUE;
                       }
                       break;
                       
      case BIT       : if(plvi->plc_value_ftype == RES_NUMERIC)
                       {
                         res->value = plvi->plc_value;
                         res->ftype = RES_NUMERIC;
                         retcode    = _TRUE;
                       }
                       break;
                       
      case FSTRING   : res->ftype = RES_ANUMERIC;
                       res->ptr   = GetNullstring();
                       retcode    = _TRUE;
                       break;
                       
    }
  }
  
  return retcode;
}
#endif

_BOOL _VarList::GetValueIntern(_RESULT *res, _VARIABLE *p, _VARLABEL *vl, _BOOL wait4plc, _BOOL dolimit, _BOOL cheat)
{
  _BOOL       retcode = _FALSE;
  _UWORD      index;
  _COMPORT    *cd;
  _LSEVARINFO *plvi;

  if(GlobalChangeSys == PRINTSCREEN_IN_USE) // sa34868
  {
    wait4plc = _TRUE;
  }

  if(GetVariable(p, vl) == _TRUE)
  {
    if(GetStateScheme(p->no, NOCARE_DEFAULT) != STATE_INVISIBLE)
    {
     #ifdef UC_GLOBAL_VAR
      if(p->info.POWPRUIC & VARBIT_VARGLOBAL)
      {
        return(GetGlobalValue(res, p, vl, dolimit));
      }
     #endif

      _BOOL localvar = _FALSE; //KAMMIC

      cd = Batch.Get_COMPORT(p->info.station);
      if(cd != NULL)
      {
        if((cd->comdef.typ == _COM_LOCAL)||(cd->comdef.typ == _COM_INTERN))
        {
          wait4plc = _TRUE;
          localvar = _TRUE; //KAMMIC

         #ifdef DIRECTCALL_LOCAL_READMETHOD
          _VARLABEL xvl;
          if(p->info.lasalid.id != 0)
          {
            switch(p->info.vartype)
            {
              case FTIME  : 
              case FDATE  : 
              case BINARY : init_VARLABEL_Makro(&xvl);
                            xvl.no = 2;
                            xvl.info[0].state = PARA_VAL;
                            xvl.info[1].state = CONST_VAL;
                            xvl.info[1].value  = DummyRead(p->info.lasalid.id);
                            vl = &xvl;
                            goto DUMMYMARK;
            }
          }          
         #endif
          
        }
      }

     #ifdef UC_MEMORIZE_PLCDATA
      GetVarInfo(&plvi, p->no);
     #endif

     #ifdef UC_FASTBIT 
      if(p->info.vartype == BIT)
      {
        // speed: bei bit wird gesamtwert vom scope ermittelt und aus diesem das bit gefiltert
        _IVALUE bval;
        if(GetValueByNo(Data.MemVarSymbol.ptr[p->no].scope,  &bval, wait4plc) == _TRUE)
        {
          if(format_BIT(res, &p->info, &bval) == _TRUE)
          {
            retcode = GetMinMax(res, p, vl, wait4plc, dolimit, cheat);
           #ifdef UC_MEMORIZE_PLCDATA
            if(retcode == _TRUE)
            {
              PlcMemorizeSet(plvi, res);
            }
           #endif
          }
        }
        
       #ifdef UC_MEMORIZE_PLCDATA
        if(retcode == _FALSE)
        {
          retcode = PlcMemorizeGet(res, plvi);
        }
       #endif
        
        return retcode;
      }
     #endif
    
     #ifdef UC_MEMORIZE_PLCDATA //KAMMIC
      if((wait4plc == _TRUE) && (cheat == _TRUE) && (localvar == _FALSE))
      {
        retcode = PlcMemorizeGet(res, plvi);
        if( retcode == _TRUE)
        {
          wait4plc = _FALSE;
        }
      }
     #endif
     
      if(vl->info[0].state != PARA_VAL)
      {
        if((p->info.xadress.info[0].state == DEF_VAL)&&(p->info.vartype != FCONSTANT)&&(p->info.vartype != FMERKER)&&(p->info.vartype != FENUMITEM))
        {
         #ifndef UC_MEMORIZE_PLCDATA
          GetVarInfo(&plvi, p->no);
         #endif
         
          index = add_PLC(plvi, _FALSE, wait4plc);

//          GetVariable(p, vl);                                   // reload variable
// reload der gesamten variable ist nicht notwendig, es wurde nur xadress verändert
          copy_VARLABEL_IADDRESS(&p->info.xadress, plvi->iaddress);

          if(wait4plc == _TRUE)
          {
            if(p->info.POWPRUIC & (VARBIT_VARGLOBAL|VARBIT_VARMEMBER))
            {
              wait_for_PLC_global(index, &p->info, vl);
            }
            else
            {
              wait_for_PLC(index, &p->info, cd);
            }
          }
        }
      }


     #ifdef DIRECTCALL_LOCAL_READMETHOD
      DUMMYMARK:
     #endif

      switch(p->info.vartype)
      {
        case FENUM     :
        case BINARY    : retcode = GetBinary(res, &p->info, vl);   break;
        case FTIME     : retcode = GetTime(res, &p->info, vl);     break;
        case FDATE     : retcode = GetDate(res, &p->info, vl);     break;
        case FSTRING   : retcode = GetString(res, &p->info, vl);   break;
        case BIT       : retcode = GetBit(res, &p->info, vl);      break;
//        case RADIO     : retcode = GetBit(res, &p->info, vl);      break;
//        case BITMASK   : retcode = GetBitmask(res, &p->info, vl);  break;
        case FENUMITEM : 
        case FCONSTANT : retcode = GetConstant(res, &p->info);     break;
        case FMERKER   : retcode = GetMerker(res, p->no);          break;
        default        : break;
      }
      
      if(retcode == _TRUE)
      {
        retcode = GetMinMax(res, p, vl, wait4plc, dolimit, cheat);
       #ifdef UC_MEMORIZE_PLCDATA
        if(retcode == _TRUE)
        {
          PlcMemorizeSet(plvi, res);
        }
       #endif 
      }
      else if(wait4plc == _FALSE)
      {
        // just in case of getting no value because server is just added to
        // updatelist, we add the limits as well to updatelist --> more speed
        // during drawing picture
        // programmers-knowhow is necessary to understand (otto, did it)
        GetMinMax(res, p, vl, wait4plc, dolimit, cheat);
        
       #ifdef UC_MEMORIZE_PLCDATA
        if(cheat == _TRUE)
        {
          retcode = PlcMemorizeGet(res, plvi);
        }
       #endif 
      }
    }
  }

  if(retcode == _FALSE)
  {
    init_RESULT_Makro(res);
    retcode = VarDummyGet(res, vl);
  }
  
  return(retcode);
}

_BOOL _VarList::GetSystemData(_RESULT *res, _VARIABLE *p, _VARLABEL *vl, _BOOL wait4plc, _BOOL dolimit, _BOOL cheat)
{
  _BOOL     retcode = _TRUE;
  _VARLABEL tmpvl;

 #ifdef OLDSTYLE_CAN
  wait4plc = _TRUE;
 #endif

  switch(vl->info[0].state)
  {
    case PARA_VAL  : retcode = _FALSE;
                     // letzter _VARSINGLE beinhaltet POINTER_VAL und pointer auf daten (wert)
                     if(vl->no > 1)
                       retcode = GetValueIntern(res, p, vl, wait4plc, dolimit, cheat);
                     break;
    case CRC_VAL   : tmpvl = *vl;
                     tmpvl.info[0].value = GetVarNoByCRC(tmpvl.info[0].value);
                     tmpvl.info[0].state = VAR_VAL;
                     retcode = GetValueIntern(res, p, &tmpvl, wait4plc, dolimit, cheat);
                     break;
    case VAR_VAL   : retcode = GetValueIntern(res, p, vl, wait4plc, dolimit, cheat);
                     break;
    case BOOL_VAL  :
    case PARA_CONST: init_VARIABLE_Makro(p);
                     retcode = _FALSE;
                     if(vl->no > 1)
                     {
                       if((vl->info[1].state == POINTER_VAL)&&(vl->info[1].value != 0))
                       {
                         res->ftype  = RES_CONSTANT;
                         res->value = *(_DWORD*)vl->info[1].value;
                         res->ptr   = &res->value;
                         res->rmin  = res->value;
                         res->rmax  = res->value;
                         retcode    = _TRUE;
                       }
                     }
                     break;
    case CONST_VAL : init_VARIABLE_Makro(p);
                     res->ftype = RES_CONSTANT;
                     res->value = vl->info[0].value;
                     res->ptr   = &res->value;
                     res->rmin  = res->value;
                     res->rmax  = res->value;
                     //retcode    = _TRUE;
                     break;
    case REAL_VAL:   init_VARIABLE_Makro(p);
                     res->ftype = RES_REAL;
                     res->value = vl->info[0].value;
                     res->ptr   = &res->value;
                     res->rmin  = res->value;
                     res->rmax  = res->value;
                     break;
    case INDEX_VAL : init_VARIABLE_Makro(p);
                     res->ftype = RES_CONSTANT;
                     res->value = GetIndexValue(vl);
                     res->ptr   = &res->value;
                     res->rmin  = res->value;
                     res->rmax  = res->value;
                     //retcode    = _TRUE;
                     break;
    case POINTER_CHAR:
                     init_VARIABLE_Makro(p);
                     res->ftype = RES_ANUMERIC;
                     res->ptr   = (_CHAR*)vl->info[0].value;
                     break;

    default        : init_VARIABLE_Makro(p);
                     init_RESULT_Makro(res);
                     break;
  }

  return(retcode);
}

#ifdef UC_GLOBAL_VAR
_BOOL _VarList::GetGlobalValue(_RESULT *res, _VARIABLE *p, _VARLABEL *vl, _BOOL dolimit)
{
  _BOOL retcode = _FALSE;
  _UDWORD tmp;


  if(get_global_data(&tmp, &p->info) == _TRUE)
  {

    switch(p->info.vartype)
    {
      case BINARY:
        retcode = format_BINARY(res, &p->info, &tmp);
      break;

    }
  }

  if(retcode == _TRUE)
    retcode = GetMinMax(res, p, vl, _TRUE, dolimit, _FALSE);

  return(retcode);
}
#endif

_IVALUE _VarList::GetConstant(_VARLABEL *vl)
{
  _IVALUE retcode = 0;

  switch(vl->info[0].state)
  {
    case BOOL_VAL  :
    case CONST_VAL : retcode = vl->info[0].value;
                     break;
    case INDEX_VAL : retcode = GetIndexValue(vl);
                     break;
//    case VAR_VAL   :
    default        : if(ConvertVarlabel(vl) == _TRUE)
                       retcode = GetConstant(vl);
                     break;
  }

  return(retcode);
}

_UWORD _VarList::GetVariableSize(_VARTYPE vartype, _UWORD format)
{
  _UWORD retcode = 0;

  switch(vartype)
  {
    case FENUM    :
    case BINARY   :
                    switch(format)
                    {
                      case SIZE_BYTE   : retcode = 1; break;
                      case SIZE_WORD   : retcode = 2; break;
                      case SIZE_DWORD  : retcode = 4; break;
                      case SIZE_UBYTE  : retcode = 1; break;
                      case SIZE_UWORD  : retcode = 2; break;
                      case SIZE_UDWORD : retcode = 4; break;
                      case SIZE_HBYTE  : retcode = 1; break;
                      case SIZE_HWORD  : retcode = 2; break;
                      case SIZE_HDWORD : retcode = 4; break;
                      case SIZE_BBYTE  : retcode = 1; break;
                      case SIZE_BWORD  : retcode = 2; break;
                      case SIZE_BDWORD : retcode = 4; break;
                      case SIZE_REAL   : retcode = 4; break;
                     #ifdef PGTEST
                      default          : Annahme("T_VARIA.CPP", __LINE__);
                                         retcode = 0;
                                         break;
                     #endif
                    }
                    break;
    case FSTRING  : retcode = (format) * sizeof(_CHAR);
                    break;
    case FTIME    : retcode = 4;
                    break;
    case FDATE    : retcode = 4;
                    break;
    case FREE     : retcode = 0;
                    break;
    case FENUMITEM :
    case FCONSTANT: retcode = 0;
                    break;
//    case RADIO    :
//    case BITMASK  :
    case BIT      : switch(format)
                    {
                      case SIZE_USINT : retcode = 1; break;
                      case SIZE_UINT  : retcode = 2; break;
                      case SIZE_BYTE3 : retcode = 3; break;
                      case SIZE_UDINT : retcode = 4; break;
                     #ifdef PGTEST
                      default         : Annahme("T_VARIA.CPP", __LINE__);
                                        retcode = 1;
                                        break;
                     #endif
                    }
                    break;
  }

  return(retcode);
}

_BOOL _VarList::SetBinary(_RESULT *res, _VARINFO *p)
{
  void     *ptr;
  _BOOL     retcode = _FALSE;
  _UWORD    size;
  _DWORD    tmp;
  _COMPORT *cp;
  _UBYTE   *result;
  _UBYTE    rdat[30];
  void     *pdest = NULL;

  if((res->ftype == RES_CONSTANT)||(res->ftype == RES_NUMERIC)||(res->ftype == RES_REAL))
  {
    switch(p->format)
    {
      case SIZE_BYTE   :
      case SIZE_WORD   :
      case SIZE_DWORD  :
      case SIZE_UBYTE  :
      case SIZE_UWORD  :
      case SIZE_UDWORD :
      case SIZE_HBYTE  :
      case SIZE_HWORD  :
      case SIZE_HDWORD :
      case SIZE_BBYTE  :
      case SIZE_BWORD  :
      case SIZE_BDWORD : tmp = res->value;
                         ptr = (void*)&tmp;
                         break;
      default          : ptr  = (void*)&res->value;
                         break;
    }


    if(p->POWPRUIC & (VARBIT_VARGLOBAL|VARBIT_VARMEMBER))
    {
      cp = Batch.Get_COMPORT(p->station);

      *(_UDWORD*)&rdat[0] = p->lasalid.id + (p->bitno & 0x1F);

      rdat[4] = GetVariableSize(p->vartype, p->format);
      *(_UDWORD*)&rdat[5] = *(_UDWORD*)ptr;//GetVariableSize(p->vartype, p->format);
      if(TxCommandIntern(I_SET_DATA, 9, rdat, &cp->comdef, (void**)&result, DEFSVRTYP) == DONE)
      {
        retcode = _TRUE;
      }

      return(retcode);
    }

    if(GetPointerToData(&pdest, p, NULL) == _FALSE)
      pdest = NULL;

    size = GetVariableSize(p->vartype, p->format);
    if(pdest != NULL)
    {
      MemCpy(rdat, pdest, size);

     #ifdef UC_COMPLEX_SERVER
      if(p->POWPRUIC & VARBIT_STRUCTELEMENT)
      {
        MemCpy((_UBYTE*)pdest+p->bitno, ptr, size);
      }
      else
     #endif
      {
        MemCpy(pdest, ptr, size);
      }
    }

    retcode = send_PLC(ptr, p);

//    if((retcode == _TRUE)&&(pdest != NULL))
//    {
//      size = GetVariableSize(p);
//      MemCpy(pdest, ptr, size);
//    }

    if((retcode == _FALSE)&&(pdest != NULL))
    {
     #ifdef UC_COMPLEX_SERVER
      if(p->POWPRUIC & VARBIT_STRUCTELEMENT)
      {
        MemCpy((_UBYTE*)pdest+p->bitno, rdat, size);
      }
      else
     #endif
      {
        MemCpy(pdest, rdat, size);
      }
    }
  }

  return(retcode);
}

_BOOL _VarList::SetBit(_RESULT *res, _VARIABLE *pv)
{
  _BOOL   retcode = _FALSE;
  _UDWORD bit, value, rvalue;
  _UWORD  size;
  void    *pdest = NULL;

  if((res->ftype == RES_CONSTANT)||(res->ftype == RES_NUMERIC))
  {
    size = GetVariableSize(pv->info.vartype, pv->info.format);
    bit  = (res->value == 0)? 0 : 1;

    if(GetInvert(pv->info.POWPRUIC) == _TRUE)
      bit = bit ^  1;

    if(GetPointerToData((void**)&pdest, &pv->info, NULL) == _FALSE)
    {
      pdest = NULL;
      
     #ifdef UC_FASTBIT 
      _VARSYMBOL *pvs; 
      if(GetVarSymbol(&pvs, pv->no) == _TRUE)
      {
        _VARIABLE tv;
        if(GetSingleVariable(&tv, pvs->scope) == _TRUE)   // get single variable      
        {
          if(GetPointerToData((void**)&pdest, &tv.info, NULL) == _FALSE)
            pdest = NULL;
        }
      }
     #endif
    }
    
    if(pdest != NULL)
    {
      rvalue = 0;
      MemCpy(&rvalue, pdest, size);
      value = rvalue;
      if(bit != 0)
        value |= (1 << pv->info.bitno);
      else
        value &= (~(1 << pv->info.bitno));
      MemCpy(pdest, &value, size);
    }

    retcode = send_PLC(&bit, &pv->info);

    if((retcode == _FALSE)&&(pdest != NULL))
      MemCpy(pdest, &rvalue, size);
  }

  return(retcode);
}

_BOOL _VarList::SetString(_RESULT *res, _VARINFO *p)
{
  _BOOL  retcode = _FALSE;
  _UWORD len, maxlen;
  _CHAR *pdest = NULL;

  if((res->ftype == RES_ANUMERIC)||(res->ptr != NULL))
  {
    if(GetPointerToData((void**)&pdest, p, NULL) == _FALSE)
      pdest = NULL;

    retcode = send_PLC(res->ptr, p);

    if((retcode == _TRUE)&&(pdest != NULL))
    {
      maxlen = p->format;
      len    = StrLen((_CHAR*)res->ptr, sizeof(_CHAR));
      len    = (len > maxlen)? maxlen : len;
      StrFill(pdest, ' ', sizeof(_CHAR), maxlen);
      MemMove(pdest, (_CHAR*)res->ptr, len * sizeof(_CHAR));
      pdest[len] = 0;
    }
  }

  return(retcode);
}

_BOOL _VarList::SetTime(_RESULT *res, _VARINFO *p)
{
  _BOOL   retcode = _FALSE;
  _UWORD  size;
  void    *pdest = NULL;

  if(res->ftype == RES_TIME)
  {
    if(GetPointerToData((void**)&pdest, p, NULL) == _FALSE)
      pdest = NULL;

    TIME_to_lasal(&res->value, &res->datim.ftime);

    retcode = send_PLC(&res->value, p);

    if((retcode == _TRUE)&&(pdest != NULL))
    {
      size = GetVariableSize(p->vartype, p->format);
      MemCpy(pdest, (void*)&res->value, size);
    }
  }

  return(retcode);
}

_BOOL _VarList::SetDate(_RESULT *res, _VARINFO *p)
{
  _BOOL  retcode = _FALSE;
  _UWORD size;
  void   *pdest;

  if(res->ftype == RES_DATE)
  {
    if(GetPointerToData((void**)&pdest, p, NULL) == _FALSE)
      pdest = NULL;

    res->datim.fdate.dayofweek = day_of_week(&res->datim.fdate);
    DATE_to_lasal(&res->value, &res->datim.fdate);

    retcode = send_PLC(&res->value, p);

    if((retcode == _TRUE)&&(pdest != NULL))
    {
      size = GetVariableSize(p->vartype, p->format);
      MemCpy(pdest, &res->value, size);
    }
  }

  return(retcode);
}

#ifdef OLDSTYLE_CAN
_BOOL _VarList::SetValueSubBit(_RESULT *res, _VARIABLE *p)
{
  _RESULT    result;
  _VARIABLE  var;
  _BOOL      retcode = _FALSE;
  _COMPORT   *cp;
 #ifndef CANOPEN
  _VARSYMBOL *pvs;
  _VARLABEL vl;
  _UDWORD tmp;
 #endif

  cp = Batch.Get_COMPORT(p->info.station);
  if(cp != NULL)
  {
    if(cp->comdef.typ != _COM_INTERN)
    {
     #ifdef CANOPEN
	  // definition can open: bit immer bit 0 (lsb) und immer aus 1 byte 
      result = *res;
      if((result.ftype == RES_CONSTANT)||(result.ftype == RES_NUMERIC))
      {
        result.value = (result.value != 0)? 1 : 0; 
        var = *p; 
        var.info.vartype = BINARY;
        var.info.format  = SIZE_BBYTE;

        retcode = SetBinary(&result, &var.info);
      }
     #else
      if(GetVarSymbol(&pvs, p->no) == _TRUE)
      {
        init_VARLABEL_Makro(&vl);
        vl.no = 1;
        vl.info[0].state = VAR_VAL;
        vl.info[0].value = pvs->scope;

        if(GetSystemData(&result, &var, &vl, _TRUE, _FALSE) == _TRUE)
        {
          if((var.info.format == SIZE_BBYTE)||(var.info.format == SIZE_BWORD)||(var.info.format == SIZE_BDWORD))
          {
            if((result.ftype == RES_CONSTANT)||(result.ftype == RES_NUMERIC))
            {
              tmp = result.value;

              if(res->value == 0)
                tmp = tmp & (0xFFFFFFFFL ^ (1 << p->info.bitno));
              else
                tmp = tmp | (1 << p->info.bitno);

              result.value =
              result.rmin  =
              result.rmax  = tmp;

              retcode = SetBinary(&result, &var.info);
            }
          }
        }
      }
     #endif
    }
    else
    {
      retcode = SetBit(res, p);
    }
  }

  return(retcode);
}
#endif

_BOOL _VarList::SetValueSub(_RESULT *res, _VARIABLE *p)
{
  _BOOL retcode = _FALSE;

 #ifdef UC_MEMBERVARIABLE
  if(p->info.POWPRUIC & VARBIT_VARMEMBER)
  {
    return _FALSE; // ganber: auf membervariablen darf nicht geschrieben werden
  }
 #endif
 
  switch(p->info.vartype)
  {
    case FENUM   :
    case BINARY  : retcode = SetBinary(res, &p->info); break;
    case BIT     :
                  #ifdef OLDSTYLE_CAN
                   retcode = SetValueSubBit(res, p);
                  #else
                   retcode = SetBit(res, p);
                  #endif
                   break;
    case FSTRING : retcode = SetString(res, &p->info); break;
    case FTIME   : retcode = SetTime(res, &p->info);   break;
    case FDATE   : retcode = SetDate(res, &p->info);   break;
    case FMERKER : retcode = SetMerker(res, p->no);    break;
  }

  #ifndef CUTOFF_USER
   if(retcode == _TRUE)
   {
     _FVOID_PT_PT ptr;
     ptr = (_FVOID_PT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_WRITE_POST);
     if(ptr != NULL)
     {
       load_this(LseThisPointer);
       ptr(res, p);
     }
   }
  #endif

  return(retcode);
}

_BOOL _VarList::SetValue(_RESULT *res, _VARIABLE *p, _VARLABEL *vl, _BOOL saveimmediate, _UWORD care_state)
{
  _BOOL retcode = _FALSE;
  _BOOL ast = _FALSE;

  if(GetVariable(p, vl) == _TRUE)
  {
   #ifdef UC_MEMBERVARIABLE
    if(p->info.POWPRUIC & VARBIT_VARMEMBER)
    {
      return _FALSE; // ganber: auf membervariablen darf nicht geschrieben werden
    }
   #endif
  
    if((chk_LASALID(&p->info.lasalid) == _TRUE)||(p->info.vartype == FMERKER))
    {
      if((care_state & NOCARE_ACCESSLEVEL)||(chk_ACCESS(p->info.accessno, _FALSE) == _TRUE))
      {
        ast = _TRUE;
      }
     #ifndef CUTOFF_USER
      else
      {
        _FBOOL_PT_PT ptr;
        ptr = (_FBOOL_PT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_NOWRITEBECAUSEACCESS);
        if(ptr != NULL)
        {
          load_this(LseThisPointer);
          ast = ptr(res, vl);
        }
      }
     #endif
    
      if(ast == _TRUE)
      {
        if(GetStateScheme(p->no, care_state) == STATE_ACTIVE)
        {
          retcode = _TRUE;

         #ifndef CUTOFF_USER
          _FBOOL_PT_PT ptr;

          ptr = (_FBOOL_PT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_WRITE);
          if(ptr != NULL)
          {
            load_this(LseThisPointer);
            if(ptr(res, p) == _FALSE)
              retcode = _FALSE;
          }
         #endif

          if(retcode == _TRUE)
          {
            retcode = SetValueSub(res, p);
          }
        }
      }
    }
  }
  else 
  {
    retcode = VarDummySet(vl, res);
  }
//  if(retcode == _FALSE)
//    SystemError(CANT_WRITE_DATA);

  return(retcode);
}

_BOOL _VarList::SetSystemDataSub(_RESULT *res, _VARIABLE *p, _VARLABEL *vl, _BOOL saveimmediate, _UWORD care_state)
{
	_BOOL retcode = _FALSE;

  if(vl->info[0].state == VAR_VAL)
  {
    retcode = SetValue(res, p, vl, saveimmediate, care_state);
//   #ifdef UC_MEMORIZE_PLCDATA // daten nicht setzen !!
//    if(retcode == _TRUE)
//    {
//      _LSEVARINFO *plvi;
//      if(GetVarInfo(&plvi, p->no) == _TRUE)
//      {
//        PlcMemorizeSet(plvi, res);
//      }
//    }
//   #endif 
  }

  return(retcode);
}

_BOOL _VarList::SetSystemData(_RESULT *res, _VARIABLE *p, _VARLABEL *vl, _BOOL saveimmediate)
{
  return(SetSystemDataSub(res, p, vl, saveimmediate, NOCARE_DEFAULT));
}

_BOOL _VarList::GetValueByNo(_UDWORD varno, _IVALUE *dest, _BOOL wait4plc)
{
  _VARLABEL vl;
  _RESULT   res;
  _VARIABLE var;

  init_VARLABEL_Makro(&vl);
  vl.no = 1;
  vl.info[0].state = VAR_VAL;
  vl.info[0].value = varno;

  if(GetSystemData(&res, &var, &vl, wait4plc, _TRUE) == _TRUE)
  {
    switch(res.ftype)
    {
      case RES_CONSTANT :
      case RES_NUMERIC  : *dest = res.value;
                          return(_TRUE);
      default           : *dest = 0; break;
    }
  }

  return(_FALSE);
}

_BOOL _VarList::GetValue(_VARLABEL *pvl, _IVALUE *dest, _BOOL wait4plc, _BOOL cheat)
{
  _RESULT   res;
  _VARIABLE var;

  if(GetSystemData(&res, &var, pvl, wait4plc, _FALSE, cheat) == _TRUE)
  {
    switch(res.ftype)
    {
      case RES_CONSTANT :
      case RES_NUMERIC  : *dest = res.value;
                          return _TRUE;
                          
      case RES_REAL     : transform_RESULT(&res, RES_NUMERIC);
                          *dest = res.value;
                          return _TRUE;
                          
      default           : *dest = 0; 
                          break;
    }
  }

  return(_FALSE);
}

_BOOL _VarList::SetValueByNo(_UDWORD varno, _IVALUE val)
{
  _VARIABLE v;
  _VARLABEL vl;
  _RESULT   res;

  init_RESULT_Makro(&res);
  init_VARLABEL_Makro(&vl);

  res.ptr   = &res.value;
  res.value = val;
  res.rmin  = val;
  res.rmax  = val;
  res.ftype = RES_CONSTANT;

  vl.no            = 1;
  vl.info[0].state = VAR_VAL;
  vl.info[0].value = varno;

  return(SetSystemData(&res, &v, &vl, _TRUE));
}

_UWORD _VarList::GetCompleteNoDigits(_VARINFO *pi)
{
  _UWORD retcode = 0;

  switch(pi->vartype)
  {
    case BINARY   : retcode = GetDigits(pi);
                    if(retcode == 0)
                      retcode = 10;
                    if(GetSign(pi) == _TRUE) retcode ++;
                    if(GetDezPoint(pi) != 0) retcode ++;
                    break;
    case FSTRING  : retcode = pi->format; //GetDigits(pi);
                    break;
    case FTIME    : // retcode = GetDigits(pi);
                    retcode = CalculDigits(pi, 0xFFFF);
                    break;
    case FDATE    : // retcode = GetDigits(pi);
                    retcode = CalculDigits(pi, 0xFFFF);
                    break;
//    case RADIO    :
    case BIT      : retcode = 1;
                    break;

  }

  return(retcode);
}

_BOOL _VarList::GetSign(_VARINFO *pi)          
{
  _BOOL retcode = _FALSE; 

  if(pi->vartype == BINARY)
  {
    switch(pi->format)
    {
      case SIZE_REAL  :
      case SIZE_BYTE  :
      case SIZE_WORD  :
      case SIZE_DWORD : retcode = _TRUE;
                        break;
    }
  }
  return(retcode);
}

_CHAR* _VarList::GetTextByNo(_UDWORD varno, _UWORD txtno)
{
  _ANYTHING anything;
  _CHAR     *retcode = NULL;

  if(varno < Data.MemLseVarInfo.no)
  {
    init_ANYTHING_Makro(&anything);

    switch(txtno)
    {
      case  0 : anything.list = VARTXT1; break;
      case  1 : anything.list = VARTXT2; break;
      case  2 : anything.list = VARTXT3; break;
      case  3 : anything.list = VARTXT4; break;
      case  4 : anything.list = VARNAME; break;
      case  5 : anything.list = VARUNIT; break;
      default : anything.list = VARNAME; break;
    }

    anything.no = varno;

    retcode = get_text(&anything, NULL, NULL, _TRUE);
  }

  return(retcode);
}

_BOOL  _VarList::GetFixDezpt(_VARINFO *pi)
{ return((pi->POWPRUIC&VARBIT_FIXDEZPT)?_TRUE:_FALSE);}
_BOOL  _VarList::GetTool(_UDWORD parameter)
{ return((parameter&0x01)? _TRUE:_FALSE);}
_BOOL  _VarList::GetProt(_UDWORD parameter)
{ return((parameter&0x02)? _TRUE:_FALSE);}
_BOOL  _VarList::GetPde(_UDWORD parameter)
{ return((parameter&0x04)? _TRUE:_FALSE);}


_BOOL  _VarList::GetZero(_UWORD powpruic)
{ return((powpruic&VARBIT_PREZERO)? _TRUE:_FALSE);}
_BOOL  _VarList::GetPermanent(_UWORD powpruic)
{ return((powpruic&VARBIT_PERMANENT)? _TRUE:_FALSE);}
_BOOL  _VarList::GetInvert(_UWORD powpruic)
{ return((powpruic&VARBIT_INVERT)? _TRUE:_FALSE);}
_UWORD _VarList::GetDigits(_VARINFO *pi)
{ return(pi->digits); }
_DWORD _VarList::GetDezPoint(_VARINFO *pi)
{ return(pi->posdezpt); }
_UDWORD _VarList::GetNo(void)
{ return(Data.MemLseVarInfo.no); }
_AsciiPuffer* _VarList::GetLabelGroup(void)
{ return(&LabelGroup); }
_AsciiPuffer* _VarList::GetLabelUnit(void)
{ return(&LabelUnit); }


// ***************************************************************************
// ***************************************************************************

_BOOL _VarList::GetLabel(_ASCII *txt, _UBYTE *varobj, _UDWORD nr)
{
	return(GetLabelVARBLOCK(txt, varobj, nr, &Data));
}

_ASCII *_VarList::GetLabelToken(_UDWORD varno)  // get labeltoken of variable
{
  //if(Data != NULL)
  {
    //if(Data.Label != NULL)
    {
      if(varno < Data.Label.GetNo())
      {
        return Data.Label.GetText(varno);
      }
    }
  }
  return NULL;
}


_UDWORD _VarList::ScanLabel(_UDWORD scope, _ASCII *name)
{
  _UDWORD          i, anz;
  _UWORD           len;
  _ASCII           *lab;
  _VARSYMBOL       *ps;
  _MEM_POINTERPARA *mpp;

  mpp = Data.Label.GetMemPointer();
  ps  = Data.MemVarSymbol.ptr;

  if(mpp->no == Data.Label.GetNo())
  {
    // map für namenliste existiert
    if(scope != DEFSCOPE)
    {
      // suche beginnt ab scope, annahme dass server erst nach eigentlichem
      // objektnamen in liste gespeichert ist
      i   = scope;
      ps  = &Data.MemVarSymbol.ptr[i];
      anz = Data.MemVarSymbol.no;

      while(anz)
      {
        anz -= 1;

        if(ps->scope == scope)
        {
          lab = (_ASCII*)mpp->ptr[i].ptr;

          if(name[0] == lab[0])
          {
            if(StrXmp(lab, name, sizeof(_ASCII)) == 0)
            {
              return(i);
            }
          }
        }

        i  += 1;
        ps += 1;

        if(i >= Data.MemVarSymbol.no)
        {
          ps = Data.MemVarSymbol.ptr;
          i  = 0;
        }
      }
    }
    else
    {
      // suche ab listenanfang

      for(i=0; i<Data.MemVarSymbol.no; i++)
      {
        if(ps->scope == scope)
        {
          lab = (_ASCII*)mpp->ptr[i].ptr;

          if(name[0] == lab[0])
          {
            if(StrXmp(lab, name, sizeof(_ASCII)) == 0)
            {
              return(i);
            }
          }
        }
        ps  += 1;
      }
    }
  }
  else
  {
    // map für namenliste existiert nicht
    lab = Data.Label.GetText(0);

    for(i=0; i<Data.MemVarSymbol.no; i++)
    {
      len = StrLen(lab, sizeof(_ASCII)) + 1;
      if(ps->scope == scope)
      {
        if(name[0] == lab[0])
        {
          if(MemCmp(lab, name, len) == 0)
          {
            return(i);
          }
        }
      }
      lab += len;
      ps  += 1;
    }
  }

  return(DEFSCOPE);
}

_UDWORD _VarList::FindIndexCRC(_CRC32 crc32, _DWORD left, _DWORD right)
{
  _DWORD  act;
  _UDWORD xcrc, index;

  if(left <= right)
  {
    act   = (left + right) / 2;
    index = MapCRC.ptr[act];
    xcrc  = Data.MemLseVarInfo.ptr[index].crc32;

    if(crc32 == xcrc)
      return(index);

    if(crc32 > xcrc)
      return(FindIndexCRC(crc32, act+1, right));

    return(FindIndexCRC(crc32, left, act-1));
  }

  return(DEFSCOPE);
}

//_UDWORD _VarList::FindAround(_UDWORD index, _CRC32 crc32, _ASCII *label)
//{
//  _UDWORD i, end;
//  _UDWORD *pvno;
//  _ASCII  cmp[MAX_LABELLEN];
//  _UBYTE  varobj;
//  _CRC32  xcrc;
//
//  if(index < 15)
//    index = 0;
//  else
//    index -= 15;
//
//  end = index + 30;
//  if(end > MapCRC.no)
//    end = MapCRC.no;
//
//  pvno = MapCRC.ptr;
//  for(i=index; i<end; i++, pvno++)
//  {
//    xcrc  = Data.MemLseVarInfo.ptr[*pvno].crc32;
//    if(xcrc == crc32)
//    {
//      varobj = 0;
//      if(GetLabel(cmp, &varobj, *pvno) == _TRUE)
//      {
//        if(StrXmp(label, cmp, sizeof(_ASCII)) == 0)
//          return(*pvno);
//      }
//    }
//  }
//
//  return(DEFSCOPE);
//}

#ifdef UC_CASE_SENSITIVE
_UDWORD _VarList::GetVarNo(_ASCII *label, _UDWORD station)
{
  _UWORD      len = StrLen(label, sizeof(_ASCII));
  _CRC32      crc32;
  _UDWORD     i;
  _LSEVARINFO *pvi;
  _ASCII      cmp[MAX_LABELLEN];
  _UBYTE      varobj;

  if(len > 0)
  {
    crc32 = MakeCrc32(station, label, len);
    pvi   = Data.MemLseVarInfo.ptr;

    i = FindIndexCRC(crc32, 0, MapCRC.no-1);
    
    if(i == DEFSCOPE)
    {
      crc32 = MakeCrc32(0xFFFFFFFFL, label, len); // eventuell ist konstante oder merker gesucht
      i = FindIndexCRC(crc32, 0, MapCRC.no-1);
    }
    
    if(i != DEFSCOPE)
    {
      varobj = 0;
      if(GetLabel(cmp, &varobj, i) == _TRUE)
      {
        if(StrXmp(label, cmp, sizeof(_ASCII)) == 0)
        {
          return(i);
        }
        else
        {
          // ggf. ist crc mehrfach vorhanden ...einfach suchen
          for(i=0; i<Data.MemLseVarInfo.no; i++, pvi++)
          {
            if(pvi->crc32 == crc32)
            {
              varobj = 0;
              if(GetLabel(cmp, &varobj, i) == _TRUE)
              {
                if(StrXmp(label, cmp, sizeof(_ASCII)) == 0)
                {
                  return(i);
                }
              }
            }
          }
        }
      }
    }
  }

  return(DEFSCOPE);
}
#else
_UDWORD _VarList::GetVarNo(_ASCII *txt, _UDWORD station)
{
  _ASCII  savetxt[200], label[200];
  _UDWORD i, clen;
  _UBYTE  varobj = 0;

  clen = StrLen(txt, sizeof(_ASCII));

  if(clen > 0)
  {
    MemCpy(savetxt, txt, clen+1);
    StrUp(savetxt, sizeof(_ASCII), clen);

    for(i=0; i<Data.no; i++)
    {
     #ifndef THR
      if((Data.MemLseVarInfo.ptr[i].station == station) ||(Data.MemLseVarInfo.ptr[i].station == 0xFFFFFFFFL))
     #endif
      {
        if(GetLabelVARBLOCK(label, &varobj, i, &Data) == _TRUE)
        {
          if(label[0] == savetxt[0])
	        {
            if(StrXmp(label, savetxt, sizeof(_ASCII)) == 0)
              return i;
//		        if(StrLen(label, sizeof(_ASCII)) == clen)
//		        {
//	            if(MemCmp(label, savetxt, clen) == 0)
//		          {
//		            return(i);
//              }
//            }
          }
        }
      }
    }
  }

  return(DEFSCOPE);
}
#endif
/*
_UWORD _VarList::GetComponentLength(_ASCII *txt)
{
	_ASCII chr;
	_UWORD retcode = 0;
	_BOOL  run = _TRUE;

	do
  {
    chr = *txt;
    if((chr == '.')||(chr == '\\')||(chr == 0))
			run = _FALSE;
    else
      retcode ++;

    txt += 1;
	}
	while(run == _TRUE);

	return(retcode);
}

_UDWORD _VarList::GetVarNo(_ASCII *txt)
{
// COMPILER_VERSION 18
	_ASCII  clabel[32], savetxt[200], *xtxt;
	_UWORD  retcode = DEFSCOPE;
	_UWORD  clen, xlen, len;

  xtxt = txt;
  xlen =
  len  = StrLen(txt, sizeof(_ASCII)) + 1;

	while(len > 0)
	{
		clen = GetComponentLength(txt);
		MemCpy(clabel, txt, clen);
		clabel[clen] = 0;
	 #ifndef UC_CASE_SENSITIVE
		StrUp(clabel, sizeof(_ASCII), clen); //StrLen(clabel, sizeof(_ASCII)));
	 #endif
		retcode = ScanLabel(retcode, clabel);
		clen ++;
		len -= clen;
		txt  = &txt[clen];
	}

	if(retcode == DEFSCOPE)
	{
    MemCpy(savetxt, xtxt, xlen);
	 #ifndef UC_CASE_SENSITIVE
		StrUp(savetxt, sizeof(_ASCII), xlen); //StrLen(savetxt, sizeof(_ASCII)));
	 #endif
		retcode = ScanLabel(DEFSCOPE, savetxt);
	}

	return(retcode);
}
*/
_BOOL _VarList::GetPointerToData(void **ptr, _VARINFO *p, _VARLABEL *pvl)
{
  _VARSINGLE *pv;

  pv   = &p->xadress.info[0];
  *ptr = NULL;

  if(pvl != NULL)
  {
    if(pvl->no > 1)
    {
      if(pvl->info[0].state == PARA_VAL)
      {
        pv = &pvl->info[pvl->no-1];
        if(pv->state == POINTER_VAL)
          *ptr = (void*)pv->value;
        else if(pv->state == CONST_VAL)
          *ptr = (void*)&pv->value;
      }

			return((*ptr != NULL)? _TRUE : _FALSE);
    }
  }

  if(pv->state == REFINDEX_VAL)
  {
    // value is an index to reflist
    *ptr = get_PLC(p, (_UWORD)pv->value);
  }
//  else if((p->station == DEFSTATION) && (pv->state == CONST_VAL))
//  {
//    // value is an offset in IpcMemory
//    *ptr = IpcMemory.GetPointer(pv->value);
//  }

  return((*ptr != NULL)? _TRUE : _FALSE);
}
/*
_BOOL _VarList::InsertLocalAdress(_ASCII *name, _UDWORD offset)
{
  _UWORD     no, i, len;
  _VARINFO   *p;
  _ASCII     *ptr, chr;

 #ifndef UC_CASE_SENSITIVE
  _ASCII cnam[120];
  StrCpy(cnam, sizeof(_ASCII), name, sizeof(_ASCII));
	StrUp(cnam, sizeof(_ASCII), StrLen(cnam, sizeof(_ASCII)));
  name = cnam;
 #endif

  if(name != NULL)
  {
    ptr = Data.Label.GetPtr();
    no  = Data.Label.GetNo();
    chr = name[0];

    for(i=0; i<no; i++)
    {
      // don't use Label.Seek() because speed is necessary !!
      len = StrLen(ptr, sizeof(_ASCII)) + 1;
      if(ptr[0] == chr)
      {
        if(MemCmp(ptr, name, len) == 0)
        {
          // bingo
          if(GetVarInfo(&p, i) == _TRUE)
          {
            if((Data.MemVarSymbol.ptr[i].scope == DEFSCOPE)&&(p->station == DEFSTATION))
            {
              p->xadress.no            = 1;
              p->xadress.info[0].state = CONST_VAL;
              p->xadress.info[0].value = (_DWORD)offset;
              return(_TRUE);
            }
          }
        }
      }
      ptr += len;
    }
  }

  return(_FALSE);
}
*/

#ifdef UC_ALARMID_FAST
 
 
#ifdef UC_ALARMID_FAST_SORT
#include "..\..\Class\SigCLib\SigCLib.h"

long Cmp_LINDEX(const void *p0, const void *p1)
{
  _LINDEX *pl0 = (_LINDEX*)p0;
  _LINDEX *pl1 = (_LINDEX*)p1;
  
  if(pl0->lasalid < pl1->lasalid) return -1;
  if(pl0->lasalid > pl1->lasalid) return 1;
  return 0;
}
#endif
 
_DWORD _VarList::Make_MEM_LINDEX(_MEM_LINDEX *p, _LSEVARINFO *pvi, _UDWORD pvi_no, _UBYTE station)
{
  _DWORD retcode = -1; // ann. too less memory

  _LINDEX *ptmp = NULL;
  if(Memory2((void**)&ptmp, pvi_no*sizeof(_LINDEX)) != 0)
  {
    retcode = 0;
    _LINDEX *ph = ptmp;
    for(_UDWORD i=0; i<pvi_no; i++, pvi++)
    {
      if((pvi->station == station) && (pvi->lasalid.id != 0))
      {
        if((pvi->vartype != OBJECT) && (pvi->vartype != BIT) && (pvi->vartype != FENUMITEM))
        {
          if((pvi->POWPRUIC & (VARBIT_VARGLOBAL | VARBIT_STRUCTELEMENT | VARBIT_VARMEMBER)) == 0)
          {
            ph->index   = i;
            ph->lasalid = pvi->lasalid.id;
            ph++;
            retcode ++;
          }
        }
      }
    }
    
    Memory2((void**)&ptmp, retcode*sizeof(_LINDEX));
    
   #ifdef UC_ALARMID_FAST_SORT
    sigclib_qsort(ptmp, retcode, sizeof(_LINDEX), Cmp_LINDEX);
   #endif
   
    p->no   = retcode;
    p->ptr  = ptmp;
    retcode = 1; // well done
  }
  
  return retcode;
}
 
_MEM_LINDEX *_VarList::Get_MEM_LINDEX(_LSEVARINFO *pvi, _UDWORD pvi_no, _UBYTE station)
{
  if(station < lib_arraysize(MlStation))
  {
    _MEM_LINDEX *ps = &MlStation[station];

    if(ps->done == 0)
    {
      ps->done = Make_MEM_LINDEX(ps, pvi, pvi_no, station);
    }
    
    return ps;
  }

  return NULL;
}

_UDWORD _VarList::Seek_MEM_LINDEX(_MEM_LINDEX *ps, _UDWORD lasalid)
{
#ifdef UC_ALARMID_FAST_SORT
  _LINDEX key;
  key.lasalid = lasalid;
  key.index = DEFSCOPE;
  _LINDEX *p = (_LINDEX*)sigclib_bsearch(&key, ps->ptr, ps->no, sizeof(_LINDEX), Cmp_LINDEX);
  if(p != NULL)
  {
    return p->index;
  }

#else

  _LINDEX *p = ps->ptr;
  _UDWORD nox = ps->no;
  while(nox--)
  {
    if(p->lasalid == lasalid)
    {
      return p->index;
    }
    p++;
  }
#endif

  return DEFSCOPE;
}

#endif

_UDWORD _VarList::GetVarNoByID(_UDWORD id, _UWORD station, _BOOL justserver)
{
  if(id != 0)
  {
  
   #ifdef UC_ALARMID_FAST
    if(justserver == _TRUE) // fastseek
    {
      _MEM_LINDEX *pms = Get_MEM_LINDEX(Data.MemLseVarInfo.ptr, Data.MemLseVarInfo.no, station);
      if(pms != NULL)
      {
        if(pms->done > 0)
        {
          return Seek_MEM_LINDEX(pms, id);
        }
      }
    }
   #endif
  
    _LSEVARINFO *p = Data.MemLseVarInfo.ptr;
    for(_UDWORD i=0; i<Data.MemLseVarInfo.no; i++)
    {
      if((p->lasalid.id == id)&&(p->station == station))
      {
        if(justserver == _FALSE)
        {
          return(i);
        }
        else if((p->vartype != OBJECT)&&(p->vartype != BIT))
        {
          return(i);
        }
      }


//      if((p->lasalid.id == id)&&(p->station == station)&&
//         ((p->vartype != OBJECT)||(justserver == _FALSE)))
//        return(i);

      p += 1;
    }
  }

  return(DEFSCOPE);
}

_UDWORD _VarList::GetEnumItemVarNo(_IVALUE value, _UDWORD varno)
{
  _VARSYMBOL *p = Data.MemVarSymbol.ptr;;

  for(_UDWORD i=0; i<Data.MemLseVarInfo.no; i++, p++)
  {
    if(p->scope == varno)
    {
      _RESULT   res;
      _VARIABLE variable;
      if(GetSingleVariable(&variable, i) == _TRUE)
      {
        if(variable.info.vartype == FENUMITEM)
        {
          if(GetConstant(&res, &variable.info) == _TRUE)
          {
            if(res.value == value)
            {
              return i;
            }
          }
        }
      }
    }
  }
  
  return DEFSCOPE;
}

_BOOL _VarList::GetEnumItems(_MEM_NO *pmemo, _UDWORD varno)
{
  _VARSYMBOL *p;
  _UDWORD    nox = 0;

  free_MEM_NO(pmemo);

  p=Data.MemVarSymbol.ptr;
  for(_UDWORD i=0; i<Data.MemLseVarInfo.no; i++, p++)
  {
    if(p->scope == varno)
    {
      if(nox == 0)
      {
        if(Memory2((void**)&pmemo->ptr, (pmemo->no + 20) * sizeof(_DWORD)) == 0)
          return _FALSE;
        nox = 20;
      }  
      
      pmemo->ptr[pmemo->no] = i;
      pmemo->no += 1;
      nox -= 1;
    }
  }
  
  return _TRUE;
}

_BOOL _VarList::GetVarSymbol(_VARSYMBOL **p, _UDWORD varno)
{
  if(varno < Data.MemVarSymbol.no)
  {
    *p = &Data.MemVarSymbol.ptr[varno];
    return _TRUE;
  }

  return _FALSE;
}

_UDWORD _VarList::LookUpIfInRootIsSameType(_UDWORD varno, _UDWORD classid)
{
  _UDWORD retcode = DEFSCOPE;

  if(varno < Data.no)
  {
    if(Data.MemLseVarInfo.ptr[varno].classid == classid)
      retcode = varno;
    else
      retcode = LookUpIfInRootIsSameType(Data.MemVarSymbol.ptr[varno].scope, classid);
  }

  return(retcode);
}

_UDWORD _VarList::GetVarNo_RootParent(_UDWORD varno)
{
  _UDWORD retcode = DEFSCOPE;

  while(varno < Data.no)
  {
    _UDWORD scope = Data.MemVarSymbol.ptr[varno].scope;
    if(scope == DEFSCOPE)
    {
      return retcode;
    }
    
    retcode = scope;
    varno   = scope;
  }
  
  return DEFSCOPE;
}

_LSEVARINFO* _VarList::Get_LSEVARINFO_RootParent(_UDWORD varno)
{
  _UDWORD root = GetVarNo_RootParent(varno);
  if(root < Data.MemLseVarInfo.no)
  {
    return &Data.MemLseVarInfo.ptr[root];
  }
  return NULL;
}

_LSEVARINFO *_VarList::GetRootInfo(_UDWORD varno)
{
  _LSEVARINFO *retcode = NULL;

  if(varno < Data.no)
  {
    _UDWORD scope = Data.MemVarSymbol.ptr[varno].scope;
    if(scope >= Data.no)
    {
      retcode = &Data.MemLseVarInfo.ptr[varno];
    }
    else
      retcode = GetRootInfo(scope);
  }

  return retcode;
}
/*
_UDWORD _VarList::FindSameType(_UDWORD classid, _UDWORD range)
{
  _UDWORD     i;
  _LSEVARINFO *pvi;

  if(range > Data.no)
    range = Data.no;

  pvi = Data.MemLseVarInfo.ptr;
  for(i=0; i<range; i++)
  {
    if(pvi->classid == classid)
      return(i);
    pvi += 1;
  }

  return(DEFSCOPE);
}
*/

_VARBLOCK *_VarList::GetPointerToData(void)
{
  return(&Data);
}

_ASCII *_VarList::GetLabelInternVARBLOCK(_ASCII *txt, _UBYTE *varobj, _UDWORD nr, _VARBLOCK *vb)
{
  _UDWORD     scope;
  _ASCII      *tmp;
  _UWORD      len;
  _LSEVARINFO *pvi;

  if((nr < vb->MemVarSymbol.no)&&(txt != NULL))
  {
    pvi = &vb->MemLseVarInfo.ptr[nr];

    if(pvi->vartype == OBJECT)
      *varobj = 1;

    scope = vb->MemVarSymbol.ptr[nr].scope;
    if(scope != DEFSCOPE)
    {
      txt = GetLabelInternVARBLOCK(txt, varobj, scope, vb);
      if(pvi->vartype == OBJECT)
        *txt = 92; //'\'
      else
        *txt = 46; //'.'
      txt +=1;
    }

    tmp = vb->Label.GetText(nr);
    len = StrLen(tmp, sizeof(_ASCII));
    MemCpy(txt, tmp, len+1);
    return(txt+len);
  }

  return(NULL);
}

_BOOL _VarList::GetLabelVARBLOCK(_ASCII *txt, _UBYTE *varobj, _UDWORD nr, _VARBLOCK *vb)
{
  txt[0]  = 0;
  *varobj = 1;   // slurry
  if(GetLabelInternVARBLOCK(txt, varobj, nr, vb) != NULL)
  {
    return _TRUE;
  }

  return _FALSE;
}

_CHAR *_VarList::GetUnitText(_UWORD unitno)
{
  _CHAR *retcode = NULL;
  _UNIT *pu;

  pu = GetUnitInfo(unitno);
  if(pu != NULL)
    retcode = get_ANYPARA(&pu->anypara, NULL);

  return(retcode);
}

_BOOL _VarList::GetVarIdChanged(void)
{
  return(VarIdChanged);
}

_UDWORD _VarList::GetVarNoByCRC(_CRC32 crc32)
{

  return(FindIndexCRC(crc32, 0, MapCRC.no-1));
}

#ifdef OLDSTYLE_CAN
void _VarList::CalculatePgAddresses(void)
{
  calculate_address(&Data);
}

_BOOL _VarList::ChangeBaseAddress(_UDWORD varno, _UDWORD address)
{
  _BOOL retcode = _FALSE;

  if(change_baseaddress(&Data, varno, address) == _TRUE)
  {
    calculate_address(&Data);
    new_PLC();
    retcode = _TRUE;
  }

  return(retcode);
}
#endif

_BOOL _VarList::CheckHostnameSub(_UDWORD no)
{
  _BOOL       retcode = _FALSE;
  _LSEVARINFO *plvi;

  if(GetVarInfo(&plvi, no) == _TRUE)
  {
    if(chk_LASALID(&plvi->lasalid) == _TRUE)
    {
      if(chk_XXCHKBIT(plvi->xxchkbit) == _TRUE)
      {
//      abfrage (aufruf) userinterface eventuell notwendig, auswertung fraglich 
        retcode = _TRUE;
      }
    }
  }

  return(retcode);
}

_BOOL _VarList::CheckHostname(_BOOL do_scanbar, _BOOL stop_at_double_name, _BOOL reload_if_damaged)
{
  _BOOL   retcode = _TRUE;
  _UWORD  bs;
  _UDWORD i1, i2, crc32;
  _ASCII  *pt1, *pt2;
  _BOOL   skip, s1, s2;

  if((HostName.name.no > 0)&&(SetUp.GetUseHostName() == _TRUE))
  {
    if(do_scanbar == _TRUE)
      bs = ScanBar.Add("Chk Hostname:", HostName.name.no);

    crc32 = make_crc_ASCII_BUFFER(&HostName.name);

    if(crc32 != HostName.crc32)
    {
      if(reload_if_damaged == _TRUE)
      {
        ReloadHostName();
        if(do_scanbar == _TRUE)
          ScanBar.End(bs);
        return(CheckHostname(do_scanbar, stop_at_double_name, _FALSE));
      }

      retcode = _FALSE;
    }
    else if((HostName.name.no > 1)&&(stop_at_double_name == _TRUE))
    {
      i1  = 0;
      pt1 = HostName.name.ptr;
      pt2 = HostName.name.ptr;

      for(i2=1; i2<HostName.name.no; i2++)
      {
        if(do_scanbar == _TRUE)
          ScanBar.Run(bs, i2);

        skip = _TRUE;
        pt2  = pt2 + StrLen(pt1, sizeof(_ASCII)) + 1;

        if((*pt1 == *pt2)&&(*pt1 != 0))
        {
          if(StrXmp(pt1, pt2, sizeof(_ASCII)) == 0)
          {
            s1 = CheckHostnameSub(HostName.index.ptr[i1]);
            s2 = CheckHostnameSub(HostName.index.ptr[i2]);

            if((s1 == _TRUE) && (s2 == _FALSE))
            {
              skip = _FALSE;
            }
            else if((s1 == _TRUE) && (s2 == _TRUE))
            {
              // do hods wos !!!
              _UBYTE  varobj = 0;
              _UDWORD vno1 = HostName.index.ptr[i1];
              _UDWORD vno2 = HostName.index.ptr[i2];
              _ASCII txt[512];
              
              txt[0] = 32;
              txt[1] = 34;
              StrCpy(&txt[2], sizeof(_ASCII), pt1, sizeof(_ASCII));
              _UWORD len = StrLen(txt, sizeof(_ASCII));
              txt[len] = 34;
              StrCpy(&txt[len+1], sizeof(_ASCII), " at ", sizeof(_ASCII));
              
              GetLabel(&txt[StrLen(txt, sizeof(_ASCII))], &varobj, vno1); // get name of variable1
              StrCat(txt, sizeof(_ASCII), " and ", sizeof(_ASCII));
              varobj = 0;
              GetLabel(&txt[StrLen(txt, sizeof(_ASCII))], &varobj, vno2); // get name of variable2
              ScanBar.End(ScanBar.Add(txt, 0));
              retcode = _FALSE;
              //break;
            }
//          else if((s1 == _FALSE) && (s2 == _FALSE))
//          {
//            skip = TRUE;
//          }
//          else if((s1 == _FALSE) && (s2 == _TRUE))
//          {
//            skip = TRUE;
//          }
          }
        }

        if(skip == _TRUE)
        {
          i1  = i2;
          pt1 = pt2;
        }
      }
    }

    if(do_scanbar == _TRUE)
      ScanBar.End(bs);
  }

  return(retcode);
}

_NAME_INDEX* _VarList::GetHostName(void)
{
  return(&HostName);
}

_ASCII *_VarList::GetHostName(_ASCII *label, _UWORD station)
{
  return(GetHostNameByVarNo(GetVarNo(label, station)));
}

_ASCII *_VarList::GetHostNameByVarNo(_UDWORD varno)
{
  _LSEVARINFO *p;
  _ASCII      *retcode = NULL;

  if(GetVarInfo(&p, varno) == _TRUE)
    retcode = gettext_ASCII_BUFFER(&HostName.name, p->hostname);

  return(retcode);
}

_ASCII *_VarList::GetHostNameByCrc(_CRC32 crc32)
{
  return(GetHostNameByVarNo(GetVarNoByCRC(crc32)));
}

_UDWORD _VarList::GetVarNoByHostName(_ASCII *hostname)
{
  _UDWORD no;
  _UDWORD retcode = DEFSCOPE;

  no = findtext_ASCII_BUFFER(&HostName.name, hostname);
  if(no < HostName.index.no)
    retcode = HostName.index.ptr[no];

  return(retcode);
}

_BOOL _VarList::GetOnlyAscii(_VARINFO *pvi)
{
  _BOOL retcode = _FALSE;


  if(pvi->vartype == FSTRING)
  {
    if(pvi->bitno > 0)
      retcode = _TRUE;

  }

  return(retcode);
}


_BOOL _VarList::GetRootParent(_LSEVARINFO **pvi, _ASCII *label, _UBYTE station)
{
  _BOOL retcode = _FALSE;
  _UWORD len, i;
  _ASCII *chr, save;
  _UDWORD varno;


  len = StrLen(label, sizeof(_ASCII));
  chr = label;

  for(i=0; i<len; i++)
  {
    if(*chr == '.')
    {
      save = *chr;
      *chr = 0;
      retcode = _TRUE;
      break;
    }

    chr++;
  }

  if(retcode == _TRUE)
  {
    retcode = _FALSE;
    varno = GetVarNo(label, station);
    *chr = save;
    if(varno != 0xFFFFFFFF)
    {
      if(GetVarInfo(pvi, varno) == _TRUE)
        retcode = _TRUE;
    }
  }

  return(retcode);
}

_UDWORD _VarList::GetNodeObjectNumber(_UDWORD varno)
{
  _VARSYMBOL *pvs;
  if(GetVarSymbol(&pvs, varno) == _TRUE)
  {
    _LSEVARINFO *pvi;
    if(GetVarInfo(&pvi, pvs->scope) == _TRUE)
    {
      if(pvi->vartype == OBJECT)
        return pvs->scope;
      
      return GetNodeObjectNumber(pvs->scope);
    }
  }
  
  return DEFSCOPE;
}


#ifdef UC_IPC
_BOOL _VarList::MakeReflistMap(_MEM_NO *dst)
{
  _LSEVARINFO *p;
  _UDWORD     i, anz = 0;
  _BOOL       retcode = _FALSE;

  if(dst != NULL)
  {
    p = Data.MemLseVarInfo.ptr;
    i = Data.MemLseVarInfo.no;
    while(i)
    {
      if(p->iaddress != DEF_IADDRESS) // geschwindigkeitsoptimierung !!
        anz += 1;

      i -= 1;
      p += 1;
    }

    if(Memory2((void**)&dst->ptr, anz * sizeof(_UDWORD)) != 0)
    {
      p = Data.MemLseVarInfo.ptr;
      for(i=0; i<Data.MemLseVarInfo.no; i++)
      {
        if(p->iaddress != DEF_IADDRESS) // geschwindigkeitsoptimierung !!
        {
          dst->ptr[dst->no++] = i;
        }

        p += 1;
      }
      retcode = _TRUE;
    }
  }

  return(retcode);
}
#endif


_BOOL _VarList::GetXIndex(_DWORD *pxindexval, _UDWORD percent_x_varno)
{
 #ifdef UC_LINKVL

  _LSEVARINFO *pvi;
  if(GetVarInfo(&pvi, percent_x_varno) == _TRUE)
  {
    _LINKVL *plvl;
    _UWORD zi;
    for(zi=0,plvl=pvi->memlinkvl.ptr; zi<pvi->memlinkvl.no; zi++,plvl++)
    {
      if(plvl->info == VL_XINDEX)
      {
        if(VarList.GetValue(&plvl->link, pxindexval, _TRUE) == _TRUE)
          return _TRUE;
      }
    }

   _VARSYMBOL *pvs;
   if(GetVarSymbol(&pvs, percent_x_varno) == _TRUE)
   {
     if(pvs->scope != DEFSCOPE)
       return GetXIndex(pxindexval, pvs->scope);
   }
  }
 #endif
 
  return _FALSE;
}


//typedef struct { _ASCII_BUFFER name;
//                 _MEM_NO       index;
//               }
//_NAME_INDEX;


/* ************************************************************************* */
/* ************** objekte während laufzeit instanzieren *******************  */
/* ************************************************************************* */
/*
_BOOL _VarList::AddAllServer(_UWORD station, _UDWORD typno, _UDWORD scope)
{
  _UDWORD     i;
  _LSEVARINFO *pi;
  _LSEVARINFO varinfo;
  _VARSYMBOL  *ps;
  _ASCII      *pl;
  _UDWORD     tmpscope;
  _BOOL       retcode = _TRUE;

  if(typno < Types.no)
  {
    pi = Types.MemLseVarInfo.ptr;
    ps = Types.MemVarSymbol.ptr;
    pl = Types.Label.GetPtr();
    for(i=0; i<Types.MemVarSymbol.no; i++)
    {
      if(retcode == _TRUE)
      {
        if(ps->scope == typno)
        {
          init_LSEVARINFO_Makro(&varinfo);
          if(copy_LSEVARINFO(&retcode, &varinfo, pi) == _TRUE)
          {
            varinfo.station = station;
            tmpscope = Data.MemVarInfo.no;
            retcode = AddNewVariable(pl, &varinfo, scope);

            if((pi->vartype == OBJECT)&&(retcode == _TRUE))
              retcode = AddAllServer(station, pi->classid, tmpscope);
          }
          free_VARINFO(&varinfo);
        }

        pl += (StrLen(pl, sizeof(_ASCII)) + 1);
        pi += 1;
        ps += 1;
      }
    }
  }

  return(retcode);
}

_BOOL _VarList::AddNewVariable(_ASCII *name, _VARINFO *pi, _UWORD scope)
{
  _BOOL      retcode = _FALSE;
  _UDWORD    size;
  _VARINFO   *pdi;
  _VARSYMBOL *pds;

  size = (Data.MemVarInfo.no + 1) * sizeof(_VARINFO);
  if(Memory2((void**)&Data.MemVarInfo.ptr, size) == _TRUE)
  {
    pdi = &Data.MemVarInfo.ptr[Data.MemVarInfo.no];
    init_VARINFO(pdi);
    size = (Data.MemVarSymbol.no + 1) * sizeof(_VARSYMBOL);
    if(Memory2((void**)&Data.MemVarSymbol.ptr, size) == _TRUE)
    {
      pds = &Data.MemVarSymbol.ptr[Data.MemVarSymbol.no];
      init_VARSYMBOL(pds);
      if(Data.Label.Add(name) == _TRUE)
      {
        retcode               = _TRUE;
        Data.no              += 1;
        Data.MemVarSymbol.no += 1;
        Data.MemVarInfo.no   += 1;
        copy_VARINFO(&retcode, pdi, pi);
        pds->scope            = scope;
        pds->index            = 1;
      }
    }
  }

  return(retcode);
}

_BOOL _VarList::CreateObjectDataForLse(_UWORD station, _ASCII *cl, _ASCII *obj)
{
  _BOOL    retcode = _FALSE;
  _UWORD   id;
  _UDWORD  scope;
  _VARINFO varinfo;

  if(LookIfNameAlreadyExists(station, DEFSCOPE, obj) == DEFSCOPE)
  {
    if(FindClassId(&id, cl) == _TRUE)
    {
      retcode = _TRUE;
      init_VARINFO(&varinfo);
      if(copy_VARINFO(&retcode, &varinfo, &Types.MemVarInfo.ptr[id]) == _TRUE)
      {
        varinfo.classid = id;
        varinfo.station = station;
        scope           = Data.MemVarInfo.no;
        if(AddNewVariable(obj, &varinfo, DEFSCOPE) == _TRUE)
        {
          retcode = AddAllServer(station, id, scope);
        }
      }
      free_VARINFO(&varinfo);
    }
  }

  return(retcode);
}

_UDWORD _VarList::LookIfNameAlreadyExists(_UWORD station, _UDWORD scope, _ASCII *name)
{
  _ASCII      *label;
  _LSEVARINFO *pvi;
  _UDWORD     i;
  _VARSYMBOL  *pvs;

  label  = Data.Label.GetPtr();
  pvi    = Data.MemLseVarInfo.ptr;
  pvs    = Data.MemVarSymbol.ptr;

  for(i=0; i<Data.MemLseVarInfo.no; i++)
  {
    if((pvi->station == station)&&(pvs->scope == scope))
    {
      if(name[0] == label[0])
      {
        if(StrXmp(label, name, sizeof(_ASCII)) == 0)
        {
          return(i);
        }
      }
    }

    label += (StrLen(label, sizeof(_ASCII)) + 1);
    pvi   += 1;
    pvs   += 1;
  }

  return(DEFSCOPE);
}

_BOOL _VarList::FindClassId(_UWORD *p, _ASCII *src)
{
	return(Types.Label.Seek(p, src));
}

_BOOL _VarList::VisualizeObject(_UWORD station, _ASCII *cl, _ASCII *obj)
{
	_BOOL       retcode = _FALSE;
	_LSEVARINFO *pi;
	_UDWORD     i, start;
  _BOOL       calarm = _FALSE;
  _ASCII      tmp[200];
  _FVOID_VOID ptr;

 #ifndef UC_CASE_SENSITIVE
  _ASCII tmpobj[40];
  StrCpy(tmpobj, sizeof(_ASCII), obj, sizeof(_ASCII));
  StrUp(tmpobj, sizeof(_ASCII), StrLen(tmpobj, sizeof(_ASCII)));
  obj = tmpobj;
 #endif

  start = Data.no;
  if(CreateObjectDataForLse(station, cl, obj) == _TRUE)
  {
    retcode = _TRUE;

    for(i=start; i<Data.no; i++)
    {
      pi = &Data.MemLseVarInfo.ptr[i];
      if(GetLabel(tmp, &pi->varobj, i) == _TRUE)
      {
        if(GetSingleID(pi, tmp) == _TRUE)
        {
          // chk if we have to create a new alarm
          if(pi->statistic.parameter & PARAMETER_ALARMBIT)
          {
            if(Alarm.Add(i) == _TRUE)                  // add alarm
              calarm = _TRUE;
          }
        }
      }
    }

    if(calarm == _TRUE)
    {
      ptr = (_FVOID_VOID)find_lasal_method(LseThisPointer, METHOD_LSE_NEWALARM);
      if(ptr != NULL)
      {
        load_this(LseThisPointer);
        ptr();
      }
		}
	}

	return(retcode);
}

_BOOL _VarList::DeleteObject(_UWORD station, _ASCII *obj)
{
	_BOOL       retcode = _FALSE;
	_COMDEF     *comdef;
	_LSEVARINFO vi;
	_UBYTE      tmp[4];
	_UWORD      *result = NULL;

	comdef = login_PLC(station);
	if(comdef != NULL)
	{
		init_LSEVARINFO_Makro(&vi);
		vi.vartype = OBJECT;
		vi.classid = 0;
		vi.station = (_UBYTE)station;
		vi.varobj = 1;
		if(GetSingleID(&vi, obj) == _TRUE)
		{
			MemCpy(&tmp[0], &vi.lasalid.id, sizeof(vi.lasalid.id));

			if(TXCOMMAND(I_DEL, sizeof(tmp) + 3, tmp, comdef, (void**)&result) == DONE)
			{
				if(result != NULL)
				{
					if(*result == LC_READY)
					{
            DeleteVariable(LookIfNameAlreadyExists(station, DEFSCOPE, obj), 0);
						retcode = _TRUE;
					}
				}
			}
		}
    free_LSEVARINFO(&vi);
	}

	return(retcode);
}

_BOOL _VarList::DeleteVariable(_UDWORD varno, _UDWORD firsttime)
{
  _VARSYMBOL *pvs;
  _VARINFO   *pvi;
  _UDWORD    i;
  _ASCII     *label;
  _BOOL      retcode = _FALSE;

  if(varno < Data.MemVarInfo.no)
  {
    retcode = _TRUE;
    pvi     = Data.MemVarInfo.ptr;
    pvs     = Data.MemVarSymbol.ptr;

    for(i=0; i<Data.MemVarInfo.no; i++)
    {
      if(i == varno)
      {
        init_LASALID(&pvi->lasalid);
        if(firsttime == 0)
        {
          label  = Data.Label.GetText(i);
          *label = '?';
        }
      }

      if(pvs->scope == varno)
      {
        if((pvi->vartype == COMPLEX)||(pvi->vartype == OBJECT))
          DeleteVariable(i, firsttime+1);
        else
          init_LASALID(&pvi->lasalid);
      }

      pvi++;
      pvs++;
    }
  }

  return(retcode);
}

_UDWORD _VarList::CreateObject(_UWORD station, _ASCII *cl, _ASCII *obj)
{
	_UDWORD     retcode = 0;
	_ASCII      tmp[300];
	_UWORD      tmpsize;
	_UBYTE      *result = NULL;
	_COMDEF     *comdef;

	comdef = login_PLC(station);
	if(comdef != NULL)
	{
    StrCpy(tmp, sizeof(_ASCII), cl, sizeof(_ASCII));
    tmpsize = StrLen(tmp, sizeof(_ASCII)) + 1;
    StrCpy(&tmp[tmpsize], sizeof(_ASCII), obj, sizeof(_ASCII));
    tmpsize += StrLen(obj, sizeof(_ASCII)) + 1;

    if(TXCOMMAND(I_NEW_OBJ, tmpsize, tmp, comdef, (void**)&result) == DONE)
    {
      // [][] ........ length
      // [][][][] .... lasalid of object
      if(result != NULL)
        retcode = *(_UDWORD*)&result[2];
		}
	}

	return(retcode);
}

_BOOL _VarList::ConnectObject(_UWORD station, _ASCII *client, _ASCII *server)
{
	_BOOL   retcode = _FALSE;
	_COMDEF *comdef;
	_ASCII  tmp[300];
	_UWORD  tmpsize;
	_UWORD  *result = NULL;


	comdef = login_PLC(station);
	if(comdef != NULL)
	{
		StrCpy(tmp, sizeof(_ASCII), client, sizeof(_ASCII));
		MemCpy(&tmp[StrLen(tmp, sizeof(_ASCII)) + 1], server, StrLen(server, sizeof(_ASCII)) + 1);
		tmpsize = StrLen(client, sizeof(_ASCII)) + 1 + StrLen(server, sizeof(_ASCII)) + 1 + 3;
		if(TXCOMMAND(I_CONNECT, tmpsize, tmp, comdef, (void**)&result) == DONE);
		{
			if(result != NULL)
			{
				if(*result == LC_READY)
				{
					retcode = _TRUE;
				}
			}
		}
	}


	return(retcode);
}

_BOOL _VarList::StartObject(_UWORD station, _ASCII *obj, _UDWORD time, _UWORD mode)
{
	_BOOL       retcode = _FALSE;
	_COMDEF     *comdef;
	_LSEVARINFO vi;
	_UBYTE      tmp[10];
	_UWORD      *result = NULL;

	comdef = login_PLC(station);
	if(comdef != NULL)
	{
		init_LSEVARINFO_Makro(&vi);
		vi.vartype = OBJECT;
		vi.classid = 0;
		vi.station = (_UBYTE)station;
		vi.varobj  = 1;
		if(GetSingleID(&vi, obj) == _TRUE)
		{
			MemCpy(&tmp[0], &vi.lasalid.id, sizeof(vi.lasalid.id));
			MemCpy(&tmp[4], &time, sizeof(time));
			MemCpy(&tmp[8], &mode, sizeof(mode));

			if(TXCOMMAND(I_SET_CYCLE, sizeof(tmp) + 3, tmp, comdef, (void**)&result) == DONE)
			{
				if(result != NULL)
				{
					if(*result == LC_READY)
					{
						retcode = _TRUE;
					}
				}
			}
		}
    free_LSEVARINFO(&vi);
	}

	return(retcode);
}

extern "C" _BOOL VarList_VisualizeObject(_UWORD station, _ASCII *cl, _ASCII *obj)
{
	return(_FALSE); //VarList.VisualizeObject(station, cl, obj));
}

extern "C" _UDWORD VarList_CreateObject(_UWORD station, _ASCII *cl, _ASCII *obj)
{
	return(_FALSE); //VarList.CreateObject(station, cl, obj));
}

extern "C" _BOOL VarList_ConnectObject(_UWORD station, _ASCII *client, _ASCII *server)
{
	return(_FALSE); //VarList.ConnectObject(station, client, server));
}

extern "C" _BOOL VarList_StartObject(_UWORD station, _ASCII *obj, _UDWORD time, _UWORD mode)
{
	return(_FALSE); //VarList.StartObject(station, obj, time, mode));
}

extern "C" _BOOL VarList_DeleteObject(_UWORD station, _ASCII *obj)
{
	return(_FALSE); //VarList.DeleteObject(station, obj));
}
*/


/* ************************************************************************* */
/* ***************************** lasal interface *************************** */
/* ************************************************************************* */

extern "C" _BOOL chk_server_XCHKBIT(_UDWORD varno)
{
  _BOOL       retcode = _FALSE;
  _VARBLOCK   *pdata;
  _VARSYMBOL  *pvs;
  _LSEVARINFO *pvi;

  pdata = VarList.GetPointerToData();
  if(varno < pdata->MemVarSymbol.no)
  {
    pvi = &pdata->MemLseVarInfo.ptr[varno];
    if(chk_LASALID(&pvi->lasalid))
    {
      retcode = chk_XXCHKBIT(pvi->xxchkbit);
      if(retcode == _TRUE)
      {
        pvs = &pdata->MemVarSymbol.ptr[varno];
        if(pvs->scope < pdata->MemVarSymbol.no)
          retcode = chk_server_XCHKBIT(pvs->scope);
      }
    }
  }

  return(retcode);
}
extern "C" _UWORD chk_server_stscheme(_UDWORD varno)
{
  _UWORD      tmp, retcode = STATE_INVISIBLE;
  _VARBLOCK   *pdata;
  _VARSYMBOL  *pvs;
  _LSEVARINFO *pvi;
  _UDWORD     ftemp;
  _MYSCHEME   fmyscheme;
  _VARLABEL   *pvl = NULL;

  pdata = VarList.GetPointerToData();
  if(varno < pdata->MemVarSymbol.no)
  {
    pvi = &pdata->MemLseVarInfo.ptr[varno];

    if(chk_LASALID(&pvi->lasalid))
    {
      init_MYSCHEME_Makro(&fmyscheme);
      fmyscheme.schemeno = pvi->stscheme;
     #ifdef UC_LINKVL
      _LINKVL *plvl;
      _UWORD zi;
      for(zi=0,plvl=pvi->memlinkvl.ptr; zi<pvi->memlinkvl.no; zi++,plvl++)
      {
        if(plvl->info == VL_STATEOVERLOAD)
        {
          pvl = &plvl->link;
          break;
        }
      }
     #endif

      if(StateScheme.Call(&ftemp, &fmyscheme, pvl, STATE_ACTIVE) == _FALSE)
        ftemp = STATESCHEME_PRESELECTION;

      retcode = (_UWORD)ftemp;

      if(retcode < STATE_INACTIVE)
      {
        pvs = &pdata->MemVarSymbol.ptr[varno];
        if(pvs->scope < pdata->MemVarSymbol.no)
        {
          tmp = chk_server_stscheme(pvs->scope);
          if(tmp > retcode)
            retcode = tmp;
        }
      }
    }
  }

  return(retcode);
}
extern "C" _BOOL VarList_GetVarInfo(_VARINFO *p, _UDWORD varno)
{
	_LSEVARINFO *vi;
	_BOOL       retcode = _FALSE;

	if(VarList.GetVarInfo(&vi, varno) == _TRUE)
	{
		retcode = _TRUE;
    copy_LSE_VARINFO(&retcode, p, vi, _TRUE);
	}
	return(retcode);
}
extern "C" _BOOL VarList_GetSingleVariable(_VARIABLE *p, _UDWORD varno)
{
	return(VarList.GetSingleVariable(p, varno));
}
extern "C" _BOOL VarList_GetVariable(_VARIABLE *p, _VARLABEL *vl)
{
  return(VarList.GetVariable(p, vl));
}
extern "C" _BOOL VarList_GetSystemData(_RESULT *res, _VARIABLE *p, _VARLABEL *vl, _BOOL wait4plc)
{
  return(VarList.GetSystemData(res, p, vl, wait4plc, _TRUE));
}
extern "C" _UWORD VarList_GetVariableSize(_VARINFO *p)
{
  return(VarList.GetVariableSize(p->vartype, p->format));
}
extern "C" _BOOL VarList_SetSystemData(_RESULT *res, _VARIABLE *p, _VARLABEL *vl, _BOOL saveimmediate)
{
  return(VarList.SetSystemData(res, p, vl, saveimmediate));
}
extern "C" _BOOL VarList_GetValueByNo(_UDWORD varno, _IVALUE *dest, _BOOL wait4plc)
{
  return(VarList.GetValueByNo(varno, dest, wait4plc));
}
extern "C" _BOOL VarList_GetValue(_VARLABEL *pvl, _IVALUE *dest, _BOOL wait4plc)
{
  return(VarList.GetValue(pvl, dest, wait4plc));
}
extern "C" _BOOL VarList_SetValue(_VARLABEL *pvl, _IVALUE value)
{
  _RESULT   result;
  _VARIABLE variable;

  init_RESULT_Makro(&result);
  result.rmax  =
  result.rmin  =
  result.value = value;
  result.ptr   = &result.value;
  result.ftype = RES_CONSTANT;
  return(VarList.SetSystemData(&result, &variable, pvl, _TRUE));
}
extern "C" _BOOL VarList_SetValueByNo(_UDWORD varno, _IVALUE val)
{
  return(VarList.SetValueByNo(varno, val));
}
extern "C" void *VarList_GetStringByNo(void *dst, _UDWORD varno, _UDWORD chrsize)
{
  _RESULT res;
  _VARLABEL vl;
  _VARIABLE vari;
  
  init_RESULT_Makro(&res);
  init_VARLABEL_Makro(&vl);
  
  vl.no = 1;
  vl.info[0].state = VAR_VAL;
  vl.info[0].value = *(_UDWORD*)&varno;

  if(VarList.GetSystemData(&res, &vari, &vl, _TRUE, _FALSE, _FALSE) == _TRUE)
  {
    if(res.ftype == RES_ANUMERIC)
    {
      if(chrsize != sizeof(_CHAR))
      {
        void *retcode = (dst != NULL)? dst : get_temptext();
        StrCpy(retcode, chrsize, res.ptr, sizeof(_CHAR));
        return retcode;
      }
      
      if(dst != NULL)
      {
        StrCpy(dst, chrsize, res.ptr, sizeof(_CHAR));
        return dst;
      }
      
      return res.ptr;
    }
  }
  return NULL;
}
extern "C" _BOOL VarList_SetStringByNo(_UDWORD varno, void *txt, _UDWORD chrsize)
{
  _RESULT res;
  _VARLABEL vl;
  _VARIABLE vari;
  
  init_RESULT_Makro(&res);
  init_VARLABEL_Makro(&vl);
  
  vl.no = 1;
  vl.info[0].state = VAR_VAL;
  vl.info[0].value = *(_UDWORD*)&varno;
  
  res.ptr = txt;
  res.ftype = RES_ANUMERIC;
  
  chrsize = (chrsize == 1)? 1:2;
  
  if(chrsize != sizeof(_CHAR))
  {
    res.ptr = get_temptext();
    StrCpy(res.ptr, sizeof(_CHAR), txt, chrsize);
  }
  
  return VarList.SetSystemData(&res, &vari, &vl, _TRUE); 
}

extern "C" _UWORD VarList_GetCompleteNoDigits(_VARINFO *pi)
{
  return(VarList.GetCompleteNoDigits(pi));
}
extern "C" _BOOL VarList_GetLabel(_ASCII *txt, _UBYTE *varobj, _UDWORD nr)
{
  return(VarList.GetLabel(txt, varobj, nr));
}
extern "C" _UDWORD VarList_GetVarNo(_ASCII *txt)
{
  return(VarList.GetVarNo(txt, 255));
}
extern "C" _UDWORD VarList_GetVarNoStation(_ASCII *txt, _UWORD station)
{
  return(VarList.GetVarNo(txt, station));
}
extern "C" _BOOL VarList_VOWPRUIC(_UWORD info, _UWORD no)
{
  // VOWPRUIC
	switch(no)
  {
    case 6 : return(VarList.GetZero(info));
		case 3 : return(_TRUE);
    case 2 : return(VarList.GetPermanent(info));
    case 1 : return(VarList.GetInvert(info));
  }

  return(_FALSE);
}
extern "C" _BOOL VarList_GetParameter(_UDWORD parameter, _UWORD no)
{
  switch(no)
  {
    case 0: return(VarList.GetTool(parameter));
    case 1: return(VarList.GetProt(parameter));
    case 2: return(VarList.GetPde(parameter));
  }
  return(_FALSE);
}
extern "C" _CHAR* VarList_GetText(_UDWORD no)
{
  _ANYTHING anything;

  init_ANYTHING_Makro(&anything);
  anything.list = TXTLIST_VTX;
  anything.no = no;
  return(get_text(&anything, NULL, NULL, _TRUE));
}
extern "C" _UWORD VarList_GetDigits(_VARINFO *pi)
{
	return(VarList.GetDigits(pi));
}
extern "C" _BOOL  VarList_GetSign(_VARINFO *pi)
{
	return(VarList.GetSign(pi));
}
extern "C" _DWORD VarList_GetDezPoint(_VARINFO *pi)
{
	return(VarList.GetDezPoint(pi));
}
extern "C" _UDWORD VarList_GetNo(void)
{
	return(VarList.GetNo());
}
extern "C" _CHAR* VarList_GetTextByNo(_UDWORD varno, _UWORD txtno)
{
	return(VarList.GetTextByNo(varno, txtno));
}
extern "C" _UNIT* VarList_GetUnitInfo(_UWORD unitno)
{
  return(VarList.GetUnitInfo(unitno));
}
extern "C" _UWORD VarList_GetUnitNo(_ASCII *txt)
{
  return(VarList.GetUnitNo(txt));
}
extern "C" _BOOL VarList_SetValueSub(_RESULT *res, _VARIABLE *p)
{
  return(VarList.SetValueSub(res, p));
}
extern "C" _UDWORD VarList_GetVarNoByID(_UDWORD id, _UWORD station, _BOOL justserver)
{
  return(VarList.GetVarNoByID(id, station, justserver));
}
extern "C" _BOOL VarList_GetVarIdChanged(void)
{
  return(VarList.GetVarIdChanged());
}
extern "C" _BOOL VarList_GetVarSymbol(_VARSYMBOL **p, _UDWORD no)
{
  return(VarList.GetVarSymbol(p, no));
}
extern "C" _UDWORD VarList_GetVarNoByCRC(_CRC32 crc32)
{
  return(VarList.GetVarNoByCRC(crc32));
}
extern "C" _LSEVARINFO *VarList_Get_LSEVARINFO(_UDWORD no)
{
  _LSEVARINFO *retcode;

  if(VarList.GetVarInfo(&retcode, no) == _FALSE)
    retcode = NULL;

  return(retcode);
}
#ifdef OLDSTYLE_CAN
extern "C" _BOOL VarList_ChangeBaseAddress(_UDWORD varno, _UDWORD address)
{
  return(VarList.ChangeBaseAddress(varno, address));
}
#endif

extern "C" _UDWORD VarList_GetUnitIndex(_UDWORD varno) // sa36634
{
  return VarList.GetUnitIndex(varno);
}
extern "C" _ASCII* VarList_GetUnitLabel(_UDWORD unitindex) // sa36634
{
  return VarList.GetUnitLabel(unitindex);
}
extern "C" _BOOL VarList_CheckHostname(_BOOL do_scanbar, _BOOL stop_at_double_name, _BOOL reload_if_damaged)
{
  return(VarList.CheckHostname(do_scanbar, stop_at_double_name, reload_if_damaged));
}
extern "C" _ASCII *VarList_GetHostName(_ASCII *label, _UWORD station)
{
  return(VarList.GetHostName(label, station));
}
extern "C" _ASCII *VarList_GetHostNameByVarNo(_UDWORD varno)
{
  return(VarList.GetHostNameByVarNo(varno));
}
extern "C" _ASCII *VarList_GetHostNameByCrc(_CRC32 crc32)
{
  return(VarList.GetHostNameByCrc(crc32));
}
extern "C" _UDWORD VarList_GetVarNoByHostName(_ASCII *hostname)
{
  return(VarList.GetVarNoByHostName(hostname));
}
extern "C" _BOOL VarList_GetOnlyAscii(_VARINFO *pvi)
{
  return(VarList.GetOnlyAscii(pvi));
}
extern "C" _UDWORD VarList_FindRoot(_UDWORD varno)
{
  return(find_root(&VarList.GetPointerToData()->MemVarSymbol, varno));
}
extern "C" _BOOL VarList_MakeLasalId(_LSEVARINFO *pva, _ASCII *label)
{
  return get_lasal_id(&pva->lasalid, label, pva);
}
extern "C" _UDWORD VarList_GetServerToEdit(void) // sa32402
{
  return ServerToEdit;
}

#ifdef _FIXED_STATESCHEME
_BOOL _VarList::PrepaireStateschemeSub(_UDWORD i)
{
  _BOOL       retcode = _TRUE;
  _LSEVARINFO *pvi;
  _VARSYMBOL  *pvs;

  if(i < Data.no)
  {
    retcode = _FALSE;
    if(GetVarInfo(&pvi, i) == _TRUE)
    {
//      if(pvi->xchkbit.seqno == DEFSEQNO)
      if((pvi->xxchkbit & 0x7FFF) == 0x7FFF)
      {
        if((pvi->stscheme == DEFAULTSCHEME)||(pvi->stscheme == NOSTSCHEME))
        {
          retcode = _TRUE;
          if(GetVarSymbol(&pvs, i) == _TRUE)
          {
            retcode = PrepaireStateschemeSub(pvs->scope);
          }
        }
      }
    }
  }

  return(retcode);
}

void _VarList::PrepaireStatescheme(void)
{
  _UDWORD     i;
  _LSEVARINFO *pvi;

  for(i=0; i<Data.no; i++)
  {
    if(PrepaireStateschemeSub(i) == _TRUE)
    {
      if(GetVarInfo(&pvi, i) == _TRUE)
        pvi->stscheme = NOSTSCHEME;
    }
  }
}
#endif

#ifdef UC_IPC

_UDWORD memsize_MEM_VARSYMBOL(_MEM_VARSYMBOL *p)
{
  return(p->no * sizeof(_VARSYMBOL));
}

_UDWORD memsize_MEM_LSEVARINFO(_MEM_LSEVARINFO *p)
{
  _UDWORD     i;
  _UDWORD     retcode = (sizeof(_LSEVARINFO) * p->no);
  _LSEVARINFO *pvi;

  for(i=0, pvi=p->ptr; i<p->no; i++, pvi++)
  {
    retcode += memorysize(pvi->xmemtext.xxno, sizeof(_ANYPARA));
   #ifdef UC_LINKVL
    retcode += memorysize(pvi->memlinkvl.no, sizeof(_LINKVL));
   #endif
  }

  return(retcode);
}

_UDWORD memsize_VARBLOCK(_VARBLOCK *p)
{
  return(memsize_MEM_VARSYMBOL(&p->MemVarSymbol)
         + memsize_MEM_LSEVARINFO(&p->MemLseVarInfo)
         + p->Label.MemSize());
}

_UDWORD _VarList::MemSize(_MEMSIZE *p)
{
  p->data = memsize_MEM_VARSYMBOL(&Data.MemVarSymbol)
          + memsize_MEM_LSEVARINFO(&Data.MemLseVarInfo);

  p->label = Data.Label.MemSize();

  return(p->data+p->label);
}
#endif

void transform_RESULT(_RESULT *pres, _RESULT_TYPE restyp)
{
  if(pres->ftype != restyp)
  {
    if((restyp == RES_NUMERIC) || (restyp == RES_CONSTANT))
    {
      if(pres->ftype == RES_REAL)
      {
        _REAL r = get_pREAL((_REAL*)&pres->value);
        
        if(r < 0)
          r = r - 0.5;
        else
          r = r + 0.5;
        
        pres->value = FTOL(r);
      }
      pres->ftype = restyp;
    }
    else if(restyp == RES_REAL)
    {
      if((pres->ftype == RES_NUMERIC) || (pres->ftype == RES_CONSTANT))
      {
        
        move_pREAL_DWORD((_REAL*)&pres->value, pres->value);
        pres->ftype = RES_REAL;
      }
    }
    
    // to be continued
    
    
  }
}


/*
void update_no(_UDWORD *pi, _UDWORD no)
{
  _UDWORD i;

  for(i=0; i<no; i++, pi++)
  {
    if(*pi != 0xFFFFFFFF)
      *pi -= 1;
  }
}

_UDWORD shrink(_LSEVARINFO *pvl, _UDWORD *pn, _UDWORD ono)
{
  _UDWORD     i, anz, no, rest;
  _LSEVARINFO *ph;

  anz = ono;
  no  = ono;

  while(no)
  {
    no -= 1;

    if(pn[no] == 0xFFFFFFFF)
    {
      rest = (anz - 1 - no);
      ph = &pvl[no];
      free_LSEVARINFO(ph);

      if(rest > 0)
      {
        MemMove(ph, ph+1, rest*sizeof(_LSEVARINFO));
        init_LSEVARINFO_Makro(ph+1);
      }

      anz -= 1;
    }
  }

  for(i=0, no=ono; i<ono; i++, no--, pn++)
  {
    if(*pn == 0xFFFFFFFF)
      update_no(pn, no);
  }

  return(anz);
}
*/
















