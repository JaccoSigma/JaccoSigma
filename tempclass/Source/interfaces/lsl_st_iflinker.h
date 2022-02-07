//****************************************************************************/
//*                                                                          */
//*  File: LSL_ST_IFLINKER.H                                                 */
//*  Date: JAN 29, 2001                                                      */
//*  By:                                                                     */
//*  Description:                                                            */
//*		This file contains the LASAL OS System Linker Routine Interface      */
//*  Revision History:                                                       */
//*      Rev     By      Date        Description                             */
//*      ---     --      ----        -----------                             */
//*      1.10    MENK    29/01/01    Initial Version                         */
//*                                                                          */
//****************************************************************************/
#ifndef LSL_ST_IFLINKER_H
#pragma once
#define LSL_ST_IFLINKER_H

#include ".\OS Interface\LinkerTypes.h"

//
//	Linker Error Message Length
//
//#define LNK_ERRORMSG_LEN 132



// Function prototype for 
FUNCTION GLOBAL __cdecl P_LNK_GetFirstError
VAR_INPUT
	dest0		: pVoid;
END_VAR
VAR_OUTPUT 
	retval0	: UDINT;
END_VAR;

// Function prototype for 
FUNCTION GLOBAL __cdecl P_LNK_GetNextError
VAR_INPUT
	dest0		: pVoid;
END_VAR
VAR_OUTPUT 
	retval0	: UDINT;
END_VAR;

// Function prototype for 
FUNCTION GLOBAL __cdecl P_LNK_GetAddress_Function
VAR_INPUT
	fctname0: ^CHAR;
	isexe0  : ^CHAR;
END_VAR
VAR_OUTPUT 
	retval0	: ^void;
END_VAR;


// Function prototype for 
FUNCTION GLOBAL __cdecl P_LNK_GetAddress_Variable
VAR_INPUT
	varname0: ^CHAR;
END_VAR
VAR_OUTPUT 
	retval0	: UDINT;
END_VAR;

// Function prototype for 
FUNCTION GLOBAL __cdecl P_LNK_GetVariableInfo
VAR_INPUT
	varname		: ^CHAR;
	info0	    : ^LSLLNK_VariableInfo;
END_VAR
VAR_OUTPUT 
	retval0   : DINT;
END_VAR;

// Function prototype for 
FUNCTION GLOBAL __cdecl P_LNK_GetDataMemoryAddress
VAR_OUTPUT 
	retval0	: UDINT;
END_VAR;

// Function prototype for 
FUNCTION GLOBAL __cdecl P_LNK_GetCodeMemoryAddress
VAR_OUTPUT 
	retval0	: UDINT;
END_VAR;

// Function prototype for 
FUNCTION GLOBAL __cdecl P_LNK_GetProjectState
VAR_OUTPUT 
	retval0	: LSLLNK_ProjectState;
END_VAR;

//unsigned long crc32(unsigned long crc, void *buf, unsigned long len);
FUNCTION GLOBAL __cdecl P_CRC32
VAR_INPUT
	CRC0		: UDINT;
	buf0	    : pVOID;
	len0        : UDINT;
END_VAR
VAR_OUTPUT 
	retval0   : UDINT;
END_VAR;

//unsigned long LNK_AddError2List(unsigned long nr, char* str0);
FUNCTION GLOBAL __cdecl P_LNK_Add2ErrorList
VAR_INPUT
	nr		: UDINT;
	str0    : ^CHAR;
END_VAR
VAR_OUTPUT 
	retval0   : UDINT;
END_VAR;

//void LNK_FreeErrorList(void);
FUNCTION GLOBAL __cdecl P_LNK_FreeErrorList;

//unsigned long LNK_GetErrorCount();
FUNCTION GLOBAL __cdecl P_LNK_GetErrorCount
VAR_OUTPUT 
	retval0   : UDINT;
END_VAR;

//unsigned long LNK_ReturnChkByModulID(unsigned long ID);
FUNCTION GLOBAL __cdecl P_LNK_GetChkByModulID
VAR_INPUT
	ID		: UDINT;
END_VAR
VAR_OUTPUT 
	Chk     : UDINT;
END_VAR;

//unsigned long LNK_ReturnPrjChk(void);
FUNCTION GLOBAL __cdecl P_LNK_GetPrjChk
VAR_OUTPUT 
	Chk     : UDINT;
END_VAR;

//unsigned long LNK_ReturnPrjChk_WithModOrder(void);
FUNCTION GLOBAL __cdecl P_LNK_GetPrjChk_WithModOrder
VAR_OUTPUT 
	Chk     : UDINT;
END_VAR;

FUNCTION GLOBAL __cdecl P_LNK_XCH_PTRS
VAR_INPUT
	ptr0    : pVoid;
END_VAR
VAR_OUTPUT 
	ret0    : pVoid;
END_VAR;

//unsigned long LNK_GetVarChk(void);
FUNCTION GLOBAL __cdecl P_LNK_GetVarChk
VAR_OUTPUT 
	Chk     : UDINT;
END_VAR;

FUNCTION GLOBAL __cdecl P_LNK_GetChkSumFromClassName
VAR_INPUT
	ClassName : ^CHAR;
END_VAR
VAR_OUTPUT
	retval    : UDINT;
END_VAR;

FUNCTION GLOBAL __cdecl P_LNK_GetChkSumByType
VAR_INPUT
	modTypeMask : UDINT;
	modTypeEq   : UDINT;
	cmtMod      : ^UDINT;
END_VAR
VAR_OUTPUT
	crc         : UDINT;
END_VAR;

//
//	LINKER Interface Macros
//
#define OS_LNK_GetFirstError(p1)          _LSL_POS^.piLINKER^.LNK_GetFirstError        $ P_LNK_GetFirstError(p1)
#define OS_LNK_GetNextError(p1)           _LSL_POS^.piLINKER^.LNK_GetNextError         $ P_LNK_GetNextError(p1)
#define OS_LNK_GetAddress_Function(p1,p2) _LSL_POS^.piLINKER^.LNK_GetAddress_Function  $ P_LNK_GetAddress_Function(p1,p2)
#define OS_LNK_GetAddress_Variable(p1)    _LSL_POS^.piLINKER^.LNK_GetAddress_Variable  $ P_LNK_GetAddress_Variable(p1)
#define OS_LNK_GetVariableInfo(p1,p2)     _LSL_POS^.piLINKER^.LNK_GetVariableInfo      $ P_LNK_GetVariableInfo(p1,p2)
#define OS_LNK_GetDataMemoryAddress()     _LSL_POS^.piLINKER^.LNK_GetDataMemoryAddress $ P_LNK_GetDataMemoryAddress()
#define OS_LNK_GetCodeMemoryAddress()     _LSL_POS^.piLINKER^.LNK_GetCodeMemoryAddress $ P_LNK_GetCodeMemoryAddress()
#define OS_LNK_GetProjectState()          _LSL_POS^.piLINKER^.LNK_GetProjectState      $ P_LNK_GetProjectState()
#define OS_LNK_LookupEmbedded             _LSL_POS^.piLINKER^.LNK_LookupEmbedded
// V3.18: Add macro to set ObjDscList pointer
#define OS_LNK_GetObjDscList              _LSL_POS^.piLINKER^.LNK_GetObjDscList


// V5.12: Runtime Modul Download by Menk
#define OS_LNK_PrepareLink		  _LSL_POS^.piLINKER^.LNK_PrepareLink
#define OS_LNK_ApplyLink		  _LSL_POS^.piLINKER^.LNK_ApplyLink


// RTKernel V3.41
#define OS_CRC32(p1,p2,p3)                _LSL_POS^.piLINKER^.LNK_CRC32                $ P_CRC32(p1,p2,p3)
// RTKernel V3.45
#define OS_LNK_Add2ErrorList(p1,p2)       _LSL_POS^.piLINKER^.LNK_Add2ErrorList        $ P_LNK_Add2ErrorList(p1,p2)
#define OS_LNK_FreeErrorList()            _LSL_POS^.piLINKER^.LNK_FreeErrorList        $ P_LNK_FreeErrorList()
#define OS_LNK_GetErrorCount()            _LSL_POS^.piLINKER^.LNK_GetErrorCount        $ P_LNK_GetErrorCount()

#define OS_LNK_GetNextChannel             _LSL_POS^.piLINKER^.LNK_GetNextChannel
#define OS_LNK_GetObjectName              _LSL_POS^.piLINKER^.LNK_GetObjectName

#define OS_LNK_GetChkByModulID(p1)      _LSL_POS^.piLINKER^.LNK_GetChkByModulID          $ P_LNK_GetChkByModulID(p1)
#define OS_LNK_GetPrjChk()              _LSL_POS^.piLINKER^.LNK_GetChecksum              $ P_LNK_GetPrjChk()
#define OS_LNK_GetPrjChk_WithModOrder() _LSL_POS^.piLINKER^.LNK_GetChecksum_WithModOrder $ P_LNK_GetPrjChk_WithModOrder()

#define OS_LNK_XCH_PTRS(p1)            _LSL_POS^.piLINKER^.LNK_xch_ptrs                $ P_LNK_XCH_PTRS(p1)

#define OS_LNK_GetVarChk()              _LSL_POS^.piLINKER^.LNK_GetVarChecksum              $ P_LNK_GetVarChk()
// RTKernel 01.02.234
#define OS_LNK_GetChkSumFromClassName(p1) _LSL_POS^.piLINKER^.LNK_GetChkSumFromClassName      $ P_LNK_GetChkSumFromClassName(p1)
// RTKernel 01.03.080
#define OS_LNK_GetChkSumByType(p1,p2,p3)     _LSL_POS^.piLINKER^.LNK_GetChkSumByType      $ P_LNK_GetChkSumByType(p1,p2,p3)

#endif  // end of #define LSL_ST_IFLINKER_H
