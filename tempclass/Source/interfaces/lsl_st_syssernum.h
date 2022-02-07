(****************************************************************************************)
(*                                                                                      *)
(* lsl_st_syssernum.h                                                                   *)
(*                                                                                      *)
(* enthält die Definitionen für Zugriff auf Seriennnummern                              *)
(*                                                                                      *)
(* HH                                                                                   *)
(*                                                                                      *)
(*                                                                                      *)
(****************************************************************************************)
#ifndef __LSL_ST_SYSSERNUM
#pragma once
#define __LSL_ST_SYSSERNUM


//
// PLC Info
//
TYPE
LSL_PLCINFO : STRUCT

  //
  // Version
  //
  udVersion : UDINT;

  // BIOS Version
  szBIOSVersion : ARRAY [0..15] OF CHAR;

  // Seriennummer
  szSerialNumber : ARRAY [0..23] OF CHAR;

  // Applikation
  szApplication : ARRAY [0..127] OF CHAR;

  END_STRUCT;
END_TYPE



//
// Interface
//
TYPE
  LSL_ISYSSERNUM : STRUCT

  //
  // Version
  //
  udVersion : UDINT;

  //
  // Interface
  //

  // Reserved
  Reserved1 : pVoid;

  // Hole Seriennnummer des PLC
  SernumGetPLC : pVoid;

  // Hole Seriennnummer eines Drives
  SernumGetPLCDrive : pVoid;

  // Hole PLC Info
  SernumGetPLCInfo : pVoid;

  END_STRUCT;
END_TYPE



// SernumGetPLC
FUNCTION __CDECL GLOBAL P_SernumGetPLC
VAR_INPUT
  pSerNum   : ^VOID;
  ulBufLen  : UDINT;
END_VAR
VAR_OUTPUT
    retval  : DINT;
END_VAR;
#define ISYSSERNUM_SERNUMGETPLC(p1,p2)  pISysSernum^.SernumGetPLC $ P_SernumGetPLC(p1,p2)


// SernumGetPLCDrive
FUNCTION __CDECL GLOBAL P_SernumGetPLCDrive
VAR_INPUT
  ucDrive   : USINT;
  pSerNum   : ^VOID;
  ulBufLen  : UDINT;
END_VAR
VAR_OUTPUT
    retval  : DINT;
END_VAR;
#define ISYSSERNUM_SERNUMGETPLCDRIVE(p1,p2,p3)  pISysSernum^.SernumGetPLCDrive $ P_SernumGetPLCDrive(p1,p2,p3)


// SernumGetPLCInfo
FUNCTION __CDECL GLOBAL P_SernumGetPLCInfo
VAR_INPUT
END_VAR
VAR_OUTPUT
    retval  : ^LSL_PLCINFO;
END_VAR;
#define ISYSSERNUM_SERNUMGETPLCINFO()  pISysSernum^.SernumGetPLCInfo $ P_SernumGetPLCInfo()

#endif
