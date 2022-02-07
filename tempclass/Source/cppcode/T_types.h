
#ifndef T_TYPES_H
#include <St_graph.h>
#include "T_IPC.H"

TYPE
#ifndef _T_TYPESX_H
#define _T_TYPESX_H

#include "T_COLOR.H"

// **************************************************************************
// **************************************************************************
// ***  sigmatek configuration (user keep off !!)                         ***
// **************************************************************************
// **************************************************************************

// ENUMERATED DATATYPES *****************************************************

#pragma pack (push, 4)

_LOCATION :   ( SL_UNDEF,
                SL_LAYER,
                SL_OBJECT,
                SL_MOTION,
                SL_INPUT,
                SL_BUTTON,
                SL_OUTPUT,
                SL_KARU
              )$UDINT;

_IMA :        ( IMA_PICTURE,
                IMA_WINDOW,
                IMA_GLOBAL,
                IMA_POPUP,
                IMA_SCREENSAVER,
                IMA_TOUCHEDITOR,
                IMA_OFF_PICTURE,
                IMA_OFF_WINDOW,
                IMA_NOTHING
              )$UDINT;

_FOUNDHID :   ( FOUND_BUTTON,
                FOUND_INPUT,
                FOUND_ESC,
                FOUND_SOFTKEY,
                FOUND_CANCLE
              )$UDINT;

_WHOBEEP :    ( BEEP_UNKNOWN,
                BEEP_BUTTON,
                BEEP_INPUT,
                BEEP_SOFTKEY,
                BEEP_SCROLLSELECT,       
                BEEP_SCROLLCONFIRM, 
                BEEP_LEVEL,              // levelbutton (scrolllist, menue)
                BEEP_ESC,                // close window (hid outside)
                BEEP_X                   // canclebutton of theme
              )$UDINT;

_ACCESSFAILED:( NOTHING_FAILED,          // unlimited access
                CHKBIT_FAILED,           // chkbit
                STATESCHEME_FAILED,      // statescheme
                ACCESS_FAILED,           // accesslevel
                USERIF_FAILED            // userinterface
              )$UDINT;

_INTERFACE :  ( _COM_LOCAL,         // local data (mae local)
                _COM_INTERN,        // intern data (2'nd task)
                _COM_COM1,          // extern data com1
                _COM_COM2,          // extern data com2
                _COM_COM3,          // extern data com3
                _COM_COM4,          // extern data com4
                _COM_CAN1,          // extern data can1
                _COM_CAN2,          // extern data can2
                _COM_TCPIP1,        // extern data tcpip1
                _COM_TCPIP1_P:=10,  // extern data tcpip1 + port
                _COM_TCPIP2_P,      // extern data tcpip2 + port
                _COM_COM5,          // extern data com5
                _COM_COM6,          // extern data com6
                _COM_COM7,          // extern data com7
                _COM_COM8,          // extern data com8
                _COM_COM9,          // extern data com9
                _COM_COM10,         // extern data com10
                _COM_CAN20:=20,
                _COM_CAN21,
                _COM_CAN22,
                _COM_CAN23,
                _COM_CAN24,
                _COM_CAN25,
                _COM_CAN26,
                _COM_CAN27,
                _COM_CAN28,
                _COM_CAN29
              )$UDINT;
             
_LOCAL :      ( LOCAL_UNKNOWN             := 0x00,
                LOCAL_ARABIC              := 0x01,
                LOCAL_BULGARIAN           := 0x02,
                LOCAL_CATALAN             := 0x03,
                LOCAL_CHINESE             := 0x04,
                LOCAL_CZECH               := 0x05,
                LOCAL_DANISH              := 0x06,
                LOCAL_GERMAN              := 0x07,
                LOCAL_GREEK               := 0x08,
                LOCAL_ENGLISH             := 0x09,
                LOCAL_SPANISH             := 0x0A,
                LOCAL_FINNISH             := 0x0B,
                LOCAL_FRENCH              := 0x0C,
                LOCAL_HEBREW              := 0x0D,
                LOCAL_HUNGARIAN           := 0x0E,
                LOCAL_ICELANDIC           := 0x0F,
                LOCAL_ITALIAN             := 0x10,
                LOCAL_JAPANESE            := 0x11,
                LOCAL_KOREAN              := 0x12,
                LOCAL_DUTCH               := 0x13,
                LOCAL_NORWEGIAN           := 0x14,
                LOCAL_POLISH              := 0x15,
                LOCAL_PORTUGESE           := 0x16,
                LOCAL_ROMANIAN            := 0x18,
                LOCAL_RUSSIAN             := 0x19,
                LOCAL_CROATIA_SERBIAN     := 0x1A,
                LOCAL_SLOVAK              := 0x1B,
                LOCAL_ALBANIAN_MACEDONIAN := 0x1C,
                LOCAL_SWEDISH             := 0x1D,
                LOCAL_THAI                := 0x1E,
                LOCAL_TURKISH             := 0x1F,
                LOCAL_URDU                := 0x20,
                LOCAL_INDONESIAN          := 0x21,
                LOCAL_UKRAINIAN           := 0x22,
                LOCAL_BYELORUSSIAN        := 0x23,
                LOCAL_SLOVENIAN           := 0x24,
                LOCAL_ESTONIAN            := 0x25,
                LOCAL_LATVIAN             := 0x26,
                LOCAL_LITHUANIAN          := 0x27,
                LOCAL_FARSI               := 0x29,
                LOCAL_VIETNAMESE          := 0x2A,
                LOCAL_PASQUE              := 0x2D,
                LOCAL_AFRICAANS           := 0x36,
                LOCAL_GEORGIAN            := 0x37,
                LOCAL_FAEROESE            := 0x38,
                LOCAL_HINDI               := 0x39
              )$UDINT;


_STDTEXTID :  ( TxtID_OK,     
                TxtID_CANCLE,
                TxtID_YES,
                TxtID_NO,
                TxtID_WEEKDAYS
              )$UDINT;
              
              
_MESSAGEBOXTYP :    ( MESSAGEBOXTYP_OK,
                      MESSAGEBOXTYP_OK_CANCLE,
                      MESSAGEBOXTYP_YES_NO
                    )$UDINT;
                 
_MESSAGEBOXRESULT : ( MESSAGEBOXRESULT_NONE,
                      MESSAGEBOXRESULT_OK,
                      MESSAGEBOXRESULT_CANCLE,
                      MESSAGEBOXRESULT_YES,
                      MESSAGEBOXRESULT_NO
                    )$UDINT;
              
_SELIS      : ( SELIS_NONE,
                SELIS_DRIVE,
                SELIS_ROOTLOCK,
                SELIS_DIRECTORY,
                SELIS_FILE
              )$UDINT;

_BOOL :       BOOL;
#define _TRUE  true
#define _FALSE false

_DOIT :       ( _IDLE,              // nothing done
                _IDIDIT,            // i did it
                _IFAILED            // i tried but failed
              )$UDINT;              // method state

_TIP :        ( _TIPCOMBI,          // autorepeat 0.5, 0.1
                _TIPCONTI,          // continuous rating
                _TIPSINGLE,         // singleshot
                _TIPAUTO            // autorepeat 0.1, 0.1
              )$UDINT;

_WORKSTATE :  ( STATE_START,        // session start
                STATE_READY,        // session ready
                STATE_RUN,          // session run
                STATE_END,          // session end
                STATE_FINISH,       // session closed
                STATE_BACKGROUND,   // refresh background (_MOTION)
                STATE_OVERLOAD,     // another overload try
                STATE_ERROR,        // error
                STATE_FLY           // download on the fly
              )$UDINT;

_VARTYPE :    ( BINARY,             // binary
                FSTRING,            // string
                FTIME,              // time
                FDATE,              // date
                XRADIO,             // nc
                BIT,                // bit datatype
                XBITMASK,           // nc
                COMPLEX,            // structure, complex
                FREE,               // nc
                FCONSTANT,          // constant type
                UNUSED,             // nc
                OBJECT,             // objecttype
                UNUSED2,            // im lse reserviert
                FMERKER,            // merker
                FENUM,              // enum
                FENUMITEM           // enumitem
              )$USINT;              // varaiablentypes


_RESULT_TYPE :( RES_DEFAULT,        // default
                RES_CONSTANT,       // constant value
                RES_NUMERIC,        // numeric value
                RES_REAL,           // real value
                RES_ANUMERIC,       // string, etc
                RES_TIME,           // time
                RES_DATE            // date
              )$UDINT;                   // result datatypes


_ANYLST      :( LOCTXT,        // local picturetext
                SOFTTXT,       // local softkeytext
                ICONLST,       // icon
                OBJLST,        // object
                MENULIST,      // menulist
                VARTXT1,       // variablentext 01
                VARTXT2,       // variablentext 02
                VARTXT3,       // variablentext 03
                VARTXT4,       // bubblehelptext
                VARNAME,       // name of variable
                IMAGESCHEME,   // imagescheme list
                VARUNIT,       // unittext via server
                TEXTSCHEME,    // link to textscheme
                OBJECTSCHEME,  // link to objectscheme
                FONTLST,       // font
                FUNCTIONLST,   // functionblocks
                CHKBITLST,     // checkbitlist
                DEFLST,
                VARTXT1_TOKEN, // variablentext 01 oder token von variablennamen
                VARTXT1_OBJNAME, // variablentext 01 oder objektname (ohne .server)
                VARLIMIT_HI,   // hilimit von variable
                VARLIMIT_LO,   // lolimit von variable
                PRJSCREEN,     // search screen in projectlist by name
                PRJWINDOW,     // search window in projectlist by name
                STATESCHEME,   // link to statescheme
                COLORSCHEME,   // link to colorscheme
                FONTSCHEME,    // link to fontscheme
                
                BAR_IO,        // io as bar (rape)
                BUTTON_IO,     // io as button (rape)

                ALARMLIST   :=  999,  // alarmlist
                TXTLIST_VTX := 1000,
                TXTLIST_STX,
                TXTLIST_002,
                TXTLIST_003,
                TXTLIST_004,
                TXTLIST_005,
                TXTLIST_006,
                TXTLIST_007,
                TXTLIST_008,
                TXTLIST_009,
                TXTLIST_010,TXTLIST_011,TXTLIST_012,TXTLIST_013,TXTLIST_014,TXTLIST_015,TXTLIST_016,TXTLIST_017,TXTLIST_018,TXTLIST_019,
                TXTLIST_020,TXTLIST_021,TXTLIST_022,TXTLIST_023,TXTLIST_024,TXTLIST_025,TXTLIST_026,TXTLIST_027,TXTLIST_028,TXTLIST_029,
                TXTLIST_030,TXTLIST_031,TXTLIST_032,TXTLIST_033,TXTLIST_034,TXTLIST_035,TXTLIST_036,TXTLIST_037,TXTLIST_038,TXTLIST_039,
                TXTLIST_040,TXTLIST_041,TXTLIST_042,TXTLIST_043,TXTLIST_044,TXTLIST_045,TXTLIST_046,TXTLIST_047,TXTLIST_048,TXTLIST_049,
                TXTLIST_050,TXTLIST_051,TXTLIST_052,TXTLIST_053,TXTLIST_054,TXTLIST_055,TXTLIST_056,TXTLIST_057,TXTLIST_058,TXTLIST_059,
                TXTLIST_060,TXTLIST_061,TXTLIST_062,TXTLIST_063,TXTLIST_064,TXTLIST_065,TXTLIST_066,TXTLIST_067,TXTLIST_068,TXTLIST_069,
                TXTLIST_070,TXTLIST_071,TXTLIST_072,TXTLIST_073,TXTLIST_074,TXTLIST_075,TXTLIST_076,TXTLIST_077,TXTLIST_078,TXTLIST_079,
                TXTLIST_080,TXTLIST_081,TXTLIST_082,TXTLIST_083,TXTLIST_084,TXTLIST_085,TXTLIST_086,TXTLIST_087,TXTLIST_088,TXTLIST_089,
                TXTLIST_090,TXTLIST_091,TXTLIST_092,TXTLIST_093,TXTLIST_094,TXTLIST_095,TXTLIST_096,TXTLIST_097,TXTLIST_098,TXTLIST_099,
                TXTLIST_100,TXTLIST_101,TXTLIST_102,TXTLIST_103,TXTLIST_104,TXTLIST_105,TXTLIST_106,TXTLIST_107,TXTLIST_108,TXTLIST_109,
                TXTLIST_110,TXTLIST_111,TXTLIST_112,TXTLIST_113,TXTLIST_114,TXTLIST_115,TXTLIST_116,TXTLIST_117,TXTLIST_118,TXTLIST_119
              )$UDINT;

// just to keep compatibility to older program-code
#define STX TXTLIST_STX
#define MTX TXTLIST_003
#define ATX TXTLIST_002
#define ZT1 TXTLIST_004
#define ZT2 TXTLIST_005
#define ZT3 TXTLIST_006
#define VTX TXTLIST_VTX

_IO_TYPE :    ( AS_DEFAULT,         // default default
                AS_NUMERIC,         // display io as numeric
                AS_ASCII,           // display io as string
                AS_CHKBOX,          // display box (bit)
                AS_BAR,             // display bar
                AS_TBO,             // text, bitmap or object
                AS_DATE,            // display date
                AS_TIME,            // display time
                AS_MENU,            // display menue
                AS_VARNAME,         // display name of variable
                AS_USER,            // nc
                AS_RADIO,           // radiobutton
                AS_NUMERIC_HIDE,    // value as stars
                AS_ASCII_HIDE,      // string as stars
                AS_THEURGE,
                AS_IPADDRESS,       // ipaddress
                AS_ENUMERATION,     // enumeration

                AS_TXTSCHEME,       // display io as text from text-scheme
                AS_INPUTBUTTON,     // cursorcontrolled touch-button (rape)
                AS_INPUTBUTTON_WITOUT_CURSOR, // inputbutton without flashing cursor
                AS_OBJECTINPUT      // io
              )$UDINT;                       // io displayformat

_BUTTON_TYPE :( _TKEY,              // nc
                _TSWITCH,           // nc
                _TRADIO             // nc
              )$UDINT;              // buttontype

_PIC_COMMAND :( CMD_NOP,            // nothing (0)
                CMD_START,          // start with first picture (0)
                CMD_NEWPICRET,      // open new (previous) picture (0)
                CMD_EXIT,           // exit window without backgroundrefresh (0)
                CMD_CLOSE,          // close picture or window (0)
                CMD_EDITNO,         // open editor with input[no] (2)

                CMD_REDRAW,         // redraw picture or window (0)
                CMD_OUTSOFTKEY,     // redraw softkey (0)

                CMD_NEWPICTURE,     // open new picture (1)
                CMD_NEWWINDOW,      // open new window (1)
                CMD_NEWSOFTKEY,     // change to defined floor and draw (1)
                CMD_NEWLANGUAGE,    // change to other language (1)
                CMD_EDIT,           // seek input and open editor (1)
                CMD_KILLEDITOR,     // kill editor without save (0)
                CMD_SAVEEDITOR,     // save editor (0)
                CMD_RETURN,         // return to previous (0)
                CMD_SCANCODE,       // set scancode for keyboard (1)
                CMD_CALL,           // call individual userfunction (2)
                CMD_BEEP,           // signal beep (1)
                CMD_ADD,            // add value to variable
                CMD_SUB,            // sub value from variable
                CMD_INC,            // value ++
                CMD_DEC,            // value --
                CMD_SET,            // set variable
                CMD_TOGGLE,         // 0-1-0
                CMD_IFE,            // if ==
                CMD_IFG,            // if >
                CMD_IFS,            // if <
                CMD_ELSE,           // else
                CMD_END,            // end
                CMD_EXITPROGRAM,    // exit application
                CMD_NEWPOPUP,       // open popupmenu
                CMD_CALL_ALARM,     // open alarmwindow
                CMD_OVERLOAD,       // overload (1)
                CMD_REDRAWTEXT,     // redraw text on screen (0)
                CMD_SCRNSCHEME,     // open screen via scheme
                CMD_DIAGNOSTIC,     // open diagnosticwindow (0)
                CMD_OVERLOAD_OBJ,   // overload just defined object (2)
                CMD_OVERLOAD_SUMMARY, // overload object with defined summary (2)
                CMD_LED,            // switch led on/off/blink (2)
                CMD_FUNCTIONBLOCK,  // call functionblock (1)
                CMD_EDITSERVER_HID, // call toucheditorwindow to edit spezified server (1)
                CMD_SWITCH_MULTICPU,// switch to substation 255[1] (1)

                CMD_SCREENSAVE,     // open screensaver (0)
                CMD_EDITNO_HID,     // open editor with input[no] and open HID-window (2)
                CMD_NEXTPICTURE,    // open next picture
                CMD_USERPOPUP,      // open userdefined popupmenu
                CMD_IOPOPUP,        // open popupmenu for input
                CMD_REGION,         // create all regions (0)
                CMD_FIRSTDRAW,      // first time draw picture (0)
                CMD_RETURN_TO_SIGN, // return to sign, previous (dumpling) (0)
                CMD_REEDIT,         // open editor for a second time
                CMD_OPENMINMAX,     // open minmaxwindow (bubble-help)
                CMD_CLOSEMINMAX,    // close minmaxwindow (bubble-help)
                CMD_RECALC_IOID,    // recalculate inputs in objectlist for editor (0)
                CMD_RETURNQUEUE,    // return and keep (copy) all commands
                CMD_AUTOSEEKINPUT,
                CMD_EDIT_HID,
                CMD_RECONFIGVAR,    // reconfig server + alarm and redraw screen (1)
                CMD_REFRESHSOFTKEY, // redraw softkey without option to change floor (0)
                CMD_ACCESSCHANGED,  // accesslevel has cahanged (0)
                CMD_NEWTOUCHEDITOR, // open new toucheditorwindow (2)
                CMD_LOCKHISTORY,    // lock/unlock historyfile (1)
                CMD_SERVERHELP,     // open server-helpwindow (1)
                CMD_IOPOPUP_HID,    // open popupmenu for input with hid
                CMD_ENTERPLUS,      // do direction on enter (1)
                CMD_FLIPSCREEN,     // flip screen (2, op1=dx, op2=(yo<<16)|yu)
                CMD_RETURNEDITOR,   // close just editorweindow (0)
                CMD_UNIQUEWINDOW,   // close all window and open new one (2, windownumber, softfloor)
                CMD_MESSAGEBOX,      // open confirmwindow (2, windownumber, messboxindex)
                CMD_EDITSERVER_HID_WIN_EDITOR, // same cmd than CMD_EDITSERVER_HID but op2 will include editorwindownumber
                CMD_ACKNOWLEDGEFLAG,
                CMD_RESET_SCREENSAVER, // wenn screensaver aktiv ist wird dieser wieder zurückgesetzt
                CMD_RETURNQUEUE_EX  // wird bei UC_MOVING_WINDOW benötigt (1)
              )$UDINT;              // commands

_RUN_STATE :  ( RUN_NOP,            // do nothing
                RUN_CLOSE,          // close window and refresh background
                RUN_EXIT            // close window without refresh
              )$UDINT;              // intern window-commands

_SYSERROR :   ( TEXT_DAYOFWEEK,
                EMTY_TEXT,
                REINITIALIZE_REFRESHLIST,  // refreshliste muss neu aufgebaut werden, als parameter wird stationsnummer übergeben
                STX_R03,
                STX_R04,
                CANT_INITIALIZE_GRAPHIX,
                CANT_LOAD_PROJECT,
                STARTPICTURE_NOT_PRESENT,
                CANT_LOAD_STARTPICTURE,
                PICTURE_NOT_PRESENT,
                CANT_LOAD_PICTURE,
                WINDOW_NOT_PRESENT,
                CANT_LOAD_WINDOW,
                CANT_LOAD_FONT,
                TOO_LESS_MEMORY,
                CANT_LOAD_GFX,
                NUMVALUE_TOO_BIG,   // numeric input too big
                NUMVALUE_TOO_SMALL, // numeric input too small
                INVALID_TIME,
                INVALID_DATE,
                CANT_WRITE_DATA,    // cant write data to local or plc
                CANT_LOAD_POPUP,
                NO_ACCESS,
                REALVALUE_TOO_BIG,         // numeric input too big
                REALVALUE_TOO_SMALL,       // numeric input too small
                STX_WRONG_DATE,            // date input error
                STX_WRONG_TIME,            // time input error
                STX_R27,
                STX_R28,
                STX_R29,
                STX_R30,
                STX_R31,
                STX_R32,
                STX_R33,
                STX_R34,
                STX_R35,
                STX_R36,
                STX_R37,
                STX_R38,
                STX_R39,
                STX_R40,
                STX_R41,
                STX_R42,
                STX_R43,
                STX_R44,
                STX_R45,
                STX_R46,
                STX_R47,
                STX_R48,
                STX_R49
              )$UDINT;              // order in system-textlist

_EVENTTYPE :  ( _EVENT_NONE,        // no event
                _EVENT_KEYPRESS,    // key pressed
                _EVENT_KEYRELEASE,  // key released
                _EVENT_HIDMOVE,     // mouse, touch, lightpen moved
                _EVENT_HIDPRESS,    // mouse, touch, lightpen pressed
                _EVENT_HIDRELEASE,  // mouse, touch, lightpen released
                _EVENT_USER1,       // userdefined
                _EVENT_USER2,       // userdefined
                _EVENT_SHUTTLEINC,  // shuttle right
                _EVENT_SHUTTLEDEC,  // shuttle left
                _EVENT_LASALOS,     // lasalos event
                
                _EVENT_TRIGGER,     // triggersignal "tipcombi"
                _EVENT_PREHIDPRESS, // erster hid-press
                _EVENT_ZOOM         // flick zoom
              )$UDINT;                           // eventtypes

_ENTER_PLUS : ( _EP_AUTO,
                _EP_LEFT,           // + left
                _EP_RIGHT,          // + right
                _EP_UP,             // + up
                _EP_DOWN,           // + down
                _EP_NONE,
                _EP_REMEMBER        // remember position for CMD_ENTERPLUS
              )$UDINT;                          // cursorcontrol after enter

_HID_BUTTON : ( _HID_LEFT,          // touch, left mousebutton
                _HID_RIGHT,         // right mousebutton
                _HID_MIDDLE         // middle mousebutton
              )$UDINT;                          // button

_POSRESULT :  ( P_NOP,              // nothing to do
                P_SCROLLUP,         // scroll list up and redraw bottom line
                P_SCROLLDOWN,       // scroll list down and redraw top line
                P_SCROLLLEFT,
                P_SCROLLRIGHT,
                P_REDRAW,           // redraw whole page
                P_CURSOR            // redraw actual position
              )$UDINT;

_FILE_STATE : ( LOAD,               // open, read only
                SAVE,               // open, write only (create new or overwrite existing)
                LOADSAVE,           // open read and write (create new or change existing)
                NOSTATE             // default
              )$UDINT;                          // file openstate

_FILE_ERROR : ( H_NOERROR,          // no error
                H_NEVEROPEN,        // handle never opend
                H_DEFINED,
                H_NOTOPEN,
                H_NOREAD,
                H_NOMEMORY,
                H_NOWRITE,
                H_WRONGLSE
              )$UDINT;                          // file errorstate

_ENTRYTYPE :  ( _PE_NONE,           // default id
                _PE_ALARMLIST,      // list-id alarmlist
                _PE_VARLIST,        // list-id varlist
                _PE_TEXTLIST,       // list-id textlist
                _PE_OBJECTLIST,     // list-id objectlist
                _PE_BMPLIST,        // list-id imagelist
                _PE_FUNCTIONLIST,   // list-id functionlist
                _PE_MENULIST,       // list-id menulist
                _PE_FONTLIST,       // list-id fontlist
                _PE_SEQUENCELIST,   // list-id sequencelist
                _PE_PICTURE,        // list-id picture
                _PE_WINDOW,         // list-id window
                _PE_COLORSCHEME,    // list-id colorsheme
                _PE_IMAGESCHEME     // list-id imagesheme
                _PE_UNITSCHEME,
                _PE_FONTSCHEME,
                _PE_SCRNSCHEME,
                _PE_STATESCHEME,
                _PE_TEXTSCHEME,
                _PE_OBJECTSCHEME, 
                _PE_SETUP,          // list-id setuplist
                _PE_PALETTE,        // list-id color palette
                _PE_HOTKEYLIST,     // list-id hotkey's
                _PE_LANGUAGE,       // list-id language.txt
                _PE_THEME,          // list-id theme
                _PE_SERVERSCHEME,   // list-id serverscheme
                _PE_GLOBALSCREEN    // list-id globalscreen
              )$UDINT;              // project-list-id

_EDITSTATE :  ( _EDITACTIVE,
                _EDITCHANGE,
                _SELECTGROUP,
                _EDITPASSIVE
              )$UDINT;

_BUBBLE :     ( BUBBLE_VIRGIN,
                BUBBLE_WATCH,
                BUBBLE_ONSCREEN,
                BUBBLE_FINISHED
              )$UDINT;

_LOGSTATE :   ( _LOGIN_INIT,
                _LOGIN_FALSE,
                _LOGIN_TRUE
              )$UDINT;

_URGETYPE :   ( _URGE_USERDEFINED,
                _URGE_BAR            := 32768,
                _URGE_INDICATOR      := 32769,
                _URGE_CLOCK          := 32770,
                _URGE_SCALE          := 32771,
                _URGE_LANGUAGESELECT := 32772,
                _URGE_SCREENCLEAN    := 32773,
                _URGE_OSZI           := 65534,
                _URGE_UNDEFINED      := 65535
              )$UDINT;
              

_TOOLSTATE :  ( TS_IDLE,
                TS_FINISH,
                TS_SAVE,
                TS_SAVESTART,
                TS_SAVERUN,
                TS_SAVEEND,
                TS_LOAD,
                TS_LOADSTART,
                TS_LOADVERSION,
                TS_LOADCRC,
                TS_LOADRUN,
                TS_LOADEND
              )$UDINT;

_TOOLERROR :  ( TE_NOERROR,
                TE_TOOLESSMEMORY,
                TE_CANTOPEN,
                TE_CANTSAVEENTRY,
                TE_NO_STATESCHEME,           // ungültiges statescheme
                TE_NO_ACCESS,                // kein access
                TE_NO_CHKBIT,                // ungültiges chkbit
                TE_ERROR_LIMITS,             // grenzwertverletzung
                TE_UNKNOWN_SERVER,           // unbekannter server
                TE_UNKNOWN_LINE_FORMAT,      // unbekannter fileeintrag
                TE_FILTER_DOESNT_FIT,        // filter passt nicht
                TE_INVALID_CRC,              // falsche crc
                TE_DISK_ERROR,               // medium full, ...
                TE_NO_TOOLCAT                // no toolcat file
              )$UDINT;


_Thread2Cmd : ( T2_NONE,
                T2_COPY_FILE,
                T2_COPY_DIRECTORY,
                T2_DELETE_FILE,
                T2_DELETE_DIRECTORY,
                T2_USERCALLMETHODE
              )$UDINT;

#pragma pack (pop)
#pragma pack (push, 1)

// DEFINED CONSTANTS ********************************************************
#define ID_OPERATOR       0x00 // es folgt ein _UBYTE (operator (+,-,/,*,|, etc))
#define ID_VARLABEL       0x01 // es folgt ein _VARLABEL (variable, konstante)
#define ID_BITNO          0x02 // es folgt ein _UWORD (kennbitnummer (#2))
#define ID_CHKBIT         0x03 // es folgt ein _UWORD (verweis in kennbitliste (#kennbit01))
#define ID_CONSTANT       0x04 // nur compiler intern (from here keep in order !!)
#define ID_NONE           0x05 // nur compiler intern (continue keep in order !!)
#define IOP_NOP           0x00 // nop
#define IOP_BRACKET_OPEN  0x01 // (
#define IOP_BRACKET_CLOSE 0x02 // )
#define IOP_A_PLUS        0x03 // arithmetic +
#define IOP_A_MINUS       0x04 // arithmetic -
#define IOP_A_MULTI       0x05 // arithmetic *
#define IOP_A_DIV         0x06 // arithmetic /
#define IOP_A_MOD         0x07 // arithmetic %
#define IOP_A_OR          0x08 // arithmetic |
#define IOP_A_AND         0x09 // arithmetic &
#define IOP_A_XOR         0x0A // arithmetic ^
#define IOP_A_NOT         0x0B // arithmetic ~
#define IOP_A_EQUAL       0x0C // arithmetic =
#define IOP_OR            0x0D // logic ||
#define IOP_AND           0x0E // logic &&
#define IOP_SMALLER       0x0F // logic <
#define IOP_SMALLEREQUAL  0x10 // logic <=
#define IOP_EQUAL         0x11 // logic ==
#define IOP_BIGGER        0x12 // logic >
#define IOP_BIGGEREQUAL   0x13 // logic >=
#define IOP_NOTEQUAL      0x14 // logic !=
#define IOP_NOT           0x15 // logic not
#define IOP_END           0x16 // commandline end ;

#define seSrcUnknown          0
#define seSrcLasalOS          1
#define seSrcUSB              2
#define seSrcShuttle          3

#define seActionUnknown       0
#define seActionDevAttached   1
#define seActionDevDetached   2
#define seActionInc           3
#define seActionDec           4

// SINGLE DATATYPES *********************************************************

#define _DEFFRAME               0
#define _SINGLEFRAME            1
#define _3DFRAME                2
#define _OVERLAPPEDFRAME        3
#define _EDGEFRAME              4
#define _DEFFRAME_NOCAREREGION  5
#define _THEMEFRAME             6

#define _DEFTHEME       0
#define T_INVERTTHEME   T_BLINK

#ifdef USER_BILLION
 #define TERMALARMPUFFER   200
#endif
#ifdef USER_KM
 #define TERMALARMPUFFER   150
#endif
#ifndef TERMALARMPUFFER
 #define TERMALARMPUFFER   50
#endif

#define BIT_REMANENT       1   // daten werden auf hd gesichert und beim starten wieder geladen
#define BIT_ONOFF          2   // bei alarm wird zeit gekommen und zeit gegangen eingetragen, ansonsten 2ter eintrag
#define BIT_USECOUNTER     4   // tritt ein alarm wiederholt auf wird nur counter beim ersteintrag erhöht, ansonsten neuer eintrag

_LSEFILE           : ARRAY [0..49] of USINT;  // sizeof(_File)  T_NUC.H
#ifdef UC_ACCESSNO_2BYTE
_ACCESS            : UINT;
#else
_ACCESS            : USINT;
#endif
_FRAMETYPE         : USINT;
_CRC32             : UDINT;

_SELECTOR          : USINT;
#define _SELECTOR_DEFAULT            0
#define _SELECTOR_ONLY_KBRD          1
#define _SELECTOR_ONLY_HID           2

#ifdef GRC_SET_PIXELCORRECTION
 #undef GRC_SET_PIXELCORRECTION
#endif
#define GRC_SET_PIXELCORRECTION     1  // include right bottom pixel
#ifdef GRC_RESET_PIXELCORRECTION
 #undef GRC_RESET_PIXELCORRECTION
#endif
#define GRC_RESET_PIXELCORRECTION   2  // exclude right bottom pixel
#ifdef GRC_SET_ORIGINCOLOR
 #undef GRC_SET_ORIGINCOLOR
#endif
#define GRC_SET_ORIGINCOLOR         4  // color: normal mode
#ifdef GRC_SET_NAVYCOLOR
 #undef GRC_SET_NAVYCOLOR
#endif
#define GRC_SET_NAVYCOLOR           5  // color: navy mode (red)
#ifdef GRC_SET_VARIATIONCOLOR
 #undef GRC_SET_VARIATIONCOLOR
#endif
#define GRC_SET_VARIATIONCOLOR      6  // color variation
#ifdef GRC_SCREENSHOT_START
 #undef GRC_SCREENSHOT_START
#endif
#define GRC_SCREENSHOT_START      10 
#ifdef GRC_SCREENSHOT_STOP
 #undef GRC_SCREENSHOT_STOP
#endif
#define GRC_SCREENSHOT_STOP      11

#ifdef UC_UNICODE
  _CHAR            : UINT;        // 1 character (ascii or unicode)
#else
  _CHAR            : SINT;         // 1 character (ascii or unicode)
#endif

_FVOID_PT_BOOL_BOOL   : ^void;
_FVOID_PT_BOOL        : ^void;
_FVOID_PT_PT_BOOL     : ^void;
_FVOID_PT_PT_PT       : ^void;
_FVOID_PT_PT          : ^void;
_FVOID_DWORD_DWORD    : ^void;
_FBOOL_PT             : ^void;
_FBOOL_DWORD          : ^void;
_FBOOL_DWORD_DWORD_PT : ^void;
_FBOOL_PT_PT_BOOL     : ^void;
_FBOOL_BOOL           : ^void;
_FVOID_VOID           : ^void;
_FUDWORD_PT_PT_PT     : ^void;
_FPT_VOID             : ^void;
_FBOOL_PT_PT_PT_BOOL  : ^void;
_FBOOL_PT_PT          : ^void;
_FDOIT_PT             : ^void;
_FDOIT_PT_PT          : ^void;
_FDOIT_PT_PT_PT_BOOL  : ^void;

_SORTFUNCT            : ^void; // bubblesort return: 0(a<=b), 1(a>b)
_QSORTFUNCT           : ^void; // quicksort  return: 0(a=b), 1(a>b), -1(a<b)

// EXTENDED DATATYPES ******************************************************

_Thread2CmdLine : STRUCT
  cmd : _Thread2Cmd;
  user1 : _DWORD;
  user2 : _UDWORD;
  op1 : array[0.._MAX_PATHLENGTH-1] of char;
  op2 : array[0.._MAX_PATHLENGTH-1] of char;
  pcallback : ^void;
  pthis : ^void;
END_STRUCT;

_MEM_ATTRIB : STRUCT
  no  : UINT;
  ptr : ^_ATTRIB;
END_STRUCT;

_MEM_COLOR : STRUCT
  no  : UINT;
  ptr : ^_COLOR;
END_STRUCT;

_MEM_FONT : STRUCT
  no  : UINT;
  ptr : ^_FONT;
END_STRUCT;

_MEM_BOOL : STRUCT
  no  : UINT;
  ptr : ^BOOL;
END_STRUCT;

_MAESYSTEMTIME : STRUCT
  wYear            : UINT;
  wMonth           : UINT;
  wDayOfWeek       : UINT;
  wDay             : UINT;
  wHour            : UINT;
  wMinute          : UINT;
  wSecond          : UINT;
  wMilliseconds    : UINT;
END_STRUCT;

_OSEVENT : STRUCT
  eventTime        : UINT;          // System time stamp
  eventType        : USINT;         // Event type flags
  eventSource      : USINT;         // Event source device ID
  eventChar        : INT;           // Keyboard Character code
  eventScan        : INT;           // Keyboard Scan code
  eventState       : INT;           // Keyboard State info
  eventButtons     : INT;           // Positional device buttons
  eventX           : INT;           // Positional device X
  eventY           : INT;           // Positional device Y
  eventUser        : ARRAY [0..1] of INT;    // User defined event data
  eventSourceID    : UINT;
END_STRUCT;

_ZOOM : STRUCT
  x                : INT;         // zoomfactor x (0 and 1000 == 1:1)
  y                : INT;         // zoomfactor y (0 and 1000 == 1:1)
END_STRUCT;

#define TYP_STANDARD 0              // standard ram (max 4 byte)
#define TYP_EXTENDED 1              // string, complex
_LASALID:STRUCT
  id               : UDINT;       // intern lasal id
  vtype            : USINT;        // intern vartype
END_STRUCT;

_TIME : STRUCT
  hour             : USINT;
  minute           : USINT;
  second           : USINT;
END_STRUCT;

_DATE : STRUCT
  day              : USINT;
  month            : USINT;
  year             : UINT;
  dayofweek        : USINT;        // so,mo,tu,we,th etc (0-6)
END_STRUCT;

_DATIM : STRUCT
  fdate            : _DATE;
  ftime            : _TIME;
END_STRUCT;

_LEXEM : STRUCT
  id               : USINT;
  op               : USINT;
  value            : DINT; 
END_STRUCT;

_COMPCODE:STRUCT
  size             : UINT;
  ptr              : ^USINT;
END_STRUCT;

#define DEFSEQNO  65535
_CHKBIT : STRUCT
  seqno            : UINT;        // sequenceno
  invert           : USINT;        // invert (0..false, 1..true)
  overload         : _COMPCODE;     // overloaded sequence
END_STRUCT;

_XCHKBIT : STRUCT
  seqno            : UINT;        // sequenceno
  invert           : USINT;        // invert (0..false, 1..true)
END_STRUCT;

_LSLCOMREGDAT : STRUCT
  id               : UDINT;
  channel          : UINT;
  varpos           : UINT;
  xtime            : UINT;
  varlistid        : UDINT;
END_STRUCT;

#define DEF_VAL       0 // default value
#define VAR_VAL       1 // variablenumber
#define CONST_VAL     2 // constant value
#define PARA_VAL      3 // format parameter like defined variable (incl. POINTER_VAL to data)
#define PARA_CONST    4 // parameter is constant and autoformat (incl. POINTER_VAL to data)
#define INDEX_VAL     5 // index into a constantbuffer
#define BOOL_VAL      6 // bool information
#define POINTER_VAL   7 // -->
#define REFINDEX_VAL  8 // adress is index to reflist
//#define SIMU_VAL      9 // simulation data
#define POINTER_CHAR  10 // pointer _CHAR 0 string
#define REAL_VAL      11 // real value
#define CRC_VAL       12 // variablencrc

_DEFVAL : USINT;

_VARSINGLE : STRUCT
  state            : _DEFVAL;
  value            : DINT;
END_STRUCT;

_XVARSINGLE : _VARSINGLE;

#define VL_MAXINDEX 2               // max no of index in variable
_VARLABEL : STRUCT
  no               : USINT;
  info             : ARRAY [0..VL_MAXINDEX] of _VARSINGLE;
END_STRUCT;

_MEM_VARLABEL : STRUCT
  no               : UINT;
  ptr              : ^_VARLABEL;
END_STRUCT;

_ANYTHING : STRUCT
  list             : _ANYLST;       // list id
  no               : UDINT;       // reference-no
END_STRUCT;

_MEM_ANYTHING : STRUCT
  no  : UINT;
  ptr : ^_ANYTHING;
END_STRUCT;

_ANYPARA : STRUCT
  anything         : _ANYTHING;
  memvarlabel      : _MEM_VARLABEL;
END_STRUCT; 

_MEM_ANYPARA : STRUCT
  no  : UINT;
  ptr : ^_ANYPARA;
END_STRUCT;

_XMEM_ANYPARA : STRUCT
  no  : USINT;
  ptr : ^_ANYPARA;
END_STRUCT;

_LINKVL : STRUCT
  info : USINT;
  link : _VARLABEL;
END_STRUCT;

_MEM_LINKVL : STRUCT
  no  : USINT;
  ptr : ^_LINKVL;
END_STRUCT;

_PRJ_ENTRY : STRUCT
  ftype            : _ENTRYTYPE;
  no               : UINT;
  nextno           : UINT;
  chkbit           : _CHKBIT;
  name             : ARRAY [0..12] of _ASCII;
  usedintern       : USINT;
  anypara          : _ANYPARA;      // titel-description
  access           : _ACCESS;       // accesslevel to screen, window
  imageno          : UINT;
  refcyc           : USINT;
 #ifdef XUSER_KM
  softinfo         : UINT;
 #endif
  time_max         : DINT;
  time_last        : DINT;
  label            : ARRAY [0..31] of _ASCII;
END_STRUCT;

_ASCII_BUFFER : STRUCT
  ptr              : ^_ASCII;     // pointer to text
  no               : UDINT;       // no of text
  size             : UDINT;       // no of characters
  space            : UDINT;       // no of reserved memory
END_STRUCT;

_FAST_ASCII_BUFFER : STRUCT
  ascii_buffer     : _ASCII_BUFFER;
  poffset          : ^UDINT;
END_STRUCT;

_MEM_NO : STRUCT
  no               : UDINT;
  ptr              : ^DINT;
END_STRUCT;

_NAME_INDEX : STRUCT
  name             : _ASCII_BUFFER;
  index            : _MEM_NO;
  crc32            : _UDWORD;
END_STRUCT;

_TEXT_BUFFER : STRUCT
  ptr              : ^_CHAR;      // pointer to text
  poff             : ^UDINT;      // pointer to textoffsettable
  no               : UDINT;       // no of text
  size             : UDINT;       // no of characters
END_STRUCT;

_LANGUAGE_INFO : STRUCT
  winid            : _MEM_NO;
  flag             : _MEM_NO;
  name             : _TEXT_BUFFER;
  winname          : _ASCII_BUFFER;
  no               : _UWORD;
END_STRUCT;

_PROJECT : STRUCT
  no               : UINT;
  ptr              : ^_PRJ_ENTRY;
  no_static        : UINT;
  prjdp            : ARRAY [0.._MAX_PATHLENGTH-1] of _ASCII;
  crc32dpne        : _CRC32;
  RevisionNumber   : UDINT;
  ProjectName      : ARRAY [0..127] of _CHAR;
END_STRUCT;

#define PARAMETER_TOOLCATALOGUE       0x00000001L
#define PARAMETER_PROTOCOL            0x00000002L
#define PARAMETER_PDE                 0x00000004L
#define PARAMETER_EXTENDED_BUBBLEHELP 0x40000000L
#define PARAMETER_ALARMBIT            0x80000000L

_STATISTIC : STRUCT
  group            : UDINT;
  parameter        : UDINT;
END_STRUCT;

_ROBY : STRUCT
  color            : _COLOR;
  length           : UINT;
  angle            : UINT;
END_STRUCT;

#define VL_UNDEFINED          0
#define VL_RAMPSETPOINT       1
#define VL_SETPOINT           2
#define VL_ACTVALUE           3
#define VL_COLORCHANGE        4
#define VL_STATEOVERLOAD      5
#define VL_ADJUSTMENTPARA     6
#define VL_FUNCTIONSWITCH     7
#define VL_REDUPPERLIMIT      8
#define VL_REDUPPERLIMITENBL  9
#define VL_XINDEX             10
#define VL_STRUCTUREOFFSET    16#FE

// Diese Daten werden nach dem laden gelöscht und werden mit varobj verodert
#define VT_GLOBAL      16#80 // Globale Variabel
#define VT_ELEMENT     16#40 // Strukturelement (bei Servern)

#ifdef UNIT_2Byte
  _VUNIT           : UINT;
  #define DEF_VUNIT         0xFFFF
#else
  _VUNIT           : USINT;
  #define DEF_VUNIT         0xFF
#endif

_VARINFO : STRUCT
  vartype          : _VARTYPE;
  classid          : UDINT;
  lo_limit         : _VARLABEL;
  hi_limit         : _VARLABEL;
  accessno         : UINT;
  format           : UINT;        // format of variable
  station          : USINT;        // stationnumber
  bitno            : USINT;        // Bitnummer
  funit            : _VUNIT;       // unit
  POWPRUIC         : USINT;        // pde, preceding zeros, toolcat,
  funitscheme      : UINT;
  reftime          : UINT;        // Refreshtime
  text1            : _ANYPARA;      // vartext 1
  text2            : _ANYPARA;      // vartext 2
  text3            : _ANYPARA;      // vartext 3
  text4            : _ANYPARA;      // vartext 4
  statistic        : _STATISTIC;
  stscheme         : UINT;
  uimenuindex      : UINT;        // menueindex
  xhkbit           : _XCHKBIT;
  userconfig       : UINT;        // Anwenderkonfiguration
 #ifdef UC_LINKVL
  linkvl           : ARRAY [0..UC_LINKVL-1] of _LINKVL;    // user link's
 #endif
  physic           : USINT;
  hostname         : UDINT;

  xadress          : _VARLABEL;    // adress of variable *
  lasalid          : _LASALID;     // lasalid
  crc32            : UDINT;
  varobj           : USINT;        // variable(0) - object(1)

  formula          : USINT;        // formula
  digits           : USINT;        // no. of digits
  posdezpt         : USINT;        // position decimalpoint
  parameter        : _VARLABEL;    // ifnec. parameter for formula
 #ifdef UC_EXOONLINE
  exoinfo          : UDINT;
 #endif
 #ifdef USER_DEMAG
  DemagDynPara     : BDINT;        // sa24193
 #endif
 
END_STRUCT;

_HILOLIM : STRUCT
  state            : USINT;
  lolim            : _IVALUE;
  hilim            : _IVALUE;
END_STRUCT;

_LSEVARINFO : STRUCT
  vartype          : _VARTYPE;
  hilolim          : _HILOLIM;
  format           : UINT;         // format of variable
  station          : USINT;        // stationnumber
  reftime_bitno    : USINT;        // Bitnummer
  funit            : _VUNIT;       // unit
  POWPRUIC         : USINT;        // pde, preceding zeros, toolcat,
  xmemtext         : _XMEM_ANYPARA; // vartext 1, 2, 3, 4
  statistic        : _STATISTIC;
  stscheme         : UINT;
  uimenuindex      : UINT;         // menueindex
  xxchkbit         : UINT;
  classid          : UINT;
  hostname         : UINT;
  funitscheme      : UINT;
  accessno         : UINT;
  userconfig       : UINT;         // Anwenderkonfiguration
  physic           : USINT;
 #ifdef UC_LINKVL
  memlinkvl        : _MEM_LINKVL;  // user link's
 #endif

  iaddress         : _UWORD;       // adress of variable *
  lasalid          : _LASALID;     // lasalid
  crc32            : UDINT;
 #ifdef UC_MULTICPU
  multiid          : array[0..UC_MULTICPU-1] of UDINT;
 #endif
 #ifdef UC_EXOONLINE
  exoinfo          : udint;
 #endif
 #ifdef UC_MEMORIZE_PLCDATA
  plc_value_ftype  : _RESULT_TYPE;
  plc_value        : dint;
 #endif
 #ifdef UC_MEMBERVARIABLE
  memberoffset     : dint;
 #endif 
 
END_STRUCT;

_SCAN : STRUCT
  max              : DINT;
  i                : DINT;
  room             : _ROOM;
  oldx             : _KOORD;
  color            : _COLOR;
  text             : ^char;
END_STRUCT;

_UNIT : STRUCT
  xdigits          : _VARLABEL;
  posdezpt         : _VARLABEL;
  formula          : USINT;
  parameter        : _VARLABEL;
  fixdp            : USINT;
  anypara          : _ANYPARA;
  leading0         : USINT;
END_STRUCT;

_MEM_UNIT : STRUCT
  no               : UINT;
  ptr              : ^_UNIT;
END_STRUCT;

_VARIABLE : STRUCT
  no               : UDINT;
  info             : _VARINFO;
END_STRUCT;

_RESULT : STRUCT
  ptr              : ^void;         // --> to data
  value            : DINT;       // numeric value
  rmin             : DINT;       // limit
  rmax             : DINT;       // limit
  datim            : _DATIM;        // date time
  ftype            : _RESULT_TYPE;  // type of result, date, numeric, string, etc
  recalculate_done : INT;        // calculation is already done for several times
  remotedata       : UINT;
  myiocrc          : UDINT;
END_STRUCT;

#define  DEF4BYTE                16#FFFFFFFF
#define  DEFSCOPE                16#FFFFFFFF
#define  DEFINDEX                0
#define  VARNO_EDITORDUMMY       16#FFFFFFFE
#define  VARNO_TIMEX_100         16#FFFFFFFD
#define  VARNO_ALTGRFLAG         16#FFFFFFFC
#define  VARNO_ALTGRFLAG_LOCK    16#FFFFFFFB
#define  VARNO_SHIFTFLAG         16#FFFFFFFA
#define  VARNO_INSERTFLAG        16#FFFFFFF9
#define  VARNO_HIRAGANA_KATAGANA 16#FFFFFFF8
#define  VARNO_JAPANESE_INPUT    16#FFFFFFF7

_VARSYMBOL : STRUCT
  scope            : UDINT;        // scope
//  index            : UDINT;       // indexwidth
END_STRUCT;

_MEM_VARSYMBOL : STRUCT
  no               : UDINT;
  ptr              : ^_VARSYMBOL;
END_STRUCT;

_MEM_LSEVARINFO : STRUCT
  no               : UDINT;
  ptr              : ^_LSEVARINFO;
END_STRUCT;

_RECTANGLE : STRUCT
  chkbit           : _CHKBIT;
  room             : _ROOM;
  attrib           : _ATTRIB;
  color            : _COLOR;
  colorframe       : _COLOR;
  frametype        : _FRAMETYPE;
END_STRUCT;

_CIRCLE : STRUCT
  chkbit           : _CHKBIT;
  room             : _ROOM;
  angle            : INT;
  arc              : INT;
  attrib           : _ATTRIB;
  color            : _COLOR;
  colorframe       : _COLOR;
END_STRUCT;

_POLYGON : STRUCT
  chkbit           : _CHKBIT;
  no               : UINT;
  ptr              : ^_DOT;
  attrib           : _ATTRIB;
  color            : _COLOR;
  colorframe       : _COLOR;
END_STRUCT;

#define SOP_SMALLER       0         // <
#define SOP_SMALLEQUAL    1         // <=
#define SOP_EQUAL         2         // ==
#define SOP_BIGGEREQUAL   3         // >=
#define SOP_BIGGER        4         // >
#define SOP_NOTEQUAL      5         // != 
_SINGLESCHEME : STRUCT
  upto             : _VARLABEL;
  op               : USINT;
  value            : UDINT;
END_STRUCT;

_MEM_SINGLESCHEME  : STRUCT
  no               : UINT;
  ptr              : ^_SINGLESCHEME;
  result_is_varno  : _BOOL;
END_STRUCT;

_SCHEME : STRUCT
  link             : _VARLABEL;
  memsinglescheme  : _MEM_SINGLESCHEME;
END_STRUCT;

_MEM_SCHEME : STRUCT
  no               : UINT;
  ptr              : ^_SCHEME;
END_STRUCT;

#define DEFAULTSCHEME 65535
_MYSCHEME : STRUCT
  schemeno         : UINT;
  overload         : _SCHEME;
END_STRUCT;

_LINKSCHEME : STRUCT
  myscheme         : _MYSCHEME;
  varlabel         : _VARLABEL;
END_STRUCT;

_MEM_LINKSCHEME : STRUCT
  no  : UINT;
  ptr : ^_LINKSCHEME;
END_STRUCT;

_DYNCOLOR : STRUCT
  linkscheme : _LINKSCHEME;
  color      : _COLOR;
END_STRUCT;

_MEM_DYNCOLOR : STRUCT
  no  : UINT;
  ptr : ^_DYNCOLOR;
END_STRUCT;

#define STATE_ACTIVE              0
#define STATE_INACTIVE            1
#define STATE_INVISIBLE           2
#define STATESCHEME_PRESELECTION  STATE_INVISIBLE

_USERTEXT : STRUCT
  ptr              : ^_CHAR;
END_STRUCT;

_PICTEXT : STRUCT
	chkbit           : _CHKBIT;
	room             : _ROOM;
	anypara          : _ANYPARA;
	font             : _FONT;
  frametype        : _FRAMETYPE;
	attrib           : _ATTRIB;
	colback          : _COLOR;        // color background (hatch)
	colframe         : _COLOR;        // color frame (top-left, bottom-right)
	coltxt           : _COLOR;        // color text
	summary          : UINT;
  stscheme         : _MYSCHEME;
  fillchr          : _UNI;
  doimage          : BOOL;
  lock_overload    : BOOL;
  
	image            : _IMAGE;
  oldcrc           : UDINT;
  onscreen         : BOOL;
  usertext         : _USERTEXT;
END_STRUCT;

_STACK_LINE : STRUCT
  cmd              : _PIC_COMMAND;
  op1              : _VARLABEL;
  op2              : _VARLABEL;
  lrm              : BOOL;
END_STRUCT;

_STACK : STRUCT
  no               : UINT;
  ptr              : ^_STACK_LINE;
END_STRUCT;

_LSEFUNCT : STRUCT
  no               : UINT;
  overload         : _STACK;
END_STRUCT;

_MEM_LSEFUNCT : STRUCT
  no               : UINT;
  ptr              : ^_LSEFUNCT;
END_STRUCT;

_GFUNCT : STRUCT
  pos_fl           : _LSEFUNCT;
  neg_fl           : _LSEFUNCT;
END_STRUCT;

_THEURGE : STRUCT
  urgetype    : _URGETYPE;
  info        : UDINT;
  memfont     : _MEM_FONT;
  memattrib   : _MEM_ATTRIB;
  memcolor    : _MEM_COLOR;
  memvarlabel : _MEM_VARLABEL;
  memanything : _MEM_ANYTHING;
  memdyncolor : _MEM_DYNCOLOR;
  memlsefunct : _MEM_LSEFUNCT;
  memanypara  : _MEM_ANYPARA;
  membool     : _MEM_BOOL;
  memimage    : _MEM_ANYTHING;  // backgroundimage

  ptr         : ^void;
END_STRUCT;

_SUBACTUAL : STRUCT
  act_output       : UINT;        // act. refresh no of output
  act_input        : UINT;        // act. refresh no of input
  act_motion       : UINT;        // act. refresh no of motion
  act_object       : UINT;        // act. refresh no of object
  act_colrect      : UINT;        // act. refresh no of colrect
  act_colpoly      : UINT;        // act. refresh no of colpoly
  act_colcirc      : UINT;        // act. refresh no of colcirc
  act_colpictxt    : UINT;        // act. refresh no of colpictxt
  act_button       : UINT;        // act. refresh no of button
  act_pictext      : UINT;        // act. refresh no of picturetext
//  cnt_priority     : UINT;        // refresh-priority counter
//  cnt              : UINT;        // refresh-counter (will never be reseted)
END_STRUCT;

_PICRUNMEMO : STRUCT
  last_editorno    : UINT;        // no of last io in editor
END_STRUCT;

_CSTATE : STRUCT
  init             : BOOL;
  value            : DINT;
END_STRUCT;

#ifdef UC_CHANGE_IO_GROUP
_IOGRPCHX : STRUCT
  serverno         : UDINT;
  summary          : UINT;
END_STRUCT;
#endif

_OBJECT : STRUCT
  chkbit           : _CHKBIT;
  xy               : _DOT;
  no               : UINT;
  attrib           : _ATTRIB;
  zoom             : _ZOOM;
  colorframe       : _COLOR;
  loader           : _VARLABEL;
  loader_scheme    : _MYSCHEME;
  stscheme         : _MYSCHEME;
  summary          : UINT;
  direction        : _DIRECTION;
  doimage          : BOOL;
  lock_overload    : BOOL;
  SubPicture       : ^void; //_SubPicture;  // -> to object
  workstate        : _WORKSTATE;
  image            : _IMAGE;
  onscreen         : BOOL;
 #ifdef UC_CHANGE_IO_GROUP
  imiogroupchanged : _IOGRPCHX;
 #endif
END_STRUCT;

_BITMAP : STRUCT
  chkbit           : _CHKBIT;
  room             : _ROOM;
  no               : UINT;
  attrib           : _ATTRIB;
  color            : _COLOR;
  colorframe       : _COLOR;
  
  userbmp          : ^_BMP;
END_STRUCT;
               
_CAROITEM : STRUCT
  objno            : UINT;
  loader           : _VARLABEL;
END_STRUCT;

_KARU : STRUCT
  objno            : UINT;
  loader           : _VARLABEL;
  SubPicture       : ^void;
  zoom             : _ZOOM;
  room             : _ROOM;
  room0            : _ROOM; // room bei offset 0
  onscreen         : BOOL; // gibt auskunft ob objekt am bildschirm platziert ist
  gesamt           : BOOL; // gibt auskunft ob ganzes oder nur halbes objekt am bildschirm platziert ist
END_STRUCT;

_MEM_KARU : STRUCT
  no               : UINT;
  ptr              : ^_KARU;
END_STRUCT;

_SCROLLBAR : STRUCT 
  room             : _ROOM;   // position
  col_back         : _COLOR;  // color
  col_front        : _COLOR;  // colorthumb
  col_frame        : _COLOR;  // color
  col_text         : _COLOR;  // color
  frametype        : _FRAMETYPE;  // frametype
  horizontal       : BOOL;    // horizontal true/false
  valid            : BOOL;    // gültig true/false
  
  width            : DINT;    // anzahl elemente
  pos              : DINT;    // position akt.element
  oldwidth         : DINT;    // referenz anzahl elemente
  oldpos           : DINT;    // referenz position akt.element
  
  active           : BOOL;
  active_dot       : _DOT;
END_STRUCT;

_MOTION : STRUCT
  chkbit           : _CHKBIT;
  room             : _ROOM;
  attrib           : _ATTRIB;
  anything         : _ANYTHING;
  varx             : _VARLABEL;
  vary             : _VARLABEL;
  loader           : _VARLABEL;
  direction        : _DIRECTION;
  stscheme         : _MYSCHEME;
  summary          : UINT;
  zoom             : _ZOOM;
  
  colback          : _COLOR;     // backgroundcolor
  dyncolback       : _LINKSCHEME; // dynamic color background 
  dyncolhatch      : _LINKSCHEME; // dynamic color background (hatch)
  memkaru          : _MEM_KARU;
  gap              : uint;        // abstand zwischen einzelnen objekten in pixel
  karuwidth        : dint; // gesamtbreite  oder gesamthöhe
  widthlastpage    : dint;
  infinite         : bool; // endloskarusell
  horizontal       : bool; // horizontale oder vertikale ausrichtung
  onscreen         : bool;
  movstate         : bool; // ist true zwischen press und release
  movbegin         : bool; // ist true zwischen press und move
  swim             : bool;
  positiv_dir      : bool; // true wenn nach rechts oder unten "gemoved" wurde
  usermove         : bool; // true wenn movement nicht vom kernel übernommen wird
  overmove         : bool; // carousell am anfang und ende weitermoven
  swimtime         : udint;
  image            : _IMAGE;
  SubPicture       : ^void;
  workstate        : _WORKSTATE;
  oldxy            : _DOT;
  move             : _DOT;
  movstart         : _DOT;
  skip_percent     : _UDWORD;
  maxmovno         : _UDWORD;    // anzahl der elemente welche "hinausgemoved" werden können. nur bei infinite=false ist dies ungleich memkaru.no
  movespeed        : _DWORD;    // geschwindigkeit (multiplikator / 10) mit welcher "gemoved" wird
 #ifdef UC_OLD_MOTIONS 
  width            : uint;
  height           : uint;
 #endif 
  varindex         : _VARLABEL;
  memoindex        : udint;
  frametype        : _FRAMETYPE;
  wiperoom         : _ROOM;
  scrbar           : _SCROLLBAR;
END_STRUCT;

_SINGLETXTSCHEME : STRUCT
  upto             : _VARLABEL;
  op               : USINT;
  anypara          : _ANYPARA;
END_STRUCT;


_TXTSCHEME : STRUCT
  no               : UINT;
  ptr              : ^_SINGLETXTSCHEME;
END_STRUCT;

_MEM_TXTSCHEME : STRUCT
  no               : UINT;
  ptr              : ^_TXTSCHEME;
END_STRUCT;

_MYTXTSCHEME : STRUCT
  schemeno         : UINT;
  overload         : _TXTSCHEME;
  anypara          : _ANYPARA;
END_STRUCT;

_IOINFO : STRUCT
  tbo              : _ANYTHING;     // text, bmp or obj number
  myscheme         : _MYSCHEME;
  mytxtscheme      : _MYTXTSCHEME;
  pcombo           : ^void;
  combocopy        : bool;
END_STRUCT;

#define BIT_REDUNDANT  1            // delete descendent zeros (format)
#define CALCUL_DIGITS  2            // nk = digits - actual_vorkommastellen (KM!)
_PICFORMAT : STRUCT
  format           : UINT;
  digits           : USINT;
  posdezpt         : _VARLABEL;
END_STRUCT;

_VIRTNAME : STRUCT
  no               : UINT;
  pthis            : ^void;
END_STRUCT;

_DIRECTION : STRUCT
  h_next           : UINT;
  v_next           : UINT;
END_STRUCT;

_IO : STRUCT
  chkbit           : _CHKBIT;       // checkbit
  attrib           :  _ATTRIB;      // attribute
  room             : _ROOM;         // given space by lse
  font             : _FONT;         // fontinformation
  frametype        : _FRAMETYPE;
  colback          : _COLOR;        // color background (hatch)
  colframe         : _COLOR;        // color frame (top-left, bottom-right)
  coltxt           : _COLOR;        // color text
  typ              : _IO_TYPE;      // displaytype
  variable         : _VARLABEL;     // link to data
  info             : _IOINFO;       // special information (depends on typ)
  picformat        : _PICFORMAT;    // overloaded format
  cstate           : _CSTATE;       // change state (old value)
  space            : _ROOM;         // calculated space (real space on screen)
  image            : _IMAGE;        // stored background
  imagecopy        : bool;  
  Obj              : ^void; //_SubPicture;  // pointer to an object (depends on typ)
  workstate        : _WORKSTATE;    // start, ready, run, end
  summary          : UINT;          // relevancy to other object
  virtname         : _VIRTNAME;     // information about virtual object name
  is_dyncolor      : BOOL;          // is any color dynamic ?
  dyncolback       : _LINKSCHEME;   // dynamic color background (hatch)
  dyncolfront      : _LINKSCHEME;   // dynamic color background (hatch)
  dyncoltl         : _LINKSCHEME;   // dynamic color frame (top-left)
  dyncolbr         : _LINKSCHEME;   // dynamic color frame (bottom-right)
  dyncoltxt        : _LINKSCHEME;   // dynamic color text
  stscheme         : _MYSCHEME;
  direction        : _DIRECTION;    // direction for seek-next
  doimage          : BOOL;
  lock_overload    : BOOL;
  newbubbletext    : _ANYPARA;
  
  lsefunct         : _LSEFUNCT;     // function for radio or chkbox when pressed
  fillchr          : _UNI;
  theurge          : ^_THEURGE;

  refcolback       : _COLOR;        // reference color background (hatch)
  refcolframe      : _COLOR;        // reference color frame (top-left, bottom-right)
  refcoltxt        : _COLOR;        // reference color text
  ioid             : UINT;          // id for struct _SEEKELEMENT
  pUser            : ^void;         // user pointer (THR)
  bRefreshAct      : BOOL;          // refresh flag (THR)
  imoverloaded     : _VARLABEL;     // i'm overloaded with ...
  priority         : USINT;         // refresh - priority
  xuser            : UDINT;         // 4 byte userdata
  editstate        : _EDITSTATE;    // _EDITACTIVE, _EDITCHANGE, _EDITPASSIVE
  onscreen         : _BOOL;
 #ifdef ZOOM_KM
  kmz              : _ZOOM;
 #endif
 #ifdef UC_CHANGE_IO_GROUP
  imiogroupchanged : _IOGRPCHX;
 #endif
 #ifdef UC_MENU_TWOCOLOR
  refstschemestate : _UDWORD;
 #endif
END_STRUCT;

_COMBOINFO : STRUCT
  lineheight       : UINT;
  frame            : _FRAMETYPE;
  linesperpage     : USINT;
END_STRUCT;

_INPUT : STRUCT
  io               : _IO;
  enter_plus       : _ENTER_PLUS;
  memseekno        : UINT;
  searchchoice     : USINT;
  selector         : _SELECTOR;
  comboinfo        : _COMBOINFO;
END_STRUCT;

_COL_RECT : STRUCT
  rectangle        : _RECTANGLE;
  dyncolback       : _LINKSCHEME;   // color background (hatch)
  dyncolfront      : _LINKSCHEME;   // color background (hatch)
  dyncoltl         : _LINKSCHEME;   // color frame (top-left)
  dyncolbr         : _LINKSCHEME;   // color frame (bottom-right)
  refcolor         : _COLOR;        // reference
  refcolorframe    : _COLOR;        // reference
  init             : BOOL;
END_STRUCT;

_COL_POLY : STRUCT
  poly:_POLYGON;
  dyncolback       : _LINKSCHEME;   // color background (hatch)
  dyncolfront      : _LINKSCHEME;   // color background (hatch)
  dyncolframe      : _LINKSCHEME;   // color frame
  refcolor         : _COLOR;
  refcolorframe	   :	_COLOR;
  init             : BOOL;
END_STRUCT;

_COL_CIRC : STRUCT
  circle           : _CIRCLE;
  dyncolback       : _LINKSCHEME;   // color background (hatch)
  dyncolfront      : _LINKSCHEME;   // color background (hatch)
  dyncolframe      : _LINKSCHEME;   // color frame
  refcolor         : _COLOR;
  refcolorframe    : _COLOR;
  init             : BOOL;
END_STRUCT;

_COL_PICTXT : STRUCT
  btx              : _PICTEXT;
  dyncolback       : _LINKSCHEME;   // color background (hatch)
  dyncolfront      : _LINKSCHEME;   // color background (hatch)
  dyncoltl         : _LINKSCHEME;   // color frame (top-left)
  dyncolbr         : _LINKSCHEME;   // color frame (bottom-right)
  dyncoltxt        : _LINKSCHEME;   // color text
  refcolback       : _COLOR;        // reference color background (hatch)
  refcolframe      : _COLOR;        // reference color frame (top-left, bottom-right)
  refcoltxt        : _COLOR;        // reference color text
  init             : BOOL;
END_STRUCT;

_EDITCOLOR : STRUCT
  colback          : _COLOR;        // color background (hatch)
  colframe         : _COLOR;        // color frame (top-left, bottom-right)
  coltxt           : _COLOR;        // color text
  dyncolback       : _LINKSCHEME;   // dynamic color background (hatch)
  dyncolfront      : _LINKSCHEME;   // dynamic color background (hatch)
  dyncoltl         : _LINKSCHEME;   // dynamic color frame (top-left)
  dyncolbr         : _LINKSCHEME;   // dynamic color frame (bottom-right)
  dyncoltxt        : _LINKSCHEME;   // dynamic color text
END_STRUCT;

_CURSORCOLOR : STRUCT
  color    : _COLOR;
  dyncolor : _LINKSCHEME;
END_STRUCT;

_BUBBLEINFO : STRUCT
  font             : _FONT;
  frametype        : _FRAMETYPE;
  attrib           : _ATTRIB;
  editcolor        : _EDITCOLOR;
  delaytime        : UDINT;
  remaintime       : UDINT;
END_STRUCT;

_MEM_POLYGON : STRUCT
  no               : UINT;
  ptr              : ^_POLYGON;
END_STRUCT;

_MEM_BITMAP : STRUCT
  no               : UINT;
  ptr              : ^_BITMAP;
END_STRUCT;

_MEM_RECTANGLE : STRUCT
  no               : UINT;
  ptr              : ^_RECTANGLE;
END_STRUCT;

_MEM_CIRCLE : STRUCT
  no               : UINT;
  ptr              : ^_CIRCLE;
END_STRUCT;

_MEM_PICTEXT : STRUCT
  no               : UINT;
  ptr              : ^_PICTEXT;
END_STRUCT;

_MEM_OBJECT : STRUCT
  no               : UINT;
  ptr              : ^_OBJECT;
END_STRUCT;

_MEM_INPUT : STRUCT
  no               : UINT;
  ptr              : ^_INPUT;
END_STRUCT;

_MEM_OUTPUT : STRUCT
  no               : UINT;
  ptr              : ^_IO;
END_STRUCT;

_MEM_MOTION : STRUCT
  no               : UINT;
  ptr              : ^_MOTION;
END_STRUCT;

_MEM_COL_RECT : STRUCT
  no               : UINT;
  ptr              : ^_COL_RECT;
END_STRUCT;

_MEM_COL_POLY : STRUCT
  no               : UINT;
  ptr              : ^_COL_POLY;
END_STRUCT;

_MEM_COL_CIRC : STRUCT
  no               : UINT;
  ptr              : ^_COL_CIRC;
END_STRUCT;

_MEM_COL_PICTXT : STRUCT
  no               : UINT;
  ptr              : ^_COL_PICTXT;
END_STRUCT;

_KEY : STRUCT
  gfunct           : _GFUNCT;
  tip              : _TIP;
  code             : UINT;
END_STRUCT;

_MEM_KEY : STRUCT
  no               : UINT;
  ptr              : ^_KEY;
END_STRUCT;

#ifdef UC_ABANDONED
_REPEAT : STRUCT
  ftime            : UDINT;
  timediff         : UDINT;
END_STRUCT;
#endif

_BUTTONIMAGES : STRUCT
  image_release    : UINT;        // released image
  image_press      : UINT;        // pressed image
  image_sel_release: UINT;        // selected+released image
  image_sel_press  : UINT;        // selected+pressed image
  image_in_release : UINT;        // inactive released image
  image_in_press   : UINT;        // inactive pressed image
  image_describe   : UINT;        // describing image
  zoom_images      : BOOL;        // zoom images to buttonsize
END_STRUCT;

_BUTTON : STRUCT
  chkbit           : _CHKBIT;
  room             : _ROOM;
  gfunct           : _GFUNCT;
  anything         : _ANYTHING;
  loader           : _VARLABEL;
  attrib           : _ATTRIB;
  framecol         : _COLOR;        // color of frame
  col              : _COLOR;        // (in-)active color
  ftype            : _BUTTON_TYPE;  // switch, key, radiobutton,...
  group            : UINT;          // radiobuttongroup
  tip              : _TIP;          // tipfunction
  summary          : UINT;
  direction        : _DIRECTION;
  enter_plus       : _ENTER_PLUS;
  access           : _ACCESS;
  frametype        : _FRAMETYPE;
  dyncolback       : _LINKSCHEME;   // dynamic color background (hatch)
  dyncolfront      : _LINKSCHEME;   // dynamic color background (hatch)
  dyncoltl         : _LINKSCHEME;   // dynamic color frame (top-left)
  dyncolbr         : _LINKSCHEME;   // dynamic color frame (bottom-right)
  stscheme         : _MYSCHEME;     // statescheme
  buttonimages     : _BUTTONIMAGES;
  varlabel         : _VARLABEL;     // server
  text             : _ANYPARA;      // released text
  text1            : _ANYPARA;      // pressed text
  font             : _FONT;
  coltxt           : _COLOR;
  dyncoltxt        : _LINKSCHEME;   // dynamic color txt
  hotkeycode       : UINT;          // scancode hotkey
  doimage          : BOOL;
  lock_overload    : BOOL;
  doubleclick      : BOOL;
  searchchoice     : USINT;         // KM Suchwahl für Cursorsteuerung
  wose             : USINT;         // aufhängepunkt für images
  selector         : _SELECTOR;     // default, only keyboard/touch
  act_momentary    : BOOL;
  set_val          : DINT;
  reset_val        : DINT;
  newbubbletext    : _ANYPARA;
  
  state            : BOOL;          // activated, inactivated
  oldstate         : BOOL;          // pressed, released
  initcombi        : BOOL;
  workstate        : _WORKSTATE;    // start, ready, run ...
  SubPicture       : ^void; //_SubPicture;  // -> to linked object
  selected         : BOOL;
  image            : _IMAGE;
  refcolor         : _COLOR;        // reference
  refcolorframe    : _COLOR;        // reference
  refcolortxt      : _COLOR;        // reference
  oldstatescheme   : UINT;
 #ifdef USER_DEMAG
  enable           : BOOL;
 #endif
 #ifdef UC_CHANGE_IO_GROUP
  imiogroupchanged : _IOGRPCHX;
 #endif
  reftxtcrc        : _CRC32;
 #ifdef NINETEEN
  full_access_given : BOOL;     // chkbit = true, statescheme = active, access = given
 #endif
  usertext         : _USERTEXT;
  usertext1        : _USERTEXT;
  keycode          : array[0..3] of uint;
  fingercount      : UDINT;
END_STRUCT;

_MEM_BUTTON : STRUCT
  no               : UINT;
  ptr              : ^_BUTTON;
END_STRUCT;

_DPNE : STRUCT
  no               : UINT;
  ptr              : ^_ASCII;
END_STRUCT;

#define ZO_NONE          0
#define ZO_RECTANGLE     1
#define ZO_TEXT          2
#define ZO_CIRCLE        3
#define ZO_POLYGON       4
#define ZO_IMAGE         5
#define ZO_GFX           6
#define ZO_OBJECT        7
#define ZO_INPUT         8
#define ZO_OUTPUT        9
#define ZO_MOTION       10
#define ZO_BUTTON       11
#define ZO_MOVIE        12
#define ZO_FREE         13          // nc
#define ZO_COLRECTANGLE 14
#define ZO_COLTEXT      15
#define ZO_COLCIRCLE    16
#define ZO_COLPOLYGON   17
#define ZO_CAROUSEL     18 // wird beim laden in ZO_MOTION umgewandelt

_ZORDER : STRUCT
  ftype            : USINT;
  no               : UINT;
END_STRUCT;

_MEM_ZORDER : STRUCT
  no               : UINT;
  ptr              : ^_ZORDER;
END_STRUCT;

_PICTUREMASK : STRUCT
  change           : BOOL;         // change-state
  room             : _ROOM;
  color            : _COLOR;
  memrectangle     : _MEM_RECTANGLE;
  mempolygon       : _MEM_POLYGON;
  memcircle        : _MEM_CIRCLE;
  mempictxt        : _MEM_PICTEXT;
  memobject        : _MEM_OBJECT;
  membitmap        : _MEM_BITMAP;
  meminput         : _MEM_INPUT;
  memoutput        : _MEM_OUTPUT;
  memmotion        : _MEM_MOTION;
  colrect          : _MEM_COL_RECT;
  colpoly          : _MEM_COL_POLY;
  colcirc          : _MEM_COL_CIRC;
  colpictxt        : _MEM_COL_PICTXT;
  memkey           : _MEM_KEY;
  membutton        : _MEM_BUTTON;
  memzorder        : _MEM_ZORDER;
  enclosed_name    : _FAST_ASCII_BUFFER;
END_STRUCT;

_STYLESHEET : STRUCT
  frametype          : _FRAMETYPE;
  fontscheme         : _FONT;
  contextalign       : _ATTRIB;
  colback            : _COLOR;
  colfront           : _COLOR;
  colhatch           : _COLOR;
  coltopleft         : _COLOR;
  colbottomright     : _COLOR;
  pdyncolback        : ^_LINKSCHEME;
  pdyncolfront       : ^_LINKSCHEME;
  pdyncolhatch       : ^_LINKSCHEME;
  pdyncoltopleft     : ^_LINKSCHEME;
  pdyncolbottomright : ^_LINKSCHEME;
END_STRUCT;

_WHOAMI : STRUCT
  ima                : _IMA;
  no                 : UDINT;
END_STRUCT;
 
_NEWSCREENPROP : STRUCT
  Position           : _ROOM;
  ColorBack          : _COLOR;
  ColorHatch         : _COLOR;
  ColorTopLeft       : _COLOR;
  ColorBottomRight   : _COLOR;
  FrameId            : _UDWORD;
  FirstInput         : _DOT;
  DrawShadow         : _BOOL;
  TouchEditorNumeric : _BOOL;
  TouchEditorAscii   : _BOOL;
  GlobalHotkeys      : _BOOL;
END_STRUCT;

_CHREDIT : STRUCT
  fstring          : ARRAY [0..299] of _CHAR;
  pos              : UINT;
  max              : UINT;
  no               : UINT;
  change           : BOOL;
  cursory          : UINT;
  ysize            : UINT;
  chr              : _CHAR;
  incdec           : BOOL;
  editshuttle      : BOOL;
  timer_cursorjump : UDINT;
//  japan_pos        : UINT;
//  japan_change     : UINT;
  japan_begin_pos  : UINT;
END_STRUCT;

_BUBBLEHELP : STRUCT
  room             : _ROOM;
  state            : _BUBBLE;
  font             : _FONT;
  attrib           : _ATTRIB;
  coltxt           : _COLOR;
  colframe         : _COLOR;
  colback          : _COLOR;
  frametype        : _FRAMETYPE;
  txt              : ^_CHAR;
END_STRUCT;

_MENCURSOR : STRUCT
  delay_timer      : UDINT;
  delay_active     : BOOL;
  active           : BOOL;
END_STRUCT;

_EDITOR : STRUCT
  actlayer         : UINT;        // actual index of subpicture
  actno            : UINT;        // actual no of input (editor)
  variable         : _VARIABLE;
  input            : _INPUT;
  result           : _RESULT;
  chredit          : _CHREDIT;      // just stupid lineeditor
  incdec           : DINT;
  timer            : UDINT;
  bubblehelp       : _BUBBLEHELP;
  mencursor        : _MENCURSOR;
  enterdirection   : BOOL;
  disable_direction_enter : BOOL;
  japan_input_mode : udint;
//  japan_count_ENTER: uint;
//  japan_count_SPACE: uint;
//  japan_pos_dictionary : uint;
//  japan_keyword    : array[0..31] of _CHAR;
END_STRUCT;

_PARA_PICKER : STRUCT
  nr_subpict       : UINT;        // actual index of subpicture
  actno            : UINT;        // act. no of output
END_STRUCT;

_ACTUAL : STRUCT
  actlayer         : UINT;        // actual subpicture
  soft_floor       : UINT;        // actual no of softkeyfloor
  language         : UINT;        // actual language
  actrefresh_cnt   : UINT;        // actual refreshcounter
END_STRUCT;

_SINGLESOFT        : STRUCT
  attrib           : _ATTRIB;       // Attribut
  color            : _COLOR;        // color button
  frame            : _COLOR;        // color frame
  text             : _COLOR;        // color text
  tip              : _TIP;          // Tipfunktion
  anypara          : _ANYPARA;      // Icon, Textgruppennummer
  font             : _FONT;
  chkbit           : _CHKBIT;       // chkbit
  access           : _ACCESS;       // key access
  gfunct           : _GFUNCT;       // functionality
  frametype        : _FRAMETYPE;
  buttonimages     : _BUTTONIMAGES;
  varlabel         : _VARLABEL;     // server
  valid            : USINT;         // is softkey valid
 #ifdef UC_DYN_SOFTKEYS
  stscheme         : _MYSCHEME;
  dyncolback       : _LINKSCHEME;
  dyncolfront      : _LINKSCHEME;
  dyncoltl         : _LINKSCHEME;
  dyncolbr         : _LINKSCHEME;
  dyncoltxt        : _LINKSCHEME;
 #endif
  wose             : USINT;         // aufhängepunkt für images
  act_momentary    : _BOOL;
  set_val          : _DWORD;
  reset_val        : _DWORD;
  newbubbletext    : _ANYPARA;

  cstate           : _CSTATE;
  state            : BOOL;         // pressed, released
  oldstate         : BOOL;         // pressed, released
END_STRUCT;

#define NO_SOFT_FLOOR  12           // no of softkeys/floor

_SOFTFLOOR : STRUCT
  floor            : ARRAY [0..NO_SOFT_FLOOR-1] of _SINGLESOFT;
END_STRUCT;

_POSITION : STRUCT
  begin            : DINT; // v: first line no on screen / h: first column no on screen
  pos              : DINT; // v: actual line position / h: actual column position
  no               : DINT; // v: number of lines / h: number of columns
  height           : UINT; // v: no of lines/page / h: number of columns/page (pagewidth)
  width            : UINT; // nc
  oldpos           : DINT;
  oldbegin         : DINT;
  lineheight       : ^USINT;
  totalheight      : UDINT;
END_STRUCT;

#define EVENT_LOCAL_KEYBOARD  1
#define EVENT_LOCAL_TOUCH     2
#define EVENT_LOCAL_USER      4
#define EVENT_LOCAL_SHUTTLE   8
#define EVENT_LOCAL_LASALOS   16

_EVENT : STRUCT
  ftype            : _EVENTTYPE;
  scancode         : UINT;
  modifier         : UINT;
  dot              : _DOT;
  button           : _HID_BUTTON;
  user             : ARRAY [0..1] of UINT;
  selfmade         : UINT;
  source           : UINT;
  state            : UINT;
  is_character     : BOOL;
  raw              : _DOT;
  lrm              : BOOL;   // event done by lasal remote commander
  timestamp        : UDINT;
  multindex        : UDINT;
  already_used     : UDINT;  // indicator if event is already in use
END_STRUCT;

_SYSTEM : STRUCT
  write                : BOOL;
  act_language         : _LANGUAGE;
  delaytime_screensave : UDINT;
  seek_algorithm       : BOOL;
  commasign            : BOOL;
  americandateformat   : BOOL;
  directiononenter     : BOOL;
  cursorwraphorizontal : BOOL;
  cursorwrapvertical   : BOOL;
  noexterncom          : BOOL;
  americantimeformat   : BOOL;
END_STRUCT;

_SINGLEALARM : STRUCT
 #ifdef SA34706
  no          : UDINT;
 #else
  no          : UINT;
 #endif
  state       : USINT;
  no_para     : USINT;
  para        : ARRAY [0..5] of DINT;
  time_on     : DINT;
  date_on     : DINT;
  time_off    : DINT;
  date_off    : DINT;

  acknowledge : USINT;
  cnt         : UINT;
  anything    : _ANYTHING;
 #ifdef UC_STATIC_ALARMSIZE
  ptr         : ARRAY [0..UC_STATIC_ALARMSIZE - 1] of USINT;
 #else
  ptr         : ^void;
  ptrsize     : UINT;
 #endif
  entrytype   : UINT;
  seqno       : UDINT;
  was_locked  : BOOL;
  alarmid     : _UDWORD;
 #ifdef UC_MULTICPU
  multicpuno  : USINT;
 #endif
END_STRUCT;

_RINGBUFFER : STRUCT
  no          : UDINT;
  begin       : UDINT;
  pos         : UDINT;
  used        : BOOL; 
END_STRUCT;

_MEM_SINGLEALARM : STRUCT
  filter        : UINT;
  ringbuffer    : _RINGBUFFER;
  xchange       : UINT;
  sortalgorithm : UINT;
  ptr           : ^_SINGLEALARM;
  noactive      : UDINT;
  pthis         : ^void;
  seqno         : _UDWORD;
  locked        : BOOL;
  para_filter   : UDINT;              // SA33569
  File          : _LSEFILE;           // muss letzte komponente sein !!
END_STRUCT;

_ALACELL : STRUCT
  index       : USINT;
  kennung     : USINT;
  no          : UINT;
  info        : UDINT;
END_STRUCT;

_PTRLINE           : ^void;
_PTRSTART          : ^void;
_PTRREADY          : ^void;
_PTRRUN            : ^void;
_PTREND            : ^void;

_COMDEF : STRUCT
  typ              : _INTERFACE;
  adress           : UINT;
  handle           : ^void;
  ipadress         : UDINT;
  port             : UDINT;
  free             : array[0..8] of udint;
END_STRUCT;

_LINEINFO : STRUCT
  chkbit           : _CHKBIT;
  value            : _VARLABEL;
  gfunct           : _GFUNCT;
  memvar           : _MEM_VARLABEL;
  anypara          : _ANYPARA;
  stscheme         : _MYSCHEME;
END_STRUCT;

_MEM_LINEINFO : STRUCT
  no               : UINT;
  ptr              : ^_LINEINFO;
END_STRUCT;

_LEVEL : STRUCT
  active           : BOOL;         // activ state
  room             : _ROOM;        // position (used space)
  color            : _COLOR;       // color
  frame_color      : _COLOR;       // color of frame
  old_pos          : _KOORD;       // sign (old position)
  trigger_state    : BOOL;
  drag             : BOOL;         // drag levelbutton
  pguppgdown       : BOOL;         // PgUp + PgDown + Up + Down Button
END_STRUCT;

_SCR_LINE          : ^void;
_SCR_BEAM          : ^void;
_SCR_DRAW          : ^void;
_SCR_START         : ^void;
_SCR_READY         : ^void;
_SCR_RUN           : ^void;
_SCR_END           : ^void;

_SCROLL : STRUCT
  out_room         : _ROOM;
  scroll_room      : _ROOM;
  font             : _FONT;
  attrib_inactiv   : _ATTRIB;
  attrib_activ     : _ATTRIB;
  color_inactiv    : _COLOR;
  color_activ      : _COLOR;
  frame_out        : _COLOR;
  frame_scroll     : _COLOR;
  line_width       : UINT;
  line_height      : UINT;
  position         : _POSITION;
  hposition        : _POSITION;
  level            : _LEVEL;
  hlevel           : _LEVEL;
  pt_line          : _SCR_LINE;     // --> to userdefined linedraw-interface
  pt_beam          : _SCR_BEAM;     // --> to userdefined beamdraw-interface
  pt_draw          : _SCR_DRAW;     // --> to userdefined backgrounddraw-interface
  pt_start         : _SCR_START;    // --> to userdefined init-interface
  pt_ready         : _SCR_READY;    // --> to userdefined ready-interface
  pt_run           : _SCR_RUN;      // --> to userdefined run-interface
  pt_end           : _SCR_END;      // --> to userdefined end-interface
  pt               : ^void;
  mencursor        : _MENCURSOR;
  Cursor           : ^void;
  noclrscr         : BOOL;
  cursor_dx1       : INT;
  cursor_dy1       : INT;
  cursor_dx2       : INT;
  cursor_dy2       : INT;
  frametype        : _FRAMETYPE;
  do_out_timer     : UDINT;
  image            : ^_BMP;
  flash_cursorcolor: _COLOR;
  nobck            : bool;
END_STRUCT;

_SINGLE_ALARM : STRUCT
  link             : _VARLABEL;
  priority         : UINT;
  chkbit           : _CHKBIT;
  group            : UINT;
  userconfig       : UINT;
  active           : _COLOR;
  passive          : _COLOR;
  quit_active      : _COLOR;
  quit_passive     : _COLOR;
  text             : _ANYPARA;
  helptext         : _ANYPARA;
  parameter        : _MEM_VARLABEL; // parametersatz
  alano            : UINT;          // alarm number
  mode             : UINT;          // userdefined mode
END_STRUCT;

_ALACONF : STRUCT
  opchg            : USINT;
  mode             : USINT;
  alano            : UINT;
END_STRUCT;

_CPUALARM : STRUCT
  lng              : UINT;
  alaconf          : _ALACONF;
  para             : ARRAY [0..0] of UDINT;
END_STRUCT;

_ALARM : STRUCT
  no               : UINT;
  ptr              : ^_SINGLE_ALARM;
END_STRUCT;

#ifdef OK_SUBLIST
_VKLIST : STRUCT
  hide             : UDINT;      // --> to hidden elements
  next             : UDINT;      // --> to next element
  jump             : UDINT;      // --> to next element with same depth
  no               : UDINT;      // real entry number
  depth            : UDINT;      // depth
  data             : UDINT;
  parent           : UDINT;      // parent (übergeordnetes element)
END_STRUCT;

_MEM_VKLIST : STRUCT
  ptr              : ^_VKLIST;   // --> to entries
  no               : UDINT;      // no of entries
  visual_no        : UDINT;      // no of open entries (visual entries)
  done_no          : UDINT;      // no of already allocated and initialized entries
END_STRUCT;
#endif

_SOFTKEYSETUP : STRUCT
  height           : UINT;       // softkey-height
  width            : UINT;       // softkey-width
  no               : UINT;       // no of softkeys/floor
  horizontal       : BOOL;       // direction (vertical/horizontal)
  align            : _ATTRIB;    // alignment
END_STRUCT;

// *************************************************************************
// *** COMMUNICATION *******************************************************
// *************************************************************************

_INDEX : STRUCT
  offset           : UDINT;       // --> to refreshed data
  lasalid          : _LASALID;      // lasalid
  ftime            : UINT;        // refreshtime for plc (unit ms)
  size             : UINT;        // size of data
  valid            : USINT;        // 0..invalid data, 1..valid data
END_STRUCT;

_INDEXDAT:STRUCT
  permanent        : UINT;        // permanent used _INDEX
  used             : UINT;        // used _INDEX incl. permanent
  no               : UINT;        // no of allocated _INDEX
  no_sent          : UINT;        // no of already sent entrys
  no_max           : UINT;        // no of entries in reflist
  data             : ^_INDEX;       // --> to _INDEX
END_STRUCT;

_REFDAT : STRUCT
  permanent        : UDINT;       // permanent used USINT
  used             : UDINT;       // used USINT incl. permanent
  size             : UDINT;       // no of allocated USINT
  data             : ^USINT;       // --> to USINT
END_STRUCT;

_REFLIST : STRUCT
  indexdat         : _INDEXDAT;
  refdat           : _REFDAT;
  login            : _LOGSTATE;
  permanent_enable : BOOL;
END_STRUCT;

#define DEFAULTCRC  0
_COMPORT : STRUCT
  comdef           : _COMDEF;
  reflist          : _REFLIST;
  crc              : UDINT;
  offline_cnt      : USINT;
  observe          : BOOL;
  offline          : BOOL;
  used             : BOOL;
#ifdef UC_NEW_ALARMSYSTEM
  alarmObject      : UDINT;
  oldValue         : UDINT;
  alarmHandle      : UDINT;
  alarmValidIndex  : UDINT;
  alarmAnzahl      : UDINT;
  alarmRecordSize  : UDINT;
  alarmXBufferSize : UDINT;
  alarmVersion     : UDINT;
  alarm_disable    : BOOL;
#endif
  add_reflist      : array[0..1] of UDINT;
  async_pingstate  : UDINT; // ASYNCPING_OFFLINE, ASYNCPING_BUSY, ASYNCPING_ONLINE
  plcstate         : DINT;
  async_onlstate   : UDINT;
  async_onltimex   : UDINT;
  loader_version   : UDINT;  
 #ifdef UC_GETLASALID_DATALINK
  lasalid_done     : UDINT;
  datalink_hdl     : ^void;
 #endif 
END_STRUCT;

_COMAP  : STRUCT
  aktmap           : UINT;
 #ifdef UC_MULTICPU
  multimap         : ARRAY [0..UC_MULTICPU-1] of UINT;
  used             : BOOL;
 #endif
END_STRUCT;

_COMSET : STRUCT
  no               : UINT;
  ptr              : ^_COMPORT;
  mapper           : ARRAY [0..255] of _COMAP;
END_STRUCT;

_IPCINI : STRUCT
  ComSet           : _COMSET;
  StartPicture     : _UWORD;
  LoginDelayTime   : _UDWORD;
  ProjectDPNE      : ARRAY [0.._MAX_PATHLENGTH-1] of _ASCII;
  StartGfxDPNE     : ARRAY [0.._MAX_PATHLENGTH-1] of _ASCII;
  SimulationDPNE   : ARRAY [0.._MAX_PATHLENGTH-1] of _ASCII;
  DataDPNE         : ARRAY [0.._MAX_PATHLENGTH-1] of _ASCII;
END_STRUCT;  

_SINGLESEEK : STRUCT
  location         : _LOCATION;
  no               : UINT;
  karuindex        : UINT;
  direction        : _DIRECTION;
END_STRUCT;

#ifndef MAX_SEEKELEMENT
 #define MAX_SEEKELEMENT 9
#endif
_SEEKELEMENT : STRUCT
  no               : UINT;
  ptr              : ARRAY [0..MAX_SEEKELEMENT-1] of _SINGLESEEK;
  ioid             : UINT;
  direction        : _DIRECTION;
  hotkeycode       : UINT; 
END_STRUCT;

_MEM_SEEKELEMENT : STRUCT
  no               : UINT;
  doneno           : UINT;
  ptr              : ^_SEEKELEMENT;
END_STRUCT;

_MY_PICTURE : STRUCT
  PScreenObject      : ^void;
  No                 : UINT;        // number of subpictures
  Ptr                : ^void; //_SubPicture;  // --> to subpicture
  memseekelement     : _MEM_SEEKELEMENT;
  Editor             : _EDITOR;
  Room               : _ROOM;
  Actual             : _ACTUAL;
  P_Window           : ^_Window;
  PicZoom            : _ZOOM;
  ParaPicker         : _PARA_PICKER;
  FirstInput         : _DOT;
  TouchEditorNumeric : BOOL;
  ToucheditorAscii   : BOOL;
  DrawShadow         : BOOL;
  StartFunct         : _LSEFUNCT;
  EndFunct           : _LSEFUNCT;
  GlobalHotkeys      : BOOL;
  JustProgram        : BOOL;
  RememberEnterPlus  : _UWORD;
END_STRUCT;

_MEM_POINTER : STRUCT
  no               : UDINT;
  ptr              : ^UDINT;
END_STRUCT;

_POINTERPARA : STRUCT
  ptr              : ^void;
  para             : UDINT;
END_STRUCT;

_MEM_POINTERPARA : STRUCT
  no               : UDINT;
  doneno           : UDINT;
  ptr              : ^_POINTERPARA;
END_STRUCT;

// *************************************************************************
// *** GLOBAL CLASSES ******************************************************
// *************************************************************************

_MEM_TextPuffer : STRUCT
  no               : UINT;
  ptr              : ^void;
END_STRUCT;

_PICTURE : STRUCT
  chkbit           : _CHKBIT;
  mask             : _PICTUREMASK;
  actual           : _SUBACTUAL;
  picrunmemo       : _PICRUNMEMO;
  TextPuffer       : _TEXT_BUFFER;
  MoveButtonGroup  : array[0..9] of udint;
END_STRUCT;

_MENUPOINTER : STRUCT
  PtrLine          : _PTRLINE;      // --> to userdefined linedraw function
  PtrStart         : _PTRSTART;     // --> to userdefined init-interface
  PtrReady         : _PTRREADY;     // --> to userdefined ready-interface
  PtrRun           : _PTRRUN;       // --> to userdefined run-interface
  PtrEnd           : _PTREND;       // --> to userdefined end-interface
  PtrData          : ^void;         // --> to userdefined data
  PtrSubList       : ^void;         // --> to sublist-data
  Ptr_LSEMENU      : ^void;         // --> to _LSEMENU
  Level            : bool;          // use vertical level
END_STRUCT;

_MENUINFO : STRUCT
  font             : _FONT;         // characterset
  attrib           : _ATTRIB;       // attribute
  xy               : _DOT;          // left-top position
  line_height      : UINT;        // line height in pixel
  width            : UINT;
  height           : UINT;
  text_color       : _COLOR;        // line color
  bar_color        : _COLOR;        // bar color
  frame_color      : _COLOR;        // frame color
  server           : _VARLABEL;
  frametype        : _FRAMETYPE;
  basement         : UINT;          // height of basement (freespace)
  inactive_text_color : _COLOR;     // inactive text color
  draw_shadow      : bool;
END_STRUCT;

_MENUCALCUL : STRUCT
  out_room         : _ROOM;         // space for whole popupmenu
  scroll_room      : _ROOM;         // space of scrollarea
  level            : _LEVEL;        // level indicator
  char_width       : UINT;        // character width
  char_height      : UINT;        // character height
END_STRUCT;

_MENU : STRUCT
  PScreenObject     : ^void;
  pointer           : _MENUPOINTER;
  position          : _POSITION;
  calcul            : _MENUCALCUL;
  info              : _MENUINFO;
  mencursor         : _MENCURSOR;
END_STRUCT;

_LSEMENU : STRUCT
  info              : _MENUINFO;
  mem_lineinfo      : _MEM_LINEINFO;
  Tbuffer           : _TEXT_BUFFER;
  lsemenuindex      : udint;
END_STRUCT;

#ifdef OK_PICMEMO
_LASTINPUT : STRUCT
  no               : UINT;
END_STRUCT;

_GLOBMEMO : STRUCT
  whoami           : _WHOAMI;
  lastinput        : _LASTINPUT;
END_STRUCT;

_MEM_GLOBMEMO : STRUCT
  no               : UINT;
  ptr              : ^_GLOBMEMO;
END_STRUCT;
#endif

#define NO_PICMEMO 20

#ifdef OK_VALIDATE
_VALIDATA : STRUCT
  vl               : _VARLABEL;
  ptr              : ^void;
END_STRUCT;

_MEM_VALIDATA : STRUCT
  no               : UINT;
  ptr              : ^_VALIDATA;
END_STRUCT;
#endif

_FONTINFO : STRUCT
  ptr              : ^_FONTPTR;
  width            : UINT;
  height           : UINT;
  size             : UDINT;
  isheap           : bool;
  attrib           : _ATTRIB;
END_STRUCT;

#ifdef UC_IPC
_GFXBMP : STRUCT
  dpne             : ARRAY [0.._MAX_PATHLENGTH-1] of _ASCII;
  bmp              : _BMP;      // --> to data
END_STRUCT;

_BMPHEAD : STRUCT
  label            : UINT;        // ('BM')
  fsize            : UDINT;       // filesize
  nc1              : UINT;        // (0)
  nc2              : UINT;        // (0)
  offs             : UDINT;       // offset of bitpattern (reference filestart)
END_STRUCT;

_BMPINFO : STRUCT
  size             : UDINT;       // length of bitpatterninfo
  width            : UDINT;       // Bitmapbreite in Pixel
  height           : UDINT;       // Bitmaphoehe in Pixel
  planes           : UINT;        // (1)
  bitcnt           : UINT;        // (4) Zahl der Bits pro Pixel
  compr            : UDINT;       // (0) Art der komprimierung
  sizeim           : UDINT;       // Bildgroesse in Bytes
  nc3              : UDINT;
  nc4              : UDINT;
  nc5              : UDINT;
  nc6              : UDINT;
END_STRUCT;

_RGB24:STRUCT
  cblue            : USINT;
  cgreen           : USINT;
  cred             : USINT;
END_STRUCT;

_RGB256 : STRUCT
  cred             : USINT;
  cgreen           : USINT;
  cblue            : USINT;
END_STRUCT;

_RGBBMP : STRUCT
cblue              : USINT;
cgreen             : USINT;
cred               : USINT;
cres               : USINT;
END_STRUCT;
#endif

#ifdef UC_DIRECTORY
_DIRLIST : STRUCT
  scroll           : _SCROLL;
  Label            : void;
END_STRUCT;

_DIRECTORYSETUP : STRUCT
  font             : _FONT;
  lineheight       : UINT;
  linewidth        : UINT;
  lineonpage       : UINT;
  offset           : _DOT;
  color_inactive   : _COLOR;
  color_active     : _COLOR;
  color_frame      : _COLOR;
END_STRUCT;

_DIRECTORY : STRUCT
  workstate        : _WORKSTATE;
  path             : ARRAY [0.._MAX_PATHLENGTH-1] of _ASCII;
  dpne             : ARRAY [0.._MAX_PATHLENGTH-1] of _ASCII;
  name             : ARRAY [0.._MAX_PATHLENGTH-1] of _ASCII;
  fkt              : _FBOOL_PT_PT;
  data             : ^void;
  pthis            : ^void;
  ready            : BOOL;
  activ            : UINT;
  oldactiv         : UINT;
  Files            : _DIRLIST;
  Direct           : _DIRLIST;
  chredit          : _CHREDIT;
  font             : _DIRECTORYSETUP;
  preelect         : ARRAY [0..99] of _ASCII;
END_STRUCT;
#endif

_SCREENSAVER : STRUCT
  actiontime : UDINT;
  image      : UINT;
  clrscr     : BOOL;
END_STRUCT;

_DEFKEYPAD   : STRUCT
  ButtonColorReleased    : _COLOR;
  ButtonColorPressed     : _COLOR;
  ButtonColorText        : _COLOR;
  TitleColorText         : _COLOR;
  LimitColorText         : _COLOR;
  WindowColorBack        : _COLOR;
  ButtonFrame            : _FRAMETYPE;
  WindowFrame            : _FRAMETYPE;
  StandardFontSchemeNo   : _UWORD;
  SmallFontSchemeNo      : _UWORD;
  StandardFontSchemeName : array[0..31] of _ASCII;
  SmallFontSchemeName    : array[0..31] of _ASCII;
  TitleBarColor          : _COLOR;
END_STRUCT;

LSLAPPHEAPINFO_TIPEX		 : STRUCT
  startaddr   : UDINT;
  totalsize   : UDINT;
  usedmem     : UDINT;
  freemem     : UDINT;
END_STRUCT;

HEAP_TYPE : STRUCT
  heap_mat    : ^void;
  applmeminfo : ^LSLAPPHEAPINFO_TIPEX;
  ChkHeapList : ^void;
END_STRUCT;

_LSEOSZI_CHANNEL : STRUCT
  vl         : _VARLABEL;
  YMin       : DINT;
  YMax       : DINT;
  color      : _COLOR;
  ringbuffer : _RINGBUFFER;
  data       : ^DINT;
END_STRUCT;

_MEM_LSEOSZI_CHANNEL : STRUCT
  no         : UINT;
  ptr        : ^_LSEOSZI_CHANNEL;
  ringbuffer : _RINGBUFFER;
  datim      : ^_DATIM;
END_STRUCT;

_LSEOSZI : STRUCT
  change    : UDINT;
  channels  : _MEM_LSEOSZI_CHANNEL;
END_STRUCT;

#define OSZICHANNELS 8

_MEM_INDEX : STRUCT
  no          : UDINT;
  used        : UDINT;
  ptr         : ^UDINT;
  bit         : ^USINT;
END_STRUCT;

#define NOCARE_STATESCHEME  0x0001
#define NOCARE_ACCESSLEVEL  0x0002
#define NOCARE_CHKBIT       0x0004
#define NOCARE_LIMITS       0x0008

#define NOCARE_DEFAULT      0x0000
#define NOCARE_ALL         (NOCARE_LIMITS or NOCARE_CHKBIT or NOCARE_ACCESSLEVEL or NOCARE_STATESCHEME)

_TOOLDATA : STRUCT
  Filter          : _STATISTIC;
  ActNo           : UDINT;
  ToolState       : _TOOLSTATE;
  ToolError       : _TOOLERROR;
  MemIndex        : _MEM_INDEX;
  Version         : UDINT;
  care_state      : UINT;
  dpne            : ARRAY[0.._MAX_PATHLENGTH-1] of _ASCII;
  Pthis           : ^void;
  CrcOffset       : UDINT;
  Crc             : UDINT;
  EndHeaderOffset : UDINT;
  ClassRevision   : UDINT; // SA37331
  CrcOk           : BOOL;
  File            : _LSEFILE;
END_STRUCT;

_HOTKEYLIST : STRUCT
  no   : UINT;
  code : ^UINT;
END_STRUCT;

_PICINDEX : STRUCT
  picno : UINT;
  no    : UINT;
  valid : UINT;
  ptr   : ^_PICINDEX;
END_STRUCT;

_DOT3D : STRUCT
  x : _KOORD;
  y : _KOORD;
  z : _KOORD;
END_STRUCT;

_SHAPEHEADER : STRUCT
    convex : BOOL;
	 pad    : DINT;
	 pbd    : DINT;
	 pcd    : DINT;
	 pdd    : DINT;
    xmax   : _KOORD;
    xmin   : _KOORD;
    ymax   : _KOORD;
    ymin   : _KOORD;
    zmax   : _KOORD;
    zmin   : _KOORD;
    ptr2d  : array[0..99] of _DOT;
END_STRUCT;

_SHAPE : STRUCT
  no        : UINT;
  ptr       : ^_DOT3D;
  color     : _COLOR;
  backcolor : _COLOR;
  header    : _SHAPEHEADER;
END_STRUCT;

_3DSHAPEHEADER : STRUCT
  xmax    : _KOORD;
  xmin    : _KOORD;
  ymax    : _KOORD;
  ymin    : _KOORD;
  zmax    : _KOORD;
  zmin    : _KOORD;
END_STRUCT;


_3DSHAPE : STRUCT
  no      : UINT;
  ptr     : ^_SHAPE;
  shading : BOOL;
  frame   : BOOL;
  header  : _3DSHAPEHEADER;
END_STRUCT;

#ifdef UC_GRAPHIC_STUDIO
_CIRCLE2D : STRUCT
  circle         : _CIRCLE;
  origin         : _ROOM;
END_STRUCT;

_POLYGON2D : STRUCT
  polygon        : _POLYGON;
  origin         : ^_DOT;
END_STRUCT;

_MEM_CIRCLE2D : STRUCT
  no             : UINT;
  ptr            : ^_CIRCLE2D;
END_STRUCT;

_MEM_POLYGON2D : STRUCT
  no             : UINT;
  ptr            : ^_POLYGON2D;
END_STRUCT;

_PICTUREMASK2D : STRUCT
  mem_circle2d   : _MEM_CIRCLE2D;
  mem_polygon2d  : _MEM_POLYGON2D;
  mem_zorder     : _MEM_ZORDER;
END_STRUCT;
#endif

_DATA : STRUCT
  no  : UDINT;
  ptr : ^void;
END_STRUCT;

_TREE : STRUCT
  no             : udint;
  done           : udint;
  ptr            : ^void;
  visuallines    : udint;
  markedlines    : udint;
END_STRUCT;

_FILEXPLORER : STRUCT
  xoffset        : int;
  FormatDate     : int;
  FormatTime     : int;
  ShowDate       : bool;
  ShowTime       : bool;
  ShowSize       : bool;
  ShowAttribute  : bool;
  ColorMarked    : _COLOR;
  ColorImage     : _COLOR;
  BoldMarked     : _BOOL;  
  FileFilter     : ^char;
  ImageDrive     : udint;
  ImageDirect    : udint;
  ImageFile      : udint;
  LockedRootSign : ^char;
  DriveList      : array[0..40] of usint;
  LasalOsCnt     : udint;               
  TreeSize       : _UWORD;
                 
  Tree           : _TREE;
  LockedRootPath : array[0.._MAX_PATHLENGTH-1] of _ASCII;
  ActPath        : array[0.._MAX_PATHLENGTH-1] of _ASCII;
  NoList         : udint;
  pList          : ^void;
  ExcludeDrives  : array[0.._MAX_PATHLENGTH-1] of _ASCII;
  SortCriteria   : udint;
  WidthName      : dint;
  WidthDate      : dint;
  WidthTime      : dint;
  WidthSize      : dint;
  ImageAttReadOnly : udint; // sa38678
  ImageAttHidden   : udint; // sa38678
  ImageAttArchiv   : udint; // sa38678
  ImageAttSystem   : udint; // sa38678
  
  
END_STRUCT;

#pragma pack (pop)

#endif

END_TYPE

// T_3D.CPP *****************************************************************
FUNCTION GLOBAL __CDECL matrize VAR_INPUT pm : ^_REAL; px : ^_REAL; py : ^_REAL; pz : ^_REAL; END_VAR;
FUNCTION GLOBAL __CDECL rotate_DOT3D VAR_INPUT p  : ^_DOT3D; w1 : INT; w2 : INT; w3 : INT; END_VAR;
FUNCTION GLOBAL __CDECL rotate_DOT VAR_INPUT p  : ^_DOT; w1 : INT; END_VAR;
#ifdef UC_GRAPHIC_STUDIO
FUNCTION GLOBAL __cdecl draw_3DSHAPE VAR_INPUT x0 : _KOORD; x1 : _KOORD; p2 : ^_3DSHAPE; END_VAR;
FUNCTION GLOBAL __cdecl polygon_to_3DSHAPE VAR_INPUT p0 : ^_3DSHAPE; p1 : ^_DOT; x2 : UINT; x3 : UINT; x4 : _COLOR; x5 : _COLOR; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl make_PICTUREMASK2D VAR_INPUT p0 : ^USINT; x1 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl out_PICTUREMASK2D VAR_INPUT p0 : ^USINT; p1 : ^_DOT; p2 : ^_ZOOM; END_VAR;
FUNCTION GLOBAL __cdecl rotate_PICTUREMASK2D VAR_INPUT p0 : ^USINT; x1 : INT; END_VAR;
FUNCTION GLOBAL __cdecl init_PICTUREMASK2D VAR_INPUT p0 : ^USINT; END_VAR;
FUNCTION GLOBAL __cdecl add_PICTUREMASK2D VAR_INPUT p0 : ^USINT; x1 : _KOORD; x2 : _KOORD; END_VAR;
FUNCTION GLOBAL __cdecl rotate_SHAPE VAR_INPUT p0 : ^_SHAPE; x0 : INT; x1 : INT; x2 : INT; END_VAR;
FUNCTION GLOBAL __cdecl rotate_3DSHAPE VAR_INPUT p0 : ^_3DSHAPE; x0 : INT; x1 : INT; x2 : INT; END_VAR;
#endif

// T_ALARM.CPP **************************************************************
FUNCTION GLOBAL __cdecl Lse_GetAlarmPointer VAR_OUTPUT retcode : ^_ALARM; END_VAR;
FUNCTION GLOBAL __cdecl store_ALACELL VAR_INPUT pt1 : ^_ALACELL; END_VAR;
FUNCTION GLOBAL __cdecl get_MEM_SINGLEALARM VAR_INPUT x0 : BOOL; END_VAR VAR_OUTPUT retcode : ^_MEM_SINGLEALARM; END_VAR;
FUNCTION GLOBAL __cdecl alarm_save VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl out_standard_alarmline VAR_INPUT p0 : ^_SINGLEALARM; p1 : ^_ROOM; x2 : _COLOR; filter : UINT; ps : ^_SCROLL := NIL; END_VAR;
FUNCTION GLOBAL __cdecl create_alarmtext VAR_INPUT p0 : ^_SINGLEALARM; x1 : BOOL; p2 : ^_COLOR; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;
FUNCTION GLOBAL __cdecl acknowledge_alarm VAR_INPUT p0 : ^_MEM_SINGLEALARM; x1 : DINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl delete_alarm VAR_INPUT p0 : ^_MEM_SINGLEALARM; x1 : DINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl add_SINGLEALARM VAR_INPUT p0 : ^_MEM_SINGLEALARM; p1 : ^_SINGLEALARM; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl active_MEM_SINGLEALARM VAR_INPUT p0 : ^_MEM_SINGLEALARM; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl CheckLostAlarm VAR_INPUT p0 : ^_ALACELL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl acknowledge_alarm_station VAR_INPUT x0 : UINT; x1 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl delete_alarm_station VAR_INPUT x0 : UINT; x1 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Lse_GetAlarmCrc VAR_OUTPUT retcode : udint; END_VAR; 
FUNCTION GLOBAL __cdecl Lse_GetAlarmconfig VAR_INPUT alaconfigno : UINT; END_VAR VAR_OUTPUT retcode : ^_SINGLE_ALARM; END_VAR;
FUNCTION GLOBAL __cdecl Lse_GetAlarmconfigIndex VAR_INPUT alaconfigno : UINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl UpdateAlarmHistoryRecord VAR_INPUT ptr:^_SINGLEALARM; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;

// T_BATCH.CPP **************************************************************
FUNCTION GLOBAL __cdecl Lse_GetComSet VAR_OUTPUT retcode : ^_COMSET; END_VAR;
FUNCTION GLOBAL __cdecl Get_COMPORT VAR_INPUT x0 : UINT; END_VAR VAR_OUTPUT retcode : ^_COMPORT; END_VAR;
FUNCTION GLOBAL __cdecl GetStartPicture VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl MakeOfflineText VAR_INPUT txt : ^_CHAR; oet : _BOOL; END_VAR;
FUNCTION GLOBAL __cdecl WatchOffline VAR_OUTPUT retcode : _BOOL; END_VAR;

// T_BMP.CPP ****************************************************************
FUNCTION GLOBAL __cdecl save_bmo VAR_INPUT p0 : ^_ASCII; x1 : _KOORD; x2 : _KOORD; x3 : _KOORD; x4 : _KOORD; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl load_bmo VAR_INPUT p0 : ^_BMP; p1 : ^_ASCII; p_errorcode : ^UINT:=NIL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl save_bmp VAR_INPUT p0 : ^_ASCII; x1 : _KOORD; x2 : _KOORD; x3 : _KOORD; x4 : _KOORD; x5 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl load_bmp VAR_INPUT p0 : ^_BMP; p1 : ^_ASCII; p_errorcode : ^UINT:=NIL; fp_callback : ^void:=NIL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
#define flash_bmo(p1, p2, p3)  flash_gfx(p1, p2, p3, NIL, NIL)
#define flash_bmp(p1, p2, p3)  flash_gfx(p1, p2, p3, NIL, NIL)
#define flash_gif(p1, p2, p3)  flash_gfx(p1, p2, p3, NIL, NIL)
#define flash_jpg(p1, p2, p3)  flash_gfx(p1, p2, p3, NIL, NIL)

// T_COM.CPP ****************************************************************
FUNCTION GLOBAL __cdecl send_command1 VAR_INPUT p0 : ^void; x1 : UINT; x2 : UINT; x3 : UINT; p4 : ^void; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl send_command2 VAR_INPUT p0 : ^void; x1 : UINT; x2 : UINT; x3 : UINT; p4 : ^void; x5 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl send_command3 VAR_INPUT p0 : ^void; x1 : UINT; x2 : UINT; x3 : UINT; p4 : ^void; p5 : ^void; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl get_dat VAR_INPUT p0 : ^_RESULT; x1 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl set_dat VAR_INPUT p0 : ^_RESULT; x1 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl login_PLC VAR_INPUT x0 : UINT; END_VAR VAR_OUTPUT retcode : ^_COMDEF; END_VAR;
FUNCTION GLOBAL __cdecl irq_PLC VAR_INPUT p0 : ^void; p1 : ^USINT; END_VAR ;
FUNCTION GLOBAL __cdecl set_offlinemode VAR_INPUT x0 : DINT; END_VAR ;
FUNCTION GLOBAL __cdecl xset_dat VAR_INPUT p0 : ^_RESULT; x1 : UDINT; x2 : UINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl xget_dat VAR_INPUT p0 : ^_RESULT; x1 : UDINT; x2 : UINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl new_PLC;
FUNCTION GLOBAL __cdecl is_online VAR_INPUT p0 : ^_COMDEF; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl is_new_online VAR_INPUT p0 : ^_COMPORT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl ChangeRemoteAddress VAR_INPUT x0 : UINT; x1 : _INTERFACE; x2 : UDINT; x3 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl SetTimeDelayOfflineWindow VAR_INPUT time_ms:UDINT; END_VAR;


// T_COMBOX.CPP *************************************************************
FUNCTION GLOBAL __CDECL Make_COMBOBOX VAR_INPUT colback : _COLOR; coltext : _COLOR; lineheight : udint; attrib : _ATTRIB; frame : _FRAMETYPE; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __CDECL Add_COMBOBOX VAR_INPUT pcb : ^void; value : dint; txt : ^void; chr_size : udint; END_VAR VAR_OUTPUT retcode : bool; END_VAR;

// T_COMPLR.CPP *************************************************************

// T_COPY.CPP ***************************************************************
FUNCTION GLOBAL __cdecl copy_SCHEME VAR_INPUT p0 : ^_SCHEME; p1 : ^_SCHEME; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_STACK VAR_INPUT p0 : ^_STACK; p1 : ^_STACK; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_BMP VAR_INPUT p0 : ^_BMP; p1 : ^_BMP; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_PICTURE VAR_INPUT p0 : ^_PICTURE; p1 : ^_PICTURE; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_MENUINFO VAR_INPUT p0 : ^_MENUINFO; p1 : ^_MENUINFO; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_MEM_VARLABEL VAR_INPUT p0 : ^_MEM_VARLABEL; p1 : ^_MEM_VARLABEL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_LINEINFO VAR_INPUT p0 : ^_LINEINFO; p1 : ^_LINEINFO; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_MEM_LINEINFO VAR_INPUT p0 : ^_MEM_LINEINFO; p1 : ^_MEM_LINEINFO; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_MEM_FONT VAR_INPUT p0 : ^_MEM_FONT; p1 : ^_MEM_FONT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_MEM_ATTRIB VAR_INPUT p0 : ^_MEM_ATTRIB; p1 : ^_MEM_ATTRIB; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_MEM_COLOR VAR_INPUT p10 : ^BOOL; p0 : ^_MEM_COLOR; p1 : ^_MEM_COLOR; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_MEM_ANYTHING VAR_INPUT p10 : ^BOOL; p0 : ^_MEM_ANYTHING; p1 : ^_MEM_ANYTHING; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_MEM_LINKSCHEME VAR_INPUT p10 : ^BOOL; p0 : ^_MEM_LINKSCHEME; p1 : ^_MEM_LINKSCHEME; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_DYNCOLOR VAR_INPUT p10 : ^BOOL; p0 : ^_DYNCOLOR; p1 : ^_DYNCOLOR; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_MEM_DYNCOLOR VAR_INPUT p10 : ^BOOL; p0 : ^_MEM_DYNCOLOR; p1 : ^_MEM_DYNCOLOR; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_LINKVL VAR_INPUT p0 : ^BOOL; p1 : ^_LINKVL; p2 : ^_LINKVL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl copy_INPUT VAR_INPUT p0 : ^BOOL; p1 : ^_INPUT; p2 : ^_INPUT; END_VAR;

// T_CRC.CPP ****************************************************************
FUNCTION GLOBAL __cdecl MakeCrc32 VAR_INPUT x0 : _CRC32; p1 : ^void; x3 : UDINT; END_VAR VAR_OUTPUT retcode : _CRC32; END_VAR;
FUNCTION GLOBAL __cdecl DeEnCode VAR_INPUT p0 : ^void; x1 : UDINT; x2 : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Random VAR_INPUT x0 : UDINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;

// T_CTEXT.CPP **************************************************************

// T_CURSOR.CPP *************************************************************
FUNCTION GLOBAL __cdecl Cursor_Get VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl Cursor_Hide VAR_INPUT p0 : ^void; END_VAR;
FUNCTION GLOBAL __cdecl Cursor_Set VAR_INPUT p0 : ^void; p1 : ^_ROOM; x2 : _COLOR; x3 : UINT; END_VAR;
FUNCTION GLOBAL __cdecl Cursor_End VAR_INPUT p0 : ^void; END_VAR;
FUNCTION GLOBAL __cdecl Cursor_Push VAR_INPUT p0 : ^void; END_VAR;
FUNCTION GLOBAL __cdecl Cursor_Pop VAR_INPUT p0 : ^void; END_VAR;

// T_DIAG.CPP ***************************************************************
FUNCTION GLOBAL __cdecl check_heap VAR_INPUT b1 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl draw_heap VAR_INPUT pr : ^_ROOM; END_VAR;

// T_DIRECT.CPP *************************************************************
#ifdef UC_DIRECTORY
FUNCTION GLOBAL __cdecl NewDirectorySetup VAR_INPUT x0 : _FILE_STATE; p1 : ^_ASCII; x2 : _FBOOL_PT_PT; p3 : ^void; p4 : ^_DIRECTORYSETUP; END_VAR ;
FUNCTION GLOBAL __cdecl DirectorySetup VAR_INPUT x0 : _FILE_STATE; p1 : ^_ASCII; x2 : _FBOOL_PT_PT; p3 : ^void; END_VAR ;
FUNCTION GLOBAL __cdecl DirectoryCall VAR_INPUT x0 : _WORKSTATE; p1 : ^_EVENT; x2 : BOOL; p3 : ^_ROOM; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
#endif
// T_EDIT.CPP ***************************************************************
FUNCTION GLOBAL __cdecl run_CHREDIT VAR_INPUT p0 : ^_CHREDIT; x1 : UINT; x2 : BOOL; x3 : BOOL; x4 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl ReOpenEditor VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl EditorOutChanged VAR_INPUT p0 : ^void; p1 : ^_EDITOR; END_VAR;
FUNCTION GLOBAL __cdecl IsEditorActive VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl incdec_time VAR_INPUT p1 : ^_TIME; val : DINT; format : DINT; END_VAR;
FUNCTION GLOBAL __cdecl incdec_date VAR_INPUT p1 : ^_DATE; val : DINT; END_VAR;
FUNCTION GLOBAL __cdecl set_MULTIKEYBOARD VAR_INPUT anzahl : UINT; keytab : ^UINT; delaytime : UINT; END_VAR;

// T_EXPLOR.CPP *************************************************************
function global __cdecl filexplo_scanmarked var_input pex : ^_FILEXPLORER; pfkt : ^void; puser1 : ^void; puser2 : ^void; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_line var_input pex : ^_FILEXPLORER; line : dint; pr : ^_ROOM; color : _COLOR; end_var;
function global __cdecl filexplo_enter var_input pex : ^_FILEXPLORER; ppos : ^_POSITION; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_path var_input pex : ^_FILEXPLORER; pselis : ^_SELIS; line : dint; rootlock : bool; end_var var_output retcode : ^char; end_var;
function global __cdecl filexplo_start var_input ppos : ^_POSITION; rootpath : ^char; exclude_drives : ^char; end_var var_output retcode : ^_FILEXPLORER; end_var;
function global __cdecl filexplo_end var_input pex : ^_FILEXPLORER; end_var var_output retcode : ^_FILEXPLORER; end_var;
function global __cdecl filexplo_update var_input pex : ^_FILEXPLORER; ppos : ^_POSITION; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_rename var_input pex : ^_FILEXPLORER; oldname : ^char; newnane : ^char; linepos : dint; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_getattrib var_input pex : ^_FILEXPLORER; pos : dint; end_var var_output retcode : udint; end_var;
function global __cdecl filexplo_delete var_input pex : ^_FILEXPLORER; ppos : ^_POSITION; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_delete_treeentry var_input pex : ^_FILEXPLORER; ppos : ^_POSITION; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_ctrlc var_input pex : ^_FILEXPLORER; line : dint; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_xctrlc var_input pex : ^_FILEXPLORER; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_is_marked var_input pex : ^_FILEXPLORER; line : dint; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_ctrlv var_input pex : ^_FILEXPLORER; dest : ^_ASCII; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_file_dir_copy_check var_input pex : ^_FILEXPLORER; dst : ^_ASCII; end_var var_output retcode : bool; end_var;
function global __cdecl CheckCreateDirectory var_input drivepath : ^_ASCII; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_can_open var_input pex : ^_FILEXPLORER; line : dint; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_can_close var_input pex : ^_FILEXPLORER; line : dint; end_var var_output retcode : bool; end_var;
function global __cdecl filexplo_make_user_path var_input dst : ^char; pex : ^_FILEXPLORER; src : ^char; end_var;
function global __cdecl filexplo_drivelist var_input pex : ^_FILEXPLORER; initstate : bool; end_var var_output retcode : dint; end_var;
function global __cdecl filexplo_updatetree var_input pex : ^_FILEXPLORER; ppos : ^_POSITION; end_var var_output retcode : ^_FILEXPLORER; end_var;
function global __cdecl filexplo_sort var_input pex : ^_FILEXPLORER; criteria : udint; end_var var_output retcode : bool; end_var;

// T_FILE.CPP ***************************************************************
FUNCTION GLOBAL __cdecl Lse_File_GetError VAR_INPUT p0 : ^_LSEFILE; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_SetError VAR_INPUT p0 : ^_LSEFILE; x1 : _FILE_ERROR; END_VAR ;
FUNCTION GLOBAL __cdecl Lse_File_Open VAR_INPUT p0 : ^_LSEFILE; x1 : _FILE_STATE; p2 : ^_ASCII; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_Close VAR_INPUT p0 : ^_LSEFILE; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_Tell VAR_INPUT p0 : ^_LSEFILE; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_Seek VAR_INPUT p0 : ^_LSEFILE; x1 : DINT; x2 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_Length VAR_INPUT p0 : ^_LSEFILE; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_Read VAR_INPUT p0 : ^_LSEFILE; p1 : ^void; x2 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_Write VAR_INPUT p0 : ^_LSEFILE; p1 : ^void; x2 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_Read_CRLF VAR_INPUT p0 : ^_LSEFILE; p1 : ^_ASCII; x2 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_ReadCrLf VAR_INPUT p0 : ^_LSEFILE; p1 : ^void; x2 : UINT; charsize : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_GetErrorState VAR_INPUT p0 : ^_LSEFILE; END_VAR VAR_OUTPUT retcode : _FILE_ERROR; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_Remove VAR_INPUT p0 : ^_LSEFILE; p1 : ^_ASCII; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_Decode VAR_INPUT p0 : ^_LSEFILE; headersize : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Lse_File_Encode VAR_INPUT p0 : ^_LSEFILE; headersize : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
#ifdef UC_IPC
FUNCTION GLOBAL __cdecl Write_BMP VAR_INPUT dpne : ^_ASCII; src : ^_BMP; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Read_BMP VAR_INPUT dpne : ^_ASCII; dst : ^_BMP; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
#endif
FUNCTION GLOBAL __cdecl Lse_File_Rename VAR_INPUT oldname : ^char; newname : ^char; END_VAR VAR_OUTPUT retcode : bool; END_VAR;

// T_FONT.CPP ***************************************************************
FUNCTION GLOBAL __cdecl font_set VAR_INPUT x0 : UINT; END_VAR VAR_OUTPUT retcode : _ATTRIB; END_VAR;
FUNCTION GLOBAL __cdecl font_set_by_language VAR_INPUT font : UINT; language : UINT; END_VAR VAR_OUTPUT retcode : _ATTRIB; END_VAR;
FUNCTION GLOBAL __cdecl font_size VAR_INPUT x0 : UINT; p1 : ^UINT; p2 : ^UINT; END_VAR ;
FUNCTION GLOBAL __cdecl font_seek VAR_INPUT p0 : ^_ASCII; END_VAR VAR_OUTPUT retcode : _FONT; END_VAR;
FUNCTION GLOBAL __cdecl font_set_direct VAR_INPUT x0 : _FONT; END_VAR VAR_OUTPUT retcode : _ATTRIB; END_VAR;
FUNCTION GLOBAL __cdecl font_size_direct VAR_INPUT x0 : _FONT; p1 : ^UINT; p2 : ^UINT; END_VAR;

// T_GFX.CPP ****************************************************************

FUNCTION GLOBAL __cdecl load_gfx VAR_INPUT p0 : ^_BMP; p1 : ^_ASCII; p_errorcode : ^UINT:=NIL; fp_callback : ^void:=NIL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl flash_gfx VAR_INPUT x0 : _KOORD; x1 : _KOORD; p2 : ^_ASCII; p_errorcode : ^UINT:=NIL; fp_callback : ^void:=NIL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl save_gfx VAR_INPUT p0 : ^_ASCII; x1 : _KOORD; x2 : _KOORD; x3 : _KOORD; x4 : _KOORD; x5 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl save_gfx_offscreen VAR_INPUT p0 : ^_ASCII; x1 : _KOORD; x2 : _KOORD; x3 : _KOORD; x4 : _KOORD; x5 : UDINT; p6 : ^_NEWSCREEN; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl save_gfx_BMP VAR_INPUT p0 : ^_ASCII; x1 : ^_BMP; x2 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl compress_BMP VAR_INPUT p0 : ^_BMP; END_VAR ;
FUNCTION GLOBAL __cdecl MakeIndexColor VAR_INPUT x0 : UINT; x1 : UINT; p2 : ^USINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl zoom_BMP VAR_INPUT p0 : ^_BMP; x1 : UINT; x2 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl raw_to_BMP VAR_INPUT p0 : ^_BMP; p1 : ^_UBYTE; x2 : _UWORD; x3 : _UWORD; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl transparent_BMP VAR_INPUT bmp : ^_BMP; cred : USINT; cgreen : USINT; cblue : USINT; END_VAR;
FUNCTION GLOBAL __cdecl invert_BMP VAR_INPUT bmp : ^_BMP; END_VAR;
FUNCTION GLOBAL __cdecl grab_bmp VAR_INPUT pd:^_BMP; x0:_KOORD; y0:_KOORD; x1:_KOORD; y1:_KOORD; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;


// T_GIF.CPP ****************************************************************
FUNCTION GLOBAL __cdecl load_gif VAR_INPUT p0 : ^_BMP; p1 : ^_ASCII; p_errorcode : ^UINT:=NIL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl save_gif VAR_INPUT p0 : ^_ASCII; x1 : _KOORD; x2 : _KOORD; x3 : _KOORD; x4 : _KOORD; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl save_gif_BMP VAR_INPUT p0 : ^_ASCII; p1 : ^_BMP; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;

// T_HELP.CPP ***************************************************************
FUNCTION GLOBAL __cdecl NewBubbleHelp_Call VAR_INPUT x : _KOORD; y : _KOORD; text : ^void; chrsize : uint; end_var var_output retcode : bool; end_var;

// T_HOTKEY.CPP *************************************************************

// T_ICON.CPP ***************************************************************
FUNCTION GLOBAL __cdecl ImageList_Get VAR_INPUT x0 : UINT; END_VAR VAR_OUTPUT retcode : ^_BMP; END_VAR;
FUNCTION GLOBAL __cdecl ImageList_Add VAR_INPUT dpne : ^_ASCII; label : ^_ASCII; invertcolor : bool := false; cred : uint := 16#FFFF; cgreen : uint := 16#FFFF; cblue : uint := 16#FFFF; END_VAR VAR_OUTPUT retcode : udint; END_VAR;
FUNCTION GLOBAL __cdecl ImageList_Add_BMO VAR_INPUT label : ^_ASCII; bmodata : ^void; END_VAR VAR_OUTPUT retcode : udint; END_VAR;

// T_INIT.CPP ***************************************************************
FUNCTION GLOBAL __cdecl MyGetScreenSize VAR_INPUT p0 : ^_ROOM; END_VAR ;
FUNCTION GLOBAL __cdecl MyClrScr VAR_INPUT c1 : _COLOR; END_VAR ;
FUNCTION GLOBAL __cdecl ScreenStart ;
FUNCTION GLOBAL __cdecl ScreenStop ;
FUNCTION GLOBAL __cdecl init_MYSCHEME VAR_INPUT p0 : ^_MYSCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl free_MYSCHEME VAR_INPUT p0 : ^_MYSCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl init_LINKSCHEME VAR_INPUT p0 : ^_LINKSCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl setup_LINKSCHEME VAR_INPUT p0 : ^_LINKSCHEME; colorschemelabel : ^_ASCII; varno : _UDWORD; END_VAR;
FUNCTION GLOBAL __cdecl free_LINKSCHEME VAR_INPUT p0 : ^_LINKSCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_SINGLESCHEME VAR_INPUT p0 : ^_MEM_SINGLESCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_SINGLESCHEME VAR_INPUT p0 : ^_MEM_SINGLESCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl init_SINGLESCHEME VAR_INPUT p0 : ^_SINGLESCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl free_SINGLESCHEME VAR_INPUT p0 : ^_SINGLESCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl setup_SINGLESCHEME VAR_INPUT p0 : ^_SINGLESCHEME; operat : usint; value : dint; index : udint; END_VAR;
FUNCTION GLOBAL __cdecl init_SCHEME VAR_INPUT p0 : ^_SCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl free_SCHEME VAR_INPUT p0 : ^_SCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_SCHEME VAR_INPUT p0 : ^_MEM_SCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_SCHEME VAR_INPUT p0 : ^_MEM_SCHEME; END_VAR ;
FUNCTION GLOBAL __cdecl init_ROOM VAR_INPUT p0 : ^_ROOM; END_VAR ;
FUNCTION GLOBAL __cdecl init_DOT VAR_INPUT p0 : ^_DOT; END_VAR ;
FUNCTION GLOBAL __cdecl init_TIME VAR_INPUT p0 : ^_TIME; END_VAR ;
FUNCTION GLOBAL __cdecl init_DATE VAR_INPUT p0 : ^_DATE; END_VAR ;
FUNCTION GLOBAL __cdecl init_DATIM VAR_INPUT p0 : ^_DATIM; END_VAR ;
FUNCTION GLOBAL __cdecl is_CHKBIT VAR_INPUT p0 : ^_CHKBIT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl is_MYSCHEME VAR_INPUT p0 : ^_MYSCHEME; p1 : ^_Scheme; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl chk_CHKBIT VAR_INPUT p0 : ^_CHKBIT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl chk_XCHKBIT VAR_INPUT p0 : ^_XCHKBIT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl init_WHOAMI VAR_INPUT p0 : ^_WHOAMI; END_VAR ;
FUNCTION GLOBAL __cdecl free_WHOAMI VAR_INPUT p0 : ^_WHOAMI; END_VAR ;
FUNCTION GLOBAL __cdecl init_NEWSCREENPROP VAR_INPUT p0 : ^_NEWSCREENPROP; END_VAR;
FUNCTION GLOBAL __cdecl free_NEWSCREENPROP VAR_INPUT p0 : ^_NEWSCREENPROP; END_VAR;
FUNCTION GLOBAL __cdecl init_CHKBIT VAR_INPUT p0 : ^_CHKBIT; END_VAR ;
FUNCTION GLOBAL __cdecl free_CHKBIT VAR_INPUT p0 : ^_CHKBIT; END_VAR ;
FUNCTION GLOBAL __cdecl init_XCHKBIT VAR_INPUT p0 : ^_XCHKBIT; END_VAR ;
FUNCTION GLOBAL __cdecl free_XCHKBIT VAR_INPUT p0 : ^_XCHKBIT; END_VAR ;
FUNCTION GLOBAL __cdecl init_ANYTHING VAR_INPUT p0 : ^_ANYTHING; END_VAR ;
FUNCTION GLOBAL __cdecl free_ANYTHING VAR_INPUT p0 : ^_ANYTHING; END_VAR ;
#ifdef UC_ABANDONED
FUNCTION GLOBAL __cdecl init_REPEAT VAR_INPUT p0 : ^_REPEAT; END_VAR ;
FUNCTION GLOBAL __cdecl free_REPEAT VAR_INPUT p0 : ^_REPEAT; END_VAR ;
#endif
FUNCTION GLOBAL __cdecl init_VIRTNAME VAR_INPUT p0 : ^_VIRTNAME; END_VAR ;
FUNCTION GLOBAL __cdecl free_VIRTNAME VAR_INPUT p0 : ^_VIRTNAME; END_VAR ;
FUNCTION GLOBAL __cdecl init_VARLABEL VAR_INPUT p0 : ^_VARLABEL; END_VAR ;
FUNCTION GLOBAL __cdecl free_VARLABEL VAR_INPUT p0 : ^_VARLABEL; END_VAR ;
FUNCTION GLOBAL __cdecl init_ZOOM VAR_INPUT p0 : ^_ZOOM; END_VAR ;
FUNCTION GLOBAL __cdecl free_ZOOM VAR_INPUT p0 : ^_ZOOM; END_VAR ;
FUNCTION GLOBAL __cdecl init_BMP VAR_INPUT p0 : ^_BMP; END_VAR ;
FUNCTION GLOBAL __cdecl free_BMP VAR_INPUT p0 : ^_BMP; END_VAR ;
FUNCTION GLOBAL __cdecl init_LEVEL VAR_INPUT p0 : ^_LEVEL; END_VAR ;
FUNCTION GLOBAL __cdecl free_LEVEL VAR_INPUT p0 : ^_LEVEL; END_VAR ;
FUNCTION GLOBAL __cdecl init_PICTEXT VAR_INPUT p0 : ^_PICTEXT; END_VAR ;
FUNCTION GLOBAL __cdecl free_PICTEXT VAR_INPUT p0 : ^_PICTEXT; END_VAR ;
FUNCTION GLOBAL __cdecl init_CIRCLE VAR_INPUT p0 : ^_CIRCLE; END_VAR ;
FUNCTION GLOBAL __cdecl free_CIRCLE VAR_INPUT p0 : ^_CIRCLE; END_VAR ;
FUNCTION GLOBAL __cdecl init_POLYGON VAR_INPUT p0 : ^_POLYGON; END_VAR ;
FUNCTION GLOBAL __cdecl free_POLYGON VAR_INPUT p0 : ^_POLYGON; END_VAR ;
FUNCTION GLOBAL __cdecl init_RECTANGLE VAR_INPUT p0 : ^_RECTANGLE; END_VAR ;
FUNCTION GLOBAL __cdecl free_RECTANGLE VAR_INPUT p0 : ^_RECTANGLE; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_POLYGON VAR_INPUT p0 : ^_MEM_POLYGON; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_POLYGON VAR_INPUT p0 : ^_MEM_POLYGON; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_RECTANGLE VAR_INPUT p0 : ^_MEM_RECTANGLE; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_RECTANGLE VAR_INPUT p0 : ^_MEM_RECTANGLE; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_CIRCLE VAR_INPUT p0 : ^_MEM_CIRCLE; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_CIRCLE VAR_INPUT p0 : ^_MEM_CIRCLE; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_PICTEXT VAR_INPUT p0 : ^_MEM_PICTEXT; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_PICTEXT VAR_INPUT p0 : ^_MEM_PICTEXT; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_OBJECT VAR_INPUT p0 : ^_MEM_OBJECT; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_OBJECT VAR_INPUT p0 : ^_MEM_OBJECT; END_VAR ;
FUNCTION GLOBAL __cdecl init_PICTURE VAR_INPUT p0 : ^_PICTURE; END_VAR ;
FUNCTION GLOBAL __cdecl free_PICTURE VAR_INPUT p0 : ^_PICTURE; END_VAR ;
FUNCTION GLOBAL __cdecl init_STYLESHEET VAR_INPUT p : ^_STYLESHEET; END_VAR;
FUNCTION GLOBAL __cdecl setup_STYLESHEET 
VAR_INPUT 
  p : ^_STYLESHEET; 
  fontschemename :^_ASCII; 
  frameid : _UDWORD; 
  contextalign : _ATTRIB; 
  colback : _COLOR; 
  colfront : _COLOR; 
  colhatch : _COLOR;
  coltopleft : _COLOR := DEFCOLOR;
  colbottomright : _COLOR := DEFCOLOR;
  pdyncolback : ^_LINKSCHEME := NIL; 
  pdyncolfront : ^_LINKSCHEME := NIL; 
  pdyncolhatch : ^_LINKSCHEME := NIL; 
  pdyncoltopleft : ^_LINKSCHEME := NIL; 
  pdyncolbottomright : ^_LINKSCHEME := NIL; 
END_VAR;
FUNCTION GLOBAL __cdecl free_STYLESHEET VAR_INPUT p : ^_STYLESHEET; END_VAR;
FUNCTION GLOBAL __cdecl init_PICTUREMASK VAR_INPUT p0 : ^_PICTUREMASK; END_VAR ;
FUNCTION GLOBAL __cdecl free_PICTUREMASK VAR_INPUT p0 : ^_PICTUREMASK; END_VAR ;
FUNCTION GLOBAL __cdecl init_REGION VAR_INPUT p0 : ^_REGION; END_VAR ;
FUNCTION GLOBAL __cdecl free_REGION VAR_INPUT p0 : ^_REGION; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_VARSYMBOL VAR_INPUT p0 : ^_MEM_VARSYMBOL; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_VARSYMBOL VAR_INPUT p0 : ^_MEM_VARSYMBOL; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_LSEVARINFO VAR_INPUT p0 : ^_MEM_LSEVARINFO; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_LSEVARINFO VAR_INPUT p0 : ^_MEM_LSEVARINFO; END_VAR ;
FUNCTION GLOBAL __cdecl init_VARINFO VAR_INPUT p0 : ^_VARINFO; END_VAR ;
FUNCTION GLOBAL __cdecl init_STATISTIC VAR_INPUT p0 : ^_STATISTIC; END_VAR ;
FUNCTION GLOBAL __cdecl free_STATISTIC VAR_INPUT p0 : ^_STATISTIC; END_VAR ;
FUNCTION GLOBAL __cdecl free_VARINFO VAR_INPUT p0 : ^_VARINFO; END_VAR ;
FUNCTION GLOBAL __cdecl init_VARSYMBOL VAR_INPUT p0 : ^_VARSYMBOL; END_VAR ;
FUNCTION GLOBAL __cdecl free_VARSYMBOL VAR_INPUT p0 : ^_VARSYMBOL; END_VAR ;
FUNCTION GLOBAL __cdecl init_VARIABLE VAR_INPUT p0 : ^_VARIABLE; END_VAR ;
FUNCTION GLOBAL __cdecl free_VARIABLE VAR_INPUT p0 : ^_VARIABLE; END_VAR ;
FUNCTION GLOBAL __cdecl init_SCROLL VAR_INPUT p0 : ^_SCROLL; END_VAR ;
FUNCTION GLOBAL __cdecl free_SCROLL VAR_INPUT p0 : ^_SCROLL; END_VAR ;
FUNCTION GLOBAL __cdecl init_EVENT VAR_INPUT p0 : ^_EVENT; END_VAR ;
FUNCTION GLOBAL __cdecl free_EVENT VAR_INPUT p0 : ^_EVENT; END_VAR ;
FUNCTION GLOBAL __cdecl init_CSTATE VAR_INPUT p0 : ^_CSTATE; END_VAR ;
FUNCTION GLOBAL __cdecl free_CSTATE VAR_INPUT p0 : ^_CSTATE; END_VAR ;
FUNCTION GLOBAL __cdecl init_IOINFO VAR_INPUT p0 : ^_IOINFO; END_VAR ;
FUNCTION GLOBAL __cdecl free_IOINFO VAR_INPUT p0 : ^_IOINFO; END_VAR ;
FUNCTION GLOBAL __cdecl init_EDITOR VAR_INPUT p0 : ^_EDITOR; END_VAR ;
FUNCTION GLOBAL __cdecl free_EDITOR VAR_INPUT p0 : ^_EDITOR; END_VAR ;
FUNCTION GLOBAL __cdecl init_RESULT VAR_INPUT p0 : ^_RESULT; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_INPUT VAR_INPUT p0 : ^_MEM_INPUT; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_INPUT VAR_INPUT p0 : ^_MEM_INPUT; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_OUTPUT VAR_INPUT p0 : ^_MEM_OUTPUT; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_OUTPUT VAR_INPUT p0 : ^_MEM_OUTPUT; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_MOTION VAR_INPUT p0 : ^_MEM_MOTION; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_MOTION VAR_INPUT p0 : ^_MEM_MOTION; END_VAR ;
FUNCTION GLOBAL __cdecl init_BITMAP VAR_INPUT p0 : ^_BITMAP; END_VAR ;
FUNCTION GLOBAL __cdecl free_BITMAP VAR_INPUT p0 : ^_BITMAP; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_BITMAP VAR_INPUT p0 : ^_MEM_BITMAP; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_BITMAP VAR_INPUT p0 : ^_MEM_BITMAP; END_VAR ;
FUNCTION GLOBAL __cdecl init_INPUT VAR_INPUT p0 : ^_INPUT; END_VAR ;
FUNCTION GLOBAL __cdecl free_INPUT VAR_INPUT p0 : ^_INPUT; END_VAR ;
FUNCTION GLOBAL __cdecl init_IO VAR_INPUT p0 : ^_IO; END_VAR ;
FUNCTION GLOBAL __cdecl free_IO VAR_INPUT p0 : ^_IO; END_VAR ;
FUNCTION GLOBAL __cdecl free_MOTION VAR_INPUT p0 : ^_MOTION; END_VAR ;
FUNCTION GLOBAL __cdecl init_MOTION VAR_INPUT p0 : ^_MOTION; END_VAR ;
FUNCTION GLOBAL __cdecl init_CAROITEM VAR_INPUT p : ^_CAROITEM; END_VAR;
FUNCTION GLOBAL __cdecl init_KARU VAR_INPUT p : ^_KARU; END_VAR;
FUNCTION GLOBAL __cdecl free_KARU VAR_INPUT p : ^_KARU; END_VAR;
FUNCTION GLOBAL __cdecl init_MEM_KARU VAR_INPUT p : ^_MEM_KARU; END_VAR; 
FUNCTION GLOBAL __cdecl free_MEM_KARU VAR_INPUT p : ^_MEM_KARU; END_VAR;
FUNCTION GLOBAL __cdecl init_SINGLE_ALARM VAR_INPUT p0 : ^_SINGLE_ALARM; END_VAR ;
FUNCTION GLOBAL __cdecl free_SINGLE_ALARM VAR_INPUT p0 : ^_SINGLE_ALARM; END_VAR ;
FUNCTION GLOBAL __cdecl init_ALARM VAR_INPUT p0 : ^_ALARM; END_VAR ;
FUNCTION GLOBAL __cdecl free_ALARM VAR_INPUT p0 : ^_ALARM; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_VARLABEL VAR_INPUT p0 : ^_MEM_VARLABEL; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_VARLABEL VAR_INPUT p0 : ^_MEM_VARLABEL; END_VAR ;
FUNCTION GLOBAL __cdecl init_BUTTON VAR_INPUT p0 : ^_BUTTON; END_VAR ;
FUNCTION GLOBAL __cdecl free_BUTTON VAR_INPUT p0 : ^_BUTTON; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_BUTTON VAR_INPUT p0 : ^_MEM_BUTTON; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_BUTTON VAR_INPUT p0 : ^_MEM_BUTTON; END_VAR ;
FUNCTION GLOBAL __cdecl free_OBJECT VAR_INPUT p0 : ^_OBJECT; END_VAR ;
FUNCTION GLOBAL __cdecl init_OBJECT VAR_INPUT p0 : ^_OBJECT; END_VAR ;
FUNCTION GLOBAL __cdecl free_GFUNCT VAR_INPUT p0 : ^_GFUNCT; END_VAR ;
FUNCTION GLOBAL __cdecl init_GFUNCT VAR_INPUT p0 : ^_GFUNCT; END_VAR ;
FUNCTION GLOBAL __cdecl free_LSEFUNCT VAR_INPUT p0 : ^_LSEFUNCT; END_VAR ;
FUNCTION GLOBAL __cdecl init_LSEFUNCT VAR_INPUT p0 : ^_LSEFUNCT; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_NO VAR_INPUT p0 : ^_MEM_NO; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_NO VAR_INPUT p0 : ^_MEM_NO; END_VAR ;
FUNCTION GLOBAL __cdecl init_COMSET VAR_INPUT p0 : ^_COMSET; END_VAR ;
FUNCTION GLOBAL __cdecl free_COMSET VAR_INPUT p0 : ^_COMSET; END_VAR ;
FUNCTION GLOBAL __cdecl init_NEWSCREEN VAR_INPUT p0 : ^_NEWSCREEN; END_VAR ;
FUNCTION GLOBAL __cdecl free_NEWSCREEN VAR_INPUT p0 : ^_NEWSCREEN; END_VAR ;
FUNCTION GLOBAL __cdecl init_SCAN VAR_INPUT p0 : ^_SCAN; END_VAR ;
FUNCTION GLOBAL __cdecl free_SCAN VAR_INPUT p0 : ^_SCAN; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_UNIT VAR_INPUT p0 : ^_MEM_UNIT; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_UNIT VAR_INPUT p0 : ^_MEM_UNIT; END_VAR ;
FUNCTION GLOBAL __cdecl init_UNIT VAR_INPUT p0 : ^_UNIT; END_VAR ;
FUNCTION GLOBAL __cdecl free_UNIT VAR_INPUT p0 : ^_UNIT; END_VAR ;
FUNCTION GLOBAL __cdecl construct_MAIN;
FUNCTION GLOBAL __cdecl init_MAIN;
FUNCTION GLOBAL __cdecl free_MAIN;
FUNCTION GLOBAL __cdecl init_ZORDER VAR_INPUT p0 : ^_ZORDER; END_VAR ;
FUNCTION GLOBAL __cdecl free_ZORDER VAR_INPUT p0 : ^_ZORDER; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_ZORDER VAR_INPUT p0 : ^_MEM_ZORDER; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_ZORDER VAR_INPUT p0 : ^_MEM_ZORDER; END_VAR ;
FUNCTION GLOBAL __cdecl chk_VARLABEL VAR_INPUT p0 : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl valid_VARLABEL VAR_INPUT p0 : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl init_SOFTFLOOR VAR_INPUT p0 : ^_SOFTFLOOR; END_VAR ;
FUNCTION GLOBAL __cdecl free_SOFTFLOOR VAR_INPUT p0 : ^_SOFTFLOOR; END_VAR ;
FUNCTION GLOBAL __cdecl init_SINGLESOFT VAR_INPUT p0 : ^_SINGLESOFT; END_VAR ;
FUNCTION GLOBAL __cdecl free_SINGLESOFT VAR_INPUT p0 : ^_SINGLESOFT; END_VAR ;
FUNCTION GLOBAL __cdecl init_PRJ_ENTRY VAR_INPUT p0 : ^_PRJ_ENTRY; END_VAR ;
FUNCTION GLOBAL __cdecl free_PRJ_ENTRY VAR_INPUT p0 : ^_PRJ_ENTRY; END_VAR ;
FUNCTION GLOBAL __cdecl init_PROJECT VAR_INPUT p0 : ^_PRJ_ENTRY; END_VAR ;
FUNCTION GLOBAL __cdecl free_PROJECT VAR_INPUT p0 : ^_PRJ_ENTRY; END_VAR ;
FUNCTION GLOBAL __cdecl init_STACK_LINE VAR_INPUT p0 : ^_STACK_LINE; END_VAR ;
FUNCTION GLOBAL __cdecl free_STACK_LINE VAR_INPUT p0 : ^_STACK_LINE; END_VAR ;
FUNCTION GLOBAL __cdecl init_FONTINFO VAR_INPUT p0 : ^_FONTINFO; END_VAR ;
FUNCTION GLOBAL __cdecl free_FONTINFO VAR_INPUT p0 : ^_FONTINFO; END_VAR ;
FUNCTION GLOBAL __cdecl init_ACTUAL VAR_INPUT p0 : ^_ACTUAL; END_VAR ;
FUNCTION GLOBAL __cdecl free_ACTUAL VAR_INPUT p0 : ^_ACTUAL; END_VAR ;
FUNCTION GLOBAL __cdecl init_SYSTEM VAR_INPUT p0 : ^_SYSTEM; END_VAR ;
FUNCTION GLOBAL __cdecl free_SYSTEM VAR_INPUT p0 : ^_SYSTEM; END_VAR ;
FUNCTION GLOBAL __cdecl init_PARA_PICKER VAR_INPUT p0 : ^_PARA_PICKER; END_VAR ;
FUNCTION GLOBAL __cdecl free_PARA_PICKER VAR_INPUT p0 : ^_PARA_PICKER; END_VAR ;
FUNCTION GLOBAL __cdecl init_CHREDIT VAR_INPUT p0 : ^_CHREDIT; END_VAR ;
FUNCTION GLOBAL __cdecl free_CHREDIT VAR_INPUT p0 : ^_CHREDIT; END_VAR ;
FUNCTION GLOBAL __cdecl init_IMAGE VAR_INPUT p0 : ^_IMAGE; END_VAR ;
FUNCTION GLOBAL __cdecl free_IMAGE VAR_INPUT p0 : ^_IMAGE; END_VAR ;
FUNCTION GLOBAL __cdecl init_POSITION VAR_INPUT p0 : ^_POSITION; END_VAR ;
FUNCTION GLOBAL __cdecl free_POSITION VAR_INPUT p0 : ^_POSITION; END_VAR ;
FUNCTION GLOBAL __cdecl init_MENUPOINTER VAR_INPUT p0 : ^_MENUPOINTER; END_VAR ;
FUNCTION GLOBAL __cdecl free_MENUPOINTER VAR_INPUT p0 : ^_MENUPOINTER; END_VAR ;
FUNCTION GLOBAL __cdecl init_MENUCALCUL VAR_INPUT p0 : ^_MENUCALCUL; END_VAR ;
FUNCTION GLOBAL __cdecl free_MENUCALCUL VAR_INPUT p0 : ^_MENUCALCUL; END_VAR ;
FUNCTION GLOBAL __cdecl init_MENU VAR_INPUT p0 : ^_MENU; END_VAR ;
FUNCTION GLOBAL __cdecl free_MENU VAR_INPUT p0 : ^_MENU; END_VAR ;
FUNCTION GLOBAL __cdecl init_SUBACTUAL VAR_INPUT p0 : ^_SUBACTUAL; END_VAR ;
FUNCTION GLOBAL __cdecl free_SUBACTUAL VAR_INPUT p0 : ^_SUBACTUAL; END_VAR ;
FUNCTION GLOBAL __cdecl init_COL_RECT VAR_INPUT p0 : ^_COL_RECT; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_COL_RECT VAR_INPUT p0 : ^_MEM_COL_RECT; END_VAR ;
FUNCTION GLOBAL __cdecl free_COL_RECT VAR_INPUT p0 : ^_COL_RECT; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_COL_RECT VAR_INPUT p0 : ^_MEM_COL_RECT; END_VAR ;
FUNCTION GLOBAL __cdecl init_COL_POLY VAR_INPUT p0 : ^_COL_POLY; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_COL_POLY VAR_INPUT p0 : ^_MEM_COL_POLY; END_VAR ;
FUNCTION GLOBAL __cdecl free_COL_POLY VAR_INPUT p0 : ^_COL_POLY; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_COL_POLY VAR_INPUT p0 : ^_MEM_COL_POLY; END_VAR ;
FUNCTION GLOBAL __cdecl init_COL_CIRC VAR_INPUT p0 : ^_COL_CIRC; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_COL_CIRC VAR_INPUT p0 : ^_MEM_COL_CIRC; END_VAR ;
FUNCTION GLOBAL __cdecl free_COL_CIRC VAR_INPUT p0 : ^_COL_CIRC; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_COL_CIRC VAR_INPUT p0 : ^_MEM_COL_CIRC; END_VAR ;
FUNCTION GLOBAL __cdecl init_COL_PICTXT VAR_INPUT p0 : ^_COL_PICTXT; END_VAR ;
FUNCTION GLOBAL __cdecl free_COL_PICTXT VAR_INPUT p0 : ^_COL_PICTXT; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_COL_PICTXT VAR_INPUT p0 : ^_MEM_COL_PICTXT; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_COL_PICTXT VAR_INPUT p0 : ^_MEM_COL_PICTXT; END_VAR ;
FUNCTION GLOBAL __cdecl init_KEY VAR_INPUT p0 : ^_KEY; END_VAR ;
FUNCTION GLOBAL __cdecl free_KEY VAR_INPUT p0 : ^_KEY; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_KEY VAR_INPUT p0 : ^_MEM_KEY; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_KEY VAR_INPUT p0 : ^_MEM_KEY; END_VAR ;
FUNCTION GLOBAL __cdecl init_COMDEF VAR_INPUT p0 : ^_COMDEF; END_VAR ;
FUNCTION GLOBAL __cdecl free_COMDEF VAR_INPUT p0 : ^_COMDEF; END_VAR ;
FUNCTION GLOBAL __cdecl init_COMPORT VAR_INPUT p0 : ^_COMPORT; END_VAR ;
FUNCTION GLOBAL __cdecl free_COMPORT VAR_INPUT p0 : ^_COMPORT; END_VAR ;
FUNCTION GLOBAL __cdecl init_INDEX VAR_INPUT p0 : ^_INDEX; END_VAR ;
FUNCTION GLOBAL __cdecl free_INDEX VAR_INPUT p0 : ^_INDEX; END_VAR ;
FUNCTION GLOBAL __cdecl init_INDEXDAT VAR_INPUT p0 : ^_INDEXDAT; END_VAR ;
FUNCTION GLOBAL __cdecl free_INDEXDAT VAR_INPUT p0 : ^_INDEXDAT; END_VAR ;
FUNCTION GLOBAL __cdecl init_REFDAT VAR_INPUT p0 : ^_REFDAT; END_VAR ;
FUNCTION GLOBAL __cdecl free_REFDAT VAR_INPUT p0 : ^_REFDAT; END_VAR ;
FUNCTION GLOBAL __cdecl init_REFLIST VAR_INPUT p0 : ^_REFLIST; END_VAR ;
FUNCTION GLOBAL __cdecl free_REFLIST VAR_INPUT p0 : ^_REFLIST; END_VAR ;
FUNCTION GLOBAL __cdecl init_LASALID VAR_INPUT p0 : ^_LASALID; END_VAR ;
FUNCTION GLOBAL __cdecl init_PICFORMAT VAR_INPUT p0 : ^_PICFORMAT; END_VAR ;
FUNCTION GLOBAL __cdecl free_PICFORMAT VAR_INPUT p0 : ^_PICFORMAT; END_VAR ;
FUNCTION GLOBAL __cdecl init_LINEINFO VAR_INPUT p0 : ^_LINEINFO; END_VAR ;
FUNCTION GLOBAL __cdecl free_LINEINFO VAR_INPUT p0 : ^_LINEINFO; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_LINEINFO VAR_INPUT p0 : ^_MEM_LINEINFO; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_LINEINFO VAR_INPUT p0 : ^_MEM_LINEINFO; END_VAR ;
FUNCTION GLOBAL __cdecl init_MENUINFO VAR_INPUT p0 : ^_MENUINFO; END_VAR ;
FUNCTION GLOBAL __cdecl free_MENUINFO VAR_INPUT p0 : ^_MENUINFO; END_VAR ;
FUNCTION GLOBAL __cdecl init_WORKSTATE VAR_INPUT p0 : ^_WORKSTATE; END_VAR ;
FUNCTION GLOBAL __cdecl init_STACK VAR_INPUT p0 : ^_STACK; END_VAR ;
FUNCTION GLOBAL __cdecl free_STACK VAR_INPUT p0 : ^_STACK; END_VAR ;
FUNCTION GLOBAL __cdecl init_ASCII_BUFFER VAR_INPUT p0 : ^_ASCII_BUFFER; END_VAR ;
FUNCTION GLOBAL __cdecl free_ASCII_BUFFER VAR_INPUT p0 : ^_ASCII_BUFFER; END_VAR ;
FUNCTION GLOBAL __cdecl init_TEXT_BUFFER VAR_INPUT p0 : ^_TEXT_BUFFER; END_VAR ;
FUNCTION GLOBAL __cdecl free_TEXT_BUFFER VAR_INPUT p0 : ^_TEXT_BUFFER; END_VAR ;
FUNCTION GLOBAL __cdecl cmp_VARLABEL VAR_INPUT p0 : ^_VARLABEL; p1 : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl init_COMPCODE VAR_INPUT p0 : ^_COMPCODE; END_VAR ;
FUNCTION GLOBAL __cdecl free_COMPCODE VAR_INPUT p0 : ^_COMPCODE; END_VAR ;
FUNCTION GLOBAL __cdecl init_SINGLESEEK VAR_INPUT p0 : ^_SINGLESEEK; END_VAR ;
FUNCTION GLOBAL __cdecl free_SINGLESEEK VAR_INPUT p0 : ^_SINGLESEEK; END_VAR ;
FUNCTION GLOBAL __cdecl init_SEEKELEMENT VAR_INPUT p0 : ^_SEEKELEMENT; END_VAR ;
FUNCTION GLOBAL __cdecl free_SEEKELEMENT VAR_INPUT p0 : ^_SEEKELEMENT; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_SEEKELEMENT VAR_INPUT p0 : ^_MEM_SEEKELEMENT; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_SEEKELEMENT VAR_INPUT p0 : ^_MEM_SEEKELEMENT; END_VAR ;
FUNCTION GLOBAL __cdecl init_MY_PICTURE VAR_INPUT p0 : ^_MY_PICTURE; END_VAR ;
FUNCTION GLOBAL __cdecl free_MY_PICTURE VAR_INPUT p0 : ^_MY_PICTURE; END_VAR ;
FUNCTION GLOBAL __cdecl setup_MY_PICTURE VAR_INPUT p0 : ^_MY_PICTURE; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl free_EDITCOLOR VAR_INPUT p0 : ^_EDITCOLOR; END_VAR ;
FUNCTION GLOBAL __cdecl init_EDITCOLOR VAR_INPUT p0 : ^_EDITCOLOR; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_ATTRIB VAR_INPUT p0 : ^_MEM_ATTRIB; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_ATTRIB VAR_INPUT p0 : ^_MEM_ATTRIB; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_COLOR VAR_INPUT p0 : ^_MEM_COLOR; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_COLOR VAR_INPUT p0 : ^_MEM_COLOR; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_ANYTHING VAR_INPUT p0 : ^_MEM_ANYTHING; END_VAR;
FUNCTION GLOBAL __cdecl free_MEM_ANYTHING VAR_INPUT p0 : ^_MEM_ANYTHING; END_VAR;
FUNCTION GLOBAL __cdecl init_MEM_ANYPARA VAR_INPUT p0 : ^_MEM_ANYPARA; END_VAR;
FUNCTION GLOBAL __cdecl free_MEM_ANYPARA VAR_INPUT p0 : ^_MEM_ANYPARA; END_VAR;
FUNCTION GLOBAL __cdecl init_MEM_LINKSCHEME VAR_INPUT p0 : ^_MEM_LINKSCHEME; END_VAR;
FUNCTION GLOBAL __cdecl free_MEM_LINKSCHEME VAR_INPUT p0 : ^_MEM_LINKSCHEME; END_VAR;
FUNCTION GLOBAL __cdecl init_DYNCOLOR VAR_INPUT p0 : ^_DYNCOLOR; END_VAR;
FUNCTION GLOBAL __cdecl free_DYNCOLOR VAR_INPUT p0 : ^_DYNCOLOR; END_VAR;
FUNCTION GLOBAL __cdecl init_MEM_DYNCOLOR VAR_INPUT p0 : ^_MEM_DYNCOLOR; END_VAR;
FUNCTION GLOBAL __cdecl free_MEM_DYNCOLOR VAR_INPUT p0 : ^_MEM_DYNCOLOR; END_VAR;
FUNCTION GLOBAL __cdecl init_MEM_FONT VAR_INPUT p0 : ^_MEM_FONT; END_VAR;
FUNCTION GLOBAL __cdecl free_MEM_FONT VAR_INPUT p0 : ^_MEM_FONT; END_VAR;
FUNCTION GLOBAL __cdecl init_MEM_BOOL VAR_INPUT p0 : ^_MEM_BOOL; END_VAR;
FUNCTION GLOBAL __cdecl free_MEM_BOOL VAR_INPUT p0 : ^_MEM_BOOL; END_VAR;
FUNCTION GLOBAL __cdecl init_MEM_TextPuffer VAR_INPUT p0 : ^_MEM_TextPuffer; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_TextPuffer VAR_INPUT p0 : ^_MEM_TextPuffer; END_VAR ;
#ifdef UC_BUBBLE_HELP
FUNCTION GLOBAL __cdecl init_BUBBLEHELPINFO VAR_INPUT p0 : ^_BUBBLEINFO; END_VAR ;
FUNCTION GLOBAL __cdecl free_BUBBLEHELPINFO VAR_INPUT p0 : ^_BUBBLEINFO; END_VAR ;
#endif

FUNCTION GLOBAL __cdecl init_CURSORCOLOR VAR_INPUT p0 : ^_CURSORCOLOR; END_VAR;
FUNCTION GLOBAL __cdecl free_CURSORCOLOR VAR_INPUT p0 : ^_CURSORCOLOR; END_VAR;
FUNCTION GLOBAL __cdecl init_ANYPARA VAR_INPUT p0 : ^_ANYPARA; END_VAR;
FUNCTION GLOBAL __cdecl free_ANYPARA VAR_INPUT p0 : ^_ANYPARA; END_VAR;

#ifdef OK_SUBLIST
FUNCTION GLOBAL __cdecl init_VKLIST VAR_INPUT p0 : ^_VKLIST; END_VAR;
FUNCTION GLOBAL __cdecl free_VKLIST VAR_INPUT p0 : ^_VKLIST; END_VAR;
FUNCTION GLOBAL __cdecl init_MEM_VKLIST VAR_INPUT p0 : ^_MEM_VKLIST; END_VAR;
FUNCTION GLOBAL __cdecl free_MEM_VKLIST VAR_INPUT p0 : ^_MEM_VKLIST; END_VAR;
#endif

FUNCTION GLOBAL __cdecl init_THEURGE VAR_INPUT p0 : ^_THEURGE; END_VAR;
FUNCTION GLOBAL __cdecl free_THEURGE VAR_INPUT p0 : ^_THEURGE; END_VAR;

#ifdef UC_DIRECTORY
FUNCTION GLOBAL __cdecl init_DIRLIST VAR_INPUT p0 : ^_DIRLIST; END_VAR ;
FUNCTION GLOBAL __cdecl free_DIRLIST VAR_INPUT p0 : ^_DIRLIST; END_VAR ;
FUNCTION GLOBAL __cdecl init_DIRECTORY VAR_INPUT p0 : ^_DIRECTORY; END_VAR ;
FUNCTION GLOBAL __cdecl free_DIRECTORY VAR_INPUT p0 : ^_DIRECTORY; END_VAR ;
FUNCTION GLOBAL __cdecl init_DIRECTORYSETUP VAR_INPUT p0 : ^_DIRECTORYSETUP; END_VAR ;
FUNCTION GLOBAL __cdecl free_DIRECTORYSETUP VAR_INPUT p0 : ^_DIRECTORYSETUP; END_VAR ;
#endif
#ifdef UC_IPC
FUNCTION GLOBAL __cdecl init_DPNE VAR_INPUT p0 : ^_DPNE; END_VAR ;
FUNCTION GLOBAL __cdecl free_DPNE VAR_INPUT p0 : ^_DPNE; END_VAR ;
#endif
FUNCTION GLOBAL __cdecl init_PICRUNMEMO VAR_INPUT p0 : ^_PICRUNMEMO; END_VAR ;
FUNCTION GLOBAL __cdecl free_PICRUNMEMO VAR_INPUT p0 : ^_PICRUNMEMO; END_VAR ;
#ifdef OK_PICMEMO
FUNCTION GLOBAL __cdecl init_LASTINPUT VAR_INPUT p0 : ^_LASTINPUT; END_VAR ;
FUNCTION GLOBAL __cdecl free_LASTINPUT VAR_INPUT p0 : ^_LASTINPUT; END_VAR ;
FUNCTION GLOBAL __cdecl init_GLOBMEMO VAR_INPUT p0 : ^_GLOBMEMO; END_VAR ;
FUNCTION GLOBAL __cdecl free_GLOBMEMO VAR_INPUT p0 : ^_GLOBMEMO; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_GLOBMEMO VAR_INPUT p0 : ^_MEM_GLOBMEMO; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_GLOBMEMO VAR_INPUT p0 : ^_MEM_GLOBMEMO; END_VAR ;
#endif
#ifdef OK_VALIDATE
FUNCTION GLOBAL __cdecl init_VALIDATA VAR_INPUT p0 : ^_VALIDATA; END_VAR ;
FUNCTION GLOBAL __cdecl free_VALIDATA VAR_INPUT p0 : ^_VALIDATA; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_VALIDATA VAR_INPUT p0 : ^_MEM_VALIDATA; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_VALIDATA VAR_INPUT p0 : ^_MEM_VALIDATA; END_VAR ;
#endif
FUNCTION GLOBAL __cdecl init_MEM_POINTER VAR_INPUT p0 : ^_MEM_POINTER; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_POINTER VAR_INPUT p0 : ^_MEM_POINTER; END_VAR ;
FUNCTION GLOBAL __cdecl init_MEM_POINTERPARA VAR_INPUT p0 : ^_MEM_POINTERPARA; END_VAR ;
FUNCTION GLOBAL __cdecl free_MEM_POINTERPARA VAR_INPUT p0 : ^_MEM_POINTERPARA; END_VAR ;
FUNCTION GLOBAL __cdecl init_SCREENSAVER VAR_INPUT p0 : ^_SCREENSAVER; END_VAR;
FUNCTION GLOBAL __cdecl free_SCREENSAVER VAR_INPUT p0 : ^_SCREENSAVER; END_VAR;
FUNCTION GLOBAL __cdecl init_DIRECTION VAR_INPUT p0 : ^_DIRECTION; END_VAR;
FUNCTION GLOBAL __cdecl free_DIRECTION VAR_INPUT p0 : ^_DIRECTION; END_VAR;
FUNCTION GLOBAL __cdecl init_SINGLEALARM VAR_INPUT p0 : ^_SINGLEALARM; END_VAR;
FUNCTION GLOBAL __cdecl free_SINGLEALARM VAR_INPUT p0 : ^_SINGLEALARM; END_VAR;
FUNCTION GLOBAL __cdecl init_MEM_SINGLEALARM VAR_INPUT p0 : ^_MEM_SINGLEALARM; END_VAR;
FUNCTION GLOBAL __cdecl free_MEM_SINGLEALARM VAR_INPUT p0 : ^_MEM_SINGLEALARM; END_VAR;
FUNCTION GLOBAL __cdecl make_MEM_SINGLEALARM VAR_INPUT p0 : ^_MEM_SINGLEALARM; x1 : UDINT; x2 : UINT; END_VAR;
FUNCTION GLOBAL __cdecl init_MENCURSOR VAR_INPUT p0 : ^_MENCURSOR; END_VAR;
FUNCTION GLOBAL __cdecl free_MENCURSOR VAR_INPUT p0 : ^_MENCURSOR; END_VAR;
FUNCTION GLOBAL __cdecl init_RINGBUFFER VAR_INPUT p0 : ^_RINGBUFFER; END_VAR;
FUNCTION GLOBAL __cdecl free_RINGBUFFER VAR_INPUT p0 : ^_RINGBUFFER; END_VAR;
FUNCTION GLOBAL __cdecl set_MENCURSOR VAR_INPUT p0 : ^_MENCURSOR; END_VAR;
FUNCTION GLOBAL __cdecl ask_MENCURSOR VAR_INPUT p0 : ^_MENCURSOR; END_VAR;

FUNCTION GLOBAL __cdecl IsDrawBackGround VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl IsDrawPicture VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl chk_LASALID VAR_INPUT p0 : ^_LASALID; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl chk_LINEINFO VAR_INPUT p0 : ^_LINEINFO; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl state_LINEINFO VAR_INPUT p0 : ^_LINEINFO; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl chk_ROOM VAR_INPUT p0 : ^_ROOM; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl init_LSEOSZI VAR_INPUT p0 : ^_LSEOSZI; END_VAR;
FUNCTION GLOBAL __cdecl free_LSEOSZI VAR_INPUT p0 : ^_LSEOSZI; END_VAR;
FUNCTION GLOBAL __cdecl init_LSEOSZI_CHANNEL VAR_INPUT p0 : ^_LSEOSZI_CHANNEL; END_VAR;
FUNCTION GLOBAL __cdecl free_LSEOSZI_CHANNEL VAR_INPUT p0 : ^_LSEOSZI_CHANNEL; END_VAR;
FUNCTION GLOBAL __cdecl init_MEM_LSEOSZI_CHANNEL VAR_INPUT p0 : ^_MEM_LSEOSZI_CHANNEL; END_VAR;
FUNCTION GLOBAL __cdecl free_MEM_LSEOSZI_CHANNEL VAR_INPUT p0 : ^_MEM_LSEOSZI_CHANNEL; END_VAR;
FUNCTION GLOBAL __cdecl init_SHAPE VAR_INPUT p0 :^_SHAPE; END_VAR;
FUNCTION GLOBAL __cdecl free_SHAPE VAR_INPUT p0 :^_SHAPE; END_VAR;
FUNCTION GLOBAL __cdecl init_3DSHAPE VAR_INPUT p0 : ^_3DSHAPE; END_VAR;
FUNCTION GLOBAL __cdecl free_3DSHAPE VAR_INPUT p0 : ^_3DSHAPE; END_VAR;
FUNCTION GLOBAL __CDECL init_LINKVL VAR_INPUT p0 : ^_LINKVL; END_VAR;
FUNCTION GLOBAL __CDECL free_LINKVL VAR_INPUT p0 : ^_LINKVL; END_VAR;
FUNCTION GLOBAL __CDECL init_DATA VAR_INPUT p0 : ^_DATA; END_VAR;
FUNCTION GLOBAL __CDECL free_DATA VAR_INPUT p0 : ^_DATA; END_VAR;
FUNCTION GLOBAL __CDECL init_USERTEXT VAR_INPUT p0 : ^_USERTEXT; END_VAR;
FUNCTION GLOBAL __CDECL setup_USERTEXT VAR_INPUT p0 : ^_USERTEXT; txt : ^void; chrsize : uint; END_VAR;
FUNCTION GLOBAL __CDECL free_USERTEXT VAR_INPUT p0 : ^_USERTEXT; END_VAR;
FUNCTION GLOBAL __CDECL AcknowledgeFlag VAR_INPUT cmd : DINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;

#ifdef UC_DELETE_REFLIST
FUNCTION GLOBAL __cdecl DeleteRefreshlistOnRequest VAR_INPUT st : BOOL; END_VAR;
#endif

// JAPAN.CPP ****************************************************************
#define JAPAN_DIACRIT_1    12317 // "
#define JAPAN_DIACRIT_2    12332 // °

// T_JPG.CPP *****************************************************************
FUNCTION GLOBAL __cdecl JpgFunctSetup VAR_INPUT fpload : ^void; fpsave : ^void; END_VAR;
FUNCTION GLOBAL __cdecl load_jpg VAR_INPUT p0 : ^_BMP; p1 : ^_ASCII; p_errorcode : ^UINT:=NIL; fp_callback : ^void:=NIL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl save_jpg VAR_INPUT p0 : ^_ASCII; x1 : _KOORD; x2 : _KOORD; x3 : _KOORD; x4 : _KOORD; x5 : _UWORD; x6 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl save_jpg_BMP VAR_INPUT p0 : ^_ASCII; x1 : ^_BMP; x2 : _UWORD; x3 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;

// T_KBRD.CPP ****************************************************************
FUNCTION GLOBAL __cdecl AddEvent VAR_INPUT p0 : ^_EVENT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl StartStopEvent VAR_INPUT x0 : BOOL; x1 : UINT; END_VAR;
FUNCTION GLOBAL __cdecl SetRP_DelayTime VAR_INPUT ms_time : udint; END_VAR VAR_OUTPUT retcode : udint; END_VAR;
FUNCTION GLOBAL __cdecl IsCtrlKeyPress VAR_INPUT pev : ^_EVENT; character : usint; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl GetPossibleHidReleaseEvent VAR_INPUT pev : ^_EVENT; button : _HID_BUTTON; multindex : udint; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl TouchDeviceLock VAR_INPUT device:udint; state:bool; END_VAR;

// T_LANG.CPP ***************************************************************
FUNCTION GLOBAL __cdecl seperate_textlist VAR_INPUT dst : ^pvoid; pno : ^UDINT; language_no : UINT; txtlst : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Language_GetStandardText VAR_INPUT pptxt : ^pvoid; pchrsize : ^uint; txtid : _STDTEXTID; END_VAR;
FUNCTION GLOBAL __cdecl Language_GetActualLocal VAR_OUTPUT retcode : _LOCAL; END_VAR;

// T_LIBEX.CPP **************************************************************
#define lib_arraysize(_P) (sizeof(_P) / sizeof(_P[0]))
function global __cdecl lib_atomic_incU32 var_input pValue:^udint; end_var var_output retcode:udint; end_var;
function global __cdecl lib_atomic_decU32 var_input pValue:^udint; end_var var_output retcode:udint; end_var;
function global __cdecl lib_atomic_cmpxchgU32 var_input pValue:^udint; cmpVal:udint; newVal:udint; end_var var_output retcode:udint; end_var;
function global __cdecl lib_is_salamander var_output retcode:udint; end_var;
function global __cdecl lib_is_linux var_output retcode:udint; end_var;
function global __cdecl lib_is_lars var_output retcode:udint; end_var;
function global __cdecl lib_tabsolute var_output retcode:udint; end_var;
function global __cdecl lib_tabsolute_nonzero var_output retcode:udint; end_var;
function global __cdecl lib_calloc var_input size:udint; end_var var_output retcode:^void; end_var;
function global __cdecl lib_malloc var_input size:udint; end_var var_output retcode:^void; end_var;
function global __cdecl lib_realloc var_input ptr:^void; size:udint; end_var var_output retcode:^void; end_var;
function global __cdecl lib_free var_input ptr:^void; end_var var_output retcode:^void; end_var;

// T_LIB.CPP ****************************************************************
FUNCTION GLOBAL __cdecl find_lasal_method VAR_INPUT p0 : ^void; x1 : UINT; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl find_this_method VAR_INPUT p0 : ^void; x1 : UINT; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl get_this_pointer VAR_INPUT p0 : ^_ASCII; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl GetVirtualMethodNumber VAR_INPUT pthis : ^void; pmeth : ^void; END_VAR VAR_OUTPUT retcode : udint; END_VAR;
FUNCTION GLOBAL __cdecl GetVirtualMethodPointer VAR_INPUT pthis : ^void; vmno : udint; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl TeachGraphicSystem VAR_INPUT x0 : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl ConfigGraphicSystem VAR_INPUT x0 : UDINT; x1 : UDINT; p2 : ^void; END_VAR;
FUNCTION GLOBAL __cdecl add_MEM_POINTER VAR_INPUT p0 : ^_MEM_POINTER; p1 : ^void; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl add_MEM_POINTERPARA VAR_INPUT p : ^_MEM_POINTERPARA; ptr : ^void; para : udint; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;

FUNCTION GLOBAL __cdecl ReadPixelStart VAR_INPUT x0:int; y0:int; x1:int; y1:int; END_VAR;
FUNCTION GLOBAL __cdecl ReadPixelStop;
FUNCTION GLOBAL __cdecl RealToStr VAR_INPUT txt : ^void; value : REAL; dzp : UINT; chr_size : UINT; END_VAR;
FUNCTION GLOBAL __cdecl StrToReal VAR_INPUT rval : ^_REAL; txt : ^void; chrsize : UINT; end_var var_output retcode : bool; end_var;
FUNCTION GLOBAL __cdecl get_multiplicator VAR_INPUT x0 : UINT; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
FUNCTION GLOBAL __cdecl GetTextRoom VAR_INPUT p0 : ^_ROOM; p1 : ^_ROOM; att : _ATTRIB; frame : _FRAMETYPE; tfd : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl qsort_CHAR VAR_INPUT p0 : ^_CHAR; p1 : ^_CHAR; END_VAR VAR_OUTPUT retcode : INT; END_VAR;
FUNCTION GLOBAL __cdecl qsort_ASCII VAR_INPUT p0 : ^_ASCII; p1 : ^_ASCII; END_VAR VAR_OUTPUT retcode : INT; END_VAR;
FUNCTION GLOBAL __cdecl Lse_DToH VAR_INPUT p0 : ^void; x1 : DINT; x2 : UINT; x3 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl Lse_DToA VAR_INPUT p0 : ^void; x1 : DINT; x2 : UINT; x3 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl Lse_StrLen VAR_INPUT p0 : ^void; x1 : UINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl Lse_StrCpy VAR_INPUT p0 : ^void; x1 : UINT; p2 : ^void; x3 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl Lse_StrCat VAR_INPUT p0 : ^void; x1 : UINT; p2 : ^void; x3 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl Lse_StrCmp VAR_INPUT p0 : ^void; x1 : UINT; p2 : ^void; x3 : UINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl Lse_StrSize VAR_INPUT p0 : ^void; x1 : UINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl Lse_StrChr VAR_INPUT p0 : ^void; x1 : _UNI; x2 : UINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl Lse_StrUp VAR_INPUT p0 : ^void; x1 : UINT; x2 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl out_radiobutton VAR_INPUT p0 : ^_ROOM; x1 : DINT; x2 : _ATTRIB; x3 : _COLOR; x4 : _COLOR; x5 : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl out_chkbox VAR_INPUT p0 : ^_ROOM; x1 : DINT; x2 : _ATTRIB; x3 : _COLOR; x4 : _COLOR; x5 : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl Lse_OutTextAlign VAR_INPUT p0 : ^_CHAR; p1 : ^_ROOM; x2 : _FONT; x3 : _ATTRIB; x4 : _COLOR; END_VAR ;
#ifdef _LSL_TARGETARCH_X86
FUNCTION GLOBAL __cdecl Lse_OutParameterString VAR_INPUT x0 : _ROOM; p1 : ^_CHAR; p2 : ^_MEM_VARLABEL; x3 : _FONT; x4 : _ATTRIB; x5 : _COLOR; END_VAR ;
#endif
FUNCTION GLOBAL __cdecl Lse_OutParaString VAR_INPUT proom : ^_ROOM; ptxt : ^_CHAR; pmv : ^_MEM_VARLABEL; fnt : _FONT; attrib : _ATTRIB; color : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl lse_format_value VAR_INPUT p0 : ^_CHAR; x1 : DINT; p2 : ^_VARINFO; p3 : ^_PICFORMAT; END_VAR ;
// String functions
FUNCTION GLOBAL __cdecl StrLen VAR_INPUT txt : ^void; size : UINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl StrChr VAR_INPUT p0 : ^void; x1 : _UNI; x2 : UINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;

FUNCTION GLOBAL __cdecl StrNCpy VAR_INPUT p0 : ^void; x1 : UINT; p2 : ^void; x3 : UINT; ml : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl StrCpy VAR_INPUT p0 : ^void; x1 : UINT; p2 : ^void; x3 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl StrCat VAR_INPUT p0 : ^void; x1 : UINT; p2 : ^void; x3 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl StrSize VAR_INPUT p0 : ^void; x1 : UINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl DelComment VAR_INPUT p0 : ^void; x1 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl DelChr VAR_INPUT p0 : ^void; x1 : UINT; x2 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl DelSpace VAR_INPUT p0 : ^void; x1 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl StrFill VAR_INPUT p0 : ^void; x1 : _UNI; x2 : UINT; x3 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl StrCmp VAR_INPUT p0 : ^void; x1 : UINT; p2 : ^void; x3 : UINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl StrUp VAR_INPUT p0 : ^void; x1 : UINT; x2 : UDINT; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl UniToAscii VAR_INPUT p0 : ^_ASCII; p1 : ^_UNI; x2 : UDINT; END_VAR ;
FUNCTION GLOBAL __cdecl AsciiToUni VAR_INPUT p0 : ^_UNI; p1 : ^_ASCII; x2 : UDINT; END_VAR ;
FUNCTION GLOBAL __cdecl uni_to_ascii VAR_INPUT p0 : ^_ASCII; p1 : ^_UNI; x2 : UDINT; END_VAR ;
FUNCTION GLOBAL __cdecl ascii_to_uni VAR_INPUT p0 : ^_UNI; p1 : ^_ASCII; x2 : UDINT; END_VAR ;
FUNCTION GLOBAL __cdecl create_filename VAR_INPUT p0 : ^_ASCII; p1 : ^_ASCII; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl sort_ASCII VAR_INPUT p0 : ^_ASCII; x1 : UINT; sortfunct : ^void := NIL; END_VAR ;
// Textdisplay functions
FUNCTION GLOBAL __cdecl OutTextAlign VAR_INPUT p0 : ^void; x1 : _UNI; p1 : ^_ROOM; x2 : _FONT; x3 : _ATTRIB; x9 : _FRAMETYPE; x4 : _COLOR; x5 : _COLOR; x6 : _COLOR; x7 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl OutTextIconAlign VAR_INPUT p0 : ^void; x1 : _UNI; p1 : ^_ROOM; x2 : _FONT; x3 : _ATTRIB; x9 : _FRAMETYPE; x4 : _COLOR; x5 : _COLOR; x6 : _COLOR; x7 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl OutTxtAlign VAR_INPUT p0 : ^void; p1 : ^_ROOM; x2 : _FONT; x3 : _ATTRIB; x4 : _COLOR; x5 : _COLOR; x6 : _COLOR; x7 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl OutMultiTextAlign VAR_INPUT p0 : ^void; x1 : _UNI; p2 : ^_ROOM; x3 : _FONT; x4 : _ATTRIB; x5 : _FRAMETYPE; x6 : _COLOR; x7 : _COLOR; x8 : _COLOR; x9 : UINT; END_VAR;
FUNCTION GLOBAL __cdecl OutMultiTxtAlign VAR_INPUT p0 : ^void; p1 : ^_ROOM; x2 : _FONT; x3 : _ATTRIB; x4 : _COLOR; x5 : _COLOR; x6 : _COLOR; x7 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl out_ANYTHING VAR_INPUT p0 : ^_ANYTHING; p1 : ^_ROOM; x2 : _FONT; x3 : _ATTRIB; x4 : _FRAMETYPE; x5 : _COLOR; x6 : _COLOR; x7 : _COLOR; p8 : ^void; END_VAR ;
// Clipping and invisible screens
FUNCTION GLOBAL __cdecl ActivateAndRegion VAR_INPUT p0 : ^_REGION; p1 : ^_REGION; p2 : ^_ROOM; END_VAR VAR_OUTPUT retcode : _REGION; END_VAR ;
FUNCTION GLOBAL __cdecl DeactivateAndRegion VAR_INPUT x0 : _REGION; p1 : ^_REGION; p2 : ^_REGION; END_VAR;
FUNCTION GLOBAL __cdecl InvertRegion VAR_OUTPUT retcode : _REGION; END_VAR;
//FUNCTION GLOBAL __cdecl ActivateScreen VAR_INPUT p0 : ^_NEWSCREEN; END_VAR ;
FUNCTION GLOBAL __cdecl push_screen VAR_INPUT p0 : ^_NEWSCREEN; END_VAR ;
FUNCTION GLOBAL __cdecl pop_screen VAR_INPUT p0 : ^_NEWSCREEN; END_VAR ;
FUNCTION GLOBAL __cdecl copy_screen VAR_INPUT p0 : ^_NEWSCREEN; p1 : ^_ROOM; END_VAR ;
// Converting
FUNCTION GLOBAL __cdecl string_to_numeric VAR_INPUT p0 : ^_CHAR; x1 : UINT; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
FUNCTION GLOBAL __cdecl string_to_date VAR_INPUT p0 : ^_DATE; p1 : ^_CHAR; x2 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl string_to_time VAR_INPUT p0 : ^_TIME; p1 : ^_CHAR; x2 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl string_to_value VAR_INPUT x0 : DINT; p1 : ^_VARINFO; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
FUNCTION GLOBAL __cdecl NumToStr VAR_INPUT p0 : ^void; x1 : DINT; x2 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl StrToNum VAR_INPUT p0 : ^DINT; p1 : ^void; x2 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl DToA VAR_INPUT p0 : ^void; x1 : DINT; x2 : UINT; x3 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl DToH VAR_INPUT p0 : ^void; x1 : DINT; x2 : UINT; x3 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl AToD VAR_INPUT p0 : ^void; x1 : UINT; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
FUNCTION GLOBAL __cdecl HToD VAR_INPUT p0 : ^void; x1 : UINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl lasal_to_DATE VAR_INPUT p0 : ^_DATE; x1 : DINT; END_VAR ;
FUNCTION GLOBAL __cdecl DATE_to_lasal VAR_INPUT p0 : ^DINT; p1 : ^_DATE; END_VAR ;
FUNCTION GLOBAL __cdecl lasal_to_TIME VAR_INPUT p0 : ^_TIME; x1 : DINT; END_VAR ;
FUNCTION GLOBAL __cdecl TIME_to_lasal VAR_INPUT p0 : ^DINT; p1 : ^_TIME; END_VAR ;
FUNCTION GLOBAL __cdecl DATE_Diff VAR_INPUT pdate0 : ^_DATE; pdate1 : ^_DATE; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
// Ringbuffer
FUNCTION GLOBAL __cdecl wpos_RINGBUFFER VAR_INPUT p0 : ^_RINGBUFFER; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl rpos_RINGBUFFER VAR_INPUT p0 : ^_RINGBUFFER; x1 : UDINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl no_RINGBUFFER VAR_INPUT p0 : ^_RINGBUFFER; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl sort_ringbuffer VAR_INPUT p0 : ^void; p1 : ^_RINGBUFFER; x2 : UINT; x3 : _SORTFUNCT; END_VAR ;
// Sorting
FUNCTION GLOBAL __cdecl sort VAR_INPUT p0 : ^void; x1 : UDINT; x2 : UINT; x3 : _SORTFUNCT; END_VAR ;
FUNCTION GLOBAL __cdecl qsort VAR_INPUT p0 : ^void; x1 : UDINT; x2 : UINT; x3 : _QSORTFUNCT; END_VAR ;
// Date and time functions
FUNCTION GLOBAL __cdecl set_DATE VAR_INPUT p0 : ^_DATE; END_VAR ;
FUNCTION GLOBAL __cdecl get_DATE VAR_INPUT p0 : ^_DATE; END_VAR ;
FUNCTION GLOBAL __cdecl set_TIME VAR_INPUT p0 : ^_TIME; END_VAR ;
FUNCTION GLOBAL __cdecl get_TIME VAR_INPUT p0 : ^_TIME; END_VAR ;
FUNCTION GLOBAL __cdecl get_DATIM VAR_INPUT p0 : ^_DATIM; END_VAR ;
FUNCTION GLOBAL __cdecl set_DATIM VAR_INPUT p0 : ^_DATIM; END_VAR ;
FUNCTION GLOBAL __cdecl day_of_week VAR_INPUT p0 : ^_DATE; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
// Formating functions
FUNCTION GLOBAL __cdecl format_variable VAR_INPUT p0 : ^_CHAR; p1 : ^_VARLABEL; END_VAR;
FUNCTION GLOBAL __cdecl format_unit VAR_INPUT p0 : ^_CHAR; p1 : ^_VARLABEL; END_VAR;
FUNCTION GLOBAL __cdecl format_value VAR_INPUT p0 : ^_CHAR; x1 : DINT; p2 : ^_VARINFO; p3 : ^_PICFORMAT; END_VAR ;
FUNCTION GLOBAL __cdecl format_string VAR_INPUT p0 : ^_CHAR; p1 :  ^_CHAR; p2 : ^_VARINFO; END_VAR;
FUNCTION GLOBAL __cdecl format_time VAR_INPUT p0 : ^_CHAR; p1 : ^_TIME; x2 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl format_date VAR_INPUT p0 : ^_CHAR; p1 : ^_DATE; x2 : UINT; END_VAR ;
// Screen and data manipulation
FUNCTION GLOBAL __cdecl delete_IO_from_screen VAR_INPUT p0 : ^_IO; END_VAR ;
FUNCTION GLOBAL __cdecl change_input VAR_INPUT p0 : ^_MY_PICTURE; x1 : UDINT; x2 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl change_output VAR_INPUT p0 : ^_MY_PICTURE; x1 : UDINT; x2 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl change_io_group VAR_INPUT p0 : ^_MY_PICTURE; x1 : UINT; x2 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
// Scheme processing
FUNCTION GLOBAL __cdecl NewDoScheme VAR_INPUT x0 : _ANYLST; x1 : DINT; x2 : UDINT; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl DoScheme VAR_INPUT p7 : ^PVOID; x0 : _ANYLST; x1 : DINT; x2 : UDINT; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl DoTextScheme VAR_INPUT x1 : DINT; x2 : UINT; p1 : ^_MEM_VARLABEL; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl get_MYSCHEME VAR_INPUT p0 : ^_MYSCHEME; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl DoColorScheme VAR_INPUT ls : ^_LINKSCHEME; defcol : _COLOR; END_VAR VAR_OUTPUT retcode : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl StupidTextScheme VAR_INPUT x1 : DINT; x2 : UDINT; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;
// Functions to access LSE lists - Textlist, IconList, ...
FUNCTION GLOBAL __cdecl make_parameterstring VAR_INPUT p0 : ^_CHAR; p1 : ^_MEM_VARLABEL; x0 : BOOL; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;
FUNCTION GLOBAL __cdecl get_ANYTHING VAR_INPUT x0 : ^_ANYTHING; x1 : ^void; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl get_text VAR_INPUT p0 : ^_ANYTHING; p1 : ^void; p2 : ^_MEM_VARLABEL; x0 : BOOL; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;
FUNCTION GLOBAL __cdecl get_ANYPARA VAR_INPUT p0 : ^_ANYPARA; p : ^void; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;
FUNCTION GLOBAL __cdecl ccreate_prefix VAR_INPUT p0 : ^_CHAR; p1 : ^_ANYTHING; p2 : ^void; p3 : ^_VARINFO; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;
// Diverse functions to get informations about color and attribut
FUNCTION GLOBAL __cdecl GetXSize VAR_INPUT x0 : _ATTRIB; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl GetYSize VAR_INPUT x0 : _ATTRIB; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl GetBackColor VAR_INPUT x0 : _COLOR; END_VAR VAR_OUTPUT retcode : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl GetPenColor VAR_INPUT x0 : _COLOR; END_VAR VAR_OUTPUT retcode : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl InvertColor VAR_INPUT x0 : _COLOR; END_VAR VAR_OUTPUT retcode : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl get_framewidth VAR_INPUT x0 : _FRAMETYPE; x1 : _ATTRIB; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl get_frameheight VAR_INPUT x0 : _FRAMETYPE; x1 : _ATTRIB; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl is_invisible VAR_INPUT x0 : _COLOR; x1 : _ATTRIB; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
// Functions to calculate a scroll area
FUNCTION GLOBAL __cdecl calculate_position VAR_INPUT p0 : ^_POSITION; p1 : ^_POSITION; p2 : ^_EVENT; p3 : ^_ROOM; END_VAR VAR_OUTPUT result : _POSRESULT; END_VAR;
FUNCTION GLOBAL __cdecl calculate_h_position VAR_INPUT p0 : ^_POSITION; p1 : ^_EVENT; x2 : UINT; END_VAR VAR_OUTPUT result : _POSRESULT; END_VAR;
FUNCTION GLOBAL __cdecl calculate_v_position VAR_INPUT p0 : ^_POSITION; p1 : ^_EVENT; x2 : UINT; END_VAR VAR_OUTPUT result : _POSRESULT; END_VAR;

// Diverse functions
FUNCTION GLOBAL __cdecl DrawButton VAR_INPUT p0 : ^_ROOM; x1 : _ATTRIB; x2 : _FRAMETYPE; x3 : _COLOR; x4 : _COLOR; END_VAR ;
FUNCTION GLOBAL __cdecl Debug VAR_INPUT x0 : DINT; x1 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl AddZoom VAR_INPUT p0 : ^_ZOOM; p1 : ^_ZOOM; END_VAR ;
FUNCTION GLOBAL __cdecl AddOffset VAR_INPUT p0 : ^_DOT; p1 : ^_DOT; p2 : ^_ZOOM; END_VAR ;
FUNCTION GLOBAL __cdecl TxtWidthFont VAR_INPUT p0 : ^void; x1 : UINT; x2 : UINT; x3 : _FONT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl MakeKoordX VAR_INPUT x0 : _KOORD; END_VAR VAR_OUTPUT retcode : _KOORD; END_VAR;
FUNCTION GLOBAL __cdecl MakeKoordY VAR_INPUT x0 : _KOORD; END_VAR VAR_OUTPUT retcode : _KOORD; END_VAR;
FUNCTION GLOBAL __cdecl IfNecHideMouseCursor VAR_INPUT p0 : ^_ROOM; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl IfNecShowMouseCursor VAR_INPUT p0 : ^UINT; END_VAR;
FUNCTION GLOBAL __cdecl ShowMouseCursor;
FUNCTION GLOBAL __cdecl HideMouseCursor;
FUNCTION GLOBAL __cdecl absolut VAR_INPUT x0 : DINT; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
FUNCTION GLOBAL __cdecl out_zoom_BMP VAR_INPUT p0 : ^_ROOM; p1 : ^_BMP; x2 : _ATTRIB; END_VAR ;
FUNCTION GLOBAL __cdecl out_part_BMP VAR_INPUT p0 : ^_ROOM; p1 : ^_BMP; x2 : _ATTRIB; END_VAR ;
FUNCTION GLOBAL __cdecl flash_memorize_zoom_gfx VAR_INPUT p0 : ^_ROOM; p1 : ^_ASCII; x2 : BOOL; END_VAR ;
FUNCTION GLOBAL __cdecl flash_memorize_gfx VAR_INPUT x0 : _KOORD; x1 : _KOORD; p2 : ^_ASCII; END_VAR ;
FUNCTION GLOBAL __cdecl ClrScr VAR_INPUT x0 : _COLOR; END_VAR ;
#ifdef UC_ABANDONED
FUNCTION GLOBAL __cdecl start_REPEAT VAR_INPUT p0 : ^_REPEAT; x1 : _TIP; END_VAR ;
FUNCTION GLOBAL __cdecl run_REPEAT VAR_INPUT p0 : ^_REPEAT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
#endif
FUNCTION GLOBAL __cdecl do_LSEFUNCT VAR_INPUT p0 : ^_LSEFUNCT; p1 : ^void; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl frame VAR_INPUT x0 : _KOORD; x1 : _KOORD; x2 : _KOORD; x3 : _KOORD; x4 : UINT; x5 : _ATTRIB; x6 : _COLOR; END_VAR ;
FUNCTION GLOBAL __cdecl round_frame VAR_INPUT x0 : _KOORD; x1 : _KOORD; x2 : _KOORD; x3 : _KOORD; x4 : UINT; x5 : _ATTRIB; x6 : _COLOR; END_VAR ;
FUNCTION GLOBAL __cdecl color_course VAR_INPUT p0 : ^_ROOM; x1 : INT; x2 : _COLOR; x3 : _COLOR; x4 : _ATTRIB; END_VAR;
FUNCTION GLOBAL __cdecl get_editor_state VAR_INPUT p0 : ^_MY_PICTURE; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl get_editor_varno VAR_INPUT p0 : ^_MY_PICTURE; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl rotate VAR_INPUT p0 : ^_DOT; x1 : DINT; END_VAR ;
FUNCTION GLOBAL __cdecl selfmade_indicator VAR_INPUT p0 : ^_ROOM; x1 : DINT; x2 : _COLOR; p3 : ^_DOT; x4 : UINT; END_VAR ;
FUNCTION GLOBAL __cdecl indicator VAR_INPUT p0 : ^_ROOM; x1 : DINT; x2 : _COLOR; END_VAR ;
FUNCTION GLOBAL __cdecl delete_eventbuffer;
FUNCTION GLOBAL __cdecl get_label_buffer VAR_INPUT list : _ANYLST; END_VAR VAR_OUTPUT retcode : ^_ASCII_BUFFER; END_VAR;
FUNCTION GLOBAL __cdecl get_no_by_label VAR_INPUT p0 : ^_ASCII; x1 : _ANYLST; x2 : UINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl out_BUTTON VAR_INPUT p0 : ^_DOT; x1 : BOOL; x2 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR ;
FUNCTION GLOBAL __cdecl OutTextIconXY VAR_INPUT x : _KOORD; y : _KOORD; txt : ^void; att : _ATTRIB; col : _COLOR; char_size : _UWORD; END_VAR;
FUNCTION GLOBAL __cdecl MenuList_GetNo VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetSingleUnitNo VAR_INPUT no : UDINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl SystemError VAR_INPUT x0 : _SYSERROR; x1 : DINT;  p2 : ^_CHAR; END_VAR;
FUNCTION GLOBAL __cdecl find_ROOM VAR_INPUT p0 : ^_ROOM; p1 : ^_ROOM; p3 : ^_ROOM; x3 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl get_LSEMENU VAR_INPUT p0 : UDINT; END_VAR VAR_OUTPUT retcode : ^_LSEMENU; END_VAR;
FUNCTION GLOBAL __cdecl transform_color_to_gray VAR_INPUT pcol : ^uint; END_VAR;
FUNCTION GLOBAL __cdecl transform_BMP_to_gray VAR_INPUT p : ^_BMP; END_VAR;
FUNCTION GLOBAL __cdecl StrCutLastTokenAndReturn VAR_INPUT dst : ^char; dpne : ^char; END_VAR VAR_OUTPUT retcode : ^char; END_VAR;
FUNCTION GLOBAL __cdecl GetLastToken VAR_INPUT dpne : ^char; END_VAR VAR_OUTPUT retcode : ^char; END_VAR;
FUNCTION GLOBAL __cdecl FileExist VAR_INPUT dpne : ^char; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl AddTokenEnd VAR_INPUT dpne : ^char; END_VAR;
FUNCTION GLOBAL __cdecl DelTokenEnd VAR_INPUT dpne : ^char; END_VAR;
FUNCTION GLOBAL __cdecl IsSalamander VAR_OUTPUT retcode : bool; END_VAR;

// T_LSEOSZ.CPP - just for internale use ************************************
FUNCTION GLOBAL __cdecl LseOsziAddChannel VAR_INPUT p0 : ^_LSEOSZI; x0 : UDINT; p1 : ^_ASCII; station : UINT; x1 : DINT; x2 : DINT; x3 : _COLOR; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl LseOsziConfigChannels VAR_INPUT p0 : ^_LSEOSZI; x1 : UDINT; p2 : ^_THEURGE; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl LseOsziConfigChannelsNew VAR_INPUT p0 : ^_LSEOSZI; x1 : UDINT; x2 : BOOL; p3 : ^_THEURGE; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl LseOsziCyclic VAR_INPUT p0 : ^_LSEOSZI; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl LseOsziDraw VAR_INPUT p0 : ^_LSEOSZI; p1 : ^_POSITION; p2 : ^_IO; p3 : ^_ROOM; x4 : UINT; x5 : UINT; x6 : UINT; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
FUNCTION GLOBAL __cdecl LseOsziDrawNew VAR_INPUT p0 : ^_DATIM; p1 : ^_LSEOSZI; p2 : ^_POSITION; p3 : ^_IO; p4 : ^_ROOM; x5 : UINT; x6 : UINT; x7 : UINT; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;

// T_MAIN.CPP - just for internal use ***************************************
FUNCTION GLOBAL __cdecl RTK_Init VAR_INPUT pt1 : ^VOID; p2 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl RTK_Ready VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl RTK_Run VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl RTK_End VAR_INPUT x0 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl GetUniquePlcId VAR_OUTPUT retcode : udint; END_VAR;

// T_MEMO.CPP ***************************************************************

// T_MENU.CPP ***************************************************************
FUNCTION GLOBAL __cdecl GetTextFromMenu VAR_INPUT x0 : UINT; x1 : DINT; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;

// T_MOVE.CPP ***************************************************************
FUNCTION GLOBAL __cdecl move_MY_PICTURE VAR_INPUT p0 : ^_MY_PICTURE; x1 : _KOORD; x2 : _KOORD; x3 : _ATTRIB; END_VAR;

// T_OBJ.CPP ****************************************************************
FUNCTION GLOBAL __cdecl ObjectList_Get_PICTURE VAR_INPUT x0 : UINT; END_VAR VAR_OUTPUT retcode : ^_PICTURE; END_VAR;
FUNCTION GLOBAL __cdecl ObjectList_Get_SubPicture VAR_INPUT objnr : UINT; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl ObjectList_Add VAR_INPUT label : ^_ASCII; mypic : ^_MY_PICTURE; END_VAR VAR_OUTPUT retcode : udint; END_VAR;

// T_OUTIO.CPP **************************************************************
FUNCTION GLOBAL __cdecl crc_string VAR_INPUT p0 : ^void; x1 : UINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl out_triangle VAR_INPUT proom:^_ROOM; color:_COLOR; END_VAR;


// T_OVRLOD.CPP *************************************************************
FUNCTION GLOBAL __cdecl overload VAR_INPUT p0 : ^UDINT; x1 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl overload_VARLABEL VAR_INPUT p0 : ^_VARLABEL; p1 : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl overload_PICTUREMASK VAR_INPUT p0 : ^_PICTUREMASK; p1 : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl change_overload_PICTUREMASK VAR_INPUT p0 : ^_PICTUREMASK; p1 : ^_VARLABEL; p2 : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl summary_overload_PICTUREMASK VAR_INPUT p0 : ^_PICTUREMASK; x1 : UINT; p2 : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
#ifdef USER_DEMAG
FUNCTION GLOBAL __cdecl summary_overload_PICTUREMASK_absolute VAR_INPUT p0 : ^_PICTUREMASK; x1 : _UWORD; p2 : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
#endif
FUNCTION GLOBAL __cdecl summary_overload_PICTURE VAR_INPUT p0 : UDINT; p1 : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;

// T_PAL.CPP ****************************************************************
FUNCTION GLOBAL __cdecl MakeColorFromIndex VAR_INPUT x0 : UINT; END_VAR VAR_OUTPUT retcode : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl Make_RGB VAR_INPUT p0 : ^_RGB; x1 : _COLOR; END_VAR;
#ifdef UC_NEWCOLORPALETTE
FUNCTION GLOBAL __cdecl SetNewColorPalette VAR_INPUT prgb : ^usint; startindex : udint; anzahl : udint; END_VAR;
#endif

// T_PARWIN.CPP *************************************************************
FUNCTION GLOBAL __cdecl XChangeParaWinObject VAR_INPUT x0 : UINT; x1 : UDINT; x2 : BOOL; x3 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;

// T_PICIDX.CPP *************************************************************
FUNCTION GLOBAL __cdecl GetPictureIndex VAR_INPUT p0 : ^_HOTKEYLIST; END_VAR VAR_OUTPUT retcode : ^_PICINDEX; END_VAR;
FUNCTION GLOBAL __cdecl FreePictureIndex VAR_INPUT p0 : ^_PICINDEX; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
#ifdef USER_DEMAG
FUNCTION GLOBAL __cdecl DestroyOffscreen;
FUNCTION GLOBAL __cdecl ReactivateOffscreen;
#endif

// T_PICMEM.CPP *************************************************************

// T_MESBOX.CPP *************************************************************
FUNCTION GLOBAL __cdecl MessageBoxCall VAR_INPUT text : ^void; chrsize : uint; font : _FONT; typ : _MESSAGEBOXTYP; pthis : ^void; pfkt : ^void; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl MessageBoxCallEx VAR_INPUT text : ^void; chrsize : uint; typ : _MESSAGEBOXTYP; pthis : ^void; pfkt : ^void; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl MessageBoxSetColorBox VAR_INPUT back : _COLOR; text : _COLOR; frame : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl MessageBoxSetColorButton VAR_INPUT back : _COLOR; text : _COLOR; frame : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl MessageBoxSetSizeBox VAR_INPUT width : udint; height : udint; END_VAR;
FUNCTION GLOBAL __cdecl MessageBoxSetSizeButton VAR_INPUT width : udint; height : udint; END_VAR;
FUNCTION GLOBAL __cdecl MessageBoxSetFrame VAR_INPUT box : _FRAMETYPE; button : _FRAMETYPE; END_VAR;
FUNCTION GLOBAL __cdecl MessageBoxSetFontScheme VAR_INPUT fontschemename : ^_ASCII; END_VAR;


// T_PICADD.CPP *************************************************************
FUNCTION GLOBAL __cdecl add_object VAR_INPUT p0 : ^_MY_PICTURE; x1 : UINT; p2 : ^_ROOM; x3 : UDINT := DEFSCOPE; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl add_text VAR_INPUT mp : ^_MY_PICTURE; pstyle : ^_STYLESHEET; pr : ^_ROOM; stschemeno : udint; pany : ^_ANYPARA; txt : ^void; chrsize : uint; overload : udint := DEFSCOPE; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl add_rectangle VAR_INPUT mp : ^_MY_PICTURE; pstyle : ^_STYLESHEET; pr : ^_ROOM; overload : udint := DEFSCOPE; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl add_button 
VAR_INPUT 
  mp : ^_MY_PICTURE; 
  pstyle : ^_STYLESHEET; 
  pr : ^_ROOM; 
  stschemeno : udint; 
  fktno_press : udint;
  fktno_release : udint;
  varno : udint;
  imageno_press : udint;
  imageno_release : udint;
  text_press : ^void;
  text_release : ^void;
  chrsize : uint; 
  act_momentary : bool;
  accessno : udint;
  hotkeycode : udint;
  overload : udint := DEFSCOPE;
  setval : dint := 1;
  resetval : dint := 0;
  imagealign : _ATTRIB := 16#FFFFFFFF;
END_VAR
VAR_OUTPUT 
  retcode : bool;
END_VAR;

FUNCTION GLOBAL __cdecl add_key
VAR_INPUT
  mp : ^_MY_PICTURE;
  pstyle : ^_STYLESHEET;
  pr : ^_ROOM;
  imageno : udint;
  text : ^void;
  chrsize : uint;
  imagealign : _ATTRIB;
  keycode : uint; 
  keycode_shift : uint;
  keycode_altgr : uint := _NOKEY;
  keycode_altgr_shift : uint := _NOKEY;
END_VAR
VAR_OUTPUT 
  retcode : bool;
END_VAR;

FUNCTION GLOBAL __cdecl add_output VAR_INPUT mp : ^_MY_PICTURE; pstyle : ^_STYLESHEET; pr : ^_ROOM; stschemeno : udint; varno : udint; iotype : _IO_TYPE; overload : _UDWORD := DEFSCOPE; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl add_input VAR_INPUT mp : ^_MY_PICTURE; pstyle : ^_STYLESHEET; pr : ^_ROOM; stschemeno : udint; varno : udint; iotype : _IO_TYPE; overload : _UDWORD := DEFSCOPE; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl add_image VAR_INPUT mp : ^_MY_PICTURE; pr : ^_ROOM; imageno : udint; pbmp : ^_BMP; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl add_carousel VAR_INPUT mp : ^_MY_PICTURE; room : ^_ROOM; cino : UINT; ci : ^_CAROITEM; varno_pos : UDINT; horizontal : bool; infinite : bool; _dx : UINT; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
FUNCTION GLOBAL __cdecl add_carousel_scrollbar VAR_INPUT mp : ^_MY_PICTURE; cno : dint; frame:_FRAMETYPE; col_back:_COLOR; col_front:_COLOR; col_symbol:_COLOR; col_topleft:_COLOR; col_bottomright:_COLOR; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl get_carousel VAR_INPUT mp : ^_MY_PICTURE; index : DINT; END_VAR VAR_OUTPUT retcode : ^_MOTION; END_VAR;
FUNCTION GLOBAL __cdecl add_imagescheme VAR_INPUT mp : ^_MY_PICTURE; pr : ^_ROOM; imageschemeno : udint; stschemeno : udint; varno : udint; overload : _UDWORD := DEFSCOPE; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl add_myio VAR_INPUT mp : ^_MY_PICTURE; pr : ^_ROOM; stschemeno : udint; varno : udint; input : bool; objectname : ^_ASCII; overload : _UDWORD := DEFSCOPE; END_VAR VAR_OUTPUT retcode : bool; END_VAR;

// T_PICPIC.CPP *************************************************************
FUNCTION GLOBAL __cdecl CyclicButton VAR_INPUT p0 : ^_EVENT; dashbrd : UDINT; globscr : UDINT; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;

// T_PICSUB.CPP *************************************************************
FUNCTION GLOBAL __cdecl get_visible_objectindex VAR_INPUT firstindex : ^dint; lastindex : ^dint; pmotion : ^_MOTION; END_VAR;
FUNCTION GLOBAL __cdecl get_pixsize VAR_INPUT pmotion : ^_MOTION; objectindex : dint; END_VAR VAR_OUTPUT retcode : udint; END_VAR;
FUNCTION GLOBAL __cdecl SetCallUserOverload VAR_INPUT p0 : ^void; END_VAR;
FUNCTION GLOBAL __cdecl SubPicture_Draw VAR_INPUT proom : ^_ROOM; pic : ^void; END_VAR;
FUNCTION GLOBAL __cdecl SubPicture_Run VAR_INPUT proom : ^_ROOM; pic : ^void; END_VAR;
FUNCTION GLOBAL __cdecl SubPicture_Copy VAR_INPUT pic : ^void; END_VAR VAR_OUTPUT retcode : ^void; END_VAR;
FUNCTION GLOBAL __cdecl SubPicture_Free VAR_INPUT pic : ^void; END_VAR;
FUNCTION GLOBAL __cdecl SubPicture_Overload VAR_INPUT pic : ^void; vl : ^_VARLABEL; END_VAR;
FUNCTION GLOBAL __cdecl SubPicture_OverloadSummary VAR_INPUT pic : ^void; summary : UINT; vl : ^_VARLABEL; END_VAR;

// T_POPSUB.CPP *************************************************************
#ifdef OK_SUBLIST
FUNCTION GLOBAL __cdecl add_entries_MEM_VKLIST VAR_INPUT p0 : ^_MEM_VKLIST; x1 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl add_MEM_VKLIST VAR_INPUT p0 : ^_MEM_VKLIST; x1 : UDINT; x2 : UDINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl del_MEM_VKLIST VAR_INPUT p0 : ^_MEM_VKLIST; x1 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl find_real_position VAR_INPUT p0 : ^_MEM_VKLIST; x1 : UDINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl action_MEM_VKLIST VAR_INPUT p0 : ^_MEM_VKLIST; x1 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl out_MEM_VKLIST VAR_INPUT p0 : ^_ROOM; p1 : ^_VKLIST; x2 : _COLOR; x3 : BOOL; p4 : ^_MEM_VKLIST; END_VAR VAR_OUTPUT retcode : _KOORD; END_VAR;
FUNCTION GLOBAL __cdecl is_action_MEM_VKLIST VAR_INPUT p0 : ^_MEM_VKLIST; x0 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl popup_get_actual_position VAR_INPUT p0 : ^void; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl find_line VAR_INPUT p0 : ^_MEM_VKLIST; x0 : UDINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl open_entire_MEM_VKLIST VAR_INPUT p0 : ^_MEM_VKLIST; END_VAR;
FUNCTION GLOBAL __cdecl close_entire_MEM_VKLIST VAR_INPUT p0 : ^_MEM_VKLIST; END_VAR;
#endif

// T_POPUP.CPP **************************************************************

// T_POS.CPP ****************************************************************
#ifdef USER_BILLION
FUNCTION GLOBAL __cdecl InvertInputColor VAR_INPUT varno : udint; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
#endif

// T_PRJ.CPP ****************************************************************
FUNCTION GLOBAL __cdecl Prj_GetNo VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl Prj_GetEntryByNo VAR_INPUT x0 : UINT; x1 : _ENTRYTYPE; END_VAR VAR_OUTPUT retcode : ^_PRJ_ENTRY; END_VAR;
FUNCTION GLOBAL __cdecl Prj_GetEntry VAR_INPUT x0 : UINT; END_VAR VAR_OUTPUT retcode : ^_PRJ_ENTRY; END_VAR;
FUNCTION GLOBAL __cdecl Prj_GetLanguageNo VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl Prj_GetLanguages VAR_OUTPUT retcode : ^_ASCII_BUFFER; END_VAR;
FUNCTION GLOBAL __cdecl Prj_Get_LANGUAGE_INFO VAR_OUTPUT retcode : ^_LANGUAGE_INFO; END_VAR;
FUNCTION GLOBAL __cdecl Prj_SwapLanguage VAR_INPUT state : BOOL; Language_no : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Prj_HostLanguageSet VAR_INPUT language:udint; END_VAR VAR_OUTPUT retcode:bool; END_VAR;
FUNCTION GLOBAL __cdecl Prj_HostLanguageSetEx VAR_INPUT idx:udint; language:udint; END_VAR VAR_OUTPUT retcode:bool; END_VAR;
FUNCTION GLOBAL __cdecl Prj_HostLanguageGetEx VAR_INPUT idx:udint; END_VAR VAR_OUTPUT retcode:udint; END_VAR;
FUNCTION GLOBAL __cdecl Prj_HostLanguageGet VAR_OUTPUT retcode:udint; END_VAR;
FUNCTION GLOBAL __cdecl Prj_HostLanguagePush VAR_OUTPUT retcode:bool; END_VAR;
FUNCTION GLOBAL __cdecl Prj_HostLanguagePop VAR_OUTPUT retcode:bool; END_VAR;
FUNCTION GLOBAL __cdecl Prj_HostLanguagePushEx VAR_INPUT idx:udint; END_VAR VAR_OUTPUT retcode:bool; END_VAR;
FUNCTION GLOBAL __cdecl Prj_HostLanguagePopEx VAR_OUTPUT retcode:bool; END_VAR;
FUNCTION GLOBAL __cdecl Prj_GetLseProjectCrc VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl Prj_GetProjectInfo VAR_OUTPUT retcode : ^_PROJECT; END_VAR;
FUNCTION GLOBAL __cdecl Prj_GetProjectEntryName VAR_INPUT pe:^_PRJ_ENTRY; END_VAR VAR_OUTPUT retcode:^char; END_VAR;

// T_SAVE.CPP ***************************************************************

// T_SCAN.CPP ***************************************************************
FUNCTION GLOBAL __cdecl ScanBar_Add VAR_INPUT p0 : ^_ASCII; x0 : DINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl ScanBar_Run VAR_INPUT x0 : UINT; x1 : DINT; END_VAR;
FUNCTION GLOBAL __cdecl ScanBar_End VAR_INPUT x0 : UINT; END_VAR;

// T_SCHEMA.CPP *************************************************************
FUNCTION GLOBAL __cdecl get_textschemeentry VAR_INPUT no : UINT; pvl : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : ^_ANYTHING; END_VAR;
FUNCTION GLOBAL __cdecl ImageScheme_Add VAR_INPUT label : ^_ASCII; pscheme : ^_SCHEME; END_VAR VAR_OUTPUT retcode : udint; END_VAR;

// T_SCROLL.CPP *************************************************************
FUNCTION GLOBAL __cdecl Start_SCROLL VAR_INPUT p0 : ^_SCROLL; x1 : BOOL; END_VAR ;
FUNCTION GLOBAL __cdecl Ready_SCROLL VAR_INPUT p0 : ^_SCROLL; x1 : BOOL; END_VAR ;
FUNCTION GLOBAL __cdecl Run_SCROLL VAR_INPUT p0 : ^_SCROLL; p1 : ^_EVENT; END_VAR ;
FUNCTION GLOBAL __cdecl End_SCROLL VAR_INPUT p0 : ^_SCROLL; END_VAR ;
FUNCTION GLOBAL __cdecl GetLinePosition_SCROLL VAR_INPUT p0 : ^_SCROLL; x1 : UDINT; p2 : ^_ROOM; END_VAR ;
FUNCTION GLOBAL __cdecl OutLine_SCROLL VAR_INPUT p0 : ^_SCROLL; x1 : UDINT; x2 : UDINT; x3 : BOOL; x4 : BOOL; END_VAR ;
FUNCTION GLOBAL __cdecl ActBeam_SCROLL VAR_INPUT p0 : ^_SCROLL; x1 : BOOL; END_VAR ;
FUNCTION GLOBAL __cdecl Beam_SCROLL VAR_INPUT p0 : ^_SCROLL; x1 : UDINT; x2 : UDINT; x3 : BOOL; END_VAR ;
FUNCTION GLOBAL __cdecl OutPage_SCROLL VAR_INPUT p0 : ^_SCROLL; END_VAR ;
FUNCTION GLOBAL __cdecl OutPageBeam_SCROLL VAR_INPUT p0 : ^_SCROLL; x0 :UDINT; END_VAR;
FUNCTION GLOBAL __cdecl Draw_SCROLL VAR_INPUT p0 : ^_SCROLL; x1 : BOOL; END_VAR ;
FUNCTION GLOBAL __cdecl ScrollUp_SCROLL VAR_INPUT p0 : ^_SCROLL; END_VAR ;
FUNCTION GLOBAL __cdecl ScrollDown_SCROLL VAR_INPUT p0 : ^_SCROLL; END_VAR ;
FUNCTION GLOBAL __cdecl GetPosition_SCROLL VAR_INPUT p0 : ^_SCROLL; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl level_draw VAR_INPUT p0 : ^_LEVEL; x1 : BOOL; x2 : BOOL; x3 : ^_BMP := NIL; frame : _FRAMETYPE := _3DFRAME; END_VAR ;
FUNCTION GLOBAL __cdecl level_run VAR_INPUT p0 : ^_LEVEL; x1 : DINT; x2 : DINT; x3 : BOOL; x4 : ^_BMP := NIL; frame : _FRAMETYPE := _3DFRAME; END_VAR ;
FUNCTION GLOBAL __cdecl level_out VAR_INPUT p0 : ^_LEVEL; x0 : UDINT; pos : DINT; horizontal : BOOL; x4 : ^_BMP := NIL; frame : _FRAMETYPE := _3DFRAME; END_VAR;
FUNCTION GLOBAL __cdecl do_HID_SCROLL VAR_INPUT p0 : ^_EVENT; p1 : ^_LEVEL; p2 : ^_POSITION; p3 : ^_LEVEL; p4 : ^_POSITION; p5 : ^_ROOM; x6 : UINT; x7 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl track_HID_SCROLL VAR_INPUT p0 : ^_EVENT; p1 : ^_LEVEL; p2 : ^_POSITION; p3 : ^_ROOM; x4 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl is_HID_SCROLL VAR_INPUT p0 : ^_EVENT; p1 : ^_SCROLL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl chk_position_SCROLL VAR_INPUT p0 : ^_SCROLL; END_VAR ;
FUNCTION GLOBAL __cdecl calculate_pageheight VAR_INPUT p0 : ^_POSITION; p1 : ^_ROOM; x2 : USINT; END_VAR VAR_OUTPUT retcode : USINT; END_VAR;

// T_SCRSVR.CPP *************************************************************

// T_SEEK.CPP ***************************************************************

// T_SETT.CPP ***************************************************************
FUNCTION GLOBAL __cdecl IpcSettings_Save VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl get_delay_screensave VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl set_delay_screensave VAR_INPUT x0 : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl get_actual_language VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl set_actual_language VAR_INPUT x0 : UINT; END_VAR;
FUNCTION GLOBAL __cdecl get_commasign VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl set_commasign VAR_INPUT x0 : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl push_commasign VAR_INPUT state : BOOL; END_VAR VAR_OUTPUT retcode:BOOL; END_VAR;
FUNCTION GLOBAL __cdecl pop_commasign VAR_INPUT state : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl get_americandateformat VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl set_americandateformat VAR_INPUT x0 : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl get_americantimeformat VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl set_americantimeformat VAR_INPUT x0 : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl get_seek_algorithm VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl set_seek_algorithm VAR_INPUT x0 : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl get_directiononenter VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl set_directiononenter VAR_INPUT x0 : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl get_cursorwraphorizontal VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl set_cursorwraphorizontal VAR_INPUT x0 : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl get_cursorwrapvertical VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl set_cursorwrapvertical VAR_INPUT x0 : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl get_noexterncom VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl set_noexterncom VAR_INPUT x0 : BOOL; END_VAR;

// T_SETUP.CPP **************************************************************
FUNCTION GLOBAL __cdecl GetEditColorBack VAR_INPUT x0 : _EDITSTATE; END_VAR VAR_OUTPUT retcode : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl GetEditColorFrame VAR_INPUT x0 : _EDITSTATE; END_VAR VAR_OUTPUT retcode : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl GetEditColorTxt VAR_INPUT x0 : _EDITSTATE; END_VAR VAR_OUTPUT retcode : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl GetEditColorCursor VAR_INPUT x0 : _EDITSTATE; END_VAR VAR_OUTPUT retcode : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl GetTargetNotice VAR_OUTPUT retcode : ^_DATA; END_VAR;
FUNCTION GLOBAL __cdecl GetEditorIncDec VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl GetHotkeysInMenu VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl Get_SOFTKEYSETUP VAR_OUTPUT retcode : ^_SOFTKEYSETUP; END_VAR;
FUNCTION GLOBAL __cdecl Get_DEFKEYPAD VAR_OUTPUT retcode : ^_DEFKEYPAD; END_VAR;
FUNCTION GLOBAL __cdecl Get_SCREENSAVER VAR_OUTPUT retcode : ^_SCREENSAVER; END_VAR;
FUNCTION GLOBAL __cdecl Get_BUBBLEINFO VAR_OUTPUT retcode : ^_BUBBLEINFO; END_VAR;
FUNCTION GLOBAL __cdecl Set_BUBBLEINFO_delaytime VAR_INPUT x0 : DINT; END_VAR;
FUNCTION GLOBAL __cdecl Set_BUBBLEINFO_remaintime VAR_INPUT x0 : DINT; END_VAR;
FUNCTION GLOBAL __cdecl SetDoubleClickOption VAR_INPUT state : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl GetDoubleClickOption VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl SetChkBoxImages VAR_INPUT unchecked : udint; checked : udint; END_VAR;
FUNCTION GLOBAL __cdecl SetRadioBoxImages VAR_INPUT unchecked : udint; checked : udint; END_VAR;
FUNCTION GLOBAL __cdecl SetWordWrap VAR_INPUT newstate : bool; END_VAR VAR_OUTPUT retcode : bool; END_VAR;

// T_SOFT.CPP ***************************************************************
FUNCTION GLOBAL __cdecl Get_SINGLESOFT VAR_INPUT ima : _IMA; no : UINT; floorno : UINT; softno : UINT; END_VAR VAR_OUTPUT retcode : ^_SINGLESOFT; END_VAR;

// T_STACK.CPP **************************************************************

// T_TASK2.CPP **************************************************************
FUNCTION GLOBAL __cdecl RTK_Thread2 VAR_INPUT pline : ^_Thread2CmdLine; END_VAR;
FUNCTION GLOBAL __cdecl init_Thread2CmdLine VAR_INPUT pline : ^_Thread2CmdLine; END_VAR;
FUNCTION GLOBAL __cdecl Thread2_AddAsyncMethod VAR_INPUT pmethod : ^void; pthis : ^void; puserdata : ^void; userdatalength : udint; END_VAR VAR_OUTPUT retcode : bool; END_VAR;

// T_TEXT.CPP ***************************************************************
FUNCTION GLOBAL __cdecl gettext_ASCII_BUFFER VAR_INPUT p0 : ^_ASCII_BUFFER; no : UDINT; END_VAR VAR_OUTPUT retcode : ^_ASCII; END_VAR;
FUNCTION GLOBAL __cdecl gettext_TEXT_BUFFER VAR_INPUT p0 : ^_TEXT_BUFFER; no : UDINT; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;
FUNCTION GLOBAL __cdecl add_TEXT_BUFFER VAR_INPUT ptb : ^_TEXT_BUFFER; txt : ^void; chrsize : UINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl textpuffer_getPtr VAR_INPUT x0 : _ANYLST; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;
FUNCTION GLOBAL __cdecl textpuffer_getSize VAR_INPUT x0 : _ANYLST; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
FUNCTION GLOBAL __cdecl textpuffer_getNo VAR_INPUT x0 : _ANYLST; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl textpuffer_getText VAR_INPUT x0 : _ANYLST; x1 : UDINT; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;

// T_THEME.CPP **************************************************************
FUNCTION GLOBAL __cdecl draw_THEME_partial VAR_INPUT room : ^_ROOM; roompart : ^_ROOM; frametype : _FRAMETYPE; attrib : _ATTRIB; color : _COLOR; END_VAR;
FUNCTION GLOBAL __cdecl GetFrameType VAR_INPUT frame1 : _FRAMETYPE; frame2 : _FRAMETYPE; frame3 : _FRAMETYPE; defaultframe : _FRAMETYPE := _3DFRAME; END_VAR VAR_OUTPUT retcode : _FRAMETYPE; END_VAR;
FUNCTION GLOBAL __cdecl has_theme_cancel VAR_INPUT frameid : _UDWORD; END_VAR VAR_OUTPUT retcode : bool; END_VAR;

// T_TOOL.CPP ***************************************************************
FUNCTION GLOBAL __cdecl get_TOOLDATA VAR_OUTPUT retcode : ^_TOOLDATA; END_VAR;
FUNCTION GLOBAL __cdecl ToolCatTeach VAR_INPUT p0 : ^_ASCII; x1 : _TOOLSTATE; p2 : ^_STATISTIC; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl save_toolcat VAR_INPUT p0 : ^_ASCII; p1 : ^UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl load_toolcat VAR_INPUT p0 : ^_ASCII; p1 : ^UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;

// T_URGE.CPP ***************************************************************

// T_USER.CPP ***************************************************************
FUNCTION GLOBAL __cdecl recalculate VAR_INPUT p0 : ^_RESULT; p1 : ^_VARIABLE; x2 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl zero_screensaver;
FUNCTION GLOBAL __cdecl reset_screensaver;
FUNCTION GLOBAL __cdecl Beep VAR_INPUT whobeep : _WHOBEEP; END_VAR;

// T_VALID.CPP **************************************************************

// T_VARIA.CPP **************************************************************
FUNCTION GLOBAL __cdecl VarList_GetVarInfo VAR_INPUT p0 : ^_VARINFO; x1 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetSingleVariable VAR_INPUT p0 : ^_VARIABLE; x1 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetVariable VAR_INPUT p0 : ^_VARIABLE; p1 : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetSystemData VAR_INPUT p0 : ^_RESULT; p1 : ^_VARIABLE; p2 : ^_VARLABEL; x3 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetVariableSize VAR_INPUT p0 : ^_VARINFO; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_SetSystemData VAR_INPUT p0 : ^_RESULT; p1 : ^_VARIABLE; p2 : ^_VARLABEL; x3 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetValueByNo VAR_INPUT x0 : UDINT; p1 : ^DINT; x2 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetValue VAR_INPUT p0 : ^_VARLABEL; p1 : ^DINT; x2 : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_SetValueByNo VAR_INPUT x0 : UDINT; x1 : DINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_SetStringByNo VAR_INPUT varno:udint; txt:^void; chrsize:udint; END_VAR VAR_OUTPUT retcode:bool; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetStringByNo VAR_INPUT dst:^void; varno:udint; chrsize:udint; END_VAR VAR_OUTPUT retcode:^void; END_VAR;
FUNCTION GLOBAL __cdecl VarList_SetValue VAR_INPUT p0 : ^_VARLABEL; p1 : DINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetCompleteNoDigits VAR_INPUT p0 : ^_VARINFO; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetLabel VAR_INPUT p0 : ^_ASCII; p1 : ^USINT; x2 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetVarNo VAR_INPUT p0 : ^_ASCII; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetVarNoStation VAR_INPUT p0 : ^_ASCII; station : UINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_VOWPRUIC VAR_INPUT x0 : UINT; x1 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetParameter VAR_INPUT x0 : UDINT; x1 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetText VAR_INPUT x0 : UDINT; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetDigits VAR_INPUT p0 : ^_VARINFO; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetSign VAR_INPUT p0 : ^_VARINFO; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetDezPoint VAR_INPUT p0 : ^_VARINFO; END_VAR VAR_OUTPUT retcode : DINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetNo VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetTextByNo VAR_INPUT x0 : UDINT; x1 : UINT; END_VAR VAR_OUTPUT retcode : ^_CHAR; END_VAR;
//FUNCTION GLOBAL __cdecl VarList_CreateObject VAR_INPUT x0 : UINT; p1 : ^_ASCII; p2 : ^_ASCII; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
//FUNCTION GLOBAL __cdecl VarList_ConnectObject VAR_INPUT x0 : UINT; p1 : ^_ASCII; p2 : ^_ASCII; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
//FUNCTION GLOBAL __cdecl VarList_StartObject VAR_INPUT x0 : UINT; p1 : ^_ASCII; x2 : UDINT; x3 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
//FUNCTION GLOBAL __cdecl VarList_DeleteObject VAR_INPUT x0 : UINT; p1 : ^_ASCII; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
//FUNCTION GLOBAL __cdecl VarList_VisualizeObject VAR_INPUT x0 : UINT; p1 : ^_ASCII; p2 : ^_ASCII; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetUnitInfo VAR_INPUT x0 : UINT; END_VAR VAR_OUTPUT retcode : ^_UNIT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_SetValueSub VAR_INPUT p0 : ^_RESULT; p1 : ^_VARIABLE; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetUnitNo VAR_INPUT p0 : ^_ASCII; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetVarNoByID VAR_INPUT x0 : UDINT; x1 : UINT; x2 : BOOL; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetVarIdChanged VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetVarSymbol VAR_INPUT p0 : ^void; x1 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_ReGetID VAR_INPUT x0 : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl chk_server_XCHKBIT VAR_INPUT x0 : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl chk_server_stscheme VAR_INPUT x0 : UDINT; END_VAR VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetVarNoByCRC VAR_INPUT x0 : _CRC32; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_Get_LSEVARINFO VAR_INPUT no : UDINT; END_VAR VAR_OUTPUT retcode : ^_LSEVARINFO; END_VAR;
FUNCTION GLOBAL __cdecl VarList_ChangeBaseAddress VAR_INPUT varno : UDINT; address : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_CheckHostname VAR_INPUT do_scanbar : BOOL; stop_at_double_name : BOOL; reload_if_damaged : BOOL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetHostName VAR_INPUT label : ^_ASCII; station : UINT; END_VAR VAR_OUTPUT retcode : ^_ASCII; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetHostNameByVarNo VAR_INPUT varno : UDINT; END_VAR VAR_OUTPUT retcode : ^_ASCII; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetHostNameByCrc VAR_INPUT crc32 : UDINT; END_VAR VAR_OUTPUT retcode : ^_ASCII;  END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetVarNoByHostName VAR_INPUT hostname : ^_ASCII; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetOnlyAscii VAR_INPUT x0 : ^_VARINFO; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl VarList_FindRoot VAR_INPUT x0 : UDINT; END_VAR VAR_OUTPUT retcode : UDINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_MakeLasalId VAR_INPUT pva : ^_LSEVARINFO; label : ^_ASCII; END_VAR VAR_OUTPUT retcode : bool; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetServerToEdit VAR_OUTPUT retcode:UDINT; END_VAR;
FUNCTION GLOBAL __cdecl VarList_GetUnitIndex var_input varno:udint; end_var var_output retcode:udint; end_var; // sa36634
FUNCTION GLOBAL __cdecl VarList_GetUnitLabel var_input unitindex:udint; end_var var_output retcode:^char; end_var; // sa36634

// T_WATCH.CPP **************************************************************
FUNCTION GLOBAL __cdecl ServerWatch_Add VAR_INPUT svrno : UDINT; pthis : ^void; pfkt : ^void; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl ServerWatch_Del VAR_INPUT svrno : UDINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;

// T_WIN.CPP ****************************************************************
FUNCTION GLOBAL __cdecl InsertCmd VAR_INPUT x0 : _PIC_COMMAND; x1 : DINT; x2 : DINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl AddCmd VAR_INPUT x0 : _PIC_COMMAND; p1 : ^_VARLABEL; p2 : ^_VARLABEL; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl SaveEditor VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl CloseEditor VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl picmemo_free;
FUNCTION GLOBAL __cdecl PicMemo_GetNo VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl PicMemo_Get VAR_OUTPUT retcode : UINT; END_VAR;
FUNCTION GLOBAL __cdecl set_keytable VAR_INPUT p0 : ^void; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl set_keymode VAR_INPUT code : UDINT; tip : _TIP; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl RefreshJustTopWindow VAR_INPUT x0 : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl DrawPicWin VAR_INPUT x0 : _IMA; x1 : UINT; x2 : UINT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl DrawPicWinOffset VAR_INPUT x0 : _IMA; x1 : UINT; x2 : UINT; poff : ^_DOT; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl do_start;

// LSE class ****************************************************************
FUNCTION GLOBAL __cdecl Lse_Reset VAR_INPUT restart : bool; END_VAR;
FUNCTION GLOBAL __cdecl Thread2_AddCmd VAR_INPUT pcmd : ^_Thread2CmdLine; END_VAR VAR_OUTPUT retcode : bool; END_VAR;


#define PROFILE_START 0x00000000
#define PROFILE_END   0x00000100

#define CLASS_LSE                          0x00000000
#define CLASS_GLOBAL                       0x00010000
#define CLASS_SCREEN                       0x00020000
#define CLASS_MYIO                         0x00030000
#define CLASS_TOOLCAT                      0x00040000

#define METHOD_LSE_START                   1
#define METHOD_LSE_READY                   2
#define METHOD_LSE_RUN                     3
#define METHOD_LSE_END                     4
#define METHOD_LSE_FCAWL                   5
#define METHOD_LSE_ACCESSINPUT             6
#define METHOD_LSE_CHKENABLE               7
#define METHOD_LSE_SYSTEMERROR             8
#define METHOD_LSE_RECALCULATE             9
#define METHOD_LSE_BUBBLEHELP             10
#define METHOD_LSE_URINIT                 11
#define METHOD_LSE_PROJECTSTART           12
#define METHOD_LSE_PROJECTREADY           13
#define METHOD_LSE_INPUTSAVED             14
#define METHOD_LSE_POWERDOWN              15
#define METHOD_LSE_INPUTNOACCESS          16
#define METHOD_LSE_OPENEDITOR             17
#define METHOD_LSE_WRITE                  18
#define METHOD_LSE_CHECKPREFIX            19
#define METHOD_LSE_GETLOOKUPEMB           20
#define METHOD_LSE_SETSTATUS              21
#define METHOD_LSE_TRACE                  22
#define METHOD_LSE_DRAWHEAP               23
#define METHOD_LSE_FOUNDHID               24
#define METHOD_LSE_CALLPOPUP              25
#define METHOD_LSE_GETTHISPOINTER         26
#define METHOD_LSE_OPENTOUCHEDITOR        27
#define METHOD_LSE_ACCESSBUTTON           28
#define METHOD_LSE_GETSTATUS              29
#define METHOD_LSE_LANGUAGEHASCHANGED     30
#define METHOD_LSE_LANGUAGEWILLCHANGE     31
#define METHOD_LSE_OFFLINE                32
#define METHOD_LSE_OVERLOADEDITOR         37
#define METHOD_LSE_CONFIGDRIVELIST        38
#define METHOD_LSE_ONLINE                 39
#define METHOD_LSE_TRY_TO_ACCESS          40
#define METHOD_LSE_CHECK_HEAP             41
#define METHOD_LSE_SWITCH_MULTICPU        42
#define METHOD_LSE_DOCMDINTERFACE         43
#define METHOD_LSE_SCRSVR_START           44
#define METHOD_LSE_SCRSVR_END             45
#define METHOD_LSE_BEEP                   46
#define METHOD_LSE_IPCINI                 47
#define METHOD_LSE_SOFTRESET              48
#define METHOD_LSE_USERSTATION            49
#define METHOD_LSE_NOWRITEBECAUSEACCESS   50
#define METHOD_LSE_PROJECTEND             51
#define METHOD_LSE_PROJECTRUN             52
#define METHOD_LSE_WRITE_POST             53
#define METHOD_LSE_WINDOW_MOVEABLE        54

#define METHOD_MYIO_GETFOCUS                  0
#define METHOD_MYIO_KILLFOCUS                 1
#define METHOD_MYIO_GETEVENT                  2
#define METHOD_MYIO_DRAW                      3
#define METHOD_MYIO_GETDATASUB                4
#define METHOD_MYIO_START                     5
#define METHOD_MYIO_READY                     6
#define METHOD_MYIO_RUN                       7
#define METHOD_MYIO_END                       8
#define METHOD_MYIO_SAVEEDITOR                9
#define METHOD_MYIO_DISPLAYCURSOR            10
#define METHOD_MYIO_SYSTEMISREADYWITHEDITOR  11
#define METHOD_MYIO_ACCESSIBLE               12
#define METHOD_MYIO_GETDATA                  13
#define METHOD_MYIO_OPENTOUCHEDITOR          14

#define TOOLCAT_LOAD_START                    0
#define TOOLCAT_LOAD_RUN                      1
#define TOOLCAT_LOAD_END                      2
#define TOOLCAT_SAVE_START                    3
#define TOOLCAT_SAVE_RUN                      4
#define TOOLCAT_SAVE_END                      5
#define TOOLCAT_ERROR_OCCURED                 6

#define LSE_LANGUAGE                0
#define LSE_COMPILERVERSION         1
#define LSE_LSEVERSION              2
#define LSE_SCREENDELAY             3
#define LSE_COMMASIGN               4
#define LSE_AMERICANDATEFORMAT      5
#define LSE_SEEKALGORITHM           6
#define LSE_DIRECTIONONENTER        7
#define LSE_OPSYSVERSION            8
#define LSE_CURSORWRAPVERTICAL      9
#define LSE_CURSORWRAPHORIZONTAL   10
#define LSE_OFFLINEMODE            11
#define LSE_NOEXTERNCOM            12
#define LSE_AMERICANTIMEFORMAT     13
#define LSE_ACTIVATEBUBBLEHELP     14
#define LSE_LSEREVISION            15

#define T_TYPES_H
#endif
