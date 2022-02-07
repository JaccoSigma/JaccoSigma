#ifndef _CNetworkLibH
 #define _CNetworkLibH

  #include "DefineCompiler.h"

  #ifdef cCompile
  
    // convert uint32 from hostformat to networkformat (big-endian)
    cExtern unsigned long sigclib_htonl(unsigned long hostlong);
  
    // convert uint16 from hostformat to networkformat (big-endian)
    cExtern unsigned short sigclib_htons(unsigned short hostshort);
  
    // convert uint32 from networkformat (big-endian) to hostformat
    cExtern unsigned long sigclib_ntohl(unsigned long netlong);
  
    // convert uint16 from networkformat (big-endian) to hostformat
    cExtern unsigned short sigclib_ntohs(unsigned short netshort);
    
    // convert string with ip-adress into u32-ip-address
    cExtern char *sigclib_u32ipaddress(unsigned long *dst, const char *txtin);
    
  #else
  
    // convert uint32 from hostformat to networkformat (big-endian)
    function global __cdecl sigclib_htonl var_input hostlong : udint; end_var var_output retcode : udint; end_var;
  
    // convert uint16 from hostformat to networkformat (big-endian)
    function global __cdecl sigclib_htons var_input hostshort : uint; end_var var_output retcode : uint; end_var;
  
    // convert uint32 from networkformat (big-endian) to hostformat
    function global __cdecl sigclib_ntohl var_input netlong : udint; end_var var_output retcode : udint; end_var;
  
    // convert uint16 from networkformat (big-endian) to hostformat
    function global __cdecl sigclib_ntohs var_input netshort : uint; end_var var_output retcode : uint; end_var;

    // convert string with ip-adress into u32-ip-address
    function global __cdecl sigclib_u32ipaddress var_input dst:^udint; txtin:^char; end_var var_output retcode:^char; end_var;
  
  #endif
 
#endif

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_htonl(unsigned long hostlong);
// Function sigclib_htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
// Note: The host byte order is Least Significant Byte first, whereas the network byte order, as used on the Internet, is Most Significant Byte first.
// --> hostlong ...... hostlong
// <-- Function returns hostlong converted to network byte order

// ------------------------------------------------------------------------------------------------
// unsigned short sigclib_htons(unsigned short hostshort)
// Function sigclib_htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
// Note: The host byte order is Least Significant Byte first, whereas the network byte order, as used on the Internet, is Most Significant Byte first.
// --> hostshort ...... hostshort
// <-- Function returns hostshort converted to network byte order
  
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_ntohl(unsigned long netlong);
// Function sigclib_ntohl() function converts the unsigned integer netlong from network byte order to host byte order.
// Note: The host byte order is Least Significant Byte first, whereas the network byte order, as used on the Internet, is Most Significant Byte first.
// --> netlong ........ netlong
// <-- Function returns netlong converted to host byte order

// ------------------------------------------------------------------------------------------------
// unsigned short sigclib_ntohs(unsigned short netshort);
// Function sigclib_ntohs() function converts the unsigned short integer netshort from network byte order to host byte order.
// Note: The host byte order is Least Significant Byte first, whereas the network byte order, as used on the Internet, is Most Significant Byte first.
// --> netshort ....... netshort
// <-- Function returns netshort converted to host byte order

// ------------------------------------------------------------------------------------------------
// char *sigclib_u32ipaddress(unsigned long *dst, const char *txtin);
// Function convert given string with ip-adress into u32-ip-address
// --> dst ............ Address of Destination where u32-ip-address should be filed
// --> txtin .......... IP-Address-String
// Function will return address of first character after IP-Address from given string
