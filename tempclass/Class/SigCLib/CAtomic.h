#ifndef _CAtomicH
 #define _CAtomicH
  
  #include "DefineCompiler.h"

  // use atomic functions to change value (S32, U32) in memory by using named operator (inc, dec, add, sub, swp, cmpxchg)
  // retcode of each function is the value right before performing operator to memorylocation 

  #ifdef cCompile
    
    // atomic get u32
    cExtern unsigned long sigclib_atomic_getU32(unsigned long *pValue);
    // atomic set u32
    cExtern void          sigclib_atomic_setU32(unsigned long *pValue, unsigned long value);
    // atomic increment of u32bit, return value corresponds to value before operator was executed
    cExtern unsigned long sigclib_atomic_incU32(unsigned long *pValue);
    // atomic decrement of u32bit, return value corresponds to value before operator was executed
    cExtern unsigned long sigclib_atomic_decU32(unsigned long *pValue);
    // atomic add of u32bit, return value corresponds to value before operator was executed
    cExtern unsigned long sigclib_atomic_addU32(unsigned long *pValue, unsigned long addVal);
    // atomic sub of u32bit, return value corresponds to value before operator was executed
    cExtern unsigned long sigclib_atomic_subU32(unsigned long *pValue, unsigned long addVal);
    // atomic swap of u32bit, return value corresponds to value before operator was executed
    cExtern unsigned long sigclib_atomic_swpU32(unsigned long *pValue, unsigned long swpVal);
    // atomic compareXchange u32bit, return value corresponds to value before operator was executed
    cExtern unsigned long sigclib_atomic_cmpxchgU32(unsigned long *pValue, unsigned long cmpVal, unsigned long newVal);
   
    // atomic get s32bit
    cExtern long sigclib_atomic_getS32(long *pValue);
    // atomic set s32bit
    cExtern void sigclib_atomic_setS32(long *pValue, long value);
    // atomic increment of s32bit, return value corresponds to value before operator was executed
    cExtern long sigclib_atomic_incS32(long *pValue);
    // atomic decrement of s32bit, return value corresponds to value before operator was executed
    cExtern long sigclib_atomic_decS32(long *pValue);
    // atomic add of s32bit, return value corresponds to value before operator was executed
    cExtern long sigclib_atomic_addS32(long *pValue, long addVal);
    // atomic sub of s32bit, return value corresponds to value before operator was executed
    cExtern long sigclib_atomic_subS32(long *pValue, long subVal);
    // atomic swap of s32bit, return value corresponds to value before operator was executed
    cExtern long sigclib_atomic_swpS32(long *pValue, long swpVal);
    // atomic compareXchange s32bit, return value corresponds to value before operator was executed
    cExtern long sigclib_atomic_cmpxchgS32(long *pValue, long cmpVal, long newVal);
    
    // atomic compareXchange pointer, return value corresponds to pointer before operator was executed
    cExtern void *sigclib_atomic_cmpxchgPtr(void **pPointer, void *cmpPtr, void *newPtr);
    
    // atomic AND u32bit, return value corresponds to value before operator was executed
    cExtern unsigned long sigclib_atomic_andU32(unsigned long *pValue, unsigned long bitVal);
    // atomic AND s32bit, return value corresponds to value before operator was executed
    cExtern long sigclib_atomic_andS32(long *pValue, long bitVal);
    // atomic NAND u32bit, return value corresponds to value before operator was executed
    cExtern unsigned long sigclib_atomic_nandU32(unsigned long *pValue, unsigned long bitVal);
    // atomic NAND s32bit, return value corresponds to value before operator was executed
    cExtern long sigclib_atomic_nandS32(long *pValue, long bitVal);
    // atomic OR u32bit, return value corresponds to value before operator was executed
    cExtern unsigned long sigclib_atomic_orU32(unsigned long *pValue, unsigned long bitVal);
    // atomic OR s32bit, return value corresponds to value before operator was executed
    cExtern long sigclib_atomic_orS32(long *pValue, long bitVal);
    // atomic XOR u32bit, return value corresponds to value before operator was executed
    cExtern unsigned long sigclib_atomic_xorU32(unsigned long *pValue, unsigned long bitVal);
    // atomic XOR s32bit, return value corresponds to value before operator was executed
    cExtern long sigclib_atomic_xorS32(long *pValue, long bitVal);

    #define sigclib_atomic_add_and_fetchU32(__p, __v)  (sigclib_atomic_addU32((__p),   ((unsigned long)(__v))) + ((unsigned long)(__v)))
    #define sigclib_atomic_add_and_fetchS32(__p, __v)  (sigclib_atomic_addS32((__p),            ((long)(__v))) +          ((long)(__v)))
    #define sigclib_atomic_sub_and_fetchU32(__p, __v)  (sigclib_atomic_subU32((__p),   ((unsigned long)(__v))) - ((unsigned long)(__v)))
    #define sigclib_atomic_sub_and_fetchS32(__p, __v)  (sigclib_atomic_subS32((__p),            ((long)(__v))) -          ((long)(__v)))
    #define sigclib_atomic_and_and_fetchU32(__p, __v)  (sigclib_atomic_andU32((__p),   ((unsigned long)(__v))) & ((unsigned long)(__v)))
    #define sigclib_atomic_and_and_fetchS32(__p, __v)  (sigclib_atomic_andS32((__p),            ((long)(__v))) &          ((long)(__v)))
    #define sigclib_atomic_or_and_fetchU32(__p, __v)   (sigclib_atomic_orU32((__p),    ((unsigned long)(__v))) | ((unsigned long)(__v)))
    #define sigclib_atomic_or_and_fetchS32(__p, __v)   (sigclib_atomic_orS32((__p),             ((long)(__v))) |          ((long)(__v)))
    #define sigclib_atomic_xor_and_fetchU32(__p, __v)  (sigclib_atomic_xorU32((__p),   ((unsigned long)(__v))) ^ ((unsigned long)(__v)))
    #define sigclib_atomic_xor_and_fetchS32(__p, __v)  (sigclib_atomic_xorS32((__p),            ((long)(__v))) ^          ((long)(__v)))
    #define sigclib_atomic_nand_and_fetchU32(__p, __v) (~(sigclib_atomic_nandU32((__p),((unsigned long)(__v))) & ((unsigned long)(__v))))
    #define sigclib_atomic_nand_and_fetchS32(__p, __v) (~(sigclib_atomic_nandS32((__p),         ((long)(__v))) &          ((long)(__v))))

    #define sigclib_atomic_fetch_and_addU32(__p, __v)  (sigclib_atomic_addU32((__p),  ((unsigned long)(__v))))
    #define sigclib_atomic_fetch_and_addS32(__p, __v)  (sigclib_atomic_addS32((__p),           ((long)(__v))))
    #define sigclib_atomic_fetch_and_subU32(__p, __v)  (sigclib_atomic_subU32((__p),  ((unsigned long)(__v))))
    #define sigclib_atomic_fetch_and_subS32(__p, __v)  (sigclib_atomic_subS32((__p),           ((long)(__v))))
    #define sigclib_atomic_fetch_and_andU32(__p, __v)  (sigclib_atomic_andU32((__p),  ((unsigned long)(__v))))
    #define sigclib_atomic_fetch_and_andS32(__p, __v)  (sigclib_atomic_andS32((__p),           ((long)(__v))))
    #define sigclib_atomic_fetch_and_orU32(__p, __v)   (sigclib_atomic_orU32((__p),   ((unsigned long)(__v))))
    #define sigclib_atomic_fetch_and_orS32(__p, __v)   (sigclib_atomic_orS32((__p),            ((long)(__v))))
    #define sigclib_atomic_fetch_and_xorU32(__p, __v)  (sigclib_atomic_xorU32((__p),  ((unsigned long)(__v))))
    #define sigclib_atomic_fetch_and_xorS32(__p, __v)  (sigclib_atomic_xorS32((__p),           ((long)(__v))))
    #define sigclib_atomic_fetch_and_nandU32(__p, __v) (sigclib_atomic_nandU32((__p), ((unsigned long)(__v))))
    #define sigclib_atomic_fetch_and_nandS32(__p, __v) (sigclib_atomic_nandS32((__p),          ((long)(__v))))

    #define sigclib_atomic_bool_compare_and_swapU32(__p, __cmp, __new) ((sigclib_atomic_cmpxchgU32((__p), (unsigned long)(__cmp), (unsigned long)(__new))) == (unsigned long)(__cmp))
    #define sigclib_atomic_bool_compare_and_swapS32(__p, __cmp, __new) ((sigclib_atomic_cmpxchgS32((__p), (long)(__cmp), (long)(__new))) == (long)(__cmp))

    
  #else
  
    // atomic get u32
    function global __cdecl sigclib_atomic_getU32 var_input pValue:^udint; end_var var_output retcode:udint; end_var;
    // atomic set u32
    function global __cdecl sigclib_atomic_setU32 var_input pValue:^udint; value:udint; end_var;
    // atomic increment of u32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_incU32 var_input pValue:^udint; end_var var_output retcode:udint; end_var;
    // atomic decrement of u32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_decU32 var_input pValue:^udint; end_var var_output retcode:udint; end_var;
    // atomic add of u32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_addU32 var_input pValue:^udint; addVal:udint; end_var var_output retcode:udint; end_var;
    // atomic sub of u32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_subU32 var_input pValue:^udint; subVal:udint; end_var var_output retcode:udint; end_var;
    // atomic swap u32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_swpU32 var_input pValue:^udint; swpVal:udint; end_var var_output retcode:udint; end_var;
    // atomic compareXchange u32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_cmpxchgU32 var_input pValue:^udint; cmpVal:udint; newVal:udint; end_var var_output retcode:udint; end_var;
    
    // atomic get s32bit
    function global __cdecl sigclib_atomic_getS32 var_input pValue:^dint; end_var var_output retcode:dint; end_var;
    // atomic set s32bit
    function global __cdecl sigclib_atomic_setS32 var_input pValue:^dint; value:dint; end_var;
    // atomic increment of s32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_incS32 var_input pValue:^dint; end_var var_output retcode:dint; end_var;
    // atomic decrement of s32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_decS32 var_input pValue:^dint; end_var var_output retcode:dint; end_var;
    // atomic add of s32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_addS32 var_input pValue:^dint; addVal:dint; end_var var_output retcode:dint; end_var;
    // atomic sub of s32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_subS32 var_input pValue:^dint; subVal:dint; end_var var_output retcode:dint; end_var;
    // atomic swap s32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_swpS32 var_input pValue:^dint; swpVal:dint; end_var var_output retcode:dint; end_var;
    // atomic compareXchange s32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_cmpxchgS32 var_input pValue:^dint; cmpVal:dint; newVal:dint; end_var var_output retcode:dint; end_var;
    
    // atomic compareXchange pointer, return value corresponds to pointer before operator was executed
    function global __cdecl sigclib_atomic_cmpxchgPtr var_input pPointer:^pvoid; cmpPtr:^void; newPtr:^void; end_var var_output retcode:^void; end_var;
    
    // atomic AND u32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_andU32 var_input pValue:^udint; bitVal:udint; end_var var_output retcode:udint; end_var;
    // atomic AND s32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_andS32 var_input pValue:^dint;  bitVal:dint; end_var var_output retcode:dint; end_var;
    // atomic NAND u32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_nandU32 var_input pValue:^udint; bitVal:udint; end_var var_output retcode:udint; end_var;
    // atomic NAND s32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_nandS32 var_input pValue:^dint; bitVal:dint; end_var var_output retcode:dint; end_var;
    // atomic OR u32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_orU32 var_input pValue:^udint; bitVal:udint; end_var var_output retcode:udint; end_var;
    // atomic OR s32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_orS32 var_input pValue:^dint; bitVal:dint; end_var var_output retcode:dint; end_var;
    // atomic XOR u32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_xorU32 var_input pValue:^udint; bitVal:udint; end_var var_output retcode:udint; end_var;
    // atomic XOR s32bit, return value corresponds to value before operator was executed
    function global __cdecl sigclib_atomic_xorS32 var_input pValue:^dint; bitVal:dint; end_var var_output retcode:dint; end_var;
    
    #define sigclib_atomic_add_and_fetchU32(__p, __v)  (sigclib_atomic_addU32((__p),  (to_udint(__v))) +   (to_udint(__v)))
    #define sigclib_atomic_add_and_fetchS32(__p, __v)  (sigclib_atomic_addS32((__p),  (to_dint(__v)))  +   (to_dint(__v)))
    #define sigclib_atomic_sub_and_fetchU32(__p, __v)  (sigclib_atomic_subU32((__p),  (to_udint(__v))) -   (to_udint(__v)))
    #define sigclib_atomic_sub_and_fetchS32(__p, __v)  (sigclib_atomic_subS32((__p),  (to_dint(__v)))  -   (to_dint(__v)))
    #define sigclib_atomic_and_and_fetchU32(__p, __v)  (sigclib_atomic_andU32((__p),  (to_udint(__v))) and (to_udint(__v)))
    #define sigclib_atomic_and_and_fetchS32(__p, __v)  (sigclib_atomic_andS32((__p),  (to_dint(__v)))  and (to_dint(__v)))
    #define sigclib_atomic_or_and_fetchU32(__p, __v)   (sigclib_atomic_orU32((__p),   (to_udint(__v))) or  (to_udint(__v)))
    #define sigclib_atomic_or_and_fetchS32(__p, __v)   (sigclib_atomic_orS32((__p),   (to_dint(__v)))  or  (to_dint(__v)))
    #define sigclib_atomic_xor_and_fetchU32(__p, __v)  (sigclib_atomic_xorU32((__p),  (to_udint(__v))) xor (to_udint(__v)))
    #define sigclib_atomic_xor_and_fetchS32(__p, __v)  (sigclib_atomic_xorS32((__p),  (to_dint(__v)))  xor (to_dint(__v)))
    #define sigclib_atomic_nand_and_fetchU32(__p, __v) (16#FFFFFFFF xor (sigclib_atomic_nandU32((__p), (to_udint(__v))) and (to_udint(__v))))
    #define sigclib_atomic_nand_and_fetchS32(__p, __v) ((-1)        xor (sigclib_atomic_nandS32((__p), (to_dint(__v)))  and (to_dint(__v))))

    #define sigclib_atomic_fetch_and_addU32(__p, __v)  (sigclib_atomic_addU32((__p),  (to_udint(__v))))
    #define sigclib_atomic_fetch_and_addS32(__p, __v)  (sigclib_atomic_addS32((__p),  (to_dint(__v))))
    #define sigclib_atomic_fetch_and_subU32(__p, __v)  (sigclib_atomic_subU32((__p),  (to_udint(__v))))
    #define sigclib_atomic_fetch_and_subS32(__p, __v)  (sigclib_atomic_subS32((__p),  (to_dint(__v))))
    #define sigclib_atomic_fetch_and_andU32(__p, __v)  (sigclib_atomic_andU32((__p),  (to_udint(__v))))
    #define sigclib_atomic_fetch_and_andS32(__p, __v)  (sigclib_atomic_andS32((__p),  (to_dint(__v))))
    #define sigclib_atomic_fetch_and_orU32(__p, __v)   (sigclib_atomic_orU32((__p),   (to_udint(__v))))
    #define sigclib_atomic_fetch_and_orS32(__p, __v)   (sigclib_atomic_orS32((__p),   (to_dint(__v))))
    #define sigclib_atomic_fetch_and_xorU32(__p, __v)  (sigclib_atomic_xorU32((__p),  (to_udint(__v))))
    #define sigclib_atomic_fetch_and_xorS32(__p, __v)  (sigclib_atomic_xorS32((__p),  (to_dint(__v))))
    #define sigclib_atomic_fetch_and_nandU32(__p, __v) (sigclib_atomic_nandU32((__p), (to_udint(__v))))
    #define sigclib_atomic_fetch_and_nandS32(__p, __v) (sigclib_atomic_nandS32((__p), (to_dint(__v))))

    #define sigclib_atomic_bool_compare_and_swapU32(__p, __cmp, __new) ((sigclib_atomic_cmpxchgU32((__p)$^udint, to_udint(__cmp), to_udint(__new))) = to_udint(__cmp))
    #define sigclib_atomic_bool_compare_and_swapS32(__p, __cmp, __new) ((sigclib_atomic_cmpxchgS32((__p)$^dint, to_dint(__cmp), to_dint(__new))) = to_dint(__cmp))

    
  #endif
  
#endif


// ************************************************************************************************
// USAGE
// ************************************************************************************************

// NOTE: Atomicity is only guaranteed when the 32bit integer is located at an 32bit aligned address.
//       In other words the address has to be an exact multiple of 4.

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_atomic_getU32(unsigned long *pValue);
// long sigclib_atomic_getS32(long *pValue);
// atomic get u32
// NOTE: 'pValue' has to be a 32bit aligned address
// --> pValue .......... address of data (NOTE: has to be 32bit-aligned)
// <-- function will return value of given address

// ------------------------------------------------------------------------------------------------
// void sigclib_atomic_setU32(unsigned long *pValue, unsigned long value);
// void sigclib_atomic_setS32(long *pValue, long value);
// atomic set u32
// NOTE: 'pValue' has to be a 32bit aligned address
// --> pValue .......... address of data (NOTE: has to be 32bit-aligned)
// --> value ........... value to set

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_atomic_incU32(unsigned long *pValue);
// long sigclib_atomic_incS32(long *pValue);
// atomic increment of u32bit, return value corresponds to value before operator was executed
// NOTE: 'pValue' has to be a 32bit aligned address
// --> pValue .......... address of data (NOTE: has to be 32bit-aligned)
// <-- function will return corresponding value before operator was executed

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_atomic_decU32(unsigned long *pValue);
// long sigclib_atomic_decS32(long *pValue);
// atomic decrement of u32bit, return value corresponds to value before operator was executed
// NOTE: 'pValue' has to be a 32bit aligned address
// --> pValue .......... address of data (NOTE: has to be 32bit-aligned)
// <-- function will return corresponding value before operator was executed

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_atomic_addU32(unsigned long *pValue, unsigned long addVal);
// long sigclib_atomic_addS32(long *pValue, long addVal);
// atomic add of u32bit, return value corresponds to value before operator was executed
// NOTE: 'pValue' has to be a 32bit aligned address
// --> pValue .......... address of data (NOTE: has to be 32bit-aligned)
// --> addval .......... value to add
// <-- function will return corresponding value before operator was executed

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_atomic_subU32(unsigned long *pValue, unsigned long addVal);
// long sigclib_atomic_subS32(long *pValue, long subVal);
// atomic sub of u32bit, return value corresponds to value before operator was executed
// NOTE: 'pValue' has to be a 32bit aligned address
// --> pValue .......... address of data (NOTE: has to be 32bit-aligned)
// --> addval .......... value to sub
// <-- function will return corresponding value before operator was executed

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_atomic_swpU32(unsigned long *pValue, unsigned long swpVal);
// long sigclib_atomic_swpS32(long *pValue, long swpVal);
// atomic swap of u32bit, return value corresponds to value before operator was executed
// NOTE: 'pValue' has to be a 32bit aligned address
// --> pValue .......... address of data (NOTE: has to be 32bit-aligned)
// <-- function will return corresponding value before operator was executed

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_atomic_andU32(unsigned long *pValue, unsigned long bitVal);
// unsigned long sigclib_atomic_nandU32(unsigned long *pValue, unsigned long bitVal);
// unsigned long sigclib_atomic_orU32(unsigned long *pValue, unsigned long bitVal);
// unsigned long sigclib_atomic_xorU32(unsigned long *pValue, unsigned long bitVal);
// Atomic OPERATION of unsigned 32bit value given by address 'pValue'. The return value corresponds to value before operator was executed.
// NOTE: 'pValue' has to be a 32bit aligned address
// --> pValue .......... address of data (NOTE: has to be 32bit-aligned)
// --> bitVal .......... u32bit value used as operand
// <-- function will return corresponding value before operator was executed
// Pseudocode:
//   retcode = *pValue;
//   *pvalue = retcode OPERATOR bitVal;
//   return retcode;

// ------------------------------------------------------------------------------------------------
// long sigclib_atomic_andS32(long *pValue, long bitVal);
// long sigclib_atomic_nandS32(long *pValue, long bitVal);
// long sigclib_atomic_orS32(long *pValue, long bitVal);
// long sigclib_atomic_xorS32(long *pValue, long bitVal);
// Atomic OPERATION of signed 32bit value given by address 'pValue'. The return value corresponds to value before operator was executed.
// NOTE: 'pValue' has to be a 32bit aligned address
// --> pValue .......... address of data (NOTE: has to be 32bit-aligned)
// --> bitVal .......... s32bit value used as operand
// <-- function will return corresponding value before operator was executed
// Pseudocode:
//   retcode = *pValue;
//   *pvalue = retcode OPERATOR bitVal;
//   return retcode;

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_atomic_cmpxchgU32(unsigned long *pValue, unsigned long cmpVal, unsigned long newVal);
// long sigclib_atomic_cmpxchgS32(long *pValue, long cmpVal, long newVal);
// Atomic compareXchange of unsigned 32bit value given by address 'pValue'. The return value corresponds to value before operator was executed
// NOTE: 'pValue' has to be a 32bit aligned address
// --> pValue .......... address of data (NOTE: has to be 32bit-aligned)
// --> cmpVal .......... value to compare
// --> newval .......... new value
// <-- function will return corresponding value before operator was executed
// Pseudocode:
//   retcode = *pValue;
//   if(*pValue == cmpVal)
//   {
//     *pValue = newVal;     
//   }
//   return retcode;

// ------------------------------------------------------------------------------------------------
// atomic compareXchange pointer, return pointer corresponds to pointer before operator was executed
// void *sigclib_atomic_cmpxchgPtr(void **pPointer, void *cmpPtr, void *newPtr);
// Atomic compareXchange of pointer given by address 'pPointer'. The return value corresponds to pointer before operator was executed
// NOTE: 'pPointer' has to be a 32bit aligned address
// --> pPointer ........ address of Pointer (NOTE: has to be 32bit-aligned)
// --> cmpPtr .......... pointer to compare
// --> newPtr .......... new pointer
// <-- function will return corresponding pointer before operator was executed
// Pseudocode:
//   retcode = *pPointer;
//   if(*pPointer == cmpPtr)
//   {
//     *pPointer = newPtr;     
//   }
//   return retcode;

// ------------------------------------------------------------------------------------------------
// #define sigclib_atomic_add_and_fetchU32(__p, __v)
// #define sigclib_atomic_add_and_fetchS32(__p, __v)
// #define sigclib_atomic_sub_and_fetchU32(__p, __v)
// #define sigclib_atomic_sub_and_fetchS32(__p, __v)
// #define sigclib_atomic_and_and_fetchU32(__p, __v)
// #define sigclib_atomic_and_and_fetchS32(__p, __v)
// #define sigclib_atomic_or_and_fetchU32(__p, __v)
// #define sigclib_atomic_or_and_fetchS32(__p, __v)
// #define sigclib_atomic_xor_and_fetchU32(__p, __v)
// #define sigclib_atomic_xor_and_fetchS32(__p, __v)
// #define sigclib_atomic_nand_and_fetchU32(__p, __v)
// #define sigclib_atomic_nand_and_fetchS32(__p, __v)
// These Macros perform the atomic operation suggested by the name, and returns the new value.
// Note that there are different Macros when using signed or unsigned variables. The address __p has to be 32bit aligned.
// Pseudocode:
//   *__p = *__p OPERATOR __v;
//   return *__p;

// ------------------------------------------------------------------------------------------------
// #define sigclib_atomic_fetch_and_addU32(__p, __v)
// #define sigclib_atomic_fetch_and_addS32(__p, __v)
// #define sigclib_atomic_fetch_and_subU32(__p, __v)
// #define sigclib_atomic_fetch_and_subS32(__p, __v)
// #define sigclib_atomic_fetch_and_andU32(__p, __v)
// #define sigclib_atomic_fetch_and_andS32(__p, __v)
// #define sigclib_atomic_fetch_and_orU32(__p, __v)
// #define sigclib_atomic_fetch_and_orS32(__p, __v)
// #define sigclib_atomic_fetch_and_xorU32(__p, __v)
// #define sigclib_atomic_fetch_and_xorS32(__p, __v)
// #define sigclib_atomic_fetch_and_nandU32(__p, __v)
// #define sigclib_atomic_fetch_and_nandS32(__p, __v)
// These Macros perform the atomic operation suggested by the name, and returns the value that had previously been in memory.
// Note that there are different Macros when using signed or unsigned variables. The address __p has to be 32bit aligned.
// Pseudocode:
//   retcode = *__p;
//   *__p = retcode OPERATOR __v;
//   return retcode;

// ------------------------------------------------------------------------------------------------
// #define sigclib_atomic_bool_compare_and_swapU32(__p, __cmp, __new)
// #define sigclib_atomic_bool_compare_and_swapS32(__p, __cmp, __new)
// These Macros perform an atomic compare and swap. That is, if the current value of *__p equals to __cmp, then write __new into *__p.
// Note that there are different Macros when using signed or unsigned variables. Address __p has to be 32bit aligned.
// Macros returns true if the comparison is successful and newval was written. On the other hand macro will return false.
// Pseudocode:
//   if(*__p == __cmp)
//   {
//     *__p = __new;
//     return true;
//   }
//   return false;

