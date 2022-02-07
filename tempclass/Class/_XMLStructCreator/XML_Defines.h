//XML_Defines.h

// most needed signs and commands ---------
//ASCII signs
#define XML_CR                16#0D
#define XML_LF                16#0A
#define XML_TABULATOR         16#09
#define XML_EQUAL             16#3D
#define XML_START             16#3C
#define XML_END               16#3E 
#define XML_SLASH             16#2F
#define XML_QUOTMARK          16#22
#define XML_SPACE             16#20
#define XML_EXCLAMATION       16#21
#define XML_APOSTROPHE        16#27
#define XML_HYPHEN            16#2D
#define XML_AMPERSAND         16#26

//UNICODE signs
#define XML_CR_UNI            16#0D//00
#define XML_LF_UNI            16#0A//00
#define XML_TABULATOR_UNI     16#09//00
#define XML_EQUAL_UNI         16#3D//00
#define XML_START_UNI         16#3C//00
#define XML_END_UNI           16#3E//00 
#define XML_SLASH_UNI         16#2F//00
#define XML_QUOTMARK_UNI      16#22//00
#define XML_SPACE_UNI         16#20//00
#define XML_EXCLAMATION_UNI   16#21//00
#define XML_APOSTROPHE_UNI    16#27//00
#define XML_HYPHEN_UNI        16#2D//00
#define XML_AMPERSAND_UNI     16#26//00
//-----------------------------------------

// defines for encoding-types -------------
#define ENCODING_UTF8          0
#define ENCODING_ISO8859_1     1
#define ENCODING_ISO8859_2     2
#define ENCODING_ISO8859_3     3
#define ENCODING_ISO8859_4     4
#define ENCODING_ISO8859_5     5
#define ENCODING_ISO8859_6     6
#define ENCODING_ISO8859_7     7
#define ENCODING_ISO8859_8     8
#define ENCODING_ISO8859_9     9
#define ENCODING_ISO8859_10   10
#define ENCODING_UTF16        11
//-----------------------------------------

// defines for encoding-strings -----------
#define STRING_UTF8         "UTF-8"
#define STRING_ISO8859_1    "ISO-8859-1"
#define STRING_ISO8859_2    "ISO-8859-2"
#define STRING_ISO8859_3    "ISO-8859-3"
#define STRING_ISO8859_4    "ISO-8859-4"
#define STRING_ISO8859_5    "ISO-8859-5"
#define STRING_ISO8859_6    "ISO-8859-6"
#define STRING_ISO8859_7    "ISO-8859-7"
#define STRING_ISO8859_8    "ISO-8859-8"
#define STRING_ISO8859_9    "ISO-8859-9"
#define STRING_ISO8859_10   "ISO-8859-10"
#define STRING_UTF16        "UTF-16"
//-----------------------------------------


// size defines wont be in here, maybe we will do everything dynamic 
// max file size will be a initializeable client of xmlWriter
// max Size defines -----------------------
#define XML_MAX_HEADER_LENGTH   254  // Signs // we will keep this define!
//-----------------------------------------

#define UNICODE_IDENTIFIER    16#FFFE 


// defines for converting of special characters -----
#define XML_CONVERT_AMPERSAND "&amp;"
#define XML_CONVERT_GREATER   "&gt;"
#define XML_CONVERT_LESS      "&lt;"
#define XML_CONVERT_QUOT      "&quot;"
#define XML_CONVERT_APOS      "&apos;"
#define XML_LENGTH_AMPERSAND  5
#define XML_LENGTH_GREATER    4
#define XML_LENGTH_LESS       4
#define XML_LENGTH_QUOT       6
#define XML_LENGTH_APOS       6
// --------------------------------------------------
