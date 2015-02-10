/* Wlibtcod.CPP -- Copyright (c) 2014 Richard Tew
   Derived from WALLEGRO.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Contains the class definition and member functions for the
   libtcodTerm class, which is the only part of Incursion which
   contains libtcod-related calls. Includes 
   both the central screen functions, as well as wrapper functions
   for file management.

   Low-Level Read/Write
     void libtcodTerm::Update() 
     void libtcodTerm::CursorOn()
     void libtcodTerm::CursorOff()
     int16 libtcodTerm::CursorX()
     int16 libtcodTerm::CursorY()
     void libtcodTerm::Save()
     void libtcodTerm::Restore()
     void libtcodTerm::PutChar(Glyph g) 
     void libtcodTerm::APutChar(int16 x, int16 y, Glyph g)
     void libtcodTerm::PutChar(int16 x, int16 y, Glyph g) 
     void libtcodTerm::GotoXY(int16 x, int16 y) 
     void libtcodTerm::Clear() 
     void libtcodTerm::Color(uint16 _attr) 
     void libtcodTerm::StopWatch(int16 milli)  
   Input Functions
     int16 libtcodTerm::GetCharRaw()
     int16 libtcodTerm::GetCharCmd()
     int16 libtcodTerm::GetCharCmd(KeyCmdMode mode)
     bool  libtcodTerm::CheckEscape()
   General Functions
     void libtcodTerm::Initialize()
     void Fatal(const char*fmt,...)
     void Error(const char*fmt,...)
   Scroll Buffer
      void  libtcodTerm::SPutChar(int16 x, int16 y,Glyph g);
      //int16 libtcodTerm::SGetGlyph(int16 x, int16 y);
      void  libtcodTerm::SPutColor(int16 x, int16 y, uint16 col);
      uint16 libtcodTerm::SGetColor(int16 x, int16 y);
      void  libtcodTerm::ClearScroll(bool full=false);
      void  libtcodTerm::BlitScrollLine(int16 wn, int32 buffline, int32 winline);
   File I/O
     bool libtcodTerm::Exists(const char *fn)
     void libtcodTerm::Delete(const char *fn)
     bool libtcodTerm::Scour(const char *fn, bool Manditory)
     void libtcodTerm::OpenRead(const char*fn)
     void libtcodTerm::OpenWrite(const char*fn)
     void libtcodTerm::OpenUpdate(const char*fn)
     void libtcodTerm::Close()
     void libtcodTerm::FRead(void *vp,size_t sz)
     void libtcodTerm::FWrite(void *vp,size_t sz)
     int32 libtcodTerm::Tell()
     void libtcodTerm::Seek(int32 pos,int8 rel)
     void libtcodTerm::Cut(int32 amt)
    
*/

#ifdef LIBTCOD_TERM

#define _CRT_SECURE_NO_WARNINGS

#undef TRACE
#undef MIN
#undef MAX
#undef ERROR
#undef EV_BREAK

#include <direct.h>
#include <stdarg.h>
#include <ctype.h> 
#include <time.h>
#include <malloc.h>
#include <Windows.h>
#undef ERROR
#undef EV_BREAK
#undef MOUSE_MOVED

#ifndef DEBUG
#ifdef USE_CRASHPAD
/* Google Breakpad. */
#include "client/windows/handler/exception_handler.h"
#undef ERROR
#undef MIN
#undef MAX
#undef EV_BREAK
#endif
#endif

#include "Incursion.h"
#undef ERROR
#undef MIN
#undef MAX

#define TCOD_NOBASETYPES
#include "libtcod.h"
#undef ERROR
#undef MIN
#undef MAX


#define CURSOR_BLINK_MS 300
#define INPUT_IDLE_MS 50

/*

Add to release libs: exception_handler.lib;crash_generation_client.lib;common.lib
Add to release paths: _dependencies\google-breakpad\src\client\windows\Release\lib
Add to release includes: _dependencies\google-breakpad\src

#ifndef DEBUG
#ifdef USE_CRASHPAD
using google_breakpad::ExceptionHandler;
#endif
#endif
*/


TCOD_color_t RGBValues[MAX_COLOURS] = {
  {   0,   0,   0 },  // BLACK
  {   0,   0, 192 },  // BLUE
  {   0, 128,   0 },  // GREEN
  {   0, 128, 128 },  // CYAN
  { 128,   0,   0 },  // RED
  { 128,   0, 128 },  // PURPLE
  { 128, 128,   0 },  // BROWN
  { 192, 192, 192 },  // GREY
  { 128, 128, 128 },  // SHADOW
  {  64,  64, 255 },  // AZURE
  {   0, 255,   0 },  // EMERALD
  {   0, 255, 255 },  // SKYBLUE
  { 255,   0,   0 },  // "PINK"
  { 255,   0, 255 },  // MAGENTA
  { 255, 255,   0 },  // YELLOW
  { 255, 255, 255 },  // WHITE
  };


TCOD_color_t RGBSofter[MAX_COLOURS] = {
  { 0, 0, 0 }, // BLACK
  { 64, 64, 187 }, // BLUE
  { 0, 128, 0 }, // GREEN
  { 0, 128, 128 }, // CYAN
  { 192, 64, 48 }, // RED
  { 128, 0, 128 }, // PURPLE
  { 128, 128, 0 }, // BROWN
  { 192, 192, 192 }, // GREY
  { 128, 128, 128 }, // SHADOW
  { 85, 85, 230 }, // AZURE
  { 0, 230, 0 }, // EMERALD
  { 0, 230, 230 }, // SKYBLUE
  { 230, 0, 0 }, // "PINK"
  { 230, 0, 230 }, // MAGENTA
  { 230, 230, 0 }, // YELLOW
  { 230, 230, 230 }, // WHITE
  };


class libtcodTerm: public TextTerm
  {
    friend void Error(const char*fmt,...);
    friend void Fatal(const char*fmt,...);

    private:
      bool isWindowed, showCursor, cursorPulse;
      TCOD_console_t bScreen, bSave, bScroll, bCurrent;
      TCOD_color_t Colors[MAX_COLOURS];
      int16 resX, resY, fontX, fontY, ocx, ocy;
      uint32 ticks_blink_last;
      int oldResX, oldResY;
      String debugText;
      /* File I/O Stuff */
      String CurrentDirectory;
      String CurrentFileName; 
      FILE *fp;
      TCOD_list_t alf;
      int32 *alf_it;
    public:
      /* Low-Level Read/Write */
      virtual void Update();
      virtual void Redraw();
      virtual void CursorOn();
      virtual void CursorOff();
      virtual void BlinkCursor();
      virtual int16 CursorX() { return cx; }
      virtual int16 CursorY() { return cy; }
      virtual void Save();
      virtual void Restore();
      virtual void PutChar(Glyph g);
      virtual void APutChar(int16 x, int16 y, Glyph g);
      virtual void PutChar(int16 x, int16 y, Glyph g);
      virtual Glyph AGetChar(int16 x, int16 y);
      virtual void GotoXY(int16 x, int16 y);
      virtual void Clear();
      virtual void Color(uint16 _attr);
      virtual void StopWatch(int16 milli);
      virtual uint32 GetElapsedMilli();
	  virtual int32 ConvertChar(Glyph g, char **out); 

      /* Input Functions */
      virtual int16 GetCharRaw();
      virtual int16 GetCharCmd();
      virtual int16 GetCharCmd(KeyCmdMode mode);
      virtual bool CheckEscape();
      virtual void ClearKeyBuff();
      virtual void PrePrompt();
      
      /* General Functions */
      virtual void Initialize();
      virtual void ShutDown();
      virtual void Reset();
      virtual bool hideOption(int16 opt) { return false; }
      virtual void SetDebugText(const char *text);
      virtual void Title();
      
      /* Scroll Buffer */
      virtual void SPutChar(int16 x, int16 y, Glyph g);
      virtual uint16 SGetChar(int16 x, int16 y);
      virtual void SPutColor(int16 x, int16 y, uint16 col);
      virtual uint16 SGetColor(int16 x, int16 y);
      virtual void SClear();
      virtual void  BlitScrollLine(int16 wn, int32 buffline, int32 winline);
      
      /* System-Independant File I/O */
      virtual const char* SaveSubDir()    { return "save"; } 
      virtual const char* ModuleSubDir()  { return "mod"; } 
      virtual const char* LibraryPath() {
          char *envLibPath;
          static char s[MAX_PATH_LENGTH] = "";
          if (strlen(s) == 0) {
              envLibPath = getenv("INCURSIONLIBPATH");
              if (envLibPath != NULL) {
                  if (strcat(s, envLibPath)) {
                      if (s[strlen(s) - 1] == '\\')
                          s[strlen(s) - 1] = '\0';
                  }
              }
              if (strlen(s) == 0) {
                  strcat(s, IncursionDirectory);
                  strcat(s, "lib");
              }
          }
          return s;
      }
      virtual const char* LogSubDir()     { return "logs"; } 
      virtual const char* ManualSubDir()  { return "man"; } 
      virtual const char* OptionsSubDir() { return "."; } 
      virtual void ChangeDirectory(const char * c, bool set) { 
          if (set)
              CurrentDirectory = c;
          else {
              CurrentDirectory = (const char*)IncursionDirectory;
              if (CurrentDirectory != c)
                  CurrentDirectory += c;
          }
          if (chdir(CurrentDirectory))
              Fatal("Unable to locate directory '%s'.", (const char*)CurrentDirectory);
      }
      virtual bool Exists(const char* fn);
      virtual void Delete(const char* fn);
      virtual void OpenRead(const char* fn);
      virtual void OpenWrite(const char* fn);
      virtual void OpenUpdate(const char* fn);
      virtual void Close();
      virtual void FRead(void*,size_t sz);
      virtual void FWrite(const void*,size_t sz);
      virtual void Seek(int32,int8);
      virtual void Cut(int32);
      virtual bool FirstFile(char * filespec);
      virtual bool NextFile();
      virtual char * MenuListFiles(const char * filespec, 
          uint16 flags, const char *title);
      virtual const char* GetFileName()
        { return CurrentFileName; }      
      virtual int32 Tell();

      void SetIncursionDirectory(const char *s);
};
  
static int16 kbStandard[][3] = {
  { KY_HELP,   SHIFT, '?' },
  { KY_HELP,       0, '/' },
  { KY_COMMA,  SHIFT, '<' },
  { KY_COMMA,      0, ',' },
  { KY_PERIOD, SHIFT, '>' },
  { KY_PERIOD,     0, '.' },
  { KY_COLON,  SHIFT, ':' },
  { KY_COLON,      0, ';' },
  { KY_QUOTE,  SHIFT, '"' },
  { KY_QUOTE,      0, '\'' },
  { KY_MINUS,  SHIFT, '_' },
  { KY_MINUS,      0, '-' },
  { KY_EQUALS, SHIFT, '+' },
  { KY_EQUALS,     0, '=' },
  { 0,              0,  0  }  };
static int16 kbSpanish[][3] = {
  { 0x0C,       SHIFT, '?' },
  { 0x08,       SHIFT, '/' },
  { 0x56,           0, '<' },
  { 0x33,           0, ',' },
  { 0x56,       SHIFT, '>' },
  { 0x34,           0, '.' },
  { 0x34,       SHIFT, ':' },
  { 0x33,       SHIFT, ';' },
  { 0x03,       SHIFT, '"' },
  { 0x0C,           0, '\'' },
  { 0x35,       SHIFT, '_' },
  { 0x35,           0, '-' },
  { 0x1B,           0, '+' },
  { 0x0B,       SHIFT, '=' },
  { 0,              0,  0  }  };
static int16 kbNorwegian[][3] = {
  { 12,         SHIFT, '?' },
  { 95,             0, '/' },
  { 95,         SHIFT, '/' },
  { 45,             0, '<' },
  { 53,             0, ',' },
  { 45,         SHIFT, '>' },
  { 54,             0, '.' },
  { 54,         SHIFT, ':' },
  { 53,         SHIFT, ';' },
  { 3,          SHIFT, '"' },
  { 42,             0, '\'' },
  { 55,         SHIFT, '_' },
  { 55,             0, '-' },
  { 106,            0, '+' },
  { 11,         SHIFT, '=' },
  { 0,              0,  0  }  };
static int16 kbPolish[][3] = {
  { KY_HELP,   SHIFT, '?' },
  { KY_HELP,       0, '/' },
  { KY_COMMA,  SHIFT, '<' },
  { KY_COMMA,      0, ',' },
  { KY_PERIOD, SHIFT, '>' },
  { KY_PERIOD,     0, '.' },
  { KY_COLON,  SHIFT, ':' },
  { KY_COLON,      0, ';' },
  { KY_QUOTE,  SHIFT, '"' },
  { KY_QUOTE,      0, '\'' },
  { KY_MINUS,  SHIFT, '_' },
  { KY_MINUS,      0, '-' },
  { KY_EQUALS, SHIFT, '+' },
  { KY_EQUALS,     0, '=' },
  { 0,              0,  0  }  };

Term *T1;
libtcodTerm *AT1;
#ifndef DEBUG
#ifdef USE_CRASHPAD
ExceptionHandler* crashdumpHandler;
#endif
#endif

/*****************************************************************************\
*                                 libtcodTerm                                *
*                              main() Function                               *
\*****************************************************************************/

int main(int argc, char *argv[]) {
    /* This path code is currently present in libtcod and curses code. */
    char executablePath[MAX_PATH_LENGTH] = "";

    /* If run normally, check to see from which directory, and if there is one, use it. */
    if (!IsDebuggerPresent() && argc >= 1) {
        /* argv[0] is the filename and maybe the path before it, if the path is there grab it. */
        const char *str = strrchr(argv[0], '\\');
        if (str != NULL) {
            int16 n = str - argv[0]; /* Copy the separator too. */
            if (!strncpy(executablePath, argv[0], n))
                Error("Failed to locate Incursion directory for '%s'", argv[0]);
            executablePath[n] = '\0';
        }
    }

    /* If run under the debugger, or from within the current directory, or if
       something went wrong above, get and use the current directory path. */
    if (strlen(executablePath) == 0) {
        if (!_getcwd(executablePath, MAX_PATH_LENGTH))
            Error("Failed to locate Incursion directory under debugger (error 23)");
    }

    /* Google Breakpad is only compiled into Release builds, which get distributed.
     * Debug builds get the option to break out into the debugger, which makes it
     * superfluous in that case. */
#ifndef DEBUG
#ifdef USE_CRASHPAD
    std::wstring wsExecutablePath(strlen(executablePath), 0);
    mbstowcs(&wsExecutablePath[0],executablePath,strlen(executablePath));
    crashdumpHandler = new ExceptionHandler(wsExecutablePath,
                           NULL, /* &filter */
                           NULL /* &callback */,
                           NULL,
                           ExceptionHandler::HANDLER_ALL);
#endif
#endif
    theGame = new Game();
    {
        AT1 = new libtcodTerm;
        AT1->SetIncursionDirectory(executablePath);
        T1 = AT1;
        // ww: otherwise we segfault checking options
        T1->SetPlayer(NULL);
        T1->Initialize();
        //T1->Title();
    }
    theGame->StartMenu();
    T1->ShutDown();
    delete theGame;
    return 0;
}

TCOD_key_t readkey(int wait) {
    TCOD_key_t key;
    if (wait)
        TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS,&key,NULL,true);
    else {
        TCOD_key_t nokey = { TCODK_NONE, 0 };
        TCOD_event_t tcod_event = TCOD_sys_check_for_event(TCOD_EVENT_KEY_PRESS,&key,NULL);
        if ((tcod_event & TCOD_EVENT_KEY) == 0)
            return nokey;
    }
    return key;
}

int glyphchar_to_char(Glyph g) {
	static int lookup_table[GLYPH_LAST + 2];
	int c = GLYPH_ID_VALUE(g);
	int lookup_value;
	if (lookup_table[GLYPH_LAST + 1] == NULL) {
		/*
   00          01          02          03          04          05          06          07          08          09          0A          0B          0C          0D          0E          0F
00 ?           WhiteFace   BlackFace   Heart       Diamond     Club        Spade       ?           ?           WhiteCircle ?           Male        Female      EighthNote  BEighthNote WhiteSun
10 LPtr        RPtr        UDArrow     2Exclamat.. Pilcrow     SectionSign DoubleLow   DArrowBase  UArrow      DArrow      RArrow      LArrow      ?           LRArrow     UPtr        DPtr
20 ...
30 ...
40 ...
50 ...
60 ...
70                                                                                                                                                                                     House
   00          01          02          03          04          05          06          07          08          09          0A          0B          0C          0D          0E          0F
80
90
A0                                                                                                 Inverted?   Not         ReverseNot  1/2         1/4         Inverted!   <<          >>
B0 LightShade  MediumShade DarkShade   BoxV1       BoxV1L1     BoxV1L2     BoxV2L1     BoxD2L1     BoxD1L2     BoxV2L2     BoxV2       BoxD2L2     BoxU2L2     BoxU2L1     BoxU1L2     BoxD1L1
C0 BoxU1R1     BoxU1H      BoxD1H      BoxV1R1     BoxH1       BoxH1V1     BoxV1R2     BoxV2R1     BoxU2R2     BoxD2R2     BoxH2U2     BoxH2D2     BoxV2R2     BoxH2       BoxV2H2     BoxH2U1
D0 BoxH2U1     BoxH2D1     BoxH1D2     BoxU2R1     BoxU1R2     BoxD1R2     BoxD2R1     BoxV2H1     BoxV1H2     BoxUL       BoxDR       FullBlock   LowerBlock  LeftBlock   RightBlock  TopBlock
E0 Alpha       Beta        Gamma       Pi          SigmaL      SigmaS      MuS         GammaS      ShoL        ThetaL      OmegaL      DeltaS      Infinity    OStroke     Summation   Product
F0 IdenticalTo PlusMinus   GreaterThE  LessThanE   IntegralTp  IntegralBtm DivisionSgn AlmostEqual Degree      BlackSquare MiddleDot   SquareRoot  SuperScrptn SuperScrpt2 BlackSquare ...
   00          01          02          03          04          05          06          07          08          09          0A          0B          0C          0D          0E          0F
		*/

		// NULL entries just use the raw glyph character value for now.
		lookup_table[GLYPH_VLINE] = 179; // Box drawings light vertical.
		lookup_table[GLYPH_HLINE] = 196; // Box drawings light horizontal.
		lookup_table[GLYPH_DIVIDE] = 246;
		lookup_table[GLYPH_APPROXIMATELY] = 247; // Almost equal to.
		lookup_table[GLYPH_BLACK_SQUARE] = 254;
		lookup_table[GLYPH_CHECK] = 251;

		lookup_table[GLYPH_PERSON] = 6;
		lookup_table[GLYPH_BULK] = 197; // Box drawings vertical and horizonal
		lookup_table[GLYPH_WATER] = lookup_table[GLYPH_APPROXIMATELY];
		lookup_table[GLYPH_FOG] = '~';
		lookup_table[GLYPH_ICE] = lookup_table[GLYPH_BLACK_SQUARE];
		lookup_table[GLYPH_WEB] = '#';
		lookup_table[GLYPH_LAVA] = lookup_table[GLYPH_APPROXIMATELY];
		lookup_table[GLYPH_BLAST] = 15; // White sun
		lookup_table[GLYPH_WALL] = 177; // Medium shade
		lookup_table[GLYPH_ROCK] = 176; // Light shade
		lookup_table[GLYPH_SOLID] = 219; // Full block
		lookup_table[GLYPH_PILLAR1] = 9; // White circle
		lookup_table[GLYPH_PILLAR2] = 15; // White sun with rays.
		lookup_table[GLYPH_VIEWPOINT] = 9; // White circle.
		lookup_table[GLYPH_GRAVE] = 239; // Product symbol.
		lookup_table[GLYPH_SHELF] = 216; // Box drawings vertical single and horizontal double.

		lookup_table[GLYPH_POTION] = 173;
		lookup_table[GLYPH_SCROLL] = 168; // Inverted question mark.
		lookup_table[GLYPH_WEAPON] = lookup_table[GLYPH_CHECK];
		lookup_table[GLYPH_ROD] = '|';
		lookup_table[GLYPH_STAFF] = '/';
		lookup_table[GLYPH_WAND] = '-';
		lookup_table[GLYPH_FOOD] = '%';
		lookup_table[GLYPH_CORPSE] = '%';
		lookup_table[GLYPH_BOOK] = lookup_table[GLYPH_BLACK_SQUARE];
		lookup_table[GLYPH_TORCH] = '}';
		lookup_table[GLYPH_LARMOUR] = 228; // Greek capital letter sigma.
		lookup_table[GLYPH_MARMOUR] = 228; // Greek capital letter sigma.
		lookup_table[GLYPH_HARMOUR] = 228; // Greek capital letter sigma.
		lookup_table[GLYPH_SHIELD] = ']';
		lookup_table[GLYPH_GAUNTLETS] = 229; // Greek small letter sigma.
		lookup_table[GLYPH_HELMET] = 155; // Cent sign.
		lookup_table[GLYPH_HEADBAND] = 155; // Cent sign.
		lookup_table[GLYPH_BOOTS] = 224; // Greek small letter alpha.
		lookup_table[GLYPH_BRACERS] = 224; // Greek small letter alpha.
		lookup_table[GLYPH_GIRDLE] = 226; // Greek capital letter gamma.
		lookup_table[GLYPH_CLOTHES] = 20; // Pilcrow.
		lookup_table[GLYPH_CONTAIN] = 127; // House.
		lookup_table[GLYPH_CROWN] = '^';
		lookup_table[GLYPH_DUST] = 240; // Identical to.
		lookup_table[GLYPH_DECK] = 240; // Identical to.
		lookup_table[GLYPH_RING] = 235; // Greek small letter delta.
		lookup_table[GLYPH_AMULET] = 11; // Male sign.
		lookup_table[GLYPH_FIGURE] = 156; // Pound sign.
		lookup_table[GLYPH_HORN] = '&';
		lookup_table[GLYPH_EYES] = 236; // Infinity.
		lookup_table[GLYPH_HERB] = '"';
		lookup_table[GLYPH_MUSH] = 227; // Greek small letter pi.
		lookup_table[GLYPH_GEM] = 4; // Diamond.
		lookup_table[GLYPH_COIN] = '$';
		lookup_table[GLYPH_TOOL] = 234; // Greek capital letter omega.
		lookup_table[GLYPH_SWORD] = '(';
		lookup_table[GLYPH_BOW] = ')';
		lookup_table[GLYPH_JUNK] = '&';
		lookup_table[GLYPH_CHEST] = 127; // House.
		lookup_table[GLYPH_CLOAK] = 6; // Hearts suit symbol.
		lookup_table[GLYPH_STATUE] = 5; // Spades suit symbol.
		lookup_table[GLYPH_PILE] = '*';
		lookup_table[GLYPH_MULTI] = 146; // Latin capital letter Ae.
		lookup_table[GLYPH_ALTAR] = '8';
		lookup_table[GLYPH_FOUNTAIN] = 244; // Integral top.
		lookup_table[GLYPH_THRONE] = 190; // Box drawings up single and left double.
		lookup_table[GLYPH_SYMBOL] = 237; // Capital O with stroke.

		lookup_table[GLYPH_HEDGE] = '"';
		lookup_table[GLYPH_RUBBLE] = ':';
		lookup_table[GLYPH_BRIDGE] = '=';
		lookup_table[GLYPH_FLOOR] = 250; // Middle dot.
		lookup_table[GLYPH_FLOOR2] = 249; // Black small square.
		lookup_table[GLYPH_VDOOR] = lookup_table[GLYPH_VLINE];
		lookup_table[GLYPH_HDOOR] = lookup_table[GLYPH_HLINE];
		lookup_table[GLYPH_ODOOR] = '+';
		lookup_table[GLYPH_BDOOR] = 241; // Plus-minus.
		lookup_table[GLYPH_PIT] = '0';
		lookup_table[GLYPH_PORTAL] = 240; // Identical to.
		lookup_table[GLYPH_SUMMONING_CIRCLE] = 233; // Greek capital letter theta.

		lookup_table[GLYPH_STORE] = 21; // Section sign.
		lookup_table[GLYPH_GUILD] = 20; // Pilcrow.
		lookup_table[GLYPH_STORE_VWALL] = 186; // Box drawings double vertical.
		lookup_table[GLYPH_STORE_HWALL] = 205; // Box drawings double horizontal.
		lookup_table[GLYPH_STORE_CORNER] = 219; // Full block.

		lookup_table[GLYPH_USTAIRS] = '<';
		lookup_table[GLYPH_DSTAIRS] = '>';
		lookup_table[GLYPH_TREE] = 157; // Yen sign.
		lookup_table[GLYPH_FURNATURE] = lookup_table[GLYPH_BLACK_SQUARE];
		lookup_table[GLYPH_UNKNOWN] = '?';
		lookup_table[GLYPH_TRASH] = '&';
		lookup_table[GLYPH_BONES] = '&';
		lookup_table[GLYPH_CHECKMARK] = lookup_table[GLYPH_CHECK];

		lookup_table[GLYPH_TRAP] = 232; // Greek capital letter sho.
		lookup_table[GLYPH_DISARMED] = lookup_table[GLYPH_DIVIDE];

		lookup_table[GLYPH_ARROW] = 24; // Upwards arrow.
		lookup_table[GLYPH_ARROW_UP] = 24; // Black up pointing arrow.
		lookup_table[GLYPH_ARROW_DOWN] = 25; // Black down pointing arrow.
		lookup_table[GLYPH_ARROW_RIGHT] = 26; // Black right pointing arrow.
		lookup_table[GLYPH_ARROW_LEFT] = 27; // Black left pointing arrow.

		lookup_table[GLYPH_POINTER_LEFT] = 17; // Black left pointing pointer.
		lookup_table[GLYPH_POINTER_RIGHT] = 16; // Black right pointing pointer.

		lookup_table[GLYPH_GUARD] = 140; // latin small letter I with circumflex.
		lookup_table[GLYPH_TOWNIE] = 139; // Latin small letter I with diaeresis.
		lookup_table[GLYPH_TOWNSCUM] = 141; // Latin small letter I with grave.
		lookup_table[GLYPH_TOWN_NPC] = 'i';
		lookup_table[GLYPH_LDEMON] = 151; // // Latin small letter U with grave.
		lookup_table[GLYPH_GDEMON] = 154; // Latin small letter U with diaeresis.
		lookup_table[GLYPH_LDEVIL] = 148; // Latin small letter O with diaeresis.
		lookup_table[GLYPH_GDEVIL] = 153; // Latin capital letter O with diaeresis.
		lookup_table[GLYPH_FIEND] = 152; // Latin small letter y with diaeresis.

		lookup_table[GLYPH_PLAYER] = '@';
		lookup_table[GLYPH_HUMAN] = 1; // White smiling face.
		lookup_table[GLYPH_ELF] = 132; // CUSTOM face graphic.
		lookup_table[GLYPH_DWARF] = 131; // CUSTOM face graphic.
		lookup_table[GLYPH_GNOME] = 133; // CUSTOM small face graphic.
		lookup_table[GLYPH_HOBBIT] = 133; // CUSTOM small face graphic.

		lookup_table[GLYPH_LAST + 1] = 1;
	}
	lookup_value = lookup_table[c];
	if (lookup_value == NULL)
		return c;
	return lookup_value;
}

  
/*****************************************************************************\
*                                 libtcodTerm                                   *
*                           Low-Level Read/Write                              *
\*****************************************************************************/

void libtcodTerm::SetDebugText(const char *text) {
    debugText = text;
}

void libtcodTerm::Update() {
    static int update_count = 0;
    TCOD_console_blit(bScreen,0,0,0,0,NULL,0,0,1.0f,1.0f);
    TCOD_console_blit(bScreen,0,0,0,0,bCurrent,0,0,1.0f,1.0f);
    if (debugText.GetLength()) {
        update_count++;
        TCOD_console_print(NULL,0,0,"%d: %s",update_count,(const char *)debugText);
        debugText = "";
    }
    TCOD_console_flush();

    updated = true;
}
  
void libtcodTerm::Redraw() {
    Update();
}

void libtcodTerm::CursorOn() {
    showCursor = true;
    ticks_blink_last = TCOD_sys_elapsed_milli();
    ocx = ocy = 0;
}

void libtcodTerm::CursorOff() {
    showCursor = false;
    cursorPulse = false;
    TCOD_console_blit(bCurrent,ocx,ocy,1,1,NULL,ocx,ocy,1.0f,1.0f);
    ocx = ocy = 0;
}
  
void libtcodTerm::BlinkCursor() {
    // Sometimes the windows may leave the cursor off the RHS.
    if (cx >= sizeX)
        return;
    cursorPulse = !cursorPulse;
    if (cursorPulse) {
        uint16 c = attr & COLOUR_MASK;
        if (c == SHADOW) 
          c = GREY;
        else if (c < 8)
          c += 8;
        TCOD_console_put_char_ex(NULL,cx,cy,'X',Colors[c], Colors[c]);
        ocy = cy; ocx = cx;
    } else
        TCOD_console_blit(bScreen,ocx,ocy,1,1,NULL,ocx,ocy,1.0f,1.0f);
    /* Won't get an update necessarily otherwise */
    TCOD_console_flush();
}
  
void libtcodTerm::Save() { 
    Update();
    TCOD_console_blit(bScreen,0,0,0,0,bSave,0,0,1.0f,1.0f);
}

void libtcodTerm::Restore() { 
    TCOD_console_blit(bSave,0,0,0,0,bScreen,0,0,1.0f,1.0f);
    Update();    
}

void libtcodTerm::PutChar(Glyph g) {
	APutChar(cx++, cy, g);
}

void libtcodTerm::APutChar(int16 x, int16 y, Glyph g) {
	int c = glyphchar_to_char(g);
	uint16 fg_idx = GLYPH_FORE_VALUE(g);
	uint16 bg_idx = GLYPH_BACK_VALUE(g);

	if (fg_idx == 0 && bg_idx == 0) {
		fg_idx = attr & COLOUR_MASK;
		bg_idx = (attr >> COLOUR_BITS) & COLOUR_MASK;
	}
	if (c >= 255)
		Error("Bad character code encountered");
	else
	    TCOD_console_put_char_ex(bScreen, x, y, c, Colors[fg_idx], Colors[bg_idx]);

	updated = false;
}

void libtcodTerm::PutChar(int16 x, int16 y, Glyph g) {
    x += activeWin->Left;
    y += activeWin->Top;
    APutChar(x,y,g);
}

Glyph libtcodTerm::AGetChar(int16 x, int16 y) {
	int16 i;
	// TODO: This will be the screen character.  In an ideal world, it would be translated back
	//       to the GLYPH_ID value.
	Glyph g = TCOD_console_get_char(bScreen, x, y);
	TCOD_color_t fgcolor, bgcolor;

	fgcolor = TCOD_console_get_char_foreground(bScreen, x, y);
	for (i = 0; i < MAX_COLOURS; i++)
		if (Colors[i].r == fgcolor.r && Colors[i].g == fgcolor.g && Colors[i].b == fgcolor.b) {
			g |= GLYPH_FORE(i);
			break;
		}

	bgcolor = TCOD_console_get_char_background(bScreen, x, y);
	for (i = 0; i < MAX_COLOURS; i++)
		if (Colors[i].r == bgcolor.r && Colors[i].g == bgcolor.g && Colors[i].b == bgcolor.b) {
			g |= GLYPH_BACK(i);
			break;
		}

	return g;
}

void libtcodTerm::GotoXY(int16 x, int16 y) 
  {
    cx = x + activeWin->Left;
    cy = y + activeWin->Top;
    //if (!Cursor.bVisible)
    //  return;
    //crd.X = cx; crd.Y = cy;
    //SetConsoleCursorPosition(hStdOut, crd); 
  }

void libtcodTerm::Clear() {
    TCOD_console_rect(bScreen,activeWin->Left,activeWin->Top,(activeWin->Right+1)-activeWin->Left,(activeWin->Bottom+1)-activeWin->Top,1,TCOD_BKGND_SET);
    cWrap = 0; cx = cy = 0;
    if (activeWin == &Windows[WIN_SCREEN] || activeWin == &Windows[WIN_MESSAGE])
        linepos = linenum = 0;
    updated = false;
}

void libtcodTerm::Color(uint16 _attr) {
    attr = _attr;
}

void libtcodTerm::StopWatch(int16 milli) {    
    switch (p ? p->Opt(OPT_ANIMATION) : 0) {
    case 0: TCOD_sys_sleep_milli(milli); return;
    case 1: TCOD_sys_sleep_milli((milli+3)/4); return; 
    case 2: return;
    }
}

uint32 libtcodTerm::GetElapsedMilli() {
    return TCOD_sys_elapsed_milli();
}

int32 libtcodTerm::ConvertChar(Glyph g, char **out) {
	return 0;
}

/*****************************************************************************\
*                                 libtcodTerm                                   *
*                             General Functions                               *
\*****************************************************************************/


void Fatal(const char*fmt,...) {
	va_list argptr; 
	va_start(argptr, fmt);
	if (!T1)
		{ printf(__buffer); exit(1); }
	T1->Clear();
	vsprintf(__buffer, fmt, argptr);
	va_end(argptr);
	sprintf(__buff2, "Fatal Error: %s\nPress [ENTER] to exit...",__buffer);
	((libtcodTerm*)T1)->Box(WIN_SCREEN,BOX_NOPAUSE|BOX_NOSAVE,RED,PINK,__buff2);
	T1->Update();
	readkey(1);
#ifdef DEBUG
	BREAKOUT;
#endif
	T1->ShutDown();
	exit(1);
}

void Error(const char*fmt,...) {
	va_list argptr;
	va_start(argptr, fmt); 
	char ch;
	vsprintf(__buffer, fmt, argptr);
	if (!T1)
		{ printf(__buffer); exit(1); }

#ifdef DEBUG
	sprintf(__buff2, "Error: %s\n[B]reak, [E]xit or [C]ontinue?",__buffer);
#else
#ifdef USE_CRASHPAD
	crashdumpHandler->WriteMinidump();
	sprintf(__buff2, "Error: %s\n[E]xit or [C]ontinue?",__buffer);
#endif
#endif
	((libtcodTerm*)T1)->Save();
	((libtcodTerm*)T1)->Box(WIN_SCREEN,BOX_NOPAUSE|BOX_NOSAVE,RED,PINK,__buff2);
	T1->Update();

	do 
	ch = toupper(readkey(1).c & 0xFF);
	while (ch != 'C' && ch != 'E' && ch != 'S'    
#ifdef DEBUG
		&& ch != 'B'
#endif
		);
	if (ch == 'E')
		{ T1->ShutDown(); exit(1); }
	if (ch == 'C')
		return;
#ifdef DEBUG
	BREAKOUT;
#endif

	/*
	else if (ch == 'S')
	theGame->PanicSave();
	*/
	((libtcodTerm*)T1)->Restore();
	va_end(argptr);
}

void libtcodTerm::Reset() {
    int16 optRes, optFont, i;
    char *envFontPath;
    char fontName[MAX_PATH_LENGTH] = "";
    bool scaled_res, scaled_font;
    int res_w, res_h;

    if (bScreen != NULL) {
        TCOD_console_delete(bScreen);
        TCOD_console_delete(bCurrent);
        TCOD_console_delete(bSave);
        TCOD_console_delete(NULL);
    }

    scaled_res = false;
    scaled_font = false;

    if (isWindowed) 
      { optRes  = theGame->Opt(OPT_WIND_RES);
        optFont = theGame->Opt(OPT_WIND_FONT); }
    else
      { optRes  = theGame->Opt(OPT_FULL_RES);
        optFont = theGame->Opt(OPT_FULL_FONT); }

	TCOD_sys_get_current_resolution(&res_w,&res_h);

RetryRes:    
    switch(optRes) {
    case 0:  resX = 640;  resY = 480;  break;
    case 1:  resX = 800;  resY = 600;  break;
    case 2:  resX = 1024; resY = 768;  break;
    case 3:  resX = 1280; resY = 960; break;
    case 4:  resX = 1280; resY = 1024; break;
    case 5:  resX = 1400; resY = 1050; break;
    case 6:  resX = 1600; resY = 1200; break;
    case 7:  resX = 1280; resY = 768;  break;
    case 8:  resX = 1280; resY = 800;  break;
    case 9:  resX = 1920; resY = 1080; break;     
    case 10: resX = 1920; resY = 1200; break;
    }

    while (resX > res_w || resY > res_h) {
        if (optRes)
            { optRes--; scaled_res = true; goto RetryRes; }
        printf("Incursion cannot initialize graphics and thus must terminate.");
        exit(1);
    }

RetryFont:
    switch(optFont) {
    case 0: fontX = 8;  fontY = 8;  
        break;
    case 1: fontX = 12; fontY = 16; 
        break;
    case 2: fontX = 16; fontY = 12; 
        break;
    case 3: fontX = 16; fontY = 16; 
        break;
    }
    
    sizeX = resX / fontX;
    sizeY = resY / fontY;
   
    if (sizeX < 80 || sizeY < 48)
        { optFont = 0; scaled_font = true; goto RetryFont; }

    /* The VS debugger finds it handy to run things where they get generated in sub-directories. */
    i = 0;
    envFontPath = getenv("INCURSIONFONTPATH");
    if (envFontPath != NULL) {
        if (strcat(fontName, envFontPath)) {
            if (fontName[strlen(fontName) - 1] != '\\')
                strcat(fontName, "\\");
        }
    }
    if (strlen(fontName) == 0) {
        strcat(fontName, IncursionDirectory);
        strcat(fontName, "fonts\\");
    }
    i = strlen(fontName);

    sprintf(fontName + i, "%dx%d.png", fontX, fontY);
	TCOD_console_set_custom_font(fontName, TCOD_FONT_LAYOUT_ASCII_INROW, 16, 16);
	TCOD_console_init_root(sizeX, sizeY, "Incursion: Halls of the Goblin King", !isWindowed, TCOD_RENDERER_SDL);

    InitWindows();

    if (theGame->Opt(OPT_SOFT_PALETTE))
        for (i=0;i!=MAX_COLOURS;i++)
            Colors[i] = RGBSofter[i];
    else
        for (i=0;i!=MAX_COLOURS;i++)
            Colors[i] = RGBValues[i];

    bScreen = TCOD_console_new(sizeX, sizeY);
    TCOD_console_set_default_background(bScreen, TCOD_black);
    TCOD_console_set_default_foreground(bScreen, TCOD_white);
    TCOD_console_clear(bScreen);
    bCurrent = TCOD_console_new(sizeX, sizeY);
    TCOD_console_set_default_background(bCurrent, TCOD_black);
    TCOD_console_set_default_foreground(bCurrent, TCOD_white);
    TCOD_console_clear(bCurrent);
    bSave = TCOD_console_new(sizeX, sizeY);
    TCOD_console_set_default_background(bSave, TCOD_black);
    TCOD_console_set_default_foreground(bSave, TCOD_white);
    TCOD_console_clear(bSave);

    if (bScroll == NULL) {
        bScroll  = TCOD_console_new(SCROLL_WIDTH, MAX_SCROLL_LINES);
        TCOD_console_set_default_background(bScroll, TCOD_black);
        TCOD_console_clear(bScroll);
    }

    if (Mode == MO_PLAY) { 
        ShowStatus();
        ShowTraits();
        ShowMap();
    } else if (Mode == MO_INV) {
        ShowStatus();
        ShowTraits();
        InvShowSlots();
    }

    Update();
    ocx = ocy = 0;
    if (scaled_res) {
        Box(WIN_SCREEN,0,PINK,GREY,
            "The screen resolution or window size you have selected in the "
            "Option Manager could not be set on your current hardware. As a "
            "result, the highest available resolution supported by Incursion "
            "was used instead. This may have also caused the font size to "
            "default back to 8x8, if whatever font size you have selected "
            "was too large to fit 80x48 characters into the available resolution."
            "\n__To change the full-screen resolution, window size and font "
            "size, select 'Change System Options' from the startup menu, "
            "navigate to the Output Options option group and set the options "
            "at the bottom of the screen accordingly.");
    } else if (scaled_font) {
        Box(WIN_SCREEN,0,PINK,GREY,
            "The screen resolution or window size you have selected in the "
            "Option Manager is too small to display a full 80x48 text grid "
            "(at least 80 columns and 48 rows of characters) with the font "
            "size you have selected. As a result, the font has been defaulted "
            "back to 8x8. If you find the display difficult to read as a "
            "result, you must choose a combination of resolution and font "
            "size that allows at least 80x48 characters."
            "\n__To change the full-screen resolution, window size and font "
            "size, select 'Change System Options' from the startup menu, "
            "navigate to the Output Options option group and set the options "
            "at the bottom of the screen accordingly.");
    }      
}    


void libtcodTerm::Initialize() {
    p = NULL; m = NULL; isHelp = false;
    ActionsSinceLastAutoSave = 0;
    cx = cy = 0;
    showCursor = false;
    cursorPulse = false;
    OptionCount = 0;
    OffscreenC = 0;
    QueuedChar = 0;
    ControlKeys = 0;
    WViewListCount = 0; 
    OptionsShown = false;
    XOff = YOff = 0;
    s1 = s2 = 0;      
    MsgHistory[0] = 0;
    Mode = MO_SPLASH;
    isWindowed = true;
    alf = NULL;

    bScreen = bCurrent = bSave = bScroll = NULL;
    
	TCOD_sys_startup();

	Reset();
    
    InitWindows();   

    SetWin(WIN_SCREEN);
    Color(14);
}

void libtcodTerm::ShutDown() {
    SetWin(WIN_SCREEN);
    Clear();
}

void libtcodTerm::SetIncursionDirectory(const char *s) {
    char tmp[MAX_PATH_LENGTH] = "";
    IncursionDirectory = (const char *)s;
    IncursionDirectory += "\\";

    strcpy(tmp, s);
    strcat(tmp, "\\mod");
    _mkdir(tmp);

    strcpy(tmp, s);
    strcat(tmp, "\\logs");
    _mkdir(tmp);

    strcpy(tmp, s);
    strcat(tmp, "\\save");
    _mkdir(tmp);
}

/* Draw the intro screen header and display the start of game libtcod-specific
 * stuff in the footer area below it. */
void libtcodTerm::Title() {
    uint8 done_rendering = 0;
    TCOD_key_t key = { TCODK_NONE, 0 };
    /*
    int credits_x, credits_y;
    TCOD_color_t colour_fg, colour_bg;*/
    static bool first_time = false;
    char *next_page = "Press a key to skip this page";

    // Draw the intro screen header.
    TextTerm::Title();
/*
    if (first_time == false) {
        int16 body_y = WinSizeY()/4-1;
        first_time = true;

        // At this point the selected window should be the footer area, which
        // TextTerm::Title() defined for it (and it should be cleared).
        // Show donation text.
        Write(0, body_y,
            "    > Support development.\n"
            "    > Encourage work on elements of interest to you.\n"
            "    > Read 'Incursion.txt' for details.");

        Write((WinSizeX() - strlen(next_page))/2, body_y + 5, next_page);

        // Render the libtcod credits then move to the menu.
        credits_x = WinLeft() + (WinSizeX()*3)/4;
        credits_y = WinTop() + (WinSizeY()*1)/4;
	    colour_fg = TCOD_console_get_default_foreground(bScreen);
	    colour_bg = TCOD_console_get_default_background(bScreen);
        while (!done_rendering && !TCOD_console_is_window_closed() && key.vk == TCODK_NONE) {
            key = readkey(0);
            done_rendering = TCOD_console_credits_render_to(bScreen,credits_x,credits_y,0);
            Update();
        }

        // The credits restores the colour when it is done, but a key press exits before that.
	    TCOD_console_set_default_foreground(bScreen, colour_fg);
	    TCOD_console_set_default_background(bScreen, colour_bg);
        // Clear the footer area.
        Clear();
    }
    */
}

/*****************************************************************************\
*                                 libtcodTerm                                  *
*                               Scroll Buffer                                *
\*****************************************************************************/

void  libtcodTerm::SPutChar(int16 x, int16 y, Glyph g) {
	int c = glyphchar_to_char(g);
	TCOD_console_put_char_ex(bScroll, x, y, c, Colors[GLYPH_FORE_VALUE(g)], Colors[GLYPH_BACK_VALUE(g)]);
}

uint16 libtcodTerm::SGetChar(int16 x, int16 y) {
	return TCOD_console_get_char(bScroll, x, y);
}

void libtcodTerm::SPutColor(int16 x, int16 y, uint16 col) {
    uint32 ga = GLYPH_ATTR(col);
    uint8 c = TCOD_console_get_char(bScroll,x,y);
	TCOD_console_put_char_ex(bScroll, x, y, c, Colors[GLYPH_FORE_VALUE(ga)], Colors[GLYPH_BACK_VALUE(ga)]);
}

uint16 libtcodTerm::SGetColor(int16 x, int16 y) {
    uint32 i, colour = 0;
    TCOD_color_t fgcolour, bgcolour;
	uint8 c = TCOD_console_get_char(bScroll, x, y);

    fgcolour = TCOD_console_get_char_foreground(bScroll,x,y);
    for (i=0;i<MAX_COLOURS;i++)
		if (Colors[i].r == fgcolour.r && Colors[i].g == fgcolour.g && Colors[i].b == fgcolour.b) {
			colour |= GLYPH_FORE(i);
            break;
        }

    bgcolour = TCOD_console_get_char_background(bScroll,x,y);
    for (i=0;i<MAX_COLOURS;i++)
		if (Colors[i].r == bgcolour.r && Colors[i].g == bgcolour.g && Colors[i].b == bgcolour.b) {
			colour |= GLYPH_BACK(i);
			break;
        }

    return (uint16)GLYPH_COLOUR_VALUE(colour);
}

void libtcodTerm::SClear() {
    TCOD_console_clear(bScroll);
}

void  libtcodTerm::BlitScrollLine(int16 wn, int32 buffline, int32 winline) {
    TCOD_console_blit(bScroll,0,buffline,min(SCROLL_WIDTH,WinSizeX()),1,bScreen,Windows[wn].Left,winline+Windows[wn].Top,1.0f,1.0f);
}

/*****************************************************************************\
*                                 libtcodTerm                                   *
*                              Input Functions                                *
\*****************************************************************************/

int16 libtcodTerm::GetCharRaw()
  { return GetCharCmd(KY_CMD_RAW); }

int16 libtcodTerm::GetCharCmd()
  { return GetCharCmd(KY_CMD_NORMAL_MODE); }

int16 libtcodTerm::GetCharCmd(KeyCmdMode mode) {
    KeySetItem * keyset = theGame->Opt(OPT_ROGUELIKE) ? RoguelikeKeySet : StandardKeySet;
    int16 keyset_start = 0, keyset_delta = 0, keyset_last;
    int16 i, ox, oy, ch;
    TextWin *wn;
	uint32 ticks_last;

    ActionsSinceLastAutoSave++;
    if (p) p->GameTimeInfo.keystrokes++;

    if (QueuedChar) {
        ch = QueuedChar;
        QueuedChar = 0;
        return ch;
    }
      
    for (keyset_last = 0; keyset[keyset_last].cmd != KY_CMD_LAST; keyset_last++)
        ;
    keyset_last--;
      
    switch (mode) {
    case KY_CMD_NORMAL_MODE: 
        keyset_start = keyset_last ; 
        keyset_delta = -1 ; 
        break; 
    case KY_CMD_TARGET_MODE: 
        keyset_start = 0; 
        keyset_delta = +1; 
        break; 
    case KY_CMD_ARROW_MODE:
        keyset_start = 0; 
        keyset_delta = +1;
        break; 
    } 
      
    ox = cx; oy = cy; wn = activeWin;
    if (theGame->InPlay()) {
        //ShowViewList(); 
        if (p->UpdateMap && Mode == MO_PLAY) {
            RefreshMap();
        }
        if (p->statiChanged) {
            ShowTraits();
            ShowStatus();
        }
    }

    Update();
    activeWin = wn;
    cx = ox; cy = oy;
    ticks_last = TCOD_sys_elapsed_milli();

    for(;;) {
		uint32 ticks0 = TCOD_sys_elapsed_milli(), ticks1;
        TCOD_key_t tcodKey;
        TCOD_event_t tcodEvent = TCOD_sys_check_for_event(TCOD_EVENT_KEY_PRESS, &tcodKey, NULL);

        if (Mode == MO_PLAY && p->UpdateMap)
            RefreshMap();

        if (Mode == MO_PLAY || Mode == MO_INV)
            if (p->statiChanged) {
                ShowTraits();
                ShowStatus();
            }
          
        ticks1 = TCOD_sys_elapsed_milli();
        if (showCursor && ticks1 > ticks_blink_last + CURSOR_BLINK_MS) {
            BlinkCursor();
            ticks_blink_last = ticks1;
        }
          
        if (TCOD_console_is_window_closed()) {
            int16 ox, oy;

CtrlBreak:
            ox = cx; oy = cy;
            if (!theGame || !theGame->InPlay()) {
                ShutDown();
                exit(0);
            }
            if (GetMode() == MO_PLAY && mode == KY_CMD_NORMAL_MODE) {
                theGame->SetQuitFlag();
                return KY_CMD_QUICK_QUIT;
            }
        }

        if (tcodKey.vk == TCODK_NONE) {
            uint32 ticks_next = ticks1 + INPUT_IDLE_MS;
            if (showCursor && ticks_next > ticks_blink_last + CURSOR_BLINK_MS)
                ticks_next = ticks_blink_last + CURSOR_BLINK_MS;
            if (ticks_next > ticks1)
                TCOD_sys_sleep_milli(ticks_next - ticks1);

            /* Allow clearing the message window ONLY if we aren't
               repeating keys, i.e., holding down arrow to run. In
               other words, if no keys are down, or keys are down 
               that weren't down earlier, it is safe to clear. */
            ClearMsgOK = true;
            continue;
        }
        
        if (theGame->Opt(OPT_CLEAR_EVERY_TURN) && ClearMsgOK && mode == KY_CMD_NORMAL_MODE && GetMode() == MO_PLAY) {
            ox = cx; oy = cy; wn = activeWin;
            linenum = linepos = 0;
            SetWin(WIN_MESSAGE);
            Clear();
            activeWin = wn;
            cx = ox; cy = oy;
            ClearMsgOK = false;
        }
        
        ticks_blink_last = TCOD_sys_elapsed_milli();

        if (tcodKey.vk == TCODK_TEXT) {
            ch = tcodKey.text[0];
        } else {
            ch = tcodKey.c;

            ControlKeys = 0;
            if (tcodKey.lctrl || tcodKey.rctrl)
                ControlKeys |= CONTROL;
            if (tcodKey.shift)
                ControlKeys |= SHIFT;
            if (tcodKey.lalt || tcodKey.ralt)
                ControlKeys |= ALT; 

            if (tcodKey.vk == TCODK_ENTER)
                if (ControlKeys & ALT) {
                    isWindowed = !isWindowed;
                    Reset();
                    Update();
                    return KY_REDRAW;
                }

            if ((ch == 'c' || tcodKey.vk == TCODK_PAUSE) && (ControlKeys & CONTROL))
                goto CtrlBreak;

#ifdef HACK_KEY_DEBUGGING
            {
                static bool debug_keys = false;
                if ((ch == 'd') && (ControlKeys & CONTROL)) {
                    debug_keys = !debug_keys;
                    if (debug_keys)
                        T1->SetDebugText("Key debugging started.");
                    else
                        T1->SetDebugText("Key debugging stopped.");
                } else if (debug_keys) {
                    char formatted[256];
                    if (tcodKey.vk != TCODK_CONTROL) {
                        int result = sprintf(formatted, "Key code: %d, Char-number: %d Char-letter: '%c' CtrlKeys: %x", tcodKey.vk, tcodKey.c, tcodKey.c, ControlKeys);
                        if (result != -1)
                            T1->SetDebugText(formatted);
                        else
                            T1->SetDebugText("Failed to format key debugging info.");
                    }
                }
            }
#endif

            switch (tcodKey.vk) {
            case TCODK_ENTER:      ch = KY_ENTER;     break; 
            case TCODK_KPENTER:    ch = KY_ENTER;     break; 
            case TCODK_ESCAPE:     ch = KY_ESC;       break;
            case TCODK_BACKSPACE:  ch = KY_BACKSPACE; break;
            case TCODK_TAB:        ch = KY_TAB;       break;
            case TCODK_RIGHT:      ch = KY_RIGHT;     break;
            case TCODK_UP:         ch = KY_UP;        break;
            case TCODK_LEFT:       ch = KY_LEFT;      break;
            case TCODK_DOWN:       ch = KY_DOWN;      break;
            case TCODK_PAGEUP:     ch = KY_PGUP;      break;
            case TCODK_PAGEDOWN:   ch = KY_PGDN;      break;
            case TCODK_HOME:       ch = KY_HOME;      break;
            case TCODK_END:        ch = KY_END;       break;

			/* Users with numpads should turn numlock off. */
            case TCODK_KP7:        ch = KY_HOME;      break;
            case TCODK_KP8:        ch = KY_UP;        break;
            case TCODK_KP9:        ch = KY_PGUP;      break;
            case TCODK_KP4:        ch = KY_LEFT;      break;
            case TCODK_KP6:        ch = KY_RIGHT;     break;
            case TCODK_KP1:        ch = KY_END;       break;
            case TCODK_KP2:        ch = KY_DOWN;      break;
            case TCODK_KP3:        ch = KY_PGDN;      break;
            case TCODK_KPDEC:      ch = KY_PERIOD;    break;

            case TCODK_F1:         ch = KY_CMD_MACRO1; break;
            case TCODK_F2:         ch = KY_CMD_MACRO2; break;
            case TCODK_F3:         ch = KY_CMD_MACRO3; break;
            case TCODK_F4:         ch = KY_CMD_MACRO4; break;
            case TCODK_F5:         ch = KY_CMD_MACRO5; break;
            case TCODK_F6:         ch = KY_CMD_MACRO6; break;
            case TCODK_F7:         ch = KY_CMD_MACRO7; break;
            case TCODK_F8:         ch = KY_CMD_MACRO8; break;
            case TCODK_F9:         ch = KY_CMD_MACRO9; break;
            case TCODK_F10:         ch = KY_CMD_MACRO10; break;
            case TCODK_F11:         ch = KY_CMD_MACRO11; break;
            case TCODK_F12:         ch = KY_CMD_MACRO12; break;
            case TCODK_CHAR:
                if (ControlKeys & (CONTROL|ALT))
                    break;
            default:
                continue;
            }
        }

        if (mode == KY_CMD_RAW)
            return ch;

        for (i = keyset_start;i>=0 && keyset[i].cmd != KY_CMD_LAST;i+=keyset_delta) { 
            if (keyset[i].raw_key != toupper(ch)) 
                continue;
            if (keyset[i].raw_key_flags != -1)
                if (keyset[i].raw_key_flags != (ControlKeys & (CONTROL|SHIFT|ALT)))
                    continue;
            if (mode == KY_CMD_ARROW_MODE && (keyset[i].cmd < KY_CMD_FIRST_ARROW || keyset[i].cmd > KY_CMD_LAST_ARROW))
                continue; 
            return keyset[i].cmd;
        }
        return ch;
    }
}

bool libtcodTerm::CheckEscape() {
    TCOD_key_t key;
    while (1) {
        key = readkey(0);
        if (key.vk == TCODK_NONE)
            return false;
        if (key.vk == TCODK_ESCAPE)
            return true;
    }
}
  
void libtcodTerm::ClearKeyBuff() {
      /*
    int16 i;
    for (i=0;i!=KEY_MAX;i++)
      if (key[i])
        return;
        */
    TCOD_key_t key;
    while (1) {
        key = readkey(0);
        if (key.vk == TCODK_NONE)
            break;
    }
}
  
void libtcodTerm::PrePrompt() {
    TCOD_key_t key;
    while (1) {
        key = readkey(0);
        if (key.vk == TCODK_NONE)
            break;
    }
}
  
/*****************************************************************************\
*                                 libtcodTerm                                   *
*                               File I/O Code                                 *
\*****************************************************************************/

bool libtcodTerm::Exists(const char *fn) {
    return (TCOD_sys_file_exists(fn) != 0);
}

void libtcodTerm::Delete(const char *fn)
  {
    if (!TCOD_sys_delete_file(fn))
        Error("Can't delete file \"%s\".",fn);
  }

void libtcodTerm::OpenRead(const char*fn)
  {
    fp = fopen(fn,"rb");
    if (!fp)
      throw ENOFILE;
    CurrentFileName = fn;
  }

void libtcodTerm::OpenWrite(const char*fn)
  {
    fp = fopen(fn,"wb+");
    if (!fp)
      throw ENODIR;
    CurrentFileName = fn;
  }

void libtcodTerm::OpenUpdate(const char*fn)
  {
    fp = fopen(fn,"rb+");
    if (!fp)
      fp = fopen(fn,"wb+");
    if (!fp)
      throw ENODIR;
    CurrentFileName = fn;
  }

void libtcodTerm::Close()
  {
    if (fp) {
      fclose(fp);
      fp = NULL;
      }
  }
  
void libtcodTerm::FRead(void *vp,size_t sz)
  {
    if (fread(vp,1,sz,fp)!=sz)
      throw EREADERR;
  }

void libtcodTerm::FWrite(const void *vp,size_t sz)
  {
    if (!fp)
      return;
    if (fwrite(vp,1,sz,fp)!=sz)
      throw EWRIERR;
  }

int32 libtcodTerm::Tell()
  { return ftell(fp); }  

void libtcodTerm::Seek(int32 pos,int8 rel)
  { if(fseek(fp,pos,rel))
      throw ECORRUPT; }

void libtcodTerm::Cut(int32 amt)
  {
    /* Fix this later! */
    int32 start, end;
    void *block;
    start = ftell(fp);
    fseek(fp,0,SEEK_END);
    end = ftell(fp);
    fseek(fp,start+amt,SEEK_SET);
    block = malloc(end - (start+amt));
    fread(block,end - (start+amt),1,fp);
    fseek(fp,start,SEEK_SET);
    fwrite(block,end - (start+amt),1,fp);
  }

char * libtcodTerm::MenuListFiles(const char * filespec, uint16 flags, const char * title) {
    char * file;
    TCOD_list_t l = TCOD_sys_get_directory_content(CurrentDirectory, filespec);
    for (int *iterator = (int *)TCOD_list_begin(l); iterator != (int *)TCOD_list_end(l); iterator++) {
        const char *fileName = (const char *)(*iterator);
        char *str = strdup(fileName);
        /* TODO: Check if 'str' can leak */
        LOption(str,(int32)str);
    }
    TCOD_list_clear_and_delete(l);
    file = (char*)LMenu(flags,title);
    if (file == (char*)(-1))
      return NULL;
    return file;
}

bool libtcodTerm::FirstFile(char * filespec) {
    alf_it = NULL;
    if (alf != NULL && TCOD_list_is_empty(alf))
        TCOD_list_clear_and_delete(alf);
    alf = TCOD_sys_get_directory_content(CurrentDirectory, filespec);
    if (TCOD_list_size(alf) == 0)
        return false;
    alf_it = (int32 *)TCOD_list_begin(alf);
    try {
        OpenRead((const char *)*alf_it);
    } catch (int) {
        fp = NULL;
        return false;
    }
    ASSERT(fp);
    return true;
}

bool libtcodTerm::NextFile() {
Retry:
    alf_it += 1;
    if (alf_it == (int32 *)TCOD_list_end(alf)) {
        TCOD_list_clear_and_delete(alf);
        return false;
    }
    try {
        OpenRead((const char *)*alf_it);
    } catch (int) {
        fp = NULL;
        goto Retry;
    }
    ASSERT(fp);
    return true;
}

#endif

