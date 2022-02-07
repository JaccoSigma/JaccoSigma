#ifndef  __XADC_H
#pragma once
#define  __XADC_H

// Interface
#define INTERFACE_XADC                       "LSL_XADC"


#define LSL_XADC_TYPE_VERSION    0x0001

TYPE
  #pragma pack (push, 1)
	LSLAPI_IXADC : STRUCT
    version	        		: UDINT;
    XADC_GetBaseAddr       	: pVoid;
    XADC_GetTemperature     : pVoid;
    XADC_GetMinTemperature  : pVoid;
    XADC_GetMaxTemperature  : pVoid;
    XADC_GetSupplyAoffset	: pVoid;
    XADC_GetAdcAoffset		: pVoid;
    XADC_GetAdcAgain		: pVoid;
    XADC_GetSupplyBoffset	: pVoid;
    XADC_GetAdcBoffset		: pVoid;
    XADC_GetAdcBgain		: pVoid;
    XADC_GetVccint			: pVoid;
    XADC_GetMinVccint		: pVoid;
    XADC_GetMaxVccint		: pVoid;
    XADC_GetVccaux			: pVoid;
    XADC_GetMinVccaux		: pVoid;
    XADC_GetMaxVccaux		: pVoid;
    XADC_GetVpVn			: pVoid;
	XADC_GetVrefp 			: pVoid;
	XADC_GetVrefn 			: pVoid;
	XADC_GetVccbram 		: pVoid;
	XADC_GetMinVccbram 		: pVoid;
	XADC_GetMaxVccbram 		: pVoid;
	XADC_GetVccpint 		: pVoid;
	XADC_GetMinVccpint 		: pVoid;
	XADC_GetMaxVccpint 		: pVoid;
	XADC_GetVccpaux 		: pVoid;
	XADC_GetMinVccpaux 		: pVoid;
	XADC_GetMaxVccpaux 		: pVoid;
	XADC_GetVccoddr 		: pVoid;
	XADC_GetMinVccoddr 		: pVoid;
	XADC_GetMaxVccoddr 		: pVoid;
	XADC_GetVauxpVauxn0		: pVoid;
	XADC_GetVauxpVauxn1		: pVoid;
	XADC_GetVauxpVauxn2		: pVoid;
	XADC_GetVauxpVauxn3		: pVoid;
	XADC_GetVauxpVauxn4		: pVoid;
	XADC_GetVauxpVauxn5		: pVoid;
	XADC_GetVauxpVauxn6		: pVoid;
	XADC_GetVauxpVauxn7		: pVoid;
	XADC_GetVauxpVauxn8		: pVoid;
	XADC_GetVauxpVauxn9		: pVoid;
	XADC_GetVauxpVauxn10	: pVoid;
	XADC_GetVauxpVauxn11	: pVoid;
	XADC_GetVauxpVauxn12	: pVoid;
	XADC_GetVauxpVauxn13	: pVoid;
	XADC_GetVauxpVauxn14	: pVoid;
	XADC_GetVauxpVauxn15	: pVoid;
	XADC_GetFlag		 	: pVoid;
	XADC_GetConfigRegister0	: pVoid;
	XADC_GetConfigRegister1	: pVoid;
	XADC_GetConfigRegister2	: pVoid;
	XADC_SetConfigRegister0	: pVoid;
	XADC_SetConfigRegister1	: pVoid;
	XADC_SetConfigRegister2	: pVoid;
	XADC_GetTestRegister0	: pVoid;
	XADC_GetTestRegister1	: pVoid;
	XADC_GetTestRegister2	: pVoid;
	XADC_GetTestRegister3	: pVoid;
	XADC_GetTestRegister4	: pVoid;
	XADC_SetTestRegister0	: pVoid;
	XADC_SetTestRegister1	: pVoid;
	XADC_SetTestRegister2	: pVoid;
	XADC_SetTestRegister3	: pVoid;
	XADC_SetTestRegister4	: pVoid;
	XADC_GetSequenzeRegister0	: pVoid;
	XADC_GetSequenzeRegister1	: pVoid;
	XADC_GetSequenzeRegister2	: pVoid;
	XADC_GetSequenzeRegister3	: pVoid;
	XADC_GetSequenzeRegister4	: pVoid;
	XADC_GetSequenzeRegister5	: pVoid;
	XADC_GetSequenzeRegister6	: pVoid;
	XADC_GetSequenzeRegister7	: pVoid;
	XADC_SetSequenzeRegister0	: pVoid;
	XADC_SetSequenzeRegister1	: pVoid;
	XADC_SetSequenzeRegister2	: pVoid;
	XADC_SetSequenzeRegister3	: pVoid;
	XADC_SetSequenzeRegister4	: pVoid;
	XADC_SetSequenzeRegister5	: pVoid;
	XADC_SetSequenzeRegister6	: pVoid;
	XADC_SetSequenzeRegister7	: pVoid;
	XADC_GetAlarmRegister0	: pVoid;
	XADC_GetAlarmRegister1	: pVoid;
	XADC_GetAlarmRegister2	: pVoid;
	XADC_GetAlarmRegister3	: pVoid;
	XADC_GetAlarmRegister4	: pVoid;
	XADC_GetAlarmRegister5	: pVoid;
	XADC_GetAlarmRegister6	: pVoid;
	XADC_GetAlarmRegister7	: pVoid;
	XADC_GetAlarmRegister8	: pVoid;
	XADC_GetAlarmRegister9	: pVoid;
	XADC_GetAlarmRegister10	: pVoid;
	XADC_GetAlarmRegister11	: pVoid;
	XADC_GetAlarmRegister12	: pVoid;
	XADC_GetAlarmRegister13	: pVoid;
	XADC_GetAlarmRegister14	: pVoid;
	XADC_GetAlarmRegister15	: pVoid;
	XADC_SetAlarmRegister0	: pVoid;
	XADC_SetAlarmRegister1	: pVoid;
	XADC_SetAlarmRegister2	: pVoid;
	XADC_SetAlarmRegister3	: pVoid;
	XADC_SetAlarmRegister4	: pVoid;
	XADC_SetAlarmRegister5	: pVoid;
	XADC_SetAlarmRegister6	: pVoid;
	XADC_SetAlarmRegister7	: pVoid;
	XADC_SetAlarmRegister8	: pVoid;
	XADC_SetAlarmRegister9	: pVoid;
	XADC_SetAlarmRegister10	: pVoid;
	XADC_SetAlarmRegister11	: pVoid;
	XADC_SetAlarmRegister12	: pVoid;
	XADC_SetAlarmRegister13	: pVoid;
	XADC_SetAlarmRegister14	: pVoid;
	XADC_SetAlarmRegister15	: pVoid;
  END_STRUCT;
  #pragma pack (pop)
END_TYPE

FUNCTION __CDECL GLOBAL P_XADC_GetBaseAddr
	VAR_OUTPUT
		retcode     : ^UDINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetTemperature
	VAR_INPUT
		temperature       	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMinTemperature
	VAR_INPUT
	temperature       	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMaxTemperature
	VAR_INPUT
		temperature       	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetSupplyAoffset
	VAR_INPUT
		supplyAoffset     	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAdcAoffset
	VAR_INPUT
		adcAoffset       	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAdcAgain
	VAR_INPUT
		adcAgain       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetSupplyBoffset
	VAR_INPUT
		supplyBoffset      	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAdcBoffset
	VAR_INPUT
		adcBoffset       	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAdcBgain
	VAR_INPUT
		adcBgain       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVccint
	VAR_INPUT
		vccint       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMinVccint
	VAR_INPUT
		vccint       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMaxVccint
	VAR_INPUT
		vccint       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVccaux
	VAR_INPUT
		vccaux       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMinVccaux
	VAR_INPUT
		vccaux       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMaxVccaux
	VAR_INPUT
		vccaux       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVpVn
	VAR_INPUT
		vpvn       	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVrefp
	VAR_INPUT
		vrefp       	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVrefn
	VAR_INPUT
		vrefn       	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVccbram
	VAR_INPUT
		vccbram       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMinVccbram
	VAR_INPUT
		vccbram       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMaxVccbram
	VAR_INPUT
		vccbram       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVccpint
	VAR_INPUT
		vccpint       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMinVccpint
	VAR_INPUT
		vccpint       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMaxVccpint
	VAR_INPUT
		vccpint       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVccpaux
	VAR_INPUT
		vccpaux       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMinVccpaux
	VAR_INPUT
		vccpaux       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMaxVccpaux
	VAR_INPUT
		vccpaux       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVccoddr
	VAR_INPUT
		vccoddr       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMinVccoddr
	VAR_INPUT
		vccoddr       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetMaxVccoddr
	VAR_INPUT
		vccoddr       		: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn0
	VAR_INPUT
		vauxpVauxn0 	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn1
	VAR_INPUT
		vauxpVauxn1       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn2
	VAR_INPUT
		vauxpVauxn2       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn3
	VAR_INPUT
		vauxpVauxn3       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn4
	VAR_INPUT
		vauxpVauxn4       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn5
	VAR_INPUT
		vauxpVauxn5       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn6
	VAR_INPUT
		vauxpVauxn6       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn7
	VAR_INPUT
		vauxpVauxn7       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn8
	VAR_INPUT
		vauxpVauxn8       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn9
	VAR_INPUT
		vauxpVauxn9       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn10
	VAR_INPUT
		vauxpVauxn10       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn11
	VAR_INPUT
		vauxpVauxn11       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn12
	VAR_INPUT
		vauxpVauxn12       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn13
	VAR_INPUT
		vauxpVauxn13       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn14
	VAR_INPUT
		vauxpVauxn14       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetVauxpVauxn15
	VAR_INPUT
		vauxpVauxn15       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetFlag
	VAR_INPUT
		flag       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetConfigRegister0
	VAR_INPUT
		configRegister0 	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetConfigRegister1
	VAR_INPUT
		configRegister1       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetConfigRegister2
	VAR_INPUT
		configRegister2       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetConfigRegister0
	VAR_INPUT
		configRegister0       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetConfigRegister1
	VAR_INPUT
		configRegister1       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetConfigRegister2
	VAR_INPUT
		configRegister2       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetTestRegister0
	VAR_INPUT
		testRegister0 	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetTestRegister1
	VAR_INPUT
		testRegister1       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetTestRegister2
	VAR_INPUT
		testRegister2       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetTestRegister3
	VAR_INPUT
		testRegister3       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetTestRegister4
	VAR_INPUT
		testRegister4       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetTestRegister0
	VAR_INPUT
		testRegister0       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetTestRegister1
	VAR_INPUT
		testRegister1       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetTestRegister2
	VAR_INPUT
		testRegister2       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetTestRegister3
	VAR_INPUT
		testRegister3       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetTestRegister4
	VAR_INPUT
		testRegister4       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetSequenzeRegister0
	VAR_INPUT
		sequenzeRegister0 	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetSequenzeRegister1
	VAR_INPUT
		sequenzeRegister1       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetSequenzeRegister2
	VAR_INPUT
		sequenzeRegister2       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetSequenzeRegister3
	VAR_INPUT
		sequenzeRegister3       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetSequenzeRegister4
	VAR_INPUT
		sequenzeRegister4       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetSequenzeRegister5
	VAR_INPUT
		sequenzeRegister5       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetSequenzeRegister6
	VAR_INPUT
		sequenzeRegister6       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetSequenzeRegister7
	VAR_INPUT
		sequenzeRegister7       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;


FUNCTION __CDECL GLOBAL P_XADC_SetSequenzeRegister0
VAR_INPUT
	sequenzeRegister0       			: UDINT;
END_VAR
VAR_OUTPUT
	retcode     : DINT;
END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetSequenzeRegister1
	VAR_INPUT
		sequenzeRegister1       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetSequenzeRegister2
	VAR_INPUT
		sequenzeRegister2       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetSequenzeRegister3
	VAR_INPUT
		sequenzeRegister3       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetSequenzeRegister4
	VAR_INPUT
		sequenzeRegister4       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetSequenzeRegister5
	VAR_INPUT
		sequenzeRegister5       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetSequenzeRegister6
	VAR_INPUT
		sequenzeRegister6       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;


FUNCTION __CDECL GLOBAL P_XADC_SetSequenzeRegister7
	VAR_INPUT
		sequenzeRegister7       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister0
	VAR_INPUT
		alarmRegister0 	: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister1
	VAR_INPUT
		alarmRegister1       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister2
	VAR_INPUT
		alarmRegister2       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister3
	VAR_INPUT
		alarmRegister3       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister4
	VAR_INPUT
		alarmRegister4       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister5
	VAR_INPUT
		alarmRegister5       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister6
	VAR_INPUT
		alarmRegister6       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister7
	VAR_INPUT
		alarmRegister7       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister8
	VAR_INPUT
		alarmRegister8       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister9
	VAR_INPUT
		alarmRegister9       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister10
	VAR_INPUT
		alarmRegister10       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister11
	VAR_INPUT
		alarmRegister11       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister12
	VAR_INPUT
		alarmRegister12       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister13
	VAR_INPUT
		alarmRegister13       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister14
	VAR_INPUT
		alarmRegister14       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_GetAlarmRegister15
	VAR_INPUT
		alarmRegister15       			: ^UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister0
	VAR_INPUT
		alarmRegister0       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister1
	VAR_INPUT
		alarmRegister1       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister2
	VAR_INPUT
		alarmRegister2       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister3
	VAR_INPUT
		alarmRegister3       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister4
	VAR_INPUT
		alarmRegister4       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister5
	VAR_INPUT
		alarmRegister5       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister6
	VAR_INPUT
		alarmRegister6       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;


FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister7
	VAR_INPUT
		alarmRegister7       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister8
	VAR_INPUT
		alarmRegister8       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister9
	VAR_INPUT
		alarmRegister9       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister10
	VAR_INPUT
		alarmRegister10       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister11
	VAR_INPUT
		alarmRegister11       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister12
	VAR_INPUT
		alarmRegister12       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister13
	VAR_INPUT
		alarmRegister13       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister14
	VAR_INPUT
		alarmRegister14       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

FUNCTION __CDECL GLOBAL P_XADC_SetAlarmRegister15
	VAR_INPUT
		alarmRegister15       			: UDINT;
	END_VAR
	VAR_OUTPUT
		retcode     : DINT;
	END_VAR;

#define OS_XADC_CHECK_VERSION(pXADC)				pXADC^.version <> LSL_XADC_TYPE_VERSION
#define OS_XADC_GETBASEADDR(pXADC)         			pXADC^.XADC_GetBaseAddr 			$ P_XADC_GetBaseAddr()
#define OS_XADC_GETTEMPERATURE(pXADC,p1)     		pXADC^.XADC_GetTemperature 			$ P_XADC_GetTemperature(p1)
#define OS_XADC_GETMINTEMPERATURE(pXADC,p1)    		pXADC^.XADC_GetMinTemperature 		$ P_XADC_GetMinTemperature(p1)
#define OS_XADC_GETMAXTEMPERATURE(pXADC,p1)    		pXADC^.XADC_GetMaxTemperature 		$ P_XADC_GetMaxTemperature(p1)
#define OS_XADC_GETSUPPLYAOFFSET(pXADC,p1)     		pXADC^.XADC_GetSupplyAoffset		$ P_XADC_GetSupplyAoffset(p1)
#define OS_XADC_GETADCAOFFSET(pXADC,p1)	    		pXADC^.XADC_GetAdcAoffset	 		$ P_XADC_GetAdcAoffset(p1)
#define OS_XADC_GETADCAGAIN(pXADC,p1)	    		pXADC^.XADC_GetAdcAgain 			$ P_XADC_GetAdcAgain(p1)
#define OS_XADC_GETSUPPLYBOFFSET(pXADC,p1)     		pXADC^.XADC_GetSupplyBoffset		$ P_XADC_GetSupplyBoffset(p1)
#define OS_XADC_GETADCBOFFSET(pXADC,p1)	    		pXADC^.XADC_GetAdcBoffset	 		$ P_XADC_GetAdcBoffset(p1)
#define OS_XADC_GETADCBGAIN(pXADC,p1)	    		pXADC^.XADC_GetAdcBgain 			$ P_XADC_GetAdcBgain(p1)
#define OS_XADC_GETVCCINT(pXADC,p1)     			pXADC^.XADC_GetVccint 				$ P_XADC_GetVccint(p1)
#define OS_XADC_GETMINVCCINT(pXADC,p1)    			pXADC^.XADC_GetMinVccint 			$ P_XADC_GetMinVccint(p1)
#define OS_XADC_GETMAXVCCINT(pXADC,p1)    			pXADC^.XADC_GetMaxVccint 			$ P_XADC_GetMaxVccint(p1)
#define OS_XADC_GETVCCAUX(pXADC,p1)     			pXADC^.XADC_GetVccaux 				$ P_XADC_GetVccaux(p1)
#define OS_XADC_GETMINVCCAUX(pXADC,p1)    			pXADC^.XADC_GetMinVccaux 			$ P_XADC_GetMinVccaux(p1)
#define OS_XADC_GETMAXVCCAUX(pXADC,p1)    			pXADC^.XADC_GetMaxVccaux 			$ P_XADC_GetMaxVccaux(p1)
#define OS_XADC_GETVPVN(pXADC,p1)     				pXADC^.XADC_GetVpVn 				$ P_XADC_GetVpVn(p1)
#define OS_XADC_GETVREFP(pXADC,p1)    	 			pXADC^.XADC_GetVrefp 				$ P_XADC_GetVrefp(p1)
#define OS_XADC_GETVREFN(pXADC,p1)    	 			pXADC^.XADC_GetVrefn 				$ P_XADC_GetVrefn(p1)
#define OS_XADC_GETVCCBRAM(pXADC,p1)     			pXADC^.XADC_GetVccbram 				$ P_XADC_GetVccbram(p1)
#define OS_XADC_GETMINVCCBRAM(pXADC,p1)    			pXADC^.XADC_GetMinVccbram 			$ P_XADC_GetMinVccbram(p1)
#define OS_XADC_GETMAXVCCBRAM(pXADC,p1)    			pXADC^.XADC_GetMaxVccbram 			$ P_XADC_GetMaxVccbram(p1)
#define OS_XADC_GETVCCPINT(pXADC,p1)     			pXADC^.XADC_GetVccpint 				$ P_XADC_GetVccpint(p1)
#define OS_XADC_GETMINVCCPINT(pXADC,p1)    			pXADC^.XADC_GetMinVccpint 			$ P_XADC_GetMinVccpint(p1)
#define OS_XADC_GETMAXVCCPINT(pXADC,p1)    			pXADC^.XADC_GetMaxVccpint 			$ P_XADC_GetMaxVccpint(p1)
#define OS_XADC_GETVCCPAUX(pXADC,p1)     			pXADC^.XADC_GetVccpaux 				$ P_XADC_GetVccpaux(p1)
#define OS_XADC_GETMINVCCPAUX(pXADC,p1)    			pXADC^.XADC_GetMinVccpaux 			$ P_XADC_GetMinVccpaux(p1)
#define OS_XADC_GETMAXVCCPAUX(pXADC,p1)    			pXADC^.XADC_GetMaxVccpaux 			$ P_XADC_GetMaxVccpaux(p1)
#define OS_XADC_GETVCCODDR(pXADC,p1)     			pXADC^.XADC_GetVccoddr 				$ P_XADC_GetVccoddr(p1)
#define OS_XADC_GETMINVCCODDR(pXADC,p1)    			pXADC^.XADC_GetMinVccoddr 			$ P_XADC_GetMinVccoddr(p1)
#define OS_XADC_GETMAXVCCODDR(pXADC,p1)    			pXADC^.XADC_GetMaxVccoddr 			$ P_XADC_GetMaxVccoddr(p1)
#define OS_XADC_GETVAUXPVAUXN0(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn0			$ P_XADC_GetVauxpVauxn0(p1)
#define OS_XADC_GETVAUXPVAUXN1(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn1			$ P_XADC_GetVauxpVauxn1(p1)
#define OS_XADC_GETVAUXPVAUXN2(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn2			$ P_XADC_GetVauxpVauxn2(p1)
#define OS_XADC_GETVAUXPVAUXN3(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn3			$ P_XADC_GetVauxpVauxn3(p1)
#define OS_XADC_GETVAUXPVAUXN4(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn4			$ P_XADC_GetVauxpVauxn4(p1)
#define OS_XADC_GETVAUXPVAUXN5(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn5			$ P_XADC_GetVauxpVauxn5(p1)
#define OS_XADC_GETVAUXPVAUXN6(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn6			$ P_XADC_GetVauxpVauxn6(p1)
#define OS_XADC_GETVAUXPVAUXN7(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn7			$ P_XADC_GetVauxpVauxn7(p1)
#define OS_XADC_GETVAUXPVAUXN8(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn8			$ P_XADC_GetVauxpVauxn8(p1)
#define OS_XADC_GETVAUXPVAUXN9(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn9			$ P_XADC_GetVauxpVauxn9(p1)
#define OS_XADC_GETVAUXPVAUXN10(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn10			$ P_XADC_GetVauxpVauxn10(p1)
#define OS_XADC_GETVAUXPVAUXN11(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn11			$ P_XADC_GetVauxpVauxn11(p1)
#define OS_XADC_GETVAUXPVAUXN12(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn12			$ P_XADC_GetVauxpVauxn12(p1)
#define OS_XADC_GETVAUXPVAUXN13(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn13			$ P_XADC_GetVauxpVauxn13(p1)
#define OS_XADC_GETVAUXPVAUXN14(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn14			$ P_XADC_GetVauxpVauxn14(p1)
#define OS_XADC_GETVAUXPVAUXN15(pXADC,p1)			pXADC^.XADC_GetVauxpVauxn15			$ P_XADC_GetVauxpVauxn15(p1)
#define OS_XADC_GETFLAG(pXADC,p1)	    			pXADC^.XADC_GetFlag		 			$ P_XADC_GetFlag(p1)
#define OS_XADC_GETCONFIGREGISTER0(pXADC,p1)		pXADC^.XADC_GetConfigRegister0		$ P_XADC_GetConfigRegister0(p1)
#define OS_XADC_GETCONFIGREGISTER1(pXADC,p1)		pXADC^.XADC_GetConfigRegister1		$ P_XADC_GetConfigRegister1(p1)
#define OS_XADC_GETCONFIGREGISTER2(pXADC,p1)		pXADC^.XADC_GetConfigRegister2		$ P_XADC_GetConfigRegister2(p1)
#define OS_XADC_SETCONFIGREGISTER0(pXADC,p1)		pXADC^.XADC_SetConfigRegister0		$ P_XADC_SetConfigRegister0(p1)
#define OS_XADC_SETCONFIGREGISTER1(pXADC,p1)		pXADC^.XADC_SetConfigRegister1		$ P_XADC_SetConfigRegister1(p1)
#define OS_XADC_SETCONFIGREGISTER2(pXADC,p1)		pXADC^.XADC_SetConfigRegister2		$ P_XADC_SetConfigRegister2(p1)
#define OS_XADC_GETTESTREGISTER0(pXADC,p1)			pXADC^.XADC_GetTestRegister0		$ P_XADC_GetTestRegister0(p1)
#define OS_XADC_GETTESTREGISTER1(pXADC,p1)			pXADC^.XADC_GetTestRegister1		$ P_XADC_GetTestRegister1(p1)
#define OS_XADC_GETTESTREGISTER2(pXADC,p1)			pXADC^.XADC_GetTestRegister2		$ P_XADC_GetTestRegister2(p1)
#define OS_XADC_GETTESTREGISTER3(pXADC,p1)			pXADC^.XADC_GetTestRegister3		$ P_XADC_GetTestRegister3(p1)
#define OS_XADC_GETTESTREGISTER4(pXADC,p1)			pXADC^.XADC_GetTestRegister4		$ P_XADC_GetTestRegister4(p1)
#define OS_XADC_SETTESTREGISTER0(pXADC,p1)			pXADC^.XADC_SetTestRegister0		$ P_XADC_SetTestRegister0(p1)
#define OS_XADC_SETTESTREGISTER1(pXADC,p1)			pXADC^.XADC_SetTestRegister1		$ P_XADC_SetTestRegister1(p1)
#define OS_XADC_SETTESTREGISTER2(pXADC,p1)			pXADC^.XADC_SetTestRegister2		$ P_XADC_SetTestRegister2(p1)
#define OS_XADC_SETTESTREGISTER3(pXADC,p1)			pXADC^.XADC_SetTestRegister3		$ P_XADC_SetTestRegister3(p1)
#define OS_XADC_SETTESTREGISTER4(pXADC,p1)			pXADC^.XADC_SetTestRegister4		$ P_XADC_SetTestRegister4(p1)
#define OS_XADC_GETSEQUENZEREGISTER0(pXADC,p1)		pXADC^.XADC_GetSequenzeRegister0	$ P_XADC_GetSequenzeRegister0(p1)
#define OS_XADC_GETSEQUENZEREGISTER1(pXADC,p1)		pXADC^.XADC_GetSequenzeRegister1	$ P_XADC_GetSequenzeRegister1(p1)
#define OS_XADC_GETSEQUENZEREGISTER2(pXADC,p1)		pXADC^.XADC_GetSequenzeRegister2	$ P_XADC_GetSequenzeRegister2(p1)
#define OS_XADC_GETSEQUENZEREGISTER3(pXADC,p1)		pXADC^.XADC_GetSequenzeRegister3	$ P_XADC_GetSequenzeRegister3(p1)
#define OS_XADC_GETSEQUENZEREGISTER4(pXADC,p1)		pXADC^.XADC_GetSequenzeRegister4	$ P_XADC_GetSequenzeRegister4(p1)
#define OS_XADC_GETSEQUENZEREGISTER5(pXADC,p1)		pXADC^.XADC_GetSequenzeRegister5	$ P_XADC_GetSequenzeRegister5(p1)
#define OS_XADC_GETSEQUENZEREGISTER6(pXADC,p1)		pXADC^.XADC_GetSequenzeRegister6	$ P_XADC_GetSequenzeRegister6(p1)
#define OS_XADC_GETSEQUENZEREGISTER7(pXADC,p1)		pXADC^.XADC_GetSequenzeRegister7	$ P_XADC_GetSequenzeRegister7(p1)
#define OS_XADC_SETSEQUENZEREGISTER0(pXADC,p1)		pXADC^.XADC_SetSequenzeRegister0	$ P_XADC_SetSequenzeRegister0(p1)
#define OS_XADC_SETSEQUENZEREGISTER1(pXADC,p1)		pXADC^.XADC_SetSequenzeRegister1	$ P_XADC_SetSequenzeRegister1(p1)
#define OS_XADC_SETSEQUENZEREGISTER2(pXADC,p1)		pXADC^.XADC_SetSequenzeRegister2	$ P_XADC_SetSequenzeRegister2(p1)
#define OS_XADC_SETSEQUENZEREGISTER3(pXADC,p1)		pXADC^.XADC_SetSequenzeRegister3	$ P_XADC_SetSequenzeRegister3(p1)
#define OS_XADC_SETSEQUENZEREGISTER4(pXADC,p1)		pXADC^.XADC_SetSequenzeRegister4	$ P_XADC_SetSequenzeRegister4(p1)
#define OS_XADC_SETSEQUENZEREGISTER5(pXADC,p1)		pXADC^.XADC_SetSequenzeRegister5	$ P_XADC_SetSequenzeRegister5(p1)
#define OS_XADC_SETSEQUENZEREGISTER6(pXADC,p1)		pXADC^.XADC_SetSequenzeRegister6	$ P_XADC_SetSequenzeRegister6(p1)
#define OS_XADC_SETSEQUENZEREGISTER7(pXADC,p1)		pXADC^.XADC_SetSequenzeRegister7	$ P_XADC_SetSequenzeRegister7(p1)
#define OS_XADC_GETALARMREGISTER0(pXADC,p1)			pXADC^.XADC_GetAlarmRegister0		$ P_XADC_GetAlarmRegister0(p1)
#define OS_XADC_GETALARMREGISTER1(pXADC,p1)			pXADC^.XADC_GetAlarmRegister1		$ P_XADC_GetAlarmRegister1(p1)
#define OS_XADC_GETALARMREGISTER2(pXADC,p1)			pXADC^.XADC_GetAlarmRegister2		$ P_XADC_GetAlarmRegister2(p1)
#define OS_XADC_GETALARMREGISTER3(pXADC,p1)			pXADC^.XADC_GetAlarmRegister3		$ P_XADC_GetAlarmRegister3(p1)
#define OS_XADC_GETALARMREGISTER4(pXADC,p1)			pXADC^.XADC_GetAlarmRegister4		$ P_XADC_GetAlarmRegister4(p1)
#define OS_XADC_GETALARMREGISTER5(pXADC,p1)			pXADC^.XADC_GetAlarmRegister5		$ P_XADC_GetAlarmRegister5(p1)
#define OS_XADC_GETALARMREGISTER6(pXADC,p1)			pXADC^.XADC_GetAlarmRegister6		$ P_XADC_GetAlarmRegister6(p1)
#define OS_XADC_GETALARMREGISTER7(pXADC,p1)			pXADC^.XADC_GetAlarmRegister7		$ P_XADC_GetAlarmRegister7(p1)
#define OS_XADC_GETALARMREGISTER8(pXADC,p1)			pXADC^.XADC_GetAlarmRegister8		$ P_XADC_GetAlarmRegister8(p1)
#define OS_XADC_GETALARMREGISTER9(pXADC,p1)			pXADC^.XADC_GetAlarmRegister9		$ P_XADC_GetAlarmRegister9(p1)
#define OS_XADC_GETALARMREGISTER10(pXADC,p1)		pXADC^.XADC_GetAlarmRegister10		$ P_XADC_GetAlarmRegister10(p1)
#define OS_XADC_GETALARMREGISTER11(pXADC,p1)		pXADC^.XADC_GetAlarmRegister11		$ P_XADC_GetAlarmRegister11(p1)
#define OS_XADC_GETALARMREGISTER12(pXADC,p1)		pXADC^.XADC_GetAlarmRegister12		$ P_XADC_GetAlarmRegister12(p1)
#define OS_XADC_GETALARMREGISTER13(pXADC,p1)		pXADC^.XADC_GetAlarmRegister13		$ P_XADC_GetAlarmRegister13(p1)
#define OS_XADC_GETALARMREGISTER14(pXADC,p1)		pXADC^.XADC_GetAlarmRegister14		$ P_XADC_GetAlarmRegister14(p1)
#define OS_XADC_GETALARMREGISTER15(pXADC,p1)		pXADC^.XADC_GetAlarmRegister15		$ P_XADC_GetAlarmRegister15(p1)
#define OS_XADC_SETALARMREGISTER0(pXADC,p1)			pXADC^.XADC_SetAlarmRegister0		$ P_XADC_SetAlarmRegister0(p1)
#define OS_XADC_SETALARMREGISTER1(pXADC,p1)			pXADC^.XADC_SetAlarmRegister1		$ P_XADC_SetAlarmRegister1(p1)
#define OS_XADC_SETALARMREGISTER2(pXADC,p1)			pXADC^.XADC_SetAlarmRegister2		$ P_XADC_SetAlarmRegister2(p1)
#define OS_XADC_SETALARMREGISTER3(pXADC,p1)			pXADC^.XADC_SetAlarmRegister3		$ P_XADC_SetAlarmRegister3(p1)
#define OS_XADC_SETALARMREGISTER4(pXADC,p1)			pXADC^.XADC_SetAlarmRegister4		$ P_XADC_SetAlarmRegister4(p1)
#define OS_XADC_SETALARMREGISTER5(pXADC,p1)			pXADC^.XADC_SetAlarmRegister5		$ P_XADC_SetAlarmRegister5(p1)
#define OS_XADC_SETALARMREGISTER6(pXADC,p1)			pXADC^.XADC_SetAlarmRegister6		$ P_XADC_SetAlarmRegister6(p1)
#define OS_XADC_SETALARMREGISTER7(pXADC,p1)			pXADC^.XADC_SetAlarmRegister7		$ P_XADC_SetAlarmRegister7(p1)
#define OS_XADC_SETALARMREGISTER8(pXADC,p1)			pXADC^.XADC_SetAlarmRegister8		$ P_XADC_SetAlarmRegister8(p1)
#define OS_XADC_SETALARMREGISTER9(pXADC,p1)			pXADC^.XADC_SetAlarmRegister9		$ P_XADC_SetAlarmRegister9(p1)
#define OS_XADC_SETALARMREGISTER10(pXADC,p1)		pXADC^.XADC_SetAlarmRegister10		$ P_XADC_SetAlarmRegister10(p1)
#define OS_XADC_SETALARMREGISTER11(pXADC,p1)		pXADC^.XADC_SetAlarmRegister11		$ P_XADC_SetAlarmRegister11(p1)
#define OS_XADC_SETALARMREGISTER12(pXADC,p1)		pXADC^.XADC_SetAlarmRegister12		$ P_XADC_SetAlarmRegister12(p1)
#define OS_XADC_SETALARMREGISTER13(pXADC,p1)		pXADC^.XADC_SetAlarmRegister13		$ P_XADC_SetAlarmRegister13(p1)
#define OS_XADC_SETALARMREGISTER14(pXADC,p1)		pXADC^.XADC_SetAlarmRegister14		$ P_XADC_SetAlarmRegister14(p1)
#define OS_XADC_SETALARMREGISTER15(pXADC,p1)		pXADC^.XADC_SetAlarmRegister15		$ P_XADC_SetAlarmRegister15(p1)
#endif
