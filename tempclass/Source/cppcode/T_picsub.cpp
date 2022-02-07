#include "T_IPC.H"
#include "T_NUC.H"

#define REFRESH_EDITOR  // refresh von eingabe im editor
#define SA19667
#define SA35472 // call writemethod of button::server bei triggerevent as well
#define SA34959 // bei button press + move über andere buttons werden diese -wenn in selber gruppe- auch gedrückt
#define SA54918 // negative Flankenfunktion aufrufen

#ifndef USER_DEMAG
 //#define SA35154 // show inaktiv images when access is not given
#endif

#ifndef _WIN_EDITOR_ALTGR_COLOR 
 #define _WIN_EDITOR_ALTGR_COLOR   LIGHTCYAN
#endif

void CallUserOverload(_SubPicture *Ppic, _UWORD summary)
{
  if((CallUserOverloadPtr != NULL)&&(Ppic != NULL))
  {
    CallUserOverloadPtr(summary, &Ppic->GetPICTURE()->mask);
  }
}

void SetCallUserOverload(_FBOOL_UWORD_PT ptr)
{
  CallUserOverloadPtr = ptr;
}

_BOOL is_redraw(_ROOM room, _DOT *offset, _ZOOM *zoom)
{

 #ifndef UC_REDRAW_ALL
  Zoom_ROOM(&room, offset, zoom);
  room.xy2.x += 1;
  room.xy2.y += 1;
  if(RoomInRegion(&room, GetActiveRegion()) == 0)
    return(_FALSE);
 #endif

  return(_TRUE);
}

void out_RECTANGLE(_RECTANGLE *p, _DOT *offset, _ZOOM *zoom)
{
  _ROOM room;

  if(DrawBackGround)
  {
    if(is_redraw(p->room, offset, zoom) == _FALSE)
      return;
//    p->color = LIGHTGREEN;
  }

	if(chk_CHKBIT(&p->chkbit) == _TRUE)
	{
    room = p->room;
    Zoom_ROOM(&room, offset, zoom);
	 #ifdef UC_MOUSE
		_UWORD hide = IfNecHideMouseCursor(&room);
	 #endif
    DrawButton(&room, p->attrib, p->frametype, p->color, p->colorframe);
	 #ifdef UC_MOUSE
		IfNecShowMouseCursor(hide);
	 #endif
   #ifdef UC_ROTATE
    RefreshRectangle(&room);
   #endif

  }
}

void out_POLYGON(_POLYGON *p, _DOT *offset, _ZOOM *zoom)
{
  _UWORD  i;
  _ATTRIB att;
  _COLOR  col, frame;
  _DOT    *pd = NULL;
  _DOT    *ph, *ps;

 #ifdef UC_ROTATE
  _ROOM room;
  room.xy1.x = 32000;
  room.xy1.y = 32000;
  room.xy2.x = -32000;
  room.xy2.y = -32000;
 #endif

  if((p->no > 1)&&(chk_CHKBIT(&p->chkbit) == _TRUE))
  {
    att   = p->attrib;
    col   = p->color;
    frame = p->colorframe;

    if((zoom->x!=DEFZOOM)||(zoom->y!=DEFZOOM)||(offset->x!=0)||(offset->y!=0))
    {
      if(Memory2((void**)&pd, p->no * sizeof(_DOT)) != 0)
      {
        ph = pd;
        ps = p->ptr;
        for(i=0; i<p->no; i++, ph++, ps++)
        {
          ph->x = Zoom(zoom->x, ps->x) + offset->x;
          ph->y = Zoom(zoom->y, ps->y) + offset->y;

         #ifdef UC_ROTATE
          if(ph->x < room.xy1.x)
            room.xy1.x = ph->x;
          if(ph->y < room.xy1.y)
            room.xy1.y = ph->y;
          if(ph->x > room.xy2.x)
            room.xy2.x = ph->x;
          if(ph->y > room.xy2.y)
            room.xy2.y = ph->y;
         #endif
        }

       #ifdef UC_MOUSE
        HideMouseCursor();
       #endif
        Polygon(p->no, pd, att, col);
        if(((att&T_FILL) == T_FILL)  && (frame != DEFCOLOR))
          Polygon(p->no, pd, SetOutlineAttrib(att), frame);
       #ifdef UC_ROTATE
        if((att & T_SIZE) > 0x11000000)
        {
          room.xy1.x -= 8;
          room.xy1.y -= 8;
          room.xy2.x += 8;
          room.xy2.y += 8;
        }
        RefreshRectangle(&room);
       #endif
       #ifdef UC_MOUSE
        ShowMouseCursor();
       #endif

        Memory2((void**)&pd, 0);
      }
    }
    else
    {
     #ifdef UC_ROTATE
      ps = p->ptr;
      for(i=0; i<p->no; i++, ps++)
      {
        if(ps->x < room.xy1.x)
          room.xy1.x = ps->x;
        if(ps->y < room.xy1.y)
          room.xy1.y = ps->y;
        if(ps->x > room.xy2.x)
          room.xy2.x = ps->x;
        if(ps->y > room.xy2.y)
          room.xy2.y = ps->y;
      }
     #endif

     #ifdef UC_MOUSE
      HideMouseCursor();
     #endif
     
      Polygon(p->no, p->ptr, att, col);
      
      if(((att&T_FILL) == T_FILL)  && (frame != DEFCOLOR))
        Polygon(p->no, p->ptr, SetOutlineAttrib(att), frame);
     #ifdef UC_ROTATE
      if((att & T_SIZE) > 0x11000000)
      {
        room.xy1.x -= 8;
        room.xy1.y -= 8;
        room.xy2.x += 8;
        room.xy2.y += 8;
      }
      RefreshRectangle(&room);
     #endif
     #ifdef UC_MOUSE
      ShowMouseCursor();
     #endif
    }
  }
}

void out_CIRCLE(_CIRCLE *p, _DOT *offset, _ZOOM *zoom)
{
	_ATTRIB att;
	_CIRCLE c;

	if(chk_CHKBIT(&p->chkbit)==_TRUE)
	{
		c   = *p;
		att = c.attrib;
    Zoom_ROOM(&c.room, offset, zoom);

	 #ifdef UC_MOUSE
		_UWORD hide = IfNecHideMouseCursor(&c.room);
	 #endif
		Oval(c.room.xy1.x, c.room.xy1.y, c.room.xy2.x, c.room.xy2.y, c.angle, c.arc, att, c.color);
		if(c.colorframe != DEFCOLOR)
		{
			att = SetOutlineAttrib(c.attrib);
			if(c.attrib & T_3D)
				att |= T_3D;
			Oval(c.room.xy1.x, c.room.xy1.y, c.room.xy2.x, c.room.xy2.y, c.angle, c.arc, att, c.colorframe);
		}

   #ifdef UC_ROTATE
      RefreshRectangle(&c.room);
   #endif

	 #ifdef UC_MOUSE
		IfNecShowMouseCursor(hide);
	 #endif
  }
}

void _SubPicture::Init(void)
{
	init_PICTURE(&Picture);
 #ifdef SA34959 
  InitMoveButtonGroup(); // alle move-button-merker initialisieren
 #endif 
}

void _SubPicture::Free(void)
{
	free_PICTURE(&Picture);
//	Init();speed
 #ifdef SA34959 
  InitMoveButtonGroup(); // alle move-button-merker initialisieren
 #endif 
}

void _SubPicture::ReDraw(_DOT *poffset, _ZOOM *pzoom)
{
  _UWORD       j;
  _ZORDER      *p;
  _PICTUREMASK *pm = &Picture.mask;
  _OBJECT      *po;
  _DOT         dot;
  _ZOOM        tmpzoom;
  _UDWORD      actin;
  
  p = pm->memzorder.ptr;
  for(j=0; j<pm->memzorder.no; j++, p++)
  {
    switch(p->ftype)
    {
//      case ZO_BUTTON       : if(p->no < pm->membutton.no)
//                               Refresh_BUTTON(&pm->membutton.ptr[p->no], poffset, pzoom, _TRUE);
//                             break;
      case ZO_OBJECT       : if(p->no < pm->memobject.no)
                             {
                               po = &pm->memobject.ptr[p->no];
                               
                               if(StateScheme.Call(&actin, &po->stscheme, NULL, STATE_ACTIVE) == _FALSE)
                               {
                                 actin = STATESCHEME_PRESELECTION;
                               }

                               if(actin != STATE_INVISIBLE) // sa29644
                               {
                                 if(po->SubPicture != NULL)
                                 {
		                               dot.x     = Zoom(pzoom->x, po->xy.x) + poffset->x;
		                               dot.y     = Zoom(pzoom->y, po->xy.y) + poffset->y;
                                   tmpzoom   = po->zoom;
                                   tmpzoom.x = Zoom_ZOOM(pzoom->x, tmpzoom.x);
                                   tmpzoom.y = Zoom_ZOOM(pzoom->y, tmpzoom.y);
                                   po->SubPicture->ReDraw(&dot, &tmpzoom);
                                 }
                               }
                             }
                             break;
      case ZO_TEXT         : if(p->no < pm->mempictxt.no)
                               Out_PICTEXT(&pm->mempictxt.ptr[p->no], poffset, pzoom, _TRUE);
                             break;
      case ZO_COLTEXT      : if(p->no < pm->colpictxt.no)
                               Out_COL_PICTXT(&pm->colpictxt.ptr[p->no], poffset, pzoom, _TRUE);
                             break;
      case ZO_INPUT        : if(p->no < pm->meminput.no)
                               OutIoIntern(&pm->meminput.ptr[p->no].io, poffset, pzoom, _TRUE, _TRUE, 0xFFFF, NULL, _FALSE);
                             break;
      case ZO_OUTPUT       : if(p->no < pm->memoutput.no)
                               OutIoIntern(&pm->memoutput.ptr[p->no], poffset, pzoom, _FALSE, _TRUE, 0xFFFF, NULL, _FALSE);
                             break;

    }
  }
}


void _SubPicture::Draw(_DOT *poffset, _ZOOM *pzoom, _Picture *Ppic)
{
  _UWORD       j;
  _ZORDER      *p;
  _KEY         *pk;
  _PICTUREMASK *pm = &Picture.mask;

  if(pm->memkey.no != 0)
  {
    j  = pm->memkey.no;
    pk = pm->memkey.ptr;
    while(j)
    {
      set_keymode(pk->code, pk->tip);
      pk += 1;
      j  -= 1;
    }
  }

  p = pm->memzorder.ptr;
  j = pm->memzorder.no;
  while(j)
  {
    switch(p->ftype)
    {
      case ZO_BUTTON       : if(p->no < pm->membutton.no)
                               Refresh_BUTTON(&pm->membutton.ptr[p->no], poffset, pzoom, _TRUE, Ppic);
                             break;
      case ZO_MOTION       : if(p->no < pm->memmotion.no)
                             {
                              #ifdef UC_OLD_MOTIONS
                               Refresh_MOTION(pm, p->no, poffset, pzoom, _TRUE);
                              #else
                               Refresh_MOTION(0xFFFF, &pm->memmotion.ptr[p->no], poffset, pzoom, _TRUE, NULL);
                              #endif
                             } 
                             break;
      case ZO_INPUT        : if(p->no < pm->meminput.no)
                               OutIoIntern(&pm->meminput.ptr[p->no].io, poffset, pzoom, _TRUE, _TRUE, 0xFFFF, NULL, _FALSE);
                             break;
      case ZO_OUTPUT       : if(p->no < pm->memoutput.no)
                               OutIoIntern(&pm->memoutput.ptr[p->no], poffset, pzoom, _FALSE, _TRUE, 0xFFFF, Ppic, _FALSE);
                             break;
      case ZO_OBJECT       : if(p->no < pm->memobject.no)
                               Refresh_OBJECT(0xFFFF, &pm->memobject.ptr[p->no], poffset, pzoom, _TRUE, Ppic); //NULL);
                             break;
      case ZO_RECTANGLE    : if(p->no < pm->memrectangle.no)
                               out_RECTANGLE(&pm->memrectangle.ptr[p->no], poffset, pzoom);
                             break;
      case ZO_POLYGON      : if(p->no < pm->mempolygon.no)
                               out_POLYGON(&pm->mempolygon.ptr[p->no], poffset, pzoom);
                             break;
      case ZO_CIRCLE       : if(p->no < pm->memcircle.no)
                               out_CIRCLE(&pm->memcircle.ptr[p->no], poffset, pzoom);
                             break;
      case ZO_TEXT         : if(p->no < pm->mempictxt.no)
                               Out_PICTEXT(&pm->mempictxt.ptr[p->no], poffset, pzoom, _TRUE);
                             break;
      case ZO_IMAGE        : if(p->no < pm->membitmap.no)
                               Out_BITMAP(&pm->membitmap.ptr[p->no], poffset, pzoom);
                             break;
      case ZO_COLRECTANGLE : if(p->no < pm->colrect.no)
                               Out_COL_RECT(&pm->colrect.ptr[p->no], poffset, pzoom, _TRUE);
                             break;
      case ZO_COLTEXT      : if(p->no < pm->colpictxt.no)
                               Out_COL_PICTXT(&pm->colpictxt.ptr[p->no], poffset, pzoom, _TRUE);
                             break;
      case ZO_COLCIRCLE    : if(p->no < pm->colcirc.no)
                               Out_COL_CIRC(&pm->colcirc.ptr[p->no], poffset, pzoom, _TRUE);
                             break;
      case ZO_COLPOLYGON   : if(p->no < pm->colpoly.no)
                               Out_COL_POLY(&pm->colpoly.ptr[p->no], poffset, pzoom, _TRUE);
                             break;
    }

    p += 1;
    j -= 1;
  }
}

void _SubPicture::Out_BITMAP(_BITMAP *p, _DOT *offset, _ZOOM *zoom)
{
  _BMP       *bmp;
  _BITMAP    t;
  _ATTRIB    attrib;

  if(DrawBackGround)
  {
    if(is_redraw(p->room, offset, zoom) == _FALSE)
      return;
  }

  if(chk_CHKBIT(&p->chkbit) == _TRUE)
  {
    bmp = (p->userbmp == NULL)? ImageList.Get(p->no) : p->userbmp;
    if(bmp != NULL)
    {
      t = *p;
      Zoom_ROOM(&t.room, offset, zoom);
      attrib = p->attrib & (~T_SIZE);
      out_zoom_BMP(&t.room, bmp, attrib);
     #ifdef UC_ROTATE
      RefreshRectangle(&t.room);
     #endif
    }
  }
}


void _SubPicture::Out_PICTEXT(_PICTEXT *p, _DOT *offset, _ZOOM *zoom, _BOOL init)
{
  _CHAR   *txt;
  _ROOM   room;
  _BOOL   status = _FALSE;
  _UWORD  actin;
  _UDWORD tmp, crc;

  if(DrawBackGround)
  {
    if(is_redraw(p->room, offset, zoom) == _FALSE)
      return;
  }

  if(chk_CHKBIT(&p->chkbit) == _TRUE)
  {
//    if(init == _TRUE)
//      doimage_PICTEXT(p);

    status = _TRUE;
    txt    = p->usertxt.ptr;
    if(txt == NULL)
      txt = get_text(&p->anypara.anything, &Picture.TextPuffer, &p->anypara.memvarlabel, _TRUE);

    if(StateScheme.Call(&tmp, &p->stscheme, NULL, STATE_ACTIVE) == _FALSE)
      tmp = STATESCHEME_PRESELECTION;

    actin = (_UWORD)tmp;
    crc   = (tmp<<16)^crc_string(txt, sizeof(_CHAR)); //((tmp<<16)^StringCRCrol(txt, sizeof(_CHAR)));

    if((crc != p->oldcrc)||(init == _TRUE))
    {
      room = p->room;
      Zoom_ROOM(&room, offset, zoom);

      if((p->image.ptr == NULL)&&(DrawBackGround == 0))
      {
        if(p->doimage == _TRUE)
          GetImage(&p->image, room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y);
      }

      status = _FALSE;
      if(DrawBackGround == 0)
        p->oldcrc = crc;

      if(actin != STATE_INVISIBLE)
      {
        if(p->image.ptr != NULL)
        {
          PutImage(&p->image, room.xy1.x, room.xy1.y);
        }
        if(txt != NULL)
        {
          OutMultiTextAlign(txt, p->fillchr, &room, p->font, p->attrib, p->frametype, p->coltxt, p->colback, p->colframe, sizeof(_CHAR));
        }

       #ifdef UC_ROTATE
        RefreshRectangle(&room);
       #endif

        status = _TRUE;
        p->onscreen = _TRUE;
      }
    }
  }

  if(status == _FALSE)
  {
    if((p->image.ptr != NULL)&&(p->onscreen == _TRUE))
    {
      PutImage(&p->image, DEFKOORD, DEFKOORD);
     #ifdef UC_ROTATE
      RefreshRectangle(&p->image.room);
     #endif
      if(DrawBackGround == 0)
        p->onscreen = _FALSE;
//        free_IMAGE(&p->image);
    }
  }
}


#ifdef OK_ENCLOSE_NAME
void _SubPicture::CallMyIoRun(_IO *pio, _BOOL input)
{
 #ifdef USER_DEMAG
  // SSC-07.10.11
  call_enclosed_name_method(METHOD_MYIO_RUN, pio, NULL, NULL, NULL, NULL, input);
 #else
  _REGION actreg, ioreg=NULL, newreg=NULL;

  // call virtual run-function
//  if(pio->virtname.pthis != NULL)
//  {
    actreg = ActivateAndRegion(&ioreg, &newreg, &pio->space);
    call_enclosed_name_method(METHOD_MYIO_RUN, pio, NULL, NULL, NULL, NULL, input);
    DeactivateAndRegion(actreg, &ioreg, &newreg);
//  }
  #endif
}
#endif

void _SubPicture::Refresh_OUTPUT(_IO *pio, _DOT *offset, _ZOOM *zoom, _UDWORD actno, _Picture *Ppic)
{
  OutIoIntern(pio, offset, zoom, _FALSE, _FALSE, actno, Ppic, _FALSE);
 #ifdef OK_ENCLOSE_NAME
  if(pio->virtname.pthis != NULL)
  {
    CallMyIoRun(pio, _FALSE);
  }
 #endif
}


void _SubPicture::Refresh_INPUT(_INPUT *pi, _DOT *offset, _ZOOM *zoom, _UDWORD actno, _Picture *Ppic)
{
  _IO *pio = &pi->io;

  if((pi->memseekno != actno)||(pi->io.bRefreshAct == _TRUE)||(pi->memseekno == 0xffff))
  {
    OutIoIntern(pio, offset, zoom, _TRUE, _FALSE, actno, Ppic, _FALSE);
   #ifdef OK_ENCLOSE_NAME
    if(pio->virtname.pthis != NULL)
      CallMyIoRun(pio, _FALSE);
   #endif
  }
 #ifdef REFRESH_EDITOR
  else
  {
   #ifdef OK_ENCLOSE_NAME
    if(pio->virtname.pthis != NULL)
      CallMyIoRun(pio, _FALSE);
   #endif

    if(Ppic != NULL)
    {
      if(pi->memseekno == actno)
      {
        _EDITOR *pedi = Ppic->GetEditor();
        if(is_SEEKELEMENT(Ppic->GetMyPicture(), pedi->actno) == _TRUE)
        {
          if(pedi->chredit.change == _FALSE)
          {
            _DOT  xoffset;
            _ZOOM xzoom;
            _BOOL merkd = pedi->input.io.doimage;
            init_DOT(&xoffset);
            init_ZOOM(&xzoom);
            pedi->input.io.doimage = _FALSE;
            OutIoIntern(&pedi->input.io, &xoffset, &xzoom, _TRUE, _FALSE, actno, Ppic, _TRUE);
            pedi->input.io.doimage = merkd;
           #ifdef OK_ENCLOSE_NAME
            if(pedi->input.io.virtname.pthis != NULL)
              CallMyIoRun(&pedi->input.io, _FALSE);
           #endif
          }
        }
      }
    }
  }
 #endif
}

_BOOL _SubPicture::RefreshAll(_UWORD actno, _DOT *offset, _ZOOM *zoom, _Picture *Ppic)
{
  _UWORD      no;
  _INPUT      *pi;
  _IO         *pio;
  _BUTTON     *pbu;
  _OBJECT     *pob;
  _MOTION     *pmm;
  _COL_RECT   *pcr;
  _COL_POLY   *pcp;
  _COL_CIRC   *pcc;
  _COL_PICTXT *pct;
  _PICTEXT    *ppt;

  no = Picture.mask.meminput.no;
  pi = Picture.mask.meminput.ptr;
  while(no--) // do all inputs ********************************************************************
  {
    Refresh_INPUT(pi, offset, zoom, actno, Ppic);
    pi++;
  }

  no  = Picture.mask.memoutput.no;
  pio = Picture.mask.memoutput.ptr;
  while(no--) // do all outputs *******************************************************************
  {
    Refresh_OUTPUT(pio, offset, zoom, actno, Ppic);
    pio++;
  }
  
  no  = Picture.mask.membutton.no;
  pbu = Picture.mask.membutton.ptr;
  while(no--) // do all buttons *******************************************************************
  {
    Refresh_BUTTON(pbu, offset, zoom, _FALSE, Ppic);
    pbu++;
  }
  
  no  = Picture.mask.memobject.no;
  pob = Picture.mask.memobject.ptr;
  while(no--) // do all objects *******************************************************************
  {
    if(pob->SubPicture != NULL)
    {
      Refresh_OBJECT(actno, pob, offset, zoom, _FALSE, Ppic);
    }
    pob++;
  }
  
  no  = Picture.mask.memmotion.no;
  pmm = Picture.mask.memmotion.ptr;
  while(no--) // do all motions *******************************************************************
  {
    Refresh_MOTION(actno, pmm, offset, zoom, _FALSE, Ppic);
    pmm++;
  }

  no  = Picture.mask.mempictxt.no;
  ppt = Picture.mask.mempictxt.ptr;
  while(no--) // do all placed text ***************************************************************
  {
    Out_PICTEXT(ppt, offset, zoom, _FALSE);
    ppt++;
  }

  no  = Picture.mask.colrect.no;
  pcr = Picture.mask.colrect.ptr;
  while(no--) // do all colored rectangles ********************************************************
  {
    Out_COL_RECT(pcr, offset, zoom, _FALSE);
    pcr++;
  }
  
  no  = Picture.mask.colpoly.no;
  pcp = Picture.mask.colpoly.ptr;
  while(no--) // do all colored polygons **********************************************************
  {
    Out_COL_POLY(pcp, offset, zoom, _FALSE);
    pcp++;
  }
  
  no  = Picture.mask.colcirc.no;
  pcc = Picture.mask.colcirc.ptr;
  while(no--) // do all colored circles ***********************************************************
  {
    Out_COL_CIRC(pcc, offset, zoom, _FALSE);
    pcc++;
  }
  
  no  = Picture.mask.colpictxt.no;
  pct = Picture.mask.colpictxt.ptr;
  while(no--) // do all colored text **************************************************************
  {
    Out_COL_PICTXT(pct, offset, zoom, _FALSE);
    pct++;
  }

  return(_TRUE);
}


_BOOL _SubPicture::RefreshPart(_UWORD actno, _DOT *offset, _ZOOM *zoom, _Picture *Ppic)
{
  _BOOL retcode = _FALSE;
  _UDWORD nox;
  
//  if(Picture.actual.cnt_priority != 0)
    
  if(Picture.mask.meminput.no > 0) // do actual input ******************************************
  {
    if(Picture.actual.act_input >= Picture.mask.meminput.no)
    {
      Picture.actual.act_input = 0;
    }
    Refresh_INPUT(&Picture.mask.meminput.ptr[Picture.actual.act_input], offset, zoom, actno, Ppic);
    Picture.actual.act_input++;
  }
  
  if(Picture.mask.memoutput.no > 0) // do actual output ****************************************
  {
    nox = (Picture.mask.memoutput.no + 1) / 2; // mehrere outputs refreshen
    while(nox--)
    {
      if(Picture.actual.act_output >= Picture.mask.memoutput.no)
      {
        Picture.actual.act_output = 0;
      }
      Refresh_OUTPUT(&Picture.mask.memoutput.ptr[Picture.actual.act_output], offset, zoom, actno, Ppic);
      Picture.actual.act_output++;
    }
  }

  if(Picture.mask.membutton.no > 0) // do actual button ****************************************
  {
    if(Picture.actual.act_button >= Picture.mask.membutton.no)
    {
      Picture.actual.act_button = 0;
    }
    Refresh_BUTTON(&Picture.mask.membutton.ptr[Picture.actual.act_button], offset, zoom, _FALSE, Ppic);
    Picture.actual.act_button++;
  }

  if(Picture.mask.memobject.no > 0) // do actual object ****************************************
  {
    nox = (Picture.mask.memobject.no + 1) / 2; // mehrere objekte refreshen
    while(nox--)
    {
      if(Picture.actual.act_object >= Picture.mask.memobject.no)
      {
        Picture.actual.act_object = 0;
      }
      _OBJECT *pob = &Picture.mask.memobject.ptr[Picture.actual.act_object];
      if(pob->SubPicture != NULL)
      {
        Refresh_OBJECT(actno, pob, offset, zoom, _FALSE, Ppic);
      }
      Picture.actual.act_object++;
    }
  }

  if(Picture.mask.memmotion.no > 0) // do actual motion ****************************************
  {
    if(Picture.actual.act_motion >= Picture.mask.memmotion.no)
    {
      Picture.actual.act_motion = 0;
    }
    Refresh_MOTION(actno, &Picture.mask.memmotion.ptr[Picture.actual.act_motion], offset, zoom, _FALSE, Ppic);
    Picture.actual.act_motion++;
  }
  
  if(Picture.mask.mempictxt.no > 0) // do actual text ******************************************
  {
    if(Picture.actual.act_pictext >= Picture.mask.mempictxt.no)
    {
      Picture.actual.act_pictext = 0;
    }
    Out_PICTEXT(&Picture.mask.mempictxt.ptr[Picture.actual.act_pictext], offset, zoom, _FALSE);
    Picture.actual.act_pictext++;
  }

  if(Picture.mask.colrect.no > 0) // do actual colored rectangle *******************************
  {
    if(Picture.actual.act_colrect >= Picture.mask.colrect.no)
    {
      Picture.actual.act_colrect = 0;
    }
    Out_COL_RECT(&Picture.mask.colrect.ptr[Picture.actual.act_colrect], offset, zoom, _FALSE);
    Picture.actual.act_colrect++;
  }

  if(Picture.mask.colpoly.no > 0) // do actual colored plygons *********************************
  {
    if(Picture.actual.act_colpoly >= Picture.mask.colpoly.no)
    {
      Picture.actual.act_colpoly = 0;
    }
    Out_COL_POLY(&Picture.mask.colpoly.ptr[Picture.actual.act_colpoly], offset, zoom, _FALSE);
    Picture.actual.act_colpoly++;
  }

  if(Picture.mask.colcirc.no > 0) // do actual colored circles *********************************
  {
    if(Picture.actual.act_colcirc >= Picture.mask.colcirc.no)
    {
      Picture.actual.act_colcirc = 0;
    }
    Out_COL_CIRC(&Picture.mask.colcirc.ptr[Picture.actual.act_colcirc], offset, zoom, _FALSE);
    Picture.actual.act_colcirc++;
  }

  if(Picture.mask.colpictxt.no > 0) // do actual colored text **********************************
  {
    if(Picture.actual.act_colpictxt >= Picture.mask.colpictxt.no)
    {
      Picture.actual.act_colpictxt = 0;
    }
    Out_COL_PICTXT(&Picture.mask.colpictxt.ptr[Picture.actual.act_colpictxt], offset, zoom, _FALSE);
    Picture.actual.act_colpictxt++;
  }
  
  return retcode;
}

_BOOL _SubPicture::Refresh(_UWORD actno, _DOT *offset, _ZOOM *zoom, _Picture *Ppic)
{
  _BOOL retcode = _FALSE;
  
  if(GlobalChangeSys != 0)
  {
    retcode = RefreshAll(actno, offset, zoom, Ppic);
  }
  else 
  {
    retcode = RefreshPart(actno, offset, zoom, Ppic);
  }
  
  return retcode;
}



_BOOL _SubPicture::OutIoComponent(_IO *pio, _RESULT *pr, _VARIABLE *pv, _DOT *offset, _ZOOM *zoom, _BOOL input, _UWORD actno, _Picture *Ppic, _BOOL iseditor)
{
  _IO   tmp;
  _BOOL retcode = _FALSE;

  tmp = *pio;

 #ifdef OK_VALIDATE
  if(input == _TRUE)
  {
    if(Validate.HasChanged(&pio->variable) == _TRUE)
      tmp.color = MakeColor_Makro(GetBackColor_Makro(tmp.color), LIGHTRED);
  }
 #endif

//  if(init == _TRUE)
//    tmp.cstate.init = _TRUE;

  if(SingleOutIo(&tmp, pr, pv, offset, zoom, input, actno, Ppic, iseditor) == _TRUE)
  {
  
    if(pio->typ == AS_ENUMERATION)
    {
      pio->typ = tmp.typ;
      pio->info = tmp.info;
    }
  
  
    pio->space       = tmp.space;
    pio->Obj         = tmp.Obj;
    pio->image       = tmp.image;
    pio->workstate   = tmp.workstate;
   #ifdef OK_ENCLOSE_NAME
    pio->virtname    = tmp.virtname;
   #endif
    pio->info.mytxtscheme = tmp.info.mytxtscheme;
    pio->xuser       = tmp.xuser;

    retcode = _TRUE;

    pio->cstate.init = tmp.cstate.init;

    if(DrawBackGround == 0)
    {
			pio->cstate      = tmp.cstate;
      pio->refcolback  = tmp.colback;       // reference color back (hatch)
      pio->refcolframe = tmp.refcolframe;   // reference color frame (top-left, bottom-right)
      pio->refcoltxt   = tmp.refcoltxt;     // reference color text
    }

   #ifdef UC_MENU_TWOCOLOR
    if(pio->typ == AS_MENU)
    {
      pio->refstschemestate = tmp.refstschemestate;
    }
   #endif

  }

  return(retcode);
}

#ifdef SA19667
_BOOL _SubPicture::CaptureBackGround(_IO *pio, _DOT *offset, _ZOOM *zoom)
{
//  if(pio->doimage == _TRUE)
//  {
    if(pio->image.ptr == NULL) //&& (pio->cstate.init == _TRUE)) //(DrawBackGround == 0))
    {
      if(SingleSizeIo(pio, offset, zoom) == _TRUE)
      {
        if((pio->typ == AS_THEURGE) && (pio->theurge != NULL))
        {
          if(pio->theurge->memimage.no != 0)
          {
            if((pio->theurge->memimage.ptr[0].list == ICONLST) && (pio->theurge->memimage.ptr[0].no != 0xffff))
              out_ANYTHING(&pio->theurge->memimage.ptr[0], &pio->space, DEFFONT, T_COPY|T_SOLID, _DEFFRAME, DEFCOLOR, DEFCOLOR, DEFCOLOR, NULL);
          }
        }

       #ifdef UC_MOUSE
        _UWORD hide = IfNecHideMouseCursor(&pio->space);
       #endif
        GetImage(&pio->image, pio->space.xy1.x, pio->space.xy1.y, pio->space.xy2.x, pio->space.xy2.y);
       #ifdef UC_MOUSE
        IfNecShowMouseCursor(hide);
       #endif
       return _TRUE;
      }
    }
//  }
  return _FALSE;
}
#endif

_BOOL _SubPicture::OutIoIntern(_IO *pio, _DOT *offset, _ZOOM *zoom, _BOOL input, _BOOL init, _UWORD actno, _Picture *Ppic, _BOOL iseditor)
{
  _RESULT   res;
  _VARIABLE var;
  _UWORD    actin;
  _UDWORD   tmp;
  _BOOL     status = _FALSE;
  _BOOL     retcode = _TRUE;

  if(DrawBackGround)
  {
    if(is_redraw(pio->room, offset, zoom) == _FALSE)
      return(_TRUE);
  }

  init_RESULT_Makro(&res);

  if(chk_CHKBIT(&pio->chkbit) == _TRUE)
  {
   #ifdef OK_ENCLOSE_NAME
    if(pio->virtname.no != 0xFFFF)
    {
      if(pio->virtname.pthis == NULL)
      {
        SingleSizeIo(pio, offset, zoom);     // calculate size for _MyIo::Start interface
        if(FindVirtualName(pio) == _FALSE)
          return(_FALSE);
      }
    }
   #endif

    _BOOL dolimit = _TRUE;

   #ifdef _OUTPUT_NOLIMIT
    if(input == _FALSE)
    {
      if((pio->typ == AS_NUMERIC)&&(pio->virtname.pthis == NULL))
      {
        dolimit = _FALSE;  // bei num output sind keine grenzwerte notwendig
      }
    }
   #endif

    if(pio->typ == AS_THEURGE)
    {
      retcode = GetValueTHEURGE(&res, &var, pio, dolimit);
    }
    else
    {
      retcode = VarList.GetSystemData(&res, &var, &pio->variable, _FALSE, dolimit, _TRUE);
    }

   #ifdef OK_ENCLOSE_NAME
    if(pio->virtname.pthis != NULL)
    {
      switch(call_enclosed_name_method(METHOD_MYIO_GETDATA, pio, &var, &res, NULL, NULL, input))
      {
        case _IFAILED : return(_FALSE);
        case _IDIDIT  : retcode = _TRUE; break;
        default       : break;
      }
       
    }
   #endif

    if(retcode == _TRUE)
    {
      if(var.info.reftime < 100)
      {
        //pio->priority = var.info.reftime/5;
        pio->priority = 30;
        if(var.info.reftime <= 10)
          pio->priority = 1;
        else if(var.info.reftime <= 50)
          pio->priority = 10;
      }

      if(StateScheme.Call(&tmp, &pio->stscheme, NULL, STATE_ACTIVE) == _FALSE)
      {
        tmp = STATESCHEME_PRESELECTION;
      }
      
      actin = (_UWORD)tmp;

      if(actin != STATE_INVISIBLE)
      {
        status  = _TRUE;
        if(init == _TRUE)
          pio->cstate.init = _TRUE;
        retcode = OutIoComponent(pio, &res, &var, offset, zoom, input, actno, Ppic, iseditor);
        if(retcode == _TRUE)
        {
         #ifdef UC_ROTATE
          RefreshRectangle(&pio->space);
         #endif
          pio->onscreen = _TRUE;
        }
      }
#ifdef SA19667      
      else if(pio->virtname.pthis != NULL) // nur bei MyIo
      {
        if(CaptureBackGround(pio, offset, zoom) == _TRUE)
          return _TRUE;
      }
#endif
    }
  }

  if(status == _FALSE) // perhaps we've to extinguish io from screen
  {
#ifdef SA19667  
    if((pio->image.ptr != NULL) && (pio->onscreen == _TRUE))
#else
    if(pio->image.ptr != NULL)
#endif
    {
      pio->onscreen = _FALSE;
//      PutImage(&pio->image, pio->space.xy1.x, pio->space.xy1.y);
     #ifdef UC_MOUSE
      HideMouseCursor();
     #endif
      PutImage(&pio->image, DEFKOORD, DEFKOORD); //pio->space.xy1.x, pio->space.xy1.y);
     #ifdef UC_MOUSE
      ShowMouseCursor();
     #endif
     #ifdef UC_ROTATE
		  RefreshRectangle(&pio->space);
     #endif
     
#ifdef SA19667
      if((pio->virtname.pthis == NULL) && (DrawBackGround == 0))
        free_IMAGE(&pio->image);
#else
      if(DrawBackGround == 0)
        free_IMAGE(&pio->image);
#endif

    }
    free_CSTATE(&pio->cstate);
  }

  return(retcode);
}

#ifdef UC_OLD_MOTIONS
_BOOL _SubPicture::Refresh_MOTION(_PICTUREMASK *pm, _UWORD i, _DOT *offset, _ZOOM *zoom, _BOOL init)
{
  // retcode wird für refresh verwendet und nicht für erfolg ob objekt ausgegeben
  // werden konnte.
  // retcode = _TRUE wenn refresh fertig oder objekt nit vorhanden, geladen etc.
  _MOTION     *m;
  _RESULT     resx, resy;
  _VARIABLE   var;
  _WORD       dx, dy;
  _DOT        xy;
  _BOOL       retcode = _TRUE;
  _MEM_MOTION *p = &pm->memmotion;

  m = &p->ptr[i];
  if(chk_CHKBIT(&m->chkbit) == _TRUE)
  {
    if((VarList.GetSystemData(&resx, &var, &m->varx, _FALSE, _TRUE) == _TRUE) &&
       (VarList.GetSystemData(&resy, &var, &m->vary, _FALSE, _TRUE) == _TRUE))
    {
			dx      = linear_distance(&resx, Zoom(zoom->x, m->room.xy2.x-m->room.xy1.x));
      dy      = linear_distance(&resy, Zoom(zoom->y, m->room.xy2.y-m->room.xy1.y));
      xy.x    = Zoom(zoom->x, m->room.xy1.x) + offset->x + dx;
      xy.y    = Zoom(zoom->y, m->room.xy1.y) + offset->y + dy;
      retcode = Out_MOTION(m, xy, init);
     #ifdef UC_ROTATE
//		  RefreshRectangle(&room);
     #endif
    }
  }

  return(retcode);
}

void calculate_ROOM(_ROOM *dest, _ROOM *src, _UWORD no)
{
  _ROOM room, *s;

  room.xy1.x = 32000;
  room.xy1.y = 32000;
  room.xy2.x = -32000;
  room.xy2.y = -32000;

  while(no)
  {
    no--;
    s = &src[no];
    linear_coordinates(&s->xy1.x, &s->xy1.y, &s->xy2.x, &s->xy2.y);
    if(s->xy1.x < room.xy1.x) room.xy1.x = s->xy1.x;
    if(s->xy1.y < room.xy1.y) room.xy1.y = s->xy1.y;
    if(s->xy2.x > room.xy2.x) room.xy2.x = s->xy2.x;
    if(s->xy2.y > room.xy2.y) room.xy2.y = s->xy2.y;
  }

  *dest = room;
}


_BOOL _SubPicture::Out_MOTION(_MOTION *p, _DOT xy, _BOOL init)
{
  // retcode wird für refresh verwendet und nicht für erfolg ob objekt ausgegeben
  // werden konnte.
  // retcode = _TRUE wenn refresh fertig oder objekt nit vorhanden, geladen etc.
  _ZOOM      zoom;
  _BOOL      retcode = _TRUE;
  _ROOM      room, sroom[2];
  _NEWSCREEN act;

  if(p->workstate == STATE_START)
  {
    load_object(&p->SubPicture, p->anything.no);
    if(p->SubPicture != NULL)
    {
      p->SubPicture->GetSize(&room);
      p->width  = room.xy2.x-room.xy1.x;
      p->height = room.xy2.y-room.xy1.y;
      p->workstate = STATE_READY;
    }
  }

  if(p->workstate == STATE_READY)
  {
    init_ZOOM(&zoom);
    room.xy1    = xy;
    room.xy2    = xy;
    room.xy2.x += p->width;
    room.xy2.y += p->height;
   #ifdef UC_MOUSE
    HideMouseCursor();
   #endif
    GetImage(&p->image, room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y);
    p->SubPicture->Draw(&xy, &zoom, NULL);
   #ifdef UC_MOUSE
    ShowMouseCursor();
   #endif
    p->oldxy     = xy;
    if(chk_VARLABEL(&p->loader) == _TRUE)
      p->workstate = STATE_OVERLOAD;
    else
      p->workstate = STATE_RUN;
  }

  if(p->workstate == STATE_OVERLOAD)
  {
    p->workstate = STATE_RUN;
//    if(p->lock_overload == _FALSE)
    p->SubPicture->Overload(&p->loader);
    CallUserOverload(p->SubPicture, p->summary);
  }

  if((p->workstate == STATE_RUN) || (p->workstate == STATE_BACKGROUND))
  {
    if((xy.x != p->oldxy.x) || (xy.y != p->oldxy.y) || (p->workstate == STATE_BACKGROUND))
    {
      sroom[0].xy1    = p->oldxy;
      sroom[0].xy2    = p->oldxy;
      sroom[1].xy1    = xy;
      sroom[1].xy2    = xy;
      sroom[0].xy2.x += p->width;
      sroom[0].xy2.y += p->height;
      sroom[1].xy2.x += p->width;
      sroom[1].xy2.y += p->height;
      calculate_ROOM(&room, sroom, 2);

      GetScreen(&act);
     #ifdef UC_MOUSE
      HideMouseCursor();
     #endif
     #ifdef OK_OFFSCREEN
      if(UseOffscreen == _TRUE)
      {
        if(DrawBackGround == 0)
        {
          ActivateScreen(&OffScreen);
          CopyScreen(&OffScreen, &act, &room, &room);
        }
      }
     #endif

      ScreenStop();

      PutImage(&p->image, DEFKOORD, DEFKOORD);

     #ifdef UC_ROTATE
      RefreshRectangle(&p->image.room);
     #endif

      sroom[0].xy1    = xy;
      sroom[0].xy2    = xy;
      sroom[0].xy2.x += p->width;
      sroom[0].xy2.y += p->height;
      if(DrawBackGround == 0)
        GetImage(&p->image, sroom[0].xy1.x, sroom[0].xy1.y, sroom[0].xy2.x, sroom[0].xy2.y);

      init_ZOOM(&zoom);
      p->SubPicture->Draw(&xy, &zoom, NULL);
     #ifdef OK_OFFSCREEN
      if(UseOffscreen == _TRUE)
      {
        if(DrawBackGround == 0)
        {
          ActivateScreen(&act);
          CopyScreen(&act, &OffScreen, &room, &room);
        }
      }
     #endif
     #ifdef UC_MOUSE
      ShowMouseCursor();
     #endif

      p->oldxy     = xy;
      p->workstate = STATE_RUN;

      ScreenStart();
    }
    else if(p->SubPicture != NULL)
    {
      init_ZOOM(&zoom);
      if(init == _FALSE)
        retcode = p->SubPicture->Refresh(0xFFFF, &xy, &zoom, NULL);
      else
      {
        p->SubPicture->Draw(&xy, &zoom, NULL);
        retcode = _TRUE;
      }
    }
  }

  return(retcode);
}

#else





_BOOL _SubPicture::Refresh_MOTION(_UWORD actno, _MOTION *p, _DOT *offset, _ZOOM *zoom, _BOOL init, _Picture *Ppic)
{
  // retcode wird für refresh verwendet und nicht für erfolg ob objekt ausgegeben
  // werden konnte.
  // retcode = _TRUE wenn refresh fertig oder objekt nit vorhanden, geladen etc.
	_BOOL     retcode = _TRUE;
  _VARIABLE var;
  _RESULT   resx, resy;
  _KOORD    dx, dy;
	_DOT      dot;
  _ROOM     room;
  _UWORD    height, width;
  _ZOOM     tmpzoom;
  _BOOL     status = _FALSE;
  _UDWORD   tmp;

  if(p->memkaru.no > 0)
    return Refresh_CAROUSEL(actno, p, offset, zoom, init, Ppic, 40);

  if(chk_CHKBIT(&p->chkbit) == _TRUE)
	{
    status = _TRUE;
    if(StateScheme.Call(&tmp, &p->stscheme, NULL, STATE_ACTIVE) == _FALSE)
      tmp = STATESCHEME_PRESELECTION;
          
    if(tmp == STATE_INVISIBLE)
      status = _FALSE;
  }
  
  if(status == _TRUE)
  {
    if((p->workstate == STATE_START) && (DrawBackGround == 0))
    {
      if(load_object(&p->SubPicture, p->anything.no) == _TRUE)
      {
        p->SubPicture->Overload(&p->loader);
        CallUserOverload(p->SubPicture, p->summary);
        
        tmpzoom   = p->zoom;
        tmpzoom.x = Zoom_ZOOM(zoom->x, tmpzoom.x);
        tmpzoom.y = Zoom_ZOOM(zoom->y, tmpzoom.y);
        
        room   = p->SubPicture->GetPICTURE()->mask.room;
        width  = Zoom(tmpzoom.x, room.xy2.x - room.xy1.x + 1);
        height = Zoom(tmpzoom.y, room.xy2.y - room.xy1.y + 1);
        room   = p->room;
        Zoom_ROOM(&room, offset, zoom);
        linear_coordinates(&room.xy1.x, &room.xy1.y, &room.xy2.x, &room.xy2.y); // sa27167
        room.xy2.x += width;
        room.xy2.y += height;
//        linear_coordinates(&room.xy1.x, &room.xy1.y, &room.xy2.x, &room.xy2.y); // sa27167
        //Rectangle(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY | T_SOLID, LIGHTGREEN); // test
       #ifndef USER_SMT 
        GetImage(&p->image, room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y);
       #endif
        p->workstate = STATE_READY;
      }
      else
      {
        p->workstate = STATE_ERROR;
      }
    }

    if(p->workstate != STATE_ERROR)
    {
      if(p->SubPicture != NULL)
      {
        if(p->workstate == STATE_BACKGROUND)
          p->workstate = STATE_RUN;
      
        if(p->workstate == STATE_OVERLOAD)
        {
//          if(p->lock_overload == _FALSE)
          p->SubPicture->Overload(&p->loader);
          CallUserOverload(p->SubPicture, p->summary);
          p->workstate = STATE_RUN;
        }      
      
        if(VarList.GetSystemData(&resx, &var, &p->varx, _FALSE, _TRUE) == _TRUE)
        {
          if(VarList.GetSystemData(&resy, &var, &p->vary, _FALSE, _TRUE) == _TRUE)
          {
            dx = linear_distance(&resx, Zoom(zoom->x, p->room.xy2.x - p->room.xy1.x + 1));
            dy = linear_distance(&resy, Zoom(zoom->y, p->room.xy2.y - p->room.xy1.y + 1));
            dot.x = Zoom(zoom->x, p->room.xy1.x) + offset->x + dx;
            dot.y = Zoom(zoom->y, p->room.xy1.y) + offset->y + dy;

            tmpzoom   = p->zoom;
            tmpzoom.x = Zoom_ZOOM(zoom->x, tmpzoom.x);
            tmpzoom.y = Zoom_ZOOM(zoom->y, tmpzoom.y);

            if((p->workstate == STATE_READY) || (DrawBackGround != 0))
            {
               p->oldxy.y = dot.y + 1;
               p->workstate = STATE_RUN;
            }

            if(p->workstate == STATE_RUN)
            {
              if((dot.x != p->oldxy.x) || (dot.y != p->oldxy.y))
              {
                //_REGION actreg, ioreg=NULL, newreg=NULL;
                //actreg = ActivateAndRegion(&ioreg, &newreg, &p->image.room);
               #ifdef UC_MOUSE
                HideMouseCursor();
               #endif
                ScreenStop();
               #ifndef USER_SMT 
                PutImage(&p->image, DEFKOORD, DEFKOORD);
               #endif 
                p->SubPicture->Draw(&dot, &tmpzoom, Ppic);
                p->onscreen = _TRUE;
                ScreenStart();
               #ifdef UC_MOUSE
                ShowMouseCursor();
               #endif
                //DeactivateAndRegion(actreg, &ioreg, &newreg);

               #ifdef UC_ROTATE
                RefreshRectangle(&p->image.room);
               #endif
               
                if(DrawBackGround == 0)
                  p->oldxy = dot;
              }
              else 
              {
                retcode = p->SubPicture->Refresh(actno, &p->oldxy, &tmpzoom, Ppic);
              }
            }
          }
        }
      }
    }
	}
  else if(p->onscreen == _TRUE)
  {
   #ifdef UC_MOUSE
    HideMouseCursor();
   #endif
    ScreenStop();
    PutImage(&p->image, DEFKOORD, DEFKOORD);
    ScreenStart();
   #ifdef UC_MOUSE
    ShowMouseCursor();
   #endif

   #ifdef UC_ROTATE
    RefreshRectangle(&p->image.room);
   #endif
    if(DrawBackGround == 0)
    {
      p->onscreen = _FALSE;
      free_IMAGE(&p->image);
      if(p->SubPicture != NULL)
      {
        p->SubPicture->Free();
        Memory2((void**)&p->SubPicture, 0);
      }
      p->workstate = STATE_START;
    }
  }

	return(retcode);
}

#endif

_BOOL _SubPicture::Read(_File *File)
{
  Free();

  File->Read_PICTURE(&Picture);

  return((File->GetError() == _FALSE)? _TRUE : _FALSE);
}


_MOTION *_SubPicture::Get_MOTION(_DWORD index)
{
  _MOTION *retcode = NULL;

  if((index >= 0) && (index < Picture.mask.memmotion.no))
    retcode = &Picture.mask.memmotion.ptr[index];
    
  return retcode;
}

_BOOL _SubPicture::Out_COL_RECT(_COL_RECT *p, _DOT *offset, _ZOOM *zoom, _BOOL init)
{
  _BOOL      retcode = _FALSE;
  _UDWORD    colhi, collo, framehi, framelo;
  _RECTANGLE rectangle;

  if(DrawBackGround)
  {
    if(is_redraw(p->rectangle.room, offset, zoom) == _FALSE)
      return(_TRUE);
  }

  if(init == _TRUE)
    p->init = _TRUE;

  if(ColorScheme.Call(&colhi, &p->dyncolback, GetBackColor_Makro(p->rectangle.color), _TRUE) == _TRUE)
  {
    if(ColorScheme.Call(&collo, &p->dyncolfront, GetPenColor_Makro(p->rectangle.color), _TRUE) == _TRUE)
    {
      if(ColorScheme.Call(&framehi, &p->dyncoltl, GetBackColor_Makro(p->rectangle.colorframe), _TRUE) == _TRUE)
      {
        if(ColorScheme.Call(&framelo, &p->dyncolbr, GetPenColor_Makro(p->rectangle.colorframe), _TRUE) == _TRUE)
        {
          rectangle            = p->rectangle;
          rectangle.color      = MakeColor_Makro((_UWORD)colhi, (_UWORD)collo);
          rectangle.colorframe = MakeColor_Makro((_UWORD)framehi, (_UWORD)framelo);

          if((p->init == _TRUE)||(rectangle.colorframe != p->refcolorframe)||(rectangle.color != p->refcolor))
          {
            out_RECTANGLE(&rectangle, offset, zoom);
            p->init          = init; //_FALSE;
            p->refcolor      = rectangle.color;
            p->refcolorframe = rectangle.colorframe;
            retcode = _TRUE;
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL _SubPicture::Out_COL_CIRC(_COL_CIRC *p, _DOT *offset, _ZOOM *zoom, _BOOL init)
{
  _BOOL   retcode = _FALSE;
  _CIRCLE circle;
  _UDWORD colhi, collo, frame;

  if(init == _TRUE)
    p->init = _TRUE;

  if(ColorScheme.Call(&colhi, &p->dyncolback, GetBackColor_Makro(p->circle.color), _TRUE) == _TRUE)
  {
    if(ColorScheme.Call(&collo, &p->dyncolfront, GetPenColor_Makro(p->circle.color), _TRUE) == _TRUE)
    {
      if(ColorScheme.Call(&frame, &p->dyncolframe, GetPenColor_Makro(p->circle.colorframe), _TRUE) == _TRUE)
      {
        circle            = p->circle;
        circle.colorframe = MakeColor_Makro((_UWORD)frame, (_UWORD)frame);
        circle.color      = MakeColor_Makro((_UWORD)colhi, (_UWORD)collo);

        if((p->init == _TRUE)||(circle.colorframe != p->refcolorframe)||(circle.color != p->refcolor))
        {
          out_CIRCLE(&circle, offset, zoom);
          p->refcolor      = circle.color;
          p->refcolorframe = circle.colorframe;
          p->init          = init; //_FALSE;
          retcode = _TRUE;
        }
      }
    }
  }

  return(retcode);
}

_BOOL _SubPicture::Out_COL_PICTXT(_COL_PICTXT *p, _DOT *offset, _ZOOM *zoom, _BOOL init)
{
  _BOOL    retcode = _FALSE;
  _UDWORD  backhi, backlo, framehi, framelo, txt;
  _PICTEXT pictext;

  if(DrawBackGround)
  {
    if(is_redraw(p->btx.room, offset, zoom) == _FALSE)
      return(_TRUE);
  }

  if(init == _TRUE)
    p->init = _TRUE;

  if(ColorScheme.Call(&backhi, &p->dyncolback, GetBackColor_Makro(p->btx.colback), _TRUE) == _TRUE)
  {
    if(ColorScheme.Call(&backlo, &p->dyncolfront, GetPenColor_Makro(p->btx.colback), _TRUE) == _TRUE)
    {
      if(ColorScheme.Call(&framehi, &p->dyncoltl, GetBackColor_Makro(p->btx.colframe), _TRUE) == _TRUE)
      {
        if(ColorScheme.Call(&framelo, &p->dyncolbr, GetPenColor_Makro(p->btx.colframe), _TRUE) == _TRUE)
        {
          if(ColorScheme.Call(&txt, &p->dyncoltxt, GetPenColor_Makro(p->btx.coltxt), _TRUE) == _TRUE)
          {
            pictext = p->btx;

            pictext.colback  = MakeColor_Makro((_UWORD)backhi, (_UWORD)backlo);
            pictext.colframe = MakeColor_Makro((_UWORD)framehi, (_UWORD)framelo);
            pictext.coltxt   = MakeColor_Makro((_UWORD)txt, (_UWORD)txt);

            if((pictext.colframe != p->refcolframe)||(pictext.colback != p->refcolback)||(pictext.coltxt != p->refcoltxt)) //||(actin != crc))
              init = _TRUE;

            Out_PICTEXT(&pictext, offset, zoom, init);

            p->btx.image    = pictext.image;
            p->btx.onscreen = pictext.onscreen;
            
            if(DrawBackGround <= 0)
            {
              p->refcolback   = pictext.colback;
              p->refcolframe  = pictext.colframe;
              p->refcoltxt    = pictext.coltxt;
            }
            
            p->btx.oldcrc   = pictext.oldcrc;
            p->init = init; //_FALSE;
            retcode = _TRUE;
          }
        }
      }
    }
  }
  return(retcode);
}

_BOOL _SubPicture::Out_COL_POLY(_COL_POLY *p, _DOT *offset, _ZOOM *zoom, _BOOL init)
{
  _BOOL    retcode = _FALSE;
  _UDWORD  colhi, collo, frame;
  _POLYGON polygon;

  if(init == _TRUE)
    p->init = _TRUE;

  if(ColorScheme.Call(&colhi, &p->dyncolback, GetBackColor_Makro(p->poly.color), _TRUE) == _TRUE)
  {
    if(ColorScheme.Call(&collo, &p->dyncolfront, GetPenColor_Makro(p->poly.color), _TRUE) == _TRUE)
    {
//      if(ColorScheme.Call(&frame, &p->dyncolframe, GetBackColor_Makro(p->poly.colorframe), _TRUE) == _TRUE)
      if(ColorScheme.Call(&frame, &p->dyncolframe, p->poly.colorframe & 0xFFFF, _TRUE) == _TRUE) // SA53043
      {
        polygon            = p->poly;
        polygon.color      = MakeColor_Makro((_UWORD)colhi, (_UWORD)collo);
        polygon.colorframe = MakeColor_Makro((_UWORD)frame, (_UWORD)frame);

        if((p->init == _TRUE)||(polygon.colorframe != p->refcolorframe)||(polygon.color != p->refcolor))
        {
          out_POLYGON(&polygon, offset, zoom);
          p->refcolor      = polygon.color;
          p->refcolorframe = polygon.colorframe;
          p->init          = init; //_FALSE;
          retcode = _TRUE;
        }
      }
    }
  }

  return(retcode);
}

_KEY *seek_memkey(_MEM_KEY *p, _UWORD code, _BOOL GlobalHotkeys)
{
  _KEY *retcode = NULL;
  _UWORD i;

  if(code == 0)
    code = 0;

  for(i=0; i<p->no; i++)
  {
    if(p->ptr[i].code == code)
    {
      retcode = &p->ptr[i];
      break;
    }
  }

  if((retcode == NULL) && (GlobalHotkeys == _TRUE))
    retcode = Hotkey.FindKey(code);

  return(retcode);
}

_UWORD PublicStateschema(_MYSCHEME *pms)
{
  _UDWORD retcode;

  if(StateScheme.Call(&retcode, pms, NULL, STATE_ACTIVE) == _FALSE)
    retcode = STATESCHEME_PRESELECTION;

  return((_UWORD)retcode);
}

_BOOL _SubPicture::DoKeyboard(_EVENT *p, _Window *Window, _BOOL GlobalHotkeys)
{
  _BOOL retcode = _FALSE;
  _KEY  *k;

  if(p->selfmade == 0)
  {
    k = seek_memkey(&Picture.mask.memkey, p->scancode, GlobalHotkeys);
    if(k != NULL)
    {
      switch(p->ftype)
      {
        case _EVENT_KEYPRESS   :
                                #ifdef XUSER_KM
                                 if(AllSoftkeys.IsHotkey(p->scancode) == _TRUE)
                                 {
                                   _SINGLESOFT ss;
                                   init_SINGLESOFT(&ss);
                                   ss.access = 0;                   // key access
                                   ss.gfunct = k->gfunct;        // functionality
                                   ss.valid  = 1;             // is softkey valid
                                   AllSoftkeys.DoKey(&ss, 0xFFFF, Window, _TRUE, p->lrm);
                                   // init_SINGLESOFT(&ss); // nicht freigeben !!!
                                   retcode = _TRUE;
                                 }
                                 else
                                 {
                                   retcode = do_LSEFUNCT_lrm(&k->gfunct.pos_fl, Window, p->lrm);
                                 }
                                #else
                                 retcode = do_LSEFUNCT_lrm(&k->gfunct.pos_fl, Window, p->lrm);
                                #endif
                                 break;
        case _EVENT_KEYRELEASE : retcode = do_LSEFUNCT_lrm(&k->gfunct.neg_fl, Window, p->lrm);
                                 break;
        default                : break;
      }

      if(retcode == _TRUE)
        free_EVENT(p);
    }
  }


  if(retcode == _FALSE)
  {
    if(Picture.mask.memmotion.no > 0)
    {
      _UWORD i;
      _MOTION  *pm;
      
      for(i=0, pm=Picture.mask.memmotion.ptr; i<Picture.mask.memmotion.no; i++, pm++)
      {
        if(pm->memkaru.no > 0) // nur wenns a carousel ist
        {
          if(pm->usermove == _FALSE) // ansonsten "moved" user  
          {
            if(chk_CHKBIT(&pm->chkbit) == _TRUE)
            {
              if(PublicStateschema(&pm->stscheme) == STATE_ACTIVE)
              {
                action_CAROUSEL(pm, p);
              }
            }
          }
        }
      }
    }
  }

  return(retcode);
}

void _SubPicture::AddRegion_MOTION(_REGION referenz)
{
  _MOTION *m;
  _UWORD  i;

  for(i=0; i<Picture.mask.memmotion.no; i++)
  {
    m = &Picture.mask.memmotion.ptr[i];
    if(m->workstate == STATE_RUN)
    {
      if(RoomInRegion(&m->image.room, referenz) != 0)
      {
//        AddRegion(&m->image.room);
        m->workstate = STATE_BACKGROUND;
      }
    }
  }
}

_BOOL position_touch_inlet(_DOT *pd, _ZOOM *zoom, _ROOM *pr, _ATTRIB att, _SubPicture *Ps, _FRAMETYPE frametype)
{
  _ROOM  room1, room;
  _BOOL  retcode = _FALSE;
  _UWORD xx, yy;
  _DOT   tmpoffset;

  if(Ps != NULL)
  {
    Ps->GetSize(&room1);

    init_DOT(&tmpoffset);
    Zoom_ROOM(&room1, &tmpoffset, zoom);

    GetTextRoom(&room, pr, att, frametype, _FALSE);

    xx = room1.xy2.x - room1.xy1.x;
    yy = room1.xy2.y - room1.xy1.y;

    switch(att & T_HORIZBOUND)
    {
      case T_RIGHTBOUND  : pd->x = room.xy2.x - xx;                  break;
      case T_CENTERBOUND : pd->x = (room.xy1.x + room.xy2.x - xx)/2; break;
      default            : pd->x = room.xy1.x;                       break;
    }
    switch(att & T_VERTBOUND)
    {
      case T_DOWNBOUND   : pd->y = room.xy2.y - yy;                  break;
      case T_MIDBOUND    : pd->y = (room.xy1.y + room.xy2.y - yy)/2; break;
      default            : pd->y = room.xy1.y;                       break;
    }

    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL button_image(_BUTTON *p, _ROOM *pr, _UWORD image, _BOOL describe)
{
  _KOORD x, y;
  _BMP  *pb = ImageList.Get(image);
  _BOOL  retcode = _FALSE;
  _ROOM  room;
  _BOOL  zoom_state;

  if(pb != NULL)
  {
    retcode    = _TRUE;
    zoom_state = _FALSE;
    if(p->buttonimages.zoom_images == _TRUE)
    {
      if(describe == _TRUE)
      {
        if(p->buttonimages.image_release >= ImageList.GetNo())
        {
          zoom_state = _TRUE;
        }
      }
      else
      {
        zoom_state = _TRUE;
      }
    }

    GetTextRoom(&room, pr, p->attrib, p->frametype, _FALSE);

    if(zoom_state == _TRUE)
    {
      out_zoom_BMP(&room, pb, T_COPY|T_SOLID);
    }
    else
    {
      // sa8203 begin
      switch((p->wose >> 2) & 0x03)
      {
        case 0x01 : x = room.xy1.x; break;
        case 0x02 : x = room.xy2.x - pb->width + 1; break;
        default   : x = ((room.xy1.x + room.xy2.x + 1) - pb->width) / 2; break;
      }
      switch(p->wose & 0x03)
      {
        case 0x01 : y = room.xy1.y; break;
        case 0x02 : y = room.xy2.y - pb->height + 1; break;
        default   : y = ((room.xy1.y + room.xy2.y + 1) - pb->height) / 2; break;
      }
      // sa8203 end

      Bitmap(x, y, pb, T_COPY|T_SOLID, 0);
    }
  }

  return(retcode);
}

static unsigned long IsValidChr(_CHAR chr)
{
  if((chr == '[') || (chr == ']') || (chr == '{') || (chr == '}') || (chr == '@'))
    return 0;
    
//  if((chr >= 'A') && (chr <= 'Z')) return 1;
//  if((chr >= 'a') && (chr <= 'z')) return 1;
//  
//  if(chr == 196) return 1; // Ä
//  if(chr == 214) return 1; // Ö
//  if(chr == 220) return 1; // Ü
//  
//  
//  
//  
//  
////  if((chr == 'ö') || (chr == 'ü') || (chr == 'ä')) return 1;
  return 1;
}

_CHAR *GetButtonText(_CRC32 *crc, _PICTURE *pic, _BUTTON *p, _BOOL pressed)
{
  _CHAR *retcode;
  
  if(pressed == _TRUE)
  {
    retcode = p->usertext1.ptr;
    if(retcode == NULL)
      retcode = get_text(&p->text1.anything, &pic->TextPuffer, &p->text1.memvarlabel, _TRUE);
  }
  else
  {
    retcode = p->usertext.ptr;
    if(retcode == NULL)
      retcode = get_text(&p->text.anything, &pic->TextPuffer, &p->text.memvarlabel, _TRUE);
  }
  
  if(retcode != NULL)
  {
    if(p->keycode[0] > 255)
    {
      _DWORD diff = p->keycode[0] - p->keycode[1];
    
      if((diff == 32) || (diff == -32))
      {
        if(retcode[0] == p->keycode[1])
        {
          if(VarList.VarDummyGet(VARNO_SHIFTFLAG) == 0)
          {
            _CHAR *tmptxt = get_temptext();
            StrCpy(tmptxt, sizeof(_CHAR), retcode, sizeof(_CHAR));
            retcode = tmptxt;
            retcode[0] = retcode[0] + diff;
          }
        }
      }
    }
    else
    {
      if((p->keycode[1] ^ p->keycode[0]) == ('A' ^ 'a'))
      {
        if(retcode[0] == p->keycode[1])
        {
          if((IsValidChr(p->keycode[0]) != 0) && (IsValidChr(p->keycode[1]) != 0))
          {
            if(VarList.VarDummyGet(VARNO_SHIFTFLAG) == 0)
            {
              _CHAR *tmptxt = get_temptext();
              StrCpy(tmptxt, sizeof(_CHAR), retcode, sizeof(_CHAR));
              retcode = tmptxt;
              retcode[0] = retcode[0] ^ ('A' ^ 'a');
            }
          }
        }
      }
    }
  }
  
  if(crc != NULL)
  {
    _CRC32 tmp = 0;
    if(p->keycode[2] != _NOKEY)
    {
      if(VarList.VarDummyGet(VARNO_ALTGRFLAG) != 0)
        tmp = 0xFFFFFFFF;
    }
  
    if(retcode != NULL)
      *crc = (StrCrc(retcode, sizeof(_CHAR)) ^ tmp);
    else
      *crc = tmp;
  }
  
  return retcode;
}

_BOOL draw_BUTTON(_PICTURE *pic, _BUTTON *p, _ROOM *room, _ZOOM *zoom, _BOOL pressed, _BOOL selected, _ATTRIB att, _COLOR col, _COLOR frame, _COLOR coltxt)
{
  _DOT    dot;
  _CHAR   *txt;
  _UDWORD tmp;
  _BOOL   retcode = _FALSE;

  if(pressed == _TRUE)
  {
    att ^= T_INVRS;
    if(GetBackColor_Makro(col) == INVISIBLE)
      col = MakeColor_Makro(GetPenColor_Makro(col), GetPenColor_Makro(col));
    frame = InvertColor_Makro(frame);
  }


  // button
 #ifdef UC_THEME
  if(p->frametype >= _THEMEFRAME)
  {
   #ifndef USER_KM
    if(selected == _TRUE)
    {
      // editorfarbe holen
      col    = SetUp.GetEditColorBack(_EDITACTIVE, col);
      col    = MakeColor_Makro(col, col); //GetPenColor_Makro();
      coltxt = SetUp.GetEditColorTxt(_EDITACTIVE, coltxt);
    }
   #endif
    draw_THEME(room, p->frametype, att, col, pressed, _FALSE);
  }
  else
 #endif
  {
    DrawButton(room, att, p->frametype, col, frame);
  }

  _COLOR altgrcol = col;

  if(selected == _TRUE)
  {
    retcode = button_image(p, room, (pressed == _TRUE)? p->buttonimages.image_sel_press : p->buttonimages.image_sel_release, _FALSE);
  }
  else
  {
    if(StateScheme.Call(&tmp, &p->stscheme, NULL, STATE_ACTIVE) == _FALSE)
      tmp = STATESCHEME_PRESELECTION;

#ifdef SA35154
    if(tmp == STATE_ACTIVE)
    {
      if(chk_ACCESS(p->access, _FALSE) == _FALSE)
      {
        tmp = STATE_INACTIVE;
      }
    }
#endif


    if(tmp == STATE_INACTIVE)
      button_image(p, room, (pressed == _TRUE)? p->buttonimages.image_in_press : p->buttonimages.image_in_release, _FALSE);
    else
      button_image(p, room, (pressed == _TRUE)? p->buttonimages.image_press : p->buttonimages.image_release, _FALSE);
  }

  // describing image
  button_image(p, room, p->buttonimages.image_describe, _TRUE);


  // text
  _UDWORD slurry = p->reftxtcrc;
  txt = GetButtonText(&p->reftxtcrc, pic, p, pressed);
  if(DrawBackGround > 0)
    p->reftxtcrc = slurry;
  
//  if(pressed == _TRUE)
//    txt = get_text(&p->text1.anything, &pic->TextPuffer, &p->text1.memvarlabel, _TRUE);
//  else
//    txt = get_text(&p->text.anything, &pic->TextPuffer, &p->text.memvarlabel, _TRUE);

  if(txt != NULL)
  {
    _ROOM inlet;
    GetTextRoom(&inlet, room, p->attrib, p->frametype, _FALSE);
    
    if((p->keycode[2] != _NOKEY) || (p->keycode[3] != _NOKEY))
    {
      if(pressed == _TRUE)
        altgrcol = GetBackColor(altgrcol);
    
      _UDWORD r = ((altgrcol & 0xF800) + (coltxt & 0xF800)) / 2;
      _UDWORD g = ((altgrcol & 0x07E0) + (coltxt & 0x07E0)) / 2;
      _UDWORD b = ((altgrcol & 0x001F) + (coltxt & 0x001F)) / 2;
      _COLOR tct = (r & 0xF800) | (g & 0x07E0) | (b & 0x001F);
      
      _CHAR ttx[4];
      _CHAR *ph = ttx;
      if(p->keycode[3] != _NOKEY)
      {
        *ph = p->keycode[3];
        ph += 1;
      }
      
      if(p->keycode[2] != _NOKEY)
      {
        *ph = '|';
        ph += 1;
        *ph = (_CHAR)p->keycode[2];
        ph += 1;
      }
      
      *ph = 0;

      
//      ttx[0] = (_CHAR)p->keycode[2];
//      ttx[1] = 0;
     #ifdef UC_GRAYSCALE
      if(VarList.VarDummyGet(VARNO_ALTGRFLAG) != 0)
      {
        _UWORD cvo = LIGHTCYAN;
        transform_color_to_gray(&cvo);
        coltxt = tct;
        tct = MakeColor(INVISIBLE, cvo);
      }
     #else
      switch(VarList.VarDummyGet(VARNO_ALTGRFLAG))
      {
        case 1:
          {
            coltxt = tct;
            tct = MakeColor(INVISIBLE, _WIN_EDITOR_ALTGR_COLOR); // LIGHTCYAN
          }
          break;
          
        case 2:
          {
            _COLOR memo = tct;
            tct    = coltxt; 
            coltxt = memo;
          }
          break;
      }
      
     #endif
      _ROOM  tin = inlet;
      tin.xy2.x = room->xy2.x;
      if(Language.GetActualLocal() == LOCAL_JAPANESE)
      {
        tin.xy2.y += 3;
      }
      OutMultiTextAlign(ttx, 0, &tin, p->font, T_COPY | T_SOLID | T_RIGHTBOUND | T_DOWNBOUND, _DEFFRAME, tct, MakeColor_Makro(INVISIBLE, INVISIBLE), MakeColor_Makro(INVISIBLE, INVISIBLE), sizeof(_CHAR));
    }
    
   #ifdef UC_THEME
    if((p->frametype & 0x7F) >= _THEMEFRAME) // wegen kompatibilität zu früher
    {
      _REGION actreg, ioreg=NULL, newreg=NULL;
      actreg = ActivateAndRegion(&ioreg, &newreg, room);
      OutMultiTextAlign(txt, 0, &inlet, p->font, p->attrib, _DEFFRAME_NOCAREREGION, coltxt, MakeColor_Makro(INVISIBLE, INVISIBLE), MakeColor_Makro(INVISIBLE, INVISIBLE), sizeof(_CHAR));
      DeactivateAndRegion(actreg, &ioreg, &newreg);
    }
    else
   #endif
    {
      OutMultiTextAlign(txt, 0, &inlet, p->font, p->attrib, _DEFFRAME, coltxt, MakeColor_Makro(INVISIBLE, INVISIBLE), MakeColor_Makro(INVISIBLE, INVISIBLE), sizeof(_CHAR));
    }
  }

  // object
  if(p->anything.list == OBJLST)
  {
    if(position_touch_inlet(&dot, zoom, room, att, p->SubPicture, p->frametype) == _TRUE)
    {
      if(DrawBackGround == 0)
        p->SubPicture->KillBackGroundImage();
      p->SubPicture->Draw(&dot, zoom, NULL);
    }
  }

  return(retcode);
}

#ifdef USER_HIRSCH
void ButtonColorHirsch(_COLOR *collo, _COLOR *colhi, _BUTTON *p)
{
  if(p->state == _FALSE)
    *collo = AccessColor(p->access, &p->dyncolfront, *collo, _FALSE); // vorsicht: immer linker error weil funktion vom kunden fehlt
  else  
    *colhi = AccessColor(p->access, &p->dyncolback, *colhi, _FALSE); // vorsicht: immer linker error weil funktion vom kunden fehlt
}
#endif

_BOOL get_stationary_state(_BOOL *pst, _VARLABEL *pvl, _DWORD setval, _DWORD resetval)
{
  _IVALUE value; 
  
  if(chk_VARLABEL_Makro(pvl))
  {
   #ifdef UC_MEMORIZE_PLCDATA
    if(VarList.GetValue(pvl, &value, _FALSE, _TRUE) == _TRUE)
   #else
    if(VarList.GetValue(pvl, &value, _TRUE, _FALSE) == _TRUE)
   #endif
    {
      if((setval == 1) && (resetval == 0))
      { 
        // sa21860
        *pst = (value == 0)? _FALSE : _TRUE;
      }
      else
      {
        *pst = (value == setval)? _TRUE : _FALSE;
      }
      return _TRUE;
    }
  }
  
  return _FALSE;
}

_BOOL _SubPicture::Out_BUTTON(_BUTTON *p, _DOT *offset, _ZOOM *zoom, _ATTRIB gatt)
{
  _BOOL   retcode;
  _COLOR  frame, col;
  _COLOR  colhi, collo, framehi, framelo, coltxt;
  _ATTRIB att;
  _ROOM   room, ri;
  _BOOL   onoff = p->state;
  _BUTTON tmp;

  if(DrawBackGround != 0)
    tmp = *p;

  room = p->room;
  Zoom_ROOM(&room, offset, zoom);

  ColorScheme.Call(&colhi,   &p->dyncolback,  GetBackColor_Makro(p->col), _TRUE);
  ColorScheme.Call(&collo,   &p->dyncolfront, GetPenColor_Makro(p->col), _TRUE);
  ColorScheme.Call(&framehi, &p->dyncoltl,    GetBackColor_Makro(p->framecol), _TRUE);
  ColorScheme.Call(&framelo, &p->dyncolbr,    GetPenColor_Makro(p->framecol), _TRUE);
  ColorScheme.Call(&coltxt,  &p->dyncoltxt,   GetPenColor_Makro(p->coltxt), _TRUE);

 #ifdef USER_HIRSCH
  ButtonColorHirsch(&collo, &colhi, p);
 #endif 

  if(p->image.ptr == NULL)
    GetImage(&p->image, room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y);

  att = (gatt != DEFATTRIB)? gatt : p->attrib;

//  col   = p->col;
//  frame = p->framecol;

  col   = MakeColor_Makro(colhi, collo);
  frame = MakeColor_Makro(framehi, framelo);

  p->refcolor      = col;
  p->refcolorframe = frame;
  p->refcolortxt   = coltxt;

  if(onoff == _FALSE)
  {
    if(p->act_momentary == false)
      get_stationary_state(&onoff, &p->varlabel, p->set_val, p->reset_val);
  }

 #ifdef UC_MOUSE
  HideMouseCursor();
 #endif

  draw_BUTTON(&Picture, p, &room, zoom, onoff, p->selected, att, col, frame, coltxt);
  retcode = onoff;

 #ifdef OK_SEEK_BUTTON
  if((p->selected == _TRUE)&&(is_button_cursor(p) == _TRUE))
  {
    col = SetUp.GetEditColorBack(_EDITACTIVE, p->col);
    GetTextRoom(&ri, &room, p->attrib, p->frametype, _FALSE);
    draw_button_cursor(ri, col, T_COPY|T_SOLID|T_INVRS);
  }
 #endif

 #ifdef UC_ROTATE
  RefreshRectangle(&room);
 #endif

 #ifdef UC_MOUSE
  ShowMouseCursor();
 #endif

  if(DrawBackGround != 0)
  {
    p->refcolor      = tmp.refcolor;
    p->refcolorframe = tmp.refcolorframe;
    p->refcolortxt   = tmp.refcolortxt;
  }

  return(retcode);
}

_BOOL _SubPicture::Refresh_BUTTON(_BUTTON *p, _DOT *offset, _ZOOM *zoom, _BOOL init, _Picture *Ppic)
{
  // retcode wird für refresh verwendet und nicht für erfolg ob objekt ausgegeben
  // werden konnte.
  // retcode = _TRUE wenn refresh fertig oder objekt nit vorhanden, geladen etc.
  _ROOM   room;
  _BOOL   retcode = _TRUE;
  _BOOL   state = _FALSE;
  _BOOL   redraw = _FALSE;
  _DOT    o;
  _COLOR  collo, colhi, framelo, framehi, coltxt;
  _UDWORD tmp1, tmp;
//  _IVALUE value;
  _BOOL   st;

 #ifdef NINETEEN
  _BOOL full_access_given = _FALSE;
 #endif

  if(chk_CHKBIT(&p->chkbit) == _TRUE)
  {
    if(StateScheme.Call(&tmp, &p->stscheme, NULL, STATE_ACTIVE) == _FALSE)
      tmp = STATESCHEME_PRESELECTION;

   #ifdef NINETEEN
    if(tmp == STATE_ACTIVE)
      full_access_given = access_button(p, _TRUE, _SELECTOR_ONLY_HID);
   #endif

    if(is_server_defined(&p->varlabel) == _TRUE)
    {
      tmp1 = VarList.GetStateScheme(p->varlabel.info[0].value, NOCARE_DEFAULT);
      if(tmp1 > tmp)
        tmp = tmp1;
    }

   #ifdef SA35154
    if(tmp == STATE_ACTIVE)
    {
      if(chk_ACCESS(p->access, _FALSE) == _FALSE)
      {
        tmp = STATE_INACTIVE;
      }
    }
   #endif
   
    if((_UWORD)tmp != p->oldstatescheme)
    {
      redraw = _TRUE;
      if(DrawBackGround == 0)
        p->oldstatescheme = (_UWORD)tmp;
    }

    if(tmp != STATE_INVISIBLE)
    {
      state = _TRUE;


      if(p->workstate == STATE_START)
      {
        p->workstate = STATE_READY;
        if(p->anything.list == OBJLST)
        {
          if(load_object(&p->SubPicture, p->anything.no) == _TRUE)
          {
           #ifdef UC_CHANGE_IO_GROUP
            change_SubPicture_group(p->SubPicture, p->imiogroupchanged.summary, p->imiogroupchanged.serverno);
           #endif
            p->SubPicture->Overload(&p->loader);
            CallUserOverload(p->SubPicture, p->summary);
          }
        }
      }

      if((p->workstate != STATE_ERROR) && (init == _TRUE))
        p->workstate = STATE_READY;

      if(p->workstate == STATE_READY)
      {
        st = Out_BUTTON(p, offset, zoom, DEFATTRIB);
        if((init == _TRUE) && (DrawBackGround == 0))
          p->oldstate = st; // beim zeichnen muss oldstate aktualisiert werden !!
        p->workstate = STATE_RUN;
      }
      else if(p->workstate == STATE_RUN)
      {
       
        ColorScheme.Call(&colhi,   &p->dyncolback,  GetBackColor_Makro(p->col), _TRUE);
        ColorScheme.Call(&collo,   &p->dyncolfront, GetPenColor_Makro(p->col), _TRUE);
        ColorScheme.Call(&framehi, &p->dyncoltl,    GetBackColor_Makro(p->framecol), _TRUE);
        ColorScheme.Call(&framelo, &p->dyncolbr,    GetPenColor_Makro(p->framecol), _TRUE);
        ColorScheme.Call(&coltxt,  &p->dyncoltxt,   GetPenColor_Makro(p->coltxt), _TRUE);

        if(p->refcolortxt != coltxt)
          redraw = _TRUE;

       #ifndef USER_HIRSCH
        if((p->refcolor != MakeColor_Makro(colhi, collo))||(p->refcolorframe != MakeColor_Makro(framehi, framelo)))
          redraw = _TRUE;
       #endif
        
  //      if(p->act_momentary == false) // auskommentiert aufgrund wunsch appli: auch ein momentary button soll den state des servers anzeigen, egal ob gedrückt oder nicht
        if(p->fingercount == 0)
        {
          get_stationary_state(&p->state, &p->varlabel, p->set_val, p->reset_val);
        }
        
        if(p->state != p->oldstate)
          redraw = _TRUE;

       #ifdef USER_HIRSCH
        ButtonColorHirsch(&collo, &colhi, p);
        if((p->refcolor != MakeColor_Makro(colhi, collo))||(p->refcolorframe != MakeColor_Makro(framehi, framelo)))
          redraw = _TRUE;
       #endif

        if(redraw == _FALSE)
        {
          _CRC32 tmpcrc;
          GetButtonText(&tmpcrc, GetPICTURE(), p, p->state);
          if(tmpcrc != p->reftxtcrc) // crc von buttontext vergleichen
          {
            redraw = _TRUE;
          }
        }

        if(redraw == _TRUE)
        {
          RefreshTouchButton(p, redraw, offset, zoom);
        }
        else
        {
          if(p->anything.list == OBJLST)
          {
            room = p->room;
            Zoom_ROOM(&room, offset, zoom);
            if(position_touch_inlet(&o, zoom, &room, p->attrib, p->SubPicture, p->frametype) == _TRUE)
            {
              // letzter parameter auf NULL weil in einem button keine eingabe sein kann!!
              retcode = p->SubPicture->Refresh(0xFFFF, &o, zoom, NULL);
            }
          }
        }
//      _IVALUE value;
//      if(VarList.GetValue(&p->varlabel, &value, _FALSE) == _TRUE)
//        Out_BUTTON(p, offset, zoom, DEFATTRIB);
      }
    }
  }

  if(state == _FALSE) // perhaps we've to extinguish button from screen
  {
    if(p->image.ptr != NULL)
    {
      if((p->selected == _TRUE) && (Ppic != NULL))
      {
        Ppic->Cursor.End();
      }
      
      PutImage(&p->image, DEFKOORD, DEFKOORD);
     #ifdef UC_ROTATE
      RefreshRectangle(&p->image.room);
     #endif
      if(DrawBackGround == 0)
        free_IMAGE(&p->image);
      if(p->workstate == STATE_RUN)
        p->workstate = STATE_READY;
    }
    // unsichtbaraer button kann nicht gedrückt oder selektiert sein
    p->state = _FALSE;
    p->oldstate = _FALSE;
    p->selected = _FALSE;
    p->fingercount = 0;
  }

 #ifdef NINETEEN
  if(p->full_access_given != full_access_given)
    p->full_access_given = full_access_given;
 #endif

// todo: testroutine, wieder auskommentieren
//Bar(p->room.xy1.x, p->room.xy1.y, p->room.xy1.x + 10, p->room.xy1.y + 10, T_COPY|T_SOLID, (p->fingercount == 0)? WHITE:LIGHTRED);
//RefreshRectangle(&p->room);

  return(retcode);
}

_UWORD statescheme_OBJECT(_OBJECT *p)
{
  _UDWORD retcode;

  if(StateScheme.Call(&retcode, &p->stscheme, NULL, STATE_ACTIVE) == _FALSE)
    retcode = STATESCHEME_PRESELECTION;

  return((_UWORD)retcode);
}

void _SubPicture::SetButtonState(_BOOL state)
{
  set_buttonstate(&Picture.mask, state);
}

_BOOL watch_loaderscheme(_OBJECT *p)
{
  _VARLABEL vl;
  _UDWORD   serverno;

  if(ServerScheme.Call(&serverno, &p->loader_scheme, NULL, DEFSCOPE) == _TRUE)
  {
    if(serverno != DEFSCOPE)
    {
      init_VARLABEL_Makro(&vl);
      vl.no = 1;
      vl.info[0].state = VAR_VAL;
      vl.info[0].value = serverno;

      if(valid_VARLABEL(&vl) == _TRUE)
      {
        if(p->loader.info[0].value != vl.info[0].value)
          p->workstate = STATE_OVERLOAD;    // objekt muss neu überladen werden
        p->loader = vl;
        return _TRUE;
      }
    }
  }

  return _FALSE;
}

_BOOL _SubPicture::Refresh_OBJECT(_UWORD actno, _OBJECT *p, _DOT *offset, _ZOOM *zoom, _BOOL init, _Picture *Ppic)
{
  // retcode wird für refresh verwendet und nicht für erfolg ob objekt ausgegeben
  // werden konnte.
  // retcode = _TRUE wenn refresh fertig oder objekt nit vorhanden, geladen etc.
  _DOT   dot;
  _BOOL  retcode = _TRUE;
  _ROOM  room;
  _ZOOM  tmpzoom;
  _BOOL  status = _FALSE;


  if(chk_CHKBIT(&p->chkbit) == _TRUE)
  {
    if(p->workstate == STATE_START)
    {
      if(load_object(&p->SubPicture, p->no) == _TRUE)
      {
        p->workstate = STATE_READY;
       #ifdef _NO_GETIMAGE
        p->doimage   = is_MYSCHEME(&p->stscheme, &StateScheme); // nicht böse
       #endif
       #ifdef UC_CHANGE_IO_GROUP
        change_SubPicture_group(p->SubPicture, p->imiogroupchanged.summary, p->imiogroupchanged.serverno);
       #endif
        if(chk_VARLABEL_Makro(&p->loader)) // Makro, speed: nicht auf true oder false abfragen
          p->workstate = STATE_OVERLOAD;
      }
      else
      {
        p->workstate = STATE_ERROR;
      }
    }

    if(p->workstate != STATE_ERROR)
    {
      if(p->SubPicture != NULL)
      {
        dot.x = 0;
        dot.y = 0;
        tmpzoom   = p->zoom;
        tmpzoom.x = Zoom_ZOOM(zoom->x, tmpzoom.x);
        tmpzoom.y = Zoom_ZOOM(zoom->y, tmpzoom.y);
        room = p->SubPicture->GetPICTURE()->mask.room;
        Zoom_ROOM(&room, &dot, &tmpzoom);
        dot.x = Zoom(zoom->x, p->xy.x) + offset->x;
        dot.y = Zoom(zoom->y, p->xy.y) + offset->y;
        room.xy1.x += dot.x;
        room.xy1.y += dot.y;
        room.xy2.x += dot.x;
        room.xy2.y += dot.y;

        if((p->image.ptr == NULL)&&(DrawBackGround == 0)&&(p->doimage == _TRUE))
        {
          GetImage2(&p->image, room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y);
        }
/*

        _KOORD width, height;
        dot.x     = Zoom(zoom->x, p->xy.x) + offset->x;
        dot.y     = Zoom(zoom->y, p->xy.y) + offset->y;
        tmpzoom   = p->zoom;
        tmpzoom.x = Zoom_ZOOM(zoom->x, tmpzoom.x);
        tmpzoom.y = Zoom_ZOOM(zoom->y, tmpzoom.y);

        if((p->image.ptr == NULL)&&(DrawBackGround == 0)&&(p->doimage == _TRUE))
        {
          room   = p->SubPicture->GetPICTURE()->mask.room;
          width  = Zoom(p->zoom.x, room.xy2.x - room.xy1.x + 1);
          height = Zoom(p->zoom.y, room.xy2.y - room.xy1.y + 1);
          GetImage(&p->image, dot.x, dot.y, dot.x+width - 1, dot.y+height - 1);
        }
*/
        if(statescheme_OBJECT(p) != STATE_INVISIBLE)
        {
          watch_loaderscheme(p);

          if(p->workstate == STATE_OVERLOAD)
          {
            p->workstate = STATE_READY;
            if(p->lock_overload == _FALSE)
            {
              p->SubPicture->Overload(&p->loader);
              CallUserOverload(p->SubPicture, p->summary);
            }
          }

          if((p->workstate == STATE_READY)||(init == _TRUE)||(DrawBackGround != 0))
          {
            if((p->image.ptr != NULL)||(p->doimage == _FALSE))
            {
              p->SubPicture->Draw(&dot, &tmpzoom, Ppic);
              if(DrawBackGround == 0)
                p->workstate = STATE_RUN;
              status       = _TRUE;
              p->onscreen  = _TRUE;
            }
          }
          else if(p->workstate == STATE_RUN)
          {
            retcode = p->SubPicture->Refresh(actno, &dot, &tmpzoom, Ppic);
            status  = _TRUE;
          }
/*
         #ifdef UC_ROTATE
          RefreshRectangle(&p->image.room);
         #endif
*/
        }
      }
    }
	}

  if(status == _FALSE) // we've to extinguish object from screen
  {
    if((p->image.ptr != NULL)&&(p->onscreen == _TRUE))  // delete object from screen
    {
     #ifdef UC_MOUSE
      HideMouseCursor();
     #endif
      PutImage2(&p->image, DEFKOORD, DEFKOORD);
     #ifdef UC_MOUSE
      ShowMouseCursor();
     #endif

     #ifdef UC_ROTATE
      _ROOM rm = p->image.room;
      rm.xy2.x += 1;
      rm.xy2.y += 1;
      RefreshRectangle(&rm);
//      RefreshRectangle(&p->image.room);
     #endif

    }

    if(DrawBackGround == 0)
    {
//      free_IMAGE(&p->image);
      p->onscreen = _FALSE;
      if(p->SubPicture != NULL)
      {
        p->SubPicture->SetButtonState(_FALSE);
        p->SubPicture->KillBackGroundImage();
      }

      // in diesem fall soll objekt überladen werden, konnte jedoch aufgrund von
      // stateschema nicht dargestellt werden. workstate STATE_OVERLOAD muss beibehalten werden.
      if(p->workstate != STATE_OVERLOAD)
        p->workstate = STATE_READY;
    }
  }

	return(retcode);
}

_BOOL _SubPicture::ChkChkBit(void)
{
  return chk_CHKBIT(&Picture.chkbit);  
}

_BOOL _SubPicture::Copy(_SubPicture *Pt)
{
	// vorsicht: diese Funktion setzt den aufruf von _SubPicture::Free voraus

	_BOOL retcode = _FALSE;

	// Free(); // böse 

	if(Pt != NULL)
	{
		retcode = _TRUE;
   #ifdef SA34959 
    InitMoveButtonGroup(); // alle move-button-merker initialisieren
   #endif
		if(copy_PICTURE(&Picture, Pt->GetPICTURE()) == _FALSE)
		{
			Free();
			retcode = _FALSE;
		}
	}

	return(retcode);
}

void _SubPicture::GetSize(_ROOM *r)
{
	*r = Picture.mask.room;
}

_PICTURE* _SubPicture::GetPICTURE(void)
{
	return(&Picture);
}

_BOOL _SubPicture::RefreshTouchButton(_BUTTON *pt, _BOOL state, _DOT *offset, _ZOOM *zoom)
{
  _BOOL retcode = _FALSE;

  if(((pt->state != pt->oldstate)||(state == _TRUE)) && (pt->workstate == STATE_RUN))
  {
    if(pt->SubPicture == NULL)
    {
      if(pt->anything.list == OBJLST)
      {
        if(load_object(&pt->SubPicture, pt->anything.no) == _TRUE)
        {
         #ifdef UC_CHANGE_IO_GROUP
          change_SubPicture_group(pt->SubPicture, pt->imiogroupchanged.summary, pt->imiogroupchanged.serverno);
         #endif
          pt->SubPicture->Overload(&pt->loader);
          CallUserOverload(pt->SubPicture, pt->summary);
        }
      }
    }

    Out_BUTTON(pt, offset, zoom, DEFATTRIB);

    pt->oldstate = pt->state;
    retcode      = _TRUE;
  }

  return(retcode);
}

_BOOL set_buttonval(_VARLABEL *pvl, _DWORD val, _BOOL lrm)
{
  if(val == ACCESS_DENIED) 
    return _TRUE; // dr1822
    
  _RESULT   result;
  _VARIABLE variable;
  
  init_RESULT_Makro(&result);
  result.value = val;
  result.rmin  = result.value;
  result.rmax  = result.value;
  result.ftype = RES_CONSTANT;
  set_lrm(&result, lrm);
  return VarList.SetSystemData(&result, &variable, pvl, _TRUE);
}

_BOOL button_release(_VARLABEL *pvl, _BOOL act_momentary, _DWORD setval, _DWORD resetval, _BOOL lrm)
{
  _BOOL retcode = _FALSE; // return state of button (true = pressed; false = released)

  if(act_momentary == _TRUE)
  {
    set_buttonval(pvl, resetval, lrm);
  }
  else 
  {
    get_stationary_state(&retcode, pvl, setval, resetval);
  }
  return retcode;
}

_BOOL button_press(_VARLABEL *pvl, _BOOL act_momentary, _DWORD setval, _DWORD resetval, _BOOL lrm)
{
  _BOOL retcode = _TRUE; // return state of button (true = pressed; false = released)

  if(act_momentary == _TRUE)
  {
    set_buttonval(pvl, setval, lrm);
  }
  else 
  {
    if(get_stationary_state(&retcode, pvl, setval, resetval) == _TRUE)
    {
      set_buttonval(pvl, (retcode == _TRUE)? resetval : setval,lrm);
      get_stationary_state(&retcode, pvl, setval, resetval);
    }
  }
  
  return retcode;
}

void _SubPicture::DoButton(_BUTTON *pt, _BOOL press_state, _Window *Window, _DOT *offset, _ZOOM *zoom, _BOOL lrm, _BOOL isrepeat)
{
 #ifdef USER_KM
 #ifdef NINETEEN
  if((pt->hotkeycode != 0)&&(pt->hotkeycode != 0xFFFF))
  {
    _ROOM xr = pt->room;
    Zoom_ROOM(&xr, offset, zoom);
    if(xr.xy1.y < 768)
    {
      _EVENT ev;
      init_EVENT_Makro(&ev);
      ev.scancode = pt->hotkeycode;
      ev.ftype = (press_state == _TRUE)? _EVENT_KEYPRESS : _EVENT_KEYRELEASE;
      ev.user[0] = 0xA5A5;
      ::Keyboard.AddEvent(&ev);
    }
  }
 #endif
 #endif

  if(press_state == _TRUE)
  {
   #ifdef UC_BUBBLE_HELP
    if(GetLseServer(LSE_ACTIVATEBUBBLEHELP))
    {
      _ROOM xroom = pt->room;
      Zoom_ROOM(&xroom, offset, zoom);
      NewBubbleHelp.Call(xroom.xy1.x, xroom.xy1.y, &pt->newbubbletext, &pt->varlabel);
      return;
    }        
   #endif
   
    if(pt->oldstate == _FALSE)
      Beep(BEEP_BUTTON);

    pt->state = _TRUE;   // button pressed
   #ifdef SA35472
    if((isrepeat == _FALSE) || (pt->act_momentary == _TRUE))
   #else
    if(isrepeat == _FALSE)
   #endif
    {
      pt->state = button_press(&pt->varlabel, pt->act_momentary, pt->set_val, pt->reset_val, lrm);
    }
    
    RefreshTouchButton(pt, _TRUE, offset, zoom);
    
    if(pt->keycode[0] != _NOKEY)
    {
      _UWORD keco = (VarList.VarDummyGet(VARNO_SHIFTFLAG) == 0)? pt->keycode[0] : pt->keycode[1];
      if(pt->keycode[2] != _NOKEY)
      {
        _DWORD grval = VarList.VarDummyGet(VARNO_ALTGRFLAG);
        if(grval != 0)
        {
          if((pt->keycode[3] != _NOKEY) && (VarList.VarDummyGet(VARNO_SHIFTFLAG) != 0))
          {
            keco = pt->keycode[3];
          }
          else
          {
            keco = pt->keycode[2];
          }
          
          if(grval == 1) // nur wenn nicht VARNO_ALTGRFLAG_LOCK (grval ist dann 2)
          {
            VarList.VarDummySet(VARNO_ALTGRFLAG, 0); // reset flag
          }
        }
      }
      
      if(keco != _NOKEY)
      {
        if(keco == _ENTER)
        {
          _WHOAMI wai = Root.FindTop()->GetWhoAmI();
          if(wai.ima == IMA_TOUCHEDITOR)
          {
//           #ifdef UC_JAPANINPUT
//            if(Japan_InputMode() == 0)
//           #endif
//            {
           #ifdef SA32402
            #ifdef UC_DYN_EDIT_COLOR
             #if(UC_DYN_EDIT_COLOR != INVISIBLE)
              _BOOL est = _TRUE;
              if(Window != NULL)
              {
                _Picture *Ppic = Window->GetP_Picture();
                if(Ppic != NULL)
                {
                  est = Ppic->EditorCheckLimits();
                }
              }
            
              if(est == _TRUE)
             #endif  
            #endif // UC_DYN_EDIT_COLOR
           #endif // SA32402 
              {
                InsertCmd(CMD_SAVEEDITOR, 0, 0);
              
                // sa27246
                // zusätzlich sollte bei diesem kommando eine beliebige crc inkrementiert werden
                // diese crc wird also jedesmal wenn eine eingabe gespeichert wird erhöht
                // Vorsicht: muss aber mittels InsertCmd() angestossen werden, 
                // ansonsten wäre crc bereits vor dem eigentlichen speichern der Änderung incrementiert
                InsertCmd(CMD_ACKNOWLEDGEFLAG, 2, 0);
            
                InsertCmd(CMD_RETURN, 0, 0);
              }
              keco = _NOKEY;
//            }
          }
        }
        else if(keco == _ESC)
        {
          _WHOAMI wai = Root.FindTop()->GetWhoAmI();
          if(wai.ima == IMA_TOUCHEDITOR)
          {
            InsertCmd(CMD_RETURN, 0, 0);
            keco = _NOKEY;
          }
        }
        
        if(keco != _NOKEY)
        {
          Keyboard.AddEvent(_EVENT_KEYPRESS, keco);
          Keyboard.AddEvent(_EVENT_KEYRELEASE, keco);
        }
      }
    }
    
    do_LSEFUNCT_lrm(&pt->gfunct.pos_fl, Window, lrm);
  }
  else
  {
    //if(is_server_defined(&pt->varlabel) == _FALSE)
    pt->state = _FALSE;    // button released
    if(isrepeat == _FALSE)
      pt->state = button_release(&pt->varlabel, pt->act_momentary, pt->set_val, pt->reset_val, lrm);  
    RefreshTouchButton(pt, _TRUE, offset, zoom);
    do_LSEFUNCT_lrm(&pt->gfunct.neg_fl, Window, lrm);
  }
}

#ifdef HID

_BOOL do_doubleclick_function(_BUTTON *pt)
{
  _BOOL retcode = _TRUE;

  if(pt->doubleclick == _TRUE)
  {
    retcode = _FALSE;
    if((pt->selected == _TRUE)||(SetUp.GetDoubleClickOption() == _TRUE))
      retcode = _TRUE;
  }
  return(retcode);
}

#ifdef SA34959

void _SubPicture::InitMoveButtonGroup(void)
{
//  for(_UDWORD i=0; i<10; i++)
//  {
//    Picture.MoveButtonGroup[i] = 0;
//  }
  
  Picture.MoveButtonGroup[0] = 0;
  Picture.MoveButtonGroup[1] = 0;
  Picture.MoveButtonGroup[2] = 0;
  Picture.MoveButtonGroup[3] = 0;
  Picture.MoveButtonGroup[4] = 0;
  Picture.MoveButtonGroup[5] = 0;
  Picture.MoveButtonGroup[6] = 0;
  Picture.MoveButtonGroup[7] = 0;
  Picture.MoveButtonGroup[8] = 0;
  Picture.MoveButtonGroup[9] = 0;
}

_BOOL _SubPicture::SetMoveButtonGroup(_UDWORD multindex, _UDWORD summary)
{
  if(multindex < 10) //MAX_MULTIFINGER)
  {
    Picture.MoveButtonGroup[multindex] = summary;
    return _TRUE;
  }
  
  return _FALSE;
}

_UDWORD _SubPicture::GetMoveButtonGroup(_UDWORD multindex)
{
  if(multindex < 10) //MAX_MULTIFINGER)
  {
    return Picture.MoveButtonGroup[multindex];
  }
  return 0;
}

void _SubPicture::ReleaseMoveButtonGroup(_UDWORD multindex)
{
  _UDWORD mbg = GetMoveButtonGroup(multindex);
  
  if(mbg != 0)
  {
    SetMoveButtonGroup(multindex, 0);
    
    _BUTTON *pt = Picture.mask.membutton.ptr;
    _UDWORD nox = Picture.mask.membutton.no;
    while(nox--)
    {
      if(pt->summary == mbg)
      {
        DoNegEdge(pt); // button loslassen wenn er zum loslassen ist
      }
    
      pt++;
    }
  }
}
#endif // SA34959

_BOOL _SubPicture::DoHID(_EVENT *pe, _Window *Window, _DOT offset, _ZOOM *zoom)
{
  _BUTTON  *pt;
  _UWORD   i;
  _BOOL    retcode = _FALSE;
  _ROOM    room;
  _ZOOM    tmpzoom;
  _IO      *pio;
  _OBJECT  *po;
  _MOTION  *pm;
  _DOT     rekoff;

  if(pe->ftype == _EVENT_HIDPRESS)
  {
    for(i=0; i<Picture.mask.membutton.no; i++)
    {
      pt = &Picture.mask.membutton.ptr[i];
      if(pt->workstate == STATE_RUN)
      {
        room = pt->room;
        Zoom_ROOM(&room, &offset, zoom);

        if((pe->dot.x >= room.xy1.x) && (pe->dot.x <= room.xy2.x) &&
           (pe->dot.y >= room.xy1.y) && (pe->dot.y <= room.xy2.y))
        {
          if(access_button(pt, _FALSE, _SELECTOR_ONLY_HID) == _TRUE)
          {
            retcode = _TRUE; // es wurde ein button gefunden
            
            pt->fingercount += 1; // anzahl der finger auf button zählen (multitouch)     
           #ifdef UC_MULTITOUCH
            if(pt->fingercount == 1)
           #endif 
            {   
            
             #ifdef USER_DEMAG
              pt->enable = _TRUE;
             #endif
           
              if(do_doubleclick_function(pt) == _TRUE)
              {
               #ifdef SA34959 
                SetMoveButtonGroup(pe->multindex, pt->summary);
               #endif 
                DoButton(pt, _TRUE, Window, &offset, zoom, pe->lrm, _FALSE);
                Keyboard.TriggerEventStart(pe, pt->tip);
              }
              else if(pt->doubleclick == _TRUE)
              {
                retcode = _FALSE;
              }
            }
            return retcode; // es wurde ein button gefunden, nicht weitersuchen
          }
        }
      }
    }
  }
  else if(pe->ftype == _EVENT_HIDRELEASE)
  {
    for(i=0; i<Picture.mask.membutton.no; i++)
    {
      pt = &Picture.mask.membutton.ptr[i];
      room = pt->room;
      Zoom_ROOM(&room, &offset, zoom);

      if((pe->dot.x >= room.xy1.x) && (pe->dot.x <= room.xy2.x) &&
         (pe->dot.y >= room.xy1.y) && (pe->dot.y <= room.xy2.y))
      {
        if(pt->fingercount > 0)
        {
          pt->fingercount -= 1;
        }
       #ifdef UC_MULTITOUCH
        if(pt->fingercount == 0) // wenn kein finger mehr auf button ist (multitouch)
       #endif 
        {
          // Keyboard.TriggerEventStop(); // nicht notwendig wird automatisch beim HIDRELEASE beendet
         #ifdef USER_DEMAG
          if(pt->enable == _TRUE)
          {
            pt->enable = _FALSE;
         #else
          if(access_button(pt, _FALSE, _SELECTOR_ONLY_HID) == _TRUE)
          {
         #endif
            if(do_doubleclick_function(pt) == _TRUE)
            {
              retcode = _TRUE;
              DoButton(pt, _FALSE, Window, &offset, zoom, pe->lrm, _FALSE);
            }
           #ifdef SA34959 
            ReleaseMoveButtonGroup(pe->multindex);
           #endif 
            return retcode; // es wurde ein button gefunden, nicht weitersuchen
          }
         #ifdef SA34959 
          ReleaseMoveButtonGroup(pe->multindex);
         #endif 
        }
      }
    }
  }
 #ifdef SA34959 
  else if(pe->ftype == _EVENT_HIDMOVE)
  {
    _UDWORD mbg = GetMoveButtonGroup(pe->multindex);
    if(mbg != 0)
    {
      for(i=0; i<Picture.mask.membutton.no; i++)
      {
        pt = &Picture.mask.membutton.ptr[i];
        
        if((pt->workstate == STATE_RUN) && (pt->fingercount == 0) && (pt->summary == mbg))
        {
          room = pt->room;
          Zoom_ROOM(&room, &offset, zoom);

          if((pe->dot.x >= room.xy1.x) && (pe->dot.x <= room.xy2.x) &&
             (pe->dot.y >= room.xy1.y) && (pe->dot.y <= room.xy2.y))
          {
            if(access_button(pt, _FALSE, _SELECTOR_ONLY_HID) == _TRUE)
            {
              pt->fingercount = 1;
              retcode = _TRUE; // es wurde ein button gefunden

              if(pt->doubleclick == _FALSE)
              {
                pt->fingercount = 1;
                DoButton(pt, _TRUE, Window, &offset, zoom, pe->lrm, _FALSE);
              }
            }
          }
        }
      }  
    }
  }
 #endif // SA34959 
  else if(pe->ftype == _EVENT_TRIGGER)
  {
    for(i=0; i<Picture.mask.membutton.no; i++)
    {
      pt = &Picture.mask.membutton.ptr[i];
      
      if((pt->fingercount > 0) && (pt->workstate == STATE_RUN)) // nur wenn finger auf button ist
      {
        room = pt->room;
        Zoom_ROOM(&room, &offset, zoom);

        if((pe->dot.x >= room.xy1.x) && (pe->dot.x <= room.xy2.x) &&
           (pe->dot.y >= room.xy1.y) && (pe->dot.y <= room.xy2.y))
        {
         #ifdef USER_DEMAG
          if(pt->enable == _TRUE)
         #else
          if(access_button(pt, _FALSE, _SELECTOR_ONLY_HID) == _TRUE)
         #endif
          {
            if(do_doubleclick_function(pt) == _TRUE)
            {
              retcode = _TRUE;
              DoButton(pt, _TRUE, Window, &offset, zoom, pe->lrm, _TRUE);
            }
            return retcode; // es wurde ein button gefunden, nicht weitersuchen
          }
        }
      }
    }
  }

  if(retcode == _FALSE) // lookup for buttons in all objects
  {
    for(i=0; i<Picture.mask.memobject.no; i++)
    {
      po = &Picture.mask.memobject.ptr[i];

      if(po->SubPicture != NULL)
      {
        if(chk_CHKBIT(&po->chkbit) == _TRUE)
        {
          if(statescheme_OBJECT(po) == STATE_ACTIVE)
          {
            rekoff    = offset;
            rekoff.x += Zoom(zoom->x, po->xy.x);
            rekoff.y += Zoom(zoom->y, po->xy.y);

            tmpzoom.x = Zoom_ZOOM(po->zoom.x, zoom->x);
            tmpzoom.y = Zoom_ZOOM(po->zoom.y, zoom->y);

            if(po->SubPicture->DoHID(pe, Window, rekoff, &tmpzoom) == _TRUE)
            {
              retcode = _TRUE;
              break;
            }
          }
        }
      }
    }
  }

  if(retcode == _FALSE) // lookup for buttons in all motions
  {
    for(i=0; i<Picture.mask.memmotion.no; i++)
    {
      pm = &Picture.mask.memmotion.ptr[i];
      
      if(chk_CHKBIT(&pm->chkbit) == _TRUE)
      {
        if(PublicStateschema(&pm->stscheme) == STATE_ACTIVE)
        {
          if(pm->memkaru.no > 0)
          {
            _KARU *pkaru = pm->memkaru.ptr;
            _UWORD ki;
            for(ki=0; ki<pm->memkaru.no; ki++, pkaru++)
            {
              if(pkaru->SubPicture != NULL)
              {
                rekoff  = pkaru->room.xy1; //offset;
                tmpzoom = pkaru->zoom;
                if((pkaru->onscreen == _TRUE)&&(pkaru->gesamt == _TRUE))
                {
                  if(pkaru->SubPicture->DoHID(pe, Window, rekoff, &tmpzoom) == _TRUE)
                  {
                    retcode = _TRUE;
                    break;
                  }
                }
              }
            }
          }
          else if(pm->SubPicture != NULL)
          {
            //init_ZOOM(&tmpzoom);
            rekoff  = pm->oldxy; //offset;
            tmpzoom = *zoom;
            AddZoom(&tmpzoom, &pm->zoom);
            if(pm->SubPicture->DoHID(pe, Window, rekoff, &tmpzoom) == _TRUE)
            {
              retcode = _TRUE;
              break;
            }
          }
        }
      }
    }
  }

  if(retcode == _FALSE) // lookup for buttons in all objectschemes
  {
    for(i=0; i<Picture.mask.memoutput.no; i++)
    {
      pio = &Picture.mask.memoutput.ptr[i];
      if(pio->Obj != NULL)
      {
        if(chk_CHKBIT(&pio->chkbit) == _TRUE)
        {
          if(PublicStateschema(&pio->stscheme) == STATE_ACTIVE)
          {
            rekoff = pio->space.xy1; //offset;
            Make_ZOOM(&tmpzoom, pio->Obj, &pio->space);

            if(pio->Obj->DoHID(pe, Window, rekoff, &tmpzoom) == _TRUE)
            {
              retcode = _TRUE;
              break;
            }
          }
        }
      }
    }
  }
 
 #ifndef UC_OLD_MOTIONS
  if((retcode == _FALSE)||(pe->ftype == _EVENT_HIDRELEASE)) // lookup for action in all carousels
  {
    // hid_release ist notwendig um alle carousells beim release zu stoppen
    if(Picture.mask.memmotion.no > 0)
    {
      for(i=0, pm=Picture.mask.memmotion.ptr; i<Picture.mask.memmotion.no; i++, pm++)
      {
        if(pm->memkaru.no > 0) // nur wenns a carousel ist
        {
          if(pm->usermove == _FALSE) // ansonsten "moved" user  
          {
            if(chk_CHKBIT(&pm->chkbit) == _TRUE)
            {
              if(PublicStateschema(&pm->stscheme) == STATE_ACTIVE)
              {
                action_CAROUSEL(pm, pe);
              }
            }
          }
        }
      }
    }
  }
 #endif
  
  return retcode;
}
#endif

_BOOL _SubPicture::CopyEditor(_EDITOR *pe)
{
  _BOOL  retcode = _FALSE;
  _INPUT *pi;

  if(Picture.mask.meminput.no >= 1)
  {
    retcode = _TRUE;
    pi      = &Picture.mask.meminput.ptr[0];
    init_CHKBIT_Makro(&pi->io.chkbit);
    pi->io.typ       = pe->input.io.typ;
    pi->io.variable  = pe->input.io.variable;
    pi->io.info      = pe->input.io.info;
    pi->io.picformat = pe->input.io.picformat;

    // virtueller name wird nur, wenn in editor kein virtueller name vergeben ist übernommen.
    if(pi->io.virtname.no >= Picture.mask.enclosed_name.ascii_buffer.no)
    {
      // kein virtueller name
      pi->io.virtname = pe->input.io.virtname;

      if(pi->io.typ == AS_THEURGE)
      {
        if(Memory2((void**)&pi->io.theurge, sizeof(_THEURGE)) != 0)
        {
          init_THEURGE(pi->io.theurge);
          copy_THEURGE(&retcode, pi->io.theurge, pe->input.io.theurge);
        }
      }
    }
    else
    {
      // virtueller name eingetragen
      if(pi->io.typ == AS_THEURGE)
      {
        pi->io.theurge = NULL;
        pi->io.typ = AS_NUMERIC;
      }
    }
  }

  return(retcode);
}

_BOOL _SubPicture::DoNegEdge(_BUTTON *pt)
{
  if(pt->fingercount > 0) // auf diesem button ist aktuell noch mindestens ein finger (multitouch)
  {
    if(chk_VARLABEL(&pt->varlabel) == _TRUE)
    {
      // es ist ein server beim button hinterlegt
      if(pt->act_momentary == _TRUE) // es ist ein momentary button
      {
        // hlawol 19.09.2016: bei momentary buttons den serverwert auf release-state setzen
        set_buttonval(&pt->varlabel, pt->reset_val, _FALSE); 
      }
        
      // tatsächlichen buttonstatus ermitteln
      get_stationary_state(&pt->state, &pt->varlabel, pt->set_val, pt->reset_val);
    }
    else
    {
      // es ist kein server beim button hinterlegt --> ungedrückt darstellen
      
     #ifdef SA54918
      do_LSEFUNCT_lrm(&pt->gfunct.neg_fl, NULL, _FALSE); // Funktion von negativer Flanke vom Button aufrufen
     #endif      
      
      pt->state = _FALSE;
    }
      
   #ifdef USER_DEMAG
    pt->enable = pt->state;
   #endif
      
    pt->fingercount = 0;
    pt->oldstate = (pt->state == _TRUE)? _FALSE : _TRUE;     // just to enable refresh !!
    
    return _TRUE;
  }
  
  return _FALSE;
}

void _SubPicture::DoAllNegEdge(_BOOL redraw)
{
  _UWORD      i;
  _MEM_BUTTON *pm;
  _BUTTON     *pt;
  _OBJECT     *po;
  _MOTION     *pmo;
  _DOT        offset;
  _ZOOM       zoom;
  _IO         *pio;

  init_DOT(&offset);
  init_ZOOM(&zoom);

  Keyboard.TriggerEventStop(NULL);

 #ifdef SA34959 
  InitMoveButtonGroup(); // alle move-button-merker zurücksetzen
 #endif
 
  pm = &Picture.mask.membutton;

  for(i=0, pt=pm->ptr; i<pm->no; i++, pt++)
  {
    if(DoNegEdge(pt) == _TRUE) // button loslassen
    {
      if(redraw == _TRUE)
      {
        RefreshTouchButton(pt, _FALSE, &offset, &zoom);
      }
    }
  }

  for(i=0; i<Picture.mask.memobject.no; i++)
  {
    po = &Picture.mask.memobject.ptr[i];
    if(po->SubPicture != NULL)
      po->SubPicture->DoAllNegEdge(redraw);
  }

  for(i=0; i<Picture.mask.memmotion.no; i++)
  {
    pmo = &Picture.mask.memmotion.ptr[i];
   #ifndef UC_OLD_MOTIONS 
    if(pmo->memkaru.no > 0)
    {
      negedge_MEM_KARU(&pmo->memkaru, _FALSE);
      stop_move_CAROUSEL(pmo); // movement in karousel beenden
    }
    else if(pmo->SubPicture != NULL)
      pmo->SubPicture->DoAllNegEdge(_FALSE);
   #else
    if(pmo->SubPicture != NULL)
      pmo->SubPicture->DoAllNegEdge(_FALSE);
   #endif
  }

  for(i=0; i<Picture.mask.memoutput.no; i++)
  {
    pio = &Picture.mask.memoutput.ptr[i];
    if(pio->Obj != NULL)
      pio->Obj->DoAllNegEdge(_FALSE);
  }

}


#ifdef OK_RUNTIME_MODIFY
_UWORD horiz_nextno = 0;
_UWORD vert_nextno  = 0;
_BOOL _SubPicture::InsertNextNo(_UWORD no, _BOOL horiz)
{
  _BOOL retcode = _FALSE;

  if(no < Picture.mask.meminput.no)
  {
    retcode = _TRUE;

    if(horiz == _TRUE)
    {
      Picture.mask.meminput.ptr[no].h_next = horiz_nextno;
      horiz_nextno ++;
    }
    else
    {
      Picture.mask.meminput.ptr[no].v_next = vert_nextno;
      vert_nextno ++;
    }
    Picture.mask.change = _TRUE;
  }

  return(retcode);
}
#endif

#ifdef OK_SUMMARY

void _SubPicture::OutSummary_IO(_IO *pio, _BOOL state, _UWORD no, _DOT *offset, _ZOOM *zoom, _BOOL all, _Picture *PPic)
{
  _COLOR       colback, coltxt, colframe;
  _ATTRIB      attrib;

  if((no == (pio->summary & 0x7FFF))||(all == _TRUE))
  {
    colframe = pio->colframe;
    colback  = pio->colback;
    coltxt   = pio->coltxt;
    attrib   = pio->attrib;
    pio->summary &= 0x7FFF;

    if(state == _TRUE)
    {
      pio->summary |= 0x8000;
      pio->colback  = SetUp.GetEditColorBack(_SELECTGROUP, pio->colback);
      pio->colframe = SetUp.GetEditColorFrame(_SELECTGROUP, pio->colframe);
      pio->coltxt   = SetUp.GetEditColorTxt(_SELECTGROUP, pio->coltxt);
      pio->attrib   = (attrib & (~T_PATTERN)) | T_SOLID;
    }

    OutIoIntern(pio, offset, zoom, _FALSE, _TRUE, 0xFFFF, PPic, _FALSE);

    pio->colframe = colframe;
    pio->colback  = colback;
    pio->coltxt   = coltxt;
    pio->attrib   = attrib;
  }
}

void _SubPicture::OutSummary(_BOOL state, _UWORD no, _DOT *offset, _ZOOM *zoom, _BOOL all, _Picture *PPic)
{
  _PICTUREMASK *pm = &Picture.mask;
  _UWORD       i;
  _PICTEXT     *p;
  _COL_PICTXT  *pc;
  _COLOR       colback, coltxt, colframe;
  _ATTRIB      attrib;
  _OBJECT      *pobj;
  _DOT         tmpoffset;
  _ZOOM        tmpzoom;
  _LSEVARINFO  *pvi;
  _BOOL        st;

  if(no != DEFSUMMARY)
  {
    for(i=0; i<pm->mempictxt.no; i++)
    {
      p  = &pm->mempictxt.ptr[i];
      st = _TRUE;
      switch(p->anypara.anything.list)
      {
        case VARTXT1_TOKEN :
        case VARTXT1 :
        case VARTXT2 :
        case VARTXT3 :
        case VARTXT4 :
        case VARNAME :
        case VARUNIT : pvi = NULL;
                       st  = cis_server_valid(&pvi, p->anypara.anything.no);
                       break;
        default      : break;
      }

      if((st == _TRUE)&&((no == p->summary)||(all == _TRUE))) //((all == _TRUE)&&(p->summary != DEFSUMMARY)))
      {
        colframe = p->colframe;
        colback  = p->colback;
        coltxt   = p->coltxt;
        attrib   = p->attrib;

        if(state == _TRUE)
        {
          p->colback  = SetUp.GetEditColorBack(_SELECTGROUP, p->colback);
          p->colframe = SetUp.GetEditColorFrame(_SELECTGROUP, p->colframe);
          p->coltxt   = SetUp.GetEditColorTxt(_SELECTGROUP, p->coltxt);
          p->attrib   = (attrib & (~T_PATTERN)) | T_SOLID;
        }

        Out_PICTEXT(p, offset, zoom, _TRUE);

        p->colframe = colframe;
        p->colback  = colback;
        p->coltxt   = coltxt;
        p->attrib   = attrib;
      }
    }

    for(i=0; i<pm->colpictxt.no; i++)
    {
      pc = &pm->colpictxt.ptr[i];
      if((no == pc->btx.summary)||(all == _TRUE)) //((all == _TRUE)&&(pc->btx.summary != DEFSUMMARY)))
      {
        st = _TRUE;
        switch(pc->btx.anypara.anything.list)
        {
          case VARTXT1_TOKEN :
          case VARTXT1 :
          case VARTXT2 :
          case VARTXT3 :
          case VARTXT4 : pvi = NULL;
                         st  = cis_server_valid(&pvi, pc->btx.anypara.anything.no);
                         break;
          default      : break;
        }

        if(st == _TRUE)
        {
        if(state == _TRUE)
        {
          p           = &pc->btx;
          colframe    = p->colframe;
          colback     = p->colback;
          coltxt      = p->coltxt;
          attrib      = p->attrib;
          p->colback  = SetUp.GetEditColorBack(_SELECTGROUP, p->colback);
          p->colframe = SetUp.GetEditColorFrame(_SELECTGROUP, p->colframe);
          p->coltxt   = SetUp.GetEditColorTxt(_SELECTGROUP, p->coltxt);
          p->attrib   = (attrib & (~T_PATTERN)) | T_SOLID;
          Out_PICTEXT(p, offset, zoom, _TRUE);
          p->colframe = colframe;
          p->colback  = colback;
          p->coltxt   = coltxt;
          p->attrib   = attrib;
        }
        else
        {
          Out_COL_PICTXT(pc, offset, zoom, _TRUE);
        }
        }
      }
    }

    for(i=0; i<pm->memoutput.no; i++)
      OutSummary_IO(&pm->memoutput.ptr[i], state, no, offset, zoom, all, PPic);

    for(i=0; i<pm->meminput.no; i++)
      OutSummary_IO(&pm->meminput.ptr[i].io, state, no, offset, zoom, all, PPic);

    for(i=0; i<pm->memobject.no; i++)
    {
      pobj = &pm->memobject.ptr[i];
      if((no == pobj->summary)||(all == _TRUE)) //((all == _TRUE)&&(pobj->summary != DEFSUMMARY)))
      {
        if((pobj->SubPicture != NULL)&&(pobj->onscreen == _TRUE))
        {
          init_DOT(&tmpoffset);
          init_ZOOM(&tmpzoom);
          tmpoffset.x = Zoom(zoom->x, pobj->xy.x) + offset->x;
          tmpoffset.y = Zoom(zoom->y, pobj->xy.y) + offset->y;
          tmpzoom.x = Zoom_ZOOM(zoom->x, pobj->zoom.x);
          tmpzoom.y = Zoom_ZOOM(zoom->y, pobj->zoom.y);
          pobj->SubPicture->OutSummary(state, no, &tmpoffset, &tmpzoom, _TRUE, PPic);
        }
      }
    }
  }
}
#endif

#ifdef OK_ENCLOSE_NAME

_ASCII *get_virtname(_UWORD no, _PICTUREMASK *pm)
{
  return(get_text(&pm->enclosed_name, no));
}

void *_SubPicture::GetThisPointer(_UWORD no)
{
  _ASCII *name;
  void   *retcode = NULL;

  name = get_virtname(no, &Picture.mask);
  if(name != NULL)
    retcode = get_this_pointer(name);

  return(retcode);
}

void theurge_needs(_THEURGE *ptu)
{
  _UWORD i;

  // farben auf dyncolor umkopieren
  if(ptu->memcolor.no == ptu->memdyncolor.no)
  {
    for(i=0; i<ptu->memcolor.no; i++)
      ptu->memdyncolor.ptr[i].color = ptu->memcolor.ptr[i];
  }
}

_BOOL _SubPicture::FindVirtualName(_IO *pio)
{
  _BOOL     retcode = _FALSE;
 #ifndef CUTOFF_USER
  void      *ptr;
  _VIRTNAME *p;

  p = &pio->virtname;

  if(p->no != 0xFFFF)
  {
    retcode = _TRUE;
    if(p->pthis == NULL)
    {
      retcode = _FALSE;
      p->pthis = GetThisPointer(p->no);
      if(p->pthis != NULL)
      {
        ptr = find_lasal_method(p->pthis, METHOD_MYIO_START);
        if(ptr != NULL)
        {
          if(pio->theurge != NULL)
            theurge_needs(pio->theurge); // ansonsten stimmt dyncolor in _MyIo::If_Start nicht

          void *tmp = p->pthis;
          load_this(tmp);
          ((_FVOID_PT_BOOL)ptr)(pio, _TRUE);            // call start methode
          retcode = _TRUE;         // objekt definiert, jedoch nicht gefunden
        }
      }
    }
  }
 #endif
  return(retcode);
}
#endif

_BOOL _SubPicture::Overload(_VARLABEL *ov, _BOOL change_ctrl_type)
{
  _BOOL retcode = _FALSE;

  if(valid_VARLABEL(ov) == _TRUE)
    retcode = overload_PICTUREMASK(&Picture.mask, ov, change_ctrl_type);

  return(retcode);
}

_BOOL _SubPicture::ChangeOverload(_VARLABEL *ovist, _VARLABEL *ovsoll)
{
  _BOOL retcode = _FALSE;

  if(valid_VARLABEL(ovsoll) == _TRUE)
    retcode = change_overload_PICTUREMASK(&Picture.mask, ovist, ovsoll);

  return(retcode);
}

_BOOL _SubPicture::SummaryOverload(_UWORD summary, _VARLABEL *ovsoll)
{
  _BOOL retcode = _FALSE;

  if(valid_VARLABEL(ovsoll) == _TRUE)
    retcode = summary_overload_PICTUREMASK(&Picture.mask, summary, ovsoll);

  return(retcode);
}

void _SubPicture::KillBackGroundImage(void)
{
  kill_backgroundimage(&Picture.mask);
}

_UDWORD _SubPicture::GetCarouselRoom(_ROOM *p)
{
  _UDWORD retcode = 0;
  for(_UWORD i=0; i<Picture.mask.memmotion.no; i++)
  {
    if(Picture.mask.memmotion.ptr[i].memkaru.no > 0)
    {
      _ROOM *pr = &Picture.mask.memmotion.ptr[i].wiperoom;
      retcode += 1;
      if((p->xy1.x == p->xy2.x)||(p->xy1.y == p->xy2.y))
      {
        *p = *pr;
      }
      else
      {
        if(pr->xy1.x < p->xy1.x)
          p->xy1.x = pr->xy1.x;
        if(pr->xy2.x > p->xy2.x)
          p->xy2.x = pr->xy2.x;
        if(pr->xy1.y < p->xy1.y)
          p->xy1.y = pr->xy1.y;
        if(pr->xy2.y > p->xy2.y)
          p->xy2.y = pr->xy2.y;
      }
    }
  }
  
  return retcode;
}

_BOOL _SubPicture::IsThereAnyCarousel(void)
{
  for(_UWORD i=0; i<Picture.mask.memmotion.no; i++)
  {
    if(Picture.mask.memmotion.ptr[i].memkaru.no > 0)
      return _TRUE;
  }
  
  return _FALSE;
}

#ifdef HID
#ifdef UC_THEME
_BOOL _SubPicture::SeekThemeCancle(_DOT *pd, _DOT offset, _ZOOM *zoom, _BOOL press)
{
  _BOOL        retcode = _FALSE;
  _ROOM        room;
  _PICTUREMASK *pm;
  _UWORD       i;
  _RECTANGLE   *pr;
  _COL_RECT    *pcr;
  _PICTEXT     *pt;
  _OBJECT      *po;
  _DOT         rekoff;
  _ZOOM        tmpzoom;

  if(press == _TRUE)
  {
    pm = &Picture.mask;

    for(i=0, pr=pm->memrectangle.ptr; i<pm->memrectangle.no; i++, pr++)
    {
      if(pr->frametype >= _THEMEFRAME)
      {
        room = pr->room;
        Zoom_ROOM(&room, &offset, zoom);
        if(is_THEME_cancle(pd, pr->frametype, &room, &pr->chkbit, NULL) == _TRUE)
        {
          retcode = _TRUE;
          draw_THEME_cancle(&room, pr->frametype, press);
          break;
        }
      }
    }

    for(i=0, pcr=pm->colrect.ptr; i<pm->colrect.no; i++, pcr++)
    {
      if(pcr->rectangle.frametype >= _THEMEFRAME)
      {
        room = pcr->rectangle.room;
        Zoom_ROOM(&room, &offset, zoom);
        if(is_THEME_cancle(pd, pcr->rectangle.frametype, &room, &pcr->rectangle.chkbit, NULL) == _TRUE)
        {
          retcode = _TRUE;
          draw_THEME_cancle(&room, pcr->rectangle.frametype, press);
          break;
        }
      }
    }

    if(retcode == _FALSE)
    {
      for(i=0, pt=pm->mempictxt.ptr; i<pm->mempictxt.no; i++, pt++)
      {
        if(pt->frametype >= _THEMEFRAME)
        {
          room = pt->room;
          Zoom_ROOM(&room, &offset, zoom);
          if(is_THEME_cancle(pd, pt->frametype, &room, &pt->chkbit, &pt->stscheme) == _TRUE)
          {
            retcode = _TRUE;
            draw_THEME_cancle(&room, pt->frametype, press);
            break;
          }
        }
      }
    }

    if(retcode == _FALSE)
    {
      for(i=0, po=pm->memobject.ptr; i<pm->memobject.no; i++, po++)
      {
        if(po->SubPicture != NULL)
        {
          rekoff    = offset;
          rekoff.x += Zoom(zoom->x, po->xy.x);
          rekoff.y += Zoom(zoom->y, po->xy.y);
          tmpzoom.x = Zoom_ZOOM(po->zoom.x, zoom->x);
          tmpzoom.y = Zoom_ZOOM(po->zoom.y, zoom->y);

          // berechne position von objekt und nur wenn click innerhalb position dann...
          po->SubPicture->GetSize(&room);
          Zoom_ROOM(&room, &rekoff, &tmpzoom);
          if((pd->x >= room.xy1.x)&&(pd->x <= room.xy2.x)&&(pd->y >= room.xy1.y)&&(pd->y <= room.xy2.y))
          {
            if(po->SubPicture->SeekThemeCancle(pd, rekoff, &tmpzoom, press) == _TRUE)
            {
              retcode = _TRUE;
              break;
            }
          }
        }
      }
    }
//                 _MEM_OUTPUT       memoutput;
//                 _MEM_COL_RECT     colrect;
//                 _MEM_COL_PICTXT   colpictxt;
  }

  if((retcode == _TRUE)&&(press == _TRUE))
    Beep(BEEP_X);

  return(retcode);
}
#endif
#endif

#ifdef NINETEEN

_DWORD _SubPicture::CyclicButton(_EVENT *pe, _DOT *offset, _ZOOM *zoom)
{
  _PICTUREMASK *pm = &Picture.mask;
  _UWORD       i;
  _ROOM        room;
  _BUTTON      *pb;
  _OBJECT      *pob;
  _IO          *pio;
  _ZOOM        nzoom;
  _DOT         noffset;
  _DWORD       hkc;

  // buttons
  for(i=0, pb=pm->membutton.ptr; i<pm->membutton.no; i++, pb++)
  {
    room = pb->room;
    Zoom_ROOM(&room, offset, zoom);

    if((pe->dot.x >= room.xy1.x) && (pe->dot.x <= room.xy2.x) &&
       (pe->dot.y >= room.xy1.y) && (pe->dot.y <= room.xy2.y))
    {
      if(pb->hotkeycode != 0xFFFF)
      {
       #ifdef USER_KM
        return(pb->hotkeycode);
       #else
        if(pb->full_access_given == _TRUE) // access=true, chkbit=true, statescheme=aktiv
        {
          return (pb->hotkeycode);
        }
       #endif
      }
    }
  }

  // buttons in objects
  for(i=0, pob=pm->memobject.ptr; i<pm->memobject.no; i++, pob++)
  {
    if(pob->SubPicture != NULL)
    {
      noffset    = *offset;
      noffset.x += Zoom(zoom->x, pob->xy.x);
      noffset.y += Zoom(zoom->y, pob->xy.y);
      nzoom.x = Zoom_ZOOM(pob->zoom.x, zoom->x);
      nzoom.y = Zoom_ZOOM(pob->zoom.y, zoom->y);

      hkc = pob->SubPicture->CyclicButton(pe, &noffset, &nzoom);
      if(hkc != -1)
        return(hkc);
    }
  }

  // buttons in objectschemes
  for(i=0, pio=pm->memoutput.ptr; i<Picture.mask.memoutput.no; i++, pio++)
  {
    if((pio->Obj != NULL)&&(pio->workstate == STATE_RUN))
    {
      noffset = pio->space.xy1; //offset;
      Make_ZOOM(&nzoom, pio->Obj, &pio->space);

      hkc = pio->Obj->CyclicButton(pe, &noffset, &nzoom);
      if(hkc != -1)
        return(hkc);
    }
  }

  return(-1);
}
#endif

void _SubPicture::MakeSize(void)
{
  make_size_PICTURE(&Picture);
}

extern "C" _BOOL get_visible_objectindex(_DWORD *firstindex, _DWORD *lastindex, _MOTION *pmotion)
{
  _BOOL  retcode = _FALSE;
  _DWORD fidx = -1;
  _DWORD lidx = -1;

  if(pmotion != NULL)
  {
    _KARU *pk = pmotion->memkaru.ptr;
    if(pk != NULL)
    {
      _UWORD i;
      for(i=0; i<pmotion->memkaru.no; i++, pk++)
      {
        if(pk->onscreen == _TRUE)
        {
          if(fidx == -1)
            fidx = i;
        }
        else if((fidx >= 0)&&(lidx == -1))
        {
          lidx = i;
        }
      }
    }
  }
  
  *lastindex  = lidx;
  *firstindex = fidx;
  
  return retcode;
}

extern "C" _UDWORD get_pixsize(_MOTION *pmotion, _DWORD objectindex)
{
  _UDWORD retcode = 0;

  if(pmotion != NULL)
  {
    _KARU *pk = pmotion->memkaru.ptr;
    if(pk != NULL)
    {
      if(objectindex < 0)
      {
        retcode = pmotion->karuwidth;
      }
      else if(objectindex < pmotion->memkaru.no)
      {
        pk = &pk[objectindex];
        if(pmotion->horizontal == _TRUE)
          retcode = pk->room.xy2.x - pk->room.xy1.x + 1;
        else
          retcode = pk->room.xy2.y - pk->room.xy1.y + 1;
      }
    }
  }
  
  return retcode;
}



_FRAMETYPE _SubPicture::GetBackgroundTheme(_ROOM *proom)
{
  _MEM_RECTANGLE *pmr = &Picture.mask.memrectangle;
  _RECTANGLE     *pr;
  _UWORD         i;
  _MEM_COL_RECT  *pmcr = &Picture.mask.colrect;
  _COL_RECT      *pcr;

  for(i=0, pr=pmr->ptr; i<pmr->no; i++, pr++)
  {
    if((pr->room.xy1.x == proom->xy1.x)&&(pr->room.xy2.x == proom->xy2.x)&&(pr->room.xy1.y == proom->xy1.y)&&(pr->room.xy2.y == proom->xy2.y))
      return(pr->frametype);
  }

  for(i=0, pcr=pmcr->ptr; i<pmcr->no; i++, pcr++)
  {
    if((pcr->rectangle.room.xy1.x == proom->xy1.x)&&(pcr->rectangle.room.xy2.x == proom->xy2.x)&&(pcr->rectangle.room.xy1.y == proom->xy1.y)&&(pcr->rectangle.room.xy2.y == proom->xy2.y))
      return(pcr->rectangle.frametype);
  }

  return(_DEFFRAME);
}

// ****************** interface für ST ********************************

_BOOL do_zoom_room_needs(_ZOOM *pzoom, _ROOM *proom, _SubPicture *pic)
{
  _BOOL retcode = _FALSE;
  _ROOM space;
  _KOORD div;
  
  if((proom != NULL) && (pic != NULL))
  {
    retcode = _TRUE;
    pic->GetSize(&space);
    init_ZOOM(pzoom);
    
    pzoom->y = 1000;
    div = (space.xy2.y - space.xy1.y);
    if(div > 0)
      pzoom->y = ((proom->xy2.y - proom->xy1.y) * 1000) / div;
    
    pzoom->x = 1000;
    div = (space.xy2.x - space.xy1.x) + 1;
    if(div > 0)
      pzoom->x = ((proom->xy2.x - proom->xy1.x) * 1000) / div;
  }
  
  return retcode;
}

extern "C" void SubPicture_Draw(_ROOM *proom, _SubPicture *pic)
{
  _ZOOM zoom;

  if(do_zoom_room_needs(&zoom, proom, pic) == _TRUE)
  {
    pic->Draw(&proom->xy1, &zoom, NULL);
  }
}

extern "C" void SubPicture_Run(_ROOM *proom, _SubPicture *pic)
{
  _ZOOM zoom;

  if(do_zoom_room_needs(&zoom, proom, pic) == _TRUE)
  {
    pic->Refresh(0, &proom->xy1, &zoom, NULL);
  }
}

extern "C" _SubPicture* SubPicture_Copy(_SubPicture *pic)
{
  _SubPicture *retcode = NULL; 
  
  if(pic != NULL)
  {
    if(Memory2((void**)&retcode, sizeof(_SubPicture)) != 0)
    {
      retcode->Init();
      if(retcode->Copy(pic) == _TRUE)
        return retcode;
        
      retcode->Free();
      Memory2((void**)&retcode, 0);
    }
  }
  
  return retcode;
}

extern "C" void SubPicture_Overload(_SubPicture *pic, _VARLABEL *vl)
{
  if((pic != NULL)&&(vl != NULL))
    pic->Overload(vl);
}

extern "C" void SubPicture_OverloadSummary(_SubPicture *pic, _UWORD summary, _VARLABEL *vl)
{
  if((pic != NULL)&&(vl != NULL))
    pic->SummaryOverload(summary, vl);
}

extern "C" void SubPicture_Free(_SubPicture *pic)
{
  if(pic != NULL)
    pic->Free();
}
