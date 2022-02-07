#include "T_IPC.H"
#include "T_NUC.H"

/*
#define TRICKNO 40

typedef struct { _UWORD no;
                 _IMAGE *ptr;
               }
_MEM_IMAGE;

typedef struct { _UWORD     cnt;
                 _KOORD     x, y;
                 _REGION    region;
                 _MEM_IMAGE *p;
               }
_TRICKFILM;

void init_TRICKFILM(_TRICKFILM *p)
{
  init_REGION(&p->region);
  p->x = p->y = 0;
  p->p = NULL;
  p->cnt = 0;
}

void free_TRICKFILM(_TRICKFILM *p)
{
  free_REGION(&p->region);
  init_TRICKFILM(p);
}

void add_TRICKFILM(_TRICKFILM *p, _MEM_IMAGE *src, _KOORD x, _KOORD y)
{
  _UWORD    i;
  _TRICKFILM *dst = NULL;

  for(i=0; i<TRICKNO; i++)
  {
    if(p[i].p == NULL)
    {
      dst = &p[i];
      break;
    }
  }

  if(dst == NULL)
  {
    free_TRICKFILM(&p[0]);
    MemMove(&p[0], &p[1], (TRICKNO-1) * sizeof(_TRICKFILM));
    dst = &p[TRICKNO-1];
  }

  init_TRICKFILM(dst);

  _ROOM tmp = src->ptr[0].room;
  tmp.xy1.x += x;
  tmp.xy1.y += y;
  tmp.xy2.x += x - 1;
  tmp.xy2.y += y - 1;

  StartRegion();
  AddRegion(&tmp);
  dst->region = EndRegion();

  for(i=0; i<TRICKNO; i++)
  {
    if(p[i].p != NULL)
    {
      _REGION tr = p[i].region;
      p[i].region = SubRegion(tr, dst->region);
      FreeRegion(&tr);
    }
  }

  dst->p = src;
  dst->x = x;
  dst->y = y;
}

void run_TRICKFILM(_TRICKFILM *p)
{
  if(p->p != NULL)
  {
    p->cnt++;
    if(p->cnt >= p->p->no)
      p->cnt = 0;

    ActivateRegion(p->region);
    PutImage(&p->p->ptr[p->cnt], p->x, p->y);
    DeactivateRegion();
  }
}


_BOOL make_cartoon(_MEM_IMAGE*, _BMP*, _UWORD);
_MEM_IMAGE  mi;
_TRICKFILM  trickfilm[TRICKNO];
_UWORD cnt, dir;
_BOOL make_trickfilm(_MEM_IMAGE*, _ROOM*, _UWORD, _UWORD);
*/

void _ScreenSaver::Init(void)
{
  init_WORKSTATE(&WorkState);
  init_IMAGE_Makro(&Image);
  init_ROOM_Makro(&Room);
  init_SCREENSAVER(&ScreenSaver);
  init_DOT(&OldPos);
  OldTime = 0;
  Width   = 0;
  Height  = 0;
}

void _ScreenSaver::Free(void)
{
  free_IMAGE(&Image);
  free_SCREENSAVER(&ScreenSaver);
  Init();
}

void _ScreenSaver::Ready(void)
{
  _UWORD x, y;
  _BMP  *bmp;

  ScreenSaver = *SetUp.Get_SCREENSAVER();
  bmp = ImageList.Get(ScreenSaver.image);
  MyGetScreenSize(&Room);
  init_WORKSTATE(&WorkState);

  if((get_delay_screensave() > 0) && (bmp != NULL))
  {
    Room.xy2.x -= bmp->width;
    Room.xy2.y -= bmp->height;

    if(Room.xy2.x < 0)
      Room.xy2.x = 0;
    if(Room.xy2.y < 0)
      Room.xy2.y = 0;

    x = Random(Room.xy2.x);
    y = Random(Room.xy2.y);

    MyClrScr(BLACK);
    OldPos.x = x;
    OldPos.y = y;
    Width  = bmp->width - 1;
    Height = bmp->height - 1;
    Bitmap(x, y, bmp, T_SOLID|T_COPY, DEFCOLOR);
   #ifdef UC_ROTATE
    _ROOM room;
    MyGetScreenSize(&room);
    RefreshRectangle(&room);
   #endif
    if(GetImage(&Image, x, y, x + Width, y + Height) == _TRUE)
    {
      OldTime = get_abstime();
      WorkState = STATE_RUN;
    }
  }

//  _UWORD i;
//  make_trickfilm(&mi, NULL, 6, 43);
//  cnt = 0XFFFF;
//  dir = 0;
//  for(i=0; i<TRICKNO; i++)
//    init_TRICKFILM(&trickfilm[i]);
}

//_KOORD slurryx, slurryy;

void _ScreenSaver::Run(void)
{
  _UWORD x, y;
//  _UDWORD i;

  if((WorkState == STATE_RUN)&&(ScreenSaver.actiontime > 0))
  {
    if(timediff(OldTime) >= ScreenSaver.actiontime)
    {
      OldTime = get_abstime();

//      if(slurryy > slurryx)
//      {
//        add_TRICKFILM(trickfilm, &mi, Random(Room.xy2.x), Random(Room.xy2.y));
//        slurryx = Random(43);
//        slurryy = 0;
//      }
//      slurryy ++;
//      for(i=0; i<TRICKNO; i++)
//        run_TRICKFILM(&trickfilm[i]);

      x = Random(Room.xy2.x);
      y = Random(Room.xy2.y);

      if(ScreenSaver.clrscr == _TRUE)
      {
        Bar(OldPos.x, OldPos.y, OldPos.x+Width, OldPos.y+Height, T_COPY|T_SOLID, BLACK);
       #ifdef UC_ROTATE
        _ROOM room;
        room.xy1.x = OldPos.x;
        room.xy1.y = OldPos.y;
        room.xy2.x = OldPos.x + Width;
        room.xy2.y = OldPos.y + Height;
        RefreshRectangle(&room);
       #endif
//        ClrScr(BLACK);
      }
      OldPos.x = x;
      OldPos.y = y;

      PutImage(&Image, x, y);
     #ifdef UC_ROTATE
      _ROOM room;
      room.xy1.x = x;
      room.xy1.y = y;
      room.xy2.x = x + Width;
      room.xy2.y = y + Height;
      RefreshRectangle(&room);
     #endif
    }
  }
}
/*
void init_MEM_IMAGE(_MEM_IMAGE *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_IMAGE(_MEM_IMAGE *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_IMAGE(&p->ptr[i]);
  Memory2((void**)&p->ptr, 0);
  init_MEM_IMAGE(p);
}

_BOOL make_trickfilm(_MEM_IMAGE *p, _ROOM *pr, _UWORD start, _UWORD no)
{
  _BOOL   retcode = _FALSE;
  _BMP    *bmp;
  _UWORD  i;
  _UDWORD size;
  _ROOM   room;

  size = no * 2 * sizeof(_IMAGE);
  if(Memory2((void**)&p->ptr, size) != 0)
  {
    retcode = _TRUE;
	p->no = no;
    for(i=0; i<no; i++)
    {
      init_IMAGE_Makro(&p->ptr[i]);
      if(retcode == _TRUE)
      {
        bmp = ImageList.Get(i+start);
        if(bmp != NULL)
        {
          if(pr == NULL)
          {
            room.xy1.x = 0;
            room.xy1.y = 0;
            room.xy2.x = bmp->width - 1;
            room.xy2.y = bmp->height - 1;
            pr = &room;
          }
          out_zoom_BMP(pr, bmp, T_COPY|T_SOLID);
        }
        retcode = (GetImage(&p->ptr[i], pr->xy1.x, pr->xy1.y, pr->xy2.x, pr->xy2.y) != 0)? _TRUE : _FALSE;
      }
    }
  }

  return(retcode);
}
*/
/*
_BOOL make_cartoon(_MEM_IMAGE *p, _BMP *bmp, _UWORD no)
{
  _UDWORD size;
  _BOOL   retcode = _FALSE;
  _ROOM   room, space;
  _UWORD  i, h, x;

  _UWORD hh;
  _REAL  alpha;

  init_MEM_IMAGE(p);

  if(bmp != NULL)
  {
    size = no * 2 * sizeof(_IMAGE);
    if(Memory2((void**)&p->ptr, size) != 0)
    {
      p->no = 2 * no;
      for(i=0; i<p->no; i++)
        init_IMAGE_Makro(&p->ptr[i]);

      init_ROOM_Makro(&room);
      room.xy1.x = 0;
      room.xy1.y = 0;
      room.xy2.x = bmp->width - 1;
      room.xy2.y = bmp->height - 1;
      space   = room;
      retcode = _TRUE;

      hh = (space.xy2.y - space.xy1.y + 1) / 2;

      for(i=0; i<no; i++)
      {
        if(retcode == _TRUE)
        {
//          h = ((space.xy2.y - space.xy1.y + 1) * i) / ((no-1)*2);
          alpha = ((90*i)/(no-1));
          alpha = (alpha * 3.1416)/180;
          h     = (_KOORD)FTOL(SIN(alpha) * hh);

          room.xy1.y = space.xy1.y + h;
          room.xy2.y = space.xy2.y - h;

if(room.xy1.y >= room.xy2.y)
{
  room.xy1.y = space.xy1.y + space.xy2.y / 2 - 1;
  room.xy2.y = room.xy1.y + 1;
}

          Bar(space.xy1.x, space.xy1.y, space.xy2.x, space.xy2.y, T_COPY|T_SOLID, BLACK);
          out_zoom_BMP(&room, bmp, T_COPY|T_SOLID);
          retcode = (GetImage(&p->ptr[i], space.xy1.x, space.xy1.y, space.xy2.x, space.xy2.y) != 0)? _TRUE : _FALSE;

          bmp->state ^= BMP_TURN;
          Bar(space.xy1.x, space.xy1.y, space.xy2.x, space.xy2.y, T_COPY|T_SOLID, BLACK);
          out_zoom_BMP(&room, bmp, T_COPY|T_SOLID);
          retcode = (GetImage(&p->ptr[no*2-1-i], space.xy1.x, space.xy1.y, space.xy2.x, space.xy2.y) != 0)? _TRUE : _FALSE;
          bmp->state ^= BMP_TURN;
        }
      }

    }
  }

  if(retcode == _FALSE)
    free_MEM_IMAGE(p);

  return(retcode);
}
*/





