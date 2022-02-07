//<NewSigmatekCFileOptimize/>
// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 22.11.2019                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#include "..\sigclib\sigclib.h"
#include "CDsClient.h"
#include "CDsIntern.h"
 
#define USE_TCP_SELECT 21 // verwende TcpSelect mit ?ms timeout, ansonsten polling 1ms
 
typedef struct
{
  unsigned long   duration;
  unsigned long   timex;
} tDscPause;

typedef struct
{
  unsigned long   wait_id;
  unsigned long   last_id;
  DsComData       *pcd;
} tDscSynchron;

typedef struct
{
  unsigned char   *ptr;
  unsigned long   bytesize_max;
  unsigned long   bytesize_rest;
  unsigned long   id;
  unsigned long   timex;
} tDsFragmentMgr;

#pragma pack(push, 1)
typedef struct
{
  uint32          fragment_id;
  uint32          nc;                // ??
  uint64          bytesize_gesamt;
  uint32          bytesize_actual;
  uint64          offset_actual;
} tDsFragmentHdr;
#pragma pack(pop)

typedef struct
{
  unsigned long   valid;             // valid flag
  char            ipv4[32];          // ipv4-host
  char            usertext[32];      // usertext
  void            *pcookie;          // cookie
  long            socket;            // used socket
  unsigned long   step;              // actual step
  unsigned long   stepforce;         // next step to perform
  unsigned long   timex_start;       // timestamp establish connection
  unsigned long   timeout_to_host;   // max-duration to wait for host
  long            plcstate;          // host plcstate
  unsigned long   clientid;          // userspecific Client ID
  unsigned long   port;              // Ip-Port
  unsigned long   dsccrc;            // descriptor-crc von gegenstelle
  
  unsigned long   inc_rx;            // rx-counter
  unsigned long   inc_tx;            // tx-counter
  unsigned long   timex_tx;          // time last tx (von daten)
  unsigned long   timex_rx;          // time last rx (von data)
  unsigned long   timex_alive_tx;    // time last tx (von alivetrigger)
  unsigned long   error;             // errorcode
  unsigned long   task_hdl;          // taskhandle
  
  tDscPause       pause;             // pause
  tDscSynchron    synchron[2];       // synchrone daten-info
  unsigned long   synchron_id_cnt;   // synchroner daten-counter
  
  tDsFragmentMgr  fragment_mgr;      // Fragmaentmanager
  unsigned long   fragment_id;       // FragmentId
  
  tDscFctDataRx   fct_data_rx;       // usercall rx-data
  tDscFctDataTx   fct_data_tx;       // usercall rx-data
  tDscFctPlcState fct_plc_state;     // usercall plc-state changed
  tDscFctTaskBeginEnd fct_task_begin_end;    // usercall task-begin / end
  unsigned long   task_runstate;     // indicator if firstrun of thread 0 firstrun, 1 run

  unsigned long   rxdata_start;      // rx-databuffer startindex
  unsigned char   rxdata[MaximumSizeOfComPackage * 2];    // rx-databuffer
} tDscData;

#define DSC_FRAGMENTSIZE_MAX   MaximumSizeOfComPackage //MaximumFragmentSize
#define DSC_ID_VALID           0xCAFEBABE

#define DSC_STEP_IDLE                   0
#define DSC_STEP_START                  1
#define DSC_STEP_PING                   2
#define DSC_STEP_SOCKET_CREATE          3
#define DSC_STEP_CONNECT                4
#define DSC_STEP_SIGNIN                 5
#define DSC_STEP_HANDSHAKE              6
#define DSC_STEP_RUN                    7
#define DSC_STEP_LOGOUT                20
#define DSC_STEP_CLOSE                 21

#define DSC_STEP_RESTART               88
#define DSC_STEP_ERROR                 99

#define DSC_ERROR_NONE                  0
#define DSC_ERROR_NO_SOCKET             1
#define DSC_ERROR_NO_HOST               2
#define DSC_ERROR_NO_HOST_ANSWER        3

//static void DataLink_FragmentLog(DsComData *pcd)
//{
//  char txt[500];
//  if(pcd->id_major == ComId_Major_DS)
//  {
//    if(pcd->id_minor == ComCmd_SendFragment)
//    {
//       void *hdl = sigclib_cfopen("C:\\FrgMgr.txt", 'a');
//       sigclib_sprintf(txt, "Fragment: %u [len=%u]", sigclib_tabsolute(), pcd->datalength);
//       sigclib_cfputs(hdl, txt);
//       sigclib_cfwrite_crlf(hdl);
//       
//       uint32 id = *(uint32*)&pcd->data[0];
//       uint32 ff = *(uint32*)&pcd->data[4];
//       uint32 ges = *(uint32*)&pcd->data[8];
//       uint32 byt = *(uint32*)&pcd->data[16];
//       uint32 off = *(uint32*)&pcd->data[20];
//       sigclib_sprintf(txt, "  id=%u, ff=%X, gesamt=%u, akt=%u, offset=%u", id, ff, ges, byt, off);
//       sigclib_cfputs(hdl, txt);
//       sigclib_cfwrite_crlf(hdl);
//       
//       if((off + byt) >= ges)
//       {
//         sigclib_cfwrite_crlf(hdl);
//       }
//       
//       sigclib_cfclose(hdl);
//    }
//  }
//}

static void init_tDsFragmentMgr(tDsFragmentMgr *p)
{
  p->ptr           = NULL;
  p->bytesize_max  = 0;
  p->bytesize_rest = 0;
  p->id            = 0;
  p->timex         = 0;
}

static void free_tDsFragmentMgr(tDsFragmentMgr *p)
{
  if(p->ptr != NULL)
  {
    sigclib_free(p->ptr);
  }
  init_tDsFragmentMgr(p);
}

static void init_tDscScynchron(tDscSynchron *p)
{
  p->wait_id = DSC_MESSAGEID_DEFAULT;
  p->last_id = DSC_MESSAGEID_DEFAULT;
  p->pcd     = NULL;
}

static void init_tDscData(tDscData *p, unsigned long timex)
{
  sigclib_memset(p, 0, sizeof(tDscData));
  init_tDscScynchron(&p->synchron[0]);
  init_tDscScynchron(&p->synchron[1]);
  init_tDsFragmentMgr(&p->fragment_mgr);
  
  p->synchron_id_cnt = 0xFACE2BAD; // startwert (crazy value)
  p->socket          = -1;
  p->step            = DSC_STEP_IDLE;
  p->stepforce       = DSC_STEP_IDLE;
  p->error           = DSC_ERROR_NONE;
  p->pcookie         = NULL;
  p->timex_start     = 0xFFFFFFFF;
  p->plcstate        = DSC_CPUSTATE_OFFLINE;
  p->clientid        = 0;
  p->task_hdl        = 0;          // taskhandle
  p->timex_tx        = timex;
  p->timex_rx        = timex;
  p->timex_alive_tx  = timex;
  p->port            = ComId_Port;
}

static void free_tDscData(tDscData *p)
{
  free_tDsFragmentMgr(&p->fragment_mgr);
  init_tDscData(p, 0);
}

static void DataLink_SetPause(tDscData *pds, unsigned long duration)
{
  pds->pause.timex    = sigclib_tabsolute();
  pds->pause.duration = duration;
}

static DsComData *DataLink_DataConsistent(tDscData *pds)
{
  if(pds->rxdata_start >= sizeof(DsComDataHdr))
  {
    DsComData *pcd = (DsComData*)pds->rxdata;
    if(pcd->datalength <= pds->rxdata_start)
    {
      return pcd;
    }
  }
  
  return NULL;
}

static DsComData *DataLink_SkipData(tDscData *pds, unsigned long length)
{
  DsComData *pcd = (DsComData*)pds->rxdata;
  
  if(length == 0xFFFFFFFF)
  {
    length = pcd->datalength;
  }
  
  if(pds->rxdata_start <= length)
  {
    pds->rxdata_start = 0;
    return NULL;
  }

  sigclib_memmove(&pds->rxdata[0], &pds->rxdata[length], pds->rxdata_start-length);
  pds->rxdata_start -= length;
  
  return DataLink_DataConsistent(pds);
}

static DsComData *DataLink_LookupData(tDscData *pds)
{
  long bytes = sigclib_tcp_nread_available(pds->socket);
  
  if(bytes > 0)
  {
    unsigned long rest = sizeof(pds->rxdata) - pds->rxdata_start;
    if(bytes > rest)
    {
      bytes = rest;
    }
  
    if(bytes > 0)
    {
      long nox = sigclib_tcp_recv(pds->socket, &pds->rxdata[pds->rxdata_start], bytes, 10);
      if(nox > 0)
      {
        pds->rxdata_start += (unsigned long)nox;
        pds->timex_rx = sigclib_tabsolute();
        pds->inc_rx ++;
      }
    }
  }

  return DataLink_DataConsistent(pds); // daten auf konsistenz überprüfen
}

static void DataLink_RxData(tDscData *pds, DsComData *p)
{
  if(pds->fct_data_rx != NULL)
  {
    if(pds->valid == DSC_ID_VALID)
    {
      pds->fct_data_rx(pds, pds->pcookie, p);
    }
  }
}

static void DataLink_CpuStateSet(tDscData *pds, long state)
{
  if(pds->plcstate != state)
  {
    pds->plcstate = state;
    if((pds->fct_plc_state != NULL) && (pds->valid == DSC_ID_VALID))
    {
      pds->fct_plc_state(pds, pds->pcookie, state);
    }
  }
}

static unsigned long DataLink_Handshake(tDscData *pds)
{
  unsigned long retcode = 0;
  
  DsComData *pcd = DataLink_LookupData(pds);
  if(pcd != NULL)
  {
    unsigned long length = pcd->datalength;
    if((pcd->id_major == ComId_Major_DS) && (pcd->id_minor == ComCmd_SignInExResult))
    {
      if(DsComData_GetU32(pcd, 0) == DS_ERROR_NONE) // abfrage ob serverseitig auch angenommen
      {
        pds->dsccrc = DsComData_GetU32(pcd, 20); // dsccrc
        DataLink_CpuStateSet(pds, DsComData_GetU32(pcd, 24)); // cpustatus
        retcode = 1;
      }
    }
    DataLink_SkipData(pds, length);
  }
  
  return retcode;
}

static long DataLink_SendIntern(tDscData *pds, DsComData *pcd);
static long DataLink_SendFragmented_Intern(tDscData *pds, DsComData *pcd)
{
  long retcode = -1;
  
  void *ph = sigclib_malloc(DSC_FRAGMENTSIZE_MAX);
  
  if(ph != NULL)
  {
    DsComData* pac = (DsComData*)ph;

    uint32 rest   = pcd->datalength;
    uint32 max    = DSC_FRAGMENTSIZE_MAX - ComDataHeaderSize - sizeof(tDsFragmentHdr);
    uint64 offset = 0;
    uint08 *pdata = (uint08*)pcd;

    retcode = 0;
    
    pac->id_major        = ComId_Major_DS;
    pac->id_minor        = ComCmd_SendFragment;
    pac->transactionSrc  = 0;
    pac->transactionDest = pcd->transactionDest;
    pac->messageId       = 0xFFFFFFFF;

    //  0 [4] .... id
    //  4 [4] .... immer 0xFFFFFFFF ??
    //  8 [8] .... Gesamtbytelänge der Daten
    // 16 [4] .... aktuelle Bytelänge
    // 20 [8] .... Offset der aktuellen Daten

    uint08 *pth1 = pac->data; // ansonsten compiler-warning
    *(uint32*)&pth1[0] = sigclib_atomic_incU32(&pds->fragment_id);
    *(uint32*)&pth1[4] = 0xFFFFFFFF;
    *(uint64*)&pth1[8] = pcd->datalength;

//    *(uint32*)&pac->data[0] = sigclib_atomic_incU32(&pds->fragment_id);
//    *(uint32*)&pac->data[4] = 0xFFFFFFFF;
//    *(uint64*)&pac->data[8] = pcd->datalength;
      
    do
    {
      uint32 part = (rest > max)? max : rest;
  
      pac->datalength = ComDataHeaderSize + 28 + part;

      uint08 *pth2 = pac->data; // ansonsten compiler-warning
      *(uint32*)&pth2[16] = part;
      *(uint64*)&pth2[20] = offset;
      
//      *(uint32*)&pac->data[16] = part;
//      *(uint64*)&pac->data[20] = offset;
      sigclib_memcpy(&pac->data[28], pdata, part);

      pdata  += part;
      offset += part;
      rest   -= part;

//DataLink_FragmentLog(pac);

      long st = DataLink_SendIntern(pds, pac);
      if(st >= 0)
      {
        retcode += st;
      }
      else
      {
        rest    = 0;
        retcode = st;
      }
    }
    while(rest);
    
    sigclib_free(ph);
  }    

  return retcode;
}

static long DataLink_SendIntern(tDscData *pds, DsComData *pcd)
{
  if(pds->socket >= 0)
  {
    if(pcd->datalength <= DSC_FRAGMENTSIZE_MAX)
    {
      if(pds->fct_data_tx != NULL)
      {
        if((pcd->id_minor != ComCmd_SignIn) || (pcd->id_major != ComId_Major_DS))
        {
          if((pds->fct_data_tx != NULL) && (pds->valid == DSC_ID_VALID))
          {
            pds->fct_data_tx(pds, pds->pcookie, pcd);
          }
        }
      }
    
      unsigned long retcode = 0;
      unsigned long len = pcd->datalength;
      unsigned char *ph = (unsigned char*)pcd;
    
      while(len)
      {
        long sent = sigclib_tcp_send(pds->socket, ph, len, 0);
        if(sent >= 0)
        {
          ph      += sent;
          len     -= sent;
          retcode += sent;
        }
        else if(sent == -2000) // tcpip not ready
        {
          sigclib_thread_delay_us(123);
        }
        else
        {
          retcode = sent;
          len     = 0;
        }
      }
    
      if(retcode >= 0)
      {
        sigclib_atomic_setU32(&pds->timex_tx, sigclib_tabsolute());
        sigclib_atomic_incU32(&pds->inc_tx);
      }
      
      return retcode;
    }
    else
    {
      return DataLink_SendFragmented_Intern(pds, pcd);
    }
  }
  
  return -1;
}

static unsigned long DataLink_SendSignOut(tDscData *pds)
{
  unsigned long retcode = 0;
  unsigned char tmp[32];
  
  DsComData *pcd = DsComData_CreateEx(tmp, sizeof(tmp), 0, ComId_Major_DS, ComCmd_SignOut);
  if(pcd != NULL)
  {
    if(DataLink_SendIntern(pds, pcd) >= 0)
    {
      retcode = 1;
    }
    
    sigclib_buffer_destroy(tmp, pcd);
  }
  
  return retcode;
}

static unsigned long DataLink_SendSignIn(tDscData *pds, unsigned long clientid)
{
  unsigned char tmp[128];
  unsigned long retcode = 0;
  unsigned long len = sigclib_strlen(pds->usertext) + 1;
  
  DsComData *pcd = DsComData_CreateEx(tmp, sizeof(tmp), 16+len, ComId_Major_DS, ComCmd_SignIn);
  if(pcd != NULL)
  {
    unsigned long glen = len + 16;
    DsComData_AddU32(pcd, glen, ComId_SignInIdStandard ^ 0xFACE);
    DsComData_AddU32(pcd, glen, clientid);
    DsComData_AddU32(pcd, glen, 0x1FE);
    DsComData_AddU32(pcd, glen, 0);
    DsComData_AddData(pcd, glen, pds->usertext, len);
    
    if(DataLink_SendIntern(pds, pcd) >= 0)
    {
      retcode = 1;
    }
    
    sigclib_buffer_destroy(tmp, pcd);
  }
  
  return retcode;
}

static unsigned long DataLink_SendAlive(tDscData *pds)
{
  unsigned long retcode = 0;
  unsigned char tmp[32];
  DsComData *pcd = DsComData_CreateEx(tmp, sizeof(tmp), 0, ComId_Major_DS, ComCmd_AliveTrigger);
  if(pcd != NULL)
  {
    if(DataLink_SendIntern(pds, pcd) >= 0)
    {
      retcode = 1;
    }
    
    sigclib_buffer_destroy(tmp, pcd);
  }
  
  return retcode;
}

static void DataLink_Error(tDscData *pds, unsigned long error)
{
  if(pds->step != DSC_STEP_ERROR)
  {
    // todo: Usercall für ErrorRise aufrufen
    pds->error = error;
    pds->step  = DSC_STEP_ERROR;
  }
}

static void DataLink_CheckAlive(tDscData *pds)
{
  unsigned long tix = sigclib_tabsolute();
  
  if((tix - pds->timex_tx) > StandardTimeOut_SendAliveTrigger)
  {
    if((tix - pds->timex_alive_tx) > 100) // Bremse
    {
      pds->timex_alive_tx = tix;
      DataLink_SendAlive(pds);
    }
  }
  
  if((tix - pds->timex_rx) > (3 * StandardTimeOut_SendAliveTrigger))
  {
    // offline, server meldet sich nicht mehr
    DataLink_Error(pds, DSC_ERROR_NO_HOST);
    DataLink_CpuStateSet(pds, DSC_CPUSTATE_OFFLINE);
    
    // versuche wieder eine Verbindung herzustellen
    pds->stepforce = DSC_STEP_RESTART;
    DataLink_SetPause(pds, 250);
  }
  
}

static unsigned long DataLink_SynchronPacketChk(tDscData *pds, tDscSynchron *psync, DsComData *pcd)
{
  if(psync->wait_id == pcd->messageId) 
  {
    DsComData *pd = psync->pcd;
    if(pd != NULL)
    {
      sigclib_memcpy(pd, pcd, pcd->datalength);
    }
    sigclib_atomic_setU32(&psync->last_id, pcd->messageId);
    
    DataLink_SkipData(pds, 0xFFFFFFFF);
    return 1;
  }

  return 0;
}

static unsigned long DataLink_FragmentManager(tDscData *pds, DsComData *pcd)
{
  tDsFragmentMgr *p   = &pds->fragment_mgr;
  tDsFragmentHdr *pfh = (tDsFragmentHdr*)pcd->data;
  
//  DataLink_FragmentLog(pcd);
  
  if(pcd->datalength >= sizeof(sizeof(DsComDataHdr)) + sizeof(tDsFragmentHdr)) // ist datenpaket gross genug ?
  {
    if((pfh->offset_actual + pfh->bytesize_actual) <= pfh->bytesize_gesamt) // sind enthaltene daten konsistent
    {
      if(pfh->bytesize_gesamt > p->bytesize_max) // alloc / realloc notwendig
      {
        if(sigclib_memory((void**)&p->ptr, pfh->bytesize_gesamt) != 0)
        {
          p->bytesize_max = pfh->bytesize_gesamt;
          p->id           = pfh->fragment_id ^ 0xCAFEBABE; // id zerstören
        }
      }
    
      if(pfh->bytesize_gesamt <= p->bytesize_max) // passt paket auch in allokierten speicher
      {
        if((pfh->offset_actual + pfh->bytesize_actual) <= p->bytesize_max) // ist offset korrekt
        {
          if(pfh->fragment_id != p->id) // neues paket ?
          {
            p->id            = pfh->fragment_id;
            p->bytesize_rest = pfh->bytesize_gesamt;
          }

          if(pfh->bytesize_actual <= p->bytesize_rest)
          {
            sigclib_memcpy(&p->ptr[pfh->offset_actual], &pcd->data[sizeof(tDsFragmentHdr)], pfh->bytesize_actual);
            p->bytesize_rest -= pfh->bytesize_actual;
            p->timex = sigclib_tabsolute();
            
            if(p->bytesize_rest == 0)
            {
              DataLink_RxData(pds, (DsComData*)p->ptr);
              p->timex = 0;
              
              if(p->bytesize_max > 50000)
              {
                void *ph        = p->ptr;
                p->bytesize_max = 0;
                p->ptr          = NULL;
                sigclib_memory((void**)&ph, 0);
              }
            }
            return 1;
          }
        }
      }
    }
  }
  
  return 0;
}

static void DataLink_Run(tDscData *pds)
{
  DsComData *pcd = DataLink_LookupData(pds);
  
  if(pcd != NULL)
  {
#ifdef USE_TCP_SELECT      
    while(pcd != NULL)
    {
#else
    unsigned long loop = 0;
    while((pcd != NULL) && (loop < 16))
    {
      loop ++;
#endif

      DataLink_CheckAlive(pds);
      
      unsigned long length = pcd->datalength;

      if(pds->valid == DSC_ID_VALID) // nur solange Handle auch gültig ist
      {
        if(pds->synchron[0].wait_id != DSC_MESSAGEID_DEFAULT) // Abfrage ob auf ein synchrones Paket[0] gewartet wird
        {
          if(DataLink_SynchronPacketChk(pds, &pds->synchron[0], pcd) != 0)
          {
            return; // Daten wurden verarbeitet, Funktion verlassen
          }
        }
      
        if(pds->synchron[1].wait_id != DSC_MESSAGEID_DEFAULT) // Abfrage ob auf ein synchrones Paket[1] gewartet wird
        {
          if(DataLink_SynchronPacketChk(pds, &pds->synchron[1], pcd) != 0)
          {
            return; // Daten wurden verarbeitet, Funktion verlassen
          }
        }

        if(pcd->id_major == ComId_Major_DS)
        {
          switch(pcd->id_minor)
          {
            case ComCmd_SignInExResult :
              break;
        
            case ComCmd_CpuState :
              DataLink_CpuStateSet(pds, DsComData_GetU32(pcd, 0));
              DataLink_RxData(pds, pcd);
              break;
            
            case ComCmd_AliveTrigger :
              DataLink_RxData(pds, pcd);
              break;
          
            case ComCmd_SendFragment :
              DataLink_FragmentManager(pds, pcd);
              break;
          
            default :
              DataLink_RxData(pds, pcd);
              break;
          }
        }
        else
        {
          DataLink_RxData(pds, pcd);
        }
      }
      
      pcd = DataLink_SkipData(pds, length);
    }
  }
  
 #ifndef USE_TCP_SELECT      
  DataLink_CheckAlive(pds);
 #endif 
}

#ifdef USE_TCP_SELECT      
static void DataLink_RunSelect(tDscData *pds)
{
  while((pds->step == DSC_STEP_RUN) && (pds->stepforce == DSC_STEP_IDLE))
  {
    long sock = pds->socket;
    sigclib_tcp_select(1, &sock, 1, 0, USE_TCP_SELECT);
  
    if(sock > 0)
    {  
      DataLink_Run(pds);
    }
    
    DataLink_CheckAlive(pds);
  }
}
#endif

static unsigned long DataLink_HostTimeoutCheck(tDscData *pds, unsigned long pause_duration, unsigned long error)
{
  if(pds->timex_start != 0) // nur wenn startzeit bekannt, ansonsten try-to-reconnect
  {
    unsigned long diff = sigclib_tabsolute() - pds->timex_start;
    if(diff > pds->timeout_to_host)
    {
      DataLink_Error(pds, error);
      return 0;
    }
  }
  
  DataLink_SetPause(pds, pause_duration);
  return 1;
}

unsigned long DataLink_Thread(tDscData *pds)
{
  if(pds->task_runstate == 0)
  {
    pds->task_runstate = 1;
    if(pds->fct_task_begin_end != NULL)
    {
      pds->fct_task_begin_end(pds, pds->pcookie, 1); // usercallback thread-firstrun
    }
  }

  if(pds->pause.duration == 0)
  {
    if(pds->stepforce != DSC_STEP_IDLE)
    {
      pds->step      = pds->stepforce;
      pds->stepforce = DSC_STEP_IDLE;
    }

    switch(pds->step)
    {
      case DSC_STEP_IDLE:
        break;
    
      case DSC_STEP_ERROR:
        break;

      case DSC_STEP_RESTART:
        pds->timex_start = 0; // nur bei reconnect auf 0 setzen
        pds->step = DSC_STEP_PING;
        break;
        
      case DSC_STEP_START :
        pds->timex_start = sigclib_tabsolute();
        if(pds->timex_start == 0) 
        {
          pds->timex_start = 0xFFFFFFFF;
        }
        pds->step = DSC_STEP_PING;
        //break; // kein break, Durchläufer
        
      case DSC_STEP_PING:
        if(sigclib_tcp_ping(pds->ipv4, 32, 500, 2000) >= 0)
        {
          pds->step = DSC_STEP_SOCKET_CREATE;
        }
        else
        {
          DataLink_HostTimeoutCheck(pds, 250, DSC_ERROR_NO_HOST);
        }
        break;

      case DSC_STEP_SOCKET_CREATE:
        if(pds->socket >= 0)
        {
          if(sigclib_tcp_socket_close(pds->socket, 0) < 0)
          {
            DataLink_SetPause(pds, 10); // socket konnt noch nicht geschlossen werden, keine ahnung weshalb --> Pause und try again
            break;
          }
        }
        pds->socket = sigclib_tcp_socket_open();
        if(pds->socket >= 0)
        {
          pds->step = DSC_STEP_CONNECT;
        }
        else
        {
          DataLink_HostTimeoutCheck(pds, 100, DSC_ERROR_NO_SOCKET);
        }
        break;
    
      case DSC_STEP_CONNECT:
        if(sigclib_tcp_connect(pds->socket, pds->ipv4, pds->port, 2000) == 0)
        {
          pds->step = DSC_STEP_SIGNIN;
        }
        else
        {
          DataLink_HostTimeoutCheck(pds, 250, DSC_ERROR_NO_HOST_ANSWER);
          if(pds->timex_start == 0) // Abfrage ob Reconnect
          {
            pds->step = DSC_STEP_RESTART; // try again
          }
        }
        break;

      case DSC_STEP_SIGNIN:
        if(DataLink_SendSignIn(pds, pds->clientid) != 0)
        {
          pds->step = DSC_STEP_HANDSHAKE;
        }
        else
        {
          DataLink_HostTimeoutCheck(pds, 200, DSC_ERROR_NO_HOST_ANSWER);
        }
        break;

      case DSC_STEP_HANDSHAKE:
        if(DataLink_Handshake(pds) == 1)
        {
          pds->step = DSC_STEP_RUN;
        }
        else
        {
          DataLink_HostTimeoutCheck(pds, 0, DSC_ERROR_NO_HOST_ANSWER);
        }
        break;
        
      case DSC_STEP_RUN:
#ifdef USE_TCP_SELECT      
        DataLink_RunSelect(pds);
#else        
        DataLink_Run(pds);
#endif        
        break;
        
      case DSC_STEP_LOGOUT:  
        DataLink_SendSignOut(pds);
        DataLink_CpuStateSet(pds, DSC_CPUSTATE_OFFLINE);
        pds->valid = 0; // ab jetzt nicht mehr verwenden
        DataLink_SetPause(pds, 100); // pause, somit haben alle funktionen, welche pds noch verwenden, zeit zum "fertigwerden"
        pds->step = DSC_STEP_CLOSE;
        break;
        
      case DSC_STEP_CLOSE:
        if(pds->socket >= 0)
        {
          sigclib_tcp_socket_close(pds->socket, 1);
          pds->socket = -1;
        }
        pds->task_runstate = 0;
        if(pds->fct_task_begin_end != NULL)
        {
          pds->fct_task_begin_end(pds, pds->pcookie, 0); // usercallback thread-end
        }
        free_tDscData(pds);
        sigclib_free(pds);
        return 0; // finito, thread wird beendet !!
        
    }
  }
  else
  {
    // pause
    if((sigclib_tabsolute() - pds->pause.timex) >= pds->pause.duration)
    {
      pds->pause.duration = 0;
    }
  }
  
  return 1;
}

void DataLink_DefaultDataRxFunct(void *pds0, void *pcookie, DsComData *pdata)
{
  UNUSED(pds0);
  UNUSED(pcookie);
  UNUSED(pdata);
}

void DataLink_DefaultPlcStateFunct(void *pds0, void *pcookie, long plcstate)
{
  UNUSED(pds0);
  UNUSED(pcookie);
  UNUSED(plcstate);
}

static tDscData *DataLink_Valid(void *pds0)
{
  if(pds0 != NULL)
  {
    tDscData *pds = (tDscData*)pds0;
    if(pds->valid == DSC_ID_VALID)
    {
      return pds;
    }
  }
   
  return NULL;
}

static long DataLink_SendSynIntern(tDscData *pds, tDscSynchron *psync, unsigned long id, DsComData *pcd_dst, DsComData *pcd_src, unsigned long timeout)
{
  long retcode = 0;
  
  if(sigclib_atomic_cmpxchgU32(&psync->wait_id, DSC_MESSAGEID_DEFAULT, id) == DSC_MESSAGEID_DEFAULT)
  {
    retcode            = 2;
    psync->last_id     = DSC_MESSAGEID_DEFAULT;
    psync->pcd         = pcd_dst;
    unsigned long memo = pcd_src->messageId; // memo
    
    if(pcd_src->messageId != id)
    {
      pcd_src->messageId = id; // neue id
    }
    
    if(DataLink_SendIntern(pds, pcd_src) >= 0)
    {
      retcode = 0;
      unsigned long tix = sigclib_tabsolute();
      while((psync->last_id != id) && ((sigclib_tabsolute() - tix) < timeout))
      {
       #if sigclib_version >= 31
        sigclib_thread_delay_us(500);
       #else
        sigclib_thread_delay(1);
       #endif
      }
        
      if(psync->last_id == id)
      {
        pcd_dst->messageId = memo; // ursprüngliche MessageId eintragen
        retcode = 1;
      }
    }
    
    if(pcd_src->messageId != memo) // falls ursprüngliche MessageId geändert wurde
    {
      pcd_src->messageId = memo; // ursprüngliche MessageId wiederherstellen
    }
    
    psync->pcd = NULL;
    sigclib_atomic_setU32(&psync->wait_id, DSC_MESSAGEID_DEFAULT); // waitfor flag zurücksetzen
  }
  
  return retcode;
}

// ************************************************************************************************
// ************************************************************************************************
// user-interface
// ************************************************************************************************
// ************************************************************************************************

void *DataLink_OpenEx(const char *ipv4, unsigned long taskpriority, unsigned long timeout_to_host, unsigned long clientid, tDscFctDataRx fct_data_rx, tDscFctDataTx fct_data_tx, tDscFctPlcState fct_plc_state, tDscFctTaskBeginEnd fct_task_begin_end, void *pcookie, const char *usertext)
{
  // Diese funktion initialisiert eine DataLink-Kommunikation
  // --> ipv4 ................ IpAdresse
  // --> taskpriority ........ Taskpriorität
  // --> timeout_to_host ..... Timeout innerhalb welcher Verbindung aufgebaut werden muss
  // --> clientid ............ Userspezifische ClientId
  // --> fct_data_rx ......... Callbackfunktion Rx
  // --> tct_data_tx ......... Callbackfunktion Tx
  // --> fct_plc_state ....... Callbackfunktion PlcState
  // --> fct_task_begin_end .. Callbackfunktion Task begin and Task has ended (Callback wird nur je einmalig aus Thread bei Start und End aufgerufen)
  // --> pcookie ............. Cookie
  // --> usertext ............ userspezifischer ascii-0-String (max.32 Zeichen incl.0-Terminierung)
  // <-- funktion retourniert ein gültiges Handle oder im Fehlerfall NULL
  // NOTE: Verbindungsaufbau wird nur gestartet. User muss auf Antwort vom Host warten. 
  //       Dh.: Erst wenn DSC_IsOnline() ungleich 0 zurückliefert kann mit der Datenübertragung begonnen werden.

  char str[64];
  unsigned long port = ComId_Port;
  
  if(ipv4 != NULL)
  {
    unsigned long len = sigclib_strlen(ipv4) + 1;
    if(len < sizeof(str))
    {
      sigclib_memcpy(str, ipv4, len);
      char *pport = sigclib_strchr(str, ':');
      if(pport != NULL)
      {
        port = sigclib_atoi(&pport[1]);
        *pport = 0;
      }
      ipv4 = str;
    }
  }

  if(ipv4 != NULL)
  {
    tDscData *pds = (tDscData*)sigclib_malloc(sizeof(tDscData));
    if(pds != NULL)
    {
      init_tDscData(pds, sigclib_tabsolute());

      if(usertext != NULL)
      {
        unsigned long len = sigclib_strlen(usertext);
        if(len >= sizeof(pds->usertext))
        {
          len = sizeof(pds->usertext) - 1;
        }
        sigclib_memcpy(pds->usertext, usertext, len);
        pds->usertext[len] = 0;
      }

      sigclib_strcpy(pds->ipv4, ipv4);
      pds->pcookie         = pcookie;
      pds->step            = DSC_STEP_START;
      pds->timeout_to_host = timeout_to_host;
      pds->fct_data_rx     = (fct_data_rx != NULL)? fct_data_rx : DataLink_DefaultDataRxFunct;
      pds->fct_data_tx     = fct_data_tx;
      pds->fct_plc_state   = (fct_plc_state != NULL)? fct_plc_state : DataLink_DefaultPlcStateFunct;
      pds->fct_task_begin_end  = fct_task_begin_end;
      pds->valid           = DSC_ID_VALID;
      pds->clientid        = clientid;
      pds->port            = port;
//      pds->fct_plc_state(pds, pds->pcookie, pds->plcstate); // userinterface cpu-state
    
      char tlabel[40];
      sigclib_sprintf(tlabel, "DataLinkClt%08X", pds);
      pds->task_hdl = sigclib_thread_create2((void*)DataLink_Thread, taskpriority, 1000, (void*)pds, tlabel);
      
      if(pds->task_hdl != 0)
      {
        return (void*)pds;
      }
      sigclib_free(pds);
    }
  }
  
  return NULL;
}

void *DataLink_Open(const char *ipv4, unsigned long taskpriority, unsigned long timeout_to_host, unsigned long clientid, tDscFctDataRx fct_data_rx, tDscFctDataTx fct_data_tx, tDscFctPlcState fct_plc_state, void *pcookie, const char *usertext)
{
  return DataLink_OpenEx(ipv4, taskpriority, timeout_to_host, clientid, fct_data_rx, fct_data_tx, fct_plc_state, NULL, pcookie, usertext);
}

void *DataLink_OpenSynEx(const char *ipv4, unsigned long taskpriority, unsigned long timeout_to_host, unsigned long clientid, tDscFctDataRx fct_data_rx, tDscFctDataTx fct_data_tx, tDscFctPlcState fct_plc_state, tDscFctTaskBeginEnd fct_task_begin_end, void *pcookie, const char *usertext)
{
  // Diese funktion erzeugt eine DataLink-Kommunikation und wartet bis Verbindung stabil ist
  // --> ipv4 ................ IpAdresse
  // --> taskpriority ........ Taskpriorität
  // --> timeout_to_host ..... Timeout innerhalb welcher Verbindung aufgebaut werden muss
  // --> clientid ............ Userspezifische ClientId
  // --> fct_data_rx ......... Callbackfunktion RxData
  // --> fct_data_tx ......... Callbackfunktion TxData
  // --> fct_plc_state ....... Callbackfunktion PlcState
  // --> fct_task_begin_end .. Callbackfunktion Task begin and Task has ended (Callback wird nur je einmalig aus Thread bei Start und End aufgerufen)
  // --> pcookie ............. Cookie
  // --> usertext ............ userspezifischer ascii-0-String (max.32 Zeichen incl.0-Terminierung)
  // <-- funktion retourniert ein gültiges Handle oder im Fehlerfall NULL
  // NOTE: Funktion blockiert bis Verbindung stabil aufgebaut wurde.

  unsigned long tix = sigclib_tabsolute();
  
  void *pds0 = DataLink_OpenEx(ipv4, taskpriority, timeout_to_host, clientid, fct_data_rx, fct_data_tx, fct_plc_state, fct_task_begin_end, pcookie, usertext);
  tDscData *pds = DataLink_Valid(pds0);
  if(pds != NULL)
  {
    do
    {
      if(DataLink_IsOnline(pds) != 0)
      {
        return pds0;
      }
      sigclib_thread_delay(1); // priorität abgeben
    }
    while((sigclib_tabsolute()-tix) <= timeout_to_host);
  
    DataLink_Close(pds0);
  }
  
  return NULL;
}

void *DataLink_OpenSyn(const char *ipv4, unsigned long taskpriority, unsigned long timeout_to_host, unsigned long clientid, tDscFctDataRx fct_data_rx, tDscFctDataTx fct_data_tx, tDscFctPlcState fct_plc_state, void *pcookie, const char *usertext)
{
  // Diese funktion erzeugt eine DataLink-Kommunikation und wartet bis Verbindung stabil ist
  // --> ipv4 .............. IpAdresse
  // --> taskpriority ...... Taskpriorität
  // --> timeout_to_host ... Timeout innerhalb welcher Verbindung aufgebaut werden muss
  // --> clientid .......... Userspezifische ClientId
  // --> fct_data_rx ....... Callbackfunktion RxData
  // --> fct_data_tx ....... Callbackfunktion TxData
  // --> fct_plc_state ..... Callbackfunktion PlcState
  // --> pcookie ........... Cookie
  // --> usertext .......... userspezifischer ascii-0-String (max.32 Zeichen incl.0-Terminierung)
  // <-- funktion retourniert ein gültiges Handle oder im Fehlerfall NULL
  // NOTE: Funktion blockiert bis Verbindung stabil aufgebaut wurde.

  return DataLink_OpenSynEx(ipv4, taskpriority, timeout_to_host, clientid, fct_data_rx, fct_data_tx, fct_plc_state, NULL, pcookie, usertext);
}

void *DataLink_Close(void *pds0)
{
  // diese Funktion schliesst eine zuvor aufgebaute Verbindung
  // --> pds0 .............. Handle
  // <-- funktion retourniert immer NULL

  tDscData *pds = DataLink_Valid(pds0);
  if(pds != NULL)
  {
    DataLink_CpuStateSet(pds, DSC_CPUSTATE_OFFLINE); // DSC_CPUSTATE_OFFLINE
    pds->valid     = 0; // ab jetzt darf handle nicht mehr verwendet werden
    pds->stepforce = DSC_STEP_LOGOUT;
  }
  
  return NULL;
}

unsigned long DataLink_ReOpen(void *pds0)
{
  // Mit dieser Funktion kann eine Verbindung neu aufgebaut werden.
  // --> pds0 .............. Handle
  // <-- funktion retourniert 1, ansonsten 0 (fehler)
  // NOTE: Voraussetzung für diese Funktion ist dass Handle noch nicht geschlossen wurde.

  tDscData *pds = DataLink_Valid(pds0);
  if(pds != NULL)
  {
    pds->stepforce = DSC_STEP_START;
    return 1;
  }
  
  return 0;
}

unsigned long DataLink_Send(void *pds0, DsComData *pdata)
{
  // Daten an SPS senden
  // --> pds0 .............. Handle
  // --> pdata ............. zu sendendes Datenpaket
  // <-- funktion retourniert 1, ansonsten 0 (Fehler)
  
  tDscData *pds = DataLink_Valid(pds0);
  if((pdata != NULL) && (pds != NULL))
  {
    if(DataLink_SendIntern(pds, pdata) >= 0)
    {
      return 1;
    }
  }
  return 0;
}

unsigned long DataLink_SendSyn(DsComData *pcd_dst, void *pds0, DsComData *pcd_src, unsigned long timeout)
{
  // funktion dient zum synchronen schicken von Datenpaket (auf Antwort warten)
  // --> pcd_dst ........... Destinationspeicher für Antwortpaket
  // --> pds0 .............. Handle
  // --> pcd_src ........... zu sendendes Datenpaket
  // --> timeout ........... Timeout innerhalb welchem die Antwort erwartet wird 
  // bei Erfolg retourniert diese Funktion 1, ansonsten 0

  tDscData *pds = DataLink_Valid(pds0);
  if((pcd_src != NULL) && (pcd_dst != NULL) && (pds != NULL))
  {
    if(timeout == 0)
    {
      timeout = 100;
    }
    
    unsigned long id = pcd_src->messageId;
    if(id == DSC_MESSAGEID_DEFAULT) // falls default, dann neue id erzeugen
    {
      id = sigclib_atomic_incU32(&pds->synchron_id_cnt);
      if(id == DSC_MESSAGEID_DEFAULT)
      { 
        id = (DSC_MESSAGEID_DEFAULT ^ 0x80000000);
      }
    }
    
    unsigned long retcode = DataLink_SendSynIntern(pds, &pds->synchron[0], id, pcd_dst, pcd_src, timeout);
    if(retcode == 0)
    {
      retcode = DataLink_SendSynIntern(pds, &pds->synchron[1], id, pcd_dst, pcd_src, timeout);
    }  
    
    return (retcode == 1)? 1 : 0;
  }
  
  return 0;
}

unsigned long DataLink_IsOnline(void *pds0)
{
  // Überprüfen ob Kommunikation stabil ist
  // --> pds0 .............. Handle
  // <-- funktion retourniert 1 wenn Kommunikation stabil, ansonsten 0 (Fehler)

  tDscData *pds = DataLink_Valid(pds0);
  if(pds != NULL)
  {
    if(pds->step == DSC_STEP_RUN)
    {
      switch(pds->plcstate)
      {
        case DSC_CPUSTATE_RUN_RAM :
        case DSC_CPUSTATE_RUN_ROM :
        case DSC_CPUSTATE_STOP_BRKPT :
        case DSC_CPUSTATE_SINGLESTEP : 
          return 1;
      }
    }
  }
  
  return 0;
}

unsigned long DataLink_GetClientId(void *pds0)
{
  // Funktion retourniert die Clientid
  // --> pds0 .............. Handle
  // <-- Die entsprechende clientid wird retourniert
  
  tDscData *pds = DataLink_Valid(pds0);
  if(pds != NULL)
  {
    return pds->clientid;
  }
  
  return 0;
}

unsigned long DataLink_GetDscCrc(void *pds0)
{
  // Funktion retourniert DescriptorCrc von CPU
  // --> pds0 .............. Handle
  // <-- Die entsprechende DscCrc wird retourniert, im Fehlerfall 0
  
  tDscData *pds = DataLink_Valid(pds0);
  if(pds != NULL)
  {
    if(DataLink_IsOnline(pds0) != 0)
    {
      return pds->dsccrc;
    }
  }
  
  return 0;
}

char *DataLink_GetUserText(void *pds0)
{
  // Funktion retourniert Usertext welcher bei Funktion DataLink_Open() angegeben werden kann
  // --> pds0 .............. Handle
  // <-- Pointer auf Usertext (ASCI-0-String), im Fehlerfall (Handle ungültig) NULL

  tDscData *pds = DataLink_Valid(pds0);
  if(pds != NULL)
  {
    return pds->usertext;
  }
  
  return NULL;
}

long DataLink_GetPlcState(void *pds0)
{
  // Funktion retourniert den CPU-Status
  // --> pds0 .............. Handle
  // <-- Der entsprechende CPU-Status wird retourniert
  
  tDscData *pds = DataLink_Valid(pds0);
  if(pds != NULL)
  {
    return pds->plcstate;
  }
  
  return DSC_CPUSTATE_OFFLINE;
}

unsigned long DataLink_NewInstCall(void *pds0, unsigned long userid, unsigned long lasalid, unsigned short cmd, void *pdata, unsigned long datalen)
{
  // Aufruf vom Newinst in SPS
  // --> pds0 .............. Handle
  // --> userid ............ UserId welche auch im Antwortpaket wieder zu finden ist
  // --> lasalid ........... LasalId des Empfangsobjektes (Lasalid vom NewInst-Server)
  // --> cmd ............... NewInst-Befehl
  // --> pdata ............. Datenstream welcher an NewInst übergeben wird
  // --> datalen ........... Bytelänge des Datenstreams 
  // bei Erfolg retourniert diese Funktion 1, ansonsten 0

  // NOTE: Antwortpaket ist folgendermassen aufgebaut:
  // [ ..... DSComDataHdr
  // [4] ... userid
  // [4] ... result (iprstate)
  // [2] ... datalength (results.uilen) immer 2 + eigentliche datalen in bytes
  // [ ..... data

  unsigned long retcode = 0;
  unsigned char tmp[256+16];
  unsigned long len = datalen + 15;
  
  if(lasalid != 0)
  {
    DsComData *pcd = DsComData_CreateEx(tmp, sizeof(tmp), len + sizeof(DsComDataHdr), ComId_Major_DS, ComCmd_NewInst);
    if(pcd != NULL)
    {
      DsComData_AddU32(pcd, len, userid);
      DsComData_AddU32(pcd, len, len);
      DsComData_AddU32(pcd, len, lasalid);
      DsComData_AddU16(pcd, len, cmd);
      DsComData_AddU08(pcd, len, 19); // P_USER_STREAM
      DsComData_AddData(pcd, len, pdata, datalen);
      retcode = DataLink_Send(pds0, pcd);
      DsComData_Destroy(tmp, pcd);
    }
  }
  
  return retcode;
}

unsigned long DataLink_NewInstCallSyn(DsComData *pcd_dst, void *pds0, unsigned long userid, unsigned long lasalid, unsigned short cmd, void *pdata, unsigned long datalen, unsigned long timeout)
{
  // Synchroner Aufruf vom Newinst in SPS
  // --> pcd_dst ........... Destinationspeicher für Antwortpaket
  // --> pds0 .............. Handle
  // --> userid ............ UserId welche auch im Antwortpaket wieder zu finden ist
  // --> lasalid ........... LasalId des Empfangsobjektes (Lasalid vom NewInst-Server)
  // --> cmd ............... NewInst-Befehl
  // --> pdata ............. Datenstream welcher an NewInst übergeben wird
  // --> datalen ........... Bytelänge des Datenstreams 
  // --> timeout ........... Timeout innerhalb welchem die Antwort erwartet wird 
  // bei Erfolg retourniert diese Funktion 1, ansonsten 0

  unsigned long retcode = 0;
  unsigned char tmp[256+16];
  unsigned long len = datalen + 1 + 14;
  
  if(lasalid != 0)
  {
    DsComData *pcd = DsComData_CreateEx(tmp, sizeof(tmp), len + sizeof(DsComDataHdr), ComId_Major_DS, ComCmd_NewInst);
    if(pcd != NULL)
    {
      DsComData_AddU32(pcd, len, userid);
      DsComData_AddU32(pcd, len, len);
      DsComData_AddU32(pcd, len, lasalid);
      DsComData_AddU16(pcd, len, cmd);
      DsComData_AddU08(pcd, len, 19); // P_USER_STREAM
      DsComData_AddData(pcd, len, pdata, datalen);
      retcode = DataLink_SendSyn(pcd_dst, pds0, pcd, timeout);
      DsComData_Destroy(tmp, pcd);
    }
  }
  
  return retcode;
}

unsigned long DataLink_SendCmdLoop(void *pds0, void *loopdata, unsigned long loopdatasize)
{
  // diese Funktion sendet ein Datenpaket zur SPS und dieses Datenpaket wird sofort wieder von SPS retourniert
  // das Antwortpaket ist mit der MinorId "ComCmd_Loop_Result" getagged und beinhaltet die selben Daten wie das Sendepaket
  // --> pds0 .............. Handle
  // --> loopdata .......... Datenpaket
  // --> loopdatasize ...... Bytelänge des Datenpaketes 
  // bei Erfolg retourniert diese Funktion 1, ansonsten 0

  unsigned long retcode = 0;
  unsigned char tmp[256];
  DsComData *pcd = DsComData_CreateEx(tmp, sizeof(tmp), loopdatasize, ComId_Major_DS, ComCmd_Loop);
  if(pcd != NULL)
  {
    DsComData_AddData(pcd, loopdatasize, loopdata, loopdatasize);
    
    retcode = DataLink_Send(pds0, pcd);
    
    DsComData_Destroy(tmp, pcd);
  }
  
  return retcode;
}

unsigned long DataLink_SendCmdLoopSyn(DsComData *pcd_dst, void *pds0, void *loopdata, unsigned long loopdatasize, unsigned long timeout)
{
  // diese Funktion sendet ein Datenpaket zur SPS und wartet auf die Antwort. (Das Datenpaket wird sofort wieder von SPS retourniert)
  // das Antwortpaket ist mit der MinorId "ComCmd_Loop_Result" getagged und beinhaltet die selben Daten wie das Sendepaket
  // --> pcd_dst ........... Destinationspeicher für Antwortpaket
  // --> pds0 .............. Handle
  // --> loopdata .......... Datenpaket
  // --> loopdatasize ...... Bytelänge des Datenpaketes
  // --> timeout ........... Timeout innerhalb welchem die Antwort erwartet wird
  // bei Erfolg retourniert diese Funktion 1, ansonsten 0

  unsigned long retcode = 0;
  unsigned char tmp[256];
  DsComData *pcd = DsComData_CreateEx(tmp, sizeof(tmp), loopdatasize, ComId_Major_DS, ComCmd_Loop);
  if(pcd != NULL)
  {
    DsComData_AddData(pcd, loopdatasize, loopdata, loopdatasize);
    
    retcode = DataLink_SendSyn(pcd_dst, pds0, pcd, timeout);
    
    DsComData_Destroy(tmp, pcd);
  }
  
  return retcode;
}

unsigned long DataLink_GetLasalId(void *pds0, const char *label, unsigned long varid)
{
  unsigned long retcode = 0;
  
  if(label != NULL)
  {
    unsigned char tmp[128];
    unsigned long len = sigclib_strlen(label) + 1;
    unsigned long bytesize = len + 8;
    
    DsComData *pcd = DsComData_CreateEx(tmp, sizeof(tmp), bytesize, ComId_Major_DS, ComCmd_GetLasalId);
    if(pcd != NULL)
    {
      DsComData_AddU32(pcd, bytesize, 1);
      DsComData_AddU32(pcd, bytesize, varid);
      DsComData_AddData(pcd, bytesize, (void*)label, len);
      
      retcode = DataLink_Send(pds0, pcd);
      
      DsComData_Destroy(tmp, pcd);
    }  
  }
  
  return retcode;
}

unsigned long DataLink_GetLasalIdSyn(void *pds0, const char *label, unsigned long timeout)
{
  unsigned long retcode = 0;
  
  if(label != NULL)
  {
    unsigned char tmp[128];
    unsigned long len = sigclib_strlen(label) + 1;
    unsigned long bytesize = len + 8;
    
    DsComData *pcd = DsComData_CreateEx(tmp, sizeof(tmp), bytesize, ComId_Major_DS, ComCmd_GetLasalId);
    if(pcd != NULL)
    {
      const unsigned long varid = 0xDEAFBABE;
      DsComData_AddU32(pcd, bytesize, 1);
      DsComData_AddU32(pcd, bytesize, varid);
      DsComData_AddData(pcd, bytesize, (void*)label, len);
      
      unsigned char dst[128];
      DsComData *pcd_dst = (DsComData*)dst;
      
      if(DataLink_SendSyn(pcd_dst, pds0, pcd, timeout) != 0)
      {
        // [4] .... anzahl
        // *[4] ... userid
        // *[4] ... lasalid
        // *[1] ... mode
      
        if((pcd_dst->id_major == ComId_Major_DS) && (pcd_dst->id_minor == ComCmd_GetLasalId_Result))
        {
          unsigned long tmp;
          CopyU32Makro(&tmp, &pcd_dst->data[4]);
          if(tmp == varid)
          {
            CopyU32Makro(&retcode, &pcd_dst->data[8]);
          }
        }
      }
      
      DsComData_Destroy(tmp, pcd);
    }  
  }
  
  return retcode;
}

