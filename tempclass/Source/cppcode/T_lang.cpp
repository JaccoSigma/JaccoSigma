#include "T_IPC.H"
#include "T_NUC.H"

void _Language::Init(void)
{
  MemSet(Ids, 0, sizeof(Ids));
  NoIds = 0;
  init_LANGUAGE_INFO(&LanguageInfo);
}

void _Language::Free(void)
{
  free_LANGUAGE_INFO(&LanguageInfo);
  Init();
}

_BOOL _Language::Read_LANGUAGE_INFO(_File *File)
{
  // information steht im projektfile und m黶ste ab compilerversion >= 36 nicht
  // mehr ausgelesen werden. information befindet sich in einzelsprachfiles ebenso
  // und wird von dort verwendet.


  File->Read_ASCII_BUFFER(&LanguageInfo.winname);

 #if COMPILER_VERSION >= 34
  File->Read_TEXT_BUFFER(&LanguageInfo.name, NULL);
  File->Read_MEM_NO(&LanguageInfo.winid);
  File->Read_MEM_NO(&LanguageInfo.flag);

  #if COMPILER_VERSION < 36
   LanguageInfo.no = LanguageInfo.winid.no;
  #endif
 #else
  LanguageInfo.no = LanguageInfo.winname.no;
 #endif

  return((File->GetError() == _TRUE) ? _FALSE : _TRUE);
}

_BOOL _Language::ReadLanguageInfoSub(_ASCII *dpne, _LANGUAGE_INFO *p, _MEM_NO *pfd)
{
  // [ ..... header
  // [4] ... absoluter offset auf sprachinformation
  // [2] ... anzahl textlisten
  // -[ .... textlisten
  // sprachinformation
  // [1] ... character size 1 oder 2
  // [4] ... anzahl zeichen windows sprachname
  // [ ..... windows sprachname mit 0 abgeschlossen (immer in _ASCII format)
  // [4] ... windows id
  // [4] ... anzahl zeichen alias sprachname
  // [ ..... alias sprachname mit 0 abgeschlossen
  // [1] ... 0 kein icon vorhanden
  // -[ .... ggf. icon (_BMP)
  // [ ..... fontschema-information (_MEM_NO)

  _BOOL   retcode = _FALSE;
  _File   File;
  _DWORD  offset;
  _UBYTE  chrsize, icon;
  _ASCII  name[200];
  _ASCII  winname[200];
  _UDWORD len, winid;
  _BMP    bmp;
  _ASCII  flagname[10];
  _UDWORD flag = 0xFFFFFFFF;

  File.Open(LOAD, dpne);

  if(File.ReadHeader() == _TRUE)
  {
    if(File.Read(&offset, sizeof(_UDWORD)) == _TRUE)
    {
      if(File.Seek(offset, SEEK_SET) == _TRUE)
      {
        File.Read(&chrsize, sizeof(_UBYTE)); // zeichengr鰏se, gilt nur f黵 aliasname
        File.Read(&len, sizeof(_UDWORD)); // anzahl zeichen windowsname
        winname[0] = 0;
        File.Read(winname, len); // windowsname, immer _ASCII-format
        File.Read(&winid, sizeof(_UDWORD)); // windows id
        name[0] = 0;
        File.Read(&len, sizeof(_UDWORD)); // anzahl zeichen alias name
        File.Read(name, len*chrsize); // aliasname
        File.Read(&icon, sizeof(_UBYTE));
        if(icon != 0)
        {
          init_BMP_Makro(&bmp);
          if(File.Read_BMP(&bmp) == _TRUE)
          {
            flagname[0] = '@';
            flagname[1] = '_';
            DToA(&flagname[2], winid, 0x800, sizeof(_ASCII));
            if(ImageList.Add(&bmp, flagname) == _TRUE)
              flag = (ImageList.GetNo()-1);
          }
        }

        File.Read_MEM_NO(pfd); // fontdata auslesen

        if(File.GetError() == _FALSE)
        {
          if(add_MEM_NO(&p->winid, winid) == _TRUE)
          {
            if(add_MEM_NO(&p->flag, flag) == _TRUE)
            {
              if(add_TEXT_BUFFER(&p->name, name, chrsize) != DEFSCOPE)
              {
                retcode = add_ASCII_BUFFER(&p->winname, winname);
              }
            }
          }
        }
      }
    }
  }

  File.Close();
  return(retcode);
}

_BOOL _Language::MakeLanguageInfo(void)
{
  _BOOL   retcode = _TRUE;
  _UWORD  i, maxno = 0;
  _ASCII  dpne[_MAX_PATHLENGTH];
  _MEM_NO fontschemadata;

  free_LANGUAGE_INFO(&LanguageInfo);

  FontScheme.Prepaire(100, NoIds);  // anzahl von 50 auf 100 erh鰄t: wunsch applikation (friher)

  for(i=0; i<NoIds; i++)
  {
    MakeDPNE(dpne, i, _FALSE);
    init_MEM_NO(&fontschemadata);
    if(ReadLanguageInfoSub(dpne, &LanguageInfo, &fontschemadata) == _TRUE)
    {
//_UWORD x;
//for(x=0; x<fontschemadata.no; x++)
//  Debug(fontschemadata.ptr[x], x+1);
//Debug("FINITO", 1, x+1);

      if(fontschemadata.no > maxno)
        maxno = fontschemadata.no;
      FontScheme.Insert_MEM_NO(&fontschemadata, SOP_EQUAL, LanguageInfo.winid.no - 1);
    }
    free_MEM_NO(&fontschemadata);
  }

  FontScheme.Finish(maxno, LanguageInfo.winid.no);

  LanguageInfo.no = LanguageInfo.winid.no;

  return(retcode);
}

_BOOL _Language::SetTextRenderEngine(_UWORD index)
{
  if(LanguageInfo.winid.no > 0)
  {
    if(index < LanguageInfo.winid.no)
    {
      ::SetTextRenderEngine(LanguageInfo.winid.ptr[index]);
      return(_TRUE);
    }

    SetTextRenderEngine(0);
  }
  return(_FALSE);
}

_UDWORD _Language::GetIDByIndex(_UDWORD idx)
{
  if(idx < LanguageInfo.winid.no)
  {
    return LanguageInfo.winid.ptr[idx];
  }
  return GetActualID();
}

_UWORD _Language::GetIndexByID(_UDWORD id)
{
  _UWORD i;

  for(i=0; i<LanguageInfo.winid.no; i++)
    if(LanguageInfo.winid.ptr[i] == id)
      return(i);
  return(0xFFFF);
}

_UDWORD _Language::GetActualID(void)
{
  _UWORD l = get_actual_language();
  if(l < LanguageInfo.winid.no)
    return LanguageInfo.winid.ptr[l];
  return 0;
}

_LOCAL _Language::GetActualLocal(void)
{
  _UDWORD id = GetActualID();
  return (_LOCAL)(id & 0xFF);
}

_BOOL _Language::LoadDirectory(_ASCII *dpne)
{
  _File   File;
  _BOOL   retcode = _FALSE;
  _ASCII  tmp[100], *p;
  _DWORD  z;

  Free();

  if(File.Open(LOAD, dpne) == _TRUE)
  {
    while(File.Read_CRLF(tmp, 100) == _TRUE)
    {
      DelSpace(tmp, sizeof(_ASCII));        // delete all space
      DelComment(tmp, sizeof(_ASCII));      // delete comment
      p = tmp;
      z = AsciiToNo(&p);
      if(z > 0)
      {
        Ids[NoIds] = z;
        NoIds ++;
      }
    }

    retcode = _TRUE;
  }

  File.Close();

  return(retcode);
}

_BOOL _Language::MakeDPNE(_ASCII *dpne, _UWORD language_no, _BOOL do_real)
{
  _BOOL   retcode = _FALSE;
  _UWORD  max;
  _UDWORD id = 0;

  if(do_real == _TRUE)
  {
    max = LanguageInfo.no;
    if(language_no < max)
      id = LanguageInfo.winid.ptr[language_no];
  }
  else
  {
    max = NoIds;
    if(language_no < max)
      id = Ids[language_no];
  }

  if(language_no < max)
  {
    StrCpy(dpne, sizeof(_ASCII), Project.GetPrjPath(), sizeof(_ASCII));
    DToA(&dpne[StrLen(dpne, sizeof(_ASCII))], id, 0x800, sizeof(_ASCII));
    StrCat(dpne, sizeof(_ASCII), ".mpc", sizeof(_ASCII));
    retcode = _TRUE;
  }

  return(retcode);
}

_BOOL _Language::LoadLanguage(_UWORD language_no)
{
  _BOOL  retcode = _FALSE;
  _ASCII dpne[_MAX_PATHLENGTH];

  if(language_no >= GetNo())
  {
    // bei ung黮tiger sprachnummer auf sprache 0 schalten
    language_no = 0;
    set_actual_language(language_no);
    SetLseServer(LSE_LANGUAGE, language_no);
  }

  if(MakeDPNE(dpne, language_no, _TRUE) == _TRUE)
  {
    retcode = TextList.LoadCompiler36(dpne);
  }

  return(retcode);
}

_UWORD _Language::GetNo(void)
{
  return(LanguageInfo.no);
}

_LANGUAGE_INFO* _Language::Get_LANGUAGE_INFO(void)
{
  return(&LanguageInfo);
}

_UWORD StdTextTabChinese[] = 
{
  0x786e, 0x5b9a, 0x0000, // OK
  0x53d6, 0x6d88, 0x0000, // Cancel
  0x662f, 0x0000, // Ja
  0x5426, 0x0000, // Nein
  0x5468, 0x4E03, 0x5468, 0x4E00, 0x5468, 0x4E8C, 0x5468, 0x4E09, 0x5468, 0x56DB, 0x5468, 0x4E94, 0x5468, 0x516D, 0x5468, 0x4E03, 0x0000, // SoMoDiMiDoFrSaSo
};

_UWORD StdTextTabJapanese[] = 
{
      79,     75, 0x0000, // OK
  0x30ad, 0x30e3, 0x30f3, 0x30bb, 0x30eb, 0x0000, // Cancel
  0x306f, 0x3044, 0x0000, // Ja
  0x3044, 0x3044, 0x3048, 0x0000, // Nein
  0x65e5, 0x0000, 0x6708, 0x0000, 0x706b, 0x0000, 0x6c34, 0x0000, 0x6728, 0x0000, 0x91d1, 0x0000, 0x571f, 0x0000, 0x65e5, 0x0000, 0x0000, // SoMoDiMiDoFrSaSo
};


_UWORD StdTextTabRuss[] = 
{
  0x004F, 0x004B, 0x0000, // OK
  0x041E, 0x0442, 0x043C, 0x0435, 0x43D, 0x430, 0x0000, // Cancel
  0x0414, 0x0430, 0x0000, // Ja
  0x041D, 0x0435, 0x0442, 0x0000, // Nein
  0x0412, 0x0441, 0x041F, 0x043D, 0x0412, 0x0442, 0x0421, 0x0440, 0x0427, 0x0442, 0x041F, 0x0442, 0x0421, 0x0431, 0x0412, 0x0441, 0x0000, // SoMoDiMiDoFrSaSo
};

_UWORD StdTextTabUkraine[] = 
{
  0x004F, 0x004B, 0x0000, // OK
  0x0412, 0x0069, 0x0434, 0x043C, 0x069, 0x43D, 0x0430, 0x0000, // Cancel
  0x0422, 0x0430, 0x043A, 0x0000, // Ja
  0x041D, 0x0069, 0x0000, // Nein
  0x041D, 0x0434, 0x041F, 0x043D, 0x0412, 0x0442, 0x0421, 0x0440, 0x0427, 0x0442, 0x041F, 0x0442, 0x0421, 0x0431, 0x041D, 0x0434, 0x0000, // SoMoDiMiDoFrSaSo
};

void _Language::GetStandardText(void **ptxt, _UWORD *pchrsize, _STDTEXTID txtid)
{
  *pchrsize = sizeof(_ASCII);
  *ptxt = NULL; 

  switch(Language.GetActualLocal())
  {
    case LOCAL_DUTCH :
      switch(txtid)
      {
        case TxtID_OK       : *ptxt = (void*)"OK";               break;
        case TxtID_CANCLE   : *ptxt = (void*)"Annuleren";        break;
        case TxtID_YES      : *ptxt = (void*)"Ja";               break;
        case TxtID_NO       : *ptxt = (void*)"Nee";              break;
        case TxtID_WEEKDAYS : *ptxt = (void*)"ZoMaDiWoDoVrZaZo"; break;
      }
      break;
      
    case LOCAL_FRENCH :
      switch(txtid)
      {
        case TxtID_OK       : *ptxt = (void*)"Confirmer";        break;
        case TxtID_CANCLE   : *ptxt = (void*)"Annuler";          break;
        case TxtID_YES      : *ptxt = (void*)"Oui";              break;
        case TxtID_NO       : *ptxt = (void*)"Ancun";            break;
        case TxtID_WEEKDAYS : *ptxt = (void*)"DiLuMaMeJeVeSaDi"; break;
      }
      break;

    case LOCAL_ITALIAN :
      switch(txtid)
      {
        case TxtID_OK       : *ptxt = (void*)"OK";               break;
        case TxtID_CANCLE   : *ptxt = (void*)"Anulla";           break;
        case TxtID_YES      : *ptxt = (void*)"Si";               break;
        case TxtID_NO       : *ptxt = (void*)"No";               break;
        case TxtID_WEEKDAYS : *ptxt = (void*)"DoLuMaMeGiVeSaDo"; break;
      }
      break;
      
    case LOCAL_GERMAN :
      switch(txtid)
      {
        case TxtID_OK       : *ptxt = (void*)"OK";               break;
        case TxtID_CANCLE   : *ptxt = (void*)"Abbruch";          break;
        case TxtID_YES      : *ptxt = (void*)"Ja";               break;
        case TxtID_NO       : *ptxt = (void*)"Nein";             break;
        case TxtID_WEEKDAYS : *ptxt = (void*)"SoMoDiMiDoFrSaSo"; break; 
      }
      break;
      
    case LOCAL_JAPANESE :  
      switch(txtid)
      {
        case TxtID_OK       : *ptxt = (void*)&StdTextTabJapanese[0];  break;
        case TxtID_CANCLE   : *ptxt = (void*)&StdTextTabJapanese[3];  break;
        case TxtID_YES      : *ptxt = (void*)&StdTextTabJapanese[9];  break;
        case TxtID_NO       : *ptxt = (void*)&StdTextTabJapanese[12]; break;
        case TxtID_WEEKDAYS : *ptxt = (void*)&StdTextTabJapanese[16]; break;
      }
      *pchrsize = sizeof(_UNI);
      break;
      
    case LOCAL_CHINESE : 
      switch(txtid)
      {
        case TxtID_OK       : *ptxt = (void*)&StdTextTabChinese[0];  break;
        case TxtID_CANCLE   : *ptxt = (void*)&StdTextTabChinese[3];  break;
        case TxtID_YES      : *ptxt = (void*)&StdTextTabChinese[6];  break;
        case TxtID_NO       : *ptxt = (void*)&StdTextTabChinese[8];  break;
        case TxtID_WEEKDAYS : *ptxt = (void*)&StdTextTabChinese[10]; break;
      }
      *pchrsize = sizeof(_UNI);
      break;
      
    case LOCAL_UKRAINIAN :
      switch(txtid)
      {
        case TxtID_OK       : *ptxt = (void*)&StdTextTabUkraine[0];  break;
        case TxtID_CANCLE   : *ptxt = (void*)&StdTextTabUkraine[3];  break;
        case TxtID_YES      : *ptxt = (void*)&StdTextTabUkraine[11]; break;
        case TxtID_NO       : *ptxt = (void*)&StdTextTabUkraine[15]; break;
        case TxtID_WEEKDAYS : *ptxt = (void*)&StdTextTabUkraine[18]; break;
      }
      *pchrsize = sizeof(_UNI);
      break;

    case LOCAL_RUSSIAN :
      switch(txtid)
      {
        case TxtID_OK       : *ptxt = (void*)&StdTextTabRuss[0];  break;
        case TxtID_CANCLE   : *ptxt = (void*)&StdTextTabRuss[3];  break;
        case TxtID_YES      : *ptxt = (void*)&StdTextTabRuss[10]; break;
        case TxtID_NO       : *ptxt = (void*)&StdTextTabRuss[13]; break;
        case TxtID_WEEKDAYS : *ptxt = (void*)&StdTextTabRuss[17]; break;
      }
      *pchrsize = sizeof(_UNI);
      break;
      
    case LOCAL_PORTUGESE :
      switch(txtid)
      {
        case TxtID_OK       : *ptxt = (void*)"OK";               break;
        case TxtID_CANCLE   : *ptxt = (void*)"Cancelar";         break;
        case TxtID_YES      : *ptxt = (void*)"Sim";              break;
        case TxtID_NO       : *ptxt = (void*)"N鉶";              break;
        case TxtID_WEEKDAYS : *ptxt = (void*)"Do2񡀒񡧬猄aDo"; break;
      }
      break;
    
    case LOCAL_SPANISH : 
      switch(txtid)
      {
        case TxtID_OK       : *ptxt = (void*)"Bien";             break;
        case TxtID_CANCLE   : *ptxt = (void*)"Cancelar";         break;
        case TxtID_YES      : *ptxt = (void*)"Si";               break;
        case TxtID_NO       : *ptxt = (void*)"No";               break;
        case TxtID_WEEKDAYS : *ptxt = (void*)"DoLuMaMiJuViSaDo"; break;
      }
      break;
      
    default :  //  case LOCAL_ENGLISH :  
      switch(txtid)
      {
        case TxtID_OK       : *ptxt = (void*)"OK";               break;
        case TxtID_CANCLE   : *ptxt = (void*)"Cancel";           break;
        case TxtID_YES      : *ptxt = (void*)"Yes";              break;
        case TxtID_NO       : *ptxt = (void*)"No";               break;
        case TxtID_WEEKDAYS : *ptxt = (void*)"SuMoTuWeThFrSaSu"; break;
      }
      break;
  }
}


#ifdef UC_IPC
_BOOL _Language::SeperateTextlist(_CHAR **dst, _UDWORD *pno, _UWORD language_no, _UWORD txtlst)
{
  _BOOL        retcode = _FALSE;
  _ASCII       dpne[_MAX_PATHLENGTH];
  _TextList    Tmp;
  _TextPuffer  *Htp;

  PushMemColor(MEMORY_PRG);

  Tmp.Init();

  if(MakeDPNE(dpne, language_no, _TRUE) == _TRUE)
  {
    if(Tmp.LoadCompiler36(dpne) == _TRUE)
    {
      Htp = Tmp.GetTextPuffer(txtlst - TXTLIST_VTX);
      if(Htp != NULL)
      {
        *pno = Htp->GetNo();
        *dst = Htp->GetText(0);
        Htp->Init();
        retcode = _TRUE;
      }
    }
  }

  Tmp.Free(_TRUE);

  PopMemColor();

  return(retcode);
}

extern "C" _BOOL seperate_textlist(_CHAR **dst, _UDWORD *pno, _UWORD language_no, _UWORD txtlst)
{
  return(Language.SeperateTextlist(dst, pno, language_no, txtlst));
}
#endif


extern "C" void Language_GetStandardText(void **ptxt, _UWORD *pchrsize, _STDTEXTID txtid)
{
  Language.GetStandardText(ptxt, pchrsize, txtid);
}

extern "C" _LOCAL Language_GetActualLocal(void)
{
  return Language.GetActualLocal();
}
