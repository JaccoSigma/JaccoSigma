//<NewSigmatekCFileOptimize/>
// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 11.11.2020                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#include "T_IPC.H"

#ifdef UC_JAPANINPUT

#include "T_NUC.H"

#define UNUSUAL_KATAKANA
#define KATAKANA_N 0x30F3


_UDWORD Japan_Katakana11(_CHAR *pd, _CHAR *ps)
{
  // 1 zeichen durch 1 anderes ersetzen (blau)
  switch(ps[0])
  {
    case 'a': pd[0] = 0x30A2; return 0x30A2;
    case 'i': pd[0] = 0x30A4; return 0x30A4;
    case 'u': pd[0] = 0x30A6; return 0x30A6;
    case 'e': pd[0] = 0x30A8; return 0x30A8;
    case 'o': pd[0] = 0x30AA; return 0x30AA;
//    case 'n': pd[0] = 0x006E; return 0x006E;
  }
  
  return 0;
}

_UDWORD Japan_Katakana21(_CHAR *pd, _CHAR *ps)
{
  // 2 zeichen durch 1 anderes ersetzen (gelb)
  switch(ps[0])
  {
    case 'b':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30D0; return 0x30D0;
        case 'i': pd[0] = 0x30D3; return 0x30D3;
        case 'u': pd[0] = 0x30D6; return 0x30D6;
        case 'e': pd[0] = 0x30D9; return 0x30D9;
        case 'o': pd[0] = 0x30DC; return 0x30DC;
      }
      break;
    case 'c':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30AB; return 0x30AB;
        case 'i': pd[0] = 0x30B7; return 0x30B7;
        case 'u': pd[0] = 0x30AF; return 0x30AF;
        case 'e': pd[0] = 0x30BB; return 0x30BB;
        case 'o': pd[0] = 0x30B3; return 0x30B3;
      }
      break;
    case 'd':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30C0; return 0x30C0;
        case 'i': pd[0] = 0x30C2; return 0x30C2;
        case 'u': pd[0] = 0x30C5; return 0x30C5;
        case 'e': pd[0] = 0x30C7; return 0x30C7;
        case 'o': pd[0] = 0x30C9; return 0x30C9;
      }
      break;
    case 'f':
      switch(ps[1])
      {
        case 'u': pd[0] = 0x30D5; return 0x30D5;
      }
      break;  
    case 'g':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30AC; return 0x30AC;
        case 'i': pd[0] = 0x30AE; return 0x30AE;
        case 'u': pd[0] = 0x30B0; return 0x30B0;
        case 'e': pd[0] = 0x30B2; return 0x30B2;
        case 'o': pd[0] = 0x30B4; return 0x30B4;
      }
      break;
    case 'h':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0x30DB; return 0x30DB;
      }
      break;  
    case 'j':
      switch(ps[1])
      {
        case 'i': pd[0] = 0x30B8; return 0x30B8;
      }
      break;  
    case 'k':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30AB; return 0x30AB;
        case 'i': pd[0] = 0x30AD; return 0x30AD;
        case 'u': pd[0] = 0x30AF; return 0x30AF;
        case 'e': pd[0] = 0x30B1; return 0x30B1;
        case 'o': pd[0] = 0x30B3; return 0x30B3;
      }
      break;  
    case 'l':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30A1; return 0x30A1;
        case 'i': pd[0] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30A5; return 0x30A5;
        case 'e': pd[0] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30A9; return 0x30A9;
      }
      break; 
    case 'm':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30DE; return 0x30DE;
        case 'i': pd[0] = 0x30DF; return 0x30DF;
        case 'u': pd[0] = 0x30E0; return 0x30E0;
        case 'e': pd[0] = 0x30E1; return 0x30E1;
        case 'o': pd[0] = 0x30E2; return 0x30E2;
      }
      break;  
    case 'n':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30CA; return 0x30CA;
        case 'i': pd[0] = 0x30CB; return 0x30CB;
        case 'u': pd[0] = 0x30CC; return 0x30CC;
        case 'e': pd[0] = 0x30CD; return 0x30CD;
        case 'o': pd[0] = 0x30CE; return 0x30CE;
        case 'n': pd[0] = KATAKANA_N; return KATAKANA_N;
      }
      break;
    case 'p':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30D1; return 0x30D1;
        case 'i': pd[0] = 0x30D4; return 0x30D4;
        case 'u': pd[0] = 0x30D7; return 0x30D7;
        case 'e': pd[0] = 0x30DA; return 0x30DA;
        case 'o': pd[0] = 0x30DD; return 0x30DD;
      }
      break;
    case 'q':
      switch(ps[1])
      {
        case 'u': pd[0] = 0x30AF; return 0x30AF;
      }
      break;   
    case 'r':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30E9; return 0x30E9;
        case 'i': pd[0] = 0x30EA; return 0x30EA;
        case 'u': pd[0] = 0x30EB; return 0x30EB;
        case 'e': pd[0] = 0x30EC; return 0x30EC;
        case 'o': pd[0] = 0x30ED; return 0x30ED;
      }
      break;
    case 's':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30B5; return 0x30B5;
        case 'i': pd[0] = 0x30B7; return 0x30B7;
        case 'u': pd[0] = 0x30B9; return 0x30B9;
        case 'e': pd[0] = 0x30BB; return 0x30BB;
        case 'o': pd[0] = 0x30BD; return 0x30BD;
      }
      break;
    case 't':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30BF; return 0x30BF;
        case 'i': pd[0] = 0x30C1; return 0x30C1;
        case 'u': pd[0] = 0x30C4; return 0x30C4;
        case 'e': pd[0] = 0x30C6; return 0x30C6;
        case 'o': pd[0] = 0x30C8; return 0x30C8;
      }
      break;
    case 'v':
      switch(ps[1])
      {
        case 'u': pd[0] = 0x30F4; return 0x30F4;
      }
      break; 
    case 'w':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30EF; return 0x30EF;
        case 'u': pd[0] = 0x30A6; return 0x30A6;
        case 'o': pd[0] = 0x30F2; return 0x30F2;
      }
      break;  
    case 'x':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30A1; return 0x30A1;
        case 'i': pd[0] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30A5; return 0x30A5;
        case 'e': pd[0] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30A9; return 0x30A9;
        case 'n': pd[0] = KATAKANA_N; return KATAKANA_N;
      }
      break;  
    case 'y':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30E4; return 0x30E4;
        case 'i': pd[0] = 0x30A4; return 0x30A4;
        case 'u': pd[0] = 0x30E6; return 0x30E6;
        case 'o': pd[0] = 0x30E8; return 0x30E8;
      }
      break;
    case 'z':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30B6; return 0x30B6;
        case 'i': pd[0] = 0x30B8; return 0x30B8;
        case 'u': pd[0] = 0x30BA; return 0x30BA;
        case 'e': pd[0] = 0x30BC; return 0x30BC;
        case 'o': pd[0] = 0x30BE; return 0x30BE;
      }
      break;
  }
  
  return 0;
}

_UDWORD Japan_Katakana31(_CHAR *pd, _CHAR *ps)
{
  // 3 zeichen durch 1 anderes ersetzen (magenta)
  if((ps[0] == 'l') && (ps[1] == 'y'))
  {
    if(ps[2] == 'a')
    {
      pd[0] = 0x30E3;
      return 0x30E3;
    }
    if(ps[2] == 'i')
    {
      pd[0] = 0x30A3;
      return 0x30A3;
    }
    if(ps[2] == 'u')
    {
      pd[0] = 0x30E5;
      return 0x30E5;
    }
    if(ps[2] == 'e')
    {
      pd[0] = 0x30A7;
      return 0x30A7;
    }
    if(ps[2] == 'o')
    { 
      pd[0] = 0x30E7;
      return 0x30E7;
    }
  }  
  else if((ps[0] == 'w') && (ps[1] == 'y'))
  {
    if(ps[2] == 'i')
    {
      pd[0] = 0x30F0;
      return 0x30F0;
    }
    if(ps[2] == 'e')
    {
      pd[0] = 0x30F1;
      return 0x30F1;
    }
  }  
  else if((ps[0] == 'x') && (ps[1] == 'y'))
  {
    if(ps[2] == 'a')
    {
      pd[0] = 0x30E3;
      return 0x30E3;
    }
    if(ps[2] == 'i')
    {
      pd[0] = 0x30A3;
      return 0x30A3;
    }
    if(ps[2] == 'u')
    {
      pd[0] = 0x30E5;
      return 0x30E5;
    }
    if(ps[2] == 'e')
    {
      pd[0] = 0x30A7;
      return 0x30A7;
    }
    if(ps[2] == 'o')
    { 
      pd[0] = 0x30E7;
      return 0x30E7;
    }
  }  
  else if(ps[1] == 'h')
  {
    if((ps[0] == 'c') && /*(ps[1] == 'h') &&*/ (ps[2] == 'i'))
    {
      pd[0] = 0x30C1;
      return 0x30C1;
    }
    if((ps[0] == 'h') && /*(ps[1] == 'h') &&*/ (ps[2] == 'o'))
    {
      pd[0] = 0x30DB;
      return 0x30DB;
    }
    if((ps[0] == 's') && /*(ps[1] == 'h') &&*/ (ps[2] == 'i'))
    {
      pd[0] = 0x30B7;
      return 0x30B7;
    }
    if((ps[0] == 'w') && /*(ps[1] == 'h') &&*/ (ps[2] == 'u'))
    {
      pd[0] = 0x30A6;
      return 0x30A6;
    }
  }

  return 0;
}

_UDWORD Japan_Katakana22(_CHAR *pd, _CHAR *ps)
{
  // 2 zeichen durch 2 andere ersetzen (grün)
  switch(ps[0])
  {
   #ifdef UNUSUAL_KATAKANA
    case 'b':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF42; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif
   #ifdef UNUSUAL_KATAKANA
    case 'c':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF43; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif
   #ifdef UNUSUAL_KATAKANA
    case 'd':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF44; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif
    case 'f':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30D5; pd[1] = 0x30A1; return 0x30A1;
        case 'i': pd[0] = 0x30D5; pd[1] = 0x30A3; return 0x30A3;
        case 'e': pd[0] = 0x30D5; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30D5; pd[1] = 0x30A9; return 0x30A9;
       #ifdef UNUSUAL_KATAKANA
        case 'n': pd[0] = 0xFF46; pd[1] = 0xFF4E; return 0xFF4E;
       #endif 
      }
      break;   
   #ifdef UNUSUAL_KATAKANA
    case 'g':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF47; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif 
   #ifdef UNUSUAL_KATAKANA
    case 'h':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF48; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif
    case 'j':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30B8; pd[1] = 0x30E3; return 0x30E3;
        case 'u': pd[0] = 0x30B8; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30B8; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30B8; pd[1] = 0x30E7; return 0x30E7;
       #ifdef UNUSUAL_KATAKANA
        case 'n': pd[0] = 0xFF4A; pd[1] = 0xFF4E; return 0xFF4E;
       #endif 
      }
      break;
   #ifdef UNUSUAL_KATAKANA
    case 'k':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF4B; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif      
   #ifdef UNUSUAL_KATAKANA
    case 'l':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF4C; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif
   #ifdef UNUSUAL_KATAKANA
    case 'm':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF4D; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif      
   #ifdef UNUSUAL_KATAKANA
    case 'p':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF50; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif      
    case 'q':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30AF; pd[1] = 0x30A1; return 0x30A1;
        case 'i': pd[0] = 0x30AF; pd[1] = 0x30A3; return 0x30A3;
        case 'e': pd[0] = 0x30AF; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30AF; pd[1] = 0x30A9; return 0x30A9;
       #ifdef UNUSUAL_KATAKANA
        case 'n': pd[0] = 0xFF51; pd[1] = 0xFF4E; return 0xFF4E;
       #endif
      }
      break;
   #ifdef UNUSUAL_KATAKANA
    case 'r':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF52; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif      
   #ifdef UNUSUAL_KATAKANA
    case 's':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF53; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif      
   #ifdef UNUSUAL_KATAKANA
    case 't':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF54; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif      
    case 'v':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x30F4; pd[1] = 0x30A1; return 0x30A1;
        case 'i': pd[0] = 0x30F4; pd[1] = 0x30A3; return 0x30A3;
        case 'e': pd[0] = 0x30F4; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30F4; pd[1] = 0x30A9; return 0x30A9;
       #ifdef UNUSUAL_KATAKANA
        case 'n': pd[0] = 0xFF56; pd[1] = 0xFF4E; return 0xFF4E;
       #endif 
      }
      break;
    case 'w':
      switch(ps[1])
      {
        case 'i': pd[0] = 0x30A6; pd[1] = 0x30A3; return 0x30A3;
        case 'e': pd[0] = 0x30A6; pd[1] = 0x30A7; return 0x30A7;
       #ifdef UNUSUAL_KATAKANA
        case 'n': pd[0] = 0xFF57; pd[1] = 0xFF4E; return 0xFF4E;
       #endif 
      }
      break;
    case 'y':
      switch(ps[1])
      {
        case 'e': pd[0] = 0x30A4; pd[1] = 0x30A7; return 0x30A7;
       #ifdef UNUSUAL_KATAKANA
        case 'n': pd[0] = 0xFF59; pd[1] = 0xFF4E; return 0xFF4E;
       #endif 
      }
      break;
   #ifdef UNUSUAL_KATAKANA
    case 'z':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF5A; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif 
      
  }

  return 0;
}

_UDWORD Japan_Katakana32(_CHAR *pd, _CHAR *ps)
{
  // 3 zeichen durch 2 andere ersetzen (orange)
  if(ps[0] == 'b') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30D3; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30D3; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30D3; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30D3; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30D3; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF42; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF42; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF42; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF42; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF42; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
  else if(ps[0] == 'c')
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30C1; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30C1; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30C1; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30C1; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30C1; pd[1] = 0x30E7; return 0x30E7;
      }
    }
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30C1; pd[1] = 0x30E3; return 0x30E3;
        case 'u': pd[0] = 0x30C1; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30C1; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30C1; pd[1] = 0x30E7; return 0x30E7;
      }
    }
  }
  else if(ps[0] == 'd') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30C2; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30C2; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30C2; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30C2; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30C2; pd[1] = 0x30E7; return 0x30E7;
      }
    }
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30C7; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30C7; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30C7; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30C7; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30C7; pd[1] = 0x30E7; return 0x30E7;
      }
    }
  }
  else if(ps[0] == 'f') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30D5; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30D5; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30D5; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30D5; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30D5; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF46; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF46; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF46; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF46; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF46; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
  else if(ps[0] == 'g') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30AE; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30AE; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30AE; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30AE; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30AE; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF47; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF47; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF47; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF47; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF47; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
  else if(ps[0] == 'h') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30D2; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30D2; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30D2; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30D2; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30D2; pd[1] = 0x30E7; return 0x30E7;
      }
    }
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30C3; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0x30C3; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0x30C3; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0x30C3; pd[1] = 0x30D8; return 0x30D8;
      }
    }
  }
  else if(ps[0] == 'j') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30B8; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30B8; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30B8; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30B8; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30B8; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF4A; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF4A; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF4A; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF4A; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF4A; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
  else if(ps[0] == 'k') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30AD; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30AD; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30AD; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30AD; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30AD; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF4B; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF4B; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF4B; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF4B; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF4B; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
 #ifdef UNUSUAL_KATAKANA
  else if(ps[0] == 'l') 
  {
    if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF4C; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF4C; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF4C; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF4C; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF4C; pd[1] = 0x30DB; return 0x30DB;
      }
    }
  }
 #endif 
  else if(ps[0] == 'm') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30DF; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30DF; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30DF; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30DF; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30DF; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF4D; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF4D; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF4D; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF4D; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF4D; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
  else if(ps[0] == 'n')
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30CB; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30CB; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30CB; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30CB; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30CB; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30F3; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0x30F3; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0x30F3; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0x30F3; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0x30F3; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
  else if(ps[0] == 'p') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30D4; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30D4; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30D4; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30D4; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30D4; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF50; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF50; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF50; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF50; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF50; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
  else if(ps[0] == 'q') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30AF; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30AF; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30AF; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30AF; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30AF; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF51; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF51; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF51; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF51; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF51; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
  else if(ps[0] == 'r') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30EA; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30EA; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30EA; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30EA; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30EA; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF52; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF52; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF52; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF52; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF52; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
  else if(ps[0] == 's') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30B7; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30B7; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30B7; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30B7; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30B7; pd[1] = 0x30E7; return 0x30E7;
      }
    }
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30B7; pd[1] = 0x30E3; return 0x30E3;
        case 'u': pd[0] = 0x30B7; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30B7; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30B7; pd[1] = 0x30E7; return 0x30E7;
      }
    }
  }
  else if(ps[0] == 't') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30C1; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30C1; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30C1; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30C1; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30C1; pd[1] = 0x30E7; return 0x30E7;
      }
    }
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30C6; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30C6; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30C6; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30C6; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30C6; pd[1] = 0x30E7; return 0x30E7;
      }
    }
  }
  else if(ps[0] == 'v') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30F4; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30F4; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30F4; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30F4; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30F4; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF56; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF56; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF56; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF56; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF56; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
  else if(ps[0] == 'w')
  {
   #ifdef UNUSUAL_KATAKANA
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF57; pd[1] = 0x30E4; return 0x30E4;
        case 'u': pd[0] = 0xFF57; pd[1] = 0x30E6; return 0x30E6;
        case 'o': pd[0] = 0xFF57; pd[1] = 0x30E8; return 0x30E8;
      }
    }
   #endif
    if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30A6; pd[1] = 0x30A1; return 0x30A1;
        case 'i': pd[0] = 0x30A6; pd[1] = 0x30A3; return 0x30A3;
        case 'e': pd[0] = 0x30A6; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30A6; pd[1] = 0x30A9; return 0x30A9;
      }
    }
  }
 #ifdef UNUSUAL_KATAKANA
  else if(ps[0] == 'x')
  {
    if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF58; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF58; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF58; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF58; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF58; pd[1] = 0x30DB; return 0x30DB;
      }
    }
  }
 #endif 
  else if(ps[0] == 'y')
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30C3; pd[1] = 0x30E4; return 0x30E4;
        case 'i': pd[0] = 0x30C3; pd[1] = 0x30A4; return 0x30A4;
        case 'u': pd[0] = 0x30C3; pd[1] = 0x30E6; return 0x30E6;
//        case 'e': pd[0] = 0x30C3; pd[1] = 0x30A4; return 0x30A4;
        case 'o': pd[0] = 0x30C3; pd[1] = 0x30E8; return 0x30E8;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF59; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF59; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF59; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF59; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF59; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }
  else if(ps[0] == 'z')
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30B8; pd[1] = 0x30E3; return 0x30E3;
        case 'i': pd[0] = 0x30B8; pd[1] = 0x30A3; return 0x30A3;
        case 'u': pd[0] = 0x30B8; pd[1] = 0x30E5; return 0x30E5;
        case 'e': pd[0] = 0x30B8; pd[1] = 0x30A7; return 0x30A7;
        case 'o': pd[0] = 0x30B8; pd[1] = 0x30E7; return 0x30E7;
      }
    }
   #ifdef UNUSUAL_KATAKANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF5A; pd[1] = 0x30CF; return 0x30CF;
        case 'i': pd[0] = 0xFF5A; pd[1] = 0x30D2; return 0x30D2;
        case 'u': pd[0] = 0xFF5A; pd[1] = 0x30D5; return 0x30D5;
        case 'e': pd[0] = 0xFF5A; pd[1] = 0x30D8; return 0x30D8;
        case 'o': pd[0] = 0xFF5A; pd[1] = 0x30DB; return 0x30DB;
      }
    }
   #endif 
  }

  return 0;
}

_UDWORD Japan_Katakana33(_CHAR *pd, _CHAR *ps)
{
  // 3 zeichen durch 3 andere ersetzen (rot)

  if((ps[0] == 'y') && (ps[1] == 'y') & (ps[2] == 'e'))
  {
    pd[0] = 0x30C3;
    pd[1] = 0x30A4;
    pd[2] = 0x30A7;
    return 0x30A7;
  }
  return 0;
}


#endif // UC_JAPANINPUT
