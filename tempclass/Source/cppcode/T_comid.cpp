#include "T_IPC.H"
#include "T_NUC.H"

#define LIDMAXSERVER 40
#define NEWLOADER_VERSION 0x225C

typedef struct { _UDWORD    object;
                 _UDWORD    server[LIDMAXSERVER];
                 _UBYTE     no;
               }
_LIDENTRY;

typedef struct { _UDWORD    no;
                 _UDWORD    gesamt;
                 _UDWORD    used;
                 _LIDENTRY  *ptr;
                 _UDWORD    *ptrindex;
                 _UBYTE     *ptrvalid;
               }
_MEM_LIDENTRY;


_BOOL SendEntry(_MEM_LIDENTRY *p, _LIDENTRY *plid)
{
  _UBYTE  varobj;
  _ASCII  label[256];
  _ASCII  *token;
  _UWORD  pos, mempos, len;
  _UWORD  i, anz, start;
  _BOOL   retcode = _FALSE;
  _UDWORD varno;

  _LASALID res[50];  
  _LSEVARINFO *pvi, *tmp;
  _COMPORT *cp;
    
  if(VarList.GetVarInfo(&pvi, plid->object) == _TRUE)
  {
    cp = Batch.Get_COMPORT(pvi->station);
    if(cp != NULL)
    {
      if(VarList.GetLabel(&label[1], &varobj, plid->object) == _TRUE)
      {
        mempos = StrLen(&label[1], 1);
        label[0] = (_UBYTE)mempos;
        mempos += 1;
        i = 0;
    
        XYSendStart:     
         
        start = i;
        anz = 0;
        pos = mempos;
        for(; i<plid->no; i++)
        {
          token = VarList.GetLabelToken(plid->server[i]);
          if(token != NULL)
          {
            len = StrLen(token, 1);
            if((pos + len) < 240)
            {
              label[pos] = (_UBYTE)len;
              pos += 1;
              MemCpy(&label[pos], token, len);
              pos += len;
              label[pos] = 0;
              anz += 1;
            }
            else
            {
              break;
            }
          }
        }
        
        
        // format:
        // [1] .. länge von token
        // [ .... token (objektname ohne abschliessende 0)
        // [1] .. länge von token
        // [ .... token (servername1 ohne abschliessende 0)
        // [1] .. länge von token
        // [ .... token (servername2 ohne abschliessende 0)
        // ...... 
        // [1] .. 0 (abschliessende 0)
        
        // send stream to plc
        if(GetAddressListObj(label, pos+1, res, anz+1, &cp->comdef) == _TRUE)
        {
          retcode = _TRUE;
          
          // empfangene lasalid's in serverliste eintragen
          if(VarList.GetVarInfo(&tmp, plid->object) == _TRUE)
          {
            tmp->lasalid = res[0];
            p->ptrvalid[plid->object] = 1;
          }
          
          for(_UWORD n=0; n<anz; n++)
          {
            varno = plid->server[start + n];
            if(VarList.GetVarInfo(&tmp, varno) == _TRUE)
            {
              tmp->lasalid = res[n+1];
              if(tmp->vartype == FSTRING)
                tmp->lasalid.vtype = TYP_EXTENDED;
                
              p->ptrvalid[varno] = 1;
            }
          }
        }
      }
    
      if(i < plid->no)
        goto XYSendStart;
    }
  }

  plid->no = 0;  
  return retcode;
}

void init_LIDENTRY(_LIDENTRY *p)
{
  p->object = 0xFFFFFFFF;
  p->no = 0;
}

_BOOL init_MEM_LIDENTRY(_MEM_LIDENTRY *p, _UDWORD anz, _UDWORD gesamt)
{
  _BOOL retcode = _FALSE;
  _UDWORD i;

  p->used = 0;
  p->no = 0;
  p->gesamt = 0;
  p->ptr = NULL;
  p->ptrindex = NULL;
  p->ptrvalid = NULL;

  if(Memory2((void**)&p->ptrindex, gesamt * sizeof(_UDWORD)) != 0)
  {
    MemSet(p->ptrindex, 0xFF, gesamt * sizeof(_UDWORD));
    if(Memory2((void**)&p->ptrvalid, gesamt) != 0)
    {
      MemSet(p->ptrvalid, 0, gesamt);
      if(Memory2((void**)&p->ptr, anz * sizeof(_LIDENTRY)) != 0)
      {
        for(i=0; i<anz; i++)
          init_LIDENTRY(&p->ptr[i]);
        p->no = anz;
        p->gesamt = gesamt;
        retcode = _TRUE;
      }
    }
  }
  
  return retcode;
}


void free_MEM_LIDENTRY(_MEM_LIDENTRY *p)
{
  Memory2((void**)&p->ptr, 0);
  Memory2((void**)&p->ptrindex, 0);
  Memory2((void**)&p->ptrvalid, 0);
  
  p->used = 0;
  p->no = 0;
  p->gesamt = 0;
}

_BOOL AddObject(_MEM_LIDENTRY *p, _UDWORD objno)
{
  if((p->used < p->no)&&(objno < p->gesamt))
  {
    p->ptr[p->used].object = objno;
    p->ptrindex[objno] = p->used;
    
    p->used += 1;
    return _TRUE;
  }
  
  return _FALSE;
}

_LIDENTRY *FindObject(_MEM_LIDENTRY *p, _UDWORD objno)
{
  if(objno < p->gesamt)
  {
    _UDWORD index = p->ptrindex[objno];
    if(index < p->no)
      return &p->ptr[index];
  }
  return NULL;
}

_BOOL AddServerToObject(_MEM_LIDENTRY *p, _UDWORD objno, _UDWORD serverno)
{
  _LIDENTRY *plid = FindObject(p, objno);
  
  if(plid != NULL)
  {
    plid->server[plid->no] = serverno;
    plid->no += 1;
    if(plid->no == LIDMAXSERVER)
      SendEntry(p, plid);
    return _TRUE;
  }
  return _FALSE;
}

_UDWORD GetLoaderVersion(_COMPORT *pcp)
{
  _UDWORD  retcode = 0;
  _UBYTE   *result = NULL;
  
  if(pcp != NULL)
  {
  
   #ifdef UC_EXOONLINE
    if(IsExoOnline(&pcp->comdef))
    {
      return ExoGetLoaderVersion(&pcp->comdef);
    }
   #endif
  
    retcode = pcp->loader_version;
    if(pcp->loader_version == 0)
    {
      if(TxCommandIntern(I_GET_VERSION, 0, NULL, &pcp->comdef, (void**)&result, DEFSVRTYP) == DONE)
      {
        if(result != NULL)
        {
          if(*(_UWORD*)result == 4)
          {
            result += 2;
            retcode = *(_UWORD*)result;
            pcp->loader_version = retcode;
          }
        }
      }
    }
  }
  
  return retcode;
}


void SendVisuReady()
{
  _UWORD   i, no = Batch.GetNo_COMPORT();
  _COMPORT *pcp;
  _UBYTE   *result = NULL;

  for(i=0; i<no; i++)
  {
    pcp = Batch.GetComportDirect(i);
    if(pcp != NULL)
    {
      if(pcp->reflist.login == _LOGIN_TRUE)
      {
        if(GetLoaderVersion(pcp) >= NEWLOADER_VERSION)
        {
          TxCommandIntern(I_VISU_RDY, 0, NULL, &pcp->comdef, (void**)&result, DEFSVRTYP);
        }
      }
    }
  }
}

_BOOL MakeList(_MEM_LIDENTRY *p)
{
  _BOOL retcode = _FALSE;
  _UDWORD no = 0;
  _LSEVARINFO *pvi;
  _UDWORD i, anz = VarList.GetNo();
  _UDWORD *pobj = NULL;
  _UBYTE valid;
  _UBYTE loader_version_valid[256];
  
  MemSet(loader_version_valid, 0, 256);
    
  if(Memory2((void**)&pobj, anz * sizeof(_UDWORD)) != 0)
  {
    for(i=0; i<anz; i++)
    {
      if(VarList.GetVarInfo(&pvi, i) == _TRUE)
      {
        if(pvi->vartype == OBJECT)
        {
          valid = loader_version_valid[pvi->station];
          if(valid == 0)
          {
            valid = 1;
            if(login_PLC(pvi->station) != NULL)
            {
              valid = 2;
              if(GetLoaderVersion(Batch.Get_COMPORT(pvi->station)) >= NEWLOADER_VERSION) // 02.02.92
                valid = 3; // loader version passt
            }
            loader_version_valid[pvi->station] = valid;
          }
          
          if(valid == 3) // nur wenn loderversion passt
          {
           #ifdef UC_GETLASALID_DATALINK        
            _COMPORT *pcp = Batch.Get_COMPORT(pvi->station);
            if(pcp != NULL)
            {
              if(pcp->lasalid_done == 0)
              {
                pobj[no] = i;
                no += 1;
              }
            }
           #else
            pobj[no] = i;
            no += 1;
           #endif
          }
        }
      }
    }
  
    if(init_MEM_LIDENTRY(p, no, anz) == _TRUE)
    {
      retcode = _TRUE;
      for(i=0; i<no; i++)
        AddObject(p, pobj[i]);
    }
    
    Memory2((void**)&pobj, 0);
  }
  
  return retcode;
}

_UDWORD ScanServerList(_MEM_LIDENTRY *p)
{
  _UDWORD i, anz = VarList.GetNo();
  _LSEVARINFO *pvi;
  _VARSYMBOL *pvs;
  _UDWORD error = 0;
  
  for(i=0; i<anz; i++)
  {
    if(VarList.GetVarInfo(&pvi, i) == _TRUE)
    {
     #ifdef UC_MEMBERVARIABLE
      if(pvi->memberoffset < 0) // nur wenns keine membervariable ist
     #endif
      {
        if((pvi->vartype != OBJECT)&&(pvi->vartype != FCONSTANT)&&(pvi->vartype != FMERKER)&&(pvi->vartype != BIT)&&(pvi->vartype != FENUMITEM))
        {
          if(VarList.GetVarSymbol(&pvs, i) == _TRUE)
          {
            if(AddServerToObject(p, pvs->scope, i) == _FALSE)
            {
              error += 1;
            }
          }
        }
      }
    }
  }
  
  return error;
}

_BOOL FinalScan(_MEM_LIDENTRY *p, _BOOL bar)
{
  _UDWORD     i, anz = VarList.GetNo();
  _LSEVARINFO *pvi, *tmp;
  _VARSYMBOL  *pvs;
  _UBYTE      varobj;
  _ASCII      label[MAX_LABELLEN];
  _UBYTE      state;
  _UWORD      sb;

  if(p->gesamt == anz)
  {
    sb = 0xFFFF;
    if(bar == _TRUE)
    {
      sb = ScanBar.Add("Finalize ID:", anz);
    }
  
    for(i=0; i<anz; i++)
    {
      ScanBar.Run(sb, i);
    
      if(p->ptrvalid[i] == 0)
      {
        if(VarList.GetVarInfo(&pvi, i) == _TRUE)
        {
         #ifdef UC_GETLASALID_DATALINK
          if(pvi->lasalid.id == 0)
         #endif
          {          
            state = 0;
            
           #ifdef UC_MEMBERVARIABLE
            if(pvi->memberoffset < 0) // nur wenn's keine membervariable ist
           #endif 
            {
              if(pvi->vartype == BIT)
              {
                if(VarList.GetVarSymbol(&pvs, i) == _TRUE)                   // scope ermitteln
                {
                  if(VarList.GetVarInfo(&tmp, pvs->scope) == _TRUE)  // varinfo von scope holen
                  {
                    if(tmp->vartype == BINARY)               // vartyp muss binary sein
                    {
                      if((tmp->format == SIZE_BDWORD)||(tmp->format == SIZE_BWORD)||(tmp->format == SIZE_BBYTE))
                      {
                        pvi->lasalid = tmp->lasalid;
                        state = p->ptrvalid[pvs->scope];
                        //p->ptrvalid[i] = state; // neuen state eintragen, bei bit nicht notwendig da bit keine untereinträge haben kann
                      }
                    }
                  }
                }
              }
              else if((pvi->vartype == FCONSTANT)||(pvi->vartype == FMERKER)||(pvi->vartype == FENUMITEM))
              {
                state = 1;
              }
              else if(pvi->POWPRUIC & VARBIT_STRUCTELEMENT)
              {
                if(VarList.GetVarSymbol(&pvs, i) == _TRUE)                   // scope ermitteln
                {
                  if(VarList.GetVarInfo(&tmp, pvs->scope) == _TRUE)  // varinfo von scope holen
                  {
                    pvi->lasalid = tmp->lasalid;
                    state = p->ptrvalid[pvs->scope];
                    p->ptrvalid[i] = state; // neuen state eintragen
                  }
                }
              }
            
              if(state == 0)
              {
                if(login_PLC(pvi->station) != NULL)
                {
                  if(VarList.GetLabel(label, &varobj, i) == _TRUE)
                  {
                    if(pvi->vartype == BIT)
                      delete_bit(label);
                   #ifdef UC_COMPLEX_SERVER
                    else if((pvi->POWPRUIC & VARBIT_STRUCTELEMENT) || (pvi->vartype == COMPLEX))
                      delete_element(label);
                   #endif
               
                    get_lasal_id(&pvi->lasalid, label, pvi);
                    p->ptrvalid[i] = 1; // wird nicht mehr besser
                  }
                }
              }
            }
          }          
        }
      }
    }
    
    ScanBar.End(sb);
    
    return _TRUE;
  }
  
  return _FALSE;
}

_BOOL GetIDFast(_UWORD sb, _BOOL bar)
{
  _BOOL retcode = _FALSE;

#ifdef UC_GLOBAL_VAR 
  return retcode;
#endif
#ifdef UC_GLOBAL_VAR_REFLIST
  return retcode;
#endif
#ifdef CANOPEN
  return retcode;
#endif
#ifdef OLDSTYLE_CAN
  return retcode;
#endif
#ifdef UC_MULTICPU
  return retcode;
#endif
 
  _MEM_LIDENTRY list;
  _UDWORD i, error;
  _MEM_LIDENTRY *p = &list;
  
  init_MEM_LIDENTRY(p, 0, 0);
  
  if(MakeList(p) == _TRUE)
  {
    if(p->no > 0)
    {
      error = ScanServerList(p);
      
      // alle id's holen
      ScanBar.Reset(sb, p->no, _TRUE);
      for(i=0; i<p->no; i++)
      {
        ScanBar.Run(sb, i);
        SendEntry(p, &p->ptr[i]);
      }
      ScanBar.Run(sb, p->no); // scanbalken voll anzeigen
    }
    
    retcode = FinalScan(p, bar);
  }
  
  free_MEM_LIDENTRY(p);
  
  return retcode;
}




