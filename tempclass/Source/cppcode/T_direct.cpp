#include "T_IPC.H"
#include "T_NUC.H"

#ifdef UC_DIRECTORY

#define PATH_NONE (_ASCII*)"[no entry]"
#define PATH_DRVE (_ASCII*)"[ :]"




_BOOL read_part(_AsciiPuffer *dest, _ASCII *dp, _ASCII *ne, _ASCII *preelect, _BOOL directory)
{
  _DDE_INFO dde;
  _DWORD    hand;
  _ASCII    name[50];
  _WORD     run = -1;
  _BOOL     st;
 #ifdef UC_LONGFILENAMES
  _ASCII    search[50];
 #else
  _UWORD    i;
  _UWORD    len;
 #endif


  dest->Free();

 #ifdef UC_LONGFILENAMES
  StrCpy(search, sizeof(_ASCII), dp, sizeof(_ASCII));
  StrCat(search, sizeof(_ASCII), ne, sizeof(_ASCII));
  hand = FindFirstEx(search, 0, 0, &dde, name, sizeof(name));

  if(hand >= 0)
    run = 0;

  while(run >= 0)
  {
    if(name[0] != 0)
    {

      if(!((name[0] == '.')&&(name[1] == 0)))
      {
        if(dde.Attributes & _DDE_DIR)
        {
          if(directory == _TRUE)
          {
            st = _TRUE;
            if(StrCmp(name, sizeof(_ASCII), "..", sizeof(_ASCII)) == 0)
              if(StrCmp(dp, sizeof(_ASCII), preelect, sizeof(_ASCII)) == 0)
                st = _FALSE;

            if(st == _TRUE)
              dest->Add(name);
          }
        }
        else
        {
          if(directory == _FALSE)
            dest->Add(name);
        }
      }
    }

    run = FindNextEx(hand, &dde, name, sizeof(name));
  }

 #else

  hand = FindFirst(dp, ne, &dde, 0, 0);

  if(hand >= 0)
    run = 0;

  while(run >= 0)
  {
    if(dde.FileName[0] != 0)
    {
      MemCpy(name, dde.FileName, 8);
      name[8] = 0;

      for(i=0; i<8; i++)
      {
        if(name[i] == ' ')
        {
          name[i] = 0;
          break;
        }
      }

      if(!((name[0] == '.')&&(name[1] == 0)))
      {
        if((dde.Extension[0] != 0)&&(dde.Extension[0] != ' '))
        {
          StrCat(name, sizeof(_ASCII), ".", sizeof(_ASCII));
          len = StrLen(name, sizeof(_ASCII));
          MemCpy(&name[len], dde.Extension, 3);
          name[len+3] = 0;
        }

        if(dde.Attributes & _DDE_DIR)
        {
          if(directory == _TRUE)
          {
            st = _TRUE;
            if(StrCmp(name, sizeof(_ASCII), "..", sizeof(_ASCII)) == 0)
              if(StrCmp(dp, sizeof(_ASCII), preelect, sizeof(_ASCII)) == 0)
                st = _FALSE;

            if(st == _TRUE)
              dest->Add(name);
          }
        }
        else
        {
          if(directory == _FALSE)
            dest->Add(name);
        }
      }
    }

    run = FindNext(hand, &dde);
  }
 #endif

  FindClose(hand);

  sort_ASCII(dest->GetText(0), dest->GetNo(), NULL);

  return(_TRUE);
}

void get_drive_list(_ASCII *drvlst, _DIRECTORY *pdir)
{
 #ifndef CUTOFF_USER
  _FVOID_PT_PT ptr = (_FVOID_PT_PT)find_lasal_method(LseThisPointer, METHOD_LSE_CONFIGDRIVELIST);

  ptr(drvlst, pdir->data);
#endif
}

_BOOL read_entrys(_AsciiPuffer *label, _AsciiPuffer *dir, _ASCII *dp, _ASCII *ne, _DIRECTORY *pdir)
{
  _ASCII *pdl, name[5], drvlst[30];
  _ASCII *preelect = pdir->preelect;

  read_part(dir, dp, (_ASCII*)"*.*", preelect, _TRUE);
  read_part(label, dp, ne, preelect, _FALSE);

  drvlst[0] = 0;
  if(preelect[0] == 0)
  {
//    dir->Add("[A:]");
//    dir->Add("[C:]");
//    dir->Add("[D:]");
    drvlst[0] = 'A';
    drvlst[1] = 'B';
    drvlst[2] = 'C';
    drvlst[3] = 0;
  }

  get_drive_list(drvlst, pdir);
  StrUp(drvlst, sizeof(_ASCII), StrLen(drvlst, sizeof(_ASCII)));
  name[0] = '[';
  name[1] = ' ';
  name[2] = ':';
  name[3] = ']';
  name[4] = 0;
  pdl = drvlst;
  while(*pdl)
  {
    name[1] = *pdl;
    dir->Add(name);
    pdl += 1;
  }

  if(dir->GetNo() == 0)
    dir->Add(PATH_NONE);
  if(label->GetNo() == 0)
    label->Add(PATH_NONE);

  return(_TRUE);
}

void _Directory::Init(void)
{
  init_DIRECTORY(&Dir);
  AddDPNE(Dir.path, (_ASCII*)"C:");
  StrCpy(Dir.name, sizeof(_ASCII), "*.*", sizeof(_ASCII));

  StrCpy(Dir.chredit.fstring, sizeof(_CHAR), Dir.name, sizeof(_ASCII));
  Dir.chredit.max = 8;
}

void _Directory::Free(void)
{
  free_DIRECTORY(&Dir);
}

void _Directory::Setup(_FILE_STATE state, _ASCII *ne, _FBOOL_PT_PT fkt, void *data, void *pthis, _DIRECTORYSETUP *pds)
{
  _UWORD pos, len, i;
  _ASCII *pne, *pa;

  free_DIRECTORYSETUP(&Dir.dirset);
  if(pds != NULL)
    Dir.dirset = *pds;

  free_CHREDIT(&Dir.chredit);
  Dir.chredit.max = 8;

  if(ne != NULL)
  {
    pos = 0;
    len = StrLen(ne, sizeof(_ASCII));
    pa  =
    pne = ne;
    for(i=0; i<len; i++)
    {
      if(*pa == 92)
      {
        pne = pa+1;
        pos = i+1;
      }
      pa++;
    }

    StrCpy(Dir.preelect, sizeof(_ASCII), ne, sizeof(_ASCII));
    Dir.preelect[pos] = 0;

    if(Dir.preelect[0] != 0)
      StrCpy(Dir.path, sizeof(_ASCII), Dir.preelect, sizeof(_ASCII));

    StrCpy(Dir.chredit.fstring, sizeof(_CHAR), pne, sizeof(_ASCII));

// neu
    StrCpy(Dir.name, sizeof(_ASCII), pne, sizeof(_ASCII));

//    pos = StrChr(ne, '.', sizeof(_ASCII));
//    if(pos != 0xFFFF)
//    {
//      Dir.name[0] = '*';
//      StrCpy(&Dir.name[1], sizeof(_ASCII), &ne[pos], sizeof(_ASCII));
//    }
// end
  }

  Dir.activ = 2;
  if(state == SAVE)
  {
    Dir.activ = 0;
  }
  Dir.fkt   = fkt;
  Dir.data  = data;
  Dir.pthis = pthis;
}

void _Directory::AddDPNE(_ASCII *dp, _ASCII *dir)
{
  StrCat(dp, sizeof(_ASCII), dir, sizeof(_ASCII));
  StrCat(dp, sizeof(_ASCII), "\\", sizeof(_ASCII));
}

void _Directory::SubDPNE(_ASCII *dpne)
{
  _UWORD len = StrLen(dpne, sizeof(_ASCII));
  _UWORD i, o=0, a=0;

  for(i=0; i<len; i++)
  {
    if(dpne[i] == 92)
    {
      o = a;
      a = i;
    }
  }

  dpne[o+1] = 0;
}

_BOOL make_dpne(_ASCII *dpne, _ASCII *dp, _CHAR *n, _ASCII *e)
{
  _UWORD i = 0;
  _BOOL  run = _TRUE;
  _ASCII chr;

  StrCpy(dpne, sizeof(_ASCII), dp, sizeof(_ASCII));
  StrCat(dpne, sizeof(_ASCII), n, sizeof(_CHAR));
  if(StrChr(n, '.', sizeof(_CHAR)) == 0xFFFF)
  {
    do
    {
      chr = e[i];

      if(chr == '.')
      {
        if(e[i+1] != '*')
          StrCat(dpne, sizeof(_ASCII), &e[i], sizeof(_ASCII));
        run = _FALSE;
      }
      else if(chr == 0)
      {
        run = _FALSE;
      }

      i++;
    }
    while(run == _TRUE);
  }

  StrUp(dpne, sizeof(_ASCII), 0);

  return(_TRUE);
}

void directory_lineout(_SCROLL *p, _AsciiPuffer *Pa, _ROOM *pr, _UWORD no, _BOOL st)
{
  _ATTRIB attrib = (st == _TRUE)? p->attrib_activ : p->attrib_inactiv;
  _COLOR  coltxt = (st == _TRUE)? p->color_activ : p->color_inactiv;
  _COLOR  colback, colframe;

  if(Pa != NULL)
  {
    if(no < Pa->GetNo())
    {
      coltxt   = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(coltxt));
      colback  = MakeColor_Makro(GetBackColor_Makro(coltxt), GetBackColor_Makro(coltxt));
      colframe = MakeColor_Makro(INVISIBLE, INVISIBLE);
			OutTextAlign(Pa->GetText(no), 0, pr, p->font, attrib, 0, coltxt, colback, colframe, sizeof(_ASCII));
		}
  }
}

void right_size(_KOORD *lineheight, _KOORD *linewidth, _UWORD *lineno, _ROOM *pr)
{
  _KOORD hh = pr->xy2.y - pr->xy1.y;
  _KOORD ww = pr->xy2.x - pr->xy1.x;

  if((*lineheight * (*lineno)) > hh)
    *lineno = (hh/(*lineheight));

  if(((*linewidth + LEVELBUTTON_WIDTH + 3) * 2) > ww)
    *linewidth = (ww / 2) - LEVELBUTTON_WIDTH - 6;
}

_BOOL _Directory::Start(_DIRECTORY *pt, _BOOL firsttime, _ROOM *pr)
{
  _BOOL   retcode = _FALSE;
  _SCROLL *ps;
  _UWORD  xx, yy, lineno;
  _KOORD  x, y, lineheight, linewidth;
  _ROOM   room;
  _COLOR  tcol;

  if(firsttime == _TRUE)
  {
    if(read_entrys(&pt->Files.Label, &pt->Direct.Label, pt->path, pt->name, &Dir) == _TRUE)
	  {
      Font.Size(Dir.dirset.font, &xx, &yy);
      lineheight = Dir.dirset.lineheight; //(yy < 22)? 22 : yy;
      linewidth  = Dir.dirset.linewidth; //(lineheight <= 22)? 150 : lineheight * 6;
      lineno     = Dir.dirset.lineonpage; //10
      if(pr == NULL)
      {
        MyGetScreenSize(&room);
        room.xy1.x = 0; //220;
        room.xy1.y = 160;
      }
      else
      {
        room = *pr;
        room.xy1.x += 10 + Dir.dirset.offset.x;
        room.xy1.y += 15 + Dir.dirset.offset.y + lineheight;
        room.xy2.x -= 10;
        room.xy2.y -= 15 + lineheight;
      }

      right_size(&lineheight, &linewidth, &lineno, &room);

		  x = room.xy1.x; // 220
		  y = room.xy1.y; // 160;

		  ps                    = &pt->Files.scroll; //List.GetScroll();
		  init_POSITION(&ps->position);

		  ps->font              = Dir.dirset.font; //DEFFONT;
		  ps->attrib_inactiv    = T_COPY|T_SOLID|T_LEFTBOUND|T_MIDBOUND|T_PROP|T_BOLD;
		  ps->attrib_activ      = T_COPY|T_SOLID|T_LEFTBOUND|T_MIDBOUND|T_PROP|T_BOLD;
		  ps->line_height       = lineheight;
		  ps->line_width        = linewidth;
		  ps->position.height   = lineno;                   // no of lines/page
		  ps->scroll_room.xy1.x = x;
		  ps->scroll_room.xy1.y = y;
		  ps->scroll_room.xy2.x = x + ps->line_width;
		  ps->scroll_room.xy2.y = y + ps->line_height * ps->position.height;
		  ps->color_inactiv     = Dir.dirset.color_inactive; //MakeColor_Makro(LSE_COLOR8, BLACK);
		  ps->color_activ       = Dir.dirset.color_active; //MakeColor_Makro(BLUE, WHITE);
		  ps->frame_out         = Dir.dirset.color_frame; //MakeColor_Makro(DARKGRAY, WHITE);  // color of frame
	    ps->frame_scroll      = DEFCOLOR;
		  ps->position.no       = pt->Files.Label.GetNo();  // no of labels
      ps->level.active      = _TRUE;
		  ps->level.room.xy1.x  = ps->scroll_room.xy2.x + 1;
		  ps->level.room.xy1.y  = ps->scroll_room.xy1.y - 1;
		  ps->level.room.xy2.x  = ps->level.room.xy1.x + LEVELBUTTON_WIDTH;
		  ps->level.room.xy2.y  = ps->scroll_room.xy2.y + 1;


		  ps->out_room.xy1.x    = ps->scroll_room.xy1.x - 1;
		  ps->out_room.xy1.y    = ps->scroll_room.xy1.y - 1;
		  ps->out_room.xy2.x    = ps->level.room.xy2.x;
		  ps->out_room.xy2.y    = ps->scroll_room.xy2.y + 1;

//		  ps->level.color       = MakeColor_Makro(LSE_COLOR8, DARKGRAY);  // color
//		  ps->level.frame_color = MakeColor_Makro(DARKGRAY, WHITE);  // color of frame
      tcol = GetBackColor_Makro(Dir.dirset.color_inactive);
      ps->level.color       = MakeColor_Makro(tcol, tcol);
      ps->level.frame_color = Dir.dirset.color_frame;        // color
		  ps->pt_line           = (_SCR_LINE)directory_lineout;
		  ps->pt                = &pt->Files.Label;
      chk_position_SCROLL(ps);

		  x                     = ps->out_room.xy2.x - ps->out_room.xy1.x + 6;
		  ps                    = &pt->Direct.scroll; //List.GetScroll();
		  *ps                   = pt->Files.scroll;
		  ps->out_room.xy1.x    += x;
		  ps->out_room.xy2.x    += x;
		  ps->scroll_room.xy1.x += x;
		  ps->scroll_room.xy2.x += x;
		  ps->position.no       = pt->Direct.Label.GetNo();  // no of lablels
		  ps->level.room.xy1.x  += x;
		  ps->level.room.xy2.x  += x;
		  ps->pt_line           = (_SCR_LINE)directory_lineout;
		  ps->pt                = &pt->Direct.Label;
		  chk_position_SCROLL(ps);

		  retcode = _TRUE;
	  }
  }
  else
  {
	 retcode = _TRUE;
  }

  return(retcode);
}

void _Directory::ActivWatch(_DIRECTORY *p, _BOOL st, _UWORD pos)
{
  switch(pos)
  {
    case 0 : // name
//             p->Files.scroll.Cursor  = NULL;
//             p->Direct.scroll.Cursor = NULL;
             OutEditorBox(p, st);
             break;
    case 1 : // direct
//             p->Files.scroll.Cursor  = NULL;
//             p->Direct.scroll.Cursor = Cursor_Get();
             ActBeam_SCROLL(&p->Direct.scroll, st);
             break;
    case 2 : // files
//             p->Files.scroll.Cursor  = Cursor_Get();
//             p->Direct.scroll.Cursor = NULL;
             ActBeam_SCROLL(&p->Files.scroll, st);
             break;
  }
}

_BOOL _Directory::Ready(_DIRECTORY *p, _BOOL firsttime, _ROOM *pr)
{
  _BOOL retcode = _TRUE;

  Start(p, firsttime, pr);

  Draw_SCROLL(&p->Files.scroll, _FALSE);
  Draw_SCROLL(&p->Direct.scroll, _FALSE);
  OutEditorBox(p, _FALSE);
  OutPath(p);

  ActivWatch(p, _TRUE, p->activ);


  return(retcode);
}

void _Directory::RunFileBox(_DIRECTORY *p, _EVENT *pe)
{
  _ASCII *pd;

  Run_SCROLL(&p->Files.scroll, pe);

  if(pe != NULL)
  {
    if(pe->ftype != _EVENT_NONE)
    {
      if(pe->ftype == _EVENT_KEYPRESS)
      {
        if(pe->scancode == _ENTER)
        {
          pd = p->Files.Label.GetText(GetPosition_SCROLL(&p->Files.scroll));
          StrCpy(p->chredit.fstring, sizeof(_CHAR), pd, sizeof(_ASCII));
          p->ready = _TRUE;
        }
      }
    }
  }
}

void _Directory::GetEditorRoom(_ROOM *pr, _DIRECTORY *p)
{
  pr->xy1.x = p->Files.scroll.out_room.xy1.x;
  pr->xy1.y = p->Files.scroll.out_room.xy1.y - p->Direct.scroll.line_height - 5;
  pr->xy2.x = p->Direct.scroll.level.room.xy2.x - 1;
  pr->xy2.y = pr->xy1.y + p->Direct.scroll.line_height;
}

void _Directory::OutEditorBox(_DIRECTORY *p, _BOOL st)
{
  _ROOM   room;
  _COLOR  coltxt = (st == _TRUE)? p->Direct.scroll.color_activ : p->Direct.scroll.color_inactiv;
  _ATTRIB attrib = (st == _TRUE)? p->Direct.scroll.attrib_activ : p->Direct.scroll.attrib_inactiv;
  _COLOR  colframe, colback;
  _FRAMETYPE frametype = _3DFRAME;


  GetEditorRoom(&room, p);
//  coltxt   = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(coltxt));
  colback  = MakeColor_Makro(GetBackColor_Makro(coltxt), GetBackColor_Makro(coltxt));
  colframe = MakeColor_Makro(INVISIBLE, INVISIBLE);
  OutTextAlign(p->chredit.fstring, 0, &room, p->Direct.scroll.font, attrib, frametype, coltxt, colback, colframe, sizeof(_CHAR));

 #ifdef UC_ROTATE
  RefreshRectangle(&room);
 #endif
}

_BOOL _Directory::IsHIDinEditor(_EVENT *pe, _DIRECTORY *p)
{
  _BOOL retcode = _FALSE;
  _ROOM room;

  if(pe->ftype == _EVENT_HIDPRESS)
  {
    GetEditorRoom(&room, p);
    if((pe->dot.x >= room.xy1.x) && (pe->dot.x <= room.xy2.x) &&
       (pe->dot.y >= room.xy1.y) && (pe->dot.y <= room.xy2.y))
    {
      retcode = _TRUE;
    }
  }

  return(retcode);
}

void _Directory::RunEditorBox(_DIRECTORY *p, _EVENT *pe)
{
  if(pe != NULL)
  {
    if(pe->ftype == _EVENT_KEYPRESS)
    {
      run_CHREDIT(&p->chredit, pe->scancode, _FALSE, _FALSE, pe->is_character);
     #ifdef MOUSE
      HideMouseCursor();
     #endif
      OutEditorBox(p, _TRUE);
     #ifdef MOUSE
      ShowMouseCursor();
     #endif

      if(pe->scancode == _ENTER)
      {
        p->ready = _TRUE;
      }
    }
  }
}

void _Directory::OutPath(_DIRECTORY *p)
{
  _ROOM  room;
  _COLOR coltxt, colframe, colback;
  _UWORD xx, width;
  _ASCII txt[300];
  _UWORD len, i;
  _UWORD run;
  _FRAMETYPE frametype = _DEFFRAME;


  StrCpy(txt, sizeof(_ASCII), p->path, sizeof(_ASCII));
  StrCat(txt, sizeof(_ASCII), p->name, sizeof(_ASCII));
  len = StrLen(txt, sizeof(_ASCII)) + 1;
  room.xy1.x = p->Files.scroll.out_room.xy1.x;
  room.xy1.y = p->Files.scroll.level.room.xy2.y + 3;
  room.xy2.x = p->Direct.scroll.level.room.xy2.x;
  room.xy2.y = room.xy1.y + p->Direct.scroll.line_height;
  coltxt     = p->Direct.scroll.color_inactiv;
  width      = room.xy2.x - room.xy1.x;
  run        = 3;

  do
  {
    xx = StrWidth(txt, sizeof(_ASCII));
    if(xx > width)
    {
      for(i=run; i<len; i++)
      {
        if(txt[i] == 92)
        {
          MemMove(&txt[7], &txt[i+1], len-i-1);
          len = len - i + 6;
          if(run == 3)
          {
            txt[3] = '.';
            txt[4] = '.';
            txt[5] = '.';
            txt[6] = 92;
            run    = 7;
          }
          break;
        }
      }
    }
    else
    {
      run = 0;
    }
  }
  while(run);

  StrUp(txt, sizeof(_ASCII), 0);
//  coltxt   = MakeColor_Makro(GetBackColor_Makro(coltxt), GetPenColor_Makro(coltxt));
  colback  = MakeColor_Makro(GetBackColor_Makro(coltxt), GetBackColor_Makro(coltxt));
  colframe = MakeColor_Makro(INVISIBLE, INVISIBLE);
  OutTextAlign(txt, 0, &room, p->Direct.scroll.font, p->Direct.scroll.attrib_inactiv, frametype, coltxt, colback, colframe, sizeof(_ASCII));

 #ifdef UC_ROTATE
  RefreshRectangle(&room);
 #endif

}

void _Directory::RunDirectBox(_DIRECTORY *p, _EVENT *pe, _ROOM *pr)
{
  _ASCII *pd;
  _ASCII name[3];

  Run_SCROLL(&p->Direct.scroll, pe);

  if(pe != NULL)
  {
    if(pe->ftype != _EVENT_NONE)
    {
      if(pe->ftype == _EVENT_KEYPRESS)
      {
        if(pe->scancode == _ENTER)
        {
          pd = p->Direct.Label.GetText(GetPosition_SCROLL(&p->Direct.scroll));
          if(StrCmp(pd, sizeof(_ASCII), PATH_NONE, sizeof(_ASCII)) != 0)
          {
            if((pd[0] == '.') && (pd[1] == '.') && (pd[2] == 0))
            {
              SubDPNE(p->path);
            }
            else if((pd[0] == '.') && (pd[1] == 0))
            {
              p->path[3] = 0;
            }
            else if((pd[0] == '[') && (pd[2] == ':') && (pd[3] == ']'))
            {
              p->path[0] = 0;
              name[0] = pd[1];
              name[1] = ':';
              name[2] = 0;
              AddDPNE(p->path, name);
            }
            else
            {
              AddDPNE(p->path, pd);
            }

//            Start(p, _TRUE);
            Ready(p, _TRUE, pr);
          }
        }
      }
    }
  }
}

_BOOL _Directory::Run(_DIRECTORY *p, _EVENT *pe, _ROOM *pr)
{
  _BOOL retcode  = _TRUE;

  if(pe->ftype != _EVENT_NONE)
  {
    if(p->activ == 1)
    {
      if((pe->ftype == _EVENT_KEYPRESS)&&(pe->scancode == _LEFT))
        p->activ = 2;
     #ifdef HID
      if(is_HID_SCROLL(pe, &p->Files.scroll) == _TRUE)
      {
        p->activ  = 2;
        pe->ftype = _EVENT_NONE; // kill event
      }
      else if(IsHIDinEditor(pe, p) == _TRUE)
      {
        p->activ  = 0;
        pe->ftype = _EVENT_NONE; // kill event
      }
     #endif
    }
    else if(p->activ == 2)
    {
      if((pe->ftype == _EVENT_KEYPRESS)&&(pe->scancode == _RIGHT))
        p->activ = 1;
     #ifdef HID
      if(is_HID_SCROLL(pe, &p->Direct.scroll) == _TRUE)
      {
        p->activ  = 1;
        pe->ftype = _EVENT_NONE; // kill event
      }
      else if(IsHIDinEditor(pe, p) == _TRUE)
      {
        p->activ  = 0;
        pe->ftype = _EVENT_NONE; // kill event
      }
     #endif
    }
   #ifdef HID
    else if(p->activ == 0)
    {
      if(is_HID_SCROLL(pe, &p->Direct.scroll) == _TRUE)
      {
        p->activ  = 1;
        pe->ftype = _EVENT_NONE; // kill event
      }
      if(is_HID_SCROLL(pe, &p->Files.scroll) == _TRUE)
      {
        p->activ  = 2;
        pe->ftype = _EVENT_NONE; // kill event
      }
    }
   #endif

    if(pe->ftype == _EVENT_KEYPRESS)
    {
      if(pe->scancode == _TAB)
      {
        p->activ ++;
        if(p->activ > 2)
          p->activ = 0;
        pe->scancode = 0;
      }
    }

    if(p->activ != p->oldactiv)
    {
      ActivWatch(p, _FALSE, p->oldactiv);
      ActivWatch(p, _TRUE, p->activ);
      p->oldactiv = p->activ;
    }


//    if(pe->ftype == _EVENT_KEYPRESS)
//      pe->ftype = _EVENT_NONE;                                    // eat event
  }

  switch(p->activ)
  {
    case 0 : RunEditorBox(p, pe); break;
    case 1 : RunDirectBox(p, pe, pr); break;
    case 2 : RunFileBox(p, pe);   break;
  }

  return(retcode);
}

_BOOL _Directory::End(_DIRECTORY *pt)
{
  free_DIRLIST(&pt->Files);
  free_DIRLIST(&pt->Direct);
  return(_TRUE);
}

_BOOL _Directory::Call(_WORKSTATE ws, _EVENT *pe, _BOOL firsttime, _ROOM *pr)
{
  _BOOL retcode = _FALSE;

  switch(ws)
  {
    case STATE_START : if(firsttime == _TRUE)
                         Dir.ready = _FALSE;
                       // if(Start(&Dir, firsttime) == _TRUE)
                       {
                         Dir.workstate = STATE_READY;
                         retcode = _TRUE;
                       }
                       break;
    case STATE_READY : if(Ready(&Dir, firsttime, pr) == _TRUE)
                       {
                         Dir.workstate = STATE_RUN;
                         retcode = _TRUE;
                       }
                       break;
    case STATE_RUN   : if(Run(&Dir, pe, pr) == _TRUE)
                       {
                         retcode = _TRUE;
                         if(Dir.ready == _TRUE)
                         {
                           make_dpne(Dir.dpne, Dir.path, Dir.chredit.fstring, Dir.name);
                           retcode = _FALSE;
                           if(Dir.fkt != NULL)
                           {
                             Root.FindTop()->AddCmd(CMD_RETURN, 0, 0, _FALSE);
                             retcode = _TRUE;
                             Dir.fkt(Dir.dpne, Dir.data);
                           }
                         }
                       }
                       break;
    case STATE_END :   if(End(&Dir) == _TRUE)
                       {
                         retcode = _TRUE;
                       }
                       break;
    default        :   break;
  }

  return(retcode);
}

void NewDirectorySetup(_FILE_STATE state, _ASCII *ne, _FBOOL_PT_PT fkt, void *data, _DIRECTORYSETUP *pds)
{
  Directory.Setup(state, ne, fkt, data, NULL, pds);
  Root.AddCmd(CMD_NEWWINDOW, UC_DIRECTORY, 0, _FALSE);
}

void DirectorySetup(_FILE_STATE state, _ASCII *ne, _FBOOL_PT_PT fkt, void *data)
{
  NewDirectorySetup(state, ne, fkt, data, NULL);
}

_BOOL DirectoryCall(_WORKSTATE ws, _EVENT *pe, _BOOL firsttime, _ROOM *pr)
{
  return(Directory.Call(ws, pe, firsttime, pr));
}
#endif


// ****************
// ****************
// ****************
// ****************
// ****************


