/* See the Incursion LICENSE file for copyright information.

     This file contains declarations for all of the global
   variables and functions used in Incursion. 
*/

class String;

void Fatal(const char*,...);
void Error(const char*,...);

void init_genrand(uint32_t s);
uint32_t genrand_int32(void);

int16 strcatf(char*,const char*,...);
inline int16 random(int16 mx);
inline const int16 dist(uint16 x1, uint16 y1, uint16 x2, uint16 y2);
inline Dir DirTo(int16 sx, int16 sy, int16 tx, int16 ty);
uint16 ResAlign(rID xID);
bool AlignConflict(uint16 al1, uint16 al2, bool strict=false);
bool godAllowsAlign(rID gID, uint16 align);
bool isLegalPersonTo(Creature *Actor, Creature *Victim);
int16 getChivalryBreach(EventInfo &e);
rID getAnimalName();
const char* GodPronoun(rID gID, bool poss);
bool effectGivesStati(rID eID);
Item* OneSomeAll(Item *it, Creature *cr, bool assume_some=false);
int16 getGodRelation(rID fromID, rID toID, bool * doWrath);
bool isExploreMode(Player *p);
extern const char* OptChoice(const char *text, int16 num);
void PurgeStrings();

void EncAddMon(rID mID, rID tID=0, rID tID2=0, rID tID3=0);

EvReturn Throw(int16 Ev, Object *p1=NULL, Object *p2=NULL, 
               Object*p3=NULL, Object *p4=NULL);
EvReturn ThrowVal(int16 Ev,int16 n, Object *p1=NULL, Object *p2
               =NULL, Object*p3=NULL, Object *p4=NULL);
EvReturn ThrowXY(int16 Ev,int16 x,int16 y, Object *p1=NULL, 
               Object *p2=NULL, Object*p3=NULL, Object *p4=NULL);

EvReturn ThrowDir(int16 Ev,Dir d,Object *p1=NULL, 
               Object *p2=NULL, Object*p3=NULL, Object *p4=NULL);
EvReturn ThrowField(int16 Ev,Field *f,Object *p1=NULL, 
               Object *p2=NULL, Object*p3=NULL, Object *p4=NULL);
EvReturn ThrowEff(int16 Ev,rID eID,Object *p1=NULL, Object *p2
               =NULL, Object*p3=NULL, Object *p4=NULL);
EvReturn ThrowEffDir(int16 Ev,rID eID,Dir d, Object *p1=NULL, Object *p2
               =NULL, Object*p3=NULL, Object *p4=NULL);
EvReturn ThrowEffXY(int16 Ev,rID eID,int16 x,int16 y, Object *p1=NULL, 
               Object *p2=NULL, Object*p3=NULL, Object *p4=NULL);
EvReturn ThrowLoc(int16 Ev,int16 x,int16 y,Object *p1=NULL, Object *p2
               =NULL, Object*p3=NULL,	Object *p4=NULL);
EvReturn ThrowDmg(int16 Ev,int16 DType,int16 DmgVal, const char*s,
               Object *p1=NULL, Object *p2=NULL,Object*p3=NULL,
	             Object *p4=NULL);
EvReturn ThrowTerraDmg(int16 Ev,int16 DType,int16 DmgVal, const char*s,
               Object *victim, rID terID); 
EvReturn ThrowDmgEff(int16 Ev,int16 DType,int16 DmgVal, const char*s,
               rID eID, Object *p1=NULL, Object *p2=NULL,Object*p3=NULL,
	             Object *p4=NULL);
EvReturn RedirectEff(EventInfo &e, rID eID, int16 Ev=EV_EFFECT);

EvReturn ReThrow(int16 ev, EventInfo &e);
void SetEvent(EventInfo &e, int16 Ev, Object *p1, Object *p2=NULL, 
                   Object*p3=NULL,Object *p4=NULL);
extern EventInfo EventStack[EVENT_STACK_SIZE];
extern int16     EventSP;
                   
int StringCompare(const void *a, const void *b);
String & DescribeSkill(int16 sk);

const char* __XPrint(Player *POV, const char *msg,va_list args);
const char* XPrint(const char*msg,...);
const char* PPrint(Player *POV, const char*msg, ...);
/* Display a message to all players who xperceive the event. */
void APrint(EventInfo &e, const char *msg1, ...);
/* Display one message to the acting player, and another to any players who xperceive the actor. */
void DPrint(EventInfo &e, const char *msg1,const char *msg2,...);
/* Display one message to the victim player, and another to any players who xperceive the victim. */
void VPrint(EventInfo &e, const char *msg1, const char *msg2, ...);
/* Display one message to the acting player, another to the victim, and yet another to any players who xperceive the actor/victim. */
void TPrint(EventInfo &e, const char *msg1, const char *msg2, const char *msg3,...);
void BDPrint(EventInfo &e, const char *msg1,...);
void BTPrint(EventInfo &e,const char *msg1,const char *msg2,...);
/* Display a message to all players (other than the actor) within the given range, who are able to hear. */
void Hear(EventInfo &e, int16 range, const char *msg1,...);
/* Display a message to all players for whom the source location or actor is visible or perceived respectively. */
void SinglePrintXY(EventInfo &e, const char*msg,...);

int8 MonHDType(int8 MType);
int8 MonGoodSaves(int8 MType);
int16 ResourceLevel(rID xID);
bool ResourceHasFlag(rID xID, int16 fl);
int16 MaxItemPlus(int16 MaxLev,rID eID);
Creature * InitShopkeeper(rID mID, int32 gold);
bool PossiblyPause(Term *T1, int x, int y, int timeout);
extern String & ItemNameFromEffect(rID eID);
extern String & DescribeSkill(int16 sk);
extern String & DescribeFeat(int16 ft);

bool isSimilarDir(Dir d, Dir d2);

extern int16 Silence;
extern int16 __spentHours;

extern int16 LastSkillCheckResult;

extern Game* theGame;
extern Registry* theRegistry;
extern Registry MainRegistry, ResourceRegistry;

extern rID Candidates[2048];
extern int16 nCandidates;

extern int16 FavEnemies[], MonGroupRarities[];
extern uint8 BlastColourSets[];


extern int16 s_racial, s_enhance, s_feat, s_domain, s_item, s_train, s_syn,
      s_armour, s_comp, s_circ, s_inh, s_size, s_kit, s_focus, s_ins;    
extern int16 Studies[9][4];
extern int32 EncumValues[4];

extern "C" int Preprocess(const char*,const char*);
extern int8 DirX[],DirY[];
extern int8 ManaMultiplier[],
            GoodSave[], 
            PoorSave[], 
            ChargeBonusTable[10][30],
            ArmourTable[33][16],
            AbsorbTable[17][16];

extern const char *KeyBindings,
                  *GlyphLegend1,
                  *GlyphLegend2,
                  *AttrTitle[],
                  *RatingWord[],
                  *SlotNames[],
                  *NumberNames[],
                  *RomanNumerals[],
                  // *TileNames[],
                  *EncumbranceNames[];

extern TextVal DTypeNames[],
               ATypeNames[],
               AbilityNames[],
               ClassAbilities[],
               RarityNames[],   
               //FeatNames[],
               WeaponGroupNames[],
               MTypeNames[],
               ITypeNames[],
               SourceNames[],
               AnnotationNames[],
               AttkVerbs1[],
               AttkVerbs2[],
               BreathTypes[],
               StatiLineStats[],
               StatiLineShorts[],
               BoltNames[],
               BeamNames[],
               CloudNames[],
               BallNames[],
               BreathNames[],
               SchoolNames[],
               StudyNames[],
               SizeNames[],
               ActionVerbs[],
               DataTypeNames[],
               BonusNames[],
               BonusNicks[],
               // FeatNames[],
               // FeatDescs[],
               SaveBonusNames[],
               PreQualNames[],
               PostQualNames[],
               QualityDescs[],
               MaterialDescs[],
               GenericPreQualNames[],
               APreQualNames[],
               APostQualNames[],
               GenericQualityDescs[],
               AQualityDescs[],
               MMDTypeNames[],
               MMAttkVerbs[],
               MMAttkVerbs2[],
               SpellPurposeNames[],
               PerDescs[],
               FileErrors[];

extern TextVal MA_CONSTNAMES[],
                M_CONSTNAMES[],
               AD_CONSTNAMES[],
                A_CONSTNAMES[],
                T_CONSTNAMES[],  
               SS_CONSTNAMES[],
             ATTR_CONSTNAMES[],
            STATI_CONSTNAMES[],
               EV_CONSTNAMES[],
           OPCODE_CONSTNAMES[],
               TA_CONSTNAMES[],
               DT_CONSTNAMES[],
               WQ_CONSTNAMES[],
               AQ_CONSTNAMES[],
               IQ_CONSTNAMES[],
               WS_CONSTNAMES[],
               SK_CONSTNAMES[],
               SN_CONSTNAMES[],
               HI_CONSTNAMES[],
               IL_CONSTNAMES[],
            PHASE_CONSTNAMES[],
             BARD_CONSTNAMES[],
              INV_CONSTNAMES[],
               CH_CONSTNAMES[],
               CA_CONSTNAMES[],
               SP_CONSTNAMES[],
               AI_CONSTNAMES[],
               MS_CONSTNAMES[],
               IF_CONSTNAMES[],
            GLYPH_CONSTNAMES[];

extern Dice MonkDamage[];

extern struct PerkAbil okStartingAbils[];
extern int16 badStartingFeats[];


extern struct SkillInfoStruct SkillInfo[];
extern int8   Synergies[][3];
extern struct AlignmentInfoStruct AlignmentInfo[];
extern struct AbilityInfoStruct AbilInfo[];
extern struct FeatInfoStruct FeatInfo[];
extern const char * FeatName(int16 ft);
extern bool FeatUnimplemented(int16 ft);

extern const char *Arrows;
extern const char *MenuLetters;
extern const int16 MonsterGlyphs[], 
                   ItemGlyphs[],
                   CreationGlyphs[],
                   WildShapeGlyphs[];
extern const char *PersonalityNames[],
                  *PersonalityDescs[];
extern int32 ExperienceChart[];
extern int8 SpellTable[27][9],
            BonusSpells[22][9];
extern int16 EncumbranceTable[];
extern int16 RogueFeats[],
             MonkFeats[],
             RangerEnemies[];
extern int16 QualityMods[][2],
             AQualityMods[][2];

extern int8 FaceRadius[];
extern int8 NeededSwings[];

extern int8 SpecialistTable[9][9];
extern int16 __SAL[];
extern int32 WealthByLevel[];

extern uint16 ActiveTraits[];
extern struct Option OptionList[];
extern struct YuseCommand YuseCommands[];

extern struct ReincarnationInfo RInf;

extern int16 p_base, p_int, p_spec, p_lev, p_conc,
             p_meta, p_spec, p_calc, p_circ;
extern const char* ps_perfect, *ps_calc;

extern int16 dc_base, dc_hard, dc_focus, dc_trick, dc_will,
             dc_beguile, dc_lev, dc_attr, dc_height, dc_affinity;
extern const char* dcs_attr;

extern rID GodIDList[];
extern int16 nGods;

extern Term *T1;

extern "C" int yyerror(const char*);
