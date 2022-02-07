//<NewSigmatekCFileOptimize/>
// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 26.07.2019                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#include "SigCLib.h"

static const unsigned char TabBase64Encode[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char TabBase64Decode[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                                 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                                 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3E,0xFF,0xFF,0xFF,0x3F,
                                                 0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                                 0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,
                                                 0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0xFF,0xFF,0xFF,0xFF,0xFF,
                                                 0xFF,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
                                                 0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,0x33,0xFF,0xFF,0xFF,0xFF,0xFF};

// ************************************************************************************************
// ************************************************************************************************
// utf8
// ************************************************************************************************
// ************************************************************************************************

unsigned long sigclib_str16_to_utf8(void *dst_utf8, const void *src_uni)
{
  // Note: gleicher Algorithmus wird in Funktion sigclib_utf8_len() verwendet

  unsigned long  retcode = 0;
  unsigned short *ps = (unsigned short*)src_uni;
  unsigned char  *pd = (unsigned char*)dst_utf8;

  while(*ps)
  {
    unsigned short chr = *ps++;
    if(chr < 0x0080)
    {
      *pd++    = *(unsigned char*)&chr;
      retcode += 1;
    }
    else if(chr < 0x0800)
    {
      *pd++    = (unsigned char)(0x00C0 | (chr >> 6));
      *pd++    = (unsigned char)(0x0080 | (chr & 0x003F));
      retcode += 2;
    }
    else
    {
      *pd++    = (unsigned char)(0x00E0 | (chr >> 12));
      *pd++    = (unsigned char)(0x0080 | ((chr >> 6) & 0x003F));
      *pd++    = (unsigned char)(0x0080 | (chr & 0x003F));
      retcode += 3;
    }
  }
  *pd = 0;
  
  return retcode;
}

unsigned long sigclib_utf8_to_str16(void *dst_uni, const void *src_utf8)
{
  unsigned long retcode = 0;
  unsigned char  *ps = (unsigned char*)src_utf8;
  unsigned short *pd = (unsigned short*)dst_uni;
  unsigned long  follow;

  while(*ps)
  {
    unsigned short chr = *ps++;
    if(chr & 0x0080)
    {
      follow = 0;
      
      if((chr & 0x00E0) == 0x00C0)
      {
        follow = 1; // 1 folgebyte
        chr    = chr & 0x001F;
      }
      else if((chr & 0x00F0) == 0x00E0)
      {
        follow = 2; // 2 folgebytes
        chr    = chr & 0x000F;
      }
      else if((chr & 0x00F8) == 0x00F0)
      {
        follow = 3; // 3 folgebytes
        chr    = chr & 0x0007;
      }
      
      while(follow--)
      {
        if((*ps & 0xC0) == 0x80)
        {
          chr = ((chr << 6) | ((*ps++) & 0x3F));
          if(follow == 0)
          {
            *pd++ = chr;
            retcode ++;
          }
        }
      }
    }
    else
    {
      *pd++ = chr;
      retcode ++;
    }
  }
  *pd = 0;

  return retcode;
}

unsigned long sigclib_ascii_to_utf8(void *dst_utf8, const void *src_ascii)
{
  // Note: gleicher Algorithmus wird in Funktion sigclib_utf8_len() verwendet
  
  unsigned long retcode = 0;
  unsigned char *ps = (unsigned char*)src_ascii;
  unsigned char *pd = (unsigned char*)dst_utf8;

  while(*ps)
  {
    unsigned char chr = *ps++;
    if(chr < 0x0080)
    {
      *pd++    = *(unsigned char*)&chr;
      retcode += 1;
    }
    else 
    {
      *pd++    = (unsigned char)(0x00C0 | (chr >> 6));
      *pd++    = (unsigned char)(0x0080 | (chr & 0x003F));
      retcode += 2;
    }
  }
  *pd = 0;
  
  return retcode;
}

unsigned long sigclib_ascii_to_utf8_len(const void *src_ascii)
{
  // funktion berechnet die Länge des utf8 codierten String anhand von src in ASCII
  // --> src ....... string to compute (ASCII-0-String)
  // retourniert wird die Anzahl der Zeichen ohne final-0, entspricht dem Ergebnis von sigclib_strlen()
  
  // Vorsicht: Muss gleicher Algorithmus wie bei Funktion sigclib_ascii_to_utf8() sein
  
  unsigned long retcode = 0;
  unsigned char *ps = (unsigned char*)src_ascii;
  while(*ps)
  {
    retcode ++;
    unsigned char chr = *ps++;
    if(chr & 0x80)
    {
      retcode ++;
    }
  }
  
  return retcode;
}

unsigned long sigclib_str16_to_utf8_len(const void *src_uni)
{
  // funktion berechnet die Länge des utf8 codierten String anhand von src in UNICODE
  // --> src ....... string to compute (UNI-0-String)
  // retourniert wird die Anzahl der Zeichen ohne final-0, entspricht dem Ergebnis von sigclib_strlen()
  
  // Vorsicht: Muss gleicher Algorithmus wie bei Funktion sigclib_str16_to_utf8() sein
  
  unsigned long retcode = 0;
  
  unsigned short *ps = (unsigned short*)src_uni;
  while(*ps)
  {
    unsigned short chr = *ps++;
    if(chr < 0x0080)
    {
      retcode += 1;
    }
    else if(chr < 0x0800)
    {
      retcode += 2;
    }
    else
    {
      retcode += 3;
    }
  }
  
  return retcode;
}

unsigned long sigclib_utf8_to_str16_len(const void *src_utf8)
{
  unsigned long retcode = 0;
  unsigned char *ps = (unsigned char*)src_utf8;
  unsigned long follow;

  // Vorsicht: Muss gleicher Algorithmus wie bei Funktion sigclib_utf8_to_str16() sein

  while(*ps)
  {
    unsigned short chr = *ps++;
    if(chr & 0x0080)
    {
      follow = 0;
      
      if((chr & 0x00E0) == 0x00C0)
      {
        follow = 1; // 1 folgebyte
      }
      else if((chr & 0x00F0) == 0x00E0)
      {
        follow = 2; // 2 folgebytes
      }
      else if((chr & 0x00F8) == 0x00F0)
      {
        follow = 3; // 3 folgebytes
      }
      
      while(follow--)
      {
        if((*ps & 0xC0) == 0x80)
        {
          if(follow == 0)
          {
            retcode ++;
          }
        }
      }
    }
    else
    {
      retcode ++;
    }
  }

  return retcode;
}

unsigned long sigclib_utf8_to_ascii_len(const void *src_utf8)
{
  return sigclib_utf8_to_str16_len(src_utf8);
}

unsigned long sigclib_utf8_to_ascii(void *dst_ascii, const void *src_utf8)
{
  unsigned long retcode = 0;
  unsigned char *ps = (unsigned char*)src_utf8;
  unsigned char *pd = (unsigned char*)dst_ascii;
  unsigned long follow;

  // Vorsicht: Muss gleicher Algorithmus wie bei Funktion sigclib_utf8_to_ascii_len() sein

  while(*ps)
  {
    unsigned short chr = *ps++;
    if(chr & 0x0080)
    {
      follow = 0;
      
      if((chr & 0x00E0) == 0x00C0)
      {
        follow = 1; // 1 folgebyte
        chr    = chr & 0x001F;
      }
      else if((chr & 0x00F0) == 0x00E0)
      {
        follow = 2; // 2 folgebytes
        chr    = chr & 0x000F;
      }
      else if((chr & 0x00F8) == 0x00F0)
      {
        follow = 3; // 3 folgebytes
        chr    = chr & 0x0007;
      }
      
      while(follow--)
      {
        if((*ps & 0xC0) == 0x80)
        {
          chr = ((chr << 6) | ((*ps++) & 0x3F));
          if(follow == 0)
          {
            *pd++ = (unsigned char)((chr < 256)? chr : '?');
            retcode ++;
          }
        }
      }
    }
    else
    {
      *pd++ = (unsigned char)chr;
      retcode ++;
    }
  }
  *pd = 0;

  return retcode;
}

// ************************************************************************************************
// ************************************************************************************************
// utf16
// ************************************************************************************************
// ************************************************************************************************

typedef unsigned short tUTF16;
typedef unsigned char  tUTF8;

static const unsigned char FirstByteMark[7]   = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
static const unsigned long OffsetsFromUTF8[6] = { 0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL };
static const unsigned char BytesFromUTF8[256] = {	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                                  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5 };

#define UTF16_ReplaceChr      0x0000FFFDUL
#define UTF16_SurrogateHighStart  0xD800UL
#define UTF16_SurrogateHighEnd    0xDBFFUL
#define UTF16_SurrogateLowStart   0xDC00UL
#define UTF16_SurrogateLowEnd     0xDFFFUL

static unsigned long sigclib_utf16_to_utf8_intern(tUTF8 **ppd, const tUTF8 *pdend, tUTF16 **pps, const tUTF16 *psend)
{
  unsigned long retcode = 0;
  tUTF16 *ps = *pps;
  tUTF8  *pd = *ppd;

  while(ps < psend)
  {
    unsigned long byteno = 2;
    unsigned long chr = *ps++;

    if((chr >= UTF16_SurrogateHighStart) && (chr <= UTF16_SurrogateHighEnd) && (ps < psend))
    {
      unsigned long chr2 = *ps;
      if((chr2 >= UTF16_SurrogateLowStart) && (chr2 <= UTF16_SurrogateLowEnd))
      {
        chr = ((chr - UTF16_SurrogateHighStart) << 10) + (chr2 - UTF16_SurrogateLowStart) + 0x0010000UL;
        ps++;
      }
    }

    if(chr < 0x80)               { byteno = 1; }
    else if(chr < 0x800)         { byteno = 2; }
    else if(chr < 0x10000)       { byteno = 3; }
    else if(chr < 0x200000)      { byteno = 4; }
    else if(chr < 0x4000000)     { byteno = 5; }
    else if(chr <= 0x7FFFFFFFUL) { byteno = 6; }
    else { chr = UTF16_ReplaceChr; }

    pd += byteno;

    if(pd > pdend)
    {
      pd -= byteno;
      retcode = 1; // destbuffer zu klein
      break;
    }

    switch(byteno) // note: "Durchläufer"
    {
      case 6: *--pd = (chr|0x80) & 0xBF; chr >>= 6;
      case 5: *--pd = (chr|0x80) & 0xBF; chr >>= 6;
      case 4: *--pd = (chr|0x80) & 0xBF; chr >>= 6;
      case 3: *--pd = (chr|0x80) & 0xBF; chr >>= 6;
      case 2: *--pd = (chr|0x80) & 0xBF; chr >>= 6;
      case 1: *--pd = chr|FirstByteMark[byteno];
    }

    pd += byteno;
  }

  *pps = ps;
  *ppd = pd;

  return retcode;
}

static unsigned long sigclib_utf8_to_utf16_intern(tUTF16 **ppd, const tUTF16 *pdend, tUTF8 **pps, const tUTF8 *psend)
{
  unsigned long retcode = 0;
  tUTF8  *ps = *pps;
  tUTF16 *pd = *ppd;

  while(ps < psend)
  {
    unsigned long chr = 0;
    unsigned long ex_bytes = BytesFromUTF8[*ps];

    if((ps + ex_bytes) > psend)
    {
      retcode = 2; // fehler sourcebuffer
      break;
    }

    switch(ex_bytes) // note: "Durchläufer"
    {
      case 5: chr += *ps++; chr <<= 6;
      case 4: chr += *ps++; chr <<= 6;
      case 3: chr += *ps++; chr <<= 6;
      case 2: chr += *ps++; chr <<= 6;
      case 1: chr += *ps++; chr <<= 6;
      case 0: chr += *ps++;
    }

    chr -= OffsetsFromUTF8[ex_bytes];

    if(pd >= pdend)
    {
      retcode = 1; // destbuffer zu klein
      break;
    }

    if(chr <= 0x0000FFFFUL)
    {
      *pd++ = chr;
    }
    else if(chr > 0x0010FFFFUL)
    {
      *pd++ = UTF16_ReplaceChr;
    }
    else
    {
      if((pd + 1) >= pdend)
      {
        retcode = 1; // destbuffer zu klein
        break;
      }
      
      chr   -= 0x0010000UL;
      *pd++ = (chr >> 10) + UTF16_SurrogateHighStart;
      *pd++ = (chr & 0x3FFUL) + UTF16_SurrogateLowStart;
    }
  }

  *pps = ps;
  *ppd = pd;

  return retcode;
}

unsigned long sigclib_utf8_to_utf16(unsigned short *pdst_utf16, unsigned long dst_bytesize, const char *psrc_utf8, unsigned long no)
{
  // convert UTF8 to UTF16
  // --> pdst_utf16 ...... destinationbuffer
  // --> dst_bytesize .... bytesize of destinationbuffer
  // --> psrc_utf8........ UTF8-coded sourcebuffer
  // --> no .............. length UTF8-coded sourcebuffer
  // function will return length of UTF16-codec destinationstring or 0 in case of error (destinationbuffer too small, format-error in sourcebuffer)

  unsigned short *pdst = pdst_utf16;
  unsigned char  *psrc = (unsigned char*)psrc_utf8;
  unsigned long  dmax  = dst_bytesize / 2;
  if(sigclib_utf8_to_utf16_intern(&pdst, pdst + dmax, &psrc, psrc + no) == 0)
  {
    unsigned long len = pdst - pdst_utf16; // das ist strlen16(utf16)
    if(len < dmax)
    {
      *pdst = 0;
    }
    return len;
  }
  
  return 0;
}

unsigned long sigclib_utf16_to_utf8(unsigned char *pdst_utf8, unsigned long dst_bytesize, unsigned short *psrc_utf16, unsigned long no)
{
  // convert UTF16 to UTF8
  // --> pdst_utf8 ....... destinationbuffer
  // --> dst_byte_size ... bytesize of destinationbuffer
  // --> psrc_utf16 ...... UTF16-coded sourcebuffer
  // --> no .............. length of UTF16-coded sourcebuffer
  // function will return length of UTF8-codec destinationstring or 0 in case of error (destinationbuffer too small, format-error in sourcebuffer)
  
  unsigned char  *pdst = pdst_utf8;
  unsigned short *psrc = psrc_utf16;
  if(sigclib_utf16_to_utf8_intern(&pdst, pdst+dst_bytesize, &psrc, psrc+no) == 0)
  {
    unsigned long len = pdst - pdst_utf8; // das ist strlen(utf8)
    if(len < dst_bytesize)
    {
      *pdst = 0;
    }
    return len;
  }
  
  return 0;
}

// ************************************************************************************************
// ************************************************************************************************
// Base64
// ************************************************************************************************
// ************************************************************************************************

unsigned long sigclib_base64_encode_size(unsigned long srcsize)
{
  unsigned long iteration = (srcsize + 2) / 3;
  return 4 * iteration;
}

unsigned long sigclib_base64_encode(unsigned char *dst, unsigned long dstsize, void *src0, unsigned long srcsize)
{
  if((srcsize > 0) && (src0 != NULL) && (dst != NULL))
  {
    unsigned char *src      = (unsigned char*)src0;
    unsigned long iteration = (srcsize + 2) / 3;
    unsigned long retcode   = (4 * iteration);
    long srclen             = (long)srcsize;
    
    if(dstsize >= retcode)
    {
      while(iteration--)
      {
        unsigned char byte, rest, idx;
        
        byte   = *src++;
        *dst++ = TabBase64Encode[byte >> 2];
        rest   = byte & 0x03;
        srclen --;
    
        byte = (srclen > 0)? *src++ : 0;
        *dst++ = TabBase64Encode[(rest << 4) | (byte >> 4)];
        rest = byte & 0x0F;
        srclen --;

        byte = (srclen > 0)? *src++ : 0;
        idx = (rest << 2) | (byte >> 6);
        rest = byte & 0x3F;
        *dst++ = (srclen > -1)? TabBase64Encode[idx] : '=';
        srclen --;

        *dst++ = (srclen > -1)? TabBase64Encode[rest] : '=';
      }
      
      return retcode;
    }
  }
  
  return 0;
}

unsigned long sigclib_base64_decode(void *dst0, unsigned long dstsize, const unsigned char *src, unsigned long srcsize)
{
  unsigned long retcode = 0;
  if((srcsize > 0) && (src != NULL) && (dst0 != NULL))
  {
    unsigned char *dst = (unsigned char*)dst0;
        
    unsigned char code[4];
    unsigned long idx = 0;
    
    while(srcsize--)
    {
      unsigned char byte = *src++;
      if((byte != '\n') && (byte != '=')) // werden ignoriert
      {
        if(byte >= sizeof(TabBase64Decode))
        {
          return 0; // something wrong with sourcedata
        }
        
        unsigned char tmp = TabBase64Decode[byte];
        
        if(tmp == 0xFF)
        {
          return 0; // something wrong with sourcedata
        }
        
        code[idx] = tmp;
        idx = (idx + 1) & 0x03;
      
        if(idx == 0)
        {
          if(dstsize < 3)
          {
            return 0;
          }
          dst[0] = (code[0] << 2) | (code[1] >> 4);
          dst[1] = (code[1] << 4) | (code[2] >> 2);
          dst[2] = (code[2] << 6) | (code[3]);
          dst += 3;
          retcode += 3;
          dstsize -= 3;
        }
      }
    }
    
    if(idx > 1)
    {
      if(dstsize < 1)
      {
        return 0;
      }
      dst[0] = (code[0] << 2) | (code[1] >> 4);
      retcode++;
      dstsize--;

      if(idx > 2)
      {
        if(dstsize < 1)
        {
          return 0;
        }
        dst[1] = (code[1] << 4) | (code[2] >> 2);
        retcode++;
        dstsize--;
      }
    }
  }
  
  return retcode;
}


// ************************************************************************************************
// ************************************************************************************************
// binary en/decode
// ************************************************************************************************
// ************************************************************************************************

void sigclib_intern_shuffle(unsigned char *pa, unsigned long size, unsigned long key);
void sigclib_intern_code(unsigned char *p, unsigned long size, unsigned long key);

void sigclib_decode_bin(void *p0, unsigned long bytesize, unsigned long key)
{
  if((p0 != NULL) && (bytesize > 0))
  {
    sigclib_intern_shuffle((unsigned char*)p0, bytesize, key);
    sigclib_intern_code((unsigned char*)p0, bytesize, key);
  }
}

void sigclib_encode_bin(void *p0, unsigned long bytesize, unsigned long key)
{
  if((p0 != NULL) && (bytesize > 0))
  {
    sigclib_intern_code((unsigned char*)p0, bytesize, key);
    sigclib_intern_shuffle((unsigned char*)p0, bytesize, key);
  }
}


// ************************************************************************************************
// ************************************************************************************************
// text en/decode
// ************************************************************************************************
// ************************************************************************************************

void sigclib_intern_decode_txt(unsigned char *p, unsigned long bytesize, unsigned long key);
void sigclib_intern_encode_txt(unsigned char *p, unsigned long bytesize, unsigned long key);

void sigclib_decode_txt(void *p0, unsigned long bytesize, unsigned long key)
{
  if((p0 != NULL) && (bytesize > 0))
  {
    sigclib_intern_decode_txt((unsigned char*)p0, bytesize, key);
  }
}

void sigclib_encode_txt(void *p0, unsigned long bytesize, unsigned long key)
{
  if((p0 != NULL) && (bytesize > 0))
  {
    sigclib_intern_encode_txt((unsigned char*)p0, bytesize, key);
  }
}
