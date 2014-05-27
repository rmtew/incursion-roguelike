/* MONSTER.CPP -- Copyright (c) 1999-2003 Julian Mensch
     The monster AI, in all its ungainly glory. Everything
   relating to monster behaviour, including monster spell-
   casting and monster inventory, is here. This file is thus
   fairly self-contained. The monster memory is then crammed
   in at the very bottom.

     Monster::Monster(rID _mID,int16 _Type)
     void Monster::ChooseAction()
     void Monster::Retarget()
     void Monster::AlertNew(Thing *nt)
     void Monster::Initialize(bool in_play)
     EvReturn Monster::Movement()
     uint32 Monster::BadFields()
     void Monster::ListEffects()
     void Monster::AddEffect(TEffect *te, rID eID, Item *src)
     uint32 Monster::SetMetamagic(rID eID, Thing *tar, uint16 Pur)
     EvReturn Monster::Inventory()
     bool Item::betterFor(Creature *c, Item *curr)
     bool Weapon::betterFor(Creature *c, Item *curr)
     bool Armour::betterFor(Creature *c, Item *curr)
     EvReturn Monster::Event(EventInfo &e)
     int16 Monster::RateAsTarget(Thing *t)

     String Monster::Describe(Player *p)

*/

#include "Incursion.h"

bool thousandItemsError = false;

ActionInfo Monster::Acts[64];
EffectInfo Monster::Effs[1024];
bool       Monster::inMelee, Monster::targVis, Monster::isAfraid,
           Monster::isSick, Monster::isBlind, Monster::isPoisoned,
           Monster::isParalyzed, Monster::hasHunger, Monster::isDiseased,
           Monster::isBleeding, Monster::isNatBlind, Monster::isTAfraid,
           Monster::hasTarget, Monster::isHiding, Monster::isStoning,
           Monster::isDrained, Monster::isEnraged, Monster::isGrabbed,
           Monster::isAvoiding, Monster::hasInnateSpellStati, Monster::isMount;
Creature  *Monster::mtarg, *Monster::rtarg, *Monster::CurrAI;
int16      Monster::nAct, Monster::nEff, Monster::nStati;
uint32     Monster::hasEffFor;

/* Things to do:
     Behaviours.
     Stati Duration == Damage done.
*/

#define BV_PSYCHO   1 /* Attack in melee until dead */
#define BV_NORMAL   2 /* Attack, flee when wounded */
#define BV_ANIMAL   3 /* Attack if hungry, flee early, eat food */
#define BV_GUARD    4 /* Stand in set location until attacked, fight
                           until target is dead/captured, return there */
#define BV_MAGE     5 /* Cast spells, avoid melee desperately */
#define BV_PLAYFUL  6 /* Use theft/prank attacks even when friendly */
#define BV_CLERIC   7 /* Buff self, buff party, heal party, otherwise fight */
#define BV_TOWNIE   8 /* Choose random target locations on map and go there
                           when friendly */
#define BV_RANGED   9  /* Prefer ranged combat; avoid melee */
#define BV_GREEDY   10 /* Acquisition above fighting */
#define BV_PRACTICAL 11 /* Aranea, Adventurers, etc. */
#define BV_FUNGI    12 /* Spray anything nearby, else nothing */
#define BV_CONSTRUCT 13
#define BV_NEMESIS   14 
#define BV_COMPANION 15 /* Fight to defend player, let player choose targets,
                             messages signaling player to choose */
#define BV_SMART     16 

/* Monster logic:
 * Call QUE_MONACT,
 *   if ACT_CONST returned, do that.
 * Do I have an extended objective?
 *   Is its priority still valid WRT my injury level?
 *     Move toward it, and return.
 * What's my biggest trouble?
 *   Can I remedy it?
 *     Do so, and return.
 * Am I "guarding"?
 *   Stand still; Timeout = -1.
 * Am I in disguise shape & near player?
 *   Change to different form.
 * Are all my targets out of sight?
 *   Am I a shapeshifter with disguise form?
 *     Change into disguise form.
 *   Can I turn invis / mirror image / displace?
 *     Am I not already?
 *       Do so.
 *   Can I hide?
 *     Hide.
 *   Note that I've prepared a surprise, and don't do
 *     anything more.
 * Can I shapeshift?
 *   If so, 1/20 chance of doing so.
 * Am I standing on a cool item?
 *   Pick it up if not drastically threatened,
 *     and unset MS_INVEN_GOOD.
 * Is my inventory non-ideal or changed?
 *   Call SelectMonItems();
 * Am I hidden?
 *   Can I move?
 *     If so, move toward target.
 *   Else, do nothing unless prey nearby.
 *     If prey nearby, sneak attack
 * What's a member of my group's biggest trouble?
 *   Can I remedy it?
 *     Do so, and return.
 * Do I have a booster item/spell?
 *   If so, cast it and return.
 * Do I have a target?
 *   If so, do I have an (effective) ranged attack?
 *     Choose one, and use it.
 *   Else, if I know where it is,
 *     Move intelligently toward target.
 *   Else, if I'm not there yet,
 *     Move toward last known position.
 *   Else, are any other targets in sight?
 *     Move there
 *   Else, are target's group members in sight?
 *     "Follow the wall" until my target is in sight.
 * Am I "chatty?" and nonhostile
 *   Am I near a sentient?
 *     If not, move toward one.
 *   else
 *     If so, chatter
 * Finally, if not in melee, even chance of:
 * -- cast attack spell
 * -- move toward player
 * -- use ranged weapon
 * else, fight in melee

Variables
 *  BuffCount -- each spell is +1, buff until equal to
 *    player, or Player+2 for BV_CLERIC
 

GetBehaviour()
 * BV_ANIMAL for animals

ChooseXXXXSpell()
 *   Chance of taking best spell == 
 *     100% - ((2nd Best Rating * 100)/Best Rating)
 *   Then repeat chance for 2nd best, etc, until one is
 *     chosen.
 *   Rating determined in part by % of remaining mana it
 *     would use.
 *   Value of 'curse'-type attack spells decreases relative
 *     to whether monster's winning and can afford them.

RelookSurrondings() when:
  -- monster enters a new region
  -- monster is initialized
  -- monster kills/acquires target
  -- RateAsTarget every item in range

RateAsTarget()
  * may choose to remember other target loc.
  * rates items as well as monsters
  * squabble patch here
  * called when item dropped/created

MoveTowardTarget()
  * "gravity" algorithm
  * Each target "pulls" monster toward it, divide pull by
      distance, multiply pull by intensity.
  * Open Doors Here
  * Handle incorporeality, displacing friendlies, push past
      in Walk

InitializeMonster()
 *  Start hidden?
 *  Start shapechanged?
 *  Start asleep?

SelectMonItems()
 hObj BestItems[MAX_SLOTS];
 * Scan through inventory:
 *   Which slot does item N go in?
 *     It N better than the current best for that slot?
 *       Then it's the new best.
 * If each best item isn't wielded, in slot order, do
 *   that and return.
 * Am I overtaxed?
 *   Drop worst item.
 * If the inventory is ideal, set MS_INVEN_GOOD, and
 *   return.
 * Choose best weapon up front, give IF_WORN
 * Choose best shield up front, give IF_WORN
 * Choose best armour up front, give IF_WORN
 * Choose best 2 rings up front, bracers, etc.
 
In Walk(EventInfo &e)
 * check sleeping monsters nearby
 * Move Silently reduces distance
 * High move silently removes Awaken possibility

ListSpells()
  * Flag so it's called only once every monster's turn
  * List every spell available to mon in array, along
      with how mon accesses it (innate, cast, item, etc.)
      and its EP_ constant.

hasSpellFor(int8)

 */

#define M_REVEAL_SHIFT       /* Reveal nature by shapeshift */
#define M_REVEAL_SPEC        /* Reveal nature when spec attack used */
#define M_REVEAL_CAST        /* Reveal when monster casts a spell */
#define M_REVEAL_THIEF       /* Reveal upon theft attack */
#define M_REVEAL_ATTACK      /* Reveal upon first attack */



#if 0
#define DF_FIRE  	0x0001
#define DF_COLD  	0x0002
#define DF_ACID  	0x0004
#define DF_ELEC  	0x0008
#define DF_POIS   0x0010
#define DF_NECR		0x0020
#define DF_PSYC   0x0040
#define DF_MAGC   0x0080

#define DF_SLEE   0x0100
#define DF_STUN   0x0200
#define DF_PLYS   0x0400
#define DF_STON		0x0800
#define DF_POLY		0x1000
#define DF_CHRM   0x2000
#define DF_DISN   0x4000
#define DF_TURN   0x8000

#define DF_SPEL   0x00010000  /* Target used spells; cast silence/dispel */
#define DF_MONK   0x00020000  /* Target used martial arts / unarmed attacks */
#define DF_WEPN   0x00040000  /* Nice weapon -- disintegrate/disarm/telobject it */
#define DF_PSIO   0x00080000  /* Uses Psionics; cast mind shield */
#define DF_ILLU   0x00100000  /* Target turned invis/used ill. -- dispel/true seeing */
#define DF_SNEA   0x00200000  /* Target Hid In Shadows -- cast 'Call Light' */
#define DF_MISL   0x00400000  /* Target uses missles -- cast ghostform/prot. arrows */
#define DF_TELE   0x00800000  /* Target Teleports */
#define DF_SUMM   0x01000000  /* Target Summons */
#define DF_STEA   0x02000000  /* Does Target Resist Theft? */
#endif

const char * alliesShouldntCast[] = {
  "Obscurement", "Rainstorm", "Rusting Grasp", 
  "Natural Erosion", "Spook", "Deeper Darkness",
  "Whirlpool", "Ring of Blades", "Etherealness",
  "Blasphemy", "Grease", "Pyrotechnics", "Web",
  "Electric Loop", "Visage of Death", "Levitation",
  "Evard's Black Tentacles", "Cloudkill",
  "Dream", "Nightmare", "Lesser Symbol", 
  "Fireball", "Colour Burst", "Confusion",
  "Immortal Grandeur", "Hallucinatory Terrain",
  "Mirage Arcana", "Insect Plague",
  "Summon Swarm", NULL };
  
bool denySpell(Monster *m, rID eID)
  {
    TEffect *te = TEFF(eID);
    Creature *cr = m->ts.getLeader();
    
    const char* name = NAME(eID);

    if (strstr(name,"acid") || strstr(name,"vitriolic") ||
        (te->ef.eval == EA_BLAST && te->ef.xval == AD_ACID) ||
        te->HasFlag(EF_EVIL))
      if (m->isMType(MA_GOOD) || (cr && (cr->isMType(MA_GOOD)
           || (cr->isCharacter() && (((Character*)cr)->desiredAlign & AL_GOOD)))))
        return true;
    
    if (te->HasFlag(EF_CHAOTIC) && m->isMType(MA_LAWFUL))
      return true;
    if (te->HasFlag(EF_LAWFUL) && m->isMType(MA_CHAOTIC))
      return true;
    
    if (cr && cr->isPlayer())
      for (int16 i=0;alliesShouldntCast[i];i++)
        if (!stricmp(name,alliesShouldntCast[i]))
          return true;
        
    return false;
  }

Monster::Monster(rID _mID,int16 _Type)
	:Creature(_mID,_Type)
	{
    ASSERT(mID);
    mMana = TMON(_mID)->Mana;
    BuffCount = 0;
    GainStatiFromBody(_mID);
    SetImage();
	}

void Monster::ChooseAction()
{
  int32 i,j,k,best,tr; 
  int16 mtp, rtp, ev;
  Item *it; rID form1, form2; int8 res;
  Creature *c, *bc, *charmer;
  static int16 aCandidates[1024];
  static rID   sCandidates[1024];


  TMonster *tm    = TMON(mID),
           *ttm   = TMON(tmID),
           *f1, *f2;
  
  if (!m || (x==-1))
    {
      Timeout = max(Timeout,30);
      return;
    }

  StateFlags &= ~(MS_CASTING|MS_STILL_CAST);

  /* This is a theoretically unnecessary kludge added after a hostile
     wight seemed unable to spot my character Jan 21 2007, causing the
     system to retarget more regularly. */
  for (int16 xx=x-6;xx<=x+6;xx++)
    for (int16 yy=y-6;yy<=y+6;yy++)
      if (m->InBounds(xx,yy) && m->At(xx,yy).Contents)
        for (c = m->FCreatureAt(xx,yy);c;c=m->NCreatureAt(xx,yy))
          if (isHostileTo(c) && Perceives(c))
            if (!ts.hasTargetThing(c))
              ts.Retarget(this);

  if (HasStati(CHARMED,CH_COMPEL))
    StatiIterNature(this,CHARMED)
      if (S->Val == CH_COMPEL)
        {
          EvReturn r;
          PEVENT(EV_COMPULSION,this,S->eID,0,r);
          if (Timeout)
            return;
        }
    StatiIterEnd(this)

//RestartLoop:
  inMelee = false; isAfraid = false;
  isSick = false; isBlind = false; isPoisoned = false;
  isParalyzed = false; hasHunger = false; isDiseased = false;
  isBleeding = false; isNatBlind = false; hasTarget = false;
  isTAfraid = false; isHiding = false; isEnraged = false;
  isAvoiding = false; hasInnateSpellStati = false;
  isMount = false;
  mtarg = rtarg = NULL;
  mtp = -1; rtp = -1;

  if (!m || (x==-1))
  {
    Timeout = max(Timeout,30);
    return;
  }

  charmer = (Creature*)GetStatiObj(CHARMED,CH_CHARM);

#define sdist(vx,vy,tx,ty) max(MYABS(vx - tx),MYABS(vy - ty))

  if (ts.shouldRetarget) ts.Retarget(this,true);

RestartTargetLoop:
  for (i=0; i<ts.tCount; i++) {
    if (!ts.t[i].isValid())
      continue; 
    switch (ts.t[i].type) {
      case TargetInvalid:
        break; 

      case TargetWander:
        if (theGame->Turn > ts.t[i].why.turnOfBirth + 500)
          ts.Wanderlust(this);
      case TargetArea:
        if (ts.t[i].data.Area.x == x &&
            ts.t[i].data.Area.y == y) {
          ts.RemoveTargetNumber(i);
          goto RestartTargetLoop;
        }
        ts.t[i].vis = false;
        break; 

      case TargetItem:
        ASSERT(theRegistry->Exists(ts.t[i].data.Item.i));
        {
          Item * it = (Item *)ts.t[i].GetThing();
          if (!it->isRealTo(this)) {
            ts.RemoveTargetNumber(i);
            goto RestartTargetLoop;
            }          
          if (it->x == x && it->y == y) { 
            ts.RemoveTargetNumber(i);
            goto RestartTargetLoop;
          }
          ts.t[i].vis = Perceives(it);
        }
        break;

      case TargetLeader:
      case TargetSummoner:
      case TargetMaster:
      case TargetAlly:
        ASSERT(theRegistry->Exists(ts.t[i].data.Creature.c));
        {
          Creature * it = (Creature *)ts.t[i].GetThing();
          /*if (it->x == x && it->y == y) { 
            ts.RemoveTargetNumber(i);
            goto RestartTargetLoop;
            }*/
          ts.t[i].vis = Perceives(it);
        }
        break; 

      case TargetEnemy:
      case OrderAttackTarget:
        ASSERT(theRegistry->Exists(ts.t[i].data.Creature.c));
        {
          Creature * it = (Creature *)ts.t[i].GetThing();
          if ((it->x == x && it->y == y) || (it->isDead()) ||
               (!it->isRealTo(this)) || (isIllusion() &&
                 it->HasStati(ASLEEP))) { 
            ts.RemoveTargetNumber(i);
            goto RestartTargetLoop;
          }
          ts.t[i].vis = Perceives(it);
          if (ts.t[i].vis && it != charmer) {
            hasTarget = true; 
            if ((isBeside(it) && (StateFlags & MS_REACH_ONLY)))
              isAvoiding = true; // ww: get out of there!
            else if (isBeside(it) ||
                ((StateFlags & MS_HAS_REACH) && sdist(it->x,it->y,x,y)==2)) {
              inMelee = true;
              if (ts.t[i].priority > mtp) { 
                mtarg = (Creature*)it;
                mtp = ts.t[i].priority;
              }
            } else { // not beside us
              if (ts.t[i].priority > rtp) { 
                rtarg = (Creature*)it;
                rtp = ts.t[i].priority;
              }
            } 
          }
        }
        break; 
    }
  } 

  isPoisoned = HasStati(POISONED);
  isDiseased = HasStati(DISEASED);
  StatiIterNature(this,AFRAID)
      if (S->Source != SS_MONI) isTAfraid = true;
      else if (cHP * 3 > (mHP + Attr[A_THP])) {
        if (S->Val == FEAR_HP && S->Source == SS_MONI) {
          IDPrint(NULL,"The <Obj> turns to fight!",this);
          StatiIter_RemoveCurrent(this);
        } 
      } else if (cMana()*2 > tMana()) {
        if (S->Val == FEAR_MANA && S->Source == SS_MONI) {
          IDPrint(NULL,"The <Obj> turns to cast spells!",this);
          StatiIter_RemoveCurrent(this);
        }
      } 
  StatiIterEnd(this)
  isAfraid = HasStati(AFRAID);
  isSick = HasStati(SICKNESS);
  isBlind = HasStati(BLIND);
  isParalyzed = HasStati(PARALYSIS);
  hasHunger = HasStati(HUNGER);
  isBleeding = HasStati(BLEEDING);
  isHiding = HasStati(HIDING);
  isStoning = HasStati(STONING);
  isEnraged = HasStati(ENRAGED);
  isGrabbed = HasStati(GRABBED) || HasStati(GRAPPLED);

  StatiIterAdjust(this)
      if (S->Mag < 0) isDrained = true;
  StatiIterEnd(this)

  isAvoiding = HasStati(AVOID_MELEE);
  hasInnateSpellStati = HasStati(INNATE_SPELL);
  isMount = HasStati(MOUNT);

  if (HasMFlag(M_BLIND))
    isBlind = isNatBlind = true;    
  if (HasMFlag(M_AVOID_MELEE))
    isAvoiding = true; 

  /* Attack mounts 1 in 3 times */
  if (!random(3)) {
    if (mtarg && mtarg->HasStati(MOUNTED))
      mtarg = (Creature*)mtarg->GetStatiObj(MOUNTED);
    if (rtarg && rtarg->HasStati(MOUNTED))
      rtarg = (Creature*)rtarg->GetStatiObj(MOUNTED);
  }

  /*if (reTargetLater)
    Retarget();*/

  ASSERT(x!=-1);               

  nAct = 0; 
  if (!(HasMFlag(M_PSYCHO) || HasMFlag(M_MINDLESS) || isEnraged || isIllusion())) {
    if (cHP*3 < (mHP+Attr[A_THP]) && !isAfraid)
    {
      if (Attr[A_MOV] > -20)
        IDPrint(NULL,"The <Obj> turns to flee!",this);
      GainTempStati(AFRAID,NULL,10+random(50),SS_MONI,FEAR_HP);
    }
    else if (cMana()*2 < tMana() && HasMFlag(M_CASTER) && !isAfraid)
    {
      if (Attr[A_MOV] > -20)
        IDPrint(NULL,"The <Obj> turns to flee!",this);
      GainTempStati(AFRAID,NULL,10+random(30),SS_MONI,FEAR_MANA);
    }
  }

  ASSERT(x!=-1);               
  ListEffects();
  ASSERT(x!=-1);               

  /* tm->Throw(EV_MONACT,this); */

#if 0                                    
  if (HasStati(OBJECTIVE))
  { Acts[nAct].Act = RES(GetStati(OBJECTIVE)->eID)->Query(QUE_MONACT);
    Acts[nAct++].Pri  = GetStati(OBJECTIVE)->Val; }        
#endif     


    if (tr = worstTrouble()) {
      if (isAfraid /* && !targVis */)
        AddAct(ACT_CAST,(tr/256)+1,NULL,EP_FIX_TROUBLE + ((tr & 0xFF)<<16));
      else
        AddAct(ACT_CAST,(tr/256)-1,NULL,EP_FIX_TROUBLE + ((tr & 0xFF)<<16));
    }
    ASSERT(x!=-1);               

    if ((StateFlags & MS_GUARDING) && !hasTarget)
      AddAct(ACT_WAIT,P_HIGH);

    best = 0; bc = NULL;
    if (isHiding && !isEnraged) {
      for(i=0;i!=8;i++)
        for (c = m->FCreatureAt(x+DirX[i],y+DirY[i]);c;
             c = m->NCreatureAt(x+DirX[i],y+DirY[i]))
          if ((int)isHostileTo(c) > best &&
               XPerceives(c))
          {
            best = isHostileTo(c);
            bc = c;
          }
      if (bc) {
        if (bc->HasStati(SLEEPING) || 
            bc->HasStati(PARALYSIS,PARA_HELD) ||
            (bc->HasStati(STUCK) && bc->HasStati(PRONE)))
          AddAct(ACT_COUPDEGRACE,P_URGENT,bc);
        if (AttackMode() == S_BRAWL || ((onPlane() != bc->onPlane()) &&
              (!InSlot(SL_WEAPON) || !InSlot(SL_WEAPON)->isMagic())))
          AddAct(ACT_NATTACK,P_URGENT,bc);
        else if (AttackMode() == S_MELEE)
          AddAct(ACT_WATTACK,P_URGENT,bc);
      }
    }

    ASSERT(x!=-1);               

    /* Later, set MS_SHIFTER in CalcValues */
    if (ttm->HasAttk(A_FORM) && ResistLevel(AD_POLY) != -1) {
      if (inMelee && ttm->isMType(tmID,MA_DRAGON)) {
        if (tm != ttm)
          AddAct(ACT_SHIFT,P_CRITICAL,NULL,0);
        goto SkipShifting;
      }

      form1 = form2 = 0;
      for(i=0;i!=NUM_TMON_ATTK;i++)
        if (ttm->Attk[i].AType == A_FORM) {
          if (form1) 
            form2 = ttm->Attk[i].u.xID;            
          else 
            form1 = ttm->Attk[i].u.xID;
        }
      f1 = TMON(form1);
      if (form2)
        f2 = TMON(form2);
      else
        f2 = NULL;
      
      if (tm == ttm && HasStati(ANIMAL_COMPANION))
        goto FormIsGood;
      
      if (!inMelee && !targVis && !isEnraged && FFCount >= 15) {

        /* If already disguised, don't change again. */
        if ((tm->isMType(form1,MA_HUMAN) && !ttm->isMType(tmID,MA_HUMAN)) 
            || (tm->HasFlag(M_HUMANOID) && !ttm->HasFlag(M_HUMANOID))
            /*|| (f1->isMType(MA_TOWNIE) && ttm->isMType(tmID,MA_TOWNSCUM))*/)
          goto FormIsGood;


        /* Take disguise form */
        if (f1 && tm != f1)
          if ((f1->isMType(form1,MA_HUMAN) && !ttm->isMType(tmID,MA_HUMAN)) 
              || (f1->HasFlag(M_HUMANOID) && !ttm->HasFlag(M_HUMANOID))
              /*|| (f1->isMType(MA_TOWNIE) && ttm->isMType(tmID,MA_TOWNSCUM))*/)
            AddAct(ACT_SHIFT,P_HIGH,NULL,1);

        if (f2 && tm != f2)
          if ((f2->isMType(form2,MA_HUMAN) && !ttm->isMType(tmID,MA_HUMAN)) 
              || (f2->HasFlag(M_HUMANOID) && !ttm->HasFlag(M_HUMANOID))
              /*|| (f2->isMType(MA_TOWNIE) && ttm->isMType(MA_TOWNSCUM))*/)
            AddAct(ACT_SHIFT,P_HIGH,NULL,2);
      }
      else if ((inMelee && !isAfraid) || isEnraged || HasStati(ANIMAL_COMPANION)) {
        /* Take War Form */
        /* Assumption: the "war form" is the one with the most
           hit dice */
        if (ttm->HitDice > tm->HitDice)
          AddAct(ACT_SHIFT,P_HIGH,NULL,0);
        if (f1 && f1->HitDice > tm->HitDice)
          AddAct(ACT_SHIFT,P_HIGH,NULL,1);
        if (f2 && f2->HitDice > tm->HitDice)
          AddAct(ACT_SHIFT,P_HIGH,NULL,2);
      }
      else if (isAfraid && !isEnraged) {
        /* Take Fleeing Form */
        /* Assumption: the "fleeing form" is the one with the 
           best Mov */
        if (ttm->Mov > tm->Mov)
          AddAct(ACT_SHIFT,P_HIGH,NULL,0);
        if (f1 && f1->Mov > tm->Mov)
          AddAct(ACT_SHIFT,P_HIGH,NULL,1);
        if (f2 && f2->Mov > tm->Mov)
          AddAct(ACT_SHIFT,P_HIGH,NULL,2);
      }
    }
    FormIsGood:
//#endif
    ASSERT(x!=-1);  
SkipShifting:             

    /* Later, set MS_HASPHASE in CalcValues() */
    if (HasAbility(CA_PHASE) && !isIllusion()) {
      if (isEnraged) {
        if (onPlane() != ((Creature*)GetStatiObj(ENRAGED))->onPlane())
          AddAct(ACT_PHASE,P_HIGH,NULL,0);
        }
      else if (isMount) {
        if (HasStati(PHASED) && onPlane() != ((Creature*)GetStatiObj(MOUNT))->onPlane())
          RemoveStati(PHASED);
        }
      else if ((isAfraid||isHiding) && !HasStati(PHASED))
        AddAct(ACT_PHASE,P_HIGH,NULL,0);
      else if (!isAfraid && inMelee && HasStati(PHASED))
        AddAct(ACT_PHASE,P_OVERRIDE,NULL,0);
    }

    if (!isHiding && !ts.hasTargetOfType(OrderDoNotHide) && 
           !isIllusion() && !HasStati(CHARGING)) {
      // ww: Hide() will take care of the rest -- whether anyone can see
      // you, if you have HideInPlainSight, etc. 
      if (HasMFlag(M_AQUATIC)) {
        if (TTER(m->PTerrainAt(x,y,this))->HasFlag(TF_WATER))
          AddAct(ACT_HIDE,P_HIGH,NULL,HI_WATER);
      } else if (HasMFlag(M_HIDE_ABOVE)) {
        /* Blackmantles, Lurkers Above, et al hide on the ceiling. How
           do they actually get up there? I haven't got a clue, and
           neither does the player. In truth, they just "float" up
           whenever the player can't see them, because anything else
           is too difficult to code. */
        if (!m->At(x,y).Visibility & VI_VISIBLE)
          AddAct(ACT_HIDE,P_HIGH,NULL,HI_CEILING);
      } else if (HasMFlag(M_HIDE_UNDER)) {
        if (m->FItemAt(x,y))
          if (TITEM(m->FItemAt(x,y)->iID)->Size >= tm->Size) 
            AddAct(ACT_HIDE,P_HIGH,NULL,HI_UNDER);
      } else if (ts.hasTargetOfType(OrderHide))
        AddAct(ACT_HIDE,P_URGENT,NULL,HI_SHADOWS);
      else if (HasSkill(SK_HIDE))
        AddAct(ACT_HIDE,P_HIGH,NULL,HI_SHADOWS);
    }    

    ASSERT(x!=-1);               

    if (!isTAfraid && !inMelee && !isEnraged && 
           !(isIllusion() && !isShade()) &&
           !ts.hasTargetOfType(OrderNoPickup)) {
      int16 ic;
      ic = 0;
      for (it=m->FItemAt(x,y);it;it=m->NItemAt(x,y)) {
        Target newT;
        ts.RateAsTarget(this,it,newT);
        i = newT.priority;
        if (ic > 20)
          break;
        if (i > 200) 
          AddAct(ACT_PICKUP,P_EXTREME,it);
        else if (i > 100) 
          AddAct(ACT_PICKUP,P_URGENT,it);
        else if (i > 50) 
          AddAct(ACT_PICKUP,P_HIGH,it);
        else if (i)
          AddAct(ACT_PICKUP,P_MODERATE,it);
        ic++;
        
        }
      }

    /* If our equipment selections aren't optimized, and we don't have
       anything better to do, we should optimize them. */
    if (!(StateFlags & MS_INVEN_GOOD))
      AddAct(ACT_EQUIP,P_MODERATE);

#if 0
    /* Turned off for now for efficiancy. Should search through the
       Targets array instead of using the grotesquely slow MapIterate.
       Also needs to check visibility, etc. Work on it later. */
    if (PartyID && !HasMFlag(M_MINDLESS) && !isEnraged) {
      best = 0; bc = NULL;
      MapIterate(m,c,i)
        if (c->isCreature())
          if (c->PartyID == PartyID)
            if (Perceives(c))
              if (j = c->worstTrouble())
                if (j > best)
                {
                  bc = c;
                  best = j;
                }
      if (bc)
        AddAct(ACT_CAST,max(0,(best / 256) - HasMFlag(M_EVIL))-1,bc,
            EP_FIX_TROUBLE + ((best & 0xFF)<<16));
    }
    /* Later, add support for buffing allies as well. */
#endif

    if ((!inMelee || !random(2)) && !isAfraid && !isEnraged && rtarg)
      AddAct(ACT_CAST,P_URGENT - (BuffCount/2) - (/* targVis ? 1 : */ 0),NULL,EP_BUFF);
    if (rtarg && !isEnraged)
      AddAct(ACT_CAST,P_LOW + random(4),rtarg,EP_SUMMON);
    else if (mtarg && !isEnraged)
      AddAct(ACT_CAST,P_LOW + random(4),mtarg,EP_SUMMON);

    if (BuffCount && !random(10))
      BuffCount--;

    /* Later, set MS_AVOID_MELEE in CalcValues() */
    if (inMelee && (isAfraid || isAvoiding) && !isEnraged)
      AddAct(ACT_CAST,P_URGENT + random(3),mtarg,EP_ESCAPE);

    if (isHiding && !isAfraid && rtarg && !isEnraged
        && (dist(rtarg->x,rtarg->y,x,y) < 6) && !random(3))
      AddAct(ACT_CAST,P_HIGH,rtarg,EP_STEALTH);

    /* Later, set MS_AVOID_MELEE in CalcValues() */
    if ((isAfraid || (inMelee && isAvoiding)) /* && !FoilCount */ && !isEnraged) {
      best = 30;
      /* Find the closest hostile */
      for(i=0; i<ts.tCount; i++)
        if ((ts.t[i].type == TargetEnemy ||
             ts.t[i].type == OrderAttackTarget) && ts.t[i].vis)
          if (ts.t[i].DistanceFrom(this) < best)
          {
            best = ts.t[i].DistanceFrom(this);
            c = (Creature*)oCreature(ts.t[i].data.Creature.c);
          }
      /* Cast something to impede its ability to pursue */                        
      if (best < 30)
        AddAct(ACT_CAST,(best/6)+1,c,EP_FOIL_PURSUIT);
    }

    /*
     * WW @  Thu Oct  7 21:21:35 PST 2004 -> foilcount is never set
     * anywhere, so we're commenting it out ...
     */
    /*
       if (FoilCount && targVis)
       FoilCount--;
     */

    if (isGrabbed && (HasMFlag(M_CASTER) || !random(4)))
      AddAct(ACT_SATTACK,P_HIGH,NULL,A_ESCA);

    /* This is the logic behind whether a monster will use its
       gate ability or not: it will only do so when facing another
       monster 3 CRs higher then itself, or when facing a player
       character (which, given the magic item christmas tree glow
       effect, it's safe to assume smart monsters like demons and
       celestials recognize). Monsters can only attempt to gate
       once a day, hence the SPEC_TIMEOUT. */
    if (ttm->HasAttk(A_GATE) && !HasStati(SPEC_TIMEOUT,A_GATE)) {
      /* weimer: also, gate when you are actually getting killed */
      if (mtarg && 
          (mtarg->ChallengeRating() >= ChallengeRating() + 3 ||
           mtarg->isPlayer() || isAfraid || cHP * 2 < mHP))
        AddAct(ACT_SATTACK,P_HIGH,mtarg,A_GATE);

      if (rtarg && 
          (rtarg->ChallengeRating() >= ChallengeRating() + 3 ||
           rtarg->isPlayer() || isAfraid || cHP * 2 < mHP))
        AddAct(ACT_SATTACK,P_HIGH,rtarg,A_GATE);
    } 

    /* Later, set MS_AVOID_MELEE in CalcValues() */
    if (!isAfraid && inMelee && mtarg && 
        (!isEnraged || mtarg == GetStatiObj(ENRAGED)))
    {
      /* Any Blast Spells */
      AddAct(ACT_CAST,P_URGENT+random(2),mtarg,EP_ATTACK|EP_CURSE);              

      if (hasEffFor & EP_DISPEL)
        if (mtarg->hMana > 30 || mtarg->isPlayer())
          {
            int16 manaCost, buffCount, sl;
            /*static int16 slots[] = { SL_AMULET, SL_LRING, SL_RRING,
                              SL_BRACERS, SL_READY, SL_WEAPON,
                              SL_CLOAK, SL_GAUNTLETS, SL_HELM }; */ 
            sl = random(9);
            mtarg->getBuffInfo(&buffCount,&manaCost,NULL);
            if (buffCount && manaCost > 150 && !random(2))
              AddAct(ACT_CAST,P_OVERRIDE,mtarg,EP_DISPEL);
            else if (buffCount && manaCost > 80) 
              AddAct(ACT_CAST,P_URGENT,mtarg,EP_DISPEL);
            else if (buffCount || (mtarg->InSlot(sl) &&
                      mtarg->InSlot(sl)->isMagic() && 
                      !mtarg->InSlot(sl)->HasStati(DISPELLED)))
              AddAct(ACT_CAST,P_MODERATE+random(2),mtarg,EP_DISPEL);
          }

      if (HasAttk(A_BREA) && !HasStati(SPEC_TIMEOUT,A_BREA))
        AddAct(ACT_SATTACK,P_URGENT,mtarg,A_BREA);
      if (HasAttk(A_BRE2) && !HasStati(SPEC_TIMEOUT,A_BRE2))
        AddAct(ACT_SATTACK,P_URGENT,mtarg,A_BRE2);
      if (HasAttk(A_SPIT) && !HasStati(SPEC_TIMEOUT,A_SPIT))
        AddAct(ACT_SATTACK,P_URGENT,mtarg,A_SPIT);

      if (HasAttk(A_ROAR) && !random(5))
        AddAct(ACT_SATTACK,P_HIGH,NULL,A_ROAR);

      if (HasAttk(A_EXPL))
        AddAct(ACT_SATTACK,P_HIGH,mtarg,A_EXPL);

      if ((!isAvoiding || isEnraged) && m->InBounds(mtarg->x,mtarg->y)) {
        /* Monsters Bull Rush enemies onto dangerous terrain */
        rID terID;
        i = DirTo(mtarg);
        terID = m->PTerrainAt(mtarg->x+DirX[i],mtarg->y+DirY[i],this);
        if (m->PTerrainAt(mtarg->x,mtarg->y,this) != terID)
          // ww: push monsters into terrain they wouldn't want to go into
          if ( TTER(terID)->HasFlag(TF_WARN) && 
              TTER(terID)->PEvent(EV_MON_CONSIDER,mtarg,mtarg->mID) == ABORT)
            if (Attr[A_STR] >= mtarg->Attr[A_STR])
              AddAct(ACT_SATTACK,P_URGENT,mtarg,A_BULL);
        // later: push them into features they wouldn't want to go over

        if (mtarg->HasStati(SLEEPING) || 
            mtarg->HasStati(PARALYSIS,PARA_HELD) ||
            (mtarg->HasStati(STUCK) && mtarg->HasStati(PRONE)))
          AddAct(ACT_COUPDEGRACE,P_URGENT,mtarg);
        /* Later, scan through array for is_special_attack, one for each */
        if (InSlot(SL_WEAPON) && !InSlot(SL_WEAPON)->HasIFlag(WT_NO_MELEE) &&
             ! ((onPlane() != mtarg->onPlane()) &&
              (!InSlot(SL_WEAPON) || !InSlot(SL_WEAPON)->isMagic()))) 
          AddAct(ACT_WATTACK,P_HIGH,mtarg);
        else
          AddAct(ACT_NATTACK,P_HIGH,mtarg);

        /* weimer: trip, disarm */
        int16 tripChance = 2;
        if (InSlot(SL_WEAPON) && InSlot(SL_WEAPON)->HasIFlag(WT_SUPER_TRIP))
          tripChance += 20;
        if (HasFeat(FT_MASTER_TRIP))
          tripChance += tripChance + 30; 
        if (random(100) < tripChance)
          AddAct(ACT_TRIP,P_HIGH,mtarg);

        ASSERT(mtarg->x != 6000);
        int16 disChance = 2;
        if (InSlot(SL_WEAPON) && InSlot(SL_WEAPON)->HasIFlag(WT_SUPER_DISARM))
          disChance += 20;
        if (HasFeat(FT_MASTER_DISARM))
          disChance += disChance + 30; 
        if (random(100) < disChance)
          AddAct(ACT_DISARM,P_HIGH,mtarg);
      }
    }

    if (!isAfraid && !inMelee && rtarg && !isEnraged &&
        m->LineOfFire(x,y,rtarg->x,rtarg->y,this))
      {
        if (dist(x,y,rtarg->x,rtarg->y) < 3 + ChallengeRating() / 2) {
          if (HasAttk(A_BREA) && !HasStati(SPEC_TIMEOUT,A_BREA))
            AddAct(ACT_SATTACK,P_URGENT,rtarg,A_BREA);
          if (HasAttk(A_BRE2) && !HasStati(SPEC_TIMEOUT,A_BRE2))
            AddAct(ACT_SATTACK,P_URGENT,rtarg,A_BRE2);
          if (HasAttk(A_SPIT) && !HasStati(SPEC_TIMEOUT,A_SPIT))
            AddAct(ACT_SATTACK,P_URGENT,rtarg,A_SPIT);
          } 
          
        if (HasAttk(A_ROAR))
          AddAct(ACT_SATTACK,P_MODERATE,NULL,A_ROAR);

        /* Ranged Combat */
        if (it = InSlot(SL_ARCHERY)) {
          if (it->needsToBeCocked())
            AddAct(ACT_LOADXBOW,P_MODERATE);
          else 
            AddAct(ACT_RATTACK,P_HIGH,rtarg);
          }
//CantShoot:

        /* Any Blast Spells */
        if (rtarg)
          AddAct(ACT_CAST,P_URGENT+random(2),rtarg,EP_ATTACK|EP_CURSE);              
        
        if (hasEffFor & EP_DISPEL)
          if (rtarg->hMana > 30 || rtarg->isPlayer())
            {
              int16 manaCost, buffCount, sl;
              /*static slots[] = { SL_AMULET, SL_LRING, SL_RRING,
                                SL_BRACERS, SL_READY, SL_WEAPON,
                                SL_CLOAK, SL_GAUNTLETS, SL_HELM }; */
              sl = random(9);
              rtarg->getBuffInfo(&buffCount,&manaCost,NULL);
              if (buffCount && manaCost > 150 && !random(2))
                AddAct(ACT_CAST,P_OVERRIDE,rtarg,EP_DISPEL);
              else if (buffCount && manaCost > 80) 
                AddAct(ACT_CAST,P_URGENT,rtarg,EP_DISPEL);
              else if (buffCount || (rtarg->InSlot(sl) &&
                        rtarg->InSlot(sl)->isMagic() && 
                        !rtarg->InSlot(sl)->HasStati(DISPELLED)))
                AddAct(ACT_CAST,P_MODERATE+random(2),rtarg,EP_DISPEL);
            }

      }

    /* ww: we would never throw chakrams because we never add
     * ACT_THROW to the list */ 
    if (!isAfraid && (rtarg || mtarg) && !isEnraged) {
      it = oItem(Inv);
      while (it && !(TITEM(it->iID)->Group & WG_THROWN))
        it = oItem(it->Next);
      if (it) {
        AddAct(ACT_THROW,P_HIGH,rtarg ? rtarg : mtarg);
      } 
    }

    if (!inMelee || isAfraid || isAvoiding || 
        (isEnraged && !isBeside(GetStatiObj(ENRAGED))))
    {    
      /* Movement */
      if (isMount || (HasStati(HIDING,HI_UNDER) && !isEnraged))
        ;
      else if (isEnraged) {
        if (!isBeside(GetStatiObj(ENRAGED)))
          AddAct(ACT_MOVE,P_HIGH);
      }
      else if (inMelee || random(5)) {
        AddAct(ACT_MOVE,P_LOW);
      } else {
        AddAct(ACT_MOVE,P_URGENT);
      }
    }

    /************************************************************************
     *                         MONSTER AI -- Stage Two                      *
     *   At this point, we've collated all the options the monster has open *
     * to it in the various Acts/Pri/ATar arrays. We now want to choose one *
     * by scanning through the array, and choosing as random option from    *
     * those with the highest priority (or, for an erratic monster, the two *
     * highest priority catagories.                                         *
     ************************************************************************/
    j = 0;
    for (i=0;i<nAct;i++) 
      for (int prioCount = 0; prioCount < Acts[i].Pri; prioCount++) {
        /*
        IPrint(Format("aCandidates[%d] = Act[%d] = %d.",j,i,Acts[i].Act));
        */
        aCandidates[j++] = i; 
      }
    if (!j) {
NothingToDo: 
      // mounts often have no viable action -- so avoid the
      // message spam about them not knowing what to do.
      if (isMount) { 
        Timeout += 10; 
        return; 
      }

      // leave a message so that we notice when this happens!

#if 0
      if (!HasMFlag(M_MINDLESS)) {
        String msg ; 
        if (!j)
          msg = Format("%s seems uncertain about what to do.",
              (const char*)Name(NA_THE)); 
        else if (best == P_IMPOSSIBLE)
          msg = Format("%s seems frustrated and uncertain.",
              (const char*)Name(NA_THE)); 
        else 
          msg = Format("%s seems to be considering its options.",
              (const char*)Name(NA_THE)); 
        // weimer debugging
        /*
           msg += Format(" (nAct=%d, j=%d, trials=%d).",nAct, j,trialsLeft);
           for (i=0;i<nAct;i++) 
           msg += Format("{T=%d,P=%d}",Acts[i].Act,Acts[i].Pri);
         */
        IDPrint(NULL,(const char*)msg);
        ts.Retarget(this); 
      }
#endif
      Timeout += 15;
      return;
    }
    // ww: now mCandidates has a priority-weighted list of things to try
    const int maxCandidate = j; 
    int curCandidate = random(j); 
    int trialsLeft = j + 1; 
    
    while (1) { 
      PurgeStrings();
      j = aCandidates[curCandidate]; 
      /*
      IPrint(Format("%d = Act[%d = aCandidates[%d]] (%d).",
            Acts[j].Act,j,curCandidate,trialsLeft));
            */
      res = NOTHING;
      if (isIllusion())
        switch (Acts[j].Act) {
          case ACT_PICKUP:
          case ACT_COUPDEGRACE:
            if (!(getIllusionFlags() & IL_SHADE))
              res = ABORT;
           break;               
          case ACT_SATTACK:
          case ACT_CAST:
          case ACT_PHASE:
          case ACT_SHIFT:
            if (!(getIllusionFlags() & IL_ADVANCED))
              res = ABORT;
           break; 
          }
      if (Acts[j].Tar && (onPlane() != Acts[j].Tar->onPlane()))    
        {
          int16 tp = Acts[j].Tar->onPlane();
          if (HasAbility(CA_PHASE) && Acts[j].Act != ACT_CAST)
            if (tp == 0 || tp == AbilityLevel(CA_PHASE))
              { res = Throw(EV_PHASE,this);
                goto DonePhaseClause; }
          if (Acts[j].Act == ACT_NATTACK && !HasAttk(A_TUCH) &&
                !HasStati(MANIFEST))
            { res = ABORT; goto DonePhaseClause; }
          if (Acts[j].Act == ACT_WATTACK && (!InSlot(SL_WEAPON) ||
                !InSlot(SL_WEAPON)->isGhostTouch()))
            { res = ABORT; goto DonePhaseClause; }
          if (Acts[j].Act == ACT_RATTACK && (!InSlot(SL_ARCHERY) ||
                !InSlot(SL_ARCHERY)->isGhostTouch()))
            { res = ABORT; goto DonePhaseClause; }
          if (Acts[j].Act == ACT_SATTACK)
            { res = ABORT; goto DonePhaseClause; }
          DonePhaseClause:;
        }
      if (res == NOTHING) switch(Acts[j].Act) {
        case ACT_WAIT:
          Timeout += 15;
          res = DONE;
          break; 
        case ACT_MOVE:
          // IPrint(Format("%s is moving.",Name()));
          res = Movement();
          break;
        case ACT_EQUIP:
          res = Inventory();
          break;
        case ACT_PICKUP:
          res = Throw(EV_PICKUP,this,NULL,Acts[j].Tar);
          if (res != ABORT)
            StateFlags &= ~MS_INVEN_GOOD;
          break;
        case ACT_LOADXBOW:
          res = Throw(EV_LOADXBOW,this,NULL,InSlot(SL_ARCHERY));
          break;
        case ACT_NATTACK:              
          res = Throw(EV_NATTACK,this,Acts[j].Tar);
          break;
        case ACT_WATTACK:
          if (Acts[j].Tar->isCreature())
            if (((Creature*)Acts[j].Tar)->getDef() > GetAttr(A_HIT_MELEE) + 18)
              if (((Creature*)Acts[j].Tar)->getDef() < GetAttr(A_HIT_MELEE) + 19 +
                   (SkillLevel(SK_FIND_WEAKNESS) / 2))
                if (HasSkill(SK_FIND_WEAKNESS) && !random(3))
                  {
                    res = ThrowVal(EV_SATTACK,A_PREC,this,Acts[j].Tar);
                    break;
                  }
          res = Throw(EV_WATTACK,this,Acts[j].Tar);
          break;
        case ACT_DISARM:
          res = ThrowVal(EV_SATTACK,A_DISA,this,Acts[j].Tar);
          break;
        case ACT_TRIP:
          res = ThrowVal(EV_SATTACK,A_TRIP,this,Acts[j].Tar);
          break;
        case ACT_COUPDEGRACE:
          res = ThrowVal(EV_SATTACK,A_COUP,this,Acts[j].Tar);
          break;
        case ACT_RATTACK:
          it = oItem(Inv);
          while (it && !it->isType(T_MISSILE))
            it = oItem(it->Next);
          if (!it)
          { res = ABORT; break; }
          it = it->TakeOne();
          res = Throw(EV_RATTACK,this,Acts[j].Tar,InSlot(SL_ARCHERY),it);
          break;
        case ACT_THROW:
          it = oItem(Inv);
          while (it && !(TITEM(it->iID)->Group & WG_THROWN))
            it = oItem(it->Next);
          res = Throw(EV_RATTACK,this,Acts[j].Tar,NULL,it);
          break;
        case ACT_SATTACK:
          res = ThrowVal(EV_SATTACK,Acts[j].Val,this,Acts[j].Tar);
          break;
        case ACT_HIDE:
          res = ThrowVal(EV_HIDE,Acts[j].Val,this);
          break;
        case ACT_SHIFT:
          switch (Acts[j].Val) {                                       
            case 0: res = ThrowEff(EV_SHIFT,tmID ,this); break;
            case 1: res = ThrowEff(EV_SHIFT,form1,this); break;
            case 2: res = ThrowEff(EV_SHIFT,form2,this); break;
          }
          break;
        case ACT_PHASE:
          res = Throw(EV_PHASE,this);
          break;
        case ACT_CAST:
         {
          ev = 0; it = NULL;
          if (!nEff || isEnraged) 
            {
              res = ABORT;
              break;
            }
          best = 0; k = -1;
          /*
             for(i=0;i!=nEff;i++)
             if ((Effs[i].Purpose & 0x0000FFFF) & (Acts[j].Val & 0x0000FFFF))
             if ((Effs[i].Purpose & 0xFFFF0000) == (Acts[j].Val & 0xFFFF0000)) 
             if (Effs[i].Rating > best) {
             best = Effs[i].Rating;
             k = i;
             }
             if (k == -1)
             {                           TROUBLE_INJURY
             res = ABORT;              EP_BUFF
             break;
             }
           */
          //bool crossphase = (Acts[j].Tar && Acts[j].Tar->onPlane() != onPlane());
          for(i=0,k=0;i!=nEff;i++)
            if ((Effs[i].Purpose & 0x0000FFFF) & (Acts[j].Val & 0x0000FFFF))
              if ((Effs[i].Purpose & 0xFFFF0000) == (Acts[j].Val & 0xFFFF0000))  {
                /*if (crossphase && Effs[i].eID && (Effs[i].Purpose & EP_ATTACK))
                  if (!(TEFF(Effs[i].eID)->Schools & SC_FORCE))
                    goto UnusableEffect;*/
                sCandidates[k++] = i;
//UnusableEffect:;
                } 
          if (k == 0)
          {
            res = ABORT;
            break;
          }
          k = sCandidates[random(k)];

          /*
          if (Effs[k].eID == FIND("command"))
            if (Acts[j].Tar == this)
              BREAKOUT
          */
          
          if (Effs[k].Source == NULL)
            ev = EV_CAST;
          else if (Effs[k].Source == ((Item*)1))
            ev = EV_INVOKE;
          else {
            it = Effs[k].Source;
            switch(it->Type) {
              case T_WAND:
                ev = EV_ZAP;
                break;
              case T_POTION:
                ev = EV_DRINK;
                break;
              case T_SCROLL:
                ev = EV_READ;
                break;
              default: 
                ev = EV_ACTIVATE; break;
            }
          }

          TEffect *te = TEFF(Effs[k].eID);
          if (te->ef.eval == EA_HEALING && isMType(MA_UNDEAD)) {
            // that won't work! 
            res = ABORT; break; 
          } 
          else if (Effs[k].Purpose & AIM_AT_ENEMY) {
            if (Acts[j].Tar == this || !Acts[j].Tar)
              Acts[j].Tar = (mtarg ? mtarg : rtarg);
            if (!Acts[j].Tar)
            { res = ABORT; break; }
          }
          else if (Effs[k].Purpose & AIM_AT_ALLY) {
            if (!Acts[j].Tar)
              Acts[j].Tar = this;
            if ((!Acts[j].Tar->isCreature()) || isHostileTo((Creature*)Acts[j].Tar))
            { res = ABORT; break; }
          }
          else if (Effs[k].Purpose & AIM_AT_SELF)
            Acts[j].Tar = this;
          else if (Effs[k].Purpose & AIM_AT_DOOR) {
            /* Implement Later */
            res = ABORT;
            break;
            }
          
          if (te->ef.aval == AR_TOUCH) {
            if (Acts[j].Tar && !isBeside(Acts[j].Tar)) 
            {
              /* If we need to cast a touch spell, and we aren't
                 in range, move toward the intended target this
                 turn. */
              res = Movement();
              break;
            }
          }

          /* We need to determine if our target is a valid target for
             this specific effect. This prevents monsters from casting
             Blight Plants on adventurers or Charm Person on golems,
             for example. */
          static EventInfo xe;
          xe.Clear();
          xe.EActor = this;
          xe.ETarget = Acts[j].Tar;
          xe.EMagic = &TEFF(Effs[k].eID)->ef;
          xe.Event = ev;
          xe.eID = Effs[k].eID;
          xe.MM  = 0; /* SetMetamagic... (later, ThrowEff -> ReThrow) */

          if (te->ef.qval & (Q_TAR|Q_DIR) || !(te->ef.qval & Q_INV))
            if (Acts[j].Tar && !isTarget(xe,Acts[j].Tar))
            { 
              res = ABORT; 
              break; 
            }  

          // weimer: here is the code where we avoid hurting our team
          // with fireballs!
          if (Effs[k].Purpose & EP_ATTACK) {
            bool isBall = te->ef.aval == AR_BALL;
            bool isBeam = te->ef.aval == AR_BEAM;
            switch (te->ef.aval) {
              case AR_BEAM: 
              case AR_BALL:
              case AR_BOLT:
                {
                  Creature * victim[1024];
                  int16 numVic;
                  if (ev == EV_INVOKE)
                    xe.vCasterLev = ChallengeRating();
                  else if (ev == EV_CAST)
                    xe.vCasterLev = CasterLev();
                  xe.vRange = xe.EMagic->lval;
                  CalcEffect(xe);
                  Magic::PredictVictimsOfBallBeamBolt(xe, isBeam, 
                      isBall, false, victim, numVic);
                  int anyAllies = 0;
                  int anyEnemies = 0;
                  bool hitMe = false; 
                  for (int i = 0; i<numVic ;i++) 
                    if (victim[i] == this) hitMe = true; 
                    else switch (ts.SpecificHostility(this,victim[i]).quality) {
                      case Enemy: anyEnemies ++; break;
                      case Ally: anyAllies ++; break;
                      case Neutral: break; /* TODO: good casters won't hurt innocent bystanders */
                    }
                  if (anyAllies || !anyEnemies) {
                    /*
                       theGame->GetPlayer(0)->IPrint(Format("-- predict %d %d (%d)", anyAllies, anyEnemies, numVic));
                     */
                    res = ABORT;
                  }
                  else if (hitMe && !te->HasFlag(EF_CASTER_IMMUNE)) {
                    res = ABORT;
                  }
                } break; 
              default: break; 
            } 
            if (res == ABORT) {
              break; 
            }
          }

          if (te->ef.qval & Q_TAR)
            res = ThrowEff(ev,Effs[k].eID,this,Acts[j].Tar,it);
          else if (TEFF(Effs[k].eID)->ef.qval & Q_LOC) {
            if (!Acts[j].Tar)
            { res = ABORT; break; }
            res = ThrowEffXY(ev,Effs[k].eID,Acts[j].Tar->x,Acts[j].Tar->y,this,NULL,it);
            break;
          }
          else if (te->ef.qval & Q_DIR) {
            if (Acts[j].Tar && (x == Acts[j].Tar->x || y == Acts[j].Tar->y || 
                  abs(x - Acts[j].Tar->x) == abs(y - Acts[j].Tar->y)))
              res = ThrowEffDir(ev,Effs[k].eID,DirTo(Acts[j].Tar),this,Acts[j].Tar,it);
            else
              res = ABORT;
          }
          else if (te->ef.qval & Q_INV) {
            Creature *c = (Creature*)(Acts[j].Tar); Item *inv_tar;
            if (!c)
              c = this;
            if (!c->isCreature())
            { res = ABORT; break; }
            if (te->HasFlag(EF_LIM_ITYPE) && te->ef.tval == T_ARMOUR)
              inv_tar = c->InSlot(SL_ARMOUR);
            else 
              inv_tar = c->InSlot(SL_WEAPON);
            if (inv_tar && isTarget(xe,inv_tar))
              res = ThrowEff(ev,Effs[k].eID,this,inv_tar,it);
            else
              res = ABORT;
          }
          else if (te->ef.aval == AR_GLOBE &&
              (!Acts[j].Tar || (DistFrom(Acts[j].Tar) <= /* effect range */ 6))) 
            res = ThrowEff(ev,Effs[k].eID,this,Acts[j].Tar,it);
          else
            res = ThrowEff(ev,Effs[k].eID,this,NULL,it);
          //ASSERT(res != NOTHING)
          break;
         }
         default:
           Error("Strange monster action! Unimplemented ACT_XXX!");
           Timeout += 30;
           return;
      }
      if (Acts[j].Act == ACT_CAST)
        if (res == DONE)
          if (Effs[k].Purpose & EP_BUFF)
            BuffCount++;

      if (isDead() || res == DONE || Timeout)
        return;

      trialsLeft--;
      if (!trialsLeft) goto NothingToDo;
      curCandidate = (curCandidate + 1) % maxCandidate; 
    } 
}

#define MAX_TARG_DIST   12      // Julian's knob
/* This function is called (by Thing::Move or Thing::PlaceAt) whenever
   a new Thing is put on a map for any reason (summoning, dropping an
   item, etc.) We evaluate its potential as a target for every nearby
   monster, and assign new TARGET stati if necessary. */
void Monster::AlertNew(Thing *nt)
{
  if (!theGame->InPlay() || !nt->m)
    return;

  for (int dx = -MAX_TARG_DIST; dx <= MAX_TARG_DIST; dx++) 
    for (int dy = -MAX_TARG_DIST; dy <= MAX_TARG_DIST; dy++) {
      int xx = nt->x + dx;
      int yy = nt->y + dy; 
      if (!nt->m->InBounds(xx,yy))
        continue; 
      for (Creature * cr = nt->m->FCreatureAt(xx,yy) ; cr ; cr = nt->m->NCreatureAt(xx,yy)) {
        cr->ts.Consider(cr,nt); 
      }
    }
}

void Monster::Initialize(bool in_play)
  {
    if (HasFeat(FT_IMPROVED_INITIATIVE))  // why not? 
      FFCount = 0;
    else 
      FFCount = 20;
    
    if (isMType(MA_SAPIENT) && !random(20) &&
          !HasMFlag(M_HOSTILE) && !isMType(MA_DEMON) &&
          !getLeader())
      StateFlags |= MS_PEACEFUL;
    
    CalcValues();
    CalcHP();
    cHP = mHP+Attr[A_THP];
     
    if ((!m) || (Flags & F_DELETE))
      return;
    
    // RemoveStati(TARGET);
    /* Every monster gets assigned the player as a target from day one.
       Why? Because freak circumstances (i.e., player teleporting from
       one room to another with the same region rID) might otherwise
       result in a player ending up in Dispater's plain sight without
       Dissy realizing there's a hostile present. */
    /*if (isMType(MA_SYLVAN))
      __asm int 3;*/
    
    /* ww: this is no longer necessary with the TargetSystem code
    for(i=0;i!=MAX_PLAYERS && (p = theGame->GetPlayer(i));i++) {     
      if (!HasStati(TARGET,TA_LEADER,p) && !(StateFlags & MS_PEACEFUL)) 
        if (isHostileTo(p))
          GainPermStati(TARGET,p,SS_MONI,TA_POTENTIAL,RateAsTarget(p),0);
      }
      */

    LastMoveDir = CENTER;

    if (m)
      ts.Retarget(this);

    if (m && !in_play && 
        (HasFeat(FT_BRACHIATION) || SkillLevel(SK_CLIMB) > 8) &&
        !HasStati(MOUNTED) && !HasStati(MOUNT) && 
        m->TreeAt(x,y)) 
      GainPermStati(ELEVATED,NULL,SS_MISC,ELEV_TREE,0);
    

    if (m && !in_play && !HasStati(HIDING) && !HasStati(MOUNT) &&
          !HasStati(ASLEEP)) {
      if (HasMFlag(M_AQUATIC) && TTER(m->TerrainAt(x,y))->HasFlag(TF_WATER)) {
        GainPermStati(HIDING,NULL,SS_CLAS,HI_WATER,SkillLevel(SK_HIDE));
        HideVal = random(11)-5;
      } else if (HasMFlag(M_HIDE_ABOVE)) {
        GainPermStati(HIDING,NULL,SS_CLAS,HI_CEILING,SkillLevel(SK_HIDE));
        HideVal = random(11)-5;
      } else if (HasMFlag(M_HIDE_UNDER) && m->FItemAt(x,y)) {
        if (TITEM(m->FItemAt(x,y)->iID)->Size >= TMON(mID)->Size) {
          GainPermStati(HIDING,NULL,SS_CLAS,HI_UNDER,SkillLevel(SK_HIDE));
          HideVal = random(11)-5;
        } 
      } else if (!m->BrightAt(x,y)) {
        GainPermStati(HIDING,NULL,SS_CLAS,HI_SHADOWS,SkillLevel(SK_HIDE));
        HideVal = random(11)-5;
      } 
      /* Start Shapechanged? */
    }

    if (m && HasAbility(CA_PHASE) && !isIllusion())
      if (!HasStati(PHASED) && !HasStati(MOUNT)) 
        GainPermStati(PHASED, NULL, SS_MISC, AbilityLevel(CA_PHASE));


    TMON(tmID)->PEvent(EV_INITIALIZE,this,tmID);

    StatiIterNature(this,TEMPLATE)
        TTEM(S->eID)->PEvent(EV_INITIALIZE,this,S->eID);
    StatiIterEnd(this)
    ASSERT(cHP == mHP + Attr[A_THP]);
    SetSilence();

    // WW: do this before Inventory() so that if we summon a Force
    // Construct Staff or whatever we can use that ...
    PreBuff();

    if (isDead()) return; 
    
    // ww: these guys won't start with silver or blessed items ...
    if (isMType(MA_DEMON) || isMType(MA_LYCANTHROPE) ||
        isMType(MA_DEVIL) || isMType(MA_UNDEAD)) {
      for(Item *it=FirstInv();it;it=NextInv()) {
        if (it->Material() == MAT_SILVER)
          it->RemoveQuality(IQ_SILVER);
        if (it->Material() == MAT_SILVER)
          it->Remove(true);
        if (it->isBlessed())
          it->IFlags &= ~IF_BLESSED; 
      } 
    } 
    if (isMType(MA_UNDEAD)) { // the undead often carry cursed items
      for(Item *it=FirstInv();it;it=NextInv()) {
        if (it->Type == T_WEAPON ||
            it->Type == T_ARMOUR ||
            it->Type == T_SHIELD ||
            it->Type == T_BOW ||
            it->Type == T_MISSILE ||
            random(100) > 20) continue; 
        it->IFlags &= IF_CURSED;
      } 
    } 

    
    if (onPlane() > PHASE_VORTEX && !HasStati(PHASED)) {
      for(Item *it=FirstInv();it;it=NextInv()) {
        it->GainPermStati(PHASED,NULL,SS_MISC,onPlane());
        } 
      }
    
    /* Illusions carry illusory items. However, we set the item's ILLUSION
       Stati object to the illsuionary creature rather than the caster, so
       that is the creature is killed/dispelled, all its items wink out at
       the same time. */
    if (isIllusion()) {
      Status *S = GetStati(ILLUSION);
      for (Item *it=FirstInv();it;it=NextInv())
        if (!it->HasStati(ILLUSION))
          it->GainPermStati(ILLUSION, this,SS_MISC,
            S->Val,S->Mag,S->eID,S->CLev);
      }
      
    for (int16 count=0; count<5 && !(StateFlags & MS_INVEN_GOOD); count++) 
      Inventory();
      
    StateFlags |= MS_INVEN_GOOD; 

    Timeout = 0; // ww: TakeOff when picking items might increment this
    UnsetSilence();
    
    if (!TMON(tmID)->HasAttk(A_SEEM)) {
      StateFlags |= MS_KNOWN;
      }
    else {
      ASSERT(TMON(tmID)->GetAttk(A_SEEM)->u.xID);
      }
    if (InSlot(SL_WEAPON)) {
      if (HasFeat(FT_WEAPON_MASTERY))
        GainPermStati(WEP_SKILL,NULL,SS_RACE,WS_MASTERY,
          0,InSlot(SL_WEAPON)->iID);    
      else if (HasFeat(FT_WEAPON_SPECIALIST))
        GainPermStati(WEP_SKILL,NULL,SS_RACE,WS_SPECIALIST,
          0,InSlot(SL_WEAPON)->iID);    
      else if (HasFeat(FT_WEAPON_FOCUS))
        GainPermStati(WEP_SKILL,NULL,SS_RACE,WS_FOCUSED,
          0,InSlot(SL_WEAPON)->iID);    
      else if (HasFeat(FT_EXOTIC_WEAPON))
        GainPermStati(WEP_SKILL,NULL,SS_RACE,WS_PROFICIENT,
          0,InSlot(SL_WEAPON)->iID);    
      }
    #ifndef NO_FACE
    /* Later, this should be added to CalcValues, in the same
       section as the weapon-dropping code for changes in A_SIZ. */
    if (Attr[A_SIZ] >= SZ_HUGE) {
      if (m) { 
        m->NewField(FI_SIZE|FI_MOBILE,x,y,FaceRadius[Attr[A_SIZ]],
            GLYPH_BULK | (Image & 0x0F00),-1,0,this);
        PlaceNear(x,y);
        }
      }
    #endif

    CalcHP();
    GainStatiFromBody(mID);
    cHP = mHP+Attr[A_THP];
     

    Item *it;
    if (HasStati(ASLEEP))
      for (it=FirstInv();it;it=NextInv())
        if (it->isType(T_ARMOUR))
          if (it->IFlags & IF_WORN)
            {
              it->IFlags &= ~IF_WORN;
              StateFlags &= ~MS_INVEN_GOOD;
            }
    for(it=FirstInv();it;it=NextInv())
      it->IFlags |= IF_PROPERTY;
      
    if (HasStati(SUMMONED))
      for (it=FirstInv();it;it=NextInv())
        it->GainPermStati(SUMMONED,this,SS_ENCH);
  }




Dir Monster::SmartDirTo(int16 tx, int16 ty, bool is_pet)
  {
    static uint16 thePath[MAX_PATH_LENGTH];
    if (!m->InBounds(tx,ty))
      return CENTER;
    if (theGame->Opt(OPT_MON_DJIKSTRA) == 1 || 
        (theGame->Opt(OPT_MON_DJIKSTRA) == 2 && is_pet)) {
      if (m->LineOfFire(x,y,tx,ty,this))
        return DirTo(tx,ty);
      bool blocked; int16 dx, dy;
      blocked = !m->ShortestPath(x,y,tx,ty,this,0,thePath);
      if (blocked)
        return CENTER;
      dx = thePath[1] % 256;
      dy = thePath[1] / 256;
      return DirTo(dx,dy);
      }
    else
      return DirTo(tx, ty);
  }



EvReturn Monster::Movement() {
    int16 d,i; Dir *dirlist, tDir; 
    bool nearWall, isIncor, oppViable, canMeld;
    Feature *f; Creature *follow;
    bool isAfraid = HasStati(AFRAID);

    if (ts.hasTargetOfType(OrderStandStill))
        return ABORT; 

    /* If you ALREADY ARE near Fearless Leader, don't
    either walk away, or walk in circles. */
    if (ts.hasTargetOfType(OrderWalkNearMe)) {
        Target *t;
        if ((getLeader() && isBeside(getLeader())) ||
            ((t=ts.getTargetOfType(OrderWalkNearMe)) &&
            t->GetThingOrNULL() && isBeside(t->GetThing())))
            if (!rtarg)
                return ABORT;
    }


    /* The rules of monster movement are thus:

    * Every target on the map has an x/y gravity of its own, 
    based on its RateAsTarget intensity, the distance we
    are from it, whether its visible, et al.
    * The number one rule is that we never move in the exact
    opposite direction of the direction we moved last round,
    unless it's the *only* move possible.
    * If we don't have a clear path to any of our targets, we
    never move in a way that will take us away from all walls.
    This gives monsters a fairly decent 'maze search' algorithm
    to outwit players trying to hide.
    * We always move, even if it's in the exact opposite direction
    that we really want to be going. The only exceptions to this
    are the two rules immediately above.

    */
    int32 wx, wy; int8 tx,ty;

    static Dir EastDirs[] = 
    { EAST, SOUTHEAST, NORTHEAST, SOUTH, NORTH, NORTHWEST, SOUTHWEST, WEST, -1 };
    static Dir WestDirs[] = 
    { WEST, NORTHWEST, SOUTHWEST, SOUTH, NORTH, NORTHEAST, SOUTHEAST, EAST, -1 };
    static Dir NorthDirs[] = 
    { NORTH, NORTHWEST, NORTHEAST, EAST, WEST, SOUTHWEST, SOUTHEAST, SOUTH, -1 };
    static Dir SouthDirs[] = 
    { SOUTH, SOUTHEAST, SOUTHWEST, WEST, EAST, NORTHWEST, NORTHEAST, NORTH, -1 };
    static Dir NEDirs[] = 
    { NORTHEAST, NORTH, EAST, NORTHWEST, SOUTHEAST, SOUTH, WEST, SOUTHWEST, -1 };
    static Dir SEDirs[] = 
    { SOUTHEAST, SOUTH, EAST, NORTHEAST, SOUTHWEST, NORTH, WEST, NORTHWEST, -1 };
    static Dir NWDirs[] = 
    { NORTHWEST, NORTH, WEST, NORTHEAST, SOUTHWEST, SOUTH, EAST, SOUTHEAST, -1 };
    static Dir SWDirs[] = 
    { SOUTHWEST, SOUTH, WEST, NORTHWEST, SOUTHEAST, NORTH, EAST, NORTHEAST, -1 };

    // IDPrint(NULL,(Format("%s moving from (%d,%d).",(const char*)Name(), x, y))); 

    wx = wy = 0; dirlist = NULL;
    bool any_objective;
    any_objective = false;

RetryTargets:
    for (i=0; i<ts.tCount; i++) {
        if (!ts.t[i].isValid()) 
            continue; 

        switch (ts.t[i].type) {
        case TargetEnemy:
        case OrderAttackTarget:
            if (ts.t[i].vis == 0)
                continue;
        case TargetItem: 
        /*
        case TargetLeader:
        case TargetSummoner:
        case TargetMaster:
        case TargetAlly:
        */
        case TargetWander:
            any_objective = true;
            break; // these are OK things to chase! 
        default: 
            continue;
        } 

        /* If the enemy is getting too close and we're a ranged fighter,
        he instead becomes a strongly /negative/ gravity, repulsing the
        monster until it's back at a safe distance for it's favoured
        tactic of ranged combat. */
        d = max(1, ts.t[i].DistanceFrom(this));
        if (ts.t[i].type == TargetWander)
            d = 1;
        else if ((ts.t[i].type == TargetEnemy ||
            ts.t[i].type == OrderAttackTarget) && isAfraid)
            d = -d;
        else if ((d <= 4) && (ts.t[i].type == TargetEnemy ||
            ts.t[i].type == OrderAttackTarget) && 
            isAvoiding && !isEnraged)
            d = -1;
        else if ((d <= 5) && (ts.t[i].type == TargetEnemy ||
            ts.t[i].type == OrderAttackTarget) && isAvoiding &&
            !isEnraged && HasStati(AVOID_MELEE,-1,ts.t[i].GetThing()))
            d = -1;


        ASSERT(i >= 0);
        ASSERT(i < ts.tCount);
        tDir = SmartDirTo(ts.t[i].X(), ts.t[i].Y());
        if (tDir == CENTER)
            continue;
        wx += (100 * ts.t[i].priority / d) * DirX[tDir];
        wy += (100 * ts.t[i].priority / d) * DirY[tDir];
    }
    if (!any_objective) {
        ts.Wanderlust(this);
        any_objective = true;
        goto RetryTargets;
    }

    if (ts.hasTargetOfType(OrderWalkToPoint)) {
        Target * t = ts.getTargetOfType(OrderWalkToPoint);
        tDir = SmartDirTo(t->X(),t->Y(),true);
        if (tDir != CENTER) {
            wx = DirX[tDir];
            wy = DirY[tDir];
            goto DoneFollow;
        } 
    } 

    if (ts.hasTargetOfType(OrderWalkInFront)) {
        follow = ts.getLeader();
        if (ts.getTargetOfType(OrderWalkInFront)->GetThingOrNULL())
            follow = (Creature*)ts.getTargetOfType(OrderWalkInFront)->GetThing();
        if (!follow)
            goto DoneFollow;

        Dir lastDir = follow->LastMoveDir;
        tx = follow->x + (DirX[lastDir] * 3);
        ty = follow->y + (DirY[lastDir] * 3);
        tDir = SmartDirTo(tx,ty,follow->isPlayer());
        if (tDir != CENTER) {
            wx = DirX[tDir];
            wy = DirY[tDir];
            goto DoneFollow;
        } 
    } 

    if (ts.hasTargetOfType(OrderWalkInBack)) {
        follow = ts.getLeader();
        if (ts.getTargetOfType(OrderWalkInBack)->GetThingOrNULL())
            follow = (Creature*)ts.getTargetOfType(OrderWalkInBack)->GetThing();
        if (!follow)
            goto DoneFollow;

        Dir lastDir = follow->LastMoveDir;
        tx = follow->x - (DirX[lastDir]);
        ty = follow->y - (DirY[lastDir]);
        tDir = SmartDirTo(tx,ty,follow->isPlayer());
        if (tDir != CENTER) {
            wx = DirX[tDir];
            wy = DirY[tDir];
            goto DoneFollow;
        } 
    } 
    if (ts.hasTargetOfType(OrderWalkNearMe)) { 
        follow = ts.getLeader();
        if (ts.getTargetOfType(OrderWalkNearMe)->GetThingOrNULL())
            follow = (Creature*)ts.getTargetOfType(OrderWalkNearMe)->GetThing();
        if (!follow)
            goto DoneFollow;

        tDir = SmartDirTo(follow->x,follow->y,follow->isPlayer());
        if (tDir != CENTER) {
            wx = DirX[tDir];
            wy = DirY[tDir];
            goto DoneFollow;
        } 
    } 

    if ((follow = ts.getLeader()) && 
        (dist(x,y,follow->x,follow->y) > 3)) { 
            tDir = SmartDirTo(follow->x,follow->y,follow->isPlayer());
            if (tDir != CENTER) {
                wx = DirX[tDir];
                wy = DirY[tDir];
                goto DoneFollow;
            } 
    }
DoneFollow:

    if (abs(wx)*2 < abs(wy))
        wx = 0;
    else if (abs(wy)*2 < abs(wx))
        wy = 0;

    if (!wx && !wy) {
        if (/* targVis && --> an rtarg is always seen ... */ rtarg) {
            wx += DirX[DirTo(rtarg)];
            wy += DirY[DirTo(rtarg)];
        } else {
            if (!(StateFlags & MS_GUARDING) &&
                !ts.hasTargetOfType(TargetWander) && 
                !ts.getLeader())
                ts.Wanderlust(this);
            return ABORT;
        }
    }

    d = -1; 
    bool first = true;
    uint32 bf = 0; 
    for (i=0;m->Fields[i];i++) {
        if (first) {
            bf = BadFields(); // expensive calculation
            first = false;
        } 
        if (m->Fields[i]->FType & bf)
            if (dist(x,y,m->Fields[i]->cx,m->Fields[i]->cy) <= m->Fields[i]->rad)
                d = OppositeDir(DirTo(m->Fields[i]->cx,m->Fields[i]->cy));
    }
    if (isEnraged)
        d = DirTo(GetStatiObj(ENRAGED));
    if (d != -1) {
        wx += DirX[d]*1000;
        wy += DirY[d]*1000;
    }

    if (wx>0 && wy>0)
        dirlist = SEDirs;
    else if (wx>0 && wy<0)
        dirlist = NEDirs;
    else if (wx<0 && wy<0)
        dirlist = NWDirs;
    else if (wx<0 && wy>0)
        dirlist = SWDirs;
    else if (wx>0)
        dirlist = EastDirs;
    else if (wx<0)
        dirlist = WestDirs;
    else if (wy>0)
        dirlist = SouthDirs;
    else
        dirlist = NorthDirs;

    // IDPrint(NULL,(Format("%s moving via (%d,%d).",(const char*)Name(), wx, wy))); 

    ASSERT(dirlist);

    nearWall = m->besideWall(x,y);
    isIncor  = onPlane() > PHASE_VORTEX; 
    canMeld  = HasAbility(CA_EARTHMELD);
    oppViable = false;

    do {
        tx = x + DirX[*dirlist];
        ty = y + DirY[*dirlist]; 

        // IDPrint(NULL,(Format("%s (%d,%d)->(%d,%d)?.",(const char*)Name(), x, y,tx,ty))); 

        if (!m->InBounds(tx,ty))
            goto NoGood;

        if (f = m->FDoorAt(tx,ty)) {
            if (!isIncor && f->Type == T_DOOR) {
                /* Assumption: no Incor creatures can open doors. */
                if (!(((Door*)f)->DoorFlags & DF_OPEN))
                    OpenDoor((Door*)f);
                if (Timeout || !m)
                    return ABORT;
            }
        }
        if (!m)
            return ABORT;

        for (f=m->FFeatureAt(tx,ty);f;f=m->NFeatureAt(tx,ty)) {
            if (f->Flags & F_XSOLID)
                goto NoGood;
            if (!isIncor && f->Flags & F_SOLID)
                goto NoGood;
        }

        if (!isIncor && !canMeld && m->At(tx,ty).Solid)
            goto NoGood;

        /*
        if (nearWall && !targVis)
        if (!m->besideWall(tx,ty))
        goto NoGood;
        */
        /*
        if (*dirlist == OppositeDir(LastMoveDir))
        {
        oppViable = true;
        goto NoGood;
        }
        */

        Dir cdir;
        if ((cdir = GetStatiVal(CHARGING)) != -1)
            if (!isSimilarDir(cdir,*dirlist)) {
                RemoveStati(CHARGING);
                //IDPrint("","The <Obj1> breaks off <his:Obj1> charge.", this);
            }
        if (HasMFlag(M_CHARGE) || (InSlot(SL_WEAPON) && InSlot(SL_WEAPON)->HasIFlag(WT_CHARGE)))
            if (canChargeInDir(*dirlist) && !HasMFlag(M_NO_CHARGE) && !HasStati(CHARGING)) {
                GainPermStati(CHARGING,NULL,SS_MISC,*dirlist,0,0);
                //IDPrint("","The <Obj1> charges!", this);
            }

        if (ThrowDir(EV_MOVE,*dirlist,this) != ABORT) {
            LastMoveDir = *dirlist;
            goto DoneMove;
        }

        if (!m)
            return DONE;

NoGood:
        dirlist++;
    } while((*dirlist) != -1);

    if (oppViable)
        if (ThrowDir(EV_MOVE,OppositeDir(LastMoveDir),this) != ABORT) {
            LastMoveDir = OppositeDir(LastMoveDir);
            goto DoneMove;
        }

    return ABORT;

DoneMove:
    /* If we're wandering, and we're stuck in a corner, change the 
    direction that we wander in. */
    if ((x == Recent[0] && y == Recent[1]) || (x == Recent[2] && y == Recent[3]) || (x == Recent[4] && y == Recent[5]))
        if (ts.hasTargetOfType(TargetWander))
            ts.Wanderlust(this); 

    Recent[0] = Recent[2];
    Recent[1] = Recent[3];
    Recent[2] = Recent[4];
    Recent[3] = Recent[5];
    Recent[4] = x;
    Recent[5] = y;

    return DONE;
}

EvReturn Monster::OpenDoor(Door *d)
  {
    EvReturn r;
    /* The logic here is as follows: a monster will first
       try to open the door (if it has hands), then attempt
       to pick the lock (if it has Lockpicking), then it
       will attempt to kick the door 3 times (if it has
       legs and Strength 14+). Failing all this, we treat
       the door like a wall. */

    if (!HasMFlag(M_NOHANDS))
      if (!d->HasStati(TRIED,1,this))
        {
          d->GainPermStati(TRIED,this,SS_MISC,1);
          r = Throw(EV_OPEN,this,d);
          if (Timeout)
            return r;
        }

    if (d->GetStatiMag(TRIED,2,this) < 5)
    {
      if (d->HasStati(TRIED,2,this))
        d->SetStatiMag(TRIED,2,this,GetStatiMag(TRIED,2,this)+1);
      else
        d->GainPermStati(TRIED,this,SS_MISC,2,1,0,0);
      r = TryToDestroyThing(d);
      if (Timeout)
        return r;
    }

    return ABORT;    
  }

uint32 Monster::BadFields()
  {
    uint32 bf;
    bf = FI_BAD;

    if ((HasMFlag(M_CASTER) && !HasFeat(FT_VOCALIZE_SPELL)) ||
        (HasAbility(CA_BLINDSIGHT) && !HasAbility(CA_INFRAVISION)))
      bf |= FI_SILENCE;

    if (!HasAbility(CA_TREMORSENSE) && !HasAbility(CA_TELEPATHY) &&
        !HasAbility(CA_BLINDSIGHT) && !((Creature *)this)->isBlind())
      bf |= FI_DARKNESS;

    if (HasMFlag(M_LIGHT_AVERSE))
      bf |= FI_LIGHT;

    return bf;
  }

void Monster::ListEffects()
  {
    TMonster *tm = TMON(tmID); Annotation *a;
    TEffect *te; rID spList[512];
    int16 i,j,ns,c; Item *it;

    bool is_caster = false;
    int8 SpellLev[] = {
      0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 
      6, 6, 7, 7, 8, 8, 9, 9, 9, 9, 9, 9 };
    ns = theGame->LastSpell();
    nEff = 0;
    hasEffFor = 0;

    /* If we've turned monster casting off, then just
      return with an empty list. */
    if (theGame->Opt(OPT_NO_CASTING))
      return;


    bool skip; rID spID;
    int16 lev, clev, sp; 
    clev = AbilityLevel(CA_SPELLCASTING);
    if (!clev)
      goto SkipSpellcasting;
      
    if (tm->GetList(SPELL_LIST,spList,511))
      {
        skip = false;
        for (i=0;spList[i] || spList[i+1] || spList[i+2];i++)
          {
            if (spList[i] == 0)
              ;
            else if (spList[i] < 50)
              {
                lev = spList[i];
                if (spList[i] > SpellLev[clev])
                  skip = true;
                else
                  skip = false;
                continue;
              }
            else if (spList[i] > 1000 && spList[i] < 2000)
              {
                int16 Source = spList[i] - 1000;
                for(sp=0;sp!=theGame->LastSpell();sp++)
                  {
                    spID = theGame->SpellID(sp);
                    if (TEFF(spID)->Level > SpellLev[clev])
                      continue;
                    if (TEFF(spID)->HasSource(Source))
                      {
                        for (j=0;spList[j];j++)
                          if (spList[j] == spID)
                            goto SkipThisAdd;
                        AddEffect(TEFF(spID),spID,NULL);
                        SkipThisAdd:
                        ;
                      }
                  }
                  
              }
            else if (spList[i] == 100)
              {
                for(sp=0;sp!=theGame->LastSpell();sp++)
                  {
                    spID = theGame->SpellID(sp);
                    if (TEFF(spID)->Level > SpellLev[clev])
                      continue;
                    if (TEFF(spID)->HasSource(spList[i+1]))
                      if (TEFF(spID)->Schools & spList[i+2])
                        {
                          for (j=0;spList[j];j++)
                            if (spList[j] == spID)
                              goto SkipThisAdd2;
                          AddEffect(TEFF(spID),spID,NULL);
                          SkipThisAdd2:
                          ;
                        }
                  }
                i += 2;                      
              }
            else if (spList[i] && !skip)
              {
                AddEffect(TEFF(spList[i]),spList[i],NULL);
              }
          }
      }
      
    StatiIterNature(this,TEMPLATE)
      if (!TTEM(S->eID)->GetList(SPELL_LIST,spList,511))
        continue;
      skip = false;
      for (i=0;spList[i] || spList[i+1] || spList[i+2];i++)
        {
          if (spList[i] == 0)
            ;
          else if (spList[i] < 50)
            {
              lev = spList[i];
              if (spList[i] > SpellLev[clev])
                skip = true;
              else
                skip = false;
              continue;
            }
          else if (spList[i] > 1000 && spList[i] < 2000)
            {
              int16 Source = spList[i] - 1000;
              for(sp=0;sp!=theGame->LastSpell();sp++)
                {
                  spID = theGame->SpellID(sp);
                  if (TEFF(spID)->Level > SpellLev[clev])
                    continue;
                  if (TEFF(spID)->HasSource(Source))
                    {
                      for (j=0;spList[j];j++)
                        if (spList[j] == spID)
                          goto SkipThisAdd3;
                      AddEffect(TEFF(spID),spID,NULL);
                      SkipThisAdd3:
                      ;
                    }
                }
                
            }
          else if (spList[i] == 100)
            {
              for(sp=0;sp!=theGame->LastSpell();sp++)
                {
                  spID = theGame->SpellID(sp);
                  if (TEFF(spID)->Level > SpellLev[clev])
                    continue;
                  if (TEFF(spID)->HasSource(spList[i+1]))
                    if (TEFF(spID)->Schools & spList[i+2])
                      {
                        for (j=0;spList[j];j++)
                          if (spList[j] == spID)
                            goto SkipThisAdd4;
                        AddEffect(TEFF(spID),spID,NULL);
                        SkipThisAdd4:
                        ;
                      }
                }
              i += 2;                      
            }
          else if (spList[i] && !skip)
            {
              AddEffect(TEFF(spList[i]),spList[i],NULL);
            }
        }
    StatiIterEnd(this)
      
    SkipSpellcasting:;

    if (tm->HasFlag(M_INNATE)) {
      a = tm->Annot(tm->AnHead);
      /* Dragons each have a list of 12 spell-like abilities, but can only
         use the first N, where N is a number from 1 to 12 based on their
         age category. All other creatures can use their full allotment of
         spell-like abilities. */
      if (isMType(MA_DRAGON))
        ns = GetAgeCatagory();
      else
        ns = 1000;
      c = 0;
      while(a) {
        if (a->AnType == AN_INNATE)
          for(i=0;i!=8 && a->u.sp[i].sID;i++) {
            te = TEFF(a->u.sp[i].sID);
            ASSERT(te) ;
            AddEffect(te,a->u.sp[i].sID,(Item*)1);
            c++;
            if (c >= ns)
              break;
          }
        a = tm->Annot(a->Next);
        if (c >= ns)
          break;
      }
    }

    // templates can grant innate spells ...
    StatiIterNature(this,TEMPLATE)
        TTemplate *tt = TTEM(S->eID);
        a = tt->Annot(tt->AnHead);
        while(a) {
          if (a->AnType == AN_INNATE)
            for(i=0;i!=8 && a->u.sp[i].sID;i++) {
              te = TEFF(a->u.sp[i].sID);
              ASSERT(te) ;
              AddEffect(te,a->u.sp[i].sID,(Item*)1);
            }
          a = tt->Annot(a->Next);
        }
    StatiIterEnd(this)
    
    /* Polymorph shouldn't be adding /spells/...
    
    StatiIterNature(this,POLYMORPH)
        tm = TMON(mID); 
        a = tm->Annot(tm->AnHead);
        while(a) {
          if (a->AnType == AN_INNATE)
            for(i=0;i!=8 && a->u.sp[i].sID;i++) {
              te = TEFF(a->u.sp[i].sID);
              ASSERT(te) ;
              AddEffect(te,a->u.sp[i].sID,(Item*)1);
            }
          a = tm->Annot(a->Next);
        }
    StatiIterEnd(this)
    */
    
    if (hasInnateSpellStati) {
      StatiIterNature(this,INNATE_SPELL)
          AddEffect(TEFF(S->eID),S->eID,(Item*)1);
      StatiIterEnd(this)
    }

    if (!theRegistry->Exists(Inv))
      Inv = 0; 

    if (Inv) {
      it = oItem(Inv);
      do {
        /* Assumptions:
           -- Monsters don't use staves (yet) or wierd miscellaneous items.
           -- Monsters will not use an item the player has not yet identified.
         */

        if (!it->eID)
          continue;

        te = TEFF(it->eID);

        if (it->Type != T_POTION && 
            it->Type != T_SCROLL && 
            it->Type != T_WAND) {
          if (te->HasFlag(EF_ACTIVATE) == 0)
            continue;
        }

        if (it->Type == T_SCROLL && !HasSkill(SK_DECIPHER))
          continue;

        AddEffect(te,it->eID,it);
      }
      while (it = oItem(it->Next));
    }
}

void Monster::AddEffect(TEffect *te, rID eID, Item *src) {
    static int16 *p, pow, rat, i, weight;
    EffectValues *ef = &(te->ef);
    pow = ef->pval.LevelAdjust(ChallengeRating()).Roll();
    rat = 1000;
    
    if (denySpell(this,eID))
      return;
    
    /* Avoid casting the same buff spell three times in 
       a row, to no useful effect. */
    if (ef->eval == EA_INFLICT || ef->eval == EA_GRANT)
      if (HasEffStati(-1,eID))
        if ((te->Purpose & EP_BUFF) || (te->Purpose & AIM_AT_SELF))
          return;

    if (m) { 
      // initialize might be called before we're on a map!
      // which calls prebuff, which calls listeffects, which calls this

    if (ef->aval == AR_MFIELD)
      for(i=0;m->Fields[i];i++)
        if (m->Fields[i]->eID == eID)
          if (m->Fields[i]->Creator == myHandle)
            return;

    if (ef->aval == AR_FIELD)
      for(i=0;m->Fields[i];i++)
        if (m->Fields[i]->eID == eID)
          if (dist(x,y,m->Fields[i]->cx,m->Fields[i]->cy) <= m->Fields[i]->rad)
            return;

    }


    if (!src || (src == (Item*)1) || !(src->isType(T_POTION) || src->isType(T_SCROLL)))
      {
        if (te->ManaCost > cMana())
          return;

        i = (te->ManaCost*100)/max(1,cMana());
        rat -= (i*rat)/100;    
      }
    
    weight = te->HasFlag(EF_COMMON) ? 4 : 1;
    
    for (i=0;i!=weight;i++) {
      ASSERT(nEff <= sizeof(Effs) / sizeof(Effs[0]));
      Effs[nEff].eID = eID;
      Effs[nEff].Source = src;
      Effs[nEff].Rating = rat;
      Effs[nEff].Purpose = te->Purpose;
      // #ifdef DEBUG
      // Effs[nEff].EffectName = NAME(eID);
      // #endif
      nEff++;
      }
    
    
    
    hasEffFor |= (te->Purpose & 0xFFFF);
  }

/*
 * WW: If you walk down the stairs and come into a room containing an enemy
 * mage, you shouldn't get a chance to stab it while it's trying to cast
 * Mage Armour -- it should have done that long ago while it was standing
 * around, since the duration for that spell is huge. Similarly, if you do
 * some sort of Planar Gate and summon a spellcaster in from the middle of
 * nowhere, it probably wasn't sitting around naked before. PC mages always
 * have a bunch of buff spells up -- why not the bad guys? 
 *
 * This function gives a monster a bunch of buffs (that it could actually
 * cast normally or innately) for free in zero time, and should be called
 * from Initialize() and Rest(). 
 */
void Monster::PreBuff()
{
  int32 i; 
  if (isIllusion())
    if (!(getIllusionFlags() & IL_ADVANCED))
      return;
  ListEffects(); 
  for (i=0;i<nEff;i++) {
    rID eID = Effs[i].eID; 
    TEffect * te = TEFF(eID); 
    /*
    IDPrint(NULL,
        Format("%s considers %s.",
          (const char*)Name(NA_THE), (const char*)NAME(eID)));
          */
    if ( (Effs[i].Purpose & EP_BUFF) && 
        !(Effs[i].Purpose & EP_PLAYER_ONLY) &&
         ((long)Effs[i].Source <= 1) && 
        (te->HasFlag(EF_DXLONG) || te->HasFlag(EF_PERSISTANT) ||
          ( te->HasFlag(EF_DLONG) && random(100) < 25)) &&
        (te->PEvent(EV_ISTARGET,this,eID) == SHOULD_CAST_IT) &&
        (!HasEffStati(-1,eID)) ) {
      if (te->HasFlag(EF_DOMAIN) && !te->HasSource(AI_DRUIDIC) &&
          !te->HasSource(AI_WIZARDRY))
        // domain spells are rare!
        if (random(10) < 8) continue; 
      // we have a winner: let's buff with this! 
      EventInfo e;
      e.Clear();
      e.eID = eID; 
      e.EActor = this; 
      e.EVictim = this; 
      e.EMap = this->m;
      if ((long)Effs[nEff].Source == 1)       e.Event = EV_INVOKE;
      else                                    e.Event = EV_CAST;
      Magic::MagicEvent(e); 
    } 
  }
} 

uint32 Monster::SetMetamagic(rID eID, Thing *tar, uint16 Pur)
  {
    uint32 MM; TEffect *te = TEFF(eID);
    // ww: Fri Jan 16 20:19:03 PST 2004
    // I got a division by 0 on the next line. 
    int16 pMana = (cMana()*100)/ max(tMana(),1);

    /* Right now, this can lead to monsters with lots of different
       MM feats using many at once, wasting mana and casting spells
       with a 15%-30% success chance. This isn't such a big issue
       for monsters of CR <= 7, but it should be improved (to select
       the best combo of Feats leaving success chance > 70%) when we
       start bringing archmages and demon lords into play. */
    MM = 0;

    if (m->FieldAt(x,y,FI_SILENCE))
      if (HasFeat(FT_VOCALIZE_SPELL))
        MM |= MM_VOCALIZE;
    if (HasStati(GRAPPLED) || HasStati(GRAPPLING) || HasStati(PRONE)
         || HasStati(GRABBED))
      if (HasFeat(FT_STILL_SPELL))
        MM |= MM_STILL;

    if (((mHP+Attr[A_THP])*2 < cHP) || !(random(4)))
      if (pMana > 50 && (cMana() > 30))
        if ((Pur & EP_ATTACK) || (Pur & EP_BUFF))
          {
            if (HasFeat(FT_MAXIMIZE_SPELL))
              if (te->ef.pval.Sides > 1)
                MM |= MM_MAXIMIZE;
            if (cMana() > 70 || !(MM & MM_MAXIMIZE))
              if (te->ef.pval.Roll())
                if (HasFeat(FT_EMPOWER_SPELL))
                  MM |= MM_EMPOWER;
          }
    
    if (cMana() > 20 && !random(3) && !MM)
      if (HasFeat(FT_TRANSMUTE_SPELL))
        if (te->ef.eval == EA_BLAST)
          if(te->ef.xval == AD_FIRE || te->ef.xval == AD_COLD ||
              te->ef.xval == AD_ACID || te->ef.xval == AD_ELEC ||
              te->ef.xval == AD_SONI || te->ef.xval == AD_TOXI) 
            MM |= MM_TRANSMUTE;

    if ((mHP+Attr[A_THP])*3 < cHP || cMana() > 70)
      if (inMelee || !random(2))
        if (HasFeat(FT_QUICKEN_SPELL))
          MM |= MM_QUICKEN;      

    if (cMana() > 15 && inMelee)
      if (HasFeat(FT_DEFENSIVE_SPELL))
        MM |= MM_DEFENSIVE;
           
    return MM;       
  }

EvReturn Monster::Inventory()
  {
    int16 i; Item *it, *it2; EvReturn res;
    /* The Archery Slot Hack:
     *   Per this algorithm, monsters must be able to find the one, single
     * 'best' inventory configuration for any given set of items -- they
     * don't adjust their inventory based on outside circumstances at all.
     * For just about everything, this works outstandingly. However, the
     * deviant case is bows: they go in the weapon slot along with melee
     * weapons, yet the monster should be able to choose both an ideal
     * melee and an ideal ranged weapon. The solution? Give monsters an
     * "archery slot", allowing them to wield a single bow and a single
     * melee weapon at the same time. Mark which was used last in StateFlags,
     * and then add the appropriate amount of timeout for switching weapons
     * on to an attack in Monster::Event before EV_RATTACK, EV_NATTACK or
     * EV_WATTACK.
     *   There's still some glitches -- monsters can wield a cursed sword
     * and still be able to use a bow for ranged attacks -- so catch those
     * in the above attack functions. I'm sure there's some other problems
     * and inconsistancies with this that I haven't seen yet, but despite
     * this it seems like the best way to solve this problem.
     */
    
    // ww: we forgot a ring here ...
    // ww: and we forgot the shield/dual weapon!
    int8 HotSlots[] = { SL_WEAPON, SL_READY, SL_ARMOUR, SL_ARCHERY, SL_LRING, 
      SL_AMULET, SL_BRACERS, SL_GAUNTLETS, SL_BOOTS, SL_HELM, SL_BELT,
      SL_CLOAK, SL_LIGHT, 0 };
      int16 counter =0;
    /* For now, we only wield one ring, and only one weapon. This
       will change in the near future. */

    if (!Inv)
      {
        /* If we have no gear, our inventory arrangement is
           as good as it can possibly be! :) */
        StateFlags |= MS_INVEN_GOOD;
        return ABORT;
      }

    for(i=0;HotSlots[i];i++) {
      if (!TMON(mID)->HasSlot(HotSlots[i]))
        continue;
        
      /* Arcane casters should NOT equip armour! */
      if (HotSlots[i] == SL_ARMOUR)
        if (HasMFlag(M_MAGE))
          continue;
      
      it2 = InSlot(HotSlots[i]);
      if (it2) {
        if (it2->isCursed()) {
          StateFlags |= MS_CURSED;
          continue;
          }
        it = oItem(Inv);
        while (it) {
          if (counter++ >= 10000) {
            if (!thousandItemsError) {
              Error("Monster has >10000 items -- Inv loop?"); 
              thousandItemsError = true;
              }
            Inv = 0; 
            return DONE; 
            } 
          if (!(it->IFlags & IF_WORN)) {
            bool okSlot = false;
            switch (HotSlots[i]) {
              case SL_WEAPON: okSlot = it->isType(T_WEAPON); break;
              case SL_READY: okSlot = it->isType(T_SHIELD) &&
                                      (it->Size() <= GetAttr(A_SIZ)); break;
              case SL_ARCHERY: okSlot = it->isType(T_BOW); break;
              default: okSlot = it->activeSlot(HotSlots[i]); break;
              } 
            if (okSlot)
              if (it->betterFor(this,it2)) {
                res = Throw(EV_TAKEOFF,this,NULL,it2);
                if (res != DONE)
                  Error("Monster can't take off uncursed item!");
                return DONE;
              }
          }
          it = oItem(it->Next);
        }
      }
      else {
        it2 = NULL;
        it  = oItem(Inv);
        /* Find the *best* item for a given slot */
        while (it) {
          if (counter++ >= 10000) {
            if (!thousandItemsError) {
              Error("Monster has >10000 items -- Inv loop?"); 
              thousandItemsError = true;
              }
            return DONE; 
            }
          if (HotSlots[i] == SL_WEAPON || HotSlots[i] == SL_ARCHERY)
            if (it->Size(this) > GetAttr(A_SIZ)+1)
              goto NextItem;
          if (it->HasStati(TRIED,EV_WIELD+400,this))
            goto NextItem;
          if (!(it->IFlags & IF_WORN)) {
            bool okSlot = false;
            switch (HotSlots[i]) {
              case SL_WEAPON: okSlot = it->isType(T_WEAPON); break;
              case SL_READY: okSlot = it->isType(T_SHIELD) &&
                                      (it->Size() <= GetAttr(A_SIZ)); break;
              case SL_ARCHERY: okSlot = it->isType(T_BOW); break;
              default: okSlot = it->activeSlot(HotSlots[i]); break;
            } 
            if (okSlot)
              if (it != it2)
                if (it2 ? it->betterFor(this,it2) : true)
                  it2 = it;
          }
          NextItem:
          it = oItem(it->Next);
        }
        if (it2 && !(it2->IFlags & IF_WORN)) {
          /* Wear it */
          res = ThrowVal(EV_WIELD,HotSlots[i],this,NULL,it2);
          if (res != DONE)
            it2->GainPermStati(TRIED,this,SS_MISC,EV_WIELD+400);
          return DONE;
          }
        }
      }
    
    /* If we make it to the end of the for loop, that means that
       every scanned slot has the best possible item in it that
       it possibly can. Thus, we set the inventory-good flag and
       return ABORT to signify that we need to choose a different
       action other than exchanging equipment; */

    StateFlags |= MS_INVEN_GOOD;
    return ABORT;

  }

bool Item::betterFor(Creature *c, Item *curr)
  {
    if (PItemLevel(c) > curr->PItemLevel(c))
      return true;
    return false;
  }

bool Weapon::betterFor(Creature *c, Item *curr)
  {
    int16 rat, crat;
    if (!((curr->isType(T_WEAPON)) || (curr->isType(T_BOW)) ||
           curr->isType(T_MISSILE)))
      return Item::betterFor(c,curr);
    TItem *ti = TITEM(iID), *ti2 = TITEM(curr->iID);
    
    if (Size(c) > c->GetAttr(A_SIZ)+1)
      return false;
    /* The formula for rating a weapon:
       maximum damage + 5% maximum damage per point of
         Accuracy, +/- 1% maximum damage per point of
         Speed, plus 3 * item's level. */
    
    rat = ti->u.w.SDmg.Sides * ti->u.w.SDmg.Number + ti->u.w.SDmg.Bonus;
    rat = ((rat*100) + (ti->u.w.Acc + ti->u.w.Spd)*5) / 100;
    rat += PItemLevel(c) * 3;

    crat = ti2->u.w.SDmg.Sides * ti2->u.w.SDmg.Number + ti2->u.w.SDmg.Bonus;
    crat = ((crat*100) + (ti2->u.w.Acc + ti2->u.w.Spd)*5) / 100;
    crat += curr->PItemLevel(c) * 3;

    return rat > crat;
  }

bool Armour::betterFor(Creature *c, Item *curr)
  {
    int16 rat, crat;
    if (!curr->isType(T_ARMOUR) && !curr->isType(T_SHIELD))
      return Item::betterFor(c,curr);
    TItem *ti = TITEM(iID), *ti2 = TITEM(curr->iID);
    
    rat = ti->u.a.Arm[0] + ti->u.a.Arm[1] + ti->u.a.Arm[2] + 
        + ti->u.a.Cov + ti->u.a.Def + PItemLevel(c)*2;
    crat = ti2->u.a.Arm[0] + ti2->u.a.Arm[1] + ti2->u.a.Arm[2] + 
        + ti2->u.a.Cov + ti2->u.a.Def + curr->PItemLevel(c)*2;

    return rat > crat;
  }

EvReturn Monster::Event(EventInfo &e) {
    Thing *t;
    switch(e.Event) {
      case POST(EV_MAGIC_HIT):
          // ww: this check is of critical importance! otherwise spellcasters
          // always end up getting killed by their friends!
          if (e.eID && TEFF(e.eID)->Purpose & (EP_BUFF|EP_FIX_TROUBLE))
              return NOTHING;
          if (e.eID && TEFF(e.eID)->HasFlag(EF_NOTBAD))
              return NOTHING;

          if (isFriendlyTo(e.EActor)) {
              if (e.isSharedSpell)
                  return NOTHING;
              if (!(e.eID && TEFF(e.eID)->Purpose & (EP_ATTACK|EP_CURSE)))
                  return NOTHING;
          }

      case POST(EV_HIT):
      case EV_MISS:
      case POST(EV_DAMAGE):
          if (!e.EActor || e.EVictim != this || e.EActor == e.EVictim || e.EVictim->isDead())
              return NOTHING;
          else if (StateFlags & MS_PEACEFUL && e.Event == EV_MAGIC_HIT)
                  return NOTHING;

          if (e.EActor != e.EVictim && !e.isTrap && !e.isActOfGod)
              if (!e.eID || (!e.EVictim->HasEffStati(CHARMED,e.eID) && !TEFF(e.eID)->HasFlag(EF_NOTBAD)))
                  if (!(isFriendlyTo(e.EActor) && e.eID && TEFF(e.eID)->ef.aval == AR_FIELD))
                      ts.ItHitMe(e.EVictim,e.EActor,e.vDmg+1);
          if (e.EActor && !e.EActor->isDead() && !Perceives(e.EActor)) {
              if (e.AType == A_FIRE || e.AType == A_HURL || (e.EMagic && e.EMagic->eval == EA_BLAST))
                  if (e.Event == POST(EV_DAMAGE) || e.Event == EV_MISS)
                      IDPrint(NULL,Format("%s looks for an unseen foe.",(const char*)Name(NA_THE)));
              StateFlags &= ~MS_GUARDING; 
              ts.Wanderlust(this, e.EActor); 
          } 
          return NOTHING;
      case PRE(EV_NATTACK):
      case PRE(EV_WATTACK):
          // ww: without this check you couldn't attack a monster that was
          // wielding a cursed bow, 'cause they'd ABORT on your PRE(WATTACK).
          if (e.EActor != this) return NOTHING; 
          if (InSlot(SL_ARCHERY))
              if (InSlot(SL_ARCHERY)->isCursed())
                  return ABORT;
          if (StateFlags & MS_BOW_ACTIVE) {
              StateFlags &= ~MS_BOW_ACTIVE;
              if (HasFeat(FT_QUICK_DRAW))
                  Timeout += 4;
              else
                  Timeout += 16;
          }
          return NOTHING;
      case PRE(EV_RATTACK):
          // ww: without this check you couldn't attack a monster that was
          // wielding a cursed bow, 'cause they'd ABORT on your PRE(WATTACK).
          if (e.EActor != this) return NOTHING; 
          if (InSlot(SL_WEAPON))
              if (InSlot(SL_WEAPON)->isCursed())
                  return ABORT;
          if (!(StateFlags & MS_BOW_ACTIVE)) {
              StateFlags |= MS_BOW_ACTIVE;
              if (HasFeat(FT_QUICK_DRAW))
                  Timeout += 4;
              else
                  Timeout += 16;
          }
          return NOTHING;
      case PRE(EV_CAST):
      case PRE(EV_DRINK):
      case PRE(EV_ACTIVATE):
      case PRE(EV_READ):
      case PRE(EV_INVOKE):
      case PRE(EV_ZAP):
          if (e.EActor != this)
              return NOTHING;
          if (denySpell(this,e.eID))
              return ABORT;
          /* This code protects against extraneous spell use by monsters.
          It's simpler to put it here rather then in ChooseAction's
          already convoluted casting sequence. For example, it prevents
          an illithid from mind-blasting a target that's already stunned,
          or a novice mage from casting Bull's Strength twice in a row.
          */
          t = e.ETarget ? e.ETarget : this;
          if (t->HasEffStati(-1,e.eID))
              return ABORT;
          /* Technically, this is broken for multi-segment spells. In practice,
          it should catch every important case and not have any unexpected
          results. */
          if (TEFF(e.eID)->ef.eval == EA_INFLICT || TEFF(e.eID)->ef.eval == EA_GRANT) {
              if (t->HasStati(TEFF(e.eID)->ef.xval))
                  return ABORT;
          }
          break;
    }    
    return NOTHING;
}

int16 Monster::RateAsTarget(Thing *t)
  {
    int16 rat = 0, lev; Monster *mt;

    #ifdef LEAN
    if (!t->isPlayer())
      return 0;
    #endif

    /* Monsters ignore illusions that they've successfully
       disbelieved -- it's as if they don't exist. */
    if (t->isIllusion())
      if (!t->isRealTo(this))
        return 0;

    if (t->isItem()) {

      if (HasMFlag(M_NOHANDS) || HasMFlag(M_MINDLESS))
        return 0;
        
      if (t->HasStati(MAGIC_AURA))
        goto Shiny;

      if (t->isType(T_CORPSE) || t->isType(T_STATUE) ||
          HasStati(DO_NOT_PICKUP))
        return 0;


      
      if (t->isType(T_HELMET) || t->isType(T_CROWN) || t->isType(T_POTION) ||
            t->isType(T_AMULET))
        if (HasMFlag(M_NOHEAD))
          return 0;

      if (t->isType(T_SCROLL))
        if (!HasSkill(SK_DECIPHER))
          return 0;

      if (t->isType(T_ARMOUR))
        if (!HasMFlag(M_HUMANOID))
          return 0;

      if (HasMFlag(M_IGNORE_ITEMS))
        return 0;

      /* Monsters will irrationally chase items with Nystrom's 
         Magical Aura on them, but not others. */
      Shiny:
      lev = ((Item*)t)->PItemLevel(this) + 1;
      rat = (10 * min(lev,max(25,t->GetStatiMag(MAGIC_AURA)))) / max(1,ChallengeRating()/3);
      if (rat <= 0)
        return  0;

      if (HasMFlag(M_GREEDY) && t->isType(T_COIN))
        rat *= 3;
      else if (HasMFlag(M_GEMS) && t->isType(T_GEM))
        rat *= 3;
      else if (HasMFlag(M_COVETOUS))
        rat *= 2;
      
      rat = min(126,rat);
      
      ASSERT(rat >= 0 && rat <= 127) 
      return rat;
      }
    else if (t->isCreature()) {
      mt = (Monster*)t;
      
      // If you have two summoned creatures and one becomes mad at you, the
      // other won't protect you unless these conditions are &&'d
      if (PartyID == mt->PartyID) { //  && !isHostileTo((Creature*)t)) {
        return 0;
      }
      
      /* Mindless creatures just attack whatever is closest, regardless
         of what it is, and even ignore Conflict. */
      if (HasMFlag(M_MINDLESS))
        return 30;



      /* Peaceful monsters will only attack things that have attacked
         them first. Note that monsters don't lose their peaceful flag
         when attacked -- if the player attacks a guardsman, for example,
         then turns invisible, the guardsman won't then start appraising
         the other townsfolk as potential targets. */
      if (StateFlags & MS_PEACEFUL)
        rat = 0;

      /* Monsters that are members of these groups are not hostile to
         each other, and generally speaking will not attack each other. */
      int8 Solidarity[] = {
        MA_GOBLIN, MA_HUMAN, MA_ELF, MA_DWARF, MA_GNOME, MA_DEVIL,
        MA_ILLITHID, MA_FAERIE, MA_JELLY, 0 };

      for(int8 i=0;Solidarity[i];i++)
        if (isMType(Solidarity[i]) && mt->isMType(Solidarity[i]))
          rat = 0;

      /* isMType is an optimization to eliminate unnecessary 
         HasAbility calls */
      if (isMType(MA_REPTILE) && HasAbility(CA_ANCESTRAL_MEMORY))
        if (mt->HasAbility(CA_ANCESTRAL_MEMORY))
          return 0;


      /* The dead hate the living */
      if (isMType(MA_UNDEAD) && mt->isMType(MA_LIVING))
        rat += 20;

      /* Elves hate undead */
      if (isMType(MA_ELF) && mt->isMType(MA_UNDEAD))
        rat += 20;

      /* Elves and dwarves feud */
      if (isMType(MA_ELF) && mt->isMType(MA_DWARF))
        rat += 10;
      if (isMType(MA_DWARF) && mt->isMType(MA_ELF))
        rat += 10;

      /* Elves hate orcs */
      if (isMType(MA_ELF) && mt->isMType(MA_ORC))
        rat += 20;

      /* Races and their Darkling counterparts */
      if (isMType(MA_GNOME) && mt->isMType(MA_KOBOLD))
        rat += 20;
      if (isMType(MA_KOBOLD) && mt->isMType(MA_GNOME))
        rat += 20;
      if (isMType(MA_ELF) && mt->isMType(MA_DROW))
        rat += 20;
      if (isMType(MA_DROW) && mt->isMType(MA_ELF))
        rat += 20;    
      if (isMType(MA_HALFLING) && mt->isMType(MA_GOBLIN))
        rat += 20;
      if (isMType(MA_GOBLIN) && mt->isMType(MA_HALFLING))
        rat += 20;
      if (isMType(MA_DWARF) && mt->isMType(MA_ORC))
        rat += 20;
      if (isMType(MA_ORC) && mt->isMType(MA_DWARF))
        rat += 20;
    
      /* Dwarves are known as giant-killers */
      if (isMType(MA_DWARF) && mt->isMType(MA_GIANT))
        rat += 20;

      /* Cats and Dogs fight */
      if (isMType(MA_CAT) && mt->isMType(MA_DOG))
        rat += 20;
      if (isMType(MA_DOG) && mt->isMType(MA_CAT))
        rat += 20;

      /* Chromatic vs. Metallic */
      if (isMType(MA_DRAGON) && mt->isMType(MA_DRAGON)) {
        if (HasMFlag(MA_EVIL) && mt->HasMFlag(MA_GOOD))
          rat += 25;
        if (HasMFlag(MA_GOOD) && mt->HasMFlag(MA_EVIL))
          rat += 25;
        }

      /* Good creatures don't attack each other */
      if (isMType(MA_GOOD) && mt->isMType(MA_GOOD)) 
        return 0;

      /* Good creatures won't attack the adventurer, even if
         he's a drow or somesuch, until he proves himself to
         be hostile. We just assume that the good monster can
         distinguish a goblin raider from a goblic PC here,
         but it's not really unreasonable, given the distinct
         flavour D&D adventurers have. */
      if (isMType(MA_GOOD) && mt->isPlayer())
        return 0;

      if (mt->isMType(MA_SYLVAN) && HasAbility(CA_SYLVAN_AFFINITY))
        return 0;

      /* Otherwise, the player is public enemy #1. This is not
         entirely realistic, but it improves gameplay, and it
         does fit with the idea that in most dungeon scenarios
         the player is a foriegn intruder into a hostile complex,
         is doing things like having noisy battles to draw
         attention to himself, and generally has the whole world
         shouting for his head. */
      if (mt->isPlayer())
        rat += 25;

      rat += 5;
      // ww: favour close things
      rat += max(min(30 - dist(x,y,t->x,t->y),30),0);
      ASSERT(rat >= 0 && rat <= 127)
      return rat;
      }

    return 0;
  }

int8 Creature::BestHDType()
{
  TMonster * tm = TMON(mID);
  int8 b = max(max(MonHDType(tm->MType[0]), MonHDType(tm->MType[1])),
              MonHDType(tm->MType[2]));
  // ww: now check all templates! 
  StatiIterNature(this,TEMPLATE)
      int8 newType = TTEM(S->eID)->AddMType;
      if (newType) b = max(b, MonHDType(newType));
  StatiIterEnd(this)
  return (b % 100); 
} 

#if 0

/* Optimized WorstTrouble() for monsters taken out when we turned
     WorstTrouble() into getTrouble -- we might put these optimizations
     back later to reduce the number of hasStati calls, but with the
     new stati system that may not be as critical. */

int16* Creature::getTroubles()
  {
    static int16 Troubles[30];
    if (!mHP)
      return NULL;
    if (CurrAI != this)
      return Creature::WorstTrouble();
    int16 i, inj = ((mHP+Attr[A_THP]-cHP)*100)/(mHP+Attr[A_THP]),
         man = (cMana()*100L)/max(nhMana(),1);
    
    if (isStoning)
      Troubles[n++] = TROUBLE_STONING + P_CRITICAL*256;
    
    if (70 <= inj)
      Troubles[n++] = TROUBLE_INJURY + P_CRITICAL*256;
    if (hasHunger)
      if (HungerState() == WEAK || HungerState() == FAINTING)
        Troubles[n++] = TROUBLE_HUNGER + P_CRITICAL*256;

    /* Later, relative by danger. */
    if (isPoisoned)
      Troubles[n++] = TROUBLE_POISON + P_EXTREME*256;
          
    if (isBlind && !isNatBlind)
      Troubles[n++] = TROUBLE_BLIND + P_EXTREME*256;

    if (70 <= man)
      Troubles[n++] = TROUBLE_LOWMANA + P_EXTREME*256;

    if (50 <= inj)
      Troubles[n++] = TROUBLE_INJURY + P_URGENT*256;
    if (isTAfraid)
      Troubles[n++] = TROUBLE_FEAR + P_URGENT*256;

    if (isSick)
      Troubles[n++] = TROUBLE_SICK + P_HIGH*256;
    if (isBleeding)
      Troubles[n++] = TROUBLE_BLEEDING + P_HIGH*256;
    if (isDiseased)
      Troubles[n++] = TROUBLE_DISEASE + P_HIGH*256;

    if (isDrained) {
      StatiIterAdjust(this)
          if (S->Val != A_SIZ && S->Mag < 0) {
            if (S->Source == SS_ENCH)
              StatiIterBreakout(this,Troubles[n++] = TROUBLE_DRAINED + 
                ( S->Mag <= -4 ? P_HIGH : P_MODERATE) * 256)
            else if (S->Source == SS_CURS)
              StatiIterBreakout(this,Troubles[n++] = TROUBLE_CURSED + 
                ( S->Mag <= -4 ? P_HIGH : P_MODERATE) * 256)
            }
      StatiIterEnd(this)
    }

    if (man <= 50)
      Troubles[n++] = TROUBLE_LOWMANA + P_MODERATE*256;

    if (hasHunger)
      if (HungerState() == STARVING)
        Troubles[n++] = TROUBLE_HUNGER + P_MODERATE*256;
            
    if (inj <= 20)
      Troubles[n++] = TROUBLE_INJURY + P_MODERATE*256;

    if (hasHunger)
      if (HungerState() == HUNGRY)
        Troubles[n++] = TROUBLE_HUNGER + P_MODERATE*256;

    /* Only use Mana potions when at below half mana. */

    return 0;
  }
#endif
