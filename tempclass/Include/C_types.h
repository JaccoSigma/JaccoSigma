//This file was generated by the LASAL2 CodeGenerator  -- 
//Please, do not edit this file (it might be overwritten by the next generator run)
#pragma pack(push, 1)
typedef struct _BasicCanObj 
  {
    USINT CanData [7+1-0];
    USINT length;  //! <Type Comment="datalength of object" Name="_BasicCanObj.length"/>
    UDINT objnumber;  //! <Type Comment="objectnumber" Name="_BasicCanObj.objnumber"/>
  } _BasicCanObj;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _ComSDO 
  {
    HDINT hdTxRxResult;
    DINT dTxCounter;
    DINT dRxCounter;
    DINT dErrCounter;
    HDINT hdLastErrCode;
    HDINT hdErrIndex;
    HDINT hdErrSubIndex;
    HDINT hdErrData;
    DINT dErrRW;
    DINT dErrLen;
  } _ComSDO;
#pragma pack(pop)

typedef CHAR _DISPLAYDUMMY [699+1-0];

typedef UDINT _DRIVETYPE; 
    #define _NotFound 0
    #define _SDD_310 1
    #define _SDD_315 2
    #define _SDD_105 3
    #define _SDD_120 4
    #define _SDD_305 5
    #define _SDD_335 6
    #define _SDD_215 7
    #define _SDD_205 8
    #define _SDD_210 9
    #define _MDD_100 10
    #define _S_340 11
    #define _SDD_115 12
    #define _SDD_1300 811237
    #define _SDD_1400 811238
    #define _SDD_1500 811239
    #define _SDD_1600 811240
    #define _MDD_2000 811300
    #define _VSDC_151 811301
    #define _DC061 881042
    #define _DC062 881801
    #define _SR011 881802
    #define _SR012 881803
    #define _DC101 881804
    #define _DC102 881805
    #define _WA011 991800
    #define _WA012 991801

#pragma pack(push, 1)
typedef struct _I_HC_Type   //! <Type Comment="Hardware Code an axis&#13;&#10;&#13;&#10;Bit 31..16 Hardware Code of the Control Board&#13;&#10;Bit 15...8 Software version of the feedback CPLD&#13;&#10;Bit   7...0 Hardware Code of the Power Stage" Name="_I_HC_Type"/>
  {
    HSINT HardwareCodePowerStage;  //! <Type Comment="Hardware Code of the Power Stage (Default: 16#80)" Name="_I_HC_Type.HardwareCodePowerStage"/>
    HSINT FPGAVersion;  //! <Type Comment="Software version of the feedback CPLD" Name="_I_HC_Type.FPGAVersion"/>
    HINT HardwareCodeControlBoard;  //! <Type Comment="Hardware Code of the Control Board (Default: 16#0000)" Name="_I_HC_Type.HardwareCodeControlBoard"/>
  } _I_HC_Type;
#pragma pack(pop)

typedef DINT _KEYBUFFER [31+1-0];

#pragma pack(push, 1)
typedef struct _KEYS_DATA 
  {
    UDINT ud_no;
    struct 
    {
    }bd_bitcode;
    _KEYBUFFER keybuffer_pressed;
    _KEYBUFFER keybuffer_new;
    _KEYBUFFER keybuffer_gone;
  } _KEYS_DATA;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _LEDS_DATA 
  {
    struct 
    {
    }bd_ledson;
    struct 
    {
    }bd_ledsblinking;
    UDINT ud_ledsblinkingtime;
  } _LEDS_DATA;
#pragma pack(pop)

typedef UDINT _LSLONL_STATUS_PLC; 
    #define LSLONL_RUN_RAM 0
    #define LSLONL_RUN_ROM 1
    #define LSLONL_RUNTIME 2
    #define LSLONL_POINTER 3
    #define LSLONL_CHKSUM 4
    #define LSLONL_WATCHDOG 5
    #define LSLONL_GENERAL_ERROR 6
    #define LSLONL_PROM_DEFECT 7
    #define LSLONL_RESET 8
    #define LSLONL_WD_DEFECT 9
    #define LSLONL_STOP 10
    #define LSLONL_PROG_BUSYS 11
    #define LSLONL_PROGRAM_LENGTH 12
    #define LSLONL_PROG_END 13
    #define LSLONL_PROG_MEMO 14
    #define LSLONL_STOP_BRKPT 15
    #define LSLONL_CPU_STOP 16
    #define LSLONL_INT_ERROR 17
    #define LSLONL_SINGLE_STEP 18
    #define LSLONL_READY 19
    #define LSLONL_LOAD 20
    #define LSLONL_UNZUL_MODUL 21
    #define LSLONL_MEMORY_FULL 22
    #define LSLONL_NOT_LINKED 23
    #define LSLONL_DIV_BY_0 24
    #define LSLONL_DIAS_ERROR 25
    #define LSLONL_WAIT 26
    #define LSLONL_OP_PROG 27
    #define LSLONL_OP_INSTALLED 28
    #define LSLONL_OS_TOO_LONG 29
    #define LSLONL_NO_OPERATING_SYSTEM 30
    #define LSLONL_SEARCH_FOR_OS 31
    #define LSLONL_NO_DEVICE 32
    #define LSLONL_UNUSED_CODE 33
    #define LSLONL_MEM_ERROR 34
    #define LSLONL_MAX_IO 35
    #define LSLONL_MODULE_LOAD_ERROR 36
    #define LSLONL_GENERAL_OS_ERROR 37
    #define LSLONL_APPLMEM_ERROR 38
    #define LSLONL_OFFLINE 39
    #define LSLONL_APPL_LOAD 40
    #define LSLONL_APPL_SAVE 41
    #define LSLONL_VARAN_MANAGER_ERROR 44
    #define LSLONL_VARAN_ERROR 45
    #define LSLONL_APPL_LOAD_ERROR 46
    #define LSLONL_APPL_SAVE_ERROR 47
    #define LSLONL_ACCESS_EXCEPTION 50
    #define LSLONL_BOUND_EXCEEDED 51
    #define LSLONL_PRIVILEDGED_INSTRUCTION 52
    #define LSLONL_FLOATING_POINT_ERROR 53
    #define LSLONL_DIAS_RISC_ERROR 60
    #define LSLONL_INTERNAL_ERROR 64
    #define LSLONL_FILE_ERROR 65
    #define LSLONL_DEBUG_ASSERTION_FAILED 66
    #define LSLONL_REALTIME_RUNTIME 67
    #define LSLONL_BACKGROUND_RUNTIME 68
    #define LSLONL_USER_DEFINED_0 95
    #define LSLONL_USER_DEFINED_1 96
    #define LSLONL_USER_DEFINED_2 97
    #define LSLONL_USER_DEFINED_3 98
    #define LSLONL_USER_DEFINED_4 99
    #define LSLONL_C_INIT 100
    #define LSLONL_RUNRAM 101
    #define LSLONL_C_RUNROM 102
    #define LSLONL_C_RUNTIME 103
    #define LSLONL_C_READY 104
    #define LSLONL_C_OK 105
    #define LSLONL_C_UNKNOWN_CID 106
    #define LSLONL_C_UNKNOWN_CONSTR 107
    #define LSLONL_C_UNKNOWN_OBJECT 108
    #define LSLONL_C_UNKNOWN_CHNL 109
    #define LSLONL_C_WRONG_CONNECT 110
    #define LSLONL_C_WRONG_ATTR 111
    #define LSLONL_C_SYNTAX_ERROR 112
    #define LSLONL_C_NO_FILE_OPEN 113
    #define LSLONL_C_OUTOF_NEAR 114
    #define LSLONL_C_OUTOF_FAR 115
    #define LSLONL_C_INCOMPATIBLE 116
    #define LSLONL_C_COMPATIBLE 117
    #define LSLONL_LINKING 224
    #define LSLONL_LINKING_ERROR 225
    #define LSLONL_LINKING_DONE 226
    #define LSLONL_OP_BURN 230
    #define LSLONL_OP_BURN_FAIL 231
    #define LSLONL_OP_INSTALL 232
    #define LSLONL_USV_WAIT 240
    #define LSLONL_REBOOT 241
    #define LSLONL_LSL_SAVE 242
    #define LSLONL_LSL_LOAD 243
    #define LSLONL_CONTINUE 252
    #define LSLONL_PRERUN 253
    #define LSLONL_PRERESET 254
    #define LSLONL_CONNECTION_BREAK 255
    #define LSLONL_NOT_CONNECTED 256

typedef USINT _VOIDL;

typedef _VOIDL *_PVOIDL;

typedef struct 
  {
    long NoHW : 1;
    long NoParameter : 1;
    long NotOnline : 1;
    long NotInitialized : 1;
    long SFFLimitationActive : 1;
    long DiagModeActive : 1;
  }_SDDAXISSTATE;

typedef UDINT _SDDUSERCMD;   //! <Type Comment="Type for user commands to the drive. See Server &apos;SDDCmdIntfc&apos; of the &apos;_DriveAxis&apos; class." Name="_SDDUSERCMD"/>
    #define _SDDIDLE 0
    #define _SDDREADALL 1
    #define _SDDINIT 2
    #define _SDDURINIT 3
    #define _SDDEXDATA 4
    #define _SDDEXDATAXML 6
    #define _SDDIMDATAXML 7
    #define _SDDERROR 8

#pragma pack(push, 1)
typedef struct _TERMINAL_INFO 
  {
    UDINT ud_stno;
    DINT d_id;
    HDINT hd_iddiasbus;
    UDINT ud_lines;
    UDINT ud_charperline;
    UDINT ud_keysinrow;
    UDINT ud_keyrows;
    UDINT ud_keyswithled;
    UDINT b_hwbeep;
  } _TERMINAL_INFO;
#pragma pack(pop)

#pragma pack(push, 4)
typedef struct _XML_ATTRIBUTE 
  {
    USINT *Name;  //! <Type Comment="Name of the attribute" Name="_XML_ATTRIBUTE.Name"/>
    USINT *Value;  //! <Type Comment="Value of the attribute" Name="_XML_ATTRIBUTE.Value"/>
  } _XML_ATTRIBUTE;
#pragma pack(pop)

#pragma pack(push, 4)
typedef struct _XML_ATTRIBUTE_LIST 
  {
    UDINT no;  //! <Type Comment="Number of attributes" Name="_XML_ATTRIBUTE_LIST.no"/>
    _XML_ATTRIBUTE *pList;  //! <Type Comment="Pointer at the list of attributes" Name="_XML_ATTRIBUTE_LIST.pList"/>
  } _XML_ATTRIBUTE_LIST;
#pragma pack(pop)

#pragma pack(push, 4)
typedef struct _XML_ELEMENT 
  {
    USINT *Name;  //! <Type Comment="Name of the element" Name="_XML_ELEMENT.Name"/>
    _XML_ATTRIBUTE_LIST Attributes;  //! <Type Comment="Attribute list of the element" Name="_XML_ELEMENT.Attributes"/>
    USINT *pText;  //! <Type Comment="Pointer at the text of the element" Name="_XML_ELEMENT.pText"/>
    _XML_ELEMENT *pChildren;  //! <Type Comment="Pointer at the child-elements" Name="_XML_ELEMENT.pChildren"/>
    _XML_ELEMENT *pFather;  //! <Type Comment="Pointer at the father element" Name="_XML_ELEMENT.pFather"/>
    _XML_ELEMENT *pNext;  //! <Type Comment="Pointer at the next element at the same level" Name="_XML_ELEMENT.pNext"/>
    _XML_ELEMENT *pPrevious;  //! <Type Comment="Pointer at the previous element at the same level" Name="_XML_ELEMENT.pPrevious"/>
    BOOL isClosed;  //! <Type Comment="True if element is already closed" Name="_XML_ELEMENT.isClosed"/>
  } _XML_ELEMENT;
#pragma pack(pop)

#pragma pack(push, 4)
typedef struct _XML_FILE 
  {
    USINT *Header;  //! <Type Comment="Header in the XML file" Name="_XML_FILE.Header"/>
    _XML_ELEMENT *MajorElement;  //! <Type Comment="Major and first element in the XML file" Name="_XML_FILE.MajorElement"/>
  } _XML_FILE;
#pragma pack(pop)

typedef UDINT DM_CPU_Types; 
    #define CPUNotIdentified 0
    #define IPC_Text_CPU 1
    #define IPC_800_CPU 2
    #define IPC_1024_CPU 3
    #define PCD_CPU 4
    #define DSE_CPU 5
    #define DCP_CPU 6
    #define DCC080_CPU 7
    #define DCP645_CPU 8
    #define AK500_CPU 9
    #define TSTROM_CPU 10
    #define NoCPU_11 11
    #define NoCPU_12 12
    #define NoCPU_13 13
    #define NoCPU_14 14
    #define NoCPU_15 15
    #define ELAN_CPU 16
    #define ELAN800_CPU 17
    #define WINPC_CPU 18
    #define TERMINAL_CPU 19
    #define DTC281_CPU 20
    #define ELAN_TEXT_CPU 21
    #define ELAN_800_CPU 22
    #define ETT321_CPU 23
    #define HERZ_CPU 24
    #define ETT261_CPU 25
    #define ETT431_CPU 26
    #define WIN_PC98_CPU 27
    #define WINPC_NT_CPU 28
    #define WINIPC_98_CPU 29
    #define WINIPC_NT_CPU 30
    #define IPC_320_CPU 31
    #define C_IPC_CPU 32
    #define NoCPU_33 33
    #define RK500_CPU 34
    #define CCL721_CPU 35
    #define CCL911_CPU 36
    #define BDF2000_CPU 37
    #define StandardPC_CPU 38
    #define CCL081CET_CPU 39
    #define CCL081_CPU 40
    #define CCL722_CPU 41
    #define DCL642_CPU 42
    #define Teachbox_CPU 43
    #define DTC081_CPU 44
    #define DTC081_IP_CPU 45
    #define ETV_CPU 46
    #define HZS511_CPU 47
    #define AUW11X_CPU 48
    #define BDF2000_USB_CPU 49
    #define DTC101_CPU 50
    #define HZS515_CPU 51
    #define CCP511_CPU 52
    #define ETVEDGE_CPU 53
    #define ARM_IMX6_CPU 65537

typedef struct 
  {
    long SwitchOn : 1;
    long EnableVoltage : 1;
    long QuickStop : 1;
    long EnableOperation : 1;
    long OperationModeSpec1 : 1;
    long OperationModeSpec2 : 1;
    long OperationModeSpec3 : 1;
    long ResetFault : 1;
    long Halt : 1;
    long Reserved1 : 1;
    long Reserved2 : 1;
    long ManufactureSpec1 : 1;
    long ManufactureSpec2 : 1;
    long ManufactureSpec3 : 1;
    long ManufactureSpec4 : 1;
    long ManufactureSpec5 : 1;
  }DS402_Control;

typedef struct 
  {
    long ReadyToSwitchOn : 1;
    long SwitchedOn : 1;
    long OperationEnabled : 1;
    long Fault : 1;
    long VoltageEnabled : 1;
    long QuickStop : 1;
    long SwitchOnDisabled : 1;
    long Warning : 1;
    long ManufactureSpec1 : 1;
    long Remote : 1;
    long TargetReached : 1;
    long InternalLimitActive : 1;
    long OperationModeSpec1 : 1;
    long OperationModeSpec2 : 1;
    long ManufactureSpec2 : 1;
    long ManufactureSpec3 : 1;
  }DS402_State;

typedef struct 
  {
    long Valid : 1;  //! <Type Comment="must be 1 to Start FileEx work" Name="FeSetup.Valid"/>
    long FileorRam : 1;  //! <Type Comment="0= work in File 1= Work File" Name="FeSetup.FileorRam"/>
    long EnableChecksum : 1;  //! <Type Comment="0=no Checksum; 1=Calculate Checksum, increased write access on the CF card" Name="FeSetup.EnableChecksum"/>
    long Encrypt : 1;  //! <Type Comment="0=no encryption; 1=encrypt file, increased write access on the CF card" Name="FeSetup.Encrypt"/>
  }FeSetup;

typedef struct   //! <Type Comment="Status Flag f�r IO Daten" Name="IO_FLAG"/>
  {
    short WrongHW : 1;  //! <Type Comment="Falsche Hardware verbunden" Name="IO_FLAG.WrongHW"/>
    short NoHW : 1;  //! <Type Comment="Keine Hardware verbunden" Name="IO_FLAG.NoHW"/>
    short NoCalibration : 1;  //! <Type Comment="Keine Kalibrierungsdaten im Modul-EEPROM" Name="IO_FLAG.NoCalibration"/>
    short ParaChkWrong : 1;  //! <Type Comment="Die Parameter-Checksumme ist falsch" Name="IO_FLAG.ParaChkWrong"/>
    short PhysicHiLimit : 1;  //! <Type Comment="IO ist am oberen physikalischen Limit" Name="IO_FLAG.PhysicHiLimit"/>
    short PhysicLoLimit : 1;  //! <Type Comment="IO ist am unteren physikalischen Limit" Name="IO_FLAG.PhysicLoLimit"/>
    short Invert : 1;  //! <Type Comment="Daten sind invertiert" Name="IO_FLAG.Invert"/>
    short dummy0 : 6;
    short OnDummyMode : 1;  //! <Type Comment="1 = Objekt ist im Dummymodus (nicht refreshed)" Name="IO_FLAG.OnDummyMode"/>
    short NotConnected : 1;  //! <Type Comment="1 = Objekt ist nicht verbunden" Name="IO_FLAG.NotConnected"/>
    short PhysicAccessOff : 1;  //! <Type Comment="1 = kein physikalischer Zugriff erlaubt" Name="IO_FLAG.PhysicAccessOff"/>
  }IO_FLAG;

#pragma pack(push, 1)
typedef struct IO_State 
  {
    IO_FLAG uiIO_Flags;
    UINT uiChNo;
  } IO_State;
#pragma pack(pop)

typedef HwBase *pHwBase;

typedef HwBaseCDIAS *pHwBaseCDIAS;

typedef UDINT SafetyConfigStateType; 
    #define _ModuleNotFound 0
    #define _SafetyClassOK 1
    #define _ReinitConfig 2
    #define _UnsafeVarNotFound 3
    #define _ModFromCfgNotFound 4
    #define _MemAllocFailed 5
    #define _ReadFWVerFailed 6
    #define _UnknownCfgError 7
    #define _WaitForSynchronicity 8
    #define _AsyncComError 9
    #define _DOsIncreasedRestartApp 10
    #define _LostPowerSupply 11
    #define _WrongSafetyHW 12

#pragma pack(push, 1)
typedef struct SafetyDiagInfo 
  {
    USINT uControllerID;  //! <Type Comment="ID of �Controller (0 = �C1, else �C2)" Name="SafetyDiagInfo.uControllerID"/>
    USINT ActErrorCode;  //! <Type Comment="actual error code" Name="SafetyDiagInfo.ActErrorCode"/>
    USINT FirstErrorCode;  //! <Type Comment="code of first error that lead to an error state" Name="SafetyDiagInfo.FirstErrorCode"/>
    UDINT ReasonCode0;  //! <Type Comment="reason for error (meaning depends on actual error code)" Name="SafetyDiagInfo.ReasonCode0"/>
    UDINT ReasonCode1;  //! <Type Comment="reason for error (meaning depends on actual error code)" Name="SafetyDiagInfo.ReasonCode1"/>
  } SafetyDiagInfo;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SafetyDiagState 
  {
    USINT RunState;  //! <Type Comment=" 1..POST&#13;&#10; 2..SERVICE&#13;&#10; 4..ERROR&#13;&#10; 8..IDLE&#13;&#10;16..CHK_CFG&#13;&#10;32..OP_TEMP&#13;&#10;64..OP" Name="SafetyDiagState.RunState"/>
    USINT ConfigState;  //! <Type Comment=" 1..INVALID&#13;&#10; 2..NOT_CONFIGURED&#13;&#10; 4..CONFIGURED_NOT_DEPLOYED_NOT_VERIFIED&#13;&#10; 8..CONFIGURED_AND_VERIFIED&#13;&#10;16..CONFIGURED_DEPLOYED_NOT_VERIFIED&#13;&#10;36..CONFIGURED_NOT_DEPLOYED_NOT_VERIFIED_DEV&#13;&#10;48..CONFIGURED_DEPLOYED_NOT_VERIFIED_DEV" Name="SafetyDiagState.ConfigState"/>
    USINT LoginLevel;  //! <Type Comment="0..not logged in&#13;&#10;1..debug&#13;&#10;2..configuration&#13;&#10;3..general" Name="SafetyDiagState.LoginLevel"/>
    UINT ErrorCounterIOState;  //! <Type Comment="Is increased on change of the error state of any input or output" Name="SafetyDiagState.ErrorCounterIOState"/>
  } SafetyDiagState;
#pragma pack(pop)

typedef DINT t_e_SafetyMemState; 
    #define _ALL_OK 0
    #define _NOT_ACTIVE 1
    #define _ASY_MEM_NOT_EQUAL 2
    #define _ISO_MEM_NOT_EQUAL 3
    #define _ASY_WR_SIZE_TOO_HIGH 4
    #define _ASY_RD_SIZE_TOO_HIGH 5
    #define _ISO_WR_SIZE_TOO_HIGH 6
    #define _ISO_RD_SIZE_TOO_HIGH 7
    #define _ISO_WR_SIZE_TOO_SMALL 8
    #define _ISO_RD_SIZE_TOO_SMALL 9
    #define _ASY_WR_INSTALL_ERROR 10
    #define _ASY_RD_INSTALL_ERROR 11
    #define _ISO_WR_INSTALL_ERROR 12
    #define _ISO_RD_INSTALL_ERROR 13

typedef UDINT t_e_VaranErrors; 
    #define _ClassOk 0
    #define _NotInitialized 1
    #define _CallBackError 2
    #define _RequiredError 3
    #define _RootError 4
    #define _NoHardware 5
    #define _WrongHardware 6
    #define _CreateDOFailed 7
    #define _DirectAccessFailed 8
    #define _PllError 9
    #define _TimeoutInInit 10
    #define _HardwareRequiredIRQ 11
    #define _HardwareNotRequiredIRQ 12
    #define _HardwareFatalErrorIRQ 13
    #define _ManagerError 14
    #define _DisableError 15
    #define _EnableError 16
    #define _MultipleError 17
    #define _SPIError 18
    #define _ErrorBootImageFPGA 19
    #define _NoCDIASInterface 20
    #define _EEPromError 21
    #define _NoVaranInterface 22
    #define _MemoryFault 23
    #define _TimesliceError 24
    #define _TimesliceErrorIRQ 25
    #define _WatchdogError 26
    #define _VaranTimeError 27
    #define _DiasTimeError 28
    #define _DORamFull 29
    #define _PortNoLink 30
    #define _VaranDriverNotExists 31
    #define _WrongDOLType 32
    #define _WrongRunStatus 33
    #define _DOHandleInvalid 34
    #define _DOCmdInvalid 35
    #define _ManagerNotExists 36
    #define _DOLAddressInvalid 37
    #define _UnknownCommand 38
    #define _ComponentNotExists 39
    #define _ClientNotExists 40
    #define _CdiasEEPromNotExists 41
    #define _CdiasEEPromNoGrant 42
    #define _CdiasEEPromNack 43
    #define _PortNotExists 44
    #define _PortIsUplink 45
    #define _NoMutex 46
    #define _NoTask 47
    #define _IDNotFound 48
    #define _IDNotInitialized 49
    #define _InvalidDeviceAddress 50
    #define _CallbackNotHandled 51
    #define _NoMem 52
    #define _NoLegacyWd 53
    #define _AdminDOLExecutionError 54
    #define _DADOLExecutionError 55
    #define _SPIFlashNoAccess 56
    #define _ClientNotready 57
    #define _TimeoutVaran 58
    #define _UnknownFault 59
    #define _DiasError 60
    #define _DeviceError 61
    #define _VaranTimeWarning 62
    #define _OnlyASYSupported 63
    #define _InvalidSerialNo 64
    #define _CDIASRequiredError 65
    #define _DIASRequiredError 66
    #define _NoCalibDataFound 67
    #define _ModuleFoundButManagerIsOff 68
    #define _InvalidConfguration 69
    #define _ClientDisabled 70
    #define _ClientCantEnable 71
    #define _CdiasAddressInvalid 72
    #define _SPIFlashInvalid 73
    #define _SPIDOLIDInvalid 74
    #define _SPIDOLInvalid 75
    #define _SPIRequiredListError 76
    #define _SPIChksumError 77
    #define _ParameterInvalid 78
    #define _DOTypeNotSupported 79
    #define _DMAError 80
    #define _PropertyIDInvalid 81
    #define _PropertyValueInvalid 82
    #define _DONumberOverflow 83
    #define _APIUsageNotAllowed 84
    #define _NodeTypeWrong 85
    #define _DataLengthInvalid 86
    #define _DOTypeInvalid 87
    #define _OperationNotAllowed 88
    #define _NodeNumberOverflow 89
    #define _NoHandleFound 90
    #define _TopologyNotAllowed 91

#pragma pack(push, 1)
typedef struct t_s_ModulInfo 
  {
    HINT Kennung;
    void *p_This;
  } t_s_ModulInfo;
#pragma pack(pop)
