
#ifndef cExtern
 #ifdef __cplusplus
  #define cExtern extern "C"
 #else
  #define cExtern extern 
 #endif
#endif

static void*         DataServiceApi[128];
static unsigned long DataServiceApiNo = 0;

cExtern void **DsApiGetObjList(unsigned long *pno)
{
  if(pno != 0)
  {
    *pno = DataServiceApiNo;
  }
  return DataServiceApi;
}

cExtern long DsApiAdd(void* pthis)
{
  if(DataServiceApiNo < (sizeof(DataServiceApi)/sizeof(DataServiceApi[0])))
  {
    DataServiceApi[DataServiceApiNo] = pthis;
    DataServiceApiNo++;
    return 1;
  }
  
  return 0;
}
