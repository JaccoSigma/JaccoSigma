//<NewSigmatekCFileOptimize/>
// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 08.07.2016                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#include "SigCLib.h"
#include "RTOS_C_Interfaces.h"

cExtern void _LookUpEmbed_C(const char *label, void **ppthis, void *pobj);

// +----------------------------------------------------------------------------------------------+
// sys functionality
// +----------------------------------------------------------------------------------------------+

void *sigclib_get_thisptr(const char *label)
{
  void *pthis = NULL;
  _LookUpEmbed_C(label, &pthis, 0);
  return pthis;
}

void *sigclib_cilget(const char *name)
{
  if(lsl_pOS != NULL)
  {
    if(lsl_pOS->piCIL != NULL)
    {
      void *retcode = NULL;
      if(lsl_pOS->piCIL->CILGet((char*)name, &retcode) == SYS_ERR_NONE)
      {
        return retcode;
      }
    }
  }
  return NULL;
}

void sigclib_reset(void)
{
  sigclib_serviceprovider("exec reset");
}  

void sigclib_restart(void)
{
  sigclib_serviceprovider("exec run");
}  

void *sigclib_virtual_methodpointer(void *pthis, long idx)
{
  if(pthis != NULL)
  {
    unsigned char *pmeth = (unsigned char*)*(void**)pthis;
    return *(void**)(pmeth + ((idx+10) * 4));
  }
  return NULL;
}

static unsigned long name_unique_inc = 0;
char *sigclib_name_unique(char *pd, const char *name)
{
  if(name == NULL)
  {
    name = "Dummy";
  }

  unsigned long actinc = sigclib_atomic_incU32(&name_unique_inc);
  sigclib_sprintf(pd, "%s%u", name, actinc);
  
  return pd;
}

char *sigclib_name_enlarge(char *pd, const char *name, void *ptr)
{
  if(name == NULL)
  {
    name = "Dummy";
  }
  
  sigclib_sprintf(pd, "%s%p", name, ptr);
  return pd;
}

// +----------------------------------------------------------------------------------------------+
// LSL_ST_SYSENV.h
// +----------------------------------------------------------------------------------------------+

cExtern long sigclib_setenvvar_intern(void *pifc, char *name, char *value); // done in SigCLib.st
cExtern long sigclib_getenvvar_intern(void *pifc, char *name, char *value, unsigned long sizeof_value); // done in SigCLib.st
static void *CilSysEnv = NULL;
long sigclib_setenvvar(const char *name, const char *value)
{
  if(CilSysEnv == NULL)
  {
    CilSysEnv = sigclib_cilget("SYSENV");
  }
  return sigclib_setenvvar_intern(CilSysEnv, (char*)name, (char*)value);
}

long sigclib_getenvvar(const char *name, char *value, unsigned long sizeof_value)
{
  if(CilSysEnv == NULL)
  {
    CilSysEnv = sigclib_cilget("SYSENV");
  }
  return sigclib_getenvvar_intern(CilSysEnv, (char*)name, value, sizeof_value);
}


// +----------------------------------------------------------------------------------------------+
// OS-Interfaces beschaffen
// +----------------------------------------------------------------------------------------------+

cExtern void MT_API_Set(unsigned long ifc, void *ptr); // done in SigCLib.st
static void *Cil_mt_api = NULL;       // "LSL_MULTITASK"
static void *Cil_lsl_tcp_user = NULL; // "TCP_USER"

cExtern void *MT_API_Usage(void)
{
  if(Cil_mt_api == NULL)
  {
    Cil_mt_api = sigclib_cilget("LSL_MULTITASK");
    MT_API_Set(0, Cil_mt_api); // inject Pointer to ST
  }
  
  return Cil_mt_api;
}

cExtern void *LSL_TCP_USER_Usage(void)
{
  if(Cil_lsl_tcp_user == NULL)
  {
    Cil_lsl_tcp_user = sigclib_cilget("TCP_USER");
    MT_API_Set(1, Cil_lsl_tcp_user); // inject Pointer to ST
  }
  
  return Cil_lsl_tcp_user;
}

//FUNCTION LSL_TCP_USER_Usage var_output retcode : ^LSL_TCP_USER; end_var
//  
//  if(lsl_tcp_user = NIL) then
//    lsl_tcp_user$^void := sigclib_cilget("TCP_USER");    
//  end_if;
//  
//  retcode := lsl_tcp_user;
//
//END_FUNCTION

//FUNCTION MT_API_Usage var_output retcode : ^LSL_MT_TYPE; end_var
//  
//  if(mt_api = NIL) then
//    mt_api$^void := sigclib_cilget("LSL_MULTITASK");
//  end_if;
//  
//  retcode := mt_api;
//
//END_FUNCTION

