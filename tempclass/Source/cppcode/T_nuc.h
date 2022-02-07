#ifndef _T_NUC_H
#define _T_NUC_H

//#define NINETEEN  9999

#include "T_COLOR.H"
#define T_SHADOW   T_STRIKEOUT       // STRIKEOUT Attribut wird nicht mehr verwendet --> 3deffekt bei Text

#define UC_SOFTKEY
//#define MENUE_CURSOR_ALT          // don't use global cursor-color-settings
//#define FIRSTINPUT_ALT
//#define UC_RECALC           // recalculation in _MYIO::Draw (old procedure)

#define COMPILER_VERSION_MIN 59  // ab dieser compilerversion funktioniert dieser kernel, muss grösser gleich 59 sein
#define COMPILER_VERSION     60  // muss grösser gleich 30 sein !!! don't forget about comment in class _Lse
#define _LSE_VERSION     0x117E

#ifdef __TURBOC__
 #ifdef _CHANGELOOKUP
  #undef _CHANGELOOKUP // geht nur bei ms-compiler
 #endif
#endif

#define PROJECTINIFILE "RAM@C:\\IPC.INI"
#define ARRAY_SIZE(p) (sizeof(p)/sizeof(p[0]))


// LASAL      = compiled with lasal - microsoft, gnu, .....
// _MSC_VER   = microsoft
// _GNUC      = gnu
// __TURBOC__ = borland

#define DEFSVRTYP -1

#define SA32402

#ifdef _MSC_VER
 #ifndef _CCOMPILER
  #define _CCOMPILER
 #endif
#endif

#ifdef _GNUC
 #ifndef _CCOMPILER
  #define _CCOMPILER
 #endif
#endif

#ifdef __TURBOC__
 #ifndef _CCOMPILER
  #define _CCOMPILER
 #endif
#endif

#ifdef _CCOMPILER

#ifndef ACCESS_DENIED
 #define ACCESS_DENIED   0x80000010
#endif

// **************************************************************************
// **************************************************************************
// ***  sigmatek configuration (user keep off !!)                         ***
// **************************************************************************
// **************************************************************************

// MACROS *******************************************************************

#ifdef _LSL_TARGETARCH_ARM
  #define load_this(atmp)  asm volatile("mov r9, %0" : : "r" (atmp));
#else
  #ifdef _MSC_VER
	#define load_this(atmp) __asm mov esi, atmp
  #elif _GNUC
	#define load_this(atmp) asm("" : : "S" (atmp));
  #else
	#define load_this(atmp) // __asm mov esi, atmp
  #endif
#endif

// ENUMERATED DATATYPES *****************************************************


#ifdef LASAL
	#pragma pack (push, 1)
#endif

typedef unsigned char _BOOL;
#define _FALSE       0
#define _TRUE        1

typedef unsigned char _VARTYPE;                       // varaiablentypes
#define BINARY       0  // binary
#define FSTRING      1  // string
#define FTIME        2  // time
#define FDATE        3  // date
#define XRADIO       4  // nc
#define BIT          5  // bit datatype
#define XBITMASK     6  // nc
#define COMPLEX      7  // structure, complex
#define FREE         8  // nc
#define FCONSTANT    9  // constant type
#define UNUSED      10  // nc
#define OBJECT      11  // objecttype
#define UNUSED2     12  // im lse reserviert
#define FMERKER     13  // merker
#define FENUM       14  // enum
#define FENUMITEM   15  // enumitem

#define PRINTSCREEN_IN_USE  0x7FFFFFFF

#ifdef LASAL
	#pragma pack (pop)
#endif

#ifdef LASAL
  #pragma pack (push, 4)
#endif

typedef enum   { SL_UNDEF,
                 SL_LAYER,
                 SL_OBJECT,
                 SL_MOTION,
                 SL_INPUT,
                 SL_BUTTON,
                 SL_OUTPUT,
                 SL_KARU,
               }
_LOCATION;

typedef enum   { IMA_PICTURE,
                 IMA_WINDOW,
                 IMA_GLOBAL,
                 IMA_POPUP,	
                 IMA_SCREENSAVER,
                 IMA_TOUCHEDITOR,
                 IMA_OFF_PICTURE,
                 IMA_OFF_WINDOW,

                 IMA_NOTHING,
               }
_IMA;

typedef enum   { FOUND_BUTTON,
                 FOUND_INPUT,
                 FOUND_ESC,
                 FOUND_SOFTKEY,
                 FOUND_CANCLE,
               }
_FOUNDHID;

typedef enum   { BEEP_UNKNOWN,
                 BEEP_BUTTON,
                 BEEP_INPUT,
                 BEEP_SOFTKEY,
                 BEEP_SCROLLSELECT,
                 BEEP_SCROLLCONFIRM, 
                 BEEP_LEVEL,        // hidlevel
                 BEEP_ESC,          // close window (hid outside)
                 BEEP_X,            // canclebutton of theme
               }
_WHOBEEP;

typedef enum   { NOTHING_FAILED,          // unlimited access
                 CHKBIT_FAILED,           // chkbit
                 STATESCHEME_FAILED,      // statescheme
                 ACCESS_FAILED,           // accesslevel
                 USERIF_FAILED            // userinterface
               }
_ACCESSFAILED;

typedef enum   { _COM_LOCAL,        // local data (mae local)
                 _COM_INTERN,       // intern data (2'nd task)
                 _COM_COM1,         // extern data com1
                 _COM_COM2,         // extern data com2
                 _COM_COM3,         // extern data com3
                 _COM_COM4,         // extern data com4
                 _COM_CAN1,         // extern data can1
                 _COM_CAN2,         // extern data can2
                 _COM_TCPIP1,       // extern data tcpip1
                 // !!! Für nachfolgende TCPIP Verbindungen muß der _COMDEF Datentyp erweitert werden.
                 // Benötigte Elemente sind unter Kommentar.
                 _COM_TCPIP1_P=10,  // extern data tcpip1 + port
                 _COM_TCPIP2_P,     // extern data tcpip2 + port
                 _COM_COM5,         // extern data com5
                 _COM_COM6,         // extern data com6
                 _COM_COM7,         // extern data com7
                 _COM_COM8,         // extern data com8
                 _COM_COM9,         // extern data com9
                 _COM_COM10,        // extern data com10
                 _COM_CAN20=20,
                 _COM_CAN21,
                 _COM_CAN22,
                 _COM_CAN23,
                 _COM_CAN24,
                 _COM_CAN25,
                 _COM_CAN26,
                 _COM_CAN27,
                 _COM_CAN28,
                 _COM_CAN29,
               }
_INTERFACE;

typedef enum   { LOCAL_UNKNOWN             = 0x00,
                 LOCAL_ARABIC              = 0x01,
                 LOCAL_BULGARIAN           = 0x02,
                 LOCAL_CATALAN             = 0x03,
                 LOCAL_CHINESE             = 0x04,
                 LOCAL_CZECH               = 0x05,
                 LOCAL_DANISH              = 0x06,
                 LOCAL_GERMAN              = 0x07,
                 LOCAL_GREEK               = 0x08,
                 LOCAL_ENGLISH             = 0x09,
                 LOCAL_SPANISH             = 0x0A,
                 LOCAL_FINNISH             = 0x0B,
                 LOCAL_FRENCH              = 0x0C,
                 LOCAL_HEBREW              = 0x0D,
                 LOCAL_HUNGARIAN           = 0x0E, 
                 LOCAL_ICELANDIC           = 0x0F,
                 LOCAL_ITALIAN             = 0x10,
                 LOCAL_JAPANESE            = 0x11,
                 LOCAL_KOREAN              = 0x12,
                 LOCAL_DUTCH               = 0x13,
                 LOCAL_NORWEGIAN           = 0x14,
                 LOCAL_POLISH              = 0x15,
                 LOCAL_PORTUGESE           = 0x16,
                 LOCAL_ROMANIAN            = 0x18,
                 LOCAL_RUSSIAN             = 0x19,
                 LOCAL_CROATIA_SERBIAN     = 0x1A,
                 LOCAL_SLOVAK              = 0x1B,
                 LOCAL_ALBANIAN_MACEDONIAN = 0x1C,
                 LOCAL_SWEDISH             = 0x1D,
                 LOCAL_THAI                = 0x1E,
                 LOCAL_TURKISH             = 0x1F,
                 LOCAL_URDU                = 0x20,
                 LOCAL_INDONESIAN          = 0x21,
                 LOCAL_UKRAINIAN           = 0x22,
                 LOCAL_BYELORUSSIAN        = 0x23,
                 LOCAL_SLOVENIAN           = 0x24,
                 LOCAL_ESTONIAN            = 0x25,
                 LOCAL_LATVIAN             = 0x26,
                 LOCAL_LITHUANIAN          = 0x27,
                 LOCAL_FARSI               = 0x29,
                 LOCAL_VIETNAMESE          = 0x2A,
                 LOCAL_PASQUE              = 0x2D,
                 LOCAL_AFRICAANS           = 0x36,
                 LOCAL_GEORGIAN            = 0x37,
                 LOCAL_FAEROESE            = 0x38,
                 LOCAL_HINDI               = 0x39
               }
_LOCAL;

typedef enum   { TxtID_OK,     
                 TxtID_CANCLE,
                 TxtID_YES,
                 TxtID_NO,
                 TxtID_WEEKDAYS,
               }
_STDTEXTID;

#define          TxtID_OFFLINE_TEXT     0xFFFC
#define          TxtID_OFFLINE_ENTER    0xFFFD
#define          TxtID_OFFLINE_ESC      0xFFFE




typedef enum   { MESSAGEBOXTYP_OK,
                 MESSAGEBOXTYP_OK_CANCLE,
                 MESSAGEBOXTYP_YES_NO
               }
_MESSAGEBOXTYP;

typedef enum   { MESSAGEBOXRESULT_NONE,
                 MESSAGEBOXRESULT_OK,
                 MESSAGEBOXRESULT_CANCLE,
                 MESSAGEBOXRESULT_YES,
                 MESSAGEBOXRESULT_NO
               }
_MESSAGEBOXRESULT;

typedef enum   { SELIS_NONE,
                 SELIS_DRIVE,
                 SELIS_ROOTLOCK,
                 SELIS_DIRECTORY,
                 SELIS_FILE
               }
_SELIS;

typedef enum   { _IDLE,         // nothing done
                 _IDIDIT,       // i did it
                 _IFAILED       // i tried but failed
               }
_DOIT;                          // method state

typedef enum   { _TIPCOMBI,     // autorepeat 0.5, 0.1
                 _TIPCONTI,     // continuous rating
                 _TIPSINGLE,    // singleshot
                 _TIPAUTO,      // autorepeat 0.1, 0.1
               }
_TIP;

typedef enum   { STATE_START,      // session start
                 STATE_READY,      // session ready
                 STATE_RUN,        // session run
                 STATE_END,        // session end
                 STATE_FINISH,     // session closed
                 STATE_BACKGROUND, // refresh background (_MOTION)
                 STATE_OVERLOAD,   // another overload try
                 STATE_ERROR,      // error
                 STATE_FLY         // download on the fly
               }
_WORKSTATE;

typedef enum   { RES_DEFAULT,   // default
                 RES_CONSTANT,  // constant value
                 RES_NUMERIC,   // numeric value
                 RES_REAL,      // real
                 RES_ANUMERIC,  // string, etc
                 RES_TIME,      // time
                 RES_DATE,      // date
               }
_RESULT_TYPE;                   // result datatypes

typedef enum   { LOCTXT,        // local picturetext
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
                 VARTXT1_TOKEN, // variablentext 01 oder token von variablenname
                 VARTXT1_OBJNAME, // variablentext 01 oder objektname (ohne .server)
                 VARLIMIT_HI,   // hilimit von variable
                 VARLIMIT_LO,   // lolimit von variable
                 PRJSCREEN,     // find screennumber by label, sa22147
                 PRJWINDOW,     // find windownumber by label, sa22147
                 STATESCHEME,   // link to statescheme
                 COLORSCHEME,   // link to colorscheme
                 FONTSCHEME,    // link to fontscheme
                 
                 BAR_IO,        // io as bar (rape)
                 BUTTON_IO,     // io as button (rape)


                 ALARMLIST   =  999,  // alarmlist
                 TXTLIST_VTX = 1000,
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
               }
_ANYLST;

// just to keep compatibility to old program-code
#define STX TXTLIST_STX
#define MTX TXTLIST_003
#define ATX TXTLIST_002
#define ZT1 TXTLIST_004
#define ZT2 TXTLIST_005
#define ZT3 TXTLIST_006
#define VTX TXTLIST_VTX

typedef enum   { AS_DEFAULT,      // default default
                 AS_NUMERIC,      // display io as numeric
                 AS_ASCII,        // display io as string
                 AS_CHKBOX,       // display box (bit)
                 AS_BAR,          // display bar
                 AS_TBO,          // text, bitmap or object
                 AS_DATE,         // display date
                 AS_TIME,         // display time
                 AS_MENU,         // display menue
                 AS_VARNAME,      // display name of variable
                 AS_USER,         // nc
                 AS_RADIO,        // radiobutton
                 AS_NUMERIC_HIDE, // value as stars
                 AS_ASCII_HIDE,   // string as stars
                 AS_THEURGE,
                 AS_IPADDRESS,    // ipaddress
                 AS_ENUMERATION,  // enumeration

                 AS_TXTSCHEME,    // display io as text from text-scheme
                 AS_INPUTBUTTON,  // cursorcontrolled touch-button (rape)
                 AS_INPUTBUTTON_WITOUT_CURSOR, // cursorcontrolled touch-button without flashing cursor
                 AS_OBJECTINPUT,  // io
               }
_IO_TYPE;                       // io displayformat

typedef enum	 { _TKEY,         // nc
                 _TSWITCH,      // nc
                 _TRADIO,       // nc
               }
_BUTTON_TYPE;                   // buttontype

typedef enum   { CMD_NOP,            // nothing (0)
                 CMD_START,          // start with first picture (0)
                 CMD_NEWPICRET,      // open new (previous) picture (0)
                 CMD_EXIT,           // exit window without backgroundrefresh (0)
                 CMD_CLOSE,          // close picture or window (0)
                 CMD_EDITNO,         // open editor with input[no] (2)

                 CMD_REDRAW,         // redraw picture or window (0)
                 CMD_OUTSOFTKEY,     // redraw softkey (0)

                 CMD_NEWPICTURE,     // open new picture (2)
                 CMD_NEWWINDOW,      // open new window (2)
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
                 CMD_REDRAWTEXT,     // redraw all text and io's
                 CMD_SCRNSCHEME,     // open screen via scheme
                 CMD_DIAGNOSTIC,     // open diagnostic window (0)
                 CMD_OVERLOAD_OBJ,   // overload just defined object (2)
                 CMD_OVERLOAD_SUMMARY, // overload object with defined summary (2)
                 CMD_LED,            // switch led on/off/blink (2)
                 CMD_FUNCTIONBLOCK,  // call functionblock (1)
                 CMD_EDITSERVER_HID, // call toucheditorwindow to edit spezified server (2)
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
                 CMD_RECALC_IOID,    // recalculate inputs in objectlist for editor (1)
                 CMD_RETURNQUEUE,    // return and keep (copy) all commands
                 CMD_AUTOSEEKINPUT,  // automatic seek first input
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
                 CMD_RETURNEDITOR,   // close just editorwindow (0)
                 CMD_UNIQUEWINDOW,   // close all window and open new one (2, windownumber, softfloor)
                 CMD_MESSAGEBOX,     // open confirmwindow (2, windownumber, messboxindex)
                 CMD_EDITSERVER_HID_WIN_EDITOR, // same cmd than CMD_EDITSERVER_HID but op2 will include editorwindownumber
                 CMD_ACKNOWLEDGEFLAG, // ist für sa27246 notwendig (1)
                 CMD_RESET_SCREENSAVER, // wenn screensaver aktiv ist wird dieser wieder zurückgesetzt
                 CMD_RETURNQUEUE_EX, // wird bei UC_MOVING_WINDOW benötigt (1)
               }
_PIC_COMMAND;                        // commands



typedef enum   { RUN_NOP,         // do nothing
                 RUN_CLOSE,       // close window and refresh background
                 RUN_EXIT,        // close window without refresh
                 RUN_CLOSE_EX,
               }
_RUN_STATE;                     // intern window-commands

typedef enum   { TEXT_DAYOFWEEK,
                 EMTY_TEXT,
                 REINITIALIZE_REFRESHLIST,  // refreshliste muss neu aufgebaut werden. als parameter wird stationsnummer übergeben
                 STX_R03,
                 STX_R04,
                 CANT_INITIALIZE_GRAPHIX,
                 CANT_LOAD_PROJECT,
                 STARTPICTURE_NOT_PRESENT,
                 CANT_LOAD_STARTPICTURE,
                 STX_R09, //PICTURE_NOT_PRESENT,
                 CANT_LOAD_PICTURE,
                 STX_R10,  //WINDOW_NOT_PRESENT,
                 CANT_LOAD_WINDOW,
                 CANT_LOAD_FONT,
                 TOO_LESS_MEMORY,
                 CANT_LOAD_GFX,
                 NUMVALUE_TOO_BIG,          // numeric input too big
                 NUMVALUE_TOO_SMALL,        // numeric input too small
                 INVALID_TIME,
                 INVALID_DATE,
                 CANT_WRITE_DATA,           // cant write data to local or plc
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
                 STX_R49,
               }
_SYSERROR;                            // order in system-textlist

typedef enum   { _EVENT_NONE,          // no event
                 _EVENT_KEYPRESS,      // key pressed
                 _EVENT_KEYRELEASE,    // key released
                 _EVENT_HIDMOVE,       // mouse, touch, lightpen moved
                 _EVENT_HIDPRESS,      // mouse, touch, lightpen pressed
                 _EVENT_HIDRELEASE,    // mouse, touch, lightpen released
                 _EVENT_USER1,         // userdefined
                 _EVENT_USER2,         // userdefined
                 _EVENT_SHUTTLEINC,    // shuttle right
                 _EVENT_SHUTTLEDEC,    // shuttle left
                 _EVENT_LASALOS,       // lasalos event
                
                 _EVENT_TRIGGER,       // triggersignal "tipcombi"
                 _EVENT_PREHIDPRESS,   // erster hid-press
                 _EVENT_ZOOM,          // flick zoom
               }
_EVENTTYPE;                           // eventtypes

typedef enum   { _EP_AUTO,
                 _EP_LEFT,            // + left
                 _EP_RIGHT,           // + right
                 _EP_UP,              // + up
                 _EP_DOWN,            // + down
                 _EP_NONE,            //
                 _EP_REMEMBER,        // remember position for CMD_ENTERPLUS
               }
_ENTER_PLUS;                          // cursorcontrol after enter

typedef enum   { _HID_LEFT,           // touch, left mousebutton
                 _HID_RIGHT,          // right mousebutton
                 _HID_MIDDLE,         // middle mousebutton
               }
_HID_BUTTON;                          // button

typedef enum   { P_NOP,               // nothing to do
                 P_SCROLLUP,          // scroll list up and redraw bottom line
                 P_SCROLLDOWN,        // scroll list down and redraw top line
                 P_SCROLLLEFT,
                 P_SCROLLRIGHT,
                 P_REDRAW,            // redraw whole page
                 P_CURSOR             // redraw actual position
               }
_POSRESULT;

typedef enum { LOAD,                  // open, read only
               SAVE,                  // open, write only (create new or overwrite existing)
               LOADSAVE,              // open read and write (create new or change existing)
               NOSTATE                // default
             }
_FILE_STATE;                          // file openstate

typedef enum { H_NOERROR,             // no error
               H_NEVEROPEN,           // handle never opend
               H_DEFINED,
               H_NOTOPEN,
               H_NOREAD,
               H_NOMEMORY,
               H_NOWRITE,
               H_WRONGLSE,
             }
_FILE_ERROR;                          // file errorstate

typedef enum   { _PE_NONE,            // default id
                 _PE_ALARMLIST,       // list-id alarmlist
                 _PE_VARLIST,         // list-id varlist
                 _PE_TEXTLIST,        // list-id textlist
                 _PE_OBJECTLIST,      // list-id objectlist
                 _PE_BMPLIST,         // list-id imagelist
                 _PE_FUNCTIONLIST,    // list-id functionlist
                 _PE_MENULIST,        // list-id menulist
                 _PE_FONTLIST,        // list-id fontlist
                 _PE_SEQUENCELIST,    // list-id sequencelist
                 _PE_PICTURE,         // list-id picture 
                 _PE_WINDOW,          // list-id window
                 _PE_COLORSCHEME,     // list-id colorsheme
                 _PE_IMAGESCHEME,     // list-id imagesheme
                 _PE_UNITSCHEME,      // list-id unitscheme
                 _PE_FONTSCHEME,      // list-id fontscheme
                 _PE_SCRNSCHEME,      // list-id screenscheme
                 _PE_STATESCHEME,     // list-id statescheme
                 _PE_TEXTSCHEME,      // list-id textscheme
                 _PE_OBJECTSCHEME,    // list-id-objectscheme
                 _PE_SETUP,           // list-id-setup
                 _PE_PALETTE,         // list-id color palette
                 _PE_HOTKEYLIST,      // list-id hotkey's
                 _PE_LANGUAGE,        // list-id language.txt
                 _PE_THEME,           // list-id theme
                 _PE_SERVERSCHEME,    // list-id serverscheme
                 _PE_GLOBALSCREEN,    // list-id globalscreen
               }
_ENTRYTYPE;                           // project-list-id

typedef enum { _EDITACTIVE,
               _EDITCHANGE,
               _SELECTGROUP,
               _EDITPASSIVE,
             }
_EDITSTATE;

typedef enum { BUBBLE_VIRGIN,
               BUBBLE_WATCH,
               BUBBLE_ONSCREEN,
               BUBBLE_FINISHED,
             }
_BUBBLE;

typedef enum { _LOGIN_INIT,
               _LOGIN_FALSE,
               _LOGIN_TRUE
             }
_LOGSTATE;

typedef enum { _URGE_USERDEFINED,
               _URGE_BAR            = 32768,
               _URGE_INDICATOR      = 32769,
               _URGE_CLOCK          = 32770,
               _URGE_SCALE          = 32771,
               _URGE_LANGUAGESELECT = 32772,
               _URGE_DELAYANDRETURN = 32773,
               _URGE_SLIDER         = 32774,
               _URGE_OSZI           = 65534,
               _URGE_UNDEFINED      = 65535,
             }
_URGETYPE;

typedef enum { TS_IDLE,
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
               TS_LOADEND,
             }
_TOOLSTATE;

typedef enum { TE_NOERROR,
               TE_TOOLESSMEMORY,
               TE_CANTOPEN,
               TE_CANTSAVEENTRY,            //
               TE_NO_STATESCHEME,           // ungültiges statescheme
               TE_NO_ACCESS,                // kein access
               TE_NO_CHKBIT,                // ungültiges chkbit
               TE_ERROR_LIMITS,             // grenzwertverletzung
               TE_UNKNOWN_SERVER,           // unbekannter server
               TE_UNKNOWN_LINE_FORMAT,      // unbekannter fileeintrag
               TE_FILTER_DOESNT_FIT,        // filter passt nicht
               TE_INVALID_CRC,
               TE_DISK_ERROR,               // medium full, ...
               TE_NO_TOOLCAT,               // no toolcat file
             }
_TOOLERROR;


typedef enum { FILEXT_UNKNOWN,
               FILEXT_BMP,
               FILEXT_GIF,
               FILEXT_BMO,
               FILEXT_JPG,
               FILEXT_TXT,
               FILEXT_LOG,
               FILEXT_BAK,
               FILEXT_LSL,
               FILEXT_INI,
               FILEXT_C,
               FILEXT_CPP,
               FILEXT_ST,
               FILEXT_H,
               FILEXT_XML,
               FILEXT_HTML,
               FILEXT_REX,
//               FILEXT_ICO,
             }
_FILEXT;


typedef enum { T2_NONE,
               T2_COPY_FILE,
               T2_COPY_DIRECTORY,
               T2_DELETE_FILE,
               T2_DELETE_DIRECTORY,
               T2_USERCALLMETHODE,
             }
_Thread2Cmd;


#ifdef LASAL
	#pragma pack (pop)
#endif

#ifdef LASAL
	#pragma pack (push, 1)
	#include <LSL_C_IFLSE.h>
	#include <lsl_c_ifssr.h>
	#include <lsl_c_iflinker.h>
  #include <RTOS_C_interfaces.h>
#endif

// DEFINED CONSTANTS ********************************************************
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

#ifndef IPC_SINGLEDATATYPE
typedef char                 _BYTE;                                 // 1 byte
typedef unsigned char        _UBYTE;                                // 1 byte
typedef short int            _WORD;                                 // 2 byte
typedef unsigned short int   _UWORD;                                // 2 byte
typedef long                 _DWORD;                                // 4 byte
typedef unsigned long        _UDWORD;                               // 4 byte
typedef float                _REAL;                  // 4 byte floating point
#define IPC_SINGLEDATATYPE
#endif

typedef _DWORD               _SVRTYP;
//typedef _UWORD               _YESNO;                // Yes/No
typedef _UWORD               _LANGUAGE;                           // language
typedef _UBYTE               _FONT;                             // no of font
typedef _UBYTE               _FRAMETYPE;                  // no of used frame
#ifdef UC_ACCESSNO_2BYTE
 typedef _UWORD               _ACCESS;                           // key access
#else
 typedef _UBYTE               _ACCESS;                           // key access
#endif
typedef _DWORD               _IVALUE;                       // value datatype
typedef char                 _ASCII;            // always 1 byte (ascii-code)
typedef _UWORD               _UNI;                // always 2 byte (uni-code)
typedef int                  _HANDLE;                              // handler
typedef _UDWORD              _CRC32;                                 // crc32

#ifdef UC_UNICODE
  typedef _UWORD             _CHAR;         // 1 character (ascii or unicode)
#else
  typedef _BYTE              _CHAR;         // 1 character (ascii or unicode)
#endif

typedef _UBYTE               _SELECTOR;
#define _SELECTOR_DEFAULT       0
#define _SELECTOR_ONLY_KBRD     1
#define _SELECTOR_ONLY_HID      2

#ifdef LASAL
  #include "COMINTFC.H"
#endif


#define _DEFFRAME               0
#define _SINGLEFRAME            1
#define _3DFRAME                2
#define _OVERLAPPEDFRAME        3
#define _EDGEFRAME              4
#define _DEFFRAME_NOCAREREGION  5
#define _THEMEFRAME             6

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

typedef void*         (*_JpgFunctLoad)          (unsigned short *width, unsigned short *height, char *dpne, unsigned short *err_code, void *fp_callback);
typedef unsigned long (*_JpgFunctSave)          (char *dpne, unsigned char *rgb, unsigned short width, unsigned short height, unsigned short quality, unsigned short color);

typedef void    (*_FVOID_PT_BOOL_BOOL)          (void*, _BOOL, _BOOL);
typedef void    (*_FVOID_PT_BOOL)               (void*, _BOOL);
typedef void    (*_FVOID_PT_PT_BOOL)            (void*, void*, _BOOL);
typedef void    (*_FVOID_PT_PT_PT)              (void*, void*, void*);
typedef void    (*_FVOID_PT_PT)                 (void*, void*);
typedef void    (*_FVOID_PT)                    (void*);
typedef void    (*_FVOID_BOOL)                  (_BOOL);
typedef void    (*_FVOID_FOUNDHID)              (_FOUNDHID);
typedef void    (*_FVOID_WHOBEEP)               (_WHOBEEP);
typedef void    (*_FVOID_DWORD_DWORD)           (_DWORD, _DWORD);
typedef _UDWORD (*_FUDWORD_UDWORD_UDWORD)       (_UDWORD, _UDWORD);
typedef void    (*_FVOID_UWORD)                 (_UWORD);
typedef void    (*_FVOID_UWORD_UWORD)           (_UWORD, _UWORD);
typedef _BOOL   (*_FBOOL_PT_PT_PT_PT)           (void*, void*, void*, void*);
typedef _BOOL   (*_FBOOL_PT_PT_PT)              (void*, void*, void*);
typedef _BOOL   (*_FBOOL_UWORD_PT_PT)           (_UWORD, void*, void*);
typedef _BOOL   (*_FBOOL_PT)                    (void*);
typedef _BOOL   (*_FBOOL_DWORD)                 (_DWORD);
typedef _BOOL   (*_FBOOL_DWORD_DWORD_PT)        (_DWORD, _DWORD, void*);
typedef _BOOL   (*_FBOOL_PT_PT_BOOL)            (void*, void*, _BOOL);
typedef _BOOL   (*_FBOOL_BOOL)                  (_BOOL);
typedef _BOOL   (*_FBOOL_PT_PT_UDWORD)          (void*, void*, _UDWORD);
typedef void    (*_FVOID_VOID)                  (void);
typedef _BOOL   (*_FBOOL_VOID)                  (void);
typedef _UDWORD (*_FUDWORD_PT_PT_UWORD)         (void*, void*, _UWORD);
typedef _UWORD  (*_PT_LOOKUPEMBEDDED)           (void*, void*, void*);
typedef void*   (*_FPT_VOID)                    (void);
typedef _BOOL   (*_FBOOL_PT_PT_PT_BOOL)         (void*, void*, void*, _BOOL);
typedef _BOOL   (*_FBOOL_PT_PT_PT_SELIS_UDWORD) (void*, void*, void*, _SELIS, _UDWORD);
typedef _BOOL   (*_FBOOL_PT_PT)                 (void*, void*);
typedef _BOOL   (*_FBOOL_UWORD_PT)              (_UWORD, void*);
typedef _DOIT   (*_FDOIT_PT)                    (void*);
typedef _DOIT   (*_FDOIT_PT_BOOL_PT)            (void*, _BOOL, void*);
typedef _DOIT   (*_FDOIT_PT_PT)                 (void*, void*);
typedef _DOIT   (*_FDOIT_PT_PT_BOOL)            (void*, void*, _BOOL);
typedef _DOIT   (*_FDOIT_PT_PT_PT_BOOL)         (void*, void*, void*, _BOOL);
typedef _DOIT   (*_FDOIT_VOID)                  (void);
typedef _DOIT   (*_FDOIT_BOOL)                  (_BOOL);
typedef _UDWORD (*_FUDWORD_UDWORD_UDWORD_PT)    (_UDWORD, _UDWORD, void*);
typedef _BOOL   (*_FBOOL_PT_UWORD)              (void*, _UWORD);
typedef void*   (*_FPT_UWORD)                   (_UWORD);
typedef _DWORD  (*_FDWORD_DWORD)                (_DWORD);
typedef _UWORD  (*_FUWORD_PT_UWORD)             (void*, _UWORD);
typedef _UWORD  (*_FUWORD_PT)                   (void*);
typedef _UWORD  (*_FUDWORD_PT_PT)               (void*, void*);
typedef void    (*_FVOID_UDWORD_DWORD_DWORD)    (_UDWORD, _DWORD, _DWORD);
typedef _BOOL   (*_FBOOL_CMD_DWORD_DWORD_PT)    (_PIC_COMMAND, _DWORD, _DWORD, void*);
typedef void    (*_FVOID_PT_UDWORD)             (void*, _UDWORD);
typedef void    (*_FVOID_PT_UDWORD_UDWORD_PT)   (void*, _UDWORD, _UDWORD, void*);
typedef _BOOL   (*_FBOOL_PT_UWORD_FOUNDHID_BOOL)(void*, _ACCESSFAILED, _FOUNDHID, _BOOL);

typedef void    (*_XCHANGE_FKT) (void*, void*, _UWORD);
typedef _UWORD  (*_SORTFUNCT) (void*, void*);  // liefert 0 (a<=b) oder 1(a>b)
typedef _WORD   (*_QSORTFUNCT) (void*, void*); // liefert -1(a<b), 0(a=b) oder 1(a>b)


typedef struct { _UDWORD length;
                 _ASCII  *label;
               } 
_SORTA;

typedef struct { unsigned long ulVersion;
                 int (*GetKeyCode)(void);
                 int (*SetKeyDelay)(unsigned int);
                 int (*SetKeyCPS)(unsigned int);
                 int (*SetKeyTable)(unsigned int*);
                 int (*SetKeyMode)(unsigned int, unsigned int);
                 int (*SetKeyTableCharCode)(void*);
               }
IKEYPAD;

#ifdef __TURBOC__
  #include "t_graph.h"
#endif
// DEFINITIONS **************************************************************
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


// USED CLASSES ************************************************************

class _Keyboard;       // t-kbrd.cpp
class _TextPuffer;     // t-text.cpp
class _Handle;         // t-file.cpp
class _File;           // t-file.cpp
class _VarList;        // t-varia.cpp
class _ChkBit;         // t-memo.cpp
class _PopUp;          // t-popup.cpp
class _ImageList;      // t-icon.cpp
class _Stack;          // t-stack.cpp
class _Window;         // t-win.cpp
class _Picture;        // t-picpic.cpp
class _SubPicture;     // t-subpic.cpp
class _Cursor;
//class _AsciiPuffer;

// LASALOS DATATYPES *******************************************************

#ifdef UC_IPC
typedef struct { _UDWORD          label;
                 _UDWORD          data;
               }
_MEMSIZE;
#endif

// EXTENDED DATATYPES ******************************************************

typedef struct { _Thread2Cmd       cmd;
                 _DWORD            user1;
                 _UDWORD           user2;
                 _ASCII            op1[_MAX_PATHLENGTH];
                 _ASCII            op2[_MAX_PATHLENGTH]; 
                 _FVOID_PT_BOOL    pcallback;
                 void              *pthis;
               }
_Thread2CmdLine;


#ifdef UC_THEME
typedef struct { _BMP         bmp_tl;  // top left
                 _BMP         bmp_tr;  // top right
                 _BMP         bmp_bl;  // bottom left
                 _BMP         bmp_br;  // bottom right
                 _BMP         bmp_l;   // left
                 _BMP         bmp_r;   // right
                 _BMP         bmp_t;   // top
                 _BMP         bmp_b;   // bottom
                 _BMP         bmp_c;   // center
                 _BMP         bmp_cx;  // cancle
               }
_FRAMEWORK;

typedef struct { _UWORD       id;
                 _WORD        delta_l;
                 _WORD        delta_r;
                 _WORD        delta_t;
                 _WORD        delta_b;
                 _WORD        xpos_cx;
                 _WORD        ypos_cx;
                 _UBYTE       colorcourse;
                 _FRAMEWORK   frame0;
                 _FRAMEWORK   frame1;
               }
_THEME;

typedef struct { _UWORD       no;
                 _THEME       *ptr;
               }
_MEM_THEME;

#endif

#define _DEFTHEME       0
#define T_INVERTTHEME   T_BLINK


typedef struct { _UDWORD      cnt;
                 _UWORD       act;
                 _UWORD       psh;
               }
_MEMORYIDX;

typedef struct { _KOORD       dx;
                 _KOORD       yo;
                 _KOORD       yu;
               }
_FLIPSCREEN;

typedef struct { _UDWORD      no;
                 _UDWORD      begin;
                 _UDWORD      pos;
                 _BOOL        used;
               }
_RINGBUFFER;

typedef struct { _UWORD       no;
                 _ATTRIB      *ptr;
               }
_MEM_ATTRIB;

typedef struct { _UWORD no;
                 _COLOR *ptr;
               }
_MEM_COLOR;

typedef struct { _UWORD no;
                 _FONT  *ptr;
               }
_MEM_FONT;

typedef struct { _UWORD no;
                 _BOOL  *ptr;
               }
_MEM_BOOL;

typedef struct { _UWORD wYear;
                 _UWORD wMonth;
                 _UWORD wDayOfWeek;
                 _UWORD wDay;
                 _UWORD wHour;
                 _UWORD wMinute;
                 _UWORD wSecond;
                 _UWORD wMilliseconds;
               }
_MAESYSTEMTIME;

typedef struct { _UWORD eventTime;          // System time stamp
                 _UBYTE eventType;          // Event type flags
                 _UBYTE eventSource;        // Event source device ID
                 short  eventChar;          // Keyboard Character code
                 short  eventScan;          // Keyboard Scan code
                 short  eventState;         // Keyboard State info
                 short  eventButtons;       // Positional device buttons
                 short  eventX;             // Positional device X
                 short  eventY;             // Positional device Y
                 short  eventUser[2];       // User defined event data
                 unsigned short eventSourceID;
               }
_OSEVENT;

typedef struct { _WORD  x;               // zoomfactor x (0 and 1000 == 1:1)
                 _WORD  y;               // zoomfactor y (0 and 1000 == 1:1)
               }
_ZOOM;

#define TYP_STANDARD 0  // standard ram (max 4 byte)
#define TYP_EXTENDED 1  // string, complex

typedef struct { _UDWORD id;             // intern lasal id
                 _UBYTE  vtype;          // intern vartype
               }
_LASALID;                                // lasal variable id

typedef struct { _UBYTE  hour;
                 _UBYTE  minute;
                 _UBYTE  second;
               }
_TIME;                                   // time

typedef struct { _UBYTE  day;
                 _UBYTE  month;
                 _UWORD  year;
                 _UBYTE  dayofweek;      // mo,tu,we,th etc (0-6)
               }
_DATE;                                   // date

typedef struct { _DATE   fdate;
                 _TIME   ftime;
               }
_DATIM;                                  // date and time

typedef struct { _UBYTE  id;
                 _UBYTE  op;
                 _IVALUE value;
               }
_LEXEM;                                  // intern: used for sequencelist

typedef struct { _UWORD  size;
                 _UBYTE  *ptr;
               }
_COMPCODE;                               // single sequence line

#define DEFSEQNO  65535
typedef struct { _UWORD    seqno;        // sequenceno
                 _UBYTE    invert;       // invert (0..false, 1..true)

                 _COMPCODE overload;     // overloaded sequence
               }
_CHKBIT;                                 // chekbit

typedef struct { _UWORD    seqno;        // sequenceno
                 _UBYTE    invert;       // invert (0..false, 1..true)
               }
_XCHKBIT;

typedef _UWORD  _XXCHKBIT;               // 0x7FFF sequenceno + 0x8000 invert


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
typedef _UBYTE _DEFVAL;

typedef struct { _DEFVAL    state;
                 _IVALUE    value;
							 }
_VARSINGLE;                              // single varlabelstatement

#define VL_MAXINDEX 2                    // max no of index in variable (niemals kleiner als 2)
typedef struct { _UBYTE     no;
                 _VARSINGLE info[VL_MAXINDEX+1];
               }
_VARLABEL;                               // variable, constant, etc

typedef struct { _UWORD        no;
                 _VARLABEL     *ptr;
               }
_MEM_VARLABEL;

typedef struct { _ANYLST     list;       // list id
								 _UDWORD     no;         // reference-no
							 }
_ANYTHING;                               // reference to any list

typedef struct { _UWORD    no;
                 _ANYTHING *ptr;
               }
_MEM_ANYTHING;

typedef struct { _ANYTHING     anything;
                 _MEM_VARLABEL memvarlabel;
               }
_ANYPARA;

typedef struct { _UWORD     no;
                 _ANYPARA   *ptr;
               }
_MEM_ANYPARA;

typedef struct { _UBYTE     xxno;
                 _ANYPARA   *xxptr;
               }
_XMEM_ANYPARA;

typedef struct { _UBYTE     info;
                 _VARLABEL  link;
               }
_LINKVL;

#ifdef UC_LINKVL
typedef struct { _UBYTE     no;
                 _LINKVL    *ptr;
               }
_MEM_LINKVL;
#endif

typedef struct { _ENTRYTYPE ftype;
                 _UWORD     no;
                 _UWORD     nextno;
                 _CHKBIT    chkbit;
                 _ASCII     name[13];
                 _UBYTE     usedintern;
                 _ANYPARA   anypara;    // titel-description
                 _ACCESS    access;
                 _UWORD     imageno;
                 _UBYTE     refcyc;
                #ifdef XUSER_KM
                 _UWORD     softinfo;
                #endif
                 _DWORD     time_max;
                 _DWORD     time_last;
                 _ASCII     label[32];
               }
_PRJ_ENTRY;                              // single projectentry

typedef struct { _ASCII  *ptr;                            // pointer to text
                 _UDWORD no;                                   // no of text
                 _UDWORD size;                           // no of characters
                 _UDWORD space;
               }
_ASCII_BUFFER;

typedef struct { _ASCII_BUFFER ascii_buffer;
                 _UDWORD       *poffset;
               }
_FAST_ASCII_BUFFER;

typedef struct { _CHAR   *ptr;                            // pointer to text
                 _UDWORD *poff;                // pointer to textoffsettable
                 _UDWORD no;                                   // no of text
                 _UDWORD size;                           // no of characters
               }
_TEXT_BUFFER;


typedef struct { _UDWORD          index;
                 _DWORD           value;
               }
_COMBOBOXLINE;

typedef struct { _FONTPTR *ptr;
                 _UWORD   width;
                 _UWORD   height;
                 _UDWORD  size;
                 _BOOL    isheap;
                 _ATTRIB  attrib;
               }
_FONTINFO;

class _TextPuffer
{
  private :
    // ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION
    // Kommen hier noch Daten hinzu, stimmt die Größe von _PICTURE im T_TYPES.H nicht mehr.
    _TEXT_BUFFER Tbuffer;

//    _BOOL   Realloc(_UDWORD);                              // allocate memory

  private :
    _BOOL   Jump(_File*);                      // overjump textbuffer in file

  public :
//    _BOOL   Write(_File*);                                 // write to handle
//    _BOOL   Add(void*, _UWORD);                               // add new text
//    _BOOL   Del(_UDWORD);                             // delete existing text
//    _BOOL   Ren(_UDWORD, _CHAR*);                    // xchange existing text
//    _BOOL   Ins(_UDWORD, _CHAR*);                          // insert new text
//    _BOOL   GetChange(void);                               // get changestate
//    void    SetChange(_BOOL c);                            // set changestate
//    _UDWORD GetOffset(_UDWORD);                         // get offset to text
    void    Init(void);                                    // init textbuffer
    void    Free(void);
    _BOOL   Read(_File*, _BOOL*);                         // read from handle
    _CHAR*  GetText(_UDWORD);                          // get pointer to text
    _BOOL   ReadLanguage(_File*, _LANGUAGE);               // read textbuffer
    _BOOL   Load(_ASCII*, _UWORD);                           // load textlist
    void    StrUp(void);                            // convert to big letters
    _BOOL   Copy(_TextPuffer*);
    _BOOL   Merge(_TextPuffer*);                          // merge textpuffer
    _UDWORD GetSize(void);                  // get no of characters in buffer
    _CHAR*  GetPtr(void);                            // get --> to first text
    _UDWORD GetNo(void);                                    // get no of text
    _UDWORD Add(void *txt, _UWORD chr_size);
};

typedef struct { _UWORD           lineheight;     // default = 0
                 _FRAMETYPE       frame;          // default = 0xFF 
                 _UBYTE           linesperpage;   // default = 0
               }
_COMBOINFO;

typedef struct { _TextPuffer      TextPuffer;
                 _UDWORD          no;
                 _COMBOBOXLINE    *ptr;
                 _ATTRIB          attrib;
                 _COLOR           color;
                 _COMBOINFO       info;
               }
_COMBOBOX;

typedef struct { _UDWORD  no;
                 _DWORD   *ptr;
               }
_MEM_NO;

typedef struct { _UDWORD  no;
                 _BMP     *ptr;
               }
_MEM_BMP;

typedef struct { _ASCII_BUFFER name;
                 _MEM_NO       index;
                 _UDWORD       crc32;
               }
_NAME_INDEX;

typedef struct { _MEM_NO        winid;         // win-id's der einzelnen sprachen
                 _MEM_NO        flag; // verweis auf image der einzelnen sprachen
                 _TEXT_BUFFER   name;        // aliasnamen der einzelnen sprachen
                 _ASCII_BUFFER  winname;      // win-namen der einzelnen sprachen
                 _UWORD         no;            // anzahl der vorhandenen sprachen
               }
_LANGUAGE_INFO;

typedef struct { _UWORD         no;
                 _PRJ_ENTRY     *ptr;
                 _UWORD         no_static;
                 _ASCII         prjdp[_MAX_PATHLENGTH];
                 _CRC32         crc32dpne;
                 _UDWORD        RevisionNumber;
                 _CHAR          ProjectName[128];
//                 _LANGUAGE_INFO xlanguage_info;
               }
_PROJECT;

#define PARAMETER_TOOLCATALOGUE       0x00000001L
#define PARAMETER_PROTOCOL            0x00000002L
#define PARAMETER_PDE                 0x00000004L
#define PARAMETER_EXTENDED_BUBBLEHELP 0x40000000L
#define PARAMETER_ALARMBIT            0x80000000L

typedef struct { _UDWORD    group;
                 _UDWORD    parameter;
               }
_STATISTIC;

typedef struct { _COLOR     color;
                 _UWORD     length;
                 _UWORD     angle;
               }
_ROBY;


// P ... free
// O ... vornullen
// W ... free
// P ... free
// R ... free
// U ... permanent
// I ... inverter
// C ... fix dezimalpoint

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
#define VL_STRUCTUREOFFSET    0xFE

typedef _UWORD  _IADDRESS;
#define DEF_IADDRESS      0xFFFF

#ifdef UNIT_2Byte
 typedef _UWORD  _VUNIT;
 #define DEF_VUNIT         0xFFFF
#else
 typedef _UBYTE  _VUNIT;
 #define DEF_VUNIT         0xFF
#endif

typedef struct { _VARTYPE    vartype;
                 _UDWORD     classid;
                 _VARLABEL   lo_limit;
                 _VARLABEL   hi_limit;
                 _UWORD      accessno;
                 _UWORD      format;                  // format of variable
                 _UBYTE      station;                      // stationnumber
                 _UBYTE      bitno;                            // Bitnummer
                 _VUNIT      funit;                                 // unit
                 _UBYTE      POWPRUIC;   //
                 _UWORD      funitscheme;                   // unitschemeno
                 _UWORD      reftime;                        // Refreshtime
                 _ANYPARA    text1;                            // vartext 1
                 _ANYPARA    text2;                            // vartext 2
                 _ANYPARA    text3;                            // vartext 3
								 _ANYPARA    text4;                            // vartext 4
                 _STATISTIC  statistic;
                 _UWORD      stscheme;
                 _UWORD      uimenuindex;                     // menueindex
                 _XCHKBIT    xchkbit;
                 _UWORD      userconfig;           // Anwenderkonfiguration
                #ifdef UC_LINKVL
                 _LINKVL     linkvl[UC_LINKVL];              // user link's
                #endif
                 _UBYTE      physic;
                 _UDWORD     hostname;

                 _VARLABEL   xadress;                 // adress of variable
                 _LASALID    lasalid;                            // lasalid
                 _CRC32      crc32;
                 _UBYTE      varobj;              // variable(0) - object(1)

                 _UBYTE      formula;                             // formula
                 _UBYTE      digits;                        // no. of digits
                 _UBYTE      posdezpt;              // position decimalpoint
                 _VARLABEL   parameter;      // ifnec. parameter for formula
                 
                #ifdef UC_EXOONLINE
                 _UDWORD     exoinfo;
                #endif
                #ifdef USER_DEMAG
                 _UDWORD     DemagDynPara; // sa24193
                #endif
               }
_VARINFO;

typedef struct { _UBYTE       state;
                 _IVALUE      lolim;
                 _IVALUE      hilim;
               }
_HILOLIM;



#define VARBIT_FIXDEZPT       0x01 // fixe Position Dezimalpoint
#define VARBIT_INVERT         0x02 // Inverter
#define VARBIT_PERMANENT      0x04 // Permanente Reflist
#define VARBIT_VARMEMBER      0x08 // Membervariable
#define VARBIT_VARGLOBAL      0x10 // globale Variable
#define VARBIT_STRUCTELEMENT  0x20 // Strukturelement (bei Servern)
#define VARBIT_PREZERO        0x40 // Vornullen

//#define VT_GLOBAL           0x80 // Globale Variabel
//#define VT_ELEMENT          0x40 // Strukturelement (bei Servern)

typedef struct { _VARTYPE     vartype;
                 _HILOLIM     hilolim;
                 _UWORD       format;                 // format of variable
                 _UBYTE       station;                     // stationnumber
                 _UBYTE       reftime_bitno;     // refreshtime + Bitnummer
                 _VUNIT       funit;                                // unit
                 _UBYTE       POWPRUIC;
                 _XMEM_ANYPARA xmemtext;               // vartext 1, 2, 3, 4
                 _STATISTIC   statistic;
                 _UWORD       stscheme;
                 _UWORD       uimenuindex;                    // menueindex
                 _XXCHKBIT    xxchkbit;
                 _UWORD       classid;
                 _UWORD       hostname;
                 _UWORD       funitscheme;                  // unitschemeno
                 _UWORD       accessno;
                 _UWORD       userconfig;          // Anwenderkonfiguration
                 _UBYTE       physic;
                #ifdef UC_LINKVL
                 _MEM_LINKVL  memlinkvl;                     // user link's
                #endif

                 _IADDRESS    iaddress;
                 _LASALID     lasalid;                           // lasalid
                 _CRC32       crc32;
                #ifdef UC_MULTICPU
                 _UDWORD      multiid[UC_MULTICPU];
                #endif
                #ifdef UC_EXOONLINE
                 _UDWORD      exoinfo;
                #endif
                #ifdef UC_MEMORIZE_PLCDATA
                 _RESULT_TYPE plc_value_ftype;
                 _IVALUE      plc_value;
                #endif
                #ifdef UC_MEMBERVARIABLE
                 _DWORD       memberoffset;
                #endif 
               }
_LSEVARINFO;


// (*
typedef void    (*_FVOID_MESSAGEBOXRESULT) (_MESSAGEBOXRESULT);
// *)

typedef struct
{
  void                    *Text;
  _UWORD                  ChrSize;
  _UDWORD                 Scancode;
  _COLOR                  ColorText;
  _COLOR                  ColorBack;
  _COLOR                  ColorFrame;
  _FRAMETYPE              FrameType;
  _FONT                   Font;
  _ROOM                   Space;
  _MESSAGEBOXRESULT       Result;
  void                    *pObj;
  _FVOID_MESSAGEBOXRESULT pFkt;
}
_MESSAGEBOXBUTTON;

typedef struct
{
  _CHAR             Text[1024];
  _COLOR            ColorText;
  _COLOR            ColorBack;
  _COLOR            ColorFrame;
  _UDWORD           ImageNo;
  _FRAMETYPE        FrameType;
  _FONT             Font;
  _MESSAGEBOXTYP    Typ;
  _MESSAGEBOXBUTTON MemButton[2];
}
_MESSAGEBOX;


typedef struct { _DWORD     max;
                 _DWORD     i;
                 _ROOM      room;
                 _KOORD     oldx;
                 _COLOR     color;
                 _ASCII     *text;
               }
_SCAN;

typedef struct { _VARLABEL  xdigits;
                 _VARLABEL  posdezpt;
                 _UBYTE     formula;
                 _VARLABEL  parameter;
                 _UBYTE     fixdp;
                 _ANYPARA   anypara;
                 _UBYTE     leading0;
               }
_UNIT;

typedef struct { _UWORD     no;
                 _UNIT     *ptr;
               }
_MEM_UNIT;

typedef struct { _UDWORD    no;
                 _VARINFO   info;
               }
_VARIABLE;

typedef struct { void          *ptr;     // --> to data
                 _IVALUE       value;    // numeric value
                 _IVALUE       rmin;     // limit
                 _IVALUE       rmax;     // limit
                 _DATIM        datim;    // date time
                 _RESULT_TYPE  ftype;    // type of result, date, numeric, string, etc
                 _WORD         recalculate_done; // calculation is already done
                 _UWORD        remotedata;
                 _UDWORD       myiocrc;  // seperate crc
               }
_RESULT;                                 // resultdatatype

#define  DEF4BYTE                0xFFFFFFFFL
#define  DEFSCOPE                0xFFFFFFFFL
#define  DEFINDEX                0
#define  VARNO_EDITORDUMMY       0xFFFFFFFEL
#define  VARNO_TIMEX_100         0xFFFFFFFDL
#define  VARNO_ALTGRFLAG         0xFFFFFFFCL
#define  VARNO_ALTGRFLAG_LOCK    0xFFFFFFFBL // NOTE: Wert von VARNO_ALTGRFLAG wird auf 0 oder 2 gesetzt
#define  VARNO_SHIFTFLAG         0xFFFFFFFAL
#define  VARNO_INSERTFLAG        0xFFFFFFF9L
#define  VARNO_HIRAGANA_KATAGANA 0xFFFFFFF8L
#define  VARNO_JAPANESE_INPUT    0xFFFFFFF7L


typedef struct { _UDWORD    scope;       // scope
//                 _UDWORD    index;       // indexwidth
               }
_VARSYMBOL;                              // symboltabelentry

typedef struct { _UDWORD    no;
                 _VARSYMBOL *ptr;
               }
_MEM_VARSYMBOL;

typedef struct { _UDWORD       no;
                 _LSEVARINFO   *ptr;
               }
_MEM_LSEVARINFO;

typedef struct { _CHKBIT    chkbit;
                 _ROOM      room;
                 _ATTRIB    attrib;
                 _COLOR     color;
                 _COLOR     colorframe;
                 _FRAMETYPE frametype;
               }
_RECTANGLE;

typedef struct { _CHKBIT    chkbit;
                 _ROOM      room;
                 _WORD      angle;
                 _WORD      arc;
                 _ATTRIB    attrib;
                 _COLOR     color;
                 _COLOR     colorframe;
               }
_CIRCLE;

typedef struct { _CHKBIT    chkbit;
                 _UWORD     no;
                 _DOT       *ptr;
                 _ATTRIB    attrib;
                 _COLOR     color;
                 _COLOR     colorframe;
               }
_POLYGON;

#define SOP_SMALLER       0x00 // <
#define SOP_SMALLEQUAL    0x01 // <=
#define SOP_EQUAL         0x02 // ==
#define SOP_BIGGEREQUAL   0x03 // >=
#define SOP_BIGGER        0x04 // >
#define SOP_NOTEQUAL      0x05 // !=
typedef struct { _VARLABEL   upto;
                 _UBYTE      op;
                 _UDWORD     value;
               }
_SINGLESCHEME;

typedef struct { _UWORD        no;
                 _SINGLESCHEME *ptr;
                 _BOOL         result_is_varno;
               }
_MEM_SINGLESCHEME;

typedef struct { _VARLABEL         link;
                 _MEM_SINGLESCHEME memsinglescheme;
               }
_SCHEME;

typedef struct { _UWORD     no;
                 _SCHEME    *ptr;
               }
_MEM_SCHEME;

#define DEFAULTSCHEME 0xFFFF
#define NOSTSCHEME    0xFFFE
typedef struct { _UWORD     schemeno;

                 _SCHEME    overload;
               }
_MYSCHEME;

typedef struct { _MYSCHEME  myscheme;
                 _VARLABEL  varlabel;
               }
_LINKSCHEME;

typedef struct { _UWORD      no;
                 _LINKSCHEME *ptr;
               }
_MEM_LINKSCHEME;

typedef struct { _LINKSCHEME linkscheme;
                 _COLOR      color;
               }
_DYNCOLOR;

typedef struct { _UWORD     no;
                 _DYNCOLOR *ptr;
               }
_MEM_DYNCOLOR;

#define STATE_ACTIVE              0x00
#define STATE_INACTIVE            0x01
#define STATE_INVISIBLE           0x02
#define STATESCHEME_PRESELECTION  STATE_INVISIBLE


typedef struct { _CHAR      *ptr;
               }
_USERTEXT;               

typedef struct { _CHKBIT    chkbit;
                 _ROOM      room;
                 _ANYPARA   anypara;
                 _FONT      font;
                 _FRAMETYPE frametype;
                 _ATTRIB    attrib;
                 _COLOR     colback;         // color background (hatch)
                 _COLOR     colframe;        // color frame (top-left, bottom-right)
                 _COLOR     coltxt;          // color text
                 _UWORD     summary;
                 _MYSCHEME  stscheme;
                 _UNI       fillchr;
                 _BOOL      doimage;
                 _BOOL      lock_overload;

                 _IMAGE     image;
                 _UDWORD    oldcrc;
                 _BOOL      onscreen;
                 _USERTEXT  usertxt;
               }
_PICTEXT;

typedef struct { _PIC_COMMAND cmd;
                 _VARLABEL    op1;
                 _VARLABEL    op2;
                 _BOOL        lrm;
               }
_STACK_LINE;

typedef struct { _UWORD       no;
                 _STACK_LINE  *ptr;
               }
_STACK;

typedef struct { _UWORD      no;


                 _STACK      overload;
               }
_LSEFUNCT;

typedef struct { _UWORD      no;
                 _LSEFUNCT   *ptr;
               }
_MEM_LSEFUNCT;

typedef struct { _LSEFUNCT   pos_fl;
                 _LSEFUNCT   neg_fl;
               }
_GFUNCT;

typedef struct { _URGETYPE       urgetype;
                 _UDWORD         info;
                 _MEM_FONT       memfont;
                 _MEM_ATTRIB     memattrib;
                 _MEM_COLOR      memcolor;
                 _MEM_VARLABEL   memvarlabel;
                 _MEM_ANYTHING   memanything;
                 _MEM_DYNCOLOR   memdyncolor;
                 _MEM_LSEFUNCT   memlsefunct;
                 _MEM_ANYPARA    memanypara;
                 _MEM_BOOL       membool;
                 _MEM_ANYTHING   memimage;  // backgroundimage

                 void            *ptr;
               }
_THEURGE;

typedef struct { _UWORD     act_output;           // act. refresh no of output
                 _UWORD     act_input;             // act. refresh no of input
                 _UWORD     act_motion;           // act. refresh no of motion
                 _UWORD     act_object;           // act. refresh no of object
                 _UWORD     act_colrect;         // act. refresh no of colrect
                 _UWORD     act_colpoly;         // act. refresh no of colpoly
                 _UWORD     act_colcirc;         // act. refresh no of colcirc
                 _UWORD     act_colpictxt;     // act. refresh no of colpictxt
                 _UWORD     act_button;           // act. refresh no of button
                 _UWORD     act_pictext;     // act. refresh no of picturetext
//                 _UWORD     cnt_priority;          // refresh-priority counter
//                 _UWORD     cnt;                   // refresh-priority counter
							 }
_SUBACTUAL;

typedef struct { _UWORD     last_editorno;          // no of last io in editor
               }
_PICRUNMEMO;

typedef struct { _BOOL      init;
                 _IVALUE    value;
               }
_CSTATE;

typedef struct { _CHKBIT     chkbit;
                 _ROOM       room;
                 _UWORD      no;
                 _ATTRIB     attrib;
                 _COLOR      color;
                 _COLOR      colorframe;
                 
                 _BMP        *userbmp;
               }
_BITMAP;

typedef struct { _UWORD      h_next;
                 _UWORD      v_next;
               }
_DIRECTION;

typedef struct { _UWORD      objno;
                 _VARLABEL   loader;
               }
_CAROITEM;

typedef struct { _UWORD      objno;
                 _VARLABEL   loader;
                 
                 _SubPicture *SubPicture;
                 _ZOOM       zoom;
                 _ROOM       room; // tatsächlicher room
                 _ROOM       room0; // room bei offset 0
                 _BOOL       onscreen; // gibt auskunft ob objekt am bildschirm platziert ist
                 _BOOL       gesamt; // gibt auskunft ob ganzes oder nur halbes objekt am bildschirm platziert ist
               }
_KARU;

typedef struct { _UWORD      no;
                 _KARU       *ptr;
               }
_MEM_KARU;

typedef struct { _BOOL       active;       // activ state
                 _ROOM       room;         // position (used space)
                 _COLOR      color;        // color
                 _COLOR      frame_color;  // color of frame
                 _KOORD      old_pos;      // sign (old position)
                 _BOOL       trigger_state;
                 _BOOL       drag;
                 _BOOL       pguppgdown;   // scrollleiste mit PgUp, PgDown, Up und Down button
               }
_LEVEL;

typedef struct { _ROOM       room;       // position
                 _COLOR      col_back;   // color
                 _COLOR      col_front;  // colorthumb
                 _COLOR      col_frame;  // color
                 _COLOR      col_text;   // color
                 _FRAMETYPE  frametype;  // frametype
                 _BOOL       horizontal; // horizontal true/false
                 _BOOL       valid;      // gültig true/false
  
                 _DWORD      width;      // anzahl elemente
                 _DWORD      pos;        // position akt.element
                 _DWORD      oldwidth;   // referenz anzahl elemente
                 _DWORD      oldpos;     // referenz position akt.element
  
                 _BOOL       active;
                 _DOT        active_dot;
}
_SCROLLBAR;

typedef struct { _CHKBIT     chkbit;
                 _ROOM       room;
                 _ATTRIB     attrib;
                 _ANYTHING   anything;
                 _VARLABEL   varx;
                 _VARLABEL   vary;
                 _VARLABEL   loader;
                 _DIRECTION  direction;
                 _MYSCHEME   stscheme;
                 _UWORD      summary;
                 _ZOOM       zoom;
                 
                 _COLOR      colback;     // backgroundcolor
                 _LINKSCHEME dyncolback;  // dynamic color background 
                 _LINKSCHEME dyncolhatch; // dynamic color background (hatch)
                 _MEM_KARU   memkaru;
                 _UWORD      gap;         // abstand zwischen einzelnen objekten in pixel
                 _DWORD      karuwidth;   // gesamtbreite  oder gesamthöhe
                 _DWORD      widthlastpage;
                 _BOOL       infinite;    // endloskarusell
                 _BOOL       horizontal;  // horizontale oder vertikale ausrichtung
                 _BOOL       onscreen;
                 _BOOL       movstate;    // ist true zwischen press und release
                 _BOOL       movbegin;    // ist true zwischen press und move
                 _BOOL       swim;
                 _BOOL       positiv_dir; // true wenn nach rechts oder unten "gemoved" wurde
                 _BOOL       usermove;    // true wenn movement nicht vom kernel übernommen werden soll
                 _BOOL       overmove;    // carousell am anfang und ende weitermoven
                 _UDWORD     swimtime;
                 _IMAGE      image;
                 _SubPicture *SubPicture;
                 _WORKSTATE  workstate;
                 _DOT        oldxy;
                 _DOT        move;
                 _DOT        movstart;
                 _UDWORD     skip_percent;
                 _UDWORD     maxmovno;    // anzahl der elemente welche "hinausgemoved" werden können. nur bei infinite=false ist dies ungleich memkaru.no
                 _DWORD      movespeed;   // geschwindigkeit (multiplikator / 10) mit welcher "gemoved" wird
                #ifdef UC_OLD_MOTIONS
                 _UWORD      width;
                 _UWORD      height;
                #endif 
                 _VARLABEL   varindex;
                 _DWORD      memoindex;
                 _FRAMETYPE  frametype;
                 _ROOM       wiperoom;
                 _SCROLLBAR  scrbar;
               }
_MOTION;

typedef struct { _VARLABEL        upto;
                 _UBYTE           op;
                 _ANYPARA         anypara;
               }
_SINGLETXTSCHEME;

typedef struct { _UWORD           no;
                 _SINGLETXTSCHEME *ptr;
               }
_TXTSCHEME;

typedef struct { _UWORD           no;
                 _TXTSCHEME       *ptr;
               }
_MEM_TXTSCHEME;

typedef struct { _UWORD           schemeno;

                 _TXTSCHEME       overload;
                 _ANYPARA         anypara;
               }
_MYTXTSCHEME;

typedef struct { _ANYTHING     tbo;                  // text, bmp or obj number

                 _MYSCHEME     myscheme;
                 _MYTXTSCHEME  mytxtscheme;
                 _COMBOBOX     *pcombo;
                 _BOOL         combocopy;
               }
_IOINFO;

#define BIT_REDUNDANT  0x0001                // delete descendent zeros (format)
#define CALCUL_DIGITS  0x0002      // nk = digits - actual_vorkommastellen (KM!)

typedef struct { _UWORD       format;
                 _UBYTE       digits;
                 _VARLABEL    posdezpt;
               }
_PICFORMAT;

typedef struct { _UWORD       no;

                 void         *pthis;
               }
_VIRTNAME;

#ifdef UC_CHANGE_IO_GROUP
typedef struct { _UDWORD      serverno;
                 _UWORD       summary;
               }
_IOGRPCHX;
#endif

typedef struct { _CHKBIT      chkbit;      // checkbit
                 _ATTRIB      attrib;      // attribute
                 _ROOM        room;        // given space by lse
                 _FONT        font;        // fontinformation
                 _FRAMETYPE   frametype;   // frametype
                 _COLOR       colback;     // color background (hatch)
                 _COLOR       colframe;    // color frame (top-left, bottom-right)
                 _COLOR       coltxt;      // color text
                 _IO_TYPE     typ;         // displaytype
                 _VARLABEL    variable;    // link to data
                 _IOINFO      info;        // special information (depends on typ)
                 _PICFORMAT   picformat;   // overloaded format
                 _CSTATE      cstate;      // change state (old value)
                 _ROOM        space;       // calculated space (real space on screen)
                 _IMAGE       image;       // stored background
                 _BOOL        imagecopy;
                 _SubPicture  *Obj;        // pointer to an object (depends on typ)
                 _WORKSTATE   workstate;   // start, ready, run, end
                 _UWORD       summary;     // relevancy to other object
                 _VIRTNAME    virtname;    // information about virtual object name
                 _BOOL        is_dyncolor; // is any color dynamic ?
                 _LINKSCHEME  dyncolback;  // dynamic color background (hatch)
                 _LINKSCHEME  dyncolfront; // dynamic color background (hatch)
                 _LINKSCHEME  dyncoltl;    // dynamic color frame (top-left)
                 _LINKSCHEME  dyncolbr;    // dynamic color frame (bottom-right)
                 _LINKSCHEME  dyncoltxt;   // dynamic color text
                 _MYSCHEME    stscheme;
                 _DIRECTION   direction;   // direction for seek-next
                 _BOOL        doimage;
                 _BOOL        lock_overload;
                 _ANYPARA     newbubbletext;

                 // info just used for chkbox and radio-button
                 _LSEFUNCT    lsefunct;    // function for radio or chkbox when pressed
                 _UNI         fillchr;

                 // just for the urge io's
                 _THEURGE     *theurge;

                 // just references
                 _COLOR       refcolback;  // reference color background (hatch)
                 _COLOR       refcolframe; // reference color frame (top-left, bottom-right)
                 _COLOR       refcoltxt;   // reference color text
                 _UWORD       ioid;        // id for struct _SEEKELEMENT
                 void        *pUser;       // user pointer (THR)
                 _BOOL        bRefreshAct; // refresh flag (THR)
                 _VARLABEL    imoverloaded;// i'm overloaded with ...
                 _UBYTE       priority;    // refresh priority
                 _UDWORD      xuser;       // 4 byte userdata
                 _EDITSTATE   editstate;   // _EDITACTIVE, _EDITCHANGE, _EDITPASSIVE
                 _BOOL        onscreen;
                #ifdef ZOOM_KM
                 _ZOOM        kmz;
                #endif
                #ifdef UC_CHANGE_IO_GROUP
                 _IOGRPCHX    imiogroupchanged;
                #endif
                #ifdef UC_MENU_TWOCOLOR
                 _UDWORD      refstschemestate;
                #endif
               }
_IO;

typedef struct { _IO          io;
                 _ENTER_PLUS  enter_plus;
                 _UWORD       memseekno;
                 _UBYTE       searchchoice;
                 _SELECTOR    selector;
                 _COMBOINFO   comboinfo;
               }
_INPUT;

typedef struct { _CHKBIT      chkbit;
                 _DOT         xy;
                 _UWORD       no;
                 _ATTRIB      attrib;
                 _ZOOM        zoom;
                 _COLOR       colorframe;
                 _VARLABEL    loader;
                 _MYSCHEME    loader_scheme;
                 _MYSCHEME    stscheme;
                 _UWORD       summary;
                 _DIRECTION   direction;
                 _BOOL        doimage;
                 _BOOL        lock_overload; 

                 _SubPicture  *SubPicture;  // -> to object
                 _WORKSTATE   workstate;
                 _IMAGE       image;
                 _BOOL        onscreen;
                #ifdef UC_CHANGE_IO_GROUP
                 _IOGRPCHX    imiogroupchanged;
                #endif
               }
_OBJECT;

typedef struct { _RECTANGLE   rectangle;
                 _LINKSCHEME  dyncolback;         // color background (hatch)
                 _LINKSCHEME  dyncolfront;        // color background (hatch)
                 _LINKSCHEME  dyncoltl;           // color frame (top-left)
                 _LINKSCHEME  dyncolbr;           // color frame (bottom-right)

                 _COLOR       refcolor;           // reference
                 _COLOR       refcolorframe;      // reference
                 _BOOL        init;
               }
_COL_RECT;

typedef struct { _POLYGON     poly;
                 _LINKSCHEME  dyncolback;         // color background (hatch)
                 _LINKSCHEME  dyncolfront;        // color background (hatch)
                 _LINKSCHEME  dyncolframe;        // color frame

                 _COLOR       refcolor;
                 _COLOR       refcolorframe;
                 _BOOL        init;
               }
_COL_POLY;

typedef struct { _CIRCLE      circle;
                 _LINKSCHEME  dyncolback;         // color background (hatch)
                 _LINKSCHEME  dyncolfront;        // color background (hatch)
                 _LINKSCHEME  dyncolframe;        // color frame

                 _COLOR       refcolor;
                 _COLOR       refcolorframe;
                 _BOOL        init;
               }
_COL_CIRC;

typedef struct { _PICTEXT     btx;
                 _LINKSCHEME  dyncolback;         // color background (hatch)
                 _LINKSCHEME  dyncolfront;        // color background (hatch)
                 _LINKSCHEME  dyncoltl;           // color frame (top-left)
                 _LINKSCHEME  dyncolbr;           // color frame (bottom-right)
                 _LINKSCHEME  dyncoltxt;          // color text

                 _COLOR       refcolback;            // reference color background (hatch)
                 _COLOR       refcolframe;           // reference color frame (top-left, bottom-right)
                 _COLOR       refcoltxt;             // reference color text
                 _BOOL        init;
               }
_COL_PICTXT;

typedef struct { _COLOR       colback;     // color background (hatch)
                 _COLOR       colframe;    // color frame (top-left, bottom-right)
                 _COLOR       coltxt;      // color text
                 _LINKSCHEME  dyncolback;  // dynamic color background (hatch)
                 _LINKSCHEME  dyncolfront; // dynamic color background (hatch)
                 _LINKSCHEME  dyncoltl;    // dynamic color frame (top-left)
                 _LINKSCHEME  dyncolbr;    // dynamic color frame (bottom-right)
                 _LINKSCHEME  dyncoltxt;   // dynamic color text
               }
_EDITCOLOR;

typedef struct { _FONT        font;
                 _FRAMETYPE   frametype;
                 _ATTRIB      attrib;
                 _EDITCOLOR   editcolor;
                 _DWORD       delaytime;
                 _DWORD       remaintime;
               }
_BUBBLEINFO;

typedef struct { _COLOR       color;   // color cursor (hatch)
                 _LINKSCHEME  dyncol;  // dynamic color cursor (hatch)
               }
_CURSORCOLOR;


typedef struct { _UWORD       no;
                 _POLYGON     *ptr;
               }
_MEM_POLYGON;

typedef struct { _UWORD       no;
                 _BITMAP      *ptr;
               }
_MEM_BITMAP;

typedef struct { _UWORD       no;
                 _RECTANGLE   *ptr;
               }
_MEM_RECTANGLE;

typedef struct { _UWORD       no;
                 _CIRCLE      *ptr;
               }
_MEM_CIRCLE;

typedef struct { _UWORD       no;
                 _PICTEXT     *ptr;
               }
_MEM_PICTEXT;

typedef struct { _UWORD       no;
                 _OBJECT      *ptr;
               }
_MEM_OBJECT;

typedef struct { _UWORD       no;
                 _INPUT       *ptr;
               }
_MEM_INPUT;

typedef struct { _UWORD       no;
                 _IO          *ptr;
               }
_MEM_OUTPUT;

typedef struct { _UWORD       no;
                 _MOTION      *ptr;
               }
_MEM_MOTION;

typedef struct { _UWORD       no;
                 _COL_RECT    *ptr;
               }
_MEM_COL_RECT;

typedef struct { _UWORD       no;
                 _COL_POLY    *ptr;
               }
_MEM_COL_POLY;

typedef struct { _UWORD       no;
                 _COL_CIRC    *ptr;
               }
_MEM_COL_CIRC;

typedef struct { _UWORD       no;
                 _COL_PICTXT  *ptr;
               }
_MEM_COL_PICTXT;

typedef struct { _GFUNCT      gfunct;
                 _TIP         tip;
                 _UWORD       code;
               }
_KEY;

typedef struct { _UWORD       no;
                 _KEY         *ptr;
               }
_MEM_KEY;

#ifdef UC_ABANDONED
typedef struct { _UDWORD      ftime;
                 _UDWORD      timediff;
               }
_REPEAT;
#endif

typedef struct { _UWORD        image_release;     // released image
                 _UWORD        image_press;       // pressed image
                 _UWORD        image_sel_release; // selected+released image
                 _UWORD        image_sel_press;   // selected+pressed image
                 _UWORD        image_in_release;  // inactive released image
                 _UWORD        image_in_press;    // inactive pressed image
                 _UWORD        image_describe;    // describing image
                 _BOOL         zoom_images;       // zoom images to buttonsize
               }
_BUTTONIMAGES;

typedef struct { _CHKBIT       chkbit;
                 _ROOM         room;
                 _GFUNCT       gfunct;
                 _ANYTHING     anything;
                 _VARLABEL     loader;
                 _ATTRIB       attrib;
                 _COLOR        framecol;       // color of frame
                 _COLOR        col;            // (in-)active color
                 _BUTTON_TYPE  ftype;          // switch, key, radiobutton,...
                 _UWORD        group;          // radiobuttongroup
                 _TIP          tip;            // tipfunction
                 _UWORD        summary;
                 _DIRECTION    direction;
                 _ENTER_PLUS   enter_plus;
                 _ACCESS       access;
                 _FRAMETYPE    frametype;      // frametype
                 _LINKSCHEME   dyncolback;     // dynamic color background (hatch)
                 _LINKSCHEME   dyncolfront;    // dynamic color background (hatch)
                 _LINKSCHEME   dyncoltl;       // dynamic color frame (top-left)
                 _LINKSCHEME   dyncolbr;       // dynamic color frame (bottom-right)
                 _MYSCHEME     stscheme;       // statescheme
                 _BUTTONIMAGES buttonimages;
                 _VARLABEL     varlabel;       // server
                 _ANYPARA      text;           // released text
                 _ANYPARA      text1;          // pressed text
                 _FONT         font;
                 _COLOR        coltxt;
                 _LINKSCHEME   dyncoltxt;      // dynamic color txt
                 _UWORD        hotkeycode;     // scancode hotkey
                 _BOOL         doimage;
                 _BOOL         lock_overload;
                 _BOOL         doubleclick;    // is doublecklick necessary
                 _UBYTE        searchchoice;   // KM Suchwahl für Cursorsteuerung
                 _UBYTE        wose;           // aufhängepunkt für images
                 _SELECTOR     selector;       // default, only keyboard/touch
                 _BOOL         act_momentary;
                 _DWORD        set_val;
                 _DWORD        reset_val;
                 _ANYPARA      newbubbletext;

                 _BOOL         state;          // activated, inactivated
                 _BOOL         oldstate;       // activated, inactivated
                 _BOOL         initcombi;
                 _WORKSTATE    workstate;      // start, ready, run ...
                 _SubPicture   *SubPicture;    // -> to linked object
                 _BOOL         selected;
                 _IMAGE        image;
                 _COLOR        refcolor;           // reference
                 _COLOR        refcolorframe;      // reference
                 _COLOR        refcolortxt;
                 _UWORD        oldstatescheme;
                #ifdef USER_DEMAG
                 _BOOL         enable;
                #endif
                #ifdef UC_CHANGE_IO_GROUP
                 _IOGRPCHX     imiogroupchanged;
                #endif
                 _CRC32        reftxtcrc;
                #ifdef NINETEEN
                 _BOOL         full_access_given; // chkbit = true, statescheme = active, access = given
                #endif
                 _USERTEXT     usertext;
                 _USERTEXT     usertext1;
                 _UWORD        keycode[4];
                 _UDWORD       fingercount;
               }
_BUTTON;


typedef struct { _UWORD        no;
                 _BUTTON       *ptr;
               }
_MEM_BUTTON;

typedef struct { _UWORD        no;
                 _ASCII        *ptr;
               }
_DPNE;

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
#define ZO_OSZI         13
#define ZO_COLRECTANGLE 14
#define ZO_COLTEXT      15
#define ZO_COLCIRCLE    16
#define ZO_COLPOLYGON   17
#define ZO_CAROUSEL     18 // wird beim laden in ZO_MOTION umgewandelt

 
typedef struct { _UBYTE  ftype;
                 _UWORD  no;
               }
_ZORDER;

typedef struct { _UWORD  no;
                 _ZORDER *ptr;
               }
_MEM_ZORDER;

typedef struct { _BOOL             change;    // change-state
                 _ROOM             room;
                 _COLOR            color;
                 _MEM_RECTANGLE    memrectangle;
                 _MEM_POLYGON      mempolygon;
                 _MEM_CIRCLE       memcircle;
                 _MEM_PICTEXT      mempictxt;
                 _MEM_OBJECT       memobject;
                 _MEM_BITMAP       membitmap;
                 _MEM_INPUT        meminput;
                 _MEM_OUTPUT       memoutput;
                 _MEM_MOTION       memmotion;
                 _MEM_COL_RECT     colrect;
                 _MEM_COL_POLY     colpoly;
                 _MEM_COL_CIRC     colcirc;
                 _MEM_COL_PICTXT   colpictxt;
                 _MEM_KEY          memkey;
                 _MEM_BUTTON       membutton;
                 _MEM_ZORDER       memzorder;
                 _FAST_ASCII_BUFFER enclosed_name;
               }
_PICTUREMASK;

typedef struct { _FRAMETYPE        frametype;      // frametype
                 _FONT             fontscheme;
                 _ATTRIB           contextalign;
                 _COLOR            colback;
                 _COLOR            colfront;
                 _COLOR            colhatch;
                 _COLOR            coltopleft;
                 _COLOR            colbottomright;
                 _LINKSCHEME       *pdyncolback;
                 _LINKSCHEME       *pdyncolfront;
                 _LINKSCHEME       *pdyncolhatch;
                 _LINKSCHEME       *pdyncoltopleft;
                 _LINKSCHEME       *pdyncolbottomright;
               }
_STYLESHEET;

typedef struct { _IMA     ima;
                 _UDWORD  no;
               }
_WHOAMI;

typedef struct { _ROOM             Position;
                 _COLOR            ColorBack;
                 _COLOR            ColorHatch;
                 _COLOR            ColorTopLeft;
                 _COLOR            ColorBottomRight;
                 _UDWORD           FrameId;
                 _DOT              FirstInput;
                 _BOOL             DrawShadow;
                 _BOOL             TouchEditorNumeric;
                 _BOOL             TouchEditorAscii;
                 _BOOL             GlobalHotkeys;
} 
_NEWSCREENPROP;

#define EDITORCHRS 300
typedef struct { _CHAR    fstring[EDITORCHRS];
                 _UWORD   pos;
                 _UWORD   max;
                 _UWORD   no;
                 _BOOL    change;
                 _UWORD   cursory;
                 _UWORD   ysize;
                 _CHAR    chr;
                 _BOOL    incdec;
                 _BOOL    editshuttle;
                 _UDWORD  timer_cursorjump;
//                 _UWORD   japan_pos;
//                 _UWORD   japan_change;
                 _UWORD   japan_begin_pos;
               }
_CHREDIT;

typedef struct { _ROOM       room;
                 _BUBBLE     xstate;
                 _FONT       font;
                 _ATTRIB     attrib;
                 _COLOR      coltxt;
                 _COLOR      colframe;
                 _COLOR      colback;
                 _FRAMETYPE  frametype;
                 _CHAR       *txt;
               }
_BUBBLEHELP;

typedef struct { _UDWORD delay_timer;
                 _BOOL   delay_active;
                 _BOOL   active;
               }
_MENCURSOR;

typedef struct { _UWORD      actlayer;         // actual index of subpicture
                 _UWORD      actno;              // actual no of input (editor)
                 _VARIABLE   variable;
                 _INPUT      input;
                 _RESULT     result;
                 _CHREDIT    chredit;            // just stupid lineeditor
                 _DWORD      incdec;
                 _UDWORD     timer;
                 _BUBBLEHELP bubblehelp;
                 _MENCURSOR  mencursor;
                 _BOOL       enterdirection;
                 _BOOL       disable_direction_enter;
                 _UDWORD     japan_input_mode;
//                 _UWORD      japan_count_ENTER;
//                 _UWORD      japan_count_SPACE;
//                 _UWORD      japan_pos_dictionary;
//                 _CHAR       japan_keyword[32];
               }
_EDITOR;

typedef struct { _UWORD      key;
                 _UDWORD     timestamp;
                 _UWORD      count;
                 _UWORD      *keytab;
                 _UWORD      no;
                 _UDWORD     delaytime;
               }
_MULTIKEYBOARD;

typedef struct { _UWORD      nr_subpict;         // actual index of subpicture
                 _UWORD      actno;              // act. no of output
               }
_PARA_PICKER;

typedef struct { _UWORD actlayer;               // actual subpicture
                 _UWORD soft_floor;             // actual no of softkeyfloor
                 _UWORD language;               // actual language
                 _UWORD actrefresh_cnt;         // actual refreshcounter
               }
_ACTUAL;

typedef struct { _ATTRIB        attrib;            // Attribut
                 _COLOR         color;             // color button
                 _COLOR         frame;             // color frame
                 _COLOR         text;              // color text
                 _TIP           tip;               // Tipfunktion
                 _ANYPARA       anypara;           // Icon, Textgruppennummer
                 _FONT          font;
                 _CHKBIT        chkbit;            // chkbit
                 _ACCESS        access;            // key access
                 _GFUNCT        gfunct;            // functionality
                 _FRAMETYPE     frametype;         // frametype
                 _BUTTONIMAGES  buttonimages;
                 _VARLABEL      varlabel;          // server
                 _UBYTE         valid;             // is softkey valid
                #ifdef UC_DYN_SOFTKEYS
                 _MYSCHEME      stscheme;
                 _LINKSCHEME    dyncolback;        // dynamic color background (hatch)
                 _LINKSCHEME    dyncolfront;       // dynamic color background (hatch)
                 _LINKSCHEME    dyncoltl;          // dynamic color frame (top-left)
                 _LINKSCHEME    dyncolbr;          // dynamic color frame (bottom-right)
                 _LINKSCHEME    dyncoltxt;         // dynamic color text
                #endif
                 _UBYTE         wose;              // aufhängepunkt für images
                 _BOOL          act_momentary;
                 _DWORD         set_val;
                 _DWORD         reset_val;
                 _ANYPARA       newbubbletext;

                 _CSTATE        cstate;
                 _BOOL          state;             // pressed, released
                 _BOOL          oldstate;          // pressed, released
               }
_SINGLESOFT;

#define NO_SOFT_FLOOR  12                              // no of softkeys/floor
typedef struct { _SINGLESOFT    floor[NO_SOFT_FLOOR];
               }
_SOFTFLOOR;

typedef struct { _DWORD begin;     // v: first line no on screen / h: first column no on screen
                 _DWORD pos;       // v: actual line position / h: actual column position
                 _DWORD no;        // v: number of lines / h: number of columns
                 _UWORD height;    // v: no of lines/page / h: number of columns/page (pagewith)
                 _UWORD width;     // nc
                 _DWORD oldpos;
                 _DWORD oldbegin;
                 _UBYTE *lineheight;  // lineheight for each line
                 _UDWORD totalheight;
               }
_POSITION;

typedef struct { _EVENTTYPE  ftype;
                 _UWORD      scancode;
                 _UWORD      modifier;
                 _DOT        dot;
                 _HID_BUTTON button;
                 _UWORD      user[2];
                 _UWORD      selfmade;
                 _UWORD      source;
                 _UWORD      state;
                 _BOOL       is_character;
                 _DOT        raw;
                 _BOOL       lrm;                // lasal remote commander
                 _UDWORD     timestamp;
                 _UDWORD     multindex;
                 _UDWORD     already_used;       // counter if event is already in use
               }
_EVENT;

#ifdef UC_MULTITOUCH
#ifdef UC_FLICK_ZOOM
#define TOUCH_BUFFER_SIZE 100
typedef struct { _UDWORD     dotindex;
                 _DOT        dot[TOUCH_BUFFER_SIZE];
                 _UDWORD     timestamp;
               }
_MULTIEVENT;
#endif
#endif

typedef struct { _BOOL       write;
                 _LANGUAGE   act_language;
                 _UDWORD     delaytime_screensave;
                 _BOOL       seek_algorithm;
                 _BOOL       commasign;
                 _BOOL       americandateformat;
                 _BOOL       directiononenter;
                 _BOOL       cursorwraphorizontal;
                 _BOOL       cursorwrapvertical;
                 _BOOL       noexterncom;
                 _BOOL       americantimeformat;
               }
_SYSTEM;


#define SINGLEALARMMAXPARANO 6
typedef struct { 
                #ifdef SA34706
                 _UDWORD      no;
                #else
                 _UWORD       no;
                #endif
                 _UBYTE       state;
                 _UBYTE       no_para;
                 _IVALUE      para[SINGLEALARMMAXPARANO];
                 _IVALUE      time_on;
                 _IVALUE      date_on;
                 _IVALUE      time_off;
                 _IVALUE      date_off;

                 _UBYTE       acknowledge;
                 _UWORD       cnt;
                 _ANYTHING    anything;
                #ifdef UC_STATIC_ALARMSIZE
                  _UBYTE      ptr[UC_STATIC_ALARMSIZE];
                #else
                 void         *ptr;
                 _UWORD       ptrsize;
                #endif
                 _UWORD       entrytype;
                 _UDWORD      seqno;
                 _BOOL        was_locked;
                 _UDWORD      alarmid;
                #ifdef UC_MULTICPU
                 _UBYTE       multicpuno;
                #endif
               }
_SINGLEALARM;

typedef struct { _UBYTE       index;
                 _UBYTE       kennung;
                 _UWORD       no;
                 _UDWORD      info;
               }
_ALACELL;


#define TREEITEMBLOCK  32

struct _subtree;
typedef enum   
{ 
  TREE_OPEND,
  TREE_CLOSED,
}
_TREEST;

#define TREE_MARKED     1
#define TREE_LASTLINE   2

typedef struct 
{ 
  _UDWORD  data[2];
  _UDWORD  flags;
  _subtree *psubtree;
  _TREEST  subtreestate;
}
_TREEITEM;

typedef struct _subtree
{
  _UDWORD   no;          // anzahl
  _UDWORD   done;        // already allocated items
  _TREEITEM *ptr;        // --> auf daten
  _UDWORD   visuallines; // anzahl der aktuell sichtbaren zeilen
  _UDWORD   markedlines; // anzahl der aktuell markierten zeilen
}
_TREE;

typedef struct
{
  _UDWORD   no;
  _TREEITEM *ptr[128];
}
_TREESCOPE;


//struct __POPMENU;

typedef void (*_PTRLINE) (_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
typedef void (*_PTRSTART)(_Window*, _PopUp*, _BOOL);
typedef void (*_PTRREADY)(_Window*, _PopUp*, _BOOL);
typedef void (*_PTRRUN)  (_Window*, _PopUp*, _EVENT*);
typedef void (*_PTREND)  (_Window*, _PopUp*);

#ifdef _TARGETARCH_ARM
	#pragma pack (push, 2)
#endif
typedef struct { _INTERFACE typ;
                 _UWORD     adress;
                 void*      handle;
                 _UDWORD    ipadress;
                 _UDWORD    port;
                 _UDWORD    free[9];
               }
_COMDEF;
#ifdef _TARGETARCH_ARM
 #pragma pack (pop)
#endif

#ifdef _TARGETARCH_ARM
	#pragma pack (push, 4)
#endif
typedef struct { _UDWORD   id;
                 _UWORD    channel;
                 _UWORD    varpos;
                 _UWORD    xtime;
                 _UDWORD   varlistid;
               }
_LSLCOMREGDAT;
#ifdef _TARGETARCH_ARM
 #pragma pack (pop)
#endif


typedef struct { _CHKBIT       chkbit;
                 _VARLABEL     value;
                 _GFUNCT       gfunct;
                 _MEM_VARLABEL memvar;
                 _ANYPARA      anypara;
                 _MYSCHEME     stscheme;
               }
_LINEINFO;

typedef struct { _UWORD        no;
                 _LINEINFO     *ptr;
               }
_MEM_LINEINFO;

struct _FW_SCROLL;


typedef void (*_SCR_LINE) (_FW_SCROLL*, void*, _ROOM*, _UDWORD, _BOOL);
typedef void (*_SCR_BEAM) (_FW_SCROLL*, void*, _UWORD, _UWORD, _BOOL);
typedef void (*_SCR_DRAW) (_FW_SCROLL*);
typedef void (*_SCR_START)(_FW_SCROLL*, _BOOL);
typedef void (*_SCR_READY)(_FW_SCROLL*, _BOOL);
typedef void (*_SCR_RUN)  (_FW_SCROLL*, _EVENT*);
typedef void (*_SCR_END)  (_FW_SCROLL*);

typedef struct _FW_SCROLL
               { _ROOM      out_room;
                 _ROOM      scroll_room;
                 _FONT      font;
                 _ATTRIB    attrib_inactiv;
                 _ATTRIB    attrib_activ;
                 _COLOR     color_inactiv;
                 _COLOR     color_activ;
                 _COLOR     frame_out;
                 _COLOR     frame_scroll;
                 _UWORD     line_width;
                 _UWORD     line_height;
                 _POSITION  position;
                 _POSITION  hposition;
                 _LEVEL     level;
                 _LEVEL     hlevel;
                 _SCR_LINE  pt_line;   // --> to userdefined linedraw-interface
                 _SCR_BEAM  pt_beam;   // --> to userdefined beamdraw-interface
                 _SCR_DRAW  pt_draw;   // --> to userdefined backgrounddraw-interface
                 _SCR_START pt_start;  // --> to userdefined init-interface
                 _SCR_READY pt_ready;  // --> to userdefined ready-interface
                 _SCR_RUN   pt_run;    // --> to userdefined run-interface
                 _SCR_END   pt_end;    // --> to userdefined end-interface
                 void       *pt;
                 _MENCURSOR mencursor;
                 _Cursor    *Cursor;
                 _BOOL      noclrscr;
                 _WORD      cursor_dx1;
                 _WORD      cursor_dy1;
                 _WORD      cursor_dx2;
                 _WORD      cursor_dy2;
                 _FRAMETYPE frametype;
                 _UDWORD    do_out_timer;
                 _BMP       *image;
                 _COLOR     flash_cursorcolor;
                 
                 _BOOL      nobck;
               }
_SCROLL;

typedef struct { _VARLABEL     link;
                 _UWORD        priority;
                 _CHKBIT       chkbit;
                 _UWORD        group;
                 _UWORD        userconfig;
                 _COLOR        active;
                 _COLOR        passive;
                 _COLOR        quit_active;
                 _COLOR        quit_passive;
                 _ANYPARA      text;
                 _ANYPARA      helptext;

                 _MEM_VARLABEL parameter;  // parametersatz
                 _UWORD        alano;      // alarm number
                 _UWORD        mode;       // userdefined mode
               }
_SINGLE_ALARM;

typedef struct { _UBYTE        opchg;
                 _UBYTE        mode;
                 _UWORD        alano;
               }
_ALACONF;

typedef struct { _UWORD        lng;
                 _ALACONF      alaconf;
                 _UDWORD       para[1];
               }
_CPUALARM;

typedef struct { _UWORD        no;
                 _SINGLE_ALARM *ptr;
               }
_ALARM;

#ifdef OK_SUBLIST
typedef struct { _UDWORD hide;      // --> to hidden elements
                 _UDWORD next;      // --> to next element
                 _UDWORD jump;      // --> to next element with same depth
                 _UDWORD no;        // real entry number
                 _UDWORD depth;     // depth
                 _UDWORD data;      // userdefined data
                 _UDWORD parent;    // parent (übergeordnetes element)
               }
_VKLIST;

typedef struct { _VKLIST *ptr;       // --> to entries
                 _UDWORD  no;        // no of entries
                 _UDWORD  visual_no; // no of open entries (visual entries)
                 _UDWORD  done_no;   // no of already allocated and initialized entries
               }
_MEM_VKLIST;
#endif

typedef struct { _UWORD  height;      // softkey-height
                 _UWORD  width;       // softkey-width
                 _UWORD  no;          // no of softkeys/floor
                 _BOOL   horizontal;  // direction (vertical/horizontal)
                 _ATTRIB align;       // alignment
               }
_SOFTKEYSETUP;

typedef struct { _DOT    HidMemoLeft;
                 _DOT    HidMemoRight;
                 _EVENT  TriggerEvent;
                 _BOOL   TriggerEventState;
                 _UDWORD TriggerEventTimex;
                 _UDWORD TriggerEventTimexDuration;
                 _BOOL   PressMemoLeft;
                 _BOOL   PressMemoRight;
               }
_EVENTMEMO;


// *************************************************************************
// *** COMMUNICATION *******************************************************
// *************************************************************************

#ifdef UC_NEW_ALARMSYSTEM
 #ifndef ALARMBUFFERNAME
  #define ALARMBUFFERNAME         "_AlarmXBuffer1"
 #endif
 #define COMMAND_REGISTERHMI        200
 #define COMMAND_GETENTRY           202
 #define COMMAND_GETVERSION         205
 #define COMMAND_GETALARMCONFIG       1
 #define COMMAND_GETALARMS            2
 #define COMMAND_QUIT                 3
 #define COMMAND_QUIT_GROUP           4
 #define COMMAND_GETUNREGID           6 // erst ab version 2
 #define COMMAND_GETALARMCONFIGUNREG  7 // erst ab version 2
 #define COMMAND_GETALARMCONFIGMULTI 11 // erst ab version 4
 
#endif


typedef struct { _UDWORD   offset;
                 _LASALID  lasalid;   // lasalid
                 _UWORD    ftime;     // refreshtime for plc (unit ms)
                 _UWORD    size;      // size of data
                 _UBYTE    valid;     // 0..invalid data, 1..valid data
               }
_INDEX;                               // single index for _INDEXDAT

typedef struct { _UWORD    permanent; // permanent used _INDEX
                 _UWORD    used;      // used _INDEX incl. permanent
                 _UWORD    no;        // no of allocated _INDEX
                 _UWORD    no_sent;   // no of already sent entrys
                 _UWORD    no_max;    // max number of entries in reflist
                 _INDEX    *data;     // --> to _INDEX
               }
_INDEXDAT;                            // indexdata for one plc

typedef struct { _UDWORD   permanent; // permanent used _UBYTE
                 _UDWORD   used;      // used _UBYTE incl. permanent
                 _UDWORD   size;      // no of allocated _UBYTE
                 _UBYTE    *data;     // --> to _UBYTE
               }
_REFDAT;                              // refreshdata for one plc

typedef struct { _INDEXDAT indexdat;
                 _REFDAT   refdat;
                 _LOGSTATE login;
                 _BOOL     permanent_enable;
               }
_REFLIST;                             // refreshlist for one plc



#define ASYNCPING_OFFLINE  0
#define ASYNCPING_BUSY     1
#define ASYNCPING_ONLINE   2
#define DEFAULTCRC 0
typedef struct { _COMDEF  comdef;
                 _REFLIST reflist;
                 _UDWORD  crc;
                 _UBYTE   offline_cnt;
                 _BOOL    observe;
                 _BOOL    offline;
                 _BOOL    used;
                #ifdef UC_NEW_ALARMSYSTEM
                 _UDWORD  alarmObject;
                 _UDWORD  oldValue;
                 _UDWORD  alarmHandle;
                 _UDWORD  alarmValidIndex;
                 _UDWORD  alarmAnzahl;
                 _UDWORD  alarmRecordSize;
                 _UDWORD  alarmXBufferSize;
                 _UDWORD  alarmVersion;
                 _BOOL    alarm_disable;
                #endif
                 _UDWORD  add_reflist[2];
                 _UDWORD  async_pingstate; // ASYNCPING_OFFLINE, ASYNCPING_BUSY, ASYNCPING_ONLINE
                 _DWORD   plcstate;
                 _UDWORD  async_onlstate;
                 _UDWORD  async_onltimex;
                 _UDWORD  loader_version;
                #ifdef UC_GETLASALID_DATALINK
                 _UDWORD  lasalid_done;
                 void    *datalink_hdl;
                #endif 
               }
_COMPORT;

typedef struct { _UWORD   aktmap;
                #ifdef UC_MULTICPU
                 _UWORD   multimap[UC_MULTICPU];
                 _BOOL    used;
                #endif
               }
_COMAP;

typedef struct { _UWORD       no;
                 _COMPORT     *ptr;
                 _COMAP       mapper[256];
               }
_COMSET;

typedef struct { _COMSET  ComSet;
                 _UWORD   StartPicture;
                 _UDWORD  LoginDelayTime;
                 _ASCII   ProjectDPNE[_MAX_PATHLENGTH];
                 _ASCII   StartGfxDPNE[_MAX_PATHLENGTH];
                 _ASCII   SimulationDPNE[_MAX_PATHLENGTH];
                 _ASCII   DataDPNE[_MAX_PATHLENGTH];
               }
_IPCINI;

//#ifdef UC_MULTICPU
//typedef struct { _UBYTE       no;
//                 _UBYTE       comportno[UC_MULTICPU];
//               }
//_MULTICPUGUIDE;
//#endif

typedef struct {
                 _LOCATION    location;
                 _UWORD       no;
                 _UWORD       karuindex;
                 _DIRECTION   direction;
               }
_SINGLESEEK;

#ifndef MAX_SEEKELEMENT
 #define MAX_SEEKELEMENT 9 // vorsicht bei änderung auch in t_types.h ändern
#endif
typedef struct {
                 _UWORD       no;
                 _SINGLESEEK  ptr[MAX_SEEKELEMENT];
                 _UWORD       ioid;
                 _DIRECTION   direction;
                 _UWORD       hotkeycode;
               }
_SEEKELEMENT;

typedef struct {
                 _UWORD       no;
                 _UWORD       doneno;
                 _SEEKELEMENT *ptr;
               }
_MEM_SEEKELEMENT;

typedef struct { void        *PScreenObject;    // muss wie bei _MENU an erster stelle sein
                 _UWORD       No;                                  // number of subpictures
                 _SubPicture *Ptr;                                     // --> to subpicture
                 _MEM_SEEKELEMENT memseekelement;
                 _EDITOR      Editor;
                 _ROOM        Room;
                 _ACTUAL      Actual;
                 _Window     *P_Window;
                 _ZOOM        PicZoom;
                 _PARA_PICKER ParaPicker;
                 _DOT         FirstInput;
                 _BOOL        TouchEditorNumeric;   // _TRUE ... able to open num toucheditor
                 _BOOL        TouchEditorAscii;     // _TRUE ... able to open anum toucheditor
                 _BOOL        DrawShadow;           // _TRUE ... if window draw shadow
                 _LSEFUNCT    StartFunct;
                 _LSEFUNCT    EndFunct;
                 _BOOL        GlobalHotkeys;
                 _BOOL        JustProgram;
                 _UWORD       RememberEnterPlus;
               }
_MY_PICTURE;

typedef struct { _UDWORD      no;
                 void       **ptr;
               }
_MEM_POINTER;

typedef struct { void        *ptr;
                 _UDWORD     para;
               }
_POINTERPARA;               

typedef struct { _UDWORD      no;
                 _UDWORD      doneno;
                 _POINTERPARA *ptr;
               }
_MEM_POINTERPARA;


typedef struct { _UDWORD  actiontime;
                 _UWORD   image;
                 _BOOL    clrscr;
               }
_SCREENSAVER;

typedef struct { _COLOR     ButtonColorReleased;
                 _COLOR     ButtonColorPressed;
                 _COLOR     ButtonColorText;
                 _COLOR     TitleColorText;
                 _COLOR     LimitColorText;
                 _COLOR     WindowColorBack;
                 _FRAMETYPE ButtonFrame;
                 _FRAMETYPE WindowFrame;
                 _UWORD     StandardFontSchemeNo;
                 _UWORD     SmallFontSchemeNo;
                 _ASCII     StandardFontSchemeName[32];
                 _ASCII     SmallFontSchemeName[32];
                 _COLOR     TitleBarColor;
               }
_DEFKEYPAD;

#ifdef LASAL
 #ifdef _GNUC
  typedef int size_t;
 #endif
typedef struct
{
	void (* clearerr)(void *);
	int (* fclose)(void *);
	int (* _fcloseall)(void);
	int (* feof)(void *);
	int (* ferror)(void *);
	int (* fflush)(void *);
	int (* fgetc)(void *);
	int (* _fgetchar)(void);
	int (* fgetpos)(void *, void *);
	char * (* fgets)(char *, int, void *);
	int (* _fileno)(void *);
	int (* _flushall)(void);
    void * (* fopen)(const char *, const char *);
	int (* fprintf)(void *, const char *, ...);
	int (* fputc)(int, void *);
	int (* fputs)(const char *, void *);
	size_t (* fread)(void *, size_t, size_t, void *);
	void * (* freopen)(const char *, const char *, void *);
	int (* fscanf)(void *, const char *, ...);
	int (* fsetpos)(void *, const void *);
	int (* fseek)(void *, long, int);
	long (* ftell)(void *);
	size_t (* fwrite)(const void *, size_t, size_t, void *);
	int (* remove)(const char *);
	int (* rename)(const char *, const char *);
	void (* rewind)(void *);
	int (* _rmtmp)(void);
	void (* setbuf)(void *, char *);
	int (* setvbuf)(void *, char *, int, size_t);
	char * (* _tempnam)(const char *, const char *);
	void * (* tmpfile)(void);
	char * (* tmpnam)(char *);
	int (* ungetc)(int, void *);

}_FILE;
#endif

typedef struct { _ASCII  dpne[_MAX_PATHLENGTH];
                 _BMP    bmp;      // --> to data
               }
_GFXBMP;

typedef struct { _UWORD  label;    // ('BM')
                 _UDWORD fsize;    // filesize
                 _UWORD  nc1;      // (0)
                 _UWORD  nc2;      // (0)
                 _UDWORD offs;     // offset of bitpattern (reference filestart)
               }
_BMPHEAD;

typedef struct { _UDWORD size;     // length of bitpatterninfo
                 _UDWORD width;    // Bitmapbreite in Pixel
                 _UDWORD height;   // Bitmaphoehe in Pixel
                 _UWORD  planes;   // (1)
                 _UWORD  bitcnt;   // (4) Zahl der Bits pro Pixel
                 _UDWORD compr;    // (0) Art der komprimierung
                 _UDWORD sizeim;   // Bildgroesse in Bytes
                 _UDWORD nc3;
                 _UDWORD nc4;
                 _UDWORD nc5;
                 _UDWORD nc6;
               }
_BMPINFO;

typedef struct { _UBYTE  cblue;
                 _UBYTE  cgreen;
                 _UBYTE  cred;
                 _UBYTE  alpha;
               }
_RGB32;

typedef struct { _UBYTE  cblue;
                 _UBYTE  cgreen;
                 _UBYTE  cred;
               }
_RGB24;

typedef struct { _UBYTE  cred;
                 _UBYTE  cgreen;
                 _UBYTE  cblue;
               }
_RGB256;

typedef struct { _UBYTE  cblue;
                 _UBYTE  cgreen;
                 _UBYTE  cred;
                 _UBYTE  cres;
               }
_RGBBMP;

typedef struct { _VARLABEL   vl;
                 _DWORD      YMin;
                 _DWORD      YMax;
                 _COLOR      color;
                 _RINGBUFFER ringbuffer;
                 _DWORD      *data;
               }
_LSEOSZI_CHANNEL;

typedef struct { _UWORD           no;
                 _LSEOSZI_CHANNEL *ptr;
                 _RINGBUFFER      ringbuffer;
                 _DATIM           *datim;
               }
_MEM_LSEOSZI_CHANNEL;

typedef struct { _UDWORD change;
                 _MEM_LSEOSZI_CHANNEL channels;
               }
_LSEOSZI;

#define OSZICHANNELS 8

typedef struct { _UDWORD startaddr;
                 _UDWORD totalsize;
                 _UDWORD usedmem;
                 _UDWORD freemem;
               }
_HEAPINFO;

typedef struct { void      *heap_mat;
                 _HEAPINFO *applmeminfo;
                 void      *ChkHeapList;
               }
_HEAP_TYPE;

typedef struct _V_HEAPBLOCK 
               {
                  _V_HEAPBLOCK *Next;
                  _UDWORD      BlockSize;
               }
_HEAPBLOCK;

typedef struct { _HEAPBLOCK *UsedList;
                 _HEAPBLOCK *FreeList;
               }
_HEAP;

typedef struct { _UWORD no;
                 _UWORD *code;
               }
_HOTKEYLIST;

struct _FW_PICINDEX;
typedef struct _FW_PICINDEX
{
  _UWORD    picno;
  _UWORD    no;
  _UWORD    valid;
  _FW_PICINDEX *ptr;
}
_PICINDEX;

typedef struct { _KOORD   x;
                 _KOORD   y;
                 _KOORD   z;
               }
_DOT3D;

typedef struct { _BOOL    convex;
					  _DWORD   pad;
					  _DWORD   pbd;
					  _DWORD   pcd;
					  _DWORD   pdd;
					  _KOORD   xmax;
					  _KOORD   xmin;
                 _KOORD   ymax;
                 _KOORD   ymin;
                 _KOORD   zmax;
                 _KOORD   zmin;
                 _DOT     ptr2d[100];
               }
_SHAPEHEADER;


typedef struct { _UWORD   no;
                 _DOT3D   *ptr;
                 _COLOR   color;
                 _COLOR   backcolor;
                 _SHAPEHEADER header;
               }
_SHAPE;

typedef struct { _KOORD  xmax;
                 _KOORD  xmin;
                 _KOORD  ymax;
                 _KOORD  ymin;
                 _KOORD  zmax;
                 _KOORD  zmin;
               }
_3DSHAPEHEADER;

typedef struct { _UWORD   no;
                 _SHAPE   *ptr;
                 _BOOL    shading;
                 _BOOL    frame;
                 _3DSHAPEHEADER  header;
			   }
_3DSHAPE;

// *************************************************************************
// *** GRAPHIC STUDIO ******************************************************
// *************************************************************************
#ifdef UC_GRAPHIC_STUDIO
typedef struct { _CIRCLE    circle;
								 _ROOM      origin;
							 }
_CIRCLE2D;

typedef struct { _POLYGON   polygon;
                 _DOT       *origin;
							 }
_POLYGON2D;

typedef struct { _UWORD       no;
                 _CIRCLE2D    *ptr;
               }
_MEM_CIRCLE2D;

typedef struct { _UWORD       no;
                 _POLYGON2D   *ptr;
               }
_MEM_POLYGON2D;

typedef struct { _MEM_CIRCLE2D     mem_circle2d;
                 _MEM_POLYGON2D    mem_polygon2d;
                 _MEM_ZORDER       mem_zorder;
               }
_PICTUREMASK2D;
#endif

typedef struct { _UDWORD      no;
                 _UDWORD      *ptr;
               }
_MEM_MAP;

typedef struct { _UDWORD      no;
                 void         *ptr;
               }
_DATA;

typedef struct
{

  _UDWORD timemin;
  _UDWORD timemax;
  _UDWORD timeaverage;
  _UDWORD timebase; // base for average;

  _UDWORD min;
  _UDWORD max;
  _UDWORD average;
  _UDWORD ref;
  _UDWORD sum;
  _UDWORD time;
  _UDWORD count;

  _BOOL valid;
}
_LSE_TIME;

#ifdef LASAL
 #pragma pack (pop)
#endif

// *************************************************************************
// *** GLOBAL CLASSES ******************************************************
// *************************************************************************
#ifdef LASAL
 #pragma pack (push, 1)
#endif

#define MESSAGEBOXNO 10
class _MessageBox
{
  private : 
    _MESSAGEBOX MessBox[MESSAGEBOXNO];
    _UDWORD     LastUsedIndex;
    
    void        Init_MESSAGEBOXBUTTON(_MESSAGEBOXBUTTON*, _FONT, _MESSAGEBOXRESULT, void*, _FVOID_MESSAGEBOXRESULT);
    void        Init_MESSAGEBOX(_MESSAGEBOX*, void*, _UWORD, _FONT, _MESSAGEBOXTYP, _UDWORD, void*, _FVOID_MESSAGEBOXRESULT);
    _UDWORD     GetUnused(void);
    _UDWORD     GetButtonNo(_MESSAGEBOX *pbox);
    _BOOL       ButtonKey(_MESSAGEBOX *pbox, _UDWORD scancode);
    _BOOL       ButtonPress(_MESSAGEBOXBUTTON *pmb);
    void        DrawButtonSingle(_MESSAGEBOXBUTTON *pmb, _BOOL state);
    void        DrawButtonAll(_ROOM *pr, _MESSAGEBOX *pbox);
    _BOOL       ButtonPosition(_ROOM *pr, _MESSAGEBOX *pbox);
    _BOOL       ButtonHid(_MESSAGEBOX *pbox, _DOT *pxy);
   
  public :
    _FRAMETYPE  FrameTypeBox;
    _FRAMETYPE  FrameTypeButton;
    _UWORD      WidthBox;
    _UWORD      WidthButton;
    _UWORD      HeightBox;
    _UWORD      HeightButton;
    _COLOR      ColorBoxBackground;
    _COLOR      ColorBoxText;
    _COLOR      ColorBoxFrame;
    _COLOR      ColorButtonBackground;
    _COLOR      ColorButtonText;
    _COLOR      ColorButtonFrame;
    _FONT       FontScheme;
  
    void        Init(void);
    void        Free(void);
    _MESSAGEBOX *Get(_UDWORD);
    void        Run(_MY_PICTURE *mp, _EVENT *pe, _UDWORD messboxindex);
    void        Ready(_MY_PICTURE *mp, _UDWORD messboxindex);
    
    _BOOL       Call(void *text, _UWORD chrsize, _FONT font, _MESSAGEBOXTYP typ, void *pthis, _FVOID_MESSAGEBOXRESULT pfkt);
};

#ifdef UC_NEWCOLORPALETTE
typedef struct { _UWORD cold;
                 _UWORD cnew;
               }
_C16IX;

typedef struct { _UWORD color;
                 _UWORD done;
               }
_COL16;
#endif

class _Palette
{
  private :
   #ifdef UC_NEWCOLORPALETTE 
    _UDWORD OldNewColAnzahl;
    _C16IX  OldNewCol16[256];
    _COL16  NewCol16[256];
    void    MakeOldNewColorTable(void);
    _UDWORD FindIndexSub(_UDWORD start, _UDWORD anz, _UWORD col16);
   #endif

    _UDWORD TargetColor;
    _UBYTE  RgbPtr[256*3];
    _BOOL   Read(_UBYTE*, _File*);
    _UWORD  MakeColor16(_UBYTE *prgb);

  public :
    void   AskSystem(void);
    void   Init(void);
    void   Free(void);
    _BOOL  Load(_ASCII*);
    void   Set(void);
    _BOOL  SetColor(_UWORD, _COLOR);
    void   Make_RGB(_RGB *rgb, _COLOR col);
    _COLOR MakeColorFromIndex(_UWORD index);
   #ifdef UC_NEWCOLORPALETTE 
    void   SetNewColorPalette(_UBYTE *prgb, _UDWORD startindex, _UDWORD anzahl);
    _UWORD GetNewColor(_UWORD col16);
   #endif
    void   Get(_UBYTE *dst, _UDWORD startidx, _UDWORD anzahl);
};

class _AsciiPuffer
{
  private :
    _ASCII_BUFFER    Abuffer;
    _MEM_POINTERPARA MemPointerPara;

  public :
	 void    Init(void);                                    // init textbuffer
	 void    Free(void);
   _BOOL   Read(_File*, _BOOL);                          // read from handle
	 _UDWORD GetOffset(_UDWORD);                         // get offset to text
	 _ASCII *GetText(_UDWORD);                          // get pointer to text
   void    StrUp(void);                            // convert to big letters
   _BOOL   Add(_ASCII*);                                       // add string
   _DWORD  GetSize(void);                  // get no of characters in buffer
   _ASCII *GetPtr(void);                            // get --> to first text
	 _UDWORD GetNo(void);                                    // get no of text
   _BOOL   MakeFastSeek(void);
   _BOOL   Seek(_UDWORD*, _ASCII*);                       // search for text
   _MEM_POINTERPARA *GetMemPointer(void);
   _BOOL   MakeMemory(_UDWORD);
   void    FinishMemory(void);
  #ifdef UC_IPC
   _UDWORD MemSize(void);
  #endif
   _ASCII_BUFFER* GetAsciiBuffer();
   _BOOL   DeepCopy(_AsciiPuffer*);
};

#pragma pack (push, 1)
typedef struct { _WORD        xoffset;
                 _WORD        FormatDate;
                 _WORD        FormatTime;
                 _BOOL        ShowDate;
                 _BOOL        ShowTime;
                 _BOOL        ShowSize;
                 _BOOL        ShowAttribute;
                 _COLOR       ColorMarked;
                 _COLOR       ColorImage;
                 _BOOL        BoldMarked;
                 _ASCII       *FileFilter;
                 _UDWORD      ImageDrive;
                 _UDWORD      ImageDirect;
                 _UDWORD      ImageFile;
                 _ASCII       *LockedRootSign;
                 _UBYTE       DriveList[41];
                 _UDWORD      LasalOsCnt;               
                 _UWORD       TreeSize;
                 
                 _TREE        Tree;
                 _ASCII       LockedRootPath[_MAX_PATHLENGTH];
                 _ASCII       ActPath[_MAX_PATHLENGTH];
                 _UDWORD      NoList;
                 _AsciiPuffer *pList;
                 _ASCII       ExcludeDrives[_MAX_PATHLENGTH];
                 _UDWORD      SortCriteria;
                 _DWORD       WidthName;
                 _DWORD       WidthDate;
                 _DWORD       WidthTime;
                 _DWORD       WidthSize;
                 _UDWORD      ImageAttReadOnly; // sa38678
                 _UDWORD      ImageAttHidden;   // sa38678
                 _UDWORD      ImageAttArchiv;   // sa38678
                 _UDWORD      ImageAttSystem;   // sa38678
               }
_FILEXPLORER;
#pragma pack (pop)

typedef struct { _UDWORD         no;
                 _MEM_VARSYMBOL  MemVarSymbol;
                 _MEM_LSEVARINFO MemLseVarInfo;
                 _AsciiPuffer    Label;
               }
_VARBLOCK;

typedef struct
{
  _UWORD      no;
  _TextPuffer *ptr;
}
_MEM_TextPuffer;

class _TextList
{
  private :
    _UDWORD         MemTextPufferIndex;
    _UDWORD         HostLanguageId[4];
    _MEM_TextPuffer MemTextPuffer[4];
    _UBYTE          HostStack[512];
    _UDWORD         HostStackNo;
    _UDWORD         HostStackError;
    
    _BOOL           Unicode;
    _CHAR           *Get(_UWORD, _UWORD);
    _UDWORD         GetSize(_UWORD);
    _UDWORD         GetNo(_UWORD);
    _MEM_TextPuffer *GetPtm(void);


  public :
    void        Init(void);
    void        Free(_BOOL free_hostlanguage_as_well);
    _BOOL       Load(_ASCII*, _UWORD, _UWORD);
    _BOOL       LoadCompiler36(_ASCII*);
    _UDWORD     GetSize(_ANYLST);
    _UDWORD     GetNo(_ANYLST);
    _CHAR       *Get(_ANYLST, _UWORD);
    _TextPuffer *GetTextPuffer(_UWORD);
    _UWORD      GetNo(void);
	  _BOOL       SwapLanguage(_BOOL state, _UWORD language); // auf hostsprache um/zurücksachalten
    _UDWORD     HostLanguageGetEx(_UDWORD idx); // retouniert langaugeid zu übergebenen hostindex
    _UDWORD     HostLanguageGet(void); // retouniert languageid zu aktuell aktiven sprache
    _BOOL       HostLanguageSet(_UDWORD idx, _UDWORD language); // hostlanguage auf index zuweisen
    _BOOL       HostLanguagePopEx(void); // auf hostlanguage umschalten
    _BOOL       HostLanguagePushEx(_UDWORD idx); // von hostlanguage wieder zurückschalten

    _BOOL       GetUnicode(void);
   #ifdef UC_IPC
    _UDWORD     MemSize(_MEMSIZE*);
   #endif
};

typedef struct { _CHKBIT       chkbit;
                 _PICTUREMASK  mask;
                 _SUBACTUAL    actual;
                 _PICRUNMEMO   picrunmemo;
                 _TextPuffer   TextPuffer;
                 _UDWORD       MoveButtonGroup[10];
               }
_PICTURE;

#ifdef LASAL
 #pragma pack (pop)
#endif

class _Sequencer
{
  private :
    _UWORD    No;
    _COMPCODE *Ptr;
   #ifdef _STATICCHKBIT
    _UWORD    *Static;
   #endif
    _BOOL     Read(_File*);

  public :
    _AsciiPuffer LseLabel;

    void      Init(void);
    void      Free(void);
    _BOOL     Load(_ASCII*);
    _BOOL     Call(_UWORD, _BOOL, _BOOL wait4plc, _BOOL cheat);
    _BOOL     CallCompiler(_COMPCODE*, _BOOL, _BOOL wait4plc, _BOOL cheat);
    _BOOL     Get(_COMPCODE**, _UWORD);
    _UWORD    GetNo(void);
   #ifdef _STATICCHKBIT
    void      InitStatic(void);
   #endif
   #ifdef UC_IPC
    _UDWORD   MemSize(_MEMSIZE*);
   #endif

};

class _Scheme
{
  protected :
    _MEM_SCHEME    MemScheme;
    _BOOL          Read(_File*, _BOOL, _BOOL);
    _BOOL          CallSub(_UDWORD*, _MEM_SINGLESCHEME*, _VARLABEL*, _BOOL cheat);
    _BOOL          PrepaireSub(_SCHEME*, _UWORD);
    _BOOL          FinishSub(_SCHEME*, _UWORD);
    _BOOL          InsertSub_MEM_NO(_SCHEME*, _UBYTE, _UDWORD, _DWORD);

  public :
    _AsciiPuffer   LseLabel;

    void           Init(void);
    void           Free(void);
    void           Set_Wait4Plc_Cheat(_BOOL wait4plc, _BOOL cheat);
    _BOOL          Load(_ASCII*, _BOOL, _BOOL);
    _BOOL          Call(_UDWORD*, _MYSCHEME*, _VARLABEL*, _UDWORD, _BOOL cheat=_FALSE);
    _BOOL          Call(_UDWORD*, _LINKSCHEME*, _UDWORD, _BOOL cheat=_FALSE);
    _BOOL          Get(_SCHEME**, _UWORD);
    _UWORD         GetNo(void);
    _BOOL          Valid(_LINKSCHEME*);
    void           Change(_UWORD, _UWORD);
    _BOOL          Insert_MEM_NO(_MEM_NO*, _UBYTE, _UWORD);
    void           TransformToGray();
    void           SetVariableResult(void);
    _UDWORD        Add(_ASCII *label, _SCHEME *pms);
};

class _FontScheme : public _Scheme
{
  private :
    _BOOL          InternLabelDone;
    _AsciiPuffer   InternLabel;
    
    _BOOL          MakeInternLabelList(void);
    _ASCII*        MakeNewLseLabel(_ASCII *label, _UWORD no);
    
  public :
    void           Init(void);
    void           Free(void);
    _UDWORD        GetIndexByLabel(_ASCII *label);
    _ASCII*        GetLabelByIndex(_UDWORD idx);
    _BOOL          Finish(_UWORD, _UWORD);
    _BOOL          Prepaire(_UWORD, _UWORD);
    _UDWORD        AddNew(_ASCII *label, _UWORD fontindex);
};

class _TextScheme
{
  private :
    _MEM_TXTSCHEME MemTxtScheme;
    _BOOL          Read(_File*);
    _ANYPARA      *CallSub(_TXTSCHEME*, _VARLABEL*);

  public :
    _AsciiPuffer   LseLabel;

    void           Init(void);
    void           Free(void);
    _BOOL          Load(_ASCII*);
    _BOOL          Call(_MYTXTSCHEME*, _VARLABEL*);
    _ANYPARA      *Call(_UWORD, _VARLABEL*);
    _ANYPARA      *Call(_UWORD no, _DWORD value);
    _BOOL          Get(_TXTSCHEME**, _UWORD);
    _UWORD         GetNo(void);
};

class _Alarm
{
  private :
    _ALARM           Data;
    _CRC32           LseCrc32;
    _BOOL            Valid;

    _SINGLE_ALARM    *LookUp(_UDWORD);
   #ifdef UC_UNREGISTERED_ALARM
    void    DoUnregisteredAlarms(_UWORD scanbar);
    _UDWORD GetUnreg(_COMPORT *pcp, _UWORD station, _UDWORD startindex, _UWORD scanbar);
   #endif
    _BOOL   ScanDoubleConfigNo(_UDWORD nox, _SINGLE_ALARM *psrc);

  public :
    _AsciiPuffer LseLabel;

    void    Init(void);
    void    Free(void);
    _BOOL   Load(_ASCII*);
    _BOOL   Config(void);
    void    GetTempBuffer(void);
    _ALARM *GetAlarm(void);
    _ALARM *GetAlarmDirect(void);
    _BOOL   Add(_UDWORD, _BOOL unregisterd);
    _UWORD  GetNo(void);
    _UWORD  GetIndex(_UWORD config_no, _COMPORT *pcp);
    _UWORD  GetIndexByGroup(_UWORD);
    _SINGLE_ALARM *Get_SINGLE_ALARM(_UWORD);

    _CRC32  GetLseCrc32(void);
    _UDWORD GetCrc32(void);
    _BOOL   SaveAlarmCrc(void);
    _BOOL   LookForAlarmCrc(void);
    _BOOL   InsertAlarmIdByIndex(_SINGLEALARM*);
    _BOOL   InsertIndexByAlarmId(_SINGLEALARM*);
};

#ifdef UC_LINKVL_FAST
class _LinkVlBuffer
{
  private:
    _BOOL   Sealed;
    _UDWORD No;
    _UDWORD Used;
    _LINKVL *Ptr;
    
  public:  
    void    Init(void);
    void    Free(void);
    _LINKVL*Get(_UDWORD anzahl, _UDWORD *rape);
    void    Finalize(_LSEVARINFO *pvi);
};
#endif

class _Stack
{
  private :
    _STACK Stack;

//    _UWORD LinesToElseOrEnd(_STACK_LINE*, _UWORD);
    _UWORD LinesToEnd(_STACK_LINE*, _UWORD);

    _UWORD LinesToElse(_STACK_LINE*, _UWORD);
    _BOOL  DoIfElseEnd(void);
   #ifdef XUSER_KM
    _BOOL  CopyJustScreenToStack(_STACK_LINE*, _UWORD*, _Stack*);
    void   ScanForIf(void);
   #endif

  public :
    void   Init(void);
    void   Free(void);
    void   AddLine(_STACK_LINE*, _BOOL);
    _BOOL  InsertCmd(_PIC_COMMAND, _DWORD, _DWORD, _BOOL);      // add command
    _BOOL  InsCmd(_PIC_COMMAND, _VARLABEL*, _VARLABEL*, _BOOL); // add command
    _BOOL  GetCommand(_STACK_LINE*);                    // get first command
    _BOOL  Read(_File*);                             // load stack from file
    _BOOL  AddStack(_Stack*, _BOOL);                            // add second stack
    _BOOL  AddPart(_STACK_LINE*, _UWORD, _BOOL);
    _BOOL  Add_LSEFUNCT(_LSEFUNCT*, _BOOL);
    _UWORD GetNo(void);                                // get no of commands
    _STACK_LINE *GetPtr(void);                   // get --> to first command
    _BOOL  LookUpCommand(_PIC_COMMAND);                  // lookfor command
    _STACK *GetStack(void);
    _BOOL  MoveStackWithoutCompromise(_STACK*);
    _BOOL  InsertStack(_Stack*, _BOOL);
   #ifdef XUSER_KM
    _BOOL  GetScreenNo(_Stack*, _UWORD*, _UWORD*);
   #endif
    void   DeleteAllCmd(void);
    _BOOL  Overload(_VARLABEL *po);
};

#ifdef UC_BUBBLE_HELP
class _NewBubbleHelp
{
  private :
    _CHAR      *Text;
    _UDWORD    TextLen;
    _ROOM      Room;
    _COLOR     ColorText;
    _COLOR     ColorBack;
    _COLOR     ColorFrame;
    _FRAMETYPE Frame;
    _FONT      Font;
    _ATTRIB    Attrib;

    _BOOL      MakePosition(_ROOM *pr, _KOORD x, _KOORD y, _FONT font, _CHAR *txt, _FRAMETYPE frametype, _ATTRIB attrib);
    void       DoView(void);

  public : 
    void       Init(void);
    void       Free(void);
    _BOOL      Call(_KOORD x, _KOORD y, void *text, _UWORD chrsize);
    _BOOL      Call(_KOORD x, _KOORD y, _ANYPARA *pany, _VARLABEL *pvl);
    _BOOL      DoSize(_PICTURE *ppic);
    void       Ready(_MY_PICTURE *mp);
    void       Run(_MY_PICTURE *mp, _EVENT *pe);
    _FRAMETYPE GetFrame(void);
};
#endif

#pragma pack(push, 1)
struct __LSEMENU;

typedef struct { _PTRLINE    PtrLine;     // --> to userdefined linedraw function
                 _PTRSTART   PtrStart;    // --> to userdefined init-interface
                 _PTRREADY   PtrReady;    // --> to userdefined ready-interface
                 _PTRRUN     PtrRun;      // --> to userdefined run-interface
                 _PTREND     PtrEnd;      // --> to userdefined end-interface
                 void*       PtrData;     // --> to userdefined data
                 void*       PtrSubList;  // --> to sublist-data
                 __LSEMENU*  Ptr_LSEMENU; // --> to lsemenu
                 bool        Level;       // use vertical level
                 
               }
_MENUPOINTER;

typedef struct { _FONT        font;         // characterset
                 _ATTRIB      attrib;       // attribute
                 _DOT         xy;           // left-top position
                 _UWORD       line_height;  // line height in pixel
                 _UWORD       width;
                 _UWORD       height;
                 _COLOR       text_color;   // line color
                 _COLOR       bar_color;    // bar color
                 _COLOR       frame_color;  // frame color
                 _VARLABEL    server;
                 _FRAMETYPE   frametype;
                 _UWORD       basement;     // height of basement (freespace)
                 _COLOR       inactive_text_color;  // inactive text color
                 _BOOL        draw_shadow;
}
_MENUINFO;

typedef struct { _ROOM       out_room;     // space for whole popupmenu
                 _ROOM       scroll_room;  // space of scrollarea
                 _LEVEL      level;        // level indicator
                 _UWORD      char_width;   // character width
                 _UWORD      char_height;  // character height
               }
_MENUCALCUL;

typedef struct { void          *PScreenObject; // muss wie bei _MY_PICTURE an erster stelle sein
                 _MENUPOINTER  pointer;
                 _POSITION     position;
                 _MENUCALCUL   calcul;
                 _MENUINFO     info;
                 _MENCURSOR    mencursor;
               }
_MENU;

typedef struct __LSEMENU
               { _MENUINFO     info;
                 _MEM_LINEINFO mem_lineinfo;
                 _TextPuffer   TextPuffer;
                 _UDWORD       lsemenuindex;
               }
_LSEMENU;

typedef struct { _UWORD        no;
                 _LSEMENU      *ptr;
               }
_MEM_LSEMENU;
#pragma pack(pop)

class _MenuList
{
  private :
    _MEM_LSEMENU MemLseMenu;
    _UDWORD      ComboBoxMenueIndex;
    _UDWORD      DictionaryMenueIndex;

    _BOOL        Read(_File*);

  public :
    _AsciiPuffer LseLabel;

    void         Init(void);
    void         Free(void);
    _BOOL        Load(_ASCII*);
    _UWORD       GetNo(void);
    _BOOL        OutIO(_IO*, _DWORD);
    _CHAR       *GetIoText(_UWORD, _DWORD, _VARLABEL*);

    _LSEMENU    *Get_LSEMENU(_UWORD);
    _CHAR       *GetLineText(_UDWORD *pstate_scheme_result, _LSEMENU*, _UWORD);
    _UWORD       GetRealNo(_LSEMENU*, _UWORD);
    _LINEINFO   *Get_LINEINFO(_LSEMENU*, _UWORD);
    _UWORD       CalculateLines(_LSEMENU*);
    _UWORD       AktLineNo(_DWORD, _LSEMENU*);
    _BOOL        Get(_MENUINFO*, _UWORD);
    _UDWORD      Add(_LSEMENU* pmen, _ASCII* name);
    _BOOL        MakeEnumerationMenu(_LSEMENU*, _UDWORD);
    _UDWORD      MakeComboBoxMenu(_COMBOBOX *pcombo);
    _BOOL        DesignEnumerationMenu(_UDWORD, _IO*, _COMBOINFO*);
    _BOOL        DesignMenu(_UDWORD menuno, _DWORD linewidth, _FONT font, _ATTRIB attrib, _COMBOINFO *pci);
    
    _BOOL        DesignDictionaryMenu(_UDWORD menuno, _IO *pio, _UDWORD linewidth);
    _UDWORD      MakeDictionaryMenu(_CHAR **pptxt, _UDWORD no);
    _BOOL        IsDictionaryMenue(_LSEMENU *p);
    _BOOL        IsDictionaryMenue(_WHOAMI *p);
};

#ifdef OK_PICMEMO
#pragma pack(push, 1)
typedef struct { _UWORD        no;
               }
_LASTINPUT;

typedef struct { _WHOAMI       whoami;
                 _LASTINPUT    lastinput; 
               }
_GLOBMEMO;

typedef struct { _UWORD        no;
                 _GLOBMEMO     *ptr;
               }
_MEM_GLOBMEMO;
#pragma pack(pop)

class _MemoPool
{
  private :
    _MEM_GLOBMEMO Memo;

    _BOOL  Seek(_GLOBMEMO**, _WHOAMI*);
    _BOOL  Add(_WHOAMI*); 

  public :
    void   Init(void);
    void   Free(void);

    _BOOL  GetLastInput(_WHOAMI*, _UWORD*);
    _BOOL  SetLastInput(_WHOAMI*, _UWORD);
};
#endif

//#define O_RDONLY          0
//#define O_BINARY          1
//#define S_IREAD           2
//#define O_RDWR            3
//#define O_BINARY          4
//#define O_TRUNC           5
//#define O_CREAT           6
//#define S_IREAD           7
//#define S_IWRITE          8
//#define SEEK_SET          0
//#define SEEK_CUR          1
//#define SEEK_END          2

class _Handle
{
  private :
   #ifndef __TURBOC__
    _FILE   *File;     // C interface
   #endif
    _HANDLE     Handle;

  protected :
    _BOOL   ChkHandle(void);
    void    Init(void);
    void    Free(void);
    _BOOL   GetCIf(void);

  public :
    _BOOL   Read(void*, _UDWORD);                     // read data from file
    _BOOL   Write(const void*, _UDWORD);                     // write data to file
    _BOOL   Open(_FILE_STATE, const _ASCII*);                         // open file
    void    Close(void);                                       // close file
    _DWORD  Tell(void);                    // get actual filepointerposition
    _BOOL   Seek(_DWORD, _UWORD);          // set actual filepointerposition
    _DWORD  Length(void);                                  // get filelength
    _BOOL   Remove(_ASCII*);
};

class _File : public _Handle
{
  private :
    _FILE_ERROR Error;
    _FILE_STATE State;
    _UWORD      MpcVer;                                // version ipc format
    _UWORD      LseVer;                                       // mae version
    _CRC32      LseCrc32;
    _UBYTE      *RamData;
    _UDWORD     RamSize;   // reservierter ramfile-speicher
    _UDWORD     RamWrite;  // tatsächlich benützter ramfile-speicher
    _UDWORD     Offset;    // ram-filepointer

    _BOOL  CheckHeader(_UBYTE*);

   #if COMPILER_VERSION >= 55
    _BOOL ReadMomentaryStationary(_BOOL *act_momentary, _DWORD *set_val, _DWORD *reset_val);
   #endif 

    _BOOL  Read_ACCESS(_ACCESS*);                         // load accesscode
    _BOOL  Read_SINGLESOFT(_SINGLESOFT*);             // load single softkey
    _BOOL  Read_RECTANGLE(_RECTANGLE*);           // load datatype RECTANGLE
    _BOOL  Read_CIRCLE(_CIRCLE*);                    // load datatype CIRCLE
    _BOOL  Read_POLYGON(_POLYGON*);                 // load datatype POLYGON
    _BOOL  Read_PICTEXT(_PICTEXT*);                 // load datatype PICTEXT
    _BOOL  Read_OBJECT(_OBJECT*);                    // load datatype OBJECT
    _BOOL  Read_BITMAP(_BITMAP*);                    // load datatype BITMAP
    _BOOL  Read_COMBOINFO(_COMBOINFO*); 
    _BOOL  Read_INPUT(_INPUT*);                       // load datatype INPUT
    _BOOL  Read_DIRECTION(_DIRECTION*);          // load datatype _DIRECTION
    _BOOL  Read_PICFORMAT(_PICFORMAT*);           // load datatype PICFORMAT
    _BOOL  Read_VIRTNAME(_VIRTNAME*);             // load datatype _VIRTNAME
    _BOOL  Read_IO(_IO*);                                // load datatype IO
    _BOOL  Read_MOTION(_MOTION*);                    // load datatype MOTION
    _BOOL  Read_CAROUSEL(_MOTION*);                // load datatype CAROUSEL
    _BOOL  Read_KARU(_KARU*);                         // load datatype _KARU 
    _BOOL  Read_MEM_KARU(_MEM_KARU*);             // load datatype _MEM_KARU 
    _BOOL  Read_BUTTON(_BUTTON*);              // load datatype TOUCH_BUTTON
    _BOOL  Read_ZOOM(_ZOOM*);                          // load datatype ZOOM
    _BOOL  Read_HILOLIM(_HILOLIM*);                 // load datatype HILOLIM
    _BOOL  Read_VARSINGLE(_VARSINGLE*);           // load datatype VARSINGLE
    _BOOL  Read_ROOM(_ROOM*);                          // load datatype ROOM
    _BOOL  Read_IOINFO(_IOINFO*, _IO_TYPE*);         // load datatype IOINFO
    _BOOL  Read_UNIT(_UNIT*);                          // load datatype UNIT
    _BOOL  Read_ATTRIB(_ATTRIB*);                    // load datatype ATTRIB
    _BOOL  Read_COL_RECT(_COL_RECT*);            // load coloured rectangles
    _BOOL  Read_COL_POLY(_COL_POLY*);              // load coloured polygons
    _BOOL  Read_COL_CIRC(_COL_CIRC*);               // load coloured circles
    _BOOL  Read_COL_PICTXT(_COL_PICTXT*);      // load coloured picturetexts
    _BOOL  Read_KEY(_KEY*);                                      // load key
    _BOOL  Read_GFUNCT(_GFUNCT*);       // load reference to glob. functions
    _BOOL  Read_FRAMETYPE(_FRAMETYPE*);
    _BOOL  Write_VARSINGLE(_VARSINGLE*);
    _BOOL  Read_THEURGE(_THEURGE*);
    _BOOL  Read_FAST_ASCII_BUFFER(_FAST_ASCII_BUFFER*);
    _BOOL  Read_BUTTONIMAGES(_BUTTONIMAGES*);
    _BOOL  Read_DYNCOLOR(_DYNCOLOR*);
    _BOOL  Read_MEM_DYNCOLOR(_MEM_DYNCOLOR*);
    _BOOL  Read_LINKVL(_LINKVL*);
    _BOOL  Read_XMEM_ANYPARA(_XMEM_ANYPARA*); 
   #ifdef UC_LINKVL
    _BOOL  Read_MEM_LINKVL(_MEM_LINKVL*);
   #endif

  #ifdef OK_RUNTIME_MODIFY
    _BOOL  WriteHeader(_UBYTE*);
    _BOOL  Write_ATTRIB(_ATTRIB*);
    _BOOL  Write_COLOR(_COLOR*);
    _BOOL  Write_CHKBIT(_CHKBIT*);
    _BOOL  Write_GFUNCT(_GFUNCT*);
    _BOOL  Write_SINGLESOFT(_SINGLESOFT*);
    _BOOL  Write_SOFTFLOOR(_SOFTFLOOR*, _UWORD);
    _BOOL  Write_RECTANGLE(_RECTANGLE*);
    _BOOL  Write_CIRCLE(_CIRCLE*);
    _BOOL  Write_POLYGON(_POLYGON*);
    _BOOL  Write_PICTEXT(_PICTEXT*);
    _BOOL  Write_ZOOM(_ZOOM*);
    _BOOL  Write_OBJECT(_OBJECT*);
    _BOOL  Write_BITMAP(_BITMAP*);
    _BOOL  Write_ROOM(_ROOM*);
    _BOOL  Write_INPUT(_INPUT*);
    _BOOL  Write_IOINFO(_IOINFO*, _IO_TYPE);
    _BOOL  Write_PICFORMAT(_PICFORMAT*);
    _BOOL  Write_IO(_IO*);
    _BOOL  Write_MOTION(_MOTION*);
    _BOOL  Write_PICTUREMASK(_PICTUREMASK*);
    _BOOL  Write_BMP(_BMP*);
    _BOOL  Write_COL_RECT(_COL_RECT*);
    _BOOL  Write_COL_POLY(_COL_POLY*);
    _BOOL  Write_COL_CIRC(_COL_CIRC*);
    _BOOL  Write_COL_PICTXT(_COL_PICTXT*);
    _BOOL  Write_KEY(_KEY*);
    _BOOL  Write_TOUCH_BUTTON(_BUTTON*);
    _BOOL  Write_VARSINGLE(_VARSINGLE*);
    _BOOL  Write_VARLABEL(_VARLABEL*);
   #ifdef UC_IPC
    _BOOL  Write_DPNE(_DPNE*);
   #endif
    _BOOL  Write_PICTURE(_PICTURE*);
   #endif
    _BOOL  Read_CRLF_ramfile(_ASCII*, _UWORD);
   #ifdef UC_UNICODE
    _BOOL  Read_CRLF_unicode(_UNI*, _UWORD);
    _BOOL  Read_CRLF_ramfile_unicode(_UNI*, _UWORD);
   #endif
    void   PrepaireRoom(void);
    void   GetRoom(_ROOM*);
    _BOOL  ReadHeaderSub(_UBYTE *head);

  public :
    void   Init(void);
    void   Free(void);
    _BOOL  SizeRamFile(_UDWORD);
    _UDWORD GetHeaderSize(void);
    _UWORD GetCompilerVersion(void);          // get target compiler version
    _UWORD GetLseVersion(void);                           // get version mae
    _UWORD GetMpcVersion(void);               // get source compiler version
    _CRC32 GetLseCrc32(void);
    _FILE_ERROR GetErrorState(void);                       // get error-type
    _BOOL  GetError(void);                                      // get error
    void   SetError(_FILE_ERROR);                           // set errorcode
//    _BOOL  Open(_FILE_STATE, _UNI*, _UDWORD expected_size = 0);      // open
    _BOOL  Open(_FILE_STATE, const _ASCII*, _UDWORD expected_size = 0, _BOOL openramfile = _FALSE);    // open
//    _BOOL  Open(_FILE_STATE, _UNI*, _UNI*, _UNI*, _UNI*);       // file open
//    _BOOL  Open(_FILE_STATE, _ASCII*, _ASCII*, _ASCII*, _ASCII*); // file open
    _BOOL  Close(_BOOL=_FALSE);                                // close file
    _DWORD Tell(void);                     // get actual filepointerposition
    _BOOL  Seek(_DWORD, _UWORD);           // set actual filepointerposition
    _DWORD Length(void);                                   // get filelength
    _BOOL  Read(void*, _UDWORD);                      // read data from file
    _BOOL  Write(const void*, _UDWORD);                      // write data to file
    _BOOL  Remove(_ASCII*);
    void   ErrorMessage(void);                             // evaluate error
    _BOOL  FileMemory(void**, _UDWORD);           // alloc or realloc memory
    _BOOL  ReadHeader(void);                              // read fileheader
    _BOOL  ReadHeaderName(_ASCII *pname);
    _BOOL  Write_RINGFILEHEADER(_UDWORD, const _ASCII*, _UDWORD, _UDWORD);
    _BOOL  Create_RINGFILE(_ASCII*, _UDWORD, const _ASCII*, _UDWORD, _UDWORD);
    _BOOL  Encode(_UDWORD);
    _BOOL  Decode(_UDWORD);
    void*  PutOutMemory(void);
    _BOOL  Read_RINGBUFFER(_RINGBUFFER*);
    _BOOL  Write_RINGBUFFER(_RINGBUFFER*);
    _BOOL  Write_HEADER(const _ASCII*, _UDWORD);
    _BOOL  Read_LSEMENU(_LSEMENU*);
    _BOOL  Read_HEADER(_UDWORD*, const _ASCII*, _BOOL check_lsecrc);
    _BOOL  Write_ENUM(_UDWORD);
    _UWORD Read_ENUM(void);                            // load datatype enum
    _BOOL  Read_BOOL(void);
    _BOOL  Read_COLOR(_COLOR*);                       // load datatype COLOR
    _BOOL  Read_DOT(_DOT*);
    _BOOL  Read_ALARM(_ALARM*);                       // load datatype ALARM
    _BOOL  Read_DATE(_DATE*);
    _BOOL  Write_DATE(_DATE*);
    _BOOL  Read_TIME(_TIME*);
    _BOOL  Write_TIME(_TIME*);
    _BOOL  Read_DATIM(_DATIM*);
    _BOOL  Write_DATIM(_DATIM*);
    _BOOL  Read_ASCII_BUFFER(_ASCII_BUFFER*);  // load datatype ASCII_BUFFER
    _BOOL  Read_MEM_UNIT(_MEM_UNIT*);              // load datatype UNITMEMO
    _BOOL  Read_SOFTFLOOR(_SOFTFLOOR*, _UWORD);         // load softkeyfloor
    _BOOL  Read_STACK_LINE(_STACK_LINE*);        // load datatype STACK_LINE
    _BOOL  Read_PRJ_ENTRY(_PRJ_ENTRY*);           // load datatype PRJ_ENTRY
    _BOOL  Read_VARLABEL(_VARLABEL*);              // load datatype VARLABEL
    _BOOL  Write_ANYTHING(_ANYTHING*);
    _BOOL  Read_ANYTHING(_ANYTHING*);              // load datatype ANYTHING
    _BOOL  Read_ANYPARA(_ANYPARA*);                 // load datatype ANYTEXT
    _BOOL  Read_CHKBIT(_CHKBIT*);                    // load datatype CHKBIT
    _BOOL  Read_XXCHKBIT(_XXCHKBIT*);               // load datatype XCHKBIT
    _BOOL  Read_LSEVARINFO(_LSEVARINFO*);        // load datatype LSEVARINFO
    _BOOL  Read_VARSYMBOL(_VARSYMBOL*);           // load datatype VARSYMBOL
    _BOOL  Read_PICTURE(_PICTURE*);                 // load datatype PICTURE
    _BOOL  Read_PICTUREMASK(_PICTUREMASK*);     // load datatype PICTUREMASK
    _BOOL  Read_ZORDER(_ZORDER*);
    _BOOL  Read_MEM_ZORDER(_MEM_ZORDER*);
    _BOOL  Read_MEM_ATTRIB(_MEM_ATTRIB*);
    _BOOL  Read_MEM_COLOR(_MEM_COLOR*);
    _BOOL  Read_MEM_ANYTHING(_MEM_ANYTHING*);
    _BOOL  Read_MEM_ANYPARA(_MEM_ANYPARA*);
    _BOOL  Read_LSEFUNCT(_LSEFUNCT*);
    _BOOL  Read_MEM_LSEFUNCT(_MEM_LSEFUNCT*);
    _BOOL  Read_MEM_LINKSCHEME(_MEM_LINKSCHEME*);
    _BOOL  Read_MEM_FONT(_MEM_FONT*);
    _BOOL  Read_MEM_MAP(_MEM_MAP*);                // load datatype _MEM_MAP
    _BOOL  Read_MEM_BOOL(_MEM_BOOL*);
    _BOOL  Read_MEM_NO(_MEM_NO*);
    _BOOL  Read_BMP(_BMP*);                             // load datatype BMP
    _BOOL  Write_BMP(_BMP*);                           // write datatype BMP
    _BOOL  Read_CRLF(_ASCII*, _UWORD);                     // load CRLF-text
    _BOOL  ReadCrLf(void*, _UWORD, _UWORD);
    _BOOL  Read_LINEINFO(_LINEINFO*);              // load datatype LINEINFO
    _BOOL  Read_MEM_VARLABEL(_MEM_VARLABEL*);  // load datatype MEM_VARLABEL
    _BOOL  Read_MEM_LINEINFO(_MEM_LINEINFO*);  // load datatype MEM_LINEINFO
    _BOOL  Read_MENUINFO(_MENUINFO*);              // load datatype MENUINFO
    _BOOL  Read_COMPCODE(_COMPCODE*);             // load datatype _COMPCODE
    _BOOL  Read_MYSCHEME(_MYSCHEME*);             // load dytatype _MYSCHEME
    _BOOL  Read_LINKSCHEME(_LINKSCHEME*);       // load dytatype _LINKSCHEME
    _BOOL  Read_SINGLESCHEME(_SINGLESCHEME*, _BOOL);
    _BOOL  Read_MEM_SINGLESCHEME(_MEM_SINGLESCHEME*, _BOOL);
    _BOOL  Read_SCHEME(_SCHEME*, _BOOL, _BOOL);
    _BOOL  Read_MEM_SCHEME(_MEM_SCHEME*, _BOOL, _BOOL);
    _BOOL  Read_TEXT_BUFFER(_TEXT_BUFFER*, _BOOL*);
    _BOOL  Read_VARBLOCK(_VARBLOCK*);
    _BOOL  Read_EDITCOLOR(_EDITCOLOR*);
    _BOOL  Read_BUBBLEINFO(_BUBBLEINFO*);
    _BOOL  Read_CURSORCOLOR(_CURSORCOLOR*);
    _BOOL  Read_SCREENSAVER(_SCREENSAVER*);
    _BOOL  Read_SINGLEALARM(_SINGLEALARM*, _UDWORD);
    _BOOL  Read_NAME_INDEX(_NAME_INDEX*);
    _BOOL  Memory_SINGLEALARM(_UBYTE *data, _SINGLEALARM *p, _UDWORD version);
    _BOOL  Write_SINGLEALARM(_SINGLEALARM*, _UDWORD);
    _BOOL  Write_MEM_VARLABEL(_MEM_VARLABEL*);
    _BOOL  Write_VARLABEL(_VARLABEL*);

   #ifdef UC_THEME
    _BOOL  Read_THEME(_THEME*);
    _BOOL  Read_MEM_THEME(_MEM_THEME*);
   #endif
   #ifdef UC_IPC
    _BOOL  Read_DPNE(_DPNE*);                         // load datatype _DPNE
   #endif

    _BOOL  IsRamFile(void);
    _UDWORD Make_CRC(_UDWORD);
    _BOOL  Read_SINGLETXTSCHEME(_SINGLETXTSCHEME*);
    _BOOL  Read_TXTSCHEME(_TXTSCHEME*);
    _BOOL  Read_MEM_TXTSCHEME(_MEM_TXTSCHEME*);
    _BOOL  Read_MEM_KEY(_MEM_KEY*);
};

#ifdef USER_BILLION
 #define TERMALARMPUFFER   200
#endif
#ifdef USER_KM
 #define TERMALARMPUFFER   150
#endif
#ifndef TERMALARMPUFFER
 #define TERMALARMPUFFER   50
#endif

//#define BIT_REMANENT       1   // daten werden auf hd gesichert und beim starten wieder geladen
#define BIT_ONOFF          2   // bei alarm wird zeit gekommen und zeit gegangen eingetragen, ansonsten 2ter eintrag
#define BIT_USECOUNTER     4   // tritt ein alarm wiederholt auf wird nur counter beim ersteintrag erhöht, ansonsten neuer eintrag
#define BIT_DELQUIT        8   // aktiver alarm kann nicht gelöscht werden, wird quittiert
#define BIT_LONGTEXT      16   // langtext anstelle von kurztext wird angezeigt
#define BIT_RESETACTIVE   32   // alle aktiven alarme werden beim einschalten der anlage gelöscht
#define BIT_POWER_ON      64   // es wird in der History beim Hochlauf immer eine Zeile "POWER ON" erzeugt. (sa39254)

#pragma pack(push, 1)
typedef struct { _UWORD       filter;
                 _RINGBUFFER  ringbuffer;
                 _UWORD       xchange;
                 _UWORD       sortalgorithm;
                 _SINGLEALARM *ptr;
                 _UDWORD      noactive;
                 void         *pthis;
                 _UDWORD      seqno;
                 _BOOL        locked;
                 _UDWORD      para_filter;  // SA33569
                 _File        File; // muss unbedingt letzte komponente sein !!
               }
_MEM_SINGLEALARM;
#pragma pack(pop)

class _TermAlarm
{
  private :
    _ASCII           HistoryDPNE[_MAX_PATHLENGTH];
    _MEM_SINGLEALARM History;
    _MEM_SINGLEALARM Temporary;
    _SINGLEALARM     Puffer[TERMALARMPUFFER];
    _BOOL            Queue;
    _IVALUE          MaxDateTime;  // for GetLostAlarms
   #ifdef UC_NEW_ALARMSYSTEM
    _UDWORD          AlaTimer;
   #endif

   #ifdef UC_MULTICPU
    _SINGLEALARM* SeekPuffer(_DWORD, _UWORD);
   #else
    _SINGLEALARM* SeekPuffer(_DWORD);
   #endif

    _BOOL  CheckParameterFilter(_MEM_SINGLEALARM *p, _SINGLEALARM *d, _SINGLEALARM *s); // SA33569
    void   CacheReset(void);
    _BOOL  CacheAdd(_SINGLEALARM *psa, _UDWORD offset);


  public :

   void    CacheRun(void);
   _BOOL   CacheFinalize(void);
	 void    Init(_BOOL);
	 void    Free(void);
	 _BOOL   Save(void);
	 _BOOL   Load(void);
	 _BOOL   Store_ALACELL(_ALACELL*);
	 void    Run(void);
	 void    ReConfig(_BOOL);
	 void    LookForReConfig(_BOOL);
	 _BOOL   Insert_MEM_SINGLEALARM(_MEM_SINGLEALARM*, _SINGLEALARM*);
	 _MEM_SINGLEALARM *Get_MEM_SINGLEALARM(_BOOL);
	 _BOOL   LockHistory(_BOOL);
	#ifdef UC_NEW_ALARMSYSTEM
	 void    ResetTimer(void);
	#else
	 _BOOL   CheckLostAlarm(_ALACELL*);
	#endif
	 _BOOL   Insert_SINGLEALARM(_MEM_SINGLEALARM*, _SINGLEALARM*);
};


#define NOCARE_STATESCHEME  0x0001
#define NOCARE_ACCESSLEVEL  0x0002
#define NOCARE_CHKBIT       0x0004
#define NOCARE_LIMITS       0x0008

#define NOCARE_DEFAULT      0x0000
#define NOCARE_ALL         (NOCARE_LIMITS|NOCARE_CHKBIT|NOCARE_ACCESSLEVEL|NOCARE_STATESCHEME)

#define ERROR_CAUSED_BY_NO_ERROR     0  // kein error
#define ERROR_CAUSED_BY_STATESCHEME  1  // ungültiges statescheme
#define ERROR_CAUSED_BY_ACCESSLEVEL  2  // ungültiger access
#define ERROR_CAUSED_BY_CHKBIT       3  // ungültiges chkbit
#define ERROR_CAUSED_BY_LIMITS       4  // grenzwertverletzung
#define ERROR_CAUSED_BY_NO_IDEA      5  // keine ahnung
#define ERROR_CAUSED_INVALID_SERVER  6  // ungültiger server (server nicht vorhanden, lasalid ungültig)

#ifdef UC_ALARMID_FAST  
  typedef struct
  {
    _UDWORD index;
    _UDWORD lasalid; 
  }
  _LINDEX;

  typedef struct
  {
    _UDWORD no;
    _LINDEX *ptr;
    _DWORD  done;  // 0 not created, 1 well done, -1 error
  } 
  _MEM_LINDEX;
#endif

class _VarList
{
  private :

    _VARBLOCK      Data;
    _MEM_UNIT      MemUnit;
    _AsciiPuffer   LabelUnit;
    _AsciiPuffer   LabelGroup;
    _BOOL          VarIdChanged;
    _MEM_MAP       MapCRC;
    _NAME_INDEX    HostName;
    _DWORD         FileOffsetHostName;
    _ASCII         VarListDPNE[_MAX_PATHLENGTH];
    _UBYTE         Stations[256];
    _UDWORD        VarDummyData[5];
    _UDWORD        MemberVariablesCount; // anzahl der membervariablen in varlist

   #ifdef UC_ALARMID_FAST  
    _MEM_LINDEX    MlStation[256];
   #endif

    
    void   VarDummyInit(void);
    _BOOL  VarDummyGet(_RESULT *res, _VARLABEL *vl);
    _BOOL  VarDummySet(_VARLABEL *vl, _RESULT *res);
//    _MEM_MAP       MapLasalID;
//    _BOOL  Read_VTX(_File*, _UWORD);                      // load vartextlist
    _BOOL  Read(_File*, _UWORD, _UWORD*);                     // load varlist
    _DWORD GetIndexValue(_VARLABEL*);                 // get defined constant
    _BOOL  GetBit(_RESULT*, _VARINFO*, _VARLABEL*);           // get bit info
    _BOOL  GetElement(_RESULT*, _VARINFO*, _VARLABEL*);   // get complex info
    _BOOL  GetString(_RESULT*, _VARINFO*, _VARLABEL*);     // get string info
    _BOOL  GetTime(_RESULT*, _VARINFO*, _VARLABEL*);         // get time info
    _BOOL  GetDate(_RESULT*, _VARINFO*, _VARLABEL*);         // get date info
    _BOOL  GetBinary(_RESULT*, _VARINFO*, _VARLABEL*);        // get bin info
    _BOOL  GetConstant(_RESULT*, _VARINFO*);             // get constant info
    _BOOL  GetMerker(_RESULT*, _UDWORD);                   // get merker info
    _BOOL  GetValueIntern(_RESULT*, _VARIABLE*, _VARLABEL*, _BOOL, _BOOL, _BOOL cheat=_FALSE);// get data
   #ifdef UC_GLOBAL_VAR
    _BOOL  GetGlobalValue(_RESULT*, _VARIABLE*, _VARLABEL*, _BOOL);
   #endif
//    _UDWORD FindAround(_UDWORD, _CRC32, _ASCII*);
    _UDWORD MakeStationCrc(void);
    _UBYTE CalculDigits(_VARINFO*, _UWORD);
    _BOOL  SetMerker(_RESULT*, _UDWORD);
    _BOOL  SetDate(_RESULT*, _VARINFO*);
    _BOOL  SetTime(_RESULT*, _VARINFO*);
    _BOOL  SetString(_RESULT*, _VARINFO*);
    _BOOL  SetRadio(_RESULT*, _VARINFO*, void*);
   #ifdef OLDSTYLE_CAN
    _BOOL  SetValueSubBit(_RESULT*, _VARIABLE*);
   #endif
    _BOOL  SetBit(_RESULT*, _VARIABLE*);
    _BOOL  SetBinary(_RESULT*, _VARINFO*);
    _BOOL  SetValue(_RESULT*, _VARIABLE*, _VARLABEL*, _BOOL, _UWORD);
    _ASCII*GetLabelInternVARBLOCK(_ASCII*, _UBYTE*, _UDWORD, _VARBLOCK*);
    _UDWORD ScanLabel(_UDWORD, _ASCII*);
    _UWORD GetComponentLength(_ASCII*);
    _BOOL  CheckHostnameSub(_UDWORD);
    _BOOL  ReloadHostName(void);
    _BOOL  PrepaireStateschemeSub(_UDWORD);
    void   RepairStation(void);
    void   DoPermanentServer(_UDWORD);
   #ifdef UC_MEMBERVARIABLE
    _UDWORD GetMemberVariableAddress(_UDWORD varno);
    _DWORD  GetMemberOffsetFromParent(_UDWORD varno);
    _UDWORD FindMemberVariables(void);
    _BOOL   DoMemberVariables(void);
   #endif //UC_MEMBERVARIABLE 
//laufzeitinstanz    _BOOL  AddAllServer(_UWORD, _UDWORD, _UDWORD);
//laufzeitinstanz    _BOOL  AddNewVariable(_ASCII*, _VARINFO*, _UWORD);
//laufzeitinstanz    _BOOL  CreateObjectDataForLse(_UWORD, _ASCII*, _ASCII*);
//laufzeitinstanz    _UDWORD LookIfNameAlreadyExists(_UWORD, _UDWORD, _ASCII*);
//laufzeitinstanz    _BOOL  FindClassId(_UWORD*, _ASCII*);
//laufzeitinstanz    _BOOL  DeleteVariable(_UDWORD, _UDWORD);

   #ifdef UC_ALARMID_FAST
    void         init_MEM_LINDEX(_MEM_LINDEX *p);
    void         free_MEM_LINDEX(_MEM_LINDEX *p);
    _DWORD       Make_MEM_LINDEX(_MEM_LINDEX *p, _LSEVARINFO *pvi, _UDWORD pvi_no, _UBYTE station);
    _MEM_LINDEX *Get_MEM_LINDEX(_LSEVARINFO *pvi, _UDWORD pvi_no, _UBYTE station);
    _UDWORD      Seek_MEM_LINDEX(_MEM_LINDEX *ps, _UDWORD lasalid);
   #endif

  public :
   #ifdef UC_LINKVL_FAST 
    _LinkVlBuffer   LinkVlBuffer;
   #endif
    void    Init(void);
    void    Free(void);
   #ifdef OLDSTYLE_CAN
    void    CalculatePgAddresses(void);
    _BOOL   ChangeBaseAddress(_UDWORD, _UDWORD);
   #endif
    _BOOL   GetVarInfo(_LSEVARINFO**, _UDWORD);              // --> to varinfo
    _UDWORD MakeVarCrc32(void);
//    _BOOL   Load_VTX(_ASCII*, _UWORD);                   // load vartextlist
    _BOOL   Load(_ASCII*, _UWORD, _UWORD*);
    _BOOL   GetPointerToData(void**, _VARINFO*, _VARLABEL*);// get --> to data
    _BOOL   ShiftVarlabel(_VARLABEL*);                        // shift index
    _IVALUE GetConstant(_VARLABEL*);     // get constant value from VARLABEL
    _BOOL   ConvertVarlabel(_VARLABEL*);     // convert varlabel to constant
    _BOOL   GetSingleVariable(_VARIABLE*, _UDWORD);   // get single variable
    _BOOL   GetVariable(_VARIABLE*, _VARLABEL*);        // get hole variable
    _BOOL   GetSystemData(_RESULT*, _VARIABLE*, _VARLABEL*, _BOOL, _BOOL, _BOOL cheat=_FALSE); // get value
    _UWORD  GetVariableSize(_VARTYPE, _UWORD);       // get size of variable
    _UDWORD GetVarNoByID(_UDWORD, _UWORD, _BOOL);    // get varno by lasalid
    _UDWORD FindIndexCRC(_CRC32, _DWORD, _DWORD);
//    _BOOL   MakeMapLasalID(void);
//    _UDWORD FindIndexLasalID(_UDWORD, _DWORD, _DWORD);

    _DWORD  VarDummyGet(_UDWORD varno);
    _BOOL   VarDummySet(_UDWORD varno, _DWORD value);
    _BOOL   IsStation(_UWORD);
    _BOOL   GetMinMax(_RESULT*, _VARIABLE*, _VARLABEL*, _BOOL, _BOOL, _BOOL cheat=_FALSE);// get min and max value
    void    GetDefaultMinMax(_RESULT*, _VARIABLE*);  // def. min and max value
    _UWORD  GetStateScheme(_UDWORD, _UWORD);
    _BOOL   SetSystemData(_RESULT*, _VARIABLE*, _VARLABEL*, _BOOL); // set value
    _BOOL   SetSystemDataSub(_RESULT*, _VARIABLE*, _VARLABEL*, _BOOL, _UWORD); // set value
    _BOOL   SetValueSub(_RESULT*, _VARIABLE*);
    _BOOL   GetValueByNo(_UDWORD, _IVALUE*, _BOOL);        // get singlevalue
    _BOOL   GetValue(_VARLABEL*, _IVALUE*, _BOOL, _BOOL cheat=_FALSE);        // get singlevalue
    _BOOL   SetValueByNo(_UDWORD, _DWORD);                 // set singlevalue

    _UWORD  GetCompleteNoDigits(_VARINFO*);      // gets no of digits for io
    _BOOL   GetLabel(_ASCII*, _UBYTE*, _UDWORD);      // get name of variable
    _BOOL   GetLabelVARBLOCK(_ASCII*, _UBYTE*, _UDWORD, _VARBLOCK*);
    _ASCII *GetLabelToken(_UDWORD);  // get labeltoken of variable
    _BOOL   GetEnumItems(_MEM_NO*, _UDWORD);
    _UDWORD GetEnumItemVarNo(_IVALUE value, _UDWORD varno);
    _UDWORD GetVarNo(_ASCII*, _UDWORD);  // get nr of variable, refered by label

    void    InitAdresses(void);                  // init all extern adresses
   #ifdef UC_DELETE_REFLIST
    void    InitAdressesOnStationNumber(_UWORD);
   #endif
   #ifdef UC_MULTICPU
    void    ChangeMultiID(_UWORD, _UWORD);
    void    GetIDSub(_UWORD, _UWORD, _UDWORD, _UWORD);
   #else
    void    GetIDSub(_UDWORD, _UWORD);
   #endif
    void    GetID(_BOOL);                               // get lasal id-list
    void    ReGetID(_BOOL);                       // try get id to new plc's
    _BOOL   InsertUnitInfo(_VARINFO*);
//    _BOOL   WriteID(_LASALID*, _UDWORD, _UBYTE);
//    _BOOL   InsertLocalAdress(_ASCII*, _UDWORD);      // insert local adress
    _UDWORD LookUpIfInRootIsSameType(_UDWORD, _UDWORD);
    _LSEVARINFO* GetRootInfo(_UDWORD);          // get --> to root info (object)
    _UDWORD      GetVarNo_RootParent(_UDWORD varno); // get varno of RootParent
    _LSEVARINFO* Get_LSEVARINFO_RootParent(_UDWORD varno); // get --> to root info
//    _UDWORD FindSameType(_UDWORD, _UDWORD);

//laufzeitinstanz		_UDWORD CreateObject(_UWORD, _ASCII*, _ASCII*);         // create object
//laufzeitinstanz    _BOOL   VisualizeObject(_UWORD, _ASCII*, _ASCII*);  // visualuize object
//laufzeitinstanz		_BOOL   ConnectObject(_UWORD, _ASCII*, _ASCII*);
//laufzeitinstanz		_BOOL   StartObject(_UWORD, _ASCII*, _UDWORD, _UWORD);
//laufzeitinstanz		_BOOL   DeleteObject(_UWORD, _ASCII*);

    _CHAR  *GetTextByNo(_UDWORD, _UWORD);
//    _CHAR  *GetText(_UWORD);
    _BOOL   GetTool(_UDWORD);
    _BOOL   GetFixDezpt(_VARINFO*);
    _BOOL   GetProt(_UDWORD);
    _BOOL   GetPde(_UDWORD);
    _BOOL   GetZero(_UWORD);
    _BOOL   GetPermanent(_UWORD);
    _BOOL   GetInvert(_UWORD);
    _UWORD  GetDigits(_VARINFO*);
    _BOOL   GetSign(_VARINFO*);
    _DWORD  GetDezPoint(_VARINFO*);
    _UDWORD GetNo(void);
    _BOOL   GetVarSymbol(_VARSYMBOL**, _UDWORD);
//    _TextPuffer  *GetTextPuffer(void);
    _AsciiPuffer *GetLabelGroup(void);
    _AsciiPuffer *GetLabelUnit(void);
    _VARBLOCK    *GetPointerToData(void);
    _CHAR  *GetUnitText(_UWORD);
    _UNIT  *GetUnitInfo(_UWORD);
    _UWORD  GetNoUnit(void);

    _UDWORD GetCrc32(void);
    _BOOL   SaveIdCrc(void);
    _BOOL   LookForIdCrc(void);
    _UWORD  GetUnitNo(_ASCII*);
    _BOOL   GetVarIdChanged(void);
    _UDWORD GetVarNoByCRC(_CRC32);

    _NAME_INDEX* GetHostName(void);
    _BOOL   CheckHostname(_BOOL, _BOOL, _BOOL);
    _ASCII *GetHostName(_ASCII*, _UWORD);
    _ASCII *GetHostNameByVarNo(_UDWORD);
    _ASCII *GetHostNameByCrc(_CRC32);
    _UDWORD GetVarNoByHostName(_ASCII*);
    _BOOL   GetOnlyAscii(_VARINFO*);
    void    PrepaireStatescheme(void);
    _BOOL   GetRootParent(_LSEVARINFO**, _ASCII*, _UBYTE);
    _UDWORD GetNodeObjectNumber(_UDWORD varno); // zugehörige objektnummer anhand servernummer suchen
    
   #ifdef UC_IPC
    _UDWORD MemSize(_MEMSIZE*);
    _BOOL   MakeReflistMap(_MEM_NO*);
   #endif
    _BOOL   GetXIndex(_DWORD *pxindexval, _UDWORD percent_x_varno);   
    _UDWORD GetUnitIndex(_UDWORD varno); // sa36634
    _ASCII *GetUnitLabel(_UDWORD unitindex); // sa36634
    _LSEVARINFO *Get(_VARLABEL *pvl);
};

class _ChkBit
{
  #define CHKBIT_BYTESIZE ((UC_CHKBIT_NO + 7) / 8)

  private :
    _UBYTE Bit[CHKBIT_BYTESIZE];

  public  :
    void  Init(void);
    void  Free(void);
    _BOOL Get(_UWORD);                                       // get chkbit
    _BOOL Set(_UWORD, _BOOL);                                // set chkbit
};

class _UnderCover
{
  private :
    _UDWORD Cnt, Pass, Code, Time;

  public:
    void   Init(void);
    _BOOL  Run(_UWORD);
};

#define EVENT_LOCAL_KEYBOARD  1
#define EVENT_LOCAL_TOUCH     2
#define EVENT_LOCAL_USER      4
#define EVENT_LOCAL_SHUTTLE   8
#define EVENT_LOCAL_LASALOS   16

class _Keyboard
{
  #ifdef UC_MULTITOUCH
   #if UC_MULTITOUCH < 2
     #define MAX_MULTIFINGER 2 // multitouch mit weniger als 2 finger ist fragwürdig !
   #else
     #define MAX_MULTIFINGER (UC_MULTITOUCH)
   #endif
  #else
   #define MAX_MULTIFINGER 1
  #endif
  
  private :
    _EVENT      Event;
    _UDWORD     mTouchDeviceLock;
    _UWORD      KillMask;
    _UDWORD     SecretTime;
    _UDWORD     MouseAnzahl;
    _UnderCover UnderCover;
    _FDOIT_PT   PtMethodSetEvent;
    _FDOIT_PT   PtMethodGetEvent;
    void        DoOsEvent(_OSEVENT*);
    void        LookUpDiagnostic(_EVENT*);
    void        MakeScancode(_EVENT *pe, _OSEVENT *posevent);
   #ifdef UC_LOGEVENT 
    _EVENT      LogBuf[UC_LOGEVENT];
    _UDWORD     LogCnt;
    _UDWORD     LogNoneCnt;
    _UDWORD     LogTimex;
    void        LogEvent(_EVENT *);
   #endif
   
    _EVENTMEMO  EventMemo[MAX_MULTIFINGER];
    _EVENTMEMO  EventMemoLrm;
    
    _UDWORD     TriggerEventValid;
    _EVENT      NextEvent;
   #ifdef UC_MULTITOUCH
   #ifdef UC_FLICK_ZOOM
    _MULTIEVENT MultiEvent[MAX_MULTIFINGER];
    _BOOL       FlickZoom(_UDWORD timestamp);
   #endif
   #endif
    _BOOL       HidPrePressValid;
    _EVENT      HidPrePressEvent;
    _ROOM       HidPrePressRoom;
   
    void        TriggerEventRun(_EVENT*);
    void        HidPrePressRun(void);
    
   #ifdef UC_RELEASE_PRESS_FILTER
    _UDWORD     RP_DelayTime;
    _OSEVENT    RP_Memo;
    _DOIT       RP_Filter(_DOIT doit, _OSEVENT *pose);
   #endif 
    
    _UBYTE      MakeMultiDisplayIndex(_UBYTE midx);
    
  public :

    _UDWORD  LasalOsCnt;
    _UDWORD  HideEventDueWindowClosed;

   _EVENTMEMO *GetEventMemo(_UDWORD);
    void     SetHidPrePress(void);
    _BOOL    TriggerEventStart(_EVENT*, _TIP);
    void     TriggerEventStop(_EVENT*);

    void     Init(void);
    void     Free(void);
    _BOOL    DoAbsolutKeyboard(void);              // read key from keybuffer
    void     DeInstall(void);
    void     Install(void);
    void     KillEvent(void);

    _BOOL    AddEvent(_EVENTTYPE, _UWORD);
    _BOOL    AddEventEx(_EVENT*, _BOOL keep_timestamp);
    void     GetEvent(_EVENT*);
    void     DelBuffer(void);
    _UDWORD  KillAllTouchEvents(void);
    void     StartStopEvent(_BOOL, _UWORD);
    _UDWORD  SetRP_DelayTime(_UDWORD ms_time);
    void     TouchDeviceLock(_UDWORD device, _BOOL state);
};

#pragma pack(push, 1)
typedef struct { _UWORD     no;
                 _UWORD     skl;
               }
_PICMEMO_SKL;
#pragma pack (pop)


#define NO_PICMEMO 20
class _PicMemo
{
  private :
    _PICMEMO_SKL Memo[NO_PICMEMO];
    _UWORD       Index;

  public :
    void   Init(void);
    void   Free(void);
    void   Add(_UWORD, _UWORD);
    _PICMEMO_SKL *Get(void);
    _UWORD GetNo(void);

};

#ifdef OK_VALIDATE

#pragma pack (push, 1);
typedef struct { _VARLABEL vl;
                 void      *ptr;
               }
_VALIDATA;

typedef struct { _UWORD    no;
                 _VALIDATA *ptr;
               }
_MEM_VALIDATA;
#pragma pack (pop)

class _Validate
{
  private :
    _MEM_VALIDATA MemData;

    _BOOL Seek(_UWORD*, _VARLABEL*);
    _BOOL AddIntern(_UWORD*, _VARLABEL*, _UDWORD);

  public :
    void  Init(void);
    void  Free(void);

    _BOOL MakePointerToData(void**, _VARLABEL*, _UDWORD);
    _BOOL GetPointerToData(void**, _VARLABEL*);
    _BOOL HasChanged(_VARLABEL*);
    void  ValidateAll(void);
    void  DischargeAll(void);
};
#endif

class _Project
{
  private :
    _PROJECT  Project;
    _UDWORD   LseProjectCrc;

    _BOOL  Read(_File*);                                   // load projectdata
    _BOOL  GetPictureAccessSub(_ASCII*, _UWORD*, _BOOL, _PRJ_ENTRY**, _BOOL sa51914);
    _BOOL  GetWindowAccessSub(_ASCII*, _UWORD*, _BOOL, _PRJ_ENTRY**);
    _BOOL  GetNameFromFile(_ASCII *dst, _ASCII *ne);
    _UDWORD GetLseProjectCrcSub(void); // get crc32 of whole data created by lse

  public :
    void  Init(void);
    void  Free(void);
    void  Start(void);                                    // interface: start
    _BOOL Ready(_BOOL);                                    // interface: init
    void  Run(_EVENT*);                                     // interface: run
    void  End(void);                                        // interface: end

    _ASCII  *GetNameOfEntry(_PRJ_ENTRY *pe);
    _EDITCOLOR *Get_EDITCOLOR(_EDITSTATE);
    _BOOL   GetWindowAccess(_ASCII*, _UWORD*, _BOOL, _PRJ_ENTRY**);
    _BOOL   GetPictureAccess(_ASCII*, _UWORD*, _BOOL, _PRJ_ENTRY**);
    _BOOL   GetPictureAccess2(_ASCII*dpne, _UWORD *pno, _BOOL system, _PRJ_ENTRY **ppe, _BOOL sa51941);
    _BOOL   Access_PRJ_ENTRY(_PRJ_ENTRY*, _BOOL);
    _PRJ_ENTRY *GetEntry(_UWORD);                       // get project entry
    _UWORD  GetNo(void);                          // get no of project entrys
    _UWORD  GetNoStatic(void);             // get no of static project entrys
   #ifdef XUSER_KM
    _BOOL   JustLoad(_ASCII*);
   #endif
    _BOOL   Load(_ASCII*);                              // load whole project
    void    ReloadText(void);                          // reload all textlist
    _UWORD  GetLanguageNo(void);              // get no of included languages
    _ASCII_BUFFER *GetLanguages(void);              // get the language names
    _LANGUAGE_INFO *Get_LANGUAGE_INFO(void);
    _UWORD  GetNextPictureNo(_UWORD);                   // get next pictureno
    _BOOL   MakeConfigDPNE(_ASCII*, const _ASCII*, _BOOL);
    _PRJ_ENTRY *GetEntryByNo(_UWORD, _ENTRYTYPE);
    _PRJ_ENTRY *GetEntryByLabel(_ASCII *label, _ENTRYTYPE ftype);
    _UDWORD GetCrc32(void);
    _ASCII* GetPrjPath(void);
    _BOOL   GetDPNE(_ASCII*, _ENTRYTYPE);
    void    InitUsedIntern(_UWORD);
    _BOOL   SetTime(_UWORD, _ENTRYTYPE, _UDWORD);
    _UDWORD GetLseProjectCrc(void); // get crc32 of whole data created by lse
    _PROJECT *Get_PROJECT(void); // get information about project
};

class _Language
{
  private:
    _UDWORD        Ids[50];
    _UWORD         NoIds;
    _LANGUAGE_INFO LanguageInfo;

    _BOOL   MakeDPNE(_ASCII*, _UWORD, _BOOL);
    _BOOL   ReadLanguageInfoSub(_ASCII*, _LANGUAGE_INFO*, _MEM_NO*);

  public:
    void    Free(void);
    void    Init(void);
    _BOOL   LoadDirectory(_ASCII*);
    _BOOL   LoadLanguage(_UWORD);
    _BOOL   MakeLanguageInfo(void);
    _UWORD  GetNo(void);
    _BOOL   Read_LANGUAGE_INFO(_File*);
    _LANGUAGE_INFO* Get_LANGUAGE_INFO(void);
    _UDWORD GetIDByIndex(_UDWORD idx);
    _UWORD  GetIndexByID(_UDWORD);
    _UDWORD GetActualID(void);
    _LOCAL  GetActualLocal(void);
    void    GetStandardText(void **ptxt, _UWORD *pchrsize, _STDTEXTID txtid);
    _BOOL   SetTextRenderEngine(_UWORD);
   #ifdef UC_IPC
    _BOOL   SeperateTextlist(_CHAR**, _UDWORD*, _UWORD, _UWORD);
   #endif
};

class _SetUp
{
  private :
    _BUBBLEINFO  BubbleInfo;
    _EDITCOLOR   ActiveEditColor;
    _EDITCOLOR   ChangeEditColor;
    _EDITCOLOR   SelectGroupColor;
    _CURSORCOLOR SelectCursorColor;
    _CURSORCOLOR ActiveCursorColor;
    _UWORD       TextFrameDistance;
    _BOOL        KeepObjectOffset;
    _BOOL        ZoomImageList;
    _BOOL        DeleteBackground;
    _COLOR       BackgroundColor;
    _SCREENSAVER ScrSaver;
    _BOOL        AutomaticSeekFirstInput;
    _BOOL        DeleteEventBuffer;
    _SOFTKEYSETUP SoftkeySetup;
    _ENTER_PLUS  DirectionOnEnter;
    _UDWORD      ColorsNo;
    _BOOL        UseTouchEditor;   // open toucheditor on input
    _DATA        TargetNotice;
    _BOOL        EditorIncDec;
    _BOOL        HotkeysInMenu;
    _BOOL        UseShuttleToEdit;
    _BOOL        UseHostName;
    _BOOL        WordWrap;
    _BOOL        DisableAltEsc;
    _BOOL        DisplayPassword;
    _BOOL        DoubleClickOption; // dr918
    _DEFKEYPAD   DefKeyPad;
    _UDWORD      CheckBoxImage[2];
    _UDWORD      RadioBoxImage[2];
    _BOOL        PossiblyLocalTextInUse;

    _BOOL       Read(_File*);
    _COLOR      IfNecChangeColor(_COLOR, _COLOR);

  public :
    _UDWORD     StartupTimeLseProject;
  
    void        Init(void);
    void        Free(void);
    _UWORD      GetNo(void);
    _BOOL       Load(_ASCII*);
    _UWORD      GetFrameSpace(void);
    _COLOR      GetBackgroundColor(void);
    _BOOL       GetDeleteBackground(void);
    _BOOL       GetPossiblyLocalTextInUse(void);
    _EDITCOLOR  *Get_EDITCOLOR(_EDITSTATE);
    _COLOR      GetEditColorBack(_EDITSTATE, _COLOR);
    _COLOR      GetEditColorFrame(_EDITSTATE, _COLOR);
    _COLOR      GetEditColorTxt(_EDITSTATE, _COLOR);
    _COLOR      GetEditColorCursor(_EDITSTATE);
   #ifdef UC_BUBBLE_HELP
    _BUBBLEINFO *Get_BUBBLEINFO(void);
    _COLOR      GetBubbleColorBack(void);
    _COLOR      GetBubbleColorFrame(void);
    _COLOR      GetBubbleColorTxt(void);
    void        Set_BUBBLEINFO_delaytime(_DWORD);
    void        Set_BUBBLEINFO_remaintime(_DWORD);
   #endif
    _SCREENSAVER *Get_SCREENSAVER(void);
    _BOOL       GetAutomaticSeekFirstInput(void);
    _BOOL       GetDeleteEventBuffer(void);
    _UWORD      GetSoftkeyHeight(void);
    _ENTER_PLUS GetDirectionOnEnter(void);
    _SOFTKEYSETUP *Get_SOFTKEYSETUP(void);
    _DEFKEYPAD  *Get_DEFKEYPAD(void);
    _BOOL       GetKeepObjectOffset(void);
    _BOOL       GetZoomImageList(void);
    _UDWORD     GetColorsNo(void);
    _BOOL       GetUseTouchEditor(void);
    _DATA*      GetTargetNotice(void);
    _BOOL       GetEditorIncDec(void);
    _BOOL       GetUseShuttleForEdit(void);
    _BOOL       GetHotkeysInMenu(void);
    _BOOL       GetUseHostName(void);
    _BOOL       GetWordWrap(void);
    _BOOL       SetWordWrap(_BOOL newstate);
    _BOOL       GetDisableAltEsc(void);
    _BOOL       GetDisplayPassword(void);
    void        SetDoubleClickOption(_BOOL);
    _BOOL       GetDoubleClickOption(void);
    void        SetChkBoxImages(_UDWORD unchecked, _UDWORD checked);
    void        SetRadioBoxImages(_UDWORD unchecked, _UDWORD checked);
    _BMP*       GetChkBoxImage(_DWORD value);
    _BMP*       GetRadioBoxImage(_DWORD value);
};

class _ImageList
{
  private :

    _MEM_BMP     Data;

    _BOOL  Read(_File*, _ASCII*);

  public :
    _AsciiPuffer LseLabel;

    void   Init(void);
    void   Free(void);
    _BOOL  Load(_ASCII*);
    _BMP   *Get(_UWORD);
    _UWORD GetNo(void);
    _BOOL  Add(_BMP*, _ASCII*);
   #ifdef UC_IPC
    _UDWORD MemSize(_MEMSIZE*);
   #endif
};

class _Font
{
  private :
    _AsciiPuffer FontFileName;
    _FONTINFO    DefaultFont;
//    _FONT        ActiveFont;
//    _BOOL        FontKaputt;
    _UWORD       No;
		_FONTINFO    *Ptr;
    _UDWORD      MemDataSize;
    _UWORD       ActualHeight;

    void    GetSizeDirect(_UWORD*, _UWORD*, _UWORD);
    _BOOL   LoadFontFile(_FONTINFO*, _ASCII*);

  public :
    _AsciiPuffer LseLabel;

    _UDWORD LoadFont(_ASCII*, _UWORD, _ATTRIB);
    void    Init(void);
    void    Free(void);
    _BOOL   Load(_ASCII*);
//    _BOOL  Set(_FONT);
    _BOOL   Set(_UWORD, _ATTRIB*);
    _BOOL   Set_By_Language(_UWORD, _ATTRIB*, _UWORD);
    _BOOL   SetDirect(_FONT, _ATTRIB*);
    _UDWORD GetMemorySize(_FONT);
    _UDWORD GetMemorySizeDirect(_FONT);
    void    Size(_UWORD, _UWORD*, _UWORD*);
    void    SizeDirect(_FONT, _UWORD*, _UWORD*);
    _UWORD  SeekFileName(_ASCII*);
    _ASCII  *GetFileName(_UWORD);
    _UWORD  GetNo(void);
    _ATTRIB GetAttrib(_UWORD);
//    void    Destroy(void);
    _UWORD  GetActualHeight(void);
   #ifdef UC_IPC
    _UDWORD MemSize(_MEMSIZE*);
   #endif
    _UDWORD Add(void *fontptr, _ASCII *label, _ATTRIB attrib, _BOOL isheap);
};

#ifdef UC_IPC
class _Gfx
{
  private :
    _UWORD  No;
	 _GFXBMP *Ptr;

	 _UDWORD Scan4File(_ASCII*);
	 _BOOL   AddToMem(_GFXBMP*);

  public :
	 void    Init(void);
	 void    Free(void);
	 _BOOL   Load(_ASCII*);
	 _BOOL   Out(_ROOM*, _ASCII*, _ATTRIB, _BOOL);
	 _BMP    *Get(_ASCII*);
};
#endif


#ifdef UC_DIRECTORY

#pragma pack (push, 1)
typedef struct { _SCROLL      scroll;
                 _AsciiPuffer Label;
               }
_DIRLIST;

typedef struct { _FONT        font;
                 _UWORD       lineheight;
                 _UWORD       linewidth;
                 _UWORD       lineonpage;
                 _DOT         offset;
                 _COLOR       color_inactive;
                 _COLOR       color_active;
                 _COLOR       color_frame;
               }
_DIRECTORYSETUP;

typedef struct { _WORKSTATE   workstate;
                 _ASCII       path[_MAX_PATHLENGTH];
                 _ASCII       dpne[_MAX_PATHLENGTH];
                 _ASCII       name[_MAX_PATHLENGTH];
                 _FBOOL_PT_PT fkt;
                 void        *data;
                 void        *pthis;
                 _BOOL        ready;
                 _UWORD       activ;
                 _UWORD       oldactiv;
                 _DIRLIST     Files;
                 _DIRLIST     Direct;
                 _CHREDIT     chredit;
                 _DIRECTORYSETUP dirset;
                 _ASCII       preelect[100];
               }
_DIRECTORY;
#pragma pack (pop)

class _Directory
{
  private :
    _DIRECTORY Dir;

    void  AddDPNE(_ASCII*, _ASCII*);
    void  SubDPNE(_ASCII*);
    _BOOL Start(_DIRECTORY*, _BOOL, _ROOM*);
    _BOOL Ready(_DIRECTORY*, _BOOL, _ROOM*);
    _BOOL Run(_DIRECTORY*, _EVENT*, _ROOM*);
    _BOOL End(_DIRECTORY*);

    void  ActivWatch(_DIRECTORY*, _BOOL, _UWORD);
    void  RunFileBox(_DIRECTORY*, _EVENT*);
    void  RunDirectBox(_DIRECTORY*, _EVENT*, _ROOM*);
    void  RunEditorBox(_DIRECTORY*, _EVENT*);
    void  OutEditorBox(_DIRECTORY*, _BOOL);
    void  OutPath(_DIRECTORY*);
    _BOOL IsHIDinEditor(_EVENT*, _DIRECTORY*);
    void  GetEditorRoom(_ROOM*, _DIRECTORY*);

  public :
    void  Init(void);
    void  Free(void);
    void  Setup(_FILE_STATE, _ASCII*, _FBOOL_PT_PT, void*, void*, _DIRECTORYSETUP*);
    _BOOL Call(_WORKSTATE, _EVENT*, _BOOL, _ROOM*);
};
#endif

class _Softkey
{
	private :
		_TextPuffer SoftkeyText;
    _UWORD      NoFloor;
    _UWORD      NoSoftkeys;
    _UWORD      ActFloor;
    _SOFTFLOOR  *Ptr;
    _BOOL       CopyOfPrevious;
   #ifdef XUSER_KM
    _UWORD      AssignedImage;
    _UWORD      PicNoMemo;
   #endif
   #ifdef UC_DYN_SOFTKEYS
    _UBYTE      RunCount;
   #endif

    void        DrawSingleSoftkey(_SINGLESOFT*, _UWORD, _BOOL, _UDWORD);
		void        GetSingleSize(_ROOM*, _UWORD);
    void        RefreshSingleSoftkey(_SINGLESOFT*, _UWORD);
    _BOOL       ChkAccess(_SINGLESOFT*, _BOOL);
    _BOOL       KeymodeSingleSoftkey(_UWORD, _TIP);

  public :
    void        Init(void);
    void        Free(void);

    _BOOL       Read(_File*);
    void        GetSize(_ROOM*);
		_DWORD      Set(_DWORD);
    _BOOL       Do(_UWORD, _Window*, _EVENT*);
		_BOOL       Copy(_Softkey*);
    void        DoAllNegEdge(_BOOL);
    _BOOL       Draw(void);
   #ifdef HID
    _UWORD      GetSoftCode(_UWORD);
    _BOOL       DoHID(_EVENT*);
   #endif
    _TextPuffer *GetTextPuffer(void);
    _SOFTFLOOR  *GetSoftFloor(void);
    _UWORD      GetNoFloor(void);
    _UWORD      GetActFloor(void);
    _UWORD      GetNoSoftkeys(void);
    _BOOL       GetCopyOfPrevious(void);
    void        SetCopyOfPrevious(_BOOL);
   #ifdef UC_DYN_SOFTKEYS
    _BOOL       Refresh(_BOOL);
   #endif
   #ifdef XUSER_KM
    _SINGLESOFT* Get_SINGLESOFT(_UWORD, _UWORD);
    void        SetAssignedImage(_UWORD);
    _UWORD      GetAssignedImage(void);
    void        PushScreenNo(_UWORD);
    _UWORD      PopScreenNo(void);
   #endif


};

#ifdef XUSER_KM
class _AllSoftkeys
{
  private:
    _UWORD   No;
    _Softkey *Ptr;
    _UDWORD  TimeDelay;

    _BOOL        IsScreenSoftkey(_SINGLESOFT*);
    _SINGLESOFT* Get_SINGLESOFT(_PRJ_ENTRY*, _UWORD, _UWORD);
    _UWORD       SeekNextPictureSub(_UWORD, _UWORD, _UWORD, _BOOL);
    _UWORD       SeekNextPicture(_SINGLESOFT*, _UWORD, _UWORD*, _BOOL, _BOOL*);
    _Softkey*    GetSoftkey(_UWORD);

  public:
    void         Init(void);
    void         Free(void);
    void         Scan(void);
    void         AssignSoftInfo(void);
    _UWORD       GetPictureNo(_SINGLESOFT*, _UWORD, _UWORD*, _BOOL, _BOOL*);
    _UWORD       GetImageNo(_SINGLESOFT*, _UWORD, _BOOL*, _UWORD);
    void         DoKey(_SINGLESOFT*, _UWORD, _Window*, _BOOL, _BOOL);
    _BOOL        IsHotkey(_UWORD);
    _SINGLESOFT* Get_SINGLESOFT(_IMA, _UWORD, _UWORD, _UWORD);
};
#endif


typedef struct { _ROOM   room;
                 _COLOR  color;
                 _BOOL   state;
                 _UWORD  visible;
                 _UWORD  shape;
                 _UDWORD timediff;
                 _UDWORD oldtime;
               }
_XCURSOR;

class _Cursor
{
  private :
    _XCURSOR Cursor;
    void     Draw(void);

  public :
    void     Push(void);
    void     Pop(void);
    void     Init(void);
    void     Free(void);
    void     Hide(void);
    void     Set(_ROOM*, _COLOR, _UWORD);
    void     SetC(_ROOM*, _COLOR, _UWORD, _COLOR);
    _BOOL    Set(_IO*, _EDITSTATE);
    void     End(void);
    void     Run(void);
    void     CheckRoom(_ROOM*, _UWORD);
    _XCURSOR *Get_XCURSOR(void);
};


class _Window : public _Stack
{
	private :
    _Window   *P_Window;
    _Window   *P_Background;
    _Picture  *P_Picture;
    _PopUp    *P_PopUp;
    _UDWORD   MessBoxIndex;           // wird für standard-messagebox verwendet
    _Softkey  Softkey;
    _WHOAMI   WhoAmI;                                                     // id
    _REGION   Region;
    _DOT      PicOffset;
    _BOOL     IsReady;
    _BOOL     Shadow;
    _BOOL     Sign;
    _BOOL     TouchEditorFlag;
    _BOOL     DisableEvents;
    _UDWORD   EventBreather;
    _UDWORD   ScreenSaverEndCnt;
    _UDWORD   EditServerNo;  // servernummer wenn editor geöffnet aber keine eingabe dazu vorhanden ist

    void    EndInternSub(void);

    void    AddRegion_MOTION(_REGION);            // add motion-room to region

    void    RedrawScreenSub(void);
    _BOOL   GetEvent(_EVENT*);                // get keyboard, touch and mouse
    void    SubtractRegion(_REGION*);                            // sub region
    void    ReDrawBackground(_BOOL);
    void    DrawBackground(_REGION*);                     // backgroundrestore
    _BOOL   CMD_ScreenSave(void);
    _BOOL   NewWindowBasics(_PIC_COMMAND , _DWORD);
//    void    CMD_ReConfigVar(_IVALUE);
    _BOOL   CMD_NewWindow(_PIC_COMMAND, _DWORD);
    _BOOL   CMD_OpenBubbleHelp(void);
    _BOOL   CMD_NewPopUp(_DWORD, _VARLABEL*, _KOORD=-1, _KOORD=-1, _VARLABEL *vl=NULL, void *pmyio=NULL);
    void    CMD_FirstDraw(void);
    void    CMD_Region(void);
    void    CMD_OutSoftkey(_DWORD);
    
    _RUN_STATE DoCommands(void);                           // execute commands
    _BOOL   LoadPicWin(_UWORD, _IMA, _Window*);  // load pic, win, screensaver
//    _BOOL   LoadPopUp(_UWORD, _Window*, _KOORD=-1, _KOORD=-1, _VARLABEL *vl=NULL, _VARLABEL *ovl=NULL); // load popupmenu
    _BOOL   LoadPopUp(_UWORD, _Window*, _KOORD, _KOORD, _VARLABEL*, _VARLABEL*, void *pmyio=NULL); // load popupmenu
    void    DoSoftkeyPress(_EVENT*);                             // do softkey
    _BOOL   IfNecCopySoftkey(_Softkey*);
		void    DoAllNegEdge(_BOOL);
    void    IAm(_IMA, _UDWORD);
    _BOOL   ReloadText(void);
    void    DoFlipScreen(void);
    void    CMD_FlipScreen(_KOORD, _KOORD, _KOORD, _BOOL);
//    void    LookUpDiagnostic(_EVENT*);
    _BOOL   CallScreensaverEnd(_EVENT *pe);

   #ifdef HID
    _BOOL   CMD_OpenServer(_UDWORD serverno, _UDWORD winno);
   #endif
    void    FinishCarouselSwim(void);
   
//    _EDITOR *GetEditor(void);
//    _EDITOR *CallInterface_If_InputIsSaved_sub(_Window*, _UDWORD);
//    _BOOL   CallInterface_If_InputIsSaved(_UDWORD, _RESULT*);

  protected :
    void    UserCall(_DWORD, _DWORD);                      // interface: fcawl

  public :
    _RUN_STATE RunIntern(void);                                  // run window
    _BOOL      GetSize(_ROOM*);                          // get size of window

    void     Init(void);                                               // init
    void     Free(void);                                               // free
    void     StartIntern(_BOOL);
    void     ReadyIntern(_BOOL);
    void     EndIntern(_BOOL);
		void     Start(_BOOL);                                 // interface: start
    void     Ready(_BOOL);                                 // interface: ready
    void     Run(_EVENT*);                                   // interface: run
    void     End(void);                                      // interface: end
    void     MakeRegion(void);                                // create region
    void     Draw(_BOOL, _BOOL);                                       // draw window
    _Window *FindRoot(void);                     // returns --> to root window
    _Window *FindTop(void);                       // returns --> to top window
    void     GetUsedMessBoxIndex(_UBYTE *pmidx);
    _BOOL    CMD_OpSystem(_PIC_COMMAND, _IVALUE, _IVALUE);
    _BOOL    CMD_OpenEditor(_STACK_LINE*, _BOOL, _BOOL, _EVENT*);
    _BOOL    CMD_CloseBubbleHelp(void);
    _BOOL    CMD_CloseEditor(void);
    _BOOL    CMD_KillEditor(void);
		_BOOL    CMD_SaveEditor(_BOOL);
    _BOOL    OpenComboBox(_UDWORD menunumber, _VARLABEL *vl, _VARLABEL *ovl, _KOORD x, _KOORD y, _BOOL killeditorafterwards, _BOOL lrm, void *pmyio);
    _BOOL    GetReady(void);
    void     RedrawScreen(_ROOM*);
    _BOOL    AddCommand(_PIC_COMMAND, _VARLABEL*, _VARLABEL*, _BOOL);  // add command
		_BOOL    AddCmd(_PIC_COMMAND, _DWORD, _DWORD, _BOOL);      // add command
    void     DeleteAllCmd(void);
    _BOOL    SystemError(_SYSERROR, _DWORD, _CHAR*);           // system error
		_Softkey *GetSoftkey(void);
    void     *GetContentPointer(void);
    _WHOAMI  GetWhoAmI(void);
    _REGION  GetRegion(void);
    _PopUp   *GetP_PopUp(void);
    _Picture *GetP_Picture(void);
    _Window  *GetP_Window(void);
    _Window  *GetP_Background(void);
    _Window  *SetP_Window(_Window*);
    _BOOL    GetShadow(void);
    void     SetShadow(_BOOL);
    void     SetSign(_BOOL);
    _DOT     GetPicOffset(void);
    void     SetPicOffset(_DOT);
    void     PowerDown(void);
    void     KillBackGroundImage(void);
    _Cursor  *GetCursor(void);
    void     Overload(_VARLABEL*);
    _BOOL    GetTouchEditorFlag(void);
    void     SetTouchEditorFlag(_BOOL);
    _BOOL    DrawPicWin(_IMA, _UWORD, _UWORD, _DOT*);
    _BOOL    ChangeLanguage(_DWORD);
    _BOOL    CallInputIsSavedOnSign(_VARLABEL*, _BOOL lrm);
    void     RefreshJustTopWindow(_BOOL);
    void     DoStart(void);
   #ifdef USER_BILLION
    _BOOL    InvertInputColor(_UDWORD);
   #endif
    _FRAMETYPE GetBackgroundTheme(_ROOM *proom);
    _BOOL    IsThereAnyCarousel(void);
    _UDWORD  GetCarouselRoom(_ROOM*);
    _UDWORD  GetMessBoxIndex(void);
    _UDWORD  GetEditServerNo(void);
//    _BOOL    InjectEditorText(_CHAR *txt);
};

class _PopUp
{
  private :
    _MENU     Menu;
    _VARLABEL ImOverloaded;
    _Window   *P_Window;
    _REGION   Region;
    _VARLABEL Input;
    void      *P_MyIo;
   #ifdef UC_MENU_NEWSCROLL 
    _DOT      prDot;
    _BOOL     bPressOK;
    _UWORD    uScrollState;
    _UDWORD   udTimeLastMove;
    _UDWORD   udTimeLastScroll;
    _DWORD    dActMoveDelay;
    _BOOL     bIsRunning;
	_BOOL     bIsFirstStart;
    _UDWORD   udOldBegin;
   #endif
  public :
    _Cursor   Cursor;
		void      *pUser;

		void      GetLinePosition(_UWORD, _ROOM*);
    void      DrawBackground(_BOOL);
    void      OutLine(_UWORD, _UWORD, _COLOR, _BOOL, _BOOL, _BOOL);
    void      Beam(_UWORD, _UWORD, _BOOL, _BOOL);
    void      OutPage(_UWORD, _BOOL);
    void      DoPositions(_EVENT*);
    void      ScrollUp(void);
    void      ScrollDown(void);
    void      Calculate(void);
    void      Draw(void);
    void      Init(void);
    void      Free(void);
    void      StartIntern(_BOOL);                          // interface: start
    void      ReadyIntern(_BOOL);                          // interface: ready
		void      Run(_EVENT*);                                  // interface: run
    void      End(void);                                     // interface: end
    _BOOL     Load(_UWORD, _Window*, _VARLABEL*, _VARLABEL*, void *pmyio=NULL);
    void      PopUpAddRegion(void);                 // make region for picture
    _BOOL     Open(_UWORD, _Window*, _KOORD, _KOORD, _VARLABEL*, _VARLABEL*, void *pmyio=NULL);
    void      GetSubPosition(_KOORD*, _KOORD*);      // get position sub-popup
    _MENU     *GetMenu(void);
    void      GetSize(_ROOM*);
    _VARLABEL *GetInput(void);
    void      *GetP_MyIo(void);
    _VARLABEL *GetImOverloaded(void);
    void      OutBasement(_KOORD, _UWORD, _ASCII*, _COLOR);
//   _BOOL      ReloadText(_WHOAMI*);
#ifdef UC_MENU_NEWSCROLL
    _BOOL       do_POPUP_SCROLL(_EVENT*, _LEVEL*, _POSITION*, _ROOM*, _UWORD);
    _POSRESULT  calculate_PopUpPosition(_POSITION*, _EVENT*, _UWORD);
#endif
};



class _SubPicture
{
  private :
    _PICTURE Picture;                                 // all picture-elements

    _BOOL  NumericSize(_IO*, _ROOM*);
    _BOOL  StandardSize(_IO*, _ROOM*);
    _BOOL  ChkNumeric(_RESULT*);
    _BOOL  ChkAscii(_RESULT*);
    _BOOL  ChkBox(_RESULT*);
    _BOOL  ChkBar(_RESULT*);
    _BOOL  ChkTxtScheme(_RESULT*, _IO*);
    _BOOL  ChkTBO(_RESULT*, _IO*);
    _BOOL  ChkTime(_RESULT*);
    _BOOL  ChkDate(_RESULT*);
    _BOOL  ChkMenue(_IO*, _RESULT*);
    _BOOL  ChkEnumeration(_IO*, _RESULT*);
		_BOOL  ChkVarName(_RESULT*);
    _BOOL  ChkTHEURGE(_IO*, _RESULT*);
    _BOOL  OutNumeric(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  OutIpAddress(_IO *pio, _RESULT *pr, _VARIABLE *pv);
    _BOOL  OutAscii(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  OutRadio(_IO*, _RESULT*);
    _BOOL  OutChkBox(_IO*, _RESULT*);
    _BOOL  OutBar(_IO*, _RESULT*);
    _BOOL  OutTBO(_IO*, _Picture*);
    _BOOL  OutTxtScheme(_IO*);
    _BOOL  OutTime(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  OutDate(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  OutMenu(_IO*, _RESULT*, _BOOL);
    _BOOL  OutUser(_IO*, _RESULT*);
		_BOOL  OutVarName(_IO*, _RESULT*, _VARIABLE*, _BOOL);
    _BOOL  OutNumericHide(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  OutAsciiHide(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  OutTHEURGE(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  GetValueTHEURGE(_RESULT*, _VARIABLE*, _IO*, _BOOL);

    _BOOL  SingleSizeIo(_IO*, _DOT*, _ZOOM*);
		_BOOL  SingleCheckIo(_IO*, _RESULT*);
    _BOOL  SingleChangeIo(_IO*, _RESULT*, _VARIABLE*, _Picture*, _BOOL input);
    _BOOL  SingleOutIo(_IO*, _RESULT*, _VARIABLE*, _DOT*, _ZOOM*, _BOOL, _UWORD, _Picture*, _BOOL);
   #ifdef OK_ENCLOSE_NAME
    void   CallMyIoRun(_IO*, _BOOL);
    void   *GetThisPointer(_UWORD);
    void   RunEnclosedName(_PICTUREMASK*, _EVENT*, _EDITOR*);
   #endif

//    void   Out_RECTANGLE(_RECTANGLE*, _DOT*, _ZOOM*);
//    void   Out_POLYGON(_POLYGON*, _DOT*, _ZOOM*);
//		void   Out_CIRCLE(_CIRCLE*, _DOT*, _ZOOM*);
    void   Out_BITMAP(_BITMAP*, _DOT*, _ZOOM*);
    void   Out_PICTEXT(_PICTEXT*, _DOT*, _ZOOM*, _BOOL);
    _BOOL  Refresh_OBJECT(_UWORD, _OBJECT*, _DOT*, _ZOOM*, _BOOL, _Picture*);
    _BOOL  Out_COL_RECT(_COL_RECT*, _DOT*, _ZOOM*, _BOOL);
    _BOOL  Out_COL_POLY(_COL_POLY*, _DOT*, _ZOOM*, _BOOL);
    _BOOL  Out_COL_CIRC(_COL_CIRC*, _DOT*, _ZOOM*, _BOOL);
    _BOOL  Out_COL_PICTXT(_COL_PICTXT*, _DOT*, _ZOOM*, _BOOL);
   #ifdef UC_OLD_MOTIONS
    _BOOL  Refresh_MOTION(_PICTUREMASK*, _UWORD, _DOT*, _ZOOM*, _BOOL);
   #else
    _BOOL  Refresh_MOTION(_UWORD, _MOTION*, _DOT*, _ZOOM*, _BOOL, _Picture*);
   #endif
    _BOOL  Refresh_CAROUSEL(_UWORD actno, _MOTION *p, _DOT *offset, _ZOOM *zoom, _BOOL init, _Picture *Ppic, _UDWORD swimtime);
    _BOOL  Out_MOTION(_MOTION*, _DOT, _BOOL);
    _BOOL  Refresh_BUTTON(_BUTTON*, _DOT*, _ZOOM*, _BOOL, _Picture*);
    void   Refresh_OUTPUT(_IO *pio, _DOT *offset, _ZOOM *zoom, _UDWORD actno, _Picture *Ppic);
    void   Refresh_INPUT(_INPUT *pi, _DOT *offset, _ZOOM *zoom, _UDWORD actno, _Picture *Ppic);
    _BOOL  RefreshAll(_UWORD actno, _DOT *offset, _ZOOM *zoom, _Picture *Ppic);
    _BOOL  RefreshPart(_UWORD actno, _DOT *offset, _ZOOM *zoom, _Picture *Ppic);
//    _UWORD SeekHorizontLine(_PICTUREMASK*, _UWORD, _ROOM*,_BOOL);
//    _UWORD SeekVerticalLine(_PICTUREMASK*, _UWORD, _ROOM*, _BOOL);
//    _UWORD SeekVerticalVirt(_PICTUREMASK*, _UWORD, _ROOM*, _BOOL);
//    _UWORD SeekHorizontVirt(_PICTUREMASK*, _UWORD, _ROOM*, _BOOL);
    _BOOL  ChkInput(_PICTUREMASK*, _UWORD);
   #ifdef OK_SEEK_NEXT
    _UWORD GetNextNo(_UWORD, _BOOL);
    _UWORD SeekNext(_UWORD, _BOOL);
    _UWORD SeekLast(_UWORD, _BOOL);
   #endif

//    _BOOL  ChkInput(_INPUT*);
    _BOOL  Out_THEURGE_Bar(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  Out_THEURGE_Indicator(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  Out_THEURGE_Clock(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  Out_THEURGE_Scale(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  Out_THEURGE_ScreenClean(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  Out_THEURGE_Slider(_IO*, _RESULT*, _VARIABLE*);
    _BOOL  GetValue_THEURGE_ScreenClean(_RESULT*, _IO*);
    _BOOL  CaptureBackGround(_IO*, _DOT*, _ZOOM*);

    _BOOL   DoNegEdge(_BUTTON *pt);
    void    ReleaseMoveButtonGroup(_UDWORD multindex);
    _BOOL   SetMoveButtonGroup(_UDWORD multindex, _UDWORD summary);
    _UDWORD GetMoveButtonGroup(_UDWORD multindex);
    void    InitMoveButtonGroup(void);

  protected :
    _BOOL  OutIO(_IO*, _RESULT*, _VARIABLE*, _BOOL);       // interface: outio

  public :
		void   Init(void);
    void   Free(void);
    _MOTION* Get_MOTION(_DWORD index);
    void   Draw(_DOT*, _ZOOM*, _Picture*);
//    void   RefreshHiPriority(_UWORD, _DOT*, _ZOOM*);
    _BOOL  Refresh(_UWORD, _DOT*, _ZOOM*, _Picture*);
    _BOOL  Read(_File*);
    _BOOL  OutIoIntern(_IO*, _DOT*, _ZOOM*, _BOOL, _BOOL, _UWORD, _Picture*, _BOOL);
    _UWORD GetLastEditor(void);
    void   SetLastEditor(_UWORD);
//    _BOOL  OutInput(_UWORD, _DOT*, _ZOOM*, _BOOL);
    void   AddRegion_MOTION(_REGION);        // make region for motion-element
    _BOOL  OutIoComponent(_IO*, _RESULT*, _VARIABLE*, _DOT*, _ZOOM*, _BOOL, _UWORD, _Picture*, _BOOL);
    _BOOL  DoKeyboard(_EVENT*, _Window*, _BOOL);
     void  DoButton(_BUTTON*, _BOOL, _Window*, _DOT*, _ZOOM*, _BOOL, _BOOL);
    _BOOL  RefreshTouchButton(_BUTTON*, _BOOL, _DOT*, _ZOOM*);
    _BOOL  Out_BUTTON(_BUTTON*, _DOT*, _ZOOM*, _COLOR);
    void   ReDraw(_DOT*, _ZOOM*);
    void   DoAllNegEdge(_BOOL);
    _BOOL  CopyEditor(_EDITOR*);
   #ifdef HID
    _BOOL  DoHID(_EVENT*, _Window*, _DOT, _ZOOM*);       // do hid event (mouse, touch, ...)
    #ifdef UC_THEME
    _BOOL  SeekThemeCancle(_DOT*, _DOT, _ZOOM*, _BOOL);
    #endif
	 #endif
   #ifdef OK_ENCLOSE_NAME
    _BOOL  FindVirtualName(_IO*);
   #endif
    _BOOL  Copy(_SubPicture*);                              // copy subpicture
    void   GetSize(_ROOM*);
    _PICTURE *GetPICTURE(void);
//    _BOOL  DoUserInit(_UWORD, _BOOL);
   #ifdef OK_RUNTIME_MODIFY
    _BOOL  InsertNextNo(_UWORD, _BOOL);
   #endif
   #ifdef OK_SUMMARY
    void   OutSummary_IO(_IO*, _BOOL, _UWORD, _DOT*, _ZOOM*, _BOOL, _Picture*);
    void   OutSummary(_BOOL, _UWORD, _DOT*, _ZOOM*, _BOOL, _Picture*);
   #endif
    _BOOL Overload(_VARLABEL *ov, _BOOL change_ctrl_type = _FALSE);
    _BOOL ChangeOverload(_VARLABEL*, _VARLABEL*);
    _BOOL SummaryOverload(_UWORD, _VARLABEL*);
    void  KillBackGroundImage(void);
    void  SetButtonState(_BOOL);
    _BOOL AddObject(_UWORD, _ROOM*, _UDWORD);
    _BOOL Add(_PICTEXT *ptxt);
    _BOOL Add(_COL_PICTXT *pctxt);
    _BOOL Add(_BUTTON *pbut);
    _BOOL Add(_IO *pio);
    _BOOL Add(_INPUT *pio);
    _BOOL Add(_BITMAP *bmp);
    _BOOL Add(_RECTANGLE *prec);
    _BOOL Add(_COL_RECT *pcrec);
		_DWORD Add(_MOTION*);                   // add motion to subpicture
    
   #ifdef NINETEEN
    _DWORD CyclicButton(_EVENT*, _DOT*, _ZOOM*);
   #endif
    _FRAMETYPE GetBackgroundTheme(_ROOM*);
    _BOOL MakeStandardWindow(_UDWORD no);
    void  MakeSize(void);
    _BOOL ChkChkBit(void);
    _BOOL IsThereAnyCarousel(void);
    _UDWORD GetCarouselRoom(_ROOM*);
    void  FinishCarouselSwim(_UWORD actno, _DOT *po, _ZOOM *zoom, _Picture *Ppic);
    
};

class _Picture
{
  private :
    _MY_PICTURE Pic;
    _BOOL       DoAutomaticSeekInput;
    _UDWORD     AutomaticSeekTimer;
   #ifdef UC_TOUCHBUBBLE_BY_SELECTION
    _UWORD      BubbleState;
    _UWORD      BubbleInput;
    _UDWORD     TimeDiff;
    void        Init_BubbleState(void);
   #endif
    _BOOL       RecalcIoID;
    _DOT        RecalcIoIDot;

    _BOOL Read(_File*, _BOOL, _UWORD);                         // load picture
		void  CalculateSize(_BOOL);                              // calculate size

    void  GetSoftkeySize(_ROOM*);
    void  DrawSingleSoftkey(_SINGLESOFT*, _UWORD, _KOORD, _UWORD);
    _BOOL CursorJump(void);

    _BOOL EditorSetValue(_RESULT *presult);
    _BOOL SaveEditNumeric(void);
   #ifdef USER_BILLION
    _BOOL SaveEditDaTim(void);
   #endif
    _BOOL SaveEdit(void);
    _BOOL SaveEditAscii(void);
    _BOOL EditNumeric(_UWORD, _UWORD, _IO_TYPE, _BOOL);
   #ifdef OK_SEEK_BUTTON
    _BOOL EditButton(_UWORD, _BOOL, _BOOL);
   #endif
		_BOOL EditMenu(_UWORD, _UWORD, _BOOL);
    _BOOL JapanDictionaryOpenDropDownList(_CHAR *keyword);
    _BOOL JapanDictionaryGetFirstMatch(void);
    _BOOL EditAscii(_EVENT*);
    _BOOL Edit(_UWORD);
    _BOOL EditChkBox(_UWORD);
    _BOOL EditRadio(_UWORD);
    _BOOL CheckForClose(void);

    _BOOL EditDate(_UWORD, _BOOL);
    _BOOL EditTime(_UWORD, _BOOL);
    _BOOL EditIpAddress(_UWORD sc, _BOOL is_character);
    _BOOL EditorToResult(_RESULT*, _EDITOR*);
    _BOOL EditorCheckLimits(_EDITOR *pe);
    _COLOR EditorGetColorText(_EDITOR *pe, _COLOR preselect);
   #ifdef HID
   #ifdef UC_THEME
    _BOOL SeekThemeCancle(_DOT*, _DOT, _ZOOM*, _BOOL);
   #endif
   #endif
    _BOOL SeekHid(_DOT *pd, _BOOL welcome, _BOOL lrm, _SELECTOR selector, _EVENT *pev);
    _BOOL Seek(_EVENT*);                                         // seek input
   #ifndef UC_NO_TAB_CURSORNAVIGATION
		_BOOL SeekTab(_UWORD*, _UWORD*, _BOOL, _DOT*, _ZOOM*, _SELECTOR);
   #endif
   #ifdef USER_BILLION
    _BOOL ChkBillionNeeds(_EDITOR*);
   #endif

  public :
    _Cursor  Cursor;
    _UWORD   RefCyc;

    void  Init(void);                                                  // init
    void  Free(void);                                                  // free
    _BOOL Load(_ASCII*, _Softkey*, _BOOL, _BOOL, _UWORD);      // load picture
    _BOOL LoadPicture(_ASCII*, _UWORD, _Window*, _BOOL, _UWORD);
    _BOOL LoadWindow(_ASCII*, _UWORD, _Window*);                // load window
    void  Draw(_DOT*, _BOOL);                               // draw picture on screen
    void  SetReturn(_Window*);
    void  GetSize(_ROOM*, _DOT*);
    void  PictureRefresh(_DOT*);                               // just refresh
    _WHOAMI GetWhoAmI(void);                                      // return id
    _BOOL DirectionOnEnter(_ENTER_PLUS);
    void  SetRecalcIoID(_BOOL, _KOORD, _KOORD);
    _BOOL EditorCheckLimits(void);

		void  AddRegion_MOTION(_REGION);         // make region for motion-element
    void  DoSoftkey(_UWORD);                            // do softkeyfunctions
//    void  StartIntern(_BOOL);
    void  ReadyIntern(_BOOL);
    void  PictureRunIntern(_Window*, _EVENT*, _DOT*);
    void  EndIntern(_Window*);

    _BOOL PermitSearch(void);
    void  SetLrm(_BOOL);
    _BOOL SaveEditor(_BOOL);
    _BOOL IsEditorOpen(void);
		_BOOL IsEditorTrue(_UWORD);
    _BOOL OpenEditor(_UWORD no, _EVENT *pev=NULL);              // open editor
    _BOOL CloseEditor(void);
		_BOOL ShowEditor(void);
    _BOOL UpdateEditor(void);
    _BOOL ReOpenEditor(void);
   #ifdef OK_ENCLOSE_NAME
    _DOIT GetEventEnclosedName(_EVENT*);
   #endif
    _BOOL RunEditor(_Window*, _EVENT*, _BOOL);
//    _BOOL CheckCursorVisible(_IO_TYPE);
    void  SubEditorRegion(_REGION*);
    void  PictureAddRegion(_DOT*);                   // make region for picture
    _BOOL BubbleHelpOk(_EDITOR*);
    _BOOL CloseBubbleHelp(void);
    _BOOL OpenBubbleHelp(void);
    _MY_PICTURE *GetMyPicture(void);
    _BOOL ReloadText(_Softkey*, _WHOAMI*);
    _BOOL Overload(_VARLABEL*, _BOOL change_ctrl_type = _FALSE);
    _BOOL ChangeOverload(_VARLABEL*, _VARLABEL*);
    _BOOL SummaryOverload(_UWORD, _VARLABEL*);
    void  ReDraw(void);
    _BOOL FindInputByVarNo(_UWORD*, _UDWORD);
    void  KillBackGroundImage(void);
//    void  RefreshEditor(_SubPicture*);
    _BOOL AutomaticSeekInput(_BOOL);
    _BOOL DoHotkey(_EVENT*);
    void  AccessChanged(void);

    void  DoAllNegEdge(_BOOL);
    _EDITOR *GetEditor(void);
		_BOOL CopyEditor(_EDITOR*, _UWORD);
    void  ShowChangedEditor(_EDITOR*);
   #ifdef NINETEEN
    _DWORD CyclicButton(_EVENT*, _UDWORD);
   #endif
   #ifdef USER_BILLION
    _BOOL  InvertInputColor(_UDWORD);
   #endif
    _FRAMETYPE GetBackgroundTheme(_ROOM *proom);
    _BOOL IsThereAnyCarousel(void);
    _UDWORD GetCarouselRoom(_ROOM*);
    void  FinishCarouselSwim(_DOT *po);
//    _BOOL InjectEditorText(_CHAR *txt, _UDWORD pos);
};

class _ObjectList
{
  private :
    _SubPicture *Ptr;
    _UWORD       No;

		_BOOL Read(_File*);

	public :
    _AsciiPuffer LseLabel;

    void        Init(void);
    void        Free(void);
    _BOOL       Load(_ASCII*);
    _SubPicture *Get(_UWORD);
    _UWORD      GetNo();
    _UDWORD     Add(_SubPicture *pic, _ASCII *label);
};


class _Batch
{
  private :
    _BOOL    DoCheck;
    _IPCINI  IpcIni;
    _UDWORD  WatchOnlineTimeDelay;
    _UDWORD  RefListChangedCnt;

//    _COMSET  ComSet;
//    _UWORD   StartPicture;
//		_ASCII   ProjectDPNE[_MAX_PATHLENGTH];
//    _ASCII   DataDPNE[_MAX_PATHLENGTH];
//		_ASCII   StartGfxDPNE[_MAX_PATHLENGTH];
//    _ASCII   SimulationDPNE[_MAX_PATHLENGTH];
//    _UDWORD  LoginDelayTime;
    
    _BOOL    WatchOnlineSub(_BOOL);
    void     MakeOfflineTextSub(_CHAR*, _COMPORT*);
    _UWORD   Insert_COMPORT_Sub(_COMDEF*, _BOOL);
    _BOOL    Insert_COMPORT(_COMDEF*, _UWORD, _BOOL, _UWORD);
    _BOOL    DoStation(_ASCII*);
    _UDWORD  GetIpAdress(_ASCII**);
   #ifdef UC_MULTICPU
    _BOOL    SwitchMultiStationSub(_UWORD, _UWORD, _BOOL);
   #endif

  public :
    void     Init(void);
		void     Free(void);
    void     Read(void);
   #ifdef UC_MULTICPU
    void     LoginAllMultiStations(void);
    _BOOL    SwitchMultiStation(_UWORD, _UWORD, _BOOL);
    void     MapAllMultiStation(_UWORD);
    _BOOL    SetAktMap(_UWORD, _UWORD);
    _UWORD   ChangeAktMap(_UWORD, _UWORD);
    void     GetMultiID(_BOOL);
   #endif
    _BOOL    IsOnline(_COMPORT*);
    _UWORD   GetStationViaComportIndex(_UWORD*, _UWORD);
		_COMPORT *Seek_COMPORT(_COMDEF*);
    _COMPORT *Get_COMPORT(_UWORD);
    _UWORD   GetStartPicture(void);
    _ASCII   *GetProjectDPNE(void);
    _ASCII   *GetDataDPNE(void);
    _ASCII   *GetStartGfxDPNE(void);
    _ASCII   *GetSimulationDPNE(void);
    _COMPORT *GetComportDirect(_UWORD);
    _UWORD   GetNo_COMPORT(void);
    _COMPORT *GetDirect_COMPORT(_UWORD);
    _UDWORD  GetLoginDelayTime(void);
   #ifdef OK_DIAGNOSTIC
    _BOOL    Read(_AsciiPuffer*, const _ASCII*);
   #endif
    _COMSET  *GetComSet(void);
    _BOOL    TryToGetOnline(_COMPORT*, _UWORD);
    _BOOL    WatchOffline(void);
    void     WatchOnline(void);
    void     ResetWatchOnline(void);
    _CHAR*   MakeOfflineText(_CHAR*, _BOOL);
    void     ResetWatchOnlineTimeDelay(void);
   #ifdef UC_DELETE_REFLIST
    _UWORD   GetAllStationNumbersToSameComport(_UBYTE*, _UWORD);
   #endif
    _UWORD   GetStation(_UWORD);
    _BOOL    GetObserve(_UWORD);
    _BOOL    SetObserve(_UWORD, _BOOL);
    _BOOL    ChangeRemoteAddress(_UWORD, _INTERFACE, _UDWORD, _BOOL);
    _BOOL    IsComdefMultipleUsed(_UWORD);
    void     StartRefreshList(_COMPORT *cp, _UDWORD no, _UDWORD idx);
    void     StartRefreshList(void);
};

class _InterBoxList
{
  private:
		_UDWORD No;
    _Stack  *Ptr;

  public:
    _AsciiPuffer LseLabel;

    void   Init(void);                                                 // init
    void   Free(void);                                                 // free
    _BOOL  Load(_ASCII*);
    _BOOL  Chk_LSEFUNCT(_LSEFUNCT*);
		_BOOL  ChkFunction(_UDWORD);
		_Stack *Get(_UDWORD);
    _UWORD GetNo(void);
};

class _ScanBar
{
  private :
    _SCAN *Ptr;
    _UWORD No;
    _UWORD Pos;
   #ifdef UC_INVISIBLE_PROGRESS
    _BOOL  Visible;
    void   Draw(_SCAN *p); // todo: zu Klasse hinzufügen
    void   DoVisible(void);
   #endif
   
  public  :
    void   Init(void);
    void   Free(void);
    
    _DWORD Get(_UWORD i);
    void   Assert(const _ASCII *txt);
    void   Message(const _ASCII *txt);
    void   Run(_UWORD, _DWORD);
    _UWORD Add(const _ASCII*, _DWORD);
    _UWORD Add(const _ASCII *txt, _DWORD max, _COLOR col);
		void   End(_UWORD);
    void   SetPos(_UWORD);
    void   Reset(_UWORD i, _DWORD max, _BOOL changecolor);
    void   ColorSet(_UWORD i, _COLOR color);

};

class _MakeBMP
{
	private :
		_KOORD  PosX;
		_UDWORD MaxSize;
		_BOOL   State;
		_BOOL   Recalculate;
		_BMP    Bmp;

	public :

	 _BOOL  Start(_UWORD, _UWORD);
	 _BOOL  Add(_KOORD, _KOORD, _UWORD);
	 _BOOL  Add(_UWORD);
	 _BMP  *Stop(void);
};

class _IpcSettings
{
  public:
    void  Init(void);
    void  Free(void);
    _BOOL Save(void);
    _BOOL Load(void);
};

class _ScreenSaver
{
  private:
    _WORKSTATE   WorkState;
    _IMAGE       Image;
    _ROOM        Room;
    _UDWORD      OldTime;
    _SCREENSAVER ScreenSaver;
    _DOT         OldPos;
    _UWORD       Width, Height;

  public:
    void Init(void);
    void Free(void);
    void Ready(void);
    void Run(void);
};

#pragma pack (push, 1)
typedef struct { _UDWORD no;
                 _UDWORD used;
                 _UDWORD *ptr;
                 _UBYTE *bit;
               }
_MEM_INDEX;

typedef struct { _STATISTIC Filter;
                 _UDWORD    ActNo;
                 _TOOLSTATE ToolState;
                 _TOOLERROR ToolError;
                 _MEM_INDEX MemIndex;
                 _UDWORD    Version;
                 _UWORD     care_state;
                 _ASCII     dpne[_MAX_PATHLENGTH];
                 void      *Pthis;
                 _UDWORD    CrcOffset;
                 _UDWORD    Crc;
                 _UDWORD    EndHeaderOffset;
                 _UDWORD    ClassRevision; // SA37331
                 _BOOL      CrcOk; 
                 _File      File; // muss am ende der structur sein (lasal !!)
               }
_TOOLDATA;
#pragma pack (pop)

class _ToolCatalogue
{
  private :
    _TOOLDATA ToolData;
    _UDWORD   CrcOffset;

    _BOOL SetError(_TOOLERROR);
    _BOOL SetError(_TOOLERROR, _RESULT*, _VARIABLE*, _VARLABEL*);

    _BOOL AlreadyInserted(_UDWORD);
    _BOOL AddEntry(_UDWORD);
    _BOOL MakeMemIndex(void);

    _BOOL SaveStart(_ASCII*);
    _BOOL SaveServer(_UDWORD);
    _BOOL SaveRun(void);
    _BOOL SaveEnd(void);

    _BOOL SingleLoad(_ASCII*);
    _BOOL LoadStart(_ASCII*);
    _BOOL LoadHeader(void);
    _BOOL LoadRun(void);
    _BOOL LoadEnd(void);
    _BOOL IdleRun(void);
    _BOOL WriteCrc(_UDWORD);
    _BOOL CheckKey(_CRC32); 

  public :
    void       Init(void);
    void       Free(void);
    _BOOL      Run(void);
    _BOOL      Teach(const _ASCII*, _TOOLSTATE, _STATISTIC*);
    _TOOLDATA* Get_TOOLDATA(void);
};

class _Hotkey
{
  private:
    _MEM_KEY MemKey;

    _BOOL Read(_File*);

  public:
    void   Init(void);
    void   Free(void);
    _BOOL  Load(_ASCII*);
    _UWORD GetNo();
    _KEY*  Get(_UWORD);
    _KEY*  FindKey(_UWORD);
    void   SetKeyMode(void);
};

#pragma pack (push, 1)
typedef struct { _UDWORD svrno;
                 _IVALUE oldvalue;
                 void    *ptrthis;
                 void    *ptrfkt;
               }
_SVRWATCH;
#pragma pack (pop)

class _ServerWatch
{
  private:
    _SVRWATCH  *Ptr;
    _UDWORD     No, MaxNo, AktNo;
    _VARLABEL   VarLabel;

    _UDWORD  Seek(_UDWORD);

  public:
    void     Init(void);
    void     Free(void);

    _BOOL    Add(_UDWORD, void*, void*);
    _BOOL    Del(_UDWORD);
    void     Run(void);
};

#ifdef UC_THEME
class _Theme
{
  private :
    _UBYTE     GuidPost[256];
    _MEM_THEME MemTheme;

    _BOOL  Read(_File*);
    void   MakeGuidPost(void);

  public:
    _BOOL   IsAction;

   #ifdef UC_IPC
    _UDWORD MemSize(_MEMSIZE*);
   #endif
    _UDWORD GetNo(void);
    void    Init(void);
    void    Free(void);
    _THEME *Get(_UWORD);
    _THEME *GetByIndex(_UWORD);
    _BOOL   Load(_ASCII*);

};
#endif


// OS variables *************************************************************
extern "C" _UDWORD    RTOSVERSION;
extern "C" _UDWORD    WhoAmI;


// global objects and variable **********************************************

#ifdef UC_FIXMEMORY
 #define FIXMEMORYNO          4 // anzahl der fixmemoblöcke, 3 sind fürs themenzeichnen notwendig
 #define FIXMEMORYSIZE   524288 // grösse eines fixmemoblocks 2^19
 extern _UBYTE *FixMemory;
 extern _UDWORD FixMemoryIndex;
#endif

//==========================================================================================
//defines for CPU Types
//#define LSL_CPUMASK       0x000000FF
#define LSL_ISINTEL       0x00000000
#define LSL_ISPPC         0x00000001
#define LSL_ISARM         0x00000002

//defines for supported features
//#define LSL_NO_DATABP     0x00010000
//#define LSL_NO_FPU        0x00020000
#ifndef LSL_OS_SALAMANDER
 #define LSL_OS_SALAMANDER 0x00040000
#endif
#ifndef LSL_EQUIPMENT_VALID
 #define LSL_EQUIPMENT_VALID 0x80000000
#endif
extern "C" _UDWORD      SigmatekOpSysInfo;
#define OpSysIsSalamander() ((SigmatekOpSysInfo & (LSL_OS_SALAMANDER|LSL_EQUIPMENT_VALID)) == (LSL_OS_SALAMANDER|LSL_EQUIPMENT_VALID)) 
#define CPUArchIsARM()      ((SigmatekOpSysInfo & (LSL_ISARM|LSL_EQUIPMENT_VALID)) == (LSL_ISARM|LSL_EQUIPMENT_VALID)) 

//==========================================================================================

extern _Keyboard       Keyboard;
extern _Project        Project;
extern _Language       Language;
extern _SetUp          SetUp;
extern _ScreenSaver    ScreenSaver;
extern _ChkBit         ChkBit;
extern _VarList        VarList;
//extern _TextPuffer     AtxList;
//extern _TextPuffer     StxList;
//extern _TextPuffer     MtxList;
//extern _TextPuffer     Zt1List;
//extern _TextPuffer     Zt2List;
//extern _TextPuffer     Zt3List;
//extern _TextPuffer     TitList;
//extern _TextPuffer     UnitList;
extern _TextList       TextList;
extern _ImageList      ImageList;
extern _Font           Font;
extern _PicMemo        PicMemo;
#ifdef UC_IPC
extern _Gfx            Gfx;
#endif
#ifdef OK_PICMEMO
extern _MemoPool       MemoPool;
#endif

extern _Window        Root;
extern _ObjectList    ObjectList;
extern _Batch         Batch;
extern _InterBoxList  InterBoxList;
extern _ScanBar       ScanBar;
extern _MenuList      MenuList;
extern _Alarm         Alarm;
extern _Sequencer     Sequencer;
extern _Scheme        ColorScheme;
extern _Scheme        UnitScheme;
extern _Scheme        ServerScheme;
extern _Scheme        ImageScheme;
extern _FontScheme    FontScheme;
extern _Scheme        ScreenScheme;
extern _Scheme        StateScheme;
extern _TextScheme    TextScheme;
extern _TermAlarm     TermAlarm;
extern _Scheme        ObjectScheme;
extern _Palette       Palette;
extern _ToolCatalogue ToolCatalogue;
extern _Hotkey        Hotkey;
extern IKEYPAD        *IKeyPad;
extern _MessageBox    MessageBox;
#ifdef UC_BUBBLE_HELP
extern _NewBubbleHelp NewBubbleHelp;
#endif
#ifdef UC_THEME
extern _Theme         Theme;
#endif
#ifdef XUSER_KM
extern _AllSoftkeys   AllSoftkeys;
#endif

#ifdef UC_DIRECTORY
extern _Directory     Directory;
#endif
#ifdef OK_VALIDATE
extern _Validate      Validate;
#endif
#ifdef OK_OFFSCREEN
extern _NEWSCREEN     OffScreen;
extern _NEWSCREEN     OnScreen;
extern _BOOL          UseOffscreen;
#endif
extern _SYSTEM       System;
extern _MEMORYIDX    MemoryIdx;
extern _IpcSettings  IpcSettings;
extern void          *LseThisPointer;
extern _UWORD        DrawBackGround;
extern _SYSERROR     SystemMessage;
extern _UWORD        DrawXPicture;
extern _UWORD        ReCalculate_IoId;
extern _PT_LOOKUPEMBEDDED LookUpEmbeddedPointer;
extern _UBYTE        MyDefaultFont[];
#ifdef UC_IPC
extern _UBYTE        MyStandardFont[];
#endif
extern _UDWORD       ServerToEdit;
extern volatile _UDWORD       GlobalChangeIrq;
extern _UDWORD       GlobalChangeIrqMemo;
extern _UDWORD       GlobalChangeSys;
extern _UDWORD       TextRenderEngine;
extern _FVOID_PT_PT_PT    MethodLseWindowRun;
extern _FVOID_PT          MethodLseProjectRun;
extern _FBOOL_PT_PT_BOOL  MethodLseRecalculate;
extern _FBOOL_PT          MethodLseAccessButton;
extern _FVOID_DWORD_DWORD MethodLseUserCall;
extern _FBOOL_PT          MethodLseAccessInput;
extern _FBOOL_DWORD       MethodLseCheckEnable;
extern _FBOOL_PT_UWORD_FOUNDHID_BOOL MethodLseTryToAccess;
extern _FVOID_FOUNDHID    MethodLseFoundSomethingByHid;


extern _JpgFunctLoad   JpgFunctLoad; // pointer to function jpgLoad
extern _JpgFunctSave   JpgFunctSave; // pointer to function jpgSave
extern _FBOOL_UWORD_PT CallUserOverloadPtr;
#ifdef ZOOM_KM
extern _UDWORD       DoZoomKM;
#endif
#ifdef OK_DIAGNOSTIC
 #ifdef OK_SUBLIST
  extern _MEM_VKLIST   DiagVarList;
 #endif
#endif
extern _BOOL          SuperMode;
#ifdef USER_DEMAG
extern _BOOL          OverloadAbsolute;
#endif

extern _ServerWatch   ServerWatch;

#ifdef UC_DELETE_REFLIST
extern _BOOL          ChkForDeleteRefreshlist;
#endif

extern _UDWORD        MouseHideState;
extern _FLIPSCREEN    FlipScreen;

extern _LSE_TIME      LseTime;
extern _MULTIKEYBOARD MultiKeyboard;

#ifdef USER_BILLION
extern void *BillionPThis;
#endif

#ifdef NINETEEN
extern _Picture DashBoard;
#endif


// init, free Makros

extern _SCHEME           data_SCHEME;
extern _VARINFO          data_VARINFO;
extern _LINKSCHEME       data_LINKSCHEME;
extern _CHKBIT           data_CHKBIT;
extern _GFUNCT           data_GFUNCT;
extern _LSEVARINFO       data_LSEVARINFO;
extern _VARLABEL         data_VARLABEL;
extern _VARIABLE         data_VARIABLE;
extern _RESULT           data_RESULT;
extern _STACK_LINE       data_STACK_LINE;
extern _BMP              data_BMP;
extern _MYSCHEME         data_MYSCHEME;
extern _EVENT            data_EVENT;
extern _ANYPARA          data_ANYPARA;
extern _IMAGE            data_IMAGE;
extern _STACK            data_STACK;
extern _LSEFUNCT         data_LSEFUNCT;
extern _IOINFO           data_IOINFO;
extern _IO               data_IO;
extern _INPUT            data_INPUT;


#define init_STACK_Makro(_p)           (*(_p) = data_STACK)
#define init_IMAGE_Makro(_p)           (*(_p) = data_IMAGE)
#define init_LSEFUNCT_Makro(_p)        (*(_p) = data_LSEFUNCT)
#define init_IOINFO_Makro(_p)          (*(_p) = data_IOINFO)
#define init_IO_Makro(_p)              (*(_p) = data_IO)
#define init_INPUT_Makro(_p)           (*(_p) = data_INPUT)
#define init_VARINFO_Makro(_p)         (*(_p) = data_VARINFO)
#define init_SCHEME_Makro(_p)          (*(_p) = data_SCHEME)
#define init_LINKSCHEME_Makro(_p)      (*(_p) = data_LINKSCHEME)
#define init_CHKBIT_Makro(_p)          (*(_p) = data_CHKBIT)
#define init_GFUNCT_Makro(_p)          (*(_p) = data_GFUNCT)
#define init_LSEVARINFO_Makro(_p)      (*(_p) = data_LSEVARINFO)
#define init_ANYPARA_Makro(_p)         (*(_p) = data_ANYPARA)
#define init_VARLABEL_Makro(_p)        (*(_p) = data_VARLABEL)
#define init_VARIABLE_Makro(_p)        (*(_p) = data_VARIABLE)
#define init_RESULT_Makro(_p)          (*(_p) = data_RESULT)
#define init_STACK_LINE_Makro(_p)      (*(_p) = data_STACK_LINE)
#define init_EVENT_Makro(_p)           (*(_p) = data_EVENT)
#define init_BMP_Makro(_p)             (*(_p) = data_BMP)
#define init_MYSCHEME_Makro(_p)        (*(_p) = data_MYSCHEME)
#define timediff(_p)                   (((_UDWORD)get_abstime()) - ((_UDWORD)(_p)))
#define chk_LASALID_Makro(_p)          (((_p)->id != DEFLASALID)? _TRUE:_FALSE)
#define init_TIME_Makro(_p)            {(_p)->hour = 0; (_p)->minute = 0; (_p)->second = 0; }
#define init_DATE_Makro(_p)            {(_p)->day = 0; (_p)->month = 0; (_p)->year = 0; (_p)->dayofweek = 0; }
#define init_ROOM_Makro(_p)            {(_p)->xy1.x = 0; (_p)->xy1.y = 0; (_p)->xy2.x = 0; (_p)->xy2.y = 0; }
#define init_ANYTHING_Makro(_p)        {(_p)->list = DEFLST; (_p)->no = 0; }
#define init_VARSYMBOL_Makro(_p)       ((_p)->scope = DEFSCOPE)
#define init_USERTEXT_Makro(_p)        ((_p)->ptr = NULL)
#define get_commasign_Makro()          (System.commasign)
#define get_americandateformat_Makro() (System.americandateformat)
#define get_americantimeformat_Makro() (System.americantimeformat)
#define get_delay_screensave_Makro()   (System.delaytime_screensave)
#define get_actual_language_Makro()    (System.act_language)
#define MakeTrueColor_Makro(cr, cg, cb)  (((((_UWORD)(cr)) << 8) & 0xF800) | ((((_UWORD)(cg)) << 3) & 0x07E0) | ((((_UWORD)(cb)) >> 3) & 0x001F))



// INTERFACE TO LASAL *******************************************************

// just that compile is posible

#define STD_METHOD_READ         0
#define STD_METHOD_WRITE        1

#define MUTEX_STACK                        0
#define MUTEX_ALARM                        1
#define MUTEX_TASK2                        2
#define MUTEX_ReallocREFLIST               3
#define MUTEX_StartREFLIST                 4

#define PROFILE_START 0x00000000
#define PROFILE_END   0x00000100

#define CLASS_LSE                          0x00000000
#define CLASS_GLOBAL                       0x00010000
#define CLASS_SCREEN                       0x00020000
#define CLASS_MYIO                         0x00030000
#define CLASS_TOOLCAT                      0x00040000

#define PROFILE0_LSE_CYCLE                 0
#define PROFILE0_DRAWPICTURE               1

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
#define METHOD_LSE_STARTEVENT             33
#define METHOD_LSE_STOPEVENT              34
#define METHOD_LSE_GETEVENT               35
#define METHOD_LSE_SETEVENT               36
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
#define METHOD_LSE_PROGRESS               55


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
#define METHOD_MYIO_SETDATA                  15
#define METHOD_MYIO_INCDECDATA               16


#define TOOLCAT_LOAD_START                    0
#define TOOLCAT_LOAD_RUN                      1
#define TOOLCAT_LOAD_END                      2
#define TOOLCAT_SAVE_START                    3
#define TOOLCAT_SAVE_RUN                      4
#define TOOLCAT_SAVE_END                      5
#define TOOLCAT_ERROR_OCCURED                 6
#define TOOLCAT_IDLE_RUN                      7
#define TOOLCAT_SAVE_LABEL                    8
#define TOOLCAT_LOAD_LABEL                    9
#define TOOLCAT_LOAD_LABEL_EX                10

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

#define GFXERROR_NONE               0 // kein fehler
#define GFXERROR_CANTOPEN           1 // file kann nicht geöffnet werden oder nicht vorhanden
#define GFXERROR_TOOLESSMEMORY      2 // zu wenig ram (image zu gross)
#define GFXERROR_USERBREAK          3 // anwender hat unterbrochen
#define GFXERROR_INVALIDFILE        4 // file kaputt
#define GFXERROR_UNKNOWNFILE        5 // unbekanntes grafikformat
#define GFXERROR_UNKNOWN       0xFFFF // unbekannter fehler

// LSE.ST *******************************************************************
extern "C" _UDWORD get_loaderversion(void);
extern "C" void    SetColorPalette(_UBYTE*);
extern "C" _DWORD  zoom_angle(_DWORD, _DWORD, _DWORD);

// T_3D.CPP *****************************************************************
extern "C" void  matrize(_REAL*, _REAL*, _REAL*, _REAL*);
extern "C" void  rotate_DOT3D(_DOT3D*, _WORD, _WORD, _WORD);
extern "C" void  rotate_DOT(_DOT*, _WORD);
#ifdef UC_GRAPHIC_STUDIO
extern "C" void draw_3DSHAPE(_KOORD, _KOORD, _3DSHAPE*);
extern "C" _BOOL polygon_to_3DSHAPE(_3DSHAPE*, _DOT*, _UWORD, _UWORD, _COLOR, _COLOR);
extern "C" _BOOL make_PICTUREMASK2D(_PICTUREMASK2D*, _UWORD);
extern "C" void  rotate_PICTUREMASK2D(_PICTUREMASK2D*, _WORD);
extern "C" void  out_PICTUREMASK2D(_PICTUREMASK2D*, _DOT*, _ZOOM*);
extern "C" void  add_PICTUREMASK2D(_PICTUREMASK2D*, _KOORD, _KOORD);
extern "C" void  init_PICTUREMASK2D(_PICTUREMASK2D*);
extern "C" void  rotate_SHAPE(_SHAPE*, _WORD, _WORD, _WORD);
extern "C" void  rotate_3DSHAPE(_3DSHAPE*, _WORD, _WORD, _WORD);
#endif

// T_ALARM.CPP **************************************************************
#ifdef UC_DO_ALARM
extern     void  init_alarmlist(void);
extern     void  free_alarmlist(void);
extern     _BOOL alarm_call(_MENU*);
#endif
extern "C" _ALARM *Lse_GetAlarmPointer(void);
extern "C" _BOOL   store_ALACELL(_ALACELL*);
extern "C" _MEM_SINGLEALARM *get_MEM_SINGLEALARM(_BOOL);
extern "C" _BOOL   alarm_save(void);
extern "C" void    out_standard_alarmline(_SINGLEALARM*, _ROOM*, _COLOR, _UWORD, _SCROLL *ps=NULL);
extern "C" _CHAR  *create_alarmtext(_SINGLEALARM*, _BOOL, _COLOR*);
extern "C" _BOOL   acknowledge_alarm(_MEM_SINGLEALARM*, _DWORD);
extern "C" _BOOL   delete_alarm(_MEM_SINGLEALARM*, _DWORD);
extern "C" _BOOL   add_SINGLEALARM(_MEM_SINGLEALARM*, _SINGLEALARM*);
extern "C" _UDWORD active_MEM_SINGLEALARM(_MEM_SINGLEALARM*);
extern "C" _BOOL   CheckLostAlarm(_ALACELL*);
extern "C" _BOOL   acknowledge_alarm_station(_UWORD, _BOOL);
extern "C" _BOOL   delete_alarm_station(_UWORD, _BOOL);
extern "C" _SINGLE_ALARM *LSE_GetAlarmconfig(_UWORD alaconfigno);
extern "C" _BOOL   UpdateAlarmHistoryRecord(_SINGLEALARM *ptr);

// T_BATCH.CPP ****************************************************************
extern "C" _COMSET *Lse_GetComSet(void);
extern "C" _COMPORT *Get_COMPORT(_UWORD);
extern "C" _UWORD  GetStartPicture(void);
extern "C" void    MakeOfflineText(_CHAR*, _BOOL);
extern "C" _BOOL   WatchOffline(void);

// T_BMP.CPP ****************************************************************
extern "C" _BOOL  save_bmo(_ASCII*, _KOORD, _KOORD, _KOORD, _KOORD);
extern "C" _BOOL  load_bmo(_BMP*, _ASCII*, _UWORD*);
extern "C" _BOOL  save_bmp(_ASCII*, _KOORD, _KOORD, _KOORD, _KOORD, _BOOL);
extern "C" _BOOL  load_bmp(_BMP*, _ASCII*, _UWORD*, _FUDWORD_UDWORD_UDWORD);

// T_COM.CPP ****************************************************************
extern _UWORD  add_PLC(_LSEVARINFO*, _BOOL, _BOOL);      // add plcvalue
extern _BOOL   update_PLC(_VARINFO*);
extern _BOOL   wait_for_PLC(_UWORD, _VARINFO*, _COMPORT*);   // wait for plc-irq (senddata)
extern _BOOL   wait_for_PLC_global(_UWORD, _VARINFO*, _VARLABEL*);
extern void*   get_PLC(_VARINFO*, _UWORD);    // get pointer to data from plc
extern _BOOL   send_PLC(void*, _VARINFO*);         // send data direct to plc
extern void    simulate_PLC(void);
extern _BOOL   config_SINGLE_ALARM(_SINGLE_ALARM*, _BOOL);
extern _BOOL   get_direct_PLC_sub(void*, _LASALID*, _COMPORT*, _UWORD, _SVRTYP);
extern _BOOL   send_command0(_UDWORD, _UWORD, _UWORD, _DWORD*);
extern _BOOL   login_PLC_Sub(_COMPORT*, _UWORD);
extern _BOOL   get_global_data(void*, _VARINFO*);
#ifdef UC_NEW_ALARMSYSTEM
extern _BOOL   setupAlarmObject(_COMPORT*);
extern void    getAlarms(_COMPORT*, _MEM_SINGLEALARM*, _MEM_SINGLEALARM*);
extern _BOOL   quitAlarm(_UWORD, _UDWORD);
extern _BOOL   synchronize_datim(_COMPORT*);
#endif
extern "C" void     new_PLC(void);                     // reset all refreshlist
extern "C" _BOOL    send_command1(_ASCII*, _UWORD, _UWORD, _UWORD, _DWORD*);
extern "C" _BOOL    send_command2(_ASCII*, _UWORD, _UWORD, _UWORD, _UBYTE*, _UDWORD);
extern "C" _BOOL    send_command3(_ASCII*, _UWORD, _UWORD, _UWORD, _DWORD*, void**);
extern "C" _BOOL    set_dat(_RESULT*, _UDWORD);
extern "C" _BOOL    get_dat(_RESULT*, _UDWORD);
extern "C" _UWORD   xset_dat(_RESULT*, _UDWORD, _UWORD);
extern "C" _UWORD   xget_dat(_RESULT*, _UDWORD, _UWORD);
extern "C" _BOOL    get_minmax(_RESULT*, _VARIABLE*, _UWORD);
extern "C" _COMDEF* login_PLC(_UWORD);
extern "C" void     irq_PLC(void*, _UBYTE*);
extern "C" void     set_offlinemode(_DWORD);
extern "C" _BOOL    is_online(_COMDEF*);
extern "C" _BOOL    ChangeRemoteAddress(_UWORD, _INTERFACE, _UDWORD, _BOOL);
extern     void     SoftReset(void);
extern     _BOOL    GetAddressListObj(_ASCII *label, _UWORD length, _LASALID *p, _UWORD no, _COMDEF *pc);
extern     _BOOL    get_lasal_id(_LASALID *p, _ASCII *label, _LSEVARINFO *pi);
extern     _UDWORD  PlcGetLasalId(_COMDEF *pcd, _ASCII *label, _LSEVARINFO *plvi);
extern     _DWORD   TxCommandIntern(_UDWORD bef, _UDWORD len, void *pdata, _COMDEF *pcd, void **presult, _SVRTYP svrtyp);
extern "C" void     SetTimeDelayOfflineWindow(_UDWORD time_ms);

// T_COMBOX.CPP *************************************************************
extern     void      free_COMBOBOX(_COMBOBOX*);
extern     void      init_COMBOBOX(_COMBOBOX*);
extern "C" _COMBOBOX *Make_COMBOBOX(_COLOR colback, _COLOR colfront, _UDWORD lineheight, _ATTRIB alignment, _FRAMETYPE frametype);
extern "C" _BOOL     Add_COMBOBOX(_COMBOBOX *p, _DWORD value, void *txt, _UDWORD chr_size);
extern     _CHAR     *GetText_COMBOBOX(_COMBOBOX *p, _DWORD value);
extern     _BOOL     DoEditorNeeds_COMBOBOX(_INPUT *pi);

// T_COMID.CPP **************************************************************
extern     _BOOL    GetIDFast(_UWORD sb, _BOOL bar);
extern     void     SendVisuReady();
extern     _UDWORD  GetLoaderVersion(_COMPORT *pcp);

// T_COMIDFAST.CPP **********************************************************
#ifdef UC_GETLASALID_DATALINK
           _BOOL    GetLasalIdUsingDataLink(_UWORD sb, _BOOL bar);
           void     FixLasalIdUsingDataLink(void);
#endif

// T_COMPLR.CPP *************************************************************
extern _BOOL   overload_COMPCODE(_COMPCODE*, _UDWORD);
#ifdef OK_DIAGNOSTIC
extern void    COMPCODE_to_text(_ASCII*, _COMPCODE*, _UDWORD);
#endif


// T_CONVERT.CPP ************************************************************
extern "C"  void  move_pREAL_REAL(_REAL *pdst, _REAL rval);
extern "C"  void  move_pREAL_DWORD(_REAL *pdst, _DWORD dval);
extern "C"  void  move_pREAL_pREAL(_REAL *pdst, _REAL *psrc);
extern     _REAL  get_pREAL(_REAL *psrc);

// T_COPY.CPP ***************************************************************
extern      void  copy_RECTANGLE(_BOOL*, _RECTANGLE*, _RECTANGLE*);
extern      void  copy_CIRCLE(_BOOL*, _CIRCLE*, _CIRCLE*);
extern      void  copy_POLYGON(_BOOL*, _POLYGON*, _POLYGON*);
extern     _BOOL  copy_CHKBIT(_BOOL*, _CHKBIT*, _CHKBIT*);
extern     _BOOL  copy_ANYPARA(_BOOL*, _ANYPARA*, _ANYPARA*);
extern     _BOOL  copy_LSE_VARINFO(_BOOL*, _VARINFO*, _LSEVARINFO*, _BOOL);
extern     _BOOL  copy_SOFTFLOOR(_BOOL*, _SOFTFLOOR*, _SOFTFLOOR*);
extern     _BOOL  copy_SINGLE_ALARM(_BOOL*, _SINGLE_ALARM*, _SINGLE_ALARM*);
extern     _BOOL  copy_TXTSCHEME(_BOOL*, _TXTSCHEME*, _TXTSCHEME*);
extern     _BOOL  copy_COMPCODE(_BOOL*, _COMPCODE*, _COMPCODE*);
extern     _BOOL  load_object(_SubPicture **, _UWORD);
extern     void   copy_INPUT_BUTTON(_INPUT*, _BUTTON*);
extern     void   kill_backgroundimage(_PICTUREMASK*);
extern     void   set_buttonstate(_PICTUREMASK*, _BOOL);
extern     _BOOL  copy_MEM_ANYPARA(_BOOL*, _MEM_ANYPARA*, _MEM_ANYPARA*);
extern     _BOOL  copy_MEM_BOOL(_MEM_BOOL*, _MEM_BOOL*);
extern     _BOOL  merge_STACK(_STACK*, _STACK*);
extern     _BOOL  copy_LSEMENU(_LSEMENU*, _LSEMENU*);
extern     _BOOL  copy_DOT(_DOT*, _DOT*);
extern     _BOOL  copy_DOT3D(_DOT3D*, _DOT3D*);
extern     _BOOL  copy_SHAPE(_SHAPE*, _SHAPE*);
extern     _BOOL  copy_3DSHAPE(_3DSHAPE*, _3DSHAPE*);
extern      void  copy_THEURGE(_BOOL*, _THEURGE*, _THEURGE*);
extern      void  copy_MYSCHEME(_BOOL*, _MYSCHEME*, _MYSCHEME*);
extern      void  copy_VARLABEL(_VARLABEL*, _VARLABEL*);
extern      void  copy_VARLABEL_IADDRESS(_VARLABEL*, _IADDRESS);
extern "C" _BOOL  copy_SCHEME(_SCHEME*, _SCHEME*);
extern "C" _BOOL  copy_STACK(_STACK*, _STACK*);
extern "C" _BOOL  copy_BMP(_BMP*, _BMP*);
extern "C" _BOOL  copy_PICTURE(_PICTURE*, _PICTURE*);
extern "C" void   copy_INPUT(_BOOL*, _INPUT*, _INPUT*);
extern "C" _BOOL  copy_MENUINFO(_MENUINFO*, _MENUINFO*);
extern "C" _BOOL  copy_MEM_VARLABEL(_MEM_VARLABEL*, _MEM_VARLABEL*);
extern "C" _BOOL  copy_LINEINFO(_LINEINFO*, _LINEINFO*);
extern "C" _BOOL  copy_MEM_LINEINFO(_MEM_LINEINFO*, _MEM_LINEINFO*);
extern "C" _BOOL  copy_MEM_FONT(_MEM_FONT*, _MEM_FONT*);
extern "C" _BOOL  copy_MEM_ATTRIB(_MEM_ATTRIB*, _MEM_ATTRIB*);
extern "C" _BOOL  copy_MEM_COLOR(_BOOL*, _MEM_COLOR*, _MEM_COLOR*);
extern "C" _BOOL  copy_MEM_ANYTHING(_BOOL*, _MEM_ANYTHING*, _MEM_ANYTHING*);
extern "C" _BOOL  copy_MEM_LINKSCHEME(_BOOL*, _MEM_LINKSCHEME*, _MEM_LINKSCHEME*);
extern "C" _BOOL  copy_DYNCOLOR(_BOOL*, _DYNCOLOR*, _DYNCOLOR*);
extern "C" _BOOL  copy_MEM_DYNCOLOR(_BOOL*, _MEM_DYNCOLOR*, _MEM_DYNCOLOR*);
extern "C" _BOOL  copy_LINKVL(_BOOL*, _LINKVL*, _LINKVL*);
extern     _BOOL  copy_SINGLESOFT(_BOOL*, _SINGLESOFT*, _SINGLESOFT*);
extern     void   copy_KARU(_KARU*, _KARU*);
extern     void   merge_ASCII_BUFFER(_BOOL *retcode, _ASCII_BUFFER *d, _ASCII_BUFFER *s);
extern     void   copy_LINKSCHEME(_BOOL *retcode, _LINKSCHEME *d, _LINKSCHEME *s);


// T_CRC.CPP ****************************************************************
extern "C" _CRC32  MakeCrc32(_CRC32, void*, _UDWORD);
extern "C" void    DeEnCode(_UBYTE*, _UDWORD, _BOOL);
extern "C" _UDWORD Random(_UDWORD);

// T_CURSOR.CPP *************************************************************
extern     void    draw_button_cursor(_ROOM, _COLOR, _ATTRIB);
extern "C" _Cursor *Cursor_Get(void);
extern "C" void    Cursor_Hide(_Cursor*);
extern "C" void    Cursor_Set(_Cursor*, _ROOM*, _COLOR, _UWORD);
extern "C" void    Cursor_End(_Cursor*);
extern "C" void    Cursor_Push(_Cursor*);
extern "C" void    Cursor_Pop(_Cursor*);

// T_DIAG.CPP ***************************************************************
extern "C"  void   draw_heap(_ROOM*);
extern "C" _BOOL   check_heap(_BOOL);
#ifdef OK_DIAGNOSTIC
extern     _BOOL   Diagnostic(_MENU*, _VARLABEL*, _UWORD);
extern     _UDWORD get_heap_crc(void);
#endif

// T_DIRECT.CPP *************************************************************
#ifdef UC_DIRECTORY
extern "C" void    NewDirectorySetup(_FILE_STATE, _ASCII*, _FBOOL_PT_PT, void*, _DIRECTORYSETUP*);
extern "C" void    DirectorySetup(_FILE_STATE, _ASCII*, _FBOOL_PT_PT, void*);
extern "C" _BOOL   DirectoryCall(_WORKSTATE, _EVENT*, _BOOL, _ROOM*);
#endif

// T_EDIT.CPP ***************************************************************
extern     void    init_cpyedit(void);
extern "C" _BOOL   run_CHREDIT(_CHREDIT*, _UWORD, _BOOL, _BOOL, _BOOL);
extern "C" _BOOL   ReOpenEditor(void);
extern "C" void    EditorOutChanged(_Picture*, _EDITOR*);
extern     _UWORD  GetDayOfMonth(_UDWORD, _UDWORD);
extern "C" _BOOL   IsEditorActive(void);
extern "C" void    incdec_time(_TIME*, _DWORD, _UWORD);
extern "C" void    incdec_date(_DATE*, _DWORD);
extern "C" void    set_MULTIKEYBOARD(_UWORD anzahl, _UWORD *tab, _UWORD delaytime); 
extern     _DOIT   CallMethod_MyIoSetValue(_RESULT *presult, _VARLABEL *pvl, void *pmyio);

// EXOONLINE.ST *************************************************************
#ifdef UC_EXOONLINE
#define IsExoOnline(p1) (*(_UDWORD*)&((p1)->handle) == UC_EXOONLINE)
extern "C" _DOIT   ExoLogin(_COMDEF *pcd);
extern "C" _BOOL   ExoLogout(_COMDEF *pcd);
extern "C" _BOOL   ExoChkOnline(_COMDEF *pcd);
extern "C" _BOOL   ExoPing(_COMDEF *pcd);
extern "C" _BOOL   ExoRun(void);
extern "C" _UDWORD ExoGetDescriptorCrc(_COMDEF *pcd);
extern "C" _UDWORD ExoGetLasalId(_UDWORD *pexoinfo, _COMDEF *pcd, _ASCII *label); 
extern "C" _BOOL   ExoAddRefList(_COMDEF *pcd, _LSLCOMREGDAT *pr, _UDWORD exoinfo);
extern "C" _BOOL   ExoStartRefList(_COMDEF *pcd);
extern "C" _BOOL   ExoClearRefList(_COMDEF *pcd);
extern "C" _UDWORD ExoGetLoaderVersion(_COMDEF *pcp);
extern "C" _BOOL   ExoGetValue(_COMDEF *pcp, void *pdst, _UDWORD lid, _UDWORD exoinfo, _UDWORD bytesize);
extern "C" _BOOL   ExoSetValue(_COMDEF *pcp, void *psrc, _UDWORD lid, _UDWORD exoinfo, _UDWORD bytesize);
extern "C" _BOOL   ExoGetString(_COMDEF *pcp, _ASCII *pdst, _UDWORD lid, _UDWORD exoinfo, _UDWORD bytesize);
extern "C" _BOOL   ExoSetString(_COMDEF *pcp, _ASCII *psrc, _UDWORD lid, _UDWORD exoinfo, _UDWORD bytesize);
extern "C" _DWORD  ExoTxCommand(_UDWORD bef, _UDWORD len, void *pdata, _COMDEF *pcd, void **presult, _SVRTYP svrtyp);
#endif

// T_EXPLOR.CPP *************************************************************
extern "C" _BOOL   filexplo_scanmarked(_FILEXPLORER *pex, _FBOOL_PT_PT_PT_SELIS_UDWORD pfkt, void *puser1, void *puser2);
extern "C" _BOOL   filexplo_update(_FILEXPLORER *pex, _POSITION *ppos);
extern "C" _BOOL   filexplo_rename(_FILEXPLORER *pex, _ASCII *oldname, _ASCII *newname, _DWORD linepos);
extern "C" _UDWORD filexplo_getattrib(_FILEXPLORER *pex, _DWORD pos);
extern "C" _BOOL   filexplo_xctrlc(_FILEXPLORER *pex);
extern "C" _BOOL   filexplo_ctrlv(_FILEXPLORER *pex, _ASCII *dst);
extern "C" _BOOL   filexplo_ctrlc(_FILEXPLORER *pex, _DWORD line);
extern "C" _BOOL   CheckCreateDirectory(_ASCII *drivepath);
extern "C" _FILEXPLORER* filexplo_start(_POSITION *ppos, _ASCII *rootpath, _ASCII *exclude_drives);
extern "C" _FILEXPLORER *filexplo_end(_FILEXPLORER *pex);
extern "C" void    filexplo_line(_FILEXPLORER *pex, _DWORD line, _ROOM *r, _COLOR col);
extern "C" _BOOL   filexplo_enter(_FILEXPLORER *pex, _POSITION *ppos);
extern "C" _ASCII  *filexplo_path(_FILEXPLORER *pex, _SELIS *pselis, _DWORD line, _BOOL rootlock);
extern "C" _BOOL   filexplo_delete(_FILEXPLORER *pex, _POSITION *ppos);
extern "C" _BOOL   filexplo_delete_treeentry(_FILEXPLORER *pex, _POSITION *ppos);
extern "C" _BOOL   filexplo_is_marked(_FILEXPLORER *pex, _DWORD line);
extern "C" _BOOL   filexplo_can_open(_FILEXPLORER *pex, _DWORD line);
extern "C" _BOOL   filexplo_can_close(_FILEXPLORER *pex, _DWORD line);
extern "C" void    filexplo_make_user_path(_ASCII *dst, _FILEXPLORER *pex, _ASCII *src);
extern     _BOOL   directory_copy(_ASCII *dst, _ASCII *src, _ASCII *filefilter);
extern     _BOOL   filexplo_delete_file(_ASCII *dpne);
extern     _BOOL   filexplo_delete_directory(_ASCII *dp);

// T_FILE.CPP ***************************************************************
extern "C" _BOOL   Lse_File_GetError(_File*);
extern "C" void    Lse_File_SetError(_File*, _FILE_ERROR);
extern "C" _BOOL   Lse_File_Open(_File*, _FILE_STATE, _ASCII*);
extern "C" _BOOL   Lse_File_Close(_File*);
extern "C" _DWORD  Lse_File_Tell(_File*);
extern "C" _BOOL   Lse_File_Seek(_File*, _DWORD, _UWORD);
extern "C" _DWORD  Lse_File_Length(_File*);
extern "C" _BOOL   Lse_File_Read(_File*, void*, _UDWORD);
extern "C" _BOOL   Lse_File_Write(_File*, void*, _UDWORD);
extern "C" _BOOL   Lse_File_Read_CRLF(_File*, _ASCII*, _UWORD);
extern "C" _BOOL   Lse_File_ReadCrLf(_File*, void*, _UWORD, _UWORD);
extern "C" _FILE_ERROR Lse_File_GetErrorState(_File*);
extern "C" _BOOL   Lse_File_Remove(_File*, _ASCII*);
extern "C" _BOOL   Lse_File_Decode(_File*, _UDWORD);
extern "C" _BOOL   Lse_File_Encode(_File*, _UDWORD);
#ifdef UC_IPC
extern "C" _BOOL   Write_BMP(const _ASCII*, _BMP*);
extern "C" _BOOL   Read_BMP(_ASCII*, _BMP*);
#endif
extern "C" _BOOL   Lse_File_Copy(_ASCII *dpne_src, _ASCII *dpne_dst);
extern "C" _BOOL   Lse_File_Rename(_ASCII *oldname, _ASCII *newname);

// T_FONT.CPP ***************************************************************
extern "C" _ATTRIB font_set(_UWORD);
extern "C" _ATTRIB font_set_by_language(_UWORD, _UWORD);
extern "C" void    font_size(_UWORD, _UWORD*, _UWORD*);
extern "C" _FONT   font_seek(_ASCII*);
extern "C" _ATTRIB font_set_direct(_FONT);
extern "C" void    font_size_direct(_FONT, _UWORD*, _UWORD*);

// T_GFX.CPP ****************************************************************
extern "C" void    compress_BMP(_BMP*);
extern "C" _BOOL   load_gfx(_BMP *bmp, _ASCII *dpne, _UWORD *p_errorcode, _FUDWORD_UDWORD_UDWORD fp_callback);
extern "C" _BOOL   flash_gfx(_KOORD, _KOORD, _ASCII*, _UWORD *p_errorcode, _FUDWORD_UDWORD_UDWORD fp_callback);
extern "C" _BOOL   save_gfx(_ASCII*, _KOORD, _KOORD, _KOORD, _KOORD, _UDWORD);
extern "C" _BOOL   save_gfx_offscreen(_ASCII*, _KOORD, _KOORD, _KOORD, _KOORD, _UDWORD, _NEWSCREEN*);
extern "C" _BOOL   save_gfx_BMP(_ASCII*, _BMP*, _UDWORD);
extern "C" _UWORD  MakeIndexColor(_UWORD, _UWORD, _UBYTE*);
extern "C" _BOOL   zoom_BMP(_BMP*, _UWORD, _UWORD);
#ifdef UC_FIXMEMORY
extern     _BOOL   fixzoom_BMP16Bit(_BMP*, _UWORD, _UWORD);
#endif
extern "C" _BOOL   raw_to_BMP(_BMP*, _UBYTE*, _UWORD, _UWORD);
extern     void    colorCorrection256(_BMP*, _RGB256*, _UWORD);
extern     _UWORD  get_pixel(_BMP*, _UWORD, _UWORD);
extern     _BOOL   get_pixel_line(_UWORD*, _BMP*, _UWORD);
extern     _BOOL   gfxerror_set(_UWORD*, _UWORD);
extern "C" void    transparent_BMP(_BMP *bmp, _UBYTE r, _UBYTE g, _UBYTE b);
extern "C" void    invert_BMP(_BMP *bmp);
extern "C" _BOOL   grab_bmp(_BMP *pd, _KOORD x0, _KOORD y0, _KOORD x1, _KOORD y1);

// T_GFXGIF.CPP *************************************************************
extern "C" _BOOL   load_gif(_BMP*, _ASCII*, _UWORD*);
extern "C" _BOOL   save_gif(_ASCII*, _KOORD, _KOORD, _KOORD, _KOORD);
extern "C" _BOOL   save_gif_BMP(_ASCII*, _BMP*);

// T_GFXICO.CPP *************************************************************
//extern     _BOOL   load_ico(_BMP*, _ASCII*, _UWORD*); // FILEXT_ICO

// T_HELP.CPP ***************************************************************
extern     _BOOL   GetTooLessMemory(void);
extern "C" _BOOL   NewBubbleHelp_Call(_KOORD x, _KOORD y, void *text, _UWORD chrsize);

// T_HOTKEY.CPP *************************************************************

// T_ICON.CPP ***************************************************************
extern     _UDWORD memsize_BMP(_BMP*);
extern "C" _BMP*   ImageList_Get(_UWORD);
extern "C" _UDWORD ImageList_Add(_ASCII *dpne, _ASCII *label, _BOOL invertcolor, _UWORD cred, _UWORD cgreen, _UWORD cblue);
extern "C" _UDWORD ImageList_Add_BMO(_ASCII *label, void *bmodata);

// T_INIT.CPP ***************************************************************
#ifdef UC_FIXMEMORY
extern "C" void   *GetFixMemory(void);
#endif
extern "C" void   init_IPCINI(_IPCINI*);
extern "C" void   free_IPCINI(_IPCINI*);
extern "C" void   MyGetScreenSize(_ROOM*);
extern "C" void   MyClrScr(_COLOR);
extern     void   set_lrm(_RESULT*, _BOOL);
extern "C" void   ScreenStart(void);
extern "C" void   ScreenStop(void);
extern void       ScreenStatePop(_UDWORD, _ROOM*);
extern _UDWORD    ScreenStatePush(void);
extern "C" void   init_MYSCHEME(_MYSCHEME*);
extern "C" void   free_MYSCHEME(_MYSCHEME*);
extern "C" void   init_LINKSCHEME(_LINKSCHEME*);
extern "C" void   setup_LINKSCHEME(_LINKSCHEME *p, _ASCII *colorschemelabel, _UDWORD varno);
extern "C" void   free_LINKSCHEME(_LINKSCHEME*);
extern "C" void   init_MEM_SINGLESCHEME(_MEM_SINGLESCHEME*);
extern "C" void   free_MEM_SINGLESCHEME(_MEM_SINGLESCHEME*);
extern "C" void   init_SINGLESCHEME(_SINGLESCHEME*);
extern "C" void   free_SINGLESCHEME(_SINGLESCHEME*);
extern "C" void   setup_SINGLESCHEME(_SINGLESCHEME *p, _UBYTE operat, _DWORD value, _UDWORD index);
extern "C" void   init_SCHEME(_SCHEME*);
extern "C" void   free_SCHEME(_SCHEME*);
extern "C" void   init_MEM_SCHEME(_MEM_SCHEME*);
extern "C" void   free_MEM_SCHEME(_MEM_SCHEME*);
extern "C" void   init_ROOM(_ROOM*);
extern "C" void   init_DOT(_DOT*);
extern "C" void   init_TIME(_TIME*);
extern "C" void   init_DATE(_DATE*);
extern "C" void   init_DATIM(_DATIM*);
extern "C" _BOOL  is_CHKBIT(_CHKBIT*);
extern "C" _BOOL  is_MYSCHEME(_MYSCHEME*, _Scheme*);
extern "C" _BOOL  chk_CHKBIT(_CHKBIT*);
extern "C" _BOOL  chk_XCHKBIT(_XCHKBIT*);
extern     _BOOL  chk_XXCHKBIT(_XXCHKBIT);
extern "C" void   init_WHOAMI(_WHOAMI*);
extern "C" void   free_WHOAMI(_WHOAMI*);
extern "C" void   init_NEWSCREENPROP(_NEWSCREENPROP *p);
extern "C" void   free_NEWSCREENPROP(_NEWSCREENPROP *p);
extern "C" void   setup_WHOAMI(_WHOAMI *pt, _IMA ima, _UDWORD no);
extern "C" void   init_CHKBIT(_CHKBIT*);
extern "C" void   free_CHKBIT(_CHKBIT*);
extern "C" void   free_XCHKBIT(_XCHKBIT*);
extern "C" void   init_XCHKBIT(_XCHKBIT*);
extern     void   init_XXCHKBIT(_XXCHKBIT*);
extern "C" void   init_ANYTHING(_ANYTHING*);
#ifdef UC_CHANGE_IO_GROUP
 extern    void   init_IOGRPCHX(_IOGRPCHX*);
 extern    void   free_IOGRPCHX(_IOGRPCHX*);
#endif
extern "C" void   free_ANYTHING(_ANYTHING*);
#ifdef UC_ABANDONED
extern "C" void   init_REPEAT(_REPEAT*);
extern "C" void   free_REPEAT(_REPEAT*);
#endif
extern "C" void   init_VIRTNAME(_VIRTNAME*);
extern "C" void   free_VIRTNAME(_VIRTNAME*);
extern "C" void   init_VARLABEL(_VARLABEL*);
extern "C" void   free_VARLABEL(_VARLABEL*);
extern "C" void   init_ZOOM(_ZOOM*);
extern "C" void   free_ZOOM(_ZOOM*);
extern "C" void   init_BMP(_BMP*);
extern "C" void   free_BMP(_BMP*);
extern "C" void   init_LEVEL(_LEVEL*);
extern "C" void   free_LEVEL(_LEVEL*);
extern "C" void   init_PICTEXT(_PICTEXT*);
extern "C" void   free_PICTEXT(_PICTEXT*);
extern "C" void   init_CIRCLE(_CIRCLE*);
extern "C" void   free_CIRCLE(_CIRCLE*);
extern "C" void   init_POLYGON(_POLYGON*);
extern "C" void   free_POLYGON(_POLYGON*);
extern "C" void   init_RECTANGLE(_RECTANGLE*);
extern "C" void   free_RECTANGLE(_RECTANGLE*);
extern "C" void   init_MEM_POLYGON(_MEM_POLYGON*);
extern "C" void   free_MEM_POLYGON(_MEM_POLYGON*);
extern "C" void   init_MEM_RECTANGLE(_MEM_RECTANGLE*);
extern "C" void   free_MEM_RECTANGLE(_MEM_RECTANGLE*);
extern "C" void   init_MEM_CIRCLE(_MEM_CIRCLE*);
extern "C" void   free_MEM_CIRCLE(_MEM_CIRCLE*);
extern "C" void   init_MEM_PICTEXT(_MEM_PICTEXT*);
extern "C" void   free_MEM_PICTEXT(_MEM_PICTEXT*);
extern "C" void   init_MEM_OBJECT(_MEM_OBJECT*);
extern "C" void   free_MEM_OBJECT(_MEM_OBJECT*);
extern "C" void   init_PICTURE(_PICTURE*);
extern "C" void   free_PICTURE(_PICTURE*);
extern "C" void   init_STYLESHEET(_STYLESHEET *p);
extern "C" void   setup_STYLESHEET(_STYLESHEET *p, _ASCII *fontschemename, _UDWORD frameid, _ATTRIB contextalign, _COLOR colback, _COLOR colfront, _COLOR colhatch, _COLOR coltopleft, _COLOR colbottomright, _LINKSCHEME *pdyncolback, _LINKSCHEME *pdyncolfront, _LINKSCHEME *pdyncolhatch, _LINKSCHEME *pdyncoltopleft, _LINKSCHEME *pdyncolbottomright);
extern "C" void   free_STYLESHEET(_STYLESHEET *p);
extern "C" void   init_PICTUREMASK(_PICTUREMASK*);
extern "C" void   free_PICTUREMASK(_PICTUREMASK*);
extern "C" void   init_REGION(_REGION*);
extern "C" void   free_REGION(_REGION*);
extern "C" void   init_MEM_VARSYMBOL(_MEM_VARSYMBOL*);
extern "C" void   free_MEM_VARSYMBOL(_MEM_VARSYMBOL*);
extern     void   init_MEM_BMP(_MEM_BMP*);
extern     void   free_MEM_BMP(_MEM_BMP*);
extern "C" void   init_MEM_LSEVARINFO(_MEM_LSEVARINFO*);
extern "C" void   free_MEM_LSEVARINFO(_MEM_LSEVARINFO*);
extern "C" void   init_VARBLOCK(_VARBLOCK*);
extern "C" void   free_VARBLOCK(_VARBLOCK*);
extern "C" void   init_STATISTIC(_STATISTIC*);
extern "C" void   free_STATISTIC(_STATISTIC*);
extern "C" void   init_VARINFO(_VARINFO*);
extern "C" void   free_VARINFO(_VARINFO*);
extern     void   init_LSEVARINFO(_LSEVARINFO*);
extern     void   free_LSEVARINFO(_LSEVARINFO*);
extern "C" void   init_VARSYMBOL(_VARSYMBOL*);
extern "C" void   free_VARSYMBOL(_VARSYMBOL*);
extern "C" void   init_VARIABLE(_VARIABLE*);
extern "C" void   free_VARIABLE(_VARIABLE*);
extern "C" void   init_SCROLL(_SCROLL*);
extern "C" void   free_SCROLL(_SCROLL*);
extern "C" void   init_EVENT(_EVENT*);
extern "C" void   free_EVENT(_EVENT*);
extern "C" void   init_CSTATE(_CSTATE*);
extern "C" void   free_CSTATE(_CSTATE*);
extern     void   init_COMBOINFO(_COMBOINFO*);
extern     void   free_COMBOINFO(_COMBOINFO*);
extern "C" void   init_IOINFO(_IOINFO*);
extern "C" void   free_IOINFO(_IOINFO*);
extern "C" void   init_EDITOR(_EDITOR*);
extern "C" void   free_EDITOR(_EDITOR*);
extern "C" void   init_MULTIKEYBOARD(_MULTIKEYBOARD*, _BOOL);
extern "C" void   free_MULTIKEYBOARD(_MULTIKEYBOARD*);
extern "C" void   init_RESULT(_RESULT*);
extern "C" void   init_MEM_INPUT(_MEM_INPUT*);
extern "C" void   free_MEM_INPUT(_MEM_INPUT*);
extern "C" void   init_MEM_OUTPUT(_MEM_OUTPUT*);
extern "C" void   free_MEM_OUTPUT(_MEM_OUTPUT*);
extern "C" void   init_SCROLLBAR(_SCROLLBAR *p);
extern "C" void   free_SCROLLBAR(_SCROLLBAR *p);
extern "C" void   init_MEM_MOTION(_MEM_MOTION*);
extern "C" void   free_MEM_MOTION(_MEM_MOTION*);
extern "C" void   init_BITMAP(_BITMAP*);
extern "C" void   free_BITMAP(_BITMAP*);
extern "C" void   init_MEM_BITMAP(_MEM_BITMAP*);
extern "C" void   free_MEM_BITMAP(_MEM_BITMAP*);
extern "C" void   init_INPUT(_INPUT*);
extern "C" void   free_INPUT(_INPUT*);
extern "C" void   init_IO(_IO*);
extern "C" void   free_IO(_IO*);
extern "C" void   init_MOTION(_MOTION*);
extern "C" void   free_MOTION(_MOTION*);
extern "C" void   init_CAROITEM(_CAROITEM *p);
extern "C" void   init_KARU(_KARU *p);
extern "C" void   free_KARU(_KARU *p);
extern "C" void   init_MEM_KARU(_MEM_KARU *p);
extern "C" void   free_MEM_KARU(_MEM_KARU *p);
extern "C" void   init_SINGLE_ALARM(_SINGLE_ALARM*);
extern "C" void   free_SINGLE_ALARM(_SINGLE_ALARM*);
extern "C" void   init_ALARM(_ALARM*);
extern "C" void   free_ALARM(_ALARM*);
extern "C" void   init_MEM_VARLABEL(_MEM_VARLABEL*);
extern "C" void   free_MEM_VARLABEL(_MEM_VARLABEL*);
extern "C" void   init_BUTTON(_BUTTON*);
extern "C" void   free_BUTTON(_BUTTON*);
extern "C" void   init_MEM_BUTTON(_MEM_BUTTON*);
extern "C" void   free_MEM_BUTTON(_MEM_BUTTON*);
extern "C" void   free_OBJECT(_OBJECT*);
extern "C" void   init_OBJECT(_OBJECT*);
extern "C" void   free_GFUNCT(_GFUNCT*);
extern "C" void   init_GFUNCT(_GFUNCT*);
extern "C" void   free_LSEFUNCT(_LSEFUNCT*);
extern "C" void   init_LSEFUNCT(_LSEFUNCT*);
extern "C" void   init_MEM_NO(_MEM_NO*);
extern "C" void   free_MEM_NO(_MEM_NO*);
extern     void   init_LANGUAGE_INFO(_LANGUAGE_INFO*);
extern     void   free_LANGUAGE_INFO(_LANGUAGE_INFO*);
extern "C" void   init_COMSET(_COMSET*);
extern "C" void   free_COMSET(_COMSET*);
extern "C" void   init_NEWSCREEN(_NEWSCREEN*);
extern "C" void   free_NEWSCREEN(_NEWSCREEN*);
extern "C" void   init_SCAN(_SCAN*);
extern "C" void   free_SCAN(_SCAN*);
extern "C" void   init_MEM_UNIT(_MEM_UNIT*);
extern "C" void   free_MEM_UNIT(_MEM_UNIT*);
extern "C" void   init_UNIT(_UNIT*);
extern "C" void   free_UNIT(_UNIT*);
extern "C" void   construct_MAIN(void);
extern "C" void   init_MAIN(_BOOL);
extern "C" void   free_MAIN(void);
extern "C" void   init_ZORDER(_ZORDER*);
extern "C" void   free_ZORDER(_ZORDER*);
extern "C" void   init_MEM_ZORDER(_MEM_ZORDER*);
extern "C" void   free_MEM_ZORDER(_MEM_ZORDER*);
extern "C" _BOOL  chk_VARLABEL(_VARLABEL*);
extern "C" _BOOL  valid_VARLABEL(_VARLABEL*);
extern     _BOOL  is_server_defined(_VARLABEL*);
extern "C" void   init_SOFTFLOOR(_SOFTFLOOR*);
extern "C" void   free_SOFTFLOOR(_SOFTFLOOR*);
extern "C" void   init_SINGLESOFT(_SINGLESOFT*);
extern "C" void   free_SINGLESOFT(_SINGLESOFT*);
extern "C" void   init_PRJ_ENTRY(_PRJ_ENTRY*);
extern "C" void   free_PRJ_ENTRY(_PRJ_ENTRY*);
extern "C" void   init_PROJECT(_PROJECT*);
extern "C" void   free_PROJECT(_PROJECT*);
extern "C" void   init_STACK_LINE(_STACK_LINE*);
extern "C" void   free_STACK_LINE(_STACK_LINE*);
extern "C" void   init_FONTINFO(_FONTINFO*);
extern "C" void   free_FONTINFO(_FONTINFO*);
extern "C" void   init_ACTUAL(_ACTUAL*);
extern "C" void   free_ACTUAL(_ACTUAL*);
extern "C" void   init_SYSTEM(_SYSTEM*);
extern "C" void   free_SYSTEM(_SYSTEM*);
extern "C" void   init_PARA_PICKER(_PARA_PICKER*);
extern "C" void   free_PARA_PICKER(_PARA_PICKER*);
extern "C" void   init_CHREDIT(_CHREDIT*);
extern "C" void   free_CHREDIT(_CHREDIT*);
extern "C" void   init_IMAGE(_IMAGE*);
extern "C" void   free_IMAGE(_IMAGE*);
extern "C" void   init_POSITION(_POSITION*);
extern "C" void   free_POSITION(_POSITION*);
extern "C" void   init_MENUPOINTER(_MENUPOINTER*);
extern "C" void   free_MENUPOINTER(_MENUPOINTER*);
extern "C" void   init_MENUCALCUL(_MENUCALCUL*);
extern "C" void   free_MENUCALCUL(_MENUCALCUL*);
extern "C" void   init_MENU(_MENU*);
extern "C" void   free_MENU(_MENU*);
extern "C" void   init_LSEMENU(_LSEMENU*);
extern "C" void   free_LSEMENU(_LSEMENU*);
extern "C" void   init_SUBACTUAL(_SUBACTUAL*);
extern "C" void   free_SUBACTUAL(_SUBACTUAL*);
extern "C" void   init_COL_RECT(_COL_RECT*);
extern "C" void   init_MEM_COL_RECT(_MEM_COL_RECT*);
extern "C" void   free_COL_RECT(_COL_RECT*);
extern "C" void   free_MEM_COL_RECT(_MEM_COL_RECT*);
extern "C" void   init_COL_POLY(_COL_POLY*);
extern "C" void   init_MEM_COL_POLY(_MEM_COL_POLY*);
extern "C" void   free_COL_POLY(_COL_POLY*);
extern "C" void   free_MEM_COL_POLY(_MEM_COL_POLY*);
extern "C" void   init_COL_CIRC(_COL_CIRC*);
extern "C" void   init_MEM_COL_CIRC(_MEM_COL_CIRC*);
extern "C" void   free_COL_CIRC(_COL_CIRC*);
extern "C" void   free_MEM_COL_CIRC(_MEM_COL_CIRC*);
extern "C" void   init_COL_PICTXT(_COL_PICTXT*);
extern "C" void   free_COL_PICTXT(_COL_PICTXT*);
extern "C" void   init_MEM_COL_PICTXT(_MEM_COL_PICTXT*);
extern "C" void   free_MEM_COL_PICTXT(_MEM_COL_PICTXT*);
extern "C" void   init_KEY(_KEY*);
extern "C" void   free_KEY(_KEY*);
extern "C" void   init_MEM_KEY(_MEM_KEY*);
extern "C" void   free_MEM_KEY(_MEM_KEY*);
extern "C" void   init_COMDEF(_COMDEF*);
extern "C" void   free_COMDEF(_COMDEF*);
extern "C" void   init_COMPORT(_COMPORT*);
extern "C" void   free_COMPORT(_COMPORT*);
extern "C" void   init_INDEX(_INDEX*);
extern "C" void   free_INDEX(_INDEX*);
extern "C" void   init_INDEXDAT(_INDEXDAT*);
extern "C" void   free_INDEXDAT(_INDEXDAT*);
extern "C" void   init_REFDAT(_REFDAT*);
extern "C" void   free_REFDAT(_REFDAT*);
extern "C" void   init_REFLIST(_REFLIST*);
extern "C" void   free_REFLIST(_REFLIST*);
extern "C" void   init_LASALID(_LASALID*);
extern "C" void   init_PICFORMAT(_PICFORMAT*);
extern "C" void   free_PICFORMAT(_PICFORMAT*);
extern "C" void   init_LINEINFO(_LINEINFO*);
extern "C" void   free_LINEINFO(_LINEINFO*);
extern "C" void   init_MEM_LINEINFO(_MEM_LINEINFO*);
extern "C" void   free_MEM_LINEINFO(_MEM_LINEINFO*);
extern "C" void   init_MENUINFO(_MENUINFO*);
extern "C" void   free_MENUINFO(_MENUINFO*);
extern "C" void   init_MEM_LSEMENU(_MEM_LSEMENU*);
extern "C" void   free_MEM_LSEMENU(_MEM_LSEMENU*);
extern "C" void   init_WORKSTATE(_WORKSTATE*);
extern "C" void   init_STACK(_STACK*);
extern "C" void   free_STACK(_STACK*);
extern "C" void   init_ASCII_BUFFER(_ASCII_BUFFER*);
extern "C" void   free_ASCII_BUFFER(_ASCII_BUFFER*);
extern "C" void   init_TEXT_BUFFER(_TEXT_BUFFER*);
extern "C" void   free_TEXT_BUFFER(_TEXT_BUFFER*);
extern "C" _UWORD cmp_VARLABEL(_VARLABEL*, _VARLABEL*);
extern "C" void   init_COMPCODE(_COMPCODE*);
extern "C" void   free_COMPCODE(_COMPCODE*);
extern "C" void   init_SINGLESEEK(_SINGLESEEK*);
extern "C" void   free_SINGLESEEK(_SINGLESEEK*);
extern "C" void   init_SEEKELEMENT(_SEEKELEMENT*);
extern "C" void   free_SEEKELEMENT(_SEEKELEMENT*);
extern "C" void   init_MEM_SEEKELEMENT(_MEM_SEEKELEMENT*);
extern "C" void   free_MEM_SEEKELEMENT(_MEM_SEEKELEMENT*);
extern "C" void   init_MY_PICTURE(_MY_PICTURE*);
extern "C" void   free_MY_PICTURE(_MY_PICTURE*);
extern "C" _BOOL  setup_MY_PICTURE(_MY_PICTURE *);
extern "C" void   free_EDITCOLOR(_EDITCOLOR*);
extern "C" void   init_EDITCOLOR(_EDITCOLOR*);
extern "C" void   init_MEM_ATTRIB(_MEM_ATTRIB*);
extern "C" void   free_MEM_ATTRIB(_MEM_ATTRIB*);
extern "C" void   init_MEM_COLOR(_MEM_COLOR*);
extern "C" void   free_MEM_COLOR(_MEM_COLOR*);
extern "C" void   init_MEM_ANYTHING(_MEM_ANYTHING*);
extern "C" void   free_MEM_ANYTHING(_MEM_ANYTHING*);
extern "C" void   init_MEM_ANYPARA(_MEM_ANYPARA*);
extern "C" void   free_MEM_ANYPARA(_MEM_ANYPARA*);
extern "C" void   init_XMEM_ANYPARA(_XMEM_ANYPARA*);
extern "C" void   free_XMEM_ANYPARA(_XMEM_ANYPARA*);
extern "C" void   init_MEM_LINKSCHEME(_MEM_LINKSCHEME*);
extern "C" void   free_MEM_LINKSCHEME(_MEM_LINKSCHEME*);
extern "C" void   init_DYNCOLOR(_DYNCOLOR*);
extern "C" void   free_DYNCOLOR(_DYNCOLOR*);
extern "C" void   init_MEM_DYNCOLOR(_MEM_DYNCOLOR*);
extern "C" void   free_MEM_DYNCOLOR(_MEM_DYNCOLOR*);
extern "C" void   init_MEM_FONT(_MEM_FONT*);
extern "C" void   free_MEM_FONT(_MEM_FONT*);
extern "C" void   init_MEM_BOOL(_MEM_BOOL*);
extern "C" void   free_MEM_BOOL(_MEM_BOOL*);
extern "C" void   init_MEM_TextPuffer(_MEM_TextPuffer*);
extern "C" void   free_MEM_TextPuffer(_MEM_TextPuffer*);
extern "C" void   init_BUBBLEINFO(_BUBBLEINFO*);
extern "C" void   free_BUBBLEINFO(_BUBBLEINFO*);
extern "C" void   init_CURSORCOLOR(_CURSORCOLOR*);
extern "C" void   free_CURSORCOLOR(_CURSORCOLOR*);
extern "C" void   init_ANYPARA(_ANYPARA*);
extern "C" void   free_ANYPARA(_ANYPARA*);
extern "C" void   init_SINGLETXTSCHEME(_SINGLETXTSCHEME*);
extern "C" void   free_SINGLETXTSCHEME(_SINGLETXTSCHEME*);
extern "C" void   init_TXTSCHEME(_TXTSCHEME*);
extern "C" void   free_TXTSCHEME(_TXTSCHEME*);
extern "C" void   init_MEM_TXTSCHEME(_MEM_TXTSCHEME*);
extern "C" void   free_MEM_TXTSCHEME(_MEM_TXTSCHEME*);
extern "C" void   init_MYTXTSCHEME(_MYTXTSCHEME*);
extern "C" void   free_MYTXTSCHEME(_MYTXTSCHEME*);
extern "C" void   init_THEURGE(_THEURGE*);
extern "C" void   free_THEURGE(_THEURGE*);
extern "C" void   init_SHAPE(_SHAPE*);
extern "C" void   free_SHAPE(_SHAPE*);
extern "C" void   init_3DSHAPE(_3DSHAPE*);
extern "C" void   free_3DSHAPE(_3DSHAPE*);
extern     void   init_HILOLIM(_HILOLIM*);
extern "C" void   init_USERTEXT(_USERTEXT*);
extern "C" _BOOL  setup_USERTEXT(_USERTEXT *p, void *txt, _UWORD chrsize);
extern "C" void   free_USERTEXT(_USERTEXT*);


extern void init_MEM_INDEX(_MEM_INDEX*);
extern void free_MEM_INDEX(_MEM_INDEX*);
extern void init_TOOLDATA(_TOOLDATA*);
extern void free_TOOLDATA(_TOOLDATA*);
extern void init_EVENTMEMO(_EVENTMEMO*);
extern void free_EVENTMEMO(_EVENTMEMO*);

extern "C" void   init_RINGBUFFER(_RINGBUFFER*);
extern "C" void   free_RINGBUFFER(_RINGBUFFER*);

#ifdef OK_SUBLIST
extern "C" void   init_VKLIST(_VKLIST*);
extern "C" void   free_VKLIST(_VKLIST*);
extern "C" void   init_MEM_VKLIST(_MEM_VKLIST*);
extern "C" void   free_MEM_VKLIST(_MEM_VKLIST*);
#endif

#ifdef UC_DIRECTORY
extern "C" void   init_DIRLIST(_DIRLIST*);
extern "C" void   free_DIRLIST(_DIRLIST*);
extern "C" void   init_DIRECTORY(_DIRECTORY*);
extern "C" void   free_DIRECTORY(_DIRECTORY*);
extern "C" void   init_DIRECTORYSETUP(_DIRECTORYSETUP*);
extern "C" void   free_DIRECTORYSETUP(_DIRECTORYSETUP*);
#endif

extern "C" void   init_DPNE(_DPNE*);
extern "C" void   free_DPNE(_DPNE*);
extern "C" void   init_PICRUNMEMO(_PICRUNMEMO*);
extern "C" void   free_PICRUNMEMO(_PICRUNMEMO*);

#ifdef OK_PICMEMO
extern "C" void   init_LASTINPUT(_LASTINPUT*);
extern "C" void   free_LASTINPUT(_LASTINPUT*);
extern "C" void   init_GLOBMEMO(_GLOBMEMO*);
extern "C" void   free_GLOBMEMO(_GLOBMEMO*);
extern "C" void   init_MEM_GLOBMEMO(_MEM_GLOBMEMO*);
extern "C" void   free_MEM_GLOBMEMO(_MEM_GLOBMEMO*);
#endif
#ifdef OK_VALIDATE
extern "C" void   init_VALIDATA(_VALIDATA*);
extern "C" void   free_VALIDATA(_VALIDATA*);
extern "C" void   init_MEM_VALIDATA(_MEM_VALIDATA*);
extern "C" void   free_MEM_VALIDATA(_MEM_VALIDATA*);
#endif
extern "C" void   init_MEM_POINTER(_MEM_POINTER*);
extern "C" void   free_MEM_POINTER(_MEM_POINTER*);
extern "C" void   init_POINTERPARA(_POINTERPARA *p);
extern "C" void   init_MEM_POINTERPARA(_MEM_POINTERPARA *p);
extern "C" void   free_MEM_POINTERPARA(_MEM_POINTERPARA *p);
extern "C" void   init_SCREENSAVER(_SCREENSAVER*);
extern "C" void   free_SCREENSAVER(_SCREENSAVER*);
extern     void   init_DEFKEYPAD(_DEFKEYPAD *p);
extern     void   free_DEFKEYPAD(_DEFKEYPAD *p);

extern "C" void   init_DIRECTION(_DIRECTION*);
extern "C" void   free_DIRECTION(_DIRECTION*);
extern "C" void   init_SINGLEALARM(_SINGLEALARM*);
extern "C" void   free_SINGLEALARM(_SINGLEALARM*);
extern "C" void   init_MEM_SINGLEALARM(_MEM_SINGLEALARM*);
extern "C" void   free_MEM_SINGLEALARM(_MEM_SINGLEALARM*);
extern "C" _BOOL  make_MEM_SINGLEALARM(_MEM_SINGLEALARM*, _UDWORD, _UWORD);
extern "C" void   init_MENCURSOR(_MENCURSOR*);
extern "C" void   free_MENCURSOR(_MENCURSOR*);
extern "C" void   set_MENCURSOR(_MENCURSOR*);
extern "C" _BOOL  ask_MENCURSOR(_MENCURSOR*);
extern "C" _BOOL  chk_LASALID(_LASALID*);
extern "C" _BOOL  chk_LINEINFO(_LINEINFO*);
extern "C" _UDWORD state_LINEINFO(_LINEINFO *p);
extern "C" _BOOL  chk_ROOM(_ROOM*);
extern "C" _BOOL  IsDrawBackGround(void);
extern "C" _BOOL  IsDrawPicture(void);
extern "C" void   init_LSEOSZI(_LSEOSZI*);
extern "C" void   free_LSEOSZI(_LSEOSZI*);
extern "C" void   init_LSEOSZI_CHANNEL(_LSEOSZI_CHANNEL*);
extern "C" void   free_LSEOSZI_CHANNEL(_LSEOSZI_CHANNEL*);
extern "C" void   init_MEM_LSEOSZI_CHANNEL(_MEM_LSEOSZI_CHANNEL*);
extern "C" void   free_MEM_LSEOSZI_CHANNEL(_MEM_LSEOSZI_CHANNEL*);
extern "C" void   init_LINKVL(_LINKVL*);
extern "C" void   free_LINKVL(_LINKVL*);
#ifdef UC_LINKVL
extern     void   init_MEM_LINKVL(_MEM_LINKVL*);
extern     void   free_MEM_LINKVL(_MEM_LINKVL*);
#endif
extern     void init_MEM_MAP(_MEM_MAP*);
extern     void free_MEM_MAP(_MEM_MAP*);
extern     void init_MEM_LSEFUNCT(_MEM_LSEFUNCT*);
extern     void free_MEM_LSEFUNCT(_MEM_LSEFUNCT*);
extern     void init_FAST_ASCII_BUFFER(_FAST_ASCII_BUFFER*);
extern     void free_FAST_ASCII_BUFFER(_FAST_ASCII_BUFFER*);
extern     void init_SOFTKEYSETUP(_SOFTKEYSETUP*);
extern     void free_SOFTKEYSETUP(_SOFTKEYSETUP*);
extern     void init_BUTTONIMAGES(_BUTTONIMAGES*);
extern     void free_BUTTONIMAGES(_BUTTONIMAGES*);
extern "C" void init_DATA(_DATA*);
extern "C" void free_DATA(_DATA*);
extern     void init_NAME_INDEX(_NAME_INDEX*);
extern     void free_NAME_INDEX(_NAME_INDEX*);
extern     void init_SVRWATCH(_SVRWATCH*);
extern     void free_SVRWATCH(_SVRWATCH*);
extern     void init_LSE_TIME(_LSE_TIME*);
extern     void free_LSE_TIME(_LSE_TIME*);

extern "C" _UDWORD AcknowledgeFlag(_DWORD cmd);

void init_TREEITEM(_TREEITEM*);
void free_TREEITEM(_TREEITEM*);
void init_TREE(_TREE*);
void free_TREE(_TREE*);
void init_TREESCOPE(_TREESCOPE*);
void free_TREESCOPE(_TREESCOPE*);
void init_FILEXPLORER(_FILEXPLORER*);
void free_FILEXPLORER(_FILEXPLORER*);

extern _UDWORD  GetRevisionOfClass_MyIo(void);
#ifdef UC_DELETE_REFLIST
extern "C" void DeleteRefreshlistOnRequest(_BOOL);
#endif

#ifdef UC_IPC
extern void init_MEMSIZE(_MEMSIZE*);
#endif

#ifdef UC_THEME
extern void init_THEME(_THEME*);
extern void free_THEME(_THEME*);
extern void init_MEM_THEME(_MEM_THEME*);
extern void free_MEM_THEME(_MEM_THEME*);
#endif

// JAPAN.CPP ****************************************************************
#ifdef UC_JAPANINPUT
 #define JAPAN_DIACRIT_1    12317 // "
 #define JAPAN_DIACRIT_2    12332 // °

 extern     void    Japan_cTor(void);
 extern     _UDWORD Japan_InputMode(void);
// extern     _UDWORD AsciiToJapanese(_CHAR *dst, _CHAR *src);
 extern     _UDWORD AsciiToJapanesePosition(_CHAR *dst, _UWORD *ppos, _CHAR *src, _UWORD japan_begin_pos);
 extern     void   *Japan_GetDictionary(void);

// JAPAN_DICTIONARY.CPP *****************************************************
 extern     void*   JapanDictionaryLoad(const char *dpne);
 extern     void*   JapanDictionaryFree(void *hdl);
 extern     _UDWORD JapanDictionaryGetText(_UNI **pptxt, _UDWORD maxno, void *hdl, _UNI *key);
#endif // UC_JAPANINPUT

// T_JPG.CPP ****************************************************************
extern "C" void   JpgFunctSetup(void *fpload, void *fpsave);
extern "C" _BOOL  load_jpg(_BMP*, _ASCII*, _UWORD*, _FUDWORD_UDWORD_UDWORD);
extern "C" _BOOL  save_jpg(_ASCII*, _KOORD, _KOORD, _KOORD, _KOORD, _UWORD, _BOOL);
extern "C" _BOOL  save_jpg_BMP(_ASCII*, _BMP*, _UWORD, _BOOL);


// T_KBRD.CPP ***************************************************************
extern "C" _BOOL  AddEvent(_EVENT*);
extern "C" void   StartStopEvent(_BOOL, _UWORD);
extern "C" _UDWORD SetRP_DelayTime(_UDWORD ms_time);
extern "C" _BOOL  IsCtrlKeyPress(_EVENT *pev, _UBYTE character);

// T_LANG.CPP ***************************************************************
#ifdef UC_IPC
extern "C" _BOOL  seperate_textlist(_CHAR **dst, _UDWORD *pno, _UWORD language_no, _UWORD txtlst);
#endif
extern "C" void   Language_GetStandardText(void **ptxt, _UWORD *pchrsize, _STDTEXTID txtid);
extern "C" _LOCAL Language_GetActualLocal(void);

// T_LibEx.cpp **************************************************************
#define lib_arraysize(_P) (sizeof(_P) / sizeof(_P[0]))
#define                    lib_sprintf    LSL_POS->piCRT->sprintf

extern "C" unsigned long   lib_tabsolute(void);
extern "C" unsigned long   lib_tabsolute_nonzero(void);
extern "C" unsigned long   lib_thread_create2(void *pfkt, unsigned long priority, unsigned long time_us, void *pcookie, const char *name);
extern "C" unsigned long   lib_is_salamander(void);
extern "C" unsigned long   lib_is_linux(void);
extern "C" unsigned long   lib_is_lars(void);
extern "C" char           *lib_name_unique(char *pd, const char *name);
extern "C" void           *lib_calloc(unsigned long size);
extern "C" void           *lib_malloc(unsigned long size);
extern "C" void           *lib_realloc(void *ptr, unsigned long size);
extern "C" void           *lib_free(void *ptr);
           unsigned long   lib_memory(void **pptr, unsigned long size);
           unsigned long   lib_strlen16(const unsigned short* str0);
           unsigned long   lib_strcut_chr16(unsigned short *str, unsigned short chr, unsigned long no);
           unsigned short *lib_strcpytill16(unsigned short *dst, const unsigned short *src, unsigned short tillchr);
           unsigned long   lib_delete_final16(unsigned short *p, unsigned short chr);
           void           *lib_fileread(unsigned long *psize, const char *dpne);
           void            lib_memcpy(void *pdst, const void *psrc, unsigned long size);
           unsigned long   lib_memcmp(void *p0, const void *p1, unsigned long size);
           unsigned long   lib_get_max_text_width(_CHAR **pptxt, unsigned long no);
extern "C" unsigned long   lib_atomic_incU32(unsigned long *pValue);
extern "C" unsigned long   lib_atomic_decU32(unsigned long *pValue);
extern "C" unsigned long   lib_atomic_cmpxchgU32(unsigned long *pValue, unsigned long cmpVal, unsigned long newVal);
extern "C" void            lib_sleep(unsigned long duration_ms);

// T_LIB.CPP ****************************************************************
extern void    Sleep(_UDWORD duration);
extern void    WriteU4(_UBYTE *dst, _UDWORD src);
extern void    WriteU2(_UBYTE *dst, _UWORD src);
extern void    WriteU1(_UBYTE *dst, _UBYTE src);
extern _UDWORD ReadU4(_UBYTE *src);
extern _UWORD  ReadU2(_UBYTE *src);
extern _UBYTE  ReadU1(_UBYTE *src);
#ifdef PGTEST
extern void    Annahme(const _ASCII*, int);                        // testfunction
#endif
extern void    Debug(const void*, _UWORD, _UWORD);
extern _BOOL   cis_server_valid(_LSEVARINFO**, _UDWORD);
extern void    FrameInlet(_ROOM*, _ROOM*, _ATTRIB);
extern _BOOL   SizeMultiTextAlign(_UWORD*, _UWORD*, void*, _FONT, _ATTRIB, _UWORD);
extern "C" void  GetTextRoom(_ROOM*, _ROOM*, _ATTRIB, _FRAMETYPE, _BOOL);
extern void    SetLseServer(_UWORD, _DWORD);
extern _DWORD  GetLseServer(_DWORD);
extern _DOIT   call_enclosed_name_method(_UWORD, _IO*, _VARIABLE*, _RESULT*, _EDITOR*, _EVENT*, _BOOL);
extern _UWORD  offset_dpne(_ASCII*, _UWORD);
extern _BOOL   editor_to_text(_EDITOR*);
//extern _UDWORD timediff(_UDWORD);                    // get timediff till now
extern _UDWORD do_floating_dezpt(_IVALUE*, _UWORD, _UWORD*);
extern _UWORD  sqroot(_DWORD);                          // calculate int sqrt
extern _BOOL   check_superpassword(_UDWORD, _UWORD);
extern _BOOL   bmp_size(_UWORD*, _UWORD*, _UWORD, _ATTRIB);
extern _KOORD  linear_distance(_RESULT*, _KOORD);
extern _WORD   calculate_bar(_RESULT*, _WORD);
extern _CHAR   *get_temptext(void);
extern _DOT    get_left_top(_DOT, _WORD, _WORD, _ATTRIB);
extern _KOORD  Zoom(_WORD, _KOORD);
extern _WORD   Zoom_ZOOM(_DWORD, _DWORD);
extern void    Make_ZOOM(_ZOOM*, _SubPicture*, _ROOM*);
extern void    Zoom_ROOM(_ROOM*, _DOT*, _ZOOM*);
extern void    align_koord(_KOORD*, _KOORD*, _KOORD*, _KOORD*);
extern _UWORD  GetPattern(_ATTRIB);
extern _ATTRIB DelPattern(_ATTRIB);
extern _ATTRIB SetOutlineAttrib(_ATTRIB);
extern _ATTRIB SetXSize(_ATTRIB, _UWORD);
extern _ATTRIB SetYSize(_ATTRIB, _UWORD);
extern void    linear_coordinates(_KOORD*, _KOORD*, _KOORD*, _KOORD*);
extern void    Shadow(_ROOM*);
extern void    GetShadowRoom(_ROOM*);
extern void    DrawShadow(_ROOM, _FRAMETYPE);
extern _BOOL   sort_xchange_ASCII(_ASCII*, _ASCII*);
extern "C" _WORD  qsort_CHAR(_CHAR*, _CHAR*);
extern "C" _WORD  qsort_ASCII(_ASCII*, _ASCII*);
extern "C" _DWORD get_multiplicator(_UWORD);
extern "C" void   RealToStr(void*, _REAL, _UWORD, _UWORD);
extern "C"_BOOL   StrToReal(_REAL*, void*, _UWORD);
extern     void   delete_bit(_ASCII *label);
#ifdef UC_COMPLEX_SERVER
extern     void     delete_element(_ASCII *label);
#endif
extern void Trace(const _ASCII *txt);
extern _BOOL   merge_MEM_VARLABEL(_MEM_VARLABEL*, _CHAR*, _MEM_VARLABEL*, _CHAR*, _MEM_VARLABEL*, _CHAR*);
extern "C" void draw_3DSHAPE(_KOORD, _KOORD, _3DSHAPE*);
extern void    slurry(void);
// Just for internal use
extern "C" void    *find_lasal_method(void*, _UWORD);
extern "C" void    *find_this_method(void*, _UWORD);
extern "C" void    *get_this_pointer(_ASCII*);
extern "C" void    *GetVirtualMethodPointer(void *pthis, _UDWORD vmno);
extern "C" _UDWORD  GetVirtualMethodNumber(void *pthis, void *pvm);
extern "C" void    TeachGraphicSystem(_UDWORD);
extern "C" void    ConfigGraphicSystem(_UDWORD, _UDWORD, void*);
extern "C" void    ReadPixelStart(_KOORD x0, _KOORD y0, _KOORD x1, _KOORD y1);
extern "C" void    ReadPixelStop(void);
extern "C" _BOOL   add_MEM_POINTER(_MEM_POINTER*, void*);
extern "C" _BOOL   add_MEM_POINTERPARA(_MEM_POINTERPARA*, void*, _UDWORD);
extern "C" _BOOL   make_MEM_POINTERPARA(_MEM_POINTERPARA *p, _UDWORD anzahl);

// Undocumented function - dont use this functions
extern "C" void    Lse_DToH(void*, _DWORD, _UWORD, _UWORD);
extern "C" void    Lse_DToA(void*, _DWORD, _UWORD, _UWORD);
extern "C" void    Lse_StrCpy(void*, _UWORD, void*, _UWORD);
extern "C" void    Lse_StrCat(void*, _UWORD, void*, _UWORD);
extern "C" _UWORD  Lse_StrCmp(const void*, _UWORD, const void*, _UWORD);
extern "C" _UWORD  Lse_StrSize(void*, _UWORD);
extern "C" _UWORD  Lse_StrChr(void*, _UNI, _UWORD);
extern "C" void    Lse_StrUp(void*, _UWORD, _UWORD);
extern "C" void    Lse_OutTextAlign(_CHAR*, _ROOM*, _FONT, _ATTRIB, _COLOR);

extern "C" void    Lse_OutParaString(_ROOM *pr, _CHAR *pt, _MEM_VARLABEL *pv, _FONT font, _ATTRIB att, _COLOR col);
#ifdef _LSL_TARGETARCH_X86
extern "C" void    Lse_OutParameterString(_ROOM, _CHAR*, _MEM_VARLABEL*, _FONT, _ATTRIB, _COLOR);
#endif
extern "C" void    lse_format_value(_CHAR*, _IVALUE, _VARINFO*, _PICFORMAT*);
// String functions
extern "C" void    StrNCpy(void*, _UWORD, const void*, _UWORD, _UWORD);
extern "C" void    StrCat(void*, _UWORD, const void*, _UWORD);
extern "C" _UWORD  StrSize(const void*, _UWORD);
extern "C" void    DelComment(void*, _UWORD);
extern "C" void    DelChr(void*, _UWORD, _UWORD);
extern "C" void    DelSpace(void*, _UWORD);
extern "C" void    StrFill(void*, _UNI, _UWORD, _UWORD);
extern "C" _UWORD  StrCmp(const void*, _UWORD, const void*, _UWORD);
extern "C" void*   StrUp(void*, _UWORD, _UDWORD);
extern "C" void    UniToAscii(_ASCII*, _UNI*, _UDWORD);
extern "C" void    AsciiToUni(_UNI*, _ASCII*, _UDWORD);
extern "C" void    uni_to_ascii(_ASCII*, _UNI*, _UDWORD);
extern "C" void    ascii_to_uni(_UNI*, _ASCII*, _UDWORD);
extern "C" _BOOL   create_filename(_ASCII*, _ASCII*);
extern "C" void    sort_ASCII(_ASCII*, _UWORD, _QSORTFUNCT);
// Textdisplay functions
extern "C" void    out_radiobutton(_ROOM*, _DWORD, _ATTRIB, _COLOR, _COLOR, _COLOR);
extern "C" void    out_chkbox(_ROOM*, _DWORD, _ATTRIB, _COLOR, _COLOR, _COLOR);
extern "C" void    OutTextAlign(void*, _UNI, _ROOM*, _FONT, _ATTRIB, _FRAMETYPE, _COLOR, _COLOR, _COLOR, _UWORD);
extern     void    OutMultiTextIconXY(_KOORD, _KOORD, void*, _ATTRIB, _COLOR, _UWORD, _FONT);
extern     void    OutMultiTextIconAlign(void*, _UNI, _ROOM*, _FONT, _ATTRIB, _FRAMETYPE, _COLOR, _COLOR, _COLOR, _UWORD);
extern "C" void    OutTextIconAlign(void*, _UNI, _ROOM*, _FONT, _ATTRIB, _FRAMETYPE, _COLOR, _COLOR, _COLOR, _UWORD);
extern "C" void    OutTxtAlign(void*, _ROOM*, _FONT, _ATTRIB, _COLOR, _COLOR, _COLOR, _UWORD);
extern "C" void    OutMultiTextAlign(void*, _UNI, _ROOM*, _FONT, _ATTRIB, _FRAMETYPE, _COLOR, _COLOR, _COLOR, _UWORD);
extern "C" void    OutMultiTxtAlign(void*, _ROOM*, _FONT, _ATTRIB, _COLOR, _COLOR, _COLOR, _UWORD);
extern "C" void    out_ANYTHING(_ANYTHING*, _ROOM*, _FONT, _ATTRIB, _FRAMETYPE, _COLOR, _COLOR, _COLOR, _TextPuffer*);
// Clipping and invisible screens
extern "C" _REGION ActivateAndRegion(_REGION*, _REGION*, _ROOM*);
extern "C" void    DeactivateAndRegion(_REGION, _REGION*, _REGION*);
extern "C" _REGION InvertRegion(void);
extern "C" void    ActivateScreen(_NEWSCREEN*);
extern "C" void    push_screen(_NEWSCREEN*);
extern "C" void    pop_screen(_NEWSCREEN*);
extern "C" void    copy_screen(_NEWSCREEN*, _ROOM*);
// Converting
extern "C" _DWORD  string_to_numeric(_CHAR*, _UWORD);
extern "C" _BOOL   string_to_date(_DATE*, _CHAR*, _UWORD);
extern "C" _BOOL   string_to_time(_TIME*, _CHAR*, _UWORD);
extern "C" _UDWORD string_to_ipaddress(_CHAR *txt);
extern "C" _IVALUE string_to_value(_IVALUE, _VARINFO*);
extern "C" void    NumToStr(void*, _DWORD, _UWORD);
extern      _DWORD AsciiToNo(_ASCII**);
extern "C" _BOOL   StrToNum(_DWORD*, void*, _UWORD);
extern "C" void    DToA(void*, _DWORD, _UWORD, _UWORD);
extern "C" void    DToH(void*, _DWORD, _UWORD, _UWORD);
extern "C" _DWORD  AToD(void*, _UWORD);
extern     _BOOL   add_hex(_UDWORD*, _UWORD);
extern "C" _UDWORD HToD(void*, _UWORD);
extern "C" void    lasal_to_DATE(_DATE*, _IVALUE);
extern "C" void    DATE_to_lasal(_IVALUE*, _DATE*);
extern "C" _DWORD  DATE_Diff(_DATE*, _DATE*);
extern "C" void    lasal_to_TIME(_TIME*, _IVALUE);
extern "C" void    TIME_to_lasal(_IVALUE*, _TIME*);
// Ringbuffer
extern "C" _UDWORD wpos_RINGBUFFER(_RINGBUFFER*);
extern "C" _UDWORD no_RINGBUFFER(_RINGBUFFER*);
extern "C" _UDWORD rpos_RINGBUFFER(_RINGBUFFER*, _UDWORD);
extern     _UDWORD teiler_RINGBUFFER(_RINGBUFFER*, _UDWORD*, _UDWORD*);
extern "C" void    sort_ringbuffer(void*, _RINGBUFFER*, _UWORD, _SORTFUNCT);
// Sorting
extern "C" void    sort(void*, _UDWORD, _UWORD, _SORTFUNCT);
extern "C" void    qsort(void*, _UDWORD, _UWORD, _QSORTFUNCT);
// Date and time functions
extern "C" void    set_DATE(_DATE*);
extern "C" void    get_DATE(_DATE*);
extern "C" void    set_TIME(_TIME*);
extern "C" void    get_TIME(_TIME*);
extern "C" void    get_DATIM(_DATIM*);
extern "C" void    set_DATIM(_DATIM*);
extern "C" _UWORD  day_of_week(_DATE*);
// Formating functions
extern "C" _UWORD  format_variable(_CHAR*, _VARLABEL*);
extern "C" _UWORD  format_unit(_CHAR*, _VARLABEL*);
extern "C" void    format_value(_CHAR*, _IVALUE, _VARINFO*, _PICFORMAT*);
extern "C" void    format_ipaddress(_CHAR*, _UDWORD);
extern "C" void    format_string(_CHAR*, _CHAR*, _VARINFO*);
extern     void    format_string_sub(_CHAR*, _CHAR*, _VARINFO*, _UWORD);
extern "C" void    format_time(_CHAR*, _TIME*, _UWORD);
extern "C" void    format_date(_CHAR*, _DATE*, _UWORD);
// Screen and data manipulation
extern "C" void    delete_IO_from_screen(_IO*);
extern "C" _BOOL   change_input(_MY_PICTURE*, _UDWORD, _UDWORD);
extern "C" _BOOL   change_output(_MY_PICTURE*, _UDWORD, _UDWORD);
extern "C" _BOOL   change_io_group(_MY_PICTURE*, _UWORD, _UDWORD);
extern     _BOOL   change_SubPicture_group(_SubPicture*, _UWORD, _UDWORD);
// Scheme processing
extern "C" void    *NewDoScheme(_ANYLST, _DWORD, _UDWORD);
extern "C" void    *DoScheme(void**, _ANYLST, _DWORD, _UDWORD);
extern "C" void    *DoTextScheme(_DWORD, _UWORD, _MEM_VARLABEL*);

extern "C" _UDWORD get_MYSCHEME(_MYSCHEME*);
extern "C" _COLOR  DoColorScheme(_LINKSCHEME *ls, _COLOR defcol);

extern "C" _CHAR   *StupidTextScheme(_DWORD, _UDWORD);
extern _BOOL result_addsub(void*, _RESULT_TYPE, void*, _RESULT_TYPE, _UWORD, void*, void*);

// Functions to access LSE lists - Textlist, IconList, ...
extern "C" _CHAR*  make_parameterstring(_CHAR*, _MEM_VARLABEL*, _BOOL);
extern     _CHAR*  make_parameterstring_sub(_CHAR *src, _MEM_VARLABEL *pmv, _BOOL justlse, _UDWORD percent_x_varno);
extern "C" void    *get_ANYTHING(_ANYTHING*, _TextPuffer*);
extern "C" _CHAR   *get_text(_ANYTHING*, _TextPuffer*, _MEM_VARLABEL*, _BOOL);
extern     _CHAR   *get_text_sub(_ANYTHING *p, _TextPuffer *PT, _MEM_VARLABEL *pmv, _BOOL prefixenable, _UDWORD percent_x_varno);
extern "C" _CHAR   *get_ANYPARA(_ANYPARA*, _TextPuffer*);
extern "C" _CHAR   *ccreate_prefix(_CHAR*, _ANYTHING*, _TextPuffer*, _VARINFO*);
// Diverse functions to get informations about color and attribut
extern "C" _UWORD  GetXSize(_ATTRIB);
extern "C" _UWORD  GetYSize(_ATTRIB);
extern "C" _COLOR  GetBackColor(_COLOR);
extern "C" _COLOR  GetPenColor(_COLOR);
extern "C" _COLOR  InvertColor(_COLOR);
extern "C" _COLOR  MakeColor(_COLOR, _COLOR);
extern "C" _UWORD  get_framewidth(_FRAMETYPE, _ATTRIB);
extern "C" _UWORD  get_frameheight(_FRAMETYPE, _ATTRIB);
extern "C" _BOOL   is_invisible(_COLOR, _ATTRIB);
// Functions to calculate a scroll area
extern "C" _POSRESULT calculate_position(_POSITION*, _POSITION*, _EVENT*, _ROOM*);
extern "C" _POSRESULT calculate_v_position(_POSITION*, _EVENT*, _UWORD);
extern "C" _POSRESULT calculate_h_position(_POSITION*, _EVENT*, _UWORD);
// Diverse functions
extern     void    DrawRound(_ROOM*, _ATTRIB, _FRAMETYPE, _COLOR, _COLOR);
extern "C" void    DrawButton(_ROOM*, _ATTRIB, _FRAMETYPE, _COLOR, _COLOR);
extern "C" void    Debug(_DWORD, _UWORD, _UWORD=0);
extern "C" void    AddZoom(_ZOOM*, _ZOOM*);
extern "C" void    AddOffset(_DOT*, _DOT*, _ZOOM*);
extern "C" _UWORD  TxtWidthFont(void*, _UWORD, _UWORD, _FONT);
extern "C" _KOORD  MakeKoordX(_KOORD);
extern "C" _KOORD  MakeKoordY(_KOORD);
extern "C" _UWORD  IfNecHideMouseCursor(_ROOM*);
extern "C" void    IfNecShowMouseCursor(_UWORD);
extern "C" void    ShowMouseCursor(void);
extern "C" void    HideMouseCursor(void);
extern "C" _DWORD  absolut(_DWORD);
extern "C" void    out_zoom_BMP(_ROOM*, _BMP*, _ATTRIB);
extern "C" void    out_part_BMP(_ROOM*, _BMP*, _ATTRIB);
extern "C" void    flash_memorize_zoom_gfx(_ROOM*, _ASCII*, _BOOL);
extern "C" void    flash_memorize_gfx(_KOORD, _KOORD, _ASCII*);
extern "C" void    ClrScr(_COLOR);
#ifdef UC_ABANDONED
extern "C" void    start_REPEAT(_REPEAT*, _TIP);
extern "C" _BOOL   run_REPEAT(_REPEAT*);
#endif
extern "C" _BOOL   do_LSEFUNCT(_LSEFUNCT*, _Window*);
extern     _BOOL   do_LSEFUNCT_lrm(_LSEFUNCT*, _Window*, _BOOL);
extern "C" void    frame(_KOORD, _KOORD, _KOORD, _KOORD, _UWORD, _ATTRIB, _COLOR);
extern "C" void    round_frame(_KOORD, _KOORD, _KOORD, _KOORD, _UWORD, _ATTRIB, _COLOR);
extern "C" void    color_course(_ROOM*, _WORD, _COLOR, _COLOR, _ATTRIB);
extern "C" _BOOL   get_editor_state(_MY_PICTURE*);
extern "C" _UDWORD get_editor_varno(_MY_PICTURE*);
extern "C" void    rotate(_DOT*, _DWORD);
extern "C" void    selfmade_indicator(_ROOM*, _DWORD, _COLOR, _DOT*, _UWORD);
extern "C" void    indicator(_ROOM*, _DWORD, _COLOR);
extern "C" void    FatalError(const _ASCII*);
extern "C" void    delete_eventbuffer(void);
extern "C" _ASCII_BUFFER* get_label_buffer(_ANYLST list);
extern "C" _UDWORD get_no_by_label(_ASCII*, _ANYLST, _UWORD);
extern "C" _LSEMENU* get_LSEMENU(_UDWORD);

extern "C" _BOOL   out_BUTTON(_DOT*, _BOOL, _BOOL);
extern     _BOOL   lookup_STACK(_STACK*, _PIC_COMMAND);
extern     _BOOL   lookup_LSEFUNCT(_LSEFUNCT*, _PIC_COMMAND);
extern "C" void    OutTextIconXY(_KOORD, _KOORD, void*, _ATTRIB, _COLOR, _UWORD);
extern     void    GetPictureSize(_ROOM*);
extern "C" _BOOL   find_ROOM(_ROOM*, _ROOM*, _ROOM*, _BOOL);
extern     _BOOL   add_MEM_NO(_MEM_NO*, _UDWORD);
extern     void    make_version_text(_ASCII*, _UDWORD, _UWORD);
extern     _UDWORD memorysize(_UWORD, _UWORD);
extern "C" _UWORD  MenuList_GetNo(void);
extern "C" void    SystemError(_SYSERROR, _DWORD=0, _CHAR*txt=NULL);
extern     void    text_1000(_ASCII*, _UDWORD);
extern     _UWORD  GetImage2(_IMAGE*, _KOORD, _KOORD, _KOORD, _KOORD);
extern     void    PutImage2(_IMAGE*, _KOORD, _KOORD);
extern     _BOOL   IsRamFile(const _ASCII*);
extern     _ASCII *ClearRamFile(_ASCII*);
extern     _ASCII *MakeRamFile(_ASCII*, _ASCII*);
extern     _UWORD  GetDriveList(_ASCII*);
extern     _FILEXT Textfile_FILEXT(_FILEXT);
extern     _FILEXT GetFiletype(_ASCII*);
extern "C" void    transform_color_to_gray(_UWORD *pcol);
extern      _COLOR transform_to_gray(_COLOR col);
extern "C" void    transform_BMP_to_gray(_BMP *p);
extern "C" _ASCII *StrCutLastTokenAndReturn(_ASCII *dst, _ASCII *dpne);
extern "C" _ASCII *GetLastToken(_ASCII *dpne);
extern "C" _BOOL   FileExist(_ASCII *dpne);
extern "C" void    AddTokenEnd(_ASCII *p);
extern "C" void    DelTokenEnd(_ASCII *p);
extern     void    GetActDateTime(_UDWORD *date, _UDWORD *time);
extern     _UBYTE  make_WOSE(_ATTRIB attrib);
extern     void    DeleteFinalZeroAfterDezimalPoint(_CHAR *ptxt);
extern "C" _BOOL   IsSalamander(void);
extern     _CHAR  *MakeStars(_CHAR *dst, _UDWORD maxno, _CHAR *src);

// T_LSEOSZ.CPP **********************************************************
extern "C" _BOOL   LseOsziAddChannel(_LSEOSZI*, _UDWORD, _ASCII*, _UWORD, _DWORD, _DWORD, _COLOR);
extern "C" _BOOL   LseOsziConfigChannels(_LSEOSZI*, _UDWORD, _THEURGE*);
extern "C" _BOOL   LseOsziConfigChannelsNew(_LSEOSZI*, _UDWORD, _BOOL, _THEURGE*);
extern "C" _BOOL   LseOsziCyclic(_LSEOSZI*);
extern "C" _DWORD  LseOsziDraw(_LSEOSZI*, _POSITION*, _IO*, _ROOM*, _UWORD, _UWORD, _UWORD);
extern "C" _DWORD  LseOsziDrawNew(_DATIM*, _LSEOSZI*, _POSITION*, _IO*, _ROOM*, _UWORD, _UWORD, _UWORD);
#ifdef USER_DEMAG
extern     _BOOL   switch_momentary_stationary(_VARLABEL *varlabel, _BOOL lrm);
#endif

// T_MAIN.CPP ***************************************************************
extern     void   *opsys_cilget(_ASCII*);
extern "C" _UDWORD GetUniquePlcId(void);
extern     _UWORD  Memory2(void**, _UDWORD);
extern     _BOOL   PushMemColor(_UWORD);
extern     _BOOL   PopMemColor(void);
extern     void    shut_down(void);
extern "C" _BOOL   RTK_End(_BOOL);
extern "C" _BOOL   RTK_Run(void);
extern "C" _BOOL   RTK_Ready(void);
extern "C" _BOOL   RTK_Init(void*, _BOOL);
#ifdef UC_IPC
extern     _UDWORD WatchSleep(void);
extern     void    WatchAwake(_UDWORD);
#endif
extern     _UBYTE  make_reftime_bitno(_UWORD, _UBYTE);
extern     _UWORD  get_reftime(_LSEVARINFO*);
extern "C" _UWORD  VarList_GetSingleUnitNo(_UDWORD);


// T_MEMO.CPP ***************************************************************

// T_MENU.CPP ***************************************************************
extern "C" _CHAR *GetTextFromMenu(_UWORD, _DWORD);

// T_MESBOX.CPP *************************************************************
extern "C" _BOOL   MessageBoxCall(void *text, _UWORD chrsize, _FONT font, _MESSAGEBOXTYP typ, void *pthis, _FVOID_MESSAGEBOXRESULT pfkt);
extern "C" _BOOL   MessageBoxCallEx(void *text, _UWORD chrsize, _MESSAGEBOXTYP typ, void *pthis, _FVOID_MESSAGEBOXRESULT pfkt);
extern "C" void    MessageBoxSetColorBox(_COLOR back, _COLOR text, _COLOR frame);
extern "C" void    MessageBoxSetColorButton(_COLOR back, _COLOR text, _COLOR frame);
extern "C" void    MessageBoxSetSizeBox(_UDWORD width, _UDWORD height);
extern "C" void    MessageBoxSetSizeButton(_UDWORD width, _UDWORD height);
extern "C" void    MessageBoxSetFrame(_FRAMETYPE box, _FRAMETYPE button);
extern "C" void    MessageBoxSetFontScheme(_ASCII *fontschemename);

// T_MOVE.CPP ***************************************************************
extern     void    add_MY_PICTURE(_MY_PICTURE *p, _KOORD x, _KOORD y);
extern "C" void    move_MY_PICTURE(_MY_PICTURE*, _KOORD, _KOORD, _ATTRIB);
extern     void    make_size_PICTURE(_PICTURE *p);
extern     void    flip_MY_PICTURE(_MY_PICTURE*, _KOORD, _KOORD, _KOORD);

// T_OBJ.CPP ****************************************************************
extern "C" _PICTURE* ObjectList_Get_PICTURE(_UWORD);
extern "C" _UDWORD   ObjectList_Add(_ASCII *label, _MY_PICTURE *mypic);

// T_OFFLINE.CPP ************************************************************
extern     _DWORD  GetPLCState(_COMPORT *cp);
extern     _ASCII *GetPLCStateText(_DWORD plcstate);
extern     void    offlinewindow_ready(_MY_PICTURE *mp);
extern     void    offlinewindow_run(_MY_PICTURE *mp, _EVENT *pe);
extern     void    offlinewindow_start(_MY_PICTURE *mp, _BOOL firsttime);

// T_OUTIO.CPP **************************************************************
extern     _DWORD  crc_numeric(_RESULT*, _VARIABLE*);
extern "C" _CRC32  crc_string(void*, _UWORD);
#ifdef USER_HIRSCH
extern "C" _COLOR  AccessColor(_UDWORD access, _LINKSCHEME *colscheme, _COLOR defcol, _BOOL is_button);
#endif
extern "C" void out_triangle(_ROOM *pr, _COLOR color);

// T_OVRLOD.CPP *************************************************************
extern "C" _BOOL overload(_UDWORD*, _UDWORD);
extern "C" _BOOL overload_VARLABEL(_VARLABEL*, _VARLABEL*);
extern "C" _BOOL overload_PICTUREMASK(_PICTUREMASK*, _VARLABEL*, _BOOL change_ctrl_type = _FALSE);
extern "C" _BOOL change_overload_PICTUREMASK(_PICTUREMASK*, _VARLABEL*, _VARLABEL*);
extern "C" _BOOL summary_overload_PICTUREMASK(_PICTUREMASK*, _UWORD, _VARLABEL*);
extern "C" _BOOL summary_overload_PICTURE(_UDWORD, _VARLABEL*);
#ifdef USER_DEMAG
extern "C" _BOOL summary_overload_PICTUREMASK_absolute(_PICTUREMASK*, _UWORD, _VARLABEL*);
#endif
extern     _BOOL overload_MEM_LINEINFO(_MEM_LINEINFO*, _VARLABEL*);
extern     _BOOL overload_LINEINFO(_LINEINFO*, _VARLABEL*);
extern     _BOOL overload_LSEMENU(_LSEMENU*, _VARLABEL*);
extern     void  overload_COL_PICTXT(_COL_PICTXT *p, _VARLABEL *ov);
extern     void  overload_COL_RECT(_COL_RECT *p, _VARLABEL *ov);
extern     void  overload_BUTTON(_BUTTON *p, _VARLABEL *ov);
extern     _BOOL overload_STACK(_STACK *ps, _VARLABEL *ov);


// T_PAL.CPP ****************************************************************
extern "C" _COLOR MakeColorFromIndex(_UWORD);
extern "C" void   Make_RGB(_RGB*, _COLOR);
#ifdef UC_NEWCOLORPALETTE
extern "C" void   SetNewColorPalette(_UBYTE *prgb, _UDWORD startindex, _UDWORD anzahl);
#endif

// T_PARWIN.CPP *************************************************************
extern "C" _BOOL XChangeParaWinObject(_UWORD, _UDWORD, _BOOL, _UWORD);

// T_PICADD.CPP *************************************************************
extern "C" _BOOL  add_rectangle(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD overload);
extern "C" _BOOL  add_button(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD stschemeno, _UDWORD fktno_press, _UDWORD fktno_release, _UDWORD varno, _UDWORD imageno_press, _UDWORD imageno_release, void *text_press, void *text_release, _UWORD chrsize, _BOOL act_momentary, _UDWORD accessno, _UDWORD hotkeycode, _UDWORD overload, _DWORD setval, _DWORD resetval, _ATTRIB imagealign);
extern "C" _BOOL  add_key(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD imageno, void *text, _UWORD chrsize, _ATTRIB imagealign, _UWORD keycode, _UWORD keycode_shift, _UWORD keycode_altgr=_NOKEY, _UWORD keycode_altgr_shift=_NOKEY);
extern "C" _BOOL  add_text(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD stschemeno, _ANYPARA *pany, void *txt, _UWORD chrsize, _UDWORD overload);
extern "C" _BOOL  add_image(_MY_PICTURE *mp, _ROOM *pr, _UDWORD imageno, _BMP *pbmp);
extern "C" _BOOL  add_output(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD stschemeno, _UDWORD varno, _IO_TYPE iotype, _UDWORD overload);
extern "C" _BOOL  add_input(_MY_PICTURE *mp, _STYLESHEET *pstyle, _ROOM *pr, _UDWORD stschemeno, _UDWORD varno, _IO_TYPE iotype, _UDWORD overload);
extern "C" _BOOL  add_object(_MY_PICTURE *mp, _UWORD no, _ROOM *pr, _UDWORD overload);
extern "C" _BOOL  add_imagescheme(_MY_PICTURE *mp, _ROOM *pr, _UDWORD imageschemeno, _UDWORD stschemeno, _UDWORD varno, _UDWORD overload);
extern "C" _BOOL  add_myio(_MY_PICTURE *mp, _ROOM *pr, _UDWORD stschemeno, _UDWORD varno, _BOOL input, _ASCII *objectname, _UDWORD overload);

// T_PICCAR.CPP *************************************************************
extern     void   action_CAROUSEL(_MOTION *pm, _EVENT *pe);
extern     void   negedge_MEM_KARU(_MEM_KARU *p, _BOOL state);
extern     void   stop_move_CAROUSEL(_MOTION *pm);

// T_PICIDX.CPP *************************************************************
extern "C" _PICINDEX *GetPictureIndex(_HOTKEYLIST*);
extern "C" _BOOL   FreePictureIndex(_PICINDEX*);

// T_PICMEM.CPP *************************************************************

// T_PICPIC.CPP *************************************************************
extern     _UWORD  statescheme_OBJECT(_OBJECT*);
extern "C" _BOOL   is_navigation_key(_UDWORD scancode);

// T_PICSUB.CPP *************************************************************
extern     void    out_RECTANGLE(_RECTANGLE*, _DOT*, _ZOOM*);
extern     void    out_POLYGON(_POLYGON*, _DOT*, _ZOOM*);
extern     void    out_CIRCLE(_CIRCLE*, _DOT*, _ZOOM*);
extern     _ASCII* get_virtname(_UWORD, _PICTUREMASK*);
extern     _BOOL   get_stationary_state(_BOOL *pst, _VARLABEL *pvl, _DWORD setval, _DWORD resetval);
extern     _BOOL   button_release(_VARLABEL *pvl, _BOOL act_momentary, _DWORD setval, _DWORD resetval, _BOOL lrm);
extern     _BOOL   button_press(_VARLABEL *pvl, _BOOL act_momentary, _DWORD setval, _DWORD resetval, _BOOL lrm);
extern     _BOOL   getimage_IO(_IO*);
extern     _BOOL   is_redraw(_ROOM, _DOT*, _ZOOM*);
extern     void    CallUserOverload(_SubPicture*, _UWORD);
extern "C" void    SetCallUserOverload(_FBOOL_UWORD_PT);
extern "C" void    SubPicture_Draw(_ROOM *proom, _SubPicture *pic);
extern "C" void    SubPicture_Run(_ROOM *proom, _SubPicture *pic);
extern "C" _SubPicture* SubPicture_Copy(_SubPicture *pic);
extern "C" void    SubPicture_Free(_SubPicture *pic);
extern "C" void    SubPicture_Overload(_SubPicture *pic, _VARLABEL *vl);
extern "C" void    SubPicture_OverloadSummary(_SubPicture *pic, _UWORD summary, _VARLABEL *vl);
extern "C" _BOOL   add_object(_MY_PICTURE *mp, _UWORD no, _ROOM *pr, _UDWORD varno);
extern "C" _DWORD  add_carousel(_MY_PICTURE *mp, _ROOM *room, _UWORD cino, _CAROITEM *ci, _UDWORD varno_xpos, _BOOL horizontal, _BOOL infinite, _UWORD _dx);
extern "C" _BOOL   add_carousel_scrollbar(_MY_PICTURE *mp, _DWORD cno, _FRAMETYPE frame, _COLOR col_back, _COLOR col_front, _COLOR col_symbol, _COLOR col_topleft, _COLOR col_bottomright);
extern "C" _MOTION* get_carousel(_MY_PICTURE *mp, _DWORD index);
extern "C" _BOOL   get_visible_objectindex(_DWORD *firstindex, _DWORD *lastindex, _MOTION *pmotion);
extern "C" _UDWORD get_pixsize(_MOTION *pmotion, _DWORD objectindex);


// T_POPSUB.CPP **************************************************************
#ifdef OK_SUBLIST
extern     void    sublist_run(_Window*, _PopUp*, _EVENT*);
extern     void    sublist_line(_Window*, _PopUp*, _UDWORD, _ROOM*, _COLOR, _BOOL);
extern     _BOOL   add_entries_MEM_VKLIST_lse(_MEM_VKLIST*, _UDWORD);
extern "C" _BOOL   add_entries_MEM_VKLIST(_MEM_VKLIST*, _UDWORD);
extern "C" _UDWORD add_MEM_VKLIST(_MEM_VKLIST*, _UDWORD, _UDWORD);
extern "C" _BOOL   del_MEM_VKLIST(_MEM_VKLIST*, _UDWORD);
extern "C" _UDWORD find_real_position(_MEM_VKLIST*, _UDWORD);
extern "C" _BOOL   action_MEM_VKLIST(_MEM_VKLIST*, _UDWORD);
extern "C" _KOORD  out_MEM_VKLIST(_ROOM*, _VKLIST*, _COLOR, _BOOL, _MEM_VKLIST*);
extern "C" _UDWORD popup_get_actual_position(_PopUp*);
extern "C" _BOOL   is_action_MEM_VKLIST(_MEM_VKLIST*, _UDWORD);
extern "C" _UDWORD find_line(_MEM_VKLIST*, _UDWORD);
extern "C" void    open_entire_MEM_VKLIST(_MEM_VKLIST*);
extern "C" void    close_entire_MEM_VKLIST(_MEM_VKLIST*);
extern     _UDWORD get_parent_no(_MEM_VKLIST*, _UDWORD);
#endif

// T_POPUP.CPP ***************************************************************
extern void   calculate_window(_MENUCALCUL*, _MENUINFO*, _POSITION*);
extern void   VARLABEL_to_text(_ASCII*, _VARLABEL*);

// T_POS.CPP *****************************************************************
//#ifdef ZOOM_KM
//extern _ZOOM  do_kmz(_ZOOM*, _ZOOM*);
//#endif
extern     _BOOL  lookup_input_in_object(_IO*, _MY_PICTURE*);
extern     _BOOL  lookup_SEEKELEMENT(_MY_PICTURE*, _KOORD, _KOORD);
extern     _BOOL  is_same_summary(_MY_PICTURE*, _UWORD, _UWORD, _UWORD, _UWORD);
extern     _BOOL  position_SEEKELEMENT(_ROOM*, _MY_PICTURE*, _UWORD, _DOT*, _ZOOM*);
extern     _BOOL  out_SEEKELEMENT(_MY_PICTURE*, _UWORD, _EDITSTATE, _DOT*, _ZOOM*);
extern     _BOOL  access_SEEKELEMENT(_MY_PICTURE*, _UWORD, _BOOL, _SELECTOR);
extern     _BOOL  summary_SEEKELEMENT(_MY_PICTURE*, _UWORD, _BOOL, _Picture*);
extern     _BOOL  load_EDITOR(_EDITOR*, _MY_PICTURE*, _UWORD, _DOT*, _ZOOM*);
extern     _UWORD layer_SEEKELEMENT(_MY_PICTURE*, _UWORD);
extern     _BOOL  is_SEEKELEMENT(_MY_PICTURE*, _UWORD);
extern     _BOOL  is_button_cursor(_BUTTON*);
extern     _BOOL  is_SEEKELEMENT_available(_MY_PICTURE*, _UWORD, _SELECTOR);
extern     _BOOL  action_SEEKELEMENT(_MY_PICTURE*, _UWORD, _BOOL, _Window*, _BOOL);
extern     _BOOL  hotkey_SEEKELEMENT(_MY_PICTURE*, _Window*, _BOOL, _EVENT*);
extern     _BOOL  is_any_input_accessible(_MY_PICTURE*, _SELECTOR);
extern     _BOOL  find_input_by_varno(_UWORD*, _MY_PICTURE*, _UDWORD varno);
extern     void   new_IO_SEEKELEMENT(_MY_PICTURE*, _UWORD);
extern     _INPUT *p_input_SEEKELEMENT(_MY_PICTURE*, _UWORD, _BOOL);
extern     void   give_editorcolor(_IO*, _EDITSTATE);
#ifdef USER_BILLION // eingabefarbe invers
extern     _BOOL  color_billioneditor(_IO*, _EDITSTATE);
extern "C" _BOOL  InvertInputColor(_UDWORD);
#endif
extern     _BOOL  valid_SELECTOR(_SELECTOR, _SELECTOR);

// T_PRJ.CPP *****************************************************************
extern "C" _UWORD         Prj_GetNo(void);
extern "C" _PRJ_ENTRY     *Prj_GetEntryByNo(_UWORD, _ENTRYTYPE);
extern "C" _PRJ_ENTRY     *Prj_GetEntry(_UWORD);
extern "C" _UWORD         Prj_GetLanguageNo(void);
extern "C" _ASCII_BUFFER  *Prj_GetLanguages(void);
extern "C" _LANGUAGE_INFO *Prj_Get_LANGUAGE_INFO(void);
extern "C" _UDWORD        Prj_GetLseProjectCrc(void);
extern "C" _PROJECT       *Prj_GetProjectInfo(void);

// T_RENDER.CPP **************************************************************
extern "C" _UDWORD SetTextRenderEngine(_UDWORD);
extern "C" void    OutTextLse(_KOORD, _KOORD, void*, _ATTRIB, _COLOR, _UWORD);
extern "C" _UWORD  StrWidthLse(void*, _UWORD);
extern "C" _UWORD  ChrWidthLse(_UNI, _UWORD);

// T_SAVE.CPP ****************************************************************

// T_SCAN.CPP ****************************************************************
extern "C" _UWORD ScanBar_Add(_ASCII*, _DWORD);
extern "C" void   ScanBar_Run(_UWORD, _DWORD);
extern "C" void   ScanBar_End(_UWORD);

// T_SCHEMA.CPP **************************************************************
extern "C" _ANYPARA *get_textschemeentry(_UWORD, _VARLABEL*);
extern     _UDWORD  add_hardcoded_fontscheme(_ASCII *fontschemename, _ASCII *fontname, void *fontdata, _ATTRIB attrib);
extern "C" _UDWORD  ImageScheme_Add(_ASCII *label, _SCHEME *pscheme);

// T_SCROLL.CPP **************************************************************
extern "C" void   Start_SCROLL(_SCROLL*, _BOOL);
extern "C" void   Ready_SCROLL(_SCROLL*, _BOOL);
extern "C" void   Run_SCROLL(_SCROLL*, _EVENT*);
extern "C" void   End_SCROLL(_SCROLL*);
extern "C" void   GetLinePosition_SCROLL(_SCROLL*, _UDWORD, _ROOM*);
extern "C" void   OutLine_SCROLL(_SCROLL*, _UDWORD, _UDWORD, _BOOL, _BOOL);
extern "C" void   ActBeam_SCROLL(_SCROLL*, _BOOL);
extern "C" void   Beam_SCROLL(_SCROLL*, _UDWORD, _UDWORD, _BOOL);
extern "C" void   OutPage_SCROLL(_SCROLL*);
extern "C" void   OutPageBeam_SCROLL(_SCROLL*, _UDWORD);
extern "C" void   Draw_SCROLL(_SCROLL*, _BOOL);
extern "C" void   ScrollUp_SCROLL(_SCROLL*);
extern "C" void   ScrollDown_SCROLL(_SCROLL*);
extern "C" _UWORD GetPosition_SCROLL(_SCROLL*);
extern "C" void   level_draw(_LEVEL*, _BOOL, _BOOL, _BMP *pbmp=NULL, _FRAMETYPE frame=_3DFRAME);
extern "C" void   level_run(_LEVEL*, _DWORD, _DWORD, _BOOL, _BMP *pbmp=NULL, _FRAMETYPE frame=_3DFRAME);
extern "C" void   level_out(_LEVEL*, _UDWORD, _DWORD, _BOOL, _BMP *pbmp=NULL, _FRAMETYPE frame=_3DFRAME);
extern "C" _BOOL  do_HID_SCROLL(_EVENT*, _LEVEL*, _POSITION*, _LEVEL*, _POSITION*, _ROOM*, _UWORD, _UWORD);
extern "C" _BOOL  track_HID_SCROLL(_EVENT*, _LEVEL*, _POSITION*, _ROOM*, _UWORD);
extern "C" _BOOL  is_HID_SCROLL(_EVENT*, _SCROLL*);
extern "C" void   chk_position_SCROLL(_SCROLL*);
extern "C" _UWORD calculate_pageheight(_POSITION*, _ROOM*, _UBYTE);
extern     void   level_triangle(_ROOM *pr, _UWORD dir, _FRAMETYPE frame, _COLOR color);
extern     void   ScrButton(_ROOM *proom, _BMP *pbmp, _ATTRIB att, _FRAMETYPE frame, _COLOR color, _COLOR frame_color, _UWORD dir);
extern     _BOOL  IsSlimDesign(const _FRAMETYPE frametype);
extern     void   GetSlimSliderDecentRoom(_ROOM *room, _ROOM *proom, _BOOL horizontal, _BOOL pguppgdown);

// T_SCRSVR.CPP *************************************************************

// T_SEEK.CPP ***************************************************************
extern _BOOL  seek_vertical(_UWORD*, _MY_PICTURE*, _UWORD, _BOOL, _UWORD, _BOOL, _DOT*, _ZOOM*, _SELECTOR);
extern _BOOL  seek_horizontal(_UWORD*, _MY_PICTURE*, _UWORD, _BOOL, _UWORD, _BOOL, _DOT*, _ZOOM*, _SELECTOR);
extern _BOOL  position_access_SEEKELEMENT(_ROOM*, _MY_PICTURE*, _UWORD, _DOT*, _ZOOM*, _SELECTOR);
extern _BOOL  seek_shuttle(_UWORD*, _MY_PICTURE*, _BOOL, _UWORD, _DOT*, _ZOOM*, _SELECTOR);

// T_SETT.CPP ***************************************************************
extern "C" _BOOL   IpcSettings_Save(void);
extern "C" _UDWORD get_delay_screensave(void);
extern "C" void    set_delay_screensave(_UDWORD);
extern "C" _UWORD  get_actual_language(void);
extern "C" void    set_actual_language(_UWORD);
extern "C" _BOOL   get_commasign(void);
extern "C" void    set_commasign(_BOOL);
extern "C" _BOOL   push_commasign(_BOOL state);
extern "C" void    pop_commasign(_BOOL state);
extern "C" _BOOL   get_americandateformat(void);
extern "C" void    set_americandateformat(_BOOL);
extern "C" _BOOL   get_americantimeformat(void);
extern "C" void    set_americantimeformat(_BOOL);
extern "C" _BOOL   get_seek_algorithm(void);
extern "C"  void   set_seek_algorithm(_BOOL);
extern "C" _BOOL   get_directiononenter(void);
extern "C"  void   set_directiononenter(_BOOL);
extern "C" _BOOL   get_cursorwraphorizontal(void);
extern "C"  void   set_cursorwraphorizontal(_BOOL);
extern "C" _BOOL   get_cursorwrapvertical(void);
extern "C"  void   set_cursorwrapvertical(_BOOL);
extern "C"  _BOOL  get_noexterncom(void);
extern "C"  void   set_noexterncom(_BOOL);


// T_SETUP.CPP **************************************************************
extern "C" _COLOR GetEditColorBack(_EDITSTATE);
extern "C" _COLOR GetEditColorFrame(_EDITSTATE);
extern "C" _COLOR GetEditColorTxt(_EDITSTATE);
extern "C" _COLOR GetEditColorCursor(_EDITSTATE);
extern "C" _DATA* GetTargetNotice(void);
extern "C" _BOOL  GetEditorIncDec(void);
extern "C" _BOOL  GetHotkeysInMenu(void);
extern "C" _SOFTKEYSETUP* Get_SOFTKEYSETUP(void);
extern "C" _DEFKEYPAD* Get_DEFKEYPAD(void);
extern "C" _SCREENSAVER* Get_SCREENSAVER(void);
extern "C" _BUBBLEINFO* Get_BUBBLEINFO(void);
extern "C" void   Set_BUBBLEINFO_delaytime(_DWORD);
extern "C" void   Set_BUBBLEINFO_remaintime(_DWORD);
extern "C" void   SetDoubleClickOption(_BOOL);
extern "C" _BOOL  GetDoubleClickOption(void);
extern "C" void   SetChkBoxImages(_UDWORD unchecked, _UDWORD checked);
extern "C" void   SetRadioBoxImages(_UDWORD unchecked, _UDWORD checked);
extern "C" _BOOL  SetWordWrap(_BOOL newstate);

// T_SLIDER.CPP *************************************************************
extern     _BOOL  out_SLIDER(_IO *pio, _RESULT *pres, _VARIABLE *pv);
extern     _DOIT  getevent_SLIDER(_EVENT *pev, _EDITOR *pedi);
extern     _DOIT  killfocus_SLIDER(_EDITOR *pedi);
extern     _DOIT  getfocus_SLIDER(_EDITOR *pedi, _EVENT *pev);
extern     _BOOL  getvalue_SLIDER(_RESULT *res, _IO *pio);
extern     void   KillAllSliderFocus(void);
extern     void   InitAllSlider(void);

// T_SOFT.CPP ***************************************************************
extern "C" _SINGLESOFT *Get_SINGLESOFT(_IMA, _UWORD, _UWORD, _UWORD);

// T_STACK.CPP **************************************************************

// T_START.CPP **************************************************************

// T_SYS.CPP ****************************************************************
extern "C" void    StrCpy(void*, _UWORD, const void*, _UWORD);
extern "C" _UWORD  StrLen(const void*, _UWORD);
extern "C" _UWORD  StrChr(void*, _UNI, _UWORD);
extern "C" _UDWORD StrCrc(void *ptxt, _UWORD chrsize);
extern "C" void*   GetCrc32Table();
extern "C" _UDWORD StrXmp(void *pTxt0, void *pTxt1, _UWORD cSize);
extern "C" _UDWORD StringCountChr(void*, _UWORD, _UWORD);
extern "C" _UDWORD AMBDC(_UDWORD, _UDWORD, _UDWORD);
extern "C" _BOOL   CilGet(const _ASCII*, void**);
extern "C" _UDWORD GetRevisionOfClass(char *classname);
extern "C" void    CircleDot(_DOT*, _UWORD, _UWORD);
extern "C" _BOOL   AlarmDisplayAddEntry(_MEM_SINGLEALARM*, _SINGLEALARM*);
extern "C" _ASCII *AlarmDPNE(_MEM_SINGLEALARM*, _ASCII*);
extern "C" void    Lse_Reset(_BOOL restart);
extern "C" _UDWORD ExternPing(_ASCII *ipaddress);
extern "C" void    Mutex(_BOOL, _UWORD);
extern "C" _BOOL   heap_size(_UDWORD*, _UDWORD*);
extern "C" _UDWORD get_microtime(void);
extern "C" void    MakeLogfile1Entry(_ASCII *txt, _DWORD p1, _DWORD p2);
extern "C" _UDWORD GetTCPIPAddressFromDNS(_ASCII*);
extern "C" _BOOL   GetTCPIPAddress(_UDWORD, _UDWORD*, _UDWORD*, _UDWORD*);
extern "C" _DWORD  LseTxCommand(_UDWORD bef, _UDWORD len, void *pdata, void *pcd, void **presult, _SVRTYP svrtyp);
extern "C" _BOOL   _Screen_GetAccess(_WHOAMI *pwai);
extern "C" _BOOL   _Screen_Create(_NEWSCREENPROP *psp, _WHOAMI *pwai);

// T_TASK2.CPP **************************************************************
extern "C" void    RTK_Thread2(_Thread2CmdLine *pline);
extern "C" void    init_Thread2CmdLine(_Thread2CmdLine *pline);
extern "C" _BOOL   Thread2_AddAsyncMethod(_FVOID_PT_UDWORD usermethod, void *pthis, void *puserdata, _UDWORD userdatalength);
extern "C" _BOOL   Thread2_AddCmd(_Thread2CmdLine *cmd);

// T_TEXT.CPP ***************************************************************
extern     _UDWORD make_crc_ASCII_BUFFER(_ASCII_BUFFER*);
extern     _BOOL   add_ASCII_BUFFER(_ASCII_BUFFER*, _ASCII*);
extern     _ASCII  *get_text(_FAST_ASCII_BUFFER*, _UDWORD);
			  _UDWORD findtext_ASCII_BUFFER(_ASCII_BUFFER*, _ASCII*);
extern "C" _ASCII  *gettext_ASCII_BUFFER(_ASCII_BUFFER*, _UDWORD);
extern "C" _CHAR   *gettext_TEXT_BUFFER(_TEXT_BUFFER*, _UDWORD);
extern "C" _UDWORD add_TEXT_BUFFER(_TEXT_BUFFER*, void*, _UWORD);
extern     _BOOL   calculate_offset_TEXT_BUFFER(_TEXT_BUFFER*);
extern "C" _CHAR*  textpuffer_getPtr(_ANYLST);
extern "C" _DWORD  textpuffer_getSize(_ANYLST);
extern "C" _UDWORD textpuffer_getNo(_ANYLST);
extern "C" _CHAR*  textpuffer_getText(_ANYLST, _UDWORD);
extern     void*   GetNullstring(void);

// T_THEME.CPP **************************************************************
extern     _BOOL   draw_THEME(_ROOM*, _FRAMETYPE, _ATTRIB, _COLOR, _BOOL, _BOOL);
extern "C" void    draw_THEME_partial(_ROOM*, _ROOM*, _FRAMETYPE, _ATTRIB, _COLOR);
extern     _BOOL   draw_THEME_part(_ROOM*, _FRAMETYPE, _ATTRIB, _COLOR, _BOOL, _BOOL);
extern     _BOOL   shadow_THEME(_ROOM*, _FRAMETYPE);
extern     _BOOL   clip_THEME(_ROOM*, _FRAMETYPE);
extern     _BOOL   inlet_THEME(_ROOM*, _ROOM*, _FRAMETYPE);
extern     _BMP*   prepaire_bmp_treashold(_BMP*, _BMP*, _COLOR);
extern     _BOOL   draw_THEME_cancle(_ROOM*, _FRAMETYPE, _BOOL);
extern     _BOOL   is_THEME_cancle(_DOT*, _FRAMETYPE, _ROOM*, _CHKBIT*, _MYSCHEME*);
extern     void    theme_minsize(_FRAMETYPE frametype, _ATTRIB attrib, _UWORD *xx, _UWORD *yy);
extern "C" _FRAMETYPE GetFrameType(_FRAMETYPE frame1, _FRAMETYPE frame2, _FRAMETYPE frame3, _FRAMETYPE defaultframe);
extern "C" _BOOL   has_theme_cancel(_UDWORD frameid);

// T_TOOL.CPP ***************************************************************
extern "C" _TOOLDATA* get_TOOLDATA(void);
extern "C" _BOOL   ToolCatTeach(const _ASCII*, _TOOLSTATE, _STATISTIC*);
extern "C" _BOOL   save_toolcat(const _ASCII*, _UDWORD*);
extern "C" _BOOL   load_toolcat(const _ASCII*, _UDWORD*);

// T_TREE.CPP ***************************************************************
extern _UDWORD   add_TREEITEM(_TREE *p, _UDWORD data0, _UDWORD data1); // add treeitem
extern _BOOL     add_SUBTREE(_TREEITEM *p, _TREE *subtree);    // add subtree to treeitem
extern _TREEITEM *get_TREEITEM(_TREE *p, _DWORD line);
extern _BOOL     is_action_TREE(_TREE *p, _DWORD line);
extern _BOOL     is_action_TREEITEM(_TREEITEM *pti);
extern _BOOL     do_action_TREE(_TREE *p, _DWORD line);
extern _BOOL     do_action_TREEITEM(_TREEITEM *pti);
extern _TREE     *find_TREE(_TREE *p, _DWORD line, _DWORD *treeline);
extern _DWORD    find_SCOPELINE(_TREE *p, _DWORD line);
extern _KOORD    out_TREE(_TREE *p, _ROOM *r, _DWORD line, _COLOR col, _BOOL isnode, _UWORD treesize);
extern _TREE     *del_TREEITEM(_TREE *p, _DWORD line);
extern _UDWORD   visuallines_TREE(_TREE *p);
extern _UDWORD   markedlines_TREE(_TREE *p);
extern _BOOL     get_TREESCOPE(_TREESCOPE *scope, _TREE *p, _DWORD line);
extern _BOOL     scan_TREE(_TREE *p, _UDWORD flag, _FBOOL_PT_PT_PT pfkt, void *puser1, void *puser2);
extern void      set_flag(_TREEITEM *pti, _UDWORD attrib);
extern void      reset_flag(_TREEITEM *pti, _UDWORD attrib);

// T_URGE.CPP ***************************************************************
extern     _BOOL   check_THEURGE(_THEURGE*);
extern     _DOIT   getevent_THEURGE(_EVENT*, _EDITOR*);
extern     _DOIT   killfocus_THEURGE(_EDITOR*);
extern     _DOIT   getfocus_THEURGE(_EDITOR *pedi, _EVENT *pev);

// T_USER.CPP ***************************************************************
extern     _BOOL   chk_ACCESS(_UWORD, _BOOL);      // interface: check access
extern     _BOOL   access_input(_INPUT*, _ACCESSFAILED*);                // interface: access
extern     _BOOL   DrawBubbleHelp(_EDITOR*);
extern     _BOOL   popup_call(_MENU*, _UWORD*);  // user interface open popup
extern     void    input_is_saved(_INPUT*, _RESULT*);       // user interface
extern     void    found_something_by_hid(_EVENT *pe, _FOUNDHID hp);
extern     _BOOL   user_station(_UWORD no, _COMDEF *cd, _BOOL *online_watch); 
extern     _BOOL   access_button(_BUTTON*, _BOOL, _SELECTOR);
extern "C" _BOOL   recalculate(_RESULT*, _VARIABLE*, _BOOL);
extern "C" void    zero_screensaver(void);
extern "C" void    reset_screensaver(void);
extern "C" void    Beep(_WHOBEEP);

// T_VARIA.CPP **************************************************************
extern _BOOL format_BINARY(_RESULT*, _VARINFO*, void*);
extern _BOOL format_FDATE(_RESULT*, void*);
extern _BOOL format_FTIME(_RESULT*, void*);
extern _BOOL format_FSTRING(_RESULT*, void*);
#ifdef UC_COMPLEX_SERVER
extern _BOOL format_ELEMENT(_VARINFO*, void*);
#endif
extern _BOOL format_BIT(_RESULT*, _VARINFO*, void*);
extern _BOOL set_format_BINARY(_RESULT*, _VARINFO*, void*);
extern _BOOL set_format_FDATE(_RESULT*, void*);
extern _BOOL set_format_FTIME(_RESULT*, void*);
extern "C" _BOOL  VarList_GetVarInfo(_VARINFO*, _UDWORD);
extern "C" _BOOL  VarList_GetSingleVariable(_VARIABLE*, _UDWORD);
extern "C" _BOOL  VarList_GetVariable(_VARIABLE*, _VARLABEL*);
extern "C" _BOOL  VarList_GetSystemData(_RESULT*, _VARIABLE*, _VARLABEL*, _BOOL);
extern "C" _UWORD VarList_GetVariableSize(_VARINFO*);
extern "C" _BOOL  VarList_SetSystemData(_RESULT*, _VARIABLE*, _VARLABEL*, _BOOL);
extern "C" _BOOL  VarList_GetValueByNo(_UDWORD, _IVALUE*, _BOOL);
extern "C" _BOOL  VarList_GetValue(_VARLABEL*, _IVALUE*, _BOOL);
extern "C" _BOOL  VarList_SetValue(_VARLABEL*, _IVALUE);
extern "C" _BOOL  VarList_SetValueByNo(_UDWORD, _IVALUE);
extern "C" _UWORD VarList_GetCompleteNoDigits(_VARINFO*);
extern "C" _BOOL  VarList_GetLabel(_ASCII*, _UBYTE*, _UDWORD);
extern "C" _UDWORD VarList_GetVarNo(_ASCII*);
extern "C" _UDWORD VarList_GetVarNoStation(_ASCII*, _UWORD);
extern "C" _BOOL  VarList_VOWPRUIC(_UWORD, _UWORD);
extern "C" _BOOL  VarList_GetParameter(_UDWORD, _UWORD);
extern "C" _CHAR* VarList_GetText(_UDWORD);
extern "C" _UWORD VarList_GetDigits(_VARINFO*);
extern "C" _BOOL  VarList_GetSign(_VARINFO*);
extern "C" _DWORD VarList_GetDezPoint(_VARINFO*);
extern "C" _UDWORD VarList_GetNo(void);
extern "C" _CHAR* VarList_GetTextByNo(_UDWORD, _UWORD);
//extern "C" _UDWORD VarList_CreateObject(_UWORD, _ASCII*, _ASCII*);
//extern "C" _BOOL  VarList_ConnectObject(_UWORD, _ASCII*, _ASCII*);
//extern "C" _BOOL  VarList_StartObject(_UWORD, _ASCII*, _UDWORD, _UWORD);
//extern "C" _BOOL  VarList_DeleteObject(_UWORD, _ASCII*);
//extern "C" _BOOL  VarList_VisualizeObject(_UWORD, _ASCII*, _ASCII*);
extern "C" _UNIT* VarList_GetUnitInfo(_UWORD);
extern "C" _UWORD VarList_GetUnitNo(_ASCII*);
extern "C" _BOOL  VarList_SetValueSub(_RESULT*, _VARIABLE*);
extern "C" _UDWORD VarList_GetVarNoByID(_UDWORD, _UWORD, _BOOL);
extern "C" _BOOL  VarList_GetVarIdChanged(void);
extern "C" _BOOL  VarList_GetVarSymbol(_VARSYMBOL**, _UDWORD);
extern "C" void   VarList_ReGetID(_BOOL);
extern "C" _BOOL  chk_server_XCHKBIT(_UDWORD);
extern "C" _UWORD chk_server_stscheme(_UDWORD);
extern "C" _UDWORD VarList_GetVarNoByCRC(_CRC32);
extern "C" _LSEVARINFO *VarList_Get_LSEVARINFO(_UDWORD);
#ifdef OLDSTYLE_CAN
extern "C" _BOOL VarList_ChangeBaseAddress(_UDWORD, _UDWORD);
#endif
extern "C" _BOOL   VarList_CheckHostname(_BOOL, _BOOL, _BOOL);
extern "C" _ASCII *VarList_GetHostName(_ASCII*, _UWORD);
extern "C" _ASCII *VarList_GetHostNameByVarNo(_UDWORD);
extern "C" _ASCII *VarList_GetHostNameByCrc(_CRC32);
extern "C" _UDWORD VarList_GetVarNoByHostName(_ASCII*);
extern "C" void MakeMinMax(_RESULT*, _RESULT*, _BOOL);
extern "C" _BOOL VarList_GetOnlyAscii(_VARINFO*);
extern "C" _UDWORD VarList_FindRoot(_UDWORD);
extern     _UDWORD find_root(_MEM_VARSYMBOL*, _UDWORD);
extern     void  transform_RESULT(_RESULT *pres, _RESULT_TYPE restyp);
/*
extern     _VARTYPE get_VARTYPE(_VARTYPE);
extern     _BOOL get_GLOBAL(_VARTYPE);
extern     _BOOL get_ELEMENT(_VARTYPE);
*/

// T_WATCH.CPP ***********************************************************
extern "C" _BOOL ServerWatch_Add(_UDWORD svrno, void *ptrthis, void *ptrfkt);
extern "C" _BOOL ServerWatch_Del(_UDWORD svrno);


// T_WIN.CPP *************************************************************
extern "C" _BOOL InsertCmd(_PIC_COMMAND, _DWORD, _DWORD);
extern "C" _BOOL AddCmd(_PIC_COMMAND, _VARLABEL*, _VARLABEL*);
extern "C" _BOOL SaveEditor(void);
extern "C" _BOOL CloseEditor(void);
extern "C" void  picmemo_free(void);
extern "C" _UWORD PicMemo_GetNo(void);
extern "C" _UWORD PicMemo_Get(void);
extern "C" _BOOL set_keytable(void*);
extern "C" _BOOL set_keymode(_UDWORD, _TIP);
extern     _BOOL do_CMD_SET(_STACK_LINE*, _BOOL);

extern "C" void    do_start(void);
extern "C" _BOOL   RealComp(_REAL*, _REAL*, _UWORD);
extern "C" void    RealCalc(_REAL*, _REAL*, _REAL*, _UWORD);
extern "C" _DWORD  ftol_(_REAL*);
extern "C" _IVALUE format_value_st(_REAL*, _VARINFO*, _UWORD);
extern "C" void    dword2real(_REAL*, _DWORD);
extern "C" void    RefreshJustTopWindow(_BOOL);
extern "C" _BOOL   DrawPicWin(_IMA, _UWORD, _UWORD);
extern "C" _BOOL   DrawPicWinOffset(_IMA, _UWORD, _UWORD, _DOT*);


// T_WIN_MOVE.CPP ********************************************************
extern     _BOOL   WindowMoveProjectReady(void);
extern     _BOOL   WindowMoveRun(void);
extern     _BOOL   WindowMoveStart(_Window *PWin, _EVENT *pe, _ROOM *pr);
extern     _BOOL   WindowMoveGetOffset(long *pdx, long *pdy, _IMA ima, unsigned long no);


#define MEMORY_DEF     0  // default memoryblockkennung
#define MEMORY_LME    11  // memoryblockkennung für lme-kernel
#define MEMORY_GRF    12  // memoryblockkennung für graphic zb.putimage, etc...
#define MEMORY_PRG    13  // memoryblockkennung für anwenderprogramm zb.alarme, ramex, memvklist, etc..
#define MEMORY_LSE    14  // memoryblockkennung für lse-kernel
#define MEMORY_LDR    15  // memoryblockkennung für loader


// heap:
// (prg) T_ALARM.CPP: Alarmliste (wie gross und wo soll sie liegen)
//       T_BMP.CPP, T_GIF.CPP, T_JPG.CPP: Load/Save Images
// (prg) T_LANG.CPP: Seperate Textlist (nur Anwender)
// (prg) T_FILE.CPP: File-Öffnen (RAM-File), Erhöhter Speicherbedarf beim Laden vom File (speziell ?MB-Serverlist)
// (prg) T_LIB.CPP: tmp-Ram für Sortierfunktion
//       T_LIB.CPP: Ram für Ringpuffer
// (prg) T_LSEOSZ.CPP: Oszi
// (prg) T_POPSUB.CPP: Verkettete Liste (_MEM_VKLIST)
// (prg) T_SCAN.CPP: ScanBar
//       T_TEXT.CPP: TextPuffer Ascii oder UNIcode
// (prg) T_WATCH.CPP: Serverwatch (nur Anwender)
// User-Speicher bei Alarm, UDC, MyIo muss vom Kernel freigegeben werden, auf welchem heap ist dieser zu finden?
// wie funktioniert Lse mit Lse-Light, wird jede kleine cpu ebenfalls multi-heap haben? wenn ja, wie ist dieser vorkonfiguriert? wenn nein, wie funktioniert lse-light kernel?

#endif // LASAL
#endif // _T_NUC_H
