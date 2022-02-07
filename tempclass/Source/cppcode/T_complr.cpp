#include "T_IPC.H"
#include "T_NUC.H"


// als operanden sind variablen (var.soll), konstanten (127, 16#0A)
// und lokale kennbits (#4) möglich

// syntax :
// 3 * (2+1);
// var.ist = (var.soll + 128) / 7;
// !(#4 OR #5)


#define ERROR_NONE      0x0000
#define ERROR_SYNTAX    0x0001

class _Compiler
{
  private :
    _UBYTE  *Start; 
    _UBYTE  *Source;
    _UBYTE  Error;
    _BOOL   Wait4Plc;
    _BOOL   Cheat;

    _IVALUE GetVarlabel(void); 
    _UWORD  GetOperator(void);
    _IVALUE GetBit(void);
    _IVALUE GetChkBit(void);
    _UWORD  GetId(void);
    void    GetLexem(_LEXEM*);
    _IVALUE DoPart(void);
    _IVALUE Save(void);

  public :
    _IVALUE Run(_COMPCODE*, _BOOL*, _BOOL wait4plc, _BOOL cheat);
};

_IVALUE _Compiler::GetVarlabel(void)
{
  _IVALUE   retcode = 0;
  _VARLABEL vl;
  _UWORD    i;

  if(Error == ERROR_NONE)
  {
    init_VARLABEL_Makro(&vl);

    vl.no = *(_UBYTE*)Source; Source += 1;
    if(vl.no == 1)
    {
      vl.info[0].state = (_DEFVAL)*(_UWORD*)Source; Source += 2;
      vl.info[0].value = *(_DWORD*)Source; Source += 4;
    }
    else
    {
      for(i=0; i<vl.no; i++)
      {
        vl.info[i].state = (_DEFVAL)*(_UWORD*)Source; Source += 2;
        vl.info[i].value = *(_DWORD*)Source; Source += 4;
      }
    }

    if(VarList.GetValue(&vl, &retcode, Wait4Plc, Cheat) == _FALSE)
      retcode = 0;
  }

  return(retcode);
}

_IVALUE _Compiler::GetBit(void)
{
  _IVALUE retcode = 0;
  _LEXEM lexem;


  // it follows a _VALABEL and also the id for a _VARLABEL

  GetLexem(&lexem);
  if(Error == ERROR_NONE)
  {
    retcode = (lexem.value != 0) ? 1 : 0;
  }

  return(retcode);
}

_UWORD _Compiler::GetOperator(void)
{
  _UWORD retcode = IOP_NOP;

  if(Error == ERROR_NONE)
  {
    retcode = Source[0];
    if(retcode > IOP_END) 
      Error |= ERROR_SYNTAX; 
    Source += 1; 
  }

  return(retcode); 
}

_IVALUE _Compiler::GetChkBit(void)
{
  _IVALUE retcode = 0;
  _UWORD  sno;

  if(Error == ERROR_NONE)
  {
    sno     = *((_UWORD*)Source);
    retcode = ((Sequencer.Call(sno, _FALSE, _TRUE, _TRUE) == _TRUE)? 1 : 0);
    Source += 2;
  }

  return(retcode);
}

_UWORD _Compiler::GetId(void)
{
  if(Error == ERROR_NONE)
  {
    _UWORD retcode = Source[0];
    if(retcode >= ID_CONSTANT)
      Error |= ERROR_SYNTAX;
    Source += 1;
    return(retcode);
  }

  return(ID_NONE);
}

void _Compiler::GetLexem(_LEXEM *pl)
{
  pl->id = GetId();

  switch(pl->id)
  {
    case ID_OPERATOR : pl->op = GetOperator();    break;
    case ID_VARLABEL : pl->id = ID_CONSTANT;
                       pl->value = GetVarlabel(); break;
    case ID_BITNO    : pl->id = ID_CONSTANT;
                       pl->value = GetBit();      break;
    case ID_CHKBIT   : pl->id = ID_CONSTANT;
                       pl->value = GetChkBit();   break;
    default          : Error |= ERROR_SYNTAX;     break;
  }
  // nur ID_CONSTANT ID_OPERATOR und ID_NONE möglich
}

_IVALUE _Compiler::Save(void)
{
  _IVALUE   retcode = 0;
  _VARLABEL *pvl, vl;
  _UBYTE    *memo;
  _RESULT   res;
  _UWORD    id;
  _VARIABLE variable;

  init_VARLABEL_Makro(&vl);
  init_RESULT_Makro(&res);

  res.value = DoPart();

  memo   = Source;
  Source = Start;
  id     = GetId();

  if(id == ID_VARLABEL)
  {
    pvl       = (_VARLABEL*)Source;
    vl.no     = pvl->no;
    MemCpy(vl.info, pvl->info, vl.no * sizeof(_VARSINGLE));
    res.rmin  = res.value;
    res.rmax  = res.value;
    res.ftype = RES_CONSTANT;

    if(VarList.SetSystemData(&res, &variable, &vl, _TRUE) == _TRUE)
      retcode = 1;
  }

  Source = memo;

  return(retcode); 
}

_IVALUE _Compiler::DoPart(void)
{
  _LEXEM  lexem;
  _UWORD  log_invert = 0;
  _UWORD  ari_invert = 0;
  _IVALUE value = 1; 
  _BOOL   run;
  _UWORD  op  = IOP_NOP; 
  _UWORD  mux = 0; 

  do
  {
    run = _TRUE; 
    GetLexem(&lexem);

    COMP_START:

    if(Error == ERROR_NONE)
    {
      switch(lexem.id)
      {
        case ID_OPERATOR :
          switch(lexem.op)
          {
            case IOP_END           : run = _FALSE;                     break;
            case IOP_BRACKET_CLOSE : run = _FALSE;                     break;
            case IOP_BRACKET_OPEN  : lexem.id    = ID_CONSTANT;
                                     lexem.value = DoPart(); goto COMP_START;
            case IOP_NOT           : log_invert ^= 1;                  break;
            case IOP_A_NOT         : ari_invert ^= 1;                  break;
            case IOP_NOP           :                                   break;
            case IOP_A_EQUAL       : value = Save(); run = _FALSE;     break;
            default                : op = lexem.op;                    break;
          }
          break;

        case ID_CONSTANT :
          if(log_invert)
          {
            lexem.value = (lexem.value > 0)? 0 : 1;
            log_invert  = 0;
          }
          if(ari_invert)
          {
            lexem.value = ~lexem.value; 
            ari_invert  = 0; 
          }

          if(mux == 0) 
          {
            value = lexem.value; 
            mux    = 1; 
          }
          else
          {
            switch(op)
            {
              case IOP_A_PLUS      : value += lexem.value;                  break;
              case IOP_A_MINUS     : value -= lexem.value;                  break;
              case IOP_A_MULTI     : value *= lexem.value;                  break;
              case IOP_A_DIV       : value /= lexem.value;                  break;
              case IOP_A_MOD       : value %= lexem.value;                  break;
              case IOP_A_OR        : value |= lexem.value;                  break;
              case IOP_A_AND       : value &= lexem.value;                  break;
              case IOP_A_XOR       : value ^= lexem.value;                  break;
              case IOP_OR          : value = (value |  lexem.value)? 1 : 0; break;
              case IOP_AND         : value = (value &  lexem.value)? 1 : 0; break;
              case IOP_SMALLER     : value = (value <  lexem.value)? 1 : 0; break;
              case IOP_SMALLEREQUAL: value = (value <= lexem.value)? 1 : 0; break;
              case IOP_EQUAL       : value = (value == lexem.value)? 1 : 0; break;
              case IOP_BIGGER      : value = (value >  lexem.value)? 1 : 0; break;
              case IOP_BIGGEREQUAL : value = (value >= lexem.value)? 1 : 0; break;
              case IOP_NOTEQUAL    : value = (value != lexem.value)? 1 : 0; break;
              default              : Error |= ERROR_SYNTAX; run = _FALSE;   break;
            }
          }
          break;

        default          :
          Error |= ERROR_SYNTAX;
          break;
      }
    }
    else
      run = _FALSE;

  }
  while(run == _TRUE);

  return(value);
}

_IVALUE _Compiler::Run(_COMPCODE *p, _BOOL *err, _BOOL wait4plc, _BOOL cheat)
{
  _IVALUE retcode = 1; 

  if(p->size > 0) 
  {
    Wait4Plc = wait4plc;
    Cheat    = cheat;
    Start    = p->ptr; 
    Source   = p->ptr; 
    Error    = 0; 
    retcode  = DoPart(); 
    *err     = (Error == ERROR_NONE)? _FALSE : _TRUE; 
  }

  return(retcode); 
}

void _Sequencer::Init(void)
{
  LseLabel.Init();
  No  = 0;
  Ptr = NULL;
 #ifdef _STATICCHKBIT
  Static = NULL;
 #endif
}

void _Sequencer::Free(void)
{
  _UWORD i;

  LseLabel.Free();
  for(i=0; i<No; i++)
    free_COMPCODE(&Ptr[i]);

  Memory2((void**)&Ptr, 0);
 #ifdef _STATICCHKBIT
  Memory2((void**)&Static, 0);
 #endif

  Init();
}

_BOOL _Sequencer::Read(_File *File)
{
  // [][]....... no of sequences
  // [ ......... sequenzes

  _UWORD i, tmp;
  _BOOL  retcode = _TRUE;
  _DWORD byte;

  Init();

  if(File->GetError() == _FALSE)
  {
    File->Read(&tmp, (sizeof(_UWORD)));

    // infotabel
    byte = tmp * sizeof(_COMPCODE);
    if(File->FileMemory((void**)&Ptr, byte) == _TRUE)
    {
      No = tmp;
      for(i=0; i<tmp; i++)
        File->Read_COMPCODE(&Ptr[i]);
    }

   #if COMPILER_VERSION >= 33
    LseLabel.Read(File, _FALSE);
   #endif

   #ifdef _STATICCHKBIT
    File->FileMemory((void**)&Static, No * sizeof(*Static));
    InitStatic();
   #endif
  }

  if(File->GetError() == _TRUE)
  {
    Free();
    retcode = _FALSE;
  }


//  else  // slurry
//  {
//    for(i=0; i<No; i++)
//      Call(i, _FALSE);
//  }

  return(retcode);
}

_BOOL _Sequencer::Load(_ASCII *dpne)
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

_BOOL _Sequencer::Call(_UWORD sno, _BOOL invert, _BOOL wait4plc, _BOOL cheat)
{
  if(sno < No)
  {
   #ifdef _STATICCHKBIT
    _BOOL retcode = _TRUE;
    _UWORD tmp = Static[sno];
    if(tmp != 0xFFFF)
    {
      if(tmp == 0)
      {
        retcode = _FALSE;
      }
      if(invert == _TRUE)
      {
        if(retcode == _TRUE)
          retcode = _FALSE;
        else
          retcode = _TRUE;
      }
    }
    else
    {
      retcode = CallCompiler(&Ptr[sno], invert, wait4plc, cheat);
      tmp = (retcode == _TRUE)? 1 : 0;
      if(invert == _TRUE)
        tmp = tmp ^ 1;
      Static[sno] = tmp;
    }
    return retcode;
   #else
    return CallCompiler(&Ptr[sno], invert, wait4plc, cheat);
   #endif
  }

  return _TRUE;
}

_BOOL _Sequencer::CallCompiler(_COMPCODE *p, _BOOL invert, _BOOL wait4plc, _BOOL cheat)
{
  _BOOL     error, retcode = _FALSE;
  _Compiler Compiler;

  if(p->ptr != NULL)
  {
    retcode = (Compiler.Run(p, &error, wait4plc, cheat) != 0)? _TRUE : _FALSE;
    if(error == _TRUE)
      retcode = _TRUE;
    else if(invert == _TRUE)
      retcode = (retcode == _TRUE)? _FALSE : _TRUE;
  }

  return(retcode);
}

_BOOL _Sequencer::Get(_COMPCODE **pp, _UWORD no)
{
  if(no < No)
  {
    *pp = &Ptr[no];
    return _TRUE;
  }

  return _FALSE;
}

_UWORD _Sequencer::GetNo(void)
{
  return(No);
}

#ifdef _STATICCHKBIT
void _Sequencer::InitStatic(void)
{
  if(Static != NULL)
    MemSet(Static, 0xFF, No * sizeof(_UWORD));
}
#endif

#ifdef UC_IPC
_UDWORD _Sequencer::MemSize(_MEMSIZE *p)
{
  _COMPCODE *pc;
  _UDWORD   i;

  p->data = 0;

  for(i=0,pc=Ptr; i<No; i++,pc++)
    p->data += pc->size;

  p->label = LseLabel.MemSize();

  return(p->data+p->label);
}
#endif
_BOOL overload_COMPCODE(_COMPCODE *p, _UDWORD loader)
{
  _DEFVAL defval;
  _UWORD  i, no;
  _UDWORD varno;
  _UBYTE  id;
  _BOOL   retcode = _FALSE;
  _UBYTE  *ptr  = (_UBYTE*)p->ptr;
  _UWORD  size  = 0;
  _UWORD  osize = p->size;

  while(size < osize)
  {
    id = *ptr; ptr += 1; size += 1;

    switch(id)
    {
      case ID_OPERATOR : ptr += 1; size += 1;
                         break;
      case ID_BITNO    : //ptr += 1; size += 1;  // it follows a _VARLABLE
                         break;                // also the id for a _VARLABEL
      case ID_CHKBIT   : ptr += 2; size += 2;
                         break;
      case ID_VARLABEL : no = *(_UBYTE*)ptr; ptr += 1; size += 1;
                         for(i=0; i<no; i++)
                         {
                           defval = (_DEFVAL)*(_UWORD*)ptr; ptr += 2; size += 2;
                           if(defval == VAR_VAL)
                           {
                             varno = *(_DWORD*)ptr;
                             if(overload(&varno, loader) == _TRUE)
                             {
                               retcode = _TRUE;
                               *(_DWORD*)ptr = varno;
                             }
                           }
                           ptr += 4; size += 4;
                         }
                         break;
    }
  }

  return(retcode);
}

#ifdef OK_DIAGNOSTIC

_ASCII *add_cpart(_ASCII *dst, const _ASCII *src, _UDWORD len, _UDWORD *pmaxlen)
{
  if(*pmaxlen > len)
  {
    MemCpy(dst, src, len);
    dst      += len;
    *dst      = 0;
    *pmaxlen -= len;
  }
  else if(*pmaxlen > 0)
  {
    if(*pmaxlen > 3)
      StrCpy(dst, sizeof(_ASCII), "...", sizeof(_ASCII)); 
    *pmaxlen = 0;
  }
  
  return dst;
}

void COMPCODE_to_text(_ASCII *txt, _COMPCODE *p, _UDWORD maxlen)
{
  _VARLABEL vl;
  _UWORD    i;
  _UBYTE    id;
  _UWORD    size = 0;
  _UBYTE    *ptr  = (_UBYTE*)p->ptr;
  _ASCII    stmp[255];

  while(size < p->size)
  {
    id = *ptr;
    ptr += 1;
    size += 1;

    switch(id)
    {
      case ID_OPERATOR : //*(_UBYTE*)ptr // operator eintragen
        switch(*(_UBYTE*)ptr)
        {
          case IOP_NOP:
            txt = add_cpart(txt, "NOP", 3, &maxlen);
            break;
          case IOP_BRACKET_OPEN:
            txt = add_cpart(txt, "(", 1, &maxlen);
            break;
          case IOP_BRACKET_CLOSE:
            txt = add_cpart(txt, ")", 1, &maxlen);
            break;
          case IOP_A_PLUS:
            txt = add_cpart(txt, "+", 1, &maxlen);
            break;
          case IOP_A_MINUS:
            txt = add_cpart(txt, "-", 1, &maxlen);
            break;
          case IOP_A_MULTI:
            txt = add_cpart(txt, "*", 1, &maxlen);
            break;
          case IOP_A_DIV:
            txt = add_cpart(txt, "/", 1, &maxlen);
            break;
          case IOP_A_MOD:
            txt = add_cpart(txt, " MOD ", 5, &maxlen);
            break;
          case IOP_A_OR:
            txt = add_cpart(txt, " OR ", 4, &maxlen);
            break;
          case IOP_A_AND:
            txt = add_cpart(txt, " AND ", 5, &maxlen);
            break;
          case IOP_A_XOR:
            txt = add_cpart(txt, " XOR ", 5, &maxlen);
            break;
          case IOP_A_NOT:
            txt = add_cpart(txt, " ~", 2, &maxlen);
            break;
          case IOP_A_EQUAL:
            txt = add_cpart(txt, " := ", 4, &maxlen);
            break;
          case IOP_OR:
            txt = add_cpart(txt, " | ", 3, &maxlen);
            break;
          case IOP_AND:
            txt = add_cpart(txt, " & ", 3, &maxlen);
            break;
          case IOP_SMALLER:
            txt = add_cpart(txt, " < ", 3, &maxlen);
            break;
          case IOP_SMALLEREQUAL:
            txt = add_cpart(txt, " <= ", 4, &maxlen);
            break;
          case IOP_EQUAL:
            txt = add_cpart(txt, " = ", 3, &maxlen);
            break;
          case IOP_BIGGER:
            txt = add_cpart(txt, " > ", 3, &maxlen);
            break;
          case IOP_BIGGEREQUAL:
            txt = add_cpart(txt, " >= ", 4, &maxlen);
            break;
          case IOP_NOTEQUAL:
            txt = add_cpart(txt, " <> ", 4, &maxlen);
            break;
          case IOP_NOT:
            txt = add_cpart(txt, " NOT ", 5, &maxlen);
            break;
          case IOP_END:
            txt = add_cpart(txt, ";", 1, &maxlen);
            break;
        }
        ptr += 1;
        size += 1;
        break;
      case ID_BITNO:
        stmp[0] = '#';
        DToA(&stmp[1], *(_UWORD*)ptr, 0x0800, sizeof(_ASCII));
        txt = add_cpart(txt, stmp, StrLen(stmp, sizeof(_ASCII)), &maxlen);
        ptr += 2; 
        size += 2; 
        break;
      case ID_CHKBIT: // *(_UWORD*)ptr // chkbit
        txt = add_cpart(txt, "ChkBit", 6, &maxlen);
        DToA(stmp, *(_UWORD*)ptr, 0x0800, sizeof(_ASCII));
        txt = add_cpart(txt, stmp, StrLen(stmp, sizeof(_ASCII)), &maxlen);
        ptr += 2; 
        size += 2;
        break;
      case ID_VARLABEL:
        init_VARLABEL_Makro(&vl);
        vl.no = *(_UBYTE*)ptr;
        ptr  += 1;
        size += 1;
        for(i=0; i<vl.no; i++)
        {
          vl.info[i].state = (_DEFVAL)*(_UWORD*)ptr; ptr += 2;
          vl.info[i].value = *(_IVALUE*)ptr; ptr += 4;
          size += 6;
        }
        VARLABEL_to_text(stmp, &vl);
        txt = add_cpart(txt, stmp, StrLen(stmp, sizeof(_ASCII)), &maxlen);
        break;
    }
  }
}
#endif

// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************

/*
_BOOL _File::Read_COMPCODE(_COMPCODE *p)
{
  _BOOL retcode = _FALSE;

  init_COMPCODE(p);

  if(GetError() == _FALSE)
  {
    if(Read(&size, sizeof(_UWORD)) == TRUE)
    {
      if(FileMemory((void**)&p->ptr, size) == _TRUE)
      {
        retcode = Read(p->ptr, size);
      }
    }
  }

  if(retcode != _TRUE)
    free_COMPCODE(p);

  return(retcode);
}


SEQUENCE
[2] ............. no of bytes (codelength)
[ ............... code

[ HEADER~ .......
[2] ............. no of compilersequences
[ SEQUENCES~ ....
*/


/*
_BOOL is_buchstabe(_ASCII a)
{
  if((a >= 97) && (a <= 122)) // von a bis z
    return(_TRUE);
  if((a >= 64) && (a <= 90)) // von @ bis Z
    return(_TRUE);
  if(a == 95) // [underline]
    return(_TRUE);

  return(_FALSE);
}

_BOOL is_zahl(_ASCII a)
{
  if((a>=48) && (a <= 57)) // von 0 bis 9
    return(_TRUE);
  return(_FALSE);
}

_BOOL is_sonderzeichen(_ASCII a)
{
  if((a == 46)||(a == 92)) // [punkt], [backslash]
    return(_TRUE);
  return(_FALSE);
}

_BOOL do_brackets(_ASCII **pp)
{
  _UWORD len;
  _ASCII a;
  _ASCII *p = *pp;
  _BOOL  komma = _FALSE;

  len = StrLen(p, sizeof(_ASCII));

  if(len > 0)
  {
    a = *p;
    while(a)
    {
      p += 1;
      if(is_zahl(a) == _FALSE)
      {
        if(komma == _FALSE)
          return(_FALSE);

        switch(a)
        {
           case 44: // [komma]
                    komma = _FALSE;
                    break;
           case 93: // [eckige klammer zu]
                    *pp = p;
                    return(_TRUE);
           case 91: // [eckige klammer auf]
                    if(do_brackets(&p) == _FALSE)
                      return(_FALSE);
                    komma = _FALSE;
                    break;
           default: return(_FALSE);
        }
      }
      else
      {
        komma = _TRUE;
      }

	  a = *p;
    }
    return(_TRUE);
  }

  return(_FALSE);
}

_BOOL check_hostname(_ASCII *p)
{
  _UWORD len;
  _ASCII a;

  // syntax : "@sigmatek[123,456]_seppi"
  // erstes zeichen:  buchstabe oder [underline] oder [at]
  // nachfolgend:     buchstabe oder zahl oder [underline] oder [at]
  // in eckiger klammer folgender syntax
  // nur zahlen welche durch [komma] getrennt sind

  DelSpace(p, sizeof(_ASCII));        // delete all space in line
  len = StrLen(p, sizeof(_ASCII));    // get strlen

  if(len > 0)
  {
    if(is_buchstabe(*p) == _TRUE)
    {
      p += 1;
      a  = *p;
      while(a)
      {
        p += 1;
        if((is_buchstabe(a) == _FALSE)&&(is_zahl(a) == _FALSE) && (is_sonderzeichen(a) == _FALSE))
        {
          if(a == 91) // [eckige klammer auf]
          {
            if(do_brackets(&p) == _FALSE)
              return(_FALSE);
          }
          else
          {
            return(_FALSE);
          }
        }
        a = *p;
      }
      return(_TRUE);
    }
  }

  return(_FALSE);
}
*/

