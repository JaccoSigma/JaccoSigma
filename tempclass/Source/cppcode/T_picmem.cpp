#include "T_IPC.H"
#include "T_NUC.H"  

#ifdef OK_PICMEMO

void _MemoPool::Init(void)
{ 
  init_MEM_GLOBMEMO(&Memo); 
}

void _MemoPool::Free(void)
{ 
  free_MEM_GLOBMEMO(&Memo); 
}

_BOOL _MemoPool::Add(_WHOAMI *pw)
{
  _BOOL   retcode = _FALSE; 
  _UDWORD size; 

  size = (Memo.no+1) * sizeof(_GLOBMEMO); 

  if(Memory2((void**)&Memo.ptr, size) != 0)
  {
    init_GLOBMEMO(&Memo.ptr[Memo.no]); 
    Memo.ptr[Memo.no].whoami = *pw; 
    Memo.no ++; 
    retcode = _TRUE; 
  }

  return(retcode); 
}

_BOOL _MemoPool::Seek(_GLOBMEMO **ppg, _WHOAMI *pw)
{
  _BOOL     retcode = _FALSE; 
  _UWORD    i; 
  _GLOBMEMO *p; 

  for(i=0; i<Memo.no; i++)
  {
    p = &Memo.ptr[i]; 
    if((p->whoami.ima == pw->ima) && (p->whoami.no == pw->no))
    {
      *ppg = p; 
      retcode = _TRUE; 
      break; 
    }
  }

  return(retcode); 
}

_BOOL _MemoPool::SetLastInput(_WHOAMI *pw, _UWORD no)
{
  _BOOL     retcode = _FALSE;
  _GLOBMEMO *pg = NULL;

  if(Seek(&pg, pw) == _TRUE)
  {
    retcode = _TRUE;
  }
  else if(Add(pw) == _TRUE)
  {
    retcode = Seek(&pg, pw);
  }

  if(retcode == _TRUE)
  {
    pg->lastinput.no    = no;
  }

  return(retcode);
}

_BOOL _MemoPool::GetLastInput(_WHOAMI *pw, _UWORD *no)
{
  _BOOL     retcode = _FALSE;
  _GLOBMEMO *pg = NULL;

  if(Seek(&pg, pw) == _TRUE)
  {
    if(pg->lastinput.no < 0xFFFF)
    {
      *no     = pg->lastinput.no;
      retcode = _TRUE;
    }
  }

  return(retcode);
}
#endif


