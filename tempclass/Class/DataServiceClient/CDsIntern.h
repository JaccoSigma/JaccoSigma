
#define CopyU32Makro(__pd, __ps) { unsigned char *__bd=(unsigned char*)__pd; unsigned char *__bs=(unsigned char*)__ps; __bd[0]=__bs[0]; __bd[1]=__bs[1]; __bd[2]=__bs[2]; __bd[3]=__bs[3]; }
#define CopyU16Makro(__pd, __ps) { unsigned char *__bd=(unsigned char*)__pd; unsigned char *__bs=(unsigned char*)__ps; __bd[0]=__bs[0]; __bd[1]=__bs[1]; }
