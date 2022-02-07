//<xxxNewSigmatekCFileOptimize/>
// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 03.02.2020                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#include "T_IPC.H"

#ifdef UC_JAPANINPUT

#include "T_NUC.H"

typedef _UDWORD (*_japan_ctb) (_CHAR*, _CHAR*);

// Japan_Hiragana.cpp *****************************************************************************
_UDWORD Japan_Hiragana11(_CHAR *pd, _CHAR *ps);
_UDWORD Japan_Hiragana21(_CHAR *pd, _CHAR *ps);
_UDWORD Japan_Hiragana22(_CHAR *pd, _CHAR *ps);
_UDWORD Japan_Hiragana31(_CHAR *pd, _CHAR *ps);
_UDWORD Japan_Hiragana32(_CHAR *pd, _CHAR *ps);
_UDWORD Japan_Hiragana33(_CHAR *pd, _CHAR *ps);

// Japan_Katakana.cpp *****************************************************************************
_UDWORD Japan_Katakana11(_CHAR *pd, _CHAR *ps);
_UDWORD Japan_Katakana21(_CHAR *pd, _CHAR *ps);
_UDWORD Japan_Katakana22(_CHAR *pd, _CHAR *ps);
_UDWORD Japan_Katakana31(_CHAR *pd, _CHAR *ps);
_UDWORD Japan_Katakana32(_CHAR *pd, _CHAR *ps);
_UDWORD Japan_Katakana33(_CHAR *pd, _CHAR *ps);

//static void *JapanDictionary = NULL;

void Japan_cTor(void)
{
//  JapanDictionary = JapanDictionaryLoad(UC_JAPANINPUT);
}

_UDWORD Japan_InputMode(void)
{
 #ifdef UC_JAPANINPUT 
  if(Language.GetActualLocal() == LOCAL_JAPANESE)
  {
    if(VarList.VarDummyGet(VARNO_JAPANESE_INPUT) != 0)
    {
      return 1 + VarList.VarDummyGet(VARNO_HIRAGANA_KATAGANA);
    }
  }
 #endif

  return 0;
}

void *Japan_GetDictionary(void)
{
  return NULL; //JapanDictionary;
}

_UDWORD AsciiToJapanesePosition(_CHAR *dst, _UWORD *ppos, _CHAR *src, _UWORD japan_begin_pos)
{
  _UDWORD retcode = 0;
  _CHAR *ps = src;
  _CHAR *pd = dst;
  _UWORD pos = 0; 
 
  _japan_ctb fp11, fp21, fp22, fp31, fp32, fp33;

  if(ppos != NULL)
  {
    pos = *ppos;
  }

  if(japan_begin_pos < pos)
  {
    retcode = japan_begin_pos;
    
    if(pd != ps)
    {
      _UDWORD nox = japan_begin_pos;
      while(nox--)
      {
        *pd++ = *ps++;
      }
    }
    else
    {
      pd += japan_begin_pos;
      ps += japan_begin_pos;
    }
  
    if(VarList.VarDummyGet(VARNO_HIRAGANA_KATAGANA) == 0)
    {
      fp11 = Japan_Hiragana11;
      fp21 = Japan_Hiragana21;
      fp22 = Japan_Hiragana22;
      fp31 = Japan_Hiragana31;
      fp32 = Japan_Hiragana32;
      fp33 = Japan_Hiragana33;
    }
    else
    {
      fp11 = Japan_Katakana11;
      fp21 = Japan_Katakana21;
      fp22 = Japan_Katakana22;
      fp31 = Japan_Katakana31;
      fp32 = Japan_Katakana32;
      fp33 = Japan_Katakana33;
    }
    
    while(*ps != 0)
    {
      if(fp33(pd, ps) != 0)
      {
        ps      += 3;
        pd      += 3;
        retcode += 3;
      }
      else if(fp32(pd, ps) != 0)
      {
        ps      += 3;
        pd      += 2;
        retcode += 2;
          
        if(pos > retcode)
        { 
          pos -= 1;
        }
      }
      else if(fp31(pd, ps) != 0)
      {        
        ps      += 3;
        pd      += 1; 
        retcode += 1;
        
        if(pos > retcode)
        { 
          pos = (pos > 2)? (pos - 2) : 0;
        }
      }
      else if(fp21(pd, ps) != 0)
      {
        ps      += 2;
        pd      += 1;
        retcode += 1;
        if(pos > retcode) 
        { 
          pos -= 1;
        }
      }
      else if(fp22(pd, ps) != 0)
      {
        ps      += 2;
        pd      += 2;
        retcode += 2;
      }
      else if(fp11(pd, ps) != 0)
      {
        ps      += 1;
        pd      += 1;
        retcode += 1;
      }
      else
      {
        pd[0]    = ps[0];
        ps      += 1;
        pd      += 1;
        retcode += 1;
      }
    }
    
    pd[0] = 0;
    
    if(ppos != NULL)
    {
      *ppos = pos;
    }
  }
  
  return retcode;
}

//_UDWORD AsciiToJapanese(_CHAR *dst, _CHAR *src)
//{
//  return AsciiToJapanesePosition(dst, NULL, src, 0);
//}

#endif // UC_JAPANINPUT