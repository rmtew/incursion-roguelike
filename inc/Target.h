/*
 * ww: Revised Incursion Targeting System ... 
 * Since Targeting is so important (e.g., we might want to attach special
 * debug information to targeting information) and since we scan though all
 * of our targets commonly, we have pulled Targets out of the Stati array.
 */

/* ww: Key Concept: "certifying"
 * We should always know why A thinks B is a target. In essence, we should
 * carry around a "proof". 
 *
 * Eventually, this "proof" can be used to give information to players when
 * they use SK_SENSE_MOTIVE (or somesuch). 
 */

/* ww: this would be a class, but disjoint unions are too annoying to do 
 * correctly in C++, and it would be a pain to unserialize later ...
 * actually, everything here used to be a class, but I gave up ... */
typedef enum {
    HostilityDefault,
    HostilityFeud,      // MA_ELF, MA_DROW
    HostilityMindless,  // M_MINDLESS
    HostilityPeaceful,  // MS_PEACEFUL
    HostilityGood,      // MA_GOOD on MA_GOOD
    HostilityOutsider,  // LE outsider on CG outsider, etc. 
    HostilityDragon,    // chromatic vs. metallic dragons
    HostilitySmite,     // Good Smites Evil!
    HostilityLeader,
    HostilityMount,
    HostilityDefendLeader,
    HostilityYourLeaderHatesMe,
    HostilityParty,
    HostilityFlag,      // M_HOSTILE
    HostilitySolidarity, // elves get along with elves, undead with undead
    HostilityEID,       // cloak of vermin friendship, aggravate monsters
    HostilityTarget,    // specific feelings about this one thing
    HostilityFood,      // carnivores eat adventurers ...
    HostilityEvil,      // evil creatures attack weaker things
    HostilityAlienation, // animals in a dungeon, elementals on the prime
    HostilityCharmed,
    HostilityCommanded,
} HostilityWhyType;

typedef struct HostilityWhy {
  HostilityWhyType type;
  union {
    struct {
      uint8 ma;
    } solidarity;
    struct {
      uint8 m1, m2;
    } feud;
    struct {
      rID eid; 
    } eid; 
  } data;
  String        & Dump();
} HostilityWhy;

typedef enum {
    Neutral,      // I will ignore you unless you hurt me
    Enemy,        // I will attack you now
    Ally          // I will cast healing spells on you, etc. 
} HostilityQual; 

typedef enum {
    Apathy        = 0,    // "I don't care": only for Neutral above
    Minimal       = 1,
    Tiny          = 2,    // evil things go after weaker things
    Weak          = 10,   // "feud" elves and dwarves
    Medium        = 20,   // "hatred" elves and drow, undead and living
    Strong        = 30,   // "loathing" aligned dragons, aligned outsiders
} HostilityQuant;

typedef struct Hostility {
  // ww: Hostility should only be between creatures. 
  HostilityQual         quality;
  HostilityQuant        quantity;
  HostilityWhy          why;
  String                & Dump(const char * toWhat);
} Hostility;

typedef enum {
    TargetDefault,
    TargetGreedy,
    TargetHitMe,
    TargetSpelledMe,
    TargetWasSeenAndHostile,
    TargetIsAllyOfEnemy,
    TargetHeardNoise,
    TargetWanderlust,
    TargetLiberated,
    TargetOrderedTo,
    TargetResents
} TargetWhyType;

typedef struct TargetWhy {
  TargetWhyType         type;
  uint32                turnOfBirth;
  union {
    struct {
      rID               spellID;
    } SpelledMe;
    struct {
      Hostility         h;
      uint16            per;
    } WasSeenAndHostile;
    struct {
      Hostility         hAlly;
      hObj              ally;
    } IsAllyOfEnemy;
  } data;
  String                & Dump(const char *toWhat);
  void                  Set(TargetWhyType _type);
} TargetWhy; 

typedef enum {
    TargetInvalid,  // should never been seen!

    TargetEnemy,
    TargetAlly,

    TargetLeader,   // Normal leader: this troll leads these goblins 
    TargetSummoner, // You cast Summon Monster II. Less likely to turn. 
    TargetMaster,   // Druid Animal (or somesuch) -- will never attack leader.
    TargetMount,    // Mount Obeys its rider

    TargetArea,
    TargetWander,

    TargetItem,

    OrderStandStill,
    OrderDoNotAttack,
    OrderAttackNeutrals,
    OrderHide,
    OrderDoNotHide,
    OrderWalkInFront,
    OrderWalkInBack,
    OrderWalkNearMe,
    OrderWalkToPoint,
    OrderBeFriendly,
    OrderAttackTarget,
    OrderTakeWatches,
    OrderGiveMeItem,
    OrderReturnHome,
    OrderNoPickup,
    TargetAny = -1
} TargetType;

typedef struct Target {
  TargetType    type;
  uint16        priority;
  uint16        vis;            // visible now? (as of last ChooseAction)
  TargetWhy     why;
  union {
    struct {
      hObj      c;
      uint32    damageDoneToMe;
    } Creature;
    struct {
      uint8     x,y;
    } Area;
    struct {
      hObj      i;
    } Item;
  } data; 
  String                & Dump();
  int16                 DistanceFrom(const Thing *me);
  uint8                 X();
  uint8                 Y();
  Thing *               GetThing();
  Thing *               GetThingOrNULL();
  bool                  isValid();
} Target; 

typedef struct TargetSystem {
public: 

#define NUM_TARGETS 32
  Target        t[NUM_TARGETS]; 
  uint8         tCount;

  // I would love to use your OArray, but I get a C++ link error I don't
  // understand:
  // OArray<Target,0,1> t;
  
  /* shouldRecalc is true if we should recalculate possible targets before
   * choosing our next action ... */
  bool          shouldRetarget;  

  /* RacialHostilty uses only MA_* things from the monster and the template
   * (and possibly also flags like M_EARTH) */
  Hostility     RacialHostility(Creature *me, Creature *t);       

  /* Specific Hostility also takes into account Stati and abilities and
   * other personal feelings (e.g., CHARMED, FRIEND, you hit me, partyID) */
  Hostility     SpecificHostility(Creature *me, Creature *t);

  /* RateAsTarget is used when you are seeing something for the first 
   * time and you're not sure if you like it or not. */
  void          RateAsTarget(Creature *me, Thing *t, Target &newT);

  /* GetTarget is used to see if you have a specific memory about this
   * thing: maybe you would normally be peaceful to it, but you remember
   * that it hit you! */ 
  Target *      GetTarget(Thing *thing);

  /* Mark the ith target as invalid. */
  void          RemoveTargetNumber(uint16 i);

  bool          isLeader(Creature *t);
  Creature *    getLeader(); 

  bool          hasTargetOfType(TargetType type);
  Target *      getTargetOfType(TargetType type);
  bool          hasTargetThing(Thing *t);

  /* gain a new target explicitly ... do not use these if you
   * can possibly help it! */
  bool          addCreatureTarget(Creature *targ, TargetType type);
  bool          giveOrder(Creature *me, Creature *master, TargetType order, Thing * victim=NULL, int x = -1, int y = -1);
  void          removeCreatureTarget(Creature *targ, TargetType type);
  bool          addTarget(Target &newT);

  /* 't' turned into something else out of sight, so we should forget
   * negative feelings we have for it */
  void          MissedShapechange(Creature *me, Creature *t);

  /* drop all current TargetWanders, add a TargetWander to some random
   * location ... */
  void          Wanderlust(Creature *me, Thing *t=NULL);

  /* something just made a big noise and we heard it ... note that we may
   * not be able to see the noise-maker, and there might not even be one
   * (ventriloquism) */
  void          HearNoise(Creature *me, uint8 x, uint8 y);

  /* 't' just struck us! 't' may be an ally, at which point we should
   * shrug it off ...*/
  void          ItHitMe(Creature *me, Creature *t, int16 damage);

  /* We're a summoned creature and have just been liberated from our
   * summoner's control via the Liberation domain granted power.     */
  void          Liberate(Creature *me, Creature *lib);

  /* I needed to make this for Lesser Planar Binding... friendly creature
   * turns hostile for reasons other than ItHitMe. */
  void          TurnHostileTo(Creature *me, Creature *hostile_to);
  void          TurnNeutralTo(Creature *me, Creature *neutral_to);


  /* should not normally be called -- debugging only, etc. */
  void          Clear();

  /* (Animal Empathy) 't' Pacified us! */
  void          Pacify(Creature *me, Creature *t);

  /* 't' has just appeared on the map (AlertNew) or for some reason we want
   * to think about it again ... we might not be able to see it, and it 
   * might be an item, creature, etc. */
  void          Consider(Creature *me, Thing *t);

  /* Look at all of the things nearby and rebuild our target array. */
  void          Retarget(Creature *me, bool force = false); 

  void          ForgetOrders(Creature *me, int ofType = -1);

  String        & Dump();
  void          Serialize(Registry &r);
} TargetSystem;

