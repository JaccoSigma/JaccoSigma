#include "T_IPC.H"
#include "T_NUC.H"


#define IsNoError_Makro() (Error == H_NOERROR)
#define IsRamFile_Makro() ((RamData != NULL) && (RamSize > 0))


//_BOOL _File::GetError(void)
//{
//  return((Error == H_NOERROR)? _FALSE : _TRUE);
//}
//
//_BOOL _File::IsRamFile(void)
//{
//  if((RamData != NULL) && (RamSize > 0))
//    return _TRUE;
//
//  return _FALSE;
//}






_BOOL Lse_File_Copy(_ASCII *dpne_dst, _ASCII *dpne_src)
{
  _BOOL   retcode = _FALSE;
  _Handle Src, Dst;
  _DWORD  size, part;
  void    *ptr;
  
  if((dpne_src != NULL) && (dpne_dst != NULL))
  {
    if(Src.Open(LOAD, dpne_src) == _TRUE)
    {
      size = Src.Length();
      if(size > 0)
      {
        if(Dst.Open(SAVE, dpne_dst) == _TRUE)
        {
          part = size;
          ptr  = NULL;
          if(Memory2((void**)&ptr, size) == 0) // grosser block
          {
            Memory2((void**)&ptr, 1024); // kleiner block
            part = 1024;
          }
        
          if(ptr != NULL)
          {
            retcode = _TRUE;
            
            while(size > 0)
            {
              if(part > size)
                part = size;
        
              Src.Read(ptr, part);
              Dst.Write(ptr, part);
          
              size -= part;
            }
            
            Memory2((void**)&ptr, 0);
          }
          
          Dst.Close();
        }
      }
      Src.Close();
    }
  }
  
  return retcode;
}

_BOOL Lse_File_Rename(_ASCII *oldname, _ASCII *newname)
{
  if(lsl_pOS->piFILE->pSFR_Rename(oldname, newname) >= 0)
    return _TRUE;
    
  return _FALSE;
}


void _Handle::Init(void)
{
  Handle = -1;
// #ifdef LASAL
//  CHandle = NULL;
//  CIfOk = _FALSE;
// #endif
}

#ifdef LASAL
_BOOL _Handle::GetCIf(void)
{
  _BOOL retcode = _FALSE;

  if(OS_CILGet((char*)"SYSFILE", (void**)&File) == 0)
    retcode = _TRUE;

  return(retcode);
}
#endif

void _Handle::Free(void)
{
  Close();
  Init();
}

_BOOL _Handle::ChkHandle(void)
{
  _BOOL retcode = _FALSE;

  if(Handle != -1)
    retcode = _TRUE;

  return(retcode);
}

_BOOL _Handle::Read(void *dest, _UDWORD size)
{
  if(Handle >= 0)
  {
    if(Fread(Handle, dest, size) == size)
      return(_TRUE);
  }
  return(_FALSE);



//  _BOOL  retcode = _FALSE;
//  _UBYTE *tmp = (_UBYTE*)dest;
//  _UWORD part;
//
//  if(Handle >= 0)
//  {
//    retcode = _TRUE;
//
//    while(size > 0)
//    {
//      part = (size > 65000)? 65000 : size;
//      size -= part;
//      if(Fread(Handle, tmp, part) != part)
//      {
//        retcode = _FALSE;
//        size    = 0;
//      }
//      tmp += part;
//    }
//  }
//  return(retcode);
}

_BOOL _Handle::Write(const void *src, _UDWORD size)
{
  if(Handle >= 0)
  {
    if(Fwrite(Handle, (void*)src, size) == size)
      return(_TRUE);
  }
  return(_FALSE);

//  _BOOL  retcode = _FALSE;
//  _UBYTE *tmp = (_UBYTE*)src;
//  _UWORD part;
//
//  if(Handle >= 0)
//  {
//    retcode = _TRUE;
//    while(size > 0)
//    {
//      part = (size > 65000)? 65000 : size;
//      size -= part;
//      if(Fwrite(Handle, tmp, part) != part)
//      {
//        retcode = _FALSE;
//        size    = 0;
//      }
//      tmp += part;
//    }
//  }
//
//  return(retcode);
}

_BOOL _Handle::Open(_FILE_STATE command, const _ASCII *dpne)
{
  _BOOL retcode = _FALSE;

  if(dpne == NULL)
    return(_FALSE);
  if(dpne[0] == 0)
    return(_FALSE);

  Init();

//#define ATT_READ_WRITE          0x00000000
//#define ATT_READ_ONLY           0x00000100
//#define ATT_OPEN_SHARED         0x00000200
//#define ATT_OPEN_NO_DIR         0x00000400
//#define ATT_OPEN_DIR            0x00000800
//#define ATT_CREATE              0x00010000 // create if it does not exist
//#define ATT_CREATE_ALWAYS       0x00020000 // always create

  if(Handle == -1)
  {
    if((command == LOAD) || (command == SAVE))
    {
      Handle = Fopen((char*)dpne, command, 0);
    }
    else
    {
      Handle = Fopen((char*)dpne, command, 0x00010000);
    }

    retcode = (Handle >= 0)? _TRUE : _FALSE;
  }

  return(retcode);
}

void _Handle::Close(void)
{

  if(Handle >= 0)
  {
    Fclose(Handle);
    Handle = -1;
    Free();
  }
}

_BOOL _Handle::Remove(_ASCII *dpne)
{
  _BOOL retcode = _FALSE;

 #ifdef LASAL
  if(GetCIf() == _TRUE)  // file must be closed
  {
    if(File->remove(dpne) == 0)
      retcode = _TRUE;
  }
  else
 #endif
  {
    if(dpne == NULL)
      retcode = _TRUE;
  }

  return(retcode);
}

_DWORD _Handle::Tell(void)
{
  _DWORD pos = -1;

  if(Handle >= 0)
  {
    pos = Ftell(Handle);
  }

  return(pos);
}

_BOOL _Handle::Seek(_DWORD offset, _UWORD fromwhere)
{
  _BOOL   retcode = _FALSE;
  _DWORD  len;

  if(Handle >= 0)
  {
    switch(fromwhere)
    {
      case SEEK_SET : len  = offset;                       break;
      case SEEK_CUR : len  = Tell() + offset;              break;
      case SEEK_END : len  = Ffilelength(Handle) + offset; break;
    }
    if(Flseek(Handle, offset, fromwhere) == len)
    {
      retcode = _TRUE;
    }
  }

  return(retcode);
}

_DWORD _Handle::Length(void)
{
  _DWORD retcode = -1;


  if(Handle >= 0)
  {
    retcode = Ffilelength(Handle);
  }

  return(retcode);
}

void _File::Init(void)
{
  _Handle::Init();
  Error       = H_NEVEROPEN;
  State       = NOSTATE;
  LseVer      = 0;
  MpcVer      = 0;
  LseCrc32    = DEFAULTCRC;

  RamData = NULL;
  RamSize = 0;
  RamWrite = 0;
  Offset  = 0;

}

void _File::Free(void)
{
  _Handle::Free();
  PushMemColor(MEMORY_PRG);
  Memory2((void**)&RamData, 0);
  PopMemColor();
  Init();
}

_UDWORD _File::GetHeaderSize(void)
{
  return(64);
}

_BOOL _File::GetError(void)
{
  return((Error == H_NOERROR)? _FALSE : _TRUE);
  //return((IsNoError_Makro())? _FALSE : _TRUE);
}

_FILE_ERROR _File::GetErrorState(void)
{
  return(Error);
}

void _File::SetError(_FILE_ERROR code)
{
  if((Error == H_NEVEROPEN) || (Error == H_NOERROR))
  {
    Error = code;
  }
}

/*
_BOOL _File::Open(_FILE_STATE command, _UNI *d, _UNI *p, _UNI *n, _UNI *e)
{
	_ASCII dn[_MAX_PATHLENGTH];
  _ASCII pn[_MAX_PATHLENGTH];
  _ASCII nn[_MAX_PATHLENGTH];
	_ASCII en[_MAX_PATHLENGTH];

	UniToAscii(dn, d, 0);
	UniToAscii(pn, p, 0);
	UniToAscii(nn, n, 0);
	UniToAscii(en, e, 0);
	return(Open(command, dn, pn, nn, en));
}

_BOOL _File::Open(_FILE_STATE command, _ASCII *d, _ASCII *p, _ASCII *n, _ASCII *e)
{
	_ASCII pathname[_MAX_PATHLENGTH];

	StrCpy(pathname, sizeof(_ASCII), d, sizeof(_ASCII));
	StrCat(pathname, sizeof(_ASCII), p, sizeof(_ASCII));
	StrCat(pathname, sizeof(_ASCII), n, sizeof(_ASCII));
	StrCat(pathname, sizeof(_ASCII), e, sizeof(_ASCII));

	return(Open(command, pathname));
}

_BOOL _File::Open(_FILE_STATE command, _UNI *dpne, _UDWORD expected_size)
{
	_ASCII fn[_MAX_PATHLENGTH];

	UniToAscii(fn, dpne, 0);
	return(Open(command, fn, expected_size));
}
*/

_BOOL _File::Decode(_UDWORD headersize)
{
  _BOOL retcode = _FALSE;

  if(GetError() == _FALSE)
  {
    if(IsRamFile() == _TRUE)
    {
      DeEnCode(RamData+headersize, RamSize-headersize, _FALSE);
      retcode = _TRUE;
    }
  }

  return(retcode);
}

_BOOL _File::Encode(_UDWORD headersize)
{
  _BOOL retcode = _FALSE;

  if(GetError() == _FALSE)
  {
    if(IsRamFile() == _TRUE)
    {
      DeEnCode(RamData+headersize, RamWrite-headersize, _TRUE);
      retcode = _TRUE;
    }
  }

  return(retcode);
}

_BOOL _File::Open(_FILE_STATE command, const _ASCII *dpne, _UDWORD expected_size, _BOOL openramfile)
{
  _BOOL   retcode = _FALSE;
  _BOOL   ram = _FALSE;
  _BOOL   ok = _TRUE;
  _DWORD  size;
  _UDWORD total, free;

  Init();

  if(ChkHandle() == _TRUE)
  {
    SetError(H_DEFINED);
  }
  else
  {
    if(::IsRamFile(dpne) == _TRUE)
    {
      dpne += 4;
      ram = _TRUE;
    }
    if(openramfile == _TRUE)
    {
      ram = _TRUE;
    }
    
    if(_Handle::Open(command, dpne) == _TRUE)
    {
      SetError(H_NOERROR);

      if(ram == _TRUE)
      {
        if((command == LOAD) || (command == LOADSAVE))
        {
          size = Length();
          // überprüfen ob genügend speicher frei ist um ram-file zu laden. es muss mindestens doppelt so
          // viel speicher frei sein.
          if(heap_size(&total, &free) == _TRUE)
          {
            free = free / 2;
            if(size >= free)
            {
//              ram = _FALSE;
              goto END_RAMOPEN;
            }
          }

          if(size != -1)
          {
            RamSize = size;
            if((size == 0)&&(command == LOADSAVE))
              RamSize = RAMFILESIZE;
            PushMemColor(MEMORY_PRG);
            if(Memory2((void**)&RamData, RamSize) != 0)
            {
              ok = _TRUE;
              if(size > 0)
              {
                ok = _FALSE;
                if(_Handle::Read(RamData, size) == _TRUE)
                {
                  if(command == LOADSAVE)
                    RamWrite = size;

                  ok = _TRUE;
                }
              }
            }
            PopMemColor();
          }
        }

        else if(command == SAVE)
        {
          PushMemColor(MEMORY_PRG);
          RamSize = (expected_size != 0)? expected_size : RAMFILESIZE;
          if(Memory2((void**)&RamData, RamSize) != 0)
          {
            RamWrite = 0;
            ok = _TRUE;
          }
          else
          {
            RamSize = 0;
//            ram = _FALSE;
            ok = _TRUE;
          }
          PopMemColor();
        }
      }

    END_RAMOPEN:

      if(ok == _TRUE)
      {
        retcode = _TRUE;
        State   = command;
      }
      else
      {
        Free();
      }
    }
    else
    {
      SetError(H_NOTOPEN);
    }
  }

  return(retcode);
}

_BOOL _File::Close(_BOOL msg)
{
	_BOOL retcode = _TRUE;


	if(GetError() == _TRUE)
	{
		if(msg == _TRUE)
		{
			ErrorMessage();
		}

		retcode = _FALSE;
	}

  if(retcode == _TRUE)
  {
    if(IsRamFile() == _TRUE)
    {
      if((State == SAVE) || (State == LOADSAVE))
      {
        retcode = _FALSE;

        _Handle::Seek(0, SEEK_SET);
        if(_Handle::Write(RamData, RamWrite) == _TRUE)
          retcode = _TRUE;
        else
          SetError(H_NOWRITE);
      }
    }
  }

  _Handle::Close();
	Free();

	return(retcode);
}

void* _File::PutOutMemory(void)
{
  void *retcode = NULL;

  if(IsRamFile() == _TRUE)
  {
    retcode = RamData;
    RamData = NULL;
    RamSize = 0;
  }

  return(retcode);
}

_BOOL _File::Read(void *dest, _UDWORD size)
{
//  Debug(Offset, 1);
  //if(GetError() == _FALSE)
  if(IsNoError_Makro())
  {
    if((State == LOAD)||(State == LOADSAVE))
    {
      //if(IsRamFile_Makro() == _TRUE)
      if(IsRamFile_Makro())
      {
        if((Offset + size) <= RamSize)
        {
         //#ifndef _LSL_TARGETARCH_ARM
          switch(size)
          {
            case 1 : 
              *(_UBYTE*)dest = *(_UBYTE*)&RamData[Offset]; 
              Offset += 1; 
              return(_TRUE);
            case 2 : 
              *(_UWORD*)dest = *(_UWORD*)&RamData[Offset]; 
              Offset += 2; 
              return(_TRUE);
            case 4 : 
              *(_UDWORD*)dest = *(_UDWORD*)&RamData[Offset]; 
              Offset += 4; 
              return(_TRUE);
          }
         //#endif
          MemCpy(dest, RamData+Offset, size);
          Offset += size;
          return _TRUE;
        }
      }
      else
      {
        if(_Handle::Read(dest, size) == _TRUE)
        {
          Offset += size;
          return _TRUE;
        }
      }
    }

    SetError(H_NOREAD);
  }

  return _FALSE;
}

_BOOL _File::Write(const void *src, _UDWORD size)
{
  _BOOL retcode = _FALSE;
  _BOOL ok = _TRUE;
  _UDWORD allocsize;

  if(GetError() == _FALSE)
  {
    if((State == SAVE)||(State == LOADSAVE))
    {
      if(IsRamFile() == _TRUE)
      {
        if((Offset + size) > RamSize)
        {
          if(size > RAMFILESIZE)
            allocsize = RamSize + size + RAMFILESIZE;
          else
            allocsize = RamSize + RAMFILESIZE;

          PushMemColor(MEMORY_PRG);
          if(Memory2((void**)&RamData, allocsize) != 0)
          {
            RamWrite += size;
            RamSize = allocsize;
          }
          else
          {
            ok = _FALSE;
            SetError(H_NOMEMORY);
          /*
            // speicher kann nicht mehr reallokiert werden
            // ramfile auf normales file umbauen und bereits gesicherte daten schreiben
            // vorsicht, bei toolcat darf dies nicht sein weil file-crc nur mit daten im ram berechnet werden kann
            _Handle::Seek(0, SEEK_SET);

            _UBYTE *trd = RamData;
            _UDWORD trs = RamWrite;

            RamData  = NULL;
            RamSize  = 0;
            RamWrite = 0;
            Offset   = 0;
            retcode  = Write(trd, trs);
            Memory2((void**)&trd, 0); // speicher freigeben
            if(retcode == _TRUE)
              retcode = Write(src, size);
            return(retcode);
         */
          }
          PopMemColor();
        }

        if(ok == _TRUE)
        {
          MemCpy(RamData+Offset, src, size);
          Offset += size;
          if(Offset > RamWrite)
            RamWrite = Offset;
          retcode = _TRUE;
        }

      }
      else
      {
        retcode = _Handle::Write(src, size);
        if(retcode == _TRUE)
          Offset += size;
        else
          SetError(H_NOWRITE);
      }
    }
    else
      SetError(H_NOWRITE);

  }

  return(retcode);
}

_DWORD _File::Length(void)
{
  _DWORD retcode = -1;

  if(GetError() == _FALSE)
  {
    if(IsRamFile() == _TRUE)
      retcode = RamSize;
    else
      retcode = _Handle::Length();

  }

  return(retcode);
}

_DWORD _File::Tell(void)
{
	_DWORD retcode = -1;

	if(GetError() == _FALSE)
	{
    if(IsRamFile() == _TRUE)
      retcode = Offset;
    else
      retcode = _Handle::Tell();

	}

	return(retcode);
}

_BOOL _File::Seek(_DWORD offs, _UWORD fromwhere)
{
  _BOOL retcode = _FALSE;
  _UDWORD newoffset;

  if(GetError() == _FALSE)
  {
    switch(fromwhere)
    {
      case SEEK_SET : newoffset = offs;            break;
      case SEEK_CUR : newoffset = Offset + offs;   break;
      case SEEK_END : if(offs > 0)
                        offs = -offs;
                      newoffset = Length() + offs; break;
    }

    if(IsRamFile() == _TRUE)
    {
      // <= because otherwise it is not possible to set the file pointer at the end of the file
      if(newoffset <= RamSize)
      {
        Offset = newoffset;
        retcode = _TRUE;
      }
    }
    else
    {
      retcode = _Handle::Seek(offs, fromwhere);
      if(retcode == _TRUE)
        Offset = newoffset;
    }
  }

  return(retcode);
}

_BOOL _File::Remove(_ASCII *dpne)
{
  _BOOL retcode;


  if(IsRamFile() == _TRUE)
    Close();

  retcode = _Handle::Remove(dpne);

  return(retcode);
}

void _File::ErrorMessage(void)
{
}

_UWORD _File::GetCompilerVersion(void)
{
  return(COMPILER_VERSION);
}

_UWORD _File::GetLseVersion(void)
{
  return((LseVer == 617)? 0 : LseVer);
}

_CRC32 _File::GetLseCrc32(void)
{
  return(LseCrc32);
}

_UWORD _File::GetMpcVersion(void)
{
  return(MpcVer);
}

_BOOL _File::CheckHeader(_UBYTE *head)
{
  _BOOL retcode = _FALSE;

  if((head[6] == 's')&&(head[16] == 'o')&&(head[18] == 'w'))
    retcode = _TRUE;

  return(retcode);
}

_BOOL _File::ReadHeaderSub(_UBYTE *head)
{
//   0 [4] .... chk-sum (crc32) exclusive header
//   4 [2] .... size of header
//   6 [8] .... "sigmatek"
//  16 [4] .... "okwh"
//  20 [4] .... lse version
//  24 [4] .... compiler version
//  28 [4] .... list id
//  32 [4] .... project id
//  60 [2] .... day
//  62 [2] .... month
//  64 [2] .... year
//  66 [2] .... sec
//  68 [2] .... min
//  70 [2] .... hour
//  80 [?] .... name of Picture, Window etc. (ascii-0-string)
//  all not defined bytes are filled with 0xFF

  _BOOL  retcode = _FALSE;

  if(GetError() == _FALSE)
  {
    if(Read(head, 128) == _TRUE)
    {
      if(CheckHeader(head) == _TRUE)
      {
        LseCrc32 = *(_CRC32*)&head[0];                 // crc32
        LseVer   = (_UWORD)(*((_UDWORD*)&head[20]));   // lse version
        MpcVer   = (_UWORD)(*((_UDWORD*)&head[24]));   // version ipc format
        retcode  = _TRUE;

       #if COMPILER_VERSION == 51
        if((MpcVer < 50)||(MpcVer > 51))
       #else
        if((MpcVer < COMPILER_VERSION_MIN)||(MpcVer > COMPILER_VERSION))
       #endif
        {
          retcode = _FALSE;
          SetError(H_WRONGLSE);
        }
      }
    }
  }

  return(retcode);
}

_BOOL _File::ReadHeader(void)
{
  _UBYTE head[128];
  return ReadHeaderSub(head);
}

_BOOL _File::ReadHeaderName(_ASCII *pname)
{
  _UBYTE head[128];
  _ASCII tmp[40];
  
  if(ReadHeaderSub(head) == _TRUE)
  {
    pname[0] = 0;
    MemCpy(tmp, &head[80], 32);
    tmp[31] = 0;
    StrCpy(pname, sizeof(_ASCII), tmp, sizeof(_ASCII));
    if(pname[0] != 0)
      return _TRUE;
  }
  return _FALSE;
}



_BOOL _File::IsRamFile(void)
{
  if((RamData != NULL) && (RamSize > 0))
    return _TRUE;

  return _FALSE;
}

_UDWORD _File::Make_CRC(_UDWORD headersize)
{
  _UDWORD retcode = 0;
  _UDWORD len;

  if(IsRamFile() == _TRUE)
  {
    if((State == SAVE)||(State == LOADSAVE))
    {
      if((RamWrite > GetHeaderSize()) && (CheckHeader(RamData) == _TRUE))
      {
        len = RamWrite - GetHeaderSize();
        retcode = MakeCrc32(0, RamData+GetHeaderSize(), len);
        *(_UDWORD*)&RamData[0] = retcode;
      }
      else
      {
        len = RamWrite - headersize;
        retcode = MakeCrc32(0, RamData+headersize, len);
      }
    }
    else
    {
      if((RamSize > GetHeaderSize()) && (CheckHeader(RamData) == _TRUE))
      {
        len = RamSize - GetHeaderSize();
        retcode = MakeCrc32(0, RamData+GetHeaderSize(), len);
      }
      else
      {
        len = RamSize - headersize;
        retcode = MakeCrc32(0, RamData+headersize, len);
      }
    }

  }

  return(retcode);
}

_BOOL _File::FileMemory(void **dest, _UDWORD size)
{
  if(GetError() == _FALSE)
  //if(IsNoError_Makro())
  {
    if(Memory2(dest, size) == 0)
    {
      SetError(H_NOMEMORY);
      return(_FALSE);
    }
    return(_TRUE);
  }

  return(_FALSE);
}

_BOOL _File::Read_COLOR(_COLOR *p)
{
  _BOOL retcode = _FALSE;

  if(GetError() == _FALSE)
  {
    retcode = Read(p, sizeof(_COLOR));
    
   #ifdef UC_NEWCOLORPALETTE 
     _UWORD chi = Palette.GetNewColor(*p >> 16);
     _UWORD clo = Palette.GetNewColor(*p & 0xFFFF);
     *p = (chi << 16) | clo;
   #endif
   
   #ifdef UC_GRAYSCALE
    _UWORD hi = ((*p) >> 16);
    transform_color_to_gray(&hi);
    
    _UWORD lo = ((*p) & 0xFFFF);
    transform_color_to_gray(&lo);
    *p = (hi << 16) | lo;
   #endif
    
//    *p = MakeColor_Makro(color_slurry(GetBackColor_Makro(*p)), color_slurry(GetPenColor_Makro(*p)));
  }

  return(retcode);
}

_BOOL _File::Read_HILOLIM(_HILOLIM *p)
{
 #if COMPILER_VERSION >= 46
  // [1] .... state   Lo4Bit = state von LoLimit, Hi4Bit = state von HiLimit
  // [4] .... LoLimit (nur wenn Lo4Bit von state <> 0)
  // [4] .... HiLimit (nur wenn Hi4Bit von state <> 0)
  init_HILOLIM(p);
  Read(&p->state, 1);

  if(p->state & 0x0F)
    Read(&p->lolim, 4);
  if(p->state & 0xF0)
    Read(&p->hilim, 4);

  return((GetError() == _TRUE)? _FALSE : _TRUE);
 #else
  _BOOL retcode;
  _VARLABEL hi, lo;
  // [_VARLABEL] ... LoLimit
  // [_VARLABEL] ... HiLimit
  Read_VARLABEL(&lo);
  retcode = Read_VARLABEL(&hi);
  p->state = (hi.info[0].state << 4) | lo.info[0].state;
  p->hilim = hi.info[0].value;
  p->lolim = lo.info[0].value;
  return(retcode);
 #endif
}

_BOOL _File::Read_VARSINGLE(_VARSINGLE *p)
{
  _UWORD data[3];

  if(Read(data, 6) == _TRUE)
  {
    p->state = (_DEFVAL)data[0];
    p->value = *(_DWORD*)&data[1];
    return(_TRUE);
  }
  return(_FALSE);
}

_BOOL _File::Read_VARLABEL(_VARLABEL *p)
{
  _BOOL  retcode = _FALSE;
  _UBYTE i, tmp;

  init_VARLABEL_Makro(p);

  if(GetError() == _FALSE)
  {
    if(Read(&tmp, sizeof(_UBYTE)) == _TRUE)
    {
     #ifdef PGTEST
      if(tmp > VL_MAXINDEX)
      {
        Annahme("T_FILE.CPP: FileSyntax", __LINE__);
        free_VARLABEL(p);
        return(_FALSE);
      }
     #endif

      _VARSINGLE *pi;
      for(i=0, retcode=_TRUE, pi=p->info; i<tmp; i++, pi++)
      {
        retcode = Read_VARSINGLE(pi); //&p->info[i]);

        if(retcode == _TRUE)
        {
          p->no ++;
        }
        else
        {
          free_VARLABEL(p);
          break;
        }
      }
    }
  }

  return(retcode);
}

_UWORD _File::Read_ENUM(void)
{
  _UWORD retcode = 0;
  Read(&retcode, sizeof(_UWORD));
  return(retcode);
}

_BOOL _File::Read_BOOL(void)
{
  _UWORD tmp;

  if(Read(&tmp, sizeof(_UWORD)) == _TRUE)
    return((tmp)? _TRUE : _FALSE);

  return(_FALSE);
}

_BOOL _File::Read_ANYTHING(_ANYTHING *p)
{
  _UWORD data[3];
 #if COMPILER_VERSION >= 32
  if(Read(data, 6) == _TRUE)
  {
    p->list = (_ANYLST)data[0];
    p->no   = *(_UDWORD*)&data[1];

    if((p->no == 0xFFFFFFFFL)||((p->no == 0xFFFF)&&((_DWORD)p->list >= 1000)))
      p->list = DEFLST;

    return(_TRUE);
  }
 #else
  if(Read(data, 4) == _TRUE)
  {
    p->list = (_ANYLST)data[0];
    p->no   = data[1];
    return(_TRUE);
  }
 #endif
  return(_FALSE);
}

_BOOL _File::Read_ANYPARA(_ANYPARA *p)
{
  init_ANYPARA_Makro(p);

  Read_ANYTHING(&p->anything);
 #if COMPILER_VERSION >= 23
  Read_MEM_VARLABEL(&p->memvarlabel);
 #endif

  if(GetError() == _TRUE)
  {
    free_ANYPARA(p);
    return(_FALSE);
  }

  return(_TRUE);
}


_BOOL _File::Read_CHKBIT(_CHKBIT *p)
{
  _UBYTE data[3];

  if(Read(data, 3) == _TRUE)
  {
	  p->seqno  = *(_UWORD*)data;
	  p->invert = data[2];
    return(_TRUE);
  }

  init_CHKBIT_Makro(p);
  return(_FALSE);
}

_BOOL _File::Read_XXCHKBIT(_XXCHKBIT *p)
{
  _UBYTE data[3];
  _UWORD tmp;

  if(Read(data, 3) == _TRUE)
  {
    tmp = (*(_UWORD*)&data[0]) & 0x7FFF;
    if(data[2])
      tmp |= 0x8000;

    *p = tmp;
    return(_TRUE);
  }

  init_XXCHKBIT(p);
  return(_FALSE);
}

#ifdef UC_LINKVL
_BOOL _File::Read_MEM_LINKVL(_MEM_LINKVL *p)
{
  _UWORD  i, no;

  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    if(no <= UC_LINKVL)
    {
     #ifdef UC_LINKVL_FAST
      _LINKVL dummy[UC_LINKVL];
      _LINKVL *ptr = VarList.LinkVlBuffer.Get(no, (_UDWORD*)&p->ptr);
      if(ptr == NULL)
        ptr = dummy;
      
      for(i=0; i<no; i++, ptr++)
        Read_LINKVL(ptr);
      p->no = no;
     #else   
      _UDWORD size = no * sizeof(_LINKVL);
      if(FileMemory((void**)&p->ptr, size) == _TRUE)
      {
        for(i=0; i<no; i++)
          Read_LINKVL(&p->ptr[i]);
        p->no = no;
      }
     #endif
    }
    else
    {
      SetError(H_NOREAD);
    }
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE);
}
#endif

_BOOL _File::Read_XMEM_ANYPARA(_XMEM_ANYPARA *p)
{
  _ANYPARA text0, text1, text2, text3;
  _UWORD   tmp = 0;

  Read_ANYPARA(&text0);                        // textno 0
  Read_ANYPARA(&text1);                        // textno 1
  Read_ANYPARA(&text2);                        // textno 2
  Read_ANYPARA(&text3);                        // textno 3

  if(GetError() == _FALSE)
  {
    if(text0.anything.list != DEFLST)
      tmp = 1;
    if(text1.anything.list != DEFLST)
      tmp = 2;
    if(text2.anything.list != DEFLST)
      tmp = 3;
    if(text3.anything.list != DEFLST)
      tmp = 4;

    if(tmp > 0)
    {
      if(FileMemory((void**)&p->xxptr, tmp * sizeof(_ANYPARA)) == _TRUE)
      {
        p->xxno = (_UBYTE)tmp;

        if(tmp == 1)
        {
          p->xxptr[0] = text0; init_ANYPARA_Makro(&text0);
        }
        else if(tmp == 2)
        {
          p->xxptr[0] = text0; init_ANYPARA_Makro(&text0);
          p->xxptr[1] = text1; init_ANYPARA_Makro(&text1);
        }
        else if(tmp == 3)
        {
          p->xxptr[0] = text0; init_ANYPARA_Makro(&text0);
          p->xxptr[1] = text1; init_ANYPARA_Makro(&text1);
          p->xxptr[2] = text2; init_ANYPARA_Makro(&text2);
        }
        else if(tmp == 4)
        {
          p->xxptr[0] = text0; init_ANYPARA_Makro(&text0);
          p->xxptr[1] = text1; init_ANYPARA_Makro(&text1);
          p->xxptr[2] = text2; init_ANYPARA_Makro(&text2);
          p->xxptr[3] = text3; init_ANYPARA_Makro(&text3);
        }
      }
    }
  }

  free_ANYPARA(&text0);
  free_ANYPARA(&text1);
  free_ANYPARA(&text2);
  free_ANYPARA(&text3);

  return((GetError()==_TRUE)? _FALSE : _TRUE);
}

_BOOL _File::Read_LSEVARINFO(_LSEVARINFO *p)
{
  // erst ab compilerversion 30
  _BOOL  retcode = _TRUE;
  _UBYTE data[17];

  init_LSEVARINFO_Makro(p);

  if(GetError() == _FALSE)
  {
    p->vartype = (_VARTYPE)Read_ENUM();
    
   #if COMPILER_VERSION >= 43
    {
     #ifdef UC_COMPLEX_SERVER
      {
        if(p->vartype & 0x40)
        {
          p->POWPRUIC |= VARBIT_STRUCTELEMENT;
        }
      }
     #endif

     #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
      {
        if(p->vartype & 0x80)
        {
          p->POWPRUIC |= VARBIT_VARGLOBAL;
          p->POWPRUIC &= ~VARBIT_STRUCTELEMENT;
        }
      }
     #endif
    }
   #endif

    p->vartype &= 0x3F;

    if(p->vartype == UNUSED2) 
    {
      // bei alter compilerversion (zB.41) wurde wenn Server eine Struktur ist bei struct-component dieser type (12) eingetragen 
      p->vartype = BINARY;
      p->POWPRUIC |= VARBIT_STRUCTELEMENT;
    }

    #if COMPILER_VERSION >= 32
    {
      Read(data, sizeof(_UDWORD));
      p->classid = *(_UWORD*)&data[0];
      
      #ifdef PGTEST
      {
        if((*(_UDWORD*)&data[0] < 0xFFFFFFFF)&&(*(_UDWORD*)&data[0] > 0xFFFF))
        {
          Annahme("T_FILE.CPP: Classid too big", __LINE__);
        }
      }
      #endif
    }
    #else
    {
      p->classid = 0;
      Read(&p->classid, sizeof(_UWORD));
    }
    #endif

    Read_HILOLIM(&p->hilolim);

    #if COMPILER_VERSION >= 46
    {
      Read(data, 17);
      p->userconfig      = *(_UWORD*)&data[0];
      p->accessno        = *(_UWORD*)&data[2];
      p->format          = *(_UWORD*)&data[4];
      p->station         = data[6];
      if((p->vartype == BINARY) || (p->vartype == FCONSTANT))
      {
        p->funit         = *(_VUNIT*)&data[8]; // auf data[9] steht HiByte von unitinfo !!
       #ifndef UNIT_2Byte 
        if(((*(_UWORD*)&data[8]) > 255)&&((*(_UWORD*)&data[8]) < 0xFFFF))
        {
          Annahme("T_FILE.CPP: More than 255 units", __LINE__);
          while(1) {};
        }
       #endif
      }
      p->statistic.group = *(_UDWORD*)&data[10];
      p->POWPRUIC       |= (data[14] & (VARBIT_INVERT|VARBIT_PERMANENT|VARBIT_PREZERO)); //0x46);
      p->reftime_bitno   = make_reftime_bitno(*(_UWORD*)&data[15], data[7]);
    }
    #else
    {
      Read(data, 16);
      p->userconfig      = *(_UWORD*)&data[0];
      p->accessno        = *(_UWORD*)&data[2];
      p->format          = *(_UWORD*)&data[4];
      p->station         = data[6];
      if((p->vartype == BINARY) || (p->vartype == FCONSTANT))
        p->funit         = data[8];
      p->statistic.group = *(_UDWORD*)&data[9];
      p->POWPRUIC       |= (data[13] & (VARBIT_INVERT|VARBIT_PERMANENT|VARBIT_PREZERO));
      p->reftime_bitno   = make_reftime_bitno(*(_UWORD*)&data[14], data[7]);
    }
    #endif

    Read_XMEM_ANYPARA(&p->xmemtext);

    Read(data, 14);
    if((p->vartype == BINARY)||(p->vartype == FCONSTANT))
    {
      p->funitscheme       = *(_UWORD*)&data[0];
    }
    p->statistic.parameter = *(_UDWORD*)&data[2];
    p->stscheme            = *(_UWORD*)&data[6];
    p->crc32               = *(_UDWORD*)&data[8];
    p->uimenuindex         = *(_UWORD*)&data[12];

    Read_XXCHKBIT(&p->xxchkbit);

   #ifdef UC_LINKVL
    Read_MEM_LINKVL(&p->memlinkvl);
   #else
    _UWORD  i, no;
    _LINKVL tmplvl;
    if(Read(&no, sizeof(_UWORD)) == _TRUE)
    {
      if(no > 0)
      {
        init_LINKVL(&tmplvl);
        for(i=0; i<no; i++)
        {
          Read_LINKVL(&tmplvl);
          free_LINKVL(&tmplvl);
        }
      }
    }
   #endif

   #if COMPILER_VERSION >= 35
    Read(&p->physic, sizeof(_UBYTE));
    Read(data, sizeof(_UDWORD));
    p->hostname = *(_UWORD*)data;
    #ifdef PGTEST
     if((*(_UDWORD*)data != 0xFFFFFFFF)&&(*(_UDWORD*)data > 0xFFFF))
     {
       Annahme("T_FILE.CPP: Hostname too big", __LINE__);
     }
    #endif
   #endif
   
    if(MpcVer >= 60) // #if COMPILER_VERSION >= 60
    {
   // signed 32 bit für offset von membervariablen
     #ifdef UC_MEMBERVARIABLE
      Read(&p->memberoffset, sizeof(_DWORD));
     #else
      Read(data, sizeof(_DWORD));
     #endif
    }
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_LSEVARINFO(p);
  }

 #ifdef USER_DEKEMA
  if(p->vartype == FTIME)
  {
    if(p->statistic.parameter & 0x8000)
      p->format = SIZE_MMSS;
  }
 #endif

 #ifdef UC_MULTICPU
  // es kann keine permanenten server geben
  p->POWPRUIC &= ~VARBIT_PERMANENT;
 #endif

  if(p->vartype == FSTRING)
  {
    p->lasalid.vtype = TYP_EXTENDED;
  }

  return(retcode);
}


_BOOL _File::Read_VARSYMBOL(_VARSYMBOL *p)
{
  _BOOL  retcode = _FALSE;

  init_VARSYMBOL_Makro(p);

  if(GetError() == _FALSE)
  {
   #if COMPILER_VERSION >= 32
    retcode = Read(&p->scope, sizeof(_UDWORD));
   #else
    _UWORD scope;
    retcode = Read(&scope, sizeof(_UWORD));
    p->scope = scope;
    if(scope == 0xFFFF)
      p->scope = DEFSCOPE;
   #endif

    if(retcode == _FALSE)
      free_VARSYMBOL(p);
  }

  return(retcode);
}

_BOOL _File::Read_ATTRIB(_ATTRIB *p)
{
  _BOOL retcode;

  retcode = Read(p, sizeof(_ATTRIB));
//  *p &= ~(T_ITALIC|T_BOLD|T_UNDERLINE|T_STRIKEOUT);
//  *p |= (T_ITALIC|T_BOLD); //|T_STRIKEOUT|T_UNDERLINE);

	return(retcode);
}

_BOOL _File::Read_ACCESS(_ACCESS *p)
{
  _BOOL retcode = _FALSE;
  _UWORD tmp;

  *p = 0;
  tmp = 0;

 #if COMPILER_VERSION >= 44
  if(Read(&tmp, sizeof(_UWORD)) == _TRUE)
 #else
  if(Read(&tmp, sizeof(_UBYTE)) == _TRUE)
 #endif
  {
   #ifdef UC_ACCESSNO_2BYTE
    *p = (_UWORD)tmp;
   #else
    *p = (_UBYTE)tmp;
   #endif
    retcode = _TRUE;
  }

  return(retcode);
}


#if COMPILER_VERSION >= 55
_BOOL _File::ReadMomentaryStationary(_BOOL *act_momentary, _DWORD *set_val, _DWORD *reset_val)
{
  _UBYTE tmp = 0;
  _BOOL  retcode = Read(&tmp, 1);
  
  if(tmp & 0x80)
  {
    Read(set_val, 4);
    retcode = Read(reset_val, 4);
  }
  
  *act_momentary = (tmp & 0x01)? _TRUE : _FALSE;
 
  return retcode;
}
#endif


_BOOL _File::Read_SINGLESOFT(_SINGLESOFT *p)
{
  _BOOL retcode = _FALSE;

  init_SINGLESOFT(p);

  if(GetError() == _FALSE)
  {
    Read_ATTRIB(&p->attrib);
    Read_COLOR(&p->color);
    Read_COLOR(&p->frame);
    Read_COLOR(&p->text);
    p->tip = (_TIP)Read_ENUM();
    Read_ANYPARA(&p->anypara);
    Read_CHKBIT(&p->chkbit);
    Read_ACCESS(&p->access);
    Read(&p->font, sizeof(_FONT));
    Read_GFUNCT(&p->gfunct);
//  ab COMPILER_VERSION 20
    Read_FRAMETYPE(&p->frametype);

   #if COMPILER_VERSION >= 28
    Read_BUTTONIMAGES(&p->buttonimages);
    Read_VARLABEL(&p->varlabel); free_VARLABEL(&p->varlabel);
    Read_VARLABEL(&p->varlabel);  // momentary - stationary

    _MYSCHEME    tmp;
    Read_MYSCHEME(&tmp);

   #if COMPILER_VERSION < 41
    free_MYSCHEME(&tmp);
    _ANYTHING tmpat;
    init_ANYTHING_Makro(&tmpat);
    Read_ANYTHING(&tmpat);
    free_ANYTHING(&tmpat);
   #endif

    Read(&p->valid, sizeof(_UBYTE));
   #endif


   #if COMPILER_VERSION >= 41
   #ifdef UC_DYN_SOFTKEYS
    _BOOL ret;
    copy_MYSCHEME(&ret, &p->stscheme, &tmp);
    free_MYSCHEME(&tmp);

    Read_LINKSCHEME(&p->dyncolback);
    Read_LINKSCHEME(&p->dyncolfront);
    Read_LINKSCHEME(&p->dyncoltl);
    Read_LINKSCHEME(&p->dyncolbr);
    Read_LINKSCHEME(&p->dyncoltxt);

   #else
    free_MYSCHEME(&tmp);
    _LINKSCHEME linkscheme;
    init_LINKSCHEME_Makro(&linkscheme);
    Read_LINKSCHEME(&linkscheme);
    free_LINKSCHEME(&linkscheme);
    Read_LINKSCHEME(&linkscheme);
    free_LINKSCHEME(&linkscheme);
    Read_LINKSCHEME(&linkscheme);
    free_LINKSCHEME(&linkscheme);
    Read_LINKSCHEME(&linkscheme);
    free_LINKSCHEME(&linkscheme);
    Read_LINKSCHEME(&linkscheme);
    free_LINKSCHEME(&linkscheme);
   #endif
   #endif

   #if COMPILER_VERSION >= 49
    Read(&p->wose, sizeof(_UBYTE));
   #endif
   
   #if COMPILER_VERSION >= 55
    ReadMomentaryStationary(&p->act_momentary, &p->set_val, &p->reset_val);
   #endif
   
   #if COMPILER_VERSION >= 57
    Read_ANYPARA(&p->newbubbletext);
   #endif
  }


  if(GetError() == _TRUE)
    free_SINGLESOFT(p);
  else
    retcode = _TRUE;

  return(retcode);
}

_BOOL _File::Read_LSEFUNCT(_LSEFUNCT *p)
{
  init_LSEFUNCT_Makro(p);
  return(Read(&p->no, sizeof(_UWORD)));
}

_BOOL _File::Read_GFUNCT(_GFUNCT *p)
{
  _BOOL retcode = _TRUE;
//  _LSEFUNCT tmp;

  init_GFUNCT_Makro(p);

  if(GetError() == _FALSE)
  {
    Read_LSEFUNCT(&p->pos_fl);
    Read_LSEFUNCT(&p->neg_fl);
//    Read_LSEFUNCT(&tmp);  // raus ab COMPILER_VERSION 20
//    Read_LSEFUNCT(&tmp);  // raus ab COMPILER_VERSION 20
//    Read_LSEFUNCT(&p->active);
//    Read_LSEFUNCT(&p->inactive);
  }

  if(GetError() == _FALSE)
    retcode = _TRUE;

  return(retcode); 
} 

_BOOL _File::Read_SOFTFLOOR(_SOFTFLOOR *ps, _UWORD no)
{
  _BOOL       retcode = _FALSE;
  _UWORD      i;
  _SINGLESOFT *pss;

  init_SOFTFLOOR(ps);

  if(no <= NO_SOFT_FLOOR)
  {
    if(GetError() == _FALSE)
    {
      for(i=0, pss=ps->floor, retcode=_TRUE; i<no; i++, pss++)
        retcode = Read_SINGLESOFT(pss);
    }

    if(retcode == _FALSE) 
      free_SOFTFLOOR(ps); 
  }

  return(retcode);
}

_UWORD get_nooperands(_PIC_COMMAND cmd)
{
  _UWORD retcode = 0;

  switch(cmd)
  {
//    case CMD_NOP,          // nothing
//    case CMD_START,        // start with first picture
//    case CMD_NEWPICRET,    // open new (previous) picture
//    case CMD_EXIT,         // exit window without backgroundrefresh
//    case CMD_REDRAW,       // redraw picture or window + softkey
//    case CMD_OUTSOFTKEY,   // redraw softkey
//    case CMD_CLOSE,        // close picture or window
//    case CMD_KILLEDITOR,   // kill editor without save
//    case CMD_RETURN,       // return to previous
//    case CMD_SAVEEDITOR,   // save editor
//    case CMD_ELSE,         // else
//    case CMD_END,          // end

//    case CMD_NEWPICTURE  : // raus ab COMPILER_VERSION 20
    case CMD_BEEP          : // beep
    case CMD_NEWSOFTKEY    : // change to defined floor and draw
    case CMD_NEWLANGUAGE   : // change to other language
    case CMD_EDIT          : // seek input and open editor
    case CMD_EDITNO        : // open editor input[no]
    case CMD_SCANCODE      : // set scancode for keyboard
    case CMD_INC           : // value ++
    case CMD_DEC           : // value --
    case CMD_TOGGLE        : // 1-0-1
    case CMD_NEWPOPUP      : // open mae-menue
    case CMD_OVERLOAD      : // overload
    case CMD_FUNCTIONBLOCK : // call functionblock

                            retcode = 1;
                            break;
    case CMD_NEWWINDOW    : // open new window
    case CMD_IFE          : // if ==
    case CMD_IFG          : // if >
    case CMD_IFS          : // if <
    case CMD_ADD          : // add value to variable
    case CMD_SUB          : // sub value from variable
    case CMD_CALL         : // call userinterface
    case CMD_SET          : // set value
    case CMD_SCRNSCHEME   : // call screen via scheme
    case CMD_NEWPICTURE   : // open new picture // ab COMPILER_VERSION 20
    case CMD_OVERLOAD_OBJ : // overload just defined object
    case CMD_OVERLOAD_SUMMARY : // overload object with defined summary (2)
    case CMD_LED :          // switch led on/off/blink
                            retcode = 2;
                            break;
    default               : break;
  }

  return(retcode);
}

_BOOL _File::Read_STACK_LINE(_STACK_LINE *p)
{
  _BOOL retcode = _FALSE;

  init_STACK_LINE_Makro(p);

  if(GetError() == _FALSE)
  {
    p->cmd = (_PIC_COMMAND)Read_ENUM();

    if(GetError() == _FALSE)
    {
      retcode = _TRUE;
      switch(get_nooperands(p->cmd))
      {
        case 1 : retcode = Read_VARLABEL(&p->op1);
                 break;
        case 2 : Read_VARLABEL(&p->op1);
                 retcode = Read_VARLABEL(&p->op2);
                 break;
      }
    }
  }

  if(retcode == _FALSE)
    free_STACK_LINE(p);

  return(retcode);
}

_BOOL _File::Read_RECTANGLE(_RECTANGLE *p)
{
  init_RECTANGLE(p);

  if(GetError() == _FALSE)
  {
    Read_CHKBIT(&p->chkbit);
    Read_ATTRIB(&p->attrib);
    Read_COLOR(&p->color);
    Read_COLOR(&p->colorframe);
    Read_ROOM(&p->room);
//  ab COMPILER_VERSION 20
    Read_FRAMETYPE(&p->frametype);
  }

  if(GetError() == _TRUE)
  {
    free_RECTANGLE(p);
    return _FALSE;
  }
  
  return _TRUE;
}

_BOOL _File::Read_CIRCLE(_CIRCLE *p)
{
	init_CIRCLE(p);

	if(GetError() == _FALSE)
	{
		Read_CHKBIT(&p->chkbit);
		Read_ATTRIB(&p->attrib);
		Read_COLOR(&p->color);
		Read_COLOR(&p->colorframe);
		Read_ROOM(&p->room);
		Read(&p->angle, sizeof(_WORD));
		Read(&p->arc, sizeof(_WORD));
	}

	if(GetError() == _TRUE)
	{
		free_CIRCLE(p);
    return _FALSE;
	}

	return _TRUE;
}

/*
void _File::PrepaireRoom(void)
{
  Room.xy1.x =  32000;
  Room.xy1.y =  32000;
  Room.xy2.x = -32000;
  Room.xy2.y = -32000;
}

void _File::GetRoom(_ROOM *pr)
{
  *pr = Room;
}
*/

_BOOL _File::Read_DOT(_DOT *p)
{
  _BOOL retcode = Read(p, sizeof(_DOT));
 #ifdef ZOOM_KM
  if(DoZoomKM == 0)
  {
    p->x = (_KOORD)(((((_DWORD)p->x - KOORDINATE_REF_X) * KOORDINATE_MUL_X)/KOORDINATE_DIV_X) + KOORDINATE_REF_X + KOORDINATE_OFF_X);
    p->y = (_KOORD)(((((_DWORD)p->y - KOORDINATE_REF_Y)* KOORDINATE_MUL_Y)/KOORDINATE_DIV_Y) + KOORDINATE_REF_Y + KOORDINATE_OFF_Y);
  }
 #endif
/*
  if(retcode == _TRUE)
  {
    if(p->x > Room.xy2.x) Room.xy2.x = p->x;
    if(p->x < Room.xy1.x) Room.xy1.x = p->x;
    if(p->y > Room.xy2.y) Room.xy2.y = p->y;
    if(p->y < Room.xy1.y) Room.xy1.y = p->y;
  }
*/
  return(retcode);
}

_BOOL _File::Read_POLYGON(_POLYGON *p)
{
  _BOOL   retcode = _FALSE;
  _UWORD  no;
  _UDWORD size;

  init_POLYGON(p);

  if(GetError() == _FALSE)
  {
    Read_CHKBIT(&p->chkbit);
    Read_ATTRIB(&p->attrib);
		Read_COLOR(&p->color);
		Read_COLOR(&p->colorframe);
		if(Read(&no, sizeof(_UWORD)) == _TRUE)
		{
			size = no * sizeof(_DOT);
			if(FileMemory((void**)&p->ptr, size) == _TRUE)
			{
				p->no = no;
        while(no)
        {
          Read_DOT(&p->ptr[p->no-no]);
          no -= 1;
        }
				retcode = _TRUE;
			}
		}
	}

  if(retcode == _FALSE)
    free_POLYGON(p);

  return(retcode);
}

_BOOL _File::Read_PICTEXT(_PICTEXT *p)
{
	init_PICTEXT(p);

	if(GetError() == _FALSE)
	{
		Read_CHKBIT(&p->chkbit);
		Read_ATTRIB(&p->attrib);
		Read_COLOR(&p->colback);
		Read_COLOR(&p->colframe);
		Read_COLOR(&p->coltxt);
		Read_ROOM(&p->room);
		Read_ANYPARA(&p->anypara);
		Read(&p->font, sizeof(_FONT));
		Read(&p->summary, sizeof(_UWORD));
    Read_MYSCHEME(&p->stscheme);    // ab COMPILER_VERSION 20
    Read_FRAMETYPE(&p->frametype);  // ab COMPILER_VERSION 20
    Read(&p->fillchr, sizeof(_UNI)); // ab COMPILER_VERSION 21
   #if COMPILER_VERSION >= 31
		Read(&p->doimage, 1);
   #endif
   #if COMPILER_VERSION >= 33
    Read(&p->lock_overload, 1);
   #endif
	}

  if(GetError() == _TRUE)
  {
		free_PICTEXT(p);
	  return _FALSE;
  }
  
	return _TRUE;
}

_BOOL _File::Read_ZOOM(_ZOOM *p)
{
  if(Read(p, sizeof(_ZOOM)) == _FALSE)
  {
    init_ZOOM(p); 
  }

 #ifdef ZOOM_KM
  if(p->x == DEFZOOM)
    p->x = 1000;
  if(p->y == DEFZOOM)
    p->y = 1000;

  if(DoZoomKM == 0)
  {
    p->x = (_WORD)(((_DWORD)p->x * KOORDINATE_MUL_X) / KOORDINATE_DIV_X);
    p->y = (_WORD)(((_DWORD)p->y * KOORDINATE_MUL_Y) / KOORDINATE_DIV_Y);
  }
 #endif

	return _TRUE;
}

_BOOL _File::Read_OBJECT(_OBJECT *p)
{
	_BOOL retcode = _TRUE;

	init_OBJECT(p);

	if(GetError() == _FALSE)
	{
    Read_CHKBIT(&p->chkbit);
    Read_ATTRIB(&p->attrib);
    Read_DOT(&p->xy);
   #if COMPILER_VERSION >= 50
    _DOT tmpdotxe;
    Read_DOT(&tmpdotxe);
    // diese rechte untere koordinate wird für die ermittlung der tatsächlichen grösse beim compilieren benötigt
   #endif
    Read(&p->no, sizeof(_UWORD));
    Read_ZOOM(&p->zoom);
//    Read_COLOR(&color); // raus ab COMPILER_VERSION 20
		Read_COLOR(&p->colorframe);

    // ab COMPILER_VERSION 18
		Read_VARLABEL(&p->loader);

    // ab COMPILER_VERSION 20
		Read_MYSCHEME(&p->stscheme);

//    p->summary = 1;
   #if COMPILER_VERSION >= 25
    Read_DIRECTION(&p->direction);
   #endif
   #if COMPILER_VERSION >= 26
    Read(&p->summary, sizeof(_UWORD));
   #endif
   #if COMPILER_VERSION >= 31
		Read(&p->doimage, 1);
   #endif
   #if COMPILER_VERSION >= 33
    Read(&p->lock_overload, 1);
   #endif
   #if COMPILER_VERSION >= 46
    Read_MYSCHEME(&p->loader_scheme);
//    p->loader_scheme.schemeno = 0;
   #endif
	}

	if(GetError() == _TRUE)
	{
		free_OBJECT(p);
		retcode = _FALSE;
	}

	return(retcode);
}

_BOOL _File::Read_BITMAP(_BITMAP *p)
{
	_BOOL retcode = _FALSE;

	init_BITMAP(p);

	if(GetError() == _FALSE)
	{
		Read_CHKBIT(&p->chkbit);
		Read_ATTRIB(&p->attrib);
		Read_COLOR(&p->color);
		Read_COLOR(&p->colorframe);
		Read_ROOM(&p->room);
		retcode = Read(&p->no, sizeof(_UWORD));
	}

  if(retcode == _FALSE)
    free_BITMAP(p);

  return(retcode);
}

_BOOL _File::Read_ROOM(_ROOM *p)
{
  Read_DOT(&p->xy1);
  return(Read_DOT(&p->xy2));
}

_BOOL _File::Read_DIRECTION(_DIRECTION *p)
{
  _UWORD data[2];

  init_DIRECTION(p);
  if(Read(data, 4) == _TRUE)
  {
    p->h_next = data[0];
    p->v_next = data[1];
    return(_TRUE);
  }
  return(_FALSE);
}

_BOOL _File::Read_COMBOINFO(_COMBOINFO *p)
{
  _UWORD tmp;
  Read(&p->lineheight, 2);
  Read_FRAMETYPE(&p->frame);
  if(Read(&tmp, 2) == _TRUE)
  {
    p->linesperpage = (_UBYTE)tmp;
    return _TRUE;
  }
  
  return _FALSE;
}

_BOOL _File::Read_INPUT(_INPUT *p)
{
  _UWORD tmpacc;

  init_INPUT_Makro(p);

  if(Read_IO(&p->io) == _TRUE)
  {
    p->enter_plus = (_ENTER_PLUS)Read_ENUM();

    Read(&tmpacc, sizeof(_UBYTE));
   #if COMPILER_VERSION < 25
    Read_DIRECTION(&p->io.direction);
   #endif

   #if COMPILER_VERSION >= 38
    Read(&p->searchchoice, sizeof(p->searchchoice));
   #endif

   #if COMPILER_VERSION >= 48
    Read(&p->selector, sizeof(_UBYTE));
    
    if(p->io.theurge != NULL)
    {
      if(p->io.theurge->urgetype == _URGE_SLIDER)
      {
        p->selector = _SELECTOR_ONLY_HID; // slider kann nur mit maus und/oder touch angewählt werden
      }
    }
   #endif
   #if COMPILER_VERSION >= 59
    if(p->io.typ == AS_ENUMERATION)
    {
      Read_COMBOINFO(&p->comboinfo);
    }
   #endif
  }

  if(GetError() == _TRUE)
  {
    free_INPUT(p);
    return _FALSE;
  }
  
  return _TRUE;
}

_BOOL _File::Read_IOINFO(_IOINFO *p, _IO_TYPE *typ)
{
  _UWORD no = 6;
  _UBYTE tmp;
  _UBYTE dummy[50];
  _BOOL  retcode = _TRUE;

  init_IOINFO_Makro(p);

  if(GetError() == _FALSE)
  {
    switch(*typ)
    {
      case AS_BAR   : p->tbo.list = BAR_IO;
                      if(Read(&tmp, sizeof(_UBYTE)) == _TRUE)
                        p->tbo.no = tmp;
                      no -= 1;
                      break;
      case AS_MENU  :
      case AS_TBO   : Read_ANYTHING(&p->tbo);
                      no -= 4;
                      // vorsicht: umbau auf io-type AS_TXTSCHEME
                      if(p->tbo.list == TEXTSCHEME)
                      {
                        *typ = AS_TXTSCHEME;
                        p->mytxtscheme.schemeno = p->tbo.no;
                        free_ANYTHING(&p->tbo);
                      }
                      break;
      default       : break;
    }

    Read(dummy, no);
//    while(no > 0)
//    {
//      Read(&tmp, sizeof(_UBYTE));
//      no --;
//    }
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_IOINFO(p);
  }
  else
  {
    switch(p->tbo.list)
    {
      case OBJECTSCHEME :
      case TEXTSCHEME   :
      case IMAGESCHEME  : p->myscheme.schemeno = p->tbo.no; break;
      default           : init_MYSCHEME_Makro(&p->myscheme); break;
    }
  }

  return(retcode);
}

_BOOL _File::Read_PICFORMAT(_PICFORMAT *p)
{
  _BOOL  retcode = _TRUE;
  _UBYTE data[3];

  init_PICFORMAT(p);

  if(GetError() == _FALSE)
  {
    Read(data, 3);
    p->format = *(_UWORD*)data;
    p->digits = data[2];
//    Read(&p->format, sizeof(_UWORD));
//    Read(&p->digits, sizeof(_UBYTE));
    retcode = Read_VARLABEL(&p->posdezpt);
  }

  if(retcode == _FALSE)
    free_PICFORMAT(p);

  return(retcode);
}

_BOOL _File::Read_VIRTNAME(_VIRTNAME *p)
{
  _BOOL  retcode = _TRUE;
  _UWORD tmp;

  init_VIRTNAME(p);

	if(GetError() == _FALSE)
  {
		Read(&tmp, sizeof(_UWORD));
	 #ifdef OK_ENCLOSE_NAME
		p->no = tmp;
	 #endif
  }

	if(GetError() == _TRUE)
	{
		retcode = _FALSE;
		free_VIRTNAME(p);
	}

  return(retcode);
}

_BOOL _File::Read_IO(_IO *p)
{
	init_IO_Makro(p);

	if(GetError() == _FALSE)
	{
		Read_CHKBIT(&p->chkbit);
		Read_ATTRIB(&p->attrib);
		Read_COLOR(&p->colback);
		Read_COLOR(&p->colframe);
		Read_COLOR(&p->coltxt);
		Read_ROOM(&p->room);
		p->typ = (_IO_TYPE)Read_ENUM();
		Read_VARLABEL(&p->variable);
		Read(&p->font, sizeof(_FONT));
		Read_IOINFO(&p->info, &p->typ);
		Read_PICFORMAT(&p->picformat);
		Read(&p->summary, sizeof(_UWORD));
    Read_VIRTNAME(&p->virtname);
    p->is_dyncolor = Read_BOOL();
    Read_LINKSCHEME(&p->dyncolback);    // color background (hatch)
    Read_LINKSCHEME(&p->dyncolfront);   // color background (hatch)
    Read_LINKSCHEME(&p->dyncoltl);      // color frame (top-left)
    Read_LINKSCHEME(&p->dyncolbr);      // color frame (bottom-right)
    Read_LINKSCHEME(&p->dyncoltxt);     // color text
    // ab COMPILER_VERSION 20
    Read_MYSCHEME(&p->stscheme);
    // ab COMPILER_VERSION 20
    Read_FRAMETYPE(&p->frametype);
    // ab COMPILER_VERSION 20
    Read_LSEFUNCT(&p->lsefunct);

   #if COMPILER_VERSION >= 22
    Read(&p->fillchr, sizeof(_UNI));
   #endif
   #if COMPILER_VERSION >= 25
    Read_DIRECTION(&p->direction);
    if(p->typ == AS_THEURGE)
    {
      if(FileMemory((void**)&p->theurge, sizeof(_THEURGE)) == _TRUE)
      {
        if(Read_THEURGE(p->theurge) == _TRUE)
        {
          if(p->theurge->memimage.no != 0)
          {
//            if((p->theurge->memimage.ptr[0].list == ICONLST) && (p->theurge->memimage.ptr[0].no != 0xffff))
//              p->colback = DEFCOLOR;
          }
        }
        else
        {
          FileMemory((void**)&p->theurge, 0);
          p->theurge = NULL;
        }
      }
    }
   #endif
   #if COMPILER_VERSION >= 31
    Read(&p->doimage, 1);
   #endif
   #if COMPILER_VERSION >= 33
    Read(&p->lock_overload, 1);
   #endif
   #if COMPILER_VERSION >= 57
    Read_ANYPARA(&p->newbubbletext);
   #endif
	}

	if(GetError() == _TRUE)
	{
		free_IO(p);
		return _FALSE;
	}
 #ifdef ZOOM_KM
  else
  {
    if(DoZoomKM == 0)
    {
      p->kmz.x = (_WORD)(((_DWORD)KOORDINATE_MUL_X * 1000L) / (KOORDINATE_DIV_X));
      p->kmz.y = (_WORD)(((_DWORD)KOORDINATE_MUL_Y * 1000L) / (KOORDINATE_DIV_Y));
    }
  }
 #endif

	return _TRUE;
}

_BOOL _File::Read_UNIT(_UNIT *p)
{
  _BOOL retcode = _TRUE;

  init_UNIT(p);

  if(GetError() == _FALSE)
  {
   #if COMPILER_VERSION >= 46
    Read_VARLABEL(&p->xdigits);
   #else
    _UBYTE dig;
    Read(&dig, sizeof(_UBYTE));
    p->xdigits.no = 1;
    p->xdigits.info[0].state = CONST_VAL;
    p->xdigits.info[0].value = dig;
   #endif
    Read_VARLABEL(&p->posdezpt);
    Read(&p->formula, sizeof(_UBYTE));
    Read_VARLABEL(&p->parameter);
    Read(&p->fixdp, sizeof(_UBYTE));
    Read_ANYPARA(&p->anypara);
    
    if(MpcVer >= 60) // #if COMPILER_VERSION >= 60
    {
      _UBYTE tmp08;
      Read(&tmp08, sizeof(_UBYTE));
      p->leading0 = tmp08 & 0x03; // nur die untersten 2 bits, rest ist unbenützt und mit 0 initialisiert
    } 
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_UNIT(p);
  }

  return(retcode);
}

_BOOL _File::Read_MEM_UNIT(_MEM_UNIT *p)
{
  _BOOL  retcode = _TRUE;
  _UWORD i, tmp;
  _DWORD size;
  _UNIT  *pu;

  init_MEM_UNIT(p);

  if(GetError() == _FALSE)
  {
    if(Read(&tmp, (sizeof(_UWORD))) == _TRUE)
    {
      size = tmp * sizeof(_UNIT);
      if(FileMemory((void**)&p->ptr, size) != 0)
      {
        p->no = tmp;
        for(i=0, pu=p->ptr; i<tmp; i++,pu++)
          Read_UNIT(pu);
      }
    }
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_MEM_UNIT(p);
  }

  return(retcode);
}

_BOOL _File::Read_MOTION(_MOTION *p)
{
  _BOOL retcode = _TRUE;

  init_MOTION(p);

  if(GetError() == _FALSE)
  {
    Read_CHKBIT(&p->chkbit);
    Read_ROOM(&p->room);
    Read_ATTRIB(&p->attrib);
    Read_ANYTHING(&p->anything);
    Read_VARLABEL(&p->varx);
    Read_VARLABEL(&p->vary);
    Read_VARLABEL(&p->loader); // COMPILER_VERSION 20

   #if COMPILER_VERSION >= 25
    Read_DIRECTION(&p->direction);
   #endif
   #if COMPILER_VERSION >= 26
    Read_MYSCHEME(&p->stscheme);
    Read(&p->summary, sizeof(_UWORD));
   #endif

   #if COMPILER_VERSION >= 45
    Read_ZOOM(&p->zoom);
   #endif

  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_MOTION(p);
  }

  return(retcode);
}


_BOOL _File::Read_KARU(_KARU *p)
{
  init_KARU(p);

  if(GetError() == _FALSE)
  {
    Read(&p->objno, 2);
    if(Read_VARLABEL(&p->loader) == _TRUE)
      return _TRUE;
  }
  
  free_KARU(p);
  return _FALSE;
}


_BOOL _File::Read_MEM_KARU(_MEM_KARU *p)
{
  _UWORD no, i;
  _KARU *pk;
  
  init_MEM_KARU(p);

  if(GetError() == _FALSE)
  {
    Read(&no, 2);
    if(FileMemory((void**)&p->ptr, no * sizeof(_KARU)) == _TRUE)
    {
      p->no = no;
      pk = p->ptr;
      for(i=0; i<no; i++, pk++)
        Read_KARU(pk);
    }
    
    if(GetError() == _FALSE)
      return _TRUE;
  }
  
  free_MEM_KARU(p);
  return _FALSE;
}


_BOOL _File::Read_CAROUSEL(_MOTION *p)
{
  _BOOL     retcode = _TRUE;
//  _COLOR    cb;
  _UBYTE    tmp;
  _VARLABEL vl;

  init_MOTION(p);

  if(GetError() == _FALSE)
  {
    Read_ROOM(&p->room);
		Read_COLOR(&p->colback);
    Read_ATTRIB(&p->attrib);
    Read(&tmp, 1);
    p->gap = tmp;
    Read(&tmp, 1);
    p->skip_percent = (tmp == 0)? 50 : tmp;
    //p->skip_percent = 20; 
    
//    p->attrib &= ~T_BOUND;
//    p->attrib |= T_MIDBOUND;
//    p->attrib |= T_CENTERBOUND;
//    p->horizontal = _FALSE;
    
    init_VARLABEL_Makro(&vl);
    Read_VARLABEL(&vl);
    
    Read(&tmp, 1);
    p->infinite  = (tmp & 1)? _TRUE : _FALSE;
    
    if(tmp & 2)
    {
      p->horizontal = _TRUE;
      p->varx = vl;
    }
    else
    {
      p->horizontal = _FALSE;
      p->vary = vl;
    }
    
    Read_CHKBIT(&p->chkbit);
    Read_MYSCHEME(&p->stscheme);
    Read_MEM_KARU(&p->memkaru);
    
    
    _LINKSCHEME ls;
    init_LINKSCHEME_Makro(&ls);
    Read_LINKSCHEME(&ls);
    free_LINKSCHEME(&ls);
    
    Read_LINKSCHEME(&ls);
    free_LINKSCHEME(&ls);
    
   #if COMPILER_VERSION >= 59
    Read_VARLABEL(&p->varindex);
    Read(&tmp, 1);
    p->overmove = (tmp & 1)? _TRUE : _FALSE;
    p->scrbar.valid = (tmp & 2)? _TRUE : _FALSE;
    Read_FRAMETYPE(&p->frametype);
    
    
    _COLOR tcol;
    Read_COLOR(&tcol); 
    p->scrbar.col_back  = GetBackColor(tcol);
    p->scrbar.col_front = GetPenColor(tcol);
    Read_COLOR(&p->scrbar.col_frame);
    Read_COLOR(&p->scrbar.col_text);
    Read(&tmp, 1);
    switch(tmp)
    {
      case  0 : p->movespeed = 10; break; // slow
      case 20 : p->movespeed = 30; break; // fast
      case 30 : p->movespeed = 38; break; // hispeed
      default : p->movespeed = 15; break; // standard
    }
    
    p->wiperoom = p->room;

    if(p->scrbar.valid == _TRUE)
    {
      if(p->horizontal == _TRUE)
      {
        p->scrbar.horizontal = _TRUE;
        p->wiperoom.xy2.y   -= (LEVELBUTTON_HEIGHT+1);
        p->scrbar.room       = p->room;
        p->scrbar.room.xy1.y = p->wiperoom.xy2.y + 1;
      }
      else
      {
        p->scrbar.horizontal = _FALSE;
        p->wiperoom.xy2.x   -= (LEVELBUTTON_WIDTH+1);
        p->scrbar.room       = p->room;
        p->scrbar.room.xy1.x = p->wiperoom.xy2.x + 1;
      }

      p->scrbar.frametype = p->frametype;  // frametype
    }
   #endif
  }
  
  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_MOTION(p);
  }
  
  return(retcode);
}

_BOOL _File::Read_PRJ_ENTRY(_PRJ_ENTRY *p)
{
  _BOOL retcode = _TRUE;

  init_PRJ_ENTRY(p);

  if(GetError() == _FALSE)
  {
    p->ftype = (_ENTRYTYPE)Read_ENUM();
    Read(&p->no, sizeof(_UWORD));
    Read_CHKBIT(&p->chkbit);
    Read(&p->name, 13);
    Read_ANYPARA(&p->anypara);
    Read_ACCESS(&p->access);      //  ab COMPILER_VERSION 20
    Read(&p->nextno, sizeof(_UWORD)); //  ab COMPILER_VERSION 21
   #if COMPILER_VERSION >= 24
    Read(&p->imageno, sizeof(_UWORD));
   #endif
   #if COMPILER_VERSION >= 29
    Read(&p->refcyc, sizeof(_UBYTE));
   #endif

    // korrektur !!
    if((p->ftype != _PE_PICTURE) && (p->ftype != _PE_WINDOW))
    {
      p->access = DEFACCESS;
      p->refcyc = 0;
    }
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_PRJ_ENTRY(p);
  }

  return(retcode);
}

_BOOL _File::Read_PICTURE(_PICTURE *p)
{
  _BOOL retcode = _TRUE;

  init_PICTURE(p);


  if(GetError() == _FALSE)
  {
    Read_CHKBIT(&p->chkbit);
    if(p->TextPuffer.ReadLanguage(this, get_actual_language_Makro()) == _TRUE)
    {
      Read_PICTUREMASK(&p->mask);
    }
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_PICTURE(p);
  }

  return(retcode);
}

_BOOL _File::Read_PICTUREMASK(_PICTUREMASK *p)
{
  _UWORD      i, no;
  _UDWORD     size;
  _BOOL       retcode = _TRUE;
  _RECTANGLE  *pr;
  _POLYGON    *pp;
  _CIRCLE     *pc;
  _PICTEXT    *pt;
  _OBJECT     *po;
  _BITMAP     *pb;
  _INPUT      *pi;
  _IO         *pio;
  _MOTION     *pm;
  _COL_RECT   *pcr;
  _COL_POLY   *pcp;
  _COL_CIRC   *pcc;
  _COL_PICTXT *pct;
  _KEY        *pmk;
  _BUTTON     *ptb;

  init_PICTUREMASK(p);
//  PrepaireRoom();

  if(GetError() == _FALSE)
  {
//    if(p->PictureText.ReadLanguage(this, System->act_language) == TRUE)
    {
      Read_ROOM(&p->room);
//      PrepaireRoom();

//      if(chk_ROOM(&p->room) == _FALSE) // wegen sa36584 (nur einen waagrechte linie im objekt) auskommentiert
//      {
//        MyGetScreenSize(&p->room);
//      }

      // read rectangles
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_RECTANGLE);
        if(FileMemory((void**)&p->memrectangle.ptr, size) == _TRUE)
        {
          p->memrectangle.no = no;
          for(i=0, pr=p->memrectangle.ptr; i<no; i++, pr++)
            Read_RECTANGLE(pr);
        }
      }

      // read polygons
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_POLYGON);
        if(FileMemory((void**)&p->mempolygon.ptr, size) == _TRUE)
        {
          p->mempolygon.no = no;
          for(i=0, pp=p->mempolygon.ptr; i<no; i++, pp++)
            Read_POLYGON(pp);
        }
      }

      // read circles
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_CIRCLE);
        if(FileMemory((void**)&p->memcircle.ptr, size) == _TRUE)
        {
          p->memcircle.no = no;
          for(i=0, pc=p->memcircle.ptr; i<no; i++, pc++)
            Read_CIRCLE(pc);
        }
      }

      // read picturetextpositions
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_PICTEXT);
        if(FileMemory((void**)&p->mempictxt.ptr, size) == _TRUE)
        {
          p->mempictxt.no = no;
          for(i=0, pt=p->mempictxt.ptr; i<no; i++, pt++)
            Read_PICTEXT(pt);
        }
      }

      // read objects
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_OBJECT);
        if(FileMemory((void**)&p->memobject.ptr, size) == _TRUE)
        {
          p->memobject.no = no;
          for(i=0, po=p->memobject.ptr; i<no; i++, po++)
            Read_OBJECT(po);
        }
      }

      // read bitmaps
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_BITMAP);
        if(FileMemory((void**)&p->membitmap.ptr, size) == _TRUE)
        {
          p->membitmap.no = no;
          for(i=0, pb=p->membitmap.ptr; i<no; i++, pb++)
            Read_BITMAP(pb);
        }
      }

      // read inputs
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_INPUT);
        if(FileMemory((void**)&p->meminput.ptr, size) == _TRUE)
        {
          p->meminput.no = no;
          for(i=0, pi=p->meminput.ptr; i<no; i++, pi++)
            Read_INPUT(pi);
        }
      }

      // read outputs
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_IO);
        if(FileMemory((void**)&p->memoutput.ptr, size) == _TRUE)
        {
          p->memoutput.no = no;
          for(i=0, pio=p->memoutput.ptr; i<no; i++, pio++)
            Read_IO(pio);
        }
      }

      // read motions
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_MOTION);
        if(FileMemory((void**)&p->memmotion.ptr, size) == _TRUE)
        {
          p->memmotion.no = no;
          for(i=0, pm=p->memmotion.ptr; i<no; i++, pm++)
            Read_MOTION(pm);
        }
      }

      // read coloured rectangles
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_COL_RECT);
        if(FileMemory((void**)&p->colrect.ptr, size) == _TRUE)
        {
          p->colrect.no = no;
          for(i=0, pcr=p->colrect.ptr; i<no; i++, pcr++)
            Read_COL_RECT(pcr);
        }
      }

      // read coloured polygons
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_COL_POLY);
        if(FileMemory((void**)&p->colpoly.ptr, size) == _TRUE)
        {
          p->colpoly.no = no;
          for(i=0, pcp=p->colpoly.ptr; i<no; i++, pcp++)
            Read_COL_POLY(pcp);
        }
      }

      // read coloured circles
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_COL_CIRC);
        if(FileMemory((void**)&p->colcirc.ptr, size) == _TRUE)
        {
          p->colcirc.no = no;
          for(i=0, pcc=p->colcirc.ptr; i<no; i++, pcc++)
            Read_COL_CIRC(pcc);
        }
      }

      // read coloured picturetexts
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_COL_PICTXT);
        if(FileMemory((void**)&p->colpictxt.ptr, size) == _TRUE)
        {
          p->colpictxt.no = no;
          for(i=0, pct=p->colpictxt.ptr; i<no; i++, pct++)
            Read_COL_PICTXT(pct);
        }
      }
      
     #if COMPILER_VERSION >= 55
      _UWORD nomot = p->memmotion.no;

      // read carousel
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = (nomot + no) * sizeof(_MOTION);
        if(FileMemory((void**)&p->memmotion.ptr, size) == _TRUE)
        {
          for(i=0; i<no; i++)
            Read_CAROUSEL(&p->memmotion.ptr[nomot + i]);
          p->memmotion.no += no;
        }
      }
     #endif
     
      // read keyboard
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_KEY);
        if(FileMemory((void**)&p->memkey.ptr, size) == _TRUE)
        {
          p->memkey.no = no;
          for(i=0, pmk=p->memkey.ptr; i<no; i++, pmk++)
            Read_KEY(pmk);
        }
      }

      // read touchbuttons
      if(Read(&no, sizeof(_UWORD)) == _TRUE)
      {
        size = no * sizeof(_BUTTON);
        if(FileMemory((void**)&p->membutton.ptr, size) == _TRUE)
        {
          p->membutton.no = no;
          for(i=0, ptb=p->membutton.ptr; i<no; i++, ptb++)
            Read_BUTTON(ptb);
        }
      }

      // read virtual objectnames
      Read_FAST_ASCII_BUFFER(&p->enclosed_name);

      // read z-order
      Read_MEM_ZORDER(&p->memzorder);
      
     #if COMPILER_VERSION >= 55
      // hier werden die z-order einträge von CAROUSEL berichtigt, weil carousels hinten an den motions hängen
      if(p->memmotion.no != nomot)
      {
        for(i=0; i<p->memzorder.no; i++)
        {
          if(p->memzorder.ptr[i].ftype == ZO_CAROUSEL)
          {
            p->memzorder.ptr[i].ftype = ZO_MOTION;
            p->memzorder.ptr[i].no += nomot;
          }
        }
      }
     #endif
      
	  }
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_PICTUREMASK(p);
  }
//  else
//  {
//    _ROOM xr;
//    GetRoom(&xr);
//  }

  return(retcode);
}

_BOOL _File::Read_BMP(_BMP *p)
{
  _BOOL   retcode = _FALSE;
  _UDWORD bytes;
  _UDWORD no_dat;
  _UWORD  data[4];

  init_BMP_Makro(p);

  if(GetError() == _FALSE)
  {
    if(Read(&no_dat, sizeof(_UDWORD)) == _TRUE)
    {
//      Read(&p->width,       sizeof(_UWORD));
//      Read(&p->height,      sizeof(_UWORD));
//      Read(&p->transparent, sizeof(_UWORD));
//      Read(&p->state,       sizeof(_UWORD));
      Read(data, 8);
      p->width       = data[0];
      p->height      = data[1];
      p->transparent = data[2];
      p->state       = data[3];
      
      
      //p->state |= 0x0004;

      if(p->state & 0x0008) //BMP_256COLOR)
        bytes = no_dat * 2; //sizeof(_BMPDAT256);
      else
        bytes = no_dat * sizeof(_BMPDAT);

      if(FileMemory((void**)&p->ptr, bytes) == _TRUE)
      {
        p->datano = no_dat;
        Read(p->ptr, bytes);
        
       #ifdef UC_GRAYSCALE
        transform_BMP_to_gray(p);
       #endif
      }
    }
  }

  if(GetError() == _TRUE)
  {
    free_BMP(p);
  }
  else
  {
    retcode = _TRUE;
    
//    if((p->state & BMP_NO_TRANSPARENT) == 0)
//    {
//      _UDWORD i;
//      _UBYTE *ph = (_UBYTE*)p->ptr;
//      ph += 1;
//      for(i=0; i<p->datano; i++)
//      {
//         if(*(_UWORD*)ph == p->transparent)
//           return(retcode);
//         ph += 3;
//      }
//      p->state |= BMP_NO_TRANSPARENT;
//    }
  }

  return(retcode);
}

#ifdef UC_IPC
_BOOL _File::Write_BMP(_BMP *p)
{
  _BOOL   retcode = _FALSE;
  _UDWORD bytes;

  if(GetError() == _FALSE)
  {
    if(Write(&p->datano, sizeof(_UDWORD)) == _TRUE)
    {
      Write(&p->width,       sizeof(_UWORD));
      Write(&p->height,      sizeof(_UWORD));
      Write(&p->transparent, sizeof(_UWORD));
      Write(&p->state,       sizeof(_UWORD));

      if(p->state & 0x0008) //BMP_256COLOR)
        bytes = p->datano * 2; //sizeof(_BMPDAT256);
      else
        bytes = p->datano * sizeof(_BMPDAT);

      Write(p->ptr, bytes);
    }
  }

  if(GetError() == _FALSE)
    retcode = _TRUE;

  return(retcode);
}

extern "C" _BOOL Write_BMP(const _ASCII *dpne, _BMP *src)
{
  _File File;
  _BOOL retcode = _FALSE;

  if(File.Open(SAVE, dpne, 0) == _TRUE)
  {
    if(File.Write_BMP(src) == _TRUE)
      retcode = _TRUE;
    File.Close();
  }

  return(retcode);
}

extern "C" _BOOL Read_BMP(_ASCII *dpne, _BMP *dst)
{
  _File File;
  _BOOL retcode = _FALSE;

  free_BMP(dst);

  if(File.Open(LOAD, dpne) == _TRUE)
  {
    if(File.Read_BMP(dst) == _TRUE)
      retcode = _TRUE;
    File.Close();
  }

  return(retcode);
}
#endif

_BOOL _File::Read_COL_RECT(_COL_RECT *p)
{
  _BOOL retcode = _TRUE;

  init_COL_RECT(p);

  if(GetError() == _FALSE)
  {
    Read_RECTANGLE(&p->rectangle);
    Read_LINKSCHEME(&p->dyncolback);         // color background (hatch)
    Read_LINKSCHEME(&p->dyncolfront);        // color background (hatch)
    Read_LINKSCHEME(&p->dyncoltl);           // color frame (top-left)
    Read_LINKSCHEME(&p->dyncolbr);           // color frame (bottom-right)
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_COL_RECT(p);
  }

  return(retcode);
}

_BOOL _File::Read_COL_POLY(_COL_POLY *p)
{
  _BOOL retcode = _TRUE;

  init_COL_POLY(p);

  if(GetError() == _FALSE)
  {
    Read_POLYGON(&p->poly);
    Read_LINKSCHEME(&p->dyncolback);         // color background (hatch)
    Read_LINKSCHEME(&p->dyncolfront);        // color background (hatch)
    Read_LINKSCHEME(&p->dyncolframe);        // color frame
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_COL_POLY(p);
  }

  return(retcode);
}

_BOOL _File::Read_COL_CIRC(_COL_CIRC *p)
{
  _BOOL   retcode = _TRUE;

  init_COL_CIRC(p);

  if(GetError() == _FALSE)
  {
    Read_CIRCLE(&p->circle);
    Read_LINKSCHEME(&p->dyncolback);         // color background (hatch)
    Read_LINKSCHEME(&p->dyncolfront);        // color background (hatch)
    Read_LINKSCHEME(&p->dyncolframe);        // color frame
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_COL_CIRC(p);
  }

  return(retcode);
}

_BOOL _File::Read_COL_PICTXT(_COL_PICTXT *p)
{
  _BOOL retcode = _TRUE;

  init_COL_PICTXT(p);

  if(GetError() == _FALSE)
  {
    Read_PICTEXT(&p->btx);
    Read_LINKSCHEME(&p->dyncolback);         // color background (hatch)
    Read_LINKSCHEME(&p->dyncolfront);        // color background (hatch)
    Read_LINKSCHEME(&p->dyncoltxt);          // color text
    Read_LINKSCHEME(&p->dyncoltl);           // color frame (top-left)
    Read_LINKSCHEME(&p->dyncolbr);           // color frame (bottom-right)
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_COL_PICTXT(p);
  }

  return(retcode);
}

_BOOL _File::Read_KEY(_KEY *p)
{
  _BOOL retcode = _FALSE;

  init_KEY(p);

  if(GetError() == _FALSE)
  {
    Read(&p->code, sizeof(_UWORD));
    p->tip = (_TIP)Read_ENUM();
    Read_GFUNCT(&p->gfunct);

   #if COMPILER_VERSION >= 24
    _CHKBIT tmp;
    init_CHKBIT_Makro(&tmp);
    Read_CHKBIT(&tmp);
    free_CHKBIT(&tmp);
   #endif

    if(GetError() == _TRUE)
      free_KEY(p);
    else
      retcode = _TRUE;
  }

  return(retcode);
}

_BOOL _File::Read_MEM_KEY(_MEM_KEY *p)
{
  _BOOL  retcode = _FALSE;
  _UWORD i;
  _KEY   *pk;


  init_MEM_KEY(p);

  Read(&p->no, sizeof(_UWORD));

  if(Memory2((void**)&p->ptr, p->no * sizeof(_KEY)) != 0)
  {
    for(i=0, pk=p->ptr; i<p->no; i++, pk++)
      Read_KEY(pk);

    if(GetError() == _FALSE)
      retcode = _TRUE;
  }

  return(retcode);

}

/*
_BOOL _File::Read_CRLF_ramfile(_ASCII *dst, _UWORD mlen)
{
  _UBYTE  *pa;
  _UDWORD len, maxlen;
  _UWORD  crlf;

  if((State == LOAD)||(State == LOADSAVE))
  {
    maxlen = mlen;

    if((Offset + maxlen) > RamSize)
      maxlen = RamSize - Offset;

    if(maxlen >= 2)
    {
      maxlen -= 1;
      len     = 2;
      pa      = &RamData[Offset];

      while(maxlen)
      {
        crlf = *(_UWORD*)pa;
//        if(crlf == 0x0A0D)
        if((crlf == 0x0A0D)||(crlf == 0x0D0A))
        {
          if(Read(dst, len) == _TRUE)
          {
            dst[len-2] = 0; // ascii-0-string
            return(_TRUE);
          }
          return(_FALSE);
        }

        len    += 1;
        maxlen -= 1;
        pa     += 1;
      }
    }

    if(Offset < RamSize)
    {
      len = RamSize - Offset;
      if(len <= mlen)
      {
        if(Read(dst, len) == _TRUE)
        {
          dst[len] = 0; // ascii-0-string
          return(_TRUE);
        }
      }
    }
  }
  else
  {
    SetError(H_NOREAD);
  }

  return(_FALSE);
}
*/

void dst_copy(_ASCII *pd, _UWORD mlen, void *ps, _UWORD dlen)
{
  if(dlen >= mlen)
    dlen = (mlen - 1);
  
  MemCpy(pd, ps, dlen);
  pd[dlen] = 0;
}

_BOOL _File::Read_CRLF_ramfile(_ASCII *dst, _UWORD mlen)
{
  _UBYTE  *pa, *ps;
  _UDWORD len, siz;

  if((State == LOAD)||(State == LOADSAVE))
  {
    len = RamSize - Offset;
    pa  = &RamData[Offset];
    ps  = pa;
    siz = 0;
    
    while(len)
    {
      siz ++;
      len --;
      
      if(pa[0] == 0x0A)
      {
        if(siz > 1)
        {
          if(pa[-1] == 0x0D)
          {
            dst_copy(dst, mlen, ps, siz-2);
            Offset += siz;
            return _TRUE;
          }
        }
        
        if(pa[1] == 0x0D)
        {
          dst_copy(dst, mlen, ps, siz-1);
          Offset += (siz + 1);
          return _TRUE;
        }
          
        dst_copy(dst, mlen, ps, siz-1);
        Offset += siz;
        return _TRUE;
      }
      
      pa ++;
    }
    
    if(siz > 0)
    {
      dst_copy(dst, mlen, ps, siz);
      Offset += siz;
      return _TRUE;
    }
  }
  else
  {
    SetError(H_NOREAD);
  }

  return _FALSE;
}


_BOOL _File::Read_CRLF(_ASCII *dst, _UWORD maxlen)
{
  _UBYTE byte;
  _UWORD size = 0;
  _BOOL  run;
  _DWORD rest;
  _BOOL  retcode = _FALSE;

  if(maxlen > 0)
    dst[size] = 0;

  if(GetError() == _FALSE)
  {
    if(IsRamFile() == _TRUE)
      return(Read_CRLF_ramfile(dst, maxlen));

    rest = Length() - Tell();

    do
    {
      run = _FALSE;
      if(rest > 0)
      {
        run = Read(&byte, 1);
        rest -= 1;
      }

      if(run == _TRUE)
      {
        retcode = _TRUE;
        if((byte == 13)||(byte == 10))
        {
          run = Read(&byte, 1);
          if(run == _TRUE)
          {
            if((byte != 13)&&(byte != 10))
              Seek(-1, SEEK_CUR);
          }
          run  = _FALSE;
          byte = 0;
        }

        if(size < maxlen)
        {
          dst[size] = byte;
          size ++;
        }
      }
    }
    while(run == _TRUE);
  }

  if(size < maxlen)
    dst[size] = 0;

  return(retcode);
}

_BOOL _File::ReadCrLf(void *dst, _UWORD maxlen, _UWORD charsize)
{
  _BOOL retcode = _FALSE;

  if(charsize == 1)
    retcode = Read_CRLF((_ASCII*)dst, maxlen);
 #ifdef UC_UNICODE
  else if(charsize == 2)
    retcode = Read_CRLF_unicode((_UNI*)dst, maxlen);
 #endif

  return(retcode);
}


#ifdef UC_UNICODE
_BOOL _File::Read_CRLF_ramfile_unicode(_UNI *dst, _UWORD mlen)
{
  _UDWORD crlf;
  _BOOL   read = _FALSE;

  if((State == LOAD)||(State == LOADSAVE))
  {
    if((RamSize - Offset) >= 2)
    {
      while(mlen)
      {
        if((Offset + 4) <= RamSize)
        {
          crlf = *(_UDWORD*)&RamData[Offset];

          if((crlf == 0x000A000D)||(crlf == 0x000D000A))
          {
            if(Read(&crlf, 4) == _FALSE)
              return(_FALSE);
            mlen = 0;
          }
          else
          {
            read = _TRUE;
          }
        }
        else if((Offset + 2) <= RamSize)
        {
          read = _TRUE;
        }
        else
        {
          mlen = 0;
        }

        if(read == _TRUE)
        {
          read = _FALSE;
          if(Read(dst, 2) == _FALSE)
            return(_FALSE);

          if(*dst != 0xFEFF)
          {
            dst += 1;
            mlen -= 1;
          }
        }
      }

      *dst = 0;

      return(_TRUE);
    }
  }
  else
  {
    SetError(H_NOREAD);
  }

  return(_FALSE);
}

_BOOL _File::Read_CRLF_unicode(_UNI *dst, _UWORD maxlen)
{
  _UWORD word;
  _UWORD size = 0;
  _BOOL  run;
  _DWORD rest;
  _BOOL  retcode = _FALSE;

  if(maxlen > 0)
    dst[size] = 0;

  if(GetError() == _FALSE)
  {
    if(IsRamFile() == _TRUE)
      return(Read_CRLF_ramfile_unicode(dst, maxlen));

    rest = Length() - Tell();

    do
    {
      run = _FALSE;
      if(rest > 1)
      {
        run = Read(&word, 2);
        rest -= 2;
      }

      if(run == _TRUE)
      {
        retcode = _TRUE;
        if((word == 13)||(word == 10))
        {
          run = Read(&word, 2);
          if(run == _TRUE)
          {
            if((word != 13)&&(word != 10))
              Seek(-2, SEEK_CUR);
          }
          run  = _FALSE;
          word = 0;
        }

        if(((size + 1) <= maxlen) && (word != 0xFEFF))
        {
          dst[size] = word;
          size ++;
        }
      }
    }
    while(run == _TRUE);
  }

  if(size < maxlen)
    dst[size] = 0;

  return(retcode);
}
#endif

_BOOL _File::Read_LINEINFO(_LINEINFO *p)
{
  _BOOL retcode = _FALSE;

  init_LINEINFO(p);

  if(GetError() == _FALSE)
  {
    Read_CHKBIT(&p->chkbit);
    Read_VARLABEL(&p->value);
    Read_GFUNCT(&p->gfunct);
   #if COMPILER_VERSION >= 22
    Read_ANYPARA(&p->anypara);                     // ab COMPILER_VERSION 22
   #else
    Read(&p->anypara.anything.no, sizeof(_UWORD)); // bis COMPILER_VERSION 22
    p->anything.list = LOCTXT;                     // bis COMPILER_VERSION 22
   #endif
    Read_MEM_VARLABEL(&p->memvar);
   #if COMPILER_VERSION >= 54    // ab COMPILER_VERSION 54
    Read_MYSCHEME(&p->stscheme);
   #endif
  }

  if(GetError() == _FALSE)
    retcode = _TRUE;
  else
    free_LINEINFO(p);

  return(retcode);
}

_BOOL _File::Read_EDITCOLOR(_EDITCOLOR *p)
{
  _BOOL  retcode = _FALSE;
  _COLOR tmpl, tmph;

  init_EDITCOLOR(p);

  if(GetError() == _FALSE)
  {
    Read_COLOR(&tmpl);
    Read_LINKSCHEME(&p->dyncolfront);
    Read_COLOR(&tmph);
    Read_LINKSCHEME(&p->dyncolback);
    p->colback = MakeColor_Makro(GetPenColor_Makro(tmph), GetPenColor_Makro(tmpl));

    Read_COLOR(&tmpl);
    Read_LINKSCHEME(&p->dyncoltl);
    Read_COLOR(&tmph);
    Read_LINKSCHEME(&p->dyncolbr);
    p->colframe = MakeColor_Makro(GetPenColor_Makro(tmpl), GetPenColor_Makro(tmph));

    Read_COLOR(&tmpl);
    Read_LINKSCHEME(&p->dyncoltxt);
    p->coltxt = MakeColor_Makro(GetPenColor_Makro(tmpl), GetPenColor_Makro(tmpl));
  }

  if(GetError() == _FALSE)
    retcode = _TRUE;
  else
    free_EDITCOLOR(p);

  return(retcode);
}

_BOOL _File::Read_BUBBLEINFO(_BUBBLEINFO *p)
{
  init_BUBBLEINFO(p);

// #ifdef UC_BUBBLE_HELP
  _BOOL   retcode = _FALSE;
  _ATTRIB tmp;
  if(GetError() == _FALSE)
  {
    Read(&p->delaytime, sizeof(_DWORD));
    Read(&p->remaintime, sizeof(_DWORD));
    Read(&p->font, sizeof(_FONT));
    Read_FRAMETYPE(&p->frametype);
    Read_ATTRIB(&tmp);
    p->attrib = (p->attrib & (T_BOUND|T_PROP))|(tmp & ~(T_BOUND|T_PROP));
    Read_EDITCOLOR(&p->editcolor);
  }

  if(GetError() == _FALSE)
    retcode = _TRUE;
  else
    free_BUBBLEINFO(p);

  return(retcode);
// #else
//  return(_TRUE);
// #endif
}

_BOOL _File::Read_CURSORCOLOR(_CURSORCOLOR *p)
{
  _BOOL  retcode = _FALSE;

  init_CURSORCOLOR(p);

  if(GetError() == _FALSE)
  {
    Read_COLOR(&p->color);
    Read_LINKSCHEME(&p->dyncol);
  }

  if(GetError() == _FALSE)
    retcode = _TRUE;
  else
    free_CURSORCOLOR(p);

  return(retcode);
}

_BOOL _File::Read_SCREENSAVER(_SCREENSAVER *p)
{
  _BOOL   retcode = _FALSE;
  _UBYTE  typ = 0;

  init_SCREENSAVER(p);

  if(GetError() == _FALSE)
  {
//    Read(&delaytime,     sizeof(_UDWORD);
    Read(&p->actiontime, sizeof(_UDWORD));
    Read(&typ,           sizeof(_UBYTE)); p->clrscr = (typ == 0)? _TRUE : _FALSE;
    Read(&p->image,      sizeof(_UWORD));

    retcode = _TRUE;
    if(GetError() == _TRUE)
    {
      free_SCREENSAVER(p);
      retcode = _FALSE;
    }
  }

  return(retcode);
}

_BOOL _File::Read_MEM_VARLABEL(_MEM_VARLABEL *p)
{
  _BOOL     retcode = _FALSE;
  _UWORD    no, i;
  _UDWORD   size;
  _VARLABEL *pv;

  init_MEM_VARLABEL(p);

  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    size = no * sizeof(_VARLABEL);
    if(FileMemory((void**)&p->ptr, size) == _TRUE)
    {
      p->no = no;
      for(i=0,pv=p->ptr; i<no; i++,pv++)
        Read_VARLABEL(pv);

      if(GetError() == _FALSE)
        return(_TRUE); //retcode = _TRUE;
    }
  }

  if(retcode == _FALSE)
    free_MEM_VARLABEL(p);

  return(retcode);
}

_BOOL _File::Read_MEM_LINEINFO(_MEM_LINEINFO *p)
{
  _BOOL     retcode = _FALSE;
  _UWORD    no, i;
  _UDWORD   size;
  _LINEINFO *pl;

  init_MEM_LINEINFO(p);

  if(GetError() == _FALSE)
  {
    if(Read(&no, sizeof(_UWORD)) == _TRUE)
    {
      size = no * sizeof(_LINEINFO);
      if(FileMemory((void**)&p->ptr, size) != 0)
      {
        p->no = no;
        for(i=0, pl=p->ptr; i<no; i++, pl++)
          Read_LINEINFO(pl);

        if(GetError() == _FALSE)
          retcode = _TRUE;
      }
    }
  }

  if(retcode == _FALSE)
    free_MEM_LINEINFO(p);

  return(retcode);
}

_BOOL _File::Read_COMPCODE(_COMPCODE *p)
{
  // [][].... size of compilersequence
  // [....... compilersequence

  _BOOL  retcode = _TRUE;
  _UWORD size;

  init_COMPCODE(p);
  if(GetError() == _FALSE)
  {
    if(Read(&size, sizeof(_UWORD)) == _TRUE)
    {
      if(FileMemory((void**)&p->ptr, size) == _TRUE)
      {
        p->size = size;
        Read(p->ptr, size);
      }
    }
  }

  if(GetError() == _TRUE)
  {
    free_COMPCODE(p);
    retcode = _FALSE;
  }

  return(retcode);
}

_BOOL _File::Read_MENUINFO(_MENUINFO *p)
{
  _BOOL retcode = _FALSE;

  init_MENUINFO(p);

  if(GetError() == _FALSE)
  {
    Read(&p->font,        sizeof(_FONT));
    Read_ATTRIB(&p->attrib);
    Read(&p->xy,          sizeof(_DOT));
    Read(&p->line_height, sizeof(_UWORD));
    Read(&p->width,       sizeof(_UWORD));
    Read(&p->height,      sizeof(_UWORD));
    Read_COLOR(&p->text_color);  // line color
    Read_COLOR(&p->bar_color);   // bar color
    Read_COLOR(&p->frame_color); // frame color

   #if COMPILER_VERSION >= 54
    Read_COLOR(&p->inactive_text_color);  // inactive line color
   #endif

   #ifdef ZOOM_KM
    p->line_height += OFFSET_MENU_LINE_HEIGHT;
   #endif

   #if COMPILER_VERSION < 22
    _TextPuffer Title;
    Title.Init();
    Title.ReadLanguage(this, get_actual_language_Makro());
    Title.Free();
   #endif

   #if COMPILER_VERSION >= 41
    Read_VARLABEL(&p->server);
   #endif

   #if COMPILER_VERSION >= 48
    Read_FRAMETYPE(&p->frametype);
   #endif

    if(MpcVer >= 60) // #if COMPILER_VERSION >= 60 // sa34116
    {
      _UBYTE tmp08;
      Read(&tmp08, sizeof(_UBYTE));
      p->draw_shadow = (tmp08 & 1)? _TRUE : _FALSE;
    } 

    if(GetError() == _FALSE)
      retcode = _TRUE;
    else
      free_MENUINFO(p);
  }

  return(retcode);
}

_BOOL _File::Read_LSEMENU(_LSEMENU *p)
{
  _BOOL retcode = _TRUE;

  init_LSEMENU(p);

  if(GetError() == _FALSE)
  {
   #if COMPILER_VERSION < 22
    p->TextPuffer.ReadLanguage(this, get_actual_language());
   #endif
    Read_MEM_LINEINFO(&p->mem_lineinfo);
    Read_MENUINFO(&p->info);
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_LSEMENU(p);
  }

  return(retcode);
}

_BOOL _File::Read_BUTTONIMAGES(_BUTTONIMAGES *p)
{
  _BOOL retcode = _TRUE;
  init_BUTTONIMAGES(p);

  if(GetError() == _FALSE)
  {
    Read(&p->image_release,     sizeof(_UWORD));  // released image
    Read(&p->image_press,       sizeof(_UWORD));  // pressed image
    Read(&p->image_sel_press,   sizeof(_UWORD));  // selected+pressed image
    Read(&p->image_sel_release, sizeof(_UWORD));  // selected+released image
    Read(&p->image_in_press,    sizeof(_UWORD));  // inactive pressed image
    Read(&p->image_in_release,  sizeof(_UWORD));  // inactive released image
    Read(&p->image_describe,    sizeof(_UWORD));  // describing image
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_BUTTONIMAGES(p);
  }

  return(retcode);
}

_BOOL _File::Read_BUTTON(_BUTTON *p)
{
  init_BUTTON(p);

  if(GetError() == _FALSE)
  {
    Read_CHKBIT(&p->chkbit);
    Read_ROOM(&p->room);
    Read_GFUNCT(&p->gfunct);
    Read_ANYTHING(&p->anything);
    Read_ATTRIB(&p->attrib);
    Read_COLOR(&p->framecol);
    Read_COLOR(&p->col);
    Read(&p->group, sizeof(_UWORD));
    p->ftype = (_BUTTON_TYPE)Read_ENUM();
    p->tip = (_TIP)Read_ENUM();
    Read(&p->summary, sizeof(_UWORD));
    p->enter_plus = (_ENTER_PLUS)Read_ENUM();
    Read_DIRECTION(&p->direction);
    // ab COMPILER_VERSION 20
    Read_FRAMETYPE(&p->frametype);
    // ab COMPILER_VERSION 20
    Read_VARLABEL(&p->loader);

   #if COMPILER_VERSION >= 25
    Read_ACCESS(&p->access);            // access level
    Read_LINKSCHEME(&p->dyncolback);    // color background (hatch)
    Read_LINKSCHEME(&p->dyncolfront);   // color background (hatch)
    Read_LINKSCHEME(&p->dyncoltl);      // color frame (top-left)
    Read_LINKSCHEME(&p->dyncolbr);      // color frame (bottom-right)
   #endif
   #if COMPILER_VERSION >= 26
    Read_MYSCHEME(&p->stscheme);
   #endif
   #if COMPILER_VERSION >= 27
    Read_BUTTONIMAGES(&p->buttonimages);
    Read_ANYPARA(&p->text);                       // released text
    Read_VARLABEL(&p->varlabel);                  // server
    Read(&p->font, sizeof(_FONT));
    Read_COLOR(&p->coltxt);
    Read_LINKSCHEME(&p->dyncoltxt);               // color txt
    Read(&p->hotkeycode,        sizeof(_UWORD));  // scancode hotkey
   #endif
   #if COMPILER_VERSION >= 31
    Read(&p->doimage, 1);
   #endif

   #if COMPILER_VERSION >= 33
    Read(&p->buttonimages.zoom_images, 1);
    Read(&p->lock_overload, 1);
   #endif

   #if COMPILER_VERSION >= 37
    Read(&p->doubleclick, 1);
   #endif

   #if COMPILER_VERSION >= 38
    Read(&p->searchchoice, sizeof(p->searchchoice));
   #endif

   #if COMPILER_VERSION >= 45
    Read(&p->wose, sizeof(p->wose));
   #endif

   #if COMPILER_VERSION >= 48
    Read(&p->selector, sizeof(_UBYTE));
   #endif

   #if COMPILER_VERSION >= 50
    Read_ANYPARA(&p->text1);                     // pressed text
   #else
    _BOOL tb = _TRUE;
    copy_ANYPARA(&tb, &p->text1, &p->text);      // pressed text = released text
   #endif
   
   #if COMPILER_VERSION >= 55
    ReadMomentaryStationary(&p->act_momentary, &p->set_val, &p->reset_val);
   #endif
   
   #if COMPILER_VERSION >= 57
    Read_ANYPARA(&p->newbubbletext);
   #endif
   
  }

  if(GetError() == _TRUE)
  {
    free_BUTTON(p);
    return _FALSE;
  }
 
  return _TRUE;
}

_BOOL _File::Read_ASCII_BUFFER(_ASCII_BUFFER *p)
{
  // [] .......... sizeof(_CHAR)
  // [][][][] .... no of text (ab compiler version 32 4byte, vorher 2byte)
  // [][][][] .... no of characters
  // [ ........... text

  _BOOL  retcode = _FALSE;
  _UBYTE data[9];

  init_ASCII_BUFFER(p);

  if(GetError() == _FALSE)
  {
   #if COMPILER_VERSION >= 32
    if(Read(data, 9) == _TRUE)
    {
      if(data[0] == 1)
      {
        p->no   = *(_UDWORD*)&data[1];
        p->size = *(_UDWORD*)&data[5];
        if(FileMemory((void**)&p->ptr, p->size) == _TRUE)                 // alloc memory
        {
          p->space = p->size;
          retcode  = Read(p->ptr, p->size);                                  // load text
        }
      }
    }
   #else
    if(Read(data, 7) == _TRUE)
    {
      if(data[0] == 1)
      {
        p->no   = *(_UWORD*)&data[1];
        p->size = *(_UDWORD*)&data[3];
        if(FileMemory((void**)&p->ptr, p->size) == _TRUE)                 // alloc memory
        {
          p->space = p->size;
          retcode  = Read(p->ptr, p->size);                                   // load text
        }
      }
    }
   #endif

    if(retcode == _FALSE)
      free_ASCII_BUFFER(p);
  }


//    if(Read(&sc, sizeof(_UBYTE)) == _TRUE) // load sizeof one character
//    {
//      if(sc == 1)                                  // charactersize must be 1
//      {
//       #if COMPILER_VERSION >= 32
//        Read(&p->no, sizeof(_UDWORD));                     // load no of text
//       #else
//        p->no = 0;
//        Read(&p->no, sizeof(_UWORD));                      // load no of text
//       #endif
//        Read(&p->size, sizeof(_UDWORD));             // load no of characters
//        FileMemory((void**)&p->ptr, p->size);                 // alloc memory
//        Read(p->ptr, p->size);                                   // load text
//        if(GetError() == _FALSE)
//          retcode = _TRUE;
//      }
//    }

  return(retcode);
}

_BOOL make_offset_FAST_ASCII_BUFFER(_FAST_ASCII_BUFFER *p)
{
  _ASCII  *ptr;
  _UDWORD i, no;
  _UDWORD size, offset, len;
  _BOOL   retcode = _FALSE;
  _UDWORD *po;

  no   = p->ascii_buffer.no;
  size = no * sizeof(_UDWORD);

  if(Memory2((void**)&p->poffset, size) != 0)
  {
    retcode = _TRUE;
    offset  = 0;
    ptr     = p->ascii_buffer.ptr;
    for(i=0, po=p->poffset; i<no; i++, po++)
    {
//      p->poffset[i] = offset;
      *po           = offset;
      len           = StrLen(ptr, sizeof(_ASCII)) + 1;
      ptr          += len;
      offset       += len;
    }
  }

  return(retcode);
}

_BOOL _File::Read_FAST_ASCII_BUFFER(_FAST_ASCII_BUFFER *p)
{
  _BOOL retcode = _FALSE;

  init_FAST_ASCII_BUFFER(p);
	if(GetError() == _FALSE)
	{
    if(Read_ASCII_BUFFER(&p->ascii_buffer) == _TRUE)
    {
      retcode = _TRUE;
      make_offset_FAST_ASCII_BUFFER(p);  // retcode nit auswerten, funktioniert auch ohne offset !!
    }
  }

  return(retcode);
}

_BOOL _File::Read_DATE(_DATE *p)
{
	_BOOL retcode = _TRUE;

	if(Read(p, sizeof(_DATE)) == _FALSE)
  {
    retcode = _TRUE;
    init_DATE(p);
  }

  return(retcode);
}

_BOOL _File::Read_ALARM(_ALARM *p)
{
  _UDWORD   size;
  _UWORD    act, lno, i, j, no, ino, nox;
  _VARLABEL tmp;

  _BOOL retcode = _TRUE;

  init_ALARM(p);

  if(GetError() == _FALSE)
  {
    if(Read(&no, sizeof(_UWORD)) == _TRUE)
    {
      ino  = 0;
      size = no * sizeof(_SINGLE_ALARM);
      if(Memory2((void**)&p->ptr, size) != 0)
      {
        for(i=0; i<no; i++)
          init_SINGLE_ALARM(&p->ptr[i]);
      
        nox = no;
        for(i=0; i<no; i++)
        {
          if(Read(&lno, sizeof(_UWORD)) == _TRUE)
          {
            _BOOL tunix = _FALSE;
            if(lno == 0)
            {
              lno = 1;
              tunix = _TRUE;
            }

            ino += lno;
            if(ino > nox)
            {
              size = ino * sizeof(_SINGLE_ALARM);
              if(Memory2((void**)&p->ptr, size) == 0)
                goto _AMEMERROR;
              
              for(j=nox; j<ino; j++)
                init_SINGLE_ALARM(&p->ptr[j]);
              nox = ino;
            }
                           
            if(lno > 0)
            {
              act    = p->no;
              p->no += lno;
              for(j=0; j<lno; j++)
              {
                //init_SINGLE_ALARM(&p->ptr[act+j]);
                if(tunix == _FALSE)
                  Read_VARLABEL(&p->ptr[act+j].link);
              }
              Read(&p->ptr[act].priority, sizeof(_UWORD));
              Read_CHKBIT(&p->ptr[act].chkbit);
              Read(&p->ptr[act].group, sizeof(_UWORD));
              Read(&p->ptr[act].userconfig, sizeof(_UWORD));
              Read_COLOR(&p->ptr[act].active);
              Read_COLOR(&p->ptr[act].passive);
              Read_COLOR(&p->ptr[act].quit_active);
              Read_COLOR(&p->ptr[act].quit_passive);
              Read_ANYPARA(&p->ptr[act].text);
              Read_ANYPARA(&p->ptr[act].helptext);

              if(lno > 1)
              {
                for(j=1; j<lno; j++)
                {
                  tmp                = p->ptr[act+j].link;
                  p->ptr[act+j]      = p->ptr[act];
                  p->ptr[act+j].link = tmp;
                }
              }
            }
          }
        }
      }
    }
  }

  if(GetError() == _TRUE)
  {
_AMEMERROR:  
    retcode = _FALSE;
    free_ALARM(p);
  }

  return(retcode);
}

_BOOL _File::Write_DATE(_DATE *p)
{
  return(Write(p, sizeof(_DATE)));
}

_BOOL _File::Read_TIME(_TIME *p)
{
  _BOOL retcode = _TRUE;

  if(Read(p, sizeof(_TIME)) == _FALSE)
  {
    retcode = _TRUE;
    init_TIME(p);
  }

  return(retcode);
}

_BOOL _File::Write_TIME(_TIME *p)
{
  return(Write(p, sizeof(_TIME)));
}

_BOOL _File::Read_DATIM(_DATIM *p)
{
  _BOOL retcode = _FALSE;

  if(Read_DATE(&p->fdate) == _TRUE)
  {
    if(Read_TIME(&p->ftime) == _TRUE)
    {
      retcode = _TRUE;
    }
  }

  if(retcode == _FALSE)
    init_DATIM(p);

  return(retcode);
}

_BOOL _File::Write_DATIM(_DATIM *p)
{
  Write_DATE(&p->fdate);
  Write_TIME(&p->ftime);

  return((GetError() == _TRUE) ? _FALSE :_TRUE);
}

_BOOL _File::Read_ZORDER(_ZORDER *p)
{
  _BOOL retcode = _FALSE;

  init_ZORDER(p);

  if(GetError() == _FALSE)
  {
    retcode = _TRUE;
    Read(&p->ftype, sizeof(_UBYTE));
    Read(&p->no,    sizeof(_UWORD));
  
    if(GetError() == _TRUE)
    {
      free_ZORDER(p);
      retcode = _FALSE;
    }
  }

  return(retcode);
}

_BOOL _File::Read_MEM_ZORDER(_MEM_ZORDER *p)
{
  _UWORD  i, no;
  _UDWORD size;
  _BOOL   retcode = _FALSE;
  _ZORDER *pz;

  init_MEM_ZORDER(p);

  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    size = no * sizeof(_ZORDER);
    if(FileMemory((void**)&p->ptr, size) == _TRUE)
    {
      retcode = _TRUE;
      p->no   = no;
      for(i=0,pz=p->ptr; i<no; i++,pz++)
        Read_ZORDER(pz);
    }

    if(GetError() == _TRUE)
    {
      free_MEM_ZORDER(p);
      retcode = _FALSE;
    }
  }

  return(retcode);
}

_BOOL _File::Read_MEM_ATTRIB(_MEM_ATTRIB *p)
{
  _BOOL   retcode = _FALSE;
  _UWORD  no, i;
  _ATTRIB *pa;

  init_MEM_ATTRIB(p);

//  no = 100;
  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    if(FileMemory((void**)&p->ptr, no * sizeof(_ATTRIB)) == _TRUE)
    {
      retcode = _TRUE;

      for(i=0, pa=p->ptr; i<no; i++, pa++)
        Read_ATTRIB(pa);
      p->no = no;
    }

    if(GetError() == _TRUE)
    {
      free_MEM_ATTRIB(p);
      retcode = _FALSE;
    }
  }

  return(retcode);
}

_BOOL _File::Read_MEM_NO(_MEM_NO *p)
{
  _BOOL   retcode = _FALSE;
  _UDWORD no, i;
  _DWORD *pno;

  init_MEM_NO(p);
  if(Read(&no, sizeof(_UDWORD)) == _TRUE)
  {
    if(FileMemory((void**)&p->ptr, no * sizeof(_DWORD)) == _TRUE)
    {
      retcode = _TRUE;
      for(i=0, pno=p->ptr; i<no; i++, pno++)
        Read(pno, sizeof(_DWORD));
      p->no = no;
    }

    if(GetError() == _TRUE)
    {
      free_MEM_NO(p);
      retcode = _FALSE;
    }
  }

  return(retcode);
}

_BOOL _File::Read_MEM_COLOR(_MEM_COLOR *p)
{
  _BOOL  retcode = _FALSE;
  _UWORD no, i;
  _COLOR *pc;

  init_MEM_COLOR(p);

  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    if(FileMemory((void**)&p->ptr, no * sizeof(_COLOR)) == _TRUE)
    {
      p->no = no;

      for(i=0, pc=p->ptr; i<no; i++, pc++)
        Read_COLOR(pc);
    }

    if(GetError() == _FALSE)
      retcode = _TRUE;
  }

  if(retcode == _FALSE)
    free_MEM_COLOR(p);

  return(retcode);
}

_BOOL _File::Read_MEM_ANYTHING(_MEM_ANYTHING *p)
{
  _BOOL     retcode = _FALSE;
  _UWORD    no, i;
  _ANYTHING *pa;


  init_MEM_ANYTHING(p);

  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    if(FileMemory((void**)&p->ptr, no * sizeof(_ANYTHING)) == _TRUE)
    {
      p->no = no;

      for(i=0, pa=p->ptr; i<no; i++, pa++)
        Read_ANYTHING(pa);
    }

    if(GetError() == _FALSE)
      retcode = _TRUE;
  }

  if(retcode == _FALSE)
    free_MEM_ANYTHING(p);

  return(retcode);
}

_BOOL _File::Read_MEM_ANYPARA(_MEM_ANYPARA *p)
{
  _BOOL    retcode = _FALSE;
  _UWORD   no, i;
  _ANYPARA *pa;


  init_MEM_ANYPARA(p);

  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    if(FileMemory((void**)&p->ptr, no * sizeof(_ANYPARA)) == _TRUE)
    {
      p->no = no;

      for(i=0, pa=p->ptr; i<no; i++, pa++)
        Read_ANYPARA(pa);
    }

    if(GetError() == _FALSE)
      retcode = _TRUE;
  }

  if(retcode == _FALSE)
    free_MEM_ANYPARA(p);

  return(retcode);
}

_BOOL _File::Read_MEM_LINKSCHEME(_MEM_LINKSCHEME *p)
{
  _BOOL       retcode = _FALSE;
  _UWORD      no, i;
  _LINKSCHEME *pl;

  init_MEM_LINKSCHEME(p);

  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    if(FileMemory((void**)&p->ptr, no * sizeof(_LINKSCHEME)) == _TRUE)
    {
      p->no = no;

      for(i=0, pl=p->ptr; i<no; i++, pl++)
        Read_LINKSCHEME(pl);
    }

    if(GetError() == _FALSE)
      retcode = _TRUE;
  }

  if(retcode == _FALSE)
    free_MEM_LINKSCHEME(p);

  return(retcode);
}

_BOOL _File::Read_DYNCOLOR(_DYNCOLOR *p)
{
  _BOOL retcode;


  init_DYNCOLOR(p);

  retcode = Read_LINKSCHEME(&p->linkscheme);

  return(retcode);
}

_BOOL _File::Read_MEM_DYNCOLOR(_MEM_DYNCOLOR *p)
{
  _BOOL     retcode = _FALSE;
  _UWORD    no, i;
  _DYNCOLOR *pd;

  init_MEM_DYNCOLOR(p);

  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    if(FileMemory((void**)&p->ptr, no * sizeof(_DYNCOLOR)) == _TRUE)
    {
      p->no = no;

      for(i=0, pd=p->ptr; i<no; i++, pd++)
        Read_DYNCOLOR(pd);
    }

    if(GetError() == _FALSE)
      retcode = _TRUE;
  }

  if(retcode == _FALSE)
    free_MEM_DYNCOLOR(p);

  return(retcode);
}

_BOOL _File::Read_MEM_FONT(_MEM_FONT *p)
{
  _BOOL  retcode = _FALSE;
  _UWORD no, i;

  init_MEM_FONT(p);

  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    if(FileMemory((void**)&p->ptr, no * sizeof(_FONT)) == _TRUE)
    {
      p->no = no;

      for(i=0; i<no; i++)
        Read(&p->ptr[i], sizeof(_FONT));
    }

    if(GetError() == _FALSE)
      retcode = _TRUE;
  }

  if(retcode == _FALSE)
    free_MEM_FONT(p);

  return(retcode);
}

_BOOL _File::Read_MEM_BOOL(_MEM_BOOL *p)
{
  _BOOL retcode = _FALSE;
  _UWORD no, i;

  init_MEM_BOOL(p);

  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    if(FileMemory((void**)&p->ptr, no * sizeof(_BOOL)) == _TRUE)
    {
      retcode = _TRUE;

      for(i=0; i<no; i++)
        p->ptr[i] = (_BOOL)Read_ENUM();

      p->no = no;
    }

    if(GetError() == _TRUE)
    {
      free_MEM_BOOL(p);
      retcode = _FALSE;
    }
  }

  return(retcode);
}

_BOOL _File::Read_MEM_LSEFUNCT(_MEM_LSEFUNCT *p)
{
  _BOOL      retcode = _FALSE;
  _UWORD     no, i;
  _LSEFUNCT *pl;

  init_MEM_LSEFUNCT(p);

  if(Read(&no, sizeof(_UWORD)) == _TRUE)
  {
    if(FileMemory((void**)&p->ptr, no * sizeof(_LSEFUNCT)) == _TRUE)
    {
      p->no = no;

      for(i=0, pl=p->ptr; i<no; i++, pl++)
        Read_LSEFUNCT(pl);
    }

    if(GetError() == _FALSE)
      retcode = _TRUE;
  }

  if(retcode == _FALSE)
    free_MEM_LSEFUNCT(p);

  return(retcode);
}

#ifdef UC_IPC
_BOOL _File::Read_DPNE(_DPNE *p)
{
  _BOOL  retcode = _FALSE;
  _UWORD tmp;

  init_DPNE(p);

  if(GetError() == _FALSE)
  {
    if(Read(&tmp, sizeof(_UWORD)) == _TRUE)
    {
      if(Memory2((void**)&p->ptr, tmp) != 0)
      {
        if(Read(p->ptr, tmp) == _TRUE)
        {
          p->no   = tmp;
          retcode = _TRUE;
        }
      }
    }
  }

  if(retcode == _FALSE)
    free_DPNE(p);

  return(retcode);
}
#endif

_BOOL _File::Read_MYSCHEME(_MYSCHEME *p)
{
	_BOOL retcode;

	init_MYSCHEME_Makro(p);
  retcode = Read(&p->schemeno, sizeof(_UWORD));

  if(retcode == _FALSE)
    free_MYSCHEME(p);

	return(retcode);
}

_BOOL _File::Read_LINKSCHEME(_LINKSCHEME *p)
{
	_BOOL retcode = _FALSE;

	init_LINKSCHEME_Makro(p);

	if(GetError() == _FALSE)
	{
		if(Read_MYSCHEME(&p->myscheme) == _TRUE)
    {
      retcode = Read_VARLABEL(&p->varlabel);
    }

    if(retcode == _FALSE)
    {
      free_LINKSCHEME(p);
    }
	}

	return(retcode);
}

_BOOL _File::Read_FRAMETYPE(_FRAMETYPE *p)
{
//Read(p, sizeof(_FRAMETYPE));
//*p = 1;
//return _TRUE;


  return(Read(p, sizeof(_FRAMETYPE)));
}

_BOOL _File::Read_SINGLESCHEME(_SINGLESCHEME *p, _BOOL value4byte)
{
	_BOOL  retcode = _TRUE;
//  _UWORD value;

	init_SINGLESCHEME(p);

	if(GetError() == _FALSE)
	{
    p->value = 0;
    Read_VARLABEL(&p->upto);
    Read(&p->op, 1);
    Read(&p->value, (value4byte == _TRUE)? sizeof(_UDWORD):sizeof(_UWORD));
	}

	if(GetError() == _TRUE)
  {
    free_SINGLESCHEME(p);
    retcode = _FALSE;
  }

	return(retcode);
}

_BOOL _File::Read_MEM_SINGLESCHEME(_MEM_SINGLESCHEME *p, _BOOL value4byte)
{
  _UWORD no, i;
  _BOOL  retcode = _TRUE;

  init_MEM_SINGLESCHEME(p);

  if(Read(&no, 2) == _TRUE)
  {
    if(Memory2((void**)&p->ptr, no * sizeof(_SINGLESCHEME)) != 0)
    {
      p->no = no;
      for(i=0; i<no; i++)
        Read_SINGLESCHEME(&p->ptr[i], value4byte);
    }
  }

	if(GetError() == _TRUE)
  {
    free_MEM_SINGLESCHEME(p);
    retcode = _FALSE;
  }

  return(retcode);
}

_BOOL _File::Read_SCHEME(_SCHEME *p, _BOOL inclusivlink, _BOOL value4byte)
{
	_BOOL retcode = _TRUE;

	init_SCHEME_Makro(p);

	if(GetError() == _FALSE)
	{
    if(inclusivlink == _TRUE)
      Read_VARLABEL(&p->link);

    Read_MEM_SINGLESCHEME(&p->memsinglescheme, value4byte);
	}

	if(GetError() == _TRUE)
  {
    free_SCHEME(p);
    retcode = _FALSE;
  }

	return(retcode);
}

_BOOL _File::Read_MEM_SCHEME(_MEM_SCHEME *p, _BOOL inclusivlink, _BOOL value4byte)
{
	_BOOL  retcode = _TRUE;
  _UWORD i, no;

	init_MEM_SCHEME(p);

	if(GetError() == _FALSE)
	{
    if(Read(&no, 2) == _TRUE)
    {
      if(FileMemory((void**)&p->ptr, no * sizeof(_SCHEME)) == _TRUE)
      {
        p->no = no;
        for(i=0; i<no; i++)
        {
//          init_COLSCHEME(&p->ptr[i]);
          Read_SCHEME(&p->ptr[i], inclusivlink, value4byte);
        }
      }
    }
	}

	if(GetError() == _TRUE)
  {
    free_MEM_SCHEME(p);
    retcode = _FALSE;
  }

	return(retcode);
}

_BOOL _File::Read_TEXT_BUFFER(_TEXT_BUFFER *p, _BOOL *unicode)
{
  // [] .......... size of character (ascii or unicode)
  // [][][][] .... no of text (ab compiler version 32 4byte, vorher 2byte)
  // [][][][] .... no of characters
  // [ ........... characters (ascii-0-string)
  _UDWORD byte;
  _BOOL   retcode = _FALSE;
  _UBYTE  sc;
  _CHAR   *tmp = NULL;

  init_TEXT_BUFFER(p);

  if(GetError() == _FALSE)
  {
    Read(&sc, sizeof(_UBYTE));             // load sizeof one character

    if(unicode != NULL)
    {
      if(sc == 1)
        *unicode = _FALSE;
      else
        *unicode = _TRUE;
    }

   #if COMPILER_VERSION >= 32
    Read(&p->no, sizeof(_UDWORD));                    // load no of text
   #else
    p->no = 0;
    Read(&p->no, sizeof(_UWORD));                    // load no of text
   #endif
    Read(&p->size, sizeof(_UDWORD));           // load no of characters

    byte = p->size * sc;                       // calculate no of bytes
    FileMemory((void**)&p->ptr, byte);                  // alloc memory
    Read(p->ptr, byte);                                    // load text
    if(GetError() == _FALSE)
    {
      retcode = _TRUE;
      if(p->size > 0)
      {
        if((sc == 1) && (sizeof(_CHAR) == 2))
        {
          retcode = _FALSE;
          if(Memory2((void**)&tmp, p->size*2) != 0)
          {
            AsciiToUni((_UNI*)tmp, (_ASCII*)p->ptr, p->size);
            Memory2((void**)&p->ptr, 0);
            p->ptr  = tmp;
            tmp     = NULL;
            retcode = _TRUE;
          }
        }
        else if((sc == 2) && (sizeof(_CHAR) == 1))
        {
          retcode = _FALSE;
          if(Memory2((void**)&tmp, p->size) != 0)
          {
            UniToAscii((_ASCII*)tmp, (_UNI*)p->ptr, p->size);
            Memory2((void**)&p->ptr, 0);
            p->ptr  = tmp;
            tmp     = NULL;
            retcode = _TRUE;
          }
        }
      }
    }
  }

  if(retcode == _FALSE)
    free_TEXT_BUFFER(p);
  else
    calculate_offset_TEXT_BUFFER(p);

  return(retcode);
}


_BOOL _File::Read_VARBLOCK(_VARBLOCK *p)
{
  _UDWORD     i, tmp, size;
  _BOOL       retcode;
  _LSEVARINFO *pvi;
  _VARSYMBOL  *pvs;

  init_VARBLOCK(p);

  if(GetError() == _FALSE)
  {
    Read(&tmp, (sizeof(_UDWORD)));
    p->no = tmp;

    // infotabel
    size = tmp * sizeof(_LSEVARINFO);
    if(FileMemory((void**)&p->MemLseVarInfo.ptr, size) == _TRUE)
    {
      p->MemLseVarInfo.no = tmp;
      for(i=0, pvi=p->MemLseVarInfo.ptr; i<tmp; i++, pvi++)
        Read_LSEVARINFO(pvi);
     #ifdef UC_LINKVL_FAST
      for(i=0, pvi=p->MemLseVarInfo.ptr; i<tmp; i++, pvi++)
        VarList.LinkVlBuffer.Finalize(pvi);
     #endif
    }

    // symboltable
    size = tmp * sizeof(_VARSYMBOL);
    if(FileMemory((void**)&p->MemVarSymbol.ptr, size) == _TRUE)
    {
      p->MemVarSymbol.no = tmp;
      for(i=0, pvs=p->MemVarSymbol.ptr; i<tmp; i++, pvs++)
        Read_VARSYMBOL(pvs);
    }

    // label
    p->Label.Read(this, _TRUE);
//   #ifndef UC_CASE_SENSITIVE
//    p->Label.StrUp();           // convert to big letters
//   #endif
  }

  retcode = _TRUE;
  if(GetError() == _TRUE)
  {
    free_VARBLOCK(p);
    p->no   = tmp;
    retcode = _FALSE;
  }

  return(retcode);
}

_BOOL _File::Write_HEADER(const _ASCII *idstring, _UDWORD version)
{
  _UBYTE header[64];

  // 0,1,2,3 ....... crc32 (0)
  // 4,5,6,7 ....... length of header
  // 8,9,10,11 ..... version
  // 12,13,14,15 ... crc32 of project dpne
  // 16 ............ id-string

  MemSet(header, 0xFF, 64);
  *(_UDWORD*)&header[0]  = 0;                                          // crc
  *(_UDWORD*)&header[4]  = 64;                            // length of header
  *(_UDWORD*)&header[8]  = version;                                // version
  *(_UDWORD*)&header[12] = Project.GetCrc32();       // crc32 of project dpne

  if(idstring != NULL)
  {
    StrCpy(&header[16], sizeof(_ASCII), idstring, sizeof(_ASCII));
  }

  return(Write(header, 64));
}

_BOOL _File::Read_HEADER(_UDWORD *version, const _ASCII *idstring, _BOOL check_lsecrc)
{
  _BOOL  st, retcode = _FALSE;
  _UBYTE header[64];

  if(Read(header, 64) == _TRUE)
  {
    if(*(_UDWORD*)&header[4] == 64)
    {
      if((check_lsecrc == _FALSE) || (*(_UDWORD*)&header[12] == Project.GetCrc32()))
      {
        st = _TRUE;
        if(idstring != NULL)
        {
          st = (StrXmp(&header[16], (void*)idstring, sizeof(_ASCII)) == 0)? _TRUE : _FALSE;
        }
        
        if(st == _TRUE)
        {
          *version = *(_UDWORD*)&header[8];
          retcode = _TRUE;
        }
      }
    }
  }

  return(retcode);
}

_BOOL _File::Write_ENUM(_UDWORD tmp)
{
  return(Write(&tmp, sizeof(_UWORD)));
}

_BOOL _File::Write_VARSINGLE(_VARSINGLE *p)
{
  if(GetError() == _FALSE)
  {
    Write_ENUM(p->state);
    Write(&p->value, sizeof(_UDWORD));
  }

  return((GetError() == _FALSE)? _TRUE : _FALSE);
}

_BOOL _File::Write_VARLABEL(_VARLABEL *p)
{
  _UBYTE i, tmp;

  if(GetError() == _FALSE)
  {
    tmp = p->no;
    if(Write(&tmp, sizeof(_UBYTE)) == _TRUE)
    {
      for(i=0; i<tmp; i++)
        Write_VARSINGLE(&p->info[i]);
    }
  }

  return((GetError() == _FALSE)? _TRUE : _FALSE);
}

_BOOL _File::Write_MEM_VARLABEL(_MEM_VARLABEL *p)
{
  _UWORD  i;

  if(GetError() == _FALSE)
  {
    Write(&p->no, sizeof(_UWORD));
    for(i=0; i<p->no; i++)
      Write_VARLABEL(&p->ptr[i]);
  }

  return((GetError() == _FALSE)? _TRUE : _FALSE);
}

_BOOL _File::Read_SINGLETXTSCHEME(_SINGLETXTSCHEME *p)
{
  _BOOL   retcode = _TRUE;


  init_SINGLETXTSCHEME(p);

  if(GetError() == _FALSE)
  {
    Read_VARLABEL(&p->upto);
    Read(&p->op, sizeof(_UBYTE));
    Read_ANYPARA(&p->anypara);
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_SINGLETXTSCHEME(p);
  }

  return(retcode);
}

_BOOL _File::Read_TXTSCHEME(_TXTSCHEME *p)
{
  _BOOL   retcode = _TRUE;
  _UWORD  i, tmp;
  _UDWORD size;

  init_TXTSCHEME(p);

  if(GetError() == _FALSE)
  {
    if(Read(&tmp, sizeof(_UWORD)) == _TRUE)
    {
      size = tmp * sizeof(_SINGLETXTSCHEME);
      if(FileMemory((void**)&p->ptr, size) != 0)
      {
        p->no = tmp;
        for(i=0; i<tmp; i++)
          Read_SINGLETXTSCHEME(&p->ptr[i]);
      }
    }
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_TXTSCHEME(p);
  }

  return(retcode);
}


_BOOL _File::Read_MEM_TXTSCHEME(_MEM_TXTSCHEME *p)
{
  _BOOL   retcode = _TRUE;
  _UWORD  i, tmp;
  _UDWORD size;

  init_MEM_TXTSCHEME(p);

  if(GetError() == _FALSE)
  {
    if(Read(&tmp, sizeof(_UWORD)) == _TRUE)
    {
      size = tmp * sizeof(_TXTSCHEME);
      if(FileMemory((void**)&p->ptr, size) != 0)
      {
        p->no = tmp;
        for(i=0; i<tmp; i++)
          Read_TXTSCHEME(&p->ptr[i]);
      }
    }
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_MEM_TXTSCHEME(p);
  }

  return(retcode);
}

_BOOL _File::Read_THEURGE(_THEURGE *p)
{
  _BOOL  retcode = _FALSE;


  init_THEURGE(p);

  if(GetError() == _FALSE)
  {
    p->urgetype = (_URGETYPE)Read_ENUM();
    Read(&p->info, sizeof(p->info));
    Read_MEM_FONT(&p->memfont);
    Read_MEM_ATTRIB(&p->memattrib);
    Read_MEM_COLOR(&p->memcolor);
    Read_MEM_VARLABEL(&p->memvarlabel);
    Read_MEM_ANYTHING(&p->memanything);
    Read_MEM_DYNCOLOR(&p->memdyncolor);
    Read_MEM_LSEFUNCT(&p->memlsefunct);
    Read_MEM_ANYPARA(&p->memanypara);
    Read_MEM_BOOL(&p->membool);
    Read_MEM_ANYTHING(&p->memimage);

    if(GetError() == _FALSE)
      retcode = check_THEURGE(p);
  }

  if(retcode == _FALSE)
    free_THEURGE(p);

  return(retcode);

}

_BOOL _File::Read_MEM_MAP(_MEM_MAP *p)
{
  _BOOL   retcode = _FALSE;
  _UDWORD tmp, size;

  init_MEM_MAP(p);
  if(GetError() == _FALSE)
  {
    if(Read(&tmp, sizeof(_UDWORD)) == _TRUE)
    {
      size = tmp*sizeof(_UDWORD);
      if(FileMemory((void**)&p->ptr, size) != 0)
      {
        p->no = tmp;
        Read(p->ptr, size);
      }
    }
  }

  if(GetError() == _FALSE)
    retcode = _TRUE;
  else
    free_MEM_MAP(p);

  return(retcode);
}

_BOOL _File::Write_ANYTHING(_ANYTHING *p)
{
  Write_ENUM(p->list);
 #if COMPILER_VERSION >= 32
  return(Write(&p->no, sizeof(_UDWORD)));
 #else
  return(Write(&p->no, sizeof(_UWORD)));
 #endif
}


_BOOL _File::Memory_SINGLEALARM(_UBYTE *data, _SINGLEALARM *p, _UDWORD version)
{
  // version 0:
  // [4] .... seqno
  // [2] .... no, bei SA34706 sind 4byte vorhanden
  // [1] .... state
  // [1] .... no parameter
  // +[4] ... parameter
  // [4] .... time_on
  // [4] .... date_on
  // [4] .... time_off
  // [4] .... date_off
  // [1] .... acknowledge
  // [2] .... count
  // [2] .... anything.list
  // [4] .... anything.no
  // [4] .... alarmid (ab version 1)
  // [2] .... userdatasize
  // [ ...... userdata
  // [2] .... entrytype
  // [1] .... cpuno nur bei UC_MULTICPU

  _BOOL retcode = _FALSE;
  
  if(GetError() == _FALSE)
  {
    _UDWORD offset = 0;
    _UDWORD no_para;

    MemSet(data, 0xFF, 512);

    *(_UDWORD*)&data[offset] = p->seqno;         offset += sizeof(_UDWORD); // 4
   #ifdef SA34706
    *(_UDWORD*)&data[offset] = p->no;            offset += sizeof(_UDWORD); // 4 
   #else
    *(_UWORD*) &data[offset] = p->no;            offset += sizeof(_UWORD);  // 2
   #endif
    *(_UBYTE*) &data[offset] = p->state;         offset += sizeof(_UBYTE);  // 1
    *(_UBYTE*) &data[offset] = p->no_para;       offset += sizeof(_UBYTE);  // 1

   #ifdef UC_ALARMGROUPS
    no_para = p->no_para & 0x7F;
   #else
    no_para = p->no_para;
   #endif

    for(_UDWORD i=0; i<no_para; i++)
    {
      *(_DWORD*)&data[offset] = p->para[i];      offset += sizeof(_DWORD);  // 4
    }

    *(_DWORD*) &data[offset] = p->time_on;       offset += sizeof(_DWORD);  // 4
    *(_DWORD*) &data[offset] = p->date_on;       offset += sizeof(_DWORD);  // 4
    *(_DWORD*) &data[offset] = p->time_off;      offset += sizeof(_DWORD);  // 4
    *(_DWORD*) &data[offset] = p->date_off;      offset += sizeof(_DWORD);  // 4
    *(_UBYTE*) &data[offset] = p->acknowledge;   offset += sizeof(_UBYTE);  // 1
    *(_UWORD*) &data[offset] = p->cnt;           offset += sizeof(_UWORD);  // 2

    *(_UWORD*) &data[offset] = p->anything.list; offset += sizeof(_UWORD);  // 2
   #if COMPILER_VERSION >= 32
    *(_UDWORD*)&data[offset] = p->anything.no;   offset += sizeof(_UDWORD); // 4
   #else
    *(_UWORD*) &data[offset] = p->anything.no;   offset += sizeof(_UWORD);  // 2
   #endif
    if(version > 0)
    {
      *(_UDWORD*)&data[offset] = p->alarmid;     offset += sizeof(_UDWORD); // 4
    }

   #ifdef UC_STATIC_ALARMSIZE
    *(_UWORD*)&data[offset] = UC_STATIC_ALARMSIZE; offset += sizeof(_UWORD);  // 2
   #else
    *(_UWORD*)&data[offset] = p->ptrsize;       offset += sizeof(_UWORD);  // 2
   #endif

   #ifdef UC_MULTICPU
   #ifdef UC_STATIC_ALARMSIZE
    if(UC_STATIC_ALARMSIZE <= (509-offset))
   #else
    if(p->ptrsize <= (509-offset))
   #endif

   #else
   #ifdef UC_STATIC_ALARMSIZE
    if(UC_STATIC_ALARMSIZE <= (510-offset))
   #else
    if(p->ptrsize <= (510-offset))
   #endif
   #endif
    {
     #ifdef UC_STATIC_ALARMSIZE
      MemCpy(&data[offset], p->ptr, UC_STATIC_ALARMSIZE); offset += UC_STATIC_ALARMSIZE;
     #else
      MemCpy(&data[offset], p->ptr, p->ptrsize); offset += p->ptrsize;
     #endif

      *(_UWORD*) &data[offset] = p->entrytype;   offset += sizeof(_UWORD);  // 2

     #ifdef UC_MULTICPU
      *(_UBYTE*)&data[offset] = p->multicpuno;   offset += sizeof(_UBYTE);  // 1
     #endif
     
     if(offset <= 512)
     {
       retcode = _TRUE;
     }
    }
  }
  
  return retcode;
}



_BOOL _File::Write_SINGLEALARM(_SINGLEALARM *p, _UDWORD version)
{
  // version 0:
  // [4] .... seqno
  // [2] .... no, bei SA34706 sind 4byte vorhanden
  // [1] .... state
  // [1] .... no parameter
  // +[4] ... parameter
  // [4] .... time_on
  // [4] .... date_on
  // [4] .... time_off
  // [4] .... date_off
  // [1] .... acknowledge
  // [2] .... count
  // [2] .... anything.list
  // [4] .... anything.no
  // [4] .... alarmid (ab version 1)
  // [2] .... userdatasize
  // [ ...... userdata
  // [2] .... entrytype
  // [1] .... cpuno nur bei UC_MULTICPU

  _UBYTE data[512];
  _BOOL  retcode = _FALSE;

  if(GetError() == _FALSE)
  {
    if(Memory_SINGLEALARM(data, p, version) == _TRUE)
    {
      retcode = Write(data, 512);
    }
  }
  
  return retcode;
/*  
  _UWORD offset, i;
  _UBYTE no_para;
  
    MemSet(data, 0xFF, 512);

    offset = 0;
    *(_UDWORD*)&data[offset] = p->seqno;         offset += sizeof(_UDWORD); // 4
   #ifdef SA34706
    *(_UDWORD*)&data[offset] = p->no;            offset += sizeof(_UDWORD); // 4 
   #else
    *(_UWORD*) &data[offset] = p->no;            offset += sizeof(_UWORD);  // 2
   #endif
    *(_UBYTE*) &data[offset] = p->state;         offset += sizeof(_UBYTE);  // 1
    *(_UBYTE*) &data[offset] = p->no_para;       offset += sizeof(_UBYTE);  // 1

   #ifdef UC_ALARMGROUPS
    no_para = p->no_para & 0x7F;
   #else
    no_para = p->no_para;
   #endif

    for(i=0; i<no_para; i++)
    {
      *(_DWORD*)&data[offset] = p->para[i];      offset += sizeof(_DWORD);  // 4
    }

    *(_DWORD*) &data[offset] = p->time_on;       offset += sizeof(_DWORD);  // 4
    *(_DWORD*) &data[offset] = p->date_on;       offset += sizeof(_DWORD);  // 4
    *(_DWORD*) &data[offset] = p->time_off;      offset += sizeof(_DWORD);  // 4
    *(_DWORD*) &data[offset] = p->date_off;      offset += sizeof(_DWORD);  // 4
    *(_UBYTE*) &data[offset] = p->acknowledge;   offset += sizeof(_UBYTE);  // 1
    *(_UWORD*) &data[offset] = p->cnt;           offset += sizeof(_UWORD);  // 2

    *(_UWORD*) &data[offset] = p->anything.list; offset += sizeof(_UWORD);  // 2
   #if COMPILER_VERSION >= 32
    *(_UDWORD*)&data[offset] = p->anything.no;   offset += sizeof(_UDWORD); // 4
   #else
    *(_UWORD*) &data[offset] = p->anything.no;   offset += sizeof(_UWORD);  // 2
   #endif
    if(version > 0)
    {
      *(_UDWORD*)&data[offset] = p->alarmid;     offset += sizeof(_UDWORD); // 4
    }

   #ifdef UC_STATIC_ALARMSIZE
    *(_UWORD*)&data[offset] = UC_STATIC_ALARMSIZE; offset += sizeof(_UWORD);  // 2
   #else
    *(_UWORD*)&data[offset] = p->ptrsize;       offset += sizeof(_UWORD);  // 2
   #endif

   #ifdef UC_MULTICPU
   #ifdef UC_STATIC_ALARMSIZE
    if(UC_STATIC_ALARMSIZE <= (509-offset))
   #else
    if(p->ptrsize <= (509-offset))
   #endif

   #else
   #ifdef UC_STATIC_ALARMSIZE
    if(UC_STATIC_ALARMSIZE <= (510-offset))
   #else
    if(p->ptrsize <= (510-offset))
   #endif
   #endif
    {
     #ifdef UC_STATIC_ALARMSIZE
      MemCpy(&data[offset], p->ptr, UC_STATIC_ALARMSIZE); offset += UC_STATIC_ALARMSIZE;
     #else
      MemCpy(&data[offset], p->ptr, p->ptrsize); offset += p->ptrsize;
     #endif

      *(_UWORD*) &data[offset] = p->entrytype;   offset += sizeof(_UWORD);  // 2

     #ifdef UC_MULTICPU
      *(_UBYTE*)&data[offset] = p->multicpuno;   offset += sizeof(_UBYTE);  // 1
     #endif


      if(offset <= 512)
        retcode = Write(data, 512);
    }
  }

  return(retcode);
*/  
}

_BOOL _File::Read_NAME_INDEX(_NAME_INDEX *p)
{
  init_NAME_INDEX(p);

  if(GetError() == _FALSE)
  {
    Read_ASCII_BUFFER(&p->name);
    Read_MEM_NO(&p->index);

    if(GetError() == _TRUE)
      free_NAME_INDEX(p);
    else
      p->crc32 = make_crc_ASCII_BUFFER(&p->name);
  }

  return((GetError() == _FALSE)? _TRUE : _FALSE);
}

_BOOL _File::Read_SINGLEALARM(_SINGLEALARM *p, _UDWORD version)
{
  _UWORD offset, size, i;
  _UBYTE data[512];
  _BOOL  retcode = _FALSE;
  _UBYTE no_para;

  init_SINGLEALARM(p);

  if(Read(data, 512) == _TRUE)
  {
    offset    = 0;
    p->seqno  = *(_UDWORD*)&data[offset]; offset += sizeof(_UDWORD); // 4
    retcode   = _TRUE;

    if(p->seqno != 0xFFFFFFFFL)
    {
     #ifdef SA34706
      p->no            = *(_UDWORD*)&data[offset]; offset += sizeof(_UDWORD); // 4
     #else
      p->no            = *(_UWORD*)&data[offset];  offset += sizeof(_UWORD);  // 2
     #endif
     
      p->state         = *(_UBYTE*)&data[offset];  offset += sizeof(_UBYTE);  // 1
      p->no_para       = *(_UBYTE*)&data[offset];  offset += sizeof(_UBYTE);  // 1

     #ifdef UC_ALARMGROUPS
      no_para = p->no_para & 0x7F;
     #else
      no_para = p->no_para;
     #endif

      if(no_para > SINGLEALARMMAXPARANO)
      {
        //fehler im file !!
        //free_SINGLEALARM(p);
        //return _FALSE;
        
        p->entrytype = 0xFFFE;
        p->cnt       = 13; // dr1659, sa21676
        return _TRUE;
      }
      
      for(i=0; i<no_para; i++)
      {
        p->para[i]     = *(_DWORD*)&data[offset];  offset += sizeof(_DWORD);  // 4
      }

      p->time_on       = *(_DWORD*)&data[offset];  offset += sizeof(_DWORD);  // 4
      p->date_on       = *(_DWORD*)&data[offset];  offset += sizeof(_DWORD);  // 4
      p->time_off      = *(_DWORD*)&data[offset];  offset += sizeof(_DWORD);  // 4
      p->date_off      = *(_DWORD*)&data[offset];  offset += sizeof(_DWORD);  // 4
      p->acknowledge   = *(_UBYTE*)&data[offset];  offset += sizeof(_UBYTE);  // 1
      p->cnt           = *(_UWORD*)&data[offset];  offset += sizeof(_UWORD);  // 2

      p->anything.list = (_ANYLST)*(_UWORD*)&data[offset];  offset += sizeof(_UWORD);  // 2
     #if COMPILER_VERSION >= 32
      p->anything.no   = *(_UDWORD*)&data[offset]; offset += sizeof(_UDWORD); // 4
     #else
      p->anything.no   = *(_UWORD*)&data[offset];  offset += sizeof(_UWORD);  // 2
     #endif

      if(version >= 1)
      {
        p->alarmid     = *(_UDWORD*)&data[offset]; offset += sizeof(_UDWORD); // 4
      }

      size             = *(_UWORD*)&data[offset];  offset += sizeof(_UWORD);  // 2

     #ifdef UC_STATIC_ALARMSIZE
      _UWORD static_size;

      static_size = size;

      if(static_size > UC_STATIC_ALARMSIZE)
        static_size = UC_STATIC_ALARMSIZE;

      MemCpy(p->ptr, &data[offset], static_size);
      offset += size;
     #else
      if(size > 0)
      {
        PushMemColor(MEMORY_PRG);
        if(Memory2((void**)&p->ptr, size) != 0)
        {
          p->ptrsize     = size;
          MemCpy(p->ptr, &data[offset], size);     offset += size;
        }
        PopMemColor();
      }
     #endif

      p->entrytype   = *(_UWORD*)&data[offset];    offset += sizeof(_UWORD);  // 2
     #ifdef UC_MULTICPU
      p->multicpuno  = *(_UBYTE*)&data[offset];    offset += sizeof(_UBYTE);  // 1
     #endif

      if(offset > 512)
      {
        retcode = _FALSE;
        free_SINGLEALARM(p);
      }
    }
  }

  return(retcode);
}

#ifdef UC_THEME

#define TID_TL   0
#define TID_TR   1
#define TID_BR   2
#define TID_BL   3
#define TID_T    4
#define TID_R    5
#define TID_B    6
#define TID_L    7
#define TID_TL_P 8
#define TID_TR_P 9
#define TID_BR_P 10
#define TID_BL_P 11
#define TID_T_P  12
#define TID_R_P  13
#define TID_B_P  14
#define TID_L_P  15
#define TID_C    16
#define TID_C_P  17
#define TID_CX   18
#define TID_CX_P 19

/*
_WORD tu_was(_WORD val, _WORD mul, _WORD div)
{
  if(val > 0)
    return(((val * mul) + div/2) / div);
  else
    return(((val * mul) - div/2) / div);
}
*/

/*
void save_bmtxt(_BMP *bmp, char *nam)
{
  char  txt[100];
  char  crlf[2];
  _File F;
  F.Open(SAVE, nam);
  
  crlf[0] = 13;
  crlf[1] = 10;
  
  DToH(txt, bmp->width, 0x800, 1);
  F.Write("0x", 2);
  F.Write(txt, StrLen(txt, 1));
  F.Write(",", 1);
  DToH(txt, bmp->height, 0x800, 1);
  F.Write("0x", 2);
  F.Write(txt, StrLen(txt, 1));
  F.Write(",", 1);
  F.Write(crlf, 2);

  _UDWORD no  = bmp->datano;
  _BMPDAT *pp = bmp->ptr;
  while(no)
  {
    for(_UDWORD i=0; i<pp->no; i++)
    {
      unsigned char xc = (pp->col >> 5) & 0x3F;
      if(pp->col == 33808)
        xc = 0xFF;
      if(pp->col == 63519)
      	xc = 0xFE;
//      DToH(txt, xc>>1, 0x800, 1);
      DToH(txt, xc, 0x800, 1);
      F.Write("0x", 2);
      F.Write(txt, StrLen(txt, 1));
      F.Write(",", 1);
    }
    pp ++;
    no --;
  }
  
  F.Close();
}
*/
_BOOL _File::Read_THEME(_THEME *p)
{
  _BOOL  retcode = _FALSE;
  _UBYTE no, id;
  _BMP   dummy;

  init_THEME(p);

  if(GetError() == _FALSE)
  {
    retcode = _TRUE;

    Read(&p->id, sizeof(_UBYTE));
    Read(&p->delta_t, sizeof(_WORD));  // p->delta_t = tu_was(p->delta_t, 768, 240);
    Read(&p->delta_r, sizeof(_WORD));  // p->delta_r = tu_was(p->delta_r, 1024, 320);
    Read(&p->delta_b, sizeof(_WORD));  // p->delta_b = tu_was(p->delta_b, 768, 240);
    Read(&p->delta_l, sizeof(_WORD));  // p->delta_l = tu_was(p->delta_l, 1024, 320);
   #if COMPILER_VERSION >= 47
    Read(&p->xpos_cx, sizeof(_WORD));
    Read(&p->ypos_cx, sizeof(_WORD));
   #endif
    Read(&p->colorcourse, sizeof(_UBYTE));

    if(Read(&no, sizeof(_UBYTE)) == _TRUE)
    {

      while(no)
      {
        no -= 1;
        if(Read(&id, sizeof(_UBYTE)) == _TRUE)
        {
          switch(id)
          {
            case TID_TL   : Read_BMP(&p->frame0.bmp_tl); break;
            case TID_TR   : Read_BMP(&p->frame0.bmp_tr); break;
            case TID_BR   : Read_BMP(&p->frame0.bmp_br); break;
            case TID_BL   : Read_BMP(&p->frame0.bmp_bl); break;
            case TID_L    : Read_BMP(&p->frame0.bmp_l);  break;
            case TID_R    : Read_BMP(&p->frame0.bmp_r);  break;
            case TID_B    : Read_BMP(&p->frame0.bmp_b);  break;
            case TID_T    : Read_BMP(&p->frame0.bmp_t);  break;
            case TID_C    : Read_BMP(&p->frame0.bmp_c);  break;
            case TID_CX   : Read_BMP(&p->frame0.bmp_cx); break;
            case TID_TL_P : Read_BMP(&p->frame1.bmp_tl); break;
            case TID_TR_P : Read_BMP(&p->frame1.bmp_tr); break;
            case TID_BR_P : Read_BMP(&p->frame1.bmp_br); break;
            case TID_BL_P : Read_BMP(&p->frame1.bmp_bl); break;
            case TID_L_P  : Read_BMP(&p->frame1.bmp_l);  break;
            case TID_R_P  : Read_BMP(&p->frame1.bmp_r);  break;
            case TID_B_P  : Read_BMP(&p->frame1.bmp_b);  break;
            case TID_T_P  : Read_BMP(&p->frame1.bmp_t);  break;
            case TID_C_P  : Read_BMP(&p->frame1.bmp_c);  break;
            case TID_CX_P : Read_BMP(&p->frame1.bmp_cx); break;
            default       : init_BMP_Makro(&dummy);
                            Read_BMP(&dummy);
                            free_BMP(&dummy);            break;
          }
/*
          switch(id)
          {
            case TID_TL   : Read_BMP(&p->frame0.bmp_tl); zoom_BMP(&p->frame0.bmp_tl, (p->frame0.bmp_tl.width * 1024)/800, (p->frame0.bmp_tl.height * 768)/600); break;
            case TID_TR   : Read_BMP(&p->frame0.bmp_tr); zoom_BMP(&p->frame0.bmp_tr, (p->frame0.bmp_tr.width * 1024)/800, (p->frame0.bmp_tr.height * 768)/600); break;
            case TID_BR   : Read_BMP(&p->frame0.bmp_br); zoom_BMP(&p->frame0.bmp_br, (p->frame0.bmp_br.width * 1024)/800, (p->frame0.bmp_br.height * 768)/600); break;
            case TID_BL   : Read_BMP(&p->frame0.bmp_bl); zoom_BMP(&p->frame0.bmp_bl, (p->frame0.bmp_bl.width * 1024)/800, (p->frame0.bmp_bl.height * 768)/600); break;
            case TID_L    : Read_BMP(&p->frame0.bmp_l);  zoom_BMP(&p->frame0.bmp_l,  (p->frame0.bmp_l.width * 1024)/800,  (p->frame0.bmp_l.height * 768)/600); break;
            case TID_R    : Read_BMP(&p->frame0.bmp_r);  zoom_BMP(&p->frame0.bmp_r,  (p->frame0.bmp_r.width * 1024)/800,  (p->frame0.bmp_r.height * 768)/600); break;
            case TID_B    : Read_BMP(&p->frame0.bmp_b);  zoom_BMP(&p->frame0.bmp_b,  (p->frame0.bmp_b.width * 1024)/800,  (p->frame0.bmp_b.height * 768)/600); break;
            case TID_T    : Read_BMP(&p->frame0.bmp_t);  zoom_BMP(&p->frame0.bmp_t,  (p->frame0.bmp_t.width * 1024)/800,  (p->frame0.bmp_t.height * 768)/600); break;
            case TID_C    : Read_BMP(&p->frame0.bmp_c);  zoom_BMP(&p->frame0.bmp_c,  (p->frame0.bmp_c.width * 1024)/800,  (p->frame0.bmp_c.height * 768)/600); break;
            case TID_CX   : Read_BMP(&p->frame0.bmp_cx); zoom_BMP(&p->frame0.bmp_cx, (p->frame0.bmp_cx.width * 1024)/800, (p->frame0.bmp_cx.height * 768)/600); break;
            case TID_TL_P : Read_BMP(&p->frame1.bmp_tl); zoom_BMP(&p->frame1.bmp_tl, (p->frame1.bmp_tl.width * 1024)/800, (p->frame1.bmp_tl.height * 768)/600); break;
            case TID_TR_P : Read_BMP(&p->frame1.bmp_tr); zoom_BMP(&p->frame1.bmp_tr, (p->frame1.bmp_tr.width * 1024)/800, (p->frame1.bmp_tr.height * 768)/600); break;
            case TID_BR_P : Read_BMP(&p->frame1.bmp_br); zoom_BMP(&p->frame1.bmp_br, (p->frame1.bmp_br.width * 1024)/800, (p->frame1.bmp_br.height * 768)/600); break;
            case TID_BL_P : Read_BMP(&p->frame1.bmp_bl); zoom_BMP(&p->frame1.bmp_bl, (p->frame1.bmp_bl.width * 1024)/800, (p->frame1.bmp_bl.height * 768)/600); break;
            case TID_L_P  : Read_BMP(&p->frame1.bmp_l);  zoom_BMP(&p->frame1.bmp_l,  (p->frame1.bmp_l.width * 1024)/800,  (p->frame1.bmp_l.height * 768)/600); break;
            case TID_R_P  : Read_BMP(&p->frame1.bmp_r);  zoom_BMP(&p->frame1.bmp_r,  (p->frame1.bmp_r.width * 1024)/800,  (p->frame1.bmp_r.height * 768)/600); break;
            case TID_B_P  : Read_BMP(&p->frame1.bmp_b);  zoom_BMP(&p->frame1.bmp_b,  (p->frame1.bmp_b.width * 1024)/800,  (p->frame1.bmp_b.height * 768)/600); break;
            case TID_T_P  : Read_BMP(&p->frame1.bmp_t);  zoom_BMP(&p->frame1.bmp_t,  (p->frame1.bmp_t.width * 1024)/800,  (p->frame1.bmp_t.height * 768)/600); break;
            case TID_C_P  : Read_BMP(&p->frame1.bmp_c);  zoom_BMP(&p->frame1.bmp_c,  (p->frame1.bmp_c.width * 1024)/800,  (p->frame1.bmp_c.height * 768)/600); break;
            case TID_CX_P : Read_BMP(&p->frame1.bmp_cx); zoom_BMP(&p->frame1.bmp_cx, (p->frame1.bmp_cx.width * 1024)/800, (p->frame1.bmp_cx.height * 768)/600); break;
            default       : init_BMP_Makro(&dummy);
                            Read_BMP(&dummy);
                            free_BMP(&dummy);            break;
          }
*/
        }
      }
    }

/*
if(p->id == 32)
{
            save_bmtxt(&p->frame0.bmp_tl, "ram@E:\\tl.txt");
            save_bmtxt(&p->frame0.bmp_tr, "ram@E:\\tr.txt");
            save_bmtxt(&p->frame0.bmp_br, "ram@E:\\br.txt");
            save_bmtxt(&p->frame0.bmp_bl, "ram@E:\\bl.txt");
            save_bmtxt(&p->frame0.bmp_l,  "ram@E:\\ll.txt");
            save_bmtxt(&p->frame0.bmp_r,  "ram@E:\\rr.txt");
            save_bmtxt(&p->frame0.bmp_b,  "ram@E:\\bb.txt");
            save_bmtxt(&p->frame0.bmp_t,  "ram@E:\\tt.txt");
}
*/

    if(GetError() == _TRUE)
    {
      free_THEME(p);
      retcode = _FALSE;
    }
  }

  return(retcode);
}

_BOOL _File::Read_MEM_THEME(_MEM_THEME *p)
{
  _BOOL   retcode = _FALSE;
  _UWORD  tmp, i;
  _UDWORD size;

  init_MEM_THEME(p);

  if(GetError() == _FALSE)
  {
    retcode = _TRUE;
    if(Read(&tmp, sizeof(_UWORD)) == _TRUE)
    {
      size = tmp * sizeof(_THEME);
      if(FileMemory((void**)&p->ptr, size) != 0)
      {
        p->no = tmp;
        for(i=0; i<tmp; i++)
          Read_THEME(&p->ptr[i]);
      }
    }

    if(GetError() == _TRUE)
    {
      retcode = _FALSE;
      free_MEM_THEME(p);
    }
  }

  return(retcode);
}
#endif

/*
_BOOL _File::Read_SINGLEALARM(_SINGLEALARM *p, _UWORD version)
{
  _BOOL  retcode = _TRUE;
  _UWORD i, tmp;
  _UWORD data[2];

  init_SINGLEALARM(p);

  if(GetError() == _FALSE)
  {
    Read(&p->no, sizeof(_UWORD));
    Read(&p->state, sizeof(_UBYTE));
    tmp = 0;
    if(Read(&tmp, sizeof(_UBYTE)) == _TRUE)
    {
      if(tmp <= 5)
      {
        p->no_para = tmp;
        for(i=0; i<tmp; i++)
          Read(&p->para[i], sizeof(_IVALUE));
      }
    }

    Read(&p->time_on, sizeof(_IVALUE));
    Read(&p->date_on, sizeof(_IVALUE));
    Read(&p->time_off, sizeof(_IVALUE));
    Read(&p->date_off, sizeof(_IVALUE));
    Read(&p->acknowledge, sizeof(_UBYTE));
    Read(&p->cnt, sizeof(_UWORD));

    if(version > 1)
    {
      Read_ANYTHING(&p->anything);
    }
    else
    {
      // _ANYTHING.no war vor compilerversion 32 (historyversion < 2) nur 2 byte
      if(Read(data, 4) == _TRUE)
      {
        p->anything.list = (_ANYLST)data[0];
        p->anything.no   = data[1];
      }
    }

    if(version == 1)
    {
      if((_UWORD)p->anything.list == 14)
        p->anything.list = ALARMLIST;  // weil eintry ALARMLIST in enum verschoben wurde
    }

    if(Read(&tmp, sizeof(_UWORD)) == _TRUE)
    {
      FileMemory((void**)&p->ptr, tmp);
      if(Read(p->ptr, tmp) == _TRUE)
        p->ptrsize = tmp;
    }

    Read(&p->entrytype, sizeof(_UWORD));
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    free_SINGLEALARM(p);
  }

  return(retcode);
}

_BOOL _File::Write_SINGLEALARM(_SINGLEALARM *p)
{
  _UWORD i;

  if(GetError() == _FALSE)
  {
    Write(&p->no, sizeof(_UWORD));
    Write(&p->state, sizeof(_UBYTE));
    Write(&p->no_para, sizeof(_UBYTE));
    for(i=0; i<p->no_para; i++)
      Write(&p->para[i], sizeof(_IVALUE));

    Write(&p->time_on, sizeof(_IVALUE));
    Write(&p->date_on, sizeof(_IVALUE));
    Write(&p->time_off, sizeof(_IVALUE));
    Write(&p->date_off, sizeof(_IVALUE));
    Write(&p->acknowledge, sizeof(_UBYTE));
    Write(&p->cnt, sizeof(_UWORD));
    Write_ANYTHING(&p->anything);

    Write(&p->ptrsize, sizeof(_UWORD));
    Write(p->ptr, p->ptrsize);
    Write(&p->entrytype, sizeof(_UWORD));
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE);
}

_BOOL _File::Write_MEM_SINGLEALARM(_MEM_SINGLEALARM *p)
{
  _UDWORD tmp, i;

  if(GetError() == _FALSE)
  {
    Write(&p->sortalgorithm, sizeof(_UWORD));
    tmp = no_RINGBUFFER(&p->ringbuffer);
    Write(&tmp, sizeof(_UDWORD));
    for(i=0; i<tmp; i++)
      Write_SINGLEALARM(&p->ptr[rpos_RINGBUFFER(&p->ringbuffer, i)]);
  }

  return((GetError() == _TRUE)? _FALSE : _TRUE);
}

_BOOL _File::Read_MEM_SINGLEALARM(_MEM_SINGLEALARM *p, _UWORD version)
{
  _BOOL        retcode = _TRUE;
  _UDWORD      i, tmp;
  _SINGLEALARM *ptr;

  if(GetError() == _FALSE)
  {
    p->ringbuffer.begin = 0;
    p->ringbuffer.pos   = 0;
    p->ringbuffer.used  = _FALSE;
    Read(&p->sortalgorithm, sizeof(_UWORD));
    if(Read(&tmp, sizeof(_UDWORD)) == _TRUE)
    {
      if(p->ringbuffer.no > 0)
      {
        for(i=0; i<tmp; i++)
        {
          ptr = &p->ptr[wpos_RINGBUFFER(&p->ringbuffer)];
          free_SINGLEALARM(ptr);
          Read_SINGLEALARM(ptr, version);
        }
      }
    }
  }

  if(GetError() == _TRUE)
  {
    retcode = _FALSE;
    p->ringbuffer.begin = 0;
    p->ringbuffer.pos   = 0;
    p->ringbuffer.used  = _FALSE;
  }
  else if((p->filter & BIT_REMANENT) == 0)
  {
    p->ringbuffer.begin = 0;
    p->ringbuffer.pos   = 0;
    p->ringbuffer.used  = _FALSE;
  }

  return(retcode);
}
*/
_BOOL _File::Read_LINKVL(_LINKVL *p)
{
  _BOOL retcode;

  init_LINKVL(p);

  Read(&p->info, sizeof(p->info));
  retcode = Read_VARLABEL(&p->link);

  return(retcode);
}

_BOOL  _File::Read_RINGBUFFER(_RINGBUFFER *p)
{
  _BOOL   retcode;
  _UDWORD tmp;

  Read(&tmp,      sizeof(_UDWORD));
  Read(&p->no,    sizeof(_UDWORD));
  Read(&p->begin, sizeof(_UDWORD));
  retcode = Read(&p->pos,   sizeof(_UDWORD));

  if(retcode == _TRUE)
    p->used = (tmp == 1)? _TRUE : _FALSE;
  else
    init_RINGBUFFER(p);

  return(retcode);
}

_BOOL  _File::Write_RINGBUFFER(_RINGBUFFER *p)
{
  _UDWORD tmp = (p->used == _TRUE)? 1 : 0;

  Write(&tmp,           sizeof(_UDWORD));
  Write(&p->no,         sizeof(_UDWORD));
  Write(&p->begin,      sizeof(_UDWORD));
  return(Write(&p->pos, sizeof(_UDWORD)));
}

_BOOL _File::SizeRamFile(_UDWORD size)
{
  _BOOL retcode = _FALSE;

  if(IsRamFile() == _TRUE)
  {
    if(size > RamSize)
    {
      PushMemColor(MEMORY_PRG);
      if(Memory2((void**)&RamData, size) != 0)
      {
        RamSize = size;
        retcode = _TRUE;
      }
      PopMemColor();
    }
    else
    {
      retcode = _TRUE;
    }
  }
  return(retcode);
}

_BOOL _File::Write_RINGFILEHEADER(_UDWORD no, const _ASCII *fileid, _UDWORD fileversion, _UDWORD userdata)
{
  // 0..63 ......... header
  // 64,65,66,67 ... anzahl der max einträge
  // 68,69,70,71 ... grösse einzelner eintrag in byte
  // 72,73,74,75 ... ursprünglicher indexbeginn
  // 76,77,78,79 ... userdata

  _UBYTE  data[512];
  _BOOL   retcode = _FALSE;
  _UDWORD size;

  Seek(0, SEEK_SET);
  if(Write_HEADER(fileid, fileversion) == _TRUE)
  {
    size = 512 - GetHeaderSize();
    MemSet(data, 0xFF, size);
    *(_UDWORD*)&data[0]  = no;
    *(_UDWORD*)&data[4]  = 512;
    *(_UDWORD*)&data[8]  = 0;
    *(_UDWORD*)&data[12] = userdata;

    retcode = Write(data, size);
  }

  return(retcode);
}

_BOOL _File::Create_RINGFILE(_ASCII *dpne, _UDWORD no, const _ASCII *fileid, _UDWORD fileversion, _UDWORD userdata)
{
  // 0..63 ......... header
  // 64,65,66,67 ... anzahl der max einträge
  // 68,69,70,71 ... grösse einzelner eintrag in byte
  // 72,73,74,75 ... ursprünglicher indexbeginn
  // 76,77,78,79 ... userdata

  _UBYTE  data[512];
  _BOOL   retcode = _FALSE;
  _UDWORD i;

  if(Open(SAVE, dpne, (no+1)*512) == _TRUE)
  {
    //SizeRamFile();

    if(Write_RINGFILEHEADER(no, fileid, fileversion, userdata) == _TRUE)
    {
      MemSet(data, 0xFF, 512);
      for(i=0; i<no; i++)
        Write(data, 512);

      if(GetError() == _FALSE)
        retcode = _TRUE;
    }

    Close();
  }

  return(retcode);
}
/*
_BOOL _File::Create_RINGFILE(_ASCII *dpne, _UDWORD no, _ASCII *fileid, _UDWORD fileversion, _UDWORD userdata)
{
  // 0..63 ......... header
  // 64,65,66,67 ... anzahl der max einträge
  // 68,69,70,71 ... grösse einzelner eintrag in byte
  // 72,73,74,75 ... ursprünglicher indexbeginn
  // 76,77,78,79 ... userdata

  _UBYTE  data[512];
  _BOOL   retcode = _FALSE;
  _UDWORD size, i;

  if(Open(SAVE, dpne) == _TRUE)
  {
    SizeRamFile((no+1)*512);

    if(Write_HEADER(fileid, fileversion) == _TRUE)
    {
      size = 512 - GetHeaderSize();
      MemSet(data, 0xFF, size);
      *(_UDWORD*)&data[0]  = no;
      *(_UDWORD*)&data[4]  = 512;
      *(_UDWORD*)&data[8]  = 0;
      *(_UDWORD*)&data[12] = userdata;

      Write(data, size);

      MemSet(data, 0xFF, 512);
      for(i=0; i<no; i++)
        Write(data, 512);

      if(GetError() == _FALSE)
        retcode = _TRUE;
    }

    Close();
  }

  return(retcode);
}
*/
// interface  *********************

_BOOL  Lse_File_GetError(_File* handl)
{
	return(handl->GetError());
}

void   Lse_File_SetError(_File* handl, _FILE_ERROR ferr)
{
	handl->SetError(ferr);
}

_BOOL  Lse_File_Open(_File* handl, _FILE_STATE state, _ASCII* dpne)
{
	return(handl->Open(state, dpne));
}

_BOOL  Lse_File_Close(_File* handl)
{
	return(handl->Close(_FALSE));
}

_BOOL Lse_File_Remove(_File* handl, _ASCII *dpne)
{
  return(handl->Remove(dpne));
}

_DWORD Lse_File_Tell(_File* handl)
{
  return(handl->Tell());
}

_BOOL  Lse_File_Seek(_File* handl, _DWORD offs, _UWORD fromwhere)
{
  return(handl->Seek(offs, fromwhere));
}

_DWORD Lse_File_Length(_File* handl)
{
  return(handl->Length());
}

_BOOL  Lse_File_Read(_File* handl, void* dest, _UDWORD size)
{
  return(handl->Read(dest, size));
}

_BOOL  Lse_File_Write(_File* handl, void* dest, _UDWORD size)
{
  return(handl->Write(dest, size));
}

_BOOL Lse_File_Decode(_File *handl, _UDWORD headersize)
{
  return(handl->Decode(headersize));
}

_BOOL Lse_File_Encode(_File *handl, _UDWORD headersize)
{
  return(handl->Encode(headersize));
}

_BOOL Lse_File_Read_CRLF(_File* handl, _ASCII *dst, _UWORD maxlen)
{
  return(handl->Read_CRLF(dst, maxlen));
}

_BOOL Lse_File_ReadCrLf(_File* handl, void *dst, _UWORD maxlen, _UWORD charsize)
{
  return(handl->ReadCrLf(dst, maxlen, charsize));
}



_FILE_ERROR Lse_File_GetErrorState(_File *handl)
{
  return(handl->GetErrorState());
}
