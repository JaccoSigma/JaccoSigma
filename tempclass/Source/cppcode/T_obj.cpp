#include "T_IPC.H"
#include "T_NUC.H"

void _ObjectList::Init(void)
{
  LseLabel.Init();
  Ptr = NULL;
  No  = 0;
}

void _ObjectList::Free(void)
{
  LseLabel.Free();
  while(No > 0)
  {
    No --;
    Ptr[No].Free();
  }

  Memory2((void**)&Ptr, 0);
  Init();
}

_BOOL _ObjectList::Load(_ASCII *dpne)
{
  _File File;
  _BOOL retcode = _FALSE;

  File.Open(LOAD, dpne, 0, _TRUE); // sa27114
  if(File.ReadHeader() == _TRUE)
  {
    retcode = Read(&File);
  }
  File.Close();

  return(retcode);
}

_BOOL _ObjectList::Read(_File *File)
{
  _UWORD   tmp, i;
  _UDWORD  size;

  Init();

  if(File->GetError() == _FALSE)
  {
    if(File->Read(&tmp, sizeof(_UWORD)) == _TRUE)
    {
      size = tmp * sizeof(_SubPicture);
      if(File->FileMemory((void**)&Ptr, size) == _TRUE)
      {
       #ifdef ZOOM_KM
        DoZoomKM += 1;
       #endif
        No = tmp;
        for(i=0; i<No; i++)
        {
          Ptr[i].Init();
          if(Ptr[i].Read(File) == _TRUE)
          {
           #if COMPILER_VERSION < 50
            _ROOM *pr = &Ptr[i].GetPICTURE()->mask.room;
            if(pr->xy2.x > pr->xy1.x)
              pr->xy2.x -= 1; // korrektur wegen lse
            if(pr->xy2.y > pr->xy1.y)
              pr->xy2.y -= 1; // korrektur wegen lse
//            if(pr->xy2.y == pr->xy1.y)
//              pr->xy2.y += 1;
//            if(pr->xy2.x == pr->xy1.x)
//              pr->xy2.x += 1;
           #endif
          }
          else
          {
            Ptr[i].Free();
          }
        }
       #ifdef ZOOM_KM
        DoZoomKM -= 1;
       #endif
      }
     #if COMPILER_VERSION >= 33
      LseLabel.Read(File, _FALSE);
     #endif
    }
  }


  return((File->GetError() == _TRUE)? _FALSE : _TRUE);
}

_SubPicture* _ObjectList::Get(_UWORD n)              
{ 
  return((n < No)? &Ptr[n]:NULL); 
}

_UWORD _ObjectList::GetNo(void)
{
  return(No);
}

_UDWORD _ObjectList::Add(_SubPicture *pic, _ASCII *label)
{
  _UDWORD retcode = DEFSCOPE;
  
  if(Memory2((void**)&Ptr, (No + 1) * sizeof(_SubPicture)) != 0)  
  {
    if(LseLabel.Add(label) == _TRUE)
    {
      Ptr[No] = pic[0];
      retcode = No;
      No ++;
    }
  }
  return retcode;
}

extern "C" _UDWORD ObjectList_Add(_ASCII *label, _MY_PICTURE *mypic)
{
  _UDWORD retcode = DEFSCOPE;

  if(mypic->No == 1)
  {
    if(ObjectList.LseLabel.Seek(&retcode, label) == _FALSE)
    {
      mypic->Ptr->MakeSize();
      retcode = ObjectList.Add(mypic->Ptr, label);
      if(retcode != DEFSCOPE)
      {
        mypic->Ptr[0].Init(); // ansonsten wird ein anschliessendes free dieses objekt freigeben
      }
    }
  }
  
  return retcode;
}

extern "C" _PICTURE* ObjectList_Get_PICTURE(_UWORD no)
{
  _PICTURE *retcode = NULL;
  _SubPicture *psp;


  psp = ObjectList.Get(no);
  if(psp != NULL)
    retcode = psp->GetPICTURE();

  return(retcode);
}

extern "C" _SubPicture* ObjectList_Get_SubPicture(_UWORD no)
{
  return ObjectList.Get(no);
}
