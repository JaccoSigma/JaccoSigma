#include "T_IPC.H"
#include "T_NUC.H"

#ifdef UC_BUBBLE_HELP

void _NewBubbleHelp::Init(void)
{
  Text    = NULL;
  TextLen = 0;
  
  init_ROOM_Makro(&Room);
  ColorText  = BLACK;
  ColorBack  = MakeColor(LIGHTGRAY, YELLOW);
  ColorFrame = MakeColor(WHITE, DARKGRAY);
  Attrib     = T_LEFTBOUND|T_MIDBOUND|T_COPY|T_SOLID|T_PROP;
  Frame      = _3DFRAME;
  Font       = 0;
}

void _NewBubbleHelp::Free(void)
{
  Memory2((void**)&Text, 0);
  Init();
}

void _NewBubbleHelp::DoView(void)
{
  _BUBBLEINFO *pbi = SetUp.Get_BUBBLEINFO();

  Font       = pbi->font;
  Attrib     = pbi->attrib;
  Frame      = pbi->frametype;
  ColorBack  = SetUp.GetBubbleColorBack();
  ColorFrame = SetUp.GetBubbleColorFrame();
  ColorText  = SetUp.GetBubbleColorTxt();  
}

_BOOL _NewBubbleHelp::DoSize(_PICTURE *ppic)
{
  ppic->mask.room = Room;
  return _TRUE;
}

void _NewBubbleHelp::Ready(_MY_PICTURE *mp)
{
  OutMultiTextAlign(Text, 0, &mp->Room, Font, Attrib, Frame, ColorText, ColorBack, ColorFrame, sizeof(_CHAR));
  
 #ifdef UC_ROTATE
  RefreshRectangle(&mp->Room);
 #endif  
}

void _NewBubbleHelp::Run(_MY_PICTURE *mp, _EVENT *pe)
{
  if(mp->JustProgram == _TRUE)
  {
    if(pe->ftype != _EVENT_NONE)
    {
      if(pe->ftype != _EVENT_HIDMOVE)
        InsertCmd(CMD_RETURN, 0, 0);
    }
  }  
}

_BOOL _NewBubbleHelp::MakePosition(_ROOM *pr, _KOORD x, _KOORD y, _FONT font, _CHAR *txt, _FRAMETYPE frametype, _ATTRIB attrib)
{
  _ROOM  screen, room;
  _UWORD xx, yy;

  if(SizeMultiTextAlign(&xx, &yy, txt, font, attrib, sizeof(_CHAR)) == _TRUE)
  {
    room.xy1.x = 1;
    room.xy1.y = 1;
    room.xy2.x = 300;
    room.xy2.y = 300;
    GetTextRoom(&screen, &room, attrib, frametype, _TRUE);
    
    xx += (2 + (room.xy2.x - room.xy1.x) - (screen.xy2.x - screen.xy1.x));
    yy += (2 + (room.xy2.y - room.xy1.y) - (screen.xy2.y - screen.xy1.y));

    y -= yy;
    
    MyGetScreenSize(&screen);

    if(xx > (screen.xy2.x - screen.xy1.x + 1))
      xx = screen.xy2.x - screen.xy1.x + 1;
    if(yy > (screen.xy2.y - screen.xy1.y + 1))
      yy = screen.xy2.y - screen.xy1.y + 1;

    if((x + xx) > screen.xy2.x)
      x = screen.xy2.x - xx;
    if((y + yy) > screen.xy2.y)
      y = screen.xy2.y - yy;
    if(x < 0)
      x = 0;
    if(y < 0)
      y = 0;

    pr->xy1.x = x;
    pr->xy1.y = y;
    pr->xy2.x = x + xx - 1;
    pr->xy2.y = y + yy - 1;

    return _TRUE;
  }

  return _FALSE;
}

_BOOL _NewBubbleHelp::Call(_KOORD x, _KOORD y, void *text, _UWORD chrsize)
{
  _BOOL retcode = _TRUE;
  
  SetLseServer(LSE_ACTIVATEBUBBLEHELP, 0);
  
  if(text != NULL)
  {
    _UDWORD len = StrLen(text, chrsize) + 1;

    if(len > TextLen)
    {
      if(Memory2((void**)&Text, len * sizeof(_CHAR)) != 0)
      {
        TextLen = len;
      }
      else
      {
        retcode = _FALSE;
      } 
    }

    if(retcode == _TRUE)
    {
      StrCpy(Text, sizeof(_CHAR), text, chrsize);
      DoView();
      if(MakePosition(&Room, x, y, Font, Text, Frame, Attrib) == _TRUE)
      {
        InsertCmd(CMD_MESSAGEBOX, UC_NEWBUBBLEWINDOW, DEFSCOPE);
        retcode = _TRUE;
      }
    }
  }
  
  return retcode;
}

_BOOL _NewBubbleHelp::Call(_KOORD x, _KOORD y, _ANYPARA *pany, _VARLABEL *pvl)
{
  _CHAR *ptxt = NULL;
  
  if(pany != NULL)
    ptxt = get_ANYPARA(pany, NULL);
  
  if((ptxt == NULL)&&(pvl != NULL))
  {
    if(valid_VARLABEL(pvl) == _TRUE)
    {  
      _LSEVARINFO *pvi;
      if(VarList.GetVarInfo(&pvi, pvl->info[0].value) == _TRUE)
      {
        if(pvi->xmemtext.xxno > 3)
        {
          ptxt = get_ANYPARA(&pvi->xmemtext.xxptr[3], NULL);
        }
      }
    }
  }
  
  if(ptxt != NULL)
    return Call(x, y, ptxt, sizeof(_CHAR));

  return Call(x, y, (void*)"???", sizeof(_ASCII));
}

_FRAMETYPE _NewBubbleHelp::GetFrame(void)
{
  return Frame;
}

#endif // UC_BUBBLE_HELP


//#define USExINTERN_HEAP
#ifdef USExINTERN_HEAP

#define XSMALLBLOCK (sizeof(_XHEAPCELL) + 4)
#define XHEAPCHECK

typedef struct _myheapheader
{
  _myheapheader  *pprev;
  _myheapheader  *pnext;
  _myheapheader  *fprev;
  _myheapheader  *fnext;
  _UDWORD        length;
  _UDWORD        data;
  _UDWORD        used;
}
_XHEAPCELL;

typedef struct
{
  _UDWORD    Init;
  _XHEAPCELL *Used;
  _XHEAPCELL *Free;
  
  void       *pend;
  void       *pstart;
  
 #ifdef XHEAPCHECK
  _UDWORD    cellno;
  _UDWORD    cellfree;
 #endif
}
_XHEAP;

_XHEAP XHeap;

void XHeapInit(_UDWORD heapsize)
{
  if(Memory((void**)&XHeap.Used, heapsize) != 0)
  {
    XHeap.pstart = (void*)XHeap.Used;
    _UBYTE *pt   = (_UBYTE*)XHeap.pstart;
    XHeap.pend   = pt + heapsize;
  
    XHeap.Used->length = heapsize;// - sizeof(_XHEAPCELL);
    XHeap.Used->data   = 0;
    XHeap.Used->pnext  = NULL;
    XHeap.Used->pprev  = NULL;
    XHeap.Used->fnext  = NULL;
    XHeap.Used->fprev  = NULL;
    XHeap.Used->used   = false;
    XHeap.Free         = XHeap.Used;
    
   #ifdef XHEAPCHECK
    XHeap.cellno   = 0;
    XHeap.cellfree = 0;
   #endif
    
    
    XHeap.Init         = 0x4F5E6D7C;
  }
}

void CompleteXHeap()
{
  _XHEAPCELL *ph = XHeap.Used;
  _XHEAPCELL *po = NULL;

 #ifdef XHEAPCHECK
  XHeap.cellno   = 0;
  XHeap.cellfree = 0;
 #endif
  
  _XHEAPCELL **ppf = &XHeap.Free;
  while(ph != NULL)
  {
    if(ph->used == false)
    {
     #ifdef XHEAPCHECK
      XHeap.cellfree += 1;
     #endif
      ph->fprev = po;
      *ppf      = ph;
      ppf       = &ph->fnext;
      po        = ph;
    }

   #ifdef XHEAPCHECK
    XHeap.cellno += 1;
   #endif
   
    ph = ph->pnext;
  }
  
  *ppf = NULL;
}

void SplitFreeBlock(_XHEAPCELL *pc, _UDWORD blocksize)
{
  _UBYTE *pt     = (_UBYTE*)pc;
  pt            += blocksize;
  _XHEAPCELL *px = (_XHEAPCELL*)pt;

  // freekette berichtigen  
  px->fprev = pc->fprev;
  if(px->fprev != NULL)
    px->fprev->fnext = px;
    
  px->fnext = pc->fnext;
  if(px->fnext != NULL)
    px->fnext->fprev = px;
  
  // usedkette und cell berichtigen
  px->pprev = pc;
  
  px->pnext = pc->pnext;
  if(px->pnext != NULL)
    px->pnext->pprev = px;
    
  px->length = pc->length - blocksize;
  px->used   = false;
  px->data   = 0;

  pc->pnext  = px;
  pc->used   = true;
  pc->length = blocksize;
  pc->fnext  = NULL;
  pc->fprev  = NULL;
  
  // ggf.start von freekette berichtigen
  if((XHeap.Free != NULL) && (XHeap.Free == pc))
    XHeap.Free = px;
}

void SplitUsedBlock(_XHEAPCELL *pc, _UDWORD blocksize)
{
  pc->fnext  = NULL;
  pc->fprev  = NULL;
  XHeap.Free = NULL;
  SplitFreeBlock(pc, blocksize);
}

_XHEAPCELL *XHeapAlloc(_UDWORD size, _UDWORD cellinfo)
{
  if(size & 0x00000003)
    size = (size & 0xFFFFFFFC) + 4;

  _UDWORD blocksize = size + sizeof(_XHEAPCELL);
  
  if(XHeap.Free == NULL)
    CompleteXHeap();
  
  _XHEAPCELL *ph = XHeap.Free;
  
  while(ph != NULL)
  {
    if(ph->length >= blocksize)
    {
      if((ph->length - blocksize) <= XSMALLBLOCK)
      {
        // freibleibender block wird zu klein, deshalb wird freeblock zu usedblock umfunktioniert
        
        // block aus freekette lösen
        if(ph->fnext != NULL)
          ph->fnext->fprev = ph->fprev;
        if(ph->fprev != NULL)
          ph->fprev->fnext = ph->fnext;
        
        // ggf.start von freekette berichtigen
        if((XHeap.Free != NULL) && (XHeap.Free == ph))
          XHeap.Free = ph->fnext;
          
        ph->used  = true;
        ph->fnext = NULL;
        ph->fprev = NULL;
        ph->data  = cellinfo;
      }
      else
      {
        // free block wird in used block umgewandelt, zusätzlich wird ein neuer free-block eingefügt
        SplitFreeBlock(ph, blocksize);
        ph->data  = cellinfo;
      }
      
      return ph;
    }
    
    ph = ph->fnext;
  }
  
  return NULL;
}

void XHeapFree(_XHEAPCELL *pc)
{
  if(pc->pprev != NULL)
  {
    if(pc->pprev->used == false)
    {
      // verschmelze mit vorgängerblock
      _XHEAPCELL *p1 = pc->pprev;
      
      p1->pnext = pc->pnext;
      if(p1->pnext != NULL)
        p1->pnext->pprev = p1;
     
      p1->length += pc->length;
      pc       = p1;
      pc->used = false;
    }
  }
  
  if(pc->pnext != NULL)
  {
    if(pc->pnext->used == false)
    {
      // verschmelze mit nachfolgerblock
      _XHEAPCELL *p2 = pc->pnext;
      
      pc->pnext = p2->pnext;
      if(pc->pnext != NULL)
        pc->pnext->pprev = pc;
    
      pc->fnext = p2->fnext;
      if(pc->fnext != NULL)
        pc->fnext->fprev = pc;
        
      if((pc->fprev == NULL) && (p2->fprev != pc))  
      {  
        pc->fprev = p2->fprev;
        if(pc->fprev != NULL)
          pc->fprev->fnext = pc;
      }

      pc->length += p2->length;
      pc->used = false;
      
      // ggf.start von freekette berichtigen
      if((XHeap.Free != NULL) && (XHeap.Free == p2))
        XHeap.Free = pc;
    }
  }
  
  // free eigentlichen block
  if(pc->used == true)
  {
    pc->used = false;
    XHeap.Free = NULL;
  }
}

_XHEAPCELL *XHeapRealloc(_XHEAPCELL* pc, _UDWORD size)
{
  if(size & 0x00000003)
    size = (size & 0xFFFFFFFC) + 4;

  _UDWORD blocksize = size + sizeof(_XHEAPCELL);

  if(blocksize <= pc->length)
  {
/*  
    if((pc->length - blocksize) > XSMALLBLOCK)
    {
      // bei verkleinerung split block in used und free
      if(pc->pnext != NULL)
      {
        if(pc->pnext->used == false)
        {
          
          return pc;
        }
      }
      SplitUsedBlock(pc, blocksize);
    }
*/
    return pc; // block ist gross genug
  }
  
  _UDWORD oldlen = pc->length - sizeof(_XHEAPCELL);
  _UDWORD data   = pc->data;

  // todo: check ob überhaupt ram frei ist

  _XHEAPCELL *ph = XHeapAlloc(size, data);
  
  if(ph != pc)
    MemCpy(&ph[1], &pc[1], oldlen);

  XHeapFree(pc);

  return ph;
}


void *MyHeapAlloc(_UDWORD size, _UDWORD info)
{
  if(size != 0)
  {
    _XHEAPCELL *p = XHeapAlloc(size, info);
    if(p != NULL)
      return &p[1];
  }
  
  return NULL;
}

void *MyHeapFree(void *p)
{
  if(p != NULL)
  {
    _UBYTE *pt = (_UBYTE*)p;
    pt -= sizeof(_XHEAPCELL);
    XHeapFree((_XHEAPCELL*)pt);
  }
  
  return NULL;
}

void *MyHeapRealloc(void *p, _UDWORD size)
{
  if(p == NULL)
    return NULL;
    
  if(size == 0)
    return MyHeapFree(p);
  
  _UBYTE *pt = (_UBYTE*)p;
  pt -= sizeof(_XHEAPCELL);
  _XHEAPCELL *pnew = XHeapRealloc((_XHEAPCELL*)pt, size);
  if(pnew != NULL)
    return &pnew[1];
  return NULL;
}


_UDWORD ChkXHeapPtr(void *ptr)
{
  if (ptr == NULL) 
    return 1;
    
  if ((ptr >= XHeap.pstart) && (ptr < XHeap.pend))
    return 1;
    
    
   return 0; 
}


_UWORD Memory2(void **dest, _UDWORD size)
{
//  size = size & 0x0FFFFFFF;

  if(XHeap.Init != 0x4F5E6D7C)
    XHeapInit(20000000);

  if(ChkXHeapPtr(*dest) == 1)
  {
    if(size == 0)
    {
      if(*dest != NULL)
        *dest = MyHeapFree(*dest);
      return 1;
    }

    if(*dest == NULL)
      *dest = MyHeapAlloc(size, 0);
    else if(*dest != NULL)  
      *dest = MyHeapRealloc(*dest, size);

    if(*dest == NULL)  
    {
      // SetTooLessMemory();
      return(0);
    }

    return(1);
  }
}

#else


//_UWORD Memory2os(void **dest, _UDWORD size)
//{
//  if(size == 0)
//  {
//    if(*dest == NULL)
//    {
//      return(1); // schneller (böse)
//    }
//  }
//
//  if(RTOSVERSION >= 0x111E)   // nur wenn opsys >= 01.01.030
//    size |= (MemoryIdx.act << 28);
//
//  if(Memory(dest, size) == 0)
//  {
//    return(0);
//  }
//
//  return(1);
//}


static _BOOL GlobalTooLessMemory = _FALSE;
void SetTooLessMemory(void)
{
  GlobalTooLessMemory = _TRUE;
}
_BOOL GetTooLessMemory(void)
{
  return(GlobalTooLessMemory);
}


//void xLOG(void *p1, void *p2, _UDWORD size)
//{
//  _File file;
//  _ASCII tok[20];
//  _ASCII txt[256];
//
//  DToH(txt, (_UDWORD)p1, 0x108, 1);
//  StrCat(txt, 1, " : ", 1);
//  DToH(tok, (_UDWORD)p2, 0x108, 1);
//  StrCat(txt, 1, tok, 1);
//  StrCat(txt, 1, " : ", 1);
//  DToA(tok, size, 0x800, 1);
//  StrCat(txt, 1, tok, 1);
//
//  
//
//  file.Open(LOADSAVE, "C:\\LOG9.TXT");
//  file.Seek(0, SEEK_END);
//  file.Write(txt, StrLen(txt, 1));
//  tok[0] = 13;
//  tok[1] = 10;
//  file.Write(tok, 2);
//  file.Close();
//  
//}

_UWORD Memory2(void **dest, _UDWORD size)
{
//  _FUWORD_PT_UDWORD_UBYTE ptr = (_FUWORD_PT_UDWORD_UBYTE)lsl_pOS->piLSE->pMemoryV1;
//  return(lsl_pOS->piLSE->pMemoryV1(dest, size, 1));

//typedef _UWORD (*_FUWORD_PT_UDWORD_UBYTE) (void**, _UDWORD, _UBYTE);
//  _FUWORD_PT_UDWORD_UBYTE ptr = (_FUWORD_PT_UDWORD_UBYTE)*(_UDWORD*)(((_UBYTE*)(&lsl_pOS->piLSE->pOS_FindNextEx)) + 4);
//  return(ptr(dest, size, MemoryIdx));
  
  if(size == 0)
  {
   #ifdef _LSL_TARGETARCH_ARM
    if((void*)ReadU4((_UBYTE*)dest) == NULL)
   #else
    if(*dest == NULL)
   #endif
    {
      return(1); // schneller (böse)
    }
  }

  if(RTOSVERSION >= 0x111E)   // nur wenn opsys >= 01.01.030
    size |= (MemoryIdx.act << 28);

  if(Memory(dest, size) == 0)
  {
    SetTooLessMemory();
    return(0);
  }
  
  return(1);
}
#endif

_BOOL NewBubbleHelp_Call(_KOORD x, _KOORD y, void *text, _UWORD chrsize)
{
 #ifdef UC_BUBBLE_HELP
  return NewBubbleHelp.Call(x, y, text, chrsize);
 #else 
  return _FALSE;
 #endif
}

