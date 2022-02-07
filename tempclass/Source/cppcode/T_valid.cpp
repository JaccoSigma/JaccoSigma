#include "T_IPC.H"
#include "T_NUC.H"  

#ifdef OK_VALIDATE

void _Validate::Init(void)
{
  init_MEM_VALIDATA(&MemData); 
}

void _Validate::Free(void)
{
  free_MEM_VALIDATA(&MemData); 
}

_BOOL _Validate::AddIntern(_UWORD *pno, _VARLABEL *pvl, _UDWORD datsize)
{
  _BOOL     retcode = _FALSE; 
  _UDWORD   size; 
  _VALIDATA *p = NULL; 

  Seek(pno, pvl); 

  if(*pno >= MemData.no)
  {
    size = (MemData.no + 1) * sizeof(_VALIDATA); 

    if(Memory2((void**)&MemData.ptr, size) != 0)
    {
      p = &MemData.ptr[MemData.no]; 
      MemData.no ++; 
      init_VALIDATA(p); 
      retcode = _TRUE; 
    }
  }
  else
  {
    p = &MemData.ptr[*pno]; 
    retcode = _TRUE; 
  }

  if(retcode == _TRUE)
  {
    retcode = _FALSE; 
    if(Memory2((void**)&p->ptr, datsize) != 0)
    {
      MemSet(p->ptr, 0, datsize); 
      p->vl   = *pvl; 
      retcode = _TRUE; 
    }
    else
    {
      free_VALIDATA(p); 
    }
  }

  return(retcode); 
}

_BOOL _Validate::Seek(_UWORD *no, _VARLABEL *pvl)
{
  _UWORD i; 

  for(i=0; i<MemData.no; i++)
  {
    if(cmp_VARLABEL(&MemData.ptr[i].vl, pvl) == 0)
    {
      *no = i; 
      return(_TRUE); 
    }
  }

  *no = MemData.no; 

  return(_FALSE); 
}

_BOOL _Validate::HasChanged(_VARLABEL *pvl)
{
  _UWORD no; 
  return(Seek(&no, pvl)); 
}

_BOOL _Validate::GetPointerToData(void **pptr, _VARLABEL *pvl)
{
  _BOOL  retcode = _FALSE; 
  _UWORD no; 

  if(Seek(&no, pvl) == _TRUE)
  {

    *pptr = MemData.ptr[no].ptr; 
    retcode = _TRUE; 
  }

  return(retcode); 
}

_BOOL _Validate::MakePointerToData(void **pptr, _VARLABEL *pvl, _UDWORD size)
{
  _UWORD no; 
  _BOOL retcode = _FALSE; 

  if(AddIntern(&no, pvl, size) == _TRUE)
  {
    *pptr   = MemData.ptr[no].ptr; 
    retcode = _TRUE; 
  } 

  return(retcode); 
}

void _Validate::ValidateAll(void)
{
  _UWORD    i; 
  _VALIDATA *p; 
  _RESULT   res; 
  _VARIABLE variable; 

  for(i=0; i<MemData.no; i++)
  {
    p = &MemData.ptr[i]; 
    if(VarList.GetSystemData(&res, &variable, &p->vl, _TRUE) == _TRUE)
      VarList.SetSystemData(&res, &variable, &p->vl, _TRUE); 
  }

  DischargeAll(); 
}

void _Validate::DischargeAll(void)
{
  free_MEM_VALIDATA(&MemData); 
}

#endif
