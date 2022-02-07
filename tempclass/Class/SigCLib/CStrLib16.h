#ifndef _CStrLib16H
 #define _CStrLib16H

  #include "DefineCompiler.h"

  #ifdef cCompile
  
    // convert ascii-0-string into uni-0-string. function will return parameter 'dst0'
    cExtern unsigned short* sigclib_tostr16(unsigned short *dst0, const char *src0);
    
    // convert uni-0-string into ascii-0-string. each character >255 will be converted to [SPACE]. function will return parameter 'dst0'
    cExtern char*           sigclib_tostr08(char *dst0, const unsigned short *src0);
    
    // get number of characters in uni-0-string.
    cExtern unsigned long   sigclib_strlen16(const unsigned short* str0);
    
    // copy uni-0-string to destination. function will return parameter 'dst0'
    cExtern unsigned short* sigclib_strcpy16(unsigned short* dst0, const unsigned short* src0);
    
    // copies up to 'count' characters from src0 to dst0. in a case where the length of src0 is less than 'count', the remainder of dst will be padded with 0.
    cExtern unsigned short* sigclib_strncpy16(unsigned short* dst0, const unsigned short* src0, unsigned long count);
    
    // compare uni-0-strings. function will return 0 when strings are equal, otherwiese <> 0
    cExtern long            sigclib_strcmp16(const unsigned short *src1, const unsigned short *src2);
    
    // add 'ps2' at the end of 'ps1'. function will return parameter 'src1'
    cExtern unsigned short* sigclib_strcat16(unsigned short *src1, const unsigned short *src2);
    
    // seek address of given character 'i' in uni-0-string. function will return valid pointer or NULL 
    cExtern unsigned short* sigclib_strchr16(const unsigned short *src1, unsigned long i);
    
    // convert all characters to uppercase in given uni-0-string. function will return parameter 'src'.
    cExtern unsigned short* sigclib_strupr16(unsigned short *src);
    
    // convert all characters to lowercase in given uni-0-string. function will return parameter 'src'.
    cExtern unsigned short* sigclib_strlwr16(unsigned short *src);
    
    // seek address of uni-0-string 'lookfor' in uni-0-string 'searchee'. function will return valid pointer or NULL if 'lookfor' is not found in 'searchee'.
    cExtern unsigned short* sigclib_strstr16(const unsigned short* searchee, const unsigned short* lookfor);
    
    // This function finds the last occurence of chr in the string pointed to by s including the 0-termination
    cExtern unsigned short* sigclib_strrchr16(const unsigned short *ps1, unsigned short chr);
    
    // copies all characters from src into dst till first occurance of 'tillchr'
    cExtern unsigned short* sigclib_strcpytill16(unsigned short *dst, const unsigned short *src, unsigned short tillchr);
    
    cExtern unsigned short* sigclib_ulltou16(unsigned long long value, unsigned short *result, unsigned long base);
    cExtern unsigned short* sigclib_lltou16(long long value, unsigned short *result, unsigned long base);
    cExtern unsigned short* sigclib_uitou16(unsigned int value, unsigned short *result, unsigned int base);
    cExtern unsigned short* sigclib_itou16(int value, unsigned short *result, unsigned int base);
    
    
  #else
  
    // convert ascii-0-string into uni-0-string. function will return parameter 'dst0'
    function global __cdecl sigclib_tostr16 var_input dst0:^uint; src0:^char; end_var var_output retcode:^uint; end_var;
    
    // convert uni-0-string into ascii-0-string. each character >255 will be converted to [SPACE]. function will return parameter 'dst0'
    function global __cdecl sigclib_tostr08 var_input dst0:^char; src0:^uint; end_var var_output retcode:^char; end_var;
    
    // get number of characters in uni-0-string.
    function global __cdecl sigclib_strlen16 var_input str0:^uint; end_var var_output retcode : udint; end_var;
    
    // copy uni-0-string to destination. function will return parameter 'dst0'
    function global __cdecl sigclib_strcpy16 var_input dst0:^uint; src0:^uint; end_var var_output retcode:^uint; end_var;
    
    // copies up to 'count' characters from src0 to dst0. in a case where the length of src0 is less than 'count', the remainder of dst will be padded with 0.
    function global __cdecl sigclib_strncpy16 var_input dst0:^uint; src0:^uint; count:udint; end_var var_output retcode:^uint; end_var;
    
    // compare uni-0-strings. function will return 0 when strings are equal, otherwiese <> 0
    function global __cdecl sigclib_strcmp16 var_input src1:^uint; src2:^uint; end_var var_output retcode:dint; end_var;
    
    // add 'ps2' at the end of 'ps1'. function will return parameter 'src1'
    function global __cdecl sigclib_strcat16 var_input ps1:^uint; ps2:^uint; end_var var_output retcode:^uint; end_var;
    
    // seek address of given character 'i' in uni-0-string. function will return valid pointer or NULL 
    function global __cdecl sigclib_strchr16 var_input ps1:^uint; i:udint; end_var var_output retcode:^uint; end_var;
    
    // convert all characters to uppercase in given uni-0-string. function will return parameter 'src'.
    function global __cdecl sigclib_strupr16 var_input src:^uint; end_var var_output retcode:^uint; end_var;
    
    // convert all characters to lowercase in given uni-0-string. function will return parameter 'src'.
    function global __cdecl sigclib_strlwr16 var_input src:^uint; end_var var_output retcode:^uint; end_var;
    
    // seek address of uni-0-string 'lookfor' in uni-0-string 'searchee'. function will return valid pointer or NULL if 'lookfor' is not found in 'searchee'.
    function global __cdecl sigclib_strstr16 var_input searchee:^uint; lookfor:^uint; end_var var_output retcode:^uint; end_var;
    
    // This function finds the last occurence of chr in the string pointed to by s including the 0-termination
    function global __cdecl sigclib_strrchr16 var_input ps1:^uint; chr:uint; end_var var_output retcode:^uint; end_var;
    
    // copies all characters from src into dst till first occurance of 'tillchr'
    function global __cdecl sigclib_strcpytill16 var_input dst:^uint; src:^uint; tillchr:uint; end_var var_output retcode:^uint; end_var;
    
    function global __cdecl sigclib_uitou16 var_input value:udint; result:^uint; base:udint; end_var var_output retcode:^uint; end_var;
    function global __cdecl sigclib_itou16 var_input value:dint; result:^uint; base:udint; end_var var_output retcode:^uint; end_var;
    
  #endif
#endif  

// ------------------------------------------------------------------------------------------------
// unsigned short* sigclib_tostr16(unsigned short *dst0, const char *src0);
// Function converts ascii-0-string into uni-0-string. 
// --> dst0 ..... Address of destination where created UNI-0-String should be captured
// --> src0 ..... Pointer to ASCII-0-String
// Function will always return given address of destination

// ------------------------------------------------------------------------------------------------
// char* sigclib_tostr08(char *dst0, const unsigned short *src0);
// Convert uni-0-string into ascii-0-string. each character >255 will be converted to [SPACE]. function will return parameter 'dst0'
// --> dst0 ..... Address of destination where created ASCII-0-String should be captured
// --> src0 ..... Pointer to UNI-0-String
// Function will always return given address of destination

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_strlen16(const unsigned short* str0);
// get number of characters in uni-0-string.
// Parameters:
// --> str0 ..... Address of UNI-0-String
// Function will return number of characters in given UNI-0-String.

// ------------------------------------------------------------------------------------------------
// unsigned short* sigclib_strcpy16(unsigned short* dst0, const unsigned short* src0);
// copy uni-0-string to destination.
// --> dst0 ..... Address of destination where copy of UNI-0-String should be captured
// --> src0 ..... Pointer to UNI-0-String
// Function will always return given address of destination

// ------------------------------------------------------------------------------------------------
// unsigned short* sigclib_strncpy16(unsigned short* dst0, const unsigned short* src0, unsigned long count);
// copies up to 'count' characters from src0 to dst0. in a case where the length of src0 is less than 'count', the remainder of dst will be padded with 0.
// --> dst0 ..... Address of destination
// --> src0 ..... Pointer to UNI-0-String
// --> count .... number of characters to copy
// Function will always return given address of destination

// ------------------------------------------------------------------------------------------------
// long sigclib_strcmp16(const unsigned short *src1, const unsigned short *src2);
// Function is used to compare each character in 2 uni-0-strings. 
// --> src1 ..... Address of 1.UNI-0-String to compare
// --> src1 ..... Address of 2.UNI-0-String to compare
// Function will return 0 when strings are equal, otherwiese <> 0

// ------------------------------------------------------------------------------------------------
// unsigned short* sigclib_strcat16(unsigned short *src1, const unsigned short *src2);
// Appends a copy of the source string (src2) to the destination string (src1).
// The terminating 0-character in destination is overwritten by the first character of source, and a 0-character is included at the end of the new string formed by the concatenation of both in destination.
// --> src1 ..... Destination
// --> src1 ..... Source
// Function will always return given address of destination

// ------------------------------------------------------------------------------------------------
// unsigned short* sigclib_strchr16(const unsigned short *src1, unsigned long i);
// seek address of given character 'i' in uni-0-string. 
// --> src1 ..... String where character is to be searched
// --> i ........ Character to search in  src1
// function will return valid pointer or NULL 

// ------------------------------------------------------------------------------------------------
// unsigned short* sigclib_strupr16(unsigned short *src);
// convert all characters to uppercase in given uni-0-string. 
// --> src ...... string to convert
// NOTE: Function will work inplace and convertion just work with characters smaller than 256
// function will return parameter 'src'.

// ------------------------------------------------------------------------------------------------
// unsigned short* sigclib_strlwr16(unsigned short *src);
// convert all characters to lowercase in given uni-0-string.
// --> src ...... string to convert
// NOTE: Function will work inplace and convertion just work with characters smaller than 256
// function will return parameter 'src'.

// ------------------------------------------------------------------------------------------------
// unsigned short* sigclib_strstr16(const unsigned short* searchee, const unsigned short* lookfor);
// seek address of uni-0-string 'lookfor' in uni-0-string 'searchee'. 
// --> searchee .. string where 'lookfor' will be searched
// --> lookfor ... string to look fo in 'serachee'
// function will return valid pointer or NULL if 'lookfor' is not found in 'searchee'.

// ------------------------------------------------------------------------------------------------
// unsigned short *sigclib_strrchr16(const unsigned short *s, unsigned short chr);
// This function finds the last occurence of chr in the string pointed to by s including the 0-termination
// --> s ........ string to be scanned. 
// --> chr ...... searchee
// Return Value:
// Returns a pointer to the located character, or a null pointer when character was not found

// ------------------------------------------------------------------------------------------------
// unsigned short* sigclib_strcpytill16(unsigned short* dst, const unsigned short* src, unsigned short tillchr)
// Copies all characters from src into dst till first occurance of 'tillchr'.
// Destination string will end right before occurance of 'tillchr'.
// --> dst ...... Pointer to the destination array where the content is to be copied. 
// --> src ...... string to be performed. 
// --> tillchr .. character to stop at
// function returns pointer to first characeter in 'src' right after found 'tillchr' or NULL if 'tillchr' was not found
// NOTE: If 'tillchr' was not found whole content of src will be copied into dst.

// ------------------------------------------------------------------------------------------------
// char* sigclib_itou16(_int32 value, _uint16 *result, _uint32 base)
// Convert value to u16-string
// Converts a value val into a null-terminated u16-string using the specified base and stores the result in the array given by result parameter.
// If base is 10 and val is negative, the resulting string is preceded with a minus sign. With any other base, value is always considered unsigned.
// result should be an array long enough to contain any possible value

// Parameters:
// --> value .... Value to be converted to a u16-string. 
// --> result ... Array in memory where to store the resulting null-terminated string. 
// --> base ..... Numerical base used to represent the value as a string, between 2 and 36, where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.

// Return Value:
// same than given parameter result

// ------------------------------------------------------------------------------------------------
// char* sigclib_uitou16(_uint32 value, _uint16 *result, _uint32 base) ----------------------------
// same than sigclib_itou16() will just use _uint32 value instead _int32

// ------------------------------------------------------------------------------------------------
// char* sigclib_lltou16(_int64 value, _uint16 *result, _uint32 base) -----------------------------
// same than sigclib_itou16() will just use _int64 value instead _int32

// ------------------------------------------------------------------------------------------------
// char* sigclib_ulltou16(_uint64 value, _uint16 *result, _uint32 base) ---------------------------
// same than sigclib_itou16() will just use _uint64 value instead _int32
