#include "T_IPC.H"
#include "T_NUC.H"

// t_explor.cpp

void init_Thread2CmdLine(_Thread2CmdLine *pline)
{
  pline->cmd       = T2_NONE;
  pline->user1     = 0;
  pline->user2     = 0;
  MemSet(pline->op1, 0, _MAX_PATHLENGTH);
  MemSet(pline->op2, 0, _MAX_PATHLENGTH);
  pline->pcallback = NULL;
  pline->pthis     = NULL;
}

void RTK_Thread2CallBack(_Thread2CmdLine *pline, _BOOL result)
{ 
  if(pline->pcallback != NULL)
  {
    void *pthis         = pline->pthis;
    _FVOID_PT_BOOL pfkt = pline->pcallback;
  
    load_this(pthis);
    pfkt(pline, result);
  }
}

void CallUserAsyncMethode(_Thread2CmdLine *pline)
{
  if(pline->pcallback != NULL)
  {
    void             *pthis     = pline->pthis;
    _FVOID_PT_UDWORD pfkt       = (_FVOID_PT_UDWORD)pline->pcallback;
    void             *puserdata = pline->op1;
    _UDWORD          userdatlen = pline->user1;
    load_this(pthis);
    pfkt(puserdata, userdatlen);
  }
}

void RTK_Thread2(_Thread2CmdLine *pline)
{
  _ASCII filefilter[30];
  _UDWORD pos;
  
  switch(pline->cmd)
  {
    case T2_COPY_FILE :
      RTK_Thread2CallBack(pline, Lse_File_Copy(pline->op1, pline->op2));
      break;
        
    case T2_COPY_DIRECTORY : 
      filefilter[0] = 0;
      pos = StrChr(pline->op2, '*', sizeof(_ASCII));
      if(pos < StrLen(pline->op2, sizeof(_ASCII)))
      {
        StrCpy(filefilter, sizeof(_ASCII), &pline->op2[pos], sizeof(_ASCII));
        pline->op2[pos] = 0;
        AddTokenEnd(pline->op2);
      }
      RTK_Thread2CallBack(pline, directory_copy(pline->op1, pline->op2, filefilter));
      break;
        
    case T2_DELETE_FILE : 
      RTK_Thread2CallBack(pline, filexplo_delete_file(pline->op1));
      break;
       
    case T2_DELETE_DIRECTORY : 
      RTK_Thread2CallBack(pline, filexplo_delete_directory(pline->op1));
      break;
      
    case T2_USERCALLMETHODE :
      CallUserAsyncMethode(pline);
      break;  
  }
}


_BOOL Thread2_AddAsyncMethod(_FVOID_PT_UDWORD usermethod, void *pthis, void *puserdata, _UDWORD userdatalength)
{
  _Thread2CmdLine cmd;
  
  if(usermethod != NULL)
  {
    init_Thread2CmdLine(&cmd);
    cmd.cmd       = T2_USERCALLMETHODE;
    cmd.pthis     = pthis;
    cmd.pcallback = (_FVOID_PT_BOOL)usermethod;
    if(puserdata != NULL)
    {
      cmd.user1 = userdatalength;
      MemCpy(cmd.op1, puserdata, userdatalength);
    }
    
    return Thread2_AddCmd(&cmd);
  }
  
  return _FALSE;
}

