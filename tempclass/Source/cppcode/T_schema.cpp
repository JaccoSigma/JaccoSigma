#include "T_IPC.H"
#include "T_NUC.H"

void _Scheme::Init(void)
{
  init_MEM_SCHEME(&MemScheme);
  LseLabel.Init();
}

void _Scheme::Free(void)
{
  free_MEM_SCHEME(&MemScheme);
  LseLabel.Free();
}

void change_op_in_SINGLESCHEME(_SINGLESCHEME *p, _UWORD ist, _UWORD soll)
{
  if(p->op == ist)
    p->op = soll;
}

void change_op_in_MEM_SINGLESCHEME(_MEM_SINGLESCHEME *p, _UWORD ist, _UWORD soll)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    change_op_in_SINGLESCHEME(&p->ptr[i], ist, soll);
}

void _Scheme::Change(_UWORD ist, _UWORD soll)
{
  _UWORD i;

  for(i=0; i<MemScheme.no; i++)
    change_op_in_MEM_SINGLESCHEME(&MemScheme.ptr[i].memsinglescheme, ist, soll);
}

_BOOL _Scheme::Read(_File *File, _BOOL inclusivlink, _BOOL value4byte)
{
  // inclusivlink ..... varlabel (link) im schema enthalten
  // value4byte ....... value information 4 byte anstelle von 2 byte (zB.: textschema)

  _BOOL retcode = _TRUE;

  Init();

  File->Read_MEM_SCHEME(&MemScheme, inclusivlink, value4byte);
 #if COMPILER_VERSION >= 35
  LseLabel.Read(File, _FALSE);
 #endif

  if(File->GetError() == _TRUE)
  {
    Free();
    retcode = _FALSE;
  }

  return(retcode);
}

void _Scheme::SetVariableResult(void)
{
  _UDWORD i;
  _SCHEME *ph = MemScheme.ptr;
  for(i=0; i<MemScheme.no; i++)
  {
    ph->memsinglescheme.result_is_varno = _TRUE;
    ph += 1;
  }
}

_BOOL _Scheme::Load(_ASCII *dpne, _BOOL inclusivlink, _BOOL value4byte)
{
  // inclusivlink ..... varlabel (link) im schema enthalten
  // value4byte ....... value information 4 byte anstelle von 2 byte (zB.: textschema)

  _File File;
  _BOOL retcode = _FALSE;

  Free();

  File.Open(LOAD, dpne);
  if(File.ReadHeader() == _TRUE)
  {
    retcode = Read(&File, inclusivlink, value4byte);
  }
  File.Close();

  return(retcode);
}

_BOOL _Scheme::CallSub(_UDWORD *dst, _MEM_SINGLESCHEME *ps, _VARLABEL *pl, _BOOL cheat)
{
  _RESULT       result;
  _VARIABLE     variable;
  _BOOL         st;
  _UWORD        no;
  _IVALUE       value;
  _SINGLESCHEME *pss;
// #ifdef UC_MEMORIZE_PLCDATA 
//  _BOOL         wait4plc = (cheat == _FALSE)? _TRUE : _FALSE;
// #else
  _BOOL         wait4plc = _TRUE;
// #endif

  if(VarList.GetSystemData(&result, &variable, pl, wait4plc, _FALSE, cheat) == _TRUE)
  {
    value = result.value;
    pss   = ps->ptr;
    no    = ps->no;

    while(no)
    {
      no--;
      if(VarList.GetSystemData(&result, &variable, &pss->upto, wait4plc, _FALSE, cheat) == _TRUE)
      {
        switch(pss->op)
        {
          case SOP_SMALLEQUAL  : st = (value <= result.value)? _TRUE : _FALSE; break;
          case SOP_EQUAL       : st = (value == result.value)? _TRUE : _FALSE; break;
          case SOP_BIGGEREQUAL : st = (value >= result.value)? _TRUE : _FALSE; break;
          case SOP_BIGGER      : st = (value > result.value)? _TRUE : _FALSE; break;
          case SOP_NOTEQUAL    : st = (value != result.value)? _TRUE : _FALSE; break;
          default              : st = (value < result.value)? _TRUE : _FALSE; break;
        }

        if(st == _TRUE)
        {
          no   = 0;
          *dst = pss->value;
        }

/*
        if(pss->flag == 0)
        {
          if(value < result.value)
          {
            no   = 0;
            *dst = pss->value;
          }
        }
        else
        {
          if(value <= result.value)
          {
            no   = 0;
            *dst = pss->value;
          }
        }
*/
      }

      pss += 1;
    }
    
    return _TRUE;
  }

  return _FALSE;
}

_BOOL _Scheme::Call(_UDWORD *dst, _MYSCHEME *pd, _VARLABEL *link, _UDWORD def, _BOOL cheat)
{
  *dst = def;

  if(&StateScheme == this)
  {
    cheat = _TRUE;
  }

  if(pd->schemeno < MemScheme.no)
  {
    if(link != NULL)
    {
      //if(chk_VARLABEL(link) == _FALSE)
      if(!chk_VARLABEL_Makro(link)) // Makro, speed: nicht auf true oder false abfragen 
        link = NULL;
    }
    
    _SCHEME *ps = &MemScheme.ptr[pd->schemeno];
    if(link == NULL)
     link = &ps->link;

    //if(chk_VARLABEL(link) == _TRUE)
    if(chk_VARLABEL_Makro(link)) // Makro, speed: nicht auf true oder false abfragen 
    {
      return CallSub(dst, &ps->memsinglescheme, link, cheat);
    }
    return _FALSE;
  }
  else if(pd->overload.memsinglescheme.no > 0)
  {
    if(link != NULL)
    {
      //if(chk_VARLABEL(link) == _FALSE)
      if(!chk_VARLABEL_Makro(link)) // Makro, speed: nicht auf true oder false abfragen 
        link = NULL;
    }
    
    if(link == NULL)
     link = &pd->overload.link;

    //if(chk_VARLABEL(link) == _TRUE)
    if(chk_VARLABEL_Makro(link)) // Makro, speed: nicht auf true oder false abfragen 
    {
      return CallSub(dst, &pd->overload.memsinglescheme, link, cheat);
    }
    return _FALSE;
  }

  return _TRUE;
}

_BOOL _Scheme::Call(_UDWORD *dst, _LINKSCHEME *pd, _UDWORD def, _BOOL cheat)
{
  if((pd->myscheme.schemeno < MemScheme.no)||(pd->myscheme.overload.memsinglescheme.no > 0)) // speed
    return(Call(dst, &pd->myscheme, &pd->varlabel, def, cheat));
    
  *dst = def;
  return _TRUE;
}

_BOOL _Scheme::Get(_SCHEME **pps, _UWORD no)
{
  if(no < MemScheme.no)
  {
    *pps = &MemScheme.ptr[no];
    return _TRUE;
  }

  *pps = NULL;
  return _FALSE;
}

_UWORD _Scheme::GetNo(void)
{
  return(MemScheme.no);
}

_BOOL _Scheme::Valid(_LINKSCHEME *p)
{
//  _BOOL     retcode = _FALSE;
//  _SCHEME   *ps;
//  _VARLABEL *link;

  if(p->myscheme.schemeno < MemScheme.no)
  {
    _VARLABEL *link = &p->varlabel;
    //if(chk_VARLABEL(link) == _FALSE)
    if(!chk_VARLABEL_Makro(link)) // Makro, speed: nicht auf true oder false abfragen
      link = NULL;
      
//    ps = &MemScheme.ptr[p->myscheme.schemeno];
//    if(link == NULL)
//      link = &ps->link;

    if(link == NULL)
      link = &MemScheme.ptr[p->myscheme.schemeno].link;

    //return chk_VARLABEL(link);
    return(chk_VARLABEL_Makro(link)? _TRUE : _FALSE); // Makro, speed
  }
  else if(p->myscheme.overload.memsinglescheme.no > 0)
  {
    //return chk_VARLABEL(&p->myscheme.overload.link);
    return (chk_VARLABEL_Makro(&p->myscheme.overload.link)? _TRUE : _FALSE); // Makro, speed
  }

  return _FALSE;
}

_BOOL _Scheme::PrepaireSub(_SCHEME *p, _UWORD entryno)
{
  _UDWORD size;
  _UWORD  i;

  init_SCHEME_Makro(p);

  size = entryno * sizeof(_SINGLESCHEME);
  if(Memory2((void**)&p->memsinglescheme.ptr, size) != 0)
  {
    for(i=0; i<entryno; i++)
      init_SINGLESCHEME(&p->memsinglescheme.ptr[i]);
    p->memsinglescheme.no   = entryno;
    return _TRUE;
  }

  return _FALSE;
}

_BOOL _Scheme::FinishSub(_SCHEME *p, _UWORD entryno)
{
  if(entryno < p->memsinglescheme.no)
  {
    for(_UWORD i=entryno; i<p->memsinglescheme.no; i++)
      free_SINGLESCHEME(&p->memsinglescheme.ptr[i]);

    Memory2((void**)&p->memsinglescheme.ptr, entryno * sizeof(_SINGLESCHEME));

    p->memsinglescheme.no = entryno;
  }
  else if(entryno > p->memsinglescheme.no)
  {
    return _FALSE;
  }

  return _TRUE;
}

_BOOL _Scheme::InsertSub_MEM_NO(_SCHEME *p, _UBYTE op, _UDWORD upto, _DWORD value)
{
  _BOOL         retcode = _FALSE;
  _SINGLESCHEME *pss;

  if(upto < p->memsinglescheme.no)
  {
    retcode = _TRUE;
    pss = &p->memsinglescheme.ptr[upto];
    pss->upto.no            = 1;
    pss->upto.info[0].state = CONST_VAL;
    pss->upto.info[0].value = upto;
    pss->op                 = op;
    pss->value              = value;
  }

  return(retcode);
}

_BOOL _Scheme::Insert_MEM_NO(_MEM_NO *p, _UBYTE op, _UWORD index)
{
  _BOOL  retcode = _TRUE;
  _UWORD i;

  for(i=0; i<p->no; i++)
  {
    if(i < MemScheme.no)
    {
      if(InsertSub_MEM_NO(&MemScheme.ptr[i], op, index, p->ptr[i]) == _FALSE)
        retcode = _FALSE;
    }
  }

  return(retcode);
}

void transform_scheme(_MEM_SINGLESCHEME *p)
{
  _UWORD i;
  _UWORD col;
  
  _SINGLESCHEME *ps;
  for(i=0, ps=p->ptr; i<p->no; i++, ps++)
  {
    col = ps->value;
    transform_color_to_gray(&col);
    ps->value = col;
  }
}

void _Scheme::TransformToGray(void)
{
  _UWORD i;
  _SCHEME *ps;
  
  for(i=0, ps=MemScheme.ptr; i<MemScheme.no; i++, ps++)
    transform_scheme(&ps->memsinglescheme);
}

_UDWORD _Scheme::Add(_ASCII *label, _SCHEME *pms)
{
  _UDWORD size = (MemScheme.no + 1) * sizeof(_SCHEME);
  if(Memory2((void**)&MemScheme.ptr, size) != 0)
  {
    MemScheme.ptr[MemScheme.no] = *pms;
  
    if(LseLabel.Add(label) == _TRUE)
    {
      _UDWORD retcode = MemScheme.no;
      MemScheme.no += 1;
      return retcode;
    }
  }

  return DEFSCOPE;
}

// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************

void _FontScheme::Init()
{
  _Scheme::Init();
  InternLabelDone = _FALSE;
  InternLabel.Init();
}

void _FontScheme::Free()
{
  InternLabel.Free();
  _Scheme::Free();
  Init();
}

_BOOL _FontScheme::MakeInternLabelList(void)
{
  if(InternLabelDone == _FALSE)
  {
    _FontScheme Tmp;
    Tmp.Init();
    _ASCII dpne[_MAX_PATHLENGTH];
    
    if(Project.GetDPNE(dpne, _PE_FONTSCHEME) == _TRUE)
    {
      if(Tmp.Load(dpne, _FALSE, _FALSE) == _TRUE)
      {
        InternLabel = Tmp.LseLabel;
        Tmp.LseLabel.Init();
        InternLabelDone = _TRUE;
      }
    }
    
    Tmp.Free();
  }
  
  return InternLabelDone;
}

_ASCII *_FontScheme::GetLabelByIndex(_UDWORD idx)
{
  if(MakeInternLabelList() == _TRUE)
    return InternLabel.GetText(idx);
  
  return NULL;
}

_UDWORD _FontScheme::GetIndexByLabel(_ASCII *label)
{
  if(MakeInternLabelList() == _TRUE)
  {
    _UDWORD idx = 0;
    if(InternLabel.Seek(&idx, label) == _TRUE)
      return idx;
      
    if(StrCmp(label, 1, "_@Standard", 1) == 0)
    {
      // T_STRIKEOUT ist füe die 3d-darstellung des textes
     #ifdef UC_IPC      
      return add_hardcoded_fontscheme(label, (_ASCII*)"_@Standard", MyStandardFont, T_PROP | T_STRIKEOUT);
     #else
      return add_hardcoded_fontscheme(label, (_ASCII*)"_@Standard", MyDefaultFont, T_PROP | T_STRIKEOUT);
     #endif 
    }
    if(StrCmp(label, 1, "_@Default", 1) == 0)
    {
      return add_hardcoded_fontscheme(label, (_ASCII*)"_@Default", MyDefaultFont, T_PROP);
    }
  }
  return 0;
}

_ASCII* _FontScheme::MakeNewLseLabel(_ASCII *label, _UWORD no)
{
  label[0] = '@';
  label[1] = '_';
  DToA(&label[2], no, 0x800, sizeof(_ASCII));
  
  return label;
}

_BOOL _FontScheme::Finish(_UWORD no, _UWORD entryno)
{
  _UWORD i;
  _BOOL  retcode = _FALSE;
  _ASCII label[20];

  if(no <= MemScheme.no)
  {
    retcode = _TRUE;
    for(i=0; i<MemScheme.no; i++)
    {
      if(FinishSub(&MemScheme.ptr[i], entryno) == _FALSE)
        retcode = _FALSE;
    }

    if(no < MemScheme.no)
    {
      for(i=no; i<MemScheme.no; i++)
        free_SCHEME(&MemScheme.ptr[i]);
      Memory2((void**)&MemScheme.ptr, no * sizeof(_SCHEME));
    }

    MemScheme.no = no;

    LseLabel.Free();
    for(i=0; i<MemScheme.no; i++)
    {
      LseLabel.Add(MakeNewLseLabel(label, i));
    }
  }

  return(retcode);
}

_BOOL _FontScheme::Prepaire(_UWORD no, _UWORD entryno)
{
  _BOOL   retcode = _FALSE;
  _UDWORD size;
  _UWORD  i;

  Free();

  size = no * sizeof(_SCHEME);
  if(Memory2((void**)&MemScheme.ptr, size) != 0)
  {
    retcode = _TRUE;
    for(i=0; i<no; i++)
    {
      if(PrepaireSub(&MemScheme.ptr[i], entryno) == _FALSE)
        retcode = _FALSE;
    }
    MemScheme.no = no;
  }

  return(retcode);
}

_UDWORD _FontScheme::AddNew(_ASCII *label, _UWORD fontindex)
{
  _UDWORD retcode = DEFSCOPE;
  _ASCII  tmplabel[20];
  _SCHEME scm;

  if(InternLabel.Seek(&retcode, label) == _TRUE)
    return retcode;
    
  retcode = DEFSCOPE;  
    
  init_SCHEME(&scm);
  if(PrepaireSub(&scm, Project.GetLanguageNo()) == _TRUE)
  {
    for(_UWORD i=0; i<scm.memsinglescheme.no; i++)
    {
      scm.memsinglescheme.ptr[i].op = SOP_EQUAL;
      scm.memsinglescheme.ptr[i].upto.no = 1;
      scm.memsinglescheme.ptr[i].upto.info[0].value = i;
      scm.memsinglescheme.ptr[i].upto.info[0].state = CONST_VAL;
      scm.memsinglescheme.ptr[i].value = fontindex;
    }
  
    if(MakeInternLabelList() == _TRUE)
    {
      if(InternLabel.Add(label) == _TRUE)
      {
      
        MakeNewLseLabel(tmplabel, LseLabel.GetNo());
        if(Add(tmplabel, &scm) != DEFSCOPE)
        {
          init_SCHEME(&scm);
          retcode = MemScheme.no - 1;
        }
      }
    }
  }

  free_SCHEME(&scm);
 
  return retcode;
}

_UDWORD add_hardcoded_fontscheme(_ASCII *fontschemename, _ASCII *fontname, void *fontdata, _ATTRIB attrib)
{
  _UDWORD fontindex = Font.Add(fontdata, fontname, attrib, _FALSE);
  
  if(fontindex != DEFSCOPE)
  {
    return FontScheme.AddNew(fontschemename, fontindex);
  }
  
  return DEFSCOPE;
}


// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************


void _TextScheme::Init(void)
{
  init_MEM_TXTSCHEME(&MemTxtScheme);
  LseLabel.Init();
}

void _TextScheme::Free(void)
{
  free_MEM_TXTSCHEME(&MemTxtScheme);
  LseLabel.Free();
}

_BOOL _TextScheme::Read(_File *File)
{
  _BOOL retcode = _TRUE;

  Init();

  File->Read_MEM_TXTSCHEME(&MemTxtScheme);
 #if COMPILER_VERSION >= 35
  LseLabel.Read(File, _TRUE);
 #endif

  if(File->GetError() == _TRUE)
  {
    Free();
    retcode = _FALSE;
  }

  return(retcode);
}

_BOOL _TextScheme::Load(_ASCII *dpne)
{
  _File File;
  _BOOL retcode = _FALSE;

  Free();

  File.Open(LOAD, dpne);
  if(File.ReadHeader() == _TRUE)
  {
    retcode = Read(&File);
  }
  File.Close();

  return(retcode);
}

_ANYPARA *_TextScheme::CallSub(_TXTSCHEME *ps, _VARLABEL *pl)
{
  _RESULT          result;
  _VARIABLE        variable;
  _BOOL            st;
  _UWORD           no;
  _IVALUE          value;
  _SINGLETXTSCHEME *pss;

  if(VarList.GetSystemData(&result, &variable, pl, _TRUE, _FALSE) == _TRUE)
  {
    value = result.value;
    pss   = ps->ptr;
    no    = ps->no;

    while(no)
    {
      no--;
      if(VarList.GetSystemData(&result, &variable, &pss->upto, _TRUE, _FALSE) == _TRUE)
      {
        switch(pss->op)
        {
          case SOP_SMALLEQUAL  : st = (value <= result.value)? _TRUE : _FALSE; break;
          case SOP_EQUAL       : st = (value == result.value)? _TRUE : _FALSE; break;
          case SOP_BIGGEREQUAL : st = (value >= result.value)? _TRUE : _FALSE; break;
          case SOP_BIGGER      : st = (value > result.value)? _TRUE : _FALSE; break;
          case SOP_NOTEQUAL    : st = (value != result.value)? _TRUE : _FALSE; break;
          default              : st = (value < result.value)? _TRUE : _FALSE; break;
        }

        if(st == _TRUE)
        {
          //no = 0;
          return &pss->anypara;
        }
      }

      pss += 1;
    }
  }

  return NULL;
}

_BOOL _TextScheme::Call(_MYTXTSCHEME *pd, _VARLABEL *link)
{
  _BOOL    retcode = _FALSE;
  _ANYPARA *pap;

  if(link != NULL)
  {
    //if(chk_VARLABEL(link) == _TRUE) 
    if(chk_VARLABEL_Makro(link)) // Makro, speed: nicht auf true oder false abfragen
    {
      if(pd->schemeno < MemTxtScheme.no)
      {
        pap = CallSub(&MemTxtScheme.ptr[pd->schemeno], link);
      }
      else if(pd->overload.no > 0)
      {
        pap = CallSub(&pd->overload, link);
      }

      if(pap != NULL)
      {
        retcode = _TRUE;
        copy_ANYPARA(&retcode, &pd->anypara, pap);
      }
    }
  }

  if(retcode == _FALSE)
    free_ANYPARA(&pd->anypara);

  return(retcode);
}

_ANYPARA *_TextScheme::Call(_UWORD no, _VARLABEL *link)
{
  if(link != NULL)
  {
    if(chk_VARLABEL_Makro(link) == _TRUE)
    {
      if(no < MemTxtScheme.no)
      {
        return CallSub(&MemTxtScheme.ptr[no], link);
      }
    }
  }

  return NULL;
}

_ANYPARA *_TextScheme::Call(_UWORD no, _DWORD value)
{
  _VARLABEL vl;
  init_VARLABEL(&vl);
  vl.no = 1;
  vl.info[0].state = CONST_VAL;
  vl.info[0].value = value;
  _ANYPARA *retcode = TextScheme.Call(no, &vl);
  free_VARLABEL(&vl);
  return retcode;
}

_BOOL _TextScheme::Get(_TXTSCHEME **pps, _UWORD no)
{
  if(no < MemTxtScheme.no)
  {
    *pps = &MemTxtScheme.ptr[no];
    return _TRUE;
  }

  *pps = NULL;
  return _FALSE;
}

_UWORD _TextScheme::GetNo(void)
{
  return(MemTxtScheme.no);
}

extern "C" _ANYPARA *get_textschemeentry(_UWORD no, _VARLABEL *vl)
{
  return(TextScheme.Call(no, vl));
}

extern "C" _UDWORD ImageScheme_Add(_ASCII *label, _SCHEME *pscheme)
{
  _SCHEME tmp;
  init_SCHEME(&tmp);

  if(copy_SCHEME(&tmp, pscheme) ==_TRUE)
  {
    return ImageScheme.Add(label, &tmp);
  }
  
  return DEFSCOPE;
}

