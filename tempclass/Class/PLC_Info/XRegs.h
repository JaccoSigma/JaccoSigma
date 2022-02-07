
FUNCTION GLOBAL __cdecl funct_XREGS_BattGood
VAR_OUTPUT
  ret : DINT;
END_VAR;

FUNCTION GLOBAL __cdecl funct_XREGS_PowerGood
VAR_OUTPUT
  ret : DINT;
END_VAR;


FUNCTION GLOBAL __cdecl funct_XREGS_SETLeds
VAR_INPUT
  val : CHAR;
END_VAR
VAR_OUTPUT
  ret : UDINT;
END_VAR;

TYPE
  T_XREGS    :STRUCT
    vers    : UDINT;
    pbase   : ^void;
    pbIRQ   : ^void;
    WDog_funct    : ^funct_WatchdogTrigger;
    IsIRQ_funct   : ^funct_XREGS_IsIRQ;
    EnIRQ_funct   : ^funct_XREGS_EnableIRQ; 
    DisIRQ_funct  : ^funct_XREGS_DisableIRQ; 
    Power_funct   : ^funct_XREGS_PowerGood;  
    Batt_funct    : ^funct_XREGS_BattGood;  
    SetMode_funct : ^funct_XREGS_SetCommMode; 
    GetMode_funct : ^funct_XREGS_GetCommMode; 
    Video_funct   : ^funct_XREGS_GetVideoMode; 
    Button_funct  : ^funct_XREGS_SetButtPressed; 
    Contrast_funct: ^funct_XREGS_SetContrast; 
    LED_funct     : ^funct_XREGS_SetLEDs; 
    
  END_STRUCT;
END_TYPE
