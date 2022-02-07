#include "T_IPC.H"
#include "T_NUC.H"

void move_MY_PICTURE(_MY_PICTURE*, _KOORD, _KOORD, _ATTRIB);

void add_DOT(_DOT *p, _KOORD x, _KOORD y)
{
  p->x += x;
  p->y += y;
}
void add_ROOM(_ROOM *p, _KOORD x, _KOORD y)
{
  p->xy1.x += x;
  p->xy1.y += y;
  p->xy2.x += x;
  p->xy2.y += y;
}
void add_RECTANGLE(_RECTANGLE *p, _KOORD x, _KOORD y)
{
  add_ROOM(&p->room, x, y);
}
void add_POLYGON(_POLYGON *p, _KOORD x, _KOORD y)
{
  _UWORD i;
  for(i=0; i<p->no; i++)
    add_DOT(&p->ptr[i], x, y);
}
void add_CIRCLE(_CIRCLE *p, _KOORD x, _KOORD y)
{
  add_ROOM(&p->room, x, y);
}
void add_PICTEXT(_PICTEXT *p, _KOORD x, _KOORD y)
{
  add_ROOM(&p->room, x, y);
}
void add_OBJECT(_OBJECT *p, _KOORD x, _KOORD y)
{
  add_DOT(&p->xy, x, y);
}
void add_IO(_IO *p, _KOORD x, _KOORD y)
{
  add_ROOM(&p->room, x, y);
}
void add_INPUT(_INPUT *p, _KOORD x, _KOORD y)
{
  add_IO(&p->io, x, y);
}
void add_COL_RECT(_COL_RECT *p, _KOORD x, _KOORD y)
{
  add_RECTANGLE(&p->rectangle, x, y);
}
void add_BITMAP(_BITMAP *p, _KOORD x, _KOORD y)
{
  add_ROOM(&p->room, x, y);
}
void add_MOTION(_MOTION *p, _KOORD x, _KOORD y)
{
  add_ROOM(&p->room, x, y);
}
void add_COL_POLY(_COL_POLY *p, _KOORD x, _KOORD y)
{
  add_POLYGON(&p->poly, x, y);
}
void add_COL_CIRC(_COL_CIRC *p, _KOORD x, _KOORD y)
{
  add_CIRCLE(&p->circle, x, y);
}
void add_COL_PICTXT(_COL_PICTXT *p, _KOORD x, _KOORD y)
{
  add_PICTEXT(&p->btx, x, y);
}
void add_BUTTON(_BUTTON *p, _KOORD x, _KOORD y)
{
  add_ROOM(&p->room, x, y);
}

void add_PICTUREMASK(_PICTUREMASK *p, _KOORD x, _KOORD y)
{
  _UWORD i;

  add_ROOM(&p->room, x, y);

  for(i=0; i<p->memrectangle.no; i++) // _MEM_RECTANGLE
    add_RECTANGLE(&p->memrectangle.ptr[i], x, y);

  for(i=0; i<p->mempolygon.no; i++) // _MEM_POLYGON
    add_POLYGON(&p->mempolygon.ptr[i], x, y);

  for(i=0; i<p->memcircle.no; i++) // _MEM_CIRCLE
    add_CIRCLE(&p->memcircle.ptr[i], x, y);

  for(i=0; i<p->mempictxt.no; i++) // _MEM_PICTEXT
    add_PICTEXT(&p->mempictxt.ptr[i], x, y);

  for(i=0; i<p->memobject.no; i++) // _MEM_OBJECT
    add_OBJECT(&p->memobject.ptr[i], x, y);

  for(i=0; i<p->membitmap.no; i++) // _MEM_BITMAP
    add_BITMAP(&p->membitmap.ptr[i], x, y);

  for(i=0; i<p->meminput.no; i++) // _MEM_INPUT
    add_INPUT(&p->meminput.ptr[i], x, y);

  for(i=0; i<p->memoutput.no; i++) // _MEM_OUTPUT
    add_IO(&p->memoutput.ptr[i], x, y);

  for(i=0; i<p->memmotion.no; i++) // _MEM_MOTION
    add_MOTION(&p->memmotion.ptr[i], x, y);

  for(i=0; i<p->colrect.no; i++) // _MEM_COL_RECT
    add_COL_RECT(&p->colrect.ptr[i], x, y);

  for(i=0; i<p->colpoly.no; i++) // _MEM_COL_POLY
    add_COL_POLY(&p->colpoly.ptr[i], x, y);

  for(i=0; i<p->colcirc.no; i++) // _MEM_COL_CIRC
    add_COL_CIRC(&p->colcirc.ptr[i], x, y);

  for(i=0; i<p->colpictxt.no; i++) // _MEM_COL_PICTXT
    add_COL_PICTXT(&p->colpictxt.ptr[i], x, y);

  for(i=0; i<p->membutton.no; i++) // _MEM_BUTTON
    add_BUTTON(&p->membutton.ptr[i], x, y);
}

void add_PICTURE(_PICTURE *p, _KOORD x, _KOORD y)
{
  add_PICTUREMASK(&p->mask, x, y);
}

void add_MY_PICTURE(_MY_PICTURE *p, _KOORD x, _KOORD y)
{
  _UWORD i;

  add_ROOM(&p->Room, x, y);
  add_DOT(&p->FirstInput, x, y);
  for(i=0; i<p->No; i++)
    add_PICTURE(p->Ptr[i].GetPICTURE(), x, y);
}

void move_MY_PICTURE(_MY_PICTURE *p, _KOORD x, _KOORD y, _ATTRIB att)
{
  _KOORD xx, yy;

  if(p != NULL)
  {
    xx = p->Room.xy2.x - p->Room.xy1.x;
    yy = p->Room.xy2.y - p->Room.xy1.y;

    switch(att & T_HORIZBOUND)
    {
      case T_RIGHTBOUND  : x = x - xx   - p->Room.xy1.x; break;
      case T_CENTERBOUND : x = x - xx/2 - p->Room.xy1.x; break;
      default            : x = x        - p->Room.xy1.x; break;
    }

    switch(att & T_VERTBOUND)
    {
      case T_DOWNBOUND   : y = y - yy   - p->Room.xy1.y; break;
      case T_MIDBOUND    : y = y - yy/2 - p->Room.xy1.y; break;
      default            : y = y        - p->Room.xy1.y; break;
    }

    add_MY_PICTURE(p, x, y);
  }
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************


void make_size_ROOM(_ROOM *proom, _ROOM *pr)
{
  if(pr->xy1.x < proom->xy1.x) 
    proom->xy1.x = pr->xy1.x;
  if(pr->xy1.y < proom->xy1.y) 
    proom->xy1.y = pr->xy1.y;
    
  if(pr->xy2.x > proom->xy2.x) 
    proom->xy2.x = pr->xy2.x;
  if(pr->xy2.y > proom->xy2.y) 
    proom->xy2.y = pr->xy2.y;
}

void make_size_RECTANGLE(_ROOM *pr, _RECTANGLE *p)
{
  make_size_ROOM(pr, &p->room);
}

void make_size_POLYGON(_ROOM *pr, _POLYGON *p)
{
  _ROOM rx;
  for(_UWORD i=0; i<p->no; i++)
  {
    rx.xy1 = p->ptr[i];
    rx.xy2 = rx.xy1;
    make_size_ROOM(pr, &rx);
  }  
}

void make_size_CIRCLE(_ROOM *pr, _CIRCLE *p)
{
  make_size_ROOM(pr, &p->room);
}

void make_size_PICTEXT(_ROOM *pr, _PICTEXT *p)
{
  make_size_ROOM(pr, &p->room);
}

void make_size_OBJECT(_ROOM *pr, _OBJECT *p)
{
  _ROOM rx;
  rx.xy1 = p->xy;
  rx.xy2 = p->xy;
  make_size_ROOM(pr, &rx); 
  // eigentlich müsste grösse vom objekt * zoom berücksichtigt werden -> momentan aber nicht relevant
}

void make_size_BITMAP(_ROOM *pr, _BITMAP *p)
{
  make_size_ROOM(pr, &p->room);
}

void make_size_IO(_ROOM *pr, _IO *p)
{
  make_size_ROOM(pr, &p->room);
}

void make_size_INPUT(_ROOM *pr, _INPUT *p)
{
  make_size_IO(pr, &p->io);
}

void make_size_MOTION(_ROOM *pr, _MOTION *p)
{
  make_size_ROOM(pr, &p->room);
}

void make_size_COL_RECT(_ROOM *pr, _COL_RECT *p)
{
  make_size_RECTANGLE(pr, &p->rectangle);
}

void make_size_COL_POLY(_ROOM *pr, _COL_POLY *p)
{
  make_size_POLYGON(pr, &p->poly);
}

void make_size_COL_CIRC(_ROOM *pr, _COL_CIRC *p)
{
  make_size_CIRCLE(pr, &p->circle);
}

void make_size_COL_PICTXT(_ROOM *pr, _COL_PICTXT *p)
{
  make_size_PICTEXT(pr, &p->btx);
}

void make_size_BUTTON(_ROOM *pr, _BUTTON *p)
{
  make_size_ROOM(pr, &p->room);
}

void make_size_PICTUREMASK(_ROOM *proom, _PICTUREMASK *p)
{
  _UDWORD i;

  for(i=0; i<p->memrectangle.no; i++) // _MEM_RECTANGLE
    make_size_RECTANGLE(proom, &p->memrectangle.ptr[i]);

  for(i=0; i<p->mempolygon.no; i++) // _MEM_POLYGON
    make_size_POLYGON(proom, &p->mempolygon.ptr[i]);

  for(i=0; i<p->memcircle.no; i++) // _MEM_CIRCLE
    make_size_CIRCLE(proom, &p->memcircle.ptr[i]);

  for(i=0; i<p->mempictxt.no; i++) // _MEM_PICTEXT
    make_size_PICTEXT(proom, &p->mempictxt.ptr[i]);

  for(i=0; i<p->memobject.no; i++) // _MEM_OBJECT
    make_size_OBJECT(proom, &p->memobject.ptr[i]);

  for(i=0; i<p->membitmap.no; i++) // _MEM_BITMAP
    make_size_BITMAP(proom, &p->membitmap.ptr[i]);

  for(i=0; i<p->meminput.no; i++) // _MEM_INPUT
    make_size_INPUT(proom, &p->meminput.ptr[i]);

  for(i=0; i<p->memoutput.no; i++) // _MEM_OUTPUT
    make_size_IO(proom, &p->memoutput.ptr[i]);

  for(i=0; i<p->memmotion.no; i++) // _MEM_MOTION
    make_size_MOTION(proom, &p->memmotion.ptr[i]);

  for(i=0; i<p->colrect.no; i++) // _MEM_COL_RECT
    make_size_COL_RECT(proom, &p->colrect.ptr[i]);

  for(i=0; i<p->colpoly.no; i++) // _MEM_COL_POLY
    make_size_COL_POLY(proom, &p->colpoly.ptr[i]);

  for(i=0; i<p->colcirc.no; i++) // _MEM_COL_CIRC
    make_size_COL_CIRC(proom, &p->colcirc.ptr[i]);

  for(i=0; i<p->colpictxt.no; i++) // _MEM_COL_PICTXT
    make_size_COL_PICTXT(proom, &p->colpictxt.ptr[i]);

  for(i=0; i<p->membutton.no; i++) // _MEM_BUTTON
    make_size_BUTTON(proom, &p->membutton.ptr[i]);
}

void make_size_PICTURE(_PICTURE *p)
{
  p->mask.room.xy1.x =  32000;
  p->mask.room.xy1.y =  32000;
  p->mask.room.xy2.x = -32000;
  p->mask.room.xy2.y = -32000;
  
  make_size_PICTUREMASK(&p->mask.room, &p->mask);
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

class _Flip
{
  private:
    _ROOM Screen;
    _KOORD Yo, Yu, Xx;

    _BOOL FChk_ROOM(_ROOM*);
    _BOOL FChk_DOT(_DOT*);
    void  Flip_DOT(_DOT*);
    void  Flip_ROOM(_ROOM*);
    void  Flip_RECTANGLE(_RECTANGLE*);
    void  Flip_CIRCLE(_CIRCLE*);
    void  Flip_PICTEXT(_PICTEXT*);
    void  Flip_OBJECT(_OBJECT*);
    void  Flip_IO(_IO*);
    void  Flip_INPUT(_INPUT*);
    void  Flip_COL_RECT(_COL_RECT*);
    void  Flip_BITMAP(_BITMAP*);
    void  Flip_MOTION(_MOTION*);
    void  Flip_COL_POLY(_COL_POLY*);
    void  Flip_COL_CIRC(_COL_CIRC*);
    void  Flip_COL_PICTXT(_COL_PICTXT*);
    void  Flip_BUTTON(_BUTTON*);
    void  Flip_POLYGON(_POLYGON*);
    void  Flip_PICTURE(_PICTURE*);
    void  Flip_MY_PICTURE(_MY_PICTURE*);
    void  Flip_PICTUREMASK(_PICTUREMASK*);

  public:
    void  Flip(_MY_PICTURE*, _KOORD, _KOORD, _KOORD);

};


_BOOL _Flip::FChk_ROOM(_ROOM *pr)
{
  if((pr->xy1.y >= Yo)&&(pr->xy1.y <= Yu)&&(pr->xy2.y >= Yo)&&(pr->xy2.y <= Yu))
    return(_TRUE);
  return(_FALSE);
}
_BOOL _Flip::FChk_DOT(_DOT *pd)
{
  if((pd->y >= Yo)&&(pd->y <= Yu))
    return(_TRUE);
  return(_FALSE);
}

void _Flip::Flip_DOT(_DOT *p)
{
  if((p->y >= Yo)&&(p->y <= Yu))
  {
    p->x += Xx;
    if(p->x > Screen.xy2.x)
    {
      p->x -= (Screen.xy2.x - Screen.xy1.x + 1);
    }
    else if(p->x < Screen.xy1.x)
    {
      p->x += (Screen.xy2.x - Screen.xy1.x + 1);
    }
  }
}

void _Flip::Flip_ROOM(_ROOM *pr)
{
  _KOORD dx;

  if((pr->xy1.y >= Yo)&&(pr->xy1.y <= Yu)&&(pr->xy2.y >= Yo)&&(pr->xy2.y <= Yu))
  {
    pr->xy1.x += Xx;
    pr->xy2.x += Xx;
    if((pr->xy1.x > Screen.xy2.x)&&(pr->xy2.x > Screen.xy2.x))
    {
      dx = Screen.xy2.x - Screen.xy1.x + 1;
      pr->xy1.x -= dx;
      pr->xy2.x -= dx;
    }
    else if((pr->xy1.x < Screen.xy1.x)&&(pr->xy2.x < Screen.xy1.x))
    {
      dx = Screen.xy2.x - Screen.xy1.x + 1;
      pr->xy1.x += dx;
      pr->xy2.x += dx;
    }
  }
}
void _Flip::Flip_RECTANGLE(_RECTANGLE *p)
{
  Flip_ROOM(&p->room);
}
void _Flip::Flip_CIRCLE(_CIRCLE *p)
{
  Flip_ROOM(&p->room);
}
void _Flip::Flip_PICTEXT(_PICTEXT *p)
{
  Flip_ROOM(&p->room);
}
void _Flip::Flip_IO(_IO *p)
{
  Flip_ROOM(&p->room);
}
void _Flip::Flip_INPUT(_INPUT *p)
{
  Flip_IO(&p->io);
}
void _Flip::Flip_OBJECT(_OBJECT *p)
{
  _SubPicture *Obj;
  _ROOM       room;
  _DOT        offset;

//Debug(p->xy.x, 1);
//Debug(p->xy.y, 2);

  if(FChk_DOT(&p->xy) == _TRUE)
  {
    Obj = ObjectList.Get(p->no);

    if(Obj != NULL)
    {
      room = Obj->GetPICTURE()->mask.room;

      init_DOT(&offset);
      Zoom_ROOM(&room, &offset, &p->zoom);

      room.xy1.x += p->xy.x;
      room.xy2.x += p->xy.x;
      room.xy1.y += p->xy.y;
      room.xy2.y += p->xy.y;

//Debug(room.xy1.x, 3);
//Debug(room.xy1.y, 4);
//Debug(room.xy1.x, 3);
//Debug(room.xy1.y, 4);

      if(FChk_ROOM(&room) == _TRUE)
      {
        Flip_DOT(&p->xy);
      }
    }
  }
}
void _Flip::Flip_COL_RECT(_COL_RECT *p)
{
  Flip_RECTANGLE(&p->rectangle);
}
void _Flip::Flip_BITMAP(_BITMAP *p)
{
  Flip_ROOM(&p->room);
}
void _Flip::Flip_MOTION(_MOTION *p)
{
  Flip_ROOM(&p->room);
}
void _Flip::Flip_COL_POLY(_COL_POLY *p)
{
  Flip_POLYGON(&p->poly);
}
void _Flip::Flip_COL_CIRC(_COL_CIRC *p)
{
  Flip_CIRCLE(&p->circle);
}
void _Flip::Flip_COL_PICTXT(_COL_PICTXT *p)
{
  Flip_PICTEXT(&p->btx);
}
void _Flip::Flip_BUTTON(_BUTTON *p)
{
  Flip_ROOM(&p->room);
}
void _Flip::Flip_POLYGON(_POLYGON *p)
{
  _UWORD i;
  _DOT  *pd;
  _KOORD x;
  _BOOL flip = _TRUE;

  for(pd=p->ptr, i=0; i<p->no; i++, pd++)
  {
    if(FChk_DOT(pd) == _FALSE)
      return;
    x = pd->x + Xx;
    if((x < Screen.xy2.x)&&(x > Screen.xy1.x))
      flip = _FALSE;
  }

  if(flip == _TRUE)
  {
    for(pd=p->ptr, i=0; i<p->no; i++, pd++)
      Flip_DOT(pd);
  }
  else
  {
    for(pd=p->ptr, i=0; i<p->no; i++, pd++)
      pd->x += Xx;
  }
}

void _Flip::Flip_PICTUREMASK(_PICTUREMASK *p)
{
  _UWORD i;

  for(i=0; i<p->memrectangle.no; i++) // _MEM_RECTANGLE
    Flip_RECTANGLE(&p->memrectangle.ptr[i]);
  for(i=0; i<p->mempolygon.no; i++) // _MEM_POLYGON
    Flip_POLYGON(&p->mempolygon.ptr[i]);
  for(i=0; i<p->memcircle.no; i++) // _MEM_CIRCLE
    Flip_CIRCLE(&p->memcircle.ptr[i]);
  for(i=0; i<p->mempictxt.no; i++) // _MEM_PICTEXT
    Flip_PICTEXT(&p->mempictxt.ptr[i]);
  for(i=0; i<p->memobject.no; i++) // _MEM_OBJECT
    Flip_OBJECT(&p->memobject.ptr[i]);
  for(i=0; i<p->membitmap.no; i++) // _MEM_BITMAP
    Flip_BITMAP(&p->membitmap.ptr[i]);
  for(i=0; i<p->meminput.no; i++) // _MEM_INPUT
    Flip_INPUT(&p->meminput.ptr[i]);
  for(i=0; i<p->memoutput.no; i++) // _MEM_OUTPUT
    Flip_IO(&p->memoutput.ptr[i]);
  for(i=0; i<p->memmotion.no; i++) // _MEM_MOTION
    Flip_MOTION(&p->memmotion.ptr[i]);
  for(i=0; i<p->colrect.no; i++) // _MEM_COL_RECT
    Flip_COL_RECT(&p->colrect.ptr[i]);
  for(i=0; i<p->colpoly.no; i++) // _MEM_COL_POLY
    Flip_COL_POLY(&p->colpoly.ptr[i]);
  for(i=0; i<p->colcirc.no; i++) // _MEM_COL_CIRC
    Flip_COL_CIRC(&p->colcirc.ptr[i]);
  for(i=0; i<p->colpictxt.no; i++) // _MEM_COL_PICTXT
    Flip_COL_PICTXT(&p->colpictxt.ptr[i]);
  for(i=0; i<p->membutton.no; i++) // _MEM_BUTTON
    Flip_BUTTON(&p->membutton.ptr[i]);
}

void _Flip::Flip_PICTURE(_PICTURE *p)
{
  Flip_PICTUREMASK(&p->mask);
}

void _Flip::Flip_MY_PICTURE(_MY_PICTURE *p)
{
  _UWORD i;

  Flip_DOT(&p->FirstInput);
  for(i=0; i<p->No; i++)
    Flip_PICTURE(p->Ptr[i].GetPICTURE());
}

void _Flip::Flip(_MY_PICTURE *p, _KOORD dxx, _KOORD dyo, _KOORD dyu)
{
  Yo     = dyo;
  Yu     = dyu;
  Xx     = dxx;
  Screen = p->Room;

  Flip_MY_PICTURE(p);
}

void flip_MY_PICTURE(_MY_PICTURE *p, _KOORD xx, _KOORD yo, _KOORD yu)
{
  _Flip Flip;

  if((p != NULL)&&(xx != 0)&&(yo < yu))
    Flip.Flip(p, xx, yo, yu);
}


