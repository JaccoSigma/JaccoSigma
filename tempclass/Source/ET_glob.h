#ifndef __ETGLOB_ST__
#define __ETGLOB_ST__


#include <lsl_st_iflinker.h>
#include <Comlink.h>
#include <lsl_string.h>
#include <Lsl_st_ifssr.h>

// LT-TEST *******************************

FUNCTION GLOBAL TAB tab_STX;
FUNCTION GLOBAL TAB tab_MTX;
FUNCTION GLOBAL TAB tab_ATX;
FUNCTION GLOBAL TAB tab_PIC00;
FUNCTION GLOBAL TAB tab_VAR;
FUNCTION GLOBAL TAB tab_KEY;



FUNCTION GLOBAL TAB tab_CHAR;
 

#endif //__ETGLOB_ST__

  #define pgtest
//*********************************************************
//  #define LSL_GUID		// use GUIDs to indentify classes
//
// is comment out because it is not used
// HaaPet 14.01.05
//*********************************************************
  #define COMLINK_LASAL

  //#define Io_Segment   16#d0000
  //#define _DET243      243

  #define DEFCHKBIT   0
  #define ET_ENTER    16#000D
  #define ET_CLEAR    27
  #define ET_LEFT     16#4B00
  #define ET_RIGHT    16#4D00
  #define ET_NOKEY    16#0000
  #define ET_BCKSPC   16#1008     
  #define ET_MINUS    16#002D
  #define ET_PLUS     16#002B
