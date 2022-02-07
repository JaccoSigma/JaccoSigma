#include "T_IPC.H"
#include "T_NUC.H"

#ifdef UC_MOVING_WINDOW
//#define UC_MOVING_WINDOW_MEMORIZE // neue positionen in file speichern und bei projektstart wieder laden

#define WINMOVEFILENAME  "WINMOVE.BIN"
#define WINMOVEFILEID    "WINMOVEID"
#define WINMOVEFILEVERSION  1


// #define WINDOW_MOVE_USEONCE  // positionsänderung wird nur einmalig verwendet und anschliessend wieder zurückgesetzt

#ifndef WINDOW_MOVE_COLOR_0
 #define WINDOW_MOVE_COLOR_0 YELLOW
#endif

#ifndef WINDOW_MOVE_COLOR_1
 #define WINDOW_MOVE_COLOR_1 BLACK
#endif

#ifndef ANZ_WINDOW_MOVE
 #define ANZ_WINDOW_MOVE  256
#endif

typedef struct
{
  _IMA          ima;
  unsigned long no;
  long          dx;
  long          dy;
}
tWindowOffset;

typedef struct
{
  unsigned long state;
  long          xold;
  long          yold;
  long          xstart;
  long          ystart;
  _IMA          ima;
  unsigned long no;
  _ROOM         room;
  _ROOM         screen;
  unsigned long messboxindex;
  unsigned long tix;
  _IMAGE        image;
  unsigned long draw;
  
  unsigned long WinOffsetNo;
  tWindowOffset WinOffset[ANZ_WINDOW_MOVE];
}
tWindowMove;

static tWindowMove WinMove;


static void WindowMoveInit(void)
{
  WinMove.state       = 0;
  WinMove.xold        = 0;
  WinMove.yold        = 0;
  WinMove.xstart      = 0;
  WinMove.ystart      = 0;
  WinMove.ima         = IMA_NOTHING;
  WinMove.no          = 0;
  WinMove.messboxindex = 0xFFFFFFFF;
  WinMove.tix         = 0;
  WinMove.draw        = 0;
  
  init_ROOM(&WinMove.room);
  init_IMAGE(&WinMove.image);
  
  WinMove.WinOffsetNo = 0;
}

//static void WindowMoveFree(void)
//{
//  free_IMAGE(&WinMove.image);
//  WindowMoveInit();
//}

static _BOOL WindowMove_MoveFrame(_DOT *dot)
{
  long dx = (long)dot->x - WinMove.xold;
  long dy = (long)dot->y - WinMove.yold;

  _KOORD x0 = WinMove.room.xy1.x;
  _KOORD y0 = WinMove.room.xy1.y;

  WinMove.xold = dot->x;
  WinMove.yold = dot->y;

  WinMove.room.xy1.x += dx;
  WinMove.room.xy1.y += dy;
  WinMove.room.xy2.x += dx;
  WinMove.room.xy2.y += dy;
  
  dx = 0;
  dy = 0;

  if(WinMove.room.xy1.x < WinMove.screen.xy1.x)
  {
    dx = WinMove.screen.xy1.x - WinMove.room.xy1.x;
  }
  if(WinMove.room.xy1.y < WinMove.screen.xy1.y)
  {
    dy = WinMove.screen.xy1.y - WinMove.room.xy1.y;
  }

  if(WinMove.room.xy2.x > WinMove.screen.xy2.x)
  {
    dx = WinMove.screen.xy2.x - WinMove.room.xy2.x;
  }
  if(WinMove.room.xy2.y > WinMove.screen.xy2.y)
  {
    dy = WinMove.screen.xy2.y - WinMove.room.xy2.y;
  }
  
  WinMove.room.xy1.x += dx;
  WinMove.room.xy2.x += dx;
  WinMove.room.xy1.y += dy;
  WinMove.room.xy2.y += dy;
  
  if((WinMove.room.xy1.x != x0)||(WinMove.room.xy1.y != y0))
  {
    return _TRUE;
  }
  return _FALSE;
}

static void WindowMove_DrawFrame(unsigned long draw, unsigned long restore)
{
  _ROOM room = WinMove.room;

  if(restore != 0)
  {
    PutImage(&WinMove.image, DEFKOORD, DEFKOORD);
  }

  if(draw != 0)
  {
    Rectangle(room.xy1.x, room.xy1.y+1, room.xy2.x, room.xy2.y-1, T_COPY|T_SOLID, MakeColor_Makro(WINDOW_MOVE_COLOR_0, WINDOW_MOVE_COLOR_0));
    Rectangle(room.xy1.x+1, room.xy1.y, room.xy2.x-1, room.xy2.y, T_COPY|T_SOLID, MakeColor_Makro(WINDOW_MOVE_COLOR_0, WINDOW_MOVE_COLOR_0));
    Rectangle(room.xy1.x+2, room.xy1.y+2, room.xy2.x-2, room.xy2.y-2, T_COPY|T_SOLID, MakeColor_Makro(WINDOW_MOVE_COLOR_1, WINDOW_MOVE_COLOR_1));
  }
  
 #ifdef UC_ROTATE
  RefreshRectangle(&WinMove.screen);
 #endif      
}

static tWindowOffset *WindowMoveFindOffset(_IMA ima, unsigned long no)
{
  tWindowOffset *pi = WinMove.WinOffset;
  
  for(unsigned long i=0; i<WinMove.WinOffsetNo; i++, pi++)
  {
    if(pi->no == no)
    {
      if(pi->ima == ima)
      {
        return pi;
      }
    }
  }
  
  return NULL;
}

static _BOOL WindowMoveSetOffset(_IMA ima, unsigned long no, long dx, long dy)
{
  tWindowOffset *pi = WindowMoveFindOffset(ima, no);
  
  if(pi != NULL)
  {
    pi->dx += dx;
    pi->dy += dy;
    return _TRUE;
  }
  
  if(WinMove.WinOffsetNo < ANZ_WINDOW_MOVE)
  {
    pi = &WinMove.WinOffset[WinMove.WinOffsetNo];
    pi->ima = ima;
    pi->no  = no;
    pi->dx  = dx;
    pi->dy  = dy;
    WinMove.WinOffsetNo++;
    return _TRUE;
  }

  return _FALSE;
}

#ifdef UC_MOVING_WINDOW_MEMORIZE
static _UDWORD WindowMoveGetCrc(void)
{
  _UDWORD tmp1 = sizeof(tWindowOffset);
  
  #ifdef _LSL_TARGETARCH_ARM
   tmp1 ^= 0x1800;
  #endif
  
  _UDWORD tmp2 = Project.GetLseProjectCrc();
  return (tmp1 << 16) ^ tmp2;
}

static _BOOL WindowMoveSaveData(void)
{
  _ASCII   dpne[_MAX_PATHLENGTH];
  _File    File;
  _UDWORD  crc32, size;
  _BOOL    retcode = _FALSE;
  
  if(Project.MakeConfigDPNE(dpne, WINMOVEFILENAME, _TRUE) == _TRUE)
  {
    size = WinMove.WinOffsetNo * sizeof(tWindowOffset);
  
    if(File.Open(SAVE, dpne, size + 8 + File.GetHeaderSize(), _TRUE) == _TRUE)
    {
      if(File.Write_HEADER(WINMOVEFILEID, WINMOVEFILEVERSION) == _TRUE)
      {
        // [4] .... lse-projectcrc32
        // [4] .... anzahl entries
        // .[ ..... data
        
        crc32 = WindowMoveGetCrc();
        File.Write(&crc32, 4);
        File.Write(&WinMove.WinOffsetNo, 4);
        File.Write(WinMove.WinOffset, size);
      }
      
      retcode = File.Close();
    }
  }
  
  return retcode;
}

static _BOOL WindowMoveLoadData(void)
{
  _ASCII   dpne[_MAX_PATHLENGTH];
  _File    File;
   _UDWORD version, crc32, nox;
  _BOOL    retcode = _FALSE;
  
  if(Project.MakeConfigDPNE(dpne, WINMOVEFILENAME, _TRUE) == _TRUE)
  {
    if(File.Open(LOAD, dpne) == _TRUE)
    {
      if(File.Read_HEADER(&version, WINMOVEFILEID, _TRUE) == _TRUE)
      {
        if(version == WINMOVEFILEVERSION)
        {
          if(File.Read(&crc32, 4) == _TRUE)
          {
            if(crc32 == WindowMoveGetCrc())
            {
              if(File.Read(&nox, 4) == _TRUE)
              {
                if(nox <= ANZ_WINDOW_MOVE)
                {
                  if(File.Read(WinMove.WinOffset, nox * sizeof(tWindowOffset)) == _TRUE)
                  {
                    WinMove.WinOffsetNo = nox;
                    retcode = _TRUE;
                  }
                }
              }
            }
          }
        }
      }
      File.Close();
    }
  }
  return retcode;
}
#endif // UC_MOVING_WINDOW_MEMORIZE


static _BOOL WindowMoveStop(_EVENT *pe)
{
  WindowMoveSetOffset(WinMove.ima, WinMove.no, WinMove.room.xy1.x - WinMove.xstart, WinMove.room.xy1.y - WinMove.ystart);
  
 #ifdef UC_MOVING_WINDOW_MEMORIZE
  WindowMoveSaveData();
 #endif // UC_MOVING_WINDOW_MEMORIZE
 
  if(WinMove.ima == IMA_WINDOW)
  {
    InsertCmd(CMD_RETURNQUEUE, 0, 0); // close window
    if(WinMove.messboxindex != 0xFFFFFFFF)
    {
      InsertCmd(CMD_MESSAGEBOX, WinMove.no, WinMove.messboxindex); // messagebox wieder öffnen
    }
    else
    {
      InsertCmd(CMD_NEWWINDOW, WinMove.no, 0); // window wieder öffnen
    }
  }
  else if(WinMove.ima == IMA_TOUCHEDITOR)
  {
    _UDWORD esno = DEFSCOPE;
    _Window *pWin = Root.FindTop();
    if(pWin != NULL)
    {
      esno = pWin->GetEditServerNo(); // servernumber
      esno |= (WinMove.no << 16); // windownumber
    }
  
    InsertCmd(CMD_RETURNQUEUE_EX, esno, 0); // close hid-editor und hid-editor wieder öffnen
  }
  
  
  free_IMAGE(&WinMove.image);
  WinMove.state = 0; // ausschalten
  WinMove.draw  = 0;
//  WindowMoveFree();
  
  return _TRUE;
}

_BOOL WindowMoveGetOffset(long *pdx, long *pdy, _IMA ima, unsigned long no)
{
  tWindowOffset *pi = WindowMoveFindOffset(ima, no);
  
  if(pi != NULL)
  {
    *pdx = pi->dx;
    *pdy = pi->dy;
    
   #ifdef WINDOW_MOVE_USEONCE
    pi->dx = 0;
    pi->dy = 0;
   #endif
   
    return _TRUE;
  }
  
  *pdx = 0;
  *pdy = 0;
  
  return _FALSE;
}

_BOOL WindowMoveProjectReady(void)
{
  WindowMoveInit();
  
 #ifdef UC_MOVING_WINDOW_MEMORIZE 
  return WindowMoveLoadData();
 #else // UC_MOVING_WINDOW_MEMORIZE 
  return _TRUE;
 #endif // UC_MOVING_WINDOW_MEMORIZE 
}

_BOOL WindowMoveStart(_Window *PWin, _EVENT *pe, _ROOM *pr)
{
  _WHOAMI wai = PWin->GetWhoAmI(); 

  if((pe->ftype == _EVENT_HIDPRESS) && (pe->multindex == 0))
  {
    if((pe->raw.x > pr->xy1.x) && (pe->raw.x < pr->xy2.x) && (pe->raw.y > pr->xy1.y) && (pe->raw.y < pr->xy2.y))
    {
      if(wai.ima == IMA_WINDOW) 
      {
        if(wai.no == UC_NEWBUBBLEWINDOW)
        {
          return _FALSE;
        }
      }
     #ifdef UC_DYN_EDIT_POSITION
      else if(wai.ima == IMA_TOUCHEDITOR)
      {
        return _FALSE; // weil editorposition dynamisch berechnet wird
      }
     #endif
    
    
     #ifndef CUTOFF_USER
      _FBOOL_PT ptr = (_FBOOL_PT)find_lasal_method(LseThisPointer, METHOD_LSE_WINDOW_MOVEABLE);
      if(ptr != NULL)
      {
        _WHOAMI *pwai = &wai;
        load_this(LseThisPointer);
        if(ptr((void*)pwai) == _FALSE)
        {
          return _FALSE; // user will das nicht
        }
      }
     #endif
    
      WinMove.state        = 1;
      WinMove.ima          = wai.ima;
      WinMove.no           = wai.no;
      WinMove.messboxindex = PWin->GetMessBoxIndex();
      WinMove.xold         = pe->raw.x;
      WinMove.yold         = pe->raw.y;
      WinMove.xstart       = pr->xy1.x;
      WinMove.ystart       = pr->xy1.y;
      WinMove.room         = *pr;
      WinMove.tix          = get_abstime();
      WinMove.draw         = 0;
      GetScreenSize(&WinMove.screen);
      
      _Softkey *psk = PWin->GetSoftkey();
      if(psk != NULL)
      {
        if(psk->GetSoftFloor() > 0) // nur wenn softkeys vorhanden
        {
          _SOFTKEYSETUP *pss = SetUp.Get_SOFTKEYSETUP();
          if(pss != NULL)
          {
            if(pss->horizontal == _TRUE)
            {
              switch(pss->align & T_VERTBOUND)
              {
                case T_UPBOUND   : WinMove.screen.xy1.y += pss->height; break;
                case T_DOWNBOUND : WinMove.screen.xy2.y -= pss->height; break;
              }
            }
            else
            {
              switch(pss->align & T_HORIZBOUND)
              {
                case T_LEFTBOUND  : WinMove.screen.xy1.x += pss->width; break;
                case T_RIGHTBOUND : WinMove.screen.xy2.x -= pss->width; break;
              }
            }
          }
        }
      }
      
      return _TRUE;
    }
  }
  return _FALSE;
}

_BOOL WindowMoveRunSub(_EVENT *pev)
{
  if((pev->ftype != _EVENT_NONE) && (pev->multindex == 0))
  {
    if(pev->ftype == _EVENT_HIDMOVE)
    {
      if((pev->dot.x != WinMove.xold) || (pev->dot.y != WinMove.yold))
      {
        if(WindowMove_MoveFrame(&pev->raw) == _TRUE)
        {
          WinMove.draw ++;
        }
      }
      return _TRUE;
    }  
    else if(pev->ftype == _EVENT_HIDRELEASE)
    {
      WindowMove_MoveFrame(&pev->raw);
      WindowMove_DrawFrame(0, 1);
      WindowMoveStop(pev);
    }
  }
  
  return _FALSE;
}


_BOOL WindowMoveRun(void)
{
  _EVENT event;
  
  if(WinMove.state != 0)
  {
    if(WinMove.state == 1)
    {
      if(timediff(WinMove.tix) > 600)
      {
        WinMove.state = 0; // zeit abgelaufen, wieder ausschalten
      }
    
      Keyboard.GetEvent(&event);
      if((event.ftype == _EVENT_HIDMOVE) && (event.multindex == 0))
      {
        _DWORD dx = event.dot.x - WinMove.xold;
        _DWORD dy = event.dot.y - WinMove.yold;
        
        dx = (dx < 0)? -dx : dx;
        dy = (dy < 0)? -dy : dy;
        
        if((dx > 10) || (dy > 10))
        {
          WinMove.state = 2;
          WindowMove_MoveFrame(&event.raw);
        }
      }
      else if((event.ftype == _EVENT_HIDRELEASE) && (event.multindex == 0))
      {
        WinMove.state = 0; // wieder ausschalten
      }
    }
  
    if(WinMove.state == 2)
    {
      WinMove.state = 0;
      // getimage
      free_IMAGE(&WinMove.image);
      if(GetImage(&WinMove.image, WinMove.screen.xy1.x, WinMove.screen.xy1.y, WinMove.screen.xy2.x, WinMove.screen.xy2.y) == _TRUE)
      {
        // draw frame
        WindowMove_DrawFrame(1, 0);
        WinMove.tix = get_abstime();
        WinMove.state = 3;
      }
    }
    
    if(WinMove.state == 3)
    {
      Keyboard.GetEvent(&event); 
      if(WindowMoveRunSub(&event) == _TRUE)
      {
        // wenn move event dann ein weiteres event aus queue holen. somit wird queue rasch geleert
        Keyboard.GetEvent(&event);
        WindowMoveRunSub(&event);
      }
      
      if((WinMove.draw != 0) && (timediff(WinMove.tix) > 50))
      {
        // rahmen neu zeichen
        WindowMove_DrawFrame(1, 1);
        WinMove.tix  = get_abstime();
        WinMove.draw = 0;
      }
    }
    return _TRUE;
  }
  
  return _FALSE;
}

#endif // UC_MOVING_WINDOW