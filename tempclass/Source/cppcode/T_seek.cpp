#include "T_IPC.H"
#include "T_NUC.H"


_UWORD _SubPicture::GetLastEditor(void)
{
  return(Picture.picrunmemo.last_editorno);
}

void _SubPicture::SetLastEditor(_UWORD no)
{
  Picture.picrunmemo.last_editorno = no;
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

_BOOL seek_shuttle(_UWORD *no, _MY_PICTURE *mp, _BOOL incdec, _UWORD actno, _DOT *offset, _ZOOM *zoom, _SELECTOR selector)
{
  _MEM_SEEKELEMENT *ms;
  _UWORD           i, noi;
  _ROOM            space;

  ms = &mp->memseekelement;

  if(ms->no > 0)
  {
    if(incdec == _TRUE)
    {
      noi = (actno < ms->no)? (actno+1) : 0;
      for(i=0; i<ms->no; i++)
      {
        noi = (noi < ms->no)? noi : 0;
        if(noi != actno)
        {
          if(position_access_SEEKELEMENT(&space, mp, noi, offset, zoom, selector) == _TRUE)
          {
            *no = noi;
            return(_TRUE);
          }
        }
        noi++;
      }
    }
    else
    {
      if(actno < ms->no)
      {
        noi = actno - 1;
        if(actno == 0)
          noi = ms->no - 1;
      }
      else
      {
        noi = ms->no - 1;
      }

      for(i=0; i<ms->no; i++)
      {
        if(noi != actno)
        {
          if(position_access_SEEKELEMENT(&space, mp, noi, offset, zoom, _SELECTOR_DEFAULT) == _TRUE)
          {
            *no = noi;
            return(_TRUE);
          }
        }
        if(noi == 0)
          noi = ms->no - 1;
        else
          noi --;
      }

    }
  }

  return(_FALSE);
}

_BOOL position_access_SEEKELEMENT(_ROOM *space, _MY_PICTURE *mp, _UWORD no, _DOT *offset, _ZOOM *zoom, _SELECTOR selector)
{
  _BOOL retcode = _FALSE;

  if(position_SEEKELEMENT(space, mp, no, offset, zoom) == _TRUE)
    retcode = access_SEEKELEMENT(mp, no, _TRUE, selector);

  return(retcode);
}

// ***************************************************************************
// NEUTRAL *******************************************************************
// ***************************************************************************

#ifdef OK_SEEK_NEXT
_UWORD seek_next(_MY_PICTURE *mp, _UWORD layer, _WORD actno, _BOOL horiz, _UWORD kinkycount, _SELECTOR selector)
{
  _UWORD       i, tmp, retcode = 0xFFFF;
  _SEEKELEMENT *se;
  _WORD        act;
  _WORD        maxact = 32000;
  _DOT         offset;
  _ZOOM        zoom;
  _ROOM        room;
  _UWORD       index = mp->memseekelement.no;

  if(mp->memseekelement.no > 0)
  {
    act = -1;
    if((actno >= 0)&&(actno < mp->memseekelement.no))
      act = (horiz == _TRUE)? mp->memseekelement.ptr[actno].direction.h_next : mp->memseekelement.ptr[actno].direction.v_next;

    se = mp->memseekelement.ptr;
    for(i=0; i<mp->memseekelement.no; i++)
    {
      tmp = (horiz == _TRUE)? se->direction.h_next : se->direction.v_next;
      if((tmp > act)&&(tmp < maxact))
      {
        maxact = tmp;
        index  = i;
      }
      se += 1;
    }

    init_DOT(&offset);
    init_ZOOM(&zoom);

    if(kinkycount < mp->memseekelement.no)
    {
      if(index < mp->memseekelement.no)
      {
        if(index != actno)
        {
          if(position_access_SEEKELEMENT(&room, mp, index, &offset, &zoom, selector) == _TRUE)
            retcode = index;
          else
            retcode = seek_next(mp, layer, index, horiz, kinkycount+1, selector);
        }
      }
      else
      {
        retcode = seek_next(mp, layer, -1, horiz, kinkycount+1, selector);
      }
    }
  }

  return(retcode);
}

_UWORD seek_last(_MY_PICTURE *mp, _UWORD layer, _WORD actno, _BOOL horiz, _UWORD kinkycount, _SELECTOR selector)
{
  _UWORD       i, tmp, retcode = 0xFFFF;
  _SEEKELEMENT *se;
  _WORD        act;
  _WORD        maxact = -1;
  _UWORD       index = mp->memseekelement.no;
  _DOT         offset;
  _ZOOM        zoom;
  _ROOM        room;

  if(mp->memseekelement.no > 0)
  {
    act = 32000;
    if((actno >= 0)&&(actno < mp->memseekelement.no))
      act = (horiz == _TRUE)? mp->memseekelement.ptr[actno].direction.h_next : mp->memseekelement.ptr[actno].direction.v_next;

    se = mp->memseekelement.ptr;
    for(i=0; i<mp->memseekelement.no; i++)
    {
      tmp = (horiz == _TRUE)? se->direction.h_next : se->direction.v_next;
      if((tmp < act)&&(tmp > maxact))
      {
        maxact = tmp;
        index  = i;
      }
      se += 1;
    }

    init_DOT(&offset);
    init_ZOOM(&zoom);

    if(kinkycount < mp->memseekelement.no)
    {
      if(index < mp->memseekelement.no)
      {
        if(index != actno)
        {
          if(position_access_SEEKELEMENT(&room, mp, index, &offset, &zoom, selector) == _TRUE)
            retcode = index;
          else
            retcode = seek_last(mp, layer, index, horiz, kinkycount+1, selector);
        }
      }
      else
      {
        retcode = seek_last(mp, layer, 32000, horiz, kinkycount+1, selector);
      }
    }
  }

  return(retcode);
}
#endif

_UWORD seek_vertical_line(_MY_PICTURE *mp, _UWORD layer, _UWORD oldno, _ROOM *pr, _BOOL down, _DOT *offset, _ZOOM *zoom, _SELECTOR selector)
{
  _UWORD retcode = oldno;
  _UWORD i;
  _KOORD yy, acty;
  _ROOM  space;
  _MEM_SEEKELEMENT *ms;

  if(chk_ROOM(pr) == _TRUE)
  {
    ms = &mp->memseekelement;
    acty = 10000;

    for(i=0; i<ms->no; i++)
    {
      if((ms->ptr[i].no > 1) && (i != oldno))
     #ifndef UC_NO_TAB_CURSORNAVIGATION
      if(ms->ptr[i].ptr[0].no == layer)
     #endif
      {
        if(position_access_SEEKELEMENT(&space, mp, i, offset, zoom, selector) == _TRUE)
        {
          if((space.xy2.x >= pr->xy1.x) && (space.xy1.x <= pr->xy2.x))
          {
            if(down == _TRUE)
            {
              yy = space.xy1.y - pr->xy2.y;
            }
            else
            {
              yy = pr->xy1.y - space.xy2.y;
            }

            if((yy > 0) && (yy < acty))
            {
              acty    = yy;
              retcode = i;
            }
          }
        }
      }
    }
  }

  return(retcode);
}

_UWORD seek_horizont_line(_MY_PICTURE *mp, _UWORD layer, _UWORD oldno, _ROOM *pr, _BOOL right, _DOT *offset, _ZOOM *zoom, _SELECTOR selector)
{
  _UWORD retcode = oldno;
  _UWORD i;
  _KOORD xx, actx;
  _ROOM  space;
  _MEM_SEEKELEMENT *ms;

  if(chk_ROOM(pr) == _TRUE)
  {
    ms = &mp->memseekelement;
    actx = 10000;

    for(i=0; i<ms->no; i++)
    {
      if((ms->ptr[i].no > 1) && (i != oldno))
     #ifndef UC_NO_TAB_CURSORNAVIGATION
      if(ms->ptr[i].ptr[0].no == layer)
     #endif
      {
        if(position_access_SEEKELEMENT(&space, mp, i, offset, zoom, selector) == _TRUE)
        {
          if((space.xy2.y >= pr->xy1.y) && (space.xy1.y <= pr->xy2.y))
          {
            if(right == _TRUE)
            {
              xx = space.xy1.x - pr->xy2.x;
            }
            else
            {
              xx = pr->xy1.x - space.xy2.x;
            }

            if((xx > 0) && (xx < actx))
            {
              actx    = xx;
              retcode = i;
            }
          }
        }
      }
    }
  }

  return(retcode);
}

#ifndef USER_KM
// ***************************************************************************
// ***************************************************************************
// SIGMATEK STANDARD-CURSOR_NAVIGATION ***************************************
// SIGMATEK STANDARD-CURSOR_NAVIGATION ***************************************
// ***************************************************************************
// ***************************************************************************
_UWORD seek_vertical_virt(_MY_PICTURE *mp, _UWORD layer, _UWORD oldno, _ROOM *pr, _BOOL down, _DOT *offset, _ZOOM *zoom, _SELECTOR selector)
{
  _UWORD retcode = oldno;
  _UWORD i;
  _KOORD xx, yy, xy, actxy;
  _ROOM  space;
  _MEM_SEEKELEMENT *ms;

  if(chk_ROOM(pr) == _TRUE)
  {
    ms = &mp->memseekelement;
    actxy = 10000;

    for(i=0; i<ms->no; i++)
    {
      if((ms->ptr[i].no > 1) && (i != oldno))
     #ifndef UC_NO_TAB_CURSORNAVIGATION
      if(ms->ptr[i].ptr[0].no == layer)
     #endif
      {
        if(position_access_SEEKELEMENT(&space, mp, i, offset, zoom, selector) == _TRUE)
        {
          xy = actxy;

          if(down == _TRUE)
          {
            xx = space.xy1.x - pr->xy2.x;
            yy = space.xy2.y - pr->xy2.y;
            if((xx <= 0) || (yy <= 0))
            {
              xx = pr->xy1.x - space.xy2.x;
              yy = space.xy2.y - pr->xy2.y;
            }

            if((xx > 0) && (yy > 0) && (xx <= yy))
            {
              xx = space.xy1.x - pr->xy1.x;
              yy = space.xy1.y - pr->xy1.y;
              xy = sqroot((yy * yy) + (xx * xx));
            }
          }
          else
          {
            xx = space.xy1.x - pr->xy2.x;
            yy = pr->xy1.y - space.xy1.y;
            if((xx <= 0) || (yy <= 0))
            {
              xx = pr->xy1.x - space.xy2.x;
              yy = pr->xy1.y - space.xy1.y;
            }

            if((xx > 0) && (yy > 0) && (xx <= yy))
            {
              xx = pr->xy1.x - space.xy1.x;
              yy = pr->xy1.y - space.xy1.y;
              xy = sqroot((yy * yy) + (xx * xx));
            }
          }

          if(xy < actxy)
          {
            actxy   = xy;
            retcode = i;
          }
        }
      }
    }
  }

  return(retcode);
}

_UWORD seek_horizont_virt(_MY_PICTURE *mp, _UWORD layer, _UWORD oldno, _ROOM *pr, _BOOL right, _DOT *offset, _ZOOM *zoom, _SELECTOR selector)
{
  _UWORD retcode = oldno;
  _UWORD i;
  _KOORD xx, yy, xy, actxy;
  _ROOM  space;
  _MEM_SEEKELEMENT *ms;

  if(chk_ROOM(pr) == _TRUE)
  {
    ms = &mp->memseekelement;
    actxy = 10000;

    for(i=0; i<ms->no; i++)
    {
      if((ms->ptr[i].no > 1) && (i != oldno))
     #ifndef UC_NO_TAB_CURSORNAVIGATION
      if(ms->ptr[i].ptr[0].no == layer)
     #endif
      {
        if(position_access_SEEKELEMENT(&space, mp, i, offset, zoom, selector) == _TRUE)
        {
          xy = actxy;

          if(right == _TRUE)
          {
            xx = space.xy2.x - pr->xy2.x;
            yy = space.xy1.y - pr->xy2.y;
            if((xx <= 0) || (yy <= 0))
            {
              xx = space.xy2.x - pr->xy2.x;
              yy = pr->xy1.y - space.xy2.y;
            }

            if((xx > 0) && (yy > 0) && (xx >= yy))
            {
              xx = space.xy1.x - pr->xy1.x;
              yy = space.xy1.y - pr->xy1.y;
              xy = sqroot((yy * yy) + (xx * xx));
            }
          }
          else
          {
            xx = pr->xy1.x - space.xy2.x;
            yy = space.xy1.y - pr->xy2.y;
            if((xx <= 0) || (yy <= 0))
            {
              xx = pr->xy1.x - space.xy2.x;
              yy = pr->xy1.y - space.xy2.y;
            }

            if((xx > 0) && (yy > 0) && (xx >= yy))
            {
              xx = pr->xy1.x - space.xy1.x;
              yy = pr->xy1.y - space.xy1.y;
              xy = sqroot((yy * yy) + (xx * xx));
            }
          }

          if(xy < actxy)
          {
            actxy   = xy;
            retcode = i;
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL seek_horizontal(_UWORD *no, _MY_PICTURE *mp, _UWORD layer, _BOOL right, _UWORD actno, _BOOL algorithmus, _DOT *offset, _ZOOM *zoom, _SELECTOR selector)
{
  _UWORD retcode = 0xFFFF;
  _ROOM  room, hlp;

  if(mp->memseekelement.no > 0)
  {
    if(algorithmus == _TRUE)
    {
      // aktuelle eingabe überprüfen
      if(position_access_SEEKELEMENT(&room, mp, actno, offset, zoom, _SELECTOR_DEFAULT) == _TRUE)
      {
        retcode = seek_horizont_line(mp, layer, actno, &room, right, offset, zoom, selector);

        if(retcode == actno)
		{
          retcode = seek_horizont_virt(mp, layer, actno, &room, right, offset, zoom, selector);
        }

        if(retcode == actno)
        {
          if(get_cursorwraphorizontal() == _TRUE)
          {
            if(right == _TRUE)
            {
              room.xy2.x = 1; //room.xy2.x - room.xy1.x;
              room.xy1.x = 0;
            }
            else
            {
              MyGetScreenSize(&hlp);
              room.xy1.x = hlp.xy2.x - 1; //(room.xy2.x - room.xy1.x);
              room.xy2.x = hlp.xy2.x;
            }

            retcode = seek_horizont_line(mp, layer, actno, &room, right, offset, zoom, selector);
          }
        }

       #ifdef VARIANTE3
        if(retcode == actno)
        {
          retcode = seek_vektor(mp, layer, actno, &room, right, offset, zoom, _TRUE, selector);
        }
       #endif
      }
      else
      {
        room.xy1.x = -1;
        room.xy1.y = -1;
        room.xy2.x = 0;
        room.xy2.y = 0;
        retcode = seek_horizont_virt(mp, layer, actno, &room, _TRUE, offset, zoom, selector);
        if(retcode == actno)
          retcode = seek_vertical_virt(mp, layer, actno, &room, _TRUE, offset, zoom, selector);
      }
    }
   #ifdef OK_SEEK_NEXT
    else
    {
      if(right == _TRUE)
        retcode = seek_next(mp, layer, actno, _TRUE, 0, selector);
      else
        retcode = seek_last(mp, layer, actno, _TRUE, 0, selector);
    }
   #endif
  }

  *no = retcode;

  return((retcode < mp->memseekelement.no)? _TRUE : _FALSE);
}

_BOOL seek_vertical(_UWORD *no, _MY_PICTURE *mp, _UWORD layer, _BOOL down, _UWORD actno, _BOOL algorithmus, _DOT *offset, _ZOOM *zoom, _SELECTOR selector)
{
  _UDWORD retcode = 0xFFFF;
  _ROOM   room, hlp;

  if(mp->memseekelement.no > 0)
  {
    if(algorithmus == _TRUE)
    {
      // aktuelle eingabe überprüfen
      if(position_access_SEEKELEMENT(&room, mp, actno, offset, zoom, _SELECTOR_DEFAULT) == _TRUE)
      {
        retcode = seek_vertical_line(mp, layer, actno, &room, down, offset, zoom, selector);

        if(retcode == actno)
		{
          retcode = seek_vertical_virt(mp, layer, actno, &room, down, offset, zoom, selector);
        }

        if(retcode == actno)
		{
          if(get_cursorwrapvertical() == _TRUE)
          {
            if(down == _TRUE)
            {
              room.xy2.y = 1;
              room.xy1.y = 0;
            }
            else
            {
              MyGetScreenSize(&hlp);
              room.xy1.y = hlp.xy2.y - 1;
              room.xy2.y = hlp.xy2.y;
            }
            retcode = seek_vertical_line(mp, layer, actno, &room, down, offset, zoom, selector);
          }
        }

       #ifdef VARIANTE3
        if(retcode == actno)
        {
          retcode = seek_vektor(mp, layer, actno, &room, down, offset, zoom, _FALSE, selector);
        }
       #endif
      }
      else
      {
        room.xy1.x = -1;
        room.xy1.y = -1;
        room.xy2.x = 0;
        room.xy2.y = 0;
        retcode = seek_vertical_virt(mp, layer, actno, &room, _TRUE, offset, zoom, selector);
        if(retcode == actno)
          retcode = seek_horizont_virt(mp, layer, actno, &room, _TRUE, offset, zoom, selector);
      }
    }
   #ifdef OK_SEEK_NEXT
    else
    {
      if(down == _TRUE)
        retcode = seek_next(mp, layer, actno, _FALSE, 0, selector);
      else
        retcode = seek_last(mp, layer, actno, _FALSE, 0, selector);
    }
   #endif
  }

  *no = retcode;

  return((retcode < mp->memseekelement.no)? _TRUE : _FALSE);
}
#else
// ***************************************************************************
// ***************************************************************************
// KM-CURSOR-NAVIGATION ******************************************************
// KM-CURSOR-NAVIGATION ******************************************************
// ***************************************************************************
// ***************************************************************************
void calc_middle(_DOT *pd, _ROOM *pr)
{
  pd->x = (pr->xy2.x + pr->xy1.x) / 2;
  pd->y = (pr->xy2.y + pr->xy1.y) / 2;
}

_UWORD seek_shortest_vektor(_MY_PICTURE *mp, _UWORD layer, _UWORD oldno, _ROOM *pr, _BOOL right_down, _DOT *offset, _ZOOM *zoom, _BOOL horizontal, _SELECTOR selector)
{
  _UWORD retcode = oldno;
  _UWORD i;
  _ROOM  space;
  _MEM_SEEKELEMENT *ms;

  _DOT   dot, vdot;
  _DWORD xx, yy, v, vmax;

  if(chk_ROOM(pr) == _TRUE)
  {
    calc_middle(&dot, pr);

    ms   = &mp->memseekelement;
    vmax = 100000;

    for(i=0; i<ms->no; i++)
    {
      if((ms->ptr[i].no > 1) && (i != oldno))
     #ifndef UC_NO_TAB_CURSORNAVIGATION
      if(ms->ptr[i].ptr[0].no == layer)
     #endif
      {
        if(position_access_SEEKELEMENT(&space, mp, i, offset, zoom, selector) == _TRUE)
        {
          calc_middle(&vdot, &space);

          v  = vmax;

          if(horizontal == _TRUE)
          {
            if(right_down == _TRUE)
            {
              if(vdot.x > dot.x)
              {
                xx = (vdot.x - dot.x);
                yy = (vdot.y - dot.y);
                v  = sqroot((xx*xx) + (yy*yy));
              }
            }
            else
            {
              if(vdot.x < dot.x)
              {
                xx = (dot.x - vdot.x);
                yy = (dot.y - vdot.y);
                v  = sqroot((xx*xx) + (yy*yy));
              }
            }
          }
          else
          {
            if(right_down == _TRUE)
            {
              if(vdot.y > dot.y)
              {
                xx = (vdot.x - dot.x);
                yy = (vdot.y - dot.y);
                v  = sqroot((xx*xx) + (yy*yy));
              }
            }
            else
            {
              if(vdot.y < dot.y)
              {
                xx = (dot.x - vdot.x);
                yy = (dot.y - vdot.y);
                v  = sqroot((xx*xx) + (yy*yy));
              }
            }
          }

          if(v < vmax)
          {
            vmax = v;
            retcode = i;
          }
        }
      }
    }
  }

  return(retcode);
}

_UWORD seek_smallest_angle(_MY_PICTURE *mp, _UWORD layer, _UWORD oldno, _ROOM *pr, _BOOL right_down, _DOT *offset, _ZOOM *zoom, _BOOL horizontal, _SELECTOR selector)
{
  _UWORD retcode = oldno;
  _UWORD i;
  _ROOM  space;
  _MEM_SEEKELEMENT *ms;

  _DOT   dot, vdot;
  _DWORD v, vmax;

  if(chk_ROOM(pr) == _TRUE)
  {
    calc_middle(&dot, pr);

    ms   = &mp->memseekelement;
    vmax = 100000;

    for(i=0; i<ms->no; i++)
    {
      if((ms->ptr[i].no > 1) && (i != oldno))
     #ifndef UC_NO_TAB_CURSORNAVIGATION
      if(ms->ptr[i].ptr[0].no == layer)
     #endif
      {
        if(position_access_SEEKELEMENT(&space, mp, i, offset, zoom, selector) == _TRUE)
        {
          calc_middle(&vdot, &space);
          v = vmax;

          if(horizontal == _TRUE)
          {
            if(right_down == _TRUE)
            {
              if(vdot.x > dot.x)
                v = vdot.y - dot.y;
            }
            else
            {
              if(vdot.x < dot.x)
                v = vdot.y - dot.y;
            }
          }
          else
          {
            if(right_down == _TRUE)
            {
              if(vdot.y > dot.y)
                v = vdot.x - dot.x;
            }
            else
            {
              if(vdot.y < dot.y)
                v = vdot.x - dot.x;
            }
          }

          if(v < 0)
            v = -v;

          if(v < vmax)
          {
            vmax = v;
            retcode = i;
          }
        }
      }
    }
  }

  return(retcode);
}

_UWORD get_searchchoice(_MY_PICTURE *mp, _UWORD actno, _BOOL right_down, _BOOL horizontal)
{
  _UWORD retcode = 0;
  _INPUT *pi = p_input_SEEKELEMENT(mp, actno, _TRUE);


  if(pi != NULL)
  {
    if(horizontal == _TRUE)
    {
      if(right_down == _TRUE)
      {
        // right
        retcode = (pi->searchchoice >> 1) & 0x01;
      }
      else
      {
        // left
        retcode = pi->searchchoice & 0x01;
      }
    }
    else
    {
      if(right_down == _TRUE)
      {
        // down
        retcode = (pi->searchchoice >> 3) & 0x01;
      }
      else
      {
        // up
        retcode = (pi->searchchoice >> 2) & 0x01;
      }
    }
  }

  return(retcode);
}

_BOOL seek_horizontal(_UWORD *no, _MY_PICTURE *mp, _UWORD layer, _BOOL right, _UWORD actno, _BOOL algorithmus, _DOT *offset, _ZOOM *zoom, _SELECTOR selector)
{
  _UWORD retcode = 0xFFFF;
  _ROOM  room, hlp;

  if(mp->memseekelement.no > 0)
  {
    if(algorithmus == _TRUE)
    {
      // aktuelle position überprüfen
      if(position_access_SEEKELEMENT(&room, mp, actno, offset, zoom, _SELECTOR_DEFAULT) == _TRUE)
      {
        if(get_searchchoice(mp, actno, right, _TRUE) == 1) // suche nach kürzesten vertorabstand im 180grad winkel
        {
          retcode = seek_shortest_vektor(mp, layer, actno, &room, right, offset, zoom, _TRUE, selector);

          if((retcode == actno)&&(get_cursorwraphorizontal() == _TRUE))
          {
            if(right == _TRUE)
            {
              room.xy1.x = -1;
              room.xy2.x = 0; //room.xy2.x - room.xy1.x;
            }
            else
            {
              MyGetScreenSize(&hlp);
              room.xy1.x = hlp.xy2.x; //(room.xy2.x - room.xy1.x);
              room.xy2.x = hlp.xy2.x + 1;
            }

            retcode = seek_shortest_vektor(mp, layer, actno, &room, right, offset, zoom, _TRUE, selector);
          }
        }
        else // suche erst in zeile dann kleinster winkel (länge gegenkathete)
        {
          retcode = seek_horizont_line(mp, layer, actno, &room, right, offset, zoom, selector);

          if(retcode == actno)
            retcode = seek_smallest_angle(mp, layer, actno, &room, right, offset, zoom, _TRUE, selector);

          if((retcode == actno)&&(get_cursorwraphorizontal() == _TRUE))
          {
            if(right == _TRUE)
            {
              room.xy1.x = -1;
              room.xy2.x = 0; //room.xy2.x - room.xy1.x;
            }
            else
            {
              MyGetScreenSize(&hlp);
              room.xy1.x = hlp.xy2.x; //(room.xy2.x - room.xy1.x);
              room.xy2.x = hlp.xy2.x + 1;
            }

            retcode = seek_horizont_line(mp, layer, actno, &room, right, offset, zoom, selector);
          }
        }
      }
      else
      {
        room.xy1.x = -1;
        room.xy1.y = -1;
        room.xy2.x = 0;
        room.xy2.y = 0;
        retcode = seek_shortest_vektor(mp, layer, actno, &room, _TRUE, offset, zoom, _TRUE, selector);
      }
    }
   #ifdef OK_SEEK_NEXT
    else
    {
      if(right == _TRUE)
        retcode = seek_next(mp, layer, actno, _TRUE, 0, selector);
      else
        retcode = seek_last(mp, layer, actno, _TRUE, 0, selector);
    }
   #endif
  }

  *no = retcode;

  return((retcode < mp->memseekelement.no)? _TRUE : _FALSE);
}

_BOOL seek_vertical(_UWORD *no, _MY_PICTURE *mp, _UWORD layer, _BOOL down, _UWORD actno, _BOOL algorithmus, _DOT *offset, _ZOOM *zoom, _SELECTOR selector)
{
  _UDWORD retcode = 0xFFFF;
  _ROOM   room, hlp;

  if(mp->memseekelement.no > 0)
  {
    if(algorithmus == _TRUE)
    {
      // aktuelle eingabe überprüfen
      if(position_access_SEEKELEMENT(&room, mp, actno, offset, zoom, _SELECTOR_DEFAULT) == _TRUE)
      {
        if(get_searchchoice(mp, actno, down, _FALSE) == 1) // suche nach kürzesten vertorabstand im 180grad winkel
        {
          retcode = seek_shortest_vektor(mp, layer, actno, &room, down, offset, zoom, _FALSE, selector);

          if((retcode == actno)&&(get_cursorwrapvertical() == _TRUE))
          {
            if(down == _TRUE)
            {
              room.xy1.y = -1;
              room.xy2.y = 0;
            }
            else
            {
              MyGetScreenSize(&hlp);
              room.xy1.y = hlp.xy2.y;
              room.xy2.y = hlp.xy2.y + 1;
            }

            retcode = seek_shortest_vektor(mp, layer, actno, &room, down, offset, zoom, _FALSE, selector);
          }
        }
        else
        {
          retcode = seek_vertical_line(mp, layer, actno, &room, down, offset, zoom, selector);

          if(retcode == actno)
            retcode = seek_smallest_angle(mp, layer, actno, &room, down, offset, zoom, _FALSE, selector);

          if((retcode == actno)&&(get_cursorwrapvertical() == _TRUE))
          {
            if(down == _TRUE)
            {
              room.xy1.y = -1;
              room.xy2.y = 0;
            }
            else
            {
              MyGetScreenSize(&hlp);
              room.xy1.y = hlp.xy2.y;
              room.xy2.y = hlp.xy2.y + 1;
            }

            retcode = seek_vertical_line(mp, layer, actno, &room, down, offset, zoom, selector);
          }
        }
       #ifdef VARIANTE3
        else if(retcode == actno)
        {
          retcode = seek_vektor(mp, layer, actno, &room, down, offset, zoom, _FALSE, selector);
        }
       #endif
      }
      else
      {
        room.xy1.x = -1;
        room.xy1.y = -1;
        room.xy2.x = 0;
        room.xy2.y = 0;
        retcode = seek_shortest_vektor(mp, layer, actno, &room, _TRUE, offset, zoom, _FALSE, selector);
      }
    }
   #ifdef OK_SEEK_NEXT
    else
    {
      if(down == _TRUE)
        retcode = seek_next(mp, layer, actno, _FALSE, 0, selector);
      else
        retcode = seek_last(mp, layer, actno, _FALSE, 0, selector);
    }
   #endif
  }

  *no = retcode;

  return((retcode < mp->memseekelement.no)? _TRUE : _FALSE);
}


#endif
