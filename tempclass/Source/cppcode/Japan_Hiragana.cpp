//<NewSigmatekCFileOptimize/>
// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 09.11.2020                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#include "T_IPC.H"

#ifdef UC_JAPANINPUT

#include "T_NUC.H"

#define UNUSUAL_HIRAGANA
#define HIRAGANA_N        0x3093


_UDWORD Japan_Hiragana11(_CHAR *pd, _CHAR *ps)
{
  // 1 zeichen durch 1 anderes ersetzen (blau)
  switch(ps[0])
  {
    case 'a': pd[0] = 0x3042; return 0x3042;
    case 'i': pd[0] = 0x3044; return 0x3044;
    case 'u': pd[0] = 0x3046; return 0x3046;
    case 'e': pd[0] = 0x3048; return 0x3048;
    case 'o': pd[0] = 0x304A; return 0x304A;
//    case 'n': pd[0] = 0x006E; return 0x006E;
  }
  
  return 0;
}

_UDWORD Japan_Hiragana21(_CHAR *pd, _CHAR *ps)
{
  // 2 zeichen durch 1 anderes ersetzen (gelb)
  switch(ps[0])
  {
    case 'b':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x3070; return 0x3070;
        case 'i': pd[0] = 0x3073; return 0x3073;
        case 'u': pd[0] = 0x3076; return 0x3076;
        case 'e': pd[0] = 0x3079; return 0x3079;
        case 'o': pd[0] = 0x307C; return 0x307C;
      }
      break;
    case 'c':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x304B; return 0x304B;
        case 'i': pd[0] = 0x3057; return 0x3057;
        case 'u': pd[0] = 0x304F; return 0x304F;
        case 'e': pd[0] = 0x305B; return 0x305B;
        case 'o': pd[0] = 0x3053; return 0x3053;
      }
      break;
    case 'd':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x3060; return 0x3060;
        case 'i': pd[0] = 0x3062; return 0x3062;
        case 'u': pd[0] = 0x3065; return 0x3065;
        case 'e': pd[0] = 0x3067; return 0x3067;
        case 'o': pd[0] = 0x3069; return 0x3069;
      }
      break;
    case 'f':
      switch(ps[1])
      {
        case 'u': pd[0] = 0x3075; return 0x3075;
      }
      break;
    case 'g':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x304C; return 0x304C;
        case 'i': pd[0] = 0x304E; return 0x304E;
        case 'u': pd[0] = 0x3050; return 0x3050;
        case 'e': pd[0] = 0x3052; return 0x3052;
        case 'o': pd[0] = 0x3054; return 0x3054;
      }
      break;
    case 'h':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0x307B; return 0x307B;
      }
      break;
    case 'j':
      switch(ps[1])
      {
        case 'i': pd[0] = 0x3058; return 0x3058;
      }
      break;
    case 'k':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x304B; return 0x304B;
        case 'i': pd[0] = 0x304D; return 0x304D;
        case 'u': pd[0] = 0x304F; return 0x304F;
        case 'e': pd[0] = 0x3051; return 0x3051;
        case 'o': pd[0] = 0x3053; return 0x3053;
      }
      break;
    case 'l':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x3041; return 0x3041;
        case 'i': pd[0] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3045; return 0x3045;
        case 'e': pd[0] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3049; return 0x3049;
      }
      break;
    case 'm':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x307E; return 0x307E;
        case 'i': pd[0] = 0x307F; return 0x307F;
        case 'u': pd[0] = 0x3080; return 0x3080;
        case 'e': pd[0] = 0x3081; return 0x3081;
        case 'o': pd[0] = 0x3082; return 0x3082;
      }
      break;
    case 'n':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x306A; return 0x306A;
        case 'i': pd[0] = 0x306B; return 0x306B;
        case 'u': pd[0] = 0x306C; return 0x306C;
        case 'e': pd[0] = 0x306D; return 0x306D;
        case 'o': pd[0] = 0x306E; return 0x306E;
        case 'n': pd[0] = HIRAGANA_N; return HIRAGANA_N;
      }
      break;
    case 'p':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x3071; return 0x3071;
        case 'i': pd[0] = 0x3074; return 0x3074;
        case 'u': pd[0] = 0x3077; return 0x3077;
        case 'e': pd[0] = 0x307A; return 0x307A;
        case 'o': pd[0] = 0x307D; return 0x307D;
      }
      break;
    case 'q':
      switch(ps[1])
      {
        case 'u': pd[0] = 0x304F; return 0x304F;
      }
      break;
    case 'r':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x3089; return 0x3089;
        case 'i': pd[0] = 0x308A; return 0x308A;
        case 'u': pd[0] = 0x308B; return 0x308B;
        case 'e': pd[0] = 0x308C; return 0x308C;
        case 'o': pd[0] = 0x308D; return 0x308D;
      }
      break;
    case 's':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x3055; return 0x3055;
        case 'i': pd[0] = 0x3057; return 0x3057;
        case 'u': pd[0] = 0x3059; return 0x3059;
        case 'e': pd[0] = 0x305B; return 0x305B;
        case 'o': pd[0] = 0x305D; return 0x305D;
      }
      break;
    case 't':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x305F; return 0x305F;
        case 'i': pd[0] = 0x3061; return 0x3061;
        case 'u': pd[0] = 0x3064; return 0x3064;
        case 'e': pd[0] = 0x3066; return 0x3066;
        case 'o': pd[0] = 0x3068; return 0x3068;
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
        case 'a': pd[0] = 0x308F; return 0x308F;
        case 'u': pd[0] = 0x3046; return 0x3046;
        case 'o': pd[0] = 0x3092; return 0x3092;
      }
      break;
    case 'x':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x3041; return 0x3041;
        case 'i': pd[0] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3045; return 0x3045;
        case 'e': pd[0] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3049; return 0x3049;
        case 'n': pd[0] = HIRAGANA_N; return HIRAGANA_N;
      }
      break;
    case 'y':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x3084; return 0x3084;
        case 'i': pd[0] = 0x3044; return 0x3044;
        case 'u': pd[0] = 0x3086; return 0x3086;
        case 'o': pd[0] = 0x3088; return 0x3088;
      }
      break;
    case 'z':
      switch(ps[1])
      {
        case 'a': pd[0] = 0x3056; return 0x3056;
        case 'i': pd[0] = 0x3058; return 0x3058;
        case 'u': pd[0] = 0x305A; return 0x305A;
        case 'e': pd[0] = 0x305C; return 0x305C;
        case 'o': pd[0] = 0x305E; return 0x305E;
      }
      break;
  }
  
  return 0;
}

_UDWORD Japan_Hiragana31(_CHAR *pd, _CHAR *ps)
{
  // 3 zeichen durch 1 anderes ersetzen (magenta)
  if((ps[0] == 'l') && (ps[1] == 'y'))
  {
    if(ps[2] == 'a')
    {
      pd[0] = 0x3083;
      return 0x3083;
    }
    if(ps[2] == 'i')
    {
      pd[0] = 0x3043;
      return 0x3043;
    }
    if(ps[2] == 'u')
    {
      pd[0] = 0x3085;
      return 0x3085;
    }
    if(ps[2] == 'e')
    {
      pd[0] = 0x3047;
      return 0x3047;
    }
    if(ps[2] == 'o')
    { 
      pd[0] = 0x3087;
      return 0x3087;
    }
  }
  else if((ps[0] == 'w') && (ps[1] == 'y'))
  {
    if(ps[2] == 'i')
    {
      pd[0] = 0x3090;
      return 0x3090;
    }
    if(ps[2] == 'e')
    {
      pd[0] = 0x3091;
      return 0x3091;
    }
  }
  else if((ps[0] == 'x') && (ps[1] == 'y'))
  {
    if(ps[2] == 'a')
    {
      pd[0] = 0x3083;
      return 0x3083;
    }
    if(ps[2] == 'i')
    {
      pd[0] = 0x3043;
      return 0x3043;
    }
    if(ps[2] == 'u')
    {
      pd[0] = 0x3085;
      return 0x3085;
    }
    if(ps[2] == 'e')
    {
      pd[0] = 0x3047;
      return 0x3047;
    }
    if(ps[2] == 'o')
    { 
      pd[0] = 0x3087;
      return 0x3087;
    }
  }
  else if(ps[1] == 'h')
  {
    if((ps[0] == 'c') && /*(ps[1] == 'h') &&*/ (ps[2] == 'i'))
    {
      pd[0] = 0x3061;
      return 0x3061;
    }
    if((ps[0] == 'h') && /*(ps[1] == 'h') &&*/ (ps[2] == 'o'))
    {
      pd[0] = 0x307B;
      return 0x307B;
    }
    if((ps[0] == 's') && /*(ps[1] == 'h') &&*/ (ps[2] == 'i'))
    {
      pd[0] = 0x3057;
      return 0x3057;
    }
    if((ps[0] == 'w') && /*(ps[1] == 'h') &&*/ (ps[2] == 'u'))
    {
      pd[0] = 0x3046;
      return 0x3046;
    }
  }
  
  return 0;
}

_UDWORD Japan_Hiragana22(_CHAR *pd, _CHAR *ps)
{
  // 2 zeichen durch 2 andere ersetzen (grün)
  switch(ps[0])
  {
   #ifdef UNUSUAL_HIRAGANA
    case 'b':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF42; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif   
   #ifdef UNUSUAL_HIRAGANA
    case 'c':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF43; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif   
   #ifdef UNUSUAL_HIRAGANA
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
        case 'a': pd[0] = 0x3075; pd[1] = 0x3041; return 0x3041;
        case 'i': pd[0] = 0x3075; pd[1] = 0x3043; return 0x3043;
        case 'e': pd[0] = 0x3075; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3075; pd[1] = 0x3049; return 0x3049;
       #ifdef UNUSUAL_HIRAGANA
        case 'n': pd[0] = 0xFF46; pd[1] = 0xFF4E; return 0xFF4E;
       #endif 
      }
      break;
   #ifdef UNUSUAL_HIRAGANA
    case 'g':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF47; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif   
   #ifdef UNUSUAL_HIRAGANA
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
        case 'a': pd[0] = 0x3058; pd[1] = 0x3083; return 0x3083;
        case 'u': pd[0] = 0x3058; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3058; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3058; pd[1] = 0x3087; return 0x3087;
       #ifdef UNUSUAL_HIRAGANA
        case 'n': pd[0] = 0xFF4A; pd[1] = 0xFF4E; return 0xFF4E;
       #endif 
      }
      break;
   #ifdef UNUSUAL_HIRAGANA
    case 'k':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF4B; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif      
   #ifdef UNUSUAL_HIRAGANA
    case 'l':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF4C; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif
   #ifdef UNUSUAL_HIRAGANA
    case 'm':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF4D; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif      
   #ifdef UNUSUAL_HIRAGANA
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
        case 'a': pd[0] = 0x304F; pd[1] = 0x3041; return 0x3041;
        case 'i': pd[0] = 0x304F; pd[1] = 0x3043; return 0x3043;
        case 'e': pd[0] = 0x304F; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x304F; pd[1] = 0x3049; return 0x3049;
       #ifdef UNUSUAL_HIRAGANA
        case 'n': pd[0] = 0xFF51; pd[1] = 0xFF4E; return 0xFF4E;
       #endif
      }
      break;
   #ifdef UNUSUAL_HIRAGANA
    case 'r':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF52; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif      
   #ifdef UNUSUAL_HIRAGANA
    case 's':
      switch(ps[1])
      {
        case 'n': pd[0] = 0xFF53; pd[1] = 0xFF4E; return 0xFF4E;
      }
      break;
   #endif      
   #ifdef UNUSUAL_HIRAGANA
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
        case 'a': pd[0] = 0x304F; pd[1] = 0x3041; return 0x3041;
        case 'i': pd[0] = 0x304F; pd[1] = 0x3043; return 0x3043;
        case 'e': pd[0] = 0x304F; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x304F; pd[1] = 0x3049; return 0x3049;
       #ifdef UNUSUAL_HIRAGANA
        case 'n': pd[0] = 0xFF56; pd[1] = 0xFF4E; return 0xFF4E;
       #endif 
      }
      break;
    case 'w':
      switch(ps[1])
      {
        case 'i': pd[0] = 0x3046; pd[1] = 0x3043; return 0x3043;
        case 'e': pd[0] = 0x3046; pd[1] = 0x3047; return 0x3047;
       #ifdef UNUSUAL_HIRAGANA
        case 'n': pd[0] = 0xFF57; pd[1] = 0xFF4E; return 0xFF4E;
       #endif 
      }
      break;
    case 'y':
      switch(ps[1])
      {
        case 'e': pd[0] = 0x3044; pd[1] = 0x3047; return 0x3047;
       #ifdef UNUSUAL_HIRAGANA
        case 'n': pd[0] = 0xFF59; pd[1] = 0xFF4E; return 0xFF4E;
       #endif 
      }
      break;
   #ifdef UNUSUAL_HIRAGANA
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

_UDWORD Japan_Hiragana32(_CHAR *pd, _CHAR *ps)
{
  // 3 zeichen durch 2 andere ersetzen (orange)
  if(ps[0] == 'b') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3073; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3073; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3073; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3073; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3073; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF42; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF42; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF42; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF42; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF42; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x3061; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3061; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3061; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3061; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3061; pd[1] = 0x3087; return 0x3087;
      }
    }
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3061; pd[1] = 0x3083; return 0x3083;
        case 'u': pd[0] = 0x3061; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3061; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3061; pd[1] = 0x3087; return 0x3087;
      }
    }
  }
  else if(ps[0] == 'd') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3062; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3062; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3062; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3062; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3062; pd[1] = 0x3087; return 0x3087;
      }
    }
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3067; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3067; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3067; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3067; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3067; pd[1] = 0x3087; return 0x3087;
      }
    }
  }
  else if(ps[0] == 'f') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3075; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3075; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3075; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3075; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3075; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF46; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF46; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF46; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF46; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF46; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x304E; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x304E; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x304E; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x304E; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x304E; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF47; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF47; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF47; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF47; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF47; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x3072; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3072; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3072; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3072; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3072; pd[1] = 0x3087; return 0x3087;
      }
    }
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3063; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0x3063; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0x3063; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0x3063; pd[1] = 0x3078; return 0x3078;
      }
    }
  }
  else if(ps[0] == 'j') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3058; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3058; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3058; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3058; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3058; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF4A; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF4A; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF4A; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF4A; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF4A; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x304D; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x304D; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x304D; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x304D; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x304D; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF4B; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF4B; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF4B; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF4B; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF4B; pd[1] = 0x307B; return 0x307B;
      }
    }
   #endif 
  }
 #ifdef UNUSUAL_HIRAGANA
  else if(ps[0] == 'l') 
  {
    if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF4C; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF4C; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF4C; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF4C; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF4C; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x307F; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x307F; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x307F; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x307F; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x307F; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF4D; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF4D; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF4D; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF4D; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF4D; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x306B; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x306B; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x306B; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x306B; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x306B; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3093; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0x3093; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0x3093; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0x3093; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0x3093; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x3074; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3074; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3074; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3074; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3074; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF50; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF50; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF50; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF50; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF50; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x304F; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x304F; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x304F; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x304F; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x304F; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF51; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF51; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF51; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF51; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF51; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x308A; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x308A; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x308A; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x308A; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x308A; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF52; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF52; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF52; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF52; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF52; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x3057; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3057; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3057; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3057; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3057; pd[1] = 0x3087; return 0x3087;
      }
    }
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3057; pd[1] = 0x3083; return 0x3083;
        case 'u': pd[0] = 0x3057; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3057; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3057; pd[1] = 0x3087; return 0x3087;
      }
    }
  }
  else if(ps[0] == 't') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3061; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3061; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3061; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3061; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3061; pd[1] = 0x3087; return 0x3087;
      }
    }
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3066; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3066; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3066; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3066; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3066; pd[1] = 0x3087; return 0x3087;
      }
    }
  }
  else if(ps[0] == 'v') 
  {
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x30F4; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x30F4; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x30F4; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x30F4; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x30F4; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF56; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF56; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF56; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF56; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF56; pd[1] = 0x307B; return 0x307B;
      }
    }
   #endif 
  }
  else if(ps[0] == 'w')
  {
   #ifdef UNUSUAL_HIRAGANA
    if(ps[1] == 'y')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF57; pd[1] = 0x3084; return 0x3084;
        case 'u': pd[0] = 0xFF57; pd[1] = 0x3086; return 0x3086;
        case 'o': pd[0] = 0xFF57; pd[1] = 0x3088; return 0x3088;
      }
    }
   #endif
    if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0x3046; pd[1] = 0x3041; return 0x3041;
        case 'i': pd[0] = 0x3046; pd[1] = 0x3043; return 0x3043;
        case 'e': pd[0] = 0x3046; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3046; pd[1] = 0x3049; return 0x3049;
      }
    }
  }
 #ifdef UNUSUAL_HIRAGANA
  else if(ps[0] == 'x')
  {
    if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF58; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF58; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF58; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF58; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF58; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x3063; pd[1] = 0x3084; return 0x3084;
        case 'i': pd[0] = 0x3063; pd[1] = 0x3044; return 0x3044;
        case 'u': pd[0] = 0x3063; pd[1] = 0x3086; return 0x3086;
//        case 'e': pd[0] = 0x3063; pd[1] = 0x3044; return 0x3044;
        case 'o': pd[0] = 0x3063; pd[1] = 0x3088; return 0x3088;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF59; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF59; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF59; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF59; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF59; pd[1] = 0x307B; return 0x307B;
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
        case 'a': pd[0] = 0x3058; pd[1] = 0x3083; return 0x3083;
        case 'i': pd[0] = 0x3058; pd[1] = 0x3043; return 0x3043;
        case 'u': pd[0] = 0x3058; pd[1] = 0x3085; return 0x3085;
        case 'e': pd[0] = 0x3058; pd[1] = 0x3047; return 0x3047;
        case 'o': pd[0] = 0x3058; pd[1] = 0x3087; return 0x3087;
      }
    }
   #ifdef UNUSUAL_HIRAGANA
    else if(ps[1] == 'h')
    {
      switch(ps[2])
      {
        case 'a': pd[0] = 0xFF5A; pd[1] = 0x306F; return 0x306F;
        case 'i': pd[0] = 0xFF5A; pd[1] = 0x3072; return 0x3072;
        case 'u': pd[0] = 0xFF5A; pd[1] = 0x3075; return 0x3075;
        case 'e': pd[0] = 0xFF5A; pd[1] = 0x3078; return 0x3078;
        case 'o': pd[0] = 0xFF5A; pd[1] = 0x307B; return 0x307B;
      }
    }
   #endif 
  }
  
  return 0; 
}

_UDWORD Japan_Hiragana33(_CHAR *pd, _CHAR *ps)
{
  // 3 zeichen durch 3 andere ersetzen (rot)

  if((ps[0] == 'y') && (ps[1] == 'y') & (ps[2] == 'e'))
  {
    pd[0] = 0x3063;
    pd[1] = 0x3044;
    pd[2] = 0x3047;
    return 0x3047;
  }
  return 0;
}

#endif // UC_JAPANINPUT
