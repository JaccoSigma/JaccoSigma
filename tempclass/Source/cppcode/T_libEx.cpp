//<NewSigmatekCFileOptimize/>
// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 03.02.2020                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#include "T_IPC.H"
#include "T_NUC.H"


void *lib_calloc(unsigned long size)
{
  // Speicher anfordern und mit 0 füllen
  void *ptr = NULL;
  if(Memory2((void**)&ptr, size) != 0)
  {
    MemSet(ptr, 0, size);
    return ptr;
  }
  
  return NULL;
}

void *lib_malloc(unsigned long size)
{
  // Speicher anfordern
  void *ptr = NULL;
  Memory2((void**)&ptr, size);
  return ptr;
}

void *lib_realloc(void *ptr, unsigned long size)
{
  // Speicher reallokieren
  if(Memory2((void**)&ptr, size) != 0)
  {
    return ptr;
  }
  
  return NULL;
}

void *lib_free(void *ptr)
{
  // Speicher freigeben
  if(ptr != NULL)
  {
    Memory2((void**)&ptr, 0);
  }
  return NULL;
}

unsigned long lib_memory(void **pptr, unsigned long size)
{
  // Speicher alloc, realloc oder free
  return Memory2(pptr, size);
}

void lib_memcpy(void *pdst, const void *psrc, unsigned long size)
{
  // Speicherinhalt kopieren
  MemCpy(pdst, psrc, size);
}

unsigned long lib_memcmp(void *p0, const void *p1, unsigned long size)
{
  // Speicherinhalt vergleichen
  return MemCmp(p0, p1, size);
}

// file ******************************

void *lib_fileread(unsigned long *psize, const char *dpne)
{
  // Funktion retourniert pointer auf Speicher mit Filecontent.

  void *ptr = NULL;

  if(psize != NULL)
  {
    *psize = 0;
  }

  _Handle hdl;

  if(hdl.Open(LOAD, dpne) == _TRUE)
  {
    long size = hdl.Length();
    if(size > 0)
    {
      unsigned long usize = (unsigned long)size;
      if(lib_memory(&ptr, usize) != 0)
      {
        if(hdl.Read(ptr, usize) == _TRUE)
        {
          if(psize != NULL)
          {
            *psize = size;
          }
        }
        else
        {
          lib_memory(&ptr, 0);
        }
      }
    }
  }
    
  hdl.Close();

  return ptr;
}


// string16 ***********************

unsigned long lib_strlen16(const unsigned short* str0)
{
  // Anzahl der Zeichen in einen U16 String zählen
  unsigned long retcode = 0;

  while(*str0)
  {
    str0++;
    retcode++;
  }
  return retcode;
}

unsigned long lib_strcut_chr16(unsigned short *str, unsigned short chr, unsigned long no)
{
  // funktion kürzt String bei erstem Vorkommen von no*chr
  unsigned long retcode = 0;
  
  if((no > 0) && (chr != 0))
  {
    while(*str)
    {
      if(*str++ == chr)
      {
        unsigned short *pe = str - 1;
        unsigned long  nox = no;
        do
        {
          nox--;
          if(nox == 0)
          {
            *pe = 0;
            return retcode;
          }
        }
        while(*str++ == chr);
      }
      
      retcode++;
    }
  }

  return retcode;
}

unsigned short *lib_strcpytill16(unsigned short *dst, const unsigned short *src, unsigned short tillchr)
{
  // kopiert alle Zeichen von 'src' auf 'dst' bis das zeichen 'tillchr' oder \0 aufgetreten ist.
  // retourniert wird ein Pointer auf das erste Zeichen nach 'tillchr' oder NULL

  if(src == NULL)
  {
    *dst = 0;
    return NULL;
  }
  
  unsigned short chr;
  do
  {
    chr    = *src++;
    *dst++ = chr;
  }
  while((chr != 0) && (chr != tillchr));

  dst--;
  unsigned short *retcode = (*dst == tillchr)? (unsigned short*)src : NULL;
  *dst = 0;
  
  return retcode;
}

unsigned long lib_delete_final16(unsigned short *p, unsigned short chr)
{
  // Löscht am Ende des Strings alle Vorkommen von 'chr' bis ein anderes Zeichen gefunden wird.
  // Retourniert wird die Zeichenlänge des ggf. neu entstandenen Strings
  // Bsp: "test  " wird zu "test" und retourniert wird 4

  unsigned long retcode = 0;

  while(*p++ != 0)
  {
    retcode ++;
  }

  if(retcode > 0)
  {
    p -= 2;
    while(*p == chr)
    {
      *p-- = 0;
      retcode --;
    }
  }
  
  return retcode;
}


// miscellaneous ********************

unsigned long lib_get_max_text_width(_CHAR **pptxt, unsigned long no)
{
  // funktion retourniert die maximale Pixelbreite der übergebenen Texte (pptxt + no).
  unsigned long retcode = 0;
  
  while(no--)
  {
    unsigned long xx = StrWidth((void*)pptxt[no], sizeof(_CHAR));
    if(xx > retcode)
    {
      retcode = xx;
    }
  }

  return retcode;  
}

static unsigned long name_unique_inc = 0;
char *lib_name_unique(char *pd, const char *name)
{
  if(name == NULL)
  {
    name = "Dummy";
  }

  unsigned long actinc = lib_atomic_incU32(&name_unique_inc);
  lib_sprintf(pd, "%s%u", name, actinc);
  
  return pd;
}

// atomic ****************************


#ifdef _LSL_TARGETARCH_X86

inline long lib_atomic_cmpxchg(volatile long *mem, long cmpVal, long newVal)
{
 #ifdef _MSC_VER
    __asm{
      mov eax, cmpVal
      mov ebx, newVal
      mov edi, mem
      lock cmpxchg [edi], ebx
    }
 #else
  long retVal;
	asm volatile( 
    "lock cmpxchgl %2, %1"
    : "=a" (retVal), "+m" (*mem)
    : "r" (newVal), "0" (cmpVal)
    : "memory"
  ); 
  return retVal;
 #endif
}

inline long lib_atomic_fetch_add(volatile long *mem, long addVal)
{
 #ifdef _MSC_VER
    __asm{
      mov edi, mem
      mov eax, addVal
      lock xadd [edi], eax
    }
 #else
  asm volatile(
    "lock xadd %0, (%1);"
    : "=a"(addVal)
    : "r"(mem), "a"(addVal)
    : "memory"
  );
  return addVal;
 #endif
}

unsigned long lib_atomic_incU32(unsigned long *pValue)
{
  // atomar increment, retourniert wird der wert VOR! der eigentlichen operation
  
  // Pseudocode:
  //   retcode = *pValue;
  //   *pValue++;
  //   return retcode;
  
  return lib_atomic_fetch_add((long*)pValue, 1);
}

unsigned long lib_atomic_decU32(unsigned long *pValue)
{
  // atomar decrement, retourniert wird der wert VOR! der eigentlichen operation
  
  // Pseudocode:
  //   retcode = *pValue;
  //   *pValue--;
  //   return retcode;
  
  return lib_atomic_fetch_add((long*)pValue, -1);
}

unsigned long lib_atomic_cmpxchgU32(unsigned long *pValue, unsigned long cmpVal, unsigned long newVal)
{
  // atomar cmpxchg
  
  // Pseudocode: 
  //   retcode = *pValue; 
  //   if(*pValue == cmpVal)
  //   {
  //     *pvalue = newVal;                 
  //   }
  //   return retcode;
  
  return (unsigned long)lib_atomic_cmpxchg((long*)pValue, (long)cmpVal, (long)newVal);
}


#endif
