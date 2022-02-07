#include "T_IPC.H"
#include "T_NUC.H"

void _ImageList::Init(void)
{
  LseLabel.Init();
  init_MEM_BMP(&Data);
}

void _ImageList::Free(void)
{
  LseLabel.Free();
  free_MEM_BMP(&Data);
  Init();
}

_BOOL _ImageList::Load(_ASCII *dpne)
{
  _File File;
  _BOOL retcode = _FALSE;

  File.Open(LOAD, dpne);
  if(File.ReadHeader() == _TRUE)
  {
    retcode = Read(&File, dpne);
  }
  File.Close();

  return(retcode);
}

_BOOL _ImageList::Read(_File *File, _ASCII *dpne)
{
  _UDWORD      size;
  _UWORD       pos, tmp, i;
  _BOOL        singleimage;
  _AsciiPuffer Label;
  _ASCII       *ne;
  _ASCII       dp[_MAX_PATHLENGTH];

  Init();

  Label.Init();

  // COMPILER_VERSION 18
  singleimage = (_BOOL)File->Read_ENUM();

  if(File->GetError() == _FALSE)
  {
    if(singleimage == _TRUE)
    {
      // single files
      if(Label.Read(File, _FALSE) == _TRUE)
      {
        pos  = offset_dpne(dpne, 1);
        MemCpy(dp, dpne, pos);
        dp[pos] = 0;
        StrCat(dp, sizeof(_ASCII), "IMAGE\\", sizeof(_ASCII));
        pos  = StrLen(dp, sizeof(_ASCII));
        ne   = Label.GetPtr();
        tmp  = Label.GetNo();
        size = tmp * sizeof(_BMP);
        if(File->FileMemory((void**)&Data.ptr, size) == _TRUE)
        {
          Data.no = tmp;
          for(i=0; i<Data.no; i++)
          {
            dp[pos] = 0;
            StrCat(dp, sizeof(_ASCII), ne, sizeof(_ASCII));
            init_BMP_Makro(&Data.ptr[i]);
            load_gfx(&Data.ptr[i], dp, NULL, NULL);
            ne += StrLen(ne, sizeof(_ASCII)) + 1;
          }
        }
      }
    }
    else
    {
      // all images in one file
      if(File->Read(&tmp, sizeof(_UWORD)) == _TRUE)
      {
        size = tmp * sizeof(_BMP);
        if(File->FileMemory((void**)&Data.ptr, size) == _TRUE)
        {
          Data.no = tmp;
          for(i=0; i<Data.no; i++)
          {
            File->Read_BMP(&Data.ptr[i]);
          }
        }
      }
    }

   #if COMPILER_VERSION >= 33
    LseLabel.Read(File, _FALSE);
   #endif
  }
  Label.Free();

  return((File->GetError() == _TRUE)? _FALSE : _TRUE);
}

_BMP* _ImageList::Get(_UWORD n)
{
  return((n < Data.no)? &Data.ptr[n]:NULL);
}

_UWORD _ImageList::GetNo(void)
{
  return(Data.no);
}

_BOOL _ImageList::Add(_BMP *p, _ASCII *name)
{
  _BOOL   retcode = _FALSE;
  _UDWORD size;

  if(p != NULL)
  {
    size = (Data.no + 1) * sizeof(_BMP);
    if(Memory2((void**)&Data.ptr, size) != 0)
    {
      Data.ptr[Data.no] = *p;
      if(LseLabel.Add(name) == _TRUE)
      {
        Data.no += 1;
        init_BMP_Makro(p); // daten neu initialisieren, da jetzt andere location
        retcode = _TRUE;
      }
    }
  }
  return(retcode);
}

#ifdef UC_IPC

_UDWORD memsize_BMP(_BMP *p)
{
  return(p->datano * 3);
}

_UDWORD memsize_MEM_BMP(_MEM_BMP *p)
{
  _UDWORD i, retcode = p->no * sizeof(_BMP);
  _BMP    *ptr;

  for(i=0, ptr=p->ptr; i<p->no; i++, ptr++)
    retcode += memsize_BMP(ptr);

  return(retcode);
}

_UDWORD _ImageList::MemSize(_MEMSIZE *p)
{
  p->data  = memsize_MEM_BMP(&Data);
  p->label = LseLabel.MemSize();

  return(p->data+p->label);
}
#endif

/*
_UDWORD q, no = Label.GetNo();
Debug(no, 1);
for(q=0; q<no; q++)
  Debug(Label.GetText(q), 1, q+2);
*/


_BMP* ImageList_Get(_UWORD n)
{
  return(ImageList.Get(n));
}

_UDWORD ImageList_Add_BMO(_ASCII *label, void *bmpdata)
{
  _BMP   bmp, dst;
  _UBYTE *ph = (_UBYTE*)bmpdata; 
  _UDWORD ino = get_no_by_label(label, ICONLST, 0);
  
  if(ino < ImageList.GetNo())
    return ino;
  
  init_BMP(&bmp);
  init_BMP(&dst);

  bmp.datano      = *(_UDWORD*)&ph[2];
  bmp.width       = *(_UWORD*)&ph[6];
  bmp.height      = *(_UWORD*)&ph[8];
  bmp.transparent = *(_UWORD*)&ph[10];   // 0x1F,0xF8 (treat pink transpatent)
  bmp.state       = *(_UWORD*)&ph[12];   // 0x03,0x00 (keep attention about transparency)
  bmp.ptr         = (_BMPDAT*)&ph[14];

  if(copy_BMP(&dst, &bmp) == _TRUE)
  {
    if(ImageList.Add(&dst, label) == _TRUE)
    {
      return get_no_by_label(label, ICONLST, 0);
    }
    
    free_BMP(&dst);
  }
  
  return DEFSCOPE;
}


_UDWORD ImageList_Add(_ASCII *dpne, _ASCII *label, _BOOL invertcolor, _UWORD cred, _UWORD cgreen, _UWORD cblue)
{
  _BMP   bmp;
  _UWORD err = 0;
  
  if(label != NULL)
  {
    if(get_no_by_label(label, ICONLST, 0) == DEFSCOPE)
    {
      init_BMP(&bmp);
      if(load_gfx(&bmp, dpne, &err, NULL) == _TRUE)
      {
        if((cred != 0xFFFF) || (cgreen != 0xFFFF) || (cblue != 0xFFFF))
          transparent_BMP(&bmp, (_UBYTE)cred, (_UBYTE)cgreen, (_UBYTE)cblue);
      
        if(invertcolor == _TRUE)
          invert_BMP(&bmp);
      
        if(ImageList.Add(&bmp, label) == _TRUE)
        {
          return get_no_by_label(label, ICONLST, 0);
        }
      }
      free_BMP(&bmp);
    }
  }
  
  return DEFSCOPE;
}



