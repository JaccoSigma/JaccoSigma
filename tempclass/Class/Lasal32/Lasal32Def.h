
#define LSL_ONL_NO_ERROR                           0    // no error
#define LSL_ONL_NO_CIL_INTERFACE                  -1    // OS Interface not available ( OS_CILGET("LSLONL", #pLslOnl) )
#define LSL_ONL_NO_VALID_CONNECTION_ID            -2    // no connection available ( error at function ONL_OPEN_TCPIP or ONL_OPEN_CAN )
#define LSL_ONL_NO_VALID_CONNECTION_POINTER       -3    // NIL pointer at connection pointer detected
#define LSL_ONL_NO_VALID_OBJECTNAME_POINTER       -4    // NIL pointer at Obejectname pointer detected
#define LSL_ONL_NO_VALID_ADDRESS_POINTER          -5    // NIL pointer at Address pointer detected
#define LSL_ONL_NO_VALID_ADDRESS                  -6    // address is not valid
#define LSL_ONL_NO_VALID_DATA_POINTER             -7    // NIL pointer at Data pointer detected
#define LSL_ONL_NO_VALID_RESULT_POINTER           -8    // NIL pointer at Result pointer detected
#define LSL_ONL_NEWER_OS_VERSION_REQUIRED         -9    // OS - Version too old. OS >= 01.02.155 required
