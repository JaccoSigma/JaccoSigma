
#ifndef _CFileLibH
 #define _CFileLibH
  
  #include "DefineCompiler.h"

  #ifdef cCompile
    
    #include "lsl_c_ifssr.h"
    #include "RTOS_C_interfaces.h"
    #include "file_iotypes.h"

    #define sigclib_logfile(...) { char __tmp[512]; sigclib_sprintf(__tmp, __VA_ARGS__); sigclib_logfileline(__tmp); }


    // file open
    cExtern long sigclib_fopen(char *dpne, unsigned long state);
      // use following state: ATT_READ_WRITE, ATT_READ_ONLY, ATT_OPEN_SHARED, ATT_OPEN_NO_DIR, ATT_OPEN_DIR
      //                      ATT_CREATE, ATT_CREATE_ALWAYS
      // function will return value < 0 in case of error, on the other hand a handle of value >= 0
  
    // file close
    cExtern void sigclib_fclose(long hdl);
  
    // read from file
    cExtern unsigned long sigclib_fread(long hdl, void *dst, unsigned long length);
  
    // read stream from file till LF or EOF occures, CR will be ignored
    cExtern char *sigclib_fgets(long hdl, char *s, unsigned long size);
  
    // read stream16 from file till LF or EOF occures, CR will be ignored
    cExtern unsigned short *sigclib_fgets16(long hdl, unsigned short *s, unsigned long size);
  
    // write 0-terminated string into file
    cExtern unsigned long sigclib_fputs(long hdl, const char *s);
  
    // write 0-terminated u16 string into file
    cExtern unsigned long sigclib_fputs16(long hdl, const unsigned short *s);
  
    // write data into file
    cExtern unsigned long sigclib_fwrite(long hdl, void *dst, unsigned long length);
  
    // write CRLF into file
    cExtern unsigned long sigclib_fwrite_crlf(long hdl);

    // write u16-CRLF into file
    cExtern unsigned long sigclib_fwrite_crlf16(long hdl);
  
    // set position of filepointer
    cExtern unsigned long sigclib_fseek(long hdl, long offset, unsigned long fromwhere);
      // #define FILE_BEGIN   0
      // #define FILE_CURRENT 1
      // #define FILE_END     2
  
    // get position of filepointer
    cExtern long sigclib_ftell(long hdl);
  
    // get length of file
    cExtern long sigclib_flength(long hdl);
  
    // remove file
    cExtern long sigclib_remove(char *dpne);

    // rename file  
    cExtern long sigclib_rename(char* oldname, char* newname);
  
    // findfirst
    cExtern long sigclib_findfirst(char *namepattern, unsigned char attr0, unsigned char attrmask, _DDE_INFO *fileinfo, char *filename, unsigned long maxlength);
  
    // findnext
    cExtern long sigclib_findnext(long hdl, _DDE_INFO *fileinfo, char *filename, unsigned long maxlength);
  
    // findclose
    cExtern long sigclib_findclose(long hdl);
    
    // create new directory, return 0 on success or negative errorcode
    cExtern long sigclib_mkdir(const char *path);

    // remove existiong directory, return 0 on success or negative errorcode
    cExtern long sigclib_rmdir(const char *path);
    
    // create new file 'dpne' with content 'data' of length 'size'. return 1 on success, or 0 if error occured
    cExtern unsigned long sigclib_filecreate(const char *dpne, void *ptr, unsigned long size);

    // get content of already existing file
    cExtern void *sigclib_fileread(unsigned long *psize, const char *dpne);

    // ********************************************************************************************
    // fast fileaccess, using ram-cache ***********************************************************
    // ********************************************************************************************
   
    // open existing or create new file, use cache of fildata when requested
    cExtern void*         sigclib_cfopen(const char *dpne, unsigned long state);
    cExtern void*         sigclib_cfopenex(const char *dpne, unsigned long state, unsigned long writesize);
      // use state 'r', 'R' to open existing file
      // use state 'w', 'W' to create new file, existing file will be overwritten
      // use state 'a', 'A' to create new or append existing file
      // when use state 'R' or 'W' filedata will be cached in ram for faster sequential read or write
      
    // close file opend with function sigclib_cfopen  
    cExtern unsigned long sigclib_cfclose(void *hdl);
      // function will return 1 on success, on the other hand 0
    
    // read data from file
    cExtern unsigned long sigclib_cfread(void *hdl, void *dst, unsigned long bytesize);
    
    // write data into file
    cExtern unsigned long sigclib_cfwrite(void *hdl, const void *src, unsigned long bytesize);
    
    // write CR-LF into file
    cExtern unsigned long sigclib_cfwrite_crlf(void *hdl);

    // write u16 CR-LF into file
    cExtern unsigned long sigclib_cfwrite_crlf16(void *hdl);

    // get length of file
    cExtern long sigclib_cflength(void *hdl);
    
    // set position of filepointer
    cExtern unsigned long sigclib_cfseek(void *hdl, long offset, unsigned long fromwhere);
      // #define FILE_BEGIN   0
      // #define FILE_CURRENT 1
      // #define FILE_END     2
    
    // get position of filepointer
    cExtern long          sigclib_cftell(void *hdl);
    
    // read stream from file till LF or EOF occures, CR will be ignored
    cExtern char*         sigclib_cfgets(void *hdl, char *s, unsigned long size);

    // read u16-stream from file till LF or EOF occures, CR will be ignored
    cExtern unsigned short *sigclib_cfgets16(void* hdl, unsigned short *s, unsigned long size);

    // write 0-terminated string into file    
    cExtern unsigned long sigclib_cfputs(void *hdl, const char *s);
    
    // write 0-terminated u16 string into file    
    cExtern unsigned long sigclib_cfputs16(void *hdl, const unsigned short *s);
    
    // memoryoperation in combination to handle
    cExtern unsigned long sigclib_cfmemory(void *hdl, void **pptr, unsigned long bytesize);

    // ask if handle is ok
    cExtern unsigned long sigclib_cfok(void *hdl);
    
    // set errorcode
    cExtern unsigned long sigclib_cferror_set(void *hdl, unsigned long val);
    
    // get errorcode
    cExtern unsigned long sigclib_cferror_get(void *hdl);
    
    // set version
    cExtern unsigned long sigclib_cfversion_set(void* hdl, unsigned long version);

    // get version
    cExtern unsigned long sigclib_cfversion_get(void* hdl);
    
    // set cookie
    cExtern unsigned long sigclib_cfcookie_set(void* hdl, void *pcookie);

    // get cookie
    cExtern  void *sigclib_cfcookie_get(void* hdl);
    
    // determine all drives that are currently available
    cExtern unsigned long sigclib_drivelist(char *dst);
    
    // delete given directory inclusive whole content (files + subdirectories)
    cExtern long sigclib_remove_directory(const char *dp0);
    
    // function is used to delete files inside of given directory including subdir. the directory itself or subdirectories will not be removed by this function.
    cExtern long sigclib_cleanup_directory(const char *dp0, const char *ne0, unsigned long subdir);
    
    // function is used to return the attributes of specific file
    cExtern long sigclib_get_attributes(const char *dpne);
    
    // function is used to assign a new set of attributes to a given file
    cExtern long sigclib_set_attributes(const char *dpne, unsigned char attributes);
    
    // function is used to write text into c:\sigclog.txt
    cExtern void sigclib_logfileline(const char *txt);

    // function is used to write formatted text into c:\sigclog.txt
    cExtern void sigclib_logfileST(const char *format, void *p0, void *p1, void *p2, void *p3, void *p4, void *p5, void *p6, void *p7, void *p8, void *p9);
    
    
    
  #else
    #include "file_iotypes.h"

    // file open
    function global __cdecl sigclib_fopen var_input dpne:^char; state:udint; end_var var_output retcode : dint; end_var;
  
    // file close
    function global __cdecl sigclib_fclose var_input hdl:dint; end_var;
  
    // read from file
    function global __cdecl sigclib_fread var_input hdl:dint; dst:^void; length:udint; end_var var_output retcode : udint; end_var;
  
    // read stream from file till LF or EOF occures
    function global __cdecl sigclib_fgets var_input hdl:dint; s:^char; size:udint; end_var var_output retcode : ^char; end_var;

    // read stream16 from file till LF or EOF occures, CR will be ignored
    function global __cdecl sigclib_fgets16 var_input hdl:dint; s:^uint; size:udint; end_var var_output retcode : ^uint; end_var;
  
    // write 0-terminated string into file  
    function global __cdecl sigclib_fputs var_input hdl:dint; s:^char; end_var var_output retcode : udint; end_var;
  
    // write 0-terminated u16 string into file
    function global __cdecl sigclib_fputs16 var_input hdl:dint; s:^uint; end_var var_output retcode : udint; end_var;
  
    // write data into file
    function global __cdecl sigclib_fwrite var_input hdl:dint; dst:^void; length:udint; end_var var_output retcode : udint; end_var;

    // write CRLF into file
    function global __cdecl sigclib_fwrite_crlf var_input hdl:dint; end_var var_output retcode:udint; end_var;

    // write u16-CRLF into file
    function global __cdecl sigclib_fwrite_crlf16 var_input hdl:dint; end_var var_output retcode:udint; end_var;
  
    // set position of filepointer
    function global __cdecl sigclib_fseek var_input hdl:dint; offset:dint; fromwhere:udint; end_var var_output retcode : udint; end_var;
  
    // get position of filepointer
    function global __cdecl sigclib_ftell var_input hdl:dint; end_var var_output retcode : dint; end_var;
  
    // get length of file
    function global __cdecl sigclib_flength var_input hdl:dint; end_var var_output retcode : dint; end_var;
    
    // remove file
    function global __cdecl sigclib_remove var_input dpne:^char; end_var var_output retcode : dint; end_var;
    
    // rename file  
    function global __cdecl sigclib_rename var_input oldname:^char; newname:^char; end_var var_output retcode : dint; end_var;
  
    // findfirst
    function global __cdecl sigclib_findfirst var_input namepattern:^char; attr0:usint; attrmask:usint; fileinfo:^_DDE_INFO; filename:^char; maxlength:udint; end_var var_output retcode:dint; end_var;
    
    // findnext
    function global __cdecl sigclib_findnext var_input hdl:dint; fileinfo:^_DDE_INFO; filename:^char; maxlength:udint; end_var var_output retcode : dint; end_var;
  
    // findclose
    function global __cdecl sigclib_findclose var_input hdl:dint; end_var var_output retcode : dint; end_var;
    
    //  hdl := sigclib_findfirst("c:\*.*", 0, 0, #dde, #name[0], 128);
    //  repeat
    //    if(dde.Attributes AND ATTR_DIR) then
    //    end_if;
    //  until(sigclib_findnext(hdl, #dde, #name[0], 128) <> 0) end_repeat;
    //  sigclib_findclose(hdl);

    // create new directory, return 0 on success or negative errorcode
    function global __cdecl sigclib_mkdir var_input path:^char; end_var var_output retcode : dint; end_var;
    
    // remove existiong directory, return 0 on success or negative errorcode
    function global __cdecl sigclib_rmdir var_input path:^char; end_var var_output retcode : dint; end_var;
  
    // create new file 'dpne' with content 'data' of length 'size'. return 1 on success, or 0 if error occured
    function global __cdecl sigclib_filecreate var_input dpne:^char; ptr:^void; size:udint; end_var var_output retcode : udint; end_var;

    // get content of already existing file
    function global __cdecl sigclib_fileread var_input psize:^udint; dpne:^char; end_var var_output retcode:^void; end_var;

    // ********************************************************************************************
    // fast fileaccess, using ram-cache ***********************************************************
    // ********************************************************************************************
   
    // open existing or create new file, use cache of fildata when requested
    function global __cdecl  sigclib_cfopen var_input dpne:^char; state:udint; end_var var_output retcode:^void; end_var;
    function global __cdecl  sigclib_cfopenex var_input dpne:^char; state:udint; writesize:udint; end_var var_output retcode:^void; end_var;
      // use state 'r', 'R' to open existing file
      // use state 'w', 'W' to create new file, existing file will be overwritten
      // use state 'a', 'A' to create new or append existing file
      // when use state 'R' or 'W' filedata will be cached in ram for faster sequential read or write
      
    // close file opend with function sigclib_cfopen
    function global __cdecl  sigclib_cfclose var_input hdl:^void; end_var var_output retcode:udint; end_var;
      // function will return 1 on success, on the other hand 0
    
    // read data from file
    function global __cdecl  sigclib_cfread var_input hdl:^void; dst:^void; bytesize:udint; end_var var_output retcode:udint; end_var;
    
    // write data into file
    function global __cdecl  sigclib_cfwrite var_input hdl:^void; stc:^void; bytesize:udint; end_var var_output retcode:udint; end_var;
    
    // write CR-LF into file
    function global __cdecl  sigclib_cfwrite_crlf var_input hdl:^void; end_var var_output retcode:udint; end_var;
    
    // write u16 CR-LF into file
    function global __cdecl  sigclib_cfwrite_crlf16 var_input hdl:^void; end_var var_output retcode:udint; end_var;
    
    // get length of file
    function global __cdecl  sigclib_cflength var_input hdl:^void; end_var var_output retcode:dint; end_var;
    
    // set position of filepointer
    function global __cdecl  sigclib_cfseek var_input hdl:^void; offset:dint; fromwhere:udint; end_var var_output retcode:udint; end_var;
      // #define FILE_BEGIN   0
      // #define FILE_CURRENT 1
      // #define FILE_END     2
    
    // get position of filepointer
    function global __cdecl  sigclib_cftell var_input hdl:^void; end_var var_output retcode:dint; end_var;
    
    // read stream from file till LF or EOF occures, CR will be ignored
    function global __cdecl  sigclib_cfgets var_input hdl:^void; s:^char; size:udint; end_var var_output retcode:^char; end_var;

    // read u16-stream from file till LF or EOF occures, CR will be ignored
    function global __cdecl  sigclib_cfgets16 var_input hdl:^void; s:^uint; size:udint; end_var var_output retcode:^uint; end_var;
    
    // write 0-terminated string into file
    function global __cdecl  sigclib_cfputs var_input hdl:^void; s:^char; end_var var_output retcode:udint; end_var;
    
    // write 0-terminated string into file
    function global __cdecl  sigclib_cfputs16 var_input hdl:^void; s:^uint; end_var var_output retcode:udint; end_var;
    
    // memoryoperation in combination to handle
    function global __cdecl  sigclib_cfmemory var_input hdl:^void; pptr:^pvoid; bytesize:udint; end_var var_output retcode:udint; end_var;

    // ask if handle is ok
    function global __cdecl  sigclib_cfok var_input hdl:^void; end_var var_output retcode:udint; end_var;
    
    // set errorcode
    function global __cdecl  sigclib_cferror_set var_input hdl:^void; val:udint; end_var var_output retcode:udint; end_var;
    
    // get errorcode
    function global __cdecl  sigclib_cferror_get var_input hdl:^void; end_var var_output retcode:udint; end_var;
    
    // set version
    function global __cdecl  sigclib_cfversion_set var_input hdl:^void; version:udint; end_var var_output retcode:udint; end_var;

    // get version
    function global __cdecl  sigclib_cfversion_get var_input hdl:^void; end_var var_output retcode:udint; end_var;
    
    // set cookie
    function global __cdecl  sigclib_cfcookie_set var_input hdl:^void; pcookie:^void; end_var var_output retcode:udint; end_var;
    
    // get cookie
    function global __cdecl  sigclib_cfcookie_get var_input hdl:^void; end_var var_output retcode:^void; end_var;
    
    // determine all drives that are currently available
    function global __cdecl sigclib_drivelist var_input dst:^char; end_var var_output retcode:udint; end_var;
    
    // delete given directory inclusive whole content (files + subdirectories)
    function global __cdecl sigclib_remove_directory var_input dp0:^char; end_var var_output retcode:dint; end_var;
  
    // function is used to delete files inside of given directory including subdir. the directory itself or subdirectories will not be removed by this function.
    function global __cdecl sigclib_cleanup_directory var_input dp0:^char; ne0:^char; subdir:udint; end_var var_output retcode:dint; end_var;
  
    // function is used to return the attributes of specific file
    function global __cdecl sigclib_get_attributes var_input dpne:^char; end_var var_output retcode:dint; end_var;

    // function is used to assign a new set of attributes to a given file
    function global __cdecl sigclib_set_attributes var_input dpne:^char; attributes:usint; end_var var_output retcode:dint; end_var;
    
    // function is used to write textline into c:\sigclog.txt
    function global __cdecl sigclib_logfileline var_input txt:^char; end_var;
    
    // function is used to write formatted textline into c:\sigclog.txt
    function global __cdecl sigclib_logfileST
      var_input
        format : ^char;
        p0 : ^void := NIL;
        p1 : ^void := NIL;
        p2 : ^void := NIL;
        p3 : ^void := NIL;
        p4 : ^void := NIL;
        p5 : ^void := NIL;
        p6 : ^void := NIL;
        p7 : ^void := NIL;
        p8 : ^void := NIL;
        p9 : ^void := NIL;
      end_var;
    
  #endif

#endif


// ************************************************************************************************
// USAGE
// ************************************************************************************************

// ------------------------------------------------------------------------------------------------
// long sigclib_fopen(char *dpne, unsigned long state);
// file open
// --> dpne ............ Drive, Path, Name + Extention of file to open (eg. c:\subdir\file.bin)
// --> state ........... ATT_READ_WRITE, ATT_READ_ONLY, ATT_OPEN_SHARED, ATT_OPEN_NO_DIR, ATT_OPEN_DIR, ATT_CREATE, ATT_CREATE_ALWAYS
// function will return value < 0 in case of error, on the other hand a handle of value >= 0
  
// ------------------------------------------------------------------------------------------------
// void sigclib_fclose(long hdl);
// file close
// --> hdl ............. valid filehandle
  
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_fread(long hdl, void *dst, unsigned long length);
// read content from file
// --> hdl ............. valid filehandle
// --> dst ............. destination memory
// --> length .......... length to read from file in bytes
// <-- function will return number of bytes to read
  
// ------------------------------------------------------------------------------------------------
// char *sigclib_fgets(long hdl, char *s, unsigned long size);
// read stream from file till <LF> or <EOF> occures, <CR> will be ignored
// --> hdl ............. valid filehandle
// --> s ............... destination memory
// --> size ............ bytesize of destination memory
// <-- function will return pointer to destination memory or NULL in case of no more data
// NOTE: destination will always be terminated by 0 (ascii-0-string)

// ------------------------------------------------------------------------------------------------
// unsigned short *sigclib_fgets16(long hdl, unsigned short *s, unsigned long size);
// read stream from file till 16Bit <LF> or <EOF> occures, <CR> will be ignored
// --> hdl ............. valid filehandle
// --> s ............... destination memory
// --> size ............ bytesize of destination memory
// <-- function will return pointer to destination memory or NULL in case of no more data
// NOTE: destination will always be terminated by 0 (u16-0-string)

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_fputs(long hdl, const char *s);
// write ascii-0-string into file
// --> hdl ............. valid filehandle
// --> s ............... string to write
// <-- function will return amount of written bytes

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_fputs16(long hdl, const unsigned short *s);
// write u16-0-string into file
// --> hdl ............. valid filehandle
// --> s ............... u16-0-string to write
// <-- function will return amount of written bytes

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_fwrite(long hdl, void *src, unsigned long length);
// write data into file
// --> hdl ............. valid filehandle
// --> src ............. data to write
// --> length .......... amount of data to write in bytes
// <-- function will return amount of written bytes
  
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_fseek(long hdl, long offset, unsigned long fromwhere);
// set position of filepointer
// --> hdl ............. valid filehandle
// --> offset .......... offset in bytes
// --> fromwhere ....... FILE_BEGIN 0, FILE_CURRENT 1, FILE_END 2
  
// ------------------------------------------------------------------------------------------------
// long sigclib_ftell(long hdl);
// get position of filepointer
// --> hdl ............. valid filehandle
// <-- function will return offset of filepointer regarding to first byte in file
  
// ------------------------------------------------------------------------------------------------
// long sigclib_flength(long hdl);
// get length of file
// --> hdl ............. valid filehandle
// <-- function will return bytesize of file  
  
// ------------------------------------------------------------------------------------------------
// long sigclib_remove(char *dpne);
// remove file
// --> dpne ............ Drive, Path, Name + Extention of file to open (eg. c:\subdir\file.bin)
// <-- function will return value <0 on error

// ------------------------------------------------------------------------------------------------
// long sigclib_rename(char* oldname, char* newname);
// rename file  
// --> oldname ......... existiong Drive, Path, Name + Extention of file to open (eg. c:\subdir\file.bin)
// --> newname ......... existiong Drive, Path, Name + Extention of file to open (eg. c:\subdir\newfile.bin)
// <-- function will return value <0 on error
  
// ------------------------------------------------------------------------------------------------
// long sigclib_findfirst(char *namepattern, unsigned char attr0, unsigned char attrmask, _DDE_INFO *fileinfo, char *filename, unsigned long maxlength);
// findfirst
// --> namepattern ..... 
// --> attr0 ...........
// --> attrmask ........
// --> fileinfo ........
// --> filename ........ address where name should be filed
// --> maxlength ....... memorysize behind addrsee where name should be filed
// <-- function will return valid handle (>=0) or a negative errorcode (<0) on error
  
// ------------------------------------------------------------------------------------------------
// long sigclib_findnext(long hdl, _DDE_INFO *fileinfo, char *filename, unsigned long maxlength);
// findnext
// --> hdl ............. valid handle
// --> fileinfo ........ address where fileinfo should be filed
// --> filename ........ address where name should be filed
// --> maxlength ....... memorysize behind addrsee where name should be filed
// <-- function will return 0 on success
  
// ------------------------------------------------------------------------------------------------
// long sigclib_findclose(long hdl);
// findclose
// --> hdl ............. valid handle

// Example:
//  char name[256];
//  _DDE_INFO dde;
//  long hdl = sigclib_findfirst("D:\\txt\\*.txt", 0, 0, &dde, name, sizeof(name));
//  if(hdl >= 0)
//  {
//    do
//    {
//      //todo: name
//      //Note: defined Attributes: ATTR_READ_ONLY, ATTR_HIDDEN, ATTR_SYSTEM, ATTR_VOLUME, ATTR_DIR, ATTR_ARCHIVE
//    }
//    while(sigclib_findnext(hdl, &dde, name, sizeof(name)) == 0);
//    sigclib_findclose(hdl);
//  }
    
// ------------------------------------------------------------------------------------------------
// long sigclib_mkdir(const char *path);
// create new directory
// --> path ............ path with subdir to create
// <-- function will return 0 on success or negative errorcode

// ------------------------------------------------------------------------------------------------
// long sigclib_rmdir(const char *path);
// remove existiong directory
// --> path ............ path with subdir to remove
// <-- function return 0 on success or negative errorcode
    
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_filecreate(const char *dpne, void *ptr, unsigned long size);
// create new file 'dpne' with content 'data' of length 'size'
// --> dpne ............ Drive, Path, Name + Extention of file to create (eg. c:\subdir\file.bin)
// --> ptr ............. data to Write
// --> size ............ amount of data in bytes
// <-- function will return 1 on success, or 0 if error occured

// ------------------------------------------------------------------------------------------------
// void *sigclib_fileread(unsigned long *psize, const char *dpne);
// get content of already existing file
// --> psize ........... address of variable where size of adressed file should be captured
// --> dpne ............ Drive, Path, Name + Extention of file to read (eg. c:\subdir\file.bin)
// <-- function will return pointer to allocated memory including filecontent, or NULL on error
// NOTE: Do not forget to use sigclib_free() to free allocated memory after usage

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_drivelist(char *dst);
// determine all drives that are currently available
// --> dst ............. array with minimum 27 cells of char
// <-- function will return number of currently available drives, each driveletter will be stored in given array.

// ------------------------------------------------------------------------------------------------
// void *sigclib_cfopen(const char *dpne, unsigned long state);
// void *sigclib_cfopenex(const char *dpne, unsigned long state, unsigned long writesize);
// open existing or create new file, use ram-cache of fildata when requested
// --> dpne ............ drive + path + name + extention of file to open
// --> state ........... attribute
//                       use state 'r', 'R' to open existing file (read access, fp=filebegin)
//                       use state 'w', 'W' to create new file, existing file will be dumped (write access, fp=filebegin)
//                       use state 'a', 'A' to create new or append existing file (write access, fp=fileend)
//                       use state 'c', 'C' to create new, read or write to existing file (read and write access, fp=filebegin)
//                       when use state 'R'(capital letter R) or 'W'(capital letter W) filedata will be cached in ram for faster sequential read or write
// --> writesize ....... estimated size of ram-cache (just when using function sigclib_cfopenex)
// <-- function will return a valid handle or NULL in case of error
      
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_cfclose(void *hdl);
// close file opend with function sigclib_cfopen  
// --> hdl ............. handle or NULL
// <-- function will return 1 on success, on the other hand 0
    
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_cfread(void *hdl, void *dst, unsigned long bytesize);
// read data from file
// --> hdl ............. handle or NULL
// --> dst ............. destination memory
// --> bytesize ........ number of bytes to read
// <-- function will return number of bytes to read or 0 in case of error
    
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_cfwrite(void *hdl, const void *src, unsigned long bytesize);
// write data into file
// --> hdl ............. handle or NULL
// --> src ............. data to write
// --> bytesize ........ amount of data to write in bytes
// <-- function will return amount of written bytes or 0 in case of error
    
// ------------------------------------------------------------------------------------------------
// write CR-LF into file
// unsigned long sigclib_cfwrite_crlf(void *hdl);
// --> hdl ............. handle or NULL
// <-- function will return amount of written bytes (always 2) or 0 in case of error
    
// ------------------------------------------------------------------------------------------------
// write u16 CR-LF into file
// unsigned long sigclib_cfwrite_crlf16(void *hdl);
// --> hdl ............. handle or NULL
// <-- function will return amount of written bytes (always 2) or 0 in case of error
    
// ------------------------------------------------------------------------------------------------
// long sigclib_cflength(void *hdl);
// get length of file
// --> hdl ............. valid filehandle
// <-- function will return bytesize of file, an negative value in case of error
    
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_cfseek(void *hdl, long offset, unsigned long fromwhere);
// set position of filepointer
// --> hdl ............. valid filehandle
// --> offset .......... offset in bytes
// --> fromwhere ....... FILE_BEGIN 0, FILE_CURRENT 1, FILE_END 2
// <-- function will return 1 or 0 in case of error
    
// ------------------------------------------------------------------------------------------------
// long sigclib_cftell(void *hdl);
// get position of filepointer
// --> hdl ............. valid filehandle
// <-- function will return offset of filepointer regarding to first byte in file or a negative value on error
    
// ------------------------------------------------------------------------------------------------
// char *sigclib_cfgets(void *hdl, char *s, unsigned long size);
// read stream from file till LF or EOF occures, CR will be ignored
// --> hdl ............. handle or NULL
// --> s ............... memorylocation (destination) where stream should be stored
// --> size ............ bytesize of memorylocation
// <-- function will return pointer to given memorylocation or NULL on error
// NOTE: destination will always be terminated by 0 (ascii-0-string)

// ------------------------------------------------------------------------------------------------
// unsigned short *sigclib_cfgets16(void *hdl, unsigned short *s, unsigned long size);
// read u16-stream from file till LF or EOF occures, CR will be ignored
// --> hdl ............. handle or NULL
// --> s ............... memorylocation (destination) where u16-stream should be stored
// --> size ............ bytesize of memorylocation
// <-- function will return pointer to given memorylocation or NULL on error
// NOTE: destination will always be terminated by 0 (u16-0-string)
   
// ------------------------------------------------------------------------------------------------
// char *sigclib_cfputs(void *hdl, const char *s);
// write ascii-0-string into file
// --> hdl ............. valid filehandle
// --> s ............... string to write
// <-- function will return amount of written bytes

// ------------------------------------------------------------------------------------------------
// unsigned short *sigclib_cfputs16(void *hdl, const unsigned short *s);
// write u16-0-string into file
// --> hdl ............. valid filehandle
// --> s ............... u16-0-string to write
// <-- function will return amount of written bytes
   
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_cfmemory(void *hdl, void **pptr, unsigned long bytesize);
// memoryoperation in combination to handle
// --> hdl ............. handle or NULL
// --> pptr ............ pointer to pointer
// --> bytesize ........ bytesize to alloc, realloc free
// <-- function will return <>0 on success, on the other hand 0

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_cfok(void *hdl);
// ask if handle is ok
// --> hdl ............. handle or NULL
// <-- function will return <>0 when handle is ok, on the other hand 0 
    
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_cferror_set(void *hdl, unsigned long val);
// set errorcode in internal structure
// --> hdl ............. handle or NULL
// --> val ............. errornumber to set
    
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_cferror_get(void *hdl);
// get errorcode
// --> hdl ............. handle or NULL
// <-- function will return errorcode, 0=everything is ok    
    
// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_cfversion_set(void* hdl, unsigned long version);
// set userspecific versionnumber in internal datastructure
// --> hdl ............. handle or NULL
// --> version ......... versionnumber to set
// <-- function will return <>0 when version is set to internal data, on the other hand 0 

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_cfversion_get(void* hdl);
// get userspecific version from internal datastructure
// --> hdl ............. handle or NULL
// <-- function will return userspecific versionnumber, set by function sigclib_cfversion_set()

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_cfcookie_set(void* hdl, void *pcookie);
// set userspecific cookie in internal datastructure
// --> hdl ............. handle or NULL
// --> pcookie ......... cookie to set
// <-- function will return <>0 when cookie is set to internal data, on the other hand 0 

// ------------------------------------------------------------------------------------------------
// void *sigclib_cfcookie_get(void* hdl);
// get userspecific cookie from internal datastructure
// --> hdl ............. handle or NULL
// <-- function will return userspecific cookie, set by function sigclib_cfcookie_set()

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_fwrite_crlf(long hdl);
// write <CR><LF> into file
// --> hdl ............. valid handle
// <-- function will return amount of written bytes (normally 2)

// ------------------------------------------------------------------------------------------------
// unsigned long sigclib_fwrite_crlf16(long hdl);
// write <CR><LF> (U-16 !) into file
// --> hdl ............. valid handle
// <-- function will return amount of written bytes (normally 4)

// ------------------------------------------------------------------------------------------------
// long sigclib_remove_directory(const char *dp0);
// delete given directory inclusive whole content (files + subdirectories)
// --> dp0 ............. drive and path (directory) to delete (eg. "C:\Test")
// <-- Function will return a negative value (< 0) on error (errorcode)
// NOTE: Rootdirectory "C:\" can not be deleted

// ------------------------------------------------------------------------------------------------
// long sigclib_cleanup_directory(const char *dp0, const char *ne0, unsigned long subdir);
// function is used to delete files inside of given directory including subdir. the directory itself or subdirectories will not be removed by this function.
// --> dp0 ............. drive and path (directory) to clean (eg. "C:\Test")
// --> ne0 ............. filenamespezifier (eg. "*.*", "*.dat") Wildcards like '*' and '?' are allowed
// --> subdir .......... set to 1 to cleanup subdirectories as well, otherwise 0
// <-- Function will return a negative value (< 0) on error (errorcode)
// NOTE: Do not delete all files in Rootdirectory of drive C. Therefore call of sigclib_cleanup_directory("C:\", "*.*", 0) won't work.

// ------------------------------------------------------------------------------------------------
// function is used to return the attributes of specific file or directory
// long sigclib_get_attributes(const char *dpne);
// --> dpne ............ drive, path and name (or directory) of file to retrieve attributes
// <-- Function will return attribute or a negative value (< 0) on error (errorcode)
// NOTE: If the function return value is positive, it contains the file's attributes, which can be any combination of the following values. A negative value will be given on error.
//   #define ATTR_READ_ONLY      	0x01
//   #define ATTR_HIDDEN         	0x02
//   #define ATTR_SYSTEM         	0x04
//   #define ATTR_VOLUME         	0x08
//   #define ATTR_DIR            	0x10
//   #define ATTR_ARCHIVE        	0x20
//   #define LONGNAME_ATTR       	(ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME)

// ------------------------------------------------------------------------------------------------
// function is used to assign a new set of attributes to a given file
// long sigclib_set_attributes(const char *dpne, unsigned char attributes);
// --> dpne ............ drive, path and name of file to set attributes
// --> attributes ...... combination of attributes to set
// <-- Function will return a negative value on error.
// NOTE: A comobation of following attributes ca ne used
//   RTF_ATTR_READ_ONLY ... file is read only
//   RTF_ATTR_HIDDEN ...... file is marked as hidden
//   RTF_ATTR_SYSTEM ...... file is marked as being a system file
//   RTF_ATTR_ARCHIVE ..... file is marked to be backed up

// ------------------------------------------------------------------------------------------------
// Function (macro) is used to write (append) formatted textline into C:\SigCLog.txt 
// sigclib_logfile()
// Usage is like sigclib_printf()
// NOTE: Each call of function will extend file C:\SigCLog.txt. Therefore do not use it periodically, you will spoil the flashdrive.
// NOTE: Logfile will not exceed size of 100000 bytes. In that case logfile will be deleted and start with first line "...".

// ------------------------------------------------------------------------------------------------
// Function is used to write (append) a single textline into C:\SigCLog.txt
// void sigclib_logfileline(const char *txt);
// --> dpne ............ ascii-text to write into log-file
// NOTE: Each single usage of function will lead to a single line in textfile (CR-LF will be automatically added)
// NOTE: Each call of function will extend file C:\SigCLog.txt. Therefore do not use it periodically, you will spoil the flashdrive.
// NOTE: Logfile will not exceed size of 100000 bytes. In that case logfile will be deleted and start with first line "...".

// ------------------------------------------------------------------------------------------------
// Function is used to write (append) formatted textline into C:\SigCLog.txt
// void sigclib_logfileST(const char *format, void *p0, void *p1, void *p2, void *p3, void *p4, void *p5, void *p6, void *p7, void *p8, void *p9);
// Parameter usage is like sigclib_sprintfST() without first parameter 'pd'
// NOTE: Each call of function will extend file C:\SigCLog.txt. Therefore do not use it periodically, you will spoil the flashdrive.
// NOTE: Logfile will not exceed size of 100000 bytes. In that case logfile will be deleted and restart with first line "...".
