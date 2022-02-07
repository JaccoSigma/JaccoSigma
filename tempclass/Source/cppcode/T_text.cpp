#include "T_IPC.H"
#include "T_NUC.H"

/*
_BOOL _TextPuffer::Add(void *txt, _UWORD chr_size)
{
  return((add_TEXT_BUFFER(&Tbuffer, txt, chr_size) == DEFSCOPE)? _FALSE : _TRUE);
}

_BOOL _TextPuffer::Del(_UDWORD pos)
{
  _UDWORD offset, len, size;
  _BOOL   retcode = _FALSE;

  if(pos < Tbuffer.no)
  {
	  offset  = GetOffset(pos);
	  len     = StrLen(&Tbuffer.ptr[offset], sizeof(_CHAR)) + 1;
	  size    = (Tbuffer.size - offset - len) * sizeof(_CHAR);
	  MemMove(&Tbuffer.ptr[offset], &Tbuffer.ptr[offset+len], size);
	  Tbuffer.size   -= len;
	  Realloc(Tbuffer.size);
	  Tbuffer.change  = _TRUE;
	  Tbuffer.no     -= 1;
	  retcode         = _TRUE;
  }

  return(retcode);
}

_BOOL _TextPuffer::Ins(_UDWORD pos, _CHAR *txt)
{
  _BOOL   retcode = _TRUE;
  _UDWORD len, offset, size;

  if(pos >= Tbuffer.no)
  {
	  retcode = Add(txt, sizeof(_CHAR));
  }
  else
  {
	  len = StrLen(txt, sizeof(_CHAR)) + 1;
	  if(Realloc(Tbuffer.size + len) == _TRUE)
	  {
      offset = GetOffset(pos);
      size   = (Tbuffer.size - offset) * sizeof(_CHAR);
      MemMove(&Tbuffer.ptr[offset+len], &Tbuffer.ptr[offset], size);
      StrCpy(&Tbuffer.ptr[offset], sizeof(_CHAR), txt, sizeof(_CHAR));
      Tbuffer.size  += len;
      Tbuffer.change = _TRUE;
      Tbuffer.no    ++;
    }
  }

  return(retcode);
}

_BOOL _TextPuffer::Ren(_UDWORD pos, _CHAR *txt)
{
  _BOOL   retcode = _FALSE;
  _UDWORD offset, len_o, len_n, size;

  if(pos < Tbuffer.no)
  {
    offset = GetOffset(pos);
    len_o  = StrLen(&Tbuffer.ptr[offset], sizeof(_CHAR)) + 1;
    len_n  = StrLen(txt, sizeof(_CHAR)) + 1;

    if(len_o == len_n)
    {
      StrCpy(&Tbuffer.ptr[offset], sizeof(_CHAR), txt, sizeof(_CHAR));
      Tbuffer.change = _TRUE;
      retcode        = _TRUE;
    }
    else if(len_o > len_n)
    {
      StrCpy(&Tbuffer.ptr[offset], sizeof(_CHAR), txt, sizeof(_CHAR));
      size    = (Tbuffer.size - offset - len_o) * sizeof(_CHAR);
      MemMove(&Tbuffer.ptr[offset+len_n], &Tbuffer.ptr[offset+len_o], size);
      Tbuffer.size   = Tbuffer.size - len_o + len_n;
      Realloc(Tbuffer.size);
      Tbuffer.change = _TRUE;
      retcode        = _TRUE;
    }
    else
    {
      size = Tbuffer.size - len_o + len_n;
      if(Realloc(size) == _TRUE)
      {
        Tbuffer.size = size;
        offset       = GetOffset(pos);
        size         = (Tbuffer.size - offset - len_o) * sizeof(_CHAR);
        MemMove(&Tbuffer.ptr[offset+len_n], &Tbuffer.ptr[offset], size);
        StrCpy(&Tbuffer.ptr[offset], sizeof(_CHAR), txt, sizeof(_CHAR));
        Tbuffer.change = _TRUE;
        retcode        = _TRUE;
      }
    }
  }

  return(retcode);
}

_BOOL _TextPuffer::Write(_File *File)
{
  // [] .......... sizeof(_CHAR)
  // [][][][] .... no of text (erst ab compilerversion 32 4byte, vorher 2byte)
  // [][][][] .... no of characters
  // [ ........... text

  _UDWORD byte;
  _UBYTE  no;

  if(File->GetError() == _FALSE)
  {
    no = sizeof(_CHAR);
    File->Write(&no, sizeof(_UBYTE));               // save sizeof character
   #if COMPILER_VERSION < 32
    File->Write(&Tbuffer.no, sizeof(_UWORD));             // save no of text
   #else
    File->Write(&Tbuffer.no, sizeof(_UDWORD));            // save no of text
   #endif
    File->Write(&Tbuffer.size, sizeof(_UDWORD));    // save no of characters
    byte = Tbuffer.size * sizeof(_CHAR);            // calculate no of bytes
    File->Write(Tbuffer.ptr, byte);                             // save text
  }

  return((File->GetError() == _FALSE)? _TRUE:_FALSE);
}

_BOOL _TextPuffer::Realloc(_UDWORD size)
{
  size = size * sizeof(_CHAR);
  return((Memory2((void**)&Tbuffer.ptr, size)!=0)? _TRUE:_FALSE);
}

_BOOL _TextPuffer::GetChange(void)
{
  return(Tbuffer.change);
}

void _TextPuffer::SetChange(_BOOL c)
{
  Tbuffer.change = c;
}

_UDWORD _TextPuffer::GetOffset(_UDWORD pos)
{
  _UDWORD offset = 0;
  _UDWORD i;

  if(pos < Tbuffer.no)
  {
    if(Tbuffer.poff != NULL)
    {
      return(poff[pos]);
    }
    else
    {
      for(i=0; i<pos; i++)
	    {
        offset += (StrLen(&Tbuffer.ptr[offset], sizeof(_CHAR)) + 1);
      }
    }
  }

  return(offset);
}
*/

_UWORD nullstring;
void *GetNullstring(void)
{
  nullstring = 0;
  return &nullstring;
}

void _TextPuffer::Init(void)
{
  init_TEXT_BUFFER(&Tbuffer);
}

void _TextPuffer::Free(void)
{
  free_TEXT_BUFFER(&Tbuffer);
//  Init(); speed
}

_BOOL _TextPuffer::Copy(_TextPuffer *Pt)
{
  _DWORD sz, len;
  _BOOL  retcode = _FALSE;

  Free();

  if(Pt != NULL)
  {
    sz = Pt->GetSize();
    len = sz * sizeof(_CHAR);
    if(Memory2((void**)&Tbuffer.ptr, len) != 0)
    {
      Tbuffer.size    = sz;
      Tbuffer.no      = Pt->GetNo();
      retcode         = _TRUE;
      MemCpy(Tbuffer.ptr, Pt->GetPtr(), len);
    }
  }

  return(retcode);
}

_BOOL _TextPuffer::Merge(_TextPuffer *Pt)
{
  _UDWORD sz, len;
  _BOOL  retcode = _FALSE;

  if(Pt != NULL)
  {
    sz  = Pt->GetSize();
    len = (Tbuffer.size + sz) * sizeof(_CHAR);
    if(Memory2((void**)&Tbuffer.ptr, len) != 0)
    {
      len = Pt->GetSize() * sizeof(_CHAR);
      MemCpy(&Tbuffer.ptr[Tbuffer.size], Pt->GetPtr(), len);
      Tbuffer.size  += sz;
      Tbuffer.no    += Pt->GetNo();
      retcode        = _TRUE;
      Memory2((void**)&Tbuffer.poff, 0); // offsetpuffer löschen
    }
  }

  return(retcode);
}

_CHAR* _TextPuffer::GetText(_UDWORD pos)
{
  return(gettext_TEXT_BUFFER(&Tbuffer, pos));
}

_UDWORD _TextPuffer::Add(void *txt, _UWORD chr_size)
{
  return  add_TEXT_BUFFER(&Tbuffer, txt, chr_size);
}

_BOOL _TextPuffer::Jump(_File *File)
{
  _UDWORD tmp;
  _UBYTE  cs;

  if(File->GetError() == _FALSE)
  {
    File->Read(&cs, sizeof(_UBYTE));               // load size of character
   #if COMPILER_VERSION >= 32
    File->Seek(4, SEEK_CUR);
   #else
    File->Seek(2, SEEK_CUR);
   #endif
    File->Read(&tmp, sizeof(_UDWORD));              // load no of characters
    if(tmp != 0)
    {
      tmp = tmp * cs;
      File->Seek(tmp, SEEK_CUR);
    }
  }

  return((File->GetError() == _FALSE)? _TRUE:_FALSE);
}

_BOOL _TextPuffer::ReadLanguage(_File *File, _LANGUAGE lan)
{
  // [][] .... no of languages
  // [ ....... TextPuffer * no of languages

  _UWORD no, i;

  if(File->GetError() == _FALSE)
  {
    if(File->Read(&no, sizeof(_UWORD)) == _TRUE)      // load no of languages
    {
      if(lan >= no)
        lan = 0;

      for(i=0; i<no; i++)
      {
        if(i == lan)
        {
          Read(File, NULL);
        }
        else
        {
          Jump(File);
        }
      }
    }
  }

  return((File->GetError() == _FALSE)? _TRUE:_FALSE);
}

_BOOL _TextPuffer::Read(_File *File, _BOOL *unicode)
{
  Free();
  return(File->Read_TEXT_BUFFER(&Tbuffer, unicode));
}

_BOOL _TextPuffer::Load(_ASCII *dpne, _UWORD language)
{
  _File File;
  _BOOL retcode = _FALSE;

  File.Open(LOAD, dpne);
  if(File.ReadHeader() == _TRUE)
  {
    retcode = ReadLanguage(&File, language);
  }
  File.Close();

  return(retcode);
}

void _TextPuffer::StrUp(void)
{
  if(Tbuffer.ptr != NULL)
    ::StrUp(Tbuffer.ptr, sizeof(_CHAR), Tbuffer.size);
}

_UDWORD _TextPuffer::GetSize(void)
{
  return(Tbuffer.size);
}

_CHAR* _TextPuffer::GetPtr(void)
{
  return(Tbuffer.ptr);
}

_UDWORD _TextPuffer::GetNo(void)
{
  return(Tbuffer.no);
}

// ***************************************************************************
// ***************************************************************************

void _AsciiPuffer::Init(void)
{
  init_ASCII_BUFFER(&Abuffer);
  init_MEM_POINTERPARA(&MemPointerPara);
}

void _AsciiPuffer::Free(void)
{
  free_ASCII_BUFFER(&Abuffer);
  free_MEM_POINTERPARA(&MemPointerPara);
  Init();
}

_BOOL _AsciiPuffer::Read(_File *File, _BOOL make_pointerlist)
{
  free_ASCII_BUFFER(&Abuffer);
  if(File->Read_ASCII_BUFFER(&Abuffer) == _TRUE)
  {
   #ifndef UC_CASE_SENSITIVE
    StrUp();
   #endif

    if(make_pointerlist == _TRUE)
    {
      MakeFastSeek();
    }

    return _TRUE;
  }

  return _FALSE;
}

_UDWORD _AsciiPuffer::GetOffset(_UDWORD pos)
{
  _UDWORD offset = 0;
  _UDWORD i;

  if(pos < Abuffer.no)
  {
    for(i=0; i<pos; i++)
    {
      offset += (StrLen(&Abuffer.ptr[offset], sizeof(_ASCII)) + 1);
    }
  }

  return(offset);
}

_ASCII* _AsciiPuffer::GetText(_UDWORD pos)
{
  if(pos < Abuffer.no)
  {
    if(pos < MemPointerPara.no)
      return (_ASCII*)MemPointerPara.ptr[pos].ptr;

    return &Abuffer.ptr[GetOffset(pos)];
  }

  nullstring = 0;
  return((_ASCII*)&nullstring);
//  return(&nullstring);
}

void _AsciiPuffer::StrUp(void)
{
  if(Abuffer.ptr != NULL)
    ::StrUp(Abuffer.ptr, sizeof(_ASCII), Abuffer.size);
}

_BOOL _AsciiPuffer::Add(_ASCII *txt)
{
  _BOOL retcode = _FALSE;

  if(add_ASCII_BUFFER(&Abuffer, txt) == _TRUE)
  {
    free_MEM_POINTERPARA(&MemPointerPara);
    retcode = _TRUE;
  }

  return(retcode);
}

_MEM_POINTERPARA* _AsciiPuffer::GetMemPointer(void)
{
  return(&MemPointerPara);
}

_DWORD _AsciiPuffer::GetSize(void)
{
  return(Abuffer.size);
}

_ASCII* _AsciiPuffer::GetPtr(void)
{
  return(Abuffer.ptr);
}

_UDWORD _AsciiPuffer::GetNo(void)
{
  return(Abuffer.no);
}

_BOOL _AsciiPuffer::MakeFastSeek(void)
{
  if(make_MEM_POINTERPARA(&MemPointerPara, Abuffer.no) == _TRUE)
  {
	  _ASCII *ptr = Abuffer.ptr;
    _POINTERPARA *ppa = MemPointerPara.ptr;
    for(_UDWORD i=0; i<Abuffer.no; i++)
    {
      ppa->para = StrLen(ptr, sizeof(_ASCII));
      ppa->ptr = ptr;
      
      ptr += (ppa->para + 1);
      ppa += 1;
    }
    MemPointerPara.no = Abuffer.no;
    
    return _TRUE;
  }
   
  return _FALSE;
}

_BOOL _AsciiPuffer::Seek(_UDWORD *no, _ASCII *label)
{
	_ASCII  *ptr;
	_UDWORD i, labellen;

 #ifndef UC_CASE_SENSITIVE
	_ASCII tmplabel[100];
	StrCpy(tmplabel, sizeof(_ASCII), label, sizeof(_ASCII));
	::StrUp(tmplabel, sizeof(_ASCII), StrLen(label, sizeof(_ASCII)));
	label = tmplabel;
 #endif
  
  if(Abuffer.no == MemPointerPara.no)
  {
    labellen = StrLen(label, sizeof(_ASCII));
    _POINTERPARA *ppa = MemPointerPara.ptr;
    for(i=0; i<MemPointerPara.no; i++)
    {
      if(ppa->para == labellen)
      {
        ptr = (_ASCII*)ppa->ptr;
        if(ptr[0] == label[0])
        {
          if(MemCmp(ptr, label, labellen) == 0)          
          {
            *no = i;
            return(_TRUE);
          }
        }
      }
      ppa += 1;
    }
    return _FALSE;
  }
  
  ptr = Abuffer.ptr;
  for(i=0; i<Abuffer.no; i++)
  {
    if(ptr[0] == label[0])
    {
      if(StrXmp(ptr, label, sizeof(_ASCII)) == 0)
      {
        *no = i;
        return(_TRUE);
      }
    }

    ptr += (StrLen(ptr, sizeof(_ASCII)) + 1);
  }

  return(_FALSE);
}

_ASCII_BUFFER* _AsciiPuffer::GetAsciiBuffer()
{
  return &Abuffer;
}


_ASCII *get_text(_FAST_ASCII_BUFFER *p, _UDWORD no)
{

  if(no < p->ascii_buffer.no)
  {
    if(p->poffset != NULL)
    {
      return &p->ascii_buffer.ptr[p->poffset[no]];
    }
    else
    {
      _ASCII *retcode = p->ascii_buffer.ptr;
      while(no)
      {
        retcode += (StrLen(retcode, sizeof(_ASCII)) + 1);
        no += 1;
      }
      
      return retcode;
    }
  }

  nullstring = 0;
  return((_ASCII*)&nullstring);
}

_UDWORD make_crc_ASCII_BUFFER(_ASCII_BUFFER *p)
{
  _UDWORD retcode = 0;

  if(p != NULL)
  {
    if((p->ptr != NULL)&&(p->no != 0)&&(p->size != 0))
    {
      retcode = MakeCrc32(retcode, p->ptr, p->size);
    }
  }

  return(retcode);
}

_BOOL add_ASCII_BUFFER(_ASCII_BUFFER *p, _ASCII *txt)
{
  _UDWORD size, len;
  _BOOL   retcode = _FALSE;

  len  = StrLen(txt, sizeof(_ASCII)) + 1;
  size = p->size + len;

  if(size <= p->space)
  {
	  MemCpy(&p->ptr[p->size], txt, len);
	  p->no   += 1;
	  p->size  = size;
    retcode  = _TRUE;
  }
  else if(Memory2((void**)&p->ptr, size) != 0)
  {
	  MemCpy(&p->ptr[p->size], txt, len);
	  p->no   += 1;
	  p->size  = size;
    p->space = size;
	  retcode  = _TRUE;
  }

  return(retcode);
}

// lasal interface ******************************************************

extern "C" _ASCII *gettext_ASCII_BUFFER(_ASCII_BUFFER *p, _UDWORD no)
{
  _ASCII *retcode = (_ASCII*)&nullstring;

  if(p != NULL)
  {
    if(no < p->no)
    {
      retcode = p->ptr;
      while(no)
      {
        no      -= 1;
        retcode += (StrLen(retcode, sizeof(_ASCII)) + 1);
      }
    }
  }
  return(retcode);
}

extern "C" _CHAR *gettext_TEXT_BUFFER(_TEXT_BUFFER *p, _UDWORD no)
{
  _CHAR *retcode = (_CHAR*)&nullstring;

  if(p != NULL)
  {
    if(no < p->no)
    {
      if(p->poff != NULL)
      {
        return(&p->ptr[p->poff[no]]);
      }
      else
      {
        retcode = p->ptr;
        while(no)
        {
          no      -= 1;
          retcode += (StrLen(retcode, sizeof(_CHAR)) + 1);
        }
      }
    }
  }
  return(retcode);
}

_UDWORD add_TEXT_BUFFER(_TEXT_BUFFER *p, void *txt, _UWORD chr_size)
{
  _UDWORD len;
  _UDWORD retcode = DEFSCOPE;

  len = StrLen(txt, chr_size) + 1;
  if(Memory2((void**)&p->ptr, (p->size + len) * sizeof(_CHAR)) != 0)
  {
	  StrCpy(&p->ptr[p->size], sizeof(_CHAR), txt, chr_size);
    retcode = p->no;
	  p->no   ++;
	  p->size += len;
    Memory2((void**)&p->poff, 0); // offsetpuffer löschen
  }

  return(retcode);
}

_BOOL calculate_offset_TEXT_BUFFER(_TEXT_BUFFER *p)
{
  _BOOL   retcode = _FALSE;
  _UDWORD no, offset;
  _UDWORD *ph;

  no = p->no;
  if(Memory2((void**)&p->poff, no * sizeof(_UDWORD)) != 0)
  {
    retcode = _TRUE;
    ph      = p->poff;
    offset  = 0;
    while(no)
    {
      *ph     = offset;
      offset += (StrLen(&p->ptr[offset], sizeof(_CHAR)) + 1);
      ph     += 1;
      no     -= 1;
    }
  }

  return(retcode);
}


_UDWORD findtext_ASCII_BUFFER(_ASCII_BUFFER *p, _ASCII *l)
{
  _ASCII  *ptr;
  _UDWORD len, i;
  _UDWORD lcmp = StrLen(l, sizeof(_ASCII)) + 1;

  if((p != NULL) && (l != NULL))
  {
    ptr = p->ptr;
    for(i=0; i<p->no; i++)
    {
      len = StrLen(ptr, sizeof(_ASCII)) + 1;
      if((len == lcmp) && (ptr[0] == l[0]))
      {
        if(MemCmp(ptr, l, len) == 0)
          return(i);
      }

      ptr += len;
    }
  }

  return(0xFFFFFFFF);
}


extern "C" _CHAR* textpuffer_getPtr(_ANYLST list)
{
  return(TextList.Get(list, 0));
}

extern "C" _DWORD textpuffer_getSize(_ANYLST list)
{
	return(TextList.GetSize(list));
}

extern "C" _UDWORD textpuffer_getNo(_ANYLST list)
{
	return(TextList.GetNo(list));
}

extern "C" _CHAR* textpuffer_getText(_ANYLST list, _UDWORD txt)
{
  return(TextList.Get(list, txt));
}


#ifdef UC_IPC
_UDWORD memsize_ASCII_BUFFER(_ASCII_BUFFER *p)
{
  return(p->size);
}

_UDWORD memsize_MEM_POINTERPARA(_MEM_POINTERPARA *p)
{
  return(p->doneno * sizeof(_POINTERPARA));
}

_UDWORD _AsciiPuffer::MemSize(void)
{
  return(memsize_ASCII_BUFFER(&Abuffer) + memsize_MEM_POINTERPARA(&MemPointerPara));
}
#endif

_BOOL _AsciiPuffer::MakeMemory(_UDWORD size)
{
  _BOOL retcode = _TRUE;

  if(Abuffer.size < size)
  {
    if(Memory((void**)&Abuffer.ptr, size) != 0)
    {
      Abuffer.space = size;
    }
    else
    {
      retcode = _FALSE;
    }
  }

  return(retcode);
}

void _AsciiPuffer::FinishMemory(void)
{
  if(Abuffer.size < Abuffer.space)
  {
    Memory((void**)&Abuffer.ptr, Abuffer.size);
    Abuffer.space = Abuffer.size;
  }
}

_BOOL _AsciiPuffer::DeepCopy(_AsciiPuffer *dst)
{
  _BOOL retcode = _TRUE;
  dst->Free();
  
  merge_ASCII_BUFFER(&retcode, &dst->Abuffer, &Abuffer);
  return retcode;
}


