/*
 * ww: Revised Incursion Targeting System ... 
 */

#include "Incursion.h"

/**************************************************************************\
 * HostilityWhy
\**************************************************************************/
String & HostilityWhy::Dump()
{
  String s;
  switch (type) {
    case HostilityDefault: s = "for no particular reason"; break;
    case HostilityMindless: s = "because it is mindless"; break;
    case HostilityFlag: s = "because it is hostile by nature"; break;
    case HostilityPeaceful: s = "because it is peaceful"; break;
    case HostilityGood: s = "because they are both good"; break;
    case HostilitySmite: s = "because it fights against evil"; break;
    case HostilityOutsider: s = "because of the traditional conflict between the various outer planes"; break;
    case HostilityDragon: s = "because of the traditional conflict between good and evil dragons"; break;
    case HostilityParty: s = "because they are in the same party"; break;
    case HostilityLeader: s = "because it owes fealty"; break;
    case HostilityMount: s = "because it follows the guidance of its rider"; break;
    case HostilityDefendLeader: s = "because it protects its leader"; break;
    case HostilityYourLeaderHatesMe: 
          s = "because it fears malicious orders"; break;
    case HostilityEID: s = Format("because of %s",NAME(data.eid.eid)); break;
    case HostilityTarget: s = "because of personal feelings"; break; 
    case HostilityFood: s = "because it needs food"; break; 
    case HostilityEvil: s = "because it is evil"; break; 
    case HostilitySolidarity:
      s = Format("because of %s solidarity", 
          Lookup(MTypeNames,data.solidarity.ma));
      break;
    case HostilityAlienation: s = "because it has been taken from its natural "
                                  "home and is panicked and scared"; break;
    case HostilityFeud:
      s = Format("because of the traditional hatred between %s and %s", 
          (const char*)Pluralize(Lookup(MTypeNames,data.feud.m1)), 
          (const char*)Pluralize(Lookup(MTypeNames,data.feud.m2)));
      break;
    case HostilityCharmed:
      s = "because it has been charmed";
     break;
    case HostilityCommanded:
      s = "because it has been commanded";
     break;
  } 
  return *tmpstr(s);
}

/**************************************************************************\
 * Hostility
\**************************************************************************/
String & Hostility::Dump(const char * toWhat)
{
  String s, s2;

  if (quantity >= Strong)
    s = "strong ";
  else if (quantity >= Medium)
    s = "moderate ";
  else if (quantity >= Weak)
    s = "weak ";
  else if (quantity >= Minimal)
    s = "minimal ";
  else 
    s = "basic ";

  switch (quality) {
    case Neutral: s2 = "neutrality"; break;
    case Enemy:   s2 = "<12>enmity<7>"; break;
    case Ally:    s2 = "<10>allegiance<7>"; break; 
  }

  return *tmpstr(Format("it felt %s%s %s", (const char*)s, (const char*)s2, (const char*)why.Dump()));
}

/**************************************************************************\
 * TargetWhy
\**************************************************************************/
void TargetWhy::Set(TargetWhyType _type)
{
  type = _type;
  turnOfBirth = theGame->Turn;
} 

String & TargetWhy::Dump(const char * toWhat)
{
  String s;
  switch (type) {
    case TargetOrderedTo:
      s = "it was ordered to";
      break; 

    case TargetDefault:
      s = "of a whim";
      break;

    case TargetGreedy:
      s = "of greed";
      break;

    case TargetHitMe:
      s = "it was attacked";
      break; 

    case TargetLiberated:
      s = "it was freed by a Liberation cleric and turned on its summoner";
     break;

    case TargetHeardNoise:
      s = "it heard a noise";
      break; 

    case TargetWanderlust:
      s = "it was bored";
      break; 

    case TargetSpelledMe:
      s = Format("it was affected by %s",
          NAME(data.SpelledMe.spellID));
      break;

    case TargetWasSeenAndHostile:
      s = Format("since it saw it %d turns ago. At that time %s",
          theGame->Turn - turnOfBirth,
          (const char*)data.WasSeenAndHostile.h.Dump(toWhat));
      return *tmpstr(s); 

    case TargetIsAllyOfEnemy:
      {
        String ally, ally2;
        if (theRegistry->Exists(data.IsAllyOfEnemy.ally)) {
          ally = oCreature(data.IsAllyOfEnemy.ally)->Name(NA_THE);
          ally2 = ally;
        } else {
          ally = "a now-dead creature";
          ally2 = "that now-dead creature";
        } 
        s = Format("it is the ally of %s and %s",
            (const char*)ally,
            (const char*)data.IsAllyOfEnemy.hAlly.Dump(ally2));
        break; 
      } 
    case TargetResents:
      s = "it resents having been charmed";
     break;
  }
  return *tmpstr(Format("because %s %d turns ago", (const char*)s, 
      theGame->Turn - turnOfBirth));
}

/**************************************************************************\
 * Target
\**************************************************************************/
String & Target::Dump()
{
  String s;
  s = Format("With priority %d it ",priority);

  String enemy;
  switch (type) {
    case TargetEnemy:    enemy = "an <12>enemy<7>";  break;
    case TargetAlly:     enemy = "an <10>ally<7>"; break;
    case TargetLeader:   enemy = "its <10>leader<7>"; break;
    case TargetSummoner: enemy = "its <10>summoner<7>"; break;
    case TargetMaster:   enemy = "its <10>master<7>"; break;
    case TargetMount:    enemy = "its <10>rider<7>"; break;
  }

  switch (type) {
    case TargetInvalid:  
      s += "has an invalid target.";
      break; 

    case TargetEnemy:    
    case TargetAlly:     
    case TargetLeader:   
    case TargetSummoner: 
    case TargetMaster:
    case TargetMount:   
      if (theRegistry->Exists(data.Creature.c)) {
        Creature * c = oCreature(data.Creature.c);
        s += Format("views <9>%s<7> as %s %s.",
            (const char*)c->Name(NA_A),
            (const char*)enemy,
            (const char*)why.Dump(c->Name(NA_A)));
      } else {
        s += Format("views a now-dead creature as %s %s.",
            (const char*)enemy,
            (const char*)why.Dump("that now-dead creature"));
      } 
      break;
    case OrderAttackTarget:
      if (theRegistry->Exists(data.Creature.c)) {
        Creature * c = oCreature(data.Creature.c);
        s += Format("will attack %s %s.",
            (const char*)c->Name(NA_A),
            (const char*)why.Dump(c->Name(NA_A)));
      } else {
        s += Format("would have attacked a now-dead creature %s.",
            (const char*)why.Dump("that now-dead creature"));
      } 
      break;
    
    case TargetItem:     
      if (theRegistry->Exists(data.Item.i)) {
        Item * i = oItem(data.Item.i);
        s += Format("seeks to acquire %s %s.",
            (const char*)i->Name(NA_A),
            (const char*)why.Dump(i->Name(NA_A)));
      } else {
        s += Format("seeks to acquire a now-gone object %s.",
            (const char*)why.Dump("that now-gone object"));
      } 
      break; 

    case TargetArea:     
      s += "patrols the area."; 
      break; 

    case TargetWander:   
      s += Format("wanders about (to %d, %d).",
          data.Area.x, data.Area.y) ;
      break; 

    case OrderStandStill:
      s += "stands still."; 
      break; 
    case OrderNoPickup:
      s += "refrains from picking up items.";
      break;

    case OrderDoNotAttack: s += "avoids attacking."; break; 
    case OrderAttackNeutrals: s += "attacks neutrals."; break; 

    case OrderHide:
      s += "hides."; 
      break; 

    case OrderDoNotHide:
      s += "avoids hiding."; 
      break; 

    case OrderWalkInFront: s += "travels in the front."; break; 
    case OrderWalkInBack: s += "travels in the back."; break; 
    case OrderWalkNearMe: s += "travels near its leader."; break; 
    case OrderWalkToPoint: s += "travels to a destination."; break; 

  }
  return *tmpstr(s); 
}

uint8 Target::Y()
{
  switch (type) {
    case TargetEnemy:
    case TargetAlly:
    case TargetLeader:
    case TargetSummoner:
    case TargetMaster:
    case TargetItem:
    case TargetMount:
    case OrderAttackTarget:
      return (uint8)GetThing()->y;

    case TargetArea:
    case TargetWander:
    case OrderWalkToPoint: 
      return data.Area.y;

    default: 
    case TargetInvalid:
      Fatal("Target.Y(): TargetInvalid");
      return -1; 
  }
}

uint8 Target::X()
{
  switch (type) {
    case TargetEnemy:
    case TargetAlly:
    case TargetLeader:
    case TargetSummoner:
    case TargetMaster:
    case TargetItem:
    case TargetMount:
    case OrderAttackTarget:
      return (uint8)GetThing()->x;

    case TargetArea:
    case TargetWander:
    case OrderWalkToPoint: 
      return data.Area.x;

    default: 
    case TargetInvalid:
      Fatal("Target.X(): TargetInvalid");
      return -1; 

  }
}

Thing * Target::GetThingOrNULL()
{
  switch (type) {
    case TargetEnemy:
    case TargetAlly:
    case TargetLeader:
    case TargetSummoner:
    case TargetMaster:
    case TargetMount:
    case OrderAttackTarget:
    default:
      if (data.Creature.c && theRegistry->Exists(data.Creature.c))
        return oCreature(data.Creature.c);
      else
        return NULL;

    case TargetItem:
      if (theRegistry->Exists(data.Item.i))
        return oItem(data.Item.i);
      else
        return NULL;

    case TargetArea:
    case TargetWander:
    case TargetInvalid:
      return NULL; 
  }
}

Thing * Target::GetThing()
{
  Thing * t = GetThingOrNULL();
  if (!t) { 
    Fatal("Target.Thing(): TargetInvalid");
  }
  return t; 
}

int16 Target::DistanceFrom(const Thing *me)
{
  switch (type) {
    case TargetEnemy:
    case TargetAlly:
    case TargetLeader:
    case TargetSummoner:
    case TargetMaster:
    case TargetMount:
    case TargetItem:
    case OrderAttackTarget:
      { 
        Thing *t = GetThing();
        return dist(me->x,me->y,t->x,t->y);
      } 

    case TargetArea:
    case TargetWander:
      return dist(me->x,me->y,data.Area.x,data.Area.y);

    default: 
    case TargetInvalid:
      Fatal("Target.DistanceFrom(): TargetInvalid");
      return -1; 
  }
}

bool Target::isValid()
{
  switch (type) {
    case TargetEnemy:
    case TargetAlly:
    case TargetLeader:
    case TargetSummoner:
    case TargetMaster:
    case TargetMount:
    case OrderAttackTarget:
      if (theRegistry->Exists(data.Creature.c))
        return true;
      break; 

    case TargetItem:
      if (theRegistry->Exists(data.Item.i))
        return true; 
      break; 

    case TargetArea:
    case TargetWander:
      return true; 

    case TargetInvalid: 
      return false; 

    default: 
      if (why.type == TargetOrderedTo)
        return true; 
  }
  type = TargetInvalid;
  return false; 
}

/**************************************************************************\
 * TargetSystem
\**************************************************************************/
String & TargetSystem::Dump()
{
  String s;
  s = "";
  for (int i=0;i<tCount;i++)
    s += t[i].Dump() + "\n\n";
  return *tmpstr(s); 
}

/**************************************************************************\
 * TargetSystem -- Hostility, Racial
\**************************************************************************/

  static const struct {
    int8                me;
    int8                you;
    HostilityQuant      q;
  } hates[] = { 

    /* must be in descending order of Strength */
    { MA_UNDEAD,        MA_LIVING,      Medium },
    { MA_LIVING,        MA_UNDEAD,      Medium },

    { MA_ELF,           MA_ORC,         Medium },
    { MA_ORC,           MA_ELF,         Medium },

    { MA_GNOME,         MA_KOBOLD,      Medium },
    { MA_KOBOLD,        MA_GNOME,       Medium },

    { MA_ELF,           MA_DROW,        Medium },
    { MA_DROW,          MA_ELF,         Medium },

    { MA_HALFLING,      MA_GOBLIN,      Medium },
    { MA_GOBLIN,        MA_HALFLING,    Medium },

    { MA_DWARF,         MA_ORC,         Medium },
    { MA_ORC,           MA_DWARF,       Medium },

    { MA_CAT,           MA_DOG,         Medium },
    { MA_DOG,           MA_CAT,         Medium },

    { MA_ELF,           MA_DWARF,       Weak },
    { MA_DWARF,         MA_ELF,         Weak },

    { 0, 0, Apathy}, 

  } ; 

  static const struct {
    int8                me;
    HostilityQuant      q;
  } self_love[] = { 

    /* must be in descending order of Strength */
    { MA_LIZARDFOLK,          Strong },
    { MA_ILLITHID,            Strong }, // hive mind

    { MA_UNDEAD,              Medium }, // work together to eat brains!
    { MA_ELF,                 Medium }, // dance the happy dance
    { MA_KOBOLD,              Medium }, // gang up on the big guys
    { MA_GNOME,               Medium },
    { MA_DWARF,               Medium },
    { MA_CELESTIAL,           Medium },
    { MA_SWARM,               Medium }, // already getting along ...

    { 0, Apathy}, 

  } ; 


Hostility TargetSystem::RacialHostility(Creature *me, Creature *t)
{
  Hostility h;
  int i;
  h.quality = Neutral;
  h.quantity = Apathy;
  h.why.type = HostilityDefault;

  if (me->HasMFlag(M_MINDLESS)) 
    goto try_self_love; // unless we're both undead, do a mindless attack!

  if (me->StateFlags & MS_PEACEFUL || hasTargetOfType(OrderBeFriendly)) {
    h.why.type = HostilityPeaceful;
    h.quantity = Weak;
    return h;
  } 

  /* outsiders from different planes hate each other */
  if (me->isMType(MA_OUTSIDER) && t->isPMType(MA_OUTSIDER,me)) {
    if ( (me->isMType(MA_GOOD) && !t->isPMType(MA_GOOD,me)) ||
         (me->isMType(MA_EVIL) && !t->isPMType(MA_EVIL,me)) ||
         (me->isMType(MA_LAWFUL) && !t->isPMType(MA_LAWFUL,me)) ||
         (me->isMType(MA_CHAOTIC) && !t->isPMType(MA_CHAOTIC,me)) ) {
      h.why.type = HostilityOutsider;
      h.quality = Enemy;
      h.quantity = Strong;
      return h;
    } 
  } 

  /* evil dragons and good dragons ... */
  if (me->isMType(MA_DRAGON) && t->isPMType(MA_DRAGON,me)) {
    if ( (me->isMType(MA_GOOD) && t->isPMType(MA_EVIL,me)) ||
         (me->isMType(MA_EVIL) && t->isPMType(MA_GOOD,me)) ) {
      h.why.type = HostilityDragon;
      h.quality = Enemy;
      h.quantity = Strong;
      return h;
    } 
  } 


  for (i=0; hates[i].me; i++) 
    if (me->isMType(hates[i].me) && t->isPMType(hates[i].you,me)) {
      if ((hates[i].me == MA_UNDEAD && me->HasMFlag(M_GOOD)) ||
          (hates[i].you == MA_UNDEAD && t->HasMFlag(M_GOOD)))
        continue;
      h.why.type = HostilityFeud;
      h.why.data.feud.m1 = hates[i].me;
      h.why.data.feud.m2 = hates[i].you;
      h.quality = Enemy;
      h.quantity = hates[i].q;
      return h;
    } 

try_self_love: 

  for (i=0; self_love[i].me; i++) 
    if (me->isMType(self_love[i].me) && t->isMType(self_love[i].me)) {
      h.why.type = HostilitySolidarity;
      h.why.data.solidarity.ma = self_love[i].me;
      h.quality = Ally;
      /* A gnome monster will support another gnome monster. However,
         she will not accept orders from a gnome PC just because they
         are both gnomes -- the affinity there is modeled by the PC's
         race social bonuses to social rolls. */ 
      if (t->isPlayer())
        h.quality = Neutral;
      h.quantity = hates[i].q;
      return h;
    } 

  if (me->HasMFlag(M_MINDLESS)) {
    h.quality = Enemy;
    h.quantity = Strong;
    h.why.type = HostilityMindless;
    return h;
  } 

  if (me->HasMFlag(M_HOSTILE) &&
        !me->getLeader()) {
    h.quality = Enemy;
    h.quantity = Strong;
    h.why.type = HostilityFlag;
    return h;
  } 

  // evil creatures are always looking for an advantage ...
  if (me->isMType(MA_EVIL) && !t->HasStati(DISGUISED) &&
      (me->ChallengeRating() >= t->ChallengeRating() ||
       t->isPlayer())) {
    h.why.type = HostilityEvil;
    h.quality = Enemy;
    h.quantity = Tiny;
    return h;
  }

  if (me->HasMFlag(M_CARNI) && t->isPMType(MA_ANIMAL,me) &&
      !me->isPlayer() && 
      me->ChallengeRating() >= t->ChallengeRating()) {
    h.why.type = HostilityFood;
    h.quality = Enemy;
    h.quantity = Tiny;
    return h;
  } 

  if (me->HasMFlag(M_HERBI) && t->isPMType(MA_PLANT,me) &&
      !me->isPlayer() && 
      me->ChallengeRating() >= t->ChallengeRating()) {
    h.why.type = HostilityFood;
    h.quality = Enemy;
    h.quantity = Tiny;
    return h;
  } 

  if (me->isMType(MA_GOOD) && t->isPMType(MA_GOOD,me) &&
       !hasTargetOfType(OrderAttackNeutrals)) {
    h.why.type = HostilityGood;
    h.quantity = Weak;
    return h;
  }

  if (me->isMType(MA_GOOD) && t->isPMType(MA_EVIL,me) &&
        !(t->Flags & MS_PEACEFUL)) {
    h.why.type = HostilitySmite;
    h.quantity = Weak;
    return h;
    }


  if (hasTargetOfType(OrderAttackNeutrals)) {
    h.why.type = HostilityLeader;
    h.quality = Enemy;
    h.quantity = Tiny;
    return h;
  } 
  
  if (me->isMType(MA_ANIMAL))
    if (!t->isPMType(MA_ANIMAL,t) && !t->HasAbility(CA_SYLVAN_AFFINITY)) {
      h.why.type = HostilityAlienation; 
      h.quality = Enemy;
      h.quantity = Tiny;
      }

  if (me->isMType(MA_ELEMENTAL+(MA_EARTH*256)))
    if (!t->isMType(MA_EARTH) && !t->isMType(MA_DWARF) && !t->isMType(MA_GNOME)) {
      h.why.type = HostilityAlienation; 
      h.quality = Enemy;
      h.quantity = Tiny;
      }
  if (me->isMType(MA_ELEMENTAL+(MA_WATER*256)))
    if (!t->isMType(MA_WATER)) {
      h.why.type = HostilityAlienation; 
      h.quality = Enemy;
      h.quantity = Tiny;
      }
  if (me->isMType(MA_ELEMENTAL+(MA_AIR*256)))
    if (!t->isMType(MA_AIR)) {
      h.why.type = HostilityAlienation; 
      h.quality = Enemy;
      h.quantity = Tiny;
      }
  if (me->isMType(MA_ELEMENTAL+(MA_FIRE*256)))
    if (!t->isMType(MA_FIRE)) {
      h.why.type = HostilityAlienation; 
      h.quality = Enemy;
      h.quantity = Tiny;
      }
      

  return h;
} 

/**************************************************************************\
 * TargetSystem -- Hostility, Personal
\**************************************************************************/
Hostility TargetSystem::SpecificHostility(Creature *me, Creature *t)
{
  Hostility h;
  Creature * me_leader = getLeader();
    
  h.quality = Neutral;
  h.quantity = Apathy;
  h.why.type = HostilityDefault;
  
  if (me->isPlayer() && t->isPlayer())
    return h;

  if (me == t) {
    h.quality = Ally;
    h.quantity = Strong;
    return h; 
  }
  
  if (me->HasStati(CHARMED,CH_DOMINATE,t)) {
    h.quality = Ally;
    h.quantity = Strong;
    h.why.type = HostilityEID;
    h.why.data.eid.eid = me->GetStatiEID(CHARMED,CH_DOMINATE,t);
    return h;
    }
  if (me->HasStati(CHARMED,CH_COMMAND,t)) {
    h.quality = Ally;
    h.quantity = Strong;
    h.why.type = HostilityCommanded;
    return h;
    }
  if (me->HasStati(CHARMED,CH_CHARM,t)) {
    h.quality = Neutral;
    h.quantity = Strong;
    if (me->GetStatiEID(CHARMED,CH_CHARM,t)) {
      h.why.type = HostilityEID;
      h.why.data.eid.eid = me->GetStatiEID(CHARMED,CH_CHARM,t);
      }
    else
      h.why.type = HostilityCharmed;
    return h;
    }  
  
  if (t->isIllusion() && !t->isShade())
    if (!t->isRealTo(me))
      return h;

  

  Target * targ;
  /*
    if (HasStati(ANIMAL_COMPANION,TA_LEADER)) {
      Creature * druid = (Creature *)GetStatiObj(ANIMAL_COMPANION,TA_LEADER);
      if (c == druid) return false;
      else if (c->isHostileTo(druid) ||
               c->HasStati(TARGET,TA_ENEMY,c))
        return true; 
    }
    if (isMType(MA_REPTILE) && HasAbility(CA_ANCESTRAL_MEMORY))
      if (c->HasAbility(CA_ANCESTRAL_MEMORY))
        return false;
    */

  if (targ = GetTarget(t)) {
    // we have a specific memory of 't' (probably that it hit us!) 
    switch (targ->type) {
      case TargetInvalid: 
      case TargetArea: 
      case TargetWander: 
      case TargetItem: 
        Fatal("SpecificHostility: bad Target type");
        break; 

      case TargetEnemy:
        h.why.type = HostilityTarget;
        h.quality = Enemy;
        h.quantity = Strong;
        return h;

      case TargetAlly:
        h.why.type = HostilityTarget;
        h.quality = Ally;
        h.quantity = Strong;
        return h;

      case TargetLeader:
      case TargetSummoner:
      case TargetMaster: 
      case TargetMount:
        h.why.type = HostilityLeader;
        h.quality = Ally;
        h.quantity = Strong;
        return h;
    } 
  } 

  // does our leader have an opinion about it? 
  
  if (me_leader == t) {
    h.quality = Ally;
    h.quantity = Strong;
    h.why.type = HostilityLeader; 
    return h; 
    } 
  else if (me_leader && me_leader != me) {
    // does it hate our leader? 
    Hostility hl = t->ts.SpecificHostility(t,me_leader);
    if (hl.quality == Enemy) {
      hl.why.type = HostilityDefendLeader; 
      return hl; 
      } 
    
    hl = me_leader->ts.SpecificHostility(me_leader,t);
    if (hl.quality != Neutral || (me_leader->isCharacter() && 
         !hasTargetOfType(OrderAttackNeutrals))) {
      hl.why.type = HostilityLeader; 
      return hl; 
      } 
    } 

  // does it have a leader? 
  Creature * t_leader = t->ts.getLeader();
  if (t_leader == me) {
    // I like my underlings!
    h.why.type = HostilityParty;
    h.quality = Ally;
    h.quantity = Strong;
    return h;
  } else if (t_leader && !t_leader->isPlayer()) {
    Hostility hl = t_leader->ts.SpecificHostility(t_leader,me);
    if (hl.quality == Enemy) {
      // WW: TODO, fix this up
      hl.why.type = HostilityYourLeaderHatesMe; 
      return hl; 
    } 
  } 

  if (me->PartyID && me->PartyID == t->PartyID) {
    h.why.type = HostilityParty;
    h.quality = Ally;
    h.quantity = Strong;
    return h;
  } 

  // we repeat this thrice, because otherwise if you have the ring of agg
  // monsters and the amulet of undead friendship, you'll get something
  // random (i.e., varying with stati order)
  StatiIterNature(t,ENEMY_TO)
      if (S->Val == -1 || me->isMType(S->Val)
            || S->h == me->myHandle) {
        h.why.type = HostilityEID;
        h.why.data.eid.eid = S->eID;
        h.quality = Enemy;
        h.quantity = Medium;
        StatiIterBreakout(t,return h)
      } 
  StatiIterEnd(t)
  /* For now, this is neutral instead of ally -- the motive behind this
     is that the game currently doesn't distinguish between "ally" and
     "party member", and we dont want a pc with ALLY_TO(MA_ELF) losing
     all their leadership xCR when they descend to a level with other
     elves on it. */
  StatiIterNature(t,ALLY_TO)
      if (S->Val == -1 || (S->Val && me->isMType(S->Val))
            || S->h == me->myHandle) {
        h.why.type = HostilityEID;
        h.why.data.eid.eid = S->eID;
        h.quality = Neutral;
        h.quantity = Strong;
        StatiIterBreakout(t,return h)
      }
  StatiIterEnd(t)
  StatiIterNature(t,NEUTRAL_TO)
      if (S->Val == -1 || (S->Val && me->isMType(S->Val))
            || S->h == me->myHandle) {
        h.why.type = HostilityEID;
        h.why.data.eid.eid = S->eID;
        h.quality = Neutral;
        h.quantity = Medium;
        StatiIterBreakout(t,return h)
      }
  StatiIterEnd(t)

  if (me->HasStati(CHARMED,CH_CALMED,t)) {
    h.quality = Neutral;
    h.quantity = Strong;
    h.why.type = HostilityEID;
    h.why.data.eid.eid = me->GetStatiEID(CHARMED,CH_CALMED,t);
    return h;
    }

  // if we have gotten this far and nothing specific tells us to be 
  // an enemy or an ally, go with the racial default
  return RacialHostility(me, t);
}

/**************************************************************************\
 * TargetSystem -- Utility - Target Getting and Checking
\**************************************************************************/
Creature * TargetSystem::getLeader()
{
  for (int i=0; i<tCount; i++) {
    if (t[i].isValid())
    switch (t[i].type) {
      case TargetLeader:   
      case TargetSummoner: 
      case TargetMaster:   
      case TargetMount:
        return (Creature *)(t[i].GetThing());
    } 
  }
  return NULL; 
}

bool TargetSystem::isLeader(Creature *t)
{
  return (getLeader() == t);
}

Target * TargetSystem::getTargetOfType(TargetType type)
{
  for (int i=0; i<tCount; i++)
    if (t[i].type == type)
      return &t[i]; 
  return NULL;
}

bool TargetSystem::hasTargetOfType(TargetType type)
{
  for (int i=0; i<tCount; i++)
    if (t[i].type == type)
      return true; 
  return false;
}

bool TargetSystem::hasTargetThing(Thing *thing)
{
  for (int i=0; i<tCount; i++)
    if (t[i].GetThingOrNULL() == thing) 
      return true; 
  return false;
}

/**************************************************************************\
 * TargetSystem -- Utility - Target Adding & Removing
\**************************************************************************/
void TargetSystem::RemoveTargetNumber(uint16 i)
{
  ASSERT(i < tCount);
  t[i].type = TargetInvalid; 
}

bool TargetSystem::addTarget(Target &newT)
{
  int i;
  for (i=0; i<tCount && t[i].isValid() ; i++)
    ;;
  if (i != NUM_TARGETS) {
    t[i] = newT;
    tCount = max(tCount, i+1);
    return true; 
    }
  for (i=tCount-1; i>=0; i--) 
    if (t[i].priority <= newT.priority) {
      t[i] = newT;
      tCount = max(tCount, i+1);
      return true;
    } 
  return false; 
}

bool TargetSystem::addCreatureTarget(Creature *targ, TargetType type)
{
  Target newT;
  newT.type = type;
  newT.data.Creature.c = targ->myHandle;
  newT.priority = 30; 
  newT.vis = false;
  newT.why.Set(TargetDefault);
  return addTarget(newT);
}

bool TargetSystem::giveOrder(Creature * me, Creature *master, TargetType order, Thing * victim, int x, int y) 
{
  switch (order) {
    case OrderNoPickup:
      for (int i = 0; i<tCount; i++)
        if (t[i].type == TargetItem)
          t[i].type = TargetInvalid;
     break;
    case OrderStandStill: 
    case OrderWalkInFront: 
    case OrderWalkInBack: 
    case OrderWalkNearMe: 
    case OrderWalkToPoint: 
      ForgetOrders(me,OrderWalkInFront);
      ForgetOrders(me,OrderWalkInBack);
      ForgetOrders(me,OrderWalkNearMe);
      ForgetOrders(me,OrderWalkToPoint);
      ForgetOrders(me,OrderStandStill);
      break; 

    case OrderDoNotHide:
    case OrderHide:
      ForgetOrders(me,OrderDoNotHide);
      ForgetOrders(me,OrderHide);
      break; 

    case OrderDoNotAttack: 
    case OrderAttackNeutrals:
    case OrderBeFriendly:
      ForgetOrders(me,OrderDoNotAttack);
      ForgetOrders(me,OrderAttackNeutrals);
      ForgetOrders(me,OrderBeFriendly);
      break; 
   
  } 

  Target newT;
  newT.type = order;
  if (victim && victim->isCreature()) 
    newT.data.Creature.c = victim->myHandle; 
  if (x != -1) {
    newT.data.Area.x = x;
    newT.data.Area.y = y;
  }
  newT.priority = 40; 
  newT.vis = false;
  newT.why.Set(TargetOrderedTo);
  return addTarget(newT);
} 

void TargetSystem::removeCreatureTarget(Creature *targ, TargetType type)
{
  int16 i;
  for (i=0;i!=tCount;i++)
    if (t[i].type == type || type == -1)
      if (!targ || t[i].data.Creature.c == targ->myHandle)
        t[i].type = TargetInvalid;
}

/**************************************************************************\
 * TargetSystem -- Targets
\**************************************************************************/
Target * TargetSystem::GetTarget(Thing *thing)
{
  for (int i=0; i<tCount; i++) 
    switch (t[i].type) {
      case TargetEnemy:
      case TargetAlly:
      case TargetLeader:
      case TargetSummoner:
      case TargetMaster:
      case TargetMount:
        if (t[i].data.Creature.c == thing->myHandle) {
          return &t[i]; 
        } 
        break;

      case TargetItem:
        if (t[i].data.Item.i == thing->myHandle) {
          return &t[i]; 
        } 
        break;

      default: break;
    }
  return NULL; 
}

void TargetSystem::RateAsTarget(Creature *me, Thing *t, Target & newT)
{
  String s;
  newT.priority = 0;
  if (t->isType(T_TRAP) || t->isFeature())
    return;
  if (!t->isRealTo(me))
    return;
  if (t->isItem()) {
    Item *i = (Item *)t;

    /* Illusions can't pick up items, therefore have no
       desire toward them. Shades can, though. */
    if (me->isIllusion() && !me->isShade())
      return;
      
    if (hasTargetOfType(OrderNoPickup))
      return;
      
    if (me->onPlane() != t->onPlane())
      if (!me->HasAbility(CA_PHASE))
        return; 
    
    if (i->isType(T_CORPSE) ||
        i->isType(T_CHEST) ||
        me->HasMFlag(M_NOHANDS) || 
        me->HasMFlag(M_IGNORE_ITEMS) || 
        me->HasMFlag(M_MINDLESS) ||
        me->HasStati(DO_NOT_PICKUP) ||
        me->isPlayer())
      return;

    if (t->isIllusion())
      {
        Creature *cr = (Creature*)t->GetStatiObj(ILLUSION);
        newT.priority = cr->SkillLevel(SK_ILLUSION)*2;
        newT.type = TargetItem;
        newT.why.Set(TargetGreedy);
        newT.data.Item.i = i->myHandle; 
        goto GoodItem;
      }

    if ((i->isType(T_HELMET) || 
          i->isType(T_CROWN) ||
          i->isType(T_AMULET)) &&
        (me->HasMFlag(M_NOHEAD)))
      return;

    if ((i->isType(T_ARMOUR)) && 
        (!me->HasMFlag(M_HUMANOID)))
      return;

    newT.type = TargetItem;
    newT.priority = 1 + i->PItemLevel(me);
    newT.why.Set(TargetGreedy);
    newT.data.Item.i = i->myHandle; 

    GoodItem:
    if (me->HasMFlag(M_GREEDY) && i->isType(T_COIN))
      newT.priority *= 5;
    else if (me->HasMFlag(M_GEMS) && i->isType(T_GEM))
      newT.priority *= 5;
    else if (me->HasMFlag(M_COVETOUS))
      newT.priority *= 4;

  } else if (t->isCreature()) {

    // unless a lot of things happen, we don't care about this creature
    newT.type = TargetInvalid;

    uint16 per = me->Percieves(t);

    if (per) { // we can see it! 
      Hostility h = SpecificHostility(me, (Creature *)t);

      if (h.quality == Enemy) { // we don't like it!

        newT.type = TargetEnemy;
        newT.priority = (int)h.quantity;
        newT.why.Set(TargetWasSeenAndHostile);
        newT.why.data.WasSeenAndHostile.h = h;
        newT.why.data.WasSeenAndHostile.per = per;
        newT.data.Creature.c = t->myHandle;
        newT.data.Creature.damageDoneToMe = 0;
      }
    }
  } else {
    s = Format("TargetSystem::RateAsTarget(%s,%s) non-item, non-creature",(const char*)me->Name(0),(const char*)t->Name(0));
    Fatal(s);
    newT.type = TargetInvalid;
  } 
  return;
} 

static int TargetSort(const void *a, const void *b)
{
  Target *l = (Target *)a,
         *r = (Target *)b;
  bool l_lead = l->type == TargetLeader || 
    l->type == TargetSummoner ||
    l->type == TargetMount ||
    l->type == TargetMaster;
  bool r_lead = r->type == TargetLeader || 
      r->type == TargetSummoner ||
      r->type == TargetMount ||
      r->type == TargetMaster;
  if (l_lead || r_lead) { 
    if (r_lead && !l_lead) return 999;
    else if (l_lead && !r_lead) return -999;
  } 
  if (r->type == TargetInvalid) return -999;
  else if (l->type == TargetInvalid) return 999;
  else if (r->priority == l->priority) {
    if (r->type == l->type) {
      return (r->GetThingOrNULL() - l->GetThingOrNULL());
    } else return (r->type - l->type); 
  } else return (r->priority - l->priority); 
}

void TargetSystem::ForgetOrders(Creature *me, int ofType)
{
  Target   targets[1024]; 
  int      count = 0, i = 0; 
  for (i=0;i<tCount;i++) {
    targets[count++] = t[i];
    if (t[i].why.type == TargetOrderedTo) {
      if (ofType == -1 || t[i].type == ofType)
        t[i].type = TargetInvalid; 
    }
  }
}

void TargetSystem::Retarget(Creature *me, bool force)
{
  if (!this) return; 
  if (!me) return;
  
  if (!force) {
    shouldRetarget = true; 
    return;
  }

  /*
  if (getLeader())
    getLeader()->IPrint(Format("%s +retarg %d. [%s]",(const char*)me->Name(),tCount,
          Dump()));
          */

  shouldRetarget = false; 

  Target   targets[1024]; 
  int      count = 0, i = 0, j; 
  memcpy(targets,t,sizeof(Target) * tCount);
  count = tCount; 

#define MAX_TARG_DIST   18      // Julian's knob

  if (me->HasStati(ENGULFER)) {
    RateAsTarget(me,me->GetStatiObj(ENGULFER),targets[count]);
    if (targets[count].priority)
      count++;
    }

  for (int dx = -MAX_TARG_DIST; dx <= MAX_TARG_DIST; dx++) 
    for (int dy = -MAX_TARG_DIST; dy <= MAX_TARG_DIST; dy++) {
      int xx = me->x + dx;
      int yy = me->y + dy; 
      if (!me->m)
        continue;
      if (!me->m->InBounds(xx,yy))
        continue; 
        
      /* Count creatures with size fields exactly once. */
      if (me->m->FieldAt(xx,yy,FI_SIZE))
        if (me->m->FCreatureAt(xx,yy)->x != xx ||
            me->m->FCreatureAt(xx,yy)->x != yy)
          continue;
      for (Thing * it = me->m->FirstAt(xx,yy);it;it=me->m->NextAt(xx,yy)) {
        if (!it->isCreature() && !it->isItem()) continue; 

        RateAsTarget(me,it,targets[count]);

        if (targets[count].priority <= 0) continue; 

        count++;
      }
    } 

  qsort(targets, count, sizeof(targets[0]), TargetSort);

  for (i=0;i<tCount;i++) {
    t[i].type = TargetInvalid; 
  }

  i = 0; j = 0;
  while (i < NUM_TARGETS && j < count) {
    if (targets[j].type == TargetInvalid) {
      j++; continue;
    } 
    if (j > 0 && 
        targets[j].GetThingOrNULL() == targets[j-1].GetThingOrNULL() &&
        targets[j].GetThingOrNULL() != NULL) {
      j++; continue;
    } 
    t[i] = targets[j];
    i++;
    j++; 
  } 
  tCount = i; 

  /*
  if (getLeader())
    getLeader()->IPrint(Format("%s -retarg %d. [%s]",(const char*)me->Name(),tCount,
          Dump()));
          */

}


void TargetSystem::Serialize(Registry &r)
{ 
}

/**************************************************************************\
 * TargetSystem -- Utility - The Environment
\**************************************************************************/
void TargetSystem::Consider(Creature *me, Thing *t)
{
  if (hasTargetThing(t))
    return;

  Target newT;

  RateAsTarget(me,t,newT);

  if (newT.priority > 0)
    addTarget(newT);
}

void TargetSystem::HearNoise(Creature *me, uint8 x, uint8 y)
{
  if (!me || !me->m) 
    return; 
  Creature *c = me->m->FCreatureAt(x,y);
  if (me->HasMFlag(M_DEAF)) 
    return;
  else if (c) {
    Consider(me,c);
  } else {
    // investigate!
    Target newT;
    newT.type = TargetArea;
    newT.priority = 1;
    newT.why.Set(TargetHeardNoise);
    newT.data.Area.x = x;
    newT.data.Area.y = y;
    addTarget(newT);
  } 
}

void TargetSystem::Wanderlust(Creature *me, Thing *wander_to)
{
  for (int i=0; i<tCount; i++)
    if (t[i].type == TargetWander)
      t[i].type = TargetInvalid; 

  for (int tries = 0; tries < 1000; tries++) {
    int x, y;
    if (wander_to) {
      x = wander_to->x;
      y = wander_to->y;
      }
    else {
      x = random(me->m->SizeX());
      y = random(me->m->SizeY());
      }
    if (me->m->SolidAt(x,y))
      continue;
    Target newT;
    newT.type = TargetWander;
    newT.priority = wander_to ? 40 : 1;
    newT.why.Set(TargetWanderlust);
    newT.data.Area.x = x;
    newT.data.Area.y = y;
    addTarget(newT);
    return; 
  } 
  return; 
}

/* fjm: Wes, I don't have a great understanding of your TargetSystem.
   Would you check this over for me and make sure it isn't corrupting
   any data structures or otherwise being bad? */

void TargetSystem::Liberate(Creature *me, Creature *lib)
  {
    Target *targ; Creature *summ;

    if (me->isDead()) return;

    summ = (Creature*)me->GetStatiObj(SUMMONED);

    ASSERT(summ && summ->isCreature());
    
    if (targ = GetTarget(summ))
      {
        targ->type = TargetEnemy;
        targ->why.Set(TargetLiberated);
        targ->priority = 50;
      }
    else
      {
        Target newT;
        newT.type = TargetEnemy;
        newT.priority = 50;
        newT.why.Set(TargetLiberated);
        newT.data.Creature.c = summ->myHandle;
        addTarget(newT);
      }

    /* Gratitude, even forgiving injury. */
    for (int i = 0; i<tCount; i++)
      if (t[i].type == TargetEnemy)
        if (t[i].data.Creature.c == lib->myHandle)
          t[i].type = TargetInvalid;


    if (lib->Percieves(me))
      {
        if (lib->Percieves(summ))
          lib->IPrint("You free the <Obj> from the <Obj>'s control!",me,summ);
        else
          lib->IPrint("You free the <Obj> from its summoner's control!",me);
      }
    if (me->Percieves(lib))
      me->IPrint("The <Obj> frees you from the <Obj>'s control!",lib,summ);
    else
      me->IPrint("You are freed from the <Obj>'s control!",summ);
    
    /* Even if you can't see the creature, you know... */
    summ->IPrint("You feel your control over the <Obj> snap!",me);
  }

void TargetSystem::ItHitMe(Creature *me, Creature *t, int16 damage) {
    Target *targ; Creature *cr, *leader; int16 i;

    /* Horrid kludge -- this should *really* take an EventInfo! */
    EventInfo &e = EventStack[EventSP];

    if (me->isDead() || me->isPlayer())
        return;

    if (t->isIllusion() && !t->isRealTo(me))
        return;

    /* When one ally attacks another, give the PC a chance to "mend fences". */
    if (leader = me->getLeader())
        if (leader == t->getLeader() && leader != t) {
            if (me->HasStati(SUMMONED) || me->HasStati(ILLUSION))
                return;
            if (leader->SkillCheck(SK_DIPLOMACY,15,true)) {
                leader->IPrint("You diffuse the <Obj>'s anger at the <Obj>'s inadvertant attack.", me, t);
                return;
            }
        }

    if (e.AType != A_GAZE && e.DType != AD_CHRM && e.DType != AD_SPE1 && e.DType != AD_SPE2)
        me->RemoveStati(CHARMED,-1,CH_CHARM,-1,t);

    if (targ = GetTarget(t)) {
        uint32 limit = 0;

        targ->data.Creature.damageDoneToMe += damage; 

        switch (targ->type) {
          case TargetInvalid: 
          case TargetArea: 
          case TargetWander: 
          case TargetItem: 
              Fatal("ItHitMe: bad Target type");
              break; 

          case TargetEnemy:         // already hate you
          case TargetMaster:        // can never hate you 
              return;

          case TargetAlly:          limit = 5 + t->Mod(A_CHA)*2; break;
          case TargetLeader:        limit = 10 + t->Mod(A_CHA)*2; break; 
          case TargetMount:         limit = 10 + t->Mod(A_CHA)*2; break;
          case TargetSummoner:      limit = 15 + t->Mod(A_CHA)*2; break; 
        }

        if (targ->data.Creature.damageDoneToMe > limit) {
            String msg;
            // OK, we hate you now! 
            targ->type = TargetEnemy;
            targ->why.Set(TargetHitMe);
            if (t->isPlayer())
                msg = Format("%s snaps and turns on you!",(const char*)me->Name(NA_THE));
            else
                msg = Format("%s snaps and turns on %s!",(const char*)me->Name(NA_THE),(const char*)t->Name(NA_THE));
            t->IDPrint(msg,msg);
        } 
        goto HorseCheck; 
    }

    Hostility h;
    h = SpecificHostility(me,t);
    if (h.quality == Enemy)          // already hate you 
        goto HorseCheck;
    if (h.quality == Ally && !t->isPlayer())        
        goto HorseCheck; 
    // until we refine the system more, allies are fast friends ... 
    // example: krenshar wailing might cause all of its allies to turn on it!

    // OK, we don't like you any more!

    MapIterate(me->m,cr,i)
        if (cr->isCreature())
            if (cr->isFriendlyTo(me) && !cr->isHostileTo(t)) {
                Target newT;
                newT.type = TargetEnemy;
                newT.priority = 20;
                newT.why.Set(TargetHitMe);
                newT.data.Creature.c = t->myHandle;
                newT.data.Creature.damageDoneToMe = damage;
                cr->ts.addTarget(newT);
                cr->ts.ForgetOrders(cr,-1);
                String msg;
                if (t->isPlayer())
                    msg = Format("%s becomes angry at you!",(const char*)cr->Name(NA_THE));
                else
                    msg = Format("%s becomes angry at %s!",(const char*)cr->Name(NA_THE), (const char*)t->Name(NA_THE));
                if (!cr->isDead())
                    cr->IDPrint(msg,msg);
                cr->GainPermStati(WAS_FRIENDLY,t,SS_MONI, h.quality == Ally);
            }

HorseCheck:
    if (me->HasStati(MOUNT) && me->GetStatiObj(MOUNT) == t)
        ThrowVal(EV_DISMOUNT,DSM_THROWN,t);
}

void TargetSystem::MissedShapechange(Creature *me, Creature *t)
{
  // I don't care about this now, we can fix it up later ...
  return; 
}

void TargetSystem::Clear()
{
  // remove all targets ...
  tCount = 0; 
}

void TargetSystem::Pacify(Creature *me, Creature *soother)
  {
    // remove all enemy targets ...
    for (int i = 0; i<tCount; i++)
      if (t[i].type == TargetEnemy)
        t[i].type = TargetInvalid;
    me->StateFlags |= MS_PEACEFUL;

    Retarget(me,true);    
    
    Creature *cr;
    if (me->HasStati(GRAPPLING) &&
        (cr = (Creature*) me->GetStatiObj(GRAPPLING))) 
      if (!me->isHostileTo(cr))
        {
          cr->RemoveStati(GRAPPLED,-1,-1,-1,me);
          cr->RemoveStati(GRABBED,-1,-1,-1,me);
          me->RemoveStati(GRAPPLING,-1,-1,-1,cr);
        }
    if (me->HasStati(GRAPPLED) &&
        (cr = (Creature*) me->GetStatiObj(GRAPPLED)))
      if (!me->isHostileTo(cr))
        {
          cr->RemoveStati(GRAPPLING,-1,-1,-1,me);
          me->RemoveStati(GRAPPLED,-1,-1,-1,cr);
        }
    if (me->HasStati(GRABBED) &&
        (cr = (Creature*) me->GetStatiObj(GRABBED)))
      if (!me->isHostileTo(cr))
        {
          cr->RemoveStati(GRAPPLING,-1,-1,-1,me);
          me->RemoveStati(GRABBED,-1,-1,-1,cr);
        }
    if (me->HasStati(ENGULFER) &&
        (cr = (Creature*) me->GetStatiObj(ENGULFER))) 
      if (!me->isHostileTo(cr))
        me->DropEngulfed(NULL);
    if (me->HasStati(ENGULFED) &&
        (cr = (Creature*) me->GetStatiObj(ENGULFED)))
      if (!me->isHostileTo(cr))
        cr->DropEngulfed(NULL);
    
    return; 
  }
  
void TargetSystem::TurnNeutralTo(Creature *me, Creature *neutral_to)
  {
    
    // remove all enemy targets ...
    for (int i = 0; i<tCount; i++)
      if (t[i].type == TargetEnemy || 
            (t[i].GetThingOrNULL() == neutral_to))
        t[i].type = TargetInvalid;
    
    
    me->StateFlags |= MS_PEACEFUL;
    
    Retarget(me,true);    
    
    Creature *cr;
    if (me->HasStati(GRAPPLING) &&
        (cr = (Creature*) me->GetStatiObj(GRAPPLING))) 
      if (!me->isHostileTo(cr))
        {
          cr->RemoveStati(GRAPPLED,-1,-1,-1,me);
          cr->RemoveStati(GRABBED,-1,-1,-1,me);
          me->RemoveStati(GRAPPLING,-1,-1,-1,cr);
        }
    if (me->HasStati(GRAPPLED) &&
        (cr = (Creature*) me->GetStatiObj(GRAPPLED)))
      if (!me->isHostileTo(cr))
        {
          cr->RemoveStati(GRAPPLING,-1,-1,-1,me);
          me->RemoveStati(GRAPPLED,-1,-1,-1,cr);
        }
    if (me->HasStati(GRABBED) &&
        (cr = (Creature*) me->GetStatiObj(GRABBED)))
      if (!me->isHostileTo(cr))
        {
          cr->RemoveStati(GRAPPLING,-1,-1,-1,me);
          me->RemoveStati(GRABBED,-1,-1,-1,cr);
        }
    if (me->HasStati(ENGULFER) &&
        (cr = (Creature*) me->GetStatiObj(ENGULFER))) 
      if (!me->isHostileTo(cr))
        me->DropEngulfed(NULL);
    if (me->HasStati(ENGULFED) &&
        (cr = (Creature*) me->GetStatiObj(ENGULFED)))
      if (!me->isHostileTo(cr))
        cr->DropEngulfed(NULL);
  }
  
void TargetSystem::TurnHostileTo(Creature *me, Creature *hostile_to)
  {
    Target *targ = GetTarget(hostile_to);

    if (!targ)
      {
        removeCreatureTarget(hostile_to,TargetAny);
        if (!addCreatureTarget(hostile_to,TargetEnemy))
          return;
        targ = GetTarget(hostile_to);
        ASSERT(targ);
      }    
        
    targ->type = TargetEnemy;
    targ->why.Set(TargetDefault);
    
    if (me->HasStati(MOUNT))
      if (me->isHostileTo((Creature*)me->GetStatiObj(MOUNT)))
        ThrowVal(EV_DISMOUNT,DSM_THROWN,me->GetStatiObj(MOUNT));
    
  }
