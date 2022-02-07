// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 14.04.2019                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#ifndef _CDsClient_H
 #define _CDsClient_H
 
 #include "..\sigclib\DefineCompiler.h"
 #include <.\Loader\Class\DataService\DS_ComDef.h>


 #define DSC_CPUSTATE_OFFLINE           -1
 #define DSC_CPUSTATE_RUN_RAM            0
 #define DSC_CPUSTATE_RUN_ROM            1
 #define DSC_CPUSTATE_STOP_BRKPT        15
 #define DSC_CPUSTATE_SINGLESTEP        18

 #ifdef cCompile

  #define DSC_MESSAGEID_DEFAULT          0
   
  typedef void ( *tDscFctDataRx)       (void*, void*, DsComData* );
  typedef void ( *tDscFctDataTx)       (void*, void*, DsComData* );
  typedef void ( *tDscFctPlcState)     (void*, void*, long);
  typedef void ( *tDscFctTaskBeginEnd) (void*, void*, unsigned long);
   
  // ----------------------------------------------------------------------------------------------
  // CDsClient.cpp
  // ----------------------------------------------------------------------------------------------
  cExtern void         *DataLink_Open(const char *ipv4, unsigned long taskpriority, unsigned long timeout_to_host, unsigned long clientid, tDscFctDataRx fct_data_rx, tDscFctDataTx fct_data_tx, tDscFctPlcState fct_plc_state, void *pcookie, const char *usertext);
  cExtern void         *DataLink_OpenEx(const char *ipv4, unsigned long taskpriority, unsigned long timeout_to_host, unsigned long clientid, tDscFctDataRx fct_data_rx, tDscFctDataTx fct_data_tx, tDscFctPlcState fct_plc_state, tDscFctTaskBeginEnd fct_task_end, void *pcookie, const char *usertext);
  cExtern void         *DataLink_OpenSyn(const char *ipv4, unsigned long taskpriority, unsigned long timeout_to_host, unsigned long clientid, tDscFctDataRx fct_data_rx, tDscFctDataTx fct_data_tx, tDscFctPlcState fct_plc_state, void *pcookie, const char *usertext);
  cExtern void         *DataLink_OpenSynEx(const char *ipv4, unsigned long taskpriority, unsigned long timeout_to_host, unsigned long clientid, tDscFctDataRx fct_data_rx, tDscFctDataTx fct_data_tx, tDscFctPlcState fct_plc_state, tDscFctTaskBeginEnd fct_task_end, void *pcookie, const char *usertext);
  cExtern unsigned long DataLink_ReOpen(void *pds0);
  cExtern void         *DataLink_Close(void *pds0);
  cExtern unsigned long DataLink_Send(void *pds0, DsComData *pdata);
  cExtern unsigned long DataLink_SendSyn(DsComData *pcd_dst, void *pds0, DsComData *pcd_src, unsigned long timeout);
  cExtern unsigned long DataLink_SendCmdLoop(void *pds0, void *loopdata, unsigned long loopdatasize);
  cExtern unsigned long DataLink_SendCmdLoopSyn(DsComData *pcd_dst, void *pds0, void *loopdata, unsigned long loopdatasize, unsigned long timeout);
  cExtern unsigned long DataLink_IsOnline(void *pds0);
  cExtern unsigned long DataLink_GetClientId(void *pds0);
  cExtern long          DataLink_GetPlcState(void *pds0);
  cExtern char         *DataLink_GetUserText(void *pds0);
  cExtern unsigned long DataLink_GetDscCrc(void *pds0);
  cExtern unsigned long DataLink_NewInstCall(void *pds0, unsigned long userid, unsigned long lasalid, unsigned short cmd, void *pdata, unsigned long datalen);
  cExtern unsigned long DataLink_NewInstCallSyn(DsComData *pcd_dst, void *pds0, unsigned long userid, unsigned long lasalid, unsigned short cmd, void *pdata, unsigned long datalen, unsigned long timeout);
  cExtern unsigned long DataLink_GetLasalId(void *pds0, const char *label, unsigned long varid);
  cExtern unsigned long DataLink_GetLasalIdSyn(void *pds0, const char *label, unsigned long timeout);


  // ----------------------------------------------------------------------------------------------
  // CDsUtils.cpp  
  // ----------------------------------------------------------------------------------------------
  cExtern void          DsComData_Init(DsComData *pcd, unsigned short id_major, unsigned short id_minor);
  cExtern void          DsComData_Empty(DsComData *pcd);
  cExtern DsComData    *DsComData_Create(void *statbuff, unsigned long statbuff_size, unsigned long userdata_size, unsigned short id_minor);
  cExtern DsComData    *DsComData_CreateEx(void *statbuff, unsigned long statbuff_size, unsigned long userdata_size, unsigned short id_major, unsigned short id_minor);
  cExtern DsComData    *DsComData_Destroy(void *statbuff, DsComData *pcd);
  cExtern unsigned long DsComData_UserDataSize(unsigned long statbuff_size);
  cExtern unsigned long DsComData_AddData(DsComData *pcd, unsigned long userdata_size, void *pdata, unsigned long datasize);
  cExtern unsigned long DsComData_AddU32(DsComData *pcd, unsigned long userdata_size, unsigned long value);
  cExtern unsigned long DsComData_AddU16(DsComData *pcd, unsigned long userdata_size, unsigned short value);
  cExtern unsigned long DsComData_AddU08(DsComData *pcd, unsigned long userdata_size, unsigned char value);
  cExtern unsigned long DsComData_AddStr(DsComData *pcd, unsigned long userdata_size, const char *pstr);
  cExtern unsigned long DsComData_Rest(DsComData *pcd, unsigned long userdata_size);
  cExtern unsigned long DsComData_SetU32(DsComData *pcd, unsigned long userdata_size, unsigned long offset, unsigned long value);
  cExtern unsigned long DsComData_IncU32(DsComData *pcd, unsigned long userdata_size, unsigned long offset);
  cExtern unsigned long DsComData_GetU32(DsComData *pcd, unsigned long offset);


 #else // cCompile
 
  // ----------------------------------------------------------------------------------------------
  // CDsClient.cpp
  // ----------------------------------------------------------------------------------------------
  function global __cdecl DataLink_Open var_input ipv4:^char; taskpriority:udint; timeout_to_host:udint; clientid:udint; fct_data_rx:^void; fct_data_tx:^void; fct_plc_state:^void; pcookie:^void; usertext:^char; end_var var_output retcode:^void; end_var;
  function global __cdecl DataLink_OpenEx var_input ipv4:^char; taskpriority:udint; timeout_to_host:udint; clientid:udint; fct_data_rx:^void; fct_data_tx:^void; fct_plc_state:^void; fct_task_end:^void; pcookie:^void; usertext:^char; end_var var_output retcode:^void; end_var;
  function global __cdecl DataLink_OpenSyn var_input ipv4:^char; taskpriority:udint; timeout_to_host:udint; clientid:udint; fct_data_rx:^void; fct_data_tx:^void; fct_plc_state:^void; pcookie:^void; usertext:^char; end_var var_output retcode:^void; end_var;
  function global __cdecl DataLink_OpenSynEx var_input ipv4:^char; taskpriority:udint; timeout_to_host:udint; clientid:udint; fct_data_rx:^void; fct_data_tx:^void; fct_plc_state:^void; fct_task_end:^void; pcookie:^void; usertext:^char; end_var var_output retcode:^void; end_var;
  function global __cdecl DataLink_ReOpen var_input pds0:^void; end_var var_output retcode:udint; end_var;
  function global __cdecl DataLink_Close var_input pds0:^void; end_var var_output retcode:^void; end_var;
  function global __cdecl DataLink_Send var_input pds0:^void; pdata:^DsComData; end_var var_output retcode:udint; end_var;
  function global __cdecl DataLink_SendSyn var_input pcd_dst:^DsComData; pds0:^void; pcd_src:^DsComData; timeout:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DataLink_SendCmdLoop var_input pds0:^void; loopdata:^void; loopdatasize:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DataLink_SendCmdLoopSyn var_input pcd_dst:^DsComData; pds0:^void; loopdata:^void; loopdatasize:udint; timeout:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DataLink_IsOnline var_input pds0:^void; end_var var_output retcode:udint; end_var;
  function global __cdecl DataLink_GetClientId var_input pds0:^void; end_var var_output retcode:udint; end_var;
  function global __cdecl DataLink_GetPlcState var_input pds0:^void; end_var var_output retcode:dint; end_var;
  function global __cdecl DataLink_GetUserText var_input pds0:^void; end_var var_output retcode:^char; end_var;
  function global __cdecl DataLink_GetDscCrc var_input pds0:^void; end_var var_output retcode:udint; end_var;
  function global __cdecl DataLink_NewInstCall var_input pds0:^void; userid:udint; lasalid:udint; cmd:uint; pdata:^void; datalen:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DataLink_NewInstCallSyn var_input pcd_dst:^DsComData; pds0:^void; userid:udint; lasalid:udint; cmd:uint; pdata:^void; datalen:udint; timeout:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DataLink_GetLasalId var_input pds0:^void; label:^char; varid:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DataLink_GetLasalIdSyn var_input pds0:^void; label:^char; timeout:udint; end_var var_output retcode:udint; end_var;

  // ----------------------------------------------------------------------------------------------
  // CDsUtils.cpp  
  // ----------------------------------------------------------------------------------------------
  function global __cdecl DsComData_Init var_input pcd:^DsComData; id_major:uint; id_minor:uint; end_var;
  function global __cdecl DsComData_Empty var_input pcd:^DsComData; end_var;
  function global __cdecl DsComData_Create var_input statbuff:^void; statbuff_size:udint; userdata_size:udint; id_minor:uint; end_var var_output retcode:^DsComData; end_var;
  function global __cdecl DsComData_CreateEx var_input statbuff:^void; statbuff_size:udint; userdata_size:udint; id_major:uint; id_minor:uint; end_var var_output retcode:^DsComData; end_var;
  function global __cdecl DsComData_Destroy var_input statbuff:^void; pcd:^DsComData; end_var var_output retcode:^DsComData; end_var;
  function global __cdecl DsComData_UserDataSize var_input statbuff_size:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DsComData_AddData var_input pcd:^DsComData; userdata_size:udint; pdata:^void; datasize:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DsComData_AddU32 var_input pcd:^DsComData; userdata_size:udint; value:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DsComData_AddU16 var_input pcd:^DsComData; userdata_size:udint; value:uint; end_var var_output retcode:udint; end_var;
  function global __cdecl DsComData_AddU08 var_input pcd:^DsComData; userdata_size:udint; value:usint; end_var var_output retcode:udint; end_var;
  function global __cdecl DsComData_AddStr var_input pcd:^DsComData; userdata_size:udint; pstr:^char; end_var var_output retcode:udint; end_var;
  function global __cdecl DsComData_Rest var_input pcd:^DsComData; userdata_size:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DsComData_SetU32 var_input pcd:^DsComData; userdata_size:udint; offset:udint; value:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DsComData_IncU32 var_input pcd:^DsComData; userdata_size:udint; offset:udint; end_var var_output retcode:udint; end_var;
  function global __cdecl DsComData_GetU32 var_input pcd:^DsComData; offset:udint; end_var var_output retcode:udint; end_var;

 #endif // cCompile
 
#endif


