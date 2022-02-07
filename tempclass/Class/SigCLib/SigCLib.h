
#ifndef _SigCLibH
 #define _SigCLibH

  #ifndef NULL
   #define NULL 0
  #endif 
  
  
  #include "CStrLib.h"
  #include "CStrLib16.h"
  #include "CTimeLib.h"
  #include "CMathLib.h"
  #include "CSortLib.h"
  #include "CFileLib.h"
  #include "CMemLib.h"
  #include "CNetworkLib.h"
  #include "CCrcLib.h"
  #include "CPlcOnline.h"
  #include "CSysLib.h"
  #include "CTcpIp.h"
  #include "CAtomic.h"
  #include "CFiFo.h"
  #include "CRingBuffer.h"
  #include "CPipe.h"
  #include "CHeap.h"
  #include "CDeEncode.h"

  #define sigclib_version 37
 
#endif

// version 0.37, 20.09.2021 ----------------------------------------------------------------------
// new functions added:
//   sigclib_randomU32(), sigclib_randomF32(), sigclib_srandom()
//   sigclib_setenvvar(), sigclib_getenvvar()
//   sigclib_isxdigit(), sigclib_isspace(), sigclib_ispunct(), sigclib_isalnum(), sigclib_isalpha(), sigclib_isprint()
//   sigclib_qsort_ex(), sigclib_bsearch_ex(), sigclib_bsearch_index_ex()
//   sigclib_logfile(), sigclib_logfileline(), sigclib_logfileST()
//   sigclib_strlcpy(), sigclib_strlcat()
//   sigclib_atomic_cmpxchgPtr()
// improved randomness done in function sigclib_random() (if not seeded, seed with date, time and tabsolute)
// improved speed at function hsort()
// Interfacepointer "LSL_MULTITASK" and "TCP_USER" changed from Language ST to C (incl. static Variables).
// Function sigclib_cfopen(): expand by attribute 'change' ('c' and 'C').
// new functions to prevent from "year 2038" problem added:
//   sigclib_timeU32(), sigclib_gmtimeU32(), sigclib_timegmU32()
//   sigclib_time64(), sigclib_gmtime64(), sigclib_timegm64()
// file CStrScn.cpp added: 
//   sigclib_sscanf(), sigclib_sscanfST()


// version 0.36, 28.06.2021 ----------------------------------------------------------------------
// 8D-2047: new functions added:
//   sigclib_atomic_andU32(), sigclib_atomic_andS32(), sigclib_atomic_nandU32(), sigclib_atomic_nandS32(), 
//   sigclib_atomic_orU32(), sigclib_atomic_orS32(), sigclib_atomic_xorU32(), sigclib_atomic_xorS32(),
//   sigclib_strtok_r(), sigclib_strtoll(), sigclib_get_attributes(), sigclib_set_attributes()
// 8D-2047: new macros added:
//   sigclib_atomic_add_and_fetchU32(__p, __v), sigclib_atomic_add_and_fetchS32(__p, __v)
//   sigclib_atomic_sub_and_fetchU32(__p, __v), sigclib_atomic_sub_and_fetchS32(__p, __v)
//   sigclib_atomic_and_and_fetchU32(__p, __v), sigclib_atomic_and_and_fetchS32(__p, __v)
//   sigclib_atomic_or_and_fetchU32(__p, __v), sigclib_atomic_or_and_fetchS32(__p, __v)
//   sigclib_atomic_xor_and_fetchU32(__p, __v), sigclib_atomic_xor_and_fetchS32(__p, __v)
//   sigclib_atomic_nand_and_fetchU32(__p, __v), sigclib_atomic_nand_and_fetchS32(__p, __v)
//   sigclib_atomic_fetch_and_addU32(__p, __v), sigclib_atomic_fetch_and_addS32(__p, __v)
//   sigclib_atomic_fetch_and_subU32(__p, __v), sigclib_atomic_fetch_and_subS32(__p, __v)
//   sigclib_atomic_fetch_and_andU32(__p, __v), sigclib_atomic_fetch_and_andS32(__p, __v)
//   sigclib_atomic_fetch_and_orU32(__p, __v), sigclib_atomic_fetch_and_orS32(__p, __v)
//   sigclib_atomic_fetch_and_xorU32(__p, __v), sigclib_atomic_fetch_and_xorS32(__p, __v)
//   sigclib_atomic_fetch_and_nandU32(__p, __v), sigclib_atomic_fetch_and_nandS32(__p, __v)
//   sigclib_atomic_bool_compare_and_swapU32(__p, __cmp, __new), sigclib_atomic_bool_compare_and_swapS32(__p, __cmp, __new)
// new types for c-language added:
//   _f32, _f64


// version 0.35, 05.03.2021 ----------------------------------------------------------------------
// new functions added:
//   sigclib_uitoa(), sigclib_lltoa(), sigclib_ulltoa(), sigclib_is_linux(), sigclib_is_gecko()
//   sigclib_ulltou16(), sigclib_lltou16(), sigclib_uitou16(), sigclib_itou16()
//   sigclib_remove_directory(), sigclib_cleanup_directory()
// Function sigclib_sprintfST() will work with value of type _int64 and _uint64 as well ("%v8"and "%V8")
//   8D-1981: sigclib_atomic_swpU32() and sigclib_atomic_swpS32() changed because of deprecated command SWP at ARM-processor
//   8D-2112: Internal of function sigclib_rand() changed.


// version 0.34, 07.10.2020 ----------------------------------------------------------------------
// new functions added:
//   sigclib_is_lars(), sigclib_tabsolute_nonzero(), sigclib_getmicrotime_nonzero()
//   sigclib_ascii_to_utf8_len(), sigclib_str16_to_utf8_len(), sigclib_utf8_to_ascii_len(), sigclib_utf8_to_str16_len()
// Function sigclib_cfopen(): expand by attribute 'append' ('A' and 'a')
// Separator '-' added to function sigclib_string_to_time()


// version 0.33, 02.10.2020 ----------------------------------------------------------------------
// new function added:
//   sigclib_gmtime_threadsafe()
// method SigCLib::UnixTimeStampConvert() done threadsafe


// version 0.32, 15.04.2020 ----------------------------------------------------------------------
// new functions added:
//   sigclib_strcpytill16(), sigclib_strrchr16() 
//   sigclib_fputs16(), sigclib_fgets16(), sigclib_fwrite_crlf16(), sigclib_fwrite_crlf()
//   sigclib_cfputs16(), sigclib_cfgets16(), sigclib_cfwrite_crlf16()
//   sigclib_strlen_ex(), sigclib_strcpy_ex(), sigclib_strcat_ex(), sigclib_strcmp_ex()


// version 0.31, 17.01.2020 ----------------------------------------------------------------------
// new functions added:
//   sigclib_utf8_to_utf16(), sigclib_utf16_to_utf8()
//   sigclib_tcp_shutdown(), sigclib_tcp_select(), sigclib_tcp_recvfrom(), sigclib_tcp_sendto(), sigclib_tcp_getlinkstatus()
//   sigclib_udp_socket_open(), sigclib_udp_bind()
//   sigclib_thread_delay_us(), sigclib_waste_us()
//   sigclib_cfwrite_crlf(), sigclib_cflength()
// new define added:
//   sigclib_version
// cBaseTypeVer (_uint32, _int32, etc.) in DefineCompiler hinzugefügt
// ST-Deklaration von Funktion sigclib_base64_encode() korrigiert


// version 0.30, 28.10.2019 ----------------------------------------------------------------------
// correction:
//   function sigclib_sprintfST() will work with signed 8-bit value on target ARM as well
//   sa53198: method SigCLib::UnixTimeStampConvert() will work with invalid timestamp, therefore a default Time/Date will be created.
// new c-macro added:
//   sigclib_round()


// version 0.29, 18.10.2019 ----------------------------------------------------------------------
// new functions added:
//   sigclib_strcrc(), sigclib_strcrc16()
//   sigclib_sort_txtbuffer(), sigclib_sort_txtptr(), sigclib_sortfkt_ASCII(), sigclib_sortfkt_ABC()
//   sigclib_leap_year()
//   sigclib_encode_bin(), sigclib_decode_bin(), sigclib_encode_txt(), sigclib_decode_txt()
//   sigclib_stricmp(), sigclib_strpbrk(), sigclib_strrchr(), sigclib_sprintfST()
//   sigclib_nameof_baseclass()
//   sigclib_fputs(), sigclib_cfputs()
//   sigclib_mkdir() and sigclib_rmdir() are accessible in ST
//   sigclib_strfdatetime(), sigclib_calendar_week()


// version 0.28, 14.08.2019 ----------------------------------------------------------------------
// new functions added:
//   sigclib_cfmemory(), sigclib_cfok(), sigclib_cferror_set(), sigclib_cferror_get(), sigclib_cfversion_set(), sigclib_cfversion_get(), sigclib_cfcookie_set(), sigclib_cfcookie_get()
//   sigclib_ascii_to_utf8(), sigclib_utf8_to_ascii()
//   sigclib_base64_encode(), sigclib_base64_decode(), sigclib_base64_encode_size()
//   sigclib_name_enlarge(), sigclib_name_unique(), sigclib_utoa(), sigclib_htoa_ex()
// new files added:
//   CDeEncode.h, CDeEncode.cpp


// version 0.27, 05.07.2019 ----------------------------------------------------------------------
// new functions added:
//   sigclib_tcp_...()
//   sigclib_drivelist()
//   sigclib_strcspn(), sigclib_atoll()
//   sigclib_thread_create2(), sigclib_thread_get_current_free_stacksize()
// new macro added:
//   sigclib_fprintf()
// new files added:
//   CLimits.h, CTcpIp.h, SigCLib.ico


// version 0.26, 24.04.2019 ----------------------------------------------------------------------
// new functions added:
//   sigclib_buffer_create()
//   sigclib_buffer_destroy()
// st-interface for sigclib_findfirst(), sigclib_findnext(), sigclib_findclose() implemented


// version 0.25, 28.02.2019 ----------------------------------------------------------------------
// correction: SA 43337 SigCLib.RealToStr() (sigclib_ftoa_comma())
// new typ "st_real" is available for c-language to put real/float values into parameter of function
// new macro added:
//   sigclib_min
//   sigclib_printf
// new functions added:
//   sigclib_strcpytill(), 
//   sigclib_lookup_class(), sigclib_lookup_object(), 
//   sigclib_isderived_from(),
//   sigclib_revisionof_class(), sigclib_revisionof_object(), sigclib_revisionof_loader(), sigclib_revisionof_opsys(), 
//   sigclib_nameof_class(), sigclib_nameof_object(),
//   sigclib_reset(), sigclib_restart(),
//   sigclib_virtual_methodpointer(),
//   sigclib_thread_create(), sigclib_thread_destroy(), sigclib_thread_delay(), sigclib_waste(),
// Option HEAPBUBBLE_INUSE added


// version 0.24, 06.07.2018 ----------------------------------------------------------------------
// New define added:   sigclib_arraysize()
// New function added:   sigclib_fileread(), sigclib_cilget()
// New Functionality "Heap" added


// version 0.23, 13.10.2017 ----------------------------------------------------------------------
// New functions added:
//   * sigclib_atomic_getU32(), sigclib_atomic_setU32(), sigclib_atomic_getS32(), sigclib_atomic_setS32()
// sa38620: Implementation of following functions changed
//   * sigclib64_sin(), sigclib64_cos(), sigclib64_tan(), sigclib64_asin(), sigclib64_acos(), sigclib64_atan()
// Correction:
//   Method RealToString() will work with values greater than 4294967295 and lower than -4294967295
//   Method UnixTimeStampConvert() and function sigclib_gmtime() will work with 1st second of each 1st day in month.


// version 0.22, 20.07.2017 ----------------------------------------------------------------------
// * new functions
//    sigclib_atomic_incU32(), sigclib_atomic_decU32(), sigclib_atomic_addU32(), sigclib_atomic_subU32(), sigclib_atomic_swpU32(), sigclib_atomic_cmpxchgU32()
//    sigclib_atomic_incS32(), sigclib_atomic_decS32(), sigclib_atomic_addS32(), sigclib_atomic_subS32(), sigclib_atomic_swpS32(), sigclib_atomic_cmpxchgS32()
//    sigclib_absday(), sigclib_day_of_week(), sigclib_lasal_to_date(), sigclib_lasal_to_time(),
//    sigclib_time_to_lasal(), sigclib_date_to_lasal(), sigclib_get_days_of_month(), sigclib_make_date()
// * functionality of cPipe added


// version 0.21, 05.07.2017 ----------------------------------------------------------------------
// * new functions
//     sigclib_get_thisptr(), sigclib_htoa(), sigclib_mutex_name()
// * functionality of cFiFo added
// * functionality of cRingBuffer added


// version 0.20, 11.04.2017 ----------------------------------------------------------------------
// * new functions 
//     sigclib_memoryV1(), sigclib_mallocV1(), sigclib_callocV1(), sigclib_reallocV1(), sigclib_freeV1()


// version 0.19, 01.02.2017 ----------------------------------------------------------------------
// * new functions
//     sigclib_str16_to_utf8(), sigclib_utf8_to_str16(), sigclib_serviceprovider(), sigclib_is_salamander()
//     sigclib_u32ipaddress(), sigclib_crc64()
// * new methods
//     ToStr16, ToStr08, StrLen16, StrCpy16, StrCmp16, StrCat16, StrChr16, StrUpr16, StrLwr16, StrStr16, Str16ToUtf8, Utf8ToStr16
// * #ifdef _DefineCompileH bei headerfile DefineCompiler.h eingebaut
// * optimierung für ARM (GNU O3) bei folgenden funktionen eingeschaltet
//   sigclib_memset(), sigclib_memcpy(), sigclib_memcmp(), sigclib_strlen(), sigclib_strcpy(), sigclib_strcmp(), sigclib_memmove(), 
//   sigclib_strupr(), sigclib_strlwr(), sigclib_strchr(), sigclib_hsort()
