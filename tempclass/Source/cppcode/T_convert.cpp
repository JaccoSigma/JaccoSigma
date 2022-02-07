#include "T_IPC.H"
#include "T_NUC.H"

_REAL get_pREAL(_REAL *psrc)
{
  _REAL rval;
  *(_UDWORD*)&rval = *(_UDWORD*)psrc;
  return rval;
}

void move_pREAL_REAL(_REAL *pdst, _REAL rval)
{
  *(_UDWORD*)pdst = *(_UDWORD*)&rval;
}

void move_pREAL_pREAL(_REAL *pdst, _REAL *psrc)
{
  _REAL rval;
  *(_UDWORD*)&rval = *(_UDWORD*)psrc;
  *(_UDWORD*)pdst  = *(_UDWORD*)&rval;
}

void move_pREAL_DWORD(_REAL *pdst, _DWORD dval)
{
  _REAL  rval = dval;
  *(_UDWORD*)pdst = *(_UDWORD*)&rval;
}
