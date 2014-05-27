/* SOCIAL.CPP -- Copyright (c) 1999-2006 Julian Mensch
     Includes all the code governing talking to creatures, including
   fast talk, barter, service spells, party dynamics, PHD, companions,
   etc.

     void Player::InitCompanions()
     bool Monster::MakeCompanion(Player *p, int16 CompType)
     int16 Player::GetGroupCR(int16 CompType, int16 AddCR)
     int16 Player::MaxGroupCR(int16 CompType)


   [B]arter
   [C]ow
   [D]ismiss
   [D]istract
   [E]nlist
   [F]ast-Talk
   [G]reet
   [O]rder
   [Q]uell
   [R]equest
   [S]urrender
   [T]aunt

   TODO:
   -- rewrite target system
   -- racial hostility in getSocialMod
   -- chaotics betray surrender
   -- fix RandGoodInv()
   -- Reputation skeleton
   -- comparative strength in Quell accounting
      for groups.
   -- MUCH better group dynamics
   -- email Wes, talk about revising TargetSystem,
        both for group dynamics and for fitting into
        the scripting/design standards better.
   -- bards aren't getting their social skill bonii
   -- remember SetImage and Update when a creature's
      hostility changes. 
   -- silver pieces, copper pieces, trade bars, etc.
   -- When a creature joins a party, give them a name
      from the same lists we use for PCs.
   -- Alluring feat
   -- Add choices to Yuse array, and bulletproof the
      individual functions so you can't, e.x., enlist
      hostile creatures.
      
   TOMMOROW:
   -- LoseMoneyTo
   -- The Surface World -- replace holy symbol, the big
        shop, etc.
   -- maybe do Suggestion (bard 2)!
   -- HTML Manual
   -- Skill Descs in Manual
   -- If bored, Prestige Classes (Master Archer!)

*/

#include "Incursion.h"


#define DY_HOSTILE    0x0001
#define DY_CHARMED    0x0002
#define DY_DOMINATED  0x0004
#define DY_ANGERED    0x0008
#define DY_STRUCK     0x0010

String modBreakdown;

bool checkEndgameStuff(EventInfo &e)
  {
    rID murgID; int32 i; Creature *cr;
    murgID = FIND("Murgash, the Goblin King");
    
    /* Murgash overrides the social events */
    if (e.EVictim->tmID == murgID)
      return false;
      
    MapIterate(e.EMap,cr,i)
      if (cr->isCreature())
        if (cr->tmID == murgID)
          if (cr->PartyID == e.EVictim->PartyID &&
              e.EVictim->PartyID != e.EActor->PartyID)
            {
              if (e.Event == EV_SURRENDER)
                {
                  if (!e.EMap->LineOfSight(e.EActor->x,e.EActor->y,cr->x,cr->y,cr))
                    e.EActor->IPrint("The <Obj> binds you and brings you before the Goblin King.",
                      e.EVictim);
                  e.EVictim = cr;
                  ReThrow(EV_SURRENDER,e);
                  return true;
                }
              e.EActor->IPrint("The <Obj> says, <2>\"Speak only to the King!\"<7>.", 
                  e.EVictim);
              return true;
            }
    
    return false;
  }

EvReturn Creature::PreTalk(EventInfo &e)
  {
    String str, chars; 
    TextVal Choices[] = {
      { EV_GREET,     "<14>G<6>reet" },
      { EV_BARTER,    "<14>B<6>arter" },
      { EV_COW,       "<14>C<6>ow" },
      { EV_DISMISS,   "<14>D<6>ismiss" },
      { EV_DISTRACT,  "<14>D<6>istract" },
      { EV_ENLIST,    "<14>E<6>nlist" },
      { EV_FAST_TALK, "<14>F<6>ast-Talk" },
      { EV_TERMS,     "<14>O<6>ffer Terms" },
      { EV_ORDER,     "<14>O<6>rder" },
      { EV_QUELL,     "<14>Q<6>uell" },
      { EV_REQUEST,   "<14>R<6>equest" },
      { EV_SURRENDER, "<14>S<6>urrender" },
      { EV_TAUNT,     "<14>T<6>aunt" },
      { 0,             NULL } };

    if (!e.ETarget->isCreature() || !e.EVictim->canTalk(e.EActor))
      {
        /* Animals can't talk, but rangers can still give orders
           to their animal companions. */
        if (e.ETarget->isCreature() && e.EVictim->isFriendlyTo(e.EActor))
          {
            e.EParam = EV_ORDER;
            return NOTHING;
          }
        
        IPrint("The <Obj> can't talk, <Obj>.", e.EVictim, e.EActor);
        return ABORT;
      }

    if (!canTalk(e.EVictim))
      {
        IPrint("You can't talk in this form.");
        return ABORT;
      }
      
    if (!e.EParam)
      {
        char ch; int16 i, n, open[15];
        for (i=0,n=0;Choices[i].Text;i++)
          {
            switch (Choices[i].Val)
              {
                case EV_BARTER:
                  if (e.EVictim->isHostileTo(this) ||
                      e.EVictim->HasStati(SUMMONED) ||
                      e.EVictim->HasMFlag(M_NOHANDS) ||
                      !((HasSkill(SK_DIPLOMACY) || GetAttr(A_CHA) >= 13) ||
                        e.EVictim->HasMFlag(M_SELLER)))
                    continue;
                 break;
                case EV_COW:
                  if (!e.EVictim->isHostileTo(this) ||
                      !HasSkill(SK_INTIMIDATE))
                    continue;
                  if (e.EVictim->HasStati(AFRAID))
                    continue;
                 break;
                case EV_DISMISS:
                  if (!e.EVictim->isFriendlyTo(this))
                    continue;
                 break;
                case EV_DISTRACT:
                  if (e.EVictim->isFriendlyTo(this) ||
                      e.EVictim->HasStati(DISTRACTED))
                    continue;
                 break;
                case EV_ENLIST:
                  if (e.EVictim->isHostileTo(this) ||
                      e.EVictim->isFriendlyTo(this))
                    continue;
                 break;
                case EV_FAST_TALK:
                  if (!e.EVictim->isHostileTo(this) ||
                      !e.EActor->HasSkill(SK_BLUFF))
                    continue;
                 break;
                case EV_GREET:
                  if (e.EVictim->HasStati(SUMMONED,-1,e.EActor))
                    continue;
                 break;
                case EV_TERMS:
                  if (!e.EVictim->isHostileTo(this) ||
                      !e.EVictim->HasStati(AFRAID))
                    continue;
                 break;
                case EV_ORDER:
                  if (!e.EVictim->isFriendlyTo(this))
                    continue;
                 break;
                case EV_QUELL:
                  if (!e.EVictim->isHostileTo(this))
                    continue;
                 break;
                case EV_REQUEST:
                  if (e.EVictim->isHostileTo(this) ||
                      e.EVictim->isFriendlyTo(this))
                    continue;
                 break;
                case EV_SURRENDER:
                  if (!e.EVictim->isHostileTo(this) ||
                      e.EVictim->HasStati(AFRAID))
                    continue;
                 break;
                case EV_TAUNT:
                  if (e.EVictim->isFriendlyTo(this) ||
                      e.EVictim->HasStati(ENRAGED))
                    continue;
                 break;
              }    
            open[n++] = i;
          }
        str = chars = NULL;
        for (i=0;i!=n;i++)
          {
            if (i)
              str += i == (n-1) ? " or " : ", ";
            str += Choices[open[i]].Text;
            chars += tolower(Choices[open[i]].Text[4]);
          }
        str += "?";
        ch = e.EActor->ChoicePrompt(XPrint(str),chars,'0');
        if (ch == -1)
          return ABORT;
        for (i=0;i!=n;i++)
          if (ch == tolower(Choices[open[i]].Text[4]))
            e.EParam = Choices[open[i]].Val;
      }
    return NOTHING;
  }     

bool Creature::canTalk(Creature *to)
  {
    if (!isMType(MA_SAPIENT))
      return false;
    if (isMType(MA_PLANT))
      if (to && to->SkillLevel(SK_KNOW_NATURE) >= 10)
        return true;
    if (HasMFlag(M_TALKABLE))
      return true;
    if (TMON(mID)->HasFlag(M_NOHEAD))
      return false;
    switch (TMON(mID)->MType[0]) {
      case MA_BAT: case MA_CAT:
      case MA_DOG: case MA_ANIMAL:
      case MA_MIMIC: case MA_QUADRUPED:
      case MA_RODENT: 
      case MA_SNAKE: case MA_TRAPPER:
      case MA_PLANT: case MA_SPIDER:
      case MA_VORTEX: case MA_WORM:
      case MA_ELEMENTAL: case MA_FUNGI:
      case MA_CONSTRUCT: case MA_PUDDING:
      case MA_QUYLTHULG: 
      case MA_JELLY: case MA_NIGHTSHADE:
        return false;
      default:
        return true;
      }
  }

EvReturn Creature::Barter(EventInfo &e)
  {
    int16 mod;
    if (!isPlayer())
      return ABORT;
    if (!doSocialSanity(EV_BARTER,e.EVictim))
      return ABORT;
    Timeout += 30;
    if (e.EVictim->HasMFlag(M_SELLER))
      goto DoBarter;
    if (isPMType(MA_EVIL,e.EVictim) && !e.EVictim->isMType(MA_EVIL))
      goto NoDesire;
    if (e.EVictim->HasStati(TRIED,EV_BARTER*100+SK_DIPLOMACY,e.EActor))
      {
        if (e.EVictim->GetStatiMag(TRIED,EV_BARTER*100+SK_DIPLOMACY,e.EActor))
          goto DoBarter;
        else
          goto NoDesire;
      }
    if ((mod = getSocialMod(e.EVictim,false)) < 0 && !e.EVictim->HasMFlag(M_SELLER))
      {
        if (HasSkill(SK_DIPLOMACY) && SkillCheck(SK_DIPLOMACY,10-mod*2,true))
          {
            IPrint("The <Obj1> was reluctant to trade with you, but you "
              "manage to persuade <him:Obj1> to reconsider.", e.EVictim);
            e.EVictim->GainPermStati(TRIED,this,SS_MISC,
              EV_BARTER*100+SK_DIPLOMACY, 1);
            goto DoBarter;
          }
        e.EVictim->GainPermStati(TRIED,this,SS_MISC,
          EV_BARTER*100+SK_DIPLOMACY, 0);
        NoDesire:
        IPrint("The <Obj> has no desire to trade with you.", e.EVictim);
        return DONE;
      }
    DoBarter:
    thisp->MyTerm->BarterManager(e.EVictim);
    return DONE;
  }

EvReturn Creature::Cow(EventInfo &e)
  {
    int16 CheckDC, c, dur, i, BestCR; 
    Creature *cr; bool doGroup; Item *it;
    
    if (!doSocialSanity(EV_COW,e.EVictim))
      return ABORT;    
    if (e.EVictim->HasStati(TRIED,SK_INTIMIDATE+EV_COW*100,this))
      {
        e.EActor->IPrint("You've already tried to cow the <Obj>.",
          e.EVictim);
        return ABORT;
      }
    
    if (e.EVictim->ResistLevel(AD_FEAR) == -1)
      { Timeout += 30; goto AutoFail; }
    
    /* HACKFIX */
    if (!e.EVictim->isMType(MA_EVIL))
      if (!(e.EActor->isMType(MA_ORC) ||
          e.EActor->LevelAs(FIND("barbarian"))))
        e.EActor->AlignedAct(AL_NONCHAOTIC,3,"intimidation");
    
    CheckDC = 10;
    
    c = 0; doGroup = false;
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(e.EVictim))
        c++;
    if (c > 1)
      if (yn(XPrint("Cow the <Obj>'s whole group?",e.EVictim)))
        { doGroup = true; CheckDC += 10; BestCR = 0;
          MapIterate(m,cr,i)
            if (cr->isCreature() && cr->isFriendlyTo(e.EVictim)) {
              if (cr->ChallengeRating() + cr->GetStatiMag(SAVE_BONUS,SN_FEAR) > BestCR)
                BestCR = cr->ChallengeRating() + cr->GetStatiMag(SAVE_BONUS,SN_FEAR);     
              cr->GainPermStati(TRIED,this,SS_MISC,
                    SK_INTIMIDATE+EV_COW*100,0,0,0);
              }
          CheckDC += max(0,BestCR*3);
        
        }
    if (!doGroup) {
      CheckDC += max(0,e.EVictim->ChallengeRating()*3);
      e.EVictim->GainPermStati(TRIED,this,SS_MISC,
                    SK_INTIMIDATE+EV_COW*100,0,0,0);
      }
    Timeout += 30;
    
    if (doGroup)
      if (checkEndgameStuff(e))
        return DONE;
    
    int16 smod; smod = getSocialMod(e.EVictim,true);
    if (!SkillCheck(SK_INTIMIDATE,CheckDC,true,smod,modBreakdown))
      {
        AutoFail:
        IPrint("The <Obj> isn't impressed with your might.", e.EVictim);
        return DONE;
      }
      
    if (LastSkillCheckResult > CheckDC + 10)
      dur = 0;
    else if (LastSkillCheckResult > CheckDC + 5)
      dur = -2;
    else
      dur = Dice::Roll(3,6) + e.EActor->SkillLevel(SK_INTIMIDATE);
      
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(e.EVictim))
        if (doGroup || cr == e.EVictim)
          {
            cr->GainTempStati(AFRAID,e.EActor,dur,SS_MISC,FEAR_PANIC);
            cr->GainTempStati(DO_NOT_PICKUP,this,dur,SS_MISC);
            if (dur <= 0)
              cr->FocusCheck(this);
            if (dur == 0)
              KillXP(cr, isMType(MA_GOOD) ? 120 : 100);
            else if (dur == -2)
              KillXP(cr,50);
            if (LastSkillCheckResult > CheckDC + 10 && cr->FirstInv())
              {
                e.EActor->IDPrint(
                  "The <Obj1> throws all <his:Obj1> gear at your feet in a "
                  "placationary manner.", "The <Obj1> throws all <his:Obj1> "
                  "gear at the <Obj2>'s feet in a placationary manner.",
                  cr,e.EActor);
                Restart:
                for (it=cr->FirstInv();it;it=cr->NextInv())
                  if (!(it->IFlags & IF_WORN) || !it->isType(T_ARMOUR))
                    {
                      it->Remove(false);
                      it->PlaceAt(m,x,y);
                      cr->Timeout += 10;
                      goto Restart;
                    }
              }
            else if (LastSkillCheckResult > CheckDC + 5 && cr->FirstInv())
              {
                if (it = cr->RandGoodInv(100)) {
                  e.EActor->IDPrint(
                    "The <Obj1> throws a tribute at your feet in a "
                    "placationary manner.", "The <Obj1> throws a tribute "
                    "at the <Obj2>'s feet in a placationary manner.",
                  cr,e.EActor);
                  it->Remove(false);
                  it->PlaceAt(m,x,y);
                  cr->Timeout += 10;
                  }
              }
          }
          
            
  
    return DONE;
  }

#define NAT_FEAR(cr) (cr->HasStati(AFRAID,FEAR_MANA) || \
                      cr->HasStati(AFRAID,FEAR_HP) || \
                      cr->HasStati(AFRAID,FEAR_PANIC) || \
                      cr->HasStati(AFRAID,FEAR_COWED) )

EvReturn Creature::OfferTerms(EventInfo &e)
  {
    int16 CheckDC, c, dur, i, BestCR; 
    Creature *cr; bool doGroup; Item *it;
    
    if (!doSocialSanity(EV_TERMS,e.EVictim))
      return ABORT;    
    
    if (e.EVictim->HasStati(AFRAID,FEAR_SKIRMISH))
      {
        IPrint("The <Obj> is not truly afraid, merely using skirmish tactics.",e.EVictim);
        return ABORT;
      }
    
    if (!NAT_FEAR(e.EVictim))
      {
        IPrint("You can't offer terms to magically frightened creatures.");
        return ABORT;
      }
    
    if (e.EVictim->HasStati(TRIED,SK_DIPLOMACY+EV_TERMS*100,this))
      {
        e.EActor->IPrint("You've already offered terms to the <Obj>.",
          e.EVictim);
        return ABORT;
      }
    
    if (!e.EVictim->isMType(MA_EVIL) &&
         e.EVictim->HasMFlag(M_IALIGN) &&
        !e.EVictim->HasStati(WAS_FRIENDLY,-1,e.EActor))
      e.EActor->AlignedAct(AL_GOOD,e.EVictim->isMType(MA_EVIL) ? 1 : 3,
        "offering terms");
    
    CheckDC = 5;
    
    c = 0; doGroup = false;
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(e.EVictim) && NAT_FEAR(cr))
        c++;
    if (c > 1)
      if (yn(XPrint("Offer terms to the <Obj>'s whole group?",e.EVictim)))
        { doGroup = true; CheckDC += 10; BestCR = 0;
          MapIterate(m,cr,i)
            if (cr->isCreature() && cr->isFriendlyTo(e.EVictim) &&
                  NAT_FEAR(cr)) {
              if (cr->ChallengeRating() > BestCR)
                BestCR = cr->ChallengeRating();     
              cr->GainPermStati(TRIED,this,SS_MISC,
                    SK_INTIMIDATE+EV_COW*100,0,0,0);
              }
          CheckDC += max(0,BestCR);
        }
        
    if (!doGroup) {
      CheckDC += max(0,e.EVictim->ChallengeRating()*2);
      e.EVictim->GainPermStati(TRIED,this,SS_MISC,
                    SK_DIPLOMACY+EV_TERMS*100,0,0,0);
      }
    Timeout += 30;

    if (doGroup)
      if (checkEndgameStuff(e))
        return DONE;
    
    int16 smod; smod = getSocialMod(e.EVictim,true);
    if (!SkillCheck(SK_DIPLOMACY,CheckDC,true,smod,modBreakdown))
      {
        IPrint("The <Obj> refuses your terms.", e.EVictim);
        return DONE;
      }
      
    if (LastSkillCheckResult > CheckDC + 5)
      dur = 0;
    else if (LastSkillCheckResult > CheckDC)
      dur = -2;
      
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(e.EVictim) && NAT_FEAR(cr))
        if (doGroup || cr == e.EVictim)
          {
            e.EActor->IDPrint("The <Obj> accepts your terms.",
                              "The <Obj> accepts the <Obj2>'s terms.",
                              cr, e.EVictim);
            cr->FocusCheck(e.EActor);
            cr->ts.TurnNeutralTo(cr,e.EActor);
            cr->StateFlags |= MS_PEACEFUL;
            KillXP(cr, isMType(MA_GOOD) ? 120 : 100);
            cr->Timeout += 50;
            cr->GainTempStati(DO_NOT_PICKUP,this,-1,SS_MISC);
            if (LastSkillCheckResult > CheckDC + 10 && cr->FirstInv())
              {
                e.EActor->IDPrint(
                  "The <Obj1> throws all <his:Obj1> gear at your feet in a "
                  "placationary manner.", "The <Obj1> throws all <his:Obj1> "
                  "gear at the <Obj2>'s feet in a placationary manner.",
                  cr,e.EActor);
                Restart:
                for (it=cr->FirstInv();it;it=cr->NextInv())
                  if (!(it->IFlags & IF_WORN) || !it->isType(T_ARMOUR))
                    {
                      it->Remove(false);
                      it->PlaceAt(m,x,y);
                      cr->Timeout += 10;
                      goto Restart;
                    }
              }
            else if (LastSkillCheckResult > CheckDC + 7 && cr->FirstInv())
              {
                if (it = cr->RandGoodInv(100)) {
                  e.EActor->IDPrint(
                    "The <Obj1> throws a tribute at your feet in a "
                    "placationary manner.", "The <Obj1> throws a tribute "
                    "at the <Obj2>'s feet in a placationary manner.",
                  cr,e.EActor);
                  it->Remove(false);
                  it->PlaceAt(m,x,y);
                  cr->Timeout += 10;
                  }
              }
          }
          
            
  
    return DONE;
  }


EvReturn Creature::Dismiss(EventInfo &e)
  {
    int16 CheckDC; bool result;

    if (!doSocialSanity(EV_DISMISS,e.EVictim))
      return ABORT;    
    
    if (!e.EVictim->isFriendlyTo(e.EActor))
      {
        IPrint("That creature isn't a member of your party.");
        return ABORT;
      }
    
    Timeout += 30;
    
    if (HasStati(TRIED,SK_DIPLOMACY + EV_DISMISS*100,e.EActor))
      result = true;
    else {
      CheckDC = 7 + max(0,e.EVictim->ChallengeRating()) +
                m->Depth;
      result = SkillCheck(SK_DIPLOMACY,CheckDC,true);
      }
      
    e.EVictim->GainPermStati(TRIED,e.EActor,SS_MISC,
      SK_DIPLOMACY + EV_DISMISS*100,result);
   
    if (result)
      IPrint("You and the <Obj> part ways on amicable terms.", e.EVictim);
    else
      IPrint("The <Obj> leaves your company feeling cheated and embittered.",
        e.EVictim);
    
    e.EVictim->ts.TurnNeutralTo(e.EVictim,this);
    
    return DONE;
  }

EvReturn Creature::Distract(EventInfo &e)
  {
    int16 CheckDC, i, c, BestRes, Res, sRes; 
    bool doGroup; Creature *cr;

    if (!doSocialSanity(EV_DISTRACT,e.EVictim))
      return ABORT;    
    
    if (!HasSkill(SK_BLUFF))
      {
        IPrint("You need the Bluff skill to distract people.");
        return ABORT;
      }
    
    c = 0; BestRes = 0;
    MapIterate(m,cr,i)
      if (cr->isCreature() && (cr->isHostileTo(this) ||
                               cr->isFriendlyTo(e.EVictim))) {
        if (cr == e.EActor)
          continue;
        c++;
        Res = max(cr->SkillLevel(SK_CONCENT),
                  cr->GetAttr(A_SAV_WILL));
        Res += GetStatiMag(TRIED,SK_BLUFF+EV_DISTRACT*100,e.EActor);
        if (cr == e.EVictim)
          sRes = Res;
        if (Res > BestRes)
          BestRes = Res;
        }
    
    if (c > 1 && yn("Distract Everyone?"))
      doGroup = true;
    else
      doGroup = false;
      
    if (doGroup)
      CheckDC = 15 + BestRes;
    else
      CheckDC = 10 + sRes;
      
    Timeout += 20;
    
    IDPrint("", "The <Obj> makes a spectacle.", this);
    
    if (!SkillCheck(SK_BLUFF,CheckDC,true))
      {
        IDPrint("Your attempt to create a distraction falls flat.",
          "The <Obj>'s attempt to create a distraction falls flat.", this);
        MapIterate(m,cr,i)
          if (cr->isCreature() && cr->isHostileTo(this))
            if (cr == e.EVictim || doGroup)
              cr->IncStatiMag(TRIED,SS_MISC,SK_BLUFF+EV_DISTRACT*100,
                    e.EActor, +5);
        return DONE;
      }
    
    MapIterate(m,cr,i)
      if (cr->isCreature() && (cr->isHostileTo(this) ||
                               cr->isFriendlyTo(e.EVictim)) &&
            m->LineOfSight(x,y,cr->x,cr->y,NULL))
        if ((cr == e.EVictim || doGroup) && cr != e.EActor) {
        
          cr->IncStatiMag(TRIED,SS_MISC,SK_BLUFF+EV_DISTRACT*100,
                e.EActor, +2);
          cr->GainTempStati(DISTRACTED, this, 2, SS_MISC, 0,
            LastSkillCheckResult - CheckDC);
          cr->IDPrint("You get distracted!",
                      "The <Obj> looks away!", cr);
          }
    
    return DONE;
  }
  
EvReturn Creature::Enlist(EventInfo &e)
  {
    int16 CheckDC, smod, Total; 
    bool hasClass;
    
    if (!e.EVictim->isMonster())
      return ABORT;
    if (!isPlayer())
      return ABORT;
    if (!doSocialSanity(EV_ENLIST,e.EVictim))
      return ABORT;          
    if (e.EVictim->HasStati(TRIED, SK_DIPLOMACY+EV_ENLIST*100,e.EActor))
      {
        IPrint("You've already failed to enlist the <Obj>.", e.EVictim);
        return ABORT;
      }
   
    if (e.EVictim->isMType(MA_AQUATIC) && !e.EVictim->HasMFlag(M_AMPHIB))
      {
        IPrint("Since the <Obj1> can't live out of water, asking <him:Obj1> "
          "to join your party is counterproductive.", e.EVictim);
        return ABORT;
      } 
    if (e.EVictim->HasStati(CHARMED))
      {
        IPrint("You cannot add a character under the effect of a charm "
          "spell to your party, though you can ask favours of them with "
          "the Issue Request function.");
        return ABORT;
      }
   
    
    Total = max(0,thisp->MaxGroupCR(PHD_PARTY)) - 
      thisp->GetGroupCR(PHD_PARTY,e.EVictim->ChallengeRating()) ;
    if (Total < 0)
      {
        IPrint("You don't have enough PHD to add that creature to your "
          "party. Dismiss some other party members, raise your Charisma "
          "or Diplomacy, or buy the Leadership feat.");
        return ABORT;
      }
    
    Timeout += 50;
    if (e.EVictim->isMType(MA_EVIL))
      e.EActor->AlignedAct(AL_NONGOOD,3,"allying with evil");
    if (e.EVictim->isMType(MA_CHAOTIC))
      if (!(e.EActor->isMType(MA_GOOD) && e.EVictim->isMType(MA_GOOD)))
        e.EActor->AlignedAct(AL_NONLAWFUL,2,"allying with chaos");
    if (e.EVictim->isMType(MA_LAWFUL) && !e.EVictim->isMType(MA_GOOD))
      e.EActor->AlignedAct(AL_NONCHAOTIC,2,"allying with law");    
      
    if (e.EVictim->HasStati(TRIED,SK_DIPLOMACY + EV_DISMISS*100,e.EActor))
      {
        if (e.EVictim->GetStatiMag(TRIED, SK_DIPLOMACY + EV_DISMISS*100,e.EActor))
          {
            IPrint("The <Obj> rejoins your party.", e.EVictim);
            goto AutoSuccess;
          }
        else
          {
            IPrint("The <Obj> has no interest in rejoining your party.", 
              e.EVictim);
            return DONE;
          }
      }      
    
    hasClass = false;
    StatiIterNature(e.EVictim,TEMPLATE)
      if (TTEM(S->eID)->TType & TM_CLASS)
        hasClass = true;
    StatiIterEnd(e.EVictim)
    if (hasClass)
      CheckDC = 15;
    else if (e.EVictim->isMType(MA_ADVENTURER))
      CheckDC = 20;
    else
      CheckDC = 25;
      
    CheckDC += max(0,e.EVictim->ChallengeRating() * 3);
    
    
    IPrint("You invite the <Obj> to join your party.", e.EVictim);

    if (isPMType(MA_EVIL,e.EVictim) && e.EVictim->isMType(MA_GOOD))
      {
        IPrint("The <Obj> doesn't want to associate with evil.", e.EVictim);
        return DONE;
      }
    if (e.EVictim->isMType(MA_DRAGON) || e.EVictim->isMType(MA_OUTSIDER) ||
            e.EVictim->HasMFlag(M_IALIGN) || strstr(NAME(e.EVictim->tmID),"Murgash"))
      {
        if (isPMType(MA_GOOD,e.EVictim) && e.EVictim->isMType(MA_EVIL))
          {
            IPrint("The <Obj> doesn't want to associate with good.", e.EVictim);
            return DONE;
          }      
        if (isPMType(MA_CHAOTIC,e.EVictim) && e.EVictim->isMType(MA_LAWFUL))
          {
            IPrint("The <Obj> doesn't want to associate with chaotics.", e.EVictim);
            return DONE;
          }
        if (isPMType(MA_LAWFUL,e.EVictim) && e.EVictim->isMType(MA_CHAOTIC))
          {
            IPrint("The <Obj> doesn't want to associate with lawfuls.", e.EVictim);
            return DONE;
          }      
      }
      
    if (HasSkill(SK_PERFORM) && SkillLevel(SK_PERFORM) >= 7)
      {
        IPrint("You weave an inspiring tale of your past heroic exploits "
          "and the great treasures you've seen.", e.EVictim);
        CheckDC -= (SkillLevel(SK_PERFORM)-5) / 2;
      }
    if (HasSkill(SK_APPRAISE) && SkillLevel(SK_APPRAISE) >= 7)
      {
        IPrint("You try to discern what drives the <Obj>, and explain how "
          "travelling with you will fulfill those needs.", e.EVictim);
        CheckDC -= (SkillLevel(SK_APPRAISE)-5) / 2;
      }
    
    smod = getSocialMod(e.EVictim,false);
    if (!SkillCheck(SK_DIPLOMACY,CheckDC,true,smod,modBreakdown))
      {
        IPrint("The <Obj> declines your offer.", e.EVictim);
        e.EVictim->GainPermStati(TRIED,e.EActor,SS_MISC,
          SK_DIPLOMACY+EV_ENLIST*100);
        return DONE;
      }
    IPrint("The <Obj> agrees to join in your adventures!", e.EVictim);
    
    AutoSuccess:
    ((Monster*)e.EVictim)->MakeCompanion(thisp,PHD_PARTY);
    return DONE;
  }
  
EvReturn Creature::FastTalk(EventInfo &e)
  {
    int16 BluffDC;

    if (!doSocialSanity(EV_FAST_TALK,e.EVictim))
      return ABORT;    
    if (e.EVictim->HasStati(TRIED, SK_BLUFF+100*EV_FAST_TALK, e.EActor))
      {
        IPrint("You've already tried to fast-talk the <Obj>.", e.EVictim);
        return ABORT;
      }
    if (!e.EActor->HasSkill(SK_BLUFF))
      {
        IPrint("You must have the Bluff skill to fast-talk others.");
        return ABORT;
      }
    
    BluffDC = 10 + e.EVictim->ChallengeRating() +
                   max(e.EVictim->SkillLevel(SK_APPRAISE),
                       e.EVictim->SkillLevel(SK_CONCENT));
    
    e.EVictim->GainTempStati(TRIED,e.EActor,30,SS_MISC,
                 SK_BLUFF+100*EV_FAST_TALK);
    
    switch (random(4))
      {
        case 0:
          TPrint(e, "You grab the <EVictim> by the shoulders and eagerly "
                       "start congratulating <it:EVictim>...",
                    "The <EActor> grabs the you by the shoulders and eagerly "
                       "starts congratulating you...",
                    "The <EActor> grabs the <EVictim> by the shoulders and eagerly "
                       "starts congratulating <it:EVictim>...");
         break;
        case 1:
          TPrint(e, "You spin a wild story for the <EVictim>...",
                    "The <EActor> starts telling you some crazy story...",
                    "The <EActor> spins a wild story for the <EVictim>...");
         break;
        case 2:
          TPrint(e, "You ramble madly in the <EVictim>'s direction...",
                    "The <EActor> rambles madly at you about something unspecific...",
                     "The <EActor> rambles madly in the <EVictim>'s direction...");
         break;
        case 3:
          DPrint(e,"You speak rapidly and persuasively on unclear matters...",
                   "The <EActor> speaks rapidly and persuasively on unclear matters...");
         break;
      }
    
    
    if (e.EActor->SkillCheck(SK_BLUFF,BluffDC,true))
      {
        VPrint(e,"You hesitate...", "The <EVictim> hesitates!");
        e.EVictim->Timeout += LastSkillCheckResult*2;
      }
    else
      VPrint(e,"You aren't fooled.", "The <EVictim> isn't fooled.");
      
    e.EActor->Timeout += 10;
  
    return DONE;
  }
  
EvReturn Creature::Greet(EventInfo &e)
  {
    Creature *cr, *best;
    Item *it; String a, b; int16 i;

    if (!doSocialSanity(EV_GREET,e.EVictim))
      return ABORT;    
      
    e.EActor->Timeout += 25;
    
    if (e.EVictim->isHostileTo(e.EActor)) {
      IPrint("The <Obj> isn't interested in idle chatter; <he:Obj> "
        "wants to kill you!", e.EVictim, e.EVictim);
      return DONE;
      }
    else
      IDPrint("You chat with the <Obj>.",
              "The <Obj2> chats with you.",
              e.EVictim, e.EActor);
    
    if (e.EActor->isPlayer() && e.EVictim->isMonster() &&
         !e.EVictim->HasStati(TRIED,SK_DIPLOMACY+EV_GREET*100,e.EActor))
      switch(random(300) / max(1,(e.EActor->SkillLevel(SK_DIPLOMACY) + 
                               getSocialMod(e.EVictim, false))))
        {
          case 1:
            IPrint("The <Obj> tells you a bit about the local dungeon layout.", 
              e.EVictim);
            ThrowEff(EV_EFFECT,FIND("Magic Mapping"),e.EActor,e.EActor);
           break;
          case 2:
            best = NULL;
            MapIterate(m,cr,i)
              if (cr->isCreature())
                if (cr->isHostileTo(e.EVictim))
                  if (best == NULL || (cr->ChallengeRating() >  
                        best->ChallengeRating()))
                    best = cr;
            if (best)
              IPrint("The <Obj1> warns you to watch out for <Str3><Obj2> "
                "<he:Obj1> heard was on this level.", e.EVictim, best,
                best->HasMFlag(M_UNIQUE) ? "" : "the dangerous ",
                best->HasMFlag(M_UNIQUE) ? " who" : "");
            else
              IPrint("The <Obj> warns you to watch out for dangerous creatures!",e.EVictim);
           break;
          case 3:
            it = NULL;
            do {
              it = Item::GenItem(0,0,m->Depth,10,DungeonItems);
              if (it->isType(T_POTION) || it->isType(T_SCROLL) ||
                  it->isType(T_RING) || it->isType(T_WAND) ||
                  it->isType(T_STAFF) || it->isType(T_BRACERS) ||
                  it->isType(T_AMULET) || it->isType(T_BOOTS) ||
                  it->isType(T_CLOAK) || it->isType(T_ROD))
                break;
              delete it;
              }
            while(1);
            a = Trim(Pluralize(it->Name(0)));
            if (a[0] == '?')
              a = a.Right(a.GetLength()-2);
            b = Trim(Pluralize(ItemNameFromEffect(it->eID)));
            IPrint("The <Obj> says, \"I've heard that <str> are <str>.\"",
              e.EVictim, (const char*)a, (const char*)b);
            delete it;
           break;
          case 4:
            IPrint("You share some heartening war stories with the <Obj>, "
              "and you feel like you have a second wind!", e.EVictim);
            cFP = min(GetAttr(A_FAT),cFP+2);
           break;
        }
    e.EVictim->GainPermStati(TRIED,e.EActor,SS_MISC,SK_DIPLOMACY+EV_GREET*100);
    return DONE;
  }

EvReturn Creature::Order(EventInfo &e) {
    Term *tm; Creature *c; int32 i;
    if (!e.EActor->isPlayer())
        return ABORT;

    if (e.isAllAllies) {
        bool found = false;
        MapIterate(m,c,i)
            if (c->isCreature() && c != e.EActor && c->isLedBy(e.EActor))
                if (e.EActor->Perceives(c))
                    found = true;
        if (!found) {
            IPrint("You have no visible allies.");
            return ABORT;
        }
    }

    tm = thisp->MyTerm;

    tm->LOption("Retarget", TargetMaster, 
        "General. The selected followers will forget current orders and targets "
        "and reevaluate all nearby creatures.");
    tm->LOption("Stand Still", OrderStandStill, 
        "Movement. The selected followers will stand still and avoid moving.");
    tm->LOption("Travel In Front", OrderWalkInFront, 
        "Movement. The selected followers will travel 30 feet (3 squares) ahead of you.");
    tm->LOption("Travel In Back", OrderWalkInBack, 
        "Movement. The selected followers will travel 10 feet (1 square) behind you.");
    tm->LOption("Travel Near Me", OrderWalkNearMe, 
        "Movement. The selected followers will travel beside you.");
    tm->LOption("Travel To Destination", OrderWalkToPoint, 
        "Movement. The selected followers will travel to the given point.");

    tm->LOption("Do Not Attack", OrderDoNotAttack, 
        "The selected followers will not attack.");
    tm->LOption("Do Not Pick Up Items", OrderNoPickup, 
        "The selected followers will not pick up items.");
    tm->LOption("Attack Neutrals", OrderAttackNeutrals, 
        "The selected followers will attack creatures it would otherwise "
        "(by default) be neutral to.");
    tm->LOption("Attack Specific Target", OrderAttackTarget,
        "The selected followers will attack a specific creature of your "
        "choice in preference to others.");

    tm->LOption("Hide", OrderHide, 
        "Hiding. The selected followers will hide when possible.");
    tm->LOption("Do Not Hide", OrderDoNotHide, 
        "Hiding. The selected followers will refrain from hiding.");
    if (!e.isAllAllies)
        tm->LOption("Give Item to Me", OrderGiveMeItem,
        "The selected followers will give you an item he has found in the dungeon. "
        "You cannot ask followers to give you their personal property (i.e., items they "
        "were generated with), though you can barter for it, but they will give you items "
        "they merely found.");
    if ((!e.EVictim) || e.EVictim->HasStati(SUMMONED))
        tm->LOption("Return to Home Plane", OrderReturnHome,
        "A summoned creature given this order "
        "will return to its plane of origin.");

    int nn = tm->LMenu(MENU_ESC|MENU_DESC|MENU_2COLS|MENU_BORDER,
        e.isAllAllies ? " -- Order All Allies -- " : 
        " -- Give Which Order -- ",WIN_MENUBOX);
    if (nn <= 0)
        return ABORT; 

    TargetType n = (TargetType) nn; 

    EventInfo xe;
    Item *it; hObj hIt;
    xe.Clear();
    switch (n) {
      case OrderWalkToPoint: 
          if (!tm->EffectPrompt(xe,Q_LOC,false,"Direct Follower Where"))
              return DONE;
          break;
      case OrderAttackTarget:
          if (!tm->EffectPrompt(xe,Q_TAR,false,"Attack What?"))
              return DONE;
          break;
      case OrderGiveMeItem:
          if (!e.EVictim->FirstInv()) {
              e.EActor->IPrint("The <Obj> has no inventory.",e.EVictim);
              return ABORT;
          }
          for(it=e.EVictim->FirstInv();it;it=e.EVictim->NextInv())
              tm->LOption(it->Name(0),it->myHandle);
          hIt = tm->LMenu(MENU_BORDER,"Which Item?");
          if (hIt == -1 || hIt == 0)
              return ABORT;
          e.EItem = oItem(hIt);
          if (e.EItem->IFlags & IF_PROPERTY) {
              IPrint("That item is part of the <Obj>'s personal property.", 
                  e.EVictim);
              return DONE;
          }
    }

    if (!e.isAllAllies) {
        c= e.EVictim;
        goto SingleTarget;
    }

RestartScan:
    MapIterate(m,c,i) {
        if (c->isCreature() && c != this) {
            if (c->isLedBy(this) && Perceives(c)) {
SingleTarget:
                switch (n) {
                case TargetMaster: 
                    c->ts.ForgetOrders(c);
                    c->ts.Retarget(c,true);
                    break;
                case OrderAttackNeutrals:
                    c->ts.giveOrder(c,this,n);
                    c->ts.Retarget(c,true);
                    break; 
                case OrderAttackTarget:
                    c->ts.ForgetOrders(c,OrderAttackTarget);
                    c->ts.giveOrder(c,this,OrderAttackTarget,xe.EVictim);
                    break;
                case OrderDoNotHide: 
                    c->Reveal(true);
                    c->ts.giveOrder(c,this,n);
                    break; 

                case OrderWalkToPoint: 
                    c->ts.giveOrder(c,this,n,NULL,xe.EXVal,xe.EYVal);
                    break; 
                case OrderGiveMeItem:
                    IPrint("The <Obj> gives you the <Obj>.", e.EVictim, e.EItem);            
                    e.EItem->Remove(false);
                    GainItem(e.EItem,false);
                    break;
                case OrderReturnHome:
                    if (c->HasStati(SUMMONED) || c->HasStati(TRANSFORMED)) {
                        c->RemoveStati(SUMMONED);
                        c->RemoveStati(TRANSFORMED);
                        if (e.isAllAllies)
                            goto RestartScan;
                    }
                    break;
                default: 
                    c->ts.giveOrder(c,this,n);
                    break; 
                }
                if (!e.isAllAllies)
                    return DONE;
            } 
        } 
    } 
    return DONE; 
}
  
EvReturn Creature::Quell(EventInfo &e)
  {
    int16 sk, dmg, CheckDC, c, i; Target *t; Item *it;
    bool isREnemy, wasDamaged, wantsTribute;
    Creature *cr;
    
    if (!doSocialSanity(EV_QUELL,e.EVictim))
      return ABORT;    
    
    if (!HasSkill(SK_DIPLOMACY))
      {
        IPrint("You need the Diplomacy skill to resolve conflicts.");
        return ABORT;
      }
    
    if (!e.EVictim->isHostileTo(e.EActor))
      {
        e.EActor->IPrint("That creature isn't hostile to you.");
        return ABORT;
      }
      
    if (e.EVictim->HasStati(TRIED,SK_DIPLOMACY+EV_QUELL*100,e.EActor))
      {
        IPrint("You've already tried to quell that creature, or "
          "one of his allies.");
        return ABORT;
      }

    if (checkEndgameStuff(e))
      return DONE;
      
    /* Trying to talk things out is almost always a wise action,
       whether it succeeds or fails. */
    Exercise(A_WIS,random(4)+1,EWIS_PERSUADE,25);
      
      
    Timeout += 30;
    
    t = e.EVictim->ts.GetTarget(e.EActor);
    
    wasDamaged = false;
    isREnemy = e.EVictim->ts.RacialHostility(e.EVictim,e.EActor).quality == Enemy;

    if (!t)
      wasDamaged = false;
    else if (t->why.type == TargetHitMe || t->why.type == TargetSpelledMe)
      { wasDamaged = true; dmg = t->data.Creature.damageDoneToMe; }
    
    sk = SK_DIPLOMACY;
    if (isREnemy) {
      /* Resolving Conflicts */
      if (!e.EVictim->isMType(MA_EVIL))
        e.EActor->AlignedAct(AL_GOOD,3,"resolving conflict");
      }
    else if (wasDamaged) {
      /* Exploitation */
      if (!e.EActor->yn("Confirm use exploitation?"))
        return ABORT;
      sk = SK_BLUFF;
      e.EActor->AlignedAct(AL_EVIL,1,"exploitation");
      e.EActor->Transgress(FIND("Essiah"),5,false,"exploitation");
      }
      
    CheckDC = 15 + max(0,e.EVictim->ChallengeRating())*3;
    
    
    if (wasDamaged)
      CheckDC += 5 + ((dmg*10)/e.EVictim->mHP);
    
    if ((e.EVictim->isMType(MA_GOBLINOID) ||
         e.EVictim->isMType(MA_OUTSIDER) ||
         e.EVictim->isMType(MA_ILLITHID) ||
         e.EVictim->isMType(MA_DRAGON)) &&
         e.EVictim->isMType(MA_EVIL) &&
         e.EActor->RandGoodInv(100 + max(1,e.EVictim->ChallengeRating())) )
      { CheckDC -= 7; wantsTribute = true; }
    else
      wantsTribute = false;
    int16 smod; smod = getSocialMod(e.EVictim,true);
    if (SkillCheck(sk,CheckDC,true,smod,modBreakdown))
      {
        if (wantsTribute && LastSkillCheckResult < CheckDC + 10)
          {
            if (SkillCheck(SK_BLUFF,11+e.EVictim->SkillLevel(SK_APPRAISE),false))
              {
                IPrint("The <Obj> wants tribute, but you convince "
                  "him you have nothing appropriate to give.",e.EVictim);
                goto Success;
              }
            it = e.EActor->RandGoodInv(100 + max(1,e.EVictim->ChallengeRating()));
            if (!it)
              {
                TPrint(e,"The <EVictim> wants tribute, but you have nothing "
                         "of sufficient value to offer.", "You demand tribute, "
                         "but the <EActor> has nothing suitable to give!",
                         "The <EVictim> wants tribute, but the <EActor> has nothing "
                         "of sufficient value to offer.");
                goto Failure;
              }
            IPrint("The <Obj1> demands your <Obj2> as tribute!",
              e.EVictim, it);
            if (yn("Accept the demand?",true))
              {
                TPrint(e,"You give your <Obj> to the <EVictim>.",
                         "The <EActor> gives <his:EActor> <Obj> to you.",
                         "The <EActor> gives <his:EActor> <Obj> to the <EVictim>.",
                         it);
                it->Remove(false);
                e.EVictim->GainItem(it,false);
                goto Success;
              }
            else
              goto Failure;
          }
        Success:
        c = 0;
        MapIterate(m,cr,i)
          if (cr->isCreature())
            if (cr->isFriendlyTo(e.EVictim))
              {
                cr->ts.TurnNeutralTo(cr, e.EActor);
                if (isMType(MA_GOOD))
                  KillXP(cr,100);
                else
                  KillXP(cr,50);
                c++;
              }
        if (c > 1)
          IPrint("You resolve the conflict with the <Obj1> and <his:Obj1> allies.", e.EVictim);
        else
          IPrint("You resolve the conflict with the <Obj1>.", e.EVictim);
        
      }
    else {
      Failure:
      c = 0;
      MapIterate(m,cr,i)
        if (cr->isCreature())
          if (cr->isFriendlyTo(e.EVictim))
            c++;
      if (c > 1)
        IPrint("You fail to resolve the conflict with the <Obj1> and <his:Obj1> allies.", e.EVictim);
      else
        IPrint("You fail to resolve the conflict with the <Obj1>.", e.EVictim);
      }
    MapIterate(m,cr,i)
      if (cr->isCreature())
        if (cr->PartyID == e.EVictim->PartyID)
          cr->GainPermStati(TRIED,e.EActor,SS_MISC,
                SK_DIPLOMACY + EV_QUELL*100);
    return DONE;
  }
  
EvReturn Creature::Request(EventInfo &e)
  {
    int16 sk, DC, i, c, BestCR, smod; TargetType n; 
    Creature *cr; bool doGroup; EventInfo xe;
    Term *tm;
    
    if (!doSocialSanity(EV_REQUEST,e.EVictim))
      return ABORT;    
    
    if (e.EVictim->HasStati(TRIED,EV_REQUEST*100,e.EActor))
      {
        e.EActor->IPrint("You already failed to persuade the <Obj1> "
          "(or a member of <his:Obj1> group); you can't try again until the "
          "next day.", e.EVictim);
        return ABORT;
      }
    
    sk = SK_DIPLOMACY;
    if (SkillLevel(SK_INTIMIDATE) > SkillLevel(SK_DIPLOMACY)) {
      if (yn(Format("Intimidate %c%+d%c, Diplomacy %c%+d%c. Use Intimidate?",
           -13, SkillLevel(SK_INTIMIDATE), -5, -13, SkillLevel(SK_DIPLOMACY), -5),true))
        sk = SK_INTIMIDATE;
      }
    else if (SkillLevel(SK_BLUFF) > SkillLevel(SK_DIPLOMACY))
      if (yn(Format("Bluff %c%+d%c, Diplomacy %c%+d%c. Use Bluff?",
           -13, SkillLevel(SK_BLUFF), -5, -13, SkillLevel(SK_DIPLOMACY), -5),true))
        sk = SK_BLUFF;  
        
    if (sk == SK_INTIMIDATE && !e.EVictim->isMType(MA_EVIL))
      e.EActor->AlignedAct(AL_NONCHAOTIC|AL_LAWFUL,3,"coercion");
    if (sk == SK_BLUFF && !e.EVictim->isMType(MA_EVIL))
      e.EActor->AlignedAct(AL_NONLAWFUL|AL_CHAOTIC,3,"treachery");
    
    e.EParam = sk;
    
    if (isPlayer())
      {
    
        tm = thisp->MyTerm;
      
        tm->LOption("Follow After Me", OrderWalkNearMe,
            "Base DC 10. Convince an NPC to follow you for a brief period, usually to lead "
            "them to monsters they will be hostile against. You receive half the "
            "experience for his kills after successfully using this request.");
        tm->LOption("Fight Against Enemy", OrderAttackTarget,
            "Base DC 15. Convince the NPC to fight against a specific enemy of your choosing, "
            "becoming hostile to that creature and its allies. You receive half "
            "of the experience for his kills after successfully using this request.");
        tm->LOption("Go To Location", OrderWalkToPoint,
            "Base DC 20. Convince the NPC to go to a specific location on the map. You receive "
            "half the experience for his kills after successfully using this request.");
        tm->LOption("Stop Fighting Unless Attacked", OrderBeFriendly,
            "Base DC 15. Convince an NPC to stop fighting and treat the creatures around it as "
            "neutrals for a brief period, even if it would usually be hostile to "
            "them.");
        tm->LOption("Take Watches With Me", OrderTakeWatches,
            "Base DC 10. Convince an NPC you are trustworthy to take watches with as you "
            "and he recover in the dungeon, thereby causing him to act like an ally when "
            "you rest, mitigating some of the danger of night-time encounters.");
        tm->LOption("Give Item to Me", OrderGiveMeItem,
            "Base DC 20. Convince an NPC to give you an item he has found in the dungeon. "
            "You cannot convince NPCs to give you their personal property (i.e., items they "
            "were generated with), though you can barter for it, but you can sometimes "
            "convince them to give you items they merely found, often by claiming it is "
            "yours by right. Once a creature possesses a dungeon item for a full day, however, "
            "it considers that item its personal property by right of plunder.");
        
        n = (TargetType) tm->LMenu(MENU_ESC|MENU_DESC|MENU_2COLS|MENU_BORDER,
          " -- Issue Which Request -- ",WIN_MENUBOX);

        xe.Clear();
        xe.EActor = e.EActor;
        Item *it; hObj hIt;
        switch (n) {
          case OrderWalkToPoint: 
            if (!tm->EffectPrompt(xe,Q_LOC,false,"Direct Creature Where"))
              return ABORT;
           break;
          case OrderAttackTarget:
            if (!tm->EffectPrompt(xe,Q_TAR|Q_CRE,false,"Attack What Creature"))
              return ABORT;
           break;
          case OrderGiveMeItem:
            if (!e.EVictim->FirstInv())
              {
                e.EActor->IPrint("The <Obj> has no inventory.",e.EVictim);
                return ABORT;
              }
            for(it=e.EVictim->FirstInv();it;it=e.EVictim->NextInv())
              tm->LOption(it->Name(0),it->myHandle);
            hIt = tm->LMenu(MENU_BORDER,"Which Item?");
            if (hIt == -1 || hIt == 0)
              return ABORT;
            e.EItem = oItem(hIt);
          }

      }
    else {
      n = OrderWalkNearMe;
      }
      
    /* Presumably, it takes some time to explain these things... */
    Timeout += 50;
      
    switch (n)
      {
        case OrderWalkNearMe:   DC = 10; break;
        case OrderBeFriendly:   DC = 15; break;
        case OrderWalkToPoint:  DC = 20; break;
        case OrderAttackTarget: DC = 15; break;
        case OrderTakeWatches:  DC = 10; break;
        case OrderGiveMeItem:   DC = 20; break;
      }
    
    /* LATER: Attack Target has a DC based on the racial hostility level
         between the NPC and the suggested target -- it's easier to get
         elves to attack dwarves than other elves. 
       Likewise, OrderByFriendly has a DC based on the highest hostility
         the creature has for those in its line of sight -- good luck
         getting dwarves to be friendly when drow are in LOS! */
    
    if (sk == SK_INTIMIDATE)
      if (e.EVictim->ResistLevel(AD_FEAR) == -1)
        { Timeout += 30; goto Failed; }
        
    c = 0; doGroup = false;
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(e.EVictim))
        c++;
    if (c > 1)
      if (yn(XPrint("Persuade the <Obj>'s whole group?",e.EVictim)))
        { doGroup = true; DC += 10; BestCR = 0;
          MapIterate(m,cr,i)
            if (cr->isCreature() && cr->isFriendlyTo(e.EVictim))
              if (cr->ChallengeRating() + (sk != SK_INTIMIDATE ? 0 :
                    cr->GetStatiMag(SAVE_BONUS,SN_FEAR)) > BestCR)
                BestCR = cr->ChallengeRating() + (sk != SK_INTIMIDATE ? 0 :
                    cr->GetStatiMag(SAVE_BONUS,SN_FEAR));     
          DC += BestCR*2;
        
        }
    if (!doGroup)
      DC += max(0,(e.EVictim->ChallengeRating() + (sk != SK_INTIMIDATE ? 0 :
                    e.EVictim->GetStatiMag(SAVE_BONUS,SN_FEAR)))*2);
    
    DC += e.EVictim->GetStatiMag(RETRY_BONUS,EV_REQUEST*100,this);
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(e.EVictim)) 
        cr->IncStatiMag(RETRY_BONUS,SS_MISC,EV_REQUEST*100,this,+3);   
        
    if (n == OrderGiveMeItem && !isBeside(e.EVictim))
      {
        IPrint("You need to get closer first.");
        return ABORT;
      }
        
    if (n == OrderGiveMeItem && e.EVictim->HasMFlag(M_SELLER))
      goto Failed;
    if (n == OrderGiveMeItem && (e.EItem->IFlags & IF_PROPERTY))
      {
        IPrint("That item is part of the <Obj>'s personal property.", 
          e.EVictim);
        return DONE;
      }
    smod = getSocialMod(e.EVictim,sk == SK_INTIMIDATE);
    if (!SkillCheck(sk,DC,true,smod,modBreakdown))
      {
        MapIterate(m,cr,i)
          if (cr->isCreature() && cr->isFriendlyTo(e.EVictim)) {
            cr->GainTempStati(TRIED,this,-2,SS_MISC,EV_REQUEST*100);
            if (sk != SK_DIPLOMACY)
              cr->ts.TurnHostileTo(cr,this);
            }
        Failed:
        if (sk != SK_DIPLOMACY)    
          IPrint("The <Obj><str> <str> and gets angry!", e.EVictim,
            c == 1 ? "" : "'s group",
            sk == SK_BLUFF ? "sees through your deception" :
            "resists your coercion");
        else
          IPrint("You fail to persuade the <Obj><Str>.",e.EVictim,
            c == 1 ? "" : "'s group");
            
        return DONE;      
      }
    
    MapIterate (m,cr,i)
      if (cr->isMonster() && cr->isFriendlyTo(e.EVictim)) {
        switch (n)
          {
            case OrderWalkNearMe:
              if (cr == e.EVictim)
                IPrint("The <Obj><str> agrees to follow you.", e.EVictim,
                        c == 1 ? "" : "'s group");
              cr->ts.giveOrder(cr,NULL,n,this); 
             break;
            case OrderBeFriendly:
              if (cr == e.EVictim)
                IPrint("You convince the <Obj><str> to stop fighting.", e.EVictim,
                        c == 1 ? "" : "'s group");
              cr->ts.giveOrder(cr,NULL,n); 
             break;
            case OrderWalkToPoint: 
              if (cr == e.EVictim)
                IPrint("You convince the <Obj><str> to investigate your story.", e.EVictim,
                        c == 1 ? "" : "'s group");            
              cr->ts.giveOrder(cr,this,n,NULL,xe.EXVal,xe.EYVal);
             break; 
            case OrderAttackTarget: 
              if (cr == e.EVictim)
                IPrint("You give the <Obj><str> reasons to smite your enemies.", e.EVictim,
                        c == 1 ? "" : "'s group");            
              cr->ts.giveOrder(cr,this,n,xe.EVictim);
             break; 
            case OrderTakeWatches:
              if (cr == e.EVictim)
                IPrint("The <Obj><str> finds your offer of mutual protection reasonable.", e.EVictim,
                        c == 1 ? "" : "'s group");            
              cr->ts.giveOrder(cr,this,n);
             break; 
            case OrderGiveMeItem:
              if (cr == e.EVictim) {
                IPrint("The <Obj> gives you the <Obj>.", e.EVictim, e.EItem);            
                e.EItem->Remove(false);
                GainItem(e.EItem,false);
                }
             break;
            
          }  
        if (n == OrderWalkNearMe ||
            n == OrderWalkToPoint ||
            n == OrderAttackTarget)
          if (!cr->HasStati(DIPLOMACY_TAG,-1,e.EActor))
            cr->GainTempStati(DIPLOMACY_TAG,e.EActor,-2,SS_MISC);
        }
  
    return DONE;
  }
  
EvReturn Creature::Surrender(EventInfo &e)
  {
    int16 CheckDC = 10, mod, i, BestCR, c, nHands;
    Creature *cr, *hasHands[64]; Item *it;
    
    if (!doSocialSanity(EV_SURRENDER,e.EVictim))
      return ABORT;    
    
 
    if (checkEndgameStuff(e))
      return DONE;
    
    /* If you're surrendering, then obviously you're going to
       be weaker then them; that shouldn't penalize the roll.
       But if by some chance you're /stronger/ and decide to
       surrender, the NPCs will happily take the opportunity
       not to have to fight you... */
    mod = - e.EVictim->getComparativeStrengthMod(this);
    if (mod < 0)
      CheckDC += mod;
    
    if (e.EVictim->isMType(MA_LAWFUL))
      CheckDC -= 5;
    
    if (e.EVictim->HasStati(TRIED,SK_DIPLOMACY+EV_SURRENDER*100,e.EActor))
      {
        IPrint("You've already tried to surrender to that creature "
          "or one of its allies.");
        return ABORT;
      }
    
    Timeout += 30;
    
    BestCR = 0; c = 0; nHands = 0;
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(e.EVictim)) {
        c = 0;
        cr->GainPermStati(TRIED,this,SS_MISC,SK_DIPLOMACY+EV_SURRENDER*100);
        if (cr->ChallengeRating() > BestCR)
          BestCR = cr->ChallengeRating();
        if (!cr->HasMFlag(M_NOHANDS))
          hasHands[nHands++] = cr;
        }
        
    
    if (!SkillCheck(SK_DIPLOMACY,CheckDC,true))
      {
        IPrint("Your pleas of surrender are ignored!");
        return DONE;
      }
      
    if (LastSkillCheckResult > CheckDC + 10)
      {
        IPrint("You negotiate a conditional surrender.");
        if (nHands)
          {
            if (getTotalMoney() > 750*BestCR)
              IPrint("The <Obj> claims <num> gold from you as a lein!", e.EVictim);
            else
              IPrint("All your gold is taken!");
            LoseMoneyTo(750*BestCR, hasHands[random(nHands)]);
            it = RandGoodInv(100);
            if (it) {
              IPrint("Your <Obj> is taken!", it);
              it->Remove(false);
              hasHands[random(nHands)]->GainItem(it,false);
              }
          }
        goto MakeNeutral;
      }
    
    if (nHands && FirstInv())
      {
        IPrint("All your treasures are taken!");
        
        Restart:
        for (it=FirstInv();it;it=NextInv())
          if (it->isMagic() || it->isType(T_COIN) || it->isType(T_GEM))
            {
              it->Remove(false);
              hasHands[random(nHands)]->GainItem(it,false);
              goto Restart;
            }
      }
    
    MakeNeutral:
    if (e.EVictim->getLeaderOrFirst()->isMType(MA_CHAOTIC) && !random(3))
      if (LastSkillCheckResult < CheckDC + 10)
        {
          IPrint("The <Obj><Str> laughs gleefully and keeps attacking!",
            e.EVictim, c > 1 ? "'s group" : "");
          return DONE;
        }
    
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(e.EVictim))
        cr->ts.TurnNeutralTo(cr,this);

    IPrint("The <Obj><Str> accepts your surrender and lets you go "
      "on your way.", e.EVictim, c > 1 ? "'s group" : "");    
    
    return DONE;
  }
  
EvReturn Creature::Taunt(EventInfo &e)
  {
    Creature *cr; bool doGroup;
    int16 i, c, BestWill, CheckDC;
    
    if (!doSocialSanity(EV_TAUNT,e.EVictim))
      return ABORT;    
    
    if (!HasSkill(SK_BLUFF))
      {
        IPrint("You need the Bluff skill to taunt people.");
        return ABORT;
      }
    
    if (e.EVictim->HasStati(TRIED,SK_BLUFF+EV_TAUNT*100,e.EActor))
      {
        IPrint("You've already tried to taunt that creature.");
        return ABORT;
      }
    
    BestWill = 0; c = 0;
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(e.EVictim))
        { c++;
          if (cr->GetAttr(A_SAV_WILL) > BestWill)
            BestWill = cr->GetAttr(A_SAV_WILL); }
    
    CheckDC = 15;
    
    if (c > 1 && yn("Taunt the whole encounter?"))
      { doGroup = true;
        CheckDC += 5 + BestWill*2; 
        MapIterate(m,cr,i)
          if (cr->isCreature() && cr->isFriendlyTo(e.EVictim))
            cr->GainTempStati(TRIED,this,-2,SS_MISC,SK_BLUFF+EV_TAUNT*100);
      }
    else {
      doGroup = false;
      CheckDC += e.EVictim->GetAttr(A_SAV_WILL)*2;
      e.EVictim->GainTempStati(TRIED,this,-2,SS_MISC,SK_BLUFF+EV_TAUNT*100);         
      }
      
    Timeout += 30;
      
    if (!SkillCheck(SK_BLUFF, CheckDC,true))
      {
        IPrint("The <Obj><Str> ignore<Str> your feeble insults.",
          e.EVictim, doGroup ? "and its allies" : "", doGroup ?
          "" : "s");
        return DONE;
      }
      
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(e.EVictim))
        if (doGroup || cr == e.EVictim)
          {
            cr->GainTempStati(ENRAGED,e.EActor,SkillLevel(SK_BLUFF),SS_MISC);
            cr->IDPrint("","The <Obj> flies into a mad rage at the <Obj>!",
              cr, e.EActor);
          }
          
    return DONE;
  }

bool Creature::isHostileToPartyOf(Creature *c)
  {
    Creature *cr; int32 i;
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(c))
        if (isHostileTo(cr))
          return true;
    return false;
  }

bool Creature::isHostileTo(Creature*c)
  {
    if (isPlayer() && c->isMonster())
      return c->isHostileTo(this);
    return (ts.SpecificHostility(this,c).quality == Enemy);
  }

bool Creature::isFriendlyTo(Creature*c)
  {
    if (c == this)
      return true;
    if (isPlayer() && c->isMonster())
      return c->isFriendlyTo(this);
    return (ts.SpecificHostility(this,c).quality == Ally);
  }
  
bool Creature::isPMType(int32 mt, Creature *cr)
  {
    if (mt > 256)
      return isPMType(mt % 256,cr) && isPMType(mt / 256,cr);
     
    /* Bluff conceals Alignment */ 
    if (mt == MA_GOOD || mt == MA_EVIL ||
        mt == MA_LAWFUL || mt == MA_CHAOTIC)
      if (SkillLevel(SK_BLUFF) > cr->SkillLevel(SK_APPRAISE))
        return cr->isMType(mt);
  
    if (HasStati(DISGUISED)) {
      rID spID;
      if (GetStatiMag(DISGUISED) < cr->SkillLevel(SK_SPOT)+
                                   cr->SkillLevel(SK_APPRAISE))
        goto IgnoreDisguise;
      if (cr->HasAbility(CA_SHARP_SENSES) && 
            GetStatiVal(DISGUISED) < 8)
        goto IgnoreDisguise;
      if (cr->HasAbility(CA_SCENT) && 
            GetStatiVal(DISGUISED) < 12)
        goto IgnoreDisguise;
      if (spID = GetStatiEID(DISGUISED))
        if (TEFF(spID)->Schools & SC_ILL)
          if (cr->HasStati(TRUE_SIGHT))
            goto IgnoreDisguise;
      return ((Creature*)GetStatiObj(DISGUISED))->isMType(mt);
      }
    IgnoreDisguise:
    
    return isMType(mt);
  
  }
  
int16 Creature::getComparativeStrengthMod(Creature *targ)
  {
    Creature *cr; int32 i, yourXCR, theirXCR, crXCR;
    rID adamantID; int16 yourCR, theirCR;
    /* Don't forget Adamant Facade! */
    
    /* If you are at 10% hit points, it's going to be a lot harder
       to convince predatory kobolds to stop fighting you; conversely,
       if you are at 90% and they're at 30%, they are now ready to
       talk. Good creatures don't care so much about this, because
       they aren't predatory and they will, on average, fight to the
       last against evil. 
       
       Later, we should include evaluation of fatigue, being stunned,
       blind, confused, etc. in the evaluation of comparative strength.     
     
     */


    yourXCR = theirXCR = 0;
    adamantID = FIND("Adamant Facade");
    
    
    /* The comparison is of the part of the actor's team the target can see
       versus the totality, visible and otherwise, of the target's team --
       if the player, with an invisible summoned monster, tries to intimidate
       a kobold with 5 friends hiding nearby, the friends count but the
       monster does not in determining the kobold's reaction. */
       
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(this))
        if (targ->Perceives(cr) || (cr == this))
          {
            crXCR = XCR(cr->ChallengeRating());
            if (!cr->HasEffStati(-1,adamantID)) 
              crXCR = (crXCR * ((cr->cHP*100)/cr->mHP)) / 100;
            yourXCR += crXCR;
          }
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr->isFriendlyTo(targ))
        {
          crXCR = XCR(cr->ChallengeRating());
          if (!cr->HasEffStati(-1,adamantID)) 
            crXCR = (crXCR * ((cr->cHP*100)/cr->mHP)) / 100;
          theirXCR += crXCR;
        }
    
    yourCR = XCRtoCR(yourXCR);
    theirCR = XCRtoCR(theirXCR);
    
    return yourCR - theirCR;
  }
  
int16 Creature::getSocialMod(Creature *cr, bool inc_str)
  {
    int16 mod; String str;
    
    mod = 0;
    str = "";
    
    #define RACE_MOD(A,B,val,reciprocal)                       \
      if ((cr->isMType(B) && isPMType(A,cr)) ||                \
          (reciprocal && (cr->isMType(A) && isPMType(B,cr))))  \
        { mod += val; str += Format(" %+d race",val);          \
          goto DoneRaceMod; }
    #define SOLIDARITY(R, val) RACE_MOD(R,R,val,false)
    
    /* Racial Solidarities */
    SOLIDARITY(MA_LIZARDFOLK,  +6);
    SOLIDARITY(MA_ILLITHID,    +4);
    SOLIDARITY(MA_DWARF,       +4);
    SOLIDARITY(MA_ELF,         +2);
    SOLIDARITY(MA_CELESTIAL,   +6);
    SOLIDARITY(MA_DEVIL,       +3);
    SOLIDARITY(MA_KOBOLD,      +2);
    SOLIDARITY(MA_GNOME,       +2);
    SOLIDARITY(MA_HALFLING,    +1);
    SOLIDARITY(MA_GOBLINOID,   +1);
    
    /* Drow know Drow treachery... */
    SOLIDARITY(MA_DROW,        -2);
    
    RACE_MOD(MA_LIZARDFOLK, MA_DRAGON, +3, true);
    RACE_MOD(MA_DWARF, MA_ELF, -1, true);
    RACE_MOD(MA_HUMAN, MA_HALFLING, +1, true);
    RACE_MOD(MA_DROW, MA_ELF, -6, false);
    RACE_MOD(MA_DROW, MA_DWARF, -6, false);
    RACE_MOD(MA_DROW, MA_DEMIHUMAN, -4, false);
    RACE_MOD(MA_KOBOLD, MA_GNOME, -6, false);
    RACE_MOD(MA_KOBOLD, MA_DEMIHUMAN, -4, false);
    RACE_MOD(MA_ORC, MA_ELF, -4, false);
    RACE_MOD(MA_ORC, MA_DWARF, -4, false);
    RACE_MOD(MA_DWARF, MA_GIANT, -4, true);
    RACE_MOD(MA_GOBLIN, MA_HALFLING, -3, false);
    RACE_MOD(MA_GOBLINOID, MA_DEMIHUMAN, -2, true);
    RACE_MOD(MA_GOOD, MA_CELESTIAL, +2, false);
    RACE_MOD(MA_EVIL, MA_CELESTIAL, -8, false);
    RACE_MOD(MA_LAWFUL, MA_DEVIL, +4, false);
    RACE_MOD(MA_CHAOTIC, MA_DEVIL, -4, false);
    RACE_MOD(MA_ALL, MA_DEMON, -10, true);
    RACE_MOD(MA_ABERRATION, MA_DEMIHUMAN, -4, false);
    RACE_MOD(MA_ELF, MA_DROW, -4, false);
    RACE_MOD(MA_DWARF, MA_EARTH, +2, true);
    RACE_MOD(MA_LIZARDFOLK, MA_WATER, +2, true);
    RACE_MOD(MA_ELF, MA_FAERIE, +2, true);
    
    
    /* Living Fear the Undead */
    RACE_MOD(MA_UNDEAD, MA_NLIVING, -4, false);
    /* Evil Undead hate the living */
    RACE_MOD(MA_NLIVING, MA_UNDEAD+MA_EVIL*256, -4, false);
    
    DoneRaceMod:
    
    if ((!cr->isMType(MA_EVIL)) && isPMType(MA_EVIL,cr))  
      { mod += -6; str += " -6 evil"; }
    else if (isMType(MA_EVIL) && isPMType(MA_EVIL,cr))  
      { mod += -2; str += " -2 evil"; }   
    else if (cr->isMType(MA_LAWFUL) && isPMType(MA_CHAOTIC,cr))
      { mod += -2; str += " -2 chaotic"; }
    else if (cr->isMType(MA_GOOD) && isPMType(MA_GOOD,cr))
      { mod += 2; str += " +2 good"; }
      
    if (HasFeat(FT_ALLURING))
      if ((StateFlags & MS_FEMALE) != (cr->StateFlags & MS_FEMALE))
        if ((isPMType(MA_DEMIHUMAN,cr) && cr->isMType(MA_DEMIHUMAN)) ||
            (isPMType(MA_GOBLINOID,cr) && cr->isMType(MA_GOBLINOID)) ||
            (isPMType(MA_REPTILIAN,cr) && cr->isMType(MA_REPTILIAN)))
          { mod += 3; str += " +3 alluring"; }
      
    if (cr->HasStati(CHARMED,CH_CHARM,this))
      { mod += 4; str += " +4 charm"; }      
      
    StatiIterNature(this, SOCIAL_MOD)
      int16 mag;
      if (S->Val && !cr->isMType(S->Val))
        continue;
      mag = S->Mag;
      if (S->eID && !TEFF(S->eID)->HasFlag(EF_MUNDANE))
        {
          if (cr->ResistLevel(AD_MIND) == -1)
            continue;
          if (mag > 0)
            mag = max(0, mag - cr->HighStatiMag(SAVE_BONUS,SN_ENCH));
          else
            mag = min(0, mag +  cr->HighStatiMag(SAVE_BONUS,SN_ENCH));
        }
      if (mag)
        {
          mod += mag;
          str += Format(" %+d %s", mag, (const char*) NAME(S->eID));
        }
    StatiIterEnd(this)

    if (inc_str && !cr->isMType(MA_GOOD))
      {
        int16 smod;
        smod = getComparativeStrengthMod(cr);
        if (smod)
          {
            mod += smod;
            str += Format(" %+d power", smod);
          }
      }
            
    modBreakdown = str;
    return mod;
  }
  
bool Creature::doSocialSanity(int16 ev, Creature *cr)
  {
    bool isTelep;
    isTelep = false;
    if (!canTalk(cr))
      {
        if (AbilityLevel(CA_TELEPATHY) >= DistFrom(cr))
          isTelep = true;
        else {
          IPrint("You can't talk in this form.");
          return false;
          }
      }
      
    if (m == cr->m &&
         !m->LineOfFire(x,y,cr->x,cr->y,this))
      {
        if (AbilityLevel(CA_TELEPATHY) >= DistFrom(cr))
          isTelep = true;
        else {
          IPrint("You cannot talk though solid barriers.");
          return false;
          }      
      }      
    if (ev != EV_DISTRACT && ev != EV_ORDER)
      if (!cr->canTalk(this))
        {
          if (cr->isMType(MA_SAPIENT) && 
              AbilityLevel(CA_TELEPATHY) >= DistFrom(cr))
            isTelep = true;
          else {
            IPrint("The <Obj> can't talk, <Obj>.", 
              cr, this);
            return false;
            }
        }
        
    if (m->FieldAt(x,y,FI_SILENCE))
      {
        IPrint("You can't talk in a region of magical silence.");
        return false;
      }
        
    if (HasStati(HIDING))
      {
        if (yn("Confirm reveal yourself by talking?"))
          Reveal(true);
        else
          return false;
      }
  
    if (cr->HasStati(ASLEEP))
      {
        if (yn(XPrint("Wake the <Obj>?",cr)))
          {
            if (cr->HasStati(ASLEEP,SLEEP_PSYCH) ||
                cr->HasStati(ASLEEP,SLEEP_DEEP) ||
                cr->HasStati(ASLEEP,SLEEP_FATIGUE))
              {
                IPrint("The <Obj> seems to be in a very deep slumber.",cr);
                return false;
              }
            cr->Awaken(-1);
          }
        else
          return false;
      }
    if (cr->HasStati(ENRAGED))
      {
        IPrint("<He:Obj>'s too furious to talk right now.",cr);
        return false;
      }
    if (cr->HasStati(PARALYSIS))
      {
        if (DistFrom(cr) <= AbilityLevel(CA_TELEPATHY))
          isTelep = true;
        else {
          IPrint("Being paralyzed, the <Obj> really isn't a great "
            "conversationalist at the moment.",cr);
          return false;
          }
      }
    
    if (isTelep)
      IPrint("You speak with the <Obj> telepathically.", cr);
    
    if (ev != EV_COW && ev != EV_DISTRACT)
      {
        if (cr->HasStati(STUNNED))
          {
            IPrint("The <Obj> staggers but does not respond.",cr);
            return false;
          }
        if (cr->HasStati(CONFUSED))
          {
            IPrint("The <Obj> babbles incoherantly in response.",cr);
            return false;
          }
      }
      
    if (ev == EV_TERMS || ev == EV_QUELL || ev == EV_COW ||
        ev == EV_SURRENDER || ev == EV_REQUEST)
      {
        StatiIterNature(this,ENEMY_TO)
          if ((S->Val && ((S->Val == -1) || cr->isMType(S->Val))) ||
               S->h == cr->myHandle)
            {
              switch(ev)
                {
                  case EV_TERMS:
                    IPrint("The <Obj1> is frightened, but you are too "
                      "repellant to <him:Obj1> for <him:Obj1> to surrender.", cr);
                   break;
                  case EV_QUELL:
                  case EV_REQUEST:
                    IPrint("The <Obj1> will hear no reason from the likes of you!", cr);
                   break;
                  default:
                    IPrint("The <Obj1> growls at you with an instinctive hate.", cr);
                   break;
                }
              StatiIterBreakout(this,return false);
            }
        StatiIterEnd(this);
      }
      
    return true;   
  }
  
int32 Creature::getTotalMoney()
  {
    Item *it; int32 cp;
    
    cp = 0;
    for (it=FirstInv();it;it=NextInv())
      if (it->Type == T_COIN)
        cp += it->Quantity * TITEM(it->iID)->Cost;
    return cp;
  }

int SortMoney(const void *a, const void *b)
  {
    #define A (*((Item**)a))
    #define B (*((Item**)b))
    
    return TITEM(B->iID)->Cost -
             TITEM(A->iID)->Cost;
  }
    

void Creature::LoseMoneyTo(int32 amt, Creature *cr)
  {
    /* Assumptions: money is not cursed. It does not do wierd
       things when you gain, lose, wield or unequip it. There
       is no magic that changes the value of money, and no
       coins with an eID or magical plus. */
    #define VALUE(it) (TITEM(it->iID)->Cost * it->GetQuantity())
    #define VONE(it)  (TITEM(it->iID)->Cost)
    Item *Stacks[64], *Money[64], *it, *it2; 
    int16 nStacks, nCoins; int32 needed, n, i, j;
    bool exact_change = true;
    
    amt *= 100;
    
    if (getTotalMoney() < amt)
      amt = getTotalMoney();
    if (!amt)
      return;
    
    

    /* Get all the creature's money */
    nCoins=0;
    for (it=FirstInv();it;it=NextInv())
      if (it->isType(T_COIN) && !it->eID)
        Money[nCoins++] = it;
        
    /* sort it by size of denomination */
    qsort(Money,nCoins,sizeof(Item*),SortMoney);

    nStacks = 0; needed = amt;
    /* Cycle through all stacks, starting with those
       with the largest denomination -- platinum first,
       then gold, etc. */
    for(i=0;i!=nCoins;i++)
      {
        it = Money[i];
        if (VALUE(it) <= needed)
          {
            Stacks[nStacks++] = it;
            needed -= VALUE(it);
          }
        else if (VONE(it) > needed)
          continue;
        else {
          n = needed / VONE(it);
          it2 = it->TakeOne();
          it2->SetQuantity(n);
          it->SetQuantity(it->GetQuantity() - (n-1));
          Stacks[nStacks++] = it2;
          needed -= VALUE(it2);
          }
      }
      
    /* Okay, so now the player has payed everything he can
       without overpaying, starting with the biggest coins
       first. Now, if there's anything left over, we want to 
       pay the remainder with a SINGLE coin of a smallest
       demonination the player has. */
    if (!needed)
      goto GiveStacks;
    
    for (i=nCoins-1;i!=-1;i--)
      {
        bool isStack = false;
        for (j=0;j!=nStacks;j++)
          if (Stacks[j] == Money[i])
            isStack = true;
        if (!isStack)
          {
            it = Money[i];
            Stacks[nStacks++] = it->TakeOne();
            break;
          }
      }
    
    GiveStacks:
    for (i=0;i!=nStacks;i++)
      {
        if (cr == NULL)
          Stacks[i]->Remove(true);
        else {
          Stacks[i]->Remove(false);
          cr->GainItem(Stacks[i],false);
          }
      }
   
  }
  
int32 Item::getShopCost(Creature *Buyer, Creature *Seller) {
    double cost;
    int16 i;
    bool plusPriced; 
    int32 priceList[10];
    
    plusPriced = false;
    cost = TITEM(iID)->Cost;
    if (TITEM(iID)->GetList(ITEM_COST,(rID*)priceList,10))
      { cost = priceList[GetInherentPlus()] * 100L; 
        plusPriced = true; }
    else if (isType(T_BOOK))
      {
        cost = 0;
        for (i=0;i!=MAX_SPELLS;i++)
          if (HasSpell(i))
            cost += max(200,150 * TEFF(theGame->SpellID(i))->Level);
      }
      
    if (isType(T_SCROLL) || isType(T_POTION))
      {
        if (eID)
          cost += (TEFF(eID)->Level + 3) * (isType(T_POTION) ? 50 : 20) *
                   10;
      }
    else if (isType(T_WAND)) {
        if (eID)
          cost += 250L * ItemLevel() * ItemLevel();
      }


    if (eID || (GetInherentPlus() && !plusPriced))
      {
        if (eID && TEFF(eID)->GetList(ITEM_COST,(rID*)priceList,10))
          cost += priceList[GetInherentPlus()] * 100;
        else {
          double defCost[] = {
            500, 1000, 2000, 3000, 4000, 6000, 8000,
            12000, 16000, 24000, 36000, 48000, 56000,
            75000, 102000, 128000, 256000, 512000,
            1000000, 1500000, 2000000 };
          if (isType(T_WEAPON) || isType(T_ARMOUR) ||
              isType(T_SHIELD) || isType(T_BOW))
            cost += defCost[max(0,min(20,ItemLevel(false)))] * (eID ? 400L : 160L);
          else    
            cost += defCost[max(0,min(20,ItemLevel(false)))] * 70L;
          }
      }
          
    if (cost) {
      cost *= Quantity;
      cost = max(1, cost / 100);
      }
    
    cost /= 3;
    
    if (!Seller || !Buyer)
      return (int32)cost;
      
    double ShopPrices[] = { 
      /* -10 to -6*/
      5000, 4750, 4500, 4250, 4000, 
      
      /* -5 to -1*/
      3750, 3500, 3250, 3000, 2750,
      
      /* 0 to +4 */
      2500, 2250, 2000, 1500, 1000,
      
      /* +5 to +9 */
      900, 800, 700, 600, 500,
      
      /* +10 to +14 */
      450, 400, 350, 300, 250,
      
      /* +15 to +19 */
      225, 200, 190, 180, 170,
      
      /* +20 to +24 */
      160, 155, 150, 145, 140,
      
      /* +25 to +30 */
      135, 130, 125, 123, 122, 120 };
    double BarterPrices[] = {
      /* -10 to -6 */
      500, 450, 400, 350, 300,
      
      /* -5 to -1 */
      275, 250, 225, 200, 175,
      
      /* 0 to +4 */
      150, 130, 110, 100, 100,
      
      /* +5 to +9 */
      100, 95, 90, 85, 80, 75,
      
      /* +10 to +14 */
      70, 68, 66, 64, 62,
      
      /* +15 to +19 */
      60, 58, 56, 54, 52,
      
      /* +20 to +24 */
      50, 48, 46, 44, 42,
      
      /* +25 to +30 */
      40, 38, 36, 34, 32, 30 };
    
    int32 index;
    
    index = Buyer->SkillLevel(SK_DIPLOMACY);
    
    index += Buyer->getSocialMod(Seller,false);
    index = max(index,-10);
    index = min(index,+30);
    
    if (Seller->HasMFlag(M_SELLER))
      cost *= ShopPrices[index + 10];
    else {
      if (Seller->getLeader() == Buyer)
        index += 10;
      cost *= BarterPrices[min(index + 10,40)];
      }
      
    cost = max(1,cost/100.0);
    
      
    return (int32)cost;
  }
  
Creature * InitShopkeeper(rID mID, int32 gold)
  {
    Creature *cr; Item *gp, *it;
    cr = new Monster(mID);
    cr->CalcValues();
    TMON(mID)->GrantGear(cr,mID,true);
    gp = Item::Create(FIND("gold piece"));
    gp->SetQuantity(gold);
    cr->GainItem(gp,true);
    for (it=cr->FirstInv();it;it=cr->NextInv()) {
      it->IFlags &= (IF_CURSED|IF_BLESSED);
      if (it->eID && TEFF(it->eID)->HasFlag(EF_CURSED))
        it->Remove(true);
      }
    return cr;
  }
  
/* -- PARTY-RELATED FUNCTIONS -- */

rID getAnimalName() 
{
  int c = 0;
  for (int m=0;theGame->Modules[m];m++)
    for (int j=0;j!=theGame->Modules[m]->szFla;j++)
      if ((theGame->Modules[m]->QFla+j)->IType == AI_ANIMAL_NAME) {
        Candidates[c++] = theGame->Modules[m]->FlavorID(j);
        if (c >= 2000)
          Fatal("Too many animal name flavours!");
      }
  if (!c)
    Fatal("Not enough animal name flavours!");
  return Candidates[random(c)];
} 

#if 0
void Player::WildShape()
{
  if (ResistLevel(AD_POLY) == -1) {
    IPrint("You find you cannot change form!");
    return;
  }
  int lev = AbilityLevel(CA_WILD_SHAPE);
  // already includes feats like FT_WILD_SHAPE_MASTERY
  if (lev < 3) {
    IPrint("You need three druid levels to assume a Wild Shape!");
    return;
  } 
  int i;
  Creature *c;

  // find all Animals with HD <= 'lev'
  for (int modIdx = 0; modIdx < 1; modIdx++) {
    Module *mod = theGame->Modules[modIdx];
    for (int idx = 0; idx < mod->szMon; idx++) {
      if (mod->QMon[idx].isMType(MA_ANIMAL) ||
          mod->QMon[idx].isMType(MA_BEAST) ||
          (lev >= 6 && mod->QMon[idx].isMType(MA_PLANT)))
        ; // good, you can be these things
      else {
        // maybe we have the ADDITIONAL_WILD_SHAPE_MA_TYPE stati
        bool good = false; 
        for(i=0;Stati[i];i++) {
          if (Stati[i]->Nature == ADDITIONAL_WILD_SHAPE_MA_TYPE &&
              mod->QMon[idx].isMType(Stati[i]->Val))
            good = true; 
        } 
        if (!good) continue;
      } 
      if (mod->QMon[idx].isMType(MA_LYCANTHROPE))
        continue; 
      if (mod->QMon[idx].HasFlag(M_NOGEN) || 
          mod->QMon[idx].HasFlag(M_NOPOLY))
        continue;
      /* new way: big sizes count as "more hit dice" */
      if (mod->QMon[idx].HitDice +
          max(mod->QMon[idx].Size - SZ_MEDIUM,0) +
          max(SZ_SMALL - mod->QMon[idx].Size,0) 
          > lev) 
        continue;
      /* old way: you need to be a certain level to get big sizes ...
      if (mod->QMon[idx].Size > SZ_MEDIUM && lev < 8) 
        continue;
      if (mod->QMon[idx].Size < SZ_SMALL && lev < 11) 
        continue;
        */
      // ww: later restrict this to animals we have seen at least once
      rID mid = mod->MonsterID(idx);
      Monster *mon = new Monster(mid);
      mon->CalcValues(); 
      MyTerm->LOption(NAME(mid),mid,mon->Describe(this),
          999 - mod->QMon[idx].HitDice);
      delete mon; 
    }
  }
  rID mid = MyTerm->LMenu(MENU_ESC|MENU_DESC|MENU_3COLS|MENU_SORTED,
      "Wild Shape Into What Type of Creature?", WIN_MAP,0);
  if (mid == (unsigned long)-1) return;

  IPrint("You turn into a <Res>.",mid);
  Shapeshift(mid,true,NULL);   
  GainPermStati(POLYMORPH,NULL,SS_ENCH,-1,-1,mid);
  // this little hackery shouldn't be necessary, but if we don't do it you
  // lose half your mana somewhere in the middle
  CalcValues();
  return; 
}

// ww: following the SRD, somewhat ...
void Player::SummonDruidAnimal()
{
  String s;
  int lev = AbilityLevel(CA_DRUID_ANIMAL);
  if (lev <= 0) return; 
  int ft_bonus = HasFeat(FT_ANIMAL_BOND) ? Mod(A_CHA) / 3 : -2; 
  int i;
  Creature *c;

  // find all Animals with CR <= 'lev'
  for (int modIdx = 0; modIdx < 1; modIdx++) {
    Module *mod = theGame->Modules[modIdx];
    for (int idx = 0; idx < mod->szMon; idx++) {
      if (!(mod->QMon[idx].isMType(MA_ANIMAL)))
        continue;
      if (mod->QMon[idx].HasFlag(M_NOGEN))
        continue;
      // ww: sorry, no animal companions that can hold the +5 sword of
      // death for you ...
      if (!mod->QMon[idx].HasFlag(M_NOHANDS))
        continue;
      if (mod->QMon[idx].CR > lev) 
        continue;
      // ww: later restrict this to animals we have seen at least once
      rID mid = mod->MonsterID(idx);
      Monster *mon = new Monster(mid);
      int diff = ((1 + lev + ft_bonus) - max(mon->ChallengeRating(),0));
      if (diff < 1) diff = 1;
      else if (diff > 12) diff = 12;
      rID tid = FIND(Format("companion;%d",diff));
      mon->AddTemplate(tid);
      MyTerm->LOption(NAME(mid),mid,mon->Describe(this),
          mod->QMon[idx].CR);
      delete mon; 
    }
  }
  rID mid = MyTerm->LMenu(MENU_ESC|MENU_DESC|MENU_3COLS|MENU_SORTED,
      "What Type of Animal Companion?", WIN_MAP,0);
  if (mid == (unsigned long)-1) return;

  // have we already done this today? 
  if (HasStati(TRIED,(int8)HAS_DRUID_ANIMAL,NULL)) {
    IPrint("You may only summon one Druid Animal per day; rest first.");
    return;
  } 

  // Wait, do we already have a druid animal? 
  MapIterate(m,c,i)    
    if (c->isMonster())          
      if (c->HasStati(IS_DRUID_ANIMAL,TA_LEADER,this))
      {
        s = Format("Dismiss %s, your current Druid Animal?",(const char *)c->Name(0));
        if (yn(s,false)) {
          // shouldn't really be death, but this is easier for now ...
          ThrowDmg(EV_DEATH,AD_NORM,0,"Druidic whim",c,c);
        } else {
          IPrint("You may only have one Druid Animal!");
          return; 
        }
      }

  Monster *mn = new Monster(mid);
  mn->PlaceAt(m,x,y);
  int diff = ((1 + lev + ft_bonus) - max(mn->ChallengeRating(),0));
  if (diff < 1) diff = 1;
  else if (diff > 12) diff = 12;
  rID tid = FIND(Format("companion;%d",diff));
  mn->AddTemplate(tid);
  mn->Initialize(true);
  mn->Named = NAME(getAnimalName());
  mn->RemoveStati(TARGET);
  mn->ts.addCreatureTarget(this,TargetMaster);
  mn->GainPermStati(IS_DRUID_ANIMAL,this,SS_MONI,TA_LEADER,0,0);
  RemoveStati(HAS_DRUID_ANIMAL,SS_MONI,TA_LEADER,0,false);
  GainPermStati(HAS_DRUID_ANIMAL,mn,SS_MONI,TA_LEADER,0,0);
  GainPermStati(TRIED,mn,SS_MONI,HAS_DRUID_ANIMAL,0,0);
  mn->PartyID = PartyID;
  mn->ts.Retarget(mn);
}
#endif 

void Player::InitCompanions()
  {
    rID mID; Monster *mn;
    TClass * tc = TCLASS(ClassID[0]);
    if (tc->HasFlag(CF_PALADIN)) { 
      if (isMType(MA_DROW) || isMType(MA_LIZARDFOLK))
        mID = FIND("riding lizard");
      else if (isMType(MA_GNOME))
        mID = FIND("riding dog");
      else if (Attr[A_SIZ] < SZ_MEDIUM)
        mID = FIND("pony");
      else 
        mID = FIND("horse");
      if (!mID)
        return;
      mn = new Monster(mID);
      mn->PlaceAt(m,x,y);
      mn->Initialize(true);
      mn->MakeCompanion(this,PHD_ANIMAL);
      mn->Named = NAME(getAnimalName());
    } 
    RES(ClassID[0])->PEvent(EV_INIT_COMP,this,this,ClassID[0]);
    
    // ww: let the player use the ability to select this
#if 0
    if (HasAbility(CA_ANIMAL_COMP)) {
      MaxCR = MaxGroupCR(PHD_ANIMAL);
      MaxCR = max(1,min(TotalLevel()+1,MaxCR));
      mID = theGame->GetMonID(PUR_SUMMON,0,MaxCR,MaxCR,MA_ANIMAL);
      if (!mID)
        return;
      mn = new Monster(mID);
      mn->PlaceAt(m,x,y);
      mn->Initialize(true);
      mn->MakeCompanion(this,PHD_ANIMAL);
      mn->Named = NAME(getAnimalName());
      }
#endif
  }

extern const char* alliesShouldntCast[];

bool Monster::MakeCompanion(Player *p, int16 CompType)
  {
    int16 Total,i; rID eID;

    if (eID = GetStatiEID(SUMMONED))
      if (TEFF(eID)->HasFlag(EF_XSUMMON))
        goto SkipPHDCheck;
    if (CompType == PHD_FREEBIE) {
      if (!HasStati(NO_PHD,-1,p))
        GainPermStati(NO_PHD,p,SS_MISC);
      goto SkipPHDCheck;
      }
    if (isIllusion())
      goto SkipPHDCheck;

    Total = XCRtoCR(XCR(p->MaxGroupCR(CompType)) - 
                          XCR(p->GetGroupCR(CompType,ChallengeRating())));
    if (CompType != PHD_PARTY) 
      Total = XCRtoCR(
        XCR(Total) +
        max(0,XCR(p->MaxGroupCR(PHD_PARTY)) - 
          XCR(p->GetGroupCR(PHD_PARTY))));
    if (Total < 0)
      {
        switch (CompType) {
          case PHD_UNDEAD:
            return false;
          case PHD_ANIMAL:
            p->IPrint("You don't feel you can control that many animals "
                      "at the same time.");
            return false;
          case PHD_MAGIC:
            if (p->SavingThrow(WILL,15+ChallengeRating())) {
              p->IPrint("Since your psychic reserves are not great enough "
                        "to fully control the <Obj>, you content yourself "
                        "with pacifying it.",this);
              if (HasStati(CHARMED))
                SetStatiVal(CHARMED,p,CH_CHARM);
              else
                StateFlags |= MS_PEACEFUL;
              }
            else {
              p->IPrint("The <Obj> breaks free of your control!",this);
              RemoveStati(CHARMED);
              }              
            return false;
          case PHD_COMMAND:
            p->IPrint("Since your psychic reserves are not great enough "
                      "to fully control the <Obj1>, you content yourself "
                      "with terrifying <him:Obj1>.",this);
            RemoveStati(CHARMED);
            GainPermStati(AFRAID,NULL,SS_MISC);
            return false;
          default:
            return false;
          }
      }
    
    SkipPHDCheck:
    ts.addCreatureTarget(p, TargetSummoner);
    ts.Retarget(this);
    if (p->Opt(OPT_WAIT_FOR_PETS))
      ts.giveOrder(this,p,OrderWalkNearMe);
    if (CompType != PHD_MAGIC)
      PartyID = p->PartyID;
    
    if (isMType(MA_ADVENTURER) && !HasStati(HEROIC_QUALITY) && !isMType(MA_UNDEAD) 
         && !HasStati(SUMMONED) && !HasStati(ILLUSION) && CompType == PHD_PARTY)
      {
        p->IPrint("You sense a certain heroic quality about the <Obj>...",this);
        GainPermStati(HEROIC_QUALITY,NULL,SS_MISC);
        cHP += 20;
      }

    for (i=0;alliesShouldntCast[i];i++)
      if (FIND(alliesShouldntCast[i]))
        RemoveEffStati(FIND(alliesShouldntCast[i]));

    CalcValues();
    
    IdentifyMon();
    StatiIterNature(this,TEMPLATE)
      IdentifyTemp(S->eID);
    StatiIterEnd(this)
    
    
    
    Creature *cr;
    if (m) {
      MapIterate(m,cr,i)
        if (cr->isCreature())
          if (cr->getLeader() == p) {
            cr->ts.removeCreatureTarget(this, TargetAny);
            cr->ts.Retarget(cr,true);
            }
      }
    
    return true;
  }

int32 Player::GetGroupXCR(int16 CompType, int16 AddCR)
  {
    int32 i,j,ct; Creature *c; int32 CRCubed;

    #define CUBE(v) (tmp=v+2,tmp*tmp*tmp)
    
    CRCubed = 0;
    MapIterate(m,c,i)
      {   
        if (c->isMonster() && c->ts.isLeader(this))          
          {
            if (c->isIllusion())
              continue;
            if (c->HasStati(NO_PHD,-1,this))
              continue;
            if (c->HasStati(SUMMONED))
              {
                rID eID = c->GetStatiEID(SUMMONED);
                if (eID && TEFF(eID)->HasFlag(EF_XSUMMON))
                  continue;
              }
            if (c->HasStati(CHARMED,CH_DOMINATE,this) ||
                c->HasStati(SUMMONED,-1,this))
              ct = PHD_MAGIC;
            else if (c->HasStati(CHARMED,CH_COMMAND,this))
              ct = PHD_COMMAND;
            else if (c->isMType(MA_ANIMAL))
              ct = PHD_ANIMAL;
            else if (c->isMType(MA_UNDEAD))
              ct = PHD_UNDEAD;
            else
              ct = PHD_PARTY;
            if (ct != CompType)
              continue;
            j = c->ChallengeRating();
            CRCubed += max(10,((j+2)*(j+2)*(j+2)));
          }
      }
    
    if (AddCR)
      CRCubed += max(0, (AddCR+2) * (AddCR+2) * (AddCR+2));

    /* The CR of party HD includes the CRs of groups that
       overflow into it. For example, a ranger has a 2 HD 
       per level buffer for animal companions. If a 6th
       level ranger has 15 HD of animals, then he must have
       3 fewer HD worth of party members. */
    
    if (CompType == PHD_PARTY) {
      if (GetGroupCR(PHD_ANIMAL) > max(0,MaxGroupCR(PHD_ANIMAL)))
        {
          CRCubed += GetGroupXCR(PHD_ANIMAL);
          CRCubed -= MaxGroupXCR(PHD_ANIMAL);
        }
      if (GetGroupCR(PHD_MAGIC) > max(0,MaxGroupCR(PHD_MAGIC)))
        {
          CRCubed += GetGroupXCR(PHD_MAGIC);
          CRCubed -= MaxGroupXCR(PHD_MAGIC);
        }
      if (GetGroupCR(PHD_COMMAND) > max(0,MaxGroupCR(PHD_COMMAND)))
        {
          CRCubed += GetGroupXCR(PHD_COMMAND);
          CRCubed -= MaxGroupXCR(PHD_COMMAND);
        }
      }

    /* PHD_UNDEAD can also overflow into PHD_MAGIC -- any mage can
       control created undead with the same pool they use for charmed
       or summoned creatures, but necromancers can use the sum of their
       bonus undead pool and the normal magic pool to control created
       undead. */
    if (CompType == PHD_MAGIC)
      if (GetGroupCR(PHD_UNDEAD) > max(0,MaxGroupCR(PHD_UNDEAD)))
        {
          CRCubed += GetGroupXCR(PHD_UNDEAD);
          CRCubed -= MaxGroupXCR(PHD_UNDEAD);
        }

    return CRCubed;
  }      

int32 Player::MaxGroupXCR(int16 CompType)
  { return XCR(MaxGroupCR(CompType)); }
  
int16 Player::MaxGroupCR(int16 CompType)
  {
    int16 bonus;
    switch (CompType)
      {
        case PHD_PARTY:  bonus = ((SkillLevel(SK_DIPLOMACY) - 5) / 5 ) * 2;
                         bonus = max(0, bonus);
                         return TotalLevel() + Mod(A_CHA) + bonus +
                                  (HasFeat(FT_LEADERSHIP) ? 3 : 0) + 
                                  HighStatiMag(BONUS_PHD,CompType);
        case PHD_MAGIC:  
          return max(0,(CasterLev() + TotalLevel())/2)
                   + AbilityLevel(CA_COMMAND_AUTHORITY)  + 
                             HighStatiMag(BONUS_PHD,CompType);
        case PHD_ANIMAL: 
          if (!HasAbility(CA_ANIMAL_COMP))
            return -10;
          return max(1,AbilityLevel(CA_ANIMAL_COMP))  + 
                                  HighStatiMag(BONUS_PHD,CompType);
        case PHD_COMMAND:
          if (!HasAbility(CA_COMMAND))
            return -10;
          return max(1,HighStatiMag(COMMAND_ABILITY))
                   + AbilityLevel(CA_COMMAND_AUTHORITY)  + 
                                  HighStatiMag(BONUS_PHD,CompType);
        case PHD_UNDEAD:
          return HighStatiMag(BONUS_PHD,CompType);
        default: 
          return TotalLevel(); // paranoia
      }
  }

int16 Player::FixSummonCR(int16 enCR, int16 CompType)
  {
    int32 Total;
    enCR++;
    do {
      enCR--;
      Total = MaxGroupXCR(CompType);
      Total -= GetGroupXCR(CompType,enCR);
      if (CompType != PHD_PARTY) 
        Total += 
          MaxGroupXCR(PHD_PARTY) - 
          GetGroupXCR(PHD_PARTY);
      }
    while (Total < 0 && enCR > -1);
    return enCR;
  }