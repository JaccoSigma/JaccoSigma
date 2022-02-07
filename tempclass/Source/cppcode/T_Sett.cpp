#include "T_IPC.H"
#include "T_NUC.H"

#define SETTINGSFILENAME    "IPC.SET"
#define SETTINGSFILEID      "IPCSET"
#define SETTINGSFILEVERSION 8

void _IpcSettings::Init(void)
{
}

void _IpcSettings::Free(void)
{
}

_BOOL _IpcSettings::Save(void)
{
  _BOOL   retcode = _FALSE;
  _ASCII  dpne[_MAX_PATHLENGTH];
  _File   File;
  _UDWORD tmp;

  // [ ..... header
  // [4] ... language (version 0)
  // [4] ... timedelay screensaver (version 1)
  // [2] ... commasign
  // [2] ... americandateformat
  // [2] ... seek algorithm
  // [2] ... direction on enter
  // [2] ... cursorwrap
  // [2] ... cursorwrap
  // [2] ... externcom
  // [6] ... flipscreeninfo (version 7)
  // [7] ... americantimeformat (version 8)

  if(Project.MakeConfigDPNE(dpne, SETTINGSFILENAME, _TRUE) == _TRUE)
  {
    if(File.Open(SAVE, dpne) == _TRUE)
    {
      File.Write_HEADER(SETTINGSFILEID, SETTINGSFILEVERSION);
      tmp = get_actual_language();
      File.Write(&tmp, sizeof(_UDWORD));  // write language
      tmp = get_delay_screensave();
      File.Write(&tmp, sizeof(_UDWORD));  // write timedelay screensaver
      tmp = get_commasign();
      File.Write_ENUM(tmp);
      tmp = get_americandateformat();
      File.Write_ENUM(tmp);
      tmp = get_seek_algorithm();
      File.Write_ENUM(tmp);
      tmp = get_directiononenter();
      File.Write_ENUM(tmp);
      tmp = get_cursorwraphorizontal();
      File.Write_ENUM(tmp);
      tmp = get_cursorwrapvertical();
      File.Write_ENUM(tmp);
      tmp = get_noexterncom();
      File.Write_ENUM(tmp);
      File.Write(&FlipScreen.dx, sizeof(_UWORD));  // write flipscreen
      File.Write(&FlipScreen.yo, sizeof(_UWORD));  // write flipscreen
      File.Write(&FlipScreen.yu, sizeof(_UWORD));  // write flipscreen
      tmp = get_americantimeformat();
      File.Write_ENUM(tmp);

      retcode = (File.GetError() == _FALSE)? _TRUE : _FALSE;
      File.Close();
    }
  }

  return(retcode);
}

_BOOL _IpcSettings::Load(void)
{
  _BOOL   retcode = _FALSE;
  _File   File;
  _ASCII  dpne[_MAX_PATHLENGTH];
  _UDWORD tmp, version;

  set_actual_language(GetLseServer(LSE_LANGUAGE));
  set_delay_screensave(GetLseServer(LSE_SCREENDELAY));
  set_directiononenter((GetLseServer(LSE_DIRECTIONONENTER) == 0)? _FALSE : _TRUE);
  set_commasign((GetLseServer(LSE_COMMASIGN) == 0)? _FALSE : _TRUE);
  set_americandateformat((GetLseServer(LSE_AMERICANDATEFORMAT) == 0)? _FALSE : _TRUE);
  set_seek_algorithm((GetLseServer(LSE_SEEKALGORITHM) == 0)? _FALSE : _TRUE);
  set_cursorwrapvertical((GetLseServer(LSE_CURSORWRAPVERTICAL) == 0)? _FALSE : _TRUE);
  set_cursorwraphorizontal((GetLseServer(LSE_CURSORWRAPHORIZONTAL) == 0)? _FALSE : _TRUE);
  set_noexterncom((GetLseServer(LSE_NOEXTERNCOM) == 0)? _FALSE : _TRUE);
  set_americantimeformat((GetLseServer(LSE_AMERICANTIMEFORMAT) == 0)? _FALSE : _TRUE);

  if(Project.MakeConfigDPNE(dpne, SETTINGSFILENAME, _TRUE) == _TRUE)
  {
    if(File.Open(LOAD, dpne) == _TRUE)
    {
      if(File.Read_HEADER(&version, SETTINGSFILEID, _TRUE) == _TRUE)
      {
        if(File.Read(&tmp, sizeof(_UDWORD)) == _TRUE)
          set_actual_language(tmp);
        if(version >= 1)
        {
          if(File.Read(&tmp, sizeof(_UDWORD)) == _TRUE)
            set_delay_screensave(tmp);
        }
        if(version >= 2)
        {
          tmp = (_BOOL)File.Read_ENUM();
          set_commasign((_BOOL)tmp);

          tmp = (_BOOL)File.Read_ENUM();
          set_americandateformat((_BOOL)tmp);
        }
        if(version >= 3)
        {
          tmp = (_BOOL)File.Read_ENUM();
          set_seek_algorithm((_BOOL)tmp);
        }
        if(version >= 4)
        {
          tmp = (_BOOL)File.Read_ENUM();
          set_directiononenter((_BOOL)tmp);
        }
        if(version >= 5)
        {
          tmp = (_BOOL)File.Read_ENUM();
          set_cursorwraphorizontal((_BOOL)tmp);
          tmp = (_BOOL)File.Read_ENUM();
          set_cursorwrapvertical((_BOOL)tmp);
        }
        if(version >= 6)
        {
          tmp = (_BOOL)File.Read_ENUM();
          set_noexterncom((_BOOL)tmp);
        }
        if(version >= 7)
        {
          File.Read(&FlipScreen.dx, sizeof(_UWORD));  // write flipscreen
          File.Read(&FlipScreen.yo, sizeof(_UWORD));  // write flipscreen
          File.Read(&FlipScreen.yu, sizeof(_UWORD));  // write flipscreen
        }
        if(version >= 8)
        {
          tmp = (_BOOL)File.Read_ENUM();
          set_americantimeformat((_BOOL)tmp);
        }

        retcode = (File.GetError() == _FALSE)? _TRUE : _FALSE;
      }
      File.Close();
    }
  }

  SetLseServer(LSE_OPSYSVERSION, 0);
  SetLseServer(LSE_SCREENDELAY, get_delay_screensave());
  SetLseServer(LSE_LANGUAGE, get_actual_language());
  SetLseServer(LSE_COMMASIGN, get_commasign());
  SetLseServer(LSE_AMERICANDATEFORMAT, get_americandateformat());
  SetLseServer(LSE_SEEKALGORITHM, get_seek_algorithm());
  SetLseServer(LSE_DIRECTIONONENTER, get_directiononenter());
  SetLseServer(LSE_CURSORWRAPVERTICAL, get_cursorwrapvertical());
  SetLseServer(LSE_CURSORWRAPHORIZONTAL, get_cursorwraphorizontal());
  SetLseServer(LSE_NOEXTERNCOM, get_noexterncom());
  SetLseServer(LSE_AMERICANTIMEFORMAT, get_americantimeformat());

  System.write = _FALSE;

  return(retcode);
}

_UDWORD get_delay_screensave(void)
{
  return(get_delay_screensave_Makro()); //System.delaytime_screensave);
}

_UWORD get_actual_language(void)
{
  return(get_actual_language_Makro()); //System.act_language);
}

_BOOL get_commasign(void)
{
  return(get_commasign_Makro()); //System.commasign);
}

_BOOL get_americandateformat(void)
{
  return(get_americandateformat_Makro()); //System.americandateformat);
}

_BOOL get_americantimeformat(void)
{
  return(get_americantimeformat_Makro()); //System.americantimeformat);
}

_BOOL get_seek_algorithm(void)
{
  return(System.seek_algorithm);
}

_BOOL get_directiononenter(void)
{
  return(System.directiononenter);
}

_BOOL get_cursorwraphorizontal(void)
{
  return(System.cursorwraphorizontal);
}

_BOOL get_cursorwrapvertical(void)
{
  return(System.cursorwrapvertical);
}

_BOOL get_noexterncom(void)
{
  return(System.noexterncom);
}

_BOOL IpcSettings_Save(void)
{
  return(IpcSettings.Save());
}

void set_delay_screensave(_UDWORD tmp)
{
  if(System.delaytime_screensave != tmp)
  {
    System.delaytime_screensave = tmp;
    System.write = _TRUE;
  }
}

void set_actual_language(_UWORD set)
{
  if(System.act_language != set)
  {
    System.act_language = set;
    System.write = _TRUE;
  }
}

_BOOL push_commasign(_BOOL state)
{
  _BOOL retcode = System.commasign;
  System.commasign = state;
  return retcode;
}

void pop_commasign(_BOOL state)
{
  System.commasign = state;
}

void set_commasign(_BOOL cs)
{
  if(System.commasign != cs)
  {
    System.commasign = cs;
    System.write = _TRUE;
  }
}

void set_americandateformat(_BOOL adf)
{
  if(System.americandateformat != adf)
  {
    System.americandateformat = adf;
    System.write = _TRUE;
  }
}

void set_americantimeformat(_BOOL adf)
{
  if(System.americantimeformat != adf)
  {
    System.americantimeformat = adf;
    System.write = _TRUE;
  }
}

void set_cursorwraphorizontal(_BOOL set)
{
  if(System.cursorwraphorizontal != set)
  {
    System.cursorwraphorizontal = set;
    System.write = _TRUE;
  }
}

void set_cursorwrapvertical(_BOOL set)
{
  if(System.cursorwrapvertical != set)
  {
    System.cursorwrapvertical = set;
    System.write = _TRUE;
  }
}

void set_seek_algorithm(_BOOL set)
{
  if(System.seek_algorithm != set)
  {
    System.seek_algorithm = set;
    System.write = _TRUE;
  }
}

void set_directiononenter(_BOOL set)
{
  if(System.directiononenter != set)
  {
    System.directiononenter = set;
    System.write = _TRUE;
  }
}

void set_noexterncom(_BOOL set)
{
  if(System.noexterncom != set)
  {
    System.noexterncom = set;
    System.write = _TRUE;
  }
}

