#include "T_IPC.H"
#include "T_NUC.H"

_BOOL check_THEURGE_Bar(_THEURGE*);
_BOOL check_THEURGE_Oszi(_THEURGE*);
_BOOL check_THEURGE_Indicator(_THEURGE*);
_BOOL check_THEURGE_Clock(_THEURGE*);
_BOOL check_THEURGE_Scale(_THEURGE*);
void BarBottomTop(_ROOM*, _IO*, _RESULT*, _BOOL, _BOOL, _BOOL);
void BarTopBottom(_ROOM*, _IO*, _RESULT*, _BOOL, _BOOL, _BOOL);
void BarLeftRight(_ROOM*, _IO*, _RESULT*, _BOOL, _BOOL, _BOOL);
void BarRightLeft(_ROOM*, _IO*, _RESULT*, _BOOL, _BOOL, _BOOL);
void ScaleDraw(_IO*, _VARIABLE*, _RESULT*, _ROOM*, _COLOR, _COLOR, _BOOL);


_DOIT getevent_THEURGE(_EVENT *pev, _EDITOR *pedi)
{
  _DOIT retcode = _IDLE;

  if(pedi->input.io.theurge != NULL)
  {
    switch(pedi->input.io.theurge->urgetype)
    {
      case _URGE_SLIDER : retcode = getevent_SLIDER(pev, pedi);
        break;
    }
  }
  
  return retcode;
}

_DOIT killfocus_THEURGE(_EDITOR *pedi)
{
  _DOIT retcode = _IDLE;

  if(pedi->input.io.theurge != NULL)
  {
    switch(pedi->input.io.theurge->urgetype)
    {
      case _URGE_SLIDER : retcode = killfocus_SLIDER(pedi);
        break;
    }
  }
  
  return retcode;
}

_DOIT getfocus_THEURGE(_EDITOR *pedi, _EVENT *pev)
{
  _DOIT retcode = _IDLE;

  if(pedi->input.io.theurge != NULL)
  {
    switch(pedi->input.io.theurge->urgetype)
    {
      case _URGE_SLIDER : retcode = getfocus_SLIDER(pedi, pev);
        break;
    }
  }
  
  return retcode;
}


_BOOL _SubPicture::GetValueTHEURGE(_RESULT *res, _VARIABLE *var, _IO *pio, _BOOL dolimit)
{
  _BOOL retcode = VarList.GetSystemData(res, var, &pio->variable, _FALSE, dolimit, _TRUE);

  if(pio->theurge != NULL)
  {
    switch(pio->theurge->urgetype)
    {
      case _URGE_DELAYANDRETURN :
        retcode = GetValue_THEURGE_ScreenClean(res, pio);
        break;
      case _URGE_SLIDER :
        retcode = getvalue_SLIDER(res, pio);
        break;
    }
  }
  
  return retcode;
}

//_BOOL _SubPicture::StartTHEURGE()
//{
//}
//
//_BOOL _SubPicture::ReadyTHEURGE()
//{
//}
//
//_BOOL _SubPicture::RunTHEURGE()
//{
//}
//
//_BOOL _SubPicture::EndTHEURGE()
//{
//}

_BOOL _SubPicture::OutTHEURGE(_IO *pio, _RESULT *pres, _VARIABLE *pv)
{
  _BOOL retcode = _TRUE;

  if(pio->theurge->info != 0xFFFFFFFF)
  {
    switch(pio->theurge->urgetype)
    {
      case _URGE_BAR:
        retcode = Out_THEURGE_Bar(pio, pres, pv);
        break;

      case _URGE_INDICATOR:
        retcode = Out_THEURGE_Indicator(pio, pres, pv);
        break;

      case _URGE_CLOCK:
        retcode = Out_THEURGE_Clock(pio, pres, pv);
        break;

      case _URGE_SCALE:
        retcode = Out_THEURGE_Scale(pio, pres, pv);
        break;

      case _URGE_DELAYANDRETURN:
        retcode = Out_THEURGE_ScreenClean(pio, pres, pv);
        break;

      case _URGE_SLIDER:
        retcode = out_SLIDER(pio, pres, pv);
        break;

      default :
        retcode = OutNumeric(pio, pres, pv);
        break;
    }
  }

  return(retcode);
}

// info 0 = BottomTop
// info 1 = TopBottom
// info 2 = Center
// info 3 = LeftRight
// info 4 = RightLeft
// info 5 = Mid
// memcolor[0] = color low limit
// memcolor[1] = color high limit
// memcolor[2] = color value
// memdyncolor[0] = color low limit
// memdyncolor[1] = color high limit
// memdyncolor[2] = color value
// memvarlabel[0] = offset for begin drawing
// memvarlabel[1] = LL
// memvarlabel[2] = HH
// memvarlabel[3] = L
// memvarlabel[4] = H
// memvarlabel[5] = offset for value (value = value + offset)
_BOOL _SubPicture::Out_THEURGE_Bar(_IO *pio, _RESULT *pres, _VARIABLE *pv)
{
  _THEURGE  *p;
  _BOOL     draw = _TRUE;
  _BOOL     offsetdraw = _FALSE;
  _BOOL     colmin = _FALSE;
  _BOOL     colmax = _FALSE;
  _ROOM     space;
  _RESULT   result[7];
  _RESULT   res;
  _VARIABLE var;
  _UWORD    i, j, mul;
  _NEWSCREEN act;
 #if COMPILER_VERSION >= 39
  _BOOL     offsetvalue = _FALSE;
  _CHAR     txt[50], *unit;
  _ANYTHING any;
 #endif


  p = pio->theurge;

  result[0] = *pres;
  for(i=0,j=1; (i<p->memvarlabel.no)&&(draw==_TRUE); i++,j++)
  {
    draw = _FALSE;

    if(VarList.GetSystemData(&result[j], &var, &p->memvarlabel.ptr[i], _FALSE, _TRUE, _TRUE) == _TRUE)
      draw = _TRUE;
  }

  if(draw == _TRUE)
  {
    mul = 1;
    if(result[3].ftype == RES_REAL)
    {
      if(get_pREAL((_REAL*)&result[4].value) < 1000.0)
        mul = 1000;
    }

    for(i=0; i<p->memvarlabel.no + 1; i++)
    {
      if(result[i].ftype == RES_REAL)
      {
        res = result[i];
        result[i].value = FTOL(get_pREAL((_REAL*)&res.value) * mul);
        result[i].ftype = RES_NUMERIC;
      }
    }

  }

  // varlables for min - max
  if(draw == _TRUE)
  {
    if(((result[2].ftype != RES_CONSTANT) && (result[2].ftype != RES_NUMERIC)) ||
       ((result[3].ftype != RES_CONSTANT) && (result[3].ftype != RES_NUMERIC)))
    {
      // converted bar
      result[2] = result[0];
      result[2].value = result[2].rmin;
      result[3] = result[0];
      result[3].value = result[3].rmax;
    }
  }

  // check limits
  if(draw == _TRUE)
  {
    if(result[2].value > result[3].value)
      draw = _FALSE;

  }

  // check for special bars
  if(draw == _TRUE)
  {
    if((result[1].ftype == RES_CONSTANT) || (result[1].ftype == RES_NUMERIC))
      offsetdraw = _TRUE;

    if((result[4].ftype == RES_CONSTANT) || (result[4].ftype == RES_NUMERIC))
      colmin = _TRUE;

    if((result[5].ftype == RES_CONSTANT) || (result[5].ftype == RES_NUMERIC))
      colmax = _TRUE;

   #if COMPILER_VERSION >= 39
    if((result[6].ftype == RES_CONSTANT) || (result[6].ftype == RES_NUMERIC))
      offsetvalue = _TRUE;
   #endif

  }

  if(draw == _TRUE)
  {
    push_screen(&act);

    if(pio->image.ptr != NULL)
      PutImage(&pio->image, DEFKOORD, DEFKOORD);
    DrawButton(&pio->space, pio->attrib, pio->frametype, pio->colback, pio->colframe);
//    FrameInlet(&space, &pio->space, pio->attrib);
    GetTextRoom(&space, &pio->space, pio->attrib, pio->frametype, _FALSE);

   #if COMPILER_VERSION >= 39
    if(offsetvalue == _TRUE)
      result[0].value += result[6].value;
   #endif

    switch(p->info)
    {
      case 0:
        BarBottomTop(&space, pio, result, offsetdraw, colmin, colmax);
        break;
      case 1:
        BarTopBottom(&space, pio, result, offsetdraw, colmin, colmax);
        break;
      case 2:
        BarLeftRight(&space, pio, result, offsetdraw, colmin, colmax);
        break;
      case 3:
        BarRightLeft(&space, pio, result, offsetdraw, colmin, colmax);
        break;
      case 4:
        BarBottomTop(&space, pio, result, _FALSE, colmin, colmax);
        break;
      case 5:
        BarLeftRight(&space, pio, result, _FALSE, colmin, colmax);
        break;

    }

   #if COMPILER_VERSION >= 39
    if(p->memdyncolor.no >= 3)
    {
      if(GetPenColor_Makro(p->memdyncolor.ptr[2].color) != INVISIBLE)
      {
        recalculate(&result[0], pv, _TRUE);
        format_value(txt, result[0].value, &pv->info, &pio->picformat);

        init_ANYTHING_Makro(&any);
        any.no   = pio->variable.info[0].value;
        any.list = VARUNIT;
        unit = get_text(&any, NULL, NULL, _TRUE);
        if(unit != NULL)
          StrCat(txt, sizeof(_CHAR), unit, sizeof(_CHAR));

        OutMultiTextAlign((void*)txt, 0, &pio->space, pio->font, pio->attrib, _DEFFRAME, p->memdyncolor.ptr[2].color, INVISIBLE, INVISIBLE, sizeof(_CHAR));
      }
    }
   #endif

    pop_screen(&act);
    copy_screen(&act, &pio->space);
  }

  return(_TRUE);

}

_KOORD BarPosition(_DWORD value, _DWORD min, _DWORD max, _KOORD range)
{
  _UWORD retcode = 0;


  range--;

  if(value < min)
    value = min;
  else if(value > max)
    value = max;

  if(min != max)
  {
	  if((max > 2000000L)||(min < -2000000L))
	  {
		  value /= 1000;
		  min   /= 1000;
		  max   /= 1000;
	  }

	  retcode = ((value - min) * range) / (absolut(max - min));
  }

  return(retcode);
}

void BarGetInfo(_IO *pio, _UWORD *no, _KOORD *pos, _COLOR *color, _RESULT *pres, _UWORD range, _BOOL offset, _BOOL colmin, _BOOL colmax)
{
  _THEURGE *p;

  p = pio->theurge;
  (*no) = 0;
  color[0] = 
  color[1] = 
  color[2] = pio->coltxt;

  if((offset == _FALSE))// && ((p->info == 4) || (p->info == 5)))
  {
    if((p->info != 4) && (p->info != 5))
    {
      pres[1].value = pres[2].value;
      offset = _TRUE;
    }
    else
    {
      pres[1].value = 0;

      if(pres[4].value > 0)
        pres[4].value = 0;
    }
  }
//  else
//    pres[1].value = pres[2].value;

//  Änderung, damit der Strich nicht angezeigt wird bei value = 0
//  if((pres[0].value <= pres[4].value) && (pres[0].value >= pres[1].value))
  if((pres[0].value < pres[4].value) && (pres[0].value > pres[1].value))
  {
    if(colmin == _TRUE)
      color[0] = p->memdyncolor.ptr[0].color;
    pos[0] = BarPosition(pres[1].value, pres[2].value, pres[3].value, range);
    pos[1] = BarPosition(pres[0].value, pres[2].value, pres[3].value, range);
    (*no) = 2;
  }
  else if((pres[1].value > pres[5].value) && (pres[0].value > pres[1].value))
  {
    if(colmax == _TRUE)
      color[0] = p->memdyncolor.ptr[1].color;
    pos[0] = BarPosition(pres[1].value, pres[2].value, pres[3].value, range);
    pos[1] = BarPosition(pres[0].value, pres[2].value, pres[3].value, range);
    (*no) = 2;
  }
//  else if((pres[0].value >= pres[1].value) || (offset == _FALSE))
  else if((pres[0].value > pres[1].value) || ((offset == _FALSE) && ((p->info == 4) || (p->info == 5))))
  {
    pos[*no] = BarPosition(pres[1].value, pres[2].value, pres[3].value, range);
    (*no)++;
    if((colmin == _TRUE) && (((pres[0].value <= pres[4].value) && (offset == _TRUE)) || (pres[1].value < pres[4].value)))
      color[(*no-1)] = p->memdyncolor.ptr[0].color;

    if((p->info == 4) || (p->info == 5))
    {
//      if((colmin == _TRUE) && (pres[0].value <= pres[4].value))
      if((colmin == _TRUE) && (pres[0].value < pres[4].value))
      {
        pos[*no] = BarPosition(pres[4].value, pres[2].value, pres[3].value, range);
        (*no)++;
        color[(*no-1)] = p->memdyncolor.ptr[0].color;
      }
    }

    if((pres[0].value > pres[4].value) && (pres[1].value < pres[4].value) && (colmin == _TRUE))
    {
      pos[*no] = BarPosition(pres[4].value, pres[2].value, pres[3].value, range);
      (*no)++;
      color[(*no-1)] = pio->coltxt;
    }

//    if((pres[0].value >= pres[5].value) && (colmax == _TRUE))
    if((pres[0].value > pres[5].value) && (colmax == _TRUE))
    {
      pos[*no] = BarPosition(pres[5].value, pres[2].value, pres[3].value, range);
      (*no)++;
      if(colmax == _TRUE)
        color[(*no-1)] = p->memdyncolor.ptr[1].color;
    }

    pos[*no] = BarPosition(pres[0].value, pres[2].value, pres[3].value, range);
    (*no)++;
  }

}

void BarRightLeft(_ROOM *ps, _IO *pio, _RESULT *pres, _BOOL offset, _BOOL colmin, _BOOL colmax)
{
  _KOORD range, pos[4];
  _COLOR color[3];
  _UWORD i, no;

 #ifdef UC_THEME
  if(pio->frametype >= _THEMEFRAME)
  {
    _REGION actreg, ioreg, newreg;
	_ROOM   space;

    range = pio->space.xy2.x - pio->space.xy1.x + 1;
    BarGetInfo(pio, &no, pos, color, pres, range, offset, colmin, colmax);
	space       = pio->space;
	space.xy2.x = pio->space.xy2.x - pos[0];

    for(i=0; i<no-1; i++)
	{
	  space.xy1.x = pio->space.xy2.x - pos[i+1];
      ioreg  = NULL;
      newreg = NULL;
      actreg = ActivateAndRegion(&ioreg, &newreg, &space);
      DrawButton(&pio->space, pio->attrib, pio->frametype, color[i], pio->colframe);
      DeactivateAndRegion(actreg, &ioreg, &newreg);
      space.xy2.x = space.xy1.x - 1;
    }
  }
  else
 #endif
  {
    range = ps->xy2.x - ps->xy1.x + 1;
    BarGetInfo(pio, &no, pos, color, pres, range, offset, colmin, colmax);
    for(i=0; i<no-1; i++)
      Bar(ps->xy2.x-pos[i], ps->xy1.y, ps->xy2.x-pos[i+1], ps->xy2.y, DelPattern(pio->attrib), color[i]);
  }
}

void BarLeftRight(_ROOM *ps, _IO *pio, _RESULT *pres, _BOOL offset, _BOOL colmin, _BOOL colmax)
{
  _KOORD range, pos[4];
  _COLOR color[3];
  _UWORD i, no;


 #ifdef UC_THEME
  if(pio->frametype >= _THEMEFRAME)
  {
    _REGION actreg, ioreg, newreg;
	_ROOM   space;

    range = pio->space.xy2.x - pio->space.xy1.x + 1;
    BarGetInfo(pio, &no, pos, color, pres, range, offset, colmin, colmax);
	space       = pio->space;
	space.xy1.x = pio->space.xy1.x + pos[0];

    for(i=0; i<no-1; i++)
	{
	  space.xy2.x = pio->space.xy1.x + pos[i+1];
      ioreg  = NULL;
      newreg = NULL;
      actreg = ActivateAndRegion(&ioreg, &newreg, &space);
      DrawButton(&pio->space, pio->attrib, pio->frametype, color[i], pio->colframe);
      DeactivateAndRegion(actreg, &ioreg, &newreg);
      space.xy1.x = space.xy2.x + 1;
    }
  }
  else
 #endif
  {
    range = ps->xy2.x - ps->xy1.x + 1;
    BarGetInfo(pio, &no, pos, color, pres, range, offset, colmin, colmax);
    for(i=0; i<no-1; i++)
      Bar(ps->xy1.x+pos[i], ps->xy1.y, ps->xy1.x+pos[i+1], ps->xy2.y, DelPattern(pio->attrib), color[i]);
  }
}

void BarTopBottom(_ROOM *ps, _IO *pio, _RESULT *pres, _BOOL offset, _BOOL colmin, _BOOL colmax)
{
  _KOORD range, pos[4];
  _COLOR color[3];
  _UWORD i, no;

 #ifdef UC_THEME
  if(pio->frametype >= _THEMEFRAME)
  {
    _REGION actreg, ioreg, newreg;
	_ROOM   space;

    range = pio->space.xy2.y - pio->space.xy1.y + 1;
    BarGetInfo(pio, &no, pos, color, pres, range, offset, colmin, colmax);
	space = pio->space;
	space.xy1.y = pio->space.xy1.y + pos[0];

    for(i=0; i<no-1; i++)
	{
	  space.xy2.y = pio->space.xy1.y + pos[i+1];
      ioreg  = NULL;
      newreg = NULL;
      actreg = ActivateAndRegion(&ioreg, &newreg, &space);
      DrawButton(&pio->space, pio->attrib, pio->frametype, color[i], pio->colframe);
      DeactivateAndRegion(actreg, &ioreg, &newreg);
      space.xy1.y = space.xy2.y + 1;
    }
  }
  else
 #endif
  {
    range = ps->xy2.y - ps->xy1.y + 1;
    BarGetInfo(pio, &no, pos, color, pres, range, offset, colmin, colmax);
    for(i=0; i<no-1; i++)
      Bar(ps->xy1.x, ps->xy1.y+pos[i], ps->xy2.x, ps->xy1.y+pos[i+1], DelPattern(pio->attrib), color[i]);
  }
}

void BarBottomTop(_ROOM *ps, _IO *pio, _RESULT *pres, _BOOL offset, _BOOL colmin, _BOOL colmax)
{
  _KOORD range, pos[4];
  _COLOR color[3];
  _UWORD i, no;

 #ifdef UC_THEME
  if(pio->frametype >= _THEMEFRAME)
  {
    _REGION actreg, ioreg, newreg;
    _ROOM   space;

    range = pio->space.xy2.y - pio->space.xy1.y + 1;
    BarGetInfo(pio, &no, pos, color, pres, range, offset, colmin, colmax);
    space       = pio->space;
    space.xy2.y = pio->space.xy2.y - pos[0];

    for(i=0; i<no-1; i++)
    {
      space.xy1.y = pio->space.xy2.y - pos[i+1];
      ioreg  = NULL;
      newreg = NULL;
      actreg = ActivateAndRegion(&ioreg, &newreg, &space);
      DrawButton(&pio->space, pio->attrib, pio->frametype, color[i], pio->colframe);
      DeactivateAndRegion(actreg, &ioreg, &newreg);
      space.xy2.y = space.xy1.y - 1;
    }
  }
  else
 #endif
  {
    range = ps->xy2.y - ps->xy1.y + 1;
    BarGetInfo(pio, &no, pos, color, pres, range, offset, colmin, colmax);
    for(i=0; i<no-1; i++)
      Bar(ps->xy1.x, ps->xy2.y-pos[i], ps->xy2.x, ps->xy2.y-pos[i+1], DelPattern(pio->attrib), color[i]);
  }
}

// info = Versionnumber
// memcolor[0] = color hour
// memcolor[0] = color minute
// memcolor[0] = color second
_BOOL _SubPicture::Out_THEURGE_Clock(_IO *pio, _RESULT *pres, _VARIABLE *pv)
{
  _THEURGE *ptu;
  _NEWSCREEN act;
  _ROOM room;
  _DOT dot;
  _ZOOM zoom;
  _DOT offset;


  ptu = pio->theurge;

  if(ptu != NULL)
  {
    if(pres->ftype == RES_TIME)
    {
      push_screen(&act);

      if(pio->image.ptr != NULL)
        PutImage(&pio->image, DEFKOORD, DEFKOORD);

      DrawButton(&pio->space, pio->attrib, pio->frametype, pio->colback, pio->colframe);

      dot.x = (pio->space.xy1.x + pio->space.xy2.x) / 2;
      dot.y = (pio->space.xy1.y + pio->space.xy2.y) / 2;
      offset.x = 0;
      offset.y = 0;

      if(pv->info.format != SIZE_MMSS)
      {
        room = pio->space;
        room.xy1.x -= dot.x;
        room.xy1.y -= dot.y;
        room.xy2.x -= dot.x;
        room.xy2.y -= dot.y;
        zoom.x = 700;
        zoom.y = 700;
        Zoom_ROOM(&room, &offset, &zoom);
        room.xy1.x += dot.x;
        room.xy1.y += dot.y;
        room.xy2.x += dot.x;
        room.xy2.y += dot.y;
        indicator(&room, 90 - ((pres->datim.ftime.hour % 12) * 60 + pres->datim.ftime.minute) / 2, MakeColor_Makro(ptu->memdyncolor.ptr[0].color, ptu->memdyncolor.ptr[0].color));
      }

      room = pio->space;
      room.xy1.x -= dot.x;
      room.xy1.y -= dot.y;
      room.xy2.x -= dot.x;
      room.xy2.y -= dot.y;
      zoom.x = 850;
      zoom.y = 850;
      Zoom_ROOM(&room, &offset, &zoom);
      room.xy1.x += dot.x;
      room.xy1.y += dot.y;
      room.xy2.x += dot.x;
      room.xy2.y += dot.y;
      indicator(&room, 90 - 6 * pres->datim.ftime.minute, MakeColor_Makro(ptu->memdyncolor.ptr[1].color, ptu->memdyncolor.ptr[1].color));

      if(pv->info.format == SIZE_HHMMSS)
        indicator(&pio->space, 90 - 6 * pres->datim.ftime.second, MakeColor_Makro(ptu->memdyncolor.ptr[2].color, ptu->memdyncolor.ptr[2].color));

      pop_screen(&act);
      copy_screen(&act, &pio->space);
    }
  }


  return(_TRUE);
}


_BOOL _SubPicture::GetValue_THEURGE_ScreenClean(_RESULT *res, _IO *pio)
{
  _UDWORD  duration, time;
  _UDWORD  xx, yy;
  _THEURGE *p;
      
  res->value = 0;
  res->ftype = RES_NUMERIC;

  p = pio->theurge;

  if(p != NULL)
  {
    if(pio->workstate == STATE_RUN) 
    {
      duration = p->memvarlabel.ptr[0].info[1].value;
      time     = get_abstime() - p->memvarlabel.ptr[0].info[2].value;
    
      xx = pio->space.xy2.x - pio->space.xy1.x + 1;
      yy = pio->space.xy2.y - pio->space.xy1.y + 1;
      if(yy > xx)
        xx = yy;
      
      if(time < duration)
      {
        res->value = xx - (time * xx) / duration;
      }
      else
      {
        res->value = 0;
        
        // nur wenn window offen oder screen in picmemo vorhanden
        if((Root.GetP_Window() != NULL) || (PicMemo.GetNo() > 1))
          InsertCmd(CMD_RETURN, 0, 0);
      }
    }
  }
  
  res->rmin = res->value;
  res->rmax = res->value;
  
  return(_TRUE);
}

_BOOL _SubPicture::Out_THEURGE_ScreenClean(_IO *pio, _RESULT *pres, _VARIABLE *pv)
{
  _BOOL retcode = _TRUE;
  _THEURGE  *p;
  _VARIABLE varia;
  _NEWSCREEN act;
  _REGION actreg, ioreg, newreg;
  
  p = pio->theurge;

  if(pio->workstate == STATE_START)
  {
    _RESULT result;
    if(VarList.GetSystemData(&result, &varia, &p->memvarlabel.ptr[0], _FALSE, _TRUE, _TRUE) == _TRUE)
    {
        if(result.value < 1)
          result.value = 1;
    
        p->memvarlabel.ptr[0].info[1].state = CONST_VAL;
        p->memvarlabel.ptr[0].info[1].value = result.value * 1000;
        p->memvarlabel.ptr[0].info[2].state = CONST_VAL;
        p->memvarlabel.ptr[0].info[2].value = get_abstime();
        pio->workstate = STATE_RUN;
    }
  }
  
  
  _ROOM room;
  
  if((pio->space.xy2.y - pio->space.xy1.y) > (pio->space.xy2.x - pio->space.xy1.x))
  {
    room.xy1.x = pio->space.xy1.x;
    room.xy1.y = pio->space.xy2.y  - pres->value;
    room.xy2.x = pio->space.xy2.x;
    room.xy2.y = pio->space.xy2.y;
  }
  else
  {
    room.xy1.x = pio->space.xy1.x;
    room.xy1.y = pio->space.xy1.y;
    room.xy2.x = pio->space.xy1.x + pres->value;
    room.xy2.y = pio->space.xy2.y;
  }
  
  push_screen(&act);
    
  if(is_invisible(pio->colback, pio->attrib) == _TRUE)
  {
    if(pio->image.ptr != NULL)
      PutImage(&pio->image, DEFKOORD, DEFKOORD);
  }
  else
  {
    DrawButton(&pio->space, pio->attrib, pio->frametype, pio->colback, pio->colframe);
  }
  
  if(pres->value > 0)
  {
    actreg = ActivateAndRegion(&ioreg, &newreg, &room);
    
    if(pio->frametype >= _THEMEFRAME)
      draw_THEME_partial(&pio->space, &room, pio->frametype, pio->attrib, pio->coltxt);
    else
      DrawButton(&pio->space, pio->attrib, pio->frametype, pio->coltxt, pio->colframe);
      
    DeactivateAndRegion(actreg, &ioreg, &newreg);
  }
  
  pop_screen(&act);
  copy_screen(&act, &pio->space);
  
  return retcode;
}


// info Versionnumber
// memvarlabel[0] = min value
// memvarlabel[1] = max value
// memvarlabel[2] = graduates
// memvarlabel[3] = subgraduates
// memcolor[0] = low limit color
// memcolor[1] = high limit color
// memdyncolor[0] = low limit color
// memdyncolor[1] = high limit color
// membool[0] = _TRUE == horizontal
_BOOL _SubPicture::Out_THEURGE_Scale(_IO *pio, _RESULT *pres, _VARIABLE *pv)
{
  _THEURGE  *p;
  _ROOM     space;
  _RESULT   res[5], r;
  _VARIABLE var;
  _UWORD    i, mul;
  _NEWSCREEN act;

  p = pio->theurge;

  if(p->memvarlabel.no >= 4)
  {
    res[0] = *pres;
    
    // get minval
    if(VarList.GetSystemData(&res[1], &var, &p->memvarlabel.ptr[0], _FALSE, _TRUE, _TRUE) == _FALSE)
      init_RESULT(&res[1]);
    if(res[1].ftype == RES_DEFAULT)
    {
      res[1].ftype = res[0].ftype;
      res[1].value = res[0].rmin;
    }
          
    // get maxval  
    if(VarList.GetSystemData(&res[2], &var, &p->memvarlabel.ptr[1], _FALSE, _TRUE, _TRUE) == _FALSE)
      init_RESULT(&res[2]);
    if(res[2].ftype == RES_DEFAULT)
    {
      res[2].ftype = res[0].ftype;
      res[2].value = res[0].rmax;
    }
          
    // get graduates  
    if(VarList.GetSystemData(&res[3], &var, &p->memvarlabel.ptr[2], _FALSE, _TRUE, _TRUE) == _FALSE)
      return(_FALSE);
    
    // get graduates  
    if(VarList.GetSystemData(&res[4], &var, &p->memvarlabel.ptr[3], _FALSE, _TRUE, _TRUE) == _FALSE)
      return(_FALSE);

    mul = 1;
    if(res[2].ftype == RES_REAL)
    {
      if(get_pREAL((_REAL*)&res[2].value) < 1000.0)
        mul = 1000;
    }

    for(i=0; i<5; i++)
    {
      if(res[i].ftype == RES_REAL)
      {
        r = res[i];
        res[i].value = FTOL(get_pREAL((_REAL*)&r.value) * mul);
        res[i].ftype = RES_NUMERIC;
      }
    }
    
    // check limits
    if(res[1].value > res[2].value)
    {
      _RESULT tmp = res[1];
      res[1] = res[2];
      res[2] = tmp;
    }

    push_screen(&act);

    if(pio->image.ptr != NULL)
      PutImage(&pio->image, DEFKOORD, DEFKOORD);

    DrawButton(&pio->space, pio->attrib, pio->frametype, pio->colback, pio->colframe);
    FrameInlet(&space, &pio->space, pio->attrib);
    ScaleDraw(pio, pv, res, &space, p->memdyncolor.ptr[0].color, p->memdyncolor.ptr[1].color, p->membool.ptr[0]);

    pop_screen(&act);
    copy_screen(&act, &pio->space);
  }

  return(_TRUE);
}

_UWORD StrMoveToNoSpace(_CHAR* str, _UWORD size)
{
  _UWORD retcode = 0xFFFF;
  _UWORD ct = 0;
  _UWORD run = 1;
  _UNI   *pu;
  _ASCII *pa;

  if(size == 2)
  {
    pu = (_UNI*)str;

    do
    {
      if(pu[ct] == 0)
      {
        run = 0;
      }
      else if(pu[ct] != 32)
      {
        run = 0;
        retcode = ct;
      }
      ct ++;
    }
    while(run);
  }
  else
  {
    pa = (_ASCII*)str;

    do
    {
      if(pa[ct] == 0)
      {
        run = 0;
      }
      else if(pa[ct] != 32)
      {
        run = 0;
        retcode = ct;
      }
      ct ++;
    }
    while(run);
  }

  return(retcode);
}

void ScaleDraw(_IO *pio, _VARIABLE *pv, _RESULT *pres, _ROOM *space, _COLOR ColGrad, _COLOR ColSubGrad, _BOOL horizontal)
{
  _KOORD k0, k1, k2, k3, k4, k5, k6, part0, part1, inc, tmp, offs, xx;
  _UWORD i, j, no0, no1, fh, fw, idx, last;
  _ATTRIB txtattrib;
  _CHAR  txt[20];
  _IVALUE val, value, minmax;
  _RESULT res;


  no0 = pres[3].value;
  no1 = pres[4].value;
  if(no0 < 2)
    no0 = 2;

  Font.Set(pio->font, &pio->attrib);
//fontattrib SetFontAttrib(pio->attrib);
  Font.Size(pio->font, &fw, &fh);

  if(horizontal == _TRUE)
  {
    part0 = (space->xy2.y - space->xy1.y + 1) / 6;
    part1 = (space->xy2.x - space->xy1.x + 1) / (no0 - 1);

    switch(pio->attrib & T_VERTBOUND)
    {
      case T_DOWNBOUND:
        txtattrib = T_COPY|T_SOLID|T_DOWNBOUND|T_CENTERBOUND|T_PROP;
        k0 = space->xy2.y;
        k1 = k0 - part0;
        k2 = k1 - part0;
        k3 = space->xy1.y;
        k4 = space->xy1.x;
        k6 = space->xy2.x;
        offs = -3;
        inc = -1;
        break;

      case T_UPBOUND:
      default:
        txtattrib = T_COPY|T_SOLID|T_UPBOUND|T_CENTERBOUND|T_PROP;
        k0 = space->xy1.y;
        k1 = k0 + part0;
        k2 = k1 + part0;
        k3 = space->xy2.y;
        k4 = space->xy1.x;
        k6 = space->xy2.x;
        offs = 3;
        inc = 1;
        break;

    }

    switch(pio->attrib & T_HORIZBOUND)
    {
      case T_RIGHTBOUND:
        val = (pres[2].value - pres[1].value) / (no0 - 1);
        value = pres[2].value;
        minmax = pres[1].value;
        init_RESULT(&res);
        res.value = pres[2].value;
        res.ftype = RES_NUMERIC;
        val = -val;
        break;

      case T_LEFTBOUND:
      default:
        val = (pres[2].value - pres[1].value) / (no0 - 1);
        value = pres[1].value;
        minmax = pres[2].value;
        init_RESULT(&res);
        res.value = pres[1].value;
        res.ftype = RES_NUMERIC;
        break;

    }

    Line(space->xy1.x, k0, space->xy2.x, k0, txtattrib, ColGrad);
    k0 += inc;
    last = no0 - 1;

    tmp = k4;
    for(i=0; i<no0; i++)
    {
      if(i == last)
        tmp = k6;
      //if((i == 0) || (i == last))
      //  Line(tmp, k0, tmp, k3, txtattrib, ColGrad);
      //else
      Line(tmp, k0, tmp, k2, txtattrib, ColGrad);

      if(i != last)
      {
        for(j=0; j<no1; j++)
        {
          k5 = tmp + (((j + 1) * (part1)) / (no1 + 1));
          Line(k5, k0, k5, k1, txtattrib, ColSubGrad);
        }
      }

      if(i == 0)
      {
        txtattrib &= ~T_HORIZBOUND;
        txtattrib |= T_LEFTBOUND;
        xx = 2;
      }
      else if(i == last)
      {
        txtattrib &= ~T_HORIZBOUND;
        txtattrib |= T_RIGHTBOUND;
        res.value = minmax;
        xx = 0;
      }
      else
      {
        txtattrib &= ~T_HORIZBOUND;
        txtattrib |= T_CENTERBOUND;
        xx = 0;
      }

      recalculate(&res, pv, _TRUE);
      format_value(txt, res.value, &pv->info, &pio->picformat);
      idx = StrMoveToNoSpace(txt, sizeof(_CHAR));
      if(idx != 0xffff)
        OutTextLse(tmp+xx, k2+offs, &txt[idx], txtattrib, MakeColor_Makro(INVISIBLE, ColGrad), sizeof(_CHAR));

      value += val;
      res.value = value;
      res.recalculate_done = 0;

      tmp = space->xy1.x + (((i + 1) * (space->xy2.x - space->xy1.x + 1)) / (no0 - 1));
    }

  }

  else
  {
    part0 = (space->xy2.x - space->xy1.x + 1) / 6;
    part1 = (space->xy2.y - space->xy1.y + 1) / (no0 - 1);

    switch(pio->attrib & T_HORIZBOUND)
    {
      case T_RIGHTBOUND:
        txtattrib = T_COPY|T_SOLID|T_UPBOUND|T_RIGHTBOUND|T_PROP;
        k0 = space->xy2.x;
        k1 = k0 - part0;
        k2 = k1 - part0;
        k3 = space->xy1.x;
        k4 = space->xy2.y;
        k6 = space->xy1.y;
        offs = -3;
        inc = -1;
        break;

      case T_LEFTBOUND:
      default:
        txtattrib = T_COPY|T_SOLID|T_UPBOUND|T_LEFTBOUND|T_PROP;
        k0 = space->xy1.x;
        k1 = k0 + part0;
        k2 = k1 + part0;
        k3 = space->xy2.x;
        k4 = space->xy2.y;
        k6 = space->xy1.y;
        offs = 3;
        inc = 1;
        break;

    }

    switch(pio->attrib & T_VERTBOUND)
    {
      case T_UPBOUND:
        val = (pres[2].value - pres[1].value) / (no0 - 1);
        value = pres[2].value;
        minmax = pres[1].value;
        init_RESULT(&res);
        res.value = pres[2].value;
        res.ftype = RES_NUMERIC;
        val = -val;
        break;

      case T_DOWNBOUND:
      default:
        val = (pres[2].value - pres[1].value) / (no0 - 1);
        value = pres[1].value;
        minmax = pres[2].value;
        init_RESULT(&res);
        res.value = pres[1].value;
        res.ftype = RES_NUMERIC;
        break;

    }

    Line(k0, space->xy1.y, k0, space->xy2.y, txtattrib, ColGrad);
    k0 += inc;
    last = no0 - 1;

    tmp = k4;
    for(i=0; i<no0; i++)
    {
      if(i == last)
        tmp = k6;
        
      //if((i == 0) || (i == last))
      //  Line(k0, tmp, k3, tmp, txtattrib, ColGrad);
      //else
      Line(k0, tmp, k2, tmp, txtattrib, ColGrad);

      if(i != last)
      {
        for(j=0; j<no1; j++)
        {
          k5 = tmp - (((j + 1) * (part1)) / (no1 + 1));
          Line(k0, k5, k1, k5, txtattrib, ColSubGrad);
        }
      }

      if(i == last)
        res.value = minmax;

      recalculate(&res, pv, _TRUE);
      format_value(txt, res.value, &pv->info, &pio->picformat);
      idx = StrMoveToNoSpace(txt, sizeof(_CHAR));
      if(idx != 0xffff)
      {
        if(i == last)
          fh = 0;
        OutTextLse(k1+offs, tmp-fh, &txt[idx], txtattrib, MakeColor_Makro(INVISIBLE, ColGrad), sizeof(_CHAR));
      }

      value += val;
      res.value = value;
      res.recalculate_done = 0;

      tmp = space->xy2.y - (((i + 1) * (space->xy2.y - space->xy1.y + 1)) / (no0 - 1));

    }
  }
}

_BOOL check_THEURGE(_THEURGE *p)
{
  _BOOL retcode = _TRUE;

  switch(p->urgetype)
  {
    case _URGE_BAR:
      retcode = check_THEURGE_Bar(p);
      break;

    case _URGE_OSZI:
      retcode = check_THEURGE_Oszi(p);
      break;

    case _URGE_INDICATOR:
      retcode = check_THEURGE_Indicator(p);
      break;

    case _URGE_CLOCK:
      retcode = check_THEURGE_Clock(p);
      break;

    case _URGE_SCALE:
      retcode = check_THEURGE_Scale(p);
      break;

    case _URGE_DELAYANDRETURN:
      retcode = _TRUE; //check_THEURGE_ScreenClean(p); // todo
      break;
      
    case _URGE_SLIDER:
      retcode = _TRUE; //check_THEURGE_Slider(p); // todo
      break;

    case _URGE_UNDEFINED:
      retcode = _FALSE;
      break;

    case _URGE_USERDEFINED:
      break;
  }

  return(retcode);
}

_BOOL check_THEURGE_Bar(_THEURGE *p)
{
  _BOOL retcode = _FALSE;


 #if COMPILER_VERSION >= 39

  if(p->memvarlabel.no == 6)
    if(p->memcolor.no == 3)
      if(p->memdyncolor.no == 3)
        retcode = _TRUE;

 #else

  if(p->memvarlabel.no == 5)
    if(p->memcolor.no == 2)
      if(p->memdyncolor.no == 2)
        retcode = _TRUE;

 #endif

  return(retcode);

}

_BOOL check_THEURGE_Oszi(_THEURGE *p)
{
  _BOOL retcode = _FALSE;


  if(p->info == 1)
  {
    if(p->memvarlabel.no == 24)
      if(p->memcolor.no == 8)
        if(p->memdyncolor.no == 8)
          retcode = _TRUE;
  }
  else if(p->info == 2)
  {
    if(p->memvarlabel.no == 26)
      if(p->memcolor.no == 9)
        if(p->memdyncolor.no == 9)
          retcode = _TRUE;
  }

  return(retcode);

}

_BOOL check_THEURGE_Indicator(_THEURGE *p)
{
  _BOOL retcode = _FALSE;


  if(p->info == 1)
  {
    if(p->memvarlabel.no >= 4)
      retcode = _TRUE;
  }

  return(retcode);
}

_BOOL check_THEURGE_Clock(_THEURGE *p)
{
  _BOOL retcode = _FALSE;


  if(p->info == 1)
  {
    if(p->memcolor.no == 3)
      retcode = _TRUE;
  }

  return(retcode);

}

_BOOL check_THEURGE_Scale(_THEURGE *p)
{
  _BOOL retcode = _FALSE;


  if(p->info == 1)
  {
    if(p->memvarlabel.no == 4)
      if(p->memcolor.no == 2)
        if(p->memdyncolor.no == 2)
          if(p->membool.no == 1)
            retcode = _TRUE;
  }

  return(retcode);
}






/*
// info = Versionnumber
// memvarlabel[0] = ValueMin
// memvarlabel[1] = ValueMax
// membool[0] = _TRUE = horizontal
_BOOL _SubPicture::Out_THEURGE_Scrollbar(_IO *pio, _RESULT *pres, _VARIABLE *pv)
{
  _RESULT vmin, vmax;
  _VARIABLE variable;
  _LEVEL *pl;
  _POSITION *pp;
  _THEURGE *ptu;
  _BOOL horizontal = _FALSE;


return(_TRUE);
  ptu = pio->theurge;
  pl = (_LEVEL*)pio->theurge->ptr;
  pp = (_POSITION*)pio->theurge->ptr + sizeof(_LEVEL);

  if(pio->cstate.init == _TRUE)
  {
    if(Memory2((void**)&pio->theurge->ptr, sizeof(_LEVEL) + sizeof(_POSITION)) != 0)
    {
      pl = (_LEVEL*)pio->theurge->ptr;
      pp = (_POSITION*)pio->theurge->ptr + sizeof(_LEVEL);
      init_LEVEL(pl);
      init_POSITION(pp);

      pl->active = _TRUE;
      pl->room = pio->space;
      pl->color = pio->colback;
      pl->frame_color = pio->colframe;


      if((pio->space.xy2.x - pio->space.xy1.x) > (pio->space.xy2.y - pio->space.xy1.y))
      {
        horizontal = _TRUE;
        pp->width = pio->space.xy2.x - pio->space.xy1.x + 1 - LEVELBUTTON_HEIGHT;
      }
      else
      {
        horizontal = _FALSE;
        pp->width = pio->space.xy2.y - pio->space.xy1.y + 1 - LEVELBUTTON_HEIGHT;
      }

    }
  }

  if((ptu != NULL) && (pl != NULL) && (pp != NULL))
  {
    if(VarList.GetSystemData(&vmin, &variable, &ptu->memvarlabel.ptr[0], _FALSE, _TRUE) == _TRUE)
    {
      if(VarList.GetSystemData(&vmax, &variable, &ptu->memvarlabel.ptr[1], _FALSE, _TRUE) == _TRUE)
      {
        pp->no = vmax.value - vmin.value + 1;
        pp->pos = pres->value;

        level_draw(pl, horizontal, pio->cstate.init);
        level_run(pl, pp->no, pp->pos, horizontal);

      }
    }
  }

  return(_TRUE);
}
*/
