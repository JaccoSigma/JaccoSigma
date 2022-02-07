#include "T_IPC.H"
#include "T_NUC.H"

void _ServerWatch::Init(void)
{
  Ptr   = NULL;
  No    = 0;
  MaxNo = 0;
  AktNo = 0;
  init_VARLABEL(&VarLabel);
  VarLabel.no            = 1;
  VarLabel.info[0].state = VAR_VAL;
}

void _ServerWatch::Free(void)
{
  PushMemColor(MEMORY_PRG);
  Memory2((void**)&Ptr, 0);
  free_VARLABEL(&VarLabel);
  PopMemColor();
  Init();
}

_BOOL _ServerWatch::Add(_UDWORD svrno, void *ptrthis, void *ptrfkt)
{
  _UDWORD   size;
  _UDWORD   i;
  _BOOL     retcode = _TRUE;
  _RESULT   result;
  _VARIABLE variable;
  _SVRWATCH *pt;

  if(Seek(svrno) >= No)
  {
    retcode = _FALSE;

    if(ptrfkt != NULL)
    {
      if(MaxNo <= No)
      {
        PushMemColor(MEMORY_PRG);
        size  = (No + 100) * sizeof(_SVRWATCH);
        if(Memory2((void**)&Ptr, size) != 0)
        {
          MaxNo = No + 100;
          pt    = &Ptr[No];
          for(i=No; i<MaxNo; i++, pt++)
            init_SVRWATCH(pt);
        }
        PopMemColor();
      }

      if(MaxNo > No)
      {
        VarLabel.info[0].value = svrno;

        if(VarList.GetSystemData(&result, &variable, &VarLabel, _TRUE, _FALSE) == _TRUE)
        {
          if((result.ftype == RES_REAL)||(result.ftype == RES_NUMERIC)||(result.ftype == RES_CONSTANT))
          {
            retcode   = _TRUE;
            pt        = &Ptr[No];
            No       += 1;

            free_SVRWATCH(pt);

            pt->svrno    = svrno;
            pt->oldvalue = result.value;
            pt->ptrthis  = ptrthis;
            pt->ptrfkt   = ptrfkt;
          }
        }
      }
    }
  }

  return(retcode);
}

_UDWORD _ServerWatch::Seek(_UDWORD svrno)
{
  _UDWORD   i;
  _SVRWATCH *pt;

  pt = Ptr;
  for(i=0; i<No; i++, pt++)
  {
    if(pt->svrno == svrno)
      return(i);
  }

  return(DEFSCOPE);
}

_BOOL _ServerWatch::Del(_UDWORD svrno)
{
  _UDWORD index;
  _BOOL   retcode = _FALSE;

  index = Seek(svrno);
  if(index < No)
  {
    retcode = _TRUE;
    MemMove(&Ptr[index], &Ptr[index+1], (No-index-1)*sizeof(_SVRWATCH));
    No --;
    init_SVRWATCH(&Ptr[No]);
  }

  return(retcode);
}

void _ServerWatch::Run(void)
{
  _RESULT                   result;
  _VARIABLE                 variable;
  _SVRWATCH                 *pt;
  void                      *pthis;
  _FVOID_UDWORD_DWORD_DWORD pfkt;
  _DWORD                    alt, neu;
  _UDWORD                   sno;

  if(No > 0)
  {
    if(AktNo >= No)
      AktNo = 0;

    pt = &Ptr[AktNo];
    VarLabel.info[0].value = pt->svrno;

    if(VarList.GetSystemData(&result, &variable, &VarLabel, _FALSE, _FALSE) == _TRUE)
    {
      if(result.value != pt->oldvalue)
      {
	      pfkt  = (_FVOID_UDWORD_DWORD_DWORD)pt->ptrfkt;
        pthis = pt->ptrthis;
        sno   = pt->svrno;
        alt   = pt->oldvalue;
        neu   = result.value;

				if(pfkt != NULL)
				{
				  load_this(pthis);
				  pfkt(sno, neu, alt);
				}

        pt->oldvalue = result.value;
      }
    }

    AktNo ++;
  }
}

// **************************************************************************
// st - interface ***********************************************************
// **************************************************************************

_BOOL ServerWatch_Add(_UDWORD svrno, void *ptrthis, void *ptrfkt)
{
  return(ServerWatch.Add(svrno, ptrthis, ptrfkt));
}

_BOOL ServerWatch_Del(_UDWORD svrno)
{
  return(ServerWatch.Del(svrno));
}

