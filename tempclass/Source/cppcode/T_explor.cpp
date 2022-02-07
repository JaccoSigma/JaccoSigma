#include "T_IPC.H"
#include "T_NUC.H"

extern void   out_runtext(_PopUp *PopUp, void *txt, _UWORD chrsize);
extern _ASCII ExplorerPath[_MAX_PATHLENGTH];


#define ID_ROOTLOCK 0x09 // id gesperrtes rootdirectory
#define ID_DRIVE    0x0A // id drive
#define ID_DIRECT   0x0B // id directory
#define ID_FILE     0x0C // id file
#define ID_EMPTY    0x0D // id leeres directory
#define ID_ERROR    0x0F // id error
#define EXATTRDLY   0x10 // attribut readonly
#define EXATTHIDDEN 0x20 // attribut hidden
#define EXATTSYSTEM 0x40 // attribut system
#define EXATTARCHIV 0x80 // attribut archiv

#define CUT_INFO        0xFF
#define CUT_INFO_DATE   0xFE
#define CUT_INFO_TIME   0xFD
#define CUT_INFO_SIZE   0xFC
#define CUT_INFO_ATTRIB 0xFB  // bei änderung/erweiterung muss die funktion StrCpyCutInfo() ebenfalls geändert werden

_ASCII filexplo_get_sign(_ASCII *txt)
{
  return (txt[0] & 0x0F);
}

_ASCII filexplo_get_sign_null(_ASCII *txt)
{
  if(txt != NULL)
    return filexplo_get_sign(txt);
  return ID_ERROR;  
}

void filexplo_set_sign(_ASCII *txt, _ASCII sign)
{
  // hibyte (attribute) wird mit absicht gelöscht
  txt[0] = sign;
}

void filexplo_set_attrib(_ASCII *txt, _ASCII att)
{
  txt[0] = filexplo_get_sign(txt) | (att & 0xF0);
}

_ASCII filexplo_get_attrib(_ASCII *txt)
{
  return (txt[0] & 0xF0);
}

_AsciiPuffer *filexplo_getList(_FILEXPLORER *pex, _UDWORD index)
{
  if(pex != NULL)
  {
    if(index < pex->NoList)
      return &pex->pList[index];
  }
  return NULL;
}

_AsciiPuffer *filexplo_addList(_FILEXPLORER *pex, _UDWORD *index)
{
  if(pex != NULL)
  {
    _AsciiPuffer *ph = pex->pList;
    for(_UDWORD i=0; i<pex->NoList; i++, ph++)
    {
      if(ph->GetNo() == 0)
      {
        *index = i;
        return ph;
      }
    }
    
    _UDWORD size = (pex->NoList + 1) * sizeof(_AsciiPuffer);
    if(Memory2((void**)&pex->pList, size) != 0)
    {
      ph = &pex->pList[pex->NoList];
      ph->Init();
      *index = pex->NoList;
      pex->NoList += 1;
      return ph;
    }
  }
  
  *index = DEF4BYTE;
  return NULL;
}

void filexplo_exclude_driveletter(_UBYTE *drvlst, _UWORD len, _ASCII *exc)
{
  _UWORD i, j, k;
  
  if(exc != NULL)
  {
    i = StrLen(exc, sizeof(_ASCII));
    
    for(j=0; j<len; j++)
    {
      for(k=0; k<i; k++)
      {
        if(drvlst[j] == exc[k])
        {
          drvlst[j] = 0;
        }
      }
    }
  }
}

void filexplo_make_volume(_ASCII *drive)
{
  _DDE_INFO dde;
  _ASCII    *pa, dpne[50], name[_MAX_PATHLENGTH];

  StrCpy(dpne, sizeof(_ASCII), drive, sizeof(_ASCII));
  AddTokenEnd(dpne);
  //StrCat(dpne, sizeof(_ASCII), "*.*", sizeof(_ASCII));
  StrCat(dpne, sizeof(_ASCII), "*", sizeof(_ASCII));

  name[0] = 0;
  _DWORD hand = FindFirstEx((char*)dpne, _DDE_VOLUME, 0, &dde, name, sizeof(name)-1);
  if(hand >= 0)
  {
    pa    = drive + StrLen(drive, sizeof(_ASCII));
    pa[0] = CUT_INFO;
    pa[1] = '(';
    StrCpy(&pa[2], sizeof(_ASCII), name, sizeof(_ASCII));
    StrCat(&pa[2], sizeof(_ASCII), ")", sizeof(_ASCII));
  }
  
  FindClose(hand);
}

_ASCII *filexplo_gettext(_FILEXPLORER *pex, _DWORD line, _TREEITEM **ppti)
{
  if(pex != NULL)
  {
    _TREEITEM *pti = get_TREEITEM(&pex->Tree, line);
    if(pti != NULL)
    {
      _AsciiPuffer *pls = filexplo_getList(pex, pti->data[1]);
      if(pls != NULL)
      {
        _ASCII *txt = pls->GetText(pti->data[0]);
        if(ppti != NULL)
        {
          if(txt != NULL)
            *ppti = pti;
        }
        return txt;
      }
    }
  }
  
  if(ppti != NULL)
    *ppti = NULL;
    
  return NULL;
}

void StrCpyCutInfo(_CHAR *dst, _UBYTE *src)
{
 #ifdef UC_UNICODE
  _CHAR chr;
 #else
  _UBYTE chr; // sa39011: muss unsigned 8Bit sein, nicht 'char' ansonsten funzt chr >= CUT_INFO_ATTRIB auf intel nicht 
 #endif
  
  do
  {
    chr    = *src++;
    if(chr >= CUT_INFO_ATTRIB)
    {
      chr = 0;
    }
    *dst++ = chr;
  }
  while(chr);
}

_DWORD OutColumnText(_KOORD x, _KOORD y, _CHAR *txt, _COLOR col, _ATTRIB att, _DWORD width)
{
  if(width > 0)
  {
    att   = (att & ~T_HORIZBOUND) | T_RIGHTBOUND;
    x    += width;
    width = 0;
  }
  else
  {
    width = StrWidth(txt, sizeof(_CHAR)) + ChrWidth(' ', sizeof(_CHAR));
  }
  
  OutTextXY(x, y, txt, att, col, sizeof(_CHAR));
  
  return x + width;
}

#define ICON16
#ifdef ICON16
static const _UBYTE icon_att_rdly[] = { 16, 16,
                                        0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0, 
                                        0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0, 
                                        0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0, 
                                        0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1, 
                                        0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1, 
                                        0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1, 
                                        0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1, 
                                        0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1, 
                                        0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0, 
                                        0,0,1,1,1,0,0,0,0,0,1,1,1,1,0,0, 
                                        0,1,1,1,0,1,1,1,0,0,1,1,1,0,0,0, 
                                        1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0, 
                                        1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0, 
                                        1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0, 
                                        1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0, 
                                        1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0};
#else
static const _UBYTE icon_att_rdly[] = { 8, 11,
                                        0,0,1,1,1,1,0,0,
                                        0,1,0,0,0,0,1,0,
                                        0,1,0,0,0,0,1,0,
                                        0,1,1,1,1,1,1,0,
                                        0,1,1,1,1,1,1,0,
                                        1,1,1,0,0,1,1,1,
                                        1,1,0,1,1,0,1,1,
                                        1,1,0,1,1,0,1,1,
                                        1,1,1,0,0,1,1,1,
                                        0,1,1,1,1,1,1,0,
                                        0,0,1,1,1,1,0,0};
                                        
                                        

#endif
//const _UBYTE icon_att_arch[] = { 6, 7,
//                                 0, 0, 1, 1, 0, 0, 
//                                 0, 0, 1, 1, 0, 0,
//                                 0, 1, 0, 0, 1, 0, 
//                                 0, 1, 0, 0, 1, 0, 
//                                 0, 1, 1, 1, 1, 0,
//                                 1, 0, 0, 0, 0, 1, 
//                                 1, 0, 0, 0, 0, 1, };

static const _UBYTE icon_att_hide[] = { 16, 16,
                                        0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                                        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                                        1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                                        1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
                                        1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,
                                        1,1,0,0,1,1,1,0,0,1,1,1,0,0,1,1,
                                        1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,
                                        1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,
                                        1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,
                                        1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,
                                        1,1,0,0,1,1,1,0,0,1,1,1,0,0,1,1,
                                        1,1,0,0,1,1,0,0,0,0,1,1,1,0,1,1,
                                        1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
                                        1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,
                                        0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,
                                        0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0};

                                      
static const _UBYTE icon_att_syst[] = { 16, 16,
                                        0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
                                        0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,
                                        0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0,
                                        0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
                                        0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,
                                        0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,
                                        1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,
                                        1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,
                                        0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,
                                        0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,
                                        0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
                                        0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0,
                                        0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,
                                        0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0};


static _KOORD show_icon_const(const _UBYTE *icon, _COLOR col, _KOORD x, _KOORD y)
{
  _KOORD x0 = x + 1;
  const _UBYTE *ph = &icon[2];
  
  _UDWORD yy = icon[1];
  
  y = y - (_KOORD)(yy / 2);
  
  while(yy--)
  {
    _UDWORD xx = icon[0];
    x = x0;
    while(xx--)
    {
      if(*ph++ != 0)
      {
        WritePixel(x, y, T_COPY|T_SOLID, col);
      }
      x++;
    }
    y++;
  }
  
  return icon[0] + 2;
}

static _KOORD show_icon(const _UBYTE *icon, _COLOR col, _UDWORD imageno, _KOORD x, _KOORD y)
{
  if(imageno == DEF4BYTE)
  {
    if(icon != NULL)
    {
      return show_icon_const(icon, col, x, y);
    }
  }
  else
  {
    _BMP *bmp = ImageList.Get(imageno);
    if(bmp != NULL)
    {
      Bitmap(x, y-bmp->height / 2, bmp, T_COPY|T_SOLID, 0);        
      return bmp->width + 2;
    }
  }
  
  return 0;  
}

_KOORD filexplo_line_show_attrib(_FILEXPLORER *pex, _KOORD x, _KOORD y, _ASCII *txt)
{
  _KOORD  retcode = 0;
  _UDWORD cut1 = StrChr(txt, CUT_INFO_ATTRIB, sizeof(_ASCII));
  
  if(cut1 < 0xFFFF)
  {
    _COLOR ic = pex->ColorImage;
   #ifdef UC_GRAYSCALE
    transform_color_to_gray((_UWORD*)&ic);
   #endif
    _CHAR tag[64];
    StrCpyCutInfo(tag, (_UBYTE*)&txt[cut1+1]);
    _CHAR *ph = tag;
    while(*ph != 0)
    {
      switch(*ph++)
      {
        case 'R':
          retcode += show_icon(icon_att_rdly, ic, pex->ImageAttReadOnly, x + retcode, y);
          break;
          
        case 'A':
          retcode += show_icon(NULL, ic, pex->ImageAttArchiv, x + retcode, y);
          break;
          
        case 'H':  
          retcode += show_icon(icon_att_hide, ic, pex->ImageAttHidden, x + retcode, y);
          break;
          
        case 'S':  
          retcode += show_icon(icon_att_syst, ic, pex->ImageAttSystem, x + retcode, y);
          break;
      }
    }
  }

  return retcode;
}

void filexplo_line_sub(_FILEXPLORER *pex, _UDWORD line, _ROOM *r, _COLOR col)
{
  _TREEITEM *pti;
  _ASCII    *txt = filexplo_gettext(pex, line, &pti);
  _CHAR     tag[64];
    
  if(txt != NULL)
  {
    _ASCII sign = filexplo_get_sign(txt);
    _KOORD y = (r->xy1.y + r->xy2.y) / 2;
    _KOORD x = out_TREE(&pex->Tree, r, line, col, ((sign == ID_DRIVE)||(sign == ID_DIRECT)||(sign == ID_ROOTLOCK))? _TRUE : _FALSE, pex->TreeSize);
   
    _UDWORD imageno = DEF4BYTE;
    switch(sign)
    {
      case ID_ROOTLOCK:
      case ID_DRIVE   : 
        imageno = pex->ImageDrive;
        break;
      case ID_DIRECT  :
        imageno = pex->ImageDirect; 
        break;
      case ID_FILE: 
        imageno = pex->ImageFile;
        break;
    }
 
    if(imageno != DEF4BYTE)
    {
      _BMP *bmp = ImageList.Get(imageno);
      if(bmp != NULL)
      {
        Bitmap(x, y-bmp->height / 2, bmp, T_COPY|T_SOLID, 0);        
         x += bmp->width + 2;
      }
    }
    else if((sign == ID_DIRECT)||(sign == ID_DRIVE)||(sign == ID_ROOTLOCK))
    {
      _COLOR ic = pex->ColorImage;
     #ifdef UC_GRAYSCALE
      transform_color_to_gray((_UWORD*)&ic);
     #endif
   
      _COLOR bc = GetBackColor_Makro(col);

      if((bc & 0xFFF0) == (ic & 0xFFF0))
        ic = LIGHTGRAY;

      Rectangle(x, y-5, x+11, y+4, T_SOLID|T_COPY, ic);
      Bar(x+2, y-3, x+12, y+6, T_SOLID|T_COPY, ic);
      if((sign == ID_DRIVE)||(sign == ID_ROOTLOCK))
        Bar(x+8, y-2, x+11, y-1, T_SOLID|T_COPY, bc);
        
      x += 15;
    }

    col = 0xFFFE0000 | (col & 0xFFFF);
    _ATTRIB att = T_COPY|T_SOLID|T_LEFTBOUND|T_MIDBOUND|T_PROP;

    if(pti->flags & TREE_MARKED)
    {
      col = MakeColor_Makro(INVISIBLE, pex->ColorMarked);
     #ifdef UC_GRAYSCALE
      transform_color_to_gray((_UWORD*)&col);
     #endif
      if(pex->BoldMarked == _TRUE)
        att |= T_BOLD;
    }
    
    if(sign == ID_ROOTLOCK)
    {
      OutTextXY(x, y, pex->LockedRootSign, att, col, sizeof(_ASCII));
    }
    else
    {
      _KOORD xx;
      _UDWORD cut1, cut = StrChr(txt, CUT_INFO, sizeof(_ASCII));

      if(pex->ShowAttribute == _TRUE) // sa38678
      {
        if(sign == ID_FILE)
        {
          x += filexplo_line_show_attrib(pex, x, y, txt);
        }
      }
      
      if(cut < 0xFFFF)
      {
        txt[cut] = 0; // ascii-0-string erzeugen ***********************
        // name ausgeben
        OutTextXY(x, y, &txt[1], att, col, sizeof(_ASCII));

        if(sign == ID_DRIVE)
        {
          // zusätzlich volumenamen ausgeben
          StrCpyCutInfo(tag, (_UBYTE*)&txt[cut+1]);
          xx = StrWidth(&txt[1], sizeof(_ASCII));
          OutTextXY(x+xx, y, tag, att, col, sizeof(_CHAR));
        }

        txt[cut] = CUT_INFO; // reparieren *****************************
        
        xx = (_KOORD)pex->xoffset + x;
        if(pex->WidthName != 0)
        {
          xx = r->xy1.x + pex->WidthName;
        }
        
        txt += cut + 1; // text pointer nach CUT_INFO stellen
        
        if(pex->ShowDate == _TRUE)
        {
          cut1 = StrChr(txt, CUT_INFO_DATE, sizeof(_ASCII));
          if(cut1 < 0xFFFF)
          {
            // zusätzlich datum ausgeben
            StrCpyCutInfo(tag, (_UBYTE*)&txt[cut1+1]);
            xx = OutColumnText(xx, y, tag, col, att, pex->WidthDate);
          }
        }
        
        if(pex->ShowTime == _TRUE)
        {
          cut1 = StrChr(txt, CUT_INFO_TIME, sizeof(_ASCII));
          if(cut1 < 0xFFFF)
          {
            // zusätzlich zeit ausgeben
            StrCpyCutInfo(tag, (_UBYTE*)&txt[cut1+1]);
            xx = OutColumnText(xx, y, tag, col, att, pex->WidthTime);
          }
        }
        
        if(pex->ShowSize == _TRUE)
        {
          cut1 = StrChr(txt, CUT_INFO_SIZE, sizeof(_ASCII));
          if(cut1 < 0xFFFF)
          {
            // zusätzlich size ausgeben
            StrCpyCutInfo(tag, (_UBYTE*)&txt[cut1+1]);
            xx = OutColumnText(xx, y, tag, col, att, pex->WidthSize);
          }
        }
      }
      else
      {
        OutTextXY(x, y, &txt[1], att, col, sizeof(_ASCII));
      }
    }
  }  
}

void filexplo_strcatcut(_ASCII *dst, _ASCII *src)
{
  _ASCII tmp[_MAX_PATHLENGTH];
  _UWORD cut;

  cut = StrChr(src, CUT_INFO, sizeof(_ASCII));
  if(cut != 0xFFFF)
  {
    StrCpy(tmp, sizeof(_ASCII), src, sizeof(_ASCII));
    tmp[cut] = 0;
    src = tmp;
  }

  StrCat(dst, sizeof(_ASCII), src, sizeof(_ASCII));
}

void make_scopepath(_ASCII *path, _FILEXPLORER *pex, _TREESCOPE *scope, _BOOL rootlock, _SELIS *pselis)
{
  path[0] = 0;
  *pselis = SELIS_NONE;

  for(_UDWORD i=0; i<scope->no; i++)
  {
    _TREEITEM *pti = scope->ptr[i];
    if(pti != NULL)
    {
      _AsciiPuffer *pls = filexplo_getList(pex, pti->data[1]);
      if(pls != NULL)
      {
        _ASCII *txt = pls->GetText(pti->data[0]);
        if(txt != NULL)
        {
          _UWORD sign = filexplo_get_sign(txt);
    
          if(sign == ID_ROOTLOCK)
          {
            *pselis = SELIS_ROOTLOCK;            
            StrCat(path, sizeof(_ASCII), (rootlock == _FALSE)? pex->LockedRootPath : pex->LockedRootSign, sizeof(_ASCII));
          }
          else if(sign != ID_EMPTY)
          {
            if(sign == ID_DRIVE)
              *pselis = SELIS_DRIVE;
            else if(sign == ID_DIRECT)
              *pselis = SELIS_DIRECTORY;
            else if(sign == ID_FILE)
              *pselis = SELIS_FILE;
          
            filexplo_strcatcut(path, &txt[1]);
          }

          if((sign != ID_FILE) && (sign != ID_EMPTY))
          {
            AddTokenEnd(path);
            //StrCat(path, sizeof(_ASCII), "\\", sizeof(_ASCII));
          }
        }
      }
    }
  }
}

_BOOL filexplo_make_path(_ASCII *path, _FILEXPLORER *pex, _DWORD line, _BOOL rootlock)
{
  _TREESCOPE scope;
  _SELIS     selis;

  path[0] = 0;

  if(get_TREESCOPE(&scope, &pex->Tree, line) == _TRUE)
  {
    make_scopepath(path, pex, &scope, rootlock, &selis);
    if(path[0] != 0)
      return _TRUE;
  }
  
  return _FALSE;
}

_UBYTE abctab[] = {
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,    1, 0x2F, // punkt ist auf 1 gesetzt, somit ist file.txt vor file 1.txt
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
0x40,  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
 'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z', 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
0x60,  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
 'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z', 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

_WORD filexplo_sort_AZ(_SORTA *p1, _SORTA *p2) // alphabetisch A-Z sortieren
{
  // liefert -1(a<b), 0(a=b) oder 1(a>b)

  _UBYTE *txt1 = ((_UBYTE*)p1->label) + 1;
  _UBYTE *txt2 = ((_UBYTE*)p2->label) + 1;

  while(1)
  {
    if(abctab[txt1[0]] < abctab[txt2[0]])
    {
      return -1;
    }
    else if(abctab[txt1[0]] > abctab[txt2[0]])
    {
      return 1;
    }
    else if(txt1[0] == 0)
    {
      return 0;
    }
    
    txt1 += 1;
    txt2 += 1;
  }
}

_WORD filexplo_sort_ZA(_SORTA *p1, _SORTA *p2) // alphabetisch Z-A sortieren
{
  // liefert -1(a<b), 0(a=b) oder 1(a>b)
  return filexplo_sort_AZ(p2, p1); // alphabetisch A-Z sortieren
}

_WORD filexplo_sort_Size_sub(_SORTA *p1, _SORTA *p2) // nach filegrösse aufsteigend sortieren
{
  _UBYTE *txt1 = ((_UBYTE*)p1->label) + 1;
  _UBYTE *txt2 = ((_UBYTE*)p2->label) + 1;
  _CHAR   tag1[32], tag2[32];
  _DWORD  size1, size2;
  _UDWORD pos1, pos2;

  pos1 = (StrChr(txt1, CUT_INFO_SIZE, sizeof(_ASCII)));
  pos2 = (StrChr(txt2, CUT_INFO_SIZE, sizeof(_ASCII)));
  
  if((pos1 < 0xFFFF) && (pos2 < 0xFFFF))
  {
    StrCpyCutInfo(tag1, &txt1[pos1+1]);
    StrCpyCutInfo(tag2, &txt2[pos2+1]);
    
    if((StrToNum(&size1, tag1, sizeof(_CHAR)) == _TRUE) && 
       (StrToNum(&size2, (_CHAR*)tag2, sizeof(_CHAR)) == _TRUE))
    {
      if(size1 != size2)
      {
        return (size1 < size2)? -1 : 1;
      }
    }
  }
  
  return 0; 
}

_WORD filexplo_sort_Size(_SORTA *p1, _SORTA *p2) // nach filegrösse aufsteigend sortieren
{
  _WORD retcode = filexplo_sort_Size_sub(p1, p2); 
  if(retcode == 0)
  {
    retcode = filexplo_sort_AZ(p1, p2);
  }
  return retcode;
}


_WORD filexplo_sort_Size_absteigend(_SORTA *p1, _SORTA *p2) // nach filegrösse absteigend sortieren
{
  _WORD retcode = filexplo_sort_Size_sub(p2, p1); 
  if(retcode == 0)
  {
    retcode = filexplo_sort_AZ(p1, p2);
  }
  return retcode;
}

static _WORD GlobalFileexplorerFormatDate;
static _WORD GlobalFileexplorerFormatTime;

_WORD filexplo_sort_DateTime_sub(_SORTA *p1, _SORTA *p2) // datum und uhrzeit aufsteigend sortieren
{
  _UBYTE *txt1 = ((_UBYTE*)p1->label) + 1;
  _UBYTE *txt2 = ((_UBYTE*)p2->label) + 1;
  _UDWORD pos1, pos2;
  _CHAR   tag1[64], tag2[64];
  _DATE   date1, date2;
  _TIME   time1, time2;

  pos1 = (StrChr(txt1, CUT_INFO_DATE, sizeof(_ASCII)));
  pos2 = (StrChr(txt2, CUT_INFO_DATE, sizeof(_ASCII)));
  
  if((pos1 < 0xFFFF) && (pos2 < 0xFFFF))
  {
    StrCpyCutInfo(tag1, &txt1[pos1+1]);
    StrCpyCutInfo(tag2, &txt2[pos2+1]);
    
    if((string_to_date(&date1, tag1, GlobalFileexplorerFormatDate) == _TRUE) && 
       (string_to_date(&date2, tag2, GlobalFileexplorerFormatDate) == _TRUE))
    {
      if(date1.year == date2.year) 
      {
        if(date1.month == date2.month) 
        {
          if(date1.day == date2.day) 
          {
            pos1 = (StrChr(txt1, CUT_INFO_TIME, sizeof(_ASCII)));
            pos2 = (StrChr(txt2, CUT_INFO_TIME, sizeof(_ASCII)));
  
            if((pos1 < 0xFFFF) && (pos2 < 0xFFFF))
            {
              StrCpyCutInfo(tag1, &txt1[pos1+1]);
              StrCpyCutInfo(tag2, &txt2[pos2+1]);
            
              if((string_to_time(&time1, tag1, GlobalFileexplorerFormatTime) == _TRUE) &&
                 (string_to_time(&time2, tag2, GlobalFileexplorerFormatTime) == _TRUE))
              {
                if(time1.hour == time2.hour)
                {

                  if(time1.minute == time2.minute)
                  {
                    if(time1.second != time2.second)
                    {
                      return (time1.second < time2.second)? -1 : 1;
                    }
                  }
                  else
                  {
                    return (time1.minute < time2.minute)? -1 : 1;
                  }
                }
                else
                {
                  return (time1.hour < time2.hour)? -1 : 1;
                }
              }
            }
          }
          else
          {
            return (date1.day < date2.day)? -1 : 1;
          }
        }
        else
        {
          return (date1.month < date2.month)? -1 : 1;
        }
      }
      else 
      {
        return (date1.year < date2.year)? -1 : 1;
      }
    }
  }
  
  return 0;
}

_WORD filexplo_sort_DateTime(_SORTA *p1, _SORTA *p2) // datum und uhrzeit aufsteigend sortieren
{
  _WORD retcode = filexplo_sort_DateTime_sub(p1, p2); 
  if(retcode == 0)
  {
    retcode = filexplo_sort_AZ(p1, p2);
  }
  return retcode;
}


_WORD filexplo_sort_DateTime_absteigend(_SORTA *p1, _SORTA *p2) // datum und uhrzeit absteigend sortieren
{
  _WORD retcode = filexplo_sort_DateTime_sub(p2, p1); 
  if(retcode == 0)
  {
    retcode = filexplo_sort_AZ(p1, p2);
  }
  return retcode;
}

_QSORTFUNCT filexplo_sort(_FILEXPLORER *pex)
{
  GlobalFileexplorerFormatDate = pex->FormatDate;
  GlobalFileexplorerFormatTime = pex->FormatTime;

  switch(pex->SortCriteria)
  {
    case 1: return (_QSORTFUNCT)filexplo_sort_ZA;
    case 2: return (_QSORTFUNCT)filexplo_sort_DateTime;
    case 3: return (_QSORTFUNCT)filexplo_sort_DateTime_absteigend;
    case 4: return (_QSORTFUNCT)filexplo_sort_Size;
    case 5: return (_QSORTFUNCT)filexplo_sort_Size_absteigend;
  }
  
  return (_QSORTFUNCT)filexplo_sort_AZ;
}

_UWORD filexplo_read_part(_FILEXPLORER *pex, _AsciiPuffer *pls, const _ASCII *dpne, _UDWORD inat, _UDWORD exat)
{
  _TIME     tim;
  _DATE     dte;
  _CHAR     *ph, info[60];
  _DDE_INFO dde;
  _DWORD    hand;
  _ASCII    *pa, name[_MAX_PATHLENGTH];
  _WORD     run = -1;
  _UDWORD   oldno = pls->GetNo();

  filexplo_set_sign(name, ID_FILE);
  //hand = FindFirstEx((char*)dpne, 0, _DDE_VOLUME, &dde, &name[1], sizeof(name)-2);
  hand = FindFirstEx((char*)dpne, inat, exat, &dde, &name[1], sizeof(name)-2);

  if(hand >= 0)
    run = 0;

  while(run >= 0)
  {
    if(name[1] != 0)
    {
      if(dde.Attributes & _DDE_DIR)
      {
        if(!((name[1] == '.')&&((name[2] == 0)||((name[2] == '.')&&(name[3] == 0)))))
        {
          filexplo_set_sign(name, ID_DIRECT);
          pls->Add(name);
        }
      }
      else
      {
        info[0]    = CUT_INFO;
        info[1]    = CUT_INFO_DATE; 
        dte.day    = dde.DateTime.Day;
        dte.month  = dde.DateTime.Month;
        dte.year   = dde.DateTime.Year + 1980;
        format_date(&info[2], &dte, pex->FormatDate);
        
        ph         = info + StrLen(info, sizeof(_CHAR));
        ph[0]      = CUT_INFO_TIME;
        tim.hour   = dde.DateTime.Hour;
        tim.minute = dde.DateTime.Minute;
        tim.second = dde.DateTime.Second;
        format_time(&ph[1], &tim, pex->FormatTime);
        
        filexplo_set_sign(name, ID_FILE);
        
        _ASCII att = 0;
        if(dde.Attributes & _DDE_RDLY)
          att |= EXATTRDLY;
        if(dde.Attributes & _DDE_HIDDEN)
          att |= EXATTHIDDEN;
        if(dde.Attributes & _DDE_SYSTEM)
          att |= EXATTSYSTEM;
        if(dde.Attributes & _DDE_ARCHIV)
          att |= EXATTARCHIV;
          
        filexplo_set_attrib(name, att);
        
        StrCat(name, sizeof(_ASCII), info, sizeof(_CHAR));

        pa    = name + StrLen(name, sizeof(_ASCII));
        pa[0] = CUT_INFO_SIZE;
        text_1000(&pa[1], dde.FileSize);
        
        if(pex->ShowAttribute == _TRUE)
        {
          pa    = &pa[StrLen(pa, sizeof(_ASCII))];
          pa[0] = CUT_INFO_ATTRIB;
          pa ++;
        
          if(dde.Attributes & _DDE_RDLY)
          {
            pa[0] = 'R'; pa ++;
          }
          if(dde.Attributes & _DDE_HIDDEN)
          {
            pa[0] = 'H'; pa ++;
          }
          if(dde.Attributes & _DDE_SYSTEM)
          {
            pa[0] = 'S'; pa ++;
          }
          if(dde.Attributes & _DDE_ARCHIV)
          {
            pa[0] = 'A'; pa ++;
          }
          
          pa[0] = 0;
        }
        
        pls->Add(name);
      }
    }

    run = FindNextEx(hand, &dde, &name[1], sizeof(name)-2);
  }

  FindClose(hand);

  if(pls->GetNo() != oldno)
  {
    sort_ASCII(pls->GetText(oldno), pls->GetNo()-oldno, filexplo_sort(pex));
  }

  return(pls->GetNo());
}

_BOOL add_emptytext(_AsciiPuffer *pls)
{
  _ASCII name[32];
  
  filexplo_set_sign(name, ID_EMPTY);
  StrCpy(&name[1], sizeof(_ASCII), "[empty]", sizeof(_ASCII));
  return pls->Add(name);
}

_UDWORD filexplo_read_sub(_FILEXPLORER *pex, _AsciiPuffer *pls, _ASCII *path)
{
  _ASCII tmp[_MAX_PATHLENGTH];

  _UWORD pos = StrLen(path, sizeof(_ASCII));
  MemCpy(tmp, path, pos);
  tmp[pos] = '*';
  tmp[pos + 1] = 0;
  
  _UDWORD retcode = filexplo_read_part(pex, pls, tmp, _DDE_DIR, _DDE_VOLUME);
  if(pex->FileFilter != NULL)
    StrCpy(&tmp[pos], sizeof(_ASCII), pex->FileFilter, sizeof(_ASCII));
  retcode += filexplo_read_part(pex, pls, tmp, 0, _DDE_DIR|_DDE_VOLUME);
  
  return retcode;
}

_TREE *filexplo_read(_FILEXPLORER *pex, _ASCII *path)
{
  _TREE *tree = NULL;
  if(Memory2((void**)&tree, sizeof(_TREE)) != 0)
  {
    init_TREE(tree);
    
    _UDWORD ils;
    _AsciiPuffer *pls = filexplo_addList(pex, &ils);
    if(pls != NULL)
    {
//      _UWORD pos = StrLen(path, sizeof(_ASCII));
//      _UDWORD start = pls->GetNo();
//      //StrCpy(&path[pos], sizeof(_ASCII), "*.*", sizeof(_ASCII));
//      StrCpy(&path[pos], sizeof(_ASCII), "*", sizeof(_ASCII));
//      filexplo_read_part(pex, pls, path, _DDE_DIR, _DDE_VOLUME);
//      if(pex->FileFilter != NULL)
//        StrCpy(&path[pos], sizeof(_ASCII), pex->FileFilter, sizeof(_ASCII));
//      filexplo_read_part(pex, pls, path, 0, _DDE_DIR|_DDE_VOLUME);
//      _UDWORD end = pls->GetNo();

      _UDWORD start = pls->GetNo();
      filexplo_read_sub(pex, pls, path);
      _UDWORD end = pls->GetNo();
  
      if(start == end)
      {
        if(add_emptytext(pls) == _TRUE)
          end = start + 1;
      }
  
      for(_UDWORD i=start; i<end; i++)
        add_TREEITEM(tree, i, ils);
    }
  }
  
  return tree;
}

void filexplo_correctlineno(_FILEXPLORER *pex, _POSITION *ppos)
{
  markedlines_TREE(&pex->Tree);
  ppos->no = visuallines_TREE(&pex->Tree);
  
  if(ppos->pos >= ppos->no)
  {
    if(ppos->no > 0)
      ppos->pos = ppos->no - 1;
    else
      ppos->pos = 0;
      
    ppos->oldpos = ppos->pos;
  }
}

_BOOL filexplo_enter_sub(_FILEXPLORER *pex, _DWORD line) //_POSITION *ppos)
{
  _ASCII sign, path[_MAX_PATHLENGTH];
  _BOOL  retcode = _FALSE;

  if(pex != NULL)
  {
    _TREEITEM *pti;
    _ASCII    *txt = filexplo_gettext(pex, line, &pti);
    if(txt != NULL)
    {
      sign = filexplo_get_sign(txt);
    
      if((sign == ID_DRIVE)||(sign == ID_DIRECT)||(sign == ID_ROOTLOCK))
      {
        if(is_action_TREEITEM(pti) == _FALSE)
        {
          path[0] = 0;
          filexplo_make_path(path, pex, line, _FALSE);
          //StrCat(path, sizeof(_ASCII), (pex->FileFilter == NULL)? "*.*" : pex->FileFilter, sizeof(_ASCII));
          add_SUBTREE(pti, filexplo_read(pex, path));
        }
          
        if(do_action_TREEITEM(pti) == _TRUE)
        {
          retcode = _TRUE;
        }
      }
    }
  }
  
  return(retcode);
}

_BOOL filexplo_open(_Window *Window, _PopUp *PopUp, _FILEXPLORER *pex, _DWORD line, _EVENT *pev)
{
  _ASCII *txt = filexplo_gettext(pex, line, NULL);

  if(txt != NULL)
  {
    if(filexplo_get_sign(txt) == ID_FILE)
    {
      _ASCII path[_MAX_PATHLENGTH];
      path[0] = 0;
      filexplo_make_path(path, pex, line, _FALSE);
   
      _FILEXT filext   = GetFiletype(path);
      _FILEXT filetype = Textfile_FILEXT(filext);
      _BOOL   st = _FALSE;

      out_runtext(PopUp, (void*)"load file...", sizeof(_ASCII));
      MakeRamFile(ExplorerPath, path);

      if((filetype == FILEXT_TXT)||((filetype == FILEXT_BMO)&&(filext != FILEXT_GIF))) // sorry, gif kann nit geladen werden
      {
        if((filext != FILEXT_JPG)||(JpgFunctLoad != NULL))
        {
          Window->AddCmd(CMD_NEWPOPUP, (filetype == FILEXT_TXT)? UC_TEXTFILE_POPUP:UC_IMAGEFILE_DIAG, 0, pev->lrm);
          st = _TRUE;
        }
      }
        
      if(st == _FALSE)
        Window->AddCmd(CMD_NEWPOPUP, UC_BINFILE_POPUP, 0, pev->lrm);
        
      return _TRUE;
    }
  }
  return _FALSE;
}

_BOOL filexplo_xctrlc_sub(_TREE *p)
{
  _BOOL retcode = _FALSE;

  if(p != NULL)
  {
    _TREEITEM *pti = p->ptr;
    for(_UDWORD i=0; i<p->no; i++, pti++)
    {
      if(pti->flags & TREE_MARKED)
      {
        reset_flag(pti, TREE_MARKED);
        retcode = _TRUE;
      }  
    
      if(pti->psubtree != NULL)
      {
        if(filexplo_xctrlc_sub(pti->psubtree) == _TRUE)
          retcode = _TRUE;
      }
    }
  }
  
  return retcode;
}

_BOOL delete_subtree(_FILEXPLORER *pex, _TREE *p)
{
  _BOOL retcode = _FALSE;
  
  if((pex != NULL) && (p != NULL))
  {
    _TREEITEM *pti = p->ptr;
    for(_UDWORD i=0; i<p->no; i++)
    {
      if(pti->psubtree != NULL)
      {
        if(delete_subtree(pex, pti->psubtree) == _TRUE)
          retcode = _TRUE;
        pti->psubtree = NULL;
      }
    }
    
    pti = p->ptr;
    if(pti != NULL)
    {
      _AsciiPuffer *pls = filexplo_getList(pex, pti->data[1]);
      if(pls != NULL)
        pls->Free();
      free_TREE(p);
      Memory((void**)&p, 0);
      retcode = _TRUE;
    }
  }
  
  return retcode;
}


_BOOL filexplo_delete_sub(_FILEXPLORER *pex, _DWORD line, _BOOL delete_if_node)
{
  if(pex != NULL)
  {
    _TREEITEM *pti = get_TREEITEM(&pex->Tree, line);
    if(pti != NULL)
    {
      if(pti->psubtree != NULL)
      {
        if(delete_subtree(pex, pti->psubtree) == _TRUE)
        {
          //Memory((void**)&pti->psubtree, 0);
          pti->psubtree = NULL;
          pti->subtreestate = TREE_CLOSED;
          
          if(delete_if_node == _TRUE)
            filexplo_delete_sub(pex, line, _FALSE);
          
          return _TRUE;
        }
      }
      else
      {
//        es bleibt der text im _AsciiPuffer bestehen, sollte eigentlich gelöscht werden. 
//        is aber nit möglich weil sonst alle nachfolgenden index falsch sind also sollte er durch einen leertext ersetzt werden
//        _AsciiPuffer *pls = filexplo_getList(pex, ils);
//        if(pls != NULL)
//        {
//          pls->Rename()
//        }
        
        _UDWORD ils = pti->data[1];
        _AsciiPuffer *pls = filexplo_getList(pex, ils);
        if(pls != NULL)
        {
          _ASCII *txt = pls->GetText(pti->data[0]);
          if(txt != NULL)
          {
            _ASCII sign = filexplo_get_sign(txt);
            if((sign == ID_DIRECT) || (sign == ID_FILE)) // nur file und directory löschen
            {
              _TREE *pt = del_TREEITEM(&pex->Tree, line);
              if(pt != NULL)
              {
                // es wurde im tree 'pt' etwas gelöscht
                if(pt->no == 0)
                {
                  // wenn der tree leer ist muss die textliste geleert werden und es wird der empty-text eingefügt
                  pls->Free();
                  if(add_emptytext(pls) == _TRUE)
                    add_TREEITEM(pt, 0, ils);
                }
          
                return _TRUE;
              }
            } 
          }
        }
      }
    }
  }
  
  return _FALSE;
}




_BOOL filexplo_delete_file(_ASCII *dpne)
{
  _Handle Hand;
  if(Hand.Remove(dpne) == _TRUE)
  {
    return _TRUE;
  }
  
  return _FALSE;
}

_UWORD filexplo_read_prt(_AsciiPuffer *pls, const _ASCII *dpne, _UDWORD inat, _UDWORD exat)
{
  _DDE_INFO dde;
  _DWORD    hand;
  _ASCII    name[_MAX_PATHLENGTH];
  _WORD     run = -1;

  name[0] = 0;
  hand = FindFirstEx((char*)dpne, inat, exat, &dde, name, sizeof(name)-1);

  if(hand >= 0)
    run = 0;

  while(run >= 0)
  {
    if(name[0] != 0)
    {
      if(dde.Attributes & _DDE_DIR)
      {
        _BOOL add = _TRUE;
        if(name[0] == '.')
        {
            if(name[1] == 0) // "."
            {
              add = _FALSE;
            }
            else if((name[1] == '.') && (name[2] == 0)) // ".."
            {
              add = _FALSE;
            }
        }
        
        if(add == _TRUE)
        {
          pls->Add(name);
        }
      }
      else
      {
        pls->Add(name);
      }
    }

    run = FindNextEx(hand, &dde, name, sizeof(name)-1);
  }

  FindClose(hand);

  return(pls->GetNo());
}

_BOOL filexplo_delete_directory(_ASCII *dp)
{
  _UDWORD i, no;
  _BOOL   retcode = _TRUE;
  _ASCII  work[_MAX_PATHLENGTH], tmp[_MAX_PATHLENGTH];
    
  StrCpy(work, sizeof(_ASCII), dp, sizeof(_ASCII));
  AddTokenEnd(work);
  StrCpy(tmp, sizeof(_ASCII), dp, sizeof(_ASCII));
  _ASCII *ph = &tmp[StrLen(tmp, sizeof(_ASCII))]; 
  StrCat(work, sizeof(_ASCII), "*", sizeof(_ASCII));

  _AsciiPuffer List;
    
  List.Init();
    
  if(filexplo_read_prt(&List, work, _DDE_DIR, _DDE_VOLUME) != 0)
  {
    no = List.GetNo();
    for(i=0; i<no; i++)
    {
      StrCpy(ph, sizeof(_ASCII), List.GetText(i), sizeof(_ASCII));
      AddTokenEnd(ph);
      if(filexplo_delete_directory(tmp) == _FALSE)
        retcode = _FALSE;
    }
  }

  List.Free();

  if(filexplo_read_prt(&List, work, 0, _DDE_DIR|_DDE_VOLUME) != 0)
  {
    no = List.GetNo();
    for(i=0; i<no; i++)
    {
      StrCpy(ph, sizeof(_ASCII), List.GetText(i), sizeof(_ASCII));
      if(filexplo_delete_file(tmp) == _FALSE)
        retcode = _FALSE;
    }
  }
    
  List.Free();
    
  if(retcode == _TRUE)
  {
    StrCpy(work, sizeof(_ASCII), dp, sizeof(_ASCII));
    DelTokenEnd(work);
    lsl_pOS->piFILE->pSFR_RemoveDir(work);
  }
 
  return retcode;
}

_BOOL filexplo_delete(_FILEXPLORER *pex, _DWORD line, _BOOL ismarked)
{
  _TREEITEM *pti;
  _ASCII    *txt = filexplo_gettext(pex, line, &pti);

  if(txt != NULL)
  {
    if((pti->flags & TREE_MARKED)||(ismarked == _FALSE))
    {
      _ASCII path[_MAX_PATHLENGTH];
      path[0] = 0;
      filexplo_make_path(path, pex, line, _FALSE);
      
      if(filexplo_get_sign(txt) == ID_FILE)
        return filexplo_delete_file(path);
      
      if(filexplo_get_sign(txt) == ID_DIRECT)
        return filexplo_delete_directory(path);
    }
  }
  
  return _FALSE;
}

void filexplo_outpage(_POSITION *ppos)
{
  ppos->oldbegin = ppos->begin + 2001;
}

typedef struct
{
  _SELIS selis;
  _ASCII dpne[_MAX_PATHLENGTH];
}
_EXPLOSCAN;

typedef struct
{
  _UDWORD     no;
  _EXPLOSCAN  *ptr;
}
_MEM_EXPLOSCAN;

void init_EXPLOSCAN(_EXPLOSCAN *p)
{
  p->selis   = SELIS_NONE;
  p->dpne[0] = 0;
}

void free_EXPLOSCAN(_EXPLOSCAN *p)
{
  init_EXPLOSCAN(p);
}

void init_MEM_EXPLOSCAN(_MEM_EXPLOSCAN *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_EXPLOSCAN(_MEM_EXPLOSCAN *p)
{
  for(_UDWORD i=0; i<p->no; i++)
    free_EXPLOSCAN(&p->ptr[i]);
  Memory2((void**)&p->ptr, 0);

  init_MEM_EXPLOSCAN(p);
}

_BOOL add_MEM_EXPLOSCAN(_MEM_EXPLOSCAN *p, _ASCII *dpne, _SELIS selis)
{
  if(Memory2((void**)&p->ptr, (p->no + 1) * sizeof(_EXPLOSCAN)) != 0)
  {
    _EXPLOSCAN *ph = &p->ptr[p->no];
    init_EXPLOSCAN(ph);
    StrCpy(ph->dpne, sizeof(_ASCII), dpne, sizeof(_ASCII));
    ph->selis = selis;
    p->no ++;
    return _TRUE;
  }
  
  return _FALSE;
}

extern "C" _BOOL scan_found(_TREESCOPE *scope, _FILEXPLORER *pex, _MEM_EXPLOSCAN *mes)
{
  _ASCII path[_MAX_PATHLENGTH];
  _SELIS selis;
  
  make_scopepath(path, pex, scope, _FALSE, &selis);
  
  if(selis != SELIS_NONE)
    return add_MEM_EXPLOSCAN(mes, path, selis);
    
  return _TRUE;
}

_BOOL filexplo_scanmarked_sub(_FILEXPLORER *pex, _FBOOL_PT_PT_PT_SELIS_UDWORD pfkt, void *puser1, void *puser2)
{
  _MEM_EXPLOSCAN mes;
  
  init_MEM_EXPLOSCAN(&mes);

  if(scan_TREE(&pex->Tree, TREE_MARKED, (_FBOOL_PT_PT_PT)scan_found, pex, &mes) == _TRUE)
  {
    _EXPLOSCAN *ph = mes.ptr;
    _UDWORD anz = mes.no;
    while(anz)
    {
      anz--;
      pfkt(puser1, puser2, ph->dpne, ph->selis, anz);
      ph++;
    }
    
    return _TRUE;
  }
  
  free_MEM_EXPLOSCAN(&mes);
  
  return _FALSE;
}


extern "C" _BOOL chk_dircopy(_ASCII *dstpath, _ASCII *srcpath)
{
  _ASCII d[_MAX_PATHLENGTH], s[_MAX_PATHLENGTH];
  
  StrCpy(s, sizeof(_ASCII), srcpath, sizeof(_ASCII));
  StrCpy(d, sizeof(_ASCII), dstpath, sizeof(_ASCII));
  AddTokenEnd(s);
  AddTokenEnd(d);
  StrUp(d, sizeof(_ASCII), 0);
  StrUp(s, sizeof(_ASCII), 0);
  
  _UWORD sl = StrLen(s, sizeof(_ASCII));
  _UWORD dl = StrLen(d, sizeof(_ASCII));

  if(s[0] == d[0])
  {
    if(dl >= sl)
    {
      if(MemCmp(d, s, sl) == 0)
        return _FALSE; // dst darf nicht gleich oder subdir von src sein
    }
    else
    {
      DelTokenEnd(s);
      StrCutLastTokenAndReturn(s, s);
      AddTokenEnd(s);
      if(StrXmp(d, s, sizeof(_ASCII)) == 0)
        return _FALSE; // dst darf nicht gleich src sein
    }
  }
  return _TRUE;
}

_BOOL chk_filecopy(_ASCII *dstpath, _ASCII *srcfile)
{
  _ASCII d[_MAX_PATHLENGTH], s[_MAX_PATHLENGTH];
  
  StrCutLastTokenAndReturn(s, srcfile);
  StrCpy(d, sizeof(_ASCII), dstpath, sizeof(_ASCII));
  AddTokenEnd(d);
  
  StrUp(d, sizeof(_ASCII), 0);
  StrUp(s, sizeof(_ASCII), 0);
  
  if(StrXmp(d, s, sizeof(_ASCII)) == 0)
    return _FALSE; // dst darf nicht gleich src sein
  
  return _TRUE;
}



_BOOL directory_copy_sub(_ASCII *dst, _ASCII *src, _ASCII *filefilter)
{
  _BOOL   retcode = _TRUE;
  _UDWORD i, no;
  _ASCII  tmp[_MAX_PATHLENGTH];
  _ASCII  work[_MAX_PATHLENGTH];
  _UWORD  len = StrLen(dst, sizeof(_ASCII));
  
  MemCpy(work, dst, len);
  work[len] = 0;
  _ASCII *ph = &work[len];

  _ASCII seek[_MAX_PATHLENGTH];
  StrCpy(seek, sizeof(_ASCII), src, sizeof(_ASCII));
  AddTokenEnd(seek);
  StrCat(seek, sizeof(_ASCII), "*", sizeof(_ASCII));

  _AsciiPuffer List;
  List.Init();
    
  if(filexplo_read_prt(&List, seek, _DDE_DIR, _DDE_VOLUME) != 0)
  {
    no = List.GetNo();
    for(i=0; i<no; i++)
    {
      StrCpy(ph, sizeof(_ASCII), List.GetText(i), sizeof(_ASCII));
      AddTokenEnd(ph);
      
      if(CheckCreateDirectory(work) == _TRUE)
      {
        StrCpy(tmp, sizeof(_ASCII), src, sizeof(_ASCII));
        StrCat(tmp, sizeof(_ASCII), List.GetText(i), sizeof(_ASCII));
        AddTokenEnd(tmp);
        if(directory_copy_sub(work, tmp, filefilter) == _FALSE)
          retcode = _FALSE;
      }
      else
      {
        retcode = _FALSE;
      }
    }
  }

  List.Free();

  if(filefilter != NULL)
  {
    if(filefilter[0] != 0)
    {
      DelTokenEnd(seek);
      StrCutLastTokenAndReturn(seek, seek);
      AddTokenEnd(seek);
      StrCat(seek, sizeof(_ASCII), filefilter, sizeof(_ASCII));
    }
  }
  
  if(filexplo_read_prt(&List, seek, 0, _DDE_DIR|_DDE_VOLUME) != 0)
  {
    StrCpy(seek, sizeof(_ASCII), src, sizeof(_ASCII));
    AddTokenEnd(seek);
    len = StrLen(seek, sizeof(_ASCII));
    _ASCII *ps = &seek[len];
    
    no = List.GetNo();
    for(i=0; i<no; i++)
    {
      StrCpy(ph, sizeof(_ASCII), List.GetText(i), sizeof(_ASCII));
      StrCpy(ps, sizeof(_ASCII), List.GetText(i), sizeof(_ASCII));
      
      if(Lse_File_Copy(work, seek) == _FALSE)
        retcode = _FALSE;
    }
  }

  List.Free();
  
  return retcode;
}

_BOOL directory_copy(_ASCII *dst, _ASCII *src, _ASCII *filefilter)
{
  _ASCII tmp[_MAX_PATHLENGTH];
  
  StrCpy(tmp, sizeof(_ASCII), src, sizeof(_ASCII));
  DelTokenEnd(tmp);
  
  _ASCII work[_MAX_PATHLENGTH];
  StrCutLastTokenAndReturn(work, tmp);
  _UWORD len = StrLen(work, sizeof(_ASCII));
  
  StrCpy(work, sizeof(_ASCII), dst, sizeof(_ASCII));
  StrCat(work, sizeof(_ASCII), &tmp[len], sizeof(_ASCII));
  AddTokenEnd(work);
  
  if(CheckCreateDirectory(work) == _TRUE)
  {
    return directory_copy_sub(work, src, filefilter);
  }

  return _FALSE;
}


extern "C" _BOOL file_dir_copy(void *puser, _ASCII *dst, _ASCII *src, _SELIS selis, _UDWORD index)
{
  _ASCII tmp[_MAX_PATHLENGTH];
  
  if(selis == SELIS_FILE)
  {
    _ASCII *name = StrCutLastTokenAndReturn(tmp, src);
    if(name != NULL)
    {
      if(name[0] != 0)
      {
        StrUp(tmp, sizeof(_ASCII), 0);
    
        if(StrXmp(tmp, dst, sizeof(_ASCII)) != 0)
        {
          StrCpy(tmp, sizeof(_ASCII), dst, sizeof(_ASCII));
          StrCat(tmp, sizeof(_ASCII), name, sizeof(_ASCII));
          return Lse_File_Copy(tmp, src);
        }
      }
    }
  }
  else if(selis == SELIS_DRIVE)
  {
    if(chk_dircopy(dst, src) == _TRUE)
    {
      return directory_copy(dst, src, NULL);
    }
  }
  
  return _TRUE;
}


_BOOL filexplo_can(_FILEXPLORER *pex, _DWORD line, _TREEST tst)
{
  if(pex != NULL)
  {
    _TREEITEM *pti;
    _ASCII    *txt = filexplo_gettext(pex, line, &pti);

    if(txt != NULL)
    {
      if(pti->subtreestate == tst)
      {
        if((txt[0] == ID_ROOTLOCK) || (txt[0] == ID_DRIVE) || (txt[0] == ID_DIRECT))
          return _TRUE;
      }
    }
  }
  
  return _FALSE;
}

// ***************************

extern "C" _BOOL filexplo_scanmarked(_FILEXPLORER *pex, _FBOOL_PT_PT_PT_SELIS_UDWORD pfkt, void *puser1, void *puser2)
{
  if(pex != NULL)
    return filexplo_scanmarked_sub(pex, pfkt, puser1, puser2);

  return _FALSE;
}

extern "C" _UDWORD filexplo_getattrib(_FILEXPLORER *pex, _DWORD pos)
{
  _UDWORD retcode = 0;

  if(pex != NULL)
  {
    _ASCII *txt = filexplo_gettext(pex, pos, NULL);
    if(txt != NULL)
    {
      _ASCII att = filexplo_get_attrib(txt);
      if(att & EXATTRDLY)
        retcode |= _DDE_RDLY;
      if(att & EXATTHIDDEN)
        retcode |= _DDE_HIDDEN;
      if(att & EXATTSYSTEM)
        retcode |= _DDE_SYSTEM;
      if(att & EXATTARCHIV)
        retcode |= _DDE_ARCHIV;
        
        
      switch(filexplo_get_sign(txt))
      {
        case ID_ROOTLOCK :
        case ID_DRIVE    : retcode |= _DDE_VOLUME;
                           break;
        case ID_DIRECT   : retcode |= _DDE_DIR;
                           break;
      }
    }
  }
  return retcode;
}

extern "C" _BOOL filexplo_update(_FILEXPLORER *pex, _POSITION *ppos)
{
  if(pex != NULL)
  {
    _DWORD treeline = -1;
    _TREEITEM *pti = get_TREEITEM(&pex->Tree, ppos->pos);
    if(pti != NULL)
    {
      if(pti->subtreestate == TREE_OPEND)
      {
        // aktuelle line steht auf einem geöffneten knoten
        treeline = ppos->pos;
      }
      else
      {
        _ASCII *txt = filexplo_gettext(pex, ppos->pos, NULL);
        if(txt != NULL)
        {
          _ASCII sign = filexplo_get_sign(txt);
          if((sign == ID_ROOTLOCK)||(sign == ID_DRIVE))
          {
            // aktuelle line ist ein drive
            treeline = ppos->pos;
          }
        }
      }
    }
  
    if(treeline < 0) 
    {
      // suche line von übergeordnetem treeknoten
      treeline = find_SCOPELINE(&pex->Tree, ppos->pos);
    } 
    
    if(treeline >= 0)
    {
      filexplo_delete_sub(pex, treeline, _FALSE);
      filexplo_enter_sub(pex, treeline);
      filexplo_correctlineno(pex, ppos);
      return _TRUE;
    }
  }
  
  return _FALSE;
}

extern "C" _BOOL filexplo_rename(_FILEXPLORER *pex, _ASCII *oldname, _ASCII *newname, _DWORD linepos)
{
  if(pex != NULL)
  {
    if(Lse_File_Rename(oldname, newname) == _TRUE)
    {
      _TREEITEM *pti = get_TREEITEM(&pex->Tree, linepos);
      if(pti != NULL)
      {
        _AsciiPuffer *pls = filexplo_getList(pex, pti->data[1]);
        if(pls != NULL)
        {
          _ASCII *txt = pls->GetText(pti->data[0]);
        
          if(txt != NULL)
          {
            _ASCII tmp[100];
            tmp[0] = txt[0];
            
            _ASCII *name = GetLastToken(newname);
            
            if(name != NULL)
            {
              StrCpy(&tmp[1], sizeof(_ASCII), name, sizeof(_ASCII));
          
              _UWORD cut = StrChr(txt, CUT_INFO, sizeof(_ASCII));
              if(cut < StrLen(txt, sizeof(_ASCII)))
                StrCat(&tmp[1], sizeof(_ASCII), &txt[cut], sizeof(_ASCII));
        
              if(StrLen(tmp, sizeof(_ASCII)) == StrLen(txt, sizeof(_ASCII)))
              {
                StrCpy(txt, sizeof(_ASCII), tmp, sizeof(_ASCII));
                return _TRUE;
              }  
              else
              {
                _UDWORD idx = pls->GetNo();
                if(pls->Add(tmp) == _TRUE)
                {
                  pti->data[0] = idx;
                  return _TRUE;
                }
              }
            }
          }
        }
      }
    }
  }
 
  return _FALSE;
}

extern "C" _BOOL filexplo_xctrlc(_FILEXPLORER *pex)
{
  if(pex != NULL)
  {
    if(filexplo_xctrlc_sub(&pex->Tree) == _TRUE)
    {
      markedlines_TREE(&pex->Tree);
      return _TRUE;
    }
  }
  
  return _TRUE;
}

extern "C" _BOOL file_dir_copy_chk(void *puser, _ASCII *dst, _ASCII *src, _SELIS selis, _UDWORD index)
{
  if(selis == SELIS_FILE)
  {
    if(chk_filecopy(dst, src) == _FALSE)
      *(_BOOL*)puser = _FALSE;
  }
  else if(selis == SELIS_DIRECTORY)
  {
    if(chk_dircopy(dst, src) == _FALSE)
      *(_BOOL*)puser = _FALSE;
  }
  return _TRUE;
}

extern "C" _BOOL filexplo_file_dir_copy_check(_FILEXPLORER *pex, _ASCII *dst)
{
  if((pex != NULL) && (dst != NULL))
  {
    _BOOL  retcode = _TRUE;
    _ASCII path[_MAX_PATHLENGTH];
    StrCutLastTokenAndReturn(path, dst);
    
    if(filexplo_scanmarked(pex, (_FBOOL_PT_PT_PT_SELIS_UDWORD)file_dir_copy_chk, &retcode, path) == _TRUE)
    {
      return retcode;
    }
  }
  
  return _FALSE;
}

extern "C" _BOOL filexplo_ctrlv(_FILEXPLORER *pex, _ASCII *dst)
{
  if((pex != NULL) && (dst != NULL))
  {
    _ASCII path[_MAX_PATHLENGTH];
    StrCutLastTokenAndReturn(path, dst);
    StrUp(path, sizeof(_ASCII), 0);
    
    if(filexplo_scanmarked(pex, (_FBOOL_PT_PT_PT_SELIS_UDWORD)file_dir_copy, NULL, path) == _TRUE)
    {
      return _TRUE;
    }
  }
  
  return _FALSE;
}

_BOOL filexplo_ctrlv_intern(_FILEXPLORER *pex, _POSITION *ppos)
{
  _ASCII path[_MAX_PATHLENGTH];
  if(filexplo_make_path(path, pex, ppos->pos, _FALSE) == _TRUE)
  {
    if(filexplo_ctrlv(pex, path) == _TRUE)
    {
      filexplo_xctrlc(pex);
      filexplo_update(pex, ppos);
      return _TRUE;
    }
  }
  
  return _FALSE;
}

extern "C" _BOOL filexplo_ctrlc(_FILEXPLORER *pex, _DWORD line)
{
  if(pex != NULL)
  {
    _TREEITEM *pti;
    _ASCII    *txt = filexplo_gettext(pex, line, &pti);
    if(txt != NULL)
    {
      _ASCII sign = filexplo_get_sign(txt);
      if((sign == ID_FILE) || (sign == ID_DIRECT))
      {
        if(pti->flags & TREE_MARKED)
          reset_flag(pti, TREE_MARKED);
        else
          set_flag(pti, TREE_MARKED);
          
        markedlines_TREE(&pex->Tree); 
          
        return _TRUE;
      }
    }
  }
  return _FALSE;
}

extern "C" _BOOL CheckCreateDirectory(_ASCII *drivepath)
{
  if(drivepath != NULL)
  {
    _ASCII part[_MAX_PATHLENGTH];
    _ASCII tmp[_MAX_PATHLENGTH];
    
    StrCpy(tmp, sizeof(_ASCII), drivepath, sizeof(_ASCII));
    DelTokenEnd(tmp);
  
    _UWORD len = StrLen(tmp, sizeof(_ASCII));
  
    if(len > 2)
    {
      StrCat(tmp, sizeof(_ASCII), "\\*", sizeof(_ASCII));
      _DDE_INFO dde;
    
      part[0] = 0;
      _DWORD hand = FindFirstEx((char*)tmp, 0, 0, &dde, part, sizeof(part)-1);
      _BOOL state = (hand >= 0)? _TRUE : _FALSE;
      FindClose(hand);

      if(state == _TRUE)
      {
        return _TRUE;
      }
      else
      {
        tmp[len] = 0;
        StrCutLastTokenAndReturn(part, tmp);
        if(CheckCreateDirectory(part) == _TRUE)
        {
          CreateDir(tmp);
          return _TRUE;
        }
      }
    }
    else
    {
      _UWORD no = GetDriveList(part);
      StrUp(part, sizeof(_ASCII), no);
      StrUp(tmp, sizeof(_ASCII), 0);
      while(no)
      {
        no --;
        if(part[no] == tmp[0])
          return _TRUE;
      }
    }
  }
  
  return _FALSE;
}

extern "C" _DWORD filexplo_drivelist(_FILEXPLORER *pex, _BOOL initstate)
{
  _DWORD retcode = 0;
  _ASCII dvlst[41];

  if(pex->LasalOsCnt != Keyboard.LasalOsCnt)
    initstate = _TRUE;
  
  if(initstate == _TRUE)
  {
    dvlst[0] = GetDriveList(&dvlst[1]);
    retcode = MemCmp(pex->DriveList, dvlst, dvlst[0] + 1);
  
    MemSet(pex->DriveList, 0, sizeof(pex->DriveList));
    MemCpy(pex->DriveList, dvlst, dvlst[0] + 1);
    pex->LasalOsCnt = Keyboard.LasalOsCnt;
  }
 
  return retcode;
}

extern "C" _FILEXPLORER* filexplo_start(_POSITION *ppos, _ASCII *rootpath, _ASCII *exclude_drives)
{
  _ASCII       dpne[_MAX_PATHLENGTH];
  _UWORD       i;
  _FILEXPLORER *pex = NULL;

  if(Memory((void**)&pex, sizeof(_FILEXPLORER)) != 0)
  {
    init_FILEXPLORER(pex);
    filexplo_drivelist(pex, _TRUE);
    
    if(exclude_drives != NULL)
    {
      StrCpy(pex->ExcludeDrives, sizeof(_ASCII), exclude_drives, sizeof(_ASCII));
    }
    
    if(rootpath != NULL)
    {
      // rootpath ist für anwender gesperrt
      i = StrLen(rootpath, sizeof(_ASCII));
      if(i > 0)
      {
        if(rootpath[i-1] == '\\')
          i -= 1;
        
        MemCpy(pex->LockedRootPath, rootpath, i);
        pex->LockedRootPath[i] = 0;
        
        if(CheckCreateDirectory(pex->LockedRootPath) == _FALSE)
          pex->LockedRootPath[0] = 0;
      }
    }
   
    filexplo_exclude_driveletter(&pex->DriveList[1], pex->DriveList[0], exclude_drives); 

    _UDWORD ils;
    _AsciiPuffer *pls = filexplo_addList(pex, &ils);
   
    if(pls != NULL)
    {
      for(i=1; i<=pex->DriveList[0]; i++)
      {
        if(pex->DriveList[i] != 0)
        {
          filexplo_set_sign(dpne, ID_DRIVE);
          dpne[1] = pex->DriveList[i];
          dpne[2] = ':';
          dpne[3] = 0;
       
          if(dpne[1] == pex->LockedRootPath[0])
          {
            filexplo_set_sign(dpne, ID_ROOTLOCK);
            StrCpy(&dpne[1], sizeof(_ASCII), "User:", sizeof(_ASCII));
          }
          else
          {
            filexplo_make_volume(&dpne[1]);
          }
        
          add_TREEITEM(&pex->Tree, pls->GetNo(), ils);
          pls->Add(dpne);
        }
      }
    }
    else
    {
      free_FILEXPLORER(pex);
      Memory2((void**)&pex, 0);
      return NULL;
    }
    
    ppos->no = visuallines_TREE(&pex->Tree);
    ppos->pos = 0;
    ppos->begin = 0;
    ppos->oldpos = 0;
    ppos->oldbegin = 0;
  }
  
  return(pex);
}

extern "C" _FILEXPLORER *filexplo_end(_FILEXPLORER *pex)
{
  if(pex != NULL)
  {
    free_FILEXPLORER(pex);
    Memory((void**)&pex, 0);
  }
  
  return NULL;
}

extern "C" void filexplo_line(_FILEXPLORER *pex, _DWORD line, _ROOM *r, _COLOR col)
{
  if(pex != NULL)
    filexplo_line_sub(pex, line, r, col);
}

extern "C" _BOOL filexplo_enter(_FILEXPLORER *pex, _POSITION *ppos)
{
  if(pex != NULL)
  {
    if(filexplo_enter_sub(pex, ppos->pos) == _TRUE)
    {
      filexplo_correctlineno(pex, ppos);
      return _TRUE;
    }    
  }
  return _FALSE;
}

extern "C" _ASCII *filexplo_path(_FILEXPLORER *pex, _SELIS *pselis, _DWORD line, _BOOL rootlock)
{
  if(pex != NULL)
  {
    pex->ActPath[0] = 0;
    filexplo_make_path(pex->ActPath, pex, line, rootlock);
    if(pselis != NULL)
    {
      _ASCII *txt = filexplo_gettext(pex, line, NULL);
      _ASCII sign = ID_EMPTY;
      if(txt != NULL)
        sign = filexplo_get_sign(txt);

      switch(sign)
      {
        case ID_FILE     : *pselis = SELIS_FILE; break;
        case ID_DIRECT   : *pselis = SELIS_DIRECTORY; break;
        case ID_ROOTLOCK : *pselis = SELIS_ROOTLOCK; break;
        case ID_DRIVE    : *pselis = SELIS_DRIVE; break;
        default          : *pselis = SELIS_NONE; break;
      }
    }
    
//    if((pisfile != NULL) || (pisdir != NULL))
//    {
//      _ASCII *txt = filexplo_gettext(pex, line, NULL);
//      _ASCII sign = ID_EMPTY;
//      if(txt != NULL)
//        sign = filexplo_get_sign(txt);
//      
//      if(pisfile != NULL)
//        *pisfile = (sign == ID_FILE)? _TRUE : _FALSE;
//        
//      if(pisdir != NULL)
//        *pisdir = (sign == ID_DIRECT)? _TRUE : _FALSE;
//      
//    }
    return pex->ActPath;
  }
  
  return NULL;
}

extern "C" _BOOL filexplo_delete_treeentry(_FILEXPLORER *pex, _POSITION *ppos)
{
  if(pex != NULL)
  {
    filexplo_delete_sub(pex, ppos->pos, _TRUE);
    filexplo_correctlineno(pex, ppos);
    return _TRUE;
  }    
  
  return _FALSE;
}

extern "C" _BOOL filexplo_delete(_FILEXPLORER *pex, _POSITION *ppos)
{
  if(pex != NULL)
  {
    if(filexplo_delete(pex, ppos->pos, _FALSE) == _TRUE)
      return filexplo_delete_treeentry(pex, ppos);
  }    
  
  return _FALSE;
}

extern "C" _BOOL filexplo_is_marked(_FILEXPLORER *pex, _DWORD line)
{
  if(pex != NULL)
  {
    _TREEITEM *pti = get_TREEITEM(&pex->Tree, line);
    if(pti != NULL)
    {
      if(pti->flags & TREE_MARKED)
      {
        return _TRUE;
      }
    }
  }
  
  return _FALSE;
}

extern "C" _BOOL filexplo_can_open(_FILEXPLORER *pex, _DWORD line)
{
  return filexplo_can(pex, line, TREE_CLOSED);
}

extern "C" _BOOL filexplo_can_close(_FILEXPLORER *pex, _DWORD line)
{
  return filexplo_can(pex, line, TREE_OPEND);
}

extern "C" void filexplo_make_user_path(_ASCII *dst, _FILEXPLORER *pex, _ASCII *src)
{
  _ASCII txt[_MAX_PATHLENGTH];

  StrCpy(dst, sizeof(_ASCII), src, sizeof(_ASCII));
  
  if(pex != NULL)
  {
    _UWORD len = StrLen(pex->LockedRootPath, sizeof(char));
    if(len > 0)
    {
      MemCpy(txt, pex->LockedRootPath, len);
    
      StrUp(txt, len, sizeof(_ASCII));
      StrUp(src, len, sizeof(_ASCII));
      
      if(MemCmp(txt, src, len) == 0)
      {
        StrCpy(dst, sizeof(_ASCII), pex->LockedRootSign, sizeof(_ASCII));
        StrCat(dst, sizeof(_ASCII), &src[len], sizeof(_ASCII));
      }
    }
  }
}

//extern "C" _BOOL filexplo_del_directory(_FILEXPLORER *pex, _ASCII *dp)
//{
//  if(pex != NULL)
//  {
//    return filexplo_delete_directory(dp);
//  }
//  
//  return _TRUE;
//}


// DIAG **************************

void popup_filexplo_end(_Window *Pw, _PopUp *PopUp)
{
  filexplo_end((_FILEXPLORER*)PopUp->GetMenu()->pointer.PtrData);
  PopUp->GetMenu()->pointer.PtrData = NULL;
}

void popup_filexplo_line(_Window *Pw, _PopUp *Pp, _UDWORD line, _ROOM *r, _COLOR col, _BOOL background)
{
  _FILEXPLORER *pex = (_FILEXPLORER*)Pp->GetMenu()->pointer.PtrData;

  if(background == _TRUE)
    Bar(r->xy1.x, r->xy1.y, r->xy2.x, r->xy2.y, T_COPY|T_SOLID, InvertColor_Makro(col));

  filexplo_line_sub(pex, line, r, col);
}

void popup_filexplo_run(_Window *Window, _PopUp *PopUp, _EVENT *pev)
{
  _MENU        *pm = PopUp->GetMenu();
  _FILEXPLORER *pex = (_FILEXPLORER*)pm->pointer.PtrData;

  if(pev->ftype == _EVENT_KEYPRESS)
  {
    if(pev->scancode == _ENTER)
    {
      if(filexplo_enter_sub(pex, pm->position.pos) == _TRUE)
      {
        filexplo_correctlineno(pex, &pm->position);
        filexplo_outpage(&pm->position);
      }
      else
      {
        filexplo_open(Window, PopUp, pex, pm->position.pos, pev);
      }
    }
    else if(pev->scancode == _DEL)
    {
      if(filexplo_delete(pex, pm->position.pos, _TRUE) == _TRUE)
      {
        filexplo_delete_sub(pex, pm->position.pos, _TRUE);
        filexplo_correctlineno(pex, &pm->position);
        filexplo_outpage(&pm->position);
      }
    }
    else if(IsCtrlKeyPress(pev, 'c') == _TRUE) // CTRL-C
    {
      if(filexplo_ctrlc(pex, pm->position.pos) == _TRUE)
        filexplo_outpage(&pm->position);
      
    }
    else if(IsCtrlKeyPress(pev, 'v') == _TRUE) // CTRL-V
    {
      filexplo_ctrlv_intern(pex, &pm->position);
      filexplo_outpage(&pm->position);
    }
  }
}

extern void diagnostic_call(_MENU *Menu, _UWORD bigline, _BOOL basement);

_BOOL filexplo_call(_MENU *Menu)
{
  _FILEXPLORER *pex = filexplo_start(&Menu->position, NULL, NULL);

  if(pex != NULL)
  {
    diagnostic_call(Menu, 1, _FALSE);
    Menu->pointer.PtrLine = popup_filexplo_line;    // --> to linedraw function
    Menu->pointer.PtrRun  = popup_filexplo_run;          // --> to run function
    Menu->pointer.PtrEnd  = popup_filexplo_end;          // --> to end function
    Menu->pointer.PtrData = pex;
    Menu->pointer.Level   = _TRUE;
    
    return(_TRUE);
  }

  return(_FALSE);
}


// **********************************
/*
_ASCII labeltokendummy[2];
_ASCII *get_labeltoken(_FILEXPLORER *pex, _TREEITEM *pti)
{
  _AsciiPuffer *pls = filexplo_getList(pex, pti->data[1]);
  if(pls != NULL)
  {
    return pls->GetText(pti->data[0]);
  }
  labeltokendummy[0] = ID_EMPTY;
  labeltokendummy[1] = 0;
  return labeltokendummy;
}


_UDWORD fileexplo_GetAllOpenNodes(_AsciiPuffer *On, _FILEXPLORER *pex, _ASCII *dp, _TREE *root)
{
  _UDWORD retcode = 0;
  _ASCII tmpnam[500];
  
  if(root != NULL)
  {
    _TREEITEM *pti = root->ptr;
    for(_UDWORD i=0; i<root->no; i++, pti++)
    {
      if(pti->psubtree != NULL)
      {
        retcode += 1;
        if(pti->subtreestate == TREE_OPEND)
        {
          _ASCII *label = get_labeltoken(pex, pti);
          
          tmpnam[0] = 0;
          if(dp != NULL)
          {
            StrCpy(tmpnam, sizeof(_ASCII), dp, sizeof(_ASCII));
            StrCat(tmpnam, sizeof(_ASCII), "\\", sizeof(_ASCII));
          }
          StrCat(tmpnam, sizeof(_ASCII), label, sizeof(_ASCII));
          On->Add(tmpnam);
          
          // geöffneten node updaten
          retcode += fileexplo_GetAllOpenNodes(On, pex, tmpnam, pti->psubtree);
        }
      }
    }
  }
  
  return retcode;
}
*/


void fileexplo_DeleteTreeTextList(_FILEXPLORER *pex, _TREE *root)
{
  if(root != NULL)
  {
    _TREEITEM *pti = root->ptr;
    for(_UDWORD i=0; i<root->no; i++, pti++)
    {
      _AsciiPuffer *pls = filexplo_getList(pex, pti->data[1]);      
      if(pls != NULL)
        pls->Free();
      
      if(pti->psubtree != NULL)
      {
        fileexplo_DeleteTreeTextList(pex, pti->psubtree);
      }
    }
  }
}

_BOOL add_pathtoken(_ASCII *path, _FILEXPLORER *pex, _TREEITEM *pti)
{
  _AsciiPuffer *pls = filexplo_getList(pex, pti->data[1]);
  
  if(pls != NULL)
  {
    _ASCII *token = pls->GetText(pti->data[0]);
    if(token != NULL)
    {
      switch(filexplo_get_sign(token))
      {
        case ID_ROOTLOCK : StrCat(path, sizeof(_ASCII), pex->LockedRootPath, sizeof(_ASCII)); // id gesperrtes rootdirectory
                           return _TRUE;
        case ID_DRIVE    : 
        case ID_DIRECT   : filexplo_strcatcut(path, &token[1]);
                           return _TRUE;
      }
    }
  }
  return _FALSE;
}

void free_subtree(_FILEXPLORER *pex, _TREEITEM *pti)
{
  fileexplo_DeleteTreeTextList(pex, pti->psubtree);
  free_TREE(pti->psubtree);
  Memory2((void**)&pti->psubtree, 0);
  pti->subtreestate = TREE_CLOSED;
}

_UDWORD check_differences(_FILEXPLORER *pex, _TREEITEM *pti, _AsciiPuffer *NewList)
{
  _ASCII *pt1, *pt2;
  if(pti != NULL)
  {
    _AsciiPuffer *pls = filexplo_getList(pex, pti->data[1]);

    if(pls != NULL)
    {
      _UDWORD oldno = pls->GetNo();
      _UDWORD newno = NewList->GetNo(); 

      if(oldno == newno)
      {
        for(_UDWORD i=0; i<oldno; i++)
        {
          pt1 = pls->GetText(i);
          pt2 = NewList->GetText(i);
          
          if((pt1 != NULL) && (pt2 != NULL))
          {
            if(StrCmp(pt1, sizeof(_ASCII), pt2, sizeof(_ASCII)) != 0)
              return 1;
          }
          else
          {
            return 1; // sollte nicht sein
          }
        }
        
        return 0; // alles gleich
      }
      else if((oldno == 1) && (newno == 0))
      {
        // überprüfen ob node "empty" ist
        if(filexplo_get_sign_null(pls->GetText(0)) == ID_EMPTY)
          return 0; 
      }
    }
  }
  return 1; // ungleich
}

_BOOL fileexplo_update(_FILEXPLORER *pex, _TREE *root, _ASCII *dp)
{
  _BOOL  retcode = _FALSE;
  _ASCII tmpnam[_MAX_PATHLENGTH];
  
  if(root != NULL)
  {
    _TREEITEM *pti = root->ptr;
    for(_UDWORD i=0; i<root->no; i++, pti++)
    {
      if(pti->psubtree != NULL)
      {
        if(pti->subtreestate == TREE_OPEND)
        {
          tmpnam[0] = 0;
          if(dp != NULL)
            StrCpy(tmpnam, sizeof(_ASCII), dp, sizeof(_ASCII));
        
          if(add_pathtoken(tmpnam, pex, pti) == _TRUE)
          {
            AddTokenEnd(tmpnam);
            // read directory of open node
            _AsciiPuffer NewList;
            NewList.Init();
            filexplo_read_sub(pex, &NewList, tmpnam);
            
            // geöffneten node auf änderungen überprüfen
            if(check_differences(pex, pti->psubtree->ptr, &NewList) != 0)
            {
              // änderung festgestellt --> node freigeben und schliessen
              free_subtree(pex, pti);
              
              // node neu einlesen und öffnen
              pti->psubtree = filexplo_read(pex, tmpnam);
              if(pti->psubtree != NULL)
              {
                pti->subtreestate = TREE_OPEND;
              }
              
              retcode = _TRUE; // änderungsmerker setzen
            }
            
            NewList.Free();
          }
          else
          {
            // sollte nicht sein --> node frigeben und schliessen
            free_subtree(pex, pti);
          }
          
          if(fileexplo_update(pex, pti->psubtree, tmpnam) == _TRUE)
          {
            retcode = _TRUE; // änderungsmerker setzen
          }
        }
        else
        {
          // geschlossenen node freigeben
          free_subtree(pex, pti);
        }
      }
    }
  }

  return retcode;
}

extern "C" _FILEXPLORER* filexplo_updatetree(_FILEXPLORER *pex, _POSITION *ppos)
{
  if(pex != NULL)
  {
    if(fileexplo_update(pex, &pex->Tree, NULL) == _TRUE)
    {
      // änderungen festgestellt
      filexplo_correctlineno(pex, ppos);
    }
  }

  return pex;
}

_UDWORD fex_start_end(_AsciiPuffer *pls, _UDWORD *pstart, _UDWORD *panz)
{
  _UDWORD end = pls->GetNo();
  _UDWORD i, anz = 0;
  
  for(i=0; i<end; i++)
  {
    if(filexplo_get_sign_null(pls->GetText(i)) == ID_FILE)
    {
      if(anz == 0)
      {
        *pstart = i;
      }
      anz++;
    }
  }
  
  *panz = anz;
  
  return anz;
}

extern "C" _BOOL filexplo_sort(_FILEXPLORER *pex, _UDWORD criteria)
{
  _BOOL retcode = _FALSE;
  _UDWORD i;
  
  if(pex != NULL)
  {
    if(pex->SortCriteria != criteria)
    {
      pex->SortCriteria = criteria;
    
      for(i=0; i<pex->NoList; i++)
      {
        _AsciiPuffer *pls = &pex->pList[i];
        _UDWORD start, anzahl; 
        if(fex_start_end(pls, &start, &anzahl) > 0)
        {
          sort_ASCII(pls->GetText(start), anzahl, filexplo_sort(pex));
          retcode = _TRUE;
        }
      }  
    }
  }
  
  return retcode;
}
