//This file was generated by the LASAL2 CodeGenerator  -- 
//Please, do not edit this file (it might be overwritten by the next generator run)
//{{LSL_DECLARATION

(*!
<Class
	Name               = "_DriveMng_SR012"
	Revision           = "1.0"
	GUID               = "{649C826E-5247-4B8E-804C-34F60836BC5D}"
	RealtimeTask       = "false"
	CyclicTask         = "true"
	DefCyclictime      = "10 ms"
	BackgroundTask     = "false"
	Sigmatek           = "true"
	OSInterface        = "false"
	HighPriority       = "false"
	Automatic          = "false"
	UpdateMode         = "Prescan"
	IconPath           = ".\Class\_DriveMngBase\Drive.ico"
	SharedCommandTable = "true"
	Objectsize         = "(586,120)"
	Comment            = "S-DIAS DC-Motor Module SR012 (20-029-012) &#13;&#10;- 1 motor output 50V/5A,  &#13;&#10;- 2 Enable inputs 24V / 0.5ms, &#13;&#10;- 4 digital inputs 24V / 5ms,  &#13;&#10;- 1 digital output 24V / 0,2A">
	<Channels>
		<Server Name="FwDate" GUID="{71E03170-3354-49E5-B23E-3318B0EF8B47}" Class="VirtualBaseInit" Visualized="false" Initialize="true" WriteProtected="true" Retentive="false" Comment="shows the firmware version date"/>
		<Server Name="FWVersion" GUID="{C0E06E19-0D30-42EF-B9EE-C4564D7D195A}" Class="VirtualBaseInit" Visualized="false" Initialize="true" WriteProtected="true" Retentive="false" Comment="shows the firmware version"/>
		<Server Name="HwVersionSdias" GUID="{CD827D63-DC3C-4EAA-82B2-DF5FEBF66E4A}" Visualized="true" Initialize="false" WriteProtected="true" Retentive="false" Comment="shows hw version in the format xx.yy (16#0120 means v1.20)"/>
		<Server Name="LEDControl" GUID="{53831168-7D8C-492B-A4BF-67AD6D4B53F1}" Visualized="true" Initialize="true" WriteProtected="false" Retentive="false" Comment="Sets a LED on Hardware, to detect Module.&#13;&#10;0 ... LED off&#13;&#10;1 ... LED on&#13;&#10;2 ... blinking slow&#13;&#10;3 ... blinking fast"/>
		<Client Name="BusInterfaceSdiasDriveMng" Required="true" Internal="true"/>
		<Client Name="FWDateString" Required="true" Internal="true"/>
		<Client Name="FWNumSTring" Required="true" Internal="true"/>
		<Client Name="Place" Required="false" Internal="false" Comment="Place of SDIAS Modul [0..63]."/>
		<Client Name="Required" Required="true" Internal="false" DefValue="1" Comment="0 = module is not required&#13;&#10;1 = module is required (CPU will crash if module is missing)"/>
		<Client Name="SdiasIn" Required="true" Internal="false" Comment="This Clients must be connected to a SDIAS_Out_[x] Server."/>
		<Client Name="TimeMultiplier" Required="false" Internal="false" DefValue="1" Comment="multiplier of cycle time"/>
	</Channels>
	<RevDoku>
		<Owner Company="Sigmatek" Author="PieSte"/>
		<Dokumentation Revision="1.0" Date="22.03.2019" Author="PieSte" Company="Sigmatek" Description="First library version."/>
	</RevDoku>
	<Network Name="_DriveMng_SR012">
		<!-- List of Components in this network -->
		<Components>
			<Object
				Name       = "_base"
				GUID       = "{69D9396A-EF67-41A6-919F-850A685308D7}"
				Class      = "_DriveMngBase"
				Position   = "(1726,120)"
				Visualized = "false">
				<Channels>
					<Server Name="AxisAvailable"/>
					<Server Name="CmdState"/>
					<Server Name="DeviceAddress"/>
					<Server Name="DeviceID"/>
					<Server Name="DriveType"/>
					<Server Name="HWVersion"/>
					<Server Name="Online"/>
					<Server Name="Release"/>
					<Server Name="RetryCounter"/>
					<Server Name="SerialNo"/>
					<Server Name="VendorID"/>
					<Client Name="cOnline"/>
					<Client Name="coStdLib"/>
					<Client Name="FwDatetr"/>
					<Client Name="FwNumStr"/>
					<Client Name="To_TaskObjectControl"/>
					<Client Name="ToBusIF"/>
					<Client Name="toVaran"/>
				</Channels>
			</Object>
			<Object
				Name       = "BusInterfaceSdiasDriveMng"
				GUID       = "{56353E33-6E0D-41EA-987D-F9C10F8624DA}"
				Class      = "_BusInterfaceSdiasDriveMng"
				Position   = "(330,330)"
				Visualized = "false">
				<Channels>
					<Server Name="CallCommand"/>
					<Server Name="ClassState"/>
					<Server Name="DeviceID"/>
					<Server Name="FPGAVersion"/>
					<Server Name="HwVersion"/>
					<Server Name="LEDControl"/>
					<Server Name="Online"/>
					<Server Name="RetryCounter"/>
					<Server Name="SDOState"/>
					<Server Name="SerialNo"/>
					<Client Name="Place"/>
					<Client Name="Required" Value="1"/>
					<Client Name="SdiasID" Value="DRIVEMNG_BUSDEVICEID_SR012"/>
					<Client Name="SdiasIn"/>
					<Client Name="TimeMultiplier" Value="1"/>
				</Channels>
			</Object>
			<Object
				Name       = "FWDateString"
				GUID       = "{8FDACB0F-3435-42FA-B10C-AE5B28B77E4B}"
				Class      = "String"
				Position   = "(1110,450)"
				Visualized = "true">
				<Channels>
					<Server Name="Data"/>
					<Client Name="SingleRealloc" Value="1"/>
				</Channels>
			</Object>
			<Object
				Name       = "FWNumSTring"
				GUID       = "{13B92ADF-31E1-4408-8F9E-B0F255B430D2}"
				Class      = "String"
				Position   = "(1110,300)"
				Visualized = "true">
				<Channels>
					<Server Name="Data"/>
					<Client Name="SingleRealloc" Value="1"/>
				</Channels>
			</Object>
		</Components>
		<Comments>
		</Comments>
		<!-- List of Connections in this network -->
		<Connections>
			<Connection Source="this.CmdState" Destination="_base.CmdState" Vertices="(2388,210),(2216,210),"/>
			<Connection Source="this.BusInterfaceSdiasDriveMng" Destination="BusInterfaceSdiasDriveMng.ClassState"/>
			<Connection Source="_base.ToBusIF" Destination="BusInterfaceSdiasDriveMng.CallCommand" Vertices="(1726,570),(1500,570),(1500,960),(892,960),"/>
			<Connection Source="BusInterfaceSdiasDriveMng.SdiasIn" Destination="this.SdiasIn" Vertices="(330,420),(120,420),(120,690),(38,690),"/>
			<Connection Source="BusInterfaceSdiasDriveMng.Place" Destination="this.Place" Vertices="(330,480),(150,480),(150,750),(38,750),"/>
			<Connection Source="BusInterfaceSdiasDriveMng.Required" Destination="this.Required" Vertices="(330,540),(180,540),(180,810),(38,810),"/>
			<Connection Source="BusInterfaceSdiasDriveMng.TimeMultiplier" Destination="this.TimeMultiplier" Vertices="(330,600),(210,600),(210,870),(38,870),"/>
			<Connection Source="this.Online" Destination="_base.Online" Vertices="(2388,270),(2216,270),"/>
			<Connection Source="this.Release" Destination="_base.Release" Vertices="(2388,330),(2216,330),"/>
			<Connection Source="this.DriveType" Destination="_base.DriveType" Vertices="(2388,510),(2216,630),"/>
			<Connection Source="this.LEDControl" Destination="BusInterfaceSdiasDriveMng.LEDControl" Vertices="(2388,870),(2310,960),(1020,960),(1020,780),(892,780),"/>
			<Connection Source="this.DeviceID" Destination="_base.DeviceID" Vertices="(2388,390),(2216,510),"/>
			<Connection Source="this.RetryCounter" Destination="_base.RetryCounter" Vertices="(2388,450),(2216,570),"/>
			<Connection Source="this.SerialNo" Destination="_base.SerialNo" Vertices="(2388,570),(2216,750),"/>
			<Connection Source="_base.FwDatetr" Destination="FWDateString.Data" Vertices="(1726,390),(1590,390),(1590,450),(1590,540),(1426,540),"/>
			<Connection Source="this.HWVersion" Destination="_base.HWVersion" Vertices="(2388,630),(2216,810),"/>
			<Connection Source="_base.cOnline" Destination="BusInterfaceSdiasDriveMng.Online" Vertices="(1726,270),(1470,270),(1470,900),(892,900),"/>
			<Connection Source="this.FWNumSTring" Destination="FWNumSTring.Data"/>
			<Connection Source="this.FWDateString" Destination="FWDateString.Data"/>
			<Connection Source="this.FwDate" Destination="FWDateString.Data" Vertices="(2388,810),(2310,900),(1530,900),(1530,540),(1426,540),"/>
			<Connection Source="_base.FwNumStr" Destination="FWNumSTring.Data" Vertices="(1726,330),(1560,330),(1560,390),(1426,390),"/>
			<Connection Source="this.FWVersion" Destination="FWNumSTring.Data" Vertices="(2388,750),(2310,870),(1560,870),(1560,390),(1426,390),"/>
			<Connection Source="this.HwVersionSdias" Destination="BusInterfaceSdiasDriveMng.HwVersion" Vertices="(2388,690),(2250,840),(1080,840),(1080,600),(892,600),"/>
		</Connections>
		<!-- Headerfiles -->
		<Options>
		</Options>
	</Network>
</Class>
*)
#pragma using _DriveMngBase

_DriveMng_SR012 : CLASS
: _DriveMngBase
  //Servers:
	HwVersionSdias 	: SvrCh_HDINT;
	FWVersion 	: SvrChCmd_UDINT;
	FwDate 	: SvrChCmd_UDINT;
	LEDControl 	: SvrCh_DINT;
  //Clients:
	FWDateString 	: CltChCmd_String;
	FWNumSTring 	: CltChCmd_String;
	SdiasIn 	: CltChCmd_DINT;
	Place 	: CltCh_UDINT;
	Required 	: CltCh_DINT;
	TimeMultiplier 	: CltCh_UDINT;
	BusInterfaceSdiasDriveMng 	: CltChCmd__BusInterfaceSdiasDriveMng;
  //Variables:
  //Functions:
	
	FUNCTION VIRTUAL GetBusDeviceID
		VAR_OUTPUT
			output 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GetDrvDeviceID
		VAR_OUTPUT
			output 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL HwVersionSdias::Read
		VAR_OUTPUT
			output (EAX) 	: HDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL LEDControl::Read
		VAR_OUTPUT
			output (EAX) 	: DINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL LEDControl::Write
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

#pragma using _BusInterfaceSdiasDriveMng
#pragma using String


//}}LSL_DECLARATION


FUNCTION GLOBAL TAB _DriveMng_SR012::@CT_
0$UINT,
2#0100000000000000$UINT, //TY__DRIVEMNG_SR012
1$UINT, 0$UINT, (SIZEOF(::_DriveMng_SR012))$UINT, 
4$UINT, 7$UINT, 0$UINT, 
TO_UDINT(582716592), "_DriveMng_SR012", //Class
TO_UDINT(496739206), "_DriveMngBase", 1$UINT, 44$UINT, //Baseclass
//Servers:
(::_DriveMng_SR012.HwVersionSdias.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(3951609296), "HwVersionSdias", 
(::_DriveMng_SR012.FWVersion.pMeth)$UINT, _CH_SVR_OBJ$UINT, 2#0000000001000000$UINT, TO_UDINT(156722779), "FWVersion", 
(::_DriveMng_SR012.FwDate.pMeth)$UINT, _CH_SVR_OBJ$UINT, 2#0000000001000000$UINT, TO_UDINT(1606377179), "FwDate", 
(::_DriveMng_SR012.LEDControl.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(288098344), "LEDControl", 
//Clients:
(::_DriveMng_SR012.FWDateString.pCh)$UINT, _CH_CLT_OBJ$UINT, 2#0000000000000010$UINT, TO_UDINT(3010322216), "FWDateString", TO_UDINT(1850111279), "String", 1$UINT, 11$UINT, 
(::_DriveMng_SR012.FWNumSTring.pCh)$UINT, _CH_CLT_OBJ$UINT, 2#0000000000000010$UINT, TO_UDINT(4286323315), "FWNumSTring", TO_UDINT(1850111279), "String", 1$UINT, 11$UINT, 
(::_DriveMng_SR012.SdiasIn.pCh)$UINT, _CH_CLT_CMD$UINT, 2#0000000000000110$UINT, TO_UDINT(3019270524), "SdiasIn", 
(::_DriveMng_SR012.Place.pCh)$UINT, _CH_CLT_DATA$UINT, 2#0000000000000100$UINT, TO_UDINT(2202636669), "Place", 
(::_DriveMng_SR012.Required.pCh)$UINT, _CH_CLT_DATA$UINT, 2#0000000000000110$UINT, TO_UDINT(648558917), "Required", 
(::_DriveMng_SR012.TimeMultiplier.pCh)$UINT, _CH_CLT_DATA$UINT, 2#0000000000000100$UINT, TO_UDINT(2421717411), "TimeMultiplier", 
(::_DriveMng_SR012.BusInterfaceSdiasDriveMng.pCh)$UINT, _CH_CLT_OBJ$UINT, 2#0000000000000010$UINT, TO_UDINT(2613118149), "BusInterfaceSdiasDriveMng", TO_UDINT(1048633017), "_BusInterfaceSdiasDriveMng", 1$UINT, 12$UINT, 
END_FUNCTION


#define USER_CNT__DriveMng_SR012 5

TYPE
	_LSL_STD_VMETH	: STRUCT
			CmdTable	: CMDMETH;
			UserFcts	: ARRAY[0..USER_CNT__DriveMng_SR012] OF ^Void;
	END_STRUCT;
END_TYPE

FUNCTION _DriveMng_SR012::@STD
	VAR_OUTPUT
		ret_code	: CONFSTATES;
	END_VAR
	VAR
		vmt	: _LSL_STD_VMETH;
		nCmdSize	: UINT;
	END_VAR

	ret_code	:= _DriveMngBase::@STD();
	IF ret_code <> C_OK THEN
		RETURN;
	END_IF;
	nCmdSize		:= _DriveMngBase::CmdState.pMeth^.nCmds$UINT*SIZEOF(pVoid) + CMDMETH.Init;

	_memcpy((#vmt.CmdTable)$^USINT, _DriveMngBase::CmdState.pMeth, nCmdSize);
	vmt.CmdTable.nCmds		:= nSTDCMD + USER_CNT__DriveMng_SR012;
#pragma warning (disable : 74)
	vmt.UserFcts[3]		:= #GetBusDeviceID();

#pragma warning (default : 74)
#pragma warning (disable : 74)
	vmt.UserFcts[4]		:= #GetDrvDeviceID();

#pragma warning (default : 74)
	_DriveMngBase::CmdState.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF _DriveMngBase::CmdState.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	HwVersionSdias.pMeth			:= StoreMethod( #HwVersionSdias::Read(), #M_NO_F() );
	IF HwVersionSdias.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	_memcpy((#vmt.CmdTable)$^USINT, FWVersion.pMeth, SIZEOF(CMDMETH));
	vmt.CmdTable.nCmds		:= nSTDCMD;
	FWVersion.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF FWVersion.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	_memcpy((#vmt.CmdTable)$^USINT, FwDate.pMeth, SIZEOF(CMDMETH));
	vmt.CmdTable.nCmds		:= nSTDCMD;
	FwDate.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF FwDate.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	LEDControl.pMeth			:= StoreMethod( #LEDControl::Read(), #LEDControl::Write() );
	IF LEDControl.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;

END_FUNCTION

//{{LSL_IMPLEMENTATION
FUNCTION VIRTUAL GLOBAL _DriveMng_SR012::LEDControl::Read
	VAR_OUTPUT
		output (EAX) 	: DINT;
	END_VAR

	LEDControl := BusInterfaceSdiasDriveMng.LEDControl.Read();
	output := LEDControl;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL _DriveMng_SR012::LEDControl::Write
	VAR_INPUT
		input (EAX) 	: DINT;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: DINT;
	END_VAR

	LEDControl := input;
	result := (BusInterfaceSdiasDriveMng.LEDControl.Write(LEDControl))$DINT;

END_FUNCTION

FUNCTION VIRTUAL _DriveMng_SR012::GetBusDeviceID
	VAR_OUTPUT
		output 	: UDINT;
	END_VAR
  
  output := DRIVEMNG_BUSDEVICEID_SR012;

END_FUNCTION


FUNCTION VIRTUAL _DriveMng_SR012::GetDrvDeviceID
	VAR_OUTPUT
		output 	: UDINT;
	END_VAR
  
  output := DRIVEMNG_DRVDEVICEID_DC061;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL _DriveMng_SR012::HwVersionSdias::Read
	VAR_OUTPUT
		output (EAX) 	: HDINT;
	END_VAR

	HwVersionSdias := BusInterfaceSdiasDriveMng.HwVersion.Read();
	output := HwVersionSdias;

END_FUNCTION