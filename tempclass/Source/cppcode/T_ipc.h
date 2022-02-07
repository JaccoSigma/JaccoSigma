#ifndef _T_IPC_H
#define _T_IPC_H

// *******************************************************************************
// *******************************************************************************
// ***  user configuration                                                     ***
// *******************************************************************************
// *******************************************************************************

#define UC_UNICODE                             // use unicode instead of asciicode
#define UC_NEW_ALARMSYSTEM                            // use multi hmi alarmsystem
//#define UC_UNREGISTERED_ALARM         // unregistered alarms should work as well
//#define UC_MOUSE                                          // terminal with mouse
//#define UC_MOUSE_AUTODETECT        // autodetection usb-mouse (OpSys >= 0101170)
#define UC_TOUCH                                      // terminal with touchscreen
//#define UC_MULTITOUCH            10          // multitouch with n fingers in use
//#define UC_FLICK_ZOOM                                     // enable zoom gesture
//#define UC_RELEASE_PRESS_FILTER 100          // avoid short injure (ms) of touch
#define UC_BUBBLE_HELP                                // enable use of bubble-help
#define UC_EDITOR_RAMP                         // use ramp for inc/dec editorvalue
#define UC_TRIANGLE_INPUT                           // show triangle at menu-input
#define UC_LINKVL                 4             // links to other servers possible
#define UC_LINKVL_FAST                     // fast links to other servers possible
#define POPUP_SHADOW              6                       // width of popup-shadow
#define _WIN_EDITOR_NUMERIC       0                 // keypad: editor windownumber 
#define _WIN_EDITOR_ANUMERIC      1                 // keypad: editor windownumber
#define _WIN_EDITOR_HEX       31998                 // keypad: editor windownumber
#define _WIN_EDITOR_ANUMFILEX 31999   // keypad: editor windownumber _FileExplorer
//#define UC_DYN_EDIT_POSITION                     // dynamic editorwindowposition
//#define UC_DELETE_REFLIST                  // delete dyn.refreshlist at overflow
#define UC_ROTATE                // use GraficShadowFrameBuffer (OpSys >= 0101079)
//#define UC_RESETRECONNECT       // reset and restart project at reconnect to plc
#define UC_HIDCLOSEWINDOW                      // close window when hid is outside
#define UC_DYN_SOFTKEYS      5                   // refresh softkeys every X cycle
//#define UC_MULTICPU        3                // use substationnumbers e.g.:255[1]
//#define UC_ZOOM_OBJSCHEME                           // zoom view of objectscheme
//#define UC_ALARMGROUPS                                        // use alarmgroups
#define UC_AUTO_RECONFIG_VAR                        // automatic try to re-connect
//#define UC_ACCESSNO_2BYTE                 // increase the accessno up to 2 bytes
//#define NINETEEN  9999                      // portrait format of 19inch Display
//#define UC_STATIC_ALARMSIZE 10      // static size of userinfo at each alarmline
#define UC_CALCULATE_H_POSITION_STEP  24        // horizontal-scroll-step in pixel
//#define UC_OVERLOAD_DEFAULT_SUMMARY_OBJECT      // ovrlod_summary obj at default
//#define UC_GRAYSCALE                         // no use of colors, just grayscale
//#define EDIT_DEFAULT_TIMEDATE '-'          // activate default time/date "--:--"
//#define UC_NO_TAB_CURSORNAVIGATION     // dont use tabulator in cursornavigation
//#define UC_NEWCOLORPALETTE                     // is used to change colorpalette
//#define UC_EDITOR_PRESIGN  // editoroption: '-' sign can be entered before value
#define UC_TRANSPARENT_TEXTCOLOR        // do not draw text with transparent color
#define UC_DYN_EDIT_COLOR      INVISIBLE  // dyn.color + edit of num.value + limit
//#define UC_MOVING_WINDOW                              // ability to move windows
//#define UC_INVISIBLE_PROGRESS                         // hide startupprogressbar
//#define UC_JAPANINPUT                                      // japanese inputmode
//#define UC_GETLASALID_DATALINK                    // use datalink to get Lasalid
#define UC_HISTORY_FILECACHE                    // Data-cache for AlarmHistoryFile

//#define USER_DEMAG
//#define USER_KM
//#define ZOOM_KM
//#define USER_LISEC
//#define USER_BILLION
//#define USER_BOY
//#define USER_HTC
//#define USER_TIG
//#define USER_DEKEMA
//#define USER_HIRSCH
//#define USER_SMT
//#define USER_HERZ

// *******************************************************************************
// *******************************************************************************
// ***  sigmatek configuration (user keep off !!)                              ***
// *******************************************************************************
// *******************************************************************************

#define UC_IPC                     // use ipc as target-system instead of terminal
#define RAMFILESIZE        10240
#define UC_NEWBUBBLEWINDOW 59995L                           // no of tooltipwindow
#define UC_CONFIRMWINDOW   59996L         // no of confirmwindow (OK, OK - CANCEL)
#define UC_RESETWINDOW     59998L                             // no of resetwindow
#define UC_OFFLINEWINDOW   59999L                           // no of offlinewindow
#define UC_REAL                                       // use floating point server
#define UC_CASE_SENSITIVE                                        // case sensitive
//#define UC_DIRECTORY        10                           // use directory-dialog
//#define UC_LONGFILENAMES             // long file names for the directory window
//#define CUTOFF_USER                                     // disable userinterface
//#define UC_GRAPHIC_STUDIO                                  // use graphic_studio
#define UC_WORDWRAP                                                // use wordwrap
#define UC_COMPLEX_SERVER                       // use 4-byte-structures at server
//#define UC_OLD_MOTIONS        // use motions in oldstyle, carousel will not work
#define UC_THEME                                        // use extended frametypes
//#define UC_CHANGE_IO_GROUP          // extended functionality of change_io_group
#define UC_INSERT_MODE                   // use key <ins> for insertmode in editor
#define USE_LOGOUT                            // use the LOGOUT function of loader
//#define UC_LOGEVENT      2000                      // log events from eventqueue
#define UC_FLICK_HID_DELAY  150                 // delaytime used at carousellmove 
//#define UC_EXOONLINE       0xFFFFFFFF           // use connection to foreign plc
//#define UC_TOUCHBUBBLE_BY_SELECTION      // show bubblehelp by touch after delay
//#define UC_TOUCHBUBBLE_IN_EDITWINDOW       // show bubblehelpinfo in edit window
//#define ET_261                                      // turn on when using ET261L
//#define OLDSTYLE_CAN                              // old style can communication
//#define CANOPEN
//#define _WIN_SERVERHELP      9                              // server topic-help
//#define UC_REDRAW_ALL             // whole background will be redrawn at closing
#define UC_MEMORIZE_PLCDATA                     // keep all values from plc in ram
//#define UC_MEMBERVARIABLE                   // membervariables will work as well
//#define UC_OVERLOAD_FCB_SUB // overload call of fcblock in functionblock as well
#define UC_ALARMID_FAST                          // speedup registration of alarms

// *******************************************************************************
// *** optimierung                                                             ***
// *******************************************************************************

// gesamten refresh nur bei änderungen in refreshlist durchführen. ansonsten
// gesamter refresh auf 4 durchlaeufe aufgeteilt
#define _CHANGELOOKUP

// nur beim ersten aufruf von chkbit-sequenz wird berechnet, für alle weiteren
// aufrufe wird berechnetes ergebnis zurückgegegen.
// beim bildaufbau werden alle berechnungen wieder verworfen.
//#define _STATICCHKBIT

// bei serverlistenzugriff wird stateschema + chkbit überprüft. dies wird rekursiv
// server->embedded->objekt durchgeführt. ist kein statescheme oder chkbit rekursiv
// angegeben, wird dies beim server vermerkt und die rekursion beim zugriff nicht aufgerufen.
#define _FIXED_STATESCHEME

// es werden keine grenzwerte bei ausgabe von num.output mittels GetSystemData geholt
#define _OUTPUT_NOLIMIT

// bei darstellung von objekt wird ggf.der hintergrund nicht gesichert (getimage)
#define _NO_GETIMAGE

// bei numerischer ausgabe wird viewport nicht wie bei allen anderen io's in funktion
// _SubPicture::SingleOutIo gesetzt. es wird sowieso bei ausgaberoutine vom text erledigt
#define _NO_VIEWPORT

// *******************************************************************************
// *******************************************************************************

//#define THR        // spezial userspecification, always turn off (excepting tom)
//#define VARTEXT_PREFIX
#define TOOLCAT_RUNCNT     1
//#define UC_GLOBAL_VAR                      // get r/w-access to global variables
//#define UC_GLOBAL_VAR_REFLIST              // get r/w-access to global variables
#define UC_FIXMEMORY            // use fix memoryblocks to speedup themes and bmps

#ifdef USER_DEKEMA // LeiChr 30.01.2015
 #ifndef UC_JPG
  #define UC_JPG                            // use jpg, class _JPG_RT is necessary
 #endif
  
 #ifdef UC_ROTATE
  #undef UC_ROTATE               // use GraficShadowFrameBuffer (OpSys >= 0101079)
 #endif
 #ifdef UC_HIDCLOSEWINDOW
  #undef UC_HIDCLOSEWINDOW                     // close window when hid is outside
 #endif
#endif

#ifdef USER_KM
 #ifndef XUSER_KM
  #define XUSER_KM
 #endif
#endif

#include "T_USER.H"

#ifdef UC_MULTITOUCH
 #ifndef UC_TOUCH
  #define UC_TOUCH
 #endif
#endif 

#ifdef ZOOM_KM
 #ifndef KOORDINATE_MUL_X
  #define KOORDINATE_MUL_X   816
 #endif
 #ifndef KOORDINATE_DIV_X
  #define KOORDINATE_DIV_X   700
 #endif
 #ifndef KOORDINATE_OFF_X
  #define KOORDINATE_OFF_X     4
 #endif
 #ifndef KOORDINATE_REF_X
  #define KOORDINATE_REF_X   100
 #endif
 #ifndef KOORDINATE_MUL_Y
  #define KOORDINATE_MUL_Y   576
 #endif
 #ifndef KOORDINATE_DIV_Y
  #define KOORDINATE_DIV_Y   494
 #endif
 #ifndef KOORDINATE_OFF_Y
  #define KOORDINATE_OFF_Y    12
 #endif
 #ifndef KOORDINATE_REF_Y
  #define KOORDINATE_REF_Y    28
 #endif
 #ifndef OFFSET_MENU_LINE_HEIGHT
  #define OFFSET_MENU_LINE_HEIGHT	10
 #endif
 #ifndef SOFTKEY_NEG_OFF_X
  #define SOFTKEY_NEG_OFF_X   65
 #endif
#endif

#define UC_PRJ_POPUP          60000              // variable diagnostic-popup
#define UC_ALARM_POPUP        60001             // alarmlist diagnostic-popup
#define UC_UNIT_POPUP         60002              // unitlist diagnostic-popup
#define UC_HOSTNAME_POPUP     60003              // hostname diagnostic-popup
#define UC_LANGUAGE_POPUP     60004                     // language.txt popup
#define UC_ALLTEXT_POPUP      60005                     // all textlist-popup
#define UC_TEXTFILE_POPUP     60006             //  textfile diagnostic-popup
#define UC_EXPLORER_POPUP     60007              // explorer diagnostic-popup
#define UC_IMAGEFILE_DIAG     60008             // imagefile diagnostic-popup
#define UC_VAR_POPUP          60009               // project diagnostic-popup
#define UC_BINFILE_POPUP      60010               // binfile diagnostic-popup
#define UC_COLSCHEME_POPUP    60011          // color-scheme diagnostic-popup
#define UC_SRNSCHEME_POPUP    60012         // screen-scheme diagnostic-popup
#define UC_FNTSCHEME_POPUP    60013           // font-scheme diagnostic-popup
#define UC_BMPSCHEME_POPUP    60014          // image-scheme diagnostic-popup
#define UC_STATSCHEME_POPUP   60015          // state-scheme diagnostic-popup
#define UC_UNITSCHEME_POPUP   60016           // unit-scheme diagnostic-popup
#define UC_TEXTSCHEME_POPUP   60017           // text-scheme diagnostic-popup
#define UC_IMAGE_POPUP        60018             // imagelist diagnostic-popup
#define UC_OBJECT_POPUP       60019            // objectlist diagnostic-popup
#define UC_FONT_POPUP         60020              // fontlist diagnostic-popup
#define UC_OBJECTSCHEME_POPUP 60021          // objectscheme diagnostic-popup
#define UC_FUNCTION_POPUP     60022          // functionlist diagnostic-popup
#define UC_MENULIST_POPUP     60023             // menuelist diagnostic-popup
#define UC_SEQUENCE_POPUP     60024          // sequencelist diagnostic-popup
#define UC_COLPAL_POPUP       60025         // color-palette diagnostic-popup
#define UC_HEAP_POPUP         60026                  // heap diagnostic-popup
//#define UC_HARDWARE_POPUP     60027                                   // nc
#define UC_SYSTEMINFO_POPUP   60028       // e.g. cycle time diagnostic-popup
#define UC_BATCH_POPUP        60029               // ipc.ini diagnostic-popup
//#define UC_PROFILER_POPUP     60030                                   // nc
#define UC_HOTKEYLIST_POPUP   60031            // hotkeylist diagnostic-popup
#define UC_SETUP_POPUP        60032                 // setup diagnostic-popup
#define UC_THEME_POPUP        60033                 // theme diagnostic-popup
#define UC_ONLINE_POPUP       60034                // online diagnostic-popup
#define UC_FONT_DIAG          60035                               // fontview
#define UC_MENU_DIAG          60036                               // menuview
#define UC_FKT_DIAG           60037                           // functionview
#define UC_TXT_DIAG           60038                           // textlistview
#define UC_VAR_DIAG           60039                           // variableview
#define UC_IMAGE_DIAG         60040                              // imageview
#define UC_TXT_DUMP_DIAG      60041                         // text dump view
#define UC_SERVERSCHEME_POPUP 60042
#define UC_TXT_FONTNAME_DIAG  60043
#define UC_ONLINE_SERVER      60044

#define UC_LSLID_POPUP        60100     // popup of variables -> save lasalid


#define UC_CHKBIT_NO   256                            // no of static chkbits


#define GLOBAL_BIT_IFWINSTART                   0x00000001
#define GLOBAL_BIT_IFWINREADY                   0x00000002
#define GLOBAL_BIT_IFWINRUN                     0x00000004
#define GLOBAL_BIT_IFWINEND                     0x00000008
#define GLOBAL_BIT_IFRECALCULATE                0x00000010
#define GLOBAL_BIT_IFACCESSINPUT                0x00000020
#define GLOBAL_BIT_IFACCESSBUTTON               0x00000040
#define GLOBAL_BIT_IFCHKENABLE                  0x00000080
#define GLOBAL_BIT_IFUSERCALL                   0x00000100
#define GLOBAL_BIT_IFFOUNDHID                   0x00000200
#define GLOBAL_BIT_IFTRYTOACCESS                0x00000400
#define GLOBAL_BIT_IFPRJRUN                     0x00000800
#define GLOBAL_BIT_IFWINMOVEABLE                0x00001000

//#define UC_PICKER           32002         // change parameter (output) enable

#define PGTEST

//#define PLC_SIMULATION                    // plc-simulation (always turn off)
#define OK_ENCLOSE_NAME       // name (ascii-0-string) is enclosed to all i/o
#define OK_SEEK_NEXT            // seek for nextnumber (manual-cursorcontrol)
//#define OK_RUNTIME_MODIFY  // do some modifications to picture during runtime
#define OK_SUMMARY                                       // group information
#define OK_PICMEMO              // memoryinformation for pictures and windows
//#define OK_VALIDATE                            // validate all changed inputs
#define OK_SEEK_BUTTON           // include buttons to editor (cursorcontrol)
#define OK_CHECK_CRCOFID // check crc of connected plc and ifnec get lasalid
#define OK_SUBLIST                          // ability to create sublist-menue
#define OK_DIAGNOSTIC                      // ability to open diagnostic-menue

#define MAX_LABELLEN      300                // max. length of variable-label
#define _MAX_PATHLENGTH   300                          // max. length of dpne

#ifdef SEEK_SET
 #undef SEEK_SET
#endif
#define SEEK_SET            0

#ifdef SEEK_CUR
 #undef SEEK_CUR
#endif
#define SEEK_CUR            1

#ifdef SEEK_END
 #undef SEEK_END
#endif
#define SEEK_END            2

#define NULL                0                                 // pointer init
#define DEFACCESS           0                    // key access (_ACCESS) init
#define DEFZOOM             0                      // zoomfactor (_ZOOM) init
#define DEFWIDTH            0                     // width (picturetext) init
#define DEFENTERPLUS        0                              // enter-plus init
#define DEFNEXTNO           0                              // verkettung init
#define DEFLASALID          0                                // lasal id init
#define DEFSTATION        255                // local or defaultstationnumber
#define DEFSUMMARY          0            // defaultvalue for groupinformation
#define DEFFORMULA        255          // defaultvalue for formula (variable)

#define SIZE_BYTE           0
#define SIZE_WORD           1
#define SIZE_DWORD          2
#define SIZE_UBYTE          3
#define SIZE_UWORD          4
#define SIZE_UDWORD         5
#define SIZE_HBYTE          6
#define SIZE_HWORD          7
#define SIZE_HDWORD         8
#define SIZE_BBYTE          9
#define SIZE_BWORD         10
#define SIZE_BDWORD        11
#define SIZE_REAL          12
#define SIZE_LREAL         13

#define SIZE_HHMM           0
#define SIZE_HHMMSS         1
#define SIZE_MMSS           2

#define SIZE_DDMM           0
#define SIZE_DDMMYY         1
#define SIZE_WWDDMM         2
#define SIZE_WWDDMMYY       3
#define SIZE_DDMMYYYY       4
#define SIZE_WWDDMMYYYY     5
#define SIZE_YYYYMMDD       6 // sa20024
#define SIZE_YYMMDD         7 // sa20024
#define SIZE_YYMM           8 // sa20024


#define SIZE_USINT          0
#define SIZE_UINT           1
#define SIZE_BYTE3          2
#define SIZE_UDINT          3

#define _SHIFT              3
#define _CTRL               4
#define _ALT                8

#define _NOKEY              0 //0x00000000
#define _LEFT           19200 //0x00004B00
#define _RIGHT          19712 //0x00004D00
#define _UP             18432 //0x00004800
#define _DOWN           20480 //0x00005000
#define _PGUP           18688 //0x00004900
#define _PGDOWN         20736 //0x00005100
#define _PGLEFT         19456 //0x00004C00 //19201
#define _PGRIGHT        19968 //0x00004E00 //19713
#define _CAROUSELINC    _PGUP
#define _CAROUSELDEC    _PGDOWN


#ifdef USER_DEMAG
 #define _F1            26624 //0x00006800
 #define _F2            26880 //0x00006900
 #define _F3            27136 //0x00006A00
 #define _F4            27392 //0x00006B00
 #define _F5            27648 //0x00006C00
 #define _F6            27904 //0x00006D00
 #define _F7            28160 //0x00006E00
 #define _F8            28416 //0x00006F00
 #define _F9            28672 //0x00007000
 #define _F10           28928 //0x00007100
#else
 #define _F1            15104 //0x00003B00
 #define _F2            15360 //0x00003C00
 #define _F3            15616 //0x00003D00
 #define _F4            15872 //0x00003E00
 #define _F5            16128 //0x00003F00
 #define _F6            16384 //0x00004000
 #define _F7            16640 //0x00004100
 #define _F8            16896 //0x00004200
 #define _F9            17152 //0x00004300
 #define _F10           17408 //0x00004400
#endif
#define _F11            22272 //0x00005700
#define _F12            22528 //0x00005800

#define _ESC               27 //0x0000001B
#define _BCKSPC             8 //0x00000008
#define _ENTER             13 //0x0000000D
#define _DIRECTIONENTER  7168 //0x00001C00 (CTRL-ENTER)
#define _POS1           18176 //0x00004700
#define _END            20224 //0x00004F00
#define _TAB                9 //0x00000009
#define _DEL            21248 //0x00005300
#define _INS            20992 //0x00005200
#define _HOME           18176 //0x00004700
#define _ALTENTER        2061 //0x0000080D
#define _ALTE            2149 //0x00000865

#define _KEY_SERVERHELP   '?'


#define _MODE_ALT      524288 //0x00080000
#define _MODE_CTRL     262144 //0x00040000
#define _MODE_SHIFT    196608 //0x00030000

#define _DDE_RDLY           1 //0x01
#define _DDE_HIDDEN         2 //0x02
#define _DDE_SYSTEM         4 //0x04
#define _DDE_VOLUME         8 //0x08
#define _DDE_DIR           16 //0x10
#define _DDE_ARCHIV        32 //0x20

#ifdef UC_MOUSE_AUTODETECT
 #ifndef UC_MOUSE
  #define UC_MOUSE
 #endif
#endif

#ifdef UC_MOUSE
 #ifndef HID
  #define HID    // human interface devices mouse, touch, ...
 #endif
#endif

#ifdef UC_TOUCH
 #ifndef HID
  #define HID    // human interface devices mouse, touch, ...
 #endif
#endif

#ifdef UC_TOUCH
 #ifdef UC_MOUSE
  #ifndef LEVELBUTTON_HEIGHT
   #define LEVELBUTTON_HEIGHT 14
  #endif
  #ifndef LEVELBUTTON_WIDTH
   #define LEVELBUTTON_WIDTH  16
  #endif
 #else
  #ifndef LEVELBUTTON_HEIGHT
   #define LEVELBUTTON_HEIGHT 34 //28
  #endif
  #ifndef LEVELBUTTON_WIDTH
   #define LEVELBUTTON_WIDTH  39 //32
  #endif
 #endif
#else
  #ifndef LEVELBUTTON_HEIGHT
   #define LEVELBUTTON_HEIGHT 14
  #endif
  #ifndef LEVELBUTTON_WIDTH
   #define LEVELBUTTON_WIDTH  16
  #endif
#endif

#define DEFAULT_SCROLL_LINE_HEIGHT  20
#define DEFAULT_SCROLL_LINE_WIDTH  200

#ifdef UC_IPC
 #define UC_HICOLOR                 // 64k colormode (otherwise 256 colormode)
 #define OK_OFFSCREEN
#else
 #undef SOFT_HEIGHT
 #define SOFT_HEIGHT 22

 #ifdef UC_UNICODE
  #undef UC_UNICODE
 #endif
 #ifdef UC_GRAPHIC_STUDIO
  #undef UC_GRAPHIC_STUDIO
 #endif
// #ifdef UC_OFFSCREENDRAW
//  #undef UC_OFFSCREENDRAW
// #endif
#endif

#ifdef USER_BILLION
 #ifdef EDIT_DEFAULT_TIMEDATE
  #undef EDIT_DEFAULT_TIMEDATE
 #endif
 #define EDIT_DEFAULT_TIMEDATE  '-' // set defaultvalue for time/date when insert '-' into time/date editor
#endif

#ifdef USER_DEKEMA
 #ifdef EDIT_DEFAULT_TIMEDATE
  #undef EDIT_DEFAULT_TIMEDATE
 #endif
 #define EDIT_DEFAULT_TIMEDATE  '-' // set defaultvalue for time/date when insert '-' into time/date editor
#endif

#ifdef USER_KM
 #ifndef SELECT_AND_CHANGE_CHKBOX
  #define SELECT_AND_CHANGE_CHKBOX // chkbox, radiobox: first hid_press = select, further hid_press = change
 #endif
 #ifndef UNIT_2Byte
  #define UNIT_2Byte
 #endif
#endif

#ifdef UC_TOUCHBUBBLE_BY_SELECTION
 #ifndef UC_TOUCH
  #undef UC_TOUCHBUBBLE_BY_SELECTION
 #endif
#endif

#ifdef UC_TOUCHBUBBLE_BY_SELECTION
 #ifndef UC_BUBBLE_HELP
  #undef UC_TOUCHBUBBLE_BY_SELECTION
 #endif
#endif

#ifdef UC_TOUCHBUBBLE_IN_EDITWINDOW
 #ifndef UC_TOUCH
  #undef UC_TOUCHBUBBLE_IN_EDITWINDOW
 #endif
#endif

#ifdef UC_TOUCHBUBBLE_IN_EDITWINDOW
 #ifndef UC_BUBBLE_HELP
  #undef UC_TOUCHBUBBLE_IN_EDITWINDOW
 #endif
#endif

#ifdef USER_DEMAG
  #ifndef UC_DELETE_WINDOWBACKGROUND
   #define UC_DELETE_WINDOWBACKGROUND
  #endif
  #ifndef UNIT_2Byte
   #define UNIT_2Byte
  #endif
  #ifdef UC_TRANSPARENT_TEXTCOLOR
   #undef UC_TRANSPARENT_TEXTCOLOR
  #endif
#else
 #ifndef USER_LISEC
  #ifndef USER_HTC
   #define SIGMATEK_STANDARD
  #endif
 #endif
#endif

//#ifdef UC_ROTATE
// #ifdef OK_OFFSCREEN
//  #undef OK_OFFSCREEN  // kein Offscreen notwendig
// #endif
//#endif

#ifndef _IsCppCompiler
 #ifdef _MSC_VER
  #define _IsCppCompiler
 #endif
 #ifdef _GNUC
  #define _IsCppCompiler
 #endif
#endif

#ifndef UC_UNICODE
 #ifdef UC_JAPANINPUT
  #undef UC_JAPANINPUT
 #endif
#endif

#define FRAME_NONE                   0
#define FRAME_SINGLE                 1
#define FRAME_3D                     2
#define FRAME_OVERLAPPED             3
#define FRAME_EDGE                   4
#define FRAME_BRICK                 23
#define FRAME_CANDY                 22
#define FRAME_CRYSTAL               32
#define FRAME_CRYSTAL_LED           33
#define FRAME_JUNIOR_BOLD           39
#define FRAME_JUNIOR                26
#define FRAME_KEY                   21
#define FRAME_KEY_LED               20
#define FRAME_PLASTIC_DASH          38
#define FRAME_PLASTIC               37 // always present, used as default
#define FRAME_STAGE_LIGHT           28
#define FRAME_STAGE                 27
#define FRAME_STEALTH               25 // always present, used as default
#define FRAME_STEALTH_LED           24
#define FRAME_TILE                  34
#define FRAME_TILE_LED              35
#define FRAME_TILE_REDLED           36
#define FRAME_CHAIN                 74
#define FRAME_CHAIN_LEFT            75
#define FRAME_CHAIN_MIDDLE          77
#define FRAME_CHAIN_RIGHT           76
#define FRAME_CORAL_CANCEL          78
#define FRAME_GLOSSY                73
#define FRAME_METAL_CANCEL          79 // always present, used as default
#define FRAME_SHADY                 70
#define FRAME_NEWAGE_BRIGHT         80
#define FRAME_NEWAGE_DARK           81
#define FRAME_BLUE_LINE             82
#define FRAME_WINDOW_BLUE          101
#define FRAME_WINDOW_BLUE_CANCEL   111
#define FRAME_WINDOW_CORAL         105
#define FRAME_WINDOW_CORAL_CANCEL  112
#define FRAME_WINDOW_GLOSSY        103
#define FRAME_WINDOW_METAL         100
#define FRAME_WINDOW_METAL_CANCEL  110
#define FRAME_WINDOW_STAGE         104
#define FRAME_WINDOW_TENDER        102
#define FRAME_WINDOW_NEWAGE        113
#define FRAME_WINDOW_NEWAGE_CANCEL 114



#ifdef _IsCppCompiler
 #define GetBackColor_Makro(_p)       ((_p) >> 16) 
 #define GetPenColor_Makro(_p)        ((_p) & 0xFFFF)
 #define InvertColor_Makro(_p)        (((_p) << 16) | ((_p) >> 16))
 #define GetXSize_Makro(_p)           (((_p) >> 28) & (0x000F))
 #define GetYSize_Makro(_p)           (((_p) >> 24) & (0x000F))
 #define MakeColor_Makro(_p1, _p2)    (((_p1) << 16) | ((_p2) & 0xFFFF))
 #define chk_VARLABEL_Makro(_p) ((_p)->no > 0)
#else
 #define GetBackColor_Makro(_p)       ((_p) SHR 16) 
 #define GetPenColor_Makro(_p)        ((_p) AND 16#FFFF)
 #define InvertColor_makro(_p)        (((_p) SHL 16) OR ((_p) SHR 16))
 #define GetXSize_Makro(_p)           (((_p) SHR 28) AND (16#000F))
 #define GetYSize_Makro(_p)           (((_p) SHR 24) AND (16#000F))
 #define MakeColor_Makro(_p1, _p2)    (((_p1) SHL 16) OR ((_p2) AND 0xFFFF))
#endif

#include <t_grc.h>                   // all graphic definitions and constants

#endif // _T_IPC_H


