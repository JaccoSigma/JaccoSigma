#include "T_IPC.H"
#include "T_NUC.H"  

/*

void _IpcMemory::Init(void)
{
  init_IPCMEMORY(&Ipcmemory); 
}

void _IpcMemory::Free(void)
{
  if(System != NULL)
    free_SYSTEM(System); 
  free_IPCMEMORY(&Ipcmemory); 
  Init(); 
}

_BOOL _IpcMemory::Alloc(void)
{
  _BOOL retcode = _FALSE; 

  if(Memory2((void**)&Ipcmemory.ptr, UC_IPCMEMORYSIZE) != 0)
  {
    retcode         = _TRUE; 
    Ipcmemory.state = _TRUE; 
    Ipcmemory.size  = UC_IPCMEMORYSIZE; 
  }

  return(retcode);  
}

_BOOL _IpcMemory::Set(_UDWORD adress, void *src, _UWORD size)
{
  _BOOL retcode = _FALSE;

  if((Ipcmemory.state == _TRUE) && ((adress + size) < Ipcmemory.size))
  {
    MemMove(&Ipcmemory.ptr[adress], src, size); 
    retcode          = _TRUE;
    Ipcmemory.change = _TRUE; 
  }

  return(retcode); 
}

void *_IpcMemory::GetPointer(_UDWORD offset)
{
  return((void*)&Ipcmemory.ptr[offset]); 
}

_BOOL _IpcMemory::Get(void **dest, _UDWORD adress, _UWORD size)
{
  _BOOL retcode; 

  if((Ipcmemory.state == _TRUE) && ((adress + size) < Ipcmemory.size))
  {
    *dest   = (void*)&Ipcmemory.ptr[adress]; 
    retcode = _TRUE; 
  }
  else
  {
    *dest   = NULL; 
    retcode = _FALSE; 
  }

  return(retcode);
}

#ifdef PGTEST
void _IpcMemory::Counter(_UWORD adress, _UWORD cnt)
{
  if(*(_UWORD*)&Ipcmemory.ptr[adress] < cnt) 
    *(_UWORD*)&Ipcmemory.ptr[adress] = (*(_UWORD*)&Ipcmemory.ptr[adress]) + 1; 
  else
    *(_UWORD*)&Ipcmemory.ptr[adress] = 0; 
}

void _IpcMemory::Inc(_UWORD i)                             
{ 
  *(_UWORD*)&Ipcmemory.ptr[i*2] = *(_UWORD*)&Ipcmemory.ptr[i*2] + 1;  
}

void _IpcMemory::Dec(_UWORD i)                             
{ 
  *(_UWORD*)&Ipcmemory.ptr[i*2] = *(_UWORD*)&Ipcmemory.ptr[i*2] - 1;  
}
#endif

_BOOL _IpcMemory::Load(void)
{
 #ifdef UC_IPC
  _BOOL   retcode = _FALSE; 
  _File   File; 
  _UWORD  sb; 
  _UDWORD dword; 

  if(Ipcmemory.state == _TRUE)
  {
    sb = ScanBar.Add("Load Data:", 0); 
    if(File.Open(LOAD, Batch.GetDataDPNE()) == _TRUE)
    {
      if(File.Read(&dword, sizeof(_UDWORD)) == _TRUE)  // version
      {
        File.Read(&dword, sizeof(_UDWORD));  // size
        if(dword > Ipcmemory.size)
          dword = Ipcmemory.size; 
        File.Read(Ipcmemory.ptr, dword); 
      }
    }

    retcode = ((File.GetError() == _TRUE)? _FALSE : _TRUE); 

    File.Close();
    ScanBar.End(sb); 
  }

  if(retcode == _FALSE) 
    Ipcmemory.urinit = _TRUE; 

  return(retcode); 
 #else
  return(_TRUE); 
 #endif
}

_BOOL _IpcMemory::Save(void)
{
 #ifdef UC_IPC
  _BOOL   retcode = _FALSE; 
  _File   File; 
  _UDWORD dword; 

  // [][][][] ..... version
  // [][][][] ..... sizeof data
  // [ ............ data
  // [ ............ alarm

  if(Ipcmemory.state == _TRUE)
  {
    if(File.Open(SAVE, Batch.GetDataDPNE()) == _TRUE)
    {
      dword = 625; 
      File.Write(&dword, sizeof(_DWORD)); 
      dword = Ipcmemory.size; 
      File.Write(&dword, sizeof(_DWORD)); 
      File.Write(Ipcmemory.ptr, Ipcmemory.size); 

      if(File.GetError() == _FALSE)
        retcode = _TRUE;
    }
    File.Close(); 
  }

  return(retcode); 
 #else
  return(_TRUE); 
 #endif
}

_UWORD _IpcMemory::Add(void **pptr, _ASCII *name, _UDWORD size)
{
  _UWORD retcode = 3; 
  _UBYTE *ptr; 

  if((Ipcmemory.used + size) < Ipcmemory.size)
  {
    *pptr   = &Ipcmemory.ptr[Ipcmemory.used]; 
    ptr     = &Ipcmemory.ptr[Ipcmemory.used + size]; 
    retcode = 0; 

    if(*(_UDWORD*)ptr != size) 
    {
      MemSet(*pptr, 0, size);                          // init memory with 0
      *(_UDWORD*)ptr   = size; 
      Ipcmemory.urinit = _TRUE; 
      retcode = 1; 
    }

    if(VarList.InsertLocalAdress(name, Ipcmemory.used) == _FALSE) // init adress 
      retcode = 2; 

    Ipcmemory.used += (size + sizeof(_UDWORD)); 
  }
  else
  {
    *pptr = NULL; 
  }

  return(retcode); 
}

void _IpcMemory::IfNecDoUrinit(void)
{
  if(Ipcmemory.urinit == _TRUE)
  {
    init_SYSTEM(System); 
    IpcUrInit(); 
    Ipcmemory.urinit = _FALSE; 
  }

  IpcInit();
}

*/

void _ChkBit::Init(void)
{
  _UWORD i; 

  Bit[0] = 1;
   
  for(i=1; i<CHKBIT_BYTESIZE; i++)
    Bit[i] = 0;  
}

void _ChkBit::Free(void)
{
  Init(); 
}

_BOOL _ChkBit::Get(_UWORD no)
{
  _DWORD byteno; 
  _UBYTE bitno; 
  _BOOL  retcode = _TRUE; 

  if(no != 0)
  {
    retcode = _FALSE; 
    if(no < UC_CHKBIT_NO)
    {
      byteno  = no / 8; 
      if(byteno < CHKBIT_BYTESIZE)
      {
        bitno   = (_UBYTE)(no % 8);
        retcode = ((Bit[byteno] >> bitno) & 1)? _TRUE : _FALSE; 
      }
    }
  }

  return(retcode); 
}

_BOOL _ChkBit::Set(_UWORD no, _BOOL st)
{
  _DWORD byteno; 
  _UBYTE bitno, byte; 
  _BOOL  retcode = _FALSE;

  if((no != 0)&&(no < UC_CHKBIT_NO))
  {
    byteno = no / 8; 
    if(byteno < CHKBIT_BYTESIZE)
    {
      bitno = (_UBYTE)(no % 8);
      byte  = Bit[byteno]; 

      if(st == _TRUE) 
      {
        Bit[byteno] = byte | (_UBYTE)(1 << bitno); 
      }
      else
      {
        Bit[byteno] = byte & (_UBYTE)(0xFF ^ (_UBYTE)(1 << bitno)); 
      }
    }
    else
    {
      retcode = _FALSE; 
    }
  }

  return(retcode); 
}

