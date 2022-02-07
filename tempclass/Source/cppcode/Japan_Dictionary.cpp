//<xxxNewSigmatekCFileOptimize/>
// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 24.02.2020                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#include "T_IPC.H"

/*

#ifdef UC_JAPANINPUT

#include "T_NUC.H"

// fileaufbau: UnicodeTextfile (alle zeichen sind unicode)
// 0xFF, 0xFE,
// key0 /text1/text2/text3/
// key1 /text/
// key2 /text/text/

// aufbau:
// [2] .... linewidth
// [2] .... Anzahl UNI-0-STRINGS
// *[ ..... key\0, text\0, text\0, ...

#pragma pack(push, 1)
typedef struct
{
  _UWORD  width; // including width
  _UWORD  nostr; // anzahl der strings, wobei erster string der key ist
  _UNI    data[2];
} JapDicRec;
#pragma pack(pop)

typedef struct
{
  _UDWORD    no_chr;
  _UDWORD    no_line;
  JapDicRec *ptr;
} tJapanDictionary;

typedef struct
{
  _UDWORD   ramsize;
  _UDWORD   offset;
  _UNI      *ptr;
} JapanFile;

void *JapanDictionaryFree(void *hdl)
{
  if(hdl != NULL)
  {
    tJapanDictionary *dict = (tJapanDictionary*)hdl;
    if(dict->ptr != NULL)
    {
      lib_free(dict->ptr);
    }
    lib_free(dict);
  }
  return NULL;
}

static _UNI *FgetU16(JapanFile *prf, _UNI *s, _UDWORD cellno)
{
  if(prf != NULL)
  {
    if((prf->ptr != NULL) && (cellno > 0) && (prf->offset < prf->ramsize))
    {
      cellno -= 1; // damit abschliessende 0 noch platz findet
      _UNI *ps = (_UNI*)s;
      _UNI *ph = &prf->ptr[prf->offset];
      while((cellno > 0) && (prf->offset < prf->ramsize))
      {
        *ps = *ph++;
        prf->offset++;
        if(*ps != 13)
        {
          if((*ps == 10) || (*ps == 0))
          {
            *ps = 0;
            return s;
          }
          ps++;
          cellno--;
        }
      }
        
      *ps = 0;
      return s;
    }
  }
  
  if(cellno > 1)
  {
    *s = 0;
  }
  return NULL;
}

static _UDWORD LineToRec(JapDicRec *prec, _UNI *line)
{
  prec->nostr = 0;
  prec->width = 2;
 
  if(lib_strcut_chr16(line, ';', 2) > 2) // cut at Gesamtkommentar ";;"
  {
    _UNI    key[128];
    _UNI    *p = line;
    _UDWORD len = 0;
    _UNI    *pd = prec->data;

    do
    {
      p = lib_strcpytill16(key, p, '/');  // copy bis zum '/'
      lib_strcut_chr16(key, ';', 1);      // cut at Einzelkommentar ";"
      len = lib_delete_final16(key, ' '); // delete final ' '
      
      if(len > 0)
      {
        len         += 1;           // final 0
        lib_memcpy(pd, key, len*2); // Token kopieren
        pd          += len;         // dst-pointer korrigieren
        prec->width += len;         // Anzahl der Zeichen korrigieren
        prec->nostr += 1;           // Anzahl der Tokens ++
      }
    }
    while(len != 0);
  }
  
  return (prec->nostr < 2)? 0 : prec->width;
}

void *JapanDictionaryLoad(const char *dpne)
{
  tJapanDictionary *dict = (tJapanDictionary*)lib_calloc(sizeof(tJapanDictionary));
  
  if(dict != NULL)
  {
    dict->no_line = 0;
    dict->no_chr  = 0;
  
    _UDWORD size;
    _UNI *praw = (_UNI*)lib_fileread(&size, dpne);
  
    if((praw != NULL) && (size > 2) && ((size & 1) == 0))
    {
      if(*praw == 0xFEFF) //  BOM
      {
        _UNI *pd = praw; 
        _UNI line[400];
        JapanFile jf;
        jf.ramsize = size / 2;
        jf.offset  = 1;    // BOM
        jf.ptr     = praw;
        
        while(FgetU16(&jf, line, lib_arraysize(line)) != NULL)
        {
          _UDWORD len = LineToRec((JapDicRec*)pd, line);
          if(len > 0)
          {
            pd += len;
            dict->no_chr += len;
            dict->no_line ++;
          }
        }
      
        dict->ptr = (JapDicRec*)praw;
        if(lib_memory((void**)&dict->ptr, dict->no_chr*2) != 0);
        {
          return (void*)dict;
        }
      }
    }
    
    lib_free(dict);
  }
  
  return NULL;
}

static _UDWORD JapanSeekIfAlreadyInserted(_UNI **pptxt, _UNI *p, _UDWORD no)
{
  while(no--)
  {
    if(StrCmp((void*)*pptxt, sizeof(_UNI), p, sizeof(_UNI)) == 0)
    {
      return 0;
    }
    pptxt++;
  }


  return 1;
}

static _UDWORD JapanDictionaryGetTextSub(_UNI **pptxt0, _UDWORD maxno, void *hdl, _UNI *key)
{
  _UDWORD retcode = 0;
  _UNI **pptxt = pptxt0;
  
  if((maxno > 0) && (pptxt != NULL) && (hdl != NULL) && (key != NULL))
  {
    _UDWORD keylen = lib_strlen16(key);
    if(keylen > 0)
    {
      tJapanDictionary *dict = (tJapanDictionary*)hdl;
    
      _UNI *ps = (_UNI*)dict->ptr;
      _UDWORD nox = dict->no_line;
      while(nox--)
      {
        JapDicRec *jdr = (JapDicRec*)ps;
        if(jdr->nostr > 1)
        {
          if(key[0] == jdr->data[0])
          {
            if(lib_memcmp(jdr->data, key, keylen*2) == 0)
            {
              _UDWORD noi = jdr->nostr - 1;
              _UNI   *ph  = jdr->data;
              while(noi--)
              {
                ph += lib_strlen16(ph) + 1;
                if(maxno == 0)
                {
                  return retcode;
                }
                
                if(JapanSeekIfAlreadyInserted(pptxt0, ph, retcode) != 0)
                {
                  pptxt[0] = ph;
                  pptxt ++;
                  maxno --;
                  retcode++;
                }
              }
            }
          }
        }
        
        ps += jdr->width;
      }
    }
  }

  return retcode;
}

_UDWORD JapanDictionaryGetText(_UNI **pptxt0, _UDWORD maxno, void *hdl, _UNI *key)
{
  _UDWORD retcode = 0;

  if(maxno > 0)
  {
    retcode = JapanDictionaryGetTextSub(pptxt0, maxno-1, hdl, key);

    if(retcode > 0)
    {
      if(JapanSeekIfAlreadyInserted(pptxt0, key, retcode) != 0)
      {
        pptxt0[retcode] = key;
        retcode ++;
      }
    }
  }
  
  return retcode;
}

#endif // UC_JAPANINPUT

*/

