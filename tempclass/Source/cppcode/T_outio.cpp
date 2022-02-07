#include "T_IPC.H"
#include "T_NUC.H"

/*
_REGION exclude_from_actual_region(_BOOL input, _UWORD actno, _Picture *Ppic)
{
  _ROOM   room;
  _REGION ioreg, newreg, actreg;

  room.xy1.x = 120;
  room.xy1.y = 250;
  room.xy2.x = 300;
  room.xy2.y = 300;

  actreg = GetActiveRegion();

  StartRegion();

  AddRegion(&room);

  ioreg = EndRegion();

  if((actreg != NULL)&&(ioreg != NULL))
  {
    newreg = SubRegion(actreg, ioreg);
    ActivateRegion(newreg);
//    FreeRegion(&actreg);
    FreeRegion(&ioreg);
  }

  return(newreg);
}
*/

extern _DWORD crc_numeric(_RESULT *pr, _VARIABLE *pv)
{
  _RESULT tmp;
  _DWORD  retcode;
  _UDWORD dtmp;

  retcode = pr->value;
  if(pv->info.formula != DEFFORMULA)
  {
    tmp = *pr;
    if(recalculate(&tmp, pv, _TRUE) == _TRUE)
      retcode = tmp.value;
  }

  dtmp    = (pv->info.posdezpt << 4) | pv->info.digits;
  retcode = retcode ^ (dtmp << 16);
  if(get_commasign_Makro() == _TRUE)
    retcode ^= 0x80000000L;

  return(retcode);
}

extern _DWORD crc_ipaddress(_RESULT *pr)
{
  return pr->value;
}

_DWORD crc_date(_RESULT *pr)
{
  _DWORD retcode = pr->value;
  if(get_americandateformat_Makro() == _TRUE)
    retcode ^= 0x80000000L;
  return retcode;
}

_DWORD crc_time(_RESULT *pr)
{
  _DWORD retcode = pr->value;
  if(get_americantimeformat_Makro() == _TRUE)
    retcode ^= 0x80000000L;
  return retcode;
}

/*
extern _DWORD crc_date(_DATE *pd)
{
  _DWORD retcode;
  DATE_to_lasal(&retcode, pd);
  if(get_americandateformat() == _TRUE)
    retcode ^= 0x80000000L;

  return(retcode);
}
extern _DWORD crc_time(_TIME *pt)
{
  _DWORD retcode;
  TIME_to_lasal(&retcode, pt);
  if(get_americantimeformat() == _TRUE)
    retcode ^= 0x80000000L;
  return(retcode);
}
*/


_CRC32 crc_string(void *ptr, _UWORD size)
{
  return StrCrc(ptr, size);
//  if(ptr != NULL)
//  {
//    if(size == 1)
//      return(MakeCrc32(0, ptr, StrLen(ptr, 1)));
//
//    return(MakeCrc32(0, ptr, StrLen(ptr, sizeof(_UNI)) * sizeof(_UNI)));
//  }
//  
//  return 0;
}

_DWORD crc_tbo(_IO *pio)
{
  _UDWORD retcode = pio->info.tbo.list;
  return((_DWORD)((retcode << 16) | pio->info.tbo.no));
}

_DWORD crc_VARLABEL(_VARLABEL *pv)
{
  _VARIABLE variable;
  _RESULT   result;
  _DWORD    retcode = 0;

  if(VarList.GetSystemData(&result, &variable, pv, _TRUE, _FALSE) == _TRUE)
  {
    switch(result.ftype)
    {
      case RES_ANUMERIC : retcode = crc_string(result.ptr, sizeof(_CHAR)); break;
      //case RES_TIME     : retcode = crc_time(&result.datim.ftime); break;
      //case RES_DATE     : retcode = crc_date(&result.datim.fdate); break;
      case RES_TIME     : retcode = crc_time(&result); break;
      case RES_DATE     : retcode = crc_date(&result); break;
      default           : retcode = result.value; break;
    }
  }

  return(retcode);
}

_DWORD crc_MEM_VARLABEL(_MEM_VARLABEL *p)
{
  _UWORD    no;
  _UDWORD   retcode = 0;
  _VARLABEL *pvl;
  _UDWORD   st;

  if(p != NULL)
  {
    no  = p->no;
    pvl = p->ptr;
    while(no > 0)
    {
      st      = retcode & 0x8000L;
      retcode = retcode << 1;
      if(st)
        retcode |= 1;
      retcode = retcode ^ (_UDWORD)crc_VARLABEL(pvl);
      no  --;
      pvl ++;
    }
  }

  return((_DWORD)retcode);
}

_DWORD crc_txtscheme(_IO *pio, _RESULT *pr)
{
  _VARLABEL    *pv;
  _MYTXTSCHEME *pm = &pio->info.mytxtscheme;
  _UDWORD      msb, retcode = pm->anypara.anything.list;
  _UWORD       i;

  retcode = retcode ^ pr->value;
  retcode = (retcode << 16)|pm->anypara.anything.no;

  pv = pm->anypara.memvarlabel.ptr;
  for(i=0; i<pm->anypara.memvarlabel.no; i++, pv++)
  {
    msb     = retcode & 0x80000000L;
    retcode = ((retcode << 1)^(_UDWORD)crc_VARLABEL(pv));
    if(msb)
      retcode ^= 1;
  }

  return((_DWORD)retcode);
}

_DWORD crc_bar(_RESULT *pr)
{
  return((((pr->rmin << 1) ^ (pr->rmax)) << 1) ^ pr->value);
}

_DWORD crc_THEURGE(_THEURGE *p, _RESULT *pr, _VARIABLE *pv)
{
  _DWORD crc;
  _COLOR col;
  _RESULT result;
  _VARIABLE var;
  _UWORD i;
  _DYNCOLOR *pdc;

  crc = crc_numeric(pr, pv);

  if(p != NULL)
  {
    for(i=0; i<p->memvarlabel.no; i++)
    {
      if(VarList.GetSystemData(&result, &var, &p->memvarlabel.ptr[i], _FALSE, _TRUE) == _TRUE)
      {
        if(crc & 0x80000000)
          crc = ((crc << 1) | 1);
        else
          crc = (crc << 1);

        crc ^= crc_numeric(&result, &var);
      }
    }

    col = DEFCOLOR;
    pdc = p->memdyncolor.ptr;

    for(i=0; i<p->memdyncolor.no; i++)
    {
      if(ColorScheme.Call(&col, &pdc->linkscheme, p->memcolor.ptr[i]) == _TRUE)
      {
        if(col != pdc->color)
        {
          pdc->color = MakeColor_Makro(INVISIBLE, col);

          if(crc & 0x80000000)
            crc = ((crc << 1) | 1);
          else
            crc = (crc << 1);

          crc ^= col;
        }
      }
      pdc++;
    }
  }

  return(crc);
}

_BOOL _SubPicture::StandardSize(_IO *pio, _ROOM *pr)
{
//  pr->xy1.x = Zoom(zoom->x, pio->room.xy1.x);
//  pr->xy2.x = Zoom(zoom->x, pio->room.xy2.x);
//  pr->xy1.y = Zoom(zoom->y, pio->room.xy1.y);
//  pr->xy2.y = Zoom(zoom->y, pio->room.xy2.y);
  *pr = pio->room;
  return _TRUE;
}

_BOOL _SubPicture::NumericSize(_IO *pio, _ROOM *pr)
{
  //StandardSize(pio, pr);
  *pr = pio->room;
  return _TRUE;
}

_BOOL _SubPicture::ChkNumeric(_RESULT *pr)
{
  if((pr->ftype == RES_NUMERIC)||(pr->ftype == RES_CONSTANT)||(pr->ftype == RES_REAL))
  {
    return _TRUE;
  }

  return _FALSE;
}

_BOOL _SubPicture::ChkAscii(_RESULT *pr)
{
  if(pr->ftype == RES_ANUMERIC)
  {
    return _TRUE;
  }

  return _FALSE;
}

_BOOL _SubPicture::ChkBox(_RESULT *pr)
{
  if((pr->ftype == RES_NUMERIC) || (pr->ftype == RES_CONSTANT))
  {
    return _TRUE;
  }

  return _FALSE;
}

_BOOL _SubPicture::ChkBar(_RESULT *pr)
{
  if((pr->ftype == RES_NUMERIC) || (pr->ftype == RES_CONSTANT))
  {
    return _TRUE;
  }

  return _FALSE;
}

#define SA38879
_BOOL _SubPicture::ChkTxtScheme(_RESULT *pr, _IO *pio)
{
  _VARLABEL vtmp;

  _RESULT_TYPE ftype = pr->ftype;
  _IVALUE value = pr->value;

#ifdef SA38879
  if(ftype == RES_REAL) // SA38879
  {
    _REAL rval = *(_REAL*)&value;  
    rval += (rval < 0)? -0.5 : 0.5;
    value = (_DWORD)rval;
    ftype = RES_NUMERIC;
  }
#endif

  if((ftype == RES_NUMERIC) || (ftype == RES_CONSTANT))
  {
    init_VARLABEL_Makro(&vtmp);
    vtmp.no = 1;
    vtmp.info[0].state = CONST_VAL;
    vtmp.info[0].value = value;
    if(TextScheme.Call(&pio->info.mytxtscheme, &vtmp) == _FALSE)
      free_ANYTHING(&pio->info.mytxtscheme.anypara.anything);
    return _TRUE;
  }

  return _FALSE;
}
#undef SA38879

_BOOL _SubPicture::ChkTBO(_RESULT *pr, _IO *pio)
{
  _VARLABEL vtmp;
  _UDWORD   dst;

  if((pr->ftype == RES_NUMERIC) || (pr->ftype == RES_CONSTANT))
  {
    switch(pio->info.tbo.list)
    {
      case IMAGESCHEME  : init_VARLABEL_Makro(&vtmp);
                          vtmp.no = 1;
                          vtmp.info[0].state = CONST_VAL;
                          vtmp.info[0].value = pr->value;
                          if(ImageScheme.Call(&dst, &pio->info.myscheme, &vtmp, 0xFFFF) == _TRUE)
                          {
                            pio->info.tbo.list = ICONLST;
                            pio->info.tbo.no   = (_UWORD)dst;
                          }
                          break;
      case OBJECTSCHEME : init_VARLABEL_Makro(&vtmp);
                          vtmp.no = 1;
                          vtmp.info[0].state = CONST_VAL;
                          vtmp.info[0].value = pr->value;
                          if(ObjectScheme.Call(&dst, &pio->info.myscheme, &vtmp, 0xFFFF) == _TRUE)
                          {
                            pio->info.tbo.list = OBJLST;
                            pio->info.tbo.no   = (_UWORD)dst;
                          }
                          break;
      default           : pio->info.tbo.no += pr->value;
                          break;
    }
    
    return _TRUE;
  }

  return _FALSE;
}

_BOOL _SubPicture::ChkTime(_RESULT *pr)
{
  if(pr->ftype == RES_TIME)
  {
    return _TRUE;
  }

  return _FALSE;
}

_BOOL _SubPicture::ChkDate(_RESULT *pr)
{
  if(pr->ftype == RES_DATE)
  {
    return _TRUE;
  }

  return _FALSE;
}

_BOOL _SubPicture::ChkMenue(_IO *pio, _RESULT *pr)
{
  if((pr->ftype == RES_NUMERIC) || (pr->ftype == RES_CONSTANT))
  {
    if(pio->info.tbo.list == MENULIST)
      return _TRUE;
  }

  return _FALSE;
}

_BOOL _SubPicture::ChkEnumeration(_IO *pio, _RESULT *pr)
{
  _BOOL   retcode = _FALSE;
  _ASCII  text[50];
  _UDWORD menuno;
  _UDWORD varno;

  if(pr->ftype == RES_NUMERIC)
  {
    if(chk_VARLABEL_Makro(&pio->variable) == _TRUE)
    {
      if(pio->variable.info[0].state == VAR_VAL)    
      {
        retcode = _TRUE;
        if((pio->info.tbo.list != MENULIST)||(pio->info.tbo.no >= MenuList.GetNo()))
        {
          retcode = _FALSE;
          varno = pio->variable.info[0].value;
          text[0] = '@';
          text[1] = 'E';
          text[2] = 'N';
          text[3] = 'U';
          text[4] = 'M';
          text[5] = '#';
          DToA(&text[6], varno, 0x0800, sizeof(_ASCII));
          if(MenuList.LseLabel.Seek(&menuno, text) == _FALSE)
          {
            menuno = DEFSCOPE;
            _LSEMENU menu;
            if(MenuList.MakeEnumerationMenu(&menu, varno) == _TRUE)
            {
              if(MenuList.Add(&menu, text) != DEFSCOPE)
              {
                if(MenuList.LseLabel.Seek(&menuno, text) == _FALSE)
                  menuno = 0xFFFFFFFF;
              }
              
              MenuList.DesignEnumerationMenu(menuno, pio, NULL);
            }
          }

          if(menuno < MenuList.GetNo())
          {
            pio->info.tbo.list = MENULIST;
            pio->info.tbo.no = menuno;
            retcode = _TRUE;
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL _SubPicture::ChkVarName(_RESULT *pr)
{
  if((pr->ftype == RES_NUMERIC) || (pr->ftype == RES_CONSTANT))
  {
    return(_TRUE);
  }

  return(_FALSE);
}

_BOOL _SubPicture::ChkTHEURGE(_IO *pio, _RESULT *pr)
{

//  if((pr->ftype == RES_NUMERIC) || (pr->ftype == RES_CONSTANT))
  if(pr->ftype != RES_DEFAULT)
  {
    if(pio->theurge != NULL)
    {
      if(pio->theurge->info != 0xFFFFFFFF)
      {
        return(_TRUE);
      }
    }
  }

  return(_FALSE);
}

_BOOL _SubPicture::OutNumeric(_IO *pio, _RESULT *pr, _VARIABLE *pv)
{
  _CHAR txt[512];

 #ifndef UC_RECALC
  if(pv->info.formula != DEFFORMULA)
    recalculate(pr, pv, _TRUE);
 #endif
  format_value(txt, pr->value, &pv->info, &pio->picformat);
  OutTextAlign(txt, 0, &pio->space, pio->font, pio->attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR));

  return _TRUE;
}

_BOOL _SubPicture::OutIpAddress(_IO *pio, _RESULT *pr, _VARIABLE *pv)
{
  _CHAR  txt[200];

  format_ipaddress(txt, *(_UDWORD*)&pr->value);
  OutTextAlign(txt, 0, &pio->space, pio->font, pio->attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR));

  return _TRUE;
}

_BOOL _SubPicture::OutNumericHide(_IO *pio, _RESULT *pr, _VARIABLE *pv)
{
  _CHAR  tmp[200];
  _UWORD i, len;

 #ifndef UC_RECALC
  if(pv->info.formula != DEFFORMULA)
    recalculate(pr, pv, _TRUE);
 #endif

  tmp[0] = 0;
  if(SuperMode == _TRUE)
    format_value(tmp, pr->value, &pv->info, &pio->picformat);

  if(SetUp.GetDisplayPassword() == _TRUE)
  {
    format_value(tmp, pr->value, &pv->info, &pio->picformat);
    len = StrLen(tmp, sizeof(_CHAR));
    for(i=0; i<len; i++)
      tmp[i] = '*';
  }

  OutTextAlign(tmp, 0, &pio->space, pio->font, pio->attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR));

  return _TRUE;
}

_BOOL _SubPicture::OutAscii(_IO *pio, _RESULT *pr, _VARIABLE *pv)
{
  _CHAR tmp[1024]; // obechr

  format_string_sub(tmp, (_CHAR*)pr->ptr, &pv->info, (sizeof(tmp) / sizeof(_CHAR)) - 1);
  //OutTextAlign(tmp, 0, &pio->space, pio->font, pio->attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR));
  OutMultiTextAlign(tmp, 0, &pio->space, pio->font, pio->attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR)); // wunsch montagsbesprechung applikation: schmidt werner

  return _TRUE;
}

_BOOL _SubPicture::OutAsciiHide(_IO *pio, _RESULT *pr, _VARIABLE *pv)
{
  _CHAR tmp[200];

  tmp[0] = 0;
  if(SuperMode == _TRUE)
  {
    format_string_sub(tmp, (_CHAR*)pr->ptr, &pv->info, (sizeof(tmp) / sizeof(_CHAR)) - 1);
  }
  
  if(SetUp.GetDisplayPassword() == _TRUE)
  {
    format_string_sub(tmp, (_CHAR*)pr->ptr, &pv->info, (sizeof(tmp) / sizeof(_CHAR)) - 1);
    MakeStars(tmp, lib_arraysize(tmp), tmp);
  }

  OutTextAlign(tmp, 0, &pio->space, pio->font, pio->attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR));

  return _TRUE;
}

_BOOL _SubPicture::OutRadio(_IO *pio, _RESULT *pr)
{
  _COLOR colback = pio->colback;

 #ifdef UC_THEME
  if(pio->frametype >= _THEMEFRAME)
  {
    if(pr->value != 0)
      draw_THEME(&pio->space, pio->frametype, pio->attrib ^ T_INVRS, pio->colback, _TRUE, _FALSE);
    else
      draw_THEME(&pio->space, pio->frametype, pio->attrib, pio->colback, _FALSE, _FALSE);
    colback = MakeColor_Makro(INVISIBLE, INVISIBLE);
  }
 #endif
  out_radiobutton(&pio->space, pr->value, pio->attrib, pio->coltxt, colback, pio->colframe);
  
  return _TRUE;
}

_BOOL _SubPicture::OutChkBox(_IO *pio, _RESULT *pr)
{
  _COLOR colback = pio->colback;

 #ifdef UC_THEME
  if(pio->frametype >= _THEMEFRAME)
  {
//    if(pr->value != 0)
//      draw_THEME(&pio->space, pio->frametype, pio->attrib ^ T_INVRS, pio->colback, _TRUE, _FALSE);
//    else
    draw_THEME(&pio->space, pio->frametype, pio->attrib, pio->colback, _FALSE, _FALSE);

    colback = MakeColor_Makro(INVISIBLE, INVISIBLE);
  }
 #endif
  out_chkbox(&pio->space, pr->value, pio->attrib, pio->coltxt, colback, pio->colframe);
  
  return _TRUE;
}

void BarBottomTop(_ROOM *r, _RESULT *v, _COLOR coltxt, _COLOR colback, _ATTRIB attrib)
{
  _KOORD height, h;

  height = r->xy2.y - r->xy1.y + 1;
  h      = calculate_bar(v, height);

  if(h != 0)
  {
    Bar(r->xy1.x, r->xy2.y-h, r->xy2.x, r->xy2.y, attrib, coltxt);
  }

  if(h != height)
  {
    if(colback != MakeColor_Makro(INVISIBLE, INVISIBLE))
    {
      Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y-1-h, attrib, colback);
    }
  }
}

void BarTopBottom(_ROOM *r, _RESULT *v, _COLOR coltxt, _COLOR colback, _ATTRIB attrib)
{
  _KOORD height, h;

  height = r->xy2.y - r->xy1.y + 1;
  h      = calculate_bar(v, height);

  if(h != 0)
  {
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy1.y+h, attrib, coltxt);
  }
  if(h != height)
  {
    if(colback != MakeColor_Makro(INVISIBLE, INVISIBLE))
    {
      Bar(r->xy1.x, r->xy1.y+h+1, r->xy2.x, r->xy2.y, attrib, colback);
    }
  }
}

void BarLeftRight(_ROOM *r, _RESULT *v, _COLOR coltxt, _COLOR colback, _ATTRIB attrib)
{
  _KOORD height, h;

  height = r->xy2.x - r->xy1.x + 1;
  h      = calculate_bar(v, height);

  if(h != 0)
  {
    Bar(r->xy1.x, r->xy1.y, r->xy1.x+h, r->xy2.y, attrib, coltxt);
  }
  if(h != height)
  {
    if(colback != MakeColor_Makro(INVISIBLE, INVISIBLE))
    {
      Bar(r->xy1.x+h+1, r->xy1.y, r->xy2.x, r->xy2.y, attrib, colback);
    }
  }
}

void BarRightLeft(_ROOM *r, _RESULT *v, _COLOR coltxt, _COLOR colback, _ATTRIB attrib)
{
  _KOORD height, h;

  height = r->xy2.x - r->xy1.x + 1;
  h      = calculate_bar(v, height);

  if(h != 0)
  {
    Bar(r->xy2.x-h, r->xy1.y, r->xy2.x, r->xy2.y, attrib, coltxt);
  }
  if(h != height)
  {
    if(colback != MakeColor_Makro(INVISIBLE, INVISIBLE))
    {
      Bar(r->xy1.x, r->xy1.y, r->xy2.x-h-1, r->xy2.y, attrib, colback);
    }
  }
}

void BarVertical(_ROOM *r, _RESULT *v, _COLOR coltxt, _COLOR colback, _ATTRIB attrib)
{
  _KOORD  height, zero;
  _ROOM   room;
  _RESULT res;

  if(v->rmin >= 0)
  {
    BarBottomTop(r, v, coltxt, colback, attrib);
  }
  else if(v->rmax <= 0)
  {
    BarBottomTop(r, v, coltxt, colback, attrib);
  }
  else
  {
    height = r->xy2.y - r->xy1.y + 1;
    zero = 0;
    if(v->rmax != v->rmin)
      zero = (height * v->rmax)/(v->rmax - v->rmin);

    if(v->value >= 0)
    {
      room       = *r;
      room.xy2.y = room.xy1.y + zero;
      res        = *v;
      res.rmin   = 0;
      BarBottomTop(&room, &res, coltxt, colback, attrib);
      if(colback != MakeColor_Makro(INVISIBLE, INVISIBLE))
      {
        Bar(r->xy1.x, r->xy1.y+zero+1, r->xy2.x, r->xy2.y, attrib, colback);
      }
    }
    else
    {
      room       = *r;
      room.xy1.y = room.xy1.y + zero;
      res        = *v;
      res.rmax   = -res.rmin;
      res.rmin   = 0;
      res.value  = -res.value;
      BarTopBottom(&room, &res, coltxt, colback, attrib);
      if(colback != MakeColor_Makro(INVISIBLE, INVISIBLE))
      {
        Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy1.y+zero-1, attrib, colback);
      }
    }
  }
}

void BarHorizontal(_ROOM *r, _RESULT *v, _COLOR coltxt, _COLOR colback, _ATTRIB attrib)
{
  _KOORD  height, zero;
  _ROOM   room;
  _RESULT res;

  if(v->rmin >= 0)
  {
    BarLeftRight(r, v, coltxt, colback, attrib);
  }
  else if(v->rmax <= 0)
  {
    BarLeftRight(r, v, coltxt, colback, attrib);
  }
  else
  {
    height = r->xy2.x - r->xy1.x + 1;
    zero   = 0;
    if(v->rmax != v->rmin)
      zero = (height * v->rmax)/(v->rmax - v->rmin);

    if(v->value >= 0)
    {
      room       = *r;
      room.xy1.x = room.xy2.x - zero;
      res        = *v;
      res.rmin   = 0;
      BarLeftRight(&room, &res, coltxt, colback, attrib);
      if(colback != MakeColor_Makro(INVISIBLE, INVISIBLE))
      {
        Bar(r->xy1.x, r->xy1.y, r->xy1.x+zero-1, r->xy2.y, attrib, colback);
      }
    }
    else
    {
      room       = *r;
      room.xy2.x = room.xy2.x - zero;
      res        = *v;
      res.rmax   = -res.rmin;
      res.rmin   = 0;
      res.value  = -res.value;
      BarRightLeft(&room, &res, coltxt, colback, attrib);
      if(colback != MakeColor_Makro(INVISIBLE, INVISIBLE))
      {
        Bar(r->xy1.x+zero+1, r->xy1.y, r->xy2.x, r->xy2.y, attrib, colback);
      }
    }
  }
}

_BOOL _SubPicture::OutBar(_IO *pio, _RESULT *pr)
{
  _ROOM space;

  if(pio->cstate.init == _TRUE)
    DrawButton(&pio->space, pio->attrib, pio->frametype, MakeColor_Makro(INVISIBLE, INVISIBLE), pio->colframe);

  GetTextRoom(&space, &pio->space, pio->attrib, pio->frametype, _FALSE);
//  FrameInlet(&space, &pio->space, pio->attrib);

  switch(pio->info.tbo.no)
  {
    case  0 : BarBottomTop(&space, pr, pio->coltxt, pio->colback, pio->attrib); break; // vertical +
    case  1 : BarTopBottom(&space, pr, pio->coltxt, pio->colback, pio->attrib); break; // vertical -
    case  2 : BarLeftRight(&space, pr, pio->coltxt, pio->colback, pio->attrib); break; // horizontal +
    case  3 : BarRightLeft(&space, pr, pio->coltxt, pio->colback, pio->attrib); break; // horizontal -
    case  4 : BarVertical(&space, pr, pio->coltxt, pio->colback, pio->attrib); break;   // format vertical
    default : BarHorizontal(&space, pr, pio->coltxt, pio->colback, pio->attrib); break; // format horizontal
  }

  return _TRUE;
}

_BOOL _SubPicture::OutTxtScheme(_IO *pio)
{
  _CHAR *txt = (_CHAR*)get_ANYPARA(&pio->info.mytxtscheme.anypara, &Picture.TextPuffer);
  if(txt != NULL)
    OutMultiTextAlign(txt, pio->fillchr, &pio->space, pio->font, pio->attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR));

  return _TRUE;
}

/*
_Picture* seek_MY_PICTURE(_SubPicture *Sp, _Window *pw)
{
  _Picture    *pp;
  _MY_PICTURE *mp;
  _UWORD      i;

  if(pw != NULL)
  {
    pp = pw->GetP_Picture();
    if(pp != NULL)
    {
      mp = pp->GetMyPicture();

      for(i=0; i<mp->No; i++)
      {
        if(Sp == &mp->Ptr[i])
          return(pp);
      }
    }

    return(seek_MY_PICTURE(Sp, pw->GetP_Background()));
  }

  return(NULL);
}
*/

_BOOL _SubPicture::OutTBO(_IO *pio, _Picture *Ppic)
{
  _CHAR       *txt;
  _CHAR       tmp[300];
  _ANYTHING   anything;
  _ATTRIB     attrib;
  _BMP        *bmp;
  _BOOL       recalc_memseek, notobj;
  _ZOOM       tmp_zoom;

  anything = pio->info.tbo;
  attrib   = pio->attrib;

  switch(anything.list)
  {
    case OBJLST  : recalc_memseek = _FALSE;
                   if(pio->workstate == STATE_START)
                   {
                     pio->workstate = STATE_ERROR;
                     notobj = _TRUE; // annahme: kein object vorhanden
                     if(pio->Obj != NULL)
                     {
                       notobj = _FALSE; // objekt doch vorhanden
                       pio->Obj->Free();
                     }

                     if(load_object(&pio->Obj, anything.no) == _FALSE)
                     {
                       Memory2((void**)&pio->Obj, 0);
                     }
                     else
                     {
                       notobj = _FALSE; // neues objekt vorhanden
                      #ifdef UC_CHANGE_IO_GROUP
                       change_SubPicture_group(pio->Obj, pio->imiogroupchanged.summary, pio->imiogroupchanged.serverno);
                      #endif
                       pio->Obj->Overload(&pio->imoverloaded);
                       CallUserOverload(pio->Obj, pio->summary);
                     }

                     if((IsDrawBackGround() == _FALSE) && (IsDrawPicture() == _FALSE) && (notobj == _FALSE))
                     {
                       recalc_memseek = _TRUE;
                     }
                   }
                   if(pio->Obj != NULL)// && (pio->workstate == STATE_READY))
                   {
                     Make_ZOOM(&tmp_zoom, pio->Obj, &pio->space);
                     //init_ZOOM(&tmp_zoom); // = pio->zoom;
                     ScreenStop(); // sa26037
                     pio->Obj->Draw(&pio->space.xy1, &tmp_zoom, Ppic); //NULL);
                     ScreenStart(); // sa26037
                     pio->workstate = STATE_RUN;
                   }

                   if(recalc_memseek == _TRUE)
                   {
                     if(Ppic != NULL)
                     {
                       // alle eingaben und editor-actpos neu berechnen
                       _MY_PICTURE *mp = Ppic->GetMyPicture();
                       if(mp->Editor.actno < mp->memseekelement.no)
                       {
                         _KOORD x, y;
                         x = (mp->Editor.input.io.space.xy1.x + mp->Editor.input.io.space.xy2.x) / 2;
                         y = (mp->Editor.input.io.space.xy1.y + mp->Editor.input.io.space.xy2.y) / 2;
                         if(mp->P_Window != NULL)
                           Ppic->SetRecalcIoID(_TRUE, x, y);
                           //mp->P_Window->AddCmd(CMD_RECALC_IOID, x, y, _FALSE);
                       }
                       else
                       {
                         Ppic->SetRecalcIoID(_TRUE, 0, 0);
                       }
                     }
                   }
                   break;
    case ICONLST : bmp = (_BMP*)get_ANYTHING(&anything, NULL);
                   if(bmp != NULL)
                   {
                    #ifdef THR
					           // THR V1.40: Activate the region in case the space changed
//					           _REGION newreg, actreg;
                     _KOORD x, y;

//                     actreg = GetActiveRegion();
//                     StartRegion();
//                     AddRegion(&pio->space);
//                     newreg = EndRegion();
//                     ActivateRegion(newreg);  // set new clipping

                     if(bmp->width != 0)
                     {
                       switch(attrib & T_HORIZBOUND)
                       {
                         case T_RIGHTBOUND  : x = pio->space.xy2.x - bmp->width + 1; break;
                         case T_CENTERBOUND : x = (pio->space.xy1.x + pio->space.xy2.x + 1 - bmp->width)/2; break;
                         default            : x = pio->space.xy1.x; break;
                       }

                       switch(attrib & T_VERTBOUND)
                       {
                         case T_DOWNBOUND   : y = pio->space.xy2.y - bmp->height + 1; break;
                         case T_MIDBOUND    : y = (pio->space.xy1.y + pio->space.xy2.y + 1 - bmp->height)/2; break;
                         default            : y = pio->space.xy1.y; break;
                       }

                       Bitmap(x, y, bmp, attrib&(~T_SIZE), 0);
                     }

                     // THR V1.40: Deactivate the region
//                     DeactivateRegion();
//                     free_REGION(&newreg);
//                     ActivateRegion(actreg);  // restore old clipping
                    #else
                     out_zoom_BMP(&pio->space, bmp, attrib&(~T_SIZE));
                    #endif
                   }
                   break;
    default      : txt = (_CHAR*)get_ANYTHING(&anything, &Picture.TextPuffer);
                   if(txt != NULL)
                   {
                     StrCpy(tmp, sizeof(_CHAR), txt, sizeof(_CHAR));
                     OutMultiTextAlign(tmp, pio->fillchr, &pio->space, pio->font, attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR));
                   }
                   break;
  }

  return _TRUE;
}

_BOOL _SubPicture::OutTime(_IO *pio, _RESULT *pr, _VARIABLE *pv)
{
  _CHAR tmp[100];

  format_time(tmp, &pr->datim.ftime, pv->info.format);
  OutTextAlign(tmp, 0, &pio->space, pio->font, pio->attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR));

  return _TRUE;
}

_BOOL _SubPicture::OutDate(_IO *pio, _RESULT *pr, _VARIABLE *pv)
{
  _CHAR tmp[100];

  format_date(tmp, &pr->datim.fdate, pv->info.format);
  OutTextAlign(tmp, 0, &pio->space, pio->font, pio->attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR));

  return _TRUE;
}

#ifdef UC_TRIANGLE_INPUT
void out_triangle(_ROOM *pr, _COLOR color)
{
 #ifdef OLD_TRIANGLE_INPUT
  _DOT    dot[3];
  dot[0].x = pr->xy2.x;
  dot[0].y = pr->xy2.y-8;
  dot[1]   = pr->xy2;
  dot[2].x = pr->xy2.x-8;
  dot[2].y = pr->xy2.y;
  Polygon(3, dot, T_COPY|T_SOLID|T_FILL, color);
 #else
  _DOT dot[8];
  dot[0].x = pr->xy2.x;
  dot[0].y = pr->xy2.y-8;
  dot[1]   = pr->xy2;
  dot[2].x = pr->xy2.x-8;
  dot[2].y = pr->xy2.y;
  dot[3].x = pr->xy2.x-6;
  dot[3].y = pr->xy2.y-2;
  dot[4].x = pr->xy2.x-8;
  dot[4].y = pr->xy2.y-4;
  dot[5].x = pr->xy2.x-4;
  dot[5].y = pr->xy2.y-8;
  dot[6].x = pr->xy2.x-2;
  dot[6].y = pr->xy2.y-6;
  dot[7]   = dot[0];
  Polygon(8, dot, T_COPY|T_SOLID, color);
 #endif
}
#endif

_BOOL _SubPicture::OutMenu(_IO *pio, _RESULT *pr, _BOOL input)
{
 #ifdef UC_MENU_TWOCOLOR
  _UDWORD actstschemestate;
 #endif
  MenuList.OutIO(pio, pr->value);

  if(input == _TRUE)
  {
	#ifndef THR
	#ifdef UC_TRIANGLE_INPUT
	 _ROOM zroom;
	 GetTextRoom(&zroom, &pio->space, pio->attrib, pio->frametype, _FALSE);
//    FrameInlet(&zroom, &pio->space, pio->attrib);
   #ifdef UC_MENU_TWOCOLOR
     if(StateScheme.Call(&actstschemestate, &pio->stscheme, NULL, 0, _TRUE) == _TRUE)
     {
       if(actstschemestate == 0)
       {
         out_triangle(&zroom, pio->coltxt);
       }
     }
   #else
     out_triangle(&zroom, pio->coltxt);
   #endif
	#endif
	#endif
  }

  return _TRUE;
}

_BOOL _SubPicture::OutVarName(_IO *pio, _RESULT *pr, _VARIABLE *pv, _BOOL input)
{
  _ASCII txt[100];
  _CHAR  tmp[100];
  _UBYTE varobj;
  _UDWORD no;

  no = VarList.GetVarNoByID(pr->value, pv->info.station, _FALSE);
  if(VarList.GetLabel(txt, &varobj, no) == _FALSE)
	 StrCpy(txt, sizeof(_ASCII), "no connection", sizeof(_ASCII));

  AsciiToUni((_UNI*)tmp, txt, 0);
  OutTextAlign(tmp, 0, &pio->space, pio->font, pio->attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR));

  if(input == _TRUE)
  {
	#ifndef THR
	#ifdef UC_TRIANGLE_INPUT
	 _ROOM zroom;
	 FrameInlet(&zroom, &pio->space, pio->attrib);
	 out_triangle(&zroom, pio->coltxt);
	#endif
	#endif
  }

  return _TRUE;
}

_BOOL _SubPicture::SingleSizeIo(_IO *pio, _DOT *offset, _ZOOM *zoom)
{
  switch(pio->typ)
  {
    case AS_NUMERIC_HIDE :  // value as stars
    case AS_ASCII_HIDE   :
    case AS_NUMERIC      :
    case AS_ASCII        : 
                           NumericSize(pio, &pio->space); 
                           Zoom_ROOM(&pio->space, offset, zoom);
                           return _TRUE;
    case AS_RADIO        :
    case AS_CHKBOX       :
    case AS_BAR          :
    case AS_TBO          :
    case AS_TXTSCHEME    :
    case AS_DATE         :
    case AS_TIME         :
    case AS_MENU         :
    case AS_USER         : 
    case AS_VARNAME      :
    case AS_IPADDRESS    :
    case AS_ENUMERATION  :
    case AS_THEURGE      : 
                           StandardSize(pio, &pio->space); 
                           Zoom_ROOM(&pio->space, offset, zoom);
                           return _TRUE;
  }


  return _FALSE;
}

_BOOL _SubPicture::SingleCheckIo(_IO *pio, _RESULT *pr)
{
  _BOOL retcode = _FALSE;

 //#ifdef USER_DEKEMA
  if(pio->virtname.pthis != NULL)
  {
    retcode = _TRUE;
  }
  else
 //#endif
  {
    switch(pio->typ)
    {
      case AS_IPADDRESS    : 
      case AS_NUMERIC_HIDE :
      case AS_NUMERIC      : retcode = ChkNumeric(pr);    break;
      case AS_ASCII_HIDE   :
      case AS_ASCII        : retcode = ChkAscii(pr);      break;
      case AS_RADIO        : retcode = ChkBox(pr);        break;
      case AS_CHKBOX       : retcode = ChkBox(pr);        break;
      case AS_BAR          : retcode = ChkBar(pr);        break;
      case AS_TBO          : retcode = ChkTBO(pr, pio);   break;
      case AS_TXTSCHEME    : retcode = ChkTxtScheme(pr, pio); break;
      case AS_DATE         : retcode = ChkDate(pr);       break;
      case AS_TIME         : retcode = ChkTime(pr);       break;
      case AS_MENU         : retcode = ChkMenue(pio, pr); break;
      case AS_VARNAME      : retcode = ChkVarName(pr);    break;
      case AS_USER         : retcode = _TRUE;             break;
      case AS_THEURGE      : retcode = ChkTHEURGE(pio, pr); break;
      case AS_ENUMERATION  : retcode = ChkEnumeration(pio, pr); break;
      default              : break;
    }
  }

 #ifdef OK_SUMMARY
  if(pio->summary & 0x8000)
  {
    pio->colback  = SetUp.GetEditColorBack(_SELECTGROUP, pio->colback);
    pio->colframe = SetUp.GetEditColorFrame(_SELECTGROUP, pio->colframe);
    pio->coltxt   = SetUp.GetEditColorTxt(_SELECTGROUP, pio->coltxt);
    pio->attrib   = (pio->attrib & (~T_PATTERN)) | T_SOLID;
    pio->is_dyncolor = _FALSE;
  }
 #endif

  return(retcode);
}


#ifdef XUSER_HIRSCH
_COLOR AccessColor(_UDWORD access, _LINKSCHEME *colscheme, _COLOR defcol, _BOOL is_input)
{
  //if(colscheme->myscheme.schemeno == 1)
  //{
    
  //}
  
  _LINKSCHEME tmp = *colscheme;
  
  tmp.varlabel.no = 1;
  tmp.varlabel.info[0].state = CONST_VAL;
  tmp.varlabel.info[0].value = ((get_abstime() % 2000) > 1000)? 0 : 1;
  
  return DoColorScheme(&tmp, (((get_abstime() % 2000) > 1000)? LIGHTGREEN : LIGHTRED));
  //return (((get_abstime() % 2000) > 1000)? LIGHTGREEN : LIGHTRED);
}
#endif


_BOOL _SubPicture::SingleChangeIo(_IO *pio, _RESULT *pr, _VARIABLE *pv, _Picture *Ppic, _BOOL input)
{
  _BOOL   retcode = pio->cstate.init;
  _UDWORD backhi, backlo, framehi, framelo, txt;
  _UDWORD crc;
 #ifdef UC_MENU_TWOCOLOR
  _UDWORD actstschemestate;
 #endif

  switch(pio->typ)
  {
    //case AS_DATE       : crc = crc_date(&pr->datim.fdate); break;
    //case AS_TIME       : crc = crc_time(&pr->datim.ftime); break;
    case AS_DATE       : crc = crc_date(pr); break;
    case AS_TIME       : crc = crc_time(pr); break;
    case AS_ASCII_HIDE :
    case AS_ASCII      : crc = crc_string(pr->ptr, sizeof(_CHAR)); break;
    case AS_BAR        : crc = crc_bar(pr); break;
    case AS_IPADDRESS  : crc = crc_ipaddress(pr); break;
    case AS_NUMERIC_HIDE :
    case AS_NUMERIC    : crc = crc_numeric(pr, pv); break;
    case AS_TBO        : crc = crc_tbo(pio); break;
    case AS_TXTSCHEME  : crc = crc_txtscheme(pio, pr); break;
    case AS_THEURGE    : crc = crc_THEURGE(pio->theurge, pr, pv); break;
    default            : crc = pr->value; break;
    
  }

  crc = crc ^ pr->myiocrc;
  
  if((pio->typ == AS_TBO) && (pio->cstate.init == _TRUE))
  {
    if((pio->cstate.value != crc) && (pio->info.tbo.list == OBJLST))//||(DrawBackGround != 0))
      pio->workstate = STATE_START;  // necessary to load new object from objectlist
  }

//  if(pio->typ == AS_THEURGE)  // just to get dyncolor's if init == _TRUE
//  {
//    crc = crc_THEURGE(pio->theurge, pr, pv);
//  }
//  else if((pio->typ == AS_TBO)&&(pio->info.tbo.list == OBJLST))
  if((pio->typ == AS_TBO)&&(pio->info.tbo.list == OBJLST))
  {
    if(DrawBackGround == 0)      // when background-restore object is already loaded
      pio->workstate = STATE_START;  // necessary to load new object from objectlist

    if((pio->cstate.value != crc) && (pio->cstate.init != _TRUE))
    {
      if(Ppic != NULL)
      {
        if(lookup_input_in_object(pio, Ppic->GetMyPicture()) == _TRUE)
        {
          Ppic->GetMyPicture()->P_Window->CMD_KillEditor();
         #ifdef FIRSTINPUT_ALT 
          Ppic->GetMyPicture()->P_Window->AddCmd(CMD_AUTOSEEKINPUT, 0, 0, _FALSE);
         #endif // FIRSTINPUT_ALT
        }
      }
    }

  }


 #ifdef UC_MENU_TWOCOLOR
  if(pio->typ == AS_MENU) 
  {
    if(StateScheme.Call(&actstschemestate, &pio->stscheme, NULL, 0, _TRUE) == _TRUE)
    {
      if(retcode == _FALSE)
      {
        if(actstschemestate != pio->refstschemestate)
        {
          retcode = _TRUE;
        }
      }
      pio->refstschemestate = actstschemestate;
    }
  }
 #endif


  if(pio->is_dyncolor == _TRUE)
  {
    if(ColorScheme.Call(&backhi, &pio->dyncolback, GetBackColor_Makro(pio->colback), _TRUE) == _TRUE)
    {
      if(ColorScheme.Call(&backlo, &pio->dyncolfront, GetPenColor_Makro(pio->colback), _TRUE) == _TRUE)
      {
        if(ColorScheme.Call(&framehi, &pio->dyncoltl, GetBackColor_Makro(pio->colframe), _TRUE) == _TRUE)
        {
          if(ColorScheme.Call(&framelo, &pio->dyncolbr, GetPenColor_Makro(pio->colframe), _TRUE) == _TRUE)
          {
            if(ColorScheme.Call(&txt, &pio->dyncoltxt, GetPenColor_Makro(pio->coltxt), _TRUE) == _TRUE)
            {
              pio->colback  = MakeColor_Makro((_UWORD)backhi, (_UWORD)backlo);
              pio->colframe = MakeColor_Makro((_UWORD)framehi, (_UWORD)framelo);
              pio->coltxt   = MakeColor_Makro((_UWORD)txt, (_UWORD)txt);

             #ifndef USER_HIRSCH
              if(retcode == _FALSE)
                if((pio->colframe != pio->refcolframe)||(pio->colback != pio->refcolback)||(pio->coltxt != pio->refcoltxt))
                  retcode = _TRUE;

              pio->refcolback  = pio->colback;
              pio->refcolframe = pio->colframe;
              pio->refcoltxt   = pio->coltxt;
             #endif
            }
          }
        }
      }
    }
  } 

 #ifdef USER_HIRSCH
  if (input == _TRUE)// christoph
  {
    pio->colback = AccessColor(pv->info.accessno, &pio->dyncolfront, pio->colback, _TRUE); // vorsicht: immer linker error weil funktion vom kunden fehlt
    crc ^= pio->colback;
  }
  
  if((pio->colframe != pio->refcolframe)||(pio->colback != pio->refcolback)||(pio->coltxt != pio->refcoltxt))
    retcode = _TRUE;

  pio->refcolback  = pio->colback;
  pio->refcolframe = pio->colframe;
  pio->refcoltxt   = pio->coltxt;
 #endif
   
  if(pio->cstate.value != crc)
  {
    pio->cstate.value = crc;
    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL _SubPicture::SingleOutIo(_IO *pio, _RESULT *pr, _VARIABLE *pv, _DOT *offset, _ZOOM *zoom, _BOOL input, _UWORD actno, _Picture *Ppic, _BOOL iseditor)
{
  _BOOL   retcode = _FALSE;
  _REGION actreg, ioreg, newreg;
  _BOOL   is, imagestate;
  _BOOL   doregion;
  _ZOOM   tmp_zoom;
 #ifndef UC_THEME
  _COLOR  tc;
 #endif
 #ifdef UC_MOUSE
  _UWORD  hide;
 #endif

  if(SingleCheckIo(pio, pr) == _TRUE)
  {
    if(SingleChangeIo(pio, pr, pv, Ppic, input) == _TRUE)
    {
      if(SingleSizeIo(pio, offset, zoom) == _TRUE)
      {
        if(iseditor == _TRUE)
        {
          give_editorcolor(pio, _EDITACTIVE);
         #ifndef UC_THEME
          if(Ppic != NULL)
          {
            if(Ppic->Cursor.Get_XCURSOR()->visible)
            {
              tc = MakeColor_Makro(Ppic->Cursor.Get_XCURSOR()->color, Ppic->Cursor.Get_XCURSOR()->color);
              pio->coltxt   ^= tc;
              pio->colback  ^= tc;
            }
          }
         #endif
          //Ppic->Cursor.Hide();
        }

        imagestate = _TRUE;
        if((pio->image.ptr == NULL) && (pio->cstate.init == _TRUE)) //(DrawBackGround == 0))
        {
          if((pio->typ == AS_THEURGE) && (pio->theurge != NULL))
          {
            if(pio->theurge->memimage.no != 0)
            {
              if((pio->theurge->memimage.ptr[0].list == ICONLST) && (pio->theurge->memimage.ptr[0].no != 0xffff))
                out_ANYTHING(&pio->theurge->memimage.ptr[0], &pio->space, DEFFONT, T_COPY|T_SOLID, _DEFFRAME, DEFCOLOR, DEFCOLOR, DEFCOLOR, NULL);
            }
          }

          if(pio->doimage == _TRUE)
          {
            imagestate = _FALSE;
           #ifdef UC_MOUSE
            hide = IfNecHideMouseCursor(&pio->space);
           #endif
            GetImage(&pio->image, pio->space.xy1.x, pio->space.xy1.y, pio->space.xy2.x, pio->space.xy2.y);
           #ifdef UC_MOUSE
            IfNecShowMouseCursor(hide);
           #endif
          }
        }

       #ifdef UC_RECALC
        retcode = _TRUE;
        if((pio->typ == AS_NUMERIC)||(pio->typ == AS_NUMERIC_HIDE)) // || (pio->typ == AS_THEURGE))
        {
          if(pv->info.formula != DEFFORMULA)
            retcode = recalculate(pr, pv, _TRUE);
        }
        if(retcode == _TRUE)
       #endif
        {
          doregion = _TRUE;
         #ifdef _NO_VIEWPORT
          if((pio->typ == AS_NUMERIC)&&(pio->virtname.pthis == NULL))
          {
            // bringt zeit weil bei numerischer anzeige in ausgaberoutine das viewport gesetz wird
            doregion = _FALSE;
          }
         #endif
         #ifdef UC_MOUSE
          hide = IfNecHideMouseCursor(&pio->space);
         #endif
          ioreg  = NULL;
          newreg = NULL;
          if(doregion == _TRUE)
          {
            //_ROOM tr = pio->space;
            //tr.xy2.y -= 10;
            //actreg = ActivateAndRegion(&ioreg, &newreg, &tr);
            actreg = ActivateAndRegion(&ioreg, &newreg, &pio->space);
          }

          retcode = _FALSE;

         #ifdef UC_THEME
		  if(iseditor == _TRUE)
            Cursor_Push(NULL);
         #endif

          if(OutIO(pio, pr, pv, input) == _TRUE)
          {
            if((pio->image.ptr != NULL) && (imagestate == _TRUE))
            {
              switch(pio->typ)
              {
               #ifdef THR
                case AS_TXTSCHEME :
                case AS_TBO       : is = _FALSE;
               #else
                case AS_TXTSCHEME :
                case AS_TBO       : is = _TRUE;
               #endif
                                    break;
                case AS_THEURGE:    is = _FALSE;
                                    break;
                default           : is = is_invisible(pio->colback, pio->attrib);
                                    break;
              }

//              if((is == _TRUE) && (pio->virtname.pthis == NULL))
              if(is == _TRUE)
                PutImage(&pio->image, pio->space.xy1.x, pio->space.xy1.y);
            }

            switch(pio->typ)
            {
              case AS_NUMERIC      : retcode = OutNumeric(pio, pr, pv); break;
              case AS_ASCII        : retcode = OutAscii(pio, pr, pv); break;
              case AS_CHKBOX       : retcode = OutChkBox(pio, pr); break;
              case AS_RADIO        : retcode = OutRadio(pio, pr); break;
              case AS_BAR          : retcode = OutBar(pio, pr); break;
              case AS_TBO          :
                                    #ifdef THR
                                     DeactivateAndRegion(actreg, &ioreg, &newreg);
                                     PutImage(&pio->image, DEFKOORD, DEFKOORD);
                                     actreg = ActivateAndRegion(&ioreg, &newreg, &pio->space);
                                     GetImage(&pio->image, pio->space.xy1.x, pio->space.xy1.y, pio->space.xy2.x, pio->space.xy2.y);
                                    #endif
                                     retcode = OutTBO(pio, Ppic);
                                     break;

              case AS_TXTSCHEME    : retcode = OutTxtScheme(pio); break;
              case AS_DATE         : retcode = OutDate(pio, pr, pv); break;
              case AS_TIME         : retcode = OutTime(pio, pr, pv); break;
              case AS_ENUMERATION  :
              case AS_MENU         : retcode = OutMenu(pio, pr, input); break;
              case AS_VARNAME      : retcode = OutVarName(pio, pr, pv, input); break;
              case AS_NUMERIC_HIDE : retcode = OutNumericHide(pio, pr, pv); break;
              case AS_ASCII_HIDE   : retcode = OutAsciiHide(pio, pr, pv); break;
              case AS_THEURGE      : retcode = OutTHEURGE(pio, pr, pv); break;
              case AS_IPADDRESS    : retcode = OutIpAddress(pio, pr, pv); break;
              default              : break;
            }

          }
          else
          {
            retcode = _TRUE;
          }

         #ifdef UC_THEME
          if(iseditor == _TRUE)
            Cursor_Pop(NULL);
         #endif

          pio->cstate.init  = _FALSE;

         #ifdef OK_ENCLOSE_NAME
          if(pio->virtname.pthis != NULL)
            call_enclosed_name_method(METHOD_MYIO_READY, pio, pv, pr, NULL, NULL, input);
         #endif
//DeactivateRegion();
//FreeRegion(&sepreg);
          if(doregion == _TRUE)
          {
            DeactivateAndRegion(actreg, &ioreg, &newreg);
          }
         #ifdef UC_MOUSE
          IfNecShowMouseCursor(hide);
         #endif
        }
      }
    }
    else
    {
      if(pio->typ == AS_TBO)
      {
        if(pio->info.tbo.list == OBJLST)
        {
          if(pio->Obj != NULL)
          {
            Make_ZOOM(&tmp_zoom, pio->Obj, &pio->space);
//           #ifdef ZOOM_KM
//            _ZOOM kmz = do_kmz(zoom, &pio->kmz);
//            pio->Obj->Refresh(actno, &pio->space.xy1, &kmz, Ppic); //, DEFATTRIB, DEFCOLOR);
//           #else
            pio->Obj->Refresh(actno, &pio->space.xy1, &tmp_zoom, Ppic); //, DEFATTRIB, DEFCOLOR);
//           #endif
          }
        }
      }
    }
/*
if(pio->ioid != 0xFFFF)
{
  _CHAR dum[20];
  DToA(dum, pio->ioid, 0x800, sizeof(_CHAR));
  Font.Set(0xFFFF, NULL);
  OutTextXY(pio->space.xy1.x, pio->space.xy1.y, "     ", T_PROP|T_COPY|T_SOLID, MakeColor_Makro(GREEN, WHITE), sizeof(_CHAR));
  OutTextXY(pio->space.xy1.x, pio->space.xy1.y, dum, T_PROP|T_COPY|T_SOLID, MakeColor_Makro(GREEN, WHITE), sizeof(_CHAR));
}
*/
  }

  return(retcode);
}



