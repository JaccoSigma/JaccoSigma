// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 30.04.2019                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#ifndef _CLimits_H
 #define _CLimits_H
 
 #include "DefineCompiler.h" 

 #ifdef cCompile

   // _int16
   #ifndef sigclib_SHORT_MAX
    #define sigclib_SHORT_MAX      32767
   #endif
   #ifndef sigclib_SHORT_MIN
    #define sigclib_SHORT_MIN      (-(sigclib_SHORT_MAX) -1)
   #endif
   #ifndef sigclib_int16_MAX
    #define sigclib_int16_MAX      sigclib_SHORT_MAX
   #endif
   #ifndef sigclib_int16_MIN
    #define sigclib_int16_MIN      sigclib_SHORT_MIN
   #endif

   // _uint16
   #ifndef sigclib_USHORT_MAX
    #define sigclib_USHORT_MAX     65535
   #endif
   #ifndef sigclib_USHORT_MIN
    #define sigclib_USHORT_MIN     0
   #endif
   #ifndef sigclib_uint16_MAX
    #define sigclib_uint16_MAX     sigclib_USHORT_MAX
   #endif
   #ifndef sigclib_uint16_MIN
    #define sigclib_uint16_MIN     sigclib_USHORT_MIN
   #endif

   // _int32
   #ifndef sigclib_LONG_MAX
    #define sigclib_LONG_MAX       2147483647L
   #endif
   #ifndef sigclib_LONG_MIN
    #define sigclib_LONG_MIN       (-(sigclib_LONG_MAX) -1L)
   #endif
   #ifndef sigclib_int32_MAX
    #define sigclib_int32_MAX      sigclib_LONG_MAX
   #endif
   #ifndef sigclib_int32_MIN
    #define sigclib_int32_MIN      sigclib_LONG_MIN
   #endif
   
   // _uint32
   #ifndef sigclib_ULONG_MAX
    #define sigclib_ULONG_MAX      4294967295UL
   #endif
   #ifndef sigclib_ULONG_MIN
    #define sigclib_ULONG_MIN      0UL
   #endif
   #ifndef sigclib_uint32_MAX
    #define sigclib_uint32_MAX     sigclib_ULONG_MAX
   #endif
   #ifndef sigclib_uint32_MIN
    #define sigclib_uint32_MIN     sigclib_ULONG_MIN
   #endif

   // _int64
   #ifndef sigclib_LONG_LONG_MAX
    #define sigclib_LONG_LONG_MAX  9223372036854775807LL
   #endif
   #ifndef sigclib_LONG_LONG_MIN
    #define sigclib_LONG_LONG_MIN  (-(sigclib_LONG_LONG_MAX) -1)
   #endif
   #ifndef sigclib_int64_MAX
    #define sigclib_int64_MAX      sigclib_LONG_LONG_MAX
   #endif
   #ifndef sigclib_int64_MIN
    #define sigclib_int64_MIN      sigclib_LONG_LONG_MIN
   #endif

   // _uint64
   #ifndef sigclib_ULONG_LONG_MAX
    #define sigclib_ULONG_LONG_MAX 0xFFFFFFFFFFFFFFFFULL
   #endif
   #ifndef sigclib_ULONG_LONG_MIN
    #define sigclib_ULONG_LONG_MIN 0ULL
   #endif
   #ifndef sigclib_uint64_MAX
    #define sigclib_uint64_MAX     sigclib_ULONG_LONG_MAX
   #endif
   #ifndef sigclib_uint64_MIN
    #define sigclib_uint64_MIN     sigclib_ULONG_LONG_MIN
   #endif

   // _f32
   #ifndef sigclib_FLOAT_MIN
    #define sigclib_FLOAT_MIN -340282346638528859811704183484516925440.0
   #endif 
   #ifndef sigclib_FLOAT_MAX
    #define sigclib_FLOAT_MAX  340282346638528859811704183484516925440.0
   #endif

 #else

 #endif
 
#endif
