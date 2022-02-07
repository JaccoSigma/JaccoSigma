//This file was generated by the LASAL2 CodeGenerator  -- 
//Please, do not edit this file (it might be overwritten by the next generator run)
//{{LSL_DECLARATION

(*!
<Class
	Name               = "Lasal32User"
	Revision           = "0.0"
	GUID               = "{63C826B3-4D41-429F-AD39-F33A6DCF9B1E}"
	RealtimeTask       = "false"
	CyclicTask         = "false"
	BackgroundTask     = "true"
	DefBackground      = "25 ms"
	Sigmatek           = "false"
	OSInterface        = "false"
	HighPriority       = "false"
	Automatic          = "false"
	UpdateMode         = "Prescan"
	SharedCommandTable = "true"
	Objectsize         = "(1050,660)">
	<Channels>
		<Server Name="IP_Interface" GUID="{609990AE-3808-46C7-BF90-1FA3029B7334}" Visualized="false" Initialize="true" DefValue="1" WriteProtected="false" Retentive="false"/>
		<Server Name="IP_num1" GUID="{24BFA44E-C107-425F-8879-739091FA40F5}" Visualized="false" Initialize="true" WriteProtected="false" Retentive="false"/>
		<Server Name="IP_num2" GUID="{F16F7A73-BD5E-49B1-B774-E2D095FD9AE9}" Visualized="false" Initialize="true" WriteProtected="false" Retentive="false"/>
		<Server Name="IP_num3" GUID="{A881EF5C-FC9A-4F93-AFA0-5C05233D7BE6}" Visualized="false" Initialize="true" WriteProtected="false" Retentive="false"/>
		<Server Name="IP_num4" GUID="{91160413-40EA-4D90-B2D4-2F61F4BEA127}" Visualized="false" Initialize="true" WriteProtected="false" Retentive="false"/>
		<Server Name="IP_Port" GUID="{A4F9E7A6-43A3-478D-855E-F77352FC109A}" Visualized="false" Initialize="true" WriteProtected="false" Retentive="false"/>
		<Server Name="My_Methods" GUID="{C05BBD9A-5109-4FFE-A29F-75F9B758F10E}" Visualized="false" Initialize="false" WriteProtected="false" Retentive="false"/>
		<Server Name="Online_Status" GUID="{176D485E-663D-495D-9CB1-0225CEADDCB1}" Visualized="false" Initialize="false" WriteProtected="false" Retentive="false"/>
		<Server Name="TimeOutTime_Logoff" GUID="{0E5EA3C1-C71E-4FE3-9B7A-80BA6CE6462C}" Visualized="false" Initialize="true" DefValue="120000" WriteProtected="false" Retentive="false" Comment="after x ms doing nothing we logoff.&#13;&#10;When 0, we don&apos;t use the timeout."/>
		<Client Name="Lasal321" Required="true" Internal="true"/>
	</Channels>
	<Separators>
		<Servers>
			<SepChn Position="2"/>
			<SepChn Position="9"/>
		</Servers>
	</Separators>
	<Network Name="Lasal32User">
		<!-- List of Components in this network -->
		<Components>
			<Object
				Name       = "Lasal321"
				GUID       = "{746B5768-D891-48CC-A17D-D382E45CC020}"
				Class      = "Lasal32"
				Position   = "(180,300)"
				Visualized = "false">
				<Channels>
					<Server Name="ClassSvr"/>
				</Channels>
			</Object>
		</Components>
		<Comments>
		</Comments>
		<!-- List of Connections in this network -->
		<Connections>
			<Connection Source="this.Lasal321" Destination="Lasal321.ClassSvr"/>
		</Connections>
		<!-- Headerfiles -->
		<Options>
		</Options>
	</Network>
</Class>
*)
Lasal32User : CLASS
	TYPE
	  Do_MyMethods :  //! <Type Public="true" Name="Do_MyMethods"/>
	  (
	    Status_Go_Online_First,
	    Do_Online,
	    Do_Offline,
	    Do_Reset,
	    Do_Run,
	    Do_Reboot,
	    Status_Still_Online,
	    Status_Its_Succesfull
	  )$UDINT;
	END_TYPE
  //Servers:
	My_Methods 	: SvrChCmd_Do_MyMethods_PTofCls_Lasal32User;
	Online_Status 	: SvrCh__LSLONL_STATUS_PLC;
	IP_Interface 	: SvrCh_UDINT;
	IP_num1 	: SvrCh_UDINT;
	IP_num2 	: SvrCh_UDINT;
	IP_num3 	: SvrCh_UDINT;
	IP_num4 	: SvrCh_UDINT;
	IP_Port 	: SvrCh_UDINT;
	TimeOutTime_Logoff 	: SvrCh_UDINT;
  //Clients:
	Lasal321 	: CltChCmd_Lasal32;
  //Variables:
		vStepper 	: DINT;
		vRefresh 	: DINT;
		ExternalStation 	: OS_LSLONL_ADDR_TCPIP;
		ONL_Handle 	: DINT;
		MethodReturncode 	: DINT;
		TimeoutSetTime 	: UDINT;
		RefreshSetTime 	: UDINT;
  //Functions:
	
	FUNCTION VIRTUAL GLOBAL Init;
	
	FUNCTION VIRTUAL GLOBAL Background
		VAR_INPUT
			EAX 	: UDINT;
		END_VAR
		VAR_OUTPUT
			state (EAX) 	: UDINT;
		END_VAR;
	
	FUNCTION VIRTUAL GLOBAL My_Methods::Write
		VAR_INPUT
			input (EAX) 	: Lasal32User::Do_MyMethods;
		END_VAR
		VAR_OUTPUT
			result (EAX) 	: Lasal32User::Do_MyMethods;
		END_VAR;
  //Tables:
	FUNCTION @STD
		VAR_OUTPUT
			ret_code	: CONFSTATES;
		END_VAR;
	FUNCTION GLOBAL TAB @CT_;
END_CLASS;

#pragma using Lasal32


//}}LSL_DECLARATION


FUNCTION GLOBAL TAB Lasal32User::@CT_
0$UINT,
2#0100000000000010$UINT, //TY_LASAL32USER
0$UINT, 0$UINT, (SIZEOF(::Lasal32User))$UINT, 
9$UINT, 1$UINT, 0$UINT, 
TO_UDINT(681806995), "Lasal32User", //Class
TO_UDINT(0), 0, 0$UINT, 0$UINT, //Baseclass
//Servers:
(::Lasal32User.My_Methods.pMeth)$UINT, _CH_CMD$UINT, 2#0000000000000000$UINT, TO_UDINT(787620908), "My_Methods", 
(::Lasal32User.Online_Status.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(1789677515), "Online_Status", 
(::Lasal32User.IP_Interface.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(1887546739), "IP_Interface", 
(::Lasal32User.IP_num1.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(4287057182), "IP_num1", 
(::Lasal32User.IP_num2.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(1720589476), "IP_num2", 
(::Lasal32User.IP_num3.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(294202418), "IP_num3", 
(::Lasal32User.IP_num4.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(2414721425), "IP_num4", 
(::Lasal32User.IP_Port.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(2926760029), "IP_Port", 
(::Lasal32User.TimeOutTime_Logoff.pMeth)$UINT, _CH_SVR$UINT, 2#0000000000000000$UINT, TO_UDINT(4052062914), "TimeOutTime_Logoff", 
//Clients:
(::Lasal32User.Lasal321.pCh)$UINT, _CH_CLT_OBJ$UINT, 2#0000000000000010$UINT, TO_UDINT(1571067297), "Lasal321", TO_UDINT(3286553581), "Lasal32", 1$UINT, 4$UINT, 
END_FUNCTION


#define USER_CNT_Lasal32User 1

TYPE
	_LSL_STD_VMETH	: STRUCT
			CmdTable	: CMDMETH;
			UserFcts	: ARRAY[0..USER_CNT_Lasal32User] OF ^Void;
	END_STRUCT;
END_TYPE

FUNCTION Lasal32User::@STD
	VAR_OUTPUT
		ret_code	: CONFSTATES;
	END_VAR
	VAR
		vmt	: _LSL_STD_VMETH;
	END_VAR

	//Command Methods
	InitCmdTable (nCmd := nSTDCMD + USER_CNT_Lasal32User, pCmd := #vmt.CmdTable);
	vmt.CmdTable.Write		:= #Write();
	vmt.CmdTable.Init		:= #Init();
#pragma warning (disable : 74)
	vmt.UserFcts[0]		:= #Background();

#pragma warning (default : 74)
	My_Methods.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF My_Methods.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	Online_Status.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF Online_Status.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	IP_Interface.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF IP_Interface.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	IP_num1.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF IP_num1.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	IP_num2.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF IP_num2.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	IP_num3.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF IP_num3.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	IP_num4.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF IP_num4.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	IP_Port.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF IP_Port.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;
	TimeOutTime_Logoff.pMeth			:= StoreMethod( #M_RD_DIRECT(), #M_WR_DIRECT() );
	IF TimeOutTime_Logoff.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;

END_FUNCTION

//{{LSL_IMPLEMENTATION
(*JM 2021 - Tested in CP312 OS 09.03.200
  #################################################
  Niet vergeten: lsl_st_onl.h Global op TRUE zetten
  HvZ Aug 2021
  #################################################
  Make sure alle IP Interfaces are not using the IP
  of the CPU you are trying to connect to. (Hint:
  DHCP default setting!?)
*)

FUNCTION VIRTUAL GLOBAL Lasal32User::Init
  
  if _FirstScan = TRUE then //our prime time! Set online settings in vars.
    //set settinngs
    ExternalStation.OnlType := ONLTYPE_TCPIP;
    ExternalStation.InterfaceNbr := IP_Interface;
    ExternalStation.IP1 := IP_num1$USINT;
    ExternalStation.IP2 := IP_num2$USINT;
    ExternalStation.IP3 := IP_num3$USINT;
    ExternalStation.IP4 := IP_num4$USINT;
    ExternalStation.Port := IP_Port;
    Online_Status := ::_LSLONL_STATUS_PLC::LSLONL_NOT_CONNECTED;
  end_if;

END_FUNCTION


FUNCTION VIRTUAL GLOBAL Lasal32User::My_Methods::Write
	VAR_INPUT
		input (EAX) 	: Lasal32User::Do_MyMethods;
	END_VAR
	VAR_OUTPUT
		result (EAX) 	: Lasal32User::Do_MyMethods;
	END_VAR
  
  My_Methods := input;
  
  case My_Methods of
  	Lasal32User::Do_MyMethods::Do_Reset:
      vStepper := 30;
     
  	Lasal32User::Do_MyMethods::Do_Run:
      vStepper := 40;

  	Lasal32User::Do_MyMethods::Do_Reboot:
      vStepper := 50;      
      
    Lasal32User::Do_MyMethods::Do_Online:
      vStepper := 10;
      
    Lasal32User::Do_MyMethods::Do_Offline:
      vStepper := 20;      
  end_case;
  
  vRefresh := 0;
  
  
	result := My_Methods;

END_FUNCTION



FUNCTION VIRTUAL GLOBAL Lasal32User::Background
	VAR_INPUT
		EAX 	: UDINT;
	END_VAR
	VAR_OUTPUT
		state (EAX) 	: UDINT;
	END_VAR
  VAR
  	dRETCode : DINT;
  END_VAR

  
  case vStepper of
  00: //Nothing, after x time, we go offline.**************************************************************************
    if TimeOutTime_Logoff > 0 & ONL_Handle > 0 then
      if (TimeoutSetTime+TimeOutTime_Logoff) < ops.tAbsolute then
        vStepper := 20;
      end_if;
    end_if;
  01: //Function Successful********************************************************************************************
    My_Methods := Status_Its_Succesfull;
    vStepper := 0;
    TimeoutSetTime := ops.tAbsolute;
  10: //Online*********************************************************************************************************
    if ONL_Handle = 0 then
      ONL_Handle := Lasal321.ONL_OPEN_TCPIP(addr:=#ExternalStation); //Tip: Set lsl_st_onl.h Global on TRUE.
      if ONL_Handle > 0 then
        vStepper := 01;
      else
        vStepper := 99;
      end_if;
    else
      My_Methods := Status_Still_Online;
      vStepper := 0;
    end_if;
  20: //Offline********************************************************************************************************
    if ONL_Handle > 0 then
      dRETCode := Lasal321.ONL_CLOSE(descr_num:=ONL_Handle);
      if dRETCode <> 0 then
        vStepper := 98;
        MethodReturncode := dRETCode;
      else
        vStepper := 01;
        ONL_Handle := 0;
        Online_Status := ::_LSLONL_STATUS_PLC::LSLONL_NOT_CONNECTED;
      end_if;
    else
      My_Methods := Status_Go_Online_First;
    end_if;
  30: //Reset**********************************************************************************************************
    if ONL_Handle > 0 then
      dRETCode := Lasal321.ONL_RESETSPS(descr_num:=ONL_Handle);
      if dRETCode <> 0 then
        vStepper := 97;
        MethodReturncode := dRETCode;
      else
        vStepper := 01;
      end_if;
    else
      My_Methods := Status_Go_Online_First;
      vStepper := 0;
    end_if;    
  40: //Run************************************************************************************************************
    if ONL_Handle > 0 then
      dRETCode := Lasal321.ONL_RUNSPS(descr_num:=ONL_Handle);
      if dRETCode <> 0 then
        vStepper := 96;
        MethodReturncode := dRETCode;
      else
        vStepper := 1;
      end_if;
    else
      My_Methods := Status_Go_Online_First;
      vStepper := 0;
    end_if;
  50: //Reboot*********************************************************************************************************
    if ONL_Handle > 0 then
      dRETCode := Lasal321.ONL_SERVICEPROVIDER(descr_num:=ONL_Handle, pCmd:="REBOOT");
      if dRETCode <> 0 then
        vStepper := 95;
        MethodReturncode := dRETCode;
      else
        vStepper := 01;
        ONL_Handle := 0;
      end_if;
    else
      My_Methods := Status_Go_Online_First;
      vStepper := 0;
    end_if;
  60: //GetCPUStatus***************************************************************************************************
    if ONL_Handle > 0 then
      dRETCode := Lasal321.ONL_GETCPUSTATUS(descr_num:=ONL_Handle, status:=#Online_Status);
      if dRETCode < 0 then
        vStepper := 94;
        MethodReturncode := dRETCode;
      else
        vStepper := 0;
      end_if;
    else
      My_Methods := Status_Go_Online_First;
      vStepper := 0;
    end_if;
    
  99: //Error going online
  98: //Error going offline
  97: //Error reset.
  96: //Error Run
  95: //Error Reboot
  94: //Error GetCPUStatus
  end_case;
  
  if vStepper = 0 then
    case vRefresh of //when online we keep checking the current status.
    00:
      RefreshSetTime := ops.tAbsolute;
      vRefresh := 10;
    10:
      if (RefreshSetTime+2000) < ops.tAbsolute then
        vRefresh := 0;
        vStepper := 60;
      end_if;
    end_case;
  end_if;
  
	state := READY;

END_FUNCTION