#ifndef HW_CONTROL_LOGGING_H
#pragma once
#define HW_CONTROL_LOGGING_H


//*****************************************************************************
//** SPAM PROTECTION SETTINGS                                                **
//*****************************************************************************

// if this is active, the logging methods only trace the strings without writing them to the log file
//#define HWCL_DISABLE_FILE_LOGGING

// if this is active, the logging methods are monitored for spam (logging too frequently)
#define HWCL_ENABLE_SPAM_PROTECTION

// log spam detection to prevent flooding of file or damaging of the media (value 0 disables this spam protection; default value 10)
// as soon as X logs arrive within 1 second (can be adjusted via HWCL_SPAM_PROTECTION_TIME), writing into the logfile is disabled
// if there is a pause of 1 second (can be adjusted via HWCL_SPAM_PROTECTION_TIME) the spam counter gets reset (it's possible to have X-1 logs per second to not disable the logging)
#define HWCL_SPAM_DETECTION_LIMIT        10

// time span in ms to watch for flooding (default: 100)
#define HWCL_SPAM_DETECTION_TIME        100

// time span in ms to reset the internal counter (disable log block) if there have not been any logs (default: 1000)
#define HWCL_SPAM_BLOCK_RELEASE_TIME   1000



// maximal size of logged object name in characters. if it's longer it will be shortened by HWCL_LimitObjName to fit in a memory sized HWC_MAX_OBJECT_NAME_SIZE + 1 for termination
#define HWCL_MAX_OBJECT_NAME_SIZE  50

// number of characters to take from the end when shrinking the name ("BeginningOfObjectName" + "~" + END_CHARS)
#define HWCL_OBJECT_NAME_END_CHARS 10


//*****************************************************************************
//** GLOBAL FUNCTIONS                                                        **
//*****************************************************************************

FUNCTION GLOBAL _HWCL_LogInit;
FUNCTION GLOBAL _HWCL_LogError
  VAR_INPUT
    pThis   : pVirtualBase;
    e_msg  : ^CHAR;
  END_VAR;

FUNCTION GLOBAL _HWCL_LogValue
  VAR_INPUT
    pThis   : pVirtualBase;
    pString : ^CHAR;
    Value   : UDINT;
    Value2  : UDINT;(* := 16#FFFFFFFF *)
    Value3  : UDINT;(* := 16#FFFFFFFF *)
  END_VAR;

FUNCTION GLOBAL _HWCL_LimitObjName
  VAR_INPUT
    pObjName : ^CHAR;
  END_VAR;

// We don't have a macro because of the dynamic number of parameters => use directly the name that the macro would have.
// Look for "sigclib_sprintfST" in "CStrLib.h" for a detailed parameter usage description
FUNCTION GLOBAL __CDECL HWC_LogPrintf
  VAR_INPUT
    pThis   : pVirtualBase;
    format : ^char;
    p0 : ^void := NIL;
    p1 : ^void := NIL;
    p2 : ^void := NIL;
    p3 : ^void := NIL;
    p4 : ^void := NIL;
    p5 : ^void := NIL;
    p6 : ^void := NIL;
    p7 : ^void := NIL;
    p8 : ^void := NIL;
    p9 : ^void := NIL;
  END_VAR
  VAR_OUTPUT
    retcode : udint;
  END_VAR;


// LogInit has to be called before logging if there is no HwControl object or logging takes place in the constructor (if there is a HwControl object it calls the LogInit in it's constructor => SDIAS and VARAN need HwControl and can therefore skip this call!)
#define HWC_LogInit()                             _HWCL_LogInit()

// logs the object name of the passed this pointer or <unknown> if NIL and the passed message
#define HWC_LogError(pThis, pMsg)                 _HWCL_LogError(pThis, pMsg)

// logvalue additionally allows to use numbers in the message as {0} {1} {2} which are then being replaced by the values passed via v1 v2 v3
#define HWC_LogValue1(pThis, pMsg, v1)            _HWCL_LogValue(pThis, pMsg, v1, 16#FFFFFFFF, 16#FFFFFFFF)
#define HWC_LogValue2(pThis, pMsg, v1, v2)        _HWCL_LogValue(pThis, pMsg, v1, v2, 16#FFFFFFFF)
#define HWC_LogValue3(pThis, pMsg, v1, v2, v3)    _HWCL_LogValue(pThis, pMsg, v1, v2, v3)
    
// limits the string length according to the constants HWC_MAX_OBJECT_NAME_SIZE (maximal length of string) and HWC_OBJECT_NAME_END_CHARS (how many characters to take from the end)
//#define HWC_LimitObjName(pMsg)                    _HWCL_LimitObjName(pMsg)

// used directly instead of macro because of the dynamic number of parameters (between 1 and 10)
//#define HWC_LogPrintf(...)



//*****************************************************************************
//** TYPES                                                                   **
//*****************************************************************************

  TYPE
    t_HWCSpamFlags : BDINT
      [
      1 FilterActive,
      2 Initialized,
      ];

#ifdef HWCL_ENABLE_SPAM_PROTECTION
    t_HWCSpamFilter : STRUCT
      aLogTimes         : ARRAY [0..HWCL_SPAM_DETECTION_LIMIT-1] OF UDINT; // timestamps of last logs
      ud_NextTimeIndex  : UDINT;  // index of next timestamp to overwrite
      SkippedMessages   : UDINT;  // number of messages skipped due to spamming (is printed as first message after the spam block)
      bd_SpamFlags      : t_HWCSpamFlags;
      ud_LastLog        : UDINT;  // time of last log try
    END_STRUCT;
#endif
  END_TYPE



//**********************************************
//** Macros                                   **
//**********************************************
//for logging functions
#ifndef OS_KernelLog0

  FUNCTION __CDECL P_KernelLog0
  VAR_INPUT
    msg	:^CHAR;
  END_VAR;

  #define OS_KernelLog0(p1)           p_KernelLog $ P_KernelLog0(p1);

  FUNCTION __CDECL P_KernelLog1
  VAR_INPUT
    msg	:^CHAR;
    Var1  : DINT;
  END_VAR;

  #define OS_KernelLog1(p1,p2)        p_KernelLog $ P_KernelLog1(p1,p2);

  FUNCTION __CDECL P_KernelLog2
  VAR_INPUT
    msg	:^CHAR;
    Var1  : DINT;
    Var2  : DINT;
  END_VAR;

  #define OS_KernelLog2(p1,p2,p3)     p_KernelLog $ P_KernelLog2(p1,p2,p3);

  FUNCTION __CDECL P_KernelLog3
  VAR_INPUT
    msg	:^CHAR;
    Var1  : DINT;
    Var2  : DINT;
    Var3  : DINT;
  END_VAR;

  #define OS_KernelLog3(p1,p2,p3,p4)  p_KernelLog $ P_KernelLog3(p1,p2,p3,p4);
#endif


#endif // HW_CONTROL_LOGGING_H