// +----------------------------------------------------------------------------------------------+
// +-[   copyright ] Sigmatek GmbH & CoKG                                                         |
// +-[      author ] kolott                                                                       |
// +-[        date ] 26.07.2019                                                                   |
// +-[ description ]------------------------------------------------------------------------------+
// |                                                                                              |
// |                                                                                              |
// +----------------------------------------------------------------------------------------------+

#ifndef _cDeEncodeH
 #define _cDeEncodeH
  
 #include "DefineCompiler.h" 
  
 #ifdef cCompile // *******************************************************************************
 
    // convert unicode-0-string to utf8-0-string, function will return number of used bytes in destination, excluding final zero
    cExtern unsigned long sigclib_str16_to_utf8(void *dst_utf8, const void *src_uni);
    
    // convert utf8-0-string to unicode-0-string, function will return number of characters in destination
    cExtern unsigned long sigclib_utf8_to_str16(void *dst_uni, const void *src_utf8);

    // convert ascii-0-string to utf8-0-string, function will return number of used bytes in destination, excluding final zero
    cExtern unsigned long sigclib_ascii_to_utf8(void *dst_utf8, const void *src_ascii);
    
    // convert utf8-0-string to ascii-0-string, function will return number of characters in destination
    cExtern unsigned long sigclib_utf8_to_ascii(void *dst_ascii, const void *src_utf8);

    // convert UTF8 to UTF16
    cExtern unsigned long sigclib_utf8_to_utf16(unsigned short *pdst_utf16, unsigned long dst_bytesize, const char *psrc_utf8, unsigned long no);

    // convert UTF16 to UTF8
    cExtern unsigned long sigclib_utf16_to_utf8(unsigned char *pdst_utf8, unsigned long dst_bytesize, unsigned short *psrc_utf16, unsigned long no);

    // compute amount of characters used in utf-8 formatted string when ASCII-0-String is given
    cExtern unsigned long sigclib_ascii_to_utf8_len(const void *src_ascii);
    
    // compute amount of characters used in ascii-formatted string when utf8-0-String is given
    cExtern unsigned long sigclib_utf8_to_ascii_len(const void *src_utf8);
    
    // compute amount of characters used in str16-formatted string when utf8-0-String is given
    cExtern unsigned long sigclib_utf8_to_str16_len(const void *src_utf8);
    
    // compute amount of characters used in utf-8 formatted string when UNICODE-0-String is given
    cExtern unsigned long sigclib_str16_to_utf8_len(const void *src_uni);

    // compute size of base64 encoded data
    cExtern unsigned long sigclib_base64_encode_size(unsigned long srcsize);

    // convert data by using Base64 encoding 
    cExtern unsigned long sigclib_base64_encode(unsigned char *dst, unsigned long dstsize, void *src0, unsigned long srcsize);
    
    // decode base64 encoded data
    cExtern unsigned long sigclib_base64_decode(void *dst0, unsigned long dstsize, const unsigned char *src, unsigned long srcsize);
 
    // inplace encode of binary data
    cExtern void sigclib_encode_bin(void *p0, unsigned long bytesize, unsigned long key);
    
    // inplace decode of binary data
    cExtern void sigclib_decode_bin(void *p0, unsigned long bytesize, unsigned long key);

    // inplace encode of textual data
    cExtern void sigclib_encode_txt(void *p0, unsigned long bytesize, unsigned long key);

    // inplace decode of textual data
    cExtern void sigclib_decode_txt(void *p0, unsigned long bytesize, unsigned long key);
 
 
 #else // *****************************************************************************************
    // convert unicode-0-string to utf8-0-string, function will return number of used bytes in destination, excluding final zero
    function global __cdecl sigclib_str16_to_utf8 var_input dst:^void; src:^void; end_var var_output retcode:udint; end_var;
    
    // convert utf8-0-string to unicode-0-string, function will return number of characters in destination
    function global __cdecl sigclib_utf8_to_str16 var_input dst:^void; src:^void; end_var var_output retcode:udint; end_var;

    // convert ascii-0-string to utf8-0-string, function will return number of used bytes in destination, excluding final zero
    function global __cdecl sigclib_ascii_to_utf8 var_input dst:^void; src:^void; end_var var_output retcode:udint; end_var;
    
    // convert utf8-0-string to ascii-0-string, function will return number of characters in destination
    function global __cdecl sigclib_utf8_to_ascii var_input dst:^void; src:^void; end_var var_output retcode:udint; end_var;

    // convert UTF8 to UTF16
    function global __cdecl sigclib_utf8_to_utf16 var_input pdst0:^uint; dst_bytesize:udint; psrc0:^char; no:udint; end_var var_output retcode:udint; end_var;

    // convert UTF16 to UTF8
    function global __cdecl sigclib_utf16_to_utf8 var_input pdst0:^char; dst_bytesize:udint; psrc0:^uint; no:udint; end_var var_output retcode:udint; end_var;

    // compute amount of characters used in utf-8 formatted string when ASCII-0-String is given
    function global __cdecl sigclib_ascii_to_utf8_len var_input src_ascii:^void; end_var var_output retcode:udint; end_var;
    
    // compute amount of characters used in ascii-formatted string when utf8-0-String is given
    function global __cdecl sigclib_utf8_to_ascii_len var_input src_utf8:^void; end_var var_output retcode:udint; end_var;
    
    // compute amount of characters used in str16-formatted string when utf8-0-String is given
    function global __cdecl sigclib_utf8_to_str16_len var_input src_utf8:^void; end_var var_output retcode:udint; end_var;
    
    // compute amount of characters used in utf-8 formatted string when UNICODE-0-String is given
    function global __cdecl sigclib_str16_to_utf8_len var_input src_uni:^void; end_var var_output retcode:udint; end_var;

    // compute size of base64 encoded data
    function global __cdecl sigclib_base64_encode_size var_input srcsize:udint; end_var var_output retcode:udint; end_var;
 
    // convert data by using Base64 encoding 
    function global __cdecl sigclib_base64_encode var_input dst:^usint; dstsize:udint; src0:^void; srcsize:udint; end_var var_output retcode:udint; end_var;
    
    // decode base64 encoded data
    function global __cdecl sigclib_base64_decode var_input dst0:^void; dstsize:udint; src:^usint; srcsize:udint; end_var var_output retcode:udint; end_var;

    // inplace encode of binary data
    function global __cdecl sigclib_encode_bin var_input p0:^void; bytesize:udint; key:udint; end_var;
    
    // inplace decode of binary data
    function global __cdecl sigclib_decode_bin var_input p0:^void; bytesize:udint; key:udint; end_var;
 
    // inplace encode of textual data
    function global __cdecl sigclib_encode_txt var_input p0:^void; bytesize:udint; key:udint; end_var;

    // inplace decode of textual data
    function global __cdecl sigclib_decode_txt var_input p0:^void; bytesize:udint; key:udint; end_var;
 
 
 #endif // ****************************************************************************************
#endif



// ************************************************************************************************
// USAGE
// ************************************************************************************************

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_str16_to_utf8(void *dst_utf8, const void *src_uni);
// convert unicode-0-string to utf8-0-string
// --> dst_utf8 ........ destinationbuffer (utf8-0-string)
// --> src_uni ......... sourcedata (unicode-0-string)
// <-- function will return number of used bytes in destination, excluding final zero
    
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_utf8_to_str16(void *dst_uni, const void *src_utf8);
// convert utf8-0-string to unicode-0-string
// --> dst_uni ......... destinationbuffer
// --> src_utf8 ........ sourcedata (utf8-0-string)
// <-- function will return number of characters in destination

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_ascii_to_utf8(void *dst_utf8, const void *src_ascii);
// convert ascii-0-string to utf8-0-string
// --> dst_utf8 ........ destinationbuffer (utf8-0-string)
// --> src_ascii ....... sourcedata (ascii-0-string)
// <-- function will return number of used bytes in destination, excluding final zero
    
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_utf8_to_ascii(void *dst_ascii, const void *src_utf8);
// convert utf8-0-string to ascii-0-string
// --> dst_ascii ....... destinationbuffer
// --> src_utf8 ........ sourcedata (utf8-0-string)
// <-- function will return number of characters in destination
// NOTE: Each non representable glyph will be exchanged by '?'

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_utf8_to_utf16(unsigned short *pdst_utf16, unsigned long dst_bytesize, const char *psrc_utf8, unsigned long no)
// convert UTF8 to UTF16
// --> pdst_utf16 ...... destinationbuffer
// --> dst_bytesize .... bytesize of destinationbuffer
// --> psrc_utf8 ....... UTF8-coded sourcebuffer
// --> no .............. length UTF8-coded sourcebuffer
// function will return length of UTF16-coded destinationstring or 0 in case of error (destinationbuffer too small, format-error in sourcebuffer)

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_utf16_to_utf8(unsigned char *pdst_utf8, unsigned long dst_bytesize, unsigned short *psrc_utf16, unsigned long no)
// convert UTF16 to UTF8
// --> pdst_utf8 ....... destinationbuffer
// --> dst_bytesize .... bytesize of destinationbuffer
// --> psrc_utf16 ...... UTF16-coded sourcebuffer
// --> no .............. length of UTF16-coded sourcebuffer
// function will return length of UTF8-codec destinationstring or 0 in case of error (destinationbuffer too small, format-error in sourcebuffer)

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_ascii_to_utf8_len(const void *src_ascii);
// compute amount of characters used in utf-8 formatted string when ASCII-0-String is given
// --> src_ascii ....... source, 0-terminated-ascii-string to compute
// Function will return length of UTF8-coded destinationstring. Returnvalue is equal to result of sigclib_strlen(utf8)

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_utf8_to_ascii_len(const void *src_utf8);
// compute amount of characters used in ascii-formatted string when utf8-0-String is given
// --> src_utf8 ........ source, 0-terminated-UTF8-string to compute
// Function will return length of ASCII-coded string after conversion from UTF8.

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_str16_to_utf8_len(const void *src_uni);
// compute amount of characters used in utf-8 formatted string when ASCII or UNICODE-0-String is given
// --> src_uni ......... source, 0-terminated-unicode-string to compute
// --> chrsize ......... Size of Single Character in Source. 1= ASCII-0-String, 2=UNICODE-0-String
// Function will return length of UTF8-coded destinationstring. Returnvalue is equal to result of sigclib_strlen(utf8)

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_utf8_to_str16_len(const void *src_utf8);
// compute amount of characters used in str16-formatted string when utf8-0-String is given
// --> src_utf8 ........ source, 0-terminated-UTF8-string to compute
// Function will return length of U16-coded string after conversion from UTF8.

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_base64_encode_size(unsigned long srcsize);
// compute size of base64 encoded data
// --> srzsize ......... size in bytes of data to encode
// <-- function will return number of bytes used for encoded data

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_base64_encode(unsigned char *dst, unsigned long dstsize, void *src0, unsigned long srcsize);
// Encodes the given data with Base64.
// Encoding is used to make binary data survive transport through transport layers that are not 8-bit clean, eg mail bodies.
// --> dst ............. destimationbubffer where encoded data should be filed
// --> dstsize ......... bytesize of destinatonbuffer
// --> src0 ............ soutcedata to encode
// --> srcsize ......... size of soutcedata to encode in byte
// <-- function will return amount of encoded bytes, in case of error 0 (destination buffer too small)
// NOTE: encoded data needs about 133% of the original data size. Therefore, the destination nuffer must be made available accordingly.

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_base64_decode(void *dst0, unsigned long dstsize, const unsigned char *src, unsigned long srcsize);
// Decodes from Base64 given data.
// --> dst0 ............ destimationbubffer where encoded data should be filed
// --> dstsize ......... bytesize of destinaitonbuffer
// --> src ............. Base64 encoded soutcedata to decode
// --> srcsize ......... length of soutcedata to decode
// <-- function will return amount of decoded bytes, in case of error 0 (wrong src-data or destination buffer too small)

// ------------------------------------------------------------------------------------------------
// void sigclib_encode_bin(void *p0, unsigned long bytesize, unsigned long key);
// inplace encode of binary data
// --> p0 .............. data to encode
// --> bytesize ........ size of bytes to encode    
// --> key ............. arbitrary userkey (necessary to decode)
// NOTE: data can be decoded by using function sigclib_decode_bin() with same bytelength and key
   
// ------------------------------------------------------------------------------------------------
// void sigclib_decode_bin(void *p0, unsigned long bytesize, unsigned long key);
// inplace decode of binary data, encoded by using function sigclib_encode_bin()
// --> p0 .............. data to decode
// --> bytesize ........ size of bytes to decode    
// --> key ............. arbitrary userkey (same value used to encode)

// ------------------------------------------------------------------------------------------------
// void sigclib_encode_txt(void *p0, unsigned long bytesize, unsigned long key);
// inplace encode of text-data (just ASCII 32 - ASCII 126 characters will be affected by this function)
// --> p0 .............. data to encode
// --> bytesize ........ size of bytes to encode    
// --> key ............. arbitrary userkey (necessary to decode)
// NOTE: data can be decoded by using function sigclib_decode_txt() with same bytelength and key
   
// ------------------------------------------------------------------------------------------------
// void sigclib_decode_txt(void *p0, unsigned long bytesize, unsigned long key);
// inplace decode of text-data, encoded by using function sigclib_encode_txt()
// --> p0 .............. data to decode
// --> bytesize ........ size of bytes to decode    
// --> key ............. arbitrary userkey (same value used to encode)



