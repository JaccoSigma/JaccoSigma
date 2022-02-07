#include "T_IPC.H"
#include "T_NUC.H"

void _MenuList::Init(void)
{
  LseLabel.Init();
  init_MEM_LSEMENU(&MemLseMenu);
  ComboBoxMenueIndex   = DEFSCOPE;
  DictionaryMenueIndex = DEFSCOPE;
}

void _MenuList::Free(void)
{
  LseLabel.Free();
  free_MEM_LSEMENU(&MemLseMenu);
  Init();
}

_BOOL _MenuList::Read(_File *File)
{
  _BOOL   retcode = _FALSE;
  _UWORD  i;
  _UWORD  tmp;
  _UDWORD size;

  if(File->GetError() == _FALSE)
  {
    if(File->Read(&tmp, sizeof(_UWORD)) == _TRUE)
    {
      size = tmp * sizeof(_LSEMENU);
      if(Memory2((void**)&MemLseMenu.ptr, size) != 0)
      {
        retcode = _TRUE;
        MemLseMenu.no = tmp;
        for(i=0; i<tmp; i++)
        {
          File->Read_LSEMENU(&MemLseMenu.ptr[i]);
          MemLseMenu.ptr[i].lsemenuindex = i;
        }
      }

     #if COMPILER_VERSION >= 33
      LseLabel.Read(File, _FALSE);
     #endif
    }

    if(File->GetError() == _TRUE)
      retcode = _FALSE;
  }

  return(retcode);
}

_BOOL _MenuList::Load(_ASCII *dpne)
{
  _File  File;
  _BOOL  retcode = _FALSE;

  Free();

  if(File.Open(LOAD, dpne) == _TRUE)
  {
    if(File.ReadHeader() == _TRUE)
    {
      retcode = Read(&File);
    }

    File.Close();

    if(retcode == _FALSE)
      Free();
  }

  return(retcode);
}

_UWORD _MenuList::GetNo(void)
{
  return(MemLseMenu.no);
}

_LSEMENU *_MenuList::Get_LSEMENU(_UWORD no)
{
  if(no < MemLseMenu.no)
    return(&MemLseMenu.ptr[no]);

  return(NULL);
}

_CHAR *_MenuList::GetIoText(_UWORD menu_no, _DWORD line_no, _VARLABEL *ovl)
{
  _CHAR     *retcode = NULL;
  _UWORD    i;
  _DWORD    value;
  _LINEINFO *ptr;
  _LSEMENU  *p;
  _LSEMENU  tmp;
  _BOOL     tmpstate = _FALSE;

  p = Get_LSEMENU(menu_no);

  if(ovl != NULL)
  {
    if(valid_VARLABEL(ovl) == _TRUE)
    {
      tmpstate = _TRUE;
      init_LSEMENU(&tmp);
      if(copy_LSEMENU(&tmp, p) == _TRUE)
      {
        p = &tmp;
        overload_LSEMENU(&tmp, ovl);
      }
      else
      {
        free_LSEMENU(&tmp);
      }
    }
  }

  if(p != NULL)
  {
    ptr = p->mem_lineinfo.ptr;

    for(i=0; i<p->mem_lineinfo.no; i++)
    {
      if(state_LINEINFO(ptr) != STATE_INVISIBLE)
      {
        if(VarList.GetValue(&ptr->value, &value, _TRUE) == _FALSE)
          value = i;

        if(value == line_no)
        {
          retcode = get_ANYPARA(&ptr->anypara, &p->TextPuffer);
          break;
        }
      }
      ptr++;
    }
  }

  if(tmpstate == _TRUE)
    free_LSEMENU(&tmp);

  return(retcode);
}

_UWORD _MenuList::GetRealNo(_LSEMENU *p, _UWORD line_no)
{
  _UWORD    i;
  _LINEINFO *ptr;

  if(p != NULL)
  {
    ptr = p->mem_lineinfo.ptr;

    for(i=0; i<p->mem_lineinfo.no; i++)
    {
      if(state_LINEINFO(ptr) != STATE_INVISIBLE)
      {
        if(line_no == 0)
          return(i);
        line_no --;
      }
      ptr++;
    }
  }

  return(0xFFFF);
}

_CHAR *_MenuList::GetLineText(_UDWORD *pstate_scheme_result, _LSEMENU *p, _UWORD line_no)
{
  _UWORD    no;
  _LINEINFO *ptr;
  _CHAR     *retcode;

  if(pstate_scheme_result != NULL)
    *pstate_scheme_result = STATE_INVISIBLE;

  if(p != NULL)
  {
    no = GetRealNo(p, line_no);
    if(no < p->mem_lineinfo.no)
    {
      ptr     = &p->mem_lineinfo.ptr[no];
      
      if(pstate_scheme_result != NULL)
        *pstate_scheme_result = state_LINEINFO(ptr);
      
      retcode = get_ANYPARA(&ptr->anypara, &p->TextPuffer);

      return(retcode);
    }
  }

  return(NULL);
}

_LINEINFO *_MenuList::Get_LINEINFO(_LSEMENU *p, _UWORD line_no)
{
  _UWORD    no;

  if(p != NULL)
  {
    no = GetRealNo(p, line_no);
    if(no < p->mem_lineinfo.no)
      return(&p->mem_lineinfo.ptr[no]);
  }

  return(NULL);
}

_BOOL _MenuList::OutIO(_IO *pio, _DWORD value)
{
  _BOOL retcode = _FALSE;
  _CHAR *txt;
  _CHAR str[1];

  if(pio->info.pcombo != NULL)
  {
    txt = GetText_COMBOBOX(pio->info.pcombo, value);
  }
  else
  {
    txt = GetIoText(pio->info.tbo.no, value, &pio->imoverloaded);
  }
  
  if(txt == NULL)
  {
    str[0] = 0;
    txt = str;
  }
  
  //if(txt != NULL)
  //{
  OutMultiTextIconAlign(txt, 0, &pio->space, pio->font, pio->attrib, pio->frametype, pio->coltxt, pio->colback, pio->colframe, sizeof(_CHAR));
  retcode = _TRUE;
 #ifdef UC_ROTATE
  RefreshRectangle(&pio->space);
 #endif
  //}

//  if((retcode == _FALSE) && (pio->colback != MakeColor_Makro(INVISIBLE, INVISIBLE)))
//  {
//    Bar(pio->space.xy1.x, pio->space.xy1.y, pio->space.xy2.x, pio->space.xy2.y, T_COPY|T_SOLID, pio->colback);
//   #ifdef UC_ROTATE
//    RefreshRectangle(&pio->space);
//   #endif
//  }

  return(retcode);
}

_BOOL _MenuList::Get(_MENUINFO *pu, _UWORD no) // THR will use it
{
  _BOOL    retcode = _FALSE;
  _LSEMENU *p;

  free_MENUINFO(pu);

  p = Get_LSEMENU(no);
  if(p != NULL)
  {
    retcode = copy_MENUINFO(pu, &p->info);

    if(retcode == _FALSE)
      free_MENUINFO(pu);
  }

  return(retcode);
}

_UWORD _MenuList::CalculateLines(_LSEMENU *p)
{
  _UWORD i, retcode = 0;

  if(p != NULL)
  {
    for(i=0; i<p->mem_lineinfo.no; i++)
    {
      if(state_LINEINFO(&p->mem_lineinfo.ptr[i]) != STATE_INVISIBLE)
        retcode ++;
    }
  }

  return(retcode);
}

_UWORD _MenuList::AktLineNo(_DWORD valency, _LSEMENU *p)
{
  _LINEINFO *ptr;
  _UWORD    i, line;
  _DWORD    value;

//  p = Get_LSEMENU(no);

  if(p != NULL)
  {
    ptr = p->mem_lineinfo.ptr;

    for(i=0, line=0; i<p->mem_lineinfo.no; i++)
    {
      if(state_LINEINFO(ptr) != STATE_INVISIBLE)
      {
        if(VarList.GetValue(&ptr->value, &value, _TRUE) == _FALSE)
          value = line;

        if(value == valency)
          return(line);
        line ++;
      }
      ptr++;
    }
  }

  return(0);
}

_UDWORD _MenuList::Add(_LSEMENU *pmen, _ASCII *name)
{
  _UDWORD no;
  _UDWORD retcode = DEFSCOPE;
  _UDWORD size = (MemLseMenu.no + 1) * sizeof(_LSEMENU);
  
  if((pmen != NULL)&&(name != NULL))
  {
    if(name[0] != 0)
    {
      if(Memory2((void**)&MemLseMenu.ptr, size) != 0)
      {
        if(LseLabel.Seek(&no, name) == _FALSE)
        {
          LseLabel.Add(name);
          MemLseMenu.ptr[MemLseMenu.no] = *pmen;
          MemLseMenu.ptr[MemLseMenu.no].lsemenuindex = MemLseMenu.no;
          retcode = MemLseMenu.no;
          MemLseMenu.no += 1;
        }
      }
    }
  }
  
  return retcode;
}

_BOOL _MenuList::DesignMenu(_UDWORD menuno, _DWORD linewidth, _FONT font, _ATTRIB attrib, _COMBOINFO *pci)
{
  _LSEMENU *pmen = Get_LSEMENU(menuno);
  _UWORD xx, yy;
  
  if(pmen != NULL)
  {
    Font.Size(font, &xx, &yy);
    pmen->info.attrib      = T_COPY | T_SOLID | T_MIDBOUND | (attrib & T_HORIZBOUND);
    pmen->info.font        = font;
    pmen->info.width       = linewidth;  // line height in pixel
    pmen->info.line_height = (pci->lineheight == 0)? yy : pci->lineheight;
    pmen->info.height      = (pmen->mem_lineinfo.no < pci->linesperpage)? pmen->mem_lineinfo.no : pci->linesperpage;
    pmen->info.frametype   = pci->frame;
                 //_COLOR       text_color;   // line color
                 //_COLOR       bar_color;    // bar color
                 //_COLOR       frame_color;  // frame color
                 //_UWORD       basement;     // height of basement (freespace)
  }

  return _TRUE;
}

_BOOL _MenuList::DesignEnumerationMenu(_UDWORD menuno, _IO *pio, _COMBOINFO *pci)
{
  _COMBOINFO ci;
  _DWORD     linewidth = pio->space.xy2.x - pio->space.xy1.x + 1;
  
  if(pci == NULL)
  {
    init_COMBOINFO(&ci);
    ci.frame        = pio->frametype;
    ci.lineheight   = 0;
    ci.linesperpage = 5;
  }
  else
  {
    ci = *pci;
  }
  
  if(ci.frame == 0xFF) // defaultwert -> frame von io für drop-down-list verwenden
    ci.frame = pio->frametype;

  if((linewidth > 1600) || (linewidth < 8))
    linewidth = 100; // zur schicherheit

  return DesignMenu(menuno, linewidth, pio->font, pio->attrib, &ci);
}

_BOOL _MenuList::MakeEnumerationMenu(_LSEMENU *p, _UDWORD varno)
{
  _BOOL     retcode = _FALSE;
  _MEM_NO   memo;
  _LINEINFO *pli;
  _UWORD    i;
  
  init_MEM_NO(&memo);
  if(VarList.GetEnumItems(&memo, varno) == _TRUE)
  {
    init_LSEMENU(p);
    if(Memory2((void**)&p->mem_lineinfo.ptr, memo.no * sizeof(_LINEINFO)) != 0)
    {
      p->mem_lineinfo.no = memo.no;
              
      for(i=0, pli=p->mem_lineinfo.ptr; i<memo.no; i++, pli++)
      {
        init_LINEINFO(pli);
        pli->anypara.anything.list = VARTXT1_TOKEN;
        pli->anypara.anything.no = memo.ptr[i];
        pli->value.no = 1;
        pli->value.info[0].state = VAR_VAL;
        pli->value.info[0].value = pli->anypara.anything.no;
      } 
      
      free_MEM_NO(&memo);
      retcode = _TRUE;
    }
  }

  if(retcode == _FALSE)
    free_LSEMENU(p);

  return retcode;
}

_UDWORD _MenuList::MakeComboBoxMenu(_COMBOBOX *pcb)
{ 
  if(ComboBoxMenueIndex >= MemLseMenu.no)
  {
    _LSEMENU tmp;
    init_LSEMENU(&tmp);
    ComboBoxMenueIndex = Add(&tmp, (_ASCII*)"_@ComboBox");
  }
  
  if(ComboBoxMenueIndex < MemLseMenu.no)
  {
    _LSEMENU *pmen = &MemLseMenu.ptr[ComboBoxMenueIndex];
    free_LSEMENU(pmen);

    if(Memory2((void**)&pmen->mem_lineinfo.ptr, (pcb->no) * sizeof(_LINEINFO)) != 0)
    {
      pmen->lsemenuindex    = ComboBoxMenueIndex;
      pmen->mem_lineinfo.no = pcb->no;
  
      _LINEINFO     *pli = pmen->mem_lineinfo.ptr;
      _COMBOBOXLINE *pci = pcb->ptr;
      for(_UDWORD i=0; i<pcb->no; i++, pli++, pci++)
      {
        init_LINEINFO(pli);
        pli->value.no = 1;
        pli->value.info[0].value   = pci->value;
        pli->value.info[0].state   = CONST_VAL;
      
        pli->anypara.anything.list = LOCTXT;
        pli->anypara.anything.no   = pci->index;
      }

      if(pmen->TextPuffer.Copy(&pcb->TextPuffer) == _TRUE)
      {
        pmen->info.text_color = pcb->color;   // line color
        return ComboBoxMenueIndex;
      }
    }
  }
  return DEFSCOPE;
}

_CHAR *GetTextFromMenu(_UWORD menu_no, _DWORD value)
{
  return(MenuList.GetIoText(menu_no, value, NULL));
}

/*
_ComplexText *_MenuList::Get_ComplexText(_UWORD no)
{
  if(no < MemMenu.no)
    return(&MemMenu.Cplx[no]);

  return(NULL);
}

*/


_BOOL _MenuList::DesignDictionaryMenu(_UDWORD menuno, _IO *pio, _UDWORD linewidth)
{
  _COMBOINFO ci;
  
  init_COMBOINFO(&ci);
  ci.frame        = pio->frametype;
  ci.lineheight   = 0;
  ci.linesperpage = 7;
  
  if(ci.frame == 0xFF) // defaultwert -> frame von io für drop-down-list verwenden
  {
    ci.frame = pio->frametype;
  }

  if((linewidth > 1600) || (linewidth < 8))
  {
    linewidth = 100; // zur schicherheit
  }

  return DesignMenu(menuno, linewidth, pio->font, T_LEFTBOUND, &ci);
}


_UDWORD _MenuList::MakeDictionaryMenu(_CHAR **pptxt, _UDWORD no)
{ 
  if(no > 0)
  {
    if(DictionaryMenueIndex >= MemLseMenu.no)
    {
      _LSEMENU tmp;
      init_LSEMENU(&tmp);
      DictionaryMenueIndex = Add(&tmp, (_ASCII*)"_@Dictionary");
    }

    if(DictionaryMenueIndex < MemLseMenu.no)
    {
      _LSEMENU *pmen = &MemLseMenu.ptr[DictionaryMenueIndex];
      free_LSEMENU(pmen);
    
      if(Memory2((void**)&pmen->mem_lineinfo.ptr, no * sizeof(_LINEINFO)) != 0)
      {
        pmen->lsemenuindex    = DictionaryMenueIndex;
        pmen->mem_lineinfo.no = no;
    
        _LINEINFO *pli = pmen->mem_lineinfo.ptr;
        for(_UDWORD i=0; i<no; i++, pli++)
        {
          init_LINEINFO(pli);
          pli->anypara.anything.list = LOCTXT;
          pli->anypara.anything.no   = i;
          
//          _CHAR txt[20];
//          StrCpy(txt, 2, "Dummy ", 1);
//          DToA(&txt[5], i, 0x800, 2);
          pmen->TextPuffer.Add(pptxt[i], 2);
        }

        return DictionaryMenueIndex;
      }
    }
  }
  
  return DEFSCOPE;
}

_BOOL _MenuList::IsDictionaryMenue(_LSEMENU *p)
{
  if(p != NULL)
  {
    if(DictionaryMenueIndex != DEFSCOPE)
    {
      if(p->lsemenuindex == DictionaryMenueIndex)
      {
        return _TRUE;
      }
    }
  }
  
  return _FALSE;
}

_BOOL _MenuList::IsDictionaryMenue(_WHOAMI *p)
{
  if(DictionaryMenueIndex != DEFSCOPE)
  {
    if((p->ima == IMA_POPUP) && (p->no == DictionaryMenueIndex))
    {
      return _TRUE;
    }
  }
  return _FALSE;
}

