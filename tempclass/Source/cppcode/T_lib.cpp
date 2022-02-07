#include "T_IPC.H"
#include "T_NUC.H"

#ifdef __TURBOC__
#include <math.h>
#endif

#define ICONID 15
#define SA33149
#define SA54151 // WordWrap in combobox
#define SA55048 // DEVVIS-29

_CHAR *get_text(_ANYTHING*, _TextPuffer*, _MEM_VARLABEL*, _BOOL);
void   lasal_to_DATE(_DATE*, _IVALUE);
void   DATE_to_lasal(_IVALUE*, _DATE*);
void   lasal_to_TIME(_TIME*, _IVALUE);
void   TIME_to_lasal(_IVALUE*, _TIME*);
//void   DToA(void*, _DWORD, _UWORD, _UWORD);
_UWORD StrSize(const void*, _UWORD);
//_UWORD StrChr(void*, _UNI, _UWORD);
//void  *StrUp(void*, _UWORD, _UWORD);
void   DelSpace(void*, _UWORD);
void   DelChr(void*, _UWORD, _UWORD);
void   StrFill(void*, _UNI, _UWORD, _UWORD);     // fill string with character
void   UniToAscii(_ASCII*, _UNI*, _UDWORD);
void   AsciiToUni(_UNI*, _ASCII*, _UDWORD);
//void   StrCpy(void*, _UWORD, const void*, _UWORD);
void   StrNCpy(void*, _UWORD, const void*, _UWORD, _UWORD);
void   StrCat(void*, _UWORD, const void*, _UWORD);
_UWORD StrCmp(void*, _UWORD, void*, _UWORD);
void  *find_lasal_method(void*, _UWORD);
_DOIT  call_enclosed_name_method(_UWORD, _IO*, _VARIABLE*, _RESULT*, _EDITOR*, _EVENT*, _BOOL);
void   NumToStr(void*, _DWORD, _UWORD);
_DWORD AsciiToNo(_ASCII**);
_DWORD get_multiplicator(_UWORD);
void   RealToStr(void*, _REAL, _UWORD, _UWORD);
_BOOL  StrToNum(_DWORD*, void*, _UWORD);

_CHAR  *make_parameterstring(_CHAR*, _MEM_VARLABEL*, _BOOL);
void    out_ANYTHING(_ANYTHING*, _ROOM*, _FONT, _ATTRIB, _FRAMETYPE, _COLOR, _COLOR, _COLOR, _TextPuffer*);
void    OutTextAlign(void*, _UNI, _ROOM*, _FONT, _ATTRIB, _FRAMETYPE, _COLOR, _COLOR, _COLOR, _UWORD);
void    OutMultiTextAlign(void*, _UNI, _ROOM*, _FONT, _ATTRIB, _FRAMETYPE, _COLOR, _COLOR, _COLOR, _UWORD);
_BOOL   SizeMultiTextAlign(_UWORD*, _UWORD*, void*, _FONT, _ATTRIB, _UWORD);
//_REGION ActivateAndRegion(_REGION*, _REGION*, _ROOM*, _ATTRIB);
_REGION ActivateAndRegion(_REGION*, _REGION*, _ROOM*);
void    DeactivateAndRegion(_REGION, _REGION*, _REGION*);
_UWORD  sqroot(_DWORD);

_BOOL   bmp_size(_UWORD*, _UWORD*, _UWORD, _ATTRIB);
_POSRESULT calculate_position(_POSITION*, _POSITION*, _EVENT*, _ROOM*);
_KOORD  linear_distance(_RESULT*, _KOORD);
void   *get_ANYTHING(_ANYTHING*, _TextPuffer*);
_DOT    get_left_top(_DOT, _WORD, _WORD, _ATTRIB);
_UWORD  day_of_week(_DATE*);

_ATTRIB SetOutlineAttrib(_ATTRIB att);
_BOOL   merge_MEM_VARLABEL(_MEM_VARLABEL*, _CHAR*, _MEM_VARLABEL*, _CHAR*, _MEM_VARLABEL*, _CHAR*);

#ifdef PGTEST
void   Annahme(const _ASCII*, int);     //
#endif

_UWORD make_bmpno(void *p, _UWORD chr_size, _UWORD *anz);
_DWORD AToDsub(void *pt, _UWORD chr_size, _UWORD *pcnt);


_UBYTE MyRadioButton[] = {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
								  0,0,0,1,1,2,2,2,2,5,5,1,1,0,0,0,
								  0,0,1,2,2,2,2,5,5,5,5,5,3,1,0,0,
								  0,1,2,2,2,2,5,5,5,5,5,5,3,3,1,0,
								  0,1,2,2,2,5,5,5,5,5,5,5,5,3,1,0,
								  1,2,2,2,5,5,5,4,4,5,5,5,5,3,3,1,
								  1,2,2,2,5,5,4,4,4,4,5,5,5,3,3,1,
								  1,2,2,5,5,4,4,4,4,4,4,5,5,3,3,1,
								  1,2,2,5,5,4,4,4,4,4,4,5,5,3,3,1,
								  1,2,2,5,5,5,4,4,4,4,5,5,3,3,3,1,
								  1,2,2,5,5,5,5,4,4,5,5,5,3,3,3,1,
								  0,1,2,5,5,5,5,5,5,5,5,3,3,3,1,0,
								  0,1,2,2,5,5,5,5,5,5,3,3,3,3,1,0,
								  0,0,1,2,5,5,5,5,5,3,3,3,3,1,0,0,
								  0,0,0,1,1,5,5,3,3,3,3,1,1,0,0,0,
								  0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0};

_UBYTE  MyCheckBox[]   = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
								  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
								  1,2,5,5,5,5,5,5,5,5,5,5,5,5,3,1,
								  1,2,5,4,5,5,5,5,5,5,5,5,4,5,3,1,
								  1,2,5,5,4,5,5,5,5,5,5,4,5,5,3,1,
								  1,2,5,5,5,4,5,5,5,5,4,5,5,5,3,1,
								  1,2,5,5,5,5,4,5,5,4,5,5,5,5,3,1,
								  1,2,5,5,5,5,5,4,4,5,5,5,5,5,3,1,
								  1,2,5,5,5,5,5,4,4,5,5,5,5,5,3,1,
								  1,2,5,5,5,5,4,5,5,4,5,5,5,5,3,1,
								  1,2,5,5,5,4,5,5,5,5,4,5,5,5,3,1,
								  1,2,5,5,4,5,5,5,5,5,5,4,5,5,3,1,
								  1,2,5,4,5,5,5,5,5,5,5,5,4,5,3,1,
								  1,2,5,5,5,5,5,5,5,5,5,5,5,5,3,1,
								  1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,
								  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};




#ifdef SA55048
 #ifdef UC_IPC
  #define MAX_TEMPTEXT_LEN 3001
 #else
  #define MAX_TEMPTEXT_LEN 501
 #endif
#else
 #ifdef UC_IPC
  #define MAX_TEMPTEXT_LEN 1001
 #else
  #define MAX_TEMPTEXT_LEN 501
 #endif
#endif

//#define TempText_WATCHDOG
static _CHAR         TempText_Puffer[MAX_TEMPTEXT_LEN * 16];
static unsigned long TempText_Index = 0;
_CHAR *get_temptext(void)
{
 #ifdef TempText_WATCHDOG
  if(TempText_Index == 0)
  {
    TempText_Puffer[MAX_TEMPTEXT_LEN *  1 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN *  2 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN *  3 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN *  4 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN *  5 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN *  6 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN *  7 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN *  8 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN *  9 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN * 10 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN * 11 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN * 12 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN * 14 - 1] = 0;
    TempText_Puffer[MAX_TEMPTEXT_LEN * 15 - 1] = 0;
  }
 #endif

  unsigned long idx = lib_atomic_incU32(&TempText_Index) & 0x0000000F;
  _CHAR *retcode = &TempText_Puffer[idx * MAX_TEMPTEXT_LEN];

 #ifdef TempText_WATCHDOG
  if(retcode[MAX_TEMPTEXT_LEN - 1] != 0)
  {
    // problem !!!
    retcode[MAX_TEMPTEXT_LEN - 1] = 0;
  }
 #endif
  
  *retcode = 0;

  return retcode;
}

void Sleep(_UDWORD duration)
{
  _UDWORD tix = get_abstime();
  while((get_abstime() - tix) < duration);
}

void WriteU4(_UBYTE *dst, _UDWORD src)
{
 #ifdef _LSL_TARGETARCH_ARM
  // wegen missaligned access exeption
  dst[0] = (_UBYTE)(src);
  dst[1] = (_UBYTE)(src >> 8);
  dst[2] = (_UBYTE)(src >> 16);
  dst[3] = (_UBYTE)(src >> 24);
 #else
  *(_UDWORD*)dst = src;
 #endif
}

void WriteU2(_UBYTE *dst, _UWORD src)
{
 #ifdef _LSL_TARGETARCH_ARM
  // wegen missaligned access exeption
  dst[0] = (_UBYTE)(src);
  dst[1] = (_UBYTE)(src >> 8);
 #else
  *(_UWORD*)dst = src;
 #endif
}

void WriteU1(_UBYTE *dst, _UBYTE src)
{
  *(_UBYTE*)dst = src;
}

_UDWORD ReadU4(_UBYTE *src)
{
 #ifdef _LSL_TARGETARCH_ARM
  // wegen missaligned access exeption
  _UDWORD retcode;
  _UBYTE *ph = (_UBYTE*)&retcode;
  ph[0] = src[0];
  ph[1] = src[1];
  ph[2] = src[2];
  ph[3] = src[3];
  return retcode;
 #else
  return *(_UDWORD*)src;
 #endif
}

_UWORD ReadU2(_UBYTE *src)
{
 #ifdef _LSL_TARGETARCH_ARM
  // wegen missaligned access exeption
  _UWORD retcode;
  _UBYTE *ph = (_UBYTE*)&retcode;
  ph[0] = src[0];
  ph[1] = src[1];
  return retcode;
 #else
  return *(_UWORD*)src;
 #endif
}

_UBYTE ReadU1(_UBYTE *src)
{
  return *(_UBYTE*)src;
}

void Trace(const _ASCII *txt)
{
  _FVOID_PT ptr = (_FVOID_PT)find_lasal_method(LseThisPointer, METHOD_LSE_TRACE);

  if(ptr != NULL)
  {
	 load_this(LseThisPointer);
	 ptr((void*)txt);
  }
}

void FatalError(const _ASCII *txt)
{
  ScanBar.End(ScanBar.Add(txt, 0));
  Trace(txt);
}

#ifdef PGTEST
void Annahme(const _ASCII *file, int line)
{
  _ASCII    txt[200];

  StrCpy(txt, sizeof(_ASCII), "(!)ASSERT:", sizeof(_ASCII));
  StrCat(txt, sizeof(_ASCII), file, sizeof(_ASCII));
  if(line != 0)
  {
    StrCat(txt, sizeof(_ASCII), ":", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], line, 0x0105, sizeof(_ASCII));
  }

  if(IsGraphInit() != 0)
  {
//    Font.Set(DEFFONT, NULL);
//    OutTextXY(0, 0, txt, T_COPY|T_SOLID, MakeColor_Makro(RED, WHITE), 1);
    Debug(txt, sizeof(_ASCII), 0);
  }

  Trace(txt);
}
#endif

_UWORD TxtWidthFont(void *str, _UWORD pos, _UWORD chr_size, _FONT font)
{
  _UDWORD retcode;

  if(chr_size == 1)
  {
    _ASCII *pa = (_ASCII*)str;
    _ASCII ma;

    ma = pa[pos];
    pa[pos] = 0;
    Font.Set(font, NULL);
    retcode = StrWidth(str, 1);
    pa[pos] = ma;
  }
  else
  {
    _UNI *pu = (_UNI*)str;
    _UNI mu;

    mu = pu[pos];
    pu[pos] = 0;
    Font.Set(font, NULL);
    retcode = StrWidthLse(str, 2);
    pu[pos] = mu;
  }


  return(retcode);
}

_DWORD string_to_numeric(_CHAR *txt, _UWORD nksoll)
{
  _CHAR  chr;
  _DWORD div, op1, op2, retcode = 0;
  _UWORD i;
  _UWORD len;
  _UWORD neg = 0;
  _BOOL  st, nkstart = _FALSE;
  _UWORD nkist = 0;
  _CHAR  *tmp = txt;

  do
  {
    chr = *tmp;
    switch(chr)
    {
      case '+' :
      case '*' :
      case '/' : *tmp = 0;
                 op1  = string_to_numeric(txt, nksoll);
                 op2  = string_to_numeric(tmp+1, nksoll);
                 *tmp = chr;
                 st   = _FALSE;

                 div = 1;
                 for(i=0; i<nksoll; i++)
                   div = div * 10;

                 if(chr == '*')
                 {
                   if(div != 0)
                   {
                     retcode = AMBDC(op1, op2, div);
                     st = _TRUE;
                   }
                 }
                 else if(chr == '+')
                 {
                   retcode = op1 + op2;
                   st = _TRUE;
                 }
                 else if(op2 != 0)
                 {
                   if(op2 != 0)
                   {
                     retcode = AMBDC(op1, div, op2);
                     st = _TRUE;
                   }
                 }
                 if(st == _TRUE)
                 {
                   return(retcode);
                 }
                 break;
    }
    tmp ++;
  }
  while(chr != 0);

  len = StrLen(txt, sizeof(_CHAR));

  for(i=0; i<len; i++)
  {
    chr = txt[i];
    if(chr == '-')
    {
      neg = neg ^ 1;
    }
    else if((chr == '.') || (chr == ','))
    {
      nkstart = _TRUE;
    }
    else if((chr >= '0') && (chr <= '9'))
    {
      if(nkstart == _TRUE)
        nkist ++;

      retcode = (retcode * 10) + (chr - '0');
    }
  }

  if(nkist < nksoll)
  {
    switch(nksoll - nkist)
    {
      case 1 : retcode *= 10;         break;
      case 2 : retcode *= 100;        break;
      case 3 : retcode *= 1000;       break;
      case 4 : retcode *= 10000;      break;
      case 5 : retcode *= 100000L;    break;
      case 6 : retcode *= 1000000L;   break;
      case 7 : retcode *= 10000000L;  break;
      case 8 : retcode *= 100000000L; break;
    }
  }
  else if(nkist > nksoll)
  {
    switch(nkist - nksoll)
    {
      case 1 : retcode /= 10;         break;
      case 2 : retcode /= 100;        break;
      case 3 : retcode /= 1000;       break;
      case 4 : retcode /= 10000;      break;
      case 5 : retcode /= 100000L;    break;
      case 6 : retcode /= 1000000L;   break;
      case 7 : retcode /= 10000000L;  break;
      case 8 : retcode /= 100000000L; break;
    }
  }

  if(neg != 0)
    retcode = -retcode;

  return(retcode);
}

#ifdef UC_IPC
_IVALUE string_to_value(_IVALUE value, _VARINFO *pvi)
{
  // vorsicht: diese funktion existiert auch in st.
  // compilerschalter UC_IPC entscheidet welche verwendet wird

  _IVALUE retcode = value;

 #ifdef UC_REAL
  _REAL   rval;
  if((pvi->vartype == BINARY)&&(pvi->format == SIZE_REAL))
  {
    rval = value;
    *(_REAL*)&retcode = (rval/get_multiplicator(VarList.GetDezPoint(pvi)));
  }
 #endif

  return(retcode);
}
#endif

#ifdef USER_DEKEMA
_DWORD AToDekema(_ASCII *a)
{
  _ASCII chr;
  _DWORD retcode = 0;
  _BOOL  run = _TRUE;

  do
  {
    chr = *a;
    a  += 1;

    if((chr >= '0') && (chr <= '9'))
    {
      retcode = (retcode * 10) + (chr - '0');
    }
    else if(chr == 0)
    {
      run = _FALSE;
    }
  }
  while(run == _TRUE);

  return(retcode);
}
#endif

_UDWORD string_to_ipaddress(_CHAR *txt)
{
  _UDWORD retcode = 0;
  _UBYTE  adr[4];
  _UWORD  len = StrLen(txt, sizeof(_CHAR));
  _UWORD  i, pos = 0;
  _CHAR   chr;

  adr[0] = 0;
  adr[1] = 0;
  adr[2] = 0;
  adr[3] = 0;

  for(i=0; i<len; i++)
  {
    chr = txt[i];
    if((chr >= '0') && (chr <= '9'))
    {
      if(pos < 4)
        adr[pos] = (adr[pos] * 10) + chr - '0';
      else
        i = len;
    }
    else
	  {
      pos++;
    }
  }

  for(i=0; i<4; i++)
    retcode = (retcode << 8) | adr[3-i];
  
  return retcode;
}


_BOOL string_to_time(_TIME *p, _CHAR *txt, _UWORD format)
{
  _UWORD len, i, pos, value;
  _CHAR  chr;
  _BOOL  retcode = _TRUE;
  _UWORD p1, p2, p3;
  _BOOL  pm = _FALSE;

  init_TIME(p);

 #ifdef EDIT_DEFAULT_TIMEDATE
  #ifdef USER_DEKEMA
   if(format == SIZE_MMSS)
   {
     // definition dekema: nur format MMSS bei eingabe von 0 = defaulttime
     _ASCII d_txt[100];
     StrCpy(d_txt, sizeof(_ASCII), txt, sizeof(_CHAR));
     if(AToDekema(d_txt) == 0)
     {
       p->second = 0xFF;
       p->minute = 0xFF;
       p->hour   = 0xFF;
       return _TRUE;
     }
   }
  #else
   if(txt[0] == EDIT_DEFAULT_TIMEDATE)
   {
     p->second = 0xFF;
     p->minute = 0xFF;
     p->hour   = 0xFF;
     return _TRUE;
   }
  #endif
 #endif

  len = StrLen(txt, sizeof(_CHAR));
  pos = 0;

  p1 = 0;
  p2 = 0;
  p3 = 0;

  for(i=0; i<len; i++)
  {
    chr = txt[i];
    if((chr >= '0') && (chr <= '9'))
    {
      value = chr - '0';
      switch(pos)
      {
        case  0 : p1 = (p1 * 10) + value; break;
        case  1 : p2 = (p2 * 10) + value; break;
        case  2 : p3 = (p3 * 10) + value; break;
        default : i = len;                break;
      }
    }
    else
    {
      if((chr == 'P')||(chr == 'p'))
      {
        pm  = _TRUE;
        pos ++;
      }
      else if((chr == 'A')||(chr == 'a'))
      {
        pm  = _FALSE;
        pos ++;
      }
      else if((chr != 'M')&&(chr != 'm'))
      {
        pos++;
      }
    }
  }

  if(format == SIZE_HHMM)
  {
    p->hour   = p1;
    p->minute = p2;
    p->second = 0;
   #ifdef USER_DEKEMA
    if((p->minute > 59)||(p->hour > 23))
      retcode = _FALSE;
   #endif
  }
  else if(format == SIZE_HHMMSS)
  {
    p->hour   = p1;
    p->minute = p2;
    p->second = p3;
   #ifdef USER_DEKEMA
    if((p->second > 59)||(p->minute > 59)||(p->hour > 23))
      retcode = _FALSE;
   #endif
  }
  else if(format == SIZE_MMSS)
  {
    pm        = _FALSE;
    p->hour   = 0;
    p->minute = p1;
    p->second = p2;
   #ifdef USER_DEKEMA
    // bei dekema können bei MMSS 0-99 minuten eingegeben werden
    if((p->minute > 99)||(p->second > 59))
      retcode = _FALSE;
   #endif
  }

  if(pm == _TRUE)
  {
    if(p->hour < 12)
      p->hour += 12;
  }

 #ifndef USER_DEKEMA
  if((p->second > 59)||(p->minute > 59)||(p->hour > 23))
    retcode = _FALSE;
 #endif

  return(retcode);
}

_BOOL string_to_date(_DATE *p, _CHAR *txt, _UWORD format)
{
  _UWORD len, i, pos, value[3];
  _CHAR  chr;
  _BOOL  retcode = _TRUE;
  _BOOL  usa = _FALSE;
  _BOOL  iso = _FALSE;

 #ifdef EDIT_DEFAULT_TIMEDATE
  if(txt[0] == EDIT_DEFAULT_TIMEDATE)
  {
    init_DATE(p);
    p->day   = 0xFF;
    p->month = 0xFF;
    p->year  = 0xFFFF;
    return(retcode);
  }
 #endif

  len = StrLen(txt, sizeof(_CHAR));
  pos = 0;

  value[0] = 0xFFFF;
  value[1] = 0xFFFF; //p->date.month;
  value[2] = 0xFFFF; //p->date.year;

  for(i=0; i<len; i++)
  {
    chr = txt[i];
    if((chr >= '0') && (chr <= '9'))
    {
      if(value[pos] == 0xFFFF)
        value[pos] = 0;
      value[pos] = (value[pos] * 10) + (chr - '0');
    }
    else
    {
      if(chr == '/')
        usa = _TRUE;
      else if(chr == '-') // sa20024
        iso = _TRUE;

      if(value[1] != 0xFFFF)
        pos = 2;
      else if(value[0] != 0xFFFF)
        pos = 1;
    }
  }

  get_DATE(p);

  if(usa == _TRUE)
  {
    i = value[0];
    value[0] = value[1];
    value[1] = i;
  }
  else if(iso == _TRUE)
  {
    // format YYYY-MM-DD, YY-MM-DD oder YY-MM
    i        = value[2];
    value[2] = value[0];
    value[0] = i;
  }
  
  if(value[2] != 0xFFFF)
  {
    if(value[2] < 70)
      value[2] += 2000;
    else if(value[2] < 100)
      value[2] += 1900;
  }

  if(value[1] != 0xFFFF)
  {
    if((value[1] < 0)||(value[1] > 12))
      return _FALSE;
    p->month = (_UBYTE)value[1];
  }

  if(value[2] != 0xFFFF)
    p->year = value[2];
  
  _UWORD daysmonth = GetDayOfMonth(p->month, p->year);
  
  if(value[0] != 0xFFFF)
  {
    if((value[0] < 0)||(value[0] > daysmonth))
      return _FALSE;
      
    p->day = (_UBYTE)value[0];
  }

  if((p->year < 1900)||(p->year > 2099)||(p->day < 1)||(p->day > daysmonth))
    retcode = _FALSE;

  return(retcode);
}

void AddZoom(_ZOOM *dst, _ZOOM *zoom)
{
  dst->x = Zoom(dst->x, zoom->x);
  dst->y = Zoom(dst->y, zoom->y);
}

void AddOffset(_DOT *dst, _DOT *offset, _ZOOM *zoom)
{
  dst->x = Zoom(zoom->x, offset->x) + dst->x;
  dst->y = Zoom(zoom->y, offset->y) + dst->y;
}

_KOORD MakeKoordX(_KOORD x)
{
  return(x);
}

_KOORD MakeKoordY(_KOORD y)
{
  return(y);
}

void StrCat(void *p1, _UWORD size1, const void *p2, _UWORD size2)
{
  _UBYTE *d;
  _UWORD index;

  index = StrLen(p1, size1);
  d = ((_UBYTE*)p1) + index;
  if(size1 > 1)
	 d = d + index;

  StrCpy(d, size1, p2, size2);
}

void StrNCpy(void *dest, _UWORD dest_size, const void *src, _UWORD src_size, _UWORD max_dest_len)
{
  _UDWORD i, size;
  _UBYTE  *pa;
  _UNI    *pu;

  if(max_dest_len > 0)
  {
	 if(StrLen(src, src_size) < max_dest_len)
    {
      StrCpy(dest, dest_size, src, src_size);
    }
    else
    {
      if(dest_size == src_size)
      {
        size = max_dest_len * src_size;
        MemCpy(dest, (void*)src, size);
      }
      else
      {
        size = max_dest_len - 1;
        if(src_size == 1)
        {
          pa = (_UBYTE*)src;
			 pu = (_UNI*)dest;
          for(i=0; i<size; i++)
          {
				*pu = *pa;
            pu++;
            pa++;
          }
        }
        else
        {
          pa = (_UBYTE*)dest;
          pu = (_UNI*)src;
			 for(i=0; i<size; i++)
			 {
				*pa = *pu;
				pa++;
				pu++;
			 }
		  }
		}

		if(dest_size == sizeof(_ASCII))
		{
		  pa = (_UBYTE*)dest;
		  pa[max_dest_len-1] = 0;
		}
		else
		{
		  pu = (_UNI*)dest;
		  pu[max_dest_len-1] = 0;
		}
	 }
  }
}
/*
void StrCpy(void *dest, _UWORD dest_size, const void *src, _UWORD src_size)
{
  _UWORD i, size;
  _UNI   *pu;
  _UBYTE *pa;

  if(dest_size == src_size)
  {
	 size = StrSize(src, src_size) + src_size;
	 MemCpy(dest, (void*)src, size);
  }
  else
  {
	 size = StrLen(src, src_size) + 1;
	 if(src_size == 1)
	 {
		pa = (_UBYTE*)src;
		pu = (_UNI*)dest;
		for(i=0; i<size; i++)
		{
				*pu = *pa;
		  pu++;
		  pa++;
		}
	 }
	 else
	 {
		pa = (_UBYTE*)dest;
		pu = (_UNI*)src;
		for(i=0; i<size; i++)
		{
		  *pa = *pu;
		  pa++;
		  pu++;
		}
	 }
  }
}
*/
_UWORD StrCmp(const void *p1, _UWORD size1, const void *p2, _UWORD size2)
{

  if(size1 == size2)
    return StrXmp((void*)p1, (void*)p2, size1); // regular path

  _UBYTE *a;
  _UWORD *u;
  _UWORD len = StrLen(p1, size1);
  
  if(len == StrLen(p2, size2))
  {
    if(size1 == sizeof(_ASCII))
    {
      a = (_UBYTE*)p1;
      u = (_UWORD*)p2;
    }
    else
    {
      a = (_UBYTE*)p2;
      u = (_UWORD*)p1;
    }

    while(len)
    {
      if(*a != *u)
        return(1);
      a ++;
      u ++;
      len --;
    }
    
    return(0);
  }

  return(1);
}

//_UWORD StrChr(void *p, _UNI c, _UWORD chr_size)
//{
//  _UNI   *pu;
//  _UBYTE *pa;
//  _UWORD len = 0;
//
//  if(chr_size == 1)
//  {
//    pa = (_UBYTE*)p;
//    while(*pa)
//    {
//      if(*pa == c)
//        return(len);
//      len ++;
//      pa  ++;
//    }
//  }
//  else
//  {
//    pu = (_UNI*)p;
//    while(*pu)
//    {
//      if(*pu == c)
//        return(len);
//      len ++;
//      pu  ++;
//    }
//  }
//
//  return(0xFFFF);
//}

_UWORD StrSize(const void *txt, _UWORD chr_size)
{
  _UWORD retcode = StrLen(txt, chr_size);

  if(chr_size == 2)
	 retcode = retcode * sizeof(_UNI);

  return(retcode);
}

void StrFill(void *p, _UNI c, _UWORD chr_size, _UWORD len)
{
  _UWORD i;

  if(len == 0)
    len = StrLen(p, chr_size);

  if(chr_size == 2)
  {
    _UNI *pu = (_UNI*)p;
    for(i=0; i<len; i++)
      pu[i] = c;
  }
  else
  {
    _UBYTE *pa = (_UBYTE*)p;
    for(i=0; i<len; i++)
      pa[i] = c;
  }
}

void *StrUp(void *p, _UWORD chr_size, _UDWORD len)
{
  _UDWORD i;

  if(len == 0)
	 len = StrLen(p, chr_size);

  if(chr_size == 2)
  {
	 _UWORD u, *pu = (_UWORD*)p;

	 for(i=0; i<len; i++)
	 {
		u = *pu;
		if((u >= 'a') && (u <= 'z'))
		  *pu -= ('a' - 'A');
		pu ++;
	 }
  }
  else
  {
	 _UBYTE a, *pa = (_UBYTE*)p;

	 for(i=0; i<len; i++)
	 {
		a = *pa;
		if((a >= 'a') && (a <= 'z'))
		  *pa -= ('a' - 'A');
		pa ++;
	 }
  }

  return(p);
}

void UniToAscii(_ASCII *d, _UNI *s, _UDWORD len)
{
  _UDWORD i;

  if(len == 0)
	 len = StrLen(s, sizeof(_UNI)) + 1;

  for(i=0; i<len; i++)
  {
	 *d = *s;
	 d ++;
	 s ++;
  }
}

void AsciiToUni(_UNI *d, _ASCII *s, _UDWORD len)
{
  _UDWORD i;
  _UBYTE *src = (_UBYTE*)s;

  if(len == 0) 
    len = StrLen(s, sizeof(_ASCII)) + 1;

  for(i=0; i<len; i++)
  {
    *d = *src;
    d ++;
    src ++;
  }
}

// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************

//_UDWORD timediff(_UDWORD t1)
//{
//  _UDWORD tn = get_abstime();
//  return(tn - t1);
//}

void insert_decpoint(_ASCII *txt, _UWORD dz)
{
	_UWORD no, len, i;

  len = StrLen(txt, sizeof(_ASCII)); 

  if(dz >= len)
  {
    no = dz - len + 2; 
    MemMove(&txt[no], txt, (len+1)); 
    for(i=0; i<no; i++)
      txt[i] = '0'; 
    txt[1] = '.';  
  }
  else
  {
    no = len - dz; 
    MemMove(&txt[no+1], &txt[no],(len-no+1)); 
    txt[no] = '.'; 
  }
}

_UWORD sqroot(_DWORD a)
{
  _UWORD retcode; 
  _DWORD sub = 1; 

	for(retcode = 0; retcode < 32000; retcode ++)
  {
    if(a < sub)
      return(retcode);

    a   = a - sub;
    sub = sub + 2;
  }

  return(retcode); 
} 

_BOOL bmp_size(_UWORD *xx, _UWORD *yy, _UWORD no, _ATTRIB att)
{
  _BOOL retcode = _FALSE;
  _BMP  *pb = ImageList.Get(no);

  if(pb != NULL)
  {
   *xx = pb->width * (1 + GetXSize_Makro(att));
   *yy = pb->height * (1 + GetYSize_Makro(att));
    retcode = _TRUE; 
  }
  else
  {
		*xx = 0;
    *yy = 0; 
  }

  return(retcode); 
}

_BOOL check_superpassword(_UDWORD pass, _UWORD count)
{
  _BOOL  retcode = _FALSE;
  _DATE  datekey, datecur;
  _DWORD daydiff;

  DeEnCode((_UBYTE*)&pass, sizeof(_UDWORD), _FALSE);
  lasal_to_DATE(&datekey, pass);
  datekey.dayofweek = 0;

  get_DATE(&datecur);
  datecur.dayofweek = 0;

  daydiff = DATE_Diff(&datekey, &datecur);

  if((daydiff >= 0)&&(daydiff < count))
    retcode = _TRUE;

  return(retcode);
}

_POSRESULT calc_pos(_POSITION *pos, _UWORD xy, _BOOL hor)
{
  _POSRESULT retcode = P_NOP;

  if((pos->oldpos != pos->pos)||(pos->oldbegin != pos->begin))
  {
    if(pos->pos < 0)
      pos->pos = 0;

    if(pos->begin < 0)
      pos->begin = 0;

    if(pos->pos >= pos->no)
    {
      if(pos->no > 0)
        pos->pos = pos->no - 1;
      else if((pos->oldpos > 0) && (pos->oldpos < pos->no))
        pos->pos = pos->oldpos;
      else
        pos->pos = 0;
    }

    if(pos->pos < pos->begin)
      pos->begin = pos->pos;

    if(pos->pos >= (pos->begin + xy))
      pos->begin = (pos->pos - xy + 1);

    if(pos->no > xy)
    {
      if(pos->begin > (pos->no - xy))
      {
        pos->begin = pos->no - xy;
      }
    }

    if(hor == _FALSE)
    {
      if((pos->oldpos != pos->pos)||(pos->oldbegin != pos->begin))
      {
        retcode = P_CURSOR;

        if(pos->oldbegin != pos->begin)
        {
          retcode = P_REDRAW;

          if(pos->begin == (pos->oldbegin + 1))
            retcode = P_SCROLLUP;
          else if(pos->begin == (pos->oldbegin - 1))
            retcode = P_SCROLLDOWN;
        }
      }
    }
    else if((pos->oldpos != pos->pos)||(pos->oldbegin != pos->begin))
    {
      retcode = P_REDRAW;
    }

    pos->oldpos   = pos->pos;
    pos->oldbegin = pos->begin;

  }

  return(retcode);
}

_POSRESULT calculate_h_position(_POSITION *pos, _EVENT *pe, _UWORD step)
{
  _POSRESULT retcode = P_NOP;

  if((pe != NULL)&&(pos != NULL))
  {
    if(pos->no > 0)
    {
      if(pe->ftype == _EVENT_KEYPRESS)
      {
        switch(pe->scancode)
        {
#ifdef USER_DEMAG
			 case _LEFT   : pos->pos -= 1;
								 break;
			 case _RIGHT  : pos->pos += 1;
								 break;
#else
			 case _LEFT   : pos->pos -= UC_CALCULATE_H_POSITION_STEP;
								 break;
			 case _RIGHT  : pos->pos += UC_CALCULATE_H_POSITION_STEP;
								 break;
#endif
			 case _PGLEFT : pos->pos -= step; //pos->height;
								 break;
			 case _PGRIGHT: pos->pos += step; //pos->height;
								 break;
        }
      }
	 }

    retcode = calc_pos(pos, pos->height, _TRUE);
  }

  return(retcode);
}

_POSRESULT calculate_v_position(_POSITION *pos, _EVENT *pe, _UWORD step)
{
  _POSRESULT retcode = P_NOP;

  if((pe != NULL)&&(pos != NULL))
  {
    if(pos->no > 0)
    {
      if(pe->ftype == _EVENT_KEYPRESS)
      {
        switch(pe->scancode)
        {
          case _UP     : pos->pos --;
                         break;
          case _DOWN   : pos->pos ++;
                         break;
          case _PGUP   : pos->pos   -= step;
                         pos->begin -= step;
                         break;
          case _PGDOWN : // dr856
		                 // alt
		                 //pos->pos += step;
                         //pos->begin += step;
                         // neu
		                 if((pos->pos + step) < pos->no)
		                   pos->pos += step;
                         else
						   pos->pos = pos->no - 1;

                         if(pos->pos >= (pos->begin + pos->height))
                           pos->begin += step;
                         // dr856 ende
                         break;
        }
      }
      else if(pe->ftype == _EVENT_SHUTTLEINC)
      {
        pos->pos += pe->scancode;
      }
      else if(pe->ftype == _EVENT_SHUTTLEDEC)
      {
        pos->pos -= pe->scancode;
      }

      retcode = calc_pos(pos, pos->height, _FALSE);
    }
  }

  return(retcode);
}

_POSRESULT calculate_position(_POSITION *pos, _POSITION *hpos, _EVENT *pe, _ROOM *pr)
{
  _POSRESULT retcode = P_NOP;
  _UWORD     step;

  if(pe != NULL)
  {
    if(pos != NULL)
    {
      retcode = calculate_v_position(pos, pe, pos->height);
    }
    if(retcode == P_NOP)
    {
      step = (pr != NULL)? (pr->xy2.x-pr->xy1.x)/4 : 1;
      retcode = calculate_h_position(hpos, pe, step);
    }
  }

  return(retcode);

/*
  if(pos->no > 0)
  {
    if(pe != NULL)
    {
      if(pe->ftype == _EVENT_KEYPRESS)
      {
        switch(pe->scancode)
        {
          case _UP     : pos->pos --;
                         break;
          case _DOWN   : pos->pos ++;
                         break;
				  case _PGUP   : pos->pos   -= pos->height;
                         pos->begin -= pos->height;
                         break;
          case _PGDOWN : pos->pos   += pos->height;
                         pos->begin += pos->height;
                         break;

          case _LEFT   : if(hpos != NULL) hpos->pos -= 4;
                         break;
          case _RIGHT  : if(hpos != NULL) hpos->pos += 4;
                         break;
          case _PGLEFT : if(hpos != NULL) hpos->pos -= hpos->height;
                         break;
          case _PGRIGHT: if(hpos != NULL) hpos->pos += hpos->height;
                         break;

        }
		}
      else if(pe->ftype == _EVENT_SHUTTLEINC)
      {
        pos->pos += pe->scancode;
      }
      else if(pe->ftype == _EVENT_SHUTTLEDEC)
      {
        pos->pos -= pe->scancode;
      }
    }

    retcode = calc_pos(pos, pos->height, _FALSE);

    if((hpos != NULL) && (retcode == P_NOP))
      retcode = calc_pos(hpos, hpos->height, _TRUE);

  }
  else
  {
    init_POSITION(pos);
  }
  return(retcode);
*/
}

/*
void calculate_hposition(_POSITION *pos, _EVENT *pe)
{
  _POSRESULT retcode = P_NOP;


  if(pos->no > 0)
  {
    if(pe != NULL)
    {
      if(pe->ftype == _EVENT_KEYPRESS)
      {
        switch(pe->scancode)
        {
          case _LEFT   : pos->pos --;
                         break;
          case _RIGHT  : pos->pos ++;
                         break;
          case _PGLEFT : pos->pos -= pos->width;
                         break;
          case _PGRIGHT: pos->pos += pos->width;
                         break;
        }
      }

    }

    if((pos->oldpos != pos->pos)||(pos->oldbegin != pos->begin))
    {
      if(pos->pos < 0) 
		  pos->pos = 0;

      if(pos->begin < 0)
        pos->begin = 0; 

      if(pos->pos >= pos->no) 
        pos->pos = pos->no - 1; 

      if(pos->pos < pos->begin)
        pos->begin = pos->pos; 

      if(pos->pos >= (pos->begin + pos->width))
        pos->begin = (pos->pos - pos->width + 1); 

      if(pos->no > pos->width)
      {
        if(pos->begin > (pos->no - pos->width))
        {
          pos->begin = pos->no - pos->width; 
        }
      }

      if((pos->oldpos != pos->pos)||(pos->oldbegin != pos->begin))
      {
        retcode = P_CURSOR; 

        if(pos->oldbegin != pos->begin)
        {
          retcode = P_REDRAW; 

          if(pos->begin == (pos->oldbegin + 1))
            retcode = P_SCROLLUP; 
          else if(pos->begin == (pos->oldbegin - 1))
            retcode = P_SCROLLDOWN; 
        }
      }

      pos->oldpos   = pos->pos;
			pos->oldbegin = pos->begin;
    }
  }
  else
  {
    init_POSITION(pos); 
  }

//  return(retcode); 
}
*/




_KOORD linear_distance(_RESULT *v, _KOORD height)
{
  _DWORD max, min, way, val;

  max = v->rmax;
  min = v->rmin;
  val = v->value;

  if(val <= min)
    return(0);
  else if(val >= max)
    return(height);

  if((max > 200000000L)||(min < -200000000L))
	{
    max = max / 10;
    min = min / 10;
    val = val / 10;
  }

  way = max - min;
  val = val - min;

  return(((val*height)/way));
}

_WORD calculate_bar(_RESULT *v, _WORD height)
{
	_WORD  retcode = 0;
	_DWORD min, max, value;

	min = v->rmin;
	max = v->rmax;
  value = v->value;

  if(value <= min)
    retcode = 0;
  else if(retcode >= max)
    retcode = height;

  else if(min != max)
  {

	  if((max > 200000000L)||(min < -200000000L))
	  {
		  value /= 1000;
		  min   /= 1000;
		  max   /= 1000;
	  }

	  retcode = ((value - min) * height) / (absolut(max - min));
  }


  return(retcode);
}

void *get_ANYTHING(_ANYTHING *p, _TextPuffer *LT)
{
  void       *retcode = NULL;
  _SCHEME    *pts;
  _TXTSCHEME *ptt;

  switch(p->list)
  {
    case IMAGESCHEME  : if(ImageScheme.Get(&pts, p->no) == _TRUE)
                          retcode = (void*)pts;
                        break;
    case OBJECTSCHEME : if(ObjectScheme.Get(&pts, p->no) == _TRUE)
                          retcode = (void*)pts;
                        break;
    case COLORSCHEME  : if(ColorScheme.Get(&pts, p->no) == _TRUE)
                          retcode = (void*)pts;
                        break;
    case STATESCHEME  : if(StateScheme.Get(&pts, p->no) == _TRUE)
                          retcode = (void*)pts;
                        break;
    case TEXTSCHEME   : if(TextScheme.Get(&ptt, p->no) == _TRUE)
                          retcode = (void*)ptt;
                        break;
    case ALARMLIST    : retcode = Alarm.Get_SINGLE_ALARM(p->no); break;
    case ICONLST      : retcode = ImageList.Get(p->no);   break;
    case OBJLST       : retcode = ObjectList.Get(p->no);  break;
    default           : retcode = (void*)get_text(p, LT, NULL, _TRUE); break;
  }

  return(retcode);
}

void out_ANYTHING(_ANYTHING *p, _ROOM *pr, _FONT font, _ATTRIB att, _FRAMETYPE frametype, _COLOR coltxt, _COLOR colback, _COLOR colframe, _TextPuffer* LocalText)
{
  _CHAR       *txt = NULL;
  _BMP        *bmp = NULL;
  _SubPicture *obj = NULL;
//  _UWORD      xx, yy;
//  _KOORD      x, y;

  switch(p->list)
	{
    case ICONLST : bmp = (_BMP*)get_ANYTHING(p, NULL);
                   break;
    case OBJLST  : obj = (_SubPicture*)get_ANYTHING(p, NULL);
                   break;
    default      : txt = (_CHAR*)get_ANYTHING(p, LocalText);
                   break;
  }

  if(txt != NULL)         // text
  {
    OutMultiTextAlign(txt, 0, pr, font, att, frametype, coltxt, colback, colframe, sizeof(_CHAR));
  }
  else if(bmp != NULL)    // bitmap
  {
    if((pr->xy2.x == DEFKOORD)&&(pr->xy2.y == DEFKOORD))
      Bitmap(pr->xy1.x, pr->xy1.y, bmp, att & (~T_SIZE), coltxt);
    else
      out_zoom_BMP(pr, bmp, att & (~T_SIZE));

//  DrawButton(pr, att, 0, colback, colframe);
//  bmp_size(&xx, &yy, p->no, att);
//  if(xx != 0)
//  {
//    switch(att & T_HORIZBOUND)
//    {
//      case T_RIGHTBOUND  : x = pr->xy2.x - xx;                 break;
//      case T_CENTERBOUND : x = (pr->xy1.x + pr->xy2.x - xx)/2; break;
//      default            : x = pr->xy1.x;                      break;
//    }
//    switch(att & T_VERTBOUND)
//    {
//      case T_DOWNBOUND   : y = pr->xy2.y - yy;                 break;
//      case T_MIDBOUND    : y = (pr->xy1.y + pr->xy2.y - yy)/2; break;
//      default            : y = pr->xy1.y;                      break;
//    }

//    Bitmap(x, y, bmp, att & (~T_SIZE), coltxt);
//  }
  }
  else if(obj != NULL)    // object
  {
//    Position Links Oben ermitteln (xy)
//    Screen.Object(xy.x, xy.y, obj, att, col);

    _ZOOM zoom;
		init_ZOOM(&zoom);
    obj->Draw(&pr->xy1, &zoom, NULL);
  }
}

_DOT get_left_top(_DOT src, _WORD xx, _WORD yy, _ATTRIB attrib)
{
  _DOT retcode;

  switch(attrib & T_HORIZBOUND)
  {
    case T_CENTERBOUND : retcode.x = src.x - xx / 2; break;
    case T_RIGHTBOUND  : retcode.x = src.x - xx;     break;
    default            : retcode.x = src.x;          break;
  }

  switch(attrib & T_VERTBOUND)
  {
    case T_MIDBOUND    : retcode.y = src.y - yy / 2; break;
    case T_DOWNBOUND   : retcode.y = src.y - yy;     break;
    default            : retcode.y = src.y;          break;
  }

  return(retcode);
}

void SystemError(_SYSERROR err, _DWORD op1, _CHAR *txt)
{
//  if(Root.SystemError(err, op1, txt) == _FALSE)
//    Root.AddCmd(CMD_CLOSE, 0, 0);
  Root.SystemError(err, op1, txt);
}

void DelComment(void *txt, _UWORD chr_size)
{
  _UBYTE *at;
  _UNI   *ut; 
  _UWORD i=0;
  _UWORD len = StrLen(txt, chr_size); 

  if(chr_size == 2)
  {
    ut = (_UNI*)txt;
    while(i < len)
    {
      if(ut[i] == ';')
      {
        ut[i] = 0; 
        i = len; 
      }

			i ++;
    }
  }
	else
	{
		at = (_UBYTE*)txt;
		while(i < len)
		{
			if(at[i] == ';')
			{
				at[i] = 0;
				i = len;
			}

			i ++;
		}
	}
}

void DelSpace(void *txt, _UWORD chr_size)
{
	_UWORD len = StrLen(txt, chr_size);
	_UWORD size;
  _UNI   uni, *pu;
  _UBYTE ascii, *pa;
  _UWORD hk = 1;

	if(chr_size == 2)
	{
		pu   = (_UNI*)txt;
		size = sizeof(_UNI);

		while(len > 0)
		{
			len --;
      uni = pu[len];
      if(uni == 34)
        hk = hk ^ 1;

			if(((uni == 32)||(uni == 9)) && (hk != 0))
			{
				MemMove(&pu[len], &pu[len+1], size);
			}
			else
			{
				size += sizeof(_UWORD);
			}
		}
	}
	else
	{
    pa   = (_UBYTE*)txt;
		size = sizeof(_UBYTE);

		while(len > 0)
		{
			len --;
      ascii = pa[len];
      if(ascii == 34)
        hk = hk ^ 1;

			if(((ascii == 32)||(ascii == 9)) && (hk != 0))
			{
				MemMove(&pa[len], &pa[len+1], size);
			}
			else
			{
				size += sizeof(_UBYTE);
			}
		}
	}
}

void DelChr(void *txt, _UWORD chr, _UWORD chr_size)
{
	_UWORD len = StrLen(txt, chr_size);
	_UWORD size;

	if(chr_size == 2)
	{
		_UWORD *pu = (_UWORD*)txt;

		size = sizeof(_UWORD);

		while(len > 0)
		{
			len --;
			if(pu[len] == chr)
			{
				MemMove(&pu[len], &pu[len+1], size);
			}
			else
			{
				size += sizeof(_UWORD);
			}
		}
	}
	else
	{
		_UBYTE *pa = (_UBYTE*)txt;

		size = sizeof(_UBYTE);

		while(len > 0)
		{
			len --;
			if(pa[len] == chr)
			{
				MemMove(&pa[len], &pa[len+1], size);
			}
			else
			{
				size += sizeof(_UBYTE);
			}
		}
	}
}

_BOOL is_invisible(_COLOR color, _ATTRIB attrib)
{
	if((attrib & T_PATTERN) == T_SOLID)
	{
		if(GetPenColor_Makro(color) == INVISIBLE)
		{
			return _TRUE;
		}
	}
	else if(color == MakeColor_Makro(INVISIBLE, INVISIBLE))
	{
		return _TRUE;
	}

	return _FALSE;
}

void FrameInlet(_ROOM *d, _ROOM *s, _ATTRIB att)
{
	_UWORD thick;

  *d = *s;
	if(att & T_OUTLINE)
  {
    thick = GetXSize_Makro(att);
    if(thick == 0)
      thick = 1;
/*
    offset   = thick / 2;
    d->xy1.x = d->xy1.x - offset + thick;
    d->xy1.y = d->xy1.y - offset + thick;
		d->xy2.x = d->xy2.x + 1 + offset - thick;
    d->xy2.y = d->xy2.y + 1 + offset - thick;
*/

    d->xy1.x = d->xy1.x + thick;
    d->xy1.y = d->xy1.y + thick;
		d->xy2.x = d->xy2.x - thick;
    d->xy2.y = d->xy2.y - thick;

  }
}

void CreateString(_CHAR *str, void *txt, _UWORD len, _UNI fillchr, _ROOM *room, _ATTRIB att, _UWORD chr_size, _UDWORD maxstr) //, _FONT font)
{
  _UWORD i, pos, fillno;
  _UWORD roomwidth, txtwidth;

  if((fillchr == 0)||(fillchr == 32))
  {
    StrCpy(str, chr_size, txt, chr_size);
  }
  else
  {
    if(len == 0)
    {
      len = StrLen(txt, chr_size);
    }
    
    if(len >= maxstr)
    {
      len = maxstr - 1;
    }
    
//fontattrib    SetFontAttribute(att);

    roomwidth = room->xy2.x - room->xy1.x + 1;
    txtwidth  = TxtWidth(txt, len, chr_size);

    if(roomwidth <= txtwidth)
    {
      StrCpy(str, chr_size, txt, chr_size);
    }
    else
    {
      fillno = (roomwidth - txtwidth) / ChrWidth((_CHAR)fillchr, chr_size);

      if((fillno + len) >= maxstr)
      {
        fillno = maxstr - len - 1;
      }

      switch(att & T_HORIZBOUND)
      {
        case T_LEFTBOUND:  MemCpy(str, txt, len * chr_size);
                           pos = len;
                           for(i=0; i<fillno; i++, pos++)
                             str[pos] = fillchr;
                           str[pos] = 0;
                           break;

        case T_RIGHTBOUND: for(i=0; i<fillno; i++)
                             str[i] = fillchr;
                           MemCpy(&str[i], txt, len * chr_size);
                           str[i+len] = 0;
                           break;

        default:           fillno /= 2;
                           for(i=0; i<fillno; i++)
                             str[i] = fillchr;
                           MemCpy(&str[i], txt, len * chr_size);
                           pos = i + len;
                           for(i=0; i<fillno; i++, pos++)
                             str[pos] = fillchr;
                           str[pos] = 0;
                           break;
      }
    }
  }
}

void OutTextAlign(void *txt, _UNI fillchr, _ROOM *pr, _FONT font, _ATTRIB att, _FRAMETYPE frametype, _COLOR coltxt, _COLOR colback, _COLOR colframe, _UWORD chr_size)
{
  _REGION actreg=NULL, ioreg=NULL, newreg=NULL;
  _KOORD  x, y;
  _ROOM   room;
  _CHAR   str[300];

  GetTextRoom(&room, pr, att, frametype, _TRUE);

 #ifdef UC_MOUSE
  _UWORD hide = IfNecHideMouseCursor(pr);
 #endif

  DrawButton(pr, att, frametype, colback, colframe);

  if(frametype != _DEFFRAME_NOCAREREGION)
    actreg = ActivateAndRegion(&ioreg, &newreg, pr); //&room);

  Font.Set(font, &att);
  coltxt = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(coltxt));

  switch(att & T_VERTBOUND)
  {
    case T_UPBOUND    : y = room.xy1.y; break;
    case T_DOWNBOUND  : y = room.xy2.y; break;
    default           : y = (room.xy1.y + room.xy2.y)/2; break;
  }

  switch(att & T_HORIZBOUND)
  {
    case T_LEFTBOUND  : x = room.xy1.x; break;
    case T_RIGHTBOUND : x = room.xy2.x; break;
    default           : x = (room.xy1.x + room.xy2.x)/2; break;
  }

//coltxt = MakeColor_Makro(LIGHTGREEN, GetPenColor_Makro(coltxt)); // dillo
  if((fillchr != 0)&&(fillchr != 32))
  {
    CreateString(str, txt, 0, fillchr, &room, att, chr_size, ARRAY_SIZE(str)); //, font);
    OutTextLse(x, y, str, T_SOLID|(att&(~T_PATTERN)), coltxt, chr_size);
  }
  else
  {
    OutTextLse(x, y, txt, T_SOLID|(att&(~T_PATTERN)), coltxt, chr_size);
  }

 #ifdef UC_MOUSE
  IfNecShowMouseCursor(hide);
 #endif

  if(frametype != _DEFFRAME_NOCAREREGION)
    DeactivateAndRegion(actreg, &ioreg, &newreg);
}

void OutTextIconAlign(void *txt, _UNI fillchr, _ROOM *pr, _FONT font, _ATTRIB att, _FRAMETYPE frametype, _COLOR coltxt, _COLOR colback, _COLOR colframe, _UWORD chr_size)
{
	_REGION actreg, ioreg=NULL, newreg=NULL;
	_KOORD  x, y;
  _ROOM   room;

  GetTextRoom(&room, pr, att, frametype, _TRUE);

 #ifdef UC_MOUSE
	_UWORD hide = IfNecHideMouseCursor(pr);
 #endif

  DrawButton(pr, att, frametype, colback, colframe);
	actreg = ActivateAndRegion(&ioreg, &newreg, pr); //&room);

	Font.Set(font, &att);
  coltxt = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(coltxt));

	switch(att & T_VERTBOUND)
	{
		case T_UPBOUND    : y = room.xy1.y; break;
		case T_DOWNBOUND  : y = room.xy2.y; break;
		default           : y = (room.xy1.y + room.xy2.y)/2; break;
	}

	switch(att & T_HORIZBOUND)
	{
		case T_LEFTBOUND  : x = room.xy1.x; break;
		case T_RIGHTBOUND : x = room.xy2.x; break;
		default           : x = (room.xy1.x + room.xy2.x)/2; break;
	}

  OutTextIconXY(x, y, txt, T_SOLID|(att&(~T_PATTERN)), coltxt, chr_size);

 #ifdef UC_MOUSE
	IfNecShowMouseCursor(hide);
 #endif
	DeactivateAndRegion(actreg, &ioreg, &newreg);

}

_UWORD count_pipe(_UWORD *pos, _UWORD *pno, void *txt, _UWORD chr_size)
{
  _UWORD len = StrLen(txt, chr_size);
  _UWORD i, no;
  _UNI   *pu;
  _UBYTE *pa;

  no = 0;

  if(len > 0)
  {
	 if(chr_size == 2)
	 {
		pu = (_UNI*)txt;
		for(i=0; i<len; i++)
		{
		  if(*pu == 124)
		  {
			 pos[no] = i;
			 no++;
		  }
		  pu++;
		}
	 }
	 else
	 {
		pa = (_UBYTE*)txt;
		for(i=0; i<len; i++)
		{
		  if(*pa == 124)
		  {
			 pos[no] = i;
			 no++;
		  }
		  pa++;
		}
	 }
  }

  pos[no] = len;
  no ++;

  *pno = no;

  return(len);
}

void OutMultiTextIconXY(_KOORD x, _KOORD y, void *txt, _ATTRIB att, _COLOR col, _UWORD chr_size, _FONT font)
{
  _UWORD off, len, i, no;
  _UBYTE ttx[400];
  _UWORD xx, yy, pos[30];
  _UBYTE *pas, *pad;
  _UNI   *pus, *pud;

  no = StringCountChr(txt, chr_size, 124);
  Font.Set(font, &att);
  if(no == 0)
  {
    OutTextIconXY(x, y, txt, att, col, chr_size);
  }
  else
  {
    Font.Size(font, &xx, &yy);
    y  -= (no * yy) / 2;
    no  = 0;
    off = 0;
    count_pipe(pos, &no, txt, chr_size);

    if(chr_size == 1)
    {
      pad = ttx;
      pas = (_UBYTE*)txt;
      for(i=0; i<no; i++)
      {
        len  = pos[i] - off;
        off += len + 1;
        MemCpy(pad, pas, len);
        pad[len] = 0;
        pas += len; // text überspringen
        pas += 1;   // pipe überspringen

        // auf pad steht der string zur ausgabe
        OutTextIconXY(x, y, pad, att, col, 1);
        y += yy;
      }
    }
    else
    {
      pud = (_UNI*)ttx;
      pus = (_UNI*)txt;
      for(i=0; i<no; i++)
      {
        len  = pos[i] - off;
        off += len + 1;
        MemCpy(pud, pus, len*2);
        pud[len] = 0;
        pus += len; // text überspringen
        pus += 1;   // pipe überspringen

        // auf pad steht der string zur ausgabe
        OutTextIconXY(x, y, pud, att, col, 2);
        y += yy;
      }
	  }
  }
}

#ifdef SA54151 //UC_MENU_USEWORDWRAP
static _CHAR* WordWrapCopyIcon(_CHAR *pu, _UWORD chr_size, _UDWORD xx)
{
  _CHAR   *retcode = (_CHAR*)get_temptext();
  _CHAR   *pwrap   = NULL;
  _UDWORD width   = 0;

  StrCpy(retcode, sizeof(_CHAR), pu, chr_size);
  pu = retcode;

  while(*pu != 0)
  {
    if((*pu == '%') && ((pu[1] == 'B')||(pu[1] == 'b')||(pu[1] == 'I')||(pu[1] == 'i')))
    {
      _UWORD no, anz = 0;
      if(pu[2] == 91)
      {
        no = make_bmpno(&pu[3], sizeof(_CHAR), &anz);
        anz += 1;
      }
      else
      {
        no = AToDsub(&pu[2], sizeof(_CHAR), &anz);
      }

      _BMP *bmp = ImageList.Get(no);
      if(bmp != NULL)
      {
        width += bmp->width;
      }
      
      pu += anz;
    }
    else 
    {
      width += ChrWidth(*pu, sizeof(_CHAR));
      if(*pu == ' ')
      {
        pwrap = pu;
      }
      else if(*pu == 124)
      {
        pwrap = NULL;
        width = 0;
      }
    }
    
    if(width > xx)
    {
      if(pwrap != NULL)
      {
        *pwrap = '|';
        pu     = pwrap;
        width  = 0;
        pwrap  = NULL;
      }
    }
    
    pu++;
  }
  
  return retcode;
}
#endif

void OutMultiTextIconAlign(void *txt, _UNI fillchr, _ROOM *pr, _FONT font, _ATTRIB att, _FRAMETYPE frametype, _COLOR coltxt, _COLOR colback, _COLOR colframe, _UWORD chr_size)
{
  _REGION actreg, ioreg=NULL, newreg=NULL;
  _ROOM   room;
  _KOORD  x, y;
  _UWORD  no, xx, yy;

  GetTextRoom(&room, pr, att, frametype, _TRUE);
  coltxt = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(coltxt));

 #ifdef UC_MOUSE
   _UWORD hide = IfNecHideMouseCursor(pr);
 #endif

  DrawButton(pr, att, frametype, colback, colframe);
  actreg = ActivateAndRegion(&ioreg, &newreg, pr); //&room);

  Font.Set(font, &att);
  Font.Size(font, &xx, &yy);
  
 #ifdef SA54151 //UC_MENU_USEWORDWRAP // sa54151, DEVVIS-23
//  if((chr_size == 2) && (SetUp.GetWordWrap() == _TRUE))
  if(SetUp.GetWordWrap() == _TRUE)
  {
    txt = WordWrapCopyIcon((_CHAR*)txt, chr_size, room.xy2.x-room.xy1.x+1);
    chr_size = sizeof(_CHAR);
  }
 #endif
 
  no = StringCountChr(txt, chr_size, 124) + 1;

  switch(att & T_VERTBOUND)
  {
    case T_UPBOUND    : y = room.xy1.y + (no * yy) / 2;    break;
    case T_DOWNBOUND  : y = room.xy2.y + 1 - (no * yy)/2;  break; // SA43162
    default           : y = (room.xy1.y + room.xy2.y) / 2; break;
  }

  switch(att & T_HORIZBOUND)
  {
    case T_LEFTBOUND  : x = room.xy1.x; break;
    case T_RIGHTBOUND : x = room.xy2.x; break;
    default           : x = (room.xy1.x + room.xy2.x)/2; break;
  }

  att = (att & (~T_VERTBOUND))|T_MIDBOUND;

  OutMultiTextIconXY(x, y, txt, att, coltxt, chr_size, font);

 #ifdef UC_MOUSE
  IfNecShowMouseCursor(hide);
 #endif
  DeactivateAndRegion(actreg, &ioreg, &newreg);
}


#ifdef UC_WORDWRAP
void do_multiline_ascii(_ASCII *txt, _UWORD xx)
{
  _UWORD width;
  _UBYTE chr, *space, *pa;

  space = NULL;
  width = 0;
  pa    = (_UBYTE*)txt;

  for(;;)
  {
    chr = *pa;
    if(chr == 0)
    {
      return;
    }
    else if(chr == 32)
    {
      space = pa;
    }
    else if(chr == 124)
    {
      space = NULL;
      width = 0;
      pa   += 1;
      continue;
    }

    width += ChrWidth(chr, sizeof(_ASCII));
    if(width > xx)
    {
      if(space != NULL)
      {
        *space = 124;
        pa     = space;
        space  = 0;
        width  = 0;
      }
    }

    pa += 1;
  }
}

void do_multiline_uni(_UNI *txt, _UWORD xx)
{
  _UWORD width;
  _UNI   chr, *space, *pu;

  space = NULL;
  width = 0;
  pu    = txt;

  for(;;)
  {
    chr = *pu;
    if(chr == 0)
    {
      return;
    }
    else if(chr == 32)
    {
      space = pu;
    }
    else if(chr == 124)
    {
      space = NULL;
      width = 0;
      pu   += 1;
      continue;
    }

    width += ChrWidthLse(chr, sizeof(_UNI));
    if(width > xx)
    {
      if(space != NULL)
      {
        *space = 124;
        pu     = space;
        space  = 0;
        width  = 0;
      }
    }

    pu += 1;
  }
}

void do_multiline(void *txt, _UWORD xx, _UWORD chr_width)
{
  if(chr_width == sizeof(_ASCII))
    do_multiline_ascii((_ASCII*)txt, xx);
  else
    do_multiline_uni((_UNI*)txt, xx);
}

#ifndef SA55048
static _CHAR wwtxt[2000];
#endif

#endif

//static _CHAR strline[1024];
void OutMultiTextAlign(void *txt, _UNI fillchr, _ROOM *pr, _FONT font, _ATTRIB att, _FRAMETYPE frametype, _COLOR coltxt, _COLOR colback, _COLOR colframe, _UWORD chr_size)
{
  _UWORD  i, no;
  _UWORD  pos[50];
  _UBYTE  *pa, *ph;
  _UNI    *pu;
  _UWORD  xx, yy;
  _KOORD  x, y;
  _UWORD  len, index;
  _REGION actreg=NULL, ioreg=NULL, newreg=NULL;
  _ROOM   room;
  _ATTRIB attold;

 #ifdef UC_WORDWRAP
  _UWORD  str_width;

  if(SetUp.GetWordWrap() == _TRUE)
  {

    if(txt != NULL)
    {
      GetTextRoom(&room, pr, att, frametype, _TRUE);
      xx = (room.xy2.x - room.xy1.x + 1);
      no = StringCountChr(txt, chr_size, 124);
      Font.Set(font, &att);
      // folgende zeile ist notwendig, ansonsten funktioniert funktion StrWidth nicht richtig
      OutTextXY(room.xy1.x, room.xy1.y, " ", att, MakeColor_Makro(INVISIBLE, BLACK), 1);

#ifdef SA55048      
      _CHAR *wwtxt = get_temptext();
#endif        
      if(no == 0) // definierte singleline (vertical line)
      {
        str_width = StrWidthLse(txt, chr_size);
        if(str_width > xx)
        {
          // string auf multiline aufteilen
          StrCpy(wwtxt, chr_size, txt, chr_size);
          txt = wwtxt;
          do_multiline(txt, xx, chr_size);
        }
      }
      else // definierte multiline (vertical line)
      {
        StrCpy(wwtxt, chr_size, txt, chr_size);
        txt = wwtxt;
        do_multiline(txt, xx, chr_size);
      }

      StrCpy(wwtxt, chr_size, txt, chr_size);
    }
  }
 #endif

  no = StringCountChr(txt, chr_size, 124);

  if(no == 0)
  {
    OutTextAlign(txt, fillchr, pr, font, att, frametype, coltxt, colback, colframe, chr_size);
  }
  else
  {
    no = 0;
    count_pipe(pos, &no, txt, chr_size);
    GetTextRoom(&room, pr, att, frametype, _TRUE);
    coltxt = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(coltxt));
    Font.Set(font, &att);
    Font.Size(font, &xx, &yy);

   #ifdef UC_MOUSE
    _UWORD hide = IfNecHideMouseCursor(pr);
   #endif

    DrawButton(pr, att, frametype, colback, colframe);

    if(frametype != _DEFFRAME_NOCAREREGION)
      actreg = ActivateAndRegion(&ioreg, &newreg, pr); //&room);

    switch(att & T_VERTBOUND)
    {
      case T_UPBOUND    : y = room.xy1.y;             break;
      case T_DOWNBOUND  : y = room.xy2.y - (no * yy) + 1; break;
      default           : y = (room.xy1.y + room.xy2.y - (no * yy))/2; break;
    }

    switch(att & T_HORIZBOUND)
    {
      case T_LEFTBOUND  : x = room.xy1.x; break;
      case T_RIGHTBOUND : x = room.xy2.x; break;
      default           : x = (room.xy1.x + room.xy2.x)/2; break;
    }

    attold = att;
    att = (att & (~T_VERTBOUND))|T_UPBOUND;

    _CHAR *pstrline = get_temptext();

    if(chr_size == 2)
    {
      index = 0;
      pu    = (_UNI*)txt;
      for(i=0; i<no; i++)
      {
        len   = pos[i] - index;
        index = pos[i] + 1;
        if((fillchr != 0)&&(fillchr != 32))
        {
          CreateString(pstrline, pu, len, fillchr, &room, attold, chr_size, MAX_TEMPTEXT_LEN); //, font);
          OutTextLse(x, y, pstrline, T_SOLID|(att&(~T_PATTERN)), coltxt, chr_size);
        }
        else
        {
          MemCpy(pstrline, pu, len * sizeof(_UNI));
          pstrline[len] = 0;
          OutTextLse(x, y, pstrline, att, coltxt, sizeof(_UNI));
// 			    OutTextLen(x, y, pu, len, att, coltxt, 2);  //Metagraphics slurry
        }

        y  += yy;
        pu += (len + 1);
      }
    }
    else
    {
      index = 0;
      pa    = (_UBYTE*)txt;
      for(i=0; i<no; i++)
      {
        len   = pos[i] - index;
		    index = pos[i] + 1;
        if((fillchr != 0)&&(fillchr != 32))
        {
          CreateString(pstrline, pa, len, fillchr, &room, attold, chr_size, MAX_TEMPTEXT_LEN); //, font);
          OutTextLse(x, y, pstrline, T_SOLID|(att&(~T_PATTERN)), coltxt, chr_size);
        }
        else
        {
          ph = (_UBYTE*)pstrline;
          MemCpy(ph, pa, len);
          ph[len] = 0;
          OutTextLse(x, y, ph, att, coltxt, sizeof(_ASCII));
//          OutTextLen(x, y, pa, len, att, coltxt, 1); // ansonsten keine render engine (3d-effekt)
        }

        y  += yy;
        pa += (len + 1);
      }
    }

   #ifdef UC_MOUSE
    IfNecShowMouseCursor(hide);
   #endif

    if(frametype != _DEFFRAME_NOCAREREGION)
      DeactivateAndRegion(actreg, &ioreg, &newreg);
  }
}

_BOOL SizeMultiTextAlign(_UWORD *pwidth, _UWORD *pheight, void *txt, _FONT font, _ATTRIB attrib, _UWORD chr_size)
{
	_UBYTE *pa, *pas, ascii;
	_UNI   *pu, *pus, uni;
	_UWORD pos[30];
	_UWORD i, no;
	_UWORD index;
  _UWORD xx, width, height;
  _BOOL  retcode = _FALSE;

  width  = 0;
  height = 0;

  if(count_pipe(pos, &no, txt, chr_size) != 0)
	{
    Font.Set(font, NULL);
//fontattrib    SetFontAttribute(attrib);
    Font.Size(font, &width, &height);
    height = no * height;

    if(chr_size == sizeof(_UNI))
    {
      pus = (_UNI*)txt;
      pu  = pus;
      for(i=0; i<no; i++)
      {
		  index      = pos[i];
        uni        = pus[index];
        pus[index] = 0;
        xx = StrWidthLse(pu, sizeof(_UNI));
        pus[index] = uni;
        pu         = &pus[index+1];
        if(xx > width)
          width = xx;
      }
    }
    else
    {
      width = 0;
      pas = (_UBYTE*)txt;
      pa  = pas;
      for(i=0; i<no; i++)
      {
        index      = pos[i];
        ascii      = pas[index];
        pas[index] = 0;
        xx = StrWidth(pa, sizeof(_ASCII));
        pas[index] = ascii;
        pa         = &pas[index+1];
        if(xx > width)
          width = xx;
      }
    }

    retcode = _TRUE;
  }

  *pwidth  = width;
  *pheight = height;

  return(retcode);
}

_KOORD Zoom(_WORD z, _KOORD xy)
{
  _DWORD tmp;

  if(z != DEFZOOM)
  {
    tmp = (_DWORD)xy * (_DWORD)z;
    if(tmp > 0)
      tmp += 500;
    else
      tmp -= 500;

    xy = (_KOORD)(tmp/1000);
  }

  return(xy);
}

_WORD Zoom_ZOOM(_DWORD a, _DWORD b)
{
  _DWORD tmp;

  if(a == DEFZOOM)
    a = 1000;
  if(b == DEFZOOM)
    b = 1000;

  tmp = (_DWORD)a * (_DWORD)b;
  if(tmp > 0)
    tmp += 500;
  else
    tmp -= 500;

  return((_WORD)(tmp/1000));
}

void Make_ZOOM(_ZOOM *pz, _SubPicture *ps, _ROOM *rsoll)
{
 #ifdef UC_ZOOM_OBJSCHEME
  _UWORD xx, yy, xxx, yyy;
  _ROOM  room;

  if((pz != NULL)&&(rsoll != NULL)&&(ps != NULL))
  {
    ps->GetSize(&room);

    xx  = rsoll->xy2.x - rsoll->xy1.x + 1;
    yy  = rsoll->xy2.y - rsoll->xy1.y + 1;
    xxx = room.xy2.x  - room.xy1.x + 1;
    yyy = room.xy2.y  - room.xy1.y + 1;

    pz->x = (xx * 1000 + (xxx / 2)) / xxx;
    pz->y = (yy * 1000 + (yyy / 2)) / yyy;
  }
  else if(pz != NULL)
  {
    init_ZOOM(pz);
  }
 #else
  pz->x = 1000;
  pz->y = 1000;
 #endif
}

void Zoom_ROOM(_ROOM *pr, _DOT *offset, _ZOOM *zoom)
{
 #ifdef SA51056
 
  float xfact = ((float)zoom->x) / 1000.0;
  float yfact = ((float)zoom->y) / 1000.0;

  if((xfact != 1.0) && (zoom->x != DEFZOOM))  
  {
    float fx = pr->xy1.x * xfact;
    float width = pr->xy2.x - pr->xy1.x + 1;
    pr->xy1.x = (_KOORD)(fx + ((fx >= 0)? 0.5 : -0.5));
    fx = pr->xy1.x + (width * xfact);
    pr->xy2.x = (_KOORD)(fx + ((fx >= 0)? 0.5 : -0.5)) - 1;
  }

  if((yfact != 1.0) && (zoom->y != DEFZOOM))
  {
    float fy     = pr->xy1.y * yfact;
    float height = pr->xy2.y - pr->xy1.y + 1;
    pr->xy1.y    = (_KOORD)(fy + ((fy >= 0)? 0.5 : -0.5));
    fy           = pr->xy1.y + (height * yfact);
    pr->xy2.y    = (_KOORD)(fy + ((fy >= 0)? 0.5 : -0.5)) - 1;
  }

  pr->xy1.x = pr->xy1.x + offset->x;
  pr->xy2.x = pr->xy2.x + offset->x;
  pr->xy1.y = pr->xy1.y + offset->y;
  pr->xy2.y = pr->xy2.y + offset->y;

 #else
  _DWORD tmp;

  if(zoom->x != DEFZOOM)
  {
	  tmp = (_DWORD)pr->xy1.x *(_DWORD)zoom->x;
	  tmp = tmp + ((tmp > 0)? 500 : -500);
	  pr->xy1.x = ((_KOORD)(tmp / 1000)) + offset->x;

	  tmp = (_DWORD)pr->xy2.x *(_DWORD)zoom->x;
	  tmp = tmp + ((tmp > 0)? 500 : -500);
	  pr->xy2.x = ((_KOORD)(tmp / 1000)) + offset->x;
  }
  else
  {
	  pr->xy1.x = pr->xy1.x + offset->x;
	  pr->xy2.x = pr->xy2.x + offset->x;
  }

  if(zoom->y != DEFZOOM)
  {
    tmp = (_DWORD)pr->xy1.y *(_DWORD)zoom->y;
    tmp = tmp + ((tmp > 0)? 500 : -500);
    pr->xy1.y = ((_KOORD)(tmp / 1000)) + offset->y;

    tmp = (_DWORD)pr->xy2.y *(_DWORD)zoom->y;
    tmp = tmp + ((tmp > 0)? 500 : -500);
    pr->xy2.y = ((_KOORD)(tmp / 1000)) + offset->y;
  }
  else
  {
	 pr->xy1.y = pr->xy1.y + offset->y;
	 pr->xy2.y = pr->xy2.y + offset->y;
  }
 #endif 
/*
  pr->xy1.x = Zoom(zoom->x, pr->xy1.x) + offset->x;
  pr->xy2.x = Zoom(zoom->x, pr->xy2.x) + offset->x;
  pr->xy1.y = Zoom(zoom->y, pr->xy1.y) + offset->y;
  pr->xy2.y = Zoom(zoom->y, pr->xy2.y) + offset->y;
*/
}

_DWORD absolut(_DWORD val)
{
  if(val < 0)
	 val = -val;

  return(val);
}

_COLOR GetBackColor(_COLOR co)            { return((co>>16)&0xFFFF); }
_COLOR GetPenColor(_COLOR co)                   { return(co&0xFFFF); }
_COLOR MakeColor(_COLOR b, _COLOR f)   { return((b<<16)|(f&0xFFFF)); }
_COLOR InvertColor(_COLOR c)              { return((c<<16)|(c>>16)); }
_UWORD GetXSize(_ATTRIB att)           { return((att>>28)&(0x000F)); }
_UWORD GetYSize(_ATTRIB att)           { return((att>>24)&(0x000F)); }
_UWORD GetPattern(_ATTRIB att)            { return((att>>8)&0x001F); }
_ATTRIB DelPattern(_ATTRIB att)
{
	att &= 0xffffe0ff;
	att |= T_SOLID;

	return(att);
}

_ATTRIB SetXSize(_ATTRIB att, _UWORD size)
{
  att &= 0x0fffffff;
  att |= size << 28;
  return(att);
}
_ATTRIB SetYSize(_ATTRIB att, _UWORD size)
{
  att &= 0xf0ffffff;
  att |= size << 24;
  return(att);
}

void linear_coordinates(_KOORD *x1, _KOORD *y1, _KOORD *x2, _KOORD *y2)
{
	_KOORD h;
  if(*x1 > *x2) { h = *x1; *x1 = *x2; *x2 = h; }
  if(*y1 > *y2) { h = *y1; *y1 = *y2; *y2 = h; }
}

_UWORD format_unit(_CHAR *txt, _VARLABEL *pvl)
{
  _UWORD    retcode = 0;
  _ANYTHING anything;
  _CHAR     *tmp;

  init_ANYTHING_Makro(&anything);
  anything.list = VARUNIT;
  anything.no   = pvl->info[0].value;
  txt[0]        = 0;

  tmp = get_text(&anything, NULL, NULL, _FALSE);
  if(tmp != NULL)
  {
    StrCpy(txt, sizeof(_CHAR), tmp, sizeof(_CHAR));
    retcode = StrLen(txt, sizeof(_CHAR));
  }

  return(retcode);
}

_BOOL format_enum(_CHAR *txt, _IVALUE val, _UDWORD varno)
{
  _UDWORD itemvarno = VarList.GetEnumItemVarNo(val, varno);

  if(itemvarno < VarList.GetNo())
  {
    _ANYTHING any;
    init_ANYTHING(&any);
  
    any.list = VARTXT1_TOKEN;
    any.no   = itemvarno;
    _CHAR *pc = get_text(&any, NULL, NULL, _FALSE);
    if(pc != NULL)
    {
      StrCpy(txt, sizeof(_CHAR), pc, sizeof(_CHAR));
      return _TRUE;
    }
  
    _ASCII *pa = VarList.GetLabelToken(itemvarno);
    if(pa != NULL)
    {
      StrCpy(txt, sizeof(_CHAR), pa, sizeof(_ASCII));
      return _TRUE;
    }
  }
  
  txt[0] = '*';
  txt[1] = 'E';
  txt[2] = '*';
  DToA(&txt[3], val, 0x0800, sizeof(_CHAR));
  
  return _FALSE;
}

_UWORD format_variable(_CHAR *txt, _VARLABEL *pvl)
{
  _RESULT   result;
  _VARIABLE variable;

  txt[0] = 0;

  if(VarList.GetSystemData(&result, &variable, pvl, _TRUE, _FALSE) == _TRUE)
  {
    if(pvl->info[0].state == CONST_VAL)
      DToA(txt, result.value, 0x0800, sizeof(_CHAR));
    else if(pvl->info[0].state == POINTER_CHAR)
      StrCpy(txt, sizeof(_CHAR), (_CHAR*)pvl->info[0].value, sizeof(_CHAR));
    else if(pvl->info[0].state != DEF_VAL)
    {
      switch(variable.info.vartype)
      {
        case FSTRING:
          format_string_sub(txt, (_CHAR*)result.ptr, &variable.info, 199);
          break;

        case FDATE:
          format_date(txt, &result.datim.fdate, variable.info.format);
			 break;

        case FTIME:
          format_time(txt, &result.datim.ftime, variable.info.format);
          break;
#ifdef SA33149
        case FENUM:
          format_enum(txt, result.value, variable.no);
          break;
#endif
        default:
          recalculate(&result, &variable, _TRUE);
          format_value(txt, result.value, &variable.info, NULL);
          break;
      }

    }

  }

  return(StrLen(txt, sizeof(_CHAR)));
}


_UDWORD do_floating_dezpt(_IVALUE *val, _UWORD digits, _UWORD *posdezpt)
{
  _UWORD  i, pos, zw;
  _IVALUE tmp, value;
  _UWORD  retcode = 1;


  value = *val;
  pos   = 0;
  tmp   = value;
  do
  {
    pos ++;
    tmp = tmp / 10;
  }
  while(tmp != 0);

  if(pos > digits)
  {
    zw = pos - digits;
    if(zw <= *posdezpt)
    {
      for(i=0; i<zw; i++)
        retcode = retcode * 10;

      *val      = value / retcode;
      *posdezpt = (*posdezpt) - zw;
    }
  }

  return(retcode);
}


void format_ipaddress(_CHAR *txt, _UDWORD address)
{
  _UWORD i;

  txt[0] = 0;
  for(i=0; i<4; i++)
  {
    if(i > 0)
      StrCat(txt, sizeof(_CHAR), ".", sizeof(_ASCII));
    NumToStr(&txt[StrLen(txt, sizeof(_CHAR))], address&0xFF, sizeof(_CHAR));
    address >>= 8;
  }
}


void format_value(_CHAR *txt, _IVALUE value, _VARINFO *pi, _PICFORMAT *pf)
{
  _UWORD dezpt = VarList.GetDezPoint(pi);
  _UWORD dig   = VarList.GetDigits(pi);
  _BOOL  pz    = VarList.GetZero(pi->POWPRUIC);
  _UWORD format;


 #ifdef UC_REAL
  #ifndef UC_IPC
   value = format_value_st((_REAL*)&value, pi, dezpt);
  #else
   if((pi->vartype == BINARY)&&(pi->format == SIZE_REAL))
   {
     _REAL tempr = get_pREAL((_REAL*)&value);
    if(tempr >= 0)
      value = FTOL((tempr * get_multiplicator(dezpt)) + 0.5);
    else
      value = FTOL((tempr * get_multiplicator(dezpt)) - 0.5);

   }
  #endif
 #endif

//  if(pi->format & CALCUL_DIGITS)
  if(VarList.GetFixDezpt(pi) == _FALSE)
    do_floating_dezpt(&value, dig, &dezpt);

  format = (dig & 0x000F) | ((dezpt << 4)& 0x00F0);

  if(pz == _TRUE)
    format |= 0x0100;

  format |= 0x0400;

//#ifndef USER_DEMAG // aufgrund von sa24870 wieder entfernen
  if(dig == 0)
//#endif  
    format |= 0x0800;

  DToA(txt, value, format, sizeof(_CHAR));
}

void format_time(_CHAR *txt, _TIME *ftime, _UWORD format)
{
  _BOOL usa = get_americantimeformat_Makro();
  _BOOL pm;
  _TIME tim;

  if((ftime->hour == 0xFF)&&(ftime->minute == 0xFF)&&(ftime->second == 0xFF))
  {
    if(format == SIZE_HHMMSS)
		  StrCpy(txt, sizeof(_CHAR), "--:--:--", sizeof(_ASCII));
    else
      StrCpy(txt, sizeof(_CHAR), "--:--", sizeof(_ASCII));
      
   #ifndef USER_DEKEMA
    if(usa == _TRUE)
		  StrCat(txt, sizeof(_CHAR), " --", sizeof(_ASCII));
   #endif
  }
  else
  {
    tim = *ftime;
    pm  = _FALSE;

    if(usa == _TRUE)
    {
      if(tim.hour >= 12)
        pm = _TRUE;

      if(tim.hour >= 13)
        tim.hour -= 12;
    }

    if(format == SIZE_HHMMSS)
    {
	    DToA(txt, tim.hour, 0x0102, sizeof(_CHAR));
      txt[2] = ':';
      DToA(&txt[3], tim.minute, 0x0102, sizeof(_CHAR));
      txt[5] = ':';
      DToA(&txt[6], tim.second, 0x0102, sizeof(_CHAR));
      if(usa == _TRUE)
      {
        txt[8] = ' ';
        txt[9] = (pm == _TRUE)? 'p':'a';
        txt[10] = 'm';
        txt[11] = 0;
      }
    }
    else if(format == SIZE_MMSS)
    {
	    DToA(txt, tim.minute, 0x0102, sizeof(_CHAR));
      txt[2] = ':';
      DToA(&txt[3], tim.second, 0x0102, sizeof(_CHAR));
    }
    else
    {
	    DToA(txt, tim.hour, 0x0102, sizeof(_CHAR));
      txt[2] = ':';
      DToA(&txt[3], tim.minute, 0x0102, sizeof(_CHAR));
      if(usa == _TRUE)
      {
        txt[5] = ' ';
        txt[6] = (pm == _TRUE)? 'p':'a';
        txt[7] = 'm';
        txt[8] = 0;
      }
    }
  }
}

_CHAR *get_wotag(_CHAR *wt, _UWORD dow)
{
  _CHAR  *txt = TextList.Get(TXTLIST_STX, 0);
  _UWORD len, no;

  wt[0] = 'w';
  wt[1] = 'd';
  wt[2] = (get_americandateformat_Makro() == _TRUE)? '/':':';
  wt[3] = 0;

  if(txt != NULL)
  {
    no = 1 + (dow % 7) * 2;
    len = StrLen(txt, sizeof(_CHAR));
    if((no+1) < len)
    {
      wt[0] = txt[no];
		  wt[1] = txt[no+1];
      return wt;
    }
  }

  _CHAR  *ph = NULL;
  _UWORD chrsize;
  _CHAR  tmp[100]; 
  Language.GetStandardText((void**)&ph, &chrsize, TxtID_WEEKDAYS);
  
  if(ph != NULL)
  {
    if(chrsize != sizeof(_CHAR))
    {
      StrCpy(tmp, sizeof(_CHAR), ph, chrsize);
      ph = tmp;
    }
    
    no  = (dow % 7) * 2;
    len = StrLen(ph, sizeof(_CHAR));
    if((no+1) < len)
    {
      wt[0] = ph[no];
		  wt[1] = ph[no+1];
    }
  }
  
  return(wt);
}

void format_date(_CHAR *txt, _DATE *dte, _UWORD format)
{
  _CHAR wt[5];
  _CHAR ch;
  _UWORD dd, mm;

  if(get_americandateformat_Makro() == _TRUE)
  {
    dd = dte->month;
    mm = dte->day;
    ch = '/';
  }
  else
  {
    dd = dte->day;
    mm = dte->month;
    ch = '.';
  }

  if(((dd == 0)&&(mm == 0))||(dte->year == 0xFFFF))
  {
    switch(format)
    {
      case SIZE_YYMM     : StrCpy(txt, sizeof(_CHAR), "--_--", sizeof(_ASCII));
                           break;
      case SIZE_YYMMDD   : StrCpy(txt, sizeof(_CHAR), "--_--_--", sizeof(_ASCII));
                           break;
      case SIZE_YYYYMMDD : StrCpy(txt, sizeof(_CHAR), "----_--_--", sizeof(_ASCII));
                           break;
      case SIZE_DDMM     : StrCpy(txt, sizeof(_CHAR), "--:--", sizeof(_ASCII));
                           txt[2] = ch;
                           break;
      case SIZE_DDMMYY   : StrCpy(txt, sizeof(_CHAR), "--:--:--", sizeof(_ASCII));
                           txt[2] = ch;
                           txt[5] = ch;
                           break;
      case SIZE_WWDDMM   : StrCpy(txt, sizeof(_CHAR), "--:--:--", sizeof(_ASCII));
                           txt[5] = ch;
                           break;
      case SIZE_WWDDMMYY : StrCpy(txt, sizeof(_CHAR), "--:--:--:--", sizeof(_ASCII));
                           txt[5] = ch;
                           txt[8] = ch;
                           break;
      case SIZE_DDMMYYYY : StrCpy(txt, sizeof(_CHAR), "--:--:----", sizeof(_ASCII));
                           txt[2] = ch;
                           txt[5] = ch;
                           break;
      default            : StrCpy(txt, sizeof(_CHAR), "--:--:--:----", sizeof(_ASCII));
                           txt[5] = ch;
                           txt[8] = ch;
                           break;
    }
  }
  else
  {
    switch(format)
    {
      case SIZE_DDMM     : DToA(txt, dd, 0x0102, sizeof(_CHAR));
                           txt[2] = ch;
                           DToA(&txt[3], mm, 0x0102, sizeof(_CHAR));
                           break;
      case SIZE_DDMMYY   : DToA(txt, dd, 0x0102, sizeof(_CHAR));
                           txt[2] = ch;
                           DToA(&txt[3], mm, 0x0102, sizeof(_CHAR));
                           txt[5] = ch;
                           DToA(&txt[6], dte->year % 100, 0x0102, sizeof(_CHAR));
                           break;
      case SIZE_WWDDMM   : StrCpy(txt, sizeof(_CHAR), get_wotag(wt, dte->dayofweek), sizeof(_CHAR));
                           DToA(&txt[3], dd, 0x0102, sizeof(_CHAR));
                           txt[5] = ch;
                           DToA(&txt[6], mm, 0x0102, sizeof(_CHAR));
                           break;
      case SIZE_WWDDMMYY : StrCpy(txt, sizeof(_CHAR), get_wotag(wt, dte->dayofweek), sizeof(_CHAR));
                           DToA(&txt[3], dd, 0x0102, sizeof(_CHAR));
                           txt[5] = ch;
                           DToA(&txt[6], mm, 0x0102, sizeof(_CHAR));
                           txt[8] = ch;
                           DToA(&txt[9], dte->year % 100, 0x0102, sizeof(_CHAR));
                           break;
      case SIZE_DDMMYYYY : DToA(txt, dd, 0x0102, sizeof(_CHAR));
                           txt[2] = ch;
                           DToA(&txt[3], mm, 0x0102, sizeof(_CHAR));
                           txt[5] = ch;
                           DToA(&txt[6], dte->year, 0x0104, sizeof(_CHAR));
                           break;
      case SIZE_YYYYMMDD : DToA(txt, dte->year, 0x0104, sizeof(_CHAR));
                           txt[4] = '-';
                           DToA(&txt[5], dte->month, 0x0102, sizeof(_CHAR));
                           txt[7] = '-';
                           DToA(&txt[8], dte->day, 0x0102, sizeof(_CHAR));
                           break;
      case SIZE_YYMMDD   : DToA(txt, dte->year % 100, 0x0102, sizeof(_CHAR));
                           txt[2] = '-';
                           DToA(&txt[3], dte->month, 0x0102, sizeof(_CHAR));
                           txt[5] = '-';
                           DToA(&txt[6], dte->day, 0x0102, sizeof(_CHAR));
                           break;
      case SIZE_YYMM     : DToA(txt, dte->year % 100, 0x0102, sizeof(_CHAR));
                           txt[2] = '-';
                           DToA(&txt[3], dte->month, 0x0102, sizeof(_CHAR));
                           break;
      default            : StrCpy(txt, sizeof(_CHAR), get_wotag(wt, dte->dayofweek), sizeof(_CHAR));
                           DToA(&txt[3], dd, 0x0102, sizeof(_CHAR));
                           txt[5] = ch;
			  									 DToA(&txt[6], mm, 0x0102, sizeof(_CHAR));
                           txt[8] = ch;
                           DToA(&txt[9], dte->year, 0x0104, sizeof(_CHAR));
                           break;
    }
  }
}

void format_string_sub(_CHAR *txt, _CHAR *src, _VARINFO *pi, _UWORD no)
{
  _UWORD digits;


  digits = VarList.GetCompleteNoDigits(pi);
  if(digits > no)
    digits = no;

  MemCpy(txt, src, digits * sizeof(_CHAR));
  txt[digits] = 0;
}

void format_string(_CHAR *txt, _CHAR *src, _VARINFO *pi)
{

  format_string_sub(txt, src, pi, 199);
}

void out_bit(_UBYTE *data, _ROOM *room, _DWORD value, _ATTRIB attrib, _COLOR coltxt, _COLOR colback, _COLOR colframe)
{
  _KOORD  xx, yy;
  _ROOM   space;
  _BMP    bmp;
  _BMPDAT bmpdat[256];
  _BMPDAT *p;
  _COLOR  color[6];


  space = *room;
  xx    = (space.xy1.x + space.xy2.x + 1) / 2 - 8;
  yy    = (space.xy1.y + space.xy2.y + 1) / 2 - 8;

  color[0] = GetBackColor_Makro(colback);
  color[1] = GetPenColor_Makro(coltxt);
  color[5] = color[0]; //GetPenColor_Makro(colback);

  if(value == 0)
  {
    color[2] = GetBackColor_Makro(colframe);
    color[3] = GetPenColor_Makro(colframe);
    color[4] = color[0]; //GetPenColor_Makro(colback);
  }
  else
  {
    color[2] = GetPenColor_Makro(colframe);
    color[3] = GetBackColor_Makro(colframe);
    color[4] = GetPenColor_Makro(coltxt);
  }

//  if(color[0] != INVISIBLE)
//    Bar(space.xy1.x, space.xy1.y, space.xy2.x, space.xy2.y, T_SOLID|T_COPY, colback);
  if(is_invisible(colback, attrib) == _FALSE)
    Bar(space.xy1.x, space.xy1.y, space.xy2.x, space.xy2.y, attrib, colback);

  p = bmpdat;
  for(_UDWORD i=0; i<256; i++)
  {
    p->no  = 1;
    p->col = color[data[i]];
    p     += 1;
  }

  init_BMP_Makro(&bmp);

  bmp.width       = 16;
  bmp.height      = 16;
  bmp.transparent = color[0]; //GetBackColor_Makro(colback);
//  bmp.state       = BMP_HICOLOR | ((color[0] == INVISIBLE)? 0 : BMP_NO_TRANSPARENT);
  bmp.state       = BMP_HICOLOR; // | ((color[0] == INVISIBLE)? 0 : BMP_NO_TRANSPARENT);
  bmp.datano      = 256;
  bmp.ptr         = bmpdat;

  Bitmap(xx, yy, &bmp, T_COPY|T_SOLID, 0);
//  Bitmap(xx, yy, &bmp, T_COPY|T_SOLID|T_DUPX|T_DUPY, 0);

//  out_zoom_BMP(room, &bmp, T_COPY|T_SOLID);
}


void out_userdefbox(_BMP *bmp, _ROOM *room, _ATTRIB attrib, _COLOR colback)
{
  _KOORD xx, yy;

  xx = (room->xy1.x + room->xy2.x + 1 - bmp->width) / 2;
  yy = (room->xy1.y + room->xy2.y + 1 - bmp->height) / 2;

  if(is_invisible(colback, attrib) == _FALSE)
    Bar(room->xy1.x, room->xy1.y, room->xy2.x, room->xy2.y, attrib, colback);

  Bitmap(xx, yy, bmp, T_COPY|T_SOLID, 0);
}

void out_radiobutton(_ROOM *room, _DWORD value, _ATTRIB attrib, _COLOR coltxt, _COLOR colback, _COLOR colframe)
{
//  _UBYTE data[256];
//  MemCpy(data, MyRadioButton, 256);
//  out_bit(data, room, value, attrib, coltxt, colback, colframe);
  _BMP *bmp = SetUp.GetRadioBoxImage(value);
  if(bmp != NULL)
  {
    out_userdefbox(bmp, room, attrib, colback);
  }
  else
  {
    out_bit(MyRadioButton, room, value, attrib, coltxt, colback, colframe);
  }
}

void out_chkbox(_ROOM *room, _DWORD value, _ATTRIB attrib, _COLOR coltxt, _COLOR colback, _COLOR colframe)
{
//  _UBYTE data[256];
//  MemCpy(data, MyCheckBox, 256);
//  out_bit(data, room, value, attrib, coltxt, colback, colframe);

  _BMP *bmp = SetUp.GetChkBoxImage(value);
  if(bmp != NULL)
  {
    out_userdefbox(bmp, room, attrib, colback);
  }
  else
  {
    out_bit(MyCheckBox, room, value, attrib, coltxt, colback, colframe);
  }
}

_BOOL editor_to_text(_EDITOR *pe)
{
  _BOOL  retcode = _TRUE;

  switch(pe->input.io.typ)
  {
    case AS_IPADDRESS: format_ipaddress(pe->chredit.fstring, pe->result.value);
                      break;
    case AS_NUMERIC : format_value(pe->chredit.fstring, pe->result.value, &pe->variable.info, NULL);
                      break;
    case AS_ASCII   : format_string_sub(pe->chredit.fstring, (_CHAR*)pe->result.ptr, &pe->variable.info, (sizeof(pe->chredit.fstring) / sizeof(_CHAR)) - 1); //, pe->input.io.info.tbo.Xwidth);
                      break;
    case AS_RADIO   :
    case AS_CHKBOX  : pe->chredit.fstring[0] = (pe->result.value == 0)? '0' : '1';
                      pe->chredit.fstring[1] = 0;
                      break;
    case AS_DATE    : format_date(pe->chredit.fstring, &pe->result.datim.fdate, pe->variable.info.format);
                      break;
    case AS_TIME    : format_time(pe->chredit.fstring, &pe->result.datim.ftime, pe->variable.info.format);
                      break;
    default         : retcode = _FALSE;
//    case AS_BAR,         // balken
//    case AS_TBO,         // text, bitmap or object
//    case AS_MENUE,
//    case AS_USER
  }

	return(retcode);
}

void set_DATIM(_DATIM *p)
{
  _MAESYSTEMTIME st;
  st.wYear         = p->fdate.year;
  st.wMonth        = p->fdate.month;
  st.wDayOfWeek    = day_of_week(&p->fdate);
  st.wDay          = p->fdate.day;
  st.wHour         = p->ftime.hour;
  st.wMinute       = p->ftime.minute;
  st.wSecond       = p->ftime.second;
  st.wMilliseconds = 0;

  Sys_SetTime((void*)&st);
}

void get_DATIM(_DATIM *p)
{
  _MAESYSTEMTIME st;
  Sys_GetTime((void*)&st);

  p->fdate.year      = st.wYear;
  p->fdate.month     = st.wMonth;
	p->fdate.dayofweek = st.wDayOfWeek;
  p->fdate.day       = st.wDay;
  p->ftime.hour      = st.wHour;
  p->ftime.minute    = st.wMinute;
  p->ftime.second    = st.wSecond;
}

void set_DATE(_DATE *p)
{
  _MAESYSTEMTIME st;

  Sys_GetTime((void*)&st);
  st.wYear      = p->year;
  st.wMonth     = p->month;
  st.wDayOfWeek = day_of_week(p);
  st.wDay       = p->day;
  Sys_SetTime((void*)&st);
}

void set_TIME(_TIME *p)
{
  _MAESYSTEMTIME st;

  Sys_GetTime((void*)&st);
  st.wHour   = p->hour;
	st.wMinute = p->minute;
  st.wSecond = p->second;
  Sys_SetTime((void*)&st);
}

void get_DATE(_DATE *p)
{
  _MAESYSTEMTIME st;

  Sys_GetTime((void*)&st);
  p->year      = st.wYear;
  p->month     = (_UBYTE)st.wMonth;
  p->dayofweek = (_UBYTE)st.wDayOfWeek;
  p->day       = (_UBYTE)st.wDay;
}

void get_TIME(_TIME *p)
{
  _MAESYSTEMTIME st;

  Sys_GetTime((void*)&st);
  p->hour   = (_UBYTE)st.wHour;
  p->minute = (_UBYTE)st.wMinute;
  p->second = (_UBYTE)st.wSecond;
}

/*
_UBYTE mday[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
_UDWORD get_absday(_DATE *p)
{
  _UDWORD days;
  _UWORD  i, maxs;
  _UWORD  year = p->year;
  _UWORD  month = p->month;

  mday[1] = 28;

  if((year % 4) == 0)
  {
    mday[1] = 29;
    if((year % 100) == 0)
    {
      mday[1] = 28;
      if((year % 400) == 0)
      {
        mday[1] = 29;
      }
    }
  }

	year --;

  maxs  = year / 4;
  maxs -= year / 100;
  maxs += year / 400;
  days = (((_UDWORD)year) * 365L) + maxs;

  month --;
  for(i=0; i<month; i++)
    days += mday[i];

  days += p->day;

  return(days);
}
*/
_UWORD nyea[13] = {0, 0,31,59,90,120,151,181,212,243,273,304,334};
_UWORD syea[13] = {0, 0,31,60,91,121,152,182,213,244,274,305,335};
_UDWORD get_absday(_DATE *p)
{
  _UDWORD days;
  _UWORD  maxs;
  _UWORD  year = p->year;
  _UWORD  month = p->month;
  _UWORD  *py = nyea;

  if((year % 4) == 0)
  {
    py = syea;
    if((year % 100) == 0)
    {
      py = nyea;
      if((year % 400) == 0)
      {
        py = syea;
      }
    }
  }

	year --;

  maxs  = year / 4;
  maxs -= year / 100;
  maxs += year / 400;
  days = (((_UDWORD)year) * 365L) + maxs;

  if(month <= 12)
    days += py[month];

  days += p->day;

  return(days);
}


_UWORD day_of_week(_DATE *p)
{
  return(get_absday(p) % 7);
}

extern "C" _DWORD DATE_Diff(_DATE *d0, _DATE *d1)
{
  _DWORD retcode;

  retcode = get_absday(d1) - get_absday(d0);

  return(retcode);
}

_UWORD offset_dpne(_ASCII *dpne, _UWORD st)
{
  _UWORD i, len = StrLen(dpne, sizeof(_ASCII));

  switch(st)
  {
    case 0 : for(i=0; i<len; i++)   // d
             {
               if(dpne[i] == ':')
								 return(i+1);
             }
             break;
    case 1 : for(i=0; i<len; i++)
             {
               if(dpne[len-i-1] == '\\')  // dp
                 return(len-i);
             }
             break;
  }

  return(0);
}


void GetShadowRoom(_ROOM *p)
{
  p->xy1.x += POPUP_SHADOW;
  p->xy1.y += POPUP_SHADOW;
  p->xy2.x += POPUP_SHADOW;
  p->xy2.y += POPUP_SHADOW;
}

void DrawShadow(_ROOM room, _FRAMETYPE frametype)
{
#if POPUP_SHADOW > 0
  _ROOM      tmp;

  GetShadowRoom(&room);

 #ifdef UC_MOUSE
  _UWORD hide = IfNecHideMouseCursor(&room);
 #endif

 #ifdef UC_THEME
  if(frametype >= _THEMEFRAME)
  {
    shadow_THEME(&room, frametype);
   #ifdef UC_ROTATE
    RefreshRectangle(&room);
   #endif
  }
  else
 #endif
  {
    tmp = room;
    tmp.xy1.y = tmp.xy2.y - POPUP_SHADOW;
    Bar(tmp.xy1.x, tmp.xy1.y, tmp.xy2.x, tmp.xy2.y, T_COPY|T_SOLID, MakeColor_Makro(BLACK, BLACK));
   #ifdef UC_ROTATE
    RefreshRectangle(&tmp);
   #endif

    tmp = room;
    tmp.xy1.x = tmp.xy2.x - POPUP_SHADOW;
    Bar(tmp.xy1.x, tmp.xy1.y, tmp.xy2.x, tmp.xy2.y, T_COPY|T_SOLID, MakeColor_Makro(BLACK, BLACK));
   #ifdef UC_ROTATE
    RefreshRectangle(&tmp);
   #endif
  }
 #ifdef UC_MOUSE
  IfNecShowMouseCursor(hide);
 #endif
#endif
}

_DWORD get_multiplicator(_UWORD dzp)
{
  switch(dzp)
  {
    case 0: return         1;
    case 1: return        10; 
    case 2: return       100; 
    case 3: return      1000; 
    case 4: return     10000; 
    case 5: return    100000; 
    case 6: return   1000000; 
    case 7: return  10000000; 
		case 8: return 100000000; 
  }

  return 1000000000;
}

_DWORD get_digits(_UWORD value)
{
  if(value >= 100000000)
    return 9;
  if(value >= 10000000)
    return 8;
  if(value >= 1000000)
    return 7;
  if(value >= 100000)
    return 6;
  if(value >= 10000)
    return 5;
  if(value >= 1000)
    return 4;
  if(value >= 100)
    return 3;
  if(value >= 10)
    return 2;
    
  return 1;
}

//_REGION ActivateAndRegion(_REGION *ior, _REGION *newr, _ROOM *r, _ATTRIB attrib)
_REGION ActivateAndRegion(_REGION *ior, _REGION *newr, _ROOM *r)
{
  _REGION ioreg, newreg, actreg;

  actreg = GetActiveRegion();

  StartRegion();
//  Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_SOLID|T_COPY|T_FILL, LIGHTGREEN);
  AddRegion(r);
  ioreg = EndRegion();

	if(actreg != NULL)
  {
    newreg = AndRegion(ioreg, actreg);
  }
  else
  {
    newreg = ioreg;
    ioreg  = NULL;
  }

  ActivateRegion(newreg);

  *ior  = ioreg;
  *newr = newreg;

  return(actreg);
}

void DeactivateAndRegion(_REGION actreg, _REGION *ior, _REGION *newr)
{
  DeactivateRegion();
  FreeRegion(newr);
  FreeRegion(ior);
  ActivateRegion(actreg);
}

_DWORD AsciiToNo(_ASCII **p)
{
  _ASCII *n, chr;
  _BOOL  neg = _FALSE;
  _DWORD retcode = 0;
  _UWORD i = 0;
  _BOOL  run = _TRUE;

  n = *p;

  do
  {
    chr = n[i];
    if(chr == '-')
    {
      neg = _TRUE;
      i ++;
    }
    else if((chr >= '0') && (chr <= '9'))
    {
      retcode = (retcode * 10) + (chr - '0');
      i ++;
    }
    else
    {
      run = _FALSE;
    }
  }
  while(run == _TRUE);

  *p = &n[i];

  if(neg == _TRUE)
    retcode = -retcode;

  return(retcode);
}

void NumToStr(void *txt, _DWORD value, _UWORD chr_size)
{
  _DWORD tmp;
  _UWORD dig = 0;

  tmp = value;
  do
  {
		dig ++;
    tmp /= 10;
  }
  while(tmp != 0);

  DToA(txt, value, dig, chr_size);
}

#ifdef UC_IPC

void RealToStr(void *txt, _REAL value, _UWORD dzp, _UWORD chr_size)
{
  // vorsicht: diese funktion existiert auch in st.
  // compilerschalter UC_IPC entscheidet welche verwendet wird
  // dzp ... anzahl der gültigen nachkommastellen

  _DWORD vk, nk, mul, len;
  _REAL  rr;
  _ASCII *pa;
 #ifdef UC_UNICODE
  _UNI   *pu; 
 #endif
 
  if(value < 0)
  {    
    if(chr_size == 1)
    {
      pa    = (_ASCII*)txt;
      pa[0] = '-';
      pa[1] = 0;
      txt   = (void*)&pa[1];
    }
   #ifdef UC_UNICODE
    else
    {
      pu    = (_UNI*)txt;
      pu[0] = '-';
      pu[1] = 0;
      txt   = (void*)&pu[1];
    }
   #endif
    value = -value;
  }

  if(dzp != 0)
  {
    mul   = get_multiplicator(dzp);
    rr    = 5.0 / ((_REAL)(mul * 10));
    value += rr;
    vk    = FTOL(value);
    nk    = FTOL((value - vk) * mul);
  }
  else
  {
    vk = FTOL(value + 0.5);
  }
  
  DToA(txt, vk, 0x0800, chr_size);
  
  if(dzp != 0)
  {
    len = StrLen(txt, chr_size);

    if(chr_size == 1)
    {
      pa      = (_ASCII*)txt;
      pa[len] = '.'; len += 1;
      pa[len] = 0;
      txt     = (void*)&pa[len];
    }
   #ifdef UC_UNICODE
    else
    {
      pu      = (_UNI*)txt;
      pu[len] = '.'; len += 1;
      pu[len] = 0;
      txt     = (void*)&pu[len];
    }
   #endif
   
    DToA(txt, nk, 0x100 | dzp, chr_size);
  }
}
#endif

#ifdef UC_IPC
_BOOL StrToReal(_REAL *dv, void *txt, _UWORD chr_size)
{
  // vorsicht: diese funktion existiert auch in st.
  // compilerschalter UC_IPC entscheidet welche verwendet wird
  _BOOL  retcode = _FALSE;
  _ASCII *at, ac;
  _BOOL  neg  = _FALSE;
  _BOOL  dot  = _FALSE;
  _BOOL  run  = _TRUE;
  _DWORD vk   = 0;
  _DWORD nk   = 0;
  _UDWORD div = 1;
  _REAL  rval = 0;

 #ifdef UC_UNICODE
  _ASCII ascii[100];
  if(chr_size == sizeof(_UNI))
  {
    StrCpy(ascii, sizeof(_ASCII), txt, sizeof(_UNI));
    chr_size = sizeof(_ASCII);
    txt      = ascii;
  }
 #endif

  if(chr_size == 1)
  {
    at = (_ASCII*)txt;
    do
    {
      ac = *at;
      if(ac == '-')
      {
        neg = _TRUE;
      }
      else if((ac == '.')||(ac == ','))
      {
        dot = _TRUE;
      }
      else if((ac >= '0') && (ac <= '9'))
      {
        retcode = _TRUE;
        if(dot == _FALSE)
        {
          vk = (vk * 10) + (ac - '0');
        }
        else
        {
          div = div * 10;
          nk = (nk * 10) + (ac - '0');
        }
      }
      else if(ac == 0)
      {
        run = _FALSE;
      }
      at ++;
    }
		while(run == _TRUE);
  }

  if(retcode == _TRUE)
  {
    rval = vk;

    if((div > 1)&&(nk != 0))
      rval = rval + (((_REAL)nk) / div);
  }

  if(neg == _TRUE)
    rval = -rval; 

  move_pREAL_REAL(dv, rval);

  return(retcode);
}
#endif

_BOOL StrToNum(_DWORD *dv, void *txt, _UWORD chr_size)
{
  _ASCII *at, ac;
 #ifdef UC_UNICODE
  _UNI   *ut, uc;
 #endif
  _BOOL  neg = _FALSE;
  _BOOL  retcode = _FALSE;
  _BOOL  run = _TRUE;
  _DWORD value = 0;
  _UWORD i = 0;

  if(chr_size == 1)
  {
    at = (_ASCII*)txt;
    do
    {
      ac = at[i];
      if(ac == '-')
      {
        neg = _TRUE;
      }
      else if((ac >= '0') && (ac <= '9'))
      {
        retcode = _TRUE;
        value   = (value * 10) + (ac - '0');
      }
      else if(ac == 0)
      {
        run = _FALSE;
      }
      i ++;
    }
    while(run == _TRUE);
  }
 #ifdef UC_UNICODE
  else if(chr_size == 2)
  {
    ut = (_UNI*)txt;
    do
    {
      uc = ut[i];
			if(uc == '-')
      {
        neg = _TRUE;
      }
      else if((uc >= '0') && (uc <= '9'))
      {
        retcode = _TRUE;
        value = (value * 10) + (uc - '0');
      }
      else if(uc == 0)
      {
        run = _FALSE;
      }

      i ++;
    }
    while(run == _TRUE);
  }
 #endif

  if(neg == _TRUE)
    value = -value;

  *dv = value;

	return(retcode);
}


#ifdef UC_UNICODE
void dtoa_UNI(_UNI *pt, _DWORD value, _UWORD format)
{
  // xxxx xx00 0000 0000 format
  //       ... .... **** no of digits
  //       ... **** .... position decimalpoint
  //       ..* .... .... preceding zeros
  //       .*. .... .... descendant zeros
  //       *.. .... .... vorzeichen unmittelbar vor wert (-3 ansonsten -  3)

  _UNI   *hp;
  _WORD  pd;
  _UWORD h, dig;
  _BOOL  neg = _FALSE;
  _BOOL  toobig = _FALSE;
  _UNI   dot = (get_commasign_Makro() == _TRUE)? ',':'.';

  dig = (format & 0x000F);
  hp  = &pt[dig];
  pd  = (format >> 4) & 0x000F;

  if(value < 0)        // look for negative value
  {
//    if(value == -2147483648L)
    if(value == 0x80000000)   //GNU
      value += 1;
		hp    += 1;
    value  = -value;
    pt[0]  = '-';
    neg    = _TRUE;
  }

  if(format & 0x0200)  // descendant zeros
  {
    h = pd;

    while(h > 0)
    {
      h --;
      if((value % 10) == 0)
      {
				pd --;
        value = value / 10;
      }
      else
      {
        h = 0;
      }
    }
  }

	if(pd != 0)          // look for decimalpoint
  {
    hp += 1;
  }

  hp[0] = 0;           // final 0 (ascii-0-string)
  hp   -= 1;

  do
  {
    hp[0] = '0' + (_CHAR)(value % 10);
    hp    -= 1;
    pd    -= 1;

    if(dig > 0)
      dig -= 1;

    if(pd == 0)
    {
      hp[0] = dot;
      hp   -= 1;
    }

    value = value / 10;

    if((dig == 0) & (value != 0))
    {
			// value too big
//      pt[0]   = '?';
      value   = 0;
      toobig = _TRUE;
    }
  }
  while(value != 0);


  if(dig > 0)   // fill with space or preceding zero
  {
    do
    {
      if((format & 0x0100)||(pd >= 0))
        hp[0] = '0';
			else
        hp[0] = ' ';

      pd -= 1;
      hp -= 1;

      if(pd == 0)
      {
        hp[0] = dot;
        hp   -= 1;
      }

	    dig -= 1;
    }
	  while(dig != 0);
  }

  if((format & 0x0400) && (neg == _TRUE))
  {
    hp = &pt[1];
    while(*hp == ' ')
      hp++;
    pt[0] = ' ';
    hp -= 1;
    *hp = '-';
	}

  if(toobig == _TRUE)
  {
    hp = pt;
    while(*hp != 0)
    {
      if(*hp != dot)
        *hp = '>';
      hp++;
    }
  }
}

void dtoh_UNI(_UNI *pt, _DWORD val, _UWORD format)
{
  _UWORD  j, i, digit;
  _CHAR   tmp[100];
  _UDWORD value = (_UDWORD)val;

  digit = format & 0x000F;

  for(j=0; j<digit; j++)
    tmp[j] = '0';
  tmp[digit] = 0;

  j = 0;
  do
  {
    switch(value % 16)
    {
      case  0 : tmp[j] = '0'; break;
      case  1 : tmp[j] = '1'; break;
      case  2 : tmp[j] = '2'; break;
      case  3 : tmp[j] = '3'; break;
      case  4 : tmp[j] = '4'; break;
      case  5 : tmp[j] = '5'; break;
      case  6 : tmp[j] = '6'; break;
      case  7 : tmp[j] = '7'; break;
      case  8 : tmp[j] = '8'; break;
      case  9 : tmp[j] = '9'; break;
      case 10 : tmp[j] = 'A'; break;
      case 11 : tmp[j] = 'B'; break;
      case 12 : tmp[j] = 'C'; break;
      case 13 : tmp[j] = 'D'; break;
      case 14 : tmp[j] = 'E'; break;
      default : tmp[j] = 'F'; break;
    }
    j++;
		value = value / 16;
  }
  while(value != 0);

  if(digit < j)
    digit = j;

	for(i=0; i<digit; i++)
    pt[i] = tmp[digit-i-1];
  pt[digit] = 0;
}

#endif

void dtoh_ASCII(_ASCII *pt, _DWORD val, _UWORD format)
{
  _UWORD  j, i, digit;
  _CHAR   tmp[100];
  _UDWORD value = (_UDWORD)val;

  digit = format & 0x000F;

  for(j=0; j<digit; j++)
    tmp[j] = '0';
  tmp[digit] = 0;

  j = 0;
  do
  {
    switch(value % 16)
    {
      case  0 : tmp[j] = '0'; break;
      case  1 : tmp[j] = '1'; break;
      case  2 : tmp[j] = '2'; break;
      case  3 : tmp[j] = '3'; break;
      case  4 : tmp[j] = '4'; break;
      case  5 : tmp[j] = '5'; break;
      case  6 : tmp[j] = '6'; break;
      case  7 : tmp[j] = '7'; break;
      case  8 : tmp[j] = '8'; break;
      case  9 : tmp[j] = '9'; break;
      case 10 : tmp[j] = 'A'; break;
      case 11 : tmp[j] = 'B'; break;
      case 12 : tmp[j] = 'C'; break;
      case 13 : tmp[j] = 'D'; break;
      case 14 : tmp[j] = 'E'; break;
      default : tmp[j] = 'F'; break;
    }
    j++;
		value = value / 16;
  }
  while(value != 0);

  if(digit < j)
    digit = j;

	for(i=0; i<digit; i++)
    pt[i] = tmp[digit-i-1];
  pt[digit] = 0;

/*

  for(i=0, j=0; i<digit; i++)
  {
    rest  = value % 16;
		value = value / 16;

    switch(rest)
		{
      case  0 : tmp[j] = '0'; break;
      case  1 : tmp[j] = '1'; break;
      case  2 : tmp[j] = '2'; break;
      case  3 : tmp[j] = '3'; break;
      case  4 : tmp[j] = '4'; break;
      case  5 : tmp[j] = '5'; break;
      case  6 : tmp[j] = '6'; break;
      case  7 : tmp[j] = '7'; break;
      case  8 : tmp[j] = '8'; break;
      case  9 : tmp[j] = '9'; break;
      case 10 : tmp[j] = 'A'; break;
      case 11 : tmp[j] = 'B'; break;
      case 12 : tmp[j] = 'C'; break;
      case 13 : tmp[j] = 'D'; break;
      case 14 : tmp[j] = 'E'; break;
      case 15 : tmp[j] = 'F'; break;
    }

    j ++;

    if((value == 0) && (pz == _FALSE))
			break;
  }

	for(i=0; i<j; i++)
    pt[i] = tmp[j-i-1];

  pt[j] = 0;
*/
}

void dtoa_ASCII(_ASCII *pt, _DWORD value, _UWORD format)
{
  // xxxx xx00 0000 0000 format
  //      .... .... **** no of digits
  //      .... **** .... position decimalpoint
  //      ...* .... .... preceding zeros (vornullen)
  //      ..*. .... .... descendant zeros (nullen bei nachkommastellen)
  //      .*.. .... .... vorzeichen unmittelbar vor wert (-3 ansonsten -  3)
  //      *... .... .... anzahl der stellen automatisch ermitteln

  _ASCII *hp;
  _WORD  pd;
  _UWORD h, dig;
  _BOOL  neg = _FALSE;
  _BOOL  toobig = _FALSE;
  _ASCII dot = (get_commasign_Makro() == _TRUE)? ',':'.';

  dig = (format & 0x000F);
  hp  = &pt[dig];
  pd  = (format >> 4) & 0x000F;

  if(value < 0)        // look for negative value
  {
//    if(value == -2147483648L)
    if(value == 0x80000000)   //GNU
      value += 1;

    hp    += 1;
    value  = -value;
    pt[0]  = '-';
    neg    = _TRUE;
  }

  if(format & 0x0200)  // descendent zeros
  {
    h = pd;

    while(h > 0)
    {
      h --;
      if((value % 10) == 0)
      {
        pd --;
        value = value / 10;
      }
      else
      {
				h = 0;
      }
		}
  }

  if(pd != 0)          // look for decimalpoint
  {
    hp += 1;
  }

  hp[0] = 0;           // final 0 (ascii-0-string)
  hp   -= 1;

  do
  {
    hp[0] = '0' + (_CHAR)(value % 10);

    hp    -= 1;
    pd    -= 1;

    if(dig > 0)
      dig -= 1;

    if(pd == 0)
    {
      hp[0] = dot;
      hp   -= 1;
    }

    value = value / 10;
    if((dig == 0) & (value != 0))
    {
      // value too big
//      pt[0]   = '?';
      value  = 0;
      toobig = _TRUE;
    }
  }
  while(value != 0);

  if(dig > 0)   // fill with space or preceding zero
  {
    do
    {
      if((format & 0x0100)||(pd >= 0))
        hp[0] = '0';
      else
        hp[0] = ' ';

      pd -= 1;
      hp -= 1;

			if(pd == 0)
			{
        hp[0] = dot;
        hp   -= 1;
      }

	    dig -= 1;
    }
	  while(dig != 0);
  }

  if((format & 0x0400) && (neg == _TRUE))
  {
    hp = &pt[1];
    while(*hp == ' ')
      hp++;
    pt[0] = ' ';
    hp -= 1;
    *hp = '-';
  }

  if(toobig == _TRUE)
  {
    hp = pt;
    while(*hp != 0)
    {
			if(*hp != dot)
        *hp = '>';
      hp++;
    }
  }


}

_DWORD AToDsub(void *pt, _UWORD chr_size, _UWORD *pcnt)
{
  _ASCII  *a, chr;
  _UNI    *c, uni;
  _BOOL   neg = _FALSE;
  _DWORD  retcode = 0;
  _BOOL   run = _TRUE;
  _UWORD  cnt = 0;

  if(chr_size == 2)
  {
    c = (_UNI*)pt;

    do
    {
      uni = *c;
      c  += 1;

      if(uni == '-')
      {
        neg = (neg == _TRUE)? _FALSE : _TRUE;
        cnt += 1;
      }
      else if((uni >= '0') && (uni <= '9'))
      {
        retcode = (retcode * 10) + (uni - '0');
        cnt += 1;
      }
      else
      {
        run = _FALSE;
      }
    }
    while(run == _TRUE);
  }
  else
  {
    a = (_ASCII*)pt;

    do
    {
      chr = *a;
      a  += 1;

      if(chr == '-')
      {
        neg = (neg == _TRUE)? _FALSE : _TRUE;
        cnt += 1;
      }
      else if((chr >= '0') && (chr <= '9'))
      {
        retcode = (retcode * 10) + (chr - '0');
        cnt += 1;
      }
      else
      {
        run = _FALSE;
      }
    }
    while(run == _TRUE);
  }

  if(neg == _TRUE)
    retcode = -retcode;

  *pcnt = cnt;

  return(retcode);
}

_DWORD AToD(void *pt, _UWORD chr_size)
{
  _UWORD cnt;
  return(AToDsub(pt, chr_size, &cnt));
}


void DToA(void *pt, _DWORD value, _UWORD format, _UWORD chr_size)
{
  // xxxx 0000 0000 0000 format
	//      .... .... **** no of digits
  //      .... **** .... position decimalpoint
  //      ...* .... .... preceding zeros
  //      ..*. .... .... descendant zeros
  //      .*.. .... .... vorzeichen unmittelbar vor wert (-3 ansonsten -  3)
  //      *... .... .... anzahl der stellen automatisch ermitteln

  _DWORD tmp;
  _UWORD nk, digs;

  if((format & 0x000F) == 0)
    format |= 0x0800;

  if(format & 0x0800)
  {
    tmp  = value;
		digs = (tmp == 0)? 1 : 0;

    while(tmp)
    {
      tmp = tmp / 10;
      digs++;
    }

    nk = ((format >> 4) & 0x0F);
    if(nk >= digs)
      digs = nk + 1;

    format = (format & 0xFFF0) | (digs & 0x000F);
  }

 #ifdef UC_UNICODE
	if(chr_size == sizeof(_UNI))
    dtoa_UNI((_UNI*)pt, value, format);
  else
 #else
	if(chr_size == sizeof(_ASCII))
 #endif
    dtoa_ASCII((_ASCII*)pt, value, format);
}

_BOOL add_hex(_UDWORD *val, _UWORD scancode)
{
  _BOOL retcode = _TRUE;

  switch(scancode)
  {
    case '0' :
    case '1' :
    case '2' :
    case '3' :
    case '4' :
    case '5' :
    case '6' :
    case '7' :
    case '8' :
    case '9' : *val = (*val) * 16 + (scancode - '0');
               break;
    case 'A' :
    case 'B' :
    case 'C' :
    case 'D' :
    case 'E' :
    case 'F' : *val = (*val) * 16 + (scancode - 'A' + 10);
               break;
    case 'a' :
    case 'b' :
    case 'c' :
    case 'd' :
    case 'e' :
    case 'f' : *val = (*val) * 16 + (scancode - 'a' + 10);
               break;
    default  : retcode = _FALSE;
               break;
  }

  return(retcode);
}

_UDWORD HToD(void *pt, _UWORD chr_size)
{
  _UBYTE *pa;
  _UWORD  no;
  _UDWORD retcode = 0;
  _UWORD  i;
  _ASCII  tmp[128];

  if(chr_size == sizeof(_UNI))
  {
    StrCpy(tmp, sizeof(_ASCII), pt, sizeof(_UNI));
    pt = tmp;
    chr_size = sizeof(_ASCII);
  }
  
  if(chr_size == sizeof(_ASCII))
  {
    pa = (_UBYTE*)pt;
    if(MemCmp(pa, "16#", 3) == 0)
      pa += 3;
    else if(MemCmp(pa, "0x", 2) == 0)
      pa += 2;

    no = StrLen(pa, chr_size);
    for(i=0; i<no; i++)
    {
      if(add_hex(&retcode, pa[i]) == _FALSE)
        break;
    }
  }

  return(retcode);
}

void DToH(void *pt, _DWORD value, _UWORD format, _UWORD chr_size)
{
	_DWORD tmp;
  _UWORD digs;

  if(format & 0x0800)
  {
    tmp  = value;
    digs = (tmp == 0)? 1 : 0;

    while(tmp)
    {
      tmp = tmp / 16;
      digs++;
    }

    format = (format & 0xFFF0) | (digs & 0x000F);
  }

 #ifdef UC_UNICODE
  if(chr_size == sizeof(_UNI))
    dtoh_UNI((_UNI*)pt, value, format);
  else
 #else
  if(chr_size == sizeof(_ASCII))
 #endif
    dtoh_ASCII((_ASCII*)pt, value, format);
}

_BOOL create_filename(_ASCII *dp, _ASCII *ne)
{
  _BOOL  retcode = _FALSE;
  _UWORD len;
  _UWORD i, begin, count;
  _ASCII tmp[10];
  _UWORD run, maxrun;
  _File  File;
  _ASCII xne[_MAX_PATHLENGTH];

  if((dp != NULL)&&(ne != NULL))
  {
    StrCpy(xne, sizeof(_ASCII), dp, sizeof(_ASCII));
    StrCat(xne, sizeof(_ASCII), ne, sizeof(_ASCII));
    len = StrLen(xne, sizeof(_ASCII));

    begin = len;
    for(i=0; i<len; i++)
    {
      if(xne[i] == '?')
      {
        begin = i;
        break;
      }
    }

    if(begin < len)
    {
      count = 0;
      for(i=begin; i<len; i++)
      {
        if(xne[i] == '?')
          count += 1;
        else
          break;
      }

      if(count > 0)
      {
        if(count > 5)
          count = 5;

        maxrun = 65535;
        switch(count)
        {
          case 1 : maxrun =    10; break;
          case 2 : maxrun =   100; break;
          case 3 : maxrun =  1000; break;
          case 4 : maxrun = 10000; break;
        }

        run = 0;
        do
        {
          DToA(tmp, run, 0x0100 | count, sizeof(_ASCII));
          MemCpy(&xne[begin], tmp, count);
          run += 1;
          if(run <= maxrun)
          {
            if(File.Open(LOAD, xne) == _TRUE)
            {
              File.Close();
            }
            else
            {
              run     = 0;
              retcode = _TRUE;
            }
          }
          else
          {
            run = 0;
          }
        }
        while(run);
      }

      if(retcode == _TRUE)
        StrCpy(ne, sizeof(_ASCII), &xne[StrLen(dp, sizeof(_ASCII))], sizeof(_ASCII));
    }
  }

  return(retcode);
}

#ifdef PGTEST
void Debug(_DWORD value, _UWORD no, _UWORD column)
{
  _ASCII  txt[100];
  _REGION act;
	_KOORD  debugy = 10;
  _ROOM   room;
  _UDWORD sst = ScreenStatePush();

 #ifdef OK_OFFSCREEN
  _NEWSCREEN acts;
  if(UseOffscreen == _TRUE)
  {
    GetScreen(&acts);
    ActivateScreen(&OnScreen);
  }
 #endif

  if(no != 0xFFFF)
    debugy = 10 + no * 10;

  act = GetActiveRegion();
  DeactivateRegion();

  DToA(txt, value, 0x0800, sizeof(_ASCII));
  StrCat(txt, sizeof(_ASCII), " (0x", sizeof(_ASCII));
  DToH(&txt[StrLen(txt, sizeof(_ASCII))], value, 0x0108, sizeof(_ASCII));
  StrCat(txt, sizeof(_ASCII), ")", sizeof(_ASCII));

//  DToH(txt, value, 0x0108, sizeof(_ASCII));
  Font.Set(255, NULL);

  GetScreenSize(&room);
  room.xy1.x = 10+column*160;
  room.xy1.y = debugy;
//  room.xy2.x = ; // wird bereits mit GetScreenSize gesetzt
  room.xy2.y = room.xy1.y + 10;

  OutTextXY(10+column*160, debugy, txt, T_COPY|T_SOLID|T_PROP, MakeColor_Makro(BLACK, WHITE), sizeof(_ASCII));

	ActivateRegion(act);

 #ifdef OK_OFFSCREEN
  if(UseOffscreen == _TRUE)
  {
    ActivateScreen(&acts);
  }
 #endif

  ScreenStatePop(sst, &room);
}

void Debug(const void *txt, _UWORD size, _UWORD no)
{
  _REGION act;
  _KOORD  debugy = 10;
  _UDWORD sst = ScreenStatePush();
  _ROOM   room;

 #ifdef OK_OFFSCREEN
  _NEWSCREEN acts;
  if(UseOffscreen == _TRUE)
  {
    GetScreen(&acts);
    ActivateScreen(&OnScreen);
  }
 #endif

  if(no != 0xFFFF)
    debugy = 10 + no * 10;

  act = GetActiveRegion();
  DeactivateRegion();

  Font.Set(255, NULL);
	OutTextXY(10, debugy, (void*)txt, T_COPY|T_SOLID|T_PROP, MakeColor_Makro(BLACK, WHITE), size);


  GetScreenSize(&room);
  room.xy1.x = 10;
  room.xy1.y = debugy;
//  room.xy2.x = ; // wird bereits mit GetScreenSize gesetzt
  room.xy2.y = debugy + 10;

  ActivateRegion(act);

 #ifdef OK_OFFSCREEN
  if(UseOffscreen == _TRUE)
  {
    ActivateScreen(&acts);
  }
 #endif

  ScreenStatePop(sst, &room);
}
#endif

#ifdef UC_MOUSE
void HideMouseCursor(void)
{
 #ifndef UC_ROTATE
  // bei Shadowframebuffer muss Mauspfeil nicht versteckt werden, da sowieso nur
  // auf virtuellem Screen ausgegeben wird. Mauspfeil befindet sich immer nur auf
  // sichtbaren Screen und die OpSys-funktion RefreshRectangle berücksichtigt die
  // Ausblendung von Mauspfeil --> fertig

  if(MouseHideState == 0) // test ob mauspfeil überhaupt sichtbar ist
    HideMouse(); // mauspfeil unsichtbar machen
  MouseHideState += 1;
 #endif
}

void ShowMouseCursor(void)
{
 #ifndef UC_ROTATE
  if(MouseHideState == 1) // test ob "letzter" mauspfeil unsichtbar
    ShowMouse(); // mauspfeil sichtbar machen
  MouseHideState -= 1;
 #endif
}

_UWORD IfNecHideMouseCursor(_ROOM *pr)
{
//  return(IfNecHideMouse(pr));
 #ifndef UC_ROTATE
  HideMouseCursor();
 #endif
  return(1);
}

void IfNecShowMouseCursor(_UWORD hide)
{
//  IfNecShowMouseCursor(hide);
 #ifndef UC_ROTATE
  if(hide != 0)
    ShowMouseCursor();
 #endif
}
#endif

_BOOL sort_xchange_ASCII(_ASCII *p0, _ASCII *p1)
{
  _UBYTE a0, a1;

 SXA_BEGIN:

  a0 = (_UBYTE)*p0;
  a1 = (_UBYTE)*p1;

  if(a1 < a0)
    return(_TRUE);

	if(a1 == a0)
  {
    if(a0 == 0)
      return(_FALSE);

    p0 ++;
    p1 ++;
    goto SXA_BEGIN; // sorry, ist aber schneller als rekursion
//    return(sort_xchange_ASCII(p0+1, p1+1));
  }

  return(_FALSE);
}

_WORD qsort_CHAR(_CHAR *p0, _CHAR *p1)
{
  _CHAR a0, a1;

 QSC_BEGIN:

  a0 = *p0;
  a1 = *p1;

  if(a1 < a0)
    return(1); // string1 kleiner string0

	if(a1 == a0)
  {
    if(a0 == 0)
      return(0);  // strings sind gleich

    p0 ++;
    p1 ++;
    goto QSC_BEGIN; // sorry, ist aber schneller als rekursion return(qsort_CHAR(p0+1, p1+1))
  }

  return(-1);
}

_WORD qsort_ASCII(_ASCII *p0, _ASCII *p1)
{
  _UBYTE a0, a1;

 SXA_BEGIN:

  a0 = (_UBYTE)*p0;
  a1 = (_UBYTE)*p1;

  if(a1 < a0)
    return(1);

	if(a1 == a0)
  {
    if(a0 == 0)
      return(-1);

    p0 ++;
    p1 ++;
    goto SXA_BEGIN; // sorry, ist aber schneller als rekursion return(sort_xchange_ASCII(p0+1, p1+1))
  }

  return(0);
}

_WORD textsortfunct(_SORTA *p1, _SORTA *p2)
{
  // liefert -1(a<b), 0(a=b) oder 1(a>b)

  _UBYTE *txt1 = (_UBYTE*)p1->label;
  _UBYTE *txt2 = (_UBYTE*)p2->label;

  while(1)
  {
    if(txt1[0] < txt2[0])
    {
      return -1;
    }
    else if(txt1[0] > txt2[0])
    {
      return 1;
    }
    else if(txt1[0] == 0)
    {
      return 0;
    }
    
    txt1 += 1;
    txt2 += 1;
  }
}

void sort_ASCII_inplace(_ASCII *src, _UWORD no)
{
  _ASCII tmp[512];
  _ASCII *p0, *p1;
  _UWORD i, l0, l1;

  if(no > 1)
  {
    no --;
    while(no)
    {
      p0 = src;
      l0 = StrLen(p0, sizeof(_ASCII)) + 1;
      for(i=0; i<no; i++)
			{
        p1 = p0 + l0;
        l1 = StrLen(p1, sizeof(_ASCII)) + 1;

        if(qsort_ASCII(p0, p1) > 0)
        {
          MemCpy(tmp, p0, l0);
          MemMove(p0, p1, l1);
          MemCpy(p0+l1, tmp, l0);
          p0 = p0 + l1;
        }
        else
        {
          l0 = l1;
          p0 = p1;
        }
      }
      no --;
    }
  }
}

void sort_ASCII(_ASCII *src, _UWORD no, _QSORTFUNCT fnct)
{
  if((no > 1)&&(src != NULL))
  {
    if(fnct == NULL)
    {
      fnct = (_QSORTFUNCT)textsortfunct;
    }
    
    _BOOL  state = _FALSE;
    _SORTA *psortarray = NULL;
    if(Memory2((void**)&psortarray, no * sizeof(_SORTA)) != 0)
    {
      _UDWORD i, gesamtlen = 0;
      _ASCII *pa = src;
      _SORTA *ps = psortarray;
      for(i=0; i<no; i++, ps++)
      {
        ps->length = StrLen(pa, sizeof(_ASCII)) + 1;
        ps->label  = pa;
        pa         += ps->length;
        gesamtlen  += ps->length;
      }
      
      _ASCII *pnewbuffer = NULL;
      if(Memory2((void**)&pnewbuffer, gesamtlen) != 0)
      {
        state = _TRUE;
        qsort(psortarray, no, sizeof(_SORTA), fnct);
        
        ps = psortarray;
        pa = pnewbuffer;
        for(i=0; i<no; i++, ps++)
        {
          MemCpy(pa, ps->label, ps->length);
          pa += ps->length;
        }
        
        MemCpy(src, pnewbuffer, gesamtlen);
        Memory2((void**)&pnewbuffer, 0);
      }
      Memory2((void**)&psortarray, 0);
    }
    
    if(state == _FALSE)
    {
      // es konnte nicht genug speicher allokiert werden --> sort inplace (langsam)
      sort_ASCII_inplace(src, no);
    }
  }
}

// **************** SORT **********************

/*
void xchange_1(_UBYTE *p1, _UBYTE *p2, _UWORD size)
{
  _UBYTE tmp = *p1;
  *p1 = *p2;
  *p2 = tmp;
}

void xchange_2(_UWORD *p1, _UWORD *p2, _UWORD size)
{
  _UWORD tmp = *p1;
  *p1 = *p2;
  *p2 = tmp;
}

void xchange_4(_UDWORD *p1, _UDWORD *p2, _UWORD size)
{
  _UDWORD tmp = *p1;
  *p1 = *p2;
  *p2 = tmp;
}

void xchange_x(void *el1, void *el2, _UWORD size)
{
	_BYTE  tmp;
	_UWORD i;

	for(i=0; i<size; i++)
	{
		tmp = *((_BYTE*)el1 + i);
		*((_BYTE*)el1 + i) = *((_BYTE*)el2 + i);
		*((_BYTE*)el2 + i) = tmp;
	}
}

void bubblesort(void *source, _UDWORD anz, _UWORD size, _SORTFUNCT fnkt)
{
  _UBYTE       *small, *start, *next;
  _UDWORD      i, j;
  _XCHANGE_FKT xcf;

  if((anz > 1) && (size > 0))
  {
    switch(size)
    {
      case  1 : xcf = (_XCHANGE_FKT)xchange_1; break;
      case  2 : xcf = (_XCHANGE_FKT)xchange_2; break;
      case  4 : xcf = (_XCHANGE_FKT)xchange_4; break;
      default : xcf = (_XCHANGE_FKT)xchange_x; break;
    }

		anz--;
    start = (_UBYTE*)source;
    for(j=0; j<anz; j++)
    {
      small = start;           // init
      next  = start + size;

      for(i=j; i<anz; i++)
      {
        if((_WORD)fnkt(small, next) > 0)
          small = next;
        next += size;
      }

      xcf(start, small, size); // xchange
      start += size;
    }
  }
}
*/

void quicksort(void *src, _DWORD left, _DWORD right, _UWORD size, _QSORTFUNCT fnkt, _DWORD max)
{
  _DWORD i, j;
  _UBYTE x[30], y[30];
  _UBYTE *pi, *pj;
  _UBYTE *p = (_UBYTE*)src;

  i = left;
  j = right;
  MemCpy(x, p+((left+right)/2)*size, size);

  do
  {
    while((i<max)&&(fnkt(p+i*size, x) < 0))
      i += 1;
    while((j>0)&&(fnkt(p+j*size, x) > 0))
      j -= 1;

    if(i <= j)
    {
      pi = p + i*size;
      pj = p + j*size;

      if(size == 2)
      {
        *(_UWORD*)y  = *(_UWORD*)pi;
        *(_UWORD*)pi = *(_UWORD*)pj;
        *(_UWORD*)pj = *(_UWORD*)y;
      }
      else if(size == 4)
      {
        *(_UDWORD*)y  = *(_UDWORD*)pi;
        *(_UDWORD*)pi = *(_UDWORD*)pj;
        *(_UDWORD*)pj = *(_UDWORD*)y;
      }
      else
      {
        MemCpy(y,  pi, size);
        MemCpy(pi, pj, size);
        MemCpy(pj, y,  size);
      }

      i += 1;
      j -= 1;
    }
  }
  while(i <= j);

  if(left < j)
    quicksort(p, left, j, size, fnkt, max);
  if(right > i)
    quicksort(p, i, right, size, fnkt, max);
}

_BOOL heapsort(void *source, _DWORD n, _UWORD size, _SORTFUNCT fnkt)
{
  _UBYTE *val = NULL;
  _DWORD parent, child, w, max, i;
  _DWORD m = (n + 6) / 8;
  _UBYTE *hp, *data = (_UBYTE*)source;
  _BOOL  retcode = _FALSE;

  PushMemColor(MEMORY_PRG);
  if(Memory2((void**)&val, size) != 0)
  {
    retcode = _TRUE;

    for(;;)
    {
      if(m)
      {
        m     -= 1;
        parent = m;
        MemCpy(val, data + (parent*size), size);
      }
      else
      {
        n -= 1;
        if(n)
        {
          hp = data + (n * size);
          MemCpy(val, hp, size);
          MemMove(hp, data, size);
          parent  = 0;
        }
        else
        {
          break;
        }
      }

      child = parent * 8 + 1;
      while(child < n)
      {
        w = n - child;
        if(w >= 8)
          w = 8;

        max = 0;
        for(i=1; i<w; i++)
          if(fnkt(data+((child+i)*size), data+(child+max)*size) != 0) // abfrage a > b
            max= i;

        child += max;

        if(fnkt(data+(child*size), val) == 0)  // abfrage a <= b
          break;

        MemMove(data+(parent*size), data+(child*size), size);
        parent = child;
        child  = parent * 8 + 1;
      }

      MemCpy(data+(parent*size), val, size);
    }

    Memory2((void**)&val, 0);
  }
  PopMemColor();

  return(retcode);
}

_BOOL qheapsort(void *source, _DWORD n, _UWORD size, _QSORTFUNCT fnkt)
{
  _UBYTE *val = NULL;
  _DWORD parent, child, w, max, i;
  _DWORD m = (n + 6) / 8;
  _UBYTE *hp, *data = (_UBYTE*)source;
  _BOOL  retcode = _FALSE;

  PushMemColor(MEMORY_PRG);
  if(Memory2((void**)&val, size) != 0)
  {
    retcode = _TRUE;

    for(;;)
    {
      if(m)
      {
        parent = --m;
        MemCpy(val, data + (parent*size), size);
      }
      else if(--n)
      {
        hp = data + (n * size);
        MemCpy(val, hp, size);
        MemMove(hp, data, size);
        parent  = 0;
      }
      else
      {
        break;
      }

      while((child = parent * 8 + 1) < n)
      {
        w = n - child < 8 ? n - child : 8;

        for(max=0, i=1; i<w; ++i)
          if(fnkt(data+((child+i)*size), data+(child+max)*size) > 0) // abfrage a > b
            max = i;

        child += max;

        if(fnkt(data+(child*size), val) <= 0)  // abfrage a <= b
          break;

        MemCpy(data+(parent*size), data+(child*size), size);
        parent = child;
      }

      MemCpy(data+(parent*size), val, size);
    }
    Memory2((void**)&val, 0);
  }
  PopMemColor();

  return(retcode);
}

void sort(void *src, _UDWORD no, _UWORD size, _SORTFUNCT fnkt)
{
  if((no > 1)&&(src != NULL)&&(size > 0)&&(fnkt != NULL))
    heapsort(src, no, size, fnkt);
}

void qsort(void *src, _UDWORD no, _UWORD size, _QSORTFUNCT fnkt)
{
  // quicksort belastet stack (rekursiv), deshalb nur elemente < 30 byte
  if((no > 1)&&(src != NULL)&&(size > 0)&&(fnkt != NULL))
  {
    if((no < 4)||(no > 16000)||(size >= 30))
      qheapsort(src, no, size, fnkt);
    else
      quicksort(src, 0, (_DWORD)no-1, size, fnkt, (_DWORD)no-1);
  }
}

/*
void sort(void *src, _UDWORD no, _UWORD size, _SORTFUNCT fnkt)
{
  if((no > 1)&&(src != NULL)&&(size > 0)&&(fnkt != NULL))
    bubblesort(src, no, size, fnkt);
}
*/

_UDWORD wpos_RINGBUFFER(_RINGBUFFER *p)
{
  _UDWORD retcode = 0;

  if(p->no > 0)
  {
    retcode = p->pos;

    if(p->used == _TRUE)
      if(p->pos == p->begin)
        p->begin = (p->pos + 1) % p->no;

    p->used = _TRUE;
    p->pos++;
    if(p->pos >= p->no)
      p->pos = 0;
  }

  return(retcode);
}

_UDWORD no_RINGBUFFER(_RINGBUFFER *p)
{
  _UDWORD retcode = 0;

  if((p->no > 0)&&(p->used == _TRUE))
  {
    if(p->begin == p->pos)
      retcode = p->no;
    else
      retcode = ((p->pos + p->no - p->begin) % p->no);
  }

  return(retcode);
}

_UDWORD rpos_RINGBUFFER(_RINGBUFFER *p, _UDWORD i)
{
  _UDWORD retcode = 0;

  if(i < no_RINGBUFFER(p))
    retcode = (p->begin + i) % p->no;

  return(retcode);
}

_UDWORD teiler_RINGBUFFER(_RINGBUFFER *p, _UDWORD *a1, _UDWORD *a2)
{
  _UDWORD anz;

  *a1 = 0;
  *a2 = 0;

  anz = no_RINGBUFFER(p);
  if(anz > 0)
  {
    if((p->begin + anz) <= p->no)
    {
      *a2 = anz;
    }
    else
    {
      *a2 = p->no - p->begin;
      *a1 = anz - *a2;
    }
  }

  return(p->begin);
}

void sort_ringbuffer(void *source, _RINGBUFFER *rb, _UWORD size, _SORTFUNCT fnkt)
{
  _UDWORD no;
  _UDWORD end, i;
  _UBYTE  *begin, *a, *b;
  _UBYTE  reserve[1000];

  if((source != NULL)&&(rb != NULL)&&(size > 0)&&(size <= sizeof(reserve))&&(fnkt != NULL))
  {
    no = no_RINGBUFFER(rb);
    if(no > 1)
    {
      begin = (_UBYTE*)source;
      end   = no - 1;

      do
      {
        for(i=0; i<end; i++)
        {
          a = begin + (rpos_RINGBUFFER(rb, i)   * size);
          b = begin + (rpos_RINGBUFFER(rb, i+1) * size);

          if(fnkt(a, b) != 0)
          {
            MemCpy(reserve, a, size);
            MemCpy(a, b, size);
            MemCpy(b, reserve, size);
          }
        }
        end --;
      }
      while(end > 0);
    }
  }
}


// ************************************************

void* find_this_method(void *pthis, _UWORD no)
{
  _UBYTE *pmeth;

  if(pthis != NULL)
  {
    pmeth = (_UBYTE*)*(void**)pthis;
    return(*(void**)(pmeth + (no * 4)));
  }
  return(NULL);
}

void* find_lasal_method(void *pthis, _UWORD no)
{
  if(pthis != NULL)
    return(find_this_method(pthis, no+10));

	return(NULL);
}

void* GetVirtualMethodPointer(void *pthis, _UDWORD vmno)
{
  if(vmno != 0xFFFFFFFF)
    return find_this_method(pthis, vmno);
  return NULL;
}

_UDWORD GetVirtualMethodNumber(void *pthis, void *pvm)
{
  if((pthis == NULL) || (pvm == NULL))
    return 0xFFFFFFFF;
    
  _UDWORD retcode = 0;
  _UBYTE *pmeth = (_UBYTE*)*(void**)pthis;
    
  while (pvm != *(void**)pmeth)
  {
    pmeth += 4;
    retcode ++;
    if(retcode >= 96)
      return 0xFFFFFFFF;
  }
  
  return retcode;
}

#ifdef OK_ENCLOSE_NAME
_DOIT call_enclosed_name_method(_UWORD method, _IO *pio, _VARIABLE *pv, _RESULT *pr, _EDITOR *ps, _EVENT *pe, _BOOL input)
{
  void  *ptr;
  _BOOL st;
  _DOIT retcode = _IDLE;

  // kein objekt definiert ..................................... retcode = _IDLE
  // defineierts objekt nicht gefunden ......................... retcode = _IFAILED
  // procedure bereits abgearbeitet, arbeit bereits erledigt ... retcode = _IDIDIT
  // fehler in überlagerter procedure .......................... retcode = _IFAILED

  if(pio->virtname.pthis != NULL)
  {
   #ifdef LASAL
    void *tmp = pio->virtname.pthis;
   #endif

    ptr = find_lasal_method(pio->virtname.pthis, method);

    if(ptr != NULL)
    {
      switch(method)
			{
        case METHOD_MYIO_ACCESSIBLE :
            load_this(tmp);
            st = ((_FBOOL_PT)ptr)((void*)pio);
            retcode = (st == _TRUE)? _IDLE : _IFAILED;
            break;
        case METHOD_MYIO_GETFOCUS :
            load_this(tmp);
            retcode = ((_FDOIT_PT)ptr)((void*)pio);
            break;
        case METHOD_MYIO_KILLFOCUS :
            load_this(tmp);
            retcode = ((_FDOIT_PT)ptr)((void*)pio);
            break;
        case METHOD_MYIO_GETEVENT :
            load_this(tmp);
            retcode = ((_FDOIT_PT_PT)ptr)((void*)ps, (void*)pe);
            break;
        case METHOD_MYIO_DRAW :
            load_this(tmp);
            retcode = ((_FDOIT_PT_PT_PT_BOOL)ptr)((void*)pio, (void*)pr, (void*)pv, input);
            break;
				case METHOD_MYIO_GETDATA :
            load_this(tmp);
            retcode = ((_FDOIT_PT_PT_PT_BOOL)ptr)((void*)pr, (void*)pio, (void*)pv, input);
            break;
        case METHOD_MYIO_READY :
            load_this(tmp);
            ((_FVOID_PT_BOOL)ptr)(pio, input);
            retcode = _IDLE;
            break;
        case METHOD_MYIO_RUN :
            load_this(tmp);
            ((_FVOID_PT_BOOL)ptr)(pio, input);
            break;
        case METHOD_MYIO_SAVEEDITOR :
            load_this(tmp);
            retcode = ((_FDOIT_PT)ptr)(ps);
            break;
        case METHOD_MYIO_DISPLAYCURSOR :
            load_this(tmp);
            retcode = ((_FDOIT_VOID)ptr)();
            break;
        case METHOD_MYIO_SYSTEMISREADYWITHEDITOR :
            load_this(tmp);
            ((_FVOID_PT_PT)ptr)(ps, pe);
            break;

      }
    }
  }

  return(retcode);
}
#endif

#define CH_OBJ       0
#define CH_CMD       1
#define CH_SVR       2
#define CH_CLT_CMD   3
#define CH_CLT_DATA  4
#define CH_CLT_OBJ   5
#define CH_OBJ_DIR   6
#define CH_CNST      7
#define CH_NONE      8
void *get_this_pointer(_ASCII *objectname0)
{
  void     *retcode = NULL;
  void     *pobj_channel;
  void     *pobj;
  _UDWORD   chmode;
  _FPT_VOID fpt;

  if(objectname0 != NULL)
  {
    if(objectname0[0] != 0)
    {
      if(LookUpEmbeddedPointer == NULL)
      {
				fpt = (_FPT_VOID)find_lasal_method(LseThisPointer, METHOD_LSE_GETLOOKUPEMB);
        if(fpt != NULL)
          LookUpEmbeddedPointer = (_PT_LOOKUPEMBEDDED)fpt();
      }

      if(LookUpEmbeddedPointer != NULL)
      {
        chmode = LookUpEmbeddedPointer((void*)objectname0, (void*)&pobj, (void*)&pobj_channel);

        switch(chmode)
        {
          case CH_OBJ      :
          case CH_CMD      :
          case CH_SVR      :
          case CH_CNST     : retcode = pobj_channel; break;
//        case CH_CLT_CMD  :
//        case CH_CLT_DATA :
//        case CH_CLT_OBJ  :
//        case CH_OBJ_DIR  :
//        case CH_NON      :
        }
      }
    }
  }

	return(retcode);
}

void lasal_to_DATE(_DATE *d, _IVALUE src)
{
  // JJJJJJJJJJJJJJJJ MMMMTTTTTTTTWWWW
  _UDWORD  tmp = src;

 #ifdef EDIT_DEFAULT_TIMEDATE
  if(src == -1)
  {
    d->dayofweek = 0xFF;
    d->day       = 0xFF;
	d->month     = 0xFF;
	d->year      = 0xFFFF;
	return;
  }
 #endif

  d->dayofweek = ((tmp & 0x0F) % 7);
  d->day       = ((tmp >> 4) & 0xFF);
  d->month     = ((tmp >> 12) & 0x0F);
  d->year      = ((tmp >> 16) & 0xFFFF);
}

void DATE_to_lasal(_IVALUE *dest, _DATE *src)
{
  // JJJJJJJJJJJJJJJJ MMMMTTTTTTTTWWWW
  _UDWORD  tmp;

 #ifdef EDIT_DEFAULT_TIMEDATE
  if((src->day == 0xFF)&&(src->month == 0xFF)&&(src->year == 0xFFFF))
  {
    *dest = -1;
	return;
  }
 #endif

  src->dayofweek = day_of_week(src);
  if(src->month < 1)  src->month = 1;
  if(src->month > 12) src->month = 12;
  if(src->day < 1)    src->day = 1;
  if(src->day > 31)   src->day = 31;

  tmp   = src->year;
  tmp   = (tmp << 4) | src->month;
	tmp   = (tmp << 8) | src->day;
  tmp   = (tmp << 4) | src->dayofweek;
  *dest = tmp;
}

void lasal_to_TIME(_TIME *dst, _IVALUE src)
{
  // HHHHHHHHMMMMMMMM SSSSSSSS00000000
  _UDWORD tmp   = src;

 #ifdef EDIT_DEFAULT_TIMEDATE
  if(src == -1)
  {
    dst->hour   = 0xFF;
    dst->minute = 0xFF;
    dst->second = 0xFF;
    return;
  }
 #endif

  dst->hour     = ((tmp >> 24) & 0xFF);
  dst->minute   = ((tmp >> 16) & 0xFF);
  dst->second   = ((tmp >>  8) & 0xFF);
}

void TIME_to_lasal(_IVALUE *dst, _TIME *src)
{
  // HHHHHHHHMMMMMMMM SSSSSSSS00000000
  _UDWORD tmp;

 #ifdef EDIT_DEFAULT_TIMEDATE
  if((src->hour == 0xFF)&&(src->minute == 0xFF)&&(src->second == 0xFF))
  {
    *dst = -1;
	return;
  }
 #endif

  tmp  = (src->hour % 24);
 #ifdef USER_DEKEMA
  // bei dekema können bei MMSS 0-99 minuten eingegeben werden
  tmp  = (tmp << 8) | (src->minute % 100);
 #else
  tmp  = (tmp << 8) | (src->minute % 60);
 #endif
  tmp  = (tmp << 8) | (src->second % 60);
  *dst = (tmp << 8);
}

void ActivateScreen(_NEWSCREEN *p)
{
//  Font.Destroy();
 #ifdef OK_OFFSCREEN
  if(UseOffscreen == _TRUE)
  {
	  SetScreen(p);
	  if(p->ptr != OnScreen.ptr)
		  ActivateRegion(NULL);
  }
 #endif
}

_ATTRIB SetOutlineAttrib(_ATTRIB att)
{
	return((att & (T_SIZE|T_COMBI|T_OUTLINE|T_ROUND)) | T_SOLID);
}

// LASAL interface ***********************************************************

extern "C" void Lse_DToH(void *pt, _DWORD value, _UWORD format, _UWORD chr_size)
{
	DToH(pt, value, format, chr_size);
}
extern "C" void Lse_DToA(void *pt, _DWORD value, _UWORD format, _UWORD chr_size)
{
  DToA(pt, value, format, chr_size);
}
extern "C" _UWORD Lse_StrLen(void *txt, _UWORD chr_size)
{
  return(StrLen(txt, chr_size));
}
extern "C" void Lse_StrCpy(void *dest, _UWORD dest_size, void *src, _UWORD src_size)
{
  StrCpy(dest, dest_size, src, src_size); 
}
extern "C" void Lse_StrCat(void *p1, _UWORD size1, void *p2, _UWORD size2)
{
  StrCat(p1, size1, p2, size2);
}
extern "C" _UWORD Lse_StrCmp(const void *p1, _UWORD size1, const void *p2, _UWORD size2)
{
  return(StrCmp(p1, size1, p2, size2));
}
extern "C" _UWORD Lse_StrSize(void *p, _UWORD chr_size)
{
  return(StrSize(p, chr_size));
}
extern "C" _UWORD Lse_StrChr(void *p, _UNI c, _UWORD chr_size)
{
  return(StrChr(p, c, chr_size));
}
extern "C" void Lse_StrUp(void *p, _UWORD chr_size, _UWORD len)
{
  StrUp(p, chr_size, len);
}
extern "C" void Lse_OutParaString(_ROOM *pr, _CHAR *pt, _MEM_VARLABEL *pv, _FONT font, _ATTRIB att, _COLOR col)
{
  _CHAR *txt      = make_parameterstring(pt, pv, _TRUE);
  _COLOR coltxt   = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(col));
  _COLOR colback  = MakeColor_Makro(GetBackColor_Makro(col), GetBackColor_Makro(col));
  _COLOR colframe = MakeColor_Makro(INVISIBLE, INVISIBLE);
  _FRAMETYPE frametype = _3DFRAME;

  if(txt != NULL)
	  OutMultiTextAlign(txt, 0, pr, font, att, frametype, coltxt, colback, colframe, sizeof(_CHAR));
}
void Lse_OutParameterString(_ROOM room, _CHAR *pt, _MEM_VARLABEL *pv, _FONT font, _ATTRIB att, _COLOR col)
{
  Lse_OutParaString(&room, pt, pv, font, att, col);
}
extern "C" void Lse_OutTextAlign(_CHAR *txt, _ROOM *pr, _FONT font, _ATTRIB att, _COLOR col)
{
  _COLOR coltxt   = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(col));
  _COLOR colback  = MakeColor_Makro(GetBackColor_Makro(col), GetBackColor_Makro(col));
  _COLOR colframe = MakeColor_Makro(INVISIBLE, INVISIBLE);

  OutTxtAlign(txt, pr, font, att, coltxt, colback, colframe, sizeof(_CHAR));
}
extern "C" void uni_to_ascii(_ASCII *dest, _UNI *src, _UDWORD no)
{
  UniToAscii(dest, src, no);
}
extern "C" void ascii_to_uni(_UNI *dest, _ASCII *src, _UDWORD no)
{
  AsciiToUni(dest, src, no);
}
extern "C" void lse_format_value(_CHAR *txt, _IVALUE value, _VARINFO *pi, _PICFORMAT *pf)
{
  format_value(txt, value, pi, pf);
}

void delete_IO_from_screen(_IO *pio)
{
  _REGION actreg, ioreg=NULL, newreg=NULL;

  if(pio->cstate.init == _TRUE)
  {
   #ifdef UC_MOUSE
    HideMouseCursor();
   #endif

//    actreg = ActivateAndRegion(&ioreg, &newreg, &pio->space, pio->attrib);
    actreg = ActivateAndRegion(&ioreg, &newreg, &pio->space);
    if(pio->image.ptr != NULL)
    {
      PutImage(&pio->image, DEFKOORD, DEFKOORD);
    }

    DeactivateAndRegion(actreg, &ioreg, &newreg);

   #ifdef UC_MOUSE
    ShowMouseCursor();
   #endif
  }
}


_BOOL xchange_VARLABEL(_VARLABEL *pvl, _UDWORD old, _UDWORD varno)
{
  _BOOL retcode = _FALSE;

  if(pvl->no > 0)
  {
    if((pvl->info[0].state == VAR_VAL)&&(pvl->info[0].value == old))
    {
      pvl->info[0].value = varno;
      retcode = _TRUE;
    }
  }

  return(retcode);
}

_BOOL change_input_SubPicture(_SubPicture*, _UDWORD, _UDWORD);

_BOOL change_input_PICTUREMASK(_PICTUREMASK *p, _UDWORD old, _UDWORD varno)
{
  _BOOL  retcode = _FALSE;
  _UWORD i;
  _IO    *pio;

  for(i=0; i<p->meminput.no; i++)
  {
    pio = &p->meminput.ptr[i].io;
    if(xchange_VARLABEL(&pio->variable, old, varno) == _TRUE)
    {
      init_CSTATE(&pio->cstate);
      if(varno >= VarList.GetNo())
        delete_IO_from_screen(pio);
      retcode = _TRUE;
    }
  }

  for(i=0; i<p->memoutput.no; i++)
  {
    pio = &p->memoutput.ptr[i];
    if(pio->Obj != NULL)
      change_input_PICTUREMASK(&pio->Obj->GetPICTURE()->mask, old, varno);
  }

  for(i=0; i<p->memobject.no; i++)
    change_input_SubPicture(p->memobject.ptr[i].SubPicture, old, varno);

  for(i=0; i<p->memmotion.no; i++)
    change_input_SubPicture(p->memmotion.ptr[i].SubPicture, old, varno);

  return(retcode);
}

_BOOL change_input_SubPicture(_SubPicture *p, _UDWORD old, _UDWORD varno)
{
  _PICTURE *ppic;
  _BOOL    retcode = _FALSE;

  if(p != NULL)
  {
    ppic = p->GetPICTURE();
    retcode = change_input_PICTUREMASK(&ppic->mask, old, varno);
  }

  return(retcode);
}

_BOOL change_input(_MY_PICTURE *p, _UDWORD old, _UDWORD varno)
{
  _UWORD   i;
  _PICTURE *ppic;
  _BOOL    retcode = _FALSE;

  if(old != varno)
  {
		for(i=0; i<p->No; i++)
    {
      ppic = p->Ptr[i].GetPICTURE();
      if(change_input_PICTUREMASK(&ppic->mask, old, varno) == _TRUE)
        retcode = _TRUE;
    }
  }

  return(retcode);
/*
  _UWORD      i, j;
  _PICTURE    *ppic;
  _MEM_INPUT  *pmi;
  _IO         *pio;
  _VARLABEL   *pvl;
  _BOOL       retcode = _FALSE;

  if(old != varno)
  {
		for(i=0; i<p->No; i++)
    {
      ppic = p->Ptr[i].GetPICTURE();
      pmi  = &ppic->mask.meminput;

      for(j=0; j<pmi->no; j++)
      {
        pio = &pmi->ptr[j].io;
        pvl = &pio->variable;

        if(pvl->no > 0)
        {
          if((pvl->info[0].state == VAR_VAL)&&(pvl->info[0].value == old))
          {
            pvl->info[0].value = varno;
            init_CSTATE(&pio->cstate);
            if(varno >= VarList.GetNo())
              delete_IO_from_screen(pio);

            retcode = _TRUE;
          }
        }
      }
    }
  }

  return(retcode);
*/
}

_BOOL change_text_sub(_PICTEXT *ptx, _UDWORD old, _UDWORD varno)
{
  _BOOL retcode = _FALSE;

  switch(ptx->anypara.anything.list)
  {
    case VARTXT1_TOKEN:
    case VARTXT1:       // variablentext 01
    case VARTXT2:       // variablentext 02
    case VARTXT3:       // variablentext 03
    case VARTXT4:       // bubblehelptext
    case VARNAME:       // name of variable
    case VARUNIT:       // unittext via server
                  if(ptx->anypara.anything.no == old)
                  {
                    retcode = _TRUE;
                    ptx->anypara.anything.no = varno;
                    ptx->oldcrc += 1;
                  }
                  break;
    default     : break;
  }

  return(retcode);
}

_BOOL change_text_SubPicture(_SubPicture*, _UDWORD, _UDWORD);
_BOOL change_text_PICTUREMASK(_PICTUREMASK *p, _UDWORD old, _UDWORD varno)
{
  _BOOL  retcode = _FALSE;
  _UWORD i;

  for(i=0; i<p->mempictxt.no; i++)
  {
    if(change_text_sub(&p->mempictxt.ptr[i], old, varno) == _TRUE)
      retcode = _TRUE;
  }

  for(i=0; i<p->colpictxt.no; i++)
  {
    if(change_text_sub(&p->colpictxt.ptr[i].btx, old, varno) == _TRUE)
      retcode = _TRUE;
  }

  for(i=0; i<p->memobject.no; i++)
    change_text_SubPicture(p->memobject.ptr[i].SubPicture, old, varno);

  for(i=0; i<p->memmotion.no; i++)
    change_text_SubPicture(p->memmotion.ptr[i].SubPicture, old, varno);

  return(retcode);
}

_BOOL change_text_SubPicture(_SubPicture *p, _UDWORD old, _UDWORD varno)
{
  _PICTURE *ppic;
  _BOOL    retcode = _FALSE;

  if(p != NULL)
  {
    ppic = p->GetPICTURE();
    retcode = change_text_PICTUREMASK(&ppic->mask, old, varno);
  }

  return(retcode);
}


_BOOL change_output_SubPicture(_SubPicture*, _UDWORD, _UDWORD);
_BOOL change_output_PICTUREMASK(_PICTUREMASK *p, _UDWORD old, _UDWORD varno)
{
  _BOOL  retcode = _FALSE;
  _UWORD i;
  _IO    *pio;

  for(i=0; i<p->memoutput.no; i++)
  {
    pio = &p->memoutput.ptr[i];
    if(xchange_VARLABEL(&pio->variable, old, varno) == _TRUE)
    {
      init_CSTATE(&pio->cstate);
      if(varno >= VarList.GetNo())
        delete_IO_from_screen(pio);
      retcode = _TRUE;
    }

    if(pio->Obj != NULL)
      change_output_PICTUREMASK(&pio->Obj->GetPICTURE()->mask, old, varno);
  }

  for(i=0; i<p->memobject.no; i++)
    change_output_SubPicture(p->memobject.ptr[i].SubPicture, old, varno);

  for(i=0; i<p->memmotion.no; i++)
    change_output_SubPicture(p->memmotion.ptr[i].SubPicture, old, varno);

  for(i=0; i<p->membutton.no; i++)
    change_output_SubPicture(p->membutton.ptr[i].SubPicture, old, varno);

  return(retcode);
}


_BOOL change_output_SubPicture(_SubPicture *p, _UDWORD old, _UDWORD varno)
{
  _PICTURE *ppic;
  _BOOL    retcode = _FALSE;

  if(p != NULL)
  {
    ppic = p->GetPICTURE();
    retcode = change_output_PICTUREMASK(&ppic->mask, old, varno);
  }

  return(retcode);
}

_BOOL change_output(_MY_PICTURE *p, _UDWORD old, _UDWORD varno)
{
  _UWORD   i;
  _PICTURE *ppic;
  _BOOL    retcode = _FALSE;

  if(old != varno)
  {
		for(i=0; i<p->No; i++)
    {
      ppic = p->Ptr[i].GetPICTURE();
      if(change_output_PICTUREMASK(&ppic->mask, old, varno) == _TRUE)
        retcode = _TRUE;
    }
  }

  return(retcode);
}

extern "C" _BOOL get_editor_state(_MY_PICTURE *p)
{
  return(is_SEEKELEMENT(p, p->Editor.actno));
}

extern "C" _UDWORD get_editor_varno(_MY_PICTURE *p)
{
  _UDWORD retcode = 0xFFFF;

  if(is_SEEKELEMENT(p, p->Editor.actno) == _TRUE)
  {
    retcode = p->Editor.variable.no;
  }

  return(retcode);
}

extern "C" void OutTxtAlign(void *txt, _ROOM *pr, _FONT font, _ATTRIB att, _COLOR coltxt, _COLOR colback, _COLOR colframe, _UWORD chr_size)
{
  _FRAMETYPE frametype = _3DFRAME;


	OutTextAlign(txt, 0, pr, font, att, frametype, coltxt, colback, colframe, chr_size);

}

extern "C" void OutMultiTxtAlign(void *txt, _ROOM *pr, _FONT font, _ATTRIB att, _COLOR coltxt, _COLOR colback, _COLOR colframe, _UWORD chr_size)
{
  _FRAMETYPE frametype = _3DFRAME;

	OutMultiTextAlign(txt, 0, pr, font, att, frametype, coltxt, colback, colframe, chr_size);
}

void selfmade_indicator(_ROOM *pr, _DWORD angle, _COLOR col, _DOT *dot, _UWORD no)
{
  _DOT   *p;
  _UWORD i;
  _ZOOM  zoom;
  _KOORD x, y;

  linear_coordinates(&pr->xy1.x, &pr->xy1.y, &pr->xy2.x, &pr->xy2.y);
  zoom.y = (pr->xy2.y - pr->xy1.y) * 5;
  zoom.x = (pr->xy2.x - pr->xy1.x) * 5;
  x = (pr->xy2.x + pr->xy1.x + 1)/ 2;
  y = (pr->xy2.y + pr->xy1.y + 1)/ 2;

  p = dot;
  for(i=0; i<no; i++)
  {
    rotate_DOT(p, angle);
    p->x = Zoom(zoom.x, p->x) + x;
    p->y = Zoom(zoom.y, p->y) + y;
    p ++;
  }

  if(GetPenColor_Makro(col) != INVISIBLE)
    Polygon(no, dot, T_COPY|T_SOLID|T_FILL, col);

  if(GetBackColor_Makro(col) != INVISIBLE)
    Polygon(no, dot, T_COPY|T_SOLID, GetBackColor_Makro(col));
}

/*
void selfmade_indicator(_ROOM *pr, _DWORD angle, _COLOR col, _DOT *dot, _UWORD no)
{
  _DOT   *p;
  _UWORD i;
  _ZOOM  zoom;
  _KOORD x, y;
  _DOT3D d3d;


  linear_coordinates(&pr->xy1.x, &pr->xy1.y, &pr->xy2.x, &pr->xy2.y);
  zoom.y = (pr->xy2.y - pr->xy1.y) * 5;
  zoom.x = (pr->xy2.x - pr->xy1.x) * 5;
  x = (pr->xy2.x + pr->xy1.x + 1)/ 2;
  y = (pr->xy2.y + pr->xy1.y + 1)/ 2;

  p = dot;
  for(i=0; i<no; i++)
  {
    d3d.x = p->x;
    d3d.y = p->y;
    d3d.z = 0;
    rotate_DOT3D(&d3d, angle, 0, 0);
    p->x = d3d.x;
    p->y = d3d.y;

    p->x = Zoom(zoom.x, p->x) + x;
    p->y = Zoom(zoom.y, p->y) + y;
    p ++;
  }

  if(GetPenColor_Makro(col) != INVISIBLE)
    Polygon(no, dot, T_COPY|T_SOLID|T_FILL, col);

  if(GetBackColor_Makro(col) != INVISIBLE)
    Polygon(no, dot, T_COPY|T_SOLID, GetBackColor_Makro(col));
}
*/


void indicator(_ROOM *pr, _DWORD angle, _COLOR col)
{
  _DOT dot[10];

  dot[0].x =   -5;
  dot[0].y =    0;
  dot[1].x =  -15;
  dot[1].y =   -4;
  dot[2].x =   90;
  dot[2].y =    0;
  dot[3].x =  -15;
  dot[3].y =    4;
  dot[4]   = dot[0];

  selfmade_indicator(pr, angle, col, dot, 5);
  
  _ROOM  cor;
  _KOORD rx = (pr->xy2.x - pr->xy1.x + 1) / 2;
  _KOORD ry = (pr->xy2.y - pr->xy1.y + 1) / 2;
  _UWORD rrx = (rx + 7) / 14;
  _UWORD rry = (ry + 7) / 14;
  cor.xy1.x = pr->xy1.x + rx - rrx;
  cor.xy2.x = pr->xy1.x + rx + rrx;
  cor.xy1.y = pr->xy1.y + ry - rry;
  cor.xy2.y = pr->xy1.y + ry + rry;
  Oval(cor.xy1.x, cor.xy1.y, cor.xy2.x, cor.xy2.y, 0, 3600, T_COPY | T_SOLID | T_FILL, col);
  Oval(cor.xy1.x, cor.xy1.y, cor.xy2.x, cor.xy2.y, 0, 3600, T_COPY | T_SOLID, col >> 16);
}

// ***
// ***
// ***

void out_part_BMP(_ROOM *pr, _BMP *bmp, _ATTRIB attrib)
{
  _REGION actreg, ioreg=NULL, newreg=NULL;

  if(bmp != NULL)
  {
    actreg = ActivateAndRegion(&ioreg, &newreg, pr);
    Bitmap(pr->xy1.x, pr->xy1.y, bmp, attrib, DEFCOLOR);
    DeactivateAndRegion(actreg, &ioreg, &newreg);
  }
}

void out_zoom_BMP(_ROOM *pr, _BMP *bmp, _ATTRIB attrib)
{
  _UWORD width, height;

  if(bmp != NULL)
  {
    if((pr->xy2.x >= pr->xy1.x)&&(pr->xy2.y >= pr->xy1.y))
    {
      width  = pr->xy2.x - pr->xy1.x + 1;
      height = pr->xy2.y - pr->xy1.y + 1;

      if((bmp->width  == width) && (bmp->height == height))
      {
        Bitmap(pr->xy1.x, pr->xy1.y, bmp, attrib, DEFCOLOR);
      }
      else
      {
       #ifdef UC_FIXMEMORY      
        if(((bmp->state & 8) == 0) && (FixMemory != NULL))
        {
          _UDWORD fsize = bmp->datano * sizeof(_BMPDAT);
          if(((width * height * sizeof(_BMPDAT)) < FIXMEMORYSIZE) && (fsize < FIXMEMORYSIZE))
          {
            if((width > 0) && (height > 0) && (bmp->width > 0) && (bmp->height > 0))
            {
              _BMP fbmp = *bmp;
              fbmp.ptr  = (_BMPDAT*)GetFixMemory();
              MemCpy(fbmp.ptr, bmp->ptr, fsize);
              if(fixzoom_BMP16Bit(&fbmp, width, height) == _TRUE)
              {
                Bitmap(pr->xy1.x, pr->xy1.y, &fbmp, attrib, DEFCOLOR);
                return;
              }
            }
          }
        }
       #endif
       
        _BMP tmp;
        init_BMP_Makro(&tmp);
        if(copy_BMP(&tmp, bmp) == _TRUE)
        {
          if(zoom_BMP(&tmp, width, height) == _TRUE)
          {
            Bitmap(pr->xy1.x, pr->xy1.y, &tmp, attrib, DEFCOLOR);
          }
          free_BMP(&tmp);
        }
      }
    }
  }
}

void ClrScr(_COLOR color)
{
  _ROOM room;
  GetScreenSize(&room);
  
  if(color == INVISIBLE)
    color = WHITE;
  
  Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY|T_SOLID, MakeColor_Makro(color, color));
  
// #ifdef UC_ROTATE
//  RefreshRectangle(&room); // wegen sa26514 auskommentiert
// #endif 
  
  ScanBar.SetPos(0);
}

#ifdef UC_ABANDONED

void start_REPEAT(_REPEAT *p, _TIP tip)
{
  p->ftime = get_abstime();
  switch(tip)
  {
    case _TIPCOMBI  : p->timediff = 500;          // autorepeat 0.5, 0.1
                      break;
    case _TIPCONTI  : p->timediff = 0xFFFFFFFF;   // continuous rating
                      break;
    case _TIPSINGLE : p->timediff = 0;            // singleshot
                      break;
    case _TIPAUTO   : p->timediff = 100;          // autorepeat 0.1, 0.1
                      break;
  }
}

_BOOL run_REPEAT(_REPEAT *p)
{
  _BOOL retcode = _FALSE;

	if(p->timediff != 0)
	{
		if((timediff(p->ftime) > p->timediff) || (p->timediff == 0xFFFFFFFF))
		{
			p->ftime = get_abstime();
//      _UDWORD xxx = get_abstime();
//      p->ftime = xxx;
			if(p->timediff == 500)
				p->timediff = 100;
			retcode = _TRUE;
		}
	}

	return(retcode);
}

#endif

_BOOL do_LSEFUNCT(_LSEFUNCT *p, _Window *Window)
{
  return(do_LSEFUNCT_lrm(p, Window, _FALSE));
}

_BOOL do_LSEFUNCT_lrm(_LSEFUNCT *p, _Window *Window, _BOOL lrm)
{
  _BOOL retcode = _FALSE;

  if(Window == NULL)
    Window = Root.FindTop();

  if(p->no != 0xFFFF)
  {
    retcode = Window->AddStack(InterBoxList.Get(p->no), lrm);
  }
  else if(p->overload.no > 0)
  {
    retcode = Window->AddPart(p->overload.ptr, p->overload.no, lrm);
  }

  return(retcode);
}

extern "C" void DrawButton(_ROOM *pr, _ATTRIB attrib, _FRAMETYPE frametype, _COLOR colback, _COLOR colframe)
{
  _COLOR transparent;

//  if(attrib & T_INVRS)  // supportanfrage SA7???: könnte so behoben werden, seifert beschliesst änderung in opsys
//  {
//    colback = InvertColor_Makro(colback);
//    attrib = attrib & (~T_INVRS);
//  }

 #ifdef UC_THEME
  if(frametype >= _THEMEFRAME)
  {
    if(attrib & T_INVRS)
    {
      colback = MakeColor_Makro(GetPenColor_Makro(colback), GetBackColor_Makro(colback));
      draw_THEME(pr, frametype, attrib ^ T_INVRS, colback, _TRUE, _FALSE);
    }
    else
    {
      draw_THEME(pr, frametype, attrib, colback, _FALSE, _FALSE);
    }
  }
  else
 #endif
  {
    transparent = MakeColor_Makro(INVISIBLE, INVISIBLE);

    if(is_invisible(colback, attrib) == _FALSE)
      Bar(pr->xy1.x, pr->xy1.y, pr->xy2.x, pr->xy2.y, attrib, colback);

    if(colframe != transparent)
      frame(pr->xy1.x, pr->xy1.y, pr->xy2.x, pr->xy2.y, frametype, attrib, colframe);
  }
}

void frame(_KOORD x1, _KOORD y1, _KOORD x2, _KOORD y2, _UWORD shape, _ATTRIB attrib, _COLOR tlbr)
{
  _COLOR tl, br;
  _UWORD FrameSize, LineSize, FrameOffset;


  tl = GetBackColor_Makro(tlbr);
  br = GetPenColor_Makro(tlbr);

  if((tl != INVISIBLE) || (br != INVISIBLE))
  {
    attrib = (attrib & (T_SIZE|T_COMBI)) | T_SOLID;
    linear_coordinates(&x1, &y1, &x2, &y2);

    switch(shape)
    {
      case _SINGLEFRAME:
        DrawTLBR(x1, y1, x2, y2, attrib, MakeColor_Makro(tl, tl));
        break;
      case _3DFRAME:
        DrawTLBR(x1, y1, x2, y2, attrib, tlbr);
        break;
      case _OVERLAPPEDFRAME:
        FrameSize = GetXSize_Makro(attrib);
        if(FrameSize < 2)
          FrameSize = 2;
        LineSize = FrameOffset = FrameSize / 2;
        attrib = SetXSize(attrib, LineSize);
        attrib = SetYSize(attrib, LineSize);
        DrawTLBR(x1+FrameOffset, y1+FrameOffset, x2, y2, attrib, MakeColor_Makro(br, br));
        DrawTLBR(x1, y1, x2-FrameOffset, y2-FrameOffset, attrib, MakeColor_Makro(tl, tl));
        break;
      case _EDGEFRAME:
        FrameSize = GetXSize_Makro(attrib);
        if(FrameSize < 3)
          FrameSize = 3;
        LineSize = FrameSize / 3;
        FrameOffset = FrameSize - LineSize;
        attrib = SetXSize(attrib, LineSize);
        attrib = SetYSize(attrib, LineSize);
        DrawTLBR(x1, y1, x2, y2, attrib, tlbr);
        DrawTLBR(x1+FrameOffset, y1+FrameOffset, x2-FrameOffset, y2-FrameOffset, attrib, InvertColor_Makro(tlbr));
        break;
    }
  }
}

void round_frame(_KOORD x1, _KOORD y1, _KOORD x2, _KOORD y2, _UWORD shape, _ATTRIB attrib, _COLOR tlbr)
{
  _COLOR tl, br;

  tl = GetBackColor_Makro(tlbr);
  br = GetPenColor_Makro(tlbr);

  if((tl != INVISIBLE) || (br != INVISIBLE))
  {
    attrib = (attrib & (T_SIZE|T_COMBI)) | T_SOLID;
    linear_coordinates(&x1, &y1, &x2, &y2);

    switch(shape)
    {
      case _SINGLEFRAME :
        Oval(x1, y1, x2, y2, 0, 3600, attrib, MakeColor_Makro(tl, tl));
        break;
      case _3DFRAME :
      case _OVERLAPPEDFRAME :
      case _EDGEFRAME :
        x2 -= 1;
        y2 -= 1;
        Oval(x1+1, y1+1, x2+1, y2+1, 0, 3600, attrib, MakeColor_Makro(br, br));
        Oval(x1, y1, x2, y2, 0, 3600, attrib, MakeColor_Makro(tl, tl));
        break;
    }
  }
}

void DrawRound(_ROOM *pr, _ATTRIB attrib, _FRAMETYPE frametype, _COLOR colback, _COLOR colframe)
{
  _COLOR transparent = MakeColor_Makro(INVISIBLE, INVISIBLE);

  if(is_invisible(colback, attrib) == _FALSE)
    Oval(pr->xy1.x, pr->xy1.y, pr->xy2.x, pr->xy2.y, 0, 3600, attrib|T_FILL, colback);

  if(colframe != transparent)
    round_frame(pr->xy1.x, pr->xy1.y, pr->xy2.x, pr->xy2.y, frametype, attrib, colframe);
}


_UWORD get_framewidth(_FRAMETYPE frametype, _ATTRIB attrib)
{
  // wegen theme sollte funktion GetTextRoom verwendet werden
  _UWORD retcode = 0;

  switch(frametype & 0x7F)
  {
    case _SINGLEFRAME     : retcode = GetXSize_Makro(attrib);
                            break;
    case _OVERLAPPEDFRAME : retcode = GetXSize_Makro(attrib);
                            retcode = (retcode < 2)? 2 : retcode;
                            break;
    case _EDGEFRAME       : retcode = GetXSize_Makro(attrib);
                            retcode = (retcode < 3)? 3 : retcode;
                            break;
    case _3DFRAME         : retcode = GetXSize_Makro(attrib);
                            retcode = (retcode < 1)? 1 : retcode;
                            break;
  }

  return(retcode);
}

_UWORD get_frameheight(_FRAMETYPE frametype, _ATTRIB attrib)
{
  // wegen theme sollte funktion GetTextRoom verwendet werden
  _UWORD retcode = 0;

  switch(frametype & 0x7F)
  {
    case _SINGLEFRAME     : retcode = GetYSize_Makro(attrib);
                            break;
    case _OVERLAPPEDFRAME : retcode = GetYSize_Makro(attrib);
                            retcode = (retcode < 2)? 2 : retcode;
                            break;
    case _EDGEFRAME       : retcode = GetYSize_Makro(attrib);
                            retcode = (retcode < 3)? 3 : retcode;
                            break;
    case _3DFRAME         : retcode = GetYSize_Makro(attrib);
                            retcode = (retcode < 1)? 1 : retcode;
                            break;
  }

  return(retcode);
}

void GetTextRoom(_ROOM *dst, _ROOM *src, _ATTRIB attrib, _FRAMETYPE frametype, _BOOL InclFrame)
{
  _UWORD xs, ys, fs;

 #ifdef UC_THEME
  if(frametype >= _THEMEFRAME)
  {
    inlet_THEME(dst, src, frametype);
  }
  else
 #endif
  {
    xs = get_framewidth(frametype, attrib);
    ys = get_frameheight(frametype, attrib);

    dst->xy1.x = src->xy1.x + xs;
    dst->xy1.y = src->xy1.y + ys;
    dst->xy2.x = src->xy2.x - xs;
    dst->xy2.y = src->xy2.y - ys;
  }

  if(InclFrame == _TRUE)
  {
    fs = SetUp.GetFrameSpace();
    dst->xy1.x += fs;
    dst->xy1.y += fs;
    dst->xy2.x -= fs;
    dst->xy2.y -= fs;
  }
}

#ifndef GRC_XFREEZE_SCREEN
 #define GRC_XFREEZE_SCREEN 7
#endif

void push_screen(_NEWSCREEN *act)
{
 #ifdef UC_ROTATE
  ScreenStop();
 #else
  #ifdef OK_OFFSCREEN
   if(UseOffscreen == _TRUE)
   {
     GetScreen(act);
     WritePixel(0, 0, T_COPY|T_SOLID, ReadPixel(0, 0)); // set copy attribute to destination screen
     ActivateScreen(&OffScreen);
   }
   else
  #endif
   {
     ScreenStop();
   }
 #endif
}

void pop_screen(_NEWSCREEN *act)
{
 #ifdef UC_ROTATE
  ScreenStart();
 #else
  #ifdef OK_OFFSCREEN
   if(UseOffscreen == _TRUE)
   {
     ActivateScreen(act);
   }
   else
  #endif
   {
     ScreenStart();
   }
 #endif
}

void copy_screen(_NEWSCREEN *act, _ROOM *pr)
{
 #ifdef UC_ROTATE
  RefreshRectangle(pr);
 #else
  #ifdef OK_OFFSCREEN
   if(UseOffscreen == _TRUE)
   {
     CopyScreen(act, &OffScreen, pr, pr);
   }
//   else
  #endif
//   {
//    #ifdef UC_ROTATE
//     RefreshRectangle(pr);
//    #endif
//   }
 #endif
}

_REGION InvertRegion(void)
{
  _REGION tmp_reg, act_reg, new_reg;
  _ROOM   room;

  act_reg = GetActiveRegion();

  StartRegion();
  GetScreenSize(&room);
  AddRegion(&room);
  tmp_reg = EndRegion();

  new_reg = XorRegion(tmp_reg, act_reg);

  free_REGION(&tmp_reg);

  ActivateRegion(new_reg);

  return(new_reg);
}

void ConfigGraphicSystem(_UDWORD cmd, _UDWORD info, void *ptr)
{
  // #define GRC_SET_PIXELCORRECTION     1  // include right bottom pixel
  // #define GRC_RESET_PIXELCORRECTION   2  // exclude right bottom pixel
  // #define GRC_SET_ORIGINCOLOR         4  // color: normal mode
  // #define GRC_SET_NAVYCOLOR           5  // color: navy mode (red)
  // #define GRC_SET_VARIATIONCOLOR      6  // color variation - ptr = 3 * _UWORD

  _FVOID_BOOL               p0;
  _FUDWORD_UDWORD_UDWORD_PT p1;
  _BOOL                     tmp;

  #ifdef LASAL
    #ifndef SetGraphicSystem
      #define SetGraphicSystem NULL
    #endif

    #ifndef SetPixelProblem
      #define SetPixelProblem NULL
    #endif
  #endif

  if(RTOSVERSION > 0x050B)       // ab version 5.12
  {
	  p1 = SetGraphicSystem;
	  if(p1 != NULL)
      p1(cmd, info, ptr);

  }
  else if(RTOSVERSION == 0x050B) // bei version 5.11
  {
	 p0 = SetPixelProblem;
    if(p0 != NULL)
    {
      if((cmd == GRC_SET_PIXELCORRECTION)||(cmd == GRC_RESET_PIXELCORRECTION))
      {
        tmp = (cmd == GRC_SET_PIXELCORRECTION)? _TRUE : _FALSE;
        p0(tmp);
      }
    }
  }

}

void ReadPixelStart(_KOORD x0, _KOORD y0, _KOORD x1, _KOORD y1)
{
  if(OpSysIsSalamander()) 
  {
    _ROOM room;
    room.xy1.x = x0;
    room.xy1.y = y0;
    room.xy2.x = x1;
    room.xy2.y = y1;
    ConfigGraphicSystem(GRC_SCREENSHOT_START, 0, &room);
  }
}

void ReadPixelStop(void)
{
  if(OpSysIsSalamander())
  {
    ConfigGraphicSystem(GRC_SCREENSHOT_STOP, 0, NULL);
  }
}

_BOOL IsSalamander(void)
{
  if(OpSysIsSalamander())
    return _TRUE;
  return _FALSE;
}

void TeachGraphicSystem(_UDWORD cmd)
{

  ConfigGraphicSystem(cmd, 0, NULL);
}

_BOOL add_MEM_POINTER(_MEM_POINTER *p, void *ptr)
{
  _UDWORD i;
  _BOOL   retcode = _FALSE;

  if(ptr != NULL)
  {
    for(i=0; i<p->no; i++)
    {
      if(ptr == p->ptr[i])
        return(_TRUE);
    }

    if(Memory2((void**)&p->ptr, (p->no+1) * sizeof(void*)) != 0)
    {
      p->ptr[p->no] = ptr;
      p->no ++;
      retcode = _TRUE;
    }
  }

  return(retcode);
}

_BOOL make_MEM_POINTERPARA(_MEM_POINTERPARA *p, _UDWORD anzahl)
{
  free_MEM_POINTERPARA(p);
  if(Memory2((void**)&p->ptr, (anzahl) * sizeof(_POINTERPARA)) != 0)
  {
    //for(_UDWORD i=0; i<anzahl; i++)
    //  init_POINTERPARA(&p->ptr[i]);
    p->no     = 0;
    p->doneno = anzahl;
    return _TRUE;
  }
  
  return _FALSE;
}

_BOOL add_MEM_POINTERPARA(_MEM_POINTERPARA *p, void *ptr, _UDWORD para)
{
  if(p->no < p->doneno)
  {
    p->ptr[p->no].ptr  = ptr;
    p->ptr[p->no].para = para;
    p->no ++;
    return _TRUE;
  }

  if(Memory2((void**)&p->ptr, (p->doneno + 10) * sizeof(_POINTERPARA)) != 0)
  {
    for(_UDWORD i=0; i<10; i++)
      init_POINTERPARA(&p->ptr[p->doneno++]);
      
    p->ptr[p->no].ptr  = ptr;
    p->ptr[p->no].para = para;
    p->no ++;
    return _TRUE;
  }

  return _FALSE;
}

void SetLseServer(_UWORD server, _DWORD value)
{
  _FVOID_DWORD_DWORD ptr = (_FVOID_DWORD_DWORD)find_lasal_method(LseThisPointer, METHOD_LSE_SETSTATUS);

  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    ptr(server, value);
  }
}

_DWORD GetLseServer(_DWORD server)
{
  _FDWORD_DWORD ptr = (_FDWORD_DWORD)find_lasal_method(LseThisPointer, METHOD_LSE_GETSTATUS);
  _DWORD        retcode = 0;

  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    retcode = ptr(server);
  }

  return(retcode);
}


void *NewDoScheme(_ANYLST scheme, _DWORD value, _UDWORD schemeno)
{
  void         *retcode = NULL;
  _VARLABEL    vtmp;
  _UDWORD      dst;
  _MYSCHEME    myscheme;
  _ANYPARA     *pa;
  _UDWORD       unitno;


  init_MYSCHEME_Makro(&myscheme);
  myscheme.schemeno = schemeno;

  init_VARLABEL_Makro(&vtmp);
  vtmp.no = 1;
  vtmp.info[0].state = CONST_VAL;
  vtmp.info[0].value = value;

  switch(scheme)
  {
    case IMAGESCHEME  : if(ImageScheme.Call(&dst, &myscheme, &vtmp, 0xFFFF) == _TRUE)
                          retcode = (void*)ImageList.Get((_UWORD)dst);
                        break;
    case OBJECTSCHEME : if(ObjectScheme.Call(&dst, &myscheme, &vtmp, 0xFFFF) == _TRUE)
                          retcode = (void*)ObjectList.Get((_UWORD)dst);
                        break;
    case TEXTSCHEME   : pa = TextScheme.Call(schemeno, &vtmp);
                        if(pa != NULL)
                          retcode = (void*)get_text(&pa->anything, NULL, &pa->memvarlabel, _TRUE);
                        break;
    case VARUNIT      : if(UnitScheme.Call(&unitno, &myscheme, NULL, 0xFFFF) == _TRUE)
                          retcode = (void*)VarList.GetUnitInfo(unitno);
                        break;
    default           : break;
  }

  return(retcode);
}

void *DoScheme(void **dummy, _ANYLST scheme, _DWORD value, _UDWORD schemeno)
{
  if(dummy != NULL)
    *dummy = NULL;

  return(NewDoScheme(scheme, value, schemeno));
}

void *DoTextScheme(_DWORD value, _UWORD schemeno, _MEM_VARLABEL *pmv)
{
  void         *retcode = NULL;
  _VARLABEL    vtmp;
  _ANYPARA     *pa;

  init_VARLABEL_Makro(&vtmp);
  vtmp.no = 1;
  vtmp.info[0].state = CONST_VAL;
  vtmp.info[0].value = value;

  pa = TextScheme.Call(schemeno, &vtmp);
  if(pa != NULL)
    retcode = (void*)get_text(&pa->anything, NULL, (pmv==NULL)?&pa->memvarlabel:pmv, _TRUE);

  return(retcode);
}

_UWORD fade_color(_UWORD start, _UWORD end, _UWORD actno, _UWORD no)
{
  if(actno == 0)
    return(start);
  if(actno == (no-1))
    return(end);

  if((start != end)&&(no > 1))
  {
    if(start < end)
      start += ((end-start)*actno / (no-1));
    else
      start -= ((start-end)*actno / (no-1));
  }

  return(start);
}

void color_course(_ROOM *pr, _WORD height, _COLOR begin, _COLOR end, _ATTRIB att)
{
  _ROOM  room = *pr;
  _COLOR color;
  _UWORD i;
  _UWORD distance;
  _RGB   b, e;

  if(height != 0)
  {
    Make_RGB(&b, begin);
    Make_RGB(&e, end);

	 #ifdef UC_MOUSE
		_UWORD hide = IfNecHideMouseCursor(&room);
	 #endif

    if(att & T_HORIZBOUND)
    {
      distance = room.xy2.x - room.xy1.x + 1;
      if(height > 0)
      {
        if(height > distance)
          height = distance;
        for(i=0; i<height; i++)
        {
          color = MakeTrueColor(fade_color(b.cred, e.cred, i, distance), fade_color(b.cgreen, e.cgreen, i, distance), fade_color(b.cblue, e.cblue, i, distance));
          Line(room.xy1.x, room.xy1.y, room.xy1.x, room.xy2.y, att, color);
          room.xy1.x++;
        }
      }
      else
      {
        height = -height;
        if(height > distance)
          height = distance;
        for(i=0; i<height; i++)
        {
          color = MakeTrueColor(fade_color(b.cred, e.cred, i, distance), fade_color(b.cgreen, e.cgreen, i, distance), fade_color(b.cblue, e.cblue, i, distance));
          Line(room.xy2.x, room.xy1.y, room.xy2.x, room.xy2.y, att, color);
          room.xy2.x--;
        }
      }
    }
    else
    {
      distance = room.xy2.y - room.xy1.y + 1;
      if(height > 0)
      {
        if(height > distance)
          height = distance;
        for(i=0; i<height; i++)
        {
          color = MakeTrueColor(fade_color(b.cred, e.cred, i, distance), fade_color(b.cgreen, e.cgreen, i, distance), fade_color(b.cblue, e.cblue, i, distance));
          Line(room.xy1.x, room.xy1.y, room.xy2.x, room.xy1.y, att, color);
          room.xy1.y++;
        }
      }
      else
      {
        height = -height;
        if(height > distance)
          height = distance;
        for(i=0; i<height; i++)
        {
          color = MakeTrueColor(fade_color(b.cred, e.cred, i, distance), fade_color(b.cgreen, e.cgreen, i, distance), fade_color(b.cblue, e.cblue, i, distance));
          Line(room.xy1.x, room.xy2.y, room.xy2.x, room.xy2.y, att, color);
          room.xy2.y--;
        }
      }
    }

	 #ifdef UC_MOUSE
		IfNecShowMouseCursor(hide);
	 #endif
  }
}

_UDWORD get_MYSCHEME(_MYSCHEME *p)
{
  _UDWORD retcode;

  if(StateScheme.Call(&retcode, p, NULL, STATE_ACTIVE) == _FALSE)
     retcode = STATESCHEME_PRESELECTION;

  return(retcode);
}

_COLOR DoColorScheme(_LINKSCHEME *ls, _COLOR defcol)
{
  _UDWORD data;
  if(ColorScheme.Call(&data, ls, defcol) == _TRUE)
    return (_COLOR)data;
    
  return defcol;
}

_BOOL change_PICTUREMASK_group(_PICTUREMASK *p, _UWORD group, _UDWORD varno)
{
  _BOOL    retcode = _FALSE;
  _UWORD   i;
  _IO      *pio;
  _PICTEXT *ptx;

  if((p != NULL)&&(group != DEFSUMMARY)) //&&(varno != DEFSCOPE))
  {
    ptx = p->mempictxt.ptr;
    for(i=0; i<p->mempictxt.no; i++)
    {
     #ifdef UC_CHANGE_IO_GROUP
      if(ptx->lock_overload == _FALSE)
     #endif
      {
        if(ptx->summary == group)
          if(change_text_sub(ptx, ptx->anypara.anything.no, varno) == _TRUE)
            retcode = _TRUE;
      }
      ptx += 1;
    }

    pio = p->memoutput.ptr;
    for(i=0; i<p->memoutput.no; i++)
    {
      if(pio->summary == group)
      {
       #ifdef UC_CHANGE_IO_GROUP
        if(pio->lock_overload == _FALSE)
        {
          if(xchange_VARLABEL(&pio->variable, pio->variable.info[0].value, varno) == _TRUE)
            retcode = _TRUE;
        }
        pio->imiogroupchanged.serverno = varno;
        pio->imiogroupchanged.summary = group;
        if(change_SubPicture_group(pio->Obj, group, varno) == _TRUE)
          retcode = _TRUE;
       #else
        if(xchange_VARLABEL(&pio->variable, pio->variable.info[0].value, varno) == _TRUE)
          retcode = _TRUE;
       #endif
      }
      pio += 1;
    }

    for(i=0; i<p->meminput.no; i++)
    {
      pio = &p->meminput.ptr[i].io;
      if(pio->summary == group)
      {
       #ifdef UC_CHANGE_IO_GROUP
        if(pio->lock_overload == _FALSE)
        {
          if(xchange_VARLABEL(&pio->variable, pio->variable.info[0].value, varno) == _TRUE)
            retcode = _TRUE;
        }
        pio->imiogroupchanged.serverno = varno;
        pio->imiogroupchanged.summary = group;
        if(change_SubPicture_group(pio->Obj, group, varno) == _TRUE)
          retcode = _TRUE;
       #else
        if(xchange_VARLABEL(&pio->variable, pio->variable.info[0].value, varno) == _TRUE)
          retcode = _TRUE;
       #endif
      }
    }

   #ifdef UC_CHANGE_IO_GROUP
    _BUTTON *pb = p->membutton.ptr;
    for(i=0; i<p->membutton.no; i++)
    {
      if(pb->summary == group)
      {
        if(pb->lock_overload == _FALSE)
        {
          if(xchange_VARLABEL(&pb->varlabel, pb->varlabel.info[0].value, varno) == _TRUE)
            retcode = _TRUE;
        }
        pb->imiogroupchanged.serverno = varno;
        pb->imiogroupchanged.summary = group;
        if(change_SubPicture_group(pb->SubPicture, group, varno) == _TRUE)
          retcode = _TRUE;
      }
      pb += 1;
    }

    _OBJECT *po = p->memobject.ptr;
    for(i=0; i<p->memobject.no; i++)
    {
      if(po->summary == group)
      {
        po->imiogroupchanged.serverno = varno;
        po->imiogroupchanged.summary = group;
        if(change_SubPicture_group(po->SubPicture, group, varno) == _TRUE)
          retcode = _TRUE;
      }
      po += 1;
    }
   #endif
  }

  return(retcode);
}

_BOOL change_SubPicture_group(_SubPicture *p, _UWORD group, _UDWORD varno)
{
  _PICTURE *ppic;
  _BOOL    retcode = _FALSE;

  if((p != NULL)&&(group != DEFSUMMARY)) //&&(varno != DEFSCOPE))
  {
    ppic = p->GetPICTURE();
    retcode = change_PICTUREMASK_group(&ppic->mask, group, varno);
  }

  return(retcode);
}

_BOOL change_io_group(_MY_PICTURE *p, _UWORD group, _UDWORD varno)
{
  _BOOL retcode = _TRUE;
  _UWORD i;

  if((p != NULL)&&(group != DEFSUMMARY)) //&&(varno != DEFSCOPE))
  {
    for(i=0; i<p->No; i++)
      change_SubPicture_group(&p->Ptr[i], group, varno);
  }

  return(retcode);
}

_UWORD get_index(_UWORD *index, _CHAR *txt, _UWORD preselect)
{
  _UWORD erg     = preselect;
  _UWORD retcode = 0;
  _BOOL  run;
	_CHAR  chr;
  _BOOL  st = _FALSE;

  do
  {
    run = _FALSE;
    chr = *txt;
    if((chr >= '0') && (chr <= '9'))
    {
      if(st == _FALSE)
      {
        erg = 0;
        st = _TRUE;
      }
      run = _TRUE;
      erg = (erg * 10) + (chr - '0');
			retcode++;
      txt++;
    }
  }
  while(run == _TRUE);

  *index = erg;

  return(retcode);
}

_CHAR *get_addition_label(_CHAR *src, _ASCII *label)
{
  _UWORD len;
  len = StrChr(src, '(', sizeof(_CHAR));
  
  if(len < 100)
  {
    while(len)
    {
      *label = *src;
      label ++;
      src ++;
      len --;
    }
  }

  *label = 0;

  return(src);
}

_BOOL get_ps_VARLABEL(_CHAR **psrc, _UWORD &index, _UWORD &preselect, _MEM_VARLABEL *pmv)
{
  _VARLABEL *pvl;
  _CHAR     *src = *psrc;

  src ++;
  src += get_index(&index, src, preselect++);
  if(index < pmv->no)
  {
    pvl = &pmv->ptr[index];
    if(pvl->no >= 1)
    {
      if((pvl->info[0].state != CONST_VAL)||(pvl->info[0].value != 0x7FFFFFFFL))
      {
        *psrc = src;
        return _TRUE;
      }
    }
  }

  return _FALSE;
}


_BOOL do_addition(_CHAR **psrc, _CHAR **ptmp, _UWORD &index, _UWORD &preselect, _MEM_VARLABEL *pvm, _UDWORD percent_x_varno, _BOOL justlse) 
{
  // syntax:
  // %[ts.name(%s)]
  // %[ts.name(%p)]
  // %[ts.name(%x)]

  _CHAR    *src, *tmp, *ptxt;
  _UWORD   len, hl;
  _ASCII   label[100];
  _UDWORD  sno;
  _BOOL    retcode = _FALSE;
  _ANYPARA *pap;

  src = *psrc;
  tmp = *ptmp;
  len = StrChr(src, ']', sizeof(_CHAR));

  if(len != 0xFFFF)
  {
    if((src[1] == 's')||(src[1] == 'S')) // es betrifft ein schema
    {
      if(src[2] == '.')
      {
        if((src[0] == 't')||(src[0] == 'T')) // konkret: textschema
        {
          src += 3;
          src = get_addition_label(src, label);

          if(TextScheme.LseLabel.Seek(&sno, label) == _TRUE)
          {
            if((src[0] == '(')&&(src[1] == '%'))
            {
              src  += 2;
              switch(*src)
              {
                case 'p' :
                case 'P' : if(justlse == _FALSE)
                             retcode = _TRUE;
                           break;
                case 's' :
                case 'S' : if(justlse == _TRUE)
                             retcode = _TRUE;
                           break;
                case 'x' :
                case 'X' : 
                           _DWORD xindexval;
                           if(VarList.GetXIndex(&xindexval, percent_x_varno) == _TRUE)
                           {
                             pap = TextScheme.Call(sno, xindexval);
                             if(pap != NULL)
                             {
                               retcode = _TRUE;
                               goto xdaNEEDS;
                             }
                           }
                           break;
                
              }

              if(retcode == _TRUE)
              {
                if(get_ps_VARLABEL(&src, index, preselect, pvm) == _TRUE)
                {
                  pap = TextScheme.Call(sno, &pvm->ptr[index]);
                  if(pap != NULL)
                  {
xdaNEEDS:                  
                    ptxt = get_ANYPARA(pap, NULL);
                    if(ptxt != NULL)
                    {
                      hl = StrLen(ptxt, sizeof(_CHAR));
                      MemCpy(tmp, ptxt, hl*sizeof(_CHAR));
                      tmp += hl;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    if(retcode == _TRUE)
    {
      *psrc += (len + 1);
      *ptmp = tmp;
    }
  }

  return(retcode);
}
/*
_UDWORD IfNecAddUnitText(_CHAR *dst, _CHAR **psrc, _UDWORD index, _MEM_VARLABEL *pmv)
{
  _CHAR *src = *psrc;
  
  if((*src == 'u')||(*src == 'U'))
  {
    src++;
    *psrc = src;
    if(index < pmv->no)
    {
      _VARLABEL *pvl = &pmv->ptr[index];
      if(pvl->no >= 1)
      {
        if((pvl->info[0].state != CONST_VAL)||(pvl->info[0].value != 0x7FFFFFFFL))
        {
          _CHAR *tmpu = get_temptext();  // get --> to temporary textpuffer
          _UDWORD lenu = format_unit(&tmpu[1], pvl);
          if(lenu > 0)
          {
            tmpu[0] = '[';
            tmpu[lenu+1] = ']';
            tmpu[lenu+2] = 0;
            StrCpy(dst, sizeof(_CHAR), tmpu, sizeof(_CHAR));
            return lenu+2;
          }
        }
      }
    }
  }
  
  return 0;
}
*/

_CHAR *make_parameterstring_sub(_CHAR *src, _MEM_VARLABEL *pmv, _BOOL justlse, _UDWORD percent_x_varno)
{
  _CHAR     *retcode = src;
  _CHAR     *tmp;
  _CHAR     chr;
  _UWORD    index, preselect, run;
  _VARLABEL *pvl;
  _DWORD    xindexval;

  if((percent_x_varno != DEFSCOPE) && (pmv == NULL))
  {
    _MEM_VARLABEL tmpmv;
    init_MEM_VARLABEL(&tmpmv);
    pmv = &tmpmv;
  }

  if((src != NULL)&&(pmv != NULL))
  {
    //if(pmv->no > 0)
   #ifdef SA55048 // DEVVIS-29
    if(StrChr(src, '%', sizeof(_CHAR)) != 0xFFFF) // nur wenn '%' Zeichen im String vorkommt...
   #endif 
    {
      tmp       = get_temptext();  // get --> to temporary textpuffer
      retcode   = tmp;
      run       = 1;
      preselect = 0;

      do
      {
        chr  = *src;  src ++;
        *tmp = chr;

        switch(chr)
        {
          case 0   : run = 0;
                     break;
          case '%' : switch(*src)
                     {
                       case 'p' : // es wird ein parameter (alarmparameter) +evntl.seine unit eingetragen
                       case 'P' : if(justlse == _FALSE)
                                  {
                                    src ++;
                                    src += get_index(&index, src, preselect++);
                                    if(index < pmv->no)
                                    {
                                      pvl = &pmv->ptr[index];
                                      if(pvl->no >= 1)
                                      {
                                        if((pvl->info[0].state != CONST_VAL)||(pvl->info[0].value != 0x7FFFFFFFL))
                                        {
                                          tmp += format_variable(tmp, pvl); //&pmv->ptr[index]);
                                        }
                                      }
                                    }
                                    
//                                    tmp += IfNecAddUnitText(tmp, &src, index, pmv); // unittext anhängen wenn %pu
                                  }
                                  else
                                  {
                                    tmp ++;
                                  }
                                  break;
//                       case 'B' : // es wird der konstante wert ICONID eingetragen
//                       case 'b' : if(justlse == _TRUE)
//                                  {
//                                    tmp[0] = ICONID;
//                                    tmp   += 1;
//                                    src   += 1;
//                                  }
//                                  break;
//                       case 'I' : // es wird der konstante wert ICONID eingetragen
//                       case 'i' : if(justlse == _TRUE)
//                                  {
//                                    tmp[0] = ICONID;
//                                    tmp   += 1;
//                                  }
                       case 'S' : // es wird ein server oder eine konstante eingetragen (im lse definiert)
                       case 's' : if(justlse == _TRUE)
                                  {
                                    src ++;
                                    src += get_index(&index, src, preselect++);
                                    if(index < pmv->no)
                                    {
                                      pvl = &pmv->ptr[index];
                                      if(pvl->no >= 1)
                                      {
                                        if((pvl->info[0].state != CONST_VAL)||(pvl->info[0].value != 0x7FFFFFFFL))
                                          tmp += format_variable(tmp, pvl);
                                      }
                                    }
//                                    tmp += IfNecAddUnitText(tmp, &src, index, pmv); // unittext anhängen wenn %su
                                  }
                                  else
                                  {
                                    tmp ++;
                                  }
                                  break;
                       case 'U' :
                       case 'u' : src ++;
                                  src += get_index(&index, src, preselect++);
                                  if(index < pmv->no)
                                    tmp += format_unit(tmp, &pmv->ptr[index]);
                                  break;
                       case 'x' :
                       case 'X' : src ++;
                                  if(VarList.GetXIndex(&xindexval, percent_x_varno) == _TRUE)
                                  {
                                     DToA(tmp, xindexval, 0x0800, sizeof(_CHAR));
                                     tmp += StrLen(tmp, sizeof(_CHAR));
                                  }
                                  break;
                       case '[' : // jetzt wirds lustig, name wurde direkt eingegeben
                                  src ++;
                                  if(do_addition(&src, &tmp, index, preselect, pmv, percent_x_varno, justlse) == _FALSE)
                                  {
                                    src --;
                                    tmp ++;
                                  }
                                  break;
                       default :  tmp ++;
                                  break;
                     }
                     break;
          default  : tmp ++;
                     break;
        }
      }
      while(run);
    }
  }

  return(retcode);
}


_CHAR *make_parameterstring(_CHAR *src, _MEM_VARLABEL *pmv, _BOOL justlse)
{
  return make_parameterstring_sub(src, pmv, justlse, DEFSCOPE);
}


#ifdef VARTEXT_PREFIX
_CHAR *ccreate_prefix(_CHAR *txt, _ANYTHING *p, _TextPuffer *PT, _VARINFO *pvi)
{
  _CHAR *retcode = NULL;
  _CHAR *tmp;

  if(pvi->text3.anything.no != 0xFFFF)
  {
   #ifndef CUTOFF_USER
    _FBOOL_UWORD_PT ptr;

    ptr = (_FBOOL_UWORD_PT)find_lasal_method(LseThisPointer, METHOD_LSE_CHECKPREFIX);
    if(ptr != NULL)
    {
      load_this(LseThisPointer);
      if(ptr(p->no, pvi) == _TRUE)
      {
        retcode = get_text(&pvi->text3.anything, PT, &pvi->text3.memvarlabel, _FALSE);
        if(retcode != NULL)
        {
          tmp = get_temptext();
          StrCpy(tmp, sizeof(_CHAR), retcode, sizeof(_CHAR));
          StrCat(tmp, sizeof(_CHAR), txt, sizeof(_CHAR));
          retcode = tmp;
        }
      }
    }
   #endif
  }

  if(retcode == NULL)
    retcode = txt;

  return(retcode);
}
#endif

_BOOL cis_server_valid(_LSEVARINFO **ppvi, _UDWORD varno)
{
  _VARLABEL varlabel;

  init_VARLABEL_Makro(&varlabel);
  varlabel.no = 1;
  varlabel.info[0].state = VAR_VAL;
  varlabel.info[0].value = varno;
  if(valid_VARLABEL(&varlabel) == _TRUE)
  {
    if(VarList.GetVarInfo(ppvi, varno) == _TRUE)
    {
      if(VarList.GetStateScheme(varno, NOCARE_DEFAULT) != STATE_INVISIBLE)
      {
        return _TRUE;
      }
    }
  }

  return _FALSE;
}

_CHAR *get_text_sub_limit(_UDWORD varno, _ANYLST list)
{
  _LSEVARINFO *pvi = NULL;
  _CHAR       *retcode;
  _TIME       tim;

  if(cis_server_valid(&pvi, varno) == _TRUE)
  {
    _RESULT   result;
    _VARIABLE variable;
    _VARLABEL vl;
  
    init_VARLABEL(&vl);
    vl.no = 1;
    vl.info[0].state = VAR_VAL;
    vl.info[0].value = varno;

    if(VarList.GetSystemData(&result, &variable, &vl, _TRUE, _TRUE) == _TRUE)
    {
      switch(variable.info.vartype)
      {
        case BINARY    :
        case FCONSTANT :
        case FMERKER   :
          result.value = (list == VARLIMIT_HI)? result.rmax : result.rmin;
          recalculate(&result, &variable, _TRUE);
          retcode = get_temptext();
          format_value(retcode, result.value, &variable.info, NULL);
          if(StrChr(retcode, '>', sizeof(_CHAR)) == 0xFFFF) 
            return retcode;
          break;
          
        case FTIME :
          init_TIME(&tim);
          if(list == VARLIMIT_HI)
          {
            tim.hour = 23;
            tim.minute = 59;
            tim.second = 59;
          }
          else
          {
            tim.hour = 0;
            tim.minute = 0;
            tim.second = 0;
          }
          retcode = get_temptext();
          format_time(retcode, &tim, variable.info.format);
          return retcode;
      }
    }
  }
  
  return NULL;
}

_CHAR *get_text_sub(_ANYTHING *p, _TextPuffer *PT, _MEM_VARLABEL *pmv, _BOOL prefixenable, _UDWORD percent_x_varno)
{
  _CHAR       *retcode = NULL;
  _LSEVARINFO *pvi = NULL;
  _ANYTHING   tmp;
  _CHAR       *ptxt1;
  _MYSCHEME   myscheme;
  _UDWORD     unitno;
  _UNIT       *pu;
  _ASCII      *pa;

  switch(p->list)
  {
    case VARTXT1_OBJNAME:
    case VARTXT1_TOKEN :
    case VARTXT1 : // variablentext 01 adressed by varno
    case VARTXT2 : // variablentext 02 adressed by varno
    case VARTXT3 : // variablentext 03 adressed by varno
    case VARTXT4 : if(cis_server_valid(&pvi, p->no) == _TRUE)
                   {
                     switch(p->list)
                     {
                       case VARTXT2 : if(pvi->xmemtext.xxno >= 2)
                                        retcode = get_text_sub(&pvi->xmemtext.xxptr[1].anything, PT, &pvi->xmemtext.xxptr[1].memvarlabel, _FALSE, p->no);
                                      break;
                       case VARTXT3 : if(pvi->xmemtext.xxno >= 3)
                                        retcode = get_text_sub(&pvi->xmemtext.xxptr[2].anything, PT, &pvi->xmemtext.xxptr[2].memvarlabel, _FALSE, p->no);
                                      break;
                       case VARTXT4 : if(pvi->xmemtext.xxno >= 4)
                                        retcode = get_text_sub(&pvi->xmemtext.xxptr[3].anything, PT, &pvi->xmemtext.xxptr[3].memvarlabel, _FALSE, p->no);
                                      break;
                       default      : if(pvi->xmemtext.xxno >= 1)
                                        retcode = get_text_sub(&pvi->xmemtext.xxptr[0].anything, PT, &pvi->xmemtext.xxptr[0].memvarlabel, _FALSE, p->no);
                                      break;
                     }

                     if((retcode == NULL)||(*(_CHAR*)retcode == 0))
                     {
                       if(p->list == VARTXT1_TOKEN)
                       {
                          pa = VarList.GetLabelToken(p->no);
                          if(pa != NULL)
                          {
                            ptxt1 = get_temptext();
                            StrCpy(ptxt1, sizeof(_CHAR), pa, sizeof(_ASCII));
                            retcode = ptxt1;
                          }
                       }
                       else if(p->list == VARTXT1_OBJNAME)
                       {
                         // übergeordneten objektnamen ermitteln
                         tmp.list = VARNAME;
                         tmp.no   = VarList.GetNodeObjectNumber(p->no);
                         if(tmp.no == DEFSCOPE)
                           tmp.no = p->no;
                         
                         pmv      = NULL;
                         retcode  = get_text(&tmp, PT, NULL, _FALSE);
                       }
                       else
                       {
                         tmp.list = VARNAME;
                         tmp.no   = p->no;
                         pmv      = NULL;
                         retcode  = get_text(&tmp, PT, NULL, _FALSE);
                       }
                     }
                   }
									 break;
    case VARNAME : // name of variable addressed by varno
                   if(cis_server_valid(&pvi, p->no) == _TRUE)
                   {
                     ptxt1 = get_temptext();
                     _UBYTE varobj = 0;
                     if(VarList.GetLabel((_ASCII*)ptxt1, &varobj, p->no) == _TRUE)
                     {
                      #ifdef UC_UNICODE
                       _CHAR *ptxt2 = get_temptext();
                       StrCpy(ptxt2, sizeof(_CHAR), ptxt1, sizeof(_ASCII));
                       retcode = ptxt2;
                      #else
                       retcode = ptxt1;
                      #endif
                     }
                   }
                   break;

    case VARUNIT : // unit text adressed by varno
                   if(cis_server_valid(&pvi, p->no) == _TRUE)
                   {
                     init_MYSCHEME_Makro(&myscheme);
                     myscheme.schemeno = pvi->funitscheme;
                     if(UnitScheme.Call(&unitno, &myscheme, NULL, pvi->funit) == _TRUE)
                     {
                       retcode = VarList.GetUnitText((_UWORD)unitno);
                       pu      = VarList.GetUnitInfo(unitno);
                       if(pu != NULL)
                         pmv = &pu->anypara.memvarlabel;
                     }
                   }
                   break;
                   
    case VARLIMIT_LO : 
    case VARLIMIT_HI :
                   retcode = get_text_sub_limit(p->no, p->list);
                   break;
                   
    case SOFTTXT :
    case LOCTXT  : if(PT != NULL)
                     retcode = PT->GetText(p->no);
                   break;
    default      : if(p->list >= (_UWORD)TXTLIST_VTX)
                     retcode = TextList.Get(p->list, p->no);
                   break;
  }

  retcode = make_parameterstring_sub(retcode, pmv, _TRUE, percent_x_varno);

 #ifdef VARTEXT_PREFIX
  _VARINFO    vi;
  _BOOL ret = _TRUE;
  if((prefixenable == _TRUE) && (retcode != NULL) && (pvi != NULL))
  {
    switch(p->list)
    {
      case VARTXT1 :
//      case VARTXT2 :
//      case VARNAME :
                     copy_LSE_VARINFO(&ret, &vi, pvi, _FALSE);
                     retcode = ccreate_prefix(retcode, p, PT, &vi);
                     break;
    }
  }
 #endif

  return(retcode);
}

_CHAR *get_text(_ANYTHING *p, _TextPuffer *PT, _MEM_VARLABEL *pmv, _BOOL prefixenable)
{
  return get_text_sub(p, PT, pmv, prefixenable, DEFSCOPE);
}

_CHAR *get_ANYPARA(_ANYPARA *p, _TextPuffer *PT)
{
  return get_text_sub(&p->anything, PT, &p->memvarlabel, _TRUE, DEFSCOPE);  
}

void delete_eventbuffer(void)
{
  Keyboard.DelBuffer();
}

_CHAR *StupidTextScheme(_DWORD value, _UDWORD schemeno)
{
  _CHAR        *retcode = NULL;
  _VARLABEL    vtmp;
  _MYSCHEME    myscheme;
  _ANYPARA     *pa;

  init_MYSCHEME_Makro(&myscheme);
  myscheme.schemeno = schemeno;

  init_VARLABEL_Makro(&vtmp);
  vtmp.no = 1;
  vtmp.info[0].state = CONST_VAL;
  vtmp.info[0].value = value;

  pa = TextScheme.Call(schemeno, &vtmp);
  if(pa != NULL)
    retcode = (_CHAR*)get_text(&pa->anything, NULL, NULL, _TRUE);

  return(retcode);
}

_BOOL result_addsub(void *v1, _RESULT_TYPE rt1, void *v2, _RESULT_TYPE rt2, _UWORD op, void *u_lim, void *o_lim)
{
  _REAL  tmpr, rr;
  _DWORD tmpd;
  _BOOL  retcode = _FALSE;

  if(rt1 == RES_CONSTANT)
    rt1 = RES_NUMERIC;
  if(rt2 == RES_CONSTANT)
    rt2 = RES_NUMERIC;

  if((rt1 == RES_REAL)&&(rt2 == RES_REAL))
  {
    switch(op)
    {
      case '+' : tmpr = get_pREAL((_REAL*)v1) + get_pREAL((_REAL*)v2);
                 retcode = _TRUE;
                 break;
      case '-' : tmpr = get_pREAL((_REAL*)v1) - get_pREAL((_REAL*)v2);
                 retcode = _TRUE;
                 break;
    }
  }
  else if((rt1 == RES_NUMERIC)&&(rt2 == RES_NUMERIC))
  {
    switch(op)
    {
      case '+' : tmpd = (*(_DWORD*)v1) + (*(_DWORD*)v2);
                 retcode = _TRUE;
                 break;
      case '-' : tmpd = (*(_DWORD*)v1) - (*(_DWORD*)v2);
                 retcode = _TRUE;
                 break;
    }
  }
  else if((rt1 == RES_REAL)&&(rt2 == RES_NUMERIC))
  {
    move_pREAL_DWORD(&rr, *(_DWORD*)v2);
    switch(op)
    {
      case '+' : tmpr = get_pREAL((_REAL*)v1) + rr;
                 retcode = _TRUE;
                 break;
      case '-' : tmpr = get_pREAL((_REAL*)v1) - rr;
                 retcode = _TRUE;
                 break;
    }
  }
  else if((rt1 == RES_NUMERIC)&&(rt2 == RES_REAL))
  {
    move_pREAL_DWORD(&rr, *(_DWORD*)v1);
    switch(op)
    {
#ifdef LASAL
      case '+' : tmpd  = FTOL(rr + get_pREAL((_REAL*)v2));
                 retcode = _TRUE;
                 break;
      case '-' : tmpd  = FTOL(rr - get_pREAL((_REAL*)v2));
                 retcode = _TRUE;
                 break;
#else
      case '+' : tmpd = (_DWORD)((_REAL)(*(_DWORD*)v1) + (*(_REAL*)v2));
                 retcode = _TRUE;
                 break;
      case '-' : tmpd = (_DWORD)((_REAL)(*(_DWORD*)v1) - (*(_REAL*)v2));
                 retcode = _TRUE;
                 break;
#endif
    }
  }
  
  
  if(retcode == _TRUE)
  {
    if(u_lim != NULL)
    {
      retcode = _FALSE;
      if(rt1 == RES_REAL)
      {
        if(tmpr >= get_pREAL((_REAL*)u_lim))
          retcode = _TRUE;
      }
      else if(rt1 == RES_NUMERIC)
      {
        if(tmpd >= (*(_DWORD*)u_lim))
          retcode = _TRUE;
      }
    }

    if((o_lim != NULL) && (retcode == _TRUE))
    {
      retcode = _FALSE;
      if(rt1 == RES_REAL)
      {
        if(tmpr <= get_pREAL((_REAL*)o_lim))
          retcode = _TRUE;
      }
      else if(rt1 == RES_NUMERIC)
      {
        if(tmpd <= (*(_DWORD*)o_lim))
          retcode = _TRUE;
      }
    }

    if(retcode == _TRUE)
    {
      if(rt1 == RES_REAL)
        move_pREAL_pREAL((_REAL*)v1, &tmpr);
      else
        *(_DWORD*)v1 = tmpd;
    }
  }

  return(retcode);
}


// **************************************************************************
// **************************************************************************
// **************************************************************************
// *
// *        For GNU compiler
// *
// **************************************************************************
// **************************************************************************
// **************************************************************************
#ifdef _GNUC
extern "C" void* memcpy( void *dest, const void *src, int count )
{
  return MemCpy(dest, const_cast<void*>(src), count);
}
#endif

_UWORD make_bmpno(void *p, _UWORD chr_size, _UWORD *anz)
{
  _UWORD  i = 0;
  _ASCII  *pa, tmp[100];
  _UNI    *pu;
  _UDWORD no;
  _DWORD  retcode = 0xFFFF;

  if(chr_size == sizeof(_ASCII))
  {
    pa = (_ASCII*)p;
    while((*pa != 93)&&(*pa != 0))
    {
      tmp[i] = *pa;
      i  += 1;
      pa += 1;
    }
    tmp[i] = 0;
  }
  else
  {
    pu = (_UNI*)p;
    while((*pu != 93)&&(*pu != 0))
    {
      tmp[i] = *(_ASCII*)pu;
      i  += 1;
      pu += 1;
    }
    tmp[i] = 0;
  }

  *anz = i+2;

  if(ImageList.LseLabel.Seek(&no, tmp) == _TRUE)
    retcode = no;

  return(retcode);
}

_UWORD icon_text_len_uni(_UNI *pu)
{
  _UWORD  no, anz, cnt = 0;
  _BMP    *bmp;
  _UNI    tmp[200];
  _UWORD  retcode = 0;

  while(*pu != 0)
  {
    if(*pu == '%')
    {
      pu++;
      if((*pu == 'B')||(*pu == 'b')||(*pu == 'I')||(*pu == 'i'))
      {
        if(cnt)
        {
          tmp[cnt] = 0;
          retcode += StrWidthLse(tmp, sizeof(_UNI));
          cnt = 0;
        }

        if(pu[1] == 91)
          no = make_bmpno(pu+2, sizeof(_UNI), &anz);
        else
          no = AToDsub(pu+1, sizeof(_UNI), &anz);

        pu += anz;

        bmp = ImageList.Get(no);
        if(bmp != NULL)
          retcode += bmp->width;
      }
      else
      {
        pu --;
        tmp[cnt] = *pu;
        cnt ++;
      }
    }
    else
    {
      tmp[cnt] = *pu;
      cnt ++;
    }

    pu ++;
  }

  if(cnt)
  {
    tmp[cnt] = 0;
    retcode += StrWidthLse(tmp, sizeof(_UNI));
  }

  return(retcode);
}

_UWORD icon_text_len_ascii(_ASCII *pa)
{
  _UWORD  no, anz, cnt = 0;
  _BMP    *bmp;
  _ASCII  tmp[200];
  _UWORD  retcode = 0;

  while(*pa != 0)
  {
    if(*pa == '%')
    {
      pa++;
      if((*pa == 'B')||(*pa == 'b')||(*pa == 'I')||(*pa == 'i'))
      {
        if(cnt)
        {
          tmp[cnt] = 0;
          retcode += StrWidth(tmp, sizeof(_ASCII));
          cnt = 0;
        }

        if(pa[1] == 91)
          no = make_bmpno(pa+2, sizeof(_ASCII), &anz);
        else
          no = AToDsub(pa+1, sizeof(_ASCII), &anz);

        pa += anz;

        bmp = ImageList.Get(no);
        if(bmp != NULL)
          retcode += bmp->width;
      }
      else
      {
        pa --;
        tmp[cnt] = *pa;
        cnt ++;
      }
    }
    else
    {
      tmp[cnt] = *pa;
      cnt ++;
    }

    pa ++;
  }

  if(cnt)
  {
    tmp[cnt] = 0;
    retcode += StrWidth(tmp, sizeof(_ASCII));
  }

  return(retcode);
}

/*
_UWORD icon_text_len_ascii(_ASCII *txt)
{
  _UWORD retcode = 0;
  _ASCII *pa, *pd;
  _UWORD no, cnt;
  _ASCII tmp[200];
  _BMP   *bmp;

  pa     = txt;
  pd     = tmp;
  tmp[0] = 0;

  while(*pa != 0)
  {
    if(*pa == '%')
    {
      pa++;
      if((*pa == 'B')||(*pa == 'b'))
      {
        if(pa[1] == 92)
          no = make_bmpno(pa+2, sizeof(_ASCII), &cnt);
        else
          no  = AToDsub(pa+1, sizeof(_ASCII), &cnt);

        pa += cnt;
        bmp = ImageList.Get(no);

        if(bmp != NULL)
          retcode += bmp->width;
      }
    }
    else
    {
      *pd = *pa;
      pd ++;
    }

    pa ++;
  }

  *pd = 0;
  retcode += StrWidth(tmp, sizeof(_ASCII));

  return(retcode);
}

_UWORD icon_text_len_uni(_UNI *txt)
{
  _UWORD retcode = 0;
  _UNI   *pu, *pd;
  _UWORD no, cnt;
  _UNI   tmp[200];
  _BMP   *bmp;

  pu     = txt;
  pd     = tmp;
  tmp[0] = 0;

  while(*pu != 0)
  {
    if(*pu == '%')
    {
      pu++;
      if((*pu == 'B')||(*pu == 'b'))
      {
        if(pu[1] == 92)
          no = make_bmpno(pu+2, sizeof(_UNI), &cnt);
        else
          no = AToDsub(pu+1, sizeof(_UNI), &cnt);

        pu += cnt;
        bmp = ImageList.Get(no);
        if(bmp != NULL)
          retcode += bmp->width;
      }
    }
    else
    {
      *pd = *pu;
      pd ++;
    }

    pu ++;
  }

  *pd = 0;
  retcode += StrWidth(tmp, sizeof(_ASCII));

  return(retcode);
}
*/

_UWORD icon_text_len(void *txt, _UWORD chr_size)
{
  _UWORD retcode;

  if(chr_size == 1)
    retcode = icon_text_len_ascii((_ASCII*)txt);
  else
    retcode = icon_text_len_uni((_UNI*)txt);

  return(retcode);
}

void icon_text_out_ASCII(_KOORD x, _KOORD y, _ASCII *txt, _ATTRIB attrib, _COLOR color)
{
  _ASCII  *pa;
  _UWORD  no, anz, cnt = 0;
  _BMP    *bmp;
  _KOORD  my;
  _ASCII  tmp[200];
  _ATTRIB tattr;

  pa    = txt;
  tattr = (attrib&~T_HORIZBOUND)|T_LEFTBOUND;

  while(*pa != 0)
  {
    if(*pa == '%')
    {
      pa++;
      if((*pa == 'B')||(*pa == 'b')||(*pa == 'I')||(*pa == 'i'))
      {
        if(cnt)
        {
          tmp[cnt] = 0;
          OutTextXY(x, y, tmp, tattr, color, sizeof(_ASCII));
          x += StrWidth(tmp, sizeof(_ASCII));
          cnt = 0;
        }

        if(pa[1] == 91)
          no = make_bmpno(pa+2, sizeof(_ASCII), &anz);
        else
          no = AToDsub(pa+1, sizeof(_ASCII), &anz);

        pa += anz;
        bmp = ImageList.Get(no);

        if(bmp != NULL)
        {
          switch(attrib & T_VERTBOUND)
          {
            case T_DOWNBOUND : my = y - bmp->height;   break;
            case T_MIDBOUND  : my = y - bmp->height/2; break;
            default          : my = y;                 break;
          }

          Bitmap(x, my, bmp, T_COPY|T_SOLID, 0);
          x += bmp->width;
        }
      }
      else
      {
        pa --;
        tmp[cnt] = *pa;
        cnt ++;
      }
    }
    else
    {
      tmp[cnt] = *pa;
      cnt ++;
    }

    pa ++;
  }

  if(cnt)
  {
    tmp[cnt] = 0;
    OutTextLse(x, y, tmp, tattr, color, sizeof(_ASCII));
  }
}

void icon_text_out_UNI(_KOORD x, _KOORD y, _UNI *txt, _ATTRIB attrib, _COLOR color)
{
  _UNI    *pu;
  _UWORD  no, anz, cnt = 0;
  _BMP    *bmp;
  _KOORD  my;
  _UNI    tmp[200];
  _ATTRIB tattr;

  pu    = txt;
  tattr = (attrib&~T_HORIZBOUND)|T_LEFTBOUND;

  while(*pu != 0)
  {
    if(*pu == '%')
    {
      pu++;
      if((*pu == 'B')||(*pu == 'b')||(*pu == 'I')||(*pu == 'i'))
      {
        if(cnt)
        {
          tmp[cnt] = 0;
          OutTextLse(x, y, tmp, tattr, color, sizeof(_UNI));
          x += StrWidthLse(tmp, sizeof(_UNI));
          cnt = 0;
        }

        if(pu[1] == 91)
          no = make_bmpno(pu+2, sizeof(_UNI), &anz);
        else
          no = AToDsub(pu+1, sizeof(_UNI), &anz);

        pu += anz;

        bmp = ImageList.Get(no);
        if(bmp != NULL)
        {
          switch(attrib & T_VERTBOUND)
          {
            case T_DOWNBOUND : my = y - bmp->height;   break;
            case T_MIDBOUND  : my = y - bmp->height/2; break;
            default          : my = y;                 break;
          }

          Bitmap(x, my, bmp, T_COPY|T_SOLID, 0);
          x += bmp->width;
        }
      }
      else
      {
        pu --;
        tmp[cnt] = *pu;
        cnt ++;
      }
    }
    else
    {
      tmp[cnt] = *pu;
      cnt ++;
    }

    pu ++;
  }

  if(cnt)
  {
    tmp[cnt] = 0;
    OutTextLse(x, y, tmp, tattr, color, sizeof(_UNI));
  }
}

void icon_text_out(_KOORD x, _KOORD y, void *txt, _UWORD chr_size, _ATTRIB attrib, _COLOR color)
{
  if(chr_size == sizeof(_ASCII))
    icon_text_out_ASCII(x, y, (_ASCII*)txt, attrib, color);
  else
    icon_text_out_UNI(x, y, (_UNI*)txt, attrib, color);
}

_UWORD StrPercent(void *txt, _UWORD chr1, _UWORD chr2, _UWORD chr3, _UWORD chr4, _UWORD chr_size)
{
  _UWORD retcode = 0;
  _ASCII *pa;
  _UNI   *pu;

  if(chr_size == sizeof(_ASCII))
  {
    pa = (_ASCII*)txt;
    while(*pa)
    {
      if(*pa == '%')
      {
        pa ++;
        if((*pa == chr1)||(*pa == chr2)||(*pa == chr3)||(*pa == chr4))
          return(retcode);
      }
      retcode += 1;
      pa      ++;
    }
  }
  else
  {
    pu = (_UNI*)txt;
    while(*pu)
    {
      if(*pu == '%')
      {
        pu ++;
        if((*pu == chr1)||(*pu == chr2)||(*pu == chr3)||(*pu == chr4))
          return(retcode);
      }
      retcode += 1;
      pu      ++;
    }
  }

  return(0xFFFF);
}

void OutTextIconXY(_KOORD x, _KOORD y, void *txt, _ATTRIB attrib, _COLOR col, _UWORD chr_size)
{
  if(StrPercent(txt, 'b', 'B', 'i', 'I', chr_size) != 0xFFFF)
  {
    switch(attrib & T_HORIZBOUND)
    {
      case T_RIGHTBOUND  : x -= icon_text_len(txt, chr_size);   break;
      case T_CENTERBOUND : x -= icon_text_len(txt, chr_size)/2; break;
    }

    icon_text_out(x, y, txt, chr_size, attrib, col);
  }
  else
  {
    OutTextLse(x, y, txt, attrib, col, chr_size);
  }
}

_BOOL out_BUTTON_sub(_DOT *dot, _Window *pw, _SubPicture *Sp, _BOOL state, _BOOL press, _DOT *offset, _ZOOM *zoom)
{
  _PICTUREMASK *pm;
  _ROOM        room;
  _UWORD       i;
  _BUTTON      *pb;
  _OBJECT      *po;
  _DOT         tmpoff;
  _ZOOM        tmpzoom;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    pb = pm->membutton.ptr;
    for(i=0; i<pm->membutton.no; i++)
    {
      room = pb->room;
      Zoom_ROOM(&room, offset, zoom);

      if((dot->x >= room.xy1.x)&&(dot->x <= room.xy2.x)&&(dot->y >= room.xy1.y)&&(dot->y <= room.xy2.y))
      {
        if(access_button(pb, _FALSE, _SELECTOR_DEFAULT) == _TRUE)
        {
          if(press == _TRUE)
          {
            Sp->DoButton(pb, _TRUE, pw, offset, zoom, _FALSE, _FALSE);
            Sp->DoButton(pb, _FALSE, pw, offset, zoom, _FALSE, _FALSE);
          }
          else
          {
            pb->selected = state;
            Sp->Out_BUTTON(pb, offset, zoom, DEFATTRIB);
          }
          return(_TRUE);
        }
      }
      pb++;
    }

    po = pm->memobject.ptr;
    for(i=0; i<pm->memobject.no; i++)
    {
      if(po->SubPicture != NULL)
      {
        tmpoff    = *offset;
        tmpoff.x += Zoom(zoom->x, po->xy.x);
        tmpoff.y += Zoom(zoom->y, po->xy.y);
        tmpzoom.x = Zoom_ZOOM(po->zoom.x, zoom->x);
        tmpzoom.y = Zoom_ZOOM(po->zoom.y, zoom->y);

        if(out_BUTTON_sub(dot, pw, po->SubPicture, state, press, &tmpoff, &tmpzoom) == _TRUE)
          return(_TRUE);
      }
      po += 1;
    }
  }

  return(_FALSE);
}

_BOOL out_BUTTON(_DOT *dot, _BOOL state, _BOOL press)
{
  _Window     *Pwin;
  _Picture    *Ppic;
  _MY_PICTURE *mp;
  _SubPicture *Spic;
  _DOT        offset;
  _ZOOM       zoom;
  _BOOL       retcode = _FALSE;
  _UWORD      no;

  Pwin = Root.FindTop();
  Ppic = Pwin->GetP_Picture();

  if(Ppic != NULL)
  {
    mp = Ppic->GetMyPicture();
    no = mp->No;

    init_DOT(&offset);
    init_ZOOM(&zoom);

    while(no)
    {
      no     -= 1;
      Spic    = &mp->Ptr[no];
      if(out_BUTTON_sub(dot, Pwin, Spic, state, press, &offset, &zoom) == _TRUE)
      {
        retcode = _TRUE;
        no = 0;
      }
    }
  }
  return(retcode);
}

_BOOL lookup_STACK(_STACK *p, _PIC_COMMAND cmd)
{
  _UWORD      no;
  _STACK_LINE *ptr;

  no  = p->no;
  ptr = p->ptr;
  while(no)
  {
    if(ptr->cmd == cmd)
      return(_TRUE);
    no  --;
    ptr ++;
  }

  return(_FALSE);
}

_BOOL lookup_LSEFUNCT(_LSEFUNCT *p, _PIC_COMMAND cmd)
{
  _Stack *PStack;

  if(p->no < InterBoxList.GetNo())
  {
    PStack = InterBoxList.Get(p->no);
    if(PStack != NULL)
    {
      if(lookup_STACK(PStack->GetStack(), cmd) == _TRUE)
        return(_TRUE);
      return(_FALSE);
    }
  }

  return(lookup_STACK(&p->overload, cmd));
}


_ASCII_BUFFER* get_label_buffer(_ANYLST list)
{
  switch(list)
  {
    case ALARMLIST   : return Alarm.LseLabel.GetAsciiBuffer();
    case MENULIST    : return MenuList.LseLabel.GetAsciiBuffer();
    case ICONLST     : return ImageList.LseLabel.GetAsciiBuffer();
    case FONTLST     : return Font.LseLabel.GetAsciiBuffer();
    case OBJLST      : return ObjectList.LseLabel.GetAsciiBuffer();
    case FUNCTIONLST : return InterBoxList.LseLabel.GetAsciiBuffer();
    case CHKBITLST   : return Sequencer.LseLabel.GetAsciiBuffer();
    case TEXTSCHEME  : return TextScheme.LseLabel.GetAsciiBuffer();
    case IMAGESCHEME : return ImageScheme.LseLabel.GetAsciiBuffer();
    case COLORSCHEME : return ColorScheme.LseLabel.GetAsciiBuffer();
    case STATESCHEME : return StateScheme.LseLabel.GetAsciiBuffer();
    //case VARUNIT     : retcode = VarList.GetUnitNo(label);
    //case VARNAME     : retcode = VarList.GetVarNo(label, station);
  }

  return(NULL);
}

_UDWORD get_no_by_label(_ASCII *label, _ANYLST list, _UWORD station)
{
  _UDWORD retcode = DEFSCOPE;
  _PRJ_ENTRY *pe;

  switch(list)
  {
    case ALARMLIST   : if(Alarm.LseLabel.Seek(&retcode, label) == _FALSE)
                         retcode = DEFSCOPE;
                       break;
    case MENULIST    : if(MenuList.LseLabel.Seek(&retcode, label) == _FALSE)
                         retcode = DEFSCOPE;
                       break;
    case ICONLST     : if(ImageList.LseLabel.Seek(&retcode, label) == _FALSE)
                         retcode = DEFSCOPE;
                       break;
    case FONTLST     : if(Font.LseLabel.Seek(&retcode, label) == _FALSE)
                         retcode = DEFSCOPE;
                       break;
    case OBJLST      : if(ObjectList.LseLabel.Seek(&retcode, label) == _FALSE)
                         retcode = DEFSCOPE;
                       break;
    case FUNCTIONLST : if(InterBoxList.LseLabel.Seek(&retcode, label) == _FALSE)
                         retcode = DEFSCOPE;
                       break;
    case CHKBITLST   : if(Sequencer.LseLabel.Seek(&retcode, label) == _FALSE)
                         retcode = DEFSCOPE;
                       break;
    case VARUNIT     : retcode = VarList.GetUnitNo(label);
                       if(retcode == 0xFFFF)
                         retcode = DEFSCOPE;
                       break;
    case VARNAME     : retcode = VarList.GetVarNo(label, station);
                       if(retcode > VarList.GetNo())
                         retcode = DEFSCOPE;
                       break;
    case TEXTSCHEME  : if(TextScheme.LseLabel.Seek(&retcode, label) == _FALSE)
                         retcode = DEFSCOPE;
                       break;
    case IMAGESCHEME : if(ImageScheme.LseLabel.Seek(&retcode, label) == _FALSE)
                         retcode = DEFSCOPE;
                       break;
    case COLORSCHEME : if(ColorScheme.LseLabel.Seek(&retcode, label) == _FALSE)
                         retcode = DEFSCOPE;
                       break;
    case STATESCHEME : if(StateScheme.LseLabel.Seek(&retcode, label) == _FALSE)
                         retcode = DEFSCOPE;
                       break;
    case PRJSCREEN   : pe = Project.GetEntryByLabel(label, _PE_PICTURE);
                       if(pe != NULL)
                         retcode = pe->no;
                       break;
    case PRJWINDOW   : pe = Project.GetEntryByLabel(label, _PE_WINDOW);
                       if(pe != NULL)
                         retcode = pe->no;
                       break;
    case FONTSCHEME  : retcode = FontScheme.GetIndexByLabel(label);
                       break;
    default          : break;
  }

  return(retcode);
}

void GetPictureSize(_ROOM *room)
{
  _SOFTKEYSETUP *pss;

  MyGetScreenSize(room);
  pss = SetUp.Get_SOFTKEYSETUP();

  if(pss->horizontal == _TRUE)
  {
    switch(pss->align & T_VERTBOUND)
    {
      case T_UPBOUND    : room->xy1.y += pss->height; break;
      case T_DOWNBOUND  : room->xy2.y -= pss->height; break;
    }
  }
  else
  {
    switch(pss->align & T_HORIZBOUND)
    {
      case T_LEFTBOUND  : room->xy1.x += pss->width; break;
      case T_RIGHTBOUND : room->xy2.x -= pss->width; break;
    }
  }
}

void find_ROOM_mitte_y(_ROOM *d, _ROOM *s, _ROOM *p)
{
  _UWORD h = (d->xy2.y - d->xy1.y + 1);
  _WORD  k= 0;

  d->xy1.y = (s->xy1.y + s->xy2.y - h) / 2;
  d->xy2.y = d->xy1.y + h - 1;

  if(p->xy2.y < d->xy2.y)
    k = p->xy2.y - d->xy2.y;
  if(d->xy1.y < p->xy1.y)
    k = p->xy1.y - d->xy1.y;

  d->xy1.y += k;
  d->xy2.y += k;
}

void find_ROOM_mitte_x(_ROOM *d, _ROOM *s, _ROOM *p)
{
  _UWORD b = (d->xy2.x - d->xy1.x + 1);
  _WORD  k = 0;

  d->xy1.x = (s->xy1.x + s->xy2.x - b)/2;
  d->xy2.x = d->xy1.x + b - 1;

  if(p->xy2.x < d->xy2.x)
    k = p->xy2.x - d->xy2.x;
  if(d->xy1.x < p->xy1.x)
    k = p->xy1.x - d->xy1.x;

  d->xy1.x += k;
  d->xy2.x += k;
}

_BOOL find_ROOM(_ROOM *d, _ROOM *s, _ROOM *p, _BOOL shadow)
{
  _BOOL  retcode = _TRUE;
  _UWORD b, h;
  _ROOM  src   = *s;
  _ROOM  place = *p;

  if(src.xy1.x > place.xy2.x) // falls s ausserhalb p
    src.xy1.x = place.xy2.x;
  if(src.xy1.y > place.xy2.y) // falls s ausserhalb p
    src.xy1.y = place.xy2.y;
  if(src.xy2.x < place.xy1.x) // falls s ausserhalb p
    src.xy2.x = place.xy1.x;
  if(src.xy2.y < place.xy1.y) // falls s ausserhalb p
    src.xy2.y = place.xy1.y;

  src.xy1.x -= 2;
  src.xy2.x += 2;
  src.xy1.y -= 2;
  src.xy2.y += 2;

  if(shadow == _TRUE)
  {
    src.xy1.x -= POPUP_SHADOW;
    src.xy1.y -= POPUP_SHADOW;

    place.xy2.x -= POPUP_SHADOW;
    place.xy2.y -= POPUP_SHADOW;
  }

  b = (d->xy2.x - d->xy1.x);
  h = (d->xy2.y - d->xy1.y);

  if((s->xy2.x + b) < place.xy2.x) // rechts
  {
    d->xy1.x = src.xy2.x;
    d->xy2.x = d->xy1.x + b;
    find_ROOM_mitte_y(d, &src, &place);
  }
  else if((place.xy1.x + b) < src.xy1.x) // links
  {
    d->xy2.x = src.xy1.x - 1;
    d->xy1.x = d->xy2.x - b;
    find_ROOM_mitte_y(d, &src, &place);
  }
  else if((src.xy2.y + h) < place.xy2.y) // unten
  {
    d->xy1.y = src.xy2.y;
    d->xy2.y = d->xy2.y + h;
    find_ROOM_mitte_x(d, &src, &place);
  }
  else if((place.xy1.y + h) < src.xy1.y) // oben
  {
    d->xy2.y = src.xy1.y - 1;
    d->xy1.y = d->xy2.y - h;
    find_ROOM_mitte_x(d, &src, &place);
  }
  else
  {
    find_ROOM_mitte_y(d, &src, &place);
    find_ROOM_mitte_x(d, &src, &place);
    retcode = _FALSE;
  }

  return(retcode);
}

_BOOL add_MEM_NO(_MEM_NO *p, _UDWORD nummer)
{
  _BOOL   retcode = _FALSE;
  _UDWORD size = (p->no + 1) * sizeof(_UDWORD);

  if(Memory2((void**)&p->ptr, size) != 0)
  {
    p->ptr[p->no] = nummer;
    p->no += 1;
    retcode = _TRUE;
  }

  return(retcode);
}

void make_version_text(_ASCII *txt, _UDWORD no, _UWORD format)
{
  if(no >= 0x1000)
  {
    DToA(txt, (no >>12)&0x0F, 0x0102, sizeof(_ASCII)); txt += 2;
    *txt = '.'; txt += 1;
    DToA(txt, (no >> 8)&0x0F, 0x0102, sizeof(_ASCII)); txt += 2;
    *txt = '.'; txt += 1;
    DToA(txt, (no)     &0xFF, 0x0103, sizeof(_ASCII)); txt += 3;
    *txt = 0;
  }
  else
  {
    DToA(txt, no, format, sizeof(_ASCII));
  }
}

_UBYTE make_reftime_bitno(_UWORD time, _UBYTE bitno)
{
  if(time <= 10)
    time = 2;
  else if(time <= 50)
    time = 1;
  else if(time <= 100)
    time = 0;
  else if(time <= 500)
    time = 3;
  else
    time = 0;

  return((time << 5) | bitno);
}

_UWORD get_reftime(_LSEVARINFO *plvi)
{
  switch(plvi->reftime_bitno >> 5)
  {
    case 0 : return(100);
    case 1 : return(50);
    case 2 : return(10);
    case 3 : return(500);
  }

  return(100);
}

_UDWORD memorysize(_UWORD no, _UWORD size)
{
  _UDWORD tmp, retcode = 0;

  if(no > 0)
  {
    retcode += 8;
    tmp = ((no * size) + 3) / 4;
    retcode += tmp * 4;
  }

  return(retcode);
}

_UWORD MenuList_GetNo(void)
{
  return(MenuList.GetNo());
}

_UWORD VarList_GetSingleUnitNo(_UDWORD varno)
{
  _UDWORD     retcode = 0xFFFF;
  _UDWORD     unitno;
  _LSEVARINFO *pvi;
  _MYSCHEME   myscheme;

  if(VarList.GetVarInfo(&pvi, varno) == _TRUE)
  {
    if((pvi->vartype == BINARY) || (pvi->vartype == FCONSTANT))
    {
      init_MYSCHEME_Makro(&myscheme);
      myscheme.schemeno = pvi->funitscheme;
      retcode = pvi->funit;
      if(UnitScheme.Call(&unitno, &myscheme, NULL, pvi->funit) == _TRUE)
        retcode = unitno;

      if(VarList.GetUnitInfo(retcode) == NULL)
        retcode = 0xFFFF;
    }
  }

  return(retcode);
}

void txt1000(_ASCII *txt, _UDWORD value)
{
  _UWORD format = 0x0800;

  if(value > 999)
  {
    txt1000(txt, value / 1000);
    StrCat(txt, sizeof(_ASCII), ".", sizeof(_ASCII));
    format = 0x103;
  }

  DToA(&txt[StrLen(txt, sizeof(_ASCII))], value % 1000, format, sizeof(_ASCII));
}

void text_1000(_ASCII *txt, _UDWORD value)
{
  txt[0] = 0;
  txt1000(txt, value);
}

_UWORD GetImage2(_IMAGE *pi, _KOORD x1, _KOORD y1, _KOORD x2, _KOORD y2)
{
  if(GetImage(pi, x1, y1, x2, y2) != 0)
  {
    pi->room.xy2.x -= 1;
    pi->room.xy2.y -= 1;
  }
  return(0);
}

void PutImage2(_IMAGE *pi, _KOORD x1, _KOORD y1)
{
  _IMAGE tmp = *pi;
  tmp.room.xy2.x += 1;
  tmp.room.xy2.y += 1;
  PutImage(&tmp, x1, y1);
}

_BOOL IsRamFile(const _ASCII *dpne)
{
  if(*(_UDWORD*)dpne == 0x404D4152) // "RAM@"
    return _TRUE;
    
  if(*(_UDWORD*)dpne == 0x406D6172) // "ram@"
    return _TRUE;
  
  return _FALSE;
}

_ASCII *ClearRamFile(_ASCII *dpne)
{
  if(IsRamFile(dpne) == _TRUE)
    return(dpne + 4);

  return(dpne);
}

_ASCII *MakeRamFile(_ASCII *dst, _ASCII *dpne)
{
  if(IsRamFile(dpne) == _FALSE)
  {
    MemMove(&dst[4], dpne, StrLen(dpne, sizeof(_ASCII))+1);
    MemCpy(dst, "RAM@", 4);
    return(dst);
  }
  else if(dst != dpne)
  {
    StrCpy(dst, sizeof(_ASCII), dpne, sizeof(_ASCII));
  }

  return(dpne);
}

_UWORD GetDriveList(_ASCII *dst)
{
  _ASCII drvlst[30], *ph;
  _UWORD i, retcode = 0;

  MemSet(drvlst, 0, sizeof(drvlst));
  OS_GetDriveListShort(drvlst);
  
  for(i=0, ph=drvlst; i<26; i++, ph++)
	{
	  if(ph[0] != 0)
	  {
	    *dst    = 'A' + i;
      dst     += 1;
      retcode += 1;
    }
  }

  *dst = 0;

  return(retcode);
}

_FILEXT Textfile_FILEXT(_FILEXT ext)
{
  switch(ext)
  {
    case FILEXT_REX :
    case FILEXT_HTML:
    case FILEXT_XML :
    case FILEXT_TXT :
    case FILEXT_LOG :
    case FILEXT_BAK :
    case FILEXT_LSL :
    case FILEXT_INI :
    case FILEXT_C   :
    case FILEXT_ST  :
    case FILEXT_CPP :
    case FILEXT_H   : return(FILEXT_TXT);
    case FILEXT_BMP :
    //case FILEXT_ICO :
    case FILEXT_GIF :
    case FILEXT_JPG :
    case FILEXT_BMO : return(FILEXT_BMO);
    default         : break;
  }

  return(FILEXT_UNKNOWN);
}

_FILEXT GetFiletype(_ASCII *dpne)
{
  _ASCII  *pa, txt[_MAX_PATHLENGTH];
  _FILEXT retcode = FILEXT_UNKNOWN;
  _UWORD  len;

  if(dpne != NULL)
  {
    pa = NULL;
    while(*dpne)
    {
      if(*dpne == '.')
        pa = dpne + 1;
      dpne += 1;
    }

    if(pa != NULL)
    {
      StrCpy(txt, sizeof(_ASCII), pa, sizeof(_ASCII));
      StrUp(txt, sizeof(_ASCII), 0);

      len = StrLen(txt, sizeof(_ASCII));
      if(len == 4)
      {
        if(MemCmp(txt, "JPEG", 4) == 0)
          retcode = FILEXT_JPG;
        else if(MemCmp(txt, "HTML", 4) == 0)
          retcode = FILEXT_HTML;
      }
      else if(len == 3)
      {
        if(MemCmp(txt, "BMP", 3) == 0)
          retcode = FILEXT_BMP;
        else if(MemCmp(txt, "GIF", 3) == 0)
          retcode = FILEXT_GIF;
        else if(MemCmp(txt, "JPG", 3) == 0)
          retcode = FILEXT_JPG;
        else if(MemCmp(txt, "BMO", 3) == 0)
          retcode = FILEXT_BMO;
        else if(MemCmp(txt, "TXT", 3) == 0)
          retcode = FILEXT_TXT;
        //else if(MemCmp(txt, "ICO", 3) == 0) // FILEXT_ICO
          //retcode = FILEXT_ICO;
        else if(MemCmp(txt, "LOG", 3) == 0)
          retcode = FILEXT_LOG;
        else if(MemCmp(txt, "BAK", 3) == 0)
          retcode = FILEXT_BAK;
        else if(MemCmp(txt, "LSL", 3) == 0)
          retcode = FILEXT_LSL;
        else if(MemCmp(txt, "INI", 3) == 0)
          retcode = FILEXT_INI;
        else if(MemCmp(txt, "CPP", 3) == 0)
          retcode = FILEXT_CPP;
        else if(MemCmp(txt, "XML", 3) == 0)
          retcode = FILEXT_XML;
        else if(MemCmp(txt, "HTM", 3) == 0)
          retcode = FILEXT_HTML;
        else if(MemCmp(txt, "REX", 3) == 0)
          retcode = FILEXT_REX;
      }
      else if(len == 2)
      {
        if(MemCmp(txt, "ST", 2) == 0)
          retcode = FILEXT_ST;
      }
      else if(len == 1)
      {
        if(txt[0] == 'C')
          retcode = FILEXT_C;
        else if(txt[0] == 'H')
          retcode = FILEXT_H;
      }
    }
  }

  return(retcode);
}

_LSEMENU *get_LSEMENU(_UDWORD no)
{
  return(MenuList.Get_LSEMENU(no));
}


#ifdef USER_DEKEMA // LeiChr 30.01.2015
extern "C" _BOOL GetGrayMode(void); // function is done in dekema-application
#endif

void transform_color_to_gray(_UWORD *pcol)
{
  _UWORD col = *pcol;
  
 #ifdef USER_DEKEMA // LeiChr 30.01.2015
  if(GetGrayMode() == _FALSE)
  {
    return;
  }
 #endif
  
  if(col != INVISIBLE)
  {
	  _UWORD r = ((((col >> 11) & 0x001F) * 255) / 31);
	  _UWORD g = ((((col >> 5) & 0x003F) * 255) / 63);
	  _UWORD b = (((col & 0x001F) * 255) / 31);
  
    //_UWORD gray = (r + g + b) / 3;
    _UWORD gray = (3*r + 6*g + b) / 10;
    
/*    
    if(gray < 32)
      gray = 0;
    else if(gray < 96)
      gray = 64;
    else if(gray < 160)
      gray = 128;
    else if(gray < 224)
      gray = 196;
    else
      gray = 255;
*/

    r = gray >> 3;
    g = gray >> 2;
    b = gray >> 3;
  
    *pcol = (b << 11) | (g << 5) | r;
  }
}

_COLOR transform_to_gray(_COLOR col)
{
  _UWORD lo = GetPenColor(col);
  _UWORD hi = GetBackColor(col);
  transform_color_to_gray(&lo);
  transform_color_to_gray(&hi);
  return MakeColor(hi, lo);
}

void transform_BMP_to_gray(_BMP *p)
{
  _UDWORD i;
  _BMPDAT *pc = p->ptr;
  
 #ifdef USER_DEKEMA // LeiChr 30.01.2015
  if(GetGrayMode() == _FALSE)
  {
    return;
  }
 #endif
  
  //if(p->state & BMP_HICOLOR) // nicht aktivieren !!
  {
    if(p->state & BMP_NO_TRANSPARENT) 
    {
      for(pc=p->ptr, i=0; i<p->datano; i++, pc++)
        transform_color_to_gray(&pc->col);
    }
    else
    {
      for(pc=p->ptr, i=0; i<p->datano; i++, pc++)
      {
        if(pc->col != p->transparent)
          transform_color_to_gray(&pc->col);
      }
    }
  }
}

void delete_bit(_ASCII *label)
{
  _ASCII chr;
  _UWORD cnt = 0;

  chr = *label;
  while(chr)
  {
    if(chr == 46)
    {
      if(cnt > 0)
      {
        *label = 0;
        return;
      }
      cnt   += 1;
    }

    label += 1;
    chr   = *label;
  }
}

#ifdef UC_COMPLEX_SERVER
void delete_element(_ASCII *label)
{
  _UWORD len, i;
  _ASCII *chr;
  _BOOL first = _FALSE;


  len = StrLen(label, sizeof(_ASCII));
  chr = label;

  for(i=0; i<len; i++)
  {
    if(*chr == '.')
    {
      if(first == _TRUE)
      {
        *chr = 0;
        break;
      }
      first = _TRUE;
    }

    chr++;
  }

}
#endif

_ASCII *StrCutLastTokenAndReturn(_ASCII *dst, _ASCII *dpne)
{
  _ASCII chr, *pe = NULL;
  _ASCII *retcode = NULL;

  do
  {
    chr  = *dpne;
    *dst = chr;

    dst ++;
    dpne ++;
  
    if(chr == 92)
    {
      retcode = dpne;
      pe      = dst;
    }
  }
  while(chr != 0);
  
  if(pe != NULL)
    *pe = 0;
    
  return retcode;
}

_ASCII *GetLastToken(_ASCII *dpne)
{
  _ASCII *retcode = NULL;

  while(1)
  {
    switch(*dpne)
    {
      case 92: retcode = dpne + 1;
        break;
      case 0 : return retcode;
    }
    dpne ++;
  }
  
  return retcode;
}

void AddTokenEnd(_ASCII *p)
{
  _UWORD len = StrLen(p, sizeof(_ASCII));
  if(len > 0)
  {
    if(p[len-1] == '\\')
      return;
  }    
  
  p[len] = '\\';
  p[len+1] = 0;
}


void DelTokenEnd(_ASCII *p)
{
  _UWORD len = StrLen(p, sizeof(_ASCII));
  if(p[len-1] == '\\')
    p[len-1] = 0;
}

_BOOL FileExist(_ASCII *dpne)
{
  if(IsRamFile(dpne) == _TRUE)
    dpne += 4;

  _Handle hand;
  if(hand.Open(LOAD, dpne) == _TRUE)
  {
    hand.Close();
    return _TRUE;
  }
  return _FALSE;
}

void GetActDateTime(_UDWORD *date, _UDWORD *time)
{
  SYSTIME tim;
  SYSDATE dat;
  
  OS_SSR_GetSysTime(&tim); // LSL_POS->piSSR->SSR_SetSysTime
  OS_SSR_GetSysDate(&dat); // LSL_POS->piSSR->SSR_SetSysDate
      
  _UDWORD tmp;
  tmp   = tim.wHour & 0xFF; 
  tmp   = (tmp << 8) | (tim.wMinute & 0xFF); 
  tmp   = (tmp << 8) | (tim.wSecond & 0xFF);
  *time = (tmp << 8); 

  tmp   = dat.wYear; 
  tmp   = (tmp << 4) | dat.wMonth; 
  tmp   = (tmp << 8) | dat.wDay;
  tmp   = (tmp << 4) | dat.wDayOfWeek; 
  *date = tmp;
}

_UBYTE make_WOSE(_ATTRIB attrib)
{
  _BYTE wose = 0;
  switch(attrib & T_HORIZBOUND)
  {
    case T_LEFTBOUND  : wose |= 1<<2; break;
    case T_RIGHTBOUND : wose |= 2<<2; break;
  }
  switch(attrib & T_VERTBOUND)
  {
    case T_UPBOUND   : wose |= 1; break;
    case T_DOWNBOUND : wose |= 2; break;
  }
  return wose;
}

void DeleteFinalZeroAfterDezimalPointSub(_CHAR *ptxt)
{
  _CHAR *pl = NULL;
  
  if(*ptxt != 0)
  {
    ptxt += 1;
  
    while(*ptxt != 0)
    {
      if(*ptxt == '0')
      {
        if(pl == NULL)
          pl = ptxt;
      }
      else
      {
        pl = NULL;
      }
    
      ptxt += 1;
    }
  
    if(pl != NULL)
      *pl = 0;
  }
}

void DeleteFinalZeroAfterDezimalPoint(_CHAR *ptxt)
{
  while(*ptxt != 0)
  {
    if((*ptxt == '.')||(*ptxt == ','))
    {
      DeleteFinalZeroAfterDezimalPointSub(ptxt + 1);
      return;
    }
    
    ptxt += 1;
  }
}

#ifdef USER_DEMAG
_BOOL switch_momentary_stationary(_VARLABEL *varlabel, _BOOL lrm)
{
  _IVALUE   value;
  _VARIABLE variable;
  _RESULT   result;
  _BOOL     retcode = _FALSE;

  if(VarList.GetValue(varlabel, &value, _TRUE) == _TRUE)
  {
    init_RESULT(&result);
    result.value = (value == 0)? 1 : 0;
    result.rmin  = result.value;
    result.rmax  = result.value;
    result.ftype = RES_CONSTANT;
    set_lrm(&result, lrm);
    retcode = VarList.SetSystemData(&result, &variable, varlabel, _TRUE);
  }

  return(retcode);
}
#endif

_CHAR *MakeStars(_CHAR *dst, _UDWORD maxno, _CHAR *src)
{
  if((maxno > 0) && (dst != NULL))
  {
    _UDWORD nox = 0;
    if(src != NULL)
    {
      nox = StrLen(src, sizeof(_CHAR));
    }
  
    if(nox >= maxno)
    {
      nox = maxno - 1;
    }
    
    _CHAR *ph = dst;
    while(nox--)
    {
      *ph++ = '*';
    }
    *ph = 0;
  }
  
  return dst;
}
