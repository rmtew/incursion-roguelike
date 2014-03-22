/* WALLEGRO.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Contains the class definition and member functions for the
   AllegroTerm class, which is the only part of Incursion which
   contains platform-dependant Allegro calls for Windows. Includes 
   both the central screen functions, as well as wrapper functions
   for file management.

   Low-Level Read/Write
     void AllegroTerm::Update() 
     void AllegroTerm::CursorOn()
     void AllegroTerm::CursorOff()
     int16 AllegroTerm::CursorX()
     int16 AllegroTerm::CursorY()
     void AllegroTerm::Save()
     void AllegroTerm::Restore()
     void AllegroTerm::PutChar(Glyph g) 
     void AllegroTerm::APutChar(int16 x, int16 y, Glyph g)
     void AllegroTerm::PutChar(int16 x, int16 y, Glyph g) 
     void AllegroTerm::GotoXY(int16 x, int16 y) 
     void AllegroTerm::Clear() 
     void AllegroTerm::Color(int16 _attr) 
     void AllegroTerm::StopWatch(int16 milli)  
   Input Functions
     int16 AllegroTerm::GetCharRaw()
     int16 AllegroTerm::GetCharCmd()
     int16 AllegroTerm::GetCharCmd(KeyCmdMode mode)
     bool  AllegroTerm::CheckEscape()
   General Functions
     void Term::InitTerminal()
     void AllegroTerm::Initialize()
     void Fatal(const char*fmt,...)
     void Error(const char*fmt,...)
   Scroll Buffer
      void  AllegroTerm::SPutChar(int16 x, int16 y,Glyph g);
      int16 AllegroTerm::SGetGlyph(int16 x, int16 y);
      void  AllegroTerm::SPutColor(int16 x, int16 y, int16 col);
      int16 AllegroTerm::SGetColor(int16 x, int16 y);
      void  AllegroTerm::ClearScroll();
      void  AllegroTerm::BlitScrollLine(int16 wn, int32 buffline, int32 winline);
   File I/O
     bool AllegroTerm::Exists(const char *fn)
     void AllegroTerm::Delete(const char *fn)
     bool AllegroTerm::Scour(const char *fn, bool Manditory)
     void AllegroTerm::OpenRead(const char*fn)
     void AllegroTerm::OpenWrite(const char*fn)
     void AllegroTerm::OpenUpdate(const char*fn)
     void AllegroTerm::Close()
     void AllegroTerm::FRead(void *vp,size_t sz)
     void AllegroTerm::FWrite(void *vp,size_t sz)
     int32 AllegroTerm::Tell()
     void AllegroTerm::Seek(int32 pos,int8 rel)
     void AllegroTerm::Cut(int32 amt)
    
*/

#ifdef ALLEGRO_TERM

#include "allegro.h"

#define DAT2C
extern DATAFILE data;

#undef TRACE
#undef MIN
#undef MAX
#undef ERROR
#undef EV_BREAK

#include <stdarg.h>
#include <ctype.h> 
#include <time.h>
#include "Incursion.h"

#define FORE(g) ((g & 0x0F00) / 256)
#define BACK(g) ((g & 0xF000) / (256*16))
#define ATTR(g) ((g & 0xFF00) / 256)
#define CHAR(g) ((g & 0x00FF))

#define SCREEN_BUFF_SIZE (300 * 200)

#define MAX_COLORS 16

RGB RGBValues[MAX_COLORS] = {
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


RGB RGBSofter[MAX_COLORS] = {
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
  
class AllegroTerm: public TextTerm
  {
    friend void Error(const char*fmt,...);
    friend void Fatal(const char*fmt,...);

    private:
      bool isWindowed, showCursor, cursorPulse;
      Glyph *bScreen, *bSave, *bScroll, *bCurrent;
      BITMAP *Glyphs[256], *Font;
      int32 Colors[MAX_COLORS];
      int16 resX, resY, fontX, fontY, ocx, ocy;
      int32 lastBlink;
      int oldResX, oldResY;
       
      
      /* File I/O Stuff */
      String CurrentFileName; 
      FILE *fp; al_ffblk alf;
      
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
      virtual void Color(int16 _attr);
      virtual void StopWatch(int16 milli);
      
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
      
      /* Scroll Buffer */
      virtual void SPutChar(int16 x, int16 y, Glyph g);
      virtual uint16 SGetChar(int16 x, int16 y);
      virtual void SPutColor(int16 x, int16 y, int16 col);
      virtual int16 SGetColor(int16 x, int16 y);
      virtual void SClear();
      virtual void  BlitScrollLine(int16 wn, int32 buffline, int32 winline);
      
      /* System-Independant File I/O */
      virtual const char* SaveSubDir()    { return "save"; } 
      virtual const char* ModuleSubDir()  { return "mod"; } 
      virtual const char* LibrarySubDir() { return "lib"; } 
      virtual const char* LogSubDir()     { return "logs"; } 
      virtual const char* ManualSubDir()  { return "man"; } 
      virtual const char* OptionsSubDir() { return "."; } 
      virtual void ChangeDirectory(const char * c) { 
        if (chdir(IncursionDirectory) ||
            chdir(c))
          Fatal("Unable to locate directory '%s'.",
            (const char*)(IncursionDirectory + SC(c)));
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

volatile int timer;
bool closePressed = false;

Term *T1;
AllegroTerm *AT1;

/*****************************************************************************\
*                                 AllegroTerm                                *
*                              main() Function                               *
\*****************************************************************************/


int main()
  {
    theGame = new Game();
    Term::InitTerminal();
    theGame->StartMenu();
    T1->ShutDown();
    delete theGame;
    return 0;
    /*
    T1->Initialize();
    T1->SetWin(WIN_SCREEN);
    T1->Clear();
    int16 i;
    for (i=0;i!=256;i++)
      T1->PutChar(i%16,i/16,i + WHITE*256);
    T1->GetCharRaw();
    T1->GetCharRaw();
    */
  }
  
END_OF_MAIN()

/*****************************************************************************\
*                                 AllegroTerm                                   *
*                           Low-Level Read/Write                              *
\*****************************************************************************/

void AllegroTerm::Update() 
  {
    int32 i, c, ox, oy; Glyph g;
    
    ox = (resX - (fontX*sizeX))/2;
    oy = (resY - (fontY*sizeY))/2;
    
    c = sizeX * sizeY;
    rest(0);
    acquire_screen();
    for (i=0;i!=c;i++)
      if (bScreen[i] != bCurrent[i])
        {
          g = bCurrent[i] = bScreen[i];
          draw_character_ex(screen,Glyphs[CHAR(g)],
            ox + (i % sizeX) * fontX, 
            oy + (i / sizeX) * fontY, 
            Colors[FORE(g)], Colors[BACK(g)]);
        }
    release_screen();
    
    updated = true;
  }
  
void AllegroTerm::Redraw()
  {
    memset(bCurrent,1,SCREEN_BUFF_SIZE * sizeof(Glyph));
    Update();
  }
  
void switch_callback()
  { /* ((AllegroTerm*)T1)->Redraw(); */ }

void timer_proc()
  { timer++; }

void AllegroTerm::CursorOn()
  {
    showCursor = true;
    lastBlink = 0;
    ocx = ocy = 0;
    install_int(timer_proc,300);
  }

void AllegroTerm::CursorOff()
  {
    Glyph g;
    int16 ox = (resX - (fontX*sizeX))/2,
          oy = (resY - (fontY*sizeY))/2;
    showCursor = false;
    cursorPulse = false;
    g = bCurrent[ocy*sizeX + ocx];
    draw_character_ex(screen,Glyphs[CHAR(g)],
      ox + ocx * fontX, 
      oy + ocy * fontY, 
      Colors[FORE(g)], Colors[BACK(g)]);
    ocx = ocy = 0;
    remove_int(timer_proc);
  }
  
void AllegroTerm::BlinkCursor()
  {
    int16 c;
    if (!showCursor)
      return;
    int16 ox = (resX - (fontX*sizeX))/2,
          oy = (resY - (fontY*sizeY))/2;
    cursorPulse = !cursorPulse;
    if (cursorPulse)  
      {
        c = attr & 0x000F;
        if (c == 8) 
          c = 7;
        else if (c < 8)
          c += 8;
        rect(screen,ox + cx*fontX              , oy + cy*fontY  + (fontY*2)/3,
                    ox + cx*fontX + (fontX - 1), oy + cy*fontY  + (fontY - 1),
                    Colors[c]);
        ocy = cy; ocx = cx;
      }
    else
      {
        Glyph g;
        g = bCurrent[ocy*sizeX + ocx];
        draw_character_ex(screen,Glyphs[CHAR(g)],
          ox + ocx * fontX, 
          oy + ocy * fontY, 
          Colors[FORE(g)], Colors[BACK(g)]);
      }
    lastBlink = timer;
  }
  
void AllegroTerm::Save()
  { 
    Update(); 
    memcpy(bSave,bScreen,sizeof(Glyph)*SCREEN_BUFF_SIZE);
  }

void AllegroTerm::Restore()
  { 
    memcpy(bScreen,bSave,sizeof(Glyph)*SCREEN_BUFF_SIZE);
    Update();    
  }

void AllegroTerm::PutChar(Glyph g) 
  {
    if (g >> 8)
      bScreen[cy*sizeX+cx] = g ;
    else
      bScreen[cy*sizeX+cx] = g | attr*256;
    cx++;
    updated = false;
  }

void AllegroTerm::APutChar(int16 x, int16 y, Glyph g) 
  {
    if (g >> 8)
      bScreen[y*sizeX+x] = g;
    else
      bScreen[y*sizeX+x] = g | attr*256;
    updated = false;
  }

void AllegroTerm::PutChar(int16 x, int16 y, Glyph g) 
  {
    x += activeWin->Left;
    y += activeWin->Top;
    if (g >> 8)
      bScreen[y*sizeX+x] = g;
    else
      bScreen[y*sizeX+x] = g | attr*256;
    updated = false;
  }

Glyph AllegroTerm::AGetChar(int16 x, int16 y)
  {
    return bScreen[y*sizeX+x];
  }


void AllegroTerm::GotoXY(int16 x, int16 y) 
  {
    cx = x + activeWin->Left;
    cy = y + activeWin->Top;
    //if (!Cursor.bVisible)
    //  return;
    //crd.X = cx; crd.Y = cy;
    //SetConsoleCursorPosition(hStdOut, crd); 
  }

void AllegroTerm::Clear() 
  {
    for(cy=activeWin->Top;cy<activeWin->Bottom+1;cy++)
      for(cx=activeWin->Left;cx<activeWin->Right+1;cx++)
        { bScreen[cy*sizeX+cx] = 0;
          bCurrent[cy*sizeX+cx] = 1; }
    cWrap = 0; cx = cy = 0;
    if (activeWin == &Windows[WIN_SCREEN] || activeWin == &Windows[WIN_MESSAGE])
      linepos = linenum = 0;
    updated = false;
    
  }

void AllegroTerm::Color(int16 _attr) 
  {
    attr = _attr;
  }

void AllegroTerm::StopWatch(int16 milli)
  {    
    switch (p ? p->Opt(OPT_ANIMATION) : 0) {
      case 0: rest(milli); return;
      case 1: rest((milli+3)/4); return; 
      case 2: return;
    }
  }


/*****************************************************************************\
*                                 AllegroTerm                                   *
*                             General Functions                               *
\*****************************************************************************/


void Term::InitTerminal()
  {
    AT1 = new AllegroTerm;
    T1 = AT1;
    T1->p = NULL; // ww: otherwise we segfault checking options
    T1->Initialize();
    //T1->Title();
  }

void Fatal(const char*fmt,...)
	{
		va_list argptr; 
		va_start(argptr, fmt);
    char ch; 
    if (!T1)
      { allegro_message(__buffer); exit(1); }
    T1->Clear();
		vsprintf(__buffer, fmt, argptr);
    sprintf(__buff2, "Fatal Error: %s\nPress [ENTER] to exit...",__buffer);
		((AllegroTerm*)T1)->Box(WIN_SCREEN,BOX_NOPAUSE|BOX_NOSAVE,RED,PINK,__buff2);
    T1->Update();
    readkey();
    #ifdef DEBUG
      BREAKOUT;
    #endif
    T1->ShutDown();
    exit(1);
    va_end(argptr);
	}

void Error(const char*fmt,...)
	{
		va_list argptr;
		va_start(argptr, fmt); 
    char ch;
    vsprintf(__buffer, fmt, argptr);
    if (!T1)
      { allegro_message(__buffer); exit(1); }
		
    #ifdef DEBUG
    sprintf(__buff2, "Error: %s\n[B]reak, [E]xit or [C]ontinue?",__buffer);
		#else
    sprintf(__buff2, "Error: %s\n[E]xit or [C]ontinue?",__buffer);
    #endif
    ((AllegroTerm*)T1)->Save();
    ((AllegroTerm*)T1)->Box(WIN_SCREEN,BOX_NOPAUSE|BOX_NOSAVE,RED,PINK,__buff2);
    T1->Update();
    
    do 
      ch = toupper(readkey() & 0xFF);
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
    ((AllegroTerm*)T1)->Restore();
    va_end(argptr);
	}



void AllegroTerm::Reset()
  {
    int16 optRes, optFont, i, fx, fy;
    static PALETTE pal; 
    bool scaled_res, scaled_font;
    
    scaled_res = false;
    scaled_font = false;
    
    if (isWindowed) 
      { optRes  = theGame->Opt(OPT_WIND_RES);
        optFont = theGame->Opt(OPT_WIND_FONT); }
    else
      { optRes  = theGame->Opt(OPT_FULL_RES);
        optFont = theGame->Opt(OPT_FULL_FONT); }

    RetryRes:
    
    switch(optRes)
      {
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
    
    //if (isWindowed)
    //  set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,oldResX,oldResY,0,0);

    if (set_gfx_mode(isWindowed ? GFX_AUTODETECT_WINDOWED : 
          GFX_AUTODETECT_FULLSCREEN, resX, resY, 0, 0) != 0)
      {
        if (optRes)
          { optRes--; scaled_res = true; goto RetryRes; }
        allegro_message("Incursion cannot initialize graphics and thus "
          "must terminate.");
        exit(1);
      }

    RetryFont:
    
    /*
    switch(optFont)
      {
        case 0: fontX = 8;  fontY = 8;  
                fx = 1; fy = 1; break;
        case 1: fontX = 12; fontY = 16; 
                fx = 1; fy = (16*8) + 2; break;
        case 2: fontX = 16; fontY = 12; 
                fx = 0; fy = (16*8) + (16*16) + 3; break;
        case 3: fontX = 16; fontY = 16; 
                fx = 0; fy = (16*8) + (16*16) + (16*12) + 4; break;
      }
    */
    switch(optFont)
      {
        case 0: fontX = 8;  fontY = 8;  
                fx = 0; fy = 0; break;
        case 1: fontX = 12; fontY = 16; 
                fx = 0; fy = (16*8); break;
        case 2: fontX = 16; fontY = 12; 
                fx = 0; fy = (16*8) + (16*16); break;
        case 3: fontX = 16; fontY = 16; 
                fx = 0; fy = (16*8) + (16*16) + (16*12) + 1; break;
      }
      
    
    
    sizeX = resX / fontX;
    sizeY = resY / fontY;
    
    if (sizeX < 80 || sizeY < 48)
      { optFont = 0; scaled_font = true; goto RetryFont; }
    
    InitWindows();

    if (theGame->Opt(OPT_SOFT_PALETTE)) {
      for (i=0;i!=MAX_COLORS;i++)
        Colors[i] = makecol(RGBSofter[i].r,
                            RGBSofter[i].g,
                            RGBSofter[i].b);
      }
    else {
      for (i=0;i!=MAX_COLORS;i++)
        Colors[i] = makecol(RGBValues[i].r,
                            RGBValues[i].g,
                            RGBValues[i].b);
      }
                
    for (i=0;i!=256;i++)
      if (Glyphs[i])
        {
          //destroy_bitmap(Glyphs[i]);
          Glyphs[i] = NULL;
        }
            
    if (!Font) {
      DATAFILE *d;
      #ifndef WIN32
      d = &data;
      if (!d)
      #endif
        d = load_datafile("#");
      if (!d) {
        Font = load_bitmap("NGlyphs.bmp",pal);
        if (!Font) {
          ShutDown();
          exit(1);
          }
        }
      else
        Font = (BITMAP*) d[0].dat;
      }
      
    for (i=0;i!=256;i++)
      Glyphs[i] = create_sub_bitmap(Font,
        (i%16)*fontX+fx,(i/16)*fontY+fy,fontX,fontY);
    
    if (isWindowed)
      set_window_title("Incursion: Halls of the Goblin King");

    //SetWin(WIN_SCREEN);
    //Clear();
    
    memset(bCurrent,1,SCREEN_BUFF_SIZE*sizeof(Glyph));
    memset(bScreen ,0,SCREEN_BUFF_SIZE*sizeof(Glyph));
    memset(bSave   ,0,SCREEN_BUFF_SIZE*sizeof(Glyph));

    if (Mode == MO_PLAY)
      { 
        ShowStatus();
        ShowTraits();
        ShowMap();
      }
    else if (Mode == MO_INV)
      {
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
      }
    else if (scaled_font) {
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

void close_pressed()
  { closePressed = true; }


void AllegroTerm::Initialize()
	{
    int16 i; BITMAP *b; PALETTE pal;
    
    p = NULL; m = NULL; isHelp = false;
    ActionsSinceLastAutoSave = 0;
    cx = cy = 0;
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
    timer = 0; 
    isWindowed = true;
    Font = NULL;
    allegro_init();
    get_desktop_resolution(&oldResX,&oldResY);
    set_config_file("Incursion.cfg");
    StopWatch(0);
    install_keyboard();
    install_timer();
    /* So we can compile resources while minimized */
    set_display_switch_mode(SWITCH_BACKAMNESIA);
    set_display_switch_callback(SWITCH_OUT,switch_callback);
    set_close_button_callback(close_pressed);
    
    bScreen  = new Glyph[SCREEN_BUFF_SIZE];
    bCurrent = new Glyph[SCREEN_BUFF_SIZE];
    bSave    = new Glyph[SCREEN_BUFF_SIZE];
    bScroll  = new Glyph[MAX_SCROLL_LINES * SCROLL_WIDTH];
    
    memset(bScreen, 0,SCREEN_BUFF_SIZE * sizeof(Glyph));
    memset(bCurrent,1,SCREEN_BUFF_SIZE * sizeof(Glyph));
    memset(bSave  , 0,SCREEN_BUFF_SIZE * sizeof(Glyph));
    memset(bScroll, 0,MAX_SCROLL_LINES * SCROLL_WIDTH * sizeof(Glyph));
    memset(Glyphs,  0,sizeof(BITMAP*)*256);

    {
      char PathBuff[1024];
      get_executable_name(PathBuff,1024);
      replace_filename(PathBuff,PathBuff,"",1024);
      IncursionDirectory = PathBuff;
      
      /* Kludge to cope with directory structure for
         multiple builds on my machine */
      IncursionDirectory = IncursionDirectory.Replace("DebugAllegro\\","");     
      IncursionDirectory = IncursionDirectory.Replace("ReleaseAllegro\\","");
      IncursionDirectory = IncursionDirectory.Replace("Debug\\","");
      IncursionDirectory = IncursionDirectory.Replace("Release\\","");
      IncursionDirectory = IncursionDirectory.Replace("debug/src/","");
      IncursionDirectory = IncursionDirectory.Replace("release/src/","");
      
    }
    
    Reset();
    
    InitWindows();   

    SetWin(WIN_SCREEN);
    Color(14);
    


	}

void AllegroTerm::ShutDown()
  {
    SetWin(WIN_SCREEN);
    Clear();
    //set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,oldResX,oldResY,0,0);
    //set_gfx_mode(GFX_TEXT,80,25,0,0);
    remove_keyboard();
    remove_timer();
    allegro_exit();
  }

/*****************************************************************************\
*                                 AllegroTerm                                  *
*                               Scroll Buffer                                *
\*****************************************************************************/

void  AllegroTerm::SPutChar(int16 x, int16 y, Glyph g)
  {
    bScroll[y*SCROLL_WIDTH+x] = g;
  }

Glyph AllegroTerm::SGetChar(int16 x, int16 y)
  {
    return bScroll[y*SCROLL_WIDTH+x];
  }

void AllegroTerm::SPutColor(int16 x, int16 y, int16 col)
  {
    bScroll[y*SCROLL_WIDTH+x] = CHAR(bScroll[y*SCROLL_WIDTH+x]) + col*256; 
  }

int16 AllegroTerm::SGetColor(int16 x, int16 y)
  {
    return ATTR(bScroll[y*SCROLL_WIDTH+x]);
  }

void AllegroTerm::SClear()
  {
    memset(bScroll,0,sizeof(Glyph)*SCROLL_WIDTH*MAX_SCROLL_LINES);
  }

void  AllegroTerm::BlitScrollLine(int16 wn, int32 buffline, int32 winline) 
  {
    memcpy(&(bScreen[(winline+Windows[wn].Top)*sizeX + Windows[wn].Left]),
           &(bScroll[buffline*SCROLL_WIDTH]), 
           sizeof(Glyph) * min(SCROLL_WIDTH,WinSizeX()));
  }

/*****************************************************************************\
*                                 AllegroTerm                                   *
*                              Input Functions                                *
\*****************************************************************************/

int16 AllegroTerm::GetCharRaw()
  { return GetCharCmd(KY_CMD_RAW); }

int16 AllegroTerm::GetCharCmd()
  { return GetCharCmd(KY_CMD_NORMAL_MODE); }

int16 AllegroTerm::GetCharCmd(KeyCmdMode mode)
  {
    KeySetItem * keyset = theGame->Opt(OPT_ROGUELIKE) ? 
                       RoguelikeKeySet : StandardKeySet;
    int16 keyset_start, keyset_delta, keyset_last;
    int16 i, ox, oy, ch; int scancode; 
    char okey[KEY_MAX]; TextWin *wn;

    
    ActionsSinceLastAutoSave++;
    if (p) p->GameTimeInfo.keystrokes++;

    if (QueuedChar) {
      ch = QueuedChar;
      QueuedChar = 0;
      return ch;
      }
      
    for (keyset_last = 0; keyset[keyset_last].cmd != 
           KY_CMD_LAST; keyset_last++)
      ;
    keyset_last--;
      
    switch (mode) 
      {
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

    
    for(;;) {
      /* volatile means no memcpy! */
      for(i=0;i!=KEY_MAX;i++)
        okey[i] = key[i];
      poll_keyboard();
      rest(0);
      rest(1);
      ControlKeys = 0;
      if (key[KEY_LCONTROL] || key[KEY_RCONTROL])
        ControlKeys |= CONTROL;
      if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
        ControlKeys |= SHIFT;
      if (key[KEY_ALT] || key[KEY_ALTGR])
        ControlKeys |= ALT; 
      
      /* Allow clearing the message window ONLY if we aren't
         repeating keys, i.e., holding down arrow to run. In
         other words, if no keys are down, or keys are down 
         that weren't down earlier, it is safe to clear. */
      for(i=0;i!=KEY_MAX;i++)
      if (key[i])
        goto AKeyIsDown;
      ClearMsgOK = true;
      AKeyIsDown:;
      for (i=0;i!=KEY_MAX;i++)
        if (key[i] && !okey[i])
          ClearMsgOK = true;

      if (Mode == MO_PLAY && p->UpdateMap)
        RefreshMap();
      if (Mode == MO_PLAY || Mode == MO_INV)
        if (p->statiChanged) {
          ShowTraits();
          ShowStatus();
          }
      
      if (showCursor)
        if (timer > lastBlink)
          BlinkCursor();
      
      if (closePressed) {
        int16 ox, oy;
        CtrlBreak:
        closePressed = false;
        ox = cx; oy = cy;
        if (!theGame || !theGame->InPlay())
          {
            ShutDown();
            exit(0);
          }
        if (GetMode() == MO_PLAY && mode == KY_CMD_NORMAL_MODE) {
          theGame->SetQuitFlag();
          return KY_CMD_QUICK_QUIT;
          }
        closePressed = false;
        }
      
      if (!keypressed()) {
        continue;
      }
        
      if (theGame->Opt(OPT_CLEAR_EVERY_TURN) && ClearMsgOK && 
           mode == KY_CMD_NORMAL_MODE && GetMode() == MO_PLAY) {
        ox = cx; oy = cy; wn = activeWin;
        linenum = linepos = 0;
        SetWin(WIN_MESSAGE);
        Clear();
        activeWin = wn;
        cx = ox; cy = oy;
        ClearMsgOK = false;
        }
        
      lastBlink = 0;
        
      ch = ureadkey(&scancode);
      
      if (scancode == KEY_ENTER)
        if (ControlKeys & ALT)
          {
            isWindowed = !isWindowed;
            Reset();
            Update();
            return KY_REDRAW;
          }
      
      if ((scancode == KEY_C || scancode == KEY_PAUSE)
            && (ControlKeys & CONTROL))
        goto CtrlBreak;
        
      switch (scancode)
        {
          case KEY_ENTER:      ch = KY_ENTER;     break; 
          case KEY_ENTER_PAD:  ch = KY_ENTER;     break; 
          case KEY_SPACE:      ch = KY_SPACE;     break;
          case KEY_ESC:        ch = KY_ESC;       break;
          case KEY_BACKSPACE:  ch = KY_BACKSPACE; break;
          case KEY_TAB:        ch = KY_TAB;       break;
          case KEY_RIGHT:      ch = KY_RIGHT;     break;
          case KEY_UP:         ch = KY_UP;        break;
          case KEY_LEFT:       ch = KY_LEFT;      break;
          case KEY_DOWN:       ch = KY_DOWN;      break;
          case KEY_PGUP:       ch = KY_PGUP;      break;
          case KEY_PGDN:       ch = KY_PGDN;      break;
          case KEY_HOME:       ch = KY_HOME;      break;
          case KEY_END:        ch = KY_END;       break;
          
          case KEY_7_PAD:      ch = KY_HOME;      break;
          case KEY_8_PAD:      ch = KY_UP;        break;
          case KEY_9_PAD:      ch = KY_PGUP;      break;
          case KEY_4_PAD:      ch = KY_LEFT;      break;
          case KEY_6_PAD:      ch = KY_RIGHT;     break;
          case KEY_1_PAD:      ch = KY_END;       break;
          case KEY_2_PAD:      ch = KY_DOWN;      break;
          case KEY_3_PAD:      ch = KY_PGDN;      break;
          
          default:
            if (ch && strchr("`~!@#$%^&*()-_=+{}[];:'\",<.>?/|\\",ch))
              ;
            else if (scancode >= KEY_A && scancode <= KEY_Z)
              ch = ((ControlKeys & SHIFT) ? 'A' : 'a') + (scancode - KEY_A);
            else if (scancode >= KEY_0 && scancode <= KEY_9)
              ch = (ControlKeys & SHIFT) ? ")!@#$%^&*("[scancode - KEY_0]
                : '0' + (scancode - KEY_0);
            else if (scancode >= KEY_F1 && scancode <= KEY_F12)
              ch = FN(scancode - KEY_F1 + 1);
            else
              continue;
        }
      if (mode == KY_CMD_RAW)
        return ch;
      for (i = keyset_start;i>=0 && keyset[i].cmd!=
            KY_CMD_LAST;i+=keyset_delta) 
        { 
          if (keyset[i].raw_key != toupper(ch)) 
            continue;
          if (keyset[i].raw_key_flags != -1)
            if (keyset[i].raw_key_flags != 
                  (ControlKeys & (CONTROL|SHIFT|ALT)))
              continue; 
          if (mode == KY_CMD_ARROW_MODE &&
                (keyset[i].cmd < KY_CMD_FIRST_ARROW ||
                keyset[i].cmd > KY_CMD_LAST_ARROW))
            continue; 
          return keyset[i].cmd;
        }
      return ch;
      }
  }

bool AllegroTerm::CheckEscape()
  {
    while (keypressed())
      if (readkey() >> 8 == KEY_ESC)
        return true;
    return false;
  }
  
void AllegroTerm::ClearKeyBuff()
  {
    int16 i;
    for (i=0;i!=KEY_MAX;i++)
      if (key[i])
        return;
    while (keypressed())
      readkey();
  }
  
void AllegroTerm::PrePrompt()
  {
    int32 i;
    Restart:
    while (keypressed())
      readkey();
    
    for (i=0;i!=KEY_MAX;i++)
      if (key[i])
        goto Restart;
  }
  
/*****************************************************************************\
*                                 AllegroTerm                                   *
*                               File I/O Code                                 *
\*****************************************************************************/

bool AllegroTerm::Exists(const char *fn)
  {
    return exists(fn);
  }

void AllegroTerm::Delete(const char *fn)
  {
    if (delete_file(fn) == -1)
      Error("Can't delete file \"%s\".",fn);
  }

void AllegroTerm::OpenRead(const char*fn)
  {
    fp = fopen(fn,"rb");
    if (!fp)
      throw ENOFILE;
    CurrentFileName = fn;
  }

void AllegroTerm::OpenWrite(const char*fn)
  {
    fp = fopen(fn,"wb+");
    if (!fp)
      throw ENODIR;
    CurrentFileName = fn;
  }

void AllegroTerm::OpenUpdate(const char*fn)
  {
    fp = fopen(fn,"rb+");
    if (!fp)
      fp = fopen(fn,"wb+");
    if (!fp)
      throw ENODIR;
    CurrentFileName = fn;
  }

void AllegroTerm::Close()
  {
    if (fp) {
      fclose(fp);
      fp = NULL;
      }
  }
  
void AllegroTerm::FRead(void *vp,size_t sz)
  {
    if (fread(vp,1,sz,fp)!=sz)
      throw EREADERR;
  }

void AllegroTerm::FWrite(const void *vp,size_t sz)
  {
    if (!fp)
      return;
    if (fwrite(vp,1,sz,fp)!=sz)
      throw EWRIERR;
  }

int32 AllegroTerm::Tell()
  { return ftell(fp); }  

void AllegroTerm::Seek(int32 pos,int8 rel)
  { if(fseek(fp,pos,rel))
      throw ECORRUPT; }

void AllegroTerm::Cut(int32 amt)
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

char * AllegroTerm::MenuListFiles(const char * filespec, 
    uint16 flags, const char * title)
  {
    if (al_findfirst(filespec,&alf,FA_ALL) != 0)
      return NULL;
    do {
      char *str = strdup(alf.name);
      LOption(str,(int32)str);
      }
    while (al_findnext(&alf) == 0);
    al_findclose(&alf);
    char * file = (char*)LMenu(flags,title);
    if (file == (char*)(-1))
      return NULL;
    return file;
  }

bool AllegroTerm::FirstFile(char * filespec)
  {
    if (al_findfirst(filespec,&alf,FA_ALL) != 0)
      return false;
    try {
      OpenRead(alf.name);
      }
    catch (int error_number) {
      fp = NULL;
      return false;
      }
    ASSERT(fp);
    return true;    
  }

bool AllegroTerm::NextFile()
  {
    Retry:
    if (al_findnext(&alf) != 0) {
      al_findclose(&alf);
      return false;
      }
    try {
      OpenRead(alf.name);
      }
    catch (int error_number) {
      fp = NULL;
      goto Retry;
      }
    ASSERT(fp);
    return true;
  }

#endif

