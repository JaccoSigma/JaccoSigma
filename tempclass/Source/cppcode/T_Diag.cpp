#include "T_IPC.H"
#include "T_NUC.H"
#include "COMINTFC.H"

#define HEAP_DETAIL_INFO  // anzeige der farbblöcke am heap
#define DIAG_OVERHEAD  6
#define UC_GENERALINFO        60099                     // general info popup

_BOOL filexplo_call(_MENU*);


_CRC32 StatHeapCrc;
_ASCII ExplorerPath[_MAX_PATHLENGTH];

#ifdef OK_DIAGNOSTIC

_BOOL scheme_call(_MENU*, _Scheme*);
_BOOL textscheme_call(_MENU*, _TextScheme*);
_BOOL heap_call(_MENU*);
_BOOL varlist_call(_MENU*);
#ifdef UC_IPC
_BOOL varview_call(_MENU*, _UDWORD);
#endif
_BOOL idlist_call(_MENU*, _VARLABEL*);
_BOOL alarmlist_call(_MENU*);
_BOOL textlist_call(_MENU*, _TextPuffer*);
_BOOL textlist_fontname_call(_MENU*, _UWORD);
_BOOL textlist_dump_call(_MENU*, _CHAR*);
_BOOL projectlist_call(_MENU*);
_BOOL functionlist_call(_MENU*);
_BOOL imagelist_call(_MENU*);
#ifdef UC_IPC
_BOOL imageview_call(_MENU*, _UDWORD);
_BOOL imagefile_call(_MENU*, _ASCII*);
#endif
_BOOL unitlist_call(_MENU*);
_BOOL objectlist_call(_MENU*);
_BOOL font_call(_MENU*);
_BOOL language_call(_MENU*);
_BOOL alltextlist_call(_MENU*);
_BOOL fontview_call(_MENU*, _UWORD);
_BOOL menulist_call(_MENU*);
_BOOL menu_call(_MENU*, _UWORD);
_BOOL stack_call(_MENU*, _Stack*);
_BOOL sequencelist_call(_MENU*);
_BOOL colpalpopup_call(_MENU*);
_BOOL systeminfo_call(_MENU*);
_BOOL batch_call(_MENU*, const _ASCII*);
_BOOL bin_call(_MENU*, const _ASCII*);
_BOOL hotkeylist_call(_MENU*);
_BOOL setup_call(_MENU*);
#ifdef UC_THEME
_BOOL themelist_call(_MENU*);
#endif
#ifdef UC_IPC
_MEMSIZE MemSize;
_BOOL general_call(_MENU*);
_BOOL onlineserver_call(_MENU*);
#endif
_BOOL hostname_call(_MENU*, _NAME_INDEX*);
_BOOL online_call(_MENU*);


void  popup_language_run(_Window*, _PopUp*, _EVENT*);
void  popup_language_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_varinfo_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_varinfo_run(_Window*, _PopUp*, _EVENT*);
#ifdef UC_IPC
void  popup_varview_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
#endif
void  popup_alarminfo_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_alarm_run(_Window*, _PopUp*, _EVENT*);
void  popup_textlist_line(_Window *, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_textlist_run(_Window*, _PopUp*, _EVENT*);
void  popup_textlist_fontname_ready(_Window *, _PopUp*, _BOOL);
void  popup_seek_ready(_Window *, _PopUp*, _BOOL);
void  popup_textlist_fontname_run(_Window*, _PopUp*, _EVENT*);
void  popup_textlist_dump_ready(_Window *, _PopUp*, _BOOL);
void  popup_project_line(_Window *, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_project_run(_Window*, _PopUp*, _EVENT*);
void  popup_unit_line(_Window *, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_image_line(_Window *, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_image_run(_Window*, _PopUp*, _EVENT*);
#ifdef UC_IPC
void  popup_imageview_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
#endif
void  popup_object_line(_Window *, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_object_run(_Window*, _PopUp*, _EVENT*);
void  popup_scheme_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
//void  popup_scheme_run(_Window*, _PopUp*, _EVENT*);
void  popup_textscheme_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_function_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_function_run(_Window*, _PopUp*, _EVENT*);
void  popup_font_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_font_run(_Window*, _PopUp*, _EVENT*);
void  popup_fontview_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_fontview_run(_Window*, _PopUp*, _EVENT*);
void  popup_alltextlist_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_alltextlist_run(_Window*, _PopUp*, _EVENT*);
void  popup_menulist_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_menulist_run(_Window*, _PopUp*, _EVENT*);
void  popup_menu_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_stack_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_sequence_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_sequence_run(_Window*, _PopUp*, _EVENT*);
void  popup_colpal_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_systeminfo_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_systeminfo_run(_Window*, _PopUp*, _EVENT*);
void  popup_batch_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_hotkeylist_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_setup_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_hostname_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
#ifdef UC_THEME
void  popup_themelist_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
#endif
void  popup_online_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
void  popup_online_run(_Window*, _PopUp*, _EVENT*);
void  jump_to_server(_MEM_VKLIST*, _POSITION*, _UDWORD);

_UDWORD SeekCnt;
_ASCII  Seek[64];
void seek_init(void)
{
  SeekCnt = 0;
  Seek[0] = 0;
}

void popup_seek_ready(_Window *Pw, _PopUp *PopUp, _BOOL firsttime)
{
  PopUp->OutBasement(0, 50, (_ASCII*)"Seek:", MakeColor_Makro(LIGHTGRAY, BLACK));
  PopUp->OutBasement(50, 0, Seek, MakeColor_Makro((Seek[0] == 0)? LIGHTGRAY:LIGHTGREEN, BLACK));
}

_BOOL seek_run_sub(_EVENT *pev, _UDWORD *pvarno)
{
  _BOOL retcode = _FALSE;

  if(((pev->scancode >= 48)&&(pev->scancode < 128))||(pev->scancode == 32)||(pev->scancode == 46))
  {
    if(SeekCnt >= 63)
      SeekCnt = 0;
    Seek[SeekCnt] = pev->scancode;
    SeekCnt      += 1;
    Seek[SeekCnt] = 0;
    init_EVENT_Makro(pev);
  }
  else if(pev->scancode == _ESC)
  {
    if(SeekCnt > 0)
    {
      seek_init();
      init_EVENT_Makro(pev);
    }
  }
  else if(pev->scancode == _BCKSPC)
  {
    if(SeekCnt > 0)
    {
      SeekCnt -= 1;
      Seek[SeekCnt] = 0;
      init_EVENT_Makro(pev);
    }
  }
  else if((pev->scancode == _ENTER)&&(SeekCnt != 0))
  {
    retcode = _TRUE;
    *pvarno = DEFSCOPE;
    if((Seek[0] >= 0x30)&&(Seek[0] <= 0x39))
      *pvarno = AToD(Seek, sizeof(_ASCII));

    init_EVENT_Makro(pev);
  }

  return(retcode);
}

_BOOL seek_run(_EVENT *pev, _AsciiPuffer *Pap, _PopUp *PopUp)
{
  _BOOL   retcode = _FALSE;
  _UDWORD lineno;
  _UDWORD memo;

  if(pev->ftype == _EVENT_KEYPRESS)
  {
    memo = SeekCnt;
    if(seek_run_sub(pev, &lineno) == _TRUE)
    {
      if(lineno == DEFSCOPE)
      {
        if(Pap->Seek(&lineno, Seek) == _FALSE)
        {
          lineno = DEFSCOPE;
        }
      }

      if(lineno < PopUp->GetMenu()->position.no)
      {
        PopUp->GetMenu()->position.pos = lineno;
        seek_init();
      }
      else
      {
        PopUp->OutBasement(50, 0, Seek, MakeColor_Makro(LIGHTRED, WHITE));
      }
    }
    if(memo != SeekCnt)
      PopUp->OutBasement(50, 0, Seek, MakeColor_Makro((Seek[0] == 0)? LIGHTGRAY:LIGHTGREEN, BLACK));
  }

  return(retcode);
}

void seek_run_varlist(_EVENT *pev, _PopUp *PopUp)
{
  _UWORD  memo, i;
  _UDWORD varno;
  _MENU   *pm;

  if(pev->ftype == _EVENT_KEYPRESS)
  {
    memo = SeekCnt;
    if(seek_run_sub(pev, &varno) == _TRUE)
    {
      if(varno == DEFSCOPE)
      {
        for(i=0; i<256; i++)
        {
          if(varno == DEFSCOPE)
          {
            if(VarList.IsStation(i) == _TRUE)
            {
              varno = VarList.GetVarNo(Seek, i);
            }
          }
        }
      }

      if(varno != DEFSCOPE)
      {
        pm = PopUp->GetMenu();
        jump_to_server((_MEM_VKLIST*)pm->pointer.PtrSubList, &pm->position, varno);
        seek_init();
      }
      else
      {
        PopUp->OutBasement(50, 0, Seek, MakeColor_Makro(LIGHTRED, WHITE));
      }
    }
    if(memo != SeekCnt)
      PopUp->OutBasement(50, 0, Seek, MakeColor_Makro((Seek[0] == 0)? LIGHTGRAY:LIGHTGREEN, BLACK));
  }
}

void tcpiptext(_ASCII*, _UDWORD);

_UDWORD menuline = 0;

//#define FONTMARK

#ifdef FONTMARK
class _FontMark
{
  private :
    _UBYTE Data[8192];
    _BOOL  Valid(_UDWORD);

  public :
    void  Init(void);
    void  Free(void);
    void  ReSetState(_UDWORD);
    _BOOL GetState(_UDWORD);

};

_FontMark FontMark;

void _FontMark::Init(void)
{
  _File   File;
  _BOOL   state = _TRUE;
  _UDWORD i;

  if(File.Open(LOAD, "C:\\FONTMARK.BIN") == _TRUE)
  {
    File.Read(Data, sizeof(Data));
    state = File.GetError();
    File.Close();
  }

  if(state == _TRUE)
  {
    MemSet(Data, 0xFF, sizeof(Data));
    for(i=0; i<30000L; i++)
    {
      if(Valid(i) == _FALSE)
        ReSetState(i);
    }
  }
}

void _FontMark::Free(void)
{
  _File File;

  if(File.Open(SAVE, "C:\\FONTMARK.BIN") == _TRUE)
  {
    File.Write(Data, sizeof(Data));
    File.Close();
  }
}


_BOOL _FontMark::Valid(_UDWORD no)
{
  if(no < 32)
    return(_FALSE);
  if((no >= 127) && (no < 160))
    return(_FALSE);
  if((no >= 0x220) && (no < 0x250))
    return(_FALSE);
  if((no >= 0x500) && (no < 0x530))
    return(_FALSE);
  if((no >= 0x7C0) && (no < 0x900))
    return(_FALSE);
  if((no >= 0xD70) && (no < 0xE00))
    return(_FALSE);
  if((no >= 0xFC0) && (no < 0x10A0))
    return(_FALSE);
  if((no >= 0x1200) && (no < 0x1E00))
    return(_FALSE);
  if((no >= 0x20B0) && (no < 0x2100))
    return(_FALSE);
  if((no >= 0x2380) && (no < 0x2400))
    return(_FALSE);
  if((no >= 0x2670) && (no < 0x2700))
    return(_FALSE);
  if((no >= 0x27C0) && (no < 0x2E80))
    return(_FALSE);
  if((no >= 0x2ED0) && (no < 0x2FF0))
    return(_FALSE);
  if((no >= 0x31A0) && (no < 0x3200))
    return(_FALSE);
  if((no >= 0x3400) && (no < 0x3440))
    return(_FALSE);
  if((no >= 0x3450) && (no < 0x3470))
    return(_FALSE);
  if((no >= 0x3480) && (no < 0x4E00))
    return(_FALSE);
  if((no >= 0x9FB0) && (no < 0xAC00))
    return(_FALSE);
  if((no >= 0xD7B0) && (no < 0xE7C0))
    return(_FALSE);
  if((no >= 0xE870) && (no < 0xF900))
    return(_FALSE);
  if((no >= 0xFA30) && (no < 0xFB10))
    return(_FALSE);
  if((no >= 0xFDD0) && (no < 0xFDF0))
    return(_FALSE);
  if((no >= 0xFE00) && (no < 0xFE20))
    return(_FALSE);

  return(_TRUE);
}



void _FontMark::ReSetState(_UDWORD no)
{
  _UBYTE  bit = (1 << (no % 8));
  _UDWORD index = no / 8;

  Data[index] = Data[index] ^ bit;
}

_BOOL _FontMark::GetState(_UDWORD no)
{
  _UBYTE bit = (1 << (no % 8));
  _UDWORD index = no / 8;

  return((Data[index] & bit)? _TRUE : _FALSE);
}
#endif

#ifdef  OK_SUBLIST

#ifdef FONTMARK
 #define CHARACTER_PER_LINE  1
#else
 #define CHARACTER_PER_LINE  16
#endif

typedef struct { _UDWORD  begin;
                 _UDWORD  end;
               }
_NOVAREA;

typedef struct { _UDWORD   no;
                 _UDWORD  *ptr;
                 _NOVAREA *area;
               }
_NOVSCOPE;

_UDWORD supercode;

_UWORD sort_NOVSCOPE(_UDWORD *p1, _UDWORD *p2)
{
  _VARBLOCK *vb = VarList.GetPointerToData();
  _UWORD     retcode = 0;

  if(sort_xchange_ASCII(vb->Label.GetText(*p1), vb->Label.GetText(*p2)) == _TRUE)
    retcode = 1;

  return(retcode);
}

_BOOL varlist_alphasort(_NOVSCOPE *pnc)
{
  _VARBLOCK *vb = VarList.GetPointerToData();
  _UDWORD   scope, i, no, size;
  _UDWORD   *p;
  _BOOL     retcode = _FALSE;
  _NOVAREA  *pa;

  no   = 0;
  size = vb->no*sizeof(_NOVAREA);

  if(Memory2((void**)&pnc->area, size) != 0)
  {
    MemSet(pnc->area, 0, size);
    for(i=0,pa=pnc->area; i<vb->no; i++, pa++)
      pa->begin = 0xFFFFFFFF;

    for(i=0; i<vb->no; i++)
    {
      scope = vb->MemVarSymbol.ptr[i].scope;
      if(scope == DEFSCOPE)
      {
        no ++;
      }
      else
      {
        pa = &pnc->area[scope];
        if(i < pa->begin)
          pa->begin = i;
        if(i >= pa->end)
          pa->end = i;
      }
    }

    if(Memory2((void**)&pnc->ptr, no*sizeof(_UDWORD)) != 0)
    {
      pnc->no = no;
      p       = pnc->ptr;
      retcode = _TRUE;
      for(i=0; i<vb->no; i++)
      {
        if(vb->MemVarSymbol.ptr[i].scope == DEFSCOPE)
        {
          *p = i;
          p += 1;
        }
      }

      sort(pnc->ptr, no, 4, (_SORTFUNCT)sort_NOVSCOPE);
    }
  }

  return(retcode);
}

void make_alphalist_sub(_VARBLOCK *vb, _MEM_VKLIST *dvl, _UDWORD scope, _UDWORD no, _NOVAREA *pa)
{
  _UDWORD i, begin, end;
  _VARSYMBOL *pvs;

  scope = add_MEM_VKLIST(dvl, scope, no);
  begin = pa[no].begin;
  end   = pa[no].end;

  pvs = &vb->MemVarSymbol.ptr[begin];

  for(i=begin; i<=end; i++)
  {
    if(pvs->scope == no)
      make_alphalist_sub(vb, dvl, scope, i, pa);
    pvs += 1;
  }
}

_BOOL make_alphalist(_VARBLOCK *vb, _MEM_VKLIST *dvl)
{
  _BOOL     retcode = _TRUE;
  _NOVSCOPE nc;
  _UDWORD   i;

  if(dvl->no != vb->no)
  {
    retcode = _FALSE;
    nc.no   = 0;
    nc.ptr  = NULL;
    nc.area = NULL;

    if(varlist_alphasort(&nc) == _TRUE)
    {
      free_MEM_VKLIST(dvl);
      add_entries_MEM_VKLIST_lse(dvl, vb->no);

      for(i=0; i<nc.no; i++)
        make_alphalist_sub(vb, dvl, DEFSCOPE, nc.ptr[i], nc.area);

      Memory2((void**)&nc.ptr,  0);
      Memory2((void**)&nc.area, 0);
      retcode = _TRUE;
    }
  }

  return(retcode);
}

#endif



_UnderCover MyKey;

void _UnderCover::Init(void)
{
  Cnt  = 0;
  Pass = 0;
  Code = 0;
  Time = get_abstime();
}

_BOOL _UnderCover::Run(_UWORD scancode)
{
  _BOOL retcode = _FALSE;

  if(timediff(Time) > 2000)
    Init();

  if((scancode >= 48)&&(scancode <= 122))
  {  
    Time = get_abstime();
    Code = MakeCrc32(Code, &scancode, 2);
    if((Code == 0xE24BDCD7L)||(Code == 0xD542EC09L)||(Code == 0x7E4BE803L))
    {
      Init();
      retcode = _TRUE;
    }
    else
    {
      Cnt += 1;
      if(add_hex(&Pass, scancode) == _FALSE)
      {
        Cnt  = 0;
        Pass = 0;
      }

      if(Cnt == 8)
      {
        if(check_superpassword(Pass, 1) == _TRUE)
        {
          Init();
          retcode = _TRUE;
        }
      }
    }
  }
  else
  {
    Init();
  }

  return(retcode);
}

_BOOL Diagnostic(_MENU *Menu, _VARLABEL *vl, _UWORD no)
{
  _BOOL  retcode = _FALSE;

  switch(no)
  {
    case UC_HEAP_POPUP         : retcode = heap_call(Menu); break;
    case UC_SYSTEMINFO_POPUP   : retcode = systeminfo_call(Menu); break;
    case UC_LANGUAGE_POPUP     : retcode = language_call(Menu); break;
    case UC_ALLTEXT_POPUP      : retcode = alltextlist_call(Menu); break;
    case UC_HOSTNAME_POPUP     : retcode = hostname_call(Menu, VarList.GetHostName()); break;
    case UC_VAR_POPUP          : retcode = varlist_call(Menu); break;
    case UC_UNIT_POPUP         : retcode = unitlist_call(Menu); break;
    case UC_ALARM_POPUP        : retcode = alarmlist_call(Menu); break;
    case UC_PRJ_POPUP          : retcode = projectlist_call(Menu); break;
    case UC_FUNCTION_POPUP     : retcode = functionlist_call(Menu); break;
    case UC_COLSCHEME_POPUP    : retcode = scheme_call(Menu, &ColorScheme); break;
    case UC_SRNSCHEME_POPUP    : retcode = scheme_call(Menu, &ScreenScheme); break;
    case UC_FNTSCHEME_POPUP    : retcode = scheme_call(Menu, &FontScheme); break;
    case UC_BMPSCHEME_POPUP    : retcode = scheme_call(Menu, &ImageScheme); break;
    case UC_STATSCHEME_POPUP   : retcode = scheme_call(Menu, &StateScheme); break;
    case UC_SERVERSCHEME_POPUP : retcode = scheme_call(Menu, &ServerScheme); break;
    case UC_TEXTSCHEME_POPUP   : retcode = textscheme_call(Menu, &TextScheme); break;
    case UC_UNITSCHEME_POPUP   : retcode = scheme_call(Menu, &UnitScheme); break;
    case UC_OBJECTSCHEME_POPUP : retcode = scheme_call(Menu, &ObjectScheme); break;
    case UC_IMAGE_POPUP        : retcode = imagelist_call(Menu); break;
    case UC_OBJECT_POPUP       : retcode = objectlist_call(Menu); break;
    case UC_LSLID_POPUP        : retcode = idlist_call(Menu, vl); break;
    case UC_FONT_POPUP         : retcode = font_call(Menu); break;
    case UC_MENULIST_POPUP     : retcode = menulist_call(Menu); break;
    case UC_SEQUENCE_POPUP     : retcode = sequencelist_call(Menu); break;
    case UC_COLPAL_POPUP       : retcode = colpalpopup_call(Menu); break;
    case UC_HOTKEYLIST_POPUP   : retcode = hotkeylist_call(Menu); break;
    case UC_SETUP_POPUP        : retcode = setup_call(Menu); break;
   #ifdef UC_THEME
    case UC_THEME_POPUP        : retcode = themelist_call(Menu); break;
   #endif
    case UC_ONLINE_POPUP       : retcode = online_call(Menu); break;
    case UC_TXT_DIAG           : retcode = textlist_call(Menu, TextList.GetTextPuffer(menuline)); break;
    case UC_FKT_DIAG           : retcode = stack_call(Menu, InterBoxList.Get(menuline)); break;
    case UC_MENU_DIAG          : retcode = menu_call(Menu, menuline); break;
    case UC_FONT_DIAG          : retcode = fontview_call(Menu, menuline); break;
   #ifdef UC_IPC
    case UC_ONLINE_SERVER      : retcode = onlineserver_call(Menu); break;
    case UC_VAR_DIAG           : retcode = varview_call(Menu, menuline); break;
    case UC_IMAGE_DIAG         : retcode = imageview_call(Menu, menuline); break;
    case UC_IMAGEFILE_DIAG     : retcode = imagefile_call(Menu, ExplorerPath); break;
    case UC_TXT_DUMP_DIAG      : retcode = textlist_dump_call(Menu, (_CHAR*)menuline); break;
    case UC_TXT_FONTNAME_DIAG  : retcode = textlist_fontname_call(Menu, menuline); break;
    case UC_GENERALINFO        : retcode = general_call(Menu); break;
    case UC_TEXTFILE_POPUP     : retcode = batch_call(Menu, ExplorerPath); break;
    case UC_BINFILE_POPUP      : retcode = bin_call(Menu, ExplorerPath); break;
    case UC_EXPLORER_POPUP     : retcode = filexplo_call(Menu); break;
   #else
    case UC_TEXTFILE_POPUP     : retcode = batch_call(Menu, "c:\\autoexec.lsl"); break;
    case UC_BATCH_POPUP        : retcode = batch_call(Menu, PROJECTINIFILE); break;
   #endif
  }

  return(retcode);
}

_BOOL out_bmp(_ROOM *r, _UDWORD no, void *txt, _UWORD char_size)
{
  _BOOL retcode = _FALSE;
  _BMP  *bmp;
  _ROOM room;

  bmp = ImageList.Get(no);
  if(bmp != NULL)
  {
    if((bmp->height != 0) && (bmp->width != 0))
    {
      room.xy1.y = r->xy1.y + 1;
      room.xy2.y = r->xy2.y - 1;
      room.xy1.x = r->xy1.x;

      if(txt != NULL)
        room.xy1.x += StrWidth(txt, char_size) + 3;

      room.xy2.x = room.xy1.x + (((room.xy2.y - room.xy1.y + 1) * bmp->width) / bmp->height) - 1;
      out_zoom_BMP(&room, bmp, T_COPY|T_SOLID);
      retcode = _TRUE;
    }
  }

  return(retcode);
}


#if COMPILER_VERSION >= 33
void do_lselabel(_ASCII *pa, _AsciiPuffer *Pa, _UDWORD no, _ASCII cv, _ASCII ce)
{
  _UWORD len;

  len = StrLen(pa, sizeof(_ASCII));
  if(cv != 0)
  {
    pa[len] = cv;
    len    += 1;
  }

  pa[len]   = 91;
  StrCpy(&pa[len+1], sizeof(_ASCII), Pa->GetText(no), sizeof(_ASCII));
  len = StrLen(pa, sizeof(_ASCII));
  pa[len]   = 93;

  if(ce != 0)
  {
    pa[len+1] = ce;
    len      += 1;
  }

  pa[len+1] = 0;
}
#endif

void add_WHOAMI(_ASCII *txt, _Window *PWin, _UWORD size, _ASCII chr)
{
  _WHOAMI      whoami;
  const _ASCII *pt;
  _UWORD       len;

  if(PWin != NULL)
  {
    len = StrLen(txt, sizeof(_ASCII));

    if(len < size)
    {
      txt[len] = chr;
      len += 1;

      whoami = PWin->GetWhoAmI();

      switch(whoami.ima)
      {
        case IMA_PICTURE     : pt = "PIC:"; break;
        case IMA_WINDOW      : pt = "WIN:"; break;
        case IMA_GLOBAL      : pt = "GLO:"; break;
        case IMA_POPUP       : pt = "POP:"; break;
        case IMA_SCREENSAVER : pt = "SAV:"; break;
        case IMA_TOUCHEDITOR : pt = "TOC:"; break;
        case IMA_OFF_PICTURE : pt = "OFF:"; break;
        case IMA_OFF_WINDOW  : pt = "OFF:"; break;
        default              : pt = "???:"; break;
      }

      StrCpy(&txt[len], sizeof(_ASCII), pt, sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], whoami.no, 0x0800, sizeof(_ASCII));
      add_WHOAMI(txt, PWin->GetP_Window(), size, ',');
    }
    else
    {
      StrCat(txt, sizeof(_ASCII), "-->", sizeof(_ASCII));
    }
  }
}

void popup_project_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _PRJ_ENTRY   *pe;
  _KOORD       y;
  _ASCII       txt[200], *plabel;
  _UWORD       pos, len;
  _DWORD       ino;
  const _ASCII *pa;

  txt[0] = 0;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < DIAG_OVERHEAD)
  {
    switch(no)
    {
      case 0 : StrCpy(txt, sizeof(_ASCII), "sys          HEAP:", sizeof(_ASCII));
               if(SuperMode == _TRUE)
                 MemCpy(txt, "SUPERMODE", 9);
               add_WHOAMI(txt, &Root, 190, ' ');
               break;
      case 1 : StrCpy(txt, sizeof(_ASCII), "sys    SYSTEMINFO:", sizeof(_ASCII)); break;
     #ifdef UC_IPC
      case 2 : StrCpy(txt, sizeof(_ASCII), "sys      EXPLORER:", sizeof(_ASCII)); break;
     #else
      case 2 : StrCpy(txt, sizeof(_ASCII), "sys       IPC.INI:", sizeof(_ASCII)); break;
     #endif
      case 3 : StrCpy(txt, sizeof(_ASCII), "0000         UNIT:", sizeof(_ASCII)); break;
      case 4 : StrCpy(txt, sizeof(_ASCII), "0000     HOSTNAME:", sizeof(_ASCII)); break;
      case 5 : StrCpy(txt, sizeof(_ASCII), "0000  ONLINE INFO:", sizeof(_ASCII)); break;
    }
  }
  else
  {
    no -= DIAG_OVERHEAD;
    pe = Project.GetEntry(no);

    if(pe != NULL)
    {
      DToA(txt, no, 0x0104, sizeof(_ASCII));
      txt[4] = ' ';

      switch(pe->ftype)
      {
        case _PE_ALARMLIST    : pa  = "       ALARM:"; break;
        case _PE_VARLIST      : pa  = "    VARIABLE:"; break;
        case _PE_TEXTLIST     : pa  = "    TEXTLIST:"; break;
        case _PE_OBJECTLIST   : pa  = "      OBJECT:"; break;
        case _PE_BMPLIST      : pa  = "       IMAGE:"; break;
        case _PE_FUNCTIONLIST : pa  = "    FUNCTION:"; break;
        case _PE_MENULIST     : pa  = "        MENU:"; break;
        case _PE_FONTLIST     : pa  = "        FONT:"; break;
        case _PE_SEQUENCELIST : pa  = "    CHECKBIT:"; break;
        case _PE_WINDOW       : pa  = "      WINDOW:"; break;
        case _PE_PICTURE      : pa  = "     PICTURE:"; break;
        case _PE_COLORSCHEME  : pa  = "   COLSCHEME:"; break;
        case _PE_IMAGESCHEME  : pa  = " IMAGESCHEME:"; break;
        case _PE_UNITSCHEME   : pa  = "  UNITSCHEME:"; break;
        case _PE_FONTSCHEME   : pa  = "  FONTSCHEME:"; break;
        case _PE_SCRNSCHEME   : pa  = "SCREENSCHEME:"; break;
        case _PE_STATESCHEME  : pa  = " STATESCHEME:"; break;
        case _PE_TEXTSCHEME   : pa  = "  TEXTSCHEME:"; break;
        case _PE_OBJECTSCHEME : pa  = "OBJECTSCHEME:"; break;
        case _PE_SETUP        : pa  = "       SETUP:"; break;
        case _PE_PALETTE      : pa  = "COLORPALETTE:"; break;
        case _PE_HOTKEYLIST   : pa  = "     HOTKEYS:"; break;
        case _PE_LANGUAGE     : pa  = "   LANGUAGES:"; break;
       #ifdef UC_THEME
        case _PE_THEME        : pa  = "      THEMES:"; break;
       #endif
        case _PE_SERVERSCHEME : pa  = "SERVERSCHEME:"; break;
        case _PE_GLOBALSCREEN : pa  = "GLOBALSCREEN:"; break;
        default               : col = MakeColor_Makro(GetBackColor_Makro(col), DARKGRAY);
                                pa  = "        VOID:"; break;
      }

      StrCpy(&txt[5], sizeof(_ASCII), pa, sizeof(_ASCII));

      pos      = StrLen(txt, sizeof(_ASCII));
      txt[pos] = (Project.Access_PRJ_ENTRY(pe, _TRUE) == _FALSE)? '*':' '; pos ++;
//      txt[pos] = (chk_CHKBIT(&pe->chkbit) == _FALSE)? '*':' '; pos ++;
      StrCpy(&txt[pos], sizeof(_ASCII), pe->name, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), " ", sizeof(_ASCII));
      pos = StrLen(txt, sizeof(_ASCII));
      ino = -1;

      switch(pe->ftype)
      {
        case _PE_PICTURE :
        case _PE_WINDOW  :
          if(pe->refcyc)
            txt[(pe->ftype == _PE_PICTURE)? 8:9] = 0xAC;
          StrCpy(&txt[pos], sizeof(_ASCII), "No:", sizeof(_ASCII));
          pos += 3;
          DToA(&txt[pos], pe->no, 0x0800, sizeof(_ASCII));
          if((pe->nextno != 0xffff)&&(pe->nextno != pe->no))
          {
            StrCat(txt, sizeof(_ASCII), "->", sizeof(_ASCII));
            DToA(&txt[StrLen(txt, sizeof(_ASCII))], pe->nextno, 0x0800, sizeof(_ASCII));
          }
          
          if(pe->time_last != -1)
          {
            StrCat(txt, sizeof(_ASCII), " [t=", sizeof(_ASCII));
            DToA(&txt[StrLen(txt, sizeof(_ASCII))], pe->time_last, 0x0800, sizeof(_ASCII));
            
            if((pe->time_max > -1)&&(pe->time_max != pe->time_last))
            {
              StrCat(txt, sizeof(_ASCII), ",max=", sizeof(_ASCII));
              DToA(&txt[StrLen(txt, sizeof(_ASCII))], pe->time_max, 0x0800, sizeof(_ASCII));
            }
            StrCat(txt, sizeof(_ASCII), "]", sizeof(_ASCII));
          }
          
          plabel = Project.GetNameOfEntry(pe);
          if(plabel != NULL)
          {
            if(plabel[0] != 0)
            {
              len = StrLen(txt, sizeof(_ASCII));
              txt[len] = ' ';
              StrCpy(&txt[len+1], sizeof(_ASCII), plabel, sizeof(_ASCII));
            }
          }
          
//          if(plabel == NULL)
//            title = get_ANYPARA(&pe->anypara, NULL);
//            
//          if(title != NULL)
//          {
//            if(title[0] != 0)
//            {
//              len = StrLen(txt, sizeof(_ASCII));
//              txt[len] = ' ';
//              StrCpy(&txt[len+1], sizeof(_ASCII), title, sizeof(_CHAR));
//            }
//          }

          out_bmp(r, pe->imageno, txt, sizeof(_ASCII));


//          bmp = ImageList.Get(pe->imageno);
//          if(bmp != NULL)
//          {
//            br.xy2.x = r->xy2.x - 1;
//            br.xy2.y = r->xy2.y - 1;
//            br.xy1.y = r->xy1.y + 1;
//            br.xy1.x = br.xy2.x - (((r->xy2.y-r->xy1.y-1)*bmp->width)/bmp->height) - 1;
//            out_zoom_BMP(&br, bmp, T_COPY|T_SOLID);
//          }
          break;
        case _PE_TEXTLIST :
          StrCpy(&txt[pos], sizeof(_ASCII), "No:", sizeof(_ASCII));
          pos += 3;
          DToA(&txt[pos], pe->no, 0x0800, sizeof(_ASCII));
          pos = StrLen(txt, sizeof(_ASCII));
          txt[pos] = ' ';
          pos ++;
          txt[pos] = 0;
          ino = TextList.GetNo((_ANYLST)((_UWORD)TXTLIST_VTX + pe->no));
          break;
        case _PE_LANGUAGE     : ino = Language.GetNo(); break;
        case _PE_VARLIST      : ino = VarList.GetNo(); break;
        case _PE_COLORSCHEME  : ino = ColorScheme.GetNo(); break;
        case _PE_IMAGESCHEME  : ino = ImageScheme.GetNo(); break;
        case _PE_UNITSCHEME   : ino = UnitScheme.GetNo(); break;
        case _PE_FONTSCHEME   : ino = FontScheme.GetNo(); break;
        case _PE_SCRNSCHEME   : ino = ScreenScheme.GetNo(); break;
        case _PE_STATESCHEME  : ino = StateScheme.GetNo(); break;
        case _PE_OBJECTSCHEME : ino = ObjectScheme.GetNo(); break;
        case _PE_SERVERSCHEME : ino = ServerScheme.GetNo(); break;
        case _PE_BMPLIST      : ino = ImageList.GetNo(); break;
        case _PE_FUNCTIONLIST : ino = InterBoxList.GetNo(); break;
        case _PE_OBJECTLIST   : ino = ObjectList.GetNo(); break;
        case _PE_TEXTSCHEME   : ino = TextScheme.GetNo(); break;
        case _PE_FONTLIST     : ino = Font.GetNo(); break;
        case _PE_MENULIST     : ino = MenuList.GetNo(); break;
        case _PE_SEQUENCELIST : ino = Sequencer.GetNo(); break;
        case _PE_HOTKEYLIST   : ino = Hotkey.GetNo(); break;
//        case _PE_SETUP        : ino = SetUp.GetNo(); break;
        case _PE_ALARMLIST    : ino = Alarm.GetNo(); break;
       #ifdef UC_THEME
        case _PE_THEME        : ino = Theme.GetNo(); break;
       #endif
        default : break;
      }

      if(ino != -1)
        DToA(&txt[pos], ino, 0x0800, sizeof(_ASCII));
    }
  }

  if(txt[0] != 0)
  {
    y = (r->xy1.y + r->xy2.y) / 2;
    OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
}

void out_runtext(_PopUp *PopUp, void *txt, _UWORD chrsize)
{
  _MENU *pm = PopUp->GetMenu();
  _ROOM room;

  PopUp->GetLinePosition(pm->position.pos-pm->position.begin, &room);
  Font.Set(pm->info.font, NULL);
 #ifdef UC_MOUSE
  _UWORD hide = IfNecHideMouseCursor(&room);
 #endif
  PopUp->Cursor.Hide();
  OutTextXY(room.xy2.x, (room.xy1.y + room.xy2.y) / 2, txt, T_COPY|T_SOLID|T_RIGHTBOUND|T_MIDBOUND, pm->info.bar_color, chrsize);
 #ifdef UC_ROTATE
  RefreshRectangle(&room);
 #endif
 #ifdef UC_MOUSE
  IfNecShowMouseCursor(hide);
 #endif
}

void popup_project_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _ASCII      *txt = NULL;
  _PRJ_ENTRY  *pe;
  _UWORD      no;
  _UWORD      wno = 0;

  if(pev->ftype == _EVENT_KEYPRESS)
  {
    no = PopUp->GetMenu()->position.pos;

    if(pev->scancode == _ENTER)
    {
      if(no < DIAG_OVERHEAD)
      {
        switch(no)
        {
          case 0 : wno = UC_HEAP_POPUP; break;
          case 1 : wno = UC_SYSTEMINFO_POPUP; break;
         #ifdef UC_IPC
          case 2 : txt = (_ASCII*)"please wait...";
                   wno = UC_EXPLORER_POPUP; break;
         #else
          case 2 : wno = UC_BATCH_POPUP; break;
         #endif
          case 3 : wno = UC_UNIT_POPUP; break;
          case 4 : wno = UC_HOSTNAME_POPUP; break;
          case 5 : wno = UC_ONLINE_POPUP; break;

        }
      }
      else
      {
        no -= DIAG_OVERHEAD;

        pe = Project.GetEntry(no);
        if(pe != NULL)
        {
          switch(pe->ftype)
          {
            case _PE_ALARMLIST    : wno = UC_ALARM_POPUP; break;
            case _PE_VARLIST      : txt = (_ASCII*)"please wait...";
                                    wno = UC_VAR_POPUP; break;
            case _PE_TEXTLIST     : wno = UC_TXT_DIAG + pe->no; break;
//            case _PE_MENULIST     : wno = ; break;
//            case _PE_SEQUENCELIST : wno = ; break;
//            case _PE_WINDOW       : wno = ; break;
//            case _PE_PICTURE      : wno = ; break;
            case _PE_FONTLIST     : wno = UC_FONT_POPUP; break;
            case _PE_FUNCTIONLIST : wno = UC_FUNCTION_POPUP; break;
            case _PE_OBJECTLIST   : wno = UC_OBJECT_POPUP; break;
            case _PE_BMPLIST      : wno = UC_IMAGE_POPUP; break;
            case _PE_COLORSCHEME  : wno = UC_COLSCHEME_POPUP; break;
            case _PE_IMAGESCHEME  : wno = UC_BMPSCHEME_POPUP; break;
            case _PE_UNITSCHEME   : wno = UC_UNITSCHEME_POPUP; break;
            case _PE_FONTSCHEME   : wno = UC_FNTSCHEME_POPUP; break;
            case _PE_SCRNSCHEME   : wno = UC_SRNSCHEME_POPUP; break;
            case _PE_STATESCHEME  : wno = UC_STATSCHEME_POPUP; break;
            case _PE_TEXTSCHEME   : wno = UC_TEXTSCHEME_POPUP; break;
            case _PE_OBJECTSCHEME : wno = UC_OBJECTSCHEME_POPUP; break;
            case _PE_MENULIST     : wno = UC_MENULIST_POPUP; break;
            case _PE_SEQUENCELIST : wno = UC_SEQUENCE_POPUP; break;
            case _PE_PALETTE      : wno = UC_COLPAL_POPUP; break;
            case _PE_HOTKEYLIST   : wno = UC_HOTKEYLIST_POPUP; break;
            case _PE_SETUP        : wno = UC_SETUP_POPUP; break;
           #ifdef UC_THEME
            case _PE_THEME        : wno = UC_THEME_POPUP; break;
           #endif
            case _PE_SERVERSCHEME : wno = UC_SERVERSCHEME_POPUP; break;
            case _PE_PICTURE      : if(Project.Access_PRJ_ENTRY(pe, _TRUE) == _TRUE)
                                      Window->AddCmd(CMD_NEWPICTURE, pe->no, 0, pev->lrm);
                                    break;
            case _PE_WINDOW       : if(Project.Access_PRJ_ENTRY(pe, _TRUE) == _TRUE)
                                      Window->AddCmd(CMD_NEWWINDOW, pe->no, 0, pev->lrm);
                                    break;
            case _PE_LANGUAGE     : wno = UC_LANGUAGE_POPUP; break;
            default               : break;
          }
        }
      }

      if(wno != 0)
      {
        if(txt != NULL)
          out_runtext(PopUp, txt, sizeof(_ASCII));

        Window->AddCmd(CMD_NEWPOPUP, wno, 0, pev->lrm);
      }

      pev->ftype = _EVENT_NONE;   // eat event
    }
   #ifdef UC_IPC
    else if(pev->scancode == _F1)
    {
      if(no >= DIAG_OVERHEAD)
      {
        no -= DIAG_OVERHEAD;
        pe = Project.GetEntry(no);
        if(pe != NULL)
        {
          switch(pe->ftype)
          {
            case _PE_VARLIST      : VarList.MemSize(&MemSize);   break;
            case _PE_BMPLIST      : ImageList.MemSize(&MemSize); break;
            case _PE_FONTLIST     : Font.MemSize(&MemSize);      break;
            case _PE_SEQUENCELIST : Sequencer.MemSize(&MemSize); break;
            case _PE_LANGUAGE     : TextList.MemSize(&MemSize);  break;
           #ifdef UC_THEME
            case _PE_THEME        : Theme.MemSize(&MemSize);     break;
           #endif

            default               : init_MEMSIZE(&MemSize);      break;
          }
          Window->AddCmd(CMD_NEWPOPUP, UC_GENERALINFO, 0, pev->lrm);
        }
      }
      pev->ftype = _EVENT_NONE;   // eat event
    }
	else if(pev->scancode == _F2)
    {
      if(no >= DIAG_OVERHEAD)
      {
        no -= DIAG_OVERHEAD;
        pe = Project.GetEntry(no);
        if((pe->ftype == _PE_PICTURE)||(pe->ftype == _PE_WINDOW))
        {
          pe->refcyc = (pe->refcyc)? 0 : 1;
          pev->ftype = _EVENT_NONE;   // eat event
          PopUp->GetMenu()->position.oldbegin += 100; // refresh screen
        }
      }
	}
   #endif
    else if((pev->is_character == _TRUE)&&(pev->scancode <= 256))
    {
      if(MyKey.Run(pev->scancode) == _TRUE)
      {
        SuperMode = (SuperMode == _FALSE)? _TRUE : _FALSE;
        PopUp->GetMenu()->position.oldbegin += 100; // refresh screen
        MyKey.Init();
      }
    }
    else
    {
      MyKey.Init();
    }
  }
}

void diagnostic_call(_MENU *Menu, _UWORD bigline, _BOOL basement)
{
  _ROOM room;

  MyGetScreenSize(&room);

  Menu->info.font        = DEFFONT;
  Menu->mencursor.active = _FALSE;

  if(room.xy2.x <= 320)
  {
    Menu->info.xy.x   = 7;
    Menu->info.xy.y   = 7;
    Menu->info.width  = room.xy2.x - 14 - LEVELBUTTON_WIDTH;
    Menu->info.height = 12 / bigline;
    Menu->info.line_height = 13 * bigline;
  }
  else
  {
    Menu->info.xy.x   = 10;
    Menu->info.xy.y   = 200;
    Menu->info.width  = room.xy2.x - 20 - LEVELBUTTON_WIDTH;
    Menu->info.height = 20 / bigline;
    Menu->info.line_height = 16 * bigline;
  }

  if(basement == _TRUE)
  {
    Menu->pointer.PtrReady = popup_seek_ready;
    Menu->info.basement = 16;
    seek_init();
  }

 #ifndef MENUE_CURSOR_ALT
  Menu->info.bar_color = MakeColor_Makro(SetUp.GetEditColorBack(_EDITACTIVE, BLACK), SetUp.GetEditColorTxt(_EDITACTIVE, WHITE));
 #endif
}

_BOOL varlist_call(_MENU *Menu)
{
  _VARBLOCK *vb = VarList.GetPointerToData();

  diagnostic_call(Menu, 1, _TRUE);
  Menu->pointer.PtrLine = popup_varinfo_line;    // --> to linedraw function
  Menu->pointer.PtrRun  = popup_varinfo_run;          // --> to run function
  Menu->pointer.PtrData = vb;
  Menu->position.no     = vb->no; //VarList.GetNo();
  supercode             = DEF4BYTE;

 #ifdef OK_SUBLIST
  _UWORD i;

  if(make_alphalist(vb, &DiagVarList) == _FALSE)
  {
    free_MEM_VKLIST(&DiagVarList);
    add_entries_MEM_VKLIST_lse(&DiagVarList, vb->no);
    for(i=0; i<vb->no; i++)
      add_MEM_VKLIST(&DiagVarList, vb->MemVarSymbol.ptr[i].scope, i);
  }

  Menu->pointer.PtrSubList = &DiagVarList;
  Menu->position.no        = DiagVarList.visual_no;
 #endif

  return(_TRUE);
}

_BOOL alarmlist_call(_MENU *Menu)
{
  _ALARM *pa;

  diagnostic_call(Menu, 1, _TRUE);
  pa = Alarm.GetAlarm();
  if(pa != NULL)
  {
    Menu->pointer.PtrLine = popup_alarminfo_line;  // --> to linedraw function
    Menu->pointer.PtrRun  = popup_alarm_run;            // --> to run function
    Menu->pointer.PtrData = (void*)pa;
    Menu->position.no     = pa->no;
  }

  return(_TRUE);
}

_BOOL projectlist_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine = popup_project_line;    // --> to linedraw function
  Menu->pointer.PtrRun  = popup_project_run;          // --> to run function
  Menu->position.no     = Project.GetNo() + DIAG_OVERHEAD;
  MyKey.Init();

  return(_TRUE);
}

_BOOL language_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine = popup_language_line;    // --> to linedraw function
  Menu->pointer.PtrRun  = popup_language_run;          // --> to run function
  Menu->position.pos    = get_actual_language();
  Menu->position.no     = Language.GetNo();
  return(_TRUE);
}

_BOOL alltextlist_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine = popup_alltextlist_line;    // --> to linedraw function
  Menu->pointer.PtrRun  = popup_alltextlist_run;          // --> to run function
  Menu->position.no     = TextList.GetNo();
  return(_TRUE);
}

_BOOL font_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine = popup_font_line;    // --> to linedraw function
  Menu->pointer.PtrRun  = popup_font_run;          // --> to run function
  Menu->position.no     = Font.GetNo();
  return(_TRUE);
}

_BOOL menulist_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _TRUE);
  Menu->pointer.PtrLine = popup_menulist_line;    // --> to linedraw function
  Menu->pointer.PtrRun  = popup_menulist_run;          // --> to run function
  Menu->position.no     = MenuList.GetNo();
  return(_TRUE);
}

_BOOL functionlist_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _TRUE);
  Menu->pointer.PtrLine = popup_function_line;    // --> to linedraw function
  Menu->pointer.PtrRun  = popup_function_run;          // --> to run function
  Menu->position.no     = InterBoxList.GetNo();
  return(_TRUE);
}

_BOOL imagelist_call(_MENU *Menu)
{
  diagnostic_call(Menu, 2, _TRUE);
  Menu->pointer.PtrLine = popup_image_line;    // --> to linedraw function
  Menu->pointer.PtrRun  = popup_image_run;
  Menu->position.no     = ImageList.GetNo();
  return(_TRUE);
}

_BOOL unitlist_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine = popup_unit_line;    // --> to linedraw function
  Menu->position.no     = VarList.GetNoUnit() + 1;
  return(_TRUE);
}

_BOOL objectlist_call(_MENU *Menu)
{
  diagnostic_call(Menu, 3, _TRUE);
  Menu->pointer.PtrLine = popup_object_line;    // --> to linedraw function
  Menu->pointer.PtrRun  = popup_object_run;          // --> to run function
  Menu->position.no     = ObjectList.GetNo();
  return(_TRUE);
}

_BOOL hostname_call(_MENU *Menu, _NAME_INDEX *p)
{
  _BOOL retcode = _FALSE;

  diagnostic_call(Menu, 1, _FALSE);

  if(p != NULL)
  {
    Menu->pointer.PtrLine = popup_hostname_line;    // --> to linedraw function
    Menu->position.no     = p->name.no;
    Menu->pointer.PtrData = (void*)p;
    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL textlist_call(_MENU *Menu, _TextPuffer *Pt)
{
  _BOOL retcode = _FALSE;

  diagnostic_call(Menu, 1, _FALSE);

  if(Pt != NULL)
  {
    Menu->pointer.PtrLine = popup_textlist_line;    // --> to linedraw function
    Menu->pointer.PtrRun  = popup_textlist_run;
    Menu->pointer.PtrData = (void*)Pt;
    Menu->position.no     = Pt->GetNo();
    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL sequencelist_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _TRUE);
  Menu->pointer.PtrLine = popup_sequence_line;    // --> to linedraw function
  Menu->pointer.PtrRun  = popup_sequence_run;          // --> to run function
  Menu->position.no     = Sequencer.GetNo();
  return(_TRUE);
}

_BOOL text_station(_ASCII *txt, _LSEVARINFO *pi, _UDWORD scope)
{
  _ASCII   *tmp, *padr = NULL;
  _BOOL    retcode = _TRUE;
  _COMPORT *cp;

  tmp = &txt[StrLen(txt, sizeof(_ASCII))];
  cp = Batch.Get_COMPORT(pi->station);
  if(cp != NULL)
  {
    {
      if(pi->lasalid.id == 0)
		  retcode = _FALSE;

     #ifdef OK_SUBLIST
      if(scope == DEFSCOPE)
     #endif
      {
        switch(cp->comdef.typ)
        {
          case _COM_LOCAL    : StrCpy(tmp, sizeof(_ASCII), " LOCAL  ", sizeof(_ASCII));
                               padr = &tmp[6];
                               break;
          case _COM_INTERN   : StrCpy(tmp, sizeof(_ASCII), " INTERN ", sizeof(_ASCII));
                               break;
          case _COM_COM1     : StrCpy(tmp, sizeof(_ASCII), " COM1   ", sizeof(_ASCII));
                               break;
          case _COM_COM2     : StrCpy(tmp, sizeof(_ASCII), " COM2   ", sizeof(_ASCII));
                               break;
          case _COM_COM3     : StrCpy(tmp, sizeof(_ASCII), " COM3   ", sizeof(_ASCII));
                               break;
          case _COM_COM4     : StrCpy(tmp, sizeof(_ASCII), " COM4   ", sizeof(_ASCII));
                               break;
          case _COM_CAN1     : StrCpy(tmp, sizeof(_ASCII), " CAN1 ", sizeof(_ASCII));
                               padr = &tmp[6];
                               break;
          case _COM_CAN2     : StrCpy(tmp, sizeof(_ASCII), " CAN2 ", sizeof(_ASCII));
                               padr = &tmp[6];
                               break;
          case _COM_CAN20    : StrCpy(tmp, sizeof(_ASCII), " CAN20 ", sizeof(_ASCII));
                               padr = &tmp[7];
                               break;
          case _COM_CAN21    : StrCpy(tmp, sizeof(_ASCII), " CAN21 ", sizeof(_ASCII));
                               padr = &tmp[7];
                               break;
          case _COM_CAN22    : StrCpy(tmp, sizeof(_ASCII), " CAN22 ", sizeof(_ASCII));
                               padr = &tmp[7];
                               break;
          case _COM_CAN23    : StrCpy(tmp, sizeof(_ASCII), " CAN23 ", sizeof(_ASCII));
                               padr = &tmp[7];
                               break;
          case _COM_CAN24    : StrCpy(tmp, sizeof(_ASCII), " CAN24 ", sizeof(_ASCII));
                               padr = &tmp[7];
                               break;
          case _COM_CAN25    : StrCpy(tmp, sizeof(_ASCII), " CAN25 ", sizeof(_ASCII));
                               padr = &tmp[7];
                               break;
          case _COM_CAN26    : StrCpy(tmp, sizeof(_ASCII), " CAN26 ", sizeof(_ASCII));
                               padr = &tmp[7];
                               break;
          case _COM_CAN27    : StrCpy(tmp, sizeof(_ASCII), " CAN27 ", sizeof(_ASCII));
                               padr = &tmp[7];
                               break;
          case _COM_CAN28    : StrCpy(tmp, sizeof(_ASCII), " CAN28 ", sizeof(_ASCII));
                               padr = &tmp[7];
                               break;
          case _COM_CAN29    : StrCpy(tmp, sizeof(_ASCII), " CAN29 ", sizeof(_ASCII));
                               padr = &tmp[7];
                               break;
          case _COM_TCPIP1   :
          case _COM_TCPIP1_P :
                               StrCpy(tmp, sizeof(_ASCII), " TCPIP1 ", sizeof(_ASCII));
                               break;
          case _COM_TCPIP2_P : StrCpy(tmp, sizeof(_ASCII), " TCPIP2 ", sizeof(_ASCII));
                               break;
          default            : StrCpy(tmp, sizeof(_ASCII), " ERROR  ", sizeof(_ASCII));
                               retcode = _FALSE;
                               break;
        }

        if(padr != NULL)
          DToA(padr, cp->comdef.adress, 0x0102, sizeof(_ASCII));


//        DToH(&tmp[StrLen(tmp, sizeof(_ASCII))], pi->lasalid.id, 0x0108, sizeof(_ASCII));
      }
    }
  }
  else
  {
   #ifdef OK_SUBLIST
    if(scope == DEFSCOPE)
   #endif
    {
      StrCpy(tmp, sizeof(_ASCII), " 000 undef.station", sizeof(_ASCII));
      DToA(&tmp[1], pi->station, 0x0103, sizeof(_ASCII));
      tmp[4] = ' ';
    }
    retcode = _FALSE;
  }

  StrCat(tmp, sizeof(_ASCII), " ", sizeof(_ASCII));

  return(retcode);
}

void popup_hostname_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII      label[20];
  _MENU       *p;
  _KOORD      y;
  _NAME_INDEX *pn;
  _ATTRIB     att;
  _ASCII      *pl;

  p   = Pp->GetMenu();
  y   = (r->xy1.y + r->xy2.y) / 2;
  pn  = (_NAME_INDEX*)p->pointer.PtrData;
  att = (p->info.attrib & (~(T_HORIZBOUND|T_VERTBOUND))) |T_LEFTBOUND|T_MIDBOUND;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < pn->name.no)
  {
    DToA(label, no, 0x0105, sizeof(_ASCII));
    if(no < pn->index.no)
    {
      StrCat(label, sizeof(_ASCII), "\\", sizeof(_ASCII));
      DToA(&label[StrLen(label, sizeof(_ASCII))], pn->index.ptr[no], 0x0105, sizeof(_ASCII));
    }
    OutTextXY(r->xy1.x, y, label, att, col, sizeof(_ASCII));

    pl = gettext_ASCII_BUFFER(&pn->name, no);
    if(pl != NULL)
      OutTextXY(r->xy1.x+100, y, pl, att, col, sizeof(_ASCII));
  }
}

void popup_textlist_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII      label[10];
  _MENU       *p;
  _KOORD      y;
  _TextPuffer *Pt;
  _ATTRIB     att;

  DToA(label, no, 0x0105, sizeof(_ASCII));

  p   = Pp->GetMenu();
  y   = (r->xy1.y + r->xy2.y) / 2;
  Pt  = (_TextPuffer*)p->pointer.PtrData;
  att = (p->info.attrib & (~(T_HORIZBOUND|T_VERTBOUND))) |T_LEFTBOUND|T_MIDBOUND;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < Pt->GetNo())
  {
    OutTextXY(r->xy1.x, y, label, att, col, sizeof(_ASCII));
    OutTextXY(r->xy1.x + 10 + StrWidth(label, sizeof(_ASCII)), y, Pt->GetText(no), att, col, sizeof(_CHAR));
  }

}

void popup_textlist_run(_Window *Pw, _PopUp *Pp, _EVENT *pe)
{
  _UWORD no;
  _MENU *pm;


  if(pe->ftype == _EVENT_KEYPRESS)
  {
    switch(pe->scancode)
    {
      case _ENTER:
        no = Pp->GetMenu()->position.pos;

        menuline = (_UDWORD)((_TextPuffer*)(Pp->GetMenu()->pointer.PtrData))->GetText(no);
        if(menuline != 0xFFFF)
        {
          Pw->AddCmd(CMD_NEWPOPUP, UC_TXT_DUMP_DIAG, 0, pe->lrm);
          pe->ftype = _EVENT_NONE;   // eat event
        }
      break;

      case _F1:
        pm = Pp->GetMenu();

        pm->info.font++;
        if(pm->info.font >= Font.GetNo())
          pm->info.font = 0;

        Font.SetDirect(pm->info.font, &pm->info.attrib);
        Pw->AddCmd(CMD_REDRAW, 0, 0, pe->lrm);

        Pw->AddCmd(CMD_NEWPOPUP, UC_TXT_FONTNAME_DIAG, 0, pe->lrm);

        menuline = pm->info.font;

        pe->ftype = _EVENT_NONE;
      break;

    }
  }

}

_BOOL textlist_fontname_call(_MENU *Menu, _UWORD font)
{
  _ASCII *txt;
  _UWORD x, y;

  diagnostic_call(Menu, 0xFFFF, _FALSE);

  Menu->pointer.PtrReady = popup_textlist_fontname_ready;
  Menu->pointer.PtrRun = popup_textlist_fontname_run;
  Menu->pointer.PtrData = (void*)(_UDWORD)font;
  Menu->position.no = 0;

  txt = Font.LseLabel.GetText(font);
  Font.SizeDirect(font, &x, &y);

  Menu->info.width = StrWidth(txt, sizeof(_ASCII)) + 50;
  Menu->info.line_height = y + 10;
  Menu->info.height = 1;

  return(_TRUE);
}

void popup_textlist_fontname_ready(_Window *Pw, _PopUp *Pp, _BOOL firsttime)
{
  _MENU *pm;
  _ASCII *txt;

  pm  = Pp->GetMenu();
  txt = Font.LseLabel.GetText((_UDWORD)pm->pointer.PtrData);
  OutTextXY(pm->calcul.scroll_room.xy1.x + 10, (pm->calcul.scroll_room.xy1.y + pm->calcul.scroll_room.xy2.y) / 2, txt, pm->info.attrib, pm->info.text_color, sizeof(_ASCII));

}

void popup_textlist_fontname_run(_Window *Pw, _PopUp *Pp, _EVENT *pe)
{

  if(pe->ftype == _EVENT_KEYPRESS)
  {
    Pw->AddCmd(CMD_RETURN, 0, 0, pe->lrm);
    pe->ftype = _EVENT_NONE;
  }

}

_BOOL textlist_dump_call(_MENU *Menu, _CHAR *text)
{
  _BOOL retcode = _FALSE;


  diagnostic_call(Menu, 1, _FALSE);

  if(text != NULL)
  {
    Menu->pointer.PtrReady  = popup_textlist_dump_ready;
    Menu->pointer.PtrData  = text;
    Menu->position.no      = 0;
    retcode = _TRUE;
  }

  return(retcode);
}

void popup_textlist_dump_ready(_Window *Pw, _PopUp *Pp, _BOOL firsttime)
{
  _MENU *pm;
  _CHAR *ptxt;
  _ROOM room;
  _UWORD lines;
  _UWORD i, j, k;
  _ASCII txt[300];
  _UWORD len;
 #ifdef UC_UNICODE
  _UWORD format = 0x0104;
 #else
  _UWORD format = 0x0102;
 #endif

  pm = Pp->GetMenu();
  ptxt = (_CHAR*)pm->pointer.PtrData;

  room = pm->calcul.scroll_room;
  lines = (room.xy2.y - room.xy1.y + 1) / pm->info.line_height;

  len = StrLen(ptxt, sizeof(_CHAR));

  j = 0;
  for(i=0; i<lines && j<len; i++)
  {
    txt[0] = 0;
    for(k=0; j<len && k<16; j++, k++)
    {
      DToH(&txt[StrLen(txt, sizeof(_ASCII))], ptxt[j], format, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), " ", sizeof(_ASCII));
    }

    Font.Set(pm->info.font, &pm->info.attrib);
    Pp->GetLinePosition(i, &room);
    OutTextXY(room.xy1.x, (room.xy1.y + room.xy2.y) / 2, txt, pm->info.attrib, pm->info.text_color, sizeof(_ASCII));
  }

}

#ifdef UC_IPC
_BOOL varview_call(_MENU *Menu, _UDWORD varno)
{

  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine  = popup_varview_line;    // --> to linedraw function
  VarList.GetVarInfo((_LSEVARINFO**)&Menu->pointer.PtrData, varno);
  Menu->position.no      = 26;

  return(_TRUE);
}

void do_limit(_ASCII *txt, _VARLABEL *vl)
{
  _CHAR tmp[100];
  _UBYTE obj;
  _VARIABLE variable;
  _IVALUE value;
  _COMPORT *pcp;


  if(vl->info[0].state != DEF_VAL)
  {
    if((vl->info[0].state == VAR_VAL) || (vl->info[0].state == REAL_VAL))
    {
      VarList.GetLabel(&txt[StrLen(txt, sizeof(_ASCII))], &obj, vl->info[0].value);
      StrCat(txt, sizeof(_ASCII), " - VALUE = ", sizeof(_ASCII));
      if(VarList.GetSingleVariable(&variable, vl->info[0].value) == _TRUE)
      {
        if(chk_LASALID(&variable.info.lasalid) == _TRUE)
        {
          pcp = Batch.Get_COMPORT(variable.info.station);
         #ifdef UC_EXOONLINE
          get_direct_PLC_sub((void*)&value, &variable.info.lasalid, pcp, 4, (_SVRTYP)variable.info.exoinfo);
         #else
          get_direct_PLC_sub((void*)&value, &variable.info.lasalid, pcp, 4, DEFSVRTYP);
         #endif
          format_value(tmp, value, &variable.info, NULL);
          StrCat(txt, sizeof(_ASCII), tmp, sizeof(_CHAR));
        }
      }
    }
    else if(vl->info[0].state == CONST_VAL)
    {
      StrCat(txt, sizeof(_ASCII), "CONSTANT - VALUE = ", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], vl->info[0].value, 0x0800, sizeof(_ASCII));
    }
  }

}

void do_vartext(_ASCII *txt, _ANYPARA *vartext)
{
  _CHAR *pchar;

  if((vartext->anything.list != DEFLST) && (vartext->anything.no != 0xFFFF))
  {
    StrCat(txt, sizeof(_ASCII), "Textlist_", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], vartext->anything.list-TXTLIST_VTX, 0x0800, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), " - Text = ", sizeof(_ASCII));

    pchar = get_text(&vartext->anything, NULL, NULL, _FALSE);
    if(pchar != NULL)
    {
      StrCat(txt, sizeof(_ASCII), pchar, sizeof(_CHAR));
    }
  }

}

void do_linkvl(_ASCII *txt, _LINKVL *linkvl)
{
  _UBYTE       obj;
  const _ASCII *pa;

  if(linkvl->info != VL_UNDEFINED)
  {
    switch(linkvl->info)
	{
      case VL_RAMPSETPOINT      : pa = "TYPE = RAMPSETPOINT - LINK = ";           break;
      case VL_SETPOINT          : pa = "TYPE = SETPOINT - LINK = ";               break;
      case VL_ACTVALUE          : pa = "TYPE = ACTUAL VALUE - LINK =";            break;
      case VL_COLORCHANGE       : pa = "TYPE = COLORVALUE - LINK = ";             break;
      case VL_STATEOVERLOAD     : pa = "TYPE = STATESCHEMEOVERLOAD - LINK = ";    break;
      case VL_ADJUSTMENTPARA    : pa = "TYPE = ADJUSTMENTPARAMETER - LINK = ";    break;
      case VL_FUNCTIONSWITCH    : pa = "TYPE = FUNCTIONSWITCH - LINK = ";         break;
      case VL_REDUPPERLIMIT     : pa = "TYPE = REDUCEUPPERLIMIT - LINK = ";       break;
      case VL_REDUPPERLIMITENBL : pa = "TYPE = REDUCEUPPERLIMITENABLE - LINK = "; break;
      case VL_STRUCTUREOFFSET   : pa = "TYPE = STRUCTUREOFFSET - OFFSET = ";      break;
      default                   : pa = NULL;                                      break;
    }

    if(pa != NULL)
    {
      StrCat(txt, sizeof(_ASCII), pa, sizeof(_ASCII));
    }

    if(linkvl->info == VL_STRUCTUREOFFSET)
    {
      if(linkvl->link.info[0].state == CONST_VAL)
      {
        DToA(&txt[StrLen(txt, sizeof(_ASCII))], linkvl->link.info[0].value, 0x0108, sizeof(_ASCII));
        return;
      }
    }

    if(linkvl->link.info[0].state != DEF_VAL)
    {
      if(linkvl->link.info[0].state == VAR_VAL)
      {
        VarList.GetLabel(&txt[StrLen(txt, sizeof(_ASCII))], &obj, linkvl->link.info[0].value);
      }
    }
  }

}

void popup_varview_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII       txt[200];
  const _ASCII *ph;
  _MENU        *pm;
  _ASCII       *pascii;
  _CHAR        *pchar;
  _KOORD       y;
  _ATTRIB      attrib;
  _BOOL        ret;
  _VARINFO     vi;
  _LSEVARINFO  *pvi;
  
  pm = Pp->GetMenu();

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if((pm->pointer.PtrData != NULL) && (no < pm->position.no))
  {
    ret = _TRUE;
    pvi = (_LSEVARINFO*)pm->pointer.PtrData;
    
    copy_LSE_VARINFO(&ret, &vi, pvi, _TRUE);

    switch(no)
    {
      case 0:
        txt[0] = 0;

       #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
        _LSEVARINFO *plvi;
        if(vi.POWPRUIC & VARBIT_VARGLOBAL)
        {
          plvi = (_LSEVARINFO*)pm->pointer.PtrData;
          StrCpy(txt, sizeof(_ASCII), "GLOBAL VAR  ", sizeof(_ASCII));

          if((plvi->memlinkvl.no > 0) && (plvi->memlinkvl.ptr != NULL))
          {
            if(plvi->memlinkvl.ptr[plvi->memlinkvl.no-1].info == VL_STRUCTUREOFFSET)
            {
              if(plvi->memlinkvl.ptr[plvi->memlinkvl.no-1].link.info[0].state == CONST_VAL)
              {
                StrCat(txt, sizeof(_ASCII), "OFFSET=", sizeof(_ASCII));
                DToA(&txt[StrLen(txt, sizeof(_ASCII))], plvi->memlinkvl.ptr[plvi->memlinkvl.no-1].link.info[0].value, 0x0105, sizeof(_ASCII));
                StrCat(txt, sizeof(_ASCII), ", ", sizeof(_ASCII));
              }
            }
          }
        }
       #endif
       #ifdef UC_COMPLEX_SERVER
        if(vi.POWPRUIC & VARBIT_STRUCTELEMENT)
        {
          StrCpy(txt, sizeof(_ASCII), "SERVER ELEMENT, OFFSET=", sizeof(_ASCII));
          DToA(&txt[StrLen(txt, sizeof(_ASCII))], vi.bitno, 0x0101, sizeof(_ASCII));
          StrCat(txt, sizeof(_ASCII), "  ", sizeof(_ASCII));
        }
       #endif

        if(vi.POWPRUIC & VARBIT_VARMEMBER)
        {
          StrCpy(txt, sizeof(_ASCII), "MEMBER VAR  ", sizeof(_ASCII));
        } 

        StrCat(txt, sizeof(_ASCII), "TYPE=", sizeof(_ASCII));

        if(vi.vartype == BINARY)
        {
          StrCat(txt, sizeof(_ASCII), "BINARY, SUBTYPE=", sizeof(_ASCII));
          switch(vi.format)
          {
            case SIZE_BYTE  : ph = "SINT";  break;
            case SIZE_WORD  : ph = "INT";   break;
            case SIZE_DWORD : ph = "DINT";  break;
            case SIZE_UBYTE : ph = "USINT"; break;
            case SIZE_UWORD : ph = "UINT";  break;
            case SIZE_UDWORD: ph = "UDINT"; break;
            case SIZE_HBYTE : ph = "HSINT"; break;
            case SIZE_HWORD : ph = "HINT";  break;
            case SIZE_HDWORD: ph = "HDINT"; break;
            case SIZE_BBYTE : ph = "BSINT"; break;
            case SIZE_BWORD : ph = "BINT";  break;
            case SIZE_BDWORD: ph = "BDINT"; break;
            case SIZE_LREAL : ph = "LREAL"; break;
            default         : ph = "REAL";  break;

          }
          StrCat(txt, sizeof(_ASCII), ph, sizeof(_ASCII));
        }
        else if(vi.vartype == FSTRING)
        {
          StrCat(txt, sizeof(_ASCII), "STRING, LENGTH=", sizeof(_ASCII));
          DToA(&txt[StrLen(txt, sizeof(_ASCII))], vi.format, 0x0103, sizeof(_ASCII));
          StrCat(txt, sizeof(_ASCII), ", ONLY ASCII=", sizeof(_ASCII));
          StrCat(txt, sizeof(_ASCII), (VarList.GetOnlyAscii(&vi) == _TRUE)? "TRUE":"FALSE", sizeof(_ASCII));
        }
        else if(vi.vartype == FTIME)
        {
          StrCat(txt, sizeof(_ASCII), "TIME, SUBTYPE=", sizeof(_ASCII));

          if(vi.format == SIZE_HHMM)
            StrCat(txt, sizeof(_ASCII), "HHMM", sizeof(_ASCII));
          else if(vi.format == SIZE_HHMMSS)
            StrCat(txt, sizeof(_ASCII), "HHMMSS", sizeof(_ASCII));
          else if(vi.format == SIZE_MMSS)
            StrCat(txt, sizeof(_ASCII), "MMSS", sizeof(_ASCII));
        }
        else if(vi.vartype == FDATE)
        {
          StrCat(txt, sizeof(_ASCII), "DATE, SUBTYPE=", sizeof(_ASCII));
          switch(vi.format)
          {
            case SIZE_DDMM       : ph = "DDMM";       break;
            case SIZE_DDMMYY     : ph = "DDMMYY";     break;
            case SIZE_WWDDMM     : ph = "WWDDMM";     break;
            case SIZE_WWDDMMYY   : ph = "WWDDMMYY";   break;
            case SIZE_DDMMYYYY   : ph = "DDMMYYYY";   break;
            case SIZE_WWDDMMYYYY : ph = "WWDDMMYYYY"; break;
            case SIZE_YYYYMMDD   : ph = "YYYY-MM-DD"; break;
            case SIZE_YYMMDD     : ph = "YY-MM-DD";   break;
            case SIZE_YYMM       : ph = "YY-MM";      break;
            default              : ph = "WWDDMMYYYY"; break;
          }
          StrCat(txt, sizeof(_ASCII), ph, sizeof(_ASCII));
        }
        else if(vi.vartype == BIT)
        {
          StrCat(txt, sizeof(_ASCII), "BIT, BITNO=", sizeof(_ASCII));
          DToA(&txt[StrLen(txt, sizeof(_ASCII))], vi.bitno + 1, 0x0102, sizeof(_ASCII));
          StrCat(txt, sizeof(_ASCII), ", INVERS=", sizeof(_ASCII));
          StrCat(txt, sizeof(_ASCII), (VarList.GetInvert(vi.POWPRUIC) == _TRUE)? "TRUE":"FALSE", sizeof(_ASCII));
        }
        else if(vi.vartype == FCONSTANT)
        {
          StrCat(txt, sizeof(_ASCII), "CONSTANT, VALUE=", sizeof(_ASCII));
          DToA(&txt[StrLen(txt, sizeof(_ASCII))], vi.lo_limit.info[0].value, 0x0800, sizeof(_ASCII));
        }
        else if(vi.vartype == FMERKER)
        {
          StrCat(txt, sizeof(_ASCII), "MERKER", sizeof(_ASCII));
        }
        else if(vi.vartype == OBJECT)
        {
          StrCat(txt, sizeof(_ASCII), "OBJECT", sizeof(_ASCII));
        }
        else if(vi.vartype == COMPLEX)
        {
          StrCat(txt, sizeof(_ASCII), "COMPLEX", sizeof(_ASCII));
        }
        else if(vi.vartype == FENUM)
        {
          StrCat(txt, sizeof(_ASCII), "ENUM", sizeof(_ASCII));
        }
        else if(vi.vartype == FENUMITEM)
        {
          StrCat(txt, sizeof(_ASCII), "ENUMITEM", sizeof(_ASCII));
        }

        StrCat(txt, sizeof(_ASCII), ", CRC=0x", sizeof(_ASCII));
        DToH(&txt[StrLen(txt, sizeof(_ASCII))], vi.crc32, 0x0108, sizeof(_ASCII));

        StrCat(txt, sizeof(_ASCII), ", LASALID=0x", sizeof(_ASCII));
        DToH(&txt[StrLen(txt, sizeof(_ASCII))], vi.lasalid.id, 0x0108, sizeof(_ASCII));

       #ifdef UC_MEMBERVARIABLE
        StrCat(txt, sizeof(_ASCII), ", Moff=", sizeof(_ASCII));
        DToA(&txt[StrLen(txt, sizeof(_ASCII))], pvi->memberoffset, 0x0800, sizeof(_ASCII));
       #endif

       #ifdef UC_MULTICPU
        _UWORD imc;
        StrCat(txt, sizeof(_ASCII), " [0x", sizeof(_ASCII));
        for(imc=0; imc<UC_MULTICPU; imc++)
        {
          if(imc != 0)
            StrCat(txt, sizeof(_ASCII), ",0x", sizeof(_ASCII));
          DToH(&txt[StrLen(txt, sizeof(_ASCII))], ((_LSEVARINFO*)pm->pointer.PtrData)->multiid[imc], 0x0108, sizeof(_ASCII));
        }
        StrCat(txt, sizeof(_ASCII), "]", sizeof(_ASCII));
       #endif

      break;
      case 1:
        StrCpy(txt, sizeof(_ASCII), "Station number   :  ", sizeof(_ASCII));
        DToA(&txt[StrLen(txt, sizeof(_ASCII))], vi.station, 0x0103, sizeof(_ASCII));
      break;
      case 2:
        StrCpy(txt, sizeof(_ASCII), "Permanent        :  ", sizeof(_ASCII));
        if((vi.vartype != OBJECT) && (vi.vartype != FCONSTANT) && (vi.vartype != FMERKER) && (vi.vartype != FENUMITEM))
        {
          StrCat(txt, sizeof(_ASCII), (VarList.GetPermanent(vi.POWPRUIC) == _TRUE)? "TRUE":"FALSE", sizeof(_ASCII));
        }
      break;
      case 3:
        StrCpy(txt, sizeof(_ASCII), "Refresh Time     :  ", sizeof(_ASCII));
        if((vi.vartype != OBJECT) && (vi.vartype != FCONSTANT) && (vi.vartype != FMERKER) && (vi.vartype != FENUMITEM))
        {
          ph = "Low";
          if(vi.reftime <= 10)
            ph = "High";
          else if(vi.reftime <= 50)
            ph = "Medium";
          else if(vi.reftime <= 100)
            ph = "Standard";

          StrCat(txt, sizeof(_ASCII), ph, sizeof(_ASCII));
        }
      break;
      case 4:
        StrCpy(txt, sizeof(_ASCII), "Accessnumber     :  ", sizeof(_ASCII));
        if((vi.vartype != OBJECT) && (vi.vartype != FCONSTANT) && (vi.vartype != COMPLEX) && (vi.vartype != FMERKER) && (vi.vartype != FENUMITEM))
        {
          DToA(&txt[StrLen(txt, sizeof(_ASCII))], vi.accessno, 0x0800, sizeof(_ASCII));
        }
      break;
      case 5:
        StrCpy(txt, sizeof(_ASCII), "Physical meaning :  ", sizeof(_ASCII));
        if((vi.vartype != OBJECT) && (vi.vartype != COMPLEX))
        {
          DToA(&txt[StrLen(txt, sizeof(_ASCII))], vi.physic, 0x0800, sizeof(_ASCII));
        }
      break;
      case 6:
        StrCpy(txt, sizeof(_ASCII), "Hostname         :  ", sizeof(_ASCII));
        if((vi.vartype != OBJECT) && (vi.vartype != COMPLEX))
        {
          pascii = VarList.GetHostNameByCrc(vi.crc32);
          if(pascii != NULL)
            StrCat(&txt[StrLen(txt, sizeof(_ASCII))], sizeof(_ASCII), pascii, sizeof(_ASCII));
        }
      break;
      case 7:
        StrCpy(txt, sizeof(_ASCII), "Leading 0        :  ", sizeof(_ASCII));
        if(vi.vartype == BINARY)
        {
          if(VarList.GetZero(pvi->POWPRUIC) == _TRUE)
            StrCat(txt, sizeof(_ASCII), "TRUE", sizeof(_ASCII));
          else
            StrCat(txt, sizeof(_ASCII), "FALSE", sizeof(_ASCII));
        }
      break;
      case 8:
        StrCpy(txt, sizeof(_ASCII), "Lower limit      :  ", sizeof(_ASCII));
        if((vi.vartype == BINARY)||(vi.vartype == FENUMITEM))
        {
          do_limit(txt, &vi.lo_limit);
        }
      break;
      case 9:
        StrCpy(txt, sizeof(_ASCII), "Upper limit      :  ", sizeof(_ASCII));
        if((vi.vartype == BINARY)||(vi.vartype == FENUMITEM))
        {
          do_limit(txt, (vi.vartype == BINARY)? &vi.hi_limit : &vi.lo_limit);
        }
      break;
      case 10:
        StrCpy(txt, sizeof(_ASCII), "Unit             :  ", sizeof(_ASCII));
        if((vi.vartype == BINARY) || (vi.vartype == FCONSTANT))
        {
          pchar = VarList.GetUnitText(vi.funit);
          if(pchar != NULL)
            StrCat(txt, sizeof(_ASCII), pchar, sizeof(_CHAR));
        }
      break;
      case 11:
        StrCpy(txt, sizeof(_ASCII), "Unit scheme      :  ", sizeof(_ASCII));
        if((vi.vartype == BINARY) || (vi.vartype == FCONSTANT))
        {
          pascii = UnitScheme.LseLabel.GetText(vi.funitscheme);
          if(pascii != NULL)
            StrCat(txt, sizeof(_ASCII), pascii, sizeof(_ASCII));
        }
      break;
      case 12:
        StrCpy(txt, sizeof(_ASCII), "Menu             :  ", sizeof(_ASCII));
        pascii = MenuList.LseLabel.GetText(vi.uimenuindex);
        if(pascii != NULL)
          StrCat(txt, sizeof(_ASCII), pascii, sizeof(_ASCII));
      break;
      case 13:
        StrCpy(txt, sizeof(_ASCII), "Vartext1         :  ", sizeof(_ASCII));
        do_vartext(txt, &vi.text1);
      break;
      case 14:
        StrCpy(txt, sizeof(_ASCII), "Vartext2         :  ", sizeof(_ASCII));
        do_vartext(txt, &vi.text2);
      break;
      case 15:
        StrCpy(txt, sizeof(_ASCII), "Vartext3         :  ", sizeof(_ASCII));
        do_vartext(txt, &vi.text3);
      break;
      case 16:
        StrCpy(txt, sizeof(_ASCII), "Bubblehelptext   :  ", sizeof(_ASCII));
        do_vartext(txt, &vi.text4);
      break;
      case 17:
        StrCpy(txt, sizeof(_ASCII), "State scheme     :  ", sizeof(_ASCII));
        pascii = StateScheme.LseLabel.GetText(vi.stscheme);
        if(pascii != NULL)
          StrCat(txt, sizeof(_ASCII), pascii, sizeof(_ASCII));
      break;
      case 18:
        StrCpy(txt, sizeof(_ASCII), "Userconfig       :  ", sizeof(_ASCII));
        DToA(&txt[StrLen(txt, sizeof(_ASCII))], vi.userconfig, 0x0800, sizeof(_ASCII));
      break;
      case 19:
        StrCpy(txt, sizeof(_ASCII), "Group            :  0x", sizeof(_ASCII));
        DToH(&txt[StrLen(txt, sizeof(_ASCII))], vi.statistic.group, 0x0008, sizeof(_ASCII));
      break;
      case 20:
        StrCpy(txt, sizeof(_ASCII), "Parameter        :  0x", sizeof(_ASCII));
        DToH(&txt[StrLen(txt, sizeof(_ASCII))], vi.statistic.parameter, 0x0008, sizeof(_ASCII));
      break;
      case 21:
        StrCpy(txt, sizeof(_ASCII), "Checkbit         :  ", sizeof(_ASCII));
        pascii = Sequencer.LseLabel.GetText(vi.xchkbit.seqno);
        if(pascii != NULL)
        {
          StrCat(txt, sizeof(_ASCII), pascii, sizeof(_ASCII));
          if(*pascii != 0)
          {
            StrCat(txt, sizeof(_ASCII), " - INVERS = ", sizeof(_ASCII));
            if(vi.xchkbit.invert == _TRUE)
              StrCat(txt, sizeof(_ASCII), "TRUE", sizeof(_ASCII));
            else
              StrCat(txt, sizeof(_ASCII), "FALSE", sizeof(_ASCII));
          }
        }
      break;
     #ifdef UC_LINKVL
      case 22:
        StrCpy(txt, sizeof(_ASCII), "Link1            :  ", sizeof(_ASCII));
        do_linkvl(txt, &vi.linkvl[0]);
      break;
      case 23:
        StrCpy(txt, sizeof(_ASCII), "Link2            :  ", sizeof(_ASCII));
        do_linkvl(txt, &vi.linkvl[1]);
      break;
      case 24:
        StrCpy(txt, sizeof(_ASCII), "Link3            :  ", sizeof(_ASCII));
        do_linkvl(txt, &vi.linkvl[2]);
      break;
      case 25:
        StrCpy(txt, sizeof(_ASCII), "Link4            :  ", sizeof(_ASCII));
        do_linkvl(txt, &vi.linkvl[3]);
      break;
     #endif

    }

    y = (r->xy1.y + r->xy2.y) / 2;
    attrib = T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND;

    OutTextXY(r->xy1.x, y, txt, attrib, col, sizeof(_ASCII));

    free_VARINFO(&vi);
  }
}
#endif

void popup_varinfo_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII       label[300];
  const _ASCII *pa;
  _UBYTE       obj;
  _LSEVARINFO  *vi;
  _KOORD       y;
  _MENU        *pm;
  _VARBLOCK    *pv;
  _UWORD       len;
//  _AsciiPuffer *Ap;

  pm = Pp->GetMenu();
  pv = (_VARBLOCK*)pm->pointer.PtrData;

  DToA(label, no, 0x0106, sizeof(_ASCII));
  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

//  if(no < pv->MemLseVarInfo.no)
//  {
//    vi = &pv->MemLseVarInfo.ptr[no];
  if(VarList.GetVarInfo(&vi, no) == _TRUE)
  {
  
    if((vi->vartype == FCONSTANT)||(vi->vartype == FMERKER)||(vi->vartype == FENUMITEM))
    {
      col = MakeColor_Makro(GetBackColor_Makro(col), GREEN);
      StrCat(label, sizeof(_ASCII), (pv->MemVarSymbol.ptr[no].scope == DEFSCOPE)? " INTERN ":" ", sizeof(_ASCII));
    }
    else
    {
      if(text_station(label, vi, pv->MemVarSymbol.ptr[no].scope) == _FALSE)
      {
        col = MakeColor_Makro(GetBackColor_Makro(col), RED); //LIGHTRED);
      }
      else if((vi->POWPRUIC & VARBIT_VARMEMBER) != 0)
      {
        col = MakeColor_Makro(GetBackColor_Makro(col), MAGENTA);
      }
      else if((vi->POWPRUIC & VARBIT_VARGLOBAL) != 0)
      {
        col = MakeColor_Makro(GetBackColor_Makro(col), BLUE);
      }
    }  
        
   #ifdef UC_GRAYSCALE
    transform_color_to_gray((_UWORD*)&col);
   #endif
        
    VarList.GetLabelVARBLOCK(&label[StrLen(label, sizeof(_ASCII))], &obj, no, pv);
    len = StrLen(label, sizeof(_ASCII));
    label[len] = ' ';

    switch(vi->vartype)
    {
      case BINARY    : pa = "BINARY";   break;
      case FSTRING   : pa = "STRING";   break;
      case FTIME     : pa = "TIME";     break;
      case FDATE     : pa = "DATE";     break;
      case BIT       : pa = "BIT";      break;
      case COMPLEX   : pa = "COMPLEX";  break;
      case FREE      : pa = "FREE";     break;
      case FCONSTANT : pa = "CONST";    break;
      case FMERKER   : pa = "MERKER";   break;
      case UNUSED    : pa = "UNUSED";   break;
      case OBJECT    : pa = "OBJECT";   break;
      case FENUM     : pa = "ENUM";     break;
      case FENUMITEM : pa = "ENUMITEM"; break;
      default        : pa = "DEFAULT";  break;
    }

    StrCpy(&label[len+1], sizeof(_ASCII), pa, sizeof(_ASCII));

    if((vi->POWPRUIC & (VARBIT_VARGLOBAL|VARBIT_VARMEMBER)) != 0)
    {
      StrCat(label, 1, " Addr=0x", 1);
      len = StrLen(label, sizeof(_ASCII));
      DToH(&label[len], vi->lasalid.id, 0x0800, sizeof(_ASCII)); // adresse von variable ausgeben
      
      if(vi->vartype == BIT)
      {
        StrCat(label, 1, " >>", 1);
        len = StrLen(label, sizeof(_ASCII));
        DToA(&label[len], vi->reftime_bitno & 0x1F, 0x0800, sizeof(_ASCII)); // bit shift 
      }
    }
  }

  y = (r->xy1.y + r->xy2.y) / 2;
  OutTextXY(r->xy1.x, y, label, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
}

void jump_to_server(_MEM_VKLIST *pm, _POSITION *pos, _UDWORD varno)
{
  _UDWORD i, no;
  _VKLIST *pvk;

  pvk = pm->ptr;
  for(i=0; i<pm->no; i++)
  {
    if(pvk->data == varno)
    {
      no = find_line(pm, i);
      if(no < pm->no)
      {
        pos->no    = pm->visual_no;  // insert no of lines
        pos->pos   = no;
        pos->begin = no - (pos->height / 2);
        if(pos->begin < 0)
	        pos->begin = 0;

        pos->oldbegin = pos->begin + 100;
//        pos->oldpos   = pos->pos;
      }
      return;
    }
    pvk += 1;
  }
}

void popup_varinfo_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _UDWORD     no;
  _VARIABLE   var;
  _COMPORT    *cp;
  _MENU       *pm;
  _CHAR       txt[30];
  _IVALUE     value;
  _TIME       time;
  _DATE       date;
  _BOOL       state;
  _MEM_VKLIST *pmvk;
  _RESULT     result;

  seek_run_varlist(pev, PopUp);

  if(pev->ftype == _EVENT_KEYPRESS)
  {
    pm = PopUp->GetMenu();

    switch(pev->scancode)
    {
     #ifdef UC_IPC
      case _F1:
        menuline = popup_get_actual_position(PopUp);
        Window->AddCmd(CMD_NEWPOPUP, UC_VAR_DIAG, 0, pev->lrm);
        pev->ftype = _EVENT_NONE;   // eat event
        break;
     #endif

      case '/':
      case '*':
        pmvk = (_MEM_VKLIST*)pm->pointer.PtrSubList;
        no   = popup_get_actual_position(PopUp);
        if(pev->scancode == '*')
          open_entire_MEM_VKLIST(pmvk);
        else
          close_entire_MEM_VKLIST(pmvk);

        pm->position.no = pmvk->visual_no;  // correct no of lines
        jump_to_server(pmvk, &pm->position, no);
        break;

      case _ENTER:
        no   = popup_get_actual_position(PopUp);

        txt[0] = '{';
        txt[1] = '?';
        txt[2] = 0;

        if(is_action_MEM_VKLIST((_MEM_VKLIST*)pm->pointer.PtrSubList, pm->position.pos) == _FALSE)
        {
          if(VarList.GetSingleVariable(&var, no) == _TRUE)
          {
            if((var.info.vartype == BINARY) || (var.info.vartype == BIT) ||
               (var.info.vartype == FTIME)  || (var.info.vartype == FDATE) ||
               (var.info.vartype == FENUM))
            {
              state = _FALSE;
              if(chk_LASALID(&var.info.lasalid) == _TRUE)
              {
                if(var.info.POWPRUIC & (VARBIT_VARGLOBAL|VARBIT_VARMEMBER))
                {
                  value = 0;
                  state = get_global_data(&value, &var.info);
                  if(format_BINARY(&result, &var.info, &value) == _TRUE)
                  {
                    value = result.value;
                  }
                  
                }
                else
                {
                  cp = Batch.Get_COMPORT(var.info.station);
                 #ifdef UC_EXOONLINE
                  state = get_direct_PLC_sub((void*)&value, &var.info.lasalid, cp, 4, (_SVRTYP)var.info.exoinfo);
                 #else
                  state = get_direct_PLC_sub((void*)&value, &var.info.lasalid, cp, 4, DEFSVRTYP);
                 #endif

                 #ifdef UC_COMPLEX_SERVER
                  if(var.info.POWPRUIC & VARBIT_STRUCTELEMENT)
                  {
                    if(state == _TRUE)
                    {
                      state = format_BINARY(&result, &var.info, &value);
                      if(state == _TRUE)
                      {
                        value = result.value;
                      }
                    }
                  }
                 #endif
                }
              }

              if(state == _TRUE)
              {
                if(var.info.vartype == FTIME)
                {
                  lasal_to_TIME(&time, value);
                  format_time(&txt[1], &time, SIZE_HHMMSS);
                }
                else if(var.info.vartype == FDATE)
                {
                  lasal_to_DATE(&date, value);
                  format_date(&txt[1], &date, SIZE_DDMMYYYY);
                }
                else 
                {
                  if((var.info.vartype == BINARY) && (var.info.format == SIZE_REAL))
                  {
                    value = (_DWORD)*(_REAL*)&value;
                  }
                  else if(var.info.vartype == BIT)
                  {
                    value = (value & (1 << var.info.bitno))? 1 : 0;
                  }

                  DToA(&txt[1], value, 0x0800, sizeof(_CHAR));
                }
              }

              StrCat(txt, sizeof(_CHAR), "}", sizeof(_ASCII));
              out_runtext(PopUp, txt, sizeof(_CHAR));
              pev->ftype = _EVENT_NONE;   // eat event
            }
            else if((var.info.vartype == FCONSTANT)||(var.info.vartype == FMERKER)||(var.info.vartype == FENUMITEM))
            {
              _RESULT   res;
              _VARLABEL vl;
              init_VARLABEL(&vl);
              init_RESULT(&res);
              vl.no = 1;
              vl.info[0].state = VAR_VAL;
              vl.info[0].value = var.no;
              if(VarList.GetSystemData(&res, &var, &vl, _TRUE, _FALSE) == _TRUE)
              {
                DToA(&txt[1], res.value, 0x0800, sizeof(_CHAR));
                StrCat(txt, sizeof(_CHAR), "}", sizeof(_ASCII));
                out_runtext(PopUp, txt, sizeof(_CHAR));
                pev->ftype = _EVENT_NONE;   // eat event
              }
            }
          }
        }
        break;
/*
      case _F1:
      {
        init_EVENT_Makro(pev);

        if(File.Open(SAVE, "RAM@c:\\unused.txt") == _TRUE)
        {
          for(i=0; i<DiagVarList.no; i++)
          {
            no = DiagVarList.ptr[i].data;

            if(VarList.GetVarInfo(&vi, no) == _TRUE)
            {
              label[0] = 0;
              if(text_station(label, vi, pv->MemVarSymbol.ptr[no].scope) == _FALSE)
              {
                VarList.GetLabelVARBLOCK(&label[StrLen(label, sizeof(_ASCII))], &obj, no, pv);

                len = StrLen(label, sizeof(_ASCII));
                label[len] = ' ';

                switch(vi->vartype)
                {
                  case BINARY :   StrCpy(&label[len+1], 1, "BINARY", 1); break;
                  case FSTRING :  StrCpy(&label[len+1], 1, "STRING", 1); break;     // string
                  case FTIME :    StrCpy(&label[len+1], 1, "TIME", 1); break;    // time
                  case FDATE :    StrCpy(&label[len+1], 1, "DATE", 1); break;     // date
                  case BIT :      StrCpy(&label[len+1], 1, "BIT", 1); break;     // bit datatype
                  case COMPLEX :  StrCpy(&label[len+1], 1, "COMPLEX", 1); break;     // structure, complex
                  case ELEMENT :  StrCpy(&label[len+1], 1, "ELEMENT", 1); break;     // element of a structure
                  case FREE :     StrCpy(&label[len+1], 1, "FREE", 1); break;     // nc
                  case FCONSTANT: StrCpy(&label[len+1], 1, "CONST", 1); break;    // constant type
                  case UNUSED :   StrCpy(&label[len+1], 1, "UNUSED", 1); break;     // nc
                  case OBJECT :   StrCpy(&label[len+1], 1, "OBJECT", 1); break;     // objecttype
                  default    :    StrCpy(&label[len+1], 1, "DEFAULT", 1); break;
                }

                len = StrLen(label, sizeof(_ASCII));
                label[len] = ' ';
                ap = VarList.GetLabelClass();
                if(vi->classid < ap->GetNo())
                  StrCpy(&label[len+1], sizeof(_ASCII), ap->GetText(vi->classid), sizeof(_ASCII));
                else
                  DToA(&label[len+1], vi->classid, 0x0800, sizeof(_ASCII));

                len = StrLen(label, sizeof(_ASCII));
                label[len] = 13;
                label[len+1] = 10;
                label[len+2] = 0;
                File.Write(label, StrLen(label, sizeof(_ASCII)));
              }
            }

          }

          File.Close();
        }
      }
*/
    }
  }
}

void VARLABEL_to_text(_ASCII *dest, _VARLABEL *vl)
{
  _UBYTE varobj;

  if(chk_VARLABEL(vl) == _TRUE)
  {
    switch(vl->info[0].state)
    {
      case VAR_VAL   : VarList.GetLabel(dest, &varobj, vl->info[0].value);
                       break;
      case CONST_VAL : DToA(dest, vl->info[0].value, 0x0800, sizeof(_ASCII));
                       break;
      default        : StrCpy(dest, sizeof(_ASCII), "???", sizeof(_ASCII));
                       break;
    }
  }
  else
  {
    StrCat(dest, sizeof(_ASCII), "-no link-", sizeof(_ASCII));
  }
}

void popup_function_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII txt[100];

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < InterBoxList.GetNo())
  {
    DToA(txt, no, 0x0105, sizeof(_ASCII));
   #if COMPILER_VERSION >= 33
    do_lselabel(txt, &InterBoxList.LseLabel, no, ' ', 0);
   #endif
    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
}

void popup_function_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _UWORD no;

  seek_run(pev, &InterBoxList.LseLabel, PopUp);

  if((pev->ftype == _EVENT_KEYPRESS) && (pev->scancode == _ENTER))
  {
    no = PopUp->GetMenu()->position.pos;
    if(no < InterBoxList.GetNo())
    {
//      Window->AddCmd(CMD_NEWPOPUP, UC_FKT_DIAG+no, 0);
      menuline = no;
      Window->AddCmd(CMD_NEWPOPUP, UC_FKT_DIAG, 0, pev->lrm);
      pev->ftype = _EVENT_NONE;   // eat event
    }
  }
}

void format_STACK(_ASCII *txt, _STACK *p, _UWORD no)
{
  const _ASCII *pt;

  if(p != NULL)
  {
    if(no < p->no)
    {
      pt = NULL;
      switch(p->ptr[no].cmd)
      {
        case CMD_NEWPICTURE       : pt = "SCREEN "; break;  // open new picture (2)
        case CMD_NEWWINDOW        : pt = "WINDOW "; break;  // open new window (1)
        case CMD_NEWSOFTKEY       : pt = "FLOOR "; break;  // change to defined floor and draw (1)
        case CMD_NEWLANGUAGE      : pt = "LANGUAGE "; break;  // change to other language (1)
        case CMD_EDIT             : pt = "EDIT "; break;  // seek input and open editor (1)
        case CMD_KILLEDITOR       : pt = "KILL EDITOR "; break;  // kill editor without save (0)
        case CMD_SAVEEDITOR       : pt = "SAVE EDITOR "; break;  // save editor (0)
        case CMD_RETURN           : pt = "RETURN "; break;  // return to previous (0)
        case CMD_SCANCODE         : pt = "SCANCODE "; break;  // set scancode for keyboard (1)
        case CMD_CALL             : pt = "USERCALL "; break;  // call individual userfunction (2)
        case CMD_BEEP             : pt = "BEEP "; break;  // signal beep (1)
        case CMD_ADD              : pt = "ADD "; break;  // add value to variable
        case CMD_SUB              : pt = "SUB "; break;  // sub value from variable
        case CMD_INC              : pt = "INC "; break;  // value ++
        case CMD_DEC              : pt = "DEC "; break;  // value --
        case CMD_SET              : pt = "SET "; break;  // set variable
        case CMD_TOGGLE           : pt = "TOGGLE "; break;  // 0-1-0
        case CMD_IFE              : pt = "IF == "; break;  // if ==
        case CMD_IFG              : pt = "IF > "; break;  // if >
        case CMD_IFS              : pt = "IF < "; break;  // if <
        case CMD_ELSE             : pt = "ELSE "; break;  // else
        case CMD_END              : pt = "END "; break;  // end
        case CMD_EXITPROGRAM      : pt = "EXIT "; break;  // exit application
        case CMD_NEWPOPUP         : pt = "MENUe "; break;  // open popupmenu
        case CMD_CALL_ALARM       : pt = "ALARM "; break;  // open alarmwindow
        case CMD_OVERLOAD         : pt = "OVERLOAD "; break;  // overload (1)
        case CMD_REDRAWTEXT       : pt = "REDRAW "; break;  // redraw all text and io's
        case CMD_SCRNSCHEME       : pt = "SCREEN BY SCHEME "; break;  // open screen via scheme
        case CMD_DIAGNOSTIC       : pt = "DIAGNOSTIC "; break;  // open diagnostic window (0)
        case CMD_OVERLOAD_OBJ     : pt = "OVERLOAD OBJECT "; break;  // overload just defined object (2)
        case CMD_OVERLOAD_SUMMARY : pt = "OVERLOAD SUMMARY "; break; // overload object with defined summary (2)
        case CMD_LED              : pt = "LED "; break;// switch led on/off/blink (2)
        case CMD_FUNCTIONBLOCK    : pt = "FUNCTIONBLOCK "; break;// call functionblock (1)
        case CMD_EDITSERVER_HID   : pt = "EDIT SERVER HID "; break;// call toucheditorwindow to edit spezified server (2)
        case CMD_SWITCH_MULTICPU  : pt = "SWITCH MULTICPU "; break;// switch to substation 255[1] (1)
        default                   : break;
      }

      if(pt != NULL)
      {
        StrCat(txt, sizeof(_ASCII), pt, sizeof(_ASCII));
        VARLABEL_to_text(&txt[StrLen(txt, sizeof(_ASCII))], &p->ptr[no].op1);
        StrCat(txt, sizeof(_ASCII), ", ", sizeof(_ASCII));
        VARLABEL_to_text(&txt[StrLen(txt, sizeof(_ASCII))], &p->ptr[no].op2);
      }
      else
      {
        StrCat(txt, sizeof(_ASCII), "???", sizeof(_ASCII));
      }
    }
  }
}

void popup_stack_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _MENU   *pm     = Pp->GetMenu();
  _Stack  *Stack = (_Stack*)pm->pointer.PtrData;
  _ASCII  txt[150];

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < Stack->GetNo())
  {
    DToA(txt, no, 0x0102, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), ": ", sizeof(_ASCII));
    format_STACK(&txt[StrLen(txt, sizeof(_ASCII))], Stack->GetStack(), no);
    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
}

_BOOL stack_call(_MENU *Menu, _Stack *Stack)
{
  _BOOL  retcode = _FALSE;
  _UWORD height, lines;

  diagnostic_call(Menu, 1, _FALSE);

  if(Stack != NULL)
  {
    Menu->info.width  = (Menu->info.width * 2) / 3;
    height            = (Menu->info.height * 2) / 3;
    lines = Stack->GetNo();
    if(lines < height)
      height = lines;
    Menu->info.height = height;

    Menu->pointer.PtrLine = popup_stack_line;    // --> to linedraw function
    Menu->pointer.PtrData = Stack;
    Menu->position.no     = Stack->GetNo();
    retcode = _TRUE;
  }

  return(retcode);
}

void popup_unit_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _MENU     *pm     = Pp->GetMenu();
  _ASCII    *txt, tmp[60];
  _CHAR     *pt;
  _UNIT     *pu;
  _RESULT   result;
  _VARIABLE variable;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no == pm->position.begin)
  {
    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, "No    Dig Dez Formula Fix Lead0 Text   ", T_UNDERLINE|T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
  else
  {
    no -= 1;
    pu = VarList.GetUnitInfo(no);
    if(pu != NULL)
    {
      txt = tmp;
      DToA(txt, no, 0x0105, sizeof(_ASCII));
      txt += StrLen(txt, sizeof(_ASCII)); *txt = ' '; txt += 1;

      VarList.GetSystemData(&result, &variable, &pu->xdigits, _TRUE, _TRUE);
      DToA(txt, result.value, 0x0002, sizeof(_ASCII));
      txt += StrLen(txt, sizeof(_ASCII)); *txt = ' '; txt += 1;

      VarList.GetSystemData(&result, &variable, &pu->posdezpt, _TRUE, _TRUE);
      DToA(txt, result.value, 0x0003, sizeof(_ASCII));
      txt += StrLen(txt, sizeof(_ASCII)); *txt = ' '; txt += 1;

      DToA(txt, pu->formula, 0x0006, sizeof(_ASCII));
      txt += StrLen(txt, sizeof(_ASCII)); *txt = ' '; txt += 1;

      DToA(txt, pu->fixdp, 0x0004, sizeof(_ASCII));
      txt += StrLen(txt, sizeof(_ASCII)); *txt = ' '; txt += 1;
      
      if(pu->leading0 == 0)
        StrCat(txt, 1, " def   ", 1);
      else if(pu->leading0 == 1)
        StrCat(txt, 1, " true  ", 1);
      else
        StrCat(txt, 1, " false ", 1);
      txt += StrLen(txt, sizeof(_ASCII)); 
      

      pt = VarList.GetUnitText(no);
      if(pt != NULL)
        StrCpy(txt, sizeof(_ASCII), pt, sizeof(_CHAR));

     #if COMPILER_VERSION >= 33
      do_lselabel(txt, VarList.GetLabelUnit(), no, ' ', 0);
     #endif

      OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, tmp, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
    }
  }
}

void popup_image_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _BMP   *bmp;
  _ROOM  room;
  _ASCII txt[100];

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  bmp = ImageList.Get(no);

  if(bmp != NULL)
  {
    DToA(txt, no, 0x0105, sizeof(_ASCII));
    room = *r;
    OutTextXY(room.xy1.x, (room.xy1.y+room.xy2.y)/2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));

//    out_bmp(r, no, txt, sizeof(_ASCII));

    if((bmp->height != 0) && (bmp->width != 0))
    {
      room.xy1.x += 60;
      room.xy2.x += 60;
      room.xy2.x  = room.xy1.x + (bmp->width * (room.xy2.y - room.xy1.y + 1))/bmp->height;
      out_zoom_BMP(&room, bmp, T_COPY|T_SOLID);

      StrCpy(txt, sizeof(_ASCII), "(", sizeof(_ASCII));
     #if COMPILER_VERSION >= 33
      do_lselabel(txt, &ImageList.LseLabel, no, 0, ':');
     #endif
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], bmp->width, 0x0800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), "*", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], bmp->height, 0x0800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ", byte:", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], bmp->datano*3, 0x0800, sizeof(_ASCII));

      if((bmp->state & BMP_NO_TRANSPARENT) == 0)
      {
        StrCat(txt, sizeof(_ASCII), ", transparent:", sizeof(_ASCII));
        DToA(&txt[StrLen(txt, sizeof(_ASCII))], bmp->transparent, 0x0800, sizeof(_ASCII));
      }

      StrCat(txt, sizeof(_ASCII), ")", sizeof(_ASCII));
      OutTextXY(room.xy2.x+10, (room.xy1.y+room.xy2.y)/2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
    }
  }
}

void popup_image_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
 #ifdef UC_IPC
  _UWORD no;

  seek_run(pev, &ImageList.LseLabel, PopUp);

  if((pev->ftype == _EVENT_KEYPRESS) && (pev->scancode == _ENTER))
  {
    no = PopUp->GetMenu()->position.pos;
    if(no < ImageList.GetNo())
    {
      menuline = no;
      Window->AddCmd(CMD_NEWPOPUP, UC_IMAGE_DIAG, 0, pev->lrm);
      pev->ftype = _EVENT_NONE;   // eat event
    }
  }

 #endif
}

#ifdef UC_IPC


_BOOL prepaire_image_call(_MENU *Menu, _BMP *bmp)
{
  _ROOM room;
  _UDWORD xx, yy, m1, d1, m2, d2;

  if(bmp != NULL)
  {
    MyGetScreenSize(&room);
    diagnostic_call(Menu, 0xFFFF, _FALSE);

    xx = (room.xy2.x - room.xy1.x + 1) - 20;
    yy = (room.xy2.y - room.xy1.y + 1) - 20 - SetUp.GetSoftkeyHeight();
    m1 = 1;
    d1 = 1;
    m2 = 1;
    d2 = 1;

    if(bmp->width > xx)
    {
      m1 = xx;
      if(bmp->width > 0)
        d1 = bmp->width;
    }

    if(bmp->height > yy)
    {
      m2 = yy;
      if(bmp->height > 0)
        d2 = bmp->height;
    }

    if((20000 * m2/d2) < (20000 * m1/d1))
    {
      m1 = m2;
      d1 = d2;
    }

    Menu->info.width       = (bmp->width * m1) / d1;
    Menu->info.line_height = (bmp->height * m1) / d1;

/*
    if((bmp->width + 20) > (room.xy2.x - room.xy1.x + 1))
      Menu->info.width = room.xy2.x - room.xy1.x + 1 - 20;
    else
      Menu->info.width = bmp->width;

    if((bmp->height + SetUp.GetSoftkeyHeight() + 20) > (room.xy2.y - room.xy1.y + 1))
      Menu->info.line_height = room.xy2.y - room.xy1.y + 1 - SetUp.GetSoftkeyHeight() - 20;
    else
      Menu->info.line_height = bmp->height + 1;
*/

    Menu->info.height = 1;

    Menu->pointer.PtrLine  = popup_imageview_line;    // --> to linedraw function
    Menu->pointer.PtrData  = (void*)bmp;
    Menu->position.no      = 1;

    return(_TRUE);
  }

  return(_FALSE);
}

_BOOL imageview_call(_MENU *Menu, _UDWORD menuline)
{
  return(prepaire_image_call(Menu, ImageList.Get(menuline)));
}

void popup_imagefile_end(_Window *Pw, _PopUp *PopUp)
{
  _BMP *bmp = (_BMP*)PopUp->GetMenu()->pointer.PtrData;

  if(bmp != NULL)
  {
    free_BMP(bmp);
    Memory((void**)&PopUp->GetMenu()->pointer.PtrData, 0);
  }
}

_BOOL imagefile_call(_MENU *Menu, _ASCII *dpne)
{
  _BMP  *bmp = NULL;
  _BOOL retcode = _FALSE;

  if(Memory((void**)&bmp, sizeof(_BMP)) != 0)
  {
    init_BMP_Makro(bmp);
    if(load_gfx(bmp, dpne, NULL, NULL) == _TRUE)
    {
      if(prepaire_image_call(Menu, bmp) == _TRUE)
      {
        Menu->pointer.PtrEnd = popup_imagefile_end;
        retcode = _TRUE;
      }
    }

    if(retcode == _FALSE)
    {
      free_BMP(bmp);
      Memory((void**)&bmp, 0);
    }
  }

  return(retcode);
}

void  popup_imageview_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *pr, _COLOR col, _BOOL background)
{
  _BMP   *bmp = (_BMP*)Pp->GetMenu()->pointer.PtrData;
  _UWORD xx, yy;

  if(bmp != NULL)
  {
    xx = (pr->xy2.x - pr->xy1.x + 1);
    yy = (pr->xy2.y - pr->xy1.y + 1);

    if((bmp->width > xx) || (bmp->height > yy))
    {
      _BMP tmp;
      init_BMP_Makro(&tmp);
      copy_BMP(&tmp, bmp);
      zoom_BMP(&tmp, xx, yy);
      Bitmap(pr->xy1.x, pr->xy1.y, &tmp, T_COPY|T_SOLID, BLACK);
      OutTextXY(pr->xy1.x+10, pr->xy1.y+10, "big image !", T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, MakeColor_Makro(BLACK, WHITE), sizeof(_ASCII));
      free_BMP(&tmp);
    }
    else
    {
      Bitmap(pr->xy1.x, pr->xy1.y, bmp, T_COPY|T_SOLID, BLACK);
    }
  }
}
#endif

void popup_object_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ZOOM       zoom;
  _ROOM       space, room;
  _SubPicture *Sp = NULL;
  _ASCII      txt[150];
  _KOORD      div;
  _DWORD      zx, zy;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(load_object(&Sp, no) == _TRUE)
  {
    DToA(txt, no, 0x0105, sizeof(_ASCII));
    room = *r;
    OutTextXY(room.xy1.x, (room.xy1.y+room.xy2.y)/2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));

    room.xy1.x += 60;
//    room.xy2.x += 60;

    init_ZOOM(&zoom);
    Sp->GetSize(&space);

    zx = 1000;
    zy = 1000;
    div = (space.xy2.y - space.xy1.y); // kein +1 weil es handelt sich um Koordinaten und nicht um höhe oder breite
    if(div > 0)
      zy = ((room.xy2.y - room.xy1.y) * 1000) / div;
    div = (space.xy2.x - space.xy1.x); // kein +1 weil es handelt sich um Koordinaten und nicht um höhe oder breite
    if(div > 0)
      zx = ((room.xy2.x - room.xy1.x + 1) * 1000) / div;

    if(zx > 1000)
      zx = 1000;
    if(zy > 1000)
      zy = 1000;

    if((zx > 0)&&(zy > 0))
    {
      if(zx > zy)
        zx = zy;

      zoom.x = zx;
      zoom.y = zx;

      Sp->Draw(&room.xy1, &zoom, NULL);
      room.xy1.x += (Zoom(zoom.x, Sp->GetPICTURE()->mask.room.xy2.x - Sp->GetPICTURE()->mask.room.xy1.x) + 10);
    }

   #if COMPILER_VERSION >= 33
    txt[0] = 0;
    do_lselabel(txt, &ObjectList.LseLabel, no, 0, 0);
    StrCat(txt, sizeof(_ASCII), " - ", sizeof(_ASCII));

    DToA(&txt[StrLen(txt, sizeof(_ASCII))], Sp->GetPICTURE()->mask.room.xy1.x, 0x0800, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), "/", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], Sp->GetPICTURE()->mask.room.xy1.y, 0x0800, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), ":", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], Sp->GetPICTURE()->mask.room.xy2.x, 0x0800, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), "/", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], Sp->GetPICTURE()->mask.room.xy2.y, 0x0800, sizeof(_ASCII));

//    StrCat(txt, sizeof(_ASCII), " ", sizeof(_ASCII));

//    DToA(&txt[StrLen(txt, sizeof(_ASCII))], Sp->GetPICTURE()->mask.newroom.xy1.x, 0x0800, sizeof(_ASCII));
//    StrCat(txt, sizeof(_ASCII), "/", sizeof(_ASCII));
//    DToA(&txt[StrLen(txt, sizeof(_ASCII))], Sp->GetPICTURE()->mask.newroom.xy1.y, 0x0800, sizeof(_ASCII));
//    StrCat(txt, sizeof(_ASCII), ":", sizeof(_ASCII));
//    DToA(&txt[StrLen(txt, sizeof(_ASCII))], Sp->GetPICTURE()->mask.newroom.xy2.x, 0x0800, sizeof(_ASCII));
//    StrCat(txt, sizeof(_ASCII), "/", sizeof(_ASCII));
//    DToA(&txt[StrLen(txt, sizeof(_ASCII))], Sp->GetPICTURE()->mask.newroom.xy2.y, 0x0800, sizeof(_ASCII));

//    if((Sp->GetPICTURE()->mask.newroom.xy2.y != Sp->GetPICTURE()->mask.room.xy2.y)||
//       (Sp->GetPICTURE()->mask.newroom.xy2.x != Sp->GetPICTURE()->mask.room.xy2.x)||
//       (Sp->GetPICTURE()->mask.newroom.xy1.y != Sp->GetPICTURE()->mask.room.xy1.y)||
//       (Sp->GetPICTURE()->mask.newroom.xy1.x != Sp->GetPICTURE()->mask.room.xy1.x))
//    col = MakeColor_Makro(GetBackColor_Makro(col), LIGHTRED);

    Font.Set(DEFFONT, NULL);
    OutTextXY(room.xy1.x, (room.xy1.y+room.xy2.y)/2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
   #endif

    Sp->Free();
    Memory2((void**)&Sp, 0);
  }
}

void popup_object_run(_Window *Pw, _PopUp *PopUp, _EVENT *pev)
{
  seek_run(pev, &ObjectList.LseLabel, PopUp);
}

void popup_alarminfo_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII        label[300];
  _MENU         *pm;
  _ALARM        *pa;
  _LSEVARINFO   *pvi;
  _SINGLE_ALARM *psi;
  _UBYTE        obj;
  _CHAR         *txt;

  pm = Pp->GetMenu();
  pa = (_ALARM*)pm->pointer.PtrData;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < pa->no)
  {
    psi = &pa->ptr[no];

    DToA(label, no, 0x0105, sizeof(_ASCII));
    StrCat(label, sizeof(_ASCII), " ", sizeof(_ASCII));

   #if COMPILER_VERSION >= 33
    do_lselabel(label, &Alarm.LseLabel, no, 0, ':');
   #endif

	  DToA(&label[StrLen(label, sizeof(_ASCII))], psi->alano, 0x0800, sizeof(_ASCII));
    StrCat(label, sizeof(_ASCII), " ", sizeof(_ASCII));

    if(chk_VARLABEL(&psi->link) == _TRUE)
    {
      if(psi->link.info[0].state == VAR_VAL)
      {
        if(VarList.GetLabel(&label[StrLen(label, sizeof(_ASCII))], &obj ,psi->link.info[0].value) == _TRUE)
        {
	        StrCat(label, sizeof(_ASCII), "  ", sizeof(_ASCII));
          //DToA(&label[StrLen(label, sizeof(_ASCII))], psi->parameter.no, 0x0800, sizeof(_ASCII));
          DToA(&label[StrLen(label, sizeof(_ASCII))], psi->alano, 0x0800, sizeof(_ASCII));
          
          
          
         /*
          StrCat(label, sizeof(_ASCII), " ", sizeof(_ASCII));
          for(i=0; i<psi->parameter.no; i++)
          {
            pp = &psi->parameter.ptr[i];
				    if(pp->no > 0)
            {
              if(pp->info[0].state == PARA_VAL)
              {
                if(VarList.GetLabel(&label[StrLen(label, sizeof(_ASCII))], &obj, pp->info[0].value) == _TRUE)
                {
                  StrCat(label, sizeof(_ASCII), " ", sizeof(_ASCII));
                }
              }
              else if(pp->info[0].state == PARA_CONST)
              {
                StrCat(label, sizeof(_ASCII), " CONST", sizeof(_ASCII));
              }
            }
          }
         */
          txt = get_ANYPARA(&psi->text, NULL);
          if(txt != NULL)
          {
            StrCat(label, sizeof(_ASCII), " ", sizeof(_ASCII));
            StrCat(label, sizeof(_ASCII), (void*)txt, sizeof(_CHAR));
          }

          VarList.GetVarInfo(&pvi, psi->link.info[0].value);
          if(pvi->lasalid.id == 0)
            col = MakeColor_Makro(GetBackColor_Makro(col), LIGHTRED);
        }
      }
    }
    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, label, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
}

void popup_alarm_run(_Window *Pw, _PopUp *PopUp, _EVENT *pev)
{
  seek_run(pev, &Alarm.LseLabel, PopUp);
}

const _ASCII *get_scheme_operator(_UWORD op)
{
  const _ASCII *retcode;

  switch(op)
  {
    case SOP_SMALLEQUAL  : retcode = "<= "; break;
    case SOP_EQUAL       : retcode = "= ";  break;
    case SOP_BIGGEREQUAL : retcode = ">= "; break;
    case SOP_BIGGER      : retcode = "> ";  break;
    case SOP_NOTEQUAL    : retcode = "<>";  break;
    default              : retcode = "< ";  break;
  }

  return(retcode);
}

void popup_scheme_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _MENU         *pm     = Pp->GetMenu();
  _Scheme       *Scheme = (_Scheme*)pm->pointer.PtrData;
  _ASCII        label[300];
  _ASCII        *pl;
  _SCHEME       *pi;
  _KOORD        x;
  _COLOR        bcolor = INVISIBLE;
  _SINGLESCHEME *ps = NULL;
  _UWORD        anz, line, schemeno, j;
  _UDWORD       bimage = 0xFFFFFFFFL;
  _UBYTE        varobj;

  DToA(label, no, 0x0105, sizeof(_ASCII));
  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  anz  = Scheme->GetNo();
  line = 0;

  for(schemeno=0; schemeno<anz; schemeno++)
  {
    line = 0;
    if(no != 0)
    {
      if(Scheme->Get(&pi, schemeno) == _TRUE)
      {
        for(j=0; j<pi->memsinglescheme.no; j++)
        {
          no   --;
          line ++;
          if(no == 0)
            break;
        }
      }
    }

    if(no == 0)
      break;
    else
      no --;
  }

  if(Scheme->Get(&pi, schemeno) == _TRUE)
  {
    if(line == 0)
    {
      if(Scheme == &FontScheme)
        pl = FontScheme.GetLabelByIndex(schemeno);
      else
        pl = Scheme->LseLabel.GetText(schemeno);
      if(pl == NULL)
      {
        StrCat(label, sizeof(_ASCII), " Scheme ", sizeof(_ASCII));
        DToA(&label[StrLen(label, sizeof(_ASCII))], schemeno, 0x0800, sizeof(_ASCII));
        StrCat(label, sizeof(_ASCII), ": ", sizeof(_ASCII));
      }
      else
      {
        StrCat(label, sizeof(_ASCII), " [", sizeof(_ASCII));
        StrCat(label, sizeof(_ASCII), pl, sizeof(_ASCII));
        StrCat(label, sizeof(_ASCII), "]: ", sizeof(_ASCII));
      }

      if(Scheme == &FontScheme)
        StrCat(label, sizeof(_ASCII), "Language", sizeof(_ASCII));
      else
        VARLABEL_to_text(&label[StrLen(label, sizeof(_ASCII))], &pi->link);
    }
    else
    {
      if(Scheme == &FontScheme)
        StrCat(label, sizeof(_ASCII), "   (Language ", sizeof(_ASCII));
      else
        StrCat(label, sizeof(_ASCII), "   (... ", sizeof(_ASCII));

      line --;
      if(line < pi->memsinglescheme.no)
      {
        ps = &pi->memsinglescheme.ptr[line];
        StrCat(label, sizeof(_ASCII), get_scheme_operator(ps->op), sizeof(_ASCII));
        VARLABEL_to_text(&label[StrLen(label, sizeof(_ASCII))], &ps->upto);
        StrCat(label, sizeof(_ASCII), ")? ", sizeof(_ASCII));

        DToA(&label[StrLen(label, sizeof(_ASCII))], ps->value, 0x0800, sizeof(_ASCII));

        if(Scheme == &ColorScheme)
          bcolor = ps->value;
        else if(Scheme == &ImageScheme)
          bimage = ps->value;
        else if(Scheme == &UnitScheme)
        {
          StrCat(label, sizeof(_ASCII), " [", sizeof(_ASCII));
          StrCat(label, sizeof(_ASCII), VarList.GetLabelUnit()->GetText(ps->value), sizeof(_ASCII));
          StrCat(label, sizeof(_ASCII), "]", sizeof(_ASCII));
        }
        else if(Scheme == &FontScheme)
        {
          StrCat(label, sizeof(_ASCII), " [", sizeof(_ASCII));
          StrCat(label, sizeof(_ASCII), Font.LseLabel.GetText(ps->value), sizeof(_ASCII));
          StrCat(label, sizeof(_ASCII), "]", sizeof(_ASCII));
        }
        else if(Scheme == &ServerScheme)
        {
          StrCat(label, sizeof(_ASCII), " [", sizeof(_ASCII));
          VarList.GetLabel(&label[StrLen(label, sizeof(_ASCII))], &varobj, ps->value);
          StrCat(label, sizeof(_ASCII), "]", sizeof(_ASCII));
        }
      }
    }

    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, label, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));

    if(bimage < ImageList.GetNo())
    {
      out_bmp(r, bimage, label, sizeof(_ASCII));
    }

    if((bcolor != INVISIBLE)&&(ps != NULL))
    {
      x = r->xy1.x + StrWidth(label, sizeof(_ASCII)) + 10;
      Button(x, r->xy1.y+1, x + r->xy2.y-r->xy1.y-1, r->xy2.y-1, T_FILL|T_COPY|T_SOLID, MakeColor_Makro(ps->value, ps->value), MakeColor_Makro(GetPenColor_Makro(col), GetPenColor_Makro(col)));
    }
  }
}

/*
void popup_scheme_run(_Window *Pw, _PopUp *PopUp, _EVENT *pev)
{
  _Scheme *Ps = (_Scheme*)PopUp->GetMenu()->pointer.PtrData;
  seek_run(pev, &Ps->LseLabel, PopUp);
}
*/

void popup_textscheme_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _MENU         *pm     = Pp->GetMenu();
  _TextScheme   *Scheme = (_TextScheme*)pm->pointer.PtrData;
  _CHAR         label[300];
  _ASCII        ascii[100];
  _TXTSCHEME    *pi;
  _SINGLETXTSCHEME *ps;
  _UWORD        anz, line, schemeno, j;
  _CHAR         *txt;

  DToA(label, no, 0x0105, sizeof(_CHAR));
  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  anz  = Scheme->GetNo();
  line = 0;

  for(schemeno=0; schemeno<anz; schemeno++)
  {
    line = 0;
    if(no != 0)
    {
      if(Scheme->Get(&pi, schemeno) == _TRUE)
      {
        for(j=0; j<pi->no; j++)
        {
          no   --;
          line ++;
          if(no == 0)
            break;
        }
      }
    }

    if(no == 0)
      break;
    else
      no --;
  }

  if(Scheme->Get(&pi, schemeno) == _TRUE)
  {
    if(line == 0)
    {
      StrCat(label, sizeof(_CHAR), " Scheme ", sizeof(_ASCII));
      DToA(&label[StrLen(label, sizeof(_CHAR))], schemeno, 0x0800, sizeof(_CHAR));
      StrCat(label, sizeof(_CHAR), ":", sizeof(_ASCII));
      OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, label, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_CHAR));
    }
    else
    {
      StrCat(label, sizeof(_CHAR), "   (... ", sizeof(_ASCII));

      line --;
      if(line < pi->no)
      {
        ps = &pi->ptr[line];
        StrCat(label, sizeof(_CHAR), get_scheme_operator(ps->op), sizeof(_ASCII));
        VARLABEL_to_text(ascii, &ps->upto);
        StrCat(label, sizeof(_CHAR), ascii, sizeof(_ASCII));
        StrCat(label, sizeof(_CHAR), ")? ", sizeof(_ASCII));

        txt = get_ANYPARA(&ps->anypara, NULL);
        if(txt != NULL)
          StrCat(label, sizeof(_CHAR), txt, sizeof(_CHAR));

        OutMultiTextAlign(label, 0, r, DEFFONT, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, _DEFFRAME, col, INVISIBLE, INVISIBLE, sizeof(_CHAR));
      }
    }
  }
}

_BOOL textscheme_call(_MENU *Menu, _TextScheme *Scheme)
{
  _TXTSCHEME *pi;
  _UWORD      i;

  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine = popup_textscheme_line;    // --> to linedraw function
  Menu->pointer.PtrData = Scheme;
  Menu->position.no     = 0;

  for(i=0; i<Scheme->GetNo(); i++)
  {
    if(Scheme->Get(&pi, i) == _TRUE)
      Menu->position.no += pi->no + 1;
  }

  return(_TRUE);
}

_BOOL scheme_call(_MENU *Menu, _Scheme *Scheme)
{
  _SCHEME *pi;
  _UWORD  i;

  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine = popup_scheme_line;    // --> to linedraw function
//  Menu->pointer.PtrRun  = popup_scheme_run;     // --> to run function
  Menu->pointer.PtrData = Scheme;
  Menu->position.no     = 0;

  for(i=0; i<Scheme->GetNo(); i++)
  {
    if(Scheme->Get(&pi, i) == _TRUE)
      Menu->position.no += pi->memsinglescheme.no + 1;
  }

  return(_TRUE);
}

void popup_font_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _KOORD y;
  _UWORD xx, yy;
  _ASCII txt[150];
  _ASCII *tmp;
  _ATTRIB attrib;
  _FONT  font;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < Font.GetNo())
  {
    DToA(txt, no, 0x0105, sizeof(_ASCII));
    txt[5] = ' ';
    txt[6] = 0;
    font   = no;

   #if COMPILER_VERSION >= 33
    do_lselabel(txt, &Font.LseLabel, no, 0, ':');
   #endif
    tmp = Font.GetFileName(no);
    if(tmp != NULL)
    {
      StrCat(txt, sizeof(_ASCII), tmp, sizeof(_ASCII));
    }

    StrCat(txt, sizeof(_ASCII), " Byte:", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], Font.GetMemorySizeDirect(no), 0x0800, sizeof(_ASCII));
    Font.SizeDirect(no, &xx, &yy);
    StrCat(txt, sizeof(_ASCII), ",Height:", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], yy, 0x0800, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), " ", sizeof(_ASCII));

//    attrib = Font.GetAttrib(no);
//    if(attrib & T_PROP)
//      StrCat(txt, sizeof(_ASCII), "PROP|", sizeof(_ASCII));
//    if(attrib & T_BOLD)
//      StrCat(txt, sizeof(_ASCII), "BOLD|", sizeof(_ASCII));
//    if(attrib & T_ITALIC)
//      StrCat(txt, sizeof(_ASCII), "ITALIC|", sizeof(_ASCII));
//    if(attrib & T_UNDERLINE)
//      StrCat(txt, sizeof(_ASCII), "UNDRLNE|", sizeof(_ASCII));
//    if(attrib & T_STRIKEOUT)
//      StrCat(txt, sizeof(_ASCII), "STRKOUT|", sizeof(_ASCII));

    y = (r->xy1.y + r->xy2.y) / 2;
    OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));

    xx = StrWidth(txt, sizeof(_ASCII));
    attrib = T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND;
    Font.SetDirect(font, &attrib);
    OutTextLse(r->xy1.x + xx, y, (_ASCII*)"{1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ}", attrib, col, sizeof(_ASCII));
  }
}

void popup_font_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _UWORD no;

  if(pev->ftype == _EVENT_KEYPRESS)
  {
    no = PopUp->GetMenu()->position.pos;
    if(no < Font.GetNo())
    {
      if(pev->scancode == _ENTER)
      {
//        Window->AddCmd(CMD_NEWPOPUP, UC_FONT_DIAG+no, 0);
        menuline = no;
        Window->AddCmd(CMD_NEWPOPUP, UC_FONT_DIAG, 0, pev->lrm);
        pev->ftype = _EVENT_NONE;   // eat event
      }
    }
  }
}

void popup_fontview_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _MENU   *pm;
  _CHAR   *p, txt[100];
  _KOORD  y;
  _UDWORD start, i;
  _ATTRIB attrib;

  attrib = T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND;      // original
  pm     = Pp->GetMenu();

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  Font.SetDirect((_UDWORD)(_UWORD*)pm->pointer.PtrData, &attrib);

  start = no * CHARACTER_PER_LINE;
  DToH(txt, start, 0x0104, sizeof(_CHAR));
  p = &txt[StrLen(txt, sizeof(_CHAR))];
  *p = ' ';  p += 1;

  for(i=0; i<CHARACTER_PER_LINE; i++, start++, p++)
  {
    *p = (start == 0)? 1 : start;
  }
  
  *p = 0;

  y = (r->xy1.y + r->xy2.y) / 2;

 #ifdef FONTMARK
  if(FontMark.GetState(no) == _FALSE)
    col = MakeColor_Makro(GetBackColor_Makro(col), LIGHTRED); 
 #endif

  OutTextLse(r->xy1.x, y, txt, attrib, col, sizeof(_CHAR));  // original

/*
  start = (cploffset + no) * CHARACTER_PER_LINE;
  DToH(txt, start, 0x0104, sizeof(_CHAR));
  p = &txt[StrLen(txt, sizeof(_CHAR))];
  *p = ' ';  p += 1;

  for(i=0; i<CHARACTER_PER_LINE; i++)
  {
    *p = start + i;
    p += 1;
  }
  *p = 0;

  Font.SetDirect(0, NULL);
  OutTextXY((r->xy1.x + r->xy2.x) / 2, y, txt, attrib, MakeColor_Makro(INVISIBLE, GetPenColor_Makro(col)), sizeof(_CHAR));  // original
*/


}

void popup_fontview_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  if(pev->ftype == _EVENT_KEYPRESS)
  {
    switch(pev->scancode)
    {
     #ifdef FONTMARK
      case _ENTER: FontMark.ReSetState(PopUp->GetMenu()->position.pos);
                   break;
      case _UP   :  
	  case _DOWN : if(pev->modifier & _SHIFT)
                     FontMark.ReSetState(pm->position.pos);
                   break;
     #endif
/*
      case '0' : cploffset = 0;
                 PopUp->OutPage(pm->position.pos, _TRUE);
                 pev->ftype = _EVENT_NONE;   // eat event
                 break;
      case '1' : cploffset = 0x1000;
                 PopUp->OutPage(pm->position.pos, _TRUE);
                 pev->ftype = _EVENT_NONE;   // eat event
                 break;
      case '2' : cploffset = 0x2000;
                 PopUp->OutPage(pm->position.pos, _TRUE);
                 pev->ftype = _EVENT_NONE;   // eat event
                 break;
      case '*' : cploffset += pm->position.height; //CHARACTER_PER_LINE;
                 PopUp->OutPage(pm->position.pos, _TRUE);
                 pev->ftype = _EVENT_NONE;   // eat event
                 break;
      case '/' : cploffset -= pm->position.height; //CHARACTER_PER_LINE;
                 PopUp->OutPage(pm->position.pos, _TRUE);
                 pev->ftype = _EVENT_NONE;   // eat event
                 break;
      case '+' : cploffset += 1; //CHARACTER_PER_LINE;
                 PopUp->OutPage(pm->position.pos, _TRUE);
                 pev->ftype = _EVENT_NONE;   // eat event
                 break;
      case '-' : cploffset -= 1; //CHARACTER_PER_LINE;
                 PopUp->OutPage(pm->position.pos, _TRUE);
                 pev->ftype = _EVENT_NONE;   // eat event
                 break;
*/
    }
  }
}

void popup_alltextlist_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII tmp[60];

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < TextList.GetNo())
  {
    StrCpy(tmp, sizeof(_ASCII), " Textlist_", sizeof(_ASCII));
    DToA(&tmp[StrLen(tmp, sizeof(_ASCII))], no, 0x0800, sizeof(_ASCII));
    StrCat(tmp, sizeof(_ASCII), " ", sizeof(_ASCII));
    DToA(&tmp[StrLen(tmp, sizeof(_ASCII))], TextList.GetNo((_ANYLST)((_UDWORD)TXTLIST_VTX + no)), 0x0800, sizeof(_ASCII));

    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, tmp, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
}

void popup_alltextlist_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _UWORD no;

  if((pev->ftype == _EVENT_KEYPRESS)&&(pev->scancode == _ENTER))
  {
    no = PopUp->GetMenu()->position.pos;
    if(no < TextList.GetNo())
    {
//      Window->AddCmd(CMD_NEWPOPUP, UC_TXT_DIAG + no, 0);
      menuline = no;
      Window->AddCmd(CMD_NEWPOPUP, UC_TXT_DIAG, 0, pev->lrm);
      pev->ftype = _EVENT_NONE;   // eat event
    }
  }

}

void popup_language_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII         tmp[200];
  _LANGUAGE_INFO *pli;
  _UDWORD        winid;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < Language.GetNo())
  {
    pli = Language.Get_LANGUAGE_INFO();

    winid = 0;
    if(no < pli->winid.no)
      winid = pli->winid.ptr[no];

//    StrCpy(tmp, sizeof(_ASCII), "0x", sizeof(_ASCII));
    DToA(tmp, winid, 0x800, sizeof(_ASCII));
    StrCat(tmp, sizeof(_ASCII), (no == get_actual_language())? ".mpc *":".mpc  ", sizeof(_ASCII));
    StrCat(tmp, sizeof(_ASCII), gettext_ASCII_BUFFER(&pli->winname, no), sizeof(_ASCII));
    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, tmp, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));

    StrWidth(tmp, sizeof(_ASCII));

    if(no < pli->flag.no)
      out_bmp(r, pli->flag.ptr[no], tmp, sizeof(_ASCII));

  }
}

void popup_language_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _UWORD no;

  if((pev->ftype == _EVENT_KEYPRESS)&&(pev->scancode == _ENTER))
  {
    no = PopUp->GetMenu()->position.pos;
    if(no < Language.GetNo())
    {
      if(no == get_actual_language())
        Window->AddCmd(CMD_NEWPOPUP, UC_ALLTEXT_POPUP, 0, pev->lrm);
      else
        Window->AddCmd(CMD_NEWLANGUAGE, no, 0, pev->lrm);

      pev->ftype = _EVENT_NONE;   // eat event
    }
  }
}



void popup_menulist_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII   txt[150];
  _ASCII   tmp[100];
  _LSEMENU *pm;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < MenuList.GetNo())
  {
    DToA(txt, no, 0x0104, sizeof(_ASCII));

    pm = MenuList.Get_LSEMENU(no);
    if(pm != NULL)
    {
      StrCpy(tmp, sizeof(_ASCII), " Width:", sizeof(_ASCII));
      DToA(&tmp[StrLen(tmp, sizeof(_ASCII))], pm->info.width, 0x0800, sizeof(_ASCII));
      StrCat(tmp, sizeof(_ASCII), ", Height:", sizeof(_ASCII));
      DToA(&tmp[StrLen(tmp, sizeof(_ASCII))], pm->info.line_height * pm->info.height, 0x0800, sizeof(_ASCII));
      StrCat(&tmp[StrLen(tmp, sizeof(_ASCII))], sizeof(_ASCII), ", Lines/Page:", sizeof(_ASCII));
      DToA(&tmp[StrLen(tmp, sizeof(_ASCII))], pm->info.height, 0x0800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), tmp, sizeof(_ASCII));
     #if COMPILER_VERSION >= 33
      do_lselabel(txt, &MenuList.LseLabel, no, ' ', 0);
     #endif
     #if COMPILER_VERSION >= 41
      _UBYTE obj = 0;
      if(pm->info.server.info[0].state != DEF_VAL)
      {
        StrCat(txt, sizeof(_ASCII), " ", sizeof(_ASCII));
        VarList.GetLabel(&txt[StrLen(txt, sizeof(_ASCII))], &obj, pm->info.server.info[0].value);
      }
     #endif

    }

    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
}

void popup_menulist_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _UWORD no;

  seek_run(pev, &MenuList.LseLabel, PopUp);

  if(pev->ftype == _EVENT_KEYPRESS)
  {
    no = PopUp->GetMenu()->position.pos;
    if(no < MenuList.GetNo())
    {
      if(pev->scancode == _ENTER)
      {
//        Window->AddCmd(CMD_NEWPOPUP, UC_MENU_DIAG+no, 0);
        menuline = no;
        Window->AddCmd(CMD_NEWPOPUP, UC_MENU_DIAG, 0, pev->lrm);
        pev->ftype = _EVENT_NONE;   // eat event
      }
      else if(pev->scancode == _F1)
      {
        Window->AddCmd(CMD_NEWPOPUP, no, 0, pev->lrm);
        pev->ftype = _EVENT_NONE;   // eat event
      }
    }
  }
}

void make_MEM_NO(_MEM_NO *pm, _VARLABEL *vl, _UDWORD filter)
{
  _UWORD      no, i;
  _UDWORD     size;
  _VARIABLE   variable;
  _LSEVARINFO *p;
  _MEM_NO     mem;
  _UWORD      station = 254;

  if(vl != NULL)
  {
    if(VarList.GetVarInfo(&p, vl->info[0].value) == _TRUE)
    {
      station = p->station;
    }
  }

  free_MEM_NO(pm);
  no = VarList.GetNo(); 
  init_MEM_NO(&mem); 

  for(i=0; i<no; i++)
  {
    if(VarList.GetSingleVariable(&variable, i) == _TRUE)
    {
      if((variable.info.statistic.group & filter) || (filter == 0))
      {
        if((variable.info.lasalid.id != DEFLASALID) && (variable.info.station == station))
        {
          size = (mem.no + 1) * sizeof(_UWORD);
          if(Memory2((void**)&mem.ptr, size) != 0)
          {
            mem.ptr[mem.no] = i;
            mem.no ++;
          }
        }
      }
    }
  }

  *pm = mem;
  init_MEM_NO(&mem);
}

void lslid_mae_run(_Window *Pw, _PopUp *Pp, _EVENT *pe)
{
  _MENU         *pm;
  _MEM_NO       *pd;
  _VARLABEL     *pv;
  _LSEVARINFO   *p;
  _STACK_LINE   sl;

  if(pe->ftype == _EVENT_KEYPRESS)
  {
    pm = Pp->GetMenu();
    pd = (_MEM_NO*)pm->pointer.PtrData;

    if(pe->scancode == _ENTER)
    {
      pv = Pp->GetInput();
      if(chk_VARLABEL(pv) == _TRUE)
      {
        if(pm->position.pos < pd->no)
        {
          if(VarList.GetVarInfo(&p, pd->ptr[pm->position.pos]) == _TRUE)
          {
            init_STACK_LINE(&sl);
            sl.cmd = CMD_SET;
            sl.op1 = *pv;
            sl.op2.no = 1;
            sl.op2.info[0].state = CONST_VAL;
            sl.op2.info[0].value = p->lasalid.id;
            Pw->AddLine(&sl, pe->lrm);                                      // save
          }
        }

        init_STACK_LINE(&sl);
        sl.cmd = CMD_RETURN_TO_SIGN;
        Pw->AddLine(&sl, pe->lrm);                                      // exit
      }
    }
  }
}

void lslid_mae_end(_Window *Pw, _PopUp *Pp)
{
  _MENU *pm;

  pm = Pp->GetMenu();
  free_MEM_NO((_MEM_NO*)pm->pointer.PtrData);
  Memory2((void**)&pm->pointer.PtrData, 0);
}

void lslid_mae_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _MENU     *pm;
  _MEM_NO   *pd;
  _ASCII    label[100];
  _UBYTE    varobj;
  _KOORD    y;

  pm = Pp->GetMenu();
  pd = (_MEM_NO*)pm->pointer.PtrData;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(pd != NULL)
  {
    label[0] = 0;
    if(no < pd->no)
    {
      if(VarList.GetLabel(label, &varobj, pd->ptr[no]) == _FALSE)
        label[0] = 0;
    }

    y = (r->xy1.y + r->xy2.y) / 2;
    OutTextXY(r->xy1.x+2, y, label, pm->info.attrib, col, sizeof(_ASCII));
  }
}

_BOOL idlist_call(_MENU *pm, _VARLABEL *vl)
{
  _MEM_NO *p = NULL;
  _UWORD  x, y;
  _BOOL   retcode = _FALSE;

  if(Memory2((void**)&p, sizeof(_MEM_NO)) != 0)
  {
    init_MEM_NO(p);
    make_MEM_NO(p, vl, 0);

    retcode = _TRUE;

    pm->info.font        = 0; //DEFFONT;
    pm->info.xy.x        = 10;
    pm->info.xy.y        = 200;
    pm->info.width       = 240;
    pm->info.height      = (p->no < 6)? p->no : 6;
    Font.Size(pm->info.font, &x, &y);
    pm->info.line_height = y;

    pm->pointer.PtrLine  = lslid_mae_line; // --> to linedraw function
    pm->pointer.PtrStart = NULL;              // --> to start function
    pm->pointer.PtrReady = NULL;              // --> to ready function
    pm->pointer.PtrRun   = lslid_mae_run;       // --> to run function
    pm->pointer.PtrEnd   = lslid_mae_end;       // --> to end function
    pm->pointer.PtrData  = (void*)p;                    // --> to data
    pm->position.no      = p->no;
  }

  return(retcode);
}

void popup_sequence_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII txt[400];
  _COMPCODE *compcode=NULL;


  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < Sequencer.GetNo())
  {
    DToA(txt, no, 0x0105, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), " ", sizeof(_ASCII));
    if(Sequencer.Get(&compcode, no) == _TRUE)
    {
     #if COMPILER_VERSION >= 33
      do_lselabel(txt, &Sequencer.LseLabel, no, 0, ':');
     #endif
      COMPCODE_to_text(&txt[StrLen(txt, sizeof(_ASCII))], compcode, 300); // 300 weil 400 - strlen(label)
      OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
    }
  }
}

void popup_sequence_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _UWORD    no;
  _COMPCODE *compcode=NULL;

  seek_run(pev, &Sequencer.LseLabel, PopUp);

  if((pev->ftype == _EVENT_KEYPRESS) && (pev->scancode == _ENTER))
  {
    no = PopUp->GetMenu()->position.pos;
    if(no < Sequencer.GetNo())
    {
      if(Sequencer.Get(&compcode, no) == _TRUE)
        out_runtext(PopUp, (void*)((Sequencer.CallCompiler(compcode, _FALSE, _TRUE, _FALSE) == _TRUE)? "{TRUE}":"{FALSE}"), sizeof(_ASCII));

      pev->ftype = _EVENT_NONE;   // eat event
    }
  }
}

_BOOL menu_call(_MENU *Menu, _UWORD menu_no)
{
  _BOOL    retcode = _FALSE;
  _UWORD   height, lines;
  _LSEMENU *pm;

  diagnostic_call(Menu, 1, _FALSE);

  pm = MenuList.Get_LSEMENU(menu_no);

  if(pm != NULL)
  {
    Menu->info.width = (Menu->info.width * 2) / 3;
    height           = (Menu->info.height * 2) / 3;
    lines            = pm->mem_lineinfo.no;

    if(lines < height)
      height = lines;

    Menu->info.height = height;

    Menu->pointer.PtrLine = popup_menu_line;    // --> to linedraw function
    Menu->pointer.PtrData = (void*)(_UDWORD)menu_no;
    Menu->position.no     = lines;
    retcode = _TRUE;
  }

  return(retcode);
}

#ifdef FONTMARK
void popup_fontview_end(_Window *pw, _PopUp *pp)
{
  FontMark.Free();
}
#endif

_BOOL fontview_call(_MENU *Menu, _UWORD font_no)
{
  _BOOL  retcode = _FALSE;
  _UWORD xx, yy;


  diagnostic_call(Menu, 1, _FALSE);

  if(font_no < Font.GetNo())
  {
    Font.SizeDirect(font_no, &xx, &yy);
    if(yy == 0)
      yy = 14;
    Menu->info.height      = (Menu->info.line_height * Menu->info.height) / yy;
    Menu->info.line_height = yy;
    Menu->pointer.PtrLine  = popup_fontview_line;    // --> to linedraw function
    Menu->pointer.PtrRun   = popup_fontview_run;     // --> to run function
    Menu->pointer.PtrData  = (void*)(_UDWORD)font_no;

 #ifdef FONTMARK
    FontMark.Init();
    Menu->pointer.PtrEnd  = popup_fontview_end;
 #endif

    Menu->position.no     = (65536L + CHARACTER_PER_LINE - 1) / CHARACTER_PER_LINE; //ObjectList.GetNo();
    retcode = _TRUE;
  }

  return(retcode);
}

void format_LINEINFO(_ASCII *txt, _LINEINFO *p, _TextPuffer *Tp)
{
  _CHAR *tmp;

  StrCat(txt, sizeof(_ASCII), " Valency:", sizeof(_ASCII));
  VARLABEL_to_text(&txt[StrLen(txt, sizeof(_ASCII))], &p->value);
  tmp = get_ANYPARA(&p->anypara, Tp);
  if(tmp != NULL)
  {
    StrCat(txt, sizeof(_ASCII), ", ", sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), tmp, sizeof(_CHAR));
  }
}

void popup_menu_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _MENU    *pm = Pp->GetMenu();
  _LSEMENU *p  = MenuList.Get_LSEMENU((_UWORD)(_UDWORD)pm->pointer.PtrData);
  _ASCII   txt[150];

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < p->mem_lineinfo.no)
  {
    DToA(txt, no, 0x0105, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), ": ", sizeof(_ASCII));
    format_LINEINFO(&txt[StrLen(txt, sizeof(_ASCII))], &p->mem_lineinfo.ptr[no], &p->TextPuffer);
    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
}

void popup_colpal_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _UWORD i, start, width;
  _KOORD x;
  _ASCII txt[20];

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < 16)
  {
	 start = no * 16;
    DToA(txt, start, 0x0103, sizeof(_ASCII));
    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
    x     = r->xy1.x + StrWidth(txt, sizeof(_ASCII)) + 4;
    width = (r->xy2.x - x + 1) / 16;
    for(i=0; i<16; i++)
    {
      Bar(x, r->xy1.y+1, x+width-2, r->xy2.y, T_COPY|T_SOLID, MakeColorFromIndex(start+i));
      x += width;
    }
  }
}

_BOOL colpalpopup_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine  = popup_colpal_line;    // --> to linedraw function
  Menu->position.no      = 16;
  return(_TRUE);
}

_BOOL systeminfo_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine  = popup_systeminfo_line;
  Menu->pointer.PtrRun   = popup_systeminfo_run;
 #ifdef UC_IPC
  Menu->position.no      = 16; // + Batch.GetComSet()->no;
 #else
  Menu->position.no      = 11; // + Batch.GetComSet()->no;
 #endif

  return(_TRUE);
}

void get_ip_data(_ASCII *txt, _UDWORD intfc, _UDWORD adr)
{
  _UDWORD address;
  _BOOL   st = _FALSE;
  _ASCII  str[40];

  if(adr == 0)
    st = GetTCPIPAddress(intfc, &address, NULL, NULL);
  else if(adr == 1)
    st = GetTCPIPAddress(intfc, NULL, &address, NULL);
  else if(adr == 2)
    st = GetTCPIPAddress(intfc, NULL, NULL, &address);
    
  if(st == _TRUE)
  {
    str[0] = 0;
    tcpiptext(str, address);
  }
  else
  {
    StrCpy(str, sizeof(_ASCII), " not available", sizeof(_ASCII));
  }
  
  StrCpy(&txt[StrLen(txt, sizeof(_ASCII)) - StrLen(str, sizeof(_ASCII))], sizeof(_ASCII), str, sizeof(_ASCII));
}

void popup_systeminfo_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII  txt[100];
  _MENU   *pm;
  _ROOM   room;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  pm = Pp->GetMenu();

  if(no < pm->position.no)
  {
    txt[0] = 0;

    switch(no)
    {
      case 0:
        StrCpy(txt, sizeof(_ASCII), "OpSys:     [RTK/x86]    ", sizeof(_ASCII));
        if(OpSysIsSalamander())
          MemCpy(&txt[12], "SAL", 3);
        if(CPUArchIsARM())
          MemCpy(&txt[16], "ARM", 3);
        if(RTOSVERSION >= 0x1000)
          make_version_text(&txt[21], RTOSVERSION, 0x0800);
        else
          DToA(&txt[StrLen(txt, sizeof(_ASCII))], ((RTOSVERSION >> 8) & 0xFF) * 100 + (RTOSVERSION & 0xFF), 0x25, sizeof(_ASCII));
        break;
      case 1:
        StrCpy(txt, sizeof(_ASCII), "LSE Version:            ", sizeof(_ASCII));
        make_version_text(&txt[21], GetLseServer(LSE_LSEVERSION), 0x0800);
        break;
      case 2:
        StrCpy(txt, sizeof(_ASCII), "KERNEL Version:         ", sizeof(_ASCII));
        make_version_text(&txt[21], _LSE_VERSION, 0x0800);
        break;
      case 3:
        StrCpy(txt, sizeof(_ASCII), "Compiler Version:       ", sizeof(_ASCII));
        DToA(&txt[StrLen(txt, sizeof(_ASCII))], COMPILER_VERSION, 0x06, sizeof(_ASCII));
        break;
      case 4:
        StrCpy(txt, sizeof(_ASCII), "LSE compiled to:     ", sizeof(_ASCII));
        if(TextList.GetUnicode() == _TRUE)
          StrCat(txt, sizeof(_ASCII), "  UNICODE", sizeof(_ASCII));
        else
          StrCat(txt, sizeof(_ASCII), "ASCIICODE", sizeof(_ASCII));
        break;
      case 5:
        StrCpy(txt, sizeof(_ASCII), "KERNEL compiled to:  ", sizeof(_ASCII));
       #ifdef UC_UNICODE
        StrCat(txt, sizeof(_ASCII), "  UNICODE", sizeof(_ASCII));
       #else
        StrCat(txt, sizeof(_ASCII), "ASCIICODE", sizeof(_ASCII));
       #endif
        break;
      case 6:
        StrCpy(txt, sizeof(_ASCII), "Screen Resolution: ", sizeof(_ASCII));
        MyGetScreenSize(&room);
        DToA(&txt[StrLen(txt, sizeof(_ASCII))], room.xy2.x + 1, 0x0104, sizeof(_ASCII));
        StrCat(txt, sizeof(_ASCII), " x ", sizeof(_ASCII));
        DToA(&txt[StrLen(txt, sizeof(_ASCII))], room.xy2.y + 1, 0x0104, sizeof(_ASCII));
        break;
      case 7:
        StrCpy(txt, sizeof(_ASCII), "TCPIP1 Address:               ", sizeof(_ASCII));
        get_ip_data(txt, 1, 0);
        //StrCpy(&txt[StrLen(txt, sizeof(_ASCII)) - StrLen(str, sizeof(_ASCII))], sizeof(_ASCII), str, sizeof(_ASCII));
        break;
      case 8:
        StrCpy(txt, sizeof(_ASCII), "TCPIP1 Subnet:                ", sizeof(_ASCII));
        get_ip_data(txt, 1, 1);
        break;
      case 9:
        StrCpy(txt, sizeof(_ASCII), "TCPIP1 Gateway:               ", sizeof(_ASCII));
        get_ip_data(txt, 1, 2);
        //StrCpy(&txt[StrLen(txt, sizeof(_ASCII)) - StrLen(str, sizeof(_ASCII))], sizeof(_ASCII), str, sizeof(_ASCII));
        break;
     #ifdef UC_IPC
      case 10:
        StrCpy(txt, sizeof(_ASCII), "Min cycle time:           [ms]", sizeof(_ASCII));
        break;
      case 11:
        StrCpy(txt, sizeof(_ASCII), "Max cycle time:           [ms]", sizeof(_ASCII));
        break;
      case 12:
        StrCpy(txt, sizeof(_ASCII), "Average cycle time:       [ms]", sizeof(_ASCII));
        break;
      case 13:
        StrCpy(txt, sizeof(_ASCII), "Timebase for calculation  [ms]  Use +/- to change", sizeof(_ASCII));
        break;
      case 14:
        StrCpy(txt, sizeof(_ASCII), "StartupTime LSE-Project:  [ms]  ", sizeof(_ASCII));
        DToA(&txt[StrLen(txt, sizeof(_ASCII))], SetUp.StartupTimeLseProject, 0x0800, sizeof(_ASCII));
        break;
     #endif
      default:
        StrCpy(txt, sizeof(_ASCII), "RESET CPU !      [press ENTER]", sizeof(_ASCII));
        break;
    }

    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
}

_UWORD lse_time(_ASCII *txt, _UDWORD time)
{
  if(LseTime.valid == _TRUE)
    DToA(txt, time, 0x0038, sizeof(_ASCII));
  else
    StrCpy(txt, sizeof(_ASCII), "       off", sizeof(_ASCII));

  return(2);
}

void popup_systeminfo_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _MENU       *pm;
  _UWORD      y, no;
  _ROOM       room;
  _ASCII      txt[100];
  _COLOR      col;
  _UWORD      ok;
 #ifdef UC_IPC
  _UDWORD     sleeptime = WatchSleep();
 #endif

  pm  = PopUp->GetMenu();

  if(pev->ftype == _EVENT_KEYPRESS)
  {
    no = PopUp->GetMenu()->position.pos;

    if((no == (pm->position.no-1)) && (pev->scancode == _ENTER))
    {
      Root.FindTop()->AddCmd(CMD_NEWWINDOW, UC_RESETWINDOW, 0, pev->lrm); // open resetwindow
    }
   #ifdef UC_IPC
    else if((no >= 10) && (no <= 12) && (pev->scancode == _ENTER))
    {
      LseTime.valid = (LseTime.valid == _TRUE) ? _FALSE : _TRUE;
      if(LseTime.valid == _FALSE)
        free_LSE_TIME(&LseTime);
      else
      {
//        LseTime.timemin = 0;
        init_LSE_TIME(&LseTime);
        LseTime.valid = _TRUE;
      }
    }
    else if(no == 13)
    {
      if(pev->scancode == '+')
      {
        if(LseTime.timebase < 999900000)
          LseTime.timebase += 100000;
        else
          LseTime.timebase = 999900000;
      }
      else if(pev->scancode == '-')
      {
        if(LseTime.timebase > 100000)
          LseTime.timebase -= 100000;
        else
          LseTime.timebase = 0;
      }
    }
   #endif
  }

  for(y=0; y<pm->position.height; y++)
  {
    ok = 0;
    no = y + pm->position.begin;
    if(no < pm->position.no)
    {
      switch(no)
      {
       #ifdef UC_IPC
        case 10:
          ok = lse_time(txt, LseTime.timemin);
          break;
        case 11:
          ok = lse_time(txt, LseTime.timemax);
          break;
        case 12:
          ok = lse_time(txt, LseTime.timeaverage);
          break;
        case 13:
          DToA(txt, LseTime.timebase / 1000, 0x0006, sizeof(_ASCII));
          ok = 2;
          break;
       #endif
      }

      if(ok != 0)
      {
        PopUp->GetLinePosition(y, &room);

        if(no == pm->position.pos)
          col = pm->info.bar_color;
        else
          col = pm->info.text_color;

        Font.Set(pm->info.font, &pm->info.attrib);
//        col = MakeColor_Makro(ReadPixel(room.xy2.x, room.xy1.y), GetPenColor_Makro(col));
       #ifdef UC_MOUSE
        _UWORD hide = IfNecHideMouseCursor(&room);
       #endif
       if(ok == 1)
        OutTextXY(room.xy2.x - 10, (room.xy1.y + room.xy2.y) / 2, "no info", T_COPY|T_SOLID|T_PROP|T_RIGHTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
       else
        OutTextXY(room.xy2.x - 10, (room.xy1.y + room.xy2.y) / 2, txt, T_COPY|T_SOLID|T_PROP|T_RIGHTBOUND|T_MIDBOUND, col, sizeof(_ASCII));

       #ifdef UC_ROTATE
        RefreshRectangle(&room);
       #endif
       #ifdef UC_MOUSE
        IfNecShowMouseCursor(hide);
       #endif
      }
    }
  }
 #ifdef UC_IPC
  WatchAwake(sleeptime);
 #endif
}

void popup_batch_end(_Window *Pw, _PopUp *Pp)
{
  _AsciiPuffer *Pap = (_AsciiPuffer*)Pp->GetMenu()->pointer.PtrData;
  if(Pap != NULL)
  {
    Pap->Free();
    Memory((void**)&Pap, 0);
  }
}

void popup_batch_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _AsciiPuffer *Pap;
  _MENU        *pm;
  _KOORD       x, y;
  _ASCII       *txt;

  pm  = Pp->GetMenu();
  Pap = (_AsciiPuffer*)pm->pointer.PtrData;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(Pap != NULL)
  {
    if(no < Pap->GetNo())
    {
      y = (r->xy1.y + r->xy2.y) / 2;
      x = r->xy1.x;

      txt = Pap->GetText(no);
      OutTextXY(x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
    }
  }
}

_BOOL batch_call(_MENU *Menu, const _ASCII *dpne)
{
  _AsciiPuffer *Pap = NULL;

  if(Memory((void**)&Pap, sizeof(_AsciiPuffer)) != 0)
  {
    Pap->Init();
    Batch.Read(Pap, dpne);
    diagnostic_call(Menu, 1, _FALSE);
    Menu->pointer.PtrLine = popup_batch_line; // --> to linedraw function
    Menu->pointer.PtrEnd  = popup_batch_end;  // --> to endfunction
    Menu->pointer.PtrData = (void*)Pap;
    Menu->position.no     = Pap->GetNo();
    return(_TRUE);
  }

  return(_FALSE);
}

void popup_bin_end(_Window *Pw, _PopUp *Pp)
{
  Memory((void**)&Pp->GetMenu()->pointer.PtrData, 0);
}

_ASCII mkhex(_UBYTE x)
{
  return "0123456789ABCDEF"[x & 0x0F];
}

void popup_bin_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _UBYTE  *ptr;
  _UDWORD size, line, pos, i;
  _KOORD  x, y;
  _UBYTE  txt[200], *pa, *pe, chr;

  ptr = (_UBYTE*)Pp->GetMenu()->pointer.PtrData;
  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(ptr != NULL)
  {
    size = *(_UDWORD*)ptr;
    line = (size + 15)/16;
    if(no < line)
    {
      y = (r->xy1.y + r->xy2.y) / 2;
      x = r->xy1.x;

      ptr = ptr + 4 + no * 16;
      pos = no * 16;
      MemSet(txt, ' ', 100);
      DToA(txt, pos, 0x0106, sizeof(_ASCII));
      txt[6] = ':';
      pa = &txt[7];
      pe = &txt[58];
      for(i=0; i<16; i++)
      {
        if(pos < size)
        {
          chr = *ptr;
          //if((chr > 31) && (chr < 128))
          if(chr != 0)
            *pe = chr;
          else
            *pe = '.';
          
          pe += 1;

          pa[1] = mkhex(chr >> 4);
          pa[2] = mkhex(chr);
          pa += 3;

          if(i == 7)
          {
            pa  += 1;
           // pe  += 1;
          }
        }
        pos += 1;
        ptr += 1;
      }
      txt[74] = 0;

      OutTextXY(x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
    }
  }
}

_BOOL bin_call(_MENU *Menu, const _ASCII *dpne)
{
  _File   File;
  _UBYTE  *ptr;
  _UDWORD size;

  if(File.Open(LOAD, dpne) == _TRUE)
  {
    size = File.Length();
    ptr  = (_UBYTE*)File.PutOutMemory();
    File.Close();

    if(size > 512000)
      size = 512000;

    if(Memory((void**)&ptr, size + 4) != 0)
    {
      MemMove(ptr+4, ptr, size);
      *(_UDWORD*)ptr = size;
      diagnostic_call(Menu, 1, _FALSE);
      Menu->pointer.PtrLine = popup_bin_line; // --> to linedraw function
      Menu->pointer.PtrEnd  = popup_bin_end;  // --> to endfunction
      Menu->pointer.PtrData = (void*)ptr;
      Menu->position.no     = (size + 15) / 16;
      return(_TRUE);
    }
  }

  return(_FALSE);
}

#endif

_BOOL hotkeylist_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine = popup_hotkeylist_line;    // --> to linedraw function
  Menu->position.no     = Hotkey.GetNo();
  return(_TRUE);
}

void popup_hotkeylist_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _KEY   *key;
  _ASCII txt[100];

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  key = Hotkey.Get(no);

  if(key != NULL)
  {
    DToA(txt, no, 0x0105, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), "  ", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], key->code, 0x105, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), " pos fl ", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], key->gfunct.pos_fl.no, 0x105, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), " neg fl ", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], key->gfunct.neg_fl.no, 0x105, sizeof(_ASCII));

    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y)/2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
}

#ifdef UC_IPC
void ins_memsize(_ASCII *txt, const _ASCII *ctx, _UDWORD siz)
{

  StrCpy(txt, sizeof(_ASCII), ctx, sizeof(_ASCII));
  if(siz != 0xFFFFFFFFL)
  {
    text_1000(&txt[StrLen(txt, sizeof(_ASCII))], siz);
  }
  else
  {
    StrCat(txt, sizeof(_ASCII), "-no info-", sizeof(_ASCII));
  }
}

void popup_general_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII txt[100];

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  txt[0] = 0;

  switch(no)
  {
    case 0 : ins_memsize(txt, "Label: ", MemSize.label); break;
    case 1 : ins_memsize(txt, " Data: ", MemSize.data); break;
    case 2 : if((MemSize.data != 0xFFFFFFFFL)&&(MemSize.label != 0xFFFFFFFFL))
               ins_memsize(txt, "Total: ", MemSize.data + MemSize.label);
             else
               ins_memsize(txt, "Total: ", 0xFFFFFFFFL);
             break;
  }

  StrCat(txt, sizeof(_ASCII), " byte", sizeof(_ASCII));
  OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y)/2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
}

_BOOL general_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine = popup_general_line;    // --> to linedraw function
  Menu->position.no = 3; //SetUp.GetNo();

  Menu->info.width  = (Menu->info.width * 2) / 3;
  Menu->info.height = 3;

  return(_TRUE);
}
#endif

_BOOL setup_call(_MENU *Menu)
{
  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrLine = popup_setup_line;    // --> to linedraw function
  Menu->position.no     = SetUp.GetNo();
  return(_TRUE);
}

void  make_bool_line(_ASCII *dst, const _ASCII *src, _BOOL st)
{
  StrCat(dst, sizeof(_ASCII), src, sizeof(_ASCII));
  StrCat(dst, sizeof(_ASCII), (st == _TRUE)? "  TRUE":"  FALSE", sizeof(_ASCII));
}

void popup_setup_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _MENU *pm;
  _ASCII txt[100], save;
  const _ASCII *hp;
 #ifdef UC_BUBBLE_HELP
  _BUBBLEINFO *bubbleinfo;
 #endif
  _COLOR colback, colframe, coltxt, colcursor;
  _ROOM room;
  _ENTER_PLUS ep;
  _SOFTKEYSETUP *sk;
  _SCREENSAVER *ss;
  _ANYTHING any;
  _DATA *data;
  _UWORD size, len;
  _KOORD y = (r->xy1.y + r->xy2.y)/2;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  pm = Pp->GetMenu();

  DToA(txt, no, 0x0105, sizeof(_ASCII));

  switch(no)
  {
    case 0:
     #ifdef UC_BUBBLE_HELP
      bubbleinfo = SetUp.Get_BUBBLEINFO();
      StrCat(txt, sizeof(_ASCII), "  BUBBLEINFO:  Delaytime: ", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], bubbleinfo->delaytime, 0x800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), "  Remaintime:", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], bubbleinfo->remaintime, 0x800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), "  Appearance:", sizeof(_ASCII));
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));

      colback = SetUp.GetBubbleColorBack();
      colframe = SetUp.GetBubbleColorFrame();
      coltxt = SetUp.GetBubbleColorTxt();
      room.xy1.x = r->xy1.x + StrWidth(txt, sizeof(_ASCII)) + 10;
      room.xy1.y = r->xy1.y + 1;
      room.xy2.x = room.xy1.x + 40;
      room.xy2.y = r->xy2.y - 1;
      OutTextAlign((void*)"Text", 0, &room, bubbleinfo->font, bubbleinfo->attrib, bubbleinfo->frametype, coltxt, colback, colframe, sizeof(_ASCII));
     #else
      StrCat(txt, sizeof(_ASCII), "  NO BUBBLEHELP INFORMATION", sizeof(_ASCII));
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
     #endif

      break;

    case 1:
      StrCat(txt, sizeof(_ASCII), "  EDITCOLORS:  Select:        Change:        Group:        CurSel:        CurAct:", sizeof(_ASCII));
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));

      colback  = SetUp.GetEditColorBack(_EDITACTIVE, MakeColor_Makro(INVISIBLE, INVISIBLE));
      colframe = SetUp.GetEditColorFrame(_EDITACTIVE, MakeColor_Makro(INVISIBLE, INVISIBLE));
      coltxt   = SetUp.GetEditColorTxt(_EDITACTIVE, MakeColor_Makro(INVISIBLE, INVISIBLE));
      save = txt[27];
      txt[27] = 0;
      room.xy1.x = r->xy1.x + StrWidth(txt, sizeof(_ASCII)) + 10;
      room.xy1.y = r->xy1.y + 1;
      room.xy2.x = room.xy1.x + 40;
      room.xy2.y = r->xy2.y - 1;
      txt[27] = save;
      OutTextAlign((void*)"000", 0, &room, 0xFF, T_FILL|T_COPY|T_SOLID|T_CENTERBOUND|T_MIDBOUND, _3DFRAME, coltxt, colback, colframe, sizeof(_ASCII));

      colback  = SetUp.GetEditColorBack(_EDITCHANGE, MakeColor_Makro(INVISIBLE, INVISIBLE));
      colframe = SetUp.GetEditColorFrame(_EDITCHANGE, MakeColor_Makro(INVISIBLE, INVISIBLE));
      coltxt   = SetUp.GetEditColorTxt(_EDITCHANGE, MakeColor_Makro(INVISIBLE, INVISIBLE));
      save = txt[42];
      txt[42] = 0;
      room.xy1.x = r->xy1.x + StrWidth(txt, sizeof(_ASCII)) + 10;
      room.xy1.y = r->xy1.y + 1;
      room.xy2.x = room.xy1.x + 40;
      room.xy2.y = r->xy2.y - 1;
      txt[42] = save;
      OutTextAlign((void*)"000", 0, &room, 0xFF, T_FILL|T_COPY|T_SOLID|T_CENTERBOUND|T_MIDBOUND, _3DFRAME, coltxt, colback, colframe, sizeof(_ASCII));

      colback  = SetUp.GetEditColorBack(_SELECTGROUP, MakeColor_Makro(INVISIBLE, INVISIBLE));
      colframe = SetUp.GetEditColorFrame(_SELECTGROUP, MakeColor_Makro(INVISIBLE, INVISIBLE));
      coltxt   = SetUp.GetEditColorTxt(_SELECTGROUP, MakeColor_Makro(INVISIBLE, INVISIBLE));
      save = txt[56];
      txt[56] = 0;
      room.xy1.x = r->xy1.x + StrWidth(txt, sizeof(_ASCII)) + 10;
      room.xy1.y = r->xy1.y + 1;
      room.xy2.x = room.xy1.x + 40;
      room.xy2.y = r->xy2.y - 1;
      txt[56] = save;
      OutTextAlign((void*)"000", 0, &room, 0xFF, T_FILL|T_COPY|T_SOLID|T_CENTERBOUND|T_MIDBOUND, _3DFRAME, coltxt, colback, colframe, sizeof(_ASCII));

      colcursor = SetUp.GetEditColorCursor(_EDITACTIVE);
      save = txt[71];
      txt[71] = 0;
      room.xy1.x = r->xy1.x + StrWidth(txt, sizeof(_ASCII)) + 10;
      room.xy1.y = r->xy1.y + 1;
      room.xy2.x = room.xy1.x + 40;
      room.xy2.y = r->xy2.y - 1;
      txt[71] = save;
      DrawButton(&room, T_FILL|T_COPY|T_SOLID, _DEFFRAME, colcursor, colcursor);

      colcursor = SetUp.GetEditColorCursor(_EDITCHANGE);
      save = txt[86];
      txt[86] = 0;
      room.xy1.x = r->xy1.x + StrWidth(txt, sizeof(_ASCII)) + 10;
      room.xy1.y = r->xy1.y + 1;
      room.xy2.x = room.xy1.x + 40;
      room.xy2.y = r->xy2.y - 1;
      txt[86] = save;
      DrawButton(&room, T_FILL|T_COPY|T_SOLID, _DEFFRAME, colcursor, colcursor);
      break;

    case 2:
      StrCat(txt, sizeof(_ASCII), "  TEXT FRAME DISTANCE:  ", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], SetUp.GetFrameSpace(), 0x800, sizeof(_ASCII));
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 3:
      make_bool_line(txt, "  KEEP OBJECT OFFSET:", SetUp.GetKeepObjectOffset());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 4:
      make_bool_line(txt, "  ZOOM IMAGE LIST:", SetUp.GetZoomImageList());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 5:
      make_bool_line(txt, "  DELETE BACKGROUND:", SetUp.GetDeleteBackground());
      StrCat(txt, sizeof(_ASCII), ", Color:", sizeof(_ASCII));
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));

      colback = SetUp.GetBackgroundColor();
      room.xy1.x = r->xy1.x + StrWidth(txt, sizeof(_ASCII)) + 10;
      room.xy1.y = r->xy1.y + 1;
      room.xy2.x = room.xy1.x + 40;
      room.xy2.y = r->xy2.y - 1;
      DrawButton(&room, T_FILL|T_COPY|T_SOLID, _DEFFRAME, colback, colback);
      break;

    case 6:
      StrCat(txt, sizeof(_ASCII), "  NUMBER COLORS:  ", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], SetUp.GetColorsNo(), 0x105, sizeof(_ASCII));
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 7:
      make_bool_line(txt, "  DELETE EVENT BUFFER:", SetUp.GetDeleteEventBuffer());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 8:
      make_bool_line(txt, "  AUTOMATIC SEEK FIRST INPUT:", SetUp.GetAutomaticSeekFirstInput());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 9:
      ss = SetUp.Get_SCREENSAVER();
      StrCat(txt, sizeof(_ASCII), "  SCREENSAVER:  Actiontime: ", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], ss->actiontime, 0x800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ", Image:", sizeof(_ASCII));
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));

      any.list = ICONLST;
      any.no = ss->image;
      room.xy1.x = r->xy1.x + StrWidth(txt, sizeof(_ASCII)) + 10;
      room.xy1.y = r->xy1.y + 1;
      room.xy2.x = room.xy1.x + pm->info.line_height - 2;
      room.xy2.y = r->xy2.y - 1;
      out_ANYTHING(&any, &room, DEFFONT, T_COPY|T_SOLID, _DEFFRAME, DEFCOLOR, DEFCOLOR, DEFCOLOR, NULL);
      break;

    case 10:
      ep = SetUp.GetDirectionOnEnter();
      StrCat(txt, sizeof(_ASCII), "  DIRECTION ON ENTER:", sizeof(_ASCII));
      hp = "  NONE";
      switch(ep)
      {
        case _EP_AUTO : hp = "  AUTO";  break;
        case _EP_LEFT : hp = "  LEFT";  break;
        case _EP_RIGHT: hp = "  RIGHT"; break;
        case _EP_UP   : hp = "  UP";    break;
        case _EP_DOWN : hp = "  DOWN";  break;
        default       : break;
      }
      StrCat(txt, sizeof(_ASCII), hp, sizeof(_ASCII));
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 11:
      sk = SetUp.Get_SOFTKEYSETUP();
      StrCat(txt, sizeof(_ASCII), "  SOFTKEYS:  Height: ", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], sk->height, 0x102, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ", Width: ", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], sk->width, 0x103, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ", No: ", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], sk->no, 0x102, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ", Direction:", sizeof(_ASCII));
      if(sk->horizontal == _TRUE)
        StrCat(txt, sizeof(_ASCII), " Horizontal", sizeof(_ASCII));
      else
        StrCat(txt, sizeof(_ASCII), " Vertical", sizeof(_ASCII));

      StrCat(txt, sizeof(_ASCII), ", Alignment:", sizeof(_ASCII));

      if(sk->horizontal == _TRUE)
      {
        if(sk->align & T_CENTERBOUND)
          StrCat(txt, sizeof(_ASCII), " Center", sizeof(_ASCII));
        else if(sk->align & T_LEFTBOUND)
          StrCat(txt, sizeof(_ASCII), " Left", sizeof(_ASCII));
        else if(sk->align & T_RIGHTBOUND)
          StrCat(txt, sizeof(_ASCII), " Right", sizeof(_ASCII));
      }
      else
      {
        if(sk->align & T_MIDBOUND)
          StrCat(txt, sizeof(_ASCII), " Mid", sizeof(_ASCII));
        else if(sk->align & T_UPBOUND)
          StrCat(txt, sizeof(_ASCII), " Up", sizeof(_ASCII));
        else if(sk->align & T_DOWNBOUND)
          StrCat(txt, sizeof(_ASCII), " Down", sizeof(_ASCII));
      }

      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 12:
      make_bool_line(txt, "  USE TOUCH EDITOR:", SetUp.GetUseTouchEditor());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 13:
      data = SetUp.GetTargetNotice();
      StrCat(txt, sizeof(_ASCII), "  TARGET NOTICE:  ", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], data->no, 0x800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), " Byte", sizeof(_ASCII));
      if(data->no > 0)
      {
        size = data->no;
        if(size > 50)
          size = 50;

        StrCat(txt, sizeof(_ASCII), ", ", sizeof(_ASCII));
        len = StrLen(txt, sizeof(_ASCII));
        txt[len] = 34; len += 1;
        MemCpy(&txt[len], data->ptr, size);
        len += size;
        if(size != data->no)
        {
          txt[len] = '.'; len += 1;
          txt[len] = '.'; len += 1;
          txt[len] = '.'; len += 1;
        }
        txt[len] = 34;
        txt[len+1] = 0;
      }
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 14:
      make_bool_line(txt, "  EDITOR INC/DEC:", SetUp.GetEditorIncDec());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 15:
      make_bool_line(txt, "  HOTKEYS IN MENU:", SetUp.GetHotkeysInMenu());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 16:
      make_bool_line(txt, "  USE SHUTTLE TO EDIT:", SetUp.GetUseShuttleForEdit());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 17:
      make_bool_line(txt, "  USE SERVER HOSTNAME:", SetUp.GetUseHostName());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 18:
      make_bool_line(txt, "  USE WORD WRAP:", SetUp.GetWordWrap());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 19:
      make_bool_line(txt, "  VISIBLE PASSWORD CHARACTERS:", SetUp.GetDisplayPassword());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;

    case 20:
      make_bool_line(txt, "  DISABLE ALT-ESC:", SetUp.GetDisableAltEsc());
      OutTextXY(r->xy1.x, y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
      break;
  }
}

#ifdef UC_THEME
_BOOL themelist_call(_MENU *Menu)
{
  diagnostic_call(Menu, 5, _FALSE);
  Menu->pointer.PtrLine = popup_themelist_line;    // --> to linedraw function
  Menu->position.no     = Theme.GetNo();
  return(_TRUE);
}

void popup_themelist_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII txt[100];
  _ROOM room, ir;
  _THEME *ptheme;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  ptheme = Theme.GetByIndex(no);
  if(ptheme != NULL)
  {
    DToA(txt, ptheme->id, 0x0105, sizeof(_ASCII));

    room.xy1.x = r->xy1.x + 50;
    room.xy1.y = r->xy1.y + 2;
    room.xy2.x = room.xy1.x + (r->xy2.y - r->xy1.y + 1);
    room.xy2.y = r->xy2.y - 2;

    draw_THEME(&room, ptheme->id, T_SOLID, DEFCOLOR, _FALSE, _TRUE);
    inlet_THEME(&ir, &room, ptheme->id);
    Rectangle(ir.xy1.x, ir.xy1.y, ir.xy2.x, ir.xy2.y, T_COPY|T_DOTTED, MakeColor_Makro(INVISIBLE, LIGHTRED));
    room.xy1.x += 100;
    room.xy2.x += 100;
    draw_THEME(&room, ptheme->id, T_SOLID, DEFCOLOR, _TRUE, _TRUE);
    inlet_THEME(&ir, &room, ptheme->id);
    Rectangle(ir.xy1.x, ir.xy1.y, ir.xy2.x, ir.xy2.y, T_COPY|T_DOTTED, MakeColor_Makro(INVISIBLE, LIGHTRED));

    //StrCat(txt, sizeof(_ASCII), (ptheme->frame0.bmp_tl.state & 0x0004)? "" : "T", 1); // transparenzflag ausgeben
    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y)/2, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
}
#endif

#ifdef UC_IPC
void popup_onlineserver_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _UBYTE  varobj;
  _MENU   *pm;
  _MEM_NO *ptr;
  _ASCII  line[400];
  _UDWORD varno;
  _UWORD  len;

  pm  = Pp->GetMenu();
  ptr = (_MEM_NO*)pm->pointer.PtrData;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  if(no < ptr->no)
  {
    varno = ptr->ptr[no];
    DToA(line, varno, 0x800, sizeof(_ASCII));
    len = StrLen(line, sizeof(_ASCII));
    line[len++] = ' ';
    VarList.GetLabel(&line[len], &varobj, varno);      // get name of variable
    OutTextXY(r->xy1.x, (r->xy1.y + r->xy2.y) / 2, line, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_MIDBOUND, col, sizeof(_ASCII));
  }
}

void popup_onlineserver_end(_Window *Pw, _PopUp *PopUp)
{
  _MEM_NO *ptr = (_MEM_NO*)PopUp->GetMenu()->pointer.PtrData;

  if(ptr != NULL)
  {
    free_MEM_NO(ptr);
    Memory((void**)&PopUp->GetMenu()->pointer.PtrData, 0);
  }
}

_BOOL onlineserver_call(_MENU *Menu)
{
  _MEM_NO *ptr = NULL;

  if(Memory2((void**)&ptr, sizeof(_MEM_NO)) != 0)
  {
    init_MEM_NO(ptr);
    if(VarList.MakeReflistMap(ptr) == _TRUE)
    {
      diagnostic_call(Menu, 1, _FALSE);
      Menu->pointer.PtrLine = popup_onlineserver_line;    // --> to linedraw function
      Menu->pointer.PtrEnd  = popup_onlineserver_end;
      Menu->pointer.PtrData = (void*)ptr;
      Menu->position.no     = ptr->no;
    }
  }

  return(_TRUE);
}
#endif

_BOOL online_call(_MENU *Menu)
{
  diagnostic_call(Menu, 3, _FALSE);
  Menu->pointer.PtrLine = popup_online_line;    // --> to linedraw function
  Menu->pointer.PtrRun  = popup_online_run;
  Menu->position.no     = Batch.GetNo_COMPORT();

  return(_TRUE);
}

void tcpiptext(_ASCII *txt, _UDWORD adress)
{
  _CHAR label[100];
  format_ipaddress(label, adress);
  StrCpy(txt, sizeof(_ASCII), label, sizeof(_CHAR));
}

void getInterfaceText(_ASCII *txt, _COMDEF *cd)
{
  _ASCII *padr = NULL;


  switch(cd->typ)
  {
    case _COM_LOCAL:
      StrCpy(txt, sizeof(_ASCII), "LOCAL", sizeof(_ASCII));
      break;
    case _COM_INTERN:
      StrCpy(txt, sizeof(_ASCII), "INTERN", sizeof(_ASCII));
      break;
    case _COM_COM1:
      StrCpy(txt, sizeof(_ASCII), "COM1", sizeof(_ASCII));
      break;
    case _COM_COM2:
      StrCpy(txt, sizeof(_ASCII), "COM2", sizeof(_ASCII));
      break;
    case _COM_COM3:
      StrCpy(txt, sizeof(_ASCII), "COM3", sizeof(_ASCII));
      break;
    case _COM_COM4:
      StrCpy(txt, sizeof(_ASCII), "COM4", sizeof(_ASCII));
      break;
    case _COM_COM5:
      StrCpy(txt, sizeof(_ASCII), "COM5", sizeof(_ASCII));
      break;
    case _COM_COM6:
      StrCpy(txt, sizeof(_ASCII), "COM6", sizeof(_ASCII));
      break;
    case _COM_COM7:
      StrCpy(txt, sizeof(_ASCII), "COM7", sizeof(_ASCII));
      break;
    case _COM_COM8:
      StrCpy(txt, sizeof(_ASCII), "COM8", sizeof(_ASCII));
      break;
    case _COM_COM9:
      StrCpy(txt, sizeof(_ASCII), "COM9", sizeof(_ASCII));
      break;
    case _COM_COM10:
      StrCpy(txt, sizeof(_ASCII), "COM10", sizeof(_ASCII));
      break;
    case _COM_CAN1:
      StrCpy(txt, sizeof(_ASCII), "CAN1:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_CAN2:
      StrCpy(txt, sizeof(_ASCII), "CAN2:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_CAN20:
      StrCpy(txt, sizeof(_ASCII), "CAN20:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_CAN21:
      StrCpy(txt, sizeof(_ASCII), "CAN21:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_CAN22:
      StrCpy(txt, sizeof(_ASCII), "CAN22:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_CAN23:
      StrCpy(txt, sizeof(_ASCII), "CAN23:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_CAN24:
      StrCpy(txt, sizeof(_ASCII), "CAN24:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_CAN25:
      StrCpy(txt, sizeof(_ASCII), "CAN25:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_CAN26:
      StrCpy(txt, sizeof(_ASCII), "CAN26:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_CAN27:
      StrCpy(txt, sizeof(_ASCII), "CAN27:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_CAN28:
      StrCpy(txt, sizeof(_ASCII), "CAN28:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_CAN29:
      StrCpy(txt, sizeof(_ASCII), "CAN29:", sizeof(_ASCII));
      padr = txt;
      break;
    case _COM_TCPIP1:
    case _COM_TCPIP1_P:
      StrCpy(txt, sizeof(_ASCII), "TCPIP1:", sizeof(_ASCII));
      tcpiptext(txt, cd->ipadress);
      break;
    case _COM_TCPIP2_P:
      StrCpy(txt, sizeof(_ASCII), "TCPIP2:", sizeof(_ASCII));
      tcpiptext(txt, cd->ipadress);
      break;
    default:
      StrCpy(txt, sizeof(_ASCII), "UNDEFINED", sizeof(_ASCII));
      break;
  }

  if(padr != NULL)
    NumToStr(&padr[StrLen(txt, sizeof(_ASCII))], cd->adress, sizeof(_ASCII));
}

void popup_online_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _ASCII   txt[120];
  _COMPORT *pcp;
  _UWORD   stations[256], i, anz;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  pcp = Batch.GetComportDirect(no);
  if(pcp != NULL)
  {
    if(pcp->used == _TRUE)
    {
      if(pcp->offline == _TRUE)
        col = MakeColor_Makro(GetBackColor_Makro(col), RED);

      StrCpy(txt, sizeof(_ASCII), "STATION: ", sizeof(_ASCII));
      anz = Batch.GetStationViaComportIndex(stations, no);
      for(i=0; i<anz; i++)
      {
        if(i > 0)
          StrCat(txt, sizeof(_ASCII), ",", sizeof(_ASCII));
        NumToStr(&txt[StrLen(txt, sizeof(_ASCII))], stations[i]&0xFF, sizeof(_ASCII));
       #ifdef UC_MULTICPU
        if(stations[i] & 0xFF00)
        {
          StrCat(txt, sizeof(_ASCII), "[", sizeof(_ASCII));
          NumToStr(&txt[StrLen(txt, sizeof(_ASCII))], (stations[i]>>8)&0xFF, sizeof(_ASCII));
          StrCat(txt, sizeof(_ASCII), "]", sizeof(_ASCII));
        }
       #endif
      }
      
      StrCat(txt, 1, " [DesCrc=0x", 1);
      DToH(&txt[StrLen(txt, 1)], pcp->crc, 0x800, 1);
      StrCat(txt, 1, "]", 1);
      
      OutTextXY(r->xy1.x, r->xy1.y, txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_UPBOUND, col, sizeof(_ASCII));

      StrCpy(txt, sizeof(_ASCII), "  ", sizeof(_ASCII));
      getInterfaceText(&txt[2], &pcp->comdef);
      StrCat(txt, sizeof(_ASCII), ", state=", sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), (pcp->offline == _FALSE)? "ONLINE":"OFFLINE", sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ", observe=", sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), (pcp->observe == _TRUE)? "TRUE":"FALSE", sizeof(_ASCII));
      OutTextXY(r->xy1.x, r->xy1.y + ((r->xy2.y - r->xy1.y + 1) / 4), txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_UPBOUND, col, sizeof(_ASCII));

      StrCpy(txt, sizeof(_ASCII), "  server=", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], pcp->reflist.indexdat.no, 0x0800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ": used=", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], pcp->reflist.indexdat.used, 0x0800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ", perm=", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], pcp->reflist.indexdat.permanent, 0x0800, sizeof(_ASCII));
      OutTextXY(r->xy1.x, r->xy1.y + (((r->xy2.y - r->xy1.y + 1) * 2)/ 4), txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_UPBOUND, col, sizeof(_ASCII));

      StrCpy(txt, sizeof(_ASCII), "  size=", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], pcp->reflist.refdat.size, 0x0800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ": used=", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], pcp->reflist.refdat.used, 0x0800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ", perm=", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], pcp->reflist.refdat.permanent, 0x0800, sizeof(_ASCII));
      OutTextXY(r->xy1.x, r->xy1.y + (((r->xy2.y - r->xy1.y + 1) * 3) / 4), txt, T_COPY|T_SOLID|T_PROP|T_LEFTBOUND|T_UPBOUND, col, sizeof(_ASCII));
    }
  }
}

void popup_online_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _MENU    *pm;
  _UWORD   y, no;
  _ROOM    room;
  _COLOR   col;
  _COMPORT *pcp;

  pm = PopUp->GetMenu();
  if((pev->ftype == _EVENT_KEYPRESS)&&(pev->scancode == _ENTER))
  {
    pcp = Batch.GetComportDirect(pm->position.pos);
    if(pcp != NULL)
    {
      if(pcp->used == _TRUE)
      {
        if(pcp->offline == _TRUE)
          Batch.TryToGetOnline(pcp, pm->position.pos);
       #ifdef UC_IPC
        Window->AddCmd(CMD_NEWPOPUP, UC_ONLINE_SERVER, 0, pev->lrm);
       #endif
      }
    }
  }

  for(y=0; y<pm->position.height; y++)
  {
    no = y + pm->position.begin;

    if(no < pm->position.no)
    {
      PopUp->GetLinePosition(y, &room);

      if(no == pm->position.pos)
        col = pm->info.bar_color;
      else
        col = pm->info.text_color;

      Font.Set(pm->info.font, &pm->info.attrib);

      // Refresh wird immer aufgerufen - keine CRC oder dergleichen
      // Ist ja nur Diagnose und es flimmert nicht
      // Somit muß kein Speicher allociert und wieder freigegeben werden
      // Cursor wird nur bei Cursorbewegung gezeichnet
      pm->pointer.PtrLine(Window, PopUp, no, &room, col, _FALSE);

     #ifdef UC_MOUSE
      _UWORD hide = IfNecHideMouseCursor(&room);
     #endif
     #ifdef UC_ROTATE
      RefreshRectangle(&room);
     #endif
     #ifdef UC_MOUSE
      IfNecShowMouseCursor(hide);
     #endif
    }
  }
}

//*****************************************************************************
// HEAP_CHECK *****************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

#ifdef HEAP_DETAIL_INFO
_UWORD get_blockindex(_HEAPBLOCK *p)
{
  return((_UWORD)(p->BlockSize >> 28)& 0x000F);
}

_COLOR get_blockcolor(_UWORD idx)
{
  _UWORD col = LIGHTGREEN;

  switch(idx)
  {
    case  1: col = WHITE;          break;
    case  2: col = BROWN;          break;
    case  3: col = GREEN;          break;
    case  4: col = LIGHTRED;       break;
    case  5: col = LIGHTCYAN;      break;
    case  6: col = DARKGRAY;       break;
    case  7: col = LIGHTGRAY;      break;
    case  8: col = RED;            break;
    case  9: col = MakeTrueColor(255, 188,   0); break; // dunkelgelb MAGENTA;        break;
    case 10: col = MakeTrueColor(  0, 102, 255); break; // blau
    case 11: col = MakeTrueColor(255, 170, 255); break; // rosa
    case 12: col = MakeTrueColor(153, 180, 255); break; // blaugrau
    case 13: col = LIGHTMAGENTA;   break;
    case 14: col = YELLOW;         break;
    case 15: col = CYAN;           break;
  }

 #ifdef UC_GRAYSCALE
  transform_color_to_gray(&col);
 #endif

  return(MakeColor_Makro(col, col));
}

typedef struct { _UDWORD size;
                 _UDWORD no;
               }
_BLSNO;
_UWORD heap_detail_info;

void heap_blockinfo(_KOORD x, _KOORD y, _COLOR color, _BLSNO *p)
{
  char   txt[110];
  _UWORD idx;

  x +=  5;
  y -= 12;

  Font.Set(255, NULL);

  for(idx=0; idx<16; idx++)
  {
    if(p->no > 0)
    {
      if(idx == MEMORY_DEF)
        StrCpy(txt, sizeof(_ASCII), "DEF: ", sizeof(_ASCII));
      else if(idx == MEMORY_LSE)
        StrCpy(txt, sizeof(_ASCII), "LSE: ", sizeof(_ASCII));
      else if(idx == MEMORY_LDR)
        StrCpy(txt, sizeof(_ASCII), "LDR: ", sizeof(_ASCII));
      else if(idx == MEMORY_PRG)
        StrCpy(txt, sizeof(_ASCII), "PRG: ", sizeof(_ASCII));
      else if(idx == MEMORY_GRF)
        StrCpy(txt, sizeof(_ASCII), "GRF: ", sizeof(_ASCII));
      else if(idx == MEMORY_LME)
        StrCpy(txt, sizeof(_ASCII), "LME: ", sizeof(_ASCII));
      else
      {
        DToA(txt, idx, 0x0102, sizeof(_ASCII));
        StrCpy(&txt[StrLen(txt, sizeof(_ASCII))], sizeof(_ASCII), " : ", sizeof(_ASCII));
      }

//      StrCpy(&txt[StrLen(txt, sizeof(_ASCII))], sizeof(_ASCII), " no:", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], p->no, 0x0800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ", size:", sizeof(_ASCII));
      text_1000(&txt[StrLen(txt, sizeof(_ASCII))], p->size);
      OutTextXY(x+17, y, txt, T_COPY|T_SOLID|T_UPBOUND, color, sizeof(_ASCII));
      
      _UWORD blucol = BLUE;
     #ifdef UC_GRAYSCALE
      transform_color_to_gray(&blucol);
     #endif
      Bar(x, y, x+15, y+8, T_COPY|T_SOLID|T_UPBOUND, MakeColor_Makro(WHITE, blucol));
      Bar(x+1, y+1, x+14, y+7, T_COPY|T_SOLID|T_UPBOUND, MakeColor_Makro(WHITE, get_blockcolor(idx)));

      y -= 12;
    }

    p += 1;
  }
}
#endif

_UDWORD get_blocksize(_HEAPBLOCK *p)
{
  return(p->BlockSize & 0x0FFFFFFFL);
}

_BOOL heapblock_ok(_HEAPBLOCK *act, _UDWORD start, _UDWORD size)
{
  _UDWORD blsize = get_blocksize(act);

  if(((_UDWORD)act < start)||
     (((_UDWORD)act + blsize + sizeof(_HEAPBLOCK)) > (start + size))||
     ((blsize & 3) != 0))
  {
    return(_FALSE);
  }

  return(_TRUE);
}

_CRC32 get_heap_crc(void)
{
  _HEAPBLOCK *p;
  _HEAP      *ptr;
  _HEAP_TYPE *mat;
  _CRC32     retcode = 0;
  _UDWORD    start, size;

  if(OS_CILGet((char*)"HEAPMAT", (void**)&mat) == SYS_ERR_NONE)
  {
    size  = mat->applmeminfo->totalsize;
    start = mat->applmeminfo->startaddr;
    ptr   = (_HEAP*)mat->heap_mat;

    if(ptr != NULL)
    {
      p = ptr->UsedList;
      while(p != NULL)
      {
        if(heapblock_ok(p, start, size) == _FALSE)
        {
          retcode = 0;
          p = NULL;
        }
        else
        {
          retcode = MakeCrc32(retcode, (void*)p, 4);
          retcode = MakeCrc32(retcode, (void*)&p->BlockSize, 4);
          retcode = MakeCrc32(retcode, (void*)&p->Next, 4);
          p = p->Next;
        }
      }

      _UDWORD free_block = 0;
      p = ptr->FreeList;
      while(p != NULL)
      {
        free_block += 1;
        p           = p->Next;
      }
      retcode = MakeCrc32(retcode, (void*)&free_block, 4);
    }
  }

  return(retcode);
}

_BOOL heap_walk_sub(_HEAPBLOCK *p,  _UDWORD heapstart, _UDWORD heapsize, _BOOL display)
{
  _BOOL retcode = _TRUE;

 #ifdef HEAP_DETAIL_INFO
  _UWORD     idx;
  _BLSNO     data[16];
  if(display == _TRUE)
  {
    MemSet(data, 0, sizeof(data));
  }
 #endif

  while(p != 0)
  {
    if(heapblock_ok(p, heapstart, heapsize) == _FALSE)
    {
      p = 0;
      retcode = _FALSE;
    }
    else
    {
     #ifdef HEAP_DETAIL_INFO
      if(display == _TRUE)
      {
        idx = get_blockindex(p);
        data[idx].size += get_blocksize(p) + sizeof(_HEAPBLOCK);
        data[idx].no   += 1;
      }
     #endif
      p = p->Next;
    }
  }

 #ifdef HEAP_DETAIL_INFO
  if((retcode == _TRUE)&&(display == _TRUE))
  {
    for(idx=0; idx<16; idx++)
    {
      Debug(data[idx].no, idx+1);
      Debug(data[idx].size, idx+1, 1);
    }
  }
 #endif

  return(retcode);
}

_BOOL heap_walk(_HEAP *ptr,  _UDWORD heapstart, _UDWORD heapsize, _BOOL display)
{
  if(ptr != 0)
  {
    if(heap_walk_sub(ptr->UsedList, heapstart, heapsize, display) == _TRUE)
      if(heap_walk_sub(ptr->FreeList, heapstart, heapsize, _FALSE) == _TRUE)
        return(_TRUE);
    return(_FALSE);
  }

  return(_TRUE);
}

_BOOL check_heap(_BOOL display)
{
  _HEAP_TYPE *mat;
  _BOOL      retcode = _FALSE;
  _DOIT      doit = _IDLE;

 #ifndef CUTOFF_USER
  _FDOIT_BOOL ptr;

  ptr = (_FDOIT_BOOL)find_lasal_method(LseThisPointer, METHOD_LSE_CHECK_HEAP);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    doit = ptr(display);
    if(doit == _IDIDIT)
      retcode = _TRUE;
  }
 #endif

  if(doit == _IDLE)
  {
    if(OS_CILGet((char*)"HEAPMAT", (void**)&mat) == SYS_ERR_NONE)
    {
      retcode = heap_walk((_HEAP*)mat->heap_mat, mat->applmeminfo->startaddr, mat->applmeminfo->totalsize, display);
    }
  }

  return(retcode);
}

void diagnostic_heap(_ROOM *pr, void *mat, _UDWORD heapsize, _UDWORD heapstart)
{
  _UWORD     lineheight = 10;
  _UWORD     width;
  _UWORD     lines;
  _UDWORD    pixel;
  _ROOM      room = *pr;
  _HEAPBLOCK *p;
  _UDWORD    start, x0, anz;
  _KOORD     x, y, y1;
  _UDWORD    used_size  = 0;
  _UDWORD    used_block = 0;
  _COLOR     usedcolor, freecolor, endcolor;
  _ASCII     txt[70];
  _HEAP      *ptr = (_HEAP*)mat;
  _BOOL      error = _FALSE;
  _UDWORD    blsize;
 #ifdef HEAP_DETAIL_INFO
  _UWORD     idx;
  _BLSNO     data[16];
  MemSet(data, 0, sizeof(data));
 #endif
 
  _UWORD     blucol = BLUE;
  _UWORD     grncol = LIGHTGREEN;
 #ifdef UC_GRAYSCALE
  transform_color_to_gray(&blucol);
  transform_color_to_gray(&grncol);
 #endif
  
  freecolor = MakeColor_Makro(blucol, blucol);

 #ifndef HEAP_DETAIL_INFO
  usedcolor = MakeColor_Makro(grncol, grncol);
 #endif
  endcolor  = MakeColor_Makro(grncol, blucol);

  Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y-12, T_COPY|T_SOLID, freecolor);

  room.xy1.x += 1;
  room.xy1.y += 1;
  room.xy2.x -= 1;
  room.xy2.y -= 12;

 #ifdef HEAP_DETAIL_INFO
  if((heap_detail_info)&&((room.xy2.x - room.xy1.x) > 560))
    room.xy1.x = room.xy1.x + 260; //(room.xy2.x - room.xy1.x) / 2;
 #endif

  width      = room.xy2.x - room.xy1.x + 1;
  lines      = (room.xy2.y - room.xy1.y + 1) / lineheight;
  pixel      = lines * width;
  room.xy2.y = room.xy1.y + lines * lineheight;

  Rectangle(room.xy1.x-1, room.xy1.y-1, room.xy2.x+1, room.xy2.y+1, T_COPY|T_DOTTED, endcolor);

  if(ptr != NULL)
  {
    p = ptr->UsedList;
    while(p != NULL)
    {
      if(heapblock_ok(p, heapstart, heapsize) == _TRUE)
      {
       #ifdef HEAP_DETAIL_INFO
        usedcolor   = get_blockcolor(get_blockindex(p));
//        endcolor    = MakeColor_Makro(usedcolor, BLUE);
       #endif
        start       = ((_UDWORD)p - heapstart);
        x0          = AMBDC(start, pixel, heapsize); //calculator(start, pixel, heapsize);
        blsize      = get_blocksize(p) + sizeof(_HEAPBLOCK);
        anz         = AMBDC(blsize, pixel, heapsize); //calculator(blsize, pixel, heapsize);
        used_size  += blsize;
        used_block += 1;
       #ifdef HEAP_DETAIL_INFO
        idx = get_blockindex(p);
        data[idx].size += blsize;
        data[idx].no   += 1;
       #endif
        p           = p->Next;

        x  = room.xy1.x + x0 % width;
	      y  = room.xy1.y + (x0 / width) * lineheight;
        y1 = y + lineheight - 2;

        while(anz)
        {
          anz --;
          Line(x, y, x, y1, T_COPY|T_SOLID, usedcolor);
          x ++;
		      if(x > room.xy2.x)
		      {
		        x   = room.xy1.x;
		        y  += lineheight;
            y1 += lineheight;
          }
        }
        Line(x, y, x, y1, T_COPY|T_SOLID, usedcolor);
//        Line(x, y, x, y1, T_COPY|T_DOTTED, endcolor);
      }
      else
      {
        error = _TRUE;
        p     = NULL;
      }
    }

    if(error == _FALSE)
    {
      // freeheap ebenfalls überprüfen
      if(heap_walk_sub(ptr->FreeList, heapstart, heapsize, _FALSE) == _FALSE)
        error = _TRUE;
    }
  }

  if(error == _TRUE)
  {
    endcolor = MakeColor_Makro(RED, WHITE);
    StrCpy(txt, sizeof(_ASCII), "ERROR block:", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], used_block, 0x0800, sizeof(_ASCII));
  }
  else
  {
    endcolor = MakeColor_Makro(blucol, WHITE);
    if(ptr == NULL)
    {
      StrCpy(txt, sizeof(_ASCII), "can't display heap...", sizeof(_ASCII));
    }
    else
    {
      _UDWORD free_block = 0;
      p = ptr->FreeList;
      while(p != NULL)
      {
        free_block += 1;
        p           = p->Next;
      }

      StrCpy(txt, sizeof(_ASCII), "no:", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], used_block, 0x0800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), " (", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], free_block, 0x0800, sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), ")", sizeof(_ASCII));
      StrCat(txt, sizeof(_ASCII), " used:", sizeof(_ASCII));
      text_1000(&txt[StrLen(txt, sizeof(_ASCII))], used_size);
      StrCat(txt, sizeof(_ASCII), " free:", sizeof(_ASCII));
      text_1000(&txt[StrLen(txt, sizeof(_ASCII))], heapsize - used_size);

     #ifdef HEAP_DETAIL_INFO
      if(heap_detail_info)
        heap_blockinfo(pr->xy1.x, room.xy2.y, endcolor, data);
     #endif
    }
  }

  Bar(pr->xy1.x, pr->xy2.y-9, pr->xy2.x, pr->xy2.y, T_COPY|T_SOLID, GetBackColor_Makro(endcolor));
  Font.Set(255, NULL);
  OutTextXY(pr->xy1.x, pr->xy2.y, txt, T_COPY|T_SOLID|T_DOWNBOUND, endcolor, sizeof(_ASCII));
}

void draw_heap(_ROOM *pr)
{
  _HEAP_TYPE *mat;

  if(OS_CILGet((char*)"HEAPMAT", (void**)&mat) == SYS_ERR_NONE)
  {
    diagnostic_heap(pr, mat->heap_mat, mat->applmeminfo->totalsize, mat->applmeminfo->startaddr);
  }
  else
  {
    diagnostic_heap(pr, NULL, 0, 0);
  }

 #ifdef UC_ROTATE
  RefreshRectangle(pr);
 #endif
}

void popup_heap_ready(_Window *Pw, _PopUp *Pp, _BOOL firsttime)
{
  _MENU *pm = Pp->GetMenu();

  draw_heap(&pm->calcul.scroll_room);
  StatHeapCrc = get_heap_crc();
}

void popup_heap_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _UDWORD crc = get_heap_crc();

 #ifdef HEAP_DETAIL_INFO
  if(pev->ftype == _EVENT_KEYPRESS)
  {
    if(pev->scancode == _ENTER)
    {
      heap_detail_info ^= 1;
      StatHeapCrc = crc + 1;
    }
/*
    else if(pev->scancode == _F1)
    {
      // 1 block mehr reserviert
      void *pt = NULL;
      Memory((void**)&pt, 1000);
    }
    else if(pev->scancode == _F2)
    {
      // heap zerstört (memleak)
      void *pt = NULL;
      void *pt1 = NULL;
      Memory((void**)&pt1, 1000);
      pt = NULL;
      Memory((void**)&pt, 1000);
      pt = NULL;
      Memory((void**)&pt, 1000);
      pt = NULL;
      Memory((void**)&pt, 1000);
      pt = NULL;
      Memory((void**)&pt, 1000);
      MemSet(pt1, 0xA5, 1100);
    }
*/
  }
 #endif

  if(crc != StatHeapCrc)
  {
    StatHeapCrc = crc;
    draw_heap(&PopUp->GetMenu()->calcul.scroll_room);
  }
}

_BOOL heap_call(_MENU *Menu)
{
 #ifdef HEAP_DETAIL_INFO
  heap_detail_info = 0;
 #endif
  diagnostic_call(Menu, 1, _FALSE);
  Menu->pointer.PtrReady = popup_heap_ready;          // --> to ready function
  Menu->pointer.PtrRun   = popup_heap_run;
  Menu->position.no      = 0;
  return(_TRUE);
}

