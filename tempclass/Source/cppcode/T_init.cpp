#include "T_IPC.H"
#include "T_NUC.H"
#include "COMINTFC.H"


#ifdef UC_FIXMEMORY
 _UBYTE           *FixMemory;
 _UDWORD          FixMemoryIndex;
#endif

_SUBACTUAL         data_SUBACTUAL;
_FAST_ASCII_BUFFER data_FAST_ASCII_BUFFER;
_MYTXTSCHEME       data_MYTXTSCHEME;
_STACK             data_STACK;
_VARLABEL          data_VARLABEL;
_STACK_LINE        data_STACK_LINE;
_LSEFUNCT          data_LSEFUNCT;
_CHKBIT            data_CHKBIT;
_IMAGE             data_IMAGE;
_RECTANGLE         data_RECTANGLE;
_VARIABLE          data_VARIABLE;
_VARINFO           data_VARINFO;
_IOINFO            data_IOINFO;
_IO                data_IO;
_INPUT             data_INPUT;
_SCHEME            data_SCHEME;
_MYSCHEME          data_MYSCHEME;
_EVENT             data_EVENT;
_LINKSCHEME        data_LINKSCHEME;
_ANYPARA           data_ANYPARA;
_RESULT            data_RESULT;
_LSEVARINFO        data_LSEVARINFO;
_BUTTON            data_BUTTON;
_PICTEXT           data_PICTEXT;
_OBJECT            data_OBJECT;
_PICTUREMASK       data_PICTUREMASK;
_PICTURE           data_PICTURE;
_BMP               data_BMP;
_SINGLESOFT        data_SINGLESOFT;
_GFUNCT            data_GFUNCT;
_SEEKELEMENT       data_SEEKELEMENT;



void urinit_SUBACTUAL(_SUBACTUAL *pt)
{
  pt->act_output    = 0;
  pt->act_input     = 0;
  pt->act_motion    = 0;
  pt->act_object    = 0;
  pt->act_colrect   = 0;
  pt->act_colpoly   = 0;
  pt->act_colcirc   = 0;
  pt->act_colpictxt = 0;
  pt->act_button    = 0;
  pt->act_pictext   = 0;
//  pt->cnt_priority  = 0;
//  pt->cnt           = 0;
}

void urinit_ANYTHING(_ANYTHING *p)
{
  p->list = DEFLST; 
  p->no = 0;
}

void urinit_ROOM(_ROOM *p)
{
  p->xy1.x = 0;
  p->xy1.y = 0;
  p->xy2.x = 0;
  p->xy2.y = 0;
}

void urinit_USERTEXT(_USERTEXT *p)
{
  p->ptr = NULL;
}

void urinit_FAST_ASCII_BUFFER(_FAST_ASCII_BUFFER *p)
{
  init_ASCII_BUFFER(&p->ascii_buffer);
  p->poffset = 0;
}

void urinit_ANYPARA(_ANYPARA *p)
{
  urinit_ANYTHING(&p->anything);
  init_MEM_VARLABEL(&p->memvarlabel);
}

void urinit_MYTXTSCHEME(_MYTXTSCHEME *p)
{
  p->schemeno = DEFAULTSCHEME;
  init_TXTSCHEME(&p->overload);
  urinit_ANYPARA(&p->anypara);
}

void urinit_STACK(_STACK *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void urinit_VARLABEL(_VARLABEL *p)
{
  _UWORD i;

  p->no = 0;
  for(i=0; i<=VL_MAXINDEX; i++)
  {
    p->info[i].state = DEF_VAL;
    p->info[i].value = 0;
  }
}

void urinit_STACK_LINE(_STACK_LINE *p)
{
  p->cmd = CMD_NOP;
  urinit_VARLABEL(&p->op1);
  urinit_VARLABEL(&p->op2);
  p->lrm = _FALSE;
}

void urinit_LSEFUNCT(_LSEFUNCT *p)
{
  urinit_STACK(&p->overload);
  p->no = 0xFFFF;
}

void urinit_CHKBIT(_CHKBIT *p)
{
  p->seqno  = DEFSEQNO;
  p->invert = 0;
  init_COMPCODE(&p->overload);
}

void urinit_IMAGE(_IMAGE *p)
{
  urinit_ROOM(&p->room);
  p->ptr = NULL;
}

void urinit_RECTANGLE(_RECTANGLE *p)
{
  urinit_CHKBIT(&p->chkbit);
  urinit_ROOM(&p->room);
  p->color      = DEFCOLOR;
  p->colorframe = DEFCOLOR;
  p->attrib     = DEFATTRIB;
  p->frametype  = _DEFFRAME;
}

void urinit_RESULT(_RESULT *p)
{
  p->ftype = RES_DEFAULT;
  p->ptr   = NULL;
  p->value = 0;
  p->rmin  = 0;
  p->rmax  = 0;
  init_DATIM(&p->datim);
  p->recalculate_done = 0;
  p->remotedata = 0;
  p->myiocrc = 0;
}

void urinit_SCHEME(_SCHEME *p)
{
  urinit_VARLABEL(&p->link);
  init_MEM_SINGLESCHEME(&p->memsinglescheme);
}

void urinit_MYSCHEME(_MYSCHEME *p)
{
  p->schemeno = DEFAULTSCHEME;
  urinit_SCHEME(&p->overload);
}

void urinit_LINKSCHEME(_LINKSCHEME *p)
{
  urinit_MYSCHEME(&p->myscheme);
  urinit_VARLABEL(&p->varlabel);
}

void urinit_VARINFO(_VARINFO *p)
{
 #ifdef UC_LINKVL
  _UWORD i;
 #endif

  p->vartype         = FREE;
  p->classid         = DEFSCOPE;
  urinit_VARLABEL(&p->lo_limit);
  urinit_VARLABEL(&p->hi_limit);
  p->userconfig      = 0;                   // Anwenderkonfiguration
  p->accessno        = 0;
  p->format          = 0x0000;          // format (size) of variable
  p->station         = DEFSTATION;                  // stationnumber
  p->bitno           = 0;                               // Bitnummer
  p->funit           = DEF_VUNIT;                            // unit
  p->funitscheme     = 0xFFFF;                 // link to unitscheme
  p->POWPRUIC        = 0;
  p->reftime         = 100;                           // refreshtime
  p->varobj          = 0;                // object(1) or variable(0)
  urinit_ANYPARA(&p->text1);                             // vartext 1
  urinit_ANYPARA(&p->text2);                             // vartext 2
  urinit_ANYPARA(&p->text3);                             // vartext 3
  urinit_ANYPARA(&p->text4);                             // vartext 4
  urinit_VARLABEL(&p->xadress);
  init_STATISTIC(&p->statistic);
  p->stscheme        = DEFAULTSCHEME;

  p->formula         = DEFFORMULA;                        // formula
  p->digits          = 0;                            // no of digits
  p->posdezpt        = 0;                   // position dezimalpoint
  urinit_VARLABEL(&p->parameter);
  init_LASALID(&p->lasalid);                             // lasal id
  p->varobj          = 0 ;
  p->uimenuindex     = 0xFFFF;
  p->crc32 = 0;

  init_XCHKBIT(&p->xchkbit);

 #ifdef UC_LINKVL
  for(i=0; i<UC_LINKVL; i++)
    init_LINKVL(&p->linkvl[i]);
 #endif
  p->physic          = 0;
  p->hostname        = DEFSCOPE;
 #ifdef UC_EXOONLINE
  p->exoinfo         = 0;
 #endif
 #ifdef USER_DEMAG
  p->DemagDynPara    = 0; // sa24193
 #endif
}

void urinit_EVENT(_EVENT *p)
{
  p->scancode     = _NOKEY;
  p->ftype        = _EVENT_NONE;
  p->modifier     = 0;
  p->user[0]      = 0;
  p->user[1]      = 0;
  p->selfmade     = 0;
  init_DOT(&p->dot);
  p->state        = 0;
  p->source       = 0;
  p->is_character = _FALSE;
  p->raw.x        = 0;
  p->raw.y        = 0;
  p->lrm          = _FALSE;
  p->timestamp    = 0;
  p->multindex    = 0;
  p->already_used = 0;
}

void urinit_VARIABLE(_VARIABLE *v)
{
	v->no = DEFSCOPE;
	urinit_VARINFO(&v->info);
}

void urinit_IOINFO(_IOINFO *p)
{
  urinit_ANYTHING(&p->tbo);
  urinit_MYSCHEME(&p->myscheme);
  urinit_MYTXTSCHEME(&p->mytxtscheme);
  p->pcombo    = NULL;
  p->combocopy = _FALSE;
}

void urinit_IO(_IO *p)
{
  urinit_CHKBIT(&p->chkbit);
  urinit_ROOM(&p->room);
  p->attrib        = DEFATTRIB;
  p->font          = DEFFONT;
  p->colback       = DEFCOLOR;
  p->colframe      = DEFCOLOR;
  p->coltxt        = DEFCOLOR;
  p->typ           = AS_DEFAULT;
  p->Obj           = NULL;
  p->summary       = DEFSUMMARY;
  p->is_dyncolor   = _FALSE;
  p->frametype     = _DEFFRAME;
  p->ioid          = 0xFFFF;
  p->doimage       = _TRUE;
  p->lock_overload = _FALSE;

  init_VIRTNAME(&p->virtname);
  init_PICFORMAT(&p->picformat);
  urinit_VARLABEL(&p->variable);
  urinit_IOINFO(&p->info);
  init_CSTATE(&p->cstate);
  urinit_ROOM(&p->space);
  urinit_IMAGE(&p->image);
  
  p->imagecopy = _FALSE;
  
  init_WORKSTATE(&p->workstate);
  urinit_LINKSCHEME(&p->dyncolback);    // color background (hatch)
  urinit_LINKSCHEME(&p->dyncolfront);   // color background (hatch)
  urinit_LINKSCHEME(&p->dyncoltl);      // color frame (top-left)
  urinit_LINKSCHEME(&p->dyncolbr);      // color frame (bottom-right)
  urinit_LINKSCHEME(&p->dyncoltxt);     // color text
  urinit_MYSCHEME(&p->stscheme);
  urinit_LSEFUNCT(&p->lsefunct);
  init_DIRECTION(&p->direction);
  urinit_ANYPARA(&p->newbubbletext);



  p->theurge     = NULL;
  p->fillchr     = 0;
  p->refcolback  = DEFCOLOR;        // reference color background (hatch)
  p->refcolframe = DEFCOLOR;        // reference color frame (top-left, bottom-right)
  p->refcoltxt   = DEFCOLOR;        // reference color text
  p->pUser       = NULL;
  p->bRefreshAct = _FALSE;
  p->onscreen    = _FALSE;

  urinit_VARLABEL(&p->imoverloaded);
 #ifdef UC_CHANGE_IO_GROUP
  init_IOGRPCHX(&p->imiogroupchanged);
 #endif
  p->priority    = 0;
  p->xuser       = 0;
  p->editstate   = _EDITPASSIVE;
 #ifdef ZOOM_KM
  p->kmz.x       = 1000;
  p->kmz.y       = 1000;
 #endif
 #ifdef UC_MENU_TWOCOLOR
  p->refstschemestate = 0;
 #endif
}

void urinit_INPUT(_INPUT *p)
{
  urinit_IO(&p->io);
  p->enter_plus   = _EP_NONE;
  p->memseekno    = 0xFFFF;
  p->searchchoice = 0;
  p->selector     = _SELECTOR_DEFAULT;
  init_COMBOINFO(&p->comboinfo);
}

void urinit_LSEVARINFO(_LSEVARINFO *p)
{
	p->vartype         = FREE;
  p->classid         = 0xFFFF;
  init_HILOLIM(&p->hilolim);
	p->userconfig      = 0;                   // Anwenderkonfiguration
  p->accessno        = 0;
	p->format          = 0x0000;          // format (size) of variable
	p->station         = DEFSTATION;                  // stationnumber
	p->reftime_bitno   = 0;      // Refreshtime = 100ms, Bitnummer = 0
	p->funit           = DEF_VUNIT;                            // unit
	p->POWPRUIC        = 0;
  init_XMEM_ANYPARA(&p->xmemtext);                       // vartexte
	p->iaddress        = DEF_IADDRESS;
  init_STATISTIC(&p->statistic);
  p->stscheme        = DEFAULTSCHEME;

	init_LASALID(&p->lasalid);                             // lasal id
  p->uimenuindex     = 0xFFFF;
  p->crc32 = 0;

  init_XXCHKBIT(&p->xxchkbit);
  p->hostname        = 0xFFFF;
 #ifdef UC_LINKVL
  init_MEM_LINKVL(&p->memlinkvl);
 #endif
  p->funitscheme     = 0xFFFF;                 // link to unitscheme
  p->physic          = 0;
 #ifdef UC_MULTICPU
  _UWORD i;
  for(i=0; i<UC_MULTICPU; i++)
    p->multiid[i] = DEFLASALID;
 #endif
 #ifdef UC_EXOONLINE
  p->exoinfo         = 0;
 #endif
 #ifdef UC_MEMORIZE_PLCDATA
  p->plc_value_ftype = RES_DEFAULT;
  p->plc_value       = 0;
 #endif
 #ifdef UC_MEMBERVARIABLE
  p->memberoffset    = -1;
 #endif 
}

void urinit_GFUNCT(_GFUNCT *p)
{
  urinit_LSEFUNCT(&p->pos_fl);
  urinit_LSEFUNCT(&p->neg_fl);
}

void urinit_BUTTON(_BUTTON *p)
{
  urinit_CHKBIT(&p->chkbit);
  urinit_ROOM(&p->room);
  urinit_GFUNCT(&p->gfunct);
  urinit_ANYTHING(&p->anything);
  urinit_VARLABEL(&p->loader);
  urinit_LINKSCHEME(&p->dyncolback);     // dynamic color background (hatch)
  urinit_LINKSCHEME(&p->dyncolfront);    // dynamic color background (hatch)
  urinit_LINKSCHEME(&p->dyncoltl);       // dynamic color frame (top-left)
  urinit_LINKSCHEME(&p->dyncolbr);       // dynamic color frame (bottom-right)
  urinit_LINKSCHEME(&p->dyncoltxt);      // dynamic color text
  urinit_MYSCHEME(&p->stscheme);
  init_DIRECTION(&p->direction);

  urinit_VARLABEL(&p->varlabel);         // server
  urinit_ANYPARA(&p->text);              // released text
  urinit_ANYPARA(&p->text1);             // pressed text
  urinit_ANYPARA(&p->newbubbletext);     // bubblehelp text
  init_BUTTONIMAGES(&p->buttonimages);
  p->coltxt            = WHITE;        // MakeColor_Makro(INVISIBLE, WHITE);
  p->font              = DEFFONT;
  p->hotkeycode        = 0xFFFF;
  p->doimage           = _TRUE;
  p->lock_overload     = _FALSE;
  p->doubleclick       = _FALSE;
  p->searchchoice      = 0;
  p->wose              = 0;
  p->selector          = _SELECTOR_DEFAULT;
  p->act_momentary     = _FALSE; 
  p->set_val           = 1;
  p->reset_val         = 0;

	p->attrib            = DEFATTRIB;
	p->framecol	         = DEFCOLOR;     // color of frame
  p->col			         = DEFCOLOR;     // (in-)active color
  p->ftype             = _TKEY;        // key
  p->group             = 0;            // radiobuttongroup
  p->tip               = _TIPSINGLE;
  p->state             = _FALSE;       // pressed, released
  p->oldstate          = _FALSE;
  p->initcombi         = _FALSE;
  p->SubPicture        = NULL;
  p->summary           = DEFSUMMARY;
  p->enter_plus        = _EP_NONE;
  p->access            = DEFACCESS;
  p->selected          = _FALSE;
  p->frametype         = _DEFFRAME;
  p->refcolor          = DEFCOLOR;     // reference
  p->refcolorframe     = DEFCOLOR;     // reference
  p->refcolortxt       = DEFCOLOR;     // reference
  p->oldstatescheme    = STATE_ACTIVE;
 #ifdef USER_DEMAG
  p->enable            = _FALSE;
 #endif

  init_WORKSTATE(&p->workstate);
  urinit_IMAGE(&p->image);
 #ifdef UC_CHANGE_IO_GROUP
  init_IOGRPCHX(&p->imiogroupchanged);
 #endif
 #ifdef NINETEEN
  p->full_access_given = _FALSE;
 #endif
  p->reftxtcrc         = 0;
  
  urinit_USERTEXT(&p->usertext);
  urinit_USERTEXT(&p->usertext1);
  p->keycode[0]        = _NOKEY;
  p->keycode[1]        = _NOKEY;  
  p->keycode[2]        = _NOKEY;
  p->keycode[3]        = _NOKEY;
  
  p->fingercount       = 0;
}

void urinit_PICTEXT(_PICTEXT *p)
{
  urinit_CHKBIT(&p->chkbit);
  urinit_ROOM(&p->room);
  urinit_ANYPARA(&p->anypara);
  p->font          = DEFFONT;
  p->frametype     = _DEFFRAME;
  p->attrib        = DEFATTRIB;
  p->colback       = DEFCOLOR;
  p->colframe      = DEFCOLOR;
  p->coltxt        = DEFCOLOR;
  p->summary       = DEFSUMMARY;
  urinit_MYSCHEME(&p->stscheme);
  p->fillchr       = 0;
  urinit_IMAGE(&p->image);
  p->oldcrc        = 12345;
  p->onscreen      = _FALSE;
  p->doimage       = _TRUE;
  p->lock_overload = _FALSE;
  urinit_USERTEXT(&p->usertxt);
}

void urinit_OBJECT(_OBJECT *p)
{
  urinit_CHKBIT(&p->chkbit);
  init_DOT(&p->xy);
  p->no            = 0xFFFF;
  p->attrib        = DEFATTRIB;
  init_ZOOM(&p->zoom);
  p->SubPicture    = NULL;
  init_WORKSTATE(&p->workstate);
  p->colorframe    = DEFCOLOR;
  urinit_IMAGE(&p->image);
  urinit_VARLABEL(&p->loader);
  p->doimage       = _TRUE;
  p->lock_overload = _FALSE;
  urinit_MYSCHEME(&p->loader_scheme);
  urinit_MYSCHEME(&p->stscheme);
  p->onscreen      = _FALSE;
  p->summary       = DEFSUMMARY;
  init_DIRECTION(&p->direction);
 #ifdef UC_CHANGE_IO_GROUP
  init_IOGRPCHX(&p->imiogroupchanged);
 #endif
}

void urinit_PICTUREMASK(_PICTUREMASK *pt)
{
  pt->change = _FALSE;
  urinit_ROOM(&pt->room);
  pt->color = MakeColor_Makro(BLACK, BLACK);
  init_MEM_RECTANGLE(&pt->memrectangle);
  init_MEM_POLYGON(&pt->mempolygon);
  init_MEM_CIRCLE(&pt->memcircle);
  init_MEM_PICTEXT(&pt->mempictxt);
  init_MEM_OBJECT(&pt->memobject);
  init_MEM_BITMAP(&pt->membitmap);
  init_MEM_INPUT(&pt->meminput);
  init_MEM_OUTPUT(&pt->memoutput);
  init_MEM_MOTION(&pt->memmotion);
  init_MEM_COL_RECT(&pt->colrect);
  init_MEM_COL_POLY(&pt->colpoly);
  init_MEM_COL_CIRC(&pt->colcirc);
  init_MEM_COL_PICTXT(&pt->colpictxt);
  init_MEM_KEY(&pt->memkey);
  init_MEM_BUTTON(&pt->membutton);
  init_MEM_ZORDER(&pt->memzorder);
  urinit_FAST_ASCII_BUFFER(&pt->enclosed_name);
}

void urinit_PICTURE(_PICTURE *p)
{
  urinit_CHKBIT(&p->chkbit);
  p->TextPuffer.Init();
  urinit_PICTUREMASK(&p->mask);
  urinit_SUBACTUAL(&p->actual);
  init_PICRUNMEMO(&p->picrunmemo);
  p->MoveButtonGroup[0] = 0;
  p->MoveButtonGroup[1] = 0;
  p->MoveButtonGroup[2] = 0;
  p->MoveButtonGroup[3] = 0;
  p->MoveButtonGroup[4] = 0;
  p->MoveButtonGroup[5] = 0;
  p->MoveButtonGroup[6] = 0;
  p->MoveButtonGroup[7] = 0;
  p->MoveButtonGroup[8] = 0;
  p->MoveButtonGroup[9] = 0;  
}

void urinit_BMP(_BMP *p)
{
  p->width       = 0;
  p->height      = 0;
  p->transparent = INVISIBLE;
  p->state       = 0;
  p->datano      = 0;
  p->ptr         = NULL;
}

void urinit_SINGLESOFT(_SINGLESOFT *p)
{
  p->attrib   = DEFATTRIB;
  p->color    = MakeColor_Makro(LIGHTBLUE, BLUE);
  p->frame    = MakeColor_Makro(WHITE, DARKGRAY);
  p->text     = MakeColor_Makro(255, WHITE);
  p->tip      = _TIPSINGLE;
  p->font     = DEFFONT;
  urinit_ANYPARA(&p->anypara);
  urinit_CHKBIT(&p->chkbit);
  urinit_GFUNCT(&p->gfunct);
  p->frametype = _DEFFRAME;
  p->access   = DEFACCESS;
  init_BUTTONIMAGES(&p->buttonimages);
  urinit_VARLABEL(&p->varlabel);
  p->valid    = 1;
 #ifdef UC_DYN_SOFTKEYS
  urinit_LINKSCHEME(&p->dyncolback);
  urinit_LINKSCHEME(&p->dyncolfront);
  urinit_LINKSCHEME(&p->dyncoltl);
  urinit_LINKSCHEME(&p->dyncolbr);
  urinit_LINKSCHEME(&p->dyncoltxt);
  urinit_MYSCHEME(&p->stscheme);
 #endif
  p->wose          = 0;
  p->act_momentary = _FALSE;
  p->set_val       = 1;
  p->reset_val     = 0;
  urinit_ANYPARA(&p->newbubbletext);

  init_CSTATE(&p->cstate);
  p->state    = _FALSE;
  p->oldstate = _FALSE;
}

void urinit_SEEKELEMENT(_SEEKELEMENT *p)
{
  _UWORD i;

  init_DIRECTION(&p->direction);

  p->no         = 0;
  p->ioid       = 0xFFFF;
  p->hotkeycode = 0xFFFF;

  for(i=0; i<MAX_SEEKELEMENT; i++)
    init_SINGLESEEK(&p->ptr[i]);
}

_UDWORD          RevClass_MyIo;
_FBOOL_UWORD_PT  CallUserOverloadPtr;
_FLIPSCREEN      FlipScreen;
_BOOL            EditorInsertMde;
_Keyboard        Keyboard;
_Project         Project;
_Language        Language;
_SetUp           SetUp;
_ScreenSaver     ScreenSaver;
_ChkBit          ChkBit;
_VarList         VarList;
_TextList        TextList;
_ImageList       ImageList;
_Font            Font;
_Alarm           Alarm;
_Sequencer       Sequencer;
_Palette         Palette;
_ToolCatalogue   ToolCatalogue;
_Scheme          ColorScheme;
_Scheme          UnitScheme;
_Scheme          ServerScheme;
_Scheme          ImageScheme;
_FontScheme      FontScheme;
_Scheme          ScreenScheme;
_Scheme          StateScheme;
_Scheme          ObjectScheme;
_TextScheme      TextScheme;
_TermAlarm       TermAlarm;
IKEYPAD          *IKeyPad;
#ifdef UC_IPC
_Gfx           Gfx;
#endif
_PicMemo       PicMemo;
_Window        Root;
_ObjectList    ObjectList;
_Batch         Batch;
_InterBoxList  InterBoxList;
_ScanBar       ScanBar;
_MenuList      MenuList;
#ifdef UC_DIRECTORY
_Directory     Directory;
#endif
#ifdef OK_PICMEMO
_MemoPool      MemoPool;
#endif
#ifdef OK_VALIDATE
_Validate      Validate;
#endif
#ifdef OK_OFFSCREEN
_NEWSCREEN     OffScreen;
_NEWSCREEN     OnScreen;
_BOOL          UseOffscreen;
#endif
_SYSTEM       System;
_MEMORYIDX    MemoryIdx;
void          *LseThisPointer;
_UWORD        DrawBackGround;
_SYSERROR     SystemMessage;
_UWORD        DrawXPicture;
_UWORD        ReCalculate_IoId;
_PT_LOOKUPEMBEDDED LookUpEmbeddedPointer;
_IpcSettings  IpcSettings;
_UDWORD       ServerToEdit;
volatile _UDWORD       GlobalChangeIrq;
_UDWORD       GlobalChangeSys;
_UDWORD       GlobalChangeIrqMemo;
_UDWORD       TextRenderEngine;
_UDWORD       AckFlag;

_FVOID_PT_PT_PT    MethodLseWindowRun;
_FVOID_PT          MethodLseProjectRun;
_FBOOL_PT_PT_BOOL  MethodLseRecalculate;
_FVOID_DWORD_DWORD MethodLseUserCall;
_FBOOL_PT          MethodLseAccessButton;
_FBOOL_PT          MethodLseAccessInput;
_FBOOL_DWORD       MethodLseCheckEnable;
_FBOOL_PT_UWORD_FOUNDHID_BOOL MethodLseTryToAccess;
_FVOID_FOUNDHID    MethodLseFoundSomethingByHid;




#ifdef OK_DIAGNOSTIC
 #ifdef OK_SUBLIST
  _MEM_VKLIST   DiagVarList;
 #endif
#endif
#ifdef UC_THEME
_Theme         Theme;
#endif
_Hotkey        Hotkey;
_MessageBox    MessageBox;
#ifdef UC_BUBBLE_HELP
_NewBubbleHelp NewBubbleHelp;
#endif
_BOOL          SuperMode;
_UDWORD        MouseHideState;
#ifdef USER_DEMAG
_BOOL          OverloadAbsolute;
#endif
#ifdef XUSER_KM
_AllSoftkeys   AllSoftkeys;
#endif
_ServerWatch   ServerWatch;
#ifdef ZOOM_KM
_UDWORD        DoZoomKM;
#endif

#ifdef UC_DELETE_REFLIST
_BOOL          ChkForDeleteRefreshlist;
#endif

#ifdef UC_ROTATE
_UDWORD        ScreenStartStop;
#endif

_LSE_TIME LseTime;
_MULTIKEYBOARD MultiKeyboard;

#ifdef USER_BILLION
void *BillionPThis;
#endif

#ifdef NINETEEN
_Picture DashBoard;
#endif

void  construct_MAIN(void);
void  init_MAIN(_BOOL);
void  free_MAIN(void);

_BOOL chk_CHKBIT(_CHKBIT*);                               // check enablebit
void  init_DOT(_DOT*);
void  init_ANYTHING(_ANYTHING*);
void  free_ANYTHING(_ANYTHING*);
void  init_REGION(_REGION*);
void  free_REGION(_REGION*);
void  init_MEM_VARSYMBOL(_MEM_VARSYMBOL*);
void  free_MEM_VARSYMBOL(_MEM_VARSYMBOL*);
void  init_MEM_LSEVARINFO(_MEM_LSEVARINFO*);
void  free_MEM_LSEVARINFO(_MEM_LSEVARINFO*);
void  init_VARINFO(_VARINFO*);
void  free_VARINFO(_VARINFO*);
void  init_VARSYMBOL(_VARSYMBOL*);
void  free_VARSYMBOL(_VARSYMBOL*);
void  init_PICTEXT(_PICTEXT*);
void  free_PICTEXT(_PICTEXT*);
void  init_CIRCLE(_CIRCLE*);
void  free_CIRCLE(_CIRCLE*);
void  init_POLYGON(_POLYGON*);
void  free_POLYGON(_POLYGON*);
void  init_RECTANGLE(_RECTANGLE*);
void  free_RECTANGLE(_RECTANGLE*);
void  init_MEM_POLYGON(_MEM_POLYGON*);
void  free_MEM_POLYGON(_MEM_POLYGON*);
void  init_MEM_RECTANGLE(_MEM_RECTANGLE*);
void  free_MEM_RECTANGLE(_MEM_RECTANGLE*);
void  init_MEM_CIRCLE(_MEM_CIRCLE*);
void  free_MEM_CIRCLE(_MEM_CIRCLE*);
void  init_MEM_PICTEXT(_MEM_PICTEXT*);
void  free_MEM_PICTEXT(_MEM_PICTEXT*);
void  init_MEM_OBJECT(_MEM_OBJECT*);
void  free_MEM_OBJECT(_MEM_OBJECT*);
void  init_PICTUREMASK(_PICTUREMASK*);
void  free_PICTUREMASK(_PICTUREMASK*);
_BOOL chk_VARLABEL(_VARLABEL*);
_BOOL valid_VARLABEL(_VARLABEL*);
void  init_VARLABEL(_VARLABEL*);
void  free_VARLABEL(_VARLABEL*);
void  init_VARIABLE(_VARIABLE*);
void  free_VARIABLE(_VARIABLE*);
void  init_OBJECT(_OBJECT*);
void  free_OBJECT(_OBJECT*);
void  init_SOFTFLOOR(_SOFTFLOOR*);
void  free_SOFTFLOOR(_SOFTFLOOR*);
void  init_SINGLESOFT(_SINGLESOFT*);
void  free_SINGLESOFT(_SINGLESOFT*);
void  init_INPUT(_INPUT*);
void  free_INPUT(_INPUT*);
void  init_IO(_IO*);
void  free_IO(_IO*);
void  init_CSTATE(_CSTATE*);
void  free_CSTATE(_CSTATE*);
void  init_IOINFO(_IOINFO*);
void  free_IOINFO(_IOINFO*);
void  init_MEM_INPUT(_MEM_INPUT*);
void  free_MEM_INPUT(_MEM_INPUT*);
void  init_MEM_OUTPUT(_MEM_OUTPUT*);
void  free_MEM_OUTPUT(_MEM_OUTPUT*);
void  free_BITMAP(_BITMAP*);
void  init_BITMAP(_BITMAP*);
void  init_MEM_BITMAP(_MEM_BITMAP*);
void  free_MEM_BITMAP(_MEM_BITMAP*);
void  init_ZOOM(_ZOOM*);
void  free_ZOOM(_ZOOM*);
void  init_PRJ_ENTRY(_PRJ_ENTRY*);
void  free_PRJ_ENTRY(_PRJ_ENTRY*);
void  init_STACK_LINE(_STACK_LINE*);
void  free_STACK_LINE(_STACK_LINE*);
//void  init_STACK(_STACK*);
//void  free_STACK(_STACK*);
void  init_BMP(_BMP*);
void  free_BMP(_BMP*);
void  init_EDITOR(_EDITOR*);
void  free_EDITOR(_EDITOR*);
void  init_IMAGE(_IMAGE*);
void  free_IMAGE(_IMAGE*);
void  init_POSITION(_POSITION*);
void  free_POSITION(_POSITION*);
void  init_MENUCALCUL(_MENUCALCUL*);
void  free_MENUCALCUL(_MENUCALCUL*);
void  init_MENU(_MENU*);
void  free_MENU(_MENU*);
void  init_LSEMENU(_LSEMENU*);
void  free_LSEMENU(_LSEMENU*);
void  init_MEM_MOTION(_MEM_MOTION*);
void  free_MEM_MOTION(_MEM_MOTION*);
void  init_SUBACTUAL(_SUBACTUAL*);
void  free_SUBACTUAL(_SUBACTUAL*);
void  init_COL_RECT(_COL_RECT*);
void  init_MEM_COL_RECT(_MEM_COL_RECT*);
void  free_COL_RECT(_COL_RECT*);
void  free_MEM_COL_RECT(_MEM_COL_RECT*);
void  init_COL_POLY(_COL_POLY*);
void  init_MEM_COL_POLY(_MEM_COL_POLY*);
void  free_COL_POLY(_COL_POLY*);
void  free_MEM_COL_POLY(_MEM_COL_POLY*);
void  init_COL_CIRC(_COL_CIRC*);
void  init_MEM_COL_CIRC(_MEM_COL_CIRC*);
void  free_COL_CIRC(_COL_CIRC*);
void  free_MEM_COL_CIRC(_MEM_COL_CIRC*);
void  init_COL_PICTXT(_COL_PICTXT*);
void  init_MEM_COL_PICTXT(_MEM_COL_PICTXT*);
void  free_COL_PICTXT(_COL_PICTXT*);
void  free_MEM_COL_PICTXT(_MEM_COL_PICTXT*);
void  init_ROOM(_ROOM*);
void  init_RESULT(_RESULT*);
void  free_GFUNCT(_GFUNCT*);
void  init_GFUNCT(_GFUNCT*);
void  init_LASALID(_LASALID*);
void  init_PICFORMAT(_PICFORMAT*);
void  free_PICFORMAT(_PICFORMAT*);
void  init_MENUINFO(_MENUINFO*);
void  free_MENUINFO(_MENUINFO*);
void  init_BUTTON(_BUTTON*);
void  free_BUTTON(_BUTTON*);
void  init_MEM_BUTTON(_MEM_BUTTON*);
void  free_MEM_BUTTON(_MEM_BUTTON*);
void  init_WORKSTATE(_WORKSTATE*);
void  init_BUBBLEHELP(_BUBBLEHELP*);
void  free_BUBBLEHELP(_BUBBLEHELP*);
void  init_DPNE(_DPNE*);
void  free_DPNE(_DPNE*);
void  init_NEWSCREEN(_NEWSCREEN*);
void  free_NEWSCREEN(_NEWSCREEN*);
void  init_LINKSCHEME(_LINKSCHEME*);
void  free_LINKSCHEME(_LINKSCHEME*);
void  init_MYSCHEME(_MYSCHEME*);
void  free_MYSCHEME(_MYSCHEME*);
void  init_MEM_TextPuffer(_MEM_TextPuffer*);
void  free_MEM_TextPuffer(_MEM_TextPuffer*);

void init_MYSCHEME(_MYSCHEME *p)
{
  //urinit_MYSCHEME(p);
  init_MYSCHEME_Makro(p);
  //*p = data_MYSCHEME;
}

void free_MYSCHEME(_MYSCHEME *p)
{
  free_SCHEME(&p->overload);
  p->schemeno = DEFAULTSCHEME;
//  init_MYSCHEME(p); speed
}

void init_LINKSCHEME(_LINKSCHEME *p)
{
  //urinit_LINKSCHEME(p);
  //*p = data_LINKSCHEME;
  init_LINKSCHEME_Makro(p);
}

void setup_LINKSCHEME(_LINKSCHEME *p, _ASCII *colorschemelabel, _UDWORD varno)
{
  _UDWORD idx;
  
  free_LINKSCHEME(p);
  
  if(ColorScheme.LseLabel.Seek(&idx, colorschemelabel) == _TRUE)
  {
    p->myscheme.schemeno = idx;
    
    init_VARLABEL(&p->varlabel);
    if(varno < VarList.GetNo())
    {
      p->varlabel.no = 1;
      p->varlabel.info[0].value = varno;
      p->varlabel.info[0].state = VAR_VAL;
    }
  }
}

void free_LINKSCHEME(_LINKSCHEME *p)
{
  free_MYSCHEME(&p->myscheme);
  free_VARLABEL(&p->varlabel);

//  init_LINKSCHEME(p); speed
}
#ifdef UC_FIXMEMORY
void *GetFixMemory(void)
{
  if(FixMemory != NULL)
  {
    FixMemoryIndex ++;
    if(FixMemoryIndex >= FIXMEMORYNO)
      FixMemoryIndex = 0;
    return &FixMemory[FixMemoryIndex * FIXMEMORYSIZE];
  }
  
  return NULL;
}
#endif
void construct_MAIN(void)
{
 #ifdef UC_ROTATE
  ScreenStartStop = 0;
 #endif

 #ifdef UC_FIXMEMORY
  FixMemory = NULL;
  FixMemoryIndex = 0;
 #endif
 
  // reihenfolge der funktionsaufrufe nicht verändern !!!

  urinit_EVENT(&data_EVENT);
  urinit_BMP(&data_BMP);
  urinit_SUBACTUAL(&data_SUBACTUAL);
  urinit_FAST_ASCII_BUFFER(&data_FAST_ASCII_BUFFER);
  urinit_MYTXTSCHEME(&data_MYTXTSCHEME);
  urinit_STACK(&data_STACK);
  urinit_STACK_LINE(&data_STACK_LINE);
  urinit_LSEFUNCT(&data_LSEFUNCT);
  urinit_CHKBIT(&data_CHKBIT);
  urinit_IMAGE(&data_IMAGE);

  urinit_RECTANGLE(&data_RECTANGLE);

  urinit_RESULT(&data_RESULT);

  urinit_VARLABEL(&data_VARLABEL);

  urinit_ANYPARA(&data_ANYPARA);

  urinit_SCHEME(&data_SCHEME);
  urinit_MYSCHEME(&data_MYSCHEME);
  urinit_LINKSCHEME(&data_LINKSCHEME);

  urinit_VARINFO(&data_VARINFO);
  urinit_VARIABLE(&data_VARIABLE);
  urinit_LSEVARINFO(&data_LSEVARINFO);
  urinit_IOINFO(&data_IOINFO);
  urinit_IO(&data_IO);
  urinit_INPUT(&data_INPUT);

  urinit_PICTEXT(&data_PICTEXT);
  urinit_BUTTON(&data_BUTTON);
  urinit_OBJECT(&data_OBJECT);

  urinit_GFUNCT(&data_GFUNCT);
  urinit_SINGLESOFT(&data_SINGLESOFT);
  urinit_SEEKELEMENT(&data_SEEKELEMENT);

  urinit_PICTUREMASK(&data_PICTUREMASK);
  urinit_PICTURE(&data_PICTURE);
}

#ifndef GRC_XFREEZE_SCREEN
 #define GRC_XFREEZE_SCREEN 7
#endif
#ifndef GRC_USE_SFB
 #define GRC_USE_SFB 8
#endif

void init_MAIN(_BOOL urinit)
{
 #ifdef UC_FIXMEMORY
  Memory2((void**)&FixMemory, FIXMEMORYSIZE * FIXMEMORYNO);
 #endif
 
  RevClass_MyIo         = 0;
  MethodLseWindowRun    = NULL;
  MethodLseProjectRun   = NULL;
  MethodLseRecalculate  = NULL;
  MethodLseUserCall     = NULL;
  MethodLseAccessButton = NULL;
  MethodLseAccessInput  = NULL;
  MethodLseCheckEnable  = NULL;  
  MethodLseTryToAccess  = NULL;  
  MethodLseFoundSomethingByHid = NULL;

  if(urinit == true)
    JpgFunctSetup(NULL, NULL);
  
  CallUserOverloadPtr = NULL;
  TextRenderEngine = 0;
  AckFlag = 0;
 #ifdef NINETEEN
  DashBoard.Init();
 #endif

  MemoryIdx.act = MEMORY_LSE;
  MemoryIdx.cnt = 0;
  MemoryIdx.psh = MEMORY_LSE;
  FlipScreen.dx = 0;
  FlipScreen.yo = 0;
  FlipScreen.yu = 0;
 #ifdef ZOOM_KM
  DoZoomKM      = 0;
 #endif
 #ifdef USER_BILLION
  BillionPThis = NULL;
 #endif
 #ifdef UC_ROTATE
  ConfigGraphicSystem(GRC_USE_SFB, 0, NULL); // use graphic shadow frame buffer
 #endif

 #ifdef UC_DELETE_REFLIST
  ChkForDeleteRefreshlist = _FALSE;
 #endif
 #ifdef USER_DEMAG
  OverloadAbsolute = _FALSE;
 #endif
  MouseHideState  = 0;
  SystemMessage   = EMTY_TEXT;
  DrawBackGround  = 0;
  DrawXPicture    = 0;
	LseThisPointer  = NULL;
  SuperMode       = _FALSE;
  ServerToEdit    = DEFSCOPE;
  GlobalChangeIrq     = 0;
  GlobalChangeSys     = 0;
  GlobalChangeIrqMemo = 0;
  IKeyPad         = NULL;
  EditorInsertMde = _FALSE;

  init_cpyedit();  // copy editor initialisieren (space bei num eingabe)

  Keyboard.Init();
	Project.Init();
  Language.Init();
  SetUp.Init();
  ScreenSaver.Init();
//	IpcMemory.Init();
	ChkBit.Init();
//  GlobalPicture.Init();
	VarList.Init();
//	AtxList.Init();
//	StxList.Init();
//	MtxList.Init();
//  Zt1List.Init();
//  Zt2List.Init();
//  Zt3List.Init();
//  UnitList.Init();
//  TitList.Init();
  TextList.Init();
  ImageList.Init();
  MenuList.Init();
  Font.Init();
  Alarm.Init();
  Sequencer.Init();
  Palette.Init();
  ToolCatalogue.Init();
  ColorScheme.Init();
  ImageScheme.Init();
  UnitScheme.Init();
  ServerScheme.Init();
  FontScheme.Init();
  ScreenScheme.Init();
  StateScheme.Init();
  TextScheme.Init();
  TermAlarm.Init(urinit);
  ObjectScheme.Init();
  init_SYSTEM(&System);

 #ifdef OK_DIAGNOSTIC
  #ifdef OK_SUBLIST
   init_MEM_VKLIST(&DiagVarList);
  #endif
 #endif

 #ifdef UC_IPC
  Gfx.Init();
 #endif
  PicMemo.Init();
  Root.Init();
  ObjectList.Init();
  Batch.Init();
  InterBoxList.Init();
  ScanBar.Init();
 #ifdef UC_DIRECTORY
  Directory.Init();
 #endif
 #ifdef OK_PICMEMO
  MemoPool.Init();
 #endif
 #ifdef OK_VALIDATE
  Validate.Init();
 #endif
 #ifdef OK_OFFSCREEN

  #ifdef UC_ROTATE
  UseOffscreen = _FALSE;
  if(RTOSVERSION < 0x1156)
    UseOffscreen = _TRUE;
  #else
  UseOffscreen = _TRUE;
  #endif

  init_NEWSCREEN(&OffScreen);
  GetScreen(&OnScreen);
 #endif
  LookUpEmbeddedPointer = NULL;
  TextList.Init();
  IpcSettings.Init();
  Hotkey.Init();
  MessageBox.Init();
 #ifdef UC_BUBBLE_HELP 
  NewBubbleHelp.Init();
 #endif
 #ifdef UC_THEME
  Theme.Init();
 #endif
 #ifdef XUSER_KM
  AllSoftkeys.Init();
 #endif
  ServerWatch.Init();
  InitAllSlider();
}

void free_MAIN(void)
{
  Keyboard.Free();
  Project.Free();
  Language.Free();
  SetUp.Free();
  ScreenSaver.Free();
//  IpcMemory.Free();
  ChkBit.Free();
//  GlobalPicture.Free();
  VarList.Free();
//  AtxList.Free();
//  StxList.Free();
//  MtxList.Free();
//  Zt1List.Free();
//  Zt2List.Free();
//  Zt3List.Free();
//  UnitList.Free();
//  TitList.Free();
  TextList.Free(_TRUE);
  ImageList.Free();
  MenuList.Free();
  Font.Free();
  Alarm.Free();
  Sequencer.Free();
  Palette.Free();
  ToolCatalogue.Free();
  ColorScheme.Free();
  ImageScheme.Free();
  UnitScheme.Free();
  ServerScheme.Free();
  FontScheme.Free();
  ScreenScheme.Free();
  StateScheme.Free();
  TextScheme.Free();
  TermAlarm.Free();
  ObjectScheme.Free();
  free_SYSTEM(&System);

 #ifdef OK_DIAGNOSTIC
  #ifdef OK_SUBLIST
   free_MEM_VKLIST(&DiagVarList);
  #endif
 #endif

 #ifdef UC_IPC
  Gfx.Free();
 #endif
  PicMemo.Free();
  Root.Free();
  ObjectList.Free();
  Batch.Free();
  InterBoxList.Free();
  ScanBar.Free();
 #ifdef UC_DIRECTORY
  Directory.Free();
 #endif
 #ifdef OK_PICMEMO
  MemoPool.Free();
 #endif
 #ifdef OK_VALIDATE
  Validate.Free();
 #endif
 #ifdef OK_OFFSCREEN
  free_NEWSCREEN(&OffScreen);
 #endif

  LseThisPointer = NULL;

  TextList.Free(_TRUE);
  IpcSettings.Free();
  Hotkey.Free();
  MessageBox.Free();
 #ifdef UC_BUBBLE_HELP 
  NewBubbleHelp.Free();
 #endif
 #ifdef UC_THEME
  Theme.Free();
 #endif
 #ifdef XUSER_KM
  AllSoftkeys.Free();
 #endif
  ServerWatch.Free();

 #ifdef NINETEEN
  DashBoard.Free();
 #endif

 #ifdef UC_FIXMEMORY  
  Memory2((void**)&FixMemory, 0);
 #endif
}

void ScreenStart(void)
{
 #ifdef UC_ROTATE
  ScreenStartStop -= 1;
  if(ScreenStartStop == 0)
  {
    ConfigGraphicSystem(GRC_XFREEZE_SCREEN, 1, NULL); // start screen
  }

 #endif
}

void ScreenStop(void)
{
 #ifdef UC_ROTATE
  if(ScreenStartStop == 0)
  {
    ConfigGraphicSystem(GRC_XFREEZE_SCREEN, 0, NULL); // stop screen
  }

  ScreenStartStop += 1;
 #endif
}

_UDWORD ScreenStatePush(void)
{
 #ifdef UC_ROTATE
  if(ScreenStartStop != 0)
  {
    ConfigGraphicSystem(GRC_XFREEZE_SCREEN, 1, NULL); // start screen
  }

  return(ScreenStartStop);
 #else
  return(0);
 #endif
}

void ScreenStatePop(_UDWORD val, _ROOM *pr)
{
 #ifdef UC_ROTATE
  if(pr == NULL)
  {
    _ROOM room;
    GetScreenSize(&room);
    RefreshRectangle(&room);
  }
  else
  {
    RefreshRectangle(pr);
  }

  if(val != 0)
  {
    ConfigGraphicSystem(GRC_XFREEZE_SCREEN, 0, NULL); // stop screen
  }

  ScreenStartStop = val;

 #endif
}

_BOOL chk_CHKBIT(_CHKBIT *p)
{
  _BOOL invert = (p->invert == 0)? _FALSE : _TRUE;
  
  if(p->seqno != DEFSEQNO)
  {
    return Sequencer.Call(p->seqno, invert, _TRUE, _TRUE);
  }
  else if(p->overload.size > 0)
  {
    return Sequencer.CallCompiler(&p->overload, invert, _TRUE, _TRUE);
  }

  return _TRUE;
}

_BOOL chk_XXCHKBIT(_XXCHKBIT xxchkbit)
{
  if((xxchkbit & 0x7FFF) != 0x7FFF)
    return Sequencer.Call(xxchkbit & 0x7FFF, (xxchkbit & 0x8000)? _TRUE : _FALSE, _TRUE, _TRUE);

  return _TRUE;
}

_BOOL chk_XCHKBIT(_XCHKBIT *p)
{
  if(p->seqno != DEFSEQNO)
    return Sequencer.Call(p->seqno, (p->invert == 0)? _FALSE : _TRUE, _TRUE, _TRUE);

  return _TRUE;
}

_BOOL is_MYSCHEME(_MYSCHEME *p, _Scheme *Scheme)
{
  if(p->schemeno < Scheme->GetNo())
    return _TRUE;
  else if(p->overload.memsinglescheme.no > 0)
    return _TRUE;

  return _FALSE;
}

_BOOL is_CHKBIT(_CHKBIT *p)
{
  if(p->seqno != DEFSEQNO)
  {
    if(p->seqno < Sequencer.GetNo())
      return _TRUE;
  }
  else if((p->overload.size > 0) && (p->overload.ptr != NULL))
  {
    return _TRUE;
  }
  
  return _FALSE;
}

void init_MEM_VARSYMBOL(_MEM_VARSYMBOL *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_VARSYMBOL(_MEM_VARSYMBOL *p)
{
  _UDWORD i;
  for(i=0; i<p->no; i++)
    free_VARSYMBOL(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);
  p->no  = 0;
//  init_MEM_VARSYMBOL(p); speed
}

void init_MEM_BMP(_MEM_BMP *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_BMP(_MEM_BMP *p)
{
  _UDWORD i;

  for(i=0; i<p->no; i++)
    free_BMP(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);
  p->no  = 0;
//  init_MEM_BMP(p); speed
}

void init_MEM_LSEVARINFO(_MEM_LSEVARINFO *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_LSEVARINFO(_MEM_LSEVARINFO *p)
{
  _UDWORD i;
  for(i=0; i<p->no; i++)
    free_LSEVARINFO(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);
  p->no  = 0;
//  init_MEM_LSEVARINFO(p); speed
}

void init_CHKBIT(_CHKBIT *p)
{
  //*p = data_CHKBIT;
  init_CHKBIT_Makro(p);
}

void free_CHKBIT(_CHKBIT *p)
{
  free_COMPCODE(&p->overload);
  p->seqno  = DEFSEQNO;
  p->invert = 0;
//  init_CHKBIT(p); speed
}

void init_XCHKBIT(_XCHKBIT *p)
{
  p->seqno  = DEFSEQNO;
  p->invert = 0;
}

void init_XXCHKBIT(_XXCHKBIT *p)
{
  *p = 0x7FFF;
}

void free_XCHKBIT(_XCHKBIT *p)
{
  p->seqno  = DEFSEQNO;
  p->invert = 0;
//  init_XCHKBIT(p); speed
}

void init_HILOLIM(_HILOLIM *p)
{
  p->state = (DEF_VAL << 4)|(DEF_VAL);
  p->lolim = -1;
  p->hilim = -1;
}

void free_HILOLIM(_HILOLIM *p)
{
  p->state = (DEF_VAL << 4)|(DEF_VAL);
  p->lolim = -1;
  p->hilim = -1;
}

void init_LSEVARINFO(_LSEVARINFO *p)
{
  //urinit_LSEVARINFO(p);
  //*p = data_LSEVARINFO;
  init_LSEVARINFO_Makro(p);
}

void free_LSEVARINFO(_LSEVARINFO *p)
{
	free_HILOLIM(&p->hilolim);
  free_STATISTIC(&p->statistic);
//  free_XXCHKBIT(&p->xxchkbit);
  free_XMEM_ANYPARA(&p->xmemtext);                         // vertexte

 #ifdef UC_LINKVL
  free_MEM_LINKVL(&p->memlinkvl);
 #endif

	init_LSEVARINFO(p);
}

void init_VARINFO(_VARINFO *p)
{
  init_VARINFO_Makro(p);
}

void free_VARINFO(_VARINFO *p)
{
 #ifdef UC_LINKVL
  _UWORD i;
 #endif

	free_VARLABEL(&p->xadress);
	free_VARLABEL(&p->parameter);
	free_VARLABEL(&p->lo_limit);
	free_VARLABEL(&p->hi_limit);
//  free_VARLABEL(&p->lost_limit);
//  free_VARLABEL(&p->hist_limit);
  free_STATISTIC(&p->statistic);
  free_XCHKBIT(&p->xchkbit);

 #ifdef UC_LINKVL
  for(i=0; i<UC_LINKVL; i++)
    free_LINKVL(&p->linkvl[i]);
 #endif

	init_VARINFO_Makro(p);
}

void init_VARSYMBOL(_VARSYMBOL *p)
{
	//p->scope = DEFSCOPE;                                   // structur
  init_VARSYMBOL_Makro(p);
}

void free_VARSYMBOL(_VARSYMBOL *p)
{
	//p->scope = DEFSCOPE;                                   // structur
  init_VARSYMBOL_Makro(p);
}

void init_DOT(_DOT *pt)
{
	pt->x = 0;
	pt->y = 0;
}

void init_ROOM(_ROOM *p)
{
  //p->xy1.x = 0;
  //p->xy1.y = 0;
  //p->xy2.x = 0;
  //p->xy2.y = 0;
  init_ROOM_Makro(p);
}

void init_ANYTHING(_ANYTHING *pt)
{
  //pt->list = DEFLST;
  //pt->no   = 0;
  init_ANYTHING_Makro(pt);
}

void free_ANYTHING(_ANYTHING *pt)
{
  //pt->list = DEFLST;
  //pt->no   = 0;
  init_ANYTHING_Makro(pt);
}

#ifdef UC_CHANGE_IO_GROUP
void init_IOGRPCHX(_IOGRPCHX *p)
{
  p->serverno = DEFSCOPE;
  p->summary  = DEFSUMMARY;
}

void free_IOGRPCHX(_IOGRPCHX *p)
{
  p->serverno = DEFSCOPE;
  p->summary  = DEFSUMMARY;
  //init_IOGRPCHX(p);
}
#endif

void init_USERTEXT(_USERTEXT *p)
{
  //p->ptr = NULL;
  init_USERTEXT_Makro(p);
}

_BOOL setup_USERTEXT(_USERTEXT *p, void *txt, _UWORD chrsize)
{
  free_USERTEXT(p);
  
  if(txt != NULL)
  {
    _UDWORD size = (StrLen(txt, chrsize) + 1) * sizeof(_CHAR);
    if(Memory2((void**)&p->ptr, size) == 0)
      return _FALSE;
    StrCpy(p->ptr, sizeof(_CHAR), txt, chrsize);
  }
  
  return _TRUE;
}

void free_USERTEXT(_USERTEXT *p)
{
  if(p->ptr != NULL)
    Memory2((void**)&p->ptr, 0);
}

void init_PICTEXT(_PICTEXT *p)
{
  //urinit_PICTEXT(p);
  *p = data_PICTEXT;
}

void free_PICTEXT(_PICTEXT *p)
{
  free_CHKBIT(&p->chkbit);
  free_ANYPARA(&p->anypara);
  free_IMAGE(&p->image);
  free_MYSCHEME(&p->stscheme);
  free_USERTEXT(&p->usertxt);
    
  init_PICTEXT(p);
}

void init_CIRCLE(_CIRCLE *pt)
{
  init_CHKBIT_Makro(&pt->chkbit);
  init_ROOM_Makro(&pt->room);
  pt->angle       = 0;
  pt->arc         = 0;
  pt->color       = DEFCOLOR;
  pt->attrib      = DEFATTRIB;
  pt->colorframe  = DEFCOLOR;
}

void free_CIRCLE(_CIRCLE *pt)
{
  free_CHKBIT(&pt->chkbit);
  init_ROOM_Makro(&pt->room);
  pt->angle       = 0;
  pt->arc         = 0;
  pt->color       = DEFCOLOR;
  pt->attrib      = DEFATTRIB;
  pt->colorframe  = DEFCOLOR;

//	init_CIRCLE(pt);speed
}

void init_POLYGON(_POLYGON *p)
{
  init_CHKBIT_Makro(&p->chkbit);
  p->no     = 0;
  p->ptr    = NULL;
  p->color  = DEFCOLOR;
  p->colorframe = DEFCOLOR;
  p->attrib = DEFATTRIB;
}

void free_POLYGON(_POLYGON *p)
{
  free_CHKBIT(&p->chkbit);
  Memory2((void**)&p->ptr, 0);
  p->no     = 0;
  p->color  = DEFCOLOR;
  p->colorframe = DEFCOLOR;
  p->attrib = DEFATTRIB;

//	init_POLYGON(p);speed
}

void init_RECTANGLE(_RECTANGLE *p)
{
  *p = data_RECTANGLE;
}

void free_RECTANGLE(_RECTANGLE *p)
{
  free_CHKBIT(&p->chkbit);
  *p = data_RECTANGLE;

  // init_RECTANGLE(p);speed
}

void init_BITMAP(_BITMAP *p)
{
  init_CHKBIT_Makro(&p->chkbit);
  init_ROOM_Makro(&p->room);
  p->no     = 0xFFFF;
  p->attrib = DEFATTRIB;
  p->color  = DEFCOLOR;
  p->colorframe = DEFCOLOR;
  p->userbmp    = NULL;
}

void free_BITMAP(_BITMAP *p)
{
  free_CHKBIT(&p->chkbit);
  init_ROOM_Makro(&p->room);
  p->no     = 0xFFFF;
  p->attrib = DEFATTRIB;
  p->color  = DEFCOLOR;
  p->colorframe = DEFCOLOR;

  if(p->userbmp != NULL)
  {
    free_BMP(p->userbmp);
    Memory2((void**)&p->userbmp, 0);
  }
//	init_BITMAP(p);speed
}

void init_MEM_POLYGON(_MEM_POLYGON *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_POLYGON(_MEM_POLYGON *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
    free_POLYGON(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
//  init_MEM_POLYGON(pt);speed
}

void init_MEM_BITMAP(_MEM_BITMAP *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_BITMAP(_MEM_BITMAP *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
    free_BITMAP(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
//  init_MEM_BITMAP(pt);speed
}

void init_MEM_RECTANGLE(_MEM_RECTANGLE *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_RECTANGLE(_MEM_RECTANGLE *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
    free_RECTANGLE(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
  //init_MEM_RECTANGLE(pt);speed
}

void init_MEM_CIRCLE(_MEM_CIRCLE *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_CIRCLE(_MEM_CIRCLE *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
    free_CIRCLE(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
//  init_MEM_CIRCLE(pt);speed
}

void init_MEM_PICTEXT(_MEM_PICTEXT *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void init_MEM_OBJECT(_MEM_OBJECT *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_OBJECT(_MEM_OBJECT *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
		free_OBJECT(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);
  pt->no  = 0;

//	init_MEM_OBJECT(pt);speed
}

void free_MEM_PICTEXT(_MEM_PICTEXT *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
    free_PICTEXT(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
//  init_MEM_PICTEXT(pt);speed
}

void init_MEM_INPUT(_MEM_INPUT *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_INPUT(_MEM_INPUT *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
    free_INPUT(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
//  init_MEM_INPUT(pt);speed
}

void init_MEM_OUTPUT(_MEM_OUTPUT *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_OUTPUT(_MEM_OUTPUT *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
    free_IO(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
//  init_MEM_OUTPUT(pt);speed
}

void init_MEM_MOTION(_MEM_MOTION *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_MOTION(_MEM_MOTION *pt)
{
	_UWORD i;

	for(i=0; i<pt->no; i++)
		free_MOTION(&pt->ptr[i]);

	Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
//	init_MEM_MOTION(pt);speed
}

void init_ZORDER(_ZORDER *p)
{
  p->ftype = ZO_NONE;
  p->no    = 0xFFFF;
}

void free_ZORDER(_ZORDER *p)
{
  p->ftype = ZO_NONE;
  p->no    = 0xFFFF;
//	init_ZORDER(p);speed
}

void init_MEM_ZORDER(_MEM_ZORDER *p)
{
  p->ptr = NULL;
  p->no  = 0;
}

void free_MEM_ZORDER(_MEM_ZORDER *p)
{
  // geschwindigkeitsoptimiert, böse
//  _UWORD i;
//  for(i=0; i<p->no; i++)
//    free_ZORDER(&p->ptr[i]);
  Memory2((void**)&p->ptr, 0);
  p->no  = 0;
//  init_MEM_ZORDER(p);speed
}

void init_THEURGE(_THEURGE *p)
{
  p->urgetype  = _URGE_UNDEFINED;
  p->info = 0xffffffff;
  init_MEM_FONT(&p->memfont);
  init_MEM_ATTRIB(&p->memattrib);
  init_MEM_COLOR(&p->memcolor);
  init_MEM_VARLABEL(&p->memvarlabel);
  init_MEM_ANYTHING(&p->memanything);
  init_MEM_DYNCOLOR(&p->memdyncolor);
  init_MEM_LSEFUNCT(&p->memlsefunct);
  init_MEM_ANYPARA(&p->memanypara);
  init_MEM_BOOL(&p->membool);
  init_MEM_ANYTHING(&p->memimage);

  p->ptr = NULL;
}

void free_THEURGE(_THEURGE *p)
{
  free_MEM_FONT(&p->memfont);
  free_MEM_ATTRIB(&p->memattrib);
  free_MEM_COLOR(&p->memcolor);
  free_MEM_VARLABEL(&p->memvarlabel);
  free_MEM_ANYTHING(&p->memanything);
  free_MEM_DYNCOLOR(&p->memdyncolor);
  free_MEM_LSEFUNCT(&p->memlsefunct);
  free_MEM_ANYPARA(&p->memanypara);
  free_MEM_BOOL(&p->membool);
  free_MEM_ANYTHING(&p->memimage);

  Memory2((void**)&p->ptr, 0);

  p->urgetype  = _URGE_UNDEFINED;
  p->info = 0xffffffff;
//  init_THEURGE(p);speed
}

void init_PICTUREMASK(_PICTUREMASK *pt)
{
  *pt = data_PICTUREMASK;
}

void free_PICTUREMASK(_PICTUREMASK *pt)
{
  // geschwindigkeitsoptimiert, böse

  if(pt->memrectangle.no)
    free_MEM_RECTANGLE(&pt->memrectangle);
  if(pt->mempolygon.no)
    free_MEM_POLYGON(&pt->mempolygon);
  if(pt->memcircle.no)
    free_MEM_CIRCLE(&pt->memcircle);
  if(pt->mempictxt.no)
    free_MEM_PICTEXT(&pt->mempictxt);
  if(pt->memobject.no)
    free_MEM_OBJECT(&pt->memobject);
  if(pt->membitmap.no)
    free_MEM_BITMAP(&pt->membitmap);
  if(pt->meminput.no)
    free_MEM_INPUT(&pt->meminput);
  if(pt->memoutput.no)
    free_MEM_OUTPUT(&pt->memoutput);
  if(pt->memmotion.no)
    free_MEM_MOTION(&pt->memmotion);
  if(pt->colrect.no)
    free_MEM_COL_RECT(&pt->colrect);
  if(pt->colpoly.no)
    free_MEM_COL_POLY(&pt->colpoly);
  if(pt->colcirc.no)
    free_MEM_COL_CIRC(&pt->colcirc);
  if(pt->colpictxt.no)
    free_MEM_COL_PICTXT(&pt->colpictxt);
  if(pt->memkey.no)
    free_MEM_KEY(&pt->memkey);
  if(pt->membutton.no)
    free_MEM_BUTTON(&pt->membutton);
  if(pt->memzorder.no)
    free_MEM_ZORDER(&pt->memzorder);

  free_FAST_ASCII_BUFFER(&pt->enclosed_name);

  init_PICTUREMASK(pt);
}

void init_STYLESHEET(_STYLESHEET *p)
{
  p->frametype          = _DEFFRAME;
  p->contextalign       = DEFATTRIB;
  p->colfront           = DEFCOLOR;
  p->colback            = DEFCOLOR;
  p->colhatch           = DEFCOLOR;
  p->coltopleft         = DEFCOLOR;
  p->colbottomright     = DEFCOLOR;
  p->fontscheme         = 0;
  p->pdyncolback        = NULL;
  p->pdyncolfront       = NULL;
  p->pdyncolhatch       = NULL;
  p->pdyncoltopleft     = NULL;
  p->pdyncolbottomright = NULL;
}

void setup_STYLESHEET(_STYLESHEET *p, _ASCII *fontschemename, _UDWORD frameid, _ATTRIB contextalign, _COLOR colback, _COLOR colfront, _COLOR colhatch, _COLOR coltopleft, _COLOR colbottomright, _LINKSCHEME *pdyncolback, _LINKSCHEME *pdyncolfront, _LINKSCHEME *pdyncolhatch, _LINKSCHEME *pdyncoltopleft, _LINKSCHEME *pdyncolbottomright)
{
  free_STYLESHEET(p);
  
  if(fontschemename != NULL)
    p->fontscheme = (_FONT)FontScheme.GetIndexByLabel(fontschemename);
 #ifdef UC_THEME 
  if((frameid < _THEMEFRAME)||(Theme.Get(frameid) != NULL))
    p->frametype  = frameid;
  else
    p->frametype  = _DEFFRAME;
 #else
  p->frametype  = frameid;
 #endif
 
  p->contextalign       = contextalign;
  
 #ifdef UC_GRAYSCALE
  p->colfront           = transform_to_gray(colfront);
  p->colback            = transform_to_gray(colback);
  p->colhatch           = transform_to_gray(colhatch);
  p->coltopleft         = transform_to_gray(coltopleft);
  p->colbottomright     = transform_to_gray(colbottomright);
 #else
  p->colfront           = colfront;
  p->colback            = colback;
  p->colhatch           = colhatch;
  p->coltopleft         = coltopleft;
  p->colbottomright     = colbottomright;
 #endif 
  
  p->pdyncolback        = pdyncolback;
  p->pdyncolfront       = pdyncolfront;
  p->pdyncolhatch       = pdyncolhatch;
  p->pdyncoltopleft     = pdyncoltopleft;
  p->pdyncolbottomright = pdyncolbottomright;
}

void free_STYLESHEET(_STYLESHEET *p)
{
  init_STYLESHEET(p);
}

void init_PICTURE(_PICTURE *p)
{
  *p = data_PICTURE;
}

void free_PICTURE(_PICTURE *p)
{
  free_CHKBIT(&p->chkbit);
  p->TextPuffer.Free();
  free_PICTUREMASK(&p->mask);
  free_SUBACTUAL(&p->actual);
  free_PICRUNMEMO(&p->picrunmemo);
//  init_PICTURE(p);speed
}

_BOOL chk_VARLABEL(_VARLABEL *p)
{
  return (chk_VARLABEL_Makro(p));
//  return((p->no == 0)? _FALSE : _TRUE);
}

_BOOL is_server_defined(_VARLABEL *p)
{
  if(chk_VARLABEL_Makro(p)) // Makro, speed: nicht auf true oder false abfragen
  {
    if(p->info[0].state == VAR_VAL)
    {
      if(p->info[0].value < VarList.GetNo())
        return _TRUE;
//      return VarList.GetVarInfo(&pvi, p->info[0].value);
    }
  }

  return _FALSE;
}

_BOOL valid_VARLABEL(_VARLABEL *p)
{
  _LSEVARINFO *pvi;
  _BOOL       retcode = _FALSE;

  if(chk_VARLABEL_Makro(p)) // Makro, speed: nicht auf true oder false abfragen
  {
    if(p->info[0].state == VAR_VAL)
    {
      if(VarList.GetVarInfo(&pvi, p->info[0].value) == _TRUE)
      {
        retcode = chk_LASALID_Makro(&pvi->lasalid);
        if(retcode == _FALSE)
        {
          if((pvi->vartype == FCONSTANT)||(pvi->vartype == FMERKER)||(pvi->vartype == FENUMITEM))
            retcode = _TRUE;
        }
      }
    }
  }

  return(retcode);
}
/*
_BOOL valid_LINKSCHEME(_LINKSCHEME *p)
{
  if(p->myscheme.schemeno)
}
*/


void set_lrm(_RESULT *pr, _BOOL lrm)
{
  pr->remotedata = (lrm == _TRUE)? 1 : 0;
}

void init_VARLABEL(_VARLABEL *vl)
{
  init_VARLABEL_Makro(vl);
  //*vl = data_VARLABEL;
}

void free_VARLABEL(_VARLABEL *vl)
{
  vl->no = 0;
//  init_VARLABEL(vl); speed
}

void init_VARIABLE(_VARIABLE *v)
{
  init_VARIABLE_Makro(v);
  //*v = data_VARIABLE;
//	v->no = DEFSCOPE;
//	init_VARINFO_Makro(&v->info);
}

void free_VARIABLE(_VARIABLE *v)
{
	free_VARINFO(&v->info);
	v->no = DEFSCOPE;
//	init_VARIABLE(v);speed
}

void init_OBJECT(_OBJECT *p)
{
  *p = data_OBJECT;
}

void free_OBJECT(_OBJECT *p)
{
  free_CHKBIT(&p->chkbit);
  free_ZOOM(&p->zoom);

  if(p->SubPicture != NULL)
  {
    p->SubPicture->Free();
    Memory2((void**)&p->SubPicture, 0);
  }

  free_IMAGE(&p->image);
  free_VARLABEL(&p->loader);
  free_MYSCHEME(&p->loader_scheme);
  free_MYSCHEME(&p->stscheme);
  free_DIRECTION(&p->direction);
 #ifdef UC_CHANGE_IO_GROUP
  free_IOGRPCHX(&p->imiogroupchanged);
 #endif

  init_OBJECT(p);
}

void init_RESULT(_RESULT *p)
{
  //urinit_RESULT(p);
  init_RESULT_Makro(p);
  //*p = data_RESULT;
}

void init_SOFTFLOOR(_SOFTFLOOR *pf)
{
  _UWORD i;

  for(i=0; i<NO_SOFT_FLOOR; i++)
    init_SINGLESOFT(&pf->floor[i]);
}

void free_SOFTFLOOR(_SOFTFLOOR *pf)
{
  _UWORD i;

  for(i=0; i<NO_SOFT_FLOOR; i++)
    free_SINGLESOFT(&pf->floor[i]);
}

void init_SINGLESOFT(_SINGLESOFT *p)
{
  *p = data_SINGLESOFT;
}

void free_SINGLESOFT(_SINGLESOFT *p)
{
  free_ANYPARA(&p->anypara);
  free_CHKBIT(&p->chkbit);
  free_GFUNCT(&p->gfunct);
  free_BUTTONIMAGES(&p->buttonimages);
  free_VARLABEL(&p->varlabel);
  free_CSTATE(&p->cstate);

 #ifdef UC_DYN_SOFTKEYS
  free_LINKSCHEME(&p->dyncolback);
  free_LINKSCHEME(&p->dyncolfront);
  free_LINKSCHEME(&p->dyncoltl);
  free_LINKSCHEME(&p->dyncolbr);
  free_LINKSCHEME(&p->dyncoltxt);
  free_MYSCHEME(&p->stscheme);
 #endif

  free_ANYPARA(&p->newbubbletext);

  *p = data_SINGLESOFT;

//  init_SINGLESOFT(p); // speed
}

void init_INPUT(_INPUT *p)
{
  //urinit_INPUT(p);
  //*p = data_INPUT;
  init_INPUT_Makro(p);
}

void free_INPUT(_INPUT *p)
{
	free_IO(&p->io);
  free_COMBOINFO(&p->comboinfo);
  p->enter_plus  = _EP_NONE;
  p->memseekno   = 0xFFFF;
  p->searchchoice = 0;
//  init_INPUT(p);speed
}

void init_IO(_IO *p)
{
  //urinit_IO(p);
  //*p = data_IO;
  init_IO_Makro(p);
}

void free_IO(_IO *p)
{
  if(p->Obj != NULL)
  {
    p->Obj->Free();
    Memory2((void**)&p->Obj, 0);
  }

//  Memory2((void**)&p->theurge, 0);
  free_CSTATE(&p->cstate);
  free_CHKBIT(&p->chkbit);
  free_VIRTNAME(&p->virtname);
  free_PICFORMAT(&p->picformat);
  free_VARLABEL(&p->variable);
  free_IOINFO(&p->info);
  
  if(p->imagecopy == _FALSE)
  {
    free_IMAGE(&p->image);
  }
  else
  {
    init_IMAGE_Makro(&p->image);
  }
  // free_WORKSTATE(&p->workstate);
  free_LINKSCHEME(&p->dyncolback);    // color background (hatch)
  free_LINKSCHEME(&p->dyncolfront);   // color background (hatch)
  free_LINKSCHEME(&p->dyncoltl);      // color frame (top-left)
  free_LINKSCHEME(&p->dyncolbr);      // color frame (bottom-right)
  free_LINKSCHEME(&p->dyncoltxt);     // color text
  free_MYSCHEME(&p->stscheme);
  free_LSEFUNCT(&p->lsefunct);
  free_DIRECTION(&p->direction);
  free_VARLABEL(&p->imoverloaded);
 #ifdef UC_CHANGE_IO_GROUP
  free_IOGRPCHX(&p->imiogroupchanged);
 #endif

  if(p->theurge != NULL)
  {
    free_THEURGE(p->theurge);
    Memory2((void**)&p->theurge, 0);
  }

  free_ANYPARA(&p->newbubbletext);

  init_IO_Makro(p);
}

void init_COMBOINFO(_COMBOINFO *p)
{
  p->lineheight   = 0;    // default
  p->frame        = 0xFF; // default
  p->linesperpage = 5;    // default
}

void free_COMBOINFO(_COMBOINFO *p)
{
  init_COMBOINFO(p);
}

void init_IOINFO(_IOINFO *p)
{
  //*p = data_IOINFO;
  init_IOINFO_Makro(p);
}

void free_IOINFO(_IOINFO *p)
{
  free_ANYTHING(&p->tbo);
  free_MYSCHEME(&p->myscheme);
  free_MYTXTSCHEME(&p->mytxtscheme);
  
  if(p->pcombo != NULL)
  {
    if(p->combocopy == _FALSE)
    {
      free_COMBOBOX(p->pcombo);
      Memory2((void**)&p->pcombo, 0);
    }
  }
  
  p->combocopy = _FALSE;
  p->pcombo = NULL;
//  init_IOINFO_Makro(p);speed
}

void init_CSTATE(_CSTATE *p)
{
  p->init  = _TRUE;
  p->value = 0x7FFFFFFFL;
}

void  free_CSTATE(_CSTATE *p)
{
  p->init  = _TRUE;
  p->value = 0x7FFFFFFFL;
//  init_CSTATE(p);speed
}

void init_ZOOM(_ZOOM *p)
{
  p->x = DEFZOOM;
  p->y = DEFZOOM;
}

void free_ZOOM(_ZOOM *p)
{
  p->x = DEFZOOM;
  p->y = DEFZOOM;
//  init_ZOOM(p);speed
}

void  init_PRJ_ENTRY(_PRJ_ENTRY *p)
{
//  p->state   = _FALSE;
//  p->window  = _FALSE;
  p->ftype      = _PE_NONE;
  p->no         = 0xFFFF;
  p->nextno     = 0xFFFF;
  p->name[0]    = 0;
  p->usedintern = 0;
  p->access     = 0;
  p->imageno    = 0xffff;
  p->refcyc     = 0;
  init_CHKBIT_Makro(&p->chkbit);
  init_ANYPARA_Makro(&p->anypara);
 #ifdef XUSER_KM
  p->softinfo   = 0xFFFF;
 #endif
  p->time_max   = -1;
  p->time_last  = -1;
  p->label[0]   = 0;
}

void  free_PRJ_ENTRY(_PRJ_ENTRY *p)
{
  free_CHKBIT(&p->chkbit);
  free_ANYPARA(&p->anypara);
  init_PRJ_ENTRY(p);
}

void init_STACK_LINE(_STACK_LINE *p)
{
  init_STACK_LINE_Makro(p);
  //*p = data_STACK_LINE;
}

void free_STACK_LINE(_STACK_LINE *p)
{
  free_VARLABEL(&p->op1);
  free_VARLABEL(&p->op2);
  init_STACK_LINE_Makro(p);
//  init_STACK_LINE(p);speed
}

void init_EVENT(_EVENT *p)
{
  init_EVENT_Makro(p);
  // *p = data_EVENT;
}

void free_EVENT(_EVENT *p)
{
  init_EVENT_Makro(p);
  //init_EVENT(p);
}

void init_WHOAMI(_WHOAMI *pt)
{
  pt->ima = IMA_PICTURE;
  pt->no  = 222112;
}

void free_WHOAMI(_WHOAMI *pt)
{
  init_WHOAMI(pt);
}

void setup_WHOAMI(_WHOAMI *pt, _IMA ima, _UDWORD no)
{
  // init_WHOAMI(pt); speed
  pt->ima = ima;
  pt->no  = no;
}

void init_NEWSCREENPROP(_NEWSCREENPROP *p)
{
  p->Position.xy1.x     = 100;
  p->Position.xy1.y     = 100;
  p->Position.xy2.x     = 200;
  p->Position.xy2.y     = 200;
  p->ColorBack          = LIGHTGRAY;
  p->ColorHatch         = LIGHTBLUE;
  p->ColorTopLeft       = WHITE;
  p->ColorBottomRight   = DARKGRAY;
  p->FrameId            = _3DFRAME;
  init_DOT(&p->FirstInput);
  p->DrawShadow         = _TRUE;
  p->TouchEditorNumeric = _TRUE;
  p->TouchEditorAscii   = _TRUE;
  p->GlobalHotkeys      = _TRUE;
}

void free_NEWSCREENPROP(_NEWSCREENPROP *p)
{
  init_NEWSCREENPROP(p);
}



void init_FONTINFO(_FONTINFO *pt)
{
  pt->ptr    = NULL;
  pt->width  = 0;
  pt->height = 0;
  pt->size   = 0;
  pt->isheap = _TRUE;
  pt->attrib = T_COPY|T_SOLID|T_PROP;
}

void free_FONTINFO(_FONTINFO *pt)
{
  if(pt->isheap == _TRUE)
    Memory2((void**)&pt->ptr, 0);
  init_FONTINFO(pt);
}

void init_ACTUAL(_ACTUAL *pt)
{
  pt->actlayer       = 0;
  pt->soft_floor     = 0;
  pt->language       = 0;
  pt->actrefresh_cnt = 0;
}

void free_ACTUAL(_ACTUAL *pt)
{
  pt->actlayer       = 0;
  pt->soft_floor     = 0;
  pt->language       = 0;
  pt->actrefresh_cnt = 0;
//  init_ACTUAL(pt);speed
}

void init_SUBACTUAL(_SUBACTUAL *p)
{
  *p = data_SUBACTUAL;
}

void free_SUBACTUAL(_SUBACTUAL *p)
{
  *p = data_SUBACTUAL;
//  init_SUBACTUAL(p);speed
}

_BOOL chk_ROOM(_ROOM *p)
{
  return(((p->xy1.y == p->xy2.y)||(p->xy2.x == p->xy1.x))? _FALSE : _TRUE);
}

void init_EDITOR(_EDITOR *p)
{
  p->actlayer = 0;
  p->actno    = 0xFFFF;
  p->incdec   = 1;
  p->timer    = 0;

  init_VARIABLE_Makro(&p->variable);
  init_INPUT_Makro(&p->input);
  init_RESULT_Makro(&p->result);
  init_CHREDIT(&p->chredit);
  init_BUBBLEHELP(&p->bubblehelp);
  init_MENCURSOR(&p->mencursor);

  p->enterdirection          = _FALSE;
  p->disable_direction_enter = _FALSE;
  p->japan_input_mode        = 0;
//  p->japan_count_ENTER       = 0;
//  p->japan_count_SPACE       = 0;
//  p->japan_pos_dictionary    = 0;
//  p->japan_keyword[0]        = 0;
}

void free_EDITOR(_EDITOR *p)
{
  init_VIRTNAME(&p->input.io.virtname); // don't call method if_end

  free_VARIABLE(&p->variable);
  free_INPUT(&p->input);
  free_CHREDIT(&p->chredit);
//  free_RESULT(&p->result);
	free_BUBBLEHELP(&p->bubblehelp);
  free_MENCURSOR(&p->mencursor);

  p->actlayer = 0;
  p->actno    = 0xFFFF;
  p->incdec   = 1;
  p->timer    = 0;

  init_RESULT_Makro(&p->result);

  p->enterdirection          = _FALSE;
  p->disable_direction_enter = _FALSE;
  p->japan_input_mode        = 0;
//  p->japan_count_ENTER       = 0;
//  p->japan_count_SPACE       = 0;
//  p->japan_pos_dictionary    = 0;
//  p->japan_keyword[0]        = 0; 
//	init_EDITOR(p);speed
}

void init_MULTIKEYBOARD(_MULTIKEYBOARD *ptr, _BOOL urinit)
{
  ptr->key = 0;
  ptr->timestamp = 0xFFFFFFFF;
  ptr->count = 0;
  
  if(urinit == _TRUE)
  {
    ptr->delaytime = 0;
    ptr->no = 0;
    ptr->keytab = NULL;
  }
}

void free_MULTIKEYBOARD(_MULTIKEYBOARD *ptr)
{
  init_MULTIKEYBOARD(ptr, _TRUE);
}

void init_IMAGE(_IMAGE *p)
{
  //*p = data_IMAGE;
  init_IMAGE_Makro(p);
}

void free_IMAGE(_IMAGE *p)
{
  // Versionsabfrage funktioniert bei LARS nicht !!!
  // Debug(lsl_pOS->piLSE->version, 1);
  // Debug(lsl_pOS->piLSE->size, 2);

	Memory2((void**)&p->ptr, 0);
	init_ROOM_Makro(&p->room);
//	init_IMAGE_Makro(p);speed
}

void init_POSITION(_POSITION *p)
{
	p->pos      = 0;
	p->begin    = 0;
	p->no       = 0;
	p->height   = 5;
	p->width    = 0;
	p->oldpos   = 0;
	p->oldbegin = 0;
  p->lineheight = NULL;
  p->totalheight = 0;
}

void free_POSITION(_POSITION *p)
{
  Memory((void**)&p->lineheight, 0);
	init_POSITION(p);
}

void init_MENUCALCUL(_MENUCALCUL *p)
{
  init_ROOM_Makro(&p->out_room);
  init_ROOM_Makro(&p->scroll_room);
  init_LEVEL(&p->level);

  p->char_width  = 1;
  p->char_height = 1; // character height
}

void free_MENUCALCUL(_MENUCALCUL *p)
{
  free_LEVEL(&p->level);
  init_MENUCALCUL(p);
}

void init_MENCURSOR(_MENCURSOR *p)
{
  p->delay_timer  = 0;
  p->delay_active = _FALSE;
  p->active       = _TRUE;
}

void free_MENCURSOR(_MENCURSOR *p)
{
  init_MENCURSOR(p);
}

void init_MENU(_MENU *p)
{
  p->PScreenObject = NULL;
  init_MENUPOINTER(&p->pointer);
  init_POSITION(&p->position);
  init_MENUCALCUL(&p->calcul);
  init_MENUINFO(&p->info);
  init_MENCURSOR(&p->mencursor);
}

void free_MENU(_MENU *p)
{
  free_MENUPOINTER(&p->pointer);
  free_POSITION(&p->position);
  free_MENUCALCUL(&p->calcul);
  free_MENUINFO(&p->info);
  free_MENCURSOR(&p->mencursor);

  init_MENU(p);
}

void init_LSEMENU(_LSEMENU *p)
{
  init_MENUINFO(&p->info);
  init_MEM_LINEINFO(&p->mem_lineinfo);
  p->TextPuffer.Init();
  p->lsemenuindex = DEFSCOPE;
}

void free_LSEMENU(_LSEMENU *p)
{
  free_MENUINFO(&p->info);
  free_MEM_LINEINFO(&p->mem_lineinfo);
  p->TextPuffer.Free();

  init_LSEMENU(p);
}

void init_CAROITEM(_CAROITEM *p)
{
  p->objno = 0xFFFF;
  init_VARLABEL_Makro(&p->loader);
}

void init_KARU(_KARU *p)
{
  p->objno = 0xFFFF;
  init_VARLABEL_Makro(&p->loader);
  p->SubPicture = NULL;
  init_ZOOM(&p->zoom);
  init_ROOM_Makro(&p->room);
  init_ROOM_Makro(&p->room0);
  p->onscreen = _FALSE;
  p->gesamt = _FALSE;
}

void free_KARU(_KARU *p)
{
  free_VARLABEL(&p->loader);
  if(p->SubPicture != NULL)
  {
    p->SubPicture->Free();
    Memory2((void**)&p->SubPicture, 0);
  }
  
  free_ZOOM(&p->zoom);
  init_ROOM_Makro(&p->room);
  
  init_KARU(p);
}

void init_MEM_KARU(_MEM_KARU *p)
{ 
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_KARU(_MEM_KARU *p)
{ 
  _UWORD i;
  
  if(p->ptr != NULL)
  {
    for(i=0; i<p->no; i++)
      free_KARU(&p->ptr[i]);
    Memory2((void**)&p->ptr, 0);
  }
  init_MEM_KARU(p);
}

void init_SCROLLBAR(_SCROLLBAR *p)
{
  init_ROOM(&p->room);       // position
  p->col_back   = LIGHTGRAY;   // color
  p->col_front  = LIGHTGRAY;
  p->col_frame  = MakeColor(DARKGRAY, WHITE);  // color
  p->col_text   = BLACK;   // color
  p->frametype  = _3DFRAME;  // frametype
  p->horizontal = _FALSE; // horizontal true/false
  p->valid      = _FALSE;      // gültig true/false
  p->width      = 0;      // anzahl elemente
  p->pos        = 0;        // position akt.element
  p->oldwidth   = 0;   // referenz anzahl elemente
  p->oldpos     = 0;     // referenz position akt.element
  p->active     = 0;
  p->active_dot.x = 0;
  p->active_dot.y = 0;
}

void free_SCROLLBAR(_SCROLLBAR *p)
{
  init_SCROLLBAR(p);
}

extern "C" void init_MOTION(_MOTION *p)
{
  init_CHKBIT_Makro(&p->chkbit);
  init_ROOM_Makro(&p->room);
  init_ANYTHING_Makro(&p->anything);
  init_VARLABEL_Makro(&p->varx);
  init_VARLABEL_Makro(&p->vary);
  init_IMAGE_Makro(&p->image);
  init_WORKSTATE(&p->workstate);
  init_DOT(&p->oldxy);
  init_VARLABEL_Makro(&p->loader);
  init_DIRECTION(&p->direction);
  init_MYSCHEME_Makro(&p->stscheme);
  init_ZOOM(&p->zoom);
  p->summary  = DEFSUMMARY;
  p->movstate = _FALSE;
  init_DOT(&p->move);
  init_DOT(&p->movstart);

  p->colback    = MakeColor_Makro(INVISIBLE, INVISIBLE);
  init_LINKSCHEME_Makro(&p->dyncolback);  // dynamic color background
  init_LINKSCHEME_Makro(&p->dyncolhatch); // dynamic color background (hatch)

  p->SubPicture    = NULL;
  p->attrib        = DEFATTRIB;
  p->onscreen      = _FALSE;
  p->swim          = _FALSE;
  p->positiv_dir   = _TRUE; // true wenn nach rechts oder unten "gemoved" wurde
  p->usermove      = _FALSE;
  p->overmove      = _TRUE; // carousell am anfang und ende weitermoven
  p->movbegin      = _FALSE;
  
  init_MEM_KARU(&p->memkaru);
  p->gap           = 0;
  p->horizontal    = _TRUE;
  p->infinite      = _FALSE;
  p->karuwidth     = 0;
  p->widthlastpage = 0;
  p->skip_percent  = 50;
  p->maxmovno      = 0;  
  p->movespeed     = 15;  
  
 #ifdef UC_OLD_MOTIONS 
  p->width = 0;
  p->height = 0;
 #endif
  init_VARLABEL(&p->varindex);
  p->memoindex     = 0;
  p->frametype     = _DEFFRAME;
  init_ROOM_Makro(&p->wiperoom);
  init_SCROLLBAR(&p->scrbar);
}

extern "C" void free_MOTION(_MOTION *p)
{
  free_CHKBIT(&p->chkbit);
//  free_ROOM(&p->room);
	free_ANYTHING(&p->anything);
  free_VARLABEL(&p->varx);
  free_VARLABEL(&p->vary);
  free_IMAGE(&p->image);
  free_VARLABEL(&p->loader);
  free_DIRECTION(&p->direction);
  free_MYSCHEME(&p->stscheme);

  free_LINKSCHEME(&p->dyncolback);  // dynamic color background (hatch)
  free_LINKSCHEME(&p->dyncolhatch); // dynamic color background (hatch)

  if(p->SubPicture != NULL)
  {
    p->SubPicture->Free();
    Memory2((void**)&p->SubPicture, 0);
  }

  free_MEM_KARU(&p->memkaru);
  free_SCROLLBAR(&p->scrbar);

  init_MOTION(p);
}

void init_CHREDIT(_CHREDIT *p)
{
  p->fstring[0]  = 0;
  p->pos         = 0;
  p->max         = 0;
  p->no          = 0;
  p->change      = _FALSE;
  p->cursory     = 0;
  p->chr         = ' ';
  p->incdec      = _TRUE;
  p->editshuttle = _FALSE;
  p->timer_cursorjump = 0;
//  p->japan_pos    = 0;
//  p->japan_change = 0;
  p->japan_begin_pos = 0;
}

void free_CHREDIT(_CHREDIT *p)
{
  init_CHREDIT(p);
}

void init_COL_RECT(_COL_RECT *p)
{
  init_RECTANGLE(&p->rectangle);
  init_LINKSCHEME_Makro(&p->dyncolback);         // color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncolfront);        // color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncoltl);           // color frame (top-left)
  init_LINKSCHEME_Makro(&p->dyncolbr);           // color frame (bottom-right)
  p->refcolor      = DEFCOLOR;           // reference
  p->refcolorframe = DEFCOLOR;           // reference
  p->init          = _TRUE;
}

void free_COL_RECT(_COL_RECT *p)
{
  free_RECTANGLE(&p->rectangle);
  free_LINKSCHEME(&p->dyncolback);         // color background (hatch)
  free_LINKSCHEME(&p->dyncolfront);        // color background (hatch)
  free_LINKSCHEME(&p->dyncoltl);           // color frame (top-left)
  free_LINKSCHEME(&p->dyncolbr);           // color frame (bottom-right)
  p->refcolor      = DEFCOLOR;           // reference
  p->refcolorframe = DEFCOLOR;           // reference
  p->init          = _TRUE;
//  init_COL_RECT(p);speed
}

void init_COL_POLY(_COL_POLY *p)
{
  init_POLYGON(&p->poly);
  init_LINKSCHEME_Makro(&p->dyncolback);         // color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncolfront);        // color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncolframe);        // color frame
  p->refcolor      = DEFCOLOR;           // reference
  p->refcolorframe = DEFCOLOR;           // reference
  p->init          = _TRUE;
}

void free_COL_POLY(_COL_POLY *p)
{
  free_POLYGON(&p->poly);
  free_LINKSCHEME(&p->dyncolback);         // color background (hatch)
  free_LINKSCHEME(&p->dyncolfront);        // color background (hatch)
  free_LINKSCHEME(&p->dyncolframe);        // color frame
  p->refcolor      = DEFCOLOR;           // reference
  p->refcolorframe = DEFCOLOR;           // reference
  p->init          = _TRUE;
//  init_COL_POLY(p);speed
}

void init_COL_CIRC(_COL_CIRC *p)
{
  init_CIRCLE(&p->circle);
  init_LINKSCHEME_Makro(&p->dyncolback);         // color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncolfront);        // color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncolframe);        // color frame
  p->refcolor      = DEFCOLOR;           // reference
  p->refcolorframe = DEFCOLOR;           // reference
  p->init          = _TRUE;
}

void free_COL_CIRC(_COL_CIRC *p)
{
  free_CIRCLE(&p->circle);
  init_LINKSCHEME_Makro(&p->dyncolback);         // color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncolfront);        // color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncolframe);        // color frame
  p->refcolor      = DEFCOLOR;           // reference
  p->refcolorframe = DEFCOLOR;           // reference
  p->init          = _TRUE;
//  init_COL_CIRC(p);speed
}

void init_COL_PICTXT(_COL_PICTXT *p)
{
  init_PICTEXT(&p->btx);
  init_LINKSCHEME_Makro(&p->dyncolback);         // color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncolfront);        // color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncoltxt);          // color text
  init_LINKSCHEME_Makro(&p->dyncoltl);           // color frame (top-left)
  init_LINKSCHEME_Makro(&p->dyncolbr);           // color frame (bottom-right)
  p->refcolback  = DEFCOLOR;         // color background (hatch)
  p->refcolframe = DEFCOLOR;        // color frame (top-left, bottom-right)
  p->refcoltxt   = DEFCOLOR;          // color text
  p->init        = _TRUE;

}

void free_COL_PICTXT(_COL_PICTXT *p)
{
  free_PICTEXT(&p->btx);
  free_LINKSCHEME(&p->dyncolback);         // color background (hatch)
  free_LINKSCHEME(&p->dyncolfront);        // color background (hatch)
  free_LINKSCHEME(&p->dyncoltxt);          // color text
  free_LINKSCHEME(&p->dyncoltl);           // color frame (top-left)
  free_LINKSCHEME(&p->dyncolbr);           // color frame (bottom-right)

  p->refcolback  = DEFCOLOR;         // color background (hatch)
  p->refcolframe = DEFCOLOR;        // color frame (top-left, bottom-right)
  p->refcoltxt   = DEFCOLOR;          // color text
  p->init        = _TRUE;
//  init_COL_PICTXT(p);speed
}

void init_MEM_COL_RECT(_MEM_COL_RECT *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_COL_RECT(_MEM_COL_RECT *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
    free_COL_RECT(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
  // init_MEM_COL_RECT(pt); speed
}

void init_MEM_COL_POLY(_MEM_COL_POLY *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_COL_POLY(_MEM_COL_POLY *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
    free_COL_POLY(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
//  init_MEM_COL_POLY(pt);speed
}

void init_MEM_COL_CIRC(_MEM_COL_CIRC *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_COL_CIRC(_MEM_COL_CIRC *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
    free_COL_CIRC(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
//  init_MEM_COL_CIRC(pt);speed
}

void init_MEM_COL_PICTXT(_MEM_COL_PICTXT *pt)
{
  pt->no  = 0;
  pt->ptr = NULL;
}

void free_MEM_COL_PICTXT(_MEM_COL_PICTXT *pt)
{
  _UWORD i;

  for(i=0; i<pt->no; i++)
    free_COL_PICTXT(&pt->ptr[i]);

  Memory2((void**)&pt->ptr, 0);

  pt->no  = 0;
//  init_MEM_COL_PICTXT(pt);speed
}

void init_BMP(_BMP *p)
{
  //*p = data_BMP;
  init_BMP_Makro(p);
}

void free_BMP(_BMP *p)
{
  Memory2((void**)&p->ptr, 0);
  //*p = data_BMP;
  init_BMP_Makro(p);
}

void init_LSEFUNCT(_LSEFUNCT *p)
{
  //*p = data_LSEFUNCT;
  init_LSEFUNCT_Makro(p);
}

void free_LSEFUNCT(_LSEFUNCT *p)
{
  free_STACK(&p->overload);
  p->no = 0xFFFF;
  // init_LSEFUNCT(p);speed
}

void init_GFUNCT(_GFUNCT *p)
{
  //*p = data_GFUNCT;
  init_GFUNCT_Makro(p);
}

void free_GFUNCT(_GFUNCT *p)
{
  free_LSEFUNCT(&p->pos_fl);
  free_LSEFUNCT(&p->neg_fl);

//  init_GFUNCT_Makro(p);speed
}

void init_KEY(_KEY *p)
{
  init_GFUNCT_Makro(&p->gfunct);
  p->tip  = _TIPSINGLE;
  p->code = 0;
}

void free_KEY(_KEY *p)
{
  free_GFUNCT(&p->gfunct);
  p->tip  = _TIPSINGLE;
  p->code = 0;
//  init_KEY(p);speed
}

void init_MEM_KEY(_MEM_KEY *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_KEY(_MEM_KEY *p)
{
  _UWORD i;
  for(i=0; i<p->no; i++)
    free_KEY(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);
  p->no  = 0;
//  init_MEM_KEY(p);speed
}

void init_UNIT(_UNIT *p)
{
  init_VARLABEL_Makro(&p->xdigits);
  p->xdigits.no            = 1;
  p->xdigits.info[0].state = CONST_VAL;
  p->xdigits.info[0].value = 5;
  p->formula = DEFFORMULA;
  init_VARLABEL_Makro(&p->posdezpt);
  init_VARLABEL_Makro(&p->parameter);
  init_ANYPARA_Makro(&p->anypara);
  p->fixdp   = 1;
  p->leading0 = 0;
}

void free_UNIT(_UNIT *p)
{
  free_VARLABEL(&p->xdigits);
  free_VARLABEL(&p->posdezpt);
  free_VARLABEL(&p->parameter);
  free_ANYPARA(&p->anypara);
  init_UNIT(p);
}

void init_MEM_UNIT(_MEM_UNIT *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_UNIT(_MEM_UNIT *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_UNIT(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);
  p->no  = 0;
//  init_MEM_UNIT(p);speed
}

void init_TIME(_TIME *p)
{
  //p->hour   = 0;
  //p->minute = 0;
  //p->second = 0;
  init_TIME_Makro(p);
}

void init_DATE(_DATE *p)
{
  //p->day       = 0;
  //p->month     = 0;
  //p->year      = 0;
  //p->dayofweek = 0;
  init_DATE_Makro(p);
}

void init_DATIM(_DATIM *p)
{
//  init_DATE(&p->fdate);
//  init_TIME(&p->ftime);
  init_DATE_Makro(&p->fdate);
  init_TIME_Makro(&p->ftime);
}

void init_SCAN(_SCAN *p)
{
  p->i     = 0;
  p->max   = 0;
  p->oldx  = 0;
  p->color = MakeColor_Makro(BLACK, LIGHTGRAY);
  p->text  = NULL;
  init_ROOM_Makro(&p->room);
}

void free_SCAN(_SCAN *p)
{
  if(p->text != NULL)
  {
    Memory2((void**)&p->text, 0);
  }  
  init_SCAN(p);
}

void init_LASALID(_LASALID *p)
{
  p->id    = DEFLASALID;
  p->vtype = TYP_STANDARD;
}

_BOOL chk_LASALID(_LASALID *p)
{
  return chk_LASALID_Makro(p);
  //return((p->id != DEFLASALID)? _TRUE:_FALSE);
}

void init_COMDEF(_COMDEF *p)
{
  MemSet(p, 0, sizeof(_COMDEF));
  p->typ      = _COM_LOCAL;
  p->adress   = 0xFFFF;
  p->handle   = NULL;
  p->ipadress = 0xFFFFFFFF;
  p->port     = 0;
}

void free_COMDEF(_COMDEF *p)
{
  init_COMDEF(p);
}

void init_PICFORMAT(_PICFORMAT *p)
{
  p->format = 0;
  p->digits = 0;
  init_VARLABEL_Makro(&p->posdezpt);
}

void free_PICFORMAT(_PICFORMAT *p)
{
  free_VARLABEL(&p->posdezpt);
  p->format = 0;
  p->digits = 0;
//  init_PICFORMAT(p);speed
}

void init_LEVEL(_LEVEL *p)
{
  p->active        = _FALSE;
  init_ROOM_Makro(&p->room);
  p->color         = MakeColor_Makro(LIGHTGRAY, LIGHTGRAY);
  p->frame_color   = MakeColor_Makro(DARKGRAY, WHITE); //DEFCOLOR;
  p->old_pos       = -1;
  p->trigger_state = _FALSE;
  p->drag          = _FALSE;
  p->pguppgdown    = _FALSE;
}

void free_LEVEL(_LEVEL *p)
{
  init_LEVEL(p);
}

void init_EVENTMEMO(_EVENTMEMO *p)
{
  init_DOT(&p->HidMemoLeft);
  init_DOT(&p->HidMemoRight);
  init_EVENT_Makro(&p->TriggerEvent);
  p->TriggerEventState = _FALSE;
  p->TriggerEventTimex = 0;
  p->TriggerEventTimexDuration = 0;
  p->PressMemoLeft  = _FALSE;
  p->PressMemoRight = _FALSE;
}

void free_EVENTMEMO(_EVENTMEMO *p)
{
  init_EVENTMEMO(p);
}

void init_MEM_VARLABEL(_MEM_VARLABEL *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_VARLABEL(_MEM_VARLABEL *p)
{
  _UWORD i;
  _VARLABEL *pvl;

  if(p->no != 0)
  {
    pvl = p->ptr;
    for(i=0; i<p->no; i++, pvl++)
      free_VARLABEL(pvl);
    p->no = 0;
  }

  if(p->ptr)
    Memory2((void**)&p->ptr, 0);

//  init_MEM_VARLABEL(p);speed
}

void init_LINEINFO(_LINEINFO *p)
{
  init_CHKBIT_Makro(&p->chkbit);
  init_VARLABEL_Makro(&p->value);
  init_GFUNCT_Makro(&p->gfunct);
  init_MEM_VARLABEL(&p->memvar);
  init_ANYPARA_Makro(&p->anypara);
  init_MYSCHEME_Makro(&p->stscheme);
}

void free_LINEINFO(_LINEINFO *p)
{
  free_CHKBIT(&p->chkbit);
  free_VARLABEL(&p->value);
  free_GFUNCT(&p->gfunct);
  free_MEM_VARLABEL(&p->memvar);
  free_ANYPARA(&p->anypara);
  free_MYSCHEME(&p->stscheme);
//  init_LINEINFO(p);speed
}

void init_MEM_LINEINFO(_MEM_LINEINFO *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_LINEINFO(_MEM_LINEINFO *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_LINEINFO(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);

  p->no  = 0;
//  init_MEM_LINEINFO(p);speed
}

_BOOL chk_LINEINFO(_LINEINFO *p)
{
  return(chk_CHKBIT(&p->chkbit));
}

_UDWORD state_LINEINFO(_LINEINFO *p)
{
//#define STATE_ACTIVE              0x00
//#define STATE_INACTIVE            0x01
//#define STATE_INVISIBLE           0x02

  _UDWORD tmp;
  _UDWORD retcode = STATE_INVISIBLE;
  
  if(chk_CHKBIT(&p->chkbit) == _TRUE)
  {
    retcode = STATE_ACTIVE;
    if(StateScheme.Call(&tmp, &p->stscheme, NULL, STATE_ACTIVE) == _TRUE)  
      retcode = (_UWORD)tmp;
  }
  return retcode;
}




void init_MENUINFO(_MENUINFO *p)
{
  p->font         = DEFFONT;
  p->attrib       = T_SOLID|T_COPY|T_PROP|T_LEFTBOUND|T_MIDBOUND;
  p->xy.x         = 0;
  p->xy.y         = 0;
  p->line_height  = 0;
  p->width        = 10;
  p->height       = 10;
  p->text_color   = MakeColor_Makro(LIGHTGRAY, BLACK);         // active line color
  p->bar_color    = MakeColor_Makro(BLUE, WHITE);                      // bar color
  p->frame_color  = MakeColor_Makro(DARKGRAY, WHITE);                // frame color
  init_VARLABEL_Makro(&p->server);
  p->frametype    = _DEFFRAME;
  p->basement     = 0;     // height of basement (freespace)
  p->inactive_text_color = MakeColor_Makro(DARKGRAY, BLACK);         // inactive line color
  p->draw_shadow  = _TRUE; // erst seit sa34116, compilerversion 60 vorhanden

//  p->Title.Init();
}

void free_MENUINFO(_MENUINFO *p)
{
//  p->Title.Free();

  free_VARLABEL(&p->server);
  init_MENUINFO(p);
}

void init_MEM_LSEMENU(_MEM_LSEMENU *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_LSEMENU(_MEM_LSEMENU *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_LSEMENU(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);

  p->no  = 0;
//  init_MEM_LSEMENU(p);speed
}

void init_MENUPOINTER(_MENUPOINTER *p)
{
  p->PtrLine     = NULL;             // --> to userdefined linedraw function
  p->PtrStart    = NULL;                // --> to userdefined init-interface
  p->PtrReady    = NULL;               // --> to userdefined ready-interface
  p->PtrRun      = NULL;                 // --> to userdefined run-interface
  p->PtrEnd      = NULL;                 // --> to userdefined end-interface
  p->PtrData     = NULL;                          // --> to userdefined data
  p->PtrSubList  = NULL;                              // --> to sublist-data
  p->Ptr_LSEMENU = NULL;
  p->Level       = _FALSE;
}

void free_MENUPOINTER(_MENUPOINTER *p)
{
  if(p->Ptr_LSEMENU != NULL)
  {
    free_LSEMENU(p->Ptr_LSEMENU);
    Memory2((void**)&p->Ptr_LSEMENU, 0);
  }

  init_MENUPOINTER(p);
}

void init_WORKSTATE(_WORKSTATE *p)
{
	*p = STATE_START;
}

void init_BUTTON(_BUTTON *p)
{
  //urinit_BUTTON(p);
  *p = data_BUTTON;
}

void free_BUTTON(_BUTTON *p)
{
  free_ANYPARA(&p->text);   // released text
  free_ANYPARA(&p->text1);  // pressed text
  free_VARLABEL(&p->varlabel);
  free_CHKBIT(&p->chkbit);
  free_GFUNCT(&p->gfunct);
  free_ANYTHING(&p->anything);
  free_VARLABEL(&p->loader);
//  free_SUBACTUAL(&p->actual);
  free_DIRECTION(&p->direction);
  free_BUTTONIMAGES(&p->buttonimages);
  if(p->SubPicture != NULL)
  {
    p->SubPicture->Free();
    Memory2((void**)&p->SubPicture, 0);
  }

  free_LINKSCHEME(&p->dyncolback);     // dynamic color background (hatch)
  free_LINKSCHEME(&p->dyncolfront);    // dynamic color background (hatch)
  free_LINKSCHEME(&p->dyncoltl);       // dynamic color frame (top-left)
  free_LINKSCHEME(&p->dyncolbr);       // dynamic color frame (bottom-right)
  free_LINKSCHEME(&p->dyncoltxt);      // dynamic color text
  free_MYSCHEME(&p->stscheme);
  free_IMAGE(&p->image);
 #ifdef UC_CHANGE_IO_GROUP
  free_IOGRPCHX(&p->imiogroupchanged);
 #endif
 
  free_USERTEXT(&p->usertext);
  free_USERTEXT(&p->usertext1);
  free_ANYPARA(&p->newbubbletext);
 
  init_BUTTON(p);
}


void init_MEM_BUTTON(_MEM_BUTTON *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_BUTTON(_MEM_BUTTON *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
	 free_BUTTON(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);

  p->no  = 0;
//  init_MEM_BUTTON(p);speed
}

void init_BUBBLEHELP(_BUBBLEHELP *p)
{
  init_ROOM_Makro(&p->room);
  p->xstate    = BUBBLE_VIRGIN;
  p->font      = DEFFONT;
  p->attrib    = T_COPY|T_SOLID|T_CENTERBOUND|T_MIDBOUND;
  p->coltxt    = MakeColor_Makro(BLACK, BLACK);
  p->colframe  = MakeColor_Makro(BLACK, BLACK);
  p->colback   = MakeColor_Makro(YELLOW, YELLOW);
  p->frametype = _DEFFRAME;
  p->txt       = NULL;
}

void free_BUBBLEHELP(_BUBBLEHELP *p)
{
  // free_ROOM(&p->room);
  init_BUBBLEHELP(p);
}

void init_STACK(_STACK *p)
{
  //*p = data_STACK;
  init_STACK_Makro(p);
}

void free_STACK(_STACK *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_STACK_LINE(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);

  p->no  = 0;
//  init_STACK_Makro(p);speed
}


void init_ASCII_BUFFER(_ASCII_BUFFER *p)
{
  p->ptr   = 0;
  p->no    = 0;
  p->size  = 0;
  p->space = 0;
}

void free_ASCII_BUFFER(_ASCII_BUFFER *p)
{
  Memory2((void**)&p->ptr, 0);
  p->no    = 0;
  p->size  = 0;
  p->space = 0;
//  init_ASCII_BUFFER(p);speed
}

void init_TEXT_BUFFER(_TEXT_BUFFER *p)
{
  p->ptr  = NULL;                            // pointer to text
  p->poff = NULL;                 // pointer to textoffsettable
  p->no   = 0;                                    // no of text
  p->size = 0;                              // no of characters
}

void free_TEXT_BUFFER(_TEXT_BUFFER *p)
{
  Memory2((void**)&p->ptr, 0);
  Memory2((void**)&p->poff, 0);
  p->no     = 0;                                    // no of text
  p->size   = 0;                              // no of characters
//  init_TEXT_BUFFER(p);speed
}

void init_SCROLL(_SCROLL *p)
{
  init_ROOM_Makro(&p->out_room);
  init_ROOM_Makro(&p->scroll_room);
  p->font           = DEFFONT;
  p->attrib_inactiv = T_COPY|T_SOLID|T_LEFTBOUND|T_MIDBOUND|T_PROP;
  p->attrib_activ   = T_COPY|T_SOLID|T_LEFTBOUND|T_MIDBOUND|T_PROP;
  p->color_inactiv  = MakeColor_Makro(LIGHTGRAY, BLUE);
  p->color_activ    = MakeColor_Makro(DARKGRAY, WHITE);
  p->frame_out      = MakeColor_Makro(WHITE, DARKGRAY);
  p->frame_scroll   = MakeColor_Makro(DARKGRAY, WHITE);
  p->line_height    = DEFAULT_SCROLL_LINE_HEIGHT;
  p->line_width     = DEFAULT_SCROLL_LINE_WIDTH;
  init_POSITION(&p->position);
  init_POSITION(&p->hposition);
  init_LEVEL(&p->level);
  init_LEVEL(&p->hlevel);
  init_MENCURSOR(&p->mencursor);
  p->pt_line        = NULL;
  p->pt_beam        = NULL;
	p->pt_draw        = NULL;
	p->pt_start       = NULL;
	p->pt_ready       = NULL;
	p->pt_run         = NULL;
	p->pt_end         = NULL;
	p->pt             = NULL;
  p->Cursor         = NULL; // don't call methode Free !!
  p->noclrscr       = _FALSE;
  p->cursor_dx1     = 0;
  p->cursor_dx2     = 0;
  p->cursor_dy1     = 0;
  p->cursor_dy2     = 0;
  p->frametype      = _3DFRAME;
  p->do_out_timer   = 0;
  p->image          = NULL;
  p->nobck          = false;
  p->flash_cursorcolor = INVISIBLE;
}

void free_SCROLL(_SCROLL *p)
{
//  free_ROOM(&p->scroll_room);
  free_POSITION(&p->position);
  free_POSITION(&p->hposition);
  free_LEVEL(&p->level);
  free_LEVEL(&p->hlevel);
  free_MENCURSOR(&p->mencursor);
  // komponente image nicht freigeben weil diese aif image in der imagelist zeigt (applikation:01.09.06)
  init_SCROLL(p);
}

#ifdef UC_DIRECTORY
void init_DIRLIST(_DIRLIST *p)
{
  p->Label.Init();
  init_SCROLL(&p->scroll);
}

void free_DIRLIST(_DIRLIST *p)
{
  p->Label.Free();
  free_SCROLL(&p->scroll);
  init_DIRLIST(p);
}

void init_DIRECTORYSETUP(_DIRECTORYSETUP *p)
{
  p->font           = 0; //DEFFONT;
  p->lineheight     = 22;
  p->linewidth      = 150;
  p->lineonpage     = 10;
  p->offset.x       = 0;
  p->offset.y       = 0;
  p->color_inactive = MakeColor_Makro(LSE_COLOR8, BLACK);
  p->color_active   = MakeColor_Makro(BLUE, WHITE);
  p->color_frame    = MakeColor_Makro(DARKGRAY, WHITE);
}

void free_DIRECTORYSETUP(_DIRECTORYSETUP *p)
{
  init_DIRECTORYSETUP(p);
}

void init_DIRECTORY(_DIRECTORY *pt)
{
  pt->activ       = 1;
  pt->oldactiv    = 1;
  pt->workstate   = STATE_START;
  pt->ready       = _FALSE;
  pt->fkt         = NULL;
  pt->data        = NULL;
  pt->pthis       = NULL;
  pt->path[0]     = 0;
  pt->dpne[0]     = 0;
  pt->preelect[0] = 0;
  init_DIRLIST(&pt->Files);
  init_DIRLIST(&pt->Direct);
  init_CHREDIT(&pt->chredit);
  init_DIRECTORYSETUP(&pt->dirset);
}

void free_DIRECTORY(_DIRECTORY *pt)
{
  free_DIRLIST(&pt->Files);
  free_DIRLIST(&pt->Direct);
  free_DIRECTORYSETUP(&pt->dirset);
  init_DIRECTORY(pt);
}
#endif

void init_DPNE(_DPNE *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_DPNE(_DPNE *p)
{
  Memory2((void**)&p->ptr, 0);
  init_DPNE(p);
}

void init_PARA_PICKER(_PARA_PICKER *p)
{
  p->nr_subpict = 0xFFFF;
  p->actno      = 0xFFFF;
}

void free_PARA_PICKER(_PARA_PICKER *p)
{
  init_PARA_PICKER(p);
}

void init_PICRUNMEMO(_PICRUNMEMO *p)
{
  p->last_editorno = 0xFFFF;
}

void free_PICRUNMEMO(_PICRUNMEMO *p)
{
  init_PICRUNMEMO(p);
}

#ifdef OK_PICMEMO
void init_LASTINPUT(_LASTINPUT *p)
{
  p->no = 0xFFFF;
}

void free_LASTINPUT(_LASTINPUT *p)
{
  init_LASTINPUT(p);
}

void init_GLOBMEMO(_GLOBMEMO *p)
{
  init_WHOAMI(&p->whoami);
  init_LASTINPUT(&p->lastinput);
}

void free_GLOBMEMO(_GLOBMEMO *p)
{
  free_WHOAMI(&p->whoami);
  free_LASTINPUT(&p->lastinput);
}

void init_MEM_GLOBMEMO(_MEM_GLOBMEMO *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_GLOBMEMO(_MEM_GLOBMEMO *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_GLOBMEMO(&p->ptr[i]);
  Memory2((void**)&p->ptr, 0);
  init_MEM_GLOBMEMO(p);
}
#endif

#ifdef OK_VALIDATE
void init_VALIDATA(_VALIDATA *p)
{
  init_VARLABEL_Makro(&p->vl);
  p->ptr = NULL;
}

void free_VALIDATA(_VALIDATA *p)
{
  free_VARLABEL(&p->vl);
  Memory2((void**)&p->ptr, 0);
//  init_VALIDATA(p);speed
}

void init_MEM_VALIDATA(_MEM_VALIDATA *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_VALIDATA(_MEM_VALIDATA *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_VALIDATA(&p->ptr[i]);

  Memory2((void**)p->ptr, 0);

  p->no  = 0;
//  init_MEM_VALIDATA(p);speed
}
#endif

_UWORD cmp_VARLABEL(_VARLABEL *v1, _VARLABEL *v2)
{
  _UWORD     i;
  _VARSINGLE *p1, *p2;

  if(v1->no == v2->no)
  {
    for(i=0; i<v1->no; i++)
    {
      p1 = &v1->info[i];
      p2 = &v2->info[i];
      if((p1->state != p2->state) || (p1->value != p2->value))
        return(1);
    }
  }
  else
  {
    return(1);
  }

  return(0);
}

void init_SYSTEM(_SYSTEM *p)
{
  p->write                = _FALSE;
  p->act_language         = 0;
  p->delaytime_screensave = 0;
  p->seek_algorithm       = _TRUE;
  p->commasign            = _FALSE;
  p->americandateformat   = _FALSE;
  p->directiononenter     = _TRUE;
  p->cursorwraphorizontal = _TRUE;
  p->cursorwrapvertical   = _TRUE;
  p->noexterncom          = _FALSE;
}

void free_SYSTEM(_SYSTEM *p)
{
  init_SYSTEM(p);
}

void init_SINGLE_ALARM(_SINGLE_ALARM *p)
{
  init_VARLABEL_Makro(&p->link);
  init_CHKBIT_Makro(&p->chkbit);
  p->priority     = 0;
  p->group        = 0;
  p->userconfig   = 0;
  p->active       = MakeColor_Makro(LIGHTGRAY, RED);
  p->passive      = MakeColor_Makro(LIGHTGRAY, GREEN);
  p->quit_active  = MakeColor_Makro(LIGHTGRAY, DARKGRAY);
  p->quit_passive = MakeColor_Makro(LIGHTGRAY, DARKGRAY);
  init_ANYPARA_Makro(&p->text);
  init_ANYPARA_Makro(&p->helptext);

  init_MEM_VARLABEL(&p->parameter);
  p->alano        = 0xFFFF;
  p->mode         = 0;
}

void free_SINGLE_ALARM(_SINGLE_ALARM *p)
{
  free_VARLABEL(&p->link);
  free_CHKBIT(&p->chkbit);
  free_ANYPARA(&p->text);
  free_ANYPARA(&p->helptext);
  free_MEM_VARLABEL(&p->parameter);

  init_SINGLE_ALARM(p);
}

void init_ALARM(_ALARM *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_ALARM(_ALARM *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_SINGLE_ALARM(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);

	init_ALARM(p);
}

void init_MEM_NO(_MEM_NO *p)
{
	p->no = 0;
	p->ptr = NULL;
}

void free_MEM_NO(_MEM_NO *p)
{
	Memory2((void**)&p->ptr, 0);
	p->no = 0;
//	init_MEM_NO(p);speed
}

void init_LANGUAGE_INFO(_LANGUAGE_INFO *p)
{
  init_MEM_NO(&p->winid);
  init_MEM_NO(&p->flag);
  init_TEXT_BUFFER(&p->name);
  init_ASCII_BUFFER(&p->winname);
  p->no = 0;
}

void free_LANGUAGE_INFO(_LANGUAGE_INFO *p)
{
  free_MEM_NO(&p->winid);
  free_MEM_NO(&p->flag);
  free_TEXT_BUFFER(&p->name);
  free_ASCII_BUFFER(&p->winname);
  init_LANGUAGE_INFO(p);
}

void init_COMPCODE(_COMPCODE *p)
{
	p->size = 0;
	p->ptr  = NULL;
}

void free_COMPCODE(_COMPCODE *p)
{
	Memory2((void**)&p->ptr, 0);
	p->size = 0;
//	init_COMPCODE(p);speed
}

void init_INDEX(_INDEX *p)
{
  p->offset = 0;
	init_LASALID(&p->lasalid);
	p->ftime  = 300;            // unit [ms}
	p->size   = 0;
	p->valid  = 0;
}

void free_INDEX(_INDEX *p)
{
	init_INDEX(p);
}

void init_INDEXDAT(_INDEXDAT *p)
{
	p->permanent = 0;
	p->used      = 0;
	p->no        = 0;
	p->no_sent   = 0;
  p->no_max    = 0;
	p->data      = NULL;
}

void free_INDEXDAT(_INDEXDAT *p)
{
	_UWORD i;

	for(i=0; i<p->no; i++)
		free_INDEX(p->data);

	Memory2((void**)&p->data, 0);
	init_INDEXDAT(p);
}

void init_REFDAT(_REFDAT *p)
{
	p->permanent = 0;
	p->used      = 0;
	p->size      = 0;
	p->data      = NULL;
}

void free_REFDAT(_REFDAT *p)
{
	Memory2((void**)&p->data, 0);
	p->permanent = 0;
	p->used      = 0;
	p->size      = 0;
//	init_REFDAT(p);speed
}

void init_REFLIST(_REFLIST *p)
{
	p->login  = _LOGIN_INIT;
	p->permanent_enable = _TRUE;
	init_REFDAT(&p->refdat);
	init_INDEXDAT(&p->indexdat);
}

void free_REFLIST(_REFLIST *p)
{
	free_REFDAT(&p->refdat);
	free_INDEXDAT(&p->indexdat);
	p->login  = _LOGIN_INIT;
	p->permanent_enable = _TRUE;
//	init_REFLIST(p);speed
}

void init_COMSET(_COMSET *p)
{
	p->no = 0;
	p->ptr = NULL;
	MemSet(p->mapper, 0xFF, sizeof(p->mapper));
}

void free_COMSET(_COMSET *p)
{
	_UWORD i;

	for(i=0; i<p->no; i++)
		free_COMPORT(&p->ptr[i]);

	Memory2((void**)&p->ptr, 0);
	init_COMSET(p);
}

void init_COMPORT(_COMPORT *p)
{
  p->crc         = DEFAULTCRC;
  p->offline_cnt = 0;
  p->observe     = _FALSE;
  p->offline     = _FALSE;
  p->used        = _FALSE;
 #ifdef UC_NEW_ALARMSYSTEM
  p->alarmObject = 0;
  p->oldValue    = 0;
  p->alarmHandle = 0xFFFFFFFF;
  p->alarmAnzahl = 0;
  p->alarmValidIndex = 0;
  p->alarmRecordSize = 0;
  p->alarmXBufferSize = 0;
  p->alarmVersion = 0;
  p->alarm_disable = _FALSE;
 #endif
  p->add_reflist[0] = 0;
  p->add_reflist[1] = 0;
  p->async_pingstate = ASYNCPING_OFFLINE;
  p->plcstate = -1;
  p->async_onlstate = 0;
  p->async_onltimex = 0;
  p->loader_version = 0;
 #ifdef UC_GETLASALID_DATALINK
  p->lasalid_done = 0;
  p->datalink_hdl = NULL;
 #endif 

  init_COMDEF(&p->comdef);
  init_REFLIST(&p->reflist);
}

void free_COMPORT(_COMPORT *p)
{
 #ifdef UC_EXOONLINE
  if(IsExoOnline(&p->comdef))
  {
    ExoLogout(&p->comdef);
    free_COMDEF(&p->comdef);
    free_REFLIST(&p->reflist);
    init_COMPORT(p);
    return;
  }
 #endif

 #ifdef USE_LOGOUT
  LOGOUT(&p->comdef);
 #endif

  free_COMDEF(&p->comdef);
  free_REFLIST(&p->reflist);

  init_COMPORT(p);
}

void init_NEWSCREEN(_NEWSCREEN *p)
{
	p->ptr    = NULL;
	p->width  = 0;
	p->height = 0;
}

void free_NEWSCREEN(_NEWSCREEN *p)
{
	DestroyOffScreen(p);
  init_NEWSCREEN(p);
}

void init_REGION(_REGION *p)
{
  *p = NULL;
}

void free_REGION(_REGION *p)
{
  if(*p != NULL)
    FreeRegion(p);

  init_REGION(p);
}

void init_SINGLESCHEME(_SINGLESCHEME *p)
{
  init_VARLABEL_Makro(&p->upto);
  p->op    = 0;
  p->value = 0xFFFFFFFFL; //INVISIBLE;
}

void free_SINGLESCHEME(_SINGLESCHEME *p)
{
  free_VARLABEL(&p->upto);
  init_SINGLESCHEME(p);
}

void setup_SINGLESCHEME(_SINGLESCHEME *p, _UBYTE operat, _DWORD value, _UDWORD index)
{
  free_SINGLESCHEME(p);
  
  p->op = operat;
  p->upto.no = 1;
  p->upto.info[0].state = CONST_VAL;
  p->upto.info[0].value = value;
  p->value = index;
}

void init_MEM_SINGLESCHEME(_MEM_SINGLESCHEME *p)
{
  p->no  = 0;
  p->ptr = NULL;
  p->result_is_varno = _FALSE;
}

void free_MEM_SINGLESCHEME(_MEM_SINGLESCHEME *p)
{
  _UWORD i;
  for(i=0; i<p->no; i++)
    free_SINGLESCHEME(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);
  p->no  = 0;
  p->result_is_varno = _FALSE;
//  init_MEM_SINGLESCHEME(p);speed
}

void init_SCHEME(_SCHEME *p)
{
  //urinit_SCHEME(p);
 // *p = data_SCHEME;
  init_SCHEME_Makro(p);
}

void free_SCHEME(_SCHEME *p)
{
  free_VARLABEL(&p->link);
  free_MEM_SINGLESCHEME(&p->memsinglescheme);
//  init_SCHEME(p);speed
}

void init_MEM_SCHEME(_MEM_SCHEME *p)
{
  p->ptr = NULL;
  p->no  = 0;
}

void free_MEM_SCHEME(_MEM_SCHEME *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_SCHEME(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);

  p->no  = 0;
//  init_MEM_SCHEME(p);speed
}

#ifdef UC_ABANDONED
void init_REPEAT(_REPEAT *p)
{
  p->ftime    = 0;
  p->timediff = 0;
}

void free_REPEAT(_REPEAT *p)
{
  init_REPEAT(p);
}
#endif

void init_VIRTNAME(_VIRTNAME *p)
{
  p->no    = 0xFFFF;
  p->pthis = NULL;
}

void free_VIRTNAME(_VIRTNAME *p)
{
  _FVOID_VOID ptr;
  void *tmp;

  if(p->pthis != NULL)
  {
    ptr = (_FVOID_VOID)find_lasal_method(p->pthis, METHOD_MYIO_END);
    if(ptr != NULL)
    {
      tmp = p->pthis;
      load_this(tmp);
      ptr();
    }
    p->pthis = NULL;
  }

  p->no    = 0xFFFF;
//  init_VIRTNAME(p);speed
}

void init_MY_PICTURE(_MY_PICTURE *p)
{
  p->PScreenObject      = NULL;
  p->No                 = 0;
  p->Ptr                = NULL;
  p->P_Window           = NULL;
  p->TouchEditorNumeric = _TRUE;   // _TRUE ... able to open num toucheditor
  p->TouchEditorAscii   = _TRUE;   // _TRUE ... able to open anum toucheditor
  p->DrawShadow         = _TRUE;
  p->GlobalHotkeys      = _TRUE;
  init_ROOM_Makro(&p->Room);
  init_ACTUAL(&p->Actual);
  init_EDITOR(&p->Editor);
  init_ZOOM(&p->PicZoom);
  init_MEM_SEEKELEMENT(&p->memseekelement);
 #ifdef UC_PICKER
  init_PARA_PICKER(&p->ParaPicker);
 #endif
  init_DOT(&p->FirstInput);
  init_LSEFUNCT_Makro(&p->StartFunct);
  init_LSEFUNCT_Makro(&p->EndFunct);
  p->JustProgram        = _FALSE;
  p->RememberEnterPlus  = 0xFFFF;
}

void free_MY_PICTURE(_MY_PICTURE *p)
{
  _UWORD i;

  for(i=0; i<p->No; i++)
    p->Ptr[i].Free();

  Memory2((void**)&p->Ptr, 0);

  free_MEM_SEEKELEMENT(&p->memseekelement);
  free_ACTUAL(&p->Actual);
  free_EDITOR(&p->Editor);
  free_ZOOM(&p->PicZoom);
 #ifdef UC_PICKER
  free_PARA_PICKER(&p->ParaPicker);
 #endif
  init_MY_PICTURE(p);
}

_BOOL setup_MY_PICTURE(_MY_PICTURE *p)
{
  free_MY_PICTURE(p);
  
  if(Memory2((void**)&p->Ptr, sizeof(_SubPicture)) != 0)
  {
    p->No = 1;
    p->Ptr->Init();
    return _TRUE;
  }
  
  return _FALSE;
}

void init_SINGLESEEK(_SINGLESEEK *p)
{
  init_DIRECTION(&p->direction);
  p->location  = SL_UNDEF;
  p->no        = 0xFFFF;
  p->karuindex = 0xFFFF;
}

void free_SINGLESEEK(_SINGLESEEK *p)
{
  free_DIRECTION(&p->direction);
  p->location  = SL_UNDEF;
  p->no        = 0xFFFF;
  p->karuindex = 0xFFFF;
//  init_SINGLESEEK(p);speed
}

void init_SEEKELEMENT(_SEEKELEMENT *p)
{
  *p = data_SEEKELEMENT;
}

void free_SEEKELEMENT(_SEEKELEMENT *p)
{
  free_DIRECTION(&p->direction);
  *p = data_SEEKELEMENT;
  //init_SEEKELEMENT(p);speed
}

void init_MEM_SEEKELEMENT(_MEM_SEEKELEMENT *p)
{
  p->no = 0;
  p->doneno = 0;
  p->ptr = NULL;
}

void free_MEM_SEEKELEMENT(_MEM_SEEKELEMENT *p)
{
//  _UWORD i;
//  for(i=0; i<p->no; i++)
//    free_SEEKELEMENT(&p->ptr[i]);
  // geschwindigkeitsoptimierung (böse)
  Memory2((void**)&p->ptr, 0);

  p->doneno = 0;
  p->no = 0;
//  init_MEM_SEEKELEMENT(p);speed
}

void init_VARBLOCK(_VARBLOCK *p)
{
  p->no = 0 ;
  init_MEM_VARSYMBOL(&p->MemVarSymbol);
  init_MEM_LSEVARINFO(&p->MemLseVarInfo);
  p->Label.Init();
}

void free_VARBLOCK(_VARBLOCK *p)
{
  free_MEM_VARSYMBOL(&p->MemVarSymbol);
  free_MEM_LSEVARINFO(&p->MemLseVarInfo);
  p->Label.Free();
  init_VARBLOCK(p);
}

void init_EDITCOLOR(_EDITCOLOR *p)
{
  p->colback  = MakeColor_Makro(LIGHTRED, LIGHTRED); // color background (hatch)
  p->colframe = MakeColor_Makro(WHITE, DARKGRAY);    // color frame (top-left, bottom-right)
  p->coltxt   = MakeColor_Makro(WHITE, WHITE);       // color text
  init_LINKSCHEME_Makro(&p->dyncolback);  // dynamic color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncolfront); // dynamic color background (hatch)
  init_LINKSCHEME_Makro(&p->dyncoltl);    // dynamic color frame (top-left)
  init_LINKSCHEME_Makro(&p->dyncolbr);    // dynamic color frame (bottom-right)
  init_LINKSCHEME_Makro(&p->dyncoltxt);   // dynamic color text
}

void free_EDITCOLOR(_EDITCOLOR *p)
{
  free_LINKSCHEME(&p->dyncolback);  // dynamic color background (hatch)
  free_LINKSCHEME(&p->dyncolfront); // dynamic color background (hatch)
  free_LINKSCHEME(&p->dyncoltl);    // dynamic color frame (top-left)
  free_LINKSCHEME(&p->dyncolbr);    // dynamic color frame (bottom-right)
  free_LINKSCHEME(&p->dyncoltxt);   // dynamic color text

  p->colback  = MakeColor_Makro(LIGHTRED, LIGHTRED); // color background (hatch)
  p->colframe = MakeColor_Makro(WHITE, DARKGRAY);    // color frame (top-left, bottom-right)
  p->coltxt   = MakeColor_Makro(WHITE, WHITE);       // color text
  //init_EDITCOLOR(p);speed
}

void init_BUBBLEINFO(_BUBBLEINFO *p)
{
  init_EDITCOLOR(&p->editcolor);
  p->attrib     = T_COPY|T_SOLID|T_CENTERBOUND|T_MIDBOUND|T_PROP;
  p->font       = DEFFONT;
  p->frametype  = _DEFFRAME;
  p->delaytime  = 2000;
  p->remaintime = 10000;
}

void free_BUBBLEINFO(_BUBBLEINFO *p)
{
  free_EDITCOLOR(&p->editcolor);
  init_BUBBLEINFO(p);
}

void init_CURSORCOLOR(_CURSORCOLOR *p)
{
  p->color = DEFCOLOR;
  init_LINKSCHEME_Makro(&p->dyncol);
}

void free_CURSORCOLOR(_CURSORCOLOR *p)
{
  free_LINKSCHEME(&p->dyncol);
  p->color = DEFCOLOR;
//  init_CURSORCOLOR(p);speed
}

void init_MEM_ATTRIB(_MEM_ATTRIB *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_ATTRIB(_MEM_ATTRIB *p)
{
  Memory2((void**)&p->ptr, 0);
  init_MEM_ATTRIB(p);
}

void init_MEM_COLOR(_MEM_COLOR *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_COLOR(_MEM_COLOR *p)
{
  Memory2((void**)&p->ptr, 0);
  init_MEM_COLOR(p);
}

void init_MEM_ANYTHING(_MEM_ANYTHING *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_ANYTHING(_MEM_ANYTHING *p)
{
  _UWORD i;
  _ANYTHING *pany;
  
  if(p->no != 0)
  {
    pany = p->ptr;
    for(i=0; i<p->no; i++, pany++)
      free_ANYTHING(pany);
    p->no = 0;
  }

  Memory2((void**)&p->ptr, 0);
//  init_MEM_ANYTHING(p);
}

void init_MEM_ANYPARA(_MEM_ANYPARA *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_ANYPARA(_MEM_ANYPARA *p)
{
  _UWORD i;
  _ANYPARA *pap;

  if(p->no != 0)
  {
    pap = p->ptr;
    for(i=0; i<p->no; i++, pap++)
      free_ANYPARA(pap);
    p->no = 0;
  }

  Memory2((void**)&p->ptr, 0);
}

void init_XMEM_ANYPARA(_XMEM_ANYPARA *p)
{
  p->xxno = 0;
  p->xxptr = NULL;
}

void free_XMEM_ANYPARA(_XMEM_ANYPARA *p)
{
  _UBYTE i;
  _ANYPARA *pap;

  if(p->xxno != 0)
  {
    pap = p->xxptr;
    for(i=0; i<p->xxno; i++, pap++)
      free_ANYPARA(pap);
    p->xxno = 0;
  }

  Memory2((void**)&p->xxptr, 0);
}

void init_MEM_FONT(_MEM_FONT *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_FONT(_MEM_FONT *p)
{
  Memory2((void**)&p->ptr, 0);
  init_MEM_FONT(p);
}

void init_MEM_BOOL(_MEM_BOOL *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_BOOL(_MEM_BOOL *p)
{
  Memory2((void**)&p->ptr, 0);
  p->no = 0;
//  init_MEM_BOOL(p);speed
}

void init_MEM_LINKSCHEME(_MEM_LINKSCHEME *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_LINKSCHEME(_MEM_LINKSCHEME *p)
{
  _UWORD i;


  for(i=0; i<p->no; i++)
    free_LINKSCHEME(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);
  p->no = 0;
//  init_MEM_LINKSCHEME(p);speed
}

void init_DYNCOLOR(_DYNCOLOR *p)
{
  init_LINKSCHEME_Makro(&p->linkscheme);
  p->color = DEFCOLOR;
}

void free_DYNCOLOR(_DYNCOLOR *p)
{
  free_LINKSCHEME(&p->linkscheme);
  p->color = DEFCOLOR;
//  init_DYNCOLOR(p);speed
}

void init_MEM_DYNCOLOR(_MEM_DYNCOLOR *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_DYNCOLOR(_MEM_DYNCOLOR *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    init_DYNCOLOR(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);
  p->no = 0;
//  init_MEM_DYNCOLOR(p);speed
}

void init_STATISTIC(_STATISTIC *p)
{
  p->group     = 0;
  p->parameter = 0;
}

void free_STATISTIC(_STATISTIC *p)
{
  p->group     = 0;
  p->parameter = 0;
//  init_STATISTIC(p);
}

void init_MEM_TextPuffer(_MEM_TextPuffer *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_TextPuffer(_MEM_TextPuffer *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    p->ptr[i].Free();

  Memory2((void**)&p->ptr, 0);
  p->no = 0;
//  init_MEM_TextPuffer(p);speed
}

void init_PROJECT(_PROJECT *p)
{
  p->no             = 0;
  p->ptr            = NULL;
  p->no_static      = 15;
  p->prjdp[0]       = 0;
  p->crc32dpne      = DEFAULTCRC;
  p->RevisionNumber = 0;
  p->ProjectName[0] = 0;
}

void free_PROJECT(_PROJECT *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_PRJ_ENTRY(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);

  init_PROJECT(p);
}

void init_MEM_POINTER(_MEM_POINTER *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_POINTER(_MEM_POINTER *p)
{
  p->no  = 0;
  Memory2((void**)&p->ptr, 0);
//  init_MEM_POINTER(p);speed
}

void init_POINTERPARA(_POINTERPARA *p)
{
  p->ptr  = NULL;
  p->para = 0;
}

void init_MEM_POINTERPARA(_MEM_POINTERPARA *p)
{
  p->no     = 0;
  p->doneno = 0;
  p->ptr    = NULL;
}

void free_MEM_POINTERPARA(_MEM_POINTERPARA *p)
{
  p->no     = 0;
  p->doneno = 0;
  Memory2((void**)&p->ptr, 0);
}

#ifdef OK_SUBLIST
void init_VKLIST(_VKLIST *p)
{
  p->hide   = DEF4BYTE;
  p->next   = DEF4BYTE;
  p->jump   = DEF4BYTE;
  p->no     = DEF4BYTE;
  p->data   = 0;
  p->depth  = 0;
  p->parent = DEF4BYTE;
}

void free_VKLIST(_VKLIST *p)
{
  init_VKLIST(p);
}

void init_MEM_VKLIST(_MEM_VKLIST *p)
{
  p->ptr       = NULL;
  p->no        = 0;
  p->visual_no = 0;
  p->done_no   = 0;
}

void free_MEM_VKLIST(_MEM_VKLIST *p)
{
  _UDWORD i;

  for(i=0; i<p->no; i++)
    free_VKLIST(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);

  init_MEM_VKLIST(p);
}
#endif

void init_ANYPARA(_ANYPARA *p)
{
  //urinit_ANYPARA(p);
  //*p = data_ANYPARA;
  init_ANYPARA_Makro(p);
}

void free_ANYPARA(_ANYPARA *p)
{
  free_ANYTHING(&p->anything);
  free_MEM_VARLABEL(&p->memvarlabel);
//  init_ANYPARA(p);speed
}

void init_SCREENSAVER(_SCREENSAVER *p)
{

  p->actiontime = 0;
  p->image      = 0xffff;
  p->clrscr     = _FALSE;

}

void init_DEFKEYPAD(_DEFKEYPAD *p)
{
  p->ButtonColorReleased       = LIGHTBLUE;
  p->ButtonColorPressed        = LIGHTGREEN;
  p->ButtonColorText           = WHITE;
  p->TitleColorText            = BLACK;
  p->LimitColorText            = DARKGRAY;
  p->TitleBarColor             = WHITE;
  p->WindowColorBack           = 0xE71C;  // entspricht COLOR30
  p->ButtonFrame               = FRAME_PLASTIC;
  p->WindowFrame               = FRAME_METAL_CANCEL;
  p->StandardFontSchemeNo      = 0xFFFF;
  p->SmallFontSchemeNo         = 0xFFFF;
  p->StandardFontSchemeName[0] = 0;
  p->SmallFontSchemeName[0]    = 0;
}

void free_DEFKEYPAD(_DEFKEYPAD *p)
{
  init_DEFKEYPAD(p);
}

void free_SCREENSAVER(_SCREENSAVER *p)
{
  init_SCREENSAVER(p);
}

void init_SINGLETXTSCHEME(_SINGLETXTSCHEME *p)
{
  init_VARLABEL_Makro(&p->upto);
  p->op = SOP_EQUAL;
  init_ANYPARA(&p->anypara);
}

void free_SINGLETXTSCHEME(_SINGLETXTSCHEME *p)
{
  free_VARLABEL(&p->upto);
  free_ANYPARA(&p->anypara);

  init_SINGLETXTSCHEME(p);
}

void init_TXTSCHEME(_TXTSCHEME *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_TXTSCHEME(_TXTSCHEME *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_SINGLETXTSCHEME(&p->ptr[i]);
  Memory2((void**)&p->ptr, 0);

  init_TXTSCHEME(p);
}

void init_MEM_TXTSCHEME(_MEM_TXTSCHEME *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_TXTSCHEME(_MEM_TXTSCHEME *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_TXTSCHEME(&p->ptr[i]);
  Memory2((void**)&p->ptr, 0);
  init_MEM_TXTSCHEME(p);
}

void init_MYTXTSCHEME(_MYTXTSCHEME *p)
{
  *p = data_MYTXTSCHEME;
}

void free_MYTXTSCHEME(_MYTXTSCHEME *p)
{
  free_TXTSCHEME(&p->overload);
  free_ANYPARA(&p->anypara);
  init_MYTXTSCHEME(p);
}

void init_DIRECTION(_DIRECTION *p)
{
  p->h_next = DEFNEXTNO;
  p->v_next = DEFNEXTNO;
}

void free_DIRECTION(_DIRECTION *p)
{
  p->h_next = DEFNEXTNO;
  p->v_next = DEFNEXTNO;
//  init_DIRECTION(p);speed
}

_BOOL IsDrawBackGround(void)
{
  return((DrawBackGround == 0)? _FALSE : _TRUE);
}

_BOOL IsDrawPicture(void)
{
  return((DrawXPicture == 0)? _FALSE : _TRUE);
}

void set_MENCURSOR(_MENCURSOR *p)
{
  if((DrawBackGround == 0)&&(p->active == _TRUE))
  {
    p->delay_active = _TRUE;
    p->delay_timer  = get_abstime();
  }
}

_BOOL ask_MENCURSOR(_MENCURSOR *p)
{
  _BOOL retcode = _FALSE;

  if(p->delay_active == _TRUE)
  {
    if(timediff(p->delay_timer) >= 500)
    {
      p->delay_active = _FALSE;
      retcode = _TRUE;
    }
  }

  return(retcode);
}

void init_SINGLEALARM(_SINGLEALARM *p)
{
  p->no           = 0xFFFF;                                    // alarmnummer
  p->state        = 0;                                // status aktiv/inaktiv
  p->no_para      = 0;                                // anzahl der parameter
  MemSet(p->para, 0, sizeof(p->para));                           // parameter
  p->time_on      = 0;                                                // time
  p->date_on      = 0;                                                // date
  p->time_off     = 0;                                                // time
  p->date_off     = 0;                                                // date

  p->acknowledge  = 0;                                           // quittiert
  p->cnt          = 0;                                              // zähler
  init_ANYTHING_Makro(&p->anything);                           // index auf ?
 #ifdef UC_STATIC_ALARMSIZE
  MemSet(p->ptr, 0, UC_STATIC_ALARMSIZE);
 #else
  p->ptr          = NULL;                                           // memory
  p->ptrsize      = 0;                                       // grösse memory
 #endif
  p->entrytype    = 0xFFFF;                                 // alarmentrytype
  p->seqno        = 0xFFFFFFFFL;                // sequenznummer für ringfile
  p->was_locked   = _FALSE;       // _TRUE wenn Datensatz nicht in Historyfile geschrieben ist (lock)
  p->alarmid      = 0;                                    // ab fileversion 1
 #ifdef UC_MULTICPU
  p->multicpuno   = 0xFF;                          // cpu-nummer bei multicpu
 #endif
}

void free_SINGLEALARM(_SINGLEALARM *p)
{
 #ifndef UC_STATIC_ALARMSIZE
  Memory2((void**)&p->ptr, 0);
 #endif
  init_SINGLEALARM(p);
}

_BOOL make_MEM_SINGLEALARM(_MEM_SINGLEALARM*p, _UDWORD no, _UWORD filter)
{
  _UDWORD      i, size;
  _SINGLEALARM *ptr;
  _BOOL        retcode = _FALSE;

  init_MEM_SINGLEALARM(p);
  size = no * sizeof(_SINGLEALARM);

  PushMemColor(MEMORY_PRG);
  if(Memory2((void**)&p->ptr, size) != 0)
  {
    ptr = p->ptr;
    for(i=0; i<no; i++)
    {
      init_SINGLEALARM(ptr);
      ptr += 1;
    }

    p->ringbuffer.no    = no;
    p->ringbuffer.begin = 0;
    p->ringbuffer.pos   = 0;
    p->filter           = filter;

    retcode  = _TRUE;
  }
  PopMemColor();

  return(retcode);
}

void init_MEM_SINGLEALARM(_MEM_SINGLEALARM *p)
{
  p->filter        = 0;
  init_RINGBUFFER(&p->ringbuffer);
  p->xchange       = 0;
  p->ptr           = NULL;
  p->sortalgorithm = 0;
  p->noactive      = 0;
  p->pthis         = NULL;
  p->seqno         = 0;
  p->locked        = _FALSE;
  p->para_filter   = 0;  // SA33569
  p->File.Init();

  // vorsicht: diese funktion wird von free nicht aufgerufen, ansonsten
  // probleme mit download on the fly
}

void free_MEM_SINGLEALARM(_MEM_SINGLEALARM *p)
{
  _UDWORD i;

  // speicherplatz für alarmpuffer (hist+tmp) wegen "download on the fly" nicht freigeben.
  // nur der inhalt wird freigegeben und neu initialisiert.
  // dieser speicher wird nämlich beim AlarmPuffer::Init erzeugt und kann deshalb nicht mehr hergestellt werden.

  p->File.Free();

  for(i=0; i<p->ringbuffer.no; i++)
    free_SINGLEALARM(&p->ptr[i]);
  //  Memory2((void**)&p->ptr, 0);

  i = p->ringbuffer.no;
  free_RINGBUFFER(&p->ringbuffer);
  p->ringbuffer.no = i;

  p->noactive = 0;
  p->xchange  = 0;
  p->seqno    = 0;
  p->locked   = _FALSE;

  //  init_MEM_SINGLEALARM(p);
}

void init_MEM_INDEX(_MEM_INDEX *p)
{
  p->no   = 0;
  p->used = 0;
  p->ptr  = NULL;
  p->bit  = NULL;
}

void free_MEM_INDEX(_MEM_INDEX *p)
{
  Memory2((void**)&p->ptr, 0);
  Memory2((void**)&p->bit, 0);
  init_MEM_INDEX(p);
}

void *get_thispointer(_UWORD index)
{
  _FPT_UWORD ptr;
  void       *retcode = NULL;

  ptr = (_FPT_UWORD)find_lasal_method(LseThisPointer, METHOD_LSE_GETTHISPOINTER);
  if(ptr != NULL)
  {
    load_this(LseThisPointer);
    retcode = ptr(index);
  }

  return(retcode);
}

void init_TOOLDATA(_TOOLDATA *p)
{
  init_STATISTIC(&p->Filter);
  init_MEM_INDEX(&p->MemIndex);
  p->ActNo           = 0;
  p->ToolState       = TS_FINISH;
  p->ToolError       = TE_NOERROR;
  p->Version         = 0;
  p->care_state      = NOCARE_DEFAULT;
  p->dpne[0]         = 0;
  p->Pthis           = get_thispointer(0);
  p->Crc             = 0;
  p->CrcOffset       = 0;
  p->ClassRevision   = 0;
  p->CrcOk           = _FALSE;
  p->EndHeaderOffset = 0;
}

void free_TOOLDATA(_TOOLDATA *p)
{
  free_STATISTIC(&p->Filter);
  free_MEM_INDEX(&p->MemIndex);
//  p->File.Free();
  init_TOOLDATA(p);
}

void init_RINGBUFFER(_RINGBUFFER *p)
{
  p->no    = 0;
  p->begin = 0;
  p->pos   = 0;
  p->used  = _FALSE;
}

void free_RINGBUFFER(_RINGBUFFER *p)
{
  init_RINGBUFFER(p);
}

void init_LSEOSZI(_LSEOSZI *p)
{
  init_MEM_LSEOSZI_CHANNEL(&p->channels);
}

void free_LSEOSZI(_LSEOSZI *p)
{
  free_MEM_LSEOSZI_CHANNEL(&p->channels);
  init_LSEOSZI(p);
}

void init_LSEOSZI_CHANNEL(_LSEOSZI_CHANNEL *p)
{
  init_VARLABEL_Makro(&p->vl);
  p->YMin = 0;
  p->YMax = 0;
  p->color = DEFCOLOR;
  init_RINGBUFFER(&p->ringbuffer);
  p->data = NULL;
}

void free_LSEOSZI_CHANNEL(_LSEOSZI_CHANNEL *p)
{
  free_RINGBUFFER(&p->ringbuffer);
  Memory2((void**)p->data, 0);
  init_LSEOSZI_CHANNEL(p);
}

void init_MEM_LSEOSZI_CHANNEL(_MEM_LSEOSZI_CHANNEL *p)
{
  p->no = 0;
  p->ptr = NULL;
  init_RINGBUFFER(&p->ringbuffer);
  p->datim = NULL;
}

void free_MEM_LSEOSZI_CHANNEL(_MEM_LSEOSZI_CHANNEL *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_LSEOSZI_CHANNEL(&p->ptr[i]);

  Memory((void**)&p->ptr, 0);
  free_RINGBUFFER(&p->ringbuffer);
  Memory((void**)&p->datim, 0);

  init_MEM_LSEOSZI_CHANNEL(p);
}

void init_MEM_LSEFUNCT(_MEM_LSEFUNCT *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_LSEFUNCT(_MEM_LSEFUNCT *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_LSEFUNCT(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);

  init_MEM_LSEFUNCT(p);
}

void init_FAST_ASCII_BUFFER(_FAST_ASCII_BUFFER *p)
{
  *p = data_FAST_ASCII_BUFFER;
}

void free_FAST_ASCII_BUFFER(_FAST_ASCII_BUFFER *p)
{
  free_ASCII_BUFFER(&p->ascii_buffer);
  Memory2((void**)&p->poffset, 0);
  p->poffset = 0;
//  init_FAST_ASCII_BUFFER(p);speed
}

void init_SOFTKEYSETUP(_SOFTKEYSETUP *p)
{
  _ROOM room;

  p->height     = 50;                           // softkey-height
  p->no         = 8;                            // no of softkeys/floor
  MyGetScreenSize(&room);
  if(room.xy2.x > 1000)
    p->no       = 10;                           // no of softkeys/floor
  else if(room.xy2.x < 700)
    p->no       = 6;

  p->width      = (room.xy2.x + 1 - room.xy1.x) / p->no;
  p->horizontal = _TRUE;                        // direction (vertical/horizontal)
  p->align      = T_DOWNBOUND|T_CENTERBOUND;    // alignment
}

void free_SOFTKEYSETUP(_SOFTKEYSETUP *p)
{
  init_SOFTKEYSETUP(p);
}

void init_BUTTONIMAGES(_BUTTONIMAGES *p)
{
  p->image_release     = 0xFFFF;       // released image
  p->image_press       = 0xFFFF;       // pressed image
  p->image_sel_press   = 0xFFFF;       // selected+pressed image
  p->image_sel_release = 0xFFFF;       // selected+released image
  p->image_in_press    = 0xFFFF;       // inactive pressed image
  p->image_in_release  = 0xFFFF;       // inactive released image
  p->image_describe    = 0xFFFF;       // describing image
  p->zoom_images       = _TRUE;       // zoom images to buttonsize
}

void free_BUTTONIMAGES(_BUTTONIMAGES *p)
{
  init_BUTTONIMAGES(p);
}

void init_LSE_TIME(_LSE_TIME *p)
{

  p->timemin = 0;
  p->timemax = 0;
  p->timeaverage = 0;;
  p->timebase = 2500000;
  p->min = 0;
  p->max = 0xFFFFFFFF;
  p->average = 0;
  p->ref = 0;
  p->sum = 0;
  p->time = 0;
  p->count = 0;
  p->valid = _FALSE;
}

void free_LSE_TIME(_LSE_TIME *p)
{

  init_LSE_TIME(p);
}

/*
void init_SHAPE(_SHAPE *p)
{
  p->no        = 0;
  p->ptr       = NULL;
  p->color     = 0;
  p->backcolor = 0;
  p->vektor    = 0;
}

void free_SHAPE(_SHAPE *p)
{
  Memory2((void**)&p->ptr, 0);
  init_SHAPE(p);
}

void init_3DSHAPE(_3DSHAPE *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_3DSHAPE(_3DSHAPE *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_SHAPE(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);
  init_3DSHAPE(p);
}
*/


void init_SHAPEHEADER(_SHAPEHEADER *p)
{
  p->convex = _FALSE;
  p->pad    = 0;
  p->pbd    = 0;
  p->pcd    = 0;
  p->pdd    = 0;
  p->xmax   = 0;
  p->xmin   = 0;
  p->ymax   = 0;
  p->ymin   = 0;
  p->zmax   = 0;
  p->zmin   = 0;
//  init_DOT(ptr2d[100]); // attention _DOT
}

void free_SHAPEHEADER(_SHAPEHEADER *p)
{
  init_SHAPEHEADER(p);
}

void init_SHAPE(_SHAPE *p)
{
  p->no        = 0;
  p->ptr       = NULL;
  p->color     = 0;
  p->backcolor = 0;
  init_SHAPEHEADER(&p->header);
}

void free_SHAPE(_SHAPE *p)
{
  Memory2((void**)&p->ptr, 0);
  free_SHAPEHEADER(&p->header);
  init_SHAPE(p);
}

void init_3DSHAPEHEADER(_3DSHAPEHEADER *p)
{
  p->xmax =
  p->xmin =
  p->ymax =
  p->ymin =
  p->zmax =
  p->zmin = 0;
}

void free_3DSHAPEHEADER(_3DSHAPEHEADER *p)
{
  init_3DSHAPEHEADER(p);
}

void init_3DSHAPE(_3DSHAPE *p)
{
  p->shading = _FALSE;
  p->frame   = _FALSE;
  p->no      = 0;
  p->ptr     = NULL;
  init_3DSHAPEHEADER(&p->header);
}

void free_3DSHAPE(_3DSHAPE *p)
{
  _UWORD i;

  for(i=0; i<p->no; i++)
    free_SHAPE(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);
  free_3DSHAPEHEADER(&p->header);

  init_3DSHAPE(p);
}

void init_LINKVL(_LINKVL *p)
{
  p->info = 0;
  init_VARLABEL_Makro(&p->link);
}

void free_LINKVL(_LINKVL *p)
{
  free_VARLABEL(&p->link);
  p->info = 0;
//  init_LINKVL(p);speed
}

#ifdef UC_LINKVL
void init_MEM_LINKVL(_MEM_LINKVL *p)
{
  p->no  = 0;
  p->ptr = NULL;
}
void free_MEM_LINKVL(_MEM_LINKVL *p)
{
  _UWORD  i;
  _LINKVL *plvl;

  if(p->no != 0)
  {
    plvl = p->ptr;
    for(i=0; i<p->no; i++, plvl++)
      free_LINKVL(plvl);
    p->no  = 0;
  }

 #ifndef UC_LINKVL_FAST
  Memory2((void**)&p->ptr, 0);
 #endif
//  init_MEM_LINKVL(p);speed
}
#endif

void init_MEM_MAP(_MEM_MAP *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_MEM_MAP(_MEM_MAP *p)
{
  Memory2((void**)&p->ptr, 0);
  p->no = 0;
//  init_MEM_MAP(p);speed
}

void init_DATA(_DATA *p)
{
  p->no = 0;
  p->ptr = NULL;
}

void free_DATA(_DATA *p)
{
  Memory2((void**)&p->ptr, 0);
  p->no = 0;
//  init_DATA(p);speed
}

void init_NAME_INDEX(_NAME_INDEX *p)
{
  p->crc32 = 0;
  init_ASCII_BUFFER(&p->name);
  init_MEM_NO(&p->index);
}

void free_NAME_INDEX(_NAME_INDEX *p)
{
  free_ASCII_BUFFER(&p->name);
  free_MEM_NO(&p->index);
  p->crc32 = 0;
//  init_NAME_INDEX(p);speed
}

void init_SVRWATCH(_SVRWATCH *ptr)
{
  ptr->svrno    = DEFSCOPE;
  ptr->oldvalue = 0;
  ptr->ptrthis  = NULL;
  ptr->ptrfkt   = NULL;
}

void free_SVRWATCH(_SVRWATCH *ptr)
{
  init_SVRWATCH(ptr);
}

#ifdef UC_IPC
void init_MEMSIZE(_MEMSIZE *p)
{
  p->data  = 0xFFFFFFFFL;
  p->label = 0xFFFFFFFFL;
}
#endif

#ifdef UC_THEME
void init_FRAMEWORK(_FRAMEWORK *p)
{
  init_BMP_Makro(&p->bmp_tl);
  init_BMP_Makro(&p->bmp_tr);
  init_BMP_Makro(&p->bmp_bl);
  init_BMP_Makro(&p->bmp_br);
  init_BMP_Makro(&p->bmp_l);
  init_BMP_Makro(&p->bmp_r);
  init_BMP_Makro(&p->bmp_t);
  init_BMP_Makro(&p->bmp_b);
  init_BMP_Makro(&p->bmp_c);
  init_BMP_Makro(&p->bmp_cx);
}

void free_FRAMEWORK(_FRAMEWORK *p)
{
  free_BMP(&p->bmp_tl);
  free_BMP(&p->bmp_tr);
  free_BMP(&p->bmp_bl);
  free_BMP(&p->bmp_br);
  free_BMP(&p->bmp_l);
  free_BMP(&p->bmp_r);
  free_BMP(&p->bmp_t);
  free_BMP(&p->bmp_b);
  free_BMP(&p->bmp_c);
  free_BMP(&p->bmp_cx);
  init_FRAMEWORK(p);
}

void init_THEME(_THEME *p)
{
  p->id               = 0;
  p->delta_l          = 0;
  p->delta_r          = 0;
  p->delta_t          = 0;
  p->delta_b          = 0;
  p->xpos_cx          = -1;
  p->ypos_cx          = -1;
  p->colorcourse      = 0;
  init_FRAMEWORK(&p->frame0);
  init_FRAMEWORK(&p->frame1);
}

void free_THEME(_THEME *p)
{
  free_FRAMEWORK(&p->frame0);
  free_FRAMEWORK(&p->frame1);
  init_THEME(p);
}

void init_MEM_THEME(_MEM_THEME *p)
{
  p->no  = 0;
  p->ptr = NULL;
}

void free_MEM_THEME(_MEM_THEME *p)
{
  _UWORD i;
  for(i=0; i<p->no; i++)
    free_THEME(&p->ptr[i]);

  Memory2((void**)&p->ptr, 0);

  init_MEM_THEME(p);
}
#endif

void MyGetScreenSize(_ROOM *pr)
{
 #ifdef NINETEEN
  pr->xy1.x = 0;
  pr->xy1.y = 0;
  pr->xy2.x = 1023;
  pr->xy2.y = 767;
 #else
  GetScreenSize(pr);
 #endif
}

void MyClrScr(_COLOR col)
{
  _ROOM room;
  MyGetScreenSize(&room);
  Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY | T_SOLID, col);
}

void init_IPCINI(_IPCINI *ptr)
{
  StrCpy(ptr->ProjectDPNE, sizeof(_ASCII), "C:\\MPC\\OTTO.MPC", sizeof(_ASCII));
  StrCpy(ptr->DataDPNE, sizeof(_ASCII), "C:\\MPC\\DATA.BIN", sizeof(_ASCII));
  ptr->StartGfxDPNE[0]   = 0;
  ptr->SimulationDPNE[0] = 0;

  ptr->StartPicture = 0;
  ptr->LoginDelayTime = 0;
  init_COMSET(&ptr->ComSet);
}

void free_IPCINI(_IPCINI *ptr)
{
  free_COMSET(&ptr->ComSet);
  init_IPCINI(ptr);
}

void init_TREEITEM(_TREEITEM *p)
{
  p->psubtree     = NULL;        // --> auf subtree
  p->subtreestate = TREE_CLOSED; // status ob subtree offen ist oder nicht
  p->data[0]      = DEF4BYTE;
  p->data[1]      = DEF4BYTE;
  p->flags        = 0;
}

void free_TREEITEM(_TREEITEM *p)
{
  if(p->psubtree != NULL)
  {
    free_TREE(p->psubtree);
    Memory2((void**)&p->psubtree, 0);
  }
  init_TREEITEM(p);
}

void init_TREE(_TREE *p)
{
  p->no   = 0;           // anzahl
  p->done = 0;
  p->ptr  = NULL;       // --> auf daten
  p->visuallines = 0;
  p->markedlines = 0;
}

void free_TREE(_TREE *p)
{
  if(p->ptr != NULL)
  {
    _UDWORD i;
    _TREEITEM *pti = p->ptr;
  
    for(i=0; i<p->no; i++, pti++)
      free_TREEITEM(pti);
  
    Memory2((void**)&p->ptr, 0);
  }
  
  init_TREE(p);
}

void init_TREESCOPE(_TREESCOPE *p)
{
  p->no = 0;
}

void free_TREESCOPE(_TREESCOPE *p)
{
  init_TREESCOPE(p);
}

void init_FILEXPLORER(_FILEXPLORER *p)
{
  p->ActPath[0]        = 0;
  p->LockedRootPath[0] = 0;
  p->xoffset           = 180;
  p->FormatDate        = SIZE_DDMMYYYY;
  p->FormatTime        = SIZE_HHMM;
  p->ShowDate          = _TRUE;
  p->ShowTime          = _TRUE; 
  p->ShowSize          = _TRUE;
  p->ShowAttribute     = _TRUE;
  p->ColorMarked       = MakeColor_Makro(LIGHTGRAY, LIGHTGREEN);
  p->ColorImage        = MakeColor_Makro(LIGHTGRAY, YELLOW);
  p->BoldMarked        = _FALSE;
  p->FileFilter        = NULL;
  p->ImageDrive        = DEF4BYTE;
  p->ImageDirect       = DEF4BYTE;
  p->ImageFile         = DEF4BYTE;
  p->LockedRootSign    = (_ASCII*)"User:";
  MemSet(p->DriveList, 0, sizeof(p->DriveList));
  MemSet(p->ExcludeDrives, 0, sizeof(p->ExcludeDrives));
  p->LasalOsCnt        = 0;
  p->TreeSize          = 10;
  
  init_TREE(&p->Tree);
  p->NoList            = 0;
  p->pList             = NULL;
  
  p->SortCriteria      = 0;
  p->WidthName         = 0;
  p->WidthDate         = 0;
  p->WidthTime         = 0;
  p->WidthSize         = 0;    
  p->ImageAttReadOnly  = DEF4BYTE;
  p->ImageAttHidden    = DEF4BYTE;
  p->ImageAttArchiv    = DEF4BYTE;
  p->ImageAttSystem    = DEF4BYTE;  
}

void free_FILEXPLORER(_FILEXPLORER *p)
{
  free_TREE(&p->Tree);
  
  if(p->pList != NULL)
  {
     for(_UDWORD i=0; i<p->NoList; i++)
       p->pList[i].Free();
     Memory2((void**)&p->pList, 0);
  }
  
  init_FILEXPLORER(p);
}

_UDWORD GetRevisionOfClass_MyIo(void)
{
  if(RevClass_MyIo == 0)
    RevClass_MyIo = GetRevisionOfClass((char*)"_MyIo");
  return RevClass_MyIo;
}

_UDWORD AcknowledgeFlag(_DWORD cmd)
{
  if(cmd == 0) // reset flag
  {
    AckFlag = 0;
    return 0;
  }
  
  if(cmd == 1) // start flag
  {
    AckFlag = 1;
    return 0;
  }
  
  if(cmd == 2) // inc flag
  {
    if(AckFlag != 0)
      AckFlag += 1;
    return 0;
  }

  if(cmd == -1) // flag abfragen und ggf.zurücksetzen
  {
    if(AckFlag > 1)
    {
      AckFlag = 0; // reset flag
      return 1;
    }
  }
  
  return 0;
}
