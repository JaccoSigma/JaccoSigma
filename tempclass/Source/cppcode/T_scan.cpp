#include "T_IPC.H"
#include "T_NUC.H"  


#ifdef UC_INVISIBLE_PROGRESS // *******************************************************************

void _ScanBar::Init(void)
{
  Ptr = NULL;
  No  = 0;
  Pos = 0;
  Visible = _FALSE;
}

void _ScanBar::Free(void)
{
  for(_UWORD i=0; i<No; i++)
  {
    free_SCAN(&Ptr[i]);
  }
  
  No = 0;
  Memory2((void**)&Ptr, 0);
  Ptr = NULL;
  //Init();
  // Pos nicht auf 0 setzen !!!
}

void _ScanBar::SetPos(_UWORD no)
{
  Pos = no;
}

_UWORD _ScanBar::Add(const _ASCII *txt, _DWORD max)
{
  return Add(txt, max, MakeColor_Makro(BLACK, LIGHTGRAY));
}

void _ScanBar::Draw(_SCAN *p)
{
  if(Visible == _TRUE)
  {
    if(p->oldx < 0)
    {
      Font.Set(DEFFONT, NULL);
      if(p->text != NULL)
      {
        OutTextXY(10, p->room.xy1.y, (void*)p->text, T_COPY|T_SOLID|T_PROP, p->color, sizeof(_ASCII));
      }
      
      if(p->max != 0)
      {
        _COLOR col1 = MakeColor_Makro(BLACK, BLACK);
        _COLOR col2 = MakeColor_Makro(WHITE, DARKGRAY);
        DrawButton(&p->room, T_COPY|T_SOLID, _3DFRAME, col1, col2);
      }
    
     #ifdef UC_ROTATE
      _ROOM room;
      room.xy1.x = 10; 
      room.xy1.y = p->room.xy1.y;
      room.xy2.x = p->room.xy2.x; 
      room.xy2.y = p->room.xy2.y;
      RefreshRectangle(&room);
     #endif
    
      p->oldx = 0;
    }  
  
    if(p->max > 0)
    {
      _KOORD x;
      if(p->i == 0)
      {
        x = 0;
      }
      else if(p->i >= p->max)
      {
        x = p->room.xy2.x - p->room.xy1.x;
      }
      else
      {
        x = ((p->i*(p->room.xy2.x - p->room.xy1.x))/p->max);
      }

      if(x != p->oldx)
      {
        if(x < p->oldx)
        {
          Bar(p->room.xy1.x+1, p->room.xy1.y+1, p->room.xy2.x-1, p->room.xy2.y-1, T_SOLID|T_COPY, MakeColor_Makro(BLACK, BLACK));
        }
        if(x > 0)
        {
          Bar(p->room.xy1.x+1, p->room.xy1.y+1, p->room.xy1.x+x, p->room.xy2.y-1, T_SOLID|T_COPY, p->color);
        }
        p->oldx = x;
       #ifdef UC_ROTATE
        RefreshRectangle(&p->room);
       #endif
      }
    }
  }
}

_UWORD _ScanBar::Add(const _ASCII *txt, _DWORD max, _COLOR col)
{
  _DWORD size;
  _UWORD retcode = 0xFFFF;
  _KOORD y, xx;
  _SCAN  *p;
  _ROOM  sroom;

  size = (No+1) * sizeof(_SCAN);

  PushMemColor(MEMORY_PRG);
  if(Memory2((void**)&Ptr, size) != 0)
  {
    GetScreenSize(&sroom);

    p = &Ptr[No];
    init_SCAN(p);
   
   #ifdef UC_GRAYSCALE   
    _UWORD c_lo = col & 0xFFFF; 
    _UWORD c_hi = (col >> 16) & 0xFFFF;
    transform_color_to_gray(&c_lo);
    transform_color_to_gray(&c_hi);
    col  = (c_hi << 16) | c_lo;
   #endif
   
    y = (Pos + 2) * 12;
    p->color      = col;
    p->max        = max;
    p->oldx       = -1;
    p->room.xy1.x = 10;
    p->room.xy1.y = y;
    p->room.xy2.x = sroom.xy2.x-20;
    p->room.xy2.y = y + 8;
    retcode = No;
    Pos ++;
    No  ++;
    _UWORD strlen = StrLen(txt, 1) + 1;
    if(Memory2((void**)&p->text, strlen) != 0)
    {
      MemCpy(p->text, txt, strlen);
      Font.Set(DEFFONT, NULL);
      xx = StrWidth((void*)p->text, sizeof(_ASCII)) + 14;
      if((xx > p->room.xy1.x) && (xx < p->room.xy2.x))
      {
        p->room.xy1.x = xx;
      }
    }

    Draw(p);

  }
  PopMemColor();

  return(retcode);
}

void _ScanBar::DoVisible(void)
{
  if(Visible == _FALSE)
  {
    Visible = _TRUE;
    for(_UDWORD i=0; i<No; i++)
    {
      Draw(&Ptr[i]);
    }
  }
}

void _ScanBar::Run(_UWORD i, _DWORD akt)
{
  _EVENT event;
  
  if(i < No)
  {
    _SCAN *p = &Ptr[i];
    p->i = akt;
    Draw(p);
    
    //UserFunctAufrufen(p, Visible); // todo
   #ifndef CUTOFF_USER
    _FVOID_PT_BOOL ptr = (_FVOID_PT_BOOL)find_lasal_method(LseThisPointer, METHOD_LSE_PROGRESS);
    if(ptr != NULL)
    {
      load_this(LseThisPointer);
      ptr((void*)p, Visible);
    }
   #endif
  }
  
  if(Visible == _FALSE)
  {
    Keyboard.GetEvent(&event);
  
    if(event.ftype == _EVENT_KEYPRESS)
    {
      if(event.scancode == _TAB)
      {
        DoVisible();
      }
    }
  }
}

void _ScanBar::Reset(_UWORD i, _DWORD max, _BOOL changecolor)
{
  _SCAN *p;

  if(i < No)
  {
    p      = &Ptr[i];
    p->i   = 0;
    p->max = max;

    _UWORD col = (changecolor == _TRUE)? LIGHTGREEN : LIGHTGRAY;
   #ifdef UC_GRAYSCALE
    transform_color_to_gray(&col);
   #endif
    p->color = MakeColor_Makro(BLACK, col);
  
    Draw(p);
  }
}

void _ScanBar::End(_UWORD i)
{
  if(i < No)
  {
    Run(i, Ptr[i].max);
  }
}

// ************************************************************************************************
// ************************************************************************************************
#else // UC_INVISIBLE_PROGRESS ********************************************************************
// ************************************************************************************************
// ************************************************************************************************

void _ScanBar::Init(void)
{
  Ptr = NULL;
  No  = 0;
  Pos = 0;
}

void _ScanBar::Free(void)
{
  _UWORD i;

  for(i=0; i<No; i++)
  {
    free_SCAN(&Ptr[i]);
  }
  
  Memory2((void**)&Ptr, 0);
  Init();
}

void _ScanBar::SetPos(_UWORD no)
{
  Pos = no;
}

_UWORD _ScanBar::Add(const _ASCII *txt, _DWORD max)
{
  return Add(txt, max, MakeColor_Makro(BLACK, LIGHTGRAY));
}

_UWORD _ScanBar::Add(const _ASCII *txt, _DWORD max, _COLOR col)
{
  _DWORD size;
  _UWORD retcode = 0xFFFF;
  _KOORD y, xx;
  _SCAN  *p;
  _COLOR col1, col2;
  _ROOM  sroom;

  size = (No+1) * sizeof(_SCAN);

  PushMemColor(MEMORY_PRG);
  if(Memory2((void**)&Ptr, size) != 0)
  {
    GetScreenSize(&sroom);

    p = &Ptr[No];
    init_SCAN(p);
    y = (Pos+2) * 12;
    p->max        = max;
    p->room.xy1.x = 10;
    p->room.xy1.y = y;
    p->room.xy2.x = sroom.xy2.x-20;
    p->room.xy2.y = y + 8;
    retcode = No;
    Pos ++;
    No  ++;
    Font.Set(DEFFONT, NULL);
    OutTextXY(10, y, (void*)txt, T_COPY|T_SOLID|T_PROP, col, sizeof(_ASCII));
   #ifdef UC_ROTATE
    RefreshRectangle(&p->room);
   #endif
    xx = StrWidth((void*)txt, sizeof(_ASCII)) + 14;
    if((xx > p->room.xy1.x)&&(xx < p->room.xy2.x))
      p->room.xy1.x = xx;

    if(max != 0)
    {
      col1 = MakeColor_Makro(BLACK, BLACK);
      col2 = MakeColor_Makro(WHITE, DARKGRAY);
      DrawButton(&p->room, T_COPY|T_SOLID, _3DFRAME, col1, col2);
//      Button(p->room.xy1.x-1,p->room.xy1.y-1, p->room.xy2.x+1,p->room.xy2.y+1, T_COPY|T_SOLID, col1, col2);
    }
  }
  PopMemColor();

 #ifdef UC_ROTATE
  RefreshRectangle(&p->room);
 #endif

  return(retcode);
}

void _ScanBar::Run(_UWORD i, _DWORD akt)
{
  _SCAN *p;
  _KOORD x;

  if(i < No)
  {
    p = &Ptr[i];
    
    p->i = akt;
    
    if(akt == 0)
      x = 0;
    else if(akt == p->max)
      x = p->room.xy2.x - p->room.xy1.x;
    else
      x = ((akt*(p->room.xy2.x - p->room.xy1.x))/p->max);

    if(x != p->oldx)
    {
      //Bar(p->room.xy1.x+1, p->room.xy1.y+1, p->room.xy1.x+x, p->room.xy2.y-1, T_SOLID|T_COPY, MakeColor_Makro(BLACK, LIGHTGRAY));
      Bar(p->room.xy1.x+1, p->room.xy1.y+1, p->room.xy1.x+x, p->room.xy2.y-1, T_SOLID|T_COPY, p->color);
      
      p->oldx = x;
     #ifdef UC_ROTATE
      RefreshRectangle(&p->room);
     #endif

    }
  }
}

void _ScanBar::Reset(_UWORD i, _DWORD max, _BOOL changecolor)
{
  _SCAN *p;

  if(i < No)
  {
    p = &Ptr[i];
    p->oldx = 0;
    p->max  = max;
   
    _UWORD col = (changecolor == _TRUE)? LIGHTGREEN : LIGHTGRAY;
   #ifdef UC_GRAYSCALE
    transform_color_to_gray(&col);
   #endif
    p->color = MakeColor_Makro(BLACK, col);
   
    Bar(p->room.xy1.x+1, p->room.xy1.y+1, p->room.xy2.x-1, p->room.xy2.y-1, T_SOLID|T_COPY, MakeColor_Makro(BLACK, BLACK));
   #ifdef UC_ROTATE
    RefreshRectangle(&p->room);
   #endif
  }
}

void _ScanBar::End(_UWORD i)
{
  if(i<No)
  {
    Run(i, Ptr[i].max);
    if((i+1) == No)
    {
      free_SCAN(&Ptr[i]);
      No --;
      PushMemColor(MEMORY_PRG);
      Memory2((void**)&Ptr, No*sizeof(_SCAN));
      PopMemColor();
    }
  }
}

#endif // UC_INVISIBLE_PROGRESS *******************************************************************

_DWORD _ScanBar::Get(_UWORD i)
{
  if(i < No)
  {
    return Ptr[i].i;
  }
  return 0;
}

void _ScanBar::Assert(const _ASCII *txt)
{
  ScanBar.End(ScanBar.Add(txt, 0, MakeColor(BLACK, YELLOW)));
 #ifdef UC_INVISIBLE_PROGRESS
  DoVisible();
  Sleep(2000);
 #endif
}

void _ScanBar::Message(const _ASCII *txt)
{
  ScanBar.End(ScanBar.Add(txt, 0));
}

void _ScanBar::ColorSet(_UWORD i, _COLOR color)
{
  if(i < No)
  {
    _SCAN *p = &Ptr[i];
   #ifdef UC_GRAYSCALE
    transform_color_to_gray(&color);
   #endif
    p->color = MakeColor_Makro(BLACK, color);
  }
}

//****************************************************************************
//  LASAL Interface
//****************************************************************************
//****************************************************************************
//****************************************************************************

extern "C" _UWORD ScanBar_Add(_ASCII *txt, _DWORD max)
{
  return(ScanBar.Add(txt, max));
}

extern "C" void ScanBar_Run(_UWORD handle, _DWORD act)
{
  ScanBar.Run(handle, act);
}

extern "C" void ScanBar_End(_UWORD handle)
{
  ScanBar.End(handle);
}
