/*
  Lasal32c.c
  
  Diese File dient nur dazu, globale Variablen mit einem Init-Wert anzulegen, 
  da es im Structured Text keine M�glichkeit daf�r gibt.
  Im ST-File ist eine VAR_EXTERNAL Deklaration f�r die Variable erforderlich
  (mit vorangestlltem _).
*/
unsigned long g_Lasal32_test = 0x1234;
