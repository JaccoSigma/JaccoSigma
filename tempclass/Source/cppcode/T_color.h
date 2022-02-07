// select color mode

#ifdef COLOR256
 #undef COLOR256
#endif
//#define COLOR256

#ifdef BLACK
 #undef BLACK
#endif
#ifdef BLUE
 #undef BLUE
#endif
#ifdef GREEN
 #undef GREEN
#endif
#ifdef CYAN
 #undef CYAN
#endif
#ifdef RED
 #undef RED
#endif
#ifdef MAGENTA
 #undef MAGENTA
#endif
#ifdef BROWN
 #undef BROWN
#endif
#ifdef LIGHTGRAY
 #undef LIGHTGRAY
#endif
#ifdef DARKGRAY
 #undef DARKGRAY
#endif
#ifdef LIGHTBLUE
 #undef LIGHTBLUE
#endif
#ifdef LIGHTGREEN
 #undef LIGHTGREEN
#endif
#ifdef LIGHTCYAN
 #undef LIGHTCYAN
#endif
#ifdef LIGHTRED
 #undef LIGHTRED
#endif
#ifdef LIGHTMAGENTA
 #undef LIGHTMAGENTA
#endif
#ifdef YELLOW
 #undef YELLOW
#endif
#ifdef WHITE
 #undef WHITE
#endif

// hicolor
#ifndef COLOR256
 #define BLACK              0 //0x0000 //MakeTrueColor(  0,   0,   0)
 #define BLUE              21 //0x0015 //MakeTrueColor(  0,   0, 168)
 #define GREEN           1344 //0x0540 //MakeTrueColor(  0, 168,   0)
 #define CYAN            1365 //0x0555 //MakeTrueColor(  0, 168, 168)
 #define RED            43008 //0xA800 //MakeTrueColor(168,   0,   0)
 #define MAGENTA        43029 //0xA815 //MakeTrueColor(168,   0, 168)
 #define BROWN          43680 //0xAAA0 //MakeTrueColor(168,  84,   0)
 #define LIGHTGRAY      44373 //0xAD55 //MakeTrueColor(168, 168, 168)
 #define DARKGRAY       21162 //0x52AA //MakeTrueColor( 84,  84,  84)
 #define LIGHTBLUE      21183 //0x52BF //MakeTrueColor( 84,  84, 255)
 #define LIGHTGREEN     26602 //0x67EA //MakeTrueColor( 84, 255,  84)
 #define LIGHTCYAN      22527 //0x57FF //MakeTrueColor( 84, 255, 255)
 #define LIGHTRED       64170 //0xFAAA //MakeTrueColor(255,  84,  84)
 #define LIGHTMAGENTA   64191 //0xFABF //MakeTrueColor(255,  84, 255)
 #define YELLOW         65514 //0xFFEA //MakeTrueColor(255, 255,  84)
 #define WHITE          65535 //0xFFFF //MakeTrueColor(255, 255, 255)
#else
 #define BLACK              0 //0x0000 //MakeTrueColor(  0,   0,   0)
 #define BLUE               1 //0x0015 //MakeTrueColor(  0,   0, 168)
 #define GREEN              2 //0x0540 //MakeTrueColor(  0, 168,   0)
 #define CYAN               3 //0x0555 //MakeTrueColor(  0, 168, 168)
 #define RED                4 //0xA800 //MakeTrueColor(168,   0,   0)
 #define MAGENTA            5 //0xA815 //MakeTrueColor(168,   0, 168)
 #define BROWN              6 //0xAAA0 //MakeTrueColor(168,  84,   0)
 #define LIGHTGRAY          7 //0xAD55 //MakeTrueColor(168, 168, 168)
 #define DARKGRAY           8 //0x52AA //MakeTrueColor( 84,  84,  84)
 #define LIGHTBLUE          9 //0x52BF //MakeTrueColor( 84,  84, 255)
 #define LIGHTGREEN        10 //0x575A //MakeTrueColor( 84, 255,  84)
 #define LIGHTCYAN         11 //0x57FF //MakeTrueColor( 84, 255, 255)
 #define LIGHTRED          12 //0xFAAA //MakeTrueColor(255,  84,  84)
 #define LIGHTMAGENTA      13 //0xFABF //MakeTrueColor(255,  84, 255)
 #define YELLOW            14 //0xFFEA //MakeTrueColor(255, 255,  84)
 #define WHITE             15 //0xFFFF //MakeTrueColor(255, 255, 255)
#endif
