
#include <lsl_st_mt.h>

TYPE
  tTaskC2 : struct
    delayUsMs : ^void;
    time_us   : udint;
    pfkt      : ^void;
    pcookie   : ^void;
  end_struct;
END_TYPE

#ifdef _LSL_TARGETARCH_X86
VAR_EXTERNAL
  _WHOAMI : udint;
END_VAR
#endif 

VAR_PRIVATE
  mt_api : ^LSL_MT_TYPE;
END_VAR

function global __cdecl lib_malloc var_input size:udint; end_var var_output retcode:^void; END_VAR;
function global __cdecl lib_free var_input ptr:^void; end_var var_output retcode:^void; END_VAR;
function global __cdecl lib_threadC2Funct var_input pcookie:^void; end_var var_output retcode:udint; end_var;
function global __cdecl lib_name_unique var_input pd:^char; name:^char; end_var var_output retcode:^char; end_var;

FUNCTION global __cdecl lib_tabsolute var_output retcode:udint; end_var

  retcode := ops.tAbsolute;

END_FUNCTION

FUNCTION global __cdecl lib_tabsolute_nonzero var_output retcode:udint; end_var

  retcode := ops.tAbsolute;
  if(retcode = 0) then
    retcode -= 1;
  end_if;

END_FUNCTION

FUNCTION global __cdecl lib_is_lars var_output retcode:udint; end_var
 #ifdef _LSL_TARGETARCH_X86
  VAR
    val : udint;
  END_VAR
 #endif
 
  retcode := 0;
  
 #ifdef _LSL_TARGETARCH_X86
  val := _WHOAMI and 16#0FFFFFFF;
  if((val = 16#1B) | (val = 16#1C) | (val = 16#1D) | (val = 16#1E)) then
    retcode := 1;
  end_if;
 #endif

END_FUNCTION

FUNCTION MT_API_Usage var_output retcode : ^LSL_MT_TYPE; end_var
  
  if(mt_api = NIL) then
   if(OS_CILGet("LSL_MULTITASK", #mt_api$void) <> SYS_ERR_NONE) then
     mt_api := nil;
   end_if;
  end_if;
  
  retcode := mt_api;

END_FUNCTION

function global __cdecl lib_ctor

  mt_api := nil;
  
end_function

#ifndef LSL_EQUIPMENT_VALID
 #define LSL_EQUIPMENT_VALID 0x80000000
#endif
#ifndef LSL_OS_LINUX
 #define LSL_OS_LINUX        0x00040000
#endif
#ifndef LSL_PLATFORMMASK    
 #define LSL_PLATFORMMASK    0x0000FF00
#endif
#ifndef LSL_OS_SALAMANDER
 #define LSL_OS_SALAMANDER   0x00000200
#endif

FUNCTION global __cdecl lib_is_salamander var_output retcode:udint; end_var
  VAR
  	ptr : ^udint;
    val : udint;
  END_VAR
  
  retcode := 0;
  ptr := #_RealMaximumTime;
  ptr += 16#F44;
  val := ptr^;

  if((val and (LSL_OS_LINUX or LSL_EQUIPMENT_VALID)) = (LSL_OS_LINUX or LSL_EQUIPMENT_VALID))  then
    val := val and LSL_PLATFORMMASK;
    if(val = LSL_OS_SALAMANDER) | (val = 0) then
      retcode := 1;
    end_if;
  end_if;

END_FUNCTION

FUNCTION global __cdecl lib_is_linux var_output retcode:udint; end_var
  VAR
  	ptr : ^udint;
  END_VAR
  
  ptr := #_RealMaximumTime;
  ptr += 16#F44;
  retcode := ((ptr^) and (LSL_OS_LINUX or LSL_EQUIPMENT_VALID)) = (LSL_OS_LINUX or LSL_EQUIPMENT_VALID);

END_FUNCTION

FUNCTION lib_thread_create_intern var_input pfkt:^void; priority:udint; flags:udint; pthis:^void; name:^char; end_var var_output retcode:udint; end_var
  VAR
    stacksize : udint;
    tmpname : array[0..32] of char;
  END_VAR

  retcode := 0;

  if(MT_API_Usage() <> NIL) then
    stacksize := (64 * 1024);
    if(lib_is_linux() = 0) then
      stacksize := (16 * 1024); // maxsize bei rtk-opsys 
    end_if;
    
    if(name = NIL) then
      name := lib_name_unique(#tmpname[0], "DefTaskName");
    end_if;
    
    retcode$MT_TASKHANDLE := OS_MT_CreateThread(pfkt, priority, stacksize, flags, pthis, name);
  end_if;

END_FUNCTION

FUNCTION global __cdecl lib_threadC2 var_input pt2:^tTaskC2; end_var
  VAR
    td2 : tTaskC2;
  END_VAR

  td2 := pt2^; // daten umkopieren
  lib_free(pt2); // daten freigeben !!
 
  while(1) do
    if(td2.pfkt$lib_threadc2Funct(td2.pcookie) = 0) then
      return; // finito
    end_if;
    td2.delayUsMs$P_MT_DelayUS(td2.time_us);
  end_while;

END_FUNCTION

FUNCTION global __cdecl lib_thread_create2 var_input pfkt:^void; priority:udint; time_us:udint; pcookie:^void; name:^char; end_var var_output retcode:udint; end_var
  VAR
    pt2 : ^tTaskC2;
  END_VAR

  retcode := 0;
  
  if((pfkt <> NIL) & (Mt_API_Usage() <> NIL)) then
  
    pt2$^void := lib_malloc(sizeof(tTaskC2));
    if(pt2 <> NIL) then
      
      pt2^.pcookie   := pcookie;
      pt2^.pfkt      := pfkt;
      pt2^.time_us   := time_us;
      pt2^.delayUsMs := MT_API^.MT_DelayUS; // us-basis
      
      if(mt_api^.version < 16#00010004) then // abfrage ob delayUS unterstützt wird
        pt2^.delayUsMs := MT_API^.MT_Delay; // ms-basis
        if(time_us <> 0) then
          pt2^.time_us := (time_us + 500) / 1000; // aufrunden
          if(pt2^.time_us = 0) then
            pt2^.time_us := 1;
          end_if;
        end_if;
      end_if;
        
      retcode := lib_thread_create_intern(#lib_threadC2(), priority, 0, pt2, name);
      // NOTE: pt2 nicht mehr weiterverwenden, wenn retcode <> 0. siehe sigclib_free in funktion sigclib_thread2()
      
      if(retcode = 0) then
        lib_free(pt2);
      end_if;
    end_if;
  end_if;
  
END_FUNCTION


FUNCTION global __cdecl lib_sleep var_input duration_ms:udint; end_var


  if(MT_API_Usage() <> nil) then
    OS_MT_DELAY(duration_ms);
  end_if;
//  mt_api^.MT_Delay$
//  P_MT_Delay(duration_ms);

END_FUNCTION

#ifdef _LSL_TARGETARCH_ARM

FUNCTION awl global __cdecl lib_atomic_incU32 var_input pValue:^udint; end_var var_output retcode:udint; end_var
  
AtomicUIncRetry
  LDREX r1, [pValue]
  ADDS r3, r1, #1
  STREX r2, r3, [pValue]
  CMP r2, #0
  BNE AtomicUIncRetry
  MOV r0, r1
 
  // L.EDI pValue
  // INC.D (EDI)

END_FUNCTION

FUNCTION awl global __cdecl lib_atomic_decU32 var_input pValue:^udint; end_var var_output retcode:udint; end_var
  
AtomicUDecRetry
  LDREX r1, [pValue]
  SUBS r3, r1, #1
  STREX r2, r3, [pValue]
  CMP r2, #0
  BNE AtomicUDecRetry
  MOV r0, r1
  
  // L.EDI pValue
  // DEC.D (EDI)

END_FUNCTION

FUNCTION awl global __cdecl lib_atomic_cmpxchgU32 var_input pValue:^udint; cmpVal:udint; newVal:udint; end_var var_output retcode:udint; end_var

AtomicSCmpXchgRetry
  MOV r4, #0
  LDREX r3, [pValue]
  TEQ r3, cmpVal
  STREXEQ r4, newVal, [pValue]
  CMP r4, #0
  BNE AtomicSCmpXchgRetry
  MOV r0, r3

END_FUNCTION

#endif