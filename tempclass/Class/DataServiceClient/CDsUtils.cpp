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

void DsComData_Init(DsComData *pcd, unsigned short id_major, unsigned short id_minor)
{
  // Grundinitialisierung von DsComData
  // --> pcd ............... Puffer
  // --> id_minor .......... minorid

  pcd->datalength      = sizeof(DsComDataHdr);
  pcd->id_major        = id_major; //ComId_Major_DS;
  pcd->id_minor        = id_minor;
  pcd->transactionSrc  = 0;
  pcd->transactionDest = 0;
  pcd->messageId       = DSC_MESSAGEID_DEFAULT;
}

DsComData *DsComData_CreateEx(void *statbuff, unsigned long statbuff_size, unsigned long userdata_size, unsigned short id_major, unsigned short id_minor)
{
  // diese Funktion erzeugt einen Kommunikationspuffer mit userspezifischer Datenlänge
  // --> statbuff .......... Adresse statischer Buffer
  // --> statbuff_size ..... Bytesize vom statischen Buffer
  // --> userdata_size ..... Anzahl der Bytes welche vom User verschickt werden sollen
  // --> id_major .......... identifikations Id (use ComId_Major_DS to communicate with loader)
  // --> id_minor .......... identifikations Id
  // <-- Funktion retouniert Adresse vom bereitgestellten Userbuffer oder im Fehlerfall NULL
  
  userdata_size += sizeof(DsComDataHdr);
  DsComData *pcd = (DsComData*)sigclib_buffer_create(statbuff, statbuff_size, userdata_size);
  
  if(pcd != NULL)
  {
    DsComData_Init(pcd, id_major, id_minor);
  }
  
  return pcd;
}

DsComData *DsComData_Create(void *statbuff, unsigned long statbuff_size, unsigned long userdata_size, unsigned short id_minor)
{
  // diese Funktion erzeugt einen Kommunikationspuffer mit userspezifischer Datenlänge
  // --> statbuff .......... Adresse statischer Buffer
  // --> statbuff_size ..... Bytesize vom statischen Buffer
  // --> userdata_size ..... Anzahl der Bytes welche vom User verschickt werden sollen
  // --> id_minor .......... identifikations Id
  // <-- Funktion retouniert Adresse vom bereitgestellten Userbuffer oder im Fehlerfall NULL
  
  return DsComData_CreateEx(statbuff, statbuff_size, userdata_size, ComId_Major_DS, id_minor);
}

DsComData *DsComData_Destroy(void *statbuff, DsComData *pcd)
{
  // diese Funktion zerstört einen zuvor angelegten Kommunikationspuffer
  // --> statbuff .......... Adresse statischer Buffer
  // --> pcd ............... Adresse Userbuffer (Returnwert von DataLink_BufferCreate())
  // <-- Funktion retourniert immer NULL
  return (DsComData*)sigclib_buffer_destroy(statbuff, pcd);
}

unsigned long DsComData_UserDataSize(unsigned long statbuff_size)
{
  // Funktion ermittelt die Anzahl der UserDataBytes welche im Puffer platz haben
  // --> statbuff_size ..... Anzahl der UserBytes welche im Puffer platz haben
  // <-- Funktion retourniert die Anzahl der UserDataBytes im Puffer
  
  return (statbuff_size > sizeof(DsComDataHdr))? (statbuff_size - sizeof(DsComDataHdr)) : 0;
}

void DsComData_Empty(DsComData *pcd)
{
  // Userdaten entfernen
  pcd->datalength = sizeof(DsComDataHdr);
}

#define DsCdRest(__pcd, __userdata_size) (__userdata_size + sizeof(DsComDataHdr) - __pcd->datalength)

unsigned long DsComData_Rest(DsComData *pcd, unsigned long userdata_size)
{
  // diese Funktion retourniert die verbleibende Anzahl an Bytes im Databuffer
  // --> pcd ............... DestinationBuffer
  // --> userdata_size ..... Gesamtbytegrösse von DsComData-DsComDataHeadersize
  return DsCdRest(pcd, userdata_size);
//  return userdata_size + sizeof(DsComDataHdr) - pcd->datalength;
}

unsigned long DsComData_SetU32(DsComData *pcd, unsigned long userdata_size, unsigned long offset, unsigned long value)
{
  // diese Funktion setzt auf offset einen u32bit Wert
  if((offset+4) <= userdata_size)
  {
//    *(unsigned long*)&pcd->data[offset] = value;
    CopyU32Makro(&pcd->data[offset], &value);
    return 1;
  }
  return 0;
}

unsigned long DsComData_IncU32(DsComData *pcd, unsigned long userdata_size, unsigned long offset)
{
  // diese Funktion incrementiert auf offset einen u32bit Wert
  if((offset+4) <= userdata_size)
  {
    //unsigned long value = *(unsigned long*)&pcd->data[offset];
    //*(unsigned long*)&pcd->data[offset] = (value + 1);
    unsigned long value;
    CopyU32Makro(&value, &pcd->data[offset]);
    value += 1;
    CopyU32Makro(&pcd->data[offset], &value);
    return 1;
  }
  return 0;
}

unsigned long DsComData_AddData(DsComData *pcd, unsigned long userdata_size, void *pdata, unsigned long datasize)
{
  // diese Funktion addiert beliebige Daten zu einem Databuffer
  // --> pcd ............... DestinationBuffer
  // --> userdata_size ..... Gesamtbytegrösse von DsComData-DsComDataHeadersize
  // --> pdata ............. zu addierende Daten
  // --> datasize .......... von zu addierenden Daten
  // <-- Funktion retourniert 1 bei Erfolg, 0 wenn daten nicht mehr addiert werden können
  
  if(datasize <= DsCdRest(pcd, userdata_size))
  {
    unsigned long offset = pcd->datalength - sizeof(DsComDataHdr);
    sigclib_memcpy(&pcd->data[offset], pdata, datasize);
    pcd->datalength += datasize;
    return 1;
  }
  
  return 0;
}

unsigned long DsComData_AddU32(DsComData *pcd, unsigned long userdata_size, unsigned long value)
{
  // diese Funktion addiert einenU32 Wert zu einem Databuffer
  // --> pcd ............... DestinationBuffer
  // --> userdata_size ..... Gesamtbytegrösse von DsComData-DsComDataHeadersize
  // --> value.............. zu addierender U32 Wert
  // <-- Funktion retourniert 1 bei Erfolg, 0 wenn daten nicht mehr addiert werden können

  if(DsCdRest(pcd, userdata_size) >= 4)
  {
    unsigned long offset = pcd->datalength - sizeof(DsComDataHdr);
    CopyU32Makro(&pcd->data[offset], &value);
    pcd->datalength += 4;
    return 1;
  }
  return 0;
}

unsigned long DsComData_AddU16(DsComData *pcd, unsigned long userdata_size, unsigned short value)
{
  // diese Funktion addiert einenU16 Wert zu einem Databuffer
  // --> pcd ............... DestinationBuffer
  // --> userdata_size ..... Gesamtbytegrösse von DsComData-DsComDataHeadersize
  // --> value.............. zu addierender U16 Wert
  // <-- Funktion retourniert 1 bei Erfolg, 0 wenn daten nicht mehr addiert werden können

  if(DsCdRest(pcd, userdata_size) >= 2)
  {
    unsigned long offset = pcd->datalength - sizeof(DsComDataHdr);
    CopyU16Makro(&pcd->data[offset], &value);
    pcd->datalength += 2;
    return 1;
  }
  return 0;
}

unsigned long DsComData_AddU08(DsComData *pcd, unsigned long userdata_size, unsigned char value)
{
  // diese Funktion addiert einenU08 Wert zu einem Databuffer
  // --> pcd ............... DestinationBuffer
  // --> userdata_size ..... Gesamtbytegrösse von DsComData-DsComDataHeadersize
  // --> value.............. zu addierender U08 Wert
  // <-- Funktion retourniert 1 bei Erfolg, 0 wenn daten nicht mehr addiert werden können

  if(DsCdRest(pcd, userdata_size) >= 1)
  {
    unsigned long offset = pcd->datalength - sizeof(DsComDataHdr);
    pcd->data[offset] = value;
    pcd->datalength += 1;
    return 1;
  }
  return 0;
}

unsigned long DsComData_AddStr(DsComData *pcd, unsigned long userdata_size, const char *pstr)
{
  // diese Funktion addiert beliebige Daten zu einem Databuffer
  // --> pcd ............... DestinationBuffer
  // --> userdata_size ..... Gesamtbytegrösse von DsComData-DsComDataHeadersize
  // --> pstr .............. zu addierender Ascii-0-String
  // <-- Funktion retourniert 1 bei Erfolg, 0 wenn daten nicht mehr addiert werden können
  
  unsigned long len = sigclib_strlen(pstr) + 1;
  return DsComData_AddData(pcd, userdata_size, (void*)pstr, len);
}

unsigned long DsComData_GetU32(DsComData *pcd, unsigned long offset)
{
  // diese Funktion liest von offset einen u32bit Wert
  // --> pcd ............... DestinationBuffer
  // --> offset ............ offset von welchem gelesen werden soll
  if((offset+4) <= pcd->datalength)
  {
    unsigned long retcode;
    CopyU32Makro(&retcode, &pcd->data[offset]); 
    return retcode;
    // return *(unsigned long*)&pcd->data[offset]; // compiler warning "strict-aliasing"
  }
  return 0;
}


