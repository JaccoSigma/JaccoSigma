//<NewSigmatekCFileOptimize/>
// +-------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                          |
// +-[      author ] kolott                                                        |
// +-[        date ] 08.07.2016                                                    |
// +-[ description ]---------------------------------------------------------------+
// |                                                                               |
// |                                                                               |
// +-------------------------------------------------------------------------------+

#include "SigCLib.h"

#ifdef _LSL_TARGETARCH_X86

inline long sigclib_atomic_cmpxchg(volatile long *mem, long cmpVal, long newVal)
{
 #ifdef _MSC_VER
    __asm{
      mov eax, cmpVal
      mov ebx, newVal
      mov edi, mem
      lock cmpxchg [edi], ebx
    }
 #else
  long retVal;
	asm volatile( 
    "lock cmpxchgl %2, %1"
    : "=a" (retVal), "+m" (*mem)
    : "r" (newVal), "0" (cmpVal)
    : "memory"
  ); 
  return retVal;
 #endif
}

inline long sigclib_atomic_fetch_add(volatile long *mem, long addVal)
{
 #ifdef _MSC_VER
    __asm{
      mov edi, mem
      mov eax, addVal
      lock xadd [edi], eax
    }
 #else
  asm volatile(
    "lock xadd %0, (%1);"
    : "=a"(addVal)
    : "r"(mem), "a"(addVal)
    : "memory"
  );
  return addVal;
 #endif
}

unsigned long sigclib_atomic_incU32(unsigned long *pValue)
{
  return sigclib_atomic_fetch_add((long*)pValue, 1);
}

unsigned long sigclib_atomic_decU32(unsigned long *pValue)
{
  return sigclib_atomic_fetch_add((long*)pValue, -1);
}

unsigned long sigclib_atomic_addU32(unsigned long *pValue, unsigned long addVal)
{
  return sigclib_atomic_fetch_add((long*)pValue, addVal);
}

unsigned long sigclib_atomic_subU32(unsigned long *pValue, unsigned long subVal)
{
  return sigclib_atomic_fetch_add((long*)pValue, -subVal);
}
 
unsigned long sigclib_atomic_cmpxchgU32(unsigned long *pValue, unsigned long cmpVal, unsigned long newVal)
{
  return (unsigned long)sigclib_atomic_cmpxchg((long*)pValue, (long)cmpVal, (long)newVal);
}
  
long sigclib_atomic_incS32(long *pValue)
{
  return sigclib_atomic_fetch_add(pValue, 1);
}

long sigclib_atomic_decS32(long *pValue)
{
  return sigclib_atomic_fetch_add(pValue, -1);
}

long sigclib_atomic_addS32(long *pValue, long addVal)
{
  return sigclib_atomic_fetch_add(pValue, addVal);
}

long sigclib_atomic_subS32(long *pValue, long subVal)
{
  return sigclib_atomic_fetch_add(pValue, -subVal);
}

long sigclib_atomic_cmpxchgS32(long *pValue, long cmpVal, long newVal)
{
  return sigclib_atomic_cmpxchg(pValue, cmpVal, newVal);
}

#endif

void *sigclib_atomic_cmpxchgPtr(void **pPointer, void *cmpPtr, void *newPtr)
{
  return (void*)sigclib_atomic_cmpxchgU32((unsigned long*)pPointer, (unsigned long)cmpPtr, (unsigned long)newPtr);
}

#define __CMPX_SPINLOCK_S32(_ALU) { do{oldval=sigclib_atomic_getS32(pValue);newval=(_ALU);res=sigclib_atomic_cmpxchgS32(pValue,oldval,newval);}while(res!=oldval); }
#define __CMPX_SPINLOCK_U32(_ALU) { do{oldval=sigclib_atomic_getU32(pValue);newval=(_ALU);res=sigclib_atomic_cmpxchgU32(pValue,oldval,newval);}while(res!=oldval); }

unsigned long sigclib_atomic_andU32(unsigned long *pValue, unsigned long bitVal)
{
  unsigned long oldval, newval, res; // note: used for spinlock !
  __CMPX_SPINLOCK_U32(oldval & bitVal);
  return oldval;
}

long sigclib_atomic_andS32(long *pValue, long bitVal)
{
  long oldval, newval, res; // note: used for spinlock !
  __CMPX_SPINLOCK_S32(oldval & bitVal);
  return oldval;
}

unsigned long sigclib_atomic_nandU32(unsigned long *pValue, unsigned long bitVal)
{
  unsigned long oldval, newval, res; // note: used for spinlock !
  __CMPX_SPINLOCK_U32(~(oldval & bitVal));
  return oldval;
}

long sigclib_atomic_nandS32(long *pValue, long bitVal)
{
  long oldval, newval, res; // note: used for spinlock !
  __CMPX_SPINLOCK_S32(~(oldval & bitVal));
  return oldval;
}

unsigned long sigclib_atomic_orU32(unsigned long *pValue, unsigned long bitVal)
{
  unsigned long oldval, newval, res; // note: used for spinlock !
  __CMPX_SPINLOCK_U32(oldval | bitVal);
  return oldval;
}

long sigclib_atomic_orS32(long *pValue, long bitVal)
{
  long oldval, newval, res; // note: used for spinlock !
  __CMPX_SPINLOCK_S32(oldval | bitVal);
  return oldval;
}

unsigned long sigclib_atomic_xorU32(unsigned long *pValue, unsigned long bitVal)
{
  unsigned long oldval, newval, res; // note: used for spinlock !
  __CMPX_SPINLOCK_U32(oldval ^ bitVal);
  return oldval;
}

long sigclib_atomic_xorS32(long *pValue, long bitVal)
{
  long oldval, newval, res; // note: used for spinlock !
  __CMPX_SPINLOCK_S32(oldval ^ bitVal);
  return oldval;
}
