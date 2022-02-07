#include "T_IPC.H"
#include "T_NUC.H"

_BOOL lookup_input_in_object(_IO*, _MY_PICTURE*);

/*
#ifdef ZOOM_KM
_ZOOM do_kmz(_ZOOM *pz, _ZOOM *kmz)
{
  _ZOOM retcode = *pz;

  if(retcode.x == DEFZOOM)
    retcode.x = 1000;
  if(retcode.y == DEFZOOM)
    retcode.y = 1000;

  retcode.x = (_WORD)((((_DWORD)retcode.x * (_DWORD)kmz->x))/1000);
  retcode.y = (_WORD)((((_DWORD)retcode.y * (_DWORD)kmz->y))/1000);

  return(retcode);
}
#endif
*/


/*
dises funktion berechnet ob alter und neuer editor die selbe gruppennummer (summary) besitzen
in diesem fall müsste die gruppierung nicht neu ausgegeben werden (flackern)
_BOOL is_same_summary(_MY_PICTURE *p, _UWORD no1, _UWORD no2, _UWORD sum1, _UWORD sum2)
{
  _BOOL        retcode = _FALSE;
  _SEEKELEMENT *ps1, *ps2;
  _SINGLESEEK  *s1, *s2;
  _UWORD       no;

  if(sum1 != DEFSUMMARY)
  {
    sum1 = sum1 & 0x7FFF;
    sum2 = sum2 & 0x7FFF;

    if(sum1 == sum2)
    {
      if((no1 < p->memseekelement.no)&&(no2 < p->memseekelement.no))
      {
        ps1 = &p->memseekelement.ptr[no1];
        ps2 = &p->memseekelement.ptr[no2];

        if(ps1->no == ps2->no)
        {
          no = ps1->no;
          if(no > 0)
          {
            s1  = ps1->ptr;
            s2  = ps2->ptr;
            no -= 1;

            while(no)
            {
              if((s1->location != s2->location)||(s1->no != s2->no))
                return(_FALSE);

              s1 += 1;
              s2 += 1;
              no -= 1;
            }

            retcode = _TRUE;
          }
        }
      }
    }
  }

  return(retcode);
}
*/


/*
_ASCII *debug_sub(_ASCII *txt, _DIRECTION *p)
{
  *txt = '('; txt += 1;
  DToA(txt, p->h_next, 0x800, sizeof(_ASCII)); txt += StrLen(txt, sizeof(_ASCII));
  *txt = ':'; txt += 1;
  DToA(txt, p->v_next, 0x800, sizeof(_ASCII)); txt += StrLen(txt, sizeof(_ASCII));
  *txt = ')'; txt += 1;
  return(txt);
}

void debug(_SEEKELEMENT *p, _UWORD line)
{
  _UWORD i;
  _ASCII *txt, ptr[100];

  txt = ptr;

  txt = debug_sub(txt, &p->direction);
  *txt = '-'; txt += 1;

  for(i=0; i<p->no; i++)
  {
    switch(p->ptr[i].location)
    {
      case SL_LAYER  : *txt = 'L'; break;
      case SL_OBJECT : *txt = 'J'; break;
      case SL_MOTION : *txt = 'M'; break;
      case SL_INPUT  : *txt = 'I'; break;
      case SL_BUTTON : *txt = 'B'; break;
      case SL_OUTPUT : *txt = 'O'; break;
      default        : *txt = '?'; break;
    }

    txt += 1;
    *txt = ':'; txt += 1;

    DToA(txt, p->ptr[i].no, 0x800, sizeof(_ASCII)); txt += StrLen(txt, sizeof(_ASCII));
    txt = debug_sub(txt, &p->ptr[i].direction);
    *txt = ' '; txt += 1;
  }

  *txt = 0;
  Debug(ptr, sizeof(_ASCII), line);
}
*/

_BOOL    is_same_summary(_MY_PICTURE*, _UWORD, _UWORD, _UWORD, _UWORD);
_BOOL    position_SEEKELEMENT(_ROOM*, _MY_PICTURE*, _UWORD, _DOT*, _ZOOM*);
_BOOL    lookup_SEEKELEMENT(_MY_PICTURE*, _KOORD, _KOORD);
_BOOL    out_SEEKELEMENT(_MY_PICTURE*, _UWORD, _EDITSTATE, _DOT*, _ZOOM*);
_BOOL    load_EDITOR(_EDITOR*, _MY_PICTURE*, _UWORD);
_UWORD   layer_SEEKELEMENT(_MY_PICTURE*, _UWORD);
_BOOL    is_SEEKELEMENT(_MY_PICTURE*, _UWORD);
_BOOL    is_SEEKELEMENT_available(_MY_PICTURE*, _UWORD, _SELECTOR);
_BOOL    access_SEEKELEMENT(_MY_PICTURE*, _UWORD, _BOOL, _SELECTOR);
//_BOOL    summary_SEEKELEMENT(_MY_PICTURE*, _UWORD, _BOOL, _Picture*);
_BOOL    action_SEEKELEMENT(_MY_PICTURE*, _UWORD, _BOOL, _Window*, _BOOL);
_BOOL    hotkey_SEEKELEMENT(_MY_PICTURE*, _Window*, _BOOL, _EVENT);
_INPUT  *p_input_SEEKELEMENT(_MY_PICTURE*, _UWORD, _BOOL);
_BOOL    is_any_input_accessible(_MY_PICTURE*, _SELECTOR);
_BOOL    find_input_by_varno(_UWORD*, _MY_PICTURE*, _UDWORD);
void     new_IO_SEEKELEMENT(_MY_PICTURE*, _UWORD);
void     give_editorcolor(_IO*, _EDITSTATE);

// static

_BOOL  lookup_SEEKELEMENT_OUTPUT(_MEM_SEEKELEMENT*, _SubPicture*, _SEEKELEMENT*, _UWORD);
_BOOL  position_SEEKELEMENT_sub(_ROOM*, _SINGLESEEK*, _SubPicture*, _DOT*, _ZOOM*);
_BOOL  load_EDITOR_sub(_EDITOR*, _SINGLESEEK*, _SubPicture*);
_BOOL  out_SEEKELEMENT_sub(_ROOM*, _SINGLESEEK*, _SubPicture*, _EDITSTATE, _DOT*, _ZOOM*);
_INPUT *p_input_SEEKELEMENT_sub(_SINGLESEEK*, _SubPicture*, _BOOL);
_BOOL  summary_SEEKELEMENT_sub(_SINGLESEEK*, _SubPicture*, _BOOL, _DOT*, _ZOOM*, _Picture *PPic);
_BOOL  action_SEEKELEMENT_sub(_ROOM*, _SINGLESEEK*, _SubPicture*, _BOOL, _Window*, _DOT*, _ZOOM*, _BOOL);
_BOOL  access_SEEKELEMENT_sub(_SINGLESEEK*, _SubPicture*, _BOOL, _SELECTOR);
/*
void slurry(_MY_PICTURE *p)
{
  _UWORD i;
  _ROOM  room;
  _DOT   offset;
  _ZOOM  zoom;
  _INPUT *pi;
  _ASCII tmp[20];

  init_DOT(&offset);
  init_ZOOM(&zoom);

  Debug(p->memseekelement.no, 1);
  for(i=0; i<p->memseekelement.no; i++)
  {
    Debug(p->memseekelement.ptr[i].ioid, i+2);

    if(position_SEEKELEMENT(&room, p, i, &offset, &zoom) == _TRUE)
    {
      Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY|T_SOLID, LIGHTRED);
      pi = p_input_SEEKELEMENT(p, i, _TRUE);
      if(pi != NULL)
      {
        Debug(pi->memseekno, i+2, 1);
        DToA(tmp, pi->memseekno, 0x0800, 1);
        OutTextXY(room.xy1.x, room.xy1.y, tmp, T_COPY|T_SOLID, WHITE, 1);
      }
    }
  }
  Debug("                                    ", 1, i+2);
}

void slurry1(_MY_PICTURE *p)
{
  _UWORD       i, j;
  _SEEKELEMENT *ps;
  _ASCII txt[100];

  ps = p->memseekelement.ptr;
  for(i=0; i<p->memseekelement.no; i++)
  {
    DToA(txt, ps->direction.h_next, 0x0800, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), "-", sizeof(_ASCII));
    DToA(&txt[StrLen(txt, sizeof(_ASCII))], ps->direction.v_next, 0x0800, sizeof(_ASCII));
    StrCat(txt, sizeof(_ASCII), ":", sizeof(_ASCII));
    for(j=0; j<ps->no; j++)
    {
      if(j != 0)
        StrCat(txt, sizeof(_ASCII), ".", sizeof(_ASCII));
      DToA(&txt[StrLen(txt, sizeof(_ASCII))], ps->ptr[j].direction.h_next, 0x0800, sizeof(_ASCII));
    }

    Debug(txt, sizeof(_ASCII), i+1);
    ps ++;
  }
}
*/

_MEM_SEEKELEMENT *smse;
/*
_UWORD sort_SEEKELEMENT(_SEEKELEMENT *p0, _SEEKELEMENT *p1, _UWORD pos, _BOOL horizontal)
{
  _UWORD retcode = 0;
  _UWORD next0, next1;

  if((pos < p0->no)&&(pos < p1->no))
  {
    if(horizontal == _TRUE)
    {
      next0 = p0->ptr[pos].direction.h_next;
      next1 = p1->ptr[pos].direction.h_next;
    }
    else
    {
      next0 = p0->ptr[pos].direction.v_next;
      next1 = p1->ptr[pos].direction.v_next;
    }

    if(next0 == DEFNEXTNO)
      next0 = 0xFFFF;
    if(next1 == DEFNEXTNO)
      next1 = 0xFFFF;

    if(next0 == next1)
      retcode = sort_SEEKELEMENT(p0, p1, pos+1, horizontal);
//    else if((next0 == DEFNEXTNO) && (next1 != DEFNEXTNO))
//      retcode = _TRUE;
    else if(next0 > next1)
      retcode = 1;
  }

  return(retcode);
}
_UWORD sort_horizontal(_UWORD *p0, _UWORD *p1)
{
  return(sort_SEEKELEMENT(&smse->ptr[*p0], &smse->ptr[*p1], 0, _TRUE));
}
_UWORD sort_vertical(_UWORD *p0, _UWORD *p1)
{
  return(sort_SEEKELEMENT(&smse->ptr[*p0], &smse->ptr[*p1], 0, _FALSE));
}
*/

_BOOL lookup_input_in_object(_IO *pio, _MY_PICTURE *mp)
{
  _BOOL        retcode = _FALSE;
  _ROOM        room;
  _DOT         offset;

  if((pio != NULL)&&(mp != NULL))
  {
    if(mp->Editor.actno < mp->memseekelement.no)
    {
      init_DOT(&offset);
      if(position_SEEKELEMENT(&room, mp, mp->Editor.actno, &offset, &mp->PicZoom) == _TRUE)
      {
        if((room.xy1.x >= pio->space.xy1.x)&&(room.xy2.x <= pio->space.xy2.x)&&(room.xy1.y >= pio->space.xy1.y)&&(room.xy2.y <= pio->space.xy2.y))
        {
          // editor befindet sich innerhalb position pio->space 
          retcode = _TRUE;
        }
      }
    }
  }

  return(retcode);
}

_WORD sort_SEEKELEMENT(_SEEKELEMENT *p0, _SEEKELEMENT *p1, _UWORD pos, _BOOL horizontal)
{
  _WORD  retcode = 0;
  _UWORD next0, next1;

  if((pos < p0->no)&&(pos < p1->no))
  {
    if(horizontal == _TRUE)
    {
      next0 = p0->ptr[pos].direction.h_next;
      next1 = p1->ptr[pos].direction.h_next;
    }
    else
    {
      next0 = p0->ptr[pos].direction.v_next;
      next1 = p1->ptr[pos].direction.v_next;
    }

    if(next0 == DEFNEXTNO)
      next0 = 0xFFFF;
    if(next1 == DEFNEXTNO)
      next1 = 0xFFFF;

    if(next0 == next1)
      retcode = sort_SEEKELEMENT(p0, p1, pos+1, horizontal);
    else if(next0 > next1)
      retcode = 1;
    else
      retcode = -1;
  }

  return(retcode);
}

_WORD sort_horizontal(_UWORD *p0, _UWORD *p1)
{
  return(sort_SEEKELEMENT(&smse->ptr[*p0], &smse->ptr[*p1], 0, _TRUE));
}

_WORD sort_vertical(_UWORD *p0, _UWORD *p1)
{
  return(sort_SEEKELEMENT(&smse->ptr[*p0], &smse->ptr[*p1], 0, _FALSE));
}

void sort_next(_MEM_SEEKELEMENT *p, _BOOL horizontal)
{
  _UWORD       i, *ph;
  _SEEKELEMENT *se;
  _UWORD       msedat[4000];

  smse = p;
  ph  = msedat;
  for(i=0; i<p->no; i++)
  {
    *ph = i;
    ph += 1;
  }

  qsort(msedat, p->no, sizeof(_UWORD), (horizontal == _TRUE)? (_QSORTFUNCT)sort_horizontal : (_QSORTFUNCT)sort_vertical);

  for(i=0; i<p->no; i++)
    msedat[p->no + msedat[i]] = i+1;

  se = p->ptr;
  ph = &msedat[p->no];
  if(horizontal == _TRUE)
  {
    for(i=0; i<p->no; i++)
    {
      se->direction.h_next = *ph;
      se += 1;
      ph += 1;
    }
  }
  else
  {
    for(i=0; i<p->no; i++)
    {
      se->direction.v_next = *ph;
      se += 1;
      ph += 1;
    }
  }
}

_BOOL is_output_OBJECTSCHEME(_IO *pio, _BOOL no_chk)
{
  _BOOL retcode = _FALSE;

  if(pio->typ == AS_TBO)
  {
    if((pio->info.tbo.list == OBJLST)||(pio->info.tbo.list == OBJECTSCHEME))
    {
      retcode = no_chk;
      if(no_chk == _FALSE)
      {
        if(valid_VARLABEL(&pio->variable) == _TRUE)
        {
          if(chk_CHKBIT(&pio->chkbit) == _TRUE)
          {
            retcode = _TRUE;
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL add_SEEKELEMENT(_SEEKELEMENT *p, _LOCATION loc, _UWORD no, _UWORD karuindex, _DIRECTION *pd)
{
  _BOOL       retcode = _FALSE;
  _SINGLESEEK *sp;

  if(p->no < (MAX_SEEKELEMENT - 1))
  {
    sp = &p->ptr[p->no];
    init_SINGLESEEK(sp);
    sp->location  = loc;
    sp->no        = no;
    sp->karuindex = karuindex;

    if(pd != NULL)
      sp->direction = *pd;

    p->no        += 1;
    retcode       = _TRUE;
  }

  return(retcode);
}

#define MSBLKSIZ 10
_BOOL add_MEM_SEEKELEMENT(_MEM_SEEKELEMENT *p, _UWORD no)
{
  _UDWORD tmpno = p->no + no;
  
  if(tmpno > p->doneno) 
  {
    tmpno = ((tmpno + MSBLKSIZ) / MSBLKSIZ) * MSBLKSIZ;
    _UDWORD size = tmpno * sizeof(_SEEKELEMENT);
      
    if(Memory2((void**)&p->ptr, size) == 0)
      return _FALSE;
        
    for(_UDWORD i=p->doneno; i<tmpno; i++)
      init_SEEKELEMENT(&p->ptr[i]);
      
    p->doneno = tmpno;
  }
    
  p->no += no;
  
  return _TRUE;
}

/*
_BOOL add_MEM_SEEKELEMENT(_MEM_SEEKELEMENT *p, _UWORD no)
{
  _UDWORD size;
  _BOOL   retcode = _TRUE;

  if(no > 0)
  {
    retcode = _FALSE;
    size = (p->no + no) * sizeof(_SEEKELEMENT);
    if(Memory2((void**)&p->ptr, size) != 0)
    {
      while(no)
      {
        init_SEEKELEMENT(&p->ptr[p->no]);
        p->no ++;
        no --;
      }

      retcode = _TRUE;
    }
  }
  return(retcode);
}
*/


_BOOL del_ioid_SEEKELEMENT(_MY_PICTURE *mp, _UWORD ioid)
{
  _BOOL            retcode = _TRUE;
  _UWORD           oldno, no;
  _SEEKELEMENT     *ptr;
  _MEM_SEEKELEMENT *p = &mp->memseekelement;

  if(ioid != 0xFFFF)
  {
    retcode = _FALSE;
    oldno   =
    no      = p->no;
    ptr     = p->ptr;
    while(no)
    {
      no --;
      if(ptr->ioid == ioid)
      {
        free_SEEKELEMENT(ptr);
        MemMove(ptr, ptr+1, no*sizeof(_SEEKELEMENT));
        p->no--;
        retcode = _TRUE;
      }
      else
      {
        ptr++;
      }
    }

    if(no < oldno)
      Memory2((void**)&p->ptr, p->no * sizeof(_SEEKELEMENT));
  }

  return(retcode);
}

_UWORD get_ioid_SEEKELEMENT(_MEM_SEEKELEMENT *p)
{
  _UWORD       no, ino, retcode = 0;
  _SEEKELEMENT *ptr, *iptr;

  iptr =
  ptr  = p->ptr;
  ino  =
  no   = p->no;
  while(no)
  {
    if(ptr->ioid == retcode)
    {
      no  = ino;
      ptr = iptr;
      retcode ++;
    }
    else
    {
      ptr++;
      no --;
    }
  }

  return(retcode);
}

_BOOL is_button_selectable(_BUTTON *p)
{
  _BOOL retcode = _FALSE;

  if(is_invisible(p->col, p->attrib) == _FALSE)
  {
    retcode = _TRUE;
  }
  else
  {
    if((ImageList.Get(p->buttonimages.image_sel_release) != NULL)&&(ImageList.Get(p->buttonimages.image_sel_press) != NULL))
      retcode = _TRUE;
  }

  return(retcode);
}

_BOOL is_button_cursor(_BUTTON *p)
{
  _BOOL retcode = _TRUE;

  if(is_invisible(p->col, p->attrib) == _TRUE)
    retcode = _FALSE;

 #ifdef UC_THEME
  if(p->frametype >= _THEMEFRAME)
  {
    retcode = _FALSE;
    if(GetPenColor_Makro(SetUp.GetEditColorCursor(_EDITACTIVE)) != INVISIBLE)
      retcode = _TRUE;
  }
 #endif

  return(retcode);
}

_BOOL lookup_SEEKELEMENT_sub(_MEM_SEEKELEMENT *dst, _SubPicture *Src, _SEEKELEMENT *source)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _UWORD       i, index;
  _OBJECT      *obj;
  _MOTION      *mot;
  _INPUT       *pi;
  _SEEKELEMENT se = *source;
  _SubPicture  *Sp;
 #ifdef OK_SEEK_BUTTON
  _BUTTON      *but;
  _UWORD       idx;
 #endif

  if(Src != NULL)
  {
    retcode = _TRUE;

    pm    = &Src->GetPICTURE()->mask;
    index = dst->no;

    if(pm->meminput.ptr != NULL)
    {
      if(add_MEM_SEEKELEMENT(dst, pm->meminput.no) == _TRUE)
      {
        pi = pm->meminput.ptr;
        for(i=0; i<pm->meminput.no; i++)
        {
          dst->ptr[index] = se;
          add_SEEKELEMENT(&dst->ptr[index], SL_INPUT, i, 0xFFFF, &pi->io.direction);
          pi ++;
          index ++;
        }
      }
    }
   #ifdef OK_SEEK_BUTTON
    if(pm->membutton.ptr != NULL)
    {
      but = pm->membutton.ptr;
      idx = 0;
      for(i=0; i<pm->membutton.no; i++)
      {
        if(is_button_selectable(but) == _TRUE)
          idx++;
        but++;
      }
      if(add_MEM_SEEKELEMENT(dst, idx) == _TRUE)
      {
        but = pm->membutton.ptr;
        for(i=0; i<pm->membutton.no; i++)
        {
//          if(is_invisible(but->col, but->attrib) == _FALSE)
          if(is_button_selectable(but) == _TRUE)
          {
            dst->ptr[index]            = se;
            dst->ptr[index].hotkeycode = but->hotkeycode;
            add_SEEKELEMENT(&dst->ptr[index], SL_BUTTON, i, 0xFFFF, &but->direction);
            index ++;
          }
          but += 1;
        }
      }
    }
   #endif

    if(pm->memobject.ptr != NULL)
    {
      for(i=0; i<pm->memobject.no; i++)
      {
        obj = &pm->memobject.ptr[i];
        Sp  = obj->SubPicture;
        if(Sp == NULL)
          Sp = ObjectList.Get(obj->no);

        if(Sp != NULL)
        {
          se = *source;
          add_SEEKELEMENT(&se, SL_OBJECT, i, 0xFFFF, &obj->direction);
          lookup_SEEKELEMENT_sub(dst, Sp, &se);
        }
      }
    }

    if(pm->memmotion.ptr != NULL)
    {
      for(i=0; i<pm->memmotion.no; i++)
      {
        mot = &pm->memmotion.ptr[i];
        
        if(mot->memkaru.no > 0)
        {
          for(index=0; index<mot->memkaru.no; index++)
          {
            Sp = mot->memkaru.ptr[index].SubPicture;
            if(Sp == NULL)
              Sp = ObjectList.Get(mot->memkaru.ptr[index].objno);
              
            if(Sp != NULL)
            {
              se = *source;
              add_SEEKELEMENT(&se, SL_KARU, i, index, &mot->direction); // karu
              lookup_SEEKELEMENT_sub(dst, Sp, &se);
            }
          }
        }
        else if(mot->anything.list == OBJLST)
        {
          Sp = mot->SubPicture;
          if(Sp == NULL)
            Sp = ObjectList.Get(mot->anything.no);

          if(Sp != NULL)
          {
            se = *source;
            add_SEEKELEMENT(&se, SL_MOTION, i, 0xFFFF, &mot->direction);
            lookup_SEEKELEMENT_sub(dst, Sp, &se);
          }
        }
      }
    }

    lookup_SEEKELEMENT_OUTPUT(dst, Src, source, 0xFFFF);
  }

  return(retcode);
}


_BOOL lookup_SEEKELEMENT_OUTPUT(_MEM_SEEKELEMENT *dst, _SubPicture *Src, _SEEKELEMENT *source, _UWORD ioid)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _UWORD       i, index;
  _SEEKELEMENT se = *source;
  _IO          *pio;
  _MOTION      *mot;
  _OBJECT      *obj;
  _SubPicture  *Sp;

  if(Src != NULL)
  {
    retcode = _TRUE;
    pm      = &Src->GetPICTURE()->mask;

    for(i=0; i<pm->memoutput.no; i++)
    {
      pio = & pm->memoutput.ptr[i];
      if(is_output_OBJECTSCHEME(pio, _TRUE) == _TRUE)
      {
//        if(pio->Obj != NULL) // objekt bereits am schirm
//        {
          if((ioid == 0xFFFF)||(ioid == pio->ioid))
          {
            se      = *source;
            se.ioid = get_ioid_SEEKELEMENT(dst);
            add_SEEKELEMENT(&se, SL_OUTPUT, i, 0xFFFF, &pio->direction);
            if(lookup_SEEKELEMENT_sub(dst, pio->Obj, &se) == _TRUE)
              pio->ioid = se.ioid;
          }
//        }
//        else // unbedingt dummy eintragen und ioid setzen
//        {
//          se      = *source;
//          se.ioid = get_ioid_SEEKELEMENT(dst);
//          add_SEEKELEMENT(&se, SL_OUTPUT, i, &pio->direction);
//          if(add_MEM_SEEKELEMENT(dst, 1) == _TRUE)
//          {
//            dst->ptr[dst->no-1] = se;
//            pio->ioid = se.ioid;
//          }
//        }
      }
    }

    if(ioid != 0xFFFF)
    {
      for(i=0; i<pm->memobject.no; i++)
      {
        obj = &pm->memobject.ptr[i];
        Sp  = obj->SubPicture;
        if(Sp == NULL)
          Sp = ObjectList.Get(obj->no);

        if(Sp != NULL)
        {
          se = *source;
          add_SEEKELEMENT(&se, SL_OBJECT, i, 0xFFFF, &obj->direction);
          lookup_SEEKELEMENT_OUTPUT(dst, Sp, &se, ioid);
        }
      }

      for(i=0; i<pm->memmotion.no; i++)
      {
        mot = &pm->memmotion.ptr[i];
        
        if(mot->memkaru.no > 0)
        {
          for(index=0; index<mot->memkaru.no; index++)
          {
            Sp = mot->memkaru.ptr[index].SubPicture;
            if(Sp != NULL)
            {
              se = *source;
              add_SEEKELEMENT(&se, SL_KARU, i, index, &mot->direction); // karu
              lookup_SEEKELEMENT_sub(dst, Sp, &se);
            }
          }
        }
        else if(mot->anything.list == OBJLST)
        {
          Sp = mot->SubPicture;
          if(Sp == NULL)
            Sp = ObjectList.Get(mot->anything.no);

          if(Sp != NULL)
          {
            se = *source;
            add_SEEKELEMENT(&se, SL_MOTION, i, 0, &mot->direction);
            lookup_SEEKELEMENT_OUTPUT(dst, Sp, &se, ioid);
          }
        }
      }
    }
  }

  return(retcode);
}

void do_memseek(_MY_PICTURE *p)
{
  _UWORD i;
  _INPUT *pi;
  _UWORD actms = p->Editor.input.memseekno;

  for(i=0; i<p->memseekelement.no; i++)
  {
    pi = p_input_SEEKELEMENT(p, i, _TRUE);
    if(pi != NULL)
    {
      if((actms != 0xFFFF)&&(pi->memseekno == actms))
        p->Editor.input.memseekno = i;

      pi->memseekno = i;
    }
  }

  sort_next(&p->memseekelement, _TRUE);
  sort_next(&p->memseekelement, _FALSE);
}

_BOOL lookup_SEEKELEMENT(_MY_PICTURE *p, _KOORD eposx, _KOORD eposy)
{
  _UWORD       i;
  _SEEKELEMENT source;
  _ROOM        room;
  _DOT         offset;

  init_SEEKELEMENT(&source);
  free_MEM_SEEKELEMENT(&p->memseekelement);

  for(i=0; i<p->No; i++)
  {
    free_SEEKELEMENT(&source);
    add_SEEKELEMENT(&source, SL_LAYER, i, 0xFFFF, NULL);
    lookup_SEEKELEMENT_sub(&p->memseekelement, &p->Ptr[i], &source);
  }

  do_memseek(p);

  if((eposx != 0)&&(eposy != 0))
  {
    init_DOT(&offset);
    for(i=0; i<p->memseekelement.no; i++)
    {
      if(position_SEEKELEMENT(&room, p, i, &offset, &p->PicZoom) == _TRUE)
      {
        if((eposx > room.xy1.x)&&(eposx < room.xy2.x)&&(eposy > room.xy1.y)&&(eposy < room.xy2.y))
        {
          p->Editor.actno = i;
          break;
        }
      }
    }
  }
  
/*
//_ROOM room;
//_DOT  offset;
_ASCII txt[30];
init_DOT(&offset);
_REGION act = GetActiveRegion();
DeactivateRegion();
for(i=0; i<p->memseekelement.no; i++)
{
  if(position_SEEKELEMENT(&room, p, i, &offset, &p->PicZoom) == _TRUE)
  {
    Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY|T_SOLID, LIGHTMAGENTA);
    DToA(txt, i, 0x800, sizeof(_ASCII));
    OutTextXY(room.xy1.x, room.xy1.y, txt, T_COPY|T_SOLID|T_PROP, WHITE, sizeof(_ASCII));
    
    //DToA(txt, p->memseekelement.ptr[i].direction.h_next, 0x800, sizeof(_ASCII));
    //StrCat(&txt[StrLen(txt, sizeof(_ASCII))], sizeof(_ASCII), ",", sizeof(_ASCII));
    //DToA(&txt[StrLen(txt, sizeof(_ASCII))], p->memseekelement.ptr[i].direction.v_next, 0x800, sizeof(_ASCII));
    //OutTextXY(room.xy1.x, room.xy1.y, txt, T_COPY|T_SOLID|T_PROP, WHITE, sizeof(_ASCII));
  }
}
ActivateRegion(act);
*/
/*
del_ioid_SEEKELEMENT(&p->memseekelement, 2);

Debug(p->memseekelement.no, 2);
for(i=0; i<p->memseekelement.no; i++)
{
  _ROOM room;
  _DOT  offset;
  _ZOOM zoom;
  init_DOT(&offset);
  init_ZOOM(&zoom);
  if(position_SEEKELEMENT(&room, p, i, &offset, &zoom) == _TRUE)
    Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY|T_SOLID, LIGHTRED);
}
*/


  return(_TRUE);
}

void new_IO_SEEKELEMENT(_MY_PICTURE *p, _UWORD ioid)
{
  _SEEKELEMENT source;
  _UWORD       i;

  del_ioid_SEEKELEMENT(p, ioid);

  init_SEEKELEMENT(&source);
  for(i=0; i<p->No; i++)
  {
    free_SEEKELEMENT(&source);
    add_SEEKELEMENT(&source, SL_LAYER, i, 0xFFFF, NULL);
    lookup_SEEKELEMENT_OUTPUT(&p->memseekelement, &p->Ptr[i], &source, ioid);
  }

  do_memseek(p);

  if(p->memseekelement.no > 999)
    lookup_SEEKELEMENT(p, 0, 0);
}

_BOOL is_SEEKELEMENT(_MY_PICTURE *p, _UWORD no)
{
  if(no < p->memseekelement.no)
    return _TRUE;

  return _FALSE;
}

/*
_BOOL scan_IO(_SubPicture *Src, _UWORD ioid)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _UWORD       i;
  _OBJECT      *obj;
  _MOTION      *mot;
//  _BUTTON      *but;
  _IO          *pio;
  _SubPicture  *Sp;

  if(Src != NULL)
  {
    pm = &Src->GetPICTURE()->mask;

    for(i=0; i<pm->meminput.no; i++)
    {
      if(pm->meminput.ptr[i].io.ioid == ioid)
        retcode = _TRUE;
    }

//   #ifdef OK_SEEK_BUTTON
//    but = pm->membutton.ptr;
//    for(i=0; i<pm->membutton.no; i++)
//    {
//      if(pm->membutton.ptr[i].ioid == ioid)
//        retcode = _TRUE;
//    }
//   #endif

    for(i=0; i<pm->memobject.no; i++)
    {
      obj = &pm->memobject.ptr[i];
      Sp  = obj->SubPicture;
      if(Sp == NULL)
        Sp = ObjectList.Get(obj->no);

      retcode = scan_IO(Sp, ioid);
    }

    for(i=0; i<pm->memmotion.no; i++)
    {
      mot = &pm->memmotion.ptr[i];
      if(mot->anything.list == OBJLST)
      {
        Sp = mot->SubPicture;
        if(Sp == NULL)
          Sp = ObjectList.Get(mot->anything.no);

        retcode = scan_IO(Sp, ioid);
      }
    }

    for(i=0; i<pm->memoutput.no; i++)
    {
      pio = & pm->memoutput.ptr[i];
      if(is_output_OBJECTSCHEME(pio, _TRUE) == _TRUE)
      {
        retcode = scan_IO(pio->Obj, ioid);
      }
    }
  }

  return(retcode);
}
*/

// ****************************************************************************
// LayerNo, LayerNo, LayerNo, LayerNo, LayerNo, LayerNo, LayerNo, LayerNo, Laye
// ****************************************************************************
_UWORD layer_SEEKELEMENT(_MY_PICTURE *p, _UWORD no)
{
  _SEEKELEMENT *pse;
  _UWORD       retcode = 0;

  if(no < p->memseekelement.no)
  {
    pse = &p->memseekelement.ptr[no];
    if(pse->no > 0)
    {
      if(pse->ptr[0].location == SL_LAYER)
      {
        retcode = pse->ptr[0].no;
      }
    }
  }

  return(retcode);
}

// ****************************************************************************
// Available, Available, Available, Available, Available, Available, Available,
// ****************************************************************************
_BOOL is_SEEKELEMENT_available(_MY_PICTURE *p, _UWORD layer, _SELECTOR selector)
{
  _BOOL  retcode = _FALSE;
  _UWORD i;

  for(i=0; i<p->memseekelement.no; i++)
  {
    if(layer_SEEKELEMENT(p, i) == layer)
    {
      if(access_SEEKELEMENT(p, i, _TRUE, selector) == _TRUE)
      {
        retcode = _TRUE;
        break;
      }
    }
  }

  return(retcode);
}

// ****************************************************************************
// Position, Position, Position, Position, Position, Position, Position, Positi
// ****************************************************************************

_BOOL position_INPUT(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _DOT *offset, _ZOOM *zoom)
{
  _IO          *pio;
  _PICTUREMASK *pm;
  _BOOL        retcode = _FALSE;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->meminput.no)
    {
      pio = &pm->meminput.ptr[p->no].io;

      if((pio->typ != AS_TBO)&&(pio->typ != AS_BAR)&&(pio->typ != AS_DEFAULT)) // SA8349
      {
        if(valid_VARLABEL(&pio->variable) == _TRUE)
        {
          if(chk_CHKBIT(&pio->chkbit) == _TRUE)
          {
            *pr = pio->room;
            Zoom_ROOM(pr, offset, zoom);
//            *pr = pio->space;
            retcode = _TRUE;
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL position_BUTTON(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _DOT *offset, _ZOOM *zoom)
{
  _BUTTON      *pb;
  _PICTUREMASK *pm;
  _BOOL        retcode = _FALSE;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->membutton.no)
    {
      pb = &pm->membutton.ptr[p->no];
      if(chk_CHKBIT(&pb->chkbit) == _TRUE)
      {
        *pr = pb->room;
        Zoom_ROOM(pr, offset, zoom);
        retcode = _TRUE;
      }
    }
  }

  return(retcode);
}

_BOOL position_OBJECT(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _DOT *offset, _ZOOM *zoom)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _OBJECT      *po;
  _ZOOM        tmp_zoom;
  _DOT         tmp_offset;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memobject.no)
    {
      po = &pm->memobject.ptr[index];
      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        tmp_offset.x = offset->x + Zoom(zoom->x, po->xy.x);
        tmp_offset.y = offset->y + Zoom(zoom->y, po->xy.y);
        tmp_zoom.x   = Zoom(zoom->x, po->zoom.x);
        tmp_zoom.y   = Zoom(zoom->y, po->zoom.y);
        retcode = position_SEEKELEMENT_sub(pr, p+1, po->SubPicture, &tmp_offset, &tmp_zoom);
      }
    }
  }

  return(retcode);
}

_BOOL position_OUTPUT(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _DOT *offset, _ZOOM *zoom)
{
  _IO          *pio;
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _ZOOM        tmp_zoom;
  _DOT         tmp_offset;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memoutput.no)
    {
      pio = &pm->memoutput.ptr[index];
      if(valid_VARLABEL(&pio->variable) == _TRUE)
      {
        if(chk_CHKBIT(&pio->chkbit) == _TRUE)
        {
          tmp_offset.x = offset->x + Zoom(zoom->x, pio->room.xy1.x);
          tmp_offset.y = offset->y + Zoom(zoom->y, pio->room.xy1.y);
//          tmp_zoom     = *zoom;
          Make_ZOOM(&tmp_zoom, pio->Obj, &pio->space);
//         #ifdef ZOOM_KM
//          _ZOOM kmz = do_kmz(&tmp_zoom, &pio->kmz);
//          retcode = position_SEEKELEMENT_sub(pr, p+1, pio->Obj, &tmp_offset, &kmz);
//         #else
//          tmp_zoom.x   = Zoom(zoom->x, pio->zoom.x);
//          tmp_zoom.y   = Zoom(zoom->y, pio->zoom.y);
          retcode = position_SEEKELEMENT_sub(pr, p+1, pio->Obj, &tmp_offset, &tmp_zoom);
//         #endif
        }
      }
    }
  }

  return(retcode);
}

_BOOL position_MOTION(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _DOT *offset, _ZOOM *zoom)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _MOTION      *po;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memmotion.no)
    {
      po = &pm->memmotion.ptr[index];
      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        if(po->onscreen == _TRUE)
        {
          //tmp_offset.x = offset->x + Zoom(zoom->x, po->room.xy1.x); // soog + dx
          //tmp_offset.y = offset->y + Zoom(zoom->y, po->room.xy1.y); // soog + dy
          //tmp_offset.x = offset->x + Zoom(zoom->x, po->oldxy.x); // soog + dx
          //tmp_offset.y = offset->y + Zoom(zoom->y, po->oldxy.y); // soog + dy
          _DOT tmp_offset = po->oldxy;
          _ZOOM tmp_zoom = *zoom;
          AddZoom(&tmp_zoom, &po->zoom);
          retcode = position_SEEKELEMENT_sub(pr, p+1, po->SubPicture, &tmp_offset, &tmp_zoom);
        }
      }
    }
  }

  return(retcode);
}

_BOOL position_KARU(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _DOT *offset, _ZOOM *zoom)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _MOTION      *pmot;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    if(p->no < pm->memmotion.no)
    {
      pmot = &pm->memmotion.ptr[p->no];
      if(chk_CHKBIT(&pmot->chkbit) == _TRUE)
      {
        if(pmot->onscreen == _TRUE)
        {
          if(p->karuindex < pmot->memkaru.no)
          {
            _KARU *pkaru = &pmot->memkaru.ptr[p->karuindex];
            //if((pkaru->onscreen == _TRUE) && (pkaru->gesamt == _TRUE))
            {
              _DOT tmp_offset = pkaru->room.xy1;
              _ZOOM tmp_zoom = pkaru->zoom;
              retcode = position_SEEKELEMENT_sub(pr, p+1, pkaru->SubPicture, &tmp_offset, &tmp_zoom);
            }
          }
        }
      }
    }
  }
  
  return(retcode);
}

_BOOL position_SEEKELEMENT_sub(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _DOT *offset, _ZOOM *zoom)
{
  _BOOL retcode = _FALSE;

  if(Sp != NULL)
  {
    switch(p->location)
    {
      case SL_OBJECT : retcode = position_OBJECT(pr, p, Sp, offset, zoom);
                       break;
      case SL_MOTION : retcode = position_MOTION(pr, p, Sp, offset, zoom);
                       break;
      case SL_KARU   : retcode = position_KARU(pr, p, Sp, offset, zoom);
                       break;
      case SL_INPUT  : retcode = position_INPUT(pr, p, Sp, offset, zoom);
                       break;
      case SL_BUTTON : retcode = position_BUTTON(pr, p, Sp, offset, zoom);
                       break;
      case SL_OUTPUT : retcode = position_OUTPUT(pr, p, Sp, offset, zoom);
                       break;
      default        : break;
    }
  }

  if(retcode == _FALSE)
    init_ROOM_Makro(pr);

  return(retcode);
}

_BOOL position_SEEKELEMENT(_ROOM *pr, _MY_PICTURE *p, _UWORD no, _DOT *offset, _ZOOM *zoom)
{
  _BOOL        retcode = _FALSE;
  _SINGLESEEK  *ps;
//  _DOT         offset;
//  _ZOOM        zoom;
  _UWORD       index;
  _SEEKELEMENT *pse;

  if(no < p->memseekelement.no)
  {
    pse = &p->memseekelement.ptr[no];
    if(pse->no > 1)
    {
      ps = &pse->ptr[0];

      if(ps->location == SL_LAYER)
      {
        index = ps->no;
        if(index < p->No)
        {
//          init_DOT(&offset);
//          init_ZOOM(&zoom);
          retcode = position_SEEKELEMENT_sub(pr, ps+1, &p->Ptr[index], offset, zoom);
        }
      }
    }
  }

  return(retcode);
}

_BOOL load_EDITOR_outof_OBJECT(_EDITOR *pe, _SINGLESEEK *p, _SubPicture *Sp)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _OBJECT      *po;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memobject.no)
    {
      po = &pm->memobject.ptr[index];
      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        retcode = load_EDITOR_sub(pe, p+1, po->SubPicture);
      }
    }
  }

  return(retcode);
}

_BOOL load_EDITOR_outof_MOTION(_EDITOR *pe, _SINGLESEEK *p, _SubPicture *Sp)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _MOTION      *po;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memmotion.no)
    {
      po = &pm->memmotion.ptr[index];
      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        if(po->onscreen == _TRUE)
        {
          retcode = load_EDITOR_sub(pe, p+1, po->SubPicture);
        }
      }
    }
  }

  return(retcode);
}

_BOOL load_EDITOR_outof_KARU(_EDITOR *pe, _SINGLESEEK *p, _SubPicture *Sp)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _MOTION      *pmot;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    if(p->no < pm->memmotion.no)
    {
      pmot = &pm->memmotion.ptr[p->no];
      if(chk_CHKBIT(&pmot->chkbit) == _TRUE)
      {
        if(pmot->onscreen == _TRUE)
        {
          if(p->karuindex < pmot->memkaru.no)
          {
            _KARU *pkaru = &pmot->memkaru.ptr[p->karuindex];
            //if((pkaru->onscreen == _TRUE) && (pkaru->gesamt == _TRUE))
            {
              retcode = load_EDITOR_sub(pe, p+1, pkaru->SubPicture);
            }
          }
        }
      }
    }
  }

  return(retcode);
}

#ifdef USER_BILLION
_BOOL get_xdatbillion(_VARLABEL *pvl, _RESULT *pres, _BOOL hi_lo)
{
  _BOOL   retcode = _FALSE;
  _RESULT myres;

  if(pvl->no > 0)
  {
    if(pvl->info[0].state == VAR_VAL)
    {
      if(xget_dat(&myres, pvl->info[0].value, NOCARE_DEFAULT|NOCARE_CHKBIT|NOCARE_STATESCHEME) == ERROR_CAUSED_BY_NO_ERROR)
      {
        if(hi_lo == _FALSE)
          pres->rmin = myres.value;
        else
          pres->rmax = myres.value;

        retcode = _TRUE;
      }
    }
  }

  return(retcode);
}
#endif

_BOOL load_EDITOR_outof_INPUT(_EDITOR *pe, _SINGLESEEK *p, _SubPicture *Sp)
{
  _INPUT       *pi;
  _PICTUREMASK *pm;
  _BOOL        retcode = _FALSE;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->meminput.no)
    {
      pi = &pm->meminput.ptr[p->no];

      if((pi->io.typ != AS_TBO)&&(pi->io.typ != AS_BAR)&&(pi->io.typ != AS_DEFAULT)) // SA8349
      {
        if(valid_VARLABEL(&pi->io.variable) == _TRUE)
        {
          if(chk_CHKBIT(&pi->io.chkbit) == _TRUE)
          {
            free_EDITOR(pe);

            _BOOL success = _TRUE;
            copy_INPUT(&success, &pe->input, pi);
            pe->input.io.Obj = NULL;
            DoEditorNeeds_COMBOBOX(&pe->input);
            
//            init_IMAGE_Makro(&pe->input.io.image);
            pe->input.io.workstate = STATE_START;
            retcode = VarList.GetSystemData(&pe->result, &pe->variable, &pe->input.io.variable, _TRUE, _TRUE);

           #ifdef USER_BILLION
            if(retcode == _TRUE)
            {
              get_xdatbillion(&pe->variable.info.lo_limit, &pe->result, _FALSE);
              get_xdatbillion(&pe->variable.info.hi_limit, &pe->result, _TRUE);
            }
           #endif

           #ifdef OK_ENCLOSE_NAME
            if(pe->input.io.virtname.pthis != NULL)
            {
              switch(call_enclosed_name_method(METHOD_MYIO_GETDATA, &pe->input.io, &pe->variable, &pe->result, NULL, NULL, _TRUE))
              {
                case _IFAILED : retcode = _FALSE; break;
                case _IDIDIT  : retcode = _TRUE;  break;
                case _IDLE    : break;
              }
              
              pe->input.io.imagecopy = _TRUE; // kopie von image erzeugen
            }
           #endif
           
            if(pe->input.io.typ == AS_THEURGE)
              pe->input.io.imagecopy = _TRUE; // kopie von image erzeugen

            if(pe->input.io.imagecopy == _TRUE) 
            {
              // bei myio und theurge
              // image ist eine kopie, kopie darf nicht freigegeben werden !!
              pe->input.io.image = pi->io.image; // kopie von image erzeugen
            }
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL load_EDITOR_outof_OUTPUT(_EDITOR *pe, _SINGLESEEK *p, _SubPicture *Sp)
{
  _IO          *pio;
  _PICTUREMASK *pm;
  _BOOL        retcode = _FALSE;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->memoutput.no)
    {
      pio = &pm->memoutput.ptr[p->no];

      if(is_output_OBJECTSCHEME(pio, _FALSE) == _TRUE)
      {
        retcode = load_EDITOR_sub(pe, p+1, pio->Obj);
      }
    }
  }

  return(retcode);
}

_BOOL load_EDITOR_outof_BUTTON(_EDITOR *pe, _SINGLESEEK *p, _SubPicture *Sp)
{
  _BUTTON      *pb;
  _PICTUREMASK *pm;
  _BOOL        retcode = _FALSE;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->membutton.no)
    {
      pb = &pm->membutton.ptr[p->no];
      if(chk_CHKBIT(&pb->chkbit) == _TRUE)
      {
        copy_INPUT_BUTTON(&pe->input, pb);
        retcode = _TRUE;
      }
    }
  }

  return(retcode);
}

_BOOL load_EDITOR_sub(_EDITOR *pe, _SINGLESEEK *p, _SubPicture *Sp)
{
  _BOOL retcode = _FALSE;

  if(Sp != NULL)
  {
    switch(p->location)
    {
      case SL_OBJECT : retcode = load_EDITOR_outof_OBJECT(pe, p, Sp);
                       break;
      case SL_MOTION : retcode = load_EDITOR_outof_MOTION(pe, p, Sp);
                       break;
      case SL_KARU   : retcode = load_EDITOR_outof_KARU(pe, p, Sp);
                       break;
      case SL_INPUT  : retcode = load_EDITOR_outof_INPUT(pe, p, Sp);
                       break;
      case SL_BUTTON : retcode = load_EDITOR_outof_BUTTON(pe, p, Sp);
                       break;
      case SL_OUTPUT : retcode = load_EDITOR_outof_OUTPUT(pe, p, Sp);
                       break;
      default        : break;
    }
  }

  return(retcode);
}

_BOOL load_EDITOR(_EDITOR *pe, _MY_PICTURE *p, _UWORD no, _DOT *offset, _ZOOM *zoom)
{
  _ROOM        room;
  _SEEKELEMENT *pse;
  _SINGLESEEK  *ps;
  _UWORD       index;
  _BOOL        retcode = _FALSE;

  if(no < p->memseekelement.no)
  {
    pse = &p->memseekelement.ptr[no];
    if(pse->no > 1)
    {
      ps = &pse->ptr[0];

      if(ps->location == SL_LAYER)
      {
        index = ps->no;
        if(index < p->No)
        {
          if(load_EDITOR_sub(pe, ps+1, &p->Ptr[index]) == _TRUE)
          {
            if(position_SEEKELEMENT(&room, p, no, offset, zoom) == _TRUE)
            {
              pe->input.io.room  = room;
              pe->input.io.space = room;
              retcode = _TRUE;
            }
          }
        }
      }
    }
  }

  return(retcode);
}

// ****************************************************************************
// Bildschirmausgabe, Bildschirmausgabe, Bildschirmausgabe, Bildschirmausgabe
// ****************************************************************************
_BOOL out_SEEKELEMENT_OBJECT(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _EDITSTATE state, _DOT *offset, _ZOOM *zoom)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _OBJECT      *po;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memobject.no)
    {
      po = &pm->memobject.ptr[index];

      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        AddOffset(offset, &po->xy, zoom);
        AddZoom(zoom, &po->zoom);

        retcode = out_SEEKELEMENT_sub(pr, p+1, po->SubPicture, state, offset, zoom);
      }
    }
  }

  return(retcode);
}

_BOOL out_SEEKELEMENT_OUTPUT(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _EDITSTATE state, _DOT *offset, _ZOOM *zoom)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _IO          *pio;
  _UWORD       index;
  _ZOOM        tmp_zoom;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memoutput.no)
    {
      pio = &pm->memoutput.ptr[index];
      if(is_output_OBJECTSCHEME(pio, _FALSE) == _TRUE)
      {
        AddOffset(offset, &pio->room.xy1, zoom);
        Make_ZOOM(&tmp_zoom, pio->Obj, &pio->space);
//       #ifdef ZOOM_KM
//        _ZOOM kmz = do_kmz(zoom, &pio->kmz);
//        retcode = out_SEEKELEMENT_sub(pr, p+1, pio->Obj, state, offset, &kmz);
//       #else
        retcode = out_SEEKELEMENT_sub(pr, p+1, pio->Obj, state, offset, &tmp_zoom);
//       #endif
      }
    }
  }

  return(retcode);
}

_BOOL out_SEEKELEMENT_MOTION(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _EDITSTATE state, _DOT *offset, _ZOOM *zoom)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _MOTION      *po;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memmotion.no)
    {
      po = &pm->memmotion.ptr[index];

      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        if(po->onscreen == _TRUE)
        {
        //AddOffset(offset, &po->room.xy1, zoom); // soog +dx + dy
        //AddOffset(offset, &po->oldxy, zoom); // soog +dx + dy
          _DOT tmp_offset = po->oldxy;
          _ZOOM tmp_zoom = *zoom;
          AddZoom(&tmp_zoom, &po->zoom);

          retcode = out_SEEKELEMENT_sub(pr, p+1, po->SubPicture, state, &tmp_offset, &tmp_zoom);
        }
      }
    }
  }

  return(retcode);
}


_BOOL out_SEEKELEMENT_KARU(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _EDITSTATE state, _DOT *offset, _ZOOM *zoom)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _MOTION      *pmot;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    if(p->no < pm->memmotion.no)
    {
      pmot = &pm->memmotion.ptr[p->no];

      if(chk_CHKBIT(&pmot->chkbit) == _TRUE)
      {
        if(pmot->onscreen == _TRUE)
        {
          if(p->karuindex < pmot->memkaru.no)
          {
            _KARU *pkaru = &pmot->memkaru.ptr[p->karuindex];
            //if((pkaru->onscreen == _TRUE) && (pkaru->gesamt == _TRUE))
            {
              _DOT tmp_offset = pkaru->room.xy1;
              _ZOOM tmp_zoom = pkaru->zoom;
              retcode = out_SEEKELEMENT_sub(pr, p+1, pkaru->SubPicture, state, &tmp_offset, &tmp_zoom);
            }
          }
        }
      }
    }
  }

  return(retcode);
}



#ifdef USER_BILLION // eingabefarbe invers
_BOOL color_billioneditor(_IO *p, _EDITSTATE state)
{
  if(state != _EDITPASSIVE)
  {
    if((p->typ == AS_NUMERIC)||
       (p->typ == AS_ASCII) ||
       (p->typ == AS_MENU) ||
       (p->typ == AS_ENUMERATION) ||
       (p->typ == AS_DATE) ||
       (p->typ == AS_TIME) ||
       (p->typ == AS_NUMERIC_HIDE) ||
       (p->typ == AS_ASCII_HIDE)
      )
    {
      return(_TRUE);
    }
  }
  return(_FALSE);
}
#endif

void give_editorcolor(_IO *p, _EDITSTATE state)
{
 #ifdef USER_BILLION // eingabefarbe invers
  if(color_billioneditor(p, state) == _FALSE)
  {
    p->coltxt  = SetUp.GetEditColorTxt(state, p->coltxt);
    p->colback = SetUp.GetEditColorBack(state, p->colback);
  }
  
  p->colframe    = SetUp.GetEditColorFrame(state, p->colframe);
  p->is_dyncolor = _FALSE;
 #else 
  _BOOL udc_slider = _FALSE;
  if(p->theurge != NULL)
  {
    if(p->theurge->urgetype == _URGE_SLIDER)
      udc_slider = _TRUE;
  }

  if(udc_slider == _TRUE)
  {
    p->coltxt = SetUp.GetEditColorBack(state, p->colback);
  }
  else
  {
    p->coltxt   = SetUp.GetEditColorTxt(state, p->coltxt);
    p->colback  = SetUp.GetEditColorBack(state, p->colback);
    p->colframe = SetUp.GetEditColorFrame(state, p->colframe);
  }
  p->is_dyncolor = _FALSE;
 #endif
 
  init_LINKSCHEME_Makro(&p->dyncolback);  // dynamic color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncolfront); // dynamic color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncoltl);    // dynamic color frame (top-left)
  init_LINKSCHEME_Makro(&p->dyncolbr);    // dynamic color frame (bottom-right)
  init_LINKSCHEME_Makro(&p->dyncoltxt);   // dynamic color text
  
}

_BOOL out_SEEKELEMENT_INPUT(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _EDITSTATE state)
{
  _IO          tmp, *pt;
  _PICTUREMASK *pm;
  _DOT         offset;
  _ZOOM        zoom;
  _BOOL        retcode = _FALSE;
  _BOOL        st = _TRUE;


  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->meminput.no)
    {
      init_DOT(&offset);
      init_ZOOM(&zoom);
      pt            = &pm->meminput.ptr[p->no].io;
      tmp           = *pt;
      tmp.room      = *pr;
      tmp.space     = *pr;

      if(state != _EDITPASSIVE)
      {
       #ifdef OK_ENCLOSE_NAME
        _BOOL doit = _TRUE;
        if(pt->virtname.pthis != NULL)
        {
          if(call_enclosed_name_method(METHOD_MYIO_DISPLAYCURSOR, pt, NULL, NULL, NULL, NULL, _TRUE) == _IDIDIT)
            doit = _FALSE;
        }
        if(doit == _TRUE)
       #endif
        {
          give_editorcolor(&tmp, state);
        }
      }

      if(DrawBackGround)
      {
        if(is_redraw(tmp.room, &offset, &zoom) == _FALSE)
          st = _FALSE;
      }    

      if(st == _TRUE)
      {
        tmp.cstate.init = _TRUE;

        Sp->OutIoIntern(&tmp, &offset, &zoom, _TRUE, _TRUE, 0xFFFF, NULL, _FALSE);
      }

      pt->image = tmp.image;               // keep backgroundimage
      pt->cstate = tmp.cstate;

      init_IO_Makro(&tmp);

/*
      tmp = pm->meminput.ptr[p->no].io;
      tmp.room  = *pr;
      tmp.space = *pr;

      if(state == _TRUE)
      {
        tmp.coltxt      = Project.GetEditColorTxt(_EDITACTIVE, tmp.coltxt);
        tmp.colframe    = Project.GetEditColorFrame(_EDITACTIVE, tmp.colframe);
        tmp.colback     = Project.GetEditColorBack(_EDITACTIVE, tmp.colback);
        tmp.is_dyncolor = _FALSE;

        init_LINKSCHEME_Makro(&tmp.dyncolback);  // dynamic color background (hatch)
        init_LINKSCHEME_Makro(&tmp.dyncolfront); // dynamic color background (hatch)
        init_LINKSCHEME_Makro(&tmp.dyncoltl);    // dynamic color frame (top-left)
        init_LINKSCHEME_Makro(&tmp.dyncolbr);    // dynamic color frame (bottom-right)
        init_LINKSCHEME_Makro(&tmp.dyncoltxt);   // dynamic color text
      }

      tmp.cstate.init = _TRUE;
      Sp->OutIoIntern(&tmp, &offset, &zoom, _TRUE, _TRUE);

      init_IO_Makro(&tmp);
*/


      retcode = _TRUE;
    }
  }

  return(retcode);
}

_BOOL out_SEEKELEMENT_BUTTON(_SINGLESEEK *p, _SubPicture *Sp, _EDITSTATE state, _DOT *offset, _ZOOM *zoom)
{
  _BUTTON      *pb;
  _PICTUREMASK *pm;
  _BOOL        retcode = _FALSE;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->membutton.no)
    {
      pb = &pm->membutton.ptr[p->no];
      if(chk_CHKBIT(&pb->chkbit) == _TRUE)
      {
        pb->selected = (state != _EDITPASSIVE)? _TRUE : _FALSE;
        Sp->Out_BUTTON(pb, offset, zoom, DEFATTRIB);
        retcode      = _TRUE;
      }
    }
  }

  return(retcode);
}

_BOOL out_SEEKELEMENT_sub(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _EDITSTATE state, _DOT *offset, _ZOOM *zoom)
{
  _BOOL retcode = _FALSE;
 #ifdef UC_MOUSE
  _UWORD hide;
 #endif

  if(Sp != NULL)
  {
   #ifdef UC_MOUSE
    hide = IfNecHideMouseCursor(pr);
   #endif
    switch(p->location)
    {
      case SL_OBJECT : retcode = out_SEEKELEMENT_OBJECT(pr, p, Sp, state, offset, zoom);
                       break;
      case SL_MOTION : retcode = out_SEEKELEMENT_MOTION(pr, p, Sp, state, offset, zoom);
                       break;
      case SL_KARU   : retcode = out_SEEKELEMENT_KARU(pr, p, Sp, state, offset, zoom);
                       break;
      case SL_INPUT  : retcode = out_SEEKELEMENT_INPUT(pr, p, Sp, state);
                       break;
      case SL_BUTTON : retcode = out_SEEKELEMENT_BUTTON(p, Sp, state, offset, zoom);
                       break;
      case SL_OUTPUT : retcode = out_SEEKELEMENT_OUTPUT(pr, p, Sp, state, offset, zoom);
                       break;
      default        : break;
    }
   #ifdef UC_MOUSE
    IfNecShowMouseCursor(hide);
   #endif
  }

  return(retcode);
}

_BOOL out_SEEKELEMENT(_MY_PICTURE *p, _UWORD no, _EDITSTATE state, _DOT *offset, _ZOOM *zoom)
{
  _SEEKELEMENT *pse;
  _SINGLESEEK  *ps;
  _UWORD       index;
  _ROOM        room;
  _BOOL        retcode = _FALSE;

  if(no < p->memseekelement.no)
  {
    if(position_SEEKELEMENT(&room, p, no, offset, zoom) == _TRUE)
    {
//      Zoom_ROOM(&room, offset, zoom);

      pse = &p->memseekelement.ptr[no];
      if(pse->no > 1)
      {
        ps = &pse->ptr[0];

        if(ps->location == SL_LAYER)
        {
          index = ps->no;
          if(index < p->No)
          {
            retcode = out_SEEKELEMENT_sub(&room, ps+1, &p->Ptr[index], state, offset, zoom);
          }
        }
      }
    }
  }

  return(retcode);
}
// ****************************************************************************
// get --> to _INPUT, get --> to _INPUT, get --> to _INPUT, get --> to _INPUT,
// ****************************************************************************
_INPUT *p_input_INPUT(_SINGLESEEK *p, _SubPicture *Sp, _BOOL no_chk)
{
  _PICTUREMASK *pm;
  _INPUT       *pi;
  _INPUT       *retcode = NULL;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->meminput.no)
    {
      pi = &pm->meminput.ptr[p->no];

      if((no_chk == _TRUE)||(valid_VARLABEL(&pi->io.variable) == _TRUE))
      {
        if((no_chk == _TRUE)||(chk_CHKBIT(&pi->io.chkbit) == _TRUE))
        {
          retcode = pi;
        }
      }
    }
  }

  return(retcode);
}

_INPUT stat_input;
_INPUT *p_input_BUTTON(_SINGLESEEK *p, _SubPicture *Sp,_BOOL no_chk)
{
  _BUTTON      *pb;
  _PICTUREMASK *pm;
  _INPUT       *retcode = NULL;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->membutton.no)
    {
      pb = &pm->membutton.ptr[p->no];
      if((no_chk == _TRUE)||(chk_CHKBIT(&pb->chkbit) == _TRUE))
      {
        init_INPUT_Makro(&stat_input);
        copy_INPUT_BUTTON(&stat_input, pb);
        retcode = &stat_input;
      }
    }
  }

  return(retcode);
}

_INPUT *p_input_OBJECT(_SINGLESEEK *p, _SubPicture *Sp, _BOOL no_chk)
{
  _INPUT       *retcode = NULL;
  _PICTUREMASK *pm;
  _OBJECT      *po;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memobject.no)
    {
      po = &pm->memobject.ptr[index];
      if((no_chk == _TRUE)||(chk_CHKBIT(&po->chkbit) == _TRUE))
      {
        retcode = p_input_SEEKELEMENT_sub(p+1, po->SubPicture, no_chk);
      }
    }
  }

  return(retcode);
}

_INPUT *p_input_OUTPUT(_SINGLESEEK *p, _SubPicture *Sp, _BOOL no_chk)
{
  _INPUT       *retcode = NULL;
  _PICTUREMASK *pm;
  _IO          *pio;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memoutput.no)
    {
      pio = &pm->memoutput.ptr[index];
      if(is_output_OBJECTSCHEME(pio, no_chk) == _TRUE)
      {
        retcode = p_input_SEEKELEMENT_sub(p+1, pio->Obj, no_chk);
      }
    }
  }

  return(retcode);
}


_INPUT *p_input_MOTION(_SINGLESEEK *p, _SubPicture *Sp, _BOOL no_chk)
{
  _INPUT       *retcode = NULL;
  _PICTUREMASK *pm;
  _MOTION      *po;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memmotion.no)
    {
      po = &pm->memmotion.ptr[index];
      if((no_chk == _TRUE)||(chk_CHKBIT(&po->chkbit) == _TRUE))
      {
        retcode = p_input_SEEKELEMENT_sub(p+1, po->SubPicture, no_chk);
      }
    }
  }

  return(retcode);
}

_INPUT *p_input_KARU(_SINGLESEEK *p, _SubPicture *Sp, _BOOL no_chk)
{
  _INPUT       *retcode = NULL;
  _PICTUREMASK *pm;
  _MOTION      *pmot;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    if(p->no < pm->memmotion.no)
    {
      pmot = &pm->memmotion.ptr[p->no];
      if((no_chk == _TRUE)||(chk_CHKBIT(&pmot->chkbit) == _TRUE))
      {
        if(p->karuindex < pmot->memkaru.no)
        {
          _KARU *pkaru = &pmot->memkaru.ptr[p->karuindex];
          //if((pkaru->onscreen == _TRUE) && (pkaru->gesamt == _TRUE))
          {
            retcode = p_input_SEEKELEMENT_sub(p+1, pkaru->SubPicture, no_chk);
          }
        }
      }
    }
  }

  return(retcode);
}

_INPUT *p_input_SEEKELEMENT_sub(_SINGLESEEK *p, _SubPicture *Sp, _BOOL no_chk)
{
  _INPUT *retcode = NULL;

  if(Sp != NULL)
  {
    switch(p->location)
    {
      case SL_OBJECT : retcode = p_input_OBJECT(p, Sp, no_chk);
                       break;
      case SL_MOTION : retcode = p_input_MOTION(p, Sp, no_chk);
                       break;
      case SL_KARU   : retcode = p_input_KARU(p, Sp, no_chk);
                       break;
      case SL_INPUT  : retcode = p_input_INPUT(p, Sp, no_chk);
                       break;
      case SL_BUTTON : retcode = p_input_BUTTON(p, Sp, no_chk);
                       break;
      case SL_OUTPUT : retcode = p_input_OUTPUT(p, Sp, no_chk);
                       break;
      default        : break;
    }
  }

  return(retcode);
}

_INPUT *p_input_SEEKELEMENT(_MY_PICTURE *p, _UWORD no, _BOOL no_chk)
{
  // no_chk = _TRUE ... es werden keine abhängigkeiten überprüft (chkbit, etc)

  _INPUT       *retcode = NULL;
  _SINGLESEEK  *ps;
  _UWORD       index;
  _SEEKELEMENT *pse;

  if(no < p->memseekelement.no)
  {
    pse = &p->memseekelement.ptr[no];
    if(pse->no > 1)
    {
      ps = &pse->ptr[0];

      if(ps->location == SL_LAYER)
      {
        index = ps->no;
        if(index < p->No)
        {
          retcode = p_input_SEEKELEMENT_sub(ps+1, &p->Ptr[index], no_chk);
        }
      }
    }
  }

  return(retcode);
}


// ****************************************************************************
// Accessible, Accessible, Accessible, Accessible, Accessible, Accessible, Acce
// ****************************************************************************

_BOOL valid_SELECTOR(_SELECTOR soll, _SELECTOR ist)
{
  _BOOL retcode = _TRUE;

  if(soll != ist)
  {
    if((soll != _SELECTOR_DEFAULT)&&(ist != _SELECTOR_DEFAULT))
    {
      retcode = _FALSE;
    }
  }
  return(retcode);
}

_BOOL access_INPUT(_SINGLESEEK *p, _SubPicture *Sp, _BOOL system, _SELECTOR selector)
{
  _BOOL retcode = _FALSE;
  _PICTUREMASK *pm;
  _INPUT       *pi;
  _LSEVARINFO  *pvi;
  _ACCESSFAILED failed = CHKBIT_FAILED;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->meminput.no)
    {
      pi = &pm->meminput.ptr[p->no];

      if(valid_SELECTOR(selector, pi->selector) == _TRUE)
      {
        if((valid_VARLABEL(&pi->io.variable) == _TRUE)&&(chk_CHKBIT(&pi->io.chkbit) == _TRUE))
        {
          if(pi->io.cstate.init == _FALSE) // überprüfen ob input bereits am bildschirm ausgegeben
          {
            if(access_input(pi, &failed) == _TRUE)
            {
              if(VarList.GetVarInfo(&pvi, pi->io.variable.info[0].value) == _TRUE)
              {
                failed = ACCESS_FAILED;
                retcode = chk_ACCESS(pvi->accessno, system);
               #ifdef OK_ENCLOSE_NAME
                if((retcode == _TRUE)&&(pi->io.virtname.pthis != NULL))
                {
                  failed = NOTHING_FAILED;
                  if(call_enclosed_name_method(METHOD_MYIO_ACCESSIBLE, &pi->io, NULL, NULL, NULL, NULL, _TRUE) == _IFAILED)
                  {
                    retcode = _FALSE;
                    failed  = USERIF_FAILED;
                  }
                }
               #endif
              }
            }
          }
        }

      #ifdef USER_BILLION
	     #define DEF_TRYTOACCESS
	    #endif
	    #ifdef USER_DEKEMA
	     #define DEF_TRYTOACCESS
      #endif

	    #ifdef DEF_TRYTOACCESS
	     #undef DEF_TRYTOACCESS
       #ifndef CUTOFF_USER
        _FBOOL_PT_UWORD_FOUNDHID_BOOL pta;
        pta = (_FBOOL_PT_UWORD_FOUNDHID_BOOL)find_lasal_method(LseThisPointer, METHOD_LSE_TRY_TO_ACCESS);
        if(pta != NULL)
        {
          load_this(LseThisPointer);
          retcode = pta((void*)pi, failed, FOUND_INPUT, retcode);
        }
       #endif
      #endif
      }
    }
  }

  return(retcode);
}

_BOOL access_BUTTON(_SINGLESEEK *p, _SubPicture *Sp, _BOOL system, _SELECTOR selector)
{
  _PICTUREMASK *pm;
  _BOOL        retcode = _FALSE;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->membutton.no)
    {
     #ifdef USER_KM
     #ifdef HID
      if(pm->membutton.ptr[p->no].doubleclick == _FALSE)
        retcode = _FALSE; // dr918
      else
     #endif
     #endif
        retcode = access_button(&pm->membutton.ptr[p->no], system, selector);
    }
  }

  return(retcode);
}

_BOOL access_OBJECT(_SINGLESEEK *p, _SubPicture *Sp, _BOOL system, _SELECTOR selector)
{
  _BOOL        st, retcode = _FALSE;
  _PICTUREMASK *pm;
  _OBJECT      *po;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memobject.no)
    {
      po = &pm->memobject.ptr[index];
      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        if(statescheme_OBJECT(po) == STATE_ACTIVE)
        {
          st = _TRUE;
          if(chk_VARLABEL(&po->loader) == _TRUE)
            st = valid_VARLABEL(&po->loader);

          if(st == _TRUE)
            retcode = access_SEEKELEMENT_sub(p+1, po->SubPicture, system, selector);
        }
      }
    }
  }

  return(retcode);
}

_BOOL access_OUTPUT(_SINGLESEEK *p, _SubPicture *Sp, _BOOL system, _SELECTOR selector)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _IO          *pio;
  _UDWORD      actin, index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memoutput.no)
    {
      pio = &pm->memoutput.ptr[index];

      if(is_output_OBJECTSCHEME(pio, _FALSE) == _TRUE)
      {
        if(StateScheme.Call(&actin, &pio->stscheme, NULL, STATE_ACTIVE) == _FALSE)
          actin = STATESCHEME_PRESELECTION;

        if(actin == STATE_ACTIVE)
        {
          retcode = access_SEEKELEMENT_sub(p+1, pio->Obj, system, selector);
        }
      }
    }
  }

  return(retcode);
}

_BOOL access_MOTION(_SINGLESEEK *p, _SubPicture *Sp, _BOOL system, _SELECTOR selector)
{
  _BOOL        retcode = _FALSE;
//  _BOOL        st;
  _PICTUREMASK *pm;
  _MOTION      *po;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memmotion.no)
    {
      po = &pm->memmotion.ptr[index];
      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        if(po->onscreen == _TRUE)
        {
          _UDWORD tmp;
          if(StateScheme.Call(&tmp, &po->stscheme, NULL, STATE_ACTIVE) == _FALSE)
            tmp = STATESCHEME_PRESELECTION;
          
          if(tmp == STATE_ACTIVE)
          {
//          st = _TRUE;
//          if(chk_VARLABEL(&po->loader) == _TRUE)
//            st = valid_VARLABEL(&po->loader);

//          if(st == _TRUE)
            retcode = access_SEEKELEMENT_sub(p+1, po->SubPicture, system, selector);
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL access_KARU(_SINGLESEEK *p, _SubPicture *Sp, _BOOL system, _SELECTOR selector)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _MOTION      *pmot;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    if(p->no < pm->memmotion.no)
    {
      pmot = &pm->memmotion.ptr[p->no];
      if(chk_CHKBIT(&pmot->chkbit) == _TRUE)
      {
        if(pmot->onscreen == _TRUE)
        {
          _UDWORD tmp;
          if(StateScheme.Call(&tmp, &pmot->stscheme, NULL, STATE_ACTIVE) == _FALSE)
            tmp = STATESCHEME_PRESELECTION;
          
          if(tmp == STATE_ACTIVE)
          {
            if(p->karuindex < pmot->memkaru.no)
            {
              _KARU *pkaru = &pmot->memkaru.ptr[p->karuindex];
              if((pkaru->onscreen == _TRUE) && (pkaru->gesamt == _TRUE))
              {
                retcode = access_SEEKELEMENT_sub(p+1, pkaru->SubPicture, system, selector);
              }
            }
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL access_SEEKELEMENT_sub(_SINGLESEEK *p, _SubPicture *Sp, _BOOL system, _SELECTOR selector)
{
  _BOOL retcode = _FALSE;

  if(Sp != NULL)
  {
    switch(p->location)
    {
      case SL_OBJECT : retcode = access_OBJECT(p, Sp, system, selector);
                       break;
      case SL_MOTION : retcode = access_MOTION(p, Sp, system, selector);
                       break;
      case SL_KARU   : retcode = access_KARU(p, Sp, system, selector);
                       break;
      case SL_INPUT  : retcode = access_INPUT(p, Sp, system, selector);
                       break;
      case SL_BUTTON : retcode = access_BUTTON(p, Sp, system, selector);
                       break;
      case SL_OUTPUT : retcode = access_OUTPUT(p, Sp, system, selector);
                       break;
      default        : break;
    }
  }

  return(retcode);
}

_BOOL access_SEEKELEMENT(_MY_PICTURE *p, _UWORD no, _BOOL system, _SELECTOR selector)
{
  _BOOL        retcode = _FALSE;
  _UWORD       index;
  _SINGLESEEK  *ps;
  _SEEKELEMENT *pse;

  if(no < p->memseekelement.no)
  {
    pse = &p->memseekelement.ptr[no];
    if(pse->no > 1)
    {
      ps = &pse->ptr[0];

      if(ps->location == SL_LAYER)
      {
        index = ps->no;
        if(index < p->No)
        {
          retcode = access_SEEKELEMENT_sub(ps+1, &p->Ptr[index], system, selector);
        }
      }
    }
  }

  return(retcode);
}

// ****************************************************************************
// Summary, Summary, Summary, Summary, Summary, Summary, Summary, Summary, Summ
// ****************************************************************************
_BOOL summary_INPUT(_SINGLESEEK *p, _SubPicture *Sp, _BOOL state, _DOT *offset, _ZOOM *zoom, _Picture *PPic)
{
  _IO          *pio;
  _PICTUREMASK *pm;
  _BOOL        retcode = _FALSE;
  _UWORD       tmp;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->meminput.no)
    {
      pio = &pm->meminput.ptr[p->no].io;

      if(valid_VARLABEL(&pio->variable) == _TRUE)
      {
        if(chk_CHKBIT(&pio->chkbit) == _TRUE)
        {
          tmp          = pio->summary;
          pio->summary = DEFSUMMARY;
          Sp->OutSummary(state, tmp, offset, zoom, _FALSE, PPic);
          pio->summary = tmp;
          retcode      = _TRUE;
        }
      }
    }
  }
  return(retcode);
}

_BOOL summary_BUTTON(_SINGLESEEK *p, _SubPicture *Sp, _BOOL state, _DOT *offset, _ZOOM *zoom)
{
  _BUTTON      *pb;
  _PICTUREMASK *pm;
  _BOOL        retcode = _FALSE;
  _UWORD       tmp;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->membutton.no)
    {
      pb = &pm->membutton.ptr[p->no];
      if(chk_CHKBIT(&pb->chkbit) == _TRUE)
      {
        tmp         = pb->summary;
        pb->summary = DEFSUMMARY;
        Sp->OutSummary(state, tmp, offset, zoom, _FALSE, NULL);
        pb->summary = tmp;
        retcode = _TRUE;
      }
    }
  }

  return(retcode);
}

_BOOL summary_OBJECT(_SINGLESEEK *p, _SubPicture *Sp, _BOOL state, _DOT *offset, _ZOOM *zoom, _Picture *PPic)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _OBJECT      *po;
  _ZOOM        tmp_zoom;
  _DOT         tmp_offset;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memobject.no)
    {
      po = &pm->memobject.ptr[index];
      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        tmp_offset.x = offset->x + Zoom(zoom->x, po->xy.x);
        tmp_offset.y = offset->y + Zoom(zoom->y, po->xy.y);
        tmp_zoom.x   = Zoom(zoom->x, po->zoom.x);
        tmp_zoom.y   = Zoom(zoom->y, po->zoom.y);
        retcode = summary_SEEKELEMENT_sub(p+1, po->SubPicture, state, &tmp_offset, &tmp_zoom, PPic);
      }
    }
  }

  return(retcode);
}

_BOOL summary_OUTPUT(_SINGLESEEK *p, _SubPicture *Sp, _BOOL state, _DOT *offset, _ZOOM *zoom, _Picture *PPic)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _IO          *pio;
  _ZOOM        tmp_zoom;
  _DOT         tmp_offset;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memoutput.no)
    {
      pio = &pm->memoutput.ptr[index];
      if(is_output_OBJECTSCHEME(pio, _FALSE) == _TRUE)
      {
        tmp_offset.x = offset->x + Zoom(zoom->x, pio->room.xy1.x);
        tmp_offset.y = offset->y + Zoom(zoom->y, pio->room.xy1.y);
//        tmp_zoom = *zoom;
        Make_ZOOM(&tmp_zoom, pio->Obj, &pio->space);
//       #ifdef ZOOM_KM
//        _ZOOM kmz = do_kmz(&tmp_zoom, &pio->kmz);
//        retcode = summary_SEEKELEMENT_sub(p+1, pio->Obj, state, &tmp_offset, &kmz, PPic);
//       #else
        retcode = summary_SEEKELEMENT_sub(p+1, pio->Obj, state, &tmp_offset, &tmp_zoom, PPic);
//       #endif
      }
    }
  }

  return(retcode);
}

_BOOL summary_MOTION(_SINGLESEEK *p, _SubPicture *Sp, _BOOL state, _DOT *offset, _ZOOM *zoom)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _MOTION      *po;
  _ZOOM        tmp_zoom;
  _DOT         tmp_offset;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memmotion.no)
    {
      po = &pm->memmotion.ptr[index];
      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        if(po->onscreen == _TRUE)
        {
        //tmp_offset.x = offset->x + Zoom(zoom->x, po->room.xy1.x); // soog + dx
        //tmp_offset.y = offset->y + Zoom(zoom->y, po->room.xy1.y); // soog + dy
        //tmp_offset.x = offset->x + Zoom(zoom->x, po->oldxy.x); // soog + dx
        //tmp_offset.y = offset->y + Zoom(zoom->y, po->oldxy.y); // soog + dy
          tmp_offset = po->oldxy;
          tmp_zoom.x = Zoom(zoom->x, po->zoom.x);
          tmp_zoom.y = Zoom(zoom->y, po->zoom.y);
          retcode = summary_SEEKELEMENT_sub(p+1, po->SubPicture, state, &tmp_offset, &tmp_zoom, NULL);
        }
      }
    }
  }

  return(retcode);
}

_BOOL summary_KARU(_SINGLESEEK *p, _SubPicture *Sp, _BOOL state, _DOT *offset, _ZOOM *zoom)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _MOTION      *pmot;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    if(p->no < pm->memmotion.no)
    {
      pmot = &pm->memmotion.ptr[p->no];
      if(chk_CHKBIT(&pmot->chkbit) == _TRUE)
      {
        if(pmot->onscreen == _TRUE)
        {
          if(p->karuindex < pmot->memkaru.no)
          {
            _KARU *pkaru = &pmot->memkaru.ptr[p->karuindex];
            //if((pkaru->onscreen == _TRUE) && (pkaru->gesamt == _TRUE))
            {
              _DOT tmp_offset = pkaru->room.xy1;
              _ZOOM tmp_zoom = pkaru->zoom;
              retcode = summary_SEEKELEMENT_sub(p+1, pkaru->SubPicture, state, &tmp_offset, &tmp_zoom, NULL);
            }
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL summary_SEEKELEMENT_sub(_SINGLESEEK *p, _SubPicture *Sp, _BOOL state, _DOT *offset, _ZOOM *zoom, _Picture *PPic)
{
  _BOOL retcode = _FALSE;

  if(Sp != NULL)
  {
    switch(p->location)
    {
      case SL_OBJECT : retcode = summary_OBJECT(p, Sp, state, offset, zoom, PPic);
                       break;
      case SL_MOTION : retcode = summary_MOTION(p, Sp, state, offset, zoom);
                       break;
      case SL_KARU   : retcode = summary_KARU(p, Sp, state, offset, zoom);
                       break;
      case SL_INPUT  : retcode = summary_INPUT(p, Sp, state, offset, zoom, PPic);
                       break;
      case SL_BUTTON : retcode = summary_BUTTON(p, Sp, state, offset, zoom);
                       break;
      case SL_OUTPUT : retcode = summary_OUTPUT(p, Sp, state, offset, zoom, PPic);
                       break;
      default        : break;
    }
  }

  return(retcode);
}

_BOOL summary_SEEKELEMENT(_MY_PICTURE *p, _UWORD no, _BOOL state, _Picture *PPic)
{
  _BOOL        retcode = _FALSE;
  _SINGLESEEK  *ps;
  _DOT         offset;
  _ZOOM        zoom;
  _UWORD       index;
  _SEEKELEMENT *pse;

  if(no < p->memseekelement.no)
  {
    pse = &p->memseekelement.ptr[no];
    if(pse->no > 1)
    {
      ps = &pse->ptr[0];

      if(ps->location == SL_LAYER)
      {
        index = ps->no;
        if(index < p->No)
        {
          init_DOT(&offset);
          init_ZOOM(&zoom);
          retcode = summary_SEEKELEMENT_sub(ps+1, &p->Ptr[index], state, &offset, &zoom, PPic);
        }
      }
    }
  }

  return(retcode);
}


// ****************************************************************************
// action on input, action on input, action on input, action on input, action o
// ****************************************************************************
_BOOL action_BUTTON(_SINGLESEEK *p, _SubPicture *Sp, _BOOL press_state, _Window *Pw, _DOT *offset, _ZOOM *zoom, _BOOL lrm)
{
  _BUTTON      *pb;
  _PICTUREMASK *pm;
  _BOOL        retcode = _FALSE;
  _UDWORD      tmp;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;

    if(p->no < pm->membutton.no)
    {
      pb = &pm->membutton.ptr[p->no];
      if(chk_CHKBIT(&pb->chkbit) == _TRUE)
      {
        if(StateScheme.Call(&tmp, &pb->stscheme, NULL, STATE_ACTIVE) == _FALSE)
          tmp = STATESCHEME_PRESELECTION;

        if(tmp == STATE_ACTIVE)
        {
          Sp->DoButton(pb, press_state, Pw, offset, zoom, lrm, _FALSE);
//          pb->state = _FALSE;  // sofort wieder auf nicht gedrückt, refresh erledigt die anzeige
          retcode   = _TRUE;
        }
      }
    }
  }

  return(retcode);
}

_BOOL action_OBJECT(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _BOOL press_state, _Window *Pw, _DOT *offset, _ZOOM *zoom, _BOOL lrm)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _OBJECT      *po;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memobject.no)
    {
      po = &pm->memobject.ptr[index];
      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        AddOffset(offset, &po->xy, zoom);
        AddZoom(zoom, &po->zoom);
        retcode = action_SEEKELEMENT_sub(pr, p+1, po->SubPicture, press_state, Pw, offset, zoom, lrm);
      }
    }
  }

  return(retcode);
}

_BOOL action_MOTION(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _BOOL press_state, _Window *Pw, _DOT *offset, _ZOOM *zoom, _BOOL lrm)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _MOTION      *po;
  _UWORD       index;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memmotion.no)
    {
      po = &pm->memmotion.ptr[index];
      if(chk_CHKBIT(&po->chkbit) == _TRUE)
      {
        if(po->onscreen == _TRUE)
        {
//        AddOffset(offset, &po->room.xy1, zoom); // soog +dx +dy
          //AddOffset(offset, &po->oldxy, zoom); // soog +dx +dy
          _DOT tmp_offset = po->oldxy;
          _ZOOM tmp_zoom = *zoom;
          AddZoom(&tmp_zoom, &po->zoom);
          retcode = action_SEEKELEMENT_sub(pr, p+1, po->SubPicture, press_state, Pw, &tmp_offset, &tmp_zoom, lrm);
        }
      }
    }
  }

  return(retcode);
}

_BOOL action_KARU(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _BOOL press_state, _Window *Pw, _DOT *offset, _ZOOM *zoom, _BOOL lrm)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _MOTION      *pmot;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    if(p->no < pm->memmotion.no)
    {
      pmot = &pm->memmotion.ptr[p->no];
      if(chk_CHKBIT(&pmot->chkbit) == _TRUE)
      {
        if(pmot->onscreen == _TRUE)
        {
          if(p->karuindex < pmot->memkaru.no)
          {
            _KARU *pkaru = &pmot->memkaru.ptr[p->karuindex];
            //if((pkaru->onscreen == _TRUE) && (pkaru->gesamt == _TRUE))
            {
              _DOT tmp_offset = pkaru->room.xy1;
              _ZOOM tmp_zoom = pkaru->zoom;
              retcode = action_SEEKELEMENT_sub(pr, p+1, pkaru->SubPicture, press_state, Pw, &tmp_offset, &tmp_zoom, lrm);
            }
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL action_OUTPUT(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _BOOL press_state, _Window *Pw, _DOT *offset, _ZOOM *zoom, _BOOL lrm)
{
  _BOOL        retcode = _FALSE;
  _PICTUREMASK *pm;
  _IO          *pio;
  _UWORD       index;
  _ZOOM        tmp_zoom;

  if(Sp != NULL)
  {
    pm = &Sp->GetPICTURE()->mask;
    index = p->no;
    if(index < pm->memoutput.no)
    {
      pio = &pm->memoutput.ptr[index];
      if(pio->Obj != NULL)
      {
        if(chk_CHKBIT(&pio->chkbit) == _TRUE)
        {
          AddOffset(offset, &pio->room.xy1, zoom); // soog +dx +dy
          Make_ZOOM(&tmp_zoom, pio->Obj, &pio->space);
//         #ifdef ZOOM_KM
//          _ZOOM kmz = do_kmz(zoom, &pio->kmz);
//          retcode = action_SEEKELEMENT_sub(pr, p+1, pio->Obj, press_state, Pw, offset, &kmz, lrm);
//         #else
          retcode = action_SEEKELEMENT_sub(pr, p+1, pio->Obj, press_state, Pw, offset, &tmp_zoom, lrm);
//         #endif
        }
      }
    }
  }

  return(retcode);
}


_BOOL action_SEEKELEMENT_sub(_ROOM *pr, _SINGLESEEK *p, _SubPicture *Sp, _BOOL press_state, _Window *Pw, _DOT *offset, _ZOOM *zoom, _BOOL lrm)
{
  _BOOL retcode = _FALSE;

  if(Sp != NULL)
  {
    switch(p->location)
    {
      case SL_OBJECT : retcode = action_OBJECT(pr, p, Sp, press_state, Pw, offset, zoom, lrm);
                       break;
      case SL_MOTION : retcode = action_MOTION(pr, p, Sp, press_state, Pw, offset, zoom, lrm);
                       break;
      case SL_KARU   : retcode = action_KARU(pr, p, Sp, press_state, Pw, offset, zoom, lrm);
                       break;
      case SL_BUTTON : retcode = action_BUTTON(p, Sp, press_state, Pw, offset, zoom, lrm);
                       break;
      case SL_OUTPUT : retcode = action_OUTPUT(pr, p, Sp, press_state, Pw, offset, zoom, lrm);
                       break;
      default        : break;
    }
  }

  return(retcode);
}

_BOOL action_SEEKELEMENT(_MY_PICTURE *p, _UWORD no, _BOOL press_state, _Window *Pw, _BOOL lrm)
{
  _BOOL        retcode = _FALSE;
  _SINGLESEEK  *ps;
  _UWORD       index;
  _SEEKELEMENT *pse;
  _ROOM        room;
  _DOT         offset;
  _ZOOM        zoom;

  init_DOT(&offset);
  init_ZOOM(&zoom);

  if(no < p->memseekelement.no)
  {
    pse = &p->memseekelement.ptr[no];
    if(pse->no > 1)
    {
      ps = &pse->ptr[0];

      if(ps->location == SL_LAYER)
      {
        index = ps->no;
        if(index < p->No)
        {
          if(position_SEEKELEMENT(&room, p, no, &offset, &zoom) == _TRUE)
          {
            retcode = action_SEEKELEMENT_sub(&room, ps+1, &p->Ptr[index], press_state, Pw, &offset, &zoom, lrm);
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL hotkey_SEEKELEMENT(_MY_PICTURE *p, _Window *Pw, _BOOL press_state, _EVENT *pe)
{
  _BOOL        retcode = _FALSE;
  _UWORD       i;
  _SEEKELEMENT *pss;

  if(pe->scancode != _NOKEY)
  {
    pss = p->memseekelement.ptr;

    for(i=0; i<p->memseekelement.no; i++)
    {
      if(pe->scancode == pss->hotkeycode)
      {
        action_SEEKELEMENT(p, i, press_state, Pw, pe->lrm);
        retcode = _TRUE;
      }
      pss += 1;
    }
  }
  return(retcode);
}

_BOOL is_any_input_accessible(_MY_PICTURE *p, _SELECTOR selector)
{
  _SEEKELEMENT *pse;
  _UWORD       i;
  _SINGLESEEK  *ps;

  for(i=0; i<p->memseekelement.no; i++)
  {
    pse = &p->memseekelement.ptr[i];
    if(pse->no > 1)
    {
      ps = &pse->ptr[0];

      if(ps->location == SL_LAYER)
      {
        if(access_SEEKELEMENT(p, i, _TRUE, selector) == _TRUE)
          return(_TRUE);
      }
    }
  }

  return(_FALSE);
}

_BOOL find_input_by_varno(_UWORD *erg, _MY_PICTURE *p, _UDWORD varno)
{
  _UWORD i;
  _INPUT *pi;

  *erg = 0xFFFF;

  for(i=0; i<p->memseekelement.no; i++)
  {
    pi = p_input_SEEKELEMENT(p, i, _FALSE);

    if(pi != NULL)
    {
      if(valid_VARLABEL(&pi->io.variable) == _TRUE)
      {
        if(pi->io.variable.info[0].value == varno)
        {
          if(access_SEEKELEMENT(p, i, _FALSE, _SELECTOR_DEFAULT) == _TRUE)
          {
            *erg = i;
            return(_TRUE);
          }
        }
      }
    }
  }

  return(_FALSE);
}


_UWORD cmp_SEEKELEMENT(_SEEKELEMENT *p1, _SEEKELEMENT *p2)
{
  _UWORD      i;
  _SINGLESEEK *pt1, *pt2;

  if(p1->no > p2->no)
  {
    pt1 = p1->ptr;
    pt2 = p2->ptr;
    for(i=0; i<p2->no; i++)
    {
      if((pt2->location != pt1->location)||(pt2->no != pt1->no))
        return(1);
    }
    return(0);
  }

  return(1);
}

#ifdef USER_BILLION
_BOOL invert_color_input_by_varno(_MY_PICTURE *p, _UDWORD varno)
{
  _UWORD i;
  _INPUT *pi;
  _COLOR color;

  for(i=0; i<p->memseekelement.no; i++)
  {
    pi = p_input_SEEKELEMENT(p, i, _FALSE);

    if(pi != NULL)
    {
      if(valid_VARLABEL(&pi->io.variable) == _TRUE)
      {
        if(pi->io.variable.info[0].value == varno)
        {
          if(access_SEEKELEMENT(p, i, _FALSE, _SELECTOR_DEFAULT) == _TRUE)
          {
            color = pi->io.colback;
            pi->io.colback = pi->io.coltxt;
            pi->io.coltxt  = color;
            pi->io.cstate.value = ~pi->io.cstate.value;
            return(_TRUE);
          }
        }
      }
    }
  }

  return(_FALSE);
}


_BOOL _Picture::InvertInputColor(_UDWORD varno)
{
  return(invert_color_input_by_varno(&Pic, varno));
}

_BOOL _Window::InvertInputColor(_UDWORD varno)
{
  if(P_Picture != NULL)
    return(P_Picture->InvertInputColor(varno));
  return(_FALSE);
}

_BOOL InvertInputColor(_UDWORD varno)
{
  return(Root.FindTop()->InvertInputColor(varno));
}
#endif // USER_BILLION

