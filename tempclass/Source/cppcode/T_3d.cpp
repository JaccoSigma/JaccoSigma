#include "T_IPC.H"
#include "T_NUC.H"

#ifdef __TURBOC__
 #include <math.h>
#endif

//**************************************************************************************
//  Ist in ST ausprogrammiert - wegen Koprozessor
#ifdef __TURBOC__
void matrize(_REAL *m, _REAL *px, _REAL *py, _REAL *pz)
{
  _REAL x, y, z;

  x = *px;
  y = *py;
  z = *pz;

  *px = m[0]*x + m[1]*y + m[2]*z;
  *py = m[3]*x + m[4]*y + m[5]*z;
  *pz = m[6]*x + m[7]*y + m[8]*z;
}

//**************************************************************************************
//  Ist in ST ausprogrammiert - wegen Koprozessor
void rotate_DOT3D(_DOT3D *p, _WORD w1, _WORD w2, _WORD w3)
{
  _REAL m[9], x, y, z;
  _REAL a1, a2, a3;

  a1 = w1;
  a2 = w2;
  a3 = w3;

  a1 = (a1 * 3.1416) / 180;
  a2 = (a2 * 3.1416) / 180;
  a3 = (a3 * 3.1416) / 180;

  x = p->x;
  y = p->y;
  z = p->z;

  m[0] = cos(a1);
  m[1] = sin(a1);
  m[2] = 0;
  m[3] = -m[1];
  m[4] = m[0];
  m[5] = 0;
  m[6] = 0;
  m[7] = 0;
  m[8] = 1;
  matrize(m, &x, &y, &z);

  m[0] = 1;
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;
  m[4] = cos(a2);
  m[5] = sin(a2);
  m[6] = 0;
  m[7] = -m[5];
  m[8] = m[4];
  matrize(m, &x, &y, &z);

  m[0] = cos(a3);
  m[1] = 0;
  m[2] = sin(a3);
  m[3] = 0;
  m[4] = 1;
  m[5] = 0;
  m[6] = -m[2];
  m[5] = 0;
  m[8] = m[0];
  matrize(m, &x, &y, &z);

  p->x = FTOL(x);
  p->y = FTOL(y);
  p->z = FTOL(z);
}
#endif //__TURBOC__


#ifdef UC_GRAPHIC_STUDIO

#define PI  3.1415

extern "C" void out_IMAGE(_ROOM *, _UWORD);

void init_CIRCLE2D(_CIRCLE2D *p)
{
  init_CIRCLE(&p->circle);
  init_ROOM_Makro(&p->origin);
}

void free_CIRCLE2D(_CIRCLE2D *p)
{
  free_CIRCLE(&p->circle);
//  free_ROOM(&p->origin);
  init_CIRCLE2D(p);
}

void init_POLYGON2D(_POLYGON2D *p)
{
  init_POLYGON(&p->polygon);
  p->origin = NULL;
}

void free_POLYGON2D(_POLYGON2D *p)
{
//  _DOT  *ptr;
//  _UWORD i;
//  for(i=0, ptr=p->origin; i<p->polygon.no; i++, ptr++)
//    free_DOT(ptr);
  Memory2((void**)&p->origin, 0);
  free_POLYGON(&p->polygon);
}

void init_MEM_CIRCLE2D(_MEM_CIRCLE2D *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_CIRCLE2D(_MEM_CIRCLE2D *p)
{
  _CIRCLE2D *ptr;
  _UWORD    i;

  for(i=0, ptr=p->ptr; i<p->no; i++, ptr++)
    free_CIRCLE2D(ptr);
  Memory2((void**)&p->ptr, 0);
  init_MEM_CIRCLE2D(p);
}

void init_MEM_POLYGON2D(_MEM_POLYGON2D *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_POLYGON2D(_MEM_POLYGON2D *p)
{
  _POLYGON2D *ptr;
  _UWORD     i;

  for(i=0, ptr=p->ptr; i<p->no; i++, ptr++)
    free_POLYGON2D(ptr);
  Memory2((void**)&p->ptr, 0);
  init_MEM_POLYGON2D(p);
}

void init_PICTUREMASK2D(_PICTUREMASK2D *p)
{
  init_MEM_CIRCLE2D(&p->mem_circle2d);
  init_MEM_POLYGON2D(&p->mem_polygon2d);
  init_MEM_ZORDER(&p->mem_zorder);
}

void free_PICTUREMASK2D(_PICTUREMASK2D *p)
{
  free_MEM_CIRCLE2D(&p->mem_circle2d);
  free_MEM_POLYGON2D(&p->mem_polygon2d);
  free_MEM_ZORDER(&p->mem_zorder);

  init_PICTUREMASK2D(p);
}

void init_DOT3D(_DOT3D *p)
{
  p->x = DEFKOORD;
  p->y = DEFKOORD;
  p->z = DEFKOORD;
}

void free_DOT3D(_DOT3D *p)
{
  init_DOT3D(p);
}

void add_3DSHAPE(_3DSHAPE *p, _KOORD x, _KOORD y, _KOORD z)
{
  _UWORD i, j;
  _SHAPE *ps;
  _DOT3D *pd;

  ps = p->ptr;
  for(i=0; i<p->no; i++)
  {
    pd = ps->ptr;
    for(j=0; j<ps->no; j++)
    {
      pd->x += x;
      pd->y += y;
      pd->z += z;
      pd += 1;
    }

    ps += 1;
  }
}

_BOOL make_MEM_RECTANGLE2D(_MEM_POLYGON2D *dst, _MEM_RECTANGLE *src)
{
  _BOOL        retcode = _FALSE;
  _UWORD       i;
  _RECTANGLE   *ps;
  _POLYGON2D   *pd;
  _UDWORD      size;
  _ROOM        room;

  if(Memory2((void**)&dst->ptr, (dst->no+src->no) * sizeof(_POLYGON2D)) != 0)
  {
    retcode  = _TRUE;
    pd       = &dst->ptr[dst->no];
    dst->no += src->no;

    for(i=0,ps=src->ptr; i<src->no; i++, pd++, ps++)
    {
      if(retcode == _TRUE)
      {
        retcode = _FALSE;
        init_POLYGON2D(pd);
        size = 5 * sizeof(_DOT);
        if(Memory2((void**)&pd->polygon.ptr, size) != 0)
        {
          if(Memory2((void**)&pd->origin, size) != 0)
          {
            retcode = _TRUE;
            pd->polygon.no = 5;
            room = ps->room;
            linear_coordinates(&room.xy1.x, &room.xy1.y, &room.xy2.x, &room.xy2.y);
            pd->polygon.ptr[0]   =
            pd->polygon.ptr[4]   = room.xy1;
            pd->polygon.ptr[1].x = room.xy2.x;
            pd->polygon.ptr[1].y = room.xy1.y;
            pd->polygon.ptr[2]   = room.xy2;
            pd->polygon.ptr[3].x = room.xy1.x;
            pd->polygon.ptr[3].y = room.xy2.y;

            MemCpy(pd->origin, pd->polygon.ptr, size);

            copy_CHKBIT(&retcode, &pd->polygon.chkbit, &ps->chkbit);
            pd->polygon.attrib     = ps->attrib;
            pd->polygon.color      = ps->color;
            pd->polygon.colorframe = ps->colorframe;
          }
        }
      }
    }
  }

  return(retcode);
}

_BOOL make_MEM_CIRCLE2D(_MEM_CIRCLE2D *dst, _MEM_CIRCLE *src)
{
  _BOOL     retcode = _FALSE;
  _UWORD    i;
  _CIRCLE   *ps;
  _CIRCLE2D *pd;

  if(Memory2((void**)&dst->ptr, src->no * sizeof(_CIRCLE2D)) != 0)
  {
    dst->no = src->no;
    retcode = _TRUE;
    for(i=0, pd=dst->ptr, ps=src->ptr; i<src->no; i++, pd++, ps++)
    {
      init_CIRCLE2D(pd);
      copy_CIRCLE(&retcode, &pd->circle, ps);
      pd->origin = ps->room;
    }
  }

  return(retcode);
}

_BOOL make_MEM_POLYGON2D(_MEM_POLYGON2D *dst, _MEM_POLYGON *src)
{
  _BOOL      retcode = _FALSE;
  _UWORD     i;
  _POLYGON   *ps;
  _POLYGON2D *pd;
  _UDWORD    size;

  if(Memory2((void**)&dst->ptr, src->no * sizeof(_POLYGON2D)) != 0)
  {
    dst->no = src->no;
    retcode = _TRUE;
    for(i=0, pd=dst->ptr, ps=src->ptr; i<src->no; i++, pd++, ps++)
    {
      init_POLYGON2D(pd);
      copy_POLYGON(&retcode, &pd->polygon, ps);
      if(retcode == _TRUE)
      {
        size = pd->polygon.no * sizeof(_DOT);
        if(Memory2((void**)&pd->origin, size) != 0)
          MemCpy(pd->origin, pd->polygon.ptr, size);
        else
          retcode = _FALSE;
      }
    }
  }

  return(retcode);
}

void make_zorder(_ZORDER **pp, _UWORD *panz, _UBYTE ftype, _UWORD no)
{
  _ZORDER *p = *pp;
  _UWORD anz = *panz;

  if(anz > 0)
  {
    p->ftype = ftype;
    p->no    = no;
    *panz -= 1;
    *pp   += 1;
  }
 #ifdef PGTEST
  else
  {
    Annahme("T_3D.CPP", __LINE__);
  }
 #endif
}

_BOOL make_PICTUREMASK2D_sub(_PICTUREMASK2D *dst, _PICTUREMASK *src)
{
  _BOOL   retcode = _FALSE;
  _UWORD  i, anz;
  _ZORDER *ps, *pd;

  if(src != NULL)
  {
    retcode = _TRUE;

    if(make_MEM_POLYGON2D(&dst->mem_polygon2d, &src->mempolygon) == _FALSE)
      retcode = _FALSE;

    if(make_MEM_RECTANGLE2D(&dst->mem_polygon2d, &src->memrectangle) == _FALSE)
      retcode = _FALSE;

    if(make_MEM_CIRCLE2D(&dst->mem_circle2d, &src->memcircle) == _FALSE)
      retcode = _FALSE;

    if(retcode == _TRUE)
    {
      retcode = _FALSE;
      anz     = dst->mem_circle2d.no + dst->mem_polygon2d.no;
      if(Memory2((void**)&dst->mem_zorder.ptr, anz * sizeof(_ZORDER)) != 0)
      {
	    retcode = _TRUE;
        dst->mem_zorder.no = anz;
        ps = src->memzorder.ptr;
        pd = dst->mem_zorder.ptr;
        for(i=0; i<src->memzorder.no; i++, ps++)
        {
          switch(ps->ftype)
          {
            case ZO_CIRCLE    : make_zorder(&pd, &anz, ZO_CIRCLE, ps->no); break;
            case ZO_RECTANGLE : make_zorder(&pd, &anz, ZO_POLYGON, ps->no + src->mempolygon.no); break;
            case ZO_POLYGON   : make_zorder(&pd, &anz, ZO_POLYGON, ps->no); break;
          }
        }
      }
    }
  }

  if(retcode == _FALSE)
    free_PICTUREMASK2D(dst);

  return(retcode);
}

void out_PICTUREMASK2D(_PICTUREMASK2D *p, _DOT *offset, _ZOOM *zoom)
{
  _UWORD  i;
  _ZORDER *pzo;

  for(i=0, pzo=p->mem_zorder.ptr; i<p->mem_zorder.no; i++, pzo++)
  {
    switch(pzo->ftype)
    {
      case ZO_CIRCLE    : if(pzo->no < p->mem_circle2d.no)
                            out_CIRCLE(&p->mem_circle2d.ptr[pzo->no].circle, offset, zoom);
                          break;
      case ZO_POLYGON   : if(pzo->no < p->mem_polygon2d.no)
                            out_POLYGON(&p->mem_polygon2d.ptr[pzo->no].polygon, offset, zoom);
                          break;
    }
  }
}

void rotate_PICTUREMASK2D(_PICTUREMASK2D *p, _WORD angle)
{
  _UWORD     i, j;
  _POLYGON2D *pp;
  _DOT       *pd, *ps;
  _DOT3D     d3d;
  _ROOM      *pr, room;

  for(i=0, pp=p->mem_polygon2d.ptr; i<p->mem_polygon2d.no; i++, pp++)
  {
    for(j=0, ps=pp->origin, pd=pp->polygon.ptr; j<pp->polygon.no; j++, ps++, pd++)
    {
      d3d.x = ps->x;
      d3d.y = ps->y;
      d3d.z = 0;
      rotate_DOT3D(&d3d, angle, 0, 0);
      pd->x = d3d.x;
      pd->y = d3d.y;
    }
  }

  for(i=0; i<p->mem_circle2d.no; i++)
  {
    room  = p->mem_circle2d.ptr[i].origin;
    linear_coordinates(&room.xy1.x, &room.xy1.y, &room.xy2.x, &room.xy2.y);
    d3d.x = room.xy1.x;
    d3d.y = room.xy1.y;
    d3d.z = 0;
    rotate_DOT3D(&d3d, angle, 0, 0);
    pr    = &p->mem_circle2d.ptr[i].circle.room;
    pr->xy1.x  = d3d.x;
    pr->xy1.y  = d3d.y;
    pr->xy2.x  = room.xy2.x + d3d.x - room.xy1.x;
    pr->xy2.y  = room.xy2.y + d3d.y - room.xy1.y;
  }

}

void add_PICTUREMASK2D(_PICTUREMASK2D *p, _KOORD x, _KOORD y)
{
  _UWORD       i, j;
  _CIRCLE2D    *pc;
  _POLYGON2D   *pp;
  _DOT         *pd;

  for(i=0, pc=p->mem_circle2d.ptr; i<p->mem_circle2d.no; i++, pc++)
  {
    pc->origin.xy1.x += x;
    pc->origin.xy1.y += y;
    pc->origin.xy2.x += x;
    pc->origin.xy2.y += y;
  }
  for(i=0, pp=p->mem_polygon2d.ptr; i<p->mem_polygon2d.no; i++, pp++)
  {
    for(j=0, pd=pp->origin; j<pp->polygon.no; j++, pd++)
    {
      pd->x += x;
      pd->y += y;
    }
  }

  rotate_PICTUREMASK2D(p, 0);
}

_BOOL make_PICTUREMASK2D(_PICTUREMASK2D *p, _UWORD no)
{
  _BOOL       retcode = _FALSE;
  _SubPicture *Sp;

  Sp = ObjectList.Get(no);
  if(Sp != NULL)
    retcode = make_PICTUREMASK2D_sub(p, &Sp->GetPICTURE()->mask);

  return(retcode);
}

void out_IMAGE(_ROOM *pr, _UWORD no)
{
  _BMP *bmp;
  bmp = ImageList.Get(no);
  if(bmp != NULL)
    out_zoom_BMP(pr, bmp, T_COPY|T_SOLID);
}

_BOOL SHAPE_convex(_SHAPE *p)
{
   _UWORD   i;
   _KOORD   z;
   _BOOL    retcode;
   _DOT3D   *ptr0;
   _DOT3D   *ptr1;
   _DOT3D   *ptr2;

   
   retcode = _TRUE;

   z = (p->ptr[1].x-p->ptr[0].x)*(p->ptr[0].y-p->ptr[p->no-2].y) - (p->ptr[1].y-p->ptr[0].y)*(p->ptr[0].x-p->ptr[p->no-2].x);
   
   if(z<0)
   {
      for(i=1, ptr0=&p->ptr[0], ptr1=&p->ptr[1], ptr2=&p->ptr[2]; i<p->no-1; i++, ptr0++, ptr1++, ptr2++)
	  {
	     z = (ptr2->x-ptr1->x)*(ptr1->y-ptr0->y) - (ptr2->y-ptr1->y)*(ptr1->x-ptr0->x);
		 if(z>0)
		    {retcode = _FALSE; break;}
	  }
   }
   if(z>0)
   {  
      for(i=1, ptr0=&p->ptr[0], ptr1=&p->ptr[1], ptr2=&p->ptr[2]; i<p->no-1; i++, ptr0++, ptr1++, ptr2++)
      {
         z = (ptr2->x-ptr1->x)*(ptr1->y-ptr0->y) - (ptr2->y-ptr1->y)*(ptr1->x-ptr0->x);
		 if(z<0)
		    {retcode = _FALSE; break;}
	  }
   }
   return(retcode);
}

void start_SHAPEHEADER(_SHAPE *p)
{
   _UWORD   i;
   _UWORD   ct_1, ct_2;
   _DOT3D   *ptr;

   p->header.convex = SHAPE_convex(p);

   for(i=0; i<p->no-1; i++)
   {
      p->header.ptr2d[i].x = p->ptr[i].x;
	  p->header.ptr2d[i].y = p->ptr[i].y;
   }

   p->header.xmin = p->ptr[0].x;       
   p->header.xmax = p->ptr[0].x;
   p->header.ymin = p->ptr[0].y;       
   p->header.ymax = p->ptr[0].y;
   p->header.zmin = p->ptr[0].z;
   p->header.zmax = p->ptr[0].z;

   for(i=1;i<p->no-1; i++)
   {
      if(p->ptr[i].x < p->header.xmin)
	     p->header.xmin = p->ptr[i].x;
      if(p->ptr[i].x > p->header.xmax)
	     p->header.xmax = p->ptr[i].x;
	  if(p->ptr[i].y < p->header.ymin)
	     p->header.ymin = p->ptr[i].y;
      if(p->ptr[i].y > p->header.ymax)
	     p->header.ymax = p->ptr[i].y;
	  if(p->ptr[i].z < p->header.zmin)
	     p->header.zmin = p->ptr[i].z;
      if(p->ptr[i].z > p->header.zmax)
	     p->header.zmax = p->ptr[i].z;
   }

   
   if(p->no>6)
   {
      ct_1 = p->no/3;
      ct_2 = (2*p->no)/3+2;
   }
   else
   {
      ct_1 = 1;
	  ct_2 = 2;
   }
   
	p->header.pad =  ((p->ptr[ct_1].y-p->ptr[0].y)*(p->ptr[ct_2].z-p->ptr[ct_1].z) - (p->ptr[ct_1].z-p->ptr[0].z)*(p->ptr[ct_2].y-p->ptr[ct_1].y));
	p->header.pbd = -((p->ptr[ct_1].x-p->ptr[0].x)*(p->ptr[ct_2].z-p->ptr[ct_1].z) - (p->ptr[ct_1].z-p->ptr[0].z)*(p->ptr[ct_2].x-p->ptr[ct_1].x));
	p->header.pcd =  ((p->ptr[ct_1].x-p->ptr[0].x)*(p->ptr[ct_2].y-p->ptr[ct_1].y) - (p->ptr[ct_1].y-p->ptr[0].y)*(p->ptr[ct_2].x-p->ptr[ct_1].x));
	p->header.pdd = 0;

   for(i=0, ptr=&p->ptr[0]; i<p->no-1; i++, ptr++)
		p->header.pdd = p->header.pdd + p->header.pad*ptr->x + p->header.pbd*ptr->y + p->header.pcd*ptr->z;

	p->header.pdd = p->header.pdd/(p->no-1);

}

void start_3DSHAPEHEADER(_3DSHAPE *p)
{
   _UWORD   j;

   p->header.xmax = p->ptr[0].header.xmax;      
   p->header.xmin = p->ptr[0].header.xmin;
   p->header.ymax = p->ptr[0].header.ymax;    
   p->header.ymin = p->ptr[0].header.ymin;
   p->header.zmax = p->ptr[0].header.zmax;    
   p->header.zmin = p->ptr[0].header.zmin;
   for(j=1;j<p->no; j++)      
   {
      if(p->ptr[j].header.xmax>p->header.xmax)
	     p->header.xmax = p->ptr[j].header.xmax;
      if(p->ptr[j].header.xmin<p->header.xmin)
	     p->header.xmin = p->ptr[j].header.xmin;
	  if(p->ptr[j].header.ymax>p->header.ymax)
	     p->header.ymax = p->ptr[j].header.ymax;
      if(p->ptr[j].header.ymin<p->header.ymin)
	     p->header.ymin = p->ptr[j].header.ymin;
	  if(p->ptr[j].header.zmax>p->header.zmax)
	     p->header.zmax = p->ptr[j].header.zmax;
      if(p->ptr[j].header.zmin<p->header.zmin)
	     p->header.zmin = p->ptr[j].header.zmin;
   }


}
/*
void copy_SHAPE(_SHAPE *d, _SHAPE *s)
void copy_3DSHAPE(_3DSHAPE *d, _3DSHAPE *s)
*/
_BOOL polygon_to_SHAPE(_SHAPE *pd, _DOT *ps, _UWORD no, _KOORD z, _COLOR col, _COLOR framecol)
{
  _DOT3D *p;
  _UWORD i;
  _BOOL  retcode = _TRUE;

  if(no != pd->no)
  {
    for(i=0, p=pd->ptr; i<pd->no; i++, p+=1)
      free_DOT3D(p);
    retcode = _FALSE;
    if(Memory2((void**)&pd->ptr, no * sizeof(_DOT3D)) != 0)
    {
      retcode = _TRUE;
      pd->no  = no;
      for(i=0, p=pd->ptr; i<no; i++, p+=1)
        init_DOT3D(p);
    }
  }

  if(retcode == _TRUE)
  {
    pd->color     = MakeColor_Makro(framecol, col);
    pd->backcolor = MakeColor_Makro(framecol, col);

    p = pd->ptr;
    for(i=0; i<no; i++)
    {
      p->x = ps->x;
      p->y = ps->y;
      p->z = z;

      ps += 1;
      p  += 1;
    }
    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL line_to_SHAPE(_SHAPE *pd, _DOT *pd1, _DOT *pd2, _KOORD z1, _KOORD z2, _COLOR col, _COLOR framecol)
{
  _BOOL  retcode = _TRUE;
  _DOT3D *p;
  _UWORD i;

  if(pd->no != 5)
  {
    for(i=0, p=pd->ptr; i<pd->no; i++, p++)
      free_DOT3D(p);
    retcode = _FALSE;
    if(Memory2((void**)&pd->ptr, 5 * sizeof(_DOT3D)) != 0)
    {
      retcode = _TRUE;
      pd->no  = 5;
      for(i=0, p=pd->ptr; i<5; i++, p++)
        init_DOT3D(p);
    }
  }

  if(retcode == _TRUE)
  {
    pd->color     = MakeColor_Makro(framecol, col);
    pd->backcolor = MakeColor_Makro(framecol, col);
    pd->no        = 5;

    p = pd->ptr;

    init_DOT3D(&p[0]);
    init_DOT3D(&p[1]);
    init_DOT3D(&p[2]);
    init_DOT3D(&p[3]);
    init_DOT3D(&p[4]);

    p[0].x =
    p[1].x =
    p[4].x = pd1->x;
    p[0].y =
    p[1].y =
    p[4].y = pd1->y;

    p[2].x =
    p[3].x = pd2->x;
    p[2].y =
    p[3].y = pd2->y;

    p[0].z =
    p[3].z =
    p[4].z = z1;

    p[1].z =
    p[2].z = z2;

    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL polygon_to_3DSHAPE(_3DSHAPE *pd, _DOT *pdot, _UWORD no, _UWORD depth, _COLOR col, _COLOR framecol, _BOOL shading, _BOOL frame)
{
  _SHAPE  *ptr;
  _UWORD  i, anz;
  _KOORD  raus, rein;
  _BOOL   retcode = _TRUE;

  anz = no - 1;

  if(pd->no != anz)
  {
    for(i=0, ptr=pd->ptr; i<pd->no; i++, ptr++)
      free_SHAPE(ptr);

    retcode = _FALSE;
    if(Memory2((void**)&pd->ptr, anz * sizeof(_SHAPE)) != 0)
    {
      retcode = _TRUE;
      pd->no  = anz;
      for(i=0, ptr=pd->ptr; i<anz; i++, ptr++)
        init_SHAPE(ptr);
    }
  }

  if(retcode == _TRUE)
  {
    pd->frame = frame;
    pd->shading = shading;
    ptr  = pd->ptr;
    raus = -(depth/2);
    rein = depth - (depth/2);
    for(i=0; i<no-1; i++)
    {
      if(line_to_SHAPE(ptr, pdot, pdot+1, raus, rein, col, framecol) == _FALSE)
      {
        retcode = _FALSE;
        break;
      }
      ptr  += 1;
      pdot += 1;
    }
  }
  return(retcode);
}

_BOOL polygon_to_SOLID3DSHAPE(_3DSHAPE *pd, _DOT *pdot, _UWORD no, _UWORD depth, _COLOR col, _COLOR framecol, _BOOL shading, _BOOL frame)
{
  _SHAPE  *ptr;
  _UWORD  i, anz;
  _KOORD  raus, rein;
  _BOOL   retcode = _TRUE;

  anz = no + 1;

  if(pd->no != anz)
  {
    for(i=0, ptr=pd->ptr; i<pd->no; i++, ptr++)
      free_SHAPE(ptr);

    retcode = _FALSE;
    if(Memory2((void**)&pd->ptr, anz * sizeof(_SHAPE)) != 0)
    {
      retcode = _TRUE;
      pd->no  = anz;
      for(i=0, ptr=pd->ptr; i<anz; i++, ptr++)
        init_SHAPE(ptr);
    }
  }

  if(retcode == _TRUE)
  {
    pd->frame = frame;
	pd->shading = shading;
    ptr  = pd->ptr;
    raus = -(depth/2);
    rein = depth - (depth/2);
    if(polygon_to_SHAPE(ptr, pdot, no, raus, col, framecol) == _TRUE)
    {
      ptr += 1;
      if(polygon_to_SHAPE(ptr, pdot, no, rein, col, framecol) == _TRUE)
      {
        retcode = _TRUE;
        ptr    += 1;
        no     -= 1;
        for(i=0; i<no; i++)
        {
          if(line_to_SHAPE(ptr, pdot, pdot+1, raus, rein, col, framecol) == _FALSE)
          {
            retcode = _FALSE;
            break;
          }
          ptr  += 1;
          pdot += 1;
        }
      }
    }
  }

  return(retcode);
}

_BOOL TWO_to_ONE3DSHAPE(_3DSHAPE *pd, _3DSHAPE *sh1, _3DSHAPE *sh2)
{
  _UWORD   anz;
  _UWORD   i,j;
  _BOOL    retcode = _TRUE;
  _SHAPE   *ptr;

  anz = sh1->no + sh2->no;
  if(pd->no != anz)
  {
    for(i=0,ptr=pd->ptr; i<pd->no; i++,ptr++)
      free_SHAPE(ptr);

    retcode = _FALSE;

    if(Memory2((void**)&pd->ptr, anz*sizeof(_SHAPE)) != 0)
    {
      retcode     = _TRUE;
      pd->no      = anz;
      pd->frame   = sh1->frame||sh2->frame;
      pd->shading = sh1->shading&&sh2->shading;
      for(i=0,ptr = pd->ptr; i<anz; i++,ptr++)
        init_SHAPE(ptr);
    }
  }

  if(retcode == _TRUE)
  {
    for(i=0; i<sh1->no; i++)
      if(copy_SHAPE(&pd->ptr[i], &sh1->ptr[i]) == _FALSE)
        retcode = _FALSE;

    for(j=0; j<sh2->no; j++)
      if(copy_SHAPE(&pd->ptr[sh1->no+j], &sh2->ptr[j]) == _FALSE)
        retcode = _FALSE;
  }

  return(retcode);
}

_WORD sort_SHAPE(_SHAPE *a, _SHAPE *b)         // Funktion, die zwei SHAPEs nach mittlerer z-Koordinate der
{                                              // Eckpunkte sortiert
   _WORD   retcode = 0;
   _KOORD  za,zb;
   _UWORD  i;

   za = 0;
   zb = 0;
   for(i=0;i<a->no-1;i++)
   {
	  za = za + a->ptr[i].z;
   }
   for(i=0; i<b->no-1; i++)
   {
      zb = zb + b->ptr[i].z;
   }
   
   za = za/a->no;
   zb = zb/b->no;   

   if(za < zb)
      retcode = 1;
   else if(za > zb)
      retcode = -1;

   return(retcode);

}

_KOORD absval(_KOORD x)            // Betragsfunktion für Koordinaten
{
   _KOORD   retcode;

   if(x>=0)
      retcode = x;
   else
      retcode = -x;

   return(retcode);
}

_DWORD absval_dword(_DWORD x)      // Betragsfunktion für Koordinaten
{
   _DWORD   retcode;

   if(x>=0)
      retcode = x;
   else
      retcode = -x;

   return(retcode);
}

_DWORD dword_sqrt(_DWORD x)
{ 
   _DWORD retcode;
   _DWORD i;
   
   if(x<0)
      retcode = 0;
   if(x==0)
      retcode = 0;
   if(x==1)
      retcode = 1;
   if(x==2)
      retcode = 1;
   if(x==3)
      retcode = 2;
   if(x==4)
      retcode = 2;
   if(x==5)
      retcode = 2;
   if(x==6)
      retcode = 2;
   if(x==7)
      retcode = 3;
   if(x==8)
      retcode = 3;
   if(x==9)
      retcode = 3; 
   if(x==10)
      retcode = 3;
   if(x>10)
   {
      if((x % 2)==0)
      {   
	     i = 2;
         while((i+2)*(i+2)<=x)
		 {
            i+=2;
		 }
		 retcode = i;
		 if(absval_dword(x-i*i)>absval_dword(x-(i+1)*(i+1)))
		 {   
			retcode = i+1;
		    if(absval_dword(x-(i+1)*(i+1))>absval_dword(x-(i+2)*(i+2)))
		       retcode = i+2; 
		 }
	  }
	  else
	  {
         i = 3;
         while((i+2)*(i+2)<=x)
		 {
            i+=2;
		 }
		 retcode = i;
		 if(absval_dword(x-i*i)>absval_dword(x-(i+1)*(i+1)))
		 {
			retcode = i+1;
		    if(absval_dword(x-(i+1)*(i+1))>absval_dword(x-(i+2)*(i+2)))
		       retcode = i+2;
		 }
	  }
   }

   return(retcode);
}

void rotate_DOT(_DOT *dp, _KOORD xrot, _KOORD yrot, _WORD b)
{
  _KOORD x, xnew, y, ynew;
  _REAL  tmpsin = SIN((b*PI)/180);
  _REAL  tmpcos = COS((b*PI)/180);

  x = dp->x - xrot;
  y = dp->y - yrot;

  xnew = FTOL(x*tmpcos + y*tmpsin);
  ynew = FTOL(-x*tmpsin + y*tmpcos);

  dp->x = xrot + xnew;
  dp->y = yrot + ynew;
}

void rotate_SHAPE(_SHAPE *p, _WORD a1, _WORD a2, _WORD a3, _BOOL shading)
{
   _DOT3D  *ptr;
   _UWORD  i;
   _UWORD  ct_1,ct_2;
   _COLOR  bluefilter = 31;
   _COLOR  greenfilter = 2016;
   _COLOR  redfilter = 63488;
   _COLOR  ffpencol, ffbackcol, newffpencol;
   _COLOR  bfpencol, bfbackcol, newbfpencol;
   _COLOR  ffbluepart, ffgreenpart, ffredpart;
   _COLOR  bfbluepart, bfgreenpart, bfredpart;
   _DWORD  norm;
   _KOORD  normkoord;
   _KOORD  Qnum, Qdenom, Hnum, Hdenom;

   Qnum = 8;
   Qdenom = 11;
   Hnum = 3;
   Hdenom = 11;

   p->header.convex = SHAPE_convex(p);
   ptr = p->ptr;
   for(i=0; i<p->no; i++)
   {
      rotate_DOT3D(ptr, a1, a2, a3);

      ptr ++;
   }
   for(i=0; i<p->no-1; i++)
   {
      p->header.ptr2d[i].x = p->ptr[i].x;
	  p->header.ptr2d[i].y = p->ptr[i].y;
   }

   p->header.xmin = p->ptr[0].x;
   p->header.xmax = p->ptr[0].x;
   p->header.ymin = p->ptr[0].y;
   p->header.ymax = p->ptr[0].y;
   p->header.zmin = p->ptr[0].z;
   p->header.zmax = p->ptr[0].z;

   for(i=1,ptr=&p->ptr[1];i<p->no-1; i++,ptr++)
   {
      if(ptr->x < p->header.xmin)
	     p->header.xmin = ptr->x;
      if(ptr->x > p->header.xmax)
	     p->header.xmax = ptr->x;
	  if(ptr->y < p->header.ymin)
	     p->header.ymin = ptr->y;
      if(ptr->y > p->header.ymax)
	     p->header.ymax = ptr->y;
	  if(ptr->z < p->header.zmin)
	     p->header.zmin = ptr->z;
      if(ptr->z > p->header.zmax)
	     p->header.zmax = ptr->z;
   }


   if(p->no>6)
   {
      ct_1 = p->no/3;
      ct_2 = (2*p->no)/3+2;
   }
   else
   {
      ct_1 = 1;
	  ct_2 = 2;
   }
   
	p->header.pad =  ((p->ptr[ct_1].y-p->ptr[0].y)*(p->ptr[ct_2].z-p->ptr[ct_1].z) - (p->ptr[ct_1].z-p->ptr[0].z)*(p->ptr[ct_2].y-p->ptr[ct_1].y));
	p->header.pbd = -((p->ptr[ct_1].x-p->ptr[0].x)*(p->ptr[ct_2].z-p->ptr[ct_1].z) - (p->ptr[ct_1].z-p->ptr[0].z)*(p->ptr[ct_2].x-p->ptr[ct_1].x));
	p->header.pcd =  ((p->ptr[ct_1].x-p->ptr[0].x)*(p->ptr[ct_2].y-p->ptr[ct_1].y) - (p->ptr[ct_1].y-p->ptr[0].y)*(p->ptr[ct_2].x-p->ptr[ct_1].x));
   
	norm = p->header.pad*p->header.pad + p->header.pbd*p->header.pbd + p->header.pcd*p->header.pcd;
	norm = dword_sqrt(norm);
//   normkoord = norm;

   if(norm<100)
   {
	 p->header.pad = 2*p->header.pad;
	  p->header.pbd = 2*p->header.pbd;
	  p->header.pcd = 2*p->header.pcd;
   }

   if(norm>1000)
   {
      if((norm<10000)&&(norm>1500))
	  {
			p->header.pad = p->header.pad/3;
		 p->header.pbd = p->header.pbd/3;
		 p->header.pcd = p->header.pcd/3;
	  }
	  else if((norm<50000)&&(norm>=10000))
	  {
			p->header.pad = p->header.pad/10;
		 p->header.pbd = p->header.pbd/10;
		 p->header.pcd = p->header.pcd/10;
	  }
	  else if(norm>50000)
	  {
			p->header.pad = p->header.pad/50;
		 p->header.pbd = p->header.pbd/50;
		 p->header.pcd = p->header.pcd/50;
	  }
	  else
	  {
			p->header.pad = p->header.pad/2;
		  p->header.pbd = p->header.pbd/2;
		  p->header.pcd = p->header.pcd/2;
		}
	}

	if(shading==_TRUE)
	{
		ffpencol  = GetPenColor_Makro(p->color);
		ffbackcol = GetBackColor_Makro(p->color);
      bfpencol  = GetPenColor_Makro(p->backcolor);
      bfbackcol = GetBackColor_Makro(p->backcolor);

		norm = p->header.pad*p->header.pad + p->header.pbd*p->header.pbd + p->header.pcd*p->header.pcd;
		norm = dword_sqrt(norm);
		normkoord = norm;

      ffbluepart  = ffpencol&bluefilter;
      ffgreenpart = ffpencol&greenfilter;
      ffredpart   = ffpencol&redfilter;
      bfbluepart  = bfpencol&bluefilter;
      bfgreenpart = bfpencol&greenfilter;
      bfredpart   = bfpencol&redfilter;

      if(normkoord!=0)
      {
			ffbluepart  = (ffbluepart*Hnum)/Hdenom  + ((ffbluepart*absval(p->header.pcd)*Qnum)/normkoord)/Qdenom;
			ffgreenpart = (ffgreenpart*Hnum)/Hdenom + ((ffgreenpart*absval(p->header.pcd)*Qnum)/normkoord)/Qdenom;
			ffredpart   = (ffredpart*Hnum)/Hdenom   + ((ffredpart*absval(p->header.pcd)*Qnum)/normkoord)/Qdenom;
			bfbluepart  = (bfbluepart*Hnum)/Hdenom  + ((bfbluepart*absval(p->header.pcd)*Qnum)/normkoord)/Qdenom;
			bfgreenpart = (bfgreenpart*Hnum)/Hdenom + ((bfgreenpart*absval(p->header.pcd)*Qnum)/normkoord)/Qdenom;
			bfredpart   = (bfredpart*Hnum)/Hdenom   + ((bfredpart*absval(p->header.pcd)*Qnum)/normkoord)/Qdenom;
      }

      ffbluepart  = ffbluepart&bluefilter;
      ffgreenpart = ffgreenpart&greenfilter;
      ffredpart   = ffredpart&redfilter;
      bfbluepart  = bfbluepart&bluefilter;
      bfgreenpart = bfgreenpart&greenfilter;
      bfredpart   = bfredpart&redfilter;

      newffpencol  = ffredpart|ffgreenpart;
      newffpencol  = newffpencol|ffbluepart;
      newbfpencol  = bfredpart|bfgreenpart;
      newbfpencol  = newbfpencol|bfbluepart;

      p->color = MakeColor_Makro(ffbackcol, newffpencol);
      p->backcolor = MakeColor_Makro(bfbackcol, newbfpencol);
   }

	p->header.pdd = 0;

	for(i=0, ptr=&p->ptr[0]; i<p->no-1; i++, ptr++)
		p->header.pdd = p->header.pdd + p->header.pad*ptr->x + p->header.pbd*ptr->y + p->header.pcd*ptr->z;

	p->header.pdd = p->header.pdd/(p->no-1);

}

void rotate_3DSHAPE(_3DSHAPE *p, _WORD a1, _WORD a2, _WORD a3)
{
  _UWORD   i;
  _SHAPE   *ptr;


  ptr = p->ptr;
  for(i=0; i<p->no; i++)
  {
    rotate_SHAPE(ptr, a1, a2, a3, p->shading);
    ptr += 1;
  }
                         
  qsort(p->ptr, p->no, sizeof(_SHAPE), (_QSORTFUNCT)sort_SHAPE);

  p->header.xmax = p->ptr[0].header.xmax;      
  p->header.xmin = p->ptr[0].header.xmin;
  p->header.ymax = p->ptr[0].header.ymax;    
  p->header.ymin = p->ptr[0].header.ymin;
  p->header.zmax = p->ptr[0].header.zmax;    
  p->header.zmin = p->ptr[0].header.zmin;
  for(i=1,ptr=&p->ptr[1];i<p->no; i++, ptr++)      
  {
      if(ptr->header.xmax>p->header.xmax)
	     p->header.xmax = ptr->header.xmax;
      if(ptr->header.xmin<p->header.xmin)
	     p->header.xmin = ptr->header.xmin;
	  if(ptr->header.ymax>p->header.ymax)
	     p->header.ymax = ptr->header.ymax;
      if(ptr->header.ymin<p->header.ymin)
	     p->header.ymin = ptr->header.ymin;
	  if(ptr->header.zmax>p->header.zmax)
	     p->header.zmax = ptr->header.zmax;
      if(ptr->header.zmin<p->header.zmin)
	     p->header.zmin = ptr->header.zmin;
  }

}

_UWORD DotOnLine2D(_KOORD xpt, _KOORD ypt, _DOT *a, _DOT *b)
{
   _UWORD   retcode;
   _KOORD   dx, dy;                      
   _KOORD   xline, yline;                
   
   retcode = 0;

   if(((xpt<=a->x)&&(xpt>=b->x))||((xpt>=a->x)&&(xpt<=b->x)))        
   {                                                                 
      if(((ypt<=a->y)&&(ypt>=b->y))||((ypt>=a->y)&&(ypt<=b->y)))
	  {
	     if((xpt==a->x)&&(xpt==b->x))                                 
         {
            if((ypt>=a->y)&&(ypt<=b->y))                              
	           retcode = 1;
            if((ypt<=a->y)&&(ypt>=b->y))
	           retcode = 1;
         }
         else 
         {  
            if((ypt==a->y)&&(ypt==b->y))                           
            {
	           if((xpt>=a->x)&&(xpt<=b->x))                           
	              retcode = 1; 
               if((xpt<=a->x)&&(xpt>=b->x))
	              retcode = 1;
            }
            else
            {  
               dx = a->x-b->x;                                        
			   dy = a->y-b->y;
               if(absval(dy)<=absval(dx))                             
               {                                                     
                  yline = b->y+((xpt-b->x)*dy)/dx;
				  if(ypt==yline)
				     retcode = 1;
	           }
               else                                                   
	           {                                                     
                  xline = b->x+((ypt-b->y)*dx)/dy;
				  if(xpt==xline)
				     retcode = 1;
	           }
            }
         }
	  } 
   }
   return(retcode);

}

_UWORD DotInPoly2D_C(_KOORD x, _KOORD y, _DOT *p, _UWORD no)    // konvexe Variante
{
   _UWORD   i;
   _UWORD   retcode;
   _KOORD   z;
   _DOT     p0, pno1, p1;
   _DOT     *pd1;
   _DOT     *pd2;

   p0 = p[0];
   p1 = p[1];
   pno1 = p[no-1];

   retcode = 1;
   z = (p0.x-pno1.x)*(y-pno1.y) - (p0.y-pno1.y)*(x-pno1.x);
   if(z<0)
   {
      for(i=0, pd1=&p[0], pd2=&p[1];i<no-1;i++, pd1++, pd2++)
	  {
         z = (pd2->x-pd1->x)*(y-pd1->y) - (pd2->y-pd1->y)*(x-pd1->x);
		 if(z>0)
		 {retcode = 0; break;}
	  }
   }
   else if(z>0)
   {
      for(i=0, pd1=&p[0], pd2=&p[1];i<no-1;i++, pd1++, pd2++)
      {
	     z = (pd2->x-pd1->x)*(y-pd1->y) - (pd2->y-pd1->y)*(x-pd1->x);
		 if(z<0)
		 {retcode = 0; break;}
	  } 
   }
   else
   {
      z = (p1.x-p0.x)*(y-p0.y) - (p1.y-p0.y)*(x-p0.x);
	  if(z<=0)
      {
         for(i=1, pd1=&p[1], pd2=&p[2]; i<no-1; i++, pd1++, pd2++)
	     {
            z = (pd2->x-pd1->x)*(y-pd1->y) - (pd2->y-pd1->y)*(x-pd1->x);
		    if(z>0)
	        {retcode = 0; break;}
	     }
      }
      else
      {
         for(i=1, pd1=&p[1], pd2=&p[2]; i<no-1; i++, pd1++, pd2++)
         {
	        z = (pd2->x-pd1->x)*(y-pd1->y) - (pd2->y-pd1->y)*(x-pd1->x);
		    if(z<0)
		    {retcode = 0; break;}
	     } 
      }
   }
  
   return(retcode);

}

_DWORD DotInPoly2D_NC(_KOORD x, _KOORD y, _DOT *p, _UWORD no)      // allgemeine Variante (auch nicht konvex) 
{
   _DWORD   retcode, count;
   _BOOL    frame;
   _KOORD   z;
   _UWORD   i;
   _DOT     p0, p1, p2, pno1, pno2, pno3;
   _DOT     *pd0;
   _DOT     *pd1;
   _DOT     *pd2;
   _DOT     *pd3;
   
   p0 = p[0];
   p1 = p[1];
   p2 = p[2];
   pno1 = p[no-1];
   pno2 = p[no-2];
   pno3 = p[no-3];
   count = 0;
   retcode = 0;
   frame = _FALSE;
   
   if(y==pno1.y)
   {
      if((pno1.y-p0.y)*(pno1.y-pno2.y)>0)
	  {
         if(x==pno1.x)
		    frame = _TRUE;
	  }
	  else if((pno1.y-p0.y)*(pno1.y-pno2.y)<0)
      {
         z = (p0.x-pno1.x)*(y-pno1.y) - (x-pno1.x)*(p0.y-pno1.y);
		 if(z<=0)
		    count++;
         else
		    count--;
   	  }
	  else if(pno1.y==p0.y)
	  {
         if((pno1.x-x)*(p0.x-x)<=0)
		    frame = _TRUE;
		 if((pno2.y-pno1.y)*(p1.y-pno1.y)<0)
		 {
            z = (p1.x-p0.x)*(y-p0.y) - (x-p0.x)*(p1.y-p0.y);
		    if(z<=0)
		       count++;
            else
		       count--;
		 }
   	  }
	
   }   
   else if(((y<pno1.y)&&(y>p0.y))||((y>pno1.y)&&(y<p0.y)))
   {
      z = (p0.x-pno1.x)*(y-pno1.y) - (x-pno1.x)*(p0.y-pno1.y);
      if(z<=0)
		 count++;
      else
		 count--;
   }
   if(!frame)
   {
      if(y==p0.y)
      {
         if((p0.y-pno1.y)*(p0.y-p1.y)>0)
	     {
            if(x==p0.x)
		       frame = _TRUE;
	     }
	     else if((p0.y-pno1.y)*(p0.y-p1.y)<0)
         {
            z = (p1.x-p0.x)*(y-p0.y) - (x-p0.x)*(p1.y-p0.y);
		    if(z<=0)
		       count++;
            else
		       count--;
  	     }
	     else if(p1.y==p0.y)
	     {
            if((p1.x-x)*(p0.x-x)<=0)
		       frame = _TRUE;
            if((pno1.y-p0.y)*(p2.y-p0.y)<0)
			{
               z = (p2.x-p1.x)*(y-p1.y) - (x-p1.x)*(p2.y-p1.y);
		       if(z<=0)
		          count++;
               else
		          count--;
			}
         }
      }   
      else if(((y<p0.y)&&(y>p1.y))||((y>p0.y)&&(y<p1.y)))
      {
         z = (p1.x-p0.x)*(y-p0.y) - (x-p0.x)*(p1.y-p0.y);
         if(z<=0)
		    count++;
         else
		    count--;
      }
   }
   if(!frame)
   {
      for(i=1, pd0=&p[0], pd1=&p[1], pd2=&p[2], pd3=&p[3]; i<no-2; i++, pd0++, pd1++, pd2++, pd3++)
	  {
	     if(y==pd1->y)
         {
            if((pd1->y-pd0->y)*(pd1->y-pd2->y)>0)
	        {
               if(x==pd1->x)
		          {frame = _TRUE; break;}
	        }
	        else if((pd1->y-pd0->y)*(pd1->y-pd2->y)<0)
            {
               z = (pd2->x-pd1->x)*(y-pd1->y) - (x-pd1->x)*(pd2->y-pd1->y);
		       if(z<=0)
		          count++;
               else
		          count--;
  	        }
	        else if(pd2->y==pd1->y)
	        {
               if((pd2->x-x)*(pd1->x-x)<=0)
		          {frame = _TRUE; break;}
			   if((pd0->y-pd1->y)*(pd3->y-pd1->y)<0)
			   {
                  z = (pd3->x-pd2->x)*(y-pd2->y) - (x-pd2->x)*(pd3->y-pd2->y);
		          if(z<=0)
		             count++;
                  else
		             count--;
			   }
            }
         }   
         else if(((y<pd1->y)&&(y>pd2->y))||((y>pd1->y)&&(y<pd2->y)))
         {
            z = (pd2->x-pd1->x)*(y-pd1->y) - (x-pd1->x)*(pd2->y-pd1->y);
            if(z<=0)
		       count++;
            else
		       count--;
         }
	  }
   }
   if(!frame)
   {
      if(y==pno2.y)
      {
         if((pno2.y-pno1.y)*(pno2.y-pno3.y)>0)
	     {
            if(x==pno2.x)
		       frame = _TRUE;
	     }
	     else if((pno2.y-pno1.y)*(pno2.y-pno3.y)<0)
         {
            z = (pno1.x-pno2.x)*(y-pno2.y) - (x-pno2.x)*(pno1.y-pno2.y);
		    if(z<=0)
		       count++;
            else
		       count--;
  	     }
	     else if(pno1.y==pno2.y)
	     {
            if((pno1.x-x)*(pno2.x-x)<=0)
		       frame = _TRUE;
            if((pno2.y-p0.y)*(pno2.y-pno3.y)<0)
			{
               z = (p0.x-pno1.x)*(y-pno1.y) - (x-pno1.x)*(p0.y-pno1.y);
		       if(z<=0)
		          count++;
               else
		          count--;
			}
         }
      }   
      else if(((y<pno2.y)&&(y>pno1.y))||((y>pno2.y)&&(y<pno1.y)))
      {
         z = (pno1.x-pno2.x)*(y-pno2.y) - (x-pno2.x)*(pno1.y-pno2.y);
         if(z<=0)
		    count++;
         else
		    count--;
      }
   }
   if(frame)
      retcode = 1;
   if((count==2)||(count==-2))
      retcode = 1;
   
   return(retcode);
}

_DWORD DotInPoly2D(_KOORD x, _KOORD y, _DOT *p, _UWORD no, _BOOL conv)
{
   _DWORD   retcode;
   
   if(conv==_TRUE)
      retcode = DotInPoly2D_C(x,y,p,no);
   else
      retcode = DotInPoly2D_NC(x,y,p,no);
   
   return(retcode);
}

_DWORD DotInPoly3D(_SHAPE *p, _KOORD x, _KOORD y)  
{
   _UWORD   i;
   _DWORD   retcode;    
   _DWORD   z;
   _DOT3D   *pd1; 
   _DOT3D   *pd2;
   
	if(p->header.pcd<=0)
	{
		retcode = 1;
	  for(i=0, pd1=&p->ptr[0], pd2=&p->ptr[1]; i<p->no-1; i++, pd1++, pd2++)
	  {
	     z = (pd2->x - pd1->x)*(y - pd1->y)-(x - pd1->x)*(pd2->y - pd1->y);
	     if(z > 0)  
		    {retcode = 0; break;}                      
      }
   }
   else                                            
   {                                               
      retcode = -1;
	  for(i=0, pd1=&p->ptr[0], pd2=&p->ptr[1]; i<p->no-1; i++, pd1++, pd2++)
	  {
	     z = (pd2->x - pd1->x)*(y - pd1->y)-(x - pd1->x)*(pd2->y - pd1->y);
	     if(z < 0)  
		    {retcode = 0; break;}                       
      }
   }
   return(retcode);                                    
}  

_UWORD DotOnPoly2D(_KOORD x, _KOORD y, _DOT *p, _UWORD no, _BOOL convex)
{
   _UWORD  retcode;
   _KOORD  i;
   
   retcode = 0;
   if(DotInPoly2D(x-1,y,p,no,convex)==0)
      retcode = 1;
   else
   {
	  if(DotInPoly2D(x+1,y,p,no,convex)==0)
         retcode = 1;
      else
      {
	     for(i=-1;i<2;i++)
         {
            if(DotInPoly2D(x+i,y+1,p,no,convex)==0)
	           {retcode = 1; break;}
	        if(DotInPoly2D(x+i,y-1,p,no,convex)==0)
	           {retcode = 1; break;}
         }
      }
   }
   return(retcode);
	  
}

_BOOL POLY_convex(_DOT *p, _UWORD no)
{  
   _UWORD   i;
   _KOORD   z;
   _BOOL    retcode=_TRUE;
   _DOT     p0, p1, pno1, pno2;
   _DOT     *pd0;
   _DOT     *pd1;
   _DOT     *pd2;

   p0 = p[0];
   p1 = p[1];
   pno1 = p[no-1];
   pno2 = p[no-2];


   z = (p1.x-p0.x)*(p0.y-pno1.y) - (p1.y-p0.y)*(p0.x-pno1.x);
   
   if(z<0)
   {
      for(i=1, pd0=&p[0], pd1=&p[1], pd2=&p[2]; i<no-2; i++, pd0++, pd1++, pd2++)
	  {
	     z = (pd2->x-pd1->x)*(pd1->y-pd0->y) - (pd2->y-pd1->y)*(pd1->x-pd0->x);
		 if(z>0)
		    {retcode = _FALSE; break;}
	  }   
	  if(retcode)
	  { 
         z = (p0.x-pno1.x)*(pno1.y-pno2.y) - (p0.y-pno1.y)*(pno1.x-pno2.x);
		 if(z>0)
		    retcode = _FALSE;
	  }
   }
   if(z>0)
   {  
      for(i=1, pd0=&p[0], pd1=&p[1], pd2=&p[2]; i<no-2; i++, pd0++, pd1++, pd2++)
      {
	     z = (pd2->x-pd1->x)*(pd1->y-pd0->y) - (pd2->y-pd1->y)*(pd1->x-pd0->x);
		 if(z<0)
		    {retcode = _FALSE; break;}
	  }
	  if(retcode)
	  {
         z = (p0.x-pno1.x)*(pno1.y-pno2.y) - (p0.y-pno1.y)*(pno1.x-pno2.x);
		 if(z<0)
		    retcode = _FALSE;
	  }
   }
   return(retcode);

}

_BOOL DotOnLine3D(_KOORD xpt, _KOORD ypt, _DOT3D *a, _DOT3D *b)   
{                                                               
   _BOOL    retcode=_FALSE;
   _KOORD   dx, dy, xline, yline;                      
      
   if(((xpt<=a->x)&&(xpt>=b->x))||((xpt>=a->x)&&(xpt<=b->x)))        
   {                                                                 
      if(((ypt<=a->y)&&(ypt>=b->y))||((ypt>=a->y)&&(ypt<=b->y)))
	  {
	     if((xpt==a->x)&&(xpt==b->x))      
         {
            if((ypt>=a->y)&&(ypt<=b->y))   
	           retcode = _TRUE;
            if((ypt<=a->y)&&(ypt>=b->y))
	           retcode = _TRUE;
         }
         else 
         {  
            if((ypt==a->y)&&(ypt==b->y))   
            {
	           if((xpt>=a->x)&&(xpt<=b->x))
	              retcode = _TRUE; 
               if((xpt<=a->x)&&(xpt>=b->x))
	              retcode = _TRUE;
            }
            else
            {  
               dx = a->x-b->x;             
			   dy = a->y-b->y;
               if(absval(dy)<=absval(dx))  
               {                                  
                  yline = b->y+((xpt-b->x)*dy)/dx;
				  if(ypt==yline)
				     retcode = _TRUE;
	           }
               else                               
	           {                                  
                  xline = b->x+((ypt-b->y)*dx)/dy;
				  if(xpt==xline)
				     retcode = _TRUE;
	           }
            }
         }
	  } 
   }
   return(retcode);
} 

void draw_SHAPE(_KOORD x, _KOORD y, _SHAPE *p)
{
  _DOT   dot[100];              
  _DOT   *d;
  _DOT3D *s;
  _UWORD i;
  _COLOR color;
                          

     d = dot;
     s = p->ptr;
     for(i=0; i<p->no; i++)
     {
     	d->x = s->x + x;
     	d->y = s->y + y;
     	d += 1;
     	s += 1;
     }

     color = p->color;
	  if(p->header.pcd < 0)
		 color = p->backcolor;

     if(GetPenColor_Makro(color) != INVISIBLE)
       Polygon(p->no, dot, T_COPY|T_SOLID|T_FILL, color);

     if(GetBackColor_Makro(color) != INVISIBLE)
       Polygon(p->no, dot, T_COPY|T_SOLID, GetBackColor_Makro(color));
  
}

void draw_3DSHAPE_MK(_KOORD xscr, _KOORD yscr, _3DSHAPE *p)      // Zeichen-Routine mit Kanten
{
   _UWORD    i,j;
   _KOORD    y,x;
   _COLOR    color;
   _BOOL     draw;
   _UWORD    inpoly;
   _BOOL     onframe;
   _REAL     xreal, yreal, z, zakt; 
   _REAL     pijx,pij1x,pijy,pij1y,pijz,pij1z;
   _REAL     a,b,c,d;
   _SHAPE    *ptr;
   _DOT3D    *pd1;
   _DOT3D    *pd2;
  
    
   for(y=p->header.ymin ; y<=p->header.ymax; y++)      
   {                                                      
      for(x=p->header.xmin; x<=p->header.xmax; x++)    
	  {  
	     draw = _FALSE;                                     
		 zakt = 10000;                                      
		    
         for(i=0, ptr=&p->ptr[0]; i<p->no; i++, ptr++)                             
		 {  
			 if(ptr->header.pcd!=0)
			{
		       if((y<=ptr->header.ymax)&&(y>=ptr->header.ymin)&&(GetPenColor_Makro(ptr->color) != INVISIBLE)) 
		       {
			      if((x<=ptr->header.xmax)&&(x>=ptr->header.xmin))    
                  {  
				     inpoly = DotInPoly2D(x,y,ptr->header. ptr2d,ptr->no-1, ptr->header.convex);
					 if((inpoly!=0))
				     {   
					    draw = _TRUE;
						onframe = (DotOnPoly2D(x,y,ptr->header.ptr2d,ptr->no-1, ptr->header.convex)!=0);
						xreal = x;
						yreal = y;
						a = ptr->header.pad;
						b = ptr->header.pbd;
						c = ptr->header.pcd;
						d = ptr->header.pdd;
						z = (d-xreal*a-yreal*b)/c;
						if(z<ptr->header.zmin)
						   z = ptr->header.zmin;
				        if(z < zakt)
						{
						   zakt = z;
                           if(onframe)
						      color = GetBackColor_Makro(ptr->color);
                           else
						   {
										if(ptr->header.pcd<0)
								  color = ptr->color;
										if(ptr->header.pcd>0)
						         color = ptr->backcolor;
						   }
						}
					 }

					 else          
					 {
                        for(j=0, pd1=&ptr->ptr[0], pd2=&ptr->ptr[1]; j<ptr->no-1; j++, pd1++, pd2++)               
			            {
                           if(DotOnLine3D(x,y,pd1,pd2)) 
				           {
                              draw = _TRUE;                                         
					          xreal = x;
						      yreal = y;
						      pijx = pd1->x;
						      pij1x = pd2->x;
						      pijy = pd1->y;
						      pij1y = pd2->y;
						      pijz = pd1->z;
						      pij1z = pd2->z;
						      if(pd1->x!=pd2->x)
					          {
						         if(pd1->y!=pd2->y)
							     {
						            if(absval(pd1->x-pd2->x)>=absval(pd1->y-pd2->y))
							           z = pijz + (pij1z-pijz)*(xreal-pijx)/(pij1x-pijx);
                                    else
								       z = pijz + (pij1z-pijz)*(yreal-pijy)/(pij1y-pijy);
							     }
							     else
							        z = pijz + (pij1z-pijz)*(xreal-pijx)/(pij1x-pijx);
                              }
				              else
					          {
                                 if(pd1->y!=pd2->y)
							        z = pijz + (pij1z-pijz)*(yreal - pijy)/(pij1y - pijy);
						         else
							     {
						            if(pd1->z<=pd2->z)
								       z = pijz;
                                    else
								       z = pij1z;
                                 }
					          }   
                              if(z<ptr->header.zmin)
							     z = ptr->header.zmin;
                              if(z<zakt)                                          
					             {color = GetBackColor_Makro(ptr->color); zakt = z;} 
							  break;	                       
				           }
			            }
					 }       
				  }          
			   }            
            }               
			else                                             
			{
               for(j=0, pd1=&ptr->ptr[0], pd2=&ptr->ptr[1]; j<ptr->no-1; j++, pd1++, pd2++)               
			   {
                  if(DotOnLine3D(x,y,pd1,pd2)) 
				  {
                     draw = _TRUE;                                         
					 xreal = x;
				     yreal = y;
					 pijx = pd1->x;
					 pij1x = pd2->x;
					 pijy = pd1->y;
					 pij1y = pd2->y;
					 pijz = pd1->z;
					 pij1z = pd2->z;
					 if(pd1->x!=pd2->x)
					 {
					    if(pd1->y!=pd2->y)
					    {
					       if(absval(pd1->x-pd2->x)>=absval(pd1->y-pd2->y))
					          z = pijz + (pij1z-pijz)*(xreal-pijx)/(pij1x-pijx);
                           else
					          z = pijz + (pij1z-pijz)*(yreal-pijy)/(pij1y-pijy);
					    }
					    else
					    z = pijz + (pij1z-pijz)*(xreal-pijx)/(pij1x-pijx);
                     }  
				     else
					 {
                        if(pd1->y!=pd2->y)
						   z = pijz + (pij1z-pijz)*(yreal - pijy)/(pij1y - pijy);
						else
						{
						   if(pd1->z<=pd2->z)
						      z = pijz;
                           else
						      z = pij1z;
                        }
					 }   
                     if(z<ptr->header.zmin)
					    z = ptr->header.zmin;
                     if(z<zakt)                                          
					    {color = GetBackColor_Makro(ptr->color); zakt = z;} 
					 break;             
				  }
			   }
			}
         }                
		 if(draw)	                                                  
	        WritePixel(x+xscr,y+yscr, T_COPY|T_SOLID, color);
	  }                  
   }                    

}

void draw_3DSHAPE_OK(_KOORD xscr, _KOORD yscr, _3DSHAPE *p)      // Zeichenroutine - ohne Kanten
{
   _UWORD    i,j;
   _KOORD    y,x;
   _COLOR    color;
   _BOOL     draw;
   _UWORD    inpoly;
   _REAL     xreal, yreal, z, zakt; 
   _REAL     pijx,pij1x,pijy,pij1y,pijz,pij1z;
   _REAL     a,b,c,d;
   _SHAPE    *ptr;
   _DOT3D    *pd1;
   _DOT3D    *pd2;
  
    
   for(y=p->header.ymin ; y<=p->header.ymax; y++)      
   {                                                      
      for(x=p->header.xmin; x<=p->header.xmax; x++)    
	  {  
	     draw = _FALSE;                                     
		 zakt = 10000;                                      
		    
         for(i=0, ptr=&p->ptr[0]; i<p->no; i++, ptr++)                             
		 {  
			 if(ptr->header.pcd!=0)
			{
		       if((y<=ptr->header.ymax)&&(y>=ptr->header.ymin)&&(GetPenColor_Makro(ptr->color) != INVISIBLE))
		       {
			      if((x<=ptr->header.xmax)&&(x>=ptr->header.xmin))    
                  {  
				     inpoly = DotInPoly2D(x,y,ptr->header.ptr2d,ptr->no-1, ptr->header.convex);
					 if(inpoly!=0)
				     {   
					    draw = _TRUE;
						xreal = x;
						yreal = y;
						a = ptr->header.pad;
						b = ptr->header.pbd;
						c = ptr->header.pcd;
						d = ptr->header.pdd;
						z = (d-xreal*a-yreal*b)/c;
						if(z<ptr->header.zmin)
						   z = ptr->header.zmin;
				        if(z < zakt)
						{
						   zakt = z;
									if(ptr->header.pcd<0)
							  color = ptr->color;
									if(ptr->header.pcd>0)
						      color = ptr->backcolor;
						}
					 } 
				  }         
			   }         
            }               
			else                                             
			{
               for(j=0, pd1=&ptr->ptr[0], pd2=&ptr->ptr[1]; j<ptr->no-1; j++, pd1++, pd2++)               
			   {
                  if(DotOnLine3D(x,y,pd1,pd2)) 
				  {
                     draw = _TRUE;                                         
					 xreal = x;
				     yreal = y;
					 pijx =  pd1->x;
					 pij1x = pd2->x;
					 pijy =  pd1->y;
					 pij1y = pd2->y;
					 pijz =  pd1->z;
					 pij1z = pd2->z;
					 if(pd1->x!=pd2->x)
					 {
					    if(pd1->y!=pd2->y)
					    {
					       if(absval(pd1->x-pd2->x)>=absval(pd1->y-pd2->y))
					          z = pijz + (pij1z-pijz)*(xreal-pijx)/(pij1x-pijx);
                           else
					          z = pijz + (pij1z-pijz)*(yreal-pijy)/(pij1y-pijy);
					    }
					    else
					    z = pijz + (pij1z-pijz)*(xreal-pijx)/(pij1x-pijx);
                     }  
				     else
					 {
                        if(pd1->y!=pd2->y)
						   z = pijz + (pij1z-pijz)*(yreal - pijy)/(pij1y - pijy);
						else
						{
						   if(pd1->z<=pd2->z)
						      z = pijz;
                           else
						      z = pij1z;
                        }
					 }   
                     if(z<ptr->header.zmin)
					    z = ptr->header.zmin;
                     if(z<zakt)                                          
					    {color = GetBackColor_Makro(ptr->color); zakt = z;} 
					 break;             
				  }
			   }
			}
         }                
		 if(draw)	                                                  
	        WritePixel(x+xscr,y+yscr, T_COPY|T_SOLID, color);
	  }
   }

}

void draw_3DSHAPE(_KOORD xscr, _KOORD yscr, _3DSHAPE *p)
{
   if(p->frame==_TRUE)
      draw_3DSHAPE_MK(xscr, yscr, p);
   else
      draw_3DSHAPE_OK(xscr, yscr, p);
}


//**************************************************************************************************************
//**************************************************************************************************************
//                                            BEISPIELE
//**************************************************************************************************************
//**************************************************************************************************************

//**************************************************************************************************************
//								                BLECH 1
//**************************************************************************************************************


extern "C" _UDWORD bl1rotto(_KOORD x, _KOORD y, _UWORD a1, _UWORD a2, _UWORD a3, _DWORD b1, _DWORD b2, _DWORD b3)
{
   _UDWORD retcode;
   _DOT3D   pdot[5], qdot[5], rdot[5], udot[5], vdot[5], wdot[5], sdot[5], tdot[5], xdot[5];  
   _SHAPE   s[9];  
   _3DSHAPE blech;
   _3DSHAPE *bl;


   pdot[0].x = -100; pdot[0].y = 25; pdot[0].z = -50;
   pdot[1].x =  100; pdot[1].y = 25; pdot[1].z = -50;
   pdot[2].x =  100; pdot[2].y = 25; pdot[2].z =  50;
   pdot[3].x = -100; pdot[3].y = 25; pdot[3].z =  50;
   pdot[4]   = pdot[0];
   s[0].ptr  = pdot;
   s[0].no   = 5;
   s[0].color = MakeColor_Makro(BLUE, LIGHTBLUE);
   s[0].backcolor = MakeColor_Makro(BLUE, LIGHTBLUE);


   qdot[0].x = 100; qdot[0].y =  25; qdot[0].z = -50;
   qdot[1].x = 100; qdot[1].y =  25; qdot[1].z =  50;
   qdot[2].x = 100; qdot[2].y = -25; qdot[2].z =  50;
   qdot[3].x = 100; qdot[3].y = -25; qdot[3].z = -50;
   qdot[4]   = qdot[0];
   s[1].ptr  = qdot;
   s[1].no   = 5;
   s[1].color = MakeColor_Makro(BLUE, LIGHTBLUE);
   s[1].backcolor = MakeColor_Makro(BLUE, LIGHTBLUE);


   rdot[0].x = -100; rdot[0].y =  25; rdot[0].z = -50;
   rdot[1].x = -100; rdot[1].y =  25; rdot[1].z =  50;
   rdot[2].x = -100; rdot[2].y = -25; rdot[2].z =  50;
   rdot[3].x = -100; rdot[3].y = -25; rdot[3].z = -50;
   rdot[4]   = rdot[0];
   s[2].ptr  = rdot;
   s[2].no   = 5;
   s[2].color = MakeColor_Makro(BLUE, LIGHTBLUE);
   s[2].backcolor = MakeColor_Makro(BLUE, LIGHTBLUE);

   sdot[0].x =  100; sdot[0].y = -25; sdot[0].z = -50;
   sdot[1].x =  100; sdot[1].y = -25; sdot[1].z =  50;
   sdot[2].x =   80; sdot[2].y = -25; sdot[2].z =  50;
   sdot[3].x =   80; sdot[3].y = -25; sdot[3].z = -50;
   sdot[4]   = sdot[0];
   s[3].ptr  = sdot;
   s[3].no   = 5;
   s[3].color = MakeColor_Makro(BLUE, LIGHTBLUE);
   s[3].backcolor = MakeColor_Makro(BLUE, LIGHTBLUE);
   
  
   tdot[0].x = -100; tdot[0].y = -25; tdot[0].z =  50;
   tdot[1].x = -100; tdot[1].y = -25; tdot[1].z = -50;
   tdot[2].x =  -70; tdot[2].y = -25; tdot[2].z = -50;
   tdot[3].x =  -70; tdot[3].y = -25; tdot[3].z =  50;
   tdot[4]   = tdot[0];
   s[4].ptr  = tdot;
   s[4].no   = 5;
   s[4].color = MakeColor_Makro(BLUE, LIGHTBLUE);
   s[4].backcolor = MakeColor_Makro(BLUE, LIGHTBLUE);


   udot[0].x =   80; udot[0].y = -25; udot[0].z = -50;
   udot[1].x =   80; udot[1].y =   5; udot[1].z = -50;
   udot[2].x =   80; udot[2].y =   5; udot[2].z =  50;
   udot[3].x =   80; udot[3].y = -25; udot[3].z =  50;
   udot[4]   = udot[0];
   s[5].ptr  = udot;
   s[5].no   = 5;
   s[5].color = MakeColor_Makro(BLUE, LIGHTBLUE);
   s[5].backcolor = MakeColor_Makro(BLUE, LIGHTBLUE);


   vdot[0].x =  -70; vdot[0].y = -25; vdot[0].z = -50;
   vdot[1].x =  -70; vdot[1].y =   5; vdot[1].z = -50;
   vdot[2].x =  -70; vdot[2].y =   5; vdot[2].z =  50;
   vdot[3].x =  -70; vdot[3].y = -25; vdot[3].z =  50;
   vdot[4]   = vdot[0];
   s[6].ptr  = vdot;
   s[6].no   = 5;
   s[6].color = MakeColor_Makro(BLUE, LIGHTBLUE);
   s[6].backcolor = MakeColor_Makro(BLUE, LIGHTBLUE);


   wdot[0].x =   80; wdot[0].y =   5; wdot[0].z = -50;
   wdot[1].x =   90; wdot[1].y =  -5; wdot[1].z = -50;
   wdot[2].x =   90; wdot[2].y =  -5; wdot[2].z =  50;
   wdot[3].x =   80; wdot[3].y =   5; wdot[3].z =  50;
   wdot[4]   = wdot[0];
   s[7].ptr  = wdot;
   s[7].no   = 5;
   s[7].color = MakeColor_Makro(BLUE, LIGHTBLUE);
   s[7].backcolor = MakeColor_Makro(BLUE, LIGHTBLUE); 


   xdot[0].x =  -70; xdot[0].y =   5; xdot[0].z = -50;
   xdot[1].x =  -80; xdot[1].y =  -5; xdot[1].z = -50;
   xdot[2].x =  -80; xdot[2].y =  -5; xdot[2].z =  50;
   xdot[3].x =  -70; xdot[3].y =   5; xdot[3].z =  50;
   xdot[4]   = xdot[0];
   s[8].ptr  = xdot;
   s[8].no   = 5;
   s[8].color = MakeColor_Makro(BLUE, LIGHTBLUE);
   s[8].backcolor = MakeColor_Makro(BLUE, LIGHTBLUE);


   blech.no = 9;
   blech.ptr = s;
   blech.shading = _TRUE;
   blech.frame = _FALSE;
   bl = &blech;

   rotate_3DSHAPE(bl, a1, a2, a3);
   retcode = get_abstime();
   draw_3DSHAPE(x,y,bl);
   retcode = timediff(retcode);
   
   return(retcode);
}
//******************************************************************************************************************


                                                                               
//******************************************************************************************************************																			   
//								                BLECH 2
//******************************************************************************************************************

extern "C" _UDWORD bl2rotto(_KOORD x, _KOORD y, _UWORD a1, _UWORD a2, _UWORD a3, _DWORD b1, _DWORD b2, _DWORD b3)
{
   _DOT3D   pdot[5], qdot[5], rdot[5], udot[5], vdot[5], wdot[5], sdot[5], tdot[5], xdot[5];
   _SHAPE   s[9];
   _3DSHAPE blech;
   _3DSHAPE *bl;


   pdot[0].x = -100; pdot[0].y =  50; pdot[0].z = -50;
   pdot[1].x =  100; pdot[1].y =  50; pdot[1].z = -50;
   pdot[2].x =  100; pdot[2].y =  50; pdot[2].z =  50;
   pdot[3].x = -100; pdot[3].y =  50; pdot[3].z =  50;
   pdot[4]   = pdot[0];
   s[0].ptr  = pdot;
   s[0].no   = 5;
   s[0].color = MakeColor_Makro(RED, LIGHTRED);
   s[0].backcolor = MakeColor_Makro(RED, LIGHTRED);


   qdot[0].x = -100; qdot[0].y =  50; qdot[0].z = -50;
   qdot[1].x = -100; qdot[1].y =  50; qdot[1].z =  50;
   qdot[2].x = -100; qdot[2].y =  40; qdot[2].z =  50;
   qdot[3].x = -100; qdot[3].y =  40; qdot[3].z = -50;
   qdot[4]   = qdot[0];
   s[1].ptr  = qdot;
   s[1].no   = 5;
   s[1].color = MakeColor_Makro(RED, LIGHTRED);
   s[1].backcolor = MakeColor_Makro(RED, LIGHTRED);


   rdot[0].x = -100; rdot[0].y =  40; rdot[0].z = -50;
   rdot[1].x =  -30; rdot[1].y =  40; rdot[1].z = -50;
   rdot[2].x =  -30; rdot[2].y =  40; rdot[2].z =  50;
   rdot[3].x = -100; rdot[3].y =  40; rdot[3].z =  50;
   rdot[4]   = rdot[0];
   s[2].ptr  = rdot;
   s[2].no   = 5;
   s[2].color = MakeColor_Makro(RED, LIGHTRED);
   s[2].backcolor = MakeColor_Makro(RED, LIGHTRED);

   sdot[0].x =  -30; sdot[0].y =  40; sdot[0].z = -50;
   sdot[1].x =  -30; sdot[1].y =  40; sdot[1].z =  50;
   sdot[2].x =  -80; sdot[2].y =   0; sdot[2].z =  50;
   sdot[3].x =  -80; sdot[3].y =   0; sdot[3].z = -50;
   sdot[4]   = sdot[0];
   s[3].ptr  = sdot;
   s[3].no   = 5;
   s[3].color = MakeColor_Makro(RED, LIGHTRED);
   s[3].backcolor = MakeColor_Makro(RED, LIGHTRED);


   tdot[0].x =  -80; tdot[0].y =   0; tdot[0].z = -50;
   tdot[1].x =  -10; tdot[1].y = -20; tdot[1].z = -50;
   tdot[2].x =  -10; tdot[2].y = -20; tdot[2].z =  50;
   tdot[3].x =  -80; tdot[3].y =   0; tdot[3].z =  50;
   tdot[4]   = tdot[0];
   s[4].ptr  = tdot;
   s[4].no   = 5;
   s[4].color = MakeColor_Makro(RED, LIGHTRED);
   s[4].backcolor = MakeColor_Makro(RED, LIGHTRED);


   udot[0].x =  -10; udot[0].y = -20; udot[0].z = -50;
   udot[1].x =  -10; udot[1].y = -20; udot[1].z =  50;
   udot[2].x =  -15; udot[2].y = -30; udot[2].z =  50;
   udot[3].x =  -15; udot[3].y = -30; udot[3].z = -50;
   udot[4]   = udot[0];
   s[5].ptr  = udot;
   s[5].no   = 5;
   s[5].color = MakeColor_Makro(RED, LIGHTRED);
   s[5].backcolor = MakeColor_Makro(RED, LIGHTRED);


   vdot[0].x =  -15; vdot[0].y = -30; vdot[0].z = -50;
   vdot[1].x =  -15; vdot[1].y = -30; vdot[1].z =  50;
   vdot[2].x =  -50; vdot[2].y = -20; vdot[2].z =  50;
   vdot[3].x =  -50; vdot[3].y = -20; vdot[3].z = -50;
   vdot[4]   = vdot[0];
   s[6].ptr  = vdot;
   s[6].no   = 5;
   s[6].color = MakeColor_Makro(RED, LIGHTRED);
   s[6].backcolor = MakeColor_Makro(RED, LIGHTRED);


   wdot[0].x =  -50; wdot[0].y = -20; wdot[0].z = -50;
   wdot[1].x =  -50; wdot[1].y = -20; wdot[1].z =  50;
   wdot[2].x =  -55; wdot[2].y = -30; wdot[2].z =  50;
   wdot[3].x =  -55; wdot[3].y = -30; wdot[3].z = -50;
   wdot[4]   = wdot[0];
   s[7].ptr  = wdot;
   s[7].no   = 5;
   s[7].color = MakeColor_Makro(RED, LIGHTRED);
   s[7].backcolor = MakeColor_Makro(RED, LIGHTRED);


   xdot[0].x = -55; xdot[0].y =  -30; xdot[0].z = -50;
   xdot[1].x =  15; xdot[1].y =  -50; xdot[1].z = -50;
   xdot[2].x =  15; xdot[2].y =  -50; xdot[2].z =  50;
   xdot[3].x = -55; xdot[3].y =  -30; xdot[3].z =  50;
   xdot[4]   = xdot[0];
   s[8].ptr  = xdot;
   s[8].no   = 5;
   s[8].color = MakeColor_Makro(RED, LIGHTRED);
   s[8].backcolor = MakeColor_Makro(RED, LIGHTRED);


   blech.no = 9;
   blech.ptr = s;
   blech.shading = _TRUE;
   blech.frame = _TRUE;
   bl = &blech;


   rotate_3DSHAPE(bl, a1, a2, a3);
   draw_3DSHAPE(x,y,bl);

   return(0);
}
//******************************************************************************************************************



//******************************************************************************************************************
//												ROBOTERARM
//******************************************************************************************************************


extern "C" _UDWORD robotto(_KOORD x, _KOORD y, _UWORD a1, _UWORD a2, _UWORD a3, _DWORD b1, _DWORD b2, _DWORD b3)
{
   _UWORD     i;
   _3DSHAPE   sh1, sh2, sh3, sh4, sh5, sh6, sh7;
   _3DSHAPE   p1, p2, p3, p4, p5, p6;
   _DOT       pkt1[13];
   _DOT       pkt2[15];
   _DOT       pkt3[21];
   _DOT       pkt4[15];
   _DOT       pkt5[13];
   _DOT       pkt6[4];
   _DOT       pkt7[4];
   _DOT       *pd;
   _DOT   	  *pd1;
   _DOT       centerpt1, centerpt2, centerpt3, centerpt4;
   _UWORD     depth1,depth2, depth3, depth4, depth5, depth6, depth7;
   _COLOR     ffcolor1, bfcolor1, ffcolor2, bfcolor2, ffcolor3, bfcolor3, ffcolor4, bfcolor4;
   _COLOR     ffcolor5, bfcolor5, ffcolor6, bfcolor6, ffcolor7, bfcolor7;
   _REAL      tmp; 


   pkt1[0].x = -20;                                   	// Teil 1
   pkt1[0].y =  15;
   pkt1[1].x = -20;
   pkt1[1].y =  95;
   pkt1[2].x =  20;
   pkt1[2].y =  95;
   pkt1[3].x =  20;
   pkt1[3].y =  15;
   for(i=1, pd=&pkt1[4]; i<9; i++, pd++)
   {
      pd->x = FTOL(20*COS((i*PI)/9));
	  pd->y = FTOL(15-15*SIN((-i*PI)/9));
   }
   pkt1[12].x = pkt1[0].x;
   pkt1[12].y = pkt1[0].y;
   depth1 = 40;
   ffcolor1 = MakeColor_Makro(BLUE, LIGHTBLUE);
   bfcolor1 = MakeColor_Makro(BLUE, LIGHTBLUE);
   init_3DSHAPE(&sh1);


   for(i=0, pd=&pkt2[0]; i<14; i++, pd++)               // Gelenk 1
   {
      pd->x = FTOL(20*COS((i*2*PI)/14));
	  pd->y = FTOL(-20*SIN((i*2*PI)/14));
   }
   pkt2[14].x = pkt2[0].x;
   pkt2[14].y = pkt2[0].y;
   depth2 = 40;
   ffcolor2 = MakeColor_Makro(DARKGRAY, LIGHTGRAY);
   bfcolor2 = MakeColor_Makro(DARKGRAY, LIGHTGRAY);
   init_3DSHAPE(&sh2);
   centerpt1.x = 0;
   centerpt1.y = 0;

   pkt3[0].x = -20;                                    	// Teil 2
   pkt3[0].y = -95;
   pkt3[1].x = -20;
   pkt3[1].y = -15;
   for(i=1, pd=&pkt3[2]; i<9; i++, pd++)
   {
     tmp = (PI-i*PI);
     pd->x = FTOL(20*COS(tmp/9));
	   pd->y = FTOL(-15-15*SIN(tmp/9));
   }
   pkt3[10].x =  20;
   pkt3[10].y = -15;
   pkt3[11].x =  20;
   pkt3[11].y = -95;
   for(i=1, pd=&pkt3[12]; i<9; i++, pd++)
   {
     tmp = (-i*PI);
     pd->x = FTOL(20*COS(tmp/9));
     pd->y = FTOL(-95-15*SIN(tmp/9));
   }
   pkt3[20].x = pkt3[0].x;
   pkt3[20].y = pkt3[0].y;
   depth3 = 40;
   ffcolor3 = MakeColor_Makro(BLUE, LIGHTBLUE);
   bfcolor3 = MakeColor_Makro(BLUE, LIGHTBLUE);
   init_3DSHAPE(&sh3);


   for(i=0, pd=&pkt4[0]; i<14; i++, pd++)                // Gelenk 2
   {
      tmp = (i*2*PI);
      pd->x = FTOL(20*COS(tmp/14));
	    pd->y = FTOL(-110-20*SIN(tmp/14));
   }
   pkt4[14].x = pkt4[0].x;
   pkt4[14].y = pkt4[0].y;
   depth4 = 40;
   ffcolor4 = MakeColor_Makro(DARKGRAY, LIGHTGRAY);
   bfcolor4 = MakeColor_Makro(DARKGRAY, LIGHTGRAY);
   init_3DSHAPE(&sh4);
   centerpt2.x =    0;
   centerpt2.y = -110;


   pkt5[0].x =  -20;                           			// Teil 3
   pkt5[0].y = -160;
   pkt5[1].x =  -20;
   pkt5[1].y = -125;
   for(i=1, pd=&pkt5[2]; i<9; i++, pd++)
   {
     tmp = (i*PI);
     pd->x = FTOL(-20*COS(tmp/9));
	   pd->y = FTOL(-125-15*SIN(tmp/9));
   }
   pkt5[10].x =   20;
   pkt5[10].y = -125;
   pkt5[11].x =   20;
   pkt5[11].y = -160;
   pkt5[12].x =  -20;
   pkt5[12].y = -160;
   depth5 = 40;
   ffcolor5 = MakeColor_Makro(BLUE,LIGHTBLUE);
   bfcolor5 = MakeColor_Makro(BLUE,LIGHTBLUE);
   init_3DSHAPE(&sh5);
   centerpt3.x =  -18;
   centerpt3.y = -160;
   centerpt4.x =   18;
   centerpt4.y = -160;


   pkt6[0].x =   -5;     	 							// Greiferteil 1
   pkt6[0].y = -200;
   pkt6[1].x =  -20;
   pkt6[1].y = -165;
   pkt6[2].x =   -5;
   pkt6[2].y = -165;
   pkt6[3].x =   -5;
   pkt6[3].y = -200;
   depth6 = 40;
   ffcolor6 = MakeColor_Makro(DARKGRAY, LIGHTGRAY);
   bfcolor6 = MakeColor_Makro(DARKGRAY, LIGHTGRAY);
   init_3DSHAPE(&sh6);


   pkt7[0].x =    5;      								// Greiferteil 2
   pkt7[0].y = -200;
   pkt7[1].x =   20;
   pkt7[1].y = -165;
   pkt7[2].x =    5;
   pkt7[2].y = -165;
   pkt7[3].x =    5;
   pkt7[3].y = -200;
   depth7 = 40;
   ffcolor7 = MakeColor_Makro(DARKGRAY, LIGHTGRAY);
   bfcolor7 = MakeColor_Makro(DARKGRAY, LIGHTGRAY);
   init_3DSHAPE(&sh7);


   for(i=0, pd=&pkt2[0], pd1=&pkt4[0]; i<15; i++, pd++, pd1++)
   {
      rotate_DOT(pd, centerpt1.x, centerpt1.y, b1);
      rotate_DOT(pd1, centerpt1.x, centerpt1.y, b1);
   }
   for(i=0, pd=&pkt3[0]; i<21; i++, pd++)
      rotate_DOT(pd, centerpt1.x, centerpt1.y, b1);
   for(i=0, pd=&pkt5[0]; i<13; i++, pd++)
      rotate_DOT(pd, centerpt1.x, centerpt1.y,b1);
   for(i=0, pd=&pkt6[0], pd1=&pkt7[0]; i<4; i++, pd++, pd1++)
   {
      rotate_DOT(pd, centerpt1.x,centerpt1.y,b1);
      rotate_DOT(pd1, centerpt1.x,centerpt1.y,b1);
   }
   rotate_DOT(&centerpt2, centerpt1.x, centerpt1.y, b1);
   rotate_DOT(&centerpt3, centerpt1.x, centerpt1.y, b1);
   rotate_DOT(&centerpt4, centerpt1.x, centerpt1.y, b1);


   for(i=0, pd=&pkt4[0]; i<15; i++, pd++)
      rotate_DOT(pd, centerpt2.x, centerpt2.y, b2);
   for(i=0, pd=&pkt5[0]; i<13; i++, pd++)
      rotate_DOT(pd, centerpt2.x, centerpt2.y, b2);
   for(i=0, pd=&pkt6[0], pd1=&pkt7[0]; i<4; i++, pd++, pd1++)
   {
      rotate_DOT(pd, centerpt2.x, centerpt2.y,b2);
      rotate_DOT(pd1, centerpt2.x, centerpt2.y,b2);
   }
   rotate_DOT(&centerpt3, centerpt2.x, centerpt2.y, b2);
   rotate_DOT(&centerpt4, centerpt2.x, centerpt2.y, b2);

   for(i=0, pd=&pkt6[0], pd1=&pkt7[0]; i<4; i++, pd++, pd1++)
   {
      rotate_DOT(pd, centerpt3.x, centerpt3.y,b3);
      rotate_DOT(pd1, centerpt4.x, centerpt4.y,-b3);
   }


   polygon_to_SOLID3DSHAPE(&sh1, pkt1, 13, depth1, ffcolor1, bfcolor1, _TRUE, _FALSE);
   polygon_to_SOLID3DSHAPE(&sh2, pkt2, 15, depth2, ffcolor2, bfcolor2, _TRUE, _FALSE);
   polygon_to_SOLID3DSHAPE(&sh3, pkt3, 21, depth3, ffcolor3, bfcolor3, _TRUE, _FALSE);
   polygon_to_SOLID3DSHAPE(&sh4, pkt4, 15, depth4, ffcolor4, bfcolor4, _TRUE, _FALSE);
   polygon_to_SOLID3DSHAPE(&sh5, pkt5, 13, depth5, ffcolor5, bfcolor5, _TRUE, _FALSE);
   polygon_to_SOLID3DSHAPE(&sh6, pkt6,  4, depth6, ffcolor6, bfcolor6, _TRUE, _FALSE);
   polygon_to_SOLID3DSHAPE(&sh7, pkt7,  4, depth7, ffcolor7, bfcolor7, _TRUE, _FALSE);

   init_3DSHAPE(&p1);
   init_3DSHAPE(&p2);
   init_3DSHAPE(&p3);
   init_3DSHAPE(&p4);
   init_3DSHAPE(&p5);
   init_3DSHAPE(&p6);

   TWO_to_ONE3DSHAPE(&p1, &sh1, &sh2);
   TWO_to_ONE3DSHAPE(&p2, &p1,  &sh3);
   TWO_to_ONE3DSHAPE(&p3, &p2,  &sh4);
   TWO_to_ONE3DSHAPE(&p4, &p3,  &sh5);
   TWO_to_ONE3DSHAPE(&p5, &p4,  &sh6);
   TWO_to_ONE3DSHAPE(&p6, &p5,  &sh7);

   rotate_3DSHAPE(&p6,a1, a2, a3);
   draw_3DSHAPE(x,y,&p6);

   free_3DSHAPE(&p1);
   free_3DSHAPE(&p2);
   free_3DSHAPE(&p3);
   free_3DSHAPE(&p4);
   free_3DSHAPE(&p5);
   free_3DSHAPE(&p6);
   free_3DSHAPE(&sh1);
   free_3DSHAPE(&sh2);
   free_3DSHAPE(&sh3);
   free_3DSHAPE(&sh4);
   free_3DSHAPE(&sh5);
   free_3DSHAPE(&sh6);
   free_3DSHAPE(&sh7);


   return(0);
}

//******************************************************************************************************************




//******************************************************************************************************************
//                                             SIGMATEK-ZEICHEN
//******************************************************************************************************************

extern "C" _UDWORD sigrotto(_KOORD xscr, _KOORD yscr, _UWORD a1, _UWORD a2, _UWORD a3, _DWORD b1, _DWORD b2, _DWORD b3)
{
  _DOT     pkt[17];
  _3DSHAPE sh;


  pkt[0].x =  -60;        pkt[0].y =  -60;
  pkt[1].x =  -60;        pkt[1].y =  -40;
  pkt[2].x =  -20;        pkt[2].y =    0;
  pkt[3].x =  -60;        pkt[3].y =   40;
  pkt[4].x =  -60;        pkt[4].y =   60;
  pkt[5].x =   50;        pkt[5].y =   60;
  pkt[6].x =   50;        pkt[6].y =   40;
  pkt[7].x =   30;        pkt[7].y =   40;
  pkt[8].x =   30;        pkt[8].y =   45;
  pkt[9].x =  -35;        pkt[9].y =   45;
  pkt[10].x =  10;        pkt[10].y =   0;
  pkt[11].x = -35;        pkt[11].y = -45;
  pkt[12].x =  30;        pkt[12].y = -45;
  pkt[13].x =  30;        pkt[13].y = -40;
  pkt[14].x =  50;        pkt[14].y = -40;
  pkt[15].x =  50;        pkt[15].y = -60;
  pkt[16].x = pkt[0].x;   pkt[16].y = pkt[0].y;

  init_3DSHAPE(&sh);
  polygon_to_SOLID3DSHAPE(&sh, pkt, 17, 60, BLUE, LIGHTBLUE, _TRUE, _FALSE);

  rotate_3DSHAPE(&sh,a1, a2, a3);
  draw_3DSHAPE(xscr,yscr,&sh);

  free_3DSHAPE(&sh);

  return(0);
}

#endif // UC_GRAPHIC_STUDIO


//******************************************************************************************************************




/*

init_3DSHAPE();
free_3DSHAPE();

polygon_to_SOLID3DSHAPE();
rotate_3DSHAPE();
draw_3DSHAPE();
rotate_DOT(pd1, centerpt4.x, centerpt4.y,-b3);
*/




