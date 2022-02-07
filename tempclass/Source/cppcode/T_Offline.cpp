#include "T_IPC.H"
#include "T_NUC.H"

#ifdef UC_AUTO_RECONFIG_VAR
 _UDWORD reconfig_delay;
#endif

#pragma pack( push, 1 )

typedef struct
{
  unsigned long OnlType;        // Online Type
  unsigned long InterfaceNbr;   // Interfac number
                                // TCPIP: 1=First Interface
  unsigned long reserved1;      // reserved for future use, must be 0
  union {
    unsigned long CanDestStation; // Can Destination Station (0..31)
    struct {
      union {
        struct {                // IP-Address
          unsigned char IP1;
          unsigned char IP2;
          unsigned char IP3;
          unsigned char IP4;
        };
        unsigned long IP;
      };
      unsigned short Port;      // Port number
      unsigned short Filler1;
    };
  };
  unsigned long Filler[3];      // Fill to 8 DWORDs
} LSLONL_ADDR, *PLSLONL_ADDR;

typedef struct
{
  unsigned long version;

  char* (*Error)(long rc);
  long (*Open)(PLSLONL_ADDR addr);
  void (*Close)(int dnum);
  void (*Dummy1)(void);
  void (*Dummy2)(void);
  void (*Dummy3)(void);
  long (*GetCpuStatus)(int dnum, unsigned char *status);
}LSL_ONL;

#pragma pack(pop)


LSL_ONL *LslOnl = NULL;

_DWORD GetPLCState(_COMPORT *cp)
{
  _DWORD     retcode = -1; // kein status oder offline
  LSLONL_ADDR onladdr;
  _DWORD      iNumOpen;

  if(cp != NULL)
  {
    if((cp->comdef.typ == _COM_TCPIP1) || (cp->comdef.typ == _COM_TCPIP1_P) || (cp->comdef.typ == _COM_TCPIP2_P))
    {
      if(LslOnl == NULL)
      {
        LslOnl = (LSL_ONL*)opsys_cilget((_ASCII*)"LSLONL");
      }
      
      if(LslOnl != NULL)
      {
        onladdr.reserved1 = 0;
        onladdr.OnlType   = 2; //ONLTYPE_TCPIP;
        onladdr.IP        = cp->comdef.ipadress;
        onladdr.Port      = cp->comdef.port;
        
        iNumOpen = LslOnl->Open(&onladdr);
        if(iNumOpen > 0)
        {
           _UBYTE byCPUStatus;
          if(LslOnl->GetCpuStatus(iNumOpen, &byCPUStatus) == 0)
          {
            retcode = byCPUStatus;
          }
          LslOnl->Close(iNumOpen);
        }
      }
    }
  }
  
  return retcode;
}

_ASCII *GetPLCStateText(_DWORD plcstate)
{
  switch(plcstate)
  {
    case   0:  return (_ASCII*)"RUN RAM";                // CS_RUN_RAM 0
    case   1:  return (_ASCII*)"RUN ROM";                // CS_RUN_ROM
    case   2:  return (_ASCII*)"RUNTIME ERROR";          // CS_RUNTIME
    case   3:  return (_ASCII*)"POINTER ERROR";          // CS_POINTER
    case   4:  return (_ASCII*)"CHECKSUM ERROR";         // CS_CHKSUM
    case   5:  return (_ASCII*)"WATCHDOG TIMEOUT";       // CS_WATCHDOG
    case   6:  return (_ASCII*)"GENERAL ERROR";          // CS_ERROR
    case   7:  return (_ASCII*)"PROM DEFECT";            // CS_PROM_DEFECT
    case   8:  return (_ASCII*)"RESET";                  // CS_RESET
    case   9:  return (_ASCII*)"WATCHDOG DEFECT";        // CS_WD_DEFECT
    case  10:  return (_ASCII*)"STOP";                   // CS_STOP
    case  11:  return (_ASCII*)"PROG BUSY";              // CS_BUSY_PROG
    case  12:  return (_ASCII*)"PROGRAM LENGTH";         // CS_PGM_TOO_LONG
    case  13:  return (_ASCII*)"PROGRAM END";            // CS_PROG_END
    case  14:  return (_ASCII*)"PROGRAM MEMO";           // CS_PROG_MEMO
    case  15:  return (_ASCII*)"STOP-BREAKPOINT";        // CS_STOP_BRKPT
    case  16:  return (_ASCII*)"CPU STOP";               // CS_CPU_STOP
    case  17:  return (_ASCII*)"INTERRUPT ERROR";        // CS_INTERROR
    case  18:  return (_ASCII*)"SINGLE STEP";            // CS_SINGLESTEP
    case  19:  return (_ASCII*)"READY";                  // CS_READY
    case  20:  return (_ASCII*)"LOAD";                   // CS_LOAD
    case  21:  return (_ASCII*)"WRONG MODULE";           // CS_WRONG_MODULE
    case  22:  return (_ASCII*)"MEMORY FULL";            // CS_MEMORY_FULL
    case  23:  return (_ASCII*)"NOT LINKED";             // CS_NOT_LINKED
    case  24:  return (_ASCII*)"DIVIDE BY ZERO";         // CS_DIV_BY
    case  25:  return (_ASCII*)"DIAS ERROR";             // CS_DIAS_ERROR
    case  26:  return (_ASCII*)"WAIT";                   // CS_WAIT
    case  27:  return (_ASCII*)"OPSYS PROGRAM";          // CS_OP_PROG
    case  28:  return (_ASCII*)"OPSYS INSTALLED";        // CS_OP_INSTALLED
    case  29:  return (_ASCII*)"OPSYS TOO LONG";         // CS_OP_TOO_LONG
    case  30:  return (_ASCII*)"NO OPERATING SYSTEM";    // CS_NO_OPSYS
    case  31:  return (_ASCII*)"SEARCH OPSYS";           // CS_SEARCH_OPSYS
    case  32:  return (_ASCII*)"NO DEVICE";              // CS_NO_DEVICE
    case  33:  return (_ASCII*)"UNUSED CODE";            // CS_UNUSED_CODE
    case  34:  return (_ASCII*)"MEMORY ERROR";           // CS_MEMORY_ERROR
    case  35:  return (_ASCII*)"MAX IO";                 // CS_MAX_IO
    case  36:  return (_ASCII*)"MODUL LOAD ERROR";       // CS_LOADERROR
    case  37:  return (_ASCII*)"BOOTIMAGE FAILURE";      // 37
    case  38:  return (_ASCII*)"APPLMEMHEAP ERROR";      // 38
    case  39:  return (_ASCII*)"OFFLINE";                // unused	39
    case  40:  return (_ASCII*)"APPLICATION LOAD";       // 40
    case  41:  return (_ASCII*)"APPLICATION SAVE";       // 41
    case  42:  return (_ASCII*)"ETHERCAT MANAGER ERR";   // CS_ETHERCATMANAGER_ERROR
    case  43:  return (_ASCII*)"ETHERCAT ERROR";         // CS_ETHERCAT_ERROR
    case  44:  return (_ASCII*)"VARAN MANAGER ERROR";    // CS_VARANMANAGER_ERROR
    case  45:  return (_ASCII*)"VARAN ERROR";            // CS_VARAN_ERROR
    case  46:  return (_ASCII*)"ERROR LOADING LOADER";   // 46
    case  47:  return (_ASCII*)"ERROR SAVING PROJECT";   // 47
    case  50:  return (_ASCII*)"ACCESS EXCEPTION";       // CS_ACCESS 50
    case  51:  return (_ASCII*)"BOUND EXCEEDED";         // CS_BND_EXCEED
    case  52:  return (_ASCII*)"PRIVLEDGED INSTR";       // CS_PRIV_INSTR
    case  53:  return (_ASCII*)"FLOAT";                  // CS_FLOAT 53
    case  60:  return (_ASCII*)"DIAS RISC BAD INSTR";    // CS_DIAS_RISCERROR 60
    case  64:  return (_ASCII*)"INTERNAL ERROR";         // CS_OS_ERROR 64
    case  65:  return (_ASCII*)"FILE ERROR";             // CS_FILE_ERROR 65
    case  66:  return (_ASCII*)"DEBUG ASSERTION FAILED"; // CS_DEBUG_ASSERT_FAILED
    case  67:  return (_ASCII*)"RT-RUNTIME ERROR";       // CS_RTRUNTIME 67
    case  68:  return (_ASCII*)"BT-RUNTIME ERROR";       // CS_BTRUNTIME 68
    case  69:  return (_ASCII*)"PROGRAM ERROR";          // CS_PROG_ERROR
    case  70:  return (_ASCII*)"C-DIAS ERROR";           // CS_CDIAS_ERROR 70
    case  72:  return (_ASCII*)"SDIAS ERROR";            // CS_SDIAS_ERROR 72
    case  73:  return (_ASCII*)"EEPROM ERROR";           // CS_EEPROM_ERROR 73
    case  75:  return (_ASCII*)"SRAM ERROR";             // CS_SRAM_ERROR 75
    case  76:  return (_ASCII*)"USER DEFINED 01";        // CS_USERDEFINED_01
    case  77:  return (_ASCII*)"USER DEFINED 02";        // CS_USERDEFINED_02
    case  78:  return (_ASCII*)"USER DEFINED 03";        // CS_USERDEFINED_03
    case  79:  return (_ASCII*)"USER DEFINED 04";        // CS_USERDEFINED_04
    case  80:  return (_ASCII*)"USER DEFINED 05";        // CS_USERDEFINED_05
    case  98:  return (_ASCII*)"RETURN FROM SCR";        // 98
    case 100:  return (_ASCII*)"LOADER INIT";            // CS_LOADER_INIT  100
    case 101:  return (_ASCII*)"LOADER RUNRAM";          // CS_LOADER_RUNRAM
    case 102:  return (_ASCII*)"LOADER RUNROM";          // CS_LOADER_RUNROM
    case 103:  return (_ASCII*)"LOADER RUNTIME";         // CS_LOADER_RUNTIME
    case 104:  return (_ASCII*)"LOADER READY";           // CS_LOADER_READY
    case 105:  return (_ASCII*)"LOADER OK";              // CS_LOADER_OK
    case 106:  return (_ASCII*)"LOADER UNKNOWN CID";     // CS_LOADER_UNKNOWN_CID
    case 107:  return (_ASCII*)"LOADER UNKNOWN CONSTR";  // CS_LOADER_UNKNOWN_CONSTR
    case 108:  return (_ASCII*)"LOADER UNKNOWN OBJECT";  // CS_LOADER_UNKNOWN_OBJECT
    case 109:  return (_ASCII*)"LOADER UNKNOWN CHNL";    // CS_LOADER_UNKNOWN_CHNL
    case 110:  return (_ASCII*)"LOADER WRONG CONNECT";   // CS_LOADER_WRONG_CONNECT 110
    case 111:  return (_ASCII*)"LOADER WRONG ATTR";      // CS_LOADER_WRONG_ATTR
    case 112:  return (_ASCII*)"LOADER SYNTAX ERROR";    // CS_LOADER_SYNTAX_ERROR
    case 113:  return (_ASCII*)"LOADER NO FILE OPEN";    // CS_LOADER_NO_FILE_OPEN
    case 114:  return (_ASCII*)"LOADER OUTOF NEAR";      // CS_LOADER_OUTOF_NEAR
    case 115:  return (_ASCII*)"LOADER OUTOF FAR";       // CS_LOADER_OUTOF_FAR
    case 116:  return (_ASCII*)"LOADER INCOMPATIBLE";    // CS_LOADER_INCOMPATIBLE 116
    case 117:  return (_ASCII*)"LOADER COMPATIBLE";      // CS_LOADER_COMPATIBLE
    case 208:  return (_ASCII*)"LASALCLASS00";           // CS_LASALCLASS00
    case 209:  return (_ASCII*)"LASALCLASS01";           // CS_LASALCLASS01
    case 210:  return (_ASCII*)"LASALCLASS02";           // CS_LASALCLASS02    // 210
    case 211:  return (_ASCII*)"LASALCLASS03";           // CS_LASALCLASS03
    case 212:  return (_ASCII*)"LASALCLASS04";           // CS_LASALCLASS04
    case 213:  return (_ASCII*)"LASALCLASS05";           // CS_LASALCLASS05
    case 214:  return (_ASCII*)"LASALCLASS06";           // CS_LASALCLASS06
    case 215:  return (_ASCII*)"LASALCLASS07";           // CS_LASALCLASS07
    case 216:  return (_ASCII*)"LASALCLASS08";           // CS_LASALCLASS08
    case 217:  return (_ASCII*)"LASALCLASS09";           // CS_LASALCLASS09
    case 218:  return (_ASCII*)"LASALCLASS0A";           // CS_LASALCLASS0A
    case 219:  return (_ASCII*)"LASALCLASS0B";           // CS_LASALCLASS0B
    case 220:  return (_ASCII*)"LASALCLASS0C";           // CS_LASALCLASS0C    // 220
    case 221:  return (_ASCII*)"LASALCLASS0D";           // CS_LASALCLASS0D
    case 222:  return (_ASCII*)"LASALCLASS0E";           // CS_LASALCLASS0E
    case 223:  return (_ASCII*)"LASALCLASS0F";           // CS_LASALCLASS0F
    case 224:  return (_ASCII*)"LINKING...";             // 224
    case 225:  return (_ASCII*)"LINKER ERROR";           // 225
    case 226:  return (_ASCII*)"LINKING DONE";           // 226
    case 232:  return (_ASCII*)"INSTALLING OS...";       // CS_OP_INSTALL 232
    case 240:  return (_ASCII*)"WAIT FOR POWERDOWN";     // CS_USVWAIT 240
    case 241:  return (_ASCII*)"REBOOTING...";           // CS_REBOOT 241
    case 242:  return (_ASCII*)"RAM SAVE";               // CS_RAMSAVE 242
    case 243:  return (_ASCII*)"RAM LOAD";               // CS_RAMLOAD 243
    case 253:  return (_ASCII*)"PRE-RUN STATE";          // CS_PRE_RUN 253
    case 254:  return (_ASCII*)"PRE-RESET STATE";        // CS_PRE_RESET 254
  }
  
  return NULL;
}

static void offline_positions(_ROOM *penter, _ROOM *pesc, _ROOM *ptxt, _ROOM *pr)
{
  if((pr->xy2.x - pr->xy1.x) >= 400)
  {
    penter->xy1.x = pr->xy1.x + 10;
    penter->xy2.x = ((pr->xy1.x + pr->xy2.x) / 2) - 5;
    penter->xy1.y = pr->xy2.y - 50;
    penter->xy2.y = pr->xy2.y - 10;
    
    pesc->xy1.x   = ((pr->xy1.x + pr->xy2.x) / 2) + 5;
    pesc->xy2.x   = pr->xy2.x - 10;
    pesc->xy1.y   = pr->xy2.y - 50;
    pesc->xy2.y   = pr->xy2.y - 10;
    
    ptxt->xy1.x   = pr->xy1.x + 10;
    ptxt->xy1.y   = pr->xy1.y + 10;
    ptxt->xy2.x   = pr->xy2.x - 10;
    ptxt->xy2.y   = pr->xy2.y - 60;
  }
  else
  {
    penter->xy1.x = pr->xy1.x + 6;
    penter->xy1.y = pr->xy1.y + 6;
    penter->xy2.x = pr->xy2.x - 6;
    penter->xy2.y = pr->xy1.y + 40;
    
    pesc->xy1.x   = pr->xy1.x + 6;
    pesc->xy1.y   = pr->xy2.y - 40;
    pesc->xy2.x   = pr->xy2.x - 6;
    pesc->xy2.y   = pr->xy2.y - 6;
    
    ptxt->xy1.x   = pr->xy1.x + 6;
    ptxt->xy1.y   = penter->xy2.y + 1;
    ptxt->xy2.x   = pr->xy2.x - 6;
    ptxt->xy2.y   = pesc->xy1.y - 1;
  }
}

void offlinewindow_start(_MY_PICTURE *mp, _BOOL firsttime)
{
  if((firsttime == _TRUE) && (mp->JustProgram == _TRUE))
  {
    if(mp->No > 0)
    {
      if(mp->Ptr != NULL)
      {
        _FONT fnt = (_FONT)FontScheme.GetIndexByLabel((_ASCII*)"_@Standard");
        
        _ROOM button_esc, button_enter, text;
        offline_positions(&button_enter, &button_esc, &text, &mp->Room);
        
        // hintergrund ****************************************************************************
        _RECTANGLE rec;
        init_RECTANGLE(&rec);
        rec.room       = mp->Room;
        rec.color      = MakeColor(LIGHTRED, LIGHTRED);
        rec.colorframe = MakeColor(WHITE, DARKGRAY);
        rec.frametype  = FRAME_STEALTH;
        rec.attrib     = T_COPY | T_SOLID;
        mp->Ptr->Add(&rec);

        // button <ENTER> *************************************************************************
        _BUTTON button;
        init_BUTTON(&button);
        button.room               = button_enter; 
        button.font               = fnt;
        button.attrib             = T_CENTERBOUND | T_MIDBOUND | T_SOLID | T_COPY;
        button.col                = MakeColor(LIGHTRED, LIGHTRED);
        button.framecol           = MakeColor(WHITE, DARKGRAY);
        button.coltxt             = MakeColor(INVISIBLE, WHITE);
        button.frametype          = FRAME_PLASTIC;
        button.text.anything.list = TXTLIST_STX;
        button.text.anything.no   = TxtID_OFFLINE_ENTER;
        button.text1              = button.text;
        button.keycode[0]         = _ENTER;
        mp->Ptr->Add(&button);
        
        // button <ESC> ***************************************************************************
        button.room               = button_esc;
        button.text.anything.list = TXTLIST_STX;
        button.text.anything.no   = TxtID_OFFLINE_ESC;
        button.text1              = button.text;
        button.keycode[0]         = _ESC;
        mp->Ptr->Add(&button);

        // text ***********************************************************************************
        _PICTEXT pictxt;
        init_PICTEXT(&pictxt);
        pictxt.room                  = text;
        pictxt.font                  = fnt;
        pictxt.frametype             = FRAME_NONE;
        pictxt.attrib                = T_CENTERBOUND | T_MIDBOUND | T_SOLID | T_COPY;
        pictxt.colback               = MakeColor(INVISIBLE, INVISIBLE);     // color background (hatch)
        pictxt.coltxt                = MakeColor(INVISIBLE, WHITE);         // color text
        pictxt.anypara.anything.list = TXTLIST_STX;
        pictxt.anypara.anything.no   = TxtID_OFFLINE_TEXT;
        mp->Ptr->Add(&pictxt);
      }
    }
  }
  
 #ifdef UC_AUTO_RECONFIG_VAR
  reconfig_delay = 0; //get_abstime();
 #endif
}

void offlinewindow_run(_MY_PICTURE *mp, _EVENT *pe)
{
  if(mp->JustProgram == _TRUE)
  {
   #ifdef UC_AUTO_RECONFIG_VAR
   
    if((reconfig_delay < 10) || (timediff(reconfig_delay) >= 2000))
    {
      // die ersten 10 durchläufe immer onlinewatch ausführen, somit ist cpu-status am bildschirm schneller upgedated wenn online
      if(++reconfig_delay >= 10)
      {
        reconfig_delay = get_abstime();
        if(reconfig_delay < 10)
        {
          reconfig_delay = 10;
        }
      }
      
      if(Batch.WatchOffline() == _TRUE)
      {
        Keyboard.DelBuffer();
        Root.FindTop()->AddCmd(CMD_RECONFIGVAR, 0, 0, _FALSE);
      }
    }
   #endif

    if(pe->ftype == _EVENT_KEYPRESS)
    {
      if(pe->scancode == _ENTER)
      {
        Root.FindTop()->AddCmd(CMD_RECONFIGVAR, 0, 0, _FALSE);
      }
      else if(pe->scancode != _ESC)
      {
        free_EVENT(pe);
      }
    }
  }
}
void offlinewindow_ready(_MY_PICTURE *mp)
{
}


/*
void offlinewindow_ready(_MY_PICTURE *mp)
{

  _ROOM room;
  _CHAR txt[400];

  if(mp->JustProgram == _TRUE)
  {
    _FONT fnt = (_FONT)FontScheme.GetIndexByLabel((_ASCII*)"_@Standard");
    room = mp->Room;
   #ifdef HID //UC_TOUCH + UC_MOUSE
    OutMultiTextAlign(Batch.MakeOfflineText(txt, _FALSE), 0, &room, fnt, T_MIDBOUND|T_CENTERBOUND|T_COPY|T_SOLID|T_PROP, _EDGEFRAME, WHITE, MakeColor_Makro(LIGHTRED, LIGHTRED), MakeColor_Makro(WHITE, RED), sizeof(_CHAR));

    pos_2button(&room, &mp->Room, _TRUE);
    OutMultiTextAlign((void*)"<ENTER>...restart", 0, &room, fnt, T_MIDBOUND|T_CENTERBOUND|T_COPY|T_SOLID|T_PROP, _3DFRAME, WHITE, MakeColor_Makro(LIGHTRED, LIGHTRED), MakeColor_Makro(WHITE, RED), sizeof(_ASCII));

    pos_2button(&room, &mp->Room, _FALSE);
    OutMultiTextAlign((void*)"<ESC>...offlinemode", 0, &room, fnt, T_MIDBOUND|T_CENTERBOUND|T_COPY|T_SOLID|T_PROP, _3DFRAME, WHITE, MakeColor_Makro(LIGHTRED, LIGHTRED), MakeColor_Makro(WHITE, RED), sizeof(_ASCII));
   #else

    OutMultiTextAlign(Batch.MakeOfflineText(txt, _TRUE), 0, &room, fnt, T_MIDBOUND|T_CENTERBOUND|T_COPY|T_SOLID|T_PROP, _EDGEFRAME, WHITE, MakeColor_Makro(LIGHTRED, LIGHTRED), MakeColor_Makro(WHITE, RED), sizeof(_CHAR));
   #endif

   #ifdef UC_AUTO_RECONFIG_VAR
    reconfig_delay = get_abstime();
   #endif
  }

 #ifdef UC_ROTATE
  RefreshRectangle(&mp->Room);
 #endif
}


void offlinewindow_run(_MY_PICTURE *mp, _EVENT *pe)
{
  if(mp->JustProgram == _TRUE)
  {
    make_offlinewindow_hid(mp, pe);

   #ifdef UC_AUTO_RECONFIG_VAR
    if((timediff(reconfig_delay))>= 2000)//(UC_AUTO_RECONFIG_VAR*1000))
    {
      reconfig_delay = get_abstime();
      if(Batch.WatchOffline() == _TRUE)
      {
        Keyboard.DelBuffer();
        Root.FindTop()->AddCmd(CMD_RECONFIGVAR, 0, 0, _FALSE);
      }
    }
   #endif

    if(pe->ftype == _EVENT_KEYPRESS)
    {
      if(pe->scancode == _ENTER)
      {
        Root.FindTop()->AddCmd(CMD_RECONFIGVAR, 0, 0, _FALSE);
      }
      else if(pe->scancode != _ESC)
      {
        free_EVENT(pe);
      }
    }
  }
}
*/