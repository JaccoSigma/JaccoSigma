//This file was generated by the LASAL2 CodeGenerator  -- 
//Please, do not edit this file (it might be overwritten by the next generator run)
//{{LSL_DECLARATION
#include "..\..\Class\FDD3000_Parameters\FDD3000_NewInstIF.h"
#include "..\..\Class\FDD3000AxisVis\FDD3000AxisVis.h"

(*!
<Class
	Name               = "FDD3000AxisVisLogHandle"
	Revision           = "1.0"
	GUID               = "{DCFCD05B-C500-41A5-9992-10B19FB5AB7C}"
	RealtimeTask       = "false"
	CyclicTask         = "false"
	BackgroundTask     = "false"
	Sigmatek           = "true"
	OSInterface        = "false"
	HighPriority       = "false"
	Automatic          = "false"
	UpdateMode         = "Prescan"
	SharedCommandTable = "true"
	Objectsize         = "(448,120)">
	<Channels>
		<Server Name="ClassSvr" GUID="{A9F6EC47-AB84-4619-A1D6-5710EC817343}" Visualized="false" Initialize="false" WriteProtected="true" Retentive="false"/>
		<Server Name="InitDone" GUID="{DB62A13F-5CED-4313-A7E8-13154B6410A9}" Visualized="true" Initialize="false" WriteProtected="true" Retentive="false"/>
		<Server Name="LogIndex" GUID="{6C5C5584-3D02-463A-92DC-EC85B1E60DC5}" Visualized="false" Initialize="false" WriteProtected="false" Retentive="false"/>
		<Server Name="LogInfoMerker" GUID="{2088FF90-714D-411E-94AA-833E3810A735}" Visualized="true" Initialize="false" WriteProtected="false" Retentive="false"/>
		<Server Name="LogMode" GUID="{FAE3890D-5581-4546-B9D1-3D5B7FEEA249}" Visualized="true" Initialize="false" WriteProtected="false" Retentive="false"/>
		<Server Name="LogOffset" GUID="{EB4A0122-7467-484A-AE35-878FDE4709E9}" Visualized="true" Initialize="false" WriteProtected="false" Retentive="false"/>
		<Server Name="LogOffsetMax" GUID="{9AE4705A-84D4-44FE-9F5D-1448047525F4}" Visualized="true" Initialize="false" WriteProtected="true" Retentive="false"/>
		<Server Name="StateMerker" GUID="{193484B0-B256-4E34-9899-5CF78619218A}" Visualized="true" Initialize="false" WriteProtected="false" Retentive="false"/>
		<Client Name="coFDD3000" Required="true" Internal="false"/>
	</Channels>
	<Dependencies>
		<Files>
			<File Path=".\Class\FDD3000_Parameters\FDD3000_NewInstIF.h" Include="true"/>
			<File Path=".\Class\FDD3000AxisVis\FDD3000AxisVis.h" Include="true"/>
		</Files>
	</Dependencies>
	<RevDoku>
		<Owner Company="Sigmatek" Author="PieSte"/>
		<Dokumentation Revision="1.0" Date="02.02.2021" Author="ZoePat" Company="Sigmatek" Description="first library version"/>
	</RevDoku>
</Class>
*)
FDD3000AxisVisLogHandle : CLASS
  //Servers:
	ClassSvr 	: SvrChCmd_DINT;
	LogMode 	: SvrCh_DINT;
	LogOffset 	: SvrCh_DINT;
	LogIndex 	: SvrCh_DINT;
	LogOffsetMax 	: SvrCh_DINT;
	LogInfoMerker 	: SvrCh_DINT;
	StateMerker 	: SvrCh_DINT;
	InitDone 	: SvrCh_DINT;
  //Clients:
	coFDD3000 	: CltChCmd_DINT;
  //Variables:
		ptrLogFile 	: ^FDD3000AxisVis::_LogFile;
  //Functions:
	
	FUNCTION VIRTUAL GLOBAL Init;
	
	FUNCTION VIRTUAL GLOBAL LogMode::Read
		VAR_OUTPUT
			output (EAX) 	: DINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL LogMode::Write
		VAR_INPUT
			input (EAX) 	: DINT;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: DINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL LogOffset::Write
		VAR_INPUT
			input (EAX) 	: DINT;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: DINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL LogIndex::Read
		VAR_OUTPUT
			output (EAX) 	: DINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL LogOffsetMax::Read
		VAR_OUTPUT
			output (EAX) 	: DINT;
		END_VAR;
  //Tables:
	FUNCTION @STD
		VAR_OUTPUT
			ret_code	: CONFSTATES;
		END_VAR;
	FUNCTION GLOBAL TAB @CT_;
END_CLASS;

//}}LSL_DECLARATION


FUNCTION GLOBAL TAB FDD3000AxisVisLogHandle::@CT_
0$UINT,
2#0100000000000000$UINT, //TY_FDD3000AXISVISLOGHANDLE
1$UINT, 0$UINT, (SIZEOF(::FDD3000AxisVisLogHandle))$UINT, 
8$UINT, 1$UINT, 0$UINT, 
TO_UDINT(4023506578), "FDD3000AxisVisLogHandle", //Class
TO_UDINT(0), 0, 0$UINT, 0$UINT, //Baseclass
//Servers:
(::FDD3000AxisVisLogHandle.ClassSvr.pMeth)$UINT, _CH_CMD$UINT, 2#0000000000000000$UINT, TO_UDINT(619352855), "ClassSvr", 
(::FDD3000AxisVisLogHandle.LogMode.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(2315356560), "LogMode", 
(::FDD3000AxisVisLogHandle.LogOffset.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(3079590910), "LogOffset", 
(::FDD3000AxisVisLogHandle.LogIndex.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(168518947), "LogIndex", 
(::FDD3000AxisVisLogHandle.LogOffsetMax.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(345900693), "LogOffsetMax", 
(::FDD3000AxisVisLogHandle.LogInfoMerker.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(2331807777), "LogInfoMerker", 
(::FDD3000AxisVisLogHandle.StateMerker.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(48725062), "StateMerker", 
(::FDD3000AxisVisLogHandle.InitDone.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(2784815143), "InitDone", 
//Clients:
(::FDD3000AxisVisLogHandle.coFDD3000.pCh)$UINT, _CH_CLT_CMD$UINT, 2#0000000000000010$UINT, TO_UDINT(3500194077), "coFDD3000", 
END_FUNCTION


#define USER_CNT_FDD3000AxisVisLogHandle 0

TYPE
	_LSL_STD_VMETH	: STRUCT
			CmdTable	: CMDMETH;
			UserFcts	: ARRAY[0..USER_CNT_FDD3000AxisVisLogHandle] OF ^Void;
	END_STRUCT;
END_TYPE

FUNCTION FDD3000AxisVisLogHandle::@STD
	VAR_OUTPUT
		ret_code	: CONFSTATES;
	END_VAR
	VAR
		vmt	: _LSL_STD_VMETH;
	END_VAR

	//Command Methods
	InitCmdTable (nCmd := nSTDCMD + USER_CNT_FDD3000AxisVisLogHandle, pCmd := #vmt.CmdTable);
	vmt.CmdTable.Init		:= #Init();
	ClassSvr.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF ClassSvr.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	LogMode.pMeth			:= StoreMethod( #LogMode::Read(), #LogMode::Write() );
	IF LogMode.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	LogOffset.pMeth			:= StoreMethod( #M_RD_DIRECT(), #LogOffset::Write() );
	IF LogOffset.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	LogIndex.pMeth			:= StoreMethod( #LogIndex::Read(), #M_WR_DIRECT() );
	IF LogIndex.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	LogOffsetMax.pMeth			:= StoreMethod( #LogOffsetMax::Read(), #M_NO_F() );
	IF LogOffsetMax.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	LogInfoMerker.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF LogInfoMerker.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	StateMerker.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF StateMerker.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;

END_FUNCTION

//{{LSL_IMPLEMENTATION
FUNCTION VIRTUAL GLOBAL FDD3000AxisVisLogHandle::Init
  VAR  	
    MyPara 	 : CmdStruct;
    MyResult : Results;
    dRetcode : iprStates;
  END_VAR

  if _FirstScan then
    MyPara.uiCmd := _FDD3000_IF_LOGDATA_GETFILEPOINTER;
    MyPara.aPara[0] := _FDD3000_IF_PROTOCOLVERSION;
    dRetCode := coFDD3000.NewInst(pPara:=#MyPara, pResult:=#MyResult);
    if dRetCode = READY & MyResult.uiLng = (sizeof(HINT)+sizeof(DINT)) then
      ptrLogFile := MyResult.aData[2]$^FDD3000AxisVis::_LogFile;        
      LogInfoMerker := 0;
      StateMerker := 0;
      InitDone := 1;
    end_if;  
  end_if;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL FDD3000AxisVisLogHandle::LogMode::Read
	VAR_OUTPUT
		output (EAX) 	: DINT;
	END_VAR

  LogMode := ptrLogFile^.Logmode;
	output := LogMode;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL FDD3000AxisVisLogHandle::LogMode::Write
	VAR_INPUT
		input (EAX) 	: DINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: DINT;
	END_VAR
  VAR  	
    MyPara 	 : CmdStruct;
    MyResult : Results;
    dRetcode : iprStates;
  END_VAR

	LogMode := input;
    MyPara.uiCmd := _FDD3000_IF_LOGDATA_CLEARLOGBUFFER;
    MyPara.aPara[0] := _FDD3000_IF_PROTOCOLVERSION;
    dRetCode := coFDD3000.NewInst(pPara:=#MyPara, pResult:=#MyResult);
    if dRetCode = READY & MyResult.uiLng = (sizeof(HINT)) then
      LogMode   := 0;
      LogOffset := 0;
    end_if;  
 	result := LogMode;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL FDD3000AxisVisLogHandle::LogOffset::Write
	VAR_INPUT
		input (EAX) 	: DINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: DINT;
	END_VAR

	LogOffset := input;
  if LogOffset > _FDD3000AXISVIS_MAXLOG_OFFSET then  // offset to more then maxno(as index) - shown entries
    LogOffset := _FDD3000AXISVIS_MAXLOG_OFFSET;      // limit to maxno(as index) - shown entries
  elsif LogOffset < 0 then
    LogOffset := 0;
  end_if;
 	result := LogOffset;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL FDD3000AxisVisLogHandle::LogIndex::Read
	VAR_OUTPUT
		output (EAX) 	: DINT;
	END_VAR

  if ptrLogFile^.AccesNr - 1 - LogOffset < 0 then
    LogIndex := _FDD3000AXISVIS_MAXLOGBUFFER_ENTRIES + ptrLogFile^.AccesNr - 1 - LogOffset; // 101 because there are 101 entries (0-100)
  else
    LogIndex := ptrLogFile^.AccesNr - 1 - LogOffset;
  end_if;
  
	output := LogIndex;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL FDD3000AxisVisLogHandle::LogOffsetMax::Read
	VAR_OUTPUT
		output (EAX) 	: DINT;
	END_VAR
VAR
	pData   : ^FDD3000AxisVis::_LogStruct;
END_VAR

  pData := #ptrLogFile^.LogData[ptrLogFile^.AccesNr];
  // check for overflow of buffer
  if (pData^.TimeStamp <> 0) | (pData^.RTStamp <> 0) then
    // there was already a overflow
    LogOffsetMax := _FDD3000AXISVIS_MAXLOG_OFFSET;  // maxno(0 to 100) - shown entries
  else
    // no overflow
    if ptrLogFile^.AccesNr > _FDD3000AXISVIS_LOGBUFFER_VISIBLEENTRIES then
      LogOffsetMax := ptrLogFile^.AccesNr - _FDD3000AXISVIS_LOGBUFFER_VISIBLEENTRIES;
    else
      LogOffsetMax := 0;
    end_if;
  end_if;

	output := LogOffsetMax;

END_FUNCTION