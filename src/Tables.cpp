/* TABLES.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Contains all of the static data used by Incursion that is
   hard-coded into the game rather then being user-definable
   in modules. There are no functions or active code in this
   file.
*/

#include "Incursion.h"

#include "ConstNam.h"              

// #include "feattab.h"

int16 __spentHours;

/* Saving throw tables for creatures up to CR 50. Nothing in
   the game should ever be given a CR greater than 50, lest
   strange memory errors occur. */
#ifdef NO_FACE
int8 FaceRadius[] = 
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
#else
int8 FaceRadius[] = {
  /* (Zero) */          0,
  /* SZ_MINISCULE */    0,
  /* SZ_TINY */         0,
  /* SZ_SMALL */        0,
  /* SZ_MEDIUM */       0,
  /* SZ_LARGE */        0,
  /* SZ_HUGE */         1,
  /* SZ_GARGANTUAN */   2,
  /* SZ_COLLOSAL */     3, // ww: let's shrink this a bit for now! was 5
  /* Paranoia */        5, 5, 5 };
#endif

// ww: how much mana do you get at level X? 
int8 ManaMultiplier[32] = {
  1, 1,
  2, 2,
  3, 3, 3,
  4, 4, 4, 4,
  5, 5, 5, 5, 5,
  6, 6, 6, 6, 6, 6,
  7, 7, 7, 7, 7, 7, 7,
  8, 8, 8 }  ;
  
int16 __SAL[] = {
  0, 1, 3, 5, 7, 9, 12, 14, 16, 18 };
  
int32 WealthByLevel[] = {
  0, 1000, 1700, 3500, 5400, 9000, 13000,
  19000, 27000, 36000, 49000, 66000,
  88000, 110000, 150000, 200000, 260000,
  340000, 440000, 580000, 760000 };

int8 NeededSwings[10] = {
  0, 20, 15, 10,
     7,  5,  4,
     3,  2,  1 };

/* I chose the divisors here based on which
   skills seemed weak or very one-dimensional,
   i.e., Balance, Concentration and Appraise, 
   or were exceptionally strongly related to the 
   second skill (Bluff and PP). A divisor of 3 
   should be fairly rare outside of weak skills, 
   to prevent skill inflation. */
int8 Synergies[][3] = {
//  Beneficiary     Improved By     Div
  { SK_DIPLOMACY,   SK_APPRAISE,     3 },
  { SK_BLUFF,       SK_APPRAISE,     3 },
  { SK_INTIMIDATE,  SK_APPRAISE,     3 },
  { SK_PERFORM,     SK_APPRAISE,     3 },
  { SK_CLIMB,       SK_BALANCE,      3 }, 
  { SK_KNOW_MAGIC,  SK_SPELLCRAFT,   5 },
  { SK_SPELLCRAFT,  SK_KNOW_MAGIC,   5 },
  { SK_DISGUISE,    SK_BLUFF,        3 }, // carry the role
  { SK_INTUITION,   SK_KNOW_MAGIC,   4 }, // knowledge backs up instinct
  { SK_METAMAGIC,   SK_KNOW_MAGIC,   4 },
  { SK_PICK_POCKET, SK_BLUFF,        4 }, // distract the mark so he's not alert
  { SK_TUMBLE,      SK_BALANCE,      3 },
  { SK_HANDLE_DEV,  SK_CONCENT,      3 }, // tense work
  { SK_HEAL,        SK_WILD_LORE,    5 }, // herbalism
  { SK_JUMP,        SK_TUMBLE,       5 },
  { SK_TUMBLE,      SK_JUMP,         5 },
  { SK_JUMP,        SK_ATHLETICS,    4 },
  { SK_CLIMB,       SK_ATHLETICS,    4 },
  { SK_SWIM,        SK_ATHLETICS,    4 },
  { SK_LOCKPICKING, SK_CONCENT,      4 }, 
  { SK_LISTEN,      SK_CONCENT,      4 },
  { SK_SPOT,        SK_CONCENT,      4 }, 
  { SK_LOCKPICKING, SK_HANDLE_DEV,   3 },
  { SK_KNOW_INF,    SK_KNOW_PLANES,  3 },
  { SK_RIDE,        SK_ANIMAL_EMP,   3 }, // nice horsie...
  { SK_KNOW_MAGIC,  SK_KNOW_PLANES,  5 },
  { SK_KNOW_PLANES, SK_KNOW_THEO,    4 },
  { 0,              0,               0 } };

int8 GoodSave[] = {
  0,
  +2,  +3,  +3,  +4,  +4, 
  +5,  +5,  +6,  +6,  +7,
  +7,  +8,  +8,  +9,  +9,
  +10, 10, +11, +11, +12,
  +12, +13, +13, +14, +14,
  +15, +15, +16, +16, +17,
  +17, +18, +18, +19, +19,
  +20, +20, +21, +21, +22,
  +22, +23, +23, +24, +24,
  +25, +25, +26, +26, +27 };

int8 PoorSave[] = {
  0,
   0,   0,  +1,  +1,  +1,
  +2,  +2,  +2,  +3,  +3,
  +3,  +4,  +4,  +4,  +5,
  +5,  +5,  +6,  +6,  +6,
  +7,  +7,  +7,  +8,  +8,
  +8,  +9,  +9,  +9,  +10,
  +10, +10, +11, +11, +11,
  +12, +12, +12, +13, +13,
  +13, +14, +14, +14, +15,
  +15, +15, +16, +16, +16 };

Dice MonkDamage[] = {
  { 1,  3,  0 },
  { 1,  6,  0 }, /* 1st Level */
  { 1,  8,  0 },
  { 1,  10, 0 }, /* 3rd Level */
  { 1,  10, 0 },
  { 1,  12, 0 }, /* 5th Level */
  { 1,  12, 0 },
  { 2,  8,  0 },
  { 2,  8,  0 }, /* 8th Level */
  { 3,  6,  0  },
  { 3,  6,  0  },
  { 1,  20, 0 }, /* 11th Level */
  { 1,  20, 0 }, 
  { 3,  8,  0 }, 
  { 3,  8,  0 }, /* 14th level */
  { 3,  10, 0 }, 
  { 3,  10, 0 }, 
  { 3,  12, 0 }, /* 17th level */
  { 3,  12, 0 }, 
  { 2,  20, 0 }, 
  { 2,  20, 0 }, /* 20th Level */
  { 2, 20, 0 } }; 

int8 ChargeBonusTable[10][30] = {
  { 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5,
    5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7 },
  { 0, 0, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5,
    5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9 },
  { 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8,
    8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13 },
  { 0, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 8, 8,
    9, 9, 10, 11, 11, 12, 12, 13, 14, 14, 15, 15, 16, 17 },
  { 0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9,
    10, 10, 11, 12, 12, 13, 14, 14, 15, 16, 16, 17, 18, 18, 19, 20 },
  { 0, 1, 2, 3, 3, 4, 5, 6, 6, 7, 8, 9, 9, 10, 
    11, 12, 12, 13, 14, 15, 15, 16, 17, 18, 18, 19, 20, 21, 21, 22 },
  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 },
  { 0, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 17, 18, 20,
    21, 23, 24, 26, 27, 28, 29, 31, 32, 34, 35, 37 },
  { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26,
    28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58 },
  { 0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39,
    42, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 90 } 
     };


int16 badStartingFeats[] = {
  FT_AP_LIGHT,
  FT_AP_MEDIUM,
  FT_AP_HEAVY,
  FT_WEAPON_FOCUS,
  FT_WEAPON_MASTERY      ,
  FT_WEAPON_PROFICIENCY  ,
  FT_WEAPON_SPECIALIST   ,
  FT_FLYBY_ATTACK   ,
  FT_REND   ,
  FT_RAKE   ,
  FT_HOVER   ,
  FT_POUNCE   ,
  FT_MULTIATTACK   ,
  FT_TRAMPLE   ,
  FT_INTENSIVE_STUDY   ,
  FT_MARTIAL_MASTERY   ,
  0
  };



PerkAbil okStartingAbils[] = {
  { CA_ARCANE_WILL       , 2 }, 
  { CA_ARCANE_WILL       , 2 }, 
  { CA_ARCANE_WILL       , 2 }, 
  { CA_ANIMAL_COMP       , 1 },
  { CA_ANIMAL_COMP       , 1 },
  { CA_ANIMAL_COMP       , 1 },
  { CA_ANIMAL_COMP       , 1 },
// Note: this is the +2 Str / +2 Con version
  { CA_BERSERK_RAGE      , 1 },
  /* ww: let's add some additional variety */
  { CA_FATESENSE         , 1 },
  { CA_FAV_ENEMY         , (3 << 16) | MA_AIR },
  { CA_FAV_ENEMY         , (3 << 16) | MA_DEMON },
  { CA_FAV_ENEMY         , (3 << 16) | MA_DEVIL },
  { CA_FAV_ENEMY         , (3 << 16) | MA_DRAGON },
  { CA_FAV_ENEMY         , (3 << 16) | MA_REPTILE },
  { CA_FAV_ENEMY         , (3 << 16) | MA_VERMIN },
  { CA_FAV_ENEMY         , (3 << 16) | MA_WATER },
  { CA_FAV_ENEMY         , (3 << 16) | MA_ORC },
  { CA_FAV_ENEMY         , (3 << 16) | MA_VERMIN },
  { CA_FAV_ENEMY         , (3 << 16) | MA_BEAST },
  { CA_FAV_ENEMY         , (3 << 16) | MA_CONSTRUCT },
  { CA_FAV_ENEMY         , (3 << 16) | MA_DRAGON },
  { CA_FAV_ENEMY         , (3 << 16) | MA_ELEMENTAL },
  { CA_FAV_ENEMY         , (3 << 16) | MA_GIANT },
  { CA_FAV_ENEMY         , (3 << 16) | MA_ILLITHID },
  { CA_FAV_ENEMY         , (3 << 16) | MA_TROLL },
  { CA_FAV_ENEMY         , (3 << 16) | MA_EARTH },
  { CA_FAV_ENEMY         , (3 << 16) | MA_AIR },
  { CA_FAV_ENEMY         , (3 << 16) | MA_FIRE },
  { CA_FAV_ENEMY         , (3 << 16) | MA_WATER },
  { CA_FAV_ENEMY         , (3 << 16) | MA_UNDEAD },
  { CA_FAV_ENEMY         , (3 << 16) | MA_DEMON },
  { CA_FAV_ENEMY         , (3 << 16) | MA_DEVIL },
  { CA_FAV_ENEMY         , (3 << 16) | MA_NAGA },
  { CA_FAV_ENEMY         , (3 << 16) | MA_GOBLINOID },
  { CA_FAV_ENEMY         , (3 << 16) | MA_ABERRATION },
  { CA_FEED_UPON_PAIN    , 1 },
  { CA_GIANT_KILLER      , 1 },
  { CA_RAPID_RIDING      , 4 },
  { CA_INCREASED_MOVE      , 2 },
  { CA_INFRAVISION       , 6 },
  { CA_INHERANT_POTENTIAL      , 1 },
  { CA_LEGEND_LORE       , 2 }, 
  { CA_NATURE_SENSE      , 1 },
  { CA_NOBLE_AEGIS       , 5 },
  { CA_PLANT_MASTERY     , 3 }, 
  { CA_PREMONITION       , 1 },
  { CA_SCENT             , 3 },
  { CA_SHARP_SENSES      , 3 },
  { CA_SMITE             , MA_CHAOTIC },
  { CA_SMITE             , MA_GOOD },
  { CA_SMITE             , MA_LAWFUL },
  { CA_SMITE             , MA_OUTSIDER },
  { CA_SMITE             , MA_UNDEAD },
  { CA_SNEAK_ATTACK      , 1 },
  { CA_SNEAK_ATTACK      , 1 },
  { CA_SNEAK_ATTACK      , 1 },
  { CA_SNEAK_ATTACK      , 1 },
  { CA_SNEAK_ATTACK      , 1 },
  { CA_STONEWORK_SENSE   , 2 },
  { CA_STONEWORK_SENSE   , 2 },
  { CA_STONEWORK_SENSE   , 2 },
  { CA_STONEWORK_SENSE   , 2 },
  { CA_STRONG_SOUL       , 2 },
  { CA_SOOTHING_WORD     , 6 },
  { CA_SOOTHING_WORD     , 6 },
  { CA_SOOTHING_WORD     , 6 },
  { CA_SCENT             , 3 },
  { CA_TELEPATHY         , 3 },
  { CA_TELEPATHY         , 3 },
  { CA_UNCANNY_DODGE     , 3 },
  { CA_UNCANNY_DODGE     , 3 },
  { CA_UNCANNY_DODGE     , 3 },
  { CA_UNEARTHLY_LUCK    , 1 },
  { CA_WEAPON_SKILL      , 2 },
  { CA_WOODLAND_STRIDE   , 1 },
  { 0                    , 0 }
};

TextVal SizeNames[] = {
        {    SZ_MINISCULE, "miniscule" }, 
        {    SZ_TINY, "tiny" }, 
        {    SZ_SMALL, "small" }, 
        {    SZ_MEDIUM, "human-sized" }, 
        {    SZ_LARGE, "large" }, 
        {    SZ_HUGE, "huge" }, 
        {    SZ_GARGANTUAN, "gargantuan" }, 
        {    SZ_COLLOSAL, "collosal" },
        { 0, NULL }};

TextVal ActionVerbs[] = {
  { EV_EAT, "eating" },
  { EV_MOVE, "running" },
  { EV_SEARCH, "searching" }, 
  { EV_SATTACK, "kicking" }, 
  { EV_SEARCH, "searching" }, 
  { EV_REST, "resting" },
//  { EV_STUDY, "studying" },
  { EV_DIG, "digging" },
  { EV_ZAP, "identifying" },
  { EV_CAST, "casting detect spells" },
//  { EV_DISARM, "disarming" },
//  { EV_REJUVINATE, "meditating" },
//  { EV_SETTRAP, "working" },
//  { EV_ALCHEMY, "working" },
  { 0, NULL } };

TextVal DataTypeNames[] = {
  { DT_VOID, "void" },
  { DT_INT8, "int8" },
  { DT_UINT8, "uint8" },
  { DT_INT16, "int16" },
  { DT_UINT16, "uint16" },
  { DT_INT32, "int32" },
  { DT_HOBJ, "hObj" },
  { DT_RID, "rID" },
  { DT_BOOL, "bool" },
  { DT_STRING, "string" },
  { DT_HTEXT, "hText" },
  { DT_UNKNOWN, "(unknown type)" },
  { 0, NULL } };

const int16 ItemGlyphs[] = {
  GLYPH_POTION,   -AI_POTION,
  GLYPH_SCROLL,   -AI_SCROLL,
  GLYPH_ROD,      -AI_ROD,
  GLYPH_STAFF,    -AI_STAFF,
  GLYPH_WAND,     -AI_WAND,
  GLYPH_EYES,     -AI_EYES,
  GLYPH_FOOD,     T_FOOD,
  GLYPH_BOOK,     T_BOOK,
  GLYPH_BOOK,     -AI_TOME,
  GLYPH_WEAPON,   T_WEAPON,
  GLYPH_WEAPON,   -AI_WEAPON,
  GLYPH_LARMOUR,   -AI_ARMOUR,
  GLYPH_SHIELD,   -AI_SHIELD,
  GLYPH_BOW,      T_BOW,
  GLYPH_ARROW,    T_MISSILE,
  GLYPH_LARMOUR,   T_ARMOUR,
  GLYPH_SHIELD,   T_SHIELD,
  GLYPH_GAUNTLETS, -AI_GAUNTLETS,
  GLYPH_HELMET,   -AI_HELM,
  GLYPH_HELMET,   -AI_HEADBAND,
  GLYPH_BOOTS,    -AI_BOOTS,
  GLYPH_BRACERS,  -AI_BRACERS,
  GLYPH_CLOAK,    -AI_CLOAK,
  GLYPH_CLOTHES,  T_CLOTHES,
  GLYPH_CONTAIN,  T_CONTAIN,
  GLYPH_GIRDLE,   -AI_GIRDLE,
/*  GLYPH_DUST,    -AI_DUST,
  GLYPH_DECK,    -AI_DECK,*/
  GLYPH_RING,    -AI_RING,
  GLYPH_AMULET,  -AI_AMULET,
  GLYPH_HORN,    -AI_HORN,
  '*',           -AI_STONE,
  GLYPH_FIGURE,  T_FIGURE,
  GLYPH_HERB,    T_HERB,
  GLYPH_MUSH,    T_MUSH,
  GLYPH_GEM,     T_GEM,
  GLYPH_COIN,    T_COIN,
  GLYPH_TOOL,    T_TOOL,
  GLYPH_POTION,  -AI_POISON,
  GLYPH_TORCH,   T_LIGHT,
  //GLYPH_JUNK,    T_JUNK,
  GLYPH_TRAP,    T_TRAP,
  GLYPH_TOOL,    -AI_WONDER,
  GLYPH_TOOL,    -AI_ALCHEMY,
  0, 0 };

const int16 CreationGlyphs[] = {
  GLYPH_FOOD,     T_FOOD,
  GLYPH_WEAPON,   T_WEAPON,
  GLYPH_BOW,      T_BOW,
  GLYPH_ARROW,    T_MISSILE,
  GLYPH_LARMOUR,   T_ARMOUR,
  GLYPH_SHIELD,   T_SHIELD,
  GLYPH_CLOTHES,  T_CLOTHES,
  GLYPH_CONTAIN,  T_CONTAIN,
  GLYPH_GIRDLE,   -AI_GIRDLE,
  GLYPH_HERB,    T_HERB,
  GLYPH_MUSH,    T_MUSH,
  GLYPH_GEM,     T_GEM,
  GLYPH_COIN,    T_COIN,
  GLYPH_TOOL,    T_TOOL,
  GLYPH_TORCH,   T_LIGHT,
  //GLYPH_JUNK,    T_JUNK,
  GLYPH_TRAP,    T_TRAP,
  GLYPH_TOOL,    -AI_ALCHEMY,
  0, 0 };


const int16 MonsterGlyphs[] = {
      /*GLYPH_GUARD, MA_GUARD,
      GLYPH_TOWNIE, MA_TOWNIE,
      GLYPH_TOWNSCUM, MA_TOWNSCUM,
      GLYPH_TOWN_NPC, MA_TOWN_NPC, */
      GLYPH_HUMAN, MA_HUMAN,
      /*GLYPH_HUMAN, MA_ADVENTURER,*/
      GLYPH_ELF, MA_ELF,
      GLYPH_DWARF, MA_DWARF,
      GLYPH_GNOME, MA_GNOME,
      GLYPH_HOBBIT, MA_HALFLING, 
      'a', MA_ANIMAL,
      'b', MA_BAT,
      'b', MA_BIRD,
      'c', MA_CAT,
      'd', MA_DOG,
      'e', MA_EYE,
      'f', MA_FAERIE,
      'g', MA_GOBLIN,
      'h', MA_HUMANOID,
      'i', MA_IMPKIN,
      'j', MA_JELLY, 
      'k', MA_KOBOLD,
      'l', MA_LYCANTHROPE,
      'm', MA_MIMIC,
      'n', MA_NAGA,
      'o', MA_ORC,
      //'p', MA_PUDDING,
      'q', MA_QUADRUPED,
      'r', MA_RODENT,
      's', MA_SPIDER,
      't', MA_TRAPPER,
      //'u', 
      'v', MA_VORTEX,
      'w', MA_WORM,
      'x', MA_VERMIN,
      'y', MA_YUAN_TI,
      //'z', MA_LIGHT,
      'A', MA_CELESTIAL,
      'B', MA_BEAST,
      'C', MA_CONSTRUCT,
      'D', MA_DRAGON,
      'E', MA_ELEMENTAL,
      'F', MA_FUNGI,
      'G', MA_GOLEM,
      'H', MA_GIANT,
      'I', MA_ILLITHID,
      'J', MA_GENIE,
      'K', MA_ELEMENTALKIN,
      'L', MA_LICH,
      'M', MA_MYTHIC,
      'N', MA_NIGHTSHADE,         
      'O', MA_OUTSIDER,
      'P', MA_PLANT,
      'Q', MA_QUYLTHULG,
      'R', MA_REPTILE,
      'S', MA_SWARM,
      'T', MA_TROLL,
      'U', MA_REVENANT,
      'V', MA_VAMPIRE,
      'W', MA_WRAITH,
      'X', MA_ABERRATION,
      'Y', MA_HOMINID,
      'Z', MA_ZOMBIE,
      GLYPH_LDEMON, MA_DEMON,
      GLYPH_LDEVIL, MA_DEVIL,
      GLYPH_GDEMON, MA_DEMON,
      GLYPH_GDEVIL, MA_DEVIL,
      0, 0 };

const int16 WildShapeGlyphs[] = {
      'a', MA_ANIMAL,
      'b', MA_BAT,
      'b', MA_BIRD,
      'c', MA_CAT,
      'd', MA_DOG,
      'q', MA_QUADRUPED,
      'r', MA_RODENT,
      's', MA_SPIDER,
      'x', MA_VERMIN,
      'R', MA_REPTILE,
      0, 0 };

int8 MonsterGroupTypes[] = {
  MA_ORC,           /* Orc Lair */
  MA_GOBLIN,        /* Goblin Camp */
  MA_TROLL,         /* Troll Cave */
  MA_KOBOLD,        /* Kobold Warren */
  MA_UNDEAD,        /* Graveyard */
  MA_UNDEAD,        /* Graveyard */
  MA_ZOMBIE,        /* Graveyard */
  MA_ADVENTURER,    /* Anywhere */
  MA_ADVENTURER,    /* Anywhere */
  MA_FUNGI,         /* Fungi Garden */
  MA_OUTSIDER,      /* Planar Crossroads */
  0 };




      

TextVal StatiLineStats[] = {
  { AFRAID, "<6>Afraid<7> " },
  { SICKNESS, "<5>Sick<7> " },
  { STUCK, "<3>Stuck<7> " },
  { RAGING, "<10>Raging<7> " },
  { HIDING, "<4>Hiding<7> " },
  { INVIS, "<14>Invisible<7> " },
  { INVIS_TO, "<6>(Invisible)<7> " },
  { PHASED, "<8>Phased<7> " },
  { STUNNED, "<11>Stunned<7> " },
  { BLINDNESS, "<10>Blind<7> " },
  { STONING, "<13>PETRIFYING<7> " },
  { NAUSEA, "<11>Nausea<7> " },
  { BLEEDING, "<4>Bleeding<7> " },
  { CHOKING, "<12>Choking<7> " },
  { CHANNELING, "<14>Channeling<7> " },
  { GRAPPLED, "<15>Held<7> " },
  { GRABBED, "<15>Ensnared<7> " },
  { GRAPPLING, "<15>Grappling<7> " },
  // ww: polymorph is obvious -- your @ changes!
  // { POLYMORPH, "<2>Polymorphed<7> " },
  { CHARMED, "<3>Charmed<7> " },
  { PRONE, "<5>Prone<7> " },
  { TUMBLING, "<12>Tumbling<7> " },
  { SINGING, "<14>Singing<7> " },
  { CHARGING, "<4>Charge<7> " },
  { EXPOSED, "<5>Exposed<7> " },
  { ANCHORED, "<5>Anchored<7> " },
  { SPRINTING, "<5>Sprinting<7> " },
  { DEFENSIVE, "<14>Defensive<7> " },
  { EYES_AVERTED, "<14>Eyes Averted<7> " },
  { MANIFEST, "<10>Manifest<7> " },
  { ELEVATED, "<10>Elevated<7> " },
  { ANC_MEM, "<10>Trance<7> " },
  { FLAWLESS_DODGE, "<10>Dodge<7> "},
  { FLURRYING, "<11>Flurry<7> "},
  { 0, NULL } };

TextVal StatiLineShorts[] = {
  { AFRAID, "<6>Af<7> " },
  { SICKNESS, "<5>Si<7> " },
  { STUCK, "<3>Stk<7> " },
  { RAGING, "<10>Rag<7> " },
  { HIDING, "<4>Hid<7> " },
  { INVIS, "<14>Inv<7> " },
  { INVIS_TO, "<6>(Inv)<7> " },
  { PHASED, "<8>Ph<7> " },
  { STUNNED, "<11>St<7> " },
  { BLINDNESS, "<10>Bli<7> " },
  { NAUSEA, "<11>Naus<7> " },
  { STONING, "<13>PETRIFYING<7> " },
  { BLEEDING, "<4>Ble<7> " },
  { CHOKING, "<12>Ch<7> " },
  { POLYMORPH, "<2>Poly<7> " },
  { CHANNELING, "<14>Chan<7> " },
  { CHARMED, "<3>Chr<7> " },
  { PRONE, "<5>Pr<7> " },
  { SINGING, "<14>Si<7> " },
  { CHARGING, "<4>Ch<7>" },
  { TUMBLING, "<12>Tu<7> " },
  { ANCHORED, "<5>An<7> " },
  { EXPOSED, "<5>Ex<7> " },
  { SPRINTING, "<5>Sp<7> " },
  { DEFENSIVE, "<14>Def<7> " },
  { MANIFEST, "<10>Mani<7> " },
  { ELEVATED, "<10>El<7> " },
  { EYES_AVERTED, "<14>Eye<7> " },
  { ANC_MEM, "<10>Tran<7> " },
  { FLAWLESS_DODGE, "<10>Dg<7>"},
  { FLURRYING, "<11>Fy<7> "},
  { 0, NULL } };


TextVal BonusNames[] = {
  { BONUS_STUDY, "study" },
  { BONUS_ENHANCE, "magic" },
  { BONUS_ARTI, "artifact" },
  { BONUS_RESIST, "resistance" },
  { BONUS_SACRED, "sacred" },
  { BONUS_MORALE, "morale" },
  { BONUS_INSIGHT, "insight" },
  { BONUS_FEAT, "feat" },
  { BONUS_WEAPON, "weapon" },
  { BONUS_ARMOUR, "armour" },
  { BONUS_SHIELD, "shield" },
  { BONUS_CLASS, "class" },
  { BONUS_NEGLEV, "neglev" },
  { BONUS_COMP, "competence" },
  { BONUS_SIZE, "size" },
  { BONUS_DEFLECT, "deflect" },
  { BONUS_DAMAGE, "damage" },
  { BONUS_RAGE, "rage" },
  { BONUS_STATUS, "status" },
  { BONUS_INHERANT, "inherent" },
  { BONUS_GRACE, "grace" },
  { BONUS_NATURAL, "natural" },
  { BONUS_SKILL, "skill" },
  { BONUS_DUAL, "dual-weapon" },
  { BONUS_FATIGUE, "fatigue" },
  { BONUS_TEMP, "template" },
  { BONUS_DODGE, "dodge" },
  { BONUS_CIRC, "circumstance" },
  { BONUS_LUCK, "luck" },
  { BONUS_ENCUM, "encumbrance" },
  { BONUS_ELEV, "elevation" },
  { BONUS_TACTIC, "tactics" },
  { BONUS_HUNGER, "hunger" },
  { BONUS_ATTR, "" },
  { BONUS_PAIN, "pain" },
  { BONUS_HIDE, "hiding" },
  { 0, NULL } };

TextVal BonusNicks[] = {
  { BONUS_ENHANCE, "Ma" },
  { BONUS_ARTI, "Art" },
  { BONUS_RESIST, "Re" },
  { BONUS_ARMOUR, "Arm" },
  { BONUS_SHIELD, "Sh" },
  { BONUS_SACRED, "Sa" },
  { BONUS_MORALE, "Mr" },
  { BONUS_INSIGHT, "In" },
  { BONUS_FEAT, "Ft" },
  { BONUS_WEAPON, "Wp" },
  { BONUS_CLASS, "Cl" },
  { BONUS_NEGLEV, "NL" },
  { BONUS_COMP, "Co" },
  { BONUS_SIZE, "Si" },
  { BONUS_DEFLECT, "Df" },
  { BONUS_DAMAGE, "Da" },
  { BONUS_RAGE, "Ra" },
  { BONUS_STATUS, "St" },
  { BONUS_INHERANT, "Ih" },
  { BONUS_GRACE, "Gr" },
  { BONUS_NATURAL, "Na" },
  { BONUS_SKILL, "Sk" },
  { BONUS_DUAL, "Du" },
  { BONUS_FATIGUE, "Fa" },
  { BONUS_TEMP, "Te" },
  { BONUS_DODGE, "Do" },
  { BONUS_ATTR, "" },
  { BONUS_CIRC, "Ci" },
  { BONUS_LUCK, "Lu" },
  { BONUS_ENCUM, "En" },
  { BONUS_ELEV, "El" },
  { BONUS_TACTIC, "Tc" },
  { BONUS_HUNGER, "Hg" },
  { BONUS_PAIN, "Pa" },
  { 0, NULL } };

TextVal AnnotationNames[] = {
  { AN_SPELLS, "AN_SPELLS" },
  { AN_INNATE, "AN_INNATE" },
  { AN_EQUIP, "AN_EQUIP" },
  { AN_EVENT, "AN_EVENT" },
  { AN_ELEMENT, "AN_ELEMENT" },
  { AN_ABILITY, "AN_ABILITY" },
  { AN_EFFECT, "AN_EFFECT" },
};


TextVal AttkVerbs1[] = {
  { A_PUNC, "punch" },
  {  A_KICK, "kick" },
  {  A_BASH, "bash" },
  {  A_GRAB, "grab" },
  {  A_CLAW, "claw" },
  {  A_BITE, "bite" },
  {  A_BUTT, "butt" },
  {  A_GORE, "gore" },
  {  A_TAIL, "tail-lash" },
  { A_TUCH, "touch" },
  { A_STNG, "sting" },
  {  A_TENT, "tentacle-lash" },
  {   A_HUGS, "hug" },
  { A_RUBS, "rub against" },
  { A_SLOB, "drool on" },
  { A_SWAT, "swat at" },
  { A_OOZE, "flow over" },
  { A_SLAM, "slam" },
  { A_STOM, "stomp" },
  { A_WRAP, "wrap around" },
  { A_SPOR, "spray spores at" },
  { A_RAKE, "rake" },
  { A_TRAM, "trample" },
  { A_CRUS, "crush" },
  { A_REND, "rend" },
  { A_SUCK, "suck" },
  { A_CONS, "constrict around" },
  { A_IMPA, "impale" },
  { A_THRO, "throw" },
  { A_SPIT, "spit at" },
  { A_HURL, "hurl" },
  { A_BREA, "breathe" },
  { A_BRE2, "breathe" },
  { A_GAZE, "gaze at" },
  { A_DGST, "digest" },
  { A_EXPL, "explode" },
  /* A_SPIK */
  { A_ROAR, "roar" },
  { A_GEST, "gesture at" },
  /* A_DETH */
  /* A_AURA */
  /* A_PASS */
  /* A_RSPL */
  { A_RSPO, "spray spores at" },
  { A_DEXP, "explode" },
  /* A_DPIT */
  { A_BULL, "bull rush" },
  /* A_COUP */
  { A_SWNG, "swing" },
  { A_THRU, "thrust" },
  { A_FIRE, "fire" },
  { A_LUNG, "lunge" },
  { A_TRIP, "trip" },
  { A_DISA, "disarm" },
  { A_RIPO, "riposte the strike" },
  { A_CHAR, "charge" },
  { A_COUP, "deliver a killing blow" },
  { A_AIDA, "aid" },
  { A_ALSO, "and" },
  { A_DEQU, "damage equipment" },
  { 0, NULL } };
  
TextVal AttkVerbs2[] = {
  { A_PUNC, "punches" },
  {  A_KICK, "kicks" },
  {  A_BASH, "bashes" },
  {  A_GRAB, "grabs" },
  {  A_CLAW, "claws" },
  {  A_BITE, "bites" },
  {  A_BUTT, "butts" },
  {  A_GORE, "gores" },
  {  A_TAIL, "tail-lashes" },
  {  A_TENT, "tentacle-lashes" },
  { A_TUCH, "touches" },
  { A_STNG, "stings" },
  {   A_HUGS, "hugs" },
  { A_RUBS, "rubs against" },
  { A_SLOB, "drools on" },
  { A_SWAT, "swats at" },
  { A_OOZE, "flows over" },
  { A_SLAM, "slams" },
  { A_STOM, "stomps" },
  { A_PUNC, "punches" },
  { A_WRAP, "wraps around" },
  { A_SPOR, "sprays spores at" },
  { A_RAKE, "rakes" },
  { A_TRAM, "tramples" },
  { A_CRUS, "crushes" },
  { A_REND, "rends" },
  { A_SUCK, "sucks" },
  { A_CONS, "constricts around" },
  { A_IMPA, "impales" },
  { A_THRO, "throws" },
  { A_SPIT, "spits at" },
  { A_HURL, "hurls" },
  { A_BREA, "breathes" },
  { A_BRE2, "breathes" },
  { A_GAZE, "gazes at" },
  { A_DGST, "digests" },
  { A_EXPL, "explodes" },
  /* A_SPIK */
  { A_ROAR, "roars" },
  { A_GEST, "gestures at" },
  /* A_DETH */
  /* A_AURA */
  /* A_PASS */
  /* A_RSPL */
  { A_RSPO, "sprays spores at" },
  { A_DEXP, "explodes" },
  /* A_DPIT */
  { A_BULL, "bull rushes" },
  { A_TRIP, "trips" },
  { A_DISA, "disarms" },
  /* A_COUP */
  { A_SWNG, "swings" },
  { A_THRU, "thrusts" },
  { A_FIRE, "fires" },
  { A_LUNG, "lunges" },
  { A_COUP, "delivers a killing blow" },
  { A_RIPO, "ripostes the strike" },
  { A_CHAR, "charges" },
  { A_AIDA, "aids" },
  { A_DEQU, "damages equipment" },
  { 0, NULL } }; 



TextVal BreathTypes[] = {
  { AD_FIRE, "fire" },
  { AD_COLD, "freezing cold" },
  { AD_ACID, "a stream of acid" },
  { AD_ELEC, "lightning" },
  { AD_TOXI, "stream of poison gas" },
  { AD_NECR, "soul-chilling darkness" },
  { AD_PSYC, "psychic fury" },
  { AD_MAGC, "force shards" },
  { AD_SLEE, "bluish gas" },
  { AD_SUNL, "stream of sunlight" },
  { AD_STUN, "strange gas" },
  { AD_PLYS, "glowing gas" },
  { AD_STUK, "glowing gas" },
  { AD_STON, "magical energies" },
  { AD_POLY, "magical energies" },
  { AD_CHRM, "magical energies" },
  { AD_STON, "magical energies" },
  { 0, NULL } };

/*TextVal MTypeGroups[] = {
  { MA_ANIMAL, "Animals" },
  { MA_BAT, "Bats" },
  { MA_BIRD, "Birds" },
  { MA_CAT, "Cats" },
  { MA_DOG, "Dogs" },
  { MA_EYE, "Eye Creatures / Beholders" },
  { MA_FAERIE, "Fey Creatures" },
  { MA_GOBLIN, "Goblinkin" },
  { MA_HUMANOID, "Humanoids" },
  { MA_JELLY, "Jellies" },
  { MA_KOBOLD, "Kobolds" },*/


TextVal MTypePlurals[] = {
  {   MA_VORTEX, "Vorticies" },
  {       MA_ELF, "Elves" },
  {     MA_DWARF, "Dwarves" },
  {   MA_GOBLIN, "Goblinkin" },
  {      MA_UNDEAD, "Undead" },
  {      MA_LICH, "Liches" },
  { MA_ZOMBIE, "Raised Undead" },
  { MA_FUNGI, "Fungi" },
  {       MA_JELLY, "Jellies" },
  {      MA_VERMIN, "Vermin" },
  {    MA_REVENANT, "Corporeal Undead" },
  {    MA_WRAITH, "Wraiths and Shadows" },
  { MA_DROW, "Drow" },
  { 0, NULL } };

/* This list should include things that a ranger could *sensibly* hate or
   specialize in killing -- categories like "quadrupeds" or 
   "creatures with blood" make no sense. */
int16 FavEnemies[] = { 

  /* Your Classical Huntsman */
  MA_ANIMAL, 

  /* Base Races */
  MA_HUMAN, MA_ELF, MA_DWARF, MA_HALFLING, MA_GNOME, MA_DROW,
  MA_ORC, MA_KOBOLD, MA_GOBLIN, MA_TROLL,
  
  /* Monsterous Races */
  MA_YUAN_TI, MA_GENIE, MA_ILLITHID, MA_NAGA, MA_FAERIE,
  MA_QUYLTHULG, MA_GOLEM,

  /* Broad Monster Types */
  MA_ABERRATION, MA_MYTHIC, MA_GIANT, MA_UNDEAD, MA_VERMIN,
  MA_ELEMENTAL, MA_PLANT, MA_DRAGON, MA_OOZE, MA_CONSTRUCT,

  /* Elemental Types */
  MA_EARTH, MA_AIR, MA_FIRE, MA_WATER, MA_COLD,

  /* Animal-Like Monsters */
  MA_REPTILE, MA_SNAKE, MA_SPIDER, MA_DOG, MA_CAT, MA_BIRD,
  MA_DINOSAUR,
  
  /* Undead Hunters! */
  MA_DEMON, MA_DEVIL, MA_VAMPIRE, MA_ZOMBIE, 0 };

int16 MonGroupRarities[] = {
        MA_ANIMAL, MR_UNCOMMON ,
         MA_HUMAN, MR_XRARE    , /* 7 */
           MA_ELF, MR_RARE     , /* 33 */
         MA_DWARF, MR_RARE     , /* 26 */
      MA_HALFLING, MR_UNCOMMON , /* 44 */
         MA_GNOME, MR_RARE     , /* 23 */
          MA_DROW, MR_RARE     , /* 26 */
           MA_ORC, MR_RARE     , /* 24 */
        MA_KOBOLD, MR_UNCOMMON , /* 57 */
        MA_GOBLIN, MR_UNCOMMON , /* 73 */
         MA_TROLL, MR_XRARE    , /* 6 */
       MA_YUAN_TI, MR_XRARE    , /* 0 */
         MA_GENIE, MR_XRARE    , /* 4 */
      MA_ILLITHID, MR_XRARE    , /* 17 */
          MA_NAGA, MR_XRARE    , /* 8 */
        MA_FAERIE, MR_UNCOMMON , /* 58 */
     MA_QUYLTHULG, MR_XRARE    , /* 0 */
         MA_GOLEM, MR_XRARE    , /* 7 */
    MA_ABERRATION, MR_UNCOMMON , /* 66 */
        MA_MYTHIC, MR_XRARE    , /* 6 */
         MA_GIANT, MR_UNCOMMON , /* 47 */
        MA_UNDEAD, MR_COMMON   , /* 91 */
        MA_VERMIN, MR_COMMON   , /* 92 */
     MA_ELEMENTAL, MR_UNCOMMON , /* 49 */
         MA_PLANT, MR_COMMON   , /* 131 */
        MA_DRAGON, MR_UNCOMMON , /* 52 */
          MA_OOZE, MR_XRARE    , /* 19 */
     MA_CONSTRUCT, MR_UNCOMMON , /* 40 */
         MA_EARTH, MR_RARE     , /* 29 */
           MA_AIR, MR_COMMON   , /* 114 */
          MA_FIRE, MR_COMMON   , /* 90 */
         MA_WATER, MR_COMMON   , /* 114 */
          MA_COLD, MR_RARE     , /* 38 */
       MA_REPTILE, MR_UNCOMMON , /* 60 */
         MA_SNAKE, MR_XRARE    , /* 19 */
        MA_SPIDER, MR_RARE     , /* 37 */
           MA_DOG, MR_UNCOMMON , /* 40 */
           MA_CAT, MR_XRARE    , /* 13 */
          MA_BIRD, MR_RARE     , /* 35 */
         MA_DEMON, MR_XRARE    , /* 7 */
         MA_DEVIL, MR_RARE     , /* 32 */
       MA_VAMPIRE, MR_XRARE    , /* 4 */
        MA_ZOMBIE, MR_UNCOMMON , /* 61 */
       MA_DINOSAUR, MR_VRARE,
  0, 0 };

TextVal RarityNames[] = {
  { MR_COMMON, "common" },
  { MR_UNCOMMON, "uncommon" },
  { MR_RARE, "rare" },
  { MR_VRARE, "very rare" },
  { MR_XRARE, "scarce" },
  { 0, NULL } };

TextVal MTypeNames[] = {
  {       MA_HUMAN, "Human" },
  {  MA_ADVENTURER, "Adventurer" },
  {         MA_ELF, "Elf" },
  {       MA_DWARF, "Dwarf" },
  {       MA_GNOME, "Gnome" },
  {      MA_HALFLING, "Halfling" },
  {    MA_DINOSAUR, "Dinosaur" },
  {         MA_BAT, "Bat" },
  {        MA_BIRD, "Bird" },
  {         MA_CAT, "Cat" },
  {         MA_DOG, "Dog" },
  { MA_EYE, "Eye Creature" },
  {      MA_FAERIE, "Faerie" },
  {   MA_GOBLIN, "Goblinkin" },
  {    MA_HUMANOID, "Humanoid" },
  {       MA_JELLY, "Jelly" },
  {      MA_ANIMAL, "Animal" },
  {       MA_SNAKE, "Snake" },
  {       MA_GOLEM, "Golem" },
  {    MA_ILLITHID, "Illithid" },
  {       MA_GENIE, "Genie" },
  {         MA_HAG, "Hag" },
  {      MA_IMPKIN, "Imp" },
  {      MA_KOBOLD, "Kobold" },
  { MA_LYCANTHROPE, "Lycanthrope" },
  {       MA_MIMIC, "Mimic" },
  {        MA_NAGA, "Naga" },
  {         MA_ORC, "Orc" },
  {   MA_QUADRUPED, "Quadruped" },
  {      MA_RODENT, "Rodent" },
  {      MA_SPIDER, "Spider" },
  {     MA_TRAPPER, "Trapper" },
  {      MA_VORTEX, "Vortex" },
  {      MA_VERMIN, "Vermin" },
  {        MA_WORM, "Worm" },
  {     MA_YUAN_TI, "Yuan-Ti" },
  {   MA_QUYLTHULG, "Quylthulg" },
  {   MA_CELESTIAL, "Celestial" },
  {       MA_BEAST, "Beast" },
  {       MA_DEMON, "Demon" },
  {       MA_DEVIL, "Devil" },
  {   MA_ELEMENTAL, "Elemental" },
  {       MA_FUNGI, "Fungus" },
  {       MA_GIANT, "Giant" },
  {  MA_ELEMENTALKIN, "Elementalkin" },
  {        MA_LICH, "Lich" },
  { MA_MYTHIC, "Mythic Hybrid" },
  {  MA_NIGHTSHADE, "Nightshade" },
  {     MA_REPTILE, "Reptile" },
  {      MA_SPIRIT, "Spirit" },
  {       MA_TROLL, "Troll" },
  {      MA_UNDEAD, "Undead" },
  {     MA_VAMPIRE, "Vampire" },
  {        MA_WORM, "Worm" },
  {     MA_PUDDING, "Pudding" },
  {       MA_PLANT, "Plant Creature" },
  {    MA_REVENANT, "Corporeal Undead" },
  {    MA_WRAITH, "Negative Undead" },
//  {      MA_BEETLE, "Beetle" },
  { MA_ZOMBIE, "Zombie" },
  { MA_SKELETON, "Skeleton" },
  {   MA_CONSTRUCT, "Construct" },
  {      MA_DRAGON, "Dragon" },
  {  MA_ABERRATION, "Aberration" },
  {    MA_HOMINID, "Hominid" },
  { MA_OUTSIDER, "Outsider" },
  { MA_EARTH, "Earth Creature" },
  { MA_AIR, "Air Creature" },
  { MA_FIRE, "Fire Creature" },
  { MA_COLD, "Cold Creature" },
  { MA_SONI, "Sound Creature" },
  { MA_ACID, "Acid Creature" },
  { MA_MAGC, "Magical Creature" },
  { MA_WATER, "Water Creature" },
  { MA_ELEC, "Electric Creature" },
  { MA_TOXI, "Toxic Creature" },
  { MA_SUNL, "Sun Creature" },
  { MA_DROW, "Drow" },
  { MA_GOBLINOID, "Goblinoid" },
  { MA_BURROW, "Burrowing Mammal" },
  { MA_NATURAL, "Natural Creature" },
  { MA_SYLVAN, "Sylvan Creature" },
  { MA_SAPIENT, "Sapient" },
  { MA_EVIL, "Evil Creature" },
  { MA_LAWFUL, "Lawful Creature" },
  { MA_CHAOTIC, "Chaotic Creature" },
  { MA_GOOD, "Good Creature" },
  { MA_AVIAN, "Avian Creature" },
  { MA_OOZE, "Ooze" },
  { MA_REPTILIAN, "Reptilian Creature" },
  { MA_SUMMONED, "Summoned Creature" },
  { MA_LIVING, "Living Creature" },
  { MA_NLIVING, "Natural Living Creature" },
  { MA_BREATHER, "Breathing Creature" },
  { MA_AQUATIC, "Aquatic Creature" },
//  { MA_MAGICAL, "Magical Creature" },
  { MA_ALLIES, "Allied Creature" },
  { MA_HOSTILES, "Hostile Creature" },
  { MA_HAS_BLOOD, "Creature with Blood" },
  { MA_PLANETOUCHED, "Planetouched" },
  { MA_SWARM, "Swarm" },
  { MA_PERSON, "Person" },

  { 0, NULL }
  };

TextVal ITypeNames[] = {
  { T_POTION, "Potions" },
  { T_CORPSE, "Corpses" },
  { T_SCROLL, "Scrolls" },
  { T_ROD, "Rods" },
  { T_STAFF, "Staves" },
  { T_WAND, "Wands" },
  { T_FOOD, "Edibles" },
  { T_BOOK, "Books" },
  { T_WEAPON, "Weapons" },
  { T_BOW, "Missile Weapons" },
  { T_MISSILE, "Missiles" },
  { T_ARMOUR, "Armours" },
  { T_SHIELD, "Shields" },
  { T_CLOTHES, "Clothing" },
  { T_CONTAIN, "Containers" },
  { T_GIRDLE, "Girdles" },
  { T_DUST, "Dusts" },
  { T_DECK, "Decks" },
  { T_FIGURE, "Figurines" },
  { T_MUSH, "Mushrooms" },
  { T_HERB, "Herbs" },
  { T_GEM, "Gems" },
  { T_TOOL, "Tools" },
  { T_LIGHT, "Light Sources" },
  { T_COIN, "Coins" },
//  { T_JUNK, "Worthless Items" },
  { T_TRAP, "Traps" },
  { T_WONDER, "Wonderous Items" },
  { T_CHEST, "Chests" }, 
  { 0, NULL } };

TextVal SourceNames[] = {
  { AI_POTION, "Potions" },
  { AI_SCROLL, "Scrolls" },
  { AI_ROD, "Rods" },
  { AI_STAFF, "Staves" },
  { AI_WAND, "Wands" },
  { AI_TOME, "Tomes" },
  { AI_GAUNTLETS, "Gauntlets" },
  { AI_HELM, "Helms" },
  { AI_HEADBAND, "Headbands" },
  { AI_BOOTS, "Boots" },
  { AI_BRACERS, "Bracers" },
  { AI_CLOAK, "Cloaks" },
  { AI_GIRDLE, "Girdles" },
  { AI_RING, "Rings" },
  { AI_AMULET, "Amulets" },
  { AI_HORN, "Horns" },
  { AI_STONE, "Stones" },
  { AI_EYES, "Lenses" },
  { AI_WEAPON, "Specific Weapons" },
  { AI_ARMOUR, "Specific Armour" },
  { AI_SHIELD, "Specific Shields" },
  { AI_WONDER, "Wonderous Items" },
  { AI_ALCHEMY, "Alchemical Items" },
  { AI_POISON, "Poisons" },
  { 0, NULL } };


TextVal DTypeNames[] = {
  { AD_NORM, "Physical Damage" },
  { AD_FIRE, "Fire" },
  { AD_COLD, "Cold" },
  { AD_ACID, "Acid" },
  { AD_ELEC, "Lightning" },
  { AD_TOXI, "Poison" },
  { AD_NECR, "Life Drain" },
  { AD_PSYC, "Psychic Damage" },
  { AD_MAGC, "Pure Magic" },
  { AD_SONI, "Sonic Damage" },
  { AD_SUNL, "Light Damage" },
  { AD_SLEE, "Sleep" },
  { AD_STUN, "Stunning" },
  { AD_PLYS, "Paralysis" },
  { AD_STUK, "Entangling" },
  { AD_STON, "Petrification" },
  { AD_POLY, "Polymorph" },
  { AD_CHRM, "Mind Effects" },
  { AD_DISN, "Disintegration" },
  { AD_CRIT, "Critical Hits" },
  { AD_BLND, "Blindness" },
  { AD_SLOW, "Slowing" },
  { AD_DREX, "Experience Drain" },
  { AD_DRMA, "Mana Drain" },
  { AD_LEGS, "Leg Injury" },
  { AD_STUK, "Webbing" },
  { AD_STCK, "Stickiness" },
  { AD_STEA, "Equipment Theft" },
  { AD_SGLD, "Gold Theft" },
  { AD_TLPT, "Teleportation" },
  { AD_RUST, "Rusting" },
  { AD_SOAK, "Soaking" },
  { AD_CONF, "Confusion" },
  { AD_CURS, "Cursing" },
  { AD_WRAP, "Wrapping" },
  { AD_DRST, "Strength Drain" },
  { AD_DRDX, "Dexterity Drain" },
  { AD_DRCO, "Constitution Drain" },
  { AD_DRIN, "Intelligence Drain" },
  { AD_DRWI, "Wisdom Drain" },
  { AD_DRCH, "Charisma Drain" },
  { AD_DRLU, "Luck Drain" },
  { AD_DISE, "Disease" },
  { AD_DCAY, "Organic Decay" },
  { AD_WERE, "Lycanthropy" },
  { AD_BLEE, "Bleeding" },
  { AD_POIS, "Poison" },
  { AD_KNOC, "Knockback" },
  { AD_FEAR, "Fear" },
  { AD_GRAB, "Grabbing" },
  { AD_ENGL, "Engulfing" },
  { AD_CHOK, "Choking" },
  { AD_SUBD, "Subdual Damage" },
  { AD_HOLY, "Holy Power" },
  { AD_EVIL, "Unholy Power" },
  { AD_LAWF, "Lawful Power" },
  { AD_SHAT, "Shattering" },
  { AD_CHAO, "Chaotic Power" },
  { AD_SLASH, "Slashing Damage" },
  { AD_PIERCE, "Piercing Damage" },
  { AD_BLUNT, "Blunt Damage" },
  { AD_SOAK, "Water Damage" },
  { AD_TRIP, "Tripping" },

  { AD_DAST, "Strength Drain" },
  { AD_DADX, "Dexterity Drain" },
  { AD_DACO, "Constitution Drain" },
  { AD_DAIN, "Intelligence Drain" },
  { AD_DAWI, "Wisdom Drain" },
  { AD_DACH, "Charisma Drain" },
  { AD_DALU, "Luck Drain" },

  { AD_VAMP, "Vampiric" },
  { AD_NECR, "Necromatic" },
  { AD_FALL, "Falling Damage" },
  { AD_SWAL, "Swallow Whole" },
  
  { DF_SLASH, "Slashing Damage" },
  { DF_PIERCE, "Piercing Damage" },
  { DF_BLUNT, "Blunt Damage" },
  { AD_SPE1, "Special" },
  { AD_SPE2, "Special" },

  { 0, NULL } 
  };

TextVal ATypeNames[] = {
  { A_BREA, "Breath Weapon" },
  { A_BRE2, "Alternate Breath Weapon" },
  { A_SPIT, "Spit Weapon" },
  { A_GEST, "Gesture Attack" },
//  { A_SCRE, "Scream Attack" },
  { A_ROAR, "Roar Attack" },
  { 0, NULL } };

TextVal MMDTypeNames[] = {
  { AD_NORM, "for %s untyped damage" },
  { AD_FIRE, "for %s fire damage" },
  { AD_COLD, "for %s cold damage" },
  { AD_ACID, "for %s acid damage" },
  { AD_ELEC, "for %s lightning damage" },
  { AD_TOXI, "for %s poison damage" },
  { AD_NECR, "for %s necromantic damage" },
  { AD_PSYC, "for %s psychic damage" },
  { AD_MAGC, "for %s damage from pure magic" },
  { AD_SUNL, "for %s light damage" },
  { AD_SONI, "for %s sonic damage" },
  { AD_SLEE, "to put enemies to sleep for %s rounds" },
  { AD_STUN, "to stun for %s rounds" },
  { AD_PLYS, "to paralyze for %s rounds" },
  { AD_STUK, "to entangle for %s rounds" },
  { AD_STON, "to petrify" },
  { AD_POLY, "to polymorph" },
  { AD_CHRM, "to charm for %s rounds" },
  { AD_DISN, "to disintegrate" },
  { AD_BLND, "to blind for %s rounds" },
  { AD_SLOW, "to slow by 50~ for %s rounds" },
  { AD_DREX, "to drain (%s x 50) XP" },
  { AD_DRMA, "to drain %s mana" },
  { AD_LEGS, "to injure legs" },
  { AD_STCK, "to stick to an enemy" },
  { AD_GRAB, "to gain a hold on its enemy" },
  { AD_STEA, "to steal equipment" },
  { AD_SGLD, "to steal gold" },
  { AD_TLPT, "to teleport an enemy %s squares away" },
  { AD_RUST, "to rust equipment" },
  { AD_CONF, "to cause confusion for %s rounds" },
  { AD_CURS, "to lay a curse" },
  { AD_WRAP, "to wrap around an enemy" },
  { AD_DRST, "to drain %s points of Strength" },
  { AD_DRDX, "to drain %s points of Dexterity" },
  { AD_DRCO, "to drain %s points of Constitution" },
  { AD_DRIN, "to drain %s points of Intelligence" },
  { AD_DRWI, "to drain %s points of Wisdom" },
  { AD_DRCH, "to drain %s points of Charisma" },
  { AD_DRLU, "to drain %s points of Luck" },
  { AD_DAST, "to damage Strength by %s points" },
  { AD_DADX, "to damage Dexterity by %s points" },
  { AD_DACO, "to damage Constitution by %s points" },
  { AD_DAIN, "to damage Intelligence by %s points" },
  { AD_DAWI, "to damage Wisdom by %s points" },
  { AD_DACH, "to damage Charisma by %s points" },
  { AD_DALU, "to damage Luck by %s points" },
  { AD_DISE, "to spread disease" },
  { AD_DCAY, "to rot away organics" },
  { AD_WERE, "to spread lycanthropy" },
  { AD_BLEE, "to cause bleeding for %s rounds" },
  { AD_POIS, "to poison the enemy" },
  { AD_KNOC, "for %s squares knockback" },
  { AD_FEAR, "to cause fear lasting %s rounds" },  
  { AD_CHOK, "to cause choking lasting %s rounds" },
  { AD_SUBD, "for %s subdual damage" },
  { AD_HOLY, "for %s holy damage" },
  { AD_EVIL, "for %s unholy damage" },
  { AD_CHAO, "for %s chaotic damage" },
  { AD_LAWF, "for %s lawful damage" },
  { AD_SHAT, "to shatter equipment for %s blunt damage" },
  { AD_SLASH, "for %s slashing damage" },
  { AD_PIERCE, "for %s piercing damage" },
  { AD_BLUNT, "for %s blunt damage" },
  { AD_SOAK, "to drench an enemy" },
  { AD_ENGL, "to engulf an enemy" },
  { AD_TRIP, "to trip an enemy" },
  { AD_SWAL, "to swallow a creature whole" },
  { AD_VAMP, "to inflict %s necromantic damage and heal commensurately" },
  { 0, NULL } };

TextVal MMAttkVerbs[] = {
  { A_PUNC, "punches" },                     
  { A_KICK, "kicks" },
  {  A_BASH, "bashes" },
  {  A_GRAB, "grabs" },
  {  A_CLAW, "claws" },
  {  A_BITE, "bites" },
  {  A_BUTT, "butts" },
  {  A_GORE, "gores" },
  { A_TUCH, "touches" },
  {  A_TAIL, "tail-lashes" },
  {  A_TENT, "tentacle-lashes" },
  { A_STNG, "stings" },
  { A_PROB, "strikes with its proboscis" },
  {   A_HUGS, "hugs" },
  { A_RUBS, "rubs against an enemy" },
  { A_SLOB, "drools" },
  { A_SWAT, "swats" },
  { A_OOZE, "flows over an enemy" },
  { A_SLAM, "slams" },
  { A_STOM, "stomps" },
  { A_PUNC, "punches" },
  { A_WRAP, "wraps around an emeny" },
  { A_SPOR, "sprays spores" },
  { A_RAKE, "rakes" },
  { A_TRAM, "tramples" },
  { A_CRUS, "crushes" },
  { A_REND, "rends" },
  { A_SUCK, "sucks" },
  { A_CONS, "constricts" },
  { A_IMPA, "impales" },
  { A_THRO, "throws" },
  { A_SPIT, "spits" },
  { A_HURL, "hurls" },
  { A_BREA, "breathes" },
  { A_BRE2, "breathes" },
  { A_GAZE, "gazes" },
  { A_DGST, "digests" },
  { A_EXPL, "explodes" },
  { A_ROAR, "roars" },
  { A_GEST, "gestures" },
  /* A_DETH */
  /* A_AURA */
  /* A_PASS */
  /* A_RSPL */
  { A_RSPO, "sprays spores" },
  { A_DEXP, "explodes" },
  { A_DEQU, "damages equipment" },
  { A_SPEL, "cast a spell at" },
  /* A_DPIT */
  { 0, NULL } };

TextVal MMAttkVerbs2[] = {
  { A_CRIT, "In addition to the normal effects, its critical hits strike %s." },
  { A_DETH, "As its death throes, it strikes %s." },
  { A_PROX, "Anyone in its proximity is afflicted %s every round." },
  { A_CPRX, "It can afflict any enemies in its proximity %s every round." },
  { A_AURA, "Anything contacting it is affected by its aura %s." },
  { A_PASS, "When it is attacked, it lashes back %s." },
  { A_DEXP, "When it dies, it explodes %s." },
  { 0, NULL } };

TextVal WeaponGroupNames[] = {
  { WG_SIMPLE, "Simple Weapons" },
  { WG_EXOTIC, "Exotic Weapons" },
  { WG_SBLADES, "Short Blades" },
  { WG_LBLADES, "Long Blades" },
  { WG_AXES, "Axes" },
  { WG_ARCHERY, "Archery" },
  { WG_STAVES, "Staves" },
  { WG_IMPACT, "Impact" },
  { WG_THROWN, "Thrown" },
  { WG_POLEARMS, "Polearms" },
  { WG_SPEARS, "Spears" },
  { WG_LANCES, "Lances" },
  { WG_DAGGERS, "Daggers" },
  { WG_MARTIAL, "Martial Weapons" },
  { WG_FLEXIBLE, "Flexible" },
  { WG_FIREARMS, "Firearms" },
  { WG_FLAILS, "Flails" },
  { WG_SHIELDS, "Shields" },
  { WG_LARMOUR, "Light Armour" },
  { WG_MARMOUR, "Medium Armour" },
  { WG_HARMOUR, "Heavy Armour" },
  { 0, NULL } };

TextVal BoltNames[] = {
  { AD_NORM, "force bolt" },
  { AD_FIRE, "fire bolt" },
  { AD_COLD, "ice bolt" },
  { AD_ACID, "acid bolt" },
  { AD_ELEC, "ball of lightning" },
  { AD_POIS, "poison orb" },
  { AD_NECR, "necromantic bolt" },
  { AD_PSYC, "psychic missile" },
  { AD_MAGC, "magic missile" },
  { -1, "bolt" },
  { 0, NULL } };

TextVal BeamNames[] = {
  { AD_NORM, "force bolt" },
  { AD_FIRE, "fire jet" },
  { AD_COLD, "frost beam" },
  { AD_ACID, "acid jet" },
  { AD_ELEC, "lightning bolt" },
  { AD_POIS, "toxic jet" },
  { AD_NECR, "necromantic ray" },
  { AD_PSYC, "psychic ray" },
  { AD_MAGC, "mana bolt" },
  { -1, "beam" },
  { 0, NULL } };

TextVal BallNames[] = {
  { AD_NORM, "force blast" },
  { AD_FIRE, "fireball" },
  { AD_COLD, "ice storm" },
  { AD_ACID, "acid cloud" },
  { AD_ELEC, "lightning storm" },
  { AD_POIS, "poison cloud" },
  { AD_NECR, "necromantic burst" },
  { AD_PSYC, "psychic storm" },
  { AD_MAGC, "mana blast" },
  { -1, "evocation" },
  { 0, NULL } };



TextVal CloudNames[] = {
  { AD_FIRE, "burning cloud" },
  { AD_COLD, "frost cloud" },
  { AD_ACID, "acidic vapours" },
  { AD_ELEC, "lightning cloud" },
  { AD_POIS, "poison cloud" },
  { AD_NECR, "necromantic cloud" },
  { AD_PSYC, "mind-rending cloud" },
  { AD_MAGC, "cloud of mana" },
  { -1, "cloud" },
  { 0, NULL } };

TextVal BreathNames[] = {
  { AD_FIRE, "fire breath" },
  { AD_COLD, "frost breath" },
  { AD_ACID, "acid jet" },
  { AD_ELEC, "lightning bolt" },
  { AD_POIS, "poison cloud" },
  { AD_NECR, "necromantic breath" },
  { AD_PSYC, "mind-rending breath" },
  { AD_MAGC, "magical breath" },
  { -1, "breath" },
  { 0, NULL } };

TextVal SaveBonusNames[] = {
  { SN_MAGIC, "magic" },
  { SN_EVIL, "evil magic" },
  { SN_NECRO, "necromancy" },
  { SN_PARA, "paralysis" },
  { SN_DEATH, "death magic" },
  { SN_ENCH, "enchantment" },
  { SN_PETRI, "petrification" },
  { SN_POISON, "poison" },
  { SN_TRAPS, "traps" },
  { SN_GRAB, "being grabbed" },
  { SN_FEAR, "fear" },
  { SN_ILLUS, "illusions" },
  { SN_CONF, "confusion" },
  { SN_COMP, "compulsion" },
  { SN_DISEASE, "disease" },
  { SN_SPELLS, "spells" },
  { SN_THEFT, "theft" },
  { SN_DISINT, "disintegration" },
  { SN_KNOCKDOWN, "knockdown" },
  { SA_CONF, "confusion" },
  { 0, NULL } };


TextVal SchoolNames[] = {
  { SC_ABJ, "Abjuration" },
  { SC_ARC, "Arcana" },
  { SC_DIV, "Divination" },
  { SC_ENC, "Enchantment" },
  { SC_EVO, "Evocation" },
  { SC_ILL, "Illusion" },
  { SC_NEC, "Necromancy" },
  { SC_THA, "Thaumaturgy" },
  { SC_WEA, "Weavecraft" },
  { SC_THE, "Theurgy" },
  { SC_NAT, "Naturalism" },
  { SC_AIR, "Air Magic" },
  { SC_EARTH, "Earth Magic" },
  { SC_FIRE, "Fire Magic" },
  { SC_WATER, "Water Magic" },
  { SC_COLD, "Cold Magic" },
  { SC_FORCE, "Force" },
  { SC_LIGHT, "Light" },
  { SC_DARKNESS, "Darkness" },
  
  { 0, NULL } };

TextVal StudyNames[] = {
  { STUDY_BARDIC, "Improve Bardic Abilities" },
  { STUDY_CASTING, "Improve Caster Level" },
  { STUDY_SHAPES, "Improve Wild Shape" },
  { STUDY_BAB, "Improve Base Attack Bonus" },
  { STUDY_UNARMED, "Improve Unarmed Attack" },
  { STUDY_SNEAK, "Improve Stealth and Sneak Attack" },
  { STUDY_TURNING, "Improve Turn/Command Undead" },
  { STUDY_SMITE, "Improve Smite Ability" },
  { STUDY_MOUNT, "Improve Sacred Mount" },
  { 0, NULL } };

int8 SpecialistTable[9][9] = {
  /* Abjurer */     { +25, +15, +15,  +0, +10,  +0, -30, -15, +10 },
  /* Arcanist */    {  +0, +25, +10,  +0,  +0, +10,  +0,  +0, +10 },
  /* Diviner */     {  +0, +10, +25,  +0, -15, -10,  +0,  +0,  +0 },
  /* Enchanter */   { +15, -10,  +0, +25, -15, +10,  +0, +10,  +0 },
  /* Evoker */      { +10,  +0, -20, -15, +25, -20, -15,  +5,  +0 },
  /* Illusionist */ {  +0, +20, +10, +10, -25, +25, -15, -10, +15 }, 
  /* Necromancer */ { -10,  +0, +15, -10,  +0, -20, +25, +15,  +0 },
  /* Thaumaturge */ {  +5,  +0, -10, +10, -10, -15, +15, +25,  +5 },
  /* Weaver */      { +15, +10, +10, -10,  +0,  +0, -15, -15, +25 } };
TextVal MaterialDescs[] = { 
  {  MAT_LIQUID, "liquid" },  /* currently only for venom */
  {  MAT_WAX, "wax" },
  {  MAT_VEGGY, "vegetable matter" },  /* foodstuffs */
  {  MAT_FLESH, "meat" },  /*   ditto    */
  {  MAT_PAPER, "paper" },
  {  MAT_CLOTH, "cloth" },
  {  MAT_LEATHER, "leather" },
  {  MAT_WOOD, "wood" },
  {  MAT_BONE, "bone" },
  {  MAT_DRAGON_HIDE, "dragon hide" },  /* not leather! */
  {  MAT_IRON, "iron" },  /* Fe - includes steel */
  {  MAT_METAL, "metal" },  /* Sn, &c. */
  {  MAT_COPPER, "copper" },  /* Cu - includes brass */
  {  MAT_SILVER, "silver" },  /* Ag */
  {  MAT_GOLD, "gold" },  /* Au */
  {  MAT_PLATINUM, "platinum" },  /* Pt */
  {  MAT_MITHRIL, "mithril" },
  {  MAT_ADAMANT, "adamant" },
  {  MAT_GLASS, "glass" },
  {  MAT_GEMSTONE, "gemstone" },
  {  MAT_MINERAL, "minerals" },
  {  MAT_WEBBING, "webbing" },
  {  MAT_INDESTRUCTABLE_STONE, "stone" }, 
  {  MAT_ICE, "ice" },
  {  MAT_MAGMA, "magma" },
  {  MAT_QUARTZ, "quartz" },
  {  MAT_CLOUD, "gas" },
  {  MAT_GRANITE, "granite" },
  {  MAT_DARKWOOD, "darkwood" },
  {  MAT_FORCE, "uncanny force" },
  {  MAT_EMPTYNESS, "nothing" }, 
  {  MAT_OIL, "oil" },
  {  MAT_MUD, "mud" },
  {  MAT_WROUGHT, "wrought iron" },
  {  MAT_IRONWOOD, "ironwood" },
  { 0, NULL } };

TextVal GenericPreQualNames[] = { 
  { IQ_SILVER, "silver" },
  { IQ_ADAMANT, "adamant" },
  { IQ_DARKWOOD, "darkwood" },
  { IQ_IRONWOOD, "ironwood" },
  { IQ_MITHRIL, "mithril" },

  { IQ_ELVEN, "elven" },
  { IQ_DWARVEN, "dwarven" },
  { IQ_ORCISH, "orcish" },

  { IQ_PROFICIENT, "proficient" }, 
  { IQ_FEATHERLIGHT, "feather-light" },
  { IQ_GHOST_TOUCH, "ghost-touch" },

  { 0, NULL } };
    
TextVal PreQualNames[] = {
  { WQ_HOLY, "holy" },
  { WQ_UNHOLY, "unholy" },
  { WQ_CHAOTIC, "chaotic" },
  { WQ_LAWFUL, "lawful" },
  { WQ_FLAMING, "flaming" },
  { WQ_SHOCKING, "shocking" },
  { WQ_FROST, "frost" },
  { WQ_CORROSIVE, "corrosive" },
  { WQ_BURST, "burst" },
  { WQ_SOULDRINK, "soul-drinking" },
  { WQ_MIGHTY, "mighty" },
  { WQ_KEEN, "keen" },
  { WQ_VORPAL, "vorpal" },
  { WQ_DEFENDING, "defending" },
  { WQ_CLEAVING, "cleaving" },
  { WQ_SMITING, "smiting" },
  { WQ_SUNDERING, "sundering" },
  { WQ_THUNDERING, "thundering" },
  { WQ_ENERGY, "brilliant energy" },
  { WQ_DANCING, "dancing" },
  { WQ_QUICK_LOADING, "quick-loading" },
  { WQ_RETURNING, "returning" },
  { WQ_VAMPIRIC, "vampiric" },
  { WQ_PARRYING, "parrying" },
//  { WQ_FEATHERLIGHT, "feather-light" },
  { WQ_BANE, "bane" },
  { WQ_IMPACT, "impact" },
  { WQ_VICIOUS, "vicious" },
  { WQ_SEEKING, "seeking" }, 
  { WQ_BINDING, "binding" }, 
  { WQ_GLOWING, "glowing" },
  { WQ_THIRSTY, "thirsty" },
  { WQ_FORKING, "forking" },
  { WQ_ROARING, "roaring" },
  { 0, NULL } };

TextVal PostQualNames[] = {
  { WQ_BALANCE, "balance" },
  { WQ_ENTROPIC, "entropy" },
  { WQ_FEY_TRAPPING, "fey-trapping" },
  { WQ_NIGHT, "the night" },
  { WQ_VENOM, "venom" },
  { WQ_WARNING, "warning" },
  { WQ_DISTANCE, "distance" },
  { WQ_SPEED, "speed" },
  { WQ_SLEEP, "sleep" },
  { WQ_ACCURACY, "accuracy" },
  { WQ_SLAYING, "slaying" },
  { WQ_DISRUPTION, "disruption" },
  { WQ_WOUNDING, "wounding" },
  { WQ_STUNNING, "stunning" },
  { WQ_TERROR, "terror" },
  { WQ_WEAKENING, "weakening" },
  { WQ_WITHERING, "withering" },
  { WQ_NUMBING, "numbing" },
  { WQ_PAIN, "pain" },
  { WQ_SUBTLETY, "subtlety" },
  { 0, NULL } };

TextVal APreQualNames[] = {
  { AQ_ANIMATED, "animated" },
  { AQ_BASHING, "bashing" },
  { AQ_BLINDING, "blinding" },
  { AQ_GRACEFUL, "graceful" },
  { AQ_SHADOW, "shadow" },
  { AQ_SLICK, "slick" },
  { AQ_RADIANT, "radiant" },
  { AQ_FEARSOME, "fearsome" },
  { AQ_SACRED, "sacred" },
  { AQ_SHIFTING, "shifting" },
  { AQ_BANEBLIND, "bane-blind" },
  { AQ_RESISTANT, "resistant" },
  { AQ_AUSPICIOUS, "auspicious" },
  { AQ_SPIKED, "spiked" },
  { AQ_GRAVEN, "graven" },
  { AQ_ARROW_DEF, "arrow-deflecting" },

  { 0, NULL } };
  
TextVal APostQualNames[] = {  
  { AQ_LIGHT_FOR, "light fortification" },
  { AQ_MEDIUM_FOR, "medium fortification" },
  { AQ_HEAVY_FOR, "heavy fortification" },
  { AQ_FIRE_RES, "fire resistance" },
  { AQ_COLD_RES, "cold resistance" },
  { AQ_ACID_RES, "acid resistance" },
  { AQ_LIGHT_RES, "lightning resistance" },
  { AQ_SONIC_RES, "sonic resistance" },
  { AQ_POISON_RES, "poison resistance" },
  { AQ_ETHEREALNESS, "etherealness" },
  { AQ_GREAT_INV, "great invulnerability" },
  { AQ_INVULN, "invulnerability" },
  { AQ_MAGIC_RES, "magic resistance" },
  { AQ_REFLECTION, "reflection" },
  { AQ_SILENT, "silent moves" },
  { AQ_MIGHT, "might" },
  { AQ_AGILITY, "agility" },
  { AQ_ENDURANCE, "endurance" },
  { AQ_HEALING, "rapid healing" },
  { AQ_STABILITY, "stability" },
  { AQ_COMMAND, "command" },
  { AQ_SHATTERING, "shattering" },
  { AQ_LIFEKEEPING, "life-keeping" },
  { 0, NULL } };

// nominally these could apply to anything: you could imagine an ironwood
// wand or an elven rope ...
TextVal GenericQualityDescs[] = {
  { IQ_SILVER, 
    "A silver item has half the hardness of iron but is a special bane to devils and lycanthropes. Against such foes a silver weapon gives "
    "a +2 attack bonus and a +2 damage bonus and strikes true even if the creature is immune to some weapons. This bonus is not cumulative "
    "(e.g., a silver bow and a silver arrow together yield +2). A silver shield or silver piece of armour gives a +2 defense bonus against "
    "attacks from such foes. Such creatures will generally not pick up silver items." },
  { IQ_ADAMANT, 
    "An adamant weapon has double the normal hardness of a weapon made out of iron, and gains a +1 material bonus to damage. Armour made "
    "from adamantine grants its wearer damage reduction of 1/- if it's light armour, 2/- if it's medium armour, and 3/- if it's heavy armour."},
  { IQ_DARKWOOD,
    "This rare magic wood is thrice as hard as normal wood but weighs only half as much. The armour check penalty of a darkwood item is " \
    "lessened by 2 compared to an ordinary item of its type." },
  { IQ_MITHRIL,
    "Mithril is a very rare silvery, glistening metal that is lighter than iron but just as hard. When worked like steel, it becomes a "
    "wonderful material from which to create weapons and armour and is occasionally used for other items as well. Mithril weapons gain a "
    "+1 material bonus to their accuracy. Mithril armours are one category lighter than normal for purposes of movement and other "
    "limitations. Heavy armours are treated as medium, and medium armours are treated as light, but light armours are still treated as light. "
    "However, you must still be proficient in the original armour type to avoid penalties. Spell failure chances for armours and shields "
    "made from mithril are decreased by 15%, maximum Dexterity bonus is increased by 3, and armour check penalties are lessened by 3 "
    "(to a minimum of 0). An item made from mithril weighs half as much as the same item made from other metals. Mithril has 150~ of "
    "the hardness of an iron and half the weight." }, 
  { IQ_IRONWOOD,
    "Ironwood is a magical substance created by druids from normal iron. While remaining natural wood in almost every way, ironwood is "
    "as strong, heavy, and resistant to fire as steel. Spells or abilities that affect metal or iron (such as rusting or the Heat Metal "
    "spell) do not function on ironwood. Spells that affect wood do affect ironwood, although ironwood does not burn. Ironwood items are "
    "freely usable by druids." },

  { IQ_ELVEN,
    "The elves craft weapons and armour that are light and fast. An elven-made item has only 75~ of its normal weight. Elven weapons have "
    "+10% bonus to speed and elven armours have their armour penalty reduced by 2." },
  { IQ_DWARVEN,
    "Dwarven weapons and armour are heavy and durable. They have 125~ of the normal weight, and a +10 bonus to their hardness. Dwarven "
    "armour gains a +1 bonus to its base damage reduction." },
  { IQ_ORCISH,
    "Orcish weapons and armour are crude and brutal. They have 50~ of their normal hit points. Orcish weapons suffer a -1 penalty to hit, "
    "but gain a +1 bonus to their base damage. Orcish armours are crude but effective in the short term. They suffer an increase of 1 "
    "to their armour penalty, but gain a +1 bonus to their base damage reduction." },
  { IQ_PROFICIENT,
    "A proficient item grants its user additional knowledge about its proper use. A user that would normally be non-proficient with it "
    "becomes proficient and suffers no penalties. A proficient weapon also increases the user's effective level of proficiency with it "
    "by one (e.g., a user that would normally be focused with the dagger gains benefits of being specialized when using a proficient dagger)."}, 
  { IQ_FEATHERLIGHT, 
    "Feather-light items weigh only a quarter as much as they normally would. A feather-light "
    "weapon is treated as if it were one size category smaller than it actually is for the "
    "purposes of its combat effects and wielding requirements. Combat fatigue accrues at a "
    "third of the normal rate when wearing feather-light armour, to a minimum of the amount "
    "normally accrued when fighting unarmoured, and feather-light armour does not impose a "
    "doubled penalty to the Swim skill." },

  { 0, NULL } };


TextVal QualityDescs[] = {
  { WQ_HOLY, "A holy weapon is infused with the metaphysical power of goodness "
             "and inflicts 2d6 points of bonus alignment damage against evil "
             "creatures, when wielded by a good-aligned character. It inflicts "
             "two negative levels on any evil character who wields it." },
  { WQ_UNHOLY, "An unholy weapon is infused with the corrupt energy of evil "
               "and inflicts 2d6 points of bonus alignment damage against good "
               "creatures, when wielded by an evil-aligned character. It inflicts "
               "two negative levels on any good character who wields it." },
  { WQ_CHAOTIC, "A chaotic weapon is infused with the force of primordial chaos "
             "and inflicts 2d6 points of bonus alignment damage against lawful "
             "creatures, when wielded by a chaotic-aligned character. It inflicts "
             "two negative levels on any lawful character who wields it." },
  { WQ_LAWFUL, "A lawful weapon is infused with the binding power of order "
             "and inflicts 2d6 points of bonus alignment damage against chaotic "
             "creatures, when wielded by a lawful-aligned character. It inflicts "
             "two negative levels on any chaotic character who wields it." },
  { WQ_BALANCE, "A weapon of balance is bound to neutrality, and inflicts 2d6 "
             "points of bonus alignment damage against any sapient creature "
             "that has no neutral component in their alignment, when wielded "
             "by a true neutral character. When wielded by a character with "
             "no neutral alignment component, it inflicts two negative levels "
             "on said character." },
  { WQ_CORROSIVE, "A corrosive weapon has a greasy feel to the steel, and erodes "
             "whatever it strikes, dealing 1d6 points of bonus acid damage on a "
             "successful hit." },
  { WQ_FLAMING, "A flaming weapon can sheathe itself in flame upon the wielder's "
             "mental command, and inflicts 1d6 points of bonus fire damage on a "
             "successful hit." },
  { WQ_SHOCKING, "A shocking weapon is charged with the power of lightning, and "
             "inflicts 1d6 points of bonus electrical damage on a successful hit." },
  { WQ_FROST, "A frost weapon is pale blue and icy cold to the touch; it inflicts "
             "1d6 points of bonus cold damage on a successful hit." },
  { WQ_THUNDERING, "A thundering weapon emits a mighty blast of thunder when it "
             "strikes, dealing 1d6 points of bonus sonic damage on a successful "
             "hit." },
  { WQ_BURST, "On a successful critical hit, this weapon inflicts #d10 elemental "
             "damage instead of the normal 1d6, where # is the weapon's critical "
             "hit damage multiplier." },
  { WQ_SOULDRINK, "A soul-drinking weapon can leech the very energies of a victim's "
             "spirit, draining 50 points of experience for every hit point of damage "
             "that it inflicts." },
  { WQ_KEEN, "A keen weapon possesses an unearthly sharp edge, and has double the "
             "normal threat range of a weapon of its type." },
  { WQ_VORPAL, "A vorpal weapon automatically decapitates (and thus kills) any "
             "creature with a head on a successful critical hit." },
  { WQ_DEFENDING, "A defending weapon adds its magic bonus to the character's defense "
             "class in addition to her attack and damage rolls." },
  { WQ_WARNING, "A weapon of warning flares when danger is near, giving you a 50% "
             "chance to negate surprise on any attack against you." },
  { WQ_CLEAVING, "A cleaving weapon inflicts a critical hit on a "
             "successful cleave attack." },
  { WQ_SMITING, "A smiting weapon doubles the bonus damage gained from a character's existing Smite ability and makes all Smite attacks critical hits." },
  { WQ_SUNDERING,
    "A sundering weapon deals triple damage in Sundering attacks and deals damage even to items with a "
    "higher magical plus. It also deals triple damage to inanimate objects like chests and doors (but not golems or magical constructs)." },
  { WQ_ENERGY,
    "A brilliant energy weapon phases cleanly through any inanimate matter and thus automatically bypasses a victim's armour. "
    "It deals normal damage to truly inaimate objects like chests and doors." },
  { WQ_DISTANCE, "A weapon of distance has its range increment doubled." },
  { WQ_SPEED, "A weapon of speed grants you a +50~ magic bonus to your attack speed (or a +25~ bonus if it is your off-hand weapon)." },
  { WQ_ACCURACY, "A weapon of accuracy strikes true, granting an additional +4 circumstance "
                 "bonus to hit, on top of its magic bonus. This bonus does not add to damage, "
                 "however." },
  { WQ_SLAYING, "A weapon of slaying (for example, troll slaying) destroys any creature "
             "of the given type outright on a successful hit, unless that target succeeds "
             "in a Fortitude saving throw (DC 10+%d) against this necromantic, death effect." },
  { WQ_QUICK_LOADING, "A quick-loading crossbow magically loads and cocks itself automatically and instantaneously. "
             "It thus fires on demand and with no extra delay, behaving much like a bow."}, 
             /*
  { WQ_DANCING, "A dancing weapon guides the wielder's hand and moves of its own "
             "accord once it determines the rhythm of a fight, gaining a culmulative "
             "+1 bonus to hit for each consecutive attack on a given target." },
             */
  { WQ_DISRUPTION, "A disruption weapon will destroy any undead creatures it strikes if they fail a Fortitude save (DC 10+%d)." },
  { WQ_RETURNING, "A returning weapon will fly back to the user's hand once it has "
             "been thrown, if it hits the intended target." },
  { WQ_THIRSTY, "A thirsty weapon deals 1d6 extra unblockable damage to creatures with blood on a successful hit." },
  { WQ_WOUNDING, "A weapon of wounding causes its victim to bleed after being struck "
             "for 1 hp of added damage per rounds for 2d12 rounds thereafter. This "
             "effect can be staunched with the Heal skill." },
  { WQ_SLEEP, "These missiles render unconscious a target whom they hit if that "
             "target is suceptable to sleep effects, fails a Fortitude save (DC %d) and "
             "if the attack roll exceeds that target's Defense Class by 5 or more points." },
  { WQ_STUNNING, "This weapon may stun its victims for 1d4+1 rounds on a critical hit, "
             "assuming that they fail a Fortitude saving throw (DC %d)." },
  { WQ_TERROR, "On a critical hit, this weapon fills its target with a mortal terror, "
             "causing the victim to flee for 3d6 rounds if a Will save (DC %d) is failed." },
  { WQ_VAMPIRIC, "A vampiric weapon heals damage to the wielder equal to one-fourth of the "
             "damage it inflicted on every successful attack." },
  { WQ_PARRYING, "A parrying weapon adds a +4 bonus to the wielder's Defense Class, "
             "but only against attacks with other swung weapons." },
   { WQ_BANE, "A bane weapon gains a +2 attack bonus and a +2 damage bonus against one chosen type of creature. " \
              "If the weapon strikes, it also deals 2d6 points of unsoakable damage." },
  { WQ_NIGHT, "A weapon of the night will put its target to sleep on a successful sneak attack, provided that " \
              "the target fails a Will Save (DC 20)."},
  { WQ_BANE, "A bane weapon gains a +2 attack bonus and a +2 damage bonus against one chosen type of creature. " \
             "If the weapon strikes, it also deals 2d6 points of unsoakable damage." },
  { WQ_IMPACT, "A knockback weapon can drive its targets back. On a successful hit the target must make a Fortitude " \
               "Save (DC 20) or be knocked back. If the target cannot move back it instead falls to the ground. If " \
               "the first save is failed the target must make another Fortitude Save (DC 20) or be stunned for 1 round. " \
               "This power only works on creatures no larger than the wielder."}, 
  { WQ_VENOM, "If a weapon of venom is coated with a poison, that poison will not wear off in combat and can be used repeatedly."}, 
  { WQ_MIGHTY, "A mighty weapon allows you to use your Strength bonus to influence the weapon's damage." },
  { WQ_FEY_TRAPPING, "A hit from this weapon limns the target with a pale glow, much like the faerie fire spell. This prevents " \
               "the target from hiding or gaining concealmeant benefits from blur, displacement, invisibility or similar effects. " \
               "In addition, the target is prevented from teleporting as per the dimensional anchor spell. These effects last for " \
               "one round per point of damage inflicted."}, 
  /*
  { WQ_ENTROPIC, "An entropic weapon inflicts ill luck on a target, granting a -2 "
             "luck penalty to all rolls for 3d6 rounds on a successful hit, or a -4 "
             "luck penalty on a critical hit. The penalty does not stack from multiple "
             "hits, but the duration does." },
             */
  { WQ_WEAKENING, "A weapon of weakening inflicts 1d6 points of Strength ability damage with every successful hit (save DC 10+%d for half)." },
  { WQ_NUMBING, "A weapon of numbing inflicts 1d6 points of Dexterity ability damage with every successful hit (save DC 10+%d for half)." },
  { WQ_WITHERING, "A weapon of withering inflicts 1d6 point of Constitution ability damage with every successful hit (save DC 10+%d for half)." },
  { WQ_VICIOUS, "A vicious weapon strikes at an enemy by consuming its wielder's own life force. At the cost of " \
            "1d6 hit points of damage to the wielder, the blade inflicts a bonus 3d6 points of unblockable damage " \
            "to any creature that it strikes. This effect only occurs if the attacker has at least two-thirds of " \
            "her total hit points." },
  { WQ_SEEKING, "A seeking weapon veers toward its target, negating any miss chance due to cover, effects like <9>displacement<7>, " \
            "being unable to see the target, being stunned, the target's elevation, or tower shields." }, 
  { WQ_BINDING, "A binding weapon binds any creature it strikes from magical movement "
             "when used to perform a Great Blow, afflicting them just as the "
             "<9>dimensional anchor<7> spell does: preventing teleportation or plane "
             "shifting. A Will save negates this effect." },
  { WQ_GLOWING, "A glowing weapon gives off light that cannot be extinguished, illuminating a "
                "30-foot area per magical plus as a torch and gaining a +2 to-Hit bonus versus "
                "light-averse creatures." },
  { 0, NULL } };

TextVal AQualityDescs[] = {
  
  { AQ_LIGHT_FOR, "This armour is magically reinforced, giving it a 25~ chance of "
              "negating any critical hit on its wearer, leaving the attack a normal "
              "hit. It also grants a +4 bonus to saving throws versus disintegration." },
  { AQ_MEDIUM_FOR, "This armour is magically reinforced, giving it a 50~ chance of "
              "negating any critical hit on its wearer, leaving the attack a normal "
              "hit. It also grants a +8 bonus to saving throws versus disintegration." },
  { AQ_HEAVY_FOR, "This armour is magically reinforced, giving it a 75~ chance of "
              "negating any critical hit on its wearer, leaving the attack a normal "
              "hit. It also grants a +12 bonus to saving throws versus disintegration." },

  { AQ_FIRE_RES, "This armour conveys upon its wearer a supernatural resistance to "
              "fire equal in magnitude to three times its magical bonus." },
  { AQ_ACID_RES, "This armour conveys upon its wearer a supernatural resistance to "
              "acid equal in magnitude to three times its magical bonus." },
  { AQ_COLD_RES, "This armour conveys upon its wearer a supernatural resistance to "
              "cold equal in magnitude to three times its magical bonus." },
  { AQ_LIGHT_RES, "This armour conveys upon its wearer a supernatural resistance to "
              "lightning and other electrical attacks equal in magnitude to three "
              "times its magical bonus." },
  { AQ_SONIC_RES, "This armour conveys upon its wearer a supernatural resistance to "
              "destructive sonic attacks equal in magnitude to three times its "
              "magical bonus." },
  { AQ_POISON_RES, "This armour conveys upon its wearer a supernatural resistance to "
              "hit point damage from toxins equal in magnitude to three times its "
              "magical bonus, as well as a circustance bonus to saving throws versus "
              "poison equal in magnitude to its magical bonus." },

  { AQ_ANIMATED, "On command, an animated shield hovers near its bearer without any "
              "apparent means of support, and can thus be used without filling the "
              "Ready Hand inventory slot. (To equip it, it should be 'a'ctivated.) "
              "It thus also incurs no armour penalty and no arcane spell failure "
              "modifier." },
  { AQ_ARROW_DEF, "A shield with this quality conveys the Deflect Arrows feat upon "
              "its bearer when equipped." },
  { AQ_BASHING, "A shield with this quality is a magical weapon, gaining its magic "
              "bonus not only to Defense Class, but to attack and damage rolls made "
              "when using the Shield Bash tactical option." },
  { AQ_BLINDING, "This quality causes a suit of armour or shield to gleam like perfectly "
              "polished steel. This increases your Defense Class by 2 versus enemies "
              "that target attacks by sight, and has a 1 in 20 chance of blinding any "
              "enemy who attacks you (Fortitude saving throw negates) for 1d4 rounds." },
  { AQ_ETHEREALNESS, "This suit of armour allows its wearer to become ethereal at will, "
              "costing two Fatigue Points per use, and functioning 3 times per day. To "
              "utilize this ability, 'a'ctivate the armour." },
  { AQ_GRACEFUL, "This armour is fluid, supple and perfectly fitted, granting its "
              "wearer a supernatural freedom of movement. As such, the armour penalty "
              "it inflicts is halved, rounding down." },
  { AQ_INVULN, "This armour bestows a limited invulnerability on you, granting you "
              "immunity to non-magical weapons." },
  { AQ_GREAT_INV, "This armour bestows a limited invulnerability on you, granting you "
              "immunity to weapons with an enchantment of +3 or less." },
  { AQ_MAGIC_RES, "This powerful armour turns aside any magic striking you, granting "
              "you a 10~ magic resistance for every magical plus it possesses." },
  { AQ_REFLECTION, "A shield of reflection turns hostile magic directed at you back "
              "upon the original attacker, duplicating the effects of <9>spell "
              "turning<7>." },
  { AQ_SHADOW, "Shadow armour blends supernaturally with shadows, giving its wearer "
              "a circumstance bonus to his Hide skill equal to twice its magical "
              "bonus." },
  { AQ_SILENT, "Silent armour magically muffles sound in a one-inch thick magical "
              "aura surronding the wearer, giving her a circumstance bonus to "
              "her Move Silently skill equal to twice its magical plus." },
  { AQ_SLICK, "Slick armour is polished and smoothed, allowing the wearer to evade "
              "others attempting to grab her. It adds a bonus equal to twice its "
              "magical plus to the wearer's Escape Artist skill." },
  { AQ_MIGHT, "Armour of might supernaturally augments its wearer's strength, granting "
              "the wearer an enhancement bonus to Strength equal to its magical plus." },
  { AQ_AGILITY, "Armour of agility supernaturally augments its wearer's reflexes, "
              "granting the wearer an enhancement bonus to Dexterity equal to its "
              "magical plus." },
  { AQ_ENDURANCE, "Armour of endurance supernaturally augments its wearer's toughness, "
              "granting the wearer an enhancement bonus to Constitution equal to "
              "its magical plus." },
  { AQ_HEALING, "Armour of rapid healing causes its wearer to quickly heal any injury "
              "suffered while it was worn, restoring a number of hit points each round "
              "equal to its magical enhancement bonus." },
  { AQ_STABILITY, "Armour of stability allows its owner to mystically root himself to "
              "the ground, giving him a circumstance bonus to resist trip and "
              "bull rush attacks equal to twice its magical plus." },
  { AQ_SPIKED, "Anyone attempting to grapple a character wearing spiked armour or wielding "
              "a spiked shield suffers 1d8 points of damage for every round they persist "
              "in the effort." },
  { AQ_COMMAND, "Armour of command imbues its wearer with supernatural majesty, giving "
              "him an enhancement bonus to his Charisma equal to its magical plus, and "
              "causing allies within 20 feet of him to gain a +1 morale bonus to attack, "
              "damage, defense class and saving throws." },
  { AQ_RADIANT, "A radiant shield or suit of armour emits a brilliant golden light, causing "
              "all squares within 60 feet of the wearer to be illuminated as if by a "
              "<9>call light<7> spell." },
  { AQ_FEARSOME, "Fearsome armour projects an aura around the wearer causing all seeing "
              "creatures within 20 feet of the wearer to suffer a morale penalty to "
              "attacks equal to his Charisma modifier or the armour's magical plus, whichever "
              "is lower. In addition, the armour can be activated to duplicate the effects "
              "of a <9>fear<7> spell." },
  { AQ_SACRED, "Embossed with the holy writings of Immotian and Mara, this armour is the "
              "special bane of undead. Its magical bonus is added to a cleric's level "
              "when she attempts to turn undead. Its coverage protects against special "
              "damage from touch attacks the way coverage normally protects against "
              "damage from other physical attacks." },
  { AQ_SHIFTING, "Shifting armour is able to metamorphose its form, becoming any type of "
              "armour the wielder wishes but retaining its magical properties. To change "
              "its shape, the wearer must 'a'ctivate the armour." },
  { AQ_BANEBLIND, "Baneblind armour makes its wearer invisible to those enemies that she "
              "has a special grudge against -- a ranger's favoured enemies, a dwarf or "
              "gnome's racial enemies and any enemy type that the wearer is wielding a "
              "bane-weapon against." },
  { AQ_RESISTANT, "Resistant armour protects its wearer from all manner of harm, granting "
              "her a resistance bonus to all her saving throws equal to its magical "
              "plus." },
  { AQ_AUSPICIOUS, "Auspicious armour bestows good fortune upon its wearer, granting a "
              "luck bonus to all her saving throws equal in magnitude to its magical "
              "bonus." },
  { AQ_GRAVEN, "Graven armour or shields are embossed with detailed carvings from all "
              "the various religious texts of Therya, and thus act as a holy symbol "
              "for any priest wearing them." },
  { AQ_LIFEKEEPING, "Lifekeeping armour allows its wearer to resist negative energy, "
              "sustaining all attributes at a level equal to its magical plus and "
              "also decreasing the experience loss suffered from life-draining "
              "attacks. Finally, it grants resistance to necromantic damage equal "
              "to thrice its magical plus." },
  { 0, NULL }

  };



#include "ArmourTab.h"

LocationInfo *Tiles;

LocationInfo __Tiles__[] = {
  /* Wall */    { GLYPH_VALUE(GLYPH_WALL, GREY),	0, 1,1,1,1,0,0,  0,0,0,0,0,0,0 },
  /* Floor */	{ GLYPH_VALUE(GLYPH_FLOOR, YELLOW),	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
  /* Rock */	{ GLYPH_VALUE(GLYPH_ROCK, BROWN),	0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  /* Border */	{ GLYPH_VALUE(GLYPH_SOLID, GREEN),	0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  };

const char* TileNames[] = {
  "Wall", "Floor", "Rock", "Border", NULL };

const char *Arrows =
  "\x18\x19\x1A\x1B";

const char* MenuLetters =
  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

Option OptionList[] = {

/* Chargen Options */
  { OPT_BEGINKIT, "Beginner's Kit", "OFF/ON", 1, 0, 0,
      "__This option allows you to begin play which some extra items designed "
      "to make the game easier for people still learning its intricacies. "
      "The kit includes Potions of Remove Curse, Cure Disease and Mana, and "
      "a Wand of Identify. Turning on this switch places the game in Explore "
      "Mode." },
  { OPT_STATS, "Initial Rolling Method", "4d6/4d6 + Perks/Point Buy", 2, 1, 0,
      "__This option determines which method is used initially to generate your "
      "starting attribute scores, out of the following choices:\n"
      "__<10>Method 4d6:<2> Five random stat sets are rolled in order, and you "
      "pick one that most suits you.\n"
      "__<10>Method 4d6 + Perks:<2> Five random stat sets are rolled in order."
      "Each set comes with two feats and a magical item. You pick the lot "
      "that most suits you. (If the item is a weapon or a piece of armour, "
      "you still need the proper proficiency to use it without penalty.) "
      "You can abort to 4d6 if you want to reroll in a baseline game.\n" 
      "__<10>Method Point Buy:<2> You purchase your scores using a point "
      "system to buy whatever level you want in each attribute. When using "
      "other methods, you can abort to Point Buy if your rolls do not suit "
      "your character concept.\n"
      /*
      "__<10>Method II:<2> You have a set number of total dice to assign among "
      "the attributes, and then that pool of dice is rolled and the three highest "
      "are summed to determine each attribute.\n"
      "__<10>Method III:<2> Seven ability scores are rolled, and you may place "
      "assign them to the seven attributes as you so desire.\n"
      "__<10>Method IV:<2> You purchase your scores using a point system "
      "to buy whatever level you want in each attribute.\n"
      "__<10>Method V:<2> You enter whatever ability scores you want, and are "
      "assigned an experience point penalty commensurate with how exceptional "
      "the chosen values are." 
      */
      },
  { OPT_REROLL, "Allow Rerolling", "NO/YES", 1, 0, 0,
      "__This option determines whether the game will allow you to reroll your "
      "attributes in order to get a better set when using methods involving "
      "randomness. The number of times you rerolled will be noted in your "
      "character dump." },
  { OPT_SUBRACES, "Use the Subraces", "NO/YES", 1, 0, 0,
      "__This option determines whether the player will be offered a choice of "
      "sub-races after he has chosen his primary race -- for example, black orcs "
      "and wood elves are subraces. This option is off initially so that new "
      "players will develop skill with the game first using standard races before "
      "trying to play the more exotic and confusing alternatives." },
  { OPT_DISALLOW, "Disallow Wizard Mode", "NO/YES", 1, 0, 0,
      "__Once a game has been started with this option active, wizard mode "
      "cannot be entered using that character. This enforces fair play for "
      "people who do not want the temptation of wizard mode available." },
  { OPT_MAX_HP, "Maximum Hit Points", "NO/HALF/YES", 2, 0, 0,
      "__This option gives your character the maximum hit points possible "
      "(or half the maximum possible) for his race, classes and level "
      "automatically. Turning on this switch places the game in Explore Mode." },
  { OPT_MAX_MANA, "Maximum Mana", "NO/HALF/YES", 2, 0, 0,
      "__This option gives your character the maximum possible mana "
      "(or half the maximum possible) for his class, level and Wisdom score. "
      "Turning on this switch places the game in Explore Mode." },
  { OPT_GENDER, "Default Character Gender", "ASK/FEMALE/MALE/RANDOM", 3, 0, 0,
      "__This option determines what the gender of a newly created character "
      "will be, and whether the player will be asked for a gender at "
      "character generation." },
  { OPT_MON_MAX_HP, "Monsters Maximum Hit Points", "NO/HALF/YES/DOUBLE", 3, 0, 0,
      "__This option gives everyone except your character the maximum hit "
      "points possible (or half or double their maximum, as you decide, "
      "instead of rolling randomly)." },
  { OPT_ATTR_FIRST, "Roll Attributes First", "NO/YES", 1, 0, 0,
      "__This option allows you to roll attributes before choosing race and "
      "class. If you gain perks from your rolling method, three perks will be "
      "listed with each attribute set, but you will only get the first one or "
      "two listed if you choose a race that only gets one or two perks." }, 
  #if 0
  /* FJM: I /really/ don't like this. It devalues one of the only unique things
     humans have going for them -- feats. It also shifts the base concept of
     the FC mechanic, so that a favoured class is "you should only play this
     race/class combo" instead of "you can multiclass this safely". I like
     the idea of dwarven monks or lizardmen bards, and I don't want to gimp
     those character types in comparison to dwarven fighters or whatnot. */
  { OPT_FAVOURED_CLASS, "Favoured Class Mechanic", "XP PENALTY/FEATS", 1, 0, 0,
      "__This option determines how racial favoured classes are handled. The "
      "traditional approach gives you a 25~ XP penalty if one of your highest-"
      "level classes is not a racial favoured class. The second approach gives "
      "you a bonus feat when your total levels in racial favoured classes equals "
      "2, 5, 8, 11, etc."},
  #endif
  { OPT_ATTR_ON_LEVEL, "Gain Attributes With Levels", "NO/YES", 1, 0, 0,
      "__If this option is set you gain a single-point inherent attribute bonus (your choice) at levels 4, 8, 12, etc." }, 
  { OPT_EASY_INTUIT, "Allow Rapid Intuition", "NO/YES/WEAPONS", 2, 2, 0,
      "__If this option is set, you receive Intuition information about an item immediately "
      "after picking it up, whereas normally you must rest to receive Intuition 'hunches' "
      "about magical items. The WEAPONS option -- the default -- allows rapid intuition only "
      "for weapons, armour, bows and missiles -- types of items non-spellcasting classes depend "
      "on that cannot easily be ID'd by flavour like potions. Setting this switch on YES "
      "places the game in Explore Mode." },
  { OPT_MON_DJIKSTRA, "Monsters Use Djikstra", "NO/YES/PETS", 2, 0, 0,
      "__If this option is set, monsters will use the Djikstra pathfinding algorithm "
      "in their AI. It makes them smarter, prevents them from getting lost and "
      "lets allies follow the PC more effectively, but is currently prohibitively "
      "slow. If this option is set to PETS, only creatures following the player will "
      "use Djikstra." },
  { OPT_OOD_MONSTERS, "Out-Of-Depth Monsters", "NO/YES", 1, 1, 0,
      "__This option determines whether the game is allowed to generate Crawl-style "
      "out-of-depth monsters on dungeon level 2 and above; having it on makes the "
      "game significantly harder." },
  { OPT_TRANSGRESS_HINTS, "Transgression Hints", "NO/YES", 1, 0, 0,
      "__This option prints a rationale behind the uneasy/guilty messages for "
      "alignment or religious conduct transgression when they are broken, and "
      "can be useful to players still coming to understand Incursion's alignment "
      "and religion system." },
  { OPT_DIFFICULTY, "Difficulty Level", "EXPLORE/TRAINING/BASELINE/CHALLENGE/NIGHTMARE", 4, 2, 0,
      "__The Difficulty Level switch skewers the generation of monsters and certain "
      "other factors in order to alter the overall difficulty of the game. EXPLORE "
      "allows players to experiment with different mechanical concepts without the "
      "game providing much of a serious challenge to the player. TRAINING reduces "
      "the difficulty more moderately, allowing new players to develop their skills. "
      "BASELINE is the default difficulty level which the game's balance is calibrated "
      "around; a victory in BASELINE mode means you can be considered to have 'beat the "
      "game'. CHALLENGE makes the game moderately more difficult by making the dungeon "
      "three levels deeper without increases the maximum character level, while NIGHTMARE "
      "works like CHALLENGE but also increases the difficulty of all monsters.\n"
      "__Setting Difficulty Level to EXPLORE or TRAINING will enable Explore Mode." }, 
  { OPT_POWER_STATS, "Power Stats", "REDUCED/BASELINE/HIGH/EXTREME", 3, 1, 0,
      "__This switch allows a character to begin with higher attribute sets. The "
      "switch affects both the rolling methods and Point Buy; in Point Buy mode, "
      "REDUCED gives you 30 points, BASELINE grants 42, HIGH 54 and EXTREME 75. "
      "Setting this switch to HIGH or EXTREME places the game in Explore Mode." }, 
  { OPT_ELUDE_DEATH, "Elude Death", "NO/ONE/THREE/SEVEN/NO DEATH", 4, 0, 0,
      "__The Elude Death option allows players to gain a number "
      "of free chances to return after dying, being transported to a safe locale "
      "and losing 15% of your total XP (minimum 750), but otherwise being unharmed."
      " This can make the game "
      "substantially easier for new players, since death is common and sometimes "
      "arbitrary in the early game, before one develops a relationship with one's "
      "god. The switch can also be set to NO DEATH, in which case characters always "
      "return immediately from death with no experience penalty for doing so.\n"
      "__It is recommended that players who want to develop the skill needed to "
      "beat the game in baseline mode use one of the settings that penalizes death, "
      "to train themselves to avoid it at any cost. When you Elude Death for the "
      "first time, or when you start the game with this switch set to NO DEATH, "
      "the game is placed in Explore Mode." },
  { OPT_NO_ALLY_TRANS, "Disable Complicity Transgressions", "NO/YES", 1, 0, 0,
      "This option causes the game to not count the actions of your allies against "
      "your own alignment. It is a short-term fix until the issue with allies "
      "and mounts attacking neutral creatures can be fixed." },
/* Terminal Options */
  { OPT_ROGUELIKE, "Roguelike Keyset", "OFF/ON", 1, 0, 0,
      "__This option causes the traditional roguelike movement keys (hjklyubn) "
      "to move your character, with their normal functions being remapped "
      "elsewhere." },
/*
  { OPT_KEYBOARD, "Keyboard Type", "Standard/Norwegian/Spanish/Polish", 2, 0, 0,
      "__This option allows a user with a Norwegian, Spanish or Polish "
      "keyboard layout to have their scan codes recognized properly by "
      "Incursion." }, 
*/
  { OPT_SIDEBAR, "Sidebar", "Default/Recent Only", 1, 0, 0,
      "__This option can cause the sidebar (that area on the right-hand side of the screen that lists your "
      "weapons and armour and the things in view) to omit some information that basically never changes "
      "(e.g., your race and gender) in order to leave more room for other information (e.g., things in view)." },
  { OPT_SHIFTARROW, "Shift Arrow Keys", "Ranged Attack/Run", 1, 0, 0,
      "__This option determines whether pressing Shift and an arrow key "
      "will activate a ranged attack shortcut or cause you to run in that "
      "direction." },
  { OPT_LOWERSCREEN, "Lower Screen Shows", "Viewlist/Resists/Pack/Equipment/Party/Quickkeys/Spellkeys", 6, 0, 0,
      "__This option determines what is shown in the lower part of the status "
      "bar -- the viewlists, active resistance levels, party status, quick key "
      "assignments or spell key assignments." },
  { OPT_SHOW_PILES, "List Piles in Viewlist", "NO/YES/NEAR", 2, 1, 0,
      "__When this option is set to No, piles of two or more object will not "
      "have their contents shown in the view list. If it is set at yes, they "
      "will be shown. If it is set at Near, only piles that are within one "
      "square of the player will have their contents listed in the view list "
      "window." }, 
  { OPT_MESSPARA, "Message Window Lines", "Paragraphs/Lines", 1, 0, 0,
      "__This option determines how messages are displayed in the upper "
      "message window -- whether every sentence begins on a new line, or "
      "whether sentences form paragraphs without being separated by carriage "
      "returns. The 'lines' option is more readable to many, but results in "
      "you seeing the --more-- prompt more frequently." },
  { OPT_AUTOPICKUP, "Autopickup", "NO/GEAR/PACK", 2, 2, 0,
      "__This option determines whether the player will automatically "
      "pick up any item she walks over that would stack with an item "
      "in her gear or in her pack."},
  { OPT_AUTOOPEN, "Auto-open Doors", "NEVER/ASK/YES", 2, 0, 0,
      "__If this option is set to YES, the player will automatically attempt "
      "to open doors by walking into them; ASK prompts the player, whereas "
      "never means doors must be opened with the 'o' command." },
  { OPT_AUTOKICK, "Auto-kick Doors", "NO/YES", 1, 0, 0,
      "__If this option is active, trying to open a locked door will cause "
      "the player character to kick it if he hs already tried and failed "
      "to pick the lock." },
  { OPT_AUTOCHEST, "Auto-open Chests", "NO/YES", 1, 0, 0,
      "__This option determines whether the player will automatically "
      "open any chest by walking over it."}, 
  { OPT_AUTOKNOCK, "Auto-Knock Locks", "NO/YES", 1, 0, 0,
      "__If this option is active, a player character that knows an 'Unlock' spell (like 'Knock') will "
      "automatically cast it when a Lockpicking skill check fails." },
  { OPT_AUTODISARM, "Auto-disarm Traps", "NO/YES", 1, 0, 0,
      "__If this option is active, walking into a known trap will cause "
      "the player to attempt to disarm it, provided that the player has any skill ranks in Handle Devices." },
  { OPT_REPEAT_KICK, "Repeat Kicking Doors", "NO/YES", 1, 1, 0,
      "__This switch causes the player to continue kicking a door until "
      "either it breaks open or he is interrupted when it is active." },  
  { OPT_LIST_PILE, "Show Pile Contents", "NO/YES", 1, 1, 0,
      "__This option determines whether the contents of a pile of "
      "objects on the ground will automatically be listed in a pop-up "
      "window whenever the player walks over it." },
  { OPT_SORT_PILE, "Sort Pile Contents", "NAME/TYPE/QUALITY", 2, 1, 0,
      "__This option determines the order in which items in piles are"
        " presented to you. In particular, when you Get an item from the"
        " ground, this sort order will be used." },
  { OPT_LOWHP_WARN, "Low Hit Point Warning", 
      "None/10~/20~/30~/40~/50~/60~/70~/80~/90~", 9, 3, 0,
      "__This option determines what percentage of hit points the player "
      "must have lost in order to receive a low hit point warning message." },
  { OPT_LOWHP_AGG, "Aggressive Low HP Warning", "NO/YES", 1, 0, 0,
      "__When this option is active, the player must explicitly press "
      "[ENTER] to get rid of the low HP warning message." },
  { OPT_WARN_EMPTY_HAND, "Empty Hand Warning", "NEVER/SMART/ALWAYS", 1, 1, 0,
    "__This option determines whether an aggressive warning is displayed when you are empty handed -- "
    "because you were disarmed by an opponent or forced to drop your weapon when reduced in size, for "
    "example. This sort of thing would be easy to notice in 'real life' but often gets lost in the "
    "noise in Roguelikes, causing you to waste precious time punching nearby monsters. The 'smart' "
    "setting disables the warning if you are polymorphed or have the Unarmed Strike ability." },
  { OPT_SHOWPATTK, "Show Player Attacks", "NO/YES", 1, 1, 0,
      "__This option determines whether the numeric aspect of the player's "
      "attacks will be listed in a window at the bottom of the screen." },
  { OPT_SHOWMATTK, "Show Monster Attacks", "NO/YES", 1, 0, 0,
      "__This option determines whether the numeric aspect of enemy "
      "attacks will be listed in a window at the bottom of the screen." },
  { OPT_SHOWSAVES, "Show Saving Throws", "NO/YES", 1, 1, 0,
      "__This option determines whether the results of the player's "
      "saving throws will be shown in a window at the bottom of the "
      "screen." },
  { OPT_SHOWSKILLS, "Show Skill Checks", "NO/YES", 1, 1, 0,
      "__This option determines whether the results of the player's "
      "skill checks will be shown in a window at the bottom of the "
      "screen." },
  { OPT_AUTOMORE, "Automatically clear \"-- more --\"", "NO/YES", 1, 0, 0,
      "__This option determines whether the game will automatically "
      "display all incoming messages without waiting for the player "
      "to press a key. (Messages can be reViewed later.)" }, 
  { OPT_DESC_ROOM, "Show Room Descriptions", "NO/YES/NEW", 2, 2, 0,
      "__This option determines whether the game will display a window "
      "showing the description of a room when you enter it -- NO (never), "
      "YES (always) or NEW (only if you've never been in a room of that "
      "type before." },  
  { OPT_STORE_ROLLS, "Store Rolls in Log", "NO/YES", 1, 1, 0,
      "__This option determines whether the results of dice rolls (attack "
      "rolls, damage rolls, saving throws and skill checks) will be stored "
      "in the message log -- the list of recent messages viewed with the "
      "'v' command -- or not." },
/* Later, centralize all the door options here; for now, leave it as is.
  { OPT_BUMP_DOOR, "Walking into Doors Behaviour", "NOTHING/OPEN/PICK/ASK/KICK", 4, 0, 0,
      "__This option defines what happens when the player character runs into "
      "a door -- he might do nothing, try to open it, try to open it or pick the "
      "lock if it is locked, ask whather to open it or pick it, or simply default "
      "to kicking it in if it is locked." },
*/
  { OPT_SELECT_JUMP, "Allow Selection Jumping", "NO/YES", 1, 1, 0,
      "This option determines how the selection prompt works when trying to "
      "select a specific creature or item. By default, pressing an arrow key "
      "will move the cursor to the next closest eligible item in that direction. "
      "However, if selection jumping is turned off, the arrow keys only move "
      "the selection cursor one square, just like when selecting a location. "
      "Some players may find this latter mode more intuitive." },
  { OPT_SELECT_DIR, "Default to Direction Selection", "NO/YES", 1, 1, 0, 
      "When you cast a spell or perform an action that allows a direction as "
      "input (such as many bolt spells), pressing any one of the arrow keys "
      "will finish the selection prompt, firing the spell in the selected "
      "direction. This allows the casting of offensive spells with a premium "
      "of keystrokes, but makes it easier to misfire spells with typos. "
      "When turned off, this option causes the selection prompt to default "
      "to target selection mode rather than direction selection mode." },
  { OPT_CLEAR_EVERY_TURN, "Clear Messages Every Turn", "NO/YES", 1, 0, 0,
      "If this option is active, the message window at the top of the "
      "screen will be cleared before each action, so that they player "
      "can confirm that all the displayed messages (i.e., an AutoPickup "
      "message) resulted from the last command keystroke entered. Normally, "
      "the message window is only cleared when an action produces a message. "
      "\n__Note that regardless of this setting, the message window is never "
      "cleared when a key is held down to repeat an action." },
  { OPT_CONTAINER_CURSOR, "Use Container Cursor", "NO/YES", 1, 0, 0,
      "This option activates an alternate inventory management mode when turned "
      "on, allows the inventory cursor to move down "
      "into the container area, to select items from the container manually. "
      "In this case, typing an item's number moves the cursor to that item "
      "rather than selecting it, and container items can be swapped with the "
      "In-Air slot by pressing [SPACE], or examined with [x]." },
  { OPT_ORG_CONTAIN, "Organize Containers", "NO/YES", 1, 1, 0,
      "When this option is active, the contents of containers are listed in "
      "the inventory screen sorted into groups by item type. This might be "
      "more intuitive for some players (especially NetHack veterans, since "
      "NetHack handles inventory like this), but takes up more screen space "
      "and thus requires more frequent scrolling of the container subwindow." },

/* Display Options */
  { OPT_PLAYER_COL, "Player Glyph Colour", 
    "Black/Blue/Green/Cyan/Red/Purple/Brown/Grey/Shadow/Azure/Emerald/Skyblue/Pink/Magenta/Yellow/White/By Race", 16, 16, 0,
      "__This option determines the colour of the glyph used to represent the player's "
      "character. By default, this glyph's color is set by the character's race." },
  { OPT_CENTER_MAP, "Center Map on Player", "NO/YES", 1, 0, 0,
      "__When this option is active, the map scrolls every time the "
      "player moves in order to keep him in the center of the map "
      "window at all times." },
  { OPT_SHOW_OFFS, "Offscreen Creatures Shown", "None/Range 5/Range 10/Range 15", 3, 1, 0,
      "__When this option is active, creatures that are off the edge of "
      "the map by up to the listed range in squares but are still visible "
      "to the player's character are shown at the edge of the map with a "
      "red background." },
  { OPT_HIGH_HIDE, "Highlight Hiding Creatures", "NO/YES", 1, 1, 0,
      "__When this option is active, monsters that are hiding in shadows "
      "but are still visible to you appear with a grey background." },
  { OPT_HIGH_FRIEND, "Highlight Non-Hostile Creatures", "NO/YES", 1, 0, 0,
      "__When this option is active, neutral creatures appear with a cyan "
      "background, and companions appear with a bright cyan background." },
  { OPT_HIGH_SLEEP, "Highlight Sleeping Creatures", "NO/YES", 1, 0, 0,
      "__When this option is active, sleeping monsters appear with a dark "
      "purple background highlight to signify their state." },
  { OPT_HIGH_MALADY, "Highlight Afflicted Creatures", "NO/YES", 1, 0, 0,
      "__When this option is active, creatures that have been assigned an "
      "obviously apparent negative stati (i.e., Stunned, Confused, Paralyzed, "
      "Blind, Stuck, Prone, etc.) are drawn with a brown background "
      "highlight to make their condition obvious." },
  { OPT_HIGH_INVIS, "Highlight Invisible Creatures", "NO/YES", 1, 0, 0,
      "__When this option is active, creatures that are invisible or phased "
      "into an adjacent plane of existence (i.e., a phase spider or <9>wraithform<2>ed "
      "mage) are drawn with a blue background." },
  { OPT_ANIMATION, "Animation Behaviour", "NORMAL/FAST/NONE/PLAYER", 3, 0, 0,
      "__Certain situations (a <9>fireball<2> spell, ranged attacks, etc.) have "
      "an animation effect associated with them. This option determines how that "
      "effect will be displayed:\n"
      "__<10>Normal:<2> All animations are displayed with their normal timing.\n"
      "__<10>Fast:<2> All animations are sped up to occur within 1/4 of their "
      "normal duration.\n"
      "__<10>None:<2> No animations are shown. This may make it difficult to "
      "tell the areas of effect of spells and where arrows landed, but it will "
      "make the game run faster in regions where many ranged attack or spells "
      "are being thrown at once.\n"
      "__<10>Player:<2> Only animations for the player's attacks and actions "
      "are shown. Monsters' missiles and spells are not animated." },
  { OPT_SHOW_HOW_SEE, "Show Perception Modes", "NO/YES", 1, 0, 0,
      "__When this option is active and a creature is examined with the 'l'ook"
      "command, a description of how the created is perceived (e.g., sight, "
      "infravision, detection, ESP) will be given." },
  { OPT_CONTEXT_HELP, "Context-Based Help", "NO/YES", 1, 1, 0,
      "__When this option is active, the help menus only show descriptions "
      "for the race and class(es) that you are currently playing, rather then "
      "every race and class in the game." },
  { OPT_HELP_MODE, "Help Format", "BOOK/MENU", 1, 0, 0,
      "__Determines the format that the online help will be shown in. In BOOK "
      "mode, lists of feats, classes and such are shown in alphabetical order "
      "to allows browsing, whereas in MENU mode the user may scroll through "
      "menus to get the description of a given feat. The former is better for "
      "learning the game; the latter is better for quick reference." },
  { OPT_MONO_HELP, "Monochrome Help", "OFF/ON", 1, 0, 0,
      "__If this option is enabled, all of the main text of the online help "
      "will be displayed as plain grey text, rather than the more colourful "
      "default. This can be useful for people that have trouble reading the "
      "help due to low-contrast monitors or visual disabilities." },
  { OPT_SORT_SPELLS, "Learn Spell Menu Order", "NAME/LEVEL", 1, 0, 0,
      "__Determines whether the spells in the 'learn a new spell' menu will be "
      "sorted by name or by level -- the former is better is you have spell "
      "levels memorized, but the latter might be clearer for new players." },
  { OPT_SHOW_ALL_SPELLS, "Show All Possible Spells", "NO/YES", 1, 0, 0,
      "__If this option is set, all arcane spells allowed to a character of "
      "your class and spell level will be shown on the Learn Spell menu, "
      "even if you don't have a spellbook with the spell in question in it. " },
  
  { OPT_TERSE_BLESSED, "Terse Blessed/Cursed Info", "NO/YES", 1, 0, 0,
      "__When this option is active, common item qualifiers will be abbreviated to save space. Blessed ('b'), cursed ('c'), "
      "uncursed (' '), unknown ('?') and ghost touch ('g') are affected."}, 
  { OPT_PRECISE_TIME, "Precise Time", "MINUTES/SECONDS/TURNS", 2, 0, 0,
      "__This option determines how accurate the clock in the sidebar will be. The default option tracks hours and minutes. "
      "It is also possible to track seconds or 'internal game turns' (5 turns to a second)." },

  { OPT_FULL_RES, "Fullscreen Display Resolution",
      "640x480/800x600/1024x768/1280x960/1280x1024/1400x1050/1600x1200/"
      "1280x768 (wide)/1280x800 (wide)/1920x1080 (wide)/1920x1200 (wide)", 10, 2, 0,
      "__This option determines what resolution Incursion will attempt to "
      "set the monitor to when playing the game in fullscreen mode. If the "
      "specified display mode cannot be set, the system will try consecutively "
      "smaller ones until one proves usable." },
  { OPT_FULL_FONT, "Fullscreen Font Size", "8x8/12x16/16x12/16x16", 3, 1, 0,
      "__This option determines what font size Incursion will use "
      "when playing the game in fullscreen mode. If the selected font "
      "size does not allow the display of at least 50 rows and 80 columns, "
      "the 8x8 font will be used instead." },
  { OPT_WIND_RES, "Windowed-Mode Window Size",
      "640x480/800x600/1024x768/1280x960/1280x1024/1400x1050/1600x1200/"
      "1280x768 (wide)/1280x800 (wide)/1920x1080 (wide)/1920x1200 (wide)", 10, 1, 0,
      "__This option determines what size Incursion will make the application "
      "window when playing in windowed mode. If the window is too large to "
      "fit on screen, the system will try consecutively smaller sizes until "
      "it finds one which fits." },
  { OPT_WIND_FONT, "Windowed-Mode Font Size", "8x8/12x16/16x12/16x16", 3, 1, 0,
      "__This option determines what font size Incursion will use "
      "when playing the game in windowed mode. If the selected font "
      "size does not allow the display of at least 50 rows and 80 columns, "
      "the 8x8 font will be used instead." },
  { OPT_SOFT_PALETTE, "Softer Palette Colors", "OFF/ON", 1, 0, 0,
      "__This option uses a 'softer' set of RGB values for the colors in "
      "the game's palette. This might make text more readable, increase "
      "contrast and appeal aesthetically to some players, but is a less "
      "accurate simulation of traditional ASCII graphics." },
      
/* Tactical Options */
#if 0
  { OPT_SUBDUAL, "Strike to Subdue", "OFF/ON", 1, 0, 0,
      "__When this option is active, characters inflict subdual damage with any "
      "blunt or slashing weapon they use. With slashing weapons, the attacker "
      "suffers a -4 penalty to the attack roll due to the awkwardness." },
  { OPT_BUMP, "Attack on Bump", "OFF/ON/ASK", 2, 0, 0,
      "__When you run into an invisible creature, or any creature while blind, "
      "this option determines whether you will attack it immediately or simply "
      "bump into it and realize that it is present." },
  { OPT_OPPOR, "Opportunity Attack Type", "None/Normal/Offhand/Trip/Grapple/Kick", 5, 1, 0,
      "__This option determines what the player will do when presented with the "
      "chance to make an attack of opportunity -- do nothing, make a normal attack, "
      "attack with an offhand weapon, try to trip the enemy, try to grapple the "
      "enemy or try to kick the enemy." },
#endif
  { OPT_KICK, "Kick by Default", "NO/YES", 1, 0, 0,
      "__When this option is active, the player will kick by default when making "
      "unarmed attacks. Kicking is slower, but it allows the player to add 1.5 "
      "times his normal Strength bonus to damage." },
#if 0
  { OPT_SHIFT_ATTK, "Shift+[DIR] Shortcut", "None/Ranged Attack/Great Blow/Run/Trip/Grapple/Bull Rush/Move", 7, 1, 0,
      "__This option determines what action you will perform when you type the "
      "[SHIFT]+[DIR] option:\n"
      "__<10>None: <2> The shortcut does nothing.\n"
      "__<10>Ranged Attack: <2> You will make a ranged attack in the direction given "
      "provided that you have a default ranged weapon ready and a default type of "
      "ammunition available. Default weapons are described in the inventory help.\n"
      "__<10>Great Blow: <2> The shortcut causes you to perform a Great Blow on "
      "whatever creature is immediately beside you in the direction indicated.\n"
      "__<10>Run: <2> The shortcut causes you to run in the direction indicated. "
      "See 'Gameplay Options' for more options used to customize the behaviour "
      "of the Run command.\n"
      "__<10>Trip: <2> You will make a trip attack against the creature in the "
      "direction you typed.\n"
      "__<10>Grapple: <2> You will initiate a grapple with a creature immediately "
      "beside you in the direction you typed.\n"
      "__<10>Bull Rush: <2> You will bull rush a creature immediately beside you "
      "in the direction that you typed.\n"
      "__<10>Move: <2> You will walk one square in the direction you typed, even "
      "if you could normally attack in that direction. This allows you to walk "
      "through incorporeal creatures (or any creatures if you are incorporeal) "
      "and to close with a creature while wielding a reach weapon." },
  { OPT_CTRL_ATTK, "Ctrl+[DIR] Shortcut", "None/Ranged Attack/Great Blow/Run/Trip/Grapple/Bull Rush/Move", 7, 7, 0,
      "__This option determines what action you will perform when you type the "
      "[SHIFT]+[DIR] option:\n"
      "__<10>None: <2> The shortcut does nothing.\n"
      "__<10>Ranged Attack: <2> You will make a ranged attack in the direction given "
      "provided that you have a default ranged weapon ready and a default type of "
      "ammunition available. Default weapons are described in the inventory help.\n"
      "__<10>Great Blow: <2> The shortcut causes you to perform a Great Blow on "
      "whatever creature is immediately beside you in the direction indicated.\n"
      "__<10>Run: <2> The shortcut causes you to run in the direction indicated. "
      "See 'Gameplay Options' for more options used to customize the behaviour "
      "of the Run command.\n"
      "__<10>Trip: <2> You will make a trip attack against the creature in the "
      "direction you typed.\n"
      "__<10>Grapple: <2> You will initiate a grapple with a creature immediately "
      "beside you in the direction you typed.\n"
      "__<10>Bull Rush: <2> You will bull rush a creature immediately beside you "
      "in the direction that you typed.\n"
      "__<10>Move: <2> You will walk one square in the direction you typed, even "
      "if you could normally attack in that direction. This allows you to walk "
      "through incorporeal creatures (or any creatures if you are incorporeal) "
      "and to close with a creature while wielding a reach weapon." },
#endif
  { OPT_AUTO_STAND, "Auto-Stand", "NO/YES", 1, 1, 0,
      "__If this option is enabled and you try to move while prone you will automatically try to stand up first. This takes time and provokes an attack of opportunity." },
  { OPT_WARN_DEQU, "Confirm Attacking Equipment Destroyers", "NO/YES", 1, 1, 0,
    "__If this option is enabled you will be asked to confirm before striking a creature that has a retributive equipment-destroying power (e.g., a rust monster or a grey ooze) if that power could possibly harm your weapon (or your bare hands!)." },
  { OPT_WARN_DROP, "Confirm Drop Equipment", "NO/YES", 1, 0, 0,
    "__If this option is active, you will be asked to confirm before dropping "
    "an item on the inventory screen." },
  
  { OPT_TWO_WEP, "Two-Weapon Fighting", "Never/Always/Smart/Bash", 3, 2, 0,
      "__This option determines when you will elect to make an attack with both "
      "weapons when wielding two weapons (or make an attack with both ends if "
      "wielding a double weapon) and when you will choose to only attack with "
      "your main weapon.\n"
      "__In Never mode, you will never attack with your offhand weapon. In Always "
      "mode, you will always make an offhand attack when you have something "
      "in your ready hands. In Smart mode, you will make an offhand attack only "
      "when wielding two actual weapons and when your character possesses the "
      "Two-Weapon Style feat. Bash mode is the same as Smart mode, but you will "
      "also make offhand Shield Bashes when wielding a shield in your off hand." },
  { OPT_AUTO_COUP, "Automatic Coup de Grace", "NO/YES", 1, 1, 0,
      "__When this option is active and you run into a helpless hostile creature, "
      "you will automatically perform a Coup de Grace rather then a normal attack. "
      "Note that this will provoke attacks of opportunity from any other creatures "
      "threatening you." },
  { OPT_RIPOSTE_MODE, "Riposte Mode", "NO/YES/ASK", 2, 1, 0,
      "__If you have the Riposte feat and a creature fails to hit you with a swung "
      "weapon, you may make an immediate counterattack against them at half of the "
      "timecost in phases of a normal attack. This tactical option determines whether "
      "you will ignore the opportunity, make the counterattack or whether the game "
      "will ask you each individual time the situation arises." },  
  { OPT_DWARVEN_AUTOFOCUS, "Dwarven Auto-Focus", "NO/YES", 1, 0, 0,
      "__When this option is active and you are playing a dwarf, the game will "
      "automatically choose appropriate targets for your Dwarven Focus."},
  { OPT_MONK_MODE, "Monk Preferred Attack Form", "NATURAL/MARTIAL", 1, 1, 0,
      "__This option determines how a character who has both a natural attack "
      "sequence and a monk's unarmed attack will behave when attacking unarmed. "
      "With MARTIAL, the character will use the monk unarmed strike table and "
      "gain all the normal benefits thereof, including stunning. With NATURAL, "
      "the character uses their natural attack sequence (usually claw/claw/bite) "
      "and gains a +1 skill bonus to speed and damage for every odd monk level. "
      "They do not gain any of the other normal benefits of a monk's unarmed "
      "strike, such as the stunning attack." },
  { OPT_OVERCAST, "Overcast Illusions", "NO/+1/+2/+3/+4/+5/+6/+7/+8/+9/+10/+11/+12", 12, 0, 0,
      "__An illusionist can create a truly impressive destructive spectacle if "
      "she desires -- the magnitude of her illusions is a selection rather than "
      "a limitation -- but doing so makes such spells less believable. When "
      "creating illusionary forces, this option determines the amount by which "
      "the caster 'increases' her caster level -- creating more powerful spells "
      "that inflict greater psychosomatic damage, but also lowering the DC of "
      "the saving throw to disbelieve them by -2 per +1 'fake' caster level "
      "gained. This option has no effect on illusionary creatures, items or "
      "traps created by the caster." },  
  { OPT_WAIT_FOR_PETS, "Wait for Companions", "NO/10/20/30/40/50/60", 6, 0, 0,
      "__This option allows the player to move slower in order to allow his "
      "companions to keep up with him. When it is turned on, every single "
      "move the player makes has its Timecost increased by the specified "
      "number of segments, provided that the player can't currently perceive "
      "any hostile creatures, and there is at least one living (undead, "
      "whatever) ally of the player somewhere on the map. The player remains "
      "able to use her full movement any time hostile creatures are within her "
      "line of sight. This option is currently implemented in a processing-intensive "
      "manner and may cause a slight slowdown. In addition, when this option "
      "is active, all summoned creatures, companions, etc. enter play by "
      "default with the order to travel next to you. This can be changed "
      "by giving them the Retarget order." },
  { OPT_COUNTERSPELL, "Counterspell Tactics", "NEVER/ALWAYS/ASK/SMART", 3, 2, 0,
      "__This option determines when a player character with the Spellcraft "
      "skill will attempt to perform counterspells, as described under that "
      "skill. The possibilities are as follows:\n"
      "__<10>NEVER:<2> The player character does not counterspell at all.\n"
      "__<10>ALWAYS:<2> The player character counterspells whenever the "
      "opportunity arises.\n"
      "__<10>ASK:<2> The player is prompted for yes/no whenever his character "
      "has an option to counterspell.\n"
      "__<10>SMART:<2> The player will counterspell whenever doing so would "
      "not put him under half his total, non-committed mana, and when he does "
      "not already have a timeout of 50 or greater." },
  { OPT_POISON_MERGE, "Poisoned Missile Stacking", "OFF/ON", 1, 0, 0,
      "__When a creature strikes another creature with a poisoned weapon, "
      "the number of doses of poison on that weapon is reduced by one. Once "
      "the doses reach zero, the weapon is no longer poisoned. This option "
      "determines whether weapons with different number of doses of poison "
      "left will stack, forming a single pile of items.\n"
      "__<10>NO:<2> Poisoned missiles never stack. This produces behaviour "
      "that is always logically correct, but can be make large numbers of "
      "unmergable piles of arrows, which might be frusterating for rogue-archers "
      "to manage in gameplay.\n"
      "__<10>YES:<2> When relevant, an arrow with less doses of poison remaining "
      "will stack with a pile of arrows poisoned with the same poison. The new "
      "pile has a 1 in (number of arrows) chance of every arrow having the lower "
      "number of doses of poison left. For example, if a rogue-archer has 40 "
      "sheaf arrows poisoned with 5 doses of deathblade, and hits a aurumvorax "
      "with five of them, those five arrows will then have 4 doses of deathblade "
      "each. When the rogue-archer picks them up again, and they merge back into "
      "the big stack of 40 arrows, the whole stack will have a 5 in 40 chance of "
      "having 4 doses of deathblade and a 35 in 40 chance of having the same 5 "
      "doses of deathblade it started with." },
  { OPT_AUTOHIDE, "Automatic Hide in Shadows", "OFF/ON", 1, 0, 0,
      "__When this option is turned on, a character with Hide in Shadows as a "
      "class skill will always attempt to hide when not already hidden, not "
      "standing in a brightly lit area and not observed by other hostile "
      "creatures." },
  { OPT_DEMORALIZE, "Use Demoralize", "OFF/ON", 1, 1, 0,
      "__When this option is on, your character will demoralize foes whenever "
      "the opportunity arises, causing them to flee. If it is turned off, the "
      "character will conceal the effect of his Intimidate and not cause foes to "
      "flee when they otherwise wouldn't." },
  { OPT_WAND_OPENING, "Use Wand of Opening", "OFF/ON/ASK", 1, 2, 0,
      "__This option determines whether the AutoLoot macro will search through "
      "your inventory for known Wands of Opening to use to try and open chests. If it "
      "is set to ASK, the macro will confirm before every shot. If it is set to "
      "ON, the game will try to zap the wand until you are at 1/3rd of your total "
      "non-held mana. If it is set to OFF, wands will not be used to open chests." },
  { OPT_USE_GAZE, "Use Gaze Attack", "OFF/ON", 1, 1, 0,
      "__If you should gain a gaze attack from some source (such as the spell "
      "<9>open the third eye<2>), this switch allows you to turn it on and off. "
      "Gaze attacks consume 2 mana per creature potentially affected, and this "
      "mana does not regenerate, so use your gaze sparingly!" },
  { OPT_DIRTY_FIGHTING, "Use Dirty Fighting", "OFF/ON/SMART", 2, 2, 0,
      "__This option determines whether a character that has the Dirty Fighting "
      "feat (such as a kobold) will use it. When it is set to OFF, it will not "
      "be used; when set to ON, it will. When set to SMART, it will only be used "
      "if your desired alignment is not lawful." },

/* Gameplay Options */
  { OPT_STOP_CORNER, "Stop Running at Corners", "NO/YES", 1, 0, 0,
      "__When this option is active, characters will stop running whenever "
      "a corridor they are in changes direction. With this active, a character "
      "will only ever run in the single direction he started running in." },
  { OPT_STOP_STAIRS, "Stop Running at Stairs", "NO/YES", 1, 0, 0,
      "__When this option is active, a character stops running the instant "
      "a set of stairs are within her line of sight." },
  { OPT_STOP_INTER, "Stop Running at Intersections", "NO/YES", 1, 1, 0,
      "__When this option is active, a character will stop running as soon "
      "as they reach an intersection in a corridor -- a place where they "
      "could viably go more then one direction, not counting backtracking. "
      "This option has no effect in rooms." },
  { OPT_STOP_REGION, "Stop Running at Region Barrier", "NO/YES", 1, 0, 0,
      "__When this option is active, characters will stop running immediately "
      "before crossing the boundary to a new region -- for example, before "
      "walking into a room from a corridor." },
  { OPT_STOP_PHASE, "Stop Running at Phase Changes", "NO/YES", 1, 0, 0,
      "__When this option is active, characters will stop running immediately "
      "before crossing the boundary from a solid region to a non-solid region (or vice-versa). This option prevents incorporeal or "
      "earth-melding characters from accidentally running to the edge of the map."}, 
  #if 0 
  { OPT_REST_BEHAVE, "Rest by Turn Behaviour", "Single Turn/Full Mana", 1, 0, 0,
      "__This option determines how the rest by turn command ('.') operates: "
      "either resting for a single turn and then asking for another command, "
      "or resting as long as mana is still regenerating unless interrupted." },
  #endif
  { OPT_KILL_CHEST, "Destroy Empty Chests", "NO/YES", 1, 0, 0,
      "__This option (which should really be replaced by a general auto-squelch) will destroy (for free, taking zero time) any empty "
      "chest you are standing on."},

/* Wizard Options */
  { OPT_NODEATH, "Cheat Death", "NO/YES", 1, 0, 0,
      "__When this option is active, you will be offered a prompt to avoid "
      "dying when you would normally be killed. Choosing 'yes' allows you "
      "to return to life with full hit points and lethal attribute damage, "
      "diseases and poisons healed." },
  { OPT_AUTOID, "Auto-Identify Items", "NO/YES", 1, 0, 0,
      "__When this option is active, you will automatically full-identify "
      "any item that you pick up." },
  { OPT_FREE_ADV, "Free Advancement", "NO/YES", 1, 0, 0,
      "__When this option is active, you can advance levels without having the "
      "amount of experience needed to gain the level in question using the "
      "Gain Level option on the character screen." },
  { OPT_NO_FATIGUE, "Infinite Fatigue", "NO/YES", 1, 0, 0,
      "__When this option is active, you will never lose fatigue points when "
      "you take actions or suffer effects that would normally cause them to "
      "be expended." },
  { OPT_SAFEREST, "Always Rest Safely", "NO/YES", 1, 0, 0,
      "__When this option is active, you will never suffer an encounter while "
      "resting, and no new monsters will be generated either." },
  { OPT_NO_SPELLFAIL, "Ignore Spell Failure", "NO/YES", 1, 0, 0,
      "__When this option is active, the game will ignore the percentile spell "
      "success rate, considering any spell the player casts to be successful." },
  { OPT_ALL_SPELLS, "Know All Spells", "NO/YES", 1, 0, 0,
      "__When this option is active, you are considered to know all of the "
      "spells a character of your class and level is eligible for. For "
      "example, a level 5 mage would know all arcane spells of spell levels "
      "one to three." },
  { OPT_INF_MANA, "Infinite Mana", "NO/YES", 1, 0, 0,
      "__When this option is active, you do not lose mana from casting spells "
      "or using magical items the way you normally would, and can cast spells "
      "successfully even if you do not have enough mana to do so normally." },
  { OPT_GHOSTWALK, "Ghostwalking", "NO/YES", 1, 0, 0,
      "__When this option is active, your character is able to walk through "
      "walls and doors as if they do not exist. You are not incorporeal or "
      "ethereal, however, and can engage corporeal creatures in combat "
      "normally." },
  { OPT_WIZ_SIGHT, "Wizard Sight", "NO/YES", 1, 0, 0,
      "__When this option is active, your character perceives as much of the "
      "map as can be displayed on the screen at any given time regardless of "
      "lighting or obstructions." },
  { OPT_FREEZE_MON, "Freeze Monsters", "NO/YES", 1, 0, 0,
      "__When this option is active, all monsters are denied their activity "
      "cycles and are frozen in place. They are still a part of the world and "
      "can be affected by spells, take attacks of opportunity and so forth." },
  { OPT_ALL_ROLLS, "Show All Combat Rolls", "NO/YES", 1, 0, 0,
      "__When this option is active, all monster combat rolls (even those far "
      "off-screen) will be displayed to you. This information tends to be more "
      "confusing than useful, but it can help with debugging." }, 

/* Stability Options */
  { OPT_NO_FREE, "Never Call free()", "NO/YES", 1, 0, 0,
      "__When this option is set, Incursion will not attempt to reclaim "
      "allocated memory. In many causes this masks (or, more accurately, stops "
      "exposing) some memory corruption errors and may allow you to play a bit "
      "longer between crashes." }, 
  { OPT_NO_SCRIPTS, "Disable all Scripts", "NO/YES/PLAYER", 2, 0, 0,
      "__If this option is turned on, Incursion will not run any of its "
      "resource scripts using its own internal virtual machine. This means "
      "that many monsters lose their special attacks or conditions, about "
      "half of all magical items and spells won't work right and several "
      "aspects of chargen will be disabled. It is intended primarily for "
      "testing, and for determining the role the VMachine plays in crashes. "
      "The PLAYER choice causes scripts to run only if the player is the "
      "EActor or EVictim in the script, preventing the game from crashing "
      "due to script errors with two monsters fighting each other." },
  { OPT_NO_CASTING, "Disable Monster Casting", "NO/YES", 1, 0, 0,
      "__This option prevents monsters from casting spells or using active "
      "magical items such as wands or potions. It is intended for tracking "
      "down whether these things are the source of memory corruption and "
      "crashes." },
  
  { 0, NULL, NULL, 0, 0, 0, NULL }

  };

TextVal FileErrors[] = {
  { ENOFILE,     "File not Found" },
  { ENOTARC,     "File not an Archive" },
  { EBADVER,     "File Version Mismatch" },
  { ENOCHUNK,    "Chunk not Found" },
  { EWRIERR,     "File Write Error" },
  { EREADERR,    "File Read Error" },
  { EMEMORY,     "Out of Memory" },
  { ECORRUPT,    "File is Corrupt" },
  { ETAMPER,     "File Tampering Detected" },
  { EHANDLE,     "Bad Object Handle" },
  { ENODIR,      "Directory not Found" },
  { 0, NULL } };

const char* EncumbranceNames[] = {
  "None", "Light", "Moderate", "Heavy", "Extreme", "(null)" };

int16 EncumbranceTable[30*3] = {
  30,      60,      100,        // ww: previously this only had 29 rows
  30,      60,      100,        // and it was supposed to go to 30 ...
  60,     130,      200,
 100,     200,      300,
 130,     260,      400,
 160,     330,      500,
 200,     400,      600,
 230,     460,      700,
 260,     530,      800,
 300,     600,      900,
 330,     660,     1000,
 380,     760,     1150,
 430,     860,     1300,
 500,    1000,     1500,
 580,    1160,     1750,
 660,    1330,     2000,
 760,    1530,     2300,
 860,    1730,     2600,
1000,    2000,     3000,
1160,    2330,     3500,
1330,    2660,     4000,
1530,    3060,     4600,
1730,    3460,     5200,
2000,    4000,     6000,
2330,    4660,     7000,
2660,    5330,     8000,
3060,    6130,     9200,
3460,    6930,    10400,
4000,    8000,    12000,
4660,    9330,    14000 };

int8 SpellTable[27][9] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0 },  /* Apprentice Level */
  { 1, 0, 0, 0, 0, 0, 0, 0, 0 },  // 1
  { 2, 0, 0, 0, 0, 0, 0, 0, 0 },  // 2
  { 2, 1, 0, 0, 0, 0, 0, 0, 0 },  // 3
  { 3, 2, 0, 0, 0, 0, 0, 0, 0 },  // 4
  { 3, 2, 1, 0, 0, 0, 0, 0, 0 },  // 5
  { 4, 3, 2, 0, 0, 0, 0, 0, 0 },  // 6
  { 5, 4, 3, 1, 0, 0, 0, 0, 0 },  // 7
  { 6, 5, 3, 2, 0, 0, 0, 0, 0 },  // 8
  { 6, 6, 3, 3, 1, 0, 0, 0, 0 },  // 9
  { 7, 6, 4, 3, 2, 0, 0, 0, 0 },  // 10
  { 7, 7, 5, 4, 3, 0, 0, 0, 0 },  // 11
  { 7, 7, 5, 4, 4, 1, 0, 0, 0 },  // 12
  { 8, 7, 6, 5, 4, 2, 0, 0, 0 },
  { 8, 8, 6, 6, 5, 3, 1, 0, 0 },
  { 8, 8, 7, 6, 5, 4, 2, 0, 0 },
  { 8, 8, 7, 7, 6, 5, 3, 1, 0 },
  { 8, 8, 8, 7, 7, 6, 4, 2, 0 },
  { 8, 8, 8, 8, 7, 7, 5, 3, 1 },
  { 8, 8, 8, 8, 8, 7, 6, 4, 2 },
  { 8, 8, 8, 8, 8, 8, 7, 6, 4 },
  { 9, 9, 9, 9, 8, 8, 8, 8, 6 },
  { 9, 9, 9, 9, 9, 9, 9, 9, 8 },
  { 10, 10, 10, 9, 9, 9, 9, 9, 9 },
  { 11, 11, 11, 10, 10, 10, 9, 9, 9 },
  { 11, 11, 11, 11, 11, 11, 10, 10, 10 },
  { 12, 12, 12, 12, 12, 12, 11, 10, 10 } };

int8 BonusSpells[22][9] = {
  { 0,  0,  0,  0,  0,  0,  0,  0,  0 }, /* Int <= 9 */
  { 1,  0,  0,  0,  0,  0,  0,  0,  0 }, /* Int == 10 */
  { 1,  0,  0,  0,  0,  0,  0,  0,  0 }, /* Int == 11 */
  { 2,  1,  0,  0,  0,  0,  0,  0,  0 }, /* Int == 12 */
  { 2,  2,  0,  0,  0,  0,  0,  0,  0 }, /* Int == 13 */
  { 3,  2,  1,  0,  0,  0,  0,  0,  0 }, /* Int == 14 */
  { 3,  3,  2,  0,  0,  0,  0,  0,  0 }, /* Int == 15 */
  { 3,  3,  2,  1,  0,  0,  0,  0,  0 }, /* Int == 16 */
  { 4,  3,  3,  2,  0,  0,  0,  0,  0 }, /* Int == 17 */
  { 4,  4,  3,  2,  1,  0,  0,  0,  0 }, /* Int == 18 */
  { 4,  4,  3,  3,  2,  0,  0,  0,  0 }, /* Int == 19 */
  { 4,  4,  4,  3,  2,  1,  0,  0,  0 }, /* Int == 20 */
  { 5,  4,  4,  3,  3,  2,  0,  0,  0 }, /* Int == 21 */
  { 5,  5,  4,  4,  3,  2,  1,  0,  0 }, /* Int == 22 */
  { 5,  5,  4,  4,  3,  3,  2,  0,  0 }, /* Int == 23 */
  { 5,  5,  5,  4,  4,  3,  2,  1,  0 }, /* Int == 24 */
  { 5,  5,  5,  4,  4,  3,  3,  2,  0 }, /* Int == 25 */
  { 6,  5,  5,  5,  4,  4,  3,  2,  1 }, /* Int == 26 */
  { 6,  6,  5,  5,  4,  4,  3,  3,  2 }, /* Int == 27 */
  { 6,  6,  5,  5,  5,  4,  4,  3,  2 }, /* Int == 28 */
  { 6,  6,  6,  5,  5,  4,  4,  3,  3 }, /* Int == 29 */
  { 6,  6,  6,  5,  5,  5,  4,  4,  3 }  /* Int == 30 */
    };



/*
const int16 SpellbookTable[] =
  PM_BASIC_INCANTATIONS,
    SP_CALL_LIGHT, SP_MAGIC_MISSLE, SP_DETECT_MAGIC, SP_PHASE_DOOR,
    SP_SHIELD, SP_MAGE_ARMOUR, SP_SLEEP, 0,
  PM_ELDRICH_SORCERY,
    SP_BLACK_BLADE, SP_DISJUNCTION, SP_IMPRISONMENT, SP_SUCCOR,
      SP_SOUL_BIND, SP_MASTER_UNDEAD, SP_GRANDEUR, 0 };
*/

char __buffer[1600];
char __buff2[80];

const char*AttrTitle[] =
  { "STR", "DEX", "CON", "INT", "WIS", "CHA", "LUC",
  "Archery Speed",
  "Brawl Speed",
  "Melee Speed",
  "Thrown Speed",
  "Offhand Speed",

  "Archery Hit",
  "Brawl Hit",
  "Melee Hit",
  "Thrown Hit",
  "Offhand Hit",

  "Armour",
  "Magic Resistance",
  NULL,

  "Archery Damage",
  "Brawl Damage",
  "Melee Damage",
  "Thrown Damage",
  "Offhand Damage",

  "Size",
  "Movement",
  "Defense Class",
  "Fortitude Save",
  "Reflex Save",
  "Will Save",
  "Arcane Casting",
  "Divine Casting",
  "Temporary Hit Points",
  "Temporary Mana Points",
  "Fatigue Points",
  "Armour Coverage",
  }; // ww: evil typo
const char*RatingWord[] ={ "Abyssal", "Hideous", "Poor", "Average", "Fair",
	"Good", "Great", "Excellent", "Incredible", "Legendary",
	"Inhuman", "Divine" };
/*int16 RatingColor[] = { DARKGRAY, RED, RED, LIGHTGRAY, BROWN, BLUE, MAGENTA,
	GREEN, CYAN, LIGHTCYAN, YELLOW, WHITE };
  */

const char* KeyBindings =
"            -- %cINCURSION KEY BINDINGS%c --        \n\
    a   activate item       o   open door/chest      \n\
    b   blast wand/staff    p   pray to your god     \n\
    c   view character      q   quaff potion         \n\
    d   dig a tunnel        r   read scroll          \n\
    e   eat something       s   search               \n\
    f   fire ranged weapon  t   talk to creature     \n\
    g   get item            u   use skill/ability    \n\
    h   hide in shadows     v   reView messages      \n\
    i   inventory screen    w   wizard mode commands \n\
    j   jump                x   turn off power/item  \n\
    k   kick door/target    y   \"yuse\" item        \n\
    l   look around room    z   rest and recover     \n\
    m   cast spell          ?   show this screen     \n\
    n   name an item/pet    .   wait one turn        \n\
  ENTER enter store/guild  < >  go up/down stairs    \n\
    ?   display this screen /   list screen glyphs   \n\
    =   set game options    -   read notebook        \n\
   ESC  show system menu   TAB  show tactical menu   \n\
  [NUM] command party mem.  .   rest for 5 phases    \n\
  SPACE enter a specific, typed command              \n\
   \x18\x19\x1D  move or attack in given direction \n\
 -----------------------------------------------------\n\
                     %cFurther Help%c";
           
const char* GlyphLegend1 =
"                     <14>Characters Shown on Screen<7>                 \n\
   a  animal (misc)    A  angel / celestial   ?  unseen creature \n\
   b  bat / bird       B  beast               <char:GLYPH_STATUE>  statue          \n\
   c  cat / feline     C  construct           <char:GLYPH_LARMOUR>  armour           \n\
   d  dog / canine     D  dragon              <char:GLYPH_STORE>  building entrance\n\
   e  eye creature     E  elemental           <char:GLYPH_CHEST>  chest         \n\
   f  faerie creature  F  fungus              8  altar           \n\
   g  goblinkin        G  golem               <char:GLYPH_PIT>   pit             \n\
   h  humanoid         H  giant               7  porticullus     \n\
   i  impkin           I  illithid            %  food / corpse   \n\
   j  jelly / ooze     J  djinn / genie       <char:GLYPH_TREE>  tree / mushroom \n\
   k  kobold           K  elementalkin        $  money           \n\
   l  lycanthrope      L  lich                \"  fence / hedge   \n\
   m  mimic            M  mythic hybrid       :  pile of rubble  \n\
   n  naga             N  spectral undead     #  webbing / fence \n\
   o  orc              O  outsider            (  bladed weapon   \n\
   q  quadruped        P  plant creature      )  bow / crossbow  \n";
const char* GlyphLegend2 = "\
   r  rat / rodent     Q  quylthulg           <char:GLYPH_WEAPON>  other weapon    \n\
   s  spider           R  reptile             /  rod / staff     \n\
   t  trapper          S  swarm               -  wand            \n\
   u  hag              T  troll              > <char:GLYPH_USTAIRS> stairs / ladder \n\
   v  vortex           U  corporeal undead    +  door (open)     \n\
   w  worm             V  vampire             <char:GLYPH_HDOOR>  door (closed)   \n\
   x  vermin / insect  W  wraith / shadow     <char:GLYPH_BDOOR>  door (broken)   \n\
   y  snake-men        X  aberration          *  pile of objects \n\
   <char:GLYPH_PLAYER>  player character Y  hominid             <char:GLYPH_WATER>  water/lava/mud  \n\
   <char:GLYPH_HUMAN>  (demi)human NPC  Z  zombie / skeleton   <char:GLYPH_TRAP>  trap            \n\
   <char:GLYPH_LDEMON>  lesser demon     <char:GLYPH_LDEVIL>  lesser devil        <char:GLYPH_DISARMED>  trap, disarmed \n\
   <char:GLYPH_GDEMON>  greater demon    <char:GLYPH_GDEVIL>  greater devil       <char:GLYPH_FOG>  fog             \n\
   <char:GLYPH_HELMET>  helmet/headband  `  swarm of insects    <char:GLYPH_JUNK>  junk / garbage\n\
   <char:GLYPH_CLOAK>  cloak    <char:GLYPH_BRACERS>  bracers    <char:GLYPH_RING>  ring     <char:GLYPH_LARMOUR>  armour\n\
   <char:GLYPH_GEM>  gemstone <char:GLYPH_GAUNTLETS>  gauntlets  <char:GLYPH_AMULET>  amulet   <char:GLYPH_SHIELD>  shield\n\
   <char:GLYPH_TOOL>  tool     <char:GLYPH_MUSH>  mushroom   <char:GLYPH_FIGURE>  figurine <char:GLYPH_GIRDLE>  girdle\n\
   <char:GLYPH_BOOK>  book     <char:GLYPH_POTION>  potion     <char:GLYPH_SCROLL>  scroll   <char:GLYPH_CLOTHES>  clothing";
     
ItemGen DungeonItems[] = {
/* Prob, Type,      Source,    NvGood, MinL,CMul,Prototype,   Flavored */
{ 50, T_WEAPON,     AI_WEAPON, false,  1,   100, NULL,        false },
{ 35, T_ARMOUR,      AI_ARMOUR,  false,  1,   100, NULL,        false },
{ 15, T_SHIELD,     AI_SHIELD, false,  1,   100, NULL,        false },
{ 15, T_BOW,        AI_BOW,    false,  1,   100, NULL,        false },
{ 5,  T_MISSILE,    AI_ARROW,  false,  1,   100, NULL,        false },
{ 8,  T_BOOK,       0,         false,  3,   200, NULL,        false },
{ 2,  T_STAFF,      AI_STAFF,  false,  7,   75,  "staff",     true },
{ 1,  T_ROD,        AI_ROD,    false,  12,  200, "rod",       true },
{ 40, T_POTION,     AI_POTION, true,   1,   5,   "potion",    true },
{ 20, T_SCROLL,     AI_SCROLL, true,   1,   3,   "scroll",    true },
{ 30, T_TOME,       AI_TOME,   false,  1,   2000,"tome",      false },
{ 2,  T_FIGURE,     0,         false,  10,  500, "figurine",  false },
{ 10, T_RING,       AI_RING,   false,  3,   100, "ring",      true },
{ 3,  T_AMULET,     AI_AMULET, false,  15,  100, "amulet",    true },
{ 15, T_FOUNTAIN,   AI_FOUNTAIN,false, 1,   0,   "fountain",  false },         
{ 40, T_WAND,       AI_WAND,   false,  2,   50,  "wand",      true },
{ 20, T_MUSH,       AI_MUSH,   true,   1,   4,   "mushroom",  true },
{ 2,  T_TOOL,       AI_HORN,   false,  3,   100, "horn",      true },
{ 5,  T_HELMET,     AI_HELM,   false,  3,   100, "helm",      true },
{ 3,  T_HELMET,     AI_HEADBAND,false, 3,   100, "headband",  true },
{ 1,  T_GEM,        AI_STONE,  false,  3,   100, "stone",     false },
{ 5,  T_CLOAK,      AI_CLOAK,  false,  5,   100, "cloak",     true },
{ 2,  T_GAUNTLETS,  AI_GAUNTLETS,false,3,   100, "gauntlets", true },
//{ 1,  T_GAUNTLETS,  AI_GAUNTLETS,false,3,   100, "gloves", true },
{ 3,  T_GIRDLE,     AI_GIRDLE, false,  3,   100, "girdle",    true },
{ 5,  T_BRACERS,    AI_BRACERS,false,  3,   100, "bracers",   true },
{ 10, T_BOOTS,      AI_BOOTS,  false,  1,   100, "boots",     true },
{ 3,  T_EYES,       AI_EYES,   false,  1,   100, "lenses",    true },
{ 0,  T_LIGHT,      0,         true,   1,   100, NULL,        false },
{ 30, T_TOOL,       0,         true,   1,   100, NULL,        false },
{ 10, T_TOOL,       AI_ALCHEMY,true,   1,   3,   "flask", false },
{ 1,  T_WONDER,     AI_WONDER, false,  1,   100, "placeholder", false },
{ 3,  T_CONTAIN,    0,         false,   1,   100, NULL,        false },
{ 0,  T_TOOL,       AI_POISON, false,  1,   50,  "small glass vial", false },
#ifndef WEIMER
{ 10, T_FOOD,       0,         true,   0,   100, NULL,        false },
#endif
{ 0,  0,            0,         false,  0,   0,   NULL,        false }
};

ItemGen IllusionItems[] = {
/* Prob, Type,      Source,    NvGood, MinL,CMul,Prototype,   Flavored */
{ 50, T_WEAPON,     AI_WEAPON, false,  1,   100, NULL,        false },
{ 20, T_ARMOUR,      AI_ARMOUR,  false,  1,   100, NULL,        false },
{ 10, T_SHIELD,     AI_SHIELD, false,  1,   100, NULL,        false },
{ 15, T_BOW,        AI_BOW,    false,  1,   100, NULL,        false },
{ 5,  T_STAFF,      AI_STAFF,  false,  7,   75,  "staff",     true },
{ 5,  T_ROD,        AI_ROD,    false,  12,  200, "rod",       true },
{ 30, T_TOME,       AI_TOME,   false,  1,   2000,"tome",      false },
{ 10, T_RING,       AI_RING,   false,  3,   100, "ring",      true },
{ 3,  T_AMULET,     AI_AMULET, false,  15,  100, "amulet",    true },
{ 5,  T_HELMET,     AI_HELM,   false,  3,   100, "helm",      true },
{ 3,  T_HELMET,     AI_HEADBAND,false, 3,   100, "headband",  true },
{ 1,  T_GEM,        AI_STONE,  false,  3,   100, "stone",     false },
{ 5,  T_CLOAK,      AI_CLOAK,  false,  5,   100, "cloak",     true },
{ 2,  T_GAUNTLETS,  AI_GAUNTLETS,false,3,   100, "gauntlets", true },
{ 1,  T_GAUNTLETS,  AI_GAUNTLETS,false,3,   100, "gloves",    true },
{ 3,  T_GIRDLE,     AI_GIRDLE, false,  3,   100, "girdle",    true },
{ 5,  T_BRACERS,    AI_BRACERS,false,  3,   100, "bracers",   true },
{ 10, T_BOOTS,      AI_BOOTS,  false,  1,   100, "boots",     true },
{ 3,  T_EYES,       AI_EYES,   false,  1,   100, "lenses",    true },
{ 20, T_WONDER,     AI_WONDER, false,  1,   100, "placeholder", false },
{ 0,  0,            0,         false,  0,   0,   NULL,        false }
};


ItemGen MonsterItems[] = {
{ 25, T_WEAPON,     AI_WEAPON, false,  1,   100, NULL,        false },
{ 15, T_ARMOUR,      AI_ARMOUR,  false,  1,   100, NULL,        false },
{ 50, T_POTION,     AI_POTION, true,   1,   5,   "potion",    true },
{ 10, T_RING,       AI_RING,   false,  3,   100, "ring",      true },
{ 1,  T_AMULET,     AI_AMULET, false,  15,  100, "amulet",    true },
{ 3,  T_BRACERS,    AI_BRACERS,false,  3,   100, "bracers",   true },
{ 3,  T_CLOAK,      AI_CLOAK,  false,  5,   100, "cloak",     true },
{ 10, T_BOOTS,      AI_BOOTS,  false,  1,   100, "boots",     true },
{ 5,  T_WONDER,     AI_WONDER, false,  1,   100, "placeholder", false },
{ 0,  0,            0,         false,  0,   0,   NULL,        false },
{ 1,  T_CONTAIN,    0,         false,   1,  100, NULL,        false },

};

ItemGen MageItems[] = {
/* Prob, Type,      Source,    NvGood, MinL,CMul,Prototype,   Flavors */
{ 50, T_POTION,     AI_POTION, true,   1,   5,   "potion",    true },
{ 35, T_SCROLL,     AI_SCROLL, true,   1,   3,   "scroll",    true }, 
{ 25, T_WAND,       AI_WAND,   false,  2,   50,  "wand",      true },
{ 10, T_WEAPON,     AI_SWORD,  false,  1,   100, NULL,        false },
{ 10, T_RING,       AI_RING,   false,  3,   100, "ring",      true },
{ 5,  T_AMULET,     AI_AMULET, false,  15,  100, "amulet",    true },
{ 0,  0,            0,         false,  0,   0,   NULL,        false }

};

ItemGen ChestItems[] = {

{ 25, T_WEAPON,     AI_WEAPON, false,  1,   100, NULL,        false },
{ 15, T_ARMOUR,      AI_ARMOUR,  false,  1,   100, NULL,        false },
{ 5,  T_MISSILE,    AI_ARROW,  false,  1,   100, NULL,        false },
{ 30, T_BOOK,       0,         false,  3,   200, NULL,        false },
{ 5,  T_STAFF,      AI_STAFF,  false,  7,   75,  "staff",     true },
{ 5,  T_ROD,        AI_ROD,    false,  12,  200, NULL,        true },
{ 40, T_POTION,     AI_POTION, true,   1,   5,   "potion",    true },
{ 10, T_SCROLL,     AI_SCROLL, true,   1,   3,   "scroll",    true },
{ 2,  T_FIGURE,     0,         false,  5,   500, "figurine",  false },
{ 10, T_RING,       AI_RING,   false,  3,   100, "ring",      true },
{ 10, T_AMULET,     AI_AMULET, false,  15,  100, "amulet",    true },
{ 30, T_WAND,       AI_WAND,   false,  2,   50,  "wand",      true },
{ 2,  T_TOOL,       AI_HORN,   false,  3,   100, "horn",      true },
{ 5,  T_HELMET,     AI_HELM,   false,  3,   100, "helm",      true },
{ 3,  T_HELMET,     AI_HEADBAND,false, 3,   100, "headband",  true },
{ 1,  T_GEM,        AI_STONE,  false,  3,   100, "stone",     false },
{ 5,  T_CLOAK,      AI_CLOAK,  false,  5,   100, "cloak",     true },
{ 2,  T_GAUNTLETS,  AI_GAUNTLETS,false,3,   100, "gauntlets", true },
{ 1,  T_GAUNTLETS,  AI_GAUNTLETS,false,3,   100, "gloves", true },
{ 3,  T_GIRDLE,     AI_GIRDLE, false,  3,   100, "girdle",    true },
{ 5,  T_BRACERS,    AI_BRACERS,false,  3,   100, "bracers",   true },
{ 10, T_BOOTS,      AI_BOOTS,  false,  1,   100, "boots",     true },
{ 40, T_TOOL,       0,         true,   1,   100, NULL,        false },
{ 40, T_WONDER,     AI_WONDER, false,  1,   100, "placeholder", false },
{ 3,  T_EYES,       AI_EYES,   false,  1,   100, "lenses",    true },
{ 3,  T_CONTAIN,    0,         false,  2,   100, NULL,        false },
{ 90, T_COIN,       0,         true,   1,   1,   NULL,        false },
{ 0,  0,            0,         false,  0,   0,   NULL,        false }

};
  
ItemGen StapleItems[] = {

{ 10, T_BOOK,       0,         false,  3,   200, NULL,        false },
{ 50, T_POTION,     AI_POTION, true,   1,   5,   "potion",    true },
{ 30, T_SCROLL,     AI_SCROLL, true,   1,   3,   "scroll",    true },
{ 10, T_LIGHT,      0,         true,   1,   100, NULL,        false },
{ 10, T_TOOL,       0,         true,   1,   100, NULL,        false },
{ 0,  0,            0,         false,  0,   0,   NULL,        false },

};

YuseCommand YuseCommands[] = {
  { EV_APPLY, "Apply", "applied",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to apply it to?",
       /* EItem1  */ Q_INV|Q_OPT, "What do you want to apply?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ YU_REVERSE },              
  { EV_ACTIVATE, "Activate", "activated",
       /* ETarget */ 0, NULL,
       /* EItem1  */ Q_INV|Q_OPT, "What do you want to activate?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },  
  { EV_BARTER, "Barter With", "bartered with",
       /* ETarget */ Q_TAR|Q_CRE, "Who do you want to barter with?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },      
  { EV_BREAK, "Break", "broken",
       /* ETarget */ Q_INV, "What do you want to break?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },  
  { EV_BURN, "Burn", "burned",
       /* ETarget */ Q_INV, "What do you want to burn?",
       /* EItem1  */ Q_INV, "What do you want to burn it with?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },  
  { EV_CLEAN, "Clean", "cleaned",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to clean?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },  
  { EV_CLEAN, "Clean With", "cleaned",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to clean?",
       /* EItem1  */ Q_INV|Q_OPT, "What do you want to clean it with?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },  
  { EV_CLOSE, "Close", "closed",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to close?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },      
  { EV_COW, "Cow", "cowed",
       /* ETarget */ Q_TAR|Q_CRE, "Who do you want to cow?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },      
  { EV_CUT, "Cut", "cut",
       /* ETarget */ Q_INV, "What do you want to cut?",
       /* EItem1  */ Q_INV, "What do you want to cut it with?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_DIG, "Dig", "dug",
       /* ETarget */ Q_DIR, "What direction do you want to dig in?",
       /* EItem1  */ Q_INV|Q_TAR|Q_NEAR, "What do you want dig with?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ YU_REVERSE },
  { EV_DIP, "Dip", "dipped",
       /* ETarget */ 0, NULL,
       /* EItem1  */ Q_INV, "What do you want dip?",
       /* EItem2  */ Q_TAR|Q_ITM|Q_NEAR|Q_INV, "What do you want to dip it in?",
       /* Flags   */ 0 },
  { EV_DISMISS, "Dismiss", "dismissed",
       /* ETarget */ Q_TAR, "Who do you want to dismiss?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },      
  { EV_DISMOUNT, "Dismount", "dismounted",
       /* ETarget */ 0, NULL,
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },           
  { EV_DISSECT, "Dissect", "dissected",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to dissect?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_DISTRACT, "Distract", "distracted",
       /* ETarget */ Q_TAR|Q_CRE, "Who do you want to distract?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },      
  { EV_DIVIDE, "Divide", "divided",
       /* ETarget */ 0, NULL,
       /* EItem   */ Q_INV, "What do you want to divide?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },                   
  { EV_DRINK, "Drink", "drunk",
       /* ETarget */ 0, NULL,
       /* EItem   */ Q_INV, "What do you want to drink?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_EAT, "Eat", "eaten",
       /* ETarget */ 0, NULL,
       /* EItem   */ Q_INV, "What do you want to eat?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_ENLIST, "Enlist", "enlisted",
       /* ETarget */ Q_TAR|Q_CRE, "Who do you want to enlist?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },        
  { EV_ENTER, "Enter", "entered",
       /* ETarget */ Q_TAR|Q_NEAR, "What do you want to enter?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_EXAMINE, "Examine", "examined",
       /* ETarget */ Q_INV|Q_TAR, "What do you want to examine?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_FAST_TALK, "Fast Talk", "fast-talked",
       /* ETarget */ Q_TAR|Q_CRE, "Who do you want to fast-talk?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },      
  { EV_POUR, "Fill", "filled",
       /* ETarget */ Q_INV, "What do you want to fill?",
       /* EItem1  */ Q_INV, "What do you want to fill it with?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 }, 
  { EV_FORCE, "Force", "forced",
       /* ETarget */ Q_TAR|Q_INV|Q_NEAR, "What do you want to force open/apart?",
       /* EItem1  */ Q_INV, "What do you want to force it with?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 }, 
  { EV_GIVE, "Give", "given",
       /* ETarget */ Q_TAR|Q_NEAR|Q_CRE, "Who do you want to give it to?",
       /* EItem1  */ Q_INV, "What do you want to give away?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ YU_REVERSE }, 
  { EV_GREET, "Greet", "greeted",
       /* ETarget */ Q_TAR|Q_CRE, "Who do you want to greet?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_IMBUE, "Imbue", "imbued",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to imbue with positive energy?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_INSERT, "Insert", "inserted",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to put it in?",
       /* EItem1  */ Q_INV, "What do you want to insert?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ YU_REVERSE },
  { EV_INSCRIBE, "Inscribe", "inscribed",
       /* ETarget */ Q_INV|Q_DIR|Q_NEAR, "What do you want to inscribe?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_INSCRIBE, "Inscribe With", "inscribed",
       /* ETarget */ Q_INV|Q_DIR|Q_NEAR, "What do you want to inscribe?",
       /* EItem1  */ Q_INV, "What do you want to use to make the inscription?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_JAM, "Jam", "jammed",
       /* ETarget */ Q_TAR|Q_NEAR, "What do you want to jam?",
       /* EItem1  */ Q_INV, "What do you want to jam it with?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_MIX, "Mix", "mixed",
       /* ETarget */ Q_INV, "What do you want to mix?",
       /* EItem1  */ Q_INV, "What do you want to mix it with?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_MOUNT, "Mount", "mounted",
       /* ETarget */ Q_TAR|Q_NEAR|Q_CRE, "What do you want to mount?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },             
  { EV_ORDER, "Order", "ordered",
       /* ETarget */ Q_TAR|Q_CRE, "Who do you want to give orders to?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_OPEN, "Open", "open",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to open?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },      
  { EV_OPEN, "Open With", "open",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to open?",
       /* EItem1  */ Q_INV, "What do you want to open it with?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },         
  { EV_PLAY, "Play", "played",
       /* ETarget */ Q_INV, "What do you want to play?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_POUR, "Pour", "poured",
       /* ETarget */ Q_INV, "What do you want to pour it on/in?",
       /* EItem1  */ Q_INV, "What do you want to pour?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ YU_REVERSE },
  { EV_PRESS, "Press", "pressed",
       /* ETarget */ Q_TAR|Q_NEAR, "What do you want to press?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_PULL, "Pull", "pulled",
       /* ETarget */ Q_TAR|Q_NEAR, "What do you want to pull?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_PUSH, "Push", "pushed",
       /* ETarget */ Q_TAR|Q_NEAR, "What do you want to push?",
       /* EItem1  */ Q_DIR, "Push it which direction?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_QUELL, "Quell", "quelled",
       /* ETarget */ Q_TAR|Q_CRE, "Who do you want to make peace with?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_READ, "Read", "read",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to read?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_REQUEST, "Issue Request", "requested",
       /* ETarget */ Q_TAR|Q_CRE, "Who do you want to issue a request to?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },             
  { EV_RUB, "Rub", "rubbed",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to rub?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_RUB, "Rub With", "rubbed", 
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to rub?",
       /* EItem1  */ Q_INV|Q_TAR|Q_NEAR, "What do you want to rub it with?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_SMELL, "Smell", "smelled",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to smell?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_SHOW, "Show", "shown",
       /* ETarget */ Q_TAR, "Who do you want to show it to?",
       /* EItem1  */ Q_INV, "What do you want to show?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ YU_REVERSE }, 
  { EV_RATTACK, "Shoot", "shot",
       /* ETarget */ Q_TAR, "Who do you want to shoot it at?",
       /* EItem1  */ Q_INV, "What do you want to shoot it from?",
       /* EItem2  */ Q_INV, "What missile do you want to propel?",
       /* Flags   */ YU_REVERSE }, 
  { EV_SIT, "Sit", "sat in/on",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to sit in/on?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_SURRENDER, "Surrender To", "surrendered to",
       /* ETarget */ Q_TAR|Q_CRE, "Who do you want to surrender to?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_TALK, "Talk", "talked to",
       /* ETarget */ Q_TAR|Q_CRE, "What do you want to talk to?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_TASTE, "Taste", "tasted",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to taste?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_TAUNT, "Taunt", "taunted",
       /* ETarget */ Q_TAR|Q_CRE, "Who do you want to taunt?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_RATTACK, "Throw", "thrown",
       /* ETarget */ Q_TAR|Q_DIR, "What do you want to throw it at?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ Q_INV, "What do you want to throw?",
       /* Flags   */ YU_REVERSE },     
  { EV_TOUCH, "Touch", "touched",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to touch?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ YU_REVERSE },
  { EV_TIE_UP, "Tie Up", "tied up",
       /* ETarget */ Q_TAR|Q_NEAR, "What do you want to tie up?",
       /* EItem1  */ Q_INV, "What do you want to bind it with?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 }, 
  { EV_TIE_TO, "Tie To", "tied to things",
       /* ETarget */ Q_TAR|Q_INV|Q_NEAR, "What do you want to tie it to?",
       /* EItem1  */ Q_INV, "What do you want to tie?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 }, 
  { EV_TWIST, "Twist", "twisted",
       /* ETarget */ Q_INV|Q_TAR|Q_NEAR, "What do you want to twist?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_WAVE, "Wave", "waved",
       /* ETarget */ Q_INV, "What do you want to wave?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },

  { EV_WIELD, "Wield", "wielded",
       /* ETarget */ Q_TAR, "What do you want to wield?",
       /* EItem1  */ 0, NULL,
       /* EItem2  */ 0, NULL,
       /* Flags   */ 0 },
  { EV_ZAP, "Zap", "zapped",
       /* ETarget */ Q_DIR|Q_TAR, "What do you want to zap it at?",
       /* EItem1  */ Q_INV, "What do you want to zap?",
       /* EItem2  */ 0, NULL,
       /* Flags   */ YU_REVERSE },

  { 0, NULL, NULL, 0, NULL, 0, NULL, 0, NULL, 0 } };



const char* SlotNames[] = {
    "Floating    ",
    "Ready Hand  ",
    "Weapon Hand ",
    "Light Source",
    "L. Shoulder ",
    "R. Shoulder ",
    "Belt        ",
    "On Eyes     ",
    "Clothing    ",
    "Armour      ",
    "Boots       ",
    "Cloak       ",
    "Left Ring   ",
    "Right Ring  ",
    "Neck        ",
    "Gauntlets   ",
    "Helmet      ",
    "Bracers     ",
    "On Belt     ",
    "On Belt     ",
    "On Belt     ",
    "On Belt     ",
    "On Belt     ",
    "Pack        " };


const char* DwarfSurnames[] = { "Battlecrest", "Warforge", "Soulsmelt",
  "Ironarrow", "Mountainbreaker", "Hillcarver", "Oakenlord", "Bloodaxe",
  "Rockheart", "Ironmane", "Shadowkeeper", "Winterheart", "Thorncarver",
  "Fireaxe", "Stonehand", "Tunnelblazer", "Ironfist", "Stonebow", 0 };

const char* DwarfMNames[] = { "Karsht", "Ingram", "Unther", "Ocklar",
  "Abasham", "Keoshad", "Shalhide", "Imram", "Bruenor", "Thorin",
  "Tarsham", "Asklosh", "Ungult", "Rauxes", "Sahjain", "Darmin",
  "Kethram", "Fehrtan", NULL };

const char* HumanMNames[] = { "Jaoin", "Sammal", "Kierny" "Kethad",
  "Rathmere", "Michealis", "Morgon", "Matthais", "Achrim", "Orchal",
  "David", "Keos", "Quinn", "Abrash", "Jabesh", "Uchie", "Denoin",
  "Pavrash", 0 };

const char* HumanFNames[] = { "Emi", "Una", "Eonin", "Amadiel",
  "Abigail", "Cathe", "Maryam", "Immona", "Samisa", "Velanore",
  "Leanna", "Triel", "Joan", "Janina", "Cattie", "Shalamar",
  "Sangraela", "Mara", "Tara", "Minuet", "Chemise", "Madrigal",
  "Tabitha", 0 };

const char* NumberNames[] = { "zero", "one", "two", "three", "four", "five",
  "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen",
  "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen",
  "twenty", "twenty-one", "twenty-two", "twenty-three", "twenty-four",
  "twenty-five", "twenty-six", "twenty-seven", "twenty-eight", "twenty-nine",
  "thirty" };


const char* RomanNumerals[11] = { "", "I", "II", "III", "IV",
  "V", "VI", "VII", "IIX", "IX", "X" };

int32 ExperienceChart[27] = { 0, 0, 1000, 2250, 4500, 10000, 15000, 21000,
  28000, 36000, 45000, 55000, 66000, 78000, 91000, 105000, 120000, 136000,
  153000, 171000, 190000, 210000, 231000, 252000, 273000, 294000, 315000 };

TextVal ClassAbilities[] = { 
  { CA_ANCESTRAL_MEMORY, "Ancestral Memory" },
  { CA_ANIMAL_COMP, "Animal Companions" },
  { CA_ARCANE_TRICKERY, "Arcane Trickery" },
  { CA_AURA_OF_MENACE, "Aura of Menace" },
  { CA_AURA_OF_VALOUR, "Aura of Valour" },
  { CA_BARDIC_MUSIC, "Bardic Music" },
  { CA_BEGUILING_MAGIC, "Beguiling Magic" },
  { CA_BERSERK_RAGE, "Berserk Rage" },
  { CA_BONUS_SCHOOL, "Bonus School" },
  { CA_BOOST_RESISTS, "Boost Resists" },
  { CA_BOOST_SPELLS, "Boost Spells" },
  { CA_BLACK_MAGIC, "Black Magic" },
  { CA_BLINDSIGHT, "Blindsight" },
  { CA_BLOOD_MAGIC, "Blood Magic" },
  { CA_BOOK_OF_SHADOWS, "Book of Shadows" },
  { CA_CHANNELING, "Channeling" },
  { CA_COMMAND, "Command" },
  { CA_CONTACT_MIND, "Contact Mind" },
  { CA_CRAFT_PSICRYSTAL, "Craft Psicrystal" },
  { CA_DEATH_AFFINITY, "Death Affinity" },
  { CA_DEVOURING, "Devouring" },
  { CA_DOMAINS, "Domains" },
//  { CA_DISARM_MAGIC_TRAP, "Disarm Magical Traps" },
  { CA_DIVINE_GRACE, "Divine Grace" },
  { CA_DWARVEN_FOCUS, "Dwarven Focus" },
  { CA_EARTHMELD, "Earthmeld" },
  { CA_EVASION, "Evasion" },
  { CA_FAMILIAR, "Familiar" },
  { CA_FATESENSE, "Fatesense" },
  { CA_FAV_ENEMY, "Favoured Enemy" },
  { CA_FLIGHT, "Flight" },
  { CA_FEAT_OF_STRENGTH, "Feat of Strength" },
  { CA_FEED_UPON_PAIN, "Feed upon Pain" },
  { CA_GIANT_KILLER, "Giant Killer" },
  { CA_GREATER_TURNING, "Greater Turning" },
  { CA_GASEOUS_FORM, "Gaseous Form" },
  { CA_INCREASED_MOVE, "Increased Move" },
  { CA_RAPID_RIDING, "Rapid Riding" },
  { CA_INFRAVISION, "Infravision" },
  { CA_INHERANT_POTENTIAL, "Inherent Potential" },
  { CA_INNATE_SPELL, "Innate Spell" },
  { CA_INSPIRE_ALLIES, "Inspire Allies" },
  { CA_INVOKE_THE_LAND, "Invoke the Land" },
  { CA_KARMIC_IMMUNITY, "Karmic Immunity" },
  { CA_KI_STRIKE, "Ki Strike" },
  { CA_LAY_ON_HANDS, "Lay on Hands" },
  { CA_LEGEND_LORE, "Legend Lore" },
  { CA_LIFE_AFFINITY, "Life Affinity" },
  { CA_LIMITED_FREE_ACTION, "Limited Free Action" },
  { CA_LOWLIGHT, "Lowlight Vision" },
  { CA_MAGICAL_NATURE, "Magical Nature" },
  { CA_MANIFESTATION, "Manifestation" },
  { CA_MAXIMIZE_HEALING, "Maximize Healing" },
  { CA_NATURE_SENSE, "Nature Sense" },
  { CA_NOBLE_AEGIS, "Noble Aegis" },
  { CA_OPPORTUNIST, "Opportunist" },
  { CA_PASS_SLIME, "Pass Slime" },
  { CA_PHASE, "Phase" },
  { CA_PREMONITION, "Premonition" },
  { CA_PROTECTIVE_WARD, "Protective Ward" },
  { CA_PSIONICS, "Psionics" },
  { CA_PURGE_MAGIC, "Purge Magic %+d" },
  { CA_REGEN, "Regeneration" },
  { CA_RETRIBUTIVE_STRIKE, "Retributive Strike" },
  { CA_REVERIE, "Reverie" },
  { CA_SACRED_AURA, "Sacred Aura" },
  { CA_SACRED_MOUNT, "Sacred Mount" },
  { CA_SCENT, "Scent" },
  { CA_SENSE_MAGIC, "Sense Magic" },
  { CA_SHARP_SENSES, "Sharp Senses" },
  { CA_SHIELD_GROUP, "Shield Group" },
  { CA_SKILL_MASTERY, "Skill Mastery" },
  { CA_SLIPPERY_MIND, "Slippery Mind" },
  { CA_SLOW_FALL, "Slow Fall" },
  { CA_SLOW_METABOLISM, "Slow Metabolism" },
  { CA_SMITE, "Smite" },
  { CA_SNEAK_ATTACK, "Sneak Attack" },
  { CA_SOOTHING_WORD, "Soothing Word" },
  { CA_SPAWN, "Spawn" },
  { CA_SPECIALIST, "Specialist" },
  { CA_SPECIES_AFFINITY, "Species Affinity" },
  { CA_SPELLCASTING, "Spellcasting" },
  { CA_STALKER, "Stalk Player" },
  { CA_STONEWORK_SENSE, "Stonework Sense" },
  { CA_STRONG_SOUL, "Strong Soul" },
  { CA_STUN_ATTACK, "Stun Attack" },
  { CA_SYLVAN_AFFINITY, "Sylvan Affinity" },
  { CA_TELEPATHY, "Telepathy" },
  { CA_TEMPRATURE_TOL, "Temprature Tolerance" },
  { CA_TOUGH_AS_HELL, "Tough as Hell" },
  { CA_TRACKING, "Tracking" },
  { CA_TREMORSENSE, "Tremorsense" },
  { CA_TUNNEL, "Tunneling" },
  { CA_TURNING, "Turn" },
  { CA_TURN_RESISTANCE, "Turn Resistance" },
  { CA_UNARMED_STRIKE, "Unarmed Strike" },
  { CA_UNCANNY_DODGE, "Uncanny Dodge" },
  { CA_UNEARTHLY_LUCK, "Unearthly Luck" },
  { CA_VERSATILITY, "Versatility" },
  { CA_WALLWALK, "Wallwalking" },
  { CA_WEATHER_SENSE, "Weather Sense" },
  { CA_PLANT_MASTERY, "Plant Mastery" },
  { CA_WEAPON_IMMUNITY, "Weapon Immunity" },
  { CA_WEAPON_SKILL, "Weapon Skill Eligibility" },
  { CA_WHITE_MAGIC, "White Magic" },
  { CA_WHOLENESS_OF_BODY, "Wholeness of Body" },
  { CA_WILD_SHAPE, "Wild Shape" },
  { CA_WOODLAND_STRIDE, "Woodland Stride" },
  { CA_ARCANE_WILL, "Arcane Will" },
  { CA_BURNING_HUNGER, "Burning Hunger" },
  { CA_FATIGUE_REGEN, "Fatigue Regeneration" },
  { CA_AIR_MASTERY, "Air Mastery" },
  { CA_WATER_MASTERY, "Water Mastery" },
  { CA_EARTH_MASTERY, "Earth Mastery" },
  { CA_FLURRY_OF_BLOWS, "Flurry of Blows" },
  { CA_UNBIND, "Unbind" },
  { CA_COMMAND_AUTHORITY, "Command Authority" },
  { CA_PREPATORY_MAGIC, "Prepatory Magic" },
  { CA_NEGATE_CRIT, "Negate Crit" },
  { CA_FLAWLESS_DODGE, "Flawless Dodge" },
  { CA_PSYCH_MIGHT, "Psychosomatic Might" },
  { CA_SPELLBREAKER, "Spellbreaker" },
  { CA_LORE_OF_ARMS, "Lore of Arms" },
  { CA_MIGHTY_STROKE, "Mighty Stroke" },
  { CA_STORYCRAFT, "Storycraft" },
  { CA_SPELL_FURY, "Spell Fury" },
  { CA_FASTING, "Fasting" },
  { CA_SIZE_ADVANTAGE, "Size Advantage" },
  { CA_THIEF_IN_THE_NIGHT, "Thief in the Night" },
  { CA_WEAPONCRAFT, "Weaponcraft" },
  { CA_DEFENSIVE_ROLL, "Defensive Roll" },
  { CA_TRICK_MAGIC, "Trick Magic" },
  { CA_TATTOOS, "Tattoos" },
  { CA_ACROBATIC_FIGHTING, "Acrobatic Fighting" },
  { CA_MASTER_ARTISAN, "Master Artisan" },
  { CA_HIDE_IN_PLAIN_SIGHT, "Hide in Plain Sight" },
  { 0, NULL } };

int16 RogueFeats[]   = { 0, 0 };
int16 MonkFeats[]    = { 0, 0 };

int16 RangerEnemies[] = {
  MA_ABERRATION,
  MA_ANIMAL,
  MA_BEAST,
  MA_CELESTIAL,
  MA_CONSTRUCT,
  MA_DEMON,
  MA_DEVIL,
  MA_DRAGON,
  MA_ELEMENTALKIN,
  MA_FAERIE,
  MA_GOBLINOID,
  MA_GIANT,
//  MA_HUMANKIN,
  MA_LYCANTHROPE, /* Later, all changers */
  MA_OOZE,
  MA_ORC,
  MA_REPTILE,
  MA_TROLL,
  MA_UNDEAD,
  MA_VERMIN,
  0, 0 };


uint16 ActiveTraits[] = {
  SK_ALCHEMY + SKILL_VAL,
  CA_ANCESTRAL_MEMORY + ABIL_VAL,
  CA_ANIMAL_COMP + ABIL_VAL, // ww
  SK_ANIMAL_EMP + SKILL_VAL,
  CA_ARCANE_TRICKERY + ABIL_VAL,
  SK_BALANCE + SKILL_VAL,
  CA_BARDIC_MUSIC + ABIL_VAL,
  CA_BERSERK_RAGE + ABIL_VAL,
  A_BREA + ATTK_VAL,
  A_BRE2 + ATTK_VAL,
  FT_BREW_POTION + FEAT_VAL,
  FT_CIRCLE_KICK + FEAT_VAL,
  CA_COMMAND + ABIL_VAL,
  SK_CRAFT + SKILL_VAL,
  FT_DANCE_OF_TUKIMA + FEAT_VAL,
  SK_DECIPHER + SKILL_VAL,
  SK_DISGUISE + SKILL_VAL,
  CA_DWARVEN_FOCUS + ABIL_VAL,
  FT_EAGLE_CLAW_ATTACK + FEAT_VAL,
  SK_ESCAPE_ART + SKILL_VAL,
  CA_FEAT_OF_STRENGTH + ABIL_VAL,
  CA_FLAWLESS_DODGE + ABIL_VAL,
  CA_FLURRY_OF_BLOWS + ABIL_VAL,
  A_GEST + ATTK_VAL,
  FT_GREATER_SOULBLADE + FEAT_VAL,
  CA_GREATER_TURNING + ABIL_VAL,
  SK_HANDLE_DEV + SKILL_VAL,
  SK_HEALING + SKILL_VAL,
  SK_HIDE + SKILL_VAL,
  CA_INNATE_SPELL + ABIL_VAL,
  CA_INSPIRE_ALLIES + ABIL_VAL,
  CA_INVOKE_THE_LAND + ABIL_VAL,
  SK_JUMP + SKILL_VAL,
  SK_KNOW_MAGIC + SKILL_VAL,
  CA_LAY_ON_HANDS + ABIL_VAL,
  SK_LISTEN + SKILL_VAL,
  CA_MANIFESTATION + ABIL_VAL,
  SK_MINING + SKILL_VAL,
  CA_PHASE + ABIL_VAL,
  SK_PICK_POCKET + SKILL_VAL,
  SK_POISON_USE + SKILL_VAL,
  CA_PROTECTIVE_WARD + ABIL_VAL,
  CA_SACRED_MOUNT + ABIL_VAL,
  FT_SCRIBE_SCROLL + FEAT_VAL,
  CA_STORYCRAFT + ABIL_VAL,
  A_ROAR + ATTK_VAL,
  SK_APPRAISE + SKILL_VAL,
  FT_SHIELD_BASH + FEAT_VAL,
  CA_SOOTHING_WORD + ABIL_VAL,
  CA_SPECIES_AFFINITY + ABIL_VAL,
  A_SPIT + ATTK_VAL,
  FT_SPRING_ATTACK + FEAT_VAL,
  FT_SUNDER + FEAT_VAL,
  SK_TUMBLE + SKILL_VAL,
  FT_WHIRLWIND_ATTACK + FEAT_VAL,
  CA_TRACKING + ABIL_VAL,
  CA_TURNING + ABIL_VAL,
  CA_UNBIND + ABIL_VAL,
  FT_WARCRY + FEAT_VAL,
  CA_WEAPONCRAFT + ABIL_VAL,
  CA_WHOLENESS_OF_BODY + ABIL_VAL,
  CA_WILD_SHAPE + ABIL_VAL,
  0 };

SkillInfoStruct SkillInfo[] = {
  /* SK_WHAT, Name, implemented, active, 
     desc, attr1, attr2, take-min-of-attrs, armour-penalty-factor */
  { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 

  { SK_ALCHEMY, "Alchemy", true, true, 
     "__This skill allows you to identify potions. It can also be used to mix "
     "potions safely, creating entirely new potions -- or blowing yourself to "
     "hell. Be careful!",
     A_INT, A_INT, false, 0 }, 

  { SK_ANIMAL_EMP, "Animal Empathy", true, true, 
     "__This skill allows you to communicate with animals and magical beasts. "
     "You can pacify angry animals, making them nonhostile. You can cow angry "
     "animals, making them afraid of you. You can soothe allied animals, "
     "removing sleep and fear. Finally, you can befriend nonhostile animals, "
     "turning them into temporary party members. The DC for an Animal Empty "
     "skill check is 15 + the challenge rating of the animal.",
     A_INT, A_WIS, false, 0 }, 

  { SK_APPRAISE, "Appraise", true, true, 
     "__This skill gives you a chance to determine a creature's alignment and "
     "personality archetype, and certain other intentions in a situation. It "
     "also gives you heightened skill when bargaining with the shopkeepers, "
     "and it lets you know the rough chance that any ally -- whether a party "
     "member, animal companion or summoned/charmed creature -- is likely to "
     "betray you or break control, and what can be used to ameliorate that "
     "coming betrayal.\n __Your protagonist's Sense Motive also allows him "
     "to lessen the tension caused by extreme differences in party "
     "personality archetypes, since it represents the ability to achieve "
     "rapport between people. Finally, it can be used to tell you things "
     "about an NPC's nature or reveal general plot information.",
     A_WIS, A_WIS, false, 0}, 

  { SK_ATHLETICS, "Athletics", true, false, 
     "__This skill increases your base movement rate by 5~ for every two "
     "skill levels you possess.", 
     A_STR, A_CON, false, 0 },
     /* lessens the fatigue you gain from strenuous activities, shortens the time you are stunned by powerful blows, and allows you to move more rapidly on the wilderness map."  */

  { SK_BALANCE, "Balance", true, true, 
     "__This skill decreases the chance of you being knocked prone by an "
     "attack or spell effect. Balance figures into the chance of you "
     "becoming prone when moving or fighting over a slippery surface such "
     "as ice or the effects of a <13>grease<7> spell.",
     A_DEX, A_DEX, false, 1 },
/*      It also improves your ability to walk straight when <15>Confused<7>, and allows you to walk on the roofs of buildings in the town or crawl over piles of rubble safely.  */

  { SK_BLUFF, "Bluff", true, true, 
     "__This skill increases your chance of getting a better deal in stores. "
     "It can also be used to gain admittance to some guilds and other places "
     "where you aren't supposed to be, on sheer force of personality alone. "
     "Finally, it helps a lot if you are caught stealing or otherwise offend "
     "the authorities.",
     A_CHA, A_CHA, false, 0 },

  { SK_CLIMB, "Climb", true, true, 
     "__The Climb skill has several uses. In a dungeon, it can allow you to "
     "safely descend (DC 10 + Depth) or ascend (DC 20 + depth) chasms. You "
     "can also climb up into a tree in a forested area (or a giant mushroom "
     "in a dungeon cave). Characters climbing in trees add [2 + (Climb Skill "
     "Rating / 3)] to their Defense Class, attack rolls and Hide skill rating "
     "while doing so, and attacks against them have a 25~ miss chance due to "
     "cover. Moving from tree to tree requires a Climb check against DC 15, "
     "with a result of 25+ allowing movement at full normal speed. To climb "
     "up or down trees or chasms, use the Ascend and Descend commands when "
     "standing under a tree or beside a chasm.",
     A_STR, A_DEX, false, 1 }, 
     /* "\n __Varying degrees of this skill also allow you to traverse varying extremes of mountains on the wilderness map as if they were normally crossable terrain. It can also be used to get out of pit traps quickly, to make use of broken stairs in lieu of normal ones, to move safely over rubble in the dungeon and to get on top of buildings in the town.\n\n */

  { SK_CONCENT, "Concentration", true, false, 
     "__This skill decreases the chance of your spells or psionic powers being "
     "disrupted if you suffer damage during the casting or activation time. In "
     "addition, each rank in Concentration decreases your normal, "
     "non-interrupted chance of spell failure by 5%.",
     A_WIS, A_CON, true, 0 } , 

  { SK_CRAFT, "Craft", true, true, 
     "__This skill allows you to craft items of various types, most notably "
     "weapons and armour. Any item you craft yourself is much easier to "
     "enchant, particularly if great degrees of skill are involved in the "
     "creation. You need to go to the appropriate location in town (such as "
     "the Smithy) to craft items.",
     A_WIS, A_DEX, true, 0 }, 

  { SK_DECIPHER, "Decipher Script", true, false ,
     "__This skill is used to read scrolls, and other ancient and/or magical "
     "writings. It also gives you a chance to identify any scroll that you "
     "pick up. At very high levels, it can increase the effective plus of stat "
     "gain tomes.\n __<15>Note:<7> Anyone can read a scroll for a spell that "
     "is on their class's spell list at their level. This skill allows its "
     "user to read any scroll, even one for a class the user has no levels "
     "in, and decreases the failure chance for a spellcaster reading a scroll "
     "above their level.", 
     A_INT, A_INT, true, 0 }, 

  { SK_DIPLOMACY, "Diplomacy", true, false, 
     "__This skill allows you to enter and advance within the Nobles' guild, "
     "to get better prices in stores and influences the chance of your meeting "
     "non-hostile monsters. It also determines the chance that a monster will "
     "accept your surrender or parlay.",
     A_CHA, A_CHA, false, 0 }, 

  { SK_DISGUISE, "Disguise", true, true,
     "__ Blah blah blah.",
     A_INT, A_CHA, false, 0 },

  { SK_ESCAPE_ART, "Escape Artist", true, false, 
     "__This skill allows you to escape from being stuck in webbings or "
     "entangling weapons. You Escape Artist skill level is added directly to "
     "your grapple check to escape from a grapple or grab.", 
     A_DEX, A_DEX, false, 1 }, 
     /* from monsters or characters who grapple, grab, ensnare or hold you, and gives you increased resistance to spells like <13>web<7> and <13>sink<7>. At high levels, it might grant you some defense even against spells that limit your movement (<13>hold person<7>, etc). Escape Artist also allows you a chance to get away if you are captured by the  authorities in town and put in the gaol.\n\n */

  { SK_FIND_WEAKNESS, "Find Weakness", true, false,
      "__Wes has no idea what Julian wants this skill to be doing.",
      A_WIS, A_WIS, false, 0 }, 

  { 0, "Gather Information", false, true, 
     //"__This skill determines your ability to eat corpses without contracting diseases. Whenever you eat a corpse you make a Food Tending skill check to avoid ill effects. The DC for this check is shown when you examine the corpse and is roughtly equal to 15 + one-half the number of steps you have taken since the corpse was created: corpses putrify *very* quickly.", 
     "Blah blah blah.",
     A_WIS, A_WIS, false, 0 }, 

  { SK_HANDLE_DEV, "Handle Device", true, true, 
     "__This skill can be used to disarm mundane or magical traps in the dungeon.", 
     /* If you have access to <13>dispel magic<7>, it can even be used to disarm magical traps. At high levels of expertise, you can collect the pieces afterward and build your own traps, using them against any monsters you want.", */
     A_DEX, A_INT, true, 0 }, 

  { SK_HEALING, "Healing", true, true, 
     "__Using this skill, you have a chance to remove maladies such as "
     "poisoning, bleeding, blindness, injured limbs and such. However, "
     "you only get one go per malady! Heal also increases the number of "
     "hit points that you regain when you rest in the dungeon, but not "
     "by a great amount compared to healing spells.", 
     A_WIS, A_WIS, false, 0 }, 

  { SK_HIDE, "Hide in Shadows", true, true, 
     "",
     A_DEX, A_DEX, false, 1 }, 

  { SK_ILLUSION, "Illusioncraft", true, false,
     "__Creating creatures and images with magic is not truly that "
     "different from sketching or sculpture; it just works in an "
     "alternative medium. This skill gives an illusionist skill in "
     "that medium, imbueing his illusions with a greater realism, "
     "detail and believability. As such, it adds [Skill Rating / 3] "
     "to the DC of any saving throw to disbelieve an illusion that "
     "he creates. This applies only to true illusion-creation spells "
     "like <9>Phantasmal Force<7>, not to all spells from the Illusion "
     "school (<9>Chromatic Orb<7>, <9>Phantasmal Killer<7>, etc.)",
     A_INT, A_INT, false, 0 }, 

  { SK_INTIMIDATE, "Intimidate", true, false, 
     "__Using this skill, you have a chance of forcing intelligent monsters "
     "to accept your surrender. If you have Animal Empathy, you can attempt "
     "the same with most living creatures.",
     A_STR, A_CHA, false, 0}, 

  { SK_INTUITION, "Intuition", true, false, 
     " __This skill allows you to gain special feelings about items picked up "
     "in the dungeon, telling you if they are good, bad or neutral.  "
     "Obviously, this is a valuable skill to single-classed Rogues and "
     "Warriors who lack divination magics or Clairsentient arts.", 
     A_WIS, A_WIS, false, 0}, 
     /* "At higher levels, it also gives you feelings about a dungeon level when you first visit it (and the game will repeat the feeling message whenever you return). These hunches can tell you when a level has a valuable magic item, a room holding many monsters, or even a legendary artifact!" */

  { SK_JUMP, "Jump", true, true, 
    "__Use this skill to cross from one square to another in the dungeon while "
    "missing a square in between. This skill will let you get past a monster "
    "without bull-rushing it, or to avoid a trap or pit. Beware, though -- if "
    "you try to jump too far, you may not end up where you want! A typical "
    "jumping DC is 10 for 1 square, +7 per extra square!", 
  A_DEX, A_STR, false, 1 }, 

  { SK_KNOW_GEO, "Knowledge (Theyra)", false, false, 
    "__This skill will give a character limited memory of the wilderness"
    " map, and is used against checks in various DCs determined by terrain"
    " to avoid becoming lost.", A_INT, A_INT, false, 0 }, 

  { SK_KNOW_INF, "Knowledge (Infernal)", true, false,
    "__Many priests and paladins study demons and devils to learn how "
    "to better fight them, while dark sorcerors want to understand "
    "exactly what they are summoning. In addition to giving you map "
    "knowledge of the Lower Planes the same way Knowledge (Theyra) "
    "does for the normal overland map, Knowledge (Infernal) "
    "grants certain bonuses when fighting demons and devils. "
    "By applying simple hearth "
    "wisdom and studying the vulnerabilities of his targets, this skill "
    "allows a character the following bonuses when facing demons and "
    "devils: his effective DC is increased by [Skill Rating / 3] "
    "against their attacks, and his understanding of their banes and "
    "biology increases the threat ratings of his attacks against them "
    "by [Skill Rating / 9]. He can also summon them more effectively; "
    "demons and devils summoned by such a caster have a [Skill Rating x 5]% "
    "bonus to their hit points.",
     A_INT, A_INT, false, 0 }, 

  { SK_KNOW_MAGIC, "Knowledge (Magic)", true, false,
    "__This skill gives a character inherent knowledge of a number of spells,"
    " thereby allowing him to cast them without a spellbook. The character has"
    " to spend spell slots to learn these spells normally, of course.",
     A_INT, A_INT, false, 0 }, 

  { SK_KNOW_MYTH, "Knowledge (Mythology)", true, false,
    "__A solid understanding of mythology sets up an adventurer to fight "
    "more effectively against mythic creatures. By applying simple hearth "
    "wisdom and studying the vulnerabilities of his targets, this skill "
    "allows a character the following bonuses when facing creatures of "
    "mythology: his effective DC is increased by [Skill Rating / 3] "
    "against their attacks, and his understanding of their banes and "
    "biology increases the threat ratings of his attacks against them "
    "by [Skill Rating / 9]. He can also summon them more effectively; "
    "mythic creatures summoned by such a caster have a [Skill Rating x 5]% "
    "bonus to their hit points.",
     A_INT, A_INT, false, 0 }, 
     
  { SK_KNOW_NATURE, "Knowledge (Nature)", true, false,
    "__This skill gives a character knowledge of flora and fauna; with it he"
    " can identify and cultivate herbs and mushrooms. Additionally, his "
    "detailed knowledge and understanding of animals gives him a certain "
    "edge when fighting them, working as per similar Knowledge skills "
    "in that regard.",
     A_INT, A_INT, false, 0 },  

  { SK_KNOW_OCEANS, "Knowledge (Oceans)", true, false,
    "__Some adventurers spend much of their lives out at sea, exploring "
    "the great Western Ocean or scouring trade routes for pirates and "
    "other threats. This knowledge skill gives map-memory of oceanic "
    "areas in a manner similar to Knowledge (Theyra), and also aids "
    "an adventurer in fighting aquatic threats. By using a thourogh "
    "knowledge of their behaviour, biology and weaknesses, this skill "
    "allows a character the following bonuses when fighting aquatic or "
    "amphibious creatures: his effective DC is increased by [Skill Rating / 3] "
    "against their attacks, and his understanding of their banes and "
    "biology increases the threat ratings of his attacks against them "
    "by [Skill Rating / 9]. He can also summon them more effectively; "
    "aquatic creatures summoned by such a caster have a [Skill Rating x 5]% "
    "bonus to their hit points.",
      A_INT, A_INT, false, 0 }, 

  { SK_KNOW_PLANES, "Knowledge (Planes)", true, false,
    "__This skill gives a character knowledge of the many planes of "
    "existence, and is necessary to cast plane-travelling spells like "
    "<9>Plane Shift<7> safely. Furthermore, it can aid the character "
    "in fighting outsiders who are not also demons, devils or celestials. "
    "By using a thorough "
    "knowledge of their behaviour, biology and weaknesses, this skill "
    "allows a character the following bonuses when fighting "
    "such outsiders: his effective DC is increased by [Skill Rating / 3] "
    "against their attacks, and his understanding of their banes and "
    "biology increases the threat ratings of his attacks against them "
    "by [Skill Rating / 9]. He can also summon them more effectively; "
    "general outsiders summoned by such a caster have a [Skill Rating x 5]% "
    "bonus to their hit points.",
     A_INT, A_INT, false, 0 }, 

  { SK_KNOW_THEO, "Knowledge (Theology)", true, false, 
    "__This skill represents a knowledge of the Theryan pantheon and your "
    "deity in particular. If you have more than 5 total levels in this skill "
    "you gain a +2 bonus to your Turn, Command or Rebuke checks.",
    A_INT, A_WIS, false, 0}, 

  { SK_KNOW_UNDEAD, "Knowledge (Undead)", true, false,
    "__The dead hate the living, and the living often return the "
    "sentiment. A character with this skill is a trained undead "
    "slayer or master necromancer, well versed in the inherent "
    "abilities and weaknesses of the Damned. By using a thorough "
    "knowledge of their behaviour, weaknesses and unnatural existence, this skill "
    "allows a character the following bonuses when fighting aquatic or "
    "the undead: his effective DC is increased by [Skill Rating / 3] "
    "against their attacks, and his understanding of their banes and "
    "biology increases the threat ratings of his attacks against them "
    "by [Skill Rating / 9]. He can also summon them more effectively; "
    "aquatic creatures summoned by such a caster have a [Skill Rating x 5]% "
    "bonus to their hit points. Note that undead are not normally "
    "subject to critical hits, but with this skill they can be struck "
    "for such with the threat range it provides alone.",
     A_INT, A_INT, false, 0 }, 


  { SK_LISTEN, "Listen", true, false, 
    "You Listen skill is used against an enemy's Move Silently skill to "
    "determine if you are surprised by an unseen attack. If you happen to "
    "possess Blindsight, it is also used to detect nearby hiding enemies. "
    "In addition, you may actively use the Listen skill to determine the "
    "general types (e.g., animal, earth creature) of nearby monsters and "
    "determine if it is safe to rest. With a high degree of skill you can "
    "determine the names (e.g., eagle) of nearby monsters.", 
    A_WIS, A_WIS, false, 0 }, 
    /* __This skill gives you some idea what kind of monsters are
     nearby when you can't see them for whatever reason, such as
     being near a closed door. With a high degree of skill, Listen
     will also reveal their direction. Listen decreases the penal-
     ties you receive for blindness, and works well in conjunct
     with the Blindfight feat. It is also used to contest Move
     Silently.\n\n */

  { SK_LOCKPICKING, "Lockpicking", true, false, 
     "__This skill gives you the opportunity to pick the lock on locked doors, "
     "thereby avoiding the noise (which attracts monsters) and potential damage "
     "involved in breaking one down. There may even be doors that can be picked "
     "but not bashed down... chests can also have locks that need picking, "
     "which makes the skill valuable, since chests often contain items too "
     "fragile to survive brute force!",
     A_DEX, A_DEX, false, 0 }, 

  { SK_METAMAGIC, "Metamagic", true, false, 
     "__Metamagic allows a spellcaster to modify the effects of a spell in "
     "various ways. Unfortunately, Metamagic is taxing and fatiguing. For "
     "every 5 points by which you succeed a Metamagic skill check against "
     "DC 15, the fatigue cost for such a spell is reduced by 1.", 
     A_INT, A_INT, false, 0}, 

  { SK_MINING, "Mining", true, false, 
     "__This skill increases the effectiveness and decreases the time required "
     "when trying to tunnel through walls in the dungeon. It also lets you "
     "appraise veins of minerals in the dungeon, identify gems and, at higher "
     "levels, it lets you tunnel through rock types you otherwise couldn't.", 
     A_WIS, A_STR, true, 0}, 

  { SK_MOVE_SILENTLY, "Move Silently", true, false, 
     "__This skill allows you to move past monsters in the dungeon without "
     "waking them up. It is contested against the defender's Listen skill "
     "to see if you can make certain surprise attacks (e.g., when you are "
     "hidden). Every two skill levels of Move Silently you possess reduce "
     "the noise radius of attacks you make by 10'.", 
     /* "It also augments your ability not to be seen when using Hide, and decreases the chance of encounters in the wilderness.", */
     A_DEX, A_DEX, false, 1},

  { SK_PERFORM, "Perform", true, false, 
    "__Every rank you have in this skill increases the radius of your Bardic "
    "Music effects by 10'. It also determines the DCs for your Bardic Music "
    "effects.", 
    A_CHA, A_CHA, false, 0}, 

  { SK_PICK_POCKET, "Pick Pockets", true, true, 
     "__This skill allows you to remove items from a monster, hostile or not, "
     "and take them for yourself. If you attempt this on a non-hostile and "
     "fail, the monster will become hostile; in a city, the authorities will "
     "also take notice!  Pick Pockets also gives you protection against "
     "monsters stealing your own possessions!", 
     A_DEX, A_CHA, false, 1}, 

  { SK_POISON_USE, "Poison Use", true, false, 
    "__This skill allows you to Apply poison to weapons and missiles without "
    "poisoning yourself. A typical DC for such an action is 10, and poisons "
    "that can be applied in such a manner are often stored in small glass "
    "vials. Every two levels of this skill increase the save DC of the poison "
    "on such a poisoned weapon by 1.", 
    A_DEX, A_INT, true, 0}, 

  { SK_RIDE, "Ride", true, false, 
    "__This skill determines the combat bonuses to attack, damage and AC you "
    "receive from being mounted, as well as determining how effectively you "
    "can control your mount, and at high levels if you can use an exotic "
    "beast as a mount.", 
  A_DEX, A_WIS, true, 0}, 

  { SK_SEARCHING, "Searching", true, true, 
    "__This skill allows you to spot secret doors and concealed traps. You get a free "
    "chance to spot a concealed trap when you move over it, and characters with preternaturally "
    "sharp senses or stonework abilities (e.g., Elves, Dwarves) get a free chance to spot secret "
    "doors when adjacent to them.  You may also actively look for hidden features in your "
    "environment. If you fail, you can't try to spot that hidden feature until you rest.", 
     A_INT, A_INT, true, 0}, 

  { SK_SENESCHAL, "Seneschal", false, false, 
    "__Wes has no idea what Julian wants this skill to be.",
  A_INT, A_INT, false, 0}, 

  { SK_SPELLCRAFT, "Spellcraft", true, false, 
    "__This skill allows you to determine which spell is being cast by an "
    "enemy spellcaster you can perceive. A typical DC for this skill is 15 + "
    "the level of the spell being cast.", 
    A_INT, A_INT, false, 0}, 

  { SK_SPOT, "Spot", true, false, 
    "__This skill is contested against the Hide skill of another creature and "
    "allows you to see such hiding creatures.", 
    /* This skill increases your ability to awaken quickly if you have an encounter while resting in the dungeon. It also gives you the opportunity to begin wilderness encounters farther away from the monsters in question.  Finally, Alertness gives you a greater chance of hitting invisible monsters and spot those that are hiding.\n\n */
     A_WIS, A_WIS, false, 0}, 

  { SK_SWIMMING, "Swimming", true, false, 
    "__This skill allows you to traverse deep water on the map (rivers, not "
    "the ocean) as if it were any other harsh terrain. In deep water, a "
    "skill check is required to make forward progress or attack at all. "
    "Failing a swim check by 5 or more causes you to start to drown and take "
    "damage.", 
     A_DEX, A_STR, true, 2}, 

  { SK_TUMBLE, "Tumble", true, true, 
    "__Using this skill allows you to move acrobatically; tumbling uses two "
    "fatigue points and lasts for only a few turns, but gives several "
    "advantages. While tumbling, a character adds a third of their Tumble "
    "skill rating to their DC versus normal attacks, and their full Tumble "
    "skill rating to their DC versus attacks of opportunity provoked by "
    "movement. Tumbling increases one's movement rate based on skill rating, "
    "and decreases attack Speed and spell success by a set 25%.",
     /* "\n __Finally, when tumbling a character may avoid provoking attacks of opportunity when jumping by making a Tumble skill check against a DC of 20.",  */
     A_DEX, A_DEX, false, 1}, 

  { SK_USE_MAGIC, "Use Magic", true, false, 
     "", 
     A_CHA, A_INT, false, 0}, 

  { SK_WILD_LORE, "Wilderness Lore", true, false, 
     "__This skill decreases the time that travel takes in the wild, and "
     "increases the amount of experience that wilderness encounters generate. "
     "It also gives you a chance to identify mushrooms and herbs, and "
     "increases your success ratio while decreasing time spent for hunting.", 
     A_WIS, A_WIS, false, 0}, 
  { 0, NULL, false, false, NULL, 0, 0, false, 0 }
} ;

AlignmentInfoStruct AlignmentInfo[] = {
      { "Lawful Good",
        "<2>Lawful Good, 'Crusader':<7>\n__ A lawful good character acts as a good person "
        "is expected or required to act. She combines a commitment to oppose evil with the "
        "discipline to fight relentlessly. She tells the truth, keeps her word, helps those "
        "in need, and speaks out against injustice. A lawful good character hates to see "
        "the guilty go unpunished.\n"
        "__ Lawful good is the best alignment you can be because it combines honour and "
        "compassion.",
        AL_LAWFUL|AL_GOOD },

      { "Neutral Good",
        "<2>Neutral Good, 'Benefactor':<7> A neutral good character does the best that a "
        "good person can do. He is devoted to helping others. He works with kings and "
        "magistrates but does not feel beholden to them.\n"
        "__ Neutral good is the best alignment you can be because it means doing what is "
        "good without bias for or against order.",
        AL_GOOD },

      { "Chaotic Good",
        "<2>Chaotic Good, 'Rebel':<7> A chaotic good character acts as his conscience "
        "directs him with little regard for what others expect of him. He makes his own "
        "way, but he's kind and benevolent. He believes in goodness and right but has "
        "little use for laws and regulations. He hates it when people try to intimidate "
        "others and tell them what to do. He follows his own moral compass, which, although "
        "good, may not agree with that of society.\n"
        "__ Chaotic good is the best alignment you can be because it combines a good heart "
        "with a free spirit.",
        AL_CHAOTIC | AL_GOOD },

      { "Lawful Neutral",
        "<2>Lawful Neutral, 'Judge':<7> A lawful neutral character acts as law, tradition, "
        "or a personal code directs her. Order and organization are paramount to her. She "
        "may believe in personal order and live by a code or standard, or she may believe "
        "in order for all and favour a strong, organized government.\n"
        "__ Lawful neutral is the best alignment you can be because it means you are "
        "reliable and honourable without being a zealot.",
        AL_LAWFUL },

      { "Neutral",
        "<2>Neutral, 'Undecided':<7> A neutral character does what seems to be a good "
        "idea. She doesn't feel strongly one way or the other when it comes to good vs. "
        "evil or law vs. chaos. Most neutral characters exhibit a lack of conviction or "
        "bias rather than a commitment to neutrality. Such a character thinks of good "
        "as better than evil -- after all, she would rather have good neighbours and "
        "rulers than evil ones. Still, she's not personally committed to upholding good "
        "in any abstract or universal way.\n"
        "__ Some neutral characters, on the other hand, commit themselves philosophically "
        "to neutrality. They see good, evil, law, and chaos as prejudices and dangerous "
        "extremes. They advocate the middle way of neutrality as the best, most balanced "
        "road in the long run.\n"
        "__ Neutral is the best alignment you can be because it means you act naturally, "
        "without prejudice or compulsion.",
        0 },

      { "Chaotic Neutral",
        "<2>Chaotic Neutral, 'Free Spirit':<7> A chaotic neutral character follows his "
        "whims. He is an individualist first and last. He values his own liberty but "
        "doesn't strive to protect others' freedom. He avoids authority, resents "
        "restrictions, and challenges traditions. A chaotic neutral character does not "
        "intentionally disrupt organizations as part of a campaign of anarchy. To do so, "
        "he would have to be motivated either by good (and a desire to liberate others) "
        "or evil (and a desire to make those different from himself suffer). A chaotic "
        "neutral character may be unpredictable, but his behaviour is not totally random. "
        "He is not as likely to jump off a bridge as to cross it.\n"
        "__ Chaotic neutral is the best alignment you can be because it represents true "
        "freedom from both society's restrictions and a do-gooder's zeal.",
        AL_CHAOTIC
      } ,

      { "Lawful Evil",
        "<2>Lawful Evil, 'Dominator':<7> A lawful evil villain methodically takes what he "
        "wants within the limits of his code of conduct without regard for whom it hurts. "
        "He cares about tradition, loyalty, and order but not about freedom, dignity, or "
        "life. He plays by the rules but without mercy or compassion. He is comfortable "
        "in a hierarchy and would like to rule, but is willing to serve. He condemns others "
        "not according to their actions but according to race, religion, homeland, or social "
        "rank. He is loath to break laws or promises.\n"
        "__This reluctance comes partly from his nature and partly because he depends on "
        "order to protect himself from those who oppose him on moral grounds. Some lawful "
        "evil villains have particular taboos, such as not killing in cold blood (but "
        "having underlings do it) or not letting children come to harm (if it can be "
        "helped). They imagine that these compunctions put them above unprincipled "
        "villains.\n"
        "__ Some lawful evil people and creatures commit themselves to evil with a zeal "
        "like that of a crusader committed to good. Beyond being willing to hurt others "
        "for their own ends, they take pleasure in spreading evil as an end unto itself. "
        "They may also see doing evil as part of a duty to an evil deity or master.\n"
        "__ Lawful evil is sometimes called 'diabolical' because devils are the epitome "
        "of lawful evil.\n"
        "__ Lawful evil is the most dangerous alignment because it represents methodical, "
        "intentional, and frequently successful evil.",
        AL_LAWFUL|AL_EVIL
      },
      { "Neutral Evil",
        "<2>Neutral Evil, 'Malefactor':<7> A neutral evil villain does whatever she can "
        "get away with. She is out for herself, pure and simple. She sheds no tears for "
        "those she kills, whether for profit, sport, or convenience. She has no love of "
        "order and holds no illusion that following laws, traditions, or codes would make "
        "her any better or more noble. On the other hand, she doesn't have the restless "
        "nature or love of conflict that a chaotic evil villain has.\n"
        "__ Some neutral evil villains hold up evil as an ideal, committing evil for its "
        "own sake. Most often, such villains are devoted to evil deities or secret "
        "societies.\n"
        "__ Neutral evil is the most dangerous alignment because it represents pure evil "
        "without honour and without variation.",
        AL_EVIL
      },
      { "Chaotic Evil",
        "<2>Chaotic Evil, 'Destroyer': <7>A chaotic evil character does whatever his "
        "greed, hatred, and lust for destruction drive him to do. He is hot-tempered, "
        "vicious, arbitrarily violent, and unpredictable. If he is simply out for "
        "whatever he can get, he is ruthless and brutal. If he is committed to the "
        "spread of evil and chaos, he is even worse. Thankfully, his plans are "
        "haphazard, and any groups he joins or forms are poorly organized. Typically, "
        "chaotic evil people can be made to work together only by force, and their "
        "leader lasts only as long as he can thwart attempts to topple or assassinate "
        "him.\n"
        "__ Chaotic evil is sometimes called 'demonic' because demons are the epitome "
        "of chaotic evil.\n__ Chaotic evil is the most dangerous alignment because it "
        "represents the destruction not only of beauty and life but also of the order "
        "on which beauty and life depend.",
        AL_CHAOTIC|AL_EVIL
      }
    } ;
    


const char* PersonalityNames[] = {
  "The Ambitious Conqueror",
  "The Charming Scoundrel",
  "The Crazed Psychopath",
  "The Disciplined Expert",
  "The Driven Brooder",
  "The Embittered Cynic",
  "The Empathic Nurturer",
  "The Eternal Romantic",
  "The Gentleman Adventurer",
  "The Honourbound Servant",
  "The Iconoclastic Rebel",
  "The Introspective Visionary",
  "The Manipulative Plotter",
  "The Righteous Zealot",
  "The White Knight",
  "The Witty Thrillseeker",
  NULL };

int16 QualityMods[][2] = {

  { WQ_BURST, 1 },
  { WQ_CORROSIVE, 1 },
  { WQ_FLAMING, 1 },
  { WQ_FROST, 1 },
  { WQ_SHOCKING, 1 },
  { WQ_THUNDERING, 1 },
  { WQ_SOULDRINK, 1 },          
  { WQ_WOUNDING, 1 },
  { WQ_GLOWING, 1 },

  { WQ_BALANCE, 2 },
  { WQ_CHAOTIC, 2 },
  { WQ_HOLY, 2 },        
  { WQ_LAWFUL, 2 },
  { WQ_UNHOLY, 2 },

  { WQ_WEAKENING, 1 },
  { WQ_WITHERING, 1 },
  { WQ_NUMBING, 1 },

  { WQ_ACCURACY, 1 },
  { WQ_BANE, 1 },
  { WQ_SEEKING, 1 },
  { WQ_VICIOUS, 1 },
  { WQ_CLEAVING, 1 },
  { WQ_DISTANCE, 1 },
  { IQ_FEATHERLIGHT, 1 },
  { IQ_GHOST_TOUCH, 1 },
  { WQ_RETURNING, 1 },
  { WQ_SMITING, 1 },
  { WQ_SUNDERING, 1 },
  { WQ_KEEN, 1 },               // ww: was 2
  { WQ_SOULDRINK, 1 },          // ww: was 2
  { WQ_STUNNING, 1 },           // ww: was 2
  { WQ_TERROR, 1 },             // ww: was 2
  { WQ_WARNING, 1 },
  { WQ_FEY_TRAPPING, 1 },
  { WQ_DEFENDING, 1 },          // ww: was 1
  { WQ_DISRUPTION, 2 },         // ww: was 3
  { IQ_PROFICIENT, 2 }, 
  { WQ_NIGHT, 2 },
  { WQ_VENOM, 1 },
  { WQ_MIGHTY, 2 },
  { WQ_QUICK_LOADING, 2}, 
  { WQ_WOUNDING, 2 },
  { WQ_THIRSTY, 2 },
  { WQ_ENERGY, 3 },
  { WQ_SLAYING, 3 },            // ww: was 4
  { WQ_SPEED, 3 },
  { WQ_SLEEP, 3 },       
  { WQ_VAMPIRIC, 3 },
  { WQ_IMPACT, 4 },
  { WQ_VORPAL, 5 },

  /* ww: unimp, currently
  { WQ_DANCING, 3 }, 
  { WQ_ENTROPIC, 2 },
  */
  { -1, -1 } };

int16 AQualityMods[40][2] = {

  { AQ_ACID_RES, 1 },           // ww: was 2
  { AQ_AGILITY, 1 },            // ww: was 2
  { AQ_ARROW_DEF, 1 },
  { AQ_BANEBLIND, 1 },          // ww: was 3
  { AQ_BASHING, 1 },
  { AQ_COLD_RES, 1 },           // ww: was 2
  { AQ_ENDURANCE, 1 },          // ww: was 2
  { IQ_FEATHERLIGHT, 1 },
  { IQ_GHOST_TOUCH, 1 },
  { AQ_GRAVEN, 1 },
  { IQ_PROFICIENT, 1 }, 
  { AQ_FIRE_RES, 1 },           // ww: was 2
  { AQ_GRACEFUL, 1 },           // ww: was 2
  { AQ_LIGHT_FOR, 1 },
  { AQ_LIGHT_RES, 1 },          // ww: was 2
  { AQ_MIGHT, 1 },              // ww: was 2
  { AQ_RADIANT, 1 },
  { AQ_REFLECTION, 1 },
  { AQ_SACRED, 1 },
  { AQ_SHADOW, 1 },             // ww: was 2
  { AQ_SHIFTING, 1 },
  { AQ_SILENT, 1 },             // ww: was 2
  { AQ_SLICK, 1 },              // ww: was 2
  { AQ_SONIC_RES, 1 },          // ww: was 3
  { AQ_STABILITY, 1 },
  { AQ_AUSPICIOUS, 2 },         // ww: was 4
  { AQ_INVULN, 2 },       
  { AQ_COMMAND, 2 },            // ww: was 4
  { AQ_BLINDING, 2 },           // ww: was 3
  { AQ_FEARSOME, 2 },           // ww: was 4
  { AQ_MEDIUM_FOR, 2 },
  { AQ_RESISTANT, 2 },
  { AQ_ANIMATED, 3 },
  { AQ_GREAT_INV, 3 },         
  { AQ_LIFEKEEPING, 3 },
  { AQ_HEAVY_FOR, 3 },
  { AQ_MAGIC_RES, 3 },
  { AQ_HEALING, 4 },
  { AQ_ETHEREALNESS, 5 },
  
  { -1, -1 } };

int8 PythDist[64 * 64] = {
 0 ,  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9 ,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 , 
 1 ,  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9 ,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 , 
 2 ,  2 ,  3 ,  4 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9 ,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 , 
 3 ,  3 ,  4 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9 ,  9 ,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 , 
 4 ,  4 ,  4 ,  5 ,  6 ,  6 ,  7 ,  8 ,  9 ,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 , 
 5 ,  5 ,  5 ,  6 ,  6 ,  7 ,  8 ,  9 ,  9 ,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 , 
 6 ,  6 ,  6 ,  7 ,  7 ,  8 ,  8 ,  9 ,  10 ,  11 ,  12 ,  13 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 , 
 7 ,  7 ,  7 ,  8 ,  8 ,  9 ,  9 ,  10 ,  11 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 , 
 8 ,  8 ,  8 ,  9 ,  9 ,  9 ,  10 ,  11 ,  11 ,  12 ,  13 ,  14 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 , 
 9 ,  9 ,  9 ,  9 ,  10 ,  10 ,  11 ,  11 ,  12 ,  13 ,  13 ,  14 ,  15 ,  16 ,  17 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 , 
 10 ,  10 ,  10 ,  10 ,  11 ,  11 ,  12 ,  12 ,  13 ,  13 ,  14 ,  15 ,  16 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 , 
 11 ,  11 ,  11 ,  11 ,  12 ,  12 ,  13 ,  13 ,  14 ,  14 ,  15 ,  16 ,  16 ,  17 ,  18 ,  19 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 , 
 12 ,  12 ,  12 ,  12 ,  13 ,  13 ,  13 ,  14 ,  14 ,  15 ,  16 ,  16 ,  17 ,  18 ,  18 ,  19 ,  20 ,  21 ,  22 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 , 
 13 ,  13 ,  13 ,  13 ,  14 ,  14 ,  14 ,  15 ,  15 ,  16 ,  16 ,  17 ,  18 ,  18 ,  19 ,  20 ,  21 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 , 
 14 ,  14 ,  14 ,  14 ,  15 ,  15 ,  15 ,  16 ,  16 ,  17 ,  17 ,  18 ,  18 ,  19 ,  20 ,  21 ,  21 ,  22 ,  23 ,  24 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 , 
 15 ,  15 ,  15 ,  15 ,  16 ,  16 ,  16 ,  17 ,  17 ,  17 ,  18 ,  19 ,  19 ,  20 ,  21 ,  21 ,  22 ,  23 ,  23 ,  24 ,  25 ,  26 ,  27 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 , 
 16 ,  16 ,  16 ,  16 ,  16 ,  17 ,  17 ,  17 ,  18 ,  18 ,  19 ,  19 ,  20 ,  21 ,  21 ,  22 ,  23 ,  23 ,  24 ,  25 ,  26 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 , 
 17 ,  17 ,  17 ,  17 ,  17 ,  18 ,  18 ,  18 ,  19 ,  19 ,  20 ,  20 ,  21 ,  21 ,  22 ,  23 ,  23 ,  24 ,  25 ,  25 ,  26 ,  27 ,  28 ,  29 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 , 
 18 ,  18 ,  18 ,  18 ,  18 ,  19 ,  19 ,  19 ,  20 ,  20 ,  21 ,  21 ,  22 ,  22 ,  23 ,  23 ,  24 ,  25 ,  25 ,  26 ,  27 ,  28 ,  28 ,  29 ,  30 ,  31 ,  32 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 , 
 19 ,  19 ,  19 ,  19 ,  19 ,  20 ,  20 ,  20 ,  21 ,  21 ,  21 ,  22 ,  22 ,  23 ,  24 ,  24 ,  25 ,  25 ,  26 ,  27 ,  28 ,  28 ,  29 ,  30 ,  31 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 , 
 20 ,  20 ,  20 ,  20 ,  20 ,  21 ,  21 ,  21 ,  22 ,  22 ,  22 ,  23 ,  23 ,  24 ,  24 ,  25 ,  26 ,  26 ,  27 ,  28 ,  28 ,  29 ,  30 ,  30 ,  31 ,  32 ,  33 ,  34 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 , 
 21 ,  21 ,  21 ,  21 ,  21 ,  22 ,  22 ,  22 ,  22 ,  23 ,  23 ,  24 ,  24 ,  25 ,  25 ,  26 ,  26 ,  27 ,  28 ,  28 ,  29 ,  30 ,  30 ,  31 ,  32 ,  33 ,  33 ,  34 ,  35 ,  36 ,  37 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  65 ,  66 , 
 22 ,  22 ,  22 ,  22 ,  22 ,  23 ,  23 ,  23 ,  23 ,  24 ,  24 ,  25 ,  25 ,  26 ,  26 ,  27 ,  27 ,  28 ,  28 ,  29 ,  30 ,  30 ,  31 ,  32 ,  33 ,  33 ,  34 ,  35 ,  36 ,  36 ,  37 ,  38 ,  39 ,  40 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 , 
 23 ,  23 ,  23 ,  23 ,  23 ,  24 ,  24 ,  24 ,  24 ,  25 ,  25 ,  25 ,  26 ,  26 ,  27 ,  27 ,  28 ,  29 ,  29 ,  30 ,  30 ,  31 ,  32 ,  33 ,  33 ,  34 ,  35 ,  35 ,  36 ,  37 ,  38 ,  39 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 , 
 24 ,  24 ,  24 ,  24 ,  24 ,  25 ,  25 ,  25 ,  25 ,  26 ,  26 ,  26 ,  27 ,  27 ,  28 ,  28 ,  29 ,  29 ,  30 ,  31 ,  31 ,  32 ,  33 ,  33 ,  34 ,  35 ,  35 ,  36 ,  37 ,  38 ,  38 ,  39 ,  40 ,  41 ,  42 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  66 ,  67 , 
 25 ,  25 ,  25 ,  25 ,  25 ,  25 ,  26 ,  26 ,  26 ,  27 ,  27 ,  27 ,  28 ,  28 ,  29 ,  29 ,  30 ,  30 ,  31 ,  31 ,  32 ,  33 ,  33 ,  34 ,  35 ,  35 ,  36 ,  37 ,  38 ,  38 ,  39 ,  40 ,  41 ,  41 ,  42 ,  43 ,  44 ,  45 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 ,  68 , 
 26 ,  26 ,  26 ,  26 ,  26 ,  26 ,  27 ,  27 ,  27 ,  28 ,  28 ,  28 ,  29 ,  29 ,  30 ,  30 ,  31 ,  31 ,  32 ,  32 ,  33 ,  33 ,  34 ,  35 ,  35 ,  36 ,  37 ,  37 ,  38 ,  39 ,  40 ,  40 ,  41 ,  42 ,  43 ,  44 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  67 ,  68 , 
 27 ,  27 ,  27 ,  27 ,  27 ,  27 ,  28 ,  28 ,  28 ,  28 ,  29 ,  29 ,  30 ,  30 ,  30 ,  31 ,  31 ,  32 ,  32 ,  33 ,  34 ,  34 ,  35 ,  35 ,  36 ,  37 ,  37 ,  38 ,  39 ,  40 ,  40 ,  41 ,  42 ,  43 ,  43 ,  44 ,  45 ,  46 ,  47 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 ,  68 ,  69 , 
 28 ,  28 ,  28 ,  28 ,  28 ,  28 ,  29 ,  29 ,  29 ,  29 ,  30 ,  30 ,  30 ,  31 ,  31 ,  32 ,  32 ,  33 ,  33 ,  34 ,  34 ,  35 ,  36 ,  36 ,  37 ,  38 ,  38 ,  39 ,  40 ,  40 ,  41 ,  42 ,  43 ,  43 ,  44 ,  45 ,  46 ,  46 ,  47 ,  48 ,  49 ,  50 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  67 ,  68 ,  69 , 
 29 ,  29 ,  29 ,  29 ,  29 ,  29 ,  30 ,  30 ,  30 ,  30 ,  31 ,  31 ,  31 ,  32 ,  32 ,  33 ,  33 ,  34 ,  34 ,  35 ,  35 ,  36 ,  36 ,  37 ,  38 ,  38 ,  39 ,  40 ,  40 ,  41 ,  42 ,  42 ,  43 ,  44 ,  45 ,  45 ,  46 ,  47 ,  48 ,  49 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 ,  68 ,  68 ,  69 , 
 30 ,  30 ,  30 ,  30 ,  30 ,  30 ,  31 ,  31 ,  31 ,  31 ,  32 ,  32 ,  32 ,  33 ,  33 ,  34 ,  34 ,  34 ,  35 ,  36 ,  36 ,  37 ,  37 ,  38 ,  38 ,  39 ,  40 ,  40 ,  41 ,  42 ,  42 ,  43 ,  44 ,  45 ,  45 ,  46 ,  47 ,  48 ,  48 ,  49 ,  50 ,  51 ,  52 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  67 ,  68 ,  69 ,  70 , 
 31 ,  31 ,  31 ,  31 ,  31 ,  31 ,  32 ,  32 ,  32 ,  32 ,  33 ,  33 ,  33 ,  34 ,  34 ,  34 ,  35 ,  35 ,  36 ,  36 ,  37 ,  37 ,  38 ,  39 ,  39 ,  40 ,  40 ,  41 ,  42 ,  42 ,  43 ,  44 ,  45 ,  45 ,  46 ,  47 ,  48 ,  48 ,  49 ,  50 ,  51 ,  51 ,  52 ,  53 ,  54 ,  55 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 ,  68 ,  68 ,  69 ,  70 , 
 32 ,  32 ,  32 ,  32 ,  32 ,  32 ,  33 ,  33 ,  33 ,  33 ,  34 ,  34 ,  34 ,  35 ,  35 ,  35 ,  36 ,  36 ,  37 ,  37 ,  38 ,  38 ,  39 ,  39 ,  40 ,  41 ,  41 ,  42 ,  43 ,  43 ,  44 ,  45 ,  45 ,  46 ,  47 ,  47 ,  48 ,  49 ,  50 ,  50 ,  51 ,  52 ,  53 ,  54 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  67 ,  68 ,  69 ,  70 ,  71 , 
 33 ,  33 ,  33 ,  33 ,  33 ,  33 ,  34 ,  34 ,  34 ,  34 ,  34 ,  35 ,  35 ,  35 ,  36 ,  36 ,  37 ,  37 ,  38 ,  38 ,  39 ,  39 ,  40 ,  40 ,  41 ,  41 ,  42 ,  43 ,  43 ,  44 ,  45 ,  45 ,  46 ,  47 ,  47 ,  48 ,  49 ,  50 ,  50 ,  51 ,  52 ,  53 ,  53 ,  54 ,  55 ,  56 ,  57 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 ,  68 ,  68 ,  69 ,  70 ,  71 , 
 34 ,  34 ,  34 ,  34 ,  34 ,  34 ,  35 ,  35 ,  35 ,  35 ,  35 ,  36 ,  36 ,  36 ,  37 ,  37 ,  38 ,  38 ,  38 ,  39 ,  39 ,  40 ,  40 ,  41 ,  42 ,  42 ,  43 ,  43 ,  44 ,  45 ,  45 ,  46 ,  47 ,  47 ,  48 ,  49 ,  50 ,  50 ,  51 ,  52 ,  52 ,  53 ,  54 ,  55 ,  56 ,  56 ,  57 ,  58 ,  59 ,  60 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  66 ,  67 ,  68 ,  69 ,  70 ,  71 ,  72 , 
 35 ,  35 ,  35 ,  35 ,  35 ,  35 ,  36 ,  36 ,  36 ,  36 ,  36 ,  37 ,  37 ,  37 ,  38 ,  38 ,  38 ,  39 ,  39 ,  40 ,  40 ,  41 ,  41 ,  42 ,  42 ,  43 ,  44 ,  44 ,  45 ,  45 ,  46 ,  47 ,  47 ,  48 ,  49 ,  49 ,  50 ,  51 ,  52 ,  52 ,  53 ,  54 ,  55 ,  55 ,  56 ,  57 ,  58 ,  59 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  67 ,  68 ,  69 ,  69 ,  70 ,  71 ,  72 , 
 36 ,  36 ,  36 ,  36 ,  36 ,  36 ,  36 ,  37 ,  37 ,  37 ,  37 ,  38 ,  38 ,  38 ,  39 ,  39 ,  39 ,  40 ,  40 ,  41 ,  41 ,  42 ,  42 ,  43 ,  43 ,  44 ,  44 ,  45 ,  46 ,  46 ,  47 ,  48 ,  48 ,  49 ,  50 ,  50 ,  51 ,  52 ,  52 ,  53 ,  54 ,  55 ,  55 ,  56 ,  57 ,  58 ,  58 ,  59 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 ,  67 ,  68 ,  69 ,  70 ,  71 ,  72 ,  73 , 
 37 ,  37 ,  37 ,  37 ,  37 ,  37 ,  37 ,  38 ,  38 ,  38 ,  38 ,  39 ,  39 ,  39 ,  40 ,  40 ,  40 ,  41 ,  41 ,  42 ,  42 ,  43 ,  43 ,  44 ,  44 ,  45 ,  45 ,  46 ,  46 ,  47 ,  48 ,  48 ,  49 ,  50 ,  50 ,  51 ,  52 ,  52 ,  53 ,  54 ,  54 ,  55 ,  56 ,  57 ,  57 ,  58 ,  59 ,  60 ,  61 ,  61 ,  62 ,  63 ,  64 ,  65 ,  65 ,  66 ,  67 ,  68 ,  69 ,  70 ,  70 ,  71 ,  72 ,  73 , 
 38 ,  38 ,  38 ,  38 ,  38 ,  38 ,  38 ,  39 ,  39 ,  39 ,  39 ,  40 ,  40 ,  40 ,  40 ,  41 ,  41 ,  42 ,  42 ,  42 ,  43 ,  43 ,  44 ,  44 ,  45 ,  45 ,  46 ,  47 ,  47 ,  48 ,  48 ,  49 ,  50 ,  50 ,  51 ,  52 ,  52 ,  53 ,  54 ,  54 ,  55 ,  56 ,  57 ,  57 ,  58 ,  59 ,  60 ,  60 ,  61 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  67 ,  68 ,  69 ,  69 ,  70 ,  71 ,  72 ,  73 ,  74 , 
 39 ,  39 ,  39 ,  39 ,  39 ,  39 ,  39 ,  40 ,  40 ,  40 ,  40 ,  41 ,  41 ,  41 ,  41 ,  42 ,  42 ,  43 ,  43 ,  43 ,  44 ,  44 ,  45 ,  45 ,  46 ,  46 ,  47 ,  47 ,  48 ,  49 ,  49 ,  50 ,  50 ,  51 ,  52 ,  52 ,  53 ,  54 ,  54 ,  55 ,  56 ,  57 ,  57 ,  58 ,  59 ,  60 ,  60 ,  61 ,  62 ,  63 ,  63 ,  64 ,  65 ,  66 ,  67 ,  67 ,  68 ,  69 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 , 
 40 ,  40 ,  40 ,  40 ,  40 ,  40 ,  40 ,  41 ,  41 ,  41 ,  41 ,  41 ,  42 ,  42 ,  42 ,  43 ,  43 ,  43 ,  44 ,  44 ,  45 ,  45 ,  46 ,  46 ,  47 ,  47 ,  48 ,  48 ,  49 ,  49 ,  50 ,  51 ,  51 ,  52 ,  52 ,  53 ,  54 ,  54 ,  55 ,  56 ,  57 ,  57 ,  58 ,  59 ,  59 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  65 ,  66 ,  66 ,  67 ,  68 ,  69 ,  70 ,  70 ,  71 ,  72 ,  73 ,  74 ,  75 , 
 41 ,  41 ,  41 ,  41 ,  41 ,  41 ,  41 ,  42 ,  42 ,  42 ,  42 ,  42 ,  43 ,  43 ,  43 ,  44 ,  44 ,  44 ,  45 ,  45 ,  46 ,  46 ,  47 ,  47 ,  48 ,  48 ,  49 ,  49 ,  50 ,  50 ,  51 ,  51 ,  52 ,  53 ,  53 ,  54 ,  55 ,  55 ,  56 ,  57 ,  57 ,  58 ,  59 ,  59 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  65 ,  65 ,  66 ,  67 ,  68 ,  69 ,  69 ,  70 ,  71 ,  72 ,  73 ,  73 ,  74 ,  75 , 
 42 ,  42 ,  42 ,  42 ,  42 ,  42 ,  42 ,  43 ,  43 ,  43 ,  43 ,  43 ,  44 ,  44 ,  44 ,  45 ,  45 ,  45 ,  46 ,  46 ,  47 ,  47 ,  47 ,  48 ,  48 ,  49 ,  49 ,  50 ,  50 ,  51 ,  52 ,  52 ,  53 ,  53 ,  54 ,  55 ,  55 ,  56 ,  57 ,  57 ,  58 ,  59 ,  59 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  65 ,  65 ,  66 ,  67 ,  68 ,  68 ,  69 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  75 ,  76 , 
 43 ,  43 ,  43 ,  43 ,  43 ,  43 ,  43 ,  44 ,  44 ,  44 ,  44 ,  44 ,  45 ,  45 ,  45 ,  46 ,  46 ,  46 ,  47 ,  47 ,  47 ,  48 ,  48 ,  49 ,  49 ,  50 ,  50 ,  51 ,  51 ,  52 ,  52 ,  53 ,  54 ,  54 ,  55 ,  55 ,  56 ,  57 ,  57 ,  58 ,  59 ,  59 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  67 ,  67 ,  68 ,  69 ,  70 ,  71 ,  71 ,  72 ,  73 ,  74 ,  75 ,  75 ,  76 , 
 44 ,  44 ,  44 ,  44 ,  44 ,  44 ,  44 ,  45 ,  45 ,  45 ,  45 ,  45 ,  46 ,  46 ,  46 ,  46 ,  47 ,  47 ,  48 ,  48 ,  48 ,  49 ,  49 ,  50 ,  50 ,  51 ,  51 ,  52 ,  52 ,  53 ,  53 ,  54 ,  54 ,  55 ,  56 ,  56 ,  57 ,  57 ,  58 ,  59 ,  59 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  67 ,  67 ,  68 ,  69 ,  70 ,  70 ,  71 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  77 , 
 45 ,  45 ,  45 ,  45 ,  45 ,  45 ,  45 ,  46 ,  46 ,  46 ,  46 ,  46 ,  47 ,  47 ,  47 ,  47 ,  48 ,  48 ,  48 ,  49 ,  49 ,  50 ,  50 ,  51 ,  51 ,  51 ,  52 ,  52 ,  53 ,  54 ,  54 ,  55 ,  55 ,  56 ,  56 ,  57 ,  58 ,  58 ,  59 ,  60 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  67 ,  67 ,  68 ,  69 ,  70 ,  70 ,  71 ,  72 ,  73 ,  73 ,  74 ,  75 ,  76 ,  77 ,  77 , 
 46 ,  46 ,  46 ,  46 ,  46 ,  46 ,  46 ,  47 ,  47 ,  47 ,  47 ,  47 ,  48 ,  48 ,  48 ,  48 ,  49 ,  49 ,  49 ,  50 ,  50 ,  51 ,  51 ,  51 ,  52 ,  52 ,  53 ,  53 ,  54 ,  54 ,  55 ,  55 ,  56 ,  57 ,  57 ,  58 ,  58 ,  59 ,  60 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  66 ,  67 ,  68 ,  69 ,  69 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  75 ,  76 ,  76 ,  77 ,  78 , 
 47 ,  47 ,  47 ,  47 ,  47 ,  47 ,  47 ,  48 ,  48 ,  48 ,  48 ,  48 ,  49 ,  49 ,  49 ,  49 ,  50 ,  50 ,  50 ,  51 ,  51 ,  51 ,  52 ,  52 ,  53 ,  53 ,  54 ,  54 ,  55 ,  55 ,  56 ,  56 ,  57 ,  57 ,  58 ,  59 ,  59 ,  60 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  66 ,  67 ,  68 ,  69 ,  69 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  75 ,  75 ,  76 ,  77 ,  78 ,  79 , 
 48 ,  48 ,  48 ,  48 ,  48 ,  48 ,  48 ,  49 ,  49 ,  49 ,  49 ,  49 ,  49 ,  50 ,  50 ,  50 ,  51 ,  51 ,  51 ,  52 ,  52 ,  52 ,  53 ,  53 ,  54 ,  54 ,  55 ,  55 ,  56 ,  56 ,  57 ,  57 ,  58 ,  58 ,  59 ,  59 ,  60 ,  61 ,  61 ,  62 ,  62 ,  63 ,  64 ,  64 ,  65 ,  66 ,  66 ,  67 ,  68 ,  69 ,  69 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  75 ,  75 ,  76 ,  77 ,  78 ,  78 ,  79 , 
 49 ,  49 ,  49 ,  49 ,  49 ,  49 ,  49 ,  49 ,  50 ,  50 ,  50 ,  50 ,  50 ,  51 ,  51 ,  51 ,  52 ,  52 ,  52 ,  53 ,  53 ,  53 ,  54 ,  54 ,  55 ,  55 ,  55 ,  56 ,  56 ,  57 ,  57 ,  58 ,  59 ,  59 ,  60 ,  60 ,  61 ,  61 ,  62 ,  63 ,  63 ,  64 ,  65 ,  65 ,  66 ,  67 ,  67 ,  68 ,  69 ,  69 ,  70 ,  71 ,  71 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  77 ,  77 ,  78 ,  79 ,  80 , 
 50 ,  50 ,  50 ,  50 ,  50 ,  50 ,  50 ,  50 ,  51 ,  51 ,  51 ,  51 ,  51 ,  52 ,  52 ,  52 ,  52 ,  53 ,  53 ,  53 ,  54 ,  54 ,  55 ,  55 ,  55 ,  56 ,  56 ,  57 ,  57 ,  58 ,  58 ,  59 ,  59 ,  60 ,  60 ,  61 ,  62 ,  62 ,  63 ,  63 ,  64 ,  65 ,  65 ,  66 ,  67 ,  67 ,  68 ,  69 ,  69 ,  70 ,  71 ,  71 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  77 ,  77 ,  78 ,  79 ,  80 ,  80 , 
 51 ,  51 ,  51 ,  51 ,  51 ,  51 ,  51 ,  51 ,  52 ,  52 ,  52 ,  52 ,  52 ,  53 ,  53 ,  53 ,  53 ,  54 ,  54 ,  54 ,  55 ,  55 ,  56 ,  56 ,  56 ,  57 ,  57 ,  58 ,  58 ,  59 ,  59 ,  60 ,  60 ,  61 ,  61 ,  62 ,  62 ,  63 ,  64 ,  64 ,  65 ,  65 ,  66 ,  67 ,  67 ,  68 ,  69 ,  69 ,  70 ,  71 ,  71 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  76 ,  77 ,  78 ,  79 ,  80 ,  80 ,  81 , 
 52 ,  52 ,  52 ,  52 ,  52 ,  52 ,  52 ,  52 ,  53 ,  53 ,  53 ,  53 ,  53 ,  54 ,  54 ,  54 ,  54 ,  55 ,  55 ,  55 ,  56 ,  56 ,  56 ,  57 ,  57 ,  58 ,  58 ,  59 ,  59 ,  60 ,  60 ,  61 ,  61 ,  62 ,  62 ,  63 ,  63 ,  64 ,  64 ,  65 ,  66 ,  66 ,  67 ,  67 ,  68 ,  69 ,  69 ,  70 ,  71 ,  71 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  76 ,  77 ,  78 ,  79 ,  79 ,  80 ,  81 ,  82 , 
 53 ,  53 ,  53 ,  53 ,  53 ,  53 ,  53 ,  53 ,  54 ,  54 ,  54 ,  54 ,  54 ,  55 ,  55 ,  55 ,  55 ,  56 ,  56 ,  56 ,  57 ,  57 ,  57 ,  58 ,  58 ,  59 ,  59 ,  59 ,  60 ,  60 ,  61 ,  61 ,  62 ,  62 ,  63 ,  64 ,  64 ,  65 ,  65 ,  66 ,  66 ,  67 ,  68 ,  68 ,  69 ,  70 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  76 ,  77 ,  78 ,  79 ,  79 ,  80 ,  81 ,  82 ,  82 , 
 54 ,  54 ,  54 ,  54 ,  54 ,  54 ,  54 ,  54 ,  55 ,  55 ,  55 ,  55 ,  55 ,  56 ,  56 ,  56 ,  56 ,  57 ,  57 ,  57 ,  58 ,  58 ,  58 ,  59 ,  59 ,  60 ,  60 ,  60 ,  61 ,  61 ,  62 ,  62 ,  63 ,  63 ,  64 ,  64 ,  65 ,  65 ,  66 ,  67 ,  67 ,  68 ,  68 ,  69 ,  70 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  76 ,  77 ,  78 ,  79 ,  79 ,  80 ,  81 ,  81 ,  82 ,  83 , 
 55 ,  55 ,  55 ,  55 ,  55 ,  55 ,  55 ,  55 ,  56 ,  56 ,  56 ,  56 ,  56 ,  57 ,  57 ,  57 ,  57 ,  58 ,  58 ,  58 ,  59 ,  59 ,  59 ,  60 ,  60 ,  60 ,  61 ,  61 ,  62 ,  62 ,  63 ,  63 ,  64 ,  64 ,  65 ,  65 ,  66 ,  66 ,  67 ,  67 ,  68 ,  69 ,  69 ,  70 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  76 ,  77 ,  78 ,  78 ,  79 ,  80 ,  81 ,  81 ,  82 ,  83 ,  84 , 
 56 ,  56 ,  56 ,  56 ,  56 ,  56 ,  56 ,  56 ,  57 ,  57 ,  57 ,  57 ,  57 ,  57 ,  58 ,  58 ,  58 ,  59 ,  59 ,  59 ,  59 ,  60 ,  60 ,  61 ,  61 ,  61 ,  62 ,  62 ,  63 ,  63 ,  64 ,  64 ,  64 ,  65 ,  66 ,  66 ,  67 ,  67 ,  68 ,  68 ,  69 ,  69 ,  70 ,  71 ,  71 ,  72 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  76 ,  77 ,  78 ,  78 ,  79 ,  80 ,  81 ,  81 ,  82 ,  83 ,  84 ,  84 , 
 57 ,  57 ,  57 ,  57 ,  57 ,  57 ,  57 ,  57 ,  58 ,  58 ,  58 ,  58 ,  58 ,  58 ,  59 ,  59 ,  59 ,  59 ,  60 ,  60 ,  60 ,  61 ,  61 ,  61 ,  62 ,  62 ,  63 ,  63 ,  64 ,  64 ,  64 ,  65 ,  65 ,  66 ,  66 ,  67 ,  67 ,  68 ,  69 ,  69 ,  70 ,  70 ,  71 ,  71 ,  72 ,  73 ,  73 ,  74 ,  75 ,  75 ,  76 ,  76 ,  77 ,  78 ,  79 ,  79 ,  80 ,  81 ,  81 ,  82 ,  83 ,  83 ,  84 ,  85 , 
 58 ,  58 ,  58 ,  58 ,  58 ,  58 ,  58 ,  58 ,  59 ,  59 ,  59 ,  59 ,  59 ,  59 ,  60 ,  60 ,  60 ,  60 ,  61 ,  61 ,  61 ,  62 ,  62 ,  62 ,  63 ,  63 ,  64 ,  64 ,  64 ,  65 ,  65 ,  66 ,  66 ,  67 ,  67 ,  68 ,  68 ,  69 ,  69 ,  70 ,  70 ,  71 ,  72 ,  72 ,  73 ,  73 ,  74 ,  75 ,  75 ,  76 ,  77 ,  77 ,  78 ,  79 ,  79 ,  80 ,  81 ,  81 ,  82 ,  83 ,  83 ,  84 ,  85 ,  86 , 
 59 ,  59 ,  59 ,  59 ,  59 ,  59 ,  59 ,  59 ,  60 ,  60 ,  60 ,  60 ,  60 ,  60 ,  61 ,  61 ,  61 ,  61 ,  62 ,  62 ,  62 ,  63 ,  63 ,  63 ,  64 ,  64 ,  64 ,  65 ,  65 ,  66 ,  66 ,  67 ,  67 ,  68 ,  68 ,  69 ,  69 ,  70 ,  70 ,  71 ,  71 ,  72 ,  72 ,  73 ,  74 ,  74 ,  75 ,  75 ,  76 ,  77 ,  77 ,  78 ,  79 ,  79 ,  80 ,  81 ,  81 ,  82 ,  83 ,  83 ,  84 ,  85 ,  86 ,  86 , 
 60 ,  60 ,  60 ,  60 ,  60 ,  60 ,  60 ,  60 ,  61 ,  61 ,  61 ,  61 ,  61 ,  61 ,  62 ,  62 ,  62 ,  62 ,  63 ,  63 ,  63 ,  64 ,  64 ,  64 ,  65 ,  65 ,  65 ,  66 ,  66 ,  67 ,  67 ,  68 ,  68 ,  68 ,  69 ,  69 ,  70 ,  70 ,  71 ,  72 ,  72 ,  73 ,  73 ,  74 ,  74 ,  75 ,  76 ,  76 ,  77 ,  77 ,  78 ,  79 ,  79 ,  80 ,  81 ,  81 ,  82 ,  83 ,  83 ,  84 ,  85 ,  86 ,  86 ,  87 , 
 61 ,  61 ,  61 ,  61 ,  61 ,  61 ,  61 ,  61 ,  62 ,  62 ,  62 ,  62 ,  62 ,  62 ,  63 ,  63 ,  63 ,  63 ,  64 ,  64 ,  64 ,  65 ,  65 ,  65 ,  66 ,  66 ,  66 ,  67 ,  67 ,  68 ,  68 ,  68 ,  69 ,  69 ,  70 ,  70 ,  71 ,  71 ,  72 ,  72 ,  73 ,  73 ,  74 ,  75 ,  75 ,  76 ,  76 ,  77 ,  78 ,  78 ,  79 ,  80 ,  80 ,  81 ,  81 ,  82 ,  83 ,  83 ,  84 ,  85 ,  86 ,  86 ,  87 ,  88 , 
 62 ,  62 ,  62 ,  62 ,  62 ,  62 ,  62 ,  62 ,  63 ,  63 ,  63 ,  63 ,  63 ,  63 ,  64 ,  64 ,  64 ,  64 ,  65 ,  65 ,  65 ,  65 ,  66 ,  66 ,  66 ,  67 ,  67 ,  68 ,  68 ,  68 ,  69 ,  69 ,  70 ,  70 ,  71 ,  71 ,  72 ,  72 ,  73 ,  73 ,  74 ,  74 ,  75 ,  75 ,  76 ,  77 ,  77 ,  78 ,  78 ,  79 ,  80 ,  80 ,  81 ,  82 ,  82 ,  83 ,  84 ,  84 ,  85 ,  86 ,  86 ,  87 ,  88 ,  88 , 
 63 ,  63 ,  63 ,  63 ,  63 ,  63 ,  63 ,  63 ,  64 ,  64 ,  64 ,  64 ,  64 ,  64 ,  65 ,  65 ,  65 ,  65 ,  66 ,  66 ,  66 ,  66 ,  67 ,  67 ,  67 ,  68 ,  68 ,  69 ,  69 ,  69 ,  70 ,  70 ,  71 ,  71 ,  72 ,  72 ,  73 ,  73 ,  74 ,  74 ,  75 ,  75 ,  76 ,  76 ,  77 ,  77 ,  78 ,  79 ,  79 ,  80 ,  80 ,  81 ,  82 ,  82 ,  83 ,  84 ,  84 ,  85 ,  86 ,  86 ,  87 ,  88 ,  88 ,  89 
  
 };
TextVal STATI_CONSTNAMES[] = {
  { SICK, "SICK" },
  { POISONED, "POISONED" },
  { BLIND, "BLIND" },
  { HALLU, "HALLU" },
  { STUNNED, "STUNNED" },
  { CONFUSED, "CONFUSED" },
  { SLEEPING, "SLEEPING" },
  { AGG_MON, "AGG_MON" },
  { PARALYSIS, "PARALYSIS" },
  { CONFLICT, "CONFLICT" },
  { AFRAID, "AFRAID" },
  { STUCK, "STUCK" },
  { BLEEDING, "BLEEDING" },
  { ILLUS_DMG, "ILLUS_DMG" },
  { CHOKING, "CHOKING" },
  { PRONE, "PRONE" },
  { EXPOSED, "EXPOSED" },
  { WOUNDED, "WOUNDED" },
  { STONING, "STONING" },
  { DISEASED, "DISEASED" },
  { NAUSEA, "NAUSEA" },
  { TELEPORTED, "TELEPORTED" },

  { MOUNTED, "MOUNTED" },
  { MOUNT, "MOUNT" },

  { WEP_SKILL, "WEP_SKILL" },
  { FAV_ENEMY, "FAV_ENEMY" },
  { EXTRA_SKILL, "EXTRA_SKILL" },
  { TURN_ABILITY, "TURN_ABILITY" },
  { GTURN_ABILITY, "GTURN_ABILITY" },
  { COMMAND_ABILITY, "COMMAND_ABILITY" },
  { SMITE_ABILITY, "SMITE_ABILITY" },
  { SPECIALTY_SCHOOL, "SPECIALTY_SCHOOL" },
  { SCHOOL_FOCUS, "SCHOOL_FOCUS" },
  { EXTRA_ABILITY, "EXTRA_ABILITY" },
  { SPECIES_AFFINITY, "SPECIES_AFFINITY" },
  { EXTRA_FEAT, "EXTRA_FEAT" },
  { INNATE_SPELL, "INNATE_SPELL" },
  { EXTRA_ATTACK, "EXTRA_ATTACK" },
  { INNATE_KIT, "INNATE_KIT" },

  { HUNGER, "HUNGER" },
  { GRABBED, "GRABBED" },
  { GRAPPLED, "GRAPPLED" },
  { GRAPPLING, "GRAPPLING" },
  { HIDING, "HIDING" },
  { RAGING, "RAGING" },
  { SUMMONED, "SUMMONED" },
  { ACTING, "ACTING" },
  { DISPELLED, "DISPELLED" },
  { MAGIC_RES, "MAGIC_RES" },
  { TEMPLATE, "TEMPLATE" },
  { SPEC_TIMEOUT, "SPEC_TIMEOUT" },
  { TRACKING, "TRACKING" },
  { ILLUSION, "ILLUSION" },
  { CHARGING, "CHARGING" },
  { TUMBLING, "TUMBLING" },
  { SINGING, "SINGING" },
  { ANIMAL_EMP_TRIED, "ANIMAL_EMP_TRIED" },
  { TRAP_EVENT, "TRAP_EVENT" },
  { DWARVEN_FOCUS, "DWARVEN_FOCUS" },
  { DEFENSIVE, "DEFENSIVE" },
  { LFA_COUNT, "LFA_COUNT" },
  { GENERATION, "GENERATION" },
  { SUSPEND_ATTK, "SUSPEND_ATTK" },
  { WOUNDED_BY, "WOUNDED_BY" },
  { DISBELIEVED, "DISBELIEVED" },
  { FLURRYING, "FLURRYING" },
  
  { INVIS, "INVIS" },
  { INVIS_TO, "INVIS_TO" },
  { SEE_INVIS, "SEE_INVIS" },
  { PHASED, "PHASED" },
  { POLYMORPH, "POLYMORPH" },
  { RESIST, "RESIST" },
  { IMMUNITY, "IMMUNITY" },
  { ADJUST, "ADJUST" },
  { ADJUST_SAC, "ADJUST_SAC" },
  { ADJUST_COMP, "ADJUST_COMP" },
  { ADJUST_ART, "ADJUST_ART" },
  { ADJUST_DEFL, "ADJUST_DEFL" },
  { ADJUST_DMG, "ADJUST_DMG" },
  { ADJUST_INH, "ADJUST_INH" },
  { ADJUST_MOR, "ADJUST_MOR" },
  { ADJUST_DODG, "ADJUST_DODG" },
  { ADJUST_ARM, "ADJUST_ARM" },
  { ADJUST_CIRC, "ADJUST_CIRC" },
  { ADJUST_NAT, "ADJUST_NAT" },
  { ADJUST_LUCK, "ADJUST_LUCK" },
  { ADJUST_RES, "ADJUST_RES" },
  { SAVE_BONUS, "SAVE_BONUS" },
  { SKILL_BONUS, "SKILL_BONUS" },
  { SUSTAIN, "SUSTAIN" },
  { ADJUST_PAIN, "ADJUST_PAIN" },
  { REGEN, "REGEN" },
  { LEVITATION, "LEVITATION" },
  { WARNING, "WARNING" },
  { FORTIFICATION, "FORTIFICATION" },
  { WATER_WALKING, "WATER_WALKING" },
  { REFLECTING, "REFLECTING" },
  { LIFESAVED, "LIFESAVED" },
  { WATER_BREATHING, "WATER_BREATHING" },
  { ANCHORED, "ANCHORED" },
  { DETECTING, "DETECTING" },
  { TIMESTOP, "TIMESTOP" },
  { PREPARED, "PREPARED" },
  { CHARMED, "CHARMED" },
  { POSSESSED, "POSSESSED" },
  { ILLUMINATED, "ILLUMINATED" },
  { SLOW_POISON, "SLOW_POISON" },
  { PERCEPTION, "PERCEPTION" },
  { PERIODIC, "PERIODIC" },
  { CONFLICT, "CONFLICT" },
  { MANIFEST, "MANIFEST" },
  { DEFENSIVE, "DEFENSIVE" },
  { MISS_CHANCE, "MISS_CHANCE" },
  { LIFESIGHT, "LIFESIGHT" },
  { ENRAGED, "ENRAGED" },
  { TRUE_SIGHT, "TRUE_SIGHT" },
  { TOUCH_ATTACK, "TOUCH_ATTACK" },
  { IGNORE_SLOT_WEIGHT, "IGNORE_SLOT_WEIGHT" },
  { IGNORES_WEAPON_IMMUNITY, "IGNORES_WEAPON_IMMUNITY" },
  { BONUS_SCHOOL_CASTING_LEVEL, "BONUS_SCHOOL_CASTING_LEVEL" },
  { DETECTING_TYPE, "DETECTING_TYPE" },
  { LUCUBRATION, "LUCUBRATION" },
  { TRANSFORMED, "TRANSFORMED" },
  { NEGATED, "NEGATED" },
  { STAFF_SPELLS, "STAFF_SPELLS" },
  { ALT_ITEM, "ALT_ITEM" },
  { ENGULFED, "ENGULFED" },
  { EFF_FLAG1, "EFF_FLAG1" },
  { EFF_FLAG2, "EFF_FLAG2" },
  { CORPSE_FLAG, "CORPSE_FLAG" },
  { DISTRACTED, "DISTRACTED" },
  
  { COCKED, "COCKED" },
  { DAMAGED, "DAMAGED" },
  { WIZLOCK, "WIZLOCK" },
  { EXTRA_QUALITY, "EXTRA_QUALITY" },
  { ALTER_PLUS, "ALTER_PLUS" },
  { BOOST_PLUS, "BOOST_PLUS" },
  { RETRY_BONUS, "RETRY_BONUS" },
  { TRIED, "TRIED" },
  { COUNTER, "COUNTER" },
  
  { NONDETECTION, "NONDETECTION" },
  { ANIMAL_COMPANION, "ANIMAL_COMPANION" },
  { HAS_ANIMAL, "HAS_ANIMAL" },

  { NEUTRAL_TO, "NEUTRAL_TO" },
  { ENEMY_TO, "ENEMY_TO" },
  { ALLY_TO, "ALLY_TO" },
  
  { PRACTICE_MISS_CHANCE, "PRACTICE_MISS_CHANCE" },
  { ALSO_TURN_MA_TYPE, "ALSO_TURN_MA_TYPE" },
  { ADDITIONAL_WILD_SHAPE_MA_TYPE, "ADDITIONAL_WILD_SHAPE_MA_TYPE" },
  { HAS_DOMAIN, "HAS_DOMAIN" },
  { AUTO_CHARGE, "AUTO_CHARGE" },
  { ALIGNMENT, "ALIGNMENT" },
  
  { CAN_FORGET_SPELL, "CAN_FORGET_SPELL" },
  { HOME_REGION, "HOME_REGION" },
  { GAZE_REFLECTION, "GAZE_REFLECTION" },
  { EYES_AVERTED, "EYES_AVERTED" },
  { MY_CORPSE_IS, "MY_CORPSE_IS" },
  { LOCKED, "LOCKED" },
  { BONUS_SLOTS, "BONUS_SLOTS" },
  { SPELL_ACCESS, "SPELL_ACCESS" },
  { WAS_FRIENDLY, "WAS_FRIENDLY" },
  { HUNG_PULSE, "HUNG_PULSE" },
  { NO_PHD, "NO_PHD" },
  { MAGIC_AURA, "MAGIC_AURA" },
  { DEATH_LOC, "DEATH_LOC" },
  { ENCOUNTER, "ENCOUNTER" },
  { CRIT_WOUNDED_BY, "CRIT_WOUNDED_BY" },
  { SEEN_RIPPLE, "SEEN_RIPPLE" },
  { STORED_MM, "STORED_MM" },
  { LAST_STATI, "LAST_STATI" },

  { 0, NULL } };

TextVal OPCODE_CONSTNAMES[] = {
//{ ZERO, "ZERO" },
  { JUMP, "JUMP" },
  { HALT, "HALT" },
  { ADD , "ADD " },
  { SUB , "SUB " },
  { MULT, "MULT" },
  { DIV , "DIV " },
  { MOD , "MOD " },
  { MOV , "MOV " },
  { BSHL, "BSHL" },
  { BSHR, "BSHR" },
  { JTRU, "JTRU" },
  { JFAL, "JFAL" },
  { CMEQ, "CMEQ" },
  { CMNE, "CMNE" },
  { CMGT, "CMGT" },
  { CMLT, "CMLT" },
  { CMLE, "CMLE" },
  { CMGE, "CMGE" },
  { LOAD, "LOAD" },
  { NOT , "NOT " },
  { NEG , "NEG " },
  { BAND, "BAND" },
  { BOR , "BOR " },
  { LAND, "LAND" },
  { LOR , "LOR " },
  { RUN , "RUN " },
  { RET , "RET " },
  { CALL, "CALL" },
  { PUSH, "PUSH" },
  { POP , "POP " },
  { SYS , "SYS " },
  { REPI, "REPI" },
  { CMEM, "CMEM" },
  { GVAR, "GVAR" },
  { SVAR, "SVAR" },
  { GOBJ, "GOBJ" },
  { JBRK, "JBRK" },
  { SBRK, "SBRK" },
  { FBRK, "FBRK" },
  { JTAB, "JTAB" },
  { INC,  "INC " },
  { DEC,  "DEC " },
  { ROLL, "ROLL" },
  { MIN,  "MIN " },
  { MAX,  "MAX " },
  { ASTR, "ASTR" },
  { MSTR, "MSTR" },
  { CSTR, "CSTR" },
  { WSTR, "WSTR" },
  { ESTR, "ESTR" },
  { LAST, "LAST" } };



/* ww: ideally, these would be written out in a text file somewhere (or at
 * least we would have the power to read in such a text file for
 * user-supplied keysets) */

KeySetItem StandardKeySet[] = {
        { KY_CMD_NORTH,             KY_UP, 0},
        { KY_CMD_SOUTH,             KY_DOWN, 0},
        { KY_CMD_EAST,              KY_RIGHT, 0},
        { KY_CMD_WEST,              KY_LEFT, 0},
        { KY_CMD_NORTHEAST,         KY_PGUP, 0},
        { KY_CMD_NORTHWEST,         KY_HOME, 0},
        { KY_CMD_SOUTHEAST,         KY_PGDN, 0},
        { KY_CMD_SOUTHWEST,         KY_END, 0},

        { KY_CMD_NORTH,             KY_UP, KY_FLAG_SHIFT},
        { KY_CMD_SOUTH,             KY_DOWN, KY_FLAG_SHIFT},
        { KY_CMD_EAST,              KY_RIGHT, KY_FLAG_SHIFT},
        { KY_CMD_WEST,              KY_LEFT, KY_FLAG_SHIFT},
        { KY_CMD_NORTHEAST,         KY_PGUP, KY_FLAG_SHIFT},
        { KY_CMD_NORTHWEST,         KY_HOME, KY_FLAG_SHIFT},
        { KY_CMD_SOUTHEAST,         KY_PGDN, KY_FLAG_SHIFT},
        { KY_CMD_SOUTHWEST,         KY_END, KY_FLAG_SHIFT},

        { KY_CMD_LOOK_NORTH,        KY_UP,   KY_FLAG_ALT},
        { KY_CMD_LOOK_SOUTH,        KY_DOWN, KY_FLAG_ALT},
        { KY_CMD_LOOK_EAST,         KY_RIGHT,KY_FLAG_ALT},
        { KY_CMD_LOOK_WEST,         KY_LEFT, KY_FLAG_ALT},
        { KY_CMD_LOOK_NORTHEAST,    KY_PGUP, KY_FLAG_ALT},
        { KY_CMD_LOOK_NORTHWEST,    KY_HOME, KY_FLAG_ALT},
        { KY_CMD_LOOK_SOUTHEAST,    KY_PGDN, KY_FLAG_ALT},
        { KY_CMD_LOOK_SOUTHWEST,    KY_END,  KY_FLAG_ALT},

        { KY_CMD_ESCAPE,            KY_ESC, 0},
        { KY_CMD_HELP,              '?', -1},
        { KY_CMD_LEGEND,            '/', -1},
        { KY_CMD_TAB,               KY_TAB, 0},
        { KY_CMD_ENTER,             KY_ENTER, 0},

        { KY_CMD_ALL_ALLIES,      'A', 0},
        { KY_CMD_LOOK,            'L', 0},
        { KY_CMD_INVENTORY,       'I', 0},
        { KY_CMD_TARGET,          'T', 0},
        { KY_CMD_DIRECTION,       'D', 0},
        { KY_CMD_EXAMINE,         'X', 0},
        { KY_CMD_EXAMINE_REGION,  'R', 0},
        { KY_CMD_EXAMINE_FEATURE, 'F', 0},
        { KY_CMD_EXAMINE_MAP,     'M', 0},
        { KY_CMD_NEXT,            'N', 0},
        { KY_CMD_OVERVIEW_MAP,    'O', 0},
        { KY_CMD_TARGET_SELF,     '*', -1},
        { KY_CMD_TARGET_MOUNT,    'H', 0},

        { KY_CMD_NAME,            'N', 0},
        { KY_CMD_UP,              '<', -1},
        { KY_CMD_DOWN,            '>', -1},
        { KY_CMD_LEGEND,          '/', -1},
        { KY_CMD_DEBUG_MESSAGES,  '!', -1},
        { KY_CMD_TALK,            'T', 0},
        { KY_CMD_EAT,             'E', 0},
        
        
        { KY_CMD_ACTIVATE,        'A', 0},
        { KY_CMD_BLAST_WAND,      'B', 0}, 
        { KY_CMD_CHAR_SHEET,      'D', 0}, 
        
        { KY_CMD_READ,            'R', 0},
        
        { KY_CMD_QUAFF,           'Q', 0}, 
        { KY_CMD_OPEN,            'O', 0}, 
        { KY_CMD_FIRE,            'F', 0}, 
        { KY_CMD_GET,             'G', 0}, 
        { KY_CMD_HIDE,            'H', 0}, 
        { KY_CMD_WIZMODE,         'W', 0}, 
        { KY_CMD_SHOW_MESSAGES,   'V', 0}, 
        { KY_CMD_MAGIC,           'M', 0}, 
        { KY_CMD_SEARCH,          'S', 0}, 
        { KY_CMD_ATTACK_MENU,     'C', 0}, 
        { KY_CMD_USE,             'U', 0}, 
        { KY_CMD_YUSE,            'Y', 0}, 
        { KY_CMD_CANCEL_MENU,     'X', 0}, 
        { KY_CMD_JUMP,            'J', 0}, 
        { KY_CMD_KICK,            'K', 0},
        { KY_CMD_PRAY,            'P', 0}, 
        { KY_CMD_OPTIONS,         '=', -1}, 
        { KY_CMD_REST,            '.', -1}, 
        { KY_CMD_RUN,             ',', -1}, 
        { KY_CMD_EXCHANGE,        '-', -1}, 
        { KY_CMD_SLEEP,           'Z', 0}, 
        { KY_CMD_LAST, 0, 0} 
} ; 

KeySetItem RoguelikeKeySet[] = {
        /* you can still use the arrowkeys in this keyset ... */
        { KY_CMD_NORTH,        KY_UP, 0},
        { KY_CMD_SOUTH,        KY_DOWN, 0},
        { KY_CMD_EAST,         KY_RIGHT, 0},
        { KY_CMD_WEST,         KY_LEFT, 0},
        { KY_CMD_NORTHEAST,    KY_PGUP, 0},
        { KY_CMD_NORTHWEST,    KY_HOME, 0},
        { KY_CMD_SOUTHEAST,    KY_PGDN, 0},
        { KY_CMD_SOUTHWEST,    KY_END, 0},

        { KY_CMD_NORTH,        KY_UP, KY_FLAG_SHIFT},
        { KY_CMD_SOUTH,        KY_DOWN, KY_FLAG_SHIFT},
        { KY_CMD_EAST,         KY_RIGHT, KY_FLAG_SHIFT},
        { KY_CMD_WEST,         KY_LEFT, KY_FLAG_SHIFT},
        { KY_CMD_NORTHEAST,    KY_PGUP, KY_FLAG_SHIFT},
        { KY_CMD_NORTHWEST,    KY_HOME, KY_FLAG_SHIFT},
        { KY_CMD_SOUTHEAST,    KY_PGDN, KY_FLAG_SHIFT},
        { KY_CMD_SOUTHWEST,    KY_END, KY_FLAG_SHIFT},

        { KY_CMD_LOOK_NORTH,        KY_UP,   KY_FLAG_ALT},
        { KY_CMD_LOOK_SOUTH,        KY_DOWN, KY_FLAG_ALT},
        { KY_CMD_LOOK_EAST,         KY_RIGHT,KY_FLAG_ALT},
        { KY_CMD_LOOK_WEST,         KY_LEFT, KY_FLAG_ALT},
        { KY_CMD_LOOK_NORTHEAST,    KY_PGUP, KY_FLAG_ALT},
        { KY_CMD_LOOK_NORTHWEST,    KY_HOME, KY_FLAG_ALT},
        { KY_CMD_LOOK_SOUTHEAST,    KY_PGDN, KY_FLAG_ALT},
        { KY_CMD_LOOK_SOUTHWEST,    KY_END,  KY_FLAG_ALT},

        /* but you really want to be using these guys! */
        { KY_CMD_NORTH,        'K', 0},
        { KY_CMD_SOUTH,        'J', 0},
        { KY_CMD_EAST,         'L', 0},
        { KY_CMD_WEST,         'H', 0},
        { KY_CMD_NORTHEAST,    'U', 0},
        { KY_CMD_NORTHWEST,    'Y', 0},
        { KY_CMD_SOUTHEAST,    'N', 0},
        { KY_CMD_SOUTHWEST,    'B', 0},

        { KY_CMD_NORTH,        'K', KY_FLAG_SHIFT},
        { KY_CMD_SOUTH,        'J', KY_FLAG_SHIFT},
        { KY_CMD_EAST,         'L', KY_FLAG_SHIFT},
        { KY_CMD_WEST,         'H', KY_FLAG_SHIFT},
        { KY_CMD_NORTHEAST,    'U', KY_FLAG_SHIFT},
        { KY_CMD_NORTHWEST,    'Y', KY_FLAG_SHIFT},
        { KY_CMD_SOUTHEAST,    'N', KY_FLAG_SHIFT},
        { KY_CMD_SOUTHWEST,    'B', KY_FLAG_SHIFT},

        { KY_CMD_LOOK_NORTH,        'K', KY_FLAG_ALT},
        { KY_CMD_LOOK_SOUTH,        'J', KY_FLAG_ALT},
        { KY_CMD_LOOK_EAST,         'L', KY_FLAG_ALT},
        { KY_CMD_LOOK_WEST,         'H', KY_FLAG_ALT},
        { KY_CMD_LOOK_NORTHEAST,    'U', KY_FLAG_ALT},
        { KY_CMD_LOOK_NORTHWEST,    'Y', KY_FLAG_ALT},
        { KY_CMD_LOOK_SOUTHEAST,    'N', KY_FLAG_ALT},
        { KY_CMD_LOOK_SOUTHWEST,    'B', KY_FLAG_ALT},

        { KY_CMD_ESCAPE,       KY_ESC, -1},
        { KY_CMD_HELP,         KY_HELP, -1},
        { KY_CMD_TAB,          KY_TAB, -1},
        { KY_CMD_ENTER,        KY_ENTER, -1},

        { KY_CMD_ALL_ALLIES,      'A', 0},
        { KY_CMD_TARGET,       'T', 0},
        { KY_CMD_DIRECTION,    'D', 0},
        { KY_CMD_EXAMINE,      'X', 0},
        { KY_CMD_EXAMINE_REGION, 'R', 0},
        { KY_CMD_EXAMINE_FEATURE, 'F', 0},
        { KY_CMD_EXAMINE_MAP, 'M', 0},
        { KY_CMD_NEXT,        'N', KY_FLAG_CONTROL},
        { KY_CMD_OVERVIEW_MAP, 'O', 0},
        { KY_CMD_TARGET_SELF,  '*', -1},
        { KY_CMD_TARGET_MOUNT, 'H', KY_FLAG_CONTROL},

        { KY_CMD_UP,              '<', -1},
        { KY_CMD_DOWN,            '>', -1},
        { KY_CMD_JOURNAL,      'E', KY_FLAG_CONTROL},
        { KY_CMD_TREAT_MONSTER,'T', KY_FLAG_CONTROL},
        
        { KY_CMD_ACTIVATE,     'A', 0},
        { KY_CMD_BLAST_WAND,   'B', KY_FLAG_CONTROL}, 
        { KY_CMD_ATTACK_MENU,  'C', 0}, 
        { KY_CMD_CHAR_SHEET,   'D', 0}, 
        { KY_CMD_EAT,          'E', 0},
        { KY_CMD_FIRE,         'F', 0}, 
        { KY_CMD_GET,          'G', 0}, 
        { KY_CMD_HIDE,         'H', KY_FLAG_CONTROL}, 
        { KY_CMD_INVENTORY,    'I', 0},
        { KY_CMD_JUMP,         'J', KY_FLAG_CONTROL }, 
        { KY_CMD_KICK,         'K', KY_FLAG_CONTROL }, 
        { KY_CMD_LOOK,         'L', KY_FLAG_CONTROL},
        { KY_CMD_MAGIC,        'M', 0}, 
        { KY_CMD_NAME,         'N', KY_FLAG_CONTROL},
        { KY_CMD_OPEN,         'O', 0}, 
        { KY_CMD_PRAY,         'P', 0},
        { KY_CMD_QUAFF,        'Q', 0}, 
        { KY_CMD_READ,         'R', 0},
        { KY_CMD_SEARCH,       'S', 0}, 
        { KY_CMD_TALK,         'T', 0},
        { KY_CMD_USE,          'U', KY_FLAG_CONTROL }, 
        { KY_CMD_SHOW_MESSAGES,'V', 0}, 
        { KY_CMD_WIZMODE,      'W', 0}, 
        { KY_CMD_CANCEL_MENU,  'X', 0}, 
        { KY_CMD_YUSE,         'Y', KY_FLAG_CONTROL }, 
        { KY_CMD_SLEEP,        'Z', 0}, 
        
        { KY_CMD_OPTIONS,      KY_EQUALS, -1}, 
        { KY_CMD_REST,         KY_PERIOD, -1}, 
        { KY_CMD_RUN,          KY_COMMA,  -1}, 
        { KY_CMD_EXCHANGE,     KY_MINUS,  -1}, 
        { KY_CMD_LAST, 0, 0} 
} ; 

// ww: why can we see something? 
TextVal PerDescs[] = {
  { PER_VISUAL, "sight" },
  { PER_INFRA, "infra" },
  { PER_SCENT, "scent" },
  { PER_BLIND, "blindsight" },
  { PER_PERCEPT, "perception" },
  { PER_TREMOR, "tremor" },
  { PER_SHADOW, "shadowy" },
  { PER_TELE, "ESP" },
  { PER_DETECT, "detect" },
  { PER_TRACK, "track" },
  { PER_SHARED, "shared" }, // share senses with animal companion
  { 0, 0},
};

TextVal KeyCmdDescs[] = {
  {       KY_CMD_NORTH           ,"North"},
  {       KY_CMD_SOUTH           ,"South"},
  {       KY_CMD_EAST            ,"East"},
  {       KY_CMD_WEST            ,"West"},
  {       KY_CMD_NORTHEAST       ,"North-East"},
  {       KY_CMD_NORTHWEST       ,"North-West"},
  {       KY_CMD_SOUTHEAST       ,"South-East"},
  {       KY_CMD_SOUTHWEST       ,"South-West"},
  /*
  {       KY_CMD_ESCAPE          ,"Escape"},
  {       KY_CMD_HELP            ,"Help"},
  {       KY_CMD_TAB             ,"Tab"},
  {       KY_CMD_ENTER           ,"Enter"},
  */
  {       KY_CMD_TARGET          ,"Target"},
  {       KY_CMD_DIRECTION       ,"Direction"},
  {       KY_CMD_EXAMINE         ,"Examine"},
  {       KY_CMD_EXAMINE_REGION  ,"Examine Region"},
  {       KY_CMD_EXAMINE_FEATURE ,"Examine Feature"},
  {       KY_CMD_EXAMINE_MAP     ,"Examine Monster"},
  {       KY_CMD_NEXT            ,"Next Object"},
  {       KY_CMD_OVERVIEW_MAP    ,"Overview Map"},
  {       KY_CMD_TARGET_SELF     ,"Target Self"},
  {       KY_CMD_TARGET_MOUNT    ,"Target Mount"},
   
  {       KY_CMD_UP              ,"Go Up"},
  {       KY_CMD_DOWN            ,"Go Down"},
  {       KY_CMD_LEGEND          ,"Legend"},
  {       KY_CMD_DEBUG_MESSAGES  ,"Debug Messages"},

  {       KY_CMD_ACTIVATE        ,"Activate an Item"},
  {       KY_CMD_BLAST_WAND      ,"Blast Wand/Staff"},
  {       KY_CMD_ATTACK_MENU     ,"Combat Options"},
  {       KY_CMD_CHAR_SHEET      ,"Display Character"},
  {       KY_CMD_EAT             ,"Eat"},
  {       KY_CMD_FIRE            ,"Fire Ranged Weapon"},
  {       KY_CMD_GET             ,"Get Item"},
  {       KY_CMD_HIDE            ,"Hide In Shadows"},
  {       KY_CMD_INVENTORY       ,"Inventory"},
  {       KY_CMD_JUMP            ,"Jump"},
  {       KY_CMD_KICK            ,"Kick"},
  {       KY_CMD_LOOK            ,"Look"},
  {       KY_CMD_MAGIC           ,"Cast Magic Spell"},
  {       KY_CMD_NAME            ,"Name"},
  {       KY_CMD_OPEN            ,"Open Door/Chest"},
  {       KY_CMD_PRAY            ,"Pray" },
  {       KY_CMD_QUAFF           ,"Quaff Potion"},
  {       KY_CMD_READ            ,"Read Scroll"},
  {       KY_CMD_SEARCH          ,"Search"},
  {       KY_CMD_TALK            ,"Talk"},
  {       KY_CMD_USE             ,"Use Skill"},
  {       KY_CMD_SHOW_MESSAGES   ,"View Messages"},
  {       KY_CMD_WIZMODE         ,"Wizard Mode"},
  {       KY_CMD_CANCEL_MENU     ,"Cancel Menu"},
  {       KY_CMD_YUSE            ,"Use Item"},
  {       KY_CMD_SLEEP           ,"Rest and Recover"},
  {       KY_CMD_REST            ,"Wait One Turn"},
  {       KY_CMD_OPTIONS         ,"Options"},
  {       KY_CMD_RUN             ,"Run"},
  {       KY_CMD_EXCHANGE        ,"Exchange Weapons"},
   
  {       KY_CMD_JOURNAL         ,"Make Journal Entry" },
  {       KY_CMD_LOOK_NORTH      ,"Look North" },
  {       KY_CMD_LOOK_SOUTH      ,"Look South" },
  {       KY_CMD_LOOK_EAST       ,"Look East" },
  {       KY_CMD_LOOK_WEST       ,"Look West" },
  {       KY_CMD_LOOK_NORTHEAST  ,"Look North-East" },
  {       KY_CMD_LOOK_NORTHWEST  ,"Look North-West" },
  {       KY_CMD_LOOK_SOUTHEAST  ,"Look South-East" },
  {       KY_CMD_LOOK_SOUTHWEST  ,"Look South-West" },

}; 

TextVal SpellPurposeNames[] = {
  { EP_ATTACK,          "Direct Attack" }, 
  { EP_STEALTH,         "Stealth" }, 
  { EP_CURSE,           "Indirect Attack" }, 
  { EP_ESCAPE,          "Rapid Escape" },
  { EP_BUFF,            "Buff" },
  { EP_PLAYER_ONLY,     "Usable By Human Players (You!) Only" },
  { EP_FOIL_PURSUIT,    "Foil Pursuit" },
  { EP_FIX_TROUBLE,     "Fix Trouble" },
  { EP_SUMMON,          "Summoning" },
  { EP_COUNTER,         "<8>Counter<7>" }, // ww: unused
  { EP_PASSAGE,         "<8>Passage<7>" }, // ww: unused
  { EP_BARRIER,         "<8>Defensive Barrier<7>" }, // ww: unused
  { AIM_AT_ENEMY,       "Aim It At An Enemy" },
  { AIM_AT_SELF,        "Aim It At Yourself" },
  { AIM_AT_ALLY,        "Aim It At An Ally" },
  { EP_UNLOCK,           "Unlock" },
        { 0, NULL }};
