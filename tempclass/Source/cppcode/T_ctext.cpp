#include "T_IPC.H"
#include "T_NUC.H"

/*
void _ComplexText::Init(void)
{
  TextPuffer.Init(); 
  init_MEM_LINEINFO(&Info); 
}

void _ComplexText::Free(void)
{
  TextPuffer.Free();
  free_MEM_LINEINFO(&Info);

  Init();
}

_BOOL _ComplexText::Copy(_ComplexText *Pc)
{
  _BOOL retcode = _FALSE;

  Free();

  if(TextPuffer.Copy(Pc->GetTextPuffer()) == _TRUE)
  {
    retcode = copy_MEM_LINEINFO(&Info, Pc->GetLineInfo());
  }

  if(retcode == _FALSE)
    Free();

  return(retcode);
}

_UWORD _ComplexText::CalculateLines(void)
{
  _UWORD i, retcode = 0;

  for(i=0; i<Info.no; i++)
  {
    if(chk_LINEINFO(&Info.ptr[i]) == _TRUE)
      retcode ++;
  }

  return(retcode);
}

_UWORD _ComplexText::GetRealNo(_UWORD no)
{
  _UWORD i;
  _UWORD retcode = 0xFFFF;

  if(no < Info.no)
  {
    for(i=0; i<Info.no; i++)
    {
      if(chk_LINEINFO(&Info.ptr[i]) == _TRUE)
      {
        if(no == 0)
        {
          retcode = i;
          break;
        }
        no --;
      }
    }
  }

  return(retcode);
}

_BOOL _ComplexText::GetLine(_LINEINFO **pl, _CHAR **pt, _UWORD no)
{
  _UWORD i;
  _BOOL  retcode = _FALSE;

  i = GetRealNo(no);
  if((i < Info.no) && (i < TextPuffer.GetNo()))
  {
    *pl = &Info.ptr[i];
//    *pt = TextPuffer.GetText(i);
    *pt = TextPuffer.GetText((*pl)->anything.no);   // bernhard wegen zuordnung von menuetext auf menuezeile
    retcode = _TRUE;
  }

//  if((no < Info.no) && (no < TextPuffer.GetNo()))
//  {
//    for(i=0; i<Info.no; i++)
//    {
//      if(chk_LINEINFO(&Info.ptr[i]) == _TRUE)
//      {
//        if(no == 0)
//        {
//          *pl = &Info.ptr[i];
//          *pt = TextPuffer.GetText(i);
//          retcode = _TRUE;
//          break;
//        }
//        no --;
//      }
//    }
//  }
  return(retcode);
}

void _ComplexText::Out(_ROOM room, _CHAR *pt, _MEM_VARLABEL *pv, _FONT font, _ATTRIB att, _FRAMETYPE frametype, _COLOR col)
{
  _COLOR coltxt   = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(col));
  _COLOR colback  = MakeColor_Makro(GetBackColor_Makro(col), GetBackColor_Makro(col));
  _COLOR colframe = MakeColor_Makro(INVISIBLE, INVISIBLE);
  OutParameterString(room, pt, pv, font, att, frametype, coltxt, colback, colframe);
}

_BOOL _ComplexText::Read(_File* File, _UWORD language)
{
  _BOOL retcode = _FALSE; 

  Free(); 

  if(File->GetError() == _FALSE)
  {
   #if COMPILER_VERSION < 22
    if(TextPuffer.ReadLanguage(File, language) == _TRUE)
   #endif
    {
      retcode = File->Read_MEM_LINEINFO(&Info); 
    }
  }

  if(retcode == _FALSE)
    Free(); 

  return(retcode); 
} 

_TextPuffer* _ComplexText::GetTextPuffer(void)                
{ 
  return(&TextPuffer); 
}
_MEM_LINEINFO* _ComplexText::GetLineInfo(void)                      
{ 
  return(&Info); 
}
_UWORD _ComplexText::GetNo(void)                                  
{ 
  return(Info.no);
}   
*/
