

#ifndef __C__MEMORY__HEADER__
#define __C__MEMORY__HEADER__

(* -- Das ist alter code der durch #define __C__MEMORY__HEADER__ in rtos_variables.h ungültig gemacht wurde

#define RAM_MAX_CELL     1000                 // maximum # of ramcells


TYPE

 RamCell 	: STRUCT
   udChk 	: UDINT;
   udCRC32 	: UDINT;
   udData 	: UDINT;
 END_STRUCT;

 RAMHDR                       : STRUCT
  udEntries                     : UDINT;
  udChk                         : UDINT;
  aData                         : ARRAY [0..RAM_MAX_CELL] OF RAMCELL;
 END_STRUCT;

END_TYPE
*)
#endif // __C__MEMORY__HEADER__