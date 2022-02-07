#include "T_IPC.H"
#include "T_NUC.H"

//#define OLDTOOL

_BOOL IsFilter(_STATISTIC *p, _STATISTIC *filter)
{
  _BOOL retcode = _FALSE;

  if((filter->parameter == 0)||(filter->parameter & p->parameter)) // toolcat bit
  {
    if((filter->group == 0)||(filter->group & p->group))
    {
      retcode = _TRUE;
    }
  }

  return(retcode);
}

_ASCII *get_ascii(_ASCII *dst, _ASCII *src)
{
  _ASCII ascii;

  ascii = *src;
  while((ascii != ',')&&(ascii != 0))
  {
    *dst  = ascii;
    dst  += 1;
    src  += 1;
    ascii = *src;
  }

  *dst = 0;

  return(src);
}

_ASCII *get_asciidata(_CHAR *dst, _ASCII *src)
{
  _ASCII ascii;

  ascii = *src;
  while((ascii != ',')&&(ascii != 0))
  {
    *dst  = ascii;
    dst  += 1;
    src  += 1;
    ascii = *src;
  }

  *dst = 0;

  return(src);
}

_ASCII *get_chrdata(_CHAR *dst, _ASCII *src)
{
  _UWORD size = 1;
  _ASCII ascii;
  _UNI   uni;

  if((src[0] == '[')&&(src[2] == ']'))
  {
    size = src[1] - '0';
    src += 3;
  }

  if(size == 1)
  {
    if(src[0] == '"')
    {
      src += 1;

      ascii = *src;
      while((ascii != '"')&&(ascii != 0))
      {
        *dst  = ascii;
        dst  += 1;
        src  += 1;
        ascii = *src;
      }
    }
  }
  else
  {
    uni = *(_ASCII*)src;
    if(uni == '"')
    {
      src += 2;
      uni = *(_UNI*)src;
      if((uni & 0xFF00) == 0x2000)
        uni = uni & 0xFF;

      while((uni != '"')&&(uni != 0))
      {
        *dst = (_CHAR)uni;
        dst += 1;
        src += 2;
        uni  = *(_UNI*)src;
        if((uni & 0xFF00) == 0x2000)
          uni = uni & 0xFF;
        if(uni == 0x2C22)
        {
          // 0x2c22 ist ein gänsefüschen + komma, weil alle space herausgelöscht
          uni = '"';
          src -= 1;
        }
      }
    }
  }

  *dst = 0;

  return(src);
}

_ASCII* scanto(_ASCII *line, _UWORD chrsize)
{
  _UWORD run = 1;

  if(chrsize == 1)
  {
    while(run)
    {
      if((line[0] == '"')||(line[0] == 0))
        run = 0;
      else
        line += 1;
    }
  }
  else
  {
    while(run)
    {
      if((*(_UNI*)&line[0]) == '"')
        run = 0;
      else
        line += 2;
    }

    line += 1;
  }

  return(line);
}

_UWORD get_station(_ASCII *line)
{
  // value, unit, {group}, {parameter}, station,
  _UWORD retcode = 255;
  _UWORD run = 1;
  _UWORD count = 0;

  do
  {
    if(line[0] == ',')
    {
      count += 1;
      if(count == 5)
        run = 0;
    }
    else if((line[0] == '[') && (line[2] == ']') && (line[3] == '"'))
    {
      if(line[1] == '1')
      {
        line = scanto(&line[4], 1);
      }
      else if((line[1] == '2') && (line[4] == 0))
      {
        line = scanto(&line[5], 2);
      }
    }
    else if(line[0] == '"')
    {
      line = scanto(&line[1], 1);
    }
    else if(line[0] == 0)
    {
      run = 0;
    }

    line++;
  }
  while(run);

  if(count == 5)
    retcode = AToD(line, sizeof(_ASCII));

  return(retcode);
}

void clean_line(_ASCII *line, _UWORD size)
{
  _UWORD run = 1;

  // alle SPACE bis Streamende oder '"' löschen
  // bei UNICODE und ASCIICODE werden alle SPACE bis zum " gelöscht.
  do
  {
    size -= 1;
    if((line[0] == 0)||(line[0] == '"'))
    {
      run = 0;
    }
    else if(line[0] == ' ')
    {
      MemMove(line, &line[1], size);
    }
    else
    {
      line += 1;
    }
  }
  while(run);
}

_TOOLERROR scan_line(_BOOL *pskip, _LSEVARINFO **ppvi, _VARLABEL *vl, _RESULT *result, _ASCII *line, _TOOLDATA *ptd) 
{
  _ASCII      chr;
  _UDWORD     varno;
  _UDWORD     station;
  _UWORD      counter;
  _LSEVARINFO *vi;
  _CHAR       *tmp;
  _TOOLERROR  retcode = TE_UNKNOWN_LINE_FORMAT;
  _UWORD      run;

  chr     = ',';
  counter = 0;
  run     = 1;
  tmp     = get_temptext();
  *pskip  = _FALSE;

  // version 0: label, value, unit, {group},
  // version 1: label, value, unit, {group}, {parameter}, station,

  do
  {
    if(chr == ',')
    {
      switch(counter)
      {
        case 0 : // label
          line  = get_ascii((_ASCII*)tmp, line);
          station = get_station(line);
          
          // SA37331:begin **************************************************************************
          if(ptd->ClassRevision >= 0x00010000) // 1.00
          {
            _FBOOL_PT_PT_UDWORD ptrL = (_FBOOL_PT_PT_UDWORD)find_lasal_method(ptd->Pthis, TOOLCAT_LOAD_LABEL);
            if(ptrL != NULL)
            {
              void *pt = ptd->Pthis;
              load_this(pt);
              _BOOL st = ptrL(ptd, tmp, station);
              if(st == _FALSE)
              {
                return TE_NOERROR; // server nicht laden, keinen fehler erzeugen
                *pskip = _TRUE;
              }
            }
          }
          // SA37331:end ****************************************************************************
          
          // SA42046: begin *************************************************************************
          if(ptd->ClassRevision >= 0x00010001) // 1.01
          {
            _FBOOL_PT_PT_PT ptrL = (_FBOOL_PT_PT_PT)find_lasal_method(ptd->Pthis, TOOLCAT_LOAD_LABEL_EX);
            if(ptrL != NULL)
            {
              void *pt = ptd->Pthis;
              load_this(pt);
              _BOOL st = ptrL(ptd, tmp, &station);
              if(st == _FALSE)
              {
                return TE_NOERROR; // server nicht laden, keinen fehler erzeugen
                *pskip = _TRUE;
              }
            }
          }
          // SA42046:end ****************************************************************************
          
          varno = VarList.GetVarNo((_ASCII*)tmp, station);
          if(varno >= VarList.GetNo())
          {
            retcode = TE_UNKNOWN_SERVER;
            run = 0;
          }
          break;
        case 1 : // data
          retcode = TE_NOERROR;
          if(VarList.GetVarInfo(&vi, varno) == _TRUE)
          {
            if(ppvi != NULL)
              *ppvi = vi;

            if((IsFilter(&vi->statistic, &ptd->Filter)) == _TRUE)
            {
              vl->no = 1;
              vl->info[0].state = VAR_VAL;
              vl->info[0].value = varno;
              switch(vi->vartype)
              {
                case FENUM   :
                case BINARY  : line = get_ascii((_ASCII*)tmp, line);
                              #ifdef UC_REAL
                               if(vi->format == SIZE_REAL)
                               {
                                 if(StrToReal((_REAL*)&result->value, (_ASCII*)tmp, sizeof(_ASCII)) == _TRUE)
                                 {
                                   result->ftype = RES_REAL;
                                   result->ptr   = (void*)&result->value;
                                   retcode       = TE_NOERROR;
                                 }
                               }
                               else
                              #endif
                               {
                                 if(StrToNum(&result->value, (_ASCII*)tmp, sizeof(_ASCII)) == _TRUE)
                                 {
                                   result->ftype = RES_NUMERIC;
                                   result->ptr   = (void*)&result->value;
                                   retcode       = TE_NOERROR;
                                 }
                               }
                               break;
                case BIT     : line = get_ascii((_ASCII*)tmp, line);
                               if(StrToNum(&result->value, (_ASCII*)tmp, sizeof(_ASCII)) == _TRUE)
                               {
                                 result->ftype = RES_NUMERIC;
                                 result->ptr   = (void*)&result->value;
                                 retcode       = TE_NOERROR;
                               }
                               break;
                case FDATE   : line = get_asciidata(tmp, line);
                               string_to_date(&result->datim.fdate, tmp, vi->format);
                               result->ftype = RES_DATE;
                               retcode       = TE_NOERROR;
                               break;
                case FTIME   : line = get_asciidata(tmp, line);
                               string_to_time(&result->datim.ftime, tmp, vi->format);
                               result->ftype = RES_TIME;
                               retcode       = TE_NOERROR;
                               break;
                case FSTRING : line = get_chrdata(tmp, line);
                               result->ftype = RES_ANUMERIC;
                               result->ptr   = tmp;
                               retcode       = TE_NOERROR;
                               break;
                default      : retcode = TE_UNKNOWN_SERVER;
                               break;
              }
            }
            else
            {
              retcode = TE_FILTER_DOESNT_FIT;
            }
          }
          else
          {
            retcode = TE_UNKNOWN_SERVER;
          }

          run = 0;
          break;
      }

      counter += 1;
    }
    else if(chr == 0)
    {
      run = 0;
    }

    chr   = *line;
    line += 1;
  }
  while(run);

  return(retcode);
}
/*
_BOOL ToDate(_DATE *date, _ASCII *line, _UDWORD varno)
{
  _BOOL       retcode = _FALSE;
  _LSEVARINFO *vi;
  _CHAR       tmpline[300];

  if(VarList.GetVarInfo(&vi, varno) == _TRUE)
  {
    StrCpy(tmpline, sizeof(_CHAR), line, sizeof(_ASCII));
    retcode = string_to_date(date, tmpline, vi->format);
  }

  return(retcode);
}

_BOOL ToTime(_TIME *time, _ASCII *line, _UDWORD varno)
{
  _BOOL       retcode = _FALSE;
  _LSEVARINFO *vi;
  _CHAR       tmpline[300];

  if(VarList.GetVarInfo(&vi, varno) == _TRUE)
  {
    StrCpy(tmpline, sizeof(_CHAR), line, sizeof(_ASCII));
    retcode = string_to_time(time, tmpline, vi->format);
  }

  return(retcode);
}
*/
_BOOL toolcat_singlesave(_ASCII *label, _VARIABLE *pv, _RESULT *pr, _File *File)
{
  _ASCII       *txt, comma = ',';
  _ASCII       tmp[512];  // obechr
  _CHAR        uni, tmpstr[1024]; // obechr
  _UWORD       i;
  _UDWORD      unitno;
  _MYSCHEME    myscheme;


  // label of variable
  File->Write(label, StrLen(label, sizeof(_ASCII)));
  File->Write(&comma, sizeof(_ASCII));

  // value of variable
//  init_RESULT(&res);
//  if(xget_dat(&res, pv->no, ToolData.care_state)
//  if(get_dat(&res, pv->no) == _TRUE)
  {
    switch(pr->ftype )
	  {
     #ifdef UC_REAL
      case RES_REAL:
        RealToStr(tmp, get_pREAL((_REAL*)&pr->value), VarList.GetDezPoint(&pv->info), sizeof(_ASCII));
        File->Write(tmp, StrLen(tmp, sizeof(_ASCII)));
        break;
     #endif

      case RES_NUMERIC:
        NumToStr(tmp, pr->value, sizeof(_ASCII));
        File->Write(tmp, StrLen(tmp, sizeof(_ASCII)));
        break;

      case RES_DATE:
        format_date(tmpstr, &pr->datim.fdate, pv->info.format);
        StrCpy(tmp, sizeof(_ASCII), tmpstr, sizeof(_CHAR));
        File->Write(tmp, StrLen(tmp, sizeof(_ASCII)));
        break;

      case RES_TIME:
        format_time(tmpstr, &pr->datim.ftime, pv->info.format);
        StrCpy(tmp, sizeof(_ASCII), tmpstr, sizeof(_CHAR));
        File->Write(tmp, StrLen(tmp, sizeof(_ASCII)));
        break;

      case RES_ANUMERIC:
       #ifdef UC_UNICODE
        if(VarList.GetOnlyAscii(&pv->info) == _FALSE)
       #endif
        {
          tmp[0] = '[';
          tmp[1] = '0' + sizeof(_CHAR);
          tmp[2] = ']';
          File->Write(tmp, 3);
          uni = '"';
          File->Write(&uni, sizeof(_CHAR));
          format_string_sub(tmpstr, (_CHAR*)pr->ptr, &pv->info, (sizeof(tmpstr) / sizeof(_CHAR)) - 1);
          File->Write(tmpstr, StrSize(tmpstr, sizeof(_CHAR)));
          File->Write(&uni, sizeof(_CHAR));
        }
       #ifdef UC_UNICODE
        else
        {
          tmp[0] = '[';
          tmp[1] = '0' + sizeof(_ASCII);
          tmp[2] = ']';
          File->Write(tmp, 3);
          tmp[0] = '"';
          File->Write(tmp, sizeof(_ASCII));
          format_string_sub(tmpstr, (_CHAR*)pr->ptr, &pv->info, (sizeof(tmpstr) / sizeof(_CHAR)) - 1);
          StrCpy(tmp, sizeof(_ASCII), tmpstr, sizeof(_CHAR));
          File->Write(tmp, StrSize(tmp, sizeof(_ASCII)));
          tmp[0] = '"';
          File->Write(&tmp, sizeof(_ASCII));
        }
       #endif
        break;

      default :
        break;
    }
  }
  File->Write(&comma, sizeof(_ASCII));

  // actual unit of variable
  init_MYSCHEME(&myscheme);
  myscheme.schemeno = pv->info.funitscheme;
  UnitScheme.Call(&unitno, &myscheme, NULL, pv->info.funit);
  txt = VarList.GetLabelUnit()->GetText((_UWORD)unitno);
  if(txt != NULL)
  {
    StrCpy(tmpstr, sizeof(_ASCII), txt, sizeof(_ASCII));
    File->Write(tmpstr, StrLen(tmpstr, sizeof(_ASCII)));
  }
  File->Write(&comma, sizeof(_ASCII));
  txt = (_ASCII*)tmpstr;

 #ifdef OLDTOOL
  _AsciiPuffer *lg;
  _UWORD       ft;
  // grouplabel
  if(pv->info.statistic.group != 0)
  {
    lg = VarList.GetLabelGroup();
    ft = 0;

    for(i=0; i<32; i++)
    {
      if(pv->info.statistic.group & (1 << i))
      {
        txt = lg->GetText(i);
        if(ft != 0)
          File->Write("|", sizeof(_ASCII));  // pipe

        ft = 1;
        File->Write(txt, StrLen(txt, sizeof(_ASCII)));
      }
    }
  }
  txt[0]   = 32; //' '
  File->Write(&txt[0], sizeof(_ASCII));
 #endif

  // group of variable
  txt[0]   = 123; //'{'
  DToA(&txt[1], pv->info.statistic.group, 0x0800, sizeof(_ASCII));
  i = StrLen(txt, sizeof(_ASCII));
  txt[i]   = 125; // '}'
  txt[i+1] = 0;
  File->Write(txt, StrLen(txt, sizeof(_ASCII)));
  File->Write(&comma, sizeof(_ASCII));

  // parameter of variable
  txt[0]   = 123; //'{'                                                // new
  DToA(&txt[1], pv->info.statistic.parameter, 0x0800, sizeof(_ASCII)); // new
  i = StrLen(txt, sizeof(_ASCII));                                     // new
  txt[i]   = 125; // '}'                                               // new
  txt[i+1] = 0;                                                        // new
  File->Write(txt, StrLen(txt, sizeof(_ASCII)));                       // new
  File->Write(&comma, sizeof(_ASCII));                                 // new

  // stationnumber of variable                                         // new
  DToA(tmp, pv->info.station, 0x0800, sizeof(_ASCII));                 // new
  File->Write(tmp, StrLen(tmp, sizeof(_ASCII)));                       // new
  File->Write(&comma, sizeof(_ASCII));                                 // new

  comma = 13;
  File->Write(&comma, sizeof(_ASCII));
  comma = 10;
  File->Write(&comma, sizeof(_ASCII));

  return((File->GetError() == _TRUE)? _FALSE : _TRUE);
}

// ***********************************************************************
// ***********************************************************************
// TOOLCATALOGUE
// ***********************************************************************
// ***********************************************************************

void _ToolCatalogue::Init(void)
{
  init_TOOLDATA(&ToolData);
}

void _ToolCatalogue::Free(void)
{
  free_TOOLDATA(&ToolData);
}

_BOOL _ToolCatalogue::SetError(_TOOLERROR te)
{

  return(SetError(te, NULL, NULL, NULL));
}

_BOOL _ToolCatalogue::SetError(_TOOLERROR te, _RESULT *pres, _VARIABLE *pvar, _VARLABEL *pvl)
{
  _FBOOL_PT_PT_PT_PT ptr;
  _BOOL              retcode = _FALSE;

  if(ToolData.ToolError == TE_NOERROR)
  {
    ToolData.ToolError = te;
    ptr = (_FBOOL_PT_PT_PT_PT)find_lasal_method(ToolData.Pthis, TOOLCAT_ERROR_OCCURED);
    if(ptr != NULL)
    {
      void *pt = ToolData.Pthis;
      load_this(pt);
      retcode = ptr(&ToolData, pres, pvar, pvl);
    }
    else
    {
      switch(ToolData.ToolError)
      {
        case TE_CANTOPEN      :
        case TE_NO_TOOLCAT    :
        case TE_DISK_ERROR    :
        case TE_TOOLESSMEMORY :
        case TE_INVALID_CRC   : 
          retcode = _FALSE;
          break;
        default : 
          retcode = _TRUE;
      }
    }
  }

  if(retcode == _TRUE)
  {
	 ToolData.ToolError = TE_NOERROR;
  }
 #ifndef USER_KM
 #ifndef USER_DEMAG
  else
  {
	 ToolData.File.Close();
	 Free();
  }
 #endif
 #endif

  return(retcode);

 // _TRUE  system should go on (next entry)
 // _FALSE stay in error state

}

_BOOL _ToolCatalogue::AlreadyInserted(_UDWORD varno)
{
  _UDWORD bytoff, bitoff;
  _UBYTE byte, bit;
  _BOOL  retcode = _FALSE;

  if(varno < ToolData.MemIndex.no)
  {
    bytoff  = varno / 8;
    bitoff  = varno % 8;
    byte    = ToolData.MemIndex.bit[bytoff];
    bit     = (1 << bitoff);
    retcode = (byte & bit)? _FALSE : _TRUE;
    ToolData.MemIndex.bit[bytoff] |= bit;
  }

  return(retcode);
}

_BOOL _ToolCatalogue::AddEntry(_UDWORD varno)
{
  _VARIABLE var;
  _BOOL     retcode = _TRUE;

  if(AlreadyInserted(varno) == _TRUE)
  {
    if(VarList.GetSingleVariable(&var, varno) == _TRUE)
    {
      if(IsFilter(&var.info.statistic, &ToolData.Filter) == _TRUE)
      {
        if((var.info.vartype != OBJECT)&&(var.info.vartype != COMPLEX))
        {
         #ifndef USER_BILLION
//          if(valid_VARLABEL(&var.info.lost_limit) == _TRUE)
//            AddEntry(var.info.lost_limit.info[0].value);
//          if(valid_VARLABEL(&var.info.hist_limit) == _TRUE)
//            AddEntry(var.info.hist_limit.info[0].value);
          if(valid_VARLABEL(&var.info.lo_limit) == _TRUE)
            AddEntry(var.info.lo_limit.info[0].value);
          if(valid_VARLABEL(&var.info.hi_limit) == _TRUE)
            AddEntry(var.info.hi_limit.info[0].value);
         #endif

          retcode = _FALSE;
          if(ToolData.MemIndex.used < ToolData.MemIndex.no)
          {
            retcode = _TRUE;
            ToolData.MemIndex.ptr[ToolData.MemIndex.used] = varno;
            ToolData.MemIndex.used++;
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL _ToolCatalogue::MakeMemIndex(void)
{
  _UDWORD i, no, size;
  _BOOL   retcode = _FALSE;

  no   = VarList.GetNo();
  size = no * sizeof(_UDWORD);

  if(Memory2((void**)&ToolData.MemIndex.ptr, size) != 0)
  {
    MemSet(ToolData.MemIndex.ptr, 0xFF, size);
    size = (no+8) / 8;
    if(Memory2((void**)&ToolData.MemIndex.bit, size) != 0)
    {
      retcode = _TRUE;
      MemSet(ToolData.MemIndex.bit, 0, size);
      ToolData.MemIndex.no   = no;
      ToolData.MemIndex.used = 0;

      for(i=0; i<no; i++)
        AddEntry(i);
    }
  }
  return(retcode);
}

_BOOL _ToolCatalogue::SaveServer(_UDWORD varno)
{
  _BOOL           retcode = _FALSE;
  _ASCII          label[MAX_LABELLEN];
  _UBYTE          varobj = 0;
  _VARIABLE       variable;
  _FBOOL_PT_PT_PT ptr;
  _RESULT         result;


  if(ToolData.File.GetError() == _FALSE)
  {
    if(VarList.GetSingleVariable(&variable, varno) == _TRUE)
    {
      if(VarList.GetLabel(label, &varobj, varno) == _TRUE)
      {
        retcode = _TRUE;
        
        // SA37331:begin **************************************************************************
        if(ToolData.ClassRevision >= 0x00010000) // 1.00
        {
          _FBOOL_PT_PT_UDWORD ptrL = (_FBOOL_PT_PT_UDWORD)find_lasal_method(ToolData.Pthis, TOOLCAT_SAVE_LABEL);
          if(ptrL != NULL)
          {
            void *pt = ToolData.Pthis;
            load_this(pt);
            retcode = ptrL(&ToolData, label, variable.info.station);
            if(retcode == _FALSE)
            {
              return _TRUE; // server nicht in file speichern, keinen fehler erzeugen
            }
          }
        }
        // SA37331:END ****************************************************************************
      
        ptr = (_FBOOL_PT_PT_PT)find_lasal_method(ToolData.Pthis, TOOLCAT_SAVE_RUN);
        if(ptr != NULL)
        {
          void *pt = ToolData.Pthis;
          load_this(pt);
          retcode = ptr(&ToolData, label, &variable);
        }

        if(retcode == _TRUE)
        {
          retcode = _FALSE;
          if(xget_dat(&result, variable.no, ToolData.care_state) == ERROR_CAUSED_BY_NO_ERROR)
            retcode = toolcat_singlesave(label, &variable, &result, &ToolData.File);
        }

        if(retcode == _TRUE)
          retcode = (ToolData.File.GetError() == _FALSE)? _TRUE : _FALSE;
        
      }
    }
  }

  return(retcode);
}

_BOOL _ToolCatalogue::WriteCrc(_UDWORD crc)
{
  _ASCII txt[20];

  if(ToolData.File.GetError() == _FALSE)
  {
    DToH(txt, crc, 0x108, sizeof(_ASCII));
    ToolData.File.Write("[CRC: 16#", 9);
    ToolData.File.Write(txt, 8);
    ToolData.File.Write("]", 1);
    txt[0] = 13;
    txt[1] = 10;
    ToolData.File.Write(txt, 2);
  }

  return((ToolData.File.GetError() == _TRUE)? _FALSE : _TRUE);
}

_BOOL _ToolCatalogue::SaveStart(_ASCII *dpne)
{
  _BOOL     retcode;
  _ASCII    txt[2];
  _FBOOL_PT ptr;
  _ASCII    xdpne[_MAX_PATHLENGTH];

  ToolData.ClassRevision = GetRevisionOfClass((char*)"_ToolCat");

  if(ToolData.File.Open(SAVE, MakeRamFile(xdpne, dpne)) == _TRUE)
  {
    if(MakeMemIndex() == _TRUE)
    {
      ToolData.ActNo = 0;
      retcode        = _TRUE;
    }
    else
    {
      retcode = SetError(TE_TOOLESSMEMORY);
    }
  }
  else
  {
    retcode = SetError(TE_CANTOPEN);
  }

  if(retcode == _FALSE)
  {
    ToolData.File.Close();
    free_MEM_INDEX(&ToolData.MemIndex);
  }
  else
  {
    retcode = _TRUE;
    ptr = (_FBOOL_PT)find_lasal_method(ToolData.Pthis, TOOLCAT_SAVE_START);
    if(ptr != NULL)
    {
      void *pt = ToolData.Pthis;
      load_this(pt);
      retcode = ptr(&ToolData);
    }
  }

  if(retcode == _TRUE)
  {
    txt[0] = 13;
    txt[1] = 10;
    ToolData.File.Write("[VERSION: 2]", 12);

    ToolData.File.Write(txt, 2);

    ToolData.CrcOffset = ToolData.File.Tell();
    WriteCrc(0);
    ToolData.EndHeaderOffset = ToolData.File.Tell();

    if(ToolData.File.GetError() == _FALSE)
    {
      ToolData.ToolState = TS_SAVERUN;
    }
    else
    {
      ToolData.File.Close();
      SetError(TE_CANTOPEN);

      retcode = _FALSE;
    }
  }

  return(retcode);
}

_BOOL _ToolCatalogue::SaveRun(void)
{
  _BOOL  retcode = _TRUE;
  _UWORD runcnt  = TOOLCAT_RUNCNT;

  do
  {
    if(ToolData.ActNo < ToolData.MemIndex.used)
    {
      if(SaveServer(ToolData.MemIndex.ptr[ToolData.ActNo]) == _FALSE)
      {
        if(ToolData.File.GetError() == _FALSE)
        {
          retcode = SetError(TE_CANTSAVEENTRY);
        }
        else
        {
          if(ToolData.File.GetErrorState() == H_NOMEMORY)
            retcode = SetError(TE_TOOLESSMEMORY);
          else
            retcode = SetError(TE_DISK_ERROR);
        }
      }

      if(retcode == _TRUE)
      {
        ToolData.ActNo ++;
        if(ToolData.ActNo >= ToolData.MemIndex.used)
          ToolData.ToolState = TS_SAVEEND;
      }
    }
    else
    {
      ToolData.ToolState = TS_SAVEEND;
    }

    if(retcode == _FALSE)
      ToolData.ToolState = TS_SAVEEND;

    runcnt -= 1;
    if(ToolData.ToolState != TS_SAVERUN)
      runcnt = 0;
  }
  while(runcnt);

  return(retcode);
}

/*
_BOOL _ToolCatalogue::SaveRun(void)
{
  _BOOL retcode = _TRUE;


  if(ToolData.ActNo < ToolData.MemIndex.used)
  {
    if(SaveServer(ToolData.MemIndex.ptr[ToolData.ActNo]) == _TRUE)
    {
      ToolData.ActNo ++;
      if(ToolData.ActNo >= ToolData.MemIndex.used)
        ToolData.ToolState = TS_SAVEEND;
    }
    else
    {
      retcode = SetError(TE_CANTSAVEENTRY);
    }
  }
  else
  {
    ToolData.ToolState = TS_SAVEEND;
  }

  if(retcode == _FALSE)
    ToolData.ToolState = TS_SAVEEND;

  return(retcode);
}
*/

_BOOL _ToolCatalogue::SaveEnd(void)
{
  _FVOID_PT ptr;

  if(ToolData.EndHeaderOffset != 0)
  {
    ToolData.Crc = ToolData.File.Make_CRC(ToolData.EndHeaderOffset);
    if(ToolData.File.Seek(ToolData.CrcOffset, SEEK_SET) == _TRUE)
      WriteCrc(ToolData.Crc);
  }

 #ifdef USER_KM
  if(ToolData.File.Close(_TRUE) == _FALSE)
    SetError(TE_DISK_ERROR);
 #endif

  ptr = (_FVOID_PT)find_lasal_method(ToolData.Pthis, TOOLCAT_SAVE_END);
  if(ptr != NULL)
  {
    void *pt = ToolData.Pthis;
    load_this(pt);
    ptr(&ToolData);
  }

 #ifndef USER_KM
  if(ToolData.File.Close(_TRUE) == _FALSE)
    SetError(TE_DISK_ERROR);
 #endif

  ToolData.ToolState = TS_FINISH;
  return(_TRUE);
}

// ****************************************************************************
// ** LOAD
// ****************************************************************************

_BOOL _ToolCatalogue::SingleLoad(_ASCII *line)
{
  _BOOL              retcode = _TRUE;
  _BOOL              retstate = _TRUE;
  _BOOL              skip = _FALSE;
  _VARLABEL          vl;
  _RESULT            result;
  _VARIABLE          variable;
  _FBOOL_PT_PT_PT_PT ptr;
  _TOOLERROR         toolerror;

  init_VARLABEL(&vl);
  init_RESULT(&result);

  toolerror = scan_line(&skip, NULL, &vl, &result, line, &ToolData);

  if(skip == _FALSE)
  {
    if(toolerror != TE_NOERROR)
    {
      retcode = SetError(toolerror);
    }
    else 
    {
      if(VarList.GetVariable(&variable, &vl) == _TRUE)
      {
        retcode = _TRUE;
        get_minmax(&result, &variable, NOCARE_LIMITS);

        ptr = (_FBOOL_PT_PT_PT_PT)find_lasal_method(ToolData.Pthis, TOOLCAT_LOAD_RUN);
        if(ptr != NULL)
        {
          void *pt = ToolData.Pthis;
          load_this(pt);
          retstate = ptr(&ToolData, &result, &variable, &vl);
        }

        if((retcode == _TRUE) && (retstate == _TRUE))
        {

          switch(xset_dat(&result, vl.info[0].value, ToolData.care_state))
          {
            case ERROR_CAUSED_BY_NO_ERROR    :
              retcode = _TRUE;
              break;

            case ERROR_CAUSED_BY_STATESCHEME :
              retcode = SetError(TE_NO_STATESCHEME, &result, &variable, &vl);
              break;

            case ERROR_CAUSED_BY_ACCESSLEVEL :
              retcode = SetError(TE_NO_ACCESS, &result, &variable, &vl);
              break;

            case ERROR_CAUSED_BY_CHKBIT      :
              retcode = SetError(TE_NO_CHKBIT, &result, &variable, &vl);
              break;

            case ERROR_CAUSED_BY_LIMITS      :
              retcode = SetError(TE_ERROR_LIMITS, &result, &variable, &vl);
              break;

            case ERROR_CAUSED_BY_NO_IDEA     :
            default                          :
              retcode = SetError(TE_CANTSAVEENTRY, &result, &variable, &vl);
              break;
          }
        }
      }
      else
      {
        retcode = SetError(TE_UNKNOWN_SERVER);
      }
    }
  }
  
  return(retcode);
}

_BOOL _ToolCatalogue::LoadStart(_ASCII *dpne)
{
  _BOOL     retcode;
  _FBOOL_PT ptr;
  _ASCII    xdpne[_MAX_PATHLENGTH];

  ToolData.ClassRevision = GetRevisionOfClass((char*)"_ToolCat");

  if(ToolData.File.Open(LOAD, MakeRamFile(xdpne, dpne)) == _TRUE)
  {
    ToolData.Version   = 0;
    retcode = _TRUE;

    ptr = (_FBOOL_PT)find_lasal_method(ToolData.Pthis, TOOLCAT_LOAD_START);
    if(ptr != NULL)
    {
      void *pt = ToolData.Pthis;
      load_this(pt);
      retcode = ptr(&ToolData);
    }

    if(retcode == _FALSE)
      ToolData.File.Close();
  }
  else
  {
    retcode = SetError(TE_CANTOPEN);
  }

  if(retcode == _TRUE)
  {
    if(ToolData.File.GetError() == _FALSE)
    {
      ToolData.ToolState = TS_LOADVERSION;
      retcode = LoadHeader();
    }
  }

  return(retcode);
}

_BOOL _ToolCatalogue::CheckKey(_CRC32 key)
{
  return(check_superpassword(key, 7));
}

_BOOL _ToolCatalogue::LoadHeader(void)
{
  _BOOL   retcode = _FALSE;
  _ASCII  line[600];
  _UDWORD crc;
  _BOOL   end = _FALSE;

  if(ToolData.File.GetError() == _FALSE)
  {
    while(end == _FALSE)
    {
      if(ToolData.File.Read_CRLF(line, sizeof(line)) != _FALSE)
      {
        DelComment(line, sizeof(_ASCII));
        DelSpace(line, sizeof(_ASCII));

        if(line[0] == '[')
        {
          switch(ToolData.ToolState)
          {
            case TS_LOADVERSION:
            {
              if(MemCmp(line, "[VERSION:", 9) == 0)
              {
                ToolData.Version = AToD(&line[9], sizeof(_ASCII));
                if((ToolData.Version < 2) || (SuperMode == _TRUE))
                {
                  ToolData.CrcOk = _TRUE;  // CRC erst ab Version 2
                  ToolData.ToolState = TS_LOADRUN;
                  end = _TRUE;
                }
                else
                {
                  ToolData.ToolState = TS_LOADCRC;
                }
                retcode = _TRUE;
              }
              break;
            }

            case TS_LOADCRC:
            {
              if(MemCmp(line, "[CRC:", 5) == 0)
              {
                ToolData.Crc = HToD(&line[5], sizeof(_ASCII));
                if(ToolData.CrcOk == _FALSE)
                {
                  crc = ToolData.File.Make_CRC(ToolData.File.Tell());

                  if((crc == ToolData.Crc) || (CheckKey(ToolData.Crc) == _TRUE))
                  {
                    ToolData.CrcOk = _TRUE;
                  }
                  else if(SetError(TE_INVALID_CRC) == _TRUE)
                  {
                    ToolData.CrcOk = _TRUE;
                  }
                }

                if(ToolData.CrcOk == _TRUE) // umbau 28.04.06 und 29.11.06 für applikation
                {
                  ToolData.ToolState = TS_LOADRUN;
                }

                retcode = _TRUE;
              }

              end = _TRUE;
              break;
            }

            default :
              break;
          }
        }
        else if(line[0] == '*')
        {
          retcode = _TRUE;
        }
      }
      else
      {
        end = _TRUE;
      }
    }
  }

  if(retcode == _FALSE)
  {
    SetError(TE_NO_TOOLCAT);
    ToolData.ToolState = TS_LOADEND;
  }

  return(retcode);
}

_BOOL _ToolCatalogue::IdleRun(void)
{
  _BOOL     retcode = _TRUE;
  _FBOOL_PT ptr = (_FBOOL_PT)find_lasal_method(ToolData.Pthis, TOOLCAT_IDLE_RUN);

  if(ptr != NULL)
  {
    void *pt = ToolData.Pthis;
    load_this(pt);
    retcode = ptr(&ToolData);
  }

  return(retcode);
}

_BOOL _ToolCatalogue::LoadRun(void)
{
  _ASCII  line[600];
  _BOOL   done;
  _BOOL   retcode = _TRUE;
  _UWORD  runcnt  = TOOLCAT_RUNCNT;

  do
  {
    if(ToolData.File.GetError() == _FALSE)
    {
      if(ToolData.File.Read_CRLF(line, sizeof(line)) != _FALSE)
      {
//        DelComment(line, sizeof(_ASCII));       // ifnec. delete present comment
//        DelSpace(line, sizeof(_ASCII));
        clean_line(line, sizeof(line));

        if(line[0] != 0)
        {
          done = _FALSE;
          if(line[0] == '*')
            done = _TRUE;

          if((done == _FALSE) && (ToolData.CrcOk == _TRUE))
            retcode = SingleLoad(line);
        }
      }
      else
      {
        ToolData.ToolState = TS_LOADEND;
      }
    }

    if(retcode == _FALSE)
      ToolData.ToolState = TS_LOADEND;

    runcnt -= 1;
    if(ToolData.ToolState != TS_LOADRUN)
      runcnt = 0;
  }
  while(runcnt);

  return(retcode);
}

_BOOL _ToolCatalogue::LoadEnd(void)
{
  _FVOID_PT ptr;

  ptr = (_FVOID_PT)find_lasal_method(ToolData.Pthis, TOOLCAT_LOAD_END);
  if(ptr != NULL)
  {
    void *pt = ToolData.Pthis;
    load_this(pt);
    ptr(&ToolData);
  }

  ToolData.File.Close();
  ToolData.ToolState = TS_FINISH;
  return(_TRUE);
}

_BOOL _ToolCatalogue::Run(void)
{
  if(ToolData.ToolState != TS_FINISH)
  {
    _BOOL retcode = _FALSE;
    
   #ifndef USER_HTC
    if(ToolData.ToolError == TE_NOERROR)
   #endif
    {
      switch(ToolData.ToolState)
      {
        case TS_IDLE        : retcode = IdleRun();
                              break;
        case TS_LOADSTART   : retcode = LoadStart(ToolData.dpne);
                              break;
        case TS_LOAD        : if(LoadStart(ToolData.dpne) == _TRUE)
                              {
//                                if(LoadHeader() == _TRUE)
                                {
                                  while(ToolData.ToolState == TS_LOADRUN)
                                    LoadRun();
                                  retcode = LoadEnd();
                                }
                              }
                              break;
        case TS_LOADRUN     : retcode = LoadRun();
                              break;
        case TS_LOADEND     : retcode = LoadEnd();
                              break;


        case TS_SAVE        : if(SaveStart(ToolData.dpne) == _TRUE)
                              {
                                while(ToolData.ToolState == TS_SAVERUN)
                                  SaveRun();
                                retcode = SaveEnd();
                              }
                              break;
        case TS_SAVESTART   : retcode = SaveStart(ToolData.dpne);
                              break;
        case TS_SAVERUN     : retcode = SaveRun();
                              break;
        case TS_SAVEEND     : retcode = SaveEnd();
                              break;
        default             : break;
      }
    }
    
    return(retcode);
  }

  return _FALSE;
}

_BOOL _ToolCatalogue::Teach(const _ASCII *dpne, _TOOLSTATE ts, _STATISTIC *pf)
{
  _BOOL retcode = _FALSE;

  if((ToolData.ToolState == TS_FINISH)&&(dpne != NULL))
  {
    Free();

    StrCpy(ToolData.dpne, sizeof(_ASCII), dpne, sizeof(_ASCII));
    ToolData.Filter    = *pf;
    ToolData.ToolState = ts;
    Run();
    retcode = _TRUE;
  }

  return(retcode);
}

_TOOLDATA* _ToolCatalogue::Get_TOOLDATA(void)
{
  return(&ToolData);
}

_BOOL load_toolcat(const _ASCII *dpne, _UDWORD *pf)
{
  _STATISTIC filter;
  _BOOL      retcode = _FALSE;
  
  if(FileExist((_ASCII*)dpne) == _TRUE)
  {
    init_STATISTIC(&filter);
    filter.parameter = 1;
    filter.group     = *pf;
    ToolCatalogue.Teach(dpne, TS_LOAD, &filter);

    if(ToolCatalogue.Get_TOOLDATA()->ToolError == TE_NOERROR)
    {
      if(ToolCatalogue.Get_TOOLDATA()->CrcOk == _TRUE) // wunsch appli (leichr) 02.02.2015
      {
        retcode = _TRUE;
      }
    }

    free_TOOLDATA(ToolCatalogue.Get_TOOLDATA());
  }
  return(retcode);
}

_BOOL save_toolcat(const _ASCII *dpne, _UDWORD *pf)
{
  _STATISTIC filter;
  _BOOL      retcode = _FALSE;

  init_STATISTIC(&filter);
  filter.parameter = 1;
  filter.group     = *pf;
  ToolCatalogue.Teach(dpne, TS_SAVE, &filter);

  if(ToolCatalogue.Get_TOOLDATA()->ToolError == TE_NOERROR)
    retcode = _TRUE;

  free_TOOLDATA(ToolCatalogue.Get_TOOLDATA());
  return(retcode);
}

_TOOLDATA *get_TOOLDATA(void)
{
  return(ToolCatalogue.Get_TOOLDATA());
}

_BOOL ToolCatTeach(const _ASCII *dpne, _TOOLSTATE state, _STATISTIC *filter)
{
  return(ToolCatalogue.Teach(dpne, state, filter));
}



