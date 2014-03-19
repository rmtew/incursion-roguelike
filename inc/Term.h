/* TERM.H -- Copyright (c) 1999-2003 Julian Mensch
     This file contains class definitions for Term and
   TextTerm.
*/

#define KY_UP    (200 + NORTH)
#define KY_DOWN  (200 + SOUTH)
#define KY_LEFT  (200 + WEST)
#define KY_RIGHT (200 + EAST)
#define KY_PGUP  (200 + NORTHEAST)
#define KY_PGDN  (200 + SOUTHEAST)
#define KY_HOME  (200 + NORTHWEST)
#define KY_END   (200 + SOUTHWEST)
#define KY_CTRL  210
#define KY_SHIFT 211
#define KY_ALT   212
#define KY_BACKSPACE 213
#define KY_F1    214
#define KY_F2    215
#define KY_F3    216
#define KY_F4    217
#define KY_F5    218
#define KY_F6    219
#define KY_F7    220
#define KY_F8    221
#define KY_F9    222
#define KY_F10   223
#define KY_F11   224
#define KY_F12   225
#define KY_REDRAW 250


#define KY_A     'a'
#define KY_Z     'z'
#define KY_ZERO  '0'
#define KY_ONE   '1'
#define KY_EIGHT '8'
#define KY_NINE  '9'
#define KY_TAB   '\t'
#define KY_ESC   27
#define KY_ENTER 13
#define KY_SPACE ' '
#define KY_EQUALS '='
#define KY_MINUS '-'
#define KY_HELP  '?'
#define KY_COMMA ','
#define KY_COLON ':'
#define KY_PERIOD '.'

#define KY_QUOTE  '\"'

#define ARROW(x) (200 + x)
#define FN(x)    (KY_CMD_MACRO1 + (x) - 1)

#define SHIFT     0x01
#define CONTROL   0x02
#define ALT       0x04

#define BOX_TOP      0x0001
#define BOX_CORNERS  0x0002
#define BOX_THINLINE 0x0004
#define BOX_NOPAUSE  0x0018
#define BOX_NOSAVE   0x0010
#define BOX_JUSTIFY  0x0020
#define BOX_NOBORDER 0x0040
#define BOX_SCROLL   0x0080 /* scroll with +/- keys if needed */
#define BOX_HYPLINKS 0x0100 /* future online help system */
#define BOX_TITLE    0x0200 /* center first line; don't scroll it */
#define BOX_NOWRAP   0x0400 /* no word wrapping */
#define BOX_WIDEBOX  0x0800 /* width > 50 allowed */
#define BOX_SAVECHAR 0x1000 /* Take 'continue' char as next command */
#define BOX_MUST_PRESS_ENTER 0x2000 /* Must press ENTER to leave box */

#define TEXT_BUFFER_LENGTH      (256 * 1024)

typedef enum {
  KY_CMD_NORMAL_MODE,
  KY_CMD_TARGET_MODE,
  KY_CMD_ARROW_MODE, /* trying to get a direction, 
                       * avoid interpreting other keys */
  KY_CMD_RAW
} KeyCmdMode; 

#define KY_CMD 192
#define KY_CMD_FIRST_ARROW KY_CMD_NORTH
#define KY_CMD_LAST_ARROW  KY_CMD_SOUTHWEST
#define KY_CMD_FIRST_LOOK_ARROW KY_CMD_LOOK_NORTH
#define KY_CMD_LAST_LOOK_ARROW  KY_CMD_LOOK_SOUTHWEST
typedef enum {
        KY_HEADER_DIR          =(KY_CMD-1),
        KY_CMD_NORTH           =(KY_CMD+0),
        KY_CMD_SOUTH           =(KY_CMD+1),
        KY_CMD_EAST            =(KY_CMD+2),
        KY_CMD_WEST            =(KY_CMD+3),
        KY_CMD_NORTHEAST       =(KY_CMD+4),
        KY_CMD_NORTHWEST       =(KY_CMD+5),
        KY_CMD_SOUTHEAST       =(KY_CMD+6),
        KY_CMD_SOUTHWEST       =(KY_CMD+7),
        KY_CMD_ESCAPE          ,
        KY_CMD_HELP            ,
        KY_CMD_TAB             ,
        KY_CMD_ENTER           ,
        
        KY_HEADER_PROMPT       ,
        KY_CMD_ALL_ALLIES      , // a
        KY_CMD_TARGET          , // t
        KY_CMD_DIRECTION       , // d
        KY_CMD_EXAMINE         , // x
        KY_CMD_EXAMINE_REGION  , // r
        KY_CMD_EXAMINE_FEATURE , // f
        KY_CMD_EXAMINE_MAP     , // m
        KY_CMD_NEXT            , // n
        KY_CMD_OVERVIEW_MAP    , // o
        KY_CMD_TARGET_SELF     , // *
        KY_CMD_TARGET_MOUNT    , // h
        
        
        
        
        
        KY_CMD_DUMP_CONTAINER  , // ^D
        KY_CMD_WAND_IDENT      , // ^I
        KY_CMD_CAST_DETECT_SPELLS   , // ^F ("find monsters") 
        KY_CMD_ORDER,            // ^o

        KY_CMD_JOURNAL           , 

        KY_CMD_TREAT_MONSTER   , // ^T

        KY_HEADER_GAME         ,
        KY_CMD_LOOK_NORTH      ,
        KY_CMD_LOOK_SOUTH      ,
        KY_CMD_LOOK_EAST       ,
        KY_CMD_LOOK_WEST       ,
        KY_CMD_LOOK_NORTHEAST  ,
        KY_CMD_LOOK_NORTHWEST  ,
        KY_CMD_LOOK_SOUTHEAST  ,
        KY_CMD_LOOK_SOUTHWEST  ,

        KY_CMD_MACRO1          ,
        KY_CMD_MACRO2          ,
        KY_CMD_MACRO3          ,
        KY_CMD_MACRO4          ,
        KY_CMD_MACRO5          ,
        KY_CMD_MACRO6          ,
        KY_CMD_MACRO7          ,
        KY_CMD_MACRO8          ,
        KY_CMD_MACRO9          ,
        KY_CMD_MACRO10         ,
        KY_CMD_MACRO11         ,
        KY_CMD_MACRO12         ,

        KY_CMD_LEGEND          , // /
        KY_CMD_DEBUG_MESSAGES  , // !
        KY_CMD_OPTIONS         , // =
        KY_CMD_REST            , // .
        KY_CMD_RUN             , // ,
        KY_CMD_EXCHANGE        , // -
        KY_CMD_UP              , // <
        KY_CMD_DOWN            , // >

        KY_CMD_ACTIVATE        , // a
        KY_CMD_BLAST_WAND      , // b
        KY_CMD_ATTACK_MENU     , // c
        KY_CMD_CHAR_SHEET      , // d
        KY_CMD_EAT             , // e
        KY_CMD_FIRE            , // f
        KY_CMD_GET             , // g
        KY_CMD_HIDE            , // h
        KY_CMD_INVENTORY       , // i
        KY_CMD_JUMP            , // j
        KY_CMD_KICK            , // k
        KY_CMD_LOOK            , // l
        KY_CMD_MAGIC           , // m
        KY_CMD_NAME            , // n
        KY_CMD_OPEN            , // o
        KY_CMD_PRAY            , // p
        KY_CMD_QUAFF           , // q
        KY_CMD_READ            , // r
        KY_CMD_SEARCH          , // s
        KY_CMD_TALK            , // t
        KY_CMD_USE             , // u
        KY_CMD_SHOW_MESSAGES   , // v
        KY_CMD_WIZMODE         , // w
        KY_CMD_CANCEL_MENU     , // x
        KY_CMD_YUSE            , // y
        KY_CMD_SLEEP           , // z 
        
        KY_CMD_QUICK_QUIT      ,

        KY_CMD_LAST
} KeyCmd; 

#define DIR_OF_KY_CMD(x) ((Dir)((x) - KY_CMD_NORTH))
        /* converts KY_CMD_SOUTHWEST into SOUTHWEST, etc.  */
#define DIR_OF_KY_CMD_LOOK(x) ((Dir)((x) - KY_CMD_LOOK_NORTH))

#define MO_SPLASH   1
#define MO_PLAY     2
#define MO_INV      3
#define MO_CHARVIEW 4
#define MO_HELP     5
#define MO_CREATE   6
#define MO_BIGMAP   7
#define MO_RECREATE 8
#define MO_DIALOG   9
#define MO_BARTER   10
#define MO_ROLLING  11

#define KY_FLAG_SHIFT   1
#define KY_FLAG_CONTROL 2
#define KY_FLAG_ALT     4

typedef struct KeySetItem {
  KeyCmd cmd ;          /* KY_CMD_TALK */
  int16 raw_key ;       /* 't' */
  int16 raw_key_flags;  /* KY_FLAG_CONTROL */ 
} KeySetItem; 

typedef struct HyperLink 
  {
    char Link[4];
    int16 LineNo;
    int16 Column;
    char Text[40];
  } HypereLink;

extern KeySetItem StandardKeySet[], RoguelikeKeySet[];                                

typedef struct LMenuOption {
  String Text;
  String Desc;
  int32 Val;
  int32 SortKey; // for non-alphabetical sorting
  bool isMarked;
  } 
  LMenuOption;

typedef struct WViewListElt {
  Thing *t;
  int32 dist;
  int16 color;
  int16 glyph; 
  } 
  WViewListElt; 

extern KeySetItem StandardKeySet[]; 

struct CharSheet
  {
    String Banner;
    String Basics;
    String sAttr[7];
    String sSaves[3];
    String sCombat[5][3];
    String sSize,
           sDefense,
           sCoverage,
           sMove,
           sHitDice,
           sFatigue,
           sMana,
           sMagicRes,
           sEncum;
    String sSpells[4];
    String sProfs;
    String sSpecials,
           sFeats, 
           sResists,
           sImmune,
           sDepths,
           sPerception,
           sSkills,
           sRacial,
           sBody,
           sAutoBuff,
           sCharGen,
           sSpiritual;
  };
                                   
class Term
	{
    friend class Map;
		protected:
      /* Basic Data */
      int16 sizeX, sizeY; 
		  Player *p;
      Object *theTarget;
		  Map *m;
      Container * currCon;
      LocationInfo *gr;
		  int16 XOff,YOff;
      Item* Selected;
      uint8 OffscreenX[64], OffscreenY[64], OffscreenC;

      HyperLink HL[512];
      int16 LinkCount;
      int16 SelectedLink;
      bool isHelp, updated;
      
      /* The Debugger */
      #ifdef BYTECODE_TRACER
      String CodeDump[9162];
      int32  szDump;
      
      rID    trace_xID;
      hCode  trace_start;
      int16  trace_Event;
      int32  trace_curr,
             trace_cursor;
             
      int32  Locals[16], cLocals;
      int32  ResVars[16], cResVars;
             
      #endif
      int16 QKeyType;
      QuickKey *QuickKeys;
      
      /* Menu Internals */
      LMenuOption Option[1024]; // ww: merge for easy sorting
      int16 OptionCount, cOption;

      /* Input Prompt Internals */
      char Input[160];
		  int8 linepos,linenum;
		  bool LineMode;
      uint8 QueuedChar;
      bool OptionsShown;

      /* View List Stuff */
      WViewListElt WViewList[16];
      int WViewListCount; 


    public:
      
      String IncursionDirectory;
      
      /* Window Handling (TextTerm.cpp) */
      virtual void InitWindows()=0;
      virtual void SetWin(int16 win)=0;
      virtual void SizeWin(int16 win, int16 x1, int16 y1, int16 x2, int16 y2)=0;
      virtual void SizeWin(int16 win, int16 sx, int16 sy)=0;
      virtual int16 WinTop()=0;
      virtual int16 WinBottom()=0;
      virtual int16 WinLeft()=0;
      virtual int16 WinRight()=0;
      virtual int16 WinSizeX()=0;
      virtual int16 WinSizeY()=0;
      virtual int16 GetMode()=0;
      virtual void  SetMode(int16 _mode)=0;
      
      /* Tidy Message I/O (TextTerm.cpp) */      
      virtual void Write(const char*str)=0;
      virtual void Write(int16 x, int16 y, const char*str)=0;
      virtual void CWrite(const char*)=0;
      virtual void Message(const char*)=0;
      virtual void AddMessage(const char *)=0; 
      virtual void ShowMessages()=0;      
      virtual void DrawBorder(int16 wn, int16 col=YELLOW)=0;
      virtual void Box(const char*text)=0;
      virtual void Box(int16 w, const char*text)=0;
      virtual void Box(int16 w, int16 fl, const char*text)=0;
      virtual void Box(int16 w, int16 fl, int16 cbor, int16 ctxt, const char*text)=0;
      virtual int16 WrapWrite(int16 x,int16 y,const char*,int16 x2=0,int16 maxlines=50)=0;  
            
      /* Scrolling Text (TextTerm.cpp) */
      virtual void UpdateScrollArea(int16 _offset, int16 wn = WIN_CUSTOM)=0;
      virtual void HyperTab(int16 wn = WIN_CUSTOM)=0;
      virtual void ScrollUp(int16 wn = WIN_CUSTOM)=0;
      virtual void ScrollDown(int16 wn = WIN_CUSTOM)=0;
      virtual void SWrite(int16 x,int16 y,const char*text, int16 wn = WIN_CUSTOM)=0;
      virtual void SGotoXY(int16 x, int16 y)=0;
      virtual void SWrite(const char*text, int16 wn = WIN_CUSTOM)=0;
      virtual int16 SWrapWrite(int16 x,int16 y,const char*, int16 y2=0, int16 wn = WIN_CUSTOM)=0;
      virtual void ClearScroll()=0;

      /* Formatted Menus (TextTerm.cpp) */
      virtual int32 LMenu(uint16 fl, const char*title,int8 MWin=WIN_MENUBOX,
                         const char*help=NULL, int8 pos = 0)=0;
      virtual bool  LMultiSelect(uint16 fl, const char*title,int8 MWin=WIN_MENUBOX,
                         const char*help=NULL, int8 pos = 0)=0;
      void SetQKeyType(QuickKey *qk, int16 typ) { QKeyType = typ;
                                                  QuickKeys = qk; }
      virtual void LOption(const char*text,int32 val,const char*desc=NULL,
                           int32 sortkey=0)=0;
      virtual void LOptionClear()=0;
      virtual void HelpTopic(const char*topic, const char*link=NULL)=0;
      virtual int32 FirstSelected()=0;
      virtual int32 NextSelected()=0;
       
      
      /* Main Gameplay Screen (Term.Cpp) */      
      virtual void ShowStatus() = 0;
      virtual void ShowTraits() = 0;
      virtual void ShowDamage(const char*str, Item *w, int8 Mode) = 0;
      virtual void ShowTime() = 0;
      
      /* Rendering the Map (Term.Cpp) */
      virtual void SetMap(Map *_m) { m = _m; gr = m->Grid; } 
      virtual Map* getMap() { return m; }
      virtual void ShowMap()=0;
      virtual void ShowMapOverview()=0;
		  virtual void AdjustMap(int16 vx,int16 vy,bool newmap=false)=0;
		  virtual void RefreshMap()=0;
      virtual void PutGlyph(int16 x,int16 y,Glyph g)=0;
      virtual Glyph GetGlyph(int16 x, int16 y)=0;
      virtual void ShowThings()=0;
      virtual int16 MSizeX()=0;
      virtual int16 MSizeY()=0;
      virtual int16 OffsetX()=0;
      virtual int16 OffsetY()=0;

      /* View List Code (Term.Cpp) */
      virtual void ShowViewList()=0;
      virtual void ResetWViewList()=0;
      virtual void WViewThing(Thing *t, int32 dist, bool assertSeen)=0;
      
      /* The Effect Prompt (Term.Cpp) */
      virtual bool   EffectPrompt(EventInfo &e, uint16 f, bool is_look=false, const char* Prompt=NULL)=0;
      virtual Thing* ChooseTarget(const char* Prompt = NULL,int16 rType = 0)=0;
      virtual Thing* ExamineSquare(int x, int y)=0;
      
      /* Other Prompts (Term.Cpp) */
      virtual bool yn(const char*msg)=0;
      virtual char ChoicePrompt(const char*msg,const char*choices,int8 col1=BROWN, 
                                  int8 col2=YELLOW,bool preprompt=true)=0;
      virtual Thing* AcquisitionPrompt(int8 Reason, int8 minlev, int8 maxlev, int8 MType=0)=0;
      virtual rID  ChooseResource(const char*prompt, int16 RType, rID eID)=0;
      virtual int32 MonsterTypePrompt(const char * prompt, int minCount = 1, int maxCount = 99999)=0;
      virtual rID MonsterOfTypePrompt(int32 type, const char * prompt)=0;
      virtual rID MonsterPrompt(const char * prompt)=0;
      virtual String & StringPrompt(int8 Color, const char*msg)=0;
      virtual const char* ReadLine(int16 x,int16 y, int16 first_ch=0)=0;
      virtual const char* ReadLine()=0;
      virtual const char* GetTypedString()=0;
      
      /* Splash Screens (Term.Cpp) */
      virtual void DisplayLegend() = 0;
      virtual void DisplayKnownItems()=0;
		  virtual void Title() = 0; 

      /* Character Sheet (Sheet.cpp) */
      virtual void CreateCharSheet(CharSheet &cs) = 0;
      virtual void CreateCharDump(String &str) = 0;
      virtual void DumpCharacter(String & filebase) = 0;
      virtual void DisplayCharSheet() = 0;
      
      /* Various Managers (Managers.Cpp) */
      virtual int16 InventoryManager(bool first, Container*theChest=NULL)=0;
      virtual int16 DumpContainers()=0; 
      virtual int16 SpellManager(int16)=0;
      virtual void  SkillManager(bool initial=false)=0; 
      virtual void  OptionManager(int16 start_cat = OPC_CHARGEN)=0; 
      virtual void  BarterManager(Creature *Seller)=0;
      virtual void  TraceWindow(rID xID, int16 Event, VMachine *VM, 
                                    hCode CP, bool running=false)=0;   
      
      /* Inline Functions */
      virtual bool ShiftDown()=0;
      virtual bool CtrlDown()=0;
      virtual bool AltDown()=0;
      
      /* Low-Level Read/Write (platform-specific) */
      virtual void Update()=0;
      virtual void CursorOn()=0;
      virtual void CursorOff()=0;
      virtual int16 CursorX()=0;
      virtual int16 CursorY()=0;
      virtual void Save()=0;
      virtual void Restore()=0;
      virtual void PutChar(Glyph g)=0;
      virtual void APutChar(int16 x, int16 y, Glyph g)=0;
      virtual void PutChar(int16 x, int16 y, Glyph g)=0;
      virtual Glyph AGetChar(int16 x, int16 y)=0;
      virtual void GotoXY(int16 x, int16 y)=0;
      virtual void Clear()=0;
      virtual void Color(int16 _attr)=0;
      virtual void StopWatch(int16 milli)=0;
      
      /* Input Functions (platform-specific) */
      virtual int16 GetCharRaw()=0;
      virtual int16 GetCharCmd()=0;
      virtual int16 GetCharCmd(KeyCmdMode mode)=0;
      virtual void  ClearKeyBuff()=0;
      virtual bool CheckEscape()=0;
      virtual void PrePrompt()=0;
      
      /* General Functions (platform-specific) */
      static void InitTerminal();
	    virtual void Initialize()=0;
	    virtual void ShutDown()=0;
	    virtual void Reset()=0;
	    virtual bool hideOption(int16 opt)=0;
	    virtual void SetPlayer(Player *_p)=0;
      virtual void ClearPlayer()=0;
      virtual void NotifyGone(hObj h)=0;
      
      /* Scroll Buffer (platform-specific) */
      virtual void SPutChar(int16 x, int16 y, Glyph g)=0;
      virtual uint16 SGetChar(int16 x, int16 y)=0;
      virtual int16 SGetColor(int16 x, int16 y)=0;
      virtual void  BlitScrollLine(int16 wn, int32 buffline, int32 winline)=0;
      
      /* System-Independant File I/O (platform-specific) */
      virtual const char* SaveSubDir()=0;
      virtual const char* ModuleSubDir()=0; 
      virtual const char* LibrarySubDir()=0; 
      virtual const char* LogSubDir()=0; 
      virtual const char* ManualSubDir()=0; 
      virtual const char* OptionsSubDir()=0;
      virtual void ChangeDirectory(const char * c)=0;
      virtual bool Exists(const char* fn)=0;
      virtual void Delete(const char* fn)=0;
      virtual void OpenRead(const char* fn)=0;
      virtual void OpenWrite(const char* fn)=0;
      virtual void OpenUpdate(const char* fn)=0;
      virtual void Close()=0;
      virtual void FRead(void*,size_t sz)=0;
      virtual void FWrite(const void*,size_t sz)=0;
      virtual void Seek(int32,int8)=0;
      virtual void Cut(int32)=0;
      virtual bool FirstFile(char * filespec)=0;
      virtual bool NextFile()=0;
      virtual char * MenuListFiles(const char * filespec, 
          uint16 flags, const char *title)=0;
      virtual const char* GetFileName()=0;
      virtual int32 Tell()=0;     
         
      int16 ActionsSinceLastAutoSave; 
	};


#ifndef NO_TEXTTERM

struct TextWin
  {
    int16 Top, 
          Bottom, 
          Left, 
          Right;
  };

class TextTerm: public Term
  {
    protected:
      TextWin Windows[WIN_LAST], *activeWin;
      uint8 ControlKeys;
      int16 cx, cy, cWrap;
      uint16 attr;
      bool ClearMsgOK, isHelp;
      int8 Mode;
      
      /* Inventory Variables */
  		int8 CurrSlot;
      int8 InvMode;
      Container *theChest;
      Item      * Contents[256];
      int16      yContents[256];
		  
      /* Scrolling Variables */
      int16 offset, scx, scy, ScrollLines;

      int16 TimeLine;

      /* Text Buffers */

      char lbuff[TEXT_BUFFER_LENGTH];
      char buff[TEXT_BUFFER_LENGTH], buff2[TEXT_BUFFER_LENGTH];
      char Text[512][80];
      char MsgHistory[10240];
      char DbgMessages[1024];
      void InvShowSlots(bool changed=true);
      /* Stati Shown In Traits Win */
      int8 s1, s2, s3, s4;
  
    public:
      /* Window Handling (TextTerm.cpp) */
      virtual void InitWindows();
      virtual void SetWin(int16 win);
      virtual void SizeWin(int16 win, int16 x1, int16 y1, int16 x2, int16 y2);
      virtual void SizeWin(int16 win, int16 sx, int16 sy);
      virtual int16 WinTop()     { return activeWin->Top; }
      virtual int16 WinBottom()  { return activeWin->Bottom; }
      virtual int16 WinLeft()    { return activeWin->Left; }
      virtual int16 WinRight()   { return activeWin->Right; }
      virtual int16 WinSizeX() { return (WinRight() - WinLeft())+1; }
      virtual int16 WinSizeY() { return (WinBottom() - WinTop())+1; }
      virtual int16 GetMode() { return Mode; }
      virtual void  SetMode(int16 _mode) { Mode = _mode; }
      
      /* Tidy Message I/O (TextTerm.cpp) */      
      virtual void Write(const char*str);
      virtual void Write(int16 x, int16 y, const char*str)
        { GotoXY(x,y); Write(str); }
      virtual void CWrite(const char*);
      virtual void Message(const char* str);
      virtual void AddMessage(const char *); 
      virtual void ShowMessages();      
      virtual void DrawBorder(int16 wn, int16 col=YELLOW);
      virtual void Box(const char*text)
        { Box(WIN_SCREEN,0,YELLOW,GREY,text); }
      virtual void Box(int16 w, const char*text)
        { Box(w,0,YELLOW,GREY,text); }
      virtual void Box(int16 w, int16 fl, const char*text)
        { Box(w, fl, YELLOW,GREY,text); }
      virtual void Box(int16 w, int16 fl, int16 cbor, int16 ctxt, const char*text);
      virtual int16 WrapWrite(int16 x,int16 y,const char* s,int16 x2=0,int16 maxlines=50);  
            
      /* Scrolling Text (TextTerm.cpp) */
      virtual void UpdateScrollArea(int16 _offset, int16 wn = WIN_CUSTOM);
      virtual void HyperTab(int16 wn = WIN_CUSTOM);
      virtual void SWrite(const char*text, int16 wn = WIN_CUSTOM);
      virtual int16 SWrapWrite(int16 x,int16 y,const char* s, int16 y2, int16 wn = WIN_CUSTOM);
      virtual void ClearScroll();
      virtual void ScrollUp(int16 wn = WIN_CUSTOM)
        { UpdateScrollArea(offset-1, wn); }
      virtual void ScrollDown(int16 wn = WIN_CUSTOM)
        { UpdateScrollArea(offset+1, wn); }
      virtual void SWrite(int16 x,int16 y,const char*text, int16 wn = WIN_CUSTOM)
        { SGotoXY(x,y); SWrite(text,wn); }
      virtual void SGotoXY(int16 x, int16 y)
        { scx = x; scy = y; }
      
      
      /* Formatted Menus (TextTerm.cpp) */
      virtual int32 LMenu(uint16 fl, const char*title,int8 MWin=WIN_MENUBOX,
                         const char*help=NULL, int8 pos = 0);
      virtual bool  LMultiSelect(uint16 fl, const char*title,int8 MWin=WIN_MENUBOX,
                         const char*help=NULL, int8 pos = 0);
      virtual void SetQKeyType(QuickKey *qk, int16 typ) { QKeyType = typ;
                                                  QuickKeys = qk; }

      virtual void LOption(const char*text,int32 val,const char*desc=NULL,
                           int32 sortkey=0);
      virtual void LOptionClear();
      virtual void HelpTopic(const char*topic, const char*link=NULL);
      virtual int32 FirstSelected();
      virtual int32 NextSelected();
      
      /* Main Gameplay Screen (Term.Cpp) */      
      virtual void ShowStatus();
      virtual void ShowTraits();
      virtual void ShowDamage(const char*str, Item *w, int8 Mode);
      virtual void ShowTime();
      
      /* Rendering the Map (Term.Cpp) */
      virtual void ShowMap();
      virtual void ShowMapOverview();
		  virtual void AdjustMap(int16 vx,int16 vy,bool newmap=false);
		  virtual void RefreshMap()
        { p->UpdateMap = true; AdjustMap(p->x,p->y); }
      virtual void PutGlyph(int16 x,int16 y,Glyph g);
      virtual Glyph GetGlyph(int16 x, int16 y);
      virtual Glyph FloorShading(int16 x, int16 y, Glyph g);
      virtual void ShowThings();
      virtual int16 MSizeX() { return (Windows[WIN_MAP].Right  - Windows[WIN_MAP].Left)+1; }
      virtual int16 MSizeY() { return (Windows[WIN_MAP].Bottom - Windows[WIN_MAP].Top)+1; }
      virtual int16 OffsetX() { return XOff; }
      virtual int16 OffsetY() { return YOff; }

      /* View List Code (Term.Cpp) */
      virtual void ShowViewList();
      virtual void ResetWViewList();
      virtual void WViewThing(Thing *t, int32 dist, bool assertSeen);
      
      /* The Effect Prompt (Term.Cpp) */
      virtual bool   EffectPrompt(EventInfo &e, uint16 f, bool is_look=false, const char* Prompt=NULL);
      virtual Thing* ChooseTarget(const char* Prompt = NULL,int16 rType = 0);
      virtual Thing* ExamineSquare(int x, int y);
      
      /* Other Prompts (Term.Cpp) */
      virtual bool yn(const char*msg);
      virtual char ChoicePrompt(const char*msg,const char*choices,int8 col1=BROWN, 
                                  int8 col2=YELLOW,bool preprompt=true);
      virtual Thing* AcquisitionPrompt(int8 Reason, int8 minlev, int8 maxlev, int8 MType);
      virtual rID  ChooseResource(const char*prompt, int16 RType, rID eID);
      virtual int32 MonsterTypePrompt(const char * prompt, int minCount = 1, int maxCount = 99999);
      virtual rID MonsterOfTypePrompt(int32 type, const char * prompt);
      virtual rID MonsterPrompt(const char * prompt);
      virtual String & StringPrompt(int8 Color, const char*msg);
      virtual const char* ReadLine(int16 x,int16 y, int16 first_ch=0);
      virtual const char* ReadLine() { return ReadLine(cx,cy); }
      virtual const char* GetTypedString() { return &Input[0]; }
      
      /* Splash Screens (Term.Cpp) */
      virtual void DisplayLegend();
      virtual void DisplayKnownItems();
		  virtual void Title(); 

      /* Character Sheet (Sheet.cpp) */
      virtual void CreateCharSheet(CharSheet &cs);
      virtual void CreateCharDump(String &str);
      virtual void DumpCharacter(String & filebase);
      virtual void DisplayCharSheet();
      
      /* Help-Related (Help.Cpp) */
      virtual void GetHelp(String & helpText, const char *topic);
      
      /* Various Managers (Managers.Cpp) */
      virtual int16 InventoryManager(bool first, Container*theChest=NULL);
      virtual int16 DumpContainers(); 
      virtual int16 SpellManager(int16);
      virtual void  SkillManager(bool initial=false); 
      virtual void  OptionManager(int16 start_cat = OPC_CHARGEN); 
      virtual void  BarterManager(Creature *Seller);
      virtual void  TraceWindow(rID xID, int16 Event, VMachine *VM, 
                                    hCode CP, bool running=false);   
      
      
      /* Inline Functions */
      virtual bool ShiftDown()  { return ControlKeys & SHIFT; }
      virtual bool CtrlDown()   { return ControlKeys & CONTROL; }
      virtual bool AltDown()    { return ControlKeys & ALT; }
      
      /* Low-Level Read/Write (platform-specific) */
      virtual void Update()=0;
      virtual void CursorOn()=0;
      virtual void CursorOff()=0;
      virtual int16 CursorX() { return cx; }
      virtual int16 CursorY() { return cy; }
      virtual void Save()=0;
      virtual void Restore()=0;
      virtual void PutChar(Glyph g)=0;
      virtual void APutChar(int16 x, int16 y, Glyph g)=0;
      virtual void PutChar(int16 x, int16 y, Glyph g)=0;
      virtual Glyph AGetChar(int16 x, int16 y)=0;
      virtual void GotoXY(int16 x, int16 y)=0;
      virtual void Clear()=0;
      virtual void Color(int16 _attr)=0;
      virtual void StopWatch(int16 milli)=0;
      
      /* Input Functions (platform-specific) */
      virtual int16 GetCharRaw()=0;
      virtual int16 GetCharCmd()=0;
      virtual int16 GetCharCmd(KeyCmdMode mode)=0;
      virtual void  ClearKeyBuff()=0;
      virtual bool CheckEscape()=0;
      virtual void PrePrompt() { }
      
      /* General Functions (platform-specific) */
	    virtual void Initialize()=0;
	    virtual void Reset()               { }
	    virtual void ShutDown()           { }
	    virtual bool hideOption(int16 opt)=0;
	    virtual void SetPlayer(Player *_p) { p = _p; }
      virtual void ClearPlayer()         { p = NULL; }
      virtual void NotifyGone(hObj h)    { }
            
      /* Scroll Buffer (platform-specific) */
      virtual void SPutChar(int16 x, int16 y, Glyph g)=0;
      virtual void SPutColor(int16 x, int16 y, int16 col)=0;
      virtual void SClear()=0;
      virtual uint16 SGetChar(int16 x, int16 y)=0;
      virtual int16 SGetColor(int16 x, int16 y)=0;
      virtual void  BlitScrollLine(int16 wn, int32 buffline, int32 winline)=0;
      
      /* System-Independant File I/O (platform-specific) */
      virtual const char* SaveSubDir()=0;
      virtual const char* ModuleSubDir()=0; 
      virtual const char* LibrarySubDir()=0; 
      virtual const char* LogSubDir()=0; 
      virtual const char* ManualSubDir()=0; 
      virtual const char* OptionsSubDir()=0;
      virtual void ChangeDirectory(const char * c)=0;
      virtual bool Exists(const char* fn)=0;
      virtual void Delete(const char* fn)=0;
      virtual void OpenRead(const char* fn)=0;
      virtual void OpenWrite(const char* fn)=0;
      virtual void OpenUpdate(const char* fn)=0;
      virtual void Close()=0;
      virtual void FRead(void*,size_t sz)=0;
      virtual void FWrite(const void*,size_t sz)=0;
      virtual void Seek(int32,int8)=0;
      virtual void Cut(int32)=0;
      virtual bool FirstFile(char * filespec)=0;
      virtual bool NextFile()=0;
      virtual char * MenuListFiles(const char * filespec, 
          uint16 flags, const char *title)=0;
      virtual const char* GetFileName()=0;
      virtual int32 Tell()=0;     
      
  };

#endif
