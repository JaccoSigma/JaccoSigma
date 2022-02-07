#include "T_IPC.H"
#include "T_NUC.H"

#define B_WIDTH   LEVELBUTTON_WIDTH
#define B_HEIGHT  (LEVELBUTTON_HEIGHT + 1)

#define SA40653 // DEVSW-435
#define SA35235 // karousell per Index auf ein bestimmte Position schalten
#ifdef USER_HERZ
 #define SA35492 // Speicherreduktion bei Karousell, BackgroundImages werden wieder freigegeben
#endif

_BOOL dot_in_ROOM(_ROOM *pr, _DOT *p)
{
  return ((p->x >= pr->xy1.x) && (p->x <= pr->xy2.x) && (p->y >= pr->xy1.y) && (p->y <= pr->xy2.y))? _TRUE : _FALSE;
}

_BOOL is_inc(_SCROLLBAR *p, _DOT *dot)
{
  if(p->horizontal == _TRUE)
  {
    if((dot->y >= p->room.xy1.y) && (dot->y <= p->room.xy2.y))
    {
      if((dot->x <= p->room.xy2.x) && (dot->x > (p->room.xy2.x - B_WIDTH)))
        return _TRUE;
    }
  }
  else if((dot->x >= p->room.xy1.x) && (dot->x <= p->room.xy2.x))
  {
    if((dot->y <= p->room.xy2.y) && (dot->y > (p->room.xy2.y - B_HEIGHT)))
      return _TRUE;
  }

  return _FALSE;
}

_BOOL is_dec(_SCROLLBAR *p, _DOT *dot)
{
  if(p->horizontal == _TRUE)
  {
    if((dot->y >= p->room.xy1.y) && (dot->y <= p->room.xy2.y))
    {
      if((dot->x >= p->room.xy1.x) && (dot->x < (p->room.xy1.x + B_WIDTH)))
        return _TRUE;
    }
  }
  else if((dot->x >= p->room.xy1.x) && (dot->x <= p->room.xy2.x))
  {
    if((dot->y >= p->room.xy1.y) && (dot->y < (p->room.xy1.y + B_HEIGHT)))
      return _TRUE;
  }

  return _FALSE;
}

_DWORD get_XX(_SCROLLBAR *p)
{
  if(p->horizontal == _TRUE)
  {
    return p->room.xy2.x - p->room.xy1.x + 1 - 3 * B_WIDTH;
  }
  
  return p->room.xy2.y - p->room.xy1.y + 1 - 3 * B_HEIGHT;
}

_DWORD thumb_position(_SCROLLBAR *p)
{
  if(p->horizontal == _TRUE)
  {
    if(p->width > 1)
      return p->room.xy1.x + B_WIDTH + (get_XX(p) * p->pos) / (p->width - 1);
    return p->room.xy1.x + B_WIDTH;
  }
  
  if(p->width > 1)
    return p->room.xy1.y + B_HEIGHT + (get_XX(p) * p->pos) / (p->width - 1);
  return p->room.xy1.y + B_HEIGHT;
}

_BOOL is_thumb(_SCROLLBAR *p, _DOT *dot)
{
  _DWORD xy;
  
  if(p->horizontal == _TRUE)
  {
    if((dot->y >= p->room.xy1.y) && (dot->y <= p->room.xy2.y))
    {
      xy = thumb_position(p); 
      if((dot->x >= xy) && (dot->x < (xy + B_WIDTH)))
        return _TRUE;
    }
  }
  else if((dot->x >= p->room.xy1.x) && (dot->x <= p->room.xy2.x))
  {
    xy = thumb_position(p); 
    if((dot->y >= xy) && (dot->y < (xy + B_HEIGHT)))
      return _TRUE;
  }
  
  return _FALSE;
}

void draw_scrbutton(_ROOM *proom, _COLOR col, _COLOR col_frame, _FRAMETYPE frametype, _BOOL invers)
{
  if(frametype < _THEMEFRAME)
  {
    DrawButton(proom, T_FILL|T_COPY|T_SOLID, _3DFRAME, col, col_frame);
  }
  else
  {
    DrawButton(proom, T_FILL|T_COPY|T_SOLID | ((invers)? T_INVERTTHEME : 0), frametype, col, col_frame);
  }
}

void draw_SCROLLBAR(_SCROLLBAR *p)
{
  _DWORD xy, tmp; 
  _ROOM  room, slimroom;
  
  if(p->valid == _TRUE)
  {
    _BOOL slimdesign = IsSlimDesign(p->frametype);
    
   #ifdef UC_MOUSE
    _UWORD hide = IfNecHideMouseCursor(&p->room);
   #endif
   
    if(slimdesign == _TRUE)
    {
      DrawButton(&p->room, T_FILL|T_COPY|T_SOLID, p->frametype, p->col_back, RED);
      GetSlimSliderDecentRoom(&slimroom, &p->room, p->horizontal, _FALSE);
      DrawButton(&slimroom, T_COPY|T_SOLID|T_FILL, p->frametype, p->col_text, p->col_frame);
    }
    else
    {
      draw_scrbutton(&p->room, p->col_back, p->col_frame, p->frametype, _TRUE);
    }
    
    if(p->horizontal == _TRUE)
    {
      if(slimdesign == _TRUE)
      {
        room       = p->room;
        room.xy2.x = room.xy1.x + B_WIDTH - 1;
        ScrButton(&room, NULL, T_FILL|T_COPY|T_SOLID, p->frametype, p->col_frame, GetBackColor(p->col_text), _LEFT);

        room       = p->room;
        room.xy1.x = p->room.xy2.x - B_WIDTH + 1;
        ScrButton(&room, NULL, T_FILL|T_COPY|T_SOLID, p->frametype, p->col_frame, GetBackColor(p->col_text), _RIGHT);
      }
      else
      {
        room       = p->room;
        room.xy2.x = room.xy1.x + B_WIDTH - 1;
        draw_scrbutton(&room, p->col_front, p->col_frame, p->frametype, _FALSE);
        level_triangle(&room, _LEFT, p->frametype, p->col_text);

        room       = p->room;
        room.xy1.x = p->room.xy2.x - B_WIDTH + 1;
        draw_scrbutton(&room, p->col_front, p->col_frame, p->frametype, _FALSE);
        level_triangle(&room, _RIGHT, p->frametype, p->col_text);
      }
      
      if(p->active == _TRUE)
      {
        xy = p->active_dot.x - B_WIDTH / 2;
      }
      else
      {
        xy = thumb_position(p);
      }

      tmp = p->room.xy2.x - B_WIDTH * 2;
      if(xy > tmp)
        xy = tmp;

      tmp = p->room.xy1.x + B_WIDTH;
      if(xy < tmp)
        xy = tmp;
      
      room.xy1.x = xy;
      room.xy1.y = p->room.xy1.y;
      room.xy2.x = room.xy1.x + B_WIDTH - 1;
      room.xy2.y = p->room.xy2.y;
      if(slimdesign == _TRUE)
      {
        slimroom.xy1.x = room.xy1.x;
        slimroom.xy2.x = room.xy2.x;
        DrawButton(&slimroom, T_COPY|T_SOLID|T_FILL, p->frametype, p->col_front, BLACK);
      }
      else
      {
        draw_scrbutton(&room, p->col_front, p->col_frame, p->frametype, p->active);
      }
    }
    else
    {
      if(slimdesign == _TRUE)
      {
        _COLOR symcol = MakeColor(p->col_text, 0);
        room       = p->room;
        room.xy2.y = room.xy1.y + B_HEIGHT - 1;
        ScrButton(&room, NULL, T_FILL|T_COPY|T_SOLID, p->frametype, p->col_front, symcol, _UP);
        room       = p->room;
        room.xy1.y = room.xy2.y - B_HEIGHT + 1;
        ScrButton(&room, NULL, T_FILL|T_COPY|T_SOLID, p->frametype, p->col_front, symcol, _DOWN);
      }
      else
      {
        room       = p->room;
        room.xy2.y = room.xy1.y + B_HEIGHT - 1;
        draw_scrbutton(&room, p->col_front, p->col_frame, p->frametype, _FALSE);
        level_triangle(&room, _UP, p->frametype, p->col_text);
      
        room       = p->room;
        room.xy1.y = room.xy2.y - B_HEIGHT + 1;
        draw_scrbutton(&room, p->col_front, p->col_frame, p->frametype, _FALSE);
        level_triangle(&room, _DOWN, p->frametype, p->col_text);
      }
      
      if(p->active == _TRUE)
      {
        xy = p->active_dot.y - B_HEIGHT / 2;
      }
      else
      {
        xy = thumb_position(p);
      }

      tmp = p->room.xy2.y - B_HEIGHT * 2;
      if(xy > tmp)
        xy = tmp;

      tmp = p->room.xy1.y + B_HEIGHT;
      if(xy < tmp)
        xy = tmp;

      room.xy1.x = p->room.xy1.x;
      room.xy1.y = xy;
      room.xy2.x = p->room.xy2.x;
      room.xy2.y = room.xy1.y + B_HEIGHT - 1;
      
      if(slimdesign == _TRUE)
      {
        slimroom.xy1.y = room.xy1.y;
        slimroom.xy2.y = room.xy2.y;
        DrawButton(&slimroom, T_COPY|T_SOLID|T_FILL, p->frametype, p->col_front, BLACK);
      }
      else
      {
        draw_scrbutton(&room, p->col_front, p->col_frame, p->frametype, p->active);
      }
    }
    
   #ifdef UC_ROTATE
    RefreshRectangle(&p->room);
   #endif
    
   #ifdef UC_MOUSE
    IfNecShowMouseCursor(hide);
   #endif
  }
}

_BOOL run_SCROLLBAR(_SCROLLBAR *p, _EVENT *pev)
{
  // retcode = _TRUE wenn durch bedienung etwas (akt.position) verändert wurde
  
  _DWORD tmp, tpos;
  _BOOL  retcode = _FALSE;
  
  if((p->valid == _TRUE) && (p->width > 1))
  {
    _UDWORD chx = 0;
    
    if(p->pos != p->oldpos)
    {
      p->oldpos = p->pos;
      chx = 1;
    }
    
    if(p->width != p->oldwidth)
    {
      p->oldwidth = p->width;
      chx = 1;
    }
    
    if(pev != NULL)
    {
    
      switch(pev->ftype)
      {
        case _EVENT_TRIGGER    :
        case _EVENT_HIDPRESS   :
          if(dot_in_ROOM(&p->room, &pev->dot) == _TRUE) 
          {
            if(is_thumb(p, &pev->dot) == _TRUE)
            {
              p->active     = _TRUE;
              p->active_dot = pev->dot;
              chx           = 1;  // changed
            }
            else if(is_inc(p, &pev->dot) == _TRUE)
            {
              if(p->pos < (p->width-1))
              {
                //Keyboard.TriggerEventStart(pev, _TIPCOMBI);
                p->pos   += 1;
                p->oldpos = p->pos; // changedmerker ist bereits gesetzt
                chx       = 1;  // changed
                retcode   = _TRUE;
              }
            }
            else if(is_dec(p, &pev->dot) == _TRUE)
            {
              if(p->pos > 0)
              {
                //Keyboard.TriggerEventStart(pev, _TIPCOMBI);
                p->pos   -= 1;
                p->oldpos = p->pos; // changedmerker ist bereits gesetzt
                chx       = 1;  // changed
                retcode   = _TRUE;
              }
            }
          }
          break;
      
        case _EVENT_HIDRELEASE :
          //Keyboard.TriggerEventStop(NULL);
          if(p->active == _TRUE)
          {
            // finalize
            chx       = 1;
            p->active = _FALSE;
          }
          break;
      
        case _EVENT_HIDMOVE :
          if(p->active == _TRUE)
          {
            if(dot_in_ROOM(&p->room, &pev->dot) == _TRUE)
            {
              tpos = p->pos;
              if(p->horizontal == _TRUE)
              {
                if(pev->dot.x != p->active_dot.x)
                {
                  p->active_dot.x = pev->dot.x;
                  chx       = 1;
                  tmp       = get_XX(p);
                  p->pos    = ((p->width - 1) * (pev->dot.x - p->room.xy1.x - B_WIDTH)) / tmp;
                  p->oldpos = p->pos; // changedmerker ist bereits gesetzt
                }
              }
              else if(pev->dot.y != p->active_dot.y)
              {
                p->active_dot.y = pev->dot.y;
                chx       = 1;
                tmp       = get_XX(p);
                p->pos    = ((p->width - 1) * (pev->dot.y - p->room.xy1.y - B_HEIGHT)) /  tmp;
                p->oldpos = p->pos; // changedmerker ist bereits gesetzt
              }
            
              if(p->pos < 0)
                p->pos = 0;
              else if(p->pos >= p->width)
                p->pos = p->width-1;
              
              retcode = (tpos != p->pos)? _TRUE : _FALSE;  
            }
          }
          break;
      }
    }
    
    if(chx != 0)
    {
      draw_SCROLLBAR(p);
    }
  }
  
  return retcode;
}


void move_to_index_CAROUSEL(_MOTION *pm, _UDWORD index);


void scrollbar_run_CAROUSEL(_MOTION *pm, _EVENT *pev)
{
  _BOOL active = pm->scrbar.active;
  pm->scrbar.width = pm->memkaru.no;
  
  if(run_SCROLLBAR(&pm->scrbar, pev) == _TRUE)
  {
    // position in scrollbar wurde geändert
    move_to_index_CAROUSEL(pm, pm->scrbar.pos);
  }

  if((active == _TRUE) && (pm->scrbar.active == _FALSE))
  {
    pm->swim = _TRUE;
  }
}

#define CARO_PERCENTAGE





#ifndef UC_OLD_MOTIONS

void negedge_KARU(_KARU *p, _BOOL state)
{
  if((p->onscreen == _TRUE)&&(p->gesamt == _TRUE))
  {
    if(p->SubPicture != NULL)
      p->SubPicture->DoAllNegEdge(state);
  }
}

void negedge_MEM_KARU(_MEM_KARU *p, _BOOL state)
{
  _UWORD i;
  
  for(i=0; i<p->no; i++)
    negedge_KARU(&p->ptr[i], state);
}

_BOOL start_KARU(_KARU *p)
{
  _BOOL retcode = _FALSE;
  
  if(p->SubPicture != NULL)
  {
    p->room.xy2.x -= p->room.xy1.x; // position wieder zurücksetzen
    p->room.xy2.y -= p->room.xy1.y;
    p->room.xy1.x = 0;
    p->room.xy1.y = 0;
  
    return _TRUE;
    //p->SubPicture->Free();
    //Memory2((void**)&p->SubPicture, 0);
  } 
    
  p->onscreen = _FALSE;
  p->gesamt   = _FALSE;
  
  if(load_object(&p->SubPicture, p->objno) == _TRUE)
  {
    p->SubPicture->Overload(&p->loader);
    p->room = p->SubPicture->GetPICTURE()->mask.room;
    retcode = _TRUE;
  }
    
  return retcode;
}

_BOOL start_CAROUSEL(_MOTION *pm)
{
  _BOOL     retcode = _TRUE;
  _UWORD    i;
  
  for(i=0; i<pm->memkaru.no; i++)
  {
    if(start_KARU(&pm->memkaru.ptr[i]) == _FALSE)
      retcode = _FALSE;
  }
  
  return retcode;
}

void xposition_KARU(_MOTION *pm, _UDWORD i, _ROOM *room, _KOORD *xoff, _ZOOM *zoom)
{
  _KARU *p = &pm->memkaru.ptr[i];

  if(p->SubPicture != NULL)
  {
    Zoom_ROOM(&p->room, &room->xy1, zoom);
    p->zoom = *zoom;
    
    p->room.xy1.x += *xoff;
    p->room.xy2.x += *xoff;

    _ATTRIB attr = pm->attrib & T_VERTBOUND;
    _KOORD  yoff = 0;
    
    if(attr == T_MIDBOUND)
      yoff = ((room->xy2.y - room->xy1.y + 1) - (p->room.xy2.y - p->room.xy1.y + 1)) / 2;
    else if(attr == T_DOWNBOUND)
      yoff = ((room->xy2.y - room->xy1.y + 1) - (p->room.xy2.y - p->room.xy1.y + 1));
    
    p->room.xy1.y += yoff;
    p->room.xy2.y += yoff;

    p->room0 = p->room;    
        
    *xoff += p->room.xy2.x - p->room.xy1.x + 1 + pm->gap;
  }
}

void yposition_KARU(_MOTION *pm, _UDWORD i, _ROOM *room, _KOORD *yoff, _ZOOM *zoom)
{
  _KARU *p = &pm->memkaru.ptr[i];
  
  if(p->SubPicture != NULL)
  {
    Zoom_ROOM(&p->room, &room->xy1, zoom);
    p->zoom = *zoom;
    
    p->room.xy1.y += *yoff;
    p->room.xy2.y += *yoff;

    _ATTRIB attr = pm->attrib & T_HORIZBOUND;
    _KOORD  xoff = 0;
    
    if(attr == T_CENTERBOUND)
      xoff = ((room->xy2.x - room->xy1.x + 1) - (p->room.xy2.x - p->room.xy1.x + 1)) / 2;
    else if(attr == T_RIGHTBOUND)
      xoff = ((room->xy2.x - room->xy1.x + 1) - (p->room.xy2.x - p->room.xy1.x + 1));

    p->room.xy1.x += xoff;
    p->room.xy2.x += xoff;
    
    p->room0 = p->room;    
    
    *yoff += p->room.xy2.y - p->room.xy1.y + 1 + pm->gap;
  }
}


_BOOL is_in_room(_DOT *pdot, _ROOM *room)
{
  if((pdot->x >= room->xy1.x) && (pdot->x <= room->xy2.x) && (pdot->y >= room->xy1.y) && (pdot->y <= room->xy2.y))
     return _TRUE;

  return _FALSE;
}

_DWORD get_pos_CAROUSEL(_VARLABEL *pv)
{
  if(pv->no == 0)
  {
    pv->no = 1;
    pv->info[0].state = CONST_VAL;
    pv->info[0].value = 0;
    return 0;
  }
  
  if(pv->info[0].state == CONST_VAL)
    return pv->info[0].value;

  _DWORD retcode = 0;
  if(VarList_GetValue(pv, &retcode, _TRUE) == _FALSE)
  {
    pv->no = 0;
    retcode = get_pos_CAROUSEL(pv);
  }
    
  return retcode;
}

void set_pos_CAROUSEL(_VARLABEL *pv, _DWORD val)
{
  if(pv->no == 0)
  {
    pv->no = 1;
    pv->info[0].state = CONST_VAL;
    pv->info[0].value = val;
    return;
  }
  
  if(pv->info[0].state == CONST_VAL)
  {
    pv->info[0].value = val;
    return;
  }
  
  if(VarList_SetValue(pv, val) == _FALSE)
  {
    pv->no = 0;
    set_pos_CAROUSEL(pv, val);
  }
}

void set_index_CAROUSEL(_MOTION *pm, _UDWORD index)
{
  pm->memoindex = index;
  pm->scrbar.pos = index;
  set_pos_CAROUSEL(&pm->varindex, index);
}

void move_to_index_CAROUSEL(_MOTION *pm, _UDWORD index)
{
  _DWORD  newpos = 0;
  _UDWORD i;
  _KARU   *pk = pm->memkaru.ptr;
  
  if(pm->horizontal == _TRUE)
  {
    for(i=0; i<index; i++, pk++)
    {
      newpos += (pk->room.xy2.x - pk->room.xy1.x + 1) + pm->gap;
    }
    set_pos_CAROUSEL(&pm->varx, -newpos);
  }
  else
  {
    for(i=0; i<index; i++, pk++)
    {
      newpos += (pk->room.xy2.y - pk->room.xy1.y + 1) + pm->gap;
    }
    set_pos_CAROUSEL(&pm->vary, -newpos);
  }
}

void stop_move_CAROUSEL(_MOTION *pm)
{
  if(pm->movstate == _TRUE)
  {
    pm->movstate = _FALSE;
    pm->movbegin = _FALSE;
    pm->swim     = _TRUE;
    pm->swimtime = get_abstime();
    
    if(pm->horizontal == _TRUE)
      pm->positiv_dir = (pm->movstart.x < pm->move.x)? _TRUE : _FALSE;
    else
      pm->positiv_dir = (pm->movstart.y < pm->move.y)? _TRUE : _FALSE;
    
  }
}


_UDWORD GetIndexOfFirst_CAROUSEL(_MOTION *p)
{
  _KARU   *pk;
  _UDWORD i, retcode = 0xFFFFFFFF; 
  _DWORD  oldpos = 2147483600;
  
  if(p->memkaru.no > 1)
  {
    if(p->horizontal == _TRUE)
    {
      for(i=0, pk=p->memkaru.ptr; i<p->memkaru.no; i++, pk++)
      {
        if(pk->gesamt == _TRUE)
        {
          if(pk->room.xy1.x < oldpos)
          {
            oldpos  = pk->room.xy1.x;
            retcode = i;
          }
        }
      }
    }
    else
    {
      for(i=0, pk=p->memkaru.ptr; i<p->memkaru.no; i++, pk++)
      {
        if(pk->gesamt == _TRUE)
        {
          if(pk->room.xy1.y < oldpos)
          {
            oldpos  = pk->room.xy1.y;
            retcode = i;
          }
        }
      }
    }
  }
  
  return retcode;
}


_WORD get_abspos(_MOTION *p, _UDWORD idx)
{
  _UDWORD i;
  _WORD   retcode = 0;

  if(idx < p->memkaru.no)
  {
    if(p->horizontal == _TRUE)
    {
      for(i=0; i<idx; i++)
      {
        retcode += p->memkaru.ptr[i].room.xy2.x - p->memkaru.ptr[i].room.xy1.x + 1 + p->gap;
      }
    }
    else
    {
      for(i=0; i<idx; i++)
      {
        retcode += p->memkaru.ptr[i].room.xy2.y - p->memkaru.ptr[i].room.xy1.y + 1 + p->gap;
      }
    }
  }
  
  return retcode;
}

void dec_CAROUSEL(_MOTION *p)
{
  // wurde wegen sa28455 auf absolute position (ohne swim) umgebaut
  _UDWORD   idx = GetIndexOfFirst_CAROUSEL(p);
  _VARLABEL *vl = (p->horizontal == _TRUE)? &p->varx : &p->vary;
  
  if(idx < p->memkaru.no)
  {
    if(idx >= (p->memkaru.no - 1))
    {
      if(p->infinite == _TRUE)
        idx = 0;
    }
    else
    {
      idx += 1;
    }
    
    set_pos_CAROUSEL(vl, -get_abspos(p, idx));
    p->swim = _TRUE; // notwendig wegen ende von nicht-infinite carousell
  }
}

void inc_CAROUSEL(_MOTION *p)
{
  // wurde wegen sa28455 auf absolute position (ohne swim) umgebaut
  _UDWORD   idx = GetIndexOfFirst_CAROUSEL(p);
  _VARLABEL *vl = (p->horizontal == _TRUE)? &p->varx : &p->vary;
  
  if(idx < p->memkaru.no)
  {
    if(idx == 0)
    {
      if(p->infinite == _TRUE)
        idx = p->memkaru.no - 1;
    }
    else
    {
      idx -= 1;
    }

    set_pos_CAROUSEL(vl, -get_abspos(p, idx));
    p->swim = _TRUE; // notwendig wegen ende von nicht-infinite carousell
  }
}

#define CAROUSEL_MINDELTAMOVE 20
void action_CAROUSEL(_MOTION *pm, _EVENT *pe)
{

  scrollbar_run_CAROUSEL(pm, pe);

 #ifdef USER_HERZ
  if(pe->ftype == _EVENT_HIDPRESS)
 #else
  if(pe->ftype == _EVENT_PREHIDPRESS)
 #endif
  {  
    if(is_in_room(&pe->dot, &pm->wiperoom) == _TRUE)
    {
      pm->movstate   = _TRUE;
      pm->movbegin   = _TRUE;
      pm->move       = pe->dot;
      pm->movstart   = pe->dot;
     #ifdef USER_HERZ
      pm->swim       = _FALSE;
     #endif 
    }
  }
  else if(pe->ftype == _EVENT_HIDRELEASE)
  {
    stop_move_CAROUSEL(pm);
  }
  else if(pe->ftype == _EVENT_HIDMOVE)
  {
   #ifdef USER_HERZ
    if(pe->lrm == _FALSE) 
   #endif
  
    if(pm->movstate == _TRUE)
    {
      if(is_in_room(&pe->dot, &pm->wiperoom) == _TRUE)
      {
        if(pm->horizontal == _TRUE)
        {
          if(pm->movbegin == _TRUE)
          {
            // erst wenn bewegung grösser als CAROUSEL_MINDELTAMOVE festgestellt wurde, wird diese auch ausgeführt
            _KOORD xx = pe->dot.x - pm->movstart.x;
            if((xx < -CAROUSEL_MINDELTAMOVE) || (xx > CAROUSEL_MINDELTAMOVE))
            {
              pm->movbegin = _FALSE;
            }
          }
        
          if(pm->movbegin == _FALSE)
          {
            _DWORD delta = pe->dot.x - pm->move.x;
            delta = (delta * (_DWORD)pm->movespeed) / 10;
            
            _DWORD newpos = get_pos_CAROUSEL(&pm->varx) + delta;
            if((pm->infinite == _FALSE) && (pm->overmove == _FALSE))
            {
              if(newpos <= -(pm->karuwidth - pm->widthlastpage))
                newpos = -(pm->karuwidth - pm->widthlastpage);
              if(newpos > 0)
                newpos = 0;
            }
            set_pos_CAROUSEL(&pm->varx, newpos);//get_pos_CAROUSEL(&pm->varx) + delta);
            pm->move = pe->dot;
            pm->swim = _FALSE;
          }
        }
        else
        {
          if(pm->movbegin == _TRUE)
          {
            // erst wenn bewegung grösser als CAROUSEL_MINDELTAMOVE festgestellt wurde, wird diese auch ausgeführt
            _KOORD yy = pe->dot.y - pm->movstart.y;
            if((yy < -CAROUSEL_MINDELTAMOVE) || (yy > CAROUSEL_MINDELTAMOVE))
            {
              pm->movbegin = _FALSE;
            }
          }
        
          if(pm->movbegin == _FALSE)
          {
            _DWORD delta = pe->dot.y - pm->move.y;
            delta = (delta * (_DWORD)pm->movespeed) / 10;
            _DWORD newpos = get_pos_CAROUSEL(&pm->vary) + delta;
            if((pm->infinite == _FALSE) && (pm->overmove == _FALSE))
            {
              if(newpos <= -(pm->karuwidth - pm->widthlastpage))
                newpos = -(pm->karuwidth - pm->widthlastpage);
              if(newpos > 0)
                newpos = 0;
            }
            set_pos_CAROUSEL(&pm->vary, newpos);
            pm->move = pe->dot;
            pm->swim = _FALSE;
          }
        }
      }
      else
      {
        stop_move_CAROUSEL(pm);
      }
    }
  }
  else if(pe->ftype == _EVENT_KEYPRESS)
  {
    if(pe->scancode == _CAROUSELINC)
      inc_CAROUSEL(pm);
    else if(pe->scancode == _CAROUSELDEC)
      dec_CAROUSEL(pm);
  }
}

void position_CAROUSEL(_MOTION *pm, _ROOM *room, _ZOOM *zoom)
{
  _UWORD    i;
  _KOORD    xypos = 0;
  _MEM_KARU *p = &pm->memkaru;
  
  if(pm->horizontal == _TRUE)
  {
    for(i=0; i<p->no; i++)
      xposition_KARU(pm, i, room, &xypos, zoom);

    pm->widthlastpage = 0; // breite der letzten möglichen seite, nur wenn infinite = false
    if(pm->infinite == _TRUE)  
    {  
      _UDWORD xx = p->ptr->room.xy2.x - p->ptr->room.xy1.x + 1 + room->xy2.x - room->xy1.x + 1;
      if(xypos <= xx)
        pm->infinite = _FALSE;
    }
    else if(pm->overmove == _FALSE)
    {
      _UDWORD xx = room->xy2.x - room->xy1.x + 1;
      _UDWORD no = p->no;
      while(no)
      {
        no --;
        _KARU *pk = &p->ptr[no];
        _DWORD width = pk->room.xy2.x - pk->room.xy1.x + 1;
        if((pm->widthlastpage + width) < xx)
          pm->widthlastpage += (width + pm->gap);
        else
          no = 0;
      }
    }
  }
  else
  {
    for(i=0; i<p->no; i++)
      yposition_KARU(pm, i, room, &xypos, zoom);

    if(pm->infinite == _TRUE)  
    {  
      _UDWORD yy = p->ptr->room.xy2.y - p->ptr->room.xy1.y + 1 + room->xy2.y - room->xy1.y + 1;
      if(xypos <= yy)
        pm->infinite = _FALSE;
    }
    else if(pm->overmove == _FALSE)
    {
      _UDWORD yy = room->xy2.y - room->xy1.y + 1;
      _UDWORD no = p->no;
      while(no)
      {
        no --;
        _KARU *pk = &p->ptr[no];
        _DWORD hh = pk->room.xy2.y - pk->room.xy1.y + 1;
        if((pm->widthlastpage + hh) < yy)
          pm->widthlastpage += (hh + pm->gap);
        else
          no = 0;
      }
    }
  }

  pm->karuwidth = xypos;  
}


void draw_KARU(_KARU *p, _DOT *poffs, _ROOM *room, _Picture *Ppic, _BOOL horizontal)
{
  _BOOL g = p->gesamt;
 #ifdef SA35492
  _BOOL onscreen = p->onscreen;
 #endif // SA35492
 
  p->onscreen = _FALSE;
  p->gesamt   = _FALSE;
  
  if(p->SubPicture != NULL)
  {
    _ROOM tmp = p->room;
  
    _UDWORD st = 0;
    if(horizontal)
    {
      if((tmp.xy2.x >= room->xy1.x)&&(tmp.xy1.x <= room->xy2.x))
        st = 1;
    }
    else
    {
      if((tmp.xy2.y >= room->xy1.y)&&(tmp.xy1.y <= room->xy2.y))
        st = 1;
    }
    
    if(st)
    {
      if(DrawBackGround == 0)
        p->SubPicture->KillBackGroundImage();
      p->SubPicture->DoAllNegEdge(_FALSE); // otto
//      KillAllSliderFocus();
      p->SubPicture->Draw(&tmp.xy1, &p->zoom, Ppic);
      p->onscreen = _TRUE;
        
      if((tmp.xy1.x >= room->xy1.x) && (tmp.xy2.x <= room->xy2.x) && (tmp.xy1.y >= room->xy1.y) && (tmp.xy2.y <= room->xy2.y))
        p->gesamt = _TRUE; // ganzes objekt ist am bildschirm
        
      return;
    }
    
    if(g == _TRUE)
      p->SubPicture->DoAllNegEdge(_FALSE);
  }
  
 #ifdef SA35492
  if((p->onscreen == _FALSE)&&(onscreen == _TRUE))
  {
    if(p->SubPicture != NULL)
    {
      // wenn objekt nicht mehr am bildschirm sichtbar ist, alle backgroundimages wieder freigeben (speicherreduktion)
      p->SubPicture->KillBackGroundImage();
    }
  }
 #endif // SA35492
}

#ifdef SA35235
_UDWORD estimated_index_horizontal(_MOTION *pm)
{
  _UWORD i;
  _MEM_KARU *p = &pm->memkaru;
  _KARU *pk = p->ptr;
 
  for(i=0; i<p->no; i++, pk++)
  {
//    if((pk->room.xy1.x <= pm->wiperoom.xy1.x) && (pk->room.xy2.x > pm->wiperoom.xy1.x))
    if((pk->room.xy1.x <= pm->wiperoom.xy1.x) && ((pk->room.xy2.x + pm->gap) >= pm->wiperoom.xy1.x)) // HubChr: 27.06.2018
    {
      _DWORD xl = pm->wiperoom.xy1.x - pk->room.xy1.x;
      _DWORD xx = pk->room.xy2.x - pk->room.xy1.x + 1;
      
      if(xl > ((xx * pm->skip_percent)/100))
      {
        return i+1;
      }
      return i;
    }
  }
  
  return 0xFFFFFFFF;
}

_UDWORD estimated_index_vertical(_MOTION *pm)
{
  _UWORD i;
  _MEM_KARU *p = &pm->memkaru;
  _KARU *pk = p->ptr;
 
  for(i=0; i<p->no; i++, pk++)
  {
//    if((pk->room.xy1.y <= pm->wiperoom.xy1.y) && (pk->room.xy2.y > pm->wiperoom.xy1.y))
    if((pk->room.xy1.y <= pm->wiperoom.xy1.y) && ((pk->room.xy2.y + pm->gap) >= pm->wiperoom.xy1.y)) // HubChr: 27.06.2018
    {
      _DWORD yo = pm->wiperoom.xy1.y - pk->room.xy1.y;
      _DWORD yy = pk->room.xy2.y - pk->room.xy1.y + 1;
      
      if(yo > ((yy* pm->skip_percent)/100))
      {
        return i+1;
      }
      return i;
    }
  }
  
  return 0xFFFFFFFF;
}

_UDWORD estimated_index(_MOTION *pm)
{
  if(pm->horizontal == _TRUE)
  {
    return estimated_index_horizontal(pm);
  }
  else
  {
    return estimated_index_vertical(pm);
  }
  
  return 0xFFFFFFFF;
}
#endif // SA35235

void draw_CAROUSEL(_MOTION *pm, _DOT *poffs, _Picture *Ppic)
{
  _UWORD i;
  _MEM_KARU *p = &pm->memkaru;
  _BOOL done = _FALSE;
 
 #ifdef SA35235
  if(pm->movstate == _TRUE)
  {
    _DWORD idx = estimated_index(pm); // während moven den zu erwartenden index ermitteln und setzen
    if(idx < p->no)
    {
      set_index_CAROUSEL(pm, idx);
      done = _TRUE;
    }
  }
 #endif //SA35235
 
  for(i=0; i<p->no; i++)
  {
    draw_KARU(&p->ptr[i], poffs, &pm->image.room, Ppic, pm->horizontal);
    if((done == _FALSE) && (p->ptr[i].gesamt == _TRUE))
    {
      set_index_CAROUSEL(pm, i);
      done = _TRUE;
    }
  }
}

void xmove_CAROUSEL(_MOTION *pm, _UWORD i, _KOORD xpos)
{
  _KARU *p = &pm->memkaru.ptr[i];

  p->room.xy1.x = p->room0.xy1.x + xpos;
  p->room.xy2.x = p->room0.xy2.x + xpos;
  
  if((pm->infinite == _TRUE) && (pm->karuwidth > 0))
  {
    if(p->room.xy2.x >= (pm->karuwidth + pm->wiperoom.xy1.x))
    {
      p->room.xy1.x -= pm->karuwidth;
      p->room.xy2.x -= pm->karuwidth;
    }
    else if(p->room.xy1.x < (pm->wiperoom.xy2.x - pm->karuwidth))
    {
      p->room.xy1.x += pm->karuwidth;
      p->room.xy2.x += pm->karuwidth;
    }
  }
}

void ymove_CAROUSEL(_MOTION *pm, _UWORD i, _KOORD ypos)
{
  _KARU *p = &pm->memkaru.ptr[i];

  p->room.xy1.y = p->room0.xy1.y + ypos;
  p->room.xy2.y = p->room0.xy2.y + ypos;
  
  if((pm->infinite == _TRUE) && (pm->karuwidth > 0))
  {
    if(p->room.xy2.y >= (pm->karuwidth + pm->wiperoom.xy1.y))
    {
      p->room.xy1.y -= pm->karuwidth;
      p->room.xy2.y -= pm->karuwidth;
    }
    else if(p->room.xy1.y < (pm->wiperoom.xy2.y - pm->karuwidth))
    {
      p->room.xy1.y += pm->karuwidth;
      p->room.xy2.y += pm->karuwidth;
    }
  }
}


void move_CAROUSEL(_MOTION *pm, _KOORD xpos, _KOORD ypos)
{
  _UWORD i;
  _MEM_KARU *p = &pm->memkaru;

  if(pm->horizontal == _TRUE)
  {
    if((pm->karuwidth > 0) && (pm->infinite == _TRUE))
      xpos = xpos % pm->karuwidth;
      
    for(i=0; i<p->no; i++)
      xmove_CAROUSEL(pm, i, xpos);
  }
  else
  {
    if((pm->karuwidth > 0) && (pm->infinite == _TRUE))
      ypos = ypos % pm->karuwidth;
      
    for(i=0; i<p->no; i++)
      ymove_CAROUSEL(pm, i, ypos);
  }
}

void kill_editor_MEM_KARU(_ROOM *room, _Picture *Ppic)
{
  if(Ppic != NULL)
  {
    if(Ppic->IsEditorOpen() == _TRUE)
    {
      _EDITOR *pedi = Ppic->GetEditor();
      if((pedi->input.io.space.xy2.y >= room->xy1.y)&&(pedi->input.io.space.xy1.y <= room->xy2.y)&& 
         (pedi->input.io.space.xy2.x >= room->xy1.x)&&(pedi->input.io.space.xy1.x <= room->xy2.x))
      {
        //Ppic->DoAllNegEdge(_TRUE);
        Ppic->CloseEditor();
      }
    }
  }
}

_BOOL refresh_KARU(_UWORD actno, _KARU *p, _Picture *Ppic)
{
  _BOOL retcode = _FALSE;

  if(p->onscreen == _TRUE)
  {
    _DOT dot = p->room.xy1;
    retcode = p->SubPicture->Refresh(actno, &dot, &p->zoom, Ppic);
  }
  
  return retcode;
}

_BOOL refresh_MEM_KARU(_UWORD actno, _MEM_KARU *p, _Picture *Ppic)
{
  _UWORD i;
  _BOOL retcode = _FALSE; 
  
  for(i=0; i<p->no; i++)
  {
    if(refresh_KARU(actno, &p->ptr[i], Ppic) == _TRUE)
      retcode = _TRUE;
  }
  
  return retcode;
}

void xswim_KARU(_MOTION *p, _UDWORD i)
{
  _KOORD dxx;
  _KARU *pk;

  if(i < p->memkaru.no)
  {
    pk  = &p->memkaru.ptr[i]; 
    dxx = pk->room.xy1.x - p->wiperoom.xy1.x;
    if((dxx <= -2) || (dxx >= 2))
      dxx = dxx / 2;
    
    dxx = (get_pos_CAROUSEL(&p->varx) - dxx);
    if(p->infinite == _TRUE)
       dxx = dxx % p->karuwidth;
    
    set_pos_CAROUSEL(&p->varx, dxx);
  }
}

void yswim_KARU(_MOTION *p, _UDWORD i)
{
  _KOORD dyy;
  _KARU *pk;

  if(i < p->memkaru.no)
  {
    pk  = &p->memkaru.ptr[i]; 
    dyy = pk->room.xy1.y - p->wiperoom.xy1.y;
    if((dyy <= -2) || (dyy >= 2))
      dyy = dyy / 2;
       
    dyy = (get_pos_CAROUSEL(&p->vary) - dyy);
    if(p->infinite == _TRUE)
      dyy = dyy % p->karuwidth;
      
    set_pos_CAROUSEL(&p->vary, dyy);
  }
}

_UDWORD x_getmaxmovno(_MOTION *p)
{
  _UDWORD retcode = p->maxmovno; 
  
  if(retcode == 0)
  {
    retcode = p->memkaru.no;
    
    if((p->infinite == _FALSE) && (retcode > 0))
    {  
      _DWORD xx = 0;
      _DWORD width = (p->wiperoom.xy2.x - p->wiperoom.xy1.x + 1);
      _UWORD no = p->memkaru.no;
      _KARU *pk = &p->memkaru.ptr[no-1];
      
      while(no)
      {
        xx += (pk->room.xy2.x - pk->room.xy1.x + 1);
        if(xx <= width)
          retcode = no;
        xx += p->gap;
        pk -= 1;
        no -= 1;
      }
    }
    
    p->maxmovno = retcode;
  }
  return retcode;
}

_UDWORD y_getmaxmovno(_MOTION *p)
{
  _UDWORD retcode = p->maxmovno; 
  
  if(retcode == 0)
  {
    retcode = p->memkaru.no;
    
    if((p->infinite == _FALSE) && (retcode > 0))
    {  
      _DWORD yy = 0;
      _DWORD height = (p->wiperoom.xy2.y - p->wiperoom.xy1.y + 1);
      _UWORD no = p->memkaru.no;
      _KARU *pk = &p->memkaru.ptr[no-1];
      
      while(no)
      {
        yy += (pk->room.xy2.y - pk->room.xy1.y + 1);
        if(yy <= height)
          retcode = no;
        yy += p->gap;
        pk -= 1;
        no -= 1;
      }
    }
    
    p->maxmovno = retcode;
  }
  return retcode;
}

void xswim_CAROUSEL(_MOTION *p)
{
  _UDWORD i, smalli, index;
  _KOORD  smallx;
  _KARU   *pk;
  _UDWORD anz = x_getmaxmovno(p); 
  
  smalli = 0;
  if(p->infinite == _TRUE)
  {
    smallx = 32000;
    for(i=0, pk=p->memkaru.ptr; i<p->memkaru.no; i++, pk++)
    {
      if(pk->room.xy1.x < smallx)
      {
        smallx = pk->room.xy1.x;
        smalli = i;
      }
    }
  }
  
  for(i=0; i<anz; i++) //p->memkaru.no; i++)
  {
    index = (smalli + i) % p->memkaru.no;
    pk    = &p->memkaru.ptr[index];

    if(pk->room.xy1.x == p->wiperoom.xy1.x)
    {
      p->swim = _FALSE;
      return;
    }
    else if(pk->room.xy2.x > p->wiperoom.xy1.x)
    {
      if(pk->room.xy1.x < p->wiperoom.xy1.x)
      {
#ifdef CARO_PERCENTAGE      
        _KOORD xl = p->wiperoom.xy1.x - pk->room.xy1.x;
        _DWORD xx = pk->room.xy2.x - pk->room.xy1.x + 1;
        
        if(p->positiv_dir == _FALSE)
        {
          if(xl >= (xx * p->skip_percent)/100)
            continue;
        }
        else
        {
          if(xl >= (xx * (100 - p->skip_percent))/100)
            continue;
        }
#else        
        _KOORD xr = pk->room.xy2.x - p->room.xy1.x;
        _KOORD xl = p->room.xy1.x - pk->room.xy1.x;
        
        if(xl > xr)
          continue;
#endif
      }
    
      xswim_KARU(p, index);
      return;
    }
  }
  
//  xswim_KARU(p, p->memkaru.no - 1);
  xswim_KARU(p, anz - 1);

}

void yswim_CAROUSEL(_MOTION *p)
{
  _UDWORD i, smalli, index;
  _KOORD  smally;
  _KARU   *pk;
  _UDWORD anz = y_getmaxmovno(p); 
  
  smalli = 0;
  if(p->infinite == _TRUE)
  {
    smally = 32000;
    for(i=0, pk=p->memkaru.ptr; i<p->memkaru.no; i++, pk++)
    {
      if(pk->room.xy1.y < smally)
      {
        smally = pk->room.xy1.y;
        smalli = i;
      }
    }
  }
    
  for(i=0; i<anz; i++) //p->memkaru.no; i++)
  {
    index = (smalli + i) % p->memkaru.no;
    pk    = &p->memkaru.ptr[index];

    if(pk->room.xy1.y == p->wiperoom.xy1.y)
    {
      p->swim = _FALSE;
      return;
    }
    else if(pk->room.xy2.y > p->wiperoom.xy1.y)
    {
      if(pk->room.xy1.y < p->wiperoom.xy1.y)
      {
#ifdef CARO_PERCENTAGE
        _KOORD yl = p->wiperoom.xy1.y - pk->room.xy1.y;
        _DWORD yy = pk->room.xy2.y - pk->room.xy1.y + 1;
        
        if(p->positiv_dir == _FALSE)
        {
          if(yl >= (yy * p->skip_percent)/100)
            continue;
        }
        else
        {
          if(yl >= (yy * (100 - p->skip_percent))/100)
            continue;
        }
      
#else
        _KOORD yr = pk->room.xy2.y - p->room.xy1.y;
        _KOORD yl = p->room.xy1.y - pk->room.xy1.y;
        
        if(yl > yr)
          continue;
#endif
      }
    
      yswim_KARU(p, index);
      return;
    }
  }
  
  //yswim_KARU(p, p->memkaru.no - 1);
  yswim_KARU(p, anz - 1);
}

void swim_CAROUSEL(_MOTION *p, _UDWORD swimtime)
{
  if((p->memkaru.no > 0) && (timediff(p->swimtime) >= swimtime))
  {
    p->swimtime = get_abstime();
    
    if(p->horizontal == _TRUE)
      xswim_CAROUSEL(p);
    else
      yswim_CAROUSEL(p);
  }
}

void swim_finish(_MOTION *p)
{
  if( p->swim == _TRUE)
  {
    for(_UDWORD i=0; i<100; i++)
    {
      swim_CAROUSEL(p, 0);
      if(p->swim == _FALSE)
      {
        return;
      }
    }
    p->swim = _FALSE;
  }
}

extern "C" void Restart_CAROUSEL(_MOTION *p)
{
  if(start_CAROUSEL(p) == _TRUE)
  {
    Keyboard.SetHidPrePress();
        
    _ZOOM tmpzoom;
    init_ZOOM(&tmpzoom);
    _ROOM room = p->wiperoom;
    linear_coordinates(&room.xy1.x, &room.xy1.y, &room.xy2.x, &room.xy2.y);
//    GetImage(&p->image, room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y);
        
    position_CAROUSEL(p, &room, &tmpzoom);
        
    p->workstate = STATE_READY; // zeichnen
    p->oldxy.x = 0;
    p->oldxy.y = 0;
    
    set_pos_CAROUSEL(&p->varx, 0);
    set_pos_CAROUSEL(&p->vary, 0);
  
    p->maxmovno = p->memkaru.no;
  
    InsertCmd(CMD_RECALC_IOID, 0, 0);
  }
  else
  {
    p->workstate = STATE_ERROR; // error
  }
}

#ifdef SA35235
static void pos_check_CAROUSEL(_MOTION *p) // sa35235
{
  // diese funktion überwacht ob auf "IndexVariable" etwas geschrieben wurde. wenn ja --> move carousel zum entsprechenden index
  _VARLABEL *pv = &p->varindex;
  
  if((pv->no != 0) && (pv->info[0].state == VAR_VAL))
  {
    _DWORD index = 0;
    if(VarList_GetValue(pv, &index, _FALSE) == _TRUE)
    {
      if(p->memoindex != index)
      {
        if(index >= p->memkaru.no)
        {
          index = p->memkaru.no - 1;
        }
        if(index < 0)
        {
          index = 0;
        }

        set_index_CAROUSEL(p, index);
        move_to_index_CAROUSEL(p, index); // move to index
      }
    }
  }
}
#endif // SA35235

_BOOL _SubPicture::Refresh_CAROUSEL(_UWORD actno, _MOTION *p, _DOT *offset, _ZOOM *zoom, _BOOL init, _Picture *Ppic, _UDWORD swimtime)
{
  // retcode wird für refresh verwendet und nicht für erfolg ob objekt ausgegeben
  // werden konnte.
  // retcode = _TRUE wenn refresh fertig oder objekt nit vorhanden, geladen etc.
	_BOOL   retcode = _TRUE;
  _BOOL   status = _FALSE;
	_DOT    dot;
  _ROOM   room;
  _ZOOM   tmpzoom;
  _UDWORD tmp;
  
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
      if(start_CAROUSEL(p) == _TRUE)
      {
        Keyboard.SetHidPrePress();
        
        tmpzoom   = p->zoom;
        tmpzoom.x = Zoom_ZOOM(zoom->x, tmpzoom.x);
        tmpzoom.y = Zoom_ZOOM(zoom->y, tmpzoom.y);
        
        room = p->wiperoom;
        Zoom_ROOM(&room, offset, zoom);
        linear_coordinates(&room.xy1.x, &room.xy1.y, &room.xy2.x, &room.xy2.y);
        //Rectangle(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY | T_SOLID, LIGHTGREEN); // test
      
        GetImage(&p->image, room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y);
        
        position_CAROUSEL(p, &room, &tmpzoom);
        
        p->workstate = STATE_READY; // zeichnen
        p->oldxy.x = 0;
        p->oldxy.y = 0;
      }
      else
      {
        p->workstate = STATE_ERROR; // error
      }
    }
   #ifdef SA40653
    else if(init == _TRUE)
    {
      if(p->workstate == STATE_RUN)
      {
        p->workstate = STATE_READY; // carousell nochmals zeichnen, wegen CMD_REDRAW
      }
    }
   #endif
   
    if((p->workstate != STATE_ERROR) && (p->workstate != STATE_START))
    {
      dot.x = get_pos_CAROUSEL(&p->varx);
      dot.y = get_pos_CAROUSEL(&p->vary);
      
      if(DrawBackGround != 0)
      {
         p->workstate = STATE_READY; // carousel neu ausgeben
      }
      else if((dot.x != p->oldxy.x) || (dot.y != p->oldxy.y))
      {
        kill_editor_MEM_KARU(&p->image.room, Ppic); // close editor wenn move
        move_CAROUSEL(p, dot.x, dot.y);
        p->workstate = STATE_READY; // carousel neu ausgeben weil position verändert
        p->oldxy = dot;
      }

//      _ROOM rr = p->image.room;
//      _ROOM rr = p->xroom;
      _ROOM rr = p->wiperoom;
//      rr.xy2.x -= 1;
//      rr.xy2.y -= 1;

      _UDWORD leveldraw = 0;
      _REGION actreg, ioreg=NULL, newreg=NULL;
                                
      if(p->workstate == STATE_READY)
      {
        actreg = ActivateAndRegion(&ioreg, &newreg, &rr);
        dot.x = Zoom(zoom->x, p->wiperoom.xy1.x) + offset->x;
        dot.y = Zoom(zoom->y, p->wiperoom.xy1.y) + offset->y;
       #ifdef UC_MOUSE
        HideMouseCursor();
       #endif
        ScreenStop();
        
        if(is_invisible(p->colback, p->attrib) == _TRUE)
          PutImage(&p->image, DEFKOORD, DEFKOORD);
        else
          DrawButton(&p->image.room, p->attrib, p->frametype, p->colback, p->colback);
        draw_CAROUSEL(p, &dot, Ppic);
        p->onscreen = _TRUE;
        ScreenStart();
       #ifdef UC_MOUSE
        ShowMouseCursor();
       #endif

       #ifdef UC_ROTATE
        RefreshRectangle(&p->image.room);
       #endif
        leveldraw = 1;
        p->workstate = STATE_RUN;
        
        DeactivateAndRegion(actreg, &ioreg, &newreg);
        
        draw_SCROLLBAR(&p->scrbar);
      }
      else 
      {
        actreg = ActivateAndRegion(&ioreg, &newreg, &rr);
        retcode = refresh_MEM_KARU(actno, &p->memkaru, Ppic);
        if(p->swim == _TRUE)
        {
          swim_CAROUSEL(p, swimtime);
        }
        
        DeactivateAndRegion(actreg, &ioreg, &newreg);
        
       #ifdef SA35235 
        pos_check_CAROUSEL(p); // sa35235
       #endif
      }
      
      scrollbar_run_CAROUSEL(p, NULL);
      
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
      //free_IMAGE(&p->image);
      //free_MEM_KARU(&p->memkaru);
      p->workstate = STATE_START;
    }
  }

	return(retcode);
}

void _SubPicture::FinishCarouselSwim(_UWORD actno, _DOT *po, _ZOOM *zoom, _Picture *Ppic)
{
  for(_UDWORD i=0; i<Picture.mask.memmotion.no; i++)
  {
    _MOTION *pm = &Picture.mask.memmotion.ptr[i];
    if(pm->memkaru.no > 0) // es ist ein karousell
    {
      if(pm->swim == _TRUE)
      {
        _UDWORD nox = 100;
        while((nox--) && (pm->swim == _TRUE))
        {        
          Refresh_CAROUSEL(actno, pm, po, zoom, _FALSE, Ppic, 0);
        }
      }
    }
  }
}


#endif
