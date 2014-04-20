/* MAP.H -- Copyright (c) 1999-2003 Julian Mensch
     This file contains class definitions for LocationInfo,
   Field, Overlay, Map, Status and Thing. 
*/


class Map;
class Thing;
class Creature;
class Character;
class Player;
class Monster;
class NPC;
class Item;
class Term;
class TextTerm;
class VMachine;
class Magic;
#ifdef WIN32
//STRUCTLIST(Thing);
#endif
extern int8 MapMakerMode;

struct LocationInfo
  {
    unsigned int Glyph       :16;
	  unsigned int Region      :8;
    unsigned int Terrain     :8;
    unsigned int Opaque      :1;   // Opaque (from map)
    unsigned int Obscure     :1;   // foggy
	  unsigned int Lit         :1;
    unsigned int Bright      :1;
	  unsigned int Solid       :1;
    unsigned int Shade       :1;   // Shade by presence of light
    unsigned int hasField    :1;
    unsigned int Dark        :1;   // Magical Darkness
    unsigned int mLight      :1;   // Magical Light
    unsigned int mTerrain    :1;   // Magical Terrain (for Dispel)
    unsigned int cOpaque     :1;   // Opaque (figured in game)
    unsigned int Special     :1;   // Special Terrain
    unsigned int isWall      :1;   // For monster pathfinding
    unsigned int isVault     :1;   // Scrying, Teleport, Dun Design
    unsigned int isSkylight  :1;   
    unsigned int mObscure    :1;   
	  unsigned int Visibility  :16;
	  uint32       Memory;
    hObj         Contents;
  };

extern Tile* MapLetterArray[127];

#define VI_VISIBLE  1
#define VI_DEFINED  2
#define VI_EXTERIOR 4
#define VI_TORCHED  8  /* lit by mobile light source */

struct Field
  {
    rID eID;
    uint32 FType;
    Glyph Image;
    uint8 cx,cy, rad;
    int16 Dur;
    hObj Creator;
    hObj Next;
    int8 Color;
    bool inArea(int16 x,int16 y)
      { return dist(x,y,cx,cy) <= rad; }
    bool Affects(Thing *t);
    uint8 FieldDC();
  };

class Term;
struct RoomInfo;

class Overlay
	{
		private:
			int16 GlyphX[MAX_OVERLAY_GLYPHS];
			int16 GlyphY[MAX_OVERLAY_GLYPHS];
			Glyph GlyphImage[MAX_OVERLAY_GLYPHS];
			int16 GlyphCount;
			hObj m;
		public:
			Overlay(hObj _m) { Active=false;m=_m; }
      Overlay() {}
			bool Active;
			void Activate();
			void DeActivate();
			void AddGlyph(int16 x,int16 y,Glyph g);
			void RemoveGlyph(int16 x,int16 y);
			void RemoveGlyph(int16 n);
      bool IsGlyphAt(int16 x, int16 y);

			void ShowGlyphs();
			friend class Map;
	};

struct MTerrain 
  {
    uint8 x,y;
    uint8 old;
    uint8 pri;
    int16 key;
  };

struct TerraRecord
  {
    int16 key;
    int16 Duration;
    int8  SaveDC;
    int8  DType;
    Dice  pval;
    rID   eID;
    hObj  Creator;      // ww: if a wall of fire burns something to death,
                        // who gets the XP? 
  };

struct EncMember
  {
    rID mID, tID, tID2, tID3,
        iID, pID, hmID, htID, htID2;
    uint16 Flags, Align; hObj hMon;
    int8 Part; int8 xxx; 
  };
  
#define MAX_ENC_MEMBERS  100

class Map: public Object
  {
   friend class Magic;
   friend void Game::MonsterEvaluation(); 
   friend void Game::MonsterEvaluationCR(); 
   friend void Game::WeaponEvaluation(); 
	 private:
	 	int16 sizeX,sizeY;
    int16 nextAvailableTerraKey;
    bool inGenerate;

    LocationInfo *Grid; 
    static Fraction slope1, slope2, slope3, test;
    rID RegionList[256]; uint8 SpecialDepths[16];
    rID TerrainList[256]; int16 CurrThing;
    

    /* Stuff used during map generation */
	 	static int32 TotalCorridorLength;
    static int32 RoomsTouched[32];
    static int32 PanelsDrawn[32];
    static rID   RoomWeights[1024];
    static rID   CorridorWeights[1024];
    static rID   VaultWeights[1024];
    static rID   StreamWeights[1024];
    static uint8 OpenX[2048], OpenY[2048];
    static int16 OpenC;
    static uint16 Corners[32], nCorners;
    static uint16 Centers[16], nCenters;
    static Rect cPanel, cRoom;
    static uint32 Con[LAST_DUNCONST];
    static uint32 RM_Weights[RM_LAST*2];
    static uint32 RC_Weights[RC_LAST*2];
    static int8  panelsX, panelsY, disX, disY, mLuck;
    static uint8 *FloodArray, *FloodStack, *EmptyArray;
    static uint8 PlayerStartX, PlayerStartY;
    static bool IndividualRooms;

    /* Encounter Gen stuff */
    static EncMember EncMem[MAX_ENC_MEMBERS];
    static int16 cEncMem;
    static int32 uniformKey[50];
    static rID   uniformChoice[50];
    static int16 cUniform;
    
    /* Print Queues */
    int8 QueueStack[32], QueueSP;

   public:
    rID dID; int16 PercentSI;
    bool inDaysPassed;
    int16 Depth, Level, EnterX, EnterY;
    int8 SpecialsLevels[64]; int16 Day;
	 	Overlay ov;
    NArray<hObj,1000,10> Things;
    // ww: I'm so tempted to put a canary word here --
    // it really looks like something is scribbling over
    // "Fields" ...
    OArray<Field,10,5> Fields;
    OArray<MTerrain,0,10>   TerraXY;
    OArray<TerraRecord,0,5> TerraList;
    NArray<uint16,20,20> TorchList;
    int16 FieldCount;
    hObj pl[4];
    int16 PlayerCount, BreedCount;
    int16 PreviousAuguries;
    /* Generic Class Functions */
	  Map();
    ~Map();
    virtual void Dump();
    virtual String & Name(int16 Flags=0);
    virtual String & Describe(Player *p);
    virtual void NotifyGone(hObj h);
		EvReturn Event(EventInfo &e);

    /* Basic Queries */
    const inline bool InBounds(int16 x,int16 y)
	     { return (x>=0) && (y>=0) && (x<sizeX) && (y<sizeY); }
	     
	  bool isGenerating() { return inGenerate; }
	     
		Glyph GlyphAt(int16 x,int16 y)
		  { return At(x,y).Glyph; }
		void SetAt(int16,int16,LocationInfo &);
		inline LocationInfo & At(int16 x,int16 y)
		  { ASSERT(InBounds(x,y));
        if (!InBounds(x,y))
          return Grid[0];
        return Grid[y*sizeX + x]; }
		rID inline RegionAt(int16 x,int16 y)
		  { ASSERT(InBounds(x,y));
        if (!InBounds(x,y))
          return RegionList[At(0,0).Region];
        return RegionList[At(x,y).Region]; }
		rID inline TerrainAt(int16 x, int16 y)
      { ASSERT(InBounds(x,y));
        if (!InBounds(x,y))
          return TerrainList[At(0,0).Terrain];
        return TerrainList[At(x,y).Terrain]; }
    rID PTerrainAt(int16 x, int16 y, Creature * watcher);
    void TerrainDisbelief(int16 x, int16 y, Creature * watcher);
    uint32 inline FlagsAt(int16 x, int16 y)
      { ASSERT(InBounds(x,y));
        return 0; }
    bool   SolidAt(int16 x, int16 y);
    bool   TreeAt(int16 x, int16 y);
    bool   OpaqueAt(int16 x, int16 y);
    bool   ObscureAt(int16 x, int16 y);
    rID   StickyAt(int16 x, int16 y);
    bool besideWall(int16 x,int16 y);
    int16 SizeX() { return sizeX; }
		int16 SizeY() { return sizeY; }
    Thing* GetAt(int16 x, int16 y, int16 t, bool first);
    
    Creature* FCreatureAt(int16 x, int16 y) 
      { return (Creature*)GetAt(x,y,T_CREATURE,true); }
    Creature* NCreatureAt(int16 x, int16 y) 
      { return (Creature*)GetAt(x,y,T_CREATURE,false); }
    bool      MCreatureAt(int16 x, int16 y) 
      { GetAt(x,y,T_CREATURE,true);
        return GetAt(x,y,T_CREATURE,false) != NULL; }

    Trap* FTrapAt(int16 x, int16 y) 
      { return (Trap*)GetAt(x,y,T_TRAP,true); }
    Trap* NTrapAt(int16 x, int16 y) 
      { return (Trap*)GetAt(x,y,T_TRAP,false); }
    bool  MTrapAt(int16 x, int16 y) 
      { GetAt(x,y,T_TRAP,true);
        return GetAt(x,y,T_TRAP,false) != NULL; }

    Container* FChestAt(int16 x, int16 y) 
      { return (Container*)GetAt(x,y,T_CHEST,true); }
    Container* NChestAt(int16 x, int16 y) 
      { return (Container*)GetAt(x,y,T_CHEST,false); }
    bool       MChestAt(int16 x, int16 y) 
      { GetAt(x,y,T_CHEST,true);
        return GetAt(x,y,T_CHEST,false) != NULL; }

    Item* FItemAt(int16 x, int16 y) 
      { return (Item*)GetAt(x,y,T_ITEM,true); }
    Item* NItemAt(int16 x, int16 y) 
      { return (Item*)GetAt(x,y,T_ITEM,false); }
    bool  MItemAt(int16 x, int16 y) 
      { GetAt(x,y,T_ITEM,true);
        return GetAt(x,y,T_ITEM,false) != NULL; }

    Feature* FFeatureAt(int16 x, int16 y) 
      { return (Feature*)GetAt(x,y,T_FEATURE,true); }
    Feature* NFeatureAt(int16 x, int16 y) 
      { return (Feature*)GetAt(x,y,T_FEATURE,false); }
    bool     MFeatureAt(int16 x, int16 y) 
      { GetAt(x,y,T_FEATURE,true);
        return GetAt(x,y,T_FEATURE,false) != NULL; }

    Door* FDoorAt(int16 x, int16 y) 
      { return (Door*)GetAt(x,y,T_DOOR,true); }
    Door* NDoorAt(int16 x, int16 y) 
      { return (Door*)GetAt(x,y,T_DOOR,false); }
    bool  MDoorAt(int16 x, int16 y) 
      { GetAt(x,y,T_DOOR,true);
        return GetAt(x,y,T_DOOR,false) != NULL; }

    Thing* FirstAt(int16 x, int16 y) 
      { return (Thing*)GetAt(x,y,T_THING,true); }
    Thing* NextAt(int16 x, int16 y) 
      { return (Thing*)GetAt(x,y,T_THING,false); }
    bool   MultiAt(int16 x, int16 y) 
      { GetAt(x,y,T_CREATURE,true);
        return GetAt(x,y,T_CREATURE,false) != NULL; }
    bool  PileAt(int16 x, int16 y)
      { GetAt(x,y,T_ITEM,true);
        return GetAt(x,y,T_ITEM,false) != NULL; }
    
		bool   PMultiAt(int16 x, int16 y, Creature *POV);
    bool   OpenAt(int16 x,int16 y);
		Feature* KnownFeatureAt(int16 x,int16 y);
    bool   LightAt(int16 x, int16 y);
    bool   BrightAt(int16 x, int16 y);
    bool   FallAt(int16 x, int16 y);
    
    Thing* FirstThing()
      { CurrThing = 0; return oThing(Things[CurrThing]); }
    Thing* NextThing()
      { return oThing(Things[++CurrThing]); }

    /* Dungeon-Generation Functions */
    void WriteAt(Rect &r,int16 x, int16 y,rID terID,rID regID,int32 Pri, bool Force=false);
    void WriteBox(Rect &r,rID regID);
    void WriteRoom(Rect &r,rID regID);
    void WriteCircle(Rect &r, rID regID);
    void WriteWalls(Rect &r,rID regID);
    void WriteStreamer(Rect &r, uint8 sx, uint8 sy, Dir d, rID regID);
    void WriteLifeCave(Rect &r, rID regID);
    void WriteCastle(Rect &r, rID regID);
    void WriteRCavern(Rect &r, rID regID);
    void WriteOctagon(Rect &r,rID regID);
    void WriteDiamond(int16 x, int16 y, rID regID);
    void WriteDestroyed(Rect &r,rID regID);
    void WriteMaze(Rect &r,rID regID, int16 inset_count=0,...);
    void WriteCross(Rect &r, rID regID);
    void WriteMap(Rect &r,rID mID);
    void WriteBlobs(Rect &r, rID regID, rID bID);

    void LoadFixedMap(rID mID, int16 _Depth, Map *Above, int8 _Luck);
    void Generate(rID dunID, int16 Depth, Map *Above,int8 Luck);
    void GeneratePrompt();
    void DrawPanel(uint8 x, uint8 y,rID regID=0);
    void PopulatePanel(Rect &r, uint16 extraFlags=0); /* HACKFIX */
    void FurnishArea(Rect &r);
    void LightPanel(Rect &r, rID regID);
    void PopulateChest(Container *c); 
    int16 isTorched(uint8 x, uint8 y, int16 t);
    void calcLight(Rect &r);
    uint16 Tunnel(uint8 sx, uint8 sy,uint8 dx, uint8 dy, 
                  uint8 TFlags, Dir StartDir,int8 TType);
    Dir  CorrectDir(int16 cx, int16 cy, int16 dx,int16 dy,Dir Curr);
    int32 FloodConnectA(int16 x, int16 y,int32 fCount);
    int32 FloodConnectB(int16 x, int16 y);
    void MakeDoor(uint8 x,uint8 y, rID fID);
    void MakeSecretDoor(uint8 x,uint8 y, rID fID);
    void SetGlyphAt(int16 x, int16 y, Glyph g)
      { if (g & 0x00FF) 
          At(x,y).Glyph &= 0xFF00;
        if (g & 0xF000)  
          At(x,y).Glyph &= 0x0FFF;
        if (g & 0x0F00)  
          At(x,y).Glyph &= 0xF0FF;
        At(x,y).Glyph |= g; }

    bool FindOpenAreas(Rect r, rID regID=0, int16 Flags=0);
    void SetRegion(Rect r, rID regID);
    uint16 GetOpenXY();

    EvReturn thEnGen(rID xID, uint32 fl, int8 CR, uint16 enAlign);
    EvReturn thEnGenXY(rID xID, uint32 fl, int8 CR, uint16 enAlign, int16 x, int16 y);
    EvReturn thEnGenSummXY(rID xID, uint32 fl, int8 CR, uint16 enAlign, Creature* crea, int16 x, int16 y);
    EvReturn thEnGenMon(rID xID, rID mID, uint32 fl, int8 CR, uint16 enAlign);
    EvReturn thEnGenMonXY(rID xID, rID mID, uint32 fl, int8 CR, uint16 enAlign, int16 x, int16 y);
    EvReturn thEnGenMType(rID xID, int16 mt, uint32 fl, int8 CR, uint16 enAlign);
    EvReturn thEnGenMTypeXY(rID xID, int16 mt, uint32 fl, int8 CR, uint16 enAlign, int16 x, int16 y);
    EvReturn thEnGenMonSummXY(rID xID, rID mID, uint32 fl, int8 CR, uint16 enAlign, Creature *crea, int16 x, int16 y);
    EvReturn thEnGenMTypeSummXY(rID xID, int16 mt, uint32 fl, int8 CR, uint16 enAlign, Creature *crea, int16 x, int16 y)
      { return thEnGenMonSummXY(xID,mt,fl,CR,enAlign,crea,x,y); }
    EvReturn rtEnGen(EventInfo &e, rID xID, uint32 fl, int8 CR, uint16 enAlign);
    
    
    void  PlaceEncounter(int16 ex=0, int16 ey=0, Creature*creator=NULL); 
    Creature * GetEncounterCreature(int32 i);
    String & PrintEncounter();
    /* Vision Functions */
    bool LineOfSight(int16 sx, int16 sy, int16 tx, int16 ty, Creature *c);
    bool LineOfVisualSight(int16 sx, int16 sy, int16 tx, int16 ty, Creature *c);
    bool LineOfFire(int16 sx, int16 sy, int16 tx, int16 ty, Creature *c);
    void VisionThing(int16 pn, Creature *c, bool do_clear);
    bool VisionPath(int8 pn, 
        const int16 sx, const int16 sy, 
        const int16 tx, const int16 ty, 
        Creature *c, const int16 SightRange, const int16 LightRAnge,
        const int16 ShadowRange);
    bool BlindsightVisionPath(int8 pn, 
        const int16 sx, const int16 sy, 
        const int16 tx, const int16 ty, 
        Creature *c, const int16 BlindRange);
    bool MarkAsSeen(const int8 pn, const int16 lx, const int16 ly,
        const int16 dist, const int16 SightRange = 1, 
        const int16 LightRange = 1, const int16 ShadowOrBlindRange = 1);
		void Update(int16 x,int16 y);
    void VUpdate(int16 x, int16 y);

    /* Field Functions */
    void UpdateFields();
    void NewField(int32 FType, int16 x, int16 y, uint8 rad,
      Glyph Img, int16 Dur = -1, rID eID = 0, Creature* Creator=NULL);
    void RemoveField(Field *);
    void RemoveEffField(rID eID);
    void RemoveFieldFrom(hObj h);
    void RemoveEffFieldFrom(rID eID, hObj h);
    EvReturn MoveField(Field *f, int16 x,int16 y, bool is_walk);
    inline bool FieldAt(int16 x,int16 y, uint32 FType = -1);
    inline Glyph FieldGlyph(int16 x,int16 y, Glyph og);
    bool DispelField(int16 x, int16 y, uint32 FType, rID eID, int16 clev);
		
    /* Message Queues */
    void SetQueue(int16 Queue);
    void UnsetQueue(int16 Queue); 
    void PrintQueue(int16 Queue);
    void EmptyQueue(int16 Queue);
    int16 QueueNum() { return QueueStack[QueueSP]; }

    /* Terraform Support */
    int8 GetTerraDC(int16 x,int16 y);
    Creature * GetTerraCreator(int16 x,int16 y);
    int8 GetTerraDType(int16 x,int16 y);
    int16 GetTerraDmg(int16 x, int16 y);
    void RemoveTerra(int16 key);
    void RemoveTerraXY(int16 x,int16 y,rID xID=0);
    void UpdateTerra();
    void WriteTerra(int16 x,int16 y,rID tID);

    void MakeNoiseXY(int16 x, int16 y, int16 radius); 

    /* Djikstra's Shortest Path Algorithm */
    void   PQInsert(uint16 Node, int16 Weight);
    int32  PQPeekMin();
    bool   PQPopMin();
    bool   ShortestPath(uint8 sx, uint8 sy, uint8 tx, uint8 ty,
                        Creature *c, int32 dangerFactor,
                        uint16 *ThePath = NULL); 
#define DF_ALL_SAFE             0
#define DF_IGNORE_TRAPS         1
#define DF_IGNORE_TERRAIN       2

    uint16 PathPoint(int16 n);
    uint16 RunOver(uint8 x, uint8 y, bool memonly, Creature *c,
                   int32 dangerFactor, bool Incor, bool Meld);

    /* Encounter Generation */
    EvReturn enGenerate(EventInfo &e);
    EvReturn enGenPart(EventInfo &e);
    EvReturn enBuildMon(EventInfo &e);
    EvReturn enSelectTemps(EventInfo &e);
    EvReturn enChooseMID(EventInfo &e);
    EvReturn enChooseTemp(EventInfo &e);
    EvReturn enGenMount(EventInfo &e);
    EvReturn enGenAlign(EventInfo &e);
    
    void  enUniformAdd(int32 key, rID choice);
    rID   enUniformGet(int32 key);
    void  enAddMon(EventInfo &e);
    void  enAddTemp(EventInfo &e, rID tID);
    void  enAddMountTemp(EventInfo &e, rID tID);
    void  enCalcCurrPartXCR(EventInfo &e);
    bool  enTemplateOk(EventInfo &e, rID tID, bool force=false);    
    void  enWarn(const char*msg,...);

    void GenEncounterStats(Term *t);

    /* Miscellaneous Stuff */
    bool Filled(int16 x,int16 y);
    void Load(rID mID);
    void RegisterPlayer(hObj h);
    void DaysPassed();
    void ResetImages();
    
    friend class Term;
    ARCHIVE_CLASS(Map,Object,r)
      r.Block((void**)&Grid,sizeof(LocationInfo)*sizeX*sizeY);
      Things.Serialize(r);
      Fields.Serialize(r);
      TerraXY.Serialize(r);
      TerraList.Serialize(r);
      TorchList.Serialize(r);
    END_ARCHIVE


	};

extern Map* TheMainMap;

#define ADDED_SIZE      128

                                

#define StatiIter(targ)                                \
  { Status *S; int16 __i; targ->__Stati.Nested++;      \
    ASSERT (targ->__Stati.Nested > 0);                 \
    for (__i=0,S=SN(0,targ);S;S=SN(++__i,targ)) {      \
      if (!S->Nature) continue;
    
#define StatiIterEnd(targ)                             \
      }                                                \
    ASSERT(targ->__Stati.Nested > 0);                  \
    targ->__Stati.Nested--;                            \
    if ((targ->__Stati.Removed ||                      \
         targ->__Stati.szAdded) &&                     \
          !targ->__Stati.Nested)                       \
      targ->_FixupStati();                             \
    }
    
#define StatiIter_RemoveCurrent(targ)                 \
   { if (S->eID && RES(S->eID)->Type == T_TEFFECT)    \
       targ->RemoveEffStati(S->eID, EV_REMOVED);      \
     else {                                           \
     Status s; s = *S;                                \
     targ->__Stati.Removed++;                         \
     S->Nature = 0; S->eID = 0; FixupBackrefs(S,targ);\
     targ->StatiOff(s); } }                          

#define StatiIter_IfNothingRemoved(targ)              \
    if (targ->__Stati.Removed == 0)

/* HACKFIX */
#define StatiIter_DispelCurrent(targ)                 \
   { if (S->eID && RES(S->eID)->Type == T_TEFFECT)    \
       targ->RemoveEffStati(S->eID, EV_DISPELLED);    \
     else {                                           \
     Status s; s = *S;                                \
     targ->__Stati.Removed++;                         \
     S->Nature = 0; S->eID = 0; FixupBackrefs(S,targ);\
     targ->StatiOff(s); } }                          
    

#define StatiIter_ElapseCurrent(targ)                  \
   { if (S->eID && RES(S->eID)->Type == T_TEFFECT)     \
       targ->RemoveEffStati(S->eID,EV_ELAPSED);        \
     else {                                           \
     Status s; s = *S;                                \
     targ->__Stati.Removed++;                         \
     S->Nature = 0; S->eID = 0; FixupBackrefs(S,targ);\
     targ->StatiOff(s,true); } }           
                                
                                
#define is_adj(n) ((n) >= ADJUST && (n) <= ADJUST_LAST)


#define StatiIterNature(targ,n)                        \
  { Status *S; int16 __i, __c=0; targ->__Stati.Nested++; \
    if (targ->__Stati.Idx &&                           \
        targ->__Stati.Idx[(n)] != 255)                 \
      for (__i=targ->__Stati.Idx[(n)];                 \
              __i<targ->__Stati.Last &&                \
              ((targ->__Stati.S[__i].Nature==n) ||     \
              (targ->__Stati.S[__i].Nature==0));__i++) \
      __c++;                                           \
    for (__i=0,S=SNBN(0,targ,__c,n);S;                 \
               S=SNBN(++__i,targ,__c,n)) {             \
      if (S->Nature != (n)) continue;

#define StatiIterAdjust(targ)                          \
  { Status *S; int16 __i, __c=0; targ->__Stati.Nested++; \
    if (targ->__Stati.Idx &&                           \
        targ->__Stati.Idx[ADJUST_IDX] != 255)          \
      for (__i=targ->__Stati.Idx[ADJUST_IDX];          \
              __i<targ->__Stati.Last &&                \
              ((is_adj(targ->__Stati.S[__i].Nature)) || \
              (targ->__Stati.S[__i].Nature==0));__i++)  \
      __c++;                                           \
    for (__i=0,S=SNBN(0,targ,__c,ADJUST_IDX);S;        \
               S=SNBN(++__i,targ,__c,ADJUST_IDX)) {    \
      if (!is_adj(S->Nature)) continue;

/*
#define StatiIterNature(targ,n)                        \
  { Status *S; int16 __i; targ->__Stati.Nested++;      \
    ASSERT (targ->__Stati.Nested > 0); \
    for (__i=0,S=SN(0,targ);S;S=SN(++__i,targ)) {      \
      if (S->Nature != n) continue;

#define StatiIterAdjust(targ)                          \
  { Status *S; int16 __i; targ->__Stati.Nested++;      \
    for (__i=0,S=SN(0,targ);S;S=SN(++__i,targ)) {      \
      if (!is_adj(S->Nature)) continue;
*/

#define StatiIterBreakout(targ,ret)                    \
    { ASSERT(targ->__Stati.Nested > 0);                \
      targ->__Stati.Nested--;                          \
      if ((targ->__Stati.Removed ||                    \
           targ->__Stati.szAdded) &&                   \
            !targ->__Stati.Nested)                     \
        targ->_FixupStati();                           \
      ret;                                             \
    }
  
                    
// ww: we're intentionally not using an Array here ...
typedef struct StatiCollection {
    Status* S;
    Status* Added; 
    int16   szAdded;
    uint8 * Idx;        // always of size LAST_STATI if allocated 
    int16   Last;       // last used elt in array "S"
    int16   Allocated;  // allocated size of array "S"
    int16   Removed;    // # removed in this iteration sweep
    int8   Nested;      // are we removing inside someone elses remove loop?
    void Serialize(Registry &r) {
      ASSERT(Added == NULL);
      if (Allocated) {
        r.Block((void**)(&S),sizeof(S[0])*Allocated);  
        r.Block((void**)(&Idx),sizeof(Idx[0])*LAST_STATI);  
      }
    } 

    void Initialize() {
      Last = 
        Removed = 
        Allocated = 
        Nested = 
        szAdded = 0;
      S = NULL;
      Idx = NULL;
      Added = NULL;
    } 

    void CopyTo(StatiCollection * dest) {
      ASSERT(Added == NULL);
      if (dest->Allocated) {
        free(dest->S);
        free(dest->Idx);
      }
      dest->Last = Last;
      dest->Allocated = Allocated;
      ASSERT(Removed == 0); 
      dest->Removed = Removed;
      if (Allocated) { 
        dest->S = (Status *)malloc(sizeof(S[0]) * Allocated);
        dest->Idx = (uint8 *)malloc(sizeof(Idx[0]) * LAST_STATI);
        memcpy(dest->S,S,sizeof(S[0]) * Allocated);
        memcpy(dest->Idx,Idx,sizeof(Idx[0]) * LAST_STATI);
      }
    } 

} StatiCollection; 

class Thing: public Object
	{
    ARCHIVE_CLASS(Thing,Object,r)
      if (isSave)
        *((long*)&hm) = m ? m->myHandle : 0; 
      else
        m = oMap((long)hm);
      Named.Serialize(r);
      __Stati.Serialize(r);
      backRefs.Serialize(r); 
    END_ARCHIVE
		public:
      Map* m;
      hObj Next, hm;
			int16 x,y;
			Glyph Image;
			int16 Timeout;
      int16 StoredMovementTimeout;
      uint32 Flags;
      String Named;
      StatiCollection __Stati; 
      NArray<hObj,10,20> backRefs;
		 public:
      Thing(Glyph _Image,int8 _Type);
      virtual String & Name(int16 Flags=0) { return *tmpstr("<Thing>"); }
      virtual void SetImage() { }
      virtual void Initialize(bool in_play = false) { }
      virtual void __IPrint(const char*, va_list ap) {}
      virtual void IPrint(const char*,...) {}
      virtual void IDPrint(const char*,const char*,...);
      virtual const char* His() { return "Its"; }
      virtual const char* his() { return "its"; }
      virtual const char* Him() { return "It"; }
      virtual const char* him() { return "it"; }
      virtual const char* He() { return "It"; }
      virtual const char* he() { return "it"; }

      virtual int8 Material() { return MAT_EMPTYNESS; } 
      virtual bool isMetallic();
      virtual bool isWooden();
      virtual bool isGhostTouch();
      virtual bool isOrganic();
      virtual bool isCorporeal();

      virtual bool isPlural() { return false; }
      virtual bool isRealTo(Creature*);
      virtual void DisbeliefCheck(Creature *watcher = NULL);
      virtual uint16 getIllusionFlags();
      virtual int16 getIllusioncraft();
            
      
      virtual bool isShadowShape() { return false; }
      virtual bool isIllusion() { return HasStati(ILLUSION); }
      virtual bool isShade() { if (HasStati(ILLUSION))
                                 if (getIllusionFlags() & IL_SHADE)
                                   return true;
                               return false; }
      virtual int16 getShadePercent() { uint32 fl = getIllusionFlags();
                                        return (fl & IL_SHADE10 ? 10 : 0) +
                                               (fl & IL_SHADE20 ? 20 : 0) +
                                               (fl & IL_SHADE40 ? 40 : 0); }
      virtual bool IllusionLOSCheck(bool test, int16 ix=-1, int16 iy=-1, int16 cx=-1, int16 cy=-1);
      virtual int16 onPlane();
      int16 GetX() {return x;}
      int16 GetY() {return y;}
      virtual bool isDead() { return Flags & F_DELETE; }
      virtual void Dump();
			void Show();
			void Move(int16,int16, bool is_walk=false);
      virtual void MoveDepth(int16 NewDepth, bool safe=false);
      Thing* ProjectTo(int16, int16, int8 range=127);
      void PlaceNear(int16, int16);
      void PlaceAt(Map* _m,int16 _x,int16 _y, bool share_square=false);
      void PlaceOpen(Map* _m)
        { uint16 xy = _m->GetOpenXY();
          PlaceAt(_m,xy%256,xy/256); }
      virtual void Remove(bool isDelete);
      virtual void NotifyGone(hObj h);
      void BoostRetry(int16 sk, Creature *c);
      Thing* ProjectDir(Dir d, int8 range=127)
        {
          switch (d) {
            case NORTH:     return ProjectTo(x,y-1000,range);
            case SOUTH:     return ProjectTo(x,y+1000,range);
            case EAST:      return ProjectTo(x+1000,y,range);
            case WEST:      return ProjectTo(x-1000,y,range);
            case NORTHEAST: return ProjectTo(x+1000,y-1000,range);
            case NORTHWEST: return ProjectTo(x-1000,y-1000,range);
            case SOUTHEAST: return ProjectTo(x+1000,y+1000,range);
            case SOUTHWEST: return ProjectTo(x-1000,y+1000,range);
            default: Fatal("Illegal Direction in ProjectDir!");
            }
        }
      inline Dir DirTo(Thing *t)
        {
          if (t->m != m)
            return CENTER;
          if (t->GetX() == x) {
            if (t->GetY() > y) return SOUTH;
            if (t->GetY() < y) return NORTH;
            return CENTER; }
          if (t->GetX() < x) {
            if (t->GetY() > y) return SOUTHWEST;
            if (t->GetY() < y) return NORTHWEST;
            return WEST; }
          if (t->GetY() > y) return SOUTHEAST;
          if (t->GetY() < y) return NORTHEAST;
          return EAST;
        }
      inline Dir DirTo(int16 tx, int16 ty)
        {
          if (tx == x) {
            if (ty > y) return SOUTH;
            if (ty < y) return NORTH;
            return CENTER; }
          if (tx < x) {
            if (ty > y) return SOUTHWEST;
            if (ty < y) return NORTHWEST;
            return WEST; }
          if (ty > y) return SOUTHEAST;
          if (ty < y) return NORTHEAST;
          return EAST;
        }

			virtual void DoTurn();
      int16 DistFrom(Thing*);
      inline Field* inField(int32 FType = -1)
        {
          if (x < 0 || y < 0 || !m)
            return false;
          if (!m->At(x,y).hasField)
            return NULL;
          for(int i=0;m->Fields[i];i++)
            if (m->Fields[i]->inArea(x,y))
              if ((FType == -1) || (FType & m->Fields[i]->FType))
                return m->Fields[i];
          return NULL;
          /*
          for(int16 i=0;m->Fields[i];i++)
            if (abs(m->Fields[i]->cx - x) <= m->Fields[i]->rad)
              if (abs(m->Fields[i]->cy - y) <= m->Fields[i]->rad)
                if (m->Fields[i]->inArea(x,y))
                  if (FType == -1 || FType == m->Fields[i]->FType)
                    return m->Fields[i];
          return NULL;
          */
        }
      /*inline*/ int16 DistFromPlayer();
			EvReturn Event(EventInfo &e);
			virtual EvReturn FieldOn(EventInfo &e)
        { return NOTHING; }
			virtual EvReturn FieldOff(EventInfo &e)
        { return NOTHING; }
      virtual bool inGroup(hObj h);

		  void GainPermStati(int16 n,Thing *t,int8 Cause,int16 Val=-1,int16 Mag=-1, rID eID = 0, int8 clev=0);
		  void GainTempStati(int16 n,Thing *t,int16 Duration,int8 Cause,int16 Val=-1,int16 Mag=-1, rID eID = 0, int8 clev=0);
      void CopyStati(Thing *t, int16 n);
      void RemoveStati(int16 n,int8 Cause=-1, int16 Val=-1,int16 Mag=-1, Thing *t=NULL);		  
      void RemoveEffStati(rID xID, int16 ev = EV_REMOVED, int16 butNotNature = 0);
      void RemoveStatiFrom(Thing *t, int16 ev=EV_REMOVED);
      void RemoveStatiSource(uint8 ss);
      void RemoveOnceStati(int16 n, int16 val);
      void UpdateStati();
      void CleanupRefedStati();
      inline bool HasStati(int16 n,int16 Val=-1,Thing *t=NULL);
      inline bool HasEffStati(int16 n,rID eID, int16 Val=-1, Thing *t=NULL);
      bool HasEffField(rID eID);
      bool        HasStatiFrom(Thing *t);
      Status* GetStati(int16 n,int16 Val=-1,Thing *t=NULL);
      Status* GetEffStati(int16 n,rID xID, int16 Val=-1, Thing *t=NULL);
      inline int16 CountStati(int16 n,int16 Val=-1,Thing *t=NULL);
      inline int16 CountEffStati(int16 n,rID xID, int16 Val=-1);
      
      int16 GetStatiVal(int16 n,Thing *t=NULL)
        { Status *s = GetStati(n,-1,t);
          return s ? s->Val : -1; }      
      int16 GetStatiMag(int16 n,int16 Val=-1,Thing *t=NULL)
        { Status *s = GetStati(n,Val,t);
          return s ? s->Mag : 0; }
      uint8 GetStatiCause(int16 n,int16 Val=-1,Thing *t=NULL)
        { Status *s = GetStati(n,Val,t);
          return s ? s->Source : 0; }
      Thing* GetStatiObj(int16 n,int16 Val=-1,Thing *t=NULL)
        { Status *s = GetStati(n,Val,t);
          return s && isValidHandle(s->h) ? oThing(s->h) : 0; }
      int16 GetStatiDur(int16 n,int16 Val=-1,Thing *t=NULL)
        { Status *s = GetStati(n,Val,t);
          return s ? s->Duration : 0; }
      rID GetStatiEID(int16 n,int16 Val=-1,Thing *t=NULL)
        { Status *s = GetStati(n,Val,t);
          return s ? s->eID : 0; }
      int16 GetStatiCLev(int16 n,int16 Val=-1,Thing *t=NULL)
        { Status *s = GetStati(n,Val,t);
          return s ? s->CLev : 0; }

      void SetStatiVal(int16 n,Thing *t,int16 Val)
        { Status *s = GetStati(n,-1,t);
          if (s) s->Val = Val; }
      void SetStatiMag(int16 n,int16 Val,Thing *t,int16 Mag)
        { Status *s = GetStati(n,Val,t);
          if (s) s->Mag = Mag; }
      void SetStatiCause(int16 n,int16 Val,Thing *t,uint8 Cause)
        { Status *s = GetStati(n,Val,t);
          if (s) s->Source = Cause; }
      void SetStatiObj(int16 n,int16 Val,Thing *t)
        { Status *s = GetStati(n,Val);
          if (s) s->h = t ? t->myHandle : 0; }
      void SetStatiDur(int16 n,int16 Val,Thing *t,int16 Dur)
        { Status *s = GetStati(n,Val,t);
          if (s) s->Duration = Dur; }
      void SetStatiEID(int16 n,int16 Val,Thing *t,rID eID)
        { Status *s = GetStati(n,Val,t);
          if (s) s->eID = eID; }
      void IncStatiMag(int16 n,int16 Source,int16 Val,Thing *t,int16 Inc)
        { Status *s = GetStati(n,Val,t);
          if (!s)
            {
              GainPermStati(n,t,Source,Val,Inc,0,0);
              s = GetStati(n,Val,t);
            }
          else
            s->Mag += Inc; }

      int16 GetEffStatiVal(int16 n,rID xID, int16 Val=-1, Thing *t=NULL)
        { Status *s = GetEffStati(n,xID,Val,t);
          return s ? s->Val : -1; }      
      int16 GetEffStatiMag(int16 n,rID xID, int16 Val=-1, Thing *t=NULL)
        { Status *s = GetEffStati(n,xID,Val,t);
          return s ? s->Mag : 0; }
      uint8 GetEffStatiCause(int16 n,rID xID, int16 Val=-1, Thing *t=NULL)
        { Status *s = GetEffStati(n,xID,Val,t);
          return s ? s->Source : 0; }
      Thing* GetEffStatiObj(int16 n,rID xID, int16 Val=-1, Thing *t=NULL)
        { Status *s = GetEffStati(n,xID,Val,t);
          return s ? oThing(s->h) : 0; }
      int16 GetEffStatiDur(int16 n,rID xID, int16 Val=-1, Thing *t=NULL)
        { Status *s = GetEffStati(n,xID,Val,t);
          return s ? s->Duration : 0; }
      int16 GetEffStatiCLev(int16 n,rID xID, int16 Val=-1, Thing *t=NULL)
        { Status *s = GetEffStati(n,xID,Val,t);
          return s ? s->CLev : 0; }

      void SetEffStatiVal(int16 n,rID xID,int16 Val, Thing *t=NULL)
        { Status *s = GetEffStati(n,xID,-1,t);
          if (s) s->Val = Val; }
      void SetEffStatiMag(int16 n,rID xID,int16 Mag, int16 Val=-1, Thing *t=NULL)
        { Status *s = GetEffStati(n,xID,Val,t);
          if (s) s->Mag = Mag; }
      void SetEffStatiCause(int16 n,rID xID,uint8 Cause, int16 Val=-1, Thing *t=NULL)
        { Status *s = GetEffStati(n,xID,Val,t);
          if (s) s->Source = Cause; }
      void SetEffStatiObj(int16 n,rID xID,Thing *t, int16 Val=-1)
        { Status *s = GetEffStati(n,xID,Val);
          if (s) s->h = t ? t->myHandle : 0; }
      void SetEffStatiDur(int16 n,rID xID,int16 Dur, int16 Val=-1, Thing *t=NULL)
        { Status *s = GetEffStati(n,xID,Val,t);
          if (s) s->Duration = Dur; }
      void IncEffStatiMag(int16 n,rID xID,int16 Source,Thing *t,int16 Inc, int16 Val=-1)
        { Status *s = GetEffStati(n,xID,Val,t);
          if (!s)
            {
              GainPermStati(n,t,Source,Val == -1 ? 0 : Val,Inc,xID,0);
              s = GetEffStati(n,xID);
            }
          else
            s->Mag += Inc; }
 
      inline bool containsAllStatiFrom(Thing *other, const uint8 *IgnoreNatures);
      inline bool hasSameStati(Thing *t, const uint8 *IgnoreNatures)
        { return    containsAllStatiFrom(t,IgnoreNatures) &&
                 t->containsAllStatiFrom(this,IgnoreNatures); }
      inline int16 HighSourceCLev(uint8 src);
      inline bool HasStatiFromSource(uint8 src);      

      inline int16 SumStatiMag(int16 n,int16 Val=-1, Thing *t=NULL);
      inline int16 HighStatiMag(int16 n,int16 Val=-1, Thing *t=NULL);
      inline int16 SumStatiVal(int16 n, Thing *t=NULL);
      inline int16 XORStatiVal(int16 n, Thing *t=NULL);

      void _FixupStati();
      virtual void StatiOn(Status s) {}
      virtual void StatiOff(Status s, bool elapsed = false) {}
	};

inline Status * SN(int16 i, Thing *targ)
  {
    if (i < targ->__Stati.Last)
      return &(targ->__Stati.S[i]);
    else if (i - targ->__Stati.Last >= 
                 targ->__Stati.szAdded)
      return NULL;
    else
      return &(targ->__Stati.Added[i -
               targ->__Stati.Last]);
  }    

inline Status * SNBN(int16 i, Thing *targ, int16 count, int16 n)
  {
    if (i < count)
      return &(targ->__Stati.S[i + targ->__Stati.Idx[(n)]]);
    else if (i - count >= targ->__Stati.szAdded)
      return NULL;
    else 
      return &(targ->__Stati.Added[(i) - count]);
  }

inline void FixupBackrefs(Status *S, Thing *targ)
  {
    /*
    if (S->h && theRegistry->Exists(S->h))            
      if (!targ->HasStatiFrom(oThing(S->h)))          
        oThing(S->h)->backRefs.RemoveItem(            
                                targ->myHandle); 
    */
  }
