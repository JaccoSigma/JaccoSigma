#include "T_IPC.H"
#include "T_NUC.H"


//_BOOL overload(_UWORD*, _ASCII*);
//_BOOL overload_PICTUREMASK(_PICTUREMASK*, _VARLABEL*, _BOOL change_ctrl_type);
_BOOL change_overload_PICTUREMASK(_PICTUREMASK*, _VARLABEL*, _VARLABEL*);
_BOOL summary_overload_PICTUREMASK(_PICTUREMASK*, _UWORD, _VARLABEL*);
_BOOL overload_ANYPARA(_ANYPARA*, _VARLABEL*);
void  overload_MEM_ANYTHING(_MEM_ANYTHING*, _VARLABEL*);
_BOOL overload_MEM_ANYPARA(_MEM_ANYPARA*, _VARLABEL*);


_BOOL overload_deep(_UDWORD *destvarno, _UDWORD varno, _UDWORD classid)
{
  _LSEVARINFO *pi;
  _VARSYMBOL  *ps;

  // note: keep off, function is kinky recursiv !!

  if(VarList.GetVarInfo(&pi, varno) == _TRUE)
  {
   #ifdef USER_DEMAG
    if((pi->classid == classid)||(OverloadAbsolute == _TRUE))
   #else
    if(pi->classid == classid)
   #endif
    {
      return(_TRUE);
    }

    if(VarList.GetVarSymbol(&ps, varno) == _TRUE)
    {
      if(ps->scope != DEFSCOPE)
      {
        *destvarno = varno;
        return(overload_deep(destvarno, ps->scope, classid));
      }
    }
  }

  return(_FALSE);
}

_BOOL overload(_UDWORD *varno, _UDWORD loader)
{
  // =====================================================================
  // varlabel :
  // classid  : 13     17       5
  // name     : objekt.embedded.test
  // name     : objekt.embedded.wert
  // ++++++++++++++++++++++++++++++++:+++++++++++++++++++++:++++++++++++++
  // loader   :                      :                     :
  // classid  : 17                   : 13                  : 5
  // name     : hugo                 : josef               : sepp
  // --------------------------------:---------------------:--------------
  // ergebnis : hugo.test            : josef.embedded.test : sepp
  // ergebnis : hugo.wert            : josef.embedded.test : sepp
  // ================================:=====================:==============

  _ASCII      label[300];
  _VARSYMBOL  *ps;
  _LSEVARINFO *pi, *po;
  _UDWORD     classid, station;
  _UBYTE      varobj;
  _UDWORD     memno, tmp;
  _BOOL       st;

  if(VarList.GetVarInfo(&po, loader) == _TRUE)
  {
    classid = po->classid;
    if(VarList.GetVarInfo(&pi, *varno) == _TRUE)
    {
      if(pi->classid == classid)
      {
        *varno = loader;
        return(_TRUE);
      }
      
      if((pi->vartype == BINARY) || (pi->vartype == FMERKER))
      {
        switch(po->vartype)
        {
          case BINARY :
          case FTIME  :
          case FDATE  :
          case FMERKER: 
            // BINARY oder MERKER mit BINARY oder TIME oder DATE oder MERKER
            *varno = loader;
            return(_TRUE);
            
          default     : 
            break;
        }
      }
      
      if((pi->vartype == FENUM) && (po->vartype == FENUM))
      {
        // ENUM mit ENUM
        *varno = loader;
        return(_TRUE);
      }
    
      if(VarList.GetVarSymbol(&ps, *varno) == _TRUE)
      {
        if(ps->scope != DEFSCOPE)
        {
          memno = *varno;
          if(overload_deep(&memno, ps->scope, classid) == _TRUE) // wicked !!
          {
            if(VarList.GetVarSymbol(&ps, memno) == _TRUE)
            {
              varobj = 0;
              tmp       = ps->scope;
              ps->scope = loader;
              st        = VarList.GetLabel(label, &varobj, *varno);
              ps->scope = tmp;
              if(st == _TRUE)
              {
                station = po->station;
                if((pi->vartype == FCONSTANT)||(pi->vartype == FMERKER)) //||(pi->vartype == FENUMITEM))
                  station = 0xFFFFFFFFL; // bei const und merker ist die stationsnummer im lse immer -1 und auch die crc damit berechnet
                  
                *varno = VarList.GetVarNo(label, station);
                
                return(_TRUE);
              }
            }
          }
        }
      }
    }
    else if(*varno == VARNO_EDITORDUMMY)
    {
      // sonderfall, es handelt sich um die platzierte variable in einem touch-editor
      *varno = loader;
      return(_TRUE);
    }
  }

  return(_FALSE);
}

_BOOL overload_VARLABEL(_VARLABEL *vl, _VARLABEL *ov)
{
  _BOOL   retcode = _FALSE;
  _UDWORD i, no;
 
  if(chk_VARLABEL_Makro(vl)) // Makro, speed: nicht auf true oder false abfragen
  {
    if((vl->info[0].state == VAR_VAL) && (valid_VARLABEL(ov) == _TRUE))
    {
      no = vl->info[0].value;

      if(overload(&no, ov->info[0].value) == _TRUE)
      {
        retcode           = _TRUE;
        vl->info[0].value = no;
        
        if(ov->no > vl->no)
        {
          for(i=vl->no; i<ov->no; i++)
            vl->info[i] = ov->info[i];
          vl->no = ov->no;
        }
      }
    }
  }

  return(retcode);
}

_BOOL overload_MEM_VARLABEL(_MEM_VARLABEL *p, _VARLABEL *ov)
{
  _UWORD    i;
  _BOOL     retcode = _FALSE;
  _VARLABEL *pvm;

  for(i=0, pvm=p->ptr; i<p->no; i++, pvm++)
  {
    if(overload_VARLABEL(pvm, ov) == _TRUE)
      retcode = _TRUE;
  }

  return(retcode);
}

_BOOL overload_STACK_LINE(_STACK_LINE *p, _VARLABEL *ov)
{
  _BOOL retcode = _FALSE;

  if(overload_VARLABEL(&p->op1, ov) == _TRUE)
    retcode = _TRUE;
  if(overload_VARLABEL(&p->op2, ov) == _TRUE)
    retcode = _TRUE;
#ifdef UC_OVERLOAD_FCB_SUB
  // bei call functionblock wird auf 2ten parameter die "overloadervariable" geschrieben
  if(p->cmd == CMD_FUNCTIONBLOCK)
  {
    if(p->op2.no == 0)
    {
      p->op2 = *ov;
    }
  }
#endif
  return(retcode);
}

_BOOL overload_STACK(_STACK *ps, _VARLABEL *ov)
{
  _BOOL retcode = _FALSE;
  
  if(ps != NULL)
  {
    if(valid_VARLABEL(ov) == _TRUE)
    {
      _UDWORD nox = ps->no;
      _STACK_LINE *psl = ps->ptr;
    
      while(nox--)
      {
        if(overload_STACK_LINE(psl, ov) == _TRUE)
        {
          retcode = _TRUE;
        }
        psl++;
      }
    }
  }
  
  return retcode;
}

_BOOL overload_LSEFUNCT(_LSEFUNCT *p, _VARLABEL *ov)
{
  _Stack      *Stack;
  _BOOL       retcode = _TRUE;
  _STACK      *ps, tmp;
  _UWORD      i;
  _STACK_LINE *psl;

  if(valid_VARLABEL(ov) == _TRUE)
  {
    if(InterBoxList.ChkFunction(p->no) == _TRUE)
    {
      Stack = InterBoxList.Get(p->no);
      ps    = Stack->GetStack();
      if(ps->no > 0)
      {
        init_STACK_Makro(&tmp);
        if(copy_STACK(&tmp, ps) == _TRUE)
        {
          p->overload = tmp;
          p->no       = 0xFFFF;
        }
        else
        {
          free_STACK(&tmp);
          retcode = _FALSE;
        }
      }
    }

    if(p->overload.no > 0)
    {
      ps = &p->overload;
      for(i=0, psl=ps->ptr; i<ps->no; i++, psl++)
      {
        if(overload_STACK_LINE(psl, ov) == _FALSE)
          retcode = _FALSE;
      }
    }
  }

  return(retcode);
}

/*
_BOOL overload_LSEFUNCT(_LSEFUNCT *p, _VARLABEL *ov)
{
  _Stack *Stack;
  _BOOL  retcode = _FALSE;
  _STACK *ps, tmp;
  _UWORD i;

  if(InterBoxList.ChkFunction(p->no) == _TRUE)
  {
    if(valid_VARLABEL(ov) == _TRUE)
    {
      Stack = InterBoxList.Get(p->no);
      ps    = Stack->GetStack();
      if(ps->no > 0)
      {
        init_STACK_Makro(&tmp);
        if(copy_STACK(&tmp, ps) == _TRUE)
        {
          for(i=0; i<tmp.no; i++)
          {
            if(overload_STACK_LINE(&tmp.ptr[i], ov) == _TRUE)
              retcode = _TRUE;
          }

          if(retcode == _TRUE)
          {
            p->no       = 0xFFFF;
            p->overload = tmp;
          }
          else
          {
            free_STACK(&tmp);
          }
        }
      }
    }
  }

  return(retcode);
}
*/

_BOOL overload_GFUNCT(_GFUNCT *p, _VARLABEL *ov)
{
  _BOOL retcode = _FALSE;

  if(overload_LSEFUNCT(&p->pos_fl, ov) == _TRUE)
    retcode = _TRUE;
  if(overload_LSEFUNCT(&p->neg_fl, ov) == _TRUE)
    retcode = _TRUE;

  return(retcode);
}

_BOOL overload_MEM_LSEFUNCT(_MEM_LSEFUNCT *p, _VARLABEL *ov)
{
  _UWORD    i;
  _BOOL     retcode = _TRUE;
  _LSEFUNCT *plf;

  for(i=0, plf=p->ptr; i<p->no; i++, plf++)
    overload_LSEFUNCT(plf, ov);

  return(retcode);
}

_BOOL overload_CHKBIT(_CHKBIT *p, _VARLABEL *ov)
{
  _UBYTE    array[500];
  _COMPCODE *pc, tmp;

  if(p->seqno != DEFSEQNO)
  {
    if(Sequencer.Get(&pc, p->seqno) == _TRUE)
    {
      if(pc->size < 500)
      {
        if(valid_VARLABEL(ov) == _TRUE)
        {
          init_COMPCODE(&tmp);
          tmp.ptr  = array;
          tmp.size = pc->size;
          MemCpy(tmp.ptr, pc->ptr, tmp.size);
          if(overload_COMPCODE(&tmp, ov->info[0].value) == _TRUE)
          {
            _BOOL st = _TRUE;
            if(copy_COMPCODE(&st, &p->overload, &tmp) == _TRUE)
            {
              p->seqno = DEFSEQNO;
              return _TRUE;
            }
          }
        }
      }
     #ifdef PGTEST
      else
      {
        Annahme("T_OVRLOD.CPP", __LINE__);
        // increase size of array
      }
     #endif
    }
  }
  else if(p->overload.size != 0)
  {
    if(valid_VARLABEL(ov) == _TRUE)
      return overload_COMPCODE(&p->overload, ov->info[0].value);
  }

  return _FALSE;
}

//_BOOL overload_SINGLESCHEME(_SINGLESCHEME *p,  _VARLABEL *ov)
//{
//  return(overload_VARLABEL(&p->upto, ov));
//}

_BOOL overload_MEM_SINGLESCHEME(_MEM_SINGLESCHEME *p, _VARLABEL *ov)
{
  _BOOL         retcode = _FALSE;
  _UWORD        i;
  _SINGLESCHEME *pss;

  if(p->result_is_varno == _TRUE)
  {
    for(i=0, pss=p->ptr; i<p->no; i++, pss++)
    {
      //if(overload_SINGLESCHEME(pss, ov) == _TRUE) 
      if(overload_VARLABEL(&pss->upto, ov) == _TRUE) // speed
        retcode = _TRUE;
      if(overload(&pss->value, ov->info[0].value) == _TRUE)      
        retcode = _TRUE;
    }
    
    return(retcode);
  }
  
  for(i=0, pss=p->ptr; i<p->no; i++, pss++)
  {
    //if(overload_SINGLESCHEME(pss, ov) == _TRUE)
    if(overload_VARLABEL(&pss->upto, ov) == _TRUE) // speed
      retcode = _TRUE;
  }
  return(retcode);
}


_BOOL overload_SCHEME(_SCHEME *d, _SCHEME *s, _VARLABEL *ov)
{
  _BOOL     st, retcode = _FALSE;
  _VARLABEL vl;

//  if(valid_VARLABEL(ov) == _TRUE) // speed
  {
    st = _FALSE;
    vl = s->link;
    if(overload_VARLABEL(&vl, ov) == _TRUE)
      st = _TRUE;

    free_SCHEME(d);
    if(copy_SCHEME(d, s) == _TRUE)
    {
      if(overload_MEM_SINGLESCHEME(&d->memsinglescheme, ov) == _TRUE)
        st = _TRUE;

      if(st == _TRUE)
      {
        d->link = vl;
        retcode = _TRUE;
      }
      else
      {
        free_SCHEME(d);
      }
    }
  }

  return(retcode);
}

_BOOL overload_MYSCHEME(_MYSCHEME *p, _VARLABEL *ov, _Scheme *Scheme)
{
  _BOOL   retcode = _FALSE;
  _SCHEME *pcs;

  if(valid_VARLABEL(ov) == _TRUE)
  {
    if(Scheme->Get(&pcs, p->schemeno) == _TRUE)
    {
      if(overload_SCHEME(&p->overload, pcs, ov) == _TRUE)
      {
        p->schemeno = DEFAULTSCHEME;
      }
    }
    else if(p->overload.memsinglescheme.no > 0)
    {
      overload_VARLABEL(&p->overload.link, ov);
      retcode = overload_MEM_SINGLESCHEME(&p->overload.memsinglescheme, ov);
    }
  }

  return(retcode);
}

_BOOL overload_LINKSCHEME(_LINKSCHEME *p, _VARLABEL *ov, _Scheme *Scheme)
{
  _BOOL retcode = _FALSE;

//  if(valid_VARLABEL(ov) == _TRUE) // speed
  {
    if(overload_VARLABEL(&p->varlabel, ov) == _TRUE)
      retcode = _TRUE;

    if(overload_MYSCHEME(&p->myscheme, ov, Scheme) == _TRUE)
      retcode = _TRUE;
  }

  return(retcode);
}
/*
_BOOL overload_MEM_LINKSCHEME(_MEM_LINKSCHEME *p, _VARLABEL *ov, _Scheme *Scheme)
{
  _UWORD      i;
  _BOOL       retcode = _FALSE;
  _LINKSCHEME *pls;

  if(valid_VARLABEL(ov) == _TRUE)
  {
    for(i=0, pls=p->ptr; i<p->no; i++, pls++)
    {
      if(overload_LINKSCHEME(pls, ov, Scheme) == _TRUE)
        retcode = _TRUE;
    }
  }
  return(retcode);
}
*/
_BOOL overload_DYNCOLOR(_DYNCOLOR *p, _VARLABEL *ov, _Scheme *Scheme)
{
  return overload_LINKSCHEME(&p->linkscheme, ov, Scheme);
}

_BOOL overload_MEM_DYNCOLOR(_MEM_DYNCOLOR *p, _VARLABEL *ov, _Scheme *Scheme)
{
  _UWORD    i;
  _BOOL     retcode = _FALSE;
  _DYNCOLOR *pdc;

  for(i=0, pdc=p->ptr; i<p->no; i++, pdc++)
  {
    if(overload_DYNCOLOR(pdc, ov, Scheme) == _TRUE)
      retcode = _TRUE;
  }
  
  return(retcode);
}

_BOOL overload_SINGLETXTSCHEME(_SINGLETXTSCHEME *p,  _VARLABEL *ov)
{
  _BOOL retcode = _FALSE;

  if(overload_VARLABEL(&p->upto, ov) == _TRUE)
    retcode = _TRUE;

  if(overload_ANYPARA(&p->anypara, ov) == _TRUE)
    retcode = _TRUE;

  return(retcode);
}

_BOOL overload_TXTSCHEME(_TXTSCHEME *p, _VARLABEL *ov)
{
  _BOOL  retcode = _FALSE;
  _UWORD i;
  _SINGLETXTSCHEME *pst;

  // if(valid_VARLABEL(ov) == _TRUE) // speed
  {
    for(i=0, pst=p->ptr; i<p->no; i++, pst++)
    {
      if(overload_SINGLETXTSCHEME(pst, ov) == _TRUE)
        retcode = _TRUE;
    }
  }

  return(retcode);
}

_BOOL overload_MYTXTSCHEME(_MYTXTSCHEME *p, _VARLABEL *ov, _TextScheme *Scheme)
{
  _BOOL      st, retcode = _FALSE;
  _TXTSCHEME *pcs = NULL;

  if(valid_VARLABEL(ov) == _TRUE)
  {
    if(Scheme->Get(&pcs, p->schemeno) == _TRUE)
    {
      st = _TRUE;
      if(copy_TXTSCHEME(&st, &p->overload, pcs) == _TRUE)
      {
        p->schemeno = DEFAULTSCHEME;
        if(overload_TXTSCHEME(&p->overload, ov) == _TRUE)
          retcode = _TRUE;
      }
    }
    else if(p->overload.no > 0)
    {
      if(overload_TXTSCHEME(&p->overload, ov) == _TRUE)
        retcode = _TRUE;
    }
  }

  return(retcode);
}

void overload_IOINFO(_IOINFO *p, _VARLABEL *ov)
{
  overload_MYTXTSCHEME(&p->mytxtscheme, ov, &TextScheme);

  switch(p->tbo.list)
  {
    case IMAGESCHEME  : overload_MYSCHEME(&p->myscheme, ov, &ImageScheme);  break;
    case OBJECTSCHEME : overload_MYSCHEME(&p->myscheme, ov, &ObjectScheme); break;
    default           : break;
  }
}


#ifdef OK_ENCLOSE_NAME
_UWORD add_FAST_ASCII_BUFFER(_FAST_ASCII_BUFFER *p, _ASCII *label)
{
  _UWORD  i, no, len;
  _ASCII  *ptr, *ph;
  _UDWORD size;

  ptr = p->ascii_buffer.ptr;
  no  = p->ascii_buffer.no;

  if(p->poffset != NULL)
  {
    for(i=0; i<no; i++)
    {
      ph = &ptr[p->poffset[i]];

      if(ph[0] == label[0])
      {
        if(StrXmp(ph, label, sizeof(_ASCII)) == 0)
          return(i);
      }
    }
  }
  else
  {
    for(i=0; i<no; i++)
    {
      if(ptr[0] == label[0])
      {
        if(StrXmp(ptr, label, sizeof(_ASCII)) == 0)
          return(i);
      }
      ptr += (StrLen(ptr, sizeof(_ASCII)) + 1);
    }
  }

  size = p->ascii_buffer.size;
  len  = StrLen(label, sizeof(_ASCII)) + 1;

  if(Memory2((void**)&p->ascii_buffer.ptr, size + len) != 0)
  {
    p->ascii_buffer.no   += 1;
    p->ascii_buffer.size += len;
    p->ascii_buffer.space = p->ascii_buffer.size;

    MemCpy(&p->ascii_buffer.ptr[size], label, len);

    if(p->poffset != NULL)
    {
      if(Memory2((void**)&p->poffset, p->ascii_buffer.no * sizeof(_UDWORD)) != 0)
        p->poffset[p->ascii_buffer.no - 1] = size;
      else
        Memory2((void**)&p->poffset, 0);
    }
    return(p->ascii_buffer.no - 1);
  }

  return(0xFFFF);
}

void overload_VIRTNAME(_VIRTNAME *p, _VARLABEL *ov, _PICTUREMASK *pm)
{
  _ASCII       *name;
  _ASCII       label[100];
  _VARLABEL    varlabel, tmp;
//  _AsciiPuffer *pa;
  _UWORD       index;
  _UBYTE       varobj = 0;
  _LSEVARINFO  *po;

  name = get_virtname(p->no, pm);
  if(name != NULL)
  {
    if(valid_VARLABEL(ov) == _TRUE)
    {
      if(VarList.GetVarInfo(&po, ov->info[0].value) == _TRUE)
      {
        init_VARLABEL_Makro(&varlabel);
        varlabel.no = 1;
        varlabel.info[0].state = VAR_VAL;
        varlabel.info[0].value = VarList.GetVarNo(name, po->station);
        tmp = varlabel;

        if(valid_VARLABEL(&varlabel) == _TRUE)
        {
          overload_VARLABEL(&varlabel, ov);
          if(cmp_VARLABEL(&varlabel, &tmp) != 0)
          {
            if(VarList.GetLabel(label, &varobj, varlabel.info[0].value) == _TRUE)
            {
              index = add_FAST_ASCII_BUFFER(&pm->enclosed_name, label);
              if(index != 0xFFFF)
              {
                free_VIRTNAME(p);
                p->no = index;
              }
            }
          }
        }
      }
    }
  }
}

void overload_MEM_INPUT_VIRTNAME(_MEM_INPUT *p, _VARLABEL *ov, _PICTUREMASK *pm)
{
  _UWORD i;
  _INPUT *pi;

  for(i=0, pi=p->ptr; i<p->no; i++, pi++)
    overload_VIRTNAME(&pi->io.virtname, ov, pm);
}

void overload_MEM_OUTPUT_VIRTNAME(_MEM_OUTPUT *p, _VARLABEL *ov, _PICTUREMASK *pm)
{
  _UWORD i;
  _IO    *po;

  for(i=0, po=p->ptr; i<p->no; i++, po++)
    overload_VIRTNAME(&po->virtname, ov, pm);
}

#endif

void overload_THEURGE(_THEURGE *p, _VARLABEL *ov)
{
  if(p != NULL)
  {
    overload_MEM_VARLABEL(&p->memvarlabel, ov);
    
    if(valid_VARLABEL(ov) == _TRUE)
    {
      overload_MEM_ANYTHING(&p->memanything, ov);
      overload_MEM_DYNCOLOR(&p->memdyncolor, ov, &ColorScheme);
      overload_MEM_ANYTHING(&p->memimage, ov);
    }
      
    overload_MEM_LSEFUNCT(&p->memlsefunct, ov);
    overload_MEM_ANYPARA(&p->memanypara, ov);
  }
}

void overload_IO(_IO *p, _VARLABEL *ov, _BOOL change_ctrl_type)
{
  if(p->lock_overload == _FALSE)
  {
    p->imoverloaded = *ov;
    p->doimage      = _TRUE;

    if(overload_VARLABEL(&p->variable, ov) == _TRUE)
    {
      if(change_ctrl_type == _TRUE)
      {
        if(p->typ == AS_NUMERIC) // wunsch: applikation (12.04.2018, wessel andre)
        {
          _LSEVARINFO *pvi = VarList.Get(&p->variable);
          if(pvi != NULL)
          {
            if(pvi->vartype == FDATE)
            {
              p->typ = AS_DATE;
            }
          }
          else if(pvi->vartype == FTIME)
          {
            if(pvi->vartype == FDATE)
            {
              p->typ = AS_TIME;
            }
          }
        }
      }
    }
    
    overload_CHKBIT(&p->chkbit, ov);
    overload_IOINFO(&p->info, ov);
    overload_MYSCHEME(&p->stscheme, ov, &StateScheme);
    overload_LSEFUNCT(&p->lsefunct, ov);
    overload_ANYPARA(&p->newbubbletext, ov);

    if(p->is_dyncolor == _TRUE)
    {
      if(valid_VARLABEL(ov) == _TRUE)
      {
        overload_LINKSCHEME(&p->dyncolback, ov, &ColorScheme);      // color background (hatch)
        overload_LINKSCHEME(&p->dyncolfront, ov, &ColorScheme);     // color background (hatch)
        overload_LINKSCHEME(&p->dyncoltl, ov, &ColorScheme);        // color frame (top-left)
        overload_LINKSCHEME(&p->dyncolbr, ov, &ColorScheme);        // color frame (bottom-right)
        overload_LINKSCHEME(&p->dyncoltxt, ov, &ColorScheme);       // color text
      }
    }

    if((p->typ == AS_THEURGE)&&(p->theurge != NULL))
      overload_THEURGE(p->theurge, ov);

    if(p->Obj != NULL)
      p->Obj->Overload(ov);
  }
//  else
//  {
//    Debug("schaas", 1, 1);
//  }
}

void overload_MEM_INPUT(_MEM_INPUT *p, _VARLABEL *ov, _BOOL change_ctrl_type)
{
  _UWORD i;
  _INPUT *pi;

  for(i=0, pi=p->ptr; i<p->no; i++, pi++)
    overload_IO(&pi->io, ov, change_ctrl_type);
}

void overload_MEM_OUTPUT(_MEM_OUTPUT *p, _VARLABEL *ov, _BOOL change_ctrl_type)
{
  _UWORD i;
  _IO    *pio;

  for(i=0, pio=p->ptr; i<p->no; i++, pio++)
    overload_IO(pio, ov, change_ctrl_type);
}

void overload_ANYTHING(_ANYTHING *p, _VARLABEL *ov)
{
  switch(p->list)
  {
    case VARTXT1_TOKEN :
    case VARUNIT : // unit of variable via variable addressiert
    case VARNAME : // name of variable via varno addressiert
    case VARTXT1 : // variablentext 01 via varno addressiert
    case VARTXT2 : // variablentext 02 via varno addressiert
    case VARTXT3 : // variablentext 03 via varno addressiert
    case VARTXT4 : 
    case VARLIMIT_LO :
    case VARLIMIT_HI :
      //if(valid_VARLABEL(ov) == _TRUE)
        overload(&p->no, ov->info[0].value);
      break;
    default      : 
      break;
  }
}

void overload_MEM_ANYTHING(_MEM_ANYTHING *p, _VARLABEL *ov)
{
  _UWORD i;
  _ANYTHING *pany;

  for(i=0, pany=p->ptr; i<p->no; i++, pany++)
    overload_ANYTHING(pany, ov);
}

_BOOL overload_ANYPARA(_ANYPARA *p, _VARLABEL *ov)
{
  _BOOL retcode = _FALSE;

  if(valid_VARLABEL(ov) == _TRUE)
    overload_ANYTHING(&p->anything, ov);
    
  if(overload_MEM_VARLABEL(&p->memvarlabel, ov) == _TRUE)
    retcode = _TRUE;

  return(retcode);
}

_BOOL overload_MEM_ANYPARA(_MEM_ANYPARA *p, _VARLABEL *ov)
{
  _BOOL    retcode = _TRUE;
  _UWORD   i;
  _ANYPARA *pany;

  for(i=0, pany=p->ptr; (i<p->no)&&(retcode==_TRUE); i++, pany++)
    retcode = overload_ANYPARA(pany, ov);

  return(retcode);

}

void overload_PICTEXT(_PICTEXT *p, _VARLABEL *ov)
{
  if(p->lock_overload == _FALSE)
  {
    overload_CHKBIT(&p->chkbit, ov);
    overload_ANYPARA(&p->anypara, ov);
    overload_MYSCHEME(&p->stscheme, ov, &StateScheme);
  }
}

void overload_MEM_PICTEXT(_MEM_PICTEXT *p, _VARLABEL *ov)
{
  _UWORD i;
  _PICTEXT *pit;

  for(i=0, pit=p->ptr; i<p->no; i++, pit++)
    overload_PICTEXT(pit, ov);
}

void overload_POLYGON(_POLYGON *p, _VARLABEL *ov)
{
  overload_CHKBIT(&p->chkbit, ov);
}

void overload_CIRCLE(_CIRCLE *p, _VARLABEL *ov)
{
  overload_CHKBIT(&p->chkbit, ov);
}

void overload_RECTANGLE(_RECTANGLE *p, _VARLABEL *ov)
{
  overload_CHKBIT(&p->chkbit, ov);
}

void overload_MEM_RECTANGLE(_MEM_RECTANGLE *p, _VARLABEL *ov)
{
  _UWORD     i;
  _RECTANGLE *pre;

  for(i=0, pre=p->ptr; i<p->no; i++, pre++)
    overload_RECTANGLE(pre, ov);
}

void overload_MEM_POLYGON(_MEM_POLYGON *p, _VARLABEL *ov)
{
  _UWORD i;
  _POLYGON *pol;

  for(i=0, pol=p->ptr; i<p->no; i++, pol++)
    overload_POLYGON(pol, ov);
}

void overload_MEM_CIRCLE(_MEM_CIRCLE *p, _VARLABEL *ov)
{
  _UWORD i;
  _CIRCLE *pci;

  for(i=0, pci=p->ptr; i<p->no; i++, pci++)
    overload_CIRCLE(pci, ov);
}

void overload_OBJECT(_OBJECT *p, _VARLABEL *ov, _BOOL change_ctrl_type)
{
  if(p->lock_overload == _FALSE)
  {
    // wird external instance überladen, so wird dass object mit dem ergebnis
    // aus ov überladen. ansonsten wird objekt mit ov überladen. (otto koller)

    _VARLABEL *pvl = ov;

    overload_CHKBIT(&p->chkbit, ov);
    if(chk_VARLABEL(&p->loader) == _TRUE) // RamAnd
    {
      if(overload_VARLABEL(&p->loader, ov) == _TRUE)
        pvl = &p->loader;
    }
    else
    {
      p->loader = *ov;
    }

    if(p->SubPicture != NULL)
      p->SubPicture->Overload(pvl, change_ctrl_type);

    overload_MYSCHEME(&p->loader_scheme, ov, &ServerScheme);
   #ifdef USER_DEMAG
    if((OverloadAbsolute == _FALSE)||(p->summary == 0)) // forderung applikation-dpg (johannes zöttl) 14.03.08
   #endif
    overload_MYSCHEME(&p->stscheme, ov, &StateScheme);
  }
}
/*
void overload_OBJECT(_OBJECT *p, _VARLABEL *ov)
{
  overload_CHKBIT(&p->chkbit, ov);

  if(chk_VARLABEL(&p->loader) == _TRUE)  // s/randi
    overload_VARLABEL(&p->loader, ov);   // s/randi
  else                                   // s/randi
    p->loader = *ov;

  if(p->SubPicture != NULL)
    p->SubPicture->Overload(&p->loader);

  overload_MYSCHEME(&p->stscheme, ov, &StateScheme);
}
*/
void overload_MEM_OBJECT(_MEM_OBJECT *p, _VARLABEL *ov, _BOOL change_ctrl_type)
{
  _UWORD  i;
  _OBJECT *po;

  for(i=0, po=p->ptr; i<p->no; i++, po++)
    overload_OBJECT(po, ov, change_ctrl_type);
}

void overload_MEM_BITMAP(_MEM_BITMAP *p, _VARLABEL *ov)
{
  _UWORD  i;
  _BITMAP *pbm;

  for(i=0, pbm=p->ptr; i<p->no; i++, pbm++)
    overload_CHKBIT(&pbm->chkbit, ov);
}

void overload_KARU(_KARU *p, _VARLABEL *ov)
{
  overload_VARLABEL(&p->loader, ov);
  if(p->SubPicture != NULL)
    p->SubPicture->Overload(ov);
}

void overload_MEM_KARU(_MEM_KARU *p, _VARLABEL *ov)
{
  _KARU *pk;
  _UDWORD i;
  
  if(p->no > 0)
  {
    for(i=0, pk=p->ptr; i<p->no; i++, pk++)
      overload_KARU(pk, ov);
  }
}

void overload_MOTION(_MOTION *p, _VARLABEL *ov)
{
  overload_CHKBIT(&p->chkbit, ov);

  if(chk_VARLABEL(&p->loader) == _TRUE)
    overload_VARLABEL(&p->loader, ov);
  else
    p->loader = *ov;

  overload_VARLABEL(&p->varx, ov);
  overload_VARLABEL(&p->vary, ov);

  if(p->SubPicture != NULL)
    p->SubPicture->Overload(ov);

  overload_MYSCHEME(&p->stscheme, ov, &StateScheme);
  overload_MEM_KARU(&p->memkaru, ov);
}

void overload_MEM_MOTION(_MEM_MOTION *p, _VARLABEL *ov)
{
  _UWORD i;
  _MOTION *pmo;

  for(i=0, pmo=p->ptr; i<p->no; i++, pmo++)
    overload_MOTION(pmo, ov);
}

void overload_COL_RECT(_COL_RECT *p, _VARLABEL *ov)
{
  overload_RECTANGLE(&p->rectangle, ov);
  if(valid_VARLABEL(ov) == _TRUE)
  {
    overload_LINKSCHEME(&p->dyncolback, ov, &ColorScheme);         // color background (hatch)
    overload_LINKSCHEME(&p->dyncolfront, ov, &ColorScheme);        // color background (hatch)
    overload_LINKSCHEME(&p->dyncoltl, ov, &ColorScheme);           // color frame (top-left)
    overload_LINKSCHEME(&p->dyncolbr, ov, &ColorScheme);           // color frame (bottom-right)
  }
}

void overload_COL_POLY(_COL_POLY *p, _VARLABEL *ov)
{
  overload_POLYGON(&p->poly, ov);
  if(valid_VARLABEL(ov) == _TRUE)
  {  
    overload_LINKSCHEME(&p->dyncolback, ov, &ColorScheme);         // color background (hatch)
    overload_LINKSCHEME(&p->dyncolfront, ov, &ColorScheme);        // color background (hatch)
    overload_LINKSCHEME(&p->dyncolframe, ov, &ColorScheme);        // color frame
  }
}

void overload_COL_CIRC(_COL_CIRC *p, _VARLABEL *ov)
{
  overload_CIRCLE(&p->circle, ov);
  if(valid_VARLABEL(ov) == _TRUE)
  {
    overload_LINKSCHEME(&p->dyncolback, ov, &ColorScheme);         // color background (hatch)
    overload_LINKSCHEME(&p->dyncolfront, ov, &ColorScheme);        // color background (hatch)
    overload_LINKSCHEME(&p->dyncolframe, ov, &ColorScheme);        // color frame
  }
}

void overload_COL_PICTXT(_COL_PICTXT *p, _VARLABEL *ov)
{
  overload_PICTEXT(&p->btx, ov);
  if(valid_VARLABEL(ov) == _TRUE)
  {
    overload_LINKSCHEME(&p->dyncolback, ov, &ColorScheme);         // color background (hatch)
    overload_LINKSCHEME(&p->dyncolfront, ov, &ColorScheme);        // color background (hatch)
    overload_LINKSCHEME(&p->dyncoltl, ov, &ColorScheme);           // color frame (top-left)
    overload_LINKSCHEME(&p->dyncolbr, ov, &ColorScheme);           // color frame (bottom-right)
    overload_LINKSCHEME(&p->dyncoltxt, ov, &ColorScheme);          // color text
  }
}

void overload_MEM_COL_RECT(_MEM_COL_RECT *p, _VARLABEL *ov)
{
  _UWORD i;
  _COL_RECT *pcr;

  for(i=0, pcr=p->ptr; i<p->no; i++, pcr++)
    overload_COL_RECT(pcr, ov);
}

void overload_MEM_COL_POLY(_MEM_COL_POLY *p, _VARLABEL *ov)
{
  _UWORD i;
  _COL_POLY *pcp;

  for(i=0, pcp=p->ptr; i<p->no; i++, pcp++)
    overload_COL_POLY(pcp, ov);
}

void overload_MEM_COL_CIRC(_MEM_COL_CIRC *p, _VARLABEL *ov)
{
  _UWORD i;
  _COL_CIRC *pcc;

  for(i=0, pcc=p->ptr; i<p->no; i++, pcc++)
    overload_COL_CIRC(pcc, ov);
}

void overload_MEM_COL_PICTXT(_MEM_COL_PICTXT *p, _VARLABEL *ov)
{
  _UWORD i;
  _COL_PICTXT *pct;

  for(i=0, pct=p->ptr; i<p->no; i++, pct++)
    overload_COL_PICTXT(pct, ov);
}

void overload_BUTTON(_BUTTON *p, _VARLABEL *ov)
{
  if(p->lock_overload == _FALSE)
  {
    overload_CHKBIT(&p->chkbit, ov);
    overload_GFUNCT(&p->gfunct, ov);

    if(chk_VARLABEL_Makro(&p->loader)) // Makro, speed: nicht auf true oder false abfragen
      overload_VARLABEL(&p->loader, ov);
    else
      p->loader = *ov;

    if(p->SubPicture != NULL)
      p->SubPicture->Overload(&p->loader);

    if(valid_VARLABEL(ov) == _TRUE)
    {
      overload_LINKSCHEME(&p->dyncolback, ov, &ColorScheme);         // color background (hatch)
      overload_LINKSCHEME(&p->dyncolfront, ov, &ColorScheme);        // color background (hatch)
      overload_LINKSCHEME(&p->dyncoltxt, ov, &ColorScheme);        // color text
      overload_LINKSCHEME(&p->dyncoltl, ov, &ColorScheme);           // color frame (top-left)
      overload_LINKSCHEME(&p->dyncolbr, ov, &ColorScheme);           // color frame (bottom-right)
    }
    
    overload_MYSCHEME(&p->stscheme, ov, &StateScheme);
    overload_VARLABEL(&p->varlabel, ov);
    overload_ANYPARA(&p->text, ov);
    overload_ANYPARA(&p->text1, ov);
    overload_ANYPARA(&p->newbubbletext, ov);
  }
}

void overload_MEM_BUTTON(_MEM_BUTTON *p, _VARLABEL *ov)
{
  _UWORD  i;
  _BUTTON *pb;

  for(i=0, pb=p->ptr; i<p->no; i++, pb++)
    overload_BUTTON(pb, ov);
}

void overload_KEY(_KEY *p, _VARLABEL *ov)
{
  overload_GFUNCT(&p->gfunct, ov);
}

void overload_MEM_KEY(_MEM_KEY *p, _VARLABEL *ov)
{
  _UWORD i;
  _KEY *pk;

  for(i=0, pk=p->ptr; i<p->no; i++, pk++)
    overload_KEY(pk, ov);
}

_BOOL overload_PICTUREMASK(_PICTUREMASK *p, _VARLABEL *ov, _BOOL change_ctrl_type)
{
  if(valid_VARLABEL(ov) == _TRUE)
  {
    overload_MEM_RECTANGLE(&p->memrectangle, ov);
    overload_MEM_POLYGON(&p->mempolygon, ov);
    overload_MEM_CIRCLE(&p->memcircle, ov);
    overload_MEM_PICTEXT(&p->mempictxt, ov);
    overload_MEM_OBJECT(&p->memobject, ov, change_ctrl_type);
    overload_MEM_BITMAP(&p->membitmap, ov);
    overload_MEM_INPUT(&p->meminput, ov, change_ctrl_type);
    overload_MEM_OUTPUT(&p->memoutput, ov, change_ctrl_type);
    overload_MEM_MOTION(&p->memmotion, ov);
    overload_MEM_COL_RECT(&p->colrect, ov);
    overload_MEM_COL_POLY(&p->colpoly, ov);
    overload_MEM_COL_CIRC(&p->colcirc, ov);
    overload_MEM_COL_PICTXT(&p->colpictxt, ov);
    overload_MEM_BUTTON(&p->membutton, ov);
    overload_MEM_KEY(&p->memkey, ov);

   #ifdef OK_ENCLOSE_NAME
    overload_MEM_INPUT_VIRTNAME(&p->meminput, ov, p);
    overload_MEM_OUTPUT_VIRTNAME(&p->memoutput, ov, p);
   #endif
  }

  return _TRUE;
}

void change_overload_MEM_OBJECT(_MEM_OBJECT *p, _VARLABEL *ovist, _VARLABEL *ovsoll)
{
  _UWORD  i;
  _OBJECT *po;

  po = p->ptr;
  for(i=0; i<p->no; i++)
  {
    if(cmp_VARLABEL(&po->loader, ovist) == 0)
      overload_OBJECT(po, ovsoll, _FALSE);
    po += 1;
  }
}

void change_overload_MEM_MOTION(_MEM_MOTION *p, _VARLABEL *ovist, _VARLABEL *ovsoll)
{
  _UWORD  i;
  _MOTION *pm;

  pm = p->ptr;
  for(i=0; i<p->no; i++)
  {
    if(cmp_VARLABEL(&pm->loader, ovist) == 0)
      overload_MOTION(pm, ovsoll);
    pm += 1;
  }
}

void change_overload_MEM_BUTTON(_MEM_BUTTON *p, _VARLABEL *ovist, _VARLABEL *ovsoll)
{
  _UWORD  i;
  _BUTTON *pb;

  pb = p->ptr;
  for(i=0; i<p->no; i++)
  {
    if(cmp_VARLABEL(&pb->loader, ovist) == 0)
      overload_BUTTON(pb, ovsoll);
    pb += 1;
  }
}
/*
void change_overload_MEM_OUTPUT(_MEM_OUTPUT *p, _VARLABEL *ovist, _VARLABEL *ovsoll)
{
  _UWORD i;
  _IO    *pio;

  pio = p->ptr;
  for(i=0; i<p->no; i++)
  {
    if((pio->typ == AS_TBO)&&(pio->info.tbo.list == OBJECTSCHEME))
    {
      if(cmp_VARLABEL(&pio->imoverloaded, ovist) == 0)
      {
        overload_VARLABEL(&pio->imoverloaded, ovsoll);
        pio->Obj->Overload(ovsoll);
//        overload_IO(pio, ovsoll);
      }
    }
    pio += 1;
  }
}
*/
_BOOL change_overload_PICTUREMASK(_PICTUREMASK *p, _VARLABEL *ovist, _VARLABEL *ovsoll)
{
  _BOOL retcode = _TRUE;

  if(valid_VARLABEL(ovsoll) == _TRUE)
  {
    change_overload_MEM_OBJECT(&p->memobject, ovist, ovsoll);
    change_overload_MEM_MOTION(&p->memmotion, ovist, ovsoll);
    change_overload_MEM_BUTTON(&p->membutton, ovist, ovsoll);
//    change_overload_MEM_OUTPUT(&p->memoutput, ovist, ovsoll);
  }

  return(retcode);
}

void summary_overload_MEM_OBJECT(_MEM_OBJECT *p, _UWORD summary, _VARLABEL *ovsoll)
{
  _UWORD  i;
  _OBJECT *po;

  po = p->ptr;
  for(i=0; i<p->no; i++)
  {
    if(summary == po->summary)
    {
//      free_VARLABEL(&po->loader);
      overload_OBJECT(po, ovsoll, _FALSE);
    }
   #ifdef UC_OVERLOAD_DEFAULT_SUMMARY_OBJECT
    else if(po->summary == 0)
    {
      if(po->SubPicture != NULL)
      {
        po->SubPicture->SummaryOverload(summary, ovsoll);
      }
    }
   #endif
    
    po += 1;
  }
}

void summary_overload_MEM_MOTION(_MEM_MOTION *p, _UWORD summary, _VARLABEL *ovsoll)
{
  _UWORD  i;
  _MOTION *pm;

  pm = p->ptr;
  for(i=0; i<p->no; i++)
  {
    if(pm->summary == summary)
      overload_MOTION(pm, ovsoll);
    pm += 1;
  }
}

void summary_overload_MEM_BUTTON(_MEM_BUTTON *p, _UWORD summary, _VARLABEL *ovsoll)
{
  _UWORD  i;
  _BUTTON *pb;

  pb = p->ptr;
  for(i=0; i<p->no; i++)
  {
    if(pb->summary == summary)
      overload_BUTTON(pb, ovsoll);
    pb += 1;
  }
}

void summary_overload_MEM_OUTPUT(_MEM_OUTPUT *p, _UWORD summary, _VARLABEL *ovsoll) //, _BOOL step_into_objscheme)
{
  _UWORD i;
  _IO    *pio;

  pio = p->ptr;
  for(i=0; i<p->no; i++)
  {
    if(pio->summary == summary)
    {
      overload_IO(pio, ovsoll, _FALSE);
//      pio->imoverloaded = *ovsoll;
      if(pio->Obj != NULL)
        pio->Obj->Overload(ovsoll);
    }
   #ifdef UC_OVERLOAD_DEFAULT_SUMMARY_OBJECT
    else if(pio->summary == 0)
    {
      if(pio->Obj != NULL)
        pio->Obj->SummaryOverload(summary, ovsoll);
    }
   #endif
   #ifdef USER_DEMAG
    else
    {
      if(pio->Obj != NULL)
        pio->Obj->SummaryOverload(summary, ovsoll);
    }
   #endif
   
    pio += 1;
  }
}

void summary_overload_MEM_INPUT(_MEM_INPUT *p, _UWORD summary, _VARLABEL *ovsoll)
{
  _UWORD i;
  _IO    *pio;
  _INPUT *pi;

  for(i=0, pi=p->ptr; i<p->no; i++, pi++)
  {
    pio = &pi->io;
    if(pio->summary == summary)
      overload_IO(pio, ovsoll, _FALSE);
  }
}

void summary_overload_MEM_PICTEXT(_MEM_PICTEXT *p, _UWORD summary, _VARLABEL *ovsoll)
{
  _UWORD   i;
  _PICTEXT *ptx;

  ptx = p->ptr;
  for(i=0; i<p->no; i++)
  {
    if(ptx->summary == summary)
      overload_PICTEXT(ptx, ovsoll);
    ptx += 1;
  }
}

void summary_overload_MEM_COL_PICTXT(_MEM_COL_PICTXT *p, _UWORD summary, _VARLABEL *ovsoll)
{
  _UWORD      i;
  _COL_PICTXT *ptx;

  ptx = p->ptr;
  for(i=0; i<p->no; i++)
  {
    if(ptx->btx.summary == summary)
      overload_COL_PICTXT(ptx, ovsoll);
    ptx += 1;
  }
}

_BOOL summary_overload_PICTUREMASK(_PICTUREMASK *p, _UWORD summary, _VARLABEL *ovsoll)
{
  _BOOL retcode = _TRUE;

  if(valid_VARLABEL(ovsoll) == _TRUE)
  {
    summary_overload_MEM_OBJECT(&p->memobject,     summary, ovsoll);
    summary_overload_MEM_MOTION(&p->memmotion,     summary, ovsoll);
    summary_overload_MEM_BUTTON(&p->membutton,     summary, ovsoll);
    summary_overload_MEM_OUTPUT(&p->memoutput,     summary, ovsoll);
    summary_overload_MEM_INPUT(&p->meminput,       summary, ovsoll);
    summary_overload_MEM_PICTEXT(&p->mempictxt,    summary, ovsoll);
    summary_overload_MEM_COL_PICTXT(&p->colpictxt, summary, ovsoll);

//    summary_overload_MEM_COL_RECT(&p->colrect,  summary, ovsoll);
//    summary_overload_MEM_COL_POLY(&p->colpoly,  summary, ovsoll);
//    summary_overload_MEM_COL_CIRC(&p->colcirc,  summary, ovsoll);
  }

  return(retcode);
}

_BOOL overload_LINEINFO(_LINEINFO *p, _VARLABEL *vl)
{
  _BOOL retcode = _FALSE;

  if(overload_CHKBIT(&p->chkbit, vl) == _TRUE)
    retcode = _TRUE;
  if(overload_VARLABEL(&p->value, vl) == _TRUE)
    retcode = _TRUE;
  if(overload_GFUNCT(&p->gfunct, vl) == _TRUE)
    retcode = _TRUE;
  if(overload_MEM_VARLABEL(&p->memvar, vl) == _TRUE)
    retcode = _TRUE;
  if(overload_ANYPARA(&p->anypara, vl) == _TRUE)
    retcode = _TRUE;
  if(overload_MYSCHEME(&p->stscheme, vl, &StateScheme) == _TRUE)
    retcode = _TRUE;

  return(retcode);
}

_BOOL overload_MEM_LINEINFO(_MEM_LINEINFO *p, _VARLABEL *vl)
{
  _UWORD    i;
  _BOOL     retcode = _FALSE;
  _LINEINFO *pli;

  pli = p->ptr;
  for(i=0; i<p->no; i++, pli++)
  {
    if(overload_LINEINFO(pli, vl) == _TRUE)
      retcode = _TRUE;
  }

  return(retcode);
}

_BOOL overload_LSEMENU(_LSEMENU *p, _VARLABEL *vl)
{
  return(overload_MEM_LINEINFO(&p->mem_lineinfo, vl));
}


// ****
// ****


/*
_BOOL overload_merge(_ASCII *dest, _ASCII *name)
{
  _UWORD len, i, nl;
  _ASCII *ptr, chr;
  _BOOL  retcode = _FALSE;

  len = StrLen(dest, sizeof(_ASCII));
  for(i=0,ptr=dest; i<len; i++,ptr++)
  {
    chr = *ptr;
    if((chr == '.')||(chr == '\\'))
    {
      retcode = _TRUE;
      nl      = StrLen(name, sizeof(_ASCII));
      MemMove(&dest[nl], &dest[i], len-i+1);
      MemCpy(dest, name, nl);
      break;
    }
  }

  return(retcode);
}
*/

#ifdef USER_DEMAG
_BOOL summary_overload_PICTUREMASK_absolute(_PICTUREMASK *p, _UWORD summary, _VARLABEL *ovsoll)
{
  // funktion setzt globale variable (OverloadAbsolute), welche in funktion overload_deep abgefragt wird
  // dies wurde mit toni besprochen und nur er wird diese funktion für sonderfall verwenden. --> kein hinweis in doku
  _BOOL retcode;
  _BOOL tmp = OverloadAbsolute;

  OverloadAbsolute = _TRUE;
  retcode = summary_overload_PICTUREMASK(p, summary, ovsoll);
  OverloadAbsolute = tmp;

  return(retcode);
}
#endif

extern "C" _BOOL summary_overload_PICTURE(_UDWORD summary, _VARLABEL *pvl)
{
  _BOOL    retcode = _FALSE;
  _Window  *PWin;
  _Picture *PPic;

  PWin = Root.FindTop();
  if(PWin != NULL)
  {
    PPic = PWin->GetP_Picture();
    if(PPic != NULL)
    {
      retcode = PPic->SummaryOverload(summary, pvl);
    }
  }

  return(retcode);
}

