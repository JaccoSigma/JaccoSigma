#ifndef SDIAS_SAFETYMANAGER_H
#pragma once
#define SDIAS_SAFETYMANAGER_H

//*****************************************************************************
//** NEWINST COMMANDS (Range reserved for SDIAS SAFETY: 0x8050 - 0x806F)     **
//*****************************************************************************

#define SDIAS_SAFETY_LOGIN                  16#8050  // login a SdiasSafetyBase object to an SdiasSafetyManager
#define SDIAS_SAFETY_LOGIN_HBG              16#8051  // login a HBG object to an SdiasSafetyManager
#define SDIAS_SET_CLASS_STATE               16#8052  // the SdiasSafetyManager sets a new classstate at SdiasSafetyBase object
#define SDIAS_SET_NUMBERS                   16#8053  // sets the serial and safety number
#define SDIAS_SET_RT_DATA                   16#8054  // sets the states of LEDS and fast unsafe inputs of the outer class
#define SDIAS_SET_SAFETY_STATE              16#8055  // sets the server safety state if the state changes                
#define SDIAS_SET_ASY_STATE                 16#8056  // sets the server values which have been changed in lower priority than realtime
#define SDIAS_SET_RETRY_COUNTER             16#8057  // sets the server retrycounteruc1 and retrycounteruc2.
#define SDIAS_RETRY_COUNTER_NOT_AVAILABLE   16#8058  // sets the server retrycounteruc1 and retrycounteruc2 -1
#define SDIAS_SET_QUITCOMERROR_IO           16#8059  // sets QuitComError from connected modules
#define SDIAS_SAFETY_LOGIN_HGW              16#805A  // login a HGW object to an SdiasSafetyManager
      //aPara[0] = version of command
      
      //Parameter of Version 1:
      //aPara[1] = this pointer
      //aPara[2] = required (0 .. no, 1 .. yes)

      //retcode = error => command not supported
      //retcode = ready => result is filled with data
      // result of version 1
      //result.aData[0x0]       = version

#define SDIAS_SAFETY_GET_HGW_INFO           16#805B  // get information from HGW (slot id, communication id)
      //aPara[0] = version of command

      //Parameter of Version 1:
      //aPara[1] = //pointer to PDO RD data in appli mem -- buffer to scp

      //retcode = error => command not supported
      //retcode = ready => result is filled with data
      // result of version 1
      //result.aData[0x0]       = version
      //result.aData[1]$UDINT  := pointer to PDO WD data in appli mem -- buffer to module
      //result.aData[5]$UINT   := InputPDOLength;  //data to scp
      //result.aData[7]$UINT   := OutputPDOLength; //data to hgw
      //result.aData[9]$UINT   := SlotID;
      //result.aData[11]$UINT  := CommunicationID;
      //result.aData[13]$USINT := Machinenumber;
      //result.aData[14]$USINT := length of compressed hw path
      //result.aData[15]$UDINT := pointer to compressed hw path
      //result.aData[19]$UDINT := safetynumber of scp
      //result.aData[23]$UDINT := pointer to errorstate server

//-------------------------------------------------------
// commands for library classes to get data
//-------------------------------------------------------

//+++++++++++++++++++++++++++++++++++++
#define SDIAS_SAFETY_GET_INFO_PLACE       0x806A

      //aPara -> not used
      
      //retcode = ready => result is filled with data
      //result.uiLng = 2
      //result.aData[0x0]$UINT  = value of client place (valid: 0 - 16 or DEACTIVATED_LSL)
//+++++++++++++++++++++++++++++++++++++


//*****************************************************************************
//** other defines                                                           **
//*****************************************************************************

#define SDIAS_SAFETY_MAX_MODULE_NO           16
#define SDIAS_SAFETY_MAX_PLACE_NR            SDIAS_SAFETY_MAX_MODULE_NO-1
#define SDIAS_SAFETY_INVALID_MAX_PLACE       16#80000000
#define SDIAS_SAFETY_INVALID_DEVICE_ID       0  // used to detect if there is a hardware connected (invalid = no hardware)
#define SDIAS_SAFETY_FAST_UNSAFE_KENNUNG     16#ACDC20C5
#define SDIAS_SAFETY_INVALID_IO_NOHW         -1;
#define SDIAS_SAFETY_PLL_OFFSET              16#140
#define SDIAS_SAFTEY_SDO_RESPONSE_TIMEOUT    5000
#define SDIAS_SAFTEY_SDO_MAX_DATA_PER_PAKET  56
#define SDIAS_SAFETY_MAX_SDO_DATA_PER_CYCLE  32 // use smaller blocks for SDOs

// bus ID for HBGs
#define SDIAS_SAFETY_MODULE_ID_CAN_BUS_MASK  16#40

// mask to get bus ID
#define SDIAS_SAFETY_MODULE_ID_BUS_MASK      16#C0

// id of blank module (if optional modules are in use)
#define SDIAS_SAFETY_MODULE_ID_BLANK_MODULE  16#31

//*****************************************************************************
//** TYPES                                                                   **
//*****************************************************************************

	TYPE
#pragma pack(push, 1)
    t_RtRefresh                 : STRUCT
	    LEDRunState               : DINT;
      LEDServiceMode            : DINT;
      LEDErrorState             : DINT;
      LEDValidationButtonState  : DINT;
      FastUnsafeOutputs         : BDINT;
	  END_STRUCT;    
    t_AsyRefresh                : STRUCT
	    QuitComError              : DINT;
	  END_STRUCT;    
    t_FastUnsafeOuts            : STRUCT
	    Kennung                   : HDINT;
      pThis                     : pVirtualBase;
	  END_STRUCT;    
#pragma pack(pop)
  END_TYPE 

#endif
