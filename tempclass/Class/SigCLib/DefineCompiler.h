#ifndef _DefineCompilerH
  #define _DefineCompilerH
  
  #ifndef cCompile
  
    #define cOptimize

    #ifdef _MSC_VER
      #define cCompile
    #endif
    #ifdef _GNUC
      #define cCompile
      
      #if (__GNUC > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 4))
       // ab gnu version 4.4 funktioniert setup 03 zum code-optimieren
       #undef cOptimize
       #define cOptimize __attribute__ ((optimize (3)))
      #endif
    #endif
  
  #endif

  #ifdef cCompile

    #ifndef cExtern
      #ifdef __cplusplus
        #define cExtern extern "C"
      #else
        #define cExtern extern 
      #endif
    #endif
  
    #ifndef cBaseTypeVer
     #define cBaseTypeVer     2
      typedef unsigned char       _uint08;
      typedef signed   char        _int08;
      typedef unsigned short      _uint16;
      typedef signed   short       _int16;
      typedef unsigned long       _uint32;
      typedef signed   long        _int32;
      typedef unsigned long long  _uint64;
      typedef signed   long long   _int64;
      typedef float                  _f32; // version 2
      typedef double                 _f64; // version 2
    #endif  
  
  #endif
  
#endif

 
 
 