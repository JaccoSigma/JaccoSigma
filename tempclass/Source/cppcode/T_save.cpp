#include "T_IPC.H"
#include "T_NUC.H"  

#ifdef OK_RUNTIME_MODIFY
/*
_BOOL _File::Write_ATTRIB(_ATTRIB *p)
{
  return(Write(p, sizeof(_ATTRIB)));
}

_BOOL _File::WriteHeader(_UBYTE *p)
{
  Write(p, 121); 
  Write("-clone-", 7); 
  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_COLOR(_COLOR *p)
{
  _BOOL retcode = _FALSE; 

  if(GetError() == _FALSE) 
  {
    retcode = Write(p, sizeof(_COLOR)); 
  }

  return(retcode); 
}

_BOOL _File::Write_ENUM(_UWORD p)
{
  return(Write(&p, sizeof(_UWORD))); 
}

_BOOL _File::Write_ANYTHING(_ANYTHING *p)
{
  Write_ENUM(p->list); 
  Write(&p->no, sizeof(_UWORD)); 

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_CHKBIT(_CHKBIT *p)
{
  Write(&p->seqno, sizeof(_UWORD)); 
  Write(&p->invert, sizeof(_UBYTE)); 

//  Write(&p->invert, sizeof(_UBYTE)); 
//  Write_VARLABEL(&p->chkbit); 

  return((GetError() == _TRUE)? _FALSE:_TRUE); 
}

_BOOL _File::Write_SINGLESOFT(_SINGLESOFT *ps)
{
  if(GetError() == _FALSE) 
  {
    Write_ATTRIB(&ps->attrib); 
    Write_COLOR(&ps->color); 
    Write_COLOR(&ps->frame); 
    Write_COLOR(&ps->text); 
    Write_ENUM(ps->tip); 
    Write_ANYTHING(&ps->no); 
    Write_CHKBIT(&ps->chkbit); 
    Write(&ps->enable, sizeof(_ENABLE)); 
    Write(&ps->font,   sizeof(_FONT)); 
    Write_GFUNCT(&ps->gfunct); 
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_GFUNCT(_GFUNCT *p)
{
  if(GetError() == _FALSE) 
  {
    Write(&p->pos_fl,   sizeof(_UWORD)); 
    Write(&p->neg_fl,   sizeof(_UWORD)); 
//    Write(&p->active,   sizeof(_UWORD));
//    Write(&p->inactive, sizeof(_UWORD));
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE);
} 

_BOOL _File::Write_SOFTFLOOR(_SOFTFLOOR *ps, _UWORD no)
{
  _UWORD i; 
  _BOOL retcode = _FALSE; 

  if(GetError() == _FALSE) 
  {
    retcode = _TRUE; 
    for(i=0; i<no; i++)
    {
      if(retcode == _TRUE)
        retcode = Write_SINGLESOFT(&ps->floor[i]); 
    }
  }

  return(retcode); 
}

_BOOL _File::Write_RECTANGLE(_RECTANGLE *p)
{
  if(GetError() == _FALSE) 
  {
    Write_CHKBIT(&p->chkbit); 
    Write_ATTRIB(&p->attrib);
    Write_COLOR(&p->color);
    Write_COLOR(&p->colorframe); 
    Write_ROOM(&p->room); 
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_CIRCLE(_CIRCLE *p)
{
  if(GetError() == _FALSE) 
  {
    Write_CHKBIT(&p->chkbit); 
    Write_ATTRIB(&p->attrib); 
    Write_COLOR(&p->color); 
    Write_ROOM(&p->room); 
    Write(&p->angle, sizeof(_WORD)); 
    Write(&p->arc, sizeof(_WORD)); 
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_POLYGON(_POLYGON *p)
{
  _UDWORD size; 

  if(GetError() == _FALSE) 
  {
    Write_CHKBIT(&p->chkbit); 
    Write_ATTRIB(&p->attrib); 
    Write_COLOR(&p->color); 
    if(Write(&p->no, sizeof(_UWORD)) == _TRUE)
    {
      size  = p->no * sizeof(_DOT); 
      Write(p->ptr, size);
    }
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_PICTEXT(_PICTEXT *p)
{
  if(GetError() == _FALSE) 
  {
    Write_CHKBIT(&p->chkbit);
    Write_ATTRIB(&p->attrib);
    Write_COLOR(&p->colback);
		Write_COLOR(&p->colframe);
		Write_COLOR(&p->coltxt);
    Write_ROOM(&p->room);
    Write_ANYTHING(&p->txt);
    Write(&p->font,   sizeof(_FONT));
		Write(&p->summary, sizeof(_UWORD));
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_ZOOM(_ZOOM *p)
{
  return(Write(p, sizeof(_ZOOM))); 
}

_BOOL _File::Write_OBJECT(_OBJECT *p)
{
  _COLOR color = MakeColor_Makro(INVISIBLE, INVISIBLE);

  if(GetError() == _FALSE)
  {
 		Write_CHKBIT(&p->chkbit);
		Write_ATTRIB(&p->attrib);
		Write(&p->xy, sizeof(_DOT));
		Write(&p->no, sizeof(_UWORD));
		Write_ZOOM(&p->zoom);
		Write_COLOR(&color);  // bitte old_color entfernen !!
		Write_COLOR(&p->colorframe);
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE);
}

_BOOL _File::Write_BITMAP(_BITMAP *p)
{
  if(GetError() == _FALSE) 
  {
		Write_CHKBIT(&p->chkbit);
		Write_ATTRIB(&p->attrib);
		Write_COLOR(&p->color);
		Write_COLOR(&p->colorframe);
		Write_ROOM(&p->room);
		Write(&p->no, sizeof(_UWORD));
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_ROOM(_ROOM *p)
{
  return(Write(p, sizeof(_ROOM))); 
} 

_BOOL _File::Write_INPUT(_INPUT *p)
{
  if(Write_IO(&p->io) == _TRUE) 
  {
    Write(&p->h_next, sizeof(_UWORD)); 
//    Write(&p->v_next, sizeof(_UWORD)); 
    Write(&p->enable, sizeof(_ENABLE)); 
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_IOINFO(_IOINFO *p, _IO_TYPE typ)
{
  _UWORD no = 6; 
  _UBYTE tmp; 

  if(GetError() == _FALSE)
  {
    switch(typ)
    {
      case AS_BAR   : tmp = p->tbo.no; 
                      Write(&tmp, sizeof(_UBYTE)); 
                      no -= 1; 
                      break; 
      case AS_TBO   : Write_ANYTHING(&p->tbo); 
                      no -= 4; 
                      break; 
      case AS_MENU  : Write_ANYTHING(&p->tbo); 
                      no -= 4; 
                      break;
    }

    while(no > 0) 
    {
      tmp = 0; 
      Write(&tmp, sizeof(_UBYTE)); 
      no --; 
    }
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_PICFORMAT(_PICFORMAT *p)
{
  if(GetError() == _FALSE) 
  {
    Write(&p->format, sizeof(_UWORD)); 
    Write(&p->digits, sizeof(_UBYTE)); 
    Write_VARLABEL(&p->posdezpt); 
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_IO(_IO *p)
{
  if(GetError() == _FALSE) 
  {
		Write_CHKBIT(&p->chkbit);
		Write_ATTRIB(&p->attrib);
		Write_COLOR(&p->colback);
		Write_COLOR(&p->colframe);
		Write_COLOR(&p->coltxt);
		Write_ROOM(&p->room);
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE);
}

_BOOL _File::Write_MOTION(_MOTION *p)
{
  if(GetError() == _FALSE)
  {
    Write_CHKBIT(&p->chkbit);
    Write_ROOM(&p->room);
    Write_ATTRIB(&p->attrib); 
    Write_ANYTHING(&p->anything);
    Write_VARLABEL(&p->varx); 
    Write_VARLABEL(&p->vary); 
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}


_BOOL _File::Write_PICTUREMASK(_PICTUREMASK *p)
{
  _UWORD  i; 

  if(GetError() == _FALSE) 
  {
    Write_ROOM(&p->room); 

    // write rectangles
    if(Write(&p->rect.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->rect.no; i++)
        Write_RECTANGLE(&p->rect.ptr[i]); 
    }

    // write polygons
    if(Write(&p->poly.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->poly.no; i++)
        Write_POLYGON(&p->poly.ptr[i]); 
    }

    // write circles
    if(Write(&p->circ.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->circ.no; i++)
        Write_CIRCLE(&p->circ.ptr[i]); 
    }

    // write picturetextpositions
    if(Write(&p->txt.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->txt.no; i++)
        Write_PICTEXT(&p->txt.ptr[i]); 
    }

    // write objects
    if(Write(&p->object.no, sizeof(_UWORD)) == _TRUE)
    { 
      for(i=0; i<p->object.no; i++)
        Write_OBJECT(&p->object.ptr[i]); 
    }

    // write bitmaps
    if(Write(&p->bitmap.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->bitmap.no; i++)
        Write_BITMAP(&p->bitmap.ptr[i]); 
    }

    // write inputs
    if(Write(&p->input.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->input.no; i++)
        Write_INPUT(&p->input.ptr[i]); 
    }

    // write outputs
    if(Write(&p->output.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->output.no; i++)
        Write_IO(&p->output.ptr[i]); 
    }

    // write motions
    if(Write(&p->motion.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->motion.no; i++)
        Write_MOTION(&p->motion.ptr[i]); 
    }

    // write coloured rectangles
    if(Write(&p->colrect.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->colrect.no; i++)
        Write_COL_RECT(&p->colrect.ptr[i]);
    }

    // write coloured polygons
    if(Write(&p->colpoly.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->colpoly.no; i++)
        Write_COL_POLY(&p->colpoly.ptr[i]);
    }

    // write coloured circles
    if(Write(&p->colcirc.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->colcirc.no; i++)
        Write_COL_CIRC(&p->colcirc.ptr[i]);
    }

    // write coloured picturetexts
    if(Write(&p->colpictxt.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->colpictxt.no; i++)
        Write_COL_PICTXT(&p->colpictxt.ptr[i]);
    }

    // write keyboard
    if(Write(&p->memkey.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->memkey.no; i++)
        Write_KEY(&p->memkey.ptr[i]);
    }

    // write touchbuttons
    if(Write(&p->touchbutton.no, sizeof(_UWORD)) == _TRUE)
    {
      for(i=0; i<p->touchbutton.no; i++)
        Write_TOUCH_BUTTON(&p->touchbutton.ptr[i]);
    }

   #ifdef UC_IPC
    // write mygfx
//    if(Write(&no, sizeof(_UWORD)) == _TRUE)
//    {
//      for(i=0; i<p->memchkbit.no; i++)
//        Write_MYGFX(&p->memgfx.ptr[i]);
//    }
   #endif
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_BMP(_BMP *p)
{
  _UDWORD bytes; 
  _UDWORD no_dat; 

  if(GetError() == _FALSE)
  {
    no_dat = p->datano; 
    if(Write(&no_dat, sizeof(_UDWORD)) == _TRUE) 
    {
      bytes = no_dat * sizeof(_BMPDAT); 
      Write(&p->dx, sizeof(_UWORD)); 
      Write(&p->dy, sizeof(_UWORD)); 
      Write(&p->transparent, sizeof(_UWORD)); 
      Write(&p->state, sizeof(_UWORD)); 
      Write(p->ptr, bytes); 
    }
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
} 

_BOOL _File::Write_COL_RECT(_COL_RECT *p)
{
  if(GetError() == _FALSE)
  {
    Write_CHKBIT(&p->rectangle.chkbit);
    Write_ATTRIB(&p->rectangle.attrib);
    Write_VARLABEL(&p->color);
    Write_COLOR(&p->rectangle.colorframe); 
    Write_ROOM(&p->rectangle.room);
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_COL_POLY(_COL_POLY *p)
{
  _UDWORD size;

  if(GetError() == _FALSE)
  {
    Write_CHKBIT(&p->poly.chkbit);
    Write_ATTRIB(&p->poly.attrib);
    Write_VARLABEL(&p->color);

    if(Write(&p->poly.no, sizeof(_UWORD)) == _TRUE)
    {
      size = p->poly.no * sizeof(_DOT);
      Write(p->poly.ptr, size);
    }
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_COL_CIRC(_COL_CIRC *p)
{
  if(GetError() == _FALSE)
  {
    Write_CHKBIT(&p->circle.chkbit);
    Write_ATTRIB(&p->circle.attrib);
    Write_VARLABEL(&p->color);
    Write_ROOM(&p->circle.room);
    Write(&p->circle.angle, sizeof(_WORD));
    Write(&p->circle.arc, sizeof(_WORD));
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}


_BOOL _File::Write_COL_PICTXT(_COL_PICTXT *p)
{
  if(GetError() == _FALSE)
  {
    Write_CHKBIT(&p->btx.chkbit);
    Write_ATTRIB(&p->btx.attrib);
    Write_VARLABEL(&p->color);
    Write_ROOM(&p->btx.room);
    Write_ANYTHING(&p->btx.txt);
    Write(&p->btx.font, sizeof(_FONT));
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_KEY(_KEY *p)
{
  if(GetError() == _FALSE)
  {
    Write(&p->code,            sizeof(_UWORD));
    Write_ENUM(p->tip); 
    Write(&p->gfunct.pos_fl,   sizeof(_UWORD));
    Write(&p->gfunct.neg_fl,   sizeof(_UWORD));
    Write(&p->gfunct.active,   sizeof(_UWORD));
    Write(&p->gfunct.inactive, sizeof(_UWORD));
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_TOUCH_BUTTON(_TOUCH_BUTTON *p)
{
  if(GetError() == _FALSE)
  {
    Write_CHKBIT(&p->chkbit);
    Write_ROOM(&p->room);
    Write_GFUNCT(&p->gfunct);
    Write_ANYTHING(&p->anything);
    Write_ATTRIB(&p->attrib);
    Write(&p->framecol, sizeof(_COLOR));
    Write(&p->col,      sizeof(_COLOR));
    Write(&p->textcol,  sizeof(_COLOR));
    Write(&p->font,     sizeof(_FONT));
    Write(&p->group,    sizeof(_UWORD));
    Write_ENUM(p->ftype);
    Write_ENUM(p->tip);
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

#ifdef UC_IPC
_BOOL _File::Write_DPNE(_DPNE *p)
{
  _UWORD tmp; 

  if(GetError() == _FALSE)
  {
    Write(&p->no, sizeof(_UWORD)); 
    Write(p->ptr, p->no); 
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_MYGFX(_MYGFX *p)
{
  if(GetError() == _FALSE)
  {
    Write_CHKBIT(&p->chkbit);
    Write_ROOM(&p->room); 
    Write_ATTRIB(&p->attrib);
    Write_DPNE(&p->dpne); 
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}
#endif

_BOOL _File::Write_VARSINGLE(_VARSINGLE *p)
{
  _DWORD tmp; 

  if(GetError() == _FALSE) 
  {
    Write_ENUM(p->state); 
    tmp = p->value; 
    Write(&tmp, sizeof(_DWORD)); 
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_VARLABEL(_VARLABEL *p)
{
  _UBYTE i, tmp; 

  if(GetError() == _FALSE) 
  {
    tmp = VL_MAXINDEX + 1; 
    Write(&tmp, sizeof(_UBYTE)); 
    for(i=0; i<tmp; i++)
      Write_VARSINGLE(&p->info[i]); 
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE); 
}

_BOOL _File::Write_PICTURE(_PICTURE *p)
{
  return(_FALSE); 
}


*/
#endif



