#include "T_IPC.H"
#include "T_NUC.H"


void init_XCURSOR(_XCURSOR *p)
{
  init_ROOM_Makro(&p->room);
  p->color    = DEFCOLOR;
  p->state    = _FALSE;
  p->visible  = 0;
  p->timediff = 0;
  p->oldtime  = 0;
  p->shape    = 0;
}

void free_XCURSOR(_XCURSOR *p)
{
  init_XCURSOR(p);
}

void _Cursor::Init(void)
{
  init_XCURSOR(&Cursor);
}

void _Cursor::Free(void)
{
  Hide();
  Init();
}

_XCURSOR *_Cursor::Get_XCURSOR(void)
{
  return(&Cursor); 
}

void _Cursor::Push(void)
{
  _UWORD tmp;
  if(Cursor.visible != 0)
  {
    tmp = Cursor.visible;
    Draw();
    Cursor.visible = tmp;
  }
}

void _Cursor::Pop(void)
{
  Push();
}

void _Cursor::Hide(void)
{
  if(Cursor.visible != 0)
    Draw();
}

void draw_button_cursor(_ROOM place, _COLOR color, _ATTRIB att)
{
//  place.xy1.x += 1;
//  place.xy1.y += 1;
//  place.xy2.x -= 1;
//  place.xy2.y -= 1;
  Rectangle(place.xy1.x, place.xy1.y, place.xy2.x, place.xy2.y, att, color);
  place.xy1.x += 1;
  place.xy1.y += 1;
  place.xy2.x -= 1;
  place.xy2.y -= 1;
  Rectangle(place.xy1.x, place.xy1.y, place.xy2.x, place.xy2.y, att, color);

}

void _Cursor::Draw(void)
{
  if(Cursor.state == _TRUE)
  {
	 #ifdef UC_MOUSE
		_UWORD hide = IfNecHideMouseCursor(&Cursor.room);
	 #endif
    switch(Cursor.shape)
    {
      case 0 : Bar(Cursor.room.xy1.x, Cursor.room.xy1.y, Cursor.room.xy2.x, Cursor.room.xy2.y, T_XOR|T_SOLID, Cursor.color);
               break;
      case 1 : draw_button_cursor(Cursor.room, Cursor.color, T_XOR|T_SOLID);
               break;
    }

   #ifdef UC_ROTATE
    RefreshRectangle(&Cursor.room);
   #endif

	 #ifdef UC_MOUSE
		IfNecShowMouseCursor(hide);
	 #endif

    if(DrawBackGround == 0)
      Cursor.visible ^= 1;
  }
}

void _Cursor::SetC(_ROOM *pr, _COLOR color, _UWORD shape, _COLOR backcol)
{
  _COLOR tmp = GetPenColor_Makro(color);

  if(DrawBackGround == 0)
  {
    End();

    if((tmp != INVISIBLE)&&((pr->xy2.x - pr->xy1.x) > 0)&&((pr->xy2.y - pr->xy1.y) > 0))
    {
      if(backcol == DEFCOLOR)
      {
//        if(shape == 1)
//          backcol = ReadPixel(pr->xy1.x, pr->xy1.y);
//        else
          backcol = ReadPixel(pr->xy1.x, pr->xy1.y);
      }

      if(backcol != tmp)
      {
        Cursor.room     = *pr;
        Cursor.state    = _TRUE;
        backcol         = tmp ^ backcol;
        backcol         = GetPenColor_Makro(backcol);
        Cursor.color    = MakeColor_Makro(backcol, backcol);
        Cursor.oldtime  = get_abstime();
        Cursor.timediff = 400;
        Cursor.shape    = shape;
        Draw();
      }
    }
  }
}

void _Cursor::Set(_ROOM *pr, _COLOR color, _UWORD shape)
{
  SetC(pr, color, shape, DEFCOLOR);
}

_BOOL _Cursor::Set(_IO *pio, _EDITSTATE editstate)
{
  _BOOL retcode = _TRUE;
  _ROOM room;

 #ifdef OK_ENCLOSE_NAME
  if(pio->virtname.pthis != NULL)
  {
    if(call_enclosed_name_method(METHOD_MYIO_DISPLAYCURSOR, pio, NULL, NULL, NULL, NULL, _TRUE) == _IDIDIT)
      return(retcode);
  }
 #endif
  {
    switch(pio->typ)
    {
      case AS_THEURGE      : if(pio->theurge != NULL)
                               if(pio->theurge->urgetype == _URGE_SLIDER)
                                 return(_FALSE);
      case AS_ENUMERATION  :
      case AS_IPADDRESS    :
      case AS_NUMERIC_HIDE :
      case AS_ASCII_HIDE   :
      case AS_NUMERIC      :
      case AS_ASCII        :
      case AS_DATE         :
      case AS_TIME         :
      case AS_CHKBOX       :
      case AS_RADIO        :
      case AS_MENU         : retcode = _TRUE;
                             GetTextRoom(&room, &pio->space, pio->attrib, pio->frametype, _FALSE);
                             SetC(&room, SetUp.GetEditColorCursor(editstate), 0, SetUp.GetEditColorBack(editstate, LIGHTBLUE));
                             break;
                             
      case AS_TBO :
      case AS_INPUTBUTTON  : retcode = _TRUE;
                             GetTextRoom(&room, &pio->space, pio->attrib, pio->frametype, _FALSE);
                             SetC(&room, SetUp.GetEditColorCursor(editstate), 1, SetUp.GetEditColorBack(editstate, LIGHTBLUE));
                             break;
      case AS_INPUTBUTTON_WITOUT_CURSOR :
                             retcode = _TRUE;
                             break;
      default              : retcode = _FALSE;
                             break;
    }
  }
 #ifdef OK_ENCLOSE_NAME
//  else if(Cursor.state == _TRUE)
//  {
//    End();
//  }
 #endif

  return(retcode);
}

void _Cursor::End(void)
{
  if(DrawBackGround == 0)
  {
    if(Cursor.state == _TRUE)
    {
      Hide();
      Cursor.state = _FALSE;
    }
  }
}

void _Cursor::Run(void)
{
  if(Cursor.state == _TRUE)
  {
    if(timediff(Cursor.oldtime) >= Cursor.timediff)
    {
      Draw();
      Cursor.oldtime = get_abstime();
    }
  }
}

void _Cursor::CheckRoom(_ROOM *room, _UWORD offset)
{
  if((room->xy1.x + offset) < room->xy2.x)
  {
    room->xy1.x += offset;
    room->xy2.x = room->xy1.x + 1;
  }
  else
  {
    room->xy1.x = room->xy2.x - 1;
  }
}

// **************************************************************************
// **************************************************************************
// **************************************************************************
/*
typedef struct { _ROOM        room;
                 _COLOR       col;
               }
_TEXTCURSOR;

extern "C" void   init_TEXTCURSOR(_TEXTCURSOR*);
extern "C" void   free_TEXTCURSOR(_TEXTCURSOR*);

class _TextCursor
{
  private:
    _TEXTCURSOR tc;
    _UDWORD     TimeDiff, TimeOld;
    _ATTRIB     att;
    _BOOL       Active, Visible, FlashOn;

  public:
    void  Init(void);
    void  Free(void);
    _BOOL Start(_ROOM*, _COLOR);
    _BOOL Set(_ROOM*, _COLOR);
    _BOOL Run(void);
    _BOOL Show();
    _BOOL Hide();
    _BOOL End();
    _COLOR CreateColor(_ROOM*, _COLOR, _ATTRIB, _FRAMETYPE, _EDITSTATE);
    void  CheckRoom(_ROOM*, _UWORD);
};

void init_TEXTCURSOR(_TEXTCURSOR *p)
{
  init_ROOM_Makro(&p->room);
  p->col = DEFCOLOR;
}

void free_TEXTCURSOR(_TEXTCURSOR *p)
{
  init_TEXTCURSOR(p);
}

void _TextCursor::Init(void)
{
  init_TEXTCURSOR(&tc);

  TimeDiff = 0;
  TimeOld = 0;
  att = DEFATTRIB;
  Active  = _FALSE;
  Visible = _FALSE;
  FlashOn = _FALSE;
}

void _TextCursor::Free(void)
{
  free_TEXTCURSOR(&tc);
  Init();
}

_BOOL _TextCursor::Start(_ROOM *room, _COLOR col)
{
  tc.room = *room;
  tc.col = col;
  att = T_XOR|T_SOLID;

  Active = _TRUE;
  Visible = _TRUE;

  return(_TRUE);
}

_BOOL _TextCursor::Set(_ROOM *room, _COLOR col)
{

  if(FlashOn == _TRUE)
  {
    Bar(tc.room.xy1.x, tc.room.xy1.y, tc.room.xy2.x, tc.room.xy2.y, att, tc.col);
    FlashOn = _FALSE;
  }

  tc.room = *room;
  tc.col = col;

  return(_TRUE);
}

_BOOL _TextCursor::Run(void)
{

  if((Visible == _TRUE) && (Active == _TRUE) && (GetPenColor_Makro(tc.col) != INVISIBLE))
  {
    if((timediff(TimeOld)) > 500)
    {
      TimeOld = get_abstime();
      Bar(tc.room.xy1.x, tc.room.xy1.y, tc.room.xy2.x, tc.room.xy2.y, att, tc.col);
      FlashOn = (FlashOn == _TRUE) ? _FALSE : _TRUE;
    }
  }

  return(_TRUE);
}

_BOOL _TextCursor::Show(void)
{

  Visible = _TRUE;

  return(_TRUE);
}

_BOOL _TextCursor::Hide(void)
{

  if(FlashOn == _TRUE)
  {
    Bar(tc.room.xy1.x, tc.room.xy1.y, tc.room.xy2.x, tc.room.xy2.y, att, tc.col);
    FlashOn = _FALSE;
  }

  Visible = _FALSE;

  return(_TRUE);
}

_BOOL _TextCursor::End(void)
{

  if(FlashOn == _TRUE)
    Bar(tc.room.xy1.x, tc.room.xy1.y, tc.room.xy2.x, tc.room.xy2.y, att, tc.col);

  Active = _FALSE;
  Visible = _FALSE;
  FlashOn = _FALSE;

  return(_TRUE);
}

_COLOR _TextCursor::CreateColor(_ROOM *room, _COLOR ColEdit, _ATTRIB att, _FRAMETYPE frametype, _EDITSTATE editstate)
{
  _COLOR retcol, backcol, curcol;
  _ROOM  TextRoom;

  curcol = SetUp.GetEditColorCursor(editstate);
  if(GetPenColor_Makro(ColEdit) == INVISIBLE)
  {
    GetTextRoom(&TextRoom, room, att, frametype, _FALSE);
    backcol = ReadPixel(room->xy1.x, room->xy1.y);
    retcol = backcol ^ curcol;

  }
  else if(GetPenColor_Makro(curcol) == INVISIBLE)
    retcol = BLACK;
  else
    retcol = ColEdit ^ curcol;


  return(retcol);
}
*/


_Cursor *Cursor_Get(void)
{
  return(Root.FindTop()->GetCursor());
}

void Cursor_Hide(_Cursor *Crs)
{
  if(Crs != NULL)
    Crs->Hide();
}

void Cursor_Set(_Cursor *Crs, _ROOM *pr, _COLOR color, _UWORD shape)
{
  if(Crs != NULL)
    Crs->Set(pr, color, shape);
}

void Cursor_End(_Cursor *Crs)
{
  if(Crs != NULL)
    Crs->End();
}

void Cursor_Push(_Cursor *Crs)
{
  if(Crs == NULL)
    Crs = Cursor_Get();
  Crs->Push();
}

void Cursor_Pop(_Cursor *Crs)
{
  if(Crs == NULL)
    Crs = Cursor_Get();
  Crs->Pop();
}


