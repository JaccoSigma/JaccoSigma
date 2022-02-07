/////////////////////////////////////////////////////////////////////
//
// filename: lsl_st_sysinfo.h
// 
// category: osinterface
//
// about file: read system information
//
// department: ...
//
// contact person:
//
// created: 
//
// history:
//    v1.0 obejoh  initiator
//    v1.1 obejoh  enhancde group sysinfo
//
/////////////////////////////////////////////////////////////////////

#ifndef  __LSL_SYSINFO_H
#pragma once
#define  __LSL_SYSINFO_H

// Interface
#define INTERFACE_LSL_SYSINFO            "LSL_SYSINFO"

// defines for possible Error Codes
#define LSL_SYSINFO_ERR_NONE		    0		// no error
#define LSL_SYSINFO_ERR_ID			    -1	// Unknown ID for SYSINFO_GetProperty
#define LSL_SYSINFO_ERR_NOT_AVAIL	  -2	// Information for given ID not available on this CPU
#define LSL_SYSINFO_ERR_BUF	    		-3	// Invalid Buffer or Buffer length
#define LSL_SYSINFO_ERR_CIL        -99  // CIL not available

TYPE
  LSL_SYSINFO_ID :
  (
	// cpu 1..19
	LSL_SYSINFO_ID_CPU_PLATFORM_STR := 1,	// STRING: (ARM,X86,..)
	LSL_SYSINFO_ID_CPU_PLATFORM,			// DINT: ECpuPlatformX86=0, ECpuPlatformArm
	LSL_SYSINFO_ID_CPU_TYPE,				// DINT: ECpuTypeX86=0, ECpuTypeVortex, ECpuTypeArm, ECpuTypeArmV5, ECpuTypeArmV7
	LSL_SYSINFO_ID_CPU_COUNT,				// DINT: count of cpu core(s)
	LSL_SYSINFO_ID_CPU_CLOCK,				// DINT	(kHz)

	// platform 20..29
	LSL_SYSINFO_ID_PLATFORM_NAME_STR := 20,	// STRING: eeprom name of the CPU
	LSL_SYSINFO_ID_PLATFORM_TYPE,			// DINT: DESTPLC_...

	// DMI only on X86 systems
	LSL_SYSINFO_DMI_BIOS_DATE		 := 30,	// string:
	LSL_SYSINFO_DMI_BIOS_VENDOR,			// string:
	LSL_SYSINFO_DMI_BIOS_VERSION,			// string:
	LSL_SYSINFO_DMI_BOARD_NAME,				// string:
	LSL_SYSINFO_DMI_BOARD_SERIAL,			// string:
	LSL_SYSINFO_DMI_BOARD_VENDOR,			// string:
	LSL_SYSINFO_DMI_BOARD_VERSION,			// string:
	LSL_SYSINFO_DMI_CHASSIS_SERIAL,			// string:
	LSL_SYSINFO_DMI_CHASSIS_TYPE,			// string:
	LSL_SYSINFO_DMI_CHASSIS_VENDOR,			// string:
	LSL_SYSINFO_DMI_CHASSIS_VERSION,		// string:
	LSL_SYSINFO_DMI_PRODUCT_NAME,			// string:
	LSL_SYSINFO_DMI_PRODUCT_SERIAL,			// string:
	LSL_SYSINFO_DMI_PRODUCT_UUID,			// string:
	LSL_SYSINFO_DMI_PRODUCT_VERSION,		// string:
	LSL_SYSINFO_DMI_SYS_VENDOR,				// string:

	LSL_SYSINFO_ID_TEMPSENS0 := 50,			// DINT: temperature in milli Celsius
	LSL_SYSINFO_ID_TEMPSENS_BOARD0 := 50,	// DINT: temperature in milli Celsius
	LSL_SYSINFO_ID_TEMPSENS_BOARD1,			// DINT: temperature in milli Celsius
	LSL_SYSINFO_ID_TEMPSENS_BOARD2,			// DINT: temperature in milli Celsius
	
	LSL_SYSINFO_ID_TEMPSENS_CPU0 := 55,		// DINT: temperature in milli Celsius
	LSL_SYSINFO_ID_TEMPSENS_CPU1,			// DINT: temperature in milli Celsius
	LSL_SYSINFO_ID_TEMPSENS_CPU2,			// DINT: temperature in milli Celsius
	LSL_SYSINFO_ID_TEMPSENS_CPU3,			// DINT: temperature in milli Celsius
  
  // 60..69 Memory info
  LSL_SYSINFO_MEM_RAM_TOTAL := 60,      // uint32: kbyte
  LSL_SYSINFO_MEM_RAM_FREE,             // uint32: kbyte
  LSL_SYSINFO_MEM_OSHEAP_TOTAL,         // uint32: kbyte
  LSL_SYSINFO_MEM_OSHEAP_FREE,          // uint32: kbyte
  LSL_SYSINFO_MEM_APP_CODE,             // uint32: kbyte
  LSL_SYSINFO_MEM_APP_HEAP,             // uint32: kbyte
  LSL_SYSINFO_MEM_PREHEAP,              // uint32: kbyte
  LSL_SYSINFO_MEM_LAST,                 // last 67

	// 90..99 SD-Card (SWISSBIT) Information
	LSL_SYSINFO_SDCARD_VENDOR			:= 90,	// string: vendor name
	LSL_SYSINFO_SDCARD_MODEL,					// string: model name
	LSL_SYSINFO_SDCARD_FIRMWARE,				// string: firmware version string
	LSL_SYSINFO_SDCARD_MAX_AVERAGE_ERASE_CNT,	// UDINT: max limit of average erase count
	LSL_SYSINFO_SDCARD_AVERAGE_ERASE_CNT,		// UDINT: current average erase count
	LSL_SYSINFO_SDCARD_POWER_ON_CNT,			// UDINT: power on counter

	// 100..119 OS Version Information
	LSL_SYSINFO_OS_VERSION := 100, // string: OS
	LSL_SYSINFO_OS_VERSION_MAJOR_MAJOR,
	LSL_SYSINFO_OS_VERSION_MAJOR,
	LSL_SYSINFO_OS_VERSION_MINOR,
	LSL_SYSINFO_OS_TYPE, // return type as uppercase string "SALAMANDER", "GECKO"
	LSL_SYSINFO_OS_TYPE_INT, // return 2 = Salamander, 3 = Gecko (CpuEquipment Platform)
	LSL_SYSINFO_OS_TYPE_VERSION, // Salamander2, Salamander3, Salamander4, Gecko1, ...

	LSL_SYSINFO_ID_LAST						// not used, id not importe
	)$UDINT;
END_TYPE

TYPE
  LSL_SYSINFOEX_ID :
  (
	LSL_SYSINFOEX_FAN_COUNT             := 0x00010000,	// uint32_t count of FAN's no idx, idxSub
	LSL_SYSINFOEX_FAN_PWM,                        // int32_t pwm of [idx], no idxSub
	LSL_SYSINFOEX_FAN_RPM,                        // int32_t RPM of [idx], no idxSub

	LSL_SYSINFOEX_SDCARD_COUNT          := 0x00020000,  // uint32_t count of SDCARD's
	LSL_SYSINFOEX_SDCARD_VENDOR,                  // string: vendor name
	LSL_SYSINFOEX_SDCARD_MODEL,                   // string: model name
	LSL_SYSINFOEX_SDCARD_FIRMWARE,                // string: firmware version string
	LSL_SYSINFOEX_SDCARD_MAX_AVERAGE_ERASE_CNT,   // uint32: max limit of average erase count
	LSL_SYSINFOEX_SDCARD_AVERAGE_ERASE_CNT,       // uint32: current average erase count
	LSL_SYSINFOEX_SDCARD_POWER_ON_CNT,            // uint32: power on counter
	LSL_SYSINFOEX_SDCARD_TYPE,                    // string: type (SD,MMC,...)
	LSL_SYSINFOEX_SDCARD_ISBOOTDEV,               // int32_t: is boot device

  LSL_SYSINFOEX_ID_LAST
	)$UDINT;
END_TYPE


TYPE
	#pragma pack (push, 1)
	LSL_SYSINFO_TYPE : STRUCT
    // version 0x00010000
		version	: UDINT;
		SYSINFO_GetProperty : pVoid;
    // version 0x00020000
    SYSINFO_GetPropertyEx : pVoid;
	END_STRUCT;
	#pragma pack (pop)
END_TYPE

////////////////////////////////////////////////////////////////////////////////
// LASAL Function Prototypes
////////////////////////////////////////////////////////////////////////////////
FUNCTION __CDECL GLOBAL P_SYSINFO_GetProperty
	VAR_INPUT
		id	: LSL_SYSINFO_ID;
		pBuf : pVoid;
		len : UDINT;
	END_VAR
	VAR_OUTPUT
		retval	: DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_SYSINFO_GetPropertyEx
	VAR_INPUT
		id	: LSL_SYSINFOEX_ID;
    index : UDINT;
    indexSub : UDINT;
		pBuf : pVoid;
		len : UDINT;
	END_VAR
	VAR_OUTPUT
		retval	: DINT;
	END_VAR;

////////////////////////////////////////////////////////////////////////////////
// LASAL MACROS
////////////////////////////////////////////////////////////////////////////////
#define OS_SYSINFO_GET_PROP(pSYSINFO,p1,p2,p3) pSYSINFO^.SYSINFO_GetProperty $ P_SYSINFO_GetProperty(p1,p2,p3)
#define OS_SYSINFO_GET_PROPEX(pSYSINFO,p1,p2,p3,p4,p5) pSYSINFO^.SYSINFO_GetPropertyEx $ P_SYSINFO_GetPropertyEx(p1,p2,p3,p4,p5)

#endif
