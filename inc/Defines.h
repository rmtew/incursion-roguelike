/* DEFINES.H -- Copyright (c) 1999-2003 Julian Mensch
     This file contains definitions of all the constants,
   bitflags, data types and macros used in Incursion. It 
   is a source file for both C++ and IncursionScript, 
   both of which share the standard C preprocessor and 
   many lexical conventions. As such, anything that won't 
   compile in one of these languages should be bracketed
   in #ifdef/#endif statements appropriately. 
*/

#define SIGNATURE 0x1248ABCD
#define SIGNATURE_TWO 0xF1F1F1F1
#define VERSION_STRING "0.6.5"

#ifndef ICOMP
#define __MINMAX_DEFINED
typedef unsigned char      uint8;
typedef signed char         int8;
typedef unsigned short     uint16;
typedef signed short        int16;
typedef unsigned long     uint32;
typedef signed long        int32;
typedef unsigned long     rID;
typedef signed char       Dir;
typedef void*             Param;
typedef unsigned short    Glyph;
typedef int8              EvReturn;

typedef signed long       hText;
typedef signed long       hCode;
typedef signed long       hData;
typedef signed long       hObj;

#define OBJ_TABLE_SIZE   65536
/* ww: was 4096, but a recent profile revealed Registry::Get() to be 
 * the function in which we spend the second-most time. This adds 480k
 * to Incursion's memory footprint. */
#define DATA_TABLE_SIZE   512

#define OBJ_TABLE_MASK   65535
#define DATA_TABLE_MASK  511
/* instead of "h % OBJ_TABLE_SIZE" we use "h & OBJ_TABLE_MASK" */

#define MAX_PLAYERS         4
#define MAX_SCROLL_LINES    8192
#define SCROLL_WIDTH        120
#define MAX_MONSTER_ACTIONS 15
#define MAX_GALLERY_SIZE    16
#define EVENT_STACK_SIZE    128
#define STRING_QUEUE_SIZE   64000


#ifdef ASSERT
  #undef ASSERT
#endif

//#ifdef DEBUG
  #define ASSERT(a) if (!(a)) Error("ASSERT failed: '" #a \
    "' in file %s, line %d.", __FILE__, __LINE__);
//#else
//  #define ASSERT(a) ;
//#endif
#define MYABS(n) ((n)>0 ? (n) : -(n))

#endif

#ifdef WIN32
  #define BREAKOUT __asm int 3;
#else
  #define BREAKOUT asm("int $3");
#endif

#ifdef ICOMP
  #define XCR(CR) ((CR+3)*(CR+3)*(CR+3))
#else
  #define XCR(CR) ((CR+3L)*(CR+3L)*(CR+3L))
#endif

#define HOUR_TURNS 18000L

#define ERROR    -1
#define NOTHING   0
#define DONE      1
#define ABORT     2
#define NOMSG     3

// reminder EV_RETURNs for EV_ISTARGET
#define SHOULD_CAST_IT  0       /* AutoBuff and monster AI will do so */
#define CAN_CAST_IT     1       /* player can, monsters won't */
#define CANNOT_CAST_IT  -1      /* No one can: invalid target */


#define FIRST_DIR 0
#define NORTH     0
#define SOUTH     1
#define EAST      2
#define WEST      3
#define NORTHEAST 4
#define NORTHWEST 5
#define SOUTHEAST 6
#define SOUTHWEST 7
#define CENTER    8
#define UP        9
#define DOWN      10
#define LAST_DIR 7

/* Coin Types */
#define GOLD      1
#define PLATINUM  2
#define SILVER    3
#define COPPER    4
#define ELECTRUM  5

#define SAL(a) (__SAL[min((a),9)])

/* These constants can appear in dice values rolled with the
   LRoll function. They allow level-based dice rolls. */



#define LEVEL_1PER1  -101
#define LEVEL        -101
#define LEVEL_2PER1  -102 /* 1 every two levels */
#define LEVEL_3PER1  -103 /* 1 every three levels */
#define LEVEL_4PER1  -104
#define LEVEL_5PER1  -105

#define LEVEL_1PER2  -106
#define LEVEL_1PER3  -107
#define LEVEL_1PER4  -108
#define LEVEL_1PER5  -109

#define LEVEL_2PER3  -110
#define LEVEL_3PER2  -111
#define LEVEL_3PER4  -112
#define LEVEL_4PER3  -113
#define LEVEL_EVERY2 -114
#define LEVEL_EVERY3 -115
#define LEVEL_EVERY4 -116
#define LEVEL_EVERY5 -117
                      
#define LEVEL_2EVERY2 -118 
#define LEVEL_2EVERY3 -119
#define LEVEL_2EVERY4 -120
#define LEVEL_2EVERY5 -121

#define LEVEL_MAX5   -122
#define LEVEL_MAX10  -123
#define LEVEL_MAX15  -124
#define LEVEL_MAX20  -125

#define LEVEL_SCALED  -126
#define LEVEL_SCALED2 -99
#define LEVEL_SCALED3 -98

#define LEVEL_PLUS5   -97
#define LEVEL_PLUS10  -96
#define LEVEL_PLUS20  -95
#define LEVEL_HALF_PLUS3 -94

#define PLUS_1PER1    -93
#define PLUS_2PER1    -92
#define PLUS_3PER1    -91
#define PLUS_4PER1    -90
#define PLUS_5PER1    -89
#define PLUS_10PER1   -88
#define PLUS_15PER1   -87
#define PLUS_20PER1   -86
#define PLUS_MINUS2   -85
#define PLUS_MINUS3   -84

#define LEVEL_MIN3    -83
#define LEVEL_MIN5    -82
#define LEVEL_MIN10   -81

#define PLUS_ADD1     -80
#define PLUS_ADD2     -79
#define PLUS_ADD3     -78
#define PLUS_ADD4     -77
#define PLUS_ADD5     -76
#define PLUS_ADD7     -75
#define PLUS_ADD10    -74
#define PLUS_ADD15    -73

#define PLUS_2PER1_ADD1     -72
#define PLUS_2PER1_ADD2     -71
#define PLUS_2PER1_ADD3     -70
#define PLUS_2PER1_ADD4     -69
#define PLUS_2PER1_ADD5     -68
#define PLUS_2PER1_ADD7     -67
#define PLUS_2PER1_ADD10    -66
#define PLUS_2PER1_ADD15    -65

#define PLUS_3PER1_ADD1     -64
#define PLUS_3PER1_ADD2     -63
#define PLUS_3PER1_ADD3     -62
#define PLUS_3PER1_ADD4     -61
#define PLUS_3PER1_ADD5     -60
#define PLUS_3PER1_ADD7     -59
#define PLUS_3PER1_ADD10    -58
#define PLUS_3PER1_ADD15    -57

#define PLUS_4PER1_ADD1     -56
#define PLUS_4PER1_ADD3     -55
#define PLUS_4PER1_ADD5     -54
#define PLUS_4PER1_ADD7     -53
#define PLUS_4PER1_ADD10    -52
#define PLUS_4PER1_ADD15    -51

#define PLUS_5PER1_ADD1     -50
#define PLUS_5PER1_ADD3     -49
#define PLUS_5PER1_ADD5     -48
#define PLUS_5PER1_ADD7     -47
#define PLUS_5PER1_ADD10    -46
#define PLUS_5PER1_ADD15    -45

#define LEVEL_LAST -44
                                                  
#define MVAL_NONE  0
#define MVAL_ADD   1
#define MVAL_SET   2
#define MVAL_PERCENT 3

#define MBOUND_NONE 0
#define MBOUND_MIN  1
#define MBOUND_MAX  2
#define MBOUND_NEAR 3

#define LUC LUCK

#define F_SOLID          0x0001 /* Prevents Travel            */
#define F_XSOLID         0x0002 /* Prevents Etherial Travel   */
#define F_INVIS          0x0004
#define F_DELETE         0x0008 /* Marked for deletion        */
#define F_UPDATE         0x0010 /* Glyph changed this turn    */
#define F_HILIGHT        0x0100 /* For Detect effects; later, 1/player. */
#define F_OPAQUE         0x0200
#define F_HIDING         0x0400
#define F_FOCUS          0x0800 /* dwarven focus target */
#define F_NO_MEMORY      0x1000 /* don't show on the map if it is out of sight */
#define F_TREE           0x2000 
#define F_OBSCURE        0x4000
#define F_STICKY         0x8000
#define F_WARN          0x10000
#define F_BRIDGE        0x20000
#define F_ALTAR         0x40000

#define KN_NATURE    0x01
#define KN_CURSE     0x02
#define KN_BLESS     0x02
#define KN_MAGIC     0x04
#define KN_PLUS      0x08
#define KN_PLUS2     0x10
#define KN_ARTI      0x20
#define KN_RUSTPROOF 0x40
#define KN_IDENTIFIED 0x60
#define KN_HANDLED   0x80

#define IF_BLESSED     0x0001
#define IF_CURSED      0x0002
#define IF_WORN        0x0004
#define IF_WEILDED     0x0004
#define IF_HELD        0x0004
#define IF_MASTERWORK  0x0008
#define IF_BROKEN      0x0010
#define IF_INTUITED    0x0020
#define IF_PROPERTY    0x0040
#define IF_RUNIC       0x0080
#define IF_DECIPHERED  0x0100
#define IF_GHOST_TOUCH 0x0200
#define IF_REAPPLYING  0x0400

/* The core game objects */
#define T_GAME     1
#define T_OBJECT   2
#define T_TERM     3
#define T_MAP      4
#define T_REGISTRY 5
#define T_MONSTER  6
#define T_PLAYER   7
#define T_NPC      8

/* The Features */
#define T_PORTAL   10
#define T_DOOR     11
#define T_TRAP     12
#define T_FEATURE  13
#define T_ALTAR    14
#define T_BARRIER  15

/* The Items */
#define T_FIRSTITEM 20
#define T_AMULET    20
#define T_ARMOR     21
#define T_BOOK      22
#define T_BOOTS      23
#define T_BOW       24
#define T_BRACERS   25
#define T_CHEST     26
#define T_CLOTHES   27
#define T_CLOAK     28
#define T_COIN      29
#define T_CONTAIN   30
#define T_CORPSE    31
#define T_CROWN     32
#define T_DECK      33
#define T_DUST      34
#define T_EYES      35
#define T_FIGURE    36
#define T_FOOD      37
#define T_GAUNTLETS 38
#define T_GEM       39
#define T_GIRDLE    40
#define T_HELMET    41
#define T_HERB      42
#define T_LIGHT     44
#define T_MISSILE   45
#define T_MUSH      46
#define T_POTION    47
#define T_RING      48
#define T_ROD        49
#define T_SCROLL    50
#define T_SHIELD    51
#define T_STAFF      52
#define T_STATUE    53
#define T_SYMBOL    54
#define T_TOME      55
#define T_TOOL      56
#define T_WAND      57
#define T_WEAPON    58
#define T_WONDER    59
#define T_FOUNTAIN  60
#define T_LASTITEM  61

/* Remember to update code in RComp.cpp if you alter these. */
#define T_TMONSTER   62
#define T_TITEM      63
#define T_TFEATURE   64
#define T_TEFFECT    65
#define T_TARTIFACT  66
#define T_TQUEST     67
#define T_TDUNGEON   68
#define T_TROUTINE   69
#define T_TNPC       70
#define T_TCLASS     70
#define T_TRACE      72
#define T_TDOMAIN    73
#define T_TGOD       74
#define T_TREGION    75
#define T_TTERRAIN   76
#define T_TTEXT      77
#define T_TVARIABLE  78
#define T_TTEMPLATE  79
#define T_TFLAVOR    80
#define T_MODULE     81

#define T_ANNOT      90
#define T_RESOURCE   91
#define T_LAST       92

#define T_STRING 99
#define T_ARRAY 100

#define T_CREATURE   (-2)
#define T_CHARACTER  (-3)
#define T_ITEM       (-4)
#define T_EVENTINFO  (-5)

/* This should be the last T_CONST here so when this
   file is auto-scanned to build T_CONSTNAMES, the -1
   value appears last. DO NOT MOVE! */
#define T_THING      (-1)


#define is_res(ot) (ot >= T_TMONSTER && ot <= T_TFLAVOR)

#define SOBJ_E        1
#define SOBJ_ETERM    2
#define SOBJ_EPLAYER  3
#define SOBJ_EACTOR   4
#define SOBJ_ETARGET  5
#define SOBJ_EVICTIM  6
#define SOBJ_EITEM    7
#define SOBJ_EITEM2   8
#define SOBJ_EMAP     9
#define SOBJ_GAME     10

#define AB_ABILITY   1
#define AB_STATI     2
#define AB_FEAT      3
#define AB_INNATE    4
#define AB_EFFECT    5

#define AN_SPELLS     1 /* Spellbook Spells */
#define AN_INNATE     2 /* Innate Abilities */
#define AN_EQUIP      3 /* Starting Equipment */
#define AN_EVENT      4 /* Event Messages & Routined */

#define AN_ELEMENT    6 /* Map Element */
#define AN_ABILITY    7 /* Race/Class Ability */
#define AN_ART_HIT    8
#define AN_ART_WIELD  9
#define AN_ART_EQUIP  10
#define AN_ART_INVOKE 11
#define AN_RELIGION   12 /* Force Religion for Class */
#define AN_FIRES      13 /* Allowed Ammo List for Ranged Wep */
#define AN_DUNCONST   14
#define AN_DUNLIST    15
#define AN_DUNSPEC    16
#define AN_AMMO       17 /* Acceptable Ammo for Weapons */
#define AN_EFFECT     18 /* Linked Effect */
#define AN_ITEM_LIST  19 /* Items that can host an effect */
#define AN_MAP_TILES  20
#define AN_DISEASE    21
#define AN_POISON     22
#define AN_TILE       24

#define SL_INAIR     0
#define SL_READY     1
#define SL_READYHAND 1
#define SL_WEAPON    2
#define SL_WEPHAND   2
#define SL_LIGHT     3
#define SL_LSHOULDER 4
#define SL_RSHOULDER 5
#define SL_BELT      6 /*Make belts containers? */
#define SL_EYES      7
#define SL_CLOTHES   8
#define SL_ARMOR     9
#define SL_BOOTS     10
#define SL_CLOAK     11
#define SL_LRING     12
#define SL_RRING     13
#define SL_AMULET    14
#define SL_GAUNTLETS 15
#define SL_HELM      16
#define SL_BRACERS   17
#define SL_BELT1     18
#define SL_BELT2     19
#define SL_BELT3     20
#define SL_BELT4     21
#define SL_BELT5     22
#define SL_PACK      23
#define SL_LAST      24
#define NUM_SLOTS    24

/* Hack -- see Monster::Inventory() */
#define SL_ARCHERY   50

#define SM_CAST           0x01
#define SM_TRICKERY       0x02


#define MAT_LIQUID    1 
#define MAT_WAX    2
#define MAT_VEGGY    3  
#define MAT_FLESH    4  
#define MAT_PAPER    5
#define MAT_CLOTH    6
#define MAT_LEATHER    7
#define MAT_WOOD    8
#define MAT_BONE    9
#define MAT_DRAGON_HIDE  10  
#define MAT_IRON    11  
#define MAT_METAL    12  
#define MAT_COPPER    13 
#define MAT_SILVER    14  
#define MAT_GOLD    15 
#define MAT_PLATINUM  16 
#define MAT_MITHRIL    17
#define MAT_ADAMANT  18
#define MAT_GLASS    19
#define MAT_GEMSTONE  20
#define MAT_MINERAL    21
#define MAT_WEBBING   22
#define MAT_INDESTRUCTABLE_STONE 23 
#define MAT_ICE      24
#define MAT_MAGMA    25
#define MAT_QUARTZ   26
#define MAT_CLOUD    27
#define MAT_GRANITE  28
#define MAT_DARKWOOD 29
#define MAT_FORCE    30
#define MAT_EMPTYNESS 31 
#define MAT_OIL      32
#define MAT_MUD      33
#define MAT_WROUGHT  34
#define MAT_IRONWOOD 35

#define IT_NOGEN     1
#define IT_EDIBLE    2
#define IT_COMMON    3 /* x3 */
#define IT_VCOMMON   4
#define IT_RARE      5
#define IT_VRARE     6
#define IT_GROUP     7
#define IT_BGROUP    8
#define IT_SPIKED    9
#define IT_STAPLE    10
#define IT_JUNK      11
#define IT_THROWABLE 12
#define IT_PLURAL    13
#define IT_MAGICAL   14
#define IT_ROPE      15
#define IT_ACTIVATE  16

#define WT_SUPER_SNEAK       17 /* d8's for sneak attacks */
#define WT_SUPER_TRIP        18 /* good for tripping */
#define WT_SUPER_DISARM      19 /* good for disarming */
#define WT_REACH             20 /* Reach Weapon; one square */
#define WT_STRIKE_NEAR       21 /* Reach Weapon; two squares */
#define WT_PENETRATING       22 /* Less Impact for real armor  */
#define WT_ENTANGLE          23 /* Can entangle a target  */
#define WT_SUBDUAL           24 /* Inflicts subdual damage */
#define WT_CHARGE            25 /* Double Damage on Change */
#define WT_SLASHING          26 /* Slashing weapon */
#define WT_PIERCING          27 /* Piercing weapon */
#define WT_BLUNT             28 /* Blunt weapon */
#define WT_DOUBLE            29 /* Double weapon; i.e. two-bladed sword */
#define WT_CROSSBOW          30 /* Requires reloading */
#define WT_PARRY             31 /* Grants +[spe] bonus to parry */
#define WT_EXOTIC_1H         32 /* Exotic to wield in one hand */
#define WT_TWO_HANDED        33
#define WT_FUMBLE1           34
#define WT_FUMBLE2           35
#define WT_STR1              36
#define WT_STR2              37
#define WT_STR3              38
#define WT_STUNNING          39
#define WT_RETURNING         40
#define WT_KNOCKDOWN         41
#define WT_WOOD_PARTS        42
#define WT_NO_STRENGTH       43 /* Strength grants no bonus (crossbow) */
#define WT_RACIAL            44
#define WT_DIGGER            45
#define WT_NO_MELEE          46 /* weapons like bolas cannot be used 
                                 * hand-to-hand */
#define WT_FIRE_DAMAGE       47 /* this should be WT_ELEMENTAL_DAMAGE
                                 * with the element stuffed somewhere, but
                                 * I'll save that for later */
#define WT_MOUNTED_ONLY      48 /* massive lances cannot be used unless
                                 * you are mounted or you are really big */
#define IT_LAST              49

#define FORT   0
#define REF    1
#define WILL   2
#define NOSAVE 3

/* Save Sub-types */
#define SA_MAGIC       0x00000001
#define SA_EVIL        0x00000002
#define SA_NECRO       0x00000004
#define SA_PARA        0x00000008
#define SA_DEATH       0x00000010
#define SA_ENCH        0x00000020
#define SA_PETRI       0x00000040
#define SA_POISON      0x00000080
#define SA_TRAPS       0x00000100
#define SA_GRAB        0x00000200
#define SA_FEAR        0x00000400
#define SA_ILLUS       0x00000800
#define SA_CONF        0x00001000
#define SA_DISEASE     0x00002000
#define SA_COMP        0x00004000
#define SA_SPELLS      0x00008000
#define SA_CLOSE       0x00010000
#define SA_THEFT       0x00020000
#define SA_DISINT      0x00040000
#define SA_KNOCKDOWN   0x00080000
#define SA_STUN        0x00100000
#define SA_REST        0x00200000
/* etc. */

/* And the Numbers */
#define SN_MAGIC       1
#define SN_EVIL        2
#define SN_NECRO       3
#define SN_PARA        4
#define SN_DEATH       5
#define SN_ENCH        6
#define SN_PETRI       7
#define SN_POISON      8
#define SN_TRAPS       9 
#define SN_GRAB        10
#define SN_FEAR        11
#define SN_ILLUS       12
#define SN_CONF        13
#define SN_DISEASE     14
#define SN_COMP        15
#define SN_SPELLS      16
#define SN_CLOSE       17
#define SN_THEFT       18
#define SN_DISINT      19
#define SN_KNOCKDOWN   20 /* ww: dwarven stability */
#define SN_STUN        21
#define SN_REST        22

#define AI_POTION     1 
#define AI_SCROLL     2 
#define AI_WAND       3 
#define AI_STAFF      4 
#define AI_ROD        5 
#define AI_ARMOR      6 
#define AI_GAUNTLETS  7 
#define AI_HELM       8 
#define AI_BOOTS      9 
#define AI_BRACERS    10 
#define AI_MUSH       11 
#define AI_HERB       12 
#define AI_RING       13 
#define AI_AMULET     14 
#define AI_SHIELD     15 
#define AI_ESSENCE    16 
#define AI_BOOK       17 
#define AI_SWORD      18 
#define AI_HAMMER     19 
#define AI_BOW        20 
#define AI_ARROW      21 
#define AI_BOLT       22 
#define AI_WEAPON     23 
#define AI_CROSSBOW   24
#define AI_DAGGER     25
#define AI_PSISKIN    26
#define AI_STONE      27
#define AI_CRYSTAL    28
#define AI_CLOAK      29
#define AI_PERIAPT    30 
#define AI_HORN       31
#define AI_PIPES      32
#define AI_GIRDLE     33
#define AI_HEADBAND   34
#define AI_TOME       35
#define AI_TRAP       36
#define AI_EYES       37
#define AI_WONDER     38
#define AI_ALCHEMY    39
#define AI_MACRO      40
#define AI_POISON     41 /* for AcquisitionPrompt ONLY! */
#define AI_FOUNTAIN   42
#define AI_SYMBOL     43

#define AI_WIZARDRY   50 
#define AI_SORCERY    51 
#define AI_THEURGY    52 
#define AI_DRUIDIC    53 
#define AI_BARDIC     54 
#define AI_WITCHCRAFT 55 
#define AI_PSIONIC    56 
#define AI_ANIMAL_NAME    57 
#define AI_TATTOO     58
#define AI_LAST       60

#define SLASH  0
#define PIERCE 1
#define BLUNT  2

#define AF_NOGEN      1
#define AF_SENTIENT   2
#define AF_BOOST_MANA 3
#define AF_CHAOTIC    4
#define AF_LAWFUL     5
#define AF_BALANCE    6
#define AF_GREATER    7
#define AF_AGGRESSIVE 8

#define AF_LAST       32

#define TF_SOLID      1
#define TF_OPAQUE     2
#define TF_OBSCURE    3
#define TF_SHADE      4
#define TF_SPECIAL    5
#define TF_WARN       6
#define TF_WATER      7
#define TF_TORCH      10
#define TF_EFFECT     11
#define TF_SHOWNAME   12
#define TF_NOGEN      13
#define TF_INTERIOR   14  /* Show as normal wall from outside */
#define TF_TREE       15
#define TF_DEEP_LIQ   16
#define TF_UNDERTOW   17
#define TF_FALL       18
#define TF_STICKY     19
#define TF_WALL       20
#define TF_BSOLID     21  /* Blocks rays/blasts */
#define TF_MSOLID     22
#define TF_CONCEAL    23  /* Conceals items within itself */
#define TF_VERDANT    24
#define TF_LOCAL_LIGHT 25 
/* the terrain itself is lit, but it doesn't have a radius like a torch */
#define TF_DEPOSIT    26
#define TF_PIT        27
#define TF_LAST       28

#define RF_CORRIDOR   1
#define RF_VAULT      2
#define RF_WARN       3
#define RF_ROOM       4
#define RF_NO_MON     5
#define RF_NO_TRAP    6
#define RF_NO_JUNK    7
#define RF_NO_TREA    8
#define RF_XTRA_MON   9
#define RF_XTRA_TRAP  10
#define RF_XTRA_JUNK  11
#define RF_XTRA_TREA  12
#define RF_TRAPMAZE   13     
#define RF_XTRA_CORP  14
#define RF_NOGEN      15
#define RF_STREAMER   16
#define RF_ROCKTYPE   17
#define RF_CHASM      18
#define RF_RIVER      19
#define RF_RAINBOW    20
#define RF_RAINBOW_W  21
#define RF_ALWAYS_LIT 22
#define RF_NEVER_LIT  23
#define RF_STAPLE     24
#define RF_ODD_WIDTH  25
#define RF_ODD_HEIGHT 26
#define RF_LAST       27

#define CF_RELIGIOUS  1
#define CF_LAWFUL     2
#define CF_GOOD       3
#define CF_CHAOTIC    4
#define CF_EVIL       5
#define CF_GOOD_FORT  6
#define CF_GOOD_REF   7
#define CF_GOOD_WILL  8
#define CF_PALADIN    9
#define CF_FAVORED    10 /* counts as a favored class (Prestige Classes) */
#define CF_PRESTIGE   11
#define CF_PSEUDO     12
#define CF_NONGOOD    13
#define CF_NONEVIL    14
#define CF_NONLAWFUL  15
#define CF_NONCHAOTIC 16
#define CF_LAST       17 /* Class/Race Flags */

#define DOF_LAST      1 /* Domain Flags */

#define GF_GOOD        1
#define GF_LAWFUL      2
#define GF_CHAOTIC     3
#define GF_EVIL        4
#define GF_NO_EVIL     6
#define GF_NO_GOOD     7
#define GF_NO_CHAOTIC  8
#define GF_NO_LAWFUL   9
#define GF_FORCE_TURN  11
#define GF_FORCE_COMMAND 12
#define GF_COMBAT_PULSE 13
#define GF_COMBAT_ANGER 14
#define GF_FORGIVING    15
#define GF_FEMALE      16
#define GF_NEUTER      17
#define GF_JEALOUS     18
#define GF_SANITY_BLASTING 19
#define GF_BLATANT_EVIL 20
#define GF_LAST       21 /* God Flags */
#define FF_LAST       1 /* Feature Flags */
#define MF_LAST       1 /* Map Flags */

#define L_HASFIELD    0x00000001
#define L_OPAQUE      0x00000002
#define L_FOGGY       0x00000004
#define L_ASOLID      0x00000008
#define L_BSOLID      0x00000010
#define L_SHADE       0x00000020
#define L_LIT         0x00000040
#define L_MLIGHT      0x00000080
#define L_SPECIAL     0x00000100
#define L_MDARK       0x00000200
#define L_SOLID       0x00000400
//#define L_

#define RM_ANY        -1
#define RM_NOROOM     0  /* Empty panel; no room */
#define RM_NORMAL     1  /* Single Small square */
#define RM_LARGE      2  /* (1d4 - panel edge) */
#define RM_CROSS      3  /* Two overlapping rectangles */
#define RM_OVERLAP    4  /* 2-4 overlapping squares */
#define RM_ADJACENT   5  /* Four squares with shared center */
#define RM_AD_ROUND   6  /* As ADJACENT, but with circles */
#define RM_AD_MIXED   7  /* As ADJACENT, but with either */
#define RM_CIRCLE     8  /* Just a big circle */
#define RM_OCTAGON    9  /* Roughly rounded room */
#define RM_DOUBLE     10 /* Room within a room */
#define RM_PILLARS    11 /* Room with pillars */
#define RM_CASTLE     12 /* "Castle" / subdivided big room */
#define RM_CHECKER    13 /* Checkerboard */
#define RM_BUILDING   14 /* Large, Subdivided */
#define RM_DESTROYED  15 /* Area collapsed; 10% rock, 30% rubble */
#define RM_GRID       16 /* Room filled with Furnishings[n] as even grid */
#define RM_LIFECAVE   17 /* Game-of-Life smooth caverns */
#define RM_RCAVERN    18 /* Repeated-L rough caverns */
#define RM_LCIRCLE    19 /* Large Circle */
#define RM_SHAPED     20 /* Special Shapes (i.e., resource room maps) */
#define RM_LIFELINK   21 /* Game of Life w. Linked Regions */
#define RM_RANDTOWN   22 /* Small, Random Town in Dungeon */
#define RM_MAZE       23 /* Random Maze fills most of panel */
#define RM_DIAMONDS   24 /* Grid of Diamond squares */
#define RM_LAST       32

#define RC_EMPTY      1
#define RC_SINGLE     2
#define RC_SINGLE_T   3
#define RC_EMPTY_T    4
#define RC_DIRTY      5
#define RC_MULTI      6
#define RC_PARTY      7
#define RC_TRIBE      8
#define RC_BUSY       9  /* 2d4+1 random monsters, 1d4 items */
#define RC_PIT        10 /* 30% chance of monster per 'inner' square */
#define RC_VAULT      11 /* 60% chance of monster per 'inner' square */
#define RC_LAST       13

#define TU_FIRST      24
#define TU_NORMAL     24
#define TU_DIAGONAL   25
#define TU_NATURAL    26
#define TU_BIG        27
#define TU_WARREN     28
#define TU_DRUNKARD   29
#define TU_REINFORCED 30

/* DF_xxxx == AD_xxxx ^ 2 */

#define DF_FIRE   0x00000001
#define DF_COLD   0x00000002
#define DF_ACID   0x00000004
#define DF_ELEC   0x00000008
#define DF_POIS   0x00000010
#define DF_NECR   0x00000020
#define DF_PSYC   0x00000040
#define DF_MAGC   0x00000080
#define DF_SUNL   0x00000100
#define DF_SONI   0x00000200
#define DF_SLASH  0x00000400
#define DF_PIERCE 0x00000800
#define DF_BLUNT  0x00001000
#define DF_STUK   0x00002000
#define DF_15     0x00004000
#define DF_SLEE   0x00008000
#define DF_STUN   0x00010000
#define DF_PLYS   0x00020000
#define DF_STON   0x00040000
#define DF_POLY   0x00080000
#define DF_MIND   0x00100000
#define DF_DISN   0x00200000
#define DF_CRIT   0x00400000
#define DF_BLND   0x00800000
#define DF_SLOW   0x01000000
#define DF_CONF   0x02000000
#define DF_DISE   0x04000000
#define DF_FEAR   0x08000000
#define DF_SUBD   0x10000000


#define AD_NORM    0    /* ordinary physical */
#define AD_FIRE    1    /* fire damage */
#define AD_COLD    2    /* frost damage */
#define AD_ACID    3    /* acid/corrosive damage */
#define AD_ELEC    4    /* electricity/lightning */
#define AD_TOXI    5    /* instant toxin (green dragon) */
#define AD_NECR    6    /* life drain */
#define AD_PSYC    7    /* psionic damage */
#define AD_MAGC    8    /* magic missiles, force bolts,... */

#define AD_SUNL    9    /* light effects */
#define AD_SONI    10   /* sound/disruption */
#define AD_SLASH   11  
#define AD_PIERCE  12 
#define AD_BLUNT   13
#define AD_STUK    14  /* entangles */
#define AD_LAST_PHYSICAL 13

#define AD_SLEE    16
#define AD_STUN    17  /* stuns */
#define AD_PLYS    18  /* paralysis */
#define AD_STON    19  /* petrifies (Medusa, Cockatrice) */
#define AD_POLY    20
#define AD_CHRM    21
#define AD_MIND    21
#define AD_DISN    22  /* disintigration */
#define AD_CRIT    23  /* Placeholder for Crit-immunity */

#define AD_BLND    24  /* blinds (glowing eye) */
#define AD_SLOW    25  /* slows */
#define AD_CONF    26  /* confuses (Umber Hulk) */
#define AD_DISE    27  /* confers diseases */
#define AD_FEAR    28  /* Causes Fear */
#define AD_SUBD    29  /* Subdual Damage */
#define AD_NAUS    30  /* nauseates */

#define AD_DREX    32  /* DF_NECR: drains life levels (Vampire) */
#define AD_DRMA    33  /* drains magic energy */
#define AD_LEGS    34  /* damages legs (xan) */
#define AD_STCK    35  /* sticks to you (Mimic) */
#define AD_STEA    36  /* steals equipment */
#define AD_SGLD    37  /* steals gold */
#define AD_TLPT    38  /* teleports you (Quantum Mech.) */
#define AD_RUST    39  /* rusts armour (Rust Monster)*/
#define AD_CURS    41  /* curse effects */
#define AD_WRAP    42  /* tentacle grab; harder to resist */
#define AD_DRST    43  /* drains strength */
#define AD_DRDX    44  /* drains dexterity (Quasit) */
#define AD_DRCO    45  /* drains constitution */
#define AD_DRIN    46  /* drains intelligence (mind flayer) */
#define AD_DRWI    47  /* drains wisdom */
#define AD_DRCH    48  /* drains charisma */
#define AD_DRLU    49  /* drains luck */
#define AD_DAST    50  /* damages strength */
#define AD_DADX    51  /* damages dexterity (Quasit) */
#define AD_DACO    52  /* damages constitution */
#define AD_DAIN    53  /* damages intelligence (mind flayer) */
#define AD_DAWI    54  /* damages wisdom */
#define AD_DACH    55  /* damages charisma */
#define AD_DALU    56  /* damages luck */
#define AD_DCAY    57  /* decays organic equipment (Brown pudding) */
#define AD_WERE    58  /* confers lycanthropy */
#define AD_BLEE    59  /* Target bleeds for [Dmg] turns */
#define AD_POIS    60  /* true poison (various oozes) */
#define AD_KNOC    61  /* Knockback */
#define AD_CHOK    62  /* Choking */
#define AD_GRAB    63  /* Grab Attack */
#define AD_ANTI    64  /* Antimagic ray (beholders/dreadnaughts) */
#define AD_ERAY    65  /* random gaze effects for beholders */
#define AD_ENGL    66  /* Engulf / Swallow */
#define AD_TRIP    67  /* Trip */
#define AD_DISA    68  /* Disarm */
#define AD_HOLY    69  /* Good Aligned Damage (holy smite) */
#define AD_LAWF    70  /* Lawful Aligned Damage (order's wrath) */
#define AD_CHAO    71  /* Chaos Aligned Damage (chaos hammer) */
#define AD_EVIL    72  /* Evil Aligned Damage (unholy blight) */
#define AD_VAMP    73  /* Vampiric damage -- heals attacker (vamp. touch) */
#define AD_SOAK    74  /* Water Elementals -- cause rust, wreck scrolls, etc. */
#define AD_SPE1    90
#define AD_SPE2    91
#define AD_SHAT    92  /* shatter equipment */
#define AD_SWAL    93
#define AD_FALL    94

#define is_wepdmg(a) (a == AD_SLASH || a == AD_PIERCE || a == AD_BLUNT)
 

/* The monster attack codes: there are five kinds of attack
   codes listed here. The first set are the Standard attacks
   of monsters. A monster can, as a single action at its
   Spd[S_MELEE], perform all the _standard_ attacks in its
   Attk array as a single action. Some standard attacks will
   have a chance in their Attk entry, meaning there is only
   that percent chance that the monster will use that attack
   with its standard melee sequence. The following are examples
   of Standard attacks:

     A_BITE, A_CLAW, A_TUCH, A_BUTT, A_STNG, etc.

     The second type of attack codes are the _special_ attacks.
   A monster must spend one of its melee actions to use a single
   special attack. Some special attacks are used automatically
   by the game, and don't end up taking actions to use.
   Examples of special attacks:

     A_BREA, A_GAZE, A_EXPL, A_DGST, A_CRUS, A_PASS.

     The third type are special maneuvers that the player (and
   any monsters with the M_HUMANOID flag) can perform in combat.
   Weapon combat attack types fall here.
   They include:

     A_BULL, A_COUP, A_THRU, A_SWNG, etc.

     The fourth type are special, _non-attack_ abilities of a
   monster that need some data space to describe well, and so
   have a place in the monster's Attk array. Examples of these
   include:

     A_CLER, A_MAGE, A_SCHL, etc.

     The fifth type of attack record is the A_ALSO record, which
   appends a second attack effect to a given attack. Thus, a
   spider that hits to inflict 3d4 points of damage and drain
   1d4 points of Con would have this single attack described by
   two Attk[] array entries, as follows:

     AType    DType    Dmg
     A_BITE   AD_NORM  3d4
     A_ALSO   AD_DRCO  1d4

*/



#define A_PUNC    1   /* Punch                   */
#define A_KICK    2   /* Kick                     */
#define A_BASH    4   /* Bash (Bludgeon)          */
#define A_GRAB    5   /* Grab                    */
#define A_CLAW    6   /* claw (punch, hit, etc.) */
#define A_BITE    7   /* bite                    */
#define A_BUTT    8   /* head butt (ex. a unicorn) */
#define A_TUCH    9   /* touches */
#define A_STNG    10  /* sting */
#define A_HUGS    11  /* crushing bearhug */
#define A_RUBS    12  /* rubs against */
#define A_SLOB    13  /* drools on */
#define A_TENT    14  /* tentacles */
#define A_PROB    15  /* "The ghoul-stirge's probiscis pinctures you... */
#define A_SWAT    16  /* "The behemoth swats at you, hitting directly!" */
#define A_OOZE    17  /* "The grey ooze oozes over you, hitting */
#define A_SLAM    19
#define A_STOM    20  /* Stomp */
#define A_WRAP    21  /* (A_GRAB) "The boa constrictor wraps around you!" */
#define A_SPOR    22  /* Fire Spores */
#define A_GORE    23  /* boars, et al. */
#define A_TAIL    24  /* tail sweep */
#define A_FALL    25  /* fall from above (blackmantle, piercer, etc.) */
#define A_TRAM    26
#define A_RAKE    27
#define A_LAST_STANDARD 29  /* last true attack */
#define is_standard_attk(a) ((a) > 0 && (a) <= A_LAST_STANDARD)

#define A_CRUS    30  /* crush, after grab */
#define A_SUCK    31  /* (A_CRUS) Suck Blood (vampire, dire mosquito, lamprey, etc. */
#define A_CONS    32  /* (A_CRUS) Constrict, after grab */
#define A_REND    33  /* rend */
#define A_IMPA    34  /* Impales you on its spikes */
#define A_THRO    35  /* Throws you */
#define A_CURB    36  /* Curb stomp ("X slams you against the concrete wall/floor!") */
#define A_DGST    37  /* digests opponent (trapper, etc.) */

#define A_LAST_POSTGRAB 39
#define is_postgrab_attk(a)  ((a) > A_LAST_STANDARD && (a) <= A_LAST_POSTGRAB)

#define A_SPIT    40  /* spits substance - ranged */
#define A_HURL    41  /* Throw object            */
#define A_BREA    42  /* breath - ranged */
#define A_GAZE    43  /* gaze - ranged */
#define A_XXXX    44
#define A_EXPL    45  /* explodes - proximity */
#define A_SPIK    46  /* $S's spikes pierce you! (passive, only vs. unarmed/grapple */
#define A_ROAR    47  
#define A_BRE2    48  
#define A_DROP    49  /* drops from above to engulf */
#define A_CPRX    50  /* Controlled Proximity */
#define A_GEST    51  /* Gestures at (spell-like attack) */
#define A_DETH    52  /* Death Throes (occurs when mon dies) */
#define A_PROX    53  /* Proximity Effect */
#define A_LAST_SPECIAL 53
#define is_special_attk(a) ((a) > A_LAST_POSTGRAB && (a) <= A_LAST_SPECIAL)

#define A_AURA    54  /* damage on contact */
#define A_PASS    55  /* UNUSED: response to melee attack */
#define A_XTRA    56  /* UNUSED: add this effect to all melee attacks */
#define A_RSPO    57  /* UNUSED: spores fired in response */
#define A_DEXP    58  /* UNUSED: Death Throes (explodes when dies) */
#define A_DPIT    59  /* UNUSED: Death Throes (sucking black pit forms) */
#define A_DEQU    60  
  /* ww: A_DEQU -- hit Item2 first, if not, then Item, if not, then
   * apply to attacker */
#define A_LAST_RESPONSE 60
#define is_response_attk(a) ((a) > A_LAST_SPECIAL && (a) <= A_LAST_RESPONSE)

#define A_BULL    61
#define A_COUP    62
#define A_SWNG    63  /* Swing                    */
#define A_THRU    64  /* Thrust                  */
#define A_FIRE    65  /* Fire [Cross]bow          */
#define A_LUNG    66  /* Lunge                    */
#define A_GREA    67  /* Great Blow              */
#define A_CALL    68  /* Called Shot */
#define A_SPRT    69  /* Sprint */
#define A_RIPO    70  /* Riposte */
#define A_CHAR    71  /* Charge */
#define A_AIDA    72  /* Aid Another */
#define A_DISA    73  /* Disarm */
#define A_TRIP    74
#define A_SUND    75
#define A_SPRI    76  /* Spring Attack */
#define A_WHIR    77  /* Whirlwind Attack */
#define A_ESCA    78  /* Escape Grapple */
#define A_DEFN    79  /* Fight Defensively */
#define A_AEYE    80  /* Avert Eyes */
#define A_PREC    81  /* Precision Strike */
#define A_LAST_MANEUVER 81
#define is_maneuver(a) ((a) > A_LAST_RESPONSE && (a) <= A_LAST_MANEUVER)

#define A_MOVE    90  
#define A_CLER    91  /* Casts Divine Spells, max spell lev = DType */
#define A_PRIM    92  /* Casts Primal Spells, max spell lev = DType */
#define A_CAST    93  /* Casts Any Spells, max spell lev = DType */
#define A_INAB    94  /* Uses list of innate spells; listed in annot. */
#define A_PSIO    95  /* Psionicist -- DType = favoured Discipline */
#define A_CONT    96  /* With AD_EFF1/2, effect constantly active */
#define A_CLA1    97  /* Has the powers of a class! DType == level,
                           eAttk[1] holds the class! */
#define A_CLA2    98  /* As above, class in eAttk[2]. */
#define A_GATE    99  /* gates in others of same MA_TYPE; dmg/5 = CR */
#define A_SUMM   100  /* summon random monsters, MA_TYPE == DType */
#define A_FORM   101  /* Shapechanger, with AD_EFF1/2 */
#define A_SEEM   102
#define A_LAST_MABILITY 102

#define is_ability_attk(a) ((a) > A_LAST_MANEUVER && (a) <= A_LAST_MABILITY)

#define A_ALSO    110  /* Bite for 3d6 AD_NORM and 1d4 AD_DRDX */
#define A_CRIT    111  /* Bite for 3d6 AD_NORM, and AD_ENGL on critical */
#define A_SPEL    112  /* Ranged Attack Roll for spells */
#define A_SPE1    113
#define A_SPE2    114

#define ATTK_FIRST 1
#define ATTK_NEXT  2
#define ATTK_PEEK  3

#define S_ARCHERY 0
#define S_BRAWL 1
#define S_MELEE 2
#define S_THROWN 3
#define S_DUAL  4


#define QUEUE_WEAPON_MSG   1
#define QUEUE_DAMAGE_MSG   2
#define QUEUE_AREA_EFFECT  3
#define QUEUE_RESIST_MSG   4
#define QUEUE_DISPEL_MSG   5
#define QUEUE_BLAST_MSG    6
#define QUEUE_DISBELIEF_MSG 7

#define TS_FOUND    0x0001
#define TS_DISARMED 0x0002
#define TS_NODISARM 0x0004
#define TS_SEARCHED 0x0008
#define TS_NORESET  0x0010

#define MS_PEACEFUL    0x0001 /* No hostility toward anyone who didn't attack first */
#define MS_GUARDING    0x0002
#define MS_FEMALE      0x0004
#define MS_KNOWN       0x0008
#define MS_INVEN_GOOD  0x0010
#define MS_CURSED      0x0020
#define MS_BOW_ACTIVE  0x0040
#define MS_SCRUTINIZED 0x0100
#define MS_POLY_KNOWN  0x0200
#define MS_SEEN_CLOSE  0x0400
#define MS_CASTING     0x0800
#define MS_HAS_REACH   0x1000
#define MS_REACH_ONLY  0x2000
#define MS_THREAT2     0x4000
#define MS_STILL_CAST  0x8000 

#define M_HUMANOID        1   
#define M_NOLIMBS         2   
#define M_NOHANDS         3   
#define M_NOHEAD          4   
#define M_NOBODY          5   
#define M_BLIND           6   
#define M_DEAF            7   
#define M_LIGHT_AVERSE    8
#define M_INCOR           9   
#define M_AMORPH          10  
#define M_MOUNTABLE       11  
#define M_FLYER           12  
#define M_MFLYER          13  
#define M_REACH           14  
#define M_CHARGE          15    /* ww: likes to charge, even w/o weapons */
#define M_NO_CHARGE       16    /* ww: will not charge */
#define M_BREEDER         17  
#define M_HAS_BLOOD       18    /* ww: override the default */
#define M_NO_BLOOD        19    /* ww: override the default */
#define M_NO_BREATH       20    /* ww: override MA_BREATHER */
#define M_NO_SIZE_HP      21    /* ww: no HP change from size (swarms) */ 
#define M_NO_CARRY_LOADS  22    /* cannot carry loads */
#define M_NOPOLY          23  
#define M_NOGEN           24  
#define M_NOGENO          25  
#define M_LAYS_EGGS       26  
#define M_THICK_HIDE      27  
#define M_POISON          28  
#define M_INVIS           29  
#define M_AMPHIB          30  
#define M_AQUATIC         31  
#define M_UNDEAD          32  
#define M_MINDLESS        33  
#define M_ANIMAL          34  
#define M_NATURAL         35  
#define M_CARNI           36  
#define M_HERBI           37  
#define M_WATER           38  
#define M_AIR             39  
#define M_EARTH           40  
#define M_FIRE            41  
#define M_COLD            42  
#define M_ACID            43  
#define M_MAGC            44  
#define M_ELEC            45  
#define M_TOXI            46  
#define M_SUNL            47  
#define M_SONI            48  
#define M_EVIL            49  
#define M_GOOD            50  
#define M_LAWFUL          51  
#define M_CHAOTIC         52  
#define M_IALIGN          53   /* Inherant Alignment; always as listed */
#define M_FRIENDLY        54
#define M_HOSTILE         55
#define M_PSYCHO          56  
#define M_SKIRMISH        57  
#define M_HIDE_UNDER      58  
#define M_HIDE_ABOVE      59  
#define M_EATSMETAL       60  
#define M_SEE_INVIS       61  
#define M_LORD            62  
#define M_KING            63  
#define M_ALL_MALE        64  
#define M_ALL_FEMALE      65  
#define M_NEUTER          66  
#define M_UNIQUE          67  
#define M_COMMON          68  
#define M_RARE            69  
#define M_VERY_RARE       70  
#define M_UNCOMMON        71  
#define M_DESERT          72  
#define M_SWAMP           73  
#define M_FOREST          74  
#define M_PLAINS          75  
#define M_CITY            76  
#define M_MINION          77  
#define M_WANDER          78  
#define M_GREEDY          79  
#define M_GEMS            80  
#define M_HOARD           81  
#define M_HOARD2          82  
#define M_PROPER          83  
#define M_DOMESTIC        84  
#define M_COVETOUS        85   
#define M_TALKABLE        86  
#define M_FIXED_HP        87  
#define M_NO_CORPSE       88  
#define M_OLD_CORPSE      89  
#define M_AVOID_MELEE     90  
#define M_INNATE          91  
#define M_SOLITARY        92  
#define M_CHIEFTAN        93  
#define M_SHAMAN          94  
#define M_VARGROUP        95  
#define M_BURROW          96   /* for Burrowfriend */
#define M_CASTER          97  
#define M_MOUNTAIN        98  
#define M_ARCTIC          99  
#define M_OCEAN           100 
#define M_XTRA_TOUCH      101 
#define M_EXCLUSIVE       102 
#define M_IGNORE_ITEMS    103  /* ww: do not chase after ground items */
#define M_GROUP           104 
#define M_SWARM           105 
#define M_SELLER          106
#define M_CLOSEID         107
#define M_UNKNOWN         108
#define M_ASTRAL          109
#define M_PLAYER          110
#define M_MAGE            111
#define M_LAST            112  

#define TT_SURFACE    0x0001 /* Never in a dungeon */
#define TT_FOREST     0x0002
#define TT_MOUNTAIN   0x0004
#define TT_ARCTIC     0x0008
#define TT_SWAMP      0x0010
#define TT_DESERT     0x0020
#define TT_OCEAN      0x0040
#define TT_TROPICAL   0x0080
#define TT_PLAINS     0x0100
#define TT_BADLANDS   0x0200
#define TT_JUNGLE     0x0400
#define TT_HILLS      0x0800
#define TT_COASTAL    0x1000
#define TT_URBAN      0x2000
#define TT_ASTRAL     0x4000
#define TT_ETHEREAL   0x8000

#define PA_AMBITIOUS_CONQUERER         0
#define PA_CHARMING_SCOUNDREL          1
#define PA_CRAZED_PSYCHOPATH           2
#define PA_DISCIPLINED_EXPERT          3
#define PA_DRIVEN_BROODER              4
#define PA_EMBITTERED_CYNIC            5
#define PA_EMPATHIC_NURTURER           6
#define PA_ETERNAL_ROMANTIC            7
#define PA_GENTLEMAN_ADVENTURER        8
#define PA_HONORBOUND_SERVANT          9
#define PA_ICONOCLASTIC_REBEL          10
#define PA_INTROSPECTIVE_VISIONARY     11
#define PA_MANIPULATIVE_PLOTTER        12
#define PA_RIGHTEOUS_ZEALOT            13
#define PA_WHITE_KNIGHT                14
#define PA_WITTY_THRILLSEEKER          15

#define MR_COMMON       1
#define MR_UNCOMMON     2
#define MR_RARE         3
#define MR_VRARE        4
#define MR_XRARE        5

#define TMF_BODY_ONLY  -1
#define TMF_OBVIOUS    -2
#define TMF_POSTSCRIPT -3
#define TMF_NOGEN      -4
#define TMF_NAMEONLY   -5
#define TMF_LAST       -6

#define SZ_MINISCULE  1
#define SZ_TINY       2
#define SZ_SMALL      3
#define SZ_MEDIUM     4
#define SZ_LARGE      5
#define SZ_HUGE       6
#define SZ_GARGANTUAN 7
#define SZ_COLLOSAL   8

#define PER_VISUAL  0x0001
#define PER_INFRA   0x0002
#define PER_SCENT   0x0004
#define PER_BLIND   0x0008
#define PER_PERCEPT 0x0010
#define PER_TREMOR  0x0020
#define PER_SHADOW  0x0040
#define PER_TELE    0x0080
#define PER_DETECT  0x0100
#define PER_TRACK   0x0200
#define PER_SHARED  0x0400
#define PER_LAST    PER_SHARED

#define EXXX_SKILL   10
#define EXXX_SAVE    11
#define EXXX_PSKILL  12  /* Skill-based, no bleedoff */

#define ESTR_DOOR    1
#define ESTR_CRIT    2
#define ESTR_GRAPPLE 3
#define ESTR_FORGE   4
#define ESTR_MINING  5
#define ESTR_UNSTUCK 6
#define ESTR_BURDEN  7   /* Encumbrance Pulse */
#define ESTR_ARMOR   8

#define EDEX_TUMBLE    1
#define EDEX_CRIT      2
#define EDEX_THROWN    3
#define EDEX_DEFENSIVE 4
#define EDEX_EVASION   5
#define EDEX_JUMP      6

#define ECON_OVERCOME  1
#define ECON_CRIT      2 /* surviving enemy crit */
#define ECON_FOOD      3 /* specially healthy food */
#define ECON_HUNGER    4 /* hunger pulse */
#define ECON_ARMOR     5 /* heavy armor pulse */
#define ECON_FATIGUE   6 /* spending fatigue up to 0 */
#define ECON_ESSIAH    7

#define EINT_CASTING   1
#define EINT_IDENT     2
#define EINT_CREATION  3
#define EINT_IDMON     4
#define EINT_XAVIAS    5
#define EINT_SABINE    5

#define EWIS_TURNING   1
#define EWIS_DONATE    2
#define EWIS_PERSUADE  3
#define EWIS_PRAYER    4
#define EWIS_CONVERT   5
#define EWIS_CREATION  6
#define EWIS_XAVIAS    7

#define ECHA_FEAR      1
#define ECHA_CHARM     2
#define ECHA_TURNING   3
#define ECHA_KILLFEAR  4
#define ECHA_SUMMON    5
#define ECHA_THRONE    6
#define ECHA_ESSIAH    7

#define ELUC_RISK      1
#define ELUC_ITEM      2
#define ELUC_FATIGUE   3
#define ELUC_CRIT      4
#define ELUC_FOUNTAIN  5
#define ELUC_MAEVE     6


#define MA_ALL          0
#define MA_ANIMAL       2  /* 'a' */
#define MA_BAT          3  /* 'b' */
#define MA_BIRD         4  /* 'b' */
#define MA_CAT          5  /* 'c' */
#define MA_DOG          6  /* 'd' */
#define MA_EYE          7  /* 'e' */
#define MA_FAERIE       9  /* 'f' */
#define MA_GOBLIN       10 /* 'g' */
#define MA_HUMANOID     11 /* 'h' */
#define MA_IMPKIN       15 /* 'i' */
#define MA_JELLY        16 /* 'j' */
#define MA_OOZE         17 /* 'j' */
#define MA_KOBOLD       18 /* 'k' */
#define MA_LYCANTHROPE  19 /* 'l' */
#define MA_MIMIC        20 /* 'm' */
#define MA_NAGA         21 /* 'n' */
#define MA_ORC          22 /* 'o' */
#define MA_PUDDING      23 /* 'p' */
#define MA_QUADRUPED    24 /* 'q' */
#define MA_RODENT       25 /* 'r' */
#define MA_SPIDER       26 /* 's' */
#define MA_TRAPPER      27 /* 't' */
#define MA_HAG          28 /* 'u' */
#define MA_VORTEX       29 /* 'v' */
#define MA_WORM         30 /* 'w' */
#define MA_VERMIN       31 /* 'x' */
#define MA_YUAN_TI      32 /* 'y' */
#define MA_LIGHT        33 /* 'z' */
#define MA_CELESTIAL    34 /* 'A' */
#define MA_BEAST        35 /* 'B' */
#define MA_CONSTRUCT    36 /* 'C' */
#define MA_DRAGON       37 /* 'D' */
#define MA_ELEMENTAL    38 /* 'E' */
#define MA_FUNGI        39 /* 'F' */
#define MA_GOLEM        40 /* 'G' */
#define MA_GIANT        41 /* 'H' */
#define MA_ILLITHID     42 /* 'I' */
#define MA_GENIE        43 /* 'J' */
#define MA_ELEMENTALKIN 44 /* 'K' */
#define MA_LICH         45 /* 'L' */
#define MA_MYTHIC       46 /* 'M' */
#define MA_NIGHTSHADE   47 /* 'N' */
#define MA_OUTSIDER     48 /* 'O' */
#define MA_PLANT        49 /* 'P' */
#define MA_QUYLTHULG    50 /* 'Q' */
#define MA_REPTILE      51 /* 'R' */
#define MA_SNAKE        52 /* 'R' */
#define MA_SPIRIT       53 /* 'S' */
#define MA_TROLL        54 /* 'T' */
#define MA_REVENANT     55 /* 'U' */
#define MA_VAMPIRE      56 /* 'V' */
#define MA_WRAITH       57 /* 'W' */
#define MA_ABERRATION   58 /* 'X' */
#define MA_HOMINID      59 /* 'Y' */
#define MA_ZOMBIE       60 /* 'Z' */
#define MA_HUMAN        61
#define MA_ELF          62
#define MA_DWARF        63
#define MA_GNOME        64
#define MA_HALFLING     65
#define MA_DROW         66
#define MA_LIZARDFOLK   67
#define MA_DINOSAUR     68  
#define MA_DEMIHUMAN    69
#define MA_SWARM        70  
#define MA_PLANETOUCHED 71
#define MA_DEMON        72
#define MA_DEVIL        73

#define MA_LAST_REAL    74

#define MA_LIVING       75 /* !construct, !vortex, !elemental, !elementalkin */
#define MA_UNDEAD       76
#define MA_HAS_BLOOD    77
#define MA_BREATHER     78      /* living, !pudding, !jelly, !mimic, !fungi */
#define MA_BURROW       79      
#define MA_AQUATIC      80      /* m_aquatic */
#define MA_SYLVAN       81          /* m_natural, elf, faerie, animal */
#define MA_AVIAN        82          /* m_flyer */
#define MA_REPTILIAN    83        /* reptile, dragon, kobold */
#define MA_GOBLINOID    84      /* goblin, orc, kobold */
#define MA_SAPIENT      85      /* !mindless, !animal, int >= 6 */
#define MA_PERSON       86      /* !fey, humanoid, !big, !undead, !mindless */
#define MA_ADVENTURER   87
#define MA_MOUNT        88      /* m_mountable */
#define MA_NATURAL      89          /* m_natural */
#define MA_NATIVE       90        /* ! outsider */
#define MA_NLIVING      91        /* ! outsider && living */
#define MA_WATER        92
#define MA_AIR          93
#define MA_EARTH        94
#define MA_FIRE         95
#define MA_COLD         96
#define MA_ACID         97
#define MA_MAGC         98      /* pure magical */
#define MA_ELEC         99
#define MA_TOXI         100
#define MA_SUNL         101
#define MA_SONI         102
#define MA_EVIL         103
#define MA_LAWFUL       104
#define MA_CHAOTIC      105
#define MA_GOOD         106
#define MA_NEUTRAL      107
#define MA_SUMMONED     108  /* stati SUMMONED */
#define MA_ALLIES       109
#define MA_HOSTILES     110
#define MA_ENEMIES      111
#define MA_SKELETON     112
#define MA_NONEVIL      113
#define MA_NONGOOD      114
#define MA_NONLAWFUL    115
#define MA_NONCHAOTIC   116
#define MA_LOUTSIDER    117
#define MA_COUTSIDER    118
#define MA_HOLY_SUMM    119
#define MA_FORCE_D12    120
#define MA_FORCE_D10    121
#define MA_FORCE_D8     122
#define MA_FORCE_D6     123
#define MA_FORCE_D4     124
#define MA_CHOICE1      125
#define MA_CHOICE2      126
#define MA_CHOICE3      127
#define MA_CHOICE4      128
#define MA_CHOICE5      129
#define MA_LAST         130



#define MN_ATTK_SEEN(n) (0  + (n*4)   + 0xFFFF*1)
#define MN_ATTK_FELT(n) (0  + (n*4)+1 + 0xFFFF*1)
#define MN_RESIST(n)    (48 + n +       0xFFFF*1)
#define MN_IMMUNE(n)    (80 + n +       0xFFFF*1)
#define MN_SPECABIL(n)  (96 + n +       0xFFFF*1)
#define MN_SEEN         (97 +           0xFFFF*1)
#define MN_HIT          (98 +           0xFFFF*1)
#define MN_KILL_COUNT   (100 +          0xFFFF*24)
#define MN_DEATH_COUNT  (124 +          0xFFFF*24)



#define MN_LAST_BIT     148

/* GetMonID Cause Constants */
#define PUR_DUNGEON  1
#define PUR_SUMMON   2
#define PUR_POLY     3
#define PUR_ILLUSION 4
#define PUR_TOWNFOLK 5
#define PUR_ACQUIRE  6
#define PUR_JUNK     7
#define PUR_EQUIP    8
#define PUR_WIZARD   9
#define PUR_GENOCIDE 10
#define PUR_POISON   11
#define PUR_DISEASE  12
#define PUR_STAPLE   13
#define PUR_CURSED   14
#define PUR_ANYMON   15
#define PUR_MUNDANE  16
#define PUR_KNOWN    17
#define PUR_MINOR    18
#define PUR_MAJOR    19
#define PUR_CRAFT    20
#define PUR_LISTING  100

#define IG_STAPLE   0x0001
#define IG_GOOD     0x0002
#define IG_GREAT    0x0004
#define IG_WARRIOR  0x0008
#define IG_WIZARD   0x0010
#define IG_ROGUE    0x0020
#define IG_CURSED   0x0040
#define IG_CHEST    0x0080
#define IG_MONEQUIP 0x0100
#define IG_KNOWN    0x0200


#define EN_ROOM     0x00000001 /* Place monsters in last created room */
#define EN_STREAMER 0x00000002 /* Place monsters in (water) streamer */
#define EN_SINGLE   0x00000004 /* Generate only one single monster */
#define EN_FREAKY   0x00000008 /* Ignore normal template restrictions */
#define EN_MAXIMIZE 0x00000010 /* *Must* be stated CR or a little above */
#define EN_DUNGEON  0x00000020 /* Create potential uniques, etc. */
#define EN_MTYPE    0x00000040 /* All monsters of given MType */
#define EN_MONID    0x00000080 /* Specific mID */
#define EN_NOPLACE  0x00000100 /* Place stuff in Candidates, not the map */
#define EN_DEPTH    0x00000200 /* Check map depth vs. monster depth */
#define EN_SUMMON   0x00000400 /* Summoned monsters */
#define EN_ANYOPEN  0x00000800 /* Place according to OpenX / OpenY */
#define EN_MULTIPLE 0x00001000 /* Generate at least 12 monsters */
#define EN_NOBUILD  0x00002000 /* Don't even *create* the monsters */
#define EN_CLUSTER  0x00004000 /* Create all mons within 5 squares of each other */
#define EN_CREATOR  0x00008000 /* Use the creator's party */
#define EN_ILLUSION 0x00010000 /* Create illusionary creatures */
#define EN_SPECIFIC 0x00020000 /* Generate specific creature, not by CR */
#define EN_HOSTILE  0x00040000 /* Create creatures hostile to summoner */

#define TM_SKILL    0x0001 /* skilled kobold, veteran mind flayer, etc. */
#define TM_CLASS    0x0002 /* orc barbarian, firenewt sorceror, etc. */
#define TM_AGECAT   0x0004 /* juvenile red dragon, great wyrm silver, eldritch wyrm crystal */
#define TM_NATURE   0x0008 /* axiomatic bulette, corrupted snake, dire bear */
#define TM_UNDEAD   0x0010 /* dwarf skeleton, kobold mummy, zombie otyugh */ 
#define TM_CHIEFTAN 0x0020 /* "chieftan"/superior specimen/leader monster to an one of to tribes */
#define TM_SHAMAN   0x0040 /* "shaman"/magical support monster to add one of to tribes */
#define TM_ATTR     0x0080 /* Exceptional Attribute templates */
#define TM_DESCENT  0x0100 /* Half-Dragon, Half-Fiend, etc. */
#define TM_PLANAR   0x0200 /* Aqueous, Flame, Celestial, Anarchic */

#define AL_LAWFUL     0x0001
#define AL_CHAOTIC    0x0002
#define AL_GOOD       0x0004
#define AL_EVIL       0x0008
#define AL_NONLAWFUL  0x0010
#define AL_NONCHAOTIC 0x0020
#define AL_NONGOOD    0x0040
#define AL_NONEVIL    0x0080

#define A_STR 0
#define A_DEX 1
#define A_CON 2
#define A_INT 3
#define A_WIS 4
#define A_CHA 5
#define A_LUC 6   /* Players have "luck", others have "loyalty" */
#define A_LOY 6 

#define A_SPD_ARCHERY 7    /* Archery Speed */
#define A_SPD_BRAWL   8    /* Brawl Speed */
#define A_SPD_MELEE   9    /* Melee Speed */
#define A_SPD_THROWN  10   /* Throwing Speed */
#define A_SPD_OFFHAND 11   /* Offhand Weapon Speed */
#define A_HIT_ARCHERY 12   /* Archery +toHit */
#define A_HIT_BRAWL   13   /* Brawl +toHit */
#define A_HIT_MELEE   14   /* Melee +toHit */
#define A_HIT_THROWN  15   /* Thrown +toHit */
#define A_HIT_OFFHAND 16   /* Offhand +toHit */
#define A_ARM         17   /* Bonuses to overall Armor */
#define A_MR          18   /* Magic Resistance */
#define A_XXX         19   /* Unused */
#define A_DMG_ARCHERY 20   /* Archery +toDmg */
#define A_DMG_BRAWL   21   /* Brawl +toDmg */
#define A_DMG_MELEE   22   /* Melee +toDmg */
#define A_DMG_THROWN  23   /* Thrown +toDmg */
#define A_DMG_OFFHAND 24   /* Offhand +toDmg */
#define A_SIZ         25   /* Size Catagory */
#define A_MOV         26   /* Movement Rate */
#define A_DEF         27   /* Defense Class */
#define A_SAV_FORT    28   /* Fortitude Save Bonus */
#define A_SAV_REF     29   /* Reflex Save Bonus */
#define A_SAV_WILL    30   /* Will Save Bonus */
#define A_ARC         31   /* Arcane Casting Bonus */
#define A_DIV         32   /* Divine Casting Bonus */
#define A_PRI         33   /* Primal Casting Bonus */
#define A_BAR         34   /* Bardic Casting Bonus */
#define A_SOR         35   /* Sorcery Casting Bonus */
#define A_THP         36   /* Temporary Hit Points */
#define A_MAN         37   /* Temporary Mana Adjust */
#define A_FAT         38   /* Maximum Fatigue Points */
#define A_COV         39   /* Armor Coverage Level */
#define A_CDEF        40
#define ATTR_LAST     41

#define A_HIT 50 /* Universal +toHit */
#define A_AID 51 /* Add to A_HIT, A_DEF, A_SAV & A_MAG */
#define A_SAV 52 /* Att to fort, ref, will */
#define A_DMG 53 /* Universal +toDmg */
#define A_SPD 54 /* Universal Speed Modifier */
#define A_MAG 55 /* Add to both A_ARC and A_DIV */

#define percent_attr(i) (i == A_MOV || i == A_SPD_MELEE ||             \
   i == A_SPD_ARCHERY || i == A_SPD_THROWN || i == A_SPD_BRAWL ||      \
   i == A_SPD_OFFHAND || i == A_SPD)
#define bonus_is_mult(i,j) (j == BONUS_WEAPON || j == BONUS_ENHANCE || \
                            j == BONUS_SACRED || j == BONUS_WEAPON ||  \
                            j == BONUS_SKILL || j == BONUS_COMP ||     \
                            j == BONUS_FEAT || AttrAdj[i][j] < 0)


#define R_HUMAN      1
#define R_ELF        2
#define R_DWARF      3
#define R_GNOME      4
#define R_HOBBIT     5
#define R_ORC        6
#define R_TROLL      7
#define R_GOBLIN     8


#define EN_NONE      0
#define EN_LIGHT     1
#define EN_MODERATE  2
#define EN_HEAVY     3
#define EN_EXTREME   4

#define SC_ABJ       0x00000001
#define SC_ARC       0x00000002
#define SC_DIV       0x00000004
#define SC_ENC       0x00000008
#define SC_EVO       0x00000010
#define SC_ILL       0x00000020
#define SC_NEC       0x00000040
#define SC_THA       0x00000080
#define SC_WEA       0x00000100
#define SC_THE       0x00000200
#define SC_NAT       0x00000400
#define SC_COLD      0x00000800
#define SC_AIR       0x00001000
#define SC_EARTH     0x00002000
#define SC_FIRE      0x00004000
#define SC_WATER     0x00008000
#define SC_FORCE     0x00010000
#define SC_LIGHT     0x00020000
#define SC_DARKNESS  0x00040000

/* Spell Knowledge Flags */
#define SP_KNOWN     0x0001
#define SP_MASTERED  0x0002
#define SP_ARCANE    0x0004
#define SP_SORCERY   0x0000
#define SP_DIVINE    0x0008
#define SP_BARDIC    0x0010
#define SP_PRIMAL    0x0020
#define SP_STAFF     0x0040
#define SP_INNATE    0x0080
#define SP_DOMAIN    0x0100
#define SP_FORGOTTEN 0x0200
#define SP_CREATE    0x0400
#define SP_TINNATE   0x0800

#define SK_ALCHEMY    1
#define SK_ANIMAL_EMP 2
#define SK_APPRAISE   3
#define SK_ATHLETICS  4
#define SK_BALANCE    5
#define SK_BLUFF      6
#define SK_CLIMB      7
#define SK_CONCENT    8
#define SK_CONCENTRATION 8
#define SK_CRAFT      9
#define SK_DECIPHER   10
#define SK_DIPLOMACY  11
#define SK_DISGUISE   12
#define SK_ESCAPE_ART 13
#define SK_FIND_WEAKNESS 14
#define SK_GATHER_INF 15
#define SK_HANDLE_DEV 16
#define SK_HEAL       17
#define SK_HEALING    17
#define SK_HIDE       18
#define SK_ILLUSION   19
#define SK_INTIMIDATE 20
#define SK_INTUITION  21
#define SK_JUMP       22
#define SK_KNOW_GEO   23
#define SK_KNOW_FIRST 23
#define SK_KNOW_INF    24  
#define SK_KNOW_MAGIC  25
#define SK_KNOW_MYTH   26 
#define SK_KNOW_NATURE 27
#define SK_KNOW_OCEANS 28
#define SK_KNOW_PLANES 29
#define SK_KNOW_THEO   30
#define SK_KNOW_UNDEAD 31
#define SK_KNOW_LAST   31
#define SK_KNOW_ALL    23,24,25,26,27,28,29,30,31
#define SK_LISTEN     32
#define SK_LOCKPICKING 33
#define SK_METAMAGIC  34
#define SK_MINING     35
#define SK_MOVE_SIL   36
#define SK_MOVE_SILENTLY 36
#define SK_PERFORM      37
#define SK_PICK_POCKET 38
#define SK_POISON_USE 39
#define SK_RIDE       40
#define SK_SEARCH     41
#define SK_SEARCHING  41
#define SK_SENESCHAL  42
#define SK_SPELLCRAFT 43
#define SK_SPOT       44
#define SK_SWIM       45
#define SK_SWIMMING   45
#define SK_TUMBLE     46
#define SK_TUMBLING   46
#define SK_USE_MAGIC  47
#define SK_WILDERNESS 48
#define SK_WILD_LORE  48
#define SK_LASTSKILL  49
#define SK_LAST       49
#define SK_ANY -1

#define ABYSSAL     0
#define POOR        1
#define AVERAGE     2
#define GOOD        3
#define GREAT       4
#define SUPERIOR    5
#define UNEARTHLY   6
//#define DIVINE      7


#define CA_ATTR_BOOST       -6
#define CA_INNATE_EFFECT    -5
#define CA_BONUS_SKILL      -4
#define CA_SORCEROR_CHOICE  -3
#define CA_ROGUE_CHOICE     -2

#define CA_ANCESTRAL_MEMORY  1
#define CA_ANIMAL_COMP       2
#define CA_AURA_OF_MENACE    4
#define CA_AURA_OF_VALOR     5
#define CA_BARDIC_MUSIC      6 /* 1-5 */
#define CA_BEGUILING_MAGIC   7
#define CA_BERSERK_RAGE      8
#define CA_BONUS_SCHOOL      9
#define CA_BOOST_RESISTS     10
#define CA_BOOST_SPELLS      11
#define CA_BLACK_MAGIC       12
#define CA_BLINDSIGHT        13
#define CA_BLOOD_MAGIC       14
#define CA_BOOK_OF_SHADOWS   15
#define CA_CHANNELING        17
#define CA_COMMAND           18
#define CA_CONTACT_MIND      19
#define CA_CRAFT_PSICRYSTAL  20
#define CA_DEATH_AFFINITY    21
#define CA_DEVOURING         22
#define CA_DOMAINS           23
// unused, this is really a feat: CA_DIRTY_FIGHTING    24
#define CA_DIVINE_GRACE      25 /* 1-2 */
#define CA_DWARVEN_FOCUS     29
#define CA_EARTHMELD         30
#define CA_EVASION           31
#define CA_FAMILIAR          32
#define CA_FATESENSE         33
#define CA_FAV_ENEMY         34
#define CA_FEED_UPON_PAIN    35
#define CA_FLIGHT            36
#define CA_FEAT_OF_STRENGTH  37
#define CA_GIANT_KILLER      38
#define CA_GREATER_TURNING   39
#define CA_GASEOUS_FORM      40
#define CA_INCREASED_MOVE    41
#define CA_INFRAVISION       42
#define CA_INHERANT_POTENTIAL 43
#define CA_INNATE_SPELL      44
#define CA_INSPIRE_ALLIES    45
#define CA_INVOKE_THE_LAND   46
#define CA_KARMIC_IMMUNITY   47
#define CA_KI_STRIKE         48
#define CA_LAY_ON_HANDS      49
#define CA_LEGEND_LORE       50
#define CA_LIFE_AFFINITY     51
#define CA_LIMITED_FREE_ACTION 52
#define CA_LOWLIGHT          53
#define CA_MAGICAL_NATURE    54
#define CA_MANIFESTATION     55
#define CA_MAXIMIZE_HEALING  56
#define CA_NATURE_SENSE      57
#define CA_NOBLE_AEGIS       58
#define CA_OPPORTUNIST       59
#define CA_PASS_SLIME        60
#define CA_PHASE             61
#define CA_PREMONITION       62
#define CA_PROTECTIVE_WARD   63
#define CA_PSIONICS          64
#define CA_PURGE_MAGIC       65
#define CA_REGEN             66
#define CA_RETRIBUTIVE_STRIKE 67
#define CA_REVERIE           68
#define CA_SACRED_AURA       69
#define CA_SACRED_MOUNT      70
#define CA_SCENT             71
#define CA_SENSE_MAGIC       72
#define CA_SHARP_SENSES      73
#define CA_SHIELD_GROUP      74
#define CA_SKILL_MASTERY     75
#define CA_SLIPPERY_MIND     76
#define CA_SLOW_FALL         77
#define CA_SLOW_METABOLISM   78
#define CA_SMITE             79
#define CA_SNEAK_ATTACK      80
#define CA_SOOTHING_WORD     81
#define CA_SPELLCASTING      82
#define CA_SPAWN             83
#define CA_SPECIALIST        84
#define CA_SPECIES_AFFINITY  85
#define CA_STALKER           86
#define CA_STONEWORK_SENSE   87
#define CA_STRONG_SOUL       88
#define CA_STUN_ATTACK       89
#define CA_SYLVAN_AFFINITY   90
#define CA_TELEPATHY         91
#define CA_TEMPRATURE_TOL    92
#define CA_TOUGH_AS_HELL     93
#define CA_TRACKING          94
#define CA_TREMORSENSE       95
#define CA_TUNNEL            96
#define CA_TURNING           97
#define CA_TURN_RESISTANCE   98
#define CA_UNARMED_STRIKE    99
#define CA_UNCANNY_DODGE     100
#define CA_UNEARTHLY_LUCK    101
#define CA_VERSATILITY       102
#define CA_WALLWALK          103
#define CA_WEATHER_SENSE     104
#define CA_WEAPON_IMMUNITY   105
#define CA_WEAPON_SKILL      106
#define CA_WHITE_MAGIC       107
#define CA_WHOLENESS_OF_BODY 108
#define CA_WILD_SHAPE        109 /* 1-4 */
#define CA_WOODLAND_STRIDE   110
#define CA_AIR_MASTERY       111
#define CA_EARTH_MASTERY     112
#define CA_WATER_MASTERY     113
#define CA_FLURRY_OF_BLOWS   114
#define CA_ARCANE_WILL       115
#define CA_BURNING_HUNGER    116
#define CA_FATIGUE_REGEN     117
#define CA_RAPID_RIDING      118
#define CA_PLANT_MASTERY     119
#define CA_COMMAND_AUTHORITY 120
#define CA_PREPATORY_MAGIC   121
#define CA_UNBIND            122
#define CA_NEGATE_CRIT       123
#define CA_FLAWLESS_DODGE    124
#define CA_PSYCH_MIGHT       125
#define CA_ARCANE_TRICKERY   126
#define CA_SPELLBREAKER      127
#define CA_LORE_OF_ARMS      128
#define CA_MIGHTY_STROKE     129
#define CA_TRICK_MAGIC       130
#define CA_STORYCRAFT        131
#define CA_SPELL_FURY        132
#define CA_FASTING           133
#define CA_SIZE_ADVANTAGE    134
#define CA_THIEF_IN_THE_NIGHT 135
#define CA_WEAPONCRAFT       136
#define CA_DEFENSIVE_ROLL    137
#define CA_TATTOOS           138
#define CA_ACROBATIC_FIGHTING 139
#define CA_GRANT_ITEM        140
#define CA_MASTER_ARTISAN    141
#define CA_HIDE_IN_PLAIN_SIGHT 142
#define CA_LAST              143

#define PHD_PARTY    1 /* Generic Pool / NPC Allies */
#define PHD_MAGIC    2 /* Dominated or Summoned Mons. */
#define PHD_ANIMAL   3 /* Ranger/Druid Animal Comps. */
#define PHD_COMMAND  4 /* Cleric's Commanded Undead */
#define PHD_UNDEAD   5 /* Necromancer Undead Pool */
#define PHD_FREEBIE  6 /* Do not count toward any PHD */
                       
#define SPEC_RELIGIOUS  1
#define SPEC_MULTICLASS 2

#define ABIL_VAL     1000
#define FEAT_VAL     2000
#define SKILL_VAL    3000
#define STATI_VAL    4000
#define SPELL_VAL    5000
#define MFLAG_VAL    6000
#define MTYPE_VAL    7000
#define ATTK_VAL     8000
#define ALIGN_VAL    9000
#define ATTR_VAL     10000
#define CLEV_VAL     11000



#define WG_SIMPLE        0x00000001
#define WG_EXOTIC        0x00000002
#define WG_SBLADES       0x00000004
#define WG_LBLADES       0x00000008
#define WG_AXES          0x00000010
#define WG_ARCHERY       0x00000020
#define WG_STAVES        0x00000040
#define WG_IMPACT        0x00000080
#define WG_THROWN        0x00000100
#define WG_POLEARMS      0x00000200
#define WG_SPEARS        0x00000400
#define WG_LANCES        0x00000800
#define WG_DAGGERS       0x00001000
#define WG_MARTIAL       0x00002000 /* Martial Arts weapons */
#define WG_FLEXIBLE      0x00004000 /* chain, nunchaku, etc. */
#define WG_FIREARMS      0x00008000
#define WG_LIGHT         0x00010000 /* Light-weight weapons */
#define WG_FLAILS        0x00020000
#define WG_SHIELDS       0x10000000
#define WG_LARMOR        0x20000000
#define WG_MARMOR        0x40000000
#define WG_HARMOR        0x80000000

#define WQ_HOLY          1
#define WQ_UNHOLY        2
#define WQ_CHAOTIC       3
#define WQ_LAWFUL        4
#define WQ_BALANCE       5
#define WQ_CORROSIVE     6
#define WQ_FLAMING       7
#define WQ_SHOCKING      8
#define WQ_FROST         9
#define WQ_THUNDERING    10
#define WQ_BURST         11
#define WQ_SOULDRINK     12
#define WQ_KEEN          13
#define WQ_VORPAL        14
#define WQ_DEFENDING     16
#define WQ_WARNING       17
#define WQ_CLEAVING      18
#define WQ_SMITING       19
#define WQ_ENERGY        20
#define WQ_DISTANCE      21 /* Xbow of... */
#define WQ_SPEED         22
#define WQ_ACCURACY      23
#define WQ_SLAYING       24
#define WQ_DANCING       25
#define WQ_DISRUPTION    26
#define WQ_RETURNING     27
#define WQ_WOUNDING      28
#define WQ_SLEEP         29
#define WQ_STUNNING      30
#define WQ_TERROR        31
#define WQ_VAMPIRIC      32
#define WQ_PARRYING      33
// #define WQ_FEATHERLIGHT  34
#define WQ_BANE          35
#define WQ_ENTROPIC      36
#define WQ_WEAKENING     37  /* These three qualities must stay in the */
#define WQ_NUMBING       38  /* same order as the attributes they afflict: */
#define WQ_WITHERING     39  /* Weakening/Str, then Numbing/Dex, then Con. */
#define WQ_VICIOUS       40
#define WQ_BINDING       41
#define WQ_SEEKING       42
#define WQ_MIGHTY        43
#define WQ_NIGHT         44
#define WQ_QUICK_LOADING 45
#define WQ_IMPACT        46
#define WQ_VENOM         47
#define WQ_SUNDERING     48
#define WQ_FEY_TRAPPING  49
#define WQ_FORKING       50
#define WQ_ROARING       51
#define WQ_GLOWING       52
#define WQ_PAIN          53
#define WQ_SUBTLETY      54
#define WQ_THIRSTY       55
#define WQ_PRECISE       56
#define WQ_UNCLEAN       57
#define WQ_SHIFTING      58
#define WQ_MECHANICAL    59
#define WQ_IMPALING      61
#define WQ_DRAINING      62
#define WQ_BLOODED       63

#define WQ_LAST          64

#define AQ_LIGHT_FOR     1
#define AQ_MEDIUM_FOR    2
#define AQ_HEAVY_FOR     3
#define AQ_FIRE_RES      4
#define AQ_COLD_RES      5
#define AQ_ACID_RES      6
#define AQ_LIGHT_RES     7
#define AQ_SONIC_RES     8
#define AQ_POISON_RES    9
#define AQ_ANIMATED      10
#define AQ_ARROW_DEF     11
#define AQ_BASHING       12
#define AQ_BLINDING      13
#define AQ_ETHEREALNESS  14
#define AQ_GHOST_TOUCH   16
#define AQ_GRACEFUL      17
#define AQ_GREAT_INV     18
#define AQ_INVULN        19
#define AQ_MAGIC_RES     20
#define AQ_REFLECTION    21
#define AQ_SHADOW        22
#define AQ_SILENT        23
#define AQ_SLICK         24
#define AQ_MIGHT         25
#define AQ_AGILITY       26
#define AQ_ENDURANCE     27
#define AQ_HEALING       28
#define AQ_STABILITY     29
#define AQ_COMMAND       30
#define AQ_RADIANT       31
#define AQ_FEARSOME      32
#define AQ_SACRED        33
#define AQ_SHIFTING      34
#define AQ_BANEBLIND     35
#define AQ_RESISTANT     36
#define AQ_AUSPICIOUS    37
#define AQ_SPIKED        38
#define AQ_LIFEKEEPING   39
#define AQ_ARTICULATED   40
#define AQ_GRAVEN        41
#define AQ_LAST          42

#define AQ_SHATTERING    43


// ww: these are really generic "item qualities", and it's getting annoying
// to keep duplicating AQ_MITHRIL, WQ_MITHRIL checks
#define IQ_DWARVEN       67
#define IQ_ELVEN         68
#define IQ_ORCISH        69
#define IQ_ADAMANT       70
#define IQ_DARKWOOD      71
#define IQ_IRONWOOD      72
#define IQ_MITHRIL       73
#define IQ_SILVER        74
#define IQ_PROFICIENT    75
#define IQ_FEATHERLIGHT  76
#define IQ_GHOST_TOUCH   77 /* For Quality Lists Only */
#define IQ_MASTERWORK    78 /* For Quality Lists Only */ 
#define IQ_BLESSED       79 /* For Quality Lists Only */
#define IQ_SACRED        80
#define IQ_WROUGHT       81

#define AQ_TRUELAST      81
#define WQ_TRUELAST      81


#define WS_NOT_PROF      0
#define WS_PROFICIENT    1
#define WS_FOCUSED       2
#define WS_SPECIALIST    3
#define WS_MASTERY       4
#define WS_HIGH_MASTERY  5
#define WS_GRAND_MASTERY 6

#define DF_VERTICAL 0x01
#define DF_OPEN     0x02
#define DF_STUCK		0x04
#define DF_LOCKED		0x08
#define DF_TRAPPED  0x10
#define DF_SECRET		0x20
#define DF_BROKEN   0x40
#define DF_SEARCHED 0x80
#define DF_PICKED   0x80

/* We avoid -1, because that's a special signifier in TextVal
   arrays for the function Lookup to use. */

#define FT_BARD_LIST         9
#define FT_OMNIFEAT_MAGE     8
#define FT_OMNIFEAT_FIGHTER  7 
#define FT_FULL_LIST         6
#define FT_WIZARD_LIST       5
#define FT_WARRIOR_LIST      4
#define FT_RANGER_LIST       3
#define FT_ROGUE_LIST        2
#define FT_MONK_LIST         1


#define FT_FIRST               (SK_LAST + 1)
#define FT_AB                  (SK_LAST + 1)
#define FT_ACROBATIC           (FT_AB + 1)
#define FT_ACUTE_SENSES        (FT_AB + 2)
#define FT_AIM_FOR_THE_EYES    (FT_AB + 3
#define FT_AIMED_SHOT          (FT_AB + 4)
#define FT_ALERTNESS           (FT_AB + 5)
#define FT_ALLURING            (FT_AB + 6)
#define FT_AMBIDEXTERITY       (FT_AB + 7)
#define FT_AMPLIFY_SPELL       (FT_AB + 8)
#define FT_ANCHOR_SPELL        (FT_AB + 9)
#define FT_ANIMAL_BOND         (FT_AB + 10)
#define FT_AP_LIGHT            (FT_AB + 11)
#define FT_AP_MEDIUM           (FT_AB + 12)
#define FT_AP_HEAVY            (FT_AB + 13)
#define FT_ARTIFICER           (FT_AB + 14)
#define FT_ARMOR_OPTIMIZATION  (FT_AB + 15)
#define FT_ARTERIAL_STRIKE     (FT_AB + 16)
#define FT_ATHLETIC            (FT_AB + 17)
#define FT_AUGMENT_HEALING     (FT_AB + 18)
#define FT_AUGMENT_SUMMONING   (FT_AB + 19)
#define FT_BACKSTAB            (FT_AB + 20)
#define FT_BATTLEFIELD_INSPIRATION (FT_AB + 21)
#define FT_BEASTIAL_EMPATHY    (FT_AB + 22)
#define FT_BEAST_SHAPES        (FT_AB + 23)
#define FT_BIND_SPELL          (FT_AB + 24)
#define FT_BLIND_FIGHT         (FT_AB + 25)
#define FT_BLINDSIGHT          (FT_AB + 26)
#define FT_BRACHIATION         (FT_AB + 27)
#define FT_BREAKFALL           (FT_AB + 28)
#define FT_BREAK_THE_BREATH    (FT_AB + 29)
#define FT_BREW_POTION         (FT_AB + 30)
#define FT_BROAD_COUNTERSPELL  (FT_AB + 31)

#define FT_CD                  (FT_AB + 32)

#define FT_CAPTIVATING         (FT_CD + 0)
#define FT_CENTERED            (FT_CD + 1)
#define FT_CHI_STRIKE          (FT_CD + 2)
#define FT_CHINK_IN_THE_ARMOR  (FT_CD + 3)
#define FT_CHOKE_HOLD          (FT_CD + 4)
#define FT_CIRCLE_KICK         (FT_CD + 5)
#define FT_CLEAR_MINDED        (FT_CD + 6)
#define FT_CLEAVE              (FT_CD + 7)
#define FT_CLEAVE_ASUNDER      (FT_CD + 8)
#define FT_CLOSE_QUARTERS_FIGHTER (FT_CD + 9)
#define FT_COMBAT_BRUTE        (FT_CD + 10)
#define FT_COMBAT_CASTING      (FT_CD + 11)
#define FT_COMBAT_FINESSE      (FT_CD + 12)
#define FT_COMBAT_OPPORTUNIST  (FT_CD + 13)
#define FT_COMBAT_REFLEXES     (FT_CD + 14)
#define FT_CONSECRATE_SPELL    (FT_CD + 15)
#define FT_CONTROL_SPELL       (FT_CD + 16)
#define FT_COORDINATED_TACTICS (FT_CD + 17)
#define FT_CRAFT_ARMOR         (FT_CD + 18)
#define FT_CRAFT_WEAPON        (FT_CD + 19)
#define FT_DANCE_OF_TUKIMA     (FT_CD + 20)
#define FT_DASH                (FT_CD + 21)
#define FT_DEADLY_CRIT         (FT_CD + 22)
#define FT_DEATH_BLOW          (FT_CD + 23)
#define FT_DECIEVER            (FT_CD + 24)
#define FT_DEFENSIVE_SHOT      (FT_CD + 25)
#define FT_DEFENSIVE_SPELL     (FT_CD + 26)
#define FT_DEFENSIVE_SYNERGY   (FT_CD + 27)
#define FT_DEFENSIVE_THROW     (FT_CD + 28)
#define FT_DEFILING_SPELL      (FT_CD + 29)
#define FT_DEFLECT_ARROWS      (FT_CD + 30)
#define FT_DEFLECT_MAGIC       (FT_CD + 31)
#define FT_DEFT_OPPORTUNIST    (FT_CD + 32)
#define FT_DETECTIVE           (FT_CD + 33)
#define FT_DIEHARD             (FT_CD + 34)
#define FT_DIRTY_FIGHTING      (FT_CD + 35)
#define FT_DIVINE_AEGIS        (FT_CD + 37)
#define FT_DIVINE_ARMOR        (FT_CD + 38)
#define FT_DIVINE_CLEANSING    (FT_CD + 39)
#define FT_DIVINE_MIGHT        (FT_CD + 40)
#define FT_DIVINE_RESISTANCE   (FT_CD + 41)
#define FT_DIVINE_VENGEANCE    (FT_CD + 42)
#define FT_DIVINE_VIGOR        (FT_CD + 43)
#define FT_DODGE               (FT_CD + 44)
#define FT_DOLOROUS_BLOW       (FT_CD + 45)
#define FT_DOMAIN_FOCUS        (FT_CD + 46)
#define FT_DOMAIN_MASTERY      (FT_CD + 47)
#define FT_DOUBLE_WEAPON_FIGHTING (FT_CD + 48)
#define FT_DRAGON_SHAPES       (FT_CD + 49)
#define FT_EFG                 (FT_CD + 50)

#define FT_EAGLE_CLAW_ATTACK   (FT_EFG + 1) 
#define FT_EARTHS_EMBRACE      (FT_EFG + 2)
#define FT_EDUCATED            (FT_EFG + 3)
#define FT_ELEGANT_DEFENSE     (FT_EFG + 4)
#define FT_ELUSIVE_TARGET      (FT_EFG + 5)
#define FT_EMPOWER_SPELL       (FT_EFG + 6)
#define FT_EMPOWER_TURNING     (FT_EFG + 7)
#define FT_ENDURANCE           (FT_EFG + 8)
#define FT_ENFOLDING_LOCK      (FT_EFG + 9)
#define FT_ENGENDER_PITY       (FT_EFG + 10)
#define FT_ENLARGE_SPELL       (FT_EFG + 11)
#define FT_EXOTIC_WEAPON       (FT_EFG + 12)
#define FT_EXPERTISE           (FT_EFG + 13)
#define FT_EXPERT_TACTICIAN    (FT_EFG + 14)
#define FT_EXTEND_SPELL        (FT_EFG + 15)
#define FT_FALLING_STAR_STRIKE (FT_EFG + 16)
#define FT_FASTER_THAN_THE_EYE (FT_EFG + 17)
#define FT_FAST_WILD_SHAPE     (FT_EFG + 18)
#define FT_FAR_SHOT            (FT_EFG + 19)
#define FT_FEATHERFOOT         (FT_EFG + 20)
#define FT_FISTS_OF_IRON       (FT_EFG + 21)
#define FT_FLICK_OF_THE_WRIST  (FT_EFG + 22)
#define FT_FLYBY_ATTACK        (FT_EFG + 23)
#define FT_FLYING_KICK         (FT_EFG + 24)
#define FT_FOCUS_SPELL         (FT_EFG + 25)
#define FT_FORTIFY_SPELL       (FT_EFG + 26)
#define FT_FORGE_RING          (FT_EFG + 27)
#define FT_FREEZE_THE_LIFEBLOOD (FT_EFG + 28)
#define FT_GRACEFUL            (FT_EFG + 29)
#define FT_GREAT_BLOW          (FT_EFG + 30)
#define FT_GREAT_CLEAVE        (FT_EFG + 31)
#define FT_GREAT_FORTITUDE     (FT_EFG + 32)
#define FT_GREAT_THROW         (FT_EFG + 33)
#define FT_GREATER_SOULBLADE   (FT_EFG + 34)
#define FT_GUARDED_INVOCATION  (FT_EFG + 35)
#define FT_GUARDED_STANCE      (FT_EFG + 36)
#define FT_GUILDMAGE           (FT_EFG + 37)
#define FT_HI                  (FT_EFG + 38)

#define FT_HAMSTRING           (FT_HI + 1)
#define FT_HARDINESS           (FT_HI + 2)
#define FT_HEIGHTED_TURNING    (FT_HI + 3)
#define FT_HEIGHTEN_SPELL      (FT_HI + 4)
#define FT_HOLD_THE_LINE       (FT_HI + 6)
#define FT_HOVER               (FT_HI + 7)
#define FT_IGNORE_WOUND        (FT_HI + 8)
#define FT_IMPROVED_CRIT       (FT_HI + 9)
#define FT_IMPROVED_DEMORALIZE (FT_HI + 10)
#define FT_IMPROVED_GRAPPLE    (FT_HI + 11)
#define FT_IMPROVED_ATTR       (FT_HI + 13)
#define FT_IMPROVED_STRENGTH   (FT_HI + 13)   /* Important! Do not change the order of the */
#define FT_IMPROVED_DEXTERITY  (FT_HI + 14)   /* Improved [Attr] feats! The game depends on */
#define FT_IMPROVED_CONSTITUTION (FT_HI + 15) /* (FT_IMPROVED_ATTR + A_WIS) being equal to */
#define FT_IMPROVED_INTELLIGENCE (FT_HI + 16) /* FT_IMPROVED_WISDOM. */
#define FT_IMPROVED_WISDOM     (FT_HI + 17)
#define FT_IMPROVED_CHARISMA   (FT_HI + 18)
#define FT_IMPROVED_LUCK       (FT_HI + 19)
#define FT_INHERANT_SPELL      (FT_HI + 20)
#define FT_INNATE_SPELL        (FT_HI + 21)
#define FT_INSIGHTFUL          (FT_HI + 22)
#define FT_INSTANT_STAND       (FT_HI + 23)
#define FT_INTENSIVE_STUDY     (FT_HI + 24)
#define FT_IRON_SKIN           (FT_HI + 25)
#define FT_IRON_WILL           (FT_HI + 26)
#define FT_IMPROVED_INITIATIVE (FT_HI + 27)
#define FT_IMPROVED_MOUNT      (FT_HI + 28)
#define FT_IMPROVED_TURNING    (FT_HI + 29)
#define FT_JKLM                (FT_HI + 30)

#define FT_JUDICIOUS_SPELL     (FT_JKLM + 1)
#define FT_KNOCKBACK           (FT_JKLM + 2) 
#define FT_KNOCK_PRONE         (FT_JKLM + 3)
#define FT_LANDED_NOBLE        (FT_JKLM + 4)
#define FT_LARCENOUS           (FT_JKLM + 5)
#define FT_LEADERSHIP          (FT_JKLM + 6)
#define FT_LIGHTNING_FISTS     (FT_JKLM + 7)
#define FT_LIGHTNING_REFLEXES  (FT_JKLM + 8)
#define FT_LIGHT_SLEEPER       (FT_JKLM + 9)
#define FT_LION_HEART          (FT_JKLM + 10)
#define FT_LIVING_WALL         (FT_JKLM + 11)
#define FT_LOADBEARER          (FT_JKLM + 12)
#define FT_LORE_OF_ACID        (FT_JKLM + 13)
#define FT_LORE_OF_DEATH       (FT_JKLM + 14)
#define FT_LORE_OF_FLAMES      (FT_JKLM + 15)
#define FT_LORE_OF_RIME        (FT_JKLM + 16)
#define FT_LORE_OF_STORMS      (FT_JKLM + 17)
#define FT_MAGE_SLAYER         (FT_JKLM + 18)
#define FT_MANTIS_LEAP         (FT_JKLM + 19)
#define FT_MARTIAL_MASTERY     (FT_JKLM + 20)
#define FT_MASTER_BULL_RUSH    (FT_JKLM + 21)
#define FT_MASTER_DISARM       (FT_JKLM + 22)
#define FT_MASTER_GREAT_BLOW   (FT_JKLM + 23)
#define FT_MASTER_THROW        (FT_JKLM + 24)
#define FT_MASTER_TRIP         (FT_JKLM + 25)
#define FT_MAXIMIZE_SPELL      (FT_JKLM + 26)
#define FT_MOBILITY            (FT_JKLM + 27)
#define FT_MONKEY_GRIP         (FT_JKLM + 28)
#define FT_MOUNTED_ARCHERY     (FT_JKLM + 29)
#define FT_MOUNTED_COMBAT      (FT_JKLM + 30)
#define FT_MULTIATTACK         (FT_JKLM + 31)
#define FT_MURDEROUS           (FT_JKLM + 32)
#define FT_MYSTIC_PREPARATION  (FT_JKLM + 33)
#define FT_MYTHIC_SHAPES       (FT_JKLM + 34)
#define FT_MISSILE_PROOF       (FT_JKLM + 35)
#define FT_MASTER_GRAPPLE      (FT_JKLM + 36)
#define FT_NOP                 (FT_JKLM + 37)

#define FT_NATURAL_APTITUDE    (FT_NOP + 1)
#define FT_NATURAL_GRAB        (FT_NOP + 2)
#define FT_NECROPHYSIOLOGY     (FT_NOP + 3)
#define FT_NON_STANDARD_FLURRY (FT_NOP + 4)
#define FT_ONE_BODY_ONE_SOUL   (FT_NOP + 5)
#define FT_OVERRUN             (FT_NOP + 6)
#define FT_PAIN_TOLERANCE      (FT_NOP + 7)
#define FT_PAIN_TOUCH          (FT_NOP + 8)            
#define FT_PARTIAL_EVASION     (FT_NOP + 9)
#define FT_PERCEPTIVE          (FT_NOP + 10)
#define FT_PERSISTANT_SPELL    (FT_NOP + 11)
#define FT_PERSUASIVE          (FT_NOP + 12)
#define FT_PHYSICIAN           (FT_NOP + 13)
#define FT_PINNING_SHOT        (FT_NOP + 14)
#define FT_PLANT_EMPATHY       (FT_NOP + 15)
#define FT_PLANT_SHAPES        (FT_NOP + 16)
#define FT_POINT_BLANK_SHOT    (FT_NOP + 17)
#define FT_POLESTAR_FIST       (FT_NOP + 18)
#define FT_POUNCE              (FT_NOP + 19)
#define FT_POWER_ATTACK        (FT_NOP + 20)
#define FT_POWER_CHARGE        (FT_NOP + 21)
#define FT_POWER_DOUBLE_WEAPON (FT_NOP + 22)
#define FT_POWER_JUMP          (FT_NOP + 23)
#define FT_POWER_KICK          (FT_NOP + 24)
#define FT_PRECISE_SHOT        (FT_NOP + 25)
#define FT_PROJECT_SPELL       (FT_NOP + 26)
#define FT_PSYCHIC_DUEL        (FT_NOP + 27)
#define FT_QRS                 (FT_NOP + 28)

#define FT_QUICK_DRAW          (FT_QRS + 1)
#define FT_QUICKEN_INNATE_MAGIC (FT_QRS + 2)
#define FT_QUICKEN_SPELL       (FT_QRS + 3)
#define FT_QUICKEN_TURNING     (FT_QRS + 4)
#define FT_QUICKER_THAN_THE_EYE (FT_QRS+ 5)
#define FT_RAKE                (FT_QRS + 6)
#define FT_RANGED_DISARM       (FT_QRS + 7)
#define FT_RAPID_RELOAD        (FT_QRS + 8)
#define FT_RAPID_SHOT          (FT_QRS + 9)
#define FT_REDIRECT_ATTACK     (FT_QRS + 10)
#define FT_REED_IN_THE_WIND    (FT_QRS + 11)
#define FT_REFLECTIVE_COUNTERSPELL (FT_QRS + 12)
#define FT_REND                (FT_QRS + 13)
#define FT_REPEAT_SPELL        (FT_QRS + 14)
#define FT_RESILIENT           (FT_QRS + 15)
#define FT_RESIST_DEATH        (FT_QRS + 16)
#define FT_RESIST_PARALYSIS    (FT_QRS + 17)
#define FT_RIDE_BY_ATTACK      (FT_QRS + 18)
#define FT_RIPOSTE             (FT_QRS + 19)
#define FT_ROLL_WITH_IT        (FT_QRS + 20)
#define FT_RUN                 (FT_QRS + 21)
#define FT_RUNNING_SHOT        (FT_QRS + 22)
#define FT_SCHOOL_FOCUS        (FT_QRS + 23)
#define FT_SCRIBE_SCROLL       (FT_QRS + 24)
#define FT_SCROLL_MASTERY      (FT_QRS + 25)
#define FT_SHIELD_BASH         (FT_QRS + 26)
#define FT_SHIELD_EXPERT       (FT_QRS + 27)
#define FT_SHIELD_FOCUS        (FT_QRS + 28)
#define FT_SHIELD_PROFICIENCY  (FT_QRS + 29)
#define FT_SHOCK_TROOPER       (FT_QRS + 30)
#define FT_SIGNATURE_ITEM      (FT_QRS + 31)
#define FT_SKILL_APTITUDE      (FT_QRS + 32)
#define FT_SKILL_FOCUS         (FT_QRS + 33)
#define FT_SLIPAWAY            (FT_QRS + 34)
#define FT_SNEAKY              (FT_QRS + 35)
#define FT_SOULBLADE           (FT_QRS + 36)
#define FT_SPELL_MASTERY       (FT_QRS + 37)
#define FT_SPELL_PENETRATION   (FT_QRS + 38)
#define FT_SPIRITED_CHARGE     (FT_QRS + 39)
#define FT_SPRING_ATTACK       (FT_QRS + 40)
#define FT_STAFF_MASTERY       (FT_QRS + 41)
#define FT_STILL_SPELL         (FT_QRS + 42)
#define FT_STRIKE_THE_WATER    (FT_QRS + 43)
#define FT_STUBBORN_MIND       (FT_QRS + 44)
#define FT_SUNDER              (FT_QRS + 45)
#define FT_SUPREME_CLEAVE      (FT_QRS + 46)
#define FT_SURE_SPELL          (FT_QRS + 47)
#define FT_SWARM               (FT_QRS + 48)
#define FT_SPEAR_AND_SHIELD    (FT_QRS + 49)
#define FT_SHOOT_FROM_COVER    (FT_QRS + 50)
#define FT_TUVWXYZ             (FT_QRS + 51)

#define FT_TACTICAL_WITHDRAWL  (FT_TUVWXYZ + 1)
#define FT_TALENTED            (FT_TUVWXYZ + 2)
#define FT_TERRIBLE_MEIN       (FT_TUVWXYZ + 3)
#define FT_TOUGHNESS           (FT_TUVWXYZ + 4)
#define FT_TRACK               (FT_TUVWXYZ + 5)
#define FT_TRAMPLE             (FT_TUVWXYZ + 6)
#define FT_TRANSMUTE_SPELL     (FT_TUVWXYZ + 7)
#define FT_TURN_EVIL           (FT_TUVWXYZ + 8)
#define FT_TURN_OUTSIDERS      (FT_TUVWXYZ + 9)
#define FT_TURN_RESISTANCE     (FT_TUVWXYZ + 10)
#define FT_TWIN_WEAPON_STYLE   (FT_TUVWXYZ + 11)
#define FT_TWO_WEAPON_STYLE    (FT_TUVWXYZ + 12)
#define FT_TWO_WEAPON_TEMPEST  (FT_TUVWXYZ + 13)
#define FT_UNBALANCING_STRIKE  (FT_TUVWXYZ + 14)
#define FT_UNSEEN_SPELL        (FT_TUVWXYZ + 15)
#define FT_VERMIN_SHAPES       (FT_TUVWXYZ + 16)
#define FT_VOCALIZE_SPELL      (FT_TUVWXYZ + 17)
#define FT_VILE_SPELL          (FT_TUVWXYZ + 18)
#define FT_WAND_MASTERY        (FT_TUVWXYZ + 19)
#define FT_WARCRY              (FT_TUVWXYZ + 20)
#define FT_WARP_SPELL          (FT_TUVWXYZ + 21)
#define FT_WEAPON_FINESSE      (FT_TUVWXYZ + 22)
#define FT_WEAPON_FOCUS        (FT_TUVWXYZ + 23)
#define FT_WEAPON_FOCUS_BITE   (FT_TUVWXYZ + 24)
#define FT_WEAPON_FOCUS_CLAW   (FT_TUVWXYZ + 25)
#define FT_WEAPON_FOCUS_TENTACLE (FT_TUVWXYZ + 26)
#define FT_WEAPON_FOCUS_HORNS  (FT_TUVWXYZ + 27)
#define FT_WEAPON_FOCUS_SPORES (FT_TUVWXYZ + 28)
#define FT_WEAPON_MASTERY      (FT_TUVWXYZ + 29)
#define FT_WEAPON_PROFICIENCY  (FT_TUVWXYZ + 30)
#define FT_WEAPON_SPECIALIST   (FT_TUVWXYZ + 31)
#define FT_WEATHER_BLAST       (FT_TUVWXYZ + 32)
#define FT_WHIRLWIND_ATTACK    (FT_TUVWXYZ + 33)
#define FT_WILD_SHAPE_ELEMENTALKIN (FT_TUVWXYZ + 34)
#define FT_WILD_SHAPE_MYTHIC   (FT_TUVWXYZ + 35)
#define FT_WILD_SHAPE_PLANT    (FT_TUVWXYZ + 36)
#define FT_WILD_SHAPE_SCENT    (FT_TUVWXYZ + 37)
#define FT_WILD_SHAPE_VERMIN   (FT_TUVWXYZ + 38)
#define FT_WILL_TO_LIVE        (FT_TUVWXYZ + 39)
#define FT_WOODSMAN            (FT_TUVWXYZ + 40)
#define FT_ZEN_ARCHERY         (FT_TUVWXYZ + 41)
#define FT_ZEN_DEFENSE         (FT_TUVWXYZ + 42)
#define FT_WILD_SHAPE_MASTERY  (FT_TUVWXYZ + 43)
#define FT_WEAPON_HIGH_MASTERY (FT_TUVWXYZ + 44)
#define FT_WEAPON_GRAND_MASTERY (FT_TUVWXYZ + 45)
#define FT_LAST                (FT_TUVWXYZ + 46)

#define MM_AMPLIFY       0x00000001  /* Penetrates magic resistance */
#define MM_AUGMENT       0x00000002  /* Summoned creatures stronger */
#define MM_ANCHOR        0x00000004  /* Extended duration */
#define MM_BIND          0x00000008  /* Will not harm you */
#define MM_CONSECRATE    0x00000010  /* 1/2 holy damage */
#define MM_CONTROL       0x00000020  /* control random effects */
#define MM_DEFENSIVE     0x00000040  /* no AoO when casting */
#define MM_DEFILING      0x00000080  /* Defiler effects */
#define MM_JUDICIOUS     0x00000100  /* Affects Only Enemies */
#define MM_EMPOWER       0x00000200  /* 150% variable effect */
#define MM_ENLARGE       0x00000400  /* x2 area */
#define MM_EXTEND        0x00000800  /* Double Range & Duration */
#define MM_FOCUS         0x00001000  /* 1/3 area of effect */
#define MM_FORTIFY       0x00002000  /* harder to dispel */
#define MM_HEIGHTEN      0x00004000  /* +4 save DC, increased HD */
#define MM_INHERANT      0x00008000  /* No spellbook needed! */
#define MM_MAXIMIZE      0x00010000  /* Maximum Damage */
#define MM_PERSISTANT    0x00020000  /* Lasts until you sleep */
#define MM_PROJECT       0x00040000  /* Touch spell becomes ray */
#define MM_QUICKEN       0x00080000  /* 1/2 normal timecost */
#define MM_REPEAT        0x00100000  /* Use repeatedly fast */
#define MM_STILL         0x00200000  /* No somantic component */
#define MM_SURE          0x00400000  /* 100% spell chance */
#define MM_TRANSMUTE     0x00800000  /* Change damage type */
#define MM_UNSEEN        0x01000000  /* Can't tell source */
#define MM_VILE          0x02000000  /* Does vile damage */
#define MM_VOCALIZE      0x04000000  /* No verbal component */
#define MM_WARP          0x08000000  /* Pass through Walls */

#define STUDY_CASTING     1
#define STUDY_BARDIC      2
#define STUDY_SHAPES      3
#define STUDY_BAB         4
#define STUDY_TURNING     5
#define STUDY_SMITE       6
#define STUDY_UNARMED     7
#define STUDY_SNEAK       8
#define STUDY_MOUNT       9
#define STUDY_LAST        10

#define EP_PLAYER_ONLY  0x00000
#define EP_ATTACK       0x00001
#define EP_STEALTH      0x00002
#define EP_CURSE        0x00004
#define EP_ESCAPE       0x00008
#define EP_BUFF         0x00010
#define EP_RESIST       0x00020
#define EP_FOIL_PURSUIT 0x00040
#define EP_FIX_TROUBLE  0x00080
#define EP_SUMMON       0x00100
#define EP_COUNTER      0x00200
#define EP_PASSAGE      0x00400
#define EP_BARRIER      0x00800
#define AIM_AT_ENEMY    0x01000
#define AIM_AT_SELF     0x02000
#define AIM_AT_ALLY     0x04000
#define AIM_AT_DOOR     0x08000
#define EP_UNLOCK       0x10000
#define EP_DISPEL       0x20000

#define VS_GENERAL       0
#define VS_MELEE         1
#define VS_THEFT         2
#define VS_MISSILES      3
#define VS_TELEPORT      4
#define VS_SIGHT         5
#define VS_CASTING       6
#define VS_SUMMON        7
#define VS_WEAPON        8
#define VS_MENTAL        9
#define VS_FIRE          10
#define VS_COLD          11
#define VS_ACID          12
#define VS_LIGHTNING     13
#define VS_POISON        14
#define VS_NECRO         15
#define VS_PSYCHIC       16
#define VS_PURE_MAGIC    17
#define VS_LIGHT         18
#define VS_SONIC         19
#define VS_UNARMED       20
#define VS_ANIMATE_DEAD  21
#define VS_PARALYSIS     22
#define VS_UNDEAD        23
#define VS_INVIS         24
#define VS_FEAR          25
#define VS_GAZE          26
#define VS_HEALING       27

#define P_IMPOSSIBLE  1
#define P_VERY_LOW    2
#define P_LOW         3
#define P_MODERATE    4
#define P_HIGH        5
#define P_URGENT      6
#define P_EXTREME     7
#define P_CRITICAL    8
#define P_OVERRIDE    20     

#define ACT_WAIT      0
#define ACT_MOVE      1
#define ACT_JUMP      2
#define ACT_WATTACK   3
#define ACT_SATTACK   4
#define ACT_NATTACK   5
#define ACT_RATTACK   6
#define ACT_OPEN      11
#define ACT_HIDE      12
#define ACT_PICKUP    13
#define ACT_BULLRUSH  14
#define ACT_COUPDEGRACE 15
#define ACT_DISARM    16
#define ACT_TRIP      17
#define ACT_KICKDOOR  18
#define ACT_CLOSEDOOR 19
#define ACT_USE_ABIL  20
#define ACT_USE_SKILL 21
#define ACT_USE_FEAT  22
#define ACT_SHIFT     23
#define ACT_INSCRIBE  25
#define ACT_STAIRS    26
#define ACT_SPIKEDOOR 27
#define ACT_PRAY      28
#define ACT_SACRIFICE 29
#define ACT_DROP      30
#define ACT_REST      31
#define ACT_SEARCH    32
#define ACT_EAT       33
#define ACT_GREET     34
#define ACT_THREATEN  35
#define ACT_SURRENDER 36
#define ACT_GIVE      37
#define ACT_CAST      38
#define ACT_EQUIP     39
#define ACT_THROW     40
#define ACT_LOADXBOW  41
#define ACT_PHASE     42
#define ACT_LAST      43


#define Q_DIR  0x0001  
#define Q_LOC  0x0002
#define Q_TAR  0x0004
#define Q_INV  0x0008  /* Choose one item from caster inv to affect */
#define Q_EQU  0x0010  /* Affects all of target creature(s)' equip (water trap) */
#define Q_CRE  0x0020
#define Q_ITM  0x0040
#define Q_LOOK 0x0080
#define Q_NEAR 0x0100
#define Q_OPT  0x0200
#define Q_ALL  0x0400

#define YU_REVERSE    0x0001

/* Bonus types do not match up exactly to the d20 system here. Instead,
 * several d20 types are missing due to the division between armor
 * rating and defense rating (both rolled into AC in d20), while others
 * are added that are implicit in d20 (feat, skill, weapon, class). 
 * Because Luck is an Attribute, there is no "luck" bonus. */

#define BONUS_BASE    0  /* Base Value */
#define BONUS_CLASS2  1
#define BONUS_CLASS3  2
#define BONUS_STUDY   3
#define BONUS_ATTR    4  /* Str +toHit, Dex +toDef, Con +toFortSav, etc. */
#define BONUS_ENHANCE 5  /* d20 "enhancement" bonus; also "resistance" */
#define BONUS_ARTI    6  /* Stacks with everything */
#define BONUS_SACRED  7  /* Bless, Chant, Prayer, etc. */
#define BONUS_MORALE  8  /* d20 "morale" bonus (bards) */
#define BONUS_INSIGHT 9  /* d20 "insight" bonus */
#define BONUS_FEAT    10  /* Bonuses from Feats */
#define BONUS_WEAPON  11  /* Weapon Accuracy & Speed */
#define BONUS_CLASS   12 /* Bonuses from Class Abilities */
#define BONUS_NEGLEV  13  /* Negative Levels */
#define BONUS_COMP    14 /* d20 "competance" bonus */
#define BONUS_SIZE    15 /* d20 "enlargement" bonus */
#define BONUS_DEFLECT 16 /* d20 "deflection" bonus */
#define BONUS_DAMAGE  17 /* d20 attribute damage */
#define BONUS_RAGE    18 /* Barbarian Rage */
#define BONUS_STATUS  19 /* stunned/confused */
#define BONUS_INHERANT 20 /* Inherant Attr Bonuses */
#define BONUS_GRACE   21 /* Paladin's "Divine Grace" Power */
#define BONUS_DODGE   22 /* d20 "dodge" bonus */
#define BONUS_NATURAL 23 /* Stat modifiers; "natural armor" */
#define BONUS_ARMOR   24 /* Armor */
#define BONUS_SKILL   25 /* Weapon Focus/Specialization/etc. */
#define BONUS_DUAL    26 /* Two-Weapon Fighting Modifiers */
#define BONUS_FATIGUE 27 /* Fatigue modifiers */
#define BONUS_TEMP    28 /* Template modifiers stack */
#define BONUS_CIRC    29 /* d20 "circumstance" bonus */
#define BONUS_SHIELD  30
#define BONUS_LUCK    31
#define BONUS_ENCUM   32
#define BONUS_RESIST  33 /* d20 "resistance" bonus */
#define BONUS_ELEV    34
#define BONUS_TACTIC  35
#define BONUS_HUNGER  36
#define BONUS_PAIN    37
#define BONUS_HIDE    38
#define BONUS_LAST    39 

#define REST_SAFE     0x0001
#define REST_FULL     0x0002
#define REST_NOMANA   0x0004
#define REST_PLOT     0x0008
#define REST_INSTANT  0x0010
#define REST_NOMSG    0x0020
#define REST_NOHEAL   0x0040
#define REST_RESTLESS 0x0080
#define REST_DUNGEON  0x0100
#define REST_NOIDENT  0x0200
#define REST_FOOD     0x0400

#define DSM_SILENT    0
#define DSM_CHOICE    1
#define DSM_THROWN    2
#define DSM_FALL      3
#define DSM_FLOAT     4
#define DSM_INCOR     5


#define GENERIC_STATI 0

/* The first set of Stati represent various maladies */
#define SICKNESS      1
#define SICK          1
#define POISONED      2
#define BLINDNESS     3
#define BLIND         3
  #define BLIND_UNWANTED        0
  #define BLIND_EYES_CLOSED     1
#define HALLU         4
#define STUNNED       5
#define CONFUSED      6
#define SLEEPING      7
#define ASLEEP        7
  #define SLEEP_MAGIC   1
  #define SLEEP_NATURAL 2
  #define SLEEP_FATIGUE 3
  #define SLEEP_PSYCH   4
  #define SLEEP_DEEP    5
#define AGG_MON       8
#define PARALYSIS     9
  #define PARA_HELD   0
  #define PARA_HYPNOTIZED 1
  #define PARA_DAZED 2
#define AFRAID        10
  #define FEAR_MAGIC    0
  #define FEAR_PANIC    1
  #define FEAR_MANA     2
  #define FEAR_HP       3
  #define FEAR_SKIRMISH 4
  #define FEAR_COWED    5
#define STUCK         11
#define STUCK_STICKY  1
#define STUCK_PINNED  2
#define STUCK_WEAPON  3
#define STUCK_VINES   4
#define STUCK_ATTACK  5
#define STUCK_BONDED  6
#define BLEEDING      12
#define ILLUS_DMG     13
#define CHOKING       14
#define PRONE         15
#define EXPOSED       16
#define WOUNDED       17 /* Called Shots */
#define STONING       18
#define DISEASED      19
#define NAUSEA        20
#define TELEPORTED    21 /* See Magic::Travel */

/* These second set of Stati hold pieces of information about
   character skills and abilities. */

#define WEP_SKILL        30
#define FAV_ENEMY        31
#define EXTRA_SKILL      32
#define TURN_ABILITY     33
#define GTURN_ABILITY    34
#define COMMAND_ABILITY  35
#define SMITE_ABILITY    36
#define SPECIALTY_SCHOOL 37
#define SCHOOL_FOCUS     38
#define EXTRA_ABILITY    39
#define SPECIES_AFFINITY 40
#define EXTRA_FEAT       41
#define INNATE_SPELL     42
#define EXTRA_ATTACK     43
#define INNATE_KIT       44 /* Innate Skill Kit */
#define BONUS_PHD        45
#define IMP_CRIT         46

/* The middle set holds the "special" stati, that are neither
   magical (in the sense of dispellable) nor maladies to be
   cured by curative effects. */
#define SPRINTING     60
#define GRABBED       62
  #define GR_GRABBED   1
  #define GR_GRAPPLED  2
  #define GR_SWALLOWED 3
  #define GR_ENGULFED  4
  #define GR_GRABBER   5
  #define GR_GRAPPLER  6
  #define GR_SWALLOWER 7
  #define GR_ENGULFER  8
#define GRAPPLED      63
#define GRAPPLING     64
#define HIDING        65
  #define HI_SHADOWS   1 /* rogues */
  #define HI_CEILING   2 /* lurker above, darkmantle, etc. */
  #define HI_UNDER     3 /* hide under objects */
  #define HI_WATER     4 /* hiding beneath the water */
#define RAGING        66
#define SUMMONED      67
#define ACTING        68 /* Eating, Digging, etc. */
#define MAGIC_RES     69
#define DISPELLED     70
#define TEMPLATE      71
#define SPEC_TIMEOUT  72
#define TRACKING      73 /* Rangers */
#define ILLUSION      74
  #define IL_SPECTRAL 1
  #define IL_IMPROVED 2
  #define IL_ADVANCED 4
  #define IL_SHADE10  8
  #define IL_SHADE20  16
  #define IL_SHADE40  32
  #define IL_SHADE    (8 + 16 + 32)
#define CHARGING      75
#define TUMBLING      76
#define SINGING       77  
  #define BARD_COURAGE    1
  #define BARD_FASCINATE  2
  #define BARD_COUNTER    3
  #define BARD_COMP       4
  #define BARD_DISTRACT   5
  #define BARD_PHOENIX    6
  #define BARD_MASS       7
  #define BARD_SPELLBREAK 8
#define ANIMAL_EMP_TRIED 78
#define TRAP_EVENT    79
#define HUNGER        80
#define DWARVEN_FOCUS 81
#define DEFENSIVE     82
#define LFA_COUNT     83
#define GENERATION    84
#define ELEVATED      85
  #define ELEV_TREE    1
  #define ELEV_CEILING 2   
#define SUSPEND_ATTK  86 
#define MOUNTED       87
#define MOUNT         88
#define WOUNDED_BY    89 /* for Retributive Strike */
#define DISBELIEVED   90
#define FLAWLESS_DODGE 91
#define FLURRYING     92
#define ANC_MEM       93
#define ENGULFED      94
  #define EG_SWALLOW  1   
  #define EG_ENGULF   2
#define ENGULFER      95
#define DISB_TERRAIN  96
#define CORPSE_FLAG   97
/* The next set, magical effects that can be dispelled */

#define INVIS         100
  #define INV_NORMAL   1
  #define INV_IMPROVED 2
#define SEE_INVIS     101
#define PHASED        102
  #define PHASE_MATERIAL     0
  #define PHASE_VORTEX       1
  #define PHASE_ASTRAL       2
  #define PHASE_ETHERIAL     3
  #define PHASE_SHADOW       4
  #define PHASE_NEGATIVE     5
#define POLYMORPH     103
#define RESIST        104
#define ADJUST_IDX    105 /* Kludge for StatiIterAdjust */
#define ADJUST        106
#define ADJUST_SAC    107
#define ADJUST_INS    108
#define ADJUST_COMP   109
#define ADJUST_ART    110
#define ADJUST_DEFL   111
#define ADJUST_DMG    112 /* Ability Damage */
#define ADJUST_INH    113
#define ADJUST_MOR    114
#define ADJUST_DODG   115
#define ADJUST_ARM    116
#define ADJUST_CIRC   117
#define ADJUST_NAT    118
#define ADJUST_LUCK   119
#define ADJUST_RES    120
#define ADJUST_PAIN   121
#define ADJUST_SIZE   122
#define ADJUST_LAST   122 /* Keep this updated!!! */
#define SAVE_BONUS    123
#define SKILL_BONUS   124
#define SUSTAIN       125
#define REGEN         126
#define LEVITATION    127
#define WARNING       128
#define FORTIFICATION 129
#define WATER_WALKING 130
#define REFLECTING    131 /* General "block area magic" stati */
  #define REF_REFLECT   1 /* turn beam back to origin -- Shield of Reflection */
  #define REF_TURNING   2 /* turn percent of magic back -- Spell Turning */
  #define REF_ABSORB    3 /* absorb physical magic -- Rod of Absorbtion */
  #define REF_QUENCH    4 /* cancel area effect completely -- Rod of Fire Extinguishing */
#define WATER_BREATHING 132
#define ANCHORED      133 /* no teleporting, etc. */
#define DETECTING     134       /* Val = MA_TYPE, Mag = Range */
#define TIMESTOP      135
#define PREPARED      136 /* Contingency */
#define CHARMED       137 /* Three levels of charm in Incursion, in Val: */
#define   CH_CHARM    1   /*   No longer attacks you (charm person) */
#define   CH_CALMED   2   /*   Won't attack if you have not yet. */
#define   CH_ALLY     3   /*   Will fight on your behalf */
#define   CH_DOMINATE 4   /*   Will do as you say, joins party, etc. */
#define   CH_COMMAND  5   /*   Command Undead / Fire Creatures / etc. */
#define   CH_COMPEL   6   /*   scripted actions, i.e., Insatiable Thirst */
#define POSSESSED     138 /* You run the monster! (Magic Jar) */
#define ILLUMINATED   139 /* Faerie Fire; plusses to hit in ranged; '?' for invis */
#define SLOW_POISON   140
#define PERCEPTION    141
#define CONFLICT      142
#define PERIODIC      143 /* Periodic Effect, i.e., =oPoly, =oBlinking */
#define MANIFEST      144
#define MISS_CHANCE   145
  #define MISS_RANGED  1
  #define MISS_FULL    2
#define LIFESIGHT     146
#define ENRAGED       147
#define TRUE_SIGHT    148
#define TOUCH_ATTACK  149
#define IGNORE_SLOT_WEIGHT      150     /* ww: for floating disc spell, etc. */
#define IGNORES_WEAPON_IMMUNITY		151                 /* ww: largely for testing  */
#define BONUS_SCHOOL_CASTING_LEVEL	152 	/* e.g., Death Domain allows you to cast SC_NEC spells at one level higher. Val = SC_NEC, Mag = +1. */
#define DETECTING_TYPE		        153         /* Val = TYPE (e.g., T_ITEM), Mag = Range */
#define INVIS_TO      154
#define LUCUBRATION   155
#define TRANSFORMED   167 /* Item into monster, i.e., Rod of the Python. */
#define NEGATED       168
#define STAFF_SPELLS  169
#define ALT_ITEM      170 /* Replace slot contents -- see Springblade Bracers */
#define XP_PENALTY    171 /* For the named weapon 'Inertia', among other things */
#define XP_GAINED     172 /* Monster already defeated; don't give xp */
#define DIPLOMACY_TAG 173 /* Monster diplomacized; give xp for their kills */
#define SLIP_MIND     174 /* Slippery Mind */
#define DISGUISED     175
#define TELEKINETIC   176
#define IMMUNITY      177
#define DISTRACTED    178
#define ACTIVATION    179

/* Item Stati */
#define COCKED            180
#define DAMAGED           181
#define WIZLOCK           182
#define EXTRA_QUALITY     183
#define ALTER_PLUS        184
#define BOOST_PLUS        185
#define RETRY_BONUS       186
#define TRIED             187
#define COUNTER           188
#define EFF_FLAG1         189
#define EFF_FLAG2         190
#define ENTERED_AT        191
#define HEAL_TIMEOUT      192
#define RESET_BY          193 /* who gets XP when a trap kills someone? */
#define COMPONENTS_OF     194
#define AVOID_MELEE       195
#define MASTERWORK        196
#define MY_GOD            197
#define CHANNELING        198 /* Turn Undead "afterglow" for Divine Feats */
#define CONDITIONAL_FEAT  199
#define DROPPED           200
#define ALCHEMY_LEV       201
#define HEARD_IT          202
#define HEROIC_QUALITY    203
#define LIFESAVED         204
#define ON_BRIDGE         205
#define DO_NOT_PICKUP     206
#define HIT_WHILE_UNREADY 207
#define STORED_MM         208
#define SEEN_RIPPLE       209
#define CRIT_WOUNDED_BY   210

#define NONDETECTION          211
#define ANIMAL_COMPANION      212
#define HAS_ANIMAL            213

// ww: these replace and generalize things like SYLVAN_AFFINITY 
// and AGG_MON -- note that the eID field must *always* be set in
// one of these
#define NEUTRAL_TO            214
#define ENEMY_TO              215
#define ALLY_TO               216
#define PRACTICE_MISS_CHANCE  217 /* ww: as you attack someone with 
                                     Displacement you get slightly better
                                     with each hit, represented by a TempStati 
                                     of type PRACTICE_MISS_CHANCE pointing
                                     to that person */
#define ALSO_TURN_MA_TYPE     218 /* ww: turn Devils with your normal turn
                                     chance, etc. */
#define ADDITIONAL_WILD_SHAPE_MA_TYPE 219
  /* ww: perhaps you can wild shape into hybrids or jellies or somesuch
   * if you have special magical items or rare feats */
#define AUTO_CHARGE 220
  /* automatically break off charges without asking if you move in
   * another direction, and starting charging again whenever you move ... */
#define HAS_DOMAIN        221
#define ALIGNMENT         222 /* PCs can choose their alignment */
  /* use M_LAWFUL, M_EVIL, M_CHAOTIC, M_GOOD as the Val */

#define CAN_FORGET_SPELL  223
#define HOME_REGION       224
#define GAZE_REFLECTION   225
  #define GR_REFLECT      1
  #define GR_IMMUNE       2
#define EYES_AVERTED      226
#define MY_CORPSE_IS      227
#define LOCKED            228
#define BONUS_SLOTS       229
#define SPELL_ACCESS      230
#define WAS_FRIENDLY      231
#define HUNG_PULSE        232
#define NO_PHD            233
#define MAGIC_AURA        234
#define SOCIAL_MOD        234
#define DEATH_LOC         235
#define LAST_STATI        236
  


#define IsMagicStati(n) (((n) >= INVIS) && ((n) <= TOUCH_ATTACK))
#define IsMaladyStati(n) (((n) >= SICKNESS) && ((n) <= DISEASED))
#define IsAdjustStati(n) (((n) >= ADJUST) && ((n) <= ADJUST_LUCK))
#define IsAbilityStati(n) (((n) >= WEP_SKILL) && ((n) <= EXTRA_FEAT))

#define TA_ENEMY     1
#define TA_ALLY      2 /* Touch-healing spells */
#define TA_ITEM      3    
#define TA_GUARD     4 /* Location of Guard */
#define TA_POLYED    5 /* Don't recognize target */
#define TA_POTENTIAL 6 /* As yet unseen */
#define TA_WANDER    7 /* Point to wander to */
#define TA_LEADER    8 /* Follow him around */

#define TI_WALL 0
#define TI_FLOOR 1
#define TI_ROCK 2
#define TI_BORDER 3        

#define EF_PERIODIC    1
#define EF_CURSED      2  /* Generate Cursed */
#define EF_HALF_CURSED 3  /* Generate 50% Cursed */
#define EF_LIM_MTYPE   4
#define EF_LIM_LEVEL   5
#define EF_LIM_ITYPE   6
#define EF_BANE_MTYPE  7
#define EF_BANE_ITYPE  8
#define EF_KNOCKBACK   9 /* Fire burns, Force stuns, etc. */
#define EF_KNOCKDOWN   10 /* Stronger Specials */
#define EF_PARTIAL     11
#define EF_MULTI       12
#define EF_ONCEONLY    13 /* for beams */
#define EF_5000XP  14
#define EF_STRAIN  15
#define EF_STRAIN2 16
#define EF_FIREDOT 17 /* Little dot races to x,y pre-effect */
#define EF_NOGEN   18
#define EF_NAMEFIRST 19 /*  Darwin's Dagger vs. Dagger of Darwin's */
#define EF_NAMEONLY 20  /*  Spellblade vs. Sword of Spellblade */
#define EF_CHAOTIC  21  /* Use is a chaotic act, alignment affects power */
#define EF_LAWFUL   22  /* Ditto */
#define EF_DSHORT   23
#define EF_D1ROUND  24
#define EF_DLONG    25
#define EF_DXLONG   26
#define EF_SAFE     27  
#define EF_LARMOR   28  
#define EF_MARMOR   29  
#define EF_HARMOR   30  
#define EF_FSTUN    31  /* Failure Stuns */
#define EF_COMMON   32  
#define EF_MUNDANE  33  /* Random Component */
#define EF_CASTER_IMMUNE 34  /* Misc2 is a *resource* */
#define EF_ALLIES_IMMUNE 35
#define EF_ANIMAL   36
#define EF_UNDEAD   37
#define EF_ELEMENTAL 38
#define EF_CR_CAP   39
#define EF_AFFECTS_ITEMS 40
#define EF_ITEMS_ONLY 41
#define EF_CAP5     42
#define EF_CAP10    43
#define EF_NEEDS_PLUS 44 /* Add item bonus to Misc1 */
#define EF_STAPLE     45
#define EF_MOBILE     46 /* For Fields -- Antianimal Shell, etc. */
#define EF_NO_PRESS   47 /* For Fields -- Prot. Evil, etc. */
#define EF_ACTIVATE   48
#define EF_ACTIVATE2  49
#define EF_ACTIVATE3  50
#define EF_ACTIVATE4  51
#define EF_ACTIVATE5  52 
#define EF_MENTAL     53
#define EF_PERMANANT  54
#define EF_EVIL       55
#define EF_GOOD       56
#define EF_LETHAL     57
#define EF_SOUND      58
#define EF_VARMANA    59
#define EF_ATTACK     60
#define EF_SHOWNAME   61
#define EF_HALF_UNTYPED 62 /* Flame Strike / Ice Storm */
#define EF_DEATH      63
#define EF_GLYPH_FOG  64
#define EF_GLYPH_INSECTS 65
#define EF_CANCEL     66
#define EF_AUTOID     67
#define EF_USERID     68
#define EF_STRIKEID   69
#define EF_FEAR       70
#define EF_BLIND_PROMPT 71 /* Don't use isTarget in EffectPrompt */
#define EF_SHOWVAL    72
#define EF_SHOWMAG    73
#define EF_SHOWDUR    74
#define EF_DEFENSIVE  75
#define EF_3PERDAY    76
#define EF_7PERDAY    77
#define EF_LOSEMANA   78
#define EF_MULTIPLE   79
#define EF_GLYPH_FLOOR 80
#define EF_HIT_MOUNT  81
#define EF_SPECABIL   82
#define EF_NO_BONUS_DMG 83 /* no WIS bonus to this spell's dmg */
#define EF_DOMAIN     84
#define EF_PERSISTANT 85
#define EF_NOTBAD     86 /* has no negative consequences */
#define EF_CASTER_IMMUNE_BLAST 87
#define EF_HARDSAVE2  88
#define EF_HARDSAVE4  89
#define EF_HARDSAVE8  90
#define EF_POSTFIX    91 /* a long sword +2, Holy Avenger */
#define EF_PROPER     92 /* the long sword 'Ringil' */
#define EF_HIDEQUAL   93
#define EF_1PERDAY    94
#define EF_XSUMMON    95
#define EF_REPEAT     96 /* For Traps -- can trigger more than once */
#define EF_DISTANT    97 /* Don't verify line of fire, i.e., Dream */
#define EF_IMONSTERS  98 /* Monsters Only (Shadow Monsters) */
#define EF_IBLAST     99 /* Forces only (Shadow Magic) */
#define EF_CURSE      100
#define EF_NOCOMP     101
#define EF_ADJUST_GLYPH 102
#define EF_COMPULSION 103
#define EF_GAZE       104
#define EF_LAST       105


#define EA_BLAST    1      /* xval = DType, pval = Damage */
#define EA_GRANT    2      /* xval = Stati, yval = Val, pval = Mag, rval = t */
#define EA_INFLICT  3      /* xval = Stati, yval = Val, pval = Mag, rval = t */
#define EA_DRAIN    4
#define EA_TERRAFORM 5
  #define TERRA_EMPTY 1
  #define TERRA_ROCK  2
  #define TERRA_ALL   3
  #define TERRA_FLOOR 6
  #define TERRA_WATER 7
  #define TERRA_CHASM 8
  #define TERRA_OPEN  9         /* ! SolidAt */
  #define TERRA_CLOSED 10       /* not TERRA_OPEN */
  #define TERRA_BRIDGE 11
#define EA_TRAVEL   6      /* pval = Distance, rval = New Map, xval = Type */ 
  #define TRAVEL_KNOWN     1       /* qval determines randomness/target ability */
  #define TRAVEL_ANYWHERE  2       /* yval can be coded x/y coord */
  #define TRAVEL_LEVEL     3
  #define TRAVEL_TO_RVAL   4
  #define TRAVEL_TO_TARG   5
  #define TRAVEL_WORLDWALK 6 /* List known wilderness locations, jump to one */   
  #define TRAVEL_POCKET    7
#define EA_IDENTIFY 7      /* pval = max item level, xval = known flags, yval = max traits revealed */ 
#define EA_ILLUSION 8      
#define EA_SUMMON   9
  #define EF_CHOOSE     64
  #define EF_GROUP      63
#define EA_ANIMATE  10
#define EA_FIELD    11 /* Light, Darkness, Web, Obscurement, etc. */
#define EA_CREATION 13 /* Major Creation, etc. -- Create Item */
  #define CREA_WIELD  0x01
  #define CREA_STOW   0x02
  #define CREA_DROP   0x04
  #define CREA_MAGIC  0x08
  #define CREA_TEMP   0x10
  #define CREA_MANY   0x20
  #define CREA_TRAP   0x40
  #define CREA_XY     0x80 /* place at target xy, not evictim->xy */
#define EA_SLAYING  18 /* Slay Living, PW Kill, Disintigrate, etc. */
#define EA_DETECT   20  /* Misc1 = Below; Misc2 = Range (or 0 = board) */
  #define DET_VISIBLE      0x01 /* Highlighted targets become visible */     
  #define DET_VALUABLE     0x02 /* Sort by value in gold */
  #define DET_REVEAL       0x04 /* Show text descsribing found targets */
  #define DET_LOCATION     0x08 /* Tell location */
  #define DET_AUGURY       0x10 /* Find best and worst */
  #define DET_HILIGHT      0x20 /* Highlight Found Targets */
  #define DET_NATURE       0x40 /* Reveals (in text) *what* is found */
  #define DET_INVIS        0x80 /* Reveals invisible creatures */
#define EA_DISPEL   22 /* Dispel Magic / Disjunction */
  #define DIS_DISPEL       0x01
  #define DIS_DISJOIN      0x02
  #define DIS_BANISH       0x04
  #define DIS_INNATE       0x08
  #define DIS_SUPPRESS     0x10
#define EA_WEAPON   23 /* Conjure Magical Weapon -- Black Blade, Spiritual Hammer, etc. */
#define EA_BANISH   24 /* Send Away -- Imprisonment, Abjure, Banishment, etc. */
#define EA_NOTIMP   25 
#define EA_HEALING  26 /* Cure Light, Heal, etc; also Gain Mana, Bonus HP */
  #define HEAL_HP      0x01
  #define HEAL_MANA    0x02
  #define HEAL_XP      0x04
  #define HEAL_ATTR    0x08
  #define HEAL_MALADY  0x10
  #define HEAL_FATIGUE 0x20
  #define HEAL_HUNGER  0x40 /* by pval amount, no choking */
  #define HEAL_SATTR   0x80 /* Heal One Attribute */
  #define HEAL_ALL_MALADIES  0x100 /* like the priest spell 'Heal' */
#define EA_RAISE    27 /* Raise Dead, Ressurection */
#define EA_MENU     28 /* Multi-Effect Staves & Artifacts */
#define EA_CANCEL   29 /* Rod of Cancellation */
#define EA_WONDER   30 /* Wand of Wonder */
#define EA_MANYTHINGS 31 /* Guess */
#define EA_POLYMORPH 32
#define EA_LEVGAIN   34
#define EA_HOLDING   35 /* Deeppockets, Bag of Holding, etc. */
#define EA_WARNING   36 /* Amulet of Warning */
#define EA_DIGGING   37 /* Wand of Digging, Rock to Mud */
#define EA_EARTHQUAKE 38
#define EA_CURE      39 /* Remove Stati */
#define EA_RESTORE   40
#define EA_TURNING   41
#define EA_REVEAL    42
  #define EF_NEEDS_CORPSE   63
  #define EF_TELL_DISTANCE  62
  #define EF_TELL_INTENSITY 61
#define EA_ENCHANT   43         /* Magic Weapon, Keen Edge, etc. */
#define EA_CONSTRUCT 44         /* Spiritual Weapon, Black Blade */
#define EA_KNOCKBACK 45
#define EA_SHIELD   8
#define EA_GENERIC   46
#define EA_VISION    47
  #define VIS_MOBILE   0x01    /* Move Viewpoint About */
  #define VIS_SPECIFIC 0x02    /* See more than '?' for mons */
  #define VIS_SEE_INV  0x04    /* See Invisible */
  #define VIS_UNSEEN   0x08    /* Look at previously unseen areas */
  #define VIS_PERCEPT  0x10    /* Use 'Perception' */
  #define VIS_LOS      0x20    /* Use Regular Line of Sight */

  #define KNOW_ALIGN   0x01
  #define KNOW_HP      0x02
  #define KNOW_LEVEL   0x04
  #define KNOW_RESISTS 0x08
  #define KNOW_STATI   0x10
  #define KNOW_INVENT  0x20
  #define KNOW_MOMMEM  0x40
  #define KNOW_LOYALTY 0x80


#define AR_NONE      0
#define AR_BOLT      1
#define AR_BEAM      2
#define AR_BALL      3
#define AR_RAY       4
#define AR_BURST     5
#define AR_TOUCH     6
#define AR_FIELD     7
#define AR_BARRIER   8
#define AR_MFIELD    9  /* Mobile Field */
#define AR_PFIELD    10 /* Mobile; can't be "pushed" */
#define AR_GLOBE     11
#define AR_BREATH    12 /* As Beam, but different message */
#define AR_GRADIENT  13 /* For Web, Magma Burst, etc. */
#define AR_POISON    14
#define AR_DISEASE   15
#define AR_CHAIN     16 /* Chain Lightning! */
#define AR_CONE      17 

/* Color Sets for Effect Overlays */
#define COL_FIRE     16
#define COL_COLD     17
#define COL_MAGIC    18
#define COL_NECRO    19
#define COL_ELEC     20
#define COL_RAINBOW  21 /* Color Spray, Prismatic X */
#define COL_LIGHT    22
#define COL_POISON   23
#define COL_ACID     24
#define COL_DISINT   25
#define COL_PETRI    26
#define COL_POLY     27
#define COL_SONIC    28
#define COL_SILVERY  29

/* Field types are sorted in order of glyph
   precedance, so a square in fields 6 and 9
   will be drawn as per the glyph for 9. */

#define FI_ITERRAIN   0x00000001
#define FI_MOBILE     0x00000002 /* moves with Creator */
#define FI_MODIFIER   0x00000004 /* applies an effect */
#define FI_CONTINUAL  0x00000008 /* Repeat effect eID every turn */
#define FI_ANTIMAG    0x00000010 /* Antimagic Shell */
#define FI_BLOCKER    0x00000020 /* Antianimal Shell, etc. */
#define FI_BAD        0x00000040 
#define FI_LIGHT      0x00000080
#define FI_FOG        0x00000100
#define FI_SHADOW     0x00000200
#define FI_CONFLICT   0x00000400
#define FI_ALT_MAGIC  0x00000800
#define FI_SILENCE    0x00001000
#define FI_KNOWN      0x00002000
#define FI_OBVIOUS    0x00004000
#define FI_SIZE       0x00008000
#define FI_DARKNESS   0x00010000

#define POR_UP_STAIR   1
#define POR_DOWN_STAIR 2
#define POR_STORE      3
#define POR_BUILDING   4
#define POR_GUILD      5
#define POR_POCKET     6 /* Rope Trick */
#define POR_DUN_ENTRY  7
#define POR_SUBDUNGEON 8
#define POR_TOWN       9
#define POR_RETURN     10

#define ACQ_ACQUIRE    1
#define ACQ_IDENT_ALL  2
#define ACQ_ANY_ITEM   3 
#define ACQ_CRAFT      4
#define ACQ_KNOWN      5
#define ACQ_MINOR      6
#define ACQ_MAJOR      7
#define ACQ_MONSTER    10
#define ACQ_SUMMON     10
#define ACQ_POLYMORPH  11
#define ACQ_GENOCIDE   12
#define ACQ_ANYMON     13
#define ACQ_RECALL     14

#define MENU_DESC         0x0001
#define MENU_ESC          0x0002
#define MENU_HELP         0x0004
#define MENU_2COLS        0x0008
#define MENU_3COLS        0x0010
#define MENU_4COLS        0x0020
#define MENU_RAW          0x0040
#define MENU_SORTED       0x0080
#define MENU_BORDER       0x0100
#define MENU_SWRAPWRITE   0x0200
#define MENU_LARGEBOX     0x0400
#define MENU_QKEY         0x0800
#define MENU_WIDE         0x1000
#define MENU_REDRAW       0x2000

#define BLOATED   3000
#define SATIATED  2500    
#define CONTENT   2000
#define PECKISH   1500
#define HUNGRY    1000 
#define STARVING   480
#define WEAK       240
#define FAINTING   120
#define STARVED      0

#define SS_ITEM  1  /* Permanant while worn */
#define SS_ENCH  2  /* Potion, Scroll or Spell */
#define SS_RACE  3  /* From your race (possibly racial knowledge) */
#define SS_CLAS  4  /* From your class */
#define SS_PERM  5  /* From some type of special effect. */
#define SS_ATTK  6  /* From being attacked (STUNNED, etc) */
#define SS_MISC  7  /* Class Ability Effects, etc. */
#define SS_ONCE  8  /* One-use Enchantment (Truestrike) */
#define SS_MONI  9  /* Tactical Stati from Monster AI */
#define SS_DOMA  10 /* From a Domain */
#define SS_SONG  11 /* Bardic Music */
#define SS_CURS  12 /* Non-Dispellable; needs Remove Curse */
#define SS_BLES  13 /* Divine Blessings, was 
                       SS_SHLD -- ww: tower shield in CalcValues() */
#define SS_BODY  14 /* from your current body (polymorph/race) */
#define SS_TMPL  15 /* from your current template */
                         /* long sword        kobold         Asmodeus         */
#define NA_LONG  0x0001  /* uncursed long...  injured kobold healthy Asmodeus */
#define NA_MECH  0x0002  /* long sword +1     kobold         Asmodeus         */
#define NA_CAPS  0x0004  /* Long swords       Kobolds        Asmodeus         */
#define NA_POSS  0x0008  /* long sword's      kobold's       Asmodeus'        */
#define NA_A     0x0010  /* a long sword      a kobold       Asmodeus         */
#define NA_THE   0x0020  /* the long sword    the kobold     Asmodeus         */
#define NA_INSC   0x0040 /* sword {blinky}     kobold          Asmodeus       */
#define NA_SINGLE 0x0080 /* The flaming sheaf arrow inflicts                  */
#define NA_SHADOW 0x0100 /* an unclearly seen monsterous form                 */
#define NA_STATI  0x0200 /* A terrified paralyzed male kobold                 */
#define NA_XCAPS  0x0400 /* The Long Sword [2d4/4d5]                          */
#define NA_MONO   0x0800 /* Monochrome (don't highlight blessed/cursed)       */
#define NA_IDENT  0x1000 /* pretend it is fully identified                    */
#define NA_NO_TERSE  0x2000 /* spell it all out */
#define NA_FLAVOR 0x4000

#define MT_TOWN  1
#define MT_DUNGEON 2
#define MT_BUILDING 3
#define MT_WILDERNESS 4
#define MT_ENCOUNTER 5
#define MT_ROOM 6
#define MT_VAULT 7

#define FOA_ALLOW_WATER   0x0001
#define FOA_WATER_ONLY    0x0002
#define FOA_ALLOW_SPEC    0x0004
#define FOA_SPEC_ONLY     0x0008
#define FOA_SOLID_ONLY    0x0010
#define FOA_DEEP_ONLY     0x0020
#define FOA_NO_TREES      0x0040
#define FOA_TREES_ONLY    0x0080
#define FOA_FLOOR_ONLY    0x0100
#define FOA_ALLOW_FALL    0x0200
#define FOA_FALL_ONLY     0x0400
#define FOA_ALLOW_WARN    0x0800
#define FOA_WARN_ONLY     0x1000

#define RM_WEIGHTS            1
#define VAULT_WEIGHTS         2
#define RC_WEIGHTS            3
#define ROOM_WEIGHTS          4
#define STREAMER_WEIGHTS      5
#define FURNISHINGS           6
#define WALL_COLORS           7
#define FLOOR_COLORS          8
#define MONSTER_LIST          9
#define MTYPE_LIST            10
#define CORRIDOR_WEIGHTS      11

#define ITEM_QUALITIES        12
#define GRANTED_SPELLS        13 
#define SPELLS_BY_CHARGE      14 /* i.e., staff of the magi */
#define BASE_ITEMS            15 /* different possible base items */
#define BANE_LIST             17
#define STAFF_SPELL_LIST      18

#define PALADIN_MOUNTS        19
#define BLACKGUARD_MOUNTS     20
#define SPELL_LIST            21
  #define ALL_SPELLS_FROM_SOURCE(ai_const) (ai_const + 1000)
  #define ALL_SPELLS_FROM_SCHOOL(src,school) 100 (src) (school)
#define SUBEFFECTS            22
#define SKILL_KIT_FOR         23
#define SECONDARY_KIT_FOR     24
#define ITEM_COST             25

#define SACRIFICE_LIST        26
#define FAVOR_CHART           27
#define GODSPEAK_LIST         28
#define CROWNING_GIFTS        29
#define AID_CHART             30
#define GOD_RELATIONS         31
  #define REL_DEFAULT         (-20)
#define FAVORED_SKILLS        32
#define ALLOWED_GODS          33


#define GROUP_LEADER          100
#define GROUP_FOLLOWER        101
#define GROUP_MOUNT           102
#define GROUP_ALLY            103
#define GROUP_PET             104

#define LEVEL_SIZEX           1
#define LEVEL_SIZEY           2
#define PANEL_SIZEX           3
#define PANEL_SIZEY           4
#define MAX_STREAMER_WIDTH    5
#define MIN_STREAMERS         6
#define MAX_STREAMERS         7
#define TERRAIN_MAPEDGE       8
#define TERRAIN_ROCK          9
#define TERRAIN_TORCH         10
#define BASE_REGION           11
#define DIGGING_REGION        12
#define TERRAFORM_REGION      13 /* Rock to Mud/Disint */
#define REDUCE_CHASM_CHANCE   14
#define MIN_STAIRS            15
#define MAX_STAIRS            16
#define ROOM_MINX             17
#define ROOM_MAXX             18
#define ROOM_MINY             19
#define ROOM_MAXY             20
#define TURN_CHANCE           21
#define STUBBORN_CORRIDOR     22
#define SEGMENT_MINLEN        23
#define SEGMENT_MAXLEN        24
#define STREAMER_CHANCE       25 

#define MONSTER_COUNT         26
#define MONSTER_FREQ          27
#define REST_SAFETY           28
#define DEAD_MAGIC            29
#define WILD_MAGIC            30
#define DESTROYED_LEV         34
#define NATURAL_LEV           35
#define VAULT_CHANCE          36
#define GVAULT_CHANCE         37
#define MAX_ARTIFACTS         38
#define MIN_ARTIFACTS         39
#define ARTI_LEVEL            40
#define ARTI_CHANCE           41
#define ARTI_LIST             42
#define ARTI_REQUIRED         43
#define DUN_DEPTH             44
#define INITIAL_CR            45
#define DUN_SPEED             46
#define SUBMAP_CHANCE         47
#define MAX_SUBMAPS           48
#define CORRIDOR_REGION       49
#define STAIRS_UP             50
#define STAIRS_DOWN           51
#define LIFE_PERCENT          52
#define LIFE_ITERATIONS       53
#define TERRAIN_PILLAR        54
#define ABOVE_DUNGEON         55
#define BELOW_DUNGEON         56
#define FIRST_LEV_PORTAL      57
#define LAST_LEV_PORTAL       58
#define PATRON_DIETY          59
#define MOOD_STRINGS          60
#define BLOB_WITH             61
#define MIN_CHASM_DEPTH       62
#define MIN_RIVER_DEPTH       63
#define TREASURE_CHANCE       64
#define CHEST_CHANCE          65
#define CHEST_MIN_ITEMS       66
#define CHEST_MAX_ITEMS       67
#define CURSED_CHANCE         68
#define STAPLE_CHANCE         69
#define STREAMER_MON_DENSITY  70
#define MONSTER_EQUILIBRIUM_BASE 71
#define MONSTER_EQUILIBRIUM_INC 72
#define TRAP_CHANCE           73
#define MIN_VAULT_DEPTH       74
#define PSYCHIC_TRAUMA        75
#define ROOM_LIT_CHANCE       76
#define LIT_CHANCE_DEPTH_MOD  77
#define TORCH_DENSITY         78
#define BIG_CORRIDOR_REGION   79


/* Constants for other resources */
#define BASE_ITEM             101
#define INITIAL_PLUS          102
#define WIELD_AS              103
#define APPEAR_AS             104
#define SKILL_KIT_MOD         107
#define SPAWN_TYPE            108
#define SERVICE_SPELL_COST    109
#define DEPOSIT_DEPTH         110
#define ROPE_MOD              111
#define TOTAL_CLASS_LEVELS    112
#define RES_SURVIVAL_CHANCE   113
#define MIN_CRAFT_LEVEL       114

#define VOICE_COLOR           120
#define ALTAR_COLOR           121
#define INTERVENTION_COST     122
#define RESSURECTION_COST     123
#define PRAYER_TIMEOUT        124
#define MIN_RAISE_LEVEL       125
#define MIN_CONVERT_FAVOR     126
#define PRAYER_DC             127
#define LEVEL_BLEEDOFF        128
#define TOLERANCE_VAL         129
#define HOLY_SYMBOL           130
#define ALTAR_COST            131
#define GODPULSE_INTERVAL     132
#define GODANGER_INTERVAL     133
#define LAY_MULTIPLIER        134
#define CHOSEN_WEAPON         135
#define CHOSEN_WEAPON_QUALITY 136
#define CHOSEN_ARMOR_QUALITY  137
#define CHOSEN_SHIELD_QUALITY 138
#define PERSONAL_ALIGN        139
#define STICK_TYPE            140
#define DRAGON_FEAR_DC        141
#define LAST_DUNCONST         142


#define MSG_IS_ANGRY      1
#define MSG_IS_ANGRY2     2
#define MSG_PLEASED       3
#define MSG_PLEASED2      4
#define MSG_PLEASED3      5
#define MSG_PROVE_WORTH   6
#define MSG_NOT_WORTHY    7
#define MSG_NOT_YOUR_KILL 8
#define MSG_UNINTERESTED  9
#define MSG_BADSAC1      10
#define MSG_BADSAC2      11
#define MSG_BADSAC3      12
#define MSG_BADSAC4      13
#define MSG_BADSAC5      14
#define MSG_BADSAC6      15
#define MSG_BADSAC7      16
#define MSG_BADSAC8      17
#define MSG_BADSAC9      19
#define MSG_BADSAC10     20
#define MSG_INSUFFICIENT 21
#define MSG_UNEASY       22
#define MSG_IMPRESSED    23
#define MSG_SATISFIED    24
#define MSG_SANCTUARY    25
#define MSG_FORSAKEN     26
#define MSG_READING      27
#define MSG_NO_INSIGHT   28
#define MSG_NO_MORE      29
#define MSG_NEUTRAL      30
#define MSG_LESSENED     31
#define MSG_MOLLIFIED    32
#define MSG_SACRIFICE    33
#define MSG_SALUTATION   34
#define MSG_JEALOUSY     35
#define MSG_TIMEOUT      36
#define MSG_PRAYER       37
#define MSG_NOAID        38
#define MSG_OUT_OF_AID   39
#define MSG_NEARLY_OUT   40
#define MSG_CONVERT      41
#define MSG_SAFEPRAY     42
#define MSG_WAITPRAY     43
#define MSG_TOLERANT     44
#define MSG_FORSAKE      45
#define MSG_OFFER_RAISE  46
#define MSG_VUNEASY      47
#define MSG_BAD_PRAY     48
#define MSG_BLESSING1    50
#define MSG_BLESSING2    51
#define MSG_BLESSING3    52
#define MSG_BLESSING4    53
#define MSG_BLESSING5    54
#define MSG_BLESSING6    55
#define MSG_BLESSING7    56
#define MSG_BLESSING8    57
#define MSG_BLESSING9    58
#define MSG_BLESSING10   59
#define MSG_CROWNED      61
#define MSG_PENANCE      62
#define MSG_CUSTOM1      63
#define MSG_CUSTOM2      64
#define MSG_CUSTOM3      65
#define MSG_CUSTOM4      66
#define MSG_CUSTOM5      67
#define MSG_CUSTOM6      68
#define MSG_CUSTOM7      69
#define MSG_CUSTOM8      70
#define MSG_CUSTOM9      71
#define MSG_CUSTOM10     72
#define MSG_CUSTOM11     73
#define MSG_CUSTOM12     74
#define MSG_EXPENDED     75
#define MSG_ANGER        76
#define MSG_ANATHEMA     77
#define MSG_NO_AID       78
#define MSG_UNEASY_R     79
#define MSG_VUNEASY_R    80
#define MSG_AID          81

#define AID_HEAL         1  
#define AID_PURIFY       2  
#define AID_SMITE        3   
#define AID_UNCURSE      4
#define AID_TELEPORT     5
#define AID_REVEAL       6
#define AID_FEED         7
#define AID_REFRESH      8
#define AID_RESTORE      9
#define AID_IDENTIFY     10
#define AID_CURE         11
#define AID_CLARITY      12
#define AID_RESSURECT    13
#define AID_BERSERK      14
#define AID_NEWBOOK      15
#define AID_MANA         16
#define AID_DEFLECT      17

#define SAC_ANGRY        (-100)
#define SAC_ABOMINATION  (-101)
#define SAC_DEFAULT      (-102)
#define SAC_UNWORTHY     (-103)    

#define GS_INVOLVED      0x0001
#define GS_FORSAKEN      0x0002
#define GS_ABANDONED     0x0004
#define GS_CROWNED       0x0008
#define GS_PENANCE       0x0010
#define GS_ANATHEMA      0x0020
#define GS_KNOWN_ANGER   0x0040

#define TROUBLE_INJURY     1
#define TROUBLE_LOWMANA    2
#define TROUBLE_HUNGER     3
#define TROUBLE_BLIND      4
#define TROUBLE_DISEASE    5
#define TROUBLE_SICK       6
#define TROUBLE_STONING    7
#define TROUBLE_DRAINED    8
#define TROUBLE_CURSED     9
#define TROUBLE_BLEEDING   10
#define TROUBLE_FEAR       11
#define TROUBLE_POISON     12
#define TROUBLE_FATIGUE    13
#define TROUBLE_NOBOOK     14 /* Missing Spellbooks */
#define TROUBLE_ENEMIES    15
#define TROUBLE_PARALYSIS  16
#define TROUBLE_HAUNTED    17

/* TROUBLE_NOBOOK occurs when you have a known arcane spell
   that you do not have a spellbook for; you god will replace
   the missing book. */



#define FU_SPLATTER           1
#define FU_TRAIL              2
#define FU_LIFEBLOB           3
#define FU_GRID               4
#define FU_COLUMNS            5
#define FU_CENTER             6
#define FU_RANDOM             7
#define FU_TABLE              8
#define FU_BED                9
#define FU_CORNERS            10   
#define FU_XCORNERS           11
#define FU_SPACED_COLUMNS            12
#define FU_CENTER_CIRCLE_SMALL   13
#define FU_CENTER_CIRCLE_MEDIUM  14
#define FU_CENTER_CIRCLE_LARGE   15
#define FU_INNER_COLUMNS      16

/* Flags for the starting gear lists
   given to monsters, races, classes
   and maps */
#define GF_RANDOM_ITEM        0x01
#define GF_RANDOM_EFFECT      0x02
#define GF_BLESSED            0x04
#define GF_CURSED             0x08
#define GF_ELSE               0x10

/* general tile flags */
#define TILE_ITEM             0x01
#define TILE_MONSTER          0x02
#define TILE_RANDOM           0x04

/* item tile flags */
#define TILE_RANDOM_EFF       0x08
#define TILE_BLESSED          0x10
#define TILE_CURSED           0x20
#define TILE_MASTERWORK       0x40
#define TILE_QUALITY          0x80

/* How to do these...? */
#define TILE_PLUS1            0x0100
#define TILE_PLUS2            0x0200
#define TILE_PLUS4            0x0400
#define TILE_KNOWN            0x0800

/* monster tile flags */
#define TILE_PEACEFUL         0x08
#define TILE_HOSTILE          0x10
#define TILE_ASLEEP           0x20
#define TILE_GUARDING         0x40
#define TILE_NO_PARTY         0x80

/* feature tile flags */
#define TILE_START            0x08
#define TILE_HIDDEN           0x10
#define TILE_LOCKED           0x20
#define TILE_OPEN             0x40
#define TILE_STUCK            0x80


#define thisp ((Player*)this)
#define thisc ((Character*)this)
#define thism ((Monster*)this)
#define thisi ((Item*)this)
#define thisw ((Weapon*)this)
#define thisa ((Armor*)this)

#define INP_LINE 1
#define INP_CHAR 2

#define WIN_SCREEN   0
#define WIN_MAP      1
#define WIN_TRAITS   2
#define WIN_STATUS   3
#define WIN_MESSAGE  4
#define WIN_DEBUG    5
#define WIN_INVEN    6
#define WIN_INPUT2   7
#define WIN_UPPER    8
#define WIN_LOWER    9
#define WIN_CSHEET   10
#define WIN_STORE    11
#define WIN_SPELLS   12
#define WIN_INPUT    13
#define WIN_MENUBOX  14
#define WIN_MENUDESC 15
#define WIN_CUSTOM   16
#define WIN_CREATION 17
#define WIN_VIEWLIST 18
#define WIN_NUMBERS  19
#define WIN_NUMBERS2 20
#define WIN_NUMBERS3 21
#define WIN_NUMBERS4 22
#define WIN_BIGMAP   23
#define WIN_OPTIONS  24
#define WIN_MESSAGE2 25
#define WIN_LAST     26

#define QKY_SPELL    1
#define QKY_ITEM     2
#define QKY_TACTIC   3
#define QKY_VERB     4
#define QKY_SKILL    5

#define MAX_QKEYS     40
//#define MAX_MACROS    48
#define MAX_MACROS    12
#define MAX_PATH_LENGTH 1024



#define OPC_CHARGEN             100
#define OPT_BEGINKIT            101
#define OPT_STATS               102
#define OPT_REROLL              103
#define OPT_DISALLOW            104
#define OPT_MAX_HP              105
#define OPT_MAX_MANA            106
#define OPT_GENDER              107
#define OPT_MON_MAX_HP          108
#define OPT_ATTR_FIRST          109
#define OPT_FAVORED_CLASS       110
#define OPT_ATTR_ON_LEVEL       111
#define OPT_SUBRACES            112
#define OPT_EASY_INTUIT         113
#define OPT_MON_DJIKSTRA        114
#define OPT_OOD_MONSTERS        115
#define OPT_TRANSGRESS_HINTS    116
#define OPT_DIFFICULTY          117
#define   DIFF_EXPLORE          0
#define   DIFF_TRAINING         1
#define   DIFF_BASELINE         2
#define   DIFF_CHALLENGE        3
#define   DIFF_NIGHTMARE        4
#define OPT_POWER_STATS         118
#define OPT_ELUDE_DEATH         119
#define OPT_NO_ALLY_TRANS       120       

// what you have to input
#define OPC_INPUT               200
#define OPT_AUTOMORE            201
#define OPT_ROGUELIKE           202
#define OPT_SHIFTARROW          203
#define OPT_AUTOPICKUP          204
#define OPT_AUTODISARM          205
#define OPT_AUTOOPEN            206
#define OPT_AUTOCHEST           207
#define OPT_AUTOKNOCK           208
#define OPT_AUTOKICK            209
#define OPT_REPEAT_KICK         210
#define OPT_KILL_CHEST          211
#define OPT_AUTO_COUP           212
#define OPT_AUTO_STAND          213
#define OPT_DWARVEN_AUTOFOCUS   214
#define OPT_BUMP_DOOR           215
#define OPT_CONTEXT_HELP        216
#define OPT_HELP_MODE           217
#define OPT_SORT_SPELLS         218
#define OPT_SHOW_ALL_SPELLS     219
#define OPT_MONO_HELP           220
#define OPT_SCROLL_MAG          221
#define OPT_SELECT_JUMP         222
#define OPT_SELECT_DIR          223
#define OPT_KEYBOARD            224
#define OPT_CLEAR_EVERY_TURN    225
#define OPT_ORG_CONTAIN         226
#define OPT_CONTAINER_CURSOR    227 

// running stops where? 
#define OPC_LIMITS              300
#define OPT_STOP_CORNER         301
#define OPT_STOP_STAIRS         302
#define OPT_STOP_INTER          303
#define OPT_STOP_REGION         304
#define OPT_STOP_PHASE          305
#define OPT_REST_BEHAVE         306
#define OPT_LOWHP_WARN          307
#define OPT_LOWHP_AGG           308
#define OPT_WARN_EMPTY_HAND     309
#define OPT_WARN_DEQU           310
#define OPT_WARN_DROP           311
#define OPT_HIGH_HIDE           350
#define OPT_HIGH_FRIEND         351
#define OPT_HIGH_SLEEP          352
#define OPT_HIGH_MALADY         353
#define OPT_HIGH_INVIS          354

// what you see
#define OPC_DISPLAY             400
#define OPT_TERSE_BLESSED       401
#define OPT_ANIMATION           402
#define OPT_CENTER_MAP          403
#define OPT_LOWERSCREEN         404
#define OPT_SIDEBAR             405
#define OPT_PRECISE_TIME        406
#define OPT_DESC_ROOM           407
#define OPT_SHOW_HOW_SEE        408
#define OPT_SHOW_PILES          409
#define OPT_SORT_PILE           410
#define OPT_PLAYER_COL          412
#define OPT_SHOWSAVES           418
#define OPT_STORE_ROLLS         419
#define OPT_MESSPARA            420
#define OPT_LIST_PILE           421
#define OPT_SHOWPATTK           422 
#define OPT_SHOWMATTK           423
#define OPT_SHOWSKILLS          424
#define OPT_EFF_PROMPT          425
#define OPT_SHOW_OFFS           426
#define OPT_WARN_TERRAIN        427
#define OPT_WARN_DARKNESS       428
#define OPT_WARN_BRIGHT         429
#define OPT_FULL_RES            430
#define OPT_FULL_FONT           431
#define OPT_WIND_RES            432
#define OPT_WIND_FONT           433
#define OPT_SOFT_PALETTE        434


#define OPC_TACTICAL            500
#define OPT_SUBDUAL             501
#define OPT_BUMP                502
#define OPT_OPPUR               503
#define OPT_KICK                504
#define OPT_SHIFT_ATTK          505
#define OPT_CTRL_ATTK           506
#define OPT_TWO_WEP             507
//#define OPT_AUTO_COUP           508
#define OPT_RIPOSTE_MODE        509
#define OPT_WAIT_PETS           510 /* Wait for Group (PC Move Rate = lowest in group) */
//#define OPT_DWARVEN_AUTOFOCUS   511
//#define OPT_AUTO_STAND          512
#define OPT_MONK_MODE           513
#define OPT_OVERCAST            514
#define OPT_WAIT_FOR_PETS       515
#define OPT_COUNTERSPELL        516
#define OPT_POISON_MERGE        517
#define OPT_AUTOHIDE            518
#define OPT_DEMORALIZE          519
#define OPT_WAND_OPENING        520
#define OPT_USE_GAZE            521

#define OPC_STABILITY           600
#define OPT_NO_FREE             601
#define OPT_NO_SCRIPTS          602
#define OPT_NO_CASTING          603

#define OPC_MACROS              700

#define OPC_WIZARD              800
#define OPT_NODEATH             801
#define OPT_AUTOID              802
#define OPT_FREE_ADV            803
#define OPT_NO_FATIGUE          804
#define OPT_SAFEREST            805
#define OPT_NO_SPELLFAIL        806
#define OPT_ALL_SPELLS          807
#define OPT_INF_MANA            808
#define OPT_GHOSTWALK           809
#define OPT_WIZ_SIGHT           810
#define OPT_FREEZE_MON          811
#define OPT_ALL_ROLLS           812

#define OPT_LAST                900

#define GLYPH_PERSON    6
#define GLYPH_BULK      197
#define GLYPH_WATER     247
#define GLYPH_FOG       '~'
#define GLYPH_ICE       254
#define GLYPH_WEB       '#'
#define GLYPH_LAVA      247
#define GLYPH_BLAST     15
#define GLYPH_WALL      177
#define GLYPH_ROCK      176
#define GLYPH_SOLID     219
#define GLYPH_PILLAR    9
#define GLYPH_VIEWPOINT 9
#define GLYPH_GRAVE     239
#define GLYPH_SHELF     216
                       
#define GLYPH_RING      235
#define GLYPH_POTION    173
#define GLYPH_SCROLL    168
#define GLYPH_AMULET    11
#define GLYPH_WEAPON    251
#define GLYPH_ROD       '|'
#define GLYPH_STAFF     '/'
#define GLYPH_WAND      '-'
#define GLYPH_FOOD      '%'
#define GLYPH_CORPSE    '%'
#define GLYPH_BOOK      254
#define GLYPH_TORCH     '}'
#define GLYPH_LARMOR    228
#define GLYPH_MARMOR    228
#define GLYPH_HARMOR    228
#define GLYPH_SHIELD    ']'
#define GLYPH_GAUNTLETS 229
#define GLYPH_HELMET    155
#define GLYPH_HEADBAND  155
#define GLYPH_BOOTS     224
#define GLYPH_BRACERS   224
#define GLYPH_GIRDLE    226
#define GLYPH_CLOTHES   20
#define GLYPH_CONTAIN   127
#define GLYPH_CROWN     '^'
#define GLYPH_DUST      240
#define GLYPH_DECK      240
#define GLYPH_RING      235
#define GLYPH_AMULET    11
#define GLYPH_FIGURE    156
#define GLYPH_HORN      '&'
#define GLYPH_EYES      236
#define GLYPH_HERB      '\"'
#define GLYPH_MUSH      227
#define GLYPH_GEM       4
#define GLYPH_COIN      '$'
#define GLYPH_TOOL      234
#define GLYPH_SWORD     '('
#define GLYPH_BOW       ')'
#define GLYPH_JUNK      '&'
#define GLYPH_CHEST     127
/*#define GLYPH_TRAP      23 or 146 */
#define GLYPH_CLOAK      6
#define GLYPH_STATUE     5
#define GLYPH_PILE      '*'
#define GLYPH_MULTI     146
#define GLYPH_ALTAR     '8'
#define GLYPH_FOUNTAIN  244
#define GLYPH_THRONE    190
#define GLYPH_SYMBOL    237

#define GLYPH_HEDGE     34
#define GLYPH_RUBBLE    ':'
#define GLYPH_BRIDGE    '='
#define GLYPH_FLOOR     250
#define GLYPH_FLOOR2    249
#define GLYPH_VDOOR     179
#define GLYPH_HDOOR     196
#define GLYPH_ODOOR     43
#define GLYPH_BDOOR     241
#define GLYPH_PIT       '0'
#define GLYPH_PORTAL    240

#define GLYPH_STORE         21
#define GLYPH_GUILD         20
#define GLYPH_STORE_VWALL  186
#define GLYPH_STORE_HWALL  205
#define GLYPH_STORE_CORNER 219

#define GLYPH_USTAIRS   '<'
#define GLYPH_DSTAIRS   '>'
#define GLYPH_TREE      157
#define GLYPH_CHECK     251
#define GLYPH_FURNATURE 254
#define GLYPH_UNKNOWN   63
#define GLYPH_TRASH     '&'
#define GLYPH_BONES     '&'
#define GLYPH_CHECKMARK 251

#define GLYPH_TRAP      232
#define GLYPH_DISARMED  246

#define GLYPH_ARROW      24
#define GLYPH_ARROW_UP   24
#define GLYPH_ARROW_DOWN 25
#define GLYPH_ARROW_RIGHT 26
#define GLYPH_ARROW_LEFT  27

#define GLYPH_GUARD      140
#define GLYPH_TOWNIE     139
#define GLYPH_TOWNSCUM   141
#define GLYPH_TOWN_NPC   'i'
#define GLYPH_LDEMON     151
#define GLYPH_GDEMON     154
#define GLYPH_LDEVIL     148
#define GLYPH_GDEVIL     153
#define GLYPH_FIEND      152

#define GLYPH_PLAYER      '@'
#define GLYPH_HUMAN       1
#define GLYPH_ELF         132
#define GLYPH_DWARF       131
#define GLYPH_GNOME       133
#define GLYPH_HOBBIT      133

#define PRIO_EMPTY               1
#define PRIO_CORRIDOR_WALL      10
#define PRIO_ROOM_WALL          20
#define PRIO_ROOM_WALL_TORCH    30
#define PRIO_ROOM_WALL_MODIFIER 40
#define PRIO_PILLARS            50
#define PRIO_ROCK_STREAMER      60
#define PRIO_DEPOSIT            65
#define PRIO_ROOM_FLOOR         70
#define PRIO_ROOM_FURNITURE     80
#define PRIO_RIVER_STREAMER     90
#define PRIO_VAULT              100
#define PRIO_FEATURE_FLOOR      110
#define PRIO_MAX                120

#define BLACK   0
#define BLUE    1
#define GREEN   2
#define CYAN    3
#define RED     4
#define PURPLE  5
#define BROWN   6
#define GREY    7
#define SHADOW  8
#define AZURE   9
#define EMERALD 10
#define SKYBLUE 11
#define PINK    12
#define MAGENTA 13
#define YELLOW  14
#define WHITE   15

#define LITERAL_CHAR -20
#define WRAP_BREAK   -21
#define BACK_COLOR   -22
#define WRAP_INDENT  -23


#define DARK_MASK(a) (a & ~0x08)
#define BRIGHT_MASK(a) (a | 0x08)
#define ATTR_MASK(a,b) (a + b*256)


#define MAX_DUNGEONS       32
#define MAX_DUNGEON_LEVELS 50
#define MAX_MODULES        126
#define MAX_WILDER_GRIDX   16
#define MAX_WILDER_GRIDY   16
#define MAX_SPELLS         2048
#define MAX_GODS           25
#define MAX_CHAR_LEVEL     11
#define MAX_CORRIDOR_LENGTH 8192
#define ANNOT_INITIAL_SIZE 50
#define MAX_SAC_CATS       10


#define MALE_NAMES 1
#define FEMALE_NAMES 2
#define FAMILY_NAMES 3

#define ENOFILE   1 /* File not found */
#define ENOTARC   2 /* File not an archive */
#define EBADVER   3 /* File versions don't match */
#define ENOCHUNK  4 /* Chunk not found */
#define EWRIERR   5 /* Error with fwrite */
#define EREADERR  6 /* Error with fread */
#define EMEMORY   7 /* Out of Memory */
#define ECORRUPT  8  /* File corrupt */
#define ETAMPER   9  /* File tampering detected */
#define EHANDLE   10 /* Bad Object Handle */
#define ENODIR    11 /* Directory not Found */

#define SAVE_NEW  1
#define SAVE_ADD  2

#define PRE(a)     (a+500)
#define POST(a)    (a+1000)
#define EVICTIM(a) (a+2000)
#define ETARGET(a) (a+2000)
#define EITEM(a)   (a+4000)
#define META(a)  (a+10000)
#define GODWATCH(a) (a+20000)

#define EV_MOVE         1
#define EV_BREAK        2
#define EV_PUSH         3
#define EV_PULL         4
#define EV_MIX          5
#define EV_POUR         6
#define EV_ZAP          7
#define EV_ATTK         11
#define EV_PICKUP       12
#define EV_WIELD        13
#define EV_PUTON        13
#define EV_WEAR         13
#define EV_INSERT       14
#define EV_ACTIVATE     15
#define EV_DROP          16
#define EV_NATTACK      17 /*Natural Attack Sequence*/
#define EV_WATTACK      18 /*Character Attack Sequence*/
#define EV_STRIKE        19
#define EV_HIT          20
#define EV_MISS          21
#define EV_CRIT           22
#define EV_FUMBLE        23
#define EV_BLOCK        24
#define EV_PARRY        25
#define EV_DODGE        26
#define EV_ATTACKMSG    27
#define EV_MAGIC_HIT    28
#define EV_OPEN         29
#define EV_CLOSE        30
#define EV_REMOVE       31
#define EV_TAKEOFF      31
#define EV_TURN         32
#define EV_SUNRISE      33
#define EV_SUNSET       34
#define EV_DRINK        35
#define EV_INVOKE       36
#define EV_CAST         37
#define EV_READ         38
#define EV_UNLOCK       39
#define EV_ENTERSTORE   40
#define EV_DAMAGE       41
#define EV_DEATH        42
#define EV_FIELDON      43
#define EV_FIELDOFF     44
#define EV_REST         45
#define EV_RESTING      46
#define EV_WALKON       47      
#define EV_RATTACK      48
#define EV_SATTACK      49
#define EV_ENTER        50
#define EV_TAKEOUT      51
#define EV_EFFECT       52
#define EV_EAT          53
#define EV_LOSEITEM     54
#define EV_TALK         55
#define EV_BARTER       56
#define EV_COW          57
#define EV_DISMISS      58
#define EV_DISTRACT     59
#define EV_ENLIST       60
#define EV_FAST_TALK    61
#define EV_GREET        62
#define EV_ORDER        63
#define EV_QUELL        64
#define EV_REQUEST      65
#define EV_SURRENDER    66
#define EV_TAUNT        67
#define EV_OATTACK      71
#define EV_SHIFT        72
#define EV_HIDE         73
#define EV_ELAPSED      74
#define EV_MAGIC_XY     75
#define EV_LOADXBOW     76
#define EV_INFECT       77
#define EV_POISON       78
#define EV_TURNING      80
#define EV_JUMP         81
#define EV_PHASE        82
#define EV_MAGIC_STRIKE 83
#define EV_REMOVED      84
#define EV_SEARCH       85
#define EV_VICTORY      87
#define EV_LEAVE        88
#define EV_ASCEND       89
#define EV_DESCEND      90
#define EV_DIG          91
#define EV_RUB          92
#define EV_PLAY         93
#define EV_TWIST        94
#define EV_APPLY        95
#define EV_INSCRIBE     96
#define EV_JAM          97
#define EV_EXAMINE      98
#define EV_DIP         100
#define EV_SIT         101
#define EV_GIVE        102
#define EV_SHOW        103
#define EV_WAVE        104
#define EV_CLEAN       105
#define EV_TASTE       106
#define EV_SMELL       108
#define EV_DISSECT     109
#define EV_MOUNT       110
#define EV_DISMOUNT    111
#define EV_PRESS       112
#define EV_BURN        113
#define EV_TIE_UP      114
#define EV_TIE_TO      115
#define EV_CUT         116
#define EV_DIVIDE      117
#define EV_IMBUE       118
#define EV_FORCE       119
#define EV_TOUCH       120
#define EV_PLACE       121
#define EV_INIT_COMP   122
#define EV_PDAMAGE     123
#define EV_ADAMAGE     124
#define EV_TDAMAGE     125
#define EV_DISPELLED   126
#define EV_ITERRAIN    127
#define EV_COMPULSION  128
#define EV_MINE        129
#define EV_DISGUISE    130
#define EV_RESEARCH    131
#define EV_PICK_POCKET 132
#define EV_BRIDGE      133
#define EV_PRAY        134
#define EV_SACRIFICE   135
#define EV_CONVERT     136
#define EV_CONVERT_ALTAR 137
#define EV_RETRIBUTION 138
#define EV_GODPULSE    139
#define EV_JEALOUSY    140
#define EV_CALC_FAVOR  141
#define EV_GIVE_AID    142
#define EV_RATE_SAC    143
#define EV_INSIGHT     144
#define EV_ALIGNED     145
#define EV_CHANGE_ALIGN 146
#define EV_BLESSING    147
#define EV_GOD_RAISE   148
#define EV_ANGER_PULSE 149

#define EV_BIRTH        150  /* Called for race/class at chargen */
#define EV_PREREQ       151  /* For (prestige) classes and psi powers */
#define EV_PRINT_NAME   152  /* Override Name() */
#define EV_ADVANCE      153
#define EV_SPECIAL      154  /* Terrain Effects */
#define EV_ISTARGET     155
#define EV_INITIALIZE   156
#define EV_RATETARG     157
#define EV_ISDETECT     158
#define EV_MISSILE      159
#define EV_MON_CONSIDER 161 /* should a monster go here? */
#define EV_GENITEM      162
#define EV_GETNAME      163
#define EV_INIT_FIELDS  164
#define EV_INIT_STATI   165
#define EV_LAND         166 /* after a jump */
#define EV_JUMP_OFF     167 /* before a jump:
                               this appears to be necessary because EV_JUMP
                               targets the destination square, so there's
                               no way to trap an event and say "you can't
                               jump out of deep water" */
#define EV_DISARM       168
#define EV_STARTING_ITEM 169
#define EV_CALC_EFFECT   170
#define EV_CRITERIA     171  /* Used with Term::ChooseResource */
#define EV_MACRO        172
#define EV_WILLBUY      173
#define EV_CONDITION    174  /* See sep_expr in grammar.acc */
#define EV_GAIN_STATI   175
#define EV_IBLESSING    176
#define EV_TERMS        177
#define EV_PICKLOCK     178

#define MSG_BLASTNAME   200  /* "fireball", "hail of burning stones" */
#define MSG_CHATTER     201 
#define MSG_INSULT      202
#define MSG_FEARFUL     203
#define MSG_ATTACK      204
#define MSG_STATINAME   205
#define MSG_CAST        206
#define MSG_ELAPSED     207
#define MSG_ABILITY     208
#define MSG_SPECABIL    209
#define MSG_SPE1DESC    210
#define MSG_SPE2DESC    211
#define MSG_SPE3DESC    212
#define MSG_ROARVERB    213
#define MSG_XSUMMON     214
#define MSG_SHOPNAME    215

#ifndef ICOMP

#define JUMP        1
#define HALT        2
#define ADD         3
#define SUB         4
#define MULT        5
#define DIV         6
#define MOD         7
#define MOV         8
#define BSHL        9
#define BSHR        10
#define JTRU        11
#define JFAL        12
#define CMEQ        13
#define CMNE        14
#define CMGT        15
#define CMLT        16
#define CMLE        17
#define CMGE        18
#define LOAD        19
#define NOT         20
#define NEG         21
#define BAND        22
#define BOR         23
#define RUN         24
#define RET         25
#define CALL        26
#define PUSH        27
#define POP         28
#define SYS         29
#define REPI        38  /* REPeat by Increment, stopping at v2 */
#define REPD        39  /* REPeat by Decrement, stopping at v2 */
#define REPN        40  /* REPeat until v1 != v2 */
#define REPE        41  /* REPeat until v1 == v2 */
#define CMEM        42
#define GVAR        43
#define SVAR        44
#define GOBJ        45
#define JTAB        46
#define SBRK        47
#define FBRK        48
#define JBRK        49
#define ROLL        50
#define INC         51
#define DEC         52
#define MIN         53
#define MAX         54
#define LAND        55
#define LOR         56
#define ASTR        57
#define MSTR        58
#define CSTR        59
#define WSTR        60
#define ESTR        61
#define CONT        62
#define LAST        63


#define MAX_OVERLAY_GLYPHS 250

#ifndef max
#define max(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a)>(b)) ? (b) : (a))
#endif

class Object;
class Map;
class Level;
class Thing;
class Creature;
class Charecter;
class Monster;
class NPC;
class Player;
class Magic;
class Attack;
class Item;
class Feature;
class Trap;
class Door;
class Inventory;
class Potion;
class Food;
class Corpse;
class Scroll;
class Book;
class Stick;
class Wand;
class Rod;
class Staff;
class Weapon;
class Armour;
class Shield;
class Helmet;
class Gauntlets;
class Cloak;
class Container;
class Girdle;
class Dust;
class Deck;
class Ring;
class Amulet;
class Figurine;
class Herb;
class Mushroom;
class Coin;
class Gem;
class Term;
class TextTerm;
class GUITerm;
class Quest;
class Game;
class Registry;
struct Annotation;
struct Field;
struct EventInfo;
struct LocationInfo;
struct VCode;
struct TextVal;
struct Dice;
struct EffectValues;
struct ItemGen;
#endif


