#include "T_IPC.H"
#include "T_NUC.H"

#define CPYEDIT ' '
#define DELAY_CURSORJUMP 1600
//#define SHOW_HTMLKEYWORD

#ifdef UC_JAPANINPUT
#define _JAPAN_KEYCODE 31
#endif

#ifdef UC_JAPANINPUT
#define DBG_JAPAN
#endif

#ifdef USER_DEKEMA
_BOOL DekemaMerker;
#endif

//#define USER_TAYU
#ifdef UC_EDITOR_PRESIGN
 #define SA28815
#endif

#ifdef USER_BILLION
 #ifdef SA28815
  #undef SA28815
 #endif
#endif 

#ifndef SA41205
 #define SA41205
#endif

#ifndef SA35625
 #define SA35625
#endif

void set_MULTIKEYBOARD(_UWORD anzahl, _UWORD *tab, _UWORD delaytime)
{
  // anzahl der tasten
  // scancode + 9 * zeichenfolge
  // scancode + 9 * zeichenfolge
  
  if((anzahl != 0)&&(tab != NULL))
  {
    MultiKeyboard.keytab = tab;
    MultiKeyboard.no = anzahl;
    MultiKeyboard.delaytime = delaytime;
  }
  else
  {
    free_MULTIKEYBOARD(&MultiKeyboard);
  }
}

#ifdef USER_TAYU
static const _UWORD tayu_keytab[] = {'1', 'S', 'T', 'U', '1', 0, 0, 0, 0, 0,
                                     '2', 'V', 'W', 'X', '2', 0, 0, 0, 0, 0,
                                     '3', 'Y', 'Z', '3',   0, 0, 0, 0, 0, 0,
                                     '4', 'J', 'K', 'L', '4', 0, 0, 0, 0, 0,
                                     '5', 'M', 'N', 'O', '5', 0, 0, 0, 0, 0,
                                     '6', 'P', 'Q', 'R', '6', 0, 0, 0, 0, 0,
                                     '7', 'A', 'B', 'C', '7', 0, 0, 0, 0, 0,
                                     '8', 'D', 'E', 'F', '8', 0, 0, 0, 0, 0,
                                     '9', 'G', 'H', 'I', '9', 0, 0, 0, 0, 0,
                                     ' ', ' ',  92,   0,   0, 0, 0, 0, 0, 0,
                                     '-', '+', '/', '-',   0, 0, 0, 0, 0, 0};

#endif

void MultiKeyboardStart()
{
 #ifdef USER_TAYU
  set_MULTIKEYBOARD(11, tayu_keytab, 700);
 #endif
  init_MULTIKEYBOARD(&MultiKeyboard, _FALSE);
}

_BOOL MultiKeyboardRun(_UWORD *sc, _UDWORD timestamp)
{
  _UWORD key = *sc;
  _BOOL retcode = _FALSE;
  _UWORD *tab = NULL;
  _UWORD anzahl, *ph;
  
  if(MultiKeyboard.keytab != NULL)
  {
    anzahl = MultiKeyboard.no;
    ph     = &MultiKeyboard.keytab[0];
    while(anzahl)
    {
      anzahl -= 1;
      if(*ph == key)
      {
        tab = ph + 1;
        anzahl = 0;
      }
      ph += 10;
    }
  
    if(tab != NULL)
    {
      if((key == MultiKeyboard.key) && ((timestamp - MultiKeyboard.timestamp) < MultiKeyboard.delaytime))
      {
        MultiKeyboard.count += 1;
        retcode = _TRUE; // letztes zeichen im eingabestring löschen
      }
      else
      {
        MultiKeyboard.count = 0;
      }
  
      if(MultiKeyboard.count > 8) 
        MultiKeyboard.count = 0; // zur sicherheit
      if(tab[MultiKeyboard.count] == 0)
        MultiKeyboard.count = 0; // wrap in keycodesequence
    
      key = tab[MultiKeyboard.count];
    }
  
    MultiKeyboard.timestamp = timestamp;
    MultiKeyboard.key = *sc;

    if(key != MultiKeyboard.key)
      *sc = key;
  }

  return retcode;
}

void add_numstring(_CHAR *txt, _WORD incdec)
{
  _UWORD i, no = StrLen(txt, sizeof(_CHAR));
  _CHAR  chr;
  _DWORD value = 0;
  _BOOL  neg = _FALSE;
  _UWORD dot = 0;

  for(i=0; i<no; i++)
  {
    chr = txt[i];
    if((chr >= 0x30)&&(chr <= 0x39))
    {
      value = (value*10) + (chr-0x30);
    }
    else if(chr == '-')
    {
      neg = (value == 0)? _TRUE : _FALSE;
    }
    else if((chr == '.')||(chr == ','))
    {
      dot = (((no - (i + 1)) << 4)&0x00F0) | 0x0800;
    }
  }

  if(neg == _TRUE)
    value = -value;

  value = value + incdec;

  DToA(txt, value, dot, sizeof(_CHAR));
}

_CHAR   Xfstring[EDITORCHRS];
_UWORD  Xpos;
_UWORD  Xno;

void init_cpyedit(void)
{
  Xfstring[0] = 0;
  Xpos = 0;
  Xno = 0;
}

void count_digits_sub(_CHAR *txt, _UWORD *pdig, _UWORD *pnk)
{
  _UWORD i, len, dig, nkdig;
  _BOOL  dot;
  _CHAR  chr;

  dot      = _FALSE;
  len      = StrLen(txt, sizeof(_CHAR));
  dig      = 0;
  nkdig    = 0;

  for(i=0; i<len; i++)
  {
    chr = *txt;
    switch(chr)
    {
      case 0x30 :
      case 0x31 :
      case 0x32 :
      case 0x33 :
      case 0x34 :
      case 0x35 :
      case 0x36 :
      case 0x37 :
      case 0x38 :
      case 0x39 : dig++;
                  if(dot == _TRUE)
                    nkdig ++;
                  break;
      case 0x2C :
      case 0x2E : dot = _TRUE;
                  break;
      case '+'  :
      case '*'  :
      case '/'  : // bisheriger string bereits auf richtigkeit überprüft, deshalb return !!
                  count_digits_sub((txt+1), pdig, pnk);
                  i = len;
                  break;
    }

    txt += 1;
  }

  if(*pdig < dig)
    *pdig = dig;

  if(*pnk < nkdig)
    *pnk = nkdig;
}

void count_digits(_CHAR *txt, _UWORD *pdig, _UWORD *pnk)
{
  *pdig = 0;
  *pnk  = 0;
  count_digits_sub(txt, pdig, pnk);
}

_BOOL chk_vorkommastellen(_CHAR *str, _EDITOR *pe)
{
  _BOOL  retcode = _FALSE;
  _UWORD dig, nk;
  _UWORD digits, posdezpt;

  count_digits(str, &dig, &nk);
  digits   = (pe->variable.info.digits == 0)? 10 : pe->variable.info.digits;
  posdezpt = pe->variable.info.posdezpt;

  if(VarList.GetFixDezpt(&pe->variable.info) == _TRUE)
  {
    digits = digits - posdezpt;
    dig    = dig - nk;
  }

  if(dig <= digits) // vorkommastellen überprüfen
    retcode = _TRUE;

  return(retcode);
}

_BOOL store_CHREDIT(_CHREDIT *pc)
{
  _BOOL retcode = _FALSE;

  if(pc->change == _TRUE)
  {
    MemCpy(Xfstring, pc->fstring, EDITORCHRS * sizeof(_CHAR));
    Xpos    = pc->pos;
    Xno     = pc->no;
    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL restore_CHREDIT(_EDITOR *pe)
{
  _BOOL    retcode = _FALSE;
  _CHREDIT *pc = &pe->chredit;

  if(Xfstring[0] != 0)
  {
    MemCpy(pc->fstring, Xfstring, EDITORCHRS * sizeof(_CHAR));
    pc->pos    = Xpos;
    pc->no     = Xno;
    pc->change = _TRUE;
    retcode    = chk_vorkommastellen(pc->fstring, pe);
  }

  return(retcode);
}

_BOOL scan_string(_CHAR *txt, _BOOL change, _EDITOR *pe)
{
  _BOOL  retcode = _TRUE;
  _CHAR  chr;
  _UWORD digits, posdezpt, len, i, dig, nkdig, vkdig;
  _BOOL  dot;

  if(change == _TRUE)
  {
    posdezpt = pe->variable.info.posdezpt;
    digits   = (pe->variable.info.digits == 0)? 10 : pe->variable.info.digits;
    dot      = _FALSE;
    len      = StrLen(txt, sizeof(_CHAR));
    dig      = 0;
    nkdig    = 0;
    vkdig    = 1000;

    for(i=0; i<len; i++)
    {
      chr = *txt;
      switch(chr)
      {
        case 0x30 :
        case 0x31 :
        case 0x32 :
        case 0x33 :
        case 0x34 :
        case 0x35 :
        case 0x36 :
        case 0x37 :
        case 0x38 :
        case 0x39 : dig++;
                    if(dot == _TRUE)
                      nkdig ++;
                    break;
        case 0x2C :
        case 0x2E : dot = _TRUE;
                    break;
        case '+'  :
        case '*'  :
        case '/'  : // bisheriger string bereits auf richtigkeit überprüft, deshalb return !!
                    return(scan_string(txt+1, change, pe));
      }

      txt += 1;
    }

    if(VarList.GetFixDezpt(&pe->variable.info) == _TRUE)//.format & CALCUL_DIGITS))
      vkdig = digits - posdezpt;

    if((dot == _FALSE)&&((dig-nkdig) >= vkdig))
      retcode = _FALSE;
    if((posdezpt != 0)&&(nkdig >= posdezpt))
      retcode = _FALSE;
    if(dig >= digits)
      retcode = _FALSE;
  }

  return(retcode);
}

_DWORD calcul_incdec_EDITOR(_EDITOR *p, _IVALUE value)
{
  _UWORD dezpt, dig;
  _DWORD retcode = 1;

  if((p->variable.info.vartype == BINARY)&&(p->variable.info.format == SIZE_REAL))
  {
    switch(VarList.GetDezPoint(&p->variable.info))
    {
      case 0 : move_pREAL_REAL((_REAL*)&retcode, 1); break;
      case 1 : move_pREAL_REAL((_REAL*)&retcode, 0.1); break;
      case 2 : move_pREAL_REAL((_REAL*)&retcode, 0.01); break;
      case 3 : move_pREAL_REAL((_REAL*)&retcode, 0.001); break;
      case 4 : move_pREAL_REAL((_REAL*)&retcode, 0.0001); break;
      case 5 : move_pREAL_REAL((_REAL*)&retcode, 0.00001); break;
    }
  }
  else if(VarList.GetFixDezpt(&p->variable.info) == _FALSE) //.format & CALCUL_DIGITS)
  {
    dezpt   = VarList.GetDezPoint(&p->variable.info);
    dig     = VarList.GetDigits(&p->variable.info);
    retcode = do_floating_dezpt(&value, dig, &dezpt);
  }

  return(retcode);
}

_BOOL _Picture::IsEditorOpen(void)
{
  return(IsEditorTrue(Pic.Editor.actno));
}

_BOOL _Picture::IsEditorTrue(_UWORD no)
{
  return(is_SEEKELEMENT(&Pic, no));
}

#ifdef USER_BILLION

extern "C" _DOIT BillionEditor_ShowEditor(void*, _EDITOR*, _BOOL, void*);
extern "C" _BOOL BillionEditor_LimitFail(void*, _EDITOR*, _RESULT*, _BOOL);
extern "C" unsigned long BillionEditor_LimitCheck(void*, _EDITOR*, _RESULT*);

void *get_billionpthis(void)
{
  if(BillionPThis == NULL)
    BillionPThis = get_this_pointer((_ASCII*)"BillionEditor");

  return(BillionPThis);
}

_BOOL call_billionlimit(_EDITOR *ped, _RESULT *pr, _BOOL minwert)
{
//  _FBOOL_PT_PT_BOOL ptr;
  void              *billionpthis;
  _BOOL             retcode = _TRUE;

  billionpthis = get_billionpthis();
  if(billionpthis != NULL)
  {
//    ptr = (_FBOOL_PT_PT_BOOL)find_lasal_method(billionpthis, 16);
//    if(ptr != NULL)
    {
//      load_this(billionpthis);
//      retcode = ptr(ped, pr, minwert);
      retcode = BillionEditor_LimitFail(billionpthis, ped, pr, minwert);

      if(retcode == _TRUE)
      {
        if(ped->variable.info.formula != DEFFORMULA)
        {
          retcode = recalculate(pr, &ped->variable, _TRUE);
        }
      }
      if(retcode == _TRUE)
      {
        ped->result = *pr;

        if(pr->ftype == RES_TIME)
        {
          format_time(ped->chredit.fstring, &pr->datim.ftime, ped->variable.info.format);
        }
        else if(pr->ftype == RES_DATE)
        {
          format_date(ped->chredit.fstring, &pr->datim.fdate, ped->variable.info.format);
        }
        else
        {
          format_value(ped->chredit.fstring, pr->value, &ped->variable.info, &ped->input.io.picformat);
          DelChr(ped->chredit.fstring, ' ', sizeof(_CHAR));
        }

        ped->chredit.change = _TRUE;
        ped->chredit.pos    = StrLen(ped->chredit.fstring, sizeof(_CHAR));
        ped->chredit.no     = ped->chredit.pos;
        retcode = _FALSE;
      }
    }
  }

  return(retcode);
}

_DOIT call_billioneditor(_EDITOR *ped, _BOOL state, _Picture *picpoi)
{
//  _FDOIT_PT_BOOL_PT ptr;
  _DOIT             retcode = _IDLE;
  void              *billionpthis;

  billionpthis = get_billionpthis();

  if(billionpthis != NULL)
  {
//    ptr = (_FDOIT_PT_BOOL_PT)find_lasal_method(billionpthis, 15);
//    if(ptr != NULL)
    {
//      load_this(billionpthis);
//      retcode = ptr(ped, state, picpoi);
      
      
      retcode = BillionEditor_ShowEditor(billionpthis, ped, state, picpoi);
      
    }
  }

  return(retcode);
}

_UDWORD call_billionchecklimit(_EDITOR *ped, _RESULT *pres)
{
//  _FUDWORD_PT_PT ptr;
  void           *billionpthis;
  _UDWORD        retcode = 0;

  billionpthis = get_billionpthis();

  if(billionpthis != NULL)
  {
//    ptr = (_FUDWORD_PT_PT)find_lasal_method(billionpthis, 17);
//    if(ptr != NULL)
    {
//      load_this(billionpthis);
//      retcode = ptr(ped, pres);
      
      
      retcode = BillionEditor_LimitCheck(billionpthis, ped, pres);
      
    }
  }

  return(retcode);
}

#endif

_BOOL _Picture::CloseEditor(void)
{
  _BOOL retcode = _FALSE;
  _ZOOM zoom;
  _DOT  offset;

 #ifdef USER_DEKEMA
  DekemaMerker = _FALSE;
 #endif

  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
   #ifdef USER_BILLION
    call_billioneditor(&Pic.Editor, _FALSE, this);
   #endif

   #ifdef OK_ENCLOSE_NAME
    if(Pic.Editor.input.io.virtname.pthis != NULL)
    {
      switch(call_enclosed_name_method(METHOD_MYIO_KILLFOCUS, &Pic.Editor.input.io, &Pic.Editor.variable, &Pic.Editor.result, NULL, NULL, _TRUE))
      {
        case _IFAILED : return(_FALSE);
        case _IDIDIT  : return(_TRUE);
        default       : break;
      }
    }
   #endif

    if(Pic.Editor.input.io.typ == AS_THEURGE)
    {
      switch(killfocus_THEURGE(&Pic.Editor))
      {
        case _IFAILED : return(_FALSE);
        case _IDIDIT  : return(_TRUE);
        default       : break;
      }
    }

   #ifdef OK_SUMMARY
    summary_SEEKELEMENT(&Pic, Pic.Editor.actno, _FALSE, NULL); //this); // keinen this übergeben dr1419
   #endif

    Cursor.End();
//o   TextCursor.End();
    if(Pic.Editor.input.io.bRefreshAct == _FALSE)
    {
      if(Pic.Editor.input.io.image.ptr != NULL)
        PutImage(&Pic.Editor.input.io.image, Pic.Editor.input.io.space.xy1.x, Pic.Editor.input.io.space.xy1.y);

      init_DOT(&offset);
      init_ZOOM(&zoom);

      offset = Pic.P_Window->GetPicOffset(); //Pic.Room.xy1;
      zoom = Pic.PicZoom;
      out_SEEKELEMENT(&Pic, Pic.Editor.actno, _EDITPASSIVE, &offset, &zoom);
    }
//    init_EDITOR(&Pic.Editor);
    free_EDITOR(&Pic.Editor);
  }

 #ifdef OK_PICMEMO
  _WHOAMI wai;

  if(Pic.P_Window != NULL)
  {
//    MemoPool.SetLastInput(&GetWhoAmI(), 0xFFFF);
    wai = GetWhoAmI();
    MemoPool.SetLastInput(&wai, 0xFFFF);
  }
 #endif

  return(retcode);
}

void draw_stream(_UWORD aktpos, _EDITOR *pe)
{
  _UWORD  xx, width;
  _CHAR   tmp[EDITORCHRS];
  _ATTRIB attrib = (pe->input.io.attrib & (~T_HORIZBOUND)) | T_LEFTBOUND;
  _ROOM   *space = &pe->input.io.space;

  width = space->xy2.x - space->xy1.x + 1;
  StrCpy(tmp, sizeof(_CHAR), pe->chredit.fstring, sizeof(_CHAR));
  tmp[aktpos] = 0;

  Font.Set(pe->input.io.font, NULL);
  
  
  xx = StrWidth(tmp, sizeof(_CHAR));
  if(xx > width)
  {
    attrib = (attrib & ~T_HORIZBOUND)|T_RIGHTBOUND;
    OutTextAlign(tmp, 0, space, pe->input.io.font, attrib, pe->input.io.frametype, pe->input.io.coltxt, pe->input.io.colback, pe->input.io.colframe, sizeof(_CHAR));
  }
  else
  {
    OutTextAlign(pe->chredit.fstring, 0, space, pe->input.io.font, attrib, pe->input.io.frametype, pe->input.io.coltxt, pe->input.io.colback, pe->input.io.colframe, sizeof(_CHAR));

//#ifdef UC_JAPANINPUT
//#ifdef DBG_JAPAN   
//    if(Japan_InputMode() != 0)
//    {
//      if(pe->chredit.japan_pos < lib_arraysize(tmp))
//      {
//        tmp[pe->chredit.japan_pos] = 0;
//        _KOORD x1 = StrWidth(tmp, sizeof(_CHAR));
//        if(x1 < xx)
//        {
//          _ROOM room;
//          GetTextRoom(&room, space, attrib, pe->input.io.frametype, _TRUE);
//      //  Line(x1, room.xy2.y-3,x1, room.xy2.y, T_COPY | T_SOLID, pe->input.io.coltxt);
//          Line(room.xy1.x+x1, room.xy2.y-2,room.xy1.x+xx, room.xy2.y-2, T_COPY | T_SOLID, pe->input.io.coltxt);
//        }
//      }
//    }
//#endif
//#endif
  }
}

_KOORD get_cursory(_EDITOR *ped)
{
  _KOORD retcode;
  _ROOM  room;

  GetTextRoom(&room, &ped->input.io.space, ped->input.io.attrib, ped->input.io.frametype, _FALSE);

  switch(ped->input.io.attrib & T_VERTBOUND)
  {
    case T_DOWNBOUND   : retcode = room.xy2.y + 1 - ped->chredit.ysize;                      break;
    case T_MIDBOUND    : retcode = ((room.xy1.y + room.xy2.y + 1 - ped->chredit.ysize) / 2); break;
    default            : retcode = room.xy1.y;                                               break;
  }

  return(retcode);
}

void EditorOutChanged(_Picture *PicPoi, _EDITOR *ped)
{
  _UWORD xt;

  Font.Size(ped->input.io.font, &xt, &ped->chredit.ysize);
  ped->chredit.cursory = get_cursory(ped);

  PicPoi->ShowChangedEditor(ped);
}

void _Picture::ShowChangedEditor(_EDITOR *ped)
{
  _ATTRIB attrib;
  _ROOM   room;

  switch(ped->input.io.typ)
  {
    case AS_IPADDRESS    :
    case AS_TIME         :
    case AS_DATE         :
    case AS_ASCII        :
    case AS_NUMERIC      :
    case AS_ASCII_HIDE   :
    case AS_NUMERIC_HIDE :
    case AS_THEURGE      :
      attrib = (ped->input.io.attrib & (~T_HORIZBOUND)) | T_LEFTBOUND;
      GetTextRoom(&room, &ped->input.io.space, ped->input.io.attrib, ped->input.io.frametype, _TRUE);
      room.xy1.y = ped->chredit.cursory;
      room.xy2.y = room.xy1.y + ped->chredit.ysize - 1;
      Cursor.Hide();
      if((ped->input.io.typ == AS_ASCII_HIDE) || (ped->input.io.typ == AS_NUMERIC_HIDE))
      {
        _CHAR stars[65]; // SA57210
        MakeStars(stars, lib_arraysize(stars), ped->chredit.fstring);
        OutTextAlign(stars, 0, &ped->input.io.space, ped->input.io.font, attrib, ped->input.io.frametype, ped->input.io.coltxt, ped->input.io.colback, ped->input.io.colframe, sizeof(_CHAR));
        Cursor.CheckRoom(&room, TxtWidthFont(stars, ped->chredit.pos, sizeof(_CHAR), ped->input.io.font));
      }
      else
      {
        // überprüfen ob editor gescrollt werden muss
        draw_stream(ped->chredit.pos, ped);
        Cursor.CheckRoom(&room, TxtWidthFont(ped->chredit.fstring, ped->chredit.pos, sizeof(_CHAR), ped->input.io.font));
      }
     #ifdef USER_BILLION // eingabefarbe invers
      if(color_billioneditor(&ped->input.io, _EDITCHANGE) == _TRUE)
      {
        Cursor.Set(&room, ped->input.io.coltxt, 0);
      }
      else
      {
        Cursor.Set(&room, SetUp.GetEditColorCursor(_EDITCHANGE), 0);
      }
     #else
      Cursor.SetC(&room, SetUp.GetEditColorCursor(_EDITCHANGE), 0, SetUp.GetEditColorBack(_EDITCHANGE, LIGHTRED));
     #endif
      break;
    case AS_CHKBOX  :
      out_chkbox(&ped->input.io.space, ped->result.value, ped->input.io.attrib, ped->input.io.coltxt, ped->input.io.colback, ped->input.io.colframe);
      break;
    case AS_RADIO   :
      out_radiobutton(&ped->input.io.space, ped->result.value, ped->input.io.attrib, ped->input.io.coltxt, ped->input.io.colback, ped->input.io.colframe);
      break;
    default         :
      attrib = (ped->input.io.attrib & (~T_HORIZBOUND)) | T_LEFTBOUND;
      OutTextAlign(ped->chredit.fstring, 0, &ped->input.io.space, ped->input.io.font, attrib, ped->input.io.frametype, ped->input.io.coltxt, ped->input.io.colback, ped->input.io.colframe, sizeof(_CHAR));
      break;
  }
}

#ifdef SA41205
static _BOOL CutToLimitRecalc(_REAL *preal, _RESULT *pres, _VARIABLE *pvar, _DWORD val)
{
  _BOOL retcode = _FALSE;
  _DWORD tval = pres->value;
  
  pres->value = val;
  if(recalculate(pres, pvar, _TRUE) == _TRUE) // in physik umrechnen
  {
    _CHAR txt[64];
    format_value(txt, pres->value, &pvar->info, NULL); // als string formattieren
    retcode = StrToReal(preal, txt, sizeof(_CHAR)); // in real wert wandeln
  }
  pres->value = tval;
  
  return retcode;
}

_UDWORD CutToLimit(_VARLABEL *pvl, _CHAR *instr, _UDWORD grenzwert)
{
  // --> 0 wenn nichts zu tun ist
  // --> 1 wenn eingegebener Wert auf die Untergrenze korrigiert werden soll
  // --> 2 wenn eingegebener Wert auf die Obergrenze korrigiert werden soll
  
  _VARIABLE variable;
  _RESULT res;
  _REAL rval, rmin, rmax;
  _UDWORD retcode = 0;

  if(StrToReal(&rval, instr, sizeof(_CHAR)) == _TRUE)
  {
    init_RESULT(&res);
    if(VarList.GetSystemData(&res, &variable, pvl, _TRUE, _TRUE, _FALSE) == _TRUE) // wert samt grenzen ermitteln
    {
      if(grenzwert == 2)
      {
        if(CutToLimitRecalc(&rmax, &res, &variable, res.rmax) == true)
        {
          if(rval <= rmax)
          {
            retcode = 2;
          }
        }
      }
      else
      {
        if(CutToLimitRecalc(&rmin, &res, &variable, res.rmin) == true)
        {
          if(rval >= rmin)
          {
            retcode = 1;
          }
        }
      }
      
//      if(CutToLimitRecalc(&rmin, &res, &variable, res.rmin) == true)
//      {
//        if(CutToLimitRecalc(&rmax, &res, &variable, res.rmax) == true)
//        {
//          if((rval >= rmin) && (rval <= rmax))
//          {
//            _REAL abs_min = rval - rmin;
//            _REAL abs_max = rmax - rval;
//            retcode = (abs_min < abs_max)? 1 : 2;
//          }
//        }
//      }
    }
  }
  
  return retcode;
}

_UDWORD CutToLimit(_EDITOR *pe, _UDWORD grenzwert)
{
  return CutToLimit(&pe->input.io.variable, pe->chredit.fstring, grenzwert);
}

#endif

_BOOL _Picture::EditorCheckLimits(_EDITOR *pe)
{
  _RESULT result;
  
  if(pe->input.io.typ == AS_NUMERIC)
  {
    init_RESULT(&result);
    EditorToResult(&result, pe);
    
    result.ftype = pe->result.ftype;
    recalculate(&result, &pe->variable, _FALSE);

   #ifdef UC_REAL
    if(result.ftype == RES_REAL)
    {
      _REAL val = get_pREAL((_REAL*)&result.value);
      _REAL min = get_pREAL((_REAL*)&pe->result.rmin);
      _REAL max = get_pREAL((_REAL*)&pe->result.rmax);
      
     #ifdef SA41205
      if(val < min)
      {
        if(CutToLimit(&Pic.Editor, 1) != 1)
        {
          return _FALSE;
        }
      }
      
      if(val > max)
      {
        if(CutToLimit(&Pic.Editor, 2) != 2)
        {
          return _FALSE;
        }
      }
     #else 
      if((val < min) || (val > max))
      {
        return _FALSE;
      }
     #endif 
    }
   #endif 
    else
    {
     #ifdef SA41205
      if(result.value < pe->result.rmin)
      {
        if(CutToLimit(&Pic.Editor, 1) != 1)
        {
          return _FALSE;
        }
      }
      
      if(result.value > pe->result.rmax)
      {
        if(CutToLimit(&Pic.Editor, 2) != 2)
        {
          return _FALSE;
        }
      }
     #else    
      if((result.value < pe->result.rmin) || (result.value > pe->result.rmax))
      {
        return _FALSE;
      }
     #endif 
    }
  }

  return _TRUE;
}

_BOOL _Picture::EditorCheckLimits(void)
{
  return EditorCheckLimits(&Pic.Editor);
}

_COLOR _Picture::EditorGetColorText(_EDITOR *pe, _COLOR preselect)
{
#ifdef SA32402
 #ifdef UC_DYN_EDIT_COLOR
  #if(UC_DYN_EDIT_COLOR != INVISIBLE)
   if(pe->input.io.typ == AS_NUMERIC)
   {
     if(EditorCheckLimits(pe) == _FALSE)
     {
//todo:     _UDWORD CutToLimit(_VARLABEL *pvl, _CHAR *instr)
     
         return UC_DYN_EDIT_COLOR;
     }
   }
  #endif 
 #endif 
#endif
  
  return preselect;
}

_BOOL _Picture::ShowEditor(void)
{
  _BOOL   retcode = _FALSE;
  _DOT    offset;
  _ZOOM   zoom;
  _REGION actreg, ioreg=NULL, newreg=NULL;
//  _ROOM   room;
//  _CHAR   stars[50];
//  _UWORD  i;


  // THR V1.40: Note that this MUST be called as it will
  // call our Draw method to prepare the initial user editor!
 #ifndef THR
 #ifndef USER_BILLION
  if(Pic.Editor.input.io.bRefreshAct == _FALSE)
 #endif
 #endif
  {

  init_ZOOM(&zoom);
  init_DOT(&offset);

  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
    if(Pic.Editor.chredit.change == _FALSE)
    {
      offset = Pic.P_Window->GetPicOffset(); //Pic.Room.xy1;
      zoom = Pic.PicZoom;
      Cursor.Hide();
      retcode = out_SEEKELEMENT(&Pic, Pic.Editor.actno, _EDITACTIVE, &offset, &zoom);

     #ifdef USER_BILLION
      if(retcode == _TRUE)
        call_billioneditor(&Pic.Editor, _TRUE, this);
     #endif

      if(DrawBackGround == 0)
        set_MENCURSOR(&Pic.Editor.mencursor);
    }
    else
    {
     #ifdef USER_BILLION // eingabefarbe invers
      if(color_billioneditor(&Pic.Editor.input.io, _EDITCHANGE) == _FALSE)
      {
        Pic.Editor.input.io.colback = SetUp.GetEditColorBack(_EDITCHANGE, Pic.Editor.input.io.colback);
        Pic.Editor.input.io.coltxt  = SetUp.GetEditColorTxt(_EDITCHANGE, Pic.Editor.input.io.coltxt);
      }
     #else
      Pic.Editor.input.io.colback = SetUp.GetEditColorBack(_EDITCHANGE, Pic.Editor.input.io.colback);
      Pic.Editor.input.io.coltxt  = EditorGetColorText(&Pic.Editor, SetUp.GetEditColorTxt(_EDITCHANGE, Pic.Editor.input.io.coltxt));
     #endif
      Pic.Editor.input.io.colframe = SetUp.GetEditColorFrame(_EDITCHANGE, Pic.Editor.input.io.colframe);
      Pic.Editor.input.io.editstate = _EDITCHANGE;

//      Pic.Editor.input.io.coltxt  = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(UC_EDIT_COL_ACTIVE));
//      Pic.Editor.input.io.colback = MakeColor_Makro(GetBackColor_Makro(UC_EDIT_COL_ACTIVE), GetBackColor_Makro(UC_EDIT_COL_ACTIVE));

     #ifdef UC_MOUSE
      _UWORD hide = IfNecHideMouseCursor(&Pic.Editor.input.io.space);
     #endif
//      actreg = ActivateAndRegion(&ioreg, &newreg, &Pic.Editor.input.io.space, Pic.Editor.input.io.attrib);
      actreg = ActivateAndRegion(&ioreg, &newreg, &Pic.Editor.input.io.space);

     #ifdef USER_BILLION
      retcode = _TRUE;
      if(call_billioneditor(&Pic.Editor, _TRUE, this) == _IDLE)
     #endif
      {
       #ifdef OK_ENCLOSE_NAME
        if(Pic.Editor.input.io.virtname.pthis != NULL)
        {
          _RESULT tmp_result;
          init_RESULT_Makro(&tmp_result);
          EditorToResult(&tmp_result, &Pic.Editor);

          switch(call_enclosed_name_method(METHOD_MYIO_DRAW, &Pic.Editor.input.io, &Pic.Editor.variable, &tmp_result, NULL, NULL, _TRUE))
          {
            case _IDIDIT  : retcode = _TRUE;  break;
            case _IFAILED : retcode = _FALSE; break;
            default       : goto NOTSOFINE;
          }
          if(retcode == _TRUE)   // in a MyIo text width resp. cursorpos is unknown
            Cursor.End();
        }
        else
       #endif
        {
          NOTSOFINE :
          ShowChangedEditor(&Pic.Editor);
          retcode = _TRUE;
        }
      }

      DeactivateAndRegion(actreg, &ioreg, &newreg);

     #ifdef UC_ROTATE
      RefreshRectangle(&Pic.Editor.input.io.space);
     #endif

     #ifdef UC_MOUSE
      IfNecShowMouseCursor(hide);
     #endif

    }
  }
  }
  return(retcode);
}

_BOOL _Picture::ReOpenEditor(void)
{
  _BOOL       retcode = _FALSE;
  _STACK_LINE sl;

  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
    if((Pic.Editor.chredit.change == _FALSE) && (Pic.P_Window != NULL))
    {
//      Pic.P_Window->AddCmd(CMD_KILLEDITOR, 0, 0);
//      Pic.P_Window->AddCmd(CMD_EDITNO, Pic.Editor.actno, 0);

      init_STACK_LINE_Makro(&sl);
      sl.op1.no = 1;
      sl.op1.info[0].value = Pic.Editor.actno;
      sl.op1.info[0].state = CONST_VAL;

      if(Pic.P_Window->CMD_CloseBubbleHelp() == _TRUE)
        Pic.P_Window->FindRoot()->MakeRegion();                 // do complete new regions
      Pic.P_Window->CMD_CloseEditor();
      Pic.P_Window->CMD_OpenEditor(&sl, _FALSE, _FALSE, NULL);

      retcode = _TRUE;
    }
  }

  return(retcode);
}

_BOOL _Picture::UpdateEditor(void)
{
  _BOOL  retcode = _FALSE;
  _UWORD no;

  if(IsEditorOpen() == _TRUE)
  {
    no = Pic.Editor.actno;
    Pic.Editor.actno++;
    retcode = OpenEditor(no);
  }

  return(retcode);
}

_BOOL _Picture::OpenEditor(_UWORD no, _EVENT *pev)
{
  _BOOL   retcode = _FALSE;
  _EDITOR tmp;
  _DOIT   doit = _IDLE;
  _DOT    offset;
  _ZOOM   zoom;
  _UWORD  x;
  _ROOM   room;

  MultiKeyboardStart();

 #ifdef USER_DEKEMA
  DekemaMerker = _FALSE;
 #endif
 #ifdef USER_DEMAG
  pev = NULL; // kundenwunsch: e-mail vom 10.06.2014 und email samt wiederholter bestätigung am 11.06.2014
 #endif

  init_EDITOR(&tmp);
  init_ZOOM(&zoom);
  VarList.VarDummySet(VARNO_INSERTFLAG, 0); // InsertFlag zurücksetzen
  VarList.VarDummySet(VARNO_SHIFTFLAG, 0);  // ShiftFlag zurücksetzen
  VarList.VarDummySet(VARNO_ALTGRFLAG, 0);  // AltGrFlag zurücksetzen

 #ifdef UC_JAPANINPUT
//  if(Language.GetActualLocal() != LOCAL_JAPANESE)
//  {
    VarList.VarDummySet(VARNO_JAPANESE_INPUT, 0); // JapaneseInputMode zurücksetzen
    VarList.VarDummySet(VARNO_ALTGRFLAG_LOCK, 0); // AltGrFlag_Lock zurücksetzen
    VarList.VarDummySet(VARNO_HIRAGANA_KATAGANA, 0);
//  }
 #endif
 
  offset = Pic.P_Window->GetPicOffset();

  if(load_EDITOR(&tmp, &Pic, no, &offset, &zoom) == _TRUE)
  {
//    Zoom_ROOM(&tmp.input.io.space, &offset, &Pic.PicZoom);
//    is_same_summary(&Pic, no, Pic.Editor.actno, tmp.input.io.summary, Pic.Editor.input.io.summary);

    CloseEditor();

   #ifdef UC_JAPANINPUT
    tmp.japan_input_mode = VarList.VarDummyGet(VARNO_JAPANESE_INPUT);
   #endif

   #ifdef OK_ENCLOSE_NAME
    if(tmp.input.io.virtname.pthis != NULL)
    {
      doit = call_enclosed_name_method(METHOD_MYIO_GETFOCUS, &tmp.input.io, &tmp.variable, &tmp.result, NULL, NULL, _TRUE);
      if(doit == _IFAILED)
        return(_FALSE);
        
      if(pev != NULL)
      {
        // wenn ein event angegeben dann _MyIo::GetEvent() aufrufen
        call_enclosed_name_method(METHOD_MYIO_GETEVENT, &tmp.input.io, &tmp.variable, &tmp.result, &tmp, pev, _TRUE);
      }
    }
   #endif

    if(tmp.input.io.typ == AS_THEURGE)
    {
      doit = getfocus_THEURGE(&tmp, pev);
      if(doit == _IFAILED)
        return(_FALSE);
    }

   #ifndef CUTOFF_USER
    _FVOID_PT ptr;

    ptr = (_FVOID_PT)find_lasal_method(LseThisPointer, METHOD_LSE_OPENEDITOR);
    if(ptr != NULL)
    {
      load_this(LseThisPointer);
      ptr((void*)&tmp);
    }
   #endif

    recalculate(&tmp.result, &tmp.variable, _TRUE);
    tmp.incdec      = calcul_incdec_EDITOR(&tmp, tmp.result.value);
    tmp.chredit.max = VarList.GetCompleteNoDigits(&tmp.variable.info);
    
    if(tmp.chredit.max >= EDITORCHRS)
    {
      tmp.chredit.max = EDITORCHRS - 1;
    }
    
    editor_to_text(&tmp);

//    CloseEditor();  // wurde nach oben verschoben weil sonst GetFocus und KillFocus von _MyIo in falscher reihenfolge

   #ifdef USER_BILLION // eingabefarbe invers
    if(color_billioneditor(&tmp.input.io, _EDITACTIVE) == _TRUE)
    {
      _UDWORD x_back, x_txt;
      _COLOR  ctmp;

      if(tmp.input.io.is_dyncolor == _TRUE)
      {
        if(ColorScheme.Call(&x_back, &tmp.input.io.dyncolfront, GetPenColor_Makro(tmp.input.io.colback)) == _TRUE)
        {
          if(ColorScheme.Call(&x_txt, &tmp.input.io.dyncoltxt, GetPenColor_Makro(tmp.input.io.coltxt)) == _TRUE)
          {
            tmp.input.io.colback  = MakeColor_Makro((_UWORD)x_back, (_UWORD)x_back);
            tmp.input.io.coltxt   = MakeColor_Makro((_UWORD)x_txt, (_UWORD)x_txt);
          }
        }
      }

      ctmp = tmp.input.io.colback;
      tmp.input.io.colback = tmp.input.io.coltxt;
      tmp.input.io.coltxt  = ctmp;
    }
   #endif

    Pic.Editor          = tmp;
    Pic.Editor.input.io.editstate = _EDITACTIVE;
    Pic.Editor.actno    = no;
    Pic.Editor.actlayer = layer_SEEKELEMENT(&Pic, no);
    
    if(Pic.Editor.input.selector != _SELECTOR_ONLY_HID)
    {
      // aktuelle eingabe nur bei inputs, welche auch über keyboard anwählbar sind setzen
      // wird für input samt tabulatoranwahl (wechsel zwischen layer0 und layer1) benötigt
      Pic.Ptr[Pic.Editor.actlayer].SetLastEditor(no);
    } 
    Pic.Editor.timer = get_abstime();
    Pic.Editor.input.io.is_dyncolor = _FALSE;
    // THR V1.40: If GetFocus returns _IDIDIT then set
    // the return code to _FALSE so that the default editor is not
    // opened
    retcode = (doit == _IDIDIT)? _FALSE : _TRUE;

    // THR V1.40: Note that this MUST be called as it will
    // call our Draw method to prepare the initial user editor!
//   #ifndef THR
//    if(Pic.Editor.input.io.bRefreshAct == _FALSE)
//   #endif
    ShowEditor();

   #ifdef OK_PICMEMO
    _WHOAMI whoami = GetWhoAmI();
    if(Pic.P_Window != NULL)
      MemoPool.SetLastInput(&whoami, no);
   #endif
   #ifdef OK_SUMMARY
    summary_SEEKELEMENT(&Pic, no, _TRUE, this);
   #endif

//    if((CheckCursorVisible(Pic.Editor.input.io.typ) == _TRUE)&&(tmp.input.io.virtname.pthis == NULL))
    {
      GetTextRoom(&room, &Pic.Editor.input.io.space, Pic.Editor.input.io.attrib, Pic.Editor.input.io.frametype, _FALSE);
      Font.Size(Pic.Editor.input.io.font, &x, &Pic.Editor.chredit.ysize);


//Debug(Pic.Editor.chredit.ysize, 1);
//Debug(room.xy2.y - room.xy1.y + 1, 2);
//Debug(Pic.Editor.input.io.space.xy1.y, 3);


      if(Pic.Editor.chredit.ysize > (room.xy2.y - room.xy1.y + 1))
        Pic.Editor.chredit.ysize = room.xy2.y - room.xy1.y;
//      Pic.Editor.chredit.cursory = ((room.xy1.y + room.xy2.y) / 2) - (Pic.Editor.chredit.ysize / 2);
      Pic.Editor.chredit.cursory = get_cursory(&Pic.Editor);
      set_MENCURSOR(&Pic.Editor.mencursor);
//o      colcur = TextCursor.CreateColor(&Pic.Editor.input.io.space, GetPenColor_Makro(SetUp.GetEditColorBack(_EDITACTIVE, Pic.Editor.input.io.colback)), Pic.Editor.input.io.attrib, Pic.Editor.input.io.frametype, _EDITACTIVE);
//o      TextCursor.Start(&room, colcur);
    }
  }

  return(retcode);
}

/*
_BOOL _Picture::CheckCursorVisible(_IO_TYPE type)
{
  _BOOL retcode = _FALSE;

  switch(type)
  {
    case AS_NUMERIC_HIDE :
    case AS_ASCII_HIDE   :
    case AS_NUMERIC      :
    case AS_ASCII        :
    case AS_DATE         :
    case AS_TIME         :
    case AS_CHKBOX       :
    case AS_RADIO        :
    case AS_MENU         : retcode = _TRUE; break;
  }

  return(retcode);
}
*/
_BOOL _Picture::EditorToResult(_RESULT *pr, _EDITOR *pe)
{
  _UWORD nk;
  _BOOL  retcode = _TRUE;

  switch(pe->input.io.typ)
  {
    case AS_THEURGE:
    case AS_NUMERIC_HIDE :
    case AS_NUMERIC : nk = VarList.GetDezPoint(&pe->variable.info);
                      pr->value = string_to_value(string_to_numeric(pe->chredit.fstring, nk), &pe->variable.info);
                      break;
    case AS_IPADDRESS:pr->value = string_to_ipaddress(pe->chredit.fstring);
                      break;
    case AS_DATE    : retcode = string_to_date(&pr->datim.fdate, pe->chredit.fstring, pe->variable.info.format);
                      break;
    case AS_TIME    : retcode = string_to_time(&pr->datim.ftime, pe->chredit.fstring, pe->variable.info.format);
                      break;
    case AS_ASCII_HIDE :
    case AS_ASCII      :
                      break;
    case AS_ENUMERATION :
    case AS_MENU    :
    case AS_RADIO   :
    case AS_CHKBOX  :
    case AS_VARNAME :
    default         : retcode = _FALSE;
                      break;
  }

  return(retcode);
}

_BOOL _Picture::DirectionOnEnter(_ENTER_PLUS ep)
{
  _BOOL retcode = _FALSE;

 #ifdef OK_SEEK_NEXT
  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
    if((Pic.Editor.enterdirection == _TRUE)||(ep != _EP_NONE))
    {
      {
        _EVENT      event;
        _BOOL       st = _TRUE;

        init_EVENT_Makro(&event);
        event.ftype = _EVENT_KEYPRESS;

        if(ep == _EP_NONE)
        {
          ep = Pic.Editor.input.enter_plus;
        }
        else if(ep != _EP_REMEMBER)
        {
          if((Pic.RememberEnterPlus != 0xFFFF)&&(Pic.Editor.actno == Pic.RememberEnterPlus))
          {
            Pic.RememberEnterPlus = 0xFFFF;
          }
          else
          {
            Pic.RememberEnterPlus = 0xFFFF;
            return(_FALSE);
          }
        }

        if(ep == _EP_AUTO)
          ep = SetUp.GetDirectionOnEnter();

        switch(ep)
        {
          case _EP_REMEMBER : Pic.RememberEnterPlus = Pic.Editor.actno;
                              retcode = _TRUE;
                              st = _FALSE;
                              break;
          case _EP_LEFT     : event.scancode = _LEFT;  break;
          case _EP_RIGHT    : event.scancode = _RIGHT; break;
          case _EP_UP       : event.scancode = _UP;    break;
          case _EP_DOWN     : event.scancode = _DOWN;  break;
          default           : st = _FALSE;             break;
        }

        if(st == _TRUE)
          retcode = Seek(&event);
      }
    }
  }
 #endif

  return(retcode);
}
/*
_BOOL _Picture::DirectionOnEnter(_ENTER_PLUS ep)
{
  _BOOL retcode = _FALSE;

 #ifdef OK_SEEK_NEXT
  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
    if((Pic.Editor.enterdirection == _TRUE)||(ep != _EP_NONE))
    {
      if(ep == _EP_REMEMBER)
      {
        Pic.RememberEnterPlus = Pic.Editor.actno;
        return(_TRUE);
      }
      else
      {
        _EVENT      event;
        _BOOL       st = _TRUE;

        init_EVENT(&event);
        event.ftype = _EVENT_KEYPRESS;

        if(ep == _EP_NONE)
        {
          ep = Pic.Editor.input.enter_plus;
        }
        else
        {
          if((Pic.RememberEnterPlus == 0xFFFF)||(Pic.Editor.actno == Pic.RememberEnterPlus))
          {
            Pic.RememberEnterPlus = 0xFFFF;
          }
          else
          {
            Pic.RememberEnterPlus = 0xFFFF;
            return(_FALSE);
          }
        }

        if(ep == _EP_AUTO)
          ep = SetUp.GetDirectionOnEnter();

        switch(ep)
        {
          case _EP_LEFT  : event.scancode = _LEFT;  break;
          case _EP_RIGHT : event.scancode = _RIGHT; break;
          case _EP_UP    : event.scancode = _UP;    break;
          case _EP_DOWN  : event.scancode = _DOWN;  break;
          default        : st = _FALSE;             break;
        }

        if(st == _TRUE)
          retcode = Seek(&event);
      }
    }
  }
 #endif

  return(retcode);
}
*/

void  _Picture::SetLrm(_BOOL lrm)
{
  set_lrm(&Pic.Editor.result, lrm);
}

_BOOL _Picture::SaveEditor(_BOOL state)
{
  _BOOL retcode = _TRUE;

  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
   #ifndef USER_TIG //sa9273
    if(Pic.Editor.chredit.change == _TRUE)
   #endif
    {
      if(state == _TRUE)
      {
        retcode = EditorToResult(&Pic.Editor.result, &Pic.Editor);
        
       #ifdef SA35625
        if(retcode == _FALSE)
        {
          switch(Pic.Editor.input.io.typ)
          {
            case AS_DATE : 
              SystemError(STX_WRONG_DATE, 0, Pic.Editor.chredit.fstring);
              break;
            case AS_TIME : 
              SystemError(STX_WRONG_TIME, 0, Pic.Editor.chredit.fstring);        
              break;
          }
        }
       #endif 
      }
      
     #ifdef OK_ENCLOSE_NAME
      //retcode = _TRUE;
      if(Pic.Editor.input.io.virtname.pthis != NULL)
      {
        switch(call_enclosed_name_method(METHOD_MYIO_SAVEEDITOR, &Pic.Editor.input.io, &Pic.Editor.variable, &Pic.Editor.result, &Pic.Editor, NULL, _TRUE))
        {
          case _IDLE : //retcode = _TRUE;
                       break;
          default    : retcode = _FALSE; 
                       break;
        }
      }
     #endif
      
      if(retcode == _TRUE)
      {
        switch(Pic.Editor.input.io.typ)
        {
          case AS_THEURGE:
          case AS_NUMERIC_HIDE :
          case AS_NUMERIC : retcode = SaveEditNumeric();
                            break;
          case AS_ASCII_HIDE :
          case AS_ASCII   : retcode = SaveEditAscii();
                            break;
          case AS_RADIO   :
          case AS_CHKBOX  : retcode = SaveEdit();
                            break;
         #ifdef USER_BILLION
          case AS_DATE    :
          case AS_TIME    : retcode = SaveEditDaTim();
                            break;
         #else
          case AS_DATE    :
          case AS_TIME    : retcode = SaveEdit();
                            break;
         #endif
          case AS_IPADDRESS: retcode = SaveEdit();
                            break;
          default         : retcode = _FALSE;
                            break;

//        case AS_BAR,         // balken
//        case AS_TBO,         // text, bitmap or object
//        case AS_MENUE,
//        case AS_USER
        }
      }
      
      if(retcode == _TRUE)
      {
        Cursor.End();
        editor_to_text(&Pic.Editor);
        Pic.Editor.input.io.editstate = _EDITACTIVE;
      }

      Pic.Editor.incdec = calcul_incdec_EDITOR(&Pic.Editor, Pic.Editor.result.value);

      if(retcode == _TRUE)
        input_is_saved(&Pic.Editor.input, &Pic.Editor.result);
     #ifdef USER_BILLION
      else
      {
        if((Pic.Editor.input.io.typ != AS_THEURGE)&&(Pic.Editor.input.io.typ != AS_NUMERIC_HIDE)&&(Pic.Editor.input.io.typ != AS_NUMERIC)&&(Pic.Editor.input.io.typ != AS_DATE)&&(Pic.Editor.input.io.typ != AS_TIME))
          Pic.Editor.chredit.change = _FALSE;  // soog, do hods wos
        else
          ShowEditor();
      }
//     #else
//      else
//        Pic.Editor.chredit.change = _FALSE;  // soog, do hods wos
     #endif
    }
  }

  return(retcode);
}

_WORD astrict(_WORD value, _WORD add, _WORD ug, _WORD og, _BOOL ueberlauf)
{
  value = value + add;
  if(value < ug)
  {
    value = ug;
    if(ueberlauf == _TRUE)
      value = og;
  }
  else if(value > og)
  {
    value = og;
    if(ueberlauf == _TRUE)
      value = ug;
  }

  return(value);
}

#ifdef UC_JAPANINPUT

//static _UDWORD GetJapanPos(_CHREDIT *p)
//{
//  _UDWORD len = p->no; //StrLen(p->fstring, sizeof(_CHAR));
//  if(p->japan_pos > len)
//  {
//    p->japan_pos = len;
//  }
//  
//  while(p->fstring[p->japan_pos] == ' ') // alle space überspringen
//  {
//    p->japan_pos++;
//  }
//  
//  return p->japan_pos;
//}

//_BOOL _Picture::JapanDictionaryGetFirstMatch(void)
//{
//  _UWORD idx = GetJapanPos(&Pic.Editor.chredit);
//  _CHAR *pkeyword = &Pic.Editor.chredit.fstring[idx];
//  if(pkeyword != NULL)
//  {
////    while(*pkeyword == ' ') // alle preceeding space überspringen
////    {
////      pkeyword++;
////    }
//  
//    _UNI *mtxt[128];
//    _UDWORD txtno = JapanDictionaryGetText(mtxt, lib_arraysize(mtxt), Japan_GetDictionary(), pkeyword);
//
//    if(txtno > 0)
//    {
//      Pic.Editor.japan_pos_dictionary = idx;
//      if(StrLen(pkeyword, sizeof(_CHAR)) < lib_arraysize(Pic.Editor.japan_keyword))
//      {
//        StrCpy(Pic.Editor.japan_keyword, sizeof(_CHAR), pkeyword, sizeof(_CHAR));
//        return InjectEditorText(mtxt[0], GetJapanPos(&Pic.Editor.chredit));
//      }
//    }
//  }
//  
//  return _FALSE;
//}

//_BOOL _Picture::JapanDictionaryOpenDropDownList(_CHAR *keyword)
//{
//  _DOT      offset;
//  _ZOOM     zoom;
//  _UNI *mtxt[128];
//
//  Cursor.End();
//
////  _CHAR  *pkeyword  = &Pic.Editor.chredit.fstring[GetJapanPos(&Pic.Editor.chredit)];
//  
//  _UDWORD txtno = JapanDictionaryGetText(mtxt, lib_arraysize(mtxt), Japan_GetDictionary(), keyword);
//  _UDWORD menu_no = MenuList.MakeDictionaryMenu(mtxt, txtno);
//
//  if(menu_no != DEFSCOPE)
//  {
//    Font.Set(Pic.Editor.input.io.font, NULL);
//    _UDWORD xx = lib_get_max_text_width(mtxt, txtno) + 24; // Textbreite des längsten Text in Pixel ermitteln
//  
//    
//    _UWORD xoff = 0; //x- Pixeloffset von DropDownmenue ermitteln
//    if(Pic.Editor.japan_pos_dictionary > 0)
//    {
//      _CHAR   tmp[64];
//      _UDWORD len  = StrLen(Pic.Editor.chredit.fstring, sizeof(_CHAR));
//      if((Pic.Editor.japan_pos_dictionary < len) && (len < lib_arraysize(tmp)))
//      {
//        MemCpy(tmp, Pic.Editor.chredit.fstring, Pic.Editor.japan_pos_dictionary*sizeof(_CHAR));
//        tmp[Pic.Editor.japan_pos_dictionary] = 0;
//        xoff = StrWidth((void*)tmp, sizeof(_CHAR));
//      }
//    }
//    
//    MenuList.DesignDictionaryMenu((_UWORD)menu_no, &Pic.Editor.input.io, xx); //&Pic.Editor.input.comboinfo);
//
//    _IO *pio = &Pic.Editor.input.io;
//    Root.FindTop()->OpenComboBox((_UWORD)menu_no, &pio->variable, &pio->imoverloaded, pio->space.xy1.x + xoff, pio->space.xy2.y, _TRUE, _FALSE, pio->virtname.pthis);
//    
//    init_DOT(&offset);
//    init_ZOOM(&zoom);
//   #ifdef UC_TOUCH
//    if(SetUp.GetUseTouchEditor() == _TRUE)
//    {
//      out_SEEKELEMENT(&Pic, Pic.Editor.actno, _EDITCHANGE, &offset, &zoom);
//    }
//   #endif
//    return _TRUE;
//  }
//  return _FALSE;
//}

//_BOOL _Picture::InjectEditorText(_CHAR *txt, _UDWORD pos)
//{
//  _EDITOR *pe = &Pic.Editor;
//  _CHREDIT *p = &pe->chredit;
//
//  StrCpy(&p->fstring[pos], sizeof(_CHAR), txt, sizeof(_CHAR));       
//  p->japan_change = 1;
//  p->japan_pos    = StrLen(p->fstring, sizeof(_CHAR));
//  p->pos          = p->japan_pos;
//  p->no           = p->japan_pos;
//  
//  return _TRUE;
//}
//
//_BOOL _Window::InjectEditorText(_CHAR *txt)
//{
//  if(P_Picture != NULL)
//  {
//    _EDITOR *pe = P_Picture->GetEditor();
//    return P_Picture->InjectEditorText(txt, pe->japan_pos_dictionary);
//  }
//
//  return _FALSE;
//}

#endif

_BOOL run_CHREDIT(_CHREDIT *pe, _UWORD sc, _BOOL insert, _BOOL isnumeric, _BOOL is_character)
{
  _DWORD size;
  _BOOL  retcode = _FALSE;


  if(pe->max > 0)
  {
   #ifdef UC_JAPANINPUT      
    if(pe->japan_begin_pos > pe->pos)
    {
      pe->japan_begin_pos = pe->pos;
    }
   #endif
   
    switch(sc)
    {
      case _ESC    :
        if(pe->change == _TRUE)
        {
          pe->incdec     = _TRUE;
          pe->change     = _FALSE;
          pe->pos        = 0;
          pe->no         = 0;
          pe->fstring[0] = 0;
          retcode        = _TRUE;
        }
        break;
      case _BCKSPC :
        if(pe->no > 0)
        {
          if(pe->pos > 0)
          {
            size = (pe->no - pe->pos + 1) * sizeof(_CHAR);
            MemMove(&pe->fstring[pe->pos-1], &pe->fstring[pe->pos], size);
            pe->pos --;
            pe->no --;
          }
          else
          {
            size = StrLen(pe->fstring, sizeof(_CHAR)) * sizeof(_CHAR);
            MemMove(&pe->fstring[0], &pe->fstring[1], size);
            pe->no --;
          }
          pe->fstring[pe->no] = 0;
//         #ifdef UC_JAPANINPUT
//          GetJapanPos(pe);
//         #endif 
          retcode = _TRUE;
        }
       #ifdef USER_DEKEMA
        else if(pe->change == _FALSE)
        {
          pe->fstring[0] = 0;
          pe->change = _TRUE;
          pe->pos = 0;
          retcode = _TRUE;
        }
       #endif
        break;
      case _POS1 :
        pe->pos = 0;
        retcode = _TRUE;
        break;
      case _END :
        pe->pos = pe->no;
        retcode = _TRUE;
        break;
      case _LEFT :
        if(pe->no > 0)
        {
          if(pe->pos > 0)
          {
            pe->pos --;
            retcode = _TRUE;
          }
        }
        break;
      case _RIGHT :
        if(pe->pos < pe->no)
        {
          pe->pos ++;
          retcode = _TRUE;
        }
        break;
      case _INS:
        if(pe->pos < pe->no)
        {
          if(pe->no < pe->max)
          {
            size = (pe->no - pe->pos + 1) * sizeof(_CHAR);
            MemMove(&pe->fstring[pe->pos+1], &pe->fstring[pe->pos], size);
            pe->fstring[pe->pos] = ' ';
            pe->no ++;
            retcode = _TRUE;
          }
        }
        break;
      case _DEL:
        if(pe->pos < pe->no)
        {
          size = (pe->no - pe->pos) * sizeof(_CHAR);
          MemMove(&pe->fstring[pe->pos], &pe->fstring[pe->pos+1], size);
          pe->no --;
          retcode = _TRUE;
        }
        break;
      case _PGLEFT :
        if(SetUp.GetUseShuttleForEdit() == _TRUE)
        {
          pe->timer_cursorjump = get_abstime();
          pe->fstring[pe->pos] = astrict(pe->fstring[pe->pos], -1, 32, 95, _TRUE);
          pe->fstring[pe->pos+1] = 0;
          pe->editshuttle = _TRUE;
          retcode = _TRUE;
        }
        break;
      case _PGRIGHT :
        if(SetUp.GetUseShuttleForEdit() == _TRUE)
        {
          pe->timer_cursorjump = get_abstime();
          pe->fstring[pe->pos] = astrict(pe->fstring[pe->pos], 1, 32, 95, _TRUE);
          pe->fstring[pe->pos+1] = 0;
          pe->editshuttle = _TRUE;
          retcode = _TRUE;
        }
        break;
      default :

       #ifdef UC_JAPANINPUT      
        if(sc == _JAPAN_KEYCODE)
        {
          if(Japan_InputMode() != 0)
          {
            pe->japan_begin_pos = pe->pos;
//            pe->no = AsciiToJapanesePosition(pe->fstring, &pe->pos, pe->fstring);
          }
        }
       #endif
       
        if((sc > 31) && (is_character == _TRUE))
        {
         #ifndef UC_UNICODE
          if(sc < 256)
         #endif
          {
            if(pe->pos < pe->max)
            {
              if((isnumeric == _TRUE) && (sc == '-'))
              {
                if(pe->change == _FALSE)
                {
                  pe->fstring[0] = '-';
                  pe->fstring[1] = 0;
                  pe->pos = 1;
                  pe->no  = 1;
                  retcode = _TRUE;
                }
                else
                {
                  if(pe->fstring[0] == '-')
                  {
                    MemMove(&pe->fstring[0], &pe->fstring[1], (StrLen(pe->fstring, sizeof(_CHAR)))*sizeof(_CHAR));
                    if(pe->no > 0)
                      pe->no --;
                    if(pe->pos > 0)
                      pe->pos --;
                  }
                  else
                  {
                    MemMove(&pe->fstring[1], &pe->fstring[0], (StrLen(pe->fstring, sizeof(_CHAR))+1) * sizeof(_CHAR));
                    pe->fstring[0] = '-';
                    if(pe->no < pe->max)
                    {
                      pe->no ++;
                      pe->pos ++;
                    }
                  }
                }
              }
              else
              {
                if(insert == _TRUE)
                {

                  if(pe->no < pe->max)
                  {
                    size = (pe->no - pe->pos) * sizeof(_CHAR);
                    MemMove(&pe->fstring[pe->pos+1], &pe->fstring[pe->pos], size);

                    pe->fstring[pe->pos] = (_CHAR)sc;
                    pe->pos++;
                    pe->no++;
                  }
                }
                else
                {
                  pe->fstring[pe->pos] = (_CHAR)sc;
                  pe->pos ++;
                  if(pe->pos > pe->no)
                    pe->no  ++;
                }
              }

              retcode = _TRUE;
              pe->fstring[pe->no] = 0;
            }
            else
            {
              pe->fstring[pe->pos-1] = (_CHAR)sc;
              retcode = _TRUE;
            }
           
           #ifdef UC_JAPANINPUT
            if(Japan_InputMode() != 0)
            {
              if(pe->japan_begin_pos > pe->pos)
              {
                pe->japan_begin_pos = pe->pos;
              }
            
              pe->no = AsciiToJapanesePosition(pe->fstring, &pe->pos, pe->fstring, pe->japan_begin_pos);
            }
           #endif 
          }
        }

    }


    if((retcode == _TRUE) && (pe->no > 0))
    {
      pe->change = _TRUE;
    }
  }

  return(retcode);
}

#ifdef USER_BILLION
_BOOL _Picture::SaveEditDaTim(void)
{
  _BOOL     retcode = _TRUE;
  _RESULT   result  = Pic.Editor.result;
  _SYSERROR error   = (Pic.Editor.input.io.typ == AS_TIME)? INVALID_TIME : INVALID_DATE;

  switch(call_billionchecklimit(&Pic.Editor, &result))
  {
    case 1 : SystemError(error, result.rmin, (_CHAR*)&Pic.Editor);
             retcode = call_billionlimit(&Pic.Editor, &result, _TRUE);
             break;
    case 2 : SystemError(error, result.rmax, (_CHAR*)&Pic.Editor); //);
             retcode = call_billionlimit(&Pic.Editor, &result, _FALSE);
             break;
  }

  if(retcode == _TRUE)
  {
    retcode = SaveEdit();
  }

  return(retcode);
}
#endif

_BOOL round_dint(_DWORD limit, _EDITOR *pedi)
{
  _BOOL   retcode = _FALSE;
  _RESULT res;

  init_RESULT_Makro(&res);
  res.rmin  = limit;
  res.rmax  = limit;
  res.value = limit;
  res.ptr   = &res.value;
  res.ftype = RES_NUMERIC;

  if(recalculate(&res, &pedi->variable, _TRUE) == _TRUE)
  {
    if(res.value == pedi->result.value)
      retcode = _TRUE;
  }

  return(retcode);
}

_DOIT CallMethod_MyIoSetValue(_RESULT *presult, _VARLABEL *pvl, void *pmyio)
{
  _DOIT retcode = _IDLE;

  if(pmyio != NULL)
  {
    if(GetRevisionOfClass_MyIo() >= 3) // ab version 0.3
    {
      void *ptr = find_lasal_method(pmyio, METHOD_MYIO_SETDATA);
      if(ptr != NULL)
      {
        load_this(pmyio);
        retcode = ((_FDOIT_PT_PT)ptr)((void*)pvl, (void*)presult);
      }
    }
  }
  
  return retcode;
}

_DOIT CallMethod_MyIoIncDecValue(_RESULT *presult, _VARLABEL *pvl, void *pmyio, _BOOL increment)
{
  _DOIT retcode = _IDLE;

  if(pmyio != NULL)
  {
    if(GetRevisionOfClass_MyIo() >= 3) // ab version 0.3
    {
      void *ptr = find_lasal_method(pmyio, METHOD_MYIO_INCDECDATA);
      if(ptr != NULL)
      {
        load_this(pmyio);
        retcode = ((_FDOIT_PT_PT_BOOL)ptr)((void*)pvl, (void*)presult, increment);
      }
    }
  }
  
  return retcode;
}

_BOOL _Picture::EditorSetValue(_RESULT *presult)
{
  _DOIT doit = CallMethod_MyIoSetValue(presult, &Pic.Editor.input.io.variable, Pic.Editor.input.io.virtname.pthis);
 
  if(doit == _IDLE)
  {
   #ifdef OK_VALIDATE
    return VarList.SetSystemData(presult, &Pic.Editor.variable, &Pic.Editor.input.io.variable, _FALSE);
   #else
    return VarList.SetSystemData(presult, &Pic.Editor.variable, &Pic.Editor.input.io.variable, _TRUE);
   #endif
  }
  
  return (doit == _IFAILED)? _FALSE : _TRUE;
}

_BOOL _Picture::SaveEditNumeric(void)
{
  _BOOL   retcode = _TRUE;
  _RESULT result  = Pic.Editor.result;

  if(Pic.Editor.variable.info.formula != DEFFORMULA)
    retcode = recalculate(&result, &Pic.Editor.variable, _FALSE);

  if(retcode == _TRUE)
  {
  #ifdef USER_BILLION
    switch(call_billionchecklimit(&Pic.Editor, &result))
    {
      case 1 : SystemError(REALVALUE_TOO_SMALL, result.rmin, (_CHAR*)&Pic.Editor);
               retcode = call_billionlimit(&Pic.Editor, &result, _TRUE);
               break;
      case 2 : SystemError(REALVALUE_TOO_BIG, result.rmax, (_CHAR*)&Pic.Editor); //);
               retcode = call_billionlimit(&Pic.Editor, &result, _FALSE);
               break;
    }
  #else
   #ifdef UC_REAL
    if(result.ftype == RES_REAL)
    {
     #ifndef UC_IPC
      if(RealComp((_REAL*)&result.value, (_REAL*)&result.rmin, IOP_SMALLER) == _TRUE)
     #else
      if(get_pREAL((_REAL*)&result.value) < get_pREAL((_REAL*)&result.rmin))
     #endif
      {
       #ifdef SA41205
        if(CutToLimit(&Pic.Editor, 1) == 1)
        {
          result.value = result.rmin;
        }
        else
       #endif 
        {
          SystemError(REALVALUE_TOO_SMALL, result.rmin, (_CHAR*)&Pic.Editor); //);
          retcode = _FALSE;
        }
      }
     #ifndef UC_IPC
      if(RealComp((_REAL*)&result.value, (_REAL*)&result.rmax, IOP_BIGGER) == _TRUE)
     #else
      if(get_pREAL((_REAL*)&result.value) > get_pREAL((_REAL*)&result.rmax))
     #endif
      {
       #ifdef SA41205
        if(CutToLimit(&Pic.Editor, 2) == 2)
        {
          result.value = result.rmax;
        }
        else
       #endif 
        {
          SystemError(REALVALUE_TOO_BIG, result.rmax, (_CHAR*)&Pic.Editor); //);
          retcode = _FALSE;
        }
      }
    }
    else
   #endif
    {
      if(result.value < result.rmin)
      {
       #ifdef SA41205
        if(CutToLimit(&Pic.Editor, 1) == 1)
        {
          result.value = result.rmin;
        }
       #else 
        if(round_dint(result.rmin, &Pic.Editor) == _TRUE)
        {
          result.value = result.rmin;
        }
       #endif 
        else
        {
         #ifdef USER_DEKEMA	// Leichr 27.01.2015
          //MJ begin
          Pic.Editor.result.value = result.value;
          //MJ end
         #endif
          SystemError(NUMVALUE_TOO_SMALL, result.rmin, (_CHAR*)&Pic.Editor); //);
          retcode = _FALSE;
        }
      }
      else if(result.value > result.rmax)
      {
       #ifdef SA41205
        if(CutToLimit(&Pic.Editor, 2) == 2)
        {
          result.value = result.rmax;
        }
       #else
        if(round_dint(result.rmax, &Pic.Editor) == _TRUE)
        {
          result.value = result.rmax;
        }
       #endif 
        else
        {
         #ifdef USER_DEKEMA	// Leichr 27.01.2015
          //MJ begin
          Pic.Editor.result.value = result.value;
          //MJ end
         #endif
          SystemError(NUMVALUE_TOO_BIG, result.rmax, (_CHAR*)&Pic.Editor); //);
          retcode = _FALSE;
        }
      }
    }
  #endif

    if(retcode == _TRUE)
    {
      retcode = _FALSE;
      if(EditorSetValue(&result) == _TRUE)
      {
//      Ptr[Editor.nr_subpict].OpenEditor(&Editor, Editor.actno);
        Pic.Editor.chredit.pos    = 0;
        Pic.Editor.chredit.no     = 0;
        Pic.Editor.chredit.change = _FALSE;
        Pic.Editor.chredit.incdec = _TRUE;
        retcode                   = _TRUE;
      }
    }
  }

  return(retcode);
}

_BOOL _Picture::SaveEdit(void)
{
  _BOOL retcode = _FALSE;

  if(EditorSetValue(&Pic.Editor.result) == _TRUE)
  {
    Pic.Editor.chredit.pos    = 0;
    Pic.Editor.chredit.no     = 0;
    Pic.Editor.chredit.change = _FALSE;
    retcode                   = _TRUE;
  }

  return(retcode);
}

_BOOL _Picture::SaveEditAscii(void)
{
  _BOOL retcode = _FALSE;
  void *hptr;

  if(Pic.Editor.chredit.change == _TRUE)
  {
    hptr              = Pic.Editor.result.ptr;
    Pic.Editor.result.ptr = Pic.Editor.chredit.fstring;

    if(EditorSetValue(&Pic.Editor.result) == _TRUE)
    {
      Pic.Editor.chredit.pos    = 0;
      Pic.Editor.chredit.no     = 0;
      Pic.Editor.chredit.change = _FALSE;
      retcode               = _TRUE; 
    }

    Pic.Editor.result.ptr = hptr;
  }

  return(retcode); 
}

#ifdef OK_SEEK_BUTTON
_BOOL _Picture::EditButton(_UWORD sc, _BOOL press_state, _BOOL lrm)
{
  _BOOL retcode = _FALSE;

  if(sc == _ENTER)
  {
    retcode = action_SEEKELEMENT(&Pic, Pic.Editor.actno, press_state, Pic.P_Window, lrm);
  }

  return(retcode);
}
#endif

_BOOL _Picture::EditMenu(_UWORD sc, _UWORD menu_no, _BOOL hid)
{
  _DOT      offset;
  _ZOOM     zoom;

  if(sc == _ENTER) //&& (Editor.chredit.change == _FALSE))
  {
    Cursor.End();

    if(Pic.Editor.input.io.typ == AS_ENUMERATION)
      MenuList.DesignEnumerationMenu(menu_no, &Pic.Editor.input.io, &Pic.Editor.input.comboinfo);

    _IO *pio = &Pic.Editor.input.io;
    Root.FindTop()->OpenComboBox(menu_no, &pio->variable, &pio->imoverloaded, pio->space.xy1.x, pio->space.xy2.y, hid, _FALSE, pio->virtname.pthis);

/*
    _VARLABEL op1, op2;
    
    init_VARLABEL_Makro(&op1);
    init_VARLABEL_Makro(&op2);

    op1.no = 1;
    op1.info[0].state = CONST_VAL;
    op1.info[0].value = menu_no;
    op1.info[1].state = CONST_VAL;
    op1.info[1].value = Pic.Editor.input.io.space.xy1.x;
    op1.info[2].state = CONST_VAL;
    op1.info[2].value = Pic.Editor.input.io.space.xy2.y;

    op2 = Pic.Editor.input.io.variable;

    // es wird die information zum überladen von menue eingetragen
    if(valid_VARLABEL(&Pic.Editor.input.io.imoverloaded) == _TRUE)
    {
      if((op2.no <= VL_MAXINDEX)&&(Pic.Editor.input.io.imoverloaded.no == 1)&&(Pic.Editor.input.io.imoverloaded.info[0].state == VAR_VAL))
        op2.info[VL_MAXINDEX] = Pic.Editor.input.io.imoverloaded.info[0];
    }

    
    Pic.P_Window->AddCommand((hid == _TRUE)? CMD_IOPOPUP_HID : CMD_IOPOPUP, &op1, &op2, _FALSE);      // add command
*/    
    
    init_DOT(&offset);
    init_ZOOM(&zoom);
   #ifdef UC_TOUCH
    if(SetUp.GetUseTouchEditor() == _TRUE)
      out_SEEKELEMENT(&Pic, Pic.Editor.actno, _EDITCHANGE, &offset, &zoom);
   #endif
  }

  return(_FALSE);
}

#ifdef USER_BILLION
_BOOL _Picture::ChkBillionNeeds(_EDITOR *pe)
{
  _BOOL     retcode = _FALSE;
  _RESULT   result;
  _VARIABLE vtmp;

  init_RESULT_Makro(&result);

  if(VarList.GetSystemData(&result, &vtmp, &pe->input.io.variable, _TRUE, _TRUE) == _TRUE)
  {
    if(EditorToResult(&result, pe) == _TRUE)
    {
      retcode = _TRUE;
      if(Pic.Editor.variable.info.formula != DEFFORMULA)
        retcode = recalculate(&result, &pe->variable, _FALSE);
    
      if(retcode == _TRUE)  
      {
       #ifdef UC_REAL
        if(result.ftype == RES_REAL)
        {
         #ifndef UC_IPC
          if(RealComp((_REAL*)&result.value, (_REAL*)&result.rmin, IOP_SMALLER) == _TRUE)
         #else
          if(get_pREAL((_REAL*)&result.value) < get_pREAL((_REAL*)&result.rmin))
         #endif
          {
            retcode = call_billionlimit(pe, &result, _TRUE);
          }
         #ifndef UC_IPC
          if(RealComp((_REAL*)&result.value, (_REAL*)&result.rmax, IOP_BIGGER) == _TRUE)
         #else
          if(get_pREAL((_REAL*)&result.value) > get_pREAL((_REAL*)&result.rmax))
         #endif
          {
            retcode = call_billionlimit(pe, &result, _FALSE);
          }
        }
        else
       #endif
        {
          if(result.value < result.rmin)
          {
            retcode = call_billionlimit(pe, &result, _TRUE);
          }
          else if(result.value > result.rmax)
          {
            retcode = call_billionlimit(pe, &result, _FALSE);
          }
        }
      }
    }
  }

  return(retcode);
}
#endif

#ifdef UC_EDITOR_RAMP
_UDWORD testtimer;
_UDWORD testcnt;
#endif

_BOOL _Picture::EditNumeric(_UWORD sc, _UWORD mul, _IO_TYPE typ, _BOOL is_character)
{
  _BOOL    c, retcode = _FALSE;
  _UWORD   nk;
  _IVALUE  value, oldvalue;
  _DWORD   tmpd;
  _CHREDIT tmpchredi;
  _BOOL    tmp1;
  _REAL rval;

 #ifdef USER_LISEC
  delete_eventbuffer();
 #endif
#ifdef UC_EDITOR_RAMP
 #ifdef USER_LISEC
  if(timediff(testtimer) < 200)
 #else
  if(timediff(testtimer) < 150)
 #endif
    testcnt += 1;              // Rampe erhöhen
  else
    testcnt = 0;

  testtimer = get_abstime();
#endif

 #ifdef USER_DEKEMA
  if((sc != _PGDOWN)&&(sc != _PGUP))
  {
    DekemaMerker = _TRUE;
  }

  if(sc == _PGDOWN)
  {
    if((Pic.Editor.chredit.change == _TRUE)&&(DekemaMerker == _TRUE))
      sc = '-';
  }
 #endif

  Pic.Editor.chredit.max = 100;

  if(sc == ',')
  {
    if(get_commasign() == _FALSE)
      sc = '.'; // komma in punkt umwandeln
  }

 #ifdef SA28815
  if((sc != '-') && (sc != '+'))
 #endif 
  { 
    if(SetUp.GetEditorIncDec() == _TRUE) // lse-einstellung
    {
      if((sc == '-') && (Pic.Editor.chredit.change == _FALSE))
      {
        sc           = _PGDOWN;
//        is_character = _FALSE;
      }
      else if((sc == '+') && (Pic.Editor.chredit.change == _FALSE))
      {
        sc           = _PGUP;
//        is_character = _FALSE;
      }
    }
    else
    {
      _BOOL st = _FALSE;
      if(is_character == _TRUE)
      {
       #ifdef USER_BILLION
        //SA8428
        if(sc == '-')
        {
          if(Pic.Editor.chredit.change == _FALSE)
          {
            Pic.Editor.chredit.change = _TRUE;
            Pic.Editor.chredit.fstring[0] = '-';
            format_value(&Pic.Editor.chredit.fstring[1], Pic.Editor.result.value, &Pic.Editor.variable.info, &Pic.Editor.input.io.picformat);
            DelChr(Pic.Editor.chredit.fstring, ' ', sizeof(_CHAR));
            Pic.Editor.chredit.pos = 1;

            if(Pic.Editor.result.value < 0)
            {
              DelChr(Pic.Editor.chredit.fstring, '-', sizeof(_CHAR));
              Pic.Editor.chredit.pos = 0;
            }

            Pic.Editor.chredit.no = StrLen(Pic.Editor.chredit.fstring, sizeof(_CHAR));
            Pic.Editor.chredit.incdec = _FALSE;
            retcode = _TRUE;
            sc = 0;
          }
        }
        else if(((sc >= '0')&&(sc <= '9'))||(sc == '.')||(sc == ','))
        {
          if(Pic.Editor.chredit.change == _TRUE)
          {
            if(Pic.Editor.chredit.fstring[0] == '-')
            {
              if(Pic.Editor.chredit.pos == 1)
              {
                Pic.Editor.chredit.fstring[1] = 0;
                Pic.Editor.chredit.no = 1;
              }
            }
            else if(Pic.Editor.chredit.pos == 0)
            {
              Pic.Editor.chredit.fstring[0] = 0;
              Pic.Editor.chredit.no = 0;
            }
          }
          else
          {
            if(Pic.Editor.result.value < 0)
            {
              Pic.Editor.chredit.fstring[0] = '-';
              Pic.Editor.chredit.fstring[1] = 0;
              Pic.Editor.chredit.pos = 1;
              Pic.Editor.chredit.no  = 1;
              Pic.Editor.chredit.incdec = _FALSE;
              Pic.Editor.chredit.change = _TRUE;
            }
          }
        }
       #else
        if((sc == '+')||(sc == '-'))
          st = _TRUE;
       #endif
      }
      else
      {
        if((sc == _PGUP)||(sc == _PGDOWN)||(sc == _PGLEFT)||(sc == _PGRIGHT))
          st = _TRUE;
      }

      if(st == _TRUE)
      {
        if(Pic.Editor.chredit.incdec == _TRUE)
        {
          if(Pic.Editor.chredit.change == _FALSE)
          {
            format_value(Pic.Editor.chredit.fstring, Pic.Editor.result.value, &Pic.Editor.variable.info, &Pic.Editor.input.io.picformat);
            Pic.Editor.chredit.change = _TRUE;
          }
        
          _DWORD   adsub;
         #ifdef UC_EDITOR_RAMP
          if(testcnt < 10)
            adsub = 1;
          else if(testcnt < 20)
            adsub = (testcnt/10)*5;
          else if(testcnt < 50)
            adsub = (testcnt/10)*10;
          else
            adsub = (testcnt/10)*100;
         #else
	        adsub = 1;
         #endif

          if((sc == _PGUP)||(sc == '+')||(sc == _PGRIGHT))
          {
            add_numstring(Pic.Editor.chredit.fstring, adsub);
          }
          else
          {
            add_numstring(Pic.Editor.chredit.fstring, -adsub);
          }
          retcode = _TRUE;
          sc = 0;
        }
      }
    }
  }
  
  if((sc == CPYEDIT)&&(typ == AS_NUMERIC_HIDE))
    sc = 0;

  switch(sc)
  {
#ifdef ET_261
    // diese änderung wurde am 20.05.05 eingebaut (sa 5585)
    case _PGRIGHT:
#endif
    case _PGUP   : if(SetUp.GetEditorIncDec() == _TRUE)
                   {
                     oldvalue = Pic.Editor.result.value;
                     _DOIT doit = CallMethod_MyIoIncDecValue(&Pic.Editor.result, &Pic.Editor.input.io.variable, Pic.Editor.input.io.virtname.pthis, true);
                     if(doit == _IDIDIT)
                     {
                       if(oldvalue != Pic.Editor.result.value)
                       {
                         Pic.Editor.chredit.change = _TRUE;
                         retcode = SaveEditor(_FALSE);
                       }
                     }
                     else if(doit == _IDLE)
                     {
                       Pic.Editor.incdec = calcul_incdec_EDITOR(&Pic.Editor, Pic.Editor.result.value+Pic.Editor.incdec);
                       Pic.Editor.incdec = calcul_incdec_EDITOR(&Pic.Editor, Pic.Editor.result.value+Pic.Editor.incdec);

                       if((Pic.Editor.variable.info.vartype == BINARY)&&(Pic.Editor.variable.info.format == SIZE_REAL))
                       {
                        #ifndef UC_IPC
                         RealCalc((_REAL*)&tmpd, (_REAL*)&Pic.Editor.incdec, (_REAL*)&mul, IOP_A_MULTI);
                        #else
                         move_pREAL_pREAL(&rval, (_REAL*)&Pic.Editor.incdec);
                         move_pREAL_REAL((_REAL*)&tmpd, rval * mul);
                         //*(_REAL*)&tmpd = *(_REAL*)&Pic.Editor.incdec * mul;
                        #endif
                       }
                       else
                       {
                         tmpd = Pic.Editor.incdec * mul;
                       }
                       
                       value = Pic.Editor.result.value;
                       result_addsub(&value, Pic.Editor.result.ftype, &tmpd, Pic.Editor.result.ftype, '+', NULL, NULL);
                       if(Pic.Editor.result.value != value)
                       {
                         Pic.Editor.result.value   = value;
                         Pic.Editor.chredit.change = _TRUE;
                         retcode = SaveEditor(_FALSE);
                         if(retcode == _FALSE)
                           Pic.Editor.result.value = oldvalue;
                       }
                     }
                   }
#ifdef ET_261
                   // diese änderung wurde am 20.05.05 eingebaut (sa 5585)
                   if(sc == _PGRIGHT)
                     Pic.Editor.chredit.change = _TRUE;
#endif

                   break;
#ifdef ET_261
    // diese änderung wurde am 20.05.05 eingebaut (sa 5585)
    case _PGLEFT:
#endif
    case _PGDOWN : if(SetUp.GetEditorIncDec() == _TRUE)
                   {
                     oldvalue = Pic.Editor.result.value;
                     _DOIT doit = CallMethod_MyIoIncDecValue(&Pic.Editor.result, &Pic.Editor.input.io.variable, Pic.Editor.input.io.virtname.pthis, false);
                     if(doit == _IDIDIT)
                     {
                       if(oldvalue != Pic.Editor.result.value)
                       {
                         Pic.Editor.chredit.change = _TRUE;
                         retcode = SaveEditor(_FALSE);
                       }
                     }
                     else if(doit == _IDLE)
                     {
                       Pic.Editor.incdec = calcul_incdec_EDITOR(&Pic.Editor, Pic.Editor.result.value-Pic.Editor.incdec);
                       Pic.Editor.incdec = calcul_incdec_EDITOR(&Pic.Editor, Pic.Editor.result.value-Pic.Editor.incdec);

                       if((Pic.Editor.variable.info.vartype == BINARY)&&(Pic.Editor.variable.info.format == SIZE_REAL))
                       {
                        #ifndef UC_IPC
                         RealCalc((_REAL*)&tmpd, (_REAL*)&Pic.Editor.incdec, (_REAL*)&mul, IOP_A_MULTI);
                        #else
                         move_pREAL_pREAL(&rval, (_REAL*)&Pic.Editor.incdec);
                         move_pREAL_REAL((_REAL*)&tmpd, rval * mul);
//                         *(_REAL*)&tmpd = *(_REAL*)&Pic.Editor.incdec * mul;
                        #endif
                       }
                       else
                       {
                         tmpd  = Pic.Editor.incdec * mul;
                       }
                       
                       value = Pic.Editor.result.value;
                       result_addsub(&value, Pic.Editor.result.ftype, &tmpd, Pic.Editor.result.ftype, '-', NULL, NULL);
                       if(Pic.Editor.result.value != value)
                       {
                         Pic.Editor.result.value   = value;
                         Pic.Editor.chredit.change = _TRUE;
                         retcode = SaveEditor(_FALSE);
                         if(retcode == _FALSE)
                           Pic.Editor.result.value = oldvalue;
                       }
                     }
                   }
#ifdef ET_261
                   // diese änderung wurde am 20.05.05 eingebaut (sa 5585)
                   if(sc == _PGLEFT)
                     Pic.Editor.chredit.change = _TRUE;
#endif
                   break;
    case '.'     :
    case ','     :
    case '*'     :
    case '/'     :
    case '+'     :
    case '-'     :
    case _ESC    :
    case _POS1   :
    case _END    :
    case _LEFT   :
    case _RIGHT  :
    case _BCKSPC : retcode = run_CHREDIT(&Pic.Editor.chredit, sc, _FALSE, _TRUE, _TRUE);
                   Pic.Editor.chredit.incdec = _FALSE;
                   break;
    case '0'     :
    case '1'     :
    case '2'     :
    case '3'     :
    case '4'     :
    case '5'     :
    case '6'     :
    case '7'     :
    case '8'     :
    case '9'     : c = Pic.Editor.chredit.change;
                   if(Pic.Editor.chredit.incdec == _TRUE)
                     c = _FALSE;
                   if(scan_string(Pic.Editor.chredit.fstring, c, &Pic.Editor) == _TRUE)
                   {
//B                    #ifdef USER_BILLION
//B                     _CHREDIT chtm = Pic.Editor.chredit;
//B                    #endif
//                   if(scan_string(Pic.Editor.chredit.fstring, Pic.Editor.chredit.change, &Pic.Editor) == _TRUE)
                     retcode = run_CHREDIT(&Pic.Editor.chredit, sc, _FALSE, _TRUE, _TRUE);
//B                    #ifdef USER_BILLION
//B                     if(ChkBillionNeeds(&Pic.Editor) == _FALSE)
//B                     {
//B                       Pic.Editor.chredit = chtm;
//B                       retcode = _FALSE;
//B                     }
//B                    #endif

                     Pic.Editor.chredit.incdec = _FALSE;
                   }
                   break;
    case CPYEDIT : tmpchredi = Pic.Editor.chredit;
                   if(restore_CHREDIT(&Pic.Editor) == _TRUE) // sieht komisch aus, stimmt aber so !!
    case _DIRECTIONENTER :
    case _ENTER  :
                   {
                     if((SetUp.GetUseShuttleForEdit() == _TRUE)&&(Pic.Editor.chredit.change == _FALSE))
                     {
                       Pic.Editor.chredit.change = _TRUE;
                       retcode = _TRUE;
                     }
                     else
                     {
                       if((sc != CPYEDIT)&&(typ != AS_NUMERIC_HIDE))
                         store_CHREDIT(&Pic.Editor.chredit);
                       if(chk_vorkommastellen(Pic.Editor.chredit.fstring, &Pic.Editor) == _TRUE)
                       {
                         nk = VarList.GetDezPoint(&Pic.Editor.variable.info);
                         Pic.Editor.result.value = string_to_value(string_to_numeric(Pic.Editor.chredit.fstring, nk), &Pic.Editor.variable.info);

                         tmp1 = Pic.Editor.chredit.change; // issue95
                         retcode = SaveEditor(_FALSE);
                         if(retcode == _FALSE) // issue95
                         {
                           // direction on enter verhindern, wennn wert nicht abgespeichert wurde
                           Pic.Editor.disable_direction_enter = _TRUE;

                          #ifdef USER_DEMAG  // SA 7745
                           Pic.Editor.chredit.fstring[0] = 0;
                           Pic.Editor.chredit.pos = 0;
                           Pic.Editor.chredit.no = 0;
                           retcode = _TRUE;
                          #endif

                           // falls wert nicht in system geschrieben werden konnte (zb.Grenzwertverletzung) sollte Änderungsstatus alten Zustand beibehalten. 
                           // Notwendig wenn Eingabe nicht übernommen und anschliessend ESC gedrückt wurde. 
                           Pic.Editor.chredit.change = tmp1; 
                         }
                         else
                         {
                          #ifdef USER_DEMAG  // SA 8142
                           Pic.P_Window->InsertCmd(CMD_CLOSEMINMAX, 0, 0, _FALSE);
                          #endif
                           Pic.Editor.disable_direction_enter = _FALSE;
                         }
                       }
                     }
                   }
                   if(sc == CPYEDIT)
                   {
                     Pic.Editor.enterdirection = retcode;
                     if(retcode == _FALSE)
                     {
                       Pic.Editor.chredit = tmpchredi;
                       retcode            = _TRUE;
                     }
                   }
                   break;
  }

  return(retcode);
}

#ifdef UC_JAPANINPUT
static _BOOL JapanDiacrit(_CHREDIT *pce, _UWORD scancode)
{
  if(pce->pos > 0)
  {
    _CHAR *ph = &pce->fstring[pce->pos - 1];
  
    if(scancode == JAPAN_DIACRIT_1)
    {
      switch(*ph)
      {
        case 0x304B: *ph = 0x304C; return _TRUE; case 0x304C: *ph = 0x304B; return _TRUE;
        case 0x304D: *ph = 0x304E; return _TRUE; case 0x304E: *ph = 0x304D; return _TRUE;
        case 0x304F: *ph = 0x3050; return _TRUE; case 0x3050: *ph = 0x304F; return _TRUE;
        case 0x3051: *ph = 0x3052; return _TRUE; case 0x3052: *ph = 0x3051; return _TRUE;
        case 0x3053: *ph = 0x3054; return _TRUE; case 0x3054: *ph = 0x3053; return _TRUE;
        case 0x3055: *ph = 0x3056; return _TRUE; case 0x3056: *ph = 0x3055; return _TRUE;
        case 0x3057: *ph = 0x3058; return _TRUE; case 0x3058: *ph = 0x3057; return _TRUE;
        case 0x3059: *ph = 0x305A; return _TRUE; case 0x305A: *ph = 0x3059; return _TRUE;
        case 0x305B: *ph = 0x305C; return _TRUE; case 0x305C: *ph = 0x305B; return _TRUE;
        case 0x305D: *ph = 0x305E; return _TRUE; case 0x305E: *ph = 0x305D; return _TRUE;
        case 0x305F: *ph = 0x3060; return _TRUE; case 0x3060: *ph = 0x305F; return _TRUE;
        case 0x3061: *ph = 0x3062; return _TRUE; case 0x3062: *ph = 0x3061; return _TRUE;
        case 0x3064: *ph = 0x3065; return _TRUE; case 0x3065: *ph = 0x3064; return _TRUE;
        case 0x3066: *ph = 0x3067; return _TRUE; case 0x3067: *ph = 0x3066; return _TRUE;
        case 0x3068: *ph = 0x3069; return _TRUE; case 0x3069: *ph = 0x3068; return _TRUE;
        case 0x306F: *ph = 0x3070; return _TRUE; case 0x3070: *ph = 0x306F; return _TRUE;
        case 0x3072: *ph = 0x3073; return _TRUE; case 0x3073: *ph = 0x3072; return _TRUE;
        case 0x3075: *ph = 0x3076; return _TRUE; case 0x3076: *ph = 0x3075; return _TRUE;
        case 0x3078: *ph = 0x3079; return _TRUE; case 0x3079: *ph = 0x3078; return _TRUE;
        case 0x307B: *ph = 0x307C; return _TRUE; case 0x307C: *ph = 0x307B; return _TRUE;
        
        case 0x30AB: *ph = 0x30AC; return _TRUE; case 0x30AC: *ph = 0x30AB; return _TRUE;
        case 0x30AD: *ph = 0x30AE; return _TRUE; case 0x30AE: *ph = 0x30AD; return _TRUE;
        case 0x30AF: *ph = 0x30B0; return _TRUE; case 0x30B0: *ph = 0x30AF; return _TRUE;
        case 0x30B1: *ph = 0x30B2; return _TRUE; case 0x30B2: *ph = 0x30B1; return _TRUE;
        case 0x30B3: *ph = 0x30B4; return _TRUE; case 0x30B4: *ph = 0x30B3; return _TRUE;
        case 0x30B5: *ph = 0x30B6; return _TRUE; case 0x30B6: *ph = 0x30B5; return _TRUE;
        case 0x30B7: *ph = 0x30B8; return _TRUE; case 0x30B8: *ph = 0x30B7; return _TRUE;
        case 0x30B9: *ph = 0x30BA; return _TRUE; case 0x30BA: *ph = 0x30B9; return _TRUE;
        case 0x30BB: *ph = 0x30BC; return _TRUE; case 0x30BC: *ph = 0x30BB; return _TRUE;
        case 0x30BD: *ph = 0x30BE; return _TRUE; case 0x30BE: *ph = 0x30BD; return _TRUE;
        case 0x30BF: *ph = 0x30C0; return _TRUE; case 0x30C0: *ph = 0x30BF; return _TRUE;
        case 0x30C1: *ph = 0x30C2; return _TRUE; case 0x30C2: *ph = 0x30C1; return _TRUE;
        case 0x30C4: *ph = 0x30C5; return _TRUE; case 0x30C5: *ph = 0x30C4; return _TRUE;
        case 0x30C6: *ph = 0x30C7; return _TRUE; case 0x30C7: *ph = 0x30C6; return _TRUE;
        case 0x30C8: *ph = 0x30C9; return _TRUE; case 0x30C9: *ph = 0x30C8; return _TRUE;
        case 0x30CF: *ph = 0x30D0; return _TRUE; case 0x30D0: *ph = 0x30CF; return _TRUE;
        case 0x30D2: *ph = 0x30D3; return _TRUE; case 0x30D3: *ph = 0x30D2; return _TRUE;
        case 0x30D5: *ph = 0x30D6; return _TRUE; case 0x30D6: *ph = 0x30D5; return _TRUE;
        case 0x30D8: *ph = 0x30D9; return _TRUE; case 0x30D9: *ph = 0x30D8; return _TRUE;
        case 0x30DB: *ph = 0x30DC; return _TRUE; case 0x30DC: *ph = 0x30DB; return _TRUE;
      }
    }
    else if(scancode == JAPAN_DIACRIT_2)
    {
      switch(*ph)
      {
        case 0x306F: *ph = 0x3071; return _TRUE; case 0x3071: *ph = 0x306F; return _TRUE;
        case 0x3072: *ph = 0x3074; return _TRUE; case 0x3074: *ph = 0x3072; return _TRUE;
        case 0x3075: *ph = 0x3077; return _TRUE; case 0x3077: *ph = 0x3075; return _TRUE;
        case 0x3078: *ph = 0x307A; return _TRUE; case 0x307A: *ph = 0x3078; return _TRUE;
        case 0x307B: *ph = 0x307D; return _TRUE; case 0x307D: *ph = 0x307B; return _TRUE;
        
        case 0x30CF: *ph = 0x30D1; return _TRUE; case 0x30D1: *ph = 0x30CF; return _TRUE;
        case 0x30D2: *ph = 0x30D4; return _TRUE; case 0x30D4: *ph = 0x30D2; return _TRUE;
        case 0x30D5: *ph = 0x30D7; return _TRUE; case 0x30D7: *ph = 0x30D5; return _TRUE;
        case 0x30D8: *ph = 0x30DA; return _TRUE; case 0x30DA: *ph = 0x30D8; return _TRUE;
        case 0x30DB: *ph = 0x30DD; return _TRUE; case 0x30DD: *ph = 0x30DB; return _TRUE;
      }    
    }
  }
  return _FALSE;
}
#endif

_BOOL _Picture::EditAscii(_EVENT *pe)
{
  _BOOL retcode = _FALSE;

  if(MultiKeyboard.no != 0)
  {
    if(MultiKeyboardRun(&pe->scancode, pe->timestamp) == _TRUE) // do handykey
      run_CHREDIT(&Pic.Editor.chredit, _BCKSPC, _FALSE, _FALSE, _FALSE); // letztes zeichen im eingabestream löschen
  }

 #ifdef UC_INSERT_MODE
  if(pe->scancode == _INS)
  {
    VarList.VarDummySet(VARNO_INSERTFLAG, (VarList.VarDummyGet(VARNO_INSERTFLAG) == 0)? 1 : 0); // insertflag setzen
    return(_TRUE);
  }
 #endif

  if((pe->scancode == _RIGHT)||(pe->scancode == _LEFT)||(pe->scancode == _POS1)||(pe->scancode == _END)||(pe->scancode == _BCKSPC))
  {
    if(Pic.Editor.chredit.change == _FALSE)
    {
      Pic.Editor.chredit.no = StrLen(Pic.Editor.chredit.fstring, sizeof(_CHAR));
      if((pe->scancode == _RIGHT)||(pe->scancode == _POS1))
        retcode = run_CHREDIT(&Pic.Editor.chredit, _POS1, _FALSE, _FALSE, pe->is_character);
      else
        retcode = run_CHREDIT(&Pic.Editor.chredit, _END, _FALSE, _FALSE, pe->is_character);
      return(retcode);
    }
  }

  Pic.Editor.chredit.timer_cursorjump = 0;

  switch(pe->scancode)
  {
    case _DIRECTIONENTER :
    case _ENTER :
                  if(SetUp.GetUseShuttleForEdit() == _TRUE)
                  {
                    if(Pic.Editor.chredit.change == _FALSE)
                    {
                      Pic.Editor.chredit.change = _TRUE;
                      retcode = _TRUE;
                    }
                    /*
                    else if(Pic.Editor.chredit.editshuttle == _TRUE)
                    {
                      if(Pic.Editor.chredit.pos < (Pic.Editor.chredit.max - 1))
                      {
                        Pic.Editor.chredit.pos++;
                        Pic.Editor.chredit.chr = 32;
                        retcode = run_CHREDIT(&Pic.Editor.chredit, Pic.Editor.chredit.chr, _FALSE, _FALSE, pe->is_character);
                        Pic.Editor.chredit.pos--;
                      }
                      else
                      {
                        retcode = SaveEditor(_FALSE);
                      }
                    }
                    */
                  }
                  if(retcode == _FALSE)
                  {
//                   #ifdef UC_JAPANINPUT
//                    if(Japan_InputMode() != 0)
//                    {
//                      if(Pic.Editor.japan_count_ENTER < 1)
//                      {
//                        // JapanInputMode und ENTER GEDRÜCKT
//                        Pic.Editor.japan_count_ENTER ++;
//                        Pic.Editor.chredit.japan_pos = Pic.Editor.chredit.pos;
//                        retcode = _TRUE;
//                      }
//                      else
//                      {
//                        retcode = SaveEditor(_FALSE);
//                        if(retcode == _TRUE)
//                        {
//                          InsertCmd(CMD_ACKNOWLEDGEFLAG, 2, 0);
//                          InsertCmd(CMD_RETURN, 0, 0);
//                        }
//                      }
//                    }
//                    else
//                   #endif
//                    {
                      retcode = SaveEditor(_FALSE);
//                    }
                  }
                  break;

    case _ALTE:
                  if(Pic.Editor.chredit.change == _FALSE)
                  {
                    Pic.Editor.chredit.no = StrLen(Pic.Editor.chredit.fstring, sizeof(_CHAR));
                    retcode = run_CHREDIT(&Pic.Editor.chredit, _END, _FALSE, _FALSE, pe->is_character);
                  }
                  break;

    default     :
/*
                  if(SetUp.GetUseShuttleForEdit() == _TRUE)
                  {
                    sc = pe->scancode;
                    if(sc == _PGRIGHT)
                    {
                      Pic.Editor.chredit.chr = astrict(Pic.Editor.chredit.chr, 1, 32, 95);
                    }
                    if(sc == _PGLEFT)
                    {
                      Pic.Editor.chredit.chr = astrict(Pic.Editor.chredit.chr, -1, 32, 95);
                    }
                    sc = Pic.Editor.chredit.chr;

                    retcode = run_CHREDIT(&Pic.Editor.chredit, sc, _FALSE, _FALSE, pe->is_character);
                    if(Pic.Editor.chredit.pos > 0)
                      Pic.Editor.chredit.pos--;
                  }
                  else
*/
                 
              
                 #ifdef UC_UNICODE
                  _BOOL state = _TRUE;
                  if((pe->is_character == _TRUE)&&(pe->scancode > 255))
                  {
                    if(VarList.GetOnlyAscii(&Pic.Editor.variable.info) == _TRUE)
                    {
                      state = _FALSE; // Zeichen nicht in Editor übernehmen
                    }
                  }
    
                 #ifdef UC_JAPANINPUT
                  if((pe->is_character == _TRUE)&&((pe->scancode == JAPAN_DIACRIT_1)||(pe->scancode == JAPAN_DIACRIT_2)))
                  {
                    // Vorsicht: Diacrits können am Lars nicht richtig angezeigt werden
                    state = _FALSE; // Diacrit wird nicht in Editor übernommen
                    if(JapanDiacrit(&Pic.Editor.chredit, pe->scancode) == _TRUE)
                    {
                      retcode = _TRUE; // Diacrit wurde mit dem Vorgängerzeichen "verschmolzen", deswegen Editorinhalt neu ausgeben
                    }
                  }
                 #endif
                 
                  if(state == _TRUE)
                 #endif
                  {
//                   #ifdef UC_JAPANINPUT
//                    if((pe->scancode == ' ') && (Japan_InputMode() != 0))
//                    {
//                      // JapanInputMode und SPACE gedrückt
//                      if(Pic.Editor.japan_count_SPACE == 0)
//                      {
//                        retcode = JapanDictionaryGetFirstMatch();
//                        if(retcode == _TRUE)
//                        {
//                          Pic.Editor.japan_count_SPACE = 1;
//                        }
//                      }
//                      else
//                      {
//                        Pic.Editor.japan_count_SPACE = 0;
//                        if(JapanDictionaryOpenDropDownList(Pic.Editor.japan_keyword) == _TRUE)
//                        {
//                          retcode = _TRUE;
//                        }
//                      }
//                      
//                      if(retcode == _FALSE)
//                      {
//                        Pic.Editor.chredit.japan_pos = Pic.Editor.chredit.pos;
//                      }
//                    }
//                    else
//                    {
//                      Pic.Editor.japan_count_SPACE = 0;
//                    }
//                    
//                    if(retcode == _FALSE)
//                   #endif 
//                    {
                      retcode = run_CHREDIT(&Pic.Editor.chredit, pe->scancode, (VarList.VarDummyGet(VARNO_INSERTFLAG) == 0)? _FALSE : _TRUE, _FALSE, pe->is_character);
//                    }
                  }
                  break;
  }

  return(retcode);
}

_BOOL _Picture::EditRadio(_UWORD sc)
{
  _BOOL  retcode = _FALSE;
  _BOOL  oldcha;
  _DWORD oldval;

  switch(sc)
  {
    case _DIRECTIONENTER :
    case _ENTER  :
    case _PGDOWN :
    case _PGUP   :
    case '1'     : if(Pic.Editor.result.value != 1)
                   {
                     oldcha = Pic.Editor.chredit.change;
                     oldval = Pic.Editor.result.value;
                     Pic.Editor.result.value   = 1;
                     Pic.Editor.chredit.change = _TRUE;
                     retcode = SaveEditor(_FALSE);
                     if(retcode == _FALSE)
                     {
                       Pic.Editor.chredit.change = oldcha;
                       Pic.Editor.result.value   = oldval;
                     }
                   }
                   break;
  }

  if(retcode == _TRUE)
    do_LSEFUNCT_lrm(&Pic.Editor.input.io.lsefunct, Pic.P_Window, _FALSE);

  return(retcode);
}

_BOOL _Picture::EditChkBox(_UWORD sc)
{
  _BOOL   retcode = _FALSE;
  _BOOL   oldcha;
  _DWORD  oldval;

  switch(sc)
  {
    case _DIRECTIONENTER :
    case _ENTER  :
    case _PGDOWN :
    case _PGUP   :
                   oldcha = Pic.Editor.chredit.change;
                   oldval = Pic.Editor.result.value;
                   Pic.Editor.chredit.change = _TRUE;
                   Pic.Editor.result.value   = (Pic.Editor.result.value == 0)? 1:0;
                   retcode = SaveEditor(_FALSE);
                   if(retcode == _FALSE)
                   {
                     Pic.Editor.chredit.change = oldcha;
                     Pic.Editor.result.value   = oldval;
                   }
                   break;
    case '0'     : if(Pic.Editor.result.value != 0)
                   {
                     oldcha = Pic.Editor.chredit.change;
                     oldval = Pic.Editor.result.value;
                     Pic.Editor.result.value = 0;
                     Pic.Editor.chredit.change = _TRUE;
                     retcode = SaveEditor(_FALSE);
                     if(retcode == _FALSE)
                     {
                       Pic.Editor.chredit.change = oldcha;
                       Pic.Editor.result.value   = oldval;
                     }
                   }
                   break;
    case '1'     : if(Pic.Editor.result.value != 1)
                   {
                     oldcha = Pic.Editor.chredit.change;
                     oldval = Pic.Editor.result.value;
                     Pic.Editor.result.value = 1;
                     Pic.Editor.chredit.change = _TRUE;
                     retcode = SaveEditor(_FALSE);
                     if(retcode == _FALSE)
                     {
                       Pic.Editor.chredit.change = oldcha;
                       Pic.Editor.result.value   = oldval;
                     }
                   }
                   break;
  }

  if(retcode == _TRUE)
    do_LSEFUNCT_lrm(&Pic.Editor.input.io.lsefunct, Pic.P_Window, _FALSE);

  return(retcode);
}

_BOOL is_num(_UWORD chr)
{
  if((chr >= 0x30)&&(chr <= 0x39))
    return(_TRUE);
  return(_FALSE);
}

_BOOL check_time_separator(_CHREDIT *p, _UDWORD sc, _UDWORD format)
{
  _BOOL retcode = _FALSE;

  if((sc >= 0x30)&&(sc <= 0x39))
  {
    if(p->pos == 2)
    {
      if((is_num(p->fstring[0]) == _TRUE)&&(is_num(p->fstring[1]) == _TRUE))
      {
        retcode = _TRUE;
      }
    }
    if((p->pos == 5)&&(format == SIZE_HHMMSS))
    {
      if((is_num(p->fstring[0]) == _TRUE)&&(is_num(p->fstring[1]) == _TRUE)&&
         (is_num(p->fstring[3]) == _TRUE)&&(is_num(p->fstring[4]) == _TRUE))
      {
        retcode = _TRUE;
      }
    }
  }

  return(retcode);
}

_BOOL check_date_separator(_CHREDIT *p, _UDWORD sc, _UDWORD format)
{
  _BOOL retcode = _FALSE;
  _UDWORD p1, p2, i; 
  _ASCII chr;
 
  if((sc >= 0x30)&&(sc <= 0x39))
  {
    p1  = 0xFFFFAABC; 
    p2  = 0xFFFFAABC;
    chr = 0;
    
    switch(format)
    {
      case SIZE_WWDDMM     :    
      case SIZE_DDMM       : p1 = 2; 
                             chr = '.';
                             break;

      case SIZE_WWDDMMYY   :    
      case SIZE_DDMMYY     : p1 = 2;
                             p2 = 5;
                             chr = '.';
                             break;

      case SIZE_WWDDMMYYYY : 
      case SIZE_DDMMYYYY   : p1 = 2;
                             p2 = 5;
                             chr = '.';
                             break;

      case SIZE_YYYYMMDD   : p1 = 4;
                             p2 = 7; 
                             chr = '-';
                             break;
                             
      case SIZE_YYMMDD     : p1 = 2;
                             p2 = 5;
                             chr = '-';
                             break;
                             
      case SIZE_YYMM       : p1 = 2;
                             chr = '-';
                             break;
    }
  
    if((chr != 0) && ((p->pos == p1) || (p->pos == p2)))
    {
      for(i=0; i<p->pos; i++)
      {
        if((i != p1) && (i != p2))
        {
          if(is_num(p->fstring[i]) == _FALSE)
            chr = 0;
        }
      }
      
      if(chr != 0)
      {
        if(chr == '.')
          chr = (get_americandateformat() == _TRUE)? '/':'.';
        retcode = run_CHREDIT(p, chr, _FALSE, _FALSE, _TRUE);
      }
    }
  }

  return retcode;
}


_BOOL Emboly(_UDWORD year)
{
  _BOOL retcode = _FALSE;

  if((year % 4) == 0)
  {
    retcode = _TRUE;
    if((year % 100) == 0)
    {
      retcode = _FALSE;
      if((year % 400) == 0)
      {
        retcode = _TRUE;
      }
    }
  }

  return(retcode);
}

_UWORD GetDayOfMonth(_UDWORD month, _UDWORD year)
{
  _UWORD retcode = 30;

  switch(month)
  {
    case  1:
    case  3:
    case  5:
    case  7:
    case  8:
    case 10:
    case 12: retcode = 31;
             break;
    case  2: retcode = (Emboly(year) == _TRUE)? 29 : 28;
             break;
  }

  return(retcode);
}

void incdec_date_intern(_DATE *dat, _DWORD val, _UDWORD format)
{
  _UDWORD rep;

  if(dat->day == 0xFF)
    dat->day = 1;
  if(dat->month == 0xFF)
    dat->month = 1;
  if(dat->year == 0xFFFF)
    dat->year = 2011;

  if(val >= 0) // INC
  {
    rep = val;
    
    if(format == SIZE_YYMM)
    {
      while(rep)
      {
        rep -= 1;
        dat->month += 1;
        if(dat->month > 12)
        {
          dat->month = 1;
          dat->year += 1;
        }
      }
    }
    else
    {
      while(rep)
      {
        rep -= 1;
        if(dat->day >= GetDayOfMonth(dat->month, dat->year))
        {
          dat->day    = 1;
          dat->month += 1;
          if(dat->month > 12)
          {
            dat->month  = 1;
            dat->year  += 1;
          }
        }
        else
        {
          dat->day += 1;
        }
      }
    }
  }
  else // DEC
  {
    rep = -val;
    if(format == SIZE_YYMM)
    {
      while(rep)
      {
        rep -= 1;
        if(dat->month <= 1)
        {
          dat->month = 12;
          dat->year -= 1;
        }
        else
        {
          dat->month -= 1;
        }
      }
    }
    else
    {
      while(rep)
      {
        rep -= 1;
        if(dat->day <= 1)
        {
          if(dat->month <= 1)
          {
            dat->month  = 12;
            dat->year  -= 1;
          }
          else
          {
            dat->month -= 1;
          }
          dat->day = GetDayOfMonth(dat->month, dat->year);
        }
        else
        {
          dat->day -= 1;
        }
      }
    }
  }
}

void incdec_date(_DATE *dat, _DWORD val)
{
  incdec_date_intern(dat, val, SIZE_DDMMYYYY);
}


_BOOL _Picture::EditDate(_UWORD sc, _BOOL is_character)
{
  _BOOL retcode = _FALSE;
  _BYTE op;
  _DATE tmp_date;

  Pic.Editor.chredit.timer_cursorjump = 0;

  switch(sc)
  {
    case _DIRECTIONENTER :
    case _ENTER  :
      if(SetUp.GetUseShuttleForEdit() == _TRUE)
      {
        if(Pic.Editor.chredit.change == _FALSE)
        {
          Pic.Editor.chredit.editshuttle = _TRUE; // shuttleeingabe aktiviert
          Pic.Editor.chredit.change = _TRUE;
          Pic.Editor.chredit.chr = 0;
          Pic.Editor.variable.info.format = SIZE_DDMMYY;
          format_date(Pic.Editor.chredit.fstring, &Pic.Editor.result.datim.fdate, Pic.Editor.variable.info.format);
          Pic.Editor.chredit.pos = 0;
          retcode = _TRUE;
        }
//        else if(Pic.Editor.chredit.editshuttle == _TRUE)
//        {
//          if(Pic.Editor.chredit.chr > 1)
//            SaveEditor(_FALSE);
//          else
//            Pic.Editor.chredit.chr++;
//          if(Pic.Editor.chredit.chr == 1)
//            Pic.Editor.chredit.pos = 3;
//          else if(Pic.Editor.chredit.chr == 2)
//            Pic.Editor.chredit.pos = 6;
//          retcode = _TRUE;
//        }
      }

      if(retcode == _FALSE)
      {
        if(string_to_date(&Pic.Editor.result.datim.fdate, Pic.Editor.chredit.fstring, Pic.Editor.variable.info.format) == _TRUE)
        {
          retcode = SaveEditor(_FALSE);
        }
        else
        {
          SystemError(INVALID_DATE);
         #ifdef USER_BILLION
          retcode = call_billionlimit(&Pic.Editor, &Pic.Editor.result, _TRUE);
         #endif
        }
      }
      break;

    case _PGUP   :
    case _PGDOWN :
      if(SetUp.GetEditorIncDec() == _TRUE)
      {
        if(Pic.Editor.chredit.change == _FALSE)
        {
          incdec_date_intern(&Pic.Editor.result.datim.fdate, (sc == _PGUP)? 1:-1, Pic.Editor.variable.info.format);
          SaveEdit();
        }
      }
      else if(Pic.Editor.chredit.incdec == _TRUE)
      {
        if(Pic.Editor.chredit.change == _FALSE)
        {
          format_date(Pic.Editor.chredit.fstring, &Pic.Editor.result.datim.fdate, Pic.Editor.variable.info.format);
          Pic.Editor.chredit.change = _TRUE;
        }
        string_to_date(&tmp_date, Pic.Editor.chredit.fstring, Pic.Editor.variable.info.format);
        incdec_date_intern(&tmp_date, (sc==_PGUP)? 1:-1, Pic.Editor.variable.info.format);
        format_date(Pic.Editor.chredit.fstring, &tmp_date, Pic.Editor.variable.info.format);
        retcode = _TRUE;
      }
      break;

    default      :
      if((is_character == _FALSE)&&((sc == _PGRIGHT)||(sc == _PGLEFT)))
      {
        if(SetUp.GetUseShuttleForEdit() == _TRUE)
        {
          Pic.Editor.chredit.timer_cursorjump = get_abstime();
          if(Pic.Editor.chredit.editshuttle == _FALSE)
          {
            // wechsel von tasteneingabe auf shuttleeingabe
            Pic.Editor.chredit.editshuttle  = _TRUE;
            Pic.Editor.chredit.chr          = 0;
            Pic.Editor.chredit.pos          = 0;
            Pic.Editor.variable.info.format = SIZE_DDMMYY;
          }

          op = -1;
          if(sc == _PGRIGHT)
            op = 1;

          if(Pic.Editor.chredit.chr == 0)
            Pic.Editor.result.datim.fdate.day = astrict(Pic.Editor.result.datim.fdate.day, op, 1, 31, _TRUE);
          else if(Pic.Editor.chredit.chr == 1)
            Pic.Editor.result.datim.fdate.month = astrict(Pic.Editor.result.datim.fdate.month, op, 1, 12, _TRUE);
          else if(Pic.Editor.chredit.chr == 2)
            Pic.Editor.result.datim.fdate.year = astrict(Pic.Editor.result.datim.fdate.year, op, 0, 99, _TRUE);

          format_date(Pic.Editor.chredit.fstring, &Pic.Editor.result.datim.fdate, Pic.Editor.variable.info.format);
          retcode = _TRUE;
        }
      }
      else
      {
      
       #ifdef EDIT_DEFAULT_TIMEDATE
        if(sc == EDIT_DEFAULT_TIMEDATE)
        {
          if((Pic.Editor.chredit.change == _TRUE) && (Pic.Editor.chredit.no > 0))
          {
            if(Pic.Editor.chredit.fstring[0] != EDIT_DEFAULT_TIMEDATE)
            {
              Pic.Editor.chredit.no = 0;
              Pic.Editor.chredit.pos = 0;
              Pic.Editor.chredit.fstring[0] = 0;
            }
          }
        }
       #endif
      
        if(Pic.Editor.chredit.editshuttle == _TRUE)
        {
          // wechsel von shuttleeingabe auf tasteneingabe
          Pic.Editor.chredit.editshuttle = _FALSE;
          Pic.Editor.chredit.chr         = 0;
          Pic.Editor.chredit.pos         = 0;
        }

        retcode = run_CHREDIT(&Pic.Editor.chredit, sc, _FALSE, _FALSE, is_character);
        if(check_date_separator(&Pic.Editor.chredit, sc, Pic.Editor.variable.info.format) == _TRUE)
          retcode = _TRUE;

        if(retcode == _TRUE)
          Pic.Editor.chredit.incdec = _FALSE; // kein manuelles inc/dec mehr möglich
      }
      break;
  }

  return(retcode);
}

_BOOL insert_apm(_CHREDIT *p, _UWORD sc)
{
  _BOOL  retcode = _FALSE;
  _UWORD i;

  if((p->change == _TRUE)&&(get_americantimeformat() == _TRUE))
  {
    if((sc == '-')||(sc == '+'))
    {
      for(i=0; i<p->no; i++)
      {
        switch(p->fstring[i])
        {
          case 'a': p->fstring[i] = 'p'; retcode = _TRUE; break;
          case 'A': p->fstring[i] = 'P'; retcode = _TRUE; break;
          case 'p': p->fstring[i] = 'a'; retcode = _TRUE; break;
          case 'P': p->fstring[i] = 'A'; retcode = _TRUE; break;
        }
      }
    }
    else if((sc == 'a')||(sc == 'A')||(sc == 'p')||(sc == 'P'))
    {
      for(i=0; i<p->no; i++)
      {
        switch(p->fstring[i])
        {
          case 'a':
          case 'A':
          case 'p':
          case 'P': p->fstring[i] = sc; retcode = _TRUE; break;
        }
      }
    }
    else if((sc == 'm')||(sc == 'M'))
    {
      for(i=0; i<p->no; i++)
      {
        switch(p->fstring[i])
        {
          case 'm':
          case 'M': p->fstring[i] = sc; retcode = _TRUE; break;
        }
      }
    }
  }

  return(retcode);
}

void incdec_time(_TIME *tim, _DWORD val, _UWORD format)
{
  _UDWORD abstim;

  if(format == SIZE_HHMM)
    val = val * 60;

 #ifdef USER_DEKEMA
  if(format == SIZE_MMSS)
  {
    abstim      = (100*60) + ((tim->minute * 60) + tim->second) + val;
    tim->second = abstim % 60;
    abstim      = abstim / 60;
    tim->minute = abstim % 100;
  }
  else
 #endif
  {
    if(tim->hour == 0xFF)
      tim->hour = 0;
    if(tim->minute == 0xFF)
      tim->minute = 0;
    if(tim->second == 0xFF)
      tim->second = 0;
  
    abstim      = (60*60*24) + ((((tim->hour * 60) + tim->minute) * 60) + tim->second) + val;
    tim->second = abstim % 60;
    abstim      = abstim / 60;
    tim->minute = abstim % 60;
    abstim      = abstim / 60;
    if(format != 2)
      tim->hour = abstim % 24;
  }
}

_BOOL _Picture::EditTime(_UWORD sc, _BOOL is_character)
{
  _BOOL retcode = _FALSE;
  _BYTE op;
//  _BOOL sst;
  _TIME tmp_time;

  Pic.Editor.chredit.timer_cursorjump = 0;

  switch(sc)
  {
    case _DIRECTIONENTER :
    case _ENTER  :
      if(SetUp.GetUseShuttleForEdit() == _TRUE)
      {
        if(Pic.Editor.chredit.change == _FALSE)
        {
          Pic.Editor.chredit.editshuttle = _TRUE; // shuttleeingabe aktiviert
          Pic.Editor.chredit.change = _TRUE;
          Pic.Editor.chredit.chr = 0;
          format_time(Pic.Editor.chredit.fstring, &Pic.Editor.result.datim.ftime, Pic.Editor.variable.info.format);
          Pic.Editor.chredit.pos = 0;
          retcode = _TRUE;
        }
      }

      if(retcode == _FALSE)
      {
        if(string_to_time(&Pic.Editor.result.datim.ftime, Pic.Editor.chredit.fstring, Pic.Editor.variable.info.format) == _TRUE)
        {
          retcode = SaveEditor(_FALSE);
        }
        else
        {
          SystemError(INVALID_TIME);
         #ifdef USER_BILLION
          retcode = call_billionlimit(&Pic.Editor, &Pic.Editor.result, _TRUE);
         #endif
        }
      }
      break;

    case _PGUP   :
    case _PGDOWN :
      if(SetUp.GetEditorIncDec() == _TRUE)
      {
        if(Pic.Editor.chredit.change == _FALSE)
        {
          incdec_time(&Pic.Editor.result.datim.ftime, (sc==_PGUP)? 1:-1, Pic.Editor.variable.info.format);
          SaveEdit();
        }
      }
      else if(Pic.Editor.chredit.incdec == _TRUE)
      {
        if(Pic.Editor.chredit.change == _FALSE)
        {
          format_time(Pic.Editor.chredit.fstring, &Pic.Editor.result.datim.ftime, Pic.Editor.variable.info.format);
          Pic.Editor.chredit.change = _TRUE;
        }
        string_to_time(&tmp_time, Pic.Editor.chredit.fstring, Pic.Editor.variable.info.format);
        incdec_time(&tmp_time, (sc==_PGUP)? 1:-1, Pic.Editor.variable.info.format);
        format_time(Pic.Editor.chredit.fstring, &tmp_time, Pic.Editor.variable.info.format);
        retcode = _TRUE;
      }
      break;

    default      :
      if((is_character == _FALSE)&&((sc == _PGRIGHT)||(sc == _PGLEFT)))
      {
        if(SetUp.GetUseShuttleForEdit() == _TRUE)
        {
          Pic.Editor.chredit.timer_cursorjump = get_abstime();
          if(Pic.Editor.chredit.editshuttle == _FALSE)
          {
            // wechsel von tasteneingabe auf shuttleeingabe
            Pic.Editor.chredit.editshuttle = _TRUE;
            Pic.Editor.chredit.chr         = 0;
            Pic.Editor.chredit.pos         = 0;
          }

          op = -1;
          if(sc == _PGRIGHT)
            op = 1;

          if(Pic.Editor.chredit.chr == 0)
          {
            Pic.Editor.result.datim.ftime.hour = astrict(Pic.Editor.result.datim.ftime.hour, op, 0, 23, _TRUE);
          }
          else if(Pic.Editor.chredit.chr == 1)
          {
            Pic.Editor.result.datim.ftime.minute = astrict(Pic.Editor.result.datim.ftime.minute, op, 0, 59, _TRUE);
          }
          else if(Pic.Editor.chredit.chr == 2)
          {
            Pic.Editor.result.datim.ftime.second = astrict(Pic.Editor.result.datim.ftime.second, op, 0, 59, _TRUE);
          }

          format_time(Pic.Editor.chredit.fstring, &Pic.Editor.result.datim.ftime, Pic.Editor.variable.info.format);
          retcode = _TRUE;
        }
      }
      else
      {
        if(Pic.Editor.chredit.editshuttle == _TRUE)
        {
          // wechsel von shuttleeingabe auf tasteneingabe
          Pic.Editor.chredit.editshuttle = _FALSE;
          Pic.Editor.chredit.chr         = 0;
          Pic.Editor.chredit.pos         = 0;
        }

       #ifdef EDIT_DEFAULT_TIMEDATE
        if(sc == EDIT_DEFAULT_TIMEDATE)
        {
          if((Pic.Editor.chredit.change == _TRUE) && (Pic.Editor.chredit.no > 0))
          {
            if(Pic.Editor.chredit.fstring[0] != EDIT_DEFAULT_TIMEDATE)
            {
              Pic.Editor.chredit.no = 0;
              Pic.Editor.chredit.pos = 0;
              Pic.Editor.chredit.fstring[0] = 0;
            }
          }
        }
       #endif

        if((sc == '-')||(sc == '+'))
        {
          if(Pic.Editor.chredit.no > 0)
          {
            //sc = 'a';
            if(insert_apm(&Pic.Editor.chredit, '-') == _TRUE)
              return(_TRUE);
          }
        }
        else if((sc == 'a')||(sc == 'A')||(sc == 'p')||(sc == 'P')||(sc == 'm')||(sc == 'M'))
        {
          if(insert_apm(&Pic.Editor.chredit, sc) == _TRUE)
            return(_TRUE);
        }
        
        retcode = run_CHREDIT(&Pic.Editor.chredit, sc, _FALSE, _FALSE, is_character);

        // automatisches einfügen separator
//        sst = _FALSE;
//        if(Pic.Editor.variable.info.format == SIZE_HHMMSS)
//          sst = _TRUE;
        if(check_time_separator(&Pic.Editor.chredit, sc, Pic.Editor.variable.info.format) == _TRUE)
          retcode = run_CHREDIT(&Pic.Editor.chredit, ':', _FALSE, _FALSE, _TRUE);

        if(retcode == _TRUE)
          Pic.Editor.chredit.incdec = _FALSE; // kein manuelles inc/dec mehr möglich
      }
      break;
  }

  return(retcode);
}

_UDWORD incdec_ipaddress(_UDWORD val, _DWORD incdec)
{
  _UDWORD tmp = val;
  tmp = (tmp >> 24) & 0xFF;
  tmp = (tmp + incdec) & 0xFF;
  
  return ((tmp << 24) | (val & 0x00FFFFFF));
}

_BOOL _Picture::EditIpAddress(_UWORD sc, _BOOL is_character)
{
  _BOOL retcode = _FALSE;
  _UDWORD tmp;

  //Pic.Editor.chredit.timer_cursorjump = 0;

  Pic.Editor.chredit.max = 15;

  switch(sc)
  {
    case _DIRECTIONENTER :
    case _ENTER  :
      Pic.Editor.result.value = string_to_ipaddress(Pic.Editor.chredit.fstring);
      retcode = SaveEditor(_FALSE);
      break;

    case _PGUP   :
    case _PGDOWN :
      if(SetUp.GetEditorIncDec() == _TRUE)
      {
        if(Pic.Editor.chredit.change == _FALSE)
        {
          Pic.Editor.result.value = incdec_ipaddress(Pic.Editor.result.value, (sc==_PGUP)? 1:-1);
          SaveEdit();
        }
      }
      else if(Pic.Editor.chredit.incdec == _TRUE)
      {
        if(Pic.Editor.chredit.change == _FALSE)
        {
          format_ipaddress(Pic.Editor.chredit.fstring, Pic.Editor.result.value);
          Pic.Editor.chredit.change = _TRUE;
        }
        tmp = string_to_ipaddress(Pic.Editor.chredit.fstring);
        tmp = incdec_ipaddress(tmp, (sc==_PGUP)? 1:-1);
        format_ipaddress(Pic.Editor.chredit.fstring, tmp);
        retcode = _TRUE;
      }
      break;

    default      :
      retcode = run_CHREDIT(&Pic.Editor.chredit, sc, _FALSE, _FALSE, _TRUE);
      if(retcode == _TRUE)
        Pic.Editor.chredit.incdec = _FALSE; // kein manuelles inc/dec mehr möglich
      break;
  }

  return(retcode);
}




#ifdef OK_ENCLOSE_NAME
_DOIT _Picture::GetEventEnclosedName(_EVENT *pe)
{
  if(pe->ftype != _EVENT_NONE)
  {
    if(Pic.Editor.input.io.virtname.pthis != NULL)
    {
      if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
      {
        return call_enclosed_name_method(METHOD_MYIO_GETEVENT, &Pic.Editor.input.io, &Pic.Editor.variable, &Pic.Editor.result, &Pic.Editor, pe, _TRUE);
      }
    }
  }

  return _IDLE;
}
#endif

_BOOL _Picture::CheckForClose(void)
{
  _BOOL retcode = _FALSE;

 #ifdef OK_SEEK_BUTTON
  if((Pic.Editor.input.io.typ == AS_INPUTBUTTON_WITOUT_CURSOR)||(Pic.Editor.input.io.typ == AS_INPUTBUTTON))
  {
    if(access_SEEKELEMENT(&Pic, Pic.Editor.actno, _TRUE, _SELECTOR_DEFAULT) == _FALSE)
      return(_TRUE);
  }
  else
 #endif

  // userlevel überprüfen
  if(chk_ACCESS(Pic.Editor.variable.info.accessno, _FALSE) == _FALSE)
  {
    retcode = _TRUE;
  }
  // stateschema bei server überprüfen
  else if(VarList.GetStateScheme(Pic.Editor.variable.no, NOCARE_DEFAULT) != STATE_ACTIVE)
  {
    retcode = _TRUE;
  }
 #ifdef SIGMATEK_STANDARD
  else
  {
    _UDWORD xtmp;
    // stateschema bei io überprüfen
    if(StateScheme.Call(&xtmp, &Pic.Editor.input.io.stscheme, NULL, STATE_ACTIVE) == _FALSE)
      xtmp = STATESCHEME_PRESELECTION;
    if(xtmp != STATE_ACTIVE)
      retcode = _TRUE;
  }
 #endif

  return(retcode);
}

#ifdef SHOW_HTMLKEYWORD
void show_html_keyword(_EDITOR *pe)
{
  _UBYTE obj;
  _ASCII txt[200];

  switch(pe->input.io.typ)
  {
    case AS_THEURGE      :
    case AS_NUMERIC_HIDE :
    case AS_NUMERIC      :
    case AS_ASCII_HIDE   :
    case AS_ASCII        :
    case AS_CHKBOX       :
    case AS_RADIO        :
    case AS_DATE         :
    case AS_TIME         :
    case AS_ENUMERATION  :
    case AS_MENU         : NumToStr(txt, pe->variable.info.station, sizeof(_ASCII));
                           StrCat(txt, sizeof(_ASCII), " ", sizeof(_ASCII));
                           VarList.GetLabel(&txt[StrLen(txt, sizeof(_ASCII))], &obj, pe->variable.no);
                           break;
    default              : StrCpy(txt, sizeof(_ASCII), "???", sizeof(_ASCII));
  }

  StrCat(txt, sizeof(_ASCII), "    ", sizeof(_ASCII));
  Debug(txt, sizeof(_ASCII), 2);
}
#endif

_BOOL _Picture::RunEditor(_Window *PWin, _EVENT *pe, _BOOL hid)
{
  _BOOL  retcode = _FALSE; // true editor ist aktiv, false editor ist inaktiv
  _BOOL  oldchange;
  _EVENT ete;

//  if(access_input(&Pic.Editor.input) == _TRUE)
  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
    _BOOL show = _FALSE;
    retcode    = _TRUE;
    
   #ifdef SHOW_HTMLKEYWORD
    if((pe->ftype == _EVENT_KEYPRESS)&&(pe->scancode == '?')&&(pe->is_character == _TRUE))
      show_html_keyword(&Pic.Editor);
   #endif

    if(CheckForClose() == _FALSE)
    {
     #ifndef THR
     #ifndef USER_BILLION
      if(Pic.Editor.input.io.bRefreshAct == _FALSE)
     #endif
     #endif
      {
        SetLrm(pe->lrm);

        if(ask_MENCURSOR(&Pic.Editor.mencursor) == _TRUE)
          Cursor.Set(&Pic.Editor.input.io, _EDITACTIVE);

#ifdef UC_JAPANINPUT
        if(Pic.Editor.input.io.typ == AS_ASCII)
        {
//          if(Pic.Editor.chredit.japan_change != 0)
//          {
//            if(PWin == Root.FindTop())
//            {
//              Pic.Editor.chredit.japan_change = 0;
//              ShowEditor();
//            }
//          }
//        
          _DWORD jval = VarList.VarDummyGet(VARNO_JAPANESE_INPUT);
          if(Pic.Editor.japan_input_mode != jval)
          {
            Pic.Editor.japan_input_mode = jval;
            
            if(jval > 0)
            {
              if(Pic.Editor.chredit.change == _TRUE)
              {
                _EVENT te;
                init_EVENT(&te);
                te.ftype    = _EVENT_KEYPRESS;
                te.scancode = _JAPAN_KEYCODE;
                EditAscii(&te);
                ShowEditor();
              }
            }
          }
        }
#endif        
        if(pe->ftype == _EVENT_KEYPRESS)
        {
          Pic.Editor.enterdirection = _TRUE;
          if(pe->scancode == _DIRECTIONENTER)
          {
            DirectionOnEnter(_EP_NONE);
          }
          else if(pe->scancode != _NOKEY)
          {
            oldchange = Pic.Editor.chredit.change;

            if((pe->scancode == _ESC) && (Pic.Editor.chredit.change == _TRUE))
            {
              Cursor.End();
              Pic.Editor.chredit.change     = _FALSE;
              Pic.Editor.chredit.pos        = 0;
              Pic.Editor.chredit.no         = 0;
              Pic.Editor.chredit.fstring[0] = 0;
              show         = _TRUE;
              init_EVENT(pe);
            }
            else
            {

              switch(Pic.Editor.input.io.typ)
              {
                case AS_THEURGE:
                case AS_NUMERIC_HIDE :
                case AS_NUMERIC : show = EditNumeric(pe->scancode, 1, Pic.Editor.input.io.typ, pe->is_character); break;
                case AS_IPADDRESS: show = EditIpAddress(pe->scancode, pe->is_character); break;
                case AS_ASCII_HIDE :
                case AS_ASCII   : show = EditAscii(pe); break;
                case AS_CHKBOX  : show = EditChkBox(pe->scancode); break;
                case AS_RADIO   : show = EditRadio(pe->scancode);   break;
                case AS_DATE    : show = EditDate(pe->scancode, pe->is_character);    break;
                case AS_TIME    : show = EditTime(pe->scancode, pe->is_character);    break;
                case AS_ENUMERATION :
                case AS_MENU    : show = EditMenu(pe->scancode, Pic.Editor.input.io.info.tbo.no, hid); break;
//                case AS_VARNAME : show = EditMenu(pe->scancode, UC_LSLID_POPUP, hid); break;
               #ifdef OK_SEEK_BUTTON
                case AS_INPUTBUTTON_WITOUT_CURSOR :
                case AS_INPUTBUTTON : EditButton(pe->scancode, _TRUE, pe->lrm);  break;
               #endif
                default         : break;
  //            case AS_BAR     :    // balken
  //            case AS_TBO     :    // text, bitmap or object
  //            case AS_USER    :
              }
            }

            Pic.Editor.timer = get_abstime();

            if(oldchange != Pic.Editor.chredit.change)
            {
              if(oldchange == _FALSE)
              {
                free_MENCURSOR(&Pic.Editor.mencursor);
                Pic.Editor.input.io.cstate.value += 1; // sa8183: wenn editorinhalt geändert wurde, muss die crc geändert werden
              }
              Cursor.End(); // bei erstmaliger editoränderung wird grosser cursor (input selected) ausgeschaltet
            }

            if(show == _TRUE)
            {
              ShowEditor();
            }
          }
         #ifdef _WIN_SERVERHELP
          if((pe->scancode == _KEY_SERVERHELP)&&(pe->is_character == _TRUE))
          {
            Root.FindTop()->AddCommand(CMD_SERVERHELP, &Pic.Editor.input.io.variable, NULL, _FALSE);
          }
         #endif
        }
        else if((pe->ftype == _EVENT_SHUTTLEINC)||(pe->ftype == _EVENT_SHUTTLEDEC))
        {
          switch(Pic.Editor.input.io.typ)
          {
            case AS_THEURGE :
            case AS_NUMERIC : if(EditNumeric((pe->ftype == _EVENT_SHUTTLEINC)? _PGRIGHT : _PGLEFT, pe->scancode, AS_NUMERIC, _FALSE) == _TRUE)
                                ShowEditor();
                              break;
            case AS_ASCII   : ete          = *pe;
                              ete.scancode = (ete.ftype == _EVENT_SHUTTLEINC)? _PGRIGHT : _PGLEFT;
                              if(EditAscii(&ete) == _TRUE)
                                ShowEditor();
                              break;
            case AS_DATE    : if(EditDate((pe->ftype == _EVENT_SHUTTLEINC)? _PGRIGHT : _PGLEFT, _FALSE) == _TRUE)
                                ShowEditor();
                              break;
            case AS_TIME    : if(EditTime((pe->ftype == _EVENT_SHUTTLEINC)? _PGRIGHT : _PGLEFT, _FALSE) == _TRUE)
                                ShowEditor();
                              break;
            default         : break;
          }
        }
        else if(pe->ftype == _EVENT_KEYRELEASE)
        {
          if((pe->scancode == _ENTER)||((pe->scancode == CPYEDIT)&&(Pic.Editor.input.io.typ == AS_NUMERIC)))
          {
           #ifdef OK_SEEK_BUTTON
            if((Pic.Editor.input.io.typ == AS_INPUTBUTTON)||(Pic.Editor.input.io.typ == AS_INPUTBUTTON_WITOUT_CURSOR))
              EditButton(_ENTER, _FALSE, pe->lrm);
           #endif
            if(Pic.Editor.disable_direction_enter == _FALSE)
              DirectionOnEnter(_EP_NONE);
          }
        }
       #ifdef UC_BUBBLE_HELP
        else if(Pic.P_Window->GetP_Window() == NULL)
        {
          if(Pic.Editor.bubblehelp.xstate == BUBBLE_VIRGIN)
          {
            Pic.Editor.bubblehelp.xstate = BUBBLE_FINISHED;
            if(BubbleHelpOk(&Pic.Editor) == _TRUE)
              Pic.Editor.bubblehelp.xstate = BUBBLE_WATCH;
          }

          if(Pic.Editor.bubblehelp.xstate == BUBBLE_WATCH)
          {
            if(timediff(Pic.Editor.timer) > (_UDWORD)SetUp.Get_BUBBLEINFO()->delaytime)
            {
              Pic.P_Window->InsertCmd(CMD_OPENMINMAX, 0, 0, _FALSE);
            }
          }
          else if(Pic.Editor.bubblehelp.xstate == BUBBLE_ONSCREEN)
          {
           #ifdef USER_DEMAG  // SA 8142
            if(Pic.Editor.chredit.change == _FALSE)
           #endif
            {
              _UDWORD remaintime = SetUp.Get_BUBBLEINFO()->remaintime;
              if(remaintime > 0)
              {
                if(timediff(Pic.Editor.timer) > (SetUp.Get_BUBBLEINFO()->delaytime + remaintime))
                {
                  Pic.P_Window->InsertCmd(CMD_CLOSEMINMAX, 0, 0, _FALSE);
  //                Pic.Editor.bubblehelp.state = BUBBLE_FINISHED;
                }
              }
            }
          }
        }
       #endif
       #ifdef OK_ENCLOSE_NAME
        call_enclosed_name_method(METHOD_MYIO_SYSTEMISREADYWITHEDITOR, &Pic.Editor.input.io, &Pic.Editor.variable, &Pic.Editor.result, &Pic.Editor, pe, _TRUE);
       #endif

        if(Pic.Editor.chredit.timer_cursorjump != 0)
        {
          if(CursorJump() == _TRUE) // wird für shuttle eingabe benötigt (automatisches cursor weiterschalten)
            ShowEditor();
        }
      }
    }
    else
    {
      if(Root.FindTop() == PWin)
      {
        // es ist die obertet fensterebene
        AddCmd(CMD_KILLEDITOR, 0, 0);
       #ifdef FIRSTINPUT_ALT 
        AddCmd(CMD_AUTOSEEKINPUT, 0, 0);
       #endif // FIRSTINPUT_ALT 
      }
      else
      {
        // vorsicht es sind fenster darüber geöffnet
        PWin->InsertCmd(CMD_KILLEDITOR, 0, 0, _FALSE);      // add command
      }
      
      
//    _REGION actreg = GetActiveRegion();
//    ActivateRegion(Region);
//    CloseEditor();
//    DeactivateRegion();
//    ActivateRegion(actreg);
      
      
      
    }
  }

  return(retcode);
}

_BOOL _Picture::CursorJump(void)
{
  // wird für shuttle eingabe benötigt (automatisches cursor weiterschalten)
  _BOOL  retcode = _FALSE;
  _EVENT tmp;

  if(timediff(Pic.Editor.chredit.timer_cursorjump) > DELAY_CURSORJUMP)
  {
    Pic.Editor.chredit.timer_cursorjump = 0;
    if(Pic.Editor.input.io.typ == AS_ASCII)
    {
      init_EVENT_Makro(&tmp);
      tmp.ftype        = _EVENT_KEYPRESS;
      tmp.is_character = _TRUE;
      tmp.scancode     = Pic.Editor.chredit.fstring[Pic.Editor.chredit.pos];
      if(EditAscii(&tmp) == _TRUE)
        ShowEditor();
    }
    else if(Pic.Editor.input.io.typ == AS_DATE)
    {
      if(Pic.Editor.chredit.editshuttle == _TRUE)
      {
        Pic.Editor.chredit.chr++;
        if(Pic.Editor.chredit.chr >= 3)
          Pic.Editor.chredit.chr = 0;

        Pic.Editor.chredit.pos = Pic.Editor.chredit.chr * 3;
        retcode = _TRUE;
      }
    }
    else if(Pic.Editor.input.io.typ == AS_TIME)
    {
      if(Pic.Editor.chredit.editshuttle == _TRUE)
      {
        Pic.Editor.chredit.chr++;
        if(Pic.Editor.chredit.chr >= ((Pic.Editor.variable.info.format == SIZE_HHMMSS)? 3 : 2))
          Pic.Editor.chredit.chr = 0;

        Pic.Editor.chredit.pos = Pic.Editor.chredit.chr * 3;
        retcode = _TRUE;
      }
    }
  }

  return(retcode);
}

#ifdef UC_BUBBLE_HELP

_BOOL _Picture::BubbleHelpOk(_EDITOR *pe)
{
  _BOOL retcode = _FALSE;
  _CHAR *bht;

  switch(pe->input.io.typ)
  {
    //case AS_IPADDRESS    :
    case AS_THEURGE      :
    case AS_CHKBOX       :
    case AS_RADIO        :
    case AS_ASCII        :
    case AS_NUMERIC      :
    case AS_NUMERIC_HIDE :
    case AS_TIME         :
    case AS_DATE         : bht = get_ANYPARA(&pe->variable.info.text4, NULL);
                             
                           if(bht != NULL)
                           {
                             if(bht[0] != 0)
                               retcode = _TRUE;
                           }
                           if((retcode == _FALSE)&&(pe->input.io.typ == AS_NUMERIC)&&(pe->variable.info.statistic.parameter & PARAMETER_EXTENDED_BUBBLEHELP))
                           {
                             retcode = _TRUE;
                           }
                           break;
    default              : break;
  }

 #ifdef UC_TOUCH                          // open touch editor on input

 #ifndef UC_TOUCHBUBBLE_IN_EDITWINDOW
  _WHOAMI  whoami;
  if((SetUp.GetUseTouchEditor() == _TRUE)&&(retcode == _TRUE))
  {
    if(Pic.P_Window != NULL)
    {
      whoami = Pic.P_Window->GetWhoAmI();
      if(whoami.ima == IMA_TOUCHEDITOR)
        retcode = _FALSE;
    }
  }
 #endif
 #endif

  return(retcode);
}

_BOOL size_minmax(_ROOM *pr, _ROOM *pe, _FONT font, _CHAR *txt, _FRAMETYPE frametype, _ATTRIB attrib)
{
  _ROOM  screen, room;
  _UWORD dx;
  _UWORD dy;
  _BOOL  retcode = _FALSE;

  if(SizeMultiTextAlign(&dx, &dy, txt, font, attrib, sizeof(_CHAR)) == _TRUE)
  {
    room.xy1.x = 1;
    room.xy1.y = 1;
    room.xy2.x = 300;
    room.xy2.y = 300;
    GetTextRoom(&screen, &room, attrib, frametype, _TRUE);
    dx += 2 + (300 - (screen.xy2.x - screen.xy1.x + 1));
    dy += 2 + (300 - (screen.xy2.y - screen.xy1.y + 1));
//    dx += 2 + (2 * (get_framewidth(frametype, attrib) + SetUp.GetFrameSpace()));
//    dy += 2 + (2 * (get_frameheight(frametype, attrib) + SetUp.GetFrameSpace()));

    MyGetScreenSize(&screen);

    if(pe->xy1.y > (screen.xy1.y + dy + 2))
      pr->xy1.y = pe->xy1.y - dy - 2;
    else
      pr->xy1.y = pe->xy2.y + 2;

    if(pe->xy1.x < (screen.xy2.x - dx + 2))
      pr->xy1.x = pe->xy1.x;
    else
      pr->xy1.x = screen.xy2.x - dx - 2;

    pr->xy2.x = pr->xy1.x + dx;
    pr->xy2.y = pr->xy1.y + dy;

    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL _Picture::CloseBubbleHelp(void)
{
  _BOOL retcode = _FALSE;

  if(Pic.Editor.bubblehelp.xstate == BUBBLE_ONSCREEN)
  {
    DrawBackGround ++;
    Pic.Editor.bubblehelp.xstate = BUBBLE_FINISHED;
    Root.RedrawScreen(&Pic.Editor.bubblehelp.room);
    retcode = _TRUE;
    DrawBackGround --;

   #ifdef UC_TOUCHBUBBLE_BY_SELECTION
    if(BubbleState == 2)
    {
      Pic.P_Window->AddCmd(CMD_KILLEDITOR, 0, 0, _FALSE);
      Init_BubbleState();
    }
   #endif
  }

  return(retcode);
}

_IVALUE calculate_bubble(_IVALUE value, _VARIABLE *pv)
{
  _RESULT result;
  _IVALUE retcode = value;

  if(pv->info.formula != DEFFORMULA)
  {
    init_RESULT_Makro(&result);
    result.rmax  =
    result.rmin  =
    result.value = value;
    if(recalculate(&result, pv, _TRUE) == _TRUE)
      retcode = result.value;
  }

  return(retcode);
}

void add_bubbletxt(_CHAR *text, _IVALUE value, const _ASCII *label, _CHAR *unit, _EDITOR *pe, _BOOL state)
{
  _UWORD len;

  if(state == _TRUE)
    StrCat(text, sizeof(_CHAR), "|", sizeof(_ASCII));

  StrCat(text, sizeof(_CHAR), label, sizeof(_ASCII));
  value = calculate_bubble(value, &pe->variable);
  format_value(&text[StrLen(text, sizeof(_CHAR))], value, &pe->variable.info, &pe->input.io.picformat);
  if(unit != NULL)
  {
    len = StrLen(text, sizeof(_CHAR));
    text[len] = ' ';
    StrCpy(&text[len+1], sizeof(_CHAR), unit, sizeof(_CHAR));
  }
}

_BOOL make_bubble_text(_CHAR *text, _EDITOR *pe)
{
  _ANYTHING anything;
  _BOOL     retcode = _FALSE;
  _CHAR     *bht, *unit;
  _RESULT   tmp;

  text[0] = 0;
  
  bht = get_ANYPARA(&pe->input.io.newbubbletext, NULL);
  if(bht == NULL)
    bht = get_ANYPARA(&pe->variable.info.text4, NULL);
    
  if(bht != NULL)
  {
    if(bht[0] != 0)
    {
      StrCpy(text, sizeof(_CHAR), bht, sizeof(_CHAR));
      retcode = _TRUE;
    }
  }

  if(pe->input.io.typ == AS_NUMERIC)
  {
    if(pe->variable.info.statistic.parameter & PARAMETER_EXTENDED_BUBBLEHELP)
    {
      init_ANYTHING_Makro(&anything);
      anything.no   = pe->variable.no;
      anything.list = VARUNIT;
      unit = get_text(&anything, NULL, NULL, _TRUE);
      free_ANYTHING(&anything);

      add_bubbletxt(text, pe->result.value, "act: ", unit, pe, retcode);

      tmp = pe->result;
      tmp.value = tmp.rmax;
      tmp.rmin  = tmp.rmax;
      recalculate(&tmp, &pe->variable, _TRUE);
      add_bubbletxt(text, tmp.value, "max: ", unit, pe, _TRUE);

      tmp = pe->result;
      tmp.value = tmp.rmin;
      tmp.rmax  = tmp.rmin;
      recalculate(&tmp, &pe->variable, _TRUE);
      add_bubbletxt(text, tmp.value, "min: ", unit, pe, _TRUE);

//      add_bubbletxt(text, pe->result.rmax, "max: ", unit, pe, _TRUE);
//      add_bubbletxt(text, pe->result.rmin, "min: ", unit, pe, _TRUE);
      retcode = _TRUE;
    }
  }

  return(retcode);
}

_BOOL _Picture::OpenBubbleHelp(void)
{
  _ROOM       room;
  _BOOL       retcode = _FALSE;
  _CHAR       *txt;
  _CHAR       label[400];
  _DOT        offset;
  _BUBBLEHELP *pb;
  _BOOL       state;

  if(Pic.P_Window->GetP_Window() == NULL)
  {
    if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
    {
      if((Pic.Editor.bubblehelp.xstate == BUBBLE_WATCH) && (BubbleHelpOk(&Pic.Editor) == _TRUE))
      {
        state = make_bubble_text(label, &Pic.Editor);
        txt   = label;

        offset = Pic.P_Window->GetPicOffset();
        if(position_SEEKELEMENT(&room, &Pic, Pic.Editor.actno, &offset, &Pic.PicZoom) == _TRUE)
        {
          pb         = &Pic.Editor.bubblehelp;
          pb->font   = SetUp.Get_BUBBLEINFO()->font;
          pb->attrib = SetUp.Get_BUBBLEINFO()->attrib;
          pb->frametype = SetUp.Get_BUBBLEINFO()->frametype;
          Font.Set(pb->font, &pb->attrib);
          if(size_minmax(&room, &room, pb->font, txt, pb->frametype, pb->attrib) == _TRUE)
          {
            pb->room      = room;
            pb->colback   = SetUp.GetBubbleColorBack();
            pb->colframe  = SetUp.GetBubbleColorFrame();
            pb->coltxt    = SetUp.GetBubbleColorTxt();
            pb->txt       = txt;

           #ifdef UC_MOUSE
            HideMouseCursor();
           #endif
            if(DrawBubbleHelp(&Pic.Editor) == _TRUE)                  // user interface
            {
              if(state == _TRUE)
              {
                OutMultiTextAlign(pb->txt, 0, &pb->room, pb->font, pb->attrib, pb->frametype, pb->coltxt, pb->colback, pb->colframe, sizeof(_CHAR));
               #ifdef UC_ROTATE
                RefreshRectangle(&pb->room);
               #endif
//                OutMultiTextAlign(pb->txt, 0, &pb->room, pb->font, T_COPY|T_SOLID|T_LEFTBOUND, pb->frametype, pb->coltxt, pb->colback, pb->colframe, sizeof(_CHAR));
                retcode = _TRUE;
                Pic.Editor.bubblehelp.xstate = BUBBLE_ONSCREEN;
              }
            }
            else
            {
              Pic.Editor.bubblehelp.xstate = BUBBLE_ONSCREEN;
            }
           #ifdef UC_MOUSE
            ShowMouseCursor();
           #endif

          }
        }
      }
    }
  }

  Pic.Editor.timer  = get_abstime();

  return(retcode);
}
#endif

// lasal interface **************************************************************

_BOOL ReOpenEditor(void)
{
  _DOT     offset;
  _Window  *P_Window;
  _Picture *P_Picture;
  _BOOL    retcode = _FALSE;

  init_DOT(&offset);

  P_Window  = Root.FindTop();
  P_Picture = P_Window->GetP_Picture();

  if(P_Picture != NULL)
  {
    retcode = P_Picture->UpdateEditor();
  }

  return(retcode);
}

/*
void _Picture::RefreshEditor(void)
{
  _BOOL   retcode = _FALSE;
  _DOT    offset;
  _ZOOM   zoom;

  if(IsEditorTrue(Pic.Editor.actno) == _TRUE)
  {
    if(Pic.Editor.chredit.change == _FALSE)
    {
      init_ZOOM(&zoom);
      init_DOT(&offset);
    }
  }
}
*/

_BOOL IsEditorActive(void)
{
  _BOOL    retcode = _FALSE;
  _WHOAMI  whoami;
  _Window  *Pw = Root.FindTop();
  _Picture *Pp;

  if(Pw != NULL)
  {
    whoami = Pw->GetWhoAmI();
    if((whoami.ima == IMA_TOUCHEDITOR)||(whoami.ima == IMA_POPUP))
    {
      retcode = _TRUE;
    }
    else if((whoami.ima == IMA_PICTURE)||(whoami.ima == IMA_WINDOW))
    {
      Pp = Pw->GetP_Picture();
      if(Pp != NULL)
      {
        retcode = Pp->IsEditorOpen();
      }
    }
  }

  return(retcode);
}
