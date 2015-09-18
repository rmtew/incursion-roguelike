/* EFFECTS.CPP -- See the Incursion LICENSE file for copyright information.

     Code implementing the various specific prebuilt magical
   effect archetypes of the Incursion engine. The code for
   executing spells and items in general is contained in 
   Magic.Cpp.

     EvReturn Magic::Vision(EventInfo &e)
     EvReturn Magic::Blast(EventInfo &e)
     EvReturn Magic::Drain(EventInfo &e)
     EvReturn Magic::Grant(EventInfo &e)
     EvReturn Magic::Inflict(EventInfo &e)
     EvReturn Magic::Polymorph(EventInfo &e)
       bool TMonster::HasSlot(int16 sl)
       void Creature::Shapeshift(rID _mID, bool merge, Item* PolySource)
     EvReturn Magic::Dispel(EventInfo &e)
     EvReturn Magic::Reveal(EventInfo &e)
     EvReturn Magic::Summon(EventInfo &e)
     EvReturn Magic::Terraform(EventInfo &e)
       void Map::UpdateTerra()
       int8 Map::GetTerraDC(int16 x, int16 y)
       int8 Map::GetTerraDType(int16 x, int16 y)
       int16 Map::GetTerraDmg(int16 x, int16 y)
       void Map::RemoveTerra(int16 key)
       void Map::RemoveTerraXY(int16 x, int16 y, rID xID);
       void Map::WriteTerra(int16 x, int16 y, rID terID);
     EvReturn Magic::Illusion(EventInfo &e)
     EvReturn Magic::Creation(EventInfo &e)
     EvReturn Magic::Detect(EventInfo &e)
     EvReturn Magic::Travel(EventInfo &e)
     EvReturn Magic::Healing(EventInfo &e)
     String TEffect::Describe(Player *p)
*/

#include "Incursion.h"

/* Kludge to convey extra info */

EvReturn Magic::Vision(EventInfo &e)
  {           
    Term *t; uint8 ch; uint8 ovx, ovy, ox, oy;
    if (!e.EActor->isPlayer())
      return ABORT;
    if (e.EItem && e.EItem->isItem())
      if (!e.EItem->isKnown(KN_MAGIC)) {
        e.EActor->IPrint("You feel your senses expand beyond your body...");
        e.EActor->IdentByTrial(e.EItem);
      }

    e.vRange *= 5;

    t = ((Player*)e.EActor)->MyTerm;
    int16 ViewpointX = e.EActor->x;
    int16 ViewpointY = e.EActor->y;
    ox = (uint8)e.EActor->x;
    oy = (uint8)e.EActor->y;
    thisp->ShadowRange = thisp->SightRange = (uint8)e.EMagic->yval;
    thisp->LightRange  = 100;
    
    do {
      e.EActor->x = ViewpointX;
      e.EActor->y = ViewpointY;
      t->AdjustMap(ViewpointX,ViewpointY);
      e.EPActor->CalcVision(); 
      t->ShowMap();
      t->ShowViewList();
      t->PutGlyph(ViewpointX,ViewpointY,GLYPH_VALUE(GLYPH_VIEWPOINT, SKYBLUE));
      ovx = (uint8)ViewpointX;
      ovy = (uint8)ViewpointY;                           
      ch = (uint8)t->GetCharCmd(KY_CMD_ARROW_MODE);
      switch(ch) {
        case KY_CMD_NORTH:     ViewpointY--; break;
        case KY_CMD_NORTHEAST: ViewpointY--; ViewpointX++; break;
        case KY_CMD_EAST:      ViewpointX++; break;
        case KY_CMD_SOUTHEAST: ViewpointY++; ViewpointX++; break;
        case KY_CMD_SOUTH:     ViewpointY++; break;
        case KY_CMD_SOUTHWEST: ViewpointY++; ViewpointX--; break;
        case KY_CMD_WEST:      ViewpointX--; break;
        case KY_CMD_NORTHWEST: ViewpointY--; ViewpointX--; break;
        default: ch = KY_CMD_ESCAPE; 
        }
      if (!e.EActor->m->InBounds(ViewpointX+DirX[DIR_OF_KY_CMD(ch)],
            ViewpointY+DirY[DIR_OF_KY_CMD(ch)]) ||
           dist(ox,oy,ViewpointX,ViewpointY) > e.vRange)
        { ViewpointX = ovx; ViewpointY = ovy; }
      }
    while(ch != KY_CMD_ESCAPE);
    e.EActor->x = ox;
    e.EActor->y = oy;
    /*
    for (ViewpointX = ox - e.vRange; 
         ViewpointX <= ox + e.vRange;
         ViewpointX++) 
      for (ViewpointY = oy - e.vRange; 
          ViewpointY <= oy + e.vRange;
          ViewpointY++) {
        if (e.EActor->m->InBounds(ViewpointX,ViewpointY) &&
            dist(e.EActor->x,e.EActor->y,ViewpointX,ViewpointY) <= e.vRange)
          e.EActor->m->MarkAsSeen(VI_VISIBLE,ViewpointX,ViewpointY,0);
      }
    */
    
    ((Player*)e.EActor)->CalcVision();
    t->AdjustMap(e.EActor->x,e.EActor->y,true);
    t->ShowMap();
    return DONE;
  }

/* Important note! This function is called from two places -- from
   the magic subsystem for spells, and from the combat subsystem
   for breath weapons et al. We thus _cannot_ assume that a blast
   is an effect -- e.EMagic might be NULL, and other effect values
   may not be set. We check if we are processing an effect by look-
   ing at eID; in the case of a breath weapon, eID == 0. */

EvReturn Magic::Blast(EventInfo &e)
{
  String bdesc, msg, *pstr; uint8 save_type;
  uint8 i; bool noadj = false;
  const char *verb1, *verb2; bool plural = false;
  uint8 num_missles; int16 tdtype; 
  bool IEvade = false, incor = false;
  save_type = NOSAVE;
  
  if (!e.ETarget)
    Fatal("Blast without target!");
  if (e.eID && (e.EItem && e.EItem->isItem()))
    e.EItem->VisibleID(e.EActor);

  if (TEFF(e.eID) && 
      (TEFF(e.eID)->HasFlag(EF_CASTER_IMMUNE) || 
       TEFF(e.eID)->HasFlag(EF_CASTER_IMMUNE_BLAST)) &&
      (e.EActor == e.EVictim)) {
    return DONE;
  } 

  // ww: a poisoned weapon ends up calling Blast() with the poison
  // effect, which reveals a distant rogue 
  if ( (e.EActor->HasFeat(FT_SHOOT_FROM_COVER)) &&
      (!(e.EActor->isBeside(e.EVictim))) &&
      (e.EActor->HasStati(HIDING)) ) {
    if (e.EVictim->isCreature())
        e.EVictim->ts.HearNoise(e.EVictim,(uint8)e.EActor->x,(uint8)e.EActor->y);
  } else 
    e.EActor->Reveal(true);
  if (e.EVictim->isCreature())
  e.EVictim->Reveal(false);

  e.EMap->SetQueue(QUEUE_BLAST_MSG);

  /* For breath weapons, damage isn't 
     prerolled. Roll it now. */
  if (!e.eID)
    e.vDmg = e.Dmg.Roll();

  int16 vicPlane = e.EVictim->onPlane();

  if (e.DType == AD_SUNL)
    if (e.EVictim->isMType(MA_UNDEAD))
      e.isGhostTouch = true;

  if (vicPlane != e.EActor->onPlane())
    if (e.DType != AD_MAGC && 
        e.DType != AD_HOLY && 
        e.DType != AD_NECR && 
        !(TEFF(e.eID)->Schools & SC_FORCE) &&
        !e.isGhostTouch) { 
      incor = true; e.vDmg = 0; goto SkipSave; 
      }

  if (e.EVictim->isCreature()) {

    if (e.eID && !TEFF(e.eID)->HasFlag(EF_PARTIAL))
      goto SkipSave;
    if (e.BlastMessageDone)
      goto DoneSave;

    save_type = e.eID ? e.EMagic->sval : REF;
    if (save_type == NOSAVE)
      goto SkipSave;
    if (e.EVictim->HasFeat(FT_WEATHER_BLAST) && save_type == REF) {
      if (e.EVictim->AbilityLevel(CA_EVASION))
        if (e.EVictim->yn("Attempt Reflex save to use Evasion? ",true))
          goto SkipWeather;
      save_type = (e.EVictim->Attr[A_SAV_FORT] > 
          e.EVictim->Attr[A_SAV_REF]) ? FORT : REF;
    }

SkipWeather:
    e.EMap->SetQueue(QUEUE_RESIST_MSG);

    if (e.EVictim->SavingThrow(save_type,e.saveDC)) {
      e.Resist = true;
      if (e.EVictim->AbilityLevel(CA_EVASION) && save_type == REF &&
          e.EVictim->ArmourType() <= WG_LARMOUR)
      {
        VPrint(e,"You dodge the blast.",
            "The <EVictim> dodges the blast.");
        e.isEvaded = true;
        e.EVictim->Exercise(A_DEX,random(10)+1,EDEX_EVASION,45); 
      }
      else {
        VPrint(e,"You partially resist.", "An <EVictim> partially resists.");
      }
    }
    else if (e.EVictim->AbilityLevel(CA_EVASION) > 9 && save_type == REF &&
        e.EVictim->ArmourType() <= WG_LARMOUR)
    {
      VPrint(e,"You partially dodge the blast.",
          "An <EVictim> partially dodges the blast.");
      e.isPartiallyEvaded = true;
    }              

    e.EMap->UnsetQueue(QUEUE_RESIST_MSG);
DoneSave:

    if (e.isEvaded)
      e.vDmg = 0;
    else if (e.isPartiallyEvaded || e.Resist) {
      if (e.EVictim->HasFeat(FT_PARTIAL_EVASION))
        e.vDmg = (e.vDmg+2) / 3;
      else 
        e.vDmg = (e.vDmg+1) / 2;
    }

SkipSave:


    if (e.EActor) {
      if ((e.DType == AD_COLD && e.EActor->HasFeat(FT_LORE_OF_RIME)) ||
          (e.DType == AD_ACID && e.EActor->HasFeat(FT_LORE_OF_ACID)) ||
          (e.DType == AD_FIRE && e.EActor->HasFeat(FT_LORE_OF_FLAMES)) ||
          (e.DType == AD_NECR && e.EActor->HasFeat(FT_LORE_OF_DEATH)) ||
          (e.DType == AD_ELEC && e.EActor->HasFeat(FT_LORE_OF_STORMS))) {
        int16 bonus = max((e.vDmg * 20) / 100,1);
        e.strDmg += Format(" +%d Lore",bonus);
        e.vDmg += bonus; 
      } 
    } 

    if (e.vDmg) {
      i = e.saveDC;
      tdtype = e.DType;

      if (e.eID && TEFF(e.eID)->HasFlag(EF_HALF_UNTYPED)) {
        e.vDmg = (e.vDmg+1)/2;
        EventInfo eCopy = e; 
        eCopy.saveDC = -1;
        eCopy.DType = AD_NORM; 
        ReThrow(EV_DAMAGE,eCopy);
      } 
      if (e.MM & MM_CONSECRATE) {
        e.vDmg = (e.vDmg+1)/2;
        EventInfo eCopy = e; 
        eCopy.saveDC = -1;
        eCopy.DType = AD_HOLY; 
        ReThrow(EV_DAMAGE,eCopy);
      }
      e.saveDC = i;
      e.DType = (int8)tdtype; 
      ReThrow(EV_DAMAGE,e); 
/*      ThrowDmg(EV_DAMAGE,e.DType,e.vDmg,NULL,
          e.EActor, e.EVictim, e.EItem, e.EItem2); */
    }

    if (e.EVictim->Flags & F_DELETE)
      e.EMap->EmptyQueue(QUEUE_RESIST_MSG);
    else
      e.EMap->PrintQueue(QUEUE_RESIST_MSG);

    if (e.vDmg) {
      Term * TActor = e.EActor->ShowCombatNumbers();
      Term * TVictim = e.EVictim->ShowCombatNumbers(); 
      if (TActor || TVictim) { 
      msg = Format("%cDamage:%c %s%s%s%s%s = %d %s",
          -MAGENTA, -GREY, e.Resist ? "(" : "", (const char*)e.Dmg.Str(), e.eID &&
          (e.MM & MM_EMPOWER) ? " + 50%" : "",
          e.Resist || e.isPartiallyEvaded ? ") / 2" : "", 
          (const char*)(e.strDmg ? e.strDmg : ""),
          (int)e.vDmg, Lookup(DTypeNames,e.DType));
        if (TVictim) {
          TVictim->SetWin(WIN_NUMBERS4);
          TVictim->Clear();
          msg.SetAt(0,-PINK);
          TVictim->Write(0,0,msg);
          TVictim->AddMessage(msg);
          msg.SetAt(0,-13);
        }
        if (TActor && e.EActor != e.EVictim) { 
          TActor->SetWin(WIN_NUMBERS3);
          TActor->Clear();
          msg = Format("%c%s %s", -MAGENTA,
              (const char *)Capitalize(e.EVictim->Name(NA_POSS)),(const char*)msg);
          TActor->Write(msg); 
          TActor->AddMessage(msg);
        }
        e.strBlastDmg = msg;
      } 
    }

//SkipDamage:
    ;
  }
  else if (e.EVictim->isItem()) {
    ReThrow(EV_DAMAGE,e);      
  }
  else if (e.EVictim->isFeature()) {
    ReThrow(EV_DAMAGE,e); 
  }
  e.EMap->UnsetQueue(QUEUE_BLAST_MSG);

  if (!e.Terse && e.EVictim->isCreature() && !e.BlastMessageDone) {

    if (e.eID && e.DType == AD_MAGC && e.EMagic->aval == AR_BOLT)
    {
      num_missles = e.Dmg.Number;
      if (num_missles == 1)
        bdesc = "magic missile";
      else {
        bdesc = Format("%d magic missiles",num_missles);
        noadj = true; plural = true; }
    }
    else
      switch((e.eID && e.EMagic) ? e.EMagic->aval : AR_BREATH)
      { case AR_BALL: case AR_BURST:
        bdesc = Lookup(BallNames,e.DType);
        break;
        case AR_BOLT:
        bdesc = Lookup(BoltNames,e.DType);
        break;   
        case AR_BEAM:
        bdesc = Lookup(BeamNames,e.DType);
        break;   
        case AR_FIELD:
        bdesc = Lookup(CloudNames,e.DType);
        break;
        case AR_BREATH:
        bdesc = Lookup(BreathNames,e.DType);
        break;
        default:
        bdesc.Empty();
      }
    if (!e.eID || !(e.MM & MM_TRANSMUTE)) {
      pstr = TEFF(e.eID ? e.eID : e.EActor->mID)->GetMessages(MSG_BLASTNAME);
      if (*pstr)
        bdesc = *pstr; 
    }
    if (!bdesc.GetLength())
      goto SkipMessage;
    if (e.EVictim->Flags & F_DELETE)
    { verb1 = plural ? "kill" : "kills"; verb2 = "slain"; }
    else if (incor)
    { verb1 = plural ? "pass through" : "passes through"; 
      verb2 = "untouched"; }
    else
    { verb1 = plural ? "strike" : "strikes"; 
      verb2 = "struck"; }
      if (noadj)
        TPrint(e,"<Str1> <Str2> an <EVictim>.", "You are <Str3> by <Str1>.",
            "An <EVictim> is <Str3> by <Str1>.",(const char*)bdesc,verb1,verb2);
      else if (e.isTrap || e.isActOfGod)
        VPrint(e,"You are <Str3> by a <Str1>.",
            "An <EVictim> is <Str3> by a <Str1>.",
            (const char*)bdesc,verb1,verb2);
      else if (e.EActor == e.EVictim)
        VPrint(e,"Your own <Str1> <Str2> you!",
            "The <EActor>'s <Str1> <Str2> <him:EActor>.",
            (const char*)bdesc,verb1); 
      else if ((!e.eID) || (e.EMagic->aval == AR_BREATH))
        TPrint(e,"Your <Str1> <Str2> an <EVictim>.", "You are <Str3> by the <EActor>'s <Str1>.",
            "An <EVictim> is <Str3> by the <EActor>'s <Str1>.",(const char*)bdesc,verb1,verb2);
      else
        TPrint(e,"Your <Str1> <Str2> an <EVictim>.", "You are <Str3> by a <Str1>.",
            "An <EVictim> is <Str3> by a <Str1>.",(const char*)bdesc,verb1,verb2);
  }
SkipMessage:

  if (save_type != NOSAVE && !e.Resist && e.EVictim->isCreature())
    if (!e.eID || (!random(2) && !(TEFF(e.eID)->ef.aval & Q_EQU)))
    {
      Item *it = NULL; int16 c;
      /* 1/3 chance of damaging the weapon, 1/3 chance of damaging
         the armour, otherwise damage a randomly chosen EXPOSED item. */
      if ((it = e.EVictim->InSlot(SL_WEAPON)) && !random(3))
        ;
      else if ((it = e.EVictim->InSlot(SL_ARMOUR)) && !random(2))
        ;
      else {
        // ww: not polymorphed equipment, etc.  
        for(c=0,it=e.EVictim->FirstInv();it;it=e.EVictim->NextInv())
          if (it->GetParent() == e.EVictim) {
            bool goodSlot = false;
            for (int s =0; s<SL_LAST; s++)
              if (e.EVictim->InSlot(s) == it)
                goodSlot = true;
            if (goodSlot) 
          Candidates[c++] = it->myHandle;
          }
        if (c)
          it = oItem(Candidates[random(c)]);
      }
      if (it)
        ThrowDmg(EV_DAMAGE,e.DType,e.vDmg/2,NULL,e.EActor,it);
    }

  if (!e.Absorb)
    e.EMap->PrintQueue(QUEUE_BLAST_MSG);
  e.EMap->EmptyQueue(QUEUE_BLAST_MSG);
  e.BlastMessageDone = true;

  return DONE;
} 

EvReturn Magic::Drain(EventInfo &e)
  {
    int16 oDC;
    if (e.EMagic->aval == AR_POISON || e.EMagic->aval == AR_DISEASE)
      if (e.Resist)
        e.vDmg /= 2;
    e.DType = e.EMagic->xval;
    oDC = e.saveDC; e.saveDC = -1;
    ReThrow(EV_DAMAGE,e);
    e.saveDC = (int8)oDC;
    return DONE;
  }

#if 0
TextVal PositiveAdj[] = { 
  { A_STR, "stronger" },
  { A_DEX, "more agile" },
  { A_CON, "tougher" },
  { A_INT, "smarter" },
  { A_WIS, "wiser" },
  { A_CHA, "beautiful" },
  { A_MOV, "fleet" },
  { A_SPD, "skillful" },
  //{ "confident", A_
  //{ "graceful", A_
  { A_DEF, "protected" },
  { A_DMG, "lethal" },
  { A_AID, "blessed" },
  { "huge", A_SIZ },
  { "safe", A_SAV },
  //{ "powerful", A_
  { NULL, 0 }
   };

TextVal NegativeAdj[] = { 
  { "weaker", A_STR },
  { "clumsier", A_DEX },
  { "frail", A_CON },
  { "less smart", A_INT },
  { "foolish", A_WIS },
  { "uglier", A_CHA },
  { "slow", A_MOV },
  { "less graceful", A_SPD },
  //{ "inaccurate", A_
  //{ "awkweard", A_
  { "exposed", A_DEF },
  { "ineffective", A_DMG },
  { "mundane", A_AID },
  { "smaller", A_SIZ },
  { "threatened", A_SAV },
  //{ "the rush fade", A_
  { NULL, 0 }
   };
#endif

EvReturn Magic::Grant(EventInfo &e) {
    if (!e.ETarget->isCreature())
        return DONE;

    if (e.EMagic->xval == TIMESTOP)
        theGame->StopTime(e.EVictim);

    if (e.isWield || e.isEnter) {
        if (e.isWield) { 
            e.ETarget->GainPermStati(e.EMagic->xval,e.EItem,SS_ITEM,e.EMagic->yval,e.vDmg,e.eID,e.vCasterLev+(e.MM & MM_FORTIFY ? 5 : 0)); 
        } else if (e.isEnter)
            e.ETarget->GainTempStati(e.EMagic->xval,e.EItem,e.vDuration,SS_ENCH,e.EMagic->yval,e.vDmg,e.eID,e.vCasterLev+(e.MM & MM_FORTIFY ? 5 : 0)); 
    } else if (e.isRemove || e.isLeave) {
        if (e.EItem && e.EItem->isItem())
            e.ETarget->RemoveStatiFrom(e.EItem);
        else
            e.ETarget->RemoveEffStati(e.eID);

        e.Terse = true; 
    } else {
        if (e.ETarget->HasEffStati(e.EMagic->xval,e.eID,e.EMagic->yval)) {
            SetSilence();
            e.ETarget->RemoveEffStati(e.eID,EV_REMOVED,-1);
            UnsetSilence();
        }

        e.ETarget->GainTempStati(e.EMagic->xval, NULL, e.vDuration, 
            TEFF(e.eID)->HasFlag(EF_ONCEONLY) ? SS_ONCE : 
            TEFF(e.eID)->HasFlag(EF_CURSE)    ? SS_CURS : 
            TEFF(e.eID)->HasFlag(EF_MUNDANE)  ? SS_MISC : SS_ENCH, 
            e.EMagic->yval, e.vDmg,e.eID,e.vCasterLev + (e.MM & MM_FORTIFY ? 5 : 0));
    }

    if (e.EMagic->xval == PHASED)
        if (e.EVictim->HasStati(ANCHORED) || e.EVictim->HasStati(MANIFEST)) {
            e.Immune = true;
            VPrint(e,"You flicker briefly but remain where you are.",
                "The <EVictim> flickers briefly but remains where <he:EVictim> is.");
        }

    /*
    Handled in GainPermStati
    if (!e.Terse && e.ETarget->isCreature())
    e.EVictim->StatiMessage(e.EMagic->xval, e.EMagic->yval, e.isRemove);
    */

    switch(e.EMagic->xval) {
    case INVIS:      case SICK:
    case POISONED:   case BLIND:
    case STUNNED:    case ASLEEP:
    case PARALYSIS:  case AFRAID:
    case STUCK:      case PRONE:
    case LEVITATION: case ILLUMINATED:
        if ((e.EItem && e.EItem->isItem()))
            e.EActor->IdentByTrial(e.EItem);
        break;
    case INVIS_TO:
        if (e.EItem && e.EItem->isItem())
            if (e.EActor->isMType(e.vDmg))
                e.EActor->IdentByTrial(e.EItem);
        break;
    case STAFF_SPELLS:
        e.EVictim->RecalcStaffSpells();
        break;     
    }

    return DONE;
}


EvReturn Magic::Inflict(EventInfo &e) {
    if (e.isRemove || e.isLeave) {
        e.ETarget->RemoveEffStati(e.eID);
        e.Terse = true; 
        return DONE; 
    }

    if (e.EMagic->xval == CHARMED && e.EMagic->yval == CH_DOMINATE)
        if (e.EVictim->isMType(MA_SAPIENT))
            e.EActor->AlignedAct(AL_NONCHAOTIC,2,"mental coercion");

    if (e.EVictim->isCreature()) {
        switch (e.EMagic->xval) {
        case STUNNED:
            if (e.EVictim->ResistLevel(AD_STUN) == -1)
                goto DoneBecauseImmune;
            break;
        case CONFUSED:
            if (e.EVictim->ResistLevel(AD_CONF) == -1)
                goto DoneBecauseImmune;
            break;
        case POISONED:
            if (e.EVictim->ResistLevel(AD_POIS) == -1)
                goto DoneBecauseImmune;
            break;
        case DISEASED:
            if (e.EVictim->ResistLevel(AD_DISE) == -1)
                goto DoneBecauseImmune;
            break;
        case BLIND:
            if (e.EVictim->ResistLevel(AD_BLND) == -1)
                goto DoneBecauseImmune;
            break;
        case ASLEEP:
            if (e.EVictim->ResistLevel(AD_SLEE) == -1)
                goto DoneBecauseImmune;
            break;
        case PARALYSIS:
            if (e.EVictim->ResistLevel(AD_PLYS) == -1)
                goto DoneBecauseImmune;
            else if (e.EVictim->UseLimitedFA())
                goto DoneBecauseImmune;
            break;
        case AFRAID:
            if (e.EVictim->ResistLevel(AD_FEAR) == -1)
                goto DoneBecauseImmune;
            break;
        case PHASED:
            if (e.EVictim->HasStati(ANCHORED) || e.EVictim->HasStati(MANIFEST)) {
                VPrint(e,"You flicker briefly but remain where you are.",
                    "The <EVictim> flickers briefly but remains where <he:EVictim> is.");
                goto DoneBecauseImmune;
            }
            break;
        default:
            break;
        }
    }

    goto NotImmune;

DoneBecauseImmune:
    e.Immune = true;
    return DONE;

NotImmune:
    if (e.EMagic->xval == TIMESTOP)
        theGame->StopTime(e.EVictim);

    StatiIterNature(e.EVictim,e.EMagic->xval)
        if (S->eID == e.eID && S->Val == e.EMagic->yval)
            e.Terse = true; 
    StatiIterEnd(e.EVictim)

    e.EVictim->GainTempStati(e.EMagic->xval, e.EActor, e.vDuration, 
        TEFF(e.eID)->HasFlag(EF_ONCEONLY) ? SS_ONCE : 
        TEFF(e.eID)->HasFlag(EF_CURSE)    ? SS_CURS : 
        TEFF(e.eID)->HasFlag(EF_MUNDANE)  ? SS_MISC : SS_ENCH, 
        e.EMagic->yval, e.vDmg, e.eID,e.vCasterLev + (e.MM & MM_FORTIFY ? 5 : 0));

    /*
    Duplicate messages...
    if (!e.Terse && e.ETarget->isCreature())
        e.EVictim->StatiMessage(e.EMagic->xval, e.EMagic->yval, e.isRemove);
    */

    if (e.EMagic->xval == ASLEEP || e.EMagic->xval == CHARMED)
        if (e.EVictim->isHostileTo(e.EActor))
            e.EActor->KillXP(e.EVictim);

    switch (e.EMagic->xval) {
    case INVIS:
    case SICK:
    case POISONED:
    case BLIND:
    case STUNNED:
    case ASLEEP:
    case PARALYSIS:
    case AFRAID:
    case STUCK:
    case PRONE:
    case LEVITATION:
    case ILLUMINATED:
        if (e.EItem && e.EItem->isItem())
            e.EActor->IdentByTrial(e.EItem);
        break;
    case INVIS_TO:
        if (e.EItem && e.EItem->isItem())
            if (e.EActor->isMType(e.vDmg))
                e.EActor->IdentByTrial(e.EItem);
        break;
    case STAFF_SPELLS:
        e.EVictim->RecalcStaffSpells();
        break;     
    }

    if (e.EMagic->xval == AFRAID || e.EMagic->xval == CHARMED)
        e.EActor->Exercise(A_CHA,max(0,random(4)+e.EVictim->ChallengeRating()-e.EActor->ChallengeRating()),ECHA_CHARM,45);

    if (e.eID && (TEFF(e.eID)->Schools & SC_ENC))
        if (e.EVictim->isCreature() && (e.EMagic->sval == WILL))
            if (e.EVictim->HasAbility(CA_SLIPPERY_MIND)) {
                /* Here, we "schedule" a second saving throw versus
                the enchantment for people who have Slippery Mind;
                see Creature::StatiOff for the actual occurance
                of the second saving throw. */
                e.EVictim->GainTempStati(SLIP_MIND,NULL,2,SS_MISC,theGame->SpellNum(e.eID),e.saveDC,0);
            }

    return DONE;
}

bool isMetaEvent(int32 ev)
  {
    if (ev >= 40000)
      ev -= 40000;
    if (ev >= 20000)
      ev -= 20000;
    if (ev >= 10000)
      return true;
    return false;
  }

EvReturn Magic::Override(EventInfo &e)
	{
    int16 i; Annotation *a;
    if (e.isRemove || e.isLeave) {
      e.ETarget->RemoveEffStati(e.eID);
      e.Terse = true; 
      return DONE; 
      }
      
    for(a=RES(e.eID)->FAnnot();a;a=RES(e.eID)->NAnnot())  
      if (a->AnType == AN_EVENT) {
        for (i=0;i!=5;i++)
          if (a->u.ev[i].Event != 0 && 
              isMetaEvent(a->u.ev[i].Event)) {
            e.EVictim->GainTempStati(TRAP_EVENT, e.EActor, e.vDuration, 
              TEFF(e.eID)->HasFlag(EF_ONCEONLY) ? SS_ONCE : 
              TEFF(e.eID)->HasFlag(EF_CURSE)    ? SS_CURS : 
              TEFF(e.eID)->HasFlag(EF_MUNDANE)  ? SS_MISC : SS_ENCH, 
              a->u.ev[i].Event - 10000, e.vDmg, e.eID,
              e.vCasterLev + (e.MM & MM_FORTIFY ? 5 : 0));            
          } 
      }
    return DONE;
  }


EvReturn Magic::Polymorph(EventInfo &e)
  {
    rID mID; uint16 i;
    /* Choose Monster */

    /* Later, allow:
       * Limited Choice by flag
       * Complete Choice by flag
       * Limited Categories (Wild Shape)
       * Higher Level Monsters
       * etc.
    */
    if (!e.ETarget->isCreature()) {
      if (e.ETarget->isItem()) {
        /* Polypiling! */
        }
      return DONE;
      }

    if (e.EVictim->ResistLevel(AD_POLY) == -1)
      {
        e.Immune = true;
        return DONE;
      }


    int count = 0; 
    Restart:
    if (++count > 100) return ABORT; 
    if (e.EMagic->rval)
      { mID = e.EMagic->rval; goto SkipChecks; }
    else if (e.MM & MM_CONTROL)
      {
        int16 best, bestCR, n;
        rID Candidates[40];
        n = 3 + max(0,e.EActor->Mod(A_INT));
        for (i=0;i!=n;i++)
          Candidates[i] = theGame->GetMonID(PUR_POLY,max(0,
            e.EVictim->ChallengeRating()-4)/2,max(1,e.EVictim->ChallengeRating()/2),100,e.EMagic->yval);
        if (e.EActor->isMonster()) {
          bestCR = 0;
          best = 0;
          for(i=0;i!=n;i++)
            if (TMON(Candidates[i])->CR*(-1*(e.EVictim->isHostileTo(e.EActor))) > bestCR)
              {
                best = i;
                bestCR = TMON(Candidates[i])->CR*(-1*(e.EVictim->isHostileTo(e.EActor)));
              }
          }
        else if (e.EActor->isPlayer()) {
          for(i=0;i!=n;i++)
            e.EPActor->MyTerm->LOption(NAME(Candidates[i]),i);
          best = (int16)e.EPActor->MyTerm->LMenu(MENU_BORDER,"Choose a form:");
          }
        mID = Candidates[best];
      }
    else
      mID = theGame->GetMonID(PUR_POLY,max(0,e.EVictim->ChallengeRating()-4)/2,
                                       max(1,e.EVictim->ChallengeRating()/2),
                                             100,e.EMagic->yval);
    if (!mID)
      return ABORT;
    if (TMON(mID)->isMType(mID,MA_DRAGON))
      goto Restart;

    SkipChecks:
    SetSilence();
    e.EVictim->RemoveStati(POLYMORPH);
    UnsetSilence();

    if (!e.Terse && !e.EMagic->rval)
      VPrint(e,"You change into a <Str1>!",
        "The <EVictim> changes into a <Str1>!", NAME(mID));

    e.EVictim->Shapeshift(mID,e.EMagic->xval!=0,(e.EItem && e.EItem->isItem()) ? e.EItem : NULL);
    e.EVictim->GainTempStati(POLYMORPH,e.isItem ? e.EItem : NULL,e.vDuration,SS_ENCH,1,0,e.eID);

    if (e.EItem && e.EItem->isItem())
      e.EItem->VisibleID(e.EActor);

    if (TMON(e.EVictim->mID)->Imm & DF_STUN)
      e.EVictim->RemoveStati(STUNNED);
    if (TMON(e.EVictim->mID)->Imm & DF_CONF)
      e.EVictim->RemoveStati(CONFUSED);
    if (TMON(e.EVictim->mID)->Imm & DF_POIS)
      e.EVictim->RemoveStati(POISONED);
    if (TMON(e.EVictim->mID)->Imm & DF_DISE)
      e.EVictim->RemoveStati(DISEASED);
    if (TMON(e.EVictim->mID)->Imm & DF_BLND)
      e.EVictim->RemoveStati(BLIND);
    if (TMON(e.EVictim->mID)->Imm & DF_SLEE)
      e.EVictim->RemoveStati(ASLEEP);
    if (TMON(e.EVictim->mID)->Imm & DF_PLYS)
      e.EVictim->RemoveStati(PARALYSIS);
    if (TMON(e.EVictim->mID)->Imm & DF_CRIT)
      e.EVictim->RemoveStati(BLEEDING);
    if (TMON(e.EVictim->mID)->Imm & DF_FEAR)
      e.EVictim->RemoveStati(AFRAID);


    return DONE;
  }

bool TMonster::HasSlot(int16 sl)
  {
    static int8 SlotMFlags[] =
      {
        0,          /* INAIR */
        -M_NOHANDS, /* READY */
        -M_NOHANDS, /* WEAPON */
        -M_NOHANDS, /* LIGHT */
        M_HUMANOID, /* LSHOULDER */
        M_HUMANOID, /* RSHOULDER */
        -M_NOBODY,  /* BELT */
        -M_BLIND,   /* EYES */
        M_HUMANOID, /* CLOTHES */
        M_HUMANOID, /* ARMOUR */
        -M_NOLIMBS, /* BOOTS */
        -M_NOBODY,  /* CLOAK */
        -M_NOHANDS, /* LRING */
        -M_NOHANDS, /* RRING */
        -M_NOHEAD,  /* AMULET */
        -M_NOHANDS, /* GAUNTLETS */
        -M_NOHEAD,  /* HELM */
        -M_NOLIMBS, /* BRACERS */
        -M_NOBODY,  /* BELT1 */
        -M_NOBODY,  /* BELT2 */
        -M_NOBODY,  /* BELT3 */
        -M_NOBODY,  /* BELT4 */
        -M_NOBODY,  /* BELT5 */
        -M_NOBODY,  /* PACK */
        0, 0, 0 };
    if (sl == SL_ARCHERY) sl = SL_WEAPON;
    return (SlotMFlags[sl] > 0 && HasFlag(SlotMFlags[sl])) ||
          (SlotMFlags[sl] < 0 && !HasFlag(-SlotMFlags[sl]));
  }

void Creature::GainStatiFromBody(rID _mID)
{
  TMonster * n = TMON(_mID); // new
  
  RemoveStatiSource(SS_BODY); /* Remove all SS_BODY stati */
  if (isIllusion())
    return;
  
  for (int i=0;i<NUM_TMON_STATI;i++) {
    if (n->Stati[i].Nature) {
      GainPermStati(
          n->Stati[i].Nature,
          this,
          SS_BODY,
          n->Stati[i].Val,
          n->Stati[i].Mag,
          n->Stati[i].eID ? n->Stati[i].eID : _mID);
      } 
    }
  
  /* Druids keep infravision while wild-shaped. Skill
     bonuses should be kept too. */  
  n = TMON(tmID);
  if (tmID != _mID)
    for (int i=0;i<NUM_TMON_STATI;i++) {
      if (n->Stati[i].Nature == SKILL_BONUS || (n->Stati[i].Nature == 
          EXTRA_ABILITY && n->Stati[i].Val == CA_INFRAVISION &&
          !HasAbility(CA_INFRAVISION))) {
        GainPermStati(
            n->Stati[i].Nature,
            this,
            SS_BODY,
            n->Stati[i].Val,
            n->Stati[i].Mag,
            n->Stati[i].eID ? n->Stati[i].eID : _mID);
        } 
      }
    
  return; 
}

void Creature::GainStatiFromTemplate(rID _tID, bool turn_on)
{

  if (isIllusion())
    return;
  ASSERT(_tID != FIND("unimplemented"));
  if (turn_on) {
    TTemplate * n = TTEM(_tID);
    for (int i=0;i<NUM_TMON_STATI;i++) {
      if (n->Stati[i].Nature) {
        ASSERT(! n->Stati[i].eID );
        ASSERT(n->Stati[i].Nature != TEMPLATE);
        GainPermStati(
            n->Stati[i].Nature,
            this,
            SS_TMPL,
            n->Stati[i].Val,
            n->Stati[i].Mag,
            n->Stati[i].eID ? n->Stati[i].eID : _tID);
      } 
    }
  } else {
    RemoveEffStati(_tID,EV_REMOVED,TEMPLATE);
  } 
  return; 
}

void Creature::Planeshift()
  {
    Item *it;
    for (it=FirstInv();it;it=NextInv())
      if (it->onPlane() != onPlane())
        if (!it->isGhostTouch() && !HasStati(MANIFEST))
          {
            it->Remove(false);
            it->PlaceAt(m,x,y);
            IDPrint("Your <Obj> falls through your body and lands at your feet.",
                    "The <Obj2>'s <Obj1> falls through <his:Obj2> body and lands at "
                    "<his:Obj2> feet.", it, this);
          } 
          
    /* No manifesting mounts, yet... */
    if (HasStati(MOUNTED) && !HasStati(MANIFEST))
      if (onPlane() != ((Creature*)GetStatiObj(MOUNTED))->onPlane())
        ThrowVal(EV_DISMOUNT,DSM_INCOR,this);
}

void Creature::Shapeshift(rID _mID, bool merge, Item* PolySource) {
    TMonster *c, *n; Item *it;

    if (tmID != mID && tmID != _mID) {
        SetSilence();
        RemoveStati(POLYMORPH);
        UnsetSilence();
    }

    if (HasStati(MOUNTED)) {
        IPrint("You are thrown from your mount as you change shape!");
        ThrowVal(EV_DISMOUNT, DSM_THROWN, this);
        if (isDead())
            return;
    }

    if (HasStati(DISGUISED)) {
        IPrint("Your transformation ruins your disguise!");
        RemoveStati(DISGUISED);
    }


    GainStatiFromBody(_mID);

    if (_mID == mID) {
        /* New Self... */
        return;
    }

    n = TMON(_mID); // new
    c = TMON(mID);

    if (n->Size > c->Size) {
        static int8 SizedSlots[] =
        { SL_ARMOUR, SL_GAUNTLETS, SL_BRACERS, SL_BOOTS, 0 };
        /* If In Armour, either
         * It Breaks
         * It Sizes Automatically
         * It "phases away" (artifacts)
         * You take damage/die
         */
    }
    if (n->Size < c->Size)
    {

    }

    bool incor_msg = false;
    if (n->HasFlag(M_INCOR) && !merge) {
        for (int8 i = 0; i != SL_LAST; i++)
            if (it = InSlot(i))
                if (it != PolySource) {
                    if (it->isGhostTouch())
                        continue;
                    if (!incor_msg) {
                        IDPrint("Your equipment falls through your body!",
                            "<Obj1>'s equipment falls through <his:Obj1> body!", this);
                        incor_msg = true;
                    }
                    Throw(EV_DROP, this, NULL, it);
                } else if (n->HasFlag(M_AMORPH) && !merge)
                    for (int8 i = 0; i != SL_LAST; i++)
                        if (InSlot(i))
                            if (InSlot(i) != PolySource) {
                                if (!incor_msg) {
                                    IDPrint("Your equipment flows off your body!",
                                        "<Obj1>'s equipment flows off <his:Obj1> body!", this);
                                    incor_msg = true;
                                }
                                Throw(EV_DROP, this, NULL, InSlot(i));
                            }
        Planeshift();
    }

    /* Effects of being an ooze/slime/fungi and touching your gear... */


    /* An array listing the MFlag you must have in order to be able
       to have an item in the slot of the index. Negative values mean
       you must *not* have the flag in question. */

    for (int8 i = 0; i != SL_LAST; i++)
        if (InSlot(i) && !n->HasSlot(i)) {
            it = InSlot(i);
            if (it == PolySource || merge || it->isType(T_LIGHT)) {
                IDPrint("Your <Obj1> merge<s:Obj1> with your new form.",
                    "The <Obj2>'s <Obj1> merge<s:Obj1> with <his:Obj2> new form.",
                    it, this);
            } else {
                IDPrint("Your <Obj1> fall<str3> free.",
                    "The <Obj2>'s <Obj1> fall<str3> free.", it, this,
                    it->isPlural() ? "" : "s");
                Throw(EV_LOSEITEM, this, NULL, it);
                Throw(EV_DROP, this, NULL, it);
            }
        }

    if (isMonster())
        if (theGame->GetPlayer(0)->Perceives(this))
            StateFlags |= MS_POLY_KNOWN;

    mID = _mID;
    SetImage();
    cHP = (cHP * 100) / max(1, mHP + Attr[A_THP]);
    CalcHP();
    cHP = max((cHP*(mHP + Attr[A_THP])) / 100, 1);
    if (m && m->InBounds(x, y)) // ww: bugfix! 
        m->Update(x, y);
}

/* HACKFIX */
EvReturn Magic::Dispel(EventInfo &e) {
    static rID Resisted[512], Dispelled[512];
    hObj   oResisted[512], oDispelled[512]; 
    int16 eCasterLev, rc, dc, j, bon; 
    bool  Something, msg;
    Item *it;

    bon = e.EActor->AbilityLevel(CA_PURGE_MAGIC);

    if (e.EMagic->xval & DIS_BANISH)
      if (e.EVictim->HasStati(SUMMONED)) {
          if (e.vDmg + bon >= 11+e.EVictim->GetStatiMag(SUMMONED)) {
              VPrint(e,NULL,"The <EVictim> winks out of existence.");
              // ww: it's really annoying when your summoned celestial elf
              // picks up Stormbringer and then disappears with it
              e.EVictim->DropAll();
              e.EVictim->Remove(true);
              return DONE;
            }
        }

    rc = 0; dc = 0; Something = false; msg = false;
    e.EMap->SetQueue(QUEUE_DISPEL_MSG);
    if (e.EMagic->xval & DIS_DISPEL) {
      StatiIter(e.EVictim)
          it = NULL;
          if (!(S->Source == SS_ENCH || S->Source == SS_ITEM || S->Source == SS_ONCE || S->Source == SS_ATTK))
            continue;

          if (!S->eID)
            continue;
          if (TEFF(S->eID)->Type != T_TEFFECT)
            continue;

          if (TEFF(S->eID)->ef.aval == AR_POISON || TEFF(S->eID)->ef.aval == AR_DISEASE)
            continue;
            
          if (TEFF(S->eID)->HasFlag(EF_MUNDANE))
            continue;

          for (j=0;j!=dc;j++)
            if (Dispelled[j] == S->eID && oDispelled[j] == S->h) {
                StatiIter_DispelCurrent(e.EVictim);
                continue;
              }
          for(j=0;j!=rc;j++)
            if (Resisted[j] == S->eID && oResisted[j] == S->h)
              goto NextStati;
          
          eCasterLev = S->CLev;

          if (S->Source == SS_ITEM)
            it = oItem(S->h);
          else
            it = NULL;

          if (!eCasterLev)
            eCasterLev = e.EVictim->ChallengeRating();
          
          if (eCasterLev+11 > e.vDmg+bon) {
              e.Resist = true;
              oResisted[rc] = S->h;
              Resisted[rc++] = S->eID;
              
              if (rc >= 512)
                StatiIterBreakout(e.EVictim,return DONE)
              continue;
            }
            
          oDispelled[dc] = S->h;
          Dispelled[dc++] = S->eID;
          if (dc > 512)
            StatiIterBreakout(e.EVictim,return DONE)
              
          if (it) {
            it->GainTempStati(DISPELLED,NULL,e.vDuration,SS_MISC,0,0,e.eID);
            if (!it->isKnown(KN_MAGIC))
              goto SkipMessage;
            }

          Something = true;
          if (!it)
            e.EVictim->IPrint("Your <5><Res><7> spell is dispelled.",S->eID);
          if (it || e.EActor->SkillCheck(SK_SPELLCRAFT,15+TEFF(S->eID)->Level)) {
              if (it)
                TPrint(e,e.EActor == e.EVictim ? NULL :
                  "The <EVictim>'s <Obj> is surrounded with a dull grey aura.",
                  "Your <Obj> is surrounded with a dull grey aura.",
                  "The <EVictim>'s <Obj> is surrounded with a dull grey aura.",it);
              else if (e.EActor != e.EVictim)
                e.EActor->IPrint("The <Obj>'s <5><Res><7> spell is dispelled.", e.EVictim,S->eID);
              msg = true;
            }

SkipMessage:
          StatiIter_DispelCurrent(e.EVictim);

NextStati:;
      StatiIterEnd(e.EVictim)
    }
    e.EMap->UnsetQueue(QUEUE_DISPEL_MSG);

    if (e.EVictim->isPlayer() || (e.EItem && e.EItem->isItem()) && e.EItem->isKnown(KN_MAGIC)) {
      if (e.Resist)
        TPrint(e,e.EActor == e.EVictim ? NULL :
                 "The <EVictim> <Str1>resists the dispelling.",
                 "You <Str1>resist the dispelling.", NULL,
                 Something ? "partially " : "");
      if (!msg && Something && (e.EActor != e.EVictim))
        e.EActor->IPrint("Some of the <Obj>'s magic was dispelled.",e.EVictim);
      if (Something || e.Resist)
        e.isSomething = true;
      e.EMap->PrintQueue(QUEUE_DISPEL_MSG);

      if ((e.EItem && e.EItem->isItem()))
        e.EItem->VisibleID(e.EActor);

      }
                      
    return DONE;
}

EvReturn Magic::Reveal(EventInfo &e) {
    Corpse* theCorpse; uint8 tlev; int16 i,d; String Text;
    Thing *o; int8 high; int16 gauge; bool DoneBenefit;
    Map *m = e.EActor->m; Thing *t; 
    tlev = e.vCasterLev; Text = "";
    
    if (TEFF(e.eID)->HasFlag(EF_NEEDS_CORPSE) && !e.EItem) {
      MapIterate(m,t,i)
        if (t->x == e.EActor->x)
          if (t->y == e.EActor->y)
            if (t->Type == T_CORPSE)
              { theCorpse = (Corpse*)t; goto FoundCorpse; }
      e.EActor->IPrint("The spell fails -- you need to be standing over a fresh corpse for it to function properly.");
      return DONE;

FoundCorpse:
      //tlev = (EffectLevel() + TMON(theCorpse->mID)->Level + 1) / 2;
      DPrint(e,NULL, "The <EActor> cuts open the <Obj> and reads auguries from its entrails.");
      Text += SC("You cut open the ") + theCorpse->Name() + SC("and read auguries from its entrails.  ");
      }

    if (e.EItem && e.EItem->isItem())
      if (!e.EItem->isKnown(KN_MAGIC)) {
        e.EActor->IPrint("You receive knowledge magically...");
        e.EActor->IdentByTrial(e.EItem);
      }

    /*
    if (m->MapType() == M_TOWN)
      {
        Text += "The sundry adversities and benefits a city can provide "
          "are far beyond the ability of a simple spell to predict.");
        goto DoneAugury;
      }
    */


    m->PreviousAuguries++;

    if (m->PreviousAuguries*4 > tlev)
      {
        Text += "The weave strained by prior divination, you are only "
          "able to glean vague and unclear signs. ";
        goto DoneAugury;
      }

    DoneBenefit = false;

    high = 0; o = NULL;
    MapIterate(m,t,i)
      if (t->isCreature())
        // if (((Creature*)t)->isHostileTo(e.EActor))
        if (((Creature*)t)->ChallengeRating() > high) {
            o = t;
            high = (int8)((Creature*)o)->ChallengeRating();
        }
    if (o == NULL)
      {
        Text += "Little adversity awaits you on this conquest. ";
        goto DoneAdversity;
      }

    /* Later: (o->ChallengeRating() vs. m->DungeonLevel())
       a moderate threat
       a serious risk
       a great danger
       a legendary challenge
       a nigh-invincible enemy
    */

    Text += "The greatest adversity lies ";

    DescribeO:

    if (TEFF(e.eID)->HasFlag(EF_TELL_DISTANCE))
      {
        gauge = (m->SizeX() + m->SizeY()) / 10;
        d = dist(e.EActor->x,e.EActor->y,o->x,o->y);
        d = (d * 100) / gauge;
        if (d > 70)
          Text += "a great span ";
        else if (d > 60)
          Text += "very far ";
        else if (d > 40)
          Text += "far ";
        else if (d > 30)
          Text += "a moderate distance ";
        else if (d > 20)
          Text += "close by to the ";
        else if (d > 10)
          Text += "very close by to the ";
        else
          Text += "shockingly near, to the ";
      }
    if (abs(o->x - e.EActor->x) > abs(o->y - e.EActor->y)*3)
      {
        if (o->x > e.EActor->x)
          Text += "east";
        else
          Text += "west";
      }
    else if (abs(o->y - e.EActor->y) > abs(o->x - e.EActor->x)*3)
      {
        if (o->y > e.EActor->y)
          Text += "south";
        else
          Text += "north";
      }
    else switch(e.EActor->DirTo(o))
      {
        case NORTHEAST: Text += "north-east"; break;
        case NORTHWEST: Text += "north-west"; break;
        case SOUTHEAST: Text += "south-east"; break;
        case SOUTHWEST: Text += "south-west"; break;
      }
    /*if (TEFF(e.eID)->HasFlag(EF_SPECIFIC))
      Text += XPrint("; you are granted a brief vision of an <Obj>",o);*/
    Text += ". ";

    if (DoneBenefit)
      goto DoneAugury;

    DoneAdversity:

    high = 0; o = NULL;
    MapIterate(m,t,i)
      if (t->isCreature()) {
        // later, scan creatures' inventories
      } else if (((Item*)m->Things[i])->ItemLevel() > high) {
          o = t;
          high = (int8)((Item*)o)->ItemLevel();
      }

    if (o == NULL)
      {
        Text += "Little benefit awaits you on this endeavour. ";
        goto DoneAdversity;
      }


    /* Later: (o->ChallengeRating() vs. m->DungeonLevel())
       a moderate profit
       a potent reward
       a great benefice
       a legendary object
       an unearthly discovery
    */

    Text += "The greatest prosperity lies ";

    DoneBenefit = true;
    goto DescribeO;

    DoneAugury:

    Text += "So it is foretold.";

    if (e.EActor->Type == T_PLAYER)
     ((Player*)e.EActor)->MyTerm->Box(WIN_SCREEN,0,MAGENTA,GREY,Text);
    return DONE;
  }

extern Creature* CandidateCreatures[2048];

EvReturn Magic::Summon(EventInfo &e) {
    String opt[20], desc[20];
    Creature *cr; int16 i, j, n, k;

    if (!e.isLoc) {
        e.EXVal = e.ETarget ? e.ETarget->x : e.EActor->x;
        e.EYVal = e.ETarget ? e.ETarget->y : e.EActor->y;
    }

    /* Note that the Depth is still dependant upon the actual map;
       the player will never meet a ghoul at 50', even if Asomdeus
       himself is doing the summoning. */

    if (TEFF(e.eID)->HasFlag(EF_XSUMMON)) {
        for (i=0;e.EActor->backRefs[i];i++)
            if (theRegistry->Exists(e.EActor->backRefs[i]))
                if (!oThing(e.EActor->backRefs[i])->isDead())
                    if (oThing(e.EActor->backRefs[i])->HasEffStati(SUMMONED,e.eID)) {
                        ReThrow(MSG_XSUMMON,e);
                        return ABORT;
                    }
    }              
    
    if (e.MM & MM_CONTROL) {
      Creature *cr[20][20];
      memset(cr,0,20*20*sizeof(Creature*));
      n = 5 + e.EActor->Mod(A_INT); 
      for (i=0;i!=n;i++)
        {
          memset(CandidateCreatures,0,2048*sizeof(Creature*));
          XTHROW(EV_ENGEN,e,
            xe.enFlags = EN_SUMMON;
            xe.enFlags |= TEFF(e.eID)->HasFlag(EF_MULTIPLE) ? 0 : EN_SINGLE;
            xe.enCR = e.vDmg;
            xe.enDepth = e.EMap->Depth;
            xe.enConstraint = e.EMagic->xval;
            if (!e.EMagic->rval)
              xe.enRegID = e.eID;
            else if (RES(e.EMagic->rval)->Type == T_TENCOUNTER)
              xe.enID = e.EMagic->rval;
            else if (RES(e.EMagic->rval)->Type == T_TMONSTER)
              {
                xe.enConstraint = e.EMagic->rval;
                xe.enID = FIND("Generic Summons");
              }
            else
              xe.enRegID = e.EMagic->rval;
            if (!e.isLoc) {
              xe.isLoc = true;
              xe.EXVal = e.ETarget ? e.ETarget->x :  e.EActor->x;
              xe.EYVal = e.ETarget ? e.ETarget->y :  e.EActor->y;
              }
            );
          for (j=0;CandidateCreatures[j];j++)
            {
              if (j >= 20)
                {
                  CandidateCreatures[j]->Remove(true);
                  continue;
                }
              cr[i][j] = CandidateCreatures[j];
              if (j == 0)
                opt[i] = cr[i][j]->Name(NA_A);
              else if (j == 1)
                opt[i] += "...";
              if (j)
                desc[i] += ", ";
              desc[i] += cr[i][j]->Name(NA_A);
            }
          T1->LOption(opt[i],i,desc[i]);
        }
          
      k = (int16)T1->LMenu(MENU_DESC|MENU_BORDER,"Choose Summoned Creatures:");
      for (i=0;i!=n;i++)
        for (j=0;cr[i][j];j++)
          {
            if (i == k) { 
              if (e.isLoc)
                cr[i][j]->PlaceAt(e.EMap,e.EXVal,e.EYVal);
              else 
                cr[i][j]->PlaceAt(e.EMap,e.EActor->x,e.EActor->y);
              cr[i][j]->Initialize(true);
              }
            else
              cr[i][j]->Remove(true);
          }      
      }
    else {
      XTHROW(EV_ENGEN,e,
        xe.enFlags = EN_SUMMON;
        xe.enFlags |= TEFF(e.eID)->HasFlag(EF_MULTIPLE) ? 0 : EN_SINGLE;
        xe.enCR = e.vDmg;
        xe.enDepth = e.EMap->Depth;
        xe.enConstraint = e.EMagic->xval;
        if (!e.EMagic->rval)
          xe.enRegID = e.eID;
        else if (RES(e.EMagic->rval)->Type == T_TENCOUNTER)
          xe.enID = e.EMagic->rval;
        else if (RES(e.EMagic->rval)->Type == T_TMONSTER)
          {
            xe.enConstraint = e.EMagic->rval;
            xe.enID = FIND("Generic Summons");
          }
        else
          xe.enRegID = e.EMagic->rval;
        if (!e.isLoc) {
          xe.isLoc = true;
          xe.EXVal = e.ETarget ? e.ETarget->x :  e.EActor->x;
          xe.EYVal = e.ETarget ? e.ETarget->y :  e.EActor->y;
          }
        );
      if (!e.EMap->GetEncounterCreature(0))
        APrint(e, "[ Summon failed in EV_ENGEN ]");
      }
      
    if (e.MM & MM_AUGMENT)
      if (TEFF(e.eID)->Schools & SC_WEA)
        {
          Creature *cr;
          for (i=0;cr = e.EMap->GetEncounterCreature(i);i++)
            {
              cr->GainPermStati(ADJUST,e.EActor,SS_ENCH,A_STR,+4,e.eID,e.vCasterLev);
              cr->GainPermStati(ADJUST,e.EActor,SS_ENCH,A_CON,+4,e.eID,e.vCasterLev);
              cr->GainPermStati(ADJUST,e.EActor,SS_ENCH,A_THP,
                    10 + max(1,TEFF(e.eID)->Level)*5,e.eID,e.vCasterLev);
            } 
        }
    if (e.EItem && e.EItem->isItem())
      e.EActor->IdentByTrial(e.EItem);
    
    e.EActor->Exercise(A_CHA,random(4)+1,ECHA_SUMMON,30);
    
    for (i=0;cr=e.EMap->GetEncounterCreature(i);i++)
      cr->IDPrint(NULL,"An <Obj> appears!", cr);
    
    return DONE;
  }

EvReturn Magic::Terraform(EventInfo &e)
  {                     
    Creature *cr;
    // WW: I'm not sure what the "Static" is doing here, but it is *really
    // buggy* -- if you cast a terraform spell on level 1 and then walk
    // downstairs to level 2, these static variables will still have the
    // level 1 map stored! This really happens!
    Map *m = NULL; 
    uint8 x,y,i;

    /*if (e.EXVal == e.EActor->x-2 && e.EYVal == e.EActor->y-2)
      __asm int 3;*/

    if (!e.EXVal)
      {
        if (e.isDir) {
          e.EXVal = e.ETarget->x + DirX[e.EDir];
          e.EYVal = e.ETarget->y + DirY[e.EDir];
          }
        else {
          e.EXVal = e.ETarget->x;
          e.EYVal = e.ETarget->y;
          }
      }
    x = (uint8)e.EXVal;
    y = (uint8)e.EYVal;
    if (!e.EActor) return ABORT; 
    m = e.EActor->m;
    if (!m) return ABORT; 
    TTerrain * ct = TTER(m->TerrainAt(x,y));

    switch(e.EMagic->xval) {
      case TERRA_EMPTY:
        if (m->SolidAt(x,y) || m->FCreatureAt(x,y))
          return ABORT;
       break;
      case TERRA_ROCK:
        i = ct->Material;
        if (i != MAT_GRANITE && i != MAT_MAGMA && i != MAT_QUARTZ
             && i != MAT_MINERAL)
          return ABORT;
        if (!m->SolidAt(x,y))
          return ABORT;
       break;
      case TERRA_ALL:
       break;
      case TERRA_FLOOR:
        if (!m->SolidAt(x,y) && 
            (GLYPH_ID_VALUE(m->At(x,y).Glyph) == GLYPH_FLOOR || 
			GLYPH_ID_VALUE(m->At(x, y).Glyph) == GLYPH_FLOOR2))
          break;
        return ABORT;
      case TERRA_OPEN:
        if (m->SolidAt(x,y))
          return ABORT;
       break;
      case TERRA_CLOSED:
        if (!m->SolidAt(x,y))
          return ABORT;
       break;
      case TERRA_WATER:
        if (ct->HasFlag(TF_WATER))
          break;
       return ABORT;
      case TERRA_CHASM:
        if (ct->Material == MAT_EMPTYNESS)
          break;
       return ABORT;
      case TERRA_BRIDGE:
        if (m->FallAt(x,y))
          break;
        if (TTER(m->TerrainAt(x,y))->HasFlag(TF_DEEP_LIQ))
          break;
       return ABORT;
      }
    e.isSomething = true;

    // ww: that field/terraform problem with the result showing up
    // on the level above is hard to track down ...
    // Wed Feb 18 21:58:41 PST 2004
    if (m != theGame->GetPlayer(0)->m) {
      ASSERT(0); 
      } 
    TTerrain * tt = TTER(e.EMagic->rval);
    if (!tt)
      { Error("EA_TERRAFORM has blank rval!");
        return ABORT; }
        
    /* Avoid double-writing terraform effects -- once for the
       target's location and once on MagicXY -- since this is
       the root of the tanglefoot bag center square bug. */
    if (tt == ct)
      return DONE;
        
    if (tt->Type == T_TFEATURE) {
      Feature *ft = new Feature(TFEAT(e.EMagic->rval)->Image,
          e.EMagic->rval,T_FEATURE);
      if (ft) {
        ft->GainTempStati(SUMMONED,e.EActor,e.vDuration,SS_ENCH,1,1,e.eID);
        ft->PlaceAt(m,x,y);               
      }
    } else {   

    if (e.terraKey == 0) {
      e.terraKey   = ++(m->nextAvailableTerraKey);
      TerraRecord * tr; 
      tr = m->TerraList.NewItem();
      tr->key      = e.terraKey;
      tr->SaveDC   = e.EMagic->sval == NOSAVE ? 0 : e.saveDC;
      tr->Duration = TEFF(e.eID)->HasFlag(EF_PERMANANT) ? 0 : e.vDuration;
      tr->eID      = e.eID;
      tr->Creator  = e.EActor->myHandle;
      if (e.MM & MM_MAXIMIZE)
        tr->pval.Set(0,0,e.Dmg.Number*e.Dmg.Sides+e.Dmg.Bonus);
      else
        tr->pval  = e.Dmg; 

      for(i=0;i!=255 && m->TerrainList[i] && 
           m->TerrainList[i] != e.EMagic->rval;i++)
        ;
      if (i == 255) { Error("Too much Terrain!"); return DONE; }
      m->TerrainList[i] = e.EMagic->rval; 
      e.terrainListIndex = i; 
      e.doVUpdate = true;   
    } 
     
    MTerrain *mt;
    mt = m->TerraXY.NewItem();
    mt->x = (uint8)e.EXVal;
    mt->y = (uint8)e.EYVal;
    mt->old = m->At(x,y).Terrain;
    mt->key = e.terraKey;

    m->At(x,y).Terrain = e.terrainListIndex;

    if (TEFF(e.eID)->HasFlag(EF_ADJUST_GLYPH))
      m->At(x,y).Glyph   = AdjustGlyphDir(tt->Image,(Dir)e.EDir);
    else  
      m->At(x,y).Glyph   = tt->Image;
    
    m->At(x,y).Solid   = tt->HasFlag(TF_SOLID);
    m->At(x,y).Special = tt->HasFlag(TF_SPECIAL);
    m->At(x,y).Opaque  = tt->HasFlag(TF_OPAQUE);
    m->At(x,y).Obscure = tt->HasFlag(TF_OBSCURE);
    m->At(x,y).Shade   = tt->HasFlag(TF_SHADE);      
    m->At(x,y).isWall  = tt->HasFlag(TF_WALL);      
    
    if (tt->HasFlag(TF_SOLID)) {
      for (cr = m->FCreatureAt(x,y);cr;cr=m->NCreatureAt(x,y))
        if (!cr->HasMFlag(M_INCOR))
          cr->PlaceNear(x,y);
      }
    
    for (cr=m->FCreatureAt(x,y);cr;cr=m->NCreatureAt(x,y))
      {
        TTER(m->TerrainAt(x,y))->PEvent(EV_ENTER,cr,m->TerrainAt(x,y));
        cr->TerrainEffects();
      }
    
    }

    if (e.EItem && e.EItem->isItem())
      e.EItem->VisibleID(e.EActor);

    return DONE;
  }

void Map::UpdateTerra()
  {
    int16 i;
    RestartTerra:
    for(i=0;i!=TerraList.Total();i++) {
      if (TerraList[i]->Duration == 1)
        { RemoveTerra(TerraList[i]->key); goto RestartTerra; }
      else if (TerraList[i]->Duration > 1)
        TerraList[i]->Duration--;
      }
  }

// ww: who made that Wall of Fire? 
Creature * Map::GetTerraCreator(int16 x,int16 y)
  {
    int32 i,j;
    if (!InBounds(x,y)) return NULL; 
    for(i=(TerraXY.Total()-1);i>=0;i--)
      if (TerraXY[i]->x == x)
        if (TerraXY[i]->y == y) {
          for (j=0;j!=TerraList.Total();j++)
            if (TerraList[j]->key == TerraXY[i]->key) {
              if (isValidHandle(TerraList[j]->Creator))
              return oCreature(TerraList[j]->Creator);
              else return NULL;
            } 

          Error("TerraXY entry without matching TerraList entry (key %d)",TerraXY[i]->key);
        }
    return NULL;
  }


int8 Map::GetTerraDC(int16 x,int16 y)
  {
    int32 i,j;
    for(i=(TerraXY.Total()-1);i>=0;i--)
      if (TerraXY[i]->x == x)
        if (TerraXY[i]->y == y) {
          for (j=0;j!=TerraList.Total();j++)
            if (TerraList[j]->key == TerraXY[i]->key)
              return TerraList[j]->SaveDC;
          Error("TerraXY entry without matching TerraList entry (key %d)",TerraXY[i]->key);
          }
    return 15;
  }

int8 Map::GetTerraDType(int16 x,int16 y)
  {
    int32 i,j;
    for(i=(TerraXY.Total()-1);i>=0;i--)
      if (TerraXY[i]->x == x)
        if (TerraXY[i]->y == y) {
          for (j=0;j!=TerraList.Total();j++)
            if (TerraList[j]->key == TerraXY[i]->key)
              return TerraList[j]->DType;
          Error("TerraXY entry without matching TerraList entry (key %d)",TerraXY[i]->key);
          }
    return AD_NORM;
  }


int16 Map::GetTerraDmg(int16 x, int16 y)
  {
    int32 i,j;
    for(i=(TerraXY.Total()-1);i>=0;i--)
      if (TerraXY[i]->x == x)
        if (TerraXY[i]->y == y) {
          for (j=0;j!=TerraList.Total();j++)
            if (TerraList[j]->key == TerraXY[i]->key)
              return TerraList[j]->pval.Roll();
          Error("TerraXY entry without matching TerraList entry (key %d)",TerraXY[i]->key);
          }
    return 10;
  }

void Map::RemoveTerra(int16 key)
  {
    int16 i,j,x,y; TTerrain *t;
    /* This is an ugly way to do this, that can end up hashing the
       heap a lot if several different terraforms are active. But
       usually the waste (in speed and hashing) is trivial compared
       to the more memory or calculation intensive parts of the game.
       How many /wall of fire/s are going to be active at any one 
       time, anyway? */
    Restart1:
    for(i=0;i!=TerraList.Total();i++)
      if (TerraList[i]->key == key)
        {
          EventInfo e;
          e.Clear();
          e.Event = EV_ELAPSED;
          e.EXVal = 0; e.EYVal = 0;
          for(j=0;j!=TerraXY.Total();j++)
            if (TerraXY[j]->key == key)
              {
                e.EXVal = TerraXY[j]->x; 
                e.EYVal = TerraXY[j]->y;
                break;
              }
          TEFF(TerraList[i]->eID)->Event(e,TerraList[i]->eID);
          TerraList.Remove(i);
          goto Restart1;
        }

    Restart2:
    for(i=0;i!=TerraXY.Total();i++)
      if (TerraXY[i]->key == key)
        {
          t = TTER(TerrainList[TerraXY[i]->old]);
          x = TerraXY[i]->x; y = TerraXY[i]->y; 
          At(x,y).Glyph   = t->Image;
          At(x,y).Solid   = t->HasFlag(TF_SOLID);
          At(x,y).Special = t->HasFlag(TF_SPECIAL);
          At(x,y).Opaque  = t->HasFlag(TF_OPAQUE);
          At(x,y).Obscure = t->HasFlag(TF_OBSCURE);
          At(x,y).Shade   = t->HasFlag(TF_SHADE);      
          At(x,y).isWall  = t->HasFlag(TF_WALL); 
          At(x,y).Terrain = TerraXY[i]->old;     
          Update(x,y);
          TerraXY.Remove(i);
          goto Restart2;
        }
  }

void Map::RemoveTerraXY(int16 x, int16 y,rID xID)
  {
    int32 i,j;
    Restart:
    for(i=0;TerraXY[i];i++)
      if (TerraXY[i]->x == x && TerraXY[i]->y == y)
        {
          for(j=0;TerraList[j];j++)
            if (TerraXY[i]->key == TerraList[j]->key) 
              if (xID && TerraList[j]->eID != xID)
                goto SkipThisTerra;
          RemoveTerra(TerraXY[i]->key);
          goto Restart;
          SkipThisTerra:
          ;
        }
  }

void Map::WriteTerra(int16 x, int16 y,rID terID) {
    static uint8 lastTerrain = 0; 
    uint8 TerrainVal, i; TTerrain *t;
    uint16 TorchPos; Creature *cr;

    if (!InBounds(x,y))
        return;

    if (TTER(terID)->Type == T_TTERRAIN)
        ;
    else if (TTER(terID)->Type == T_TFEATURE) {
        Feature *ft = new Feature(TFEAT(terID)->Image,terID,T_FEATURE);
        if (ft)                                
            ft->PlaceAt(this,x,y);               
        return ;
    } else {
        ASSERT(0); 
    } 


    if (TTER(terID)->HasFlag(TF_SOLID)) 
        for (cr = FCreatureAt(x,y);cr;cr=NCreatureAt(x,y)) {
            At(x,y).Solid = true;
            cr->PlaceAt(this,x,y);
        }

    if (TerrainList[lastTerrain] == terID)
        TerrainVal = lastTerrain;
    else {
        for(i=0;TerrainList[i];i++)
            if (TerrainList[i] == terID)
            { TerrainVal = i; goto FoundTerrain; }
            if (i == 255)
                Fatal("Too many Terrain types on one map!");
            TerrainVal = i;
            TerrainList[i++] = terID;
FoundTerrain:;
    }

    At(x,y).Terrain = TerrainVal;
    t = TTER(terID);
    At(x,y).Glyph   = t->Image;
    At(x,y).Solid   = t->HasFlag(TF_SOLID);
    At(x,y).Special = t->HasFlag(TF_SPECIAL);
    At(x,y).Opaque  = t->HasFlag(TF_OPAQUE);
    At(x,y).Obscure = t->HasFlag(TF_OBSCURE);
    At(x,y).Shade   = t->HasFlag(TF_SHADE);      
    At(x,y).isWall  = t->HasFlag(TF_WALL);      

    // ww: summoning circles throw up magic fields, etc. 
    if (t) { 
        if (t->HasFlag(TF_SPECIAL)) {
            EventInfo e; 
            e.Clear();
            e.EMap = this;
            e.EXVal = x;
            e.EYVal = y;
            e.Event = EV_INITIALIZE;
            t->Event(e,terID);
        }
        if (t->HasFlag(TF_TORCH)) {
            TorchPos = x+y*256;
            TorchList.Add(TorchPos);
        }
    }

    Update(x,y);
}


EvReturn Magic::Illusion(EventInfo &e)
  {
    rID spList[64]; EvReturn r;
    char ch; rID spID; Thing *il; int16 i;
    
    // avoid inexplicable crashes until
    // the illusion system is stable.
    if (!e.EActor->isPlayer())
      return DONE;
    
    if (e.EItem && e.EItem->isItem())
      e.EActor->IdentByTrial(e.EItem);
    
    if (TEFF(e.eID)->HasFlag(EF_IMONSTERS))
      ch = 'c';
    else if (TEFF(e.eID)->HasFlag(EF_IBLAST))
      ch = 'f';
    else {
      do 
        ch = e.EActor->ChoicePrompt(
          "Create illusionary creature, item, trap or force?",
          "citf","citf"[random(4)]);
      while (ch == -1);
      }
    
        
    e.illType = ch;

    if (e.EActor->isPlayer() && (ch != 'f'))
      {
        if (!T1->EffectPrompt(e,Q_LOC,false,"Target Illusion"))
          return ABORT;        
      }

    switch (ch)
      {
        case 'c':
          for(i=0;e.EActor->backRefs[i];i++)
            if (theRegistry->Exists(e.EActor->backRefs[i]))
              if ((il = oThing(e.EActor->backRefs[i]))->isIllusion())
                if (il->GetStatiObj(ILLUSION) == e.EActor)
                  if (il->isCreature())
                    {
                      e.EActor->IPrint("You can only have one illusory creature at a time.");
                      if (!e.EActor->yn(XPrint("Cancel the <Obj>?",il)))
                        return ABORT;
                      il->Remove(true);
                    }       
        
          if (!e.isLoc) {
            e.EXVal = e.ETarget ? e.ETarget->x : e.EActor->x;
            e.EYVal = e.ETarget ? e.ETarget->y : e.EActor->y;
            }
            
          XTHROW(EV_ENGEN,e,
            xe.enFlags = EN_ILLUSION; /* HACKFIX */
            xe.enFlags |= TEFF(e.eID)->HasFlag(EF_MULTIPLE) ? 0 : EN_SINGLE;
            xe.enCR = e.vDmg;
            xe.enDepth = e.EMap->Depth;
            xe.enConstraint = e.EMagic->xval;

            if (!e.EMagic->rval)
              xe.enRegID = e.eID;
            else if (RES(e.EMagic->rval)->Type == T_TENCOUNTER)
              xe.enID = e.EMagic->rval;
            else if (RES(e.EMagic->rval)->Type == T_TMONSTER)
              {
                xe.enConstraint = e.EMagic->rval;
                xe.enID = FIND("Generic Summons");
              }
            else
              xe.enRegID = e.EMagic->rval;
            );

          
          if ((e.EItem && e.EItem->isItem()))
            e.EActor->IdentByTrial(e.EItem);
          
          return DONE;
         break;
        case 'i':
          Item *it;
          it = Item::GenItem(IG_GREAT,0,e.EActor->SkillLevel(SK_ILLUSION),10,IllusionItems);
          it->GainTempStati(ILLUSION,e.EActor,e.vDuration, SS_ENCH,e.saveDC, 0,
                              e.eID, e.vCasterLev);
          it->PlaceAt(e.EMap, e.EXVal, e.EYVal);          
          return DONE;
        case 'f':
          if (!TEFF(e.eID)->GetList(SPELL_LIST,spList,63))
            { Error("No spell list in Illusion spell!"); return DONE; }
          if (e.EActor->isPlayer())
            {
              for (i=0;spList[i];i++)
                T1->LOption(NAME(spList[i]),i,TEFF(spList[i])->
                      Describe((Player*)e.EActor));
              spID = spList[T1->LMenu(MENU_3COLS|MENU_DESC|MENU_BORDER,
                        "Choose a Force:")];
            }
          else
            {
              for (i=0;spList[i];i++)
                ;
              spID = spList[random(i)];
            }
          e.effIllusion = true;
          e.illFlags = e.EMagic->yval;
          e.ill_eID = e.eID;
          if (e.EActor->isPlayer()) {
            e.eID = spID;
            /*if (!T1->EffectPrompt(e,TEFF(spID)->ef.qval,false,"Target Illusionary Force"))
              { e.eID = e.ill_eID; return ABORT; }*/
            e.eID = e.ill_eID;
            }
          else
            return ABORT;
          r = RedirectEff(e,spID,EV_EFFECT);
          e.illFlags = 0;
          e.effIllusion = false;
          e.effDisbelieved = false;
          e.ill_eID = 0;
          return r;
        default:
          e.EActor->IPrint("Not implemented yet.");
          return DONE;
      }
    return DONE;
  }

EvReturn Magic::Creation(EventInfo &e)
  {
    Thing *t; int16 i; rID xID, iID; 
    if (e.EMagic->rval) {
      xID = e.EMagic->rval;
      switch (RES(xID)->Type) 
        {
           case T_TITEM:
            t = Item::Create(e.EMagic->rval);
            if (!t)
              return ABORT;
            if (e.EActor->onPlane() != PHASE_MATERIAL)
              ((Item *)t)->GainPermStati(PHASED,NULL,SS_MISC,e.EActor->onPlane());
            ((Item*)t)->MakeMagical(0,min(5,e.vDmg/3));
            if (e.EMagic->xval & CREA_MANY)
              ((Item *)t)->SetQuantity(LevelAdjust(e.EMagic->yval,
                                                   e.vCasterLev));
            ((Item*)t)->MakeKnown(-1);
           break;
          case T_TEFFECT:
            if (e.EMagic->xval & CREA_TRAP) {
              t = new Trap(FIND("trap"),xID);
            } else { 
              t = NULL;
              for(i=0;DungeonItems[i].Prob;i++)
                if (TEFF(xID)->HasSource(DungeonItems[i].Source))
                {
                  if (DungeonItems[i].Prototype)
                    iID = FIND(DungeonItems[i].Prototype);
                  else
                    iID = theGame->GetItemID(PUR_ACQUIRE,(int8)e.vDmg,-2,(int8)DungeonItems[i].Type);
                  if (!iID)
                    return ABORT;
                  t = Item::Create(iID);
                  if (!t)
                    return ABORT;
                  ((Item*)t)->MakeMagical(xID,max(5,e.vDmg/3));
                  break;
                }
              if (!t)
                return ABORT;
            }
           break;
          case T_TFEATURE:
            if (TFEAT(xID)->FType == T_PORTAL)
              t = new Portal(xID);
            else if (TFEAT(xID)->FType == T_DOOR)
              t = new Door(xID);
            else
              t = new Feature(TFEAT(xID)->Image,xID,T_FEATURE);
            if (!t)
              return ABORT;
           break;
          default:
            Error(XPrint("Strange resource in rval for Item::Create in $\"<Res>\".",e.eID));
            return ABORT;
        }
      }
    else
      {
        /* if (e.MM & MM_CONTROL)... */
        t = Item::GenItem(0,e.eID,e.vDmg,10,DungeonItems);
        if (!t)
          return ABORT;
      }
    if (e.EMagic->xval & CREA_TEMP)
      t->GainTempStati(SUMMONED,e.EActor,e.vDuration,SS_ENCH,1,1,e.eID);\

    if (e.EMagic->xval & CREA_WIELD)
      {
        ASSERT(t->isType(T_ITEM))
        e.EVictim->GainItem((Item*)t,false);
      }
    else if (e.EMagic->xval & CREA_XY)
    {
        t->PlaceAt(e.EMap,e.EXVal,e.EYVal);
    } 
    else 
      {
        t->PlaceAt(e.EMap,e.EVictim->x,e.EVictim->y);
      }
    if (e.EItem && e.EItem->isItem())
      e.EItem->VisibleID(e.EActor);
    return DONE;
  }


EvReturn Magic::EConstruct(EventInfo &e)
  {
    return ABORT;
  }
          
EvReturn Magic::Detect(EventInfo &e)
  {
    // ww: Detection reworking!
    Error("EA_DETECT and Detect should not be used.");
    return ABORT; 
#if 0
    uint16 tot,i; Thing *t;
    if (e.EItem)
      if (!e.EItem->isKnown(KN_MAGIC)) {
        e.EActor->IPrint("You receive knowledge magically...");
        e.EActor->IdentByTrial(e.EItem);
      }
    if (e.EActor->HasStati(DETECTING)) {
      SetSilence();
      e.EActor->RemoveStati(DETECTING);
      UnsetSilence();
      }
    if (e.EMagic->xval & DET_HILIGHT  && e.EActor->isPlayer())
      e.EActor->GainTempStati(DETECTING, e.isItem ? e.EItem : NULL, e.vDuration,
        SS_ENCH, e.EMagic->xval);
    
    MapIterate(e.EActor->m,t,i)
      {
        /* Don't detect the player */
        if (e.ETarget == t)
          continue;
        if (e.vDmg > 0)
          if (dist(t->x,t->y,e.EActor->x,e.EActor->y) > e.vDmg) {
            continue;
          } 
        if (e.EMagic->yval)
          if (!t->isType(e.EMagic->yval)) {
            continue;
          } 
        if (TEFF(e.eID)->PEvent(EV_ISDETECT,t,e.eID) == -1) {
          continue;
        } 
        if ((e.EMagic->xval & DET_HILIGHT) && e.EActor->isPlayer())
          {
            t->Flags |= F_HILIGHT;
            t->m->Update(t->GetX(),t->GetY());
          }
      }
    return DONE;
    #endif
  }

EvReturn Magic::Travel(EventInfo &e)
  {
    int16 i, _x, _y;
    e.vDmg = max(e.vDmg,7);

    if (!e.EActor || e.EActor->isDead() || !e.EActor->m)
      return ABORT; 

    Map *m = e.EActor->m;
    
    if (e.EVictim->HasStati(ANCHORED))
      goto Failed;
      
    e.EVictim->UnGrapple();

    // ww: previously there were big problems where your mount could get
    // teleported away from under you (Dismissal, say) but you would still
    // have the MOUNTED stati ... 
    if (e.EVictim->HasStati(MOUNTED)) {
      ThrowVal(EV_DISMOUNT,DSM_THROWN,e.EVictim,
        e.EVictim->GetStatiObj(MOUNTED));
    } 
    if (e.EVictim->HasStati(MOUNT)) {
      Thing * rider = e.EVictim->GetStatiObj(MOUNT);
      ThrowVal(EV_DISMOUNT,DSM_THROWN,rider,e.EVictim);
    } 

    switch(e.EMagic->xval) {
      case TRAVEL_TO_TARG:
        TravelTarg:
        if (e.isLoc)
          { _x = e.EXVal;
            _y = e.EYVal; }
        else if (e.ETarget)
          { _x = e.ETarget->x;
            _y = e.ETarget->y; }
        else
          goto Failed;
        if (!e.EMap->InBounds(_x,_y))
          goto Failed;
       break;
      case TRAVEL_KNOWN:
      case TRAVEL_ANYWHERE:
        if (e.MM & MM_CONTROL)
          {
            if (e.EActor->isMonster())
              goto TravelTarg;
            e.vRange = (int8)e.vDmg;
            Thing *t = e.EVictim;
            if (!T1->EffectPrompt(e,Q_LOC))
              goto Failed;
            e.ETarget = t;
            goto TravelTarg;
          }
#ifdef OLD_TRACKING
        if (e.EActor == e.EVictim)
          for(i=0;e.EActor->Stati[i];i++)
            if (e.EActor->Stati[i]->Nature == TRACKING)
              if (t = oThing(e.EActor->Stati[i]->h))
                if (e.EActor->m == t->m)
                  if (dist(e.EActor->x,e.EActor->y,t->x,t->y) <= e.vRange)
                    if(e.EActor->yn(XPrint("Teleport directly to the <Obj>?",t),true))
                      {
                        e.ETarget = t;
                        e.isDir = e.isLoc = false;
                        goto TravelTarg;
                      }
#endif

        for(i=0;i!=50;i++)
          {
            _x = e.EActor->x - e.vDmg + random(e.vDmg * 2);
            _y = e.EActor->y - e.vDmg + random(e.vDmg * 2);
            if (!m->InBounds(_x,_y))
              continue;
            if (m->SolidAt(_x,_y))
              continue;
            // ww: dimdooring into a chasm is *really* annoying
            if (TREG(m->RegionAt(_x,_y))->HasFlag(RF_CHASM) &&
                (!e.EActor->HasStati(LEVITATION) && 
                 !e.EActor->HasMFlag(M_FLYER)))
              continue; 
            if (dist(_x,_y,e.EActor->x,e.EActor->y) < e.vDmg/2)
              continue;
            if (m->FCreatureAt(_x,_y))
              continue;
            if (m->At(e.EVictim->x,e.EVictim->y).isVault != m->At(_x,_y).isVault)
              continue;
            if (e.EActor->Type == T_PLAYER || e.EActor->Type == T_NPC)
              if (!(m->At(_x,_y).Memory || e.EMagic->xval == TRAVEL_ANYWHERE))
                continue;
            break;
          }
        if (i == 50)
          goto Failed;
       break;
      default:
        Error(XPrint("Unknown TRAVEL_ constant in effect <Res>!",e.eID));
        return ABORT;
      }
    e.isSomething = true;
    VPrint(e,NULL,"The <EVictim> vanishes!");
    e.EVictim->PlaceNear(_x,_y);
    VPrint(e,NULL,"An <EVictim> appears!");
    if (e.EItem && e.EItem->isItem())
      e.EVictim->IdentByTrial(e.EItem);
    e.EVictim->GainPermStati(TELEPORTED,NULL,SS_MISC,0,0,0);
    return DONE;

    Failed:
    VPrint(e,"You flicker for a brief instant, but remain where you are.",
             "The <EVictim> flickers in place.");
    if (e.EItem && e.EItem->isItem())
      e.EVictim->IdentByTrial(e.EItem);
    return DONE;
  }

EvReturn Magic::Healing(EventInfo &e)
  {
    int16 i, amt;
    bool id = false, recalc = false;


    if (e.EMagic->xval & HEAL_MANA)
      if (e.EVictim->cMana() < e.EVictim->nhMana()) {
        e.EVictim->GainMana(e.vDmg);
        id = true;
        e.EVictim->IPrint("Your magical reserves are replenished!");
        }

    if ((e.EMagic->xval & HEAL_XP) && e.EVictim->XPDrained()) {
      e.EVictim->IPrint("Your vital force is renewed!");
      if (e.vDmg == 0) e.vDmg = 6; // ww:mysteriously, sometimes this happens
      e.EVictim->RestoreXP(min((int)e.EVictim->XPDrained(),e.vDmg*500));
      /* ww: it's not uncommon to be down 6000 XP after a battle with 6
       * wraiths. 300 per potion doesn't cut it.  */ 
      id = true;
      }

    if (e.EMagic->xval & HEAL_HP) {
      if (e.EVictim->cHP < e.EVictim->mHP+e.EVictim->Attr[A_THP] ||
            e.EVictim->isMType(MA_UNDEAD)) {
        if (e.EMagic->yval)
          amt = (int16)((e.EActor->cMana()*e.EMagic->yval)/10);
        else
          amt = 5000;
        amt = min(e.vDmg,amt);
        if (!amt)
          goto SkipHeal;
        if (e.EMagic->yval)
          e.EActor->LoseMana((amt*10)/e.EMagic->yval,true);
          
        if (e.EVictim->isMType(MA_UNDEAD)) {
          // ouchy!
          VPrint(e,"Your unlife is disrupted!", "The <EVictim>'s unlife is disrupted!");
          ThrowDmg(EV_DAMAGE,AD_NORM,amt*2,NULL,
              e.EActor, e.EVictim, (e.EItem && e.EItem->isItem()) ? e.EItem : NULL, e.EItem2);
          return DONE; 
          }         
       
        amt = min(max(0,(e.EVictim->mHP+e.EVictim->Attr[A_THP]) - e.EVictim->cHP),amt);
        e.EVictim->cHP += amt;
        VPrint(e,"Your wounds heal<Str>!", "The <EVictim>'s wounds heal<Str>!",
          e.EVictim->cHP == (e.EVictim->mHP+e.EVictim->Attr[A_THP]) ? " fully" : "");
        id = true;        
        }
      }
    SkipHeal:

    if (e.EMagic->xval & (HEAL_ATTR|HEAL_SATTR))
      for(i=0;i!=7;i++)
        if ((!(e.EMagic->xval & HEAL_SATTR)) || e.EMagic->yval == i)
          if (e.EVictim->HasStati(ADJUST_DMG,i)) {
            if ((-e.EVictim->SumStatiMag(ADJUST_DMG,i)) <= (e.vDmg/5))
              e.EVictim->RemoveStati(ADJUST_DMG,-1,i);
            else {
              amt = e.EVictim->SumStatiMag(ADJUST_DMG,i) + e.vDmg/5;
              e.EVictim->RemoveStati(ADJUST_DMG,-1,i);
              e.EVictim->GainPermStati(ADJUST_DMG,NULL,SS_MISC,i,amt);
              }
            id = true; recalc = true;
            e.EVictim->IPrint("You feel restored!");
            }

    if (e.EMagic->xval & HEAL_FATIGUE)
      if (e.EVictim->cFP < e.EVictim->GetAttr(A_FAT)) {
        e.EVictim->cFP = min(e.EVictim->GetAttr(A_FAT),
          e.EVictim->cFP + e.vDmg);
        e.EVictim->IPrint("You feel <Str>refreshed!",
          e.EVictim->cFP == e.EVictim->GetAttr(A_FAT) ? "fully " : "");
        id = true; recalc = true;
        }

    if (e.EMagic->xval & HEAL_MALADY)
      if (e.EVictim->HasStati(e.EMagic->yval))
        {
          e.EVictim->RemoveStati(e.EMagic->yval);
          id = true;
        }

    if (e.EMagic->xval & HEAL_ALL_MALADIES) {
      int16 n;
      n = e.EVictim->__Stati.Last;
      e.EVictim->RemoveStati(BLIND);
      e.EVictim->RemoveStati(CONFUSED);
      e.EVictim->RemoveStati(PARALYSIS,-1,PARA_DAZED);
      e.EVictim->RemoveStati(DISEASED);
      e.EVictim->RemoveStati(STUNNED);
      e.EVictim->RemoveStati(POISONED);
      e.EVictim->RemoveStati(HALLU);
      e.EVictim->RemoveStati(BLEEDING);
      e.EVictim->RemoveStati(CHOKING);
      e.EVictim->RemoveStati(WOUNDED);
      e.EVictim->RemoveStati(POLYMORPH);
      if (n != e.EVictim->__Stati.Last)
        id = true;
    } 

    if (e.EMagic->xval & HEAL_HUNGER)
      if (e.EVictim->HungerState() < CONTENT) {
        e.EVictim->SetStatiDur(HUNGER,-1,NULL,CONTENT);
        e.EVictim->IPrint("You feel less hungry now.");
        id = true; recalc = true;
        }

    if (id && !(e.EMagic->xval & HEAL_ALL_MALADIES))
      if (!e.EVictim->isFriendlyTo(e.EActor) &&
           e.EActor != e.EVictim)
        e.EActor->AlignedAct(AL_GOOD,2,"healing neutrals");

    if (id && e.EItem && e.EItem->isItem())
      e.EVictim->IdentByTrial(e.EItem,-1);
    if (recalc)
      e.EVictim->CalcValues();

    return DONE;
  }


