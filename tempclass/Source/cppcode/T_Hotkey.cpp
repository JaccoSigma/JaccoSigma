#include "T_IPC.H"
#include "T_NUC.H"


void _Hotkey::Init(void)
{

  init_MEM_KEY(&MemKey);
}

void _Hotkey::Free(void)
{

  free_MEM_KEY(&MemKey);
}

_BOOL _Hotkey::Load(_ASCII *dpne)
{
  _BOOL retcode = _FALSE;
  _File file;


  if(file.Open(LOAD, dpne) == _TRUE)
  {
    if(file.ReadHeader() == _TRUE)
      retcode = Read(&file);
  }

  file.Close();

  return(retcode);
}

_BOOL _Hotkey::Read(_File *file)
{
  _BOOL retcode = _FALSE;


  Init();

  if(file->GetError() == _FALSE)
    retcode = file->Read_MEM_KEY(&MemKey);


  if(file->GetError() == _TRUE)
  {
    Free();
    retcode = _FALSE;
  }

  return(retcode);
}

_KEY* _Hotkey::FindKey(_UWORD code)
{
  _KEY *retcode = NULL;
  _UWORD i;


  for(i=0; i<MemKey.no; i++)
  {
    if(MemKey.ptr[i].code == code)
    {
      retcode = &MemKey.ptr[i];
      break;
    }
  }

  return(retcode);

}

_UWORD _Hotkey::GetNo(void)
{

  return(MemKey.no);
}

_KEY* _Hotkey::Get(_UWORD no)
{
  _KEY *retcode = NULL;

  if(no < MemKey.no)
    retcode = &MemKey.ptr[no];

  return(retcode);
}

void _Hotkey::SetKeyMode(void)
{
  _UWORD i;
  _KEY  *ptr = MemKey.ptr;

  for(i=0; i<MemKey.no; i++, ptr++)
  {
    set_keymode(ptr->code, ptr->tip);
  }
}
