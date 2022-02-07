#include "T_IPC.H"
#include "T_NUC.H"

_BOOL copy_BMP(_BMP*, _BMP*);
_BOOL copy_PICTURE(_PICTURE*, _PICTURE*);
_BOOL copy_SCHEME(_SCHEME*, _SCHEME*);
_BOOL copy_STACK(_STACK*, _STACK*);
_BOOL copy_COMPCODE(_COMPCODE*, _COMPCODE*);
_BOOL copy_LSE_VARINFO(_BOOL*, _VARINFO*, _LSEVARINFO*, _BOOL);
_BOOL copy_SOFTFLOOR(_BOOL*, _SOFTFLOOR*, _SOFTFLOOR*);
_BOOL copy_TXTSCHEME(_BOOL*, _TXTSCHEME*, _TXTSCHEME*);
//_BOOL copy_UNIT(_BOOL*, _UNIT*, _UNIT*);
_BOOL copy_DOT(_DOT*, _DOT*); 
_BOOL copy_DOT3D(_DOT3D*, _DOT3D*);
_BOOL copy_SHAPE(_SHAPE*, _SHAPE*);
_BOOL copy_3DSHAPE(_3DSHAPE*, _3DSHAPE*);

void copy_VARLABEL(_VARLABEL *d, _VARLABEL *s)
{
  // falls funktion umgebaut wird, unbedingt funktion copy_LSE_VARINFO ändern
  //  init_VARLABEL(d);
  *d = *s;
}

void copy_ANYTHING(_ANYTHING *d, _ANYTHING *s)
{
  // falls funktion umgebaut wird, unbedingt funktion copy_LSE_VARINFO ändern
  //  init_ANYTHING_Makro(d);
  *d = *s;
}

_BOOL copy_ANYPARA(_BOOL *retcode, _ANYPARA *d, _ANYPARA *s)
{
  // falls funktion umgebaut wird, unbedingt funktion copy_LSE_VARINFO ändern
  if(*retcode == _TRUE)
  {
    copy_ANYTHING(&d->anything, &s->anything);
    if(copy_MEM_VARLABEL(&d->memvarlabel, &s->memvarlabel) == _FALSE)
      *retcode = _FALSE;
  }

  return(*retcode);
}

_BOOL copy_COMPCODE(_BOOL *retcode, _COMPCODE *d, _COMPCODE *s)
{
  init_COMPCODE(d);

  if(d->size == s->size)
  {
    MemCpy(d->ptr, s->ptr, d->size);
  }
  else
  {
    if(Memory2((void**)&d->ptr, s->size) != 0)
    {
      d->size = s->size;
      MemCpy(d->ptr, s->ptr, d->size);
    }
    else
    {
      *retcode = _FALSE;
    }
  }

  return(*retcode);
}

_BOOL copy_CHKBIT(_BOOL *retcode, _CHKBIT *d, _CHKBIT *s)
{
  if(*retcode == _TRUE)
  {
    d->seqno  = s->seqno;
    d->invert = s->invert;
    copy_COMPCODE(retcode, &d->overload, &s->overload);
  }

  return(*retcode);
}

void copy_MEM_SINGLESCHEME(_BOOL *retcode, _MEM_SINGLESCHEME *d, _MEM_SINGLESCHEME *s)
{
  _UDWORD size;

  init_MEM_SINGLESCHEME(d);

  if(s->no > 0)
  {
    size = s->no * sizeof(_SINGLESCHEME);

    if(Memory2((void**)&d->ptr, size) != 0)
    {
      MemCpy(d->ptr, s->ptr, size);
      d->no              = s->no;
      d->result_is_varno = s->result_is_varno;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void copy_SCHEME(_BOOL *retcode, _SCHEME *d, _SCHEME *s)
{
  init_SCHEME_Makro(d);

  d->link = s->link;
  copy_MEM_SINGLESCHEME(retcode, &d->memsinglescheme, &s->memsinglescheme);
}

void copy_MYSCHEME(_BOOL *retcode, _MYSCHEME *d, _MYSCHEME *s)
{
  init_MYSCHEME_Makro(d);

  d->schemeno = s->schemeno;
  copy_SCHEME(retcode, &d->overload, &s->overload);
}

_BOOL copy_SINGLETXTSCHEME(_BOOL *retcode, _SINGLETXTSCHEME *d, _SINGLETXTSCHEME *s)
{
  if(*retcode == _TRUE)
  {
    copy_VARLABEL(&d->upto, &s->upto);
    d->op = s->op;
    copy_ANYPARA(retcode, &d->anypara, &s->anypara);
  }

  return(*retcode);
}

_BOOL copy_TXTSCHEME(_BOOL *retcode, _TXTSCHEME *d, _TXTSCHEME *s)
{
  _UDWORD size;
  _UWORD  i;

  if(*retcode == _TRUE)
  {
    size = s->no * sizeof(_SINGLETXTSCHEME);

    if(s->no == d->no)
    {
      for(i=0; i<s->no; i++)
        copy_SINGLETXTSCHEME(retcode, &d->ptr[i], &s->ptr[i]);
    }
    else
    {
      if(s->no < d->no)
        for(i=s->no; i<d->no; i++)
          free_SINGLETXTSCHEME(&d->ptr[i]);

      if(Memory2((void**)&d->ptr, size) != 0)
      {
        if(s->no > d->no)
          for(i=d->no; i<s->no; i++)
            init_SINGLETXTSCHEME(&d->ptr[i]);

        d->no = s->no; 
        for(i=0; i<s->no; i++)
          copy_SINGLETXTSCHEME(retcode, &d->ptr[i], &s->ptr[i]);
      }
      else
      {
        *retcode = _FALSE;
      }
    }
  }

  return(*retcode);
}

_BOOL copy_MYTXTSCHEME(_BOOL *retcode, _MYTXTSCHEME *d, _MYTXTSCHEME *s)
{
  if(*retcode == _TRUE)
  {
    d->schemeno = s->schemeno;
    copy_TXTSCHEME(retcode, &d->overload, &s->overload);
    copy_ANYPARA(retcode, &d->anypara, &s->anypara);
  }

  return(*retcode);
}

void copy_LINKSCHEME(_BOOL *retcode, _LINKSCHEME *d, _LINKSCHEME *s)
{
  init_LINKSCHEME_Makro(d);

  copy_VARLABEL(&d->varlabel, &s->varlabel);
  copy_MYSCHEME(retcode, &d->myscheme, &s->myscheme);
}

void copy_DPNE(_BOOL *retcode, _DPNE *d, _DPNE *s)
{
  init_DPNE(d);

  if(Memory2((void**)&d->ptr, s->no) != 0)
  {
    MemCpy(d->ptr, s->ptr, s->no);
    d->no = s->no;
  }
  else
  {
    *retcode = _FALSE;
  }
}

void copy_STACK_LINE(_STACK_LINE *d, _STACK_LINE *s)
{
  *d = *s;
}

_BOOL copy_STACK(_BOOL *retcode, _STACK *d, _STACK *s)
{
  _UWORD  i;
  _UDWORD size = s->no * sizeof(_STACK_LINE);

  init_STACK_Makro(d);

  if(*retcode == _TRUE)
  {
    if(Memory2((void**)&d->ptr, size) != 0)
    {
      for(i=0; i<s->no; i++)
        copy_STACK_LINE(&d->ptr[i], &s->ptr[i]);
      d->no = s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }

  return(*retcode);
}

_BOOL merge_STACK(_STACK *d, _STACK *s)
{
  _UWORD  i;
  _BOOL   retcode = _FALSE;
  _UDWORD size = (d->no + s->no) * sizeof(_STACK_LINE);

  if(Memory2((void**)&d->ptr, size) != 0)
  {
    for(i=0; i<s->no; i++)
      copy_STACK_LINE(&d->ptr[d->no+i], &s->ptr[i]);

    d->no  += s->no;
    retcode = _TRUE;
  }

  return(retcode);
}


void copy_LSEFUNCT(_BOOL *retcode, _LSEFUNCT *d, _LSEFUNCT *s)
{
  //init_LSEFUNCT_Makro(d);

  *d = *s;
  copy_STACK(retcode, &d->overload, &s->overload);
}

void copy_GFUNCT(_BOOL *retcode, _GFUNCT *d, _GFUNCT *s)
{
  init_GFUNCT_Makro(d);

  copy_LSEFUNCT(retcode, &d->pos_fl, &s->pos_fl);
  copy_LSEFUNCT(retcode, &d->neg_fl, &s->neg_fl);
}

void copy_MEM_LSEFUNCT(_BOOL *retcode, _MEM_LSEFUNCT *d, _MEM_LSEFUNCT *s)
{
  _UWORD  i;
  _UDWORD size;

  init_MEM_LSEFUNCT(d);

  if(s->no > 0)
  {
    size = s->no * sizeof(_LSEFUNCT);
    if(Memory2((void**)&d->ptr, size) != 0)
    {
      d->no = s->no;
      for(i=0; i<s->no; i++)
      {
        copy_LSEFUNCT(retcode, &d->ptr[i], &s->ptr[i]);
      }
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

_BOOL copy_IOINFO(_BOOL *retcode, _IOINFO *d, _IOINFO *s)
{
  if(*retcode == _TRUE)
  {
    copy_ANYTHING(&d->tbo, &s->tbo);
    copy_MYSCHEME(retcode, &d->myscheme, &s->myscheme);
    copy_MYTXTSCHEME(retcode, &d->mytxtscheme, &s->mytxtscheme);
    d->pcombo    = s->pcombo;
    d->combocopy = (d->pcombo != NULL)? _TRUE : _FALSE;
  }
  return(*retcode);
}

void copy_DIRECTION(_DIRECTION *d, _DIRECTION *s)
{
  *d = *s;
}

void copy_THEURGE(_BOOL *retcode, _THEURGE *d, _THEURGE *s)
{
  if((d != NULL)&&(s != NULL))
  {
    *d = *s;

    init_MEM_FONT(&d->memfont);
    init_MEM_ATTRIB(&d->memattrib);
    init_MEM_COLOR(&d->memcolor);
    init_MEM_VARLABEL(&d->memvarlabel);
    init_MEM_ANYTHING(&d->memanything);
    init_MEM_DYNCOLOR(&d->memdyncolor);
    init_MEM_LSEFUNCT(&d->memlsefunct);
    init_MEM_ANYPARA(&d->memanypara);
    init_MEM_BOOL(&d->membool);
    init_MEM_ANYTHING(&d->memimage);

    d->ptr = NULL;

    copy_MEM_FONT(&d->memfont, &s->memfont);
    copy_MEM_ATTRIB(&d->memattrib, &s->memattrib);
    copy_MEM_COLOR(retcode, &d->memcolor, &s->memcolor);
    copy_MEM_VARLABEL(&d->memvarlabel, &s->memvarlabel);
    copy_MEM_ANYTHING(retcode, &d->memanything, &s->memanything);
    copy_MEM_DYNCOLOR(retcode, &d->memdyncolor, &s->memdyncolor);
    copy_MEM_LSEFUNCT(retcode, &d->memlsefunct, &s->memlsefunct);
    copy_MEM_ANYPARA(retcode, &d->memanypara, &s->memanypara);
    copy_MEM_BOOL(&d->membool, &s->membool);
    copy_MEM_ANYTHING(retcode, &d->memimage, &s->memimage);
  }
}

void copy_IO(_BOOL *retcode, _IO *d, _IO *s)
{
//  init_IO(d);

  *d = *s;
  init_CHKBIT_Makro(&d->chkbit);
  copy_CHKBIT(retcode, &d->chkbit, &s->chkbit);

  init_CSTATE(&d->cstate);
  init_ROOM_Makro(&d->space);
  init_IMAGE_Makro(&d->image);

  init_LINKSCHEME_Makro(&d->dyncolback);
  init_LINKSCHEME_Makro(&d->dyncolfront);
  init_LINKSCHEME_Makro(&d->dyncoltl);
  init_LINKSCHEME_Makro(&d->dyncolbr);
  init_LINKSCHEME_Makro(&d->dyncoltxt);

  d->theurge = NULL;

  if(s->is_dyncolor == _TRUE)
  {
    copy_LINKSCHEME(retcode, &d->dyncolback, &s->dyncolback);
    copy_LINKSCHEME(retcode, &d->dyncolfront, &s->dyncolfront);
    copy_LINKSCHEME(retcode, &d->dyncoltl, &s->dyncoltl);
    copy_LINKSCHEME(retcode, &d->dyncolbr, &s->dyncolbr);
    copy_LINKSCHEME(retcode, &d->dyncoltxt, &s->dyncoltxt);
  }

//  init_DIRECTION(&d->direction);
  copy_DIRECTION(&d->direction, &s->direction);

//  init_MYSCHEME(&d->stscheme);
  copy_MYSCHEME(retcode, &d->stscheme, &s->stscheme);

  init_LSEFUNCT_Makro(&d->lsefunct);
  copy_LSEFUNCT(retcode, &d->lsefunct, &s->lsefunct);

  init_IOINFO_Makro(&d->info);
  copy_IOINFO(retcode, &d->info, &s->info);

  init_ANYPARA(&d->newbubbletext);
  copy_ANYPARA(retcode, &d->newbubbletext, &s->newbubbletext);

  if(s->typ == AS_THEURGE)
  {
    if(Memory2((void**)&d->theurge, sizeof(_THEURGE)) != 0)
      copy_THEURGE(retcode, d->theurge, s->theurge);
  }
 #ifdef ZOOM_KM
  d->kmz = s->kmz;
 #endif
 #ifdef UC_MENU_TWOCOLOR
  d->refstschemestate = s->refstschemestate;
 #endif
}

void copy_RECTANGLE(_BOOL *retcode, _RECTANGLE *d, _RECTANGLE *s)
{
  // init_RECTANGLE(d);

  *d = *s;
  init_CHKBIT_Makro(&d->chkbit);
  copy_CHKBIT(retcode, &d->chkbit, &s->chkbit);
}

void copy_POLYGON(_BOOL *retcode, _POLYGON *d, _POLYGON *s)
{
  _UDWORD size = s->no * sizeof(_DOT);

  *d     = *s;
  d->ptr = NULL;

  if(Memory2((void**)&d->ptr, size) != 0)
  {
    MemCpy(d->ptr, s->ptr, size);
  }
  else
  {
    *retcode = _FALSE;
  }
}

void copy_CIRCLE(_BOOL *retcode, _CIRCLE *d, _CIRCLE *s)
{
  // init_CIRCLE(d);

  *d = *s;
  init_CHKBIT_Makro(&d->chkbit);
  copy_CHKBIT(retcode, &d->chkbit, &s->chkbit);
}

void copy_USERTEXT(_BOOL *retcode, _USERTEXT *d, _USERTEXT *s)
{
  if(s->ptr != NULL)
  {
    d->ptr = NULL;
    _UWORD len = (StrLen(s->ptr, sizeof(_CHAR)) + 1) * sizeof(_CHAR);
    if(Memory2((void**)&d->ptr, len) != 0)
      MemCpy(d->ptr, s->ptr, len);
    else
      *retcode = _FALSE;
  }
}

void copy_PICTEXT(_BOOL *retcode, _PICTEXT *d, _PICTEXT *s, _UWORD txo)
{
  //init_PICTEXT(d);

  *d = *s;
  init_ANYPARA_Makro(&d->anypara);
  init_MYSCHEME_Makro(&d->stscheme);
  init_IMAGE_Makro(&d->image);
  init_USERTEXT_Makro(&d->usertxt);

  init_CHKBIT_Makro(&d->chkbit);
  copy_CHKBIT(retcode, &d->chkbit, &s->chkbit);
  copy_ANYPARA(retcode, &d->anypara, &s->anypara);
  copy_MYSCHEME(retcode, &d->stscheme, &s->stscheme);
  copy_USERTEXT(retcode, &d->usertxt, &s->usertxt);

  if(txo != 0)
    if(d->anypara.anything.list == LOCTXT)
      d->anypara.anything.no += txo;
}

void copy_OBJECT(_BOOL *retcode, _OBJECT *d, _OBJECT *s)
{
  // init_OBJECT(d);

  *d = *s;
  d->SubPicture = NULL;

  init_IMAGE_Makro(&d->image);
  init_VARLABEL_Makro(&d->loader);
  init_WORKSTATE(&d->workstate);

  init_MYSCHEME_Makro(&d->stscheme);
  copy_MYSCHEME(retcode, &d->stscheme, &s->stscheme);

  init_CHKBIT_Makro(&d->chkbit);
  copy_CHKBIT(retcode, &d->chkbit, &s->chkbit);
  copy_VARLABEL(&d->loader, &s->loader);
}

void copy_BITMAP(_BOOL *retcode, _BITMAP *d, _BITMAP *s)
{
  //init_BITMAP(d);

  *d = *s;
  init_CHKBIT_Makro(&d->chkbit);
  copy_CHKBIT(retcode, &d->chkbit, &s->chkbit);
}

void copy_INPUT(_BOOL *retcode, _INPUT *d, _INPUT *s)
{
  // init_INPUT(d);
  *d = *s;
  init_IO_Makro(&d->io);
  copy_IO(retcode, &d->io, &s->io);
}

void copy_KARU(_KARU *d, _KARU *s)
{
  *d = *s;
  d->SubPicture = NULL;
  
  init_VARLABEL_Makro(&d->loader);
  copy_VARLABEL(&d->loader, &s->loader);
}

void copy_MOTION(_BOOL *retcode, _MOTION *d, _MOTION *s)
{
//  init_MOTION(d);

  *d = *s;
  init_IMAGE_Makro(&d->image);
  init_WORKSTATE(&d->workstate);
  init_MEM_KARU(&d->memkaru);

  d->SubPicture = NULL;

  init_CHKBIT_Makro(&d->chkbit);
  copy_CHKBIT(retcode, &d->chkbit, &s->chkbit);

  init_MYSCHEME_Makro(&d->stscheme);
  copy_MYSCHEME(retcode, &d->stscheme, &s->stscheme);

  copy_ANYTHING(&d->anything, &s->anything);
  copy_VARLABEL(&d->varx, &s->varx);
  copy_VARLABEL(&d->vary, &s->vary);
//  copy_MEM_KARU(&d->memkaru, &s->memkaru);
}

void copy_COL_RECT(_BOOL *retcode, _COL_RECT *d, _COL_RECT *s)
{
  init_COL_RECT(d);

  copy_RECTANGLE(retcode, &d->rectangle, &s->rectangle);
  copy_LINKSCHEME(retcode, &d->dyncolback, &s->dyncolback);
  copy_LINKSCHEME(retcode, &d->dyncolfront, &s->dyncolfront);
  copy_LINKSCHEME(retcode, &d->dyncoltl, &s->dyncoltl);
  copy_LINKSCHEME(retcode, &d->dyncolbr, &s->dyncolbr);

  d->init = _TRUE;
}

void copy_COL_POLY(_BOOL *retcode, _COL_POLY *d, _COL_POLY *s)
{
  init_COL_POLY(d);

  copy_POLYGON(retcode, &d->poly, &s->poly);
  copy_LINKSCHEME(retcode, &d->dyncolback, &s->dyncolback);
  copy_LINKSCHEME(retcode, &d->dyncolfront, &s->dyncolfront);
  copy_LINKSCHEME(retcode, &d->dyncolframe, &s->dyncolframe);

  d->init = _TRUE;
}

void copy_COL_CIRC(_BOOL *retcode, _COL_CIRC *d, _COL_CIRC *s)
{
  init_COL_CIRC(d);

  copy_CIRCLE(retcode, &d->circle, &s->circle);
  copy_LINKSCHEME(retcode, &d->dyncolback, &s->dyncolback);
  copy_LINKSCHEME(retcode, &d->dyncolfront, &s->dyncolfront);
  copy_LINKSCHEME(retcode, &d->dyncolframe, &s->dyncolframe);

  d->init  = _TRUE;
}

void copy_COL_PICTXT(_BOOL *retcode, _COL_PICTXT *d, _COL_PICTXT *s, _UWORD txo)
{
  init_COL_PICTXT(d);

  copy_PICTEXT(retcode, &d->btx, &s->btx, txo);
  copy_LINKSCHEME(retcode, &d->dyncolback, &s->dyncolback);
  copy_LINKSCHEME(retcode, &d->dyncolfront, &s->dyncolfront);
  copy_LINKSCHEME(retcode, &d->dyncoltl, &s->dyncoltl);
  copy_LINKSCHEME(retcode, &d->dyncolbr, &s->dyncolbr);
  copy_LINKSCHEME(retcode, &d->dyncoltxt, &s->dyncoltxt);

  d->init = _TRUE;
}

void copy_KEY(_BOOL *retcode, _KEY *d, _KEY *s)
{
  // init_KEY(d);

  *d = *s;
  copy_GFUNCT(retcode, &d->gfunct, &s->gfunct);
}

void copy_BUTTON(_BOOL *retcode, _BUTTON *d, _BUTTON *s)
{
  // init_BUTTON(d)

  *d = *s;
  init_CHKBIT_Makro(&d->chkbit);
  copy_CHKBIT(retcode, &d->chkbit, &s->chkbit);

  init_GFUNCT_Makro(&d->gfunct);
  copy_GFUNCT(retcode, &d->gfunct, &s->gfunct);

  init_ANYTHING_Makro(&d->anything);
  copy_ANYTHING(&d->anything, &s->anything);

  init_VARLABEL_Makro(&d->loader);
  copy_VARLABEL(&d->loader, &s->loader);

  init_DIRECTION(&d->direction);
  copy_DIRECTION(&d->direction, &s->direction);

  init_LINKSCHEME_Makro(&d->dyncolback);
  copy_LINKSCHEME(retcode, &d->dyncolback, &s->dyncolback);

  init_LINKSCHEME_Makro(&d->dyncolfront);
  copy_LINKSCHEME(retcode, &d->dyncolfront, &s->dyncolfront);

  init_LINKSCHEME_Makro(&d->dyncoltl);
  copy_LINKSCHEME(retcode, &d->dyncoltl, &s->dyncoltl);

  init_LINKSCHEME_Makro(&d->dyncolbr);
  copy_LINKSCHEME(retcode, &d->dyncolbr, &s->dyncolbr);

  init_MYSCHEME_Makro(&d->stscheme);
  copy_MYSCHEME(retcode, &d->stscheme, &s->stscheme);

  init_VARLABEL_Makro(&d->varlabel);
  copy_VARLABEL(&d->varlabel, &s->varlabel);

  init_ANYPARA_Makro(&d->text);
  copy_ANYPARA(retcode, &d->text, &s->text);

  init_ANYPARA_Makro(&d->text1);
  copy_ANYPARA(retcode, &d->text1, &s->text1);

  init_WORKSTATE(&d->workstate);

  init_USERTEXT_Makro(&d->usertext);
  copy_USERTEXT(retcode, &d->usertext, &s->usertext);
  
  init_USERTEXT_Makro(&d->usertext1);
  copy_USERTEXT(retcode, &d->usertext1, &s->usertext1);

  init_ANYPARA_Makro(&d->newbubbletext);
  copy_ANYPARA(retcode, &d->newbubbletext, &s->newbubbletext);

  d->SubPicture = NULL;
  d->selected = _FALSE;
  d->fingercount = s->fingercount;
}

void merge_MEM_ZORDER(_BOOL *retcode, _MEM_ZORDER *d, _MEM_ZORDER *s, _PICTUREMASK *dpm)
{
  // this function has to be called before all other elements are merged
  _UDWORD size = (d->no + s->no) * sizeof(_ZORDER);
  _ZORDER *p;
  _UWORD  i;

  if(Memory2((void**)&d->ptr, size) != 0)
  {
    if(d->no == 0)
    {
      MemCpy(d->ptr, s->ptr, size);
      d->no = s->no;
    }
    else
    {
      for(i=0; i<s->no; i++)
      {
        p = &d->ptr[d->no];
        d->no ++;
        init_ZORDER(p);
        *p = s->ptr[i];

        switch(p->ftype)
        {
          case ZO_RECTANGLE    : p->no += dpm->memrectangle.no; break;
          case ZO_TEXT         : p->no += dpm->mempictxt.no;    break;
          case ZO_CIRCLE       : p->no += dpm->memcircle.no;    break;
          case ZO_POLYGON      : p->no += dpm->mempolygon.no;   break;
          case ZO_IMAGE        : p->no += dpm->membitmap.no;    break;
          case ZO_OBJECT       : p->no += dpm->memobject.no;    break;
          case ZO_INPUT        : p->no += dpm->meminput.no;     break;
          case ZO_OUTPUT       : p->no += dpm->memoutput.no;    break;
          case ZO_BUTTON       : p->no += dpm->membutton.no;    break;
          case ZO_COLRECTANGLE : p->no += dpm->colrect.no;      break;
          case ZO_COLTEXT      : p->no += dpm->colpictxt.no;    break;
          case ZO_COLCIRCLE    : p->no += dpm->colcirc.no;      break;
          case ZO_COLPOLYGON   : p->no += dpm->colpoly.no;      break;
          case ZO_MOTION       : p->no += dpm->memmotion.no;    break;
          //case ZO_CAROUSEL     : p->no += dpm->memcarousel.no;  break;
        }
      }
    }
  }
  else
  {
    *retcode = _FALSE;
  }
}

void merge_ASCII_BUFFER(_BOOL *retcode, _ASCII_BUFFER *d, _ASCII_BUFFER *s)
{
  _UDWORD size;

  size = s->size + d->size;

	if(Memory2((void**)&d->ptr, size) != 0)
	{
		MemCpy(&d->ptr[d->size], s->ptr, s->size);
		d->no   += s->no;
    d->size  = size;
    d->space = size;
	}
  else
  {
    *retcode = _FALSE;
  }
}

void merge_MEM_RECTANGLE(_BOOL *retcode, _MEM_RECTANGLE *d, _MEM_RECTANGLE *s)
{
	_UWORD     i;
  _RECTANGLE *p, *pr;
  _UDWORD    size;

	if(s->no > 0)
	{
    size = (d->no+s->no) * sizeof(_RECTANGLE);
		if(Memory2((void**)&d->ptr, size) != 0)
		{
      if(d->no == 0)
      {
        MemCpy(d->ptr, s->ptr, size);
        d->no = s->no;
      }
      else
      {
        p = &d->ptr[d->no];
        for(i=0, pr=s->ptr; i<s->no; i++, pr++)
        {
          copy_RECTANGLE(retcode, p, pr);
          p += 1;
        }

			  d->no  += s->no;
      }
		}
    else
    {
      *retcode = _FALSE;
    }
	}
}

void merge_MEM_POLYGON(_BOOL *retcode, _MEM_POLYGON *d, _MEM_POLYGON *s)
{
  _UWORD   i;
  _POLYGON *p, *pp;

  if(s->no > 0)
  {
    if(Memory2((void**)&d->ptr, (d->no + s->no) * sizeof(_POLYGON)) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, pp=s->ptr; i<s->no; i++, pp++)
      {
        copy_POLYGON(retcode, p, pp);
        p += 1;
      }

      d->no += s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void merge_MEM_CIRCLE(_BOOL *retcode, _MEM_CIRCLE *d, _MEM_CIRCLE *s)
{
  _UWORD  i;
  _CIRCLE *p, *pc;
  _UDWORD size;

  if(s->no > 0)
  {
    size = (d->no + s->no) * sizeof(_CIRCLE);
    if(Memory2((void**)&d->ptr, size) != 0)
    {
      if(d->no == 0)
      {
        MemCpy(d->ptr, s->ptr, size);
        d->no = s->no;
      }
      else
      {
        p = &d->ptr[d->no];
        for(i=0, pc=s->ptr; i<s->no; i++, pc++)
        {
          copy_CIRCLE(retcode, p, pc);
          p += 1;
        }

        d->no  += s->no;
      }
    }
    else
    {
      *retcode = _FALSE;
    }
  }

}

void merge_MEM_PICTEXT(_BOOL *retcode, _MEM_PICTEXT *d, _MEM_PICTEXT *s, _UWORD txo)
{
  _UWORD   i;
  _PICTEXT *p, *pt;
  _UDWORD  size;

  if(s->no > 0)
  {
    size = (d->no + s->no) * sizeof(_PICTEXT);
    if(Memory2((void**)&d->ptr, size) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, pt=s->ptr; i<s->no; i++, pt++)
      {
        copy_PICTEXT(retcode, p, pt, txo);
        p += 1;
      }
      d->no += s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void merge_MEM_OBJECT(_BOOL *retcode, _MEM_OBJECT *d, _MEM_OBJECT *s)
{
  _UWORD  i;
  _OBJECT *p, *po;

  if(s->no > 0)
  {
    if(Memory2((void**)&d->ptr, (d->no + s->no) * sizeof(_OBJECT)) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, po=s->ptr; i<s->no; i++, po++)
      {
        copy_OBJECT(retcode, p, po);
        p += 1;
      }

      d->no  += s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void merge_MEM_BITMAP(_BOOL *retcode, _MEM_BITMAP *d, _MEM_BITMAP *s)
{
  _UWORD  i;
  _BITMAP *p, *pb;
  _UDWORD size; 

  if(s->no)
  {
    size = (d->no + s->no) * sizeof(_BITMAP);
    if(Memory2((void**)&d->ptr, size) != 0)
    {
      if(d->no == 0)
      {
        MemCpy(d->ptr, s->ptr, size);
        d->no = s->no; 
      }
      else
      {
        p = &d->ptr[d->no];
        for(i=0, pb=s->ptr; i<s->no; i++, pb++)
        {
          copy_BITMAP(retcode, p, pb);
          p += 1;
        }
        d->no  += s->no;
      }
    }
    else
    {
      *retcode = _TRUE;
    }
  }
}

void merge_MEM_INPUT(_BOOL *retcode, _MEM_INPUT *d, _MEM_INPUT *s)
{
  _UWORD  i;
  _INPUT *p, *pi;

  if(s->no > 0)
  {
    if(Memory2((void**)&d->ptr, (d->no + s->no) * sizeof(_INPUT)) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, pi=s->ptr; i<s->no; i++, pi++)
      {
        copy_INPUT(retcode, p, pi);
        p += 1;
      }
      d->no += s->no;
    }
    else
    {
      *retcode = _TRUE;
    }
  }
}

void merge_MEM_OUTPUT(_BOOL *retcode, _MEM_OUTPUT *d, _MEM_OUTPUT *s)
{
  _UWORD i;
  _IO    *p, *po;

  if(s->no > 0)
  {
    if(Memory2((void**)&d->ptr, (d->no + s->no) * sizeof(_IO)) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, po=s->ptr; i<s->no; i++, po++)
      {
        copy_IO(retcode, p, po);
        p += 1;
      }
      d->no += s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void merge_MEM_MOTION(_BOOL *retcode, _MEM_MOTION *d, _MEM_MOTION *s)
{
  _UWORD  i;
  _MOTION *p, *pm;

  if(s->no > 0)
  {
    if(Memory2((void**)&d->ptr, (d->no + s->no) * sizeof(_MOTION)) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, pm=s->ptr; i<s->no; i++, pm++)
      {
        copy_MOTION(retcode, p, pm);
        p += 1;
      }
      d->no += s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void merge_MEM_COL_RECT(_BOOL *retcode, _MEM_COL_RECT *d, _MEM_COL_RECT *s)
{
  _UWORD    i;
  _COL_RECT *p, *pcr;

  if(s->no > 0)
  {
    if(Memory2((void**)&d->ptr, (d->no + s->no) * sizeof(_COL_RECT)) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, pcr=s->ptr; i<s->no; i++, pcr++)
      {
        copy_COL_RECT(retcode, p, pcr);
        p += 1;
      }
      d->no += s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void merge_MEM_COL_POLY(_BOOL *retcode, _MEM_COL_POLY *d, _MEM_COL_POLY *s)
{
  _UWORD    i;
  _COL_POLY *p, *pcp;

  if(s->no > 0)
  {
    if(Memory2((void**)&d->ptr, (d->no + s->no) * sizeof(_COL_POLY)) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, pcp=s->ptr; i<s->no; i++, pcp++)
      {
        copy_COL_POLY(retcode, p, pcp);
        p += 1;
      }
      d->no += s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void merge_MEM_COL_CIRC(_BOOL *retcode, _MEM_COL_CIRC *d, _MEM_COL_CIRC *s)
{
  _UWORD    i;
  _COL_CIRC *p, *pcc;

  if(s->no > 0)
  {
    if(Memory2((void**)&d->ptr, (d->no + s->no) * sizeof(_COL_CIRC)) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, pcc=s->ptr; i<s->no; i++, pcc++)
      {
        copy_COL_CIRC(retcode, p, pcc);
        p += 1;
      }
      d->no += s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void merge_MEM_COL_PICTXT(_BOOL *retcode, _MEM_COL_PICTXT *d, _MEM_COL_PICTXT *s, _UWORD txo)
{
  _UWORD      i;
  _COL_PICTXT *p, *pct;

  if(s->no > 0)
  {
    if(Memory2((void**)&d->ptr, (d->no + s->no) * sizeof(_COL_PICTXT)) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, pct=s->ptr; i<s->no; i++, pct++)
      {
        copy_COL_PICTXT(retcode, p, pct, txo);
        p += 1;
      }
      d->no += s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void merge_MEM_KEY(_BOOL *retcode, _MEM_KEY *d, _MEM_KEY *s)
{
  _UWORD i;
  _KEY   *p, *pk;

  if(s->no > 0)
  {
    if(Memory2((void**)&d->ptr, (d->no + s->no) * sizeof(_KEY)) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, pk=s->ptr; i<s->no; i++, pk++)
      {
        copy_KEY(retcode, p, pk);
        p += 1;
      }
      d->no  += s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void merge_MEM_BUTTON(_BOOL *retcode, _MEM_BUTTON *d, _MEM_BUTTON *s)
{
	_UWORD  i;
  _BUTTON *p, *pb;

	if(s->no > 0)
	{
    if(Memory2((void**)&d->ptr, (d->no + s->no) * sizeof(_BUTTON)) != 0)
    {
      p = &d->ptr[d->no];
      for(i=0, pb=s->ptr; i<s->no; i++, pb++)
      {
        copy_BUTTON(retcode, p, pb);
        p += 1;
      }
      d->no += s->no;
    }
    else
    {
      *retcode = _FALSE;
    }
  }
}

void copy_FAST_ASCII_BUFFER(_BOOL *retcode, _FAST_ASCII_BUFFER *d, _FAST_ASCII_BUFFER *s)
{
  _UDWORD size;

  init_FAST_ASCII_BUFFER(d);

  if(*retcode == _TRUE)
  {
    merge_ASCII_BUFFER(retcode, &d->ascii_buffer, &s->ascii_buffer);
    if(*retcode == _TRUE)
    {
      if(s->poffset != NULL)
      {
        size = d->ascii_buffer.no * sizeof(_UDWORD);
        if(Memory2((void**)&d->poffset, size) != 0)
          MemCpy(d->poffset, s->poffset, size);
      }
    }
  }
}

_BOOL copy_PICTUREMASK(_PICTUREMASK *d, _PICTUREMASK *s, _UWORD txo)
{
	_BOOL  retcode = _TRUE;

	d->room  = s->room;
	d->color = s->color;

  // note: z-order has to be copied (merged) at first
  merge_MEM_ZORDER(&retcode, &d->memzorder, &s->memzorder, d);
  if(s->memrectangle.no != 0)
	  merge_MEM_RECTANGLE(&retcode, &d->memrectangle, &s->memrectangle);
  if(s->mempolygon.no != 0)
	  merge_MEM_POLYGON(&retcode, &d->mempolygon, &s->mempolygon);
  if(s->memcircle.no != 0)
	  merge_MEM_CIRCLE(&retcode, &d->memcircle, &s->memcircle);
  if(s->mempictxt.no != 0)
	  merge_MEM_PICTEXT(&retcode, &d->mempictxt, &s->mempictxt, txo);
  if(s->memobject.no != 0)
	  merge_MEM_OBJECT(&retcode, &d->memobject, &s->memobject);
  if(s->membitmap.no != 0)
	  merge_MEM_BITMAP(&retcode, &d->membitmap, &s->membitmap);
  if(s->meminput.no != 0)
	  merge_MEM_INPUT(&retcode, &d->meminput, &s->meminput);
  if(s->memoutput.no != 0)
	  merge_MEM_OUTPUT(&retcode, &d->memoutput, &s->memoutput);
  if(s->memmotion.no != 0)
	  merge_MEM_MOTION(&retcode, &d->memmotion, &s->memmotion);
  if(s->colrect.no != 0)
	  merge_MEM_COL_RECT(&retcode, &d->colrect, &s->colrect);
  if(s->colpoly.no != 0)
	  merge_MEM_COL_POLY(&retcode, &d->colpoly, &s->colpoly);
  if(s->colcirc.no != 0)
	  merge_MEM_COL_CIRC(&retcode, &d->colcirc, &s->colcirc);
  if(s->colpictxt.no != 0)
	  merge_MEM_COL_PICTXT(&retcode, &d->colpictxt, &s->colpictxt, txo);
  if(s->memkey.no != 0)
	  merge_MEM_KEY(&retcode, &d->memkey, &s->memkey);
  if(s->membutton.no != 0)
	  merge_MEM_BUTTON(&retcode, &d->membutton, &s->membutton);
 #ifdef OK_ENCLOSE_NAME
//	merge_ASCII_BUFFER(&retcode, &d->enclosed_name, &s->enclosed_name);
  copy_FAST_ASCII_BUFFER(&retcode, &d->enclosed_name, &s->enclosed_name);
 #endif

	return(retcode);
}

_BOOL copy_SCHEME(_SCHEME *d, _SCHEME *s)
{
  _BOOL retcode = _TRUE;
  copy_SCHEME(&retcode, d, s);
  return(retcode);
}

_BOOL copy_STACK(_STACK *d, _STACK *s)
{
  _BOOL retcode = _TRUE;
  return(copy_STACK(&retcode, d, s));
}


// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************

_BOOL copy_PICTURE(_PICTURE *d, _PICTURE *s)
{
  _BOOL retcode = _FALSE;

  if(copy_PICTUREMASK(&d->mask, &s->mask, d->TextPuffer.GetNo()) == _TRUE)
  {
    if(d->TextPuffer.Merge(&s->TextPuffer) == _TRUE)
    {
      init_SUBACTUAL(&d->actual);
      retcode = _TRUE;
    }
  }

  return(retcode);
}

_BOOL load_object(_SubPicture **p, _UWORD no)
{
  _SubPicture *obj;
  _BOOL       retcode = _FALSE;

  if(*p == NULL)
  {
    if(Memory2((void**)p, sizeof(_SubPicture)) != 0)
    {
      (*p)->Init();
      retcode = _TRUE;
    }
  }
  else
  {
    (*p)->Free();
    retcode = _TRUE;
  }

  if(retcode == _TRUE)
  {
    obj = ObjectList.Get(no);
    if(obj != NULL)
    {
      if(obj->ChkChkBit() == _TRUE) // chkbit wird erst seit sa24986, dr1922 überprüft
      {
        return (*p)->Copy(obj);
      }
    }
    
    (*p)->Free();
    Memory2((void**)p, 0);
    retcode = _FALSE;
  }

  return(retcode);
}

_BOOL copy_BMP(_BMP *dst, _BMP *src)
{
  _BOOL   retcode = _FALSE;
  _UDWORD size;
  _BMP    tmp;

  if((src != NULL)&&(dst != NULL))
  {
    init_BMP_Makro(&tmp);
    tmp     = *src;
    tmp.ptr = NULL;
    size    = src->datano * sizeof(_BMPDAT);
    if(Memory2((void**)&tmp.ptr, size) != 0)
    {
      retcode = _TRUE;
      MemCpy(tmp.ptr, src->ptr, size);
      free_BMP(dst);
      *dst = tmp;
    }
  }

  return(retcode);
}


void copy_INPUT_BUTTON(_INPUT *pi, _BUTTON *pt)
{
  init_INPUT_Makro(pi);

  copy_DIRECTION(&pi->io.direction, &pt->direction);

  pi->enter_plus       = pt->enter_plus;
  pi->io.summary       = pt->summary;
//  pi->io.chkbit        = pt->chkbit;
  init_CHKBIT_Makro(&pi->io.chkbit);
//  copy_CHKBIT(&state, &pi->io.chkbit, &pt->chkbit); // nicht kopieren, im aktiven editor macht ein ungültiges chkbit keinen sinn
  
  pi->io.room          = pt->room;
  pi->io.space         = pt->room;

  pi->io.typ           = AS_INPUTBUTTON;
  if(is_button_cursor(pt) == _FALSE)
    pi->io.typ         = AS_INPUTBUTTON_WITOUT_CURSOR;

  pi->io.info.tbo.list = BUTTON_IO;
  pi->io.info.tbo.no   = 0;
  pi->io.attrib        = pt->attrib;
  pi->io.coltxt        = pt->col;
  pi->io.colframe      = pt->col;
  pi->io.colback       = pt->col;
  pi->searchchoice     = pt->searchchoice;
  pi->io.frametype     = pt->frametype;

  pi->io.xuser         = 0;
  if(pt->state == _TRUE)
  {
    if(is_server_defined(&pt->varlabel) == _FALSE)
      pi->io.xuser     = 1; // es ist ein momentary button mit status pressed
  }

//  pi->io.variable.no   = 1;
//  pi->io.variable.info[0].state = VAR_VAL;
//  pi->io.variable.info[0].value = 0xFFFF;
}

void set_buttonstate(_PICTUREMASK *p, _BOOL state)
{
  _UWORD  i;
  _BUTTON *pb;

  for(i=0, pb=p->membutton.ptr; i<p->membutton.no; i++, pb++)
  {
//    pb = &p->membutton.ptr[i];
    pb->state    = state;
    pb->oldstate = state;
  }
}

void kill_backgroundimage(_PICTUREMASK *p)
{
  _UWORD      i;
  _OBJECT     *po;
  _BUTTON     *pb;
  _IO         *pio;
  _PICTEXT    *ptx;
  _INPUT      *pin;
  _MOTION     *pmm;
  _COL_PICTXT *pcp;

  for(i=0, ptx=p->mempictxt.ptr; i<p->mempictxt.no; i++, ptx++)
    free_IMAGE(&ptx->image);

  for(i=0, pin=p->meminput.ptr; i<p->meminput.no; i++, pin++)
  {
    if(pin->io.Obj != NULL)
      pin->io.Obj->KillBackGroundImage();
    free_IMAGE(&pin->io.image);
  }

  for(i=0, pio=p->memoutput.ptr; i<p->memoutput.no; i++, pio++)
  {
    if(pio->Obj != NULL)
      pio->Obj->KillBackGroundImage();
    free_IMAGE(&pio->image);
  }

  for(i=0, pcp=p->colpictxt.ptr; i<p->colpictxt.no; i++, pcp++)
    free_IMAGE(&pcp->btx.image);

  for(i=0, pmm=p->memmotion.ptr; i<p->memmotion.no; i++, pmm++)
  {
    if(pmm->SubPicture != NULL)
      pmm->SubPicture->KillBackGroundImage();
    free_IMAGE(&pmm->image);
  }

  for(i=0, po=p->memobject.ptr; i<p->memobject.no; i++, po++)
  {
    if(po->SubPicture != NULL)
      po->SubPicture->KillBackGroundImage();
    free_IMAGE(&po->image);
  }

  for(i=0, pb=p->membutton.ptr; i<p->membutton.no; i++, pb++)
  {
    if(pb->SubPicture != NULL)
      pb->SubPicture->KillBackGroundImage();
  }
}

_BOOL copy_MEM_VARLABEL(_MEM_VARLABEL *d, _MEM_VARLABEL *s)
{
  _DWORD size = s->no * sizeof(_VARLABEL);

  if(s->no == d->no)
  {
    MemCpy(d->ptr, s->ptr, size);
    return _TRUE;
  }
  else
  {
    if(Memory2((void**)&d->ptr, size) != 0)
    {
      d->no = s->no;
      MemCpy(d->ptr, s->ptr, size);
      return _TRUE;
    }
  }

  free_MEM_VARLABEL(d);

  return _FALSE;
}

_BOOL copy_MEM_FONT(_MEM_FONT *d, _MEM_FONT *s)
{
  _BOOL  retcode = _FALSE;
  _DWORD size;

  size = s->no * sizeof(_FONT);

  if(s->no == d->no)
  {
    MemCpy(d->ptr, s->ptr, size);
    retcode = _TRUE;
  }
  else
  {
    if(Memory2((void**)&d->ptr, size) != 0)
    {
      d->no = s->no;
      MemCpy(d->ptr, s->ptr, size);
      retcode = _TRUE;
    }
  }

  if(retcode == _FALSE)
    free_MEM_FONT(d);

  return(retcode);
}

_BOOL copy_MEM_ATTRIB(_MEM_ATTRIB *d, _MEM_ATTRIB *s)
{
  _BOOL  retcode = _FALSE;
  _DWORD size;

  size = s->no * sizeof(_ATTRIB);

  if(s->no == d->no)
  {
    MemCpy(d->ptr, s->ptr, size);
    retcode = _TRUE;
  }
  else
  {
    if(Memory2((void**)&d->ptr, size) != 0)
    {
      d->no = s->no;
      MemCpy(d->ptr, s->ptr, size);
      retcode = _TRUE;
    }
  }

  if(retcode == _FALSE)
    free_MEM_ATTRIB(d);

  return(retcode);
}

_BOOL copy_MEM_BOOL(_MEM_BOOL *d, _MEM_BOOL *s)
{
  _BOOL  retcode = _FALSE;
  _DWORD size;

  size = s->no * sizeof(_BOOL);

  if(s->no == d->no)
  {
    MemCpy(d->ptr, s->ptr, size);
    retcode = _TRUE;
  }
  else
  {
    if(Memory2((void**)&d->ptr, size) != 0)
    {
      d->no = s->no;
      MemCpy(d->ptr, s->ptr, size);
      retcode = _TRUE;
    }
  }

  if(retcode == _FALSE)
    free_MEM_BOOL(d);

  return(retcode);
}

_BOOL copy_MEM_COLOR(_BOOL *retcode, _MEM_COLOR *d, _MEM_COLOR *s)
{
  _DWORD size;

  if(*retcode == _TRUE)
  {
    size = s->no * sizeof(_COLOR);

    if(s->no == d->no)
    {
      MemCpy(d->ptr, s->ptr, size);
    }
    else
    {
      if(Memory2((void**)&d->ptr, size) != 0)
      {
        d->no = s->no;
        MemCpy(d->ptr, s->ptr, size);
      }
      else
      {
        *retcode = _FALSE;
      }
    }
  }

  return(*retcode);
}

_BOOL copy_MEM_ANYTHING(_BOOL *retcode, _MEM_ANYTHING *d, _MEM_ANYTHING *s)
{
  _DWORD size;

  if(*retcode == _TRUE)
  {
    size = s->no * sizeof(_ANYTHING);

    if(s->no == d->no)
    {
      MemCpy(d->ptr, s->ptr, size);
    }
    else
    {
      if(Memory2((void**)&d->ptr, size) != 0)
      {
        d->no = s->no;
        MemCpy(d->ptr, s->ptr, size);
      }
      else
      {
        *retcode = _FALSE;
      }
    }
  }

  return(*retcode);
}

_BOOL copy_MEM_ANYPARA(_BOOL *retcode, _MEM_ANYPARA *d, _MEM_ANYPARA *s)
{
  _UWORD i;
  _ANYPARA *panypara;


  if(*retcode == _TRUE)
  {
    if(s->no != d->no)
    {
      *retcode = _FALSE;
      if(Memory2((void**)&d->ptr, s->no * sizeof(_ANYPARA)) != 0)
      {
        d->no = s->no;
        *retcode = _TRUE;
      }

    }
    
    if(*retcode == _TRUE)
    {
      panypara = d->ptr;
      for(i=0; i<d->no; i++,panypara++)
      {
        init_ANYPARA_Makro(panypara);
        if(*retcode == _TRUE)
          copy_ANYPARA(retcode, panypara, &s->ptr[i]);
      }
    }

  }

  return(*retcode);
}

_BOOL copy_MEM_LINKSCHEME(_BOOL *retcode, _MEM_LINKSCHEME *d, _MEM_LINKSCHEME *s)
{
  _DWORD size;
  _UWORD i;

  if(*retcode == _TRUE)
  {
    size = s->no * sizeof(_LINKSCHEME);

    if(s->no != d->no)
    {
      if(Memory2((void**)&d->ptr, size) != 0)
        d->no = s->no;
      else
        *retcode = _FALSE;
    }

    if(*retcode == _TRUE)
    {
      for(i=0; i<d->no; i++)
        copy_LINKSCHEME(retcode, &d->ptr[i], &s->ptr[i]);
    }
  }

  return(*retcode);
}

_BOOL copy_MEM_DYNCOLOR(_BOOL *retcode, _MEM_DYNCOLOR *d, _MEM_DYNCOLOR *s)
{
  _DWORD size;
  _UWORD i;

  if(*retcode == _TRUE)
  {
    size = s->no * sizeof(_DYNCOLOR);

    if(s->no != d->no)
    {
      if(Memory2((void**)&d->ptr, size) != 0)
        d->no = s->no;
      else
        *retcode = _FALSE;
    }

    if(*retcode == _TRUE)
    {
      for(i=0; i<d->no; i++)
        copy_DYNCOLOR(retcode, &d->ptr[i], &s->ptr[i]);
    }
  }

  return(*retcode);
}


_BOOL copy_DYNCOLOR(_BOOL *retcode, _DYNCOLOR *d, _DYNCOLOR *s)
{

  copy_LINKSCHEME(retcode, &d->linkscheme, &s->linkscheme);
  d->color = s->color;

  return(*retcode);
}
/*
_BOOL copy_LINEINFO(_LINEINFO *d, _LINEINFO *s)
{
  _BOOL retcode = _TRUE;

  free_LINEINFO(d);

  *d = *s;
  init_MEM_VARLABEL(&d->memvar);
  init_ANYPARA_Makro(&d->anypara);

  if(copy_ANYPARA(&retcode, &d->anypara, &s->anypara) == _TRUE)
    retcode = copy_MEM_VARLABEL(&d->memvar, &s->memvar);

  if(retcode == _FALSE)
    free_LINEINFO(d);

  return(retcode);
}

_BOOL copy_MEM_LINEINFO(_MEM_LINEINFO *d, _MEM_LINEINFO *s)
{
  _DWORD size;
  _UWORD i;
  _BOOL  retcode = _FALSE;

  free_MEM_LINEINFO(d);

  size = s->no * sizeof(_LINEINFO);

  if(Memory2((void**)&d->ptr, size) != 0)
  {
    retcode = _TRUE;
    d->no   = s->no;
    for(i=0; i<d->no; i++)
    {
      init_LINEINFO(&d->ptr[i]);
      if(copy_LINEINFO(&d->ptr[i], &s->ptr[i]) == _FALSE)
        retcode = _FALSE;
    }
  }

  if(retcode == _FALSE)
    free_MEM_LINEINFO(d);

  return(retcode);
}
*/

_BOOL copy_CSTATE(_BOOL *retcode, _CSTATE *d, _CSTATE *s)
{

  *d = *s;

  return(*retcode);
}

_BOOL copy_BUTTONIMAGES(_BOOL *retcode, _BUTTONIMAGES *d, _BUTTONIMAGES *s)
{

  *d = *s;

  return(*retcode);
}

_BOOL copy_SINGLESOFT(_BOOL *retcode, _SINGLESOFT *d, _SINGLESOFT *s)
{
  if(*retcode == _TRUE)
  {
   *d = *s;

    d->state    = _FALSE;
    d->oldstate = _FALSE;

    init_ANYPARA_Makro(&d->anypara);
    copy_ANYPARA(retcode, &d->anypara, &s->anypara);
    init_CHKBIT_Makro(&d->chkbit);
    copy_CHKBIT(retcode, &d->chkbit, &s->chkbit);
    init_GFUNCT_Makro(&d->gfunct);
    copy_GFUNCT(retcode, &d->gfunct, &s->gfunct);
    init_BUTTONIMAGES(&d->buttonimages);
    copy_BUTTONIMAGES(retcode, &d->buttonimages, &s->buttonimages);
    init_VARLABEL_Makro(&d->varlabel);
    copy_VARLABEL(&d->varlabel, &s->varlabel);
   #ifdef UC_DYN_SOFTKEYS
    init_MYSCHEME_Makro(&d->stscheme);
    copy_MYSCHEME(retcode, &d->stscheme, &s->stscheme);
    init_LINKSCHEME_Makro(&d->dyncolback);
    copy_LINKSCHEME(retcode, &d->dyncolback, &s->dyncolback);
    init_LINKSCHEME_Makro(&d->dyncolfront);
    copy_LINKSCHEME(retcode, &d->dyncolfront, &s->dyncolfront);
    init_LINKSCHEME_Makro(&d->dyncoltl);
    copy_LINKSCHEME(retcode, &d->dyncoltl, &s->dyncoltl);
    init_LINKSCHEME_Makro(&d->dyncolbr);
    copy_LINKSCHEME(retcode, &d->dyncolbr, &s->dyncolbr);
    init_LINKSCHEME_Makro(&d->dyncoltxt);
    copy_LINKSCHEME(retcode, &d->dyncoltxt, &s->dyncoltxt);
   #endif
    init_CSTATE(&d->cstate);
    copy_CSTATE(retcode, &d->cstate, &s->cstate);
    init_ANYPARA_Makro(&d->newbubbletext);
    copy_ANYPARA(retcode, &d->newbubbletext, &s->newbubbletext);
  }

  return(*retcode);
}

_BOOL copy_SOFTFLOOR(_BOOL *retcode, _SOFTFLOOR *d, _SOFTFLOOR *s)
{
  _UWORD i;

  if(*retcode == _TRUE)
  {
    for(i=0; i<NO_SOFT_FLOOR; i++)
      copy_SINGLESOFT(retcode, &d->floor[i], &s->floor[i]);
  }

  return(*retcode);
}
/*
_BOOL copy_VARINFO(_BOOL *retcode, _VARINFO *d, _VARINFO *s)
{
 #ifdef UC_LINKVL
  _UWORD i;
 #endif


  if(*retcode == _TRUE)
  {
    *d = *s;

    init_VARLABEL(&d->lo_limit);
//    init_VARLABEL(&d->lost_limit);
    init_VARLABEL(&d->hi_limit);
//    init_VARLABEL(&d->hist_limit);
    init_ANYPARA_Makro(&d->text1);                             // vartext 1
    init_ANYPARA_Makro(&d->text2);                             // vartext 2
    init_ANYPARA_Makro(&d->text3);                             // vartext 3
    init_ANYPARA_Makro(&d->text4);                             // vartext 4
    init_VARLABEL(&d->xadress);
    init_VARLABEL(&d->parameter);

    copy_VARLABEL(&d->lo_limit,   &s->lo_limit);
//    copy_VARLABEL(&d->lost_limit, &s->lost_limit);
    copy_VARLABEL(&d->hi_limit,   &s->hi_limit);
//    copy_VARLABEL(&d->hist_limit, &s->hist_limit);
    copy_ANYPARA(retcode, &d->text1, &s->text1);         // vartext 1
    copy_ANYPARA(retcode, &d->text2, &s->text2);         // vartext 2
    copy_ANYPARA(retcode, &d->text3, &s->text3);         // vartext 3
    copy_ANYPARA(retcode, &d->text4, &s->text4);         // vartext 4
    copy_VARLABEL(&d->xadress, &s->xadress);
    copy_VARLABEL(&d->parameter, &s->parameter);
   #ifdef UC_LINKVL
    for(i=0; i<UC_LINKVL; i++)
      copy_LINKVL(retcode, &d->linkvl[i], &s->linkvl[i]);
   #endif
  }

  return(*retcode);
}
*/

void copy_VARLABEL_IADDRESS(_VARLABEL *d, _IADDRESS s)
{
  // diese funktion setzt voraus dass vorher init_VARLABEL aufgerufen wurde !!
  // init_VARLABEL(d);
  if(s != DEF_IADDRESS)
  {
    d->no = 1;
    d->info[0].state = REFINDEX_VAL;
    d->info[0].value = s;
  }
}

_BOOL copy_LSE_VARINFO(_BOOL *retcode, _VARINFO *d, _LSEVARINFO *s, _BOOL st)
{
  _UWORD seqno;

  init_VARINFO_Makro(d);

  if(*retcode == _TRUE)
  {
    d->vartype     = s->vartype;
    d->classid     = s->classid;

    d->lo_limit.no             = 1;
    d->lo_limit.info[0].state  = s->hilolim.state & 0x0F;
    d->lo_limit.info[0].value  = s->hilolim.lolim;
    d->hi_limit.no             = 1;
    d->hi_limit.info[0].state  = (s->hilolim.state >> 4) & 0x0F;
    d->hi_limit.info[0].value  = s->hilolim.hilim;

    d->accessno    = s->accessno;
    d->format      = s->format;
    d->station     = s->station;
    d->bitno       = s->reftime_bitno & 0x1F;
    d->funit       = s->funit;
    d->POWPRUIC    = s->POWPRUIC;
    d->funitscheme = s->funitscheme;
    d->reftime     = get_reftime(s);

    switch(s->xmemtext.xxno)
    {
      case 1 : d->text1 = s->xmemtext.xxptr[0];
               break;
      case 2 : d->text1 = s->xmemtext.xxptr[0];
               d->text2 = s->xmemtext.xxptr[1];
               break;
      case 3 : d->text1 = s->xmemtext.xxptr[0];
               d->text2 = s->xmemtext.xxptr[1];
               d->text3 = s->xmemtext.xxptr[2];
               break;
      case 4 : d->text1 = s->xmemtext.xxptr[0];
               d->text2 = s->xmemtext.xxptr[1];
               d->text3 = s->xmemtext.xxptr[2];
               d->text4 = s->xmemtext.xxptr[3];
               break;
    }

    if(st == _FALSE)
    {
      // alle abhängigkeiten werden gelöscht (nicht mitkopiert)
      d->text1.memvarlabel.no  = 0;
      d->text1.memvarlabel.ptr = NULL;
      d->text2.memvarlabel.no  = 0;
      d->text2.memvarlabel.ptr = NULL;
      d->text3.memvarlabel.no  = 0;
      d->text3.memvarlabel.ptr = NULL;
      d->text4.memvarlabel.no  = 0;
      d->text4.memvarlabel.ptr = NULL;
    }

   #ifdef _FIXED_STATESCHEME
    if(s->stscheme == NOSTSCHEME)
      d->stscheme = DEFAULTSCHEME;
    else
      d->stscheme = s->stscheme;
   #else
    d->stscheme    = s->stscheme;
   #endif

    d->statistic   = s->statistic;
    d->uimenuindex = s->uimenuindex;                     // menueindex

    seqno             = s->xxchkbit & 0x7FFF;
    d->xchkbit.seqno  = (seqno < 0x7FFF)? seqno : DEFSEQNO;
    d->xchkbit.invert = (s->xxchkbit & 0x8000)? _TRUE : _FALSE;
    d->userconfig  = s->userconfig;           // Anwenderkonfiguration

   #ifdef UC_LINKVL
    if(s->memlinkvl.no)
    {
      switch(s->memlinkvl.no)
      {
        case 1 : copy_LINKVL(retcode, &d->linkvl[0], &s->memlinkvl.ptr[0]);
                 break;
        case 2 : copy_LINKVL(retcode, &d->linkvl[0], &s->memlinkvl.ptr[0]);
                 copy_LINKVL(retcode, &d->linkvl[1], &s->memlinkvl.ptr[1]);
                 break;
        case 3 : copy_LINKVL(retcode, &d->linkvl[0], &s->memlinkvl.ptr[0]);
                 copy_LINKVL(retcode, &d->linkvl[1], &s->memlinkvl.ptr[1]);
                 copy_LINKVL(retcode, &d->linkvl[2], &s->memlinkvl.ptr[2]);
                 break;
        case 4 : copy_LINKVL(retcode, &d->linkvl[0], &s->memlinkvl.ptr[0]);
                 copy_LINKVL(retcode, &d->linkvl[1], &s->memlinkvl.ptr[1]);
                 copy_LINKVL(retcode, &d->linkvl[2], &s->memlinkvl.ptr[2]);
                 copy_LINKVL(retcode, &d->linkvl[3], &s->memlinkvl.ptr[3]);
                 break;
        default: copy_LINKVL(retcode, &d->linkvl[0], &s->memlinkvl.ptr[0]);
                 copy_LINKVL(retcode, &d->linkvl[1], &s->memlinkvl.ptr[1]);
                 copy_LINKVL(retcode, &d->linkvl[2], &s->memlinkvl.ptr[2]);
                 copy_LINKVL(retcode, &d->linkvl[3], &s->memlinkvl.ptr[3]);
                 copy_LINKVL(retcode, &d->linkvl[4], &s->memlinkvl.ptr[4]);
                 break;
      }
    }
   #endif

    d->hostname    = (s->hostname == 0xFFFF)? DEFSCOPE : s->hostname;
    d->physic      = s->physic;

    copy_VARLABEL_IADDRESS(&d->xadress, s->iaddress);
//    d->xadress     = s->xadress;
    d->lasalid     = s->lasalid;
    d->crc32       = s->crc32;
    d->varobj      = 1;

    d->formula     = 0;                             // formula
    d->digits      = 0;                        // no. of digits
    d->posdezpt    = 0;              // position decimalpoint
//    init_VARLABEL(&d->parameter);      // ifnec. parameter for formula

   #ifdef UC_EXOONLINE
    d->exoinfo = s->exoinfo;
   #endif

    if(*retcode == _TRUE)
      *retcode = VarList.InsertUnitInfo(d);
  }

  return(*retcode);
}

_BOOL copy_SINGLE_ALARM(_BOOL *retcode, _SINGLE_ALARM *d, _SINGLE_ALARM *s)
{
  if(*retcode == _TRUE)
  {
    *d = *s;

    init_VARLABEL_Makro(&d->link);
    init_ANYPARA_Makro(&d->text);
    init_ANYPARA_Makro(&d->helptext);
    init_MEM_VARLABEL(&d->parameter);

    copy_VARLABEL(&d->link, &s->link);
    init_CHKBIT_Makro(&d->chkbit);
    copy_CHKBIT(retcode, &d->chkbit, &s->chkbit);
    copy_ANYPARA(retcode, &d->text, &s->text);
    copy_ANYPARA(retcode, &d->helptext, &s->helptext);
    if(copy_MEM_VARLABEL(&d->parameter, &s->parameter) == _FALSE)
      *retcode = _FALSE;
  }

  return(*retcode);
}

_BOOL copy_LINKVL(_BOOL *retcode, _LINKVL *d, _LINKVL *s)
{

  if(*retcode == _TRUE)
  {
    d->info = s->info;
    copy_VARLABEL(&d->link, &s->link);
    *retcode = _TRUE;
  }

  return(*retcode);
}

_BOOL copy_LINEINFO(_LINEINFO *d, _LINEINFO *s)
{
  _BOOL retcode = _TRUE;

  free_LINEINFO(d);

  copy_CHKBIT(&retcode, &d->chkbit, &s->chkbit);
  copy_VARLABEL(&d->value, &s->value);
  copy_GFUNCT(&retcode, &d->gfunct, &s->gfunct);
  copy_ANYPARA(&retcode, &d->anypara, &s->anypara);

  if(copy_MEM_VARLABEL(&d->memvar, &s->memvar) == _FALSE)
    retcode = _FALSE;

  copy_MYSCHEME(&retcode, &d->stscheme, &s->stscheme);

  return(retcode);
}

_BOOL copy_MEM_LINEINFO(_MEM_LINEINFO *d, _MEM_LINEINFO *s)
{
  _BOOL     retcode = _FALSE;
  _LINEINFO *spli, *dpli;
  _UWORD    i;

  free_MEM_LINEINFO(d);

  if(Memory2((void**)&d->ptr, s->no * sizeof(_LINEINFO)) != 0)
  {
    retcode = _TRUE;
    d->no   = s->no;
    dpli    = d->ptr;
    spli    = s->ptr;
    for(i=0; i<s->no; i++, dpli++, spli++)
     {
      init_LINEINFO(dpli);
      if(copy_LINEINFO(dpli, spli) == _FALSE)
        retcode = _FALSE;
    }
  }

  return(retcode);
}

_BOOL copy_MENUINFO(_MENUINFO *d, _MENUINFO *s)
{
  *d = *s;

  init_VARLABEL_Makro(&d->server);
  copy_VARLABEL(&d->server, &s->server);

  return(_TRUE);
}

_BOOL copy_LSEMENU(_LSEMENU *d, _LSEMENU *s)
{
  _BOOL retcode = _TRUE;

  free_LSEMENU(d);

  if(copy_MENUINFO(&d->info, &s->info) == _FALSE)
    retcode = _FALSE;
  if(copy_MEM_LINEINFO(&d->mem_lineinfo, &s->mem_lineinfo) == _FALSE)
    retcode = _FALSE;
  if(d->TextPuffer.Copy(&s->TextPuffer) == _FALSE)
    retcode = _FALSE;

  if(retcode == _FALSE)
    free_LSEMENU(d);

  d->lsemenuindex = s->lsemenuindex;

  return(retcode);
}

_BOOL copy_DOT(_DOT *d, _DOT *s)
{
  *d = *s;
  return(_TRUE);
}

_BOOL copy_DOT3D(_DOT3D *d, _DOT3D *s)
{
  *d = *s;
  return(_TRUE);
}

_BOOL copy_SHAPE(_SHAPE *d, _SHAPE *s)
{
  _BOOL   retcode = _FALSE;
  _UDWORD size = s->no*sizeof(_DOT3D);

  *d     = *s;
  d->ptr = NULL;

  if(Memory2((void**)&d->ptr, size) != 0)
  {
    MemCpy(d->ptr, s->ptr, size);
    retcode = _TRUE;
  }
  else
  {
    init_SHAPE(d);
  }

  return(retcode);
}

_BOOL copy_3DSHAPE(_3DSHAPE *d, _3DSHAPE *s)
{
  _BOOL   retcode = _FALSE;
  _UWORD  i;
  _SHAPE  *dp, *sp;
  _UDWORD size = s->no * sizeof(_SHAPE);

  *d     = *s;
  d->ptr = NULL;

  if(Memory2((void**)&d->ptr, size) != 0)
  {
    MemSet(d->ptr, 0, size);
    retcode = _TRUE;
    for(i=0, dp=d->ptr, sp=s->ptr; i<s->no; i++, dp++, sp++)
    {
      if(copy_SHAPE(dp, sp) == _FALSE)
      {
        free_3DSHAPE(d);
        return(_FALSE);
      }
    }
  }

  return(retcode);
}

/*
_BOOL copy_UNIT(_BOOL *retcode, _UNIT *d, _UNIT *s)
{
  if(*retcode == _TRUE)
  {
    *d = *s;
    init_VARLABEL(&d->posdezpt);
    init_VARLABEL(&d->parameter);
    init_ANYPARA_Makro(&d->anypara);
    copy_VARLABEL(&d->posdezpt, &s->posdezpt);
    copy_VARLABEL(&d->parameter, &s->parameter);
    copy_ANYPARA(retcode, &d->anypara, &s->anypara);

  }

  return(retcode);
}
*/

