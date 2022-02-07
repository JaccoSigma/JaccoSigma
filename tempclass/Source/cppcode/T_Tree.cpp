#include "T_IPC.H"
#include "T_NUC.H"

_UDWORD add_TREEITEM(_TREE *p, _UDWORD data0, _UDWORD data1)
{
  _UDWORD   retcode = DEF4BYTE;
  _UDWORD   anz;
  _TREEITEM *pti;
  
  if(p != NULL)
  {
    if(p->no >= p->done)
    {
      anz = (p->no + TREEITEMBLOCK) * sizeof(_TREEITEM);
      if(Memory2((void**)&p->ptr, anz) != 0)
      {
        pti = &p->ptr[p->no];
        anz = TREEITEMBLOCK;
        while(anz)
        {
          init_TREEITEM(pti);
          pti += 1;
          anz -= 1;
        }
        
        p->done += TREEITEMBLOCK;
      }
    }
  
    if(p->no < p->done)
    {
      pti           = &p->ptr[p->no];
      pti->data[0]  = data0;
      pti->data[1]  = data1;
      retcode       = p->no;
      set_flag(pti, TREE_LASTLINE);
      
      if(p->no > 0)
        reset_flag(&p->ptr[p->no-1], TREE_LASTLINE);
      
      p->no        += 1;
    }
  }
  
  return retcode;
}

_BOOL add_SUBTREE(_TREEITEM *pti, _TREE *subtree)
{
  if(pti != NULL)
  {
    if(pti->psubtree == subtree)
      return _FALSE;
  
    if(pti->psubtree != NULL)
      free_TREE(pti->psubtree);
    pti->psubtree = subtree;
    
    return _TRUE;
  }
  
  return _FALSE;
}

_BOOL del_TREEITEM_index(_TREE *p, _UDWORD index)
{
  if(p != NULL)
  {
    if(index < p->no)
    {
      _TREEITEM *pti = &p->ptr[index];
      free_TREEITEM(pti);
      
      _UDWORD size = (p->no - index - 1) * sizeof(_TREEITEM);
      MemMove(pti, pti+1, size);
      p->no -= 1;
      init_TREEITEM(&p->ptr[p->no]);
      
      if(p->no > 0)
        set_flag(&p->ptr[p->no-1], TREE_LASTLINE);
      
      return _TRUE;
    }
  }
  
  return _FALSE;
}

_TREE *del_TREEITEM_sub(_TREE *p, _DWORD *line)
{
  if(p != NULL)
  {
    _TREEITEM *pti = p->ptr;
    for(_UDWORD i=0; i<p->no; i++)
    {
      if((*line) == 0)
      {
        if(del_TREEITEM_index(p, i) == _TRUE)
          return p;
      }
      
      (*line) --;
      if((pti->psubtree != NULL) && (pti->subtreestate == TREE_OPEND))
      {
        _TREE *ph = del_TREEITEM_sub(pti->psubtree, line);
        if(ph != NULL)
          return ph;
      }
      
      pti ++;
    }
  }

  return _FALSE;
}

_TREE *del_TREEITEM(_TREE *p, _DWORD line)
{
  // es wird NULL wenn kein item gelöscht wurde 
  // oder der _TREE in welchem das item gelöscht wurde retouniert
  return del_TREEITEM_sub(p, &line);
}

_TREEITEM *get_TREEITEM_sub(_TREE *p, _DWORD *line)
{
  if(p != NULL)
  {
    _TREEITEM *pti = p->ptr;
    _UDWORD anz = p->no;
    while(anz)
    {
      if((*line) == 0)
        return pti;
        
      (*line) --;
      if((pti->psubtree != NULL) && (pti->subtreestate == TREE_OPEND))
      {
        _TREEITEM *ph = get_TREEITEM_sub(pti->psubtree, line);
        if(ph != NULL)
          return ph;
      }
      
      pti ++;
      anz --;
    }
  }

  return NULL;
    
}

_BOOL find_SCOPELINE_sub(_TREE *p, _DWORD *line, _DWORD *treeline, _DWORD start)
{
  if(p != NULL)
  {
    _TREEITEM *pti = p->ptr;
    _UDWORD   anz  = p->no;
    
    while(anz)
    {
      if((*line) == 0)
      {
        *treeline = start;
        return _TRUE;
      }
       
      (*line) --;
      (*treeline) ++;
      
      if((pti->psubtree != NULL) && (pti->subtreestate == TREE_OPEND))
      {
        if(find_SCOPELINE_sub(pti->psubtree, line, treeline, (*treeline) - 1) == _TRUE)
        {
          return _TRUE;
        }
      }
      
      pti ++;
      anz --;
    }
  }
  
  return _FALSE;
}

_DWORD find_SCOPELINE(_TREE *p, _DWORD line)
{
  _DWORD treeline = 0;
  find_SCOPELINE_sub(p, &line, &treeline, 0);
  return treeline;
}


_TREEITEM *get_TREEITEM(_TREE *p, _DWORD line)
{
  return get_TREEITEM_sub(p, &line);
}

_TREEITEM *get_TREEITEM2_sub(_TREE *p, _UDWORD &depth, _DWORD &line)
{
  if(p != NULL)
  {
    _TREEITEM *pti = p->ptr;
    _UDWORD anz = p->no;
    while(anz)
    {
      anz --;
      
      if(line == 0)
        return pti;
      
      line --;
      if((pti->psubtree != NULL) && (pti->subtreestate == TREE_OPEND))
      {
        depth ++;
        _TREEITEM *ph = get_TREEITEM2_sub(pti->psubtree, depth, line);
        if(ph != NULL)
          return ph;
        depth --;
      }
      
      pti ++;
    }
  }

  return NULL;
    
}

_TREEITEM *get_TREEITEM2(_TREE *p, _UDWORD *depth, _DWORD line)
{
  *depth = 0;
  return get_TREEITEM2_sub(p, *depth, line);
}

_BOOL get_TREESCOPE_sub(_TREESCOPE *scope, _TREE *p, _DWORD &line)
{
  if(p != NULL)
  {
    _TREEITEM *pti = p->ptr;
    _UDWORD anz = p->no;
    while(anz)
    {
      anz --;
      
      if(line == 0)
      {
        scope->ptr[scope->no] = pti;
        scope->no ++;
        return _TRUE;
      }
      
      line --;
      if((pti->psubtree != NULL) && (pti->subtreestate == TREE_OPEND))
      {
        scope->ptr[scope->no] = pti;
        scope->no ++;
        if(get_TREESCOPE_sub(scope, pti->psubtree, line) == _TRUE)
          return _TRUE;
        scope->no --;
      }
      
      pti ++;
    }
  }
  
  return _FALSE;
}

_BOOL get_TREESCOPE(_TREESCOPE *scope, _TREE *p, _DWORD line)
{
  init_TREESCOPE(scope);
  return get_TREESCOPE_sub(scope, p, line);
}

_BOOL scan_TREE_sub(_TREESCOPE *scope, _TREE *p, _UDWORD flag, _FBOOL_PT_PT_PT pfkt, void *puser1, void *puser2)
{
  if(p != NULL)
  {
    _TREEITEM *pti = p->ptr;
    _UDWORD anz = p->no;
    while(anz)
    {
      anz --;
      if(pti->flags & flag)
      {
        scope->ptr[scope->no] = pti;
        scope->no ++;
        if(pfkt(scope, puser1, puser2) == _FALSE)
          return _FALSE;
        scope->no --;
      }
      
      if(pti->psubtree != NULL)
      {
        scope->ptr[scope->no] = pti;
        scope->no ++;
        if(scan_TREE_sub(scope, pti->psubtree, flag, pfkt, puser1, puser2) == _FALSE)
          return _FALSE;
        scope->no --;
      }
      
      pti ++;
    }
    return _TRUE;
  }
  return _FALSE;
}  

_BOOL scan_TREE(_TREE *p, _UDWORD flag, _FBOOL_PT_PT_PT pfkt, void *puser1, void *puser2)
{
  _TREESCOPE scope;
  init_TREESCOPE(&scope);
  return scan_TREE_sub(&scope, p, flag, pfkt, puser1, puser2);
}

_BOOL is_action_TREEITEM(_TREEITEM *pti)
{
  if(pti != NULL)
  {
    if(pti->psubtree != NULL)
    {
      return _TRUE;
    }
  }
  
  return _FALSE;
}

_BOOL is_action_TREE(_TREE *p, _DWORD line)
{
  return is_action_TREEITEM(get_TREEITEM(p, line));
}

_UDWORD visuallines_TREE(_TREE *p)
{
  _UDWORD retcode = 0;

  if(p != NULL)
  {
    _TREEITEM *pti = p->ptr;
    for(_UDWORD i=0; i<p->no; i++, pti++)
    {
      if(pti->psubtree != NULL)
      {
        if(pti->subtreestate == TREE_OPEND)
        {
          retcode += visuallines_TREE(pti->psubtree);
        }
        else
        {
          pti->psubtree->visuallines = 0;
        }
      }
    }
    
    retcode += p->no;
    p->visuallines = retcode;
  }
  
  return retcode;
}

_UDWORD markedlines_TREE(_TREE *p)
{
  _UDWORD retcode = 0;
  if(p!= NULL)
  {
    _TREEITEM *pti = p->ptr;
    for(_UDWORD i=0; i<p->no; i++, pti++)
    {
      if(pti->psubtree != NULL)
        retcode += markedlines_TREE(pti->psubtree);
        
      if(pti->flags & TREE_MARKED)
        retcode += 1;
    }
    p->markedlines = retcode;
  }
  return retcode;
}

void set_flag(_TREEITEM *pti, _UDWORD attrib)
{
  pti->flags |= attrib;
}

void reset_flag(_TREEITEM *pti, _UDWORD attrib)
{
  pti->flags &= ~attrib;
}

_BOOL do_action_TREEITEM(_TREEITEM *pti)
{
  if(pti != NULL)
  {
    if(pti->psubtree != NULL)
    {
      pti->subtreestate = (pti->subtreestate == TREE_OPEND)? TREE_CLOSED : TREE_OPEND;
      return _TRUE;
    }
  }
  
  return _FALSE;
}

_BOOL do_action_TREE(_TREE *p, _DWORD line)
{
  return do_action_TREEITEM(get_TREEITEM(p, line));
}

_KOORD out_TREE(_TREE *p, _ROOM *r, _DWORD line, _COLOR col, _BOOL isnode, _UWORD treesize)
{
  _KOORD x = 0;
  
  if(treesize > 3)
  {
    _UDWORD   depth = 0;
    _TREEITEM *pti = get_TREEITEM2(p, &depth, line);
  
    if(pti != NULL)
    {
      _UDWORD i, sign;
      _KOORD  x0, y0, y, ye;
      _BOOL   lastline = (p->visuallines == (line + 1))? _TRUE : _FALSE;

      y  = (r->xy1.y + r->xy2.y) / 2;
      x  = r->xy1.x;
      ye = r->xy2.y;
    
      if(lastline == _TRUE) // is lastline
        ye = y;
    
      // vertical lines
      for(i=0; i<depth; i++)
      {
        x += treesize;
        Line(x, r->xy1.y, x, ye, T_DOTTED|T_COPY, col);
      }

      if(pti->flags & TREE_LASTLINE)
      {
        ye = y;
      }
      
      x += treesize;
      Line(x, r->xy1.y, x, ye, T_DOTTED|T_COPY, col);

      // horizontal line
      Line((lastline == _TRUE)? r->xy1.x+treesize : x, y, x+treesize, y, T_DOTTED|T_COPY, col);
    
      if((pti->psubtree != NULL)||(isnode == _TRUE))
      {
        sign = (pti->subtreestate == TREE_CLOSED)? '+' : '-';

        x0 = x - (treesize / 2);
        y0 = y - (treesize / 2);
        _COLOR sub = GetBackColor_Makro(col);
        if(sub != INVISIBLE)
          Bar(x0, y0, x0 + (treesize-1), y0 + (treesize-1), T_SOLID|T_COPY, sub);
        Rectangle(x0, y0, x0 + (treesize-1), y0 + (treesize-1), T_DOTTED|T_COPY, col);

        _KOORD ty = y0 + (treesize / 2) - 1;
        Line(x0+2, ty, x0 + (treesize-3), ty, T_SOLID|T_COPY, col);
        ty += 1;
        Line(x0+2, ty, x0 + (treesize-3), ty, T_SOLID|T_COPY, col);

        if(sign == '+')
        {
          _KOORD tx = x0 + (treesize / 2) - 1;
          Line(tx, y0+2, tx, y0 + (treesize-3), T_SOLID|T_COPY, col);
          tx += 1;
          Line(tx, y0+2, tx, y0 + (treesize-3), T_SOLID|T_COPY, col);
        }
//        else // sieht leider doof aus
//        {
//          Line(x+treesize, y, x+treesize, r->xy2.y, T_DOTTED|T_COPY, col);          
//        }
      }

      x += treesize;
    }
  }
  
  return(x);
}
