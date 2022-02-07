#include "T_IPC.H"
#include "T_NUC.H"


void draw_scale_line(_ROOM *pr, _COLOR color, _COLOR subcolor, _IVALUE astart, _IVALUE aarc, _IVALUE graduates, _IVALUE subgraduates)
{
  _DWORD tno;
  _DWORD alpha;
  _DOT   xy[5];
  _DWORD sub;
  _DWORD tmp;

  if(graduates > 1)
  {
    tmp = graduates + ((graduates-1) * subgraduates);
    tno = tmp;
    sub = subgraduates;
    while(tno)
    {
      tno  -= 1;
      alpha = astart + ((aarc * tno) / (tmp - 1));

      if(sub == subgraduates)
      {
        xy[0].x = 90;
        xy[0].y = 0;
        xy[1].x = 100;
        xy[1].y = 0;
        xy[2].x = 100;
        xy[2].y = 1;
        xy[3].x = 90;
        xy[3].y = 1;
        xy[4]   = xy[0];

		    selfmade_indicator(pr, alpha, color, xy, 5);
	      sub     = 0;
      }
      else
      {
        xy[0].x = 95;
        xy[0].y = 0;
        xy[1].x = 100;
        xy[1].y = 0;
		    selfmade_indicator(pr, alpha, subcolor, xy, 2);
	      sub     = sub + 1;
      }
    }
  }
}

void make_text_from_value(_CHAR *ptxt, _IVALUE value, _IO *pio, _VARIABLE *pv, _RESULT_TYPE restype)
{
	_RESULT result;

  // serverwert in struktur result stellen
  init_RESULT_Makro(&result);
  result.rmin  =
  result.rmax  =
  result.value = value;
  result.ftype = RES_NUMERIC;
  
  transform_RESULT(&result, restype);

  *ptxt = 0;
  // serverwert anhand seiner unit umrechnen
  if(recalculate(&result, pv, _TRUE) == _TRUE)
  {
    // wert anhand von unitinfo in string formatieren
    format_value(ptxt, result.value, &pv->info, &pio->picformat);
  }
}

void draw_value(_IO *pio, _VARIABLE *pv, _IVALUE value, _IVALUE angle, _RESULT_TYPE restype)
{
  // angle = 180+Astart+AArc/2

  _CHAR     txt[20];
  _ANYTHING anything;
  _CHAR     *ltxt;
  _DOT      xy[2];

  if(GetPenColor_Makro(pio->coltxt) != INVISIBLE)
  {
    // digitalen wert (für ausgabe) in string umwandeln
    make_text_from_value(txt, value, pio, pv, restype);

    // ermittle unittext
    init_ANYTHING_Makro(&anything);
    anything.no   = pio->variable.info[0].value;
    anything.list = VARUNIT;
    ltxt          = get_text(&anything, NULL, NULL, _TRUE);
    if(ltxt != NULL)
      StrCat(txt, sizeof(_CHAR), ltxt, sizeof(_CHAR));

    xy[0].x = 0;
    xy[0].y = 0;
    xy[1].x = 30;
    xy[1].y = 0;

    selfmade_indicator(&pio->space, angle, MakeColor_Makro(INVISIBLE, INVISIBLE), xy, 2);
    OutTextLse(xy[1].x, xy[1].y, txt, T_COPY|T_SOLID|T_MIDBOUND|T_CENTERBOUND|T_PROP, MakeColor_Makro(INVISIBLE, pio->coltxt), sizeof(_CHAR));
  }
  else
  {
    // folgende zeile nicht entfernen ansonsten passt font-attribut bei ausgabe von scale nicht (metagraphics is doof)
	  OutTextXY(pio->space.xy1.x, pio->space.xy1.y, " ", T_COPY|T_SOLID|T_LEFTBOUND|T_UPBOUND|T_PROP, MakeColor_Makro(INVISIBLE, BLACK), 1);
  }
}

void draw_scale_value(_ROOM *pr, _COLOR color, _IO *pio, _VARIABLE *pv, _IVALUE astart, _IVALUE aarc, _IVALUE graduates, _IVALUE vmax, _IVALUE vmin, _RESULT_TYPE restype)
{
  _DWORD tno;
  _DWORD alpha;
  _DOT   xy[2];
  _ROOM  room;
  _UWORD width;
  _UWORD height;
  _CHAR  txt[20];
  _UWORD tmp;

  if(graduates > 1)
  {
    // höhe und breite von skalierungszahlen berechnen (grundlage: min und/oder maxwert)
    font_size(pio->font, &width, &height);
    make_text_from_value(txt, vmax, pio, pv, restype);
    if(restype == RES_REAL)
      DeleteFinalZeroAfterDezimalPoint(txt);
    width = StrWidth(txt, sizeof(_CHAR));
    make_text_from_value(txt, vmin, pio, pv, restype);
    if(restype == RES_REAL)
      DeleteFinalZeroAfterDezimalPoint(txt);
    tmp = StrWidth(txt, sizeof(_CHAR));
    if(tmp > width)
	    width = tmp;

    // elliptischen raum für skalenwerte berechnen
    width       = width / 2;
    height      = height / 2;
    room        = *pr;
    room.xy1.y += height;
    room.xy2.y -= height;
    room.xy1.x += width;
    room.xy2.x -= width;

    // zahlenwerte auf bildschirm malen
    tno = graduates;
    while(tno)
    {
      tno -= 1;
      alpha = ((aarc * tno) / (graduates - 1));
      if((alpha < 360)&&(alpha > -360))
      {
        alpha = alpha + astart;

        xy[0].x = 0;
        xy[0].y = 0;
        xy[1].x = 87;
        xy[1].y = 0;

        selfmade_indicator(&room, alpha, MakeColor_Makro(INVISIBLE, INVISIBLE), xy, 2);
        
        if(restype == RES_REAL)
        {
          _REAL avv = (_REAL)vmin + (_REAL)((vmax - vmin) * tno) / (_REAL)(graduates - 1);
          make_text_from_value(txt, *(_IVALUE*)&avv, pio, pv, RES_NUMERIC); // RES_NUMERIC passt weil bereits REAL, ansonsten wird nochmals transformiert 
          DeleteFinalZeroAfterDezimalPoint(txt);
        }
        else
        {
          long tmp1 = vmin + (long)((((float)vmax - (float)vmin) * (float)tno) / (float)(graduates - 1)); // float wegen sa30029
          make_text_from_value(txt, tmp1, pio, pv, restype);
        }
	      OutTextLse(xy[1].x, xy[1].y, txt, T_COPY|T_SOLID|T_MIDBOUND|T_CENTERBOUND|T_PROP, color, sizeof(_CHAR));
      }
    }
  }
}

// info = Versionnumber
// memvarlabel[0] = AngleMin
// memvarlabel[1] = AngleMax
// memvarlabel[2] = ValueMin
// memvarlabel[3] = ValueMax
_BOOL _SubPicture::Out_THEURGE_Indicator(_IO *pio, _RESULT *pr, _VARIABLE *pv)
{
  _BOOL      retcode;
  _NEWSCREEN act;
  _DWORD     pixelvalue;
  _DWORD     tmp;
  _THEURGE   *ptu;
  _IVALUE    astart, aarc, graduates, subgraduates, vmax, vmin;
  _COLOR     color_subgraduates, color_graduates, color_indicator;
  _WORD      xoff, yoff, dataangle;

  retcode = _FALSE;

  switch(pio->attrib & T_HORIZBOUND)
  {
    case T_RIGHTBOUND  : xoff = (pio->space.xy2.x - pio->space.xy1.x);  break;
    case T_CENTERBOUND : xoff = 0;                      break;
    default            : xoff = -(pio->space.xy2.x - pio->space.xy1.x); break;
  }
  switch(pio->attrib & T_VERTBOUND)
  {
    case T_DOWNBOUND   : yoff = (pio->space.xy2.y - pio->space.xy1.y);  break;
    case T_MIDBOUND    : yoff = 0;                      break;
    default            : yoff = -(pio->space.xy2.y - pio->space.xy1.y); break;
  }

  dataangle = 0;
  if((pio->attrib & (T_VERTBOUND|T_HORIZBOUND)) == (T_MIDBOUND|T_CENTERBOUND))
    dataangle = 180;

  if(xoff > 0)
    pio->space.xy2.x += xoff;
  else if(xoff < 0)
    pio->space.xy1.x += xoff;

  if(yoff > 0)
    pio->space.xy2.y += yoff;
  else if(yoff < 0)
    pio->space.xy1.y += yoff;

  ptu = pio->theurge;

  if(ptu != NULL)
  {
    color_indicator    =
    color_graduates    =
    color_subgraduates = MakeColor_Makro(pio->coltxt, pio->coltxt);

    if(ptu->memvarlabel.no >= 4)
    {
      retcode = _TRUE;

      if(VarList.GetValue(&ptu->memvarlabel.ptr[0], &astart, _FALSE) == _FALSE)
        retcode = _FALSE;
      if(VarList.GetValue(&ptu->memvarlabel.ptr[1], &aarc, _FALSE) == _FALSE)
        retcode = _FALSE;
      if(VarList.GetValue(&ptu->memvarlabel.ptr[2], &vmin, _FALSE) == _FALSE)
        vmin = pr->rmin; // retcode = _FALSE; dr1104 
      if(VarList.GetValue(&ptu->memvarlabel.ptr[3], &vmax, _FALSE) == _FALSE)
        vmax = pr->rmax; // retcode = _FALSE; dr1104 

      graduates    = 0;
      subgraduates = 0;
      if(ptu->memvarlabel.no >= 5)
      {
        VarList.GetValue(&ptu->memvarlabel.ptr[4], &graduates, _FALSE);
        VarList.GetValue(&ptu->memvarlabel.ptr[5], &subgraduates, _FALSE);
      }

      if(ptu->memdyncolor.no >= 4)
      {
        color_graduates    = ptu->memdyncolor.ptr[0].color;
        color_graduates    = MakeColor_Makro(color_graduates, color_graduates);
        color_subgraduates = ptu->memdyncolor.ptr[1].color;
        color_subgraduates = MakeColor_Makro(color_subgraduates, color_subgraduates);
        color_indicator    = MakeColor_Makro(ptu->memdyncolor.ptr[2].color, ptu->memdyncolor.ptr[3].color);
      }
      else if(ptu->memcolor.no >= 4)
      {
        color_graduates    = ptu->memcolor.ptr[0];
        color_graduates    = MakeColor_Makro(color_graduates, color_graduates);
        color_subgraduates = ptu->memcolor.ptr[1];
        color_subgraduates = MakeColor_Makro(color_subgraduates, color_subgraduates);
        color_indicator    = MakeColor_Makro(ptu->memcolor.ptr[2], ptu->memcolor.ptr[3]);
      }
    }
  }

  if(retcode == _TRUE)
  {
    _DWORD vmin_val = vmin;
    _DWORD vmax_val = vmax;
    
    pixelvalue = pr->value;
    if(pr->ftype == RES_REAL)
    {
      _DWORD  xrng = vmax - vmin; 
      _DWORD  xmul = ((xrng > -1000) && (xrng < 1000))? 100 : 10;
      _RESULT xres = *pr;
      
      move_pREAL_REAL((_REAL*)&xres.value, get_pREAL((_REAL*)&xres.value) * xmul);
      transform_RESULT(&xres, RES_NUMERIC);
      pixelvalue = xres.value;
      vmin *= xmul;
      vmax *= xmul;
    }

    if(pixelvalue < vmin)
      pixelvalue = vmin;
    if(pixelvalue > vmax)
      pixelvalue = vmax;

    tmp = vmax - vmin;
    if(tmp != 0)
    {
      pixelvalue = astart + (long)((((float)pixelvalue - (float)vmin) * (float)aarc) / (float)tmp);  // float wegen sa30029
    }
    
    // auf unsichtbaren bildschirm schalten
    push_screen(&act);
    PutImage(&pio->image, DEFKOORD, DEFKOORD);

    // ggf. rahmen zeichnen
//    round_frame(pio->space.xy1.x, pio->space.xy1.y, pio->space.xy2.x, pio->space.xy2.y, pio->frametype, pio->attrib, pio->colframe);
    DrawRound(&pio->space, pio->attrib, pio->frametype, pio->colback, pio->colframe);

    // font setzen
    font_set(pio->font);

    // linien der skala ausgben
    draw_scale_line(&pio->space, color_graduates, color_subgraduates, astart, aarc, graduates, subgraduates);

    // digitalen wert samt unit ausgeben
    draw_value(pio, pv, pr->value, dataangle+astart+aarc/2, RES_NUMERIC); // RES_NUMERIC stimmt, ansonsten wird value nämlich transformiert

    // zahlenwerte der skala ausgeben. unbedingt nach ausgabe dig.wert weil sonst font-attribute nicht gesetzt ist
    draw_scale_value(&pio->space, MakeColor_Makro(INVISIBLE, color_graduates), pio, pv, astart, aarc, graduates, vmax_val, vmin_val, pr->ftype);

    // zeiger malen
    indicator(&pio->space, pixelvalue, color_indicator);

    // alles wieder auf sichtbaren bildschirm kopieren
    pop_screen(&act);
    copy_screen(&act, &pio->space);
  }

  if(xoff > 0)
    pio->space.xy2.x -= xoff;
  else if(xoff < 0)
    pio->space.xy1.x -= xoff;

  if(yoff > 0)
    pio->space.xy2.y -= yoff;
  else if(yoff < 0)
    pio->space.xy1.y -= yoff;

  return(retcode);
}

