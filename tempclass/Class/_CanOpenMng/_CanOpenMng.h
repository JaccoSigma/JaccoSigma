#ifndef _CANOPENMNG_H
#pragma once
#define _CANOPENMNG_H

#define  CO_MAX_CAN_OBJECTS          (30)  //maximum number of connected _CanOpenBasicDS301-Objects

//Nmt defines
#define CO_STARTREMOTENODE          16#01   // Start Remote Node
#define CO_STOPREMOTENODE           16#02   // Stop Remote Node
#define CO_SETPREOPERATIONAL        16#80   // set node in Pre-Operational
#define CO_RESETNODE                16#81   // Reset Node
#define CO_RESETCOMMUNICATION       16#82   // Reset Communication

//hearbeat defines
#define CO_HB_BOOT_UP               16#00
#define CO_HB_STOPPED               16#04  
#define CO_HB_OPERATIONAL           16#05
#define CO_HB_PREOPERATIONAL        16#7F

#define CO_SYNC_OBJNR                0x80   // default COB-ID for sync message
#define CO_WAIT_FOR_SYNC            (1000)  // time to wait for sync


#define CO_CAN_Device_TIMEOUT        4000   // timout for sending sdos
#define CO_CAN_DEVICE_NOANSWER        -1    // can device is not available



// ------------------------------------------------------------------------------------------------------------------
// RECEIVE PDO's DRIVE --> PLC --------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
// Receive Object Numbers -------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
#define CO_RPDO1_OBJNR             16#180   // default COB-ID for RPDO1 (From can node view it's a TPDO)
#define CO_RPDO2_OBJNR             16#280   // default COB-ID for RPDO2 (From can node view it's a TPDO)
#define CO_RPDO3_OBJNR             16#380   // default COB-ID for RPDO3 (From can node view it's a TPDO)
#define CO_RPDO4_OBJNR             16#480   // default COB-ID for RPDO4 (From can node view it's a TPDO)

// ------------------------------------------------------------------------------------------------------------------
// TRANSMIT PDO's PLC --> DRIVE -------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
// Transmit Object Numbers ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
#define CO_TPDO1_OBJNR             16#200   // default COB-ID for TPDO1 (From can node view it's a RPDO)
#define CO_TPDO2_OBJNR             16#300   // default COB-ID for TPDO2 (From can node view it's a RPDO)
#define CO_TPDO3_OBJNR             16#400   // default COB-ID for TPDO3 (From can node view it's a RPDO)
#define CO_TPDO4_OBJNR             16#500   // default COB-ID for TPDO4 (From can node view it's a RPDO)

// ------------------------------------------------------------------------------------------------------------------
// SDO Object Numbers ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
#define CO_RSDO_OBJNR              16#580   // default COB-ID for RSDO
#define CO_TSDO_OBJNR              16#600   // default COB-ID for TSDO

#define CO_NG_OBJNR                16#700   // default COB-ID for NODE GUARDING
#define CO_EMCY_OBJNR               16#80   // default COB-ID for EMERGENCY OBJECT 

#define CO_SDO_RW_ERROR             16#80   // RETURN CODE OF SDO IF WE HAVE AN READ WRITE ERROR

#define CO_CANPDOTIMEOUT			         5	  // Timeout Zeit für die PDO Objekte in ms
#define CO_CANPDOTIMEOUT_FIRSTPDO   2000	  // Timeout Zeit für das erste PDO das nach Operationa übertragen wird in ms

#define CO_CANRESETFAULTTIMEOUT		    50	  // Timeout für ResetFault

#define CO_CANMAXSDOOBJ               10    // Max. SDO objects


#define CO_MAP_PARA_PDO_INACTIVE      16#C0000000     // Offset data inactive
#define CO_MAP_PARA_PDO_ACTIVE        16#40000000     // Offset data active

// ------------------------------------------------------------------------------------------------------------------
// TRANSMIT PDO's PLC --> CanOpen device -------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
// TPDO1 ------------------------------------------------------------------------------------------------------------
#define CO_INDEX_TPDO1_SETTING        16#1400
#define CO_TRANSMISSION_TYPE_TPDO1    16#01           // 16#FF Event triggered , 16#01 Sent by Sync
#define CO_INDEX_TPDO1_CHOICE         16#2600
#define CO_CHOICE_TPDO1_PARA          16#01           // CHOOSE EITHER AN DEFAULT PDO OR AN FREE MAPPABLE ONE
#define CO_INDEX_TPDO1_MAPPING        16#1600


// TPDO2 ------------------------------------------------------------------------------------------------------------
#define CO_INDEX_TPDO2_SETTING        16#1401
#define CO_TRANSMISSION_TYPE_TPDO2    16#01           // 16#FF Event triggered , 16#01 Sent by Sync
#define CO_INDEX_TPDO2_CHOICE         16#2601
#define CO_CHOICE_TPDO2_PARA          16#26           // CHOOSE EITHER AN DEFAULT PDO OR AN FREE MAPPABLE ONE
#define CO_INDEX_TPDO2_MAPPING        16#1601


// TPDO3 ------------------------------------------------------------------------------------------------------------
#define CO_INDEX_TPDO3_SETTING        16#1402
#define CO_TRANSMISSION_TYPE_TPDO3    16#01           // 16#FF Event triggered , 16#01 Sent by Sync
#define CO_INDEX_TPDO3_CHOICE         16#2602
#define CO_CHOICE_TPDO3_PARA          16#27           // CHOOSE EITHER AN DEFAULT PDO OR AN FREE MAPPABLE ONE
#define CO_INDEX_TPDO3_MAPPING        16#1602


// TPDO4  ------------------------------------------------------------------------------------------------------------
#define CO_INDEX_TPDO4_SETTING        16#1403
#define CO_TRANSMISSION_TYPE_TPDO4    16#01           // 16#FF Event triggered , 16#01 Sent by Sync
#define CO_INDEX_TPDO4_CHOICE         16#2603
#define CO_CHOICE_TPDO4_PARA          16#28           // CHOOSE EITHER AN DEFAULT PDO OR AN FREE MAPPABLE ONE
#define CO_INDEX_TPDO4_MAPPING        16#1603


// ------------------------------------------------------------------------------------------------------------------
// RECEIVE CanOpen device --> PLC --------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
// RPDO1 ------------------------------------------------------------------------------------------------------------
#define CO_INDEX_RPDO1_SETTING        16#1800         // Communication parameter index
#define CO_TRANSMISSION_TYPE_RPDO1    16#01           // 16#FF Event triggered , 16#01 Sent by Sync
#define CO_INDEX_RPDO1_CHOICE         16#2A00         //
#define CO_CHOICE_RPDO1_PARA          16#17           // CHOOSE EITHER AN DEFAULT PDO OR AN FREE MAPPABLE ONE
#define CO_INDEX_RPDO1_MAPPING        16#1A00         // Mapping parameter index

// RPDO2 ------------------------------------------------------------------------------------------------------------
#define CO_INDEX_RPDO2_SETTING        16#1801         // Communication parameter index
#define CO_TRANSMISSION_TYPE_RPDO2    16#01           // 16#FF Event triggered , 16#01 Sent by Sync
#define CO_INDEX_RPDO2_CHOICE         16#2A01         // 
#define CO_CHOICE_RPDO2_PARA          16#25           // CHOOSE EITHER AN DEFAULT PDO OR AN FREE MAPPABLE ONE
#define CO_INDEX_RPDO2_MAPPING        16#1A01         // Mapping parameter index

// RPDO3 ------------------------------------------------------------------------------------------------------------
#define CO_INDEX_RPDO3_SETTING        16#1802         // Communication parameter index
#define CO_TRANSMISSION_TYPE_RPDO3    16#01           // 16#FF Event triggered , 16#01 Sent by Sync
#define CO_INDEX_RPDO3_CHOICE         16#2A02         //
#define CO_CHOICE_RPDO3_PARA          16#0            // Not defined
#define CO_INDEX_RPDO3_MAPPING        16#1A02         // Mapping parameter index

// RPDO4 ------------------------------------------------------------------------------------------------------------
#define CO_INDEX_RPDO4_SETTING        16#1803         // Communication parameter index
#define CO_TRANSMISSION_TYPE_RPDO4    16#01           // 16#FF Event triggered , 16#01 Sent by Sync
#define CO_INDEX_RPDO4_CHOICE         16#2A03         //
#define CO_CHOICE_RPDO4_PARA          16#0            // Not defined
#define CO_INDEX_RPDO4_MAPPING        16#1A03         // Mapping parameter index



#define CO_INDEX_DRIVEMODE_W          16#6060         // WRITE DRIVE MODE 
#define CO_INDEX_DRIVEMODE_R          16#6061         // READ DRIVE MODE
#define CO_DEFAULT_SUBINDEX           16#00

// ------------------------------------------------------------------------------------------------------------------
// TRANSMIT PDO's PLC --> DRIVE -------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
// TPDO1 ------------------------------------------------------------------------------------------------------------
#define CO_MAP_PARA_TPDO1_1           16#60400010     // Controlword

// TPDO2 ------------------------------------------------------------------------------------------------------------
#define CO_MAP_PARA_TPDO2_1           16#607A0020;    // Target position for Mode pp  607A..IndexNr 00..SubIndex 20..32Bit
#define CO_MAP_PARA_TPDO2_2           16#60810020;    // Velocity for Mode pp         6081..IndexNr 00..SubIndex 20..32Bit

// TPDO3 ------------------------------------------------------------------------------------------------------------
#define CO_MAP_PARA_TPDO3_1           16#60FF0020;    // Target velocity für Mode vl

// TPDO4  ------------------------------------------------------------------------------------------------------------
#define CO_MAP_PARA_TPDO4_1           16#60830020;    // Accel Mode pp
#define CO_MAP_PARA_TPDO4_2           16#60840020;    // Decel Mode pp

// ------------------------------------------------------------------------------------------------------------------
// RECEIVE PDO's DRIVE --> PLC --------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
// RPDO1 ------------------------------------------------------------------------------------------------------------
#define CO_MAP_PARA_RPDO1_1           16#60410010     // Statusword

// RPDO2 ------------------------------------------------------------------------------------------------------------
#define CO_MAP_PARA_RPDO2_1           16#20700320     // Incremental Demand value  2070..IndexNr 03..SubIndex 20..32Bit
#define CO_MAP_PARA_RPDO2_2           16#20700218;    // rpm                       2070..IndexNr 00..SubIndex 18..24Bit

// RPDO3 ------------------------------------------------------------------------------------------------------------
#define CO_MAP_PARA_RPDO3_1           16#0            // Not defined
#define CO_MAP_PARA_RPDO3_2           16#0;           // Not defined

// RPDO4 ------------------------------------------------------------------------------------------------------------
#define CO_MAP_PARA_RPDO4_1           16#0            // Not defined
#define CO_MAP_PARA_RPDO4_2           16#0;           // Not defined

// ------------------------------------------------------------------------------------------------------------------
// HOMING PARA ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
// REF OFFSET -------------------------------------------------------------------------------------------------------
#define CO_INDEX_REFOFFSET                16#607C     // default index for reference offset
                          
// REFMOPDE -------------------------------------------------------------------------------------------------------------------
#define CO_INDEX_REFMODE                  16#6098     // default index for reference mode
                          
// SPEED1 ---------------------------------------------------------------------------------------------------------------------
#define CO_INDEX_REFSPEED                 16#6099     // default index for reference speed
                          
// ACCEL ----------------------------------------------------------------------------------------------------------------------
#define CO_INDEX_REFACCEL                 16#609A     // default index for reference acceleration


#define CO_DS402_TIMEOUT_DRIVEMODECHANGE         1000 // maximal wait time for change of drivemode
                                         
#define CO_TRANSMISSION_TYPE_EVERYSYNC          16#01 // 16#01 Sent by every Sync message
#define CO_TRANSMISSION_TYPE_EVENT              16#FF // 16#FF Event triggered
                                         
#define CO_XDC_VALUES_TYPE_UNKNOWN                 0  // datatype of value in xdc file is unknown
#define CO_XDC_VALUES_TYPE_DEC                     5  // datatype of value in xdc file is decimal
#define CO_XDC_VALUES_TYPE_HEX                     7  // datatype of value in xdc file is hexadezimal
#define CO_XDC_VALUES_TYPE_STR_BOOL             1001  // datatype of value in xdc file is string (False,True) to bool
                                         
#define CO_MAX_PLACE_NUMBER                      127
#define CO_MAX_PDO_MAPPING_ENTRIES                 9  //maximum number of pdo mapping parameters (largest subnumber + 8 parameters)
#define CO_MAX_PDO_OBJECTS                        32  //maximum number of pdo objects
                                          
#define CO_XDC_MAX_OBJECT_INDEXES                256  //maximum number of indexes which are stored from xdc file
#define CO_XDC_MAX_STRING_LENGTH                  64  //maximum size of one string
#define CO_XDC_CONVERT_INVALIDVALUE      16#F8000010  //invalid value if conversion failed
#define CO_MAX_STRING_LENGTH                     255  //max length of a string
                                          
#define CO_PDO_MAPPING_OFFSET_TO_COBID         0x200  // offset from PDO communication parameter to PDO mapping
#define CO_MAP_PARA_PDO_VALID_FLAG       16#80000000  // Bit 31 = 0: valid PDO exists / is valid 
                                                      // Bit 31 = 1: PDO does not exist / is not valid 
#define CO_MAX_LOGGING_ENTRIES                   16   // maximum of entries for sdo error buffer pipe
#define CO_MAX_CANOPEN_DATASIZE                   8   // max size of one can open object

//pdo communication parameter
#define CO_SUBINDEX_HIGHEST_SUBNBR             0x00   // CanOpen subindex number for highest sub index number
#define CO_SUBINDEX_COB_ID                     0x01   // CanOpen subindex number for Cob-ID
#define CO_SUBINDEX_TRANS_TYPE                 0x02   // CanOpen subindex number for transmission type
#define CO_SUBINDEX_INHIBIT_TIME               0x03   // CanOpen subindex number for inhibit time
#define CO_SUBINDEX_RESERVED                   0x04   // CanOpen subindex number for reserved
#define CO_SUBINDEX_EVENT_TIMER                0x05   // CanOpen subindex number for event timer
#define CO_SUBINDEX_SYNC_START_VALUE           0x06   // CanOpen subindex number for sync start value
                                   
#define CO_NUMBER_DEFAULT_PDO                  4      //amount of default pdos pdo 1-4
#define CO_RPDO_ALLOWED_RETRYS                 2      //is used for timeout check of RPdo data if client is activated. 


#define CO_ACCESSTYPE_STR_CONST                "const"      // CanOpen accesstype
#define CO_ACCESSTYPE_STR_RO                   "ro"         // CanOpen accesstype
#define CO_ACCESSTYPE_STR_WO                   "wo"         // CanOpen accesstype
#define CO_ACCESSTYPE_STR_RW                   "rw"         // CanOpen accesstype
#define CO_ACCESSTYPE_STR_RWR                  "rwr"        // CanOpen accesstype
#define CO_ACCESSTYPE_STR_RWW                  "rww"        // CanOpen accesstype
#define CO_ACCESSTYPE_STR_READ                 "read"       // CanOpen accesstype
#define CO_ACCESSTYPE_STR_WRITE                "write"      // CanOpen accesstype
#define CO_ACCESSTYPE_STR_READWRITE            "readwrite"  // CanOpen accesstype
#define CO_ACCESSTYPE_STR_READWRITEREAD        "readwriteoutput"  // CanOpen accesstype
#define CO_ACCESSTYPE_STR_READWRITEWRITE       "readwriteinput"   // CanOpen accesstype

#define CO_ACCESSTYPE_DEC_NOTUSED              0        // CanOpen accesstype
#define CO_ACCESSTYPE_DEC_UNKNOWN              1        // CanOpen accesstype
#define CO_ACCESSTYPE_DEC_CONST                2        // CanOpen accesstype
#define CO_ACCESSTYPE_DEC_RO                   3        // CanOpen accesstype
#define CO_ACCESSTYPE_DEC_WO                   4        // CanOpen accesstype
#define CO_ACCESSTYPE_DEC_RW                   5        // CanOpen accesstype
#define CO_ACCESSTYPE_DEC_RWR                  6        // CanOpen accesstype
#define CO_ACCESSTYPE_DEC_RWW                  7        // CanOpen accesstype


//segment Transfer
//*****************************************************
#define CO_SEGMENT_INACTIVE                  0
#define CO_DOWNLOAD_SEGMENT_ACTIVE           1
#define CO_UPLOAD_SEGMENT_ACTIVE             2
#define CO_CYCLIC_POLLING_TIME               100             //Time we wait after we set an request to the reader to poll data
#define CO_CO_CS_TOGGLEBIT_START_VALUE       0
#define CO_WAIT4RESP_UPLOAD_TIMEOUT          3000
#define CO_WAIT4RESP_DOWNLOAD_TIMEOUT        3000

//Command Specifier - Response
#define CO_CO_CS_UPLOAD_SEGMENT_RESPONSE     0
#define CO_CO_CS_DOWNLOAD_SEGMENT_RESPONSE   1
#define CO_CO_CS_INITIATE_UPLOAD_RESPONSE    2
#define CO_CO_CS_INITIATE_DOWNLOAD_RESPONSE  3
#define CO_CO_COMMAND_SPECIFIER_MASK         16#E0 //1110 0000

//Command Specifier - Request
#define CO_CO_CS_DOWNLOAD_SEGMENT_REQUEST    0
#define CO_CO_CS_INITIATE_DOWNLOAD_REQUEST   1
#define CO_CO_CS_INITIATE_UPLOAD_REQUEST     2
#define CO_CO_CS_UPLOAD_SEGMENT_REQUEST      3
#define CO_CO_CS_ABORT_TRANSFER_REQUEST      4
#define CO_SEGMENT_DATA_LENGTH               256

#endif //_CANOPENMNG_H