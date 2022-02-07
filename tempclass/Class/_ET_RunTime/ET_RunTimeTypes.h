#ifndef __ET_KERNEL_TYPES__
#define __ET_KERNEL_TYPES__

TYPE
  ET_CHKBIT :    UINT;
  ET_LIST : ( ET_STX:=0,
    ET_ATX,
    ET_MTX,
    ET_ET1,
    ET_LTX,
    ET_DEFLST )$USINT;
#pragma pack(push, 1)
  ET_ANYTHING : STRUCT
    list :      ET_LIST;
    no :      UINT;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_BTX : STRUCT
    xpos :      USINT;
    ypos :      USINT;
    width :      USINT;
    anything :      ET_ANYTHING;
    chkbit :      ET_CHKBIT;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_TXTBUF : STRUCT
    no :      UINT;
    ptr :     ^UINT;
    dummy : ARRAY[
      0..3] OF        char;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_SPRACHE : STRUCT
    no :      UINT;
    txtbuf :      ET_TXTBUF;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_REFTXT : STRUCT
    no :      UINT;
    ptr :     ^ET_BTX;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_ACTUAL : STRUCT
    input :      UINT;
    output :      UINT;
  END_STRUCT;

#pragma pack(pop) 
   ET_PICSTATE : ( PS_DRAW:=0,
    PS_REFRESH,
    PS_ERROR )$USINT;

#pragma pack(push, 1)
  ET_CHREDIT : STRUCT
    pos :      UINT;
    txt : ARRAY[
      0..99] OF        char;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_EDITOR : STRUCT
    no :      UINT;
    io :      ET_IO;
    result :      ET_RESULT;
    chredit :      ET_CHREDIT;
    newres :      ET_RESULT;
    change :      BOOL;
    variable :      ET_VARIABLE;
    cntr :      UINT;
    actpos :      UINT;
    memo : ARRAY[
      0..39] OF        char;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_PICTURE : STRUCT
    sprache :      ET_SPRACHE;
    reftxt :       ET_REFTXT;
    refoutput :    ET_REFOUTPUT;
    refinput :     ET_REFINPUT;
    actual :       ET_ACTUAL;
    prefkey :      ET_PREFKEY;
    picstate :     ET_PICSTATE;
    editor :       ET_EDITOR;
    picno :      UINT;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_PICMEMO : STRUCT
    no :      UINT;
    memo : ARRAY[
      0..9] OF        UINT;
  END_STRUCT;

#pragma pack(pop) 
   ET_IO_FORMAT : ( IF_NONE:=0,
    IF_NUMERIC,
    IF_HHMM,
    IF_HHMMSS,
    IF_TTMM,
    IF_TTMMYY,
    IF_TTMMYYYY,
    IF_WOTTMM,
    IF_WOTTMMYY,
    IF_WOTTMMYYYY,
    IF_STRING,
    IF_TEXT,
    IF_GRAPHIC,
    IF_CODE )$USINT;

#pragma pack(push, 1)
  ET_IO : STRUCT
    xpos :      USINT;
    ypos :      USINT;
    varno :      UINT;
    oldval :      UDINT;
    io_format :      ET_IO_FORMAT;
    chkbit :      ET_CHKBIT;
    anything :      ET_ANYTHING;
    txtlen :      USINT;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_REFOUTPUT : STRUCT
    no :      UINT;
    ptr :     ^ET_IO;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_REFINPUT : STRUCT
    no :      UINT;
    ptr :     ^ET_IO;
  END_STRUCT;

#pragma pack(pop) 
   ET_VARTYP : ( ET_UBYTE:=0,
    ET_BYTE,
    ET_UWORD,
    ET_WORD,
    ET_UDWORD,
    ET_DWORD,
    ET_BOOL,
    ET_TIME,
    ET_DATE,
    ET_STRING,
    ET_OBJECT,
    ET_STRUCT,
    ET_TEXT )$USINT;
  ET_VARLABELSTATE : ( VLS_NONE:=0,
    VLS_CONSTANT,
    VLS_VARIABLE )$USINT;

#pragma pack(push, 1)
  ET_VARLABEL : STRUCT
    state :      ET_VARLABELSTATE;
    value :      DINT;
  END_STRUCT;

#pragma pack(pop) 
   ET_STATION :    USINT;

#pragma pack(push, 1)
  ET_VARINFO : STRUCT
    vartyp :      ET_VARTYP;
    station :      ET_STATION;
    limit_hi :      ET_VARLABEL;
    limit_lo :      ET_VARLABEL;
    units :      UINT;
    format :      HINT;
    scope :      UINT;
    refreshtime :      UINT;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_VARLIST : STRUCT
    no :      UINT;
    ptr :     ^ET_VARINFO;
    txtbuf :      ET_TXTBUF;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_RESULT : STRUCT
    value :      DINT;
    hi_limit :      DINT;
    lo_limit :      DINT;
    my_time :      SYSTIME;
    my_date :      SYSDATE;
    my_string :     ^CHAR;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_VARIABLE : STRUCT
    no :      UINT;
    info :      ET_VARINFO;
  END_STRUCT;

#pragma pack(pop) 
   ET_INTERBEF : ( _ET_CHAPIC:=0,
    _ET_RETPIC,
    _ET_LAN,
    _ET_FCAWL,
    _ET_INC,
    _ET_DEC,
    _ET_ADD,
    _ET_SUB,
    _ET_SET,
    _ET_BEEP,
    _ET_TOGGLE,
    _ET_LED,
    _ET_KEY,
    _ET_NEXT,
    _ET_BEFOR,
    _ET_KEYCOD,
    _ET_STRING,
    _ET_NOP )$USINT;

#pragma pack(push, 1)
  ET_INTERZLE : STRUCT
    bef :      ET_INTERBEF;
    op1 :      ET_VARLABEL;
    op2 :      ET_VARLABEL;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_INTERPRETER : STRUCT
    no :      USINT;
    zle : ARRAY[
      0..9] OF        ET_INTERZLE;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  ET_REFKEY : STRUCT
    no :      UINT;
    offset : ARRAY[
      0..1] OF        UINT;
  END_STRUCT;

#pragma pack(pop) 
   ET_PREFKEY :   ^ET_REFKEY;

#pragma pack(push, 1)
  ET_CURSOR : STRUCT
    x :      UINT;
    y :      UINT;
    width :      UINT;
    memo : ARRAY[
      0..29] OF        char;
    slurry : ARRAY[
      0..29] OF        char;
    status :      BOOL;
    active :      BOOL;
  END_STRUCT;

#pragma pack(pop) 
   ET_SYMBOL : STRUCT
    valid :      BOOL;
    id :      ET_LASALID;
    offset :      UINT;
    sent :      BOOL;
    varobj :      BOOL;
    reflistentryno :      UINT;
  END_STRUCT;
  ET_SYMBOLTABLE : STRUCT
    no :      UINT;
    ptr :     ^ET_SYMBOL;
    pdata :     ^USINT;
  END_STRUCT;
  ET_INTERFACE : ( ET_COM_LOCAL:=0,
    ET_COM_INTERN,
    ET_COM_COM1,
    ET_COM_COM2,
    ET_COM_COM3,
    ET_COM_COM4,
    ET_COM_CAN1,
    ET_COM_CAN2 )$UDINT;
  ET_LOGIN : ( ET_LOGIN_INIT:=0,
    ET_LOGIN_TRUE,
    ET_LOGIN_FALSE )$UINT;

#pragma pack(push, 1) 
  ET_COMDEF : STRUCT            
    typ :      ET_INTERFACE;
    adress :      UINT;
    handle :     ^VOID;
    login :      ET_LOGIN;
  END_STRUCT;
#pragma pack(pop) 

#pragma pack(push, 1)
  ET_LASALID : STRUCT
    id :      UDINT;
    typ :      USINT;
    len :      UINT;
  END_STRUCT;

#pragma pack(pop) 
 
#pragma pack(push, 1)
  typ_lslcommregdata : STRUCT
    id :      UDINT;
    channel :      UINT;
    varpos :      UINT;
    ftime :      UINT;
    varlistid :      UDINT;
  END_STRUCT;

#pragma pack(pop) 
   ET_ABCDATA : STRUCT
    delaytime :      UDINT;
    index :      USINT;
    tabno :      USINT;
  END_STRUCT;

#endif // __ET_KERNEL_TYPES__
END_TYPE