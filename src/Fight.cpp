/* FIGHT.CPP -- Copyright 1999-2003 (c) Julian Mensch
     Massive file. All of the functions related to direct melee
   and missile combat are here. (Attack spells are handled in
   Magic.Cpp).
     
     EvReturn Creature::WAttack(EventInfo &e)
     EvReturn Creature::RAttack(EventInfo &e)
     EvReturn Creature::NAttack(EventInfo &e)
     EvReturn Creature::SAttack(EventInfo &e) 
     EvReturn Creature::OAttack(EventInfo &e) 
     void Creature::ProvokeAoO()
     void Creature::ProvokeAoO(Creature *c, bool from_move)
     void Creature::DoGazeAttack()
     void Creature::DoProxAttack()
     EvReturn Creature::PreStrike(EventInfo &e) 
     EvReturn Creature::Strike(EventInfo &e) 
     bool Creature::ManeuverCheck(EventInfo &e)
     EvReturn Creature::Hit(EventInfo &e)
     EvReturn Creature::Miss(EventInfo &e)
     EvReturn Creature::Damage(EventInfo &e)
     EvReturn Creature::BullRush(EventInfo &e)
     EvReturn Creature::Death(EventInfo &e)
     EvReturn Player::Death(EventInfo &e)
     EvReturn Weapon::QualityDmg(EventInfo &e)
     int16 Creature::WeaponSaveDC(Item *wp, int16 at)
     int8 Creature::AttackMode()
     Dice& Creature::DmgVal(int16 type, bool is_large)
     EvReturn Creature::AttackMsg(EventInfo &e)

*/

#include "Incursion.h"

/* Occasional messages:
   Name screams in agony
   Your Weapon crushes Name's skull!
   Blood sprays from's name's chest!
   You hear something crack within Name's body!
   Name's flesh melts with a hiss!
   You lunge center, [sliding past Name's defenses and ] hitting her.
   etc.
*/       
/*Example messages:



You lunge center, missing the kobold wildly!
You punch high, hitting the kobold;
The Kobold attacks low, clawing you badly!

Structure:
You verb1 location, Hit/Missing Name[;moreinfo]!
										-or-
You verb1 location, but Name blocks/parries/dodges!

verb1a: punch, kick, swing, lunge, smash, blast, annihilate
verb1b: punches, kicks, swings, lunges, smashes, blasts, annihilates
verb2a: hit, miss, blast, annihilate
moreinfo = your weapon melts/corrodes/vanishes, Name seems unhurt,
	the black blade draws the life from Name, name dies!


Attack Value (AV) [Brawl, Melee, Archery, Throwing]
	Monster AV -or- Class AV, whichever is higher
	+A_STR/Prowess Bonus (Brawl & Melee)
	+AGI/Aim Bonus     (Missle Weapons)
	-4 if blind
	-3 if confused
	-1 if hallucinating

Defense Value (DV) [Dodge, Block, Parry, Missle Deflection(monk)]
	Monster DV -or- Class DV, whichever is higher
	+AGI Bonus
	etc.

DoBlow:
	1)Determine if brawl/melee/missle
	2)Roll 1d20:
		1-4: May be botch, depending on skill.
		16-20: May be critical, depending on skill.
		Otherwise:
			if Roll + AV > Target's DV
				if  Target->Defend(Roll + AV)
					BLOCK/PARRY
				else
					HIT
			MISS




*/

/* Things to check:

   Incorporeality
   Tiny vs. Levitation
   Invisibility for -4.
   Light Averse + Call Light == -4 Attack.
   Poisoned Weapons
   Blessed Weapons vs Incorp.
   Damage Resistance (Always 10 + CR)
   Touch Spells
   Damage & Resistance
   Weapon Qualities
   Monster Special Attacks:
     Damage Stats
     Grant Stati (Stunned, Confused, etc.)
   Piercing vs. Skeletons
   Lots of Monster Flags!  
*/


String DebugAttk, DebugDef, DebugDmg;


bool isLegalPersonTo(Creature *Actor, Creature *Victim)
  {
    bool isLegalPerson;
    isLegalPerson = false;
    
    if (Victim->isMType(MA_EVIL) && Victim->HasMFlag(M_IALIGN)
         && !Actor->isMType(MA_DEVIL))
      return false;
    if (Victim->HasMFlag(M_MINDLESS))
      return false;
    
    if (Actor->isMType(MA_GOOD) || Actor->isMType(MA_REPTILE))
      isLegalPerson = Victim->isMType(MA_SAPIENT);
    else if (Actor->isMType(MA_GOBLINOID))
      isLegalPerson = Victim->isMType(MA_SAPIENT) &&
                      Victim->isMType(MA_GOBLINOID) &&
                      Victim->isMType(MA_NLIVING);
    else
      isLegalPerson = Victim->isMType(MA_SAPIENT) &&
                      (Victim->isMType(MA_DEMIHUMAN) ||
                       Victim->isMType(MA_CELESTIAL)) &&
                      Victim->isMType(MA_NLIVING);
    return isLegalPerson;
}

/* Determines both breach of chivalry and attacking fleeing foe */
int16 getChivalryBreach(EventInfo &e) {
    int16 i, breach;
    bool givenTerms, isLegalPerson, vicFleeing, vicUnready;

    breach = 0;

    if (!e.EVictim)
        return breach;
    if (!e.EVictim->isCreature())
        return breach;

    vicFleeing = e.EVictim->HasStati(AFRAID) && e.EVictim->GetStatiVal(AFRAID) != FEAR_SKIRMISH;
    for (i = 0; i != EventSP; i++)
        if (EventStack[i].vicNotFleeing && EventStack[i].EVictim == e.EVictim)
            vicFleeing = false;

    vicUnready = e.EVictim->HasStati(PRONE) || e.EVictim->HasStati(BLIND) || e.EVictim->HasStati(PARALYSIS) || e.EVictim->HasStati(STUCK);
    for (i = 0;i != EventSP; i++)
        if (EventStack[i].vicReady && EventStack[i].EVictim == e.EVictim)
            vicUnready = false;

    /* Inherently evil creatures are not subject to law or good. */
    if (e.EVictim->isMType(MA_EVIL) && e.EVictim->HasMFlag(M_IALIGN))
        return breach;

    isLegalPerson = isLegalPersonTo(e.EActor,e.EVictim);
    givenTerms = e.EVictim && 
        (e.EVictim->HasStati(TRIED, SK_DIPLOMACY+EV_TERMS*100,e.EActor) || e.EVictim->HasStati(TRIED, SK_INTIMIDATE+EV_COW*100,e.EActor));

    if (vicFleeing && !givenTerms && e.EVictim->isMType(MA_SAPIENT)) {
        breach |= AL_NONGOOD;
        if (isLegalPerson)
            breach |= AL_NONLAWFUL;
    }

    /* Only paladins, heavy armour wearers, mounted fighters and followers of Erich are subject to the code of chivalry... */
    if (e.EActor->HasAbility(CA_LAY_ON_HANDS) || e.EActor->HasStati(MOUNTED) ||
        !stricmp(NAME(e.EActor->getGod()),"Erich") || (e.EActor->InSlot(SL_ARMOUR) && e.EActor->InSlot(SL_ARMOUR)->isGroup(WG_HARMOUR))) {
        if (vicUnready)
            if (isLegalPerson && !e.EVictim->HasStati(HIT_WHILE_UNREADY, -1, e.EActor))
                breach |= AL_NONLAWFUL;
    }

    return breach;
}
  
bool attackSanity(EventInfo &e) {
    bool doAlignWarning; 
    int16 breach;
    Creature *l;
    
    breach = getChivalryBreach(e);
    doAlignWarning = false;
    if (e.EActor->isMType(MA_LAWFUL) || (e.EActor->isCharacter() && 
           e.EPActor->desiredAlign & AL_LAWFUL))
      if (breach & AL_NONLAWFUL)
        doAlignWarning = true;
    if (e.EActor->isMType(MA_GOOD) || (e.EActor->isCharacter() && 
           e.EPActor->desiredAlign & AL_GOOD))
      if (breach & AL_NONGOOD)
        doAlignWarning = true;
        
    if (doAlignWarning)
      if (!e.EActor->yn(Format("%s %s victim?",
                e.isAoO ? "Take opportunity on" : "Confirm attack",
                e.EVictim->HasStati(AFRAID) ? "fleeing" :
                e.EVictim->HasStati(PRONE) ? "prone" :
                e.EVictim->HasStati(BLIND) ? "blinded" :
                e.EVictim->HasStati(PARALYSIS) ? "paralyzed" : "stuck"),true))
        return false;        

    /* Creatures with a lawful or good leader may be under orders
       not to do things which contravene the leader's alignment. */
    l = e.EActor->getLeader();
    if (l && (l != e.EActor) && (l->isMType(MA_LAWFUL) || (l->isCharacter() && 
           ((Player*)l)->desiredAlign & AL_LAWFUL)))
      if (breach & AL_NONLAWFUL)
        return false;
    if (l && (l != e.EActor) && (l->isMType(MA_GOOD) || (l->isCharacter() && 
           ((Player*)l)->desiredAlign & AL_GOOD)))
      if (breach & AL_NONGOOD)
        return false;

    return true;
  }     

EvReturn Creature::WAttack(EventInfo &e)
{
    int8 CleaveCount,j; Creature *CleaveList[64], *c;
    bool isTWF = false; TAttack *at; String s;
    bool startedAfraid = HasStati(AFRAID);
    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
      e.EItem = NULL;
    
    if (!EInSlot(SL_WEAPON))
      return ABORT;
      
    if (!attackSanity(e))
      return ABORT;

    if (!e.ETarget)
      return ABORT;
    if (!e.ETarget->isCreature() && !e.ETarget->isFeature() &&
        !e.ETarget->isItem()) {
      e.EActor->IPrint("You can't attack that sort of thing.");
      return ABORT; 
    }
    
    if (HasStati(STUCK))
    {
      e.EActor->IPrint("You can't attack with a weapon while stuck.");
      return ABORT;
    }
    else if (HasStati(AFRAID) && !(e.EVictim && !e.EVictim->isCreature())
               && !HasFeat(FT_LION_HEART)) {
      e.EActor->IPrint("You're too scared to attack!");
      return ABORT;
      }
    else if (HasStati(CHARMED,-1,e.EVictim)) {
      IPrint("You have no desire to hurt your dear friend!");
      return ABORT;
      }
    else if (HasStati(NAUSEA)) {
      if (HasSkill(SK_CONCENT) && SkillLevel(SK_CONCENT) >= 10)
        if (yn("You are Nauseated. Use Concentration to attack?"))
          {
            if (SkillCheck(SK_CONCENT,20,true))
              {
                IPrint("You overcome your nausea to attack!");
                goto OvercomeNausea;
              }
            else
              {
                IPrint("You try to attack, but only retch and vomit!");
                ProvokeAoO();
                Timeout += 40;
                return DONE;
              }
          }
      e.EActor->IPrint("You're too nauseated to attack!");
      return ABORT;
      }
    else if (ts.hasTargetOfType(OrderDoNotAttack)) {
      return ABORT; 
    } 
    OvercomeNausea:
    // ww: I'm tired of getting raked through walls ...
    if (!(e.ETarget->isItem() && !e.ETarget->m))
      if (!m->InBounds(x,y) ||
          (e.EActor->onPlane() == PHASE_MATERIAL &&
             !(m->LineOfFire(x,y,e.ETarget->x,e.ETarget->y,this)))) {
        IPrint("You do not have a clear line of attack!"); 
        return ABORT; 
        } 
    if (EInSlot(SL_WEAPON)->HasIFlag(WT_NO_MELEE)) {

        s = Format("%s is cannot be used as a melee weapon! Brawl?",
              (const char*)EInSlot(SL_WEAPON)->Name(NA_THE));
        if (yn(s,true)) 
          return ReThrow(EV_NATTACK,e);
        else 
          return ABORT; 
    } 

    if (HasStati(ILLUSION)) {
      if (!e.ETarget->isCreature())
        return ABORT;
      e.actIllusion = true;
      }
    if (e.EVictim->HasStati(ILLUSION))
      e.vicIllusion = true;

    if (HasStati(GRABBED) || HasStati(GRAPPLED) || 
          HasStati(GRAPPLING) || HasStati(ENGULFED)) {
      /* ww: from the SRD: when you are grabbed or grappling, you may
       * only use natural attacks or a "light" weapon. For humans
       * this includes knives and whatnot which are SZ_TINY. */
      if (EInSlot(SL_WEAPON)->isType(T_WEAPON) && 
          !((Weapon *)EInSlot(SL_WEAPON))->useInGrapple(e.EActor)) {
        s = Format("Your weapon is too large to use while %s! Brawl?",
              HasStati(ENGULFED) ? ( GetStatiVal(ENGULFED) == EG_ENGULF ?
                "engulfed" : "swallowed" ) : "grappled");
        if (yn(s,true)) 
          return ReThrow(EV_NATTACK,e);
        else 
          return ABORT; 
      } 
    } 

    if (EInSlot(SL_WEAPON)->HasIFlag(WT_MOUNTED_ONLY)) {
      if (!HasStati(MOUNTED) && Attr[A_SIZ] <= EInSlot(SL_WEAPON)->Size(this)) {
        IPrint(Format("You are too small to use %s unless you are mounted.",
              (const char*) EInSlot(SL_WEAPON)->Name(NA_THE)));
        return ABORT; 
      } 
    } 

    /* ww: are stuck in no-attack webbing or somesuch? */
    if (!m->InBounds(x,y) || 
        TTER(m->TerrainAt(x,y))->Event(e,m->TerrainAt(x,y)) == ABORT)
      return ABORT; 

    /* Paranoia */
    if (isBeside(e.ETarget)) {
      if (e.EActor->StateFlags & MS_REACH_ONLY && e.ETarget->isCreature()) 
          {
            IPrint("You can't strike something that close with a reach weapon.");
            return ABORT;
          }
    } else if (!isBeside(e.ETarget,1)) {
      IPrint("You can't strike something *that* far away.");
      return ABORT;
    } else if (!(e.EActor->StateFlags & MS_HAS_REACH)) {
      IPrint("You can't strike something that far away.");
      return ABORT;
    }

    if (e.ETarget->isCreature() && 
        e.EActor->isPlayer() && 
        e.EVictim->HasAttk(A_DEQU) &&
        e.EPActor->Opt(OPT_WARN_DEQU)) {
      TAttack *ta = e.EVictim->GetAttk(A_DEQU);
      int max = ta->u.a.Dmg.Number * ta->u.a.Dmg.Sides + ta->u.a.Dmg.Bonus;
      //if (e.EPActor->HasFeat(FT_SIGNATURE_ITEMS)) max /= 2; 
      int hard = EInSlot(SL_WEAPON) ? 
        EInSlot(SL_WEAPON)->Hardness(ta->DType) : 
        e.EActor->ResistLevel(ta->DType);
      if (ta->DType == AD_SHAT && e.EActor->ResistLevel(AD_SHAT) == -1) hard=-1;
      if (ta->DType == AD_RUST && e.EActor->ResistLevel(AD_RUST) == -1) hard=-1;
      if (ta->DType == AD_SOAK && e.EActor->ResistLevel(AD_SOAK) == -1) hard=-1;
      if (hard == -1 || hard >= max)
        ;
      else {
        s = Format("Attack %s (%s %s)?",
            (const char*)e.EVictim->Name(NA_THE),
            (const char*)ta->u.a.Dmg.Str(),
            Lookup(DTypeNames,ta->DType));
        if (!yn(s,true)) 
          return ABORT; 
      } 
    } 

    if (e.EVictim && e.EVictim->isCreature())
      if (!e.EVictim->HasStati(AFRAID) ||
          e.EVictim->GetStatiVal(AFRAID) == FEAR_SKIRMISH ||
          e.EVictim->HasFeat(FT_LION_HEART))
        e.vicNotFleeing = true; 
    if (e.EVictim && e.EVictim->isCreature())
      if (!(e.EVictim->HasStati(PRONE) ||
            e.EVictim->HasStati(BLIND) ||
            e.EVictim->HasStati(PARALYSIS) ||
            e.EVictim->HasStati(STUCK)))
        e.vicReady = true;

    if (e.ETarget->isCreature() && 
        (e.EActor->isPlayer() && ((Player *)e.EActor)->Opt(OPT_AUTO_COUP)) &&
        (e.ETarget->HasStati(SLEEPING) || 
        (e.EVictim->HasStati(STUCK) && e.EVictim->HasStati(PRONE) &&
            e.EActor->SkillLevel(SK_FIND_WEAKNESS) >= 20) ||
        e.EVictim->HasStati(PARALYSIS,PARA_HELD)) &&
        (!e.EActor->HasStati(CONFUSED)) &&
        (e.EVictim->isMType(MA_LIVING) ||
         (e.EVictim->isMType(MA_UNDEAD) && e.EActor->HasFeat(FT_NECROPHYSIOLOGY)))) {
      // coup-de-grace instead!
      return ThrowVal(EV_SATTACK,A_COUP,e.EActor,e.EVictim);
    } 

    CleaveCount = 0;

DoCleave:
    if (e.ETarget->HasStati(GRABBED,0,e.EActor))
      if (e.EActor->HasStati(GRAPPLING))
        e.vicHeld = true;

    e.EItem   = EInSlot(SL_WEAPON);
    e.Dmg     = e.EActor->DmgVal(S_MELEE, 
      e.ETarget->isCreature() && (e.EVictim->GetAttr(A_SIZ) > SZ_MEDIUM));
    ASSERT(e.Dmg.Sides >= 0); 
    ASSERT(e.Dmg.Number >= 0); 
    e.vHit    = (int8)Attr[A_HIT_MELEE];
    e.vDef    = e.ETarget->isCreature() ? e.EVictim->getDef() : 0;
    // Mighty Cleaving weapons give you free criticals (A_COUP here doesn't
    // have all of the "if they live, make another save" effects) for
    // cleave attacks. 
    if (e.isCleave && e.EItem && e.EItem->HasQuality(WQ_CLEAVING)) {
      e.AType = A_COUP;
      e.isCrit = true; 
    } else 
      e.AType = A_SWNG;
    e.DType   = (int8)e.EItem->DamageType(e.EVictim); 
    e.vThreat = (int8)e.EItem->Threat(e.EActor);
    e.vCrit   = (int8)e.EItem->CritMult(e.EActor);

    bool wasProneBefore = e.ETarget->HasStati(PRONE);
    if (ReThrow(EV_STRIKE, e) == ABORT)
      return ABORT;
    bool isProneNow = e.ETarget->HasStati(PRONE);

    if (AttackMode() == S_DUAL && !(e.ETarget->isDead()))
        {
          e.isOffhand = true; isTWF = true;
          e.EItem   = EInSlot(SL_READY);
          e.Dmg     = e.EActor->DmgVal(S_DUAL, e.ETarget->isCreature() && 
              (e.EVictim->GetAttr(A_SIZ) > SZ_MEDIUM));
          e.vHit    = (int8)Attr[A_HIT_OFFHAND];
          e.vDef    = e.ETarget->isCreature() ? e.EVictim->getDef() : 0;
          e.AType   = A_SWNG;
          e.DType   = (int8)e.EItem->DamageType(e.EVictim); 
          e.vThreat = (int8)e.EItem->Threat(e.EActor);
          e.vCrit   = (int8)e.EItem->CritMult(e.EActor);
          if (ReThrow(EV_STRIKE, e) == ABORT)
            return ABORT;
          e.isOffhand = false;
        }

    bool downedVictim = e.Died || (isProneNow && !wasProneBefore);

    // ww: cleave only through creatures
    if (!e.ETarget->isCreature()) downedVictim = false; 

    if (downedVictim && !isDead() &&
        ( (e.EActor->HasFeat(FT_GREAT_CLEAVE)) ||
          (e.EActor->HasFeat(FT_CLEAVE) && !CleaveCount))) {
      e.isCleave = true;
      e.Died     = false;
      e.isHit    = false;
      CleaveList[CleaveCount++] = e.EVictim;
      // ww: this is supposed to be your "threatened area" (from the
      // textual description), so if you cleave with a halberd you should
      // be able to hit someone two squares away and *not* someone
      // adjacent. While we're here, let's add some bounds checks,
      // sight checks and alliance checks. 
      for (int dx = -2; dx <= 2; dx ++)
        for (int dy = -2; dy <= 2; dy ++) {
          int xx = e.EActor->x + dx;
          int yy = e.EActor->y + dy;
          if (!m->InBounds(xx,yy))
            continue;
          for (c=m->FCreatureAt(xx,yy);c;c=m->NCreatureAt(xx,yy)) {
            if (c == e.EActor) continue; 
            if (!e.EActor->Perceives(c)) continue;
            if (!e.EActor->isHostileTo(c)) continue;
            if (!m->LineOfFire(e.EActor->x,e.EActor->y,xx,yy,e.EActor) &&
                e.EActor->onPlane() == PHASE_MATERIAL) continue; 
            // check for too close
            if (EInSlot(SL_WEAPON) &&
                EInSlot(SL_WEAPON)->HasIFlag(WT_REACH) &&
                !EInSlot(SL_WEAPON)->HasIFlag(WT_STRIKE_NEAR) &&
                isBeside(c))
              continue; 
            // check for too far
            if (!isBeside(c) &&
                !(e.EActor->StateFlags & MS_HAS_REACH))
              continue; 
            for(j=0;j!=CleaveCount;j++)
              if (CleaveList[j] == c)
                goto SkipThisTarget;
            {
              int breach;
              EventInfo xe;
              xe = e;
              xe.ETarget = c;
              breach = getChivalryBreach(xe);
              if (((breach & AL_NONGOOD) && e.EActor->isMType(MA_GOOD)) ||
                  ((breach & AL_NONLAWFUL) && e.EActor->isMType(MA_LAWFUL)))
                if (!yn(XPrint("Unchivalrously cleave <Obj>?",c)))
                  continue;
            }
            
            
            e.EVictim = c;
            goto DoCleave;
          }
SkipThisTarget:;
        } 
    }

    if (e.EActor->HasMFlag(M_XTRA_TOUCH) && !isDead())
      if (at = e.EActor->GetAttk(A_TUCH))
        {
          
          e.EItem     = NULL;
          e.EItem2    = NULL;
          e.vHit      = (int8)Attr[A_HIT_BRAWL];
          e.vDef    = e.ETarget->isCreature() ? e.EVictim->getDef() : 0;
          e.Dmg       = at->u.a.Dmg;
          e.Dmg.Bonus += e.EActor->Attr[A_DMG_BRAWL]; 
          e.saveDC    = (int8)e.EActor->GetPower(at->u.a.DC);
          e.AType     = at->AType;
          e.DType     = at->DType;
          e.vThreat   = 20;
          e.vCrit     = 2;
          ReThrow(EV_STRIKE,e);
        }

    if (e.EActor->HasFeat(FT_EXPERT_TACTICIAN) && !isDead() &&
        (e.isSurprise || e.EVictim->noDexDefense())
        && !e.EActor->HasStati(TRIED,FT_EXPERT_TACTICIAN,e.EVictim)) { 
        e.EActor->GainTempStati(TRIED,e.EVictim,1,SS_MISC,FT_EXPERT_TACTICIAN);
      // attack takes no time
    } else  if (isTWF) {
      Timeout += 
          3000 / max((100 + min(Attr[A_SPD_MELEE],Attr[A_SPD_OFFHAND])*5),10) +
          3000 / max((100 + max(Attr[A_SPD_MELEE],Attr[A_SPD_OFFHAND])*5),10) / 2;
      }
    else
      Timeout += 3000 / 
          max((100 + Attr[A_SPD_MELEE]*5),10);
    if (startedAfraid)
      Timeout *= 2;
    

    return DONE;
}

EvReturn Creature::RAttack(EventInfo &e)
{
  int8 oldSlot;
  int8 i;
  bool startedAfraid = HasStati(AFRAID);
  if (!e.EItem2)
    return ABORT;

    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
      e.EItem = NULL;
    

  if (!attackSanity(e))
    return ABORT;


  oldSlot = -1;
  for (i=1;i!=SL_LAST;i++)
    if (e.EActor->InSlot(i) == e.EItem2)
      { oldSlot = i; break; }
  
  /* TODO: Some of this needs to be moved into PreRAttack, in order
     to deal with stuff that traps RAttack, like the Javelins of
     Lightning. */

  if (HasStati(AFRAID) && !HasFeat(FT_LION_HEART)) {
    e.EActor->IPrint("You're too scared to make a ranged attack!");
    return ABORT;
  }
  if (HasStati(STUCK)) {
    e.EActor->IPrint("You can't attack with a ranged weapon while stuck.");
    return ABORT;
  }
  if (e.EVictim && HasStati(CHARMED,-1,e.EVictim)) {
    IPrint("You have no desire to hurt your dear friend!");
    return ABORT;
  } else if (ts.hasTargetOfType(OrderDoNotAttack)) {
    return ABORT; 
  } 


  /* ww: are stuck in no-attack webbing or somesuch? */
  if (!m->InBounds(x,y) || 
      TTER(m->TerrainAt(x,y))->Event(e,m->TerrainAt(x,y)) == ABORT)
    return ABORT; 

  if (HasStati(ILLUSION)) {
    if (!e.ETarget->isCreature())
      return ABORT;
    if (!(getIllusionFlags() & IL_ADVANCED))
      return ABORT;
    e.actIllusion = true;
    }
  if (e.EVictim && e.EVictim->HasStati(ILLUSION))
    e.vicIllusion = true;

  if (e.EItem && e.EItem->needsToBeCocked()) {
    if (e.EActor->isPlayer() || 
        e.EActor->yn("Your crossbow isn't loaded. Load it?",true)) 
      Throw(EV_LOADXBOW,e.EActor,NULL,e.EItem);
  } 

  /* You *CAN* fire a crossbow while swallowed or engulfed. */
  if (e.EActor->HasStati(GRAPPLED) || e.EActor->HasStati(GRABBED)
      || (e.EActor->HasStati(ENGULFED) && (!e.EItem || !e.EItem->HasIFlag(WT_CROSSBOW)))) {
    e.EActor->IPrint("You can't <Str> while you're <Str>!",
        e.EItem ? "shoot" : "throw things",
        HasStati(ENGULFED) ? ( GetStatiVal(ENGULFED) == EG_ENGULF ?
                "engulfed" : "swallowed" ) : "being grappled");
    return ABORT;
  }

  if (e.EActor->HasStati(GRAPPLING)) {
    e.EActor->IPrint("You can't <Str> while you're grappling!",
        e.EItem ? "shoot" : "throw things");
    return ABORT;
  }

    if (e.EVictim && e.EVictim->isCreature())
      if (!e.EVictim->HasStati(AFRAID) || e.EVictim->HasFeat(FT_LION_HEART) ||
          e.EVictim->GetStatiVal(AFRAID) == FEAR_SKIRMISH)
        e.vicNotFleeing = true; 
    if (e.EVictim && e.EVictim->isCreature())
      if (!(e.EVictim->HasStati(PRONE) ||
            e.EVictim->HasStati(BLIND) ||
            e.EVictim->HasStati(PARALYSIS) ||
            e.EVictim->HasStati(STUCK)))
        e.vicReady = true;

  if (!e.EActor->HasFeat(FT_DEFENSIVE_SHOT)) {
    e.EActor->ProvokeAoO();
    if (e.EActor->isDead())
      return DONE; 
  }
  if (e.EActor->isDead())
    return ABORT;

  Creature *Intended; 
  if (e.isLoc) {
    Intended = NULL; bool no_per = false; int16 c = 0; Creature *cr;
    /* If you shoot at a spot with multiple creatures, you're shooting at
       the largest creature you perceive in that spot. But you might be
       shooting at a spot with no perceived creatures, because you suspect
       that there's an invisible creature there, in which case your intended
       target is one of the largest invisible creatures, hostile or otherwise. */
    RetryIntendedSearch:
    for (cr=m->FCreatureAt(e.EXVal,e.EYVal);cr;cr=m->NCreatureAt(e.EXVal,e.EYVal))
      if (no_per || e.EActor->Perceives(cr))
        if (no_per || cr->isHostileTo(e.EActor))
          {
            /* Smaller creatures aren't candidates. */
            if (c && oCreature(Candidates[c-1])->GetAttr(A_SIZ) > cr->GetAttr(A_SIZ))
              continue;
            /* Larger creatures overwrite all prior candidates. */
            if (c && oCreature(Candidates[c-1])->GetAttr(A_SIZ) < cr->GetAttr(A_SIZ))
              c = 0;
            Candidates[c++] = cr->myHandle;
          }
    if (!c && !no_per)
      {
        no_per = true;
        c = 0;
        goto RetryIntendedSearch;
      }
    Intended = c ? oCreature(Candidates[random(c)]) : NULL;
    }
  else
    Intended = e.EVictim;
  if (Intended == e.EActor) {
    IPrint("There is no reason to shoot yourself."); 
    return ABORT; 
    } 

  e.vRange = max(1,(e.EItem ? e.EItem : e.EItem2)->RangeInc(e.EActor));

  if (e.EActor->HasFeat(FT_FAR_SHOT))
    e.vRange *= 2;

  if (e.EItem2->GetQuantity() > 1) {
    e.remainingAmmo = e.EItem2;
    e.EItem2 = e.EItem2->TakeOne();
    }

  /* ww: massive change here: with the old "flight" logic arrows
   * basically only hit opponents along cardinal directions:
   * @
   *      X
   * You would always miss the X in such a scenario because the arrow
   * would never actually reach its square. Now we steal the BallBeamBolt
   * trajectory code. */ 

  // where do we start? 
  int16 sx, sy, cx, cy, lx, ly;
  lx = sx = cx = e.EActor->x * 2;
  ly = sy = cy = e.EActor->y * 2;

  // glyph (colour set below) 
  Glyph g = GLYPH_ID(WHITE);
  Glyph gcolour = TITEM(e.EItem2->iID)->Image & GLYPH_COLOUR_MASK;

  int8 dirX = 0, dirY = 0;
  // if we're not a direction, keep track of slopes
  bool AdjustDone = false;
  Fraction slope, test;
  // where are we going? 
  if (e.ETarget) {
    e.EXVal = e.ETarget->x;
    e.EYVal = e.ETarget->y;
  }
  if (!e.isDir) {
    // first, handle degenerate cases to avoid slope division by 0
    if (e.EXVal * 2 == cx) {
      e.isDir = true;
      e.EDir = (sy < e.EYVal * 2) ? SOUTH : NORTH;
    } else if (e.EYVal * 2 == cy) {
      e.isDir = true;
      e.EDir = (sx < e.EXVal * 2) ? EAST : WEST;
    } else { 
      dirX = (e.EXVal * 2 == cx) ? 0 : ((e.EXVal*2 >= cx) ? 1 : -1);
      dirY = (e.EYVal * 2 == cy) ? 0 : ((e.EYVal*2 >= cy) ? 1 : -1);
      slope.Set(abs((e.EYVal*2 - dirY) - cy), 
          abs((e.EXVal*2 - dirX) - cx)); 
    } 
  }
  // map overlay 
  Map * orig_map = m; 
  Map &m = *(e.EActor->m);
  Overlay &o = e.EActor->m->ov;
  o.Activate();
  int distSoFar = 0;
  bool hitSelf = false;
  Thing * possTarget = NULL;
  if (Intended && e.EActor && e.EActor->DistFrom(Intended) < 1) {
    // you're shooting at a chest at you feet
    possTarget = Intended;
    goto hitNow; 
  } 
  // if you're intentionally striking yourself, skip the travel phase
  if ((e.isLoc && (e.EXVal == e.EActor->x && e.EYVal == e.EActor->y)) || 
      (e.isDir && (e.EDir == CENTER)) || 
      (!e.isDir && !e.isLoc && e.EActor == e.EVictim))
  {
    IPrint("There is no reason to shoot yourself."); 
    return ABORT; 
    possTarget = e.EActor;
    hitSelf = true; 
    goto hitNow; 
  }
  // travel! 
  while(!hitSelf && m.InBounds(cx/2,cy/2)// && !m.At(cx/2,cy/2).Solid
      ) {

    /* wall of air, presumably */
    if (TTER(m.TerrainAt(cx/2,cy/2))->HasFlag(TF_SPECIAL)) {
      e.Event = EV_MISSILE;
      if (TTER(m.TerrainAt(cx/2,cy/2))->Event(e,
            m.TerrainAt(cx/2,cy/2)) == ABORT)
        break;
      e.Event = EV_RATTACK;
    }
    
    if (m.FieldAt(cx/2,cy/2,FI_ITERRAIN))
      {
        rID tID = m.PTerrainAt(cx/2,cy/2,NULL);
        if (TTER(tID)->HasFlag(TF_SOLID) && e.EItem2)
          {
            e.EItem2->IDPrint(NULL, "The <hObj> passes through the "
              "<Res> as if it wasn't there!", e.EItem2, tID);
            m.TerrainDisbelief(cx/2,cy/2,NULL);
          }
      }

    /* last-known-good X and Y */
    lx = cx;
    ly = cy;

    // draw something on this square? 
    if (m.At(cx/2,cy/2).Visibility & VI_DEFINED) {
      o.AddGlyph(cx/2,cy/2, g | gcolour);
      o.ShowGlyphs();
      PossiblyPause(T1,cx/2,cy/2,30);
      o.DeActivate();
      o.Activate();
    }

    // move to the next spot! 
    if (e.isDir) {
      cx += DirX[e.EDir]*2;
      cy += DirY[e.EDir]*2;
      g = AdjustGlyphDir(TITEM(e.EItem2->iID)->Image,(Dir)e.EDir);
    }
    else if (!AdjustDone && dist(cx/2,cy/2,e.EXVal,e.EYVal)==1) {
      AdjustDone = true;
      cx = e.EXVal*2;
      cy = e.EYVal*2;        
    }
    else {
      test.Set(abs((cy + dirY)-sy), abs((cx + dirX)-sx));
      if (test > slope) {
        cx += 2 * dirX;
        if (dirX > 0) g = GLYPH_ARROW_RIGHT;
        else g = GLYPH_ARROW_LEFT;
      } else if (test < slope) {
        cy += 2 * dirY;
        if (dirY > 0) g = GLYPH_ARROW_DOWN;
        else g = GLYPH_ARROW_UP; 
      } else {
        cx += 2 * dirX;
        cy += 2 * dirY;
        if ((dirX < 0 && dirY < 0) /* NE */ ||
            (dirX > 0 && dirY > 0) /* SW */)
          g = '\\';
        else
          g= '/';
      }
    }


    Thing * ptList[16]; 
    int16 ptc;
    
    ptc = 0;
    for (possTarget=m.FCreatureAt(cx/2,cy/2);possTarget && ptc<16;
           possTarget=m.NCreatureAt(cx/2,cy/2))
      ptList[ptc++] = possTarget;
    possTarget = NULL;
    
    if (ptc)
      {
        /* If we're shooting into a square with two or more creatures,
           and one is the intended target, Precise Shot helps us hit
           the right one, but we still have a better than even chance
           of doing so anyway. */
        if (ptc > 1 && Intended && Intended->x == cx/2 && Intended->y == cy/2)
          if (Dice::Roll(1,100) <= 
                (e.EActor->HasFeat(FT_PRECISE_SHOT) ? 95 : 60))
            { possTarget = Intended; goto hitNow; }

        /* Otherwise, choose a random target */
        possTarget = ptList[random(ptc)];
      }

    /*
    DOESN'T work well with hanging vines as features, etc.
    
    if (!possTarget) {
      possTarget = m.KnownFeatureAt(cx/2,cy/2); 
      if (possTarget && possTarget->isType(T_DOOR) && 
          (((Door *)possTarget)->DoorFlags & DF_OPEN))
        possTarget = NULL;
      }
    */
    
    if (possTarget && possTarget != e.EActor) {
      lx = cx;
      ly = cy; 
      if (Intended && (possTarget != Intended))
        if (Dice::Roll(1,100) <= 
            (e.EActor->HasFeat(FT_PRECISE_SHOT) ? 75 : 25))
          goto SkipAttack;
hitNow: 
      EventInfo oe = e;
      e.ETarget = possTarget;
      int16 vicSize = e.ETarget->isCreature() ? 
        e.EVictim->GetAttr(A_SIZ) : SZ_MEDIUM; 
      e.Dmg     = (vicSize > SZ_MEDIUM) ?
        e.EItem2->LDmg() : e.EItem2->SDmg();
      if (e.EItem) {
        Dice d2 = vicSize > SZ_MEDIUM ?
          e.EItem->LDmg() : e.EItem->SDmg();
        e.Dmg.Number += d2.Number;
        e.Dmg.Sides += d2.Sides;
        e.Dmg.Bonus += d2.Bonus;
        e.vHit    = (int8)Attr[A_HIT_ARCHERY]; 
        e.Dmg.Bonus += Attr[A_DMG_ARCHERY];
        e.AType   = A_FIRE;
        // ww: +4 arrows give you +4 to hit and +4 to damage
        if (e.EItem2) {
          TItem *ti = TITEM(e.EItem2->iID);
          Dice d3 = vicSize > SZ_MEDIUM ?
            e.EItem2->LDmg() : e.EItem2->SDmg();
          e.Dmg.Bonus += e.EItem2->GetPlus() + d3.Bonus;
          e.vHit += ti->u.w.Acc + e.EItem2->GetPlus() + 
            e.EItem2->HasQuality(IQ_MITHRIL) - 
            e.EItem2->HasQuality(IQ_ORCISH);
        } 
      }
      else {
        // ww: very important, otherwise this stuff never gets calculated
        e.EActor->CalcValues(false,e.EItem2); 
        e.vHit       = (int8)Attr[A_HIT_THROWN]; 
        e.Dmg.Bonus += Attr[A_DMG_THROWN];
        e.AType      = A_HURL;
      }
      e.vDef    = (int8)(e.ETarget->isCreature () ? e.EVictim->getDef() : 0);
      e.DType   = (int8)(e.EItem ? e.EItem : e.EItem2)->DamageType(e.EVictim);
      e.vThreat = (int8)(e.EItem ? e.EItem : e.EItem2)->Threat(e.EActor);
      e.vCrit   = (int8)(e.EItem ? e.EItem : e.EItem2)->CritMult(e.EActor);
      if (ReThrow(EV_STRIKE, e) == ABORT)
        break;
      if (e.isHit)
        break;
      e = oe;
    }
SkipAttack:

    // features are things like doors and gravestones
    if (m.SolidAt(cx/2,cy/2))
      break; 

    // finally, if we aimed at *this specific spot* (e.g., to throw a
    // fireball in empty middle square between two enemies), stop here
    if (cx / 2 == e.EXVal && cy / 2 == e.EYVal) 
      break; 
  }

  o.ShowGlyphs();
  o.DeActivate();

  if (e.EItem2 && 
      (e.EItem2->HasQuality(WQ_RETURNING) ||
       (TITEM(e.EItem2->iID)->HasFlag(WT_RETURNING) && e.isHit))) {
     e.EItem2->Remove(false);
    e.EActor->IPrint(Format("%s returns to you.",(const char*)e.EItem2->Name(NA_THE)));     
    if (oldSlot != -1) {
      if (InSlot(oldSlot)) {
        if (InSlot(oldSlot)->TryStack(e.EItem2))
          goto Returned;
        }
      else if (ThrowVal(EV_WIELD,oldSlot,e.EActor,NULL,e.EItem2) == DONE)
        goto Returned;
      }
    e.EActor->GainItem(e.EItem2,true);
    Returned:;      
  } else { 
    e.EItem2->Image = g; 
    e.EItem2->PlaceAt(orig_map,lx/2,ly/2); 
    if (e.EItem2->m != orig_map)
      goto MissileDestroyed;
    /* The PlaceAt() call does not check for suitability of placement.  PlaceNear() does and may destroy the missile
       if it cannot move it to any suitable tile (i.e. missile fired and landing on magma "maze") */
    e.EItem2->PlaceNear(lx/2,ly/2); 
    if (e.EItem2->m != orig_map)
      goto MissileDestroyed;
    e.EItem2->SetImage();
    e.EItem2 = e.EItem2->TryStack(e.EItem2->m,e.EItem2->x,e.EItem2->y);
  }

  MissileDestroyed:
  if (e.EActor->HasFeat(FT_EXPERT_TACTICIAN) && e.EVictim && 
      (e.isSurprise || e.EVictim->noDexDefense()) && 
      !e.EActor->HasStati(TRIED,FT_EXPERT_TACTICIAN,e.EVictim)) { 
      e.EActor->GainTempStati(TRIED,e.EVictim,1,SS_MISC,FT_EXPERT_TACTICIAN);
    // attack takes no time
  } else if (e.EItem)
    Timeout += 3000 / 
        max((100 + Attr[A_SPD_ARCHERY]*5),10);
  else
    Timeout += 3000 / 
        max((100 + Attr[A_SPD_THROWN]*5),10);
  if (startedAfraid)
    Timeout *= 2;


  if (e.EItem) {
    SetSilence();
    e.EItem->RemoveStati(COCKED); 
    UnsetSilence();
  }
  
  if (e.EItem2 && e.EItem2->m)
    {
      e.EItem2->Image =
        TITEM(e.EItem2->iID)->Image;
      e.EItem2->m->Update(e.EItem2->x,e.EItem2->y);
    }
  
  return DONE;
}



EvReturn Creature::NAttack(EventInfo &e) /* this == EActor */
{
  TAttack* at; String s;
  int8 HitCount = 0, ClawCount = 0;
  bool OneAttack = false, isFF = false;
  bool startedAfraid = HasStati(AFRAID);
    
    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
      e.EItem = NULL;


  if (!attackSanity(e))
    return ABORT;

    if (e.EVictim && e.EVictim->isCreature())
      if (!e.EVictim->HasStati(AFRAID) || e.EVictim->HasFeat(FT_LION_HEART) ||
          e.EVictim->GetStatiVal(AFRAID) == FEAR_SKIRMISH)
        e.vicNotFleeing = true; 
    if (e.EVictim && e.EVictim->isCreature())
      if (!(e.EVictim->HasStati(PRONE) ||
            e.EVictim->HasStati(BLIND) ||
            e.EVictim->HasStati(PARALYSIS) ||
            e.EVictim->HasStati(STUCK)))
        e.vicReady = true;

  if (HasStati(AFRAID) && !(e.EVictim && !e.EVictim->isCreature())
        && !HasFeat(FT_LION_HEART)) {
    e.EActor->IPrint("You're too scared to attack!");
    return ABORT;
  }
  if (HasStati(CHARMED,-1,e.ETarget)) {
    IPrint("You have no desire to hurt your dear friend!");
    return ABORT;
  } else if (ts.hasTargetOfType(OrderDoNotAttack)) {
    return ABORT; 
  }
    if (HasStati(ILLUSION)) {
      if (!e.ETarget->isCreature())
        return ABORT;
      e.actIllusion = true;
      }
    if (e.EVictim->HasStati(ILLUSION))
      e.vicIllusion = true;

    if (HasStati(NAUSEA)) {
      if (HasSkill(SK_CONCENT) && SkillLevel(SK_CONCENT) >= 10)
        if (yn("You are Nauseated. Use Concentration to attack?"))
          {
            if (SkillCheck(SK_CONCENT,20,true))
              {
                IPrint("You overcome your nausea to attack!");
                goto OvercomeNausea;
              }
            else
              {
                IPrint("You try to attack, but only retch and vomit!");
                ProvokeAoO();
                Timeout += 40;
                return DONE;
              }
          }
      e.EActor->IPrint("You're too nauseated to attack!");
      return ABORT;
      }

  OvercomeNausea:

  // ww: I'm tired of getting raked through walls ...
  if (!m->InBounds(x,y) ||
      (e.EActor->onPlane() == PHASE_MATERIAL &&
       !(m->LineOfFire(x,y,e.ETarget->x,e.ETarget->y,this)))) {
    IPrint("You do not have a clear line of attack!"); 
    return ABORT; 
  } 
    if (!e.ETarget->isCreature() && !e.ETarget->isFeature() &&
        !e.ETarget->isItem()) {
      e.EActor->IPrint("You can't attack that sort of thing.");
      return ABORT; 
    }
  if (!e.EActor->isBeside(e.ETarget,
        e.EActor->StateFlags & MS_HAS_REACH ? 1 : 0)) {
        
    if (e.EActor->HasStati(TELEKINETIC))
      if (dist(e.EActor->x,e.EActor->y,e.ETarget->x,e.ETarget->y) <=
            e.EActor->HighStatiMag(TELEKINETIC))
        {
          if (!e.EMap->LineOfFire(e.EActor->x,e.EActor->y,
                                  e.ETarget->x,e.ETarget->y,e.EActor))
            {
              IPrint("You don't have a clear line of fire!");
              return ABORT;
            }
          if (!(e.EActor->Perceives(e.ETarget) & (PER_VISUAL|PER_INFRA|PER_BLIND)))
            {
              IPrint("You can only telekinese things you can see!");
              return ABORT;
            }            
          e.isTelekinetic = true;
          
        }
    if (!e.isTelekinetic)
      {
        IPrint("You can't strike something that far away.");
        return ABORT;
      }
  } 
  if (HasMFlag(M_SWARM)) {
    IPrint("Swarms don't make direct physical attacks."); 
    return ABORT;
  } 

  /* ww: are stuck in no-attack webbing or somesuch? */
  if (!m->InBounds(x,y) || TTER(m->TerrainAt(x,y))->Event(e,m->TerrainAt(x,y)) == ABORT)
    return ABORT; 

    if (e.ETarget->isCreature() && 
        e.EActor->isPlayer() && 
        e.EVictim->HasAttk(A_DEQU) &&
        e.EPActor->Opt(OPT_WARN_DEQU)) {
      TAttack *ta = e.EVictim->GetAttk(A_DEQU);
      int max = ta->u.a.Dmg.Number * ta->u.a.Dmg.Sides + ta->u.a.Dmg.Bonus;
      //if (e.EPActor->HasFeat(FT_SIGNATURE_ITEMS)) max /= 2; 
      int hard = EInSlot(SL_WEAPON) ? 
        EInSlot(SL_WEAPON)->Hardness(ta->DType) : 
        e.EActor->ResistLevel(ta->DType);
      if (ta->DType == AD_SHAT && e.EActor->ResistLevel(AD_SHAT) == -1) hard=-1;
      if (ta->DType == AD_RUST && e.EActor->ResistLevel(AD_RUST) == -1) hard=-1;
      if (ta->DType == AD_SOAK && e.EActor->ResistLevel(AD_SOAK) == -1) hard=-1;
      if (hard == -1 || hard >= max)
        ;
      else {
        s = Format("Attack %s (%s %s)?",
            (const char*)e.EVictim->Name(NA_THE),
            (const char*)ta->u.a.Dmg.Str(),
            Lookup(DTypeNames,ta->DType));
        if (!yn(s,true)) 
          return ABORT; 
      } 
    } 


  if (e.ETarget->isCreature() && !e.isTelekinetic && 
      (e.EActor->isPlayer() && ((Player *)e.EActor)->Opt(OPT_AUTO_COUP)) &&
        (e.EVictim->HasStati(SLEEPING) || 
        (e.EVictim->HasStati(STUCK) && e.EVictim->HasStati(PRONE) &&
            e.EActor->SkillLevel(SK_FIND_WEAKNESS) >= 20) ||
        e.EVictim->HasStati(PARALYSIS,PARA_HELD) ||
        (e.EVictim->HasStati(STUCK) && e.EVictim->HasStati(PRONE))) &&
        (!e.EActor->HasStati(CONFUSED)) &&
        (e.EVictim->isMType(MA_LIVING) ||
         (e.EVictim->isMType(MA_UNDEAD) && e.EActor->HasFeat(FT_NECROPHYSIOLOGY)))) {
      // coup-de-grace instead!
      return ThrowVal(EV_SATTACK,A_COUP,e.EActor,e.EVictim);
    } 

  if (e.ETarget->HasStati(GRABBED,-1,e.EActor))
    if (e.EActor->HasStati(GRAPPLING))
      e.vicHeld = true;

  if (e.EVictim->isCreature() &&  e.EVictim->isFlatFooted())
    isFF = true;

  /* Later: what if you have no hands? */
  if (HasStati(TOUCH_ATTACK) && !HasAttk(A_TUCH) && !e.isTelekinetic)
  {
    OneAttack = true;
    e.EItem   = NULL;
    e.vHit    = (int8)Attr[A_HIT_BRAWL];
    e.vDef      = e.ETarget->isCreature() ? e.EVictim->getDef() : 0;
    e.Dmg.Set(0,0,0);
    e.saveDC = 0;
    e.AType   = A_TUCH;
    e.DType   = AD_NORM; /* Unused */
    e.vThreat = 20; /* HasFeat(FT_IMPROVED_CRITICAL_BITE), etc. */
    e.vCrit   = 2;
    EvReturn res = ReThrow(EV_STRIKE, e);
    if (e.EActor->HasFeat(FT_EXPERT_TACTICIAN) && 
        (e.isSurprise || e.EVictim->noDexDefense()) 
        && !e.EActor->HasStati(TRIED,FT_EXPERT_TACTICIAN,e.EVictim)) { 
        e.EActor->GainTempStati(TRIED,e.EVictim,1,SS_MISC,FT_EXPERT_TACTICIAN);
      // attack takes no time
    } else Timeout += 2000 / max((100 + Attr[A_SPD_BRAWL]*5),10);
    /* Touch Attacks are quicker */
    return res; 
  }

  TAttack buf[1024];
  int max;
  if (e.isTelekinetic)
    { buf[0].AType = A_PUNC;
      buf[0].DType = AD_BLUNT;
      buf[0].u.a.Dmg =
        MonkDamage[max(1,e.EActor->
          AbilityLevel(CA_UNARMED_STRIKE))];
      max = 1; }
  else 
    max = ListAttacks(buf,1024);
  int count = 0, i =0; 

  if (e.vicHeld && !e.isTelekinetic) {
    for (i=0;i<max;i++) { 
      TAttack * at = & buf[i]; 
      if (is_postgrab_attk(at->AType)) {
        ASSERT(++count < 50); 
        OneAttack = true;
        if (at->DType == AD_GRAB && e.vicHeld)
          continue;
        if (at->AType == A_ALSO)
          continue;
        e.EItem     = NULL;
        e.vHit      = (int8)Attr[A_HIT_BRAWL];
        e.vDef      = e.ETarget->isCreature() ? e.EVictim->getDef() : 0;
        e.Dmg       = at->u.a.Dmg;
        // ww: this check seems necessary, otherwise fire damage is
        // related to STR and whatnot ...
        if (at->DType >= AD_SLASH && at->DType <= AD_BLUNT) 
          e.Dmg.Bonus += e.EActor->Attr[A_DMG_BRAWL];
       e.saveDC = (int8)e.EActor->GetPower(at->u.a.DC);
        e.AType   = at->AType;
        e.DType   = at->DType;
        e.vThreat = 20;
        e.vCrit   = 2;
        e.isNAttack = true;
        OneAttack = true;
        if (ReThrow(EV_STRIKE, e) == ABORT) {
          return ABORT;
        } 
        if (e.Died || e.ADied) {
          goto DoneSequence;
        } 
      }
    }
    // ww: why shouldn't a vampire get to keep clawing you (say, your back)
    // while it is grappling you in a life-or-death situation? 
    /* if (OneAttack) {
      goto DoneSequence;
    }  */
  }

  for (i=0;i<max;i++) {
    TAttack * at = &buf[i]; 
    if (is_standard_attk(at->AType)) {
      if (at->DType == AD_GRAB && e.vicHeld)
        continue;
      if (at->AType == A_ALSO)
        continue;
      e.vHit = e.isHit = e.isCrit = e.isFumble = e.Immune = 
        e.MagicRes = e.Resist = e.isWImmune = e.isEvaded = 
        e.isPartiallyEvaded = false;  
      e.EItem     = NULL;
      e.vHit      = (int8)Attr[A_HIT_BRAWL];
      if (OneAttack && !(isFF && e.EActor->HasFeat(FT_POUNCE)))
        e.vHit -= e.EActor->HasFeat(FT_MULTIATTACK) ? 2 : 5;
      e.vDef      = e.ETarget->isCreature() ? e.EVictim->getDef() : 0;
      e.Dmg       = at->u.a.Dmg;
      e.bDmg = e.vDmg = e.aDmg = e.xDmg = 0; 
      // ww: wow, evil typo: e.Dmg.Bonus += e.EVictim->Attr[A_DMG_BRAWL]; 
      // ww: this check seems necessary, otherwise fire damage is
      // related to STR and whatnot ...
      if (at->DType >= AD_SLASH && at->DType <= AD_BLUNT) 
        e.Dmg.Bonus += e.EActor->Attr[A_DMG_BRAWL];
      e.saveDC = (int8)e.EActor->GetPower(at->u.a.DC);
      e.AType   = at->AType;
      e.DType   = at->DType;
      e.vThreat = 20; /* HasFeat(FT_IMPROVED_CRITICAL_BITE), etc. */
      e.vCrit   = 2;
      /* Normally, secondary natural attacks inflict damage with only
         1/2 of the creature's Strength bonus (or the full penalty,
         for low-Strength monsters. If the creature has the Rend Feat,
         however, and is either striking a held victim or has already
         hit the victim once this attack sequence, the creature may add
         his full Strength bonus to the damage of all attacks. */
      if (OneAttack && e.EActor->Mod(A_STR) > 0)
        if (!e.EActor->HasFeat(FT_REND) || !(e.vicHeld || HitCount))
          e.Dmg.Bonus -= e.EActor->Mod(A_STR)/2;
      e.isNAttack = true;
      if (ReThrow(EV_STRIKE, e) == ABORT) {
        return ABORT;
      } 
      if (e.isHit)
        HitCount++;
      if (e.isHit && e.AType == A_CLAW)
        ClawCount++;
      OneAttack = true;
      if (e.Died || e.ADied)  {
        goto DoneSequence;
      } 
      else if (e.isHit && e.EActor->HasFeat(FT_NATURAL_GRAB) && !e.isTelekinetic) {
        if (HitCount == 1 || e.EActor->HasFeat(FT_MULTIATTACK)) {
          /* ww: srd: you can then start a grapple attack as a free
           * action without provoking an attack of opportunity ... */
          if (e.EActor->HasStati(GRAPPLING)) {
            e.EActor->IPrint("You can only grapple one target at once.");
          } else {
            EventInfo e2 = e;
            e2.Event = EV_STRIKE; 
            e2.AType = A_GRAB; 
            e2.DType = AD_GRAB; 
            e2.Resist = false; 
            e2.saveDC = -1; // no save, normal maneuver check suffices!
            e2.isHit = true; 
            e2.vRoll = 10; 
            ReThrow(EV_STRIKE,e2); 
          } 
          if (e.EVictim->HasStati(GRABBED,-1,e.EActor))
            if (e.EActor->HasStati(GRAPPLING))
              e.vicHeld = true;
        }
      } 
    }
  }

  if (e.EActor->HasFeat(FT_RAKE) && ClawCount >= 2) {
    at = e.EActor->GetAttk(A_CLAW);
    ASSERT(at);
    e.EItem     = NULL;
    e.vHit      = (int8)Attr[A_HIT_BRAWL];
    e.vDef = e.ETarget->isCreature() ? e.EVictim->getDef() : 0;
    e.Dmg       = at->u.a.Dmg;
    e.Dmg.Bonus += e.EActor->Attr[A_DMG_BRAWL]; 
    e.saveDC = (int8)e.EActor->GetPower(at->u.a.DC);
    e.AType   = A_RAKE;
    e.DType   = at->DType;
    e.vThreat = 20; /* HasFeat(FT_IMPROVED_CRITICAL_BITE), etc. */
    e.vCrit   = 2;
    e.isNAttack = false;
    OneAttack = true;
    if (ReThrow(EV_STRIKE, e) == ABORT)
      return ABORT;
    if (e.Died || e.ADied)
      goto DoneSequence;
  }

  if (HitCount && e.ETarget->isCreature() && !e.EVictim->isDead() && 
          e.EActor->HasStati(MOUNT) && !e.isTelekinetic)
    if (((Creature*)e.EActor->GetStatiObj(MOUNT))->HasFeat(FT_OVERRUN))
    {
      e.saveDC = 10 + ((Creature*)e.EActor->GetStatiObj(MOUNT))->
        SkillLevel(SK_RIDE) / 2;
      e.DType = AD_TRIP;
      ReThrow(EV_DAMAGE,e);
    }

  if (e.EActor->HasFeat(FT_TRAMPLE) && e.EVictim->HasStati(PRONE) && !e.isTelekinetic) {
    e.EItem     = NULL;
    e.vHit      = (int8)Attr[A_HIT_BRAWL];
    e.vDef = e.ETarget->isCreature() ? e.EVictim->getDef() : 0;
    e.Dmg.Set(1,12,(int8)e.EActor->Attr[A_DMG_BRAWL]);
    e.saveDC = (int8)e.EActor->GetPower(10 + e.EActor->ChallengeRating());
    e.AType   = A_TRAM;
    e.DType   = AD_BLUNT;
    e.vThreat = 20; /* HasFeat(FT_IMPROVED_CRITICAL_TRAMPLE) ? 18 : 20. */
    e.vCrit   = 2;
    e.isNAttack = false;
    OneAttack = true;
    if (ReThrow(EV_STRIKE, e) == ABORT)
      return ABORT;
    if (e.Died || e.ADied)
      goto DoneSequence;
  }

DoneSequence:

  if (!OneAttack)
  {
    IPrint("You have no method of attack in this form.");
    return ABORT;
  }
  if (e.EActor->HasFeat(FT_EXPERT_TACTICIAN) && 
      (e.isSurprise || e.EVictim->noDexDefense())
      && !e.EActor->HasStati(TRIED,FT_EXPERT_TACTICIAN,e.EVictim)) { 
    e.EActor->GainTempStati(TRIED,e.EVictim,1,SS_MISC,FT_EXPERT_TACTICIAN);
    // takes no time
  } else Timeout += 3000 / max((100 + Attr[A_SPD_BRAWL]*5),10);

  if (startedAfraid)
    Timeout *= 2;

  return DONE;
}

EvReturn Creature::SAttack(EventInfo &e) { /* this == EActor */
    static EffectValues ef;
    TEffect *te;
    EvReturn r;
    Thing *t;
    int16 i, max;
    bool startedAfraid = HasStati(AFRAID);

    if (!e.AType)
        e.AType = (int8)e.EParam; 
    TAttack *ta = TMON(mID)->GetAttk(e.AType);
    e.vThreat = 20;

    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
        e.EItem = NULL;

    StatiIterNature(this,SPEC_TIMEOUT)
        if (S->Val == e.AType)
            if (!S->h || e.EVictim->myHandle == S->h) {           
                IPrint("That ability is timed out right now.");
                StatiIterBreakout(this,return ABORT)
            }
    StatiIterEnd(this)

    // ww: may well get an attack from a template!
    if (!ta) {
        StatiIterNature(this,TEMPLATE)
            TTemplate *tt = TTEM(S->eID);
            ta = tt->GetNewAttk(e.AType);
        StatiIterEnd(this)
    }
    if (!ta)
        ta = TMON(tmID)->GetAttk(e.AType);

    if (!m->InBounds(x,y) ||
            (e.EVictim && e.EVictim->isCreature() && e.EActor->onPlane() == PHASE_MATERIAL &&
             !(m->LineOfFire(x,y,e.EVictim->x,e.EVictim->y,this)))) {
        IPrint("You do not have a clear line of attack!"); 
        return ABORT; 
    } 

    if (e.EVictim)
        if (!attackSanity(e))
            return ABORT;

    if (e.AType == A_GRAB || e.AType == A_BULL || e.AType == A_KICK || e.AType == A_THRO || e.AType == A_DISA || e.AType == A_TRIP || e.AType == A_PREC) {
        int16 reachDist = 1 + FaceRadius[e.EActor->GetAttr(A_SIZ)] + (e.EActor->StateFlags & MS_HAS_REACH ? 1 : 0);
        if (e.EVictim->isCreature())
            reachDist += FaceRadius[e.EVictim->GetAttr(A_SIZ)];
        if (e.EActor->DistFrom(e.EVictim) > reachDist) {
            if (e.EActor->HasStati(TELEKINETIC) && e.EActor->HighStatiMag(TELEKINETIC) >= e.EActor->DistFrom(e.EVictim))
                e.isTelekinetic = true;
            else {
                e.EActor->IPrint("That target is out of range for that maneuver.");
                return ABORT;
            }
        }
    }

    if (e.AType != A_PROX && e.AType != A_EXPL && e.AType != A_DGST && e.AType != A_DEFN && e.AType != A_AEYE && e.AType != A_CRUS &&
            e.AType != A_SPRT && e.AType != A_DEQU && e.AType != A_AURA) {
        if (HasStati(AFRAID) && !(e.EVictim && !e.EVictim->isCreature()) && !HasFeat(FT_LION_HEART))  {
            e.EActor->IPrint("You're too scared to attack!");
            return ABORT;
        } else if (e.EVictim && HasStati(CHARMED,-1,e.EVictim)) {
            IPrint("You have no desire to hurt your dear friend!");
            return ABORT;
        } else if (HasStati(NAUSEA)) {
            if (HasSkill(SK_CONCENT) && SkillLevel(SK_CONCENT) >= 10)
                if (yn("You are Nauseated. Use Concentration to attack?")) {
                    if (SkillCheck(SK_CONCENT,20,true)) {
                        IPrint("You overcome your nausea to attack!");
                        goto OvercomeNausea;
                    } else {
                        IPrint("You try to attack, but only retch and vomit!");
                        ProvokeAoO();
                        Timeout += 40;
                        return DONE;
                    }
                }
            e.EActor->IPrint("You're too nauseated to attack!");
            return ABORT;
        }
    }
OvercomeNausea:
    if (HasStati(CONFUSED)) {
        IPrint("You're too confused to make a non-standard attack.");
        return ABORT;
    } else if (HasStati(NEGATED))
        if ((r = HandleNegate(e)) != NOTHING) {
            return r;
        } else if (e.EVictim && HasStati(CHARMED,-1,e.EVictim)) {
            IPrint("You have no desire to hurt your dear friend!");
            return ABORT;
        } else if (ts.hasTargetOfType(OrderDoNotAttack)) {
            return ABORT; 
        } else if (!m) { // you can't attack if you're nowhere
            return ABORT;
        /* ww: are stuck in no-attack webbing or somesuch? */
        } else if (x != -1 && TTER(m->TerrainAt(x,y))->Event(e,m->TerrainAt(x,y)) == ABORT)
            return ABORT; 

    if (e.EVictim && e.EVictim->isCreature())
        if (!e.EVictim->HasStati(AFRAID) || e.EVictim->GetStatiVal(AFRAID) == FEAR_SKIRMISH || e.EVictim->HasFeat(FT_LION_HEART))
            e.vicNotFleeing = true; 
    if (e.EVictim && e.EVictim->isCreature())
        if (!(e.EVictim->HasStati(PRONE) || e.EVictim->HasStati(BLIND) || e.EVictim->HasStati(PARALYSIS) || e.EVictim->HasStati(STUCK)))
            e.vicReady = true;

    switch (e.AType) {
    case A_BREA:
    case A_BRE2:
    case A_SPIT: 
        if (!LoseFatigue(1,true))
            return ABORT;
        /* This is a kludgy way to handle breath weapons, writing
        the effects into a dummy resource in memory -- but it
        is far, far better then the old way! */        
        //ASSERT(e.ETarget);
        if (e.AType == A_BREA || e.AType == A_BRE2) 
            e.eID = FIND("Breath Weapon");
        else 
            e.eID = FIND("Spit Weapon");
        te = TEFF(e.eID);

        /*if (e.ETarget->x != x && e.ETarget->y != y && 
        abs(e.ETarget->x - x) != abs(e.ETarget->y - y))
        return ABORT; */

        if (!ta) return ABORT; 

        te->ef.pval = ta->u.a.Dmg;
        te->ef.pval.Number += e.EActor->GetPower(0)*2;
        te->ef.pval.Number = max(1,e.Dmg.Number);
        e.Dmg = te->ef.pval;
        te->ef.lval = 3 + e.EActor->ChallengeRating()/2;
        e.vRange = te->ef.lval;
        e.DType = te->ef.xval = ta->DType;
        e.isNAttack = true;
        ReThrow(EV_EFFECT,e);    
        GainTempStati(SPEC_TIMEOUT,NULL,random(4)+2,SS_MISC,e.AType);
        Timeout += 30; /* For game balance, there is no speed on
                        breath weapons; they always take a full
                        10 phases. */
        if (startedAfraid)
            Timeout *= 2;
        return DONE;
        break;
    case A_EXPL:
        if (!ta)
            return ABORT; 
        e.Dmg    = ta->u.a.Dmg;
        e.AType  = ta->AType;
        e.DType  = ta->DType;
        e.saveDC = (int8)e.EActor->GetPower(ta->u.a.DC);
        ReThrow(EV_ATTACKMSG,e);
        MapIterate(m,t,i) 
            if (t->isCreature())
                if (DistFrom(t) <= 3 && m->LineOfFire(x,y,t->x,t->y,this))
                    if (!((Creature*)t)->HasMFlag(M_BLIND) || !isMType(MA_VORTEX)) {
                        e.ETarget = t;
                        ReThrow(EV_HIT,e);
                    }  
        e.ETarget = NULL;
        Remove(true);
        return DONE;       
    case A_DGST:
    case A_CRUS:
        if (!ta)
            return ABORT; 
        e.Dmg    = ta->u.a.Dmg;
        if (e.AType == A_CRUS)
            e.Dmg.Bonus += (e.EActor->Mod(A_STR)*3)/2;
        e.AType  = ta->AType;
        e.DType  = ta->DType;
        e.saveDC = (int8)e.EActor->GetPower(ta->u.a.DC);
        e.isHit  = true;
        ReThrow(EV_HIT,e);
        ReThrow(EV_ATTACKMSG,e);
        break; 
    case A_ROAR: 
    case A_GEST:
        //ASSERT(ta);
        if (!ta)
            return ABORT;

        Reveal(true);
        if (e.AType == A_ROAR)
            MapIterate(m,t,i) 
                if (t->isCreature() && DistFrom(t) <= 20)
                    if (t->HasStati(SINGING))
                        if (t->GetStati(SINGING)->Val == BARD_COUNTER) {
                            e.EMap->SetQueue(QUEUE_DAMAGE_MSG);
                            ((Creature*)t)->IDPrint("Your singing negates the effect.",
                                "The <Obj>'s singing negates the effect.",t);
                            e.EMap->UnsetQueue(QUEUE_DAMAGE_MSG);
                            goto SkipSoundAttack;
                        }

        TAttack buf[1024];
        max = ListAttacks(buf,1024);

        MapIterate(m,t,i) 
            if (t->isCreature())
                if (DistFrom(t) <= 12 && m->LineOfFire(x,y,t->x,t->y,this))
                    if (!((Creature*)t)->HasMFlag(M_DEAF) && ((Creature*)t)->ResistLevel(AD_SONI) != -1) {
                        if (e.EActor == t)
                            continue;
                        e.isNAttack = true;
                        for (int j=0;j<max;j++) {
                            TAttack * ta = &buf[j]; 
                            if (ta->AType != e.AType) continue; 
                            ASSERT(ta);
                            e.ETarget = t;
                            e.EMap->SetQueue(QUEUE_DAMAGE_MSG);
                            do {
                                e.Dmg    = ta->u.a.Dmg;
                                e.vDmg   = max(1,e.Dmg.Roll());
                                e.DType  = ta->DType;
                                e.saveDC = (int8)e.EActor->GetPower(ta->u.a.DC);
                                e.isHit = true; 
                                ReThrow(EV_HIT,e);
                                if (j++ == max) break; 
                                ta = &buf[j];
                            }
                            while ((ta->AType == A_ALSO));
                            e.EMap->UnsetQueue(QUEUE_DAMAGE_MSG);
                        }
                    }
SkipSoundAttack:  
        e.ETarget = NULL;
        e.EActor->Timeout += 30;
        if (startedAfraid)
            Timeout *= 2;

        ReThrow(EV_ATTACKMSG,e);
        return DONE;       
    case A_CPRX:
    case A_PROX:
        {
            if (!ta)
                return ABORT; 
            ASSERT(e.ETarget);
            ASSERT(ta);
            TAttack buf[1024];
            int max = ListAttacks(buf,1024); 
            for (i=0;i<max;i++) {
                TAttack * ta = &buf[i]; 
                if ((ta->AType == e.AType) ||
                    // all normal attacks from a swarm are not
                    // proximity attacks
                        (e.EActor->HasMFlag(M_SWARM) && (is_postgrab_attk(ta->AType) || is_standard_attk(ta->AType) || ta->AType == A_ALSO))) {
                    e.EActor->Reveal(true);
                    EventInfo e2 = e; 
                    e2.Dmg    = ta->u.a.Dmg;
                    e2.vDmg   = max(1,e2.Dmg.Roll());
                    e2.strDmg = Format(" (%d) (proximity)",e2.vDmg);
                    e2.DType  = ta->DType;
                    e2.saveDC = (int8)e2.EActor->GetPower(ta->u.a.DC);
                    e2.isHit = true; 
                    e2.isNAttack = true;
                    ReThrow(EV_DAMAGE,e2);
                    ReThrow(EV_ATTACKMSG,e2);
                }
            }
        }
        return DONE;
    case A_GAZE:
        if (!ta)
            return ABORT; 
        ASSERT(e.ETarget);
        if (!m->LineOfVisualSight(x,y,e.ETarget->x,e.ETarget->y,this)) {
            IPrint("You don't have a clear line of sight.");
            return ABORT;
        }
        if (e.ETarget->HasStati(GAZE_REFLECTION,GR_REFLECT)) {
            DPrint(e,"Your gaze is reflected back on you!",
                "The <EActor>'s gaze is reflected!");
            e.EVictim = e.EActor; 
        } 
        if (e.ETarget->HasStati(EYES_AVERTED) && random(100) < 50) {
            TPrint(e,"The <EVictim> averts its eyes from your gaze!",
                "You avert your eyes from the <EVictim>'s gaze!",
                "The <EVictim> averts its eyes from the <EActor>'s gaze!");
            e.Immune = true;
            return DONE; 
        } 
        if (e.EVictim->isBlind() || e.EVictim->HasStati(GAZE_REFLECTION,GR_IMMUNE)) { 
                DPrint(e,"The <EVictim> is immune to your gaze!",
                    "The <EVictim> is immune to the <EActor>'s gaze!");
                e.Immune = true;
                return DONE; 
        }
        { // quite possible to have multiple gaze attacks!
            TAttack buf[1024];
            int max = ListAttacks(buf,1024);
            for (i=0;i<max;i++) {
                TAttack * ta = &buf[i]; 
                if (ta->AType == A_GAZE) {
                    e.Dmg    = ta->u.a.Dmg;
                    e.vDmg   = max(1,e.Dmg.Roll());
                    e.DType  = ta->DType;
                    e.saveDC = (int8)e.EActor->GetPower(ta->u.a.DC);
                    e.strDmg = ""; 
                    e.isHit = true; 
                    e.Resist = e.Immune = false; 
                    e.EMap->SetQueue(QUEUE_DAMAGE_MSG);
                    e.EActor->LoseMana(2,true);
                    ReThrow(EV_DAMAGE,e);
                    e.EMap->UnsetQueue(QUEUE_DAMAGE_MSG);
                    ReThrow(EV_ATTACKMSG,e);
                } 
            }
        }
        return DONE; 
    case A_GATE:
        const char *plane;

        if (!ta)
            return ABORT; 

        if (HasStati(SPEC_TIMEOUT, A_GATE)) {
            IPrint("You can only use your gate ability once a day.");
            return ABORT;
        }

        GainTempStati(SPEC_TIMEOUT,NULL,600,SS_MISC,A_GATE);
        if (isMType(MA_CHAOTIC) && isMType(MA_EVIL))
            plane = "the Abyss";
        else if (isMType(MA_LAWFUL) && isMType(MA_EVIL))
            plane = "the Nine Hells";
        else if (isMType(MA_GOOD))
            plane = "the Upper Planes";
        else if (isMType(MA_LAWFUL))
            plane = "Nirvana";
        else if (isMType(MA_CHAOTIC))
            plane = "Pandemonium";
        else if (isMType(MA_EVIL))
            plane = "Hades";
        else
            plane = "the Concordant Opposition";

        if (!e.Terse)
            IDPrint("You invoke the powers of <Str>!",
            "The <Obj2> invokes the powers of <Str1>!",plane,this);
        if (random(100)+1 > ta->u.a.Dmg.Bonus) {
            IDPrint("Your call remains unanswered.",
                "<His:Obj> call remains unanswered.",this);
        } else {
            XTHROW(EV_ENGEN,e,
                xe.enFlags = EN_SUMMON;
            xe.enID = FIND("gated outsiders");
            xe.EXVal = x;
            xe.EYVal = y;
            xe.enCR = ta->u.a.DC;
            xe.enDepth = m->Depth;
            xe.vDuration = 300;
            );
        }
        Timeout += /*HasFeat(FT_RAPID_GATE) ? 30 : */ 50;
        return DONE;
    case A_AURA: 
        {
            if (!ta)
                return ABORT; 

            EventInfo e2 = e; 
            e2.Dmg    = ta->u.a.Dmg;
            e2.vDmg   = e2.Dmg.Roll();
            e2.strDmg = ""; 
            if (e.EVictim->HasFeat(FT_FISTS_OF_IRON)) {
                e2.vDmg /= 2;
                e2.strDmg += " / 2 FoI";
            } 
            e2.isHit = true; 
            ReThrow(EV_DAMAGE,e2);
            ReThrow(EV_ATTACKMSG,e2);
            break; 
        }
    case A_DEQU:
        {
            if (!ta)
                return ABORT; 

            Item *it;
            EventInfo e2 = e; 
            e2.DType  = ta->DType;
            e2.isHit = true; 
            e2.strDmg = ""; 
            e2.saveDC = (int8)e2.EActor->GetPower(ta->u.a.DC);
            it = e.EItem2;
            if (!it)
                it = e.EItem;

            if (!it) {
                // ok, apply to attacker
                e2.Dmg    = ta->u.a.Dmg;
                e2.vDmg   = e2.Dmg.Roll();
                if (e2.saveDC > 0)
                    if (e2.EVictim->SavingThrow(REF,e2.saveDC)) {
                        VPrint(e2,"You avoid harm to yourself when striking the <EActor>.",
                            "The <EVictim> avoids harm when striking the <EActor>.");
                        return DONE;
                    }
                if (e.EVictim->HasFeat(FT_FISTS_OF_IRON)) {
                    e2.vDmg /= 2;
                    e2.strDmg += " / 2 FoI";
                } 
                ReThrow(EV_DAMAGE,e2);
                ReThrow(EV_ATTACKMSG,e2);
                return DONE; 
            } else { 
                e2.Dmg    = ta->u.a.Dmg;
                e2.Dmg.Number *= 3; 
                e2.Dmg.Bonus *= 3; 
                e2.vDmg   = e2.Dmg.Roll();
                e2.EItem = it;
                e2.EItem2 = NULL;
                e2.ETarget = it; 
                if (e.saveDC > 0)
                    if (e.EVictim->SavingThrow(REF,e.saveDC)) {
                        VPrint(e2,"You avoid harm to your <EItem>.",
                            "The <EVictim> protects <his:EVictim> <EItem>.");
                        return DONE;
                    }
                ReThrow(EV_DAMAGE,e2);
                ReThrow(EV_ATTACKMSG,e2);
                return DONE;
            }
            break; 
        }
    //case A_DETH:
    case A_GREA:
        int16 fc;
        if (HasStati(EXPOSED)) {
            IPrint("You're still off balance from the last great blow.");
            return ABORT;
        }
        fc = max(0,2 - e.EActor->AbilityLevel(CA_MIGHTY_STROKE));
        if (fc && !e.EActor->LoseFatigue(fc,true))
            return ABORT;

        if (!EInSlot(SL_WEAPON)) {
            if (!TMON(mID)->Attk[0].AType) {
                IPrint("You have no active method of attack in this form.");
                return ABORT;
            }
            e.EItem   = NULL;
            e.vHit    = (int8)Attr[A_HIT_BRAWL];
            e.vDef    = (int8)e.EVictim->getDef();
            e.Dmg     = DmgVal(S_BRAWL,e.EVictim->GetAttr(A_SIZ) > SZ_MEDIUM);
            e.AType   = TMON(mID)->Attk[0].AType;
            e.DType   = TMON(mID)->Attk[0].DType;
            e.vThreat = 17;
            e.vCrit   = 2;
        } else {
            e.EItem    = EInSlot(SL_WEAPON);
            e.Dmg      = e.EActor->DmgVal(S_MELEE, 
                e.EVictim->GetAttr(A_SIZ) > SZ_MEDIUM);
            e.vHit     = (int8)Attr[A_HIT_MELEE]; /*+ e.EItem->ToHit() */
            e.vDef     = (int8)e.EVictim->getDef();
            e.AType    = A_SWNG;
            e.DType    = (int8)e.EItem->DamageType(e.EVictim);
            e.vThreat  = e.EItem->Threat(e.EActor)-3;
            e.vCrit    = (int8)e.EItem->CritMult(e.EActor);
        }
        e.isGreatBlow = true;
        if (ReThrow(EV_STRIKE, e) == ABORT)
            return ABORT;

        if (e.EActor->HasFeat(FT_MASTER_GREAT_BLOW)) {
            e.EActor->Timeout += 3000 / max((100 + e.EActor->Attr[e.EItem ? A_SPD_MELEE : A_SPD_BRAWL]*5),10);
            e.EActor->GainTempStati(EXPOSED,NULL,1,SS_MISC,0,2);
        } else {
            e.EActor->Timeout += 5000 / max((100 + e.EActor->Attr[e.EItem ? A_SPD_MELEE : A_SPD_BRAWL]*5),10);
            e.EActor->GainTempStati(EXPOSED,NULL,2,SS_MISC,0,2);
        }
        if (startedAfraid)
            e.EActor->Timeout *= 2;
        break;
    case A_PREC:
        if (HasStati(EXPOSED)) {
            IPrint("You cannot make precision strokes while off balance.");
            return ABORT;
        }
        if (HasStati(STUNNED)) {
            IPrint("You cannot make precision strokes while stunned.");
            return ABORT;
        }
        if (HasStati(PRONE)) {
            IPrint("You cannot make precision strokes while prone.");
            return ABORT;
        }

        if (!EInSlot(SL_WEAPON)) {
            if (!TMON(mID)->Attk[0].AType) {
                IPrint("You have no active method of attack in this form.");
                return ABORT;
            }
            // TODO: What if the monster type has a ranged method of attack?
            e.EItem   = NULL;
            e.vHit    = (int8)Attr[A_HIT_BRAWL];
            e.vDef    = (int8)e.EVictim->getDef();
            e.Dmg     = DmgVal(S_BRAWL,e.EVictim->GetAttr(A_SIZ) > SZ_MEDIUM);
            e.AType   = TMON(mID)->Attk[0].AType;
            e.DType   = TMON(mID)->Attk[0].DType;
            e.vThreat = 17;
            e.vCrit   = 2;
        } else {
            int8 attackMode = AttackMode();
            if (attackMode == S_ARCHERY || attackMode == S_THROWN) {
                IPrint("You cannot make precision strokes with a ranged weapon.");
                return ABORT;
            }

            e.EItem    = EInSlot(SL_WEAPON);
            e.Dmg      = e.EActor->DmgVal(S_MELEE, e.EVictim->GetAttr(A_SIZ) > SZ_MEDIUM);
            e.vHit     = (int8)Attr[A_HIT_MELEE]; 
            e.vDef     = (int8)e.EVictim->getDef();
            e.AType    = A_SWNG;
            e.DType    = (int8)e.EItem->DamageType(e.EVictim);
            e.vThreat  = e.EItem->Threat(e.EActor)-3;
            e.vCrit    = (int8)e.EItem->CritMult(e.EActor);
        }
        e.isPrecision = true;
        if (ReThrow(EV_STRIKE, e) == ABORT)
            return ABORT;
        e.EActor->Timeout += 3000 / max((100 + e.EActor->Attr[e.EItem ? A_SPD_MELEE : A_SPD_BRAWL]*5),10);
        e.EActor->Timeout += 45;
        if (startedAfraid)
            e.EActor->Timeout *= 2;
        break;
    case A_BULL:
        if (e.EActor->HasStati(GRAPPLED) && !e.isTelekinetic) {
            e.EActor->IPrint("You can't bull rush while grappled.");
            return ABORT;
        } else if (e.EVictim->HasMFlag(M_SWARM) && !e.isTelekinetic) {
            IPrint("You can't bull rush a swarm!");
            break;
        }
        if (e.EActor->HasStati(GRABBED) && !e.isTelekinetic) {
            e.EActor->IPrint("You can't bull rush while ensnared.");
            return ABORT;
        }
        if (e.EActor->HasStati(GRAPPLING) && !e.isTelekinetic) {
            e.EActor->IPrint("You can't bull rush while grappling.");
            return ABORT;
        }
        if (e.EActor->HasStati(STUCK) && !e.isTelekinetic) {
            e.EActor->IPrint("You can't bull rush while stuck.");
            return ABORT;
        } else if (e.EActor->onPlane() != e.EVictim->onPlane() && !e.isTelekinetic) {
            IPrint("You cannot bull rush a creature on another plane!"); 
            break; 
        } 
        e.EParam = e.EActor->ChoicePrompt("Displace, force forward or move past?","dfm",'f');
        if (e.EParam == -1)
            return ABORT;

        e.Dmg.Set(0,0,0);
        e.vHit  = (int8)Attr[A_HIT_BRAWL];
        e.vDef  = (int8)e.EVictim->getDef();
        e.DType = AD_BLUNT;

        if (!HasFeat(FT_MASTER_BULL_RUSH))
            e.EActor->ProvokeAoO(e.EVictim);
        if (e.EActor->isDead())
            return DONE;        

        if (ReThrow(EV_STRIKE,e) == ABORT)
            return ABORT;

        e.EActor->Timeout += 3000 / max((100 + min(e.EActor->Attr[A_SPD_BRAWL],e.EActor->Attr[A_MOV])*5), 10);
        if (startedAfraid)
            e.EActor->Timeout *= 2;
        return DONE;
    case A_TRIP:
        if (e.EActor->HasStati(STUCK) && !e.isTelekinetic) {
            if (!e.isCounterTrip)
                IPrint("You can't trip something while stuck.");
            return ABORT;
        } else if (e.EVictim->isAerial()) {
            if (!e.isCounterTrip)
                IPrint("You can't trip a flying creature!");
            break;
        } else if (e.EVictim->HasMFlag(M_SWARM)) {
            if (!e.isCounterTrip)
                IPrint("You can't trip a swarm!");
            break;
        } else if (e.EVictim->HasStati(PRONE)) {
            if (!e.isCounterTrip)
                IPrint("<He:Obj>'s already prone!",e.EVictim);
            break;
        } else if (e.EActor->onPlane() != e.EVictim->onPlane() && !e.isTelekinetic) {
            if (!e.isCounterTrip)
                IPrint("You cannot trip a creature on another plane!"); 
            break; 
        } else if (e.EActor->AttackMode() == S_MELEE) {
            e.vHit = (int8)e.EActor->Attr[A_HIT_MELEE];
            e.EItem = e.EActor->EInSlot(SL_WEAPON);
        } else
            e.vHit = (int8)e.EActor->Attr[A_HIT_BRAWL];
        e.vDef = (int8)e.EVictim->getDef();
        e.DType = AD_TRIP; e.saveDC = -1;
        e.Dmg.Set(0,0,0);

        if (ReThrow(EV_STRIKE,e) == ABORT)
            return ABORT;

        if ((e.Resist || e.Immune) && !e.isCounterTrip)
            if (e.EActor->HasMFlag(M_HUMANOID) && !e.EVictim->HasMFlag(M_NOLIMBS)) {
                EventInfo e2;
                e2.Clear();
                e2 = e;
                e2.EVictim = e.EActor;
                e2.EActor =  e.EVictim;
                e2.isCounterTrip = true;
                ReThrow(EV_SATTACK, e2);
            }

        if (e.isCounterTrip)
            return DONE;
        if (e.EActor->AttackMode() == S_MELEE) 
            e.EActor->Timeout = 3000 / max((100 + e.EActor->Attr[A_SPD_MELEE]*5),10);
        else
            e.EActor->Timeout = 3000 / max((100 + e.EActor->Attr[A_SPD_BRAWL]*5),10);
        if (startedAfraid)
            e.EActor->Timeout *= 2;

        return DONE;      
    case A_DISA:
        if (!e.EVictim->EInSlot(SL_WEAPON)) {
            if (!e.isCounterTrip)
                IPrint("You cannot disarm an unarmed creature!");
            return ABORT; 
        }
        if (e.EActor->HasStati(STUCK) && !e.isTelekinetic) {
            if (!e.isCounterTrip)
                IPrint("You can't disarm something while stuck.");
            return ABORT;
        }

        if (e.EActor->AttackMode() == S_MELEE) {
            e.vHit = (int8)e.EActor->Attr[A_HIT_MELEE];
            e.EItem = e.EActor->EInSlot(SL_WEAPON);
        } else
            e.vHit = (int8)e.EActor->Attr[A_HIT_BRAWL];
        e.vDef = (int8)e.EVictim->getDef();
        e.DType = AD_DISA; e.saveDC = -1;
        e.Dmg.Set(0,0,0);

        if (!HasFeat(FT_MASTER_DISARM))
            e.EActor->ProvokeAoO(e.EVictim);

        if (e.EActor->isDead())
            return DONE;        

        if (ReThrow(EV_STRIKE,e) == ABORT)
            return ABORT;

        if ((e.Resist || e.Immune) && !e.isCounterTrip)
            if (e.EActor->HasMFlag(M_HUMANOID) && !e.EVictim->HasMFlag(M_NOLIMBS)) {
                EventInfo e2;
                e2.Clear();
                e2 = e;
                e2.EVictim = e.EActor;
                e2.EActor =  e.EVictim;
                e2.isCounterTrip = true;
                ReThrow(EV_SATTACK, e2);
            }

        if (e.isCounterTrip)
            return DONE;

        if (e.EActor->AttackMode() == S_MELEE) 
            e.EActor->Timeout += 3000 / max((100 + e.EActor->Attr[A_SPD_MELEE]*5),10);
        else
            e.EActor->Timeout += 3000 / max((100 + e.EActor->Attr[A_SPD_BRAWL]*5),10);
        if (startedAfraid)
            e.EActor->Timeout *= 2;
        return DONE;      
    case A_GRAB:
        if (e.EActor->InSlot(SL_WEAPON) && e.EActor->InSlot(SL_READY) && !e.isTelekinetic) {
            IPrint("You need a free hand to initiate a grapple.");
            return ABORT;
        }
        if (e.EActor->HasStati(GRAPPLING) && !e.isTelekinetic) {
            IPrint("You can only grapple one target at once.");
            return ABORT;
        } else if (e.EVictim->HasMFlag(M_SWARM)) {
            IPrint("You can't grapple a swarm!");
            break;
        } else if (e.EActor->HasMFlag(M_SWARM)) {
            IPrint("As a swarm, you can't grapple anything!");
            break;
        }
        if (e.EActor->HasMFlag(M_NOHANDS) && !e.isTelekinetic && !e.EActor->HasFeat(FT_NATURAL_GRAB)) {
            IPrint("You can't initiate grapples in this form.");
            return ABORT;
        } else if (e.EActor->onPlane() != e.EVictim->onPlane() && !e.isTelekinetic) {
            IPrint("You cannot grapple a creature on another plane!"); 
            break; 
        } 
        e.vHit = (int8)e.EActor->Attr[A_HIT_BRAWL];
        e.vDef = (int8)e.EVictim->getDef();
        e.DType = AD_GRAB; e.saveDC = -1;
        e.Dmg.Set(0,0,0);
        if (!e.EActor->HasFeat(FT_MASTER_GRAPPLE) && !e.EActor->HasFeat(FT_NATURAL_GRAB))
            e.EActor->ProvokeAoO(e.EVictim);
        if (e.EActor->isDead())
            return DONE;
        if (ReThrow(EV_STRIKE,e) == ABORT)
            return ABORT;

        if (!e.isHit)
            e.EActor->Timeout += 3000 / max((100 + e.EActor->Attr[A_SPD_BRAWL]*5),10);
        else
            e.EActor->Timeout += 30;
        if (startedAfraid)
            e.EActor->Timeout *= 2;
        return DONE;
    case A_ESCA:
        if (e.ETarget = GetStatiObj(GRAPPLING)) 
            if (yn(XPrint("Release the <Obj>?",e.EVictim),true)) {
                e.EVictim->RemoveStati(GRAPPLED,-1,-1,-1,this);
                e.EVictim->RemoveStati(GRABBED,-1,-1,-1,this);
                RemoveStati(GRAPPLING,-1,-1,-1,e.EVictim);
                if (!HasStati(GRAPPLED) && !HasStati(GRABBED))
                    return DONE;
            }

        if (!HasStati(GRAPPLED) && !HasStati(GRABBED)) {
            IPrint("You aren't being grappled presently.");
            return DONE;
        }

        if (GetAttr(A_MOV) <= -20 && HasMFlag(M_NOLIMBS)) {
            IPrint("You can't break grapples in this form.");
            return DONE;
        }

        {
            int checkStati[] = { GRAPPLED, GRABBED, 0 };
            Creature *Grapplers[64];
            int16 j;
            memset(Grapplers,0,sizeof(Creature*)*64);
            j = 0;
            for (i=0;checkStati[i];i++) {
                StatiIterNature(this,checkStati[i])
                    for (j=0;Grapplers[j];j++)
                        if (Grapplers[j] == oCreature(S->h))
                            goto SkipRepeat;
                    Grapplers[j] = oCreature(S->h);
SkipRepeat:;
                StatiIterEnd(this)
            }
            int n; n=0;
            for (i=0;Grapplers[i];i++) {
                e.EVictim = Grapplers[i];
                e.isHit = true;
                e.Resist = false;
                if (ManeuverCheck(e)) {
                    e.Resist = false;
                    RemoveStati(GRAPPLED,-1,-1,-1,e.EVictim);
                    RemoveStati(GRABBED,-1,-1,-1,e.EVictim);
                    e.EVictim->RemoveStati(GRAPPLING,-1,-1,-1,this);
                    /* Delay the grappler a bit, to give the escapee time
                    to run at least one square away before the next
                    attempt. */
                    e.EVictim->Timeout += 15;
                } else
                    e.Resist = true;
                ReThrow(EV_ATTACKMSG,e);
                if (e.Resist) 
                    e.EActor->Timeout += 30;
                if (n++ > 250)
                    Error("Infinite grapple check loop error!");
            } 
        }
        /* Shouldn't successfully breaking out of a grapple take time?
        Logically, yes, but in practice it doesn't because if it did
        the enemy would simply grapple you again on its turn, making
        any attempt to break a grapple practically useless. */
        return DONE;
    case A_KICK:
        if (HasAttk(A_KICK) && e.ETarget->isCreature())
            return ReThrow(EV_NATTACK,e);
        // ww: also check for horses and whatnot: I polymorphed into a
        // horse and was told I couldn't kick ... :-)
        if (!HasMFlag(M_HUMANOID) && !isMType(MA_QUADRUPED) && !e.isTelekinetic) {
            IPrint("You can't kick effectively in this form."); 
            return ABORT;
        }
        e.vHit = (int8)e.EActor->Attr[A_HIT_BRAWL];
        e.vDef = e.ETarget->isCreature() ? e.EVictim->getDef() : 0;
        e.DType = AD_BLUNT;
        e.saveDC = -1;
        e.Dmg = DmgVal(S_BRAWL, (!e.ETarget->isCreature()) || 
            (e.EVictim->GetAttr(A_SIZ) > SZ_MEDIUM));
        e.Dmg.Bonus += max(0,(Mod(A_STR)+1)/2);
        if (ReThrow(EV_STRIKE,e) == ABORT)
            return ABORT;
        // wow, I don't know what this massive timeout is about, but here's
        // what it ends up doing: you kick open a door (happens now) and
        // the monsters on the other side get 5000 units to beat you into
        // submission while you can't react! 
        if (e.ETarget->isCreature())
            Timeout += 5000 / 
            max((100 + Attr[A_SPD_BRAWL]*5),10);
        else
            Timeout += 1000 / 
            max((100 + Attr[A_SPD_BRAWL]*5),10);
        if (startedAfraid)
            e.EActor->Timeout *= 2;
        break;
    case A_COUP:
        bool kill_flag; kill_flag = false;
        if (!EInSlot(SL_WEAPON) || !EInSlot(SL_WEAPON)->isType(T_WEAPON)) {
            if ((HasAbility(CA_UNARMED_STRIKE) || HasFeat(FT_MARTIAL_MASTERY)) || HasStati(POLYMORPH) || !isPlayer())
                ;
            else {
                IPrint("You need to wield a melee weapon to perform a coup de grace.");
                return ABORT;
            }
        }  
        if (e.EVictim->HasStati(SLEEPING) || 
            (e.EVictim->HasStati(PARALYSIS,PARA_HELD) && !(max(e.EVictim->SkillLevel(SK_CONCENT),e.EVictim->SkillLevel(SK_ESCAPE_ART)) >= 15)) ||
            (e.EVictim->HasStati(STUCK) && e.EVictim->HasStati(PRONE) && e.EActor->SkillLevel(SK_FIND_WEAKNESS) >= 20))
            ; // OK, victim cannot defend
        else {
            IPrint("You can only perform a coup de grace against a helpless creature.");
            return ABORT;
        }
        if (!e.EVictim->isMType(MA_LIVING) || (!e.EActor->HasFeat(FT_NECROPHYSIOLOGY) && e.EVictim->isMType(MA_UNDEAD))) {
            IPrint("Unliving creatures cannot be coup de graced.");
            return ABORT;
        }

        /* Coup de Grace provokes an attack of opportunity. */
        e.EActor->ProvokeAoO();
        if (e.EActor->isDead())
            return DONE;

        e.EItem = EInSlot(SL_WEAPON);
        if (e.EItem) {
            e.Dmg   = DmgVal(S_MELEE, e.EVictim->GetAttr(A_SIZ) > SZ_MEDIUM);
            e.vHit  = (int8)e.EActor->Attr[A_HIT_MELEE];
            e.DType    = (int8)e.EItem->DamageType(e.EVictim);
            e.vThreat = (int8)e.EItem->Threat(e.EActor);
            e.vCrit   = (int8)e.EItem->CritMult(e.EActor);
        } else { 
            TAttack buf[1024];
            TAttack * at = NULL;
            int max = ListAttacks(buf,1024);
            for (i=0;i<max;i++) {
                at = &buf[i]; 
                if (is_standard_attk(at->AType))
                    break; 
            } 
            if (!at) {
                IPrint("You have no natural attack suitable for a coup de grace.");
                return ABORT;
            } 
            e.EItem     = NULL;
            e.vHit      = (int8)Attr[A_HIT_BRAWL];
            e.Dmg       = at->u.a.Dmg;
            e.Dmg.Bonus += e.EActor->Attr[A_DMG_BRAWL]; 
            e.saveDC = (int8)e.EActor->GetPower(at->u.a.DC);
            e.DType   = at->DType;
            e.vThreat = 20; /* HasFeat(FT_IMPROVED_CRITICAL_BITE), etc. */
            e.vCrit   = 2;
            e.isNAttack = true;
        } 
        e.vDef  = (int8)e.EVictim->getDef();
        if (e.EVictim->ResistLevel(AD_CRIT) != -1 || (e.EVictim->isMType(MA_UNDEAD) && e.EActor->HasFeat(FT_NECROPHYSIOLOGY)))
            e.isCrit = true; 
        e.isHit = true;
        if (ReThrow(EV_STRIKE, e) == ABORT)
            return ABORT;
        if (!(e.EVictim->isDead()))
            if (e.EVictim->ResistLevel(AD_CRIT) != -1)
                if (!e.EVictim->SavingThrow(FORT,15 + e.EActor->Mod(A_DEX) + e.EActor->SkillLevel(SK_FIND_WEAKNESS)/2,SA_DEATH)) {
                    ReThrow(EV_DEATH,e);
                    kill_flag = true;
                }
        if (!(e.EVictim->isDead()) && !kill_flag)
            VPrint(e,"Miraculously, you survive.",
            "Miraculously, the <EVictim> is still alive.");    
        Timeout += 50; /* a "full round action", in the base system */
        if (startedAfraid)
            e.EActor->Timeout *= 2;
        return DONE;
    case A_THRO:
        if (!HasStati(GRAPPLING,-1,e.EVictim)) {
            IPrint("You can only throw a creature you have already grappled.");
            return ABORT;
        }
        if (e.EActor->HasStati(STUCK)) {
            IPrint("You can't throw a creature while stuck.");
            return ABORT;
        }

        /* Succeed or fail, end the grapple */
        e.EVictim->RemoveStati(GRABBED,SS_ATTK,-1,-1,e.EActor);
        e.EVictim->RemoveStati(GRAPPLED,SS_ATTK,-1,-1,e.EActor);
        e.EActor->RemoveStati(GRAPPLING,SS_ATTK,-1,-1,e.EVictim);

        /* Held by someone else? */
        if (e.EVictim->HasStati(GRABBED) || e.EVictim->HasStati(GRAPPLING))
            return ABORT;

        /* The usual grappling AoO */
        e.EActor->ProvokeAoO();        
        if (e.EActor->isDead())
            return DONE;

        e.vHit = (int8)e.EActor->Attr[A_HIT_BRAWL];
        e.vDef = e.ETarget->isCreature() ? e.EVictim->getDef() : 0;
        e.DType = AD_KNOC; e.saveDC = -1;
        e.Dmg.Set(1,max(1,e.EActor->Mod(A_STR) + 1),0);

        if (ReThrow(EV_STRIKE,e) == ABORT)
            return ABORT;

        e.EActor->Timeout += 3000 / max((100 + e.EActor->Attr[A_SPD_BRAWL]*5),10);
        if (startedAfraid)
            e.EActor->Timeout *= 2;
        return DONE;
    case A_WHIR:  
        bool found; found = false;
        if (!e.EActor->HasFeat(FT_WHIRLWIND_ATTACK))
            return ABORT;
        if (e.EActor->HasStati(STUCK)) {
            IPrint("You can't execute a whirlwind of blows while stuck.");
            return ABORT;
        }
        for(i=0;i!=8;i++) {
            /* Clear away the flags from the last attack. */
            e.Clear();
            e.EActor = this;
            for (e.EVictim=m->FCreatureAt(e.EActor->x+DirX[i],e.EActor->y+DirY[i]);e.EVictim;
                    e.EVictim=m->NCreatureAt(e.EActor->x+DirX[i],e.EActor->y+DirY[i])) {
                if (!found) {
                    DPrint(e,"You execute a great flurry of attacks!",
                        "The <EActor> executes a great flurry of attacks!");
                    found = true;
                }            
                if (!EInSlot(SL_WEAPON)) {
                    if (!TMON(mID)->Attk[0].AType) {
                        IPrint("You have no active method of attack in this form.");
                        return ABORT;
                    }
                    e.EItem   = NULL;
                    e.vHit    = (int8)Attr[A_HIT_BRAWL];
                    e.vDef    = (int8)e.EVictim->getDef();
                    e.Dmg     = TMON(mID)->Attk[0].u.a.Dmg;
                    e.Dmg.Bonus += e.EActor->Attr[A_DMG_BRAWL]; 
                    e.AType   = TMON(mID)->Attk[0].AType;
                    e.DType   = TMON(mID)->Attk[0].DType;
                    e.vThreat = 20;
                    e.vCrit   = 2;
                } else {
                    e.EItem    = EInSlot(SL_WEAPON);
                    e.Dmg      = e.EActor->DmgVal(S_MELEE, 
                        e.EVictim->GetAttr(A_SIZ) > SZ_MEDIUM);
                    e.vHit     = (int8)Attr[A_HIT_MELEE]; /*+ e.EItem->ToHit() */
                    e.vDef     = (int8)e.EVictim->getDef();
                    e.AType    = A_SWNG;
                    e.DType    = (int8)e.EItem->DamageType(e.EVictim);
                    e.vThreat  = (int8)e.EItem->Threat(e.EActor);
                    e.vCrit    = (int8)e.EItem->CritMult(e.EActor);
                }
                if (ReThrow(EV_STRIKE, e) == ABORT)
                    continue;
                if (e.EActor->HasStati(STUNNED) || e.EActor->HasStati(PARALYSIS) ||
                    e.EActor->isDead() || (e.EActor->HasStati(AFRAID) && 
                    !e.EActor->HasFeat(FT_LION_HEART)))
                    break;   
            }
        }
        if (!found) {
            IPrint("But there's nobody nearby!");
            return ABORT;
        }
        e.EActor->Timeout += 5000 / max((100 + e.EActor->Attr[EInSlot(SL_WEAPON) ? A_SPD_MELEE : A_SPD_BRAWL]*5),10);
        if (startedAfraid)
            e.EActor->Timeout *= 2;
        break;  
    case A_SPRI:
        if (HasStati(GRAPPLED) || HasStati(GRABBED)) {
            IPrint("You can't spring attack while ensnared.");
            return ABORT;
        }
        if (HasStati(GRAPPLING)) {
            IPrint("You can't spring attack while grappling.");
            return ABORT;
        }
        if (HasStati(CONFUSED)) {
            IPrint("You can't spring attack while confused.");
            return ABORT;
        }
        if (HasStati(STUCK)) {
            IPrint("You can't spring attack while stuck.");
            return ABORT;
        }
        if (dist(e.EActor->x,e.EActor->y,e.EVictim->x,e.EVictim->y) > 4) {
            IPrint("The <Obj> is too far away to Spring Attack!");
            return ABORT;
        }
        /* Later, we should use EV_PUSH to trigger traps, hit barriers,
        etc. in the path to the monster and back. For now, we just 
        ignore these aspects. */
        switch (AttackMode()) {
        case S_MELEE:
        case S_DUAL:
            return ReThrow(EV_WATTACK,e);
        case S_BRAWL:
            return ReThrow(EV_NATTACK,e);
        default:
            IPrint("You can't Spring Attack with a ranged weapon!");
        }
        break;    
    case A_DEFN:
        if (HasStati(DEFENSIVE)) {
            IPrint("You're already fighting defensively.");
            return ABORT;
        }
        IDPrint("You go on the defensive.",
            "The <Obj> goes on the defensive.",this);
        GainPermStati(DEFENSIVE,NULL,SS_MISC,0,0,0);
        break;         
    case A_AEYE:
        if (HasStati(EYES_AVERTED)) {
            IPrint("You're already averting your eyes.");
            return ABORT;
        }
        IDPrint("You avert your eyes.",
            "The <Obj> averts its eyes.",this);
        GainPermStati(EYES_AVERTED,NULL,SS_MISC,0,0,0);
        break;         
    case A_SUND:
        if (!e.EVictim->EInSlot(SL_WEAPON)) {
            IPrint("That foe has no weapon to sunder.");
            return ABORT;
        }
        if (e.EActor->HasStati(STUCK)) {
            IPrint("You can't sunder a weapon while stuck.");
            return ABORT;
        }
        if (!EInSlot(SL_WEAPON)) {
            IPrint("You need a weapon yourself to sunder an enemy's.");
            return ABORT;
        }
        if (!e.EVictim->EInSlot(SL_WEAPON)) {
            IPrint("That creature has no weapon readied to sunder.");
            return ABORT;
        }
        // ww: this was forgotten! 
        if (!HasFeat(FT_SUNDER)) {
            e.EActor->ProvokeAoO(e.EVictim);
            if (e.EActor->isDead()) return ABORT; 
        }
        e.vHit    = (int8)(e.EActor->Attr[A_HIT_MELEE] - 4);
        e.vDef    = (int8)e.EVictim->getDef();
        e.saveDC  = -1;
        e.EItem   = EInSlot(SL_WEAPON);
        e.EItem2  = e.EVictim->EInSlot(SL_WEAPON);
        e.Dmg     = e.EActor->DmgVal(S_MELEE, e.EVictim->GetAttr(A_SIZ) > SZ_MEDIUM);
        e.DType   = (int8)e.EItem->DamageType(e.EVictim); 
        e.vThreat = (int8)e.EItem->Threat(e.EActor);
        e.vCrit   = (int8)e.EItem->CritMult(e.EActor);
        if (ReThrow(EV_STRIKE,e) == ABORT)
            return ABORT;
        Timeout += 3000 / max((100 + Attr[A_SPD_MELEE]*5),10);
        if (startedAfraid)
            e.EActor->Timeout *= 2;
        break;
    case A_SPRT:
        if (HasStati(SPRINTING)) {
            IPrint("You're already sprinting.");
            return ABORT;
        }
        if (HasStati(STUCK)) {
            IPrint("You can't run while stuck.");
            return ABORT;
        }
        if (HasStati(GRABBED) || HasStati(GRAPPLING) || HasStati(GRAPPLED)) {
            IPrint("You can't run while grappling.");
            return ABORT;
        }
        if (HasStati(LEVITATION)) {
            IPrint("You can't sprint while levitating.");
            return ABORT;
        }
        if (HasStati(STUNNED)) {
            IPrint("You can't sprint while stunned.");
            return ABORT;
        }
        if (HasStati(CONFUSED)) {
            IPrint("You can't sprint while confused.");
            return ABORT;
        }
        if (GetAttr(A_MOV) <= -20) {
            IPrint("You have no active locomotion in this form.");
            return ABORT;
        }
        if (!LoseFatigue(1,true))
            return ABORT;
        GainTempStati(SPRINTING,NULL,1 + max(0,Mod(A_CON)),SS_MISC);
        IDPrint("You push yourself to the limit!",
            "The <Obj> puts on a burst of speed!",this);
        return DONE;    
    default:
        if (e.AType <= A_LAST_STANDARD) {
            e.DType = ta->DType;
            e.saveDC = ta->u.a.DC;
            e.Dmg = ta->u.a.Dmg;
            e.vHit    = (int8)e.EActor->GetAttr(A_HIT_BRAWL);
            e.vDef    = (int8)e.EVictim->getDef();
            e.vThreat = 20;
            e.vCrit   = 2;
            return ReThrow(EV_STRIKE,e);
        }
        IPrint(Format("Unimplemented special attack %d!",e.AType));
        return ABORT;
    }
    return DONE;  
}


// doesn't handle Flat-Footed, unseen attacker, surprise round, etc. 
bool Creature::noDexDefense()
{
  if (HasStati(ENGULFED))
    return true;
  if (HasStati(ELEVATED,ELEV_CEILING))
    return true;
  if (HasStati(BLIND) && !HasFeat(FT_BLIND_FIGHT)) 
    return true;
  else if (GetStatiMag(CHARGING) > 0) {
    if (HasStati(MOUNTED) && HasFeat(FT_RIDE_BY_ATTACK))
      ; 
    else if (HasFeat(FT_RUN) && !HasStati(MOUNTED))
      ; 
    else 
      return true; 
    } 
  int badStati[] = { 
    ASLEEP, EXPOSED, GRABBED, GRAPPLED, GRAPPLING,
    NAUSEA, PARALYSIS, PRONE, STUCK, STUNNED, 0 }; 
  for (int i=0; badStati[i]; i++)
    if (HasStati(badStati[i]))
      return true; 
  if (HighStatiMag(DISTRACTED) >= 10)
    return true;
  
  if (x > 0 && y > 0 && m && GetAttr(A_SIZ) >= TREG(m->RegionAt(x,y))->Size)
    return true;
  return false; 
}

bool Creature::canMakeAoO(Creature * victim)
{
  /* Some Stati prevent you from executing attacks of opportunity */
  if (
      HasStati(AFRAID) || 
      HasStati(CHARGING) ||
      HasStati(CHARMED,-1,victim) || 
      HasStati(CONFUSED) || 
      HasStati(GRAPPLED) || 
      HasStati(GRAPPLING) ||
      HasStati(NAUSEA) ||
      HasStati(PARALYSIS) ||
      HasStati(SLEEPING) ||
      HasStati(ENGULFED) || 
      HasStati(STUCK) || 
      HasStati(STUNNED) || 
      HasMFlag(M_SWARM) || // WW: SRD
      ts.hasTargetOfType(OrderDoNotAttack) ||
      (HasStati(BLIND) && !HasMFlag(M_BLIND) && !HasFeat(FT_BLIND_FIGHT))
    )
      return false;
  return true;
} 

EvReturn Creature::OAttack(EventInfo &e) 
  {
    int16 i; TAttack *at = NULL;

    if (!e.ETarget || !e.ETarget->isCreature())
      return ABORT; 

    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
      e.EItem = NULL;

    if (!canMakeAoO(e.EVictim))
      return ABORT; 

    /* ww: are stuck in no-attack webbing or somesuch? */
    if (!m->InBounds(x,y) || 
        TTER(m->TerrainAt(x,y))->Event(e,m->TerrainAt(x,y)) == ABORT)
      return ABORT; 

    if (e.EActor->HasStati(AFRAID))
      return ABORT;

    if (!attackSanity(e))
      return ABORT;

    if (isPlayer() && (HasStati(HIDING) || HasStati(INVIS)))
      if (!yn(XPrint("Take opportunity on the <Obj>?",e.EVictim)))
        return ABORT;

    if (e.EVictim->HasStati(GRABBED,0,e.EActor))
      if (e.EActor->HasStati(GRAPPLING))
        e.vicHeld = true;

    e.isAoO = true; /* Attack of Opportunity flag */
    e.EActor->AoO = max(0,e.EActor->AoO - 1);

    if (e.EVictim && e.EVictim->isCreature())
      if (!e.EVictim->HasStati(AFRAID) ||
          e.EVictim->GetStatiVal(AFRAID) == FEAR_SKIRMISH)
        e.vicNotFleeing = true; 
    if (e.EVictim && e.EVictim->isCreature())
      if (!(e.EVictim->HasStati(PRONE) ||
            e.EVictim->HasStati(BLIND) ||
            e.EVictim->HasStati(PARALYSIS) ||
            e.EVictim->HasStati(STUCK)))
        e.vicReady = true;

    if (!EInSlot(SL_WEAPON))
      {
        if (!TMON(mID)->Attk[0].AType)
          return ABORT;

        e.EItem   = NULL;
        e.vHit    = (int8)Attr[A_HIT_BRAWL];
        e.vDef    = (int8)e.EVictim->getDef();
        e.AType   = 0;
        e.isNAttack = true;
        e.vThreat = 20;
        e.vCrit   = 2;

        TAttack buf[1024];
        int max = ListAttacks(buf,1024);
        for (i=0;i<max;i++) {
          at = &buf[i];
          if (is_standard_attk(at->AType))
            {
              e.Dmg     = at->u.a.Dmg;
              e.AType   = at->AType;
              e.DType   = at->DType;
              int res = ReThrow(EV_STRIKE, e);
              return res;
            }
        }
      }
    else
      {
        e.EItem    = EInSlot(SL_WEAPON);
        e.Dmg      = e.EActor->DmgVal(S_MELEE, 
            e.EVictim->GetAttr(A_SIZ) > SZ_MEDIUM);
        e.vHit     = (int8)Attr[A_HIT_MELEE]; /*+ e.EItem->ToHit() */
        e.vDef     = (int8)e.EVictim->getDef();
        e.AType    = A_SWNG;
        e.DType    = (int8)e.EItem->DamageType(e.EVictim);
        e.vThreat  = (int8)e.EItem->Threat(e.EActor);
        e.vCrit    = (int8)e.EItem->CritMult(e.EActor);
        Item * it = EInSlot(SL_WEAPON); 
       	if (ReThrow(EV_STRIKE, e) == ABORT)
          return ABORT;
      }

    return DONE;
  }

bool Creature::isThreatened(bool perceived_only)
  {
    Creature *cr; int32 i;
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->DistFrom(this) <= 16)
        if (cr->isHostileTo(this) && 
            ((!perceived_only) || Perceives(cr)))
          if (m->LineOfFire(cr->x,cr->y,x,y,cr))
            return true;
    return false;
  }

void Creature::ProvokeAoO(Creature *c, bool from_move)
{
  if (isDead()) return;

  /* Later, this should only work in areas where the creature
     has room to fly around -- vast chambers one or more size
     categories larger than the creature, and outdoors. */
  bool needFlight = HasFeat(FT_FLYBY_ATTACK) && isAerial();

  if (c) { 
    /* AoO for C only! */

    /* This function is called when we:
     * Attempt disarm without Master Disarm
     * Attempt bull rush without Master Bull Rush
     * Are tripped by someone with Master Trip
     * Strike unarmed without Martial Mastery
     * Try to close with someone who has reach
     */
    if (c->AoO && (!needFlight || c->isAerial())) {
      if (!c->isBeside(this,c->StateFlags & MS_HAS_REACH ? 1 : 0))
        return;
      EventInfo e;
      SetEvent(e,EV_OATTACK,c,this);
      e.isMoveAoO = from_move;
      int16 res = ReThrow(EV_OATTACK,e);
      /*
         if (res != ABORT) 
         c->IPrint("You make an attack of opportunity on an <Obj>.",this);
       */
    } else c->IPrint("You miss an opportunity on an <Obj>.",this);

    return; 
  }

  int16 i; ;
  /* This function is called when we do any of the following:
   * cast in melee
   * fire a ranged weapon in melee
   * flee someone's threatened area
   */
  int8 ReachX[] =
  { 0, 1, 2, 2, 2, 2, 2, 1, 0,-1,-2,-2,-2,-2,-2,-1 };
  int8 ReachY[] =
  { 2, 2, 2, 1, 0,-1,-2,-2,-2,-2,-2,-1, 0, 1, 2, 2 };

  for (i=0;i!=8 && !isDead();i++)
    if (m->InBounds(x+DirX[i],y+DirY[i]))
      for (c=m->FCreatureAt(x+DirX[i],y+DirY[i]);c;c=m->NCreatureAt(x+DirX[i],y+DirY[i])) 
        if (c->isHostileTo(this) || this->isHostileTo(c))
          if ((!needFlight) || c->isAerial())
            if (c->Perceives(this))
              if (!(c->StateFlags & MS_REACH_ONLY)) {
                if (c->AoO)  {
                  EventInfo e;
                  SetEvent(e,EV_OATTACK,c,this);
                  e.isMoveAoO = from_move;
                  ReThrow(EV_OATTACK,e); 
                  if (isDead() || !m)
                    break;
                } else
                  c->IPrint("You miss an opportunity on an <Obj>.",this);
              }

  for(i=0;i!=16 && !isDead();i++)
    if (m->InBounds(x+ReachX[i],y+ReachY[i]))
      for (c=m->FCreatureAt(x+ReachX[i],y+ReachY[i]);c;c=m->NCreatureAt(x+ReachX[i],y+ReachY[i])) 
        if ((c->StateFlags & MS_HAS_REACH) && c->isHostileTo(this)) 
          if ((!needFlight) || c->isAerial())
            if (c->Perceives(this))
              if (m->LineOfFire(x,y,c->x,c->y,this)) {
                if (c->AoO) {
                  EventInfo e;
                  SetEvent(e,EV_OATTACK,c,this);
                  e.isMoveAoO = from_move;
                  ReThrow(EV_OATTACK,e); 
                  if (isDead() || !m)
                    break;
                } else
                  c->IPrint("You miss an opportunity on an <Obj>.",this);
              }
  }



bool Creature::canChargeInDir(Dir d)
  {
    Creature *cr; int32 i; Dir d2;
    if (!m || isDead() || x == -1)
      return false;
    MapIterate(m,cr,i)
      if (cr->isCreature() && DistFrom(cr) <= 25) {
        d2 = DirTo(cr);
        /* If there's an enemy to your southeast, you can charge south,
           east or southeast. If there's an enemy to your north, you
           can charge north, northeast or northwest. */
        if (isSimilarDir(d,d2))
          if (cr->isHostileTo(this) && cr != this)
            if (Perceives(cr) && m->LineOfFire(x,y,cr->x,cr->y,this))
              return true;
        }
    return false;
  }
              
void Creature::DoGazeAttack()
  {
    Creature *c; int16 i;
    if (!m || isDead()) return;
    TAttack *at;
    at = GetAttk(A_GAZE);
    if (!at)
      return;
    if (isPlayer())
      if (!theGame->Opt(OPT_USE_GAZE))
        return;
    MapIterate(m,c,i) {
      if (!m || isDead()) return;
      if (c->isCreature())
        if (c != this)
          if (dist(c->x,c->y,x,y) <= 6)
            if (isHostileTo(c) && !c->isBlind())
              if (Perceives(c) &&
                  (c->Perceives(this) & (PER_VISUAL|PER_INFRA))) {
                if (cMana() < 2)
                  return;
                if (c->HasStati(CHARMED,-1,this))
                  continue;
                if (c->HasStati(STONING) && at->DType == AD_STON)
                  continue;
                if (c->HasStati(CONFUSED) && at->DType == AD_CONF)
                  continue;
                if (c->HasStati(STUNNED) && at->DType == AD_STUN)
                  continue;
                if (c->HasStati(PARALYSIS) && at->DType == AD_PLYS)
                  continue;
                ThrowVal(EV_SATTACK,A_GAZE,this,c);
              }
    }
  }

void Creature::DoProxAttack()
  {
    Creature *c; int16 i;
    if (!m || isDead()) return;
    MapIterate(m,c,i)
      if (c->isCreature())
        if (c != this && c->tmID != tmID)
          if (isBeside(c)) {
            ThrowVal(EV_SATTACK,A_PROX,this,c);
            if ( (isHostileTo(c) || c->isHostileTo(this)) &&
                 Perceives(c) ) {
              ThrowVal(EV_SATTACK,A_CPRX,this,c);
            }
          }
  }

EvReturn Creature::HandleNegate(EventInfo &e)
  {
    if (e.Event == EV_SATTACK)
      if (e.AType != A_BREA && e.AType != A_EXPL &&
	  		  e.AType != A_ROAR && e.AType != A_GEST &&
		  		e.AType != A_CPRX && e.AType != A_PROX &&
			  	e.AType != A_GAZE && e.AType != A_GATE &&
				  e.AType != A_AURA && e.AType != A_DEQU &&
				  e.AType != A_BRE2)
			  return NOTHING;
		if (e.Event == EV_DAMAGE)
		  {
		    if (e.DType <= AD_LAST_PHYSICAL ||
		        e.DType == AD_SUBD || e.DType == AD_WRAP)
		      return NOTHING;
		    if (!e.isNAttack)
		      return NOTHING;
		  }
    
    if (GetStatiVal(NEGATED))
      {
        // We use the NEGATED Stati val as a flag for a
        // monster to SEE if it knows it has been negated
        // If it does, return abort so it will choose a
        // different action; otherwise, consider the 
        // action wasted.
        return ABORT;
      }
    if (isPlayer())
      IPrint("Nothing happens.");
    else 
      SetStatiVal(NEGATED,NULL,1);    
      
    switch (e.Event)
      {
        case EV_SATTACK:
          switch (e.AType)
            {
              case A_GAZE: return DONE;
              case A_PROX: return DONE;
              
              case A_BREA: case A_SPIT: case A_BRE2:
                TPrint(e,"You try to <Str1>, but nothing comes forth!",
                         "The <EActor> tries to <Str1>, but nothing comes forth!",
                         e.AType == A_SPIT ? "spit" : "breathe");
                Timeout += 30;
                return DONE;
              case A_ROAR: 
                String *verb;
                verb = TMON(e.EActor->tmID)->GetMessages(MSG_ROARVERB);
                if (verb[0].GetLength()) 
                  DPrint(e,"You <Str1>, but it seems purely mundane.",
                           "The <EActor> <Str2>, but it seems ineffectual.",
                           (const char*)(verb[0]), (const char*)(verb[1]));
                else
                  DPrint(e,"You roar, but it seems purely mundane.",
                           "The <EActor> roars, but it seems ineffectual.");
                Timeout += 30;
                return DONE;
              case A_GEST:
                DPrint(e,"You gesture, but it seems purely mundane.",
                         "The <EActor> gestures, but it seems ineffectual.");
                Timeout += 30;
                return DONE;
            }
        case EV_DAMAGE:
          return DONE;
        case EV_INVOKE:
          if (!TEFF(e.eID)->HasFlag(EF_SPECABIL))
            IDPrint("You concentrate, but nothing happens.",
                    "An <Obj> concentrates.",this);
          /* Should show MSG_NEGATED for things like a vrock's
               dance and such. */
          Timeout += 20;
         break;         
      }
    return NOTHING;
  }     

EvReturn Creature::PreStrike(EventInfo &e) /* this == EActor */
	{
    int16 swarm, walls, i, v; 
    Creature *c; Item *it;
    
    if (e.EActor->isDead())
      return ABORT;
    if (e.EActor->isIllusion())
      if (e.EVictim->HasStati(ASLEEP,SLEEP_PSYCH))
        return ABORT;
      
    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
      e.EItem = NULL;
      
    if (e.EVictim->isDead())
      return ABORT;
    else if (!e.ETarget->isCreature())
      return NOTHING;
    else if (e.vHit)
      e.strHit = Format("%+d",(e.vHit));
    else
      e.strHit = "";
    e.strDef = Format("%d",e.vDef);
    e.strDmg = "";
    e.strXDmg = "";
    e.xDmg = 0;
    e.vRideCheck = 0;
    e.isHit = false;
    e.isBypass = false;
    e.Absorb = false;
    e.isFlanking = false;
    e.actIncor = false;
    e.actUnseen = false;
    e.vicIncor = false;
    e.vicUnseen = false;
    e.isGhostTouch = false;
    e.isSeeking = false; 
    e.isSurprise = false;
    e.isWImmune = false;
    e.wasFriendly = !e.EVictim->isHostileToPartyOf(e.EActor);

    e.vicIncor = e.EVictim->onPlane() != PHASE_MATERIAL &&
                 e.EVictim->onPlane() != e.EActor->onPlane();
    e.actIncor = e.EActor->onPlane() != PHASE_MATERIAL &&
                 e.EActor->onPlane() != e.EVictim->onPlane();
    if ((e.EItem && e.EItem->isGhostTouch() && !e.EItem2) ||
        (e.EItem2 && e.EItem2->isGhostTouch())) 
      e.isGhostTouch = true;
      

    if ((e.EItem && e.EItem->HasQuality(WQ_SEEKING)) ||
        (e.EItem2 && e.EItem2->HasQuality(WQ_SEEKING))) 
      e.isSeeking = true; 
    
    e.modStr = (int8)e.EActor->Mod(A_STR); 
    if (e.EItem && (e.AType == A_SWNG || e.AType == A_FIRE ||
                     e.AType == A_GREA || e.AType == A_HURL)) {
      if (e.EActor->InSlot(SL_READY) == e.EActor->InSlot(SL_WEAPON) &&
           e.EActor->InSlot(SL_READY) == e.EItem)
        e.modStr += e.EActor->Mod(A_STR) / 2;
      else if (e.EActor->InSlot(SL_READY) == e.EItem &&
               e.EActor->InSlot(SL_WEAPON) != e.EItem &&
               !e.EActor->HasFeat(FT_POWER_DOUBLE_WEAPON))
        e.modStr /= 2;
      }
    e.modStr = max(0,e.modStr);
     
    e.subStr = false;

    if (theGame->isTimeStopped())
      {
        /* Later, check if TIMESTOP Stati has special
           flag allowing fighting... */
        IPrint("You're outside time. You can't hurt them.");
        return ABORT;
      }
      
   if (e.EVictim->isFlatFooted())
      if (!e.EVictim->AbilityLevel(CA_UNCANNY_DODGE))
        {        
          e.isFlatFoot = true; 
          if (e.EVictim->HasFeat(FT_IMPROVED_INITIATIVE))
            if (Dice::Roll(1,20) + e.EVictim->Mod(A_DEX) >= 15)
              e.isFlatFoot = false;
          if (e.EVictim->isPlayer())
            e.EPVictim->statiChanged = true;
        }
    if (e.EActor->isFlatFooted() && e.EActor->isPlayer())
      e.EPActor->statiChanged = true;
    e.EActor->FFCount = 0;
    e.EVictim->FFCount = 0;
    

    // ww: I think is hiding/surprise/unseen logic can get a lot simpler:
    if (!e.EVictim->Perceives(e.EActor) && 
        !e.EVictim->HasFeat(FT_BLIND_FIGHT)) {
      e.actUnseen = true;
      e.isSurprise = true;
      if (e.EActor->HasMFlag(M_HIDE_ABOVE) && 
          !e.EActor->HasStati(CHARGING)) {
        e.EActor->GainPermStati(CHARGING,e.EActor,SS_ATTK,2,2);
        /* already covered elsewhere
        VPrint(e,"The <EActor> charges down at you from above!",
            "The <EActor> charges down at the <EVictim>!");
            */
      }
    }
    if (e.EActor->HasStati(INVIS)) {
      if (!(e.EActor->GetStatiVal(INVIS) == INV_IMPROVED)) 
        RemoveStati(INVIS);
      } 
    StatiIterNature(e.EActor,INVIS_TO)
      if (S->Val != INV_IMPROVED)
        if (e.EVictim->isMType(S->Mag))
          StatiIter_RemoveCurrent(e.EActor)
    StatiIterEnd(e.EActor)
       
    if (!e.EActor->Perceives(e.EVictim) 
        || (e.EActor->HasStati(EYES_AVERTED) && random(100) < 50)
       // && !e.EActor->HasFeat(FT_BLIND_FIGHT)
       // ww: blindfight is handled later
        ) {
      e.vicUnseen = true;
    } 

    if (e.isSurprise && !e.EVictim->HasStati(SLEEPING) &&
        (e.EActor->onPlane() == e.EVictim->onPlane()))
      if (e.EActor->SkillLevel(SK_MOVE_SIL) + Dice::Roll(1,20,0) <
             e.EVictim->SkillLevel(SK_LISTEN) + Dice::Roll(1,20,0)) {
        e.EActor->IPrint("The <Obj> hears you!",e.EVictim);
        e.isSurprise = false;
        }

    if (e.EVictim->HasStati(SLEEPING)) {
      e.isSurprise = true;
      e.EVictim->Awaken();
      }

    if (e.isSurprise || e.EVictim->isFlatFooted())
      if (e.EVictim->HasAbility(CA_PREMONITION))
        if (random(100)+1 <= 50)
          {
            e.EVictim->IPrint("You have a premonition!");
            e.isSurprise = false;
            e.EVictim->FFCount = 0;
            if (e.EVictim->isPlayer())
              e.EPVictim->statiChanged = true;
          }


    if (e.isSurprise) {
      if (it = e.EVictim->EInSlot(SL_WEAPON))
        if (it->HasQuality(WQ_WARNING))
          if (random(100)+1 <= 50)
            {
              e.EVictim->IPrint("Your <Obj> flares!", it);
              if (e.EVictim->isPlayer())
                e.EVictim->IdentByTrial(it,WQ_WARNING);
              e.isSurprise = false;
            }
          else
            goto SkipWarning;

      if (e.EVictim->HasStati(WARNING))
        if (random(100)+1 <= 50)
          {
            it = (Item*) e.EVictim->GetStatiObj(WARNING);
            if (it && it->isItem()) {
              e.EVictim->IPrint("Your <Obj> flares!", it);
              if (e.EVictim->isPlayer())
                e.EVictim->IdentByTrial(it);
              }
            e.isSurprise = false;
          }

      }
    

    SkipWarning:    

    /* This should be a lot more accurate than it is now. */
    if (e.isTelekinetic)
      {
        /* Remove Str/Dex */
        e.vHit -= (e.EActor->HasFeat(FT_WEAPON_FINESSE) && 
                     e.EActor->Mod(A_DEX) > e.EActor->Mod(A_STR)) ?
                     e.EActor->Mod(A_DEX) : e.EActor->Mod(A_STR);
        e.subStr = true;
        /* Later, account for:
           * Str x 1 & 1/2 for two-handed,
           * Str x 1/2 for off-hand,
           * etc.
         */
        
        e.vHit += e.EActor->Mod(A_INT);
        e.strHit += Format(" %+d Int", e.EActor->Mod(A_INT));
        
        e.vDmg += e.EActor->Mod(A_WIS);
        e.strHit += Format(" %+d Wis", e.EActor->Mod(A_WIS));
      }
         
      
        

    if (e.isSurprise)
      {
        e.vHit += 4;
        e.strHit += " +4 surprise";
      }

    if (e.actUnseen || e.isFlatFoot || e.isSurprise || 
        e.EVictim->noDexDefense()) { 
      
      /* This is a kludge -- we should eventually add a
         A_UDC (Unaware DC) -- so that we don't have to
         recalculate all the time. */
      e.EVictim->CalcValues();
      
      
      /* Victim is denied Dex bonus. */
      int loss = e.EVictim->AttrAdj[A_DEF][BONUS_ATTR]; 
      e.vDef -= max(0,loss); 
      if (loss > 0)
        e.strDef += Format(" -%d noDex",loss);

      loss = e.EVictim->AttrAdj[A_DEF][BONUS_DODGE];
      if (e.EVictim->StateFlags & MS_CASTING)
        loss = 0; 
      e.vDef -= max(0,loss); 
      if (loss > 0)
        e.strDef += Format(" -%d noDodge",loss);

      // your rapier will not help you in a headlock
      loss = e.EVictim->AttrAdj[A_DEF][BONUS_WEAPON]; 
      if (e.EVictim->StateFlags & MS_CASTING)
        loss = 0; 
      e.vDef -= max(0,loss); 
      if (loss > 0)
        e.strDef += Format(" -%d noWep",loss);
      } 
    else if (it = e.EVictim->EInSlot(SL_WEAPON))
      if ((e.EItem && e.EItem->HasIFlag(WT_SLASHING)) && 
          !e.EVictim->HasFeat(FT_EXPERTISE)) {
        // ww: again, a +10 sword is better at parrying than a +0 sword
        int parryAmount = it->ParryVal(e.EVictim);
        if (parryAmount)
        {
          // ww: fixed: the parry mod was actually applied twice before ...
          e.strDef += Format(" %+d parry",parryAmount);
          e.vDef += parryAmount; 
        }
      }

    if (e.EActor->isAerial() && (e.AType == A_FIRE || e.AType == A_SPIT))
      if (!e.EActor->HasStati(FT_HOVER))
        {
          e.vHit -= 2;
          e.strHit += " -2 flight";
        }
    
    if (e.isAoO && e.EActor->HasFeat(FT_DEFT_OPPORTUNIST))
      {
        e.vHit += 4;
        e.strHit += " +4 Deft";
      }
    
    
    if (TMON(e.EActor->tmID)->isMType(e.EActor->tmID, MA_HALFLING) && 
          e.AType == A_FIRE && e.EItem->iID == FIND("sling"))
      e.vThreat -= max(0,e.EActor->Mod(A_LUC));
    if (TMON(e.EActor->tmID)->isMType(e.EActor->tmID, MA_HALFLING) && 
          e.AType == A_HURL && e.EItem2->iID == FIND("rock"))
      e.vThreat -= max(0,e.EActor->Mod(A_LUC));  
      
    
    if (e.EItem && e.EItem->HasStati(MASTERWORK))
      {
        e.bDmg += e.EItem->GetStatiVal(MASTERWORK);
        e.strDmg += Format(" %+d MW", e.EItem->GetStatiVal(MASTERWORK));
      }
    if (e.EItem2 && e.EItem2->HasStati(MASTERWORK))
      {
        e.bDmg += e.EItem2->GetStatiVal(MASTERWORK);
        e.strDmg += Format(" %+d MW", e.EItem2->GetStatiVal(MASTERWORK));
      }
    
    if (e.EActor->isMType(MA_GOOD) && e.EVictim->isMType(MA_EVIL))
      {
        int16 alignBonus;
        if (e.EActor->isCharacter())
          alignBonus = (-e.EPActor->alignGE) >= 60 ? 2 :
                       (-e.EPActor->alignGE) >= 40 ? 1 : 0;
        else
          alignBonus = HasMFlag(M_IALIGN) ? 2 : 1;
        if (alignBonus) {
          e.vHit += alignBonus;
          e.strHit += Format(" %+d good",alignBonus);
          }
      }
      
    if (e.EActor->HasStati(AFRAID))
      {
        e.vHit -= 4;
        e.strHit += " -4 fear";
      }

    if (e.AType == A_TUCH) {
      int bonusHit = 0;
      int bonusBless = 0; 

      static int armourSlots[] = {
        SL_ARMOUR, SL_READY, SL_WEAPON, 0
      } ;
      for (int j=0; armourSlots[j]; j++) {
        Item *it = NULL;
        int8 ii = armourSlots[j];
        if ((it = e.EVictim->EInSlot(ii)) && it->activeSlot(ii)) {
          if (it->isBlessed() && (it->isType(T_SHIELD) || it->isType(T_ARMOUR)))
            bonusBless += 2; 
          if (it->isGhostTouch())
              continue;
          else if (it->isType(T_SHIELD)) 
              bonusHit += it->DefVal(e.EVictim,false);
        } 
      }
      if (bonusHit) { 
        e.vDef -= bonusHit;
        e.strDef += Format(" %+d touch",-bonusHit);
      } 
      if (bonusBless && (
          e.EActor->isType(MA_UNDEAD) ||
          e.EActor->isType(MA_DEVIL) ||
          e.EActor->isType(MA_DEMON) ||
          e.EActor->isType(MA_LYCANTHROPE))) { 
        e.vDef += bonusBless;
        e.strDef += Format(" %+d blessed",bonusBless);
      } 
    }

    /* Needed to prevent crashes when evaluating night hag --
       e.EActor can die somewhere above here. */
    if (e.EActor->isDead() || e.EVictim->isDead())
      return ABORT;


    if (e.EActor->isFlanking(e.EVictim)) {
      /* Attacker gets +2 to hit */
      e.isFlanking = true;
      e.vHit += 2;
      e.strHit += " +2 flanking";
      }

    // ww: this ugliness is because it is hard to predict when we'll be
    // making a sneak attack ...
    if (e.EActor->HasFeat(FT_BACKSTAB) && 
        (e.isSurprise || e.isFlatFoot || e.isFlanking || 
         (e.isAoO && e.EActor->HasFeat(FT_COMBAT_OPPORTUNIST)) ||
         e.EVictim->noDexDefense()) &&
        (e.EActor->DistFrom(e.EVictim) <= 3)) {
        e.vHit += 4;
        e.strHit += " +4 BS";
    } 

    if (e.EActor->HasMFlag(M_LIGHT_AVERSE) && e.EActor->inField(FI_LIGHT))
      {
        e.vHit -= 4;
        e.strHit += " -4 light";
      }

    if (e.EVictim->HasMFlag(M_LIGHT_AVERSE) && e.EVictim->inField(FI_LIGHT))
      {
        e.vDef -= 4;
        e.strDef += " -4 light";
      }

    if (e.EVictim->HasAbility(CA_GIANT_KILLER) &&
        e.EActor->Attr[A_SIZ] > e.EVictim->Attr[A_SIZ] &&
        e.EActor->HasMFlag(M_HUMANOID) &&
        !e.isSurprise)
      {
        int gkBonus = (e.EActor->Attr[A_SIZ] - e.EVictim->Attr[A_SIZ]) * 2;
        e.vDef += gkBonus;
        e.strDef += Format(" %+d giant-killer",gkBonus);
      }
    else if (e.EActor->HasAbility(CA_GIANT_KILLER) &&
        e.EVictim->Attr[A_SIZ] > e.EActor->Attr[A_SIZ] &&
        e.EVictim->HasMFlag(M_HUMANOID))
      {
        int gkBonus = (e.EVictim->Attr[A_SIZ] - e.EActor->Attr[A_SIZ]);
        e.vThreat -= gkBonus;
      }
      
    if (e.EActor->HasAbility(CA_SIZE_ADVANTAGE))
      if (e.EActor->GetAttr(A_SIZ) == SZ_SMALL)
        {
          int16 ab = e.EActor->AbilityLevel(CA_SIZE_ADVANTAGE);
          if (ab >= 17)
            e.vThreat--; 
          if (ab >= 9)
            e.vThreat--;
          if (ab >= 3)
            e.vThreat--;
          e.vThreat--;
        }

    /* already in calc values!
    if ((e.EItem && e.EItem->HasQuality(WQ_ACCURACY)) ||
        (e.AType != A_SUND && e.EItem2 && e.EItem2->HasQuality(WQ_ACCURACY))) 
      {
        e.vHit += 4;
        e.strHit += " +4 accuracy";
      }
      */
    
    if ((e.EItem && e.EItem->HasQuality(WQ_GLOWING)) ||
        (e.EItem2 && e.EItem2->HasQuality(WQ_GLOWING)))
      if (e.EVictim->HasMFlag(M_LIGHT_AVERSE) ||
          e.EVictim->isMType(MA_WRAITH))
        {
          e.vHit += 2;
          e.strHit += " +2 glow";
        }


    {
      int b = 0; 
      if (e.EItem && e.EItem->HasQuality(WQ_BANE) &&
            e.EItem->isBaneOf(e.EVictim))
        b += 2;
      if (e.EItem2 && e.EItem2->HasQuality(WQ_BANE) &&
            e.EItem2->isBaneOf(e.EVictim))
        b += 2; 
      if (b > 0) { 
        e.vHit += b;
        e.bDmg += b;
        e.strHit += Format(" %+d bane",b);
        e.strDmg += Format(" %+d bane",b);
      }
    } 



    if (e.EVictim->HasStati(ILLUMINATED)) {
      e.vHit += 1;
      e.strHit += " +1 FF";
    }

    /* Swarm Tactics in Melee: Attacker */
    swarm = 0;
    if (e.EActor->HasFeat(FT_SWARM))
      if (abs(e.EActor->x - e.EVictim->x) <= 1 && abs(e.EActor->y - e.EVictim->y) <= 1)
        for(i=0;i!=8;i++)
          for (c=m->FCreatureAt(e.EActor->x+DirX[i],e.EActor->y+DirY[i]);c;
                 c=m->NCreatureAt(e.EActor->x+DirX[i],e.EActor->y+DirY[i]))
            if (!c->isHostileTo(e.EActor) && !e.EActor->isHostileTo(c))
              if (c->Attr[A_SIZ] <= e.EActor->Attr[A_SIZ])
                swarm++;
    if (swarm)
      {
        e.vHit += swarm;
        e.bDmg += swarm;
        e.strHit += Format(" +%d swarm",swarm);
        e.strDmg += Format(" +%d swarm",swarm);
      }
    /* Swarm Tactics in Melee: Defender */
    swarm = 0;
    if (e.EVictim->HasFeat(FT_SWARM))
      if (abs(e.EActor->x - e.EVictim->x) <= 1 && abs(e.EActor->y - e.EVictim->y) <= 1)
        for(i=0;i!=8;i++)
          for (c=m->FCreatureAt(e.EVictim->x+DirX[i],e.EVictim->y+DirY[i]);c;
                c=m->NCreatureAt(e.EVictim->x+DirX[i],e.EVictim->y+DirY[i]))
            if (!c->isHostileTo(e.EVictim) && !e.EVictim->isHostileTo(c))
              if (c->Attr[A_SIZ] <= e.EVictim->Attr[A_SIZ])
                swarm++;
    if (swarm)
      {
        e.vDef += swarm;
        e.strDef += Format(" +%d swarm",swarm);
      }

    /* Close Tactics in Melee: Attacker */
    walls = 0;
    if (e.EActor->HasFeat(FT_CLOSE_QUARTERS_FIGHTER))
      if (abs(e.EActor->x - e.EVictim->x) <= 1 && abs(e.EActor->y - e.EVictim->y) <= 1)
        for(i=0;i!=8;i++)
          if (m->SolidAt(e.EActor->x+DirX[i],e.EActor->y+DirY[i]))
                walls++;
    if (walls >= 4)
      {
        e.vHit += 1;
        e.bDmg += 1;
        e.strHit += " +1 close";
        e.strDmg += " +1 close";
      }
    walls = 0;
    if (e.EVictim->HasFeat(FT_CLOSE_QUARTERS_FIGHTER))
      if (abs(e.EActor->x - e.EVictim->x) <= 1 && abs(e.EActor->y - e.EVictim->y) <= 1)
        for(i=0;i!=8;i++)
          if (m->SolidAt(e.EVictim->x+DirX[i],e.EVictim->y+DirY[i]))
                walls++;
    if (walls >= 4)
      {
        e.vDef += 1;
        e.strDef += " +1 close";
      }


    if (c = (Creature*)e.EActor->GetStatiObj(DWARVEN_FOCUS))
      {
        if (c == e.EVictim)
          { e.strHit += " +2 focus"; e.vHit += 2;
            e.strDmg += " +2 focus"; e.bDmg += 2; }
        else if (e.EVictim->isHostileTo(c))
          { e.strHit += " -2 focus"; e.vHit -= 2;
            e.strDmg += " -2 focus"; e.bDmg -= 2; }
      }    
      
    if (e.EVictim->HasFeat(FT_TERRIBLE_MEIN))
      if ((is_standard_attk(e.AType) ||
          is_postgrab_attk(e.AType) ||
          e.AType == A_SWNG || e.AType == A_GREA) &&
            e.DType <= AD_LAST_PHYSICAL)
        if (e.EActor->isMType(MA_SAPIENT) && !(e.EActor->
              ResistLevel(AD_FEAR) == -1))
          {
            int16 bon, cha;
            bon = e.EActor->SumStatiMag(SAVE_BONUS,SN_FEAR);
            cha = e.EVictim->Mod(A_CHA);
            if (cha > max(0,bon))
              { e.strDmg += Format(" %+d mein", -(cha - bon));
                e.bDmg   -= (cha - bon); }
          }

    /* This needs to be here rather then in CalcValues because we
       don't wield a thrown weapon before throwing it, ergo we
       can't do this kind of precalculation. */
    if (e.AType == A_HURL && !((Weapon *)e.EItem2)->useStrength())
      e.subStr = true;
    else if (e.vicIncor && !e.isGhostTouch)
      e.subStr = true;

    if (e.isGreatBlow) {
      e.vHit += 2;
      e.bDmg += 2;
      if (e.isGhostTouch || !e.vicIncor)
        e.Dmg.Bonus += max(0,e.EActor->Mod(A_STR));
      e.strHit += " +2 GB";
      e.strDmg += " +2 GB";
      if (e.EActor->HasStati(SMITE_ABILITY)) {
        v = 0;
        StatiIterNature(e.EActor,SMITE_ABILITY)
            if ((!S->Val) || (e.EVictim->isMType(S->Val)))
              v += S->Mag;
        StatiIterEnd(this)

        if ((e.EItem && e.EItem->HasQuality(WQ_SMITING)) || 
            (e.EItem2 && e.EItem2->HasQuality(WQ_SMITING))) {
          e.AType = A_COUP; // free hit, not insta-kill 
          e.isCrit = true; 
          v *= 2;
        }

        if (v) {
          e.bDmg += v;
          e.strDmg += Format(" +%d smite",v);
          if (e.EActor->Mod(A_CHA) > 0) {
            e.vHit += e.EActor->Mod(A_CHA);
            e.strHit += Format(" +%d smite",
                      e.EActor->Mod(A_CHA));
            }
          }      
        }      
      }
    
    if (e.isPrecision) {
      e.vHit += e.EActor->SkillLevel(SK_FIND_WEAKNESS) / 2;
      e.strHit += Format(" %+d precise", 
           e.EActor->SkillLevel(SK_FIND_WEAKNESS) / 2);
      }

    /* Clerics with Retribution have their threat ranges increased
       by two when they have lost 50% or more of their total hit
       points in combat. */
    /* This ability may be too powerful, or too weak. Feel free to
       tinker with the numbers, but please don't remove it or alter
       the basic concept. */
    if (e.EActor->HasAbility(CA_RETRIBUTIVE_STRIKE))
      if (e.EActor->HasStati(WOUNDED_BY,0,e.EVictim))
        if (e.EActor->GetStatiMag(WOUNDED_BY,0,e.EVictim) >= 
              max(1,e.EActor->mHP / 10) )
          {
            int16 i;
            //e.vThreat -= 2;
            i = ((e.EActor->GetStatiMag(WOUNDED_BY,0,e.EVictim) * 10) /
                  e.EActor->mHP);
            i = max(1,AbilityLevel(CA_RETRIBUTIVE_STRIKE));
            e.strHit += Format(" + %d Ret",i);
            e.strDmg += Format(" + %d Ret",i*2);
            e.vHit += i;
            e.bDmg += i*2;        
          }

    if (e.isMoveAoO && e.EVictim->HasStati(TUMBLING)) {
      e.vDef += e.EVictim->SkillLevel(SK_TUMBLE) / 2;
      e.strDef += Format(" + %d tumble", e.EVictim->SkillLevel(SK_TUMBLE) / 2);
      }
    if (e.EActor->HasStati(TUMBLING) && e.EActor->SkillLevel(SK_TUMBLE) >
                                        e.EVictim->GetAttr(A_WIS))
      e.vThreat -= 2;

    int16 n;
    if (n = e.EActor->getBestKnowSkill(e.EVictim,true))
      {
        e.vThreat -= n / 9;
      }
    if (n = e.EVictim->getBestKnowSkill(e.EActor,true))
      {
        e.vDef += n / 3;
        e.strDef += Format(" + %d know", n / 3);
      }
    

    if (e.AType == A_SPEL) {
      if (e.eID && !TEFF(e.eID)->HasFlag(EF_MUNDANE))
        {
          e.vHit += e.EActor->Mod(A_INT);
          e.strHit += Format(" %+d Int", e.EActor->Mod(A_INT));
        }
      e.vHit += e.EActor->Mod(A_DEX);
      e.strHit += Format(" %+d Dex", e.EActor->Mod(A_DEX));
      e.vHit += 5;
      e.strHit += Format(" +5 Acc");
      }

    if (e.AType == A_FIRE || e.AType == A_SPEL || e.AType == A_HURL) {
      i = dist(e.EActor->x,e.EActor->y,e.EVictim->x,e.EVictim->y);
      if (dist(e.EActor->x,e.EActor->y,e.EVictim->x,e.EVictim->y) <= 3)
        if (e.EActor->HasFeat(FT_POINT_BLANK_SHOT))
          {
            e.vHit += 1;
            e.bDmg += 1;
            e.strHit += " +1 PBS";
            e.strDmg += " +1 PBS";
          }
      if (i / e.vRange) 
        {
          e.vHit -= 2 * (i / e.vRange);
          e.strHit += Format(" -%d range",2*(i/e.vRange));
        }
      } 

    if (e.AType != A_FIRE && e.AType != A_HURL) {
      int bonus = 0; 
      if (e.EActor->HasStati(CHARGING))
        bonus = max(bonus, e.EActor->ChargeBonus());
      Creature * rider = (Creature *)e.EActor->GetStatiObj(MOUNT);
      if (rider && rider->HasStati(CHARGING))
        bonus = max(bonus, rider->ChargeBonus());
      // in Hit(), we double the damage of a charging lance and handle
      // FT_SPIRITED_CHARGE
      if (bonus) { 
        e.vHit += 2;
        e.bDmg += bonus;
        if (HasFeat(FT_POWER_CHARGE))
          e.vThreat -= bonus / 3; 
        e.strHit += " +2 charge";
        e.strDmg += Format (" %+d charge",bonus);
      }
#if 0
      int8 cb = ChargeBonus[min(29,GetStatiMag(CHARGING))], mult = 1;
      // ww: the textual description given to the user would lead one to
      // believe that these things stack ... and why not? 
      if (HasFeat(FT_POWER_CHARGE))
        e.vThreat -= cb;
      if (e.EItem && e.EItem->HasIFlag(WT_CHARGE))
        mult++;
      if (e.EActor->HasStati(MOUNTED))
        if (HasFeat(FT_SPIRITED_CHARGE))
          mult++;
      if (e.EActor->HasFeat(FT_FLYING_KICK))
        if (e.AType == A_KICK)
          mult++;
      cb *= mult;            
      e.vHit += cb;
      e.bDmg += cb;
      e.strHit += Format (" %+d charge",cb);
      e.strDmg += Format (" %+d charge",cb);
#endif
      }
    if (e.isAoO && e.isMoveAoO && e.EVictim->HasFeat(FT_MOBILITY)) {
      e.strDef += " +4 mobility";
      e.vDef += 4;
      }
    if (e.isAoO && e.isMoveAoO && e.EVictim->HasStati(MOUNTED) &&
                 e.EVictim->HasFeat(FT_RIDE_BY_ATTACK)) {
      e.strDef += " +4 ride-by";
      e.vDef += 4;
      }
    if (e.isAoO && e.isMoveAoO && e.EVictim->HasStati(CHARGING)) {
      int32 bonus = max(e.EVictim->ChargeBonus(),1); 
      if (bonus) { 
        e.bDmg += (int16)bonus;
        e.strDmg += Format (" %+d charge",bonus);
      }
      if (e.EActor->HasFeat(FT_GUARDED_STANCE)) {
        e.strHit += " +8 stance";
        e.vHit += 8;
      }
    } 

    if (e.actIncor) {
      // srd: An incorporeal creature's attacks pass through (ignore)
      // natural armour, armour, and shields, although deflection bonuses and
      // force effects (such as mage armour) work normally against it. 
      e.isBypass = true; 
    } 

    if (e.EActor->HasStati(MANIFEST))
      e.isGhostTouch = true;
    if (e.EVictim->HasStati(MANIFEST)) {
      e.isGhostTouch = true;
      if (!e.EActor->HasMFlag(M_MINDLESS)) {
        e.vDef += 2;
        e.strDef += " +2 awe";
        }
      }
    /* Currently, *only* elementals have the air/earth/water mastery abilities.
       We thus check if it's an elemental first, and only afterward if it has
       any of the abilities. This is because isMType calls are fairly cheap,
       timewise, whereas HasAbility calls are more substantial. If we want to
       give these abilities out to other monsters later, this should be
       changed. */
    if (e.EActor->isMType(MA_ELEMENTAL))
      {
        if (e.EActor->HasAbility(CA_AIR_MASTERY) && e.EVictim->isAerial() &&
             !m->SolidAt(e.EVictim->x,e.EVictim->y))
          {
            e.vHit += e.EActor->AbilityLevel(CA_AIR_MASTERY);
            e.strHit += Format(" +%d AirMas", e.EActor->AbilityLevel(CA_AIR_MASTERY));
          }
        if (e.EActor->HasAbility(CA_WATER_MASTERY) && !e.EVictim->isAerial())
          if (TTER(m->TerrainAt(e.EVictim->x,e.EVictim->y))->HasFlag(TF_WATER) || 
              TTER(m->TerrainAt(e.EVictim->x,e.EVictim->y))->Material == MAT_ICE)
            {
              e.vHit += e.EActor->AbilityLevel(CA_WATER_MASTERY);
              e.strHit += Format(" +%d WaterMas", e.EActor->AbilityLevel(CA_WATER_MASTERY));
            }
    
        if (e.EActor->HasAbility(CA_EARTH_MASTERY) && !e.EVictim->isAerial())
          if ((i = TTER(m->TerrainAt(e.EVictim->x,e.EVictim->y))->Material) == 
             MAT_GRANITE || i == MAT_QUARTZ || i == MAT_MAGMA || i == MAT_MINERAL)
            {
              if (m->SolidAt(e.EVictim->x,e.EVictim->y)) {
                e.vHit += e.EActor->AbilityLevel(CA_EARTH_MASTERY)*2;
                e.strHit += Format(" +%d EarthMas", e.EActor->AbilityLevel(CA_EARTH_MASTERY)*2);
                }
              else {
                e.vHit += e.EActor->AbilityLevel(CA_EARTH_MASTERY);
                e.strHit += Format(" +%d EarthMas", e.EActor->AbilityLevel(CA_EARTH_MASTERY));
                }
            }
       }
    if (e.EVictim->isMType(MA_ELEMENTAL)) 
      {
        if (e.EVictim->HasAbility(CA_AIR_MASTERY) && e.EActor->isAerial() &&
             !m->SolidAt(e.EActor->x,e.EActor->y))
          {
            e.vDef += e.EVictim->AbilityLevel(CA_AIR_MASTERY);
            e.strDef += Format(" +%d AirMas", e.EVictim->AbilityLevel(CA_AIR_MASTERY));
          }
        if (e.EVictim->HasAbility(CA_WATER_MASTERY) && !e.EActor->isAerial())
          if (TTER(m->TerrainAt(e.EActor->x,e.EActor->y))->HasFlag(TF_WATER) || 
              TTER(m->TerrainAt(e.EActor->x,e.EActor->y))->Material == MAT_ICE)
            {
              e.vDef += e.EVictim->AbilityLevel(CA_WATER_MASTERY);
              e.strDef += Format(" +%d WaterMas", e.EVictim->AbilityLevel(CA_WATER_MASTERY));
            }
    
        if (e.EVictim->HasAbility(CA_EARTH_MASTERY) && !e.EActor->isAerial())
          if ((i = TTER(m->TerrainAt(e.EActor->x,e.EActor->y))->Material) == 
             MAT_GRANITE || i == MAT_QUARTZ || i == MAT_MAGMA || i == MAT_MINERAL)
            {
              if (m->SolidAt(e.EActor->x,e.EActor->y)) {
                e.vDef += e.EVictim->AbilityLevel(CA_EARTH_MASTERY)*2;
                e.strDef += Format(" +%d EarthMas", e.EVictim->AbilityLevel(CA_EARTH_MASTERY)*2);
                }
              else {
                e.vDef += e.EVictim->AbilityLevel(CA_EARTH_MASTERY);
                e.strDef += Format(" +%d EarthMas", e.EVictim->AbilityLevel(CA_EARTH_MASTERY));
                }
            }
       }

    {
      int a_mag = e.EActor->AbilityLevel(CA_PLANT_MASTERY);
      int v_mag = e.EVictim->AbilityLevel(CA_PLANT_MASTERY);
      bool plant_at = e.EActor->m->TreeAt(e.EActor->x,e.EActor->y) ||
                      e.EVictim->m->TreeAt(e.EVictim->x,e.EVictim->y);
      if (a_mag && (plant_at || e.EVictim->isMType(MA_PLANT))) {
        e.vHit += a_mag; 
        e.strHit += Format(" +%d PlantMas", a_mag);
      } 
      if (v_mag && (plant_at || e.EActor->isMType(MA_PLANT))) {
        e.vDef += v_mag; 
        e.strDef += Format(" +%d PlantMas", v_mag);
      } 
    } 

    if (TTER(m->TerrainAt(e.EVictim->x,e.EVictim->y))->
               HasFlag(TF_WATER) && !e.EVictim->isAerial())
      {
        int16 swim_mod;
        swim_mod = e.EVictim->isMType(MA_AQUATIC) ? 0 : -4;
        if (e.EVictim->HasSkill(SK_SWIM) && e.EVictim->SkillLevel(SK_SWIM) > 10)
          swim_mod += e.EVictim->SkillLevel(SK_SWIM) - 10;
        e.vDef += swim_mod;
        e.strDef += Format(" %+d Swim", swim_mod);
      }

    if (e.EActor->HasStati(FAV_ENEMY)) {
      v = getFavEnemyBonus(e.EVictim);
      if (v) {
        e.strDmg += Format(" %+d FE",v);
        e.strHit += Format(" %+d FE",v);
        e.vHit += v; e.bDmg += v;
        }
      }

    if ((e.EItem && e.EItem->Material() == MAT_SILVER) ||
        (e.EItem2 && e.EItem2->Material() == MAT_SILVER))
      if (e.EVictim->isMType(MA_LYCANTHROPE) || e.EVictim->isMType(MA_DEVIL))
        {
          /* Later, silver weapons will inflict normal, instead of subdual,
             damage to these creatures in addition to (or instead of) the
             combat bonuses. */
          e.vHit += 2; e.strHit += " +2 silver";
          e.bDmg += 2; e.strDmg += " +2 silver";
        }

    if ( 
        ((e.EVictim->EInSlot(SL_ARMOUR) &&
           e.EVictim->EInSlot(SL_ARMOUR)->Material() == MAT_SILVER) ||
          (e.EVictim->EInSlot(SL_READY) &&
           e.EVictim->EInSlot(SL_READY)->isType(T_SHIELD) && 
           e.EVictim->EInSlot(SL_READY)->Material() == MAT_SILVER)) &&
         (e.EActor->isMType(MA_DEMON) || e.EActor->isMType(MA_LYCANTHROPE) ||
          e.EActor->isMType(MA_DEVIL) || e.EActor->isMType(MA_UNDEAD)) ) {
      e.vDef += 2; e.strDef += " +2 silver";
    } 

    if (c = ((Creature*)e.EVictim)->ts.getLeader())
      if (c->HasAbility(CA_SHIELD_GROUP) && 
          dist(e.EVictim->x,e.EVictim->y,c->x,c->y) <= 9)
        {
          e.vDef += c->AbilityLevel(CA_SHIELD_GROUP);
          e.strDef += Format(" %+d SG", c->AbilityLevel(CA_SHIELD_GROUP));
        }

    if (e.isSurprise && e.EVictim->HasAbility(CA_NOBLE_AEGIS))
      {
        e.vDef += 4;
        e.strDef += " +4 NoAe";
      }

    /* fjm: This is an important balance tweak for Rogues and
       other stealth-based fighters who are often Small. Dex
       can replace Str when modifying damage if A) the weapon
       is smaller then the target, B) Dex > Str, C) it is a
       surprise attack against a flat-footed foe. */
    if (e.isSurprise && e.isFlatFoot)
      if (e.AType == A_SWNG || e.AType == A_FIRE ||
          e.AType == A_THRU || e.AType == A_LUNG ||
          e.AType == A_CALL || e.AType == A_PUNC ||
          e.AType == A_HURL || e.AType == A_KICK)
        if (e.EItem == NULL || (e.EItem->Size(e.EActor) <= SZ_MEDIUM))
          if (e.EActor->Mod(A_STR) < e.EActor->Mod(A_DEX))
          {
            e.subStr = true;
            e.bDmg += e.EActor->Mod(A_DEX);
            e.strDmg += Format(" %+d Dex",
              e.EActor->Mod(A_DEX));
          }

    if (e.EVictim->HasStati(MOUNT))
      {
        Creature *rider = (Creature*) e.EVictim->GetStatiObj(MOUNT);
        if (rider && rider->HasFeat(FT_MOUNTED_COMBAT))
          e.vRideCheck = rider->SkillLevel(SK_RIDE) + Dice::Roll(1,20);
      }

    if (e.AType == A_FIRE || e.AType == A_HURL)
      if (e.EActor->HasStati(MOUNTED))
        {
          int16 val;
          val = e.EActor->HasFeat(FT_MOUNTED_ARCHERY) ? -2 : -4;
          if (e.EActor->HasStati(CHARGING))
            val *= 2;
          e.vHit += val;
          e.strHit += Format(" %+d mounted",val);
        }
        
    if (e.subStr)
      e.Dmg.Bonus -= e.modStr;

    if (e.AType == A_KICK && e.EActor->HasFeat(FT_POWER_KICK))
      { e.strDmg += " +2 PK"; e.bDmg += 2; }
    
    if (e.EVictim->isCharacter())
      if (e.EActor->HasStati(PRONE) ||
          e.EActor->HasStati(BLIND) ||
          e.EActor->HasStati(STUCK) ||
          e.EActor->HasStati(PARALYSIS))
        e.EActor->GainTempStati(HIT_WHILE_UNREADY,e.EVictim,-2,SS_MISC);

    return NOTHING;
  }

EvReturn Creature::Strike(EventInfo &e) /* this == EActor */
	{
    int16 i, tx, ty; bool isSilent; Item *it;

    isSilent = Silence != 0;

    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
      e.EItem = NULL;

    if (e.EActor->isMType(MA_FUNGI) && e.EVictim->isMType(MA_FUNGI))
      if (e.EActor->tmID == e.EVictim->tmID)
        return ABORT;

    if (!e.ETarget->isCreature())
      return ReThrow(EV_DAMAGE,e);

    e.vRoll = random(20) + 1;
    e.vtRoll = random(20) + 1;
    if (e.EActor->HasFeat(FT_MURDEROUS))
      e.vtRoll += 2;

    #if 0
    if (e.EItem && !e.EItem->isGroup(WG_MARTIAL) && 
         (e.AType == A_SWNG || e.AType == A_GREA) &&
         !(e.isFlatFoot || e.isSurprise || e.actUnseen))
      {
        Item *other;
        other = e.EVictim->InSlot(SL_WEAPON) ?
                e.EVictim->InSlot(SL_WEAPON) :
                e.EVictim->InSlot(SL_READY);
        if (other && other->isType(T_WEAPON))
          {
            if (e.EVictim->InSlot(SL_READY) &&
                e.EVictim->InSlot(SL_READY)->Size() > other->Size())
              other = e.EVictim->InSlot(SL_READY);
            if (e.EItem->Size() + 1 < other->Size())
              e.EActor->ProvokeAoO(e.EVictim);
            if (e.EActor->isDead())
              return DONE;
          }
      }
    #endif
    
    if (e.EActor->HasStati(ENGULFED))
      {
        /* If we got here with this, it's part of some special or natural
           sequence, so don't print a message. */
        if (e.AType == A_BULL || e.AType == A_GRAB || e.AType == A_DISA ||
            e.AType == A_TRIP  || e.AType == A_THRO)
          return ABORT;
        e.isHit = true;
        e.isBypass = e.EActor->GetStatiVal(ENGULFED) ==
                       EG_SWALLOW ? true : false;
        e.isCrit = false;
        e.vRoll = 10;
        goto DoHit;
      }      

    if ((e.AType == A_PUNC || e.AType == A_KICK) && !e.EActor->isMType(MA_QUADRUPED))
      if (!(HasAbility(CA_UNARMED_STRIKE) || HasFeat(FT_MARTIAL_MASTERY)))
        {
          e.EActor->ProvokeAoO(e.EVictim);
          if (e.EActor->isDead())
            return DONE;
        }

    if (e.actUnseen) {
      /* Attacker gets +2 to hit */
      e.vHit += 2;
      e.strHit += " +2 conceal";
      }
    
    if (e.vicUnseen && !e.isSeeking && 
        !e.EActor->HasStati(GRAPPLING,-1,e.EVictim)) {
      /* 50% miss chance; 25% with Blind-Fight */
      if (!random(e.EActor->HasStati(FT_BLIND_FIGHT) ? 4 : 2))
        {
          e.isWildMiss = true;
          e.strDef += " (unable to see)";
          goto PostAttack;
        }

      }
      

    if (e.EActor->HasStati(STUNNED) && !e.isSeeking && !random(2))
      {
        e.isWildMiss = true;
        e.strDef += " (stunned)";
        goto PostAttack;
      }

    if (!e.isSeeking && e.EVictim->HasStati(MISS_CHANCE) &&
           (e.vRoll < 20)) {
      // ww: after a few wild misses at someone with Displacement, you
      // eventually get the hang of it ... the PRACTICE_MISS_CHANCE
      // temporary stati gives you a 10% reduction in their miss chance
      // per recent hit ...
      int practice = 0;
      Status * practice_s = e.EActor->GetStati(PRACTICE_MISS_CHANCE,-1,
          e.EVictim);
      if (practice_s) {
        practice = practice_s->Mag;
        practice_s->Mag += 10; 
        practice_s->Duration += 120; 
      } else {
        e.EActor->GainTempStati(PRACTICE_MISS_CHANCE,e.EVictim,120,
            SS_ATTK,-1,10,0,0);
      }

      StatiIterNature(e.EVictim,MISS_CHANCE)
          if (S->Val == MISS_RANGED &&
              e.AType != A_FIRE && e.AType != A_HURL)
            continue; 
          bool can_see_you = 
            e.EActor->HasStati(SEE_INVIS) || 
            e.EActor->HasStati(TRUE_SIGHT) ||
            e.EActor->HasMFlag(M_SEE_INVIS) ||
            e.EVictim->HasStati(ILLUMINATED);

          /* True Seeing negates the miss chance from both
             blur and displacement, but not from blink. */
          if (can_see_you)
            if (S->eID && 
                TEFF(S->eID)->Schools & SC_ILL)
              continue;
          if (S->eID && 
              TEFF(S->eID)->Schools & SC_WEA) {
            bool can_hit_you = e.isGhostTouch;
            /*
              (e.EItem && e.EItem->isGhostTouch()) ||
              (e.EItem2 && e.EItem2->isGhostTouch());
              */
            if (can_see_you && can_hit_you)
              continue;
            else if (can_see_you || can_hit_you) 
              if (random(100)+1+practice <= S->Mag/2) {
                e.isWildMiss = true;
                e.strDef += Format(" (%s/2)", NAME(S->eID));
                StatiIterBreakout(e.EVictim,goto PostAttack)
              }
            continue;
          }
          if (random(100)+1+practice <= S->Mag) {
            e.strDef += Format(" (%s)", NAME(S->eID));
            e.isWildMiss = true;
            StatiIterBreakout(e.EVictim,goto PostAttack)
            }
      StatiIterEnd(e.EVictim)
    }
    /* 25% cover from climbing trees. */
    if (!e.isSeeking && e.EVictim->HasStati(ELEVATED,ELEV_TREE) && 
          m->TreeAt(e.EVictim->x, e.EVictim->y))
      if (random(100)+1 <= 25)
        {
          e.strDef += " (elevation)";
          e.isWildMiss = true;
          goto PostAttack;
        }
    /* 50% cover from tower shields */
    {
    Item * shld = e.EVictim->EInSlot(SL_READY);
    if (shld && shld->isType(T_SHIELD) && !e.isSeeking && 
        shld->Size() > e.EVictim->Attr[A_SIZ])
      if (random(100)+1 <= 50)
      {
          e.strDef += Format(" (shield)");
          e.isWildMiss = true;
          goto PostAttack;
      }
    }

          
    /* A target is considered to have 50% cover if there is a block
       right beside them in the same direction as their enemy. For
       example, in this case the hill giant has 50% cover against
       the adventurer, but not vice versa -- the giant can just throw
       a boulder, then duck back around the corner for cover:
         ###############
         ##        @
         ##H############
         ## ############
    */
    if (e.AType == A_HURL || e.AType == A_FIRE || e.AType == A_SPEL)
      {
        tx = e.EActor->x - e.EVictim->x;
        ty = e.EActor->y - e.EVictim->y;
        if (abs(tx) >= abs(ty)*3)
          ty = 0;
        else if (abs(ty) >= abs(tx)*3)
          tx = 0;
        if (tx > 1)  tx = 1;
        if (tx < -1) tx = -1;
        if (ty > 1)  ty = 1;
        if (ty < -1) ty = -1;
        if (e.EMap->SolidAt(e.EVictim->x+tx,e.EVictim->y+ty))
          if (!e.isSeeking && random(100)+1 < 50)
            {
              e.isWildMiss = true;
              e.strDef += " (cover)";
              goto PostAttack;
            }
      } 


    if ( ( (e.AType == A_HURL || e.AType == A_FIRE) &&
         (e.EActor->HasFeat(FT_SHOOT_FROM_COVER)) &&
         (!(e.EActor->isBeside(e.EVictim))) &&
         (e.EActor->HasStati(HIDING)) )) {
      e.EVictim->ts.HearNoise(e.EVictim,(uint8)e.EActor->x,(uint8)e.EActor->y);
    } else 
      e.EActor->Reveal(true);
    e.EVictim->Reveal(false);



    if (e.actIncor && !e.EActor->HasStati(IGNORES_WEAPON_IMMUNITY)) {
      bool attk_ok = (e.AType == A_TUCH) || (e.AType == A_SPEL) ||
                     (e.AType == A_COUP && e.EActor->HasAttk(A_TUCH));
      if (!e.vicIncor && !e.isGhostTouch && (!attk_ok)) {
        e.EMap->SetQueue(QUEUE_DAMAGE_MSG);
        TPrint(e,"Your attack passes clean through the <EVictim>.",
          "The <EActor>'s attack passes clean through you.",
          "The <EActor>'s attack passes clean through the <EVictim>.");
        e.EMap->UnsetQueue(QUEUE_DAMAGE_MSG);
        return DONE;
      }
    }

    if (e.vicIncor && !e.EActor->HasStati(IGNORES_WEAPON_IMMUNITY)) 
      if (!e.actIncor && !e.isGhostTouch && e.AType != A_SPEL) {
        if (e.EItem) {
          if (e.EItem->isBlessed() && e.EVictim->isMType(MA_EVIL))
            goto AttackOk; 
          else if (e.EItem->GetPlus() > 0)
            goto AttackOk;
          } 
        else if (e.EItem2) {
          if (e.EItem2->isBlessed() && e.EVictim->isMType(MA_EVIL))
            goto AttackOk; 
          else if (e.EItem2->GetPlus() > 0)
            goto AttackOk;
          } 
        else if (!e.EItem && !e.EItem2 && 
                   e.EActor->HasAbility(CA_KI_STRIKE)) {
          // ww: ki strike means your fists count as +X magical weapons
          goto AttackOk;
        }
        e.EMap->SetQueue(QUEUE_DAMAGE_MSG);
        TPrint(e,"Your attack passes clean through the <EVictim>.",
            "The <EActor>'s attack passes clean through you.",
            "The <EActor>'s attack passes clean through the <EVictim>.");
        e.EMap->UnsetQueue(QUEUE_DAMAGE_MSG);
        goto PostAttack; //ww: return DONE;
      }
      
      
    AttackOk:

    /* Implement Escape Artist vs. Grab */

    /* If EVictim has GRABBED, no nead for roll! */

    /* Bull Rush and Disarm are attacks, but they do not require a
       (conventional) attack roll. Instead, they use only the opposed
       check performed in ManeuverCheck. Disarm has an "opposed attack
       roll" but this is treated as an opposed roll rather than an
       attack roll. */
    if (e.AType == A_BULL || e.AType == A_DISA || e.AType == A_COUP || e.AType == A_THRO)
      { e.isHit = true; e.vRoll = 10; }
    if (is_postgrab_attk(e.AType))
      e.isHit = true;
    if (e.isHit) 
        goto SkipRoll; 

    /* This uses the *exact* logic of the OGL system, as follows:
       - An unmodified roll of a 20 is always a hit
       - An unmodified roll within the threat range of an attack
         scores a threat, provided it hits, but does not score an
         automatic hit. Thus, 1st level fighters with rapiers hit
         Great Wyrms 5% of the time, not 15%.
    */        

    if (e.AType == A_HURL && e.EVictim->HasSkill(SK_PICK_POCKET))
      if (e.EVictim->SkillLevel(SK_PICK_POCKET) >= 15)
        if (e.EItem2 && e.EItem2->Size() <= SZ_SMALL)
          {
            if (e.EVictim->SkillCheck(SK_PICK_POCKET,11 + (e.vHit + e.vRoll)/2,true))
              {
                int16 tm;
                VPrint(e,"You catch the thrown <EItem2>!",
                         "The <EVictim> catches the thrown <EItem2>!");
                tm = e.EVictim->Timeout;
                e.EVictim->GainItem(e.EItem2);
                e.EVictim->Timeout = tm;
                goto PostAttack;
              }
            else
              e.EVictim->IPrint("You fail to catch the thrown <Obj>!", e.EItem2);
          }                                  
                            
    if ((e.vHit + e.vRoll >= max(e.vDef,
          (e.vRideCheck ? e.vRideCheck : -40))) || e.vRoll == 20)
      e.isHit = true;
    else
      e.isHit = false;

    /* This should always stay the LAST check. You get comparatively
       few FDs each day, so it's important that the game not "burn"
       them on attacks that would be evaded by miss chance, poor
       attack roll, etc. */
    if (e.isHit && e.EVictim->HasStati(FLAWLESS_DODGE,1))
      if (e.EVictim->SumStatiMag(FLAWLESS_DODGE) <
            e.EVictim->AbilityLevel(CA_FLAWLESS_DODGE) + e.EVictim->Mod(A_DEX))
        {
          e.EVictim->IncStatiMag(FLAWLESS_DODGE,SS_MISC,1,NULL,1);
          e.EMap->SetQueue(QUEUE_DAMAGE_MSG);
        
          TPrint(e,"The <EVictim> flawlessly eludes your attack.",
                   "You flawlessly elude the <EActor>'s attack.",
                   "The <EVictim> flawlessly eludes the <EActor>'s attack.");
          e.EVictim->IPrint(Format("(%d left)", (e.EVictim->AbilityLevel(CA_FLAWLESS_DODGE) 
            + e.EVictim->Mod(A_DEX) - e.EVictim->SumStatiMag(FLAWLESS_DODGE))));
          e.EVictim->Exercise(A_DEX,random(3)+1,EDEX_EVASION,45);
          e.EMap->UnsetQueue(QUEUE_DAMAGE_MSG);
          e.isHit = false;
        }

    if (!e.isHit && e.vRoll == 1 && (e.vHit + e.vtRoll) < e.vDef)
      e.isFumble = true;
    if (e.isHit && (e.vRoll >= e.vThreat) && ((e.vHit + e.vtRoll) >= max(e.vRideCheck,e.vDef)))
      e.isCrit = true;
    else
      e.isCrit = false;  

    m->SetQueue(QUEUE_DAMAGE_MSG);
        
    if (e.isCrit && e.EVictim->ResistLevel(AD_CRIT) == -1)
      {
        if (e.EVictim->isMType(MA_UNDEAD))
          if (e.EActor->HasFeat(FT_NECROPHYSIOLOGY) ||
              e.vRoll >= (21 - (e.EActor->SkillLevel(SK_KNOW_UNDEAD) / 9)))
            goto NotCritImmune;
        VPrint(e,"Your lack of physiology prevents serious injury.",
                 "The <EVictim>'s lack of physiology prevents serious injury.");
        e.isCrit = false;
        NotCritImmune:;
      } 

    if (e.isCrit && e.EVictim->HasStati(FORTIFICATION))
      if (e.EVictim->GetStatiMag(FORTIFICATION) >= random(100)+1)
        {
          e.isCrit = false; it = (Item*)GetStatiObj(FORTIFICATION);
          if (it && it->isType(T_ARMOUR))
            VPrint(e,"Your armour negates the critical!",
                     "The <EVictim>'s armour negates the critical!");
          else if (it && it->isType(T_SHIELD))
            VPrint(e,"Your shield negates the critical!",
                     "The <EVictim>'s shield negates the critical!");
          else
            APrint(e,"Your critical hit is negated!",
                     "The critical hit on you is negated!");
          // ww: I got a segfault because 'it' was null ...
          if (it) it->VisibleID(e.EVictim);
        }
    if (e.isCrit && e.EVictim->AbilityLevel(CA_NEGATE_CRIT))
      if (e.EVictim->AbilityLevel(CA_NEGATE_CRIT)*5 >= random(100)+1)
        {
          e.isCrit = false;
          VPrint(e,"Unearthly luck prevents a brutal injury!",
                   "Unearthly luck prevents a brutal injury!");
        }

    m->UnsetQueue(QUEUE_DAMAGE_MSG);
        

    SkipRoll:

    if (e.AType == A_BULL || e.AType == A_GRAB || e.AType == A_DISA ||
       e.AType == A_TRIP  || e.AType == A_THRO)
      if (!ManeuverCheck(e))  
        {
          /* Attack hit, but oppposed roll failed; do not throw either
             EV_HIT or EV_MISS. */
          goto PostAttack;
        }

    DoHit:

    if (e.AType == A_SPEL)
      {
        if (e.isHit)
          ReThrow(EV_MAGIC_STRIKE,e);
        else if (e.eID && !e.BlastMessageDone)
          {
            String *pstr;
            pstr = TEFF(e.eID)->GetMessages(MSG_BLASTNAME);
            if ((const char*) (pstr[0]))
              TPrint(e,"Your <str1> misses the <EVictim><str2>.",
                       "The <EActor>'s <str1> misses you<str2>.",
                       "The <EActor>'s <str1> misses the <EVictim><str2>.",
                       (const char*) (pstr[0]), 
                       e.isWildMiss ? " wildly" : "");
            else
              TPrint(e,"Your <res1> misses the <EVictim><str2>.",
                       "The <EActor>'s <res1> misses you<str2>.",
                       "The <EActor>'s <res1> misses the <EVictim><str2>.",
                       e.eID, e.isWildMiss ? " wildly" : "");
          }
          
        goto PostAttack;
      }

    Map *mp; mp = e.EMap;
    mp->SetQueue(QUEUE_DAMAGE_MSG);

    if(e.isHit) {
      ReThrow(EV_HIT,e);
      if (e.isNAttack) {
        TAttack buf[1024];
        int max = ListAttacks(buf,1024);
        for (i=0;i<max;i++) {
          TAttack * at = &buf[i];
          if (at->AType == e.AType)
            break;
        } 
        for (i++ ; i<max ; i++) {
          TAttack * at = &buf[i];
          if (at->AType == A_ALSO || at->AType == A_CRIT) { 
            if (at->AType == A_ALSO || e.isCrit)
              {
                EventInfo eCopy = e; 
                eCopy.DType  = at->DType;
                eCopy.Dmg    = at->u.a.Dmg;
                eCopy.Immune = false;
                eCopy.Resist = false;
                eCopy.saveDC = (int8)e.EActor->GetPower(at->u.a.DC);
                ReThrow(EV_HIT,eCopy);
              }
            } 
          else break; 
        } 
      }
    }
    else
      ReThrow(EV_MISS,e);
    
    mp->UnsetQueue(QUEUE_DAMAGE_MSG);

    PostAttack:

    if (!isSilent)
      ReThrow(EV_ATTACKMSG,e);

    e.EActor->RemoveOnceStati(ADJUST,A_STR);
    e.EActor->RemoveOnceStati(ADJUST,A_DEX);
    e.EActor->RemoveOnceStati(ADJUST,A_HIT);
    e.EActor->RemoveOnceStati(ADJUST,A_SPD);
    e.EActor->RemoveOnceStati(ADJUST,A_DMG);
    e.EActor->RemoveOnceStati(ADJUST,A_AID);
    if (e.AType == A_HURL) {
      RemoveOnceStati(ADJUST,A_DMG_THROWN);
      RemoveOnceStati(ADJUST,A_SPD_THROWN);
      RemoveOnceStati(ADJUST,A_HIT_THROWN);
      }
    if (e.AType == A_FIRE) {
      RemoveOnceStati(ADJUST,A_DMG_ARCHERY);
      RemoveOnceStati(ADJUST,A_SPD_ARCHERY);
      RemoveOnceStati(ADJUST,A_HIT_ARCHERY);
      }


    if (!e.isAoO)
      if (!(e.EVictim->isDead() && e.EActor->HasFeat(FT_RIDE_BY_ATTACK)))
        e.EActor->RemoveStati(CHARGING);
    if (e.EVictim && e.EVictim->HasStati(ACTING)) {
      e.EVictim->HaltAction("struck");
    }

    if (!is_response_attk(e.AType) && e.EVictim->isBeside(e.EActor)) {
      TAttack *at;
      TAttack buf[1024];
      int max = e.EVictim->ListAttacks(buf,1024);
      for (i=0;i<max;i++) {
        at = &buf[i]; 
        if (is_response_attk(at->AType)) {
          if (at->DType == AD_GRAB)
            if (e.EVictim->HasStati(GRAPPLING))
              continue;
          if (at->AType == A_DEQU && !e.isHit)
            continue; 
          EventInfo e2;
          e2.Clear();
          e2.EActor  = e.EVictim;
          e2.EVictim = e.EActor;
          e2.EItem   = e.EItem; // handy for A_DEQU
          e2.EItem2  = e.EItem2;
          e2.vHit    = (int8)e.EVictim->Attr[A_HIT_BRAWL];
          e2.vDef    = (int8)e.EActor->getDef();
          e2.Dmg     = at->u.a.Dmg;
          e2.saveDC = (int8)e.EActor->GetPower(at->u.a.DC);
          e2.AType   = at->AType;
          e2.DType   = at->DType;
          e2.vThreat = 20; 
          e2.vCrit   = 2;
          e2.isNAttack = true;
          if (ReThrow(EV_SATTACK, e2) == ABORT)
            break; // ww: return ABORT;
          if (e2.Died || e2.ADied)
            break;
          }
      }
    }
    if (e.EActor->isDead())
      e.ADied = true;

    // ww: handle Riposte!
    if (!e.ADied && !e.isHit && !e.isSurprise && 
        e.EVictim->isBeside(e.EActor) && e.EVictim->HasFeat(FT_RIPOSTE) && 
        (e.EVictim->ArmourType() == 0 || e.EVictim->ArmourType() & WG_LARMOUR) &&
        e.EVictim->Encumbrance() <= EN_LIGHT) {
      e.EVictim->IPrint("You riposte!");
      e.EActor->IPrint(Format("%s ripostes!",(const char*)e.EVictim->Name(NA_THE)));
      e.EActor->ProvokeAoO(e.EVictim,false);
    }

    if (e.EActor->isDead())
      e.ADied = true;

    /* Combat awakens nearby creatures */
    /* ww: but silently slitting a throat does not -- moving from bedrolls
     * to bedroll and taking out the sleepers is common in high fantasy */
    if ( (e.AType == A_HURL || e.AType == A_FIRE) &&
         (e.EActor->HasFeat(FT_SHOOT_FROM_COVER)) &&
         (!(e.EActor->isBeside(e.EVictim))) &&
         (e.EActor->HasStati(HIDING)) ) 
      ; // shoot from cover -- noise already covered
    else if (e.AType != A_COUP)
      e.EActor->MakeNoise(max(10 - (e.EActor->SkillLevel(SK_MOVE_SIL) / 2),1));        
    
    if (e.EActor->isCharacter())
      {
        int16 N, L; Item *ar;
        N = 10;
        N += (int16)(e.EItem ? e.EItem->Weight()/10 : 10);
        if (e.EItem && e.EItem->HasQuality(IQ_FEATHERLIGHT))
          N = 10;
        
        if (e.EActor->InSlot(SL_READY) && 
            e.EActor->InSlot(SL_READY)->isType(T_SHIELD))
          N += (int16)(e.EActor->InSlot(SL_READY)->Weight()/5);
        ar = e.EActor->InSlot(SL_ARMOUR);
        if (!ar)
          L = 10000;
        else if (ar->isGroup(WG_HARMOUR))
          L = 1000;
        else if (ar->isGroup(WG_MARMOUR))
          L = 2000;
        else if (ar->isGroup(WG_LARMOUR))
          L = 5000;
        
        if (ar && ar->HasQuality(IQ_FEATHERLIGHT))
          L = min(10000,L*3);
          
        e.EPActor->fracFatigue += N;
        if (e.EPActor->fracFatigue >= L)
          {
            e.EActor->IPrint("<3>The fighting fatigues you.<7>");
            e.EPActor->fracFatigue = 0;
            e.EActor->LoseFatigue(1,false);
          }
    
    
    
      }
  
  
  
    return DONE;
  }

bool Creature::ManeuverCheck(EventInfo &e)
  {
    int8 r1, r2, v1, v2, i;
    r1 = (int8)Dice::Roll(1,20,0);
    r2 = (int8)Dice::Roll(1,20,0);
    e.vOpp1 = r1;
    e.vOpp2 = r2;        
        
        
    if ((e.AType == A_TRIP || e.AType == A_DISA) && 
        (e.EActor->AttackMode() == S_MELEE ||
         e.EActor->AttackMode() == S_DUAL))
      v1 = (int8)e.EActor->GetBAB(S_MELEE);
    else 
      v1 = (int8)e.EActor->GetBAB(S_MELEE);
    
    if (e.EVictim->InSlot(SL_WEAPON))
      v2 = (int8)max(e.EVictim->GetBAB(S_MELEE),e.EVictim->GetBAB(S_BRAWL));
    else
      v2 = (int8)e.EVictim->GetBAB(S_BRAWL);

    if (!v1)
      e.strOpp1 = Format("1d20 (%d)",r1);
    else  
      e.strOpp1 = Format("1d20 (%d) %+d base",r1, v1);      
    if (!v2)
      e.strOpp2 = Format("1d20 (%d)",r2);
    else  
      e.strOpp2 = Format("1d20 (%d) %+d base",r2, v2);    
    e.vOpp1 += v1;
    e.vOpp2 += v2;    

    // do we get to use dex instead of STR if we want? 
    bool cf1 = e.EActor->HasFeat(FT_WEAPON_FINESSE), 
         cf2 = e.EVictim->HasFeat(FT_WEAPON_FINESSE); 
    bool size4 = false;  // bull/grab/esca x4 size modifer? 

    switch (e.AType) {
      case A_BULL:
        size4 = true; 
        if (e.EVictim->GetStatiMag(SAVE_BONUS,SN_KNOCKDOWN,NULL)) {
          int amt = e.EVictim->GetStatiMag(SAVE_BONUS,SN_KNOCKDOWN,NULL);
          e.vOpp2 += amt;
          e.strOpp2 += Format(" %+d stable", amt);
          }
        break; 

      case A_GRAB:
        size4 = true; 
        if ((i = (int8)e.EVictim->GetStatiMag(SAVE_BONUS,SN_GRAB))) {
          e.vOpp2 += i;
          e.strOpp2 += Format(" %+d vs. grab",i);
        }
        if (e.EActor->HasFeat(FT_MASTER_GRAPPLE)) {
          e.vOpp1 += 4; e.strOpp1 += " +4 feat"; 
        }
        if (e.EVictim->HasFeat(FT_MASTER_GRAPPLE)) {
          e.vOpp2 += 4; e.strOpp2 += " +4 feat"; 
        }
        break; 

      case A_ESCA: 
        size4 = true; 
        i = e.EActor->SkillLevel(SK_ESCAPE_ART) - 
          // ww: we'll be adding in your dex/str bonus later
          e.EActor->Mod2((int8)SkillAttr(SK_ESCAPE_ART));
        if (i > 0) { 
          e.vOpp1 += i;
          e.strOpp1 += Format(" %+d skill",i);
          }
        if (e.EActor->HasFeat(FT_SLIPAWAY)) {
          e.vOpp1 += 6;
          e.strOpp1 += " +6 slipaway";
          } 
        if ((i = (int8)e.EActor->GetStatiMag(SAVE_BONUS,SN_GRAB))) {
          e.vOpp1 += i;
          e.strOpp1 += Format(" %+d vs. grab",i);
          }
        if (e.EActor->HasFeat(FT_MASTER_GRAPPLE)) {
          e.vOpp1 += 4; e.strOpp1 += " +4 feat"; 
          }
        if (e.EVictim->HasFeat(FT_MASTER_GRAPPLE)) {
          e.vOpp2 += 4; e.strOpp2 += " +4 feat"; 
          }
        break; 

      case A_DISA: 
        cf1 = cf2 = true;
        if (e.EActor->EInSlot(SL_WEAPON)) {
          if (e.EActor->EInSlot(SL_WEAPON)->Size() != SZ_MEDIUM)
            e.strOpp1 += Format(" %+d wepsize", 
                (e.EActor->EInSlot(SL_WEAPON)->Size() - SZ_MEDIUM)*2);
          e.vOpp1 += (e.EActor->EInSlot(SL_WEAPON)->Size() - SZ_MEDIUM)*2;
          if (e.EActor->EInSlot(SL_WEAPON)->HasIFlag(WT_SUPER_DISARM)) { 
            e.vOpp1 += 4; e.strOpp1 += " +4 wep"; 
            }
          }
        if (e.EVictim->EInSlot(SL_WEAPON) && 
            e.EVictim->EInSlot(SL_WEAPON)->Size() != SZ_MEDIUM) {
          e.strOpp2 += Format(" %+d wepsize", (e.EVictim->EInSlot(SL_WEAPON)->Size() - SZ_MEDIUM)*2);
          e.vOpp2 += (e.EVictim->EInSlot(SL_WEAPON)->Size() - SZ_MEDIUM)*2;
          } 
        if (e.EVictim->EInSlot(SL_READY) == e.EVictim->EInSlot(SL_WEAPON)) { 
          e.strOpp2 += " +4 (2H)"; e.vOpp2 += 4; 
          }
        if (e.EActor->HasFeat(FT_MASTER_DISARM)) {
          e.vOpp1 += 4; e.strOpp1 += " +4 feat"; 
          }
        if (e.EVictim->HasFeat(FT_MASTER_DISARM)) {
          e.vOpp2 += 4; e.strOpp2 += " +4 feat"; 
          }
        break; 

      case A_TRIP: 
        cf1 = cf2 = true;
        if (e.EVictim->isMType(MA_QUADRUPED)) { 
          e.vOpp2 += 4; e.strOpp2 += " +4 stable"; 
        }
        if (e.EActor->EInSlot(SL_WEAPON) && 
            e.EActor->EInSlot(SL_WEAPON)->HasIFlag(WT_SUPER_TRIP)) { 
          e.vOpp1 += 4; e.strOpp1 += " +4 wep"; 
        }
        if (e.EActor->HasFeat(FT_MASTER_TRIP)) {
          e.vOpp1 += 4; e.strOpp1 += " +4 feat"; 
        }
        if (e.EVictim->HasFeat(FT_MASTER_TRIP)) {
          e.vOpp2 += 4; e.strOpp2 += " +4 feat"; 
        }
        break; 
    } 

    if (size4) { 
      if (e.EActor->Attr[A_SIZ] != SZ_MEDIUM)
        e.strOpp1 += Format(" %+d size",(e.EActor->Attr[A_SIZ] - SZ_MEDIUM)*4);
      if (e.EVictim->Attr[A_SIZ] != SZ_MEDIUM)
        e.strOpp2 += Format(" %+d size",(e.EVictim->Attr[A_SIZ] - SZ_MEDIUM)*4);
      e.vOpp1 += (e.EActor->Attr[A_SIZ] - SZ_MEDIUM)*4;
      e.vOpp2 += (e.EVictim->Attr[A_SIZ] - SZ_MEDIUM)*4;
      }

    if (cf1 && e.EActor->Mod(A_DEX) > e.EActor->Mod(A_STR)) {
      if (e.EActor->Mod(A_DEX))
        e.strOpp1 += Format(" %+d Dex", (e.EActor->Mod(A_DEX)));
      e.vOpp1 += e.EActor->Mod(A_DEX);
      } 
    else {
      if (e.EActor->Mod(A_STR))
        e.strOpp1 += Format(" %+d Str", (e.EActor->Mod(A_STR)));
      e.vOpp1 += e.EActor->Mod(A_STR);
      } 

    if ((cf2 || e.AType == A_TRIP) && e.EVictim->Mod(A_DEX) > e.EVictim->Mod(A_STR)) {
      if (e.EVictim->Mod(A_DEX))
        e.strOpp2 += Format(" %+d Dex", (e.EVictim->Mod(A_DEX)));
      e.vOpp2 += e.EVictim->Mod(A_DEX);
      } 
    else {
      if (e.EVictim->Mod(A_STR))
        e.strOpp2 += Format(" %+d Str", (e.EVictim->Mod(A_STR)));
      e.vOpp2 += e.EVictim->Mod(A_STR);
      } 

    if (e.vOpp2 >= e.vOpp1)
      e.Resist = true;
    else if (e.AType == A_THRO)
      e.Dmg.Bonus += (e.vOpp1 - e.vOpp2)/3;
    
    /* Remember to put stuff for ESTR_GRAPPLE here when we figure out
       what it should be doing. */
    
    return e.vOpp1 > e.vOpp2;
  }
  
void Creature::UnGrapple()
  {
    /* Later, this become Creature::Ungrapple(), and we need to check for
      multiple grapplers at the same time. */
    if (HasStati(GRAPPLING)) {
      if (GetStatiObj(GRAPPLING)) {
        GetStatiObj(GRAPPLING)->RemoveStati(GRAPPLED,-1,-1,-1,this);
        GetStatiObj(GRAPPLING)->RemoveStati(GRABBED,-1,-1,-1,this);
        }
      RemoveStati(GRAPPLING);
      }
    if (HasStati(GRAPPLED)) {
      if (GetStatiObj(GRAPPLED)) 
        GetStatiObj(GRAPPLED)->RemoveStati(GRAPPLING,-1,-1,-1,this);
      RemoveStati(GRAPPLED);
      }
    if (HasStati(GRABBED)) {
      if (GetStatiObj(GRAPPLED)) 
        GetStatiObj(GRAPPLED)->RemoveStati(GRAPPLING,-1,-1,-1,this);
      RemoveStati(GRABBED);
      }

    if (HasStati(ENGULFED))
      {
        Creature * cr = (Creature*) GetStatiObj(ENGULFED);
        cr->DropEngulfed(this);
      }

    /* ww: these go away as well -- otherwise teleporting away when
     * you're stuck in a spiked chain doesn't un-entangle you ... */
    RemoveStati(STUCK);
    RemoveStati(PRONE);
    RemoveStati(ELEVATED);
  }

EvReturn Creature::Hit(EventInfo &e) /* this == EVictim!! */
{
  EvReturn r; int16 oHP, i;

  oHP = e.EVictim->cHP;

    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
      e.EItem = NULL;


  if (e.AType == A_BULL)
    return BullRush(e);
  if (e.AType == A_BREA || e.AType == A_SPIT || e.AType == A_BRE2)
  {
    TAttack *at;
    r = Blast(e);
    /* Breath weapons perform the A_ALSO effects only if the 
       subject fails the saving throw for half damage. */
    if (r != ABORT && !e.Resist) {                                     
      TAttack buf[1024];
      int max = ListAttacks(buf,1024);
      for (i = 0; i<max; i++)
        if (buf[i].AType == e.AType)
          break;
      if (i < max) { 
        for (i++; i<max; i++) {
          at = &buf[i]; 
          if (at->AType != A_ALSO) break; 
          e.AType = at->AType; 
          e.DType  = at->DType;
          e.Dmg    = at->u.a.Dmg;
          e.saveDC = (int8)e.EActor->GetPower(at->u.a.DC);
          e.EMap->SetQueue(QUEUE_DAMAGE_MSG);
          ReThrow(EV_HIT,e);
          e.EMap->UnsetQueue(QUEUE_DAMAGE_MSG);
        } 
      } 
    }
    return r;
  }

  if (e.AType == A_FIRE && e.EVictim->HasFeat(FT_DEFLECT_ARROWS))
    if (e.EVictim->AoO && !e.isSurprise)
      if (e.EVictim->SavingThrow(REF,15))
      {
        e.EVictim->AoO--;
        TPrint(e,"The <EVictim> deflects your <Obj>!",
            "You deflect the <Obj>!",
            "The <EVictim> deflects the <Obj>!",e.EItem2);
        goto DoneHit;
      }

  if (e.AType == A_FIRE && e.EVictim->HasFeat(FT_MISSILE_PROOF) &&
      e.EVictim->EInSlot(SL_READY) && e.EItem2 && 
      e.EVictim->AoO && 
      e.EVictim->EInSlot(SL_READY)->isType(T_SHIELD) &&
      !e.EVictim->isFlatFooted() && !e.isSurprise) {
    int16 DC; DC = 20;
    DC -= e.EVictim->EInSlot(SL_READY)->ArmVal(e.DType,false);
    DC += e.EItem2->GetPlus();
    if (e.EVictim->SavingThrow(REF,DC)) {
      e.EVictim->AoO--; 
      TPrint(e,"The <EVictim> deflects your <Obj> with a shield!",
          "You deflect the <Obj> your shield!",
          "The <EVictim> deflects the <Obj> with a shield!",e.EItem2);
      goto DoneHit;
    }
  }

  if (e.EVictim->HasStati(IMMUNITY,e.DType) /*|| 
                                              (TMON(e.EVictim->mID)->Imm & */) {
      e.Immune = true;
      goto AfterEffects;
      }

      TMonster *tm; tm = TMON(mID);

      if (e.EActor->HasFeat(FT_POWER_ATTACK) && e.EActor->GetAttr(A_STR) >= 13)
      if (e.DType == AD_SLASH || e.DType == AD_PIERCE || e.DType == AD_BLUNT)
      {
      if (e.EActor->EInSlot(SL_WEAPON) &&
        !((Weapon *)e.EActor->EInSlot(SL_WEAPON))->useStrength())
      ;
      else { 
      i = (e.vHit+e.vRoll) - e.vDef;
      if (e.AType == A_SWNG && (e.EActor->EInSlot(SL_WEAPON) == e.EActor->EInSlot(SL_READY)))
      i /= 2;
      else
      i /= 3;
      i = min(i,e.EActor->Mod2(A_STR));  
      if (i) {
        e.bDmg += i;
        e.strDmg += Format(" %+d PA",i);
      }
      }
      }

  if (e.EActor->HasFeat(FT_AIMED_SHOT) && e.EActor->GetAttr(A_DEX) >= 13)
    if (e.AType == A_FIRE || e.AType == A_HURL) 
    {
      int16 limit = e.EActor->Mod2(A_DEX);
      i = (e.vHit+e.vRoll) - e.vDef;
      i /= 3;
      if (e.EActor->HasFeat(FT_ZEN_ARCHERY))
        limit += e.EActor->Mod2(A_WIS);
      i = min(i,limit);
      if (i) {
        e.bDmg += i;
        e.strDmg += Format(" %+d AS",i);
      }
    }


  if (e.isCrit) {
    e.vDmg = 0;
    if (e.EVictim->HasFeat(FT_ROLL_WITH_IT)) {
      int32 vn, vb;
      vn = e.Dmg.Number;
      vb = e.Dmg.Bonus;
      vn = vn * (5 + e.vCrit*5);
      vb = vb * (5 + e.vCrit*5);
      e.Dmg.Number = (int8)max(2,vn/10);
      e.Dmg.Bonus  = (int8)(vb / 10);
      }
    else {
      e.Dmg.Number *= e.vCrit;
      e.Dmg.Bonus  *= e.vCrit; 
      }
    e.vDmg = e.Dmg.Roll() + 
               e.bDmg * e.vCrit;
    }
  else
    e.vDmg = e.Dmg.Roll() + e.bDmg;
  
  e.vDmg = max(1,e.vDmg);
  ASSERT(e.vDmg > 0)


  if (GetStatiMag(CHARGING)) {
    int32 mult = 1 + ( (HasFeat(FT_SPIRITED_CHARGE) && HasStati(MOUNTED)) ||
                       (HasFeat(FT_FLYING_KICK) && e.AType == A_KICK) );
    if (e.EItem && e.EItem->HasIFlag(WT_CHARGE)) mult++;
    e.vDmg *= (int16)mult; 
  } 
  {
    Creature * rider = (Creature *)GetStatiObj(MOUNT);
    if (rider && rider->GetStatiMag(CHARGING)) {
      int32 mult = 1; 
      if (e.EItem && e.EItem->HasIFlag(WT_CHARGE)) mult++;
      e.vDmg *= (int16)mult; 
    } 
  }

  if (e.isAoO && e.isMoveAoO && e.EVictim->GetStatiMag(CHARGING)) { 
    int32 mult = 1; 
    if (e.EItem && e.EItem->HasIFlag(WT_CHARGE)) {
      mult++;
      e.vDmg *= (int16)mult; 
      e.vicCharging = true; 
      // vic will be dead by AttackMsg, so record this mult now! 
    }
  } 

  if (!e.saveDC) {
    if (e.EActor)
      e.saveDC = 10 + e.EActor->ChallengeRating();

    else
      e.saveDC = 15;
  }

  if (e.AType == A_SUND) {
    bool isSunderingWeapon = e.EItem->HasQuality(WQ_SUNDERING); 
    if (!isSunderingWeapon && e.EItem->GetPlus() < e.EItem2->GetPlus()) {
      TPrint(e,"Your <EItem> is ineffective against the <EVictim>'s <EItem2>!",
          "The <EActor>'s <EItem> is ineffective against your <EItem2>!",
          "The <EActor>'s <EItem> is ineffective against the <EVictim>'s <EItem2>!");
      return DONE;
    }
    oHP = e.EItem2->GetHP();
    // ww: let's try this out! 
    if (e.EActor->HasFeat(FT_SUNDER)) {
      e.vDmg *= 2; 
      e.strDmg += " x2 Sunder"; 
    }
    if (isSunderingWeapon) { 
      e.vDmg *= 3; 
      e.strDmg += " x3 Sundering"; 
    }
    ThrowDmg(EV_DAMAGE,e.DType,e.vDmg,NULL,e.EActor,e.EItem2,e.EItem);
    if (e.EItem2->GetHP() >= oHP && !(e.EItem2->isDead()))
      VPrint(e,"Your <EItem2> is unharmed!",
          "The <EVictim>'s <EItem2> is unharmed!");
    else if (e.EActor->HasFeat(FT_CLEAVE_ASUNDER) && 
        (e.EItem2->GetHP() <= 0 || e.EItem2->isDead())) {
      /* WW: FIXME! absolutely nothing I can do, no combination of
       * SetQueue, e.Terse, nothing, will get this message to print out.
       * */
      DPrint(e,"You cleave through after sundering the <Obj>.",
          "The <EActor> cleaves through after sundering the <Obj>.",
          e.EItem2);
      EventInfo e2 = e;
      e2.isCleave = true;
      e2.Event = EV_WATTACK;
      ReThrow(EV_WATTACK,e2);
    } 
    return DONE;
  }

  if (e.AType != A_TUCH || e.EActor->HasAttk(A_TUCH))
    r = ReThrow(EV_DAMAGE,e);

AfterEffects:
  
    
  if (e.AType == A_TUCH && e.EActor->HasStati(TOUCH_ATTACK)
                        && !e.EVictim->isDead())
  {
      int8 ii;
    e.eID = e.EActor->GetStatiEID(TOUCH_ATTACK);
    for(ii=0;;ii++) {
        ASSERT(TEFF(e.eID)->Vals(ii))
        if (TEFF(e.eID)->Vals(ii)->aval == AR_TOUCH)
          break;
    }
    e.efNum = ii;
    e.isSpell = true; 
    ReThrow(EV_MAGIC_STRIKE,e);

    /* If this touch spell allows multiple uses, reduce the
       number remaining by one; otherwise, get rid of the 
       TOUCH_ATTACK Stati completely. */
    if (e.EActor->GetStatiMag(TOUCH_ATTACK) > 1)
      e.EActor->SetStatiMag(TOUCH_ATTACK,-1,NULL,
          e.EActor->GetStatiMag(TOUCH_ATTACK)-1);
    else
      e.EActor->RemoveStati(TOUCH_ATTACK);
  }  

  if (e.AType == A_SWNG && e.EActor->HasAttk(A_TUCH) && 
         e.EActor->HasMFlag(M_INCOR))
    {
      TAttack *at = e.EActor->GetAttk(A_TUCH);
      EventInfo e2;
      e2 = e;
      e2.AType  = A_TUCH;
      e2.DType  = at->DType;
      e2.Dmg    = at->u.a.Dmg;
      e2.saveDC = at->u.a.DC;
      ReThrow(EV_HIT,e2);
    }

  // ww: the old logic was preventing poisoned arrows from ever having
  // any effect!
  if (oHP > e.EVictim->cHP && !e.EVictim->isDead()) {
    for (i=0;i<2;i++) {
      Item * wep = i ? e.EItem2 : e.EItem;
      if (wep) {
        StatiIterNature(wep,POISONED)
            if (e.EVictim->isDead()) break; 

            rID poisonID = S->eID;

            if (wep->HasQuality(WQ_VENOM))
            ; // never wears off! 
            else if (S->Mag <= 1)
              { StatiIter_RemoveCurrent(wep); }
            else if (S->Mag > 1) 
              S->Mag--;

            // victim can make a saving throw -- note that this is
            // specifically not done later on by any of the effect handling
            // code for poison, and that poisons use sval for save DC and not
            // for save type
            int resistLevel = e.EVictim->ResistLevel(AD_POIS);
            if (resistLevel == -1) {
            VPrint(e,"You are immune to the <Res> on the <Obj>.",
              "The <EVictim> is immune to the <Res> on the <Obj>.",
              poisonID, wep);
            break; 
            }
            int saveDC = (TEFF(poisonID)->ef.sval +
                // ww: let's have that poison-use skill do something: the
                // better with it you are, the better you are able to coat
                // your blade in the right places or whatnot and make it
                // hurt the other guy
                (e.EActor->SkillLevel(SK_POISON_USE) / 3)) -
              resistLevel;
            if (e.EVictim->SavingThrow(FORT,saveDC,SA_POISON)) {
              VPrint(e,"You resist the <Res> on the <Obj>.",
                  "The <EVictim> resists the <Res> on the <Obj>.",
                  poisonID, wep);
              continue;
            }
            //            ThrowEff(EV_EFFECT,poisonID,e.p[0].o,e.p[1].o,e.p[2].o,e.p[3].o);
            e.EVictim->GainPermStati(POISONED,NULL,SS_ATTK, 0, 0, poisonID);
        StatiIterEnd(wep)
      } 
    }
  }

  if (e.EItem)
    if (e.EItem->isType(T_WEAPON) || e.EItem->isType(T_BOW))
      e.EWeapon->QualityDmg(e);
  if (e.EItem2 && e.AType != A_SUND)
    if (e.EItem2->isType(T_WEAPON) || e.EItem2->isType(T_MISSILE))
      ((Weapon*)e.EItem2)->QualityDmg(e);

  /* Moved from QualityDmg to account for the Knock Prone feat. The save DC
     is based on the weapon, or 10 + Mod(STR) + 1/2 BAB if there is no weapon,
     +4 if the attacker as Knock Prone feat. */
  if (!e.EVictim->isDead())
    {
      bool knockProne = e.EActor->HasFeat(FT_KNOCK_PRONE);
      if (knockProne && e.EItem && ! ((Weapon *)e.EItem)->useStrength())
        knockProne = false; 
      // hitting someone with a no-strength weapon (like a bow) won't knock
      // someone down
      if ((e.EItem  &&  e.EItem->HasIFlag(WT_KNOCKDOWN)) ||
          (e.EItem2 && e.EItem2->HasIFlag(WT_KNOCKDOWN)))
        knockProne = true; 
      if (knockProne &&
          (e.isCrit || !random(3) || e.isGreatBlow || e.AType == A_HURL)) {
        int DC = e.EActor->WeaponSaveDC(e.EItem ? e.EItem : e.EItem2,
            WT_KNOCKDOWN) +
          ((e.EActor->HasFeat(FT_KNOCK_PRONE)) ? 4 : 0);
        ThrowDmg(EV_DAMAGE,AD_TRIP,DC,"tripping attack",
            e.EActor,e.EVictim,e.EItem,e.EItem2);
        } 
    }

    /* fjm: I want to keep this to punches and kicks. Lizardmonks can
      now toggle back and forth between monk attacks and lizard attacks,
      and stunning with claws just doesn't make conceptual sense to me
      the way stunning with fists does. */
    if ((e.AType == A_PUNC || e.AType == A_KICK) && !e.EVictim->isDead())
      if (e.EActor->HasAbility(CA_STUN_ATTACK))
        if (e.isCrit || !random(3) || e.isGreatBlow)
          if (e.EVictim->isMType(MA_LIVING) && e.EVictim->ResistLevel(AD_STUN) != -1)
            if (!e.EVictim->SavingThrow(FORT, e.EActor->StunAttackDC(),SA_STUN))
            {
              VPrint(e,"You are stunned by the blow!",
                  "The <EVictim> reels!");
              e.EVictim->GainTempStati(STUNNED,NULL,Dice::Roll(1,4)+2,SS_ATTK);
              if (e.EActor->HasFeat(FT_PAIN_TOUCH) &&
                    e.EVictim->HasMFlag(M_HUMANOID) &&
                    e.AType == A_PUNC)
                {
                  VPrint(e,"You body is shot through with sheer agony!",
                          "The <EVictim> howls in agony!");
                  e.EVictim->GainTempStati(NAUSEA,NULL,2,SS_ATTK);
                }
            }

    if ((e.AType == A_PUNC) && !e.EVictim->isDead())
      if (e.EActor->HasFeat(FT_FALLING_STAR_STRIKE))
        if (e.isCrit || !random(3) || e.isGreatBlow)
          if (e.EVictim->isMType(MA_LIVING) && e.EVictim->ResistLevel(AD_BLND) != -1
                && e.EVictim->HasMFlag(M_HUMANOID) && !e.EVictim->HasMFlag(M_BLIND))
            if (!e.EVictim->SavingThrow(FORT, e.EActor->StunAttackDC(),SA_PARA))
              {
                VPrint(e,"The <EActor>'s strike blinds you!",
                    "The <EVictim> is blinded!");
                e.EVictim->GainTempStati(BLIND,NULL,Dice::Roll(3,6),SS_ATTK);
              }

    if ((e.AType == A_PUNC) && !e.EVictim->isDead())
      if (e.EActor->HasFeat(FT_FREEZE_THE_LIFEBLOOD))
        if (e.EActor->ChallengeRating() - 2 >= e.EVictim->ChallengeRating())
          if (e.isCrit || !random(3) || e.isGreatBlow)
            if (e.EVictim->isMType(MA_LIVING) && e.EVictim->ResistLevel(AD_PLYS))
              if (!e.EVictim->SavingThrow(FORT, e.EActor->StunAttackDC(),SA_PARA))
                {
                  VPrint(e,"The <EActor>'s nerve strike paralyzes you!",
                      "The <EVictim> is paralyzed!");
                  e.EVictim->GainTempStati(PARALYSIS,NULL,Dice::Roll(1,4)+2,SS_ATTK);
                }
    
    if ((e.AType == A_PUNC || e.AType == A_KICK) && !e.EVictim->isDead())
      if (e.EActor->HasFeat(FT_CHI_STRIKE))
        if (e.EVictim->isMType(MA_LIVING) && e.EVictim->cMana())
          if (!e.EVictim->SavingThrow(FORT, e.EActor->StunAttackDC(),0))
            {
              int16 lMana;
              lMana = Dice::Roll((int8)e.vDmg,8);
              e.EVictim->LoseMana(lMana,true);
              VPrint(e,"Mana bleeds out of your chi gates!",
                  "Mana bleeds out of the <EVictim>'s chi gates!");
            }

    if (e.isCleave && e.EActor->HasFeat(FT_SUPREME_CLEAVE))
    {
      i = Dice::Roll(1,8); e.xDmg += i; e.strXDmg += " +1d8 SC";
      ThrowDmg(EV_DAMAGE,e.DType,i,e.EActor->Name(NA_A),e.EVictim,e.EItem);


    }
    /* Why isn't this in QualityDmg? So that is shows up in strXDmg even 
      if it isn't necessary to kill the creature, to remind the player
      that he is getting his mighty cleaving damage. Also, so that it
      shows up parallel with Supreme Cleave (i.e. "+1d8 SC +1d8 MC") for
      aesthetics. */
    /* ww: let's try something a little more powerful: right now
    * WQ_CLEAVING is disappointing! Cleaves happen so rarely ... 
    if (e.isCleave && e.EItem && e.EItem->HasQuality(WQ_CLEAVING)) 
    {
    i = Dice::Roll(1,8); e.xDmg += i; e.strXDmg += " +1d8 MC";
    ThrowDmg(EV_DAMAGE,e.DType,i,e.EActor->Name(NA_A),e.EVictim,e.EItem,e.EItem2);              
    }

    */

    /* When small barbarians rage, they throw their foes off balance,
      granting them a culmulative circumstance penalty of -1 per hit,
      for 1d4+1 rounds per hit. */ 
    if (e.EActor->isSmallRace() && e.EActor->HasStati(RAGING) && !e.EVictim->isDead())
      if (e.EActor->Mod(A_CHA)+Dice::Roll(1,20) > e.EVictim->Mod(A_WIS)+11)
        {
          int16 pen, dur;
          pen = 1;
          /* This code assumes that this is the only way you
            can get a ADJUST_CIRC/A_AID/SS_ATTK Stati, but
            given that effects don't use SS_ATTK and very
            little else gives ADJUST_CIRC, I think that's fine. */
          dur = Dice::Roll(1,4,1);
          
          StatiIter(e.EVictim)
            if (S->Nature == ADJUST_CIRC && 
                S->Val == A_AID && S->Mag < 0 &&
                S->Source == SS_ATTK)
              {
                pen = abs(S->Mag) + 1;
                dur += S->Duration;
              }
          StatiIterEnd(e.EVictim)
          
          /* The maximum penalty a multi-class small barbarian
            can impose is equal to their barbarian levels, while
            single-classed barbarians are unrestricted. */
          if (e.EActor->isCharacter())
            if (e.EPActor->Level[1])
              pen = min(pen,e.EActor->AbilityLevel(CA_BERSERK_RAGE));
          
          e.EVictim->RemoveStati(ADJUST_CIRC,SS_ATTK,A_AID);
          e.EVictim->GainTempStati(ADJUST_CIRC,NULL,dur,SS_ATTK,A_AID,-pen,0,0);
          TPrint(e,
            "Your frenzy throws the <EVictim><Str1> off balance.",
            "The <EActor>'s frenzy throws you<Str1> off balance.",
            "The <EActor>'s frenzy throws the <EVictim><Str1> off balance.",
            pen == 1 ? "" : " more");
        }
      
    if (HasAbility(CA_SNEAK_ATTACK) && !e.EVictim->isDead() && !HasStati(GRABBED)
          && !HasStati(GRAPPLED) && (e.EVictim->ResistLevel(AD_CRIT) != -1 ||
          (e.EVictim->isMType(MA_UNDEAD) && e.EActor->HasFeat(FT_NECROPHYSIOLOGY))))
      if ((e.isSurprise || e.isFlatFoot || e.isFlanking || 
            e.EVictim->noDexDefense() ||
            (e.isAoO && e.EActor->HasFeat(FT_COMBAT_OPPORTUNIST))
          ) && (e.EActor->DistFrom(e.EVictim) <= 3))
      {
        Dice saDmg; 
        saDmg.Bonus = 0;
        saDmg.Number = (int8)AbilityLevel(CA_SNEAK_ATTACK);
        saDmg.Sides  = /* e.EActor->HasFeat(FT_MARTIAL_MASTERY) ||
                          e.EActor->HasStati(POLYMORPH) ? */ 6 /* : 4*/;

          if (e.EItem2)
            if (e.EItem2->isType(T_WEAPON) || e.EItem2->isType(T_MISSILE))
              saDmg.Sides = TITEM(e.EItem2->iID)->HasFlag(WT_SUPER_SNEAK) ? 8 : 6;
        if (e.EItem)
          if (e.EItem->isType(T_WEAPON) || e.EItem->isType(T_BOW))
            saDmg.Sides = max(saDmg.Sides,TITEM(e.EItem->iID)->HasFlag(WT_SUPER_SNEAK) ? 8 : 6);
        i = saDmg.Roll(); 
        i -= e.EVictim->AbilityLevel(CA_NOBLE_AEGIS)*2;
        if (e.EVictim->AbilityLevel(CA_NOBLE_AEGIS))
          e.strXDmg += Format(" +%s-%d SA",(const char*)saDmg.Str(),
              e.EVictim->AbilityLevel(CA_NOBLE_AEGIS)*2); 
        else
          e.strXDmg += Format(" +%s SA",(const char*)saDmg.Str()); 
        if (i > 0) {
          e.xDmg += i;
          // ww: let's try making sneak attack damage 'AD_NORM' --
          // otherwise the other guy's armour tends to block it ... 
          // which makes life tricky for the low-level rogue ...
          ThrowDmg(EV_DAMAGE,AD_NORM,i,XPrint("an <Obj>'s sneak attack",e.EActor),
              e.EActor,e.EVictim,e.EItem,e.EItem2);
          if ((e.EItem && e.EItem->HasQuality(WQ_NIGHT) ||
                (e.EItem2 && e.EItem2->HasQuality(WQ_NIGHT)))) {
            int16 DC; DC = 20 - e.EVictim->ResistLevel(AD_SLEE,false);
            if (DC <= 20 && DC > 0 && !e.EVictim->isDead() && 
                !e.EVictim->SavingThrow(WILL,DC,SA_MAGIC))
              // ww: should be two hours
              VPrint(e,"You are magically put to sleep!",
                  "The <EVictim> is magically put to sleep!");
            e.EVictim->GainTempStati(ASLEEP,NULL,12,SS_ATTK,0);
            }
          e.isSneakAttack = true; 
        }
      }

    // ww: dirty fighting ... I'm unclear about the wording, I think
    // perhaps this is only supposed to work on each monster once. That
    // would be quite a bit of bookkeeping and it's a weak effect anyway. 
    // FJM: back to old version due to general weakening of overpowered
    // feats, c.f., Improved Attribute to +1.
    if (e.EActor->HasFeat(FT_DIRTY_FIGHTING) && e.AType != A_FIRE &&
        e.EVictim->isMType(MA_LIVING) && (e.EActor->GetAttr(A_WIS) >
          e.EVictim->GetAttr(A_WIS)) && !e.EVictim->isDead()) {
      if (e.EActor->isPlayer()) {
        switch (e.EPActor->Opt(OPT_DIRTY_FIGHTING))
          {
            case 0:
              goto SkipDirty;
            case 2:
              if (e.EPActor->desiredAlign & AL_LAWFUL)
                goto SkipDirty;
             break;
          }
        }
      else if (e.EActor->getAlignment() & AL_LAWFUL)
        goto SkipDirty;
      if (e.EVictim->HasStati(TRIED,FT_DIRTY_FIGHTING,e.EActor))
        goto SkipDirty;
      
      e.EVictim->GainTempStati(TRIED,e.EActor,Dice::Roll(3,6),SS_MISC,
                                FT_DIRTY_FIGHTING,0,0,0);
      Dice dfDmg; 
      dfDmg.Bonus = 0;
      dfDmg.Number = 1; 
      dfDmg.Sides  = 8; 
      i = dfDmg.Roll(); 
      // ww: just for fun and flavour, let's have Noble Aegis block Dirty
      // Fighting
      i -= e.EVictim->AbilityLevel(CA_NOBLE_AEGIS);
      if (e.EVictim->AbilityLevel(CA_NOBLE_AEGIS))
        e.strXDmg += Format(" +%s-%d DF",(const char*)dfDmg.Str(),
            e.EVictim->AbilityLevel(CA_NOBLE_AEGIS)); 
      else
        e.strXDmg += Format(" +%s DF",(const char*)dfDmg.Str()); 
      if (i > 0) { 
        e.xDmg += i;
        ThrowDmg(EV_DAMAGE,e.DType,i,XPrint("an <Obj>'s dirty fighting",
              e.EActor), e.EActor,e.EVictim,e.EItem,e.EItem2);
        if (!e.EVictim->isDead() && !e.EVictim->HasMFlag(M_BLIND))
        if (!e.EVictim->SavingThrow(REF,10 + e.EActor->GetBAB(e.EItem ? 
              S_MELEE : S_BRAWL) / 2 + e.EActor->Mod(A_WIS) - 
              e.EVictim->AbilityLevel(CA_NOBLE_AEGIS))) {
          e.EVictim->GainTempStati(BLIND,NULL,Dice::Roll(1,4,1),SS_ATTK);
          TPrint(e, "You briefly blind the <EVictim>!",
                    "The <EActor> briefly blinds you!",
                    "The <EActor> briefly blinds the <EVictim>!");
          }
      } 
      SkipDirty:;
    }

    if (e.isCrit && !(e.EVictim->isDead()))
      e.EVictim->LoseFatigue(1,false);

    // ww: see ChargeBonus() for comments about breaking lances
    if (e.isHit && e.EItem && e.EActor->HasStati(CHARGING)) {
      int32 bonus = e.EActor->ChargeBonus(); 
      if (bonus > 0 && !random(4)) 
        ThrowDmg(EV_DAMAGE,AD_BLUNT,(int16)bonus,"the strain of being used in a charge",e.EActor,e.EItem);
    } 

  DoneHit:

    
    if (e.EActor->HasStati(CHARGING) && e.EVictim->HasFeat(FT_LIVING_WALL) && !e.EVictim->isDead())
      if (e.aDmg <= (e.EVictim->mHP + e.EVictim->GetAttr(A_THP))/4)
        if (!e.EVictim->HasStati(STUNNED) && !e.EVictim->HasStati(PRONE))
          {
            TPrint(e,"When you strike the <EVictim>, <he:EVictim> holds <his:EVictim> ground "
                      "and you are knocked from your feet -- it's like striking "
                      "a living wall!",
                    "When the <EActor> strikes you, you hold your ground and <he:EActor> "
                      "is knocked from <his:EActor> feet -- it's like striking "
                      "a living wall!",
                    "When the <EActor> strikes the <EVictim>, <he:EVictim> holds "
                      "<his:EVictim> ground and the <EActor> is knocked from <his:EActor> "
                      "feet -- it's like striking a living wall!");
            e.EActor->GainPermStati(PRONE,NULL,SS_ATTK);
            if (!e.EActor->SavingThrow(FORT,15 + e.EVictim->Mod(A_STR) + 
                                            e.EVictim->GetBAB(S_MELEE)))
              e.EActor->GainTempStati(STUNNED,NULL,max(1,e.EVictim->Mod(A_STR)),SS_ATTK);          
          }
    

    if (e.AType == A_SWNG || e.AType == A_FIRE)
      if (e.EItem && e.EItem->swingCount < 30 && 
              !e.EItem->isKnown(KN_MAGIC|KN_CURSE|KN_PLUS)) 
        if (e.EActor->HasAbility(CA_LORE_OF_ARMS))
        {
          int8 nSwings = NeededSwings [ min(9,
            e.EActor->AbilityLevel(CA_LORE_OF_ARMS)) ];
          e.EItem->swingCount++;
          if (e.EItem->swingCount > nSwings)
            {
              e.EItem->MakeKnown(KN_MAGIC|KN_CURSE|KN_PLUS);
              if (e.EItem->isMagic())
                Exercise(A_INT,random(8)+e.EItem->ItemLevel(),EINT_IDENT,75); 
              e.EItem->Inscrip.Empty();
              if (TITEM(e.EItem->iID)->HasFlag(IT_PLURAL))
                IPrint("These must be <Str>!",(const char*)e.EItem->Name(NA_MECH));
              else
                IPrint("This must be <Str>!",(const char*)e.EItem->Name(NA_A|NA_MECH));
            }      
        }
    if (e.AType == A_FIRE)
      if (e.EItem2 && e.EItem2->swingCount < 30 && 
              !e.EItem2->isKnown(KN_MAGIC|KN_CURSE|KN_PLUS)) 
        if (e.EActor->HasAbility(CA_LORE_OF_ARMS))
        {
          int8 nSwings = NeededSwings [ min(9,
            e.EActor->AbilityLevel(CA_LORE_OF_ARMS)) ];
          e.EItem2->swingCount++;
          if (e.remainingAmmo)
            e.remainingAmmo->swingCount++;
          
          if (e.EItem2->swingCount > nSwings)
            {
              e.EItem2->MakeKnown(KN_MAGIC|KN_CURSE|KN_PLUS);
              if (e.EItem2->isMagic())
                Exercise(A_INT,random(8)+e.EItem2->ItemLevel(),EINT_IDENT,75); 
              e.EItem2->Inscrip.Empty();
              if (e.remainingAmmo) {
                e.remainingAmmo->MakeKnown(KN_MAGIC|KN_CURSE|KN_PLUS);
                e.remainingAmmo->Inscrip.Empty();
                }
              Item *it = e.remainingAmmo ? e.remainingAmmo : e.EItem;
              if (TITEM(it->iID)->HasFlag(IT_PLURAL))
                IPrint("These must be <Str>!",(const char*)it->Name(NA_MECH));
              else
                IPrint("This must be <Str>!",(const char*)it->Name(NA_A|NA_MECH));
            }      
        }
        
      Item *ar = e.EVictim->InSlot(SL_ARMOUR);
      if (ar && ar->swingCount < 30 && !ar->isKnown(KN_MAGIC|KN_CURSE|KN_PLUS)) 
        if (e.EVictim->AbilityLevel(CA_LORE_OF_ARMS) >= 3)
        {
          int8 nSwings = NeededSwings [ min(9,
            e.EVictim->AbilityLevel(CA_LORE_OF_ARMS) - 2) ];
          
          ar->swingCount++;
          if (ar->swingCount > nSwings)
            {
              ar->MakeKnown(KN_MAGIC|KN_CURSE|KN_PLUS);
              if (ar->isMagic())
                e.EVictim->Exercise(A_INT,random(8)+ar->ItemLevel(),EINT_IDENT,75); 
              ar->Inscrip.Empty();
              if (TITEM(ar->iID)->HasFlag(IT_PLURAL))
                e.EVictim->IPrint("These must be <Str>!",(const char*)ar->Name(NA_MECH));
              else
                e.EVictim->IPrint("This must be <Str>!",(const char*)ar->Name(NA_A|NA_MECH));
            }      
        }

      Item *sh = e.EVictim->InSlot(SL_READY);
      if (sh && sh->swingCount < 30 && sh->isType(T_SHIELD) && !sh->isKnown(KN_MAGIC|KN_CURSE|KN_PLUS)) 
        if (e.EVictim->AbilityLevel(CA_LORE_OF_ARMS) >= 3)
        {
          int8 nSwings = NeededSwings [ min(9,
            e.EVictim->AbilityLevel(CA_LORE_OF_ARMS) - 2) ];
          
          sh->swingCount++;
          if (sh->swingCount > nSwings)
            {
              sh->MakeKnown(KN_MAGIC|KN_CURSE|KN_PLUS);
              if (sh->isMagic())
                e.EVictim->Exercise(A_INT,random(8)+sh->ItemLevel(),EINT_IDENT,75);
              sh->Inscrip.Empty();
              if (TITEM(sh->iID)->HasFlag(IT_PLURAL))
                e.EVictim->IPrint("These must be <Str>!",(const char*)sh->Name(NA_MECH));
              else
                e.EVictim->IPrint("This must be <Str>!",(const char*)sh->Name(NA_A|NA_MECH));
            }      
        }

    if (isCharacter() && e.isCrit)
      {
        if (e.EItem && (e.EItem->isGroup(WG_LIGHT|WG_DAGGERS|WG_FLEXIBLE) ||
              e.AType == A_FIRE || e.AType == A_HURL))
          Exercise(A_DEX,random(6)+max(0,e.EVictim->ChallengeRating() - 
                                        e.EActor->ChallengeRating()),EDEX_CRIT,75);
        if (e.EItem && e.EItem->Size() > GetAttr(A_SIZ))
          Exercise(A_STR,random(8)+max(0,e.EVictim->ChallengeRating() - 
                                        e.EActor->ChallengeRating()),ESTR_CRIT,75);
        else if (e.EItem && e.EItem->Size() == GetAttr(A_SIZ) && !e.EItem->isGroup(WG_LIGHT)
                  && !(InSlot(SL_READY) && InSlot(SL_READY)->isType(T_WEAPON)))
          Exercise(A_STR,random(4)+max(0,e.EVictim->ChallengeRating() - 
                                        e.EActor->ChallengeRating()),ESTR_CRIT,75);
      }
    if (e.AType == A_HURL && e.EItem2 && e.EItem2->Size() <= SZ_SMALL && !random(3))
      Exercise(A_DEX,1,EDEX_THROWN,75);
    if (e.EVictim->isCharacter() && !e.EVictim->isDead() && e.isCrit)
      if (e.EVictim->ChallengeRating() <= e.EActor->ChallengeRating())
        {
          Exercise(A_LUC,random(30)+1,ELUC_CRIT,60);
          Exercise(A_CON,random(12)+1,ECON_CRIT,40);
        }
    
    if (e.EVictim->HasStati(DEFENSIVE) && !random(3))
      e.EVictim->Exercise(A_DEX,1,EDEX_DEFENSIVE,40);

    if (e.EItem && e.EItem->HasStati(MASTERWORK))
      {
        if (e.EItem->HighStatiMag(MASTERWORK) <= 1) {
          e.EItem->RemoveStati(MASTERWORK);
          e.EActor->IPrint("Your blade no longer cuts as effortlessly.");
          }
        else
          e.EItem->IncStatiMag(MASTERWORK,SS_MISC,-1,NULL,-1);
      }
        
      

    return DONE;
  }

EvReturn Creature::Miss(EventInfo &e)
	{
    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
      e.EItem = NULL;

	  if (e.EVictim->HasFeat(FT_DEFENSIVE_THROW))
	    if (e.vRoll + e.vHit + 8 < e.vDef)
	      if (e.EVictim->AoO && !e.EVictim->InSlot(SL_WEAPON))
	        if (e.EActor->HasMFlag(M_HUMANOID) && 
	              !e.EActor->HasStati(PRONE) &&
	              !e.EVictim->HasMFlag(M_NOLIMBS))
	          {
	            int16 time; 
	            e.EVictim->AoO--;
	            time = e.EVictim->Timeout;
	            ThrowVal(EV_SATTACK,A_TRIP,e.EVictim,e.EActor);
	            e.EVictim->Timeout = time;
	            if (e.EActor->HasStati(PRONE) && 
	                e.EVictim->HasFeat(FT_GREAT_THROW) &&
	                !e.EActor->isDead())
	              ThrowDmg(EV_DAMAGE,AD_KNOC,Dice::Roll(1,3,
	                max(0,e.EVictim->Mod(A_STR))), "Great Throw", e.EVictim, e.EActor);
  	        }

		return DONE;
}

EvReturn Creature::Damage(EventInfo &e) {
    int8 subtype = 0, lv, at, Percent; 
    uint8 Col;
    int16 n, c, EDType;
    EvReturn r;
    rID xID;
    String why;
    Item *it = NULL;
    bool coll;

    if (e.EVictim->isDead())
        return ABORT;

    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
        e.EItem = NULL;

    if (e.EActor == e.EVictim)
        goto IgnoreMorality;
    if (e.EVictim->HasStati(TRANSFORMED))
        goto IgnoreMorality;
    if (e.isActOfGod)
        goto IgnoreMorality;
    if (e.EActor->isMonster())
        if (e.EVictim->isPlayer())
            goto IgnoreMorality;

    if (e.AType == A_GAZE && !e.wasFriendly)
        goto IgnoreMorality;

    if (e.wasFriendly) {
        int16 mag;
        if (e.DType == AD_NECR && e.EVictim->isMType(MA_UNDEAD))
            goto IgnoreMorality;
        if (e.EVictim->ResistLevel(e.DType) == -1)
            goto IgnoreMorality;
        if (!e.EVictim->isMType(MA_SAPIENT))
            if (!(isMType(MA_ANIMAL) && e.EActor->LevelAs(FIND("Druid"))))
                goto IgnoreMorality;

        mag = e.EVictim->isDead() ? 3 : 1;
        if (e.EVictim->ChallengeRating()+4 < e.EActor->ChallengeRating())
            mag++;
        if (e.EVictim->isMType(MA_GOOD))
            mag++;
        if (e.isSurprise)
            mag++;
        if (e.DType == AD_SLASH || e.DType == AD_PIERCE || e.DType == AD_BLUNT)
            mag++;
        if (e.EActor->Perceives(e.EVictim))
            mag++;

        /* Allowance for clipping people with area spells for tactical reasons. */
        coll = false;
        if (e.EMagic && (e.EMagic->aval == AR_BALL || e.EMagic->aval == AR_BEAM || e.EMagic->aval == AR_BREATH || e.EMagic->aval == AR_GLOBE || e.EMagic->aval == AR_CHAIN)) {
            mag = max(1, mag/2);
            coll = true;
        }

        e.EActor->AlignedAct(AL_NONGOOD|AL_EVIL,mag,
            coll ? "collateral damage" : "unprovoked attack",
            e.EVictim);
        if (isLegalPersonTo(e.EActor,e.EVictim))
            e.EActor->AlignedAct(AL_NONLAWFUL,mag,
            "unlawful attack");
    }

    if ((e.EItem && e.EItem->HasStati(POISONED)) || (e.EItem2 && e.EItem2->HasStati(POISONED)) || e.DType == AD_POIS)
        if (!e.EActor->isMType(MA_DROW))
            e.EActor->AlignedAct(AL_NONLAWFUL,2,"using poison");

    if (int16 breach = getChivalryBreach(e))
        e.EActor->AlignedAct(breach, 1,
            (breach == (AL_NONLAWFUL|AL_NONGOOD)) ? "unchivalrous, merciless attack" :
            ((breach == AL_NONLAWFUL) ? "unchivalrous attack" : "merciless attack"),
            e.EVictim);

    if (e.EVictim->HasStati(TRIED,SK_DIPLOMACY+EV_SURRENDER*100,e.EActor)) {
        e.EActor->AlignedAct(AL_NONLAWFUL, 5, "attacking surrendered creatures", e.EVictim);
        if (!e.EVictim->isMType(MA_EVIL))
            e.EActor->AlignedAct(AL_NONGOOD, 2, "attacking surrendered creatures", e.EVictim);
    }

IgnoreMorality:;

    if (e.EActor && e.EActor->HasStati(NEGATED))
    { r = e.EActor->HandleNegate(e);
    if (r != NOTHING) return r; }

    {
        Item *ar = e.EVictim->InSlot(SL_ARMOUR);
        if (e.AType > 0 && e.AType <= A_LAST_STANDARD && ar)
            if (e.DType == AD_DREX || e.DType == AD_BLND ||
                e.DType == AD_STUN || e.DType == AD_SLOW ||
                e.DType == AD_CONF || e.DType == AD_DISE ||
                e.DType == AD_STON || e.DType == AD_PLYS ||
                e.DType == AD_WERE || e.DType == AD_PLYS ||
                (e.DType >= AD_DAST && e.DType <= AD_DALU) ||
                (e.DType >= AD_DRST && e.DType <= AD_DRLU) ||
                e.DType == AD_BLEE) 
                if (e.AType != A_TUCH || (ar && ar->HasQuality(AQ_SACRED)))
                    if (e.vHit < e.vDef + (e.EVictim->GetAttr(A_COV)-4))
                    {
                        VPrint(e, "Your armour blocks the <str>!",
                            "The <EVictim>'s armour blocks the <str>!",
                            (const char*)Lower(Lookup(DTypeNames,e.DType)));
                        return DONE;
                    } 
    }

    if (e.EVictim->HasAbility(CA_TOUGH_AS_HELL))
        if (e.DType <= AD_LAST_PHYSICAL)
            if (e.EVictim->Mod(A_CON) > 0)
            {
                int16 mod, percent;
                Armour *ar = (Armour*) e.EVictim->InSlot(SL_ARMOUR);
                mod = max(0,e.EVictim->Mod(A_CON));
                percent = (e.EVictim->AbilityLevel(CA_TOUGH_AS_HELL) * 100) /
                    (max(1,e.EVictim->ChallengeRating()) + ((!ar) ? 0 :
                    ar->isGroup(WG_HARMOUR) ? 9 :
                    (ar->isGroup(WG_MARMOUR) ? 6 : 3)));
                mod = (mod * percent) / 100;
                if (mod > 0)
                {
                    e.strDmg += Format(" %+d TaH", -mod);
                    e.bDmg   -= mod;
                    e.vDmg   = max(1, e.vDmg - mod);
                }
            }

            if (e.vDmg > e.EVictim->cHP / 3)
                if (e.AType && (e.EActor != e.EVictim))
                    if (e.DType == AD_SLASH || e.DType == AD_BLUNT)
                        if (e.EVictim->HasFeat(FT_REED_IN_THE_WIND))
                            if (!(e.EVictim->getArmourType(false) & (WG_MARMOUR|WG_HARMOUR)))
                                if (!e.EVictim->HasStati(PRONE) &&
                                    e.EVictim->HasMFlag(M_HUMANOID))
                                    if (e.EVictim->isMonster() || e.EVictim->yn(Format(
                                        "Use Reed in the Wind (vs. %d damage)?", e.vDmg)))
                                    {
                                        e.vDmg /= 3;
                                        e.EVictim->GainPermStati(PRONE,NULL,SS_MISC,0,0,0,0);
                                        VPrint(e, "You fall with the blow, lessening the force.",
                                            "The <EVictim> falls with the blow, lessening the force.");
                                    }

                                    if (e.DType <= AD_LAST_PHYSICAL && e.isCrit)
                                        if (e.EVictim->isCharacter())
                                            e.EPVictim->GodDeflect(e);

                                    if (e.actIllusion)
                                    {
                                        if (!e.EActor->isRealTo(e.EVictim))
                                        {
                                            e.Immune = true;
                                            e.EActor->DisbeliefCheck();
                                            return DONE;
                                        }
                                        /* For now... */
                                        if (e.DType > AD_LAST_PHYSICAL)
                                            return DONE;
                                    }
                                    if (e.effIllusion)
                                        if (e.DType > AD_LAST_PHYSICAL)
                                            return DONE;
                                    /* When you trade blows with a non-Improved illusion, you
                                    automatically get a chance to disbelieve it every hit
                                    that lands, due to the absence of kinetics, smell, etc. */
                                    if (e.vicIllusion && e.EActor)
                                        if (!(e.EVictim->getIllusionFlags() & IL_IMPROVED))
                                            e.EVictim->DisbeliefCheck(e.EActor);

                                    e.EVictim->FFCount = 0;
                                    if (e.EVictim->isPlayer())
                                        e.EPVictim->statiChanged = true;
                                    if (e.EActor) {
                                        e.EActor->FFCount = 0;
                                        if (e.EActor->isPlayer())
                                            e.EPActor->statiChanged = true;
                                    }
                                    // ww: this was AType, which was a mistake, I believe -- "flame
                                    // hobgoblins" would take full damage (+1, actually, from the -1 armour) 
                                    // from fire bolts, etc. 
                                    if (e.EVictim->ResistLevel(e.DType) == -1)
                                    {
                                        e.Immune = true;
                                        if (!e.Terse) {
                                            /* Illusions are immune to these things, not resistant (see
                                            ResistLevel), but the clever illusionist will make the
                                            failure look like a resistance, not an immmunity, to avoid
                                            giving away the creature's nature. */
                                            if (e.EVictim->isIllusion())
                                                if (e.DType == AD_MIND || e.DType == AD_PLYS ||
                                                    e.DType == AD_STON || e.DType == AD_STUN ||
                                                    e.DType == AD_CONF || e.DType == AD_VAMP ||
                                                    e.DType == AD_HOLY || e.DType == AD_EVIL ||
                                                    e.DType == AD_LAWF || e.DType == AD_CHAO ||
                                                    e.DType == AD_POIS || e.DType == AD_DISE ||
                                                    e.DType == AD_SLEE || e.DType == AD_FEAR)
                                                    if (e.EVictim->isRealTo(theGame->GetPlayer(0)))
                                                    { VPrint(e,NULL,"The <EVictim> resists.");
                                            goto DoneResistMsg; }
                                            VPrint(e,"You are unaffected.","The <EVictim> is unaffected.");
DoneResistMsg:
                                            ;
                                        }
                                        e.Terse = true;
                                        return DONE;
                                    }

                                    e.vicIncor = (e.EVictim->HasStati(PHASED)
                                        || e.EVictim->HasMFlag(M_INCOR));
                                    e.actIncor = (e.EActor && (e.EActor->HasStati(PHASED)
                                        || e.EActor->HasMFlag(M_INCOR)));

                                    if (e.vicIncor && !e.actIncor && !e.isGhostTouch) {
                                        // SRD: Even when hit by spells or magic weapons, it has a 50% chance
                                        // to ignore any damage from a corporeal source (except for positive
                                        // energy, negative energy, force effects such as magic missile, or
                                        // attacks made with ghost touch weapons). 
                                        if (e.DType == AD_HOLY || e.DType == AD_NECR || e.DType == AD_VAMP ||
                                            e.DType == AD_MAGC || e.DType == AD_PSYC || e.DType == AD_SUNL)
                                            ;
                                        else if (random(100) < 50) {
                                            e.Immune = true;
                                            if (!e.Terse)
                                                VPrint(e,"You, being incorporeal, are unaffected.",
                                                "The incorporeal <EVictim> is unaffected.");
                                            e.Terse = true;
                                            return DONE;
                                        } 
                                    } 


                                    if (HasStati(ACTING)) {
                                        why = 
                                            (e.EActor && e.EActor != this) ? 
                                            Format("%s: %s",(const char*)
                                            Lower(Lookup(DTypeNames,e.DType)),
                                            (const char*)e.EActor->Name(NA_A))
                                            : Lower(Lookup(DTypeNames,e.DType));
                                        HaltAction(why,false); 
                                    }

                                    EDType = e.DType;

                                    /* Implement Pick Pockets vs. A_STEA/A_SGLD */
                                    switch(e.DType)
                                    {
                                    case AD_NECR:
                                        if (e.EVictim->isMType(MA_UNDEAD)) {
                                            // SRD: necro damage (like inflict spells) heals undead ...
                                            n = e.vDmg; 
                                            if (e.EVictim->mHP+e.EVictim->Attr[A_THP] > e.EVictim->cHP) {
                                                if (e.EVictim->mHP+e.EVictim->Attr[A_THP] > e.EVictim->cHP + n) 
                                                    e.EVictim->cHP += n;
                                                else
                                                    e.EVictim->cHP = e.EVictim->mHP+e.EVictim->Attr[A_THP];
                                                DPrint(e,
                                                    "Necromatic magic heals the undead <EVictim>'s wounds <Str>!",
                                                    "Necromatic magic heals your wounds <Str>!", 
                                                    e.EVictim->cHP == (e.EVictim->mHP+e.EVictim->Attr[A_THP]) ? " fully" : "");
                                            }
                                            e.Immune = true;
                                            return DONE; 
                                        } 
                                    case AD_SUNL:
                                        if (e.DType == AD_SUNL) {
                                            if (e.EVictim->isMType(MA_PLANT)) {
                                                n = e.vDmg; 
                                                if (e.EVictim->mHP+e.EVictim->Attr[A_THP] > e.EVictim->cHP) {
                                                    if (e.EVictim->mHP+e.EVictim->Attr[A_THP] > e.EVictim->cHP + n) 
                                                        e.EVictim->cHP += n;
                                                    else
                                                        e.EVictim->cHP = e.EVictim->mHP+e.EVictim->Attr[A_THP];
                                                    DPrint(e,
                                                        "Sunlight heals the <EVictim>'s wounds <Str>!",
                                                        "Sunlight heals your wounds <Str>!", 
                                                        e.EVictim->cHP == (e.EVictim->mHP+e.EVictim->Attr[A_THP]) ? " fully" : "");
                                                }
                                                e.Immune = true;
                                                return DONE; 
                                            } 
                                            if (e.EVictim->isMType(MA_WRAITH)) {
                                                e.strDmg += SC(" x2 light");
                                                e.vDmg *= 2; 
                                            }
                                            EDType = AD_FIRE;
                                        }

                                    case AD_NORM:
                                    case AD_FIRE:
                                    case AD_COLD:
                                    case AD_ACID:
                                    case AD_ELEC:
                                    case AD_TOXI:
                                    case AD_PSYC:
                                    case AD_MAGC:
                                    case AD_SONI:
                                    case AD_SLASH:
                                    case AD_PIERCE:
                                    case AD_BLUNT:

                                        if (EDType == AD_ACID)
                                            if (e.EVictim->isMType(MA_LIVING) || !(e.EVictim->isMType(MA_PLANT)
                                                || e.EVictim->isMType(MA_VORTEX) || e.EVictim->isMType(MA_ELEMENTAL)))
                                                if ((e.EActor != e.EVictim) && !(e.isTrap || e.isActOfGod))
                                                    e.EActor->AlignedAct(AL_EVIL,2,"acid damage");

                                        if ((EDType == AD_COLD && e.EVictim->isMType(MA_FIRE)) || (EDType == AD_FIRE && e.EVictim->isMType(MA_COLD))) {
                                            e.strDmg += Format(" %+d vulnerability", e.vDmg / 2);
                                            e.vDmg = (e.vDmg * 3) / 2;
                                        }

                                        /* Straight Damage Types */

                                        /* ww: previously this checked isType(T_WEAPON), which meant that
                                        * even a +5 bow with +5 arrows wouldn't hurt something with
                                        * +1 weapon immunity ... */
                                        /* ww: finally, Monks with Ki Strike count as +X weapons */
                                        if (is_wepdmg(EDType) && e.EActor &&
                                            (e.AType == A_SWNG || e.AType == A_FIRE || e.AType == A_HURL || e.AType == A_GREA || e.AType == A_LUNG || e.AType == A_CALL ||
                                            e.AType == A_PREC)) {
                                                int16 wil = e.EVictim->AbilityLevel(CA_WEAPON_IMMUNITY);
                                                if (wil > 0 && !e.EActor->HasStati(IGNORES_WEAPON_IMMUNITY)) {
                                                    if (e.EItem && e.EItem->GetPlus() >= wil)
                                                        goto NotWImmune; 
                                                    else if (e.EItem2 && e.EItem2->GetPlus() >= wil)
                                                        goto NotWImmune; 
                                                    /*else if (!e.EItem && !e.EItem2 &&
                                                    e.EActor->AbilityLevel(CA_KI_STRIKE) >= wil)
                                                    goto NotWImmune;*/ 
                                                    else if ((e.EVictim->isMType(MA_DEVIL) || 
                                                        e.EVictim->isMType(MA_UNDEAD) ||
                                                        e.EVictim->isMType(MA_LYCANTHROPE) ||
                                                        e.EVictim->isMType(MA_DEMON)) &&
                                                        ((e.EItem && (e.EItem->isBlessed() ||
                                                        e.EItem->Material() == MAT_SILVER)) ||
                                                        (e.EItem2&& (e.EItem2->isBlessed() ||
                                                        e.EItem2->Material() == MAT_SILVER)) ))
                                                        goto NotWImmune; 
                                                    e.isWImmune = true;
                                                    return DONE;
                                                } 
                                        } 
NotWImmune:

                                        if (is_wepdmg(EDType) && e.vRoll && e.EVictim->Attr[A_COV]) {
                                            /* On a critical hit, we use the critical roll, rather than the
                                            attack roll, to determine if armour is bypassed. This can 
                                            (very rarely) lead to the illogical situation where a lower
                                            threat range is better, because you would rather bypass the
                                            tough armour on a roll of a 19 rather than "just" score a
                                            critical hit. This is sufficiently rare as to not be a serious
                                            issue.
                                            The reason crits don't automatically bypass armour is that
                                            this pushes them too far into the range of being stupid player
                                            instakills -- they can end up doing x4 or x5 damage on average
                                            to a heavily armoured player.
                                            */
                                            if (e.vHit + (e.isCrit ? e.vtRoll : e.vRoll) + e.vPenetrateBonus >= 
                                                e.vDef + e.EVictim->Attr[A_COV])
                                                e.isBypass = true;
                                            if (e.EItem && e.EItem->isItem() && e.EItem->HasQuality(WQ_ENERGY))
                                                e.isBypass = true;
                                            if (e.AType != A_SUND && e.EItem2 && e.EItem2->isItem() && e.EItem2->HasQuality(WQ_ENERGY))
                                                e.isBypass = true;

                                        }

                                        e.vArm = (int8)e.EVictim->ResistLevel(EDType,e.isBypass);

                                        /* ww: replaced for now by the weimer alternate penetration system 
                                        if (is_wepdmg(e.DType)) {
                                        if (e.EActor->Mod2(A_STR) > 0)
                                        e.vPen = min(e.EActor->Mod2(A_STR),e.vArm/2);
                                        else
                                        e.vPen = 0;
                                        if (e.EItem && e.EItem->isItem() && e.EItem->HasIFlag(WT_PENETRATING))
                                        e.vPen += 2;
                                        else if (e.AType != A_SUND && e.EItem2 && e.EItem2->isItem() && e.EItem2->HasIFlag(WT_PENETRATING))
                                        e.vPen += 2;
                                        }
                                        */

                                        if (e.vDmg < 5)
                                            Col = 0;
                                        else if (e.vDmg < 10)
                                            Col = 1;
                                        else if (e.vDmg < 100)
                                            Col = (e.vDmg / 10) + 1;
                                        else if (e.vDmg < 200)
                                            Col = ((e.vDmg - 100)/20) + 11;
                                        else
                                            Col = 15;

                                        if (e.vArm > 0) {
                                            // ww: I'm having serious consistency problems with the way
                                            // this plays out. Imagine that you (critically) hit something
                                            // for 180 points of damage (this is not unreasonable, just
                                            // kinda rare -- high STR, magical great maul, critical hit
                                            // x4). I just did it now, so it's possible. Anyway, that's
                                            // enough force to kill about 30 normal humans (say, 5 hp
                                            // each). And yet, something with armour 8 (plate mail, say)
                                            // can completely resist this blow ArmourTable[8][15] = 24%
                                            // of the time. One fourth of the time your plate armour reduces
                                            // to nothing enough force to kill 30 men? I think not. Note
                                            // that the plate mail's chance of reducing a hit for 1 point
                                            // of damage to 0 here is ArmourTable[8][0] = 43% ... one
                                            // conclusion here is that the ArmourTable[i] does not drop off
                                            // fast enough. However, ArmourTable is big and I don't want to
                                            // change that. What I will do here is say that you can only
                                            // completely absorb an attack with mag <= your armour rating x
                                            // 2. 
                                            // fjm: I upped the minAmount a bit, from vArm to 10 + vArm*2,
                                            // so that armour can absorb small and medium blows instead of
                                            // just small blows. I think that's still in the realm of
                                            // believability.
                                            Percent = AbsorbTable[min(e.vArm,16)][Col];
                                            int minAmount = 10 + e.vArm*2;
                                            if ((e.EItem && e.EItem->isItem() && 
                                                e.EItem->HasIFlag(WT_PENETRATING)) ||
                                                (e.EItem2 && e.EItem2->isItem() && 
                                                e.EItem2->HasIFlag(WT_PENETRATING))) {
                                                    Percent /= 2;
                                                    minAmount /= 2; 
                                            }
                                            /* Later, this should be altered to make /actual/ armour
                                            count more for absorbing blows then magic bonuses, and
                                            barbarian natural armour should not absorb blows at all. */
                                            if (Dice::Roll(1,100) <= Percent && e.vDmg <= minAmount) { 
Absorbed:
                                                e.Absorb = true;
                                                e.vPen = 0; // 100% absorbed
                                                e.aDmg = 0; 
                                                if (!is_wepdmg(EDType))
                                                    e.Resist = true;
                                                if (!e.Terse) {
                                                    if ((EDType == AD_SLASH || e.DType == AD_PIERCE
                                                        || EDType == AD_BLUNT) && e.EVictim->EInSlot(SL_ARMOUR))
                                                        VPrint(e,"Your <Str>armour absorbs the attack!",
                                                        "The <EVictim>'s <Str>armour absorbs the attack!",
                                                        e.isBypass ? "natural " : "");
                                                    else VPrint(e,"You resist completely!",
                                                        "The <EVictim> resists completely!");
                                                }
                                                return DONE;
                                            }

                                            Percent = ArmourTable[min(e.vArm,32)][Col];
                                            if (e.EItem && e.EItem->isItem() && 
                                                e.EItem->HasIFlag(WT_PENETRATING))
                                                Percent /= 2;
                                            else if (e.EItem2 && e.EItem2->isItem() && 
                                                e.EItem2->HasIFlag(WT_PENETRATING))
                                                Percent /= 2;
                                            e.vPen = 100 - Percent;

                                            e.aDmg = ((int32)e.vDmg * ((int32)(e.vPen))) / 100L;
                                            if (!e.aDmg)
                                                goto Absorbed;
                                            if (!is_wepdmg(EDType))
                                                e.Resist = true;
                                        } 
                                        else 
                                            e.aDmg = e.vDmg;


                                        if (is_wepdmg(EDType))
                                            if (e.vDef > e.vHit + 14)
                                                if (e.EVictim->HasAbility(CA_DEFENSIVE_ROLL))
                                                {
                                                    uint32 at = getArmourType(false);

                                                    if (at & WG_HARMOUR)
                                                        goto CantRoll;

                                                    e.vDefRoll = 5;

                                                    if (at & WG_MARMOUR)
                                                        ;
                                                    else if (at)
                                                        e.vDefRoll -= 1;
                                                    else
                                                        e.vDefRoll -= 2;

                                                    if (e.EVictim->isSmallRace() && 
                                                        e.EVictim->GetAttr(A_SIZ) == SZ_SMALL)
                                                        if (e.EActor->GetAttr(A_SIZ) >= SZ_MEDIUM)
                                                            e.vDefRoll -= 1;

                                                    if (e.vDef > e.vHit + 19)
                                                        e.vDefRoll -= 1;

                                                    e.vDefRoll = max(1,e.vDefRoll);

                                                    e.aDmg = max(1, (e.aDmg*e.vDefRoll)/6);
                                                    e.isDefRoll = true;
                                                    e.strXDmg = Format(" x (%d/6)",e.vDefRoll) + e.strXDmg;
                                                    /* People that benefit from Defensive Roll are going
                                                    to see this message A LOT, hence it being so short
                                                    -- avoids message spam. */
                                                    e.EVictim->IDPrint("You roll!", "The <Obj> rolls with the hit!", e.EActor);
                                                    e.EVictim->Exercise(A_DEX,1,EDEX_EVASION,45);
                                                }
CantRoll:

                                                if (((!e.actIllusion) || e.EActor->isShade()) &&
                                                    ((!e.effIllusion) || e.illFlags & IL_SHADE))
                                                {
                                                    int16 per, rDmg, iDmg;
                                                    if (e.EActor->isShade())
                                                    {
                                                        per = e.EActor->getShadePercent();
                                                        rDmg = (e.aDmg * per) / 100;
                                                        iDmg = e.aDmg - rDmg;
                                                    }
                                                    else if (e.illFlags & IL_SHADE)
                                                    {
                                                        per = 0 + ((e.illFlags & IL_SHADE10) ? 10 : 0)
                                                            + ((e.illFlags & IL_SHADE20) ? 20 : 0)
                                                            + ((e.illFlags & IL_SHADE40) ? 40 : 0);
                                                        rDmg = (e.aDmg * per) / 100;
                                                        iDmg = e.aDmg - rDmg;
                                                    }                   
                                                    else
                                                    { rDmg = e.aDmg; iDmg = 0; }


                                                    if(rDmg >= e.EVictim->cHP)
                                                    {
                                                        if (e.EVictim->HasFeat(FT_IGNORE_WOUND))
                                                            if (!e.EVictim->HasStati(TRIED,FT_IGNORE_WOUND))
                                                            {
                                                                e.EVictim->IPrint("<3>You endure a nearly-mortal stroke!<7>");
                                                                e.EActor->IPrint("You were sure that blow should have felled "
                                                                    "the <Obj>, but no.", e.EVictim);
                                                                e.EVictim->GainTempStati(TRIED,NULL,-2,SS_MISC,FT_IGNORE_WOUND);
                                                                goto WoundIgnored;
                                                            }

                                                            if (e.EVictim->cHP + e.EVictim->SumStatiMag(ILLUS_DMG) > rDmg)
                                                                ReThrow(EV_PDAMAGE, e);
                                                            else { 
                                                                e.EVictim->cHP = 0;
                                                                ReThrow(EV_DEATH, e);
                                                                return DONE;
                                                            }
                                                    }
                                                    e.EVictim->cHP -= e.aDmg;
                                                    if (iDmg && !e.EVictim->HasStati(SLEEPING,SLEEP_PSYCH) && 
                                                        e.EActor->isRealTo(e.EVictim))
                                                    {
                                                        e.aDmg = iDmg;
                                                        ReThrow(EV_PDAMAGE, e);
                                                        e.aDmg = rDmg;
                                                    } 
                                                }
                                                else
                                                    ReThrow(EV_PDAMAGE,e);

                                                if (e.vicIllusion && !e.actIllusion)
                                                    if (e.EVictim->isShade() && (e.EActor == e.EVictim || 
                                                        !e.EActor || !e.EVictim->isRealTo(e.EActor)))
                                                        if ((e.EVictim->mHP - e.EVictim->cHP) * 100 / 
                                                            e.EVictim->mHP > e.EVictim->getShadePercent())
                                                        {
                                                            e.EVictim->cHP = 0;
                                                            ReThrow(EV_DEATH, e);
                                                            return DONE;
                                                        } 

WoundIgnored:

                                                        if (HasAbility(CA_RETRIBUTIVE_STRIKE) && e.EActor && e.EActor != this)
                                                        {
                                                            if (HasStati(WOUNDED_BY,0,e.EActor)) {
                                                                SetStatiMag(WOUNDED_BY,0,e.EActor,
                                                                    GetStatiMag(WOUNDED_BY,0,e.EActor) + e.aDmg );
                                                                SetStatiDur(WOUNDED_BY,0,e.EActor,
                                                                    max(GetStatiDur(WOUNDED_BY,0,e.EActor) + e.aDmg*2, e.aDmg*2+10) );
                                                            }
                                                            else
                                                                GainTempStati(WOUNDED_BY,e.EActor,e.aDmg*2+10,SS_MISC,0,e.aDmg,0,0);
                                                        }

                                                        if (isPlayer() && ((cHP+e.aDmg)*3 >= mHP+Attr[A_THP]))
                                                            if (cHP*3 <= mHP+Attr[A_THP]) {
                                                                IPrint("<12>You are gravely wounded!<7>");
                                                                Player * pl = (Player *)this; 
                                                                if (e.EActor) 
                                                                    pl->AddJournalEntry(Format("You are <12>gravely wounded by %s<7>!", (const char*)e.EActor->Name(NA_A)));
                                                                else
                                                                    pl->AddJournalEntry(Format("You are <12>gravely wounded<7>!"));
                                                                if (!(e.isTrap || e.isActOfGod))
                                                                    e.EVictim->GainPermStati(CRIT_WOUNDED_BY,e.EActor,SS_MISC);
                                                            } 

                                                            if (HasStati(PARALYSIS))
                                                                RemoveStati(PARALYSIS,-1,PARA_HYPNOTIZED);

                                                            if (HasStati(ELEVATED) && !isDead())
                                                            {
                                                                int16 ClimbDC;
                                                                ClimbDC = GetStatiVal(ELEVATED) == ELEV_TREE ? 10 : 20;
                                                                ClimbDC += (e.vDmg * 40) / (mHP+GetAttr(A_THP));
                                                                if (!SkillCheck(SK_CLIMB,ClimbDC,false))
                                                                    ClimbFall();           
                                                            }       

                                                            if ((e.DType == AD_SLASH || e.DType == AD_PIERCE ||
                                                                e.DType == AD_BLUNT) && HasStati(LEVITATION) && !isDead())
                                                                ThrowDmg(EV_DAMAGE,AD_KNOC,e.vDmg / max(1,mHP/20),
                                                                "aerial damage knockback", e.EActor, e.EVictim, e.EItem, e.EItem2);


                                                            return DONE;
                                    case AD_VAMP:
                                        if (!e.EActor)
                                        {
                                            e.DType = AD_NORM;
                                            return ReThrow(EV_DAMAGE,e);
                                        }
                                        n = cHP;
                                        e.DType = AD_NECR;
                                        r = ReThrow(EV_DAMAGE,e);
                                        e.DType = AD_VAMP;
                                        if (r != DONE)
                                            return r;
                                        if (cHP < n)
                                            if (e.EActor->mHP+e.EActor->Attr[A_THP] > e.EActor->cHP) {
                                                if (e.EActor->mHP+e.EActor->Attr[A_THP] > e.EActor->cHP + (n-cHP)) 
                                                    e.EActor->cHP += (int16)n-cHP;
                                                else
                                                    e.EActor->cHP = e.EActor->mHP+e.EActor->Attr[A_THP];
                                                DPrint(e,"Your wounds heal<Str>!", "The <EActor>'s wounds heal<Str>!",
                                                    e.EActor->cHP == (e.EActor->mHP+e.EActor->Attr[A_THP]) ? " fully" : "");
                                            }
                                            return DONE;
                                    case AD_ENGL:
                                    case AD_SWAL:
                                        if (!e.EActor)
                                            return ABORT;
                                        if (e.DType == AD_SWAL)
                                            if (!(e.EActor->GetAttr(A_SIZ) > e.EVictim->GetAttr(A_SIZ)))
                                                return DONE;
                                        if (!(e.EActor->GetAttr(A_SIZ) >= e.EVictim->GetAttr(A_SIZ)))
                                            return DONE;
                                        if (e.EVictim->HasStati(ENGULFER))
                                            return ABORT;
                                        if (e.EVictim->HasStati(ENGULFED))
                                            return ABORT;
                                        if (e.EActor->HasStati(ENGULFER))
                                            return ABORT;
                                        if (e.EActor->HasStati(ENGULFED))
                                            return ABORT;
                                        if (e.EVictim->HasStati(MOUNT))
                                            return ABORT;  
                                        if (e.saveDC <= 0 || 
                                            !e.EVictim->SavingThrow(REF,e.saveDC)) 
                                        {
                                            e.EVictim->DoEngulf(e.EActor);
                                            if (e.DType == AD_ENGL)
                                                TPrint(e,"You engulf the <EVictim>!",
                                                "You are engulfed by the <EVictim>!",
                                                "The <EVictim> is engulfed by the <EActor>!");
                                            else
                                                TPrint(e,"You swallow the <EVictim> whole!",
                                                "You get swallowed whole!",
                                                "The <EVictim> gets swallowed whole!");
                                        }
                                        else
                                        { e.Resist = true; } 
                                        return DONE; 
                                    case AD_GRAB:
                                        if (!e.EActor)
                                            return ABORT;
                                        if (e.EActor && e.EVictim->HasStati(GRABBED,-1,e.EActor))
                                            return ABORT;
                                        if (e.EActor && e.EVictim->HasStati(GRAPPLED,-1,e.EActor))
                                            return ABORT;
                                        if (e.saveDC <= 0 || 
                                            !e.EVictim->SavingThrow(REF,e.saveDC,SA_GRAB)) {
                                                e.EVictim->GainPermStati(GRABBED,   e.EActor,  SS_ATTK);
                                                e.EActor ->GainPermStati(GRAPPLING, e.EVictim, SS_ATTK);
                                                e.vicHeld = true;
                                        }
                                        else
                                        { e.Resist = true;  } 
                                        break;
                                    case AD_STON:
                                        if (e.EVictim->HasStati(STONING) || !e.EVictim->isMType(MA_LIVING)
                                            || e.EVictim->ResistLevel(AD_STON) == -1)
                                        { e.Immune = true; break; }
                                        if (!e.EVictim->SavingThrow(REF,e.saveDC,SA_PETRI)) 
                                        {
                                            e.EVictim->StatiMessage(STONING,0,false);
                                            e.EVictim->GainTempStati(STONING, e.EActor,  SS_ATTK, 20);
                                        }
                                        else
                                            e.Resist = true;
                                        break;
                                    case AD_FEAR:
                                        if (e.EVictim->HasStati(AFRAID))
                                            return ABORT;          
                                        if (e.AType == A_PROX)
                                            e.saveDC += (int8)TMON(e.EVictim->tmID)->GetConst(DRAGON_FEAR_DC);
                                        if (!e.EVictim->SavingThrow(WILL,e.saveDC,SA_FEAR)) {
                                            if (!e.Terse)
                                                VPrint(e,"You are terrified!", "The <EVictim> seems terrified!"); 
                                            e.EVictim->GainTempStati(AFRAID,e.EActor,SS_ATTK,(int8)e.vDmg,FEAR_MAGIC);
                                        }
                                        else
                                            e.Resist = true;
                                        break;
                                    case AD_BLND:
                                        if (e.EVictim->isBlind()) {
                                            e.Immune = true;
                                            VPrint(e,"Being already blind, you are unaffected.",
                                                "Being already blind, the <EVictim> is unaffected.");
                                        } 
                                        else if (!e.EVictim->SavingThrow(FORT,e.saveDC)) {
                                            if (!e.Terse)
                                                VPrint(e,"You are blinded!", "The <EVictim> is blinded!"); 
                                            e.EVictim->GainTempStati(BLIND,e.EActor,e.vDmg, SS_ATTK);
                                        }
                                        else
                                            e.Resist = true;
                                        break;
                                    case AD_PLYS:
                                        if (e.EVictim->HasStati(PARALYSIS,PARA_HELD))
                                            return ABORT; 
                                        if (e.EVictim->UseLimitedFA())
                                            return DONE;         
                                        if (!e.EVictim->SavingThrow(FORT,e.saveDC,SA_PARA)) {
                                            if (!e.Terse)
                                                e.EVictim->StatiMessage(PARALYSIS,0,false);
                                            e.EVictim->GainTempStati(PARALYSIS,e.EActor,e.vDmg,SS_ATTK,PARA_HELD);
                                        }
                                        else
                                            e.Resist = true;
                                        break;
                                    case AD_STUK:
                                        if (e.EVictim->HasStati(STUCK))
                                            return ABORT; 
                                        else if (e.EVictim->HasMFlag(M_AMORPH)) {
                                            e.Immune = true;
                                            break;
                                        } 
                                        if (!e.EVictim->SavingThrow(REF,e.saveDC,SA_GRAB)) {
                                            if (!e.Terse)
                                                e.EVictim->StatiMessage(STUCK,0,false);

                                            e.EVictim->GainTempStati(STUCK,e.EActor,e.vDmg,SS_ATTK,(int16)e.EParam);
                                        } else e.Resist = true;
                                        break;
                                    case AD_SLOW:
                                        if (!e.EVictim->SavingThrow(FORT,e.saveDC,SA_PARA)) {
                                            /* Kludge, but necessary */
                                            xID = FIND("slow");
                                            ASSERT(xID);
                                            ThrowEff(EV_MAGIC_STRIKE,xID,e.EActor ? e.EActor : e.EVictim,e.EVictim);
                                        }
                                        else
                                            e.Resist = true;
                                        break;
                                    case AD_DRMA:
                                        if (!e.EVictim->SavingThrow(WILL,e.saveDC,SA_ENCH)) {
                                            if (!e.Terse)
                                                TPrint(e,"You leech away the <EVictim>'s magical energy!",
                                                "Your magical energy is leeched away!", NULL); 
                                            LoseMana(e.vDmg,true);
                                        }
                                        else
                                            e.Resist = true;
                                        break;
                                    case AD_CONF:
                                        if (e.EVictim->HasStati(CONFUSED))
                                            return ABORT;          
                                        if (!e.EVictim->SavingThrow(WILL,e.saveDC,SA_ENCH|SA_CONF)) {
                                            if (!e.Terse)
                                                VPrint(e,"Your suddenly feel disoriented!", 
                                                "The <EVictim> staggers..."); 
                                            e.EVictim->GainTempStati(CONFUSED,NULL,e.vDmg,SS_ATTK);
                                        }
                                        else
                                            e.Resist = true;
                                        break;
                                    case AD_NAUS:
                                        if (e.EVictim->HasStati(NAUSEA))
                                            return ABORT;          
                                        if (!e.EVictim->SavingThrow(WILL,e.saveDC,0)) {
                                            if (!e.Terse)
                                                VPrint(e,"Your suddenly feel nauseated!", 
                                                "The <EVictim> retches..."); 
                                            e.EVictim->GainTempStati(NAUSEA,NULL,e.vDmg,SS_ATTK);
                                        }
                                        else
                                            e.Resist = true;
                                        break;
                                    case AD_EVIL:
                                        if (e.EVictim->isMType(MA_EVIL))
                                            break;
                                        else if (!e.EVictim->isMType(MA_GOOD))
                                            e.vDmg /= 2;
                                        e.DType = AD_NORM;
                                        if (!e.Terse)
                                            VPrint(e,"Malevolent energies burn at your spirit!",
                                            "Malevolent energies burn at the <EVictim>!");
                                        ReThrow(EV_DAMAGE,e);
                                        break; 
                                    case AD_CHAO:
                                        if (e.EVictim->isMType(MA_CHAOTIC))
                                            break;
                                        else if (!e.EVictim->isMType(MA_LAWFUL))
                                            e.vDmg /= 2;
                                        e.DType = AD_NORM;
                                        if (!e.Terse)
                                            VPrint(e,"Primordial chaos lashes at you!", 
                                            "Primordial chaos lashes at the <EVictim>!");
                                        ReThrow(EV_DAMAGE,e);
                                        break;           
                                    case AD_LAWF:
                                        if (e.EVictim->isMType(MA_LAWFUL))
                                            break;
                                        else if (!e.EVictim->isMType(MA_CHAOTIC))
                                            e.vDmg /= 2;
                                        e.DType = AD_NORM;
                                        if (!e.Terse)
                                            VPrint(e,"Order's fury binds you!", 
                                            "Order's fury binds the <EVictim>.");
                                        ReThrow(EV_DAMAGE,e);
                                        break;           
                                    case AD_HOLY:
                                        if (e.EVictim->isMType(MA_GOOD) &&
                                            !e.EVictim->isMType(MA_UNDEAD))
                                            break;
                                        else if (!e.EVictim->isMType(MA_EVIL))
                                            e.vDmg /= 2;
                                        e.DType = AD_NORM;
                                        if (!e.Terse)
                                            VPrint(e,"Holy power smites you!",
                                            "Holy power smites the <EVictim>!");
                                        ReThrow(EV_DAMAGE,e);
                                        break;                   
                                    case AD_DRST:
                                    case AD_DRDX:
                                    case AD_DRCO:
                                    case AD_DRIN:
                                    case AD_DRWI:
                                    case AD_DRCH:
                                    case AD_DRLU:
                                    case AD_DAST:
                                    case AD_DADX:
                                    case AD_DACO:
                                    case AD_DAIN:
                                    case AD_DAWI:
                                    case AD_DACH:
                                    case AD_DALU:
                                        /* We don't necessarily know what kind of effect is *causing* the
                                        stat drain, so we have to make some guesses based on the nature
                                        of the attacker: an inherently evil creature that drains stats
                                        is presumed to use inherently evil means to do so; an undead is
                                        assumed to drain based on negative energy, and a non-undead that
                                        bites is assumed to be using poison. */
                                        subtype = 0;
                                        at = e.DType >= AD_DAST ? (e.DType-AD_DAST) : (e.DType-AD_DRST);
                                        if (at >= A_STR && at <= A_CON && e.EVictim->isMType(MA_UNDEAD)) {
                                            /* SRD: &#151;Not subject to critical hits, nonlethal damage,
                                            * ability drain, or energy drain. Immune to damage to its
                                            * physical ability scores (Strength, Dexterity, and
                                            * Constitution), as well as to fatigue and exhaustion
                                            * effects.<br> */
                                            e.Immune = true; break;
                                        } 
                                        if (e.EActor && e.EActor->isMType(MA_EVIL))
                                            subtype |= SA_EVIL;
                                        if (e.EActor && e.EActor->isMType(MA_UNDEAD)) {
                                            subtype |= SA_NECRO;
                                            if (e.EVictim->ResistLevel(AD_NECR) == -1)
                                            { e.Immune = true; break; }
                                        }
                                        if (e.AType == A_BITE && e.EActor && !e.EActor->isMType(MA_UNDEAD))
                                            subtype |= SA_POISON;
                                        e.vDmg =  max(1,e.Dmg.Roll());
                                        e.vDmg -= AbilityLevel(CA_STRONG_SOUL);

                                        if (e.vDmg <= 0) {
                                            VPrint(e,"The strength of your spirit turns aside the effects.",NULL);
                                            e.vDmg = 0;
                                            return DONE;
                                        }

                                        if (e.EVictim->SavingThrow(FORT,e.saveDC,subtype)) 
                                        {
                                            e.vDmg = max(1,e.vDmg/2);
                                            e.Resist = true;
                                        }

                                        if (e.EVictim->HasStati(SUSTAIN,at))
                                            e.vDmg = max(0,e.vDmg - e.EVictim->SumStatiMag(SUSTAIN,at));
                                        if (!e.vDmg || Attr[at] <= 0) {
                                            e.Immune = true;
                                            break;
                                        }
                                        if ((Attr[at] - e.vDmg <= 0) /* && (at == A_STR || at == A_CON)*/)
                                        {
                                            if (e.EMagic) {
                                                if (e.EActor == e.EVictim || !e.EActor)
                                                    e.GraveText = NAME(e.eID);
                                                else
                                                    e.GraveText = XPrint("An <Obj>'s <Res>",e.EActor,e.eID);
                                            }
                                            else if (e.AType == A_TUCH && e.EActor)
                                                e.GraveText = XPrint("A <Obj>'s Touch",e.EActor);
                                            else if (e.EActor && e.AType == A_BITE || e.AType == A_SUCK)
                                                e.GraveText = XPrint("A <Obj>'s Bite",e.EActor);
                                            else if (e.EActor)
                                                e.GraveText = e.EActor->Name(NA_A|NA_CAPS);
                                            else 
                                                e.GraveText = "attribute drain";
                                            ReThrow(EV_DEATH,e);
                                            return DONE;
                                        }
                                        if (e.DType >= AD_DAST) /* permadrain */
                                            e.EVictim->GainPermStati(ADJUST_DMG,NULL,SS_ATTK,e.DType-AD_DAST,-e.vDmg);
                                        else                    /* recoverable drain; currently identical */
                                            e.EVictim->GainPermStati(ADJUST_DMG,NULL,SS_ATTK,e.DType-AD_DRST,-e.vDmg);
                                        switch (at)
                                        {
                                        case A_STR: e.EVictim->IPrint("<4>You feel weaker.<7>"); break;
                                        case A_DEX: e.EVictim->IPrint("<4>You feel numb.<7>"); break;
                                        case A_CON: e.EVictim->IPrint("<4>You feel unhealthy.<7>"); break;
                                        case A_INT: e.EVictim->IPrint("<4>Your head throbs.<7>"); break;
                                        case A_WIS: e.EVictim->IPrint("<4>You feel dizzy.<7>"); break;
                                        case A_CHA: e.EVictim->IPrint("<4>You feel craven.<7>"); break;
                                        case A_LUC: e.EVictim->IPrint("<4>You feel unlucky.<7>"); break;
                                        }
                                        break;
                                    case AD_DREX:
                                        if (e.EVictim->HasStati(SUSTAIN,A_AID)) {
                                            lv = (int8)e.EVictim->SumStatiMag(SUSTAIN,A_AID);
                                            if (lv*3 >= e.vDmg) {
                                                e.Immune = true;
                                                break;
                                            }
                                            else
                                                e.vDmg -= lv*3;
                                        }
                                        if (e.EVictim->ResistLevel(AD_NECR) == -1)
                                        { e.Immune = true; break; }
                                        if (e.EVictim->SavingThrow(WILL,e.saveDC,SA_NECRO))
                                        {
                                            e.vDmg /= 2;
                                            e.Resist = true;
                                        }
                                        if (!e.Terse)
                                            VPrint(e,"You feel a chill in your very soul!",
                                            "The <EVictim> looks deathly pale!");
                                        if (e.EVictim->isCharacter())
                                            ((Character*)e.EVictim)->DrainXP(e.vDmg * 50);
                                        else {
                                            if (e.EVictim->HasStati(ADJUST_DMG,A_AID))
                                                e.EVictim->GetStati(ADJUST_DMG,A_AID)->Mag -= 
                                                e.vDmg / max(1,e.EVictim->ChallengeRating() * 5);
                                            else
                                                // ww: the signs were reversed here, so one hit from a
                                                // spectre actually gave another creature a +1 neglev bonus
                                                // ... :-) 
                                                e.EVictim->GainPermStati(ADJUST_DMG,NULL,SS_ATTK,A_AID,
                                                -(max(1,e.vDmg / max(1,e.EVictim->ChallengeRating() * 5))));
                                            lv = max(1,e.EVictim->ChallengeRating());
                                            lv += e.EVictim->SumStatiMag(ADJUST_DMG,A_AID);
                                            if (lv <= 0)
                                                ReThrow(EV_DEATH,e);
                                        }
                                        break; 
                                    case AD_DISE:
                                        ASSERT(e.EActor);
                                        if (e.EVictim->ResistLevel(AD_DISE) == -1)
                                        {
                                            e.Immune = true;
                                            return DONE;
                                        }
                                        n = 0;
                                        for(xID = TMON(e.EActor->mID)->FirstRes(AN_DISEASE);xID;
                                            xID = TMON(e.EActor->mID)->NextRes(AN_DISEASE))
                                            Candidates[n++] = xID;
                                        StatiIterNature(e.EActor,TEMPLATE)
                                            for(xID = TMON(S->eID)->FirstRes(AN_DISEASE);xID;
                                                xID = TMON(S->eID)->NextRes(AN_DISEASE))
                                                Candidates[n++] = xID;
                                        StatiIterEnd(e.EActor)
                                            if (!n)
                                            {
                                                if (e.eID && TEFF(e.eID)->ef.aval == AR_DISEASE)
                                                    xID = e.eID;
                                                else
                                                    return DONE; 
                                                Error(XPrint("Creature <Res> has an AD_DISE attack, but no diseases!",e.EActor->mID));
                                                return DONE;
                                            }
                                            xID = Candidates[random((int16)n)];
                                            if (e.EVictim->HasEffStati(DISEASED,xID))
                                                break;
                                            if (!e.EVictim->SavingThrow(FORT,e.saveDC)) {
                                                e.Event = EV_INFECT;
                                                switch (TEFF(xID)->Event(e,xID)) {
                                                case ABORT:
                                                    return ABORT;
                                                case DONE:
                                                    return DONE;
                                                }
                                                if (!e.Terse)
                                                    VPrint(e,"You feel unclean.", "The <EVictim> squirms!"); 
                                                e.EVictim->GainPermStati(DISEASED,NULL,SS_ATTK, 0, 0, xID);
                                            }
                                            else
                                                e.Resist = true;
                                            break;
                                    case AD_POIS:
                                        ASSERT(e.EActor);
                                        if (e.EVictim->ResistLevel(AD_TOXI) == -1)
                                        {
                                            e.Immune = true;
                                            return DONE;
                                        }
                                        n = 0;
                                        for(xID = TMON(e.EActor->mID)->FirstRes(AN_POISON);xID;
                                            xID = TMON(e.EActor->mID)->NextRes(AN_POISON))
                                            Candidates[n++] = xID;
                                        StatiIterNature(e.EActor,TEMPLATE)
                                            for(xID = TMON(S->eID)->FirstRes(AN_POISON);xID;
                                                xID = TMON(S->eID)->NextRes(AN_POISON))
                                                Candidates[n++] = xID;
                                        StatiIterEnd(e.EActor)
                                            if (!n)
                                            {
                                                if (e.eID && TEFF(e.eID)->ef.aval == AR_POISON)
                                                    xID = e.eID;
                                                else
                                                    return DONE; 
                                                /*
                                                Error(XPrint("Creature <Res> has an AD_POIS attack for <Res>, but no poisons!",e.EActor->mID, e.EVictim->mID));
                                                */
                                            } else
                                                xID = Candidates[random((int16)n)];
                                            if (e.EVictim->HasEffStati(POISONED,xID))
                                                break;
                                            if (e.saveDC == 0)
                                                e.saveDC = TEFF(xID)->ef.sval;
                                            if (!e.EVictim->SavingThrow(FORT,e.saveDC)) {
                                                e.Event = EV_POISON;
                                                switch (TEFF(xID)->Event(e,xID)) {
                                                case ABORT:
                                                    return ABORT;
                                                case DONE:
                                                    return DONE;
                                                }
                                                if (!e.Terse)
                                                    VPrint(e,"You've been poisoned!", "The <EVictim> staggers!"); 
                                                e.EVictim->GainPermStati(POISONED,NULL,SS_ATTK, 0, 0, xID);
                                            }
                                            else
                                                e.Resist = true;
                                            break;
                                    case AD_TRIP:
                                        {
                                            if (e.EVictim->HasStati(PRONE) || 
                                                e.EVictim->HasMFlag(M_AMORPH) ||
                                                (e.vicIncor && !e.actIncor && !e.isGhostTouch) ||
                                                e.EVictim->HasMFlag(M_NOLIMBS)) {
                                                    e.Immune = true; 
                                                    break;
                                            }
                                            if (e.EActor && e.EVictim && e.EActor != e.EVictim &&
                                                !e.eID && !e.EActor->HasFeat(FT_MASTER_TRIP) &&
                                                (e.AType == A_TRIP)) 
                                                e.EActor->ProvokeAoO(e.EActor);              
                                            if (e.EActor->isDead())
                                                return ABORT;
                                            if (e.saveDC == -1)
                                                goto SkipTripSave;
                                            int16 DC; DC = e.saveDC;
                                            if (DC == 0) DC = e.vDmg;
                                            // ww: subtract the secondary dex mod so that you only get the
                                            // ranks and bonuses -- your DEX mod will count again on the
                                            // reflex save
                                            if (e.EVictim->HasSkill(SK_BALANCE))
                                                DC -= (e.EVictim->SkillLevel(SK_BALANCE) - e.EVictim->Mod2(A_DEX)) / 2;
                                            if (DC < 1 || e.EVictim->SavingThrow(REF,DC,SA_KNOCKDOWN)) { 
                                                e.Resist = true; 
                                                break; 
                                            }
SkipTripSave:
                                            if (e.EActor) {
                                                e.EActor->Reveal(false);
                                                e.EActor->MakeNoise(6);
                                            }
                                            if (!e.Terse)
                                                e.EVictim->StatiMessage(PRONE,0,false);
                                            e.EVictim->GainPermStati(PRONE,NULL,SS_ATTK);
                                            /* VPrint(e,"You are knocked prone!",
                                            "The <EVictim> is knocked prone!"); */       
                                            break;
                                        }
                                    case AD_DISA:
                                        Item *wp; int16 tx,ty;
                                        if (!(wp = e.EVictim->EInSlot(SL_WEAPON)))
                                            break;
                                        if (e.EVictim->SavingThrow(REF,e.saveDC))
                                        { e.Resist = true; break; }
                                        do {
                                            tx = (e.EVictim->x - 1) + random(3);
                                            ty = (e.EVictim->y - 1) + random(3);
                                        }
                                        while(e.EVictim->m->SolidAt(tx,ty)&& 
                                            (tx != e.EVictim->x || ty != e.EVictim->y));
                                        Throw(EV_LOSEITEM,e.EVictim,NULL,wp);
                                        wp->Remove(false); /* Paranoia */
                                        wp->PlaceAt(e.EVictim->m,tx,ty);
                                        break;
                                    case AD_KNOC:
                                        int32 i, curr;
                                        curr = e.EVictim->x+e.EVictim->y*256;
                                        e.EDir = e.EActor ? e.EActor->DirTo(e.EVictim) : random(8);
                                        if (e.eID || !e.EVictim->SavingThrow(FORT,e.saveDC,SA_KNOCKDOWN)) {
                                            for(i=0;i!=e.vDmg;i++)
                                                if (ThrowDir(EV_PUSH,(Dir)e.EDir,e.EVictim,e.EActor) == ABORT)
                                                {
                                                    /* Later, Inflict Impact Damage? */
                                                    break;
                                                }
                                                if (curr != e.EVictim->x+e.EVictim->y*256 && !e.Terse)
                                                    e.EVictim->IDPrint("You are knocked back!",
                                                    "The <Obj> is forced backward!",e.EVictim);
                                        }
                                        break;
                                    case AD_SHAT:
                                        if (e.DType == AD_SHAT && e.EVictim->isMType(MA_CONSTRUCT)) {
                                            VPrint(e,"Your construct body shatters!",
                                                "The <EVictim>'s construct body shatters !");
                                            e.DType = AD_NORM;
                                            ThrowDmg(EV_DAMAGE,e.DType,e.vDmg,"shattering",e.EActor,e.EVictim);
                                            if (e.EVictim->isDead())
                                                return DONE; 
                                        } 
                                        e.DType = AD_BLUNT; 
                                    case AD_DCAY:
                                        if (e.DType == AD_DCAY && e.EVictim->isMType(MA_PLANT)) {
                                            VPrint(e,"You wither and decay!",
                                                "The <EVictim> withers and decays!");
                                            e.DType = AD_NORM;
                                            ThrowDmg(EV_DAMAGE,e.DType,e.vDmg,"decay",e.EActor,e.EVictim);
                                            if (e.EVictim->isDead())
                                                return DONE; 
                                            e.DType = AD_DCAY;
                                        } 
                                        // fall through!
                                    case AD_RUST:
                                        /* 1/4 chance of damaging: weapon, shield, armour, random 
                                        * unexposed item */
                                        { 
                                            // ww: bugfix: if you are a wild-shaped druid and your equipment
                                            // has merged with your body, it won't rust! 
                                            if ((it = e.EVictim->EInSlot(SL_WEAPON)) && it->Hardness(e.DType) != -1 && !random(4))
                                                (void)0;
                                            if ((it = e.EVictim->EInSlot(SL_READY)) && it->Hardness(e.DType) != -1 && !random(3)) {
                                                (void)0;
                                            } else if ((it = e.EVictim->EInSlot(SL_ARMOUR)) && it->Hardness(e.DType) != -1 && !random(2)) {
                                                (void)0;
                                            } else {
                                                TMonster * tm = TMON(e.EVictim->mID);
                                                for(c=0,it=e.EVictim->FirstInv();it;it=e.EVictim->NextInv())
                                                    if (it->Hardness(e.DType) != -1 && it->GetParent() == e.EVictim){
                                                        bool inGoodSlot = false;
                                                        for (int s=0; s<SL_LAST; s++)
                                                            if (it == e.EVictim->EInSlot(s) && tm->HasSlot(s))
                                                                inGoodSlot = true;
                                                        if (inGoodSlot) 
                                                            Candidates[c++] = it->myHandle;
                                                    }
                                                    if (c)
                                                        it = oItem(Candidates[random(c)]);
                                            }
                                            if (it)
                                                ThrowDmg(EV_DAMAGE,e.DType,e.vDmg,NULL,e.EActor,it);
                                            else
                                                e.Immune = true;
                                            break;
                                        }
                                    case AD_SOAK:
                                        e.EVictim->IDPrint("You get soaked!","The <Obj> gets soaked!",e.EVictim);
                                        for(c=0,it=e.EVictim->FirstInv();it;it=e.EVictim->NextInv()) {
                                            bool inside = (it->GetParent() != e.EVictim);
                                            int dmg = e.vDmg;
                                            if (inside && it->GetParent()->isItem()) {
                                                int hard = ((Item *)(it->GetParent()))->Hardness(AD_SOAK);
                                                if (hard == -1) dmg /= 2;
                                                else dmg -= hard; 
                                                if (dmg <= 0) {
                                                    continue;
                                                }
                                            } 
                                            if (it->Hardness(AD_SOAK) != -1) {
                                                /* e.EVictim->IPrint(Format("Soaking %s for %d.", (const char*)(it->Name()),dmg)); */
                                                ThrowDmg(EV_DAMAGE,e.DType,dmg,NULL,e.EActor,it);
                                            } else if (it->Type == T_LIGHT && it->Material() != MAT_FORCE) {
                                                /* average water elemental soak = 3d4, average torch
                                                * age = 3000, getting soaked once should destroy your
                                                * torch */
                                                it->Age -= (int16)((dmg * 400) / it->Quantity); 
                                                if (it->Age <= 1) {
                                                    IDPrint("Your <Obj2> is soaked and destroyed!",
                                                        "The <Obj1>'s <Obj2> is soaked destroyed!",this,it);
                                                    it->Remove(true);
                                                    if (e.EVictim->isPlayer())
                                                        ((Player *)e.EVictim)->UpdateMap = true;
                                                }
                                            } 
                                        }
                                        if (e.EVictim->isMType(MA_ELEMENTAL) && e.EVictim->isMType(MA_FIRE))
                                            ThrowDmg(EV_DAMAGE,AD_NORM,e.vDmg*2,"getting soaked",e.EActor,e.EVictim);
                                        break;
                                    case AD_STEA:
                                        ASSERT(e.EActor);
                                        if (e.EVictim->SavingThrow(REF,e.saveDC,SA_THEFT)) 
                                        { e.Resist = true; break; }
                                        for(c=0,it=e.EVictim->FirstInv();it;it=e.EVictim->NextInv())
                                            if (it != EInSlot(SL_PACK) && it != EInSlot(SL_ARMOUR) && it != EInSlot(SL_WEAPON) &&
                                                it->GetParent() == e.EVictim && it != EInSlot(SL_BRACERS) && it !=
                                                EInSlot(SL_BOOTS)) 
                                                Candidates[c++] = it->myHandle;
                                        if (c) {
                                            it = oItem(Candidates[random(c)]);
                                            if (it->Quantity > (uint32)e.vDmg) {
                                                it->Quantity = it->Quantity - e.vDmg + 1;
                                                it = it->TakeOne();
                                                it->Quantity = e.vDmg;
                                            }
                                            it->Remove(false);
                                            e.EActor->IPrint("You stole an <Obj>!",it);
                                            e.EVictim->IPrint("Your <Obj1> <Str2> stolen!",it,
                                                it->Quantity > 1 ? "were" : "was");
                                            e.EActor->GainItem(it,false);
                                            e.EActor->StateFlags &= ~MS_INVEN_GOOD;
                                            if (e.EActor->isMonster() && e.EActor->HasMFlag(M_SKIRMISH))
                                                if (!HasStati(AFRAID) && !random(2))
                                                    e.EActor->GainTempStati(AFRAID,NULL,10+random(10),SS_MONI,
                                                    FEAR_SKIRMISH);
                                        }
                                        break;
                                    case AD_STUN:
                                        if (e.EVictim->HasStati(STUNNED))
                                            return ABORT;          
                                        if (!e.EVictim->SavingThrow(FORT,e.saveDC,SA_PARA)) {
                                            if (!e.Terse)
                                                VPrint(e,"You are stunned!", "The <EVictim> is stunned!"); 
                                            e.EVictim->GainTempStati(STUNNED,0,SS_ATTK,(int8)e.vDmg);
                                        }
                                        else
                                            e.Resist = true;
                                        break;
                                    case AD_CHOK:
                                        {
                                            if (!e.EVictim->isMType(MA_BREATHER)) {
                                                e.Immune = true; 
                                                break;
                                            } 
                                            if (e.EVictim->isMType(MA_AIR))
                                                e.vDmg *= 2;
                                            e.DType = AD_NORM;
                                            if (!e.Terse)
                                                VPrint(e,"You choke and cannot breathe!",
                                                "The <EVictim> chokes and cannot breathe!");
                                            ReThrow(EV_DAMAGE,e);
                                            break; 
                                        } 
                                        /*case AD_SGLD:
                                        if (e.EVictim->SavingThrow(REF,e.saveDC,SA_THEFT)) {
                                        for(c=0,it=e.EVictim->FirstInv();it;it=e.EVictim->NextInv())
                                        if (it->isType(T_COIN)) 
                                        Candidates[c++] = it->myHandle;
                                        if (c) {
                                        it = oItem(Candidates[random(c)]);
                                        if (it->Quantity > e.vDmg)
                                        {
                                        it->Quantity = it->Quantity - e.vDmg + 1;
                                        it = it->TakeOne();
                                        it->Quantity = e.vDmg;
                                        }
                                        it->Remove(false);
                                        e.EActor->IPrint("You stole an <Obj>!",it);
                                        e.EVictim->IPrint("A <Obj> was stolen!",it);
                                        e.EActor->GainItem(it,false);
                                        e.EActor->StateFlags &= ~MS_INVEN_GOOD;
                                        }
                                        break;*/
                                    case AD_SPE1:
                                    case AD_SPE2:
                                        Error("Undefined special attack damage type AD_SPE%d for monster %s vs. monster %s!",
                                            (e.DType - AD_SPE1) + 1, (const char*) e.EActor->Name(0), 
                                            (const char*) e.EVictim->Name(0));
                                        return DONE;
                                    case AD_BLEE:
                                        if (!e.EVictim->isMType(MA_HAS_BLOOD))
                                            return ABORT;          
                                        if (!e.Terse)
                                            VPrint(e,"You start bleeding!", "The <EVictim> starts bleeding!");
                                        // ww: "Mag" is how much bleeding damage is done each turn:
                                        // here we've made it "2"
                                        e.EVictim->GainTempStati(BLEEDING,NULL,e.vDmg,SS_ATTK,1,2);
                                        break; 

                                    case AD_SLEE: 
                                        if (e.vDmg == 0) { 
                                            e.vDmg = Dice::Roll(3,4);
                                        }
                                        /*
                                        VPrint(e,"You are hit by sleep damage!",
                                        (const char*)Format("The <EVictim> is hit by %d sleep damage!",e.vDmg));
                                        */
                                        e.EVictim->GainTempStati(ASLEEP,NULL,e.vDmg,SS_ENCH,SLEEP_MAGIC);
                                        break; 
                                    case AD_FALL:
                                        if (HasAbility(CA_SLOW_FALL))
                                        {
                                            if (e.vDmg < AbilityLevel(CA_SLOW_FALL)*5)
                                            {
                                                IPrint("You control your fall.");
                                                e.Immune = true;
                                                return DONE;
                                            }
                                            else
                                            {
                                                IPrint("You partially control your fall.");
                                                e.vDmg -= AbilityLevel(CA_SLOW_FALL)*5;
                                                e.Resist = true;
                                            }
                                        }
                                        if (HasSkill(SK_TUMBLE) && SkillCheck(SK_TUMBLE,20,false))
                                        {
                                            IPrint("You land gracefully, lessening the impact.");
                                            e.vDmg /= 2;
                                        }
                                        else {
                                            SetSilence();
                                            if (!e.EVictim->HasStati(PRONE))
                                                e.EVictim->GainPermStati(PRONE,NULL,SS_MISC);
                                            UnsetSilence();
                                            MakeNoise(10+e.vDmg*2);
                                            Creature *cr;
                                            for (cr=m->FCreatureAt(x,y);cr;cr=m->NCreatureAt(x,y))
                                                if (cr != this)
                                                {
                                                    cr->GainPermStati(PRONE,NULL,SS_MISC);
                                                    ThrowDmg(EV_DAMAGE,AD_NORM,e.vDmg,XPrint("having a <Obj> fall on <him:Obj2>",
                                                        this, cr), this, cr);
                                                    String s;
                                                    s = XPrint(", killing <him:Obj>",cr);
                                                    VPrint(e, "You land on an <Obj><str>! Crunch!", 
                                                        "The <EVictim> lands on a <Obj><str>! Crunch!", cr, cr->isDead() ?
                                                        (const char *) s : "");
                                                    // soft landing :-)
                                                    e.vDmg /= 2;
                                                    // only land on one creature.
                                                    goto SkipSplat;
                                                } 
                                                if (e.vDmg >= 13)
                                                    IPrint("Splat.");
SkipSplat:;
                                        }

                                        if (HasStati(MOUNTED))
                                        {
                                            Creature *mount = (Creature*) GetStatiObj(MOUNTED);
                                            ThrowDmg(EV_DAMAGE,AD_FALL,e.vDmg, XPrint(
                                                "falling into a <Res>",m->TerrainAt(x,y)),mount,mount);
                                            if (!mount->isDead())
                                                ThrowVal(EV_DISMOUNT,DSM_FALL,this);
                                        }            
                                        e.DType = AD_NORM;
                                        if (!(e.Immune || e.Resist))
                                            GainTempStati(STUNNED,NULL,Dice::Roll(1,4,2),SS_MISC);
                                        return ReThrow(EV_DAMAGE,e);
                                    case AD_DISN:
                                        xID = FIND("disintegrate");
                                        ASSERT(xID);
                                        ThrowEff(EV_EFFECT,xID,e.EActor ? e.EActor : e.EVictim,e.EVictim);
                                        break;
                                    case AD_LEGS:
                                    case AD_STCK:
                                    case AD_SGLD:
                                    case AD_TLPT:
                                    case AD_CURS:
                                    case AD_WRAP:
                                    case AD_WERE:
                                    case AD_SUBD:
                                    default:
                                        Error("Undefined attack damage-effect [%s] to [%s] from [%s]'s [%s]!",Lookup(DTypeNames,e.DType),
                                            (const char*)e.EVictim->Name(),
                                            (const char*)e.EActor->Name(),
                                            NAME(e.eID));
                                        return ABORT;
                                    }
                                    return DONE;
}

EvReturn Creature::PDamage(EventInfo &e)
  {
    if (e.effIllusion && e.effDisbelieved)
      return DONE;
      
    if (e.aDmg >= e.EVictim->cHP)
      {
        ThrowDmg(EV_DAMAGE,AD_DACO,1+random(2),"the strain of psychosomatic death",
                                  e.EActor, e.EVictim);
        if (e.EVictim->isDead())
          return DONE;
        VPrint(e,"The world goes dark...",
                 "The <EVictim> collapses unconscious.");
        e.Terse = true;
        /* Psychosomatic death cures all illusory damage. */
        e.EVictim->RemoveStati(ILLUS_DMG);
        e.EVictim->GainTempStati(ASLEEP,NULL,Dice::Roll(3,6),SS_MISC,
           SLEEP_PSYCH,0,0,0);
        /* When someone collapses instead of dying, that's a good
           cause for a disbelief check. Advanced illusions cover
           this with an illusion of death or mortal wounding, 
           however. */
        if (e.actIllusion && !(e.EActor->GetStatiVal(ILLUSION) & IL_ADVANCED))
          e.EActor->DisbeliefCheck();
      }
    else
      {
        /* Grant or increment ILLUS_DMG. Spectral illusions give damage
           lasting 3d6 x 100 turns, other illusions' psychosomatic damage
           lasts only 3d6 turns. */
        e.EVictim->IncStatiMag(ILLUS_DMG,SS_MISC,0,e.EActor,e.aDmg);        
        /* Note that EActor might not have ILLUSION if the psych damage
           comes from hallucinatory terrain -- but it's still good because
           GetStatiVal will return -1, which includes IL_SPECTRAL, and all
           terrain illusions are assumed to be spectral by default. */
        e.EVictim->SetStatiDur(ILLUS_DMG,-1,e.EActor, max(
           e.EVictim->GetStatiMag(ILLUS_DMG,-1,e.EActor), Dice::Roll(3,6) *
           ( e.EActor->GetStatiVal(ILLUSION) & IL_SPECTRAL ? 100 : 1 ) ) );    
        e.EVictim->cHP -= e.aDmg;
      }
    return DONE;
  }


EvReturn Creature::BullRush(EventInfo &e)
  {
    int16 tx,ty; Creature *c, *ea;
    /* There are currently some known inadequicies with this. You can
       bull rush an incorporeal creature standing in solid rock while
       corporeal, and end up passing through the rock, for example.
       This code will be expanded upon soon. */
    e.EDir = e.EActor->DirTo(e.EVictim);
    switch(e.EParam)
      {
        case 'f':
          tx = e.EVictim->x+DirX[e.EDir];
          ty = e.EVictim->y+DirY[e.EDir];
          if (!e.EActor->m->OpenAt(tx,ty))
            {
              if (e.EActor->m->SolidAt(tx,ty) && e.EActor->isCorporeal()) {
                VPrint(e,"You crash into something solid!",
                         "The <EVictim> crashes into something solid!");
                ThrowDmg(EV_DAMAGE,AD_NORM,Dice::Roll(1,6)+e.EActor->Mod(A_STR),
                  XPrint("an <Obj>'s bull rush", e.EVictim), e.EVictim, e.EVictim);
                }
              else for (c=e.EActor->m->FCreatureAt(tx,ty);c;c=e.EActor->m->NCreatureAt(tx,ty)) {
                ea = e.EActor; e.EActor = c;
                TPrint(e,"The <EVictim> crashes into you!",
                         "You crash into the <EActor>!",
                         "The <EVictim> crashes into the <EActor>!");
                e.EActor = ea;
                ThrowDmg(EV_DAMAGE,AD_BLUNT,Dice::Roll(1,3)+e.EActor->Mod(A_STR),
                  XPrint("colliding with an <Obj>", e.EVictim), c, c);
                ThrowDmg(EV_DAMAGE,AD_BLUNT,Dice::Roll(1,6)+e.EActor->Mod(A_STR),
                  XPrint("colliding with an <Obj>", e.EVictim), e.EVictim, e.EVictim);
                }
              
            }
          else
            {
              if (ThrowXY(EV_PUSH,tx,ty,e.EVictim,e.EActor) == ABORT)
                return ABORT;
              tx = e.EActor->x+DirX[e.EDir]; ty = e.EActor->y+DirY[e.EDir];
              if (ThrowXY(EV_PUSH,tx,ty,e.EActor) == ABORT)
                return ABORT;
              VPrint(e,"You are forced back!", "The <EVictim> is forced back!");
            }
         break;
        case 'd':
          DoDisplace:
          tx = e.EActor->x;
          ty = e.EActor->y;
          if (ThrowXY(EV_PUSH,e.EVictim->x,e.EVictim->y,e.EActor,e.EVictim) == ABORT)
            break;
          if (ThrowXY(EV_PUSH,tx,ty,e.EVictim,e.EActor) == ABORT)
            break;
          TPrint(e,"You displace the <EVictim>.",
                   "The <EActor> displaces you.",
                   "The <EActor> displaces the <EVictim>.");
         break;
        case 'm':
          /* Target the spot immediately past EVictim */
          e.EDir = e.EActor->DirTo(e.EVictim);
          tx = e.EVictim->x + DirX[e.EDir];
          ty = e.EVictim->y + DirY[e.EDir];
          if (!e.EActor->m->OpenAt(tx,ty))
            goto DoDisplace;
          if (ThrowXY(EV_PUSH,tx,ty,e.EActor,e.EVictim) == ABORT)
            break;
          TPrint(e,"You charge past the <EVictim>.",
                   "The <EActor> charges past you.",
                   "The <EActor> charges past the <EVictim>.");
         break;
      }
    return DONE; // ww: previously returned no value! 
  }

EvReturn Creature::Death(EventInfo &e)
{
  int16 ox = x,oy = y, i; Map *om = m;
  Corpse *c;
  c = NULL;
  if (e.EVictim->isDead())
    return ABORT;
  else if (this != e.EVictim)
    return NOTHING; 

    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
      e.EItem = NULL;


  if (HasStati(TRANSFORMED))
    {
      RemoveStati(TRANSFORMED);
      // Creature::StatiOff does everything else.
      return DONE;
    } 
    
  if (HasMFlag(M_IALIGN) && e.EActor != e.EVictim)
    if (e.EActor->isCharacter())
      {
        uint16 dAlign = e.EPActor->desiredAlign;
        if (isMType(MA_EVIL) && (dAlign & AL_GOOD))
          e.EActor->AlignedAct(AL_GOOD,1);
        if (isMType(MA_CHAOTIC) && (dAlign & AL_LAWFUL))
          e.EActor->AlignedAct(AL_LAWFUL,1);
        if (isMType(MA_LAWFUL) && (dAlign & AL_CHAOTIC))
          e.EActor->AlignedAct(AL_CHAOTIC,1);
        if (isMType(MA_GOOD))
          e.EActor->AlignedAct(AL_EVIL,2,
            "killing a good creature");
      }
  
  if (e.EActor != e.EVictim)
    if (HasStati(WAS_FRIENDLY,-1,e.EActor) && !isMType(MA_EVIL))
      e.EActor->AlignedAct(AL_EVIL|AL_NONGOOD,3 + isMType(MA_GOOD)*2,
        "unprovoked killing");
    
  if (HasStati(ENGULFER))
    DropEngulfed();
    
    
  GainPermStati(DEATH_LOC,NULL,SS_MISC,x+y*256,m->Depth,0,0);
    
  Flags |= F_DELETE;
  if (HasAttk(A_AURA) && isBeside(e.EActor) && !e.EActor->isDead())
    ThrowVal(EV_SATTACK,A_AURA,this,e.EActor);
  if (HasAttk(A_DETH) && !e.EActor->isDead())
    ThrowVal(EV_SATTACK,A_DETH,this,e.EActor);
  if (HasAttk(A_DEQU) && isBeside(e.EActor) && !e.EActor->isDead())
    ThrowVal(EV_SATTACK,A_DEQU,this,e.EActor);
    
    
  e.Died = true;
  ASSERT(isDead());
  // ww: was this a druid's animal companion?  
  StatiIterNature(this,ANIMAL_COMPANION)
      Creature *c = oCreature(S->h);
      if (c) c->IPrint(
          Format("%c%s, your animal companion, is no longer with you.%c",
            -RED, (const char*)Name(0), -GREY));
  StatiIterEnd(this)
  if (HasAttk(A_DETH))
    ThrowVal(EV_SATTACK,A_DETH,this,e.EActor);
  if (HasStati(POLYMORPH)) {
    IDPrint(NULL,"In death, the <Obj1> reverts to <his:Obj1> true form -- "
        "that must have been a<str> <Res>!",this,
        strchr("aeiouAEIOU",NAME(tmID)[0]) ? "n" : "", tmID);
  }
  else if (HasAttk(A_SEEM) && !(StateFlags & MS_KNOWN)) {
    StateFlags |= MS_KNOWN;
    IDPrint(NULL,"That must have been a<str> <Res>!",
      strchr("aeiouAEIOU",NAME(tmID)[0]) ? "n" : "", tmID);
  }

  if (e.EActor && !e.EVictim->isIllusion())
    e.EActor->KillXP(this);
  if (m) {
    c = new Corpse(this);
    GainPermStati(MY_CORPSE_IS,c,SS_MISC);
    if (HasMFlag(M_INCOR) || HasStati(PHASED) || HasAbility(CA_PHASE))
      {
        int16 plane;
        plane = 0;
        if (HasStati(PHASED))
          plane = GetStatiVal(PHASED);
        else if (HasMFlag(M_INCOR)) {
          if (isMType(MA_UNDEAD))
            plane = PHASE_NEGATIVE;
          else if (HasMFlag(M_ASTRAL))
            plane = PHASE_ASTRAL;
          else
            plane = PHASE_ETHEREAL;
          }
        else if (isMType(MA_OUTSIDER))
          plane = AbilityLevel(CA_PHASE);
        if (plane)
          c->GainPermStati(PHASED, NULL, SS_MISC, plane);
      }
        
      
      
    if (!HasMFlag(M_NO_CORPSE) && !isIllusion()
          && !HasStati(SUMMONED))
      c->PlaceAt(m,ox,oy);
    else
      c->Remove(true);
  }
 
  if (HasStati(MOUNTED)) {
    ThrowVal(EV_DISMOUNT,DSM_THROWN,this);
  } 
  if (HasStati(MOUNT)) {
    Thing *rider = GetStatiObj(MOUNT);
    ThrowVal(EV_DISMOUNT,DSM_THROWN,rider);
  } 

  /* Killing frightened creatures increases an evil character's
     reputation for ruthlessness and tyrrany, thus training their
     charisma. */
  if (HasStati(AFRAID) && e.EActor && e.EActor != e.EVictim && e.EActor->isMType(MA_EVIL))
    e.EActor->Exercise(A_CHA,random(4)+1,ECHA_KILLFEAR,35);
    
  if (e.EActor && e.EActor->HasSkill(SK_INTIMIDATE) && e.EActor->SkillLevel(SK_INTIMIDATE) >= 10
        && ((!e.EActor->isPlayer()) || (e.EPActor->Opt(OPT_DEMORALIZE))) && e.EActor->m)
    if (e.isCrit || e.EActor->HasFeat(FT_IMPROVED_DEMORALIZE))
      {
        Creature *cr; int16 pen, bonus;
        pen = (e.EActor->SkillLevel(SK_INTIMIDATE)-5)/5;
        MapIterate(m,cr,i)
          if (cr->isCreature() && !cr->HasMFlag(M_MINDLESS) && cr->isFriendlyTo(this))
            if (cr != this && cr->Perceives(e.EActor) && !cr->isPlayer())
              {
                if (cr->ResistLevel(AD_FEAR) == -1)
                  continue;
                bonus = cr->SumStatiMag(SAVE_BONUS,SN_FEAR);
                if (bonus >= pen)
                  continue;
                EventInfo e2;
                e2.Clear();
                e2.EActor = e.EActor;
                e2.EVictim = cr;
                if (cr->ChallengeRating() <= (e.EActor->HasFeat(FT_IMPROVED_DEMORALIZE) ? 5 : 3))
                  {
                    cr->GainTempStati(AFRAID,e.EActor,Dice::Roll(3,6),
                      SS_MISC,FEAR_COWED,0,FIND("Demoralize"),0);
                    
                    TPrint(e2,"Seeing you kill the <Obj>, the <EVictim> turns to flee!",
                              NULL,"Seeing the <EActor> kill the <Obj>, the <EVictim> "
                              "turns to flee!", e.EVictim);
                  }
                else
                  {
                    cr->GainTempStati(ADJUST_MOR,e.EActor,Dice::Roll(3,6),
                      SS_MISC,A_AID,bonus - pen,FIND("Demoralize"),0);
                    TPrint(e2,"Seeing you kill the <Obj>, the <EVictim> looks shaken!",
                              NULL,"Seeing the <EActor> kill the <Obj>, the <EVictim> "
                              "looks shaken!", e.EVictim);
                  }
                /* Crash bug paranoia */
                if (!e.EActor->m)
                  break;                
              }
      }
  
  if (e.EActor && e.EActor->HasFeat(FT_BATTLEFIELD_INSPIRATION))
    if (e.EVictim->ChallengeRating() >= e.EActor->ChallengeRating())
      {
        int16 mod = 1 + e.EVictim->ChallengeRating() - 
                        e.EActor->ChallengeRating();
        rID eID = FIND("Battlefield Inspiration");
        Creature *cr; int16 i, dur;
        MapIterate(e.EActor->m,cr,i)
          if (cr->isCreature() && cr->DistFrom(e.EActor) < 13)
            if (cr->isFriendlyTo(e.EActor) && cr->Perceives(e.EActor) &&
                  cr != e.EActor)
              {
                int16 curMod, curDur;
                curMod = 0;
                dur = Dice::Roll(3,6) * 10;
                if (cr->HasEffStati(ADJUST_MOR,eID,-1,e.EActor))
                  {
                    curDur = cr->GetEffStatiDur(ADJUST_MOR,eID,-1,e.EActor);
                    curMod = cr->GetEffStatiMag(ADJUST_MOR,eID,-1,e.EActor);
                    cr->SetEffStatiDur(ADJUST_MOR,eID,curDur + dur,-1,e.EActor);
                    cr->SetEffStatiMag(ADJUST_MOR,eID,max(curMod,mod),-1,e.EActor);
                  }
                else
                  cr->GainTempStati(ADJUST_MOR,e.EActor,dur,SS_MISC,A_AID,mod,eID);
                cr->IPrint("You are <Str>inspired by the <Obj>'s heroism!",
                            curMod ? "further " : "", e.EActor);
                e.EActor->IPrint("You <Str>inspire the <Obj>!",
                            curMod ? "further " : "", cr);
              }
      }

  RemoveStati(SUMMONED);

  if (!isIllusion())
    DropAll();
  Timeout = 0;              
    
  if (m) 
    m->Update(ox,oy);
  
  {
    Feature *ft; EventInfo xe;
    for (ft=om->FFeatureAt(ox,oy);ft;ft=om->NFeatureAt(ox,oy))
      if (ft->Flags & F_ALTAR && e.EActor && e.EActor->isCharacter())
        if (!(e.EVictim->HasStati(SUMMONED) || e.EVictim->HasStati(ILLUSION)))
          {
            xe.Clear();
            xe.EActor = e.EActor;
            xe.EVictim = e.EVictim;
            xe.EItem = c;
            xe.eID = ft->GetStatiEID(MY_GOD);
            ReThrow(EV_SACRIFICE,xe);
            break;
          }
  }     
   
  if (e.EActor)
    FocusCheck(e.EActor);  
  Remove(true);
  x = y = -1;  
  return DONE;
}

EvReturn Player::Death(EventInfo &e)
{
  if (e.EVictim->isDead())
    return ABORT;

    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
      e.EItem = NULL;


  if (HasAttk(A_DETH))
    ThrowVal(EV_SATTACK,A_DETH,this,e.EActor);
  if (thisp->Opt(OPT_NODEATH)) {
    if (!thisp->MyTerm->yn("You die... Die?"))
      {
        cHP = mHP+Attr[A_THP]; e.Died = false;
        RemoveStati(ADJUST_DMG);
        RemoveStati(POISONED);
        RemoveStati(DISEASED);  
        AddJournalEntry("You cheat death.");
        deathCount++;
        if (AttrDeath) {
          AttrDeath = 0;
          RemoveStati(ADJUST_DMG);
        }
        return DONE;
      }
    }
  else if (Opt(OPT_ELUDE_DEATH)) {
    int32 maxdeaths;
    switch (Opt(OPT_ELUDE_DEATH)) {
      case 1: maxdeaths = 1; break;
      case 2: maxdeaths = 3; break;
      case 3: maxdeaths = 7; break;
      case 4: maxdeaths = 10000000; break;
      }
    if ((deathCount < maxdeaths) &&
          !thisp->MyTerm->yn("You die... Die?"))
      {
        cHP = mHP+Attr[A_THP]; e.Died = false;
        RemoveStati(ADJUST_DMG);
        RemoveStati(POISONED);
        RemoveStati(DISEASED);  
        AddJournalEntry("You cheat death.");
        deathCount++;
        ExploreMode = true;
        if (maxdeaths < 10)
          {
            int32 XPCost;
            XPCost = TotalXP();
            XPCost *= 15;
            XPCost /= 100;
            XPCost = max(750,XPCost);
            IPrint("You lose <Num> xp.", min(TotalXP(),XPCost));
            LoseXP(XPCost);
          }
        if (AttrDeath) {
          AttrDeath = 0;
          RemoveStati(ADJUST_DMG);
          }
        PlaceSomewhereSafe();
        return DONE;
      }    
    }
  else
  { 
    SetSilence();
  }
  (new Corpse(this))->PlaceAt(m,x,y);
  DropAll();
  e.EVictim->Flags |= F_INVIS;
  m->Update(x,y);
  Flags |= F_DELETE;
  ASSERT(isDead());
  MyTerm->ShowStatus();

  if (e.GraveText)
    GraveText = e.GraveText;
  else if (e.EActor)
    GraveText = e.EActor->Name(NA_A);
  else
    GraveText = "Dungeon Perils";
    
  return DONE;
}

/* ww: we assume that 'this' actually has quality q --  it may be
 * a normal quality or one granted by a spell (e.g., Flame Blade) or
 * whatever ... 
 *
 * this should basically only be called by QualityDmg below! */
void Weapon::QualityDmgSingle(EventInfo &e, int32 q)
{
  bool id = false; 
  const char *txt1, *txt2, *txt3, *txt4;
  int16 n; 
  switch (q) {
    case WQ_VAMPIRIC: 
      if (!e.ETarget->isCreature()) break; 
      if (e.EVictim->isMType(MA_LIVING) && 
          e.EVictim->ResistLevel(AD_NECR) != -1)
        if (e.EActor->cHP < e.EActor->mHP+e.EActor->Attr[A_THP]) 
          /* ww: this logic was backwards before! */
        {
          e.EActor->cHP = min(e.EActor->cHP + e.vDmg / 4,
              e.EActor->mHP+e.EActor->Attr[A_THP]);
          DPrint(e,"Your wounds heal<str>!",
              "The <EActor>'s wounds heal<Str>!",
              (e.EActor->mHP+e.EActor->Attr[A_THP]) == 
              e.EActor->cHP ? " fully" : "");
          id = true;
        }
      break;

    case WQ_BALANCE:
      {
      if (!e.ETarget->isCreature()) break; 
      if (!e.EVictim->isMType(MA_NEUTRAL)) break; 
      if (e.EActor->isMType(MA_GOOD) || e.EActor->isMType(MA_EVIL) ||
          e.EActor->isMType(MA_LAWFUL) || e.EActor->isMType(MA_CHAOTIC))
        break; 
      txt1 = "The power of Balance"; txt2 = "balanced";
      txt3 = "grey radiance"; txt4 = "magic";
      DmgType = AD_MAGC;
      goto AlignmentQuality;
      }

    case WQ_UNHOLY:
      if (!e.ETarget->isCreature()) break; 
      if (!e.EActor->isMType(MA_EVIL)) break; 
      if (!e.EVictim->isMType(MA_GOOD))
        break;
      txt1 = "Corrosive entropy"; txt2 = "impure";
      txt3 = "oily black anti-light"; txt4 = "unholy";
      DmgType = AD_EVIL;
      goto AlignmentQuality;
    case WQ_LAWFUL:
      if (!e.ETarget->isCreature()) break; 
      if (!e.EActor->isMType(MA_LAWFUL)) break; 
      if (!e.EVictim->isMType(MA_CHAOTIC))
        break;
      txt1 = "The binding power of law"; txt2 = "righteous";
      txt3 = "rich azure radience"; txt4 = "lawful";
      DmgType = AD_LAWF;
      goto AlignmentQuality;
    case WQ_CHAOTIC:
      if (!e.ETarget->isCreature()) break; 
      if (!e.EActor->isMType(MA_CHAOTIC)) break; 
      if (!e.EVictim->isMType(MA_LAWFUL))
        break;
      txt1 = "Primordial chaos"; txt2 = "discordant";
      txt3 = "roiling red energy"; txt4 = "chaotic";
      DmgType = AD_CHAO;
      goto AlignmentQuality;
    case WQ_HOLY:
      if (!e.ETarget->isCreature()) break;
      if (!e.EActor->isMType(MA_GOOD)) break; 
      if (!e.EVictim->isMType(MA_EVIL))
        break;
      txt1 = "Holy purity"; txt2 = "sacred";
      txt3 = "white light"; txt4 = "holy";
      DmgType = AD_HOLY;
AlignmentQuality:
      n = Dice::Roll(2,6); e.xDmg += n;
      e.strXDmg += Format(" +2d6 %s",txt4);
      ThrowDmg(EV_DAMAGE, DmgType, n,
          txt1,e.EActor,e.EVictim);
      switch(random(3)) {
        case 1: VPrint(e,"<Str> sears your flesh!",
                    "<Str> sears the <EVictim>'s flesh!",
                    txt1); break;
        case 2: TPrint(e,"The <EVictim> recoils from your <Str> weapon!",
                    "You recoil from the <EActor>'s <Str> weapon!",
                    "The <EVictim> recoils from the <EActor>'s <Str> weapon!",
                    txt3); break;
        case 3: VPrint(e,"A nimbus of <Str> blasts you!",
                    "A nimbus of <Str> blasts the <EVictim>!",
                    txt3); break;
      }
      id = true;
      break;
    case WQ_FLAMING:
      txt1 = "Flames burn";
      txt2 = "erupt in flame";
      txt3 = "erupts in flames";
      txt4 = "fire";
      DmgType = AD_FIRE;
      goto ElemQuality;
    case WQ_THIRSTY:
      if (e.ETarget->isCreature() && e.EVictim->isMType(MA_HAS_BLOOD)) {
        txt1 = "Blood spurts from";
        txt2 = "bleed freely";
        txt3 = "bleeds freely";
        txt4 = "blood";
        DmgType = AD_NORM;
        goto ElemQuality;
      } else break; 
    case WQ_WOUNDING:
      VPrint(e,"You start bleeding!", "The <EVictim> starts bleeding!");
      // ww: "Mag" is how much bleeding damage is done each turn:
      // here we've made it "1"
      e.EVictim->GainTempStati(BLEEDING,NULL,Dice::Roll(2,12),SS_ENCH,1,1);
      id = true;
      break;            
    case WQ_SOULDRINK:
      txt1 = "Negative energy blasts";
      txt2 = "are struck by negative energy";
      txt3 = "is struck by negative energy";
      txt4 = "necro";
      DmgType = AD_NECR;
      goto ElemQuality;
    case WQ_THUNDERING:
      txt1 = "Thunder blasts";
      txt2 = "are struck by thunderous force";
      txt3 = "is struck by thunderous force";
      txt4 = "sonic";
      DmgType = AD_SONI;
      goto ElemQuality;
    case WQ_FROST: 
      txt1 = "Frost sears";
      txt4 = "cold";
      if (e.ETarget->isCreature() && e.EVictim->isMType(MA_LIVING)) {
        txt2 = "are frozen alive";
        txt3 = "is frozen alive";
      }
      else {
        txt2 = "are blasted by cold";
        txt3 = "is blasted by cold";
      }                        
      DmgType = AD_COLD;
      goto ElemQuality;
    case WQ_SHOCKING:
      txt1 = "Lightning shocks";
      txt2 = "are engulfed in a vortex of electricity";
      txt3 = "is engulfed in a vortex of electricity";
      DmgType = AD_ELEC;
      txt4 = "lightning";
      goto ElemQuality;
    case WQ_CORROSIVE:
      txt1 = NULL;
      if (e.isCrit && HasQuality(WQ_BURST))
        TPrint(e,"Your weapon's corrosive aura sears the <EVictim>!",
            "The <EActor>'s weapon's corrosive aura sears you!",
            "The <EActor>'s weapon's corrosive aura sears the <EVictim>.");
      else
        VPrint(e,"Corrosive energies melt your flesh!",
            "Corrosive energies melt the <EVictim>'s flesh!");
      DmgType = AD_ACID;
      txt4 = "acid";

ElemQuality:
      // WW: SRD: Swarms take full damage from elemental weapon damage
      {
      bool isSwarm = e.ETarget->isCreature() && e.EVictim->HasMFlag(M_SWARM);
      
      if (e.isCrit && HasQuality(WQ_BURST)) {
        if (isSwarm) { 
          n = CritMult(e.EActor) * 10; 
          e.strXDmg += Format(" +%d %s",n,txt4);
        } else { 
          n = Dice::Roll((int8)CritMult(e.EActor),10);
          e.xDmg += n;
          e.strXDmg += Format(" +%dd10 %s",CritMult(e.EActor),txt4);
        }
        e.xDmg += n;
        if (txt1)
          VPrint(e,"You <Str1>!", "The <EVictim> <Str2>!",txt2,txt3); 
        ThrowDmg(EV_DAMAGE,DmgType,n, Name(NA_A),e.EActor,e.EVictim);
      }
      else {
        if (isSwarm) { 
          e.strXDmg += Format(" +6 %s",txt4); 
          n = 6; 
        } else { 
          e.strXDmg += Format(" +1d6 %s",txt4); 
          n = Dice::Roll(1,6); 
        }
        e.xDmg += n;
        if (txt1)
          VPrint(e,"<Str1> you!", "<Str1> the <EVictim>!",txt1); 
        ThrowDmg(EV_DAMAGE, DmgType, n, Name(NA_A), e.EActor,e.EVictim);
      }
      id = true;
      }
      break;

    case WQ_VORPAL:
      if (!e.ETarget->isCreature()) break; 
      if (e.isCrit && !e.EVictim->HasMFlag(M_NOHEAD))
      {
        TPrint(e, "You behead the <EVictim>!",
            "The <EActor> beheads you!",
            "The <EActor> beheads the <EVictim>!");
        e.Died = true;
        ThrowDmg(EV_DEATH,0,0,XPrint("a <Obj> with a <Obj>",e.EActor,this),
            e.EActor,e.EVictim);
        id = true;
        break;
      }
      break;
    case WQ_VICIOUS:
      if (!e.ETarget->isCreature()) 
        break;
      if ((e.EActor->cHP * 3) > (2 * (e.EActor->mHP + e.EActor->Attr[A_THP])))
        {
        
          VPrint(e, "The <Obj> deals a vicious wound!",
              "The <Obj> deals a vicious wound!",this);
          n = Dice::Roll(3,6); e.xDmg += n; e.strXDmg += " +3d6 vicious";
          ThrowDmg(EV_DAMAGE, AD_NORM,n,XPrint("a <Obj> with a <Obj>",
                e.EActor,this), e.EActor,e.EVictim);
          id = true;
          ThrowDmg(EV_DAMAGE, AD_NORM,Dice::Roll(1,6),XPrint("your vicious <Obj>",
                this), e.EActor,e.EActor);
        }
      break; 
    case WQ_IMPACT: 
      if (!e.ETarget->isCreature()) break; 
      if (e.EVictim->GetAttr(A_SIZ) <= e.EActor->GetAttr(A_SIZ)) {
        if (!(e.EVictim->SavingThrow(FORT,20,SA_KNOCKDOWN))) {
          e.EDir = e.EActor->DirTo(e.EVictim);
          if (ThrowDir(EV_PUSH,(Dir)e.EDir,e.EVictim,e.EActor) == ABORT) {
            ThrowDmg(EV_DAMAGE,AD_TRIP,20,"knockback attack",
                e.EActor,e.EVictim,e.EItem,e.EItem2);
          } else {
            e.EVictim->IDPrint("You are knocked back!",
                "The <Obj> is forced backward!",e.EVictim);
          } 
          id = true; 
          if (!(e.EVictim->SavingThrow(FORT,20,SA_KNOCKDOWN))) 
            ThrowDmg(EV_DAMAGE,AD_STUN,1,"knockback attack",
                e.EActor,e.EVictim,e.EItem,e.EItem2);
        } 
      } 
      break; 

    case WQ_FEY_TRAPPING: 
      if (!e.ETarget->isCreature()) break; 
      {
        if (e.vDmg > 0) { 
          e.EVictim->GainTempStati(ILLUMINATED,e.EActor,e.vDmg,SS_ATTK);
          e.EVictim->GainTempStati(ANCHORED,e.EActor,e.vDmg,SS_ATTK);
          VPrint(e,"You are limned by a strangely solid light!",
              "The <EVictim> is limned by a strangely solid light!");
          id = true; 
        } 
      } 
      break; 

    case WQ_BANE:
      if (!e.ETarget->isCreature()) break; 
      if (e.EVictim->isMType(Bane))
      {
        VPrint(e, "The <Obj> bites deep!",
            "The <Obj> bites deep!",this);
        n = Dice::Roll(2,6); e.xDmg += n; e.strXDmg += " +2d6 bane";
        ThrowDmg(EV_DAMAGE, AD_NORM,n,XPrint("a <Obj> with a <Obj>",
              e.EActor,this), e.EActor,e.EVictim);
        id = true;
        break;
      }
      break;
    case WQ_SLAYING:
      if (!e.ETarget->isCreature()) break; 
      if (e.EVictim->isMType(Bane))
      {
        id = true;
        if (!e.isCrit)
          // ww: was divide by 2, but then even artifact weapons (level 10)
          // were only a fort save vs. 15 ... 
          if (e.EVictim->SavingThrow(FORT,10+ItemLevel(),SA_DEATH))
          {
            e.Resist = true;
            break;
          }
        if (e.EVictim->ResistLevel(AD_NECR) == -1)
        {
          e.Immune = true;
          break;
        }
        VPrint(e, "You are slain!", "The <EVictim> is slain!");
        ThrowDmg(EV_DEATH,0,0,XPrint("a <Obj> with a <Obj>",e.EActor,this),
            e.EActor,e.EVictim);
        break;
      }
    case WQ_TERROR:            
      if (!e.ETarget->isCreature()) break; 
      if (/* e.isCrit && */ e.EVictim->ResistLevel(AD_FEAR) != -1) {
        if (e.EVictim->SavingThrow(WILL,10+GetPlus()*2+e.EActor->Mod(A_CHA),SA_FEAR))
        {
          e.Resist = true;
          break;
        }
        e.EVictim->GainTempStati(AFRAID,e.EVictim,Dice::Roll(3,6),SS_ENCH,FEAR_MAGIC);
        VPrint(e,"You are filled with mortal terror!",
            "The <EVictim> is filled with mortal terror!");
        id = true;
      }
      break; 
    case WQ_DISRUPTION:
      if (!e.ETarget->isCreature()) break; 
      if (e.EVictim->isMType(MA_UNDEAD))
        if (!e.EVictim->SavingThrow(WILL,10+GetPlus()*2+e.EActor->Mod(A_CHA),0))
        {
          VPrint(e,"You are disrupted by holy power!",
              "The <EVictim> is disrupted by holy power!");
          ThrowDmg(EV_DEATH,0,0,XPrint("a <Obj> with a <Obj>",e.EActor,this),
              e.EActor,e.EVictim);
          id = true;
        }  
      break;
    case WQ_STUNNING:            
      if (!e.ETarget->isCreature()) break; 
      if (/* e.isCrit && */ e.EVictim->ResistLevel(AD_STUN) != -1) {
        if (e.EVictim->SavingThrow(FORT,10+GetPlus()*2+e.EActor->Mod(A_STR),SA_PARA))
        {
          e.Resist = true;
          break;
        }
        e.EVictim->GainTempStati(STUNNED,NULL,Dice::Roll(1,4)+1,SS_ENCH);
        VPrint(e,"You are stunned!",
            "The <EVictim> reels...");
        id = true;
      }
      break; 
    case WQ_WEAKENING:
    case WQ_NUMBING:
    case WQ_WITHERING:
      if (!e.ETarget->isCreature()) break; 
      e.saveDC = 10+ItemLevel();
      DmgType = e.DType; 
      e.Dmg.Number = 1;
      e.Dmg.Sides = 6;
      e.Dmg.Bonus = 0; 
      e.DType = AD_DAST + ((int8)q - WQ_WEAKENING);
      ReThrow(EV_DAMAGE,e);
      e.DType = DmgType;
      id = true;
      break;
    default:
      break;
  }
  if (id)
    VisibleID(e.EActor);
} 

void Weapon::DoQualityDmgSingle(EventInfo &e, int32 skip)
{
  int i;
  for (i=0;i!=8;i++)
    if (Qualities[i] && !(e.ETarget->isDead()) && Qualities[i] != skip)
      QualityDmgSingle(e,Qualities[i]);

  StatiIterNature(this,EXTRA_QUALITY)
      if (!e.ETarget->isDead() && S->Val != skip)
      QualityDmgSingle(e,S->Val);
  StatiIterEnd(this)
} 

EvReturn Weapon::QualityDmg(EventInfo &e) {
  int16 n; 

  /* ww: some weapon qualities must work even if the victim is now dead */
  if (HasQuality(WQ_VAMPIRIC))
    QualityDmgSingle(e,WQ_VAMPIRIC);

  if (e.EVictim->isDead())
    return DONE;

  if (HasIFlag(WT_STUNNING) && !e.EVictim->HasStati(STUNNED))
    if (e.isCrit || !random(3) || e.isGreatBlow)
      if (e.EVictim->isMType(MA_LIVING) && e.EVictim->ResistLevel(AD_STUN) != -1)
        if (!e.EVictim->SavingThrow(FORT, e.EActor->WeaponSaveDC(this,WT_STUNNING),SA_PARA))
        {
          VPrint(e,"You are stunned by the blow!",
              "The <EVictim> reels!");
          e.EVictim->GainTempStati(STUNNED,NULL,Dice::Roll(1,4),SS_ATTK);
        }
  if (HasIFlag(WT_ENTANGLE) && !e.EVictim->HasStati(GRABBED,-1,e.EActor))
    if (e.EVictim->GetAttr(A_SIZ) <= TITEM(iID)->Size + 1)
      if (e.isCrit || !random(3) || e.AType == A_HURL)
        if (!e.EVictim->SavingThrow(REF,e.EActor->WeaponSaveDC(this,WT_ENTANGLE),SA_PARA|SA_GRAB))
        {
          SetSilence();
          DAMAGE(e.EActor,e.EVictim,AD_STUK,-1,
            "entangling weapon",xe.EParam = STUCK_WEAPON);
          UnsetSilence();
          if (e.EVictim->HasStati(STUCK)) {
            TPrint(e,"Your <Obj> entangles the <EVictim>.",
                "The <EActor>'s <Obj> entangles you.",
                "The <EActor>'s <Obj> entangles the <EVictim>.",this);
          } else {
            TPrint(e,"Your <Obj> fails to entangle the <EVictim>.",
                "The <EActor>'s <Obj> fails to entangle you.",
                "The <EActor>'s <Obj> fails to entangle the <EVictim>.",this);
          } 
        }

  if (Material() == MAT_WROUGHT)
    if (e.EVictim->isMType(MA_ELF) || e.EVictim->isMType(MA_DEMON) ||
            e.EVictim->isMType(MA_FAERIE))
    {
      /* Horrid kludge, see Iron Sword in pspells.irh */
      if (e.EActor->HasEffStati(TRAP_EVENT,FIND("Iron Sword")))
        { e.strXDmg += Format(" +5d6 iron"); 
          n = Dice::Roll(5,6); e.xDmg += n; }
      else
        { e.strXDmg += Format(" +2d6 iron"); 
          n = Dice::Roll(2,6); e.xDmg += n; }
      ThrowDmg(EV_DAMAGE, AD_NORM, n, Name(NA_A), e.EActor,e.EVictim);
      VPrint(e,"Wrought iron sears your flesh!", 
               "Wrought iron sears the <EVictim>'s flesh!"); 
    } 

  DoQualityDmgSingle(e,WQ_VAMPIRIC);

  return DONE;
}

int16 Creature::WeaponSaveDC(Item *wp, int16 at)
  {
    /* before we passed in the attribute, which conflated things like
    * closing and entangling -- now we pass in the action */
    if (!wp) 
      switch (at) { 
        case WT_ENTANGLE: 
        case WT_REACH: 
          return 10 + (GetBAB(S_BRAWL) / 2) + Mod2(A_DEX);
        case WT_STUNNING: 
        case WT_KNOCKDOWN: 
          return 10 + (GetBAB(S_BRAWL) / 2) + Mod2(A_STR);
        default: 
        Error("Unknown type of Weapon Save DC");
      }
    /* ww: let's modify Julian's basic formula a bit: it was:
      return 8 + (ChallengeRating() / 2) + wp->GetPlus() +
      + Mod(at) + TITEM(wp->iID)->u.w.ParryMod;
    */
    int16 DC; DC = 10;
    int8 sk = WepSkill(wp);
    if (sk == WS_NOT_PROF) 
      DC -= 4; /* if you're not trained, don't expect miracles */
    else
      DC += (sk - WS_PROFICIENT)*2;
    DC += GetBAB(S_BRAWL) / 2;
    DC += wp->GetPlus();
    TItem *ti = TITEM(wp->iID);

    switch (at) {
      case WT_ENTANGLE: 
        DC += ti->u.w.Acc;
        DC += Mod2(A_DEX);
      break;
      case WT_REACH: 
        if (ti->HasFlag(WT_REACH))
          DC += ti->u.w.ParryMod;
        DC += Mod2(A_DEX); 
      break;
      case WT_STUNNING: 
      case WT_KNOCKDOWN:
        DC += Mod2(A_STR) + (int16)(min(wp->Weight() / (40 * wp->Quantity), 5) + ((ti->Size - SZ_MEDIUM) * 2));
        break; 
      default:  
        Error("Unknown type of Weapon Save DC");
      }
    return DC;
  }

int8 Creature::AttackMode() {
    /* If you have a touch spell active, and you can possibly have a free hand, that's assumed to be your prefered method of attack. */
    if (HasStati(TOUCH_ATTACK))
        if (!(EInSlot(SL_WEAPON) && EInSlot(SL_READY)) || (EInSlot(SL_WEAPON) == EInSlot(SL_READY)))
            return S_BRAWL;
    /* Later, Optimize to avoid unnecessary InSlot calls */
    if (EInSlot(SL_WEAPON) == NULL)
        return S_BRAWL;
    if (EInSlot(SL_WEAPON)->isType(T_BOW))
        return S_ARCHERY;
    if (EInSlot(SL_WEAPON)->isType(T_MISSILE) || EInSlot(SL_WEAPON)->thrownOnly())
        return S_THROWN;
    if (EInSlot(SL_READY) && EInSlot(SL_READY)->isType(T_WEAPON))
        if ((EInSlot(SL_READY) != EInSlot(SL_WEAPON)) || TITEM(InSlot(SL_WEAPON)->iID)->HasFlag(WT_DOUBLE))
            return S_DUAL;
    return S_MELEE;
}

Dice& Creature::DmgVal(int16 type, bool is_large)
{
  Item *it;
  static Dice d, d2;
  switch(type)
  {
    case S_BRAWL:
      d = MonkDamage[AbilityLevel(CA_UNARMED_STRIKE)];
      d.Bonus += Attr[A_DMG_BRAWL];
      break;
    case S_ARCHERY:
      it=EInSlot(SL_WEAPON);
      if(it)
        if (TITEM(it->iID)->IType == T_BOW)
        {
          d = it->SDmg();
          d.Bonus += Attr[A_DMG_ARCHERY];
          for (int8 i=0;i!=SL_LAST;i++)
            if (it = EInSlot(i))
              if (it->isType(T_MISSILE))
              {
                d2 = is_large ? it->LDmg() : it->SDmg();
                d.Sides += d2.Sides;
                d.Bonus += d2.Bonus;
                if (WepSkill(EInSlot(SL_WEAPON)) >= WS_MASTERY)
                  d.Sides += 2;
                return d;
              }
          return d;
        }
      d.Set(0,0,0);
      return d;
    case S_DUAL:
      it=EInSlot(SL_READY);
      if(!it) {
        Error("Two-Wep mode with no second weapon wielded!");
        d.Number = d.Sides = d.Bonus = 0;
        return d;
      }
      d = is_large ? it->LDmg() : it->SDmg();
      d.Bonus += Attr[A_DMG_OFFHAND];
      if (WepSkill(it) >= WS_MASTERY)
        d.Sides += 2;
      break;
    case S_MELEE:
      it=EInSlot(SL_WEAPON);
      if(!it) {
        Error("Melee mode with no weapon wielded!");
        d.Number = d.Sides = d.Bonus = 0;
        return d;
      }
      if(it->Type!=T_WEAPON || it->thrownOnly())
      {
        //Something other than a weapon in your hand;
//NotMeleeWeapon:
        d.Number=1;
        d.Sides=3;
        d.Bonus = (int8)Attr[A_DMG_MELEE];
        return d;
      }
      d = is_large ? it->LDmg() : it->SDmg();
      d.Bonus += Attr[A_DMG_MELEE];
      if (WepSkill(it) >= WS_MASTERY)
        d.Sides += 2;
      break;
    case S_THROWN:
      for (int8 i=0;i!=SL_LAST;i++)
        if (it = EInSlot(i))
          if (it->RangeInc(this))
          {
            d = is_large ? it->LDmg() : it->SDmg();
            d.Bonus += Attr[A_DMG_THROWN];
            if (WepSkill(it) >= WS_MASTERY)
              d.Sides += 2;
            return d;
          }
      d.Set(1,3,(int8)Attr[A_DMG_THROWN]);
      return d;
    default:
      Error("Unimplemented combat type!");
      d.Number = d.Sides = d.Bonus = 0;
      d.Bonus += Attr[A_DMG_THROWN];
      return d;
      break;
    }
  return d;
  }

EvReturn Creature::AttackMsg(EventInfo &e) {
    const char *loc, *verb_s, *verb_p, *result; 
    String prelude, s1, s2, s3, chargeStr;
    int16 saveSil;
    bool note_death; 
    const char *cMult[] = { "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7" }, *rMult[] = { "x0", "x1", "x1.5", "x2", "x2.5", "x3", "x3.5", "x4" };

    s1 = "";
    s2 = "";
    chargeStr = "";

    /* Watch out for Traps in EItem! */ 
    if (e.EItem && !e.EItem->isItem())
        e.EItem = NULL;

    {
        Player *p; int16 dis; Dir d;
        p = theGame->GetPlayer(0);
        if (!(p->Perceives(e.EActor) || p->Perceives(e.EVictim)) && p->m == m && p->x > 0) {
            static TextVal DirNames[] = {
                { NORTH, "north" },
                { SOUTH, "south" },
                { EAST, "east" },
                { WEST, "west" },
                { NORTHEAST, "north-east" },
                { NORTHWEST, "north-west" },
                { SOUTHEAST, "south-east" },
                { SOUTHWEST, "south-west" },
                { 0, NULL }
            };    

            d = p->DirTo(e.EActor);
            if ((dis = p->DistFrom(e.EActor)) < 10 + p->SkillLevel(SK_LISTEN))
                if (!p->HasStati(HEARD_IT,d)) {
                    if (p->SkillCheck(SK_LISTEN,10))
                        p->IPrint("You hear the sounds of conflict to the <Str>.",
                        (const char*) Lower(LookupOnly(DirNames,d)));
                    p->GainTempStati(HEARD_IT,NULL,SS_MISC,4,d);
                }
        }
    }

    /* Kludge to avoid giveaway messages for invisible/ethereal creatures
    in the path of missile weapons. */
    if (e.EActor && e.EVictim)
        if (!e.EActor->isBeside(e.EVictim) || e.AType == A_FIRE || e.AType == A_HURL)
            if (!e.EVictim->isPlayer())
                if (!e.EActor->Perceives(e.EVictim))
                    if (e.EVictim->onPlane() != e.EActor->onPlane())
                    {
                        if (e.EActor && e.EActor->m)
                            e.EActor->m->EmptyQueue(QUEUE_DAMAGE_MSG);
                        return DONE;
                    }

    saveSil = Silence;
    Silence = false;

    if (e.strDmg == NULL)
        e.strDmg = "";

    Term *TActor = NULL;
    Term *TVictim = NULL;

    if (e.EActor) 
        TActor = e.EActor->ShowCombatNumbers();
    else
        Fatal("no Actor in AttackMsg?");

    if (e.EVictim) 
        TVictim = e.EVictim->ShowCombatNumbers();

    if (TActor || TVictim) {
        s3 = Format("%c%s ",-GREY, (const char *)Capitalize(e.EActor->Name(NA_POSS),true));
        if (TActor) { 
            TActor->SetWin(WIN_NUMBERS3);
            TActor->Clear();
        }

        if (TVictim) {
            TVictim->SetWin(WIN_NUMBERS4);
            TVictim->Clear();
        }

        if ((e.AType == A_BULL || e.AType == A_DISA || e.AType == A_TRIP || e.AType == A_GRAB || e.AType == A_THRO || e.AType == A_ESCA) && e.isHit) {
            String s0; 

            /* XPrint Needed here for colour extension below */
            switch (e.AType) {
            case A_BULL: s0 = XPrint("<13>Bull Rush:<7> "); break;
            case A_THRO: s0 = XPrint("<14>Grapple:<7> "); break;
            case A_GRAB: s0 = XPrint("<14>Grapple:<7> "); break;
            case A_ESCA: s0 = XPrint("<14>Grapple:<7> "); break;
            case A_DISA: s0 = XPrint("<10>Disarm:<7> "); break;
            case A_TRIP: s0 = XPrint("<10>Trip:<7> "); break;
            }        

            s1 = Format("%s = %d vs. %s = %d %c[%s]%c",
                (const char*)e.strOpp1, e.vOpp1, (const char*)e.strOpp2, e.vOpp2, -(e.Resist ? RED : EMERALD), e.Resist ? "failure" : "success", -GREY);

            if (TActor) {
                if (e.EActor->isPlayer())  {
                    TActor->Write(0,0,XPrint(s0.TrueLeft(64) + s1));
                    if (theGame->Opt(OPT_STORE_ROLLS))
                        TActor->AddMessage(s0 + s1);
                } else {
                    s3.SetAt(0, s0[0]);
                    TActor->Write(0,0,XPrint((s3 + s0).TrueLeft(64) + s1));
                    if (theGame->Opt(OPT_STORE_ROLLS))
                        TActor->AddMessage(s3+s0+s1);
                } 
            } 
            if (TVictim) {
                s3.SetAt(0, s0[0]);
                TVictim->Write(0,0,XPrint((s3 + s0).TrueLeft(64) + s1));
                if (theGame->Opt(OPT_STORE_ROLLS))
                    TVictim->AddMessage(s3 + s0+s1);
            }
        } else {
            if (e.strDef.GetLength() > 3)
                e.strDef += Format(" = %d",e.vDef);

            s1 = Format("%c%s:%c 1d20 (%d) %s = %d vs. ",
                -14,e.AType == A_SPEL ? NAME(e.eID) : "Attack", -7,e.vRoll, (const char*)e.strHit, e.vHit + e.vRoll);
            if (e.vRideCheck > e.vDef)
                s1 += Format("Ride %d [", e.vRideCheck);
            else
                s1 += Format("Def %s [", (const char*)e.strDef); 

            s1 += e.isCrit ? "crit" : (e.vRoll >= e.vThreat ? "threat" : (e.isHit ? "hit" : "miss"));
            s1 += "]";
            if (e.AType == A_SPEL)
                goto SkipPrintingDamageButStillPrintHitRoll;
            if (e.AType == A_TUCH && !e.EActor->HasAttk(A_TUCH))
                if (GetStatiMag(CHARGING)) {
                    int32 mult = 1 + ((HasFeat(FT_SPIRITED_CHARGE) && HasStati(MOUNTED)) || (HasFeat(FT_FLYING_KICK) && e.AType == A_KICK));
                    if (e.EItem && e.EItem->HasIFlag(WT_CHARGE)) mult++;
                    if (mult > 1) 
                        chargeStr += Format("x%d ",mult);
                } 
            {
                Creature * rider = (Creature *)GetStatiObj(MOUNT);
                if (rider && rider->GetStatiMag(CHARGING)) {
                    int32 mult = 1; 
                    if (e.EItem && e.EItem->HasIFlag(WT_CHARGE))
                        mult++;
                    if (mult > 1)
                        chargeStr += Format("x%d ",mult);
                } 
            }
            if (e.vicCharging) {
                if (e.isAoO && e.isMoveAoO && e.vicCharging) { 
                    int32 mult = 1; 
                    if ((e.EItem != NULL) && e.EItem->HasIFlag(WT_CHARGE))
                        mult++;
                    if (mult > 1) 
                        chargeStr += Format("x%d ",mult); 
                } 
            } 

            if (e.isCrit && e.strDmg.GetLength())
                s2 = Format("%cDamage:%c %s + (%s) %s %s= %d",-13,-7,
                    (const char*)e.Dmg.Str(), (const char*)Trim(e.strDmg), e.EVictim->HasFeat(FT_ROLL_WITH_IT) ? rMult[e.vCrit] : cMult[e.vCrit],
                    (const char*)chargeStr, (int)e.vDmg);
            else if (e.isHit) {
                s2 = Format("%cDamage:%c %s%s %s= %d",-13,-7,
                    (const char*)e.Dmg.Str(),(const char*)e.strDmg, (const char*)chargeStr, (int)e.vDmg);
            }
            if (e.isBypass) {
                if (e.vPen)
                    s2 += Format(" bypassed Cov %d (%d other %s, %d%% Pen) = %d",
                        e.EVictim->Attr[A_COV], e.vArm, is_wepdmg(e.DType) ? "Arm" : "Res", e.vPen, e.aDmg);
                else
                    s2 += Format(" bypassed Cov %d = %d",e.EVictim->Attr[A_COV], e.aDmg);
            } else if (e.isHit && e.vArm) {
                s2 += Format(" vs. %s %d",is_wepdmg(e.DType) ? 
                    "Arm" : "Res", e.vArm);
                if (e.vPen)
                    s2 += Format(" (%d%% Pen)",e.vPen);
                s2 += Format(" = %d",e.aDmg);
            }
            if (e.strXDmg.GetLength())
                s2 += Format("%s = %d",(const char*)e.strXDmg,e.aDmg+e.xDmg);
            if (!is_wepdmg(e.DType))
                s2 += Format(" %s",Lookup(DTypeNames,e.DType));

SkipPrintingDamageButStillPrintHitRoll:
            if (e.AType == A_GAZE || e.AType == A_PROX || e.AType == A_CPRX || e.AType == A_ROAR || e.AType == A_DEQU || e.AType == A_AURA ||
                e.EActor->HasStati(ENGULFED) || is_postgrab_attk(e.AType))
                s1 = ""; 

            if (TActor) {
                if (e.EActor->isPlayer()) { 
                    if (s1.GetLength()) {
                        TActor->Write(0,0, XPrint(s1.TrueLeft(64) + "\n" + s2));
                        if (theGame->Opt(OPT_STORE_ROLLS))
                            TActor->AddMessage(s1);
                    } else {
                        TActor->Write(0,0, XPrint(s2));
                    } 
                    if (theGame->Opt(OPT_STORE_ROLLS))
                        TActor->AddMessage(s2);
                } else {
                    s3.SetAt(0,s1[0]);
                    if (s1.GetLength()) { 
                        TActor->Write(0,0, XPrint((s3 + s1).TrueLeft(64)));
                        if (theGame->Opt(OPT_STORE_ROLLS))
                            TActor->AddMessage(s3 + s1);
                    }
                    if (s2.GetLength()) {
                        s3.SetAt(0, s2[0]);
                        TActor->Write(0,1, XPrint(s3 + s2));
                        if (theGame->Opt(OPT_STORE_ROLLS))
                            TActor->AddMessage(s3 + s2);
                    }
                } 
            } if (TVictim) { 
                s3.SetAt(0,s1[0]);
                if (s1.GetLength()) { 
                    TVictim->Write(0,0, XPrint((s3 + s1).TrueLeft(64)));
                    if (theGame->Opt(OPT_STORE_ROLLS))
                        TVictim->AddMessage(s3 + s1);
                }
                if (s2.GetLength()) {
                    s3.SetAt(0,s2[0]);
                    TVictim->Write(0,1, XPrint(s3 + s2));
                    if (theGame->Opt(OPT_STORE_ROLLS))
                        TVictim->AddMessage(s3 + s2); 
                }
            }
        }
    } else {
        // unless a player can see the actor or the victim, there's no reason to do anything else here ...
        // a recent (Sun Nov  2 16:28:02 PST 2003) profile run suggests that this is the fourth-most time consuming function in Incursion
        if (e.EMap)
            for(int16 i=0;i!=MAX_PLAYERS;i++)
                if (Player *p = oPlayer(e.EMap->pl[i]))
                    if (p != e.EActor && p != e.EVictim)
                        if ((e.EActor && p->XPerceives(e.EActor)) && (!e.EVictim || p->XPerceives(e.EVictim)))
                            goto SkipDamage;
        return DONE;
    } 

SkipDamage:
    if (e.AType == A_SPEL) {
        if (e.strBlastDmg.GetLength() > 0) {
            if (TActor) {
                TActor->SetWin(WIN_NUMBERS3);
                TActor->Write(0,1,e.strBlastDmg);
                if (theGame->Opt(OPT_STORE_ROLLS))
                    TActor->AddMessage(e.strBlastDmg);
            }
            if (TVictim) {
                TVictim->SetWin(WIN_NUMBERS4);
                TVictim->Write(0,1,e.strBlastDmg);
                if (theGame->Opt(OPT_STORE_ROLLS))
                    TVictim->AddMessage(e.strBlastDmg);
            }              
        }
        return DONE;
    }

    if (e.Terse)
        goto SkipMainMessage;

    loc = "";
    verb_s = verb_p = Lookup(AttkVerbs2,e.AType);
    result = e.Died ? "kill" : (e.isCrit ? (random(2) ? "blast" : "brutaliz") : (e.isHit ? "hitt" : "miss"));

    if (e.isAoO)
        prelude = "Seeing an opportunity, ";
    else if (e.isCleave)
        prelude = "Cleaving through, ";
    else if (e.isSurprise) {
        /* Kludge, but it works (for single player). */
        if (e.EVictim->isPlayer())
            prelude = "Catching you unaware, ";
        else
            prelude = XPrint("Catching <him:Obj> unaware, ",e.EVictim);
    } else if (e.isGreatBlow) {
        switch(random(4)) {
        case 0: prelude = "Giving a mighty battle-howl, ";
        case 1: prelude = "Drawing back to land a terrible blow, ";
        case 2: prelude = "Preparing to smite your enemy, ";
        case 3: prelude = "With great and terrible force, ";
        }
    } else if (e.isPrecision) {
        prelude = "With considered precision, ";
    } else if (e.isFlatFoot) {
        /* Kludge, but it works (for single player). */
        if (e.EVictim->isPlayer())
            prelude = "Catching you flat-footed, ";
        else
            prelude = XPrint("Catching <him:Obj> flat-footed, ",e.EVictim);
    } else if (e.isBypass) {
        /* Kludge, but it works (for single player). */
        if (e.EVictim->isPlayer())
            prelude = "Bypassing your armour, ";
        else
            prelude = XPrint("Bypassing <his:Obj> armour, ",e.EVictim);
    } else if (e.isOffhand) {
        if (e.EActor->isPlayer())
            prelude = "With your off hand, ";
        else
            prelude = XPrint("With <his:Obj> off hand, ",e.EActor);
    } else
        prelude = "";

    switch(e.AType) {
    case A_BREA:
    case A_BRE2:
    case A_SPIT:
        TPrint(e,
            "<Str4>you <Str1> <Str3>.",
            "<Str4>a <EActor> <Str2> <Str3>.",
            "<Str4>a <EActor> <Str2> <Str3>.",
            Lookup(AttkVerbs1,e.AType), Lookup(AttkVerbs2,e.AType), 
            Lookup(BreathTypes,e.DType), (const char*)prelude);
        break;
    case A_ROAR:
        String *verb;
        /* Player-Ally Monsters cause trouble with A_ROAR, c.f. chasme */
        if (e.EActor->getLeader())
            if (e.EActor->getLeader()->isPlayer())
                return ABORT;

        if ((verb = TMON(e.EActor->tmID)->GetMessages(MSG_ROARVERB)) && verb[0].GetLength()) {
            TPrint(e,
                "<Str3>you <Str1>.",
                "<Str3>the <EActor> <Str2>.",
                "<Str3>the <EActor> <Str2>.",
                (const char*)(verb[0]), (const char*)(verb[1]), 
                (const char*)prelude);
            break;
        }
        if ((verb = TMON(e.EActor->mID)->GetMessages(MSG_ROARVERB)) && verb[0].GetLength()) {
            TPrint(e,
                "<Str3>you <Str1>.",
                "<Str3>the <EActor> <Str2>.",
                "<Str3>the <EActor> <Str2>.",
                (const char*)(verb[0]), (const char*)(verb[1]), 
                (const char*)prelude);
            break;
        }
        // fall through          
    case A_EXPL:
        TPrint(e,
            "<Str3>you <Str1>.",
            "<Str3>the <EActor> <Str2>.",
            "<Str3>the <EActor> <Str2>.",
            Lookup(AttkVerbs1,e.AType), Lookup(AttkVerbs2,e.AType), (const char*)prelude);
        break;
    case A_PROB:
        if (e.isHit) {
            TPrint(e,
                "<Str1>your probiscis pierces the <EVictim>.",
                "<Str1>the <EActor>'s probiscis pierces you.",
                "<Str1>the <EActor>'s probiscis pierces a <EVictim>.",
                (const char*)prelude);
            note_death = true;
        }
        else
            TPrint(e,
            "<Str1>your probiscis misses the <EVictim>.",
            "<Str1>the <EActor>'s probiscis misses you.",
            "<Str1>the <EActor>'s probiscis misses a <EVictim>.",
            (const char*)prelude);
        break;
    case A_FIRE:
        TPrint(e,
            "<Str4>you <Str1> an <EItem2> at an <EVictim>, <Str3>ing <him:EVictim><Str5>.",
            "<Str4>a <EActor> <Str2> an <EItem2> at you, <Str3>ing you<Str5>.", 
            "<Str4>a <EActor> <Str2> an <EItem2> at <EVictim>, <Str3>ing <him:EVictim><Str5>.",
            Lookup(AttkVerbs1,e.AType), Lookup(AttkVerbs2,e.AType),
            result, (const char*)prelude,
            e.isWildMiss ? " wildly" : "");
        break;
    case A_ESCA:
        if (e.Resist)
            TPrint(e,"You try to break the <EVictim>'s hold on you, but cannot.",
            "The <EActor> tries to break your hold on <him:EActor>, but cannot.",
            "The <EActor> tries to break the <EVictim>'s hold on <him:EActor>, but cannot.");
        else
            TPrint(e,"You tear free of the <EVictim>'s grip!",
            "The <EActor> tears free of your grip!",
            "The <EActor> tears free of the <EVictim>'s grip.");
        break;                                    
    case A_SUCK:
        TPrint(e,"You suck the <EVictim>'s blood.",
            "The <EActor> sucks your blood.",
            "The <EActor> sucks a <EVictim>'s blood.");
        break;
    case A_SPOR:
    case A_OOZE:
    case A_BITE:
    case A_TUCH:
    case A_HUGS:
    case A_RUBS:
    case A_SLOB:
    case A_SLAM:
    case A_IMPA:
    case A_GAZE:
        if (e.Died && e.isHit)
            TPrint(e,"<Str1>you <Str2> an <EVictim>, killing <him:EVictim>.",
            "<Str1>a <EActor> <Str3> you.",
            "<Str1>a <EActor> <Str3> an <EVictim>, killing <him:EVictim>.",
            (const char*)prelude,
            Lookup(AttkVerbs1,e.AType),
            Lookup(e.isHit ? AttkVerbs2 : AttkVerbs1,e.AType));
        else
            TPrint(e,"<Str8>you<Str1> <Str6> an <EVictim><Str3><Str5>.",
            "<Str8>a <EActor><Str2> <Str7> you<Str4><Str5>.",
            "<Str8>a <EActor><Str2> <Str7> an <EVictim><Str4><Str5>.",
            e.isHit ? "" : " try to",
            e.isHit ? "" : " tries to",
            e.isHit ? "" : ", but miss",
            e.isHit ? "" : ", but misses",
            e.isWildMiss ? " wildly" : "",
            Lookup(AttkVerbs1,e.AType),
            Lookup(e.isHit ? AttkVerbs2 : AttkVerbs1,e.AType),
            (const char*)prelude);
        break;
    case A_TENT:
        TPrint(e,"<Str8>you<Str1> <Str6> an <EVictim> with your tentacles<Str3><Str5>.",
            "<Str8>a <EActor><Str2> <Str7> you with <his:EActor> tentacles<Str4><Str5>.",
            "<Str8>a <EActor><Str2> <Str7> an <EVictim> with <his:EActor> tentacles<Str4><Str5>.",
            e.isHit ? "" : " try to",
            e.isHit ? "" : " tries to",
            e.isHit ? "" : ", but miss",
            e.isHit ? "" : ", but misses",
            e.isWildMiss ? " wildly" : "",
            "snare",
            e.isHit ? "snares" : "snare",
            (const char*)prelude);
        break;
    case A_SUND:
        TPrint(e,"<Str8>you<Str1> <Str6> the <EVictim>'s <EItem2><Str3><Str5>.",
            "<Str8>a <EActor><Str2> <Str7> your <EItem2><Str4><Str5>.",
            "<Str8>a <EActor><Str2> <Str7> an <EVictim>'s <EItem2><Str4><Str5>.",
            e.isHit ? "" : " try to",
            e.isHit ? "" : " tries to",
            e.isHit ? "" : ", but miss",
            e.isHit ? "" : ", but misses",
            e.isWildMiss ? " wildly" : "",
            "strike",
            e.isHit ? "strikes" : "strike",
            (const char*)prelude);
        break;
    case A_DEQU: 
    case A_AURA:
    case A_ALSO:
    case A_PROX:
    case A_CPRX:
        switch(e.DType) {
        case AD_FIRE:
            VPrint(e,"Fire sears you!","Fire sears the <EVictim>!"); 
            break;
        case AD_COLD:
            VPrint(e,"Frost chills you!","Frost chills the <EVictim>!"); 
            break;
        case AD_HOLY:
            if (e.aDmg)
                VPrint(e,"Holy purity sears you!","Holy purity sears the <EVictim>!"); 
            break;
        case AD_ACID:
            VPrint(e,"Acid splashes you!","Acid splashes the <EVictim>!"); 
            break;
        case AD_ELEC:
            VPrint(e,"Lightning zaps you!","Lightning zaps the <EVictim>!"); 
            break;
        case AD_EVIL:
            if (e.aDmg)
                VPrint(e,"Dark power burns you!","Dark power burns the <EVictim>!"); 
        case AD_SONI: 
            if (e.aDmg)
                VPrint(e,"Sonic vibrations strike you!","Sonic vibrations strike the <EVictim>!"); 
            break;
            /* Other auras, such as AD_SOAK and AD_DREX, have special messages
            printed in the damage section. */
        }
        break;
    case A_WRAP:
    case A_CRUS:
    case A_REND:
    case A_CONS:
    case A_BULL:
    case A_TRIP:
    case A_DISA:
    case A_THRO:          
        TPrint(e,"<Str7>you<Str1> <Str5> an <EVictim><Str3>.",
            "<Str7>a <EActor><Str2> <Str6> you<Str4>.",
            "<Str7>a <EActor><Str2> <Str6> an <EVictim><Str4>.",
            (e.isHit && !e.Resist) ? "" : " try to",
            (e.isHit && !e.Resist) ? "" : " tries to",
            (e.isHit && !e.Resist) ? "" : ", but fail",
            (e.isHit && !e.Resist) ? "" : ", but fails",
            Lookup(AttkVerbs1,e.AType),
            Lookup((e.isHit && !e.Resist) ? AttkVerbs2 : AttkVerbs1,e.AType),
            (const char*)prelude);
        break;
    case A_HURL:
        TPrint(e,"<Str2>you throw <str3>, <Str1>ing the <EVictim>.",
            "<Str2>a <EActor> throws <str3>, <Str1>ing you.",
            "<Str2>a <EActor> throws <str3>, <Str1>ing a <EVictim>.",
            result, (const char*)prelude,
            (const char*)e.EItem2->Name(NA_A));
        break;
    case A_SWNG:
    case A_THRU:
        if (e.EVictim->HasMFlag(M_HUMANOID)) {
            loc = !random(3) ? "high" : !random(2) ? "center" : "low";
            TPrint(e,"<Str5>you <Str1> <Str4>, <Str3>ing the <EVictim><Str6>.",
                "<Str5>a <EActor> <Str2> <Str4>, <Str3>ing you<Str6>.",
                "<Str5>a <EActor> <Str2> <Str4>, <Str3>ing a <EVictim><Str6>.",
                Lookup(AttkVerbs1,e.AType), Lookup(AttkVerbs2,e.AType), 
                result, loc, (const char*)prelude, e.isWildMiss ? " wildly" : "");
            break;
        }    
        /* NO BREAK HERE */
    default:
        TPrint(e,"<Str4>you <Str1>, <Str3>ing the <EVictim>.",
            "<Str4>a <EActor> <Str2>, <Str3>ing you.",
            "<Str4>a <EActor> <Str2>, <Str3>ing a <EVictim>.",
            Lookup(AttkVerbs1,e.AType), Lookup(AttkVerbs2,e.AType), 
            result, (const char*)prelude);
        break;
    }

SkipMainMessage:
    e.EMap->PrintQueue(QUEUE_DAMAGE_MSG);

    if (e.EVictim && !e.Terse && !e.Absorb && !e.eID) {
        if (e.isWImmune)
            DPrint(e,
                "Your weapon fails to penetrate.",
                "The <EActor>'s weapon fails to penetrate.");
        if (e.actIllusion && !e.EActor->isShade() && !e.EActor->isRealTo(e.EVictim)) {
            VPrint(e,
                "You ignore the illusion's attack.",
                "The <EVictim> ignores the attack completely.");
            e.EActor->DisbeliefCheck();
        } else if (e.Immune && e.EVictim->isCreature())
            VPrint(e,
                "You are unaffected.",
                "The <EVictim> seems unaffected.");
        else if (e.Resist && e.DType != AD_DISA && e.DType != AD_TRIP && e.AType != A_ESCA)
            VPrint(e,
                "You resist the effects.",
                "The <EVictim> resists the effects.");
    }

    Silence = saveSil;
    return DONE;
}


