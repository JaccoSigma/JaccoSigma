//This file was generated by the LASAL2 CodeGenerator  -- 
//Please, do not edit this file (it might be overwritten by the next generator run)
//{{LSL_DECLARATION

(*!
<Class
	Name               = "My_ReadClass"
	Revision           = "0.0"
	GUID               = "{C799FFBC-920B-4FC7-9E82-9AE3630CC383}"
	RealtimeTask       = "false"
	CyclicTask         = "true"
	DefCyclictime      = "1 ms"
	BackgroundTask     = "false"
	Sigmatek           = "false"
	OSInterface        = "false"
	HighPriority       = "false"
	Automatic          = "false"
	UpdateMode         = "Prescan"
	SharedCommandTable = "true"
	Objectsize         = "(490,120)">
	<Channels>
		<Server Name="ReadCyclic" GUID="{FD1FBAA2-7D33-4004-909D-042D1E908FC0}" Visualized="false" Initialize="false" WriteProtected="false" Retentive="false"/>
		<Server Name="ReadWriteOnce" GUID="{85A16F92-964B-467A-94FF-4D1EF5B439D2}" Visualized="false" Initialize="false" WriteProtected="false" Retentive="false"/>
		<Server Name="WriteServer" GUID="{DDAD6A30-F38B-4555-A8B6-C3DF43E46B7C}" Visualized="false" Initialize="false" WriteProtected="false" Retentive="false"/>
		<Client Name="ReadClient" Required="false" Internal="false"/>
	</Channels>
</Class>
*)
My_ReadClass : CLASS
  //Servers:
	ReadWriteOnce 	: SvrChCmd_DINT;
	ReadCyclic 	: SvrCh_DINT;
	WriteServer 	: SvrCh_DINT;
  //Clients:
	ReadClient 	: CltCh_DINT;
  //Variables:
  //Functions:
	
	FUNCTION VIRTUAL GLOBAL CyWork
		VAR_INPUT
			EAX 	: UDINT;
		END_VAR
		VAR_OUTPUT
			state (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL ReadWriteOnce::Write
		VAR_INPUT
			input (EAX) 	: DINT;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: DINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL WriteServer::Write
		VAR_INPUT
			input (EAX) 	: DINT;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: DINT;
		END_VAR;
  //Tables:
	FUNCTION @STD
		VAR_OUTPUT
			ret_code	: CONFSTATES;
		END_VAR;
	FUNCTION GLOBAL TAB @CT_;
END_CLASS;

//}}LSL_DECLARATION


FUNCTION GLOBAL TAB My_ReadClass::@CT_
0$UINT,
2#0100000000000010$UINT, //TY_MY_READCLASS
0$UINT, 0$UINT, (SIZEOF(::My_ReadClass))$UINT, 
3$UINT, 1$UINT, 0$UINT, 
TO_UDINT(161804777), "My_ReadClass", //Class
TO_UDINT(0), 0, 0$UINT, 0$UINT, //Baseclass
//Servers:
(::My_ReadClass.ReadWriteOnce.pMeth)$UINT, _CH_CMD$UINT, 2#0000000000000000$UINT, TO_UDINT(4257960355), "ReadWriteOnce", 
(::My_ReadClass.ReadCyclic.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(496851259), "ReadCyclic", 
(::My_ReadClass.WriteServer.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(702189139), "WriteServer", 
//Clients:
(::My_ReadClass.ReadClient.pCh)$UINT, _CH_CLT_DATA$UINT, 2#0000000000000000$UINT, TO_UDINT(425953570), "ReadClient", 
END_FUNCTION


#define USER_CNT_My_ReadClass 0

TYPE
	_LSL_STD_VMETH	: STRUCT
			CmdTable	: CMDMETH;
			UserFcts	: ARRAY[0..USER_CNT_My_ReadClass] OF ^Void;
	END_STRUCT;
END_TYPE

FUNCTION My_ReadClass::@STD
	VAR_OUTPUT
		ret_code	: CONFSTATES;
	END_VAR
	VAR
		vmt	: _LSL_STD_VMETH;
	END_VAR

	//Command Methods
	InitCmdTable (nCmd := nSTDCMD + USER_CNT_My_ReadClass, pCmd := #vmt.CmdTable);
	vmt.CmdTable.Write		:= #Write();
	vmt.CmdTable.CyWork		:= #CyWork();
	ReadWriteOnce.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF ReadWriteOnce.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	ReadCyclic.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF ReadCyclic.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	WriteServer.pMeth			:= StoreMethod( #M_RD_DIRECT(), #WriteServer::Write() );
	IF WriteServer.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;

END_FUNCTION

//{{LSL_IMPLEMENTATION


FUNCTION VIRTUAL GLOBAL My_ReadClass::ReadWriteOnce::Write
	VAR_INPUT
		input (EAX) 	: DINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: DINT;
	END_VAR

	ReadWriteOnce := input;
  
//  ReadClient := ReadClient.Read();
  WriteServer.Write(input:=WriteServer);
  
	result := ReadWriteOnce := 0;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL My_ReadClass::CyWork
	VAR_INPUT
		EAX 	: UDINT;
	END_VAR
	VAR_OUTPUT
		state (EAX) 	: UDINT;
	END_VAR
  
  if ReadCyclic = 1 then
    ReadClient := ReadClient.Read();
  end_if;
  
  if ReadCyclic = 2 then
    WriteServer.Write(input:=WriteServer);
  end_if;


	state := READY;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL My_ReadClass::WriteServer::Write
	VAR_INPUT
		input (EAX) 	: DINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: DINT;
	END_VAR

	WriteServer := input;
  
  
  
	result := WriteServer;

END_FUNCTION