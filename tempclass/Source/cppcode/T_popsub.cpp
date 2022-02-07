#include "T_IPC.H"
#include "T_NUC.H"

#ifdef OK_SUBLIST
_BOOL   add_entries_MEM_VKLIST_lse(_MEM_VKLIST*, _UDWORD);
_BOOL   add_entries_MEM_VKLIST(_MEM_VKLIST*, _UDWORD);
_UDWORD add_MEM_VKLIST(_MEM_VKLIST*, _UDWORD, _UDWORD);
_BOOL   del_MEM_VKLIST(_MEM_VKLIST*, _UDWORD);
_UDWORD find_real_position(_MEM_VKLIST*, _UDWORD);
_BOOL   action_MEM_VKLIST(_MEM_VKLIST*, _UDWORD);
_KOORD  out_MEM_VKLIST(_ROOM*, _VKLIST*, _COLOR, _BOOL, _MEM_VKLIST*);
void    sublist_run(_Window*, _PopUp*, _EVENT*);
void    sublist_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
_UDWORD popup_get_actual_position(_PopUp*);
_BOOL   is_action_MEM_VKLIST(_MEM_VKLIST*, _UDWORD);
_UDWORD find_line(_MEM_VKLIST*, _UDWORD);
_UDWORD get_parent_no(_MEM_VKLIST*, _UDWORD);


_VKLIST *find_last_VKLIST(_MEM_VKLIST *p, _UDWORD no, _UDWORD *entries)
{
  _VKLIST *retcode = &p->ptr[no];

  (*entries) += 1;

  while(retcode->next != DEF4BYTE)
  {
    (*entries) += 1;
    retcode = &p->ptr[retcode->next];
  }

  return(retcode);
}

_VKLIST *find_last_VKLIST(_MEM_VKLIST *p, _UDWORD no)
{
  _VKLIST *retcode = &p->ptr[no];

  while(retcode->next != DEF4BYTE)
    retcode = &p->ptr[retcode->next];

  return(retcode);
}

_VKLIST *find_jump_VKLIST(_MEM_VKLIST *p, _UDWORD no, _UDWORD jump, _UDWORD *entries)
{
  _VKLIST *retcode = &p->ptr[no];

  (*entries) += 1;

  while(retcode->next != jump)
  {
    (*entries) += 1;
    retcode = &p->ptr[retcode->next];
  }

//  if(retcode->next != jump)
//    retcode = find_jump_VKLIST(p, retcode->next, jump, entries);

  return(retcode);
}

_BOOL open_MEM_VKLIST(_MEM_VKLIST *p, _UDWORD no)
{
  _BOOL   retcode = _FALSE;
  _VKLIST *ptr;
  _UDWORD tmp, entries = 0;

  if(no < p->no)
  {
    ptr = &p->ptr[no];
    if(ptr->hide != DEF4BYTE)
    {
      tmp       = ptr->next;
      ptr->next = ptr->hide;
      ptr->hide = DEF4BYTE;
      ptr       = find_last_VKLIST(p, ptr->next, &entries);
      ptr->next = tmp;
      ptr->jump = tmp;
      retcode   = _TRUE;
    }

    p->visual_no += entries;
  }

  return(retcode);
}

_BOOL close_MEM_VKLIST(_MEM_VKLIST *p, _UDWORD no)
{
  _BOOL   retcode = _FALSE;
  _VKLIST *ptr;
  _UDWORD entries = 0;

  if(no < p->no)
  {
    ptr = &p->ptr[no];
    if((ptr->hide == DEF4BYTE)&&(ptr->jump != ptr->next))
    {
      ptr->hide = ptr->next;
      ptr->next = ptr->jump;
      ptr       = find_jump_VKLIST(p, ptr->hide, ptr->jump, &entries);
      ptr->next = DEF4BYTE;
      ptr->jump = DEF4BYTE;
      retcode   = _TRUE;
    }
    p->visual_no -= entries;
  }

  return(retcode);
}

void open_entire_MEM_VKLIST(_MEM_VKLIST *p)
{
  _UDWORD i;

  for(i=0; i<p->no; i++)
    open_MEM_VKLIST(p, i);
}

void close_entire_MEM_VKLIST(_MEM_VKLIST *p)
{
  _UDWORD i;

  i = p->no;
  while(i > 0)
  {
    i -= 1;
    close_MEM_VKLIST(p, i);
  }
}

_BOOL add_entries_MEM_VKLIST(_MEM_VKLIST *p, _UDWORD no)
{
  _BOOL retcode;

  PushMemColor(MEMORY_PRG);
  retcode = add_entries_MEM_VKLIST_lse(p, no);
  PopMemColor();

  return(retcode);
}

_BOOL add_entries_MEM_VKLIST_lse(_MEM_VKLIST *p, _UDWORD no)
{
  _BOOL   retcode = _FALSE;
  _UDWORD size = ((_UDWORD)p->done_no + no) * sizeof(_VKLIST);
  _VKLIST *ptr;

  if(Memory2((void**)&p->ptr, size) != 0)
  {
    retcode     = _TRUE;
    ptr         = &p->ptr[p->done_no];
    p->done_no += no;
    while(no)
    {
      init_VKLIST(ptr);
      ptr += 1;
      no --;
    }
  }

  return(retcode);
}

_UDWORD add_MEM_VKLIST(_MEM_VKLIST *p, _UDWORD parent, _UDWORD data)
{
  _VKLIST *ptr, *prev;
  _UDWORD retcode = DEF4BYTE;
  _BOOL   state = _TRUE;

  if(p->no >= p->done_no)
    state = add_entries_MEM_VKLIST(p, 1);

  if(state == _TRUE)
  {
    ptr = &p->ptr[p->no];
//    init_VKLIST(ptr);

    ptr->data  = data;
    ptr->no    = p->no;
    ptr->depth = 0;

    if(parent < p->no) // subelement
    {
      ptr->parent = parent;
      prev = &p->ptr[parent];
      ptr->depth = prev->depth + 1;
      if(prev->hide == DEF4BYTE)
      {
        prev->hide = p->no;
      }
      else
      {
        prev = find_last_VKLIST(p, prev->hide);
        prev->next = p->no;
        prev->jump = p->no;
      }
    }
    else
    {
      p->visual_no ++;
      if(p->no > 0) // nextelement
      {
        prev = find_last_VKLIST(p, 0);
        prev->next = p->no;
        prev->jump = p->no;
      }
    }

    retcode = p->no;
    p->no ++;
  }

  return(retcode);
}

void cvkg(_UDWORD *p, _UWORD start, _UDWORD a)
{
  if((*p != DEF4BYTE) && (*p >= start))
  {
    if(*p >= a)
      *p -= a;
  }
}

void cvk(_UDWORD *p, _UWORD start, _UDWORD a)
{
  if((*p != DEF4BYTE) && (*p > start))
  {
    if(*p >= a)
      *p -= a;
  }
}

_BOOL del_MEM_VKLIST(_MEM_VKLIST *p, _UDWORD index)
{
/*  
  _UDWORD i, anz = 1;
  _VKLIST *ps;
  _UDWORD size;

  if(index < p->no)
  {
    ps = &p->ptr[index];
  
    if(index <= (p->no - 1)) // es ist die letzte zeile
    {
      size = sizeof(_VKLIST) * (p->no - (index + 1));
      MemMove(&p->ptr[index], &p->ptr[index+1], size);
    }
  
    p->no -= 1;
  
    for(i=0, ps=p->ptr; i<p->no; i++, ps++)
    {
      cvk(&ps->hide,   index, anz);
      cvk(&ps->next,   index, anz);
      cvk(&ps->jump,   index, anz);
      cvk(&ps->no,     index, anz);
      cvk(&ps->parent, index, anz);
    }
    
    p->visual_no -= 1;
    
    return true;
  }
*/
  return false;
}

/*
_BOOL del_MEM_VKLIST(_MEM_VKLIST *p, _UDWORD no)
{
  _BOOL   retcode = _TRUE;
  _VKLIST *pvk;
  _UDWORD i;

  if(no < p->no)
  {
    if(retcode == _TRUE)
    {
      retcode = _FALSE;
      pvk = &p->ptr[0];
      for(i=0; i<p->no; i++, pvk++)
      {

        if((pvk->next == no) && ((pvk->depth < p->ptr[no].depth) || (pvk->depth == p->ptr[no].depth)))
        {
          pvk->next = p->ptr[no].next;
          pvk->jump = p->ptr[no].next;
          retcode = _TRUE;
          break;
        }
        else if((pvk->jump == no) && (pvk->depth > p->ptr[no].depth))
        {
          pvk->jump = p->ptr[no].jump;
          retcode = _TRUE;
          break;
        }
      }
    }


    // last line ??
    if(no == (p->no-1))
    {
      p->ptr[p->no-1].hide = DEFSCOPE;
      p->ptr[p->no-1].next = DEFSCOPE;
      p->ptr[p->no-1].jump = DEFSCOPE;
    }

    // the previously element was found
    if(retcode == _TRUE)
    {
      free_VKLIST(&p->ptr[no]);
      p->no--;
      p->done_no--;
      p->visual_no--;
    }

  }


  return(retcode);
}
*/
_UDWORD find_real_position(_MEM_VKLIST *p, _UDWORD no)
{
  _VKLIST *ptr;
  _UDWORD retcode = DEF4BYTE;

  if(no < p->no)
  {
    ptr = p->ptr;
    while(no)
    {
      no--;
      if(ptr->next < p->no)
      {
        ptr = &p->ptr[ptr->next];
      }
      else
      {
        no  = 0;
        ptr = NULL;
      }
    }

    if(ptr != NULL)
      retcode = ptr->no;
  }

  return(retcode);
}

_BOOL action_MEM_VKLIST(_MEM_VKLIST *p, _UDWORD no)
{
  _BOOL retcode;

  no = find_real_position(p, no);

  retcode = open_MEM_VKLIST(p, no);
  if(retcode == _FALSE)
    retcode = close_MEM_VKLIST(p, no);

  return(retcode);
}

_UDWORD find_line_position(_MEM_VKLIST *p, _UDWORD no)
{
  // funktion findet anhand von absoluter zeile die in liste dargestellte zeilennummer
  _VKLIST *ptr;
  _UDWORD retcode = 0;

  if(no < p->no)
  {
    ptr = p->ptr;
    do
    {
      if(ptr->no == no)
        return(retcode);

      retcode += 1;
      if(ptr->next < p->no)
        ptr = &p->ptr[ptr->next];
      else
        ptr = NULL;
    }
    while(ptr != NULL);
  }

  return(DEF4BYTE);
}

_BOOL find_line_sub(_MEM_VKLIST *p, _UDWORD no)
{
  _BOOL retcode = _FALSE;

  if(no < p->no)
  {
    find_line_sub(p, p->ptr[no].parent);
    // aufruf von open_MEM_VKLIST() unbedingt von aussen nach innen abarbeiten
    // (rekursion vorher), ansonsten stimmt anzahl der sichtbaren elemente nicht
    open_MEM_VKLIST(p, no);
    retcode = _TRUE;
  }

  return(retcode);
}

_UDWORD find_line(_MEM_VKLIST *p, _UDWORD no)
{
  // diese funktion öffnet alle übergeordneten einträge um eine bestimmte zeile anzeigen zu können
  // die bestimmte zeile wird absolut (position im array) angegeben
  // der returncode entspricht der zeilennummer welche im scrollmenue verwendet wird
  _UDWORD retcode = DEF4BYTE;

  if(no < p->no)
  {
    find_line_sub(p, no);
    retcode = find_line_position(p, no);
  }

  return(retcode);
}


_UDWORD width_lineinfo(_VKLIST *ptr)
{
  return((ptr->depth + 2) * 10);
}

_KOORD out_MEM_VKLIST(_ROOM *r, _VKLIST *ptr, _COLOR col, _BOOL lastline, _MEM_VKLIST *pmvk)
{
  _UDWORD i, sign = 0;
  _KOORD  x, x0, y0, y, ye;

  y  = (r->xy1.y + r->xy2.y) / 2;
  x  = r->xy1.x;
  ye = (lastline == _TRUE)? y : r->xy2.y;

  // vertical lines
  for(i=0; i<ptr->depth; i++)
  {
    x += 10;
    Line(x, r->xy1.y, x, ye, T_DOTTED|T_COPY, col);
  }

  if(ptr->next < pmvk->no)
    if(pmvk->ptr[ptr->next].depth < ptr->depth)
      ye = y;

  x += 10;
  Line(x, r->xy1.y, x, ye, T_DOTTED|T_COPY, col);

  // horizontal line
  Line((lastline == _TRUE)? r->xy1.x+10 : x, y, x+10, y, T_DOTTED|T_COPY, col);

  if(ptr->hide != DEF4BYTE)
    sign = '+';
  else if(ptr->next != ptr->jump)
    sign = '-';

  if(sign != 0)
  {
    x0 = x-5;
    y0 = y-5;
    Bar(x0, y0, x0+9, y0+9, T_SOLID|T_COPY, MakeColor_Makro(GetBackColor_Makro(col), GetBackColor_Makro(col)));
    Rectangle(x0, y0, x0+9, y0+9, T_DOTTED|T_COPY, col);

    Line(x0+2, y0+4, x0+7, y0+4, T_SOLID|T_COPY, col);
    Line(x0+2, y0+5, x0+7, y0+5, T_SOLID|T_COPY, col);

    if(sign == '+')
    {
      Line(x0+4, y0+2, x0+4, y0+7, T_SOLID|T_COPY, col);
      Line(x0+5, y0+2, x0+5, y0+7, T_SOLID|T_COPY, col);
    }
  }

  x += 10;

  return(x);
}

void sublist_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _UDWORD     no;
  _MEM_VKLIST *pmvk;
  _MENU       *pm;

  pm = PopUp->GetMenu();
  if(pm->pointer.PtrRun != NULL)
    pm->pointer.PtrRun(Window, PopUp, pev);

  if((pev->ftype == _EVENT_KEYPRESS) && (pev->scancode == _ENTER))
  {
    pmvk = (_MEM_VKLIST*)pm->pointer.PtrSubList;
    no   = pm->position.pos;
    if(action_MEM_VKLIST(pmvk, no) == _TRUE)
    {
      pm->position.no = pmvk->visual_no;  // correct no of lines
      PopUp->OutPage(no, _TRUE);                 // out page

      pev->ftype = _EVENT_NONE;   // eat event
    }
  }
}

void sublist_line(_Window *Pw, _PopUp *Pp, _UDWORD no, _ROOM *r, _COLOR col, _BOOL background)
{
  _MENU       *pm;
  _VKLIST     *ptr;
  _MEM_VKLIST *pmvk;
  _ROOM       room;
  _BOOL       lastline;

  pm   = Pp->GetMenu();
  pmvk = (_MEM_VKLIST*)pm->pointer.PtrSubList;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  lastline = ((no+1) == (_UDWORD)pm->position.no)? _TRUE : _FALSE;
  no       = find_real_position(pmvk, no);

  if(no < pmvk->no)
  {
    ptr  = &pmvk->ptr[no];
    room = *r;
    room.xy1.x = out_MEM_VKLIST(r, ptr, col, lastline, pmvk);

    if(pm->pointer.PtrLine != NULL)
      pm->pointer.PtrLine(Pw, Pp, ptr->data, &room, col, _FALSE);
  }
}

_BOOL is_action_MEM_VKLIST(_MEM_VKLIST *pmvk, _UDWORD no)
{
  _UDWORD lno;
  _VKLIST *pvk;
  _BOOL   retcode = _FALSE;

  if(pmvk != NULL)
  {
    lno = find_real_position(pmvk, no);
    if(lno < pmvk->no)
    {
      pvk = &pmvk->ptr[lno];
      if((pvk->hide != DEF4BYTE)||(pvk->jump != pvk->next))
        retcode = _TRUE;

    }
  }

  return(retcode);
}

#endif

_UDWORD popup_get_actual_position(_PopUp *PopUp)
{
  _MENU *pm;

  pm = PopUp->GetMenu();

 #ifdef OK_SUBLIST
  _MEM_VKLIST *pmvk;
  _UDWORD     retcode = 0;
  _UDWORD     no;

  pmvk = (_MEM_VKLIST*)pm->pointer.PtrSubList;
  if(pmvk != NULL)
  {
    no = find_real_position(pmvk, pm->position.pos);
    if(no < pmvk->no)
      retcode = pmvk->ptr[no].data;
  }
 #else
  retcode = pm->position.pos;
 #endif

  return(retcode);
}

_UDWORD get_parent_no(_MEM_VKLIST *p, _UDWORD no)
{
  _VKLIST *ptr;
  _UDWORD i;

  if(no < p->no)
  {
    for(i=0,ptr=p->ptr; i<p->no; i++,ptr++)
    {
      if(ptr->data == no)
        return(i);
    }
  }

  return(DEF4BYTE);
}
