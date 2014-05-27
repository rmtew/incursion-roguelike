/* MAGIC.CPP -- Copyright (c) 1999-2003 Julian Mensch
     This file implements the core system for handling magical effects
   in Incursion; the specific precoded effect archetypes are handled
   in Effects.Cpp. All code general to all effects goes here, as does
   code for areas of effect and specific magical actions (casting spells,
   blasting wands, reading scrolls, etc.

     void Player::ToggleMM(uint32 mm,int16 sp)
     String TEffect::Power(int8 Mod)
     EffectValues* TEffect::Vals(int8 num)

     bool Magic::isTarget(EventInfo &e, Thing *t)
     void Magic::CalcEffect(EventInfo &e)
     EvReturn Magic::MagicEvent(EventInfo &e)
     EvReturn Magic::MagicStrike(EventInfo &e)
     EvReturn Magic::MagicHit(EventInfo &e)
     EvReturn Magic::MagicXY(EventInfo &e,int16 tx,int16 ty)

     EvReturn Magic::AGlobe(EventInfo &e)
     EvReturn Magic::ABarrier(EventInfo &e)
     EvReturn Magic::AField(EventInfo &e)
     EvReturn Magic::ABeam(EventInfo &e)
     EvReturn Magic::ABall(EventInfo &e)
     EvReturn Magic::ABolt(EventInfo &e)
     EvReturn Magic::ATouch(EventInfo &e)

     EvReturn Creature::Cast(EventInfo &e)
     EvReturn Creature::Invoke(EventInfo &e)
     int16 Creature::SpellRating(rID eID, uint32 mm, bool perceived)
     int16 Character::SpellRating(rID eID, uint32 mm, bool perceived)
     EvReturn Item::DrinkPotion(EventInfo &e)
     EvReturn Item::ReadScroll(EventInfo &e)
     EvReturn Item::Activate(EventInfo &e)
     EvReturn Item::UseStaff(EventInfo &e)
     EvReturn Item::ZapWand(EventInfo &e)

*/

#include "Incursion.h"

#define is_elemental_dmg(i) (i == AD_FIRE || i == AD_COLD || i == AD_ELEC || \
                             i == AD_TOXI || i == AD_SONI || i == AD_ACID)  

bool effectGivesStati(rID eID)
  {
    EffectValues *ev; int16 i;
    TEffect *te = TEFF(eID);
    for(i=0,ev=te->Vals(i);ev;ev=te->Vals(++i))
      if (ev->eval == EA_GRANT || ev->eval == EA_INFLICT ||
          ev->eval == EA_OVERRIDE)
        return true;
    return false;
  }

void Player::ToggleMM(uint32 mm,int16 sp)
  {
    int16 ft;
    // ww: no meta-magic on innate spells currently
    if (Spells[sp] & SP_INNATE)
      return; 
    if (MMArray[sp] & mm)
      {
        MMArray[sp] &= ~mm;
        return;
      }
    switch(mm)
      {
        case MM_AMPLIFY:    ft = FT_AMPLIFY_SPELL;    break;
        case MM_AUGMENT:    ft = FT_AUGMENT_SUMMONING;break;
        case MM_ANCHOR:     ft = FT_ANCHOR_SPELL;     break;
        case MM_BIND:       ft = FT_BIND_SPELL;       break;
        case MM_CONSECRATE: ft = FT_CONSECRATE_SPELL; break;
        case MM_CONTROL:    ft = FT_CONTROL_SPELL;    break;
        case MM_DEFENSIVE:  ft = FT_DEFENSIVE_SPELL;  break;
        // case MM_DEFILING:   ft = FT_DEFILING_SPELL;   break;
        case MM_EMPOWER:    ft = FT_EMPOWER_SPELL;    break;
        case MM_ENLARGE:    ft = FT_ENLARGE_SPELL;    break;
        case MM_FOCUS:      ft = FT_FOCUS_SPELL;      break;
        case MM_FORTIFY:    ft = FT_FORTIFY_SPELL;    break;
        case MM_HEIGHTEN:   ft = FT_HEIGHTEN_SPELL;   break;
        case MM_INHERANT:   ft = FT_INHERANT_SPELL;   break;
        case MM_JUDICIOUS:  ft = FT_JUDICIOUS_SPELL;  break;
        case MM_MAXIMIZE:   ft = FT_MAXIMIZE_SPELL;   break;
        case MM_EXTEND:     ft = FT_EXTEND_SPELL;     break;
        case MM_PROJECT:    ft = FT_PROJECT_SPELL;    break;
        case MM_QUICKEN:    ft = FT_QUICKEN_SPELL;    break;
        case MM_REPEAT:     ft = FT_REPEAT_SPELL;     break;
        case MM_STILL:      ft = FT_STILL_SPELL;      break;
        case MM_SURE:       ft = FT_SURE_SPELL;       break;
        case MM_TRANSMUTE:  ft = FT_TRANSMUTE_SPELL;  break;
        case MM_UNSEEN:     ft = FT_UNSEEN_SPELL;     break;
        case MM_VILE:       ft = FT_VILE_SPELL;       break;
        case MM_VOCALIZE:   ft = FT_VOCALIZE_SPELL;   break;
        case MM_WARP:       ft = FT_WARP_SPELL;       break;
        default: Error("Illegal metamagic value in Player::ToggleMM!");
                            ft = FT_STILL_SPELL;      break;
      }
    if (!HasFeat(ft))
      return;
    MMArray[sp] |= mm;
  }

int16 Creature::SpellDmgBonus(rID eID) 
  { 
    TEffect *te = TEFF(eID);
    int16 db;
    if (te->HasFlag(EF_NO_BONUS_DMG)) 
      return 0;
    db = Mod(A_WIS) + AbilityLevel(CA_ARCANE_WILL);
    if (( (te->Vals(0)->eval == EA_BLAST && 
           te->Vals(0)->xval <= AD_LAST_PHYSICAL) ||
          (te->Vals(1) && te->Vals(1)->eval == EA_BLAST && 
           te->Vals(1)->xval <= AD_LAST_PHYSICAL) ||
          (te->Vals(2) && te->Vals(2)->eval == EA_BLAST && 
           te->Vals(2)->xval <= AD_LAST_PHYSICAL) ||
          (te->Vals(3) && te->Vals(3)->eval == EA_BLAST && 
           te->Vals(3)->xval <= AD_LAST_PHYSICAL)) &&
        HasAbility(CA_SPELL_FURY) &&
        (te->HasSource(AI_WIZARDRY) || te->HasSource(AI_SORCERY)))
      {
        if (isCharacter())
          if (!(thisc->Spells[theGame->SpellNum(eID)] & (SP_ARCANE | 
                                                        SP_SORCERY)))
            return db;
        /* For monsters, we assume that orcs get it for any spell
          that has AI_WIZARDRY/AI_SORCERY as a spell, even if it's,
          say, a staff spell or cleric spell. Not technically
          correct, but not a huge deal. */
        db += max(0,Mod(A_WIS));
      }
    return db;
  } 

String & TEffect::Power(int8 Mod, Creature *Caster, rID spID, Item *it)
  {
    EffectValues *ef; bool printed,percent;
    String str; printed = false;
    Dice d; EventInfo e;
    e.Clear();
    e.EActor = Caster;
    e.EVictim = Caster;
    e.EItem = it;
    if (it)
      e.isItem = true;
    e.eID = spID;
    e.vCasterLev = Mod;
    str = "";
    for(int16 i=0;ef = Vals(i);i++)
      {
        e.EMagic = ef;
        e.efNum = i;
        if (Caster)
          Caster->CalcEffect(e);
        if (Mod)
          d = ef->pval.LevelAdjust(Mod,Mod);
        else
          d = ef->pval.LevelAdjust(e.vCasterLev,Mod);
          
        if (Caster) { 
          if (ef->eval == EA_BLAST || ef->eval == EA_DRAIN)
            if (ef->xval <= 16) /* i.e., hit point damage */
              d.Bonus += Caster->SpellDmgBonus(spID);
          if (ef->eval == EA_INFLICT)
            if (IsAdjustStati(ef->xval) && ef->yval == A_ARM)
              d.Bonus += Caster->SpellDmgBonus(spID); 
          }
        // ww: don't display "[+4, +4]" --> "[+4]" suffices
        if (i > 0 && Vals(i-1)->eval == Vals(i)->eval &&
                     Vals(i-1)->xval == Vals(i)->xval &&
                     Vals(i-1)->pval == Vals(i)->pval)
            continue; 
        switch(Vals(i)->eval)
          {
            case EA_TRAVEL:
            case EA_BLAST:
            case EA_DRAIN:
            case EA_HEALING:
              if (printed)
                str += ", ";
              str += d.Str();
              printed = true;
             break;
            case EA_GRANT:
            case EA_INFLICT:
                if  (!IsAdjustStati(ef->xval) && 
                      ef->xval != RESIST &&
                      ef->xval != SAVE_BONUS && 
                      ef->xval != DETECTING && 
                      ef->xval != SKILL_BONUS)
                break;
              percent = false;
              if (ef->xval != RESIST && ef->xval != SAVE_BONUS &&
                  ef->xval != SKILL_BONUS)
                if (ef->yval == A_SPD_ARCHERY || ef->yval == A_SPD_BRAWL ||
                     ef->yval == A_SPD_MELEE || ef->yval == A_SPD_OFFHAND ||
                     ef->yval == A_SPD_THROWN || ef->yval == A_MOV ||
                     ef->yval == A_SPD || ef->yval == A_MAG || ef->yval == A_ARC ||
                     ef->yval == A_DIV)
                percent = true;
              if (printed)
                str += ", ";
              if (ef->pval.LevelAdjust(Mod,Mod).Sides == 1 ||
                  ef->pval.LevelAdjust(Mod,Mod).Sides == 0)
                str += Format("%+d%s",d.Roll() * (percent?5:1),
                  percent ? "~" : ""); 
              else 
                str += Format("%s%s",(const char*)d.Str(),
                  percent ? "x5~" : ""); 
              printed = true;
            default:
              break;
          }
      }
    return *tmpstr(str);
  }
                  
                                   
uint8 BlastColourSets[] = {
 /* BLACK */       BLACK, BLACK, BLACK, BLACK,
 /* BLUE */        BLUE, BLUE, BLUE, BLUE,
 /* GREEN */       GREEN, GREEN, GREEN, GREEN,
 /* CYAN */        CYAN, CYAN, CYAN, CYAN,
 /* RED */         RED, RED, RED, RED,
 /* PURPLE */      PURPLE, PURPLE, PURPLE, PURPLE,
 /* BROWN */       BROWN, BROWN, BROWN, BROWN,
 /* GREY */        GREY, GREY, GREY, GREY,
 /* SHADOW */      SHADOW, SHADOW, SHADOW, SHADOW,
 /* AZURE */       AZURE, AZURE, AZURE, AZURE,
 /* EMERALD */     EMERALD, EMERALD, EMERALD, EMERALD,
 /* SKYBLUE */     SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE,
 /* PINK */        PINK, PINK, PINK, PINK,
 /* MAGENTA */     MAGENTA, MAGENTA, MAGENTA, MAGENTA,
 /* YELLOW */      YELLOW, YELLOW, YELLOW, YELLOW,
 /* WHITE */       WHITE, WHITE, WHITE, WHITE,
 /* COL_FIRE */    RED, PINK, YELLOW, RED, 
 /* COL_COLD */    SKYBLUE, SKYBLUE, BLUE, WHITE,
 /* COL_MAGIC */   PINK, GREY, YELLOW, GREY,
 /* COL_NECRO */   SHADOW, SHADOW, BLACK, GREY,
 /* COL_ELEC */    YELLOW, CYAN, YELLOW, WHITE,
 /* COL_RAINBOW */ PINK, EMERALD, MAGENTA, SKYBLUE,
 /* COL_LIGHT */   WHITE, YELLOW, WHITE, WHITE,
 /* COL_POISON */  GREEN, GREEN, BLUE, GREY,
 /* COL_ACID */    GREEN, GREEN, EMERALD, BLACK,
 /* COL_DISINT */  EMERALD, EMERALD, GREEN, EMERALD,
 /* COL_PETRI */   GREY, GREY, BROWN, RED,
 /* COL_POLY */    PINK, PINK, RED, MAGENTA,
 /* COL_SONIC */   GREY, WHITE, WHITE, BROWN,
 /* COL_SILVERY */ WHITE, GREY, WHITE, SKYBLUE, 
                   0,0,0,0,
                   0,0,0,0 };


EffectValues* TEffect::Vals(int8 num)
  {
    Annotation *a;
    if (num == 0)
      return &ef;
    for(a=FAnnot();a;a=NAnnot()) {
      if (a->AnType == AN_EFFECT)
        if (!--num)
          return &(a->u.ef);
      }
    return NULL;
  }


/* This should really be rewritten to accept a rID instead of an
   event, for ease of use. Perhaps two overloaded functions? c.f.
   Thing::Event for PRE(EV_FIELDON/OFF) */

bool Magic::isTarget(EventInfo &_e, Thing *t)
  {
    EventInfo e; e = _e; e.ETarget = t;
    Creature *c; TEffect *te = TEFF(e.eID);
    Thing *ot; 
      if (!te) return true; 
      int16 lev, mt;
    if (e.EMagic == NULL)
      e.EMagic = &TEFF(e.eID)->ef;
    // ww: no matter what, there must be open air between you and the other
    // guy -- getting nuked by an orbital laser strike while the mage sits
    // in the bunker is too strong in incursion
    // fjm: ACKACKACK! Divination spells like Detect Objects et al use the
    // isTarget routine. I agree with this, but there needs to be a condition
    // to account for this. Later.
    // ww: however, you are a valig target for yourself even if you are not
    // InBounds() -- this happens when you put on your equipment at the
    // beginning of the game! 
    if (e.EActor && t && e.EActor != t) {
      if (!e.EActor->m ||
          (t->isCreature() && 
            (!t->m ||
             !(e.EActor->m == t->m) || 
             !(e.EActor->m->InBounds(e.EActor->x,e.EActor->y)) ||
             !(t->m->InBounds(t->x,t->y)) ||
             !(TEFF(e.eID)->HasFlag(EF_DISTANT) ||
               e.EActor->m->LineOfFire(e.EActor->x,e.EActor->y,t->x,t->y,e.EActor))))) 
        return false; 
      }
      
    /* Catch here things like Glyph of Warding across walls */
	  if (e.isLoc && !TEFF(e.eID)->HasFlag(EF_DISTANT) && e.EActor->m)
      if (!e.EActor->m->LineOfFire(e.EActor->x,e.EActor->y,e.EXVal,e.EYVal,e.EActor))
        if (e.EMagic->aval != AR_BOLT &&
            e.EMagic->aval != AR_BEAM &&
            e.EMagic->aval != AR_BALL &&
            e.EMagic->aval != AR_RAY &&
            e.EMagic->aval != AR_BURST &&
            e.EMagic->aval != AR_CHAIN &&
            !(e.MM & MM_WARP))
          return false;

    if (t->isCreature())
      {
        if (e.MM & MM_BIND)
          if (e.EActor == t)
            return false;
            
        if (e.MM & MM_JUDICIOUS)
          if (!((Creature*)t)->isHostileTo(e.EActor))
            return false;

            
        int16 test;
        e.Terse = false;
        switch (test = te->Event(e,e.eID,EV_ISTARGET)) {
          case CANNOT_CAST_IT: return false;
          case SHOULD_CAST_IT: 
          case CAN_CAST_IT: 
           break; 
          default: Error("EV_ISTARGET for <Res> returned?",e.eID);
          }
        if (te->HasFlag(EF_ITEMS_ONLY))
          return false;
        c = (Creature*)t;
        if (c == e.EActor && te->HasFlag(EF_CASTER_IMMUNE))
          return false;
        if (te->HasFlag(EF_ALLIES_IMMUNE) && c->isFriendlyTo(e.EActor))
          return false;
        if (te->HasFlag(EF_DEATH))
          if (c->ResistLevel(AD_NECR) == -1) /* "Death Ward" spell */
            return false;
        if (te->HasFlag(EF_MENTAL))
          if ((c->ResistLevel(AD_MIND) == -1) || !c->isMType(MA_LIVING)) 
            return false;

        if (te->HasFlag(EF_LIM_LEVEL)) {
          /* Skip this check for the EffectPrompt call */
          if (e.vCasterLev) {
            /* ww: this logic was a little weird before -- a level 1 mage
             * casting a heigtened sleep would get (1/2)*2 = 0. 
             * There was a mysterious 'lev+7,lev*2' min ... but since 
             * the greatest game enemy is CR 7, we'll skip that for now. */
            lev = LevelAdjust(e.EMagic->tval,e.vCasterLev*
                (e.MM & MM_HEIGHTEN) ? 2 : 1,
                (e.EItem && e.EItem->isItem()) ? e.EItem->GetPlus() : 0);

            if (lev < c->ChallengeRating())
              return false;
          }
        }
        if (te->HasFlag(EF_LIM_MTYPE)) {
          /* If you specify a tval only in the first segment, that
             goes for the entire effect. However, different segments
             can have different tvals, as MCvsE shows. */
          mt = e.EMagic->tval ? e.EMagic->tval :
                               TEFF(e.eID)->ef.tval;  
          switch(mt)
            {
              case MA_SUMMONED:
                if (c->HasStati(SUMMONED))
                  return true;
                return false;
              case MA_ENEMIES:
              case MA_HOSTILES:
                if (c->isHostileTo(e.EActor))
                  return true;
                return false;
              case MA_ALLIES:
                if (c == e.EActor || c->isFriendlyTo(e.EActor))
                  return true;
                return false;
              default:
                if (c->isMType(mt))
                  return true;
                return false;
            }
          }
        return true;
      }
    /* else if (t->Type == T_DOOR) return true; */ 
    else if (t->isItem() || t->isFeature()) {
      if (!te->HasFlag(EF_AFFECTS_ITEMS))
        return false;
      switch (te->Event(e,e.eID,EV_ISTARGET)) {
          case CAN_CAST_IT: 
          case SHOULD_CAST_IT:
            break;
          case CANNOT_CAST_IT: return false; 
          default: Error("EV_ISTARGET for <Res>",e.eID);
        }
      if (te->HasFlag(EF_LIM_ITYPE))
        return t->isType(e.EMagic->tval);
      return true;
      }
    else if (e.EMagic->eval == EA_TERRAFORM)
      return true; // you can aim that anywhere 
    else if (e.EMagic->aval == AR_BALL)
      return true; // you can aim a fireball nowhere special and hope
                   // that things get caught in the blast 
    /* What the hell IS it?! */
    else return false;
  }

  void Magic::CalcEffect(EventInfo &e) {
      TEffect *te = TEFF(e.eID); 
      int32 sp_flags = 0, i;
      int16 efNum;

      efNum = te->Vals(e.efNum) ? e.efNum : 0;
      e.EMagic = te->Vals(e.efNum);

      if (e.isItem && e.EItem->isItem() && e.EItem->isBlessed())
          e.isBlessed = true;
      if (e.isItem && e.EItem->isItem() && e.EItem->isCursed())
          e.isCursed = true;

      if (e.isTrap || e.isItem)
          sp_flags = 0;
      else if (e.EActor->isCharacter())
          sp_flags = e.EPActor->Spells[theGame->SpellNum(e.eID)];
      else {
          sp_flags = SP_ARCANE | SP_DIVINE | SP_SORCERY | SP_PRIMAL;

          // ugly kludge; sometimes gives higher caster level
          if (e.EActor->HasMFlag(M_INNATE))
              sp_flags |= SP_INNATE;
      }

      if (e.isItem) {
          e.saveDC = 10 + (e.EItem->GetPlus() * 2);
          if (e.EActor)
              e.saveDC += e.EActor->Mod(A_WIS);
          if (te->HasFlag(EF_HARDSAVE2))
              e.saveDC += 2;
          if (te->HasFlag(EF_HARDSAVE4))
              e.saveDC += 4;
          if (te->HasFlag(EF_HARDSAVE8))
              e.saveDC += 8;
      } else if ((e.AType == A_BREA || e.AType == A_SPIT ||
          e.DType == AD_SPE1 || e.DType == AD_SPE2) &&
          e.EActor->HasAttk(e.AType)) {
          e.saveDC = e.EActor->GetPower(e.EActor->GetAttk(e.AType)->u.a.DC);
      } else if (e.isTrap || !e.EActor)
          e.saveDC = 10 + te->Level;
      else if (TEFF(e.eID)->HasFlag(EF_SPECABIL))
          e.saveDC = 10 + e.EActor->ChallengeRating() + e.EActor->Mod(A_CHA);
      else 
          e.saveDC = e.EActor->getSpellDC(e.eID, e.isArcaneTrickery,e.MM & MM_HEIGHTEN);

      if (e.isItem && e.EItem && e.EItem->isType(T_SCROLL))
          e.vCasterLev = max(TEFF(e.EItem->eID)->Level*2-1,e.EActor->SkillLevel(SK_DECIPHER) - 2); 

      if (e.isItem && e.EItem && e.EItem->isItem())
          e.vCasterLev = e.EItem->ItemLevel();
      else if (!e.EActor)
          e.vCasterLev = te->Level;
      else if (e.isTrap) 
          e.vCasterLev = TEFF(e.eID)->Level; 
      else if (te->ef.aval == AR_POISON || te->ef.aval == AR_DISEASE)
          e.vCasterLev = te->Level;
      else if (sp_flags & SP_INNATE) 
          e.vCasterLev = max(1,e.EActor->ChallengeRating());
      else
          e.vCasterLev = e.EActor->CasterLev();

      StatiIterNature(e.EActor,BONUS_SCHOOL_CASTING_LEVEL) {
          if (S->Val & te->Schools)
              e.vCasterLev += S->Mag; 
      }
      StatiIterEnd(e.EActor)

      if ( (e.EActor->isMType(MA_LIZARDFOLK) && (TEFF(e.eID)->Schools & SC_WATER)) ||
        (e.EActor->isMType(MA_DWARF) && (TEFF(e.eID)->Schools & SC_EARTH)) ) {
        e.vCasterLev += 1;
        if (e.isItem)
        e.saveDC += 2;
      }

      if (e.EItem && e.EItem->isItem())
          e.vAlchemy = max(10,e.EItem->GetStatiMag(ALCHEMY_LEV));

      if (e.effIllusion && !(e.illFlags & IL_SHADE))
          if (e.EActor->isPlayer())
              e.vCasterLev += ((Player*)e.EActor)->Opt(OPT_OVERCAST); 

      e.vRange = (te->ef.lval && !te->ef.aval) ? LevelAdjust(te->Vals(efNum)->lval,
          e.vCasterLev, (e.EItem && e.EItem->isItem()) ? abs(e.EItem->GetPlus()) : 0) :
          5 + e.vCasterLev/2;
      e.vRadius = te->ef.lval ? LevelAdjust(te->Vals(efNum)->lval,
          e.vCasterLev, (e.EItem && e.EItem->isItem()) ? abs(e.EItem->GetPlus()) : 0) : 5;
      if (e.MM & MM_ENLARGE)
          e.vRadius *= 2;
      if (e.MM & MM_EXTEND)
          e.vRange *= 2;

      /* Calculate duration */
      if (te->HasFlag(EF_DSHORT))
          e.vDuration = Dice::Roll(1,4)+1;
      else if (te->HasFlag(EF_D1ROUND))
          e.vDuration = 2;
      else if (te->HasFlag(EF_DLONG))
          e.vDuration = 100 + e.vCasterLev*10;
      else if (te->HasFlag(EF_DXLONG))
          e.vDuration = 1000 + e.vCasterLev*100;
      else if (te->HasFlag(EF_PERSISTANT))
          e.vDuration = -2;
      else if (te->HasFlag(EF_PERMANANT))
          e.vDuration = -1;
      else
          e.vDuration = 10 + e.vCasterLev*2;
      if ((e.MM & MM_ANCHOR) && e.vDuration > 1)
          e.vDuration *= 2;
      if ((te->Purpose & EP_BUFF) && e.vDuration > 1 && 
          e.EActor->HasFeat(FT_MYSTIC_PREPARATION))
          e.vDuration *= 2; 
      if ((e.MM & MM_PERSISTANT) && e.vDuration >= 10)
          e.vDuration = -2;
      else if ((e.MM & MM_EXTEND) && e.vDuration >= 1)
          e.vDuration *= 2;

      if (e.MM & MM_TRANSMUTE) {
          char ch;
          ch = e.Transmute;
          switch (ch) {
          case 'f': e.DType = AD_FIRE; break;
          case 'c': e.DType = AD_COLD; break;
          case 'a': e.DType = AD_ACID; break;
          case 'l': e.DType = AD_ELEC; break;
          case 'p': e.DType = AD_TOXI; break;
          case 's': e.DType = AD_SONI; break;
          }
      } else if (TEFF(e.eID)->Vals(efNum)->eval == EA_BLAST)
          e.DType = TEFF(e.eID)->Vals(efNum)->xval;

      e.Dmg = TEFF(e.eID)->Vals(efNum)->pval.LevelAdjust(e.vCasterLev,
          ((e.EItem && e.EItem->isItem())) ? e.EItem->GetPlus() : 0);
      if (e.isSpell || (e.isItem && e.EItem->isType(T_SCROLL))) { 
          if (e.EMagic->eval == EA_BLAST || e.EMagic->eval == EA_DRAIN)
              if (e.EMagic->xval <= 16) /* i.e., hit point damage */
                  e.Dmg.Bonus += e.EActor->SpellDmgBonus(e.eID); 
          if (e.EMagic->eval == EA_INFLICT)
              if (IsAdjustStati(e.EMagic->xval) && e.EMagic->yval == A_ARM)
                  e.Dmg.Bonus += e.EActor->SpellDmgBonus(e.eID); 
      }

      if (e.MM & MM_MAXIMIZE)
          e.vDmg = max(0,e.Dmg.Number) * abs(e.Dmg.Sides) + e.Dmg.Bonus;
      else {
          if (e.Dmg.Sides < 0)
              e.Dmg.Sides = -e.Dmg.Sides;
          if (e.Dmg.Number < 0)
              e.Dmg.Number = 0;
          e.vDmg = e.Dmg.Roll();
      }
      if (e.MM & MM_EMPOWER)
          e.vDmg = (e.vDmg*150)/100;

      Dice od = e.Dmg;

      e.vChainMax = e.vCasterLev/4;

      int16 oldEvent = e.Event;
      e.Event = EV_CALC_EFFECT;
      TEFF(e.eID)->Event(e,e.eID);
      e.Event = oldEvent;

      /* Special kludge -- if EV_CALC_EFFECT changed e.Dmg, recalculate
      e.vDmg based on new e.Dmg, but otherwise let EV_CALC_EFFECT set
      e.vDmg on its own and don't overwrite. */
      if (!(od == e.Dmg)) {
          e.Dmg = TEFF(e.eID)->Vals(efNum)->pval.LevelAdjust(e.vCasterLev,
              ((e.EItem && e.EItem->isItem())) ? e.EItem->GetPlus() : 0);
          if (e.MM & MM_MAXIMIZE)
              e.vDmg = e.Dmg.Number * e.Dmg.Sides + e.Dmg.Bonus;
          else
              e.vDmg = e.Dmg.Roll();
          if (e.MM & MM_EMPOWER)
              e.vDmg = (e.vDmg*150)/100;
      }


  }

  EvReturn Magic::MagicEvent(EventInfo &e) {
      EvReturn r; Thing *t; int16 i;
      bool FieldDone = false,
          TransmutePromptDone = false;
      TEffect *te = TEFF(e.eID);

      /* Let's work out illusionary magic later. */
      if (e.EActor->isIllusion() || (e.EItem && e.EItem->isIllusion()))
          return ABORT; 

      r = NOTHING;

      /* fjm Mar 2006: removed something for "pseudo-effects" here
      because efNum wasn't being set properly. It should be set
      to 0 at the start of EV_EFFECT, which iterates through the
      whole Effect, right? If you change this, verify that the
      dragonshield doesn't fire when you equip it. */
      e.efNum = 0;

      /* Cases with magical throwing items (i.e., Javelin of Lightning),
      where we have it in Item2 and want it in Item for the Effect. */
      if (e.EItem2 && !e.EItem)
          e.EItem = e.EItem2;

      if (e.isActivation && e.EActor->m->FieldAt(e.EActor->x,e.EActor->y,FI_SILENCE)) {
          e.EActor->IPrint("You cannot speak command words in fields of magical silence.");
          return ABORT;
      }

      if (e.eID == 0 || (e.EItem && e.EItem->HasStati(DISPELLED))) {
Nothing:
          if (e.isWield || e.isRemove || e.isEnter || e.isLeave)
              return DONE;
          switch(random(4)) {
          case 0:e.EActor->IPrint("Nothing happens.");break;
          case 1:e.EActor->IPrint("There's no effect.");break;
          case 2:e.EActor->IPrint("It doesn't do anything.");break;
          case 3:e.EActor->IPrint("Nothing unusual occurs.");break;
          }
          return DONE;
      }

      if (e.MM & MM_TRANSMUTE)
          do {
              e.Transmute = e.EActor->ChoicePrompt("Fire, Cold, Acid, Lightning, Poison or Sonic damage?",
                  "fcalps", "fcalps"[random(6)]);
          } while (e.Transmute == -1);

      {
          uint16 align = 0;
          if (TEFF(e.eID)->HasFlag(EF_EVIL))
              align |= AL_NONGOOD;
          if (TEFF(e.eID)->HasFlag(EF_CHAOTIC))
              align |= AL_NONLAWFUL;
          if (TEFF(e.eID)->HasFlag(EF_LAWFUL))
              align |= AL_NONCHAOTIC;
          if (align && !(e.isTrap && e.isActOfGod))
              e.EActor->AlignedAct(align,1,"using aligned magic");
      }

      if (!e.isActivation && !e.isWield && !e.isRemove && !e.isEnter && !e.isLeave) {
          e.Event = MSG_CAST;
          te->Event(e,e.eID);
          e.Event = EV_EFFECT;
      }

      if (e.isActivation && (e.EItem && e.EItem->isItem()))
          if (te->HasFlag(EF_3PERDAY) || te->HasFlag(EF_7PERDAY) || te->HasFlag(EF_1PERDAY)) {
              if (te->HasFlag(EF_1PERDAY) && e.EItem->GetCharges() >= 1)
                  goto Nothing;
              if (te->HasFlag(EF_3PERDAY) && e.EItem->GetCharges() >= 3)
                  goto Nothing;
              if (te->HasFlag(EF_7PERDAY) && e.EItem->GetCharges() >= 7)
                  goto Nothing;
              e.EItem->SetCharges(e.EItem->GetCharges()+1);
          }

      if (te->HasFlag(EF_SOUND) && e.EMap) {
          if (e.EMap->FieldAt(e.EActor->x,e.EActor->y,FI_SILENCE)) {
              e.EActor->IDPrint("The magical silence negates your <Res>.",
                  "Nothing happens.", e.eID);
              return ABORT;
          }
          MapIterate(e.EMap,t,i)
              if (t->isCreature() && e.EActor->DistFrom(t) <= 30)
                  if (t->HasStati(SINGING) && !e.EMap->FieldAt(e.EActor->x,e.EActor->y,FI_SILENCE))
                      if (t->GetStati(SINGING)->Val == BARD_COUNTER) {
                          ((Creature*)t)->IDPrint("Your singing negates the effect.",
                              "The <Obj>'s singing negates the effect.",t);
                          return ABORT;
                      }
      }


      while (e.EMagic = te->Vals(e.efNum)) {
SkipLoop:

          CalcEffect(e);

          e.isFirstBlastXY = true;

          if (e.isActivation != te->HasFlag(EF_ACTIVATE+min(e.efNum,4)))
              { e.efNum++; continue; }

          if (te->HasFlag(EF_PERIODIC) && !e.isPeriodic) {                                         
              if (e.isWield || e.isEnter)
                  e.EActor->GainPermStati(PERIODIC, e.isWield ? e.EItem : NULL,SS_ITEM,e.EMagic->lval,0,e.eID);
              else if (e.isRemove || e.isLeave)
                  e.EActor->RemoveStati(PERIODIC, SS_ITEM, -1, -1, e.isItem ? e.EItem : NULL);
              else
                  e.EActor->GainTempStati(PERIODIC, e.isItem ? e.EItem : NULL,e.vDuration,SS_ITEM, e.EMagic->lval,0,e.eID);
              { e.efNum++; continue; }
          }

          if (e.isWield || e.isRemove || e.isEnter || e.isLeave)
              if (e.EMagic->eval != EA_GRANT)
                  if (e.EMagic->aval != AR_MFIELD) {
                    e.efNum++;
                    continue;
                  }

          e.Dmg = e.EMagic->pval.LevelAdjust(e.vCasterLev,((e.EItem && e.EItem->isItem())) ? e.EItem->GetPlus() : 0);

          if (e.isSpell || (e.isItem && e.EItem->isType(T_SCROLL))) { 
              if (e.EMagic->eval == EA_BLAST || e.EMagic->eval == EA_DRAIN)
                  if (e.EMagic->xval <= 16) /* i.e., hit point damage */
                      e.Dmg.Bonus += e.EActor->SpellDmgBonus(e.eID); 
              if (e.EMagic->eval == EA_INFLICT)
                  if (IsAdjustStati(e.EMagic->xval) && e.EMagic->yval == A_ARM)
                      e.Dmg.Bonus += e.EActor->SpellDmgBonus(e.eID); 
          }

          // ww: this looks very suspicious to me: you can't tell the
          // difference between a self hit and aiming a fireball at some floor
          // to hit some nearby monsters ... so I'll add the EXVal check
          if (!e.EVictim && e.EXVal <= 0)
              e.EVictim = e.EActor;

          r = te->Event(e,e.eID,0);

          switch(r) {
          case NOTHING: break;
          case DONE:    goto NextSegment;
          case ABORT:   return ABORT;
          }

          if (e.EMagic->eval == EA_HEALING &&
              e.EMagic->xval & HEAL_HP &&
              e.EMagic->aval == AR_NONE) {
              Creature *cr, *cr2, *ot; bool isPhoenix; int16 i;
              isPhoenix = false;
              /* Optimization -- only PCs sing! */
              cr = theGame->GetPlayer(0);
              if (cr && cr->HasStati(SINGING))
                  if (cr->GetStatiVal(SINGING) == BARD_PHOENIX)
                      if (cr->isFriendlyTo(e.EActor))
                          if (cr->DistFrom(e.EActor) <= cr->SkillLevel(SK_PERFORM)+2)
                              isPhoenix = true;

              if (isPhoenix) { 
                  ot = e.EVictim;
                  MapIterate(e.EMap,cr2,i)
                      if (cr2->isCreature() && cr2->isFriendlyTo(e.EActor) &&
                          cr2->DistFrom(cr) <= cr->SkillLevel(SK_PERFORM)+2)
                          if (cr2 == e.EActor ||
                              !e.EActor->m->FieldAt(cr2->x,cr2->y,FI_SILENCE))
                          {
                              e.EVictim = cr2;
                              ReThrow(EV_MAGIC_STRIKE,e);
                          }
                          e.EVictim = ot;
                          return DONE;
              }
          }

          switch(e.EMagic->aval) {
          case AR_BOLT:
          case AR_BEAM:
          case AR_BREATH:
          case AR_RAY:
          case AR_CHAIN:
          case AR_BALL:
          case AR_BURST:
              e.EActor->Reveal(true);
              // r = ABolt(e); break;
              r = ABallBeamBolt(e); break;
          case AR_TOUCH:
              r = ATouch(e); break;
          case AR_FIELD:
          case AR_MFIELD:
              /* We only ever want to create one field per effect, even if
              that effect has a whole slew of EffectValues. */
              /* The logic of the isActivbation thing is this: when a field
              has a continual effect, we use the isActivation flag to
              signify its heartbeat (i.e., damage from Summon Swarm).
              In this case, e.EItem will be NULL. If we're activing an
              item that produces a field (Horn of Fog), e.EItem will not
              be NULL, so we go to AField(). */
              if ((e.isActivation && !e.EItem && (e.EMagic->rval & FI_CONTINUAL)) || e.isEnter || e.isLeave)
                  r = ReThrow(EV_MAGIC_STRIKE,e);
              else if (!FieldDone) {
                  FieldDone = true;
                  r = AField(e); 
              }
              break;
          case AR_BARRIER:
              e.EActor->Reveal(true);
              r = ABarrier(e); break;
          case AR_GLOBE:
              /* Globes don't *inherently* give away your pos. */
              r = AGlobe(e); break;
          case AR_NONE:
              r = ReThrow(EV_MAGIC_STRIKE,e); 
              break;
          case AR_POISON:
              if (e.EVictim->ResistLevel(AD_TOXI)!=-1)
                  if (isTarget(e,e.EVictim))
                      r = ReThrow(EV_MAGIC_STRIKE,e);
              break;
          case AR_DISEASE:
              if (e.EVictim->ResistLevel(AD_DISE)!=-1)
                  if (isTarget(e,e.EVictim))
                      r = ReThrow(EV_MAGIC_STRIKE,e);
              break;
          default:
              Error("Unknown aval in effect (aval %d, effect %s)!",
                  e.EMagic->aval,(const char*)NAME(e.eID));
              return ABORT;
          }

          if (e.efNum == -1) /* Pseudo-effects */
              return DONE;

          while(te->Vals(e.efNum+1) && 
              (te->Vals(e.efNum+1)->aval == te->Vals(e.efNum)->aval))
              e.efNum++;

          if (r == ERROR || r == ABORT)
              return r;
NextSegment:
          e.efNum++;
      }

      if (e.EMap)
          e.EMap->PrintQueue(QUEUE_DAMAGE_MSG);    

      if (e.doVUpdate && e.EActor && e.EActor->m)
          e.EActor->m->VUpdate(e.EActor->x,e.EActor->y);

      if (!e.isSomething && !(r == DONE) && !FieldDone)
          goto Nothing;
      return r;
  }

EvReturn Magic::MagicStrike(EventInfo &e)
  {
    EvReturn r; bool nonPartialSave = false;
    TEffect *te = TEFF(e.eID); uint32 save_fl;
    int8 first_efNum = e.efNum; Creature *c;
    Item *it;

    
    if (te->HasFlag(EF_ATTACK) && e.AType != A_SPEL)
      {
        e.AType   = A_SPEL;
        /* This is a kludge to represent the fact that we can't get
           Base Attack Bonus easily -- it should be the character's
           BAB that's applied here. */
        e.vHit    = e.EActor->GetBAB(S_ARCHERY);
        e.vDef    = e.EVictim->GetAttr(A_DEF);
        e.vThreat = 20;
        return ReThrow(EV_STRIKE,e);
      }

   e.Resist = false;
   e.Immune = false;
   /* Kludge to prevent multiple EV_MAGIC_HIT messages 
      for one target; see Resource::Event. */
   e.MHMessageDone = false;
   e.BlastMessageDone = false;

    /* At this point we need to have one concrete target that we
       are effecting, even if it's the caster. Thus, ensure that
       we have a valid EVictim. */
    if (!e.ETarget)
      e.ETarget = e.EActor;

    if (e.eID && TEFF(e.eID)->HasFlag(EF_GAZE)) {
      if (e.EVictim->HasStati(GAZE_REFLECTION,GR_IMMUNE))
        e.Immune = true;
      else if (e.EVictim->HasStati(GAZE_REFLECTION,GR_REFLECT))
        {
          e.EActor->IDPrint("Your gaze is reflected back at you!",
            "The <Obj>'s gaze is reflected back at <him:Obj>!", e.EActor);
          e.EVictim = e.EActor;
        }
      }

    if (e.ETarget->isCreature() && !TEFF(e.eID)->HasSource(AI_PSIONIC) &&
                                   !TEFF(e.eID)->HasFlag(EF_MUNDANE))
        if ((e.EActor->isHostileTo(e.EVictim) ||
            e.EVictim->isHostileTo(e.EActor)) &&
            e.EVictim->GetAttr(A_MR) > 0) { 
          int roll = random(100) + 1; 
          int actor_side = 
            (roll + e.EActor->ChallengeRating() * 2) +
            (e.EActor->HasFeat(FT_SPELL_PENETRATION) ? 20 : 0);
          bool bypass_mr = 
            actor_side >
            (e.EVictim->GetAttr(A_MR) + e.EVictim->ChallengeRating() * 2);

          Term * TActor = e.EActor->ShowCombatNumbers();
          Term * TVictim = e.EVictim->ShowCombatNumbers();

          if (TActor || TVictim) { 
            String smr;
            smr = XPrint((const char *)Format("<10>Magic Res:<7> 1d100 (%d) %+d%s = %d vs %d %+d = %d [%s]",
                roll, 
                e.EActor->ChallengeRating() * 2,
                (e.EActor->HasFeat(FT_SPELL_PENETRATION) ?  " +20 feat" : ""),
                actor_side, 
                e.EVictim->GetAttr(A_MR),
                e.EVictim->ChallengeRating() * 2,
                e.EVictim->GetAttr(A_MR) + e.EVictim->ChallengeRating() * 2,
                bypass_mr ? "bypassed" : "resisted"));
            if (TActor) {
              TActor->SetWin(WIN_NUMBERS3);
              TActor->Clear();
              TActor->Write(smr);
              if (theGame->Opt(OPT_STORE_ROLLS))
                TActor->AddMessage(smr);
              } 
            if (TVictim) {
              TVictim->SetWin(WIN_NUMBERS3);
              TVictim->Clear();
              TVictim->Write(smr);
              if (theGame->Opt(OPT_STORE_ROLLS))
                TVictim->AddMessage(smr);
            } 
          }

          if (!bypass_mr)  
          e.MagicRes = true;

        } 

      if (e.effIllusion)
        {
          uint16 saveFlags, ocast; 
          Creature *Illusionist;
          
          if (e.EActor->m)
            e.EActor->m->SetQueue(QUEUE_DISBELIEF_MSG);
          
          if (!e.ETarget->isCreature())
            { e.effDisbelieved = true; goto Disbelieved; }
          if (e.EVictim->HasStati(TRUE_SIGHT))
            { e.effDisbelieved = true; goto Disbelieved; }
          if (e.EVictim->HasMFlag(M_MINDLESS))
            { e.effDisbelieved = true; goto Disbelieved; }
          
          if (!(e.illFlags & IL_IMPROVED))
            if (e.EVictim->HasAbility(CA_SHARP_SENSES) ||
                  e.EVictim->HasAbility(CA_TREMORSENSE)  ||
                  e.EVictim->HasAbility(CA_BLINDSIGHT) ||
                  e.EVictim->HasAbility(CA_SCENT) )
              { e.effDisbelieved = true; goto Disbelieved; }
          if (!e.EActor->isRealTo(e.EVictim))
            { e.effDisbelieved = true; goto Disbelieved; }
              
          saveFlags = SA_ILLUS | SA_MAGIC;
          if (TEFF(e.ill_eID)->HasSource(AI_WIZARDRY) ||
              TEFF(e.ill_eID)->HasSource(AI_SORCERY) ||
              TEFF(e.ill_eID)->HasSource(AI_THEURGY) ||
              TEFF(e.ill_eID)->HasSource(AI_BARDIC) ||
              TEFF(e.ill_eID)->HasSource(AI_WITCHCRAFT) )
            saveFlags |= SA_SPELLS;
          if (TEFF(e.ill_eID)->HasFlag(EF_EVIL))
            saveFlags |= SA_EVIL;
          if (TEFF(e.ill_eID)->Schools & SC_NEC)
            saveFlags |= SA_NECRO;
          
          Illusionist = e.EActor;
          while (Illusionist && Illusionist->HasStati(ILLUSION))
            Illusionist = (Creature*) Illusionist->GetStatiObj(ILLUSION);
          if (Illusionist->isPlayer() && !(e.illFlags & IL_SHADE))
            ocast = -theGame->Opt(OPT_OVERCAST)*2;
          else
            ocast = 0; 
                        
          if (e.EVictim->SavingThrow(WILL,10 + (Illusionist ? Illusionist->
                SkillLevel(SK_ILLUSION) / 2 : 10) + ocast + (e.isArcaneTrickery ? 8 : 0)
                + (e.EActor->HasStati(SCHOOL_FOCUS,SC_ILL) ? 2 : 0), saveFlags))
            {
              e.EActor->IPrint("The <Obj> disbelieves!", e.EVictim);
              e.effDisbelieved = true;
              goto DisbeliefMessageDone;
            }
          
          Disbelieved:
          if (e.effDisbelieved && e.EVictim->isCreature())
            e.EActor->IPrint("The <Obj> sees through your illusion automatically.", e.EVictim);
          else if (e.effDisbelieved)
            APrint(e,"The <Obj> is unaffected.", e.EVictim);
          DisbeliefMessageDone:
          if (e.EActor->m)
            e.EActor->m->UnsetQueue(QUEUE_DISBELIEF_MSG);
          
          
        }            

      bool doResistDeath;
      doResistDeath = false;
      if (TEFF(e.eID)->HasFlag(EF_DEATH))
        if (e.EVictim && e.EVictim->isCreature())
          if (e.EVictim->HasFeat(FT_RESIST_DEATH))
            {
              int16 count;
              count = e.EVictim->GetStatiMag(TRIED,FT_RESIST_DEATH);
              if (count < e.EVictim->Mod(A_CON)*2)
                {
                  e.EVictim->IncStatiMag(TRIED,SS_MISC,FT_RESIST_DEATH,NULL,1);
                  doResistDeath = true;
                  TPrint(e,"The <EVictim> shrugs off your death magic with sheer toughness!",
                    "You shrug off the <EActor>'s death magic with sheer toughness! (<Num> left).", 
                           NULL, max(0,e.EVictim->Mod(A_CON)*2 - (count+1)));
                }
            }
        
    do {
      /* Set EMagic to the current effect component */
      e.EMagic = te->Vals(e.efNum);

      if (e.isActivation != te->HasFlag(EF_ACTIVATE+min(e.efNum,4)))
        goto SkipSegment;

      e.Immune = e.MagicRes || doResistDeath || !isTarget(e,e.ETarget);

            

      /* Changed to accomidate Magic C vs. E */
      if (e.Immune)
        goto SkipSegment;
 
      CalcEffect(e);

      /* Handle DType & Transmute Spell */
      if (e.EMagic->eval == EA_BLAST) {
        if (!(e.MM & MM_TRANSMUTE) || !is_elemental_dmg(e.EMagic->xval))
          e.DType = e.EMagic->xval;
        }

      /* We make a new saving throw for a given effect component if and
         only if A) it's the first component in this strike, or B) if it
         has a different sval from the component immediately before it. */      
      if (e.EMagic->aval != AR_POISON && e.EMagic->aval != AR_DISEASE)
        if (!e.Resist && ((!te->HasFlag(EF_PARTIAL)) || e.EMagic->eval != EA_BLAST))
          if (e.ETarget->isCreature() && e.EActor != e.EVictim)
            if ((e.efNum == first_efNum) || (te->Vals(e.efNum-1)->sval != 
                                               e.EMagic->sval)) 
              {
                save_fl = SA_MAGIC | SA_SPELLS;
                if (te->HasFlag(EF_DEATH))
                  save_fl |= SA_DEATH;
                if (e.EMagic->eval == EA_INFLICT && e.EMagic->xval == PARALYSIS)
                  save_fl |= SA_PARA;
                if (te->HasFlag(EF_EVIL) || e.EActor->isMType(MA_EVIL))
                  save_fl |= SA_EVIL;
                if (te->Schools & SC_NEC)
                  save_fl |= SA_NECRO;
                if (te->Schools & SC_ENC)
                  save_fl |= SA_ENCH;
                if (te->Schools & SC_ILL)
                  save_fl |= SA_ILLUS;
                if (e.EMagic->eval == EA_INFLICT && e.EMagic->xval == CONFUSED)
                  save_fl |= SA_CONF;
                if (e.EMagic->eval == EA_INFLICT && e.EMagic->xval == CHARMED &&
                       e.EMagic->yval == CH_DOMINATE)
                  save_fl |= SA_COMP;
                if (e.EMagic->eval == EA_INFLICT && e.EMagic->xval == STONING)
                  save_fl |= SA_PETRI;
                if (e.EMagic->eval == EA_INFLICT && e.EMagic->xval == AFRAID)
                  save_fl |= SA_FEAR;
                if (TEFF(e.eID)->HasFlag(EF_FEAR))
                  save_fl |= SA_FEAR;

                e.Resist = false;
                if (e.EMagic->sval != NOSAVE && e.EMagic->sval < 10 &&
                    !(e.EVictim->HasStati(ANIMAL_COMPANION,TA_LEADER,e.EActor)))
                  if (e.EVictim->SavingThrow(e.EMagic->sval, e.saveDC, save_fl))
                    {
                      e.Resist = true;
                      nonPartialSave = true;
                    }
              }


        
        /* If we haven't resisted the effect, or we did resist but still
           need to apply half damage, throw the effect. */
        if (!e.Immune)
          if ((!e.Resist) || (te->HasFlag(EF_PARTIAL) && 
          // ww: previously breath weapons never did any damage -- they
          // have no EMagic field
              (!e.EMagic || e.EMagic->eval == EA_BLAST)))
            {
              r = ReThrow(EV_MAGIC_HIT,e);
              if (r == ABORT) 
                return ABORT;
              if (!e.Immune)
                e.notFullyImmune = true;
            }   

        
        SkipSegment:
        /* If there's another effect component right after this one, and
           it has the same aval as this one, perform it as well immediately. */
        if (te->Vals(e.efNum+1) && (te->Vals(e.efNum+1)->aval == e.EMagic->aval))
          e.efNum++;
        else
          break;
          
      }
    while (1);

    /* If the target was immune, skip over all the segments that have the same
       area as this one. */
    while(te->Vals(e.efNum+1) && te->Vals(e.efNum+1)->aval == te->Vals(e.efNum)->aval)
      e.efNum++;

    /* We do not print the "unaffected" message when:
       1. The target is not immune. (duh)
       2. The target was affected by a different segment.
       3. The target is not a creature, and the spell has an area.
       4. The target is a creature, and the spell affects items only.
       5. The target is the caster, and the spell has an area.
       6. The spell is going to affect the target's equipment.
    */
    
    if (e.EActor->m)
      e.EActor->m->PrintQueue(QUEUE_DISBELIEF_MSG);
    
    if (e.Immune && !e.notFullyImmune)
      if (e.EVictim->isCreature() || te->Vals(e.efNum)->aval == 0)
        if (e.EVictim != e.EActor || te->Vals(e.efNum)->aval == 0)
          if (e.EVictim->isItem() || !TEFF(e.eID)->HasFlag(EF_ITEMS_ONLY))
            if (!(te->Vals(e.efNum)->qval & Q_EQU))
              {
                VPrint(e,"You are unaffected.", 
                  "The <EVictim> is unaffected.");
                goto SkipResistMsg;
              }
    if (nonPartialSave)
      if (!(e.EVictim->Flags & F_DELETE))
        {
          TPrint(e,"The <EVictim> resists your magic.",
            "You resist the <EActor>'s magic.",NULL);
        }
    SkipResistMsg:

    if (TEFF(e.eID)->ef.qval & Q_EQU)
        if (e.EVictim->isCreature()){
          // ww: as with EV_DAMAGE/AD_RUST, if you are polymorphed this won't
          // hurt merged equipment
          for (it = e.EVictim->FirstInv(); it; it = e.EVictim->NextInv()) {
            if (it->GetParent() == e.EVictim) /* Exposed Equipment Only */
              {
                bool goodSlot = false;
                for (int s =0; s<SL_LAST; s++)
                  if (e.EVictim->InSlot(s) == it)
                    goodSlot = true;
                if (goodSlot) { 
                  EventInfo eCopy = e; 
                  eCopy.ETarget = it;
                  int res = ReThrow(EV_DAMAGE,eCopy);
                }
              }
          }
      }
    if (e.EMagic->aval == AR_BALL || TEFF(e.eID)->HasFlag(EF_HIT_MOUNT))
      if (e.EVictim->HasStati(MOUNTED))
        {
          c = e.EVictim;
          e.ETarget = c->GetStatiObj(MOUNTED);
          ReThrow(EV_MAGIC_STRIKE,e);
          e.EVictim = c;
        }
    if (e.EMap && e.AType != A_TUCH)
      e.EMap->PrintQueue(QUEUE_DAMAGE_MSG);    
    return DONE;
  }

EvReturn Magic::MagicHit(EventInfo &e)
  {
    EvReturn r;       

    if (!e.EMagic)
      e.EMagic = &TEFF(e.eID)->ef;       

    switch(e.EMagic->eval)
      {
        case EA_BLAST: r = Blast(e); break;
        case EA_GRANT: r = Grant(e); break;
        case EA_DRAIN: r = Drain(e); break;
        case EA_INFLICT: r = Inflict(e); break;
        case EA_OVERRIDE: r = Override(e); break;
        case EA_DETECT: r = Detect(e); break;
        case EA_TRAVEL: r = Travel(e); break;
        case EA_HEALING: r = Healing(e); break;
        case EA_DISPEL: r = Dispel(e); break;
        case EA_POLYMORPH: r = Polymorph(e); break;
        case EA_REVEAL: r = Reveal(e); break;
        case EA_SUMMON: r = Summon(e); break;
        case EA_ILLUSION: r = Illusion(e); break;
        case EA_CONSTRUCT: r = EConstruct(e); break;
        case EA_VISION: r = Vision(e); break;
        case EA_TERRAFORM: r = Terraform(e); break;
        case EA_CREATION: r = Creation(e); break;
        case EA_GENERIC: r = NOTHING; break;
        default: e.EActor->IPrint("That effect isn't implemented yet.");
          return ABORT;
      }
    return r;
  }

EvReturn Magic::MagicXY(EventInfo &e,int16 tx,int16 ty)
  {
    Map *m; EvReturn r, r2; int16 oe, o_efNum;

    e.EXVal = tx; e.EYVal = ty;
    if (!e.EActor) return DONE;
    m = e.EActor->m;
    if (!m) return DONE; 

    oe = e.Event;
    e.Event = EV_MAGIC_XY;
    if (m->dID) {
      r = RES(m->dID)->Event(e,m->dID);
      if (r != NOTHING && r != NOMSG)
        { e.Event = oe; return r; }
      }
    r = TREG(m->RegionAt(tx,ty))->Event(e,m->RegionAt(tx,ty));
    if (r != NOTHING && r != NOMSG)
      { e.Event = oe; return r; }
    r = TTER(m->TerrainAt(tx,ty))->Event(e,m->TerrainAt(tx,ty));
    if (r != NOTHING && r != NOMSG)
      { e.Event = oe; return r; }
    r = TEFF(e.eID)->Event(e,e.eID);
    if (r != NOTHING && r != NOMSG)
      { e.Event = oe; return r; }
    e.Event = oe;

    r = NOTHING;
    r2 = NOTHING;
    o_efNum = e.efNum;
    do {
      e.EMagic = TEFF(e.eID)->Vals(e.efNum);
      if (!e.EMagic) 
        break;
      switch(e.EMagic->eval)
        {
          case EA_BLAST:
            /* Melt ice, etc. */
            break;
          case EA_TERRAFORM:          
            CalcEffect(e);
            r2 = Terraform(e);
            if (r2 == ABORT)
              return ABORT;
            if (r2 != NOTHING && r2 != NOMSG)
              r = r2;
          default:
           break;
        }
      e.efNum++;
      }
    while (TEFF(e.eID)->Vals(e.efNum) &&
           TEFF(e.eID)->Vals(e.efNum)->aval ==
             TEFF(e.eID)->Vals(e.efNum-1)->aval);
    e.efNum = o_efNum; 
    
    return r2;
  }

#if 0
Construct::Construct(int16 _Type, Creature *_Creator,rID _mID,
  rID _spID, int16 _Duration)
  : Monster(_mID)
  {
    Creator = _Creator->myHandle;
    spID = _spID;
    CType = _Type;
    Duration = _Duration;
  }

void Construct::CalcValues()
  {
    Creature::CalcValues();
  }

void Construct::ChooseAction()
  {
    Monster::ChooseAction();
  }
#endif


// ww: before these effects were calling StopWatch even if the player was
// nowhere nearby 
bool PossiblyPause(Term *T1, int x, int y, int timeout)
{
  Player *p = theGame->GetPlayer(0);
  if (p && p->m->isGenerating())
    return false;
  if (!p->m->InBounds(p->x,p->y) ||
      !p->m->InBounds(x,y))
    return false;
  else if (p->m->LineOfSight(p->x,p->y,x,y,p)) {
    if (timeout) T1->StopWatch(timeout);
    return true; 
  } 
  return false; 
} 


EvReturn Magic::AGlobe(EventInfo &e)
  {
    Map *m; Thing *t; Glyph g;
      TEffect *te = TEFF(e.eID);
    int16 i, cx,cy,x, y, x1,x2,y1, _efNum = e.efNum;
    m = e.EActor->m;
    if (e.isLoc)
      { cx = e.EXVal; cy = e.EYVal; }
    else
      { cx = e.ETarget->x; cy = e.ETarget->y; }

    /*
    e.vRange = e.EMagic->lval;

    if (e.MM & MM_ENLARGE)
      e.vRange *= 2;
    if (e.MM & MM_FOCUS)
      e.vRange /= 2;
    */
    
    /* Do Highlight Effect */
    if (m && (!m->isGenerating()) && e.EMagic->cval && PossiblyPause(T1,cx,cy,0)) {
      for(x = cx-e.vRadius; x <= cx+e.vRadius; x++)
        for(y = cy-e.vRadius; y <= cy+e.vRadius; y++)
          if ((dist(cx,cy,x,y) < e.vRadius) && m->InBounds(x,y))
            {
              g = T1->GetGlyph(x,y);
              g = (g & 0x00FF) | (e.EMagic->cval << 8);
              T1->PutGlyph(x,y,g);
            }
      T1->Update();
      PossiblyPause(T1,cx,cy, e.EActor->isPlayer() ? 1500 : 500);
      T1->RefreshMap();
      T1->Update();
      }
    
    for(x = cx-e.vRadius; x <= cx+e.vRadius; x++)
      for(y = cy-e.vRadius; y <= cy+e.vRadius; y++)
        if ((dist(cx,cy,x,y) < e.vRadius) && m->InBounds(x,y))
          {
            MagicXY(e,x,y);
            e.isFirstBlastXY = false;
          }
    e.EXVal = cx; 
    e.EYVal = cy;

    if (!m)
      return DONE;

    MapIterate(m,t,i)
        if (dist(t->x,t->y,cx,cy) < e.vRadius)
        { e.ETarget = t; e.efNum = _efNum;
            if (t == e.EActor && te && te->HasFlag(EF_CASTER_IMMUNE))
              ;
            else 
              ReThrow(EV_MAGIC_STRIKE,e); 
          }
    return DONE;
  }

EvReturn Magic::ABarrier(EventInfo &e)
  {
    int16 x1,y1,x2,y2,lim, _efNum = e.efNum; 
    bool stop1, stop2;
    Creature *cr; Map *m = e.EActor->m;
    /* Later, metamagic feat to allow set hemisphere effect around
       player, like so:
           ###
          ## ##
          # @ #
          ## ##
           ###

      Hemisphere Spell
    */
    /* The ABarrier area of effect is normally only used with the
       EA_TERRAFORM archetype, and I can't guarantee its function
       with any other archetype. That said, I can't imagine how it
       would be useful with any other archetype, either. */

    if (e.EDir != NORTH && e.EDir != SOUTH && e.EDir != EAST &&
      e.EDir != WEST)
      {
        e.EActor->IPrint("A <9><Res><7> can only be erected along a cardinal direction.",
          e.eID);
        return ABORT;
      }

    if (e.EMagic->eval == EA_TERRAFORM) {
      if ((TTER(e.EMagic->rval)->Image & 0x00FF) == 179)
        if (e.EDir == NORTH || e.EDir == SOUTH)
          TTER(e.EMagic->rval)->Image = 
            (TTER(e.EMagic->rval)->Image & 0xFF00) | 196;
      if ((TTER(e.EMagic->rval)->Image & 0x00FF) == 186)
        if (e.EDir == NORTH || e.EDir == SOUTH)
          TTER(e.EMagic->rval)->Image = 
            (TTER(e.EMagic->rval)->Image & 0xFF00) | 205;
      }

    x1 = x2 = e.EActor->x + DirX[e.EDir];
    y1 = y2 = e.EActor->y + DirY[e.EDir];

      if (!m)
        return ABORT; 


      if (m->SolidAt(x1,y1))
      {
        e.EActor->IPrint("You can't conjure a wall into solid stone!");
        return DONE;
      }

    stop1 = stop2 = false;
    if (e.MM & MM_ENLARGE)
      lim = max(e.vRadius*2,20);
    else
      lim = max(e.vRadius,10);

    while ((!stop1) || (!stop2)) {
      if (!stop1) {
          if (m->SolidAt(x1,y1))
          stop1 = true;
        else
          {
            MagicXY(e,x1,y1);
            e.isFirstBlastXY = false;            
            lim--;
          }
          if (!m->SolidAt(x1,y1))
          for (cr=m->FCreatureAt(x1,y1);cr;cr=m->NCreatureAt(x1,y1))
            if (!cr->HasMFlag(M_INCOR))
              cr->PlaceNear(x1,y1);

        }
      if (!stop2 && ((x1 != x2) || (y1 != y2))) {
          if (m->SolidAt(x2,y2))
          stop2 = true;
        else
          {
            MagicXY(e,x2,y2);
            e.isFirstBlastXY = false;            
            lim--;
          }
          if (!m->SolidAt(x2,y2))
          for (cr=m->FCreatureAt(x2,y2);cr;cr=m->NCreatureAt(x2,y2))
            if (!cr->HasMFlag(M_INCOR))
              cr->PlaceNear(x2,y2);
        }
      x1 += DirY[e.EDir];
      y1 += DirX[e.EDir];
      x2 -= DirY[e.EDir];
      y2 -= DirX[e.EDir];
      T1->Update();
      if (!PossiblyPause(T1,x1,y1,15)) PossiblyPause(T1,x2,y2,15);
      /*if (lim <= 0)
        goto LimitBarrier;*/
      }

      { Thing * t; int i; 
        MapIterate(e.EMap,t,i) {
          if (t->isType(T_DOOR))
            t->SetImage();
        } 
      }

    return DONE;

#if 0
    LimitBarrier:
    DPrint(e,"Since your caster level does not let you create a wall large "
      "enough to anchor itself on both sides, it ceases to form.",
      "The <EActor>'s conjured wall collapses.");
    b->Remove();
    return DONE;

    DropBarrier:
    DPrint(e,"Since there is a creature in its path, the wall ceases to form.",
      "Since there is a creature in its path, the wall ceases to form.");
    b->Remove();

#endif
  }


EvReturn Magic::AField(EventInfo &e)
  {
    Glyph g; int16 tx,ty,i, x, y;
    if (!e.EActor || !e.EActor->m) return ABORT;
    
      /* Later, figure out if blast, hit, target, self, etc. */
    if (e.isRemove) {
      e.EMap->RemoveEffField(e.eID);
      return DONE;
      }
    else if (e.isWield) 
      e.vDuration = -1;
      
    if (TEFF(e.eID)->HasFlag(EF_GLYPH_INSECTS))
      g = '`';
    else if (TEFF(e.eID)->HasFlag(EF_GLYPH_FOG))
      g = '~';
    else if (TEFF(e.eID)->HasFlag(EF_GLYPH_FLOOR))
      g = GLYPH_FLOOR;
    else
      g = GLYPH_FLOOR2;

    g |= e.EMagic->cval * 256;

    tx = e.EXVal ? e.EXVal : e.ETarget ? e.ETarget->x : e.EActor->x;
    ty = e.EYVal ? e.EYVal : e.ETarget ? e.ETarget->y : e.EActor->y;

    e.vRadius = LevelAdjust(e.EMagic->lval,e.vCasterLev,(e.EItem && e.EItem->isItem()) ? e.EItem->GetPlus() : 0);
    e.vRadius = abs(e.vRadius); 
    if (e.MM & MM_ENLARGE)
      e.vRadius *= 2;
    else if (e.MM & MM_FOCUS)
      e.vRadius = max(1,e.vRadius/2);
    
    for(x = tx-e.vRadius; x <= tx+e.vRadius; x++)
      for(y = ty-e.vRadius; y <= ty+e.vRadius; y++)
        if ((dist(tx,ty,x,y) < e.vRadius) && e.EActor->m->InBounds(x,y))
          {
            MagicXY(e,x,y);
            e.isFirstBlastXY = false;
          }
    e.EXVal = tx; 
    e.EYVal = ty;

    e.EActor->m->NewField(e.EMagic->rval | (e.EMagic->aval == AR_MFIELD ? FI_MOBILE : 0),
        tx,ty,e.vRadius, g, e.vDuration,e.eID,e.EActor);
    return DONE;
  }


// ww: Beam, Ball and Bolt should all work the same way, more or less
// let's remove these arbitrary distinctions that make beams only work on
// cardinal directions and balls occasionally do weird things ... 
// If isBeam is true, we travel through creatures and continue to strike
// things.
// If isBall is true, at the end of our travels we explode outward. 

EvReturn Magic::ABallBeamBolt(EventInfo &e)
{
  EventInfo eCopy; Feature *ft; Creature *cr;
  TEffect *te = NULL; int16 tc, j, k, ti, ac;
  static Thing* TargetList[128], *ArcList[64];
  bool isBeam, isMulti, isChain, isBall,
       isProject, wasEngulfed;

  if (!e.EActor->m)
    return ABORT;

  if (e.MM & MM_PROJECT)
    { isBeam = true; isMulti = false;
      isChain = false; isBall = false;
      isProject = true; }
  else switch (e.EMagic ? e.EMagic->aval : AR_BEAM)
    {
      case AR_BOLT:
        isBeam = false; isMulti = false;
        isChain = false; isBall = false;
        isProject = true;
       break; 
      case AR_BEAM:
      case AR_BREATH:
        isBeam = true; isMulti = true;
        isChain = false; isBall = false;
        isProject = true;
       break;        
      case AR_RAY:
        isBeam = true; isMulti = false;
        isChain = false; isBall = false;
        isProject = true;
       break;        
      case AR_CHAIN:
        isBeam = true; isMulti = true;
        isChain = true; isBall = false;
        isProject = true;
       break;        
      case AR_BALL:
        isBeam = false; isMulti = false;
        isChain = false; isBall = true;
        isProject = true;
       break;        
      case AR_BURST:
        isBeam = false; isMulti = false;
        isChain = false; isBall = true;
        isProject = false;
       break;        
    }

  #define ADD_TARGET(t)                         \
     if (tc < 127) {                            \
       for(k=ti;k!=tc;k++)                      \
         if (TargetList[k] == t)                \
           break;                               \
       if (k == tc)                             \
         TargetList[tc++] = t;                  \
       }                                        
  tc = 0; ti = 0; ac = 0;

  if (e.eID) te = TEFF(e.eID);

  // ww: WarpCount 
  int16 WarpCount, ChainCount;
  if (e.MM & MM_WARP)
    WarpCount = max(2,3+e.EActor->Mod(A_INT));
  else
    WarpCount = 0;

  int ourEvent = e.isNAttack ? EV_HIT : EV_MAGIC_STRIKE;

  // use the default colour unless we're transmuting the elemental type
  int16 Color = e.EMagic ? e.EMagic->cval : 1 + random(15);
  if ((e.MM & MM_TRANSMUTE) || !e.EMagic || e.EMagic->aval == AR_BREATH)
    switch(e.DType) {
      case AD_ACID: Color = GREEN; break;
      case AD_COLD: Color = SKYBLUE; break;
      case AD_DISN: Color = EMERALD; break;
      case AD_DREX: case AD_DRST: case AD_DRCO: Color = SHADOW; break;
      case AD_ELEC: Color = YELLOW; break;
      case AD_FIRE: Color = RED; break;
      case AD_SONI: Color = WHITE; break;
      case AD_SUNL: Color = YELLOW; break;
      case AD_TOXI: Color = PURPLE; break;
    }

  // glyph (colour set below) 
  int16 g = 15 ;

  // where do we start? 
  int16 sx, sy, cx, cy; 
  sx=cx=e.EActor->x*2;
  sy=cy=e.EActor->y*2;

  int8 dirX = 0, dirY = 0;
  // if we're not a direction, keep track of slopes
  bool AdjustDone = false;
  Fraction slope, test;

  // do we think something happened? 
  bool vis = true; 
    if (!e.EActor->isBlind())
    e.isSomething = true; 

  // minimum range for wand beams ...
  if (isBeam && e.EItem && 
      (e.EItem->isType(T_WAND) || e.EItem->isType(T_POTION)))
    e.vRange = max(5,e.vRange);
  
  // map overlay 
  Map &m = *(e.EActor->m);

  Overlay &o = e.EActor->m->ov;
  o.Activate();

  int distSoFar = 0;

  Thing * possTarget = NULL;
  bool hitSelf = false;
  
  wasEngulfed = false;
  if (e.EActor->HasStati(ENGULFED))
    {
      if (isBall)
        {
          ADD_TARGET(e.EActor);
          ADD_TARGET(e.EActor->GetStatiObj(ENGULFED));
        }
      else if ((e.isLoc && (e.EXVal == e.EActor->x && e.EYVal == e.EActor->y)) || 
               (e.isDir && (e.EDir == CENTER)) || 
               (!e.isDir && !e.isLoc && e.EActor == e.EVictim))
        {
          ADD_TARGET(e.EActor);
        }
      else {
        ADD_TARGET(e.EActor->GetStatiObj(ENGULFED));
        }
      wasEngulfed = true;
      goto DoHits;
    }
  
  
  // if you're intentionally striking yourself, skip the travel phase
  if ((e.isLoc && (e.EXVal == e.EActor->x && e.EYVal == e.EActor->y)) || 
      (e.isDir && (e.EDir == CENTER)) || 
      (!e.isDir && !e.isLoc && e.EActor == e.EVictim))
  {
    ADD_TARGET(e.EActor);
    hitSelf = true;
    EventInfo eCopy;
    eCopy = e; eCopy.isLoc = true;
    eCopy.EXVal = e.EActor->x; 
    eCopy.EYVal = e.EActor->y;
    MagicXY(eCopy,e.EActor->x,e.EActor->y);
    goto DoneProject; 
  }
  
    if (e.EVictim && !e.isDir)
      if (e.EActor->x == e.EVictim->x &&
          e.EActor->y == e.EVictim->y)
        {
          ADD_TARGET(e.EVictim);
          goto DoneProject;
        }
  
  if (!isProject)
    { if (dist(e.EActor->x,e.EActor->y,e.EXVal,e.EYVal) > e.vRange)
        return ABORT;
      sx=cx=e.EXVal*2;
      sy=cy=e.EYVal*2;
      goto DoneProject; }
  
   
   NextChain:

   if (e.EVictim)
     ArcList[ac++] = e.EVictim;

  // where are we going? 
  // if (!e.isLoc && !e.isDir) {
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


  // travel! 
    while(!hitSelf && m.InBounds(cx/2,cy/2)) {

    // are we out of range? 
    if (distSoFar++ >= e.vRange)
      break; 

      // features are things like doors and gravestones
      for (ft=m.FFeatureAt(cx/2,cy/2);ft;ft=m.NFeatureAt(cx/2,cy/2))
        if (ft->Flags & F_SOLID)
          {
            ADD_TARGET(ft);
            if (WarpCount)
              WarpCount--;
            else
              goto DoneProject;
          }

    // can we pass through this solid wall? 
      if (m.At(cx/2,cy/2).Solid) {
      // i.e., wand of fire vs. wall of ice
      eCopy = e; eCopy.isLoc = true;
      eCopy.EXVal = cx / 2; 
      eCopy.EYVal = cy / 2;
      MagicXY(eCopy,cx/2,cy/2); 
      if (m.At(cx/2,cy/2).Solid) {
        if (WarpCount)
          WarpCount--;
        else
          break;
        }
      if (!isMulti)
        break;
      }

    // draw something on this square? 
    if (m.At(cx/2,cy/2).Visibility & VI_DEFINED)
      vis = true;


    if (vis) {
      int16 colour; 
      if (isBeam) 
        colour = (BlastColourSets[(Color * 4)+random(4)] << 8);
      else 
        colour = (BlastColourSets[Color * 4] << 8);
      o.AddGlyph(cx/2,cy/2, g | colour);
      o.ShowGlyphs();
      PossiblyPause(T1,cx/2,cy/2,35);
      if (!isBeam) {
        o.DeActivate();
        o.Activate();
        }
    }

    {
      EventInfo eCopy;
      eCopy = e; eCopy.isLoc = true;
      eCopy.EXVal = cx / 2; 
      eCopy.EYVal = cy / 2;
      MagicXY(eCopy,cx/2,cy/2); 
    }
    // move to the next spot! 
    if (e.isDir) {
      cx += DirX[e.EDir]*2;
      cy += DirY[e.EDir]*2;
      // beams get special pretty glyphs
      if (isBeam) switch (e.EDir) {
        case NORTH: case SOUTH: g='|'; break;
        case EAST: case WEST: g='-'; break;
        case NORTHEAST: case SOUTHWEST: g='/'; break;
        case NORTHWEST: case SOUTHEAST: g='\\'; break;
        default: g='*';
      }
    }
    else if (!isBeam && (!AdjustDone) && dist(cx/2,cy/2,e.EXVal,e.EYVal)==1) {
      AdjustDone = true;
      cx = e.EXVal*2;
      cy = e.EYVal*2;        
    }
    else {
      // ww: FIXME: currently diagonal beams don't go as far as they should
      test.Set(abs((cy + dirY)-sy), abs((cx + dirX)-sx));
      if (test > slope) {
        cx += 2 * dirX;
        if (isBeam) g = '-'; 
      } else if (test < slope) {
        cy += 2 * dirY;
        if (isBeam) g = '|'; 
      } else {
        cx += 2 * dirX;
        cy += 2 * dirY;
        if (isBeam) {
          if ((dirX < 0 && dirY < 0) /* NE */ ||
              (dirX > 0 && dirY > 0) /* SW */)
            g = '\\';
          else
            g= '/';
        } 
      }
    }
    if (!m.InBounds(cx/2,cy/2))
      break;

    if (e.EVictim && e.EVictim->x==cx/2 &&
                     e.EVictim->y==cy/2)
      { ADD_TARGET(e.EVictim)
        if (isChain || !isBeam)
          break; }
    for(cr=m.FCreatureAt(cx/2,cy/2);cr;cr=m.NCreatureAt(cx/2,cy/2))
      if (cr!=e.EActor || e.vChainCount != 0)
        {
          if (cr != e.EVictim)
            { ADD_TARGET(cr); }
          if (!isMulti) goto OuterBreak; 
        } 

    // finally, if we aimed at *this specific spot* (e.g., to throw a
    // fireball in empty middle square between two enemies), stop here
      if (cx / 2 == e.EXVal && cy / 2 == e.EYVal && !isBeam) {
        for (ft=m.FFeatureAt(cx/2,cy/2);ft;ft=m.NFeatureAt(cx/2,cy/2)) {
          ADD_TARGET(ft);
          }
        break; 
        }
    }
  OuterBreak:

  if (isChain)
    {
      Thing *t, *best; int16 bdist;
      e.vChainCount++;
      if (e.vChainCount > e.vChainMax)
        goto DoneChain;
      
      bdist = 1000;
      for (t = m.FirstThing(); t; t = m.NextThing())
        {
          if (dist(cx/2,cy/2,t->x,t->y) > e.vRange)
            continue;
          
          /* Don't arc to doors and such */
          if (!(t->isItem() || t->isCreature()))
            continue;
          
          /* Don't arc to items unless we affect them */
          if (t->isItem() && !TEFF(e.eID)->HasFlag(EF_AFFECTS_ITEMS))
            continue;
          
          /* Don't arc to friends unless benevolent effect */
          if (t->isCreature())
            if (((Creature*)t)->isFriendlyTo(e.EActor))
             if (!TEFF(e.eID)->HasFlag(EF_NOTBAD))
                continue;
          
          /* Don't arc to same target twice */
          if (t == e.EVictim)
            continue;
          
          for (int16 i=0;i!=ac;i++)
            if (t == ArcList[i])
              { goto OuterContinue; }
              
          /* Need clear line of fire */
          if (!m.LineOfFire(cx/2,cy/2,t->x,t->y,e.EActor))
            continue;
          
          /* Choose the closest valid target to arc to; if there
             are multiple targets the same distance away, it should
             be random which one we arc to. */
          if ((dist(cx/2,cy/2,t->x,t->y) < bdist) || 
              (dist(cx/2,cy/2,t->x,t->y) == bdist && !random(2)))
            {  
              best = t;
              bdist = dist(cx/2,cy/2,t->x,t->y);
            }   
          
          OuterContinue:;
        }
        
      if (bdist != 1000)
        {
          distSoFar = 0;
          sx = cx;
          sy = cy;
          e.ETarget = best;
          e.EXVal = best->x;
          e.EYVal = best->y;
          e.isDir = false;
          ti = max(0,tc-1);
          goto NextChain;
        }
    }
  DoneChain:
  
  DoneProject:

  

  if (isBall) { 
    int x = cx / 2;
    int y = cy / 2; 
    int i = 0; 

    m.SetQueue(QUEUE_AREA_EFFECT);

    int16 ListX[60],   ListY[60],
          OListX[60], OListY[60], 
          szList, szOList,
          GlyphCount,MinGlyphs;

    OListX[0] = x; OListY[0] = y;
    szOList = 1; szList = 0;
    GlyphCount = 0; 
    MinGlyphs = e.MM & MM_ENLARGE ? 20*e.EMagic->lval : 
                e.MM & MM_FOCUS   ?    e.EMagic->lval :
                                     3*e.EMagic->lval;



    int16 wesCount = 0; // no infinite loops
    while (GlyphCount < MinGlyphs && wesCount++ < 5000) {
      for(i=0;i!=szOList;i++)
        for (j=0;j!=4;j++) {
          vis = PossiblyPause(T1,OListX[i] + DirX[j],OListY[i] + DirY[j],0); 
          if (m.InBounds(OListX[i] + DirX[j], OListY[i] + DirY[j]) && 
                  !o.IsGlyphAt(OListX[i] + DirX[j],OListY[i] + DirY[j])) {                          
            eCopy = e; eCopy.isLoc = true;
            eCopy.EXVal = OListX[i] + DirX[j]; 
            eCopy.EYVal = OListY[i] + DirY[j]; 
            MagicXY(eCopy,eCopy.EXVal,eCopy.EYVal);
            if (!m.At(OListX[i] + DirX[j],OListY[i] + DirY[j]).Solid)                          
              {     
                NotSolidAfterAll:                                                             
                for (ft=m.FFeatureAt(OListX[i] + DirX[j],OListY[i] + DirY[j]);ft;
                        ft=m.NFeatureAt(OListX[i] + DirX[j],OListY[i] + DirY[j])) {                    
                  ADD_TARGET(ft);            
                  if (ft->Flags & F_SOLID)
                    goto lt;                                                   
                  }                                                              
                for (Thing *t=m.FirstAt(OListX[i] + DirX[j],OListY[i] + DirY[j]);
                          t;t=m.NextAt(OListX[i] + DirX[j],OListY[i] + DirY[j]))                   
                  { ADD_TARGET(t); }         
                ListX[szList] = OListX[i] + DirX[j];                                    
                ListY[szList] = OListY[i] + DirY[j];                                    
                if (vis)                                                         
                  o.AddGlyph(ListX[szList],ListY[szList],                        
                    15 | (BlastColourSets[Color*4+1+random(3)] << 8));            
                szList++; GlyphCount++;                                          
                if (szList > 58)                                                 
                  goto Breakout;                                                 
                lt:;                                                             
              }
            }
          }




      o.ShowGlyphs();
      if (vis)
        T1->StopWatch(70);
      /*if (GlyphCount < MinGlyphs) {
        for(i=0;i!=szOList;i++)
          {
            APPLY_GLYPH_AT(+1,+1,EEE)
            APPLY_GLYPH_AT(+1,-1,FFF)
            APPLY_GLYPH_AT(-1,+1,GGG)
            APPLY_GLYPH_AT(-1,-1,HHH)
          }
        o.ShowGlyphs();
        }*/
      memcpy(OListX,ListX,sizeof(OListX));
      memcpy(OListY,ListY,sizeof(OListY));
      szOList = szList; szList = 0;
      }
    Breakout:
    if (vis)
      T1->StopWatch(300);
    m.UnsetQueue(QUEUE_AREA_EFFECT);
    m.PrintQueue(QUEUE_AREA_EFFECT);
  } else if (!isBeam) {
    // a bolt terraforming spell, for example
    eCopy = e; eCopy.isLoc = true;
    eCopy.EXVal = cx / 2; 
    eCopy.EYVal = cy / 2; 
    MagicXY(eCopy,eCopy.EXVal,eCopy.EYVal);
    } 

  DoHits:
  for (j=0;j!=tc;j++)
    {
      Creature *cr;
      eCopy = e;
      eCopy.ETarget = TargetList[j];
      eCopy.EXVal = TargetList[j]->x;
      eCopy.EYVal = TargetList[j]->y;
      cr = (Creature*) TargetList[j];
      if (e.EMagic && (e.EMagic->aval == AR_BOLT || e.EMagic->aval == AR_RAY))
        if (cr->isCreature() && cr->HasFeat(FT_DEFLECT_MAGIC))
          if (cr->AoO && cr->Perceives(e.EActor) && !cr->isFlatFooted())
            if (cr->SavingThrow(REF,15))
              {
                cr->AoO--;
                TPrint(eCopy,"The <EVictim> deflects your <9><Res><7>!",
                    "You deflect the <9><Res><7>!",
                    "The <EVictim> deflects the <9><Res><7>!",e.eID);
                continue;
              }
      
      ReThrow(ourEvent, eCopy);
    }
    
  if (wasEngulfed && !e.EActor->HasStati(ENGULFED)) {
    wasEngulfed = false;
    e.EVictim = false;
    goto NextChain;
    }

  // now we're done projecting
  o.DeActivate();
  e.EActor->MakeNoise(10);
  e.doVUpdate = true; 
  return DONE;
}

void Magic::PredictVictimsOfBallBeamBolt(EventInfo &e, 
  bool isBeam, bool isBall, bool isChain, Creature * victim[], int16 & numVic)
{
  Feature *ft; Creature *cr;
  TEffect *te = NULL;
  if (e.eID) te = TEFF(e.eID);

  numVic = 0; 

  // ww: WarpCount 
  int16 WarpCount;
  if (e.MM & MM_WARP)
    WarpCount = max(2,3+e.EActor->Mod(A_INT));
  else
    WarpCount = 0;

  // where do we start? 
  int16 sx, sy, cx, cy; 
  sx=cx=e.EActor->x*2;
  sy=cy=e.EActor->y*2;

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

  // minimum range for wand beams ...
  if (isBeam && e.EItem && 
      (e.EItem->isType(T_WAND) || e.EItem->isType(T_POTION)))
    e.vRange = max(5,e.vRange);

  // map overlay 
  Map &m = *(e.EActor->m);
  Overlay &o = e.EActor->m->ov;
  o.Activate();

  int distSoFar = 0;

  Thing * possTarget = NULL;
  bool hitSelf = false;
  // if you're intentionally striking yourself, skip the travel phase
  if ((e.isLoc && (e.EXVal == e.EActor->x && e.EYVal == e.EActor->y)) || 
      (e.isDir && (e.EDir == CENTER)) || 
      (!e.isDir && !e.isLoc && e.EActor == e.EVictim))
  {
    victim[numVic++] = e.EActor;
    hitSelf = true; 
    goto DoneProject; 
  }

  // travel! 
    while(!hitSelf && m.InBounds(cx/2,cy/2) 
         // && (!m.At(cx/2,cy/2).Solid || WarpCount)
          ) {

    // are we out of range on a beam? 
    if (isBeam && distSoFar++ >= e.vRange)
      break; 

      // features are things like doors and gravestones
      for (ft=m.FFeatureAt(cx/2,cy/2);ft;ft=m.NFeatureAt(cx/2,cy/2))
        if (ft->Flags & F_SOLID)
          {
            if (WarpCount)
              WarpCount--;
            else
              goto OuterBreak;
          }

      // can we pass through this solid wall? 
      else if (m.At(cx/2,cy/2).Solid) {
      if (WarpCount)
        WarpCount--;
      else
        break;
    }

    // move to the next spot! 
    if (e.isDir) {
      cx += DirX[e.EDir]*2;
      cy += DirY[e.EDir]*2;
      // beams get special pretty glyphs
      }
    else if (!isBeam && (!AdjustDone) && dist(cx/2,cy/2,e.EXVal,e.EYVal)==1) {
      AdjustDone = true;
      cx = e.EXVal*2;
      cy = e.EYVal*2;        
      }
    else {
      // ww: FIXME: currently diagonal beams don't go as far as they should
      test.Set(abs((cy + dirY)-sy), abs((cx + dirX)-sx));
      if (test > slope) {
        cx += 2 * dirX;
      } else if (test < slope) {
        cy += 2 * dirY;
      } else {
        cx += 2 * dirX;
        cy += 2 * dirY;
        }
      }
    if (!m.InBounds(cx/2,cy/2))
      break;


    if (e.EVictim && e.EVictim->x==cx/2 &&
                     e.EVictim->y==cy/2)
      { 
        victim[numVic++] = e.EVictim; 
        if (!isBeam)
          break;
      }
    
    for(cr=m.FCreatureAt(cx/2,cy/2);cr;cr=m.NCreatureAt(cx/2,cy/2))
      if (cr!=e.EActor)
        {
          possTarget = cr;
          if (e.EVictim != cr)
            victim[numVic++] = cr;
          if (!isBeam) 
            goto OuterBreak; 
        } 

    if (cx / 2 == e.EXVal && cy / 2 == e.EYVal && !isBeam)
      break; 
  }
  OuterBreak:
  
  /* TODO: Add Chain Predict code */
  
  DoneProject:

  if (isBall) { 
    int x = cx / 2;
    int y = cy / 2; 
    int i = 0, j; 

    int16 ListX[60],   ListY[60],
          OListX[60], OListY[60], 
          szList, szOList,
          GlyphCount,MinGlyphs;

    OListX[0] = x; OListY[0] = y;
    szOList = 1; szList = 0;
    GlyphCount = 0; 
    MinGlyphs = e.MM & MM_ENLARGE ? 10*e.vRadius : 
                e.MM & MM_FOCUS   ?    e.vRadius :
                                     3*e.vRadius;

    int16 wesCount = 0;  // no infinite loops!
    while (GlyphCount < MinGlyphs && wesCount++ < 5000) {
      for(i=0;i!=szOList;i++)
        for (j=0;j!=4;j++)
          {
            if (m.InBounds(OListX[i] + DirX[j],OListY[i] + DirY[j]))                             
              if (!m.At(OListX[i] + DirX[j],OListY[i] + DirY[j]).Solid)                          
                {                                                                  
                  for (ft=m.FFeatureAt(OListX[i] + DirX[j],OListY[i] + DirY[j]);ft;
                       ft=m.NFeatureAt(OListX[i] + DirX[j],OListY[i] + DirY[j])) {                    
                    if (ft->Flags & F_SOLID)
                        goto lt;                                                   
                    }                                                              
                  for (Thing *t=m.FirstAt(OListX[i] + DirX[j],OListY[i] + DirY[j]);t;
                              t=m.NextAt(OListX[i] + DirX[j],OListY[i] + DirY[j]))                   
                    { if (t->isCreature()) 
                        victim[numVic++] = (Creature *)t; } 
                  ListX[szList] = OListX[i] + DirX[j];                                    
                  ListY[szList] = OListY[i] + DirY[j];                                    
                  szList++; GlyphCount++; 
                  if (szList > 58)
                    goto Breakout;                                         
                  lt:;                                                             
                }
          }
      memcpy(OListX,ListX,sizeof(OListX));
      memcpy(OListY,ListY,sizeof(OListY));
      szOList = szList; szList = 0;
      }
    Breakout:;
  } 

  // now we're done projecting
  o.DeActivate();
  return; 
}

                      
EvReturn Magic::ATouch(EventInfo &e)
  {
    if (e.MM & MM_PROJECT) {
      e.vRange = 6;
      //return ABeam(e);
      return ABallBeamBolt(e);
      }

    /* Set up the touch attack Stati */
    e.EActor->RemoveStati(TOUCH_ATTACK);
    e.EActor->GainTempStati(TOUCH_ATTACK,e.EActor,e.vDuration,SS_ENCH,0,
      LevelAdjust(e.EMagic->lval,e.vCasterLev, (e.EItem && e.EItem->isItem()) ? e.EItem->GetPlus() : 0),e.eID);   
    
    /* Skip over all the effect components with AR_TOUCH */
    while(TEFF(e.eID)->Vals(e.efNum+1) &&
      TEFF(e.eID)->Vals(e.efNum+1)->aval == AR_TOUCH)
      e.efNum++;
    return DONE;
}

EvReturn Creature::Cast(EventInfo &e) {
    int16 k, mCost, dmgMod, rating, dmg_pen, roll; 
    EvReturn res; Item *it; bool dmgFail, wasThreatened;
    int16 i, j, oHP, fc, sc, sr, cpen; Creature *c; String MMStr;
    int16 castingTimeout = 
        3000 / max(25,100+10*(1 + e.EActor->Mod(A_INT) -
        TEFF(e.eID)->Level));
    int16 specMod; 
    Counterspeller csp[64]; int16 cc; EvReturn csr;

    dmgFail = false;
    roll = 1;
    rating = dmg_pen = 0;

    e.sp = theGame->SpellNum(e.eID);

    if (isCharacter())
        if (thisc->Spells[e.sp] & SP_INNATE)
            if (!e.MM)
                return ReThrow(EV_INVOKE,e);

    if (isMType(MA_ELF) && (TEFF(e.eID)->Schools & SC_NEC) && !(TEFF(e.eID)->Schools & SC_ABJ)) {
        IPrint("You can't cast necromatic spells.");
        return ABORT;
    } 

    if (HasStati(PARALYSIS))
        if (!(e.MM & MM_STILL) || !(e.MM & MM_VOCALIZE)) {
            IPrint("You can only cast still, vocalized spells while paralyzed.");
            return ABORT;
        }

    if (HasStati(POLYMORPH))
        if (HasMFlag(M_NOHANDS))
            if (!(e.MM & MM_STILL)) {
                IPrint("You cannot perform arcane gestures without hands!");
                return ABORT;
            }

    if (e.EVictim && e.EVictim->HasEffStati(-1,e.eID) && 
        (TEFF(e.eID)->HasFlag(EF_PERSISTANT) || TEFF(e.eID)->HasFlag(
        EF_PERMANANT) || TEFF(e.eID)->HasFlag(EF_CURSE)) &&
        e.eID != FIND("Bestow Curse")) {
        IPrint("Since the <Obj> already has <9><Res><7> cast on them, "
            "a second casting would have no effect.", e.EVictim, e.eID);
        return ABORT;
    }

    if (TEFF(e.eID)->HasFlag(EF_PERSISTANT) || TEFF(e.eID)->HasFlag(EF_PERMANANT))
        for(j=0;m->Fields[j];j++)
            if (m->Fields[j]->eID == e.eID && m->Fields[j]->Creator == myHandle)
                if (m->Fields[j]->FType & FI_MOBILE) {
                    IPrint("You already have <9><Res><7> cast on you, "
                        "a second casting would have no effect.", e.eID);
                    return ABORT;
                }

    if (isCharacter())
        if (thisc->Spells[e.sp] & SP_STAFF)
            e.isStaffSpell = true;

    e.isSpell = true;

    // ww: spellcasting requires real concentration! you can still
    // invoke innate abilities, though ...

    /* fjm: staff spells can be used in these conditions... */
    if (!e.isStaffSpell) {
        if (HasStati(RAGING)) {
            IPrint("You can't cast spells while you are in a killing rage!");
            return ABORT;
        } else if (HasStati(SPRINTING) || HasStati(TUMBLING)) {
            IPrint("You can't cast spells while you are sprinting or tumbling!");
            return ABORT;
        } else if (HasStati(CHARGING))  {
            if (HasStati(AUTO_CHARGE))
                RemoveStati(CHARGING);
            else {
                IPrint("You can't cast spells while charging!");
                return ABORT;
            }
        } else if (HasStati(ENRAGED))  {
            IPrint("You can't cast spells while enraged!");
            return ABORT;
        }
    }

    if (e.isStaffSpell)
        if (!(thisc->Spells[e.sp] & SP_KNOWN)) {
            int16 fc = GetStaffFatigueCost(e.eID);
            if (!LoseFatigue(fc,true))
                return ABORT;
        }

    if (e.sp == -1) {
        Error("Creature::Cast -- eID is not a valid spell!");
        return ABORT;
    }

    if (isPlayer())
        e.MM = MMFeats(e.sp);
    else
        e.MM = thism->SetMetamagic(e.eID,e.ETarget,TEFF(e.eID)->Purpose);

    /* ww: are stuck in no-cast webbing or somesuch? */
    if (TTER(m->TerrainAt(x,y))->Event(e,m->TerrainAt(x,y)) == ABORT)
        return ABORT; 

    /* Silence DOES block staff-spells, though. */
    if (inField(FI_SILENCE) && !(e.MM & MM_VOCALIZE)) {
        IPrint("You can't cast spells in areas of magical silence.");
        return ABORT;
    }

    if (isPlayer())    
        if (InSlot(SL_WEAPON) && InSlot(SL_READY) && !(e.MM & MM_STILL))
            if (InSlot(SL_WEAPON) != InSlot(SL_READY)) {
                /* One might wonder why mages can cast spells while wielding
                a two-handed weapon, but not while wielding two weapons,
                or a weapon and a shield. The in-game reason is that with
                a 2H weapon, you can /hold/ (as opposed to fight with) it
                with one hand long enough to cast, whereas two weapons
                keep both hands continually occupied. The real reason, 
                though, is that the quarterstaff -- the most traditional
                mage weapon -- is two-handed, and would otherwise be
                nearly unusable by mages. */
                // ww: however, this just ends up being annoying: the player
                // can waste wall-clock time swapping the weapon out, casting
                // the spell and putting it back. Just adding the timeout here
                // is a very close approximation. 
                if (!HasFeat(FT_QUICK_DRAW) && !(e.MM & MM_STILL)) {
                    IPrint("You need a hand free for somatic components, so you stow something, cast the spell, and then put it back: this takes extra time.");
                    castingTimeout += 3000 / (100 + 10 * Mod(A_DEX));
                } 
            }

    if (!isCharacter())
        goto HasComponent;

    if (e.isStaffSpell)
        goto HasComponent;

    if (e.MM & MM_INHERANT)
        goto HasComponent;

    if (thisp->Spells[e.sp] & (SP_PRIMAL|SP_BARDIC|SP_STAFF|SP_SORCERY))
        goto HasComponent;

    if (thisp->Spells[e.sp] & (SP_DIVINE|SP_DOMAIN)) {
        int okSlots[] = { SL_READY, SL_WEAPON, SL_AMULET, SL_ARMOUR, 0};
        Item *it;
        for (i=0;okSlots[i];i++) {
            if ((it=InSlot(okSlots[i])) && it->eID && 
                !strncmp(NAME(it->eID),NAME(thisc->GodID),
                strlen(NAME(thisc->GodID))))
                goto HasComponent;
            if ((it=InSlot(okSlots[i])) && (it->isType(T_ARMOUR) ||
                it->isType(T_SHIELD)) && it->HasQuality(AQ_GRAVEN))
                goto HasComponent;
        } 
    } 

    if (thisp->Spells[e.sp] & SP_ARCANE) {
        Item *bestBook;
        int16 bestPercent, per;

        if (HasStati(LUCUBRATION))
            if (GetStatiVal(LUCUBRATION) >= TEFF(e.eID)->Level)
                goto HasComponent;

        bestPercent = 0;
        bestBook = NULL;
        for (it = FirstInv(); it ;it=NextInv())
            if (it->HasSpell(e.sp)) {
                per = (it->GetHP()*100) / it->MaxHP();
                if (!bestBook || per > bestPercent) {
                    bestBook = it;
                    bestPercent = per;
                }
            }
            if (bestBook) {
                if (bestPercent < 100)
                    if (!SkillCheck(SK_DECIPHER,10+bestPercent/10,true))
                        dmgFail = true;
                goto HasComponent;
            }
    }

    if (thisp->Spells[e.sp] & (SP_DIVINE|SP_DOMAIN))
        IPrint("You need to have a holy symbol equipped to cast divine spells.");
    else
        IPrint("You don't have a spellbook with that spell currently in your inventory.");
    return ABORT;

HasComponent:

    if (isCharacter() && thisp->Spells[e.sp] & (SP_DIVINE|SP_DOMAIN)) {
        if (!thisp->GodID) {
            IPrint("You must have a god to cast spells.");
            return ABORT;
        }
        if (!thisp->isWorthyOf(thisp->GodID,false)) {
            IPrint("<Res> has revoked your access to <str> divine magic.",
                thisp->GodID, GodPronoun(thisp->GodID, true));
            return ABORT;
        }
    }

    if (HasStati(SINGING) && !(e.MM & MM_VOCALIZE)) {
        if (yn("Stop singing?",true))
            RemoveStati(SINGING);
        else
            return ABORT;
    }
    if (HasStati(ELEVATED) && !(e.MM & MM_STILL)) {
        if (!SkillCheck(SK_CLIMB, (GetStatiVal(ELEVATED) == ELEV_CEILING) ? 25: 15)) {
            IPrint("You try to perform arcane gestures from your perch, but "
                "instead slip and fall!");
            ClimbFall();
            goto SpellFails;
        }
    } else if (HasStati(LEVITATION) && !(e.MM & MM_STILL)) {
        if (!SkillCheck(SK_BALANCE, 7 + TEFF(e.eID)->Level*3)) {
            IDPrint("You are unable to perform the gestures correctly without "
                "solid footing!", "The <Obj> is unable to perform magical gestures "
                "successfully without solid footing!", this);
            goto SpellFails;
        }
    }

    if (!(e.MM & MM_VOCALIZE)) {
        Creature *cr; i;
        /* Optimization -- only PCs sing! 
        MapIterate(m,cr,i)
        if (cr->isCreature())
        */
        cr = theGame->GetPlayer(0);
        if (cr && cr->HasStati(SINGING))
            if (cr->GetStatiVal(SINGING) == BARD_SPELLBREAK)
                if (cr->DistFrom(this) <= cr->SkillLevel(SK_PERFORM)+2) {
                    int16 bardCheck, casterCheck;
                    SkillCheck(SK_CONCENT,0,true);
                    casterCheck = LastSkillCheckResult;
                    cr->SkillCheck(SK_PERFORM,0,true);
                    bardCheck = LastSkillCheckResult;
                    if (casterCheck >= bardCheck) {
                        IDPrint("You manage to cast correctly despite the "
                            "distracting chant.", "The <Obj> manages to cast "
                            "correctly despite the distracting chant.", this);
                    } else if (SavingThrow(WILL,10+cr->AbilityLevel(CA_BARDIC_MUSIC)/2
                        +cr->Mod(A_CHA), SA_CONF)) {
                            IDPrint("The chanting distracts you, spoiling your spell!",
                                "The chanting distracts the <Obj>, spoiling <his:Obj1> "
                                "spell!", this);
                            goto SpellFails;
                    } else {
                        IDPrint("The chanting confuses you utterly as you try to cast!",
                            "The chanting confuses the <Obj> utterly as <he:Obj1> tries "
                            "to cast!", this);
                        GainTempStati(CONFUSED,NULL,Dice::Roll(2,4),SS_ATTK,0,0);
                        goto SpellFails;
                    }
                }
    }

    mCost = getSpellMana(e.eID,e.MM,&specMod);

    if (e.MM & MM_QUICKEN) 
        castingTimeout /= 2; 

    /* Lower Mana, Test Spell Failure, etc.
    The actual effects of the spell are handled
    by the Magic class. */
    if ((cMana() < mCost) && !(Type == T_PLAYER && thisp->Opt(OPT_INF_MANA))) {
        IPrint("You don't have enough mana.");
        if (Type == T_PLAYER && cMana() >= (mCost / 2))
            if (thisp->MyTerm->yn("Attempt spell anyway?")) {
                /* take damage */
                LoseMana(mCost);
                if (random(25)+80 > SpellRating(e.eID))
                    goto SpellFails;
                goto ContinueCasting;
            }
        return ABORT;
    }
ContinueCasting:

    if (e.MM && e.EActor->HasSkill(SK_METAMAGIC)) {
        fc = MMFeatLevels(e.MM);
        /* Since we're reducing skill inflation, let's make this a
        tad bit easier. DC 20 -> 10. */
        e.EActor->SkillCheck(SK_METAMAGIC,10,true,false); 
        sc = LastSkillCheckResult - 10;
        if (sc > 0) { 
            fc -= 1 + (sc / 5); 
            if (fc < 0) fc = 0; 
        } 
        if (fc) {
            e.EActor->IPrint(Format("Metamagic Fatigue Cost: %d (out of %d).",fc,
                MMFeatLevels(e.MM)));
            if (!LoseFatigue(fc,true)) return ABORT;
        }
    }

    if (specMod < 0)
        if (random(100) < abs(specMod*2)) {
            int16 sc = GetStatiVal(SPECIALTY_SCHOOL), worst, mod, i;
            mod = 50; worst = 0;
            for (i=0;i!=9;i++)
                if (TEFF(e.eID)->Schools & XBIT(i))
                    if (mod > SpecialistTable[sc][i]) {
                        mod = SpecialistTable[sc][i];
                        worst = i;
                    }
            e.EActor->IPrint("Lacking aptitude for <Str>, you find casting <9><Str2><7> draining.",
                Lookup(SchoolNames,XBIT(worst)),NAME(e.eID));
            LoseFatigue(1,false); 
            if (HasStati(ASLEEP))
                return DONE;
        }

    if (HasStati(POLYMORPH) && !(e.MM & MM_STILL)) {
        e.EActor->IPrint("Arcane gestures in this strange form take you extra time.");
        castingTimeout += 15;
    }
    if (HasStati(ELEVATED) && (GetStatiVal(ELEVATED) == ELEV_CEILING)) {
        e.EActor->IPrint("Your awkward perch causes casting to take extra time.");
        castingTimeout += 15;
    }
    if (HasStati(MOUNTED) && !(e.MM & MM_STILL) && SkillLevel(SK_BALANCE) < 20) {
        e.EActor->IPrint("Casting on a moving mount takes you extra time.");
        castingTimeout += 15;
    }

    if (Type != T_PLAYER || !thisp->Opt(OPT_INF_MANA)) 
        LoseMana(mCost,TEFF(e.eID)->HasFlag(EF_LOSEMANA));

    MMStr = "";
    if (e.MM & MM_QUICKEN)
        MMStr += "quickened ";
    if (e.MM & MM_MAXIMIZE)
        MMStr += "maximized ";
    if (e.MM & MM_EMPOWER)
        MMStr += "empowered ";
    if (e.MM & MM_STILL)
        MMStr += "stilled ";
    if (e.MM & MM_VOCALIZE)
        MMStr += "vocalized ";
    if (e.MM & MM_TRANSMUTE)
        MMStr += "transmuted ";
    if (e.MM & MM_CONSECRATE)
        MMStr += "consecrated ";

    cc = 0;
    MapIterate(m,c,i)
        if (c->isCreature() && c != this)
            if (c->Perceives(this)) {
                if (c->HasSkill(SK_SPELLCRAFT) && c->SkillCheck(SK_SPELLCRAFT,10+TEFF(e.eID)->Level,true)) {
                    c->IPrint("The <Obj> begins casting <Str><Str><5><Res><7><Str>.",this,
                        MMStr.GetLength() ? "a ":"", (const char*)MMStr,e.eID,
                        e.MM & MM_DEFENSIVE ? " defensively" : "");
                    if (cc < 62) {
                        csp[cc].Check = LastSkillCheckResult;
                        csp[cc].cr = c;
                        cc++;
                    }
                } else
                    c->IPrint("The <Obj> begins casting a spell<Str>.",this,
                    e.MM & MM_DEFENSIVE ? "defensively " : "");
            }

    csp[cc].cr = NULL;
    csp[cc].Check = 0;

    oHP = cHP;

    if (!(e.MM & MM_VOCALIZE))
        e.EActor->MakeNoise(12);

    StateFlags |= MS_CASTING;
    if (e.MM & MM_STILL)
        StateFlags |= MS_STILL_CAST;
    if (isPlayer())
        thisp->statiChanged = true;

    if (!(e.MM & MM_DEFENSIVE || TEFF(e.eID)->HasFlag(EF_DEFENSIVE)))
        e.EActor->ProvokeAoO();
    if (e.EActor->isDead()) {
        StateFlags &= ~(MS_CASTING|MS_STILL_CAST);
        return ABORT;
    }

    if (e.EActor->HasStati(INVIS))
        if (!(e.EActor->GetStatiVal(INVIS) == INV_IMPROVED)) 
            if ((TEFF(e.eID)->Purpose & (EP_ATTACK|EP_CURSE|EP_SUMMON)) ||
                !((e.EVictim && e.EVictim->isCreature() && e.EVictim->isFriendlyTo(e.EActor)) || TEFF(e.eID)->HasFlag(EF_NOTBAD)))
                RemoveStati(INVIS);
    StatiIterNature(e.EActor,INVIS_TO)
        if (S->Val != INV_IMPROVED)
            if ((TEFF(e.eID)->Purpose & (EP_ATTACK|EP_CURSE|EP_SUMMON)) ||
                !((e.EVictim && e.EVictim->isCreature() && e.EVictim->isFriendlyTo(e.EActor)) || TEFF(e.eID)->HasFlag(EF_NOTBAD)))
                StatiIter_RemoveCurrent(e.EActor);
    StatiIterEnd(e.EActor)     

    /* Get Spell Rating */
    rating = SpellRating(e.eID,e.MM);
    roll   = random(100)+1;

    dmg_pen = 0;
    if (oHP > cHP) {
        dmg_pen = ((oHP-cHP)*300) / (mHP+GetAttr(A_THP));
        dmg_pen = max(0,dmg_pen - ((ConcentBuffer() - (concentUsed + p_conc))*5));
        rating -= dmg_pen;
    }  

    if (dmgFail) {
        IPrint("You fail to evoke the blurred runes in your damaged "
            "spellbook properly!");
        goto SpellFails;
    }

    if (!(isPlayer() && thisp->Opt(OPT_NO_SPELLFAIL)))
        if (roll > rating) {
SpellFails:
            // ww: now combat casting and quicken stack -- before you might
            // have had the un-intuitive situation where a spell cast with
            // quicken takes 15 but a spell disrupted with quicken takes 30!
            if (HasFeat(FT_COMBAT_CASTING))
                castingTimeout /= 2; 
            Timeout += castingTimeout; 

            if (roll <= rating + dmg_pen)
                IDPrint("Your spell is disrupted!",
                "The <Obj>'s spell is disrupted!", this);
            else {
                switch(random(3)) {
                case 0:
                    IDPrint("The spell fizzles.","The casting fails.",this);
                    break;
                case 1:
                    IDPrint("You fail to get the spell off.","The spell fails.", this);
                    break;
                case 2:
                    IDPrint("The magic fails you.","The casting fails.",this);
                    break;
                }
                Abuse(A_INT,1);
            }
            if (random(20) == 3) {
                IPrint("The incomplete spell drains your physical reserves.");
                LoseFatigue(1,false);
            } 
            return ABORT;
        }

    /* Set timeout for spellcasting */
    Timeout += castingTimeout; 

    csr = Counterspell(e,csp);
    if (csr == ABORT)
        return DONE;

    /* Spell Disruption Check */

    if (TEFF(e.eID)->ef.eval == EA_HEALING)
        if (e.EActor->HasAbility(CA_MAXIMIZE_HEALING))
            if (e.EActor->AbilityLevel(CA_MAXIMIZE_HEALING) >= TEFF(e.eID)->Level)
                e.MM |= MM_MAXIMIZE;

    if (isPlayer() && csr == NOTHING && // ww: optimization, can be removed later
        (e.EActor == e.EVictim || 
        (!e.EVictim && TEFF(e.eID)->ef.qval == 0))) { 
            Status * s = GetStati(HAS_ANIMAL,-1,0);
            if (s) {
                Creature *c = oCreature(s->h);
                EventInfo eCopy = e;
                eCopy.EVictim = c;
                if (ReThrow(EV_EFFECT,eCopy) != ABORT) 
                    e.EActor->IPrint(Format("%s is affected through your special bond.", (const char*)c->Name(0)));
            } 
    } 

    if (isCharacter())
        wasThreatened = isThreatened();
    else
        wasThreatened = false;

    res = ReThrow(EV_EFFECT,e);

    if (e.MM && TEFF(e.eID)->HasFlag(EF_LOSEMANA))
        if (HasEffStati(-1,e.eID))
            GainTempStati(STORED_MM,
                GetEffStatiObj(-1,e.eID),
                GetEffStatiDur(-1,e.eID),
                SS_ENCH,
                e.MM & 0x0000FFFF,
                (e.MM & 0xFFFF0000) << 16,
                e.eID, GetEffStatiCLev(-1,e.eID));    

    if (isCharacter() && wasThreatened) {
        if (TEFF(e.eID)->BaseChance <= 30) // SP_VERY_HARD
            Exercise(A_INT,random(12)+1,EINT_CASTING,30);
        else if (TEFF(e.eID)->BaseChance <= 60) // SP_HAND
            Exercise(A_INT,random(4)+1,EINT_CASTING,30);

        if (e.MM)
            Exercise(A_INT,random(MMFeatLevels(e.MM)*2)+1,EINT_CASTING,45);
    }    

    return res;
}

EvReturn Creature::Invoke(EventInfo &e)
  {
    EvReturn res, csr;

    if (HasStati(NAUSEA)) {
      IPrint("You can't use innate abilities while you are nauseated!");
      return ABORT;
      }
    if (HasStati(NEGATED))
      {
        HandleNegate(e);
        return ABORT;
      }  

    if (e.EVictim && e.EVictim->HasEffStati(-1,e.eID))
      {
        IPrint("Since the <Obj> already has <9><Res><7> cast on them, "
          "a second invocation would have no effect.", e.EVictim, e.eID);
        return ABORT;
      }
       
    if (!TEFF(e.eID)->HasFlag(EF_SPECABIL))
      IDPrint(NULL,"An <Obj> concentrates.",this);
    
    if (isMType(MA_ELF) && (TEFF(e.eID)->Schools & SC_NEC)
                        && !(TEFF(e.eID)->Schools & SC_ABJ))
      {
        Timeout += 20;
        IPrint("Nothing happens.");
        return DONE;
      } 
       
    int32 mCost = (TEFF(e.eID)->ManaCost);    

    // ww: this used to be one third of the cost, but that made enemy
    // innate spellcasters *way too strong*
    if (!isCharacter() || 
        !(thisp->Spells[theGame->SpellNum(e.eID)] & SP_KNOWN)) {
      /* Only charge mana for innate abilities that we /don't/ also
         know as spells proper. */
      if (cMana() < mCost) {
        IPrint("You don't have enough mana.");
        return ABORT;
      }


    if (TEFF(e.eID)->Purpose & EP_BUFF && HasFeat(FT_MYSTIC_PREPARATION)) {
      mCost = max( (mCost-2), 1);
    }
    LoseMana(mCost,TEFF(e.eID)->HasFlag(EF_LOSEMANA));
    } 

    Timeout += 20;
    
    e.isSpell = true; // ww: I think we want this. Otherwise we don't 
    // get the wis damage bonus on an innate magic missile, etc. 
    
    if (!HasFeat(FT_GUARDED_INVOCATION) && !TEFF(e.eID)->HasFlag(EF_DEFENSIVE))
      e.EActor->ProvokeAoO();
    // ww: we weren't checking this before!
    if (e.EActor->isDead())
      return DONE; 

    csr = Counterspell(e,NULL);
    if (csr == ABORT)
      return DONE;

    if (isPlayer() && // ww: optimization, can be removed later
        (e.EActor == e.EVictim || 
         (!e.EVictim && TEFF(e.eID)->ef.qval == 0))) { 
      Status * s = GetStati(HAS_ANIMAL,-1,0);
      if (s) {
        Creature *c = oCreature(s->h);
        EventInfo eCopy = e;
        eCopy.EVictim = c;
        eCopy.isSharedSpell = true;
        if ( ReThrow(EV_EFFECT,eCopy) != ABORT) 
          e.EActor->IPrint(
            Format("%s is affected through your special bond.", (const char*)c->Name(0)));
      } 
    } 

    res = ReThrow(EV_EFFECT,e);
    
    return res;

  }
  
int SortBySpellcraftCheck(const void *a, const void *b) {
    return ((Counterspeller*)b)->Check - ((Counterspeller*)a)->Check;
} 
  
EvReturn Creature::Counterspell(EventInfo &e, Counterspeller *csp) {
    int16 cc, i, sn, DC, lv, mCost;
    bool isDispel;
    rID csID, xID, dispID;
    Counterspeller _csp[64];
    Creature *c;

    if (csp) {    
        cc = 0;
        while(csp[cc].cr)
            cc++;
    } else {
        cc = 0;
        csp = _csp;

        MapIterate(m,c,i)
            if (c->isCreature() && c != this)
                if (c->Perceives(this)) 
                    if (c->HasSkill(SK_SPELLCRAFT)) {
                        if (e.Event != EV_CAST && !c->HasFeat(FT_BROAD_COUNTERSPELL))
                            continue;

                        c->SkillCheck(SK_SPELLCRAFT,0,true);
                        if (cc < 62) {
                            csp[cc].Check = LastSkillCheckResult;
                            csp[cc].cr = c;
                            cc++;
                        }
                    }

        csp[cc].cr = NULL;
        csp[cc].Check = 0;
    }

    qsort(csp,cc,sizeof(Counterspeller),&SortBySpellcraftCheck);

    lv = TEFF(e.eID)->Level + (e.MM & MM_FORTIFY ? 3 : 0);
    dispID = FIND("dispel magic");

    for (i=0;csp[i].cr;i++) {
        if (!(csp[i].cr->isHostileTo(this)))
            continue;

        /* For now, monsters only counterspell 33% of the time.
        Why? Because they know ALL the spells they have the
        caster level to cast, which would be tremendously
        unfair to a PC mage -- and the messages would just
        serve to hilight this fudge. */
        if (random(3) && !csp[i].cr->isPlayer())
            continue;

        /* We need to recheck this below -- this is an optimization. */
        if (!(csp[i].Check > 15 + TEFF(e.eID)->Level))
            continue;

        csID = 0;
        isDispel = false;
        for (sn=0;sn!=theGame->LastSpell();sn++)
            if (csp[i].cr->getSpellFlags(xID = theGame->SpellID(sn))  & SP_KNOWN)
                if (TEFF(xID)->Level >= lv)
                    if (TEFF(xID)->Schools & TEFF(e.eID)->Schools & (SC_ABJ|SC_ARC|SC_DIV|SC_ENC|SC_EVO|SC_ILL|SC_NEC|SC_THA|SC_WEA))
                        if (csID == 0 || TEFF(xID)->ManaCost < TEFF(csID)->ManaCost) {
                            csID = xID; mCost = max(TEFF(csID)->ManaCost, TEFF(e.eID)->ManaCost);
                        }

        if (!csID)
            if ((csp[i].cr->getSpellFlags(dispID) & SP_KNOWN)) {
                isDispel = true; mCost = max(TEFF(dispID)->ManaCost, TEFF(e.eID)->ManaCost);
            }

        if (!csID)
            continue;   

        if (!(csp[i].Check > 15 + TEFF(e.eID)->Level + (isDispel ? 5 : 0)))
            continue;

        if (csp[i].cr->SkillLevel(SK_SPELLCRAFT) >= 25)
            mCost = mCost / 3;
        else if (csp[i].cr->SkillLevel(SK_SPELLCRAFT) >= 20)
            mCost = mCost / 2;
        else if (csp[i].cr->SkillLevel(SK_SPELLCRAFT) >= 15)
            mCost = (mCost * 2) / 3;

        switch (csp[i].cr->isPlayer() ? ((Player*)csp[i].cr)->Opt(OPT_COUNTERSPELL) : 3) {
        case 0:  // do not counterspell
            goto DoNotCounter;
        case 1:  // always counterspell
            goto DoCounterspell;
        case 2:  // ask player
            if (csp[i].cr->yn(XPrint("Best opt. is <9><Res><5> (mana <Num>). Timeout <Num>. Counter?", csID, mCost, Timeout)))
                goto DoCounterspell;
            else
                goto DoNotCounter;
        case 3: // smart choice
            if (csp[i].cr->cMana() - mCost >= nhMana() / 2)
                if (e.EActor->Timeout < 50)
                    goto DoCounterspell;
            goto DoNotCounter;
        }

DoCounterspell:
        if (csp[i].cr->cMana() < TEFF(csID)->ManaCost) {
            csp[i].cr->IPrint("You lack the mana to counterspell.");
            continue;
        }          
        if (csp[i].cr->Timeout > 80) {
            csp[i].cr->IPrint("You're too busy to counterspell.");
            continue;
        }

        goto Counterspelled;
DoNotCounter:;
    } 

    // nobody's able to counterspell
    return NOTHING;

Counterspelled:
    EventInfo xe;
    xe.Clear();
    xe.EActor = csp[i].cr;
    xe.EVictim = this;
    TPrint(xe,"You mutter a quick counterspell using <9><Res><7>.",
        "The <EActor> counters your spell with <9><Res><7>.",
        "The <EActor> mutters a quick counterspell!", csID);

    csp[i].cr->Timeout += min(3, (isDispel ? 20 : 15) - csp[i].cr->SkillLevel(SK_SPELLCRAFT));
    LoseMana(mCost);

    if (csp[i].cr->HasFeat(FT_REFLECTIVE_COUNTERSPELL))
        if (e.EActor != e.EVictim && e.EVictim != NULL)
            if (e.EXVal != e.EActor->x ||  e.EYVal != e.EActor->y)
                if (TEFF(e.eID)->Purpose & (EP_ATTACK|EP_CURSE)) {
                    TPrint(xe, "Your counterspell turns the magic back on the <EVictim>!",
                        "The <EActor>'s counterspell turns your magic back on you!",
                        "The <EActor>'s counterspell turns the <EVictim>'s magic "
                        "back on <him:EVictim>.");
                    e.EVictim = e.EActor;
                    return NOTHING;
                }

    return ABORT;
}
   

int16 Creature::SpellRating(rID eID, uint32 mm, bool perceived)
  {
    int16 Chance, i;

    if (isMType(MA_ELF) && (TEFF(eID)->Schools & SC_NEC)
                        && !(TEFF(eID)->Schools & SC_ABJ))
      return 0;

    Chance = TEFF(eID)->BaseChance;
    Chance += Mod(A_INT)*5;
    Chance += CasterLev()*3;

    Chance -= MMFeatLevels(mm) * 10;

    /* This is not always correct -- it lets NPC wizards in plate
       mail cast spells that are shared on both the arcane and
       divine list without spell failure due to armour -- but its
       close enough for now. */
    if ( TEFF(eID)->HasSource(AI_THEURGY) || 
         TEFF(eID)->HasSource(AI_DRUIDIC) )
      Chance += max(Attr[A_ARC],Attr[A_DIV])*5;
    else
      Chance += Attr[A_ARC]*5;

    if (Chance > 90)
      Chance = 90 + ((Chance - 90) / 10);

    if (Chance > 50 && (mm & MM_SURE))
      return 100;

    return max(2,min((uint16)98,Chance));
  }

int16 p_base, p_int, p_lev, p_conc,
      p_meta, p_spec, p_calc, p_circ;
String ps_circ;
const char* ps_perfect, *ps_calc;

int16 Character::SpellRating(rID eID, uint32 mm, bool perceived)
  {
    int16 Chance,j; uint32 i;
    int16 concentLeft;
    int16 sp = theGame->SpellNum(eID);
    TEffect *te = TEFF(eID);
    
    p_base = p_lev = p_circ = p_spec = p_meta = p_calc = p_int = p_conc = 0;
    ps_circ.Empty(); ps_perfect = NULL; ps_calc = NULL;
    
    if (isMType(MA_ELF) && (te->Schools & SC_NEC)
                        && !(te->Schools & SC_ABJ))
      return -1;
    
    /* Does the character have the needed spellbook equipped? */
    // ignore for now...
    if ((Spells[sp] & (SP_INNATE|SP_STAFF))) {
      ps_perfect = (Spells[sp] & SP_INNATE) ? "innate" : "staff";
      return 100;
    } 
    if (HasStati(POLYMORPH)) {
      TMonster * tm = TMON(mID);
      for(rID sID = tm->FirstRes(AN_INNATE); sID; 
          sID = tm->NextRes(AN_INNATE)) {
        if (sID == eID) {
          ps_perfect = "innate"; 
          return 100; 
          }
      } 
    }

    /* Is the spell known? */
    if (!(Spells[sp] & (SP_KNOWN|SP_DOMAIN))) {
      if (Type == T_PLAYER)
        if (thisp->Opt(OPT_ALL_SPELLS)) {
          if (TEFF(eID)->Level > ((CasterLev()+1)/2))
            return -1;            
          if (TEFF(eID)->HasSource(AI_WIZARDRY))
            goto KnowAnyway;
          if (TEFF(eID)->HasSource(AI_SORCERY))
            goto KnowAnyway;
          if (TEFF(eID)->HasSource(AI_THEURGY))
            goto KnowAnyway;
          if (TEFF(eID)->HasSource(AI_DRUIDIC))
            goto KnowAnyway;
          }
      return -1;
      }
    KnowAnyway:

    if (!te->BaseChance) /* Turn Undead or something for */
      return 99;                /* Wizard mode debugging.       */

    /* return the base chance + modifier */
    p_base = te->BaseChance;
    p_int  = Mod2(A_INT)*5;

    /*
        I want wizards to have some choice about where to put
      their skill points, rather than having Concentration be
      so utterly manditory. It's the skill you buy if you want
      to cast spells while distracted, not if you want to cast
      spells full-stop.
      Chance += 5 * (SkillLevel(SK_CONCENT) - (te->Level *3));
    */

    p_lev  = (CasterLev() - SAL(te->Level))*2;

    p_meta = -MMFeatLevels(mm)*5;
    
    if (HasStati(SPECIALTY_SCHOOL) && (Spells[sp] & (SP_ARCANE))) {
      i = GetStatiVal(SPECIALTY_SCHOOL);
      p_spec = -50;
      /* Choose the most favourable modifier out of all the
         schools that the spell belongs to. */
      for(j = 0;j!=9;j++)
        if (TEFF(eID)->Schools & XBIT(j))
          p_spec = max(p_spec,SpecialistTable[i][j]);

      if (p_spec == -50)
        p_spec = 0;
      }     
    /* If the spell is on your list as both arcane and divine/primal,
       you benefit from the positive specMod, but aren't hindered by
       the negative one -- you don't get WORSE at casting any of your
       druidic Evocations when you become an Illusionist, but you do
       get better at shared mage/druid illusions. */
    if (Spells[sp] & (SP_DIVINE|SP_BARDIC|SP_PRIMAL|SP_SORCERY))
      p_spec = max(p_spec,0);
    

    int16 *at;
    if (perceived) 
      at = KAttr;
    else
      at = Attr;
      
    p_calc = -100;
    if (Spells[sp] & (SP_DIVINE|SP_DOMAIN))
      if (at[A_DIV]*5 > p_calc)
        { p_calc = at[A_DIV]*5; ps_calc = "divine"; }
    if (Spells[sp] & (SP_BARDIC))
      if (at[A_BAR]*5 > p_calc)
        { p_calc = at[A_BAR]*5; ps_calc = "bardic"; }
    if (Spells[sp] & (SP_PRIMAL))
      if (at[A_PRI]*5 > p_calc)
        { p_calc = at[A_PRI]*5; ps_calc = "primal"; }
    if (Spells[sp] & (SP_SORCERY))
      if (at[A_SOR]*5 > p_calc)
        { p_calc = at[A_SOR]*5; ps_calc = "sorcery"; }
    if (Spells[sp] & (SP_ARCANE))
      if (at[A_ARC]*5 > p_calc)
        { p_calc = at[A_ARC]*5; ps_calc = "arcane"; }
    if (p_calc == -100)
      p_calc = 0;
    
    
    p_circ = 0; ps_circ.Empty();
    if (!(mm & MM_STILL)) {
      if (HasStati(GRAPPLED) || HasStati(GRAPPLING) || HasStati(GRABBED) || HasStati(STUCK))
        { p_circ = -60; ps_circ += "grapple/"; }
      else if (HasStati(PRONE))
        { p_circ = -20; ps_circ += "prone/"; }
      else if (m && TTER(m->TerrainAt(x,y))->HasFlag(TF_DEEP_LIQ))
        { p_circ = -40; ps_circ += "water/"; }
      else if (m && TTER(m->TerrainAt(x,y))->HasFlag(TF_STICKY))
        { p_circ = -40; ps_circ += "terrain/"; }
      }
    if (HasStati(MOUNTED) && isThreatened())
      { p_circ -= max(0,40 - SkillLevel(SK_RIDE)*2);
        ps_circ += "mounted/"; }
    
    concentLeft = ConcentBuffer() - concentUsed;
    p_conc = min(abs(p_circ),concentLeft*5);

    Chance = p_base + p_int + p_lev + p_meta + p_spec + p_calc + p_circ + p_conc;

    if (Chance > max(90,p_base))
      Chance = max(90,p_base) + ((Chance - max(90,p_base)) / 5);

    if (Chance > 50 && (mm & MM_SURE))
      return 100;

    return max(2,min((uint16)100,Chance));

  }
  
int16 dc_base, dc_attr, dc_focus, dc_trick, dc_will,
      dc_beguile, dc_hard, dc_lev, dc_height, dc_affinity;
const char *dcs_attr;
  
int16 Creature::getSpellDC(rID spID, bool isTrick, bool isHeight)
  {
    int32 i, sp_flags;
    dc_base = 10;
    
    dc_lev = TEFF(spID)->Level;
  
    if (isCharacter())
      sp_flags = ((Character*)this)->Spells[theGame->SpellNum(spID)];
    else
      sp_flags = SP_ARCANE | SP_DIVINE | SP_SORCERY | SP_PRIMAL;
      
    dc_attr = -20;
    if (sp_flags & SP_ARCANE)
      if (Mod(A_INT) > dc_attr)
        { dc_attr = Mod(A_INT); dcs_attr = "Int"; }
    if (sp_flags & (SP_DIVINE|SP_PRIMAL|SP_DOMAIN))
      if (Mod(A_WIS) > dc_attr)
        { dc_attr = Mod(A_WIS); dcs_attr = "Wis"; }
    if (sp_flags & (SP_SORCERY|SP_BARDIC|SP_INNATE))
      if (Mod(A_CHA) > dc_attr)
        { dc_attr = Mod(A_CHA); dcs_attr = "Cha"; }
    if (dc_attr == -20)
      dc_attr = 0;
    
    dc_focus = 0;
    StatiIterNature(this,SCHOOL_FOCUS)
        if (S->Mag & TEFF(spID)->Schools)
          dc_focus = 2;
    StatiIterEnd(this)

    if (sp_flags & SP_DOMAIN) {
      if (HasFeat(FT_DOMAIN_MASTERY))
        dc_focus += 4; 
      else if (HasFeat(FT_DOMAIN_FOCUS))
        dc_focus += 2; 
      }

    dc_beguile = 0;
    if (HasAbility(CA_BEGUILING_MAGIC))
      if (TEFF(spID)->HasFlag(EF_MENTAL))
        dc_beguile = max(0,Mod(A_CHA));
      
    dc_trick = isTrick ? 8 : 0;
    dc_height = isHeight ? 4 : 0;

    dc_hard = 0;
    if (TEFF(spID)->HasFlag(EF_HARDSAVE2))
      dc_hard += 2;
    if (TEFF(spID)->HasFlag(EF_HARDSAVE4))
      dc_hard += 4;
    if (TEFF(spID)->HasFlag(EF_HARDSAVE8))
      dc_hard += 8;
      
    dc_will = AbilityLevel(CA_ARCANE_WILL);

    dc_affinity = 0;
    if ( (isMType(MA_LIZARDFOLK) && (TEFF(spID)->Schools & SC_WATER)) ||
         (isMType(MA_DWARF) &&      (TEFF(spID)->Schools & SC_EARTH)) )
      dc_affinity += 2;

    return dc_base + dc_lev + dc_attr + dc_focus + dc_will +
           dc_beguile + dc_trick + dc_hard + dc_height + dc_affinity; 
  }
  
int16 Creature::getSpellMana(rID spID, uint32 MM, int16 *specMod2)
  {
    int16 mCost, mult, sp, specMod, j, sch;
    sp = theGame->SpellNum(spID);
    
    mCost = TEFF(spID)->ManaCost;

    if (specMod2)
      *specMod2 = 0;
    
    if (!(isPlayer() && (thisp->Spells[sp] & SP_KNOWN)))
      if (HasEffStati(INNATE_SPELL,spID) || (isPlayer() && 
             (thisp->Spells[sp] & SP_INNATE)))
        goto SkipSpellSpecifics;
        
    if (MM)
      {
        mult = 2 + MMFeatLevels(MM);
        mCost = (mCost * mult + 1) / 2;
        mCost = max(mCost,TEFF(spID)->ManaCost + (3*(mult-2)));
      }

    /* New rule -- specialist wizards suffer more for casting from thier
       opposition schools. The mana cost is modified by the specialist
       cost, and there is a percentage chance equal to the specialist
       penalty that the opposition spell drains a point of fatigue. Thus,
       wizards can use opposition spells, but not use them as staples of
       their strategy. This serves to diffrentiate the specialists from
       each other in play. */
    specMod = 0;
    if ((!isPlayer()) || (thisp->Spells[sp] & SP_ARCANE))
      if (HasStati(SPECIALTY_SCHOOL)) {
        sch = GetStatiVal(SPECIALTY_SCHOOL);
        specMod = -50;
        /* Choose the most favourable modifier out of all the
           schools that the spell belongs to. */
        for(j = 0;j!=9;j++)
          if (TEFF(spID)->Schools & XBIT(j)) 
            specMod = max(specMod,SpecialistTable[sch][j]);
              
        if (specMod == -50)
          specMod = 0;
        }     
    if (isPlayer() && (thisp->Spells[sp] & 
          (SP_DIVINE|SP_BARDIC|SP_PRIMAL|SP_SORCERY)))
      specMod = max(specMod,0);

    
    if (TEFF(spID)->Purpose & EP_BUFF)
      {
        if (AbilityLevel(CA_PREPATORY_MAGIC))          
          {
            int16 percent = 100 - AbilityLevel(CA_PREPATORY_MAGIC)*5;
            mCost = max( (mCost*percent)/100 , 1);
          }
        if (HasFeat(FT_MYSTIC_PREPARATION))
          mCost = max( (mCost-2), 1);
      }
      
    if (specMod < 0)
      mCost += (mCost * abs(specMod)) / 10;
    else if (specMod > 0)
      mCost = max (1, mCost - ((mCost * abs(specMod)) / 50));

    if (specMod2)
      *specMod2 = specMod;
    
    SkipSpellSpecifics:
    
    if ( (isMType(MA_LIZARDFOLK) && (TEFF(spID)->Schools & SC_WATER)) ||
         (isMType(MA_DWARF) && (TEFF(spID)->Schools & SC_EARTH)) ) 
      mCost = (mCost+1)/2;
    
    return mCost;
  }

uint32 Creature::getStoredMM(rID spID)
  {
    uint32 MM;
    MM = 0;
    StatiIterNature(this,STORED_MM)
      if (S->eID == spID)
        MM |= (S->Val + S->Mag >> 16);
    StatiIterEnd(this)
    return MM;
  }

void Creature::getBuffInfo(int16 *pBuffCount, int16 *pManaCost, rID **pBuffList)
  {
    static rID buffList[64]; TEffect *te;
    int16 buffCount, manaCost, i; 
    
    buffCount = manaCost = 0;
    
    StatiIter(this)
      if (!(S->eID && S->Source == SS_ENCH))
        continue;
      for (i=0;i!=buffCount;i++)
        if (buffList[i] == S->eID)
          continue;
      te = TEFF(S->eID);
      if (!(te->HasFlag(EF_LOSEMANA) && te->HasFlag(EF_NOTBAD)))
        continue;
      buffList[buffCount] = S->eID;
      buffCount++;
      
      manaCost += getSpellMana(S->eID,getStoredMM(S->eID),NULL);
    StatiIterEnd(this)
    buffList[buffCount] = 0;
    if (pBuffCount)
      *pBuffCount = buffCount;
    if (pManaCost)
      *pManaCost = manaCost;
    if (pBuffList)
      *pBuffList = &buffList[0];
  }
   
void Character::RecalcStaffSpells()
  {
    int16 i, j; Item *it;
    rID StaffSpellList[64];
    
    for (i=0;i!=MAX_SPELLS;i++)
      Spells[i] &= ~SP_STAFF;
      
    if (!HasStati(STAFF_SPELLS))
      return;
      
    StatiIterNature(this,STAFF_SPELLS)
      it = oItem(S->h);
      if (!it->isKnown(KN_MAGIC))
        continue;
      memset(StaffSpellList,0,sizeof(rID)*64);
      RES(S->eID)->GetList(
            STAFF_SPELL_LIST,
            StaffSpellList, 63);
      for (j=0;j!=60;j++)
        {
          if (StaffSpellList[j] >= 256) {
            if (isMType(MA_ELF))
              if ((TEFF(StaffSpellList[j])->Schools & SC_NEC) &&
                  !(TEFF(StaffSpellList[j])->Schools & SC_ABJ))
                goto SkipNecro;
            Spells[theGame->SpellNum(
              StaffSpellList[j])] |= SP_STAFF;
            }
          else
            if (S->h && oThing(S->h)->isType(T_ITEM))
              if (oItem(S->h)->GetPlus() < StaffSpellList[j])
                goto FinishedThisList;
          SkipNecro:
          if (!(StaffSpellList[j] ||
                StaffSpellList[j+1] ||
                StaffSpellList[j+2]))
            break;
        }
      FinishedThisList:;
    StatiIterEnd(this)
  }
       
int16 Creature::GetStaffFatigueCost(rID spID)
  {
    int16 i, j, cost; Item *it;
    rID StaffSpellList[64];
    
    if (!HasStati(STAFF_SPELLS))
      return -1;
      
    StatiIterNature(this,STAFF_SPELLS)
      it = oItem(S->h);
      if (!it->isKnown(KN_MAGIC))
        continue;
      memset(StaffSpellList,0,sizeof(rID)*64);
      RES(S->eID)->GetList(
            STAFF_SPELL_LIST,
            StaffSpellList, 63);
      cost = 1;
      for (j=0;j!=60;j++) {
        if (StaffSpellList[j] < 256)
          cost = StaffSpellList[j];
        else if (StaffSpellList[j] == spID)
          StatiIterBreakout(this,return cost);
        if (!(StaffSpellList[j] ||
              StaffSpellList[j+1] ||
              StaffSpellList[j+2]))
          break;
        }
    StatiIterEnd(this)
    return -1;
  }

EvReturn Item::DrinkPotion(EventInfo &e)
  {
    EvReturn result;
    e.EActor->AccessTime(e.EItem);
    e.EItem = e.EItem->TakeOne();
    DPrint(e,NULL,"The <EActor> drinks <Str>.",(const char*)e.EItem->Name(NA_A));
    e.EItem->Remove(false);

    /*
       This is way too harsh for a roguelike, where potions of
       dimension door are supposed to keep you alive.
       e.EActor->ProvokeAoO();  // ww: SRD, Combat I
     */
    if (e.EActor->isDead())
      return ABORT; 

    e.eID = eID;

    if (e.EActor->isMType(MA_ELF))
      if (e.eID && (TEFF(e.eID)->Schools & SC_NEC)
           && !(TEFF(e.eID)->Schools & SC_ABJ))
        {
          e.EActor->Timeout += 30;
          e.EActor->IPrint("Nothing happens.");
          return DONE;
        }

    result = ReThrow(EV_EFFECT,e);
    e.EActor->Timeout += 30;
    if (!e.EItem->isKnown(KN_MAGIC) && e.EActor->HasSkill(SK_ALCHEMY))
      if (e.EActor->SkillCheck(SK_ALCHEMY, 10 + e.EItem->ItemLevel(),true))
        e.EActor->IdentByTrial(e.EItem);
    
    e.EItem->Remove(true);
    
    return result;
  }

EvReturn Item::ReadScroll(EventInfo &e)
  {
    EvReturn result, csr; int8 sLevel, cLevel;
    bool preserved;
    e.EActor->AccessTime(e.EItem);
    e.EItem = e.EItem->TakeOne();
    preserved = false;

    if (e.EActor->isMType(MA_ELF))
      if (e.eID && (TEFF(e.eID)->Schools & SC_NEC)
           && !(TEFF(e.eID)->Schools & SC_ABJ))
        {
          e.EActor->Timeout += 30;
          e.EActor->IPrint("Nothing happens.");
          return DONE;
        }

    sLevel = TEFF(eID)->Level*2-1;
    
    
    cLevel = e.EActor->SkillLevel(SK_DECIPHER);

    /*
    if (!cLevel) {
      e.EActor->IPrint("It's all just gibberish to you.");
      return ABORT;
      }
    */
    
    DPrint(e,NULL,"The <EActor> reads <Str>.", (const char*)e.EItem->Name(NA_A));

    if (TEFF(eID)->HasFlag(EF_STAPLE))
      sLevel -= 5;  // very easy to use a staple scroll 

    if (sLevel > cLevel)
      {
        e.EActor->IPrint("This scroll is powerful, and possibly beyond you!");
        if (!Inscrip.GetLength())
          Inscrip = "scary";  
        if (e.EActor->SavingThrow(WILL,15))
          if (e.EActor->yn("Stop reading?",false))
            return ABORT;
      }
      
    if (!e.EActor->SkillCheck(SK_DECIPHER,10 + sLevel,true))
      {
        DPrint(e,"The magic runs amok!","The magic runs amok!");
        /* WildMagic(e,sLevel); */               
        result = DONE;
        goto Failed;
      }

//DoScroll:
    if (LastSkillCheckResult > 20 + sLevel && !random(2)) {
      e.EActor->IPrint("You manage to preserve the power within the ancient writing!");
      preserved = true;
      }
    else {
      if (sLevel > cLevel)  
        e.EActor->IPrint("You manage to read the eldritch magic properly.");
      }
    e.EItem->Remove(false);
      
    e.eID = eID;

    csr = e.EActor->Counterspell(e,NULL);
    if (csr == ABORT)
      return DONE;

    result = ReThrow(EV_EFFECT,e);

    if (!e.EItem->isKnown(KN_MAGIC) && e.EActor->HasSkill(SK_KNOW_MAGIC))
      if (e.EActor->SkillCheck(SK_KNOW_MAGIC,10 + e.EItem->ItemLevel(),true))
        e.EActor->IdentByTrial(e.EItem);

Failed:
    e.EActor->Timeout += max(10,50 - cLevel*2);

    if (e.EActor->SpellRating(eID) <= 0)
      {
        if (e.EActor->cMana() <= (TEFF(eID)->ManaCost*2)/3) 
          ThrowDmg(EV_DAMAGE,AD_NORM,TEFF(eID)->Level*2,
            "scroll strain",e.EActor,e.EActor,e.EItem);
        e.EActor->LoseMana((TEFF(eID)->ManaCost*2)/3,TEFF(eID)->HasFlag(EF_LOSEMANA));
      }   
    if (preserved) {
      if (e.EItem->Owner() != e.EActor)
        e.EActor->GainItem(e.EItem,true);
      }
    else
      e.EItem->Remove(true);
    return result;
  }
      
EvReturn Item::Activate(EventInfo &e)
  {             
    EvReturn r;
    e.isActivation = true;
    if (!e.eID)
      e.eID = eID;

    e.EActor->Timeout += 30;
    e.EActor->AccessTime(e.EItem);
    
    if (e.EActor->isMType(MA_ELF))
      if (e.eID && (TEFF(e.eID)->Schools & SC_NEC)
           && !(TEFF(e.eID)->Schools & SC_ABJ))
        {
          e.EActor->IPrint("Nothing happens.");
          return DONE;
        }        

    if (e.EItem->Quantity > 1)
      e.EItem = e.EItem->TakeOne();
    e.isItem = true;
    r = ReThrow(EV_EFFECT,e);
    /* I'm not sure what this is supposed to do (returning
       weapons?) but it's killing my attempt to implement
       the Rod of the Python. */
    /*
    if (e.EItem->Owner() != e.EActor)
      if (!(e.EItem->Flags & F_DELETE))
        e.EActor->GainItem(e.EItem);
    */
    return r;
      
  }
      
EvReturn Item::UseStaff(EventInfo &e)
  {

    return DONE;
  }

EvReturn Item::ZapWand(EventInfo &e)
  {
    int16 Cost, roll, rollA, rollB, rollC, Rating, Lev, Supercharge, SpecBonus;
    EvReturn r, csr;
    if (!Charges)
      {
        Nothing:
        e.eID = 0;
        return ABORT; 
        // ww: previously this let you zap wands even when
        // you were out of chages! really! ReThrow(EV_EFFECT,e);
      }

    if (!e.eID && e.EItem)
      e.eID = e.EItem->eID;
    if (!e.eID)
      goto Nothing;

    int16 zappingTimeout = 
            2000 / max(25,100+e.EActor->Mod(A_INT)*5);  
    // ww: let's bring that INT mod into play here
    e.EActor->Timeout += zappingTimeout;
    e.EActor->AccessTime(e.EItem);

    if (e.EActor->isMType(MA_ELF))
      if ((TEFF(eID)->Schools & SC_NEC)
           && !(TEFF(eID)->Schools & SC_ABJ))
        goto Nothing;

    Cost = TEFF(eID)->ManaCost;
    if (e.EActor->isPlayer() && e.EPActor->Opt(OPT_INF_MANA))
      Cost = 0;

    if (e.EActor->cMana() < Cost)
      {
        if (e.EItem->isKnown(KN_PLUS|KN_MAGIC)) {
          e.EActor->IPrint("You don't have enough mana.");
          return ABORT;
          }
        if (e.EActor->cMana() >= max(1,Cost/2))
          if ((e.EItem->isKnown(KN_PLUS|KN_MAGIC)) && e.EActor->yn("Fire it anyway?",true))
            {
              ThrowDmg(EV_DAMAGE,AD_NORM,Dice::Roll(TEFF(e.eID)->ManaCost - 
                e.EActor->cMana(), 4,0),"wand drain",NULL,e.EActor,e.EItem);
              e.EActor->LoseMana(Cost,TEFF(e.eID)->HasFlag(EF_LOSEMANA));
              goto Success;
            }
        if (!(e.EItem->isKnown(KN_PLUS|KN_MAGIC))) {
          e.EActor->IPrint("Your remaining mana fails to activate the wand.");
          e.EActor->LoseMana(Cost,TEFF(e.eID)->HasFlag(EF_LOSEMANA));
          }
        return DONE;
                  
      }
    Success:
    
    Supercharge = 0;

    Rating = e.EActor->SkillLevel(SK_USE_MAGIC); 
    
    rollA  = Dice::Roll(1,20);
    rollB  = Dice::Roll(1,20);
    rollC  = 0; 
    StatiIterNature(e.EActor, SKILL_BONUS)
      if (S->Val == SK_USE_MAGIC && S->Source == SS_PERM)
        rollC = Dice::Roll(1,20);
    StatiIterEnd(e.EActor)
    
    roll   = max(rollA,rollB);
    roll   = max(roll ,rollC);
    Lev    = e.EItem->ItemLevel();
    SpecBonus = 0; 
    if (TEFF(eID)->HasFlag(EF_STAPLE))
      Lev -= 5;  // very easy to use a staple wand 

    // ww: if you are a transmuter, you're better at using wands of
    // polymorph (or whatever). However, negative school bonuses don't
    // apply here -- otherwise an otherwise-equal wizard might actually be
    // worse than a warrior with a wand ...
    if (e.EActor->HasStati(SPECIALTY_SCHOOL)) {
      int i = e.EActor->GetStatiVal(SPECIALTY_SCHOOL);
      int m = -50;
      /* Choose the most favourable modifier out of all the
         schools that the spell belongs to. */
      for(int j = 0;j!=9;j++)
        if (TEFF(eID)->Schools & XBIT(j))
          m = max(m,SpecialistTable[i][j]);

      if (m > 0)
        SpecBonus = m / 5;
      }     
    Rating += SpecBonus;

    if (e.EActor->isPlayer()) {
      e.EPActor->MyTerm->SetWin(WIN_NUMBERS);
      e.EPActor->MyTerm->Clear();
      String skillStr;
      if (rollC)
        skillStr = Format("%cUse Magic Check:%c 1d20 (%d [%d/%d/%d]) %+d ",
          -13,-7,roll,rollA,rollB,rollC,Rating - SpecBonus);
      else
        skillStr = Format("%cUse Magic Check:%c 1d20 (%d [%d/%d]) %+d ",
          -13,-7,roll,rollA,rollB,Rating - SpecBonus);      
      if (SpecBonus)
        skillStr += Format("%+d (spec) ",SpecBonus);
      skillStr += Format("= %d vs DC %d %c[%s]%c.",
        Rating+roll, 10+Lev/2, 
        Rating+roll >= 10+(Lev/2) ? -EMERALD: -RED,
        Rating+roll >= 20+Lev   ? "*success*" :
        Rating+roll >= 10+(Lev/2) ? "success" : "failure",
        -7);
      e.EPActor->MyTerm->Write(skillStr);
      if (e.EPActor->Opt(OPT_STORE_ROLLS))
        e.EPActor->MyTerm->AddMessage(skillStr);
      }

    /* ww: this was unbalanced -- supercharged wands that don't cost mana
     * *and* don't cast charges were infinite deathsticks --
     * now wands always drain charges and cost mana */
         if (Rating + roll >= 30 + Lev) { Supercharge = 4; }
    else if (Rating + roll >= 25 + Lev) { Supercharge = 3; }
    else if (Rating + roll >= 20 + Lev) { Supercharge = 2; }
    else if (Rating + roll >= 15 + Lev) { Supercharge = 1; }
    else if (Rating + roll >= 10 + Lev/2) { /* normal success */ }
    else goto Failure; 

    e.EActor->LoseMana(Cost + (Cost * Supercharge / ((Cost+1) * (Supercharge+1))),
                        TEFF(e.eID)->HasFlag(EF_LOSEMANA));

    Charges--;
    e.eID = eID;
    if (e.EVictim == e.EActor)
      DPrint(e,NULL,"The <EActor> fires an <EItem> at <him:EActor>self.");
    else if (e.EVictim) 
      TPrint(e,NULL,"The <EActor> fires an <EItem> at you.",
                    "The <EActor> fires an <EItem> at <ETarget>.");
    else
      DPrint(e,NULL,"The <EActor> fires an <EItem>.");
    if (Supercharge) {
      IPrint("It was a <str> shot!",
        Supercharge == 1 ? "good" : "great");
      e.EItem->SetInherentPlus(e.EItem->GetInherentPlus() + Supercharge);
      }

    csr = e.EActor->Counterspell(e,NULL);
    if (csr != ABORT) 
      r = ReThrow(EV_EFFECT,e);
    else
      r = DONE;
      
    e.EItem->SetInherentPlus(e.EItem->GetInherentPlus() - Supercharge);
    
    //if (!Charges) { IPrint("The worn-out wand disappears!"); Remove(true); }

    return r;

    Failure:
    e.EActor->LoseMana(Cost/3,TEFF(e.eID)->HasFlag(EF_LOSEMANA));
    Charges--;
    e.eID = eID;
    DPrint(e,"You fail to activate the <EItem>.",
             "The <EActor> fails to activate an <EItem>.");
    if (!Charges) { IPrint("The worn-out wand disappears!"); Remove(true); }
    return DONE;

  }
