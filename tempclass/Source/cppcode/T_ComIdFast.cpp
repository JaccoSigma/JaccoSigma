//<xxxNewSigmatekCFileOptimize/>
// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 05.03.2020                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#include "T_IPC.H"
#include "T_NUC.H"

#ifdef UC_GETLASALID_DATALINK

#include "..\..\Class\DataServiceClient\CDsClient.h"
#include "..\..\Class\SigCLib\SigCLib.h"

#define DSC_VALID_LOADERVERSION   0x22D7
#define DEFAULT_FASTLID 0xFFFEBAB7  // muss ungerade sein !!


typedef void (*tSaveDsComData) (DsComData*, unsigned long);
unsigned long LoaderDone = 0;

typedef struct
{
  void         *hdl;
  _COMPORT     *pcp;
  DsComData    *pcd;          // pointer auf tx-puffer
  unsigned long userdatasize; // userdatagrösse im tx-puffer
  unsigned long no;           // aktuelle anzahl im tx-puffer
  unsigned long no_rx;        // gesamtanzahl der gesendeten
  unsigned long no_tx;        // gesamtanzahl der empfangenen
  
  unsigned long no_tmp_tx;
  
  unsigned long scratch[2000]; // scratchdata welcher als tx-puffer verwendet wird wurde von 4000 auf 2000 verkleinert, Applikation Demag + Franz Aschl
  unsigned long plcstate;     // plcstate
}
tDataLink;

typedef struct
{
  tDataLink     *ptr;
  unsigned long no;
}
tMemDataLink;

static void init_tDataLink(tDataLink *p)
{
  p->hdl          = NULL;
  p->pcp          = NULL;
  p->userdatasize = DsComData_UserDataSize(sizeof(p->scratch));
  p->pcd          = DsComData_Create(p->scratch, sizeof(p->scratch), p->userdatasize, ComCmd_GetLasalId);
  p->no           = 0;
  p->no_rx        = 0; // gesamtanzahl der gesendeten
  p->no_tx        = 0; // gesamtanzahl der empfangenen
  p->no_tmp_tx    = 0;
  p->plcstate     = 0xFFFFFFFF;
}

static void free_tDataLink(tDataLink *p)
{
  p->hdl          = NULL;
  p->pcp          = NULL;
  p->userdatasize = 0;
  p->pcd          = DsComData_Destroy(p->scratch, p->pcd);
  p->no           = 0;
  p->no_rx        = 0; // gesamtanzahl der gesendeten
  p->no_tx        = 0; // gesamtanzahl der empfangenen
  p->no_tmp_tx    = 0;
  p->plcstate     = 0xFFFFFFFF;
}

void FctDataRx(void *phdl, void *pcookie, DsComData *pcd)
{
  if(pcookie != NULL)
  {
    tDataLink *pdl = (tDataLink*)pcookie;
   
   #pragma pack(push, 1)
    typedef struct
    {
      uint32 varno;
      uint32 lid;
      uint08 typ;
    } tVli;
   #pragma pack(pop) 
    
    if(pcd->id_minor == ComCmd_GetLasalId_Result)
    {
      uint32 anz = *(uint32*)&pcd->data[0];
      tVli *pvli = (tVli*)&pcd->data[4];
      
      while(anz--)
      {
        _LSEVARINFO *pvi;
        if(VarList.GetVarInfo(&pvi, pvli->varno) == _TRUE)
        {
          pvi->lasalid.id = (pvli->lid == 0)? DEFAULT_FASTLID : pvli->lid;
          pvi->lasalid.vtype = (pvi->vartype == FSTRING)? TYP_EXTENDED : TYP_STANDARD;
        }
        
        pvli ++;
        pdl->no_rx ++;
        
        if(pdl->no_tmp_tx > 0)
        {
          pdl->no_tmp_tx --;
        }
      }
    }
  }
}

//void FctDataTx(void *phdl, void *pcookie, DsComData *pcd)
//{
//  tDataLink *pmdl = (tDataLink*)pcookie;
//}

void FctPlcState(void *phdl, void *pcookie, long plcstate)
{
  tDataLink *pdl = (tDataLink*)pcookie;
  pdl->plcstate = plcstate;
}

static _BOOL SendInternAndWaitFor(tDataLink *pdl, _BOOL post_wait)
{
  if(pdl->no > 0)
  {
    if(pdl->no_tmp_tx > 0)
    {
      unsigned long tix = sigclib_tabsolute();
      do
      {
        if(pdl->no_tmp_tx == 0)
        {
          break;
        }
        lib_sleep(3);
      }
      while((sigclib_tabsolute() - tix) < 2000);
    }
  
    pdl->no_tmp_tx = pdl->no;
    if(DataLink_Send(pdl->hdl, pdl->pcd) != 0)
    {
      DsComData_Empty(pdl->pcd);
      DsComData_AddU32(pdl->pcd, pdl->userdatasize, 0); // anzahl auf 0 setzen
      pdl->no_tx += pdl->no;
      pdl->no     = 0;
      
      if(post_wait == _TRUE)
      {
        unsigned long tix = sigclib_tabsolute();
        do
        {
          if(pdl->no_rx >= pdl->no_tx)
          {
            return _TRUE;
          }
          lib_sleep(3);
        }
        while((sigclib_tabsolute() - tix) < 2000);
        return _FALSE;
      }
      
      return _TRUE;
    }

    return _FALSE;
  }
  
  return _TRUE;
}

/*
static _BOOL WaitForPlc_Response(tMemDataLink *pmdl, _UDWORD timeout)
{
  unsigned long tix = sigclib_tabsolute();

  do
  {
    unsigned long count = 0;
    unsigned long no = pmdl->no;
    tDataLink *pdl = pmdl->ptr;
    
    while(no--)
    {
      SendInternAndWaitFor(pdl, _TRUE);
  
      if(pdl->no_rx >= pdl->no_tx)
      {
        count ++;
        if(pdl->pcp->lasalid_done == 0)
        {
          pdl->pcp->lasalid_done = 1; // lasalid's sind bereits alle ermittelt
        }
      }
  
      if(count == pmdl->no)
      {
        return _TRUE;
      }
  
      pdl++; 
    }
  }
  while((sigclib_tabsolute() - tix) < timeout);

  return _FALSE;
}
*/

static _BOOL WaitForPlc_Response(tMemDataLink *pmdl, _UDWORD timeout)
{
  unsigned long count = 0;
  unsigned long no = pmdl->no;
  tDataLink *pdl = pmdl->ptr;
    
  while(no--)
  {
    SendInternAndWaitFor(pdl, _TRUE);
    if(pdl->no_rx >= pdl->no_tx)
    {
      count ++;
    }
    pdl->pcp->lasalid_done = 1; // lasalid's sind bereits alle ermittelt
    pdl++; 
  }

  return (count == pmdl->no)? _TRUE : _FALSE;
}

static void CloseAllDataLink(tMemDataLink *pmdl)
{
  tDataLink *pdl = pmdl->ptr;
  for(_UDWORD i=0; i<pmdl->no; i++)
  {
    pdl->hdl               = DataLink_Close(pdl->hdl);
    pdl->pcp->datalink_hdl = NULL;
    free_tDataLink(pdl);
    
    pdl++;
  }
}

static uint32 SendName(tDataLink *pdl, uint32 varno, _LSEVARINFO *pvi)
{
  if(pdl != NULL)
  {
    _LSEVARINFO *pvi_root = VarList.Get_LSEVARINFO_RootParent(varno);
    if(pvi_root != NULL)
    {
      if(pvi_root->lasalid.id == DEFAULT_FASTLID)
      {
        // falls sein Rootparent bereits abgefragt ist und nicht existiert, existiert auch Child nicht
        pvi->lasalid.id = DEFAULT_FASTLID;
        return 1;
      }
    }
  
    char label[260];
    _UBYTE varobj;
    
    if(VarList.GetLabel(&label[4], &varobj, varno) == _TRUE)
    {
      if(pdl->no == 0)
      {
        DsComData_Empty(pdl->pcd);
        DsComData_AddU32(pdl->pcd, pdl->userdatasize, 0); // anzahl
      }
    
      *(unsigned long*)&label[0] = varno;
      uint32 len = StrLen(&label[4], sizeof(char)) + 4 + 1;
    
//      if((DsComData_Rest(pdl->pcd, pdl->userdatasize) < len) || (pdl->no >= 110))
//      {
//        SendInternSyn(pdl);
//      }
      
      if(DsComData_Rest(pdl->pcd, pdl->userdatasize) < len)
      {
        SendInternAndWaitFor(pdl, _FALSE);
      }
      
      DsComData_AddData(pdl->pcd, pdl->userdatasize, label, len);
      DsComData_IncU32(pdl->pcd, pdl->userdatasize, 0); // inc anzahl
      pdl->no ++;
    }
    
    return 1;
  }
  
  return 0;
}

static _BOOL FetchValidLasalID(_LSEVARINFO *pvi)
{
  if((pvi->vartype != FCONSTANT) && (pvi->vartype != FMERKER) && (pvi->vartype != FENUMITEM) && (pvi->vartype != BIT))
  {
    if((pvi->POWPRUIC & VARBIT_VARMEMBER) == 0) // nur wenn keine membervariable
    {
      if((pvi->POWPRUIC & VARBIT_STRUCTELEMENT) == 0) // nur wenn kein strukturelement
      {
        if(pvi->lasalid.id == 0) // neu
        {
          return _TRUE;
        }
      }
    }
  }
  return _FALSE;
}

static void FinalizeScanId(void)
{
  _UDWORD anz = VarList.GetNo();
    
  for(_UDWORD i=0; i<anz; i++)
  {
    _LSEVARINFO *p;
    _VARSYMBOL  *pvs;
    _LSEVARINFO *pvi;
  
    if(VarList.GetVarInfo(&p, i) == _TRUE)
    {
      // id aller bitserver ermitteln wenn lasalid ungültig und scope auf
      // bit datentyp (zB.: BINT etc.) zeigt.
      if(p->vartype == BIT)                               // server buss BIT sein
      {
        if(chk_LASALID_Makro(&p->lasalid) == _FALSE)        // lasalid muss _false sein
        {
          if(VarList.GetVarSymbol(&pvs, i) == _TRUE)                   // scope ermitteln
          {
            if(VarList.GetVarInfo(&pvi, pvs->scope) == _TRUE)  // varinfo von scope holen
            {
              if(pvi->vartype == BINARY)               // vartyp muss binary sein
              {
                if((pvi->format == SIZE_BDWORD)||(pvi->format == SIZE_BWORD)||(pvi->format == SIZE_BBYTE))
                {
                  p->lasalid = pvi->lasalid;
                }
              }
            }
          }
        }
      }
      else if(p->POWPRUIC & VARBIT_STRUCTELEMENT)
      {
        if(VarList.GetVarSymbol(&pvs, i) == _TRUE)                   // scope ermitteln
        {
          if(VarList.GetVarInfo(&pvi, pvs->scope) == _TRUE)  // varinfo von scope holen
          {
            p->lasalid = pvi->lasalid;
          }
        }
      }
    }
  }
}

_BOOL GetLasalIdUsingDataLink(_UWORD sb, _BOOL bar)
{

 #ifdef UC_GLOBAL_VAR 
  return _FALSE;
 #endif
 #ifdef UC_GLOBAL_VAR_REFLIST
  return _FALSE;
 #endif
 #ifdef CANOPEN
  return _FALSE;
 #endif
 #ifdef OLDSTYLE_CAN
  return _FALSE;
 #endif
 #ifdef UC_MULTICPU
  return _FALSE;
 #endif  

 #define PDLMAXNO 32
  
  tMemDataLink mdl;
  
  mdl.ptr = (tDataLink*)lib_calloc(sizeof(tDataLink) * PDLMAXNO);
  mdl.no = 0;

  uint32 anz_extern = 0;
  
  if(mdl.ptr != NULL)
  {
    for(_UDWORD station=0; station<256; station++)
    {
      _COMPORT *pcp = Batch.Get_COMPORT(station);
      if(pcp != NULL)
      {
        if(pcp->loader_version >= DSC_VALID_LOADERVERSION)
        {
          // aktuell werden die lokalen lasalids nicht mit datalink protokoll abgeholt, comlink ist schneller auf salamander opsys
          if((pcp->comdef.typ == _COM_TCPIP1_P) || (pcp->comdef.typ == _COM_TCPIP2_P) || (pcp->comdef.typ == _COM_INTERN) || (pcp->comdef.typ == _COM_LOCAL))
          {
            if((pcp->datalink_hdl == NULL) && (mdl.no < PDLMAXNO))
            {
              tDataLink *pdl = &mdl.ptr[mdl.no];
              init_tDataLink(pdl);
            
              char ipv4[28];
            
              if((pcp->comdef.typ == _COM_INTERN) || (pcp->comdef.typ == _COM_LOCAL))
              {
                sigclib_strcpy(ipv4, "127.0.0.1");
              }
              else
              {
                sigclib_tcp_ulongtostr(ipv4, sizeof(ipv4), pcp->comdef.ipadress);
              }
            
              void *hdl = DataLink_OpenSyn(ipv4, 12, 3000, 0x1000+station, FctDataRx, NULL, FctPlcState, pdl, NULL);
              if(hdl != NULL)
              {
                pdl->hdl = hdl;
                pdl->pcp = pcp;
                pcp->datalink_hdl = (void*)pdl; //&mdl.ptr[mdl.no];
                mdl.no++;
                
                if((pcp->comdef.typ != _COM_INTERN) && (pcp->comdef.typ != _COM_LOCAL))
                {
                  anz_extern ++;
                }
              }
            }
          }
        }
      }
    }
  
    if(mdl.no > 0)
    {
      if(anz_extern > 0) // nur wenn auch eine externe station vorhanden ist
      {
        _UDWORD scanbarstate = 0;
        _UDWORD anz = VarList.GetNo();
  
        // Zuerst nur alle RootElemente (zB. Objekte) abfragen ob überhaupt existent. 
        // Wenn nicht existieren die Childs (zB. EmbObj oder Svr) auch nicht
  
        _UDWORD effort = 0;
        for(_UDWORD i=0; i<anz; i++)
        {
          _VARSYMBOL *pvs;
          if(VarList.GetVarSymbol(&pvs, i) == _TRUE)
          {
            if(pvs->scope == DEFSCOPE) // nur RootElement
            {
              _LSEVARINFO *pvi;
              if(VarList.GetVarInfo(&pvi, i) == _TRUE)
              {
                if(FetchValidLasalID(pvi) == _TRUE)
                {
                  _COMPORT *pcp = Batch.Get_COMPORT(pvi->station);
                  if(pcp != NULL)
                  {
                    if(pcp->datalink_hdl != NULL)
                    {
                      tDataLink *pdl = (tDataLink*)pcp->datalink_hdl;
                      SendName(pdl, i, pvi);
                  
                      if(scanbarstate == 0)
                      {
                        scanbarstate = 1;
                        ScanBar.Reset(sb, anz, _TRUE);
                        ScanBar.ColorSet(sb, LIGHTCYAN);
                      }
                      
                      ScanBar.Run(sb, effort++);
                    }
                  }
                }
              }
            }
          }
        }
        
        if(effort != 0)
        {
          WaitForPlc_Response(&mdl, 2000); // warte auf letztes Antwortpaket
        }
  
        for(_UDWORD i=0; i<anz; i++)
        {
          _LSEVARINFO *pvi;
        
          if(VarList.GetVarInfo(&pvi, i) == _TRUE)
          {
            if(FetchValidLasalID(pvi) == _TRUE)
            {
              _COMPORT *pcp = Batch.Get_COMPORT(pvi->station);
              if(pcp != NULL)
              {
                if(pcp->datalink_hdl != NULL)
                {
                  tDataLink *pdl = (tDataLink*)pcp->datalink_hdl;
                  SendName(pdl, i, pvi);
                  
                  if(scanbarstate == 0)
                  {
                    scanbarstate = 1;
                    ScanBar.Reset(sb, anz, _TRUE);
                    ScanBar.ColorSet(sb, LIGHTCYAN);
                  }
                  
                  if(i > effort)
                  {
                    effort = i;
                  }
                  
                  ScanBar.Run(sb, effort++);
                }
              }
            }
          }
        }
        
        WaitForPlc_Response(&mdl, 2000); // warte auf letztes Antwortpaket
        
        FinalizeScanId(); // scan finalize
        
        if(scanbarstate != 0)
        {
          ScanBar.Run(sb, anz);
        }
      }
      
      CloseAllDataLink(&mdl);
      lib_sleep(10);
    }
    
    mdl.ptr = (tDataLink*)lib_free(mdl.ptr);
    mdl.no  = 0;
    
    return _TRUE;
  }
  
  return _FALSE;
}

void FixLasalIdUsingDataLink(void)
{
 #ifdef UC_GLOBAL_VAR 
  return _FALSE;
 #endif
 #ifdef UC_GLOBAL_VAR_REFLIST
  return _FALSE;
 #endif
 #ifdef CANOPEN
  return _FALSE;
 #endif
 #ifdef OLDSTYLE_CAN
  return _FALSE;
 #endif
 #ifdef UC_MULTICPU
  return _FALSE;
 #endif

  _UDWORD anz = VarList.GetNo();
    
  for(_UDWORD i=0; i<anz; i++)
  {
    _LSEVARINFO *pvi;
    
    if(VarList.GetVarInfo(&pvi, i) == _TRUE)
    {
      if(pvi->lasalid.id == DEFAULT_FASTLID)
      {
        pvi->lasalid.id = 0;
      }
    }
  }
}

#endif

