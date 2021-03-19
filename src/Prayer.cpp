/* PRAYER.CPP -- See the Incursion LICENSE file for copyright information.

     Includes all of the code for prayer, sacrifice, divine
   intervention, divine blessings, crowning, etc. */
   
#include "Incursion.h"

static TextVal AidTypes[] = {
  { AID_HEAL,       "healing you" },
  { AID_PURIFY,     "purifying your body" },
  { AID_SMITE,      "smiting your enemies" },
  { AID_UNCURSE,    "lifting curses" },
  { AID_TELEPORT,   "transporting you to safety" },
  { AID_FEED,       "satiating your hunger" },
  { AID_REFRESH,    "removing fatigue" },
  { AID_RESTORE,    "replenishing your spirit" },
  { AID_CURE,       "curing blindness or permanent wounds" },
  { AID_CLARITY,    "clearing your mind" },
  { AID_RESURRECT,  "resurrecting you" },
  { AID_BERSERK,    "improving your fighting ability" },
  { AID_NEWBOOK,    "replacing lost spellbooks" },
  { AID_MANA,       "restoring spent mana" },
  { AID_DEFLECT,    "deflecting lethal blows" },
  { 0, NULL } };

bool godAllowsAlign(rID gID, uint16 align)
  { 
    if (gID && TGOD(gID)->HasFlag(GF_LAWFUL) && !(align & AL_LAWFUL)) 
      return false;
    if (gID && TGOD(gID)->HasFlag(GF_CHAOTIC) && !(align & AL_CHAOTIC)) 
      return false;
    if (gID && TGOD(gID)->HasFlag(GF_EVIL) && !(align & AL_EVIL)) 
      return false;
    if (gID && TGOD(gID)->HasFlag(GF_GOOD) && !(align & AL_GOOD)) 
      return false;      
    
    if (gID && TGOD(gID)->HasFlag(GF_NO_LAWFUL) && (align & AL_LAWFUL)) 
      return false;
    if (gID && TGOD(gID)->HasFlag(GF_NO_CHAOTIC) && (align & AL_CHAOTIC)) 
      return false;
    if (gID && TGOD(gID)->HasFlag(GF_NO_EVIL) && (align & AL_EVIL)) 
      return false;
    if (gID && TGOD(gID)->HasFlag(GF_NO_GOOD) && (align & AL_GOOD)) 
      return false;     
    return true;
  }
  
const char* GodPronoun(rID gID, bool poss)
  {
    if (TGOD(gID)->HasFlag(GF_FEMALE))
      return "her";
    else if (TGOD(gID)->HasFlag(GF_NEUTER))
      return poss ? "its" : "it";
    else
      return poss ? "his" : "him";
  }

EvReturn Character::PrePray(EventInfo &e)
  {
    rID AltarID; Feature *ft; char ch; 
    Item *it; bool found; hObj h; 
    AltarID = 0;
    
    for (ft = m->FFeatureAt(x,y);ft;ft=m->NFeatureAt(x,y))
      if (ft->Flags & F_ALTAR)
        {
          AltarID = ft->GetStatiEID(MY_GOD);
          e.ETarget = ft;
          break;
        }
    if (!GodID && !AltarID)
      {
        IPrint("You have no god to pray to.");
        return ABORT;
      }
    else if (GodID && AltarID && GodID != AltarID) {
      ch = ChoicePrompt(Format("Pray to [1] %s or [2] %s?", NAME(AltarID), 
                            NAME(GodID)), "12");
      if (ch == -1)
        return ABORT;
      e.eID = ((ch == '1') ? AltarID : GodID);
      } 
    else if (AltarID && !GodID)
      {
        if (!yn(Format("Confirm pray to %s?", NAME(AltarID))))
          return ABORT;
        e.eID = AltarID;
      }
    else
      e.eID = GodID;
      
    e.godNum = theGame->GodNum(e.eID);
    ASSERT(e.godNum >= 0);
    ASSERT(e.godNum < MAX_GODS);
      
    /* Trying to convert an altar. */
    if (AltarID && AltarID != e.eID)
      return ReThrow(EV_CONVERT_ALTAR,e);
      
    if (AltarID == e.eID && AltarID != GodID)
      ch = ChoicePrompt("Convert, request aid or make sacrifice?", "cas");
    else if (AltarID && AltarID == e.eID)
      ch = ChoicePrompt("Request aid, bless items, seek insight or sacrifice?", "abis");
    else
      ch = ChoicePrompt("Request aid or seek insight?", "ai");
      
    if (ch == -1)
      return ABORT;
      
    if (ch != 'i')
      if (!isWorthyOf(e.eID,ch == 's'))
        {
          GodMessage(e.eID,MSG_NOT_WORTHY);
          return DONE;
        }
      
    switch (ch)
      {
        case 'a':
          /* Fall through to EV_PRAY */
         break;
        case 'c':
          return ReThrow(EV_CONVERT, e);
        case 'i':
          return ReThrow(EV_INSIGHT, e);
         break; 
        case 'b':
          if (Anger[e.godNum] > (int16)TGOD(e.eID)->GetConst(TOLERANCE_VAL))
            {
              GodMessage(e.eID,MSG_BAD_PRAY);
              e.EParam = Anger[e.godNum] - TGOD(e.eID)->GetConst(TOLERANCE_VAL);
              ReThrow(EV_RETRIBUTION,e);
              return DONE;
            }
          if (calcFavour(e.eID) > 100)         
            for (it=m->FItemAt(x,y);it;it=m->NItemAt(x,y))
              {
                e.EItem = it;
                ReThrow(EV_IBLESSING, e);
              }
          if (!e.isSomething)
            IPrint("Nothing happens.");
          return DONE;
        case 's':
          found = false;
          for (it=FirstInv();it;it=NextInv()) {
            found = true;
            thisp->MyTerm->LOption(it->Name(0),it->myHandle);
            }
          for (it=m->FItemAt(x,y);it;it=m->NItemAt(x,y)) {
            found = true;
            thisp->MyTerm->LOption(Format("%s (on floor)", 
               (const char*) it->Name(0)), it->myHandle);
            }
          if (!found)
            {
              IPrint("You have nothing to offer.");
              return ABORT;
            }
          h = thisp->MyTerm->LMenu(MENU_ESC|MENU_2COLS|MENU_BORDER, "Offer what?");
          if (h == -1)
            return ABORT;
          e.EItem = oItem(h);
          if (e.EItem->iID == FIND("corpse"))
            {
              e.EVictim = new Monster(((Corpse*)e.EItem)->mID);
              StatiIter(e.EItem)
                if (S->Nature == TEMPLATE || S->Nature == ALIGNMENT ||
                    S->Nature == CORPSE_FLAG || S->Nature == WAS_FRIENDLY)
                  e.EVictim->GainPermStati(S->Nature,oThing(S->h),
                          S->Source, S->Val, S->Mag, S->eID, S->CLev);
              StatiIterEnd(e.EItem);
              e.EVictim->CalcValues();
            }
          else
            e.EVictim = NULL;

          return ReThrow(EV_SACRIFICE, e);
        case KY_ESC:
          return ABORT; 
      }
    return NOTHING;
  }

EvReturn Character::Insight(EventInfo &e)
  {
    int16 tol;
    if (isThreatened())
      {
        IPrint("You cannot seek insight while fighting.");
        return ABORT;
      }
  
    GodMessage(e.eID,MSG_READING);
    if (HasStati(TRIED,SK_KNOW_THEO+2000))
      {
        GodMessage(e.eID,MSG_NO_MORE);
        return DONE;
      }
    if (!SkillCheck(SK_KNOW_THEO,15,true))
      {
        GodMessage(e.eID,MSG_NO_INSIGHT);
        GainTempStati(TRIED,NULL,-2,SS_MISC,SK_KNOW_THEO+2000);
        return DONE;
      }
      
    tol = (int16)TGOD(e.eID)->GetConst(TOLERANCE_VAL);
    
    if (getGodFlags(e.eID) & GS_FORSAKEN)
      GodMessage(e.eID, MSG_FORSAKEN);
    else if (getGodFlags(e.eID) & GS_ANATHEMA)
      GodMessage(e.eID, MSG_ANATHEMA);
    else if (Anger[e.godNum] && Anger[e.godNum] <= tol)
      GodMessage(e.eID, MSG_TOLERANT);
    else if (Anger[e.godNum] > 10+tol)
      GodMessage(e.eID, MSG_IS_ANGRY2);
    else if (Anger[e.godNum] > tol)
      GodMessage(e.eID, MSG_IS_ANGRY);
    /*else if (FavourLev[e.godNum] >= 8)
      GodMessage(e.eID, MSG_PLEASED3);
    else if (FavourLev[e.godNum] >= 5)
      GodMessage(e.eID, MSG_PLEASED2);
    else if (FavourLev[e.godNum] >= 2)
      GodMessage(e.eID, MSG_PLEASED);
    else 
      GodMessage(e.eID, MSG_NEUTRAL);*/
    
    if (Anger[e.godNum])
      setGodFlags(e.eID,GS_KNOWN_ANGER);
    
    rID aidChart[64]; int16 i; bool first;
    first = true; String A, B;
    TGOD(e.eID)->GetList(AID_CHART,aidChart,64);
    A = ""; B = "";
    for (i=0;aidChart[i];i+=3)
      if (calcFavour(e.eID) >= (int32)aidChart[i+2] /
             (e.eID == GodID ? 1 : (int32)TEFF(e.eID)->GetConst(LAY_MULTIPLIER)))
        {

         if (aidChart[i] == AID_RESURRECT)
           if (TotalLevel() < (int16)TEFF(GodID)->GetConst(MIN_RAISE_LEVEL) ||
                 e.eID != GodID)
             continue;
         
         if (B.GetLength())
           {
             if (A.GetLength())
               A += ", ";
             A += B;
           }
         
         if (aidChart[i] == AID_IDENTIFY) {
           if (aidChart[i+1] == T_ITEM)
             B = "identifying items";
           else
             B = Format("identifying %s", (const char*)
                   Pluralize(Lower(Lookup(ITypeNames,aidChart[i+1]))));
           }
         else
           B = Lookup(AidTypes,aidChart[i]);
       }
     if (A.GetLength())
       A += SC(" and ") + B;
     else
       A = B;
    
    if (!A.GetLength())
      GodMessage(e.eID, MSG_NO_AID);
    else if (Anger[e.godNum] <= tol && !PrayerTimeout[e.godNum])
      GodMessage(e.eID, MSG_SAFEPRAY, e.eID, (const char*) A);
    else if (PrayerTimeout[e.godNum] && Anger[e.godNum] <= tol)
      GodMessage(e.eID, MSG_WAITPRAY);
    
    if (FavPenalty[e.godNum] + TGOD(e.eID)->
          GetConst(INTERVENTION_COST) > 100)
      return DONE;
    if (PrayerTimeout[e.godNum] || Anger[e.godNum] > tol)
      return DONE;
       
    for (i=0;aidChart[i];i+=3)
      if (aidChart[i] == AID_IDENTIFY && 
           calcFavour(e.eID) >= (int32)aidChart[i+2] /
             (e.eID == GodID ? 1 : (int32)TEFF(GodID)->GetConst(LAY_MULTIPLIER)))
        {
          if (first) {
            if (!yn("Do you want to seek divine insight into your possessions?"))
              return DONE;
            if (e.eID != GodID)
              ReThrow(EV_JEALOUSY,e);
            if (Flags & F_DELETE)
              return DONE;    
            GodMessage(e.eID,MSG_AID + AID_IDENTIFY);
            first = false;
            PrayerTimeout[e.godNum] = (int16)TGOD(e.eID)->GetConst(PRAYER_TIMEOUT) + 
                                    TotalLevel() + random(5);
            FavPenalty[e.godNum] += (int16)TGOD(e.eID)->GetConst(INTERVENTION_COST);
      
            if (FavPenalty[e.godNum] > 70 && FavPenalty[e.godNum] - 
                  TGOD(e.eID)->GetConst(INTERVENTION_COST) <= 70)
              GodMessage(e.eID,MSG_NEARLY_OUT);
            }
          e.EParam  = AID_IDENTIFY;
          e.EParam2 = aidChart[i+1];
          ReThrow(EV_GIVE_AID,e);
          return DONE;
          
        }
    return DONE;
  }
    
EvReturn Character::Sacrifice(EventInfo &e)
  {
    int16 i, sacType, sacCat, sacMult; 
    rID SacList[20];
    int32 sacVal, newVal, lowestVal, oldFavour; bool isImpressed;
    
    /* VERY IMPORTANT for live-sacs, which don't go through 
       PrePray() */
    e.godNum = theGame->GodNum(e.eID);
    
    
    if (e.EItem && e.EItem->isCursed())
      {
        e.EItem->MakeKnown(KN_CURSE);
        IPrint("You find you cannot remove the <Obj> to sacrifice it!",e.EItem);
        return DONE;
      }
    
    if (getGodFlags(e.eID) & GS_FORSAKEN)
      {
        GodMessage(e.eID,MSG_BAD_PRAY);
        e.EParam = 12;
        ReThrow(EV_RETRIBUTION,e);
        return DONE;
      } 
    
    TGOD(e.eID)->GetList(SACRIFICE_LIST,SacList,20);

    sacType = -1;

    if (isPlayer() && thisp->WizardMode)
      oldFavour = calcFavour(e.eID);

    setGodFlags(e.eID,GS_INVOLVED);

    if (e.eID != GodID)
      ReThrow(EV_JEALOUSY,e);
    if (Flags & F_DELETE)
      return DONE;

    switch(e.EParam)
      {
        case 0:
        case SAC_DEFAULT:
          break;
        case SAC_ANGRY:
          sacCat = 0;
          goto Angry;
        case SAC_ABOMINATION:
          sacCat = 0;
          goto Abomination;
        case SAC_UNWORTHY:
          goto Unworthy;
        default:
          sacType = (int16)e.EParam;
          goto FoundCat;
      }
      
    
    sacVal = 0;
    
    FoundCat:
    
    if (e.EVictim && (e.EVictim->HasStati(SUMMONED) ||
                      e.EVictim->HasStati(ILLUSION)))
      goto Uninterested;
    
    sacCat = -1;
    
    lowestVal = 100000000L;
    for (i=0;i!=20 && SacList[i];i+=2) {
        int16 sacValue = (int16)SacList[i];
      if ((sacType == sacValue) ||
          (sacType == -1 && e.EVictim && e.EVictim->isMType(sacValue)) ||
          (sacType == -1 && !e.EVictim && e.EItem->isType(-sacValue)))
        if (SacVals[e.godNum][i/2] < lowestVal)
          { sacCat = i/2; lowestVal = SacVals[e.godNum][i/2]; }
    }
        
    if (sacCat == -1)
      {
        Uninterested:
        GodMessage(e.eID,MSG_UNINTERESTED);
        e.EItem->PlaceAt(m,x,y);
        return DONE;
      }
    
    if (e.EItem)
      e.EItem->Remove(true);
    GodMessage(e.eID,MSG_SACRIFICE);        
    
    sacMult = 10;
    switch (SacList[sacCat*2+1])
      {
        case SAC_ANGRY:       goto Angry; 
        case SAC_ABOMINATION: goto Abomination;
        case SAC_UNWORTHY:    goto Unworthy;
        default:
          if (SacList[sacCat*2+1] > 0)
              sacMult = (int16)SacList[sacCat*2+1];
      }
    
    if (e.vMag)
      sacVal = e.vMag;
    else if (e.EVictim)
      sacVal = XCR(e.EVictim->ChallengeRating());
    else if (e.EItem && e.EItem->isType(T_BOOK))
      sacVal = 100 + e.EItem->ItemLevel()*50;
    else if (e.EItem)
      sacVal = e.EItem->getShopCost(NULL,NULL);
    else
      sacVal = 0;
      
      
    sacVal = (sacVal * sacMult) / 10;
    
    sacVal *= (10 + SkillLevel(SK_KNOW_THEO));
    sacVal /= 10;
    
    isImpressed = false;
    if (e.EVictim) {
      if (sacVal > SacVals[e.godNum][sacCat])
        isImpressed = true;
      newVal = max(SacVals[e.godNum][sacCat],sacVal);
      }
    else {
      newVal = SacVals[e.godNum][sacCat] + (sacVal / 3);
      if (sacVal > WealthByLevel[TotalLevel() / 2] / 20)
        isImpressed = true;
      }
    
    int16 sacAlign;
    sacAlign = (int16)TGOD(e.eID)->GetConst(PERSONAL_ALIGN);
    sacAlign &= desiredAlign;
    if (sacAlign & AL_EVIL)
      sacAlign |= AL_NONGOOD;
    if (sacAlign & AL_CHAOTIC)
      sacAlign |= AL_NONLAWFUL;
    if (sacAlign & AL_LAWFUL)
      if (!(TGOD(e.eID)->GetConst(PERSONAL_ALIGN) & AL_GOOD))
        sacAlign |= AL_NONCHAOTIC;
    AlignedAct(sacAlign,isImpressed ? 3 : 1,"making a sacrifice");
    
    if (Anger[e.godNum] > 0)
      {
        /*
        if (!isImpressed)
          {
            GodMessage(e.eID,MSG_INSUFFICIENT);
            return DONE;
          }
        */
        Anger[e.godNum] = max(0,Anger[e.godNum]-3);
        if (Anger[e.godNum] > 0)
          GodMessage(e.eID,MSG_LESSENED);
        else
          GodMessage(e.eID,MSG_MOLLIFIED);
        return DONE;
      }
    
    SacVals[e.godNum][sacCat] = 
      max(newVal,SacVals[e.godNum][sacCat]);
    
    if (isImpressed) {
      GodMessage(e.eID,MSG_IMPRESSED);
      Exercise(A_WIS,Dice::Roll(1,10),EWIS_PRAYER,50);
      }
    else {
      GodMessage(e.eID,MSG_SATISFIED);
      Exercise(A_WIS,Dice::Roll(1,4),EWIS_PRAYER,25);
      }
        
    gainedFavour(e.eID);
    
    if (isPlayer() && thisp->WizardMode) {
      int32 Favour; rID Levs[15];
      Favour = calcFavour(e.eID);
      TGOD(e.eID)->GetList(FAVOUR_CHART,Levs,15);
      IPrint(Format("(sacVal %d, gained %d, favour %d, lev %d, next %d.)",
        sacVal, Favour - oldFavour, Favour, FavourLev[e.godNum], 
        Levs[FavourLev[e.godNum]]));
      }
    
    return DONE;
    
    Angry:
    Transgress(e.eID,1,false,"offensive sacrifice");
    GodMessage(e.eID,MSG_BADSAC1 + sacCat);
    return DONE;
    
    Abomination:
    Transgress(e.eID,5,true,"very offensive sacrifice");
    GodMessage(e.eID,MSG_BADSAC1 + sacCat);
    return DONE;
    
    Unworthy:
    GodMessage(e.eID,MSG_INSUFFICIENT);
    return DONE;
    
  }

void Character::lowerAnger(rID gID, int16 amt)
  {
    int16 gn;
    gn = theGame->GodNum(gID);
    if (getGodFlags(gID) & (GS_ANATHEMA))
      return;
    if (Anger[gn] && amt)
      {
        Anger[gn] = max(0,Anger[gn] - amt);
        if (Anger[gn] > 0)
          GodMessage(gID,MSG_LESSENED);
        else
          GodMessage(gID,MSG_MOLLIFIED);
      }
  }

int16 getGodRelation(rID fromID, rID toID, bool * doWrath)
  {
    rID relList[100]; 
    int16 i, mag;
    
    mag = 3;
    if (doWrath)
      *doWrath = false;
      
    TGOD(fromID)->GetList(GOD_RELATIONS,relList,100);
    
    for (i=0;relList[i];i+=3)
        if (relList[i] == REL_DEFAULT) {
            mag = (int16)relList[i+1];
            if (doWrath)
                *doWrath = relList[i+2] != 0;
            break;
        }
    
    for (i=0;relList[i] || relList[i+1] || relList[i+2];i++)
        if (relList[i] == toID) {
            while (relList[i+1] > 0x00010000)
                i++;
            mag = (int16)relList[i+1];
            if (doWrath)
                *doWrath = relList[i+2] != 0;
            break;
        }
    return mag;
  }
  
EvReturn Character::Jealousy(EventInfo &e)
  {
    int16 mag; bool doWrath;
    if (!GodID)
      return NOTHING;
    
    mag = getGodRelation(GodID,e.eID,&doWrath);
        
    if (!mag)
      return DONE;
        
        
    if (doWrath || EventStack[max(0,EventSP-1)].Event == EV_CONVERT)
      Transgress(GodID,mag,true,"divine jealousy");
    else
      Transgress(GodID,mag,false,"divine jealousy");
  
    return DONE;
  }

void Character::gainFavour(rID gID, int32 amt, bool advance, bool stack)
  {
    if (stack)
      SacVals[theGame->GodNum(gID)][MAX_SAC_CATS+1] += amt;
    else
      SacVals[theGame->GodNum(gID)][MAX_SAC_CATS] = max(amt,
        SacVals[theGame->GodNum(gID)][MAX_SAC_CATS]);
    
    if (advance && gID == GodID)
      gainedFavour(gID);
  }

void Character::gainedFavour(rID gID)
  {
    int16 lv, godNum; EvReturn r;
    int32 fav; rID favourChart[15];

    /* HACKFIX */
    //if (gID != GodID)
    //  return;



    godNum = theGame->GodNum(gID);
    lv = FavourLev[godNum];
    fav = calcFavour(gID);
    TGOD(gID)->GetList(FAVOUR_CHART,favourChart,15);

    if (lv >= 9)
      return;

    if (fav > (int32)favourChart[lv])
      {
        SetSilence();
        if (!isWorthyOf(gID,false)) {
          UnsetSilence();
          return;
          }
        UnsetSilence();
        FavourLev[godNum]++;
        PEVENT(EV_BLESSING,this,gID,e.EParam = FavourLev[godNum],r);
        if (r == ABORT)
          {
            FavourLev[godNum]--;
            return;
          }
        GodMessage(gID,MSG_BLESSING1 + (FavourLev[godNum] - 1));
        AddAbilities(gID,FavourLev[godNum]);
      }
  
  }

int32 Character::calcFavour(rID gID)
  {
    int32 Favour; int16 i, godNum;
    godNum = theGame->GodNum(gID);
    
    Favour = 0;
    for (i=0;i!=MAX_SAC_CATS+2;i++)
      Favour += SacVals[godNum][i];
    
    EventInfo e;
    e.Clear();
    e.EParam = Favour;
    e.EActor = this;
    e.EVictim = this;
    e.eID = gID;
    e.godNum = godNum;
    ReThrow(EV_CALC_FAVOUR,e);
    Favour = e.EParam;
    
    Favour = (Favour * (100 - FavPenalty[e.godNum])) / 100;
    
    // Favour *= 3;
    
    return Favour;
  }


int16 Creature::worstTrouble()
  {
    int16 *tr;
    tr = getTroubles();
    return tr[0];
  }

int16* Creature::getTroubles() 
  {
    int16 n; Item *it;
    static int16 Troubles[40];
    if (!mHP) {
      Troubles[0] = 0;
      return Troubles;
      }
      
    int32 inj = (((mHP+Attr[A_THP])-cHP)*100)/(mHP+Attr[A_THP]),
         man = (cMana()*100L)/max(nhMana(),1);
    
    /* Mana is CRITICAL to mages, priests & druids, somewhat critical
       to fighter-mages and less significant to everyone else, so
       adjust accordingly. */   
    man = ((man * 30L) + ((man * 70L * CasterLev()) / max(1,ChallengeRating()))) / 100;
    
    n = 0;
    
    if (70 <= inj)
      Troubles[n++] = TROUBLE_INJURY + P_CRITICAL*256;
    else if (50 <= inj)
      Troubles[n++] = TROUBLE_INJURY + P_URGENT*256;
    else if (20 <= inj)
      Troubles[n++] = TROUBLE_INJURY + P_MODERATE*256;
    else if (inj)
      Troubles[n++] = TROUBLE_INJURY + P_LOW*256;

    if (92 <= man)
      Troubles[n++] = TROUBLE_LOWMANA + P_CRITICAL*256;
    else if (80 <= man)
      Troubles[n++] = TROUBLE_LOWMANA + P_EXTREME*256;
    else if (70 <= man)
      Troubles[n++] = TROUBLE_LOWMANA + P_URGENT*256;
    else if (50 <= man)
      Troubles[n++] = TROUBLE_LOWMANA + P_MODERATE*256;
    else if (20 <= man)
      Troubles[n++] = TROUBLE_LOWMANA + P_LOW*256;

    if (HungerState() == WEAK || HungerState() == FAINTING)
      Troubles[n++] = TROUBLE_HUNGER + P_CRITICAL*256;
    else if (HungerState() == STARVING)
      Troubles[n++] = TROUBLE_HUNGER + P_URGENT*256;
    else if (HungerState() == HUNGRY)
      Troubles[n++] = TROUBLE_HUNGER + P_MODERATE*256;
          
    if (HasStati(STONING))
      Troubles[n++] = TROUBLE_STONING + P_CRITICAL*256;
    if (HasStati(POISONED))
      Troubles[n++] = TROUBLE_POISON + P_EXTREME*256;
    if (HasStati(BLIND,0))
      Troubles[n++] = TROUBLE_BLIND + P_EXTREME*256;
    if (HasStati(AFRAID))
      Troubles[n++] = TROUBLE_FEAR + P_URGENT*256;
    if (HasStati(SICKNESS))
      Troubles[n++] = TROUBLE_SICK + P_HIGH*256;
    if (HasStati(BLEEDING))
      Troubles[n++] = TROUBLE_BLEEDING + P_HIGH*256;
    if (HasStati(DISEASED))
      Troubles[n++] = TROUBLE_DISEASE + P_HIGH*256;
    if (HasStati(PARALYSIS))
      Troubles[n++] = TROUBLE_PARALYSIS + P_EXTREME*256;

    StatiIterAdjust(this)
      if (IsAdjustStati(S->Nature) && S->Mag < 0 && S->Val != A_SIZ) {
        if (S->Source == SS_ENCH)
          Troubles[n++] = TROUBLE_DRAINED +  
            ( S->Mag <= -4 ? P_HIGH : P_MODERATE) * 256;
        }
      if (S->Source == SS_CURS)
        Troubles[n++] = TROUBLE_CURSED + 
              ( S->Mag <= -4 ? P_HIGH : P_MODERATE) * 256;
    StatiIterEnd(this)
    
    for (it=FirstInv();it;it=NextInv())
      if (it->IFlags & IF_WORN)
        if (it->isCursed())
          Troubles[n++] = TROUBLE_CURSED + P_HIGH*256;
      
    if (isPlayer())
      {
        int16 missingSpells = 0, totalSpells = 0, i; Creature *cr;
        for (i=0;i!=theGame->LastSpell();i++)
          if ((thisp->Spells[i] & SP_KNOWN) && (thisp->Spells[i] & SP_ARCANE))
            {
              totalSpells++;
              for (it = FirstInv(); it ;it=NextInv())
                if (it->HasSpell(i))
                  goto NextSpell;
              missingSpells++;
              NextSpell:;
            }
        if (missingSpells*2 < totalSpells)
          Troubles[n++] = TROUBLE_NOBOOK + P_CRITICAL*256;
        else if (missingSpells*3 < totalSpells)
          Troubles[n++] = TROUBLE_NOBOOK + P_EXTREME*256;
        else if (missingSpells*5 < totalSpells)
          Troubles[n++] = TROUBLE_NOBOOK + P_HIGH*256;
        else if (missingSpells)
          Troubles[n++] = TROUBLE_NOBOOK + P_MODERATE*256;
          
        
        int32 myXCR = 0, enemyXCR = 0;
          
        MapIterate(m,cr,i)
          if (cr->isCreature() && cr->DistFrom(this) < 16) {
            if (cr->isHostileTo(this) && m->LineOfFire(x,y,cr->x,cr->y,cr))
              enemyXCR += XCR(cr->ChallengeRating());
            else if (cr->isFriendlyTo(this) && m->LineOfFire(x,y,cr->x,cr->y,cr))
              myXCR    += XCR(cr->ChallengeRating());
            }
            
        myXCR = (myXCR * (100 - inj)) / 100;
        
        if (enemyXCR > myXCR*3)
          Troubles[n++] = TROUBLE_ENEMIES + P_CRITICAL*256;
        else if (enemyXCR > myXCR*2)
          Troubles[n++] = TROUBLE_ENEMIES + P_EXTREME*256;
        else if (enemyXCR > (myXCR*3)/2)
          Troubles[n++] = TROUBLE_ENEMIES + P_HIGH*256;
        else if (enemyXCR > myXCR)
          Troubles[n++] = TROUBLE_ENEMIES + P_MODERATE*256;
        else if (enemyXCR > myXCR / 2)
          Troubles[n++] = TROUBLE_ENEMIES + P_LOW*256;
          
        {
          Creature *cr; int16 i;
          MapIterate(m,cr,i)
            if (cr->isCreature())
              if (cr->onPlane() == PHASE_ETHEREAL ||
                  cr->HasStati(INVIS))
                if (m->LineOfFire(x,y,cr->x,cr->y,cr) &&
                      cr->isHostileTo(this) && !Perceives(cr))
                  {
                    Troubles[n++] = TROUBLE_HAUNTED + P_EXTREME*256;
                    break;
                  }
        }
          
          
      }
      
    Troubles[n]   = 0;
    Troubles[n+1] = 0;
    
    return Troubles;
  }

void Creature::PlaceSomewhereSafe()
  {
    int16 sx, sy, nx, ny, i;
    sx = m->SizeX();
    sy = m->SizeY();
    i = 0;
    IDPrint("The world shifts!", "The <EActor> vanishes!");
    UnGrapple();
    do {
      i++;
      if (i >= 10000)
        return;
      nx = random(sx);
      ny = random(sy);
      if (!m->OpenAt(nx,ny))
        continue;
      if (m->FallAt(nx,ny))
        continue;
      if (TTER(m->TerrainAt(nx,ny))->HasFlag(TF_WARN))
        continue;
      PlaceNear(nx,ny);
      i += 20;
      }
    while(isThreatened(false));
}

EvReturn Character::GiveAid(EventInfo &e) {
    int16 n, gained; 
    Item *it;
    rID spBooks[128], bkID;
    rID gID;

    gID = theGame->GodID(e.godNum);
    setGodFlags(gID,GS_INVOLVED);

    if (e.EParam != AID_IDENTIFY)
        GodMessage(gID,(int16)(MSG_AID + e.EParam));

    switch (e.EParam) {
    case AID_HEAL:
        cHP = mHP + GetAttr(A_THP);
        break;
    case AID_PURIFY:
        RemoveStati(POISONED);
        RemoveStati(DISEASED);
        RemoveStati(STONING);
        RemoveStati(PARALYSIS);
        break;
    case AID_CURE:
        RemoveStati(BLIND);
        RemoveStati(WOUNDED);
        break;
    case AID_SMITE:
        Error("AID_SMITE without custom override!");
        break;
    case AID_UNCURSE:
        for (int8 i = 0; i != SL_LAST; i++)
            if ((it = InSlot(i)) && it->isCursed()) {
                if (it->eID && TEFF(it->eID)->HasFlag(EF_CURSED)) {
                    DPrint(e, "Your <Obj> crumbles to ash!",
                        "The <EActor>'s <Obj> crumbles to ash!");
                    it->Remove(true);
                } else {
                    DPrint(e, "Your <Obj> glows with a soft blue light.",
                        "The <EActor>'s <Obj> glows with a soft blue light.");
                    it->IFlags &= ~(IF_CURSED);
                }
            }
        RemoveStatiSource(SS_CURS);
        break;
    case AID_TELEPORT:

        break;
    case AID_FEED:
        if (HasStati(HUNGER))
            SetStatiDur(HUNGER,-1,NULL,(SATIATED+BLOATED)/2);
        break;
    case AID_REFRESH:
        cFP = GetAttr(A_FAT);
        break;
    case AID_RESTORE:
        RemoveStati(ADJUST_DMG);
        RestoreXP(600000);
        break;
    case AID_MANA:
        uMana = 0;
        break;
    case AID_CLARITY:
        IPrint("Divine clarity restores focus to your thoughts!");
        RemoveStati(STUNNED);
        RemoveStati(CONFUSED);
        RemoveStati(CHARMED);
        RemoveStati(PARALYSIS);
        {
            Creature *cr;
            int16 i;

            MapIterate(m,cr,i)
                if (cr->isCreature())
                    if (cr->onPlane() == PHASE_ETHEREAL || cr->HasStati(INVIS))
                        if (m->LineOfFire(x,y,cr->x,cr->y,cr) && cr->isHostileTo(this)) {
                            IPrint("You vision expands to encompass the invisible!");
                            GainTempStati(SEE_INVIS, NULL, 60, SS_MISC, 0, 0, FIND("See Invisibility"));
                            break;
                        }
        }
        break;
    case AID_BERSERK:
        IPrint("<Res>'s spirit comes over you, and you fly into a berzerk rage!",gID);
        GainTempStati(RAGING,NULL,100,SS_MISC,8,20);
        break;
    case AID_NEWBOOK:
        /* Make Array of all spellbooks */
        n = 0;
        for (int16 i = 0; i != theGame->Modules[0]->szItm; i++) {
            bkID = theGame->Modules[0]->ItemID(i);
            if (TITEM(bkID)->Type != T_BOOK)
                continue;
            spBooks[n++] = bkID;
        }
        spBooks[n] = 0;

        gained = 0;      
        for (uint16 i = 0; i != theGame->LastSpell(); i++)
            if ((Spells[i] & SP_KNOWN) && (Spells[i] & SP_ARCANE)) {
                for (it = FirstInv(); it ;it=NextInv())
                    if (it->HasSpell(i))
                        goto NextSpell;

                it = Item::Create(spBooks[0]);
                for(n=0;spBooks[n];n++) {
                    it->iID = spBooks[n];
                    if (it->HasSpell(i)) {
                        IPrint("<Res> replaces your <Obj>!", gID, it);
                        GainItem(it,true);
                        if (it->Owner() != this)
                            goto Done;
                        gained++;
                    }
                }
NextSpell:;
                if (gained >= 5)
                    break;
            }
Done:
        break;
    case AID_IDENTIFY:
        SetSilence();
        for (it=FirstInv();it;it=NextInv())
            if (it->isType((int16)e.EParam2))
                it->MakeKnown(KN_MAGIC|KN_NATURE|KN_PLUS|KN_CURSE|KN_IDENTIFIED);
        UnsetSilence();
        break;
    }

    return DONE;
}  

void Character::GrantSymbol(rID gID) {
    rID iID, eID; Item *it;
    iID = FIND("holy symbol");
    eID = TGOD(gID)->GetConst(HOLY_SYMBOL);
    
    if (!eID || !iID)
      return;
    
    for (it=FirstInv();it;it=NextInv())
      if (it->iID == iID && it->eID == eID)
        return;
        
    IPrint("A holy symbol of <Res> shimmers into existence in your hands!", gID);
    it = Item::Create(iID);
    it->eID = eID;
    GainItem(it,false);
  }
  
  
/* IMPORTANT IDEA -- PRAYER_TIMEOUT is the number of times
   you have gained any single amount of experience between
   now and your last prayer. It starts at 0 (you can receive
   divine aid from the beginning of the game) and gets
   decrimented in GainXP. */

int16 AidPairs[22][2] = {
  { TROUBLE_INJURY,   AID_HEAL },
  { TROUBLE_LOWMANA,  AID_REFRESH },
  { TROUBLE_HUNGER,   AID_FEED },
  { TROUBLE_BLIND,    AID_CURE },
  { TROUBLE_DISEASE,  AID_PURIFY },
  { TROUBLE_SICK,     AID_PURIFY },
  { TROUBLE_STONING,  AID_PURIFY },
  { TROUBLE_DRAINED,  AID_RESTORE },
  { TROUBLE_CURSED,   AID_UNCURSE },
  { TROUBLE_BLEEDING, AID_HEAL },
  { TROUBLE_FEAR,     AID_CLARITY },
  { TROUBLE_POISON,   AID_PURIFY },
  { TROUBLE_FATIGUE,  AID_REFRESH },
  { TROUBLE_NOBOOK,   AID_NEWBOOK },
  { TROUBLE_LOWMANA,  AID_MANA },
  { TROUBLE_ENEMIES,  AID_SMITE },
  { TROUBLE_ENEMIES,  AID_TELEPORT },
  { TROUBLE_PARALYSIS, AID_CLARITY },
  { TROUBLE_HAUNTED,  AID_CLARITY },
  { TROUBLE_PARALYSIS, AID_PURIFY },
  { 0, 0 },
  };

EvReturn Character::Pray(EventInfo &e) {
    TGod *tg = TGOD(e.eID);
    int16 retry, i, j, k, cFavour;
    int16 *Troubles; rID aidChart[64];
    bool doneSomething;
    String je;

    je = XPrint("You prayed to <Res> for divine aid.\n",e.eID);

    Timeout += 30;

    retry = GetStatiMag(TRIED,SK_KNOW_THEO+1000);
    if (!SkillCheck(SK_KNOW_THEO,(int16)tg->GetConst(PRAYER_DC),true,-retry, retry ? "retry" : "")) {
        RemoveStati(TRIED,-1,SK_KNOW_THEO+1000);
        GainTempStati(TRIED,NULL,-2,SS_MISC,SK_KNOW_THEO+1000,retry+2);
        IPrint("You fail to recite the ritual prayers and invocations correctly.");
        AddJournalEntry("You tried to pray for aid, but failed.");
        return DONE;
    }

    if (e.eID != GodID) {
        je += "The response was divine jealousy.";
        AddJournalEntry(je);
        ReThrow(EV_JEALOUSY,e);
    }
    if (Flags & F_DELETE)
        return DONE;

    if (Anger[e.godNum] > (int16)TGOD(e.eID)->GetConst(TOLERANCE_VAL)) {
        GodMessage(e.eID,MSG_BAD_PRAY);
        je += "The result was divine retribution.";
        AddJournalEntry(je);
        e.EParam = Anger[e.godNum] - TGOD(e.eID)->GetConst(TOLERANCE_VAL);
        ReThrow(EV_RETRIBUTION,e);
        return DONE;
    }

    if (PrayerTimeout[e.godNum] > 0) {
        GodMessage(e.eID,MSG_TIMEOUT);
        je += "You called for aid too soon after your last prayer.";
        AddJournalEntry(je);
        Transgress(e.eID,1,true,"prayer during timeout");
        PrayerTimeout[e.godNum] += (int16)tg->GetConst(PRAYER_TIMEOUT);
        return DONE;
    }

    GodMessage(e.eID,MSG_PRAYER);

    if (FavPenalty[e.godNum] + tg->GetConst(INTERVENTION_COST) > 100) { 
        je += "Your god had no more aid to give.";
        AddJournalEntry(je);
        GodMessage(e.eID,MSG_OUT_OF_AID);
        return DONE;
    }

    Troubles = getTroubles();
    tg->GetList(AID_CHART,aidChart,64);
    cFavour = (int16)calcFavour(e.eID);
    doneSomething = false;

    if (HasAbility(CA_DOMAINS) ||
        HasAbility(CA_TURNING))
        GrantSymbol(e.eID);
    if (Timeout)
        doneSomething = true;

    for (i=0;Troubles[i];i++)
        for (j=0;aidChart[j];j+=3) {
            if ((int16)aidChart[j+1] > (Troubles[i] / 256))
                continue;
            if ((int16)aidChart[j+2] > cFavour / (e.eID == GodID ? 1 : 5))
                continue;
            for (k=0;AidPairs[k][0];k++)
                if (AidPairs[k][1] == aidChart[j] &&
                    AidPairs[k][0] == (Troubles[i] % 256))
                {
                    e.EParam = aidChart[j];
                    ReThrow(EV_GIVE_AID,e);
                    je += XPrint("<Res> aided you by <Str>.\n",
                        e.eID, Lookup(AidTypes,aidChart[j]));
                    doneSomething = true;
                    e.EParam = 0;
                } 
        }

    if (!doneSomething) {
        je += XPrint("<Res> had no aid to give you.",e.eID);
        GodMessage(e.eID,MSG_NOAID);
        AddJournalEntry(je);
        return DONE;
    }

    PrayerTimeout[e.godNum] = (int16)tg->GetConst(PRAYER_TIMEOUT) + TotalLevel() + random(5);
    FavPenalty[e.godNum] += (int16)tg->GetConst(INTERVENTION_COST);

    if (FavPenalty[e.godNum] > 70 && FavPenalty[e.godNum] - tg->GetConst(INTERVENTION_COST) <= 70) {
        GodMessage(e.eID,MSG_NEARLY_OUT);
        je += XPrint("<Res> cannot aid you much further.",e.eID);
    }

    AddJournalEntry(je);    

    return DONE;
}
  
  
EvReturn Character::Convert(EventInfo &e)
  {
    int16 neededFavour, i; 
    rID gID = e.eID;
  
    if (HasAbility(CA_DOMAINS))
      {
        IPrint("Priests changing gods is not implemented yet!");
        return ABORT;
      }
    
    rID GodList[50];
    for (i=0;Level[i];i++)
      if (TCLASS(ClassID[i])->GetList(ALLOWED_GODS,GodList,49))
        {
          for (int16 j=0;GodList[j];j++)
            if (e.eID == GodList[j])
              goto GodIsOkay;
          
          if (!yn(XPrint("<Res> is not a <Res> god! Confirm attempt conversion?",
                   e.eID,ClassID[i])))
            return ABORT;
          if (TCLASS(ClassID[i])->HasFlag(CF_PALADIN))
            PaladinFall();
          GodIsOkay:;
        }
        
    AddJournalEntry(XPrint("You attempted conversion to <Res>.", e.eID));

    if (e.eID != GodID)
        ReThrow(EV_JEALOUSY,e);
    if (Flags & F_DELETE)
        return DONE;      
    
    if (getGodFlags(e.eID) & GS_ANATHEMA) {
        GodMessage(e.eID,MSG_FORSAKE);
        return DONE;
    }
    
    if (getGodAnger(e.eID)) {
        GodMessage(e.eID,MSG_ANGER);
        return DONE;
    }
    
    neededFavour = (int16)TGOD(e.eID)->GetConst(MIN_CONVERT_FAVOUR);    
    if (getGodFlags(e.eID) & GS_FORSAKEN)
      neededFavour = max(neededFavour*5,5000);
    
    if (!GodID)
      neededFavour /= 10;
    
    if (calcFavour(e.eID) < neededFavour) {
        if (getGodFlags(e.eID) & GS_FORSAKEN)
          GodMessage(e.eID,MSG_FORSAKEN);
        else
          GodMessage(e.eID,MSG_PROVE_WORTH);
        return DONE;
      }
    
    IPrint("You now follow <Res>.",e.eID);
    GodMessage(e.eID,MSG_CONVERT);
    setGodFlags(e.eID,GS_INVOLVED);
    GrantSymbol(e.eID);
    
    /* Lose favour with the god you convert away from -- later, if
       the god is friendly, just halve it. */
    if (GodID) {
      setGodFlags(GodID,GS_ABANDONED);
      e.godNum = theGame->GodNum(GodID);
      for (i=0;i!=10;i++)
        SacVals[e.godNum][i] = 0;
      /* This is an ugly, nonsensical kludge. The idea of the favour
         penalty is that if you need a lot of help from your god,
         that god is less likely to make you a champion. The game-
         world logic falls apart in the face of conversion, however:
         Essiah shouldn't hold it against you if Immotian helped you
         a lot before you turned your back on him and converted to
         worshipping her, but it would be a huge abuse if you could
         wrack up a 90% penalty with Immotian, then move to Essiah
         after all those interventions with a clean slate and advance
         to the point of crowning.
         
           So for now, you carry your penalty over from your old god,
         which actually makes it pointless to make FavPenalty an
         array, but this may change in the future when I come up with
         a method that's both in-game logical and OOC balanced.
         */
      FavPenalty[e.godNum] = max(FavPenalty[e.godNum],
             FavPenalty[theGame->GodNum(GodID)]);

      }
     
    /* Lose all favour with god you are converting to -- you need to
       re-earn it to move up with him again. */
    e.godNum = theGame->GodNum(e.eID);
    for (i=0;i!=10;i++)
      SacVals[e.godNum][i] = 0;
    
    GodID = e.eID;
    
    Exercise(A_WIS,Dice::Roll(5,10),EWIS_CONVERT,40);
    
    return DONE;
  }
  
EvReturn Character::ConvertAltar(EventInfo &e)
  {
    int16 roll, bonus, rel; rID altarID; 
    bool doWrath, isCeded; String nums;
    
    altarID = e.ETarget->GetStatiEID(MY_GOD);
    
    if (!GodID)
      { IPrint("But you are not yet pledged to any specific deity!");
        return ABORT; }
    
    if (isThreatened())
      { IPrint("You cannot convert an altar while in combat.");
        return ABORT; }
        
    IPrint("You begin the ritual prayers to consecrate this altar "
           "in <Res>'s name...", GodID);
    
    if (!LoseFatigue(2,true))
      return ABORT;
    if (isPlayer())
      if (thisp->SpendHours(4,4) != DONE)
        return DONE;
    
    rel = getGodRelation(GodID,altarID,&doWrath);
        
    if (!SkillCheck(SK_KNOW_THEO,15,true))
      {
        IPrint("You fail to perform the consecration correctly.");
        if (rel || TGOD(altarID)->HasFlag(GF_JEALOUS)) 
          Transgress(altarID,rel,false,"attempted altar theft");
        return DONE;
      }
           
    roll = random(20)+1;
    
    if (getGodFlags(GodID) & (GS_ANATHEMA|GS_FORSAKEN|GS_PENANCE))
      {
        GodMessage(GodID,MSG_BAD_PRAY);
        Transgress(GodID,2,true,"praying after utter rejection");
        goto autoFail;
      }
      
    
    bonus = FavourLev[theGame->GodNum(GodID)];
    bonus += Mod(A_WIS);
    nums = Format("%cAltar Conversion:%c 1d20 (%d) %+d favour %+d Wis ", -YELLOW, -GREY, roll, FavourLev[theGame->GodNum(GodID)], Mod(A_WIS));
    isCeded = false;               
    if (rel == 0 && !TGOD(altarID)->HasFlag(GF_JEALOUS)) {
      IPrint("Based on <str> respect for <Res>, <Res> cedes the altar to you.", GodPronoun(altarID,true), GodID, altarID);
      bonus += 4;
      nums += "+4 ceded ";
      isCeded = true;
    }
      
    nums += Format("= %d vs. DC 15 %c[%s]%c", bonus+roll, 
      bonus + roll >= 15 ? -EMERALD : -RED,
      bonus + roll >= 15 ? "success" : "failure");
    
    thisp->MyTerm->SetWin(WIN_NUMBERS);
    thisp->MyTerm->Clear();
    thisp->MyTerm->Write(0,0,nums);
    
    if (bonus + roll > 15)
      {
        IPrint("The altar changes shape!");
        Exercise(A_WIS,Dice::Roll(4,10),EWIS_CONVERT,60);
        e.ETarget->RemoveStati(MY_GOD);
        e.ETarget->GainPermStati(MY_GOD,NULL,SS_MISC,0,0,GodID);
        AddJournalEntry(XPrint("You converted an altar of <Res> to <Res>.", 
          altarID, GodID));
      }
    else
      {
        if (bonus + roll > 12)
          IPrint("The altar trembles fiercely!");
        else if (bonus + roll > 7 || isCeded) {
          autoFail:
          IPrint("The altar shatters!");
          e.ETarget->Remove(true);
          }
        else {
          IPrint("The altar trembles and explodes!");
          ThrowDmg(EV_DAMAGE,AD_BLUNT,Dice::Roll(2,10),
                  "an exploding altar", this, this);
          ThrowDmg(EV_DAMAGE,AD_FIRE,Dice::Roll(2,10),
                  "an exploding altar", this, this);
          e.ETarget->Remove(true);
          }      
          
        AddJournalEntry(XPrint("You failed to convert an altar of <Res> to <Res>.", 
          altarID, GodID));
      }            
      
    FavPenalty[theGame->GodNum(altarID)] += 20;
    if (!isCeded)
      Transgress(altarID,rel+5,true,"altar theft");
    
    return DONE;
  } 

EvReturn Character::Retribution(EventInfo &e)
  {
    IPrint("You will suffer <Res>'s retribution!", e.eID);
    return DONE;
  } 

EvReturn Character::GodDeflect(EventInfo &e)
  {
    int16 i; rID aidChart[64];
    if (!GodID)
      return NOTHING;
      
    if (e.vDmg < (cHP - (mHP/5)) &&
        (!e.EItem || !e.EItem->HasQuality(WQ_VORPAL)))
      return NOTHING;
      
    e.godNum = theGame->GodNum(GodID);
    
    if (!(getGodFlags(GodID) & GS_INVOLVED))
      return NOTHING;
      
    if (FavPenalty[e.godNum] + TGOD(GodID)->GetConst(INTERVENTION_COST) > 100)
      return NOTHING;
      
    if (Anger[e.godNum] > max(3,(int16)TGOD(GodID)->GetConst(TOLERANCE_VAL)))
      return NOTHING;
      
    if (getGodFlags(GodID) & (GS_ANATHEMA|GS_FORSAKEN))
      return NOTHING;
    
    TGOD(GodID)->GetList(AID_CHART,aidChart,64);
    for (i=0;aidChart[i];i+=3)
      if (aidChart[i] == AID_DEFLECT)
        if (calcFavour(e.eID) >= (int32)aidChart[i+2])
          goto DoDeflect;
          
    return NOTHING;
    
    DoDeflect:
    e.vDmg /= e.vCrit;
    VPrint(e,"<14>Part of the power of the blow is miraculously deflected! "
             "You feel favoured!<7>", "Some force deflected part of the force "
             "of that blow!");
    AddJournalEntry(XPrint("<Res> shielded you from a critical hit.", GodID));

    if (e.EItem && e.EItem->HasQuality(WQ_VORPAL)) {
      e.EVictim->IPrint("You feel strangely certain that <Obj> would "
        "have decapitated you!", e.EItem);
      e.EItem->MakeKnown(0xFF);
      }

    /* Intervention Penalty */
    FavPenalty[e.godNum] += (int16)TGOD(GodID)->GetConst(INTERVENTION_COST);
    return DONE;
    
  }

EvReturn Player::GodRaise(EventInfo &e)
  {
    int16 i; rID aidChart[64]; Item *it;
    if (!GodID)
      return NOTHING;
    e.eID = GodID;
    e.godNum = theGame->GodNum(e.eID);
    
    if (!(getGodFlags(e.eID) & GS_INVOLVED))
      return NOTHING;
    
    if (FavPenalty[e.godNum] + TGOD(e.eID)->GetConst(RESURRECTION_COST) > 100)
      return NOTHING;
      
    if (Anger[e.godNum] > max(3,(int16)TGOD(e.eID)->GetConst(TOLERANCE_VAL)))
      return NOTHING;
      
    if (getGodFlags(e.eID) & (GS_ANATHEMA|GS_FORSAKEN))
      return NOTHING;
    
    if (TotalLevel() < (int16)TGOD(e.eID)->GetConst(MIN_RAISE_LEVEL))
      return NOTHING;
      
    if (BAttr[A_CON] <= 3)
      return NOTHING;
    
    TGOD(e.eID)->GetList(AID_CHART,aidChart,64);
    for (i=0;aidChart[i];i+=3)
      if (aidChart[i] == AID_RESURRECT)
        if (calcFavour(e.eID) >= (int32)aidChart[i+2])
          goto DoResurrect;
          
    return NOTHING;
    
    DoResurrect:
    GodMessage(GodID,MSG_OFFER_RAISE);
    MyTerm->SetWin(WIN_MAP);
    MyTerm->Clear();
    if (!yn("Do you want to return to life?"))
      return DONE;
  
    if (resChance < random(100))
      {
        IPrint("Your soul does not prove strong enough to withstand "
               "the trauma of an attempted resurrection.");
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(WHITE);
        MyTerm->Write("Press [ENTER] to continue... ");
        MyTerm->CursorOn();
        while(MyTerm->GetCharRaw() != KY_ENTER)
          ;
        MyTerm->CursorOff();
        return DONE;
      }
         
    resChance -= max(0,(10 - Mod(A_CON)));
    
    /* Intervention Penalty */
    FavPenalty[e.godNum] += (int16)TGOD(GodID)->GetConst(RESURRECTION_COST);
    
    /* You have enough XP for the level right below the one you are
       currently at -- simulate losing a level. */
    XP = max(0,min(XP-2000,(XP*85L)/100L));
    
    /* Reduce Con by one permanently */
    BAttr[A_CON]--;
    
    /* Fix Problems */
    RemoveStatiSource(SS_CURS);
    RemoveStati(DISEASED);
    RemoveStati(POISONED);
    RemoveStati(STUCK);
    RemoveStati(BLEEDING);
    RemoveStati(CHOKING);
    StatiIter(this)
      if (S->Duration != -1 && S->Duration != 0 && S->Nature != HUNGER)
        { StatiIter_ElapseCurrent(this); }
      else if (S->Source == SS_ATTK)
        { StatiIter_ElapseCurrent(this); }
      else if (S->Nature == HUNGER)
        S->Duration = SATIATED - 100;
    StatiIterEnd(this);
    RemoveStati(ADJUST_DMG);
    cHP = mHP + GetAttr(A_THP);
    uMana = hMana = 0;
    
    /* Do the actual raising */
    Flags &= ~(F_DELETE|F_INVIS);
    AddJournalEntry(XPrint("<Res> resurrected you when you were killed by <Str>.",
                            e.eID, (const char*) GraveText));
    GraveText.Empty();
    
    /* Move back to DLev 1 */
    MoveDepth(1,true);
    for (Thing *t=m->FirstThing();t;t=m->NextThing())
      if (GLYPH_ID_VALUE(t->Image) == GLYPH_USTAIRS)
        PlaceAt(m,t->x,t->y);
    SetSilence();
    e.EParam = AID_TELEPORT;
    GiveAid(e);
    UnsetSilence();
    
    /* Give essential equipment */
    if (FIND("Resurrection Gear")) {
      TMON(FIND("Resurrection Gear"))->GrantGear(this,FIND("Resurrection Gear"),1);
      for (it=FirstInv();it;it=NextInv()) {
        it->IFlags |= IF_BLESSED;
        if (it->iID == FIND("holy symbol"))
          it->eID = FIND(Format("%s;symbol",NAME(GodID)));
        }
      }
    
    /* Resurrection Message */
    GodMessage(GodID,MSG_AID + AID_RESURRECT);
    AddJournalEntry(XPrint("<14>You died, but <Res> resurrected you!<7>", GodID));    
    return DONE;
  }
  
EvReturn Character::IBlessing(EventInfo &e)
  {
    int8 qual;
    int16 i;
    bool isChosen;

    qual = 0;
    if (e.EItem->isType(T_WEAPON))
      qual = (int8)TGOD(e.eID)->GetConst(CHOSEN_WEAPON_QUALITY);
    else if (e.EItem->isType(T_SHIELD))
      qual = (int8)TGOD(e.eID)->GetConst(CHOSEN_SHIELD_QUALITY);
    else if (e.EItem->isType(T_ARMOUR))
      qual = (int8)TGOD(e.eID)->GetConst(CHOSEN_ARMOUR_QUALITY);

    isChosen = (e.EItem->iID == TGOD(e.eID)->GetConst(CHOSEN_WEAPON));
    
    if (!qual)
      goto SkipQuality;
    
    if (!e.EItem->GetInherentPlus())
      goto SkipQuality;
    
    if (e.EItem->HasQuality(qual))
      goto SkipQuality;
      
    if (!e.EItem->QualityOK(qual))
      goto SkipQuality;
      
    e.EItem->AddQuality(qual);
    if (e.EItem->ItemLevel() > FavourLev[theGame->GodNum(e.eID)] +  Mod(A_WIS) + isChosen*4) {
        e.EItem->RemoveQuality(qual);
        goto SkipQuality;
    }
    
    APrint(e,"The <Obj> glows with a deep silvery light!",e.EItem);
    e.isSomething = true;
    e.EItem->IFlags &= ~IF_CURSED;
    e.EItem->IFlags |= IF_BLESSED;
    for (i=0;QualityMods[i][0];i++)
      if (QualityMods[i][0] == qual)
        FavPenalty[theGame->GodNum(e.eID)] +=
          (QualityMods[i][1] * 3) + (int16)(e.EItem->GetQuantity() / 5);
    return DONE;

SkipQuality:
    if (e.EItem->IFlags & IF_BLESSED)
      return DONE;
        
    APrint(e,"The <Obj> glows with a soft blue light.", e.EItem);
    e.isSomething = true;
    e.EItem->IFlags &= ~IF_CURSED;
    e.EItem->IFlags |= IF_BLESSED;
    FavPenalty[theGame->GodNum(e.eID)] += 1 + (int16)(e.EItem->GetQuantity() / 5);
          
    return DONE;
}    
  
void Character::Transgress(rID gID, int16 mag, bool doWrath, const char *reason) {
    EventInfo e;
    
    if (getGodFlags(gID) & (GS_FORSAKEN|GS_ANATHEMA))
      return;
      
    if (gID != GodID) {
      if (random(60) > mag)
        return;
      if (Anger[theGame->GodNum(gID)] > 10 && !doWrath)
        return;
      mag = 1;
      }
    
    /* Prevent anger wraparound */
    if (Anger[theGame->GodNum(gID)] > 50)
      return;
    
    int16 att;
    if (att = AngerThisTurn[theGame->GodNum(gID)])
      {
        if (att > mag)
          return;
        AngerThisTurn[theGame->GodNum(gID)] = mag;
        Anger[theGame->GodNum(gID)] += (mag - att);
      
      }
    
    if (theGame->GetPlayer(0)->WizardMode)
      IPrint(Format("Anger: %s %+d (%s).", 
        (const char*) NAME(gID), mag, reason ? reason : "no reason given"));

    Anger[theGame->GodNum(gID)] += mag;
    e.Clear();
    
    if (doWrath && Anger[theGame->GodNum(gID)] >
                     (int16)TGOD(gID)->GetConst(TOLERANCE_VAL))
      {
        EventInfo e;
        e.Clear();
        e.EParam = Anger[theGame->GodNum(gID)] - 
                   TGOD(gID)->GetConst(TOLERANCE_VAL);
        e.eID = gID;
        e.EActor = this;
        e.EVictim = this;
        ReThrow(EV_RETRIBUTION,e);
      }
    
    if (Anger[theGame->GodNum(gID)] >= 50)
      setGodFlags(gID,GS_ANATHEMA);
    if (Anger[theGame->GodNum(gID)] > 35 && gID == GodID)
      Forsake();
    else if (!att || (att < 5 && mag >= 5)) {
      if (theGame->Opt(OPT_TRANSGRESS_HINTS) && reason) {
        if (mag >= 5 && (gID == GodID))
          GodMessage(gID,MSG_VUNEASY_R,reason);
        else if (mag && (gID == GodID))
          GodMessage(gID,MSG_UNEASY_R,reason);
        }
      else {
        if (mag >= 5 && (gID == GodID))
          GodMessage(gID,MSG_VUNEASY);
        else if (mag && (gID == GodID))
          GodMessage(gID,MSG_UNEASY);
        }
      if (mag && (gID == GodID) && Anger[theGame->GodNum(gID)])
        setGodFlags(gID,GS_KNOWN_ANGER);
      } 
    if (gID == GodID)
      Abuse(A_WIS,Dice::Roll((int8)mag,10));
  }  
  
void Character::Forsake()
  {
    int16 i;
    if (!GodID)
      return;
    
    for (i=0;i!=10;i++)
      SacVals[theGame->GodNum(GodID)][i] = 0;
    RemoveStatiSource(SS_BLES);
       
    if (TGOD(GodID)->HasFlag(GF_FORGIVING))
      {
        if (Anger[theGame->GodNum(GodID)] > 30)
          goto realForsake;
        GodMessage(GodID,MSG_PENANCE);
        Anger[theGame->GodNum(GodID)] = max(10 +
          (int16)TGOD(GodID)->GetConst(TOLERANCE_VAL),
          Anger[theGame->GodNum(GodID)]);
        
        setGodFlags(GodID, GS_PENANCE);
        return;
      }
      
    realForsake:
    GodMessage(GodID,MSG_FORSAKE);
    Anger[theGame->GodNum(GodID)] = max(10 +
      (int16)TGOD(GodID)->GetConst(TOLERANCE_VAL),
      Anger[theGame->GodNum(GodID)]);
    setGodFlags(GodID, GS_FORSAKEN);    
    GodID = 0;
  }
  
bool Character::isWorthyOf(rID gID, bool penanceOK)
  {

    switch (TGOD(gID)->PEvent(EV_PREREQ,this,gID))
      {
        case ABORT:
          return false;
        case DONE:
          return true;
        case NOMSG:
          if (EventSP > -1)
            EventStack[EventSP].Terse = true;
          return false;
        default:
          break;
      }
    
    if (HasStati(ALIGNMENT))
      if (!godAllowsAlign(gID,GetStatiVal(ALIGNMENT)))
        return false;
    
    if (getGodFlags(gID) & (GS_ANATHEMA|GS_FORSAKEN|(penanceOK ? 0 : GS_PENANCE)))
      return false;
    
    return true;
  }
  
void Character::GodMessage(rID gID, int16 msgnum, ...)
  {
    va_list ap;
		va_start(ap, msgnum);
    rID msgList[128]; String msg, msg2, je; 
    bool quoted, isCustom; int16 i, col, tcol;
    TextVal DefaultMessages[] = {

      { MSG_ANGER, "A voice booms, |You draw my ire, mortal!|" },      
      { MSG_PROVE_WORTH, "A voice booms, |Thou must prove thy worth, mortal!|" },
      { MSG_NOT_WORTHY, "A voice booms, |Thou art not worthy of my gifts!|" },
      { MSG_BAD_PRAY, "A voice booms, |Thou darest call upon me?!|" },
      { MSG_JEALOUSY, "A voice booms, |Thou darest spurn me?!|" },
      { MSG_CONVERT, "|Honour my name and my covenant, and thou shalt attain great glory!|" },
      
      
      
      /* Transgression message */
      { MSG_UNEASY,  "You feel uneasy." },
      { MSG_VUNEASY, "You feel very uneasy." },
      { MSG_UNEASY,  "You feel uneasy (<Str>)." },
      { MSG_VUNEASY, "You feel very uneasy (<Str>)." },
      { MSG_FORSAKE, "A voice booms, |Thou hast wrought unholy acts in my name, "
                     "and thus thou art forever anathema to me!|" },
      { MSG_PENANCE, "A voice intones, |Thou hast wrought unholy acts in my name; "
                     "thus I curse thee and revoke all that I have given!|" },               
      
      /* Invoking Aid Messages */
      { MSG_TIMEOUT, "A voice booms, |Thou hast invoked my aid far too casually, mortal!|" },
      { MSG_PRAYER, "You are surrounded in a column of shimmering light." },
      { MSG_NOAID, "A voice speaks solemnly in your mind: |That which you pray for is "
                     "outside of my dominion.|" },
      { MSG_OUT_OF_AID, "A voice speaks solemnly, |Thou must journey on thine own now. "
                         "I have given thee all that I can." },
      { MSG_NEARLY_OUT, "A voice speaks sadly, |The time when you will move beyond my "
                         "ability to aid you draws near. You must decide carefully whether "
                         "you are able to continue this quest.|" },
    
      /* Responses to sacrifices */
      { MSG_SACRIFICE, "Your sacrifice vanishes in a burst of silvery flame." },
      { MSG_INSUFFICIENT,  "Bitter smoke rises from the altar. You feel inadequate." },
      { MSG_SATISFIED, "Pungent smoke rises from the altar. You feel that <Res> "
                       "is satisfied with your offering." },
      { MSG_IMPRESSED, "Sweet smoke rises from the altar. You feel that <Res> "
                       "is impressed by your offering." },
      { MSG_BADSAC1,   "A voice booms, |You dare to offer me *that*?!|" },
      { MSG_UNINTERESTED, "<Res> seems uninterested in your offering." },
      { MSG_LESSENED,  "You feel that <Res>'s anger is fading." },
      { MSG_MOLLIFIED, "You feel that <Res>'s anger has faded." },
      { MSG_EXPENDED,  "You feel that <Res>'s anger is now fully spent." },
      
      /* Responses to seeking insight. */
      { MSG_READING,   "You think back on signs and omens to try to understand how "
                       "<Res> judges you." },
      { MSG_FORSAKEN,  "You feel certain that <Res> has forsaken you eternally." },
      { MSG_ANATHEMA,  "You feel that you are anathema to <Res>." },
      { MSG_IS_ANGRY2, "You feel that <Res> is furious with your conduct." },
      { MSG_IS_ANGRY,  "You fear you have drawn <Res>'s ire." },
      { MSG_NEUTRAL,   "You feel you are of little concern to <Res>." },
      { MSG_PLEASED,   "You feel <Res> is pleased with your devotion." },
      { MSG_PLEASED2,  "You feel <Res> is deeply pleased with your devotion." },
      { MSG_PLEASED3,  "You feel very close to <Res>." },
      { MSG_NO_INSIGHT,"You gain no clear insight into your status with <Res>." },
      { MSG_NO_MORE,   "You gain no further insight into your status with <Res>." },
      { MSG_SAFEPRAY,  "You feel that <Res> would aid you in a time of need by <Str>." },
      { MSG_WAITPRAY,  "You feel that <Res> cannot aid you again so soon." },
      { MSG_NO_AID,    "You feel that <Res> will not aid you at all." },
      { MSG_TOLERANT,  "You feel that <Res> will forgive your minor transgressions." },
      
      { MSG_BLESSING1, "|I grant unto thee this blessing!|" },
      { MSG_BLESSING2, "|I grant unto thee this blessing!|" },
      { MSG_BLESSING3, "|I grant unto thee this blessing!|" },
      { MSG_BLESSING4, "|I grant unto thee this blessing!|" },
      { MSG_BLESSING5, "|I grant unto thee this blessing!|" },
      { MSG_BLESSING6, "|I grant unto thee this blessing!|" },
      { MSG_BLESSING7, "|I grant unto thee this blessing!|" },
      { MSG_BLESSING8, "|I grant unto thee this blessing!|" },
      { MSG_BLESSING9, "|I grant unto thee this blessing!|" },
      
      
      { MSG_AID + AID_HEAL, "Your wounds heal fully!" },
      { MSG_AID + AID_PURIFY, "Your afflictions are purified!" },
      { MSG_AID + AID_CURE, "Your body regenerates fully!" },
      { MSG_AID + AID_UNCURSE, "Your curses are lifted!" },
      { MSG_AID + AID_TELEPORT, "<Res> carries you to safety!" },
      { MSG_AID + AID_FEED, "You no longer feel at all hungry." },
      { MSG_AID + AID_REFRESH, "You no longer feel fatigued." },
      { MSG_AID + AID_RESTORE, "Your body and mind are fully restored!" },
      { MSG_AID + AID_CLARITY, "Divine clarity restores focus to your thoughts!" },
      { MSG_AID + AID_BERSERK, "<Res>'s spirit comes over you, and you fly into a berzerk rage!" },
      { MSG_AID + AID_IDENTIFY, "<Res> reveals to you the nature of your equipment!" }, 
      { MSG_AID + AID_MANA, "Your magical energy is replenished!" },
      { MSG_AID + AID_RESURRECT, "You awaken, shocked to find that you are alive again!" },
      
      { MSG_OFFER_RAISE, "A voice speaks into the darkness: |I offer you a second "
                         "chance to find glory in my name!|" },
      
      { 0, NULL }
      };
    
    if (EventSP > -1)
      if (EventStack[EventSP].Terse)
        if (msgnum == MSG_NOT_WORTHY)
          return;
    
    
    TGOD(gID)->GetList(GODSPEAK_LIST,msgList,128);
    
    isCustom = false;
    
    Retry:
    
    for (i=0;msgList[i];i+=2)
      if (msgList[i] == msgnum)
        {
          msg = theGame->GetText(gID,msgList[i+1]);
          isCustom = true;
          goto FoundMsg;
        }
    
    
    if (!LookupOnly(DefaultMessages,msgnum))
      if (msgnum >= MSG_BADSAC2 && msgnum <= MSG_BADSAC9)
        { msgnum = MSG_BADSAC1; goto Retry; }

    if (!LookupOnly(DefaultMessages,msgnum)) {
      IPrint("<9><Res> says something, but you aren't sure what it is!<7> "
             "(God messages still in progress!)", gID);
      goto Done;
      }
    
    msg = Lookup(DefaultMessages,msgnum);
    
    FoundMsg:

    if (isCustom || msgnum == MSG_SAFEPRAY) 
      msg = __XPrint(thisp,msg,ap);
    else
      msg = XPrint(msg,gID);
    
    if (TGOD(gID)->HasFlag(GF_SANITY_BLASTING) && msgnum != MSG_SALUTATION)
      if (strchr(msg,'|'))
        if (SumStatiMag(SUSTAIN,A_INT) < 3)
          {
            IPrint("Your sanity struggles to balance in the presence of "
              "a vastly alien mind!");
            ThrowDmg(EV_DAMAGE,AD_DAIN,random(4),"sanity-blasting divine contact",
                       this, this);
            if (isDead())
              return;
          }
    
    msg2 = ""; quoted = false;
    col = (int16)TGOD(gID)->GetConst(VOICE_COLOUR);
    for (i=0;msg[i];i++)
      {
        tcol = BlastColourSets[col*4 + random(4)];
        if (msg[i] == '|')
          {
            quoted = !quoted;
            if (quoted)
              msg2 += Format("<%d>", tcol);
            msg2 += "\"";
            if (!quoted)
              msg2 += "<7>";
          }
        else {
          msg2 += msg[i];
          if (col >= 16 && quoted)
            msg2 += Format("<%d>", tcol);
          }
      }

    IPrint(msg2);
    
    if (msgnum == MSG_MOLLIFIED ||
        msgnum == MSG_EXPENDED)
      resetGodFlags(gID,GS_KNOWN_ANGER);
    else if (msgnum == MSG_ANGER ||
             msgnum == MSG_JEALOUSY ||
             msgnum == MSG_BAD_PRAY)
      if (Anger[theGame->GodNum(gID)])
        setGodFlags(gID,GS_KNOWN_ANGER);
    
    switch (msgnum)
      {
        case MSG_CUSTOM1:
        case MSG_CUSTOM2:
        case MSG_CUSTOM3:
        case MSG_CUSTOM4:
        case MSG_CUSTOM5:
        case MSG_CUSTOM6:
        case MSG_CUSTOM7:
        case MSG_CUSTOM8:
        case MSG_CUSTOM9:
        case MSG_NOT_WORTHY:
        case MSG_PROVE_WORTH:
        case MSG_UNEASY:
        case MSG_VUNEASY:
        case MSG_UNEASY_R:
        case MSG_VUNEASY_R:
        case MSG_CONVERT:
        case MSG_BLESSING1:
        case MSG_BLESSING2:
        case MSG_BLESSING3:
        case MSG_BLESSING4:
        case MSG_BLESSING5:
        case MSG_BLESSING6:
        case MSG_BLESSING7:
        case MSG_BLESSING8:
        case MSG_BLESSING9:
        case MSG_EXPENDED:
        case MSG_FORSAKE:
           AddJournalEntry(Decolorize(msg2));
          break;
        case MSG_INSUFFICIENT:
        case MSG_IMPRESSED:
        case MSG_BADSAC1:
        case MSG_BADSAC2:
        case MSG_BADSAC3:
        case MSG_BADSAC4:
        case MSG_BADSAC5:
        case MSG_BADSAC6:
        case MSG_BADSAC7:
        case MSG_BADSAC8:
        case MSG_BADSAC9:
        case MSG_BADSAC10:
        case MSG_LESSENED:
        case MSG_MOLLIFIED:
          if (EventStack[EventSP].Event == EV_SACRIFICE)
            je = XPrint("You sacrificed an <Obj> to <Res>.\n",
                   EventStack[EventSP].EItem, gID);
          else
            je = "";
          je += Decolorize(msg2);
          AddJournalEntry(je);
         break;
      }

    Done:
    va_end(ap);
  
  }
  
/* Support Function for Sabine's Wrath */

void Character::SwapAttributes(int16 n)
  {
    int16 i, v, a, b;
    for (i=0;i!=n;i++)
      {
        a = random(7);
        do
          b = random(7);
        while (b == a);
        
        v = BAttr[a];
        BAttr[a] = BAttr[b];
        BAttr[b] = v;
      }
    CalcValues();
  }

/* Support Function for Essiah's Favour */

int32 Character::TotalExploreFavour()
  {
    int32 fav; int16 open, explored, i, j, x, y;
    Map *M;
    
    fav = 0;
    
    for (i=0;theGame->DungeonID[i];i++)
      for (j=0;theGame->DungeonLevels[i][j];j++)
        {
          M = oMap(theGame->DungeonLevels[i][j]);
          open = explored = 0;
          for (x=0; x<M->SizeX(); x++)
            for (y=0; y<M->SizeY(); y++) 
              if (!M->SolidAt(x,y)) {
                open++;
                if (M->At(x,y).Memory) 
                  explored++;
                }
          
          fav += (50 * explored * (j+1)) / open;
        }
    
    return fav;
  
  }



/*************************************************************************************
 *                              ALIGNMENT-RELATED FUNCTIONS                          *
 *************************************************************************************/   
  
/* Enlisting inherently evil creatures should be an evil act. 
   Socializing with demons/devils offends many gods as well. */ 
 
EvReturn Character::AlignedAct(EventInfo &e)
  {
    uint16 cAlign, dAlign, nAlign;
    bool isGuilty, isFoolish; int16 i;
    
    // You feel guilty/foolish
    
    /* Chaotic Good and Chaotic Neutral characters care about the
       personal freedoms of all creatures, and thus AL_NONCHAOTIC
       acts transgress against the Chaotic portion of their alignment.
       Chaotic Evil characters care only about their own freedoms,
       and thus similar acts do not transgress the Chaotic portion
       of their own alignment. 
       Lawful Evil characters must still obey the 'Law' portion of
       their alignment, however; if they violate it they 'feel
       foolish' instead of guilty because they weaken the lopsided
       social contract that allows them to exploit others for their
       own benefit. 
    */
    
    cAlign = GetStatiVal(ALIGNMENT);
    dAlign = desiredAlign; nAlign = cAlign;
    isGuilty = false;
    isFoolish = false;

    if (cAlign & AL_EVIL)
      if (cAlign & AL_CHAOTIC)
        if (e.EParam & AL_NONCHAOTIC)
          e.EParam &= (~AL_NONCHAOTIC);

    
    if (cAlign & AL_GOOD) {
      if (e.EParam & AL_NONGOOD) {
        alignGE = (alignGE * (10 - (int16)e.vMag)) / 10;
        isGuilty = true;
        }
      else if (e.EParam & AL_GOOD)
        alignGE -= (int16)e.vMag;
      if (!(dAlign & AL_GOOD))
        alignGE = max(-30, alignGE);
      }
    else if (cAlign & AL_EVIL) {
      if (e.EParam & AL_NONEVIL) {
        alignGE = (alignGE * (10 - (int16)e.vMag)) / 10;
        isFoolish = true;
        }
      else if (e.EParam & AL_EVIL)
        alignGE += (int16)e.vMag;
      if (!(dAlign & AL_EVIL))
        alignGE = min(30, alignGE);
      }
    else if (e.EParam & AL_GOOD) {
      alignGE -= (int16)e.vMag;
      if (!(dAlign & AL_GOOD))
        alignGE = max(alignGE,0);
      }
    else if (e.EParam & AL_EVIL)
      alignGE += (int16)e.vMag;
      
    if (alignGE > -20)
      nAlign &= (~AL_GOOD);
    else
      nAlign |= AL_GOOD;
      
    if (alignGE < 20)
      nAlign &= (~AL_EVIL);
    else
      nAlign |= AL_EVIL;

    if (cAlign & AL_LAWFUL) {
      if (e.EParam & AL_NONLAWFUL) {
        alignLC = (alignLC * (10 - (int16)e.vMag)) / 10;
        if (cAlign & AL_EVIL)
          isFoolish = true;
        else
          isGuilty = true;
        }
      else if (e.EParam & AL_LAWFUL)
        alignLC -= (int16)e.vMag;
      if (!(dAlign & AL_LAWFUL))
        alignLC = max(-30, alignLC);
      }
    else if (cAlign & AL_CHAOTIC) {
      if (e.EParam & AL_NONCHAOTIC) {
        alignLC = (alignLC * (10 - (int16)e.vMag)) / 10;
        if (cAlign & AL_EVIL)
          isFoolish = true;
        else
          isGuilty = true;
        }
      else if (e.EParam & AL_CHAOTIC)
        alignLC += (int16)e.vMag;
      if (!(dAlign & AL_CHAOTIC))
        alignLC = max(30, alignLC);
      }
    else if (e.EParam & AL_LAWFUL) {
      alignLC -= (int16)e.vMag;
      if (!(dAlign & AL_LAWFUL))
        alignLC = max(alignLC,0);
      }
    else if (e.EParam & AL_CHAOTIC) {
      alignLC -= (int16)e.vMag;
      if (!(dAlign & AL_CHAOTIC))
        alignLC = min(alignLC,0);
      }
    
    alignLC = max(-70,min(70,alignLC));
    alignGE = max(-70,min(70,alignGE));
      
    if (alignLC > -20)
      nAlign &= (~AL_LAWFUL);
    if (alignLC < 20)
      nAlign &= (~AL_CHAOTIC);
      
    if (abs(alignLC) < 20 && !(cAlign & (AL_LAWFUL|AL_CHAOTIC)) && 
         (e.EParam & (AL_CHAOTIC|AL_LAWFUL|AL_NONCHAOTIC|AL_NONLAWFUL)))
      {
        if (abs(alignLC) >= 10)
          IPrint ((theGame->Opt(OPT_TRANSGRESS_HINTS) && e.GraveText.GetLength())
            ? "You feel closer to <Str>." : "You feel closer to <Str> (<Str>).",
            alignLC < 0 ? "lawful stability" : "chaotic independence", 
            (const char*) e.GraveText);
        if (!(e.EParam & (AL_GOOD|AL_EVIL|AL_NONGOOD|AL_NONEVIL)))
          goto SkipGuiltMessages;
      }
      
    if (e.GraveText.GetLength() && theGame->Opt(OPT_TRANSGRESS_HINTS)) {
      if (isFoolish) 
        IPrint("<5>You feel foolish (<Str>).<7>",
          (const char*)e.GraveText);
      else if (isGuilty)
        IPrint("<5>You feel guilty (<Str>).<7>",
          (const char*)e.GraveText);
      }
    else {
      if (isFoolish) 
        IPrint("<5>You feel foolish.<7>");
      else if (isGuilty)
        IPrint("<5>You feel guilty.<7>");
      }
      
    SkipGuiltMessages:
      
    if (nAlign != cAlign) {
      String msg;
      if (!nAlign) 
        msg = XPrint("You are now <3>True Neutral<7>.");
      else
        msg = XPrint("You are now <3><Str> <Str><7>.",
          (nAlign & AL_CHAOTIC ? "Chaotic" :
           nAlign & AL_LAWFUL ? "Lawful" : "Neutral"),
          (nAlign & AL_GOOD ? "Good" :
           nAlign & AL_EVIL ? "Evil" : "Neutral"));
      IPrint(msg);
      AddJournalEntry(msg);
      SetStatiVal(ALIGNMENT,NULL,nAlign);
      if (desiredAlign == cAlign)
        desiredAlign = nAlign;

      if (GodID && !godAllowsAlign(GodID,nAlign))
        Forsake();
      }
   

    
    for (i=0;i!=3;i++)
      if (ClassID[i] && Level[i])
        if (TCLASS(ClassID[i])->HasFlag(CF_PALADIN))
          {
            if (!(nAlign & AL_GOOD) || !(nAlign & AL_LAWFUL))
              if (!isFallenPaladin)
                PaladinFall();
          } 
     
    return DONE;
  }
  
EvReturn Character::ChangeAlign(EventInfo &e)
  {
    return NOTHING;
  }
  
void Character::AlignedAct(uint16 type, int16 mag, const char *reason,Creature *vic)
  {
    EventInfo e;
    e.Clear();
    e.EActor = this;
    e.EVictim = vic;
    e.EParam = type;
    e.vMag   = mag;
    e.GraveText = reason;
    ReThrow(EV_ALIGNED,e);
  }
  
void Creature::AlignedAct(uint16 type, int16 mag, const char *reason,Creature *vic)
  {
    if (theGame->Opt(OPT_NO_ALLY_TRANS))
      return;
    if (getLeader() && (getLeader() != this)) {
      if (reason)
        if (strstr(reason,"chival") || 
            strstr(reason,"merciless") ||
            strstr(reason,"acid") ||
            strstr(reason,"undead") ||
            strstr(reason,"collat") ||
            strstr(reason,"poison") ||
            strstr(reason,"mental coercion"))
        return;
      if (strstr(reason,"unprovok") && vic &&
           !ts.hasTargetOfType(OrderAttackNeutrals))
        {
          Target *t;
          t = ts.GetTarget(vic);
          if (!t)
            return;
          if (t->type == OrderAttackTarget ||
              t->type == TargetEnemy)
            goto Complicity;
          return;
        }
          
      Complicity:
      getLeader()->AlignedAct(type,max(1,mag/2),
        Format ("complicity to %s",reason));
      }
    else {
      if (strstr(reason,"unprovok") && vic && !vic->isMType(MA_EVIL) &&
            ts.hasTargetOfType(OrderAttackTarget))
        {
          Target *t;
          t = ts.getTargetOfType(OrderAttackTarget);
          if (!t)
            return;
          if (vic == t->GetThingOrNULL())
            {
              Creature *instigator;
              instigator = (Creature*) GetStatiObj(DIPLOMACY_TAG);
              if (instigator)
                instigator->AlignedAct(type,mag,
                  Format ("complicity to %s",reason));
            }
        
        }
      }
  }
  
int16 Creature::getAlignment()
  {
    int16 align;
    align = 0;
    if (isMType(MA_LAWFUL))
      align |= AL_LAWFUL;
    if (isMType(MA_CHAOTIC))
      align |= AL_CHAOTIC;
    if (isMType(MA_GOOD))
      align |= AL_GOOD;
    if (isMType(MA_EVIL))
      align |= AL_EVIL;
    return align;
  }
  
    
