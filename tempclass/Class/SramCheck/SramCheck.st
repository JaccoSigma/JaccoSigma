//This file was generated by the LASAL2 CodeGenerator  -- 
//Please, do not edit this file (it might be overwritten by the next generator run)
//{{LSL_DECLARATION

(*!
<Class
	Name               = "SramCheck"
	Revision           = "0.0"
	GUID               = "{A9137B28-630B-4394-8096-20314BE25A17}"
	RealtimeTask       = "false"
	CyclicTask         = "false"
	BackgroundTask     = "true"
	Sigmatek           = "false"
	OSInterface        = "false"
	HighPriority       = "false"
	Automatic          = "false"
	UpdateMode         = "Prescan"
	SharedCommandTable = "true"
	Objectsize         = "(392,120)">
	<Channels>
		<Server Name="ClassSvr" GUID="{A9ECA2C8-A643-4FC9-A169-7ED2872B9FB6}" Visualized="true" Remotely="true" Initialize="false" WriteProtected="false" Retentive="SRam"/>
		<Server Name="Data" GUID="{5D33047C-BA0B-405E-B760-9CB61025D6C2}" Class="StringRAM" Visualized="true" Remotely="true" Initialize="true" WriteProtected="false" Retentive="false"/>
		<Server Name="Data0" GUID="{F7604D15-1585-451C-AC13-E0D0215FB7DD}" Class="StringRAM" Visualized="true" Remotely="true" Initialize="true" WriteProtected="false" Retentive="false"/>
		<Server Name="Data1" GUID="{83B151A2-CAE1-407F-A5F7-CE6CEE6C09C2}" Class="StringRAM" Visualized="true" Remotely="true" Initialize="true" WriteProtected="false" Retentive="false"/>
		<Server Name="Data2" GUID="{E9BC692B-4B17-4D8D-936C-B7ED1F9EC77B}" Class="StringRAM" Visualized="true" Remotely="true" Initialize="true" WriteProtected="false" Retentive="false"/>
		<Server Name="Data3" GUID="{FEA3700F-0E50-4E5C-89F5-D529D80E2EED}" Class="Ram" Visualized="true" Remotely="true" Initialize="true" WriteProtected="false" Retentive="false"/>
		<Server Name="Data4" GUID="{6CF2B163-C6A3-41BD-8EA6-FD18E6A3773A}" Class="Ram" Visualized="true" Remotely="true" Initialize="true" WriteProtected="false" Retentive="false"/>
		<Server Name="m_udLength" GUID="{35FA8D07-7788-4325-9CF9-1579936F996D}" Class="RAMex" Visualized="true" Remotely="true" Initialize="true" WriteProtected="false" Retentive="false" Comment="length of the data buffer"/>
		<Server Name="RAMexDo" GUID="{A98AE25A-055A-427B-BF0B-1B8785DBFB1B}" Visualized="true" Remotely="true" Initialize="false" WriteProtected="false" Retentive="false"/>
		<Server Name="Server" GUID="{4E4CFF52-0194-4CF1-BF88-71957111E115}" Visualized="true" Remotely="true" Initialize="false" WriteProtected="false" Retentive="SRam"/>
		<Server Name="Server0" GUID="{629B58D4-5115-40EC-8B1A-F1F06366AFB1}" Visualized="true" Remotely="true" Initialize="false" WriteProtected="false" Retentive="SRam"/>
		<Server Name="Server1" GUID="{272A5F60-2A4C-4623-BFAC-E48BA1576435}" Visualized="true" Remotely="true" Initialize="false" WriteProtected="false" Retentive="File"/>
		<Server Name="Server2" GUID="{7F617192-736A-45EE-8585-581DCE6B71ED}" Visualized="true" Remotely="true" Initialize="false" WriteProtected="false" Retentive="File"/>
		<Server Name="Server3" GUID="{B8EC1146-EB9D-4BEF-99BC-2C592E763C3D}" Visualized="true" Remotely="true" Initialize="false" WriteProtected="false" Retentive="File"/>
		<Client Name="Ram1" Required="true" Internal="true"/>
		<Client Name="Ram2" Required="true" Internal="true"/>
		<Client Name="RAMex1" Required="true" Internal="true"/>
	</Channels>
	<Network Name="SramCheck">
		<!-- List of Components in this network -->
		<Components>
			<Object
				Name       = "Ram1"
				GUID       = "{185970A6-2B71-4BB9-B9AD-902173727AF7}"
				Class      = "Ram"
				Position   = "(60,1410)"
				Visualized = "true"
				Remotely   = "true">
				<Channels>
					<Server Name="Data"/>
				</Channels>
			</Object>
			<Object
				Name       = "Ram2"
				GUID       = "{A4D9A2A7-91C0-4331-AD62-5DCB2D068E84}"
				Class      = "Ram"
				Position   = "(60,1560)"
				Visualized = "true"
				Remotely   = "true">
				<Channels>
					<Server Name="Data"/>
				</Channels>
			</Object>
			<Object
				Name       = "RAMex1"
				GUID       = "{5E127014-D8AB-470C-B0A8-AD9FFF6CE3D7}"
				Class      = "RAMex"
				Position   = "(60,1740)"
				Visualized = "true"
				Remotely   = "true">
				<Channels>
					<Server Name="m_udLength"/>
					<Client Name="MultiTask"/>
					<Client Name="ThreadSafe" Value="0"/>
					<Client Name="UseFile"/>
				</Channels>
			</Object>
		</Components>
		<Comments>
		</Comments>
		<!-- List of Connections in this network -->
		<Connections>
			<Connection Source="this.Ram1" Destination="Ram1.Data"/>
			<Connection Source="this.Ram2" Destination="Ram2.Data"/>
			<Connection Source="this.Data3" Destination="Ram1.Data" Vertices="(532,810),(600,1500),(360,1500),"/>
			<Connection Source="this.Data4" Destination="Ram2.Data" Vertices="(532,870),(600,1650),(360,1650),"/>
			<Connection Source="this.RAMex1" Destination="RAMex1.m_udLength"/>
			<Connection Source="this.m_udLength" Destination="RAMex1.m_udLength" Vertices="(532,930),(360,1830),"/>
		</Connections>
		<!-- Headerfiles -->
		<Options>
		</Options>
	</Network>
</Class>
*)
SramCheck : CLASS
  //Servers:
	ClassSvr 	: SvrChCmd_DINT;
	Server 	: SvrCh_DINT;
	Server0 	: SvrCh_DINT;
	Server1 	: SvrCh_DINT;
	Server2 	: SvrCh_DINT;
	Server3 	: SvrCh_DINT;
	Data 	: SvrChCmd_UDINT;
	Data0 	: SvrChCmd_UDINT;
	Data1 	: SvrChCmd_UDINT;
	Data2 	: SvrChCmd_UDINT;
	Data3 	: SvrChCmd_DINT;
	Data4 	: SvrChCmd_DINT;
	m_udLength 	: SvrChCmd_UDINT;
	RAMexDo 	: SvrCh_DINT;
  //Clients:
	Ram1 	: CltChCmd_Ram;
	Ram2 	: CltChCmd_Ram;
	RAMex1 	: CltChCmd_RAMex;
  //Variables:
  //Functions:
	
	FUNCTION VIRTUAL GLOBAL Init;
	
	FUNCTION VIRTUAL GLOBAL Background
		VAR_INPUT
			EAX 	: UDINT;
		END_VAR
		VAR_OUTPUT
			state (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data::Read
		VAR_OUTPUT
			output (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data::Write
		VAR_INPUT
			input (EAX) 	: UDINT;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data0::Read
		VAR_OUTPUT
			output (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data0::Write
		VAR_INPUT
			input (EAX) 	: UDINT;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data1::Read
		VAR_OUTPUT
			output (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data1::Write
		VAR_INPUT
			input (EAX) 	: UDINT;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data2::Read
		VAR_OUTPUT
			output (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data2::Write
		VAR_INPUT
			input (EAX) 	: UDINT;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data3::Read
		VAR_OUTPUT
			output (EAX) 	: DINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data3::Write
		VAR_INPUT
			input (EAX) 	: DINT;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: DINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data4::Read
		VAR_OUTPUT
			output (EAX) 	: DINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL Data4::Write
		VAR_INPUT
			input (EAX) 	: DINT;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: DINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL m_udLength::Read
		VAR_OUTPUT
			output (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL m_udLength::Write
		VAR_INPUT
			input (EAX) 	: UDINT;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL RAMexDo::Write
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

#pragma using Ram
#pragma using RAMex


//}}LSL_DECLARATION


FUNCTION GLOBAL TAB SramCheck::@CT_
0$UINT,
2#0100000000000010$UINT, //TY_SRAMCHECK
0$UINT, 0$UINT, (SIZEOF(::SramCheck))$UINT, 
14$UINT, 3$UINT, 0$UINT, 
TO_UDINT(3868681063), "SramCheck", //Class
TO_UDINT(0), 0, 0$UINT, 0$UINT, //Baseclass
//Servers:
(::SramCheck.ClassSvr.pMeth)$UINT, _CH_CMD$UINT, 2#0000000000001000$UINT, TO_UDINT(619352855), "ClassSvr", 
(::SramCheck.Server.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000001000$UINT, TO_UDINT(2861896304), "Server", 
(::SramCheck.Server0.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000001000$UINT, TO_UDINT(2759080719), "Server0", 
(::SramCheck.Server1.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000011000$UINT, TO_UDINT(3547532185), "Server1", 
(::SramCheck.Server2.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000011000$UINT, TO_UDINT(1249532451), "Server2", 
(::SramCheck.Server3.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000011000$UINT, TO_UDINT(1031629493), "Server3", 
(::SramCheck.Data.pMeth)$UINT, _CH_SVR_OBJ$UINT, 2#0000000000000000$UINT, TO_UDINT(2607161047), "Data", 
(::SramCheck.Data0.pMeth)$UINT, _CH_SVR_OBJ$UINT, 2#0000000000000000$UINT, TO_UDINT(3975675464), "Data0", 
(::SramCheck.Data1.pMeth)$UINT, _CH_SVR_OBJ$UINT, 2#0000000000000000$UINT, TO_UDINT(2616250078), "Data1", 
(::SramCheck.Data2.pMeth)$UINT, _CH_SVR_OBJ$UINT, 2#0000000000000000$UINT, TO_UDINT(49913700), "Data2", 
(::SramCheck.Data3.pMeth)$UINT, _CH_SVR_OBJ$UINT, 2#0000000000000000$UINT, TO_UDINT(1979625458), "Data3", 
(::SramCheck.Data4.pMeth)$UINT, _CH_SVR_OBJ$UINT, 2#0000000000000000$UINT, TO_UDINT(3952753233), "Data4", 
(::SramCheck.m_udLength.pMeth)$UINT, _CH_SVR_OBJ$UINT, 2#0000000000000000$UINT, TO_UDINT(3370139294), "m_udLength", 
(::SramCheck.RAMexDo.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(4048708566), "RAMexDo", 
//Clients:
(::SramCheck.Ram1.pCh)$UINT, _CH_CLT_OBJ$UINT, 2#0000000000000010$UINT, TO_UDINT(621924016), "Ram1", TO_UDINT(1903583397), "Ram", 1$UINT, 3$UINT, 
(::SramCheck.Ram2.pCh)$UINT, _CH_CLT_OBJ$UINT, 2#0000000000000010$UINT, TO_UDINT(3155730186), "Ram2", TO_UDINT(1903583397), "Ram", 1$UINT, 3$UINT, 
(::SramCheck.RAMex1.pCh)$UINT, _CH_CLT_OBJ$UINT, 2#0000000000000010$UINT, TO_UDINT(2844025099), "RAMex1", TO_UDINT(933570811), "RAMex", 1$UINT, 17$UINT, 
END_FUNCTION


#define USER_CNT_SramCheck 1

TYPE
	_LSL_STD_VMETH	: STRUCT
			CmdTable	: CMDMETH;
			UserFcts	: ARRAY[0..USER_CNT_SramCheck] OF ^Void;
	END_STRUCT;
END_TYPE

FUNCTION SramCheck::@STD
	VAR_OUTPUT
		ret_code	: CONFSTATES;
	END_VAR
	VAR
		vmt	: _LSL_STD_VMETH;
	END_VAR

	//Command Methods
	InitCmdTable (nCmd := nSTDCMD + USER_CNT_SramCheck, pCmd := #vmt.CmdTable);
	vmt.CmdTable.Write		:= #M_WR_DIRECT();
	vmt.CmdTable.Init		:= #Init();
#pragma warning (disable : 74)
	vmt.UserFcts[0]		:= #Background();

#pragma warning (default : 74)
	ClassSvr.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF ClassSvr.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	Server.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF Server.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	Server0.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF Server0.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	Server1.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF Server1.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	Server2.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF Server2.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	Server3.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF Server3.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	_memcpy((#vmt.CmdTable)$^USINT, Data.pMeth, SIZEOF(CMDMETH));
	vmt.CmdTable.nCmds		:= nSTDCMD;
	vmt.CmdTable.Read		:= #Data::Read();
	vmt.CmdTable.Write		:= #Data::Write();
	Data.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF Data.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	_memcpy((#vmt.CmdTable)$^USINT, Data0.pMeth, SIZEOF(CMDMETH));
	vmt.CmdTable.nCmds		:= nSTDCMD;
	vmt.CmdTable.Read		:= #Data0::Read();
	vmt.CmdTable.Write		:= #Data0::Write();
	Data0.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF Data0.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	_memcpy((#vmt.CmdTable)$^USINT, Data1.pMeth, SIZEOF(CMDMETH));
	vmt.CmdTable.nCmds		:= nSTDCMD;
	vmt.CmdTable.Read		:= #Data1::Read();
	vmt.CmdTable.Write		:= #Data1::Write();
	Data1.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF Data1.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	_memcpy((#vmt.CmdTable)$^USINT, Data2.pMeth, SIZEOF(CMDMETH));
	vmt.CmdTable.nCmds		:= nSTDCMD;
	vmt.CmdTable.Read		:= #Data2::Read();
	vmt.CmdTable.Write		:= #Data2::Write();
	Data2.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF Data2.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	_memcpy((#vmt.CmdTable)$^USINT, Data3.pMeth, SIZEOF(CMDMETH));
	vmt.CmdTable.nCmds		:= nSTDCMD;
	vmt.CmdTable.Read		:= #Data3::Read();
	vmt.CmdTable.Write		:= #Data3::Write();
	Data3.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF Data3.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	_memcpy((#vmt.CmdTable)$^USINT, Data4.pMeth, SIZEOF(CMDMETH));
	vmt.CmdTable.nCmds		:= nSTDCMD;
	vmt.CmdTable.Read		:= #Data4::Read();
	vmt.CmdTable.Write		:= #Data4::Write();
	Data4.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF Data4.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	_memcpy((#vmt.CmdTable)$^USINT, m_udLength.pMeth, SIZEOF(CMDMETH));
	vmt.CmdTable.nCmds		:= nSTDCMD;
	vmt.CmdTable.Read		:= #m_udLength::Read();
	vmt.CmdTable.Write		:= #m_udLength::Write();
	m_udLength.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF m_udLength.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	RAMexDo.pMeth			:= StoreMethod( #M_RD_DIRECT(), #RAMexDo::Write() );
	IF RAMexDo.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;

END_FUNCTION

//{{LSL_IMPLEMENTATION


FUNCTION VIRTUAL GLOBAL SramCheck::Data::Read
	VAR_OUTPUT
		output (EAX) 	: UDINT;
	END_VAR

	Data := StringRAM1.Data.Read();
	output := Data;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Data::Write
	VAR_INPUT
		input (EAX) 	: UDINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: UDINT;
	END_VAR

	Data := input;
	result := (StringRAM1.Data.Write(Data))$UDINT;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Data0::Read
	VAR_OUTPUT
		output (EAX) 	: UDINT;
	END_VAR

	Data0 := StringRAM2.Data.Read();
	output := Data0;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Data0::Write
	VAR_INPUT
		input (EAX) 	: UDINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: UDINT;
	END_VAR

	Data0 := input;
	result := (StringRAM2.Data.Write(Data0))$UDINT;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Data1::Read
	VAR_OUTPUT
		output (EAX) 	: UDINT;
	END_VAR

	Data1 := StringRAM3.Data.Read();
	output := Data1;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Data1::Write
	VAR_INPUT
		input (EAX) 	: UDINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: UDINT;
	END_VAR

	Data1 := input;
	result := (StringRAM3.Data.Write(Data1))$UDINT;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Data2::Read
	VAR_OUTPUT
		output (EAX) 	: UDINT;
	END_VAR

	Data2 := StringRAM4.Data.Read();
	output := Data2;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Data2::Write
	VAR_INPUT
		input (EAX) 	: UDINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: UDINT;
	END_VAR

	Data2 := input;
	result := (StringRAM4.Data.Write(Data2))$UDINT;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Data3::Read
	VAR_OUTPUT
		output (EAX) 	: DINT;
	END_VAR

	Data3 := Ram1.Data.Read();
	output := Data3;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Data3::Write
	VAR_INPUT
		input (EAX) 	: DINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: DINT;
	END_VAR

	Data3 := input;
	result := (Ram1.Data.Write(Data3))$DINT;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Data4::Read
	VAR_OUTPUT
		output (EAX) 	: DINT;
	END_VAR

	Data4 := Ram2.Data.Read();
	output := Data4;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Data4::Write
	VAR_INPUT
		input (EAX) 	: DINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: DINT;
	END_VAR

	Data4 := input;
	result := (Ram2.Data.Write(Data4))$DINT;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::m_udLength::Read
	VAR_OUTPUT
		output (EAX) 	: UDINT;
	END_VAR

	m_udLength := RAMex1.m_udLength.Read();
	output := m_udLength;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::m_udLength::Write
	VAR_INPUT
		input (EAX) 	: UDINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: UDINT;
	END_VAR

	m_udLength := input;
	result := (RAMex1.m_udLength.Write(m_udLength))$UDINT;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Init

  if _FirstScan = TRUE then
    
    if RAMex1.m_udLength = 0 then
      RAMex1.SetSize(udSize:=sizeof(RAMexDo));
    else
      RAMex1.GetData(pData:=#RAMexDo$USINT);
    end_if;
  
  end_if;


END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::Background
	VAR_INPUT
		EAX 	: UDINT;
	END_VAR
	VAR_OUTPUT
		state (EAX) 	: UDINT;
	END_VAR

	state := READY;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL SramCheck::RAMexDo::Write
	VAR_INPUT
		input (EAX) 	: DINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: DINT;
	END_VAR

	RAMexDo := input;
  
  if input = 0 then
    RAMex1.GetData(pData:=#RAMexDo$USINT);
  else
    RAMex1.SetData(pData:=#RAMexDo$USINT);
  end_if;

  
  
	result := RAMexDo;

END_FUNCTION