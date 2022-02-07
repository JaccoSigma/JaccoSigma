#include "T_IPC.H"
#include "T_NUC.H"  

void _InterBoxList::Init(void)
{
  LseLabel.Init();
  No  = 0;
  Ptr = NULL;
}

void _InterBoxList::Free(void)
{
  _UDWORD i;

  LseLabel.Free();
  for(i=0; i<No; i++)
    Ptr[i].Free();

  Memory2((void**)&Ptr, 0);

  Init();
}

_BOOL _InterBoxList::Load(_ASCII *dpne)
{
  // [][] ... no of interpreterboxes
  // [....... interpreterinfo

  _File    File;
  _BOOL    retcode = _FALSE;
  _UDWORD  tmp, i;
  _UDWORD  size;

  Free();

  if(File.Open(LOAD, dpne) == _TRUE)
  {
    if(File.ReadHeader() == _TRUE)
    {
      if(File.Read(&tmp, sizeof(_UDWORD)) == _TRUE)
      {
        size = tmp * sizeof(_Stack);
        if(Memory2((void**)&Ptr, size) == _TRUE)
        {
          No = tmp;
          for(i=0, retcode=_TRUE; i<No; i++)
          {
            Ptr[i].Init();
            retcode = Ptr[i].Read(&File);
          }
        }
      }
    }

    if(retcode == _FALSE)
      Free();

   #if COMPILER_VERSION >= 33
    LseLabel.Read(&File, _FALSE);
   #endif

  }
  File.Close();

  return(retcode);
}

_Stack DummyStack;
_Stack* _InterBoxList::Get(_UDWORD nr)
{
  if(nr < No)
    return(&Ptr[nr]);

  DummyStack.Init(); 
  return(&DummyStack); 
}                        

_BOOL _InterBoxList::Chk_LSEFUNCT(_LSEFUNCT *p)
{
  if(p->no < No)
    return(_TRUE);

  if(p->overload.no > 0)
    return(_TRUE);

  return(_FALSE);
}

_BOOL _InterBoxList::ChkFunction(_UDWORD nr)
{
  return((nr < No)? _TRUE : _FALSE);
}

_UWORD _InterBoxList::GetNo(void)
{
  return(No);
}
