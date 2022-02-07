//****************************************************************************/
//*                                                                          */
//*  File: LSL_ST_IFSSR.H                                                    */
//*  Date: JAN 26, 2001                                                      */
//*  Description:                                                            */
//*		This file contains the LASAL OS System Service Routine Interface     */
//*  Revision History:                                                       */
//*      Rev     By      Date        Description                             */
//*      ---     --      ----        -----------                             */
//*      1.10    MENK    26/01/01    Initial Version                         */
//*                                                                          */
//****************************************************************************/
//* $$!ENDCMD */
#ifndef LSL_IFSSR_H
#pragma once
#define LSL_IFSSR_H


#include ".\OS Interface\TimeTypes.h"
#include ".\OS Interface\StorageTypes.h"

  TYPE

	  LSLDATETIME		: STRUCT
	    wYear        : UINT; 
	    wMonth       : UINT; 
	    wDay         : UINT; 
	    wDayOfWeek   : UINT;
	    wHour        : UINT; 
	    wMinute      : UINT; 
	    wSecond      : UINT;
      wMillisecond : UINT;
	  END_STRUCT;

    LSLTIMESTAMP : STRUCT
      secondPart : DINT;
      seconds : UDINT;
    END_STRUCT;

  END_TYPE



// Function prototype for SSR_InstallIRQ()
FUNCTION GLOBAL __cdecl P_SSR_InstallIRQ 
VAR_INPUT
	IRQNumber0    : UINT;
	IRQFunction0  : pVoid;
END_VAR
VAR_OUTPUT 
	ret0          : DINT;
END_VAR;

// Function prototype for SSR_InstallIRQLASAL()
FUNCTION GLOBAL __cdecl P_SSR_InstallIRQLASAL 
VAR_INPUT
	IRQNumber0    : UINT;
	IRQFunction0  : pVoid;
	thispointer0  : pVoid;
END_VAR
VAR_OUTPUT 
	ret0          : DINT;
END_VAR;

// Function prototype for SSR_CreateMailbox()
FUNCTION GLOBAL __cdecl P_SSR_CreateMailbox 
VAR_INPUT
  MailboxName0   : ^CHAR;
  MailboxDataLen0: UDINT;
  MailboxSlots0  : UDINT;
END_VAR
VAR_OUTPUT 
	ret0          : pVoid;
END_VAR;

// Function prototype for SSR_MailboxGet()
FUNCTION GLOBAL __cdecl P_SSR_MailboxGet 
VAR_INPUT
	Mailbox0      : pVoid;
	pMailboxData0 : pVoid;
END_VAR
VAR_OUTPUT 
	ret0          : DINT;
END_VAR;

// Function prototype for SSR_MailboxPut()
FUNCTION GLOBAL __cdecl P_SSR_MailboxPut 
VAR_INPUT
	Mailbox0      : pVoid;
	pMailboxData0 : pVoid;
END_VAR
VAR_OUTPUT 
	ret0          : DINT;
END_VAR;

// Function prototype for SSR_MailboxDelete()
FUNCTION GLOBAL __cdecl P_SSR_MailboxDelete 
VAR_INPUT
	Mailbox0      : pVoid;
END_VAR
VAR_OUTPUT 
	ret0          : DINT;
END_VAR;


/////////////////////////////////////////////////////////
// Function prototype for SSR_CreateSemaphore()
FUNCTION GLOBAL __cdecl P_SSR_CreateSemaphore
VAR_INPUT
  SemName0   : ^CHAR;
  SemType0   : UINT;
END_VAR
VAR_OUTPUT 
	ret0          : pVoid;
END_VAR;

// Function prototype for SSR_SemSignal()
FUNCTION GLOBAL __cdecl P_SSR_SemSignal
VAR_INPUT
	Semaphore0      : pVoid;
END_VAR
VAR_OUTPUT 
	ret0          : DINT;
END_VAR;

// Function prototype for SSR_SemPulse()
FUNCTION GLOBAL __cdecl P_SSR_SemPulse
VAR_INPUT
	Semaphore0      : pVoid;
END_VAR
VAR_OUTPUT 
	ret0          : DINT;
END_VAR;

// Function prototype for SSR_SemWait()
FUNCTION GLOBAL __cdecl P_SSR_SemWait
VAR_INPUT
	Semaphore0      : pVoid;
END_VAR
VAR_OUTPUT 
	ret0          : DINT;
END_VAR;

// Function prototype for SSR_SemWaitCond()
FUNCTION GLOBAL __cdecl P_SSR_SemWaitCond
VAR_INPUT
	Semaphore0      : pVoid;
END_VAR
VAR_OUTPUT 
	ret0          : DINT;
END_VAR;

// Function prototype for SSR_SemDelete()
FUNCTION GLOBAL __cdecl P_SSR_SemDelete
VAR_INPUT
	Semaphore0      : pVoid;
END_VAR
VAR_OUTPUT 
	ret0          : DINT;
END_VAR;

// Function prototype for SSR_CurrentTaskHandle()
FUNCTION GLOBAL __cdecl P_SSR_CurrentTaskHandle 
VAR_OUTPUT 
	ret0          : UDINT;
END_VAR;



/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function prototype for SSR_OutW()
FUNCTION GLOBAL __cdecl P_SSR_OutW
VAR_INPUT
	PortNumber0  : UINT;
	Value0       : UINT;
END_VAR;

// Function prototype for SSR_OutB()
FUNCTION GLOBAL __cdecl P_SSR_OutB
VAR_INPUT
	PortNumber0  : UINT;
	Value0       : UINT;
END_VAR;

// Function prototype for SSR_InpW()
FUNCTION GLOBAL __cdecl P_SSR_InpW
VAR_INPUT
	PortNumber0  : UINT;
END_VAR
VAR_OUTPUT 
	ret0         : UINT;
END_VAR;

// Function prototype for SSR_InpB()
FUNCTION GLOBAL __cdecl P_SSR_InpB
VAR_INPUT
	PortNumber0  : UINT;
END_VAR
VAR_OUTPUT 
	ret0         : UINT;
END_VAR;

// ...............................................................................................
// ... function to give user mem read/write access
// ...............................................................................................
FUNCTION GLOBAL __cdecl P_SSR_MemoryAccess
VAR_INPUT
	Addr0    : UDINT;
	Length0  : UDINT;
END_VAR
VAR_OUTPUT 
	ret0     : DINT;
END_VAR;



//short int SSR_CanTxObj(unsigned char cannr , short int objnr , unsigned char length, void* data)
FUNCTION GLOBAL __cdecl P_SSR_CanTxObj
VAR_INPUT
	cannr0    : USINT;
	objnr0    : DINT;
	length0   : USINT;
    data0     : PVOID;
END_VAR
VAR_OUTPUT 
	ret0     : INT;
END_VAR;


//int SSR_AddCanObj(unsigned char cannr , short int objnr, unsigned char length, unsigned char mode, void* actionptr)
FUNCTION GLOBAL __cdecl P_SSR_AddCanObj
VAR_INPUT
	cannr0     : USINT;
	objnr0     : DINT;
	length0    : USINT;
	mode0      : USINT;
  actionptr0 : PVOID;
  thisptr0 : PVOID;
END_VAR
VAR_OUTPUT 
	ret0     : DINT;
END_VAR;



//pVoid SSR_Malloc(unsigned long size0)
FUNCTION GLOBAL __cdecl P_SSR_Malloc
VAR_INPUT
  size0   : UDINT;
END_VAR
VAR_OUTPUT 
  ret0    : PVOID;
END_VAR;

//Function SSR_Free(void *ptr0);
FUNCTION GLOBAL __cdecl P_SSR_Free
VAR_INPUT
  ptr0     : PVOID;
END_VAR;


//functions for Time-management

//	void SSR_SetSysTime(SYSTIME* time0);
FUNCTION GLOBAL __cdecl P_SSR_SetSysTime
VAR_INPUT
  time0     : ^SYSTIME;
END_VAR;

//  void SSR_SetSysDate(SYSDATE* date0);
FUNCTION GLOBAL __cdecl P_SSR_SetSysDate
VAR_INPUT
  date0     : ^SYSDATE;
END_VAR;

//  void SSR_GetSysTime(SYSTIME* time0);
FUNCTION GLOBAL __cdecl P_SSR_GetSysTime
VAR_INPUT
  time0     : ^SYSTIME;
END_VAR;

//  void SSR_GetSysDate(SYSDATE* date0);
FUNCTION GLOBAL __cdecl P_SSR_GetSysDate
VAR_INPUT
  date0     : ^SYSDATE;
END_VAR;

//void* SSR_ReAlloc(void *mptr0, unsigned long size0)
FUNCTION GLOBAL __cdecl P_SSR_ReAlloc
VAR_INPUT
  ptr0     : PVOID;
  size0    : UDINT;
END_VAR
VAR_OUTPUT 
  ret0    : PVOID;
END_VAR;

//void MemMove(void *d, void *s, unsigned long size);
FUNCTION GLOBAL __cdecl P_SSR_MemMove
VAR_INPUT
  dest0      : PVOID;
  source0    : PVOID;
  size0      : UDINT;
END_VAR
VAR_OUTPUT
  pret : PVOID;
END_VAR;

//void MemCpy(void *d, void *s, unsigned long size);
FUNCTION GLOBAL __cdecl P_SSR_MemCpy
VAR_INPUT
  dest0      : PVOID;
  source0    : PVOID;
  size0      : UDINT;
END_VAR
VAR_OUTPUT
  pret : PVOID;
END_VAR;


//void MemSet(void *p, unsigned char c, unsigned long size);
FUNCTION GLOBAL __cdecl P_SSR_MemSet
VAR_INPUT
  ptr0      : PVOID;
  char0     : USINT;
  size0      : UDINT;
END_VAR
VAR_OUTPUT
  pret : PVOID;
END_VAR;


//unsigned short MemCmp(void *d, void *s, unsigned long size) ;
FUNCTION GLOBAL __cdecl P_SSR_MemCmp
VAR_INPUT
  dest0      : PVOID;
  source0    : PVOID;
  size0      : UDINT;
END_VAR
VAR_OUTPUT
  pret : UINT;
END_VAR;



// SSR_LoginIntoCANNew(unsigned char cannr0,unsigned char canstation0, short int txobjnr0);
FUNCTION GLOBAL __cdecl P_SSR_LoginIntoCANNew
VAR_INPUT
  cannr0      : USINT;
  canstation0 : USINT;
  txobjnr0    : UINT;
END_VAR
VAR_OUTPUT 
  ret0        : UINT;
END_VAR;

// void SSR_Delay(unsigned long msecs0)
FUNCTION GLOBAL __cdecl P_SSR_Delay
VAR_INPUT
  msecs0    : UDINT;
END_VAR;


// unsigned long SSR_EE_Read(unsigned short addr0, unsigned short *data0)
FUNCTION GLOBAL __cdecl P_EE_Read
VAR_INPUT
  addr0    : UINT;
  data0    : ^UINT;
END_VAR
VAR_OUTPUT 
  ret0    : UDINT;
END_VAR;

// unsigned long SSR_EE_Write(unsigned short addr0, unsigned short data0)
FUNCTION GLOBAL __cdecl P_EE_Write
VAR_INPUT
  addr0    : UINT;
  data0    : UINT;
END_VAR
VAR_OUTPUT 
  ret0    : UDINT;
END_VAR;

// unsigned long SSR_ButtonPressed(void)
FUNCTION GLOBAL __cdecl P_ButPressed
VAR_OUTPUT 
  ret0    : UDINT;
END_VAR;

// unsigned long SSR_SetTimerIntVal(void)
FUNCTION GLOBAL __cdecl P_SetTimerIntVal
VAR_INPUT
  Interval0 : UDINT;
END_VAR
VAR_OUTPUT 
  ret0    : UDINT;
END_VAR;

// unsigned long SSR_GetTimerIntVal(void)
FUNCTION GLOBAL __cdecl P_GetTimerIntVal
VAR_OUTPUT 
  ret0    : UDINT;
END_VAR;


FUNCTION GLOBAL __cdecl P_InstallDiasHandler
VAR_INPUT
  fct0    : pVoid;
  param0  : pVoid;
END_VAR;

// void  (* AddToKernelServiceProvider)(char *cmd0, unsigned long immedialty0);
FUNCTION GLOBAL __cdecl P_AddToKernelSP
VAR_INPUT
  cmd0    : ^USINT;
  immedialty0: UDINT;
END_VAR
//not supported 10.10.2002
//5.59f supported again
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;




///////////////////////////////////////////////////////////////////////////////////////////////
// PROM Routines
///////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
// EPROM on Dias PLC Socket => Dias Station 62
// long (* PROMGetInfo_Socket)(LSLPROMINFO_TYPE *info0);
FUNCTION GLOBAL __cdecl P_PROMGetInfo_Socket
VAR_INPUT
  info0    : ^LSLPROMINFO_TYPE;
END_VAR
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;
// long (* PROMSetData_Socket)(unsigned long addr0, void *data0, unsigned long datalen0);
FUNCTION GLOBAL __cdecl P_PROMSetData_Socket
VAR_INPUT
  addr0    : UDINT;
  data0    : pVoid;
  datalen0 : UDINT;
END_VAR
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;
// long (* PROMGetData_Socket)(void *data0, unsigned long addr0, unsigned long datalen0);
FUNCTION GLOBAL __cdecl P_PROMGetData_Socket
VAR_INPUT
  data0    : pVoid;
  addr0    : UDINT;
  datalen0 : UDINT;
END_VAR
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;


//-------------------------------------------------------------------------------
// EPROM on DiasBus as DIAS Modul => Dias Station 61
// long (* PROMGetInfo_Modul)(LSLPROMINFO_TYPE *info0);
FUNCTION GLOBAL __cdecl P_PROMGetInfo_Modul
VAR_INPUT
  info0    : ^LSLPROMINFO_TYPE;
END_VAR
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;
// long (* PROMSetData_Modul)(unsigned long addr0, void *data0);
FUNCTION GLOBAL __cdecl P_PROMSetData_Modul
VAR_INPUT
  addr0    : UDINT;
  data0    : pVoid;
END_VAR
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;
// long (* PROMGetData_Modul)(void *data0, unsigned long addr0);
FUNCTION GLOBAL __cdecl P_PROMGetData_Modul
VAR_INPUT
  data0    : pVoid;
  addr0    : UDINT;
END_VAR
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;

//    int InitBasicCanObj(unsigned char cannr , short int objnr, 
//			       short int mask,void* actionptr, void* thispointer);  
FUNCTION GLOBAL __cdecl P_InitBasicCanObj
VAR_INPUT
  cannr    : USINT;
  objnr    : INT;
  mask     : INT;
  actionptr : pVoid;
  thisptr  : pVoid;
END_VAR
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;

// int   (* SetHandler)(int type, void *fct, void *param);
FUNCTION GLOBAL __cdecl P_SetHandler
VAR_INPUT
  handlerType   : DINT;
  pFct    : pVoid;
  param   : pVoid;
END_VAR
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;

// int   (* SkipOverIDIV)(unsigned long exceptionCode,
//                        void *exceptionInfo,
//                        unsigned long quotient,
//                        unsigned long remainder);
FUNCTION GLOBAL __cdecl P_SkipOverIDIV
VAR_INPUT
  exceptionCode : UDINT;
  exceptionInfo : pVoid;
  quotient : UDINT;
  remainder : UDINT;
END_VAR
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;

FUNCTION GLOBAL __cdecl P_GetDivErrorInfo
VAR_INPUT
  exceptionInfo   : pVoid;
  dividend        : ^UDINT;
  dividendHigh    : ^UDINT;
END_VAR
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;

FUNCTION GLOBAL __cdecl P_GetModuleInfoByIp
VAR_INPUT
  instPtr   : ^UDINT;
  ModuleName    : ^USINT;
  ModuleNameSize   : UDINT;
  FuncName    : ^USINT;
  FuncNameSize   : UDINT;
  OffsetInModule  : ^UDINT;                
END_VAR
VAR_OUTPUT 
  ret0    : DINT;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_MallocV1
VAR_INPUT
  size0   : UDINT;
  mark0   : USINT;
END_VAR
VAR_OUTPUT 
  ret0    : PVOID;
END_VAR;
  
FUNCTION GLOBAL __cdecl P_SSR_ReAllocV1
VAR_INPUT
  ptr0     : PVOID;
  size0    : UDINT;
  mark0    : USINT;
END_VAR
VAR_OUTPUT 
  ret0    : PVOID;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_MarkAllocV1
VAR_INPUT
  ptr0    : PVOID;
  size0   : UDINT;
  mark0   : USINT;
END_VAR
VAR_OUTPUT 
  ret0    : PVOID;
END_VAR;
  
FUNCTION GLOBAL __cdecl P_SSR_ShrinkAllocV1
VAR_INPUT
  ptr0    : PVOID;
  size0   : UDINT;
END_VAR
VAR_OUTPUT 
  ret0    : PVOID;
END_VAR;
  

// ab 1.1.79
FUNCTION GLOBAL __cdecl P_SSR_Reboot
VAR_INPUT
END_VAR
VAR_OUTPUT 
END_VAR;

// ab 1.1.80
FUNCTION GLOBAL __cdecl P_SSR_ReadDateTime
VAR_INPUT
  pDateTime : ^LSLDATETIME;
END_VAR
VAR_OUTPUT 
  retVal : DINT;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_SetDateTime
VAR_INPUT
  pDateTime : ^LSLDATETIME;
END_VAR
VAR_OUTPUT 
  retVal : DINT;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_DateTime2Timestamp
VAR_INPUT
  pDateTime : ^LSLDATETIME;
  pTimestamp : ^LSLTIMESTAMP;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_Timestamp2DateTime
VAR_INPUT
  pDateTime : ^LSLDATETIME;
  pTimestamp : ^LSLTIMESTAMP;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_AdjustDateTime
VAR_INPUT
  pTimestampDiff : ^LSLTIMESTAMP;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_CompareDateTime
VAR_INPUT
  pDateTime1 : ^LSLDATETIME;
  pDateTime2 : ^LSLDATETIME;
  pTimestampDiff : ^LSLTIMESTAMP;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_TimestampAdd
VAR_INPUT
  pTsSumme : ^LSLTIMESTAMP;
  pTsSummand1 : ^LSLTIMESTAMP;
  pTsSummand2 : ^LSLTIMESTAMP;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_TimestampSub
VAR_INPUT
  pTsDifferenz : ^LSLTIMESTAMP;
  pTsMinuend : ^LSLTIMESTAMP;
  pTsSubtrahend : ^LSLTIMESTAMP;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_TimestampDiv
VAR_INPUT
  pTsQuotient : ^LSLTIMESTAMP;
  pTsDividend : ^LSLTIMESTAMP;
  divisor : UDINT;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_TimestampMult
VAR_INPUT
  pTsProdukt : ^LSLTIMESTAMP;
  pTsFaktor : ^LSLTIMESTAMP;
  faktor : UDINT;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_SetBreakPoint
VAR_INPUT
  nr : DINT;
  addr : UDINT;
  len : UDINT;
  bpType : DINT;
  stackOfs : UDINT;
  stackSize : UDINT;
  cntr : UDINT;
  action_ : DINT;
  pCode : pVoid;
  codeLen : UDINT;
  allowDataBpInSystemtask : DINT;
END_VAR
VAR_OUTPUT
  retVal : UDINT;
END_VAR;

FUNCTION GLOBAL __cdecl P_SSR_HeapOutOfMemSetCallback
VAR_INPUT
  pThis : PVOID;
  pCallback : PVOID;
END_VAR
VAR_OUTPUT
  retVal : DINT;
END_VAR;


//
//	IISR Interface Macros
//
// Function prototype for ISR_MailboxGet()

#define OS_SSR_InstallIRQ(p1,p2) _LSL_POS^.piSSR^.SSR_InstallIRQ $ P_SSR_InstallIRQ(p1,p2)
#define OS_SSR_InstallIRQObject(p1,p2) _LSL_POS^.piSSR^.SSR_InstallIRQObject $ P_SSR_InstallIRQ(p1,p2)

#define OS_SSR_CreateMailbox(p1,p2,p3) _LSL_POS^.piSSR^.SSR_CreateMailbox $ P_SSR_CreateMailbox(p1,p2,p3)
#define OS_SSR_MailboxGet(p1,p2) _LSL_POS^.piSSR^.SSR_MailboxGet $ P_SSR_MailboxGet(p1,p2)
#define OS_SSR_MailboxPut(p1,p2) _LSL_POS^.piSSR^.SSR_MailboxPut $ P_SSR_MailboxPut(p1,p2)
#define OS_SSR_MailboxDelete(p1) _LSL_POS^.piSSR^.SSR_MailboxDelete $ P_SSR_MailboxDelete(p1)

#define OS_SSR_CreateSemaphore(p1, p2) _LSL_POS^.piSSR^.SSR_CreateSemaphore $ P_SSR_CreateSemaphore(p1, p2)
#define OS_SSR_SemSignal(p1) _LSL_POS^.piSSR^.SSR_SemSignal $ P_SSR_SemSignal(p1)
#define OS_SSR_SemPulse(p1) _LSL_POS^.piSSR^.SSR_SemPulse $ P_SSR_SemPulse(p1)
#define OS_SSR_SemWait(p1) _LSL_POS^.piSSR^.SSR_SemWait $ P_SSR_SemWait(p1)
#define OS_SSR_SemWaitCond(p1) _LSL_POS^.piSSR^.SSR_SemWaitCond $ P_SSR_SemWaitCond(p1)
#define OS_SSR_SemDelete(p1) _LSL_POS^.piSSR^.SSR_SemDelete $ P_SSR_SemDelete(p1)

#define OS_SSR_CurrentTaskHandle() _LSL_POS^.piSSR^.SSR_CurrentTaskHandle $ P_SSR_CurrentTaskHandle()

#define OS_SSR_OutW(p1, p2) _LSL_POS^.piSSR^.SSR_OutW $ P_SSR_OutW(p1,p2)
#define OS_SSR_OutB(p1, p2) _LSL_POS^.piSSR^.SSR_OutB $ P_SSR_OutB(p1,p2)
#define OS_SSR_InpW(p1) _LSL_POS^.piSSR^.SSR_InpW $ P_SSR_InpW(p1)
#define OS_SSR_InpB(p1) _LSL_POS^.piSSR^.SSR_InpB $ P_SSR_InpB(p1)

#define OS_SSR_MemoryAccess(p1, p2) _LSL_POS^.piSSR^.SSR_MemoryAccess $ P_SSR_MemoryAccess(p1,p2)

#define OS_SSR_CanTxObj(p1,p2,p3,p4) _LSL_POS^.piSSR^.SSR_CanTxObj $ P_SSR_CanTxObj(p1,p2,p3,p4)
#define OS_SSR_AddCanObj(p1,p2,p3,p4,p5,p6) _LSL_POS^.piSSR^.SSR_AddCanObj $ P_SSR_AddCanObj(p1,p2,p3,p4,p5,p6)

#define OS_SSR_Malloc(p1) _LSL_POS^.piSSR^.SSR_Malloc $ P_SSR_Malloc(p1)
#define OS_SSR_Free(p1) _LSL_POS^.piSSR^.SSR_Free $ P_SSR_Free(p1)

#define OS_SSR_SetSysTime(p1) _LSL_POS^.piSSR^.SSR_SetSysTime $ P_SSR_SetSysTime(p1)
#define OS_SSR_SetSysDate(p1) _LSL_POS^.piSSR^.SSR_SetSysDate $ P_SSR_SetSysDate(p1)
#define OS_SSR_GetSysTime(p1) _LSL_POS^.piSSR^.SSR_GetSysTime $ P_SSR_GetSysTime(p1)
#define OS_SSR_GetSysDate(p1) _LSL_POS^.piSSR^.SSR_GetSysDate $ P_SSR_GetSysDate(p1)

#define OS_SSR_Realloc(p1,p2) _LSL_POS^.piSSR^.SSR_Realloc $ P_SSR_ReAlloc(p1,p2)

#define OS_SSR_InstallIRQLASAL(p1,p2,p3) _LSL_POS^.piSSR^.SSR_InstallIRQLASAL $ P_SSR_InstallIRQLASAL(p1,p2,p3)

#define OS_SSR_MemMove(p1,p2,p3) _LSL_POS^.piSSR^.pMemMove $ P_SSR_MemMove(p1,p2,p3)
#define OS_SSR_MemCpy(p1,p2,p3) _LSL_POS^.piSSR^.pMemCpy $ P_SSR_MemCpy(p1,p2,p3)
#define OS_SSR_MemSet(p1,p2,p3) _LSL_POS^.piSSR^.pMemSet $ P_SSR_MemSet(p1,p2,p3)
#define OS_SSR_MemCmp(p1,p2,p3) _LSL_POS^.piSSR^.pMemCmp $ P_SSR_MemCmp(p1,p2,p3)

#define OS_SSR_LoginIntoCANNew(p1,p2,p3) _LSL_POS^.piSSR^.SSR_LoginIntoCANNew $ P_SSR_LoginIntoCANNew(p1,p2,p3) 

#define OS_SSR_Delay(p1) _LSL_POS^.piSSR^.SSR_Delay $ P_SSR_Delay(p1)

#define OS_SSR_EE_Read(p1,p2) _LSL_POS^.piSSR^.EE_Read $ P_EE_Read(p1, p2)
#define OS_SSR_EE_Write(p1,p2) _LSL_POS^.piSSR^.EE_Write $ P_EE_Write(p1, p2)
#define OS_SSR_ButPressed() _LSL_POS^.piSSR^.ButtonPressed $ P_ButPressed()

#define OS_SSR_SetTimerIntVal(p1) _LSL_POS^.piSSR^.SetTimerIntVal $ P_SetTimerIntVal(p1)
#define OS_SSR_GetTimerIntVal() _LSL_POS^.piSSR^.GetTimerIntVal $ P_GetTimerIntVal()

#define OS_SSR_SetCompatLevel(p1) _LSL_POS^.piSSR^.SetCompatLevel $ P_SetCompatLevel(p1)

#define OS_SSR_InstallDiasHandler(p1,p2) _LSL_POS^.piSSR^.InstallDiasHandler $ P_InstallDiasHandler(p1,p2)

#define OS_SSR_AddToKernelSP(p1,p2) _LSL_POS^.piSSR^.AddToKernelSP $ P_AddToKernelSP(p1,p2)

#define OS_SSR_InitBasicCANObj(p1,p2,p3,p4,p5) _LSL_POS^.piSSR^.InitBasicCAN $ P_InitBasicCanObj(p1,p2,p3,p4,p5)

#define OS_SSR_SetHandler(p1,p2,p3) _LSL_POS^.piSSR^.SetHandler $ P_SetHandler(p1,p2,p3)
#define OS_SSR_SkipOverIDIV(p1,p2,p3,p4) _LSL_POS^.piSSR^.SkipOverIDIV $ P_SkipOverIDIV(p1,p2,p3,p4)

///////////////////////////////////////////////////////////////////////////////////////////////
// #defines for PROM Routines
///////////////////////////////////////////////////////////////////////////////////////////////
    // EPROM on Dias PLC Socket => Dias Station 62
#define OS_SSR_PROMGetInfo_Socket(p1) _LSL_POS^.piSSR^.PROMGetInfo_Socket $ P_PROMGetInfo_Socket(p1)
#define OS_SSR_PROMSetData_Socket(p1,p2,p3) _LSL_POS^.piSSR^.PROMSetData_Socket $ P_PROMSetData_Socket(p1,p2,p3)
#define OS_SSR_PROMGetData_Socket(p1,p2,p3) _LSL_POS^.piSSR^.PROMGetData_Socket $ P_PROMGetData_Socket(p1,p2,p3)
    // EPROM on DiasBus as DIAS Modul => Dias Station 61
#define OS_SSR_PROMGetInfo_Modul(p1) _LSL_POS^.piSSR^.PROMGetInfo_Modul $ P_PROMGetInfo_Modul(p1)
#define OS_SSR_PROMSetData_Modul(p1,p2) _LSL_POS^.piSSR^.PROMSetData_Modul $ P_PROMSetData_Modul(p1,p2)
#define OS_SSR_PROMGetData_Modul(p1,p2) _LSL_POS^.piSSR^.PROMGetData_Modul $ P_PROMGetData_Modul(p1,p2)

   // FLASHPROM ONBOARD 
#define OS_SSR_PROMGetInfo_Flash(p1) _LSL_POS^.piSSR^.LSLPROM_GetInfo_Flash $ P_PROMGetInfo_Socket(p1)
#define OS_SSR_PROMSetData_Flash(p1,p2,p3) _LSL_POS^.piSSR^.LSLPROM_SetData_Flash $ P_PROMSetData_Socket(p1,p2,p3)
#define OS_SSR_PROMGetData_Flash(p1,p2,p3) _LSL_POS^.piSSR^.LSLPROM_GetData_Flash $ P_PROMGetData_Socket(p1,p2,p3)


#define OS_SSR_MallocV1(p1,p2)    _LSL_POS^.piSSR^.SSR_MallocV1 $ P_SSR_MallocV1(p1,p2)
#define OS_SSR_ReallocV1(p1,p2,p3)  _LSL_POS^.piSSR^.SSR_ReallocV1 $ P_SSR_ReAllocV1(p1,p2,p3)
// 01.01.185
#define OS_SSR_MarkAllocV1(p1,p2,p3)    _LSL_POS^.piSSR^.SSR_MarkAllocV1 $ P_SSR_MarkAllocV1(p1,p2,p3)
#define OS_SSR_ShrinkAlloc(p1,p2)    _LSL_POS^.piSSR^.SSR_ShrinkAlloc $ P_SSR_ShrinkAllocV1(p1,p2)


// ab 1.1.79
#define OS_SSR_Reboot() _LSL_POS^.piSSR^.SSR_Reboot $ P_SSR_Reboot()

// ab 1.1.80
#define OS_SSR_ReadDateTime(p1)             _LSL_POS^.piSSR^.ReadDateTime $ P_SSR_ReadDateTime(p1)  
#define OS_SSR_SetDateTime(p1)              _LSL_POS^.piSSR^.SetDateTime $ P_SSR_SetDateTime(p1)
#define OS_SSR_DateTime2Timestamp(p1,p2)    _LSL_POS^.piSSR^.DateTime2Timestamp $ P_SSR_DateTime2Timestamp(p1,p2) 
#define OS_SSR_Timestamp2DateTime(p1,p2)    _LSL_POS^.piSSR^.Timestamp2DateTime $ P_SSR_Timestamp2DateTime(p1,p2)
#define OS_SSR_AdjustDateTime(p1)           _LSL_POS^.piSSR^.AdjustDateTime $ P_SSR_AdjustDateTime(p1)
#define OS_SSR_CompareDateTime(p1,p2,p3)    _LSL_POS^.piSSR^.CompareDateTime $ P_SSR_CompareDateTime(p1,p2,p3)
#define OS_SSR_TimestampAdd(p1,p2,p3)       _LSL_POS^.piSSR^.TimestampAdd $ P_SSR_TimestampAdd(p1,p2,p3)
#define OS_SSR_TimestampSub(p1,p2,p3)       _LSL_POS^.piSSR^.TimestampSub $ P_SSR_TimestampSub(p1,p2,p3)  
#define OS_SSR_TimestampDiv(p1,p2,p3)       _LSL_POS^.piSSR^.TimestampDiv $ P_SSR_TimestampDiv(p1,p2,p3)  
#define OS_SSR_TimestampMult(p1,p2,p3)      _LSL_POS^.piSSR^.TimestampMult $ P_SSR_TimestampMult(p1,p2,p3)  

// ab 1.1.123
#define OS_SSR_SetBreakPoint(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11)  _LSL_POS^.piSSR^.SetBreakPoint $ P_SSR_SetBreakPoint(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11)  

// ab 1.1.144
#define OS_SSR_HeapOutOfMemSetCallback(p1,p2)    _LSL_POS^.piSSR^.HeapOutOfMemSetCallback $ P_SSR_HeapOutOfMemSetCallback(p1,p2) 

// ab 1.1.156
#define OS_SSR_GetDivErrorInfo(p1,p2,p3) _LSL_POS^.piSSR^.GetDivErrorInfo $ P_GetDivErrorInfo(p1,p2,p3)
#define OS_SSR_GetModuleInfoByIp(p1,p2,p3,p4,p5,p6) _LSL_POS^.piSSR^.GetModuleInfoByIp $ P_GetModuleInfoByIp(p1,p2,p3,p4,p5,p6)

#define LSLPROMERROR_NONE          0
#define LSLPROMERROR_NODIAS       -1
#define LSLPROMERROR_IOADDR       -2
#define LSLPROMERROR_DEVID        -3
#define LSLPROMERROR_NOMOREMEM    -4
#define LSLPROMERROR_NOINFO       -5

#define LSLPROMERROR_ADDR        -10
#define LSLPROMERROR_PROG        -11
#define LSLPROMERROR_LENISNULL   -12
#define LSLPROMERROR_LENTOOSMALL -13

#define LSLPROMDEVCODE_128K      0xd5 //device code
#define LSLPROMDEVCODE_256K      0xda
#define LSLPROMDEVCODE_512K      0xa4
#define LSLPROMDEVCODE_1024K     0xFF
//**HH
// ist zwar ein Blödsinn, aber nicht besser zu lösen!
#define LSLPROMDEVCODE_2048K     0xFE

//
//
//	End of File
//
#endif  // end of #define LSL_IFSSR_H
