#ifndef FDD3000_NEWINSTIF_h
#pragma once
#define FDD3000_NEWINSTIF_h

#define _FDD3000_IF_PROTOCOLVERSION 16#0001

//*****************************************************************************
//** NewInst 0x8490-0x849F                                                   **
//*****************************************************************************

//*************************************************************************************
//*********** NewInst Commands

#define _FDD3000_IF_GET_PROPERTY            16#8490   //get one property from FDD3000 class
#define _FDD3000_IF_SET_PROPERTY            16#8491   //set one property at FDD3000 class  
//not implemented #define _FDD3000_IF_GET_PROPERTIES          16#8492   //get more properties from FDD3000 class
//not implemented #define _FDD3000_IF_SET_PROPERTIES          16#8493   //set more properties at FDD3000 class                      
                                                                        
//*************************************************************************************
//commands for Logdata
#define _FDD3000_IF_LOGDATA_GETFILEPOINTER  16#8494  //get pointer to log data
#define _FDD3000_IF_LOGDATA_CLEARLOGBUFFER  16#8495  //clear log buffer

//*************************************************************************************
//commands for Filehandling
#define _FDD3000_IF_FH_SETFILENAME          16#8496  //set own file name for read/write file


// Parameters for commands *************************
// 
//  _FDD3000_IF_GET_PROPERTY *****************
//  Command Version 1 **  
//  CmdStruct
//  aPara[0]$DINT  : Command Version : 1  
//  aPara[1]$DINT  : Property ID
//  aPara[2]$DINT  : data length
//  aPara[3]$^DINT : pointer to copy data
//  
//  results
//  uiLng         : 6 Byte (sizeof(HINT)+sizeof(DINT))
//  aPara[0]$HINT : NewInst version of FDDD3000
//  aPara[2]$DINT : return Code 
//                    0 .. ok
//                   -1 .. property id is not available
//                   -2 .. property id is not readable
//                   -3 .. pointer is not valid
//                   -4 .. datalength is smaller then 4 byte
//                   -6 .. datalength is to small for available data
// 
//  
//  _FDD3000_IF_SET_PROPERTY *****************
//  Command Version 1 **  
//  CmdStruct
//  aPara[0]$DINT  : Command Version : 1  
//  aPara[1]$DINT  : Property ID
//  aPara[2]$DINT  : data length
//  aPara[3]$^DINT : pointer to copy data
//    
//  results
//  uiLng         : 6 Byte (sizeof(HINT)+sizeof(DINT))
//  aPara[0]$HINT : NewInst version of FDDD3000
//  aPara[2]$DINT : return Code
//                    0 .. ok
//                   -1 .. property id is not available
//                   -2 .. property id is not writeable
//                   -3 .. pointer is not valid
//                   -4 .. datalength is smaller then 4 byte
//                   -5 .. VOV is active - write of property is not possible
//                   
//   
// 
//  _FDD3000_IF_LOGDATA_GETFILEPOINTER *****************
//  Command Version 1 **  
//  CmdStruct
//  aPara[0]$DINT  : Command Version : 1  
//  
//  results  
//  uiLng         : 6 Byte (sizeof(HINT)+sizeof(DINT))
//  aPara[0]$HINT : NewInst version of FDDD3000
//  aPara[2]$DINT : Pointer to Logfile 
// 
// 
//  _FDD3000_IF_LOGDATA_CLEARLOGBUFFER *****************
//  Command Version 1 **  
//  CmdStruct
//  aPara[0]$DINT  : Command Version : 1  
//  
//  results
//  uiLng         : 2 Byte (sizeof(HINT))
//  aPara[0]$HINT : NewInst version of FDDD3000
// 
// 
//  _FDD3000_IF_FH_SETFILENAME *****************
//  Command Version 1 **  
//  CmdStruct
//  aPara[0]$DINT  : Command Version : 1  
//  aPara[1]$^CHAR  : Pointer to file name ^CHAR
//  aPara[2]$^CHAR  : pointer to filepath ^CHAR
//    
//  results
//  uiLng         : 6 Byte (sizeof(HINT)+sizeof(DINT))
//  aPara[0]$HINT : NewInst version of FDDD3000
//  aPara[2]$DINT : return Code 0..Error 1..OK
// 

//****************************************************************************
#define _FDD3000_IF_PROPERTY_ID_DEVICE_STATE                              1  //ReadOnly     Depends on Server "DeviceState" of class
#define _FDD3000_IF_PROPERTY_ID_ERROR_QUIT                                2  //ReadWrite    Depends on Server "ErrorQuit" of class
#define _FDD3000_IF_PROPERTY_ID_CANDISABLED                               3  //ReadOnly     Depends on Server "CanDisbled" of class
#define _FDD3000_IF_PROPERTY_ID_DATAREADY                                 4  //ReadOnly     Depends on Server "DataReady" of class
#define _FDD3000_IF_PROPERTY_ID_ASYNCSTATE                                5  //ReadOnly     Depends on Server "AsyncState" of class
#define _FDD3000_IF_PROPERTY_ID_ERROR_SDO                                 6  //ReadOnly     Depends on Server "ErrorSDO" of class
#define _FDD3000_IF_PROPERTY_ID_ERROR_ALLOC                               7  //ReadOnly     Depends on Server "ErrorAlloc" of class
#define _FDD3000_IF_PROPERTY_ID_EMCYSTATUSBYTE1                           8  //ReadOnly     Depends on Server "EmcyStatusByte0_3" of class
#define _FDD3000_IF_PROPERTY_ID_EMCYSTATUSBYTE2                           9  //ReadOnly     Depends on Server "EmcyStatusByte4_7" of class
#define _FDD3000_IF_PROPERTY_ID_PDO_DATA_1                               10  //ReadWrite
#define _FDD3000_IF_PROPERTY_ID_PDO_DATA_2                               11  //ReadWrite
#define _FDD3000_IF_PROPERTY_ID_PDO_DATA_3                               12  //ReadWrite
#define _FDD3000_IF_PROPERTY_ID_PDO_DATA_4                               13  //ReadWrite

//*************************************************************************************
//*********** Get/Set Property ID's DS402
#define _FDD3000_IF_PROPERTY_ID_PROFILEDRIVEMODE                         50  //ReadOnly     Depends on Server "DriveMode" of class
#define _FDD3000_IF_PROPERTY_ID_CONTROLWORD_DS402                        51  //ReadWrite    Depends on Server "ControlWord" of class
#define _FDD3000_IF_PROPERTY_ID_STATUSWORD_DS402                         52  //ReadOnly     Depends on Server "StatusWord" of class
#define _FDD3000_IF_PROPERTY_ID_DRIVEERROR                               53  //ReadOnly     Depends on Server "DriveError" of class
#define _FDD3000_IF_PROPERTY_ID_DRIVEERRORSTATUS                         54  //ReadOnly     Depends on Server "DriveErrorStatus" of class
#define _FDD3000_IF_PROPERTY_ID_ENABLE                                   55  //ReadWrite    Depends on Server "Enable" of class
#define _FDD3000_IF_PROPERTY_ID_POSITION                                 56  //ReadWrite    Depends on Server "Position" of class
#define _FDD3000_IF_PROPERTY_ID_SPEED                                    57  //ReadWrite    Depends on Server "Speed" of class
#define _FDD3000_IF_PROPERTY_ID_CURRENT                                  58  //ReadWrite    Depends on Server "Current" of class
#define _FDD3000_IF_PROPERTY_ID_DESTINATIONPOS                           59  //ReadWrite    Depends on Server "DestinationPos" of class
#define _FDD3000_IF_PROPERTY_ID_DESTINATIONSPEED                         60  //ReadWrite    Depends on Server "DestinationSpeed" of class
#define _FDD3000_IF_PROPERTY_ID_DESTINATIONACCEL                         61  //ReadWrite    Depends on Server "DestinationAccel" of class
#define _FDD3000_IF_PROPERTY_ID_DESTINATIONDECEL                         62  //ReadWrite    Depends on Server "DestinationDecel" of class
#define _FDD3000_IF_PROPERTY_ID_STOP                                     63  //ReadWrite    Depends on Server "Stop" of class

//*************************************************************************************
//*********** Get/Set Property ID's FDD3000 
#define _FDD3000_IF_PROPERTY_ID_COMM_MODE                               100  //ReadOnly     get FDD3000 type of communication (CAN-ETHERCAT-VARAN)
#define _FDD3000_IF_PROPERTY_ID_STO_INPUT                               101  //ReadOnly     get STO signal: 1 = both hw inputs are high; 0 = at least 1 input is low
#define _FDD3000_IF_PROPERTY_ID_STR_PRODUCTCODE                         102  //ReadOnly     Depends on Server "ProductCodeDrive" of class                   string of product code
#define _FDD3000_IF_PROPERTY_ID_STR_SERIALNO                            103  //ReadOnly     Depends on Server "SerialNoDrive" of class                      string of serial number
#define _FDD3000_IF_PROPERTY_ID_FWVERSIONDRIVE                          104  //ReadOnly     Depends on Server "FwVersionDrive" of class
#define _FDD3000_IF_PROPERTY_ID_FWVERSIONCOMM                           105  //ReadOnly     Depends on Server "FwVersionComm" of class
#define _FDD3000_IF_PROPERTY_ID_HWVERSIONCOMM                           106  //ReadOnly     Depends on Server "HwVersionComm" of class
#define _FDD3000_IF_PROPERTY_ID_DRIVEMODE                               107  //ReadOnly     Depends on Server "OPModeDrive" of class                       OpenLoop or RFC-A Mode
#define _FDD3000_IF_PROPERTY_ID_AXERROR                                 108  //ReadOnly     Depends on Server "DriveError" of class   
#define _FDD3000_IF_PROPERTY_ID_QUITAXERROR                             109  //ReadWrite    Depends on Server "QuitError" of class   
#define _FDD3000_IF_PROPERTY_ID_ONLINE                                  110  //ReadOnly     Depends on Server "Online" of class   
#define _FDD3000_IF_PROPERTY_ID_ERROR_SDO_CODE                          111  //ReadOnly     Depends on Server "ErrorSDO" of class

//*************************************************************************************
//act values on hwk
#define _FDD3000_IF_PROPERTY_ID_HWK_MAXREFCLAMP                         120  //ReadWrite    Depends on Server "MaxRefClamp" of class   
#define _FDD3000_IF_PROPERTY_ID_HWK_MINREFCLAMP                         121  //ReadWrite    Depends on Server "MinRefClamp" of class   
#define _FDD3000_IF_PROPERTY_ID_HWK_ACCRATE                             122  //ReadWrite    Depends on Server "AccRate" of class   
#define _FDD3000_IF_PROPERTY_ID_HWK_DECRATE                             123  //ReadWrite    Depends on Server "DecRate" of class   
#define _FDD3000_IF_PROPERTY_ID_HWK_MOTORRATEDCURRENT                   124  //ReadWrite    Depends on Server "MotorRatedCurrent" of class   
#define _FDD3000_IF_PROPERTY_ID_HWK_MOTORRATEDSPEED                     125  //ReadWrite    Depends on Server "MotorRatedSpeed" of class   
#define _FDD3000_IF_PROPERTY_ID_HWK_MOTORRATEDVOLTAGE                   126  //ReadWrite    Depends on Server "MotorRatedVoltage" of class   
#define _FDD3000_IF_PROPERTY_ID_HWK_MOTORRATEDPOWERFACTOR               127  //ReadWrite    Depends on Server "MotorRatedPowerFactor" of class   
#define _FDD3000_IF_PROPERTY_ID_HWK_MOTORRATEDFREQUENCY                 128  //ReadWrite    Depends on Server "MotorRatedFrequency" of class   
#define _FDD3000_IF_PROPERTY_ID_HWK_LOWFREQUENCYVOLTAGEBOOST            129  //ReadWrite    Depends on Server "LowFrequencyVoltageBoost" of class   
#define _FDD3000_IF_PROPERTY_ID_HWK_STATORRESISTANCE                    130  //ReadWrite    Depends on Server "StatorResistance" of class                   openloop - RFC-A
#define _FDD3000_IF_PROPERTY_ID_HWK_TRANSIENTINDUCTANCE                 131  //ReadWrite    Depends on Server "TransientResistance" of class                openloop - RFC-A
#define _FDD3000_IF_PROPERTY_ID_HWK_STATORINDUCTANCE                    132  //ReadWrite    Depends on Server "StatorInductance" of class                   openloop - RFC-A
#define _FDD3000_IF_PROPERTY_ID_HWK_MAXIMUMDEADTIMECOMP                 133  //ReadOnly     Depends on Server "MaximumDeadTimeComp" of class                openloop
#define _FDD3000_IF_PROPERTY_ID_HWK_CURRENTATMAXIMUMDEADTIME            134  //ReadOnly     Depends on Server "CurrentAtMaximumDeadtime" of class           openloop
#define _FDD3000_IF_PROPERTY_ID_HWK_CURRENTLOOPKPGAIN                   135  //ReadWrite    Depends on Server "CurrentLoopKPGain" of class                  RFC-A
#define _FDD3000_IF_PROPERTY_ID_HWK_CURRENTLOOPKIGAIN                   136  //ReadWrite    Depends on Server "CurrentLoopKiGain" of class                  RFC-A
#define _FDD3000_IF_PROPERTY_ID_HWK_SATURATIONBREAKPOINT1               137  //ReadWrite    Depends on Server "SaturationBreakpoint1" of class              RFC-A
#define _FDD3000_IF_PROPERTY_ID_HWK_SATURATIONBREAKPOINT3               138  //ReadWrite    Depends on Server "SaturationBreakpoint3" of class              RFC-A
#define _FDD3000_IF_PROPERTY_ID_HWK_SATURATIONBREAKPOINT2               139  //ReadWrite    Depends on Server "SaturationBreakpoint2" of class              RFC-A
#define _FDD3000_IF_PROPERTY_ID_HWK_SATURATIONBREAKPOINT4               140  //ReadWrite    Depends on Server "SaturationBreakpoint4" of class              RFC-A

//*************************************************************************************
//act values on drive
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_MAXREFCLAMP                    150  //ReadOnly     Depends on Server "MaxRefClamp" of internal class   
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_MINREFCLAMP                    151  //ReadOnly     Depends on Server "MinRefClamp" of internal class   
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_ACCRATE                        152  //ReadOnly     Depends on Server "AccRate" of internal class   
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_DECRATE                        153  //ReadOnly     Depends on Server "DecRate" of internal class   
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_MOTORRATEDCURRENT              154  //ReadOnly     Depends on Server "MotorRatedCurrent" of internal class   
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_MOTORRATEDSPEED                155  //ReadOnly     Depends on Server "MotorRatedSpeed" of internal class   
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_MOTORRATEDVOLTAGE              156  //ReadOnly     Depends on Server "MotorRatedVoltage" of internal class   
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_MOTORRATEDPOWERFACTOR          157  //ReadOnly     Depends on Server "MotorRatedPowerFactor" of internal class   
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_MOTORRATEDFREQUENCY            158  //ReadOnly     Depends on Server "MotorRatedFrequency" of internal class   
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_LOWFREQUENCYVOLTAGEBOOST       159  //ReadOnly     Depends on Server "LowFrequencyVoltageBoost" of internal class   
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_STATORRESISTANCE               160  //ReadOnly     Depends on Server "StatorResistance" of internal class            openloop - RFC-A
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_TRANSIENTINDUCTANCE            161  //ReadOnly     Depends on Server "TransientResistance" of internal class         openloop - RFC-A
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_STATORINDUCTANCE               162  //ReadOnly     Depends on Server "StatorInductance" of internal class            openloop - RFC-A
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_MAXIMUMDEADTIMECOMP            163  //ReadOnly     Depends on Server "MaximumDeadTimeComp" of internal class         openloop
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_CURRENTATMAXIMUMDEADTIME       164  //ReadOnly     Depends on Server "CurrentAtMaximumDeadtime" of internal class    openloop
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_CURRENTLOOPKPGAIN              165  //ReadOnly     Depends on Server "CurrentLoopKPGain" of internal class           RFC-A
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_CURRENTLOOPKIGAIN              166  //ReadOnly     Depends on Server "CurrentLoopKiGain" of internal class           RFC-A
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_SATURATIONBREAKPOINT1          167  //ReadOnly     Depends on Server "SaturationBreakpoint1" of internal class       RFC-A
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_SATURATIONBREAKPOINT3          168  //ReadOnly     Depends on Server "SaturationBreakpoint3" of internal class       RFC-A
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_SATURATIONBREAKPOINT2          169  //ReadOnly     Depends on Server "SaturationBreakpoint2" of internal class       RFC-A
#define _FDD3000_IF_PROPERTY_ID_ACTDRIVE_SATURATIONBREAKPOINT4          170  //ReadOnly     Depends on Server "SaturationBreakpoint4" of internal class       RFC-A

//*************************************************************************************
//old values from drive
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_MAXREFCLAMP                    180  //ReadOnly     Depends on Server "MaxRefClamp" of internal class   
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_MINREFCLAMP                    181  //ReadOnly     Depends on Server "MinRefClamp" of internal class   
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_ACCRATE                        182  //ReadOnly     Depends on Server "AccRate" of internal class   
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_DECRATE                        183  //ReadOnly     Depends on Server "DecRate" of internal class   
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_MOTORRATEDCURRENT              184  //ReadOnly     Depends on Server "MotorRatedCurrent" of internal class   
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_MOTORRATEDSPEED                185  //ReadOnly     Depends on Server "MotorRatedSpeed" of internal class   
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_MOTORRATEDVOLTAGE              186  //ReadOnly     Depends on Server "MotorRatedVoltage" of internal class   
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_MOTORRATEDPOWERFACTOR          187  //ReadOnly     Depends on Server "MotorRatedPowerFactor" of internal class   
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_MOTORRATEDFREQUENCY            188  //ReadOnly     Depends on Server "MotorRatedFrequency" of internal class   
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_LOWFREQUENCYVOLTAGEBOOST       189  //ReadOnly     Depends on Server "LowFrequencyVoltageBoost" of internal class   
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_STATORRESISTANCE               190  //ReadOnly     Depends on Server "StatorResistance" of internal class            openloop - RFC-A
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_TRANSIENTINDUCTANCE            191  //ReadOnly     Depends on Server "TransientResistance" of internal class         openloop - RFC-A
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_STATORINDUCTANCE               192  //ReadOnly     Depends on Server "StatorInductance" of internal class            openloop - RFC-A
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_MAXIMUMDEADTIMECOMP            193  //ReadOnly     Depends on Server "MaximumDeadTimeComp" of internal class         openloop
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_CURRENTATMAXIMUMDEADTIME       194  //ReadOnly     Depends on Server "CurrentAtMaximumDeadtime" of internal class    openloop
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_CURRENTLOOPKPGAIN              195  //ReadOnly     Depends on Server "CurrentLoopKPGain" of internal class           RFC-A
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_CURRENTLOOPKIGAIN              196  //ReadOnly     Depends on Server "CurrentLoopKiGain" of internal class           RFC-A
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_SATURATIONBREAKPOINT1          197  //ReadOnly     Depends on Server "SaturationBreakpoint1" of internal class       RFC-A
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_SATURATIONBREAKPOINT3          198  //ReadOnly     Depends on Server "SaturationBreakpoint3" of internal class       RFC-A
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_SATURATIONBREAKPOINT2          199  //ReadOnly     Depends on Server "SaturationBreakpoint2" of internal class       RFC-A
#define _FDD3000_IF_PROPERTY_ID_OLDDRIVE_SATURATIONBREAKPOINT4          200  //ReadOnly     Depends on Server "SaturationBreakpoint4" of internal class       RFC-A

//*************************************************************************************
//commands for setup node - autotune - store node parameters (VOV Control)            
#define _FDD3000_IF_PROPERTY_ID_VOV_ACTIVE                              210  //ReadWrite    Depends on Server "VOV_Active" of internal class                  Set VOV active mode - due to this all write NewInst Commands are blocked
#define _FDD3000_IF_PROPERTY_ID_ACTCYCLICSTEP                           211  //ReadOnly     Depends on Server "ActCyclicStep" of internal class               Shows actual cyclic step
#define _FDD3000_IF_PROPERTY_ID_START_LSS                               212  //ReadWrite    Depends on Server "StartLSS" of internal class                    Is used to start Layer Setting Service for configuration of Node Number and Node Bitrate
#define _FDD3000_IF_PROPERTY_ID_ACTLLSSTEP                              213  //ReadOnly     Depends on Server "ActCyclicStep" of internal class               Shows actual LSS step
#define _FDD3000_IF_PROPERTY_ID_SETNODEID                               214  //ReadOnly     Depends on Server "SetNodeID" of internal class                   Node ID which is set with LSS. Is taken from hwk object
#define _FDD3000_IF_PROPERTY_ID_SETBITRATE                              215  //ReadOnly     Depends on Server "SetBitrate" of internal class                  Bitrate which is set with LSS. Is taken from _CanOpenMng
#define _FDD3000_IF_PROPERTY_ID_ACTSEARCHBITRATE                        216  //ReadOnly     Depends on Server "ActSearchBitrate" of internal class            Shows the actual searching bitrate when LSS is executed
//Autotune - Parameter Handling                                                                                  
#define _FDD3000_IF_PROPERTY_ID_STARTAUTOTUNE                           220  //ReadWrite    Depends on Server "StartAutotune" of internal class               Start Autotune handling
#define _FDD3000_IF_PROPERTY_ID_ACTAUTOTUNESTEP                         221  //ReadOnly     Depends on Server "ActAutotuneStep" of internal class             Shows the actual Autotune step
#define _FDD3000_IF_PROPERTY_ID_SETDRIVEMODE                            222  //ReadWrite    Depends on Server "SetDriveMode" of internal class                Set Drive Operating mode (Openloop / RFC-A) 
#define _FDD3000_IF_PROPERTY_ID_SETUSERMOTORPARAS                       223  //ReadWrite    Depends on Server "SetUserMotorParas" of internal class           Start Parameter - File handling
#define _FDD3000_IF_PROPERTY_ID_DATASOURCE                              224  //ReadWrite    Depends on Server "DataSource" of internal class                  Select Datasource for Parameter - File handling
#define _FDD3000_IF_PROPERTY_ID_FILESELECT                              225  //ReadWrite    Depends on Server "FileSelect" of internal class                  Select File for read/write 1..with serial nbr, 2..with general tag
#define _FDD3000_IF_PROPERTY_ID_MAXSPEED                                226  //ReadOnly     Depends on Server "MaxSpeed" of internal class                    
#define _FDD3000_IF_PROPERTY_ID_MINSPEED                                227  //ReadOnly     Depends on Server "MinSpeed" of internal class                
#define _FDD3000_IF_PROPERTY_ID_ACTLOGDATA                              228  //ReadOnly     Depends on Server "ActLogData" of internal class                       

//*************************************************************************************
//commands ECAT Specific 
#define _FDD3000_IF_PROPERTY_ID_ECAT_CLASSSTATE                         300  //ReadOnly
#define _FDD3000_IF_PROPERTY_ID_ECAT_ETHERCATSTATE                      301  //ReadOnly
#define _FDD3000_IF_PROPERTY_ID_ECAT_SLAVESTATE                         302  //ReadOnly
#define _FDD3000_IF_PROPERTY_ID_ECAT_DEVICEADRESS                       303  //ReadOnly
#define _FDD3000_IF_PROPERTY_ID_ECAT_VENDORID                           304  //ReadOnly
#define _FDD3000_IF_PROPERTY_ID_ECAT_PRODUCTCODE                        305  //ReadOnly
#define _FDD3000_IF_PROPERTY_ID_ECAT_REVISIONNO                         306  //ReadOnly
#define _FDD3000_IF_PROPERTY_ID_ECAT_SERIALNO                           307  //ReadOnly
#define _FDD3000_IF_PROPERTY_ID_ECAT_STR_DEVICENAME                     308  //ReadOnly
#define _FDD3000_IF_PROPERTY_ID_ECAT_STR_PRODUCTREVISION                309  //ReadOnly
#define _FDD3000_IF_PROPERTY_ID_ECAT_AL_STATUSCODE                      310  //ReadOnly
                                                                        
//*****************************************************************************
//** TYPES                                                                   **
//*****************************************************************************
                                                                        
	TYPE                                                                  
                                                                        
    t_FDD3000_IF_COMMTYPE :                                             
    (                                                                   
      _COMMTYPE_UNKNOWN,
      _COMMTYPE_CANOPEN,
      _COMMTYPE_ETHERCAT,
      _COMMTYPE_VARAN
    )$UDINT;
        
  END_TYPE 
#endif