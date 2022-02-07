
#include <loader\include\userdef.h>

#ifndef DataserviceAPIex_H 
 #define DataserviceAPIex_H 

 #ifdef DATASERVICE_LASAL
 
  #include <loader\class\dataservice\DS_ComDef.h>
  
 #else // DATASERVICE_LASAL
 
  TYPE
    #pragma pack (push, 1)
     DsComData : struct
       datalength      : udint;
       id_major        : uint;
       id_minor        : uint;
       transactionSrc  : uint;
       transactionDest : uint;
       messageId       : udint;
       data            : array[0..1023] of usint;
     end_struct;
    #pragma pack (pop)
  END_TYPE
 
  #ifndef  ComDataHeaderSize
   #define ComDataHeaderSize  16
  #endif
 
 #endif // DATASERVICE_LASAL
 
#endif // DataserviceAPI_H 