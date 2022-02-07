/*
  Lasal32c.c
  
  Diese File dient nur dazu, globale Variablen mit einem Init-Wert anzulegen, 
  da es im Structured Text keine Möglichkeit dafür gibt.
  Im ST-File ist eine VAR_EXTERNAL Deklaration für die Variable erforderlich
  (mit vorangestlltem _).
*/
unsigned long g_Lasal32_test = 0x1234;
