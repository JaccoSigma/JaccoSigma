#ifdef _CCOMPILER

typedef _UBYTE   PREFIX; 
typedef _UBYTE   INSTRSET; 
typedef _UDWORD  IPRSTATES; 

extern "C" void      Ini_Com_IF(void); 
extern "C" void      ComLink(void);
extern "C" void      ComlinkCyWork(void);		
extern "C" void      ComlinkInit(void);
extern "C" _UWORD    LOGIN(void*); 
#ifdef CANOPEN
extern "C" void      TXUPD(void*, void*, _SVRTYP);
#else
extern "C" void      TXUPD(void*, void*);
#endif
extern "C" void      StartStopRefresh(void*, _UWORD, _UWORD);
#ifdef LASAL_VERSION
extern "C" void      INSTALLCALLBACK(void*, void*);
#else
extern "C" void      INSTALLCALLBACK(void*);
#endif
extern "C" _DWORD    LDR_SetRs232ComlinkParams(_UDWORD, _UWORD, _DWORD, void*, void*);
extern "C" _UWORD    LOGOUT(void*); 



// INSTRSET
#define I_GET_OBJ            0  // name -> handle
#define I_GET_OBJ_CLS        1  // name -> handle and class name
#define I_READ_CLASS         2  // index -> class info
#define I_WRITE              3  // write to server
#define I_WRITE_TO_CLNT      4  // write to the server connected to a client
#define I_READ_OBJECT        5  // class header, index -> object info
#define I_READ_CHANNEL       6  // read the channel
#define I_READ_METHOD        7  // class header, index -> method info
#define I_READ               8  // handle -> read server channel
#define I_STARTPROG          9  // program no, label no -> start parallel
#define I_RUNPROG           10  // program no, label no -> start sequential
#define I_CMD               11  // handle, cmd, paras -> execute method
#define I_DELAY             12  // time -> wait till time elapsed
#define I_NOP               13  // ==== not used
#define I_LBL               14  // ==== not used
#define I_GOTO              15  // goto label
#define I_ENDPROG           16  // terminates the current ipr program
#define I_CALL              17  // subroutine call
#define I_RET               18  // return from subroutine
#define I_FUNCTION          19  // ======= not used
#define I_ENDFUNCTION       20  // ======= not used
#define I_INC               21  // handle -> increments data image
#define I_DEC               22  // handle -> decrements data image
#define I_START_LOAD_PR     23  // prepare OPsystem for new program to load
#define I_LOAD_PROG         24  // load new program code
#define I_COMMENT           25  // line is not executed
#define I_JMPIF             26  // jump if condition is true
#define I_WAITFOR           27  // wait until condition is true
#define I_SETFORTIME        28  // ======= not used
#define I_GETPROG           29  // load program code from the PLC
#define I_GETPROGSTATE      30  // program no -> state
#define I_CHECK_FOR_LOAD    31  // check if new progs are to load
#define I_GET_DESC_CRC      32  // CRC of all LASAL-descriptor lists in the PLC
#define I_READ_CONNECT      33  // get the client connection
#define I_GET_CLS           34  // get the class name from an object's address
#define I_GET_OBJ_NAME      35  // get object name from class name and object addr
#define I_READ_CLT          36  // call read method of connected server
#define I_STOPPROG          37  // stops a interpreter program
#define I_GET_PROGNR        38  // get programnumber of programname
#define I_GET_ACT_OFFSET    39  // get the actual offset of the program
#define I_GET_TRIGGER_COUNT 40  // get the count of the trigger
#define I_GET_VERSION       41  // returns the loader version
#define I_NEW_OBJ           42  // get the programname of the programnumber
#define I_CONNECT           43
#define I_SET_CYCLE         44
#define I_DEL               45
#define I_STOP_IPR_CHEK_FOR_LOAD 46 //stop all interpreter and checkforload
#define I_REGISTER   		    47	// for login and to inform the PLC that the channel is still used
#define I_RELEASE			      48	// release the communication channel at end of debug session
#define I_TRY_SOFTLOAD_IPR	49	// Try to load an program from the temporary memory in a soft mode
#define I_END_SOFTLOAD_IPR	50	// End the softload
#define I_GET_CFL_CYCLE 	  51	// Get the Cycle time of the CheckForLoad
#define I_GET_CALLED_IPR	  52	// Liefert die Nummer des Interpreters der von diesem Ipr aufgerufen wurde
#define I_GET_CALLED_FROM	  53	// Bringt die Nummer des Iprs von dem dieser Ipr aufgerufen wurde
#define I_INIT				      54	// calls the init-method of a server or an object
#define I_GET_CLS_BY_NAME	  55	// gets a class header pointer for a given class name
#define I_GET_NXT_DERIVED	  56	// get next derived class of a base class
#define I_LOCK              57  // lock communication buffer
#define I_UNLOCK            58  // unlock communcation buffer
#define I_LSLCMD            59
#define I_CMD_DEBUGIP       60  // gleich wie I_CMD, nur wird für die Serveradresse der this-Pointer (DebugIp Objekt) verwendet
#define I_CREATE_PROG       61  // ++pr:test
#define I_LOAD_PROG2        62  // wie I_LOAD_PROG jedoch ohne den Fehler mit der falschen Länge bei der Checksummenberechnung
#define I_START_IPR         63  // ++pr:test
#define I_GET_GLOBAL_ADDR   64
#define I_GET_DATA          65
#define I_GETPROGSTATE_ALL  66  // program state
#define I_SET_DATA          67
#define I_GET_STACKINFO     68
#define I_GET_OBJ_LIST      69
#define I_VISU_RDY          70
#define I_ILLEGAL           71



// PREFIX
#define P_IMMED     0 // data source is a constant
#define P_VARIA     1 // data source is a channel
#define P_SYS       2 // data source are system variables
#define P_POPEN     3 // open parenthesis
#define P_PCLOSE    4 // close parenthesis
#define P_ADD       5 // addition
#define P_SUB       6 // subtraction
#define P_COMMA     7 //
#define P_EOL       8 // end of line
#define P_EQ        9 // comparisons
#define P_NEQ      10
#define P_GT       11
#define P_GEQ      12
#define P_LT       13
#define P_LEQ      14
#define P_NOT      15 // binary negation
#define P_AND      16
#define P_OR       17
#define P_XOR      18
#define P_USER_STREAM 19
#define P_MUL      20
#define P_DIV      21
#define P_ILLEGAL  22

// IPRSTATES
#define DONE        0
#define BUSY        1
#define GO_ON       2
#define IPERROR     3
#define QUIT        4


#define	LC_INIT             0
#define	LC_RUNRAM           1
#define	LC_RUNROM           2
#define	LC_RUNTIME          3
#define	LC_READY            4
#define	LC_OK               5
#define	LC_UNKNOWN_CID      6
#define	LC_UNKNOWN_CONSTR   7
#define	LC_UNKNOWN_OBJECT   8
#define	LC_UNKNOWN_CHNL     9
#define	LC_WRONG_CONNECT    10
#define	LC_WRONG_ATTR       11
#define	LC_SYNTAX_ERROR     12
#define	LC_NO_FILE_OPEN     13
#define	LC_OUTOF_NEAR       14
#define	LC_OUTOF_FAR        15
#define	LC_INCOMPATIBLE     16
#define	LC_COMPATIBLE       17


// nur fuer test
//extern "C" void TESTCOM(void); 
//extern "C" void INIT_TESTCOM(void);

#endif

