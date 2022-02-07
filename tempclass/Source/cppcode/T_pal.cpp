#include "T_IPC.H"
#include "T_NUC.H"

_UBYTE DefaultRgbTab[256*3] = {
   0,   0,   0,   0,   0, 168,   0, 168,   0,
   0, 168, 168, 168,   0,   0, 168,   0, 168,
 168,  84,   0, 168, 168, 168,  84,  84,  84,
  84,  84, 255,  84, 255,  84,  84, 255, 255,
 255,  84,  84, 255,  84, 255, 255, 255,  84,
 255, 255, 255,   0,   0,   0,  20,  20,  20,
  32,  32,  32,  44,  44,  44,  56,  56,  56,
  68,  68,  68,  80,  80,  80,  96,  96,  96,
 112, 112, 112, 128, 128, 128, 144, 144, 144,
 160, 160, 160, 180, 180, 180, 200, 200, 200,
 224, 224, 224, 255, 255, 255,   0,   0, 255,
  64,   0, 255, 124,   0, 255, 188,   0, 255,
 255,   0, 255, 255,   0, 188, 255,   0, 124,
 255,   0,  64, 255,   0,   0, 255,  64,   0,
 255, 124,   0, 255, 188,   0, 255, 255,   0,
 188, 255,   0, 124, 255,   0,  64, 255,   0,
   0, 255,   0,   0, 255,  64,   0, 255, 124,
   0, 255, 188,   0, 255, 255,   0, 188, 255,
   0, 124, 255,   0,  64, 255, 124, 124, 255,
 156, 124, 255, 188, 124, 255, 220, 124, 255,
 255, 124, 255, 255, 124, 220, 255, 124, 188,
 255, 124, 156, 255, 124, 124, 255, 156, 124,
 255, 188, 124, 255, 220, 124, 255, 255, 124,
 220, 255, 124, 188, 255, 124, 156, 255, 124,
 124, 255, 124, 124, 255, 156, 124, 255, 188,
 124, 255, 220, 124, 255, 255, 124, 220, 255,
 124, 188, 255, 124, 156, 255, 180, 180, 255,
 196, 180, 255, 216, 180, 255, 232, 180, 255,
 255, 180, 255, 255, 180, 232, 255, 180, 216,
 255, 180, 196, 255, 180, 180, 255, 196, 180,
 255, 216, 180, 255, 232, 180, 255, 255, 180,
 232, 255, 180, 216, 255, 180, 196, 255, 180,
 180, 255, 180, 180, 255, 196, 180, 255, 216,
 180, 255, 232, 180, 255, 255, 180, 232, 255,
 180, 216, 255, 180, 196, 255,   0,   0, 112,
  28,   0, 112,  56,   0, 112,  84,   0, 112,
 112,   0, 112, 112,   0,  84, 112,   0,  56,
 112,   0,  28, 112,   0,   0, 112,  28,   0,
 112,  56,   0, 112,  84,   0, 112, 112,   0,
  84, 112,   0,  56, 112,   0,  28, 112,   0,
   0, 112,   0,   0, 112,  28,   0, 112,  56,
   0, 112,  84,   0, 112, 112,   0,  84, 112,
   0,  56, 112,   0,  28, 112,  56,  56, 112,
  68,  56, 112,  84,  56, 112,  96,  56, 112,
 112,  56, 112, 112,  56,  96, 112,  56,  84,
 112,  56,  68, 112,  56,  56, 112,  68,  56,
 112,  84,  56, 112,  96,  56, 112, 112,  56,
  96, 112,  56,  84, 112,  56,  68, 112,  56,
  56, 112,  56,  56, 112,  68,  56, 112,  84,
  56, 112,  96,  56, 112, 112,  56,  96, 112,
  56,  84, 112,  56,  68, 112,  80,  80, 112,
  88,  80, 112,  96,  80, 112, 104,  80, 112,
 112,  80, 112, 112,  80, 104, 112,  80,  96,
 112,  80,  88, 112,  80,  80, 112,  88,  80,
 112,  96,  80, 112, 104,  80, 112, 112,  80,
 104, 112,  80,  96, 112,  80,  88, 112,  80,
  80, 112,  80,  80, 112,  88,  80, 112,  96,
  80, 112, 104,  80, 112, 112,  80, 104, 112,
  80,  96, 112,  80,  88, 112,   0,   0,  64,
  16,   0,  64,  32,   0,  64,  48,   0,  64,
  64,   0,  64,  64,   0,  48,  64,   0,  32,
  64,   0,  16,  64,   0,   0,  64,  16,   0,
  64,  32,   0,  64,  48,   0,  64,  64,   0,
  48,  64,   0,  32,  64,   0,  16,  64,   0,
   0,  64,   0,   0,  64,  16,   0,  64,  32,
   0,  64,  48,   0,  64,  64,   0,  48,  64,
   0,  32,  64,   0,  16,  64,  32,  32,  64,
  40,  32,  64,  48,  32,  64,  56,  32,  64,
  64,  32,  64,  64,  32,  56,  64,  32,  48,
  64,  32,  40,  64,  32,  32,  64,  40,  32,
  64,  48,  32,  64,  56,  32,  64,  64,  32,
  56,  64,  32,  48,  64,  32,  40,  64,  32,
  32,  64,  32,  32,  64,  40,  32,  64,  48,
  32,  64,  56,  32,  64,  64,  32,  56,  64,
  32,  48,  64,  32,  40,  64,  44,  44,  64,
  48,  44,  64,  52,  44,  64,  60,  44,  64,
  64,  44,  64,  64,  44,  60,  64,  44,  52,
  64,  44,  48,  64,  44,  44,  64,  48,  44,
  64,  52,  44,  64,  60,  44,  64,  64,  44,
  60,  64,  44,  52,  64,  44,  48,  64,  44,
  44,  64,  44,  44,  64,  48,  44,  64,  52,
  44,  64,  60,  44,  64,  64,  44,  60,  64,
  44,  52,  64,  44,  48,  64,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,
 255, 255, 254};

#ifdef UC_NEWCOLORPALETTE

_WORD sortcol16(_C16IX *p1, _C16IX *p2)
{
  if(p1->cold < p2->cold)
    return -1;
  if(p1->cold == p2->cold)
    return 0;
  
  return 1;
}

void _Palette::MakeOldNewColorTable(void)
{
  _UDWORD i;

  if(OldNewColAnzahl > 0)
  {
    _C16IX *ponc = &OldNewCol16[0];
    for(i=0; i<OldNewColAnzahl; i++, ponc++)
    {
      ponc->cold = MakeColor16(&RgbPtr[ponc->cold * 3]); // index in aktuelle col16 umrechnen
    }
    
    qsort(OldNewCol16, OldNewColAnzahl, sizeof(_C16IX), (_QSORTFUNCT)sortcol16);
  }
  
  _COL16 *pc16 = &NewCol16[0];
  _UBYTE *prgb = &RgbPtr[0];
  for(i=0; i<256; i++, pc16++, prgb+=3)
  {
    if(pc16->done == 0)
    {
      pc16->color = MakeColor16(prgb);
      pc16->done  = 0xFFFF;
    }
  }
}

void _Palette::SetNewColorPalette(_UBYTE *prgb, _UDWORD startindex, _UDWORD anzahl)
{
  if(anzahl > 0)
  {
    if(prgb == NULL)
      prgb = DefaultRgbTab;

    if((startindex + anzahl) <= 256)
    {
      _COL16 *pc16     = &NewCol16[startindex];
      _C16IX *ponc     = &OldNewCol16[OldNewColAnzahl];
      OldNewColAnzahl += anzahl;

      for(_UDWORD i=0; i<anzahl; i++, pc16++, ponc++, prgb+=3)
      {
        pc16->color = MakeColor16(prgb);
        pc16->done  = 1; 
        ponc->cnew  = pc16->color;
        ponc->cold  = i + startindex; // index eintragen, wird später in col16 umberechnet
      }
    }
  }
}

_UDWORD _Palette::FindIndexSub(_UDWORD start, _UDWORD anz, _UWORD col16)
{
  if(anz > 0)
  {
    _UDWORD index = start + (anz / 2);
    _UWORD  tmpcol = OldNewCol16[index].cold;

    if(col16 < tmpcol)
    {
      return (FindIndexSub(start, (anz / 2), col16));
    }
    else if (col16 > tmpcol)
    {
      return (FindIndexSub(index + 1, ((anz - 1) / 2), col16));
    }

    return index;
  }
   
  return 0xFFFFFFFF;
}

_UWORD _Palette::GetNewColor(_UWORD col16)
{
  _UDWORD index = FindIndexSub(0, OldNewColAnzahl, col16);
  if(index < OldNewColAnzahl)
    return OldNewCol16[index].cnew;
  
  return col16;
}

extern "C" void SetNewColorPalette(_UBYTE *prgb, _UDWORD startindex, _UDWORD anzahl)
{
  Palette.SetNewColorPalette(prgb, startindex, anzahl);
}
#endif


void _Palette::Init(void)
{
 #ifdef UC_NEWCOLORPALETTE
  OldNewColAnzahl = 0;
  MemSet(OldNewCol16, 0, sizeof(OldNewCol16));
  MemSet(NewCol16, 0, sizeof(NewCol16));
 #endif
  TargetColor = 65535;
  MemCpy(RgbPtr, DefaultRgbTab, 256*3);
  AskSystem();
}

void _Palette::Free(void)
{
  Init();
}

void _Palette::AskSystem(void)
{
  TargetColor = GetNoColors() + 1;
}

void _Palette::Set(void)
{
 #ifdef UC_IPC
  if(TargetColor <= 256)
  {
    _UBYTE tmp[256*3];
    _UWORD i;
    
    for(i=0; i<256*3; i++)
      tmp[i] = RgbPtr[i]>>2;
    SetColorPalette(tmp);
  }
 #endif
}

_UWORD _Palette::MakeColor16(_UBYTE *prgb)
{
  _UWORD retcode = MakeTrueColor(prgb[0], prgb[1], prgb[2]);
  if(retcode >= 0xFFFE)
  {
    if((prgb[0] == 0xFF)&&(prgb[1] == 0xFF)&&(prgb[2] == 0xFE))
      return 0xFFFE;
    return 0xFFFF;
  }
  return retcode;
}

_BOOL _Palette::Read(_UBYTE *dst, _File *File)
{
  _UDWORD tmp;
  _BOOL   retcode = _FALSE;

  if(File->Read(&tmp, sizeof(_UDWORD)) == _TRUE)
  {
    if((tmp == 256) && (dst != NULL))
      retcode = File->Read(dst, 256*3);
  }

  return(retcode);
}

_BOOL _Palette::Load(_ASCII *dpne)
{
  _File  File;
  _BOOL  retcode = _FALSE;

  File.Open(LOAD, dpne);
  if(File.ReadHeader() == _TRUE)
  {
    retcode = Read(RgbPtr, &File);
  }
  File.Close();

  if((retcode == _TRUE)&&(TargetColor <= 256))
    Set();

 #ifdef UC_NEWCOLORPALETTE
  MakeOldNewColorTable();
 #endif
 
  return(retcode);
}

_COLOR _Palette::MakeColorFromIndex(_UWORD index)
{
  if(TargetColor <= 256)
    return(index);

  if(index < 256)
  {
   #ifdef UC_NEWCOLORPALETTE
    _COL16 *pc16 = &NewCol16[index];
    if(pc16->done != 0) // sa24489
      return pc16->color;
   #endif
  
    return MakeColor16(&RgbPtr[index * 3]);
  }

  return(index);
}

void _Palette::Make_RGB(_RGB *rgb, _COLOR col)
{
  _UWORD blue, green, red;

  if((col < 256) && (TargetColor <= 256))
  {
    *rgb = *(_RGB*)&RgbPtr[col*3];
  }
  else
  {
    blue  = col & 0x1F;
    green = (col >> 5) & 0x3F;
    red   = (col >> 11) & 0x1F;

    if(blue  == 0x1F) blue  = 0xFF; else blue  = (blue * 0xFF)/0x1F;
    if(green == 0x3F) green = 0xFF; else green = (green * 0xFF)/0x3F;
    if(red   == 0x1F) red   = 0xFF; else red   = (red * 0xFF)/0x1F;

    rgb->cred   = (_UBYTE)red;
    rgb->cgreen = (_UBYTE)green;
    rgb->cblue  = (_UBYTE)blue;

//    rgb->cred   = (col&0xF800) >> 8;
//    rgb->cgreen = (col&0x07E0) >> 3;
//    rgb->cblue  = (col&0x001F) << 3;
  }
}

_BOOL _Palette::SetColor(_UWORD no, _COLOR col)
{
  _RGB   rgb;
  _BOOL  retcode = _FALSE;
  _UBYTE *ptr;

  if(no < 256)
  {
    ptr = &RgbPtr[no * 3];
    Make_RGB(&rgb, col);
    ptr[0] = rgb.cred;
    ptr[1] = rgb.cgreen;
    ptr[2] = rgb.cblue;
    Set();
    retcode = _TRUE;
  }

  return(retcode);
}

void _Palette::Get(_UBYTE *dst, _UDWORD startidx, _UDWORD anzahl)
{
  MemCpy(dst, &RgbPtr[startidx*3], anzahl*3);
}

_COLOR MakeColorFromIndex(_UWORD no)
{
  return Palette.MakeColorFromIndex(no);
}

void Make_RGB(_RGB *rgb, _COLOR col)
{
  Palette.Make_RGB(rgb, col);
}


