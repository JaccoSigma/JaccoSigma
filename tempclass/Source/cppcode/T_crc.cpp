#include "T_IPC.H"
#include "T_NUC.H"

#define IDCRCFILENAME    "IDCRC.SET"
#define IDCRCFILEID      "IDCRCSET"
#define IDCRCFILEVERSION 5

#define ALARMCRCFILENAME    "ALCRC.SET"
#define ALARMCRCFILEID      "ALCRCSET"
#define ALARMCRCFILEVERSION 2

const _UDWORD m_CrcTable[] =
{
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};

_CRC32 MakeCrc32(_CRC32 retcode, void *ptr, _UDWORD len)
{
	_UBYTE *buf = (_UBYTE *)ptr;

  retcode = retcode ^ 0xFFFFFFFFL;

  while(len)
  {
    retcode = m_CrcTable[(retcode^(*buf++))&0xff]^(retcode>>8);
    len--;
  }

  return(retcode^0xFFFFFFFFL);
}

_UDWORD oldrandom;
_UDWORD cntrandom;
_UDWORD Random(_UDWORD max)
{
  _UBYTE  *table = (_UBYTE*)m_CrcTable;
  _UDWORD random = 0;

  if(max > 0)
  {
    oldrandom = *(_UDWORD*)&table[oldrandom % (sizeof(m_CrcTable) - 4)] + cntrandom;
    random    = (get_abstime() ^ oldrandom) % max;
  }
  cntrandom += 1;

  return(random);
}

#ifdef OK_CHECK_CRCOFID


_BOOL WriteConnectionCrc(_File *File)
{
  _BOOL    retcode = _FALSE;
  _UDWORD  i, tmp, no;
  _COMPORT *pcp;
  _COMSET  *pcs;

  if(File->GetError() == _FALSE)
  {
    tmp = VarList.GetCrc32();
    File->Write(&tmp, sizeof(_UDWORD));                    // crc stations
    no = Batch.GetNo_COMPORT();
    File->Write(&no, sizeof(_UDWORD));                   // no of comports

    for(i=0; i<no; i++)
    {
      tmp = DEFAULTCRC;
      pcp = Batch.GetComportDirect(i);
      if(pcp != NULL)
        tmp = pcp->crc;
      File->Write(&tmp, sizeof(_UDWORD));           // crc of each station
    }

   #ifdef UC_MULTICPU
    tmp = UC_MULTICPU;
   #else
    tmp = 0;
   #endif
    File->Write(&tmp, sizeof(_UDWORD));                  // no of multicpu

    pcs = Batch.GetComSet();
    tmp = MakeCrc32(0, pcs->mapper, sizeof(pcs->mapper));
    File->Write(&tmp, sizeof(_UDWORD));          // crc32 of connectionmap

    retcode = (File->GetError() == _FALSE)? _TRUE : _FALSE;
  }

  return(retcode);
}

_BOOL ReadConnectionCrc(_File *File)
{
  _BOOL    retcode = _FALSE;
  _UDWORD  tmp1, tmp2;
  _UWORD   i, no;
  _COMPORT *pcp;
  _COMSET  *pcs;

  if(File->GetError() == _FALSE)
  {
    tmp1 = VarList.GetCrc32();
    tmp2 = tmp1 + 1;
    File->Read(&tmp2, sizeof(_UDWORD));                // crc32 stations
    if(tmp1 != tmp2) goto STOP_AND_GET_PLCID;

    File->Read(&tmp2, sizeof(_UDWORD));           // max. no of stations
    no = Batch.GetNo_COMPORT();
    if(tmp2 != no) goto STOP_AND_GET_PLCID;
    for(i=0; i<no; i++)
    {
      pcp = Batch.GetComportDirect(i);
      tmp1 = (pcp != NULL)? pcp->crc : DEFAULTCRC;
     #ifdef UC_EXOONLINE
      if(tmp1 == DEFAULTCRC) goto STOP_AND_GET_PLCID;
     #endif
      tmp2 = tmp1 + 1;
      File->Read(&tmp2, sizeof(_UDWORD));
      if(tmp1 != tmp2) goto STOP_AND_GET_PLCID;
    }

    File->Read(&tmp2, sizeof(_UDWORD));                // no of multicpu
   #ifdef UC_MULTICPU
    if(tmp2 != UC_MULTICPU) goto STOP_AND_GET_PLCID;
   #else
    if(tmp2 != 0) goto STOP_AND_GET_PLCID;
   #endif

    pcs  = Batch.GetComSet();
    tmp1 = MakeCrc32(0, pcs->mapper, sizeof(pcs->mapper));
    tmp2 = tmp1 + 1;
    File->Read(&tmp2, sizeof(_UDWORD));          // crc32 of connectionmap
    if(tmp1 != tmp2) goto STOP_AND_GET_PLCID;

    retcode = (File->GetError() == _FALSE)? _TRUE : _FALSE;
  }

STOP_AND_GET_PLCID:
  return(retcode);
}

_UDWORD _Alarm::GetCrc32(void)
{
  _CRC32      retcode = 0;
  _UDWORD     i;
  _VARLABEL   *vl;
  _LSEVARINFO *pvi;

  for(i=0; i<Data.no; i++)
  {
    vl = &Data.ptr[i].link;

    retcode = MakeCrc32(retcode, vl, sizeof(_VARLABEL));
    if(chk_VARLABEL(vl) == _TRUE)
    {
      if(vl->info[0].state == VAR_VAL)
      {
        if(VarList.GetVarInfo(&pvi, vl->info[0].value) == _TRUE)
        {
          retcode = MakeCrc32(retcode, &pvi->lasalid, sizeof(_LASALID));
        }
      }
    }
  }

  return(retcode);
}

_BOOL _Alarm::LookForAlarmCrc(void)
{
  _File    File;
  _BOOL    retcode = _FALSE;
  _ASCII   dpne[_MAX_PATHLENGTH];
  _UDWORD  i, tmp1, tmp2;
  _SINGLE_ALARM *psa;

  if(Project.MakeConfigDPNE(dpne, ALARMCRCFILENAME, _TRUE) == _TRUE)
  {
    if(File.Open(LOAD, dpne) == _TRUE)
    {
      if(File.Read_HEADER(&tmp1, ALARMCRCFILEID, _TRUE) == _TRUE)
      {
        if(tmp1 == ALARMCRCFILEVERSION)
        {
          tmp1 = GetCrc32();
          tmp2 = tmp1 + 1;
          File.Read(&tmp2, sizeof(_UDWORD));                 // crc of alarms
          if(tmp1 != tmp2) goto STOP_AND_GET_ALARM;

          tmp1 = Data.no;
          tmp2 = tmp1 + 1;
          File.Read(&tmp2, sizeof(_UDWORD));                  // no of alarms
          if(tmp1 != tmp2) goto STOP_AND_GET_ALARM;

          if(ReadConnectionCrc(&File) == _FALSE)    // crc of all connections
            goto STOP_AND_GET_ALARM;

          psa = Data.ptr;
          for(i=0; i<Data.no; i++)
          {
            if(File.Read(&psa->alano, sizeof(_UWORD)) == _FALSE)
              goto STOP_AND_GET_ALARM;
            if(File.Read(&psa->mode, sizeof(_UWORD)) == _FALSE)
              goto STOP_AND_GET_ALARM;
            if(File.Read_MEM_VARLABEL(&psa->parameter) == _FALSE)
              goto STOP_AND_GET_ALARM;
            psa += 1;
          }

          retcode = (File.GetError() == _TRUE)? _FALSE : _TRUE;
        }
      }
STOP_AND_GET_ALARM:
      File.Close();
    }
  }

  return(retcode);
}

_BOOL _Alarm::SaveAlarmCrc(void)
{
  _File    File;
  _ASCII   dpne[_MAX_PATHLENGTH];
  _BOOL    retcode = _FALSE;
  _UDWORD  tmp;
  _UWORD   i;
  _SINGLE_ALARM *psa;

  // [ ...... header (64 bytes)
  // [4] .... crc of alarms
  // [4] .... no of alarms
  // [ ...... alarminformation

  if(Project.MakeConfigDPNE(dpne, ALARMCRCFILENAME, _TRUE) == _TRUE)
  {
    if(File.Open(SAVE, dpne) == _TRUE)
    {
      if(File.Write_HEADER(ALARMCRCFILEID, ALARMCRCFILEVERSION) == _TRUE)
      {
        tmp = GetCrc32();
        File.Write(&tmp, sizeof(_UDWORD));                   // crc of alarms
        tmp = Data.no; //GetNo();
        File.Write(&tmp, sizeof(_UDWORD));                    // no of alarms

        WriteConnectionCrc(&File);                  // crc of all connections

        psa = Data.ptr;
        for(i=0; i<Data.no; i++)
        {
          File.Write(&psa->alano, sizeof(_UWORD));                 // alarmno
          File.Write(&psa->mode, sizeof(_UWORD));         // userdefined mode
          File.Write_MEM_VARLABEL(&psa->parameter);        // alarm parameter
          psa += 1;
        }
        retcode = (File.GetError() == _TRUE)? _FALSE : _TRUE;
      }

      File.Close();
    }
  }

  return(retcode);
}

_UDWORD _VarList::GetCrc32(void)
{
  _CRC32      retcode = 0;
  _UDWORD     no   = Data.MemLseVarInfo.no;
  _LSEVARINFO *ptr = Data.MemLseVarInfo.ptr;
  _UWORD      station = 255;

  while(no)
  {
    if(ptr->station != station)
    {
      retcode = retcode ^ station;
      retcode = retcode ^ no;
      retcode = (retcode & 0x80000000)? ((retcode << 1) | 1) : (retcode << 1);
      station = ptr->station;
    }

    ptr++;
    no--;
  }
  return(retcode);
}

_UDWORD _VarList::MakeVarCrc32(void)
{
 #ifdef UC_MEMBERVARIABLE
  _UDWORD retcode = MemberVariablesCount; 
 #else
  _UDWORD retcode = 0; 
 #endif
 
  retcode = MakeCrc32(retcode, Data.Label.GetPtr(), Data.Label.GetSize());
  
 #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
  retcode = retcode ^ 0xFFFFFFFF;
 #endif
  
  return retcode;
}

_BOOL _VarList::LookForIdCrc(void)
{
  _File       File;
  _BOOL       retcode = _FALSE;
  _ASCII      dpne[_MAX_PATHLENGTH];
  _CRC32      tmp1;
  _UDWORD     tmp2;
  _LSEVARINFO *ptr;

  if(Project.MakeConfigDPNE(dpne, IDCRCFILENAME, _TRUE) == _TRUE)
  {
    if(File.Open(LOAD, dpne) == _TRUE)
    {
      if(File.Read_HEADER(&tmp1, IDCRCFILEID, _TRUE) == _TRUE)
      {
        if(tmp1 == IDCRCFILEVERSION)
        {
          tmp1 = MakeVarCrc32();
          tmp2 = tmp1 + 1;
          File.Read(&tmp2, sizeof(_UDWORD));  // crc32 server and objectnames
          if(tmp1 != tmp2) goto STOP_AND_GET_ID;

          if(ReadConnectionCrc(&File) == _FALSE)    // crc of all connections
            goto STOP_AND_GET_ID;

          tmp1 = Data.MemLseVarInfo.no;
          tmp2 = tmp1 + 1;
          File.Read(&tmp2, sizeof(_UDWORD));                // no of lasalids
          if(tmp1 != tmp2) goto STOP_AND_GET_ID;
          ptr  = Data.MemLseVarInfo.ptr;
          while(tmp1)
          {
           #ifdef UC_MULTICPU
            File.Read(ptr->multiid, UC_MULTICPU*sizeof(_UDWORD));
            if(File.Read(&ptr->lasalid.vtype, sizeof(_UBYTE)) == _FALSE)
              goto STOP_AND_GET_ID;
            ptr->lasalid.id = ptr->multiid[0];
           #else
            _UBYTE data[12];
            if(File.Read(data, 5) == _FALSE)
              goto STOP_AND_GET_ID;
            ptr->lasalid.id    = *(_UDWORD*)&data[0];
            ptr->lasalid.vtype = *(_UBYTE*)&data[4];
            
            if(ptr->lasalid.vtype == TYP_EXTENDED)
              if(ptr->vartype != FSTRING)
                goto STOP_AND_GET_ID;
                
            #ifndef UC_EXOONLINE
            if(ptr->vartype == FSTRING)
              if(ptr->lasalid.vtype != TYP_EXTENDED)
                goto STOP_AND_GET_ID;            
            #endif 
           #endif

            ptr++;
            tmp1--;
          }

         #if IDCRCFILEVERSION >= 2
          tmp2 = 0;
          File.Read(&tmp2, 1);

         #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
          if(tmp2 != 1)
            goto STOP_AND_GET_ID;
         #else
          if(tmp2 != 0)
            goto STOP_AND_GET_ID;
         #endif
         #endif

          retcode = (File.GetError() == _TRUE)? _FALSE : _TRUE;
        }
      }

STOP_AND_GET_ID:
      File.Close();

    }
  }

  return(retcode);
}

_BOOL _VarList::SaveIdCrc(void)
{
  _File       File;
  _ASCII      dpne[_MAX_PATHLENGTH];
  _BOOL       retcode = _FALSE;
  _CRC32      tmp;
  _LSEVARINFO *ptr;
  _UBYTE      tmp1;

  // [ ......... header (64 bytes)
  // [4] ....... no of multicpu (default = 0) {version 5}
  // [4] ....... crc32 server and objectnames
  // [4] ....... crc32 stations
  // [4] ....... no of max stations (256)
  // +[4] ...... crc of each station
  // [4] ....... no of lasalids
  // +[1] ...... typeid
  // +[4] ...... lasalid
  // [1] ....... use globalvar

  if(Project.MakeConfigDPNE(dpne, IDCRCFILENAME, _TRUE) == _TRUE)
  {
    if(File.Open(SAVE, dpne) == _TRUE)
    {
      if(File.Write_HEADER(IDCRCFILEID, IDCRCFILEVERSION) == _TRUE)
      {
        tmp = MakeVarCrc32();
       
        File.Write(&tmp, sizeof(_UDWORD));    // crc32 server and objectnames

        WriteConnectionCrc(&File);                // crc32 of all connections

        tmp = Data.MemLseVarInfo.no;
        File.Write(&tmp, sizeof(_UDWORD));                  // no of lasalids
        ptr = Data.MemLseVarInfo.ptr;
        while(tmp)
        {
         #ifdef UC_MULTICPU
          File.Write(ptr->multiid, UC_MULTICPU * sizeof(_UDWORD));
          File.Write(&ptr->lasalid.vtype, sizeof(_UBYTE));
         #else
          _UBYTE data[5];
          *(_UDWORD*)&data[0] = ptr->lasalid.id;
          *(_UBYTE*)&data[4]  = ptr->lasalid.vtype;
          File.Write(data, 5);
         #endif

          ptr++;
          tmp--;
        }

       #if defined(UC_GLOBAL_VAR) || defined(UC_GLOBAL_VAR_REFLIST)
        tmp1 = 1;
       #else
        tmp1 = 0;
       #endif

        File.Write(&tmp1, 1);

        retcode = (File.GetError() == _TRUE)? _FALSE : _TRUE;
      }

      File.Close();
    }
  }

  return(retcode);
}
#endif

void turn_sub(_UBYTE *src, _UDWORD size)
{
  _UBYTE  *begin, *end;
  _UBYTE  tmp;
  _UDWORD i;

  if(size > 1)
  {
    begin = src;
    end   = &src[size-1];
    size  = size / 2;

    for(i=0; i<size; i++, begin++, end--)
    {
      tmp    = *begin;
      *begin = *end;
      *end   = tmp;
    }
  }
}

void encode_sub(_UBYTE *src, _UDWORD size)
{
  _UBYTE  value, tmp;
  _UDWORD i;
  _UBYTE  *ptr = src;

  if(size > 1)
  {
    for(i=0, value=0xFF; i<size; i++, ptr++)
    {
      tmp    = *ptr;
      tmp   ^= value;
      value  = tmp;
      tmp   ^= (((_UBYTE)i)^((_UBYTE)size));
      *ptr   = tmp;
    }

    turn_sub(src, size);
  }
}

void decode_sub(_UBYTE *src, _UDWORD size)
{
  _UBYTE  nval, value, tmp;
  _UBYTE  *ptr = src;
  _UDWORD i;

  if(size > 1)
  {
    turn_sub(src, size);

    for(i=0, value=0xFF; i<size; i++, ptr++)
    {
      tmp   = *ptr;
      tmp  ^= (((_UBYTE)i)^((_UBYTE)size));
      nval  = tmp;
      tmp  ^= value;
      *ptr  = tmp;

      value = nval;
    }
  }
}

void DeEnCode(_UBYTE *ptr, _UDWORD size, _BOOL encode)
{
  _UDWORD siz2 = size/2;

  if(encode == _TRUE)
  {
    encode_sub(ptr, size);
    encode_sub(ptr, siz2);
    encode_sub(ptr+size-siz2, siz2);
    encode_sub(ptr+siz2/2, siz2);
  }
  else
  {
    decode_sub(ptr+siz2/2, siz2);
    decode_sub(ptr+size-siz2, siz2);
    decode_sub(ptr, siz2);
    decode_sub(ptr, size);
  }
}



//********************************************************************
//********************************************************************
//********************************************************************
//********************************************************************
//********************************************************************

/*
#define BITS          14                // Setting the number of bits to 12, 13
#define HASHING_SHIFT (BITS-8)               // or 14 affects several constants
#define MAX_VALUE     ((1 << BITS) - 1)    // Note that MS-DOS machines need to
#define MAX_CODE      (MAX_VALUE - 1)   // compile their code in large model if
                                                        // 14 bits are selected
#if BITS == 14
  #define TABLE_SIZE 18041               // The string table size needs to be a
#endif                                  // prime number that is somewhat larger
#if BITS == 13                                                  // than 2**BITS
  #define TABLE_SIZE 9029
#endif
#if BITS <= 12
  #define TABLE_SIZE 5021
#endif

int *code_value;                                // This is the code value array
unsigned int *prefix_code;                 // This array holds the prefix codes
unsigned char *append_character;         // This array holds the appended chars
unsigned char decode_stack[4000];        // This array holds the decoded string

unsigned char *decode_string(unsigned char *buffer, unsigned int code)
{
  int i;

  i = 0;
  while(code > 255)
  {
    *buffer++ = append_character[code];
    code = prefix_code[code];
    if(i++>=4094)
    {
      // Fatal error during code expansion
      return(NULL);
    }
  }
  *buffer = code;
  return(buffer);
}

int           input_bit_count;
unsigned long input_bit_buffer;

unsigned int input_code(_File *Input)
{
  unsigned int return_value;
  unsigned long tmp;

  while(input_bit_count <= 24)
  {
    tmp = 0;
    Input->Read(&tmp, 1);

    input_bit_buffer |= (tmp << (24 - input_bit_count));
    input_bit_count += 8;
  }
  return_value = (input_bit_buffer >> (32-BITS));
  input_bit_buffer <<= BITS;
  input_bit_count -= BITS;

  return(return_value);
}

void decompress(_File *Input, _File *Output)
{
  unsigned int next_code;
  unsigned int new_code;
  unsigned int old_code;
  int character;
  int counter;
  unsigned char *string;

  next_code        = 256;           // This is the next available code to define
  counter          = 0;               // Counter is used as a pacifier.
  input_bit_count  = 0;
  input_bit_buffer = 0L;

  old_code  = input_code(Input);  // Read in the first code, initialize the
  character = old_code;          // character variable, and send the first

  Output->Write(&old_code, 1);

  while((new_code = input_code(Input)) != (MAX_VALUE))
  {
    if((++counter) == 1000)   // This section of code prints out
      counter = 0;           // It is just a pacifier.

    if(new_code >= next_code)
    {
      *decode_stack = character;
      string = decode_string(decode_stack+1, old_code);
    }
    else
      string = decode_string(decode_stack, new_code);

    character = *string;
    while(string >= decode_stack)
    {
      Output->Write(string, 1);
      string -= 1;
    }

    if(next_code <= MAX_CODE)
    {
      prefix_code[next_code] = old_code;
      append_character[next_code] = character;
      next_code ++;
    }
    old_code = new_code;
  }
}

// ****************************************************************************
// compress *******************************************************************
// ****************************************************************************

int find_match(int hash_prefix, unsigned int hash_character)
{
  int index;
  int offset;

  index = (hash_character << HASHING_SHIFT) ^ hash_prefix;
  if(index == 0)
    offset = 1;
  else
    offset = TABLE_SIZE - index;

  while(1)
  {
    if(code_value[index] == -1)
      return(index);
    if((prefix_code[index] == hash_prefix) && (append_character[index] == hash_character))
      return(index);
    index -= offset;
    if(index < 0)
      index += TABLE_SIZE;
  }
}

void output_code(_File *Output, unsigned int code)
{
  static int output_bit_count = 0;
  static unsigned long output_bit_buffer = 0L;
  _UBYTE tmp;

  output_bit_buffer |= (unsigned long) code << (32-BITS-output_bit_count);
  output_bit_count += BITS;

  while(output_bit_count >= 8)
  {
    tmp = output_bit_buffer >> 24;
    Output->Write(&tmp, 1);
    output_bit_buffer <<= 8;
    output_bit_count -= 8;
  }
}

void compress(_File *Input, _File *Output)
{
  _UDWORD n, length;
  unsigned int character, string_code;
  unsigned int next_code;
  unsigned int index;
  int i;

  next_code = 256;
  for(i=0;i<TABLE_SIZE;i++)
    code_value[i] = -1;

  i = 0;
  length = Input->Length();
  string_code = 0;
  if(Input->Read(&string_code, 1) == _TRUE)
  {
    length -= 1;
    for(n=0; n<length; n++)
    {
      character = 0;
      if(Input->Read(&character, 1) == _TRUE)
      {
        if((++i) == 1000)
          i = 0;

        index = find_match(string_code, character);

        if(code_value[index] != -1)
        {
          string_code = code_value[index];
        }
        else
        {
          if(next_code <= MAX_CODE)
          {
            code_value[index] = next_code++;
            prefix_code[index] = string_code;
            append_character[index] = character;
          }
          output_code(Output, string_code);
          string_code = character;
        }
      }
    }
  }

  output_code(Output, string_code);
  output_code(Output, MAX_VALUE);
  output_code(Output, 0);
}

void test(void)
{
  _File OutputFile;
  _File LzwFile;
  _File InputFile;

  append_character = NULL,
  prefix_code      = NULL;
  code_value       = NULL;

  Memory2((void**)&code_value, TABLE_SIZE*sizeof(unsigned int));
  Memory2((void**)&prefix_code, TABLE_SIZE*sizeof(unsigned int));
  Memory2((void**)&append_character, TABLE_SIZE*sizeof(unsigned char));

  if((code_value != NULL) && (prefix_code != NULL) && (append_character != NULL))
  {
    if(InputFile.Open(LOAD, "RAM@A:\\TEST0.TXT") == _TRUE)
    {
      if(LzwFile.Open(SAVE, "RAM@A:\\TEST.LZW") == _TRUE)
      {
        compress(&InputFile, &LzwFile);

        LzwFile.Close();
      }
      InputFile.Close();
    }
  }

  Memory2((void**)&code_value, 0);


  // open the files for expansion

  Memory2((void**)&prefix_code, TABLE_SIZE*sizeof(unsigned int));
  Memory2((void**)&append_character, TABLE_SIZE*sizeof(unsigned char));

  if((prefix_code != NULL) && (append_character != NULL))
  {
    if(LzwFile.Open(LOAD, "RAM@A:\\TEST.LZW") == _TRUE)
    {
      if(OutputFile.Open(SAVE, "RAM@A:\\TEST1.TXT") == _TRUE)
      {
        decompress(&LzwFile, &OutputFile);

        OutputFile.Close();
      }
      LzwFile.Close();
    }
  }


  Memory2((void**)&prefix_code, 0);
  Memory2((void**)&append_character, 0);


  _UDWORD length, i;
  _UBYTE t1, t2; 

  if(InputFile.Open(LOAD, "RAM@A:\\TEST0.TXT") == _TRUE)
  {
    if(OutputFile.Open(LOAD, "RAM@A:\\TEST1.TXT") == _TRUE)
    {
      length = OutputFile.Length();
      if(length == InputFile.Length())
      {
        for(i=0; i<length; i++)
        {
          if((OutputFile.Read(&t1, 1) == _TRUE) && (InputFile.Read(&t2, 1) == _TRUE))
          {
            if(t1 != t2)
              Debug("DoHodsWos", 1, 1);
          }
          else
          {
            Debug("DoHodsWos", 1, 1);
          }
        }
      }
      else
      {
        Debug("DoHodsWos", 1, 1);
      }

      Debug("Ready", 1, 2);
      OutputFile.Close();
    }
    InputFile.Close();
  }
}

*/


