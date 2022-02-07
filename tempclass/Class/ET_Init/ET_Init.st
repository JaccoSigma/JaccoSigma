//This file was generated by the LASAL2 CodeGenerator  -- 
//Please, do not edit this file (it might be overwritten by the next generator run)
//{{LSL_DECLARATION

(*!
<Class
	Name               = "ET_Init"
	Revision           = "1.1"
	GUID               = "{B8451F41-16BF-11D5-B924-00106092053B}"
	RealtimeTask       = "false"
	CyclicTask         = "false"
	BackgroundTask     = "false"
	Sigmatek           = "true"
	OSInterface        = "false"
	HighPriority       = "false"
	Automatic          = "false"
	UpdateMode         = "Prescan"
	IconPath           = ".\Class\ET_Init\toolbox.ICO"
	SharedCommandTable = "true"
	Objectsize         = "(284,120)"
	Comment            = "V1.0&#13;&#10;class for initilization &#13;&#10;the user doesn�t need to connect to this class&#13;&#10;">
	<Channels>
		<Server Name="Server0" GUID="{B8451F42-16BF-11D5-B924-00106092053B}" Visualized="true" Initialize="false" WriteProtected="false" Retentive="false" Comment="not needed"/>
	</Channels>
	<RevDoku>
		<Owner Company="Sigmatek GmbH &amp; Co KG"/>
		<Dokumentation Revision="1.1" Date="2013-06-28" Author="spimar" Company="Sigmatek GmbH &amp; Co KG" Description="ARM ready"/>
	</RevDoku>
</Class>
*)
ET_Init : CLASS
  //Servers:
	Server0 	: SvrChCmd_DINT;
  //Clients:
  //Variables:
  //Functions:
				//! <Function Comment="initilisation method&#13;&#10;not for user" Name="init_ANYTHING"/>
	FUNCTION GLOBAL init_ANYTHING
		VAR_INPUT
			pany 	: ^ET_ANYTHING;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_BTX"/>
	FUNCTION GLOBAL init_BTX
		VAR_INPUT
			pbtx 	: ^ET_BTX;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_TXTBUF"/>
	FUNCTION GLOBAL init_TXTBUF
		VAR_INPUT
			ptxt 	: ^ET_TXTBUF;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_SPRACHE"/>
	FUNCTION GLOBAL init_SPRACHE
		VAR_INPUT
			plan 	: ^ET_SPRACHE;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_REFTXT"/>
	FUNCTION GLOBAL init_REFTXT
		VAR_INPUT
			pref 	: ^ET_REFTXT;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_PICTURE"/>
	FUNCTION GLOBAL init_PICTURE
		VAR_INPUT
			ppic 	: ^ET_PICTURE;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_REFOUTPUT"/>
	FUNCTION GLOBAL init_REFOUTPUT
		VAR_INPUT
			pout 	: ^ET_REFOUTPUT;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_REFINPUT"/>
	FUNCTION GLOBAL init_REFINPUT
		VAR_INPUT
			pin 	: ^ET_REFINPUT;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_IO"/>
	FUNCTION GLOBAL init_IO
		VAR_INPUT
			pio 	: ^ET_IO;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_VARLIST"/>
	FUNCTION GLOBAL init_VARLIST
		VAR_INPUT
			plst 	: ^ET_VARLIST;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_VARLABEL"/>
	FUNCTION GLOBAL init_VARLABEL
		VAR_INPUT
			plab 	: ^ET_VARLABEL;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_VARINFO"/>
	FUNCTION GLOBAL init_VARINFO
		VAR_INPUT
			pv 	: ^ET_VARINFO;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_RESULT"/>
	FUNCTION GLOBAL init_RESULT
		VAR_INPUT
			pre 	: ^ET_RESULT;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_VARIABLE"/>
	FUNCTION GLOBAL init_VARIABLE
		VAR_INPUT
			pv 	: ^ET_VARIABLE;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_ACTUAL"/>
	FUNCTION GLOBAL init_ACTUAL
		VAR_INPUT
			pac 	: ^ET_ACTUAL;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_INTERPRETER"/>
	FUNCTION GLOBAL init_INTERPRETER
		VAR_INPUT
			pt 	: ^ET_INTERPRETER;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_INTERZLE"/>
	FUNCTION GLOBAL init_INTERZLE
		VAR_INPUT
			pt 	: ^ET_INTERZLE;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_REFKEY"/>
	FUNCTION GLOBAL init_REFKEY
		VAR_INPUT
			pt 	: ^ET_REFKEY;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_PICMEMO"/>
	FUNCTION GLOBAL init_PICMEMO
		VAR_INPUT
			pm 	: ^ET_PICMEMO;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_CHREDIT"/>
	FUNCTION GLOBAL init_CHREDIT
		VAR_INPUT
			pe 	: ^ET_CHREDIT;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_EDITOR"/>
	FUNCTION GLOBAL init_EDITOR
		VAR_INPUT
			ped 	: ^ET_EDITOR;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_CURSOR"/>
	FUNCTION GLOBAL init_CURSOR
		VAR_INPUT
			pch 	: ^ET_CURSOR;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_SYSTIME"/>
	FUNCTION GLOBAL init_SYSTIME
		VAR_INPUT
			ps 	: ^SYSTIME;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_SYSDATE"/>
	FUNCTION GLOBAL init_SYSDATE
		VAR_INPUT
			ps 	: ^SYSDATE;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_SYMBOL"/>
	FUNCTION GLOBAL init_SYMBOL
		VAR_INPUT
			ps 	: ^ET_SYMBOL;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_SYMBOLTABLE"/>
	FUNCTION GLOBAL init_SYMBOLTABLE
		VAR_INPUT
			pm 	: ^ET_SYMBOLTABLE;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_COMDEF"/>
	FUNCTION GLOBAL init_COMDEF
		VAR_INPUT
			pch 	: ^ET_COMDEF;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_LASALID"/>
	FUNCTION GLOBAL init_LASALID
		VAR_INPUT
			pl 	: ^ET_LASALID;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_typ_lslcommregdata"/>
	FUNCTION GLOBAL init_typ_lslcommregdata
		VAR_INPUT
			prd 	: ^typ_lslcommregdata;
		END_VAR;
				//! <Function Comment="initilization method&#13;&#10;not for user" Name="init_ABCDATA"/>
	FUNCTION GLOBAL init_ABCDATA
		VAR_INPUT
			pt 	: ^ET_ABCDATA;
		END_VAR;
  //Tables:
	FUNCTION @STD
		VAR_OUTPUT
			ret_code	: CONFSTATES;
		END_VAR;
	FUNCTION GLOBAL TAB @CT_;
END_CLASS;

//}}LSL_DECLARATION


FUNCTION GLOBAL TAB ET_Init::@CT_
0$UINT,
2#0100000000000000$UINT, //TY_ET_INIT
1$UINT, 1$UINT, (SIZEOF(::ET_Init))$UINT, 
1$UINT, 0$UINT, 0$UINT, 
TO_UDINT(2007384455), "ET_Init", //Class
TO_UDINT(0), 0, 0$UINT, 0$UINT, //Baseclass
//Servers:
(::ET_Init.Server0.pMeth)$UINT, _CH_CMD$UINT, 2#0000000000000000$UINT, TO_UDINT(2759080719), "Server0", 
//Clients:
END_FUNCTION


#define USER_CNT_ET_Init 0

TYPE
	_LSL_STD_VMETH	: STRUCT
			CmdTable	: CMDMETH;
			UserFcts	: ARRAY[0..USER_CNT_ET_Init] OF ^Void;
	END_STRUCT;
END_TYPE

FUNCTION ET_Init::@STD
	VAR_OUTPUT
		ret_code	: CONFSTATES;
	END_VAR
	VAR
		vmt	: _LSL_STD_VMETH;
	END_VAR

	//Command Methods
	InitCmdTable (nCmd := nSTDCMD + USER_CNT_ET_Init, pCmd := #vmt.CmdTable);
	vmt.CmdTable.Write		:= #M_WR_DIRECT();
	Server0.pMeth		:= StoreCmd (pCmd := #vmt.CmdTable, SHARED);

	IF Server0.pMeth THEN
		ret_code	:= C_OK;
	ELSE
		ret_code	:= C_OUTOF_NEAR;
		RETURN;
	END_IF;

END_FUNCTION

//{{LSL_IMPLEMENTATION


//*********************Inserted from .\ET_Init\ET_Init_00_00.st*********************



 


FUNCTION GLOBAL ET_Init::init_ANYTHING
VAR_INPUT
	pany		: ^ET_ANYTHING;
END_VAR

  pany^.list := ET_DEFLST;
  pany^.no	 := 0;

END_FUNCTION //GLOBAL ET_Init::init_ANYTHING

FUNCTION GLOBAL ET_Init::init_TXTBUF
VAR_INPUT
	ptxt		: ^ET_TXTBUF;
END_VAR

  ptxt^.no   := 0; 
  ptxt^.ptr  := NIL; 
  ptxt^.dummy[0] := '?';  
  ptxt^.dummy[1] := '?';  
  ptxt^.dummy[2] := '?';  
  ptxt^.dummy[3] := 0;  

END_FUNCTION //GLOBAL ET_Init::init_TXTBUF

FUNCTION GLOBAL ET_Init::init_SPRACHE
VAR_INPUT
	plan		: ^ET_SPRACHE;
END_VAR

  plan^.no := 0; 
  init_TXTBUF(#plan^.txtbuf);

END_FUNCTION //GLOBAL ET_Init::init_SPRACHE

FUNCTION GLOBAL ET_Init::init_REFTXT
VAR_INPUT
	pref		: ^ET_REFTXT;
END_VAR

  pref^.no    := 0;
  pref^.ptr   := NIL;

END_FUNCTION //GLOBAL ET_Init::init_REFTXT

FUNCTION GLOBAL ET_Init::init_PICTURE
VAR_INPUT
	ppic		: ^ET_PICTURE;
END_VAR

  init_SPRACHE(#ppic^.sprache);
  init_REFTXT(#ppic^.reftxt);
  init_REFOUTPUT(#ppic^.refoutput);
  init_REFINPUT(#ppic^.refinput);  
  init_ACTUAL(#ppic^.actual);
  init_EDITOR(#ppic^.editor);
  ppic^.prefkey  := NIL;
  ppic^.picstate := PS_ERROR; 
  ppic^.picno := 16#FFFF;

END_FUNCTION //GLOBAL ET_Init::init_PICTURE

FUNCTION GLOBAL ET_Init::init_BTX
VAR_INPUT
	pbtx		: ^ET_BTX;
END_VAR

  pbtx^.xpos   := 0;
  pbtx^.ypos   := 0;
  pbtx^.width  := 0;
  init_ANYTHING(#pbtx^.anything);
  pbtx^.chkbit := DEFCHKBIT;

END_FUNCTION //GLOBAL ET_Init::init_BTX

FUNCTION GLOBAL ET_Init::init_REFOUTPUT
VAR_INPUT
	pout		: ^ET_REFOUTPUT;
END_VAR

  pout^.no  := 0;
  pout^.ptr := NIL;

END_FUNCTION //GLOBAL ET_Init::init_REFOUTPUT

FUNCTION GLOBAL ET_Init::init_REFINPUT
VAR_INPUT
	pin		: ^ET_REFINPUT;
END_VAR

  pin^.no  := 0;
  pin^.ptr := NIL;

END_FUNCTION //GLOBAL ET_Init::init_REFINPUT

FUNCTION GLOBAL ET_Init::init_IO
VAR_INPUT
	pio		: ^ET_IO;
END_VAR

  pio^.xpos      := 0;
  pio^.ypos      := 0;
  pio^.varno     := 16#FFFF;
  pio^.io_format := IF_NONE; 
  pio^.oldval    := 0;
  pio^.chkbit    := DEFCHKBIT;
  init_ANYTHING(#pio^.anything); 
  pio^.txtlen    := 0;

END_FUNCTION //GLOBAL ET_Init::init_IO

FUNCTION GLOBAL ET_Init::init_VARLABEL
VAR_INPUT
	plab		: ^ET_VARLABEL;
END_VAR

  plab^.state := VLS_NONE;
  plab^.value := 0;
   
END_FUNCTION //GLOBAL ET_Init::init_VARLABEL

FUNCTION GLOBAL ET_Init::init_VARINFO
VAR_INPUT
	pv		: ^ET_VARINFO;
END_VAR

  pv^.vartyp  := ET_BOOL;    
  pv^.station := 16#FF;  
  init_VARLABEL(#pv^.limit_hi);
  init_VARLABEL(#pv^.limit_lo);
  pv^.units   := 0;
  pv^.format  := 16#0001;
  pv^.scope   := 0;
  pv^.refreshtime := 0;
    
END_FUNCTION //GLOBAL ET_Init::init_VARIABLE

FUNCTION GLOBAL ET_Init::init_VARLIST
VAR_INPUT
	plst		: ^ET_VARLIST;
END_VAR
  
  plst^.no  := 0;
  plst^.ptr := NIL;
  init_TXTBUF(#plst^.txtbuf);

END_FUNCTION //GLOBAL ET_Init::init_VARLIST

FUNCTION GLOBAL ET_Init::init_RESULT
VAR_INPUT
	pre		: ^ET_RESULT;
END_VAR

  pre^.value    := 0;
  pre^.hi_limit := 0;
  pre^.lo_limit := 0;
  init_SYSTIME(#pre^.my_time);
  init_SYSDATE(#pre^.my_date);
  pre^.my_string := NIL; 
 
END_FUNCTION //GLOBAL ET_Init::init_RESULT

FUNCTION GLOBAL ET_Init::init_VARIABLE
VAR_INPUT
	pv		: ^ET_VARIABLE;
END_VAR

  pv^.no	:= 0;
  init_VARINFO(#pv^.info);

END_FUNCTION //GLOBAL ET_Init::init_VARIABLE

FUNCTION GLOBAL ET_Init::init_ACTUAL
VAR_INPUT
	pac		: ^ET_ACTUAL;
END_VAR

  pac^.input  := 0;
  pac^.output := 0;

END_FUNCTION //GLOBAL ET_Init::init_ACTUAL

FUNCTION GLOBAL ET_Init::init_INTERZLE
VAR_INPUT
	pt		: ^ET_INTERZLE;
END_VAR

  pt^.bef := _ET_NOP;
  init_VARLABEL(#pt^.op1);
  init_VARLABEL(#pt^.op2);

END_FUNCTION //GLOBAL ET_Init::init_INTERZLE

FUNCTION GLOBAL ET_Init::init_INTERPRETER
VAR_INPUT
	pt		: ^ET_INTERPRETER;
END_VAR
VAR
    i		:UINT;
END_VAR

  pt^.no := 0;
  for i:=0 to 9 do
    init_INTERZLE(#pt^.zle[i]);
  end_for;

END_FUNCTION //GLOBAL ET_Init::init_INTERPRETER

FUNCTION GLOBAL ET_Init::init_REFKEY
VAR_INPUT
	pt		: ^ET_REFKEY;
END_VAR

  pt^.no := 0;
  pt^.offset[0] := 0;  
  pt^.offset[1] := 0;

END_FUNCTION //GLOBAL ET_Init::init_REFKEY

FUNCTION GLOBAL ET_Init::init_PICMEMO
VAR_INPUT
	pm		: ^ET_PICMEMO;
END_VAR
VAR
  i         :UINT;
END_VAR

  pm^.no   := 0;
  for i:=0 to 9 do
    pm^.memo[i] := 0; 
  end_for;  

END_FUNCTION //GLOBAL ET_Init::init_PICMEMO

FUNCTION GLOBAL ET_Init::init_CHREDIT
VAR_INPUT
	pe		: ^ET_CHREDIT;
END_VAR
VAR
    i		: UINT;
END_VAR;

  pe^.pos      := 0;
  for i:=0 to 99 do
    pe^.txt[i] := 0;
  end_for;

END_FUNCTION //GLOBAL ET_Init::init_CHREDIT

FUNCTION GLOBAL ET_Init::init_EDITOR
VAR_INPUT
	ped		: ^ET_EDITOR;
END_VAR
VAR
    i       : UINT;
END_VAR


  ped^.no      := 16#FFFF;
  init_IO(#ped^.io);
  init_RESULT(#ped^.result);
  init_CHREDIT(#ped^.chredit);
  init_RESULT(#ped^.newres);
  ped^.change  := FALSE;
  init_VARIABLE(#ped^.variable);
  ped^.cntr    := 0;
  ped^.actpos  := 0;
    
  for i:=0 to 39 do
    ped^.memo[i] := 0;
  end_for;


END_FUNCTION //GLOBAL ET_Init::init_EDITOR

FUNCTION GLOBAL ET_Init::init_CURSOR
VAR_INPUT
  pch 	: ^ET_CURSOR;
END_VAR
VAR
  i         : UINT;
END_VAR

  pch^.x       := 0;
  pch^.y       := 0;
  pch^.width   := 0;
  for i:=0 to 29 do
    pch^.memo[i]   := 0;
	pch^.slurry[i] := 0; 
  end_for;
  pch^.status     := FALSE;
  pch^.active     := false; 

END_FUNCTION //GLOBAL ET_Init::init_CURSOR

FUNCTION GLOBAL ET_Init::init_SYSTIME
VAR_INPUT
	ps		: ^SYSTIME;
END_VAR

  ps^.wHour   := 0;
  ps^.wMinute := 0;
  ps^.wSecond := 0;

END_FUNCTION //GLOBAL ET_Init::init_SYSTIME

FUNCTION GLOBAL ET_Init::init_SYSDATE
VAR_INPUT
	ps		: ^SYSDATE;
END_VAR

  ps^.wYear      := 0;
  ps^.wMonth     := 0;
  ps^.wDay	     := 0;
  ps^.wDayOfWeek := 0;

END_FUNCTION //GLOBAL ET_Init::init_SYSDATE

FUNCTION GLOBAL ET_Init::init_SYMBOL
VAR_INPUT
	ps		: ^ET_SYMBOL;
END_VAR

  ps^.valid  := FALSE;  
  init_LASALID(#ps^.id);
  ps^.offset := 16#FFFF;
  ps^.sent   := FALSE; 
  ps^.varobj := FALSE; 
  ps^.reflistentryno := 16#FFFF; 

END_FUNCTION //GLOBAL ET_Init::init_SYMBOL

FUNCTION GLOBAL ET_Init::init_SYMBOLTABLE
VAR_INPUT
	pm		: ^ET_SYMBOLTABLE;
END_VAR

  pm^.no     := 0;
  pm^.ptr    := NIL;  
  pm^.pdata  := NIL;  

END_FUNCTION //GLOBAL ET_Init::init_SYMBOLTABLE

FUNCTION GLOBAL ET_Init::init_COMDEF
VAR_INPUT
  pch 	: ^ET_COMDEF;
END_VAR

  pch^.typ     := ET_COM_LOCAL;
  pch^.adress  := 0;
  pch^.handle  := 0;   //16#FFFF;
  pch^.login   := ET_LOGIN_INIT;

END_FUNCTION //GLOBAL ET_Init::init_COMDEF

FUNCTION GLOBAL ET_Init::init_LASALID
VAR_INPUT
	pl		: ^ET_LASALID;
END_VAR

  pl^.id  := 0;
  pl^.typ := 0;
  pl^.len := 0;

END_FUNCTION //GLOBAL ET_Init::init_LASALID

FUNCTION GLOBAL ET_Init::init_typ_lslcommregdata
VAR_INPUT
	prd		: ^typ_lslcommregdata;
END_VAR

  prd^.id        := 0;
  prd^.channel   := 0;
  prd^.varpos    := 0;
  prd^.ftime     := 0;
  prd^.varlistid := 0;


END_FUNCTION //GLOBAL ET_Init::init_REGDATA

FUNCTION GLOBAL ET_Init::init_ABCDATA
VAR_INPUT
	pt		: ^ET_ABCDATA;
END_VAR

  pt^.delaytime := 20000;
  pt^.index     := 0;
  pt^.tabno     := 255; 

END_FUNCTION //GLOBAL ET_Init::init_ABC_DATA
