/* SKILLS.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Implements the skill and special ability menu, the cancel
   active function command and the effects of all of the ind-
   ividual actively-used skills, racial powers and class
   abilities.
   
     void Player::UseMenu()
     void Player::CancelMenu()
     void Character::UseSkill(uint8 sk)
     bool Creature::SkillCheck(int16 sk, int16 DC, bool show)
     void Character::UseFeat(uint8 ft)
     void Character::UseAbility(uint8 ab,int16 pa)
     bool Creature::UseLimitedFA()
     EvReturn Creature::Hide(EventInfo &e)  
     EvReturn Creature::Search(EventInfo &e)
     void Creature::Devour(rID cmID)
     void Creature::Reveal(bool intentional)
     void Creature::Awaken(int8 MoveSil)
     void Creature::FocusCheck(Creature *killer)
     EvReturn Creature::Turn(EventInfo &e)
     EvReturn Creature::Phase(EventInfo &e)
     void Creature::SetupDig(Dir d)
     EvReturn Creature::Dig(EventInfo &e)      
     void Character::LegendIdent(Item *it)
     bool Creature::ItemPrereq(rID xID, int16 ReqLevel, int16 TrickDC)

*/    

#include "Incursion.h"

int16 LastSkillCheckResult = 0;

/* Elf, Grey -> Grey Elf */
String & DeSubrace(const char *RaceName)
  {
    int16 l;
    if (!strstr(RaceName,", "))
      return *tmpstr(RaceName);
    l = strstr(RaceName,", ") - RaceName;
    return *tmpstr(Right(RaceName, strlen(RaceName) - (l+2)) +
           SC(" ") + Left(RaceName, l));
  }

String & DescribeSkill(int16 sk)
{
  String res, rn; int16 i, j; rID xID;

  res = "";
  if (!SkillInfo[sk].imp)
    res += Format("%c(NOT IMPLEMENTED!) ", -PINK);

  if (SkillInfo[sk].active) 
    res += Format("%c(Active) ",-EMERALD);
  else
    res += Format("%c(Passive) ",-YELLOW);

  if (isFamiliar(sk))
    res += Format("%c[Familiar] ",-BLUE);

  if (SkillInfo[sk].attr1 == SkillInfo[sk].attr2) {
    res += Format("%c%s", -SKYBLUE, AttrTitle[SkillInfo[sk].attr1]);
  } else {
    if (SkillInfo[sk].take_min) 
      res += Format("%cworst%c of", -PINK, -GREY);
    else 
      res += Format("%cbest%c of", -EMERALD, -GREY);
    res += Format(" %c%s%c and %c%s",
        -SKYBLUE, 
        AttrTitle[SkillInfo[sk].attr1],
        -GREY, 
        -SKYBLUE, 
        AttrTitle[SkillInfo[sk].attr2]);
  } 

  if (SkillInfo[sk].armour_penalty == 2) 
    res += Format(" %c[DOUBLE armour penalty]",-RED);
  else if (SkillInfo[sk].armour_penalty) 
    res += Format(" %c[armour penalty]",-RED);
  
  res += Format("\n%c__Possessed By:%c Some Humans",-13,-7);
  for (int mIdx=0;theGame->Modules[mIdx];mIdx++) {
    Module * m = theGame->Modules[mIdx];
    for (i=0; i<m->szRac; i++) {
      xID = theGame->Modules[mIdx]->RaceID(i);
      for (j=0;j!=12;j++)
        if (TRACE(xID)->Skills[j] == sk)
          res += Format(", %s", (const char*)
            Pluralize(DeSubrace(NAME(xID))));
      }
    }
  for (int mIdx=0;theGame->Modules[mIdx];mIdx++) {
    Module * m = theGame->Modules[mIdx];
    for (i=0; i<m->szCla; i++) {
      xID = theGame->Modules[mIdx]->ClassID(i);
      if (TCLASS(xID)->HasFlag(CF_PSEUDO))
        continue;
      for (j=0;j!=40;j++)
        if (TCLASS(xID)->Skills[j] == sk)
          res += Format(", %s", (const char*)
            Pluralize(NAME(xID)));
      }
    }
  // ugly kludge, but the nice way's a lot of work
  switch(sk)
    {
      case SK_DIPLOMACY:
      case SK_APPRAISE: res += ", Community priests"; break;
      case SK_CRAFT:     res += ", Craft priests"; break;
      case SK_SPOT:
      case SK_LISTEN:    res += ", Guardian priests"; break;
      case SK_USE_MAGIC:
      case SK_DECIPHER:  res += ", Magic priests"; break;
      case SK_WILD_LORE: res += ", Plant priests"; break;
      case SK_FIND_WEAKNESS:
      case SK_SEARCH:    res += ", Retribution priests"; break;
      case SK_BLUFF:
      case SK_PICK_POCKET:
      case SK_HIDE:
      case SK_MOVE_SIL:  res += ", Trickery priests"; break;
    }
  if (sk >= SK_KNOW_FIRST && sk <= SK_KNOW_LAST)
    res += ", Knowledge priests";
  if (xID = FIND(Format("help::%s",SkillInfo[sk].name)))    
    res += Format(".\n%c__Effects:%c%s",-YELLOW,-CYAN,DESC(xID));
  else
    res += Format(".\n%c__Effects:%c%s",-YELLOW,-CYAN,SkillInfo[sk].desc);

  return *tmpstr(res); 
} 

void Player::UseMenu(int32 SuppliedIndex) {
    int32 i = 0;
    uint16 j;
    bool found = false;
    EventInfo e; String str;

    if (SuppliedIndex) {
        i = SuppliedIndex;
        goto HasSuppliedIndex;
    }

    while(ActiveTraits[i]) {
        switch((ActiveTraits[i] / 1000)*1000) {
        case ABIL_VAL: /* Class Ability */
            if (ActiveTraits[i] == CA_TURNING + ABIL_VAL) {
                StatiIterNature(this,TURN_ABILITY)
                    str = "Turn "; str += Pluralize(Lookup(MTypeNames,S->Val));
                    MyTerm->LOption(str,i+(S->Val*1000L)); found = true;
                StatiIterEnd(this)
            } else if (ActiveTraits[i] == CA_COMMAND + ABIL_VAL) {
                StatiIterNature(this,COMMAND_ABILITY)
                    str = "Command "; str += Pluralize(Lookup(MTypeNames,S->Val));
                    MyTerm->LOption(str,i+(S->Val*1000L)); found = true;
                StatiIterEnd(this)
            } else if (ActiveTraits[i] == CA_GREATER_TURNING + ABIL_VAL) {
                StatiIterNature(this,GTURN_ABILITY)
                    if (S->Val == MA_UNDEAD)
                        str = "Greater Turning";
                    else
                    { str = "Greater Turning: "; 
                    str += Lookup(MTypeNames,S->Val); }
                    MyTerm->LOption(str,i+(S->Val*1000L)); found = true;
                StatiIterEnd(this)
            } else if (ActiveTraits[i] == CA_ARCANE_TRICKERY + ABIL_VAL) {
                int16 total = AbilityLevel(CA_ARCANE_TRICKERY); 
                if (!total)
                { i++; continue; }
                MyTerm->LOption(Format("%s (%d left)", 
                    Lookup(ClassAbilities,ActiveTraits[i] % 1000),
                    total - GetStatiMag(SPEC_TIMEOUT,ABIL_VAL + CA_ARCANE_TRICKERY)),i);
            } else if (ActiveTraits[i] == CA_SACRED_MOUNT + ABIL_VAL) {
                if (AbilityLevel(CA_SACRED_MOUNT) < 5)
                { i++; continue; }
                MyTerm->LOption("Summon Sacred Mount",i);
            } else if (ActiveTraits[i] == CA_FLAWLESS_DODGE + ABIL_VAL) {
                int16 total = AbilityLevel(CA_FLAWLESS_DODGE) + Mod(A_DEX); 
                if (total == Mod(A_DEX))
                { i++; continue; }
                MyTerm->LOption(Format("%s (%d left)", 
                    Lookup(ClassAbilities,ActiveTraits[i] % 1000),
                    total - GetStatiMag(FLAWLESS_DODGE)),i);
            } else if (ActiveTraits[i] == CA_INNATE_SPELL + ABIL_VAL) {
                for(j=0;j!=MAX_SPELLS;j++)
                    if (Spells[j] & SP_INNATE) {
                        str = "Innate <6>";
                        str += NAME(theGame->SpellID(j));
                        str += "<7>";
                        MyTerm->LOption(XPrint(str),i+j*1000);
                    }
                if (HasStati(POLYMORPH)) {
                    TMonster * tm = TMON(mID);
                    for(rID sID = tm->FirstRes(AN_INNATE); sID; 
                        sID = tm->NextRes(AN_INNATE)) {
                            j = theGame->SpellNum(sID);
                            str = Format("Innate (%s) <6>",NAME(mID));
                            str += NAME(theGame->SpellID(j));
                            str += "<7>";
                            MyTerm->LOption(XPrint(str),i+j*1000);
                    } 
                } 
            } else if (HasAbility(ActiveTraits[i] % 1000)) {
                MyTerm->LOption(Lookup(ClassAbilities,ActiveTraits[i] % 1000),i);
                found = true;
            }
            break;
        case FEAT_VAL: /* Active Feat */
            if (HasFeat(ActiveTraits[i] % 1000)) {
                MyTerm->LOption(FeatName(ActiveTraits[i] % 1000),i);
                found = true; }
            break;
        case SKILL_VAL: /* Active Skill */
            int16 sk;
            sk = ActiveTraits[i] % 1000;
            if (HasSkill(sk, sk==SK_HEALING || sk == SK_HANDLE_DEV)) 
                if (ActiveTraits[i] % 1000 != SK_BALANCE ||
                    SkillLevel(SK_BALANCE) >= 10) {
                        MyTerm->LOption(SkillInfo[ ActiveTraits[i] % 1000 ].name,i);
                        found = true; }
                break;
        case ATTK_VAL: /* Polymorph Attack */
            if (HasAttk(ActiveTraits[i] % 1000)) {
                if (ActiveTraits[i] % 1000 == A_ROAR) {
                    String verb = TMON(mID)->GetMessages(MSG_ROARVERB)[0];
                    if (verb.GetLength()) {
                        MyTerm->LOption(Capitalize(verb,true),i);
                        break;
                    }
                } else
                    MyTerm->LOption(Lookup(ATypeNames,ActiveTraits[i] % 1000),i);
                found = true;
            }
            break;
        default:
            Fatal("Something strange in ActiveTraits[] array!");
        }
        i++;
    }

    if (!found) {
        IPrint("You have no active abilities, feats or skills.");
        return;
    }

    MyTerm->SetQKeyType(QuickKeys, QKY_SKILL);
    i = MyTerm->LMenu(MENU_SORTED|MENU_ESC|MENU_BORDER|MENU_QKEY,
        "Use which skill, feat or ability?",WIN_MENUBOX);
    if (i == -1)
        return;

HasSuppliedIndex:

    switch((ActiveTraits[i%1000] / 1000)*1000) {
    case ABIL_VAL:  UseAbility((uint8)(ActiveTraits[i%1000] % 1000),(int16)(i/1000)); return;
    case FEAT_VAL:  UseFeat(ActiveTraits[i%1000] % 1000); return;
    case SKILL_VAL: UseSkill(ActiveTraits[i%1000] % 1000); return;
    case ATTK_VAL:
        e.Clear();
        e.EActor = this;
        e.EParam   = ActiveTraits[i] % 1000;
        switch (ActiveTraits[i] % 1000) {
        case A_BREA:
        case A_BRE2:
            if (!MyTerm->EffectPrompt(e,Q_DIR|Q_TAR|Q_LOC))
                return;
            ReThrow(EV_SATTACK,e);
            break;
        case A_ROAR:
            ReThrow(EV_SATTACK,e);
            break;
        default:
            Error("Unimplemented PC polymorph attack!");
        }
    }
}

void Player::CancelMenu() {
  /* The cancel menu lists:
   * Active Spells
   * Wild Shape
   * Tracking
   * Tumbling
   * Charging
   * Chanting (Chant/Prayer)
   * Singing
   * etc.

Later:
   * Cancel Illusions (that require continued concentration)
   * Dismiss Summoned Creatures (to get PHD back)
   * Turn off Psionic Powers
   * Cancel Fly/Gaseous Form/Tenser's Trans. other "double-edged" spells
   * Cancel (some) Walls (Wall of Force advantage over Stone)

   */
    rID spells[1024] = { 0, };
    Field *fields[100] = { 0, };
    int num_spell = 1, num_field = 1;
    int i, j, c = 0;

    StatiIter(this)
        if (S->eID && RES(S->eID)->Type == T_TEFFECT) {
            if (S->h && oThing(S->h)->isItem())
                if (oItem(S->h)->Owner() != this)
                    continue;    

            if (!TEFF(S->eID)->HasFlag(EF_CANCEL)) continue; 

            bool found = false; 
            for (i=0; i<num_spell; i++)
                if (spells[i] == S->eID)
                    found = true;
            if (found) continue; 

            spells[num_spell] = S->eID;

            MyTerm->LOption(XPrint("Drop <9><Str><7>",
                NAME(S->eID)),num_spell); 

            num_spell++; c++;
        } else
            switch (S->Nature) {
            case TUMBLING: c++;
                MyTerm->LOption("Stop Tumbling",0-S->Nature);
                break;
            case MOUNTED: c++;
                MyTerm->LOption(Format("Dismount %s",
                    (const char*)(oThing(S->h))->Name(NA_THE)),0-S->Nature);
                break;
            case CHARGING: c++;
                MyTerm->LOption("Break off your charge",0-S->Nature);
                break;
            case AUTO_CHARGE: c++;
                MyTerm->LOption("Stop automatically charging",0-S->Nature);
                break;
            case DISGUISED: c++;
                MyTerm->LOption("Remove your Disguise",0-S->Nature);
                break;        
            case SINGING: c++;
                MyTerm->LOption("Stop Singing",0-S->Nature);
                break;
            case RAGING: c++;
                MyTerm->LOption("Stop Raging",0-S->Nature);
                break;
            case FLAWLESS_DODGE: c++;
                MyTerm->LOption("Turn Off Flawless Dodge",0-S->Nature);
                break;
            case POLYMORPH: 
                if (S->h == myHandle) { 
                    c++;
                    MyTerm->LOption(XPrint("Return to <Res> form",RaceID),0-S->Nature);
                }
                break;
            case HIDING: c++;
                MyTerm->LOption("Stop Hiding",0-S->Nature);
                break;
            case FLURRYING: c++;
                MyTerm->LOption("Break off Flurry of Blows", 0-S->Nature);
                break;
            case DEFENSIVE: c++;
                MyTerm->LOption("Stop Fighting Defensively",0-S->Nature);
                break;
            case EYES_AVERTED: c++;
                MyTerm->LOption("Stop Averting Your Eyes",0-S->Nature);
                break;
            case BLINDNESS:
                if (S->Val == BLIND_EYES_CLOSED) { c++;
                MyTerm->LOption("Open Your Eyes",0-S->Nature);
                } 
                break; 
            case SPRINTING: c++;
                MyTerm->LOption("Stop Sprinting",0-S->Nature);
                break;
            }
    StatiIterEnd(this)

    if (isPlayer()) {
        if (thisp->DigMode) {
            MyTerm->LOption("Stop Mining", 2000 + SK_MINING);
            c++;
        }

        for(j=0;m->Fields[j];j++)
            if (m->Fields[j]->Creator == myHandle) {
                TEffect *eff = TEFF(m->Fields[j]->eID);
                if (eff->HasFlag(EF_CANCEL)) {
                    bool found = false; 
                    for (i=0; i<num_spell; i++)
                        if (spells[i] == m->Fields[j]->eID)
                            found = true;
                    if (found) continue;

                    fields[num_field] = m->Fields[j];
                    MyTerm->LOption(XPrint("Drop <9><Str><7>",NAME(m->Fields[j]->eID)),num_field + 1024); 

                    num_field++; c++;
                }
            }
    }

    if (!c) {
        IPrint("You have nothing active to cancel.");
        return;
    }

    c = MyTerm->LMenu(MENU_ESC|MENU_BORDER,"Cancel Standing Ability:");

    if (isPlayer() && c == 2000 + SK_MINING)
    {
        thisp->DigMode = false;
        IPrint("You stop mining.");
        return;
    }

    if (c == -1)
        return;

    if (c > 0) {
        /* it's an eID */
        if (c < 1024) { /* Spell */
            IPrint("Dropping <Res>.",spells[c]);
            RemoveEffStati(spells[c],EV_ELAPSED);
        } else { /* Field */
            c -= 1024;
            IPrint("Dropping <Res>.",fields[c]->eID);
            m->RemoveField(fields[c]);
        }
    } else {
        c = -c; /* it's a stati nature */
        if (c == MOUNTED) 
            ThrowVal(EV_DISMOUNT,DSM_CHOICE,this,GetStatiObj(MOUNTED));
        if (c == FLAWLESS_DODGE)
        {
            /* Don't kill the whole Stati; it has the number of dodges
            stored in the Mag field. Instead, just set the flag to
            zero to indicate inactive. */
            SetStatiVal(FLAWLESS_DODGE,NULL,0);
            return;
        }
        RemoveStati(c);
    } 
}
#if 0

void Player::CancelMenu()
  {
    int16 i,c; rID last_eID;
    /* The cancel menu lists:
       * Active Spells
       * Wild Shape
       * Tracking
       * Tumbling
       * Charging
       * Chanting (Chant/Prayer)
       * Singing
       * etc.

      Later:
        * Cancel Illusions (that require continued concentration)
        * Dismiss Summoned Creatures (to get PHD back)
        * Turn off Psionic Powers
        * Cancel Fly/Gaseous Form/Tenser's Trans. other "double-edged" spells
        * Cancel (some) Walls (Wall of Force advantage over Stone)

    */
    c = 0; last_eID = 0;
    for (i=0;Stati[i];i++) {
      if (Stati[i]->Nature == TARGET)
        continue;
      if (Stati[i]->eID && RES(Stati[i]->eID)->Type == T_TEFFECT)
        {
          if (Stati[i]->eID == last_eID)
            continue;
          if (Stati[i]->h)
            if (oThing(Stati[i]->h)->isItem())
              continue;
          if (TEFF(Stati[i]->eID)->HasFlag(EF_CANCEL))
            { c++; last_eID = Stati[i]->eID;
              MyTerm->LOption(XPrint("Drop <9><Str><7>",
                NAME(Stati[i]->eID)),i); }
        }
      else switch (Stati[i]->Nature)
        {
          case TRACKING:
            c++;
            MyTerm->LOption(XPrint("Stop tracking <Str>",
              oThing(Stati[i]->h)->Name(NA_THE)),i);
           break;
          case TUMBLING:
            c++;
            MyTerm->LOption("Stop Tumbling",i);
           break;
          case MOUNTED:
            c++;
            MyTerm->LOption(Format("Dismount %s",
                  (oThing(Stati[i]->h))->Name(NA_THE)),i);
           break;
          case CHARGING:
            c++;
            MyTerm->LOption("Break off your charge",i);
           break;
          case AUTO_CHARGE:
            c++;
            MyTerm->LOption("Stop automatically charging",i);
           break;
          case SINGING:
            c++;
            MyTerm->LOption("Stop Singing",i);
           break;
          case RAGING:
            c++;
            MyTerm->LOption("Stop Raging",i);
           break;
          case POLYMORPH:
            c++;
            MyTerm->LOption(XPrint("Return to <Res> form",RaceID),i);
           break;
          case HIDING:
            c++;
            MyTerm->LOption("Stop Hiding",i);
           break;
          case DEFENSIVE:
            c++;
            MyTerm->LOption("Stop Fighting Defensively",i);
           break;
          case ANC_MEM:
            c++;
            MyTerm->LOption("Cease the Ancestral Trance", i);
           break;
        }
      }

    if (!c)
      {
        IPrint("You have nothing active to cancel.");
        return;
      }

    c = MyTerm->LMenu(MENU_ESC,"Cancel Standing Ability:");

    if (c == -1)
      return;

    if (Stati[c]->Nature == MOUNTED)
      ThrowVal(EV_DISMOUNT,DSM_CHOICE,this,GetStatiObj(MOUNTED));
    else if (Stati[c]->eID)
      RemoveEffStati(Stati[c]->eID,EV_ELAPSED);
    else
      RemoveStati(Stati[c]->Nature,
                  Stati[c]->Source,
                  Stati[c]->Val,
                  Stati[c]->Mag,
                  oThing(Stati[c]->h),
                  true);
      
  }
#endif

/* returns 'true' if any time was taken and the character actually 
 * tried to disarm the trap */
bool Character::DisarmTrap(Trap *tr, bool active)
  {
    Creature *msa;
    char ch; int16 CheckDC;
    ch = 'd';
    msa = MostSkilledAlly(SK_HANDLE_DEV);
    if (!(tr->TrapFlags & TS_FOUND))
      return false;
    else if (!isBeside(tr) && !HasStati(TELEKINETIC)) {
      if (active)
        IPrint("You must be adjacent to a trap to disarm it.");
      return false; 
      } 
    else if (HasMFlag(M_NOHANDS)) {
      if (active)
        IPrint("You need hands to be able to work with traps.");
      return false; 
      } 
    else if (isBlind()) {
      if (active)
        IPrint("You must be able to see to be able to work with traps.");
      return false; 
      } 
    else if (tr->TrapFlags & TS_NODISARM) {
      if (active)
        IPrint("You already tried to disarm that trap.");
      return false; 
      } 
    else if (tr->TrapFlags & TS_DISARMED) {
      if (!active)
        return false;
      if (SkillLevel(SK_HANDLE_DEV) >= 15) {
        ch = ChoicePrompt("Reset trap or salvage components?", "rs");
        if (ch == -1)
          return false;
        }
      else if (SkillLevel(SK_HANDLE_DEV) >= 10)
        ch = 'r';
      else {
        IPrint("That trap is already disarmed."); 
        return false;
        } 
      } 

    CheckDC = 15;
    if (ch == 's')
      CheckDC += 10;
    else if (ch == 'r')
      CheckDC += 5;   
    CheckDC += tr->TrapLevel(); 
    
    if (!yn(Format("%s -- Handle Device %+d, DC %d. Proceed?", ch == 'd' ?
      "Disarm" : ch == 'r' ? "Reset" : "Salvage", msa->SkillLevel(SK_HANDLE_DEV), CheckDC)))
      return true;
    
    Timeout += 50; 

    switch (ch)
      {
        case 'd':
          if (tr->HasStati(RESET_BY,0,this))
            {
              IDPrint("You disarm your own <Obj2>.",
                  "The <Obj> disarms the <Obj2>.",this,tr);
              tr->TrapFlags |= TS_DISARMED;
              tr->SetImage();
            }
          else if (SkillCheck(SK_HANDLE_DEV,CheckDC,true,
                tr->GetStatiMag(RETRY_BONUS,SK_HANDLE_DEV,this)) &&
                ThrowEffXY(EV_DISARM,tr->tID,tr->x,tr->y,this,tr,NULL,NULL) != ABORT) 
            {
              IDPrint("You disarm the <Obj2>.",
                  "The <Obj> disarms the <Obj2>.",this,tr);
              tr->TrapFlags |= TS_DISARMED;
              tr->RemoveStati(RESET_BY);
              tr->SetImage();
              if (!tr->HasStati(TRIED, TS_DISARMED, this)) 
                {
                  // TrapLevel ~= DC - 15, give 100-200 XP
                  GainXP(90 + (CheckDC - 14) * 10);
                  // no farming in incursion: you cannot get the XP for disarming this
                  // trap more than once!
                  tr->GainPermStati(TRIED,this,SS_ATTK,TS_DISARMED);
                }
            }
          else 
            { 
              tr->BoostRetry(SK_HANDLE_DEV,this);
              tr->TrapFlags |= TS_NODISARM;  
              if (!HasFeat(FT_LARCENOUS) && !SavingThrow(REF,15,SA_TRAPS)) {
                IDPrint("You set off the <Obj2>!",
                    "The <Obj> sets off the <Obj2>!",this,tr);
                EventInfo e; 
                e.Clear();
                e.EActor = this;
                e.EItem = (Item*)tr;
                tr->TriggerTrap(e,true);
                } 
              else 
                IDPrint("You fail to disarm the <Obj2>!",
                    "The <Obj> fails to disarm the <Obj2>!",this,tr);
            }
          return true; 
        case 'r':
          if (tr->TrapFlags & TS_NORESET)
            {
              IPrint("You already tried to reset that trap.");
              return false;
            }
          if (!TEFF(tr->tID)->HasFlag(EF_MUNDANE)) {
            if (cMana() < tr->TrapLevel()*3)
              { IPrint("You lack the mana to reset that trap.");
                return false; }
            if (yn(Format("Rearming the magical trap will cost %d mana. Continue?",
                   tr->TrapLevel()*3)))
              { LoseMana(tr->TrapLevel()*3,true); }
            else
              return false;
            }
          if (!SkillCheck(SK_HANDLE_DEV,CheckDC,true))
            {
              IDPrint("You fail to reset the <Obj>.",
                "The <Obj2> tries to reset the <Obj>, but fails.",
                tr, this);
              tr->TrapFlags |= TS_NORESET;
              return true;
            }
          IDPrint("You reset the <Obj>.",
            "The <Obj2> resets the <Obj>.",
            tr, this);
          tr->TrapFlags &= ~TS_DISARMED;
          tr->GainPermStati(RESET_BY,this,SS_MISC);
          tr->SetImage();
          return true;
        case 's':
          IPrint("You begin taking apart the <Obj>...", tr);
          if (isPlayer())
            if (thisp->SpendHours(1,1) != DONE)
              return true;
          if (!SkillCheck(SK_HANDLE_DEV,CheckDC,true))
            {
              IPrint("You fail to preserve the components of the <Obj>.",tr);
            }          
          else
            {
              Item *comp;
              comp = Item::Create(FIND("trap components"));
              if (!comp) {
                Error("Cannot create trap components!");
                return true;
                }
              comp->GainPermStati(COMPONENTS_OF,NULL,SS_MISC,0,1,tr->tID);
              if (tr->HasStati(XP_GAINED))
                comp->GainPermStati(XP_GAINED,tr->GetStatiObj(XP_GAINED),SS_MISC);
              IPrint("You carefully remove and package the elements of the <Obj>.",tr);
              GainItem(comp,false);
            }
          tr->Remove(true);
          return true;
      }
    return false;          
  } 

int8 Creature::SkillKitMod(int16 sk)
  {
    Item *it; int16 mod, best, sec, i, cKit, rope_mod;
    rID kits[65];
    
    /* These frequently rolled skills have no skill kit
       that modifies them, so let's avoid gratuitous
       inventory scanning. */
    if (sk == SK_MOVE_SIL || sk == SK_HIDE || sk == SK_SPELLCRAFT ||
        sk == SK_SPOT || sk == SK_LISTEN || sk == SK_INTIMIDATE)
      return 0;
    
    mod = best = sec = cKit = rope_mod = 0;

    if (sk == SK_KNOW_THEO)
      {
        int8 sl;
        if (getGod())
          for (sl=0;sl!=SL_LAST;sl++)
            if (InSlot(sl) && InSlot(sl)->isType(T_SYMBOL))
              if (InSlot(sl)->activeSlot(sl))
                if (InSlot(sl)->eID == TGOD(getGod())->GetConst(HOLY_SYMBOL))
                  best = 4;
      }

    for (it=FirstInv();it;it=NextInv()) {
      if (TEFF(it->iID)->ListHasItem(SKILL_KIT_FOR,sk))
        {
          mod = (int16)TEFF(it->iID)->GetConst(SKILL_KIT_MOD) + it->GetPlus();
          if (mod > best)
            best = mod;
        }
      if (TEFF(it->iID)->ListHasItem(SECONDARY_KIT_FOR,sk))
        {
          for (i=0;i!=cKit;i++)
            if (kits[i] == it->iID)
              continue;
          sec += (int16)TEFF(it->iID)->GetConst(SKILL_KIT_MOD) + it->GetPlus();
          kits[cKit++] = it->iID;
        }
      if (sk == SK_CLIMB && it->HasIFlag(IT_ROPE))
        rope_mod = max(rope_mod,(int16)TITEM(it->iID)->
          GetConst(SKILL_KIT_MOD));
      }

    if (HasStati(INNATE_KIT,sk))
      best = max(best,GetStatiMag(INNATE_KIT,sk));
      
    return best + sec + rope_mod;
  }

bool Creature::HasSkillKit(int16 sk)
  {
    Item *it;
    
    /* These frequently rolled skills have no skill kit
       that modifies them, so let's avoid gratuitous
       inventory scanning. */
    if (sk == SK_MOVE_SIL || sk == SK_HIDE || sk == SK_SPELLCRAFT ||
        sk == SK_SPOT || sk == SK_LISTEN || sk == SK_INTIMIDATE)
      return 0;
    
    if (HasStati(INNATE_KIT,sk))
      return true;
    
    for (it=FirstInv();it;it=NextInv())
      if (TEFF(it->iID)->ListHasItem(SKILL_KIT_FOR,sk))
        return true;
    return false;
  }

void Character::UseSkill(uint8 sk)
  {
    EventInfo e; Trap *tr; int16 i, PartyID; Thing *t; String s;
    int8 Check = (int8)Dice::Roll(1,20,(int8)SkillLevel(sk)), DC;
    /* Skill Check: 7 + 1d20 (15) = 22 vs. DC 25 (failure) */
    int16 NeedKitFor[] = { SK_HEAL, SK_DISGUISE, SK_ALCHEMY,
                           SK_MINING, SK_CRAFT, SK_LOCKPICKING, 0 };
    
    for (i=0;NeedKitFor[i];i++)
      if (NeedKitFor[i] == sk)
        if (!HasSkillKit(sk))
          {
            IPrint("You need a skill kit to use that skill.");
            return;
          }
        
    switch(sk)
      {
        case SK_BALANCE:
          if (!isPlayer())
            return;
          e.Clear();
          e.EActor = this;
          e.eID = FIND("create tightrope bridge");
          thisp->MyTerm->EffectPrompt(e,Q_LOC,false, 
            "Create Tightrope Bridge");
          ReThrow(EV_EFFECT,e);
         return;
        // ww: let's implement Julian's active listen
        case SK_LISTEN: 
          {
            Creature *posse[500], *in_sight = NULL;
            int16 Nearest[50], j, spec, ll;
            int8 DirToGroup[50]; 
            Creature* Heard[50], *cr;
            TextVal Sounds[] = {
              { MA_SPIDER, NULL },
              { MA_RODENT, "the skittering of tiny claws" },
              { MA_BAT, "the fluttering of tiny wings" },
              { MA_CAT, "the growling of a feline" },
              { MA_DOG, "the howling of a canid" },
              { MA_BURROW, "the sound of burrowing" },
              { MA_HOMINID, "the hooting calls of primates" },
              { MA_VERMIN, "clicking and chittering" },
              { MA_QUADRUPED, "the clacking of hooves on stone" },
              { MA_WRAITH, "a horrific moaning" },
              { MA_ZOMBIE, "a slow shambling" },
              { MA_SKELETON, "the clacking of bones" },
              { MA_HUMAN, "traces of hushed conversation" },
              { MA_ELF, "the lilting tones of the elvish language" },
              { MA_KOBOLD, "high-pitched laughter" },
              { MA_DWARF, "sonorous, dwarven chanting" },
              { MA_TROLL, "the vicious snarling of trolls" },
              { MA_GOBLINOID, "the guttural tones of a goblinoid tongue" },
              { MA_DEVIL, "diabolical chanting" },
              { MA_DEMON, "loathsome demonic snarling" },
              { MA_OOZE, "faint slurping sounds" },
              { MA_ABERRATION, "something slithering" },
              { MA_PLANT, "the rustling of vines" },
              { MA_CELESTIAL, "faint, glorious music" },
              { MA_CONSTRUCT, "heavy grinding noises" },
              { MA_BEAST, "monsterous growling" },
              { MA_YUAN_TI, "sibilant voices" },
              { MA_NAGA, "sibilant voices" },
              { MA_SNAKE, "hissing" },
              { MA_DRAGON, "a loud, slow heartbeat" },
              { MA_GIANT, "very heavy footfalls" },
              { MA_VORTEX, "a howling like that of a cyclone" },
              { MA_FAERIE, "high-pitched laughter" },
              { MA_FIRE+(MA_ELEMENTAL*256), "the crackling of a bonfire" },
              { MA_AIR+(MA_ELEMENTAL*256), "the rustling of wind" },
              { MA_EARTH+(MA_ELEMENTAL*256), "the grinding of stone against stone" },
              { MA_WATER+(MA_ELEMENTAL*256), "water sloshing around" },
              { MA_HUMANOID, "indistinct footsteps" },
              { MA_AQUATIC, "water flowing" },
              { MA_AVIAN, "the fluttering of wings" },
              { 0, NULL } };
            TextVal Distance[] = {
              { 40, "exceedingly faintly" },
              { 30, "extremely far away" },
              { 25, "very far away" },
              { 20, "quite far away" },
              { 15, "at a distance" },
              { 12, "at a moderate distance" },
              { 8, "close by" },
              { 5, "very close by" },
              { 0, "extremely close by" },
              { 0, NULL } };
            TextVal Direct[] = {
              { NORTH, "north" },
              { SOUTH, "south" },
              { EAST, "east" },
              { WEST, "west" },
              { NORTHEAST, "north-east" },
              { NORTHWEST, "north-west" },
              { SOUTHEAST, "south-east" },
              { SOUTHWEST, "south-west" },
              { 0, NULL } };
            spec = 0;
            int16 pc = 0, Chance; 
            memset(Nearest,0,sizeof(int16)*50);
            memset(Heard,0,sizeof(Creature*)*50);
            memset(DirToGroup,0,sizeof(int8)*50);
            bool groups[114] = { 0, };
            RestEncounterChance(3, PartyID, in_sight, Chance);
            pc = 0;
            ll = 0;
            
            if (isThreatened())
              {
                IPrint("You can't listen in combat.");
                break;
              }
              
            Timeout += 40;
            
            MapIterate(m,cr,i)
              if (cr->isCreature() && pc < 498)
                posse[pc++] = cr;
            for (i=0;i<pc;i++) {
              /* For clarity, exclude what you see clearly */
              if (Perceives(posse[i]) & ~PER_SHADOW)
                continue;
              /* First, do you hear it? */
              int16 DC = posse[i]->SkillLevel(SK_MOVE_SIL) +
                dist(x,y,posse[i]->x,posse[i]->y);
                
              /* Too easy to scum this now...
               * SkillCheck(SK_LISTEN,DC,false,0);
               */
              
              int16 sc = SkillLevel(SK_LISTEN)*2 - DC;
              if (sc > 15) { // you hear it perfectly! 
                for (j=0;Heard[j];j++)
                  if (Heard[j]->mID == posse[i]->mID)
                    if (Heard[j]->GetStatiEID(TEMPLATE) ==
                        posse[i]->GetStatiEID(TEMPLATE))
                      goto SkipDuplicate;
                Heard[spec++] = posse[i];
                SkipDuplicate:;
                } 
              else if (sc > 0) { // hear general traits
                for (j=0;Sounds[j].Val;j++)
                  if (posse[i]->isMType(Sounds[j].Val))
                    {
                      if (!Sounds[j].Text)
                        break;
                      if (sc > 5 && (!Nearest[j] || Nearest[j] >
                           dist(x,y,posse[i]->x,posse[i]->y)))
                        DirToGroup[j] = DirTo(
                          posse[i]->x,posse[i]->y) + 10;
                      if (Nearest[j])
                        Nearest[j] = min(Nearest[j],
                          dist(x,y,posse[i]->x,posse[i]->y));
                      else
                        Nearest[j] = 
                          dist(x,y,posse[i]->x,posse[i]->y);
                      
                        
                    }

                }  
            } 
            s = "You hear"; 
            bool first = true; 
            for (i=0;Heard[i];i++)
              {
                Creature *m; 
                if (!first) {
                  s += ",";
                  ll = (int16)s.GetLength();
                  }
                s += " ";
                
                m = new Monster(Heard[i]->tmID);
                StatiIterNature(m,TEMPLATE)
                  if (S->Mag && TTEM(S->eID)->TType & 
                        (TM_UNDEAD|TM_AGECAT|TM_PLANAR))
                    {
                      m->AddTemplate(S->eID);
                      m->IdentifyTemp(S->eID);
                    }
                StatiIterEnd(m)
                s += m->Name(NA_A);
                m->Remove(true);
                
                s += " ";
                for (j=0;Distance[j].Val > dist(
                     x,y,Heard[i]->x,Heard[i]->y);j++)
                  ;
                s += Distance[j].Text;
                s += " to the ";
                s += Lookup(Direct,DirTo(
                      Heard[i]->x,Heard[i]->y));
                first = false;
              }
            for (i=0;Sounds[i].Val;i++)
              {
                if (!Nearest[i])
                  continue;
                if (!Sounds[i].Text)
                  continue;
                if (!first) {
                  s += ",";
                  ll = (int16)s.GetLength();
                }
                s += " ";
                s += Sounds[i].Text;
                s += " ";
                for (j=0;Distance[j].Val > Nearest[i];j++)
                  ;
                s += Distance[j].Text;
                if (DirToGroup[j]) {
                  s += " to the ";
                  s += Lookup(Direct,DirToGroup[j]-10);
                  }
                first = false;
              }
            
            if (first) 
              s += " nothing"; 
            
            if (ll)
              s = s.Left(ll - 1) + SC(" and") + s.Right(s.GetLength() - ll);
            s += "."; 
            IPrint(s); 
            if (HasStati(TRIED,SK_LISTEN))
              goto FailedRestListen;
            if (SkillCheck(SK_LISTEN,15,true,0)) {
              if (Chance <= 0)
                IPrint("You think it is safe to rest.");
              else if (Chance < 100) 
                IPrint("If you rest, there may be an encounter.");
              else 
                IPrint("It is not safe to rest."); 
              }
            else {
              GainTempStati(TRIED,NULL,-1,SS_MISC,SK_LISTEN,3);
              FailedRestListen:
              IPrint("You can't tell whether it's safe to rest.");
              } 
          } 
          break; 

        case SK_HIDE: 
          ThrowVal(EV_HIDE,HI_SHADOWS,this);
         return;
        case SK_KNOW_MAGIC:
          Throw(EV_RESEARCH,this);
         return;
        case SK_TUMBLE:
          if (!LoseFatigue(1,true))
            break;
          Exercise(A_DEX,random(4)+1,EDEX_TUMBLE,20);
          GainTempStati(TUMBLING,NULL,Dice::Roll(1,4)+SkillLevel(SK_TUMBLE),SS_MISC);
          IDPrint("You begin a series of intricate leaps and tumbles.",
                  "The <Obj> jumps and weaves with a dancing gait.",this);
         break;
        case SK_JUMP:
          e.Clear();
          e.EActor = this;
          if (!thisp->MyTerm->EffectPrompt(e,Q_LOC))
            break;
          ReThrow(EV_JUMP,e);
         break;
        case SK_HANDLE_DEV:
          Timeout += 50;
          
          MapIterate(m,t,i)
            if (t->isType(T_TRAP) && (tr = (Trap*)t))
              if (isBeside(tr) && Perceives(tr))
                if (DisarmTrap(tr,true)) 
                  return; 
          if (HasStati(TELEKINETIC))
            {
              e.Clear();
              e.vRange = (int8)GetStatiMag(TELEKINETIC);
              if (thisp->MyTerm->EffectPrompt(e,Q_TAR,false,"Disarm a trap:"))
                if (e.ETarget->isType(T_TRAP))
                  if (DisarmTrap(tr,true))
                    return;
              return;
              
            }
          
          IPrint("You see nothing suitable to use that skill on.");
          return;
        case SK_ANIMAL_EMP:
          e.Clear();
          e.EActor = this;
          e.EMap = e.EActor->m;
          Check = (int8)Dice::Roll(1,20);
          if (isPlayer()) {
            if (!thisp->MyTerm->EffectPrompt(e,Q_TAR|Q_CRE))
              return /*ABORT*/;
            }
          else
            return /*ABORT*/;

          e.EParam = SkillLevel(SK_ANIMAL_EMP);
          StatiIterNature(this,SPECIES_AFFINITY)
              if (e.EVictim->isMType(S->Val))
                { e.EParam = max(e.EParam,S->Mag);
                  StatiIterBreakout(this,goto ValidTarget) }
          StatiIterEnd(this)
          
          if (e.EVictim->isMType(MA_ANIMAL) && SkillLevel(SK_ANIMAL_EMP))
            goto ValidTarget;
          if (e.EVictim->isMType(MA_BEAST) && HasFeat(FT_BEASTIAL_EMPATHY) && SkillRanks[SK_ANIMAL_EMP])
            goto ValidTarget;
          if ((e.EVictim->isMType(MA_PLANT) || e.EVictim->isMType(MA_FUNGI)) &&
                    HasFeat(FT_PLANT_EMPATHY) && SkillRanks[SK_ANIMAL_EMP])
            goto ValidTarget;
          IPrint("You have no primal link to that sort of creature.");
          return /*ABORT*/;
          ValidTarget:
          e.EActor->Timeout += 50;
          if (e.EVictim->HasStati(ANIMAL_EMP_TRIED,-1,this))
            {
              IPrint("You already tried your animal empathy on that creature.");
              return /*ABORT*/;
            }
          if (e.EVictim->isCharacter() || /* e.EVictim->isMType(MA_SAPIENT) ||  */
                                              !e.EVictim->isMType(MA_LIVING))
            {
              IPrint("Strangely, you can't get a read on that creature.");
              return /*DONE*/;
            }
          Retry:
          switch (ChoicePrompt("Pacify it, Cow it, Soothe it or Befriend it?","pcsb?",'p',YELLOW,BROWN))
            {
              case -1:
                return;
              case 'p':
                if (!e.EVictim->isHostileTo(e.EActor))
                  {
                    IPrint("But it isn't hostile.");
                    return /*DONE*/;
                  }
                e.EVictim->RemoveStati(SLEEPING,-1,SLEEP_NATURAL);
                TPrint(e,"You look deep into the <EVictim>'s eyes.", NULL,
                         "The <EActor> looks deep into the <EVictim>'s eyes.");
                if ((DC = e.EVictim->ChallengeRating() + 15) > e.EParam+Check) {
                  TPrint(e,"The <EVictim> snarls and breaks free of your gaze.", NULL,
                         "The <EVictim> snarls and breaks free of the <EActor>'s gaze.");
                  e.ETarget->GainTempStati(ANIMAL_EMP_TRIED,this,500,SS_MISC,0,0,0);
                  }
                else {
                  e.EVictim->StateFlags |= MS_PEACEFUL;
                  e.EVictim->ts.Pacify(e.EVictim,this);
                  VPrint(e,NULL,"The <EVictim> settles back and seems much calmer now.");
                  }
               break;   
              case 'b':
                if (e.EVictim->isHostileTo(e.EActor))
                  {
                    IPrint("You can't befriend a hostile creature.");
                    return /*DONE*/;
                  }
                if (!e.EVictim->isBeside(e.EActor))
                  {
                    IPrint("You need to get closer to do that.");
                    return /*ABORT*/;
                  }
                e.EVictim->RemoveStati(SLEEPING,-1,SLEEP_NATURAL);
                VPrint(e,"You reach out to gently touch the <EVictim>.", NULL,
                         "The <EActor> reaches out to gently touch the <EVictim>.");
                if ((DC = e.EVictim->ChallengeRating()*2 + 15) > e.EParam+Check) {
                  VPrint(e, NULL, "The <EVictim> backs away awkwardly.");
                  e.ETarget->GainTempStati(ANIMAL_EMP_TRIED,this,-2,SS_MISC,0,0,0);
                  }
                else {
                  if (isPlayer())
                    {
                      if (!e.EVictim->isMonster())
                        return;
                      if (!((Monster*)e.EVictim)->MakeCompanion(thisp,PHD_ANIMAL))
                        break;
                    }
                  else
                    e.EVictim->PartyID = e.EActor->PartyID;
                  TPrint(e,"The <EVictim> moves up to you and nuzzles your hand. "
                           "<He:EVictim> seems friendly now.",NULL,
                           "The <EVictim> moves up to you and nuzzles the <EActor>'s hand.");
                  }
               break;   
              case 'c':
                if (!e.EVictim->isHostileTo(e.EActor))
                  {
                    IPrint("That creature isn't hostile.");
                    return /*DONE*/;
                  }
                e.EVictim->RemoveStati(SLEEPING,-1,SLEEP_NATURAL);
                TPrint(e,"You stare down the <EVictim>.", NULL,
                         "The <EActor> stares down the <EVictim>.");
                if ((DC = e.EVictim->ChallengeRating() + 10) > e.EParam+Check) {
                  TPrint(e,"The <EVictim> snarls and breaks free of your gaze.", NULL,
                         "The <EVictim> snarls and breaks free of the <EActor>'s gaze.");
                  e.ETarget->GainTempStati(ANIMAL_EMP_TRIED,this,500,SS_MISC,0,0,0);
                  }
                else {
                  e.EVictim->GainTempStati(AFRAID,NULL,(int8)(5+e.EParam*2),SS_MISC,FEAR_PANIC);
                  VPrint(e,NULL,"The <EVictim> turns to flee.");
                  }
               break;   
              case 's':
                if (!e.EVictim->HasStati(AFRAID))
                  {
                    IPrint("That creature isn't panicked.");
                    return /*DONE*/;
                  }
                e.EVictim->RemoveStati(SLEEPING,-1,SLEEP_NATURAL);
                TPrint(e,"You whisper softly to the <EVictim>.", NULL,
                         "The <EActor> whispers softly to the <EVictim>.");
                if ((DC = e.EVictim->ChallengeRating() + 10) > e.EParam+Check) {
                  TPrint(e,"You fail to calm down the <EVictim>.", NULL,
                         "The <EActor> fails to calm down the <EVictim>.");
                  e.ETarget->GainTempStati(ANIMAL_EMP_TRIED,this,500,SS_MISC,0,0,0);
                  }
                else {
                  e.EVictim->RemoveStati(AFRAID);
                  VPrint(e,NULL,"The <EVictim> seems steadier now.");
                  }
               break;
              case '?':
                if (isPlayer())
                  thisp->MyTerm->HelpTopic("help::skills","AN");
                goto Retry;
              default: 
                return;
            }
          if (isPlayer()) {
            thisp->MyTerm->SetWin(WIN_NUMBERS);
            thisp->MyTerm->Clear();
            thisp->MyTerm->Write(Format("%c%s Check:%c 1d20 (%d) %c %d = %d vs DC %d [%s]",
              -13,SkillInfo[SK_ANIMAL_EMP].name, -7,Check,e.EParam >= 0 ? '+' : '-',
              e.EParam, e.EParam+Check, DC, e.EParam+Check >= DC ? "success" : "failure"));
            if (thisp->Opt(OPT_STORE_ROLLS))
              thisp->MyTerm->AddMessage(Format("%c%s Check:%c 1d20 (%d) %c %d = %d vs DC %d [%s]",
                -13,SkillInfo[SK_ANIMAL_EMP].name, -7,Check,e.EParam >= 0 ? '+' : '-',
                e.EParam, e.EParam+Check, DC, e.EParam+Check >= DC ? "success" : "failure"));
            }
          return /*DONE*/;   
        case SK_CRAFT:
          CraftItem(SK_CRAFT + SKILL_VAL);
         break;
        case SK_DISGUISE:
          Throw(EV_DISGUISE,this);
         break;
        case SK_PICK_POCKET:
          {
            EventInfo xe;
            xe.Clear();
            xe.EActor = this;
            if (!thisp->MyTerm->EffectPrompt(xe,Q_NEAR|Q_TAR|Q_CRE,false,"Pick Pocket -- "))
              break;
            ReThrow(EV_PICK_POCKET, xe);
          }
         break;
        case SK_MINING:
          if (thisp->DigMode) {
            thisp->DigMode = false;
            thisp->MyTerm->ShowTraits();
            break;
          }
          if (!EInSlot(SL_WEAPON) || !EInSlot(SL_WEAPON)->HasIFlag(WT_DIGGER))
          {
            IPrint("You need to wield a digging implement first.");
            return;
          }
          thisp->DigMode = true;
          RemoveStati(CHARGING);
          thisp->MyTerm->ShowTraits();
          break;
        case SK_ALCHEMY:
          CraftItem(SK_ALCHEMY + SKILL_VAL); 
         break;  
        case SK_HEAL:
        {
          int16 healDC, i, choice, amt;
          TEffect *te; const char *ab;
          bool found = false, failed = false;
          
          if (!HasSkillKit(SK_HEAL))
            { IPrint("You need a healing kit for that."); }
          if (isThreatened())
            { IPrint("You can't use Healing in combat."); } 
          
          
          
          if (HasStati(BLEEDING)) 
            if (!HasStati(HEAL_TIMEOUT,AD_BLEE))
              { thisp->MyTerm->LOption("Treat Bleeding",AD_BLEE); 
                found = true; }
          if (!(HasStati(HEAL_TIMEOUT,AD_DAST) ||
                HasStati(HEAL_TIMEOUT,AD_DADX) ||
                HasStati(HEAL_TIMEOUT,AD_DACO) ||
                HasStati(HEAL_TIMEOUT,AD_DAIN) ||
                HasStati(HEAL_TIMEOUT,AD_DAWI) ||
                HasStati(HEAL_TIMEOUT,AD_DACH) ||
                HasStati(HEAL_TIMEOUT,AD_DALU) ) )
            {
              if (HasStati(POISONED))
                if (!HasStati(HEAL_TIMEOUT,AD_POIS))
                  { thisp->MyTerm->LOption("Treat Poison",AD_POIS);
                    found = true; }
              if (HasStati(DISEASED))
                if (!HasStati(HEAL_TIMEOUT,AD_DISE))
                  { thisp->MyTerm->LOption("Treat Disease",AD_DISE);  
                    found = true; }
            }
          if (HasStati(BLINDNESS))
            if (!HasStati(HEAL_TIMEOUT,AD_BLND))
              { thisp->MyTerm->LOption("Treat Blindness",AD_BLND);  
                found = true; }
          
          const char* AbilityName[] = { "Strength", "Dexterity",
            "Constitution", "Intelligence", "Wisdom", "Charisma", "Luck" };
          /* Get the player to make a tactical decision -- treat either
             the poison or disease, or the ability damage that results
             from said poison or disease, not both, each day. */
          if (!(HasStati(HEAL_TIMEOUT,AD_POIS) ||
                HasStati(HEAL_TIMEOUT,AD_DISE) ))
            for (i=0;i<=A_LUC;i++)
              if (HasStati(ADJUST_DMG,i))
                if (!HasStati(HEAL_TIMEOUT,AD_DAST+i))
                  { thisp->MyTerm->LOption(Format(
                      "Treat %s Damage",AbilityName[i]),AD_DAST+i);
                    found = true; }                  
          
          if (HasStati(STONING))
            if (!HasStati(HEAL_TIMEOUT, AD_STON)) {
                thisp->MyTerm->LOption("Forestall Petrification",AD_STON);  
                found = true;
            }
          
          if (!found) {
              IPrint("You have no untreated ailments at present.");
              return;
          }
          
          choice = (int16)thisp->MyTerm->LMenu(MENU_ESC|MENU_BORDER,"What do you want to do?", WIN_MENUBOX);
          if (choice == -1)
            return;
          
          int16 spent;
          if (isPlayer())
            if (thisp->SpendHours(1,1,spent,true) != DONE)
              return;
          
          switch(choice)
            {
              case AD_BLEE:
                healDC = 10 + SumStatiMag(BLEEDING)*3 +
                           GetStatiEID(BLEEDING) ? 5 : 0;
                if (SkillCheck(SK_HEAL,healDC,true))
                  { RemoveStati(BLEEDING);
                    IPrint("You stop the bleeding."); }
                else {
                  failed = true;
                  IPrint("You fail to staunch the flow of blood.");
                  }
               break;
              case AD_BLND:
                if (GetStatiDur(BLINDNESS) == -1) /* Permanent */
                  healDC = 30;
                else if (GetStatiDur(BLINDNESS) < -1) /* Days */
                  healDC = 25;
                else
                  healDC = min(10 + GetStatiDur(BLINDNESS)/3,20);
                if (SkillCheck(SK_HEAL,healDC,true))
                  { RemoveStati(BLINDNESS);
                    IPrint("You manage to restore your sight."); }
                else { 
                  failed = true;
                  IPrint("You are unable to treat the blindness effectively.");
                  }
               break;
              case AD_POIS:
              case AD_DISE:
                StatiIterNature(this, ((choice == AD_POIS) ? 
                                    POISONED : DISEASED))
                    te = TEFF(S->eID);
                    healDC = te->Vals(0)->sval;
                    if (!SkillCheck(SK_HEAL,healDC,true))
                      { failed = true;
                        IPrint("You fail to treat the <Res>.",S->eID); }
                    else if (LastSkillCheckResult > (healDC - 5) + 
                                      (te->Vals(0)->lval - S->Mag)*5)
                      { IPrint("You purge the <Res>.",S->eID); 
                        StatiIter_RemoveCurrent(this); }
                    else
                      { IPrint("You lessen the effect of the <Res>.",S->eID);
                        failed = true; S->Mag += 1 + 
                          ((LastSkillCheckResult - healDC)/5); }
                StatiIterEnd(this)
               break; 
              case AD_DAST: case AD_DADX: case AD_DACO:
              case AD_DAIN: case AD_DAWI: case AD_DACH:
              case AD_DALU:
                ab = AbilityName[choice - AD_DAST];
                healDC = 10 - GetStatiMag(ADJUST_DMG,A_STR + (choice - AD_DAST))*2;
                if (!SkillCheck(SK_HEAL,healDC,true))
                  { IPrint("You fail to treat the <str> damage.",ab);
                    failed = true; break; }
                amt = 1 + (LastSkillCheckResult - healDC)/5;
                StatiIterNature(this,ADJUST_DMG)
                  if (S->Val != (A_STR + choice - AD_DAST))
                    continue;
                  if (amt >= (-S->Mag))
                    { 
                      amt -= (-S->Mag); 
                      StatiIter_RemoveCurrent(this);
                    }
                  else
                    {
                      S->Mag -= (-amt);
                      amt = 0;
                    }
                StatiIterEnd(this)
                IPrint("You <str>treat the <str> damage.",
                  HasStati(ADJUST_DMG,(A_STR + choice - AD_DAST)) ?
                  "" : "fully ", ab);
                failed = HasStati(ADJUST_DMG,(A_STR + choice - AD_DAST));
               break;
              case AD_STON:
                healDC = 20;
                if (SkillCheck(SK_HEAL,healDC,true))
                  { IPrint("You forestall your coming petrification.");
                    StatiIterNature(this,STONING)
                      S->Duration += SkillLevel(SK_HEAL)*20; 
                    StatiIterEnd(this)
                  }
                else
                  { IPrint("You fail to slow the petrification effect.");
                    failed = true; }
               break;
            } 
          if (failed)
            GainTempStati(HEAL_TIMEOUT,NULL,-2,SS_MISC,choice);
         break; 
        }
        case SK_POISON_USE:
          {
            char ch; Item *it; rID vialID; bool found;
            hObj hPoison, hWeapon;
            vialID = FIND("small glass vial");
            ch = ChoicePrompt("Create or apply poison?", "ca");
            if (ch == -1)
              return;
            if (ch == 'a')
              {
                found = false;
                for (it=FirstInv();it;it=NextInv())
                  if (it->HasStati(POISONED) && it->iID == vialID)
                    { found = true;
                      thisp->MyTerm->LOption(it->Name(NA_LONG),it->myHandle); }
                if (!found)
                  { IPrint("You have no poisons to apply.");
                    return; }
                hPoison = thisp->MyTerm->LMenu(MENU_BORDER|MENU_ESC,"Choose a Poison:");
                if (hPoison == -1)
                  return;
                found = false;
                for (it=FirstInv();it;it=NextInv())
                  if (it->isType(T_WEAPON) || it->isType(T_MISSILE))
                    { found = true;
                      thisp->MyTerm->LOption(it->Name(0),it->myHandle); }
                if (!found)
                  { IPrint("You have weapons to poison.");
                    return; }
                hWeapon = thisp->MyTerm->LMenu(MENU_BORDER|MENU_ESC,"Choose a Weapon:");
                if (hWeapon == -1)
                  return;
                Throw(EV_APPLY,this,oItem(hWeapon),oItem(hPoison));
                return;
              }
            else
              {
                if (!HasSkill(SK_ALCHEMY) || SkillLevel(SK_POISON_USE) < 12)
                  {
                    IPrint("To brew poisons, you must have both Alchemy and "
                      "Poison Use as class skills, and a Poison Use rating of 12+.");
                    return;
                  }
                if (!HasSkillKit(SK_ALCHEMY))
                  {
                    IPrint("You need an alchemy set to make poison.");
                    return;
                  }
                CraftItem(SK_POISON_USE + SKILL_VAL);
                return;
              }
                
          }
        default:
          IPrint("That skill isn't implemented yet.");
      }
  }

Creature * Creature::MostSkilledAlly(int16 sk)
  {
    Creature *cr, *best; 
    int16 blev, i;
    if (!m)
      return this;
    blev = SkillLevel(sk);
    best = this;
    MapIterate(m,cr,i)
      if (cr->isCreature() && cr != this)
        if (DistFrom(cr) < 6 && cr->HasSkill(sk))
          if (cr->isFriendlyTo(this))
            if (Perceives(this) &&
                 m->LineOfFire(x,y,cr->x,cr->y,cr))
              if (cr->SkillLevel(sk) > blev)
                { best = cr; blev = cr->SkillLevel(sk); }
    return best;
  }  

/* fjm: I had this a bool, and saved the skill check result in LastSkillCheckResult.
   You (Wes) changed it to give the difference of check and DC, and then I didn't
   realize this and called it as a pass/fail a bunch of times when implementing
   skills. For intuitiveness, I set it back to bool; you can get the old result
   easily by subtracting the DC you sent it from LastSkillCheckResult. */
bool Creature::SkillCheck(int16 sk, int16 DC, bool show, int16 mod1, 
                             const char* mod1Str, int16 mod2, const char* mod2Str)
  {
    int16 roll, rollA, rollB, rollC, sr, armPen = 0; 
    String sStr, sRolls; Creature *msa;
    
    rollA = Dice::Roll(1,20);
    rollB = rollC = 0;
    /* This isn't in the OGL system, but in a roguelike game it is
       important that rogues be able to read scrolls and fire wands,
       and rangers and druids tame animals, with some degree of
       reliability. Abilities which work 50% of the time in combat
       are practically useless in a RL, and continually failing to
       charm animals is just annoying. */
    if (sk == SK_USE_MAGIC || sk == SK_ANIMAL_EMP || sk == SK_DECIPHER)
      rollB = Dice::Roll(1,20);
    
    /* Skill Focus */
    StatiIterNature(this, SKILL_BONUS)
      if (S->Val == sk && S->Source == SS_PERM)
        rollC = Dice::Roll(1,20);
    StatiIterEnd(this)
    
    roll = max(rollA,rollB);
    roll = max(roll, rollC);
    
    if (HasAbility(CA_SKILL_MASTERY))
      {
        // ugly kludge
        rID rogueID = FIND("rogue");
        ASSERT(rogueID);
        if (TCLASS(rogueID)->HasSkill(sk))
          roll = max(roll, min(15,7 + Mod(A_INT)));
      }
      
    
    if (sk == SK_LOCKPICKING || sk == SK_HEALING ||
        sk == SK_HANDLE_DEV || sk == SK_SEARCHING ||
        sk == SK_INTUITION || sk == SK_ANIMAL_EMP ||
        sk == SK_DIPLOMACY || sk == SK_BLUFF ||
        sk == SK_INTIMIDATE || sk == SK_DECIPHER)
      msa = MostSkilledAlly(sk);
    else
      msa = this;
      
    sr   = msa->SkillLevel(sk);

    /* The "Suggestion clause", hardcoded in here because there's no
       other easy way to do it; see the Suggestion spell description
       for more information. */
    if (EventSP != -1 && (sk == SK_DIPLOMACY || sk == SK_BLUFF || 
          sk == SK_INTIMIDATE) && EventStack[EventSP].EVictim &&
          EventStack[EventSP].EVictim != this &&
          EventStack[EventSP].EVictim->isCreature())
      {
        rID eID; Creature *vic = EventStack[EventSP].EVictim;
        if (HasStati(SKILL_BONUS,sk))
          if ((eID = GetStatiEID(SKILL_BONUS,sk)) &&
                (TEFF(eID)->Schools & SC_ENC))
            {
              if (vic->ResistLevel(AD_MIND) == -1)
                sk -= HighStatiMag(SKILL_BONUS,sk);
              else
                sk -= min(HighStatiMag(SKILL_BONUS,sk),
                  vic->ResistLevel(AD_MIND) +
                  vic->HighStatiMag(SAVE_BONUS,SN_ENCH));
            }
      
      }

    if (show && msa != this)
      IPrint("<Obj> aids you with <9><Str><7>.", msa,
        SkillInfo[sk].name);

    bool succ = (sr + roll + mod1 + mod2 + armPen) >= DC ||
                  (roll == 20 && !isThreatened() && (sk == SK_ESCAPE_ART ||
                   sk == SK_CLIMB || sk == SK_HANDLE_DEV || sk == SK_SEARCH ||
                   sk == SK_BALANCE));
      

    if (show && isPlayer()) {
      thisp->MyTerm->SetWin(WIN_NUMBERS);
      thisp->MyTerm->Clear();
      String modStr;
      modStr = "";
      if (mod1Str && strlen(mod1Str) != 0 && mod1) {
        if (isalpha(mod1Str[0]))
          modStr += Format(" %+d %s",mod1,mod1Str);
        else
          modStr += mod1Str;
        }
      if (mod2Str && strlen(mod2Str) != 0 && mod2) {
        if (isalpha(mod2Str[0]))
          modStr += Format(" %+d %s",mod2,mod2Str);
        else
          modStr += mod2Str;
        }
        
      if (rollB && rollC)
        sRolls = Format(" [%d/%d/%d]",rollA,rollB,rollC);
      else if (rollB)
        sRolls = Format(" [%d/%d]",rollA,rollB);
      else if (rollC)
        sRolls = Format(" [%d/%d]",rollA,rollC);
      else
        sRolls = "";
        
      sStr = Format((DC == 0) ? "%c%s Check%s%s%s:%c 1d20 (%d%s) %+d%s%s = %d."
        : "%c%s Check%s%s%s:%c 1d20 (%d%s) %+d%s%s = %d vs DC %d %c[%s]%c.",
        -13,SkillInfo[sk].name, 
        msa == this ? "" : " [",
        msa == this ? "" : (const char*) Lower(msa->Name(0)),
        msa == this ? "" : "]",
        -7,roll, (const char*) sRolls,
        sr,(const char*) modStr,
        armPen ? (const char*)Format(" %+d armour",armPen) : "",
        sr+roll+mod1+mod2+armPen,DC,succ ? -EMERALD : -PINK,
        succ ? "success" : "failure", -7);
      thisp->MyTerm->Write(0,0,sStr);
      if (thisp->Opt(OPT_STORE_ROLLS))
        thisp->MyTerm->AddMessage(sStr);
      }
    LastSkillCheckResult = sr + roll + mod1 + mod2 + armPen;
    RemoveOnceStati(SKILL_BONUS,sk);
    /* Kludge -- -2 == -1, see RemoveOnceStati */
    RemoveOnceStati(SKILL_BONUS,-2);
    
    if (isCharacter() && DC)
      if (sr + roll + mod1 + mod2 + armPen >= DC)
        if (sr + 1 + mod1 + mod2 + armPen < DC)
          {
            int16 col, die, i, cap; bool testThreat;
            testThreat = false;
            die = (DC - 10);
            col = EXXX_SKILL;
            cap = 30;
            
            switch (sk)
              {
                case SK_LOCKPICKING:
                  col = EXXX_PSKILL;
                  cap = 60;
                  die /= 2;
                 break;
                case SK_JUMP:
                  testThreat = true;
                  col = EDEX_JUMP;
                  die /= 2;
                  cap = 20;
                 break;
                case SK_METAMAGIC:
                  testThreat = true;
                  cap = 50;
                 break;
                case SK_BALANCE:
                  testThreat = true;
                  if (m->FallAt(x,y))
                    { cap = 75; die *= 2; }
                 break;
                case SK_BLUFF:
                case SK_INTIMIDATE:
                case SK_DIPLOMACY:
                  col = EXXX_PSKILL;
                  die *= 2;
                  cap = 75;
                 break;
                case SK_DECIPHER:
                  die /= 2;
                  col = EXXX_PSKILL;
                  cap = 60;
                break;
                case SK_ESCAPE_ART:
                case SK_RIDE:
                case SK_SWIM:
                  testThreat = true;
                break;
              }
            if ((!testThreat) || isThreatened()) {
              for (i=1;SkillInfo[i].name;i++)
                if (SkillInfo[i].sk == sk && die >= 1)
                  {
                    Exercise(SkillInfo[i].attr1,Dice::Roll(1,(int8)die),col,cap);
                    if (SkillInfo[i].attr2 != SkillInfo[i].attr1)
                        Exercise(SkillInfo[i].attr2,Dice::Roll(1,(int8)die),col,cap);
                  }
              for (i=0;GodIDList[i];i++)
                {
                  int32 fsList[30];
                  TGOD(GodIDList[i])->GetList(FAVOURED_SKILLS,(rID*)fsList,30);
                  for (int8 j=0;fsList[j];j++)
                    if (sk == fsList[j])
                      goto OnTheList;
                  continue;
                  OnTheList:
                  gainFavour(GodIDList[i], max(0,10 * (sr + roll + mod1 + 
                              mod2 + armPen - 15)), false, true);
                } 
              }
          }
    
    return succ;
  } 

void Character::UseFeat(uint16 ft)
  {
    switch(ft)
      {
        case FT_SCRIBE_SCROLL:
          CraftItem(FT_SCRIBE_SCROLL + FEAT_VAL);
         break;
        case FT_BREW_POTION:
          CraftItem(FT_BREW_POTION + FEAT_VAL);
         break;
        case FT_DANCE_OF_TUKIMA:
          ThrowEff(EV_EFFECT,FIND("Dance of Tukima"),this);
         break;
        //case FT_GREATER_SOULBLADE:
        // break;
        case FT_WARCRY:
          ThrowEff(EV_EFFECT,FIND("Warcry"),this);
         break;          
        default:
          IPrint("That feat isn't implemented yet.");
      }
  }

bool PossiblyPause(Term *T1, int x, int y, int timeout);

void Character::UseAbility(uint8 ab,int16 pa) {
    int16 i,j,k, cx, cy, fc;
    Thing *targ;
    Creature *c;
    EventInfo e;
    String msg1, msg2;
    k = 0; // ww: previously used before defined in TRACKING
    switch(ab) {
    case CA_ANIMAL_COMP: 
        if (isPlayer())
            thisp->SummonAnimalCompanion(false);
        break; 
    case CA_SACRED_MOUNT:
        if (isPlayer())
            thisp->SummonAnimalCompanion(true);
        break;
    case CA_SOOTHING_WORD:
        j = 10 + AbilityLevel(CA_SOOTHING_WORD)/2 + Mod(A_CHA);
        if (!LoseFatigue(3,true))
            break;

        IDPrint("You speak a word of indescribable peace.",
            "The <Obj> speaks a word that moves your spirit.",this);
        MapIterate(m,c,i)
            if (c->isCreature() && c->isHostileTo(this))
                if (c->isMType(MA_LIVING) && !c->isMType(MA_SAPIENT))
                    if (dist(x,y,c->x,c->y) <= 6) {
                        if (!c->SavingThrow(WILL,j,SA_ENCH|SA_MAGIC)) {
                            c->ts.Pacify(c,this);
                            c->StateFlags |= MS_PEACEFUL;
                            if (c->isCreature())
                                ((Creature*)c)->ts.Retarget((Creature *)c);
                            c->IDPrint("You feel your hostility drain away.",
                                "The <Obj> calms and ceases attacking.",c);
                        } else
                            c->IDPrint("You resist the effects.",
                            "The <Obj> resists the effects.",c);
                    }
                    break;
    case CA_LAY_ON_HANDS:
        e.Clear();
        e.EActor = this;
        if (!thisp->MyTerm->EffectPrompt(e,Q_TAR|Q_NEAR|Q_CRE))
            return;
        if (!e.EVictim->isCreature())
            return;
        if (!e.EActor->LoseFatigue(2,true))
            return;
        e.EActor->Timeout += 30;
        if (e.EVictim == e.EActor)
            DPrint(e,"You lay hands upon yourself.",
            "The <EActor> lays hands upon <him:EActor>self.");
        else
            TPrint(e,"You lay hands upon the <EVictim>.",
            "The <EActor> lays hands upon you.",
            "The <EActor> lays hands upon the <EVictim>.");                     
        if (e.EVictim->isMType(MA_UNDEAD)) {
            VPrint(e,"Holy energy sears you!",
                "Holy energy sears the <EVictim>!");
            ThrowDmg(EV_DAMAGE,AD_HOLY,Dice::Roll(2,6) + max(1,2+Mod(A_CHA))*AbilityLevel(CA_LAY_ON_HANDS),
                "a paladin's touch",this,e.EVictim);
        } else if (e.EVictim->isMType(MA_LIVING) && 
            e.EVictim->cHP != (e.EVictim->mHP+e.EVictim->Attr[A_THP])) {
                e.EVictim->cHP = min(e.EVictim->mHP+e.EVictim->Attr[A_THP],
                    e.EVictim->cHP + Dice::Roll(2,6) + max(1,2+Mod(A_CHA))*AbilityLevel(CA_LAY_ON_HANDS));
                VPrint(e,"Your wounds heal<Str>!", "The <EVictim>'s wounds heal<Str>!",
                    e.EVictim->cHP == e.EVictim->mHP+e.EVictim->Attr[A_THP] ? " fully" : "");
        } else
            DPrint(e,"Nothing happens.", "Nothing happens.");
        return;          
    case CA_INNATE_SPELL:
        e.Clear();
        e.EActor = this;
        e.EMap = m;
        e.eID = theGame->SpellID(pa);
        if (TEFF(e.eID)->ef.qval)
            if (!thisp->MyTerm->EffectPrompt(e,TEFF(e.eID)->ef.qval))
                return;
        ReThrow(EV_INVOKE,e);
        break;
    case CA_BERSERK_RAGE:
        fc = 3;
        if (!InSlot(SL_ARMOUR))
            fc = 1;
        else if (InSlot(SL_ARMOUR)->isGroup(WG_HARMOUR))
            fc = 4;
        else if (InSlot(SL_ARMOUR)->isGroup(WG_MARMOUR))
            fc = 3;
        else if (InSlot(SL_ARMOUR)->isGroup(WG_LARMOUR))
            fc = 2;
        else
            Error("Armour is neither Light, Medium nor Heavy?!");

        if (HasStati(AFRAID)) {
            IPrint("You cannot rage while afraid!");
            break;
        }

        if (AbilityLevel(CA_BERSERK_RAGE) >= 4)
            fc -= 1;
        if (fc == 0)
            if (!(Dice::Roll(1,20) + Mod(A_CON) > 20))
                fc = 1;    

        if (fc)
            if (!LoseFatigue(fc,true))
                break;
        if (AbilityLevel(CA_BERSERK_RAGE) >= 16)
            i = 8;
        else if (AbilityLevel(CA_BERSERK_RAGE) >= 10)
            i = 6;
        else if (AbilityLevel(CA_BERSERK_RAGE) >= 3 || !Level[1])
            i = 4;
        else 
            i = 2;
        //Timeout += 6;
        //GainPermStati(RAGING,NULL,SS_MISC,i,AbilityLevel(CA_BERSERK_RAGE));
        GainTempStati(RAGING,NULL,10+AbilityLevel(CA_BERSERK_RAGE)*3,SS_MISC,i,
            AbilityLevel(CA_BERSERK_RAGE));
        FFCount = 0;
        IDPrint("You drive yourself into a killing fury.",
            "The <Obj> flies into a killing fury!",this);
        break;
    case CA_TRACKING:
        j = max(1,Mod(A_WIS)+1+(AbilityLevel(CA_TRACKING)/5));
        k = 0;
        StatiIter(this)
            if (S->Nature == TRACKING)
                k++;
        StatiIterEnd(this)
        if (k >= j) {
            IPrint("You are tracking too many targets already.");
            return;
        }
        targ = thisp->MyTerm->ChooseTarget();
        if (targ == NULL || targ == this)
            break;
        GainPermStati(TRACKING,targ,SS_MISC,0,min(125,40 + AbilityLevel(CA_TRACKING)*5));
        m->Update(targ->x,targ->y);
        break;
    case CA_MANIFESTATION:
        if (!LoseFatigue(3,true))
            return;
        IDPrint("Your mind travels back to the ancient ages of faerie, and you "
            "feel eldritch power well up within you...",
            "The <Obj> becomes a spectable of searing, terrible majesty...");
        GainTempStati(MANIFEST,NULL,Dice::Roll(1,4,TotalLevel()/3),SS_MISC,0,0,0);
        break;                     
    case CA_PROTECTIVE_WARD:
        if (!LoseFatigue(2,true))
            return;
        IPrint("You call upon divine aid to ward away harm.");
        GainTempStati(ADJUST_SAC,this,20+AbilityLevel(CA_PROTECTIVE_WARD)*3,
            SS_ONCE,A_SAV,AbilityLevel(CA_PROTECTIVE_WARD),FIND("protective ward"));
        break;  
    case CA_FEAT_OF_STRENGTH:
        if (!LoseFatigue(4,true))
            return;
        IPrint("You push your strength to the limit!");
        GainTempStati(ADJUST_SAC,this,1,SS_MISC,A_STR,AbilityLevel(CA_FEAT_OF_STRENGTH),
            FIND("feat of strength"));
        break;
    case CA_UNBIND:
        Glyph g;
        if (!LoseFatigue(2,true))
            return;
        e.Clear();
        e.EActor = this;
        e.EVictim = NULL;
        DPrint(e,"You invoke the liberating power of <Str>!",
            "<Obj2> invokes the power of <Str> to free the enslaved!",
            isCharacter() ? NAME(thisp->GodID) : "Semirath",this);
        i = max(1,AbilityLevel(CA_UNBIND) + Mod(A_CHA));

        if (PossiblyPause(T1,x,y,0)) {
            for(cx = x-i; cx <= x+i; cx++)
                for(cy = y-i; cy <= y+i; cy++)
                    if ((dist(cx,cy,x,y) <= i) && m->InBounds(cx,cy))
                    {
                        g = T1->GetGlyph(cx,cy);
                        g = (g & 0x00FF) | AZURE<<8;
                        T1->PutGlyph(cx,cy,g);
                    }
                    T1->Update();
                    PossiblyPause(T1,x,y, isPlayer() ? 1500 : 500);
                    T1->RefreshMap();
                    T1->Update();
        }
        MapIterate(m,targ,i)
            if (dist(x,y,targ->x,targ->y) <= i)
                if (targ->isCreature())
                    if (targ->HasStati(SUMMONED)) {
                        c = (Creature*)targ->GetStatiObj(SUMMONED);
                        if (c && c->isCreature()) {
                            if (((Creature*)targ)->isHostileTo(c))
                                continue;
                            int16 jm, km;
                            j = random(20)+1;
                            k = random(20)+1;
                            jm = j + Mod(A_CHA);
                            km = k + c->Mod(A_CHA);
                            if (isPlayer()) {                           
                                thisp->MyTerm->SetWin(WIN_NUMBERS3);
                                thisp->MyTerm->Clear();
                                msg1 = Format("%cUnbind Check (%s %s):%c ",
                                    -YELLOW,
                                    (const char*)c->Name(NA_POSS|NA_THE),
                                    (const char*)targ->Name(0),
                                    -GREY);
                                msg2 = Format("1d20 [%d] + %d Cha vs. 1d20 [%d] + %d Cha %c[%s]%c",
                                    j, Mod(A_CHA), k, c->Mod(A_CHA),
                                    jm >= km ? (-EMERALD) : (-PINK),
                                    jm >= km ? "success" : "failure",
                                    -GREY);
                                thisp->MyTerm->Write(0,0,msg1);
                                thisp->MyTerm->Write(2,1,msg2);
                                if (thisp->Opt(OPT_STORE_ROLLS))
                                    thisp->MyTerm->AddMessage(msg1 + msg2);
                            }
                            if (jm >= km) {
                                if (targ->isMonster())
                                    ((Monster*)targ)->ts.Liberate((Monster*)targ,this);  
                            } else
                                IPrint("The <Obj1> retains control of <his:Obj1> <Obj2>!",
                                c, targ);
                        } else if (targ->isMonster())
                            ((Monster*)targ)->ts.Liberate((Monster*)targ,this);  
                    }
        break;
    case CA_BARDIC_MUSIC:
        int16 range, bonus, song; 
        range = SkillLevel(SK_PERFORM) + 2;
        if (HasStati(SINGING)) {
            if (yn("You already have a song in progress. Cut it off?"))
                RemoveStati(SINGING);
            else
                return;
        }
        if (m->FieldAt(x,y,FI_SILENCE)) {
            IPrint("You can't sing in an area of magical silence.");
            return;
        }
        if (isPlayer()) {
            thisp->MyTerm->LOption("Inspire Courage",BARD_COURAGE);
            if (AbilityLevel(ab) >= 3)
                thisp->MyTerm->LOption("Fascination",BARD_FASCINATE);
            if (AbilityLevel(ab) >= 5)
                thisp->MyTerm->LOption("Countersong",BARD_COUNTER);
            if (AbilityLevel(ab) >= 7)
                thisp->MyTerm->LOption("Inspire Competence",BARD_COMP);
            if (AbilityLevel(ab) >= 9)
                thisp->MyTerm->LOption("Hymn of the Phoenix Arisen",BARD_PHOENIX);
            if (AbilityLevel(ab) >= 10)
                thisp->MyTerm->LOption("Mass Fascination",BARD_MASS);
            if (AbilityLevel(ab) >= 11)
                thisp->MyTerm->LOption("Spellbreaker Chant",BARD_SPELLBREAK);

            song = (int16)thisp->MyTerm->LMenu(MENU_ESC|MENU_BORDER," -- Bardic Music -- ",WIN_MENUBOX,"bard");
            if (song == -1)
                return;
        } else
            song = pa;
        if (!LoseFatigue(1,true))
            return;
        Timeout += 15;

        bonus = max(1,Mod(A_CHA));
        if (isCharacter())
            if (thisp->Level[1] || thisp->Level[2])
                bonus = max(bonus,thisp->LevelAs(FIND("bard")));

        GainTempStati(SINGING,NULL,5+(max(0,Mod(A_CON))+SkillLevel(SK_PERFORM))*3,SS_MISC,song,range,0);

        switch (song) {
        case BARD_COURAGE:
        case BARD_COMP:
            IDPrint("You begin an awesome, uplifting song.",
                "The <Obj> sings an awe-inspiring song!"); break;
        case BARD_PHOENIX:
            IDPrint("You begin a rousing, vital ballad.",
                "The <Obj> sings an awe-inspiring song!"); break;
        case BARD_SPELLBREAK:
            IDPrint("You begin a sonorous, rhythmic chant.",
                "The <Obj> sings an awe-inspiring song!"); break;
        case BARD_FASCINATE:
        case BARD_MASS:
            IDPrint("You begin a lilting, hypnotic melody.",
                "The <Obj> sings an awe-inspiring song!"); break;
        case BARD_COUNTER:
            IDPrint("You begin singing an aggressive, engulfing crescendo.",
                "The <Obj> sings an awe-inspiring song!"); break;
        }
        switch(song) {
        case BARD_COURAGE:
            MapIterate(m,c,i)
                if (c->isCreature() && c != this)
                    if (dist(x,y,c->x,c->y) <= range)
                        if (!c->isHostileTo(this)) {
                            if (c->HasMFlag(M_DEAF) || c->HasMFlag(M_MINDLESS) ||
                                m->FieldAt(c->x,c->y,FI_SILENCE))
                                continue;
                            c->IDPrint("You feel valourous!",
                                "The <Obj> seems inspired!", c);
                            c->RemoveStati(AFRAID);
                            c->GainPermStati(IMMUNITY,this,SS_SONG,AD_FEAR);
                            c->GainPermStati(ADJUST_MOR,this,SS_SONG,A_HIT,bonus);
                            c->GainPermStati(ADJUST_MOR,this,SS_SONG,A_DMG,bonus);
                        }
            break;
        case BARD_COUNTER:
        case BARD_SPELLBREAK:
            /* This is handled in Creature::SAttack & Creature::Cast. */
            break;
        case BARD_PHOENIX:
            MapIterate(m,c,i)
                if (c->isCreature())
                    if (dist(x,y,c->x,c->y) <= range)
                        if (!c->isHostileTo(this)) {
                            if (c->HasMFlag(M_DEAF) || c->HasMFlag(M_MINDLESS) || m->FieldAt(c->x,c->y,FI_SILENCE))
                                continue;
                            c->IDPrint("You feel uplifted!", "The <Obj> seems revitalized!", c);                                 
                            c->GainPermStati(RESIST,this,SS_SONG,AD_FIRE,range-2);
                            c->GainPermStati(ADJUST_CIRC,this,SS_SONG,A_FAT,6);
                        }
            break;
        case BARD_FASCINATE:
GetTarget:
            c = (Creature*) thisp->MyTerm->ChooseTarget(NULL,T_CREATURE);
            if (!c)
                return;
            if (!c->isCreature())    
                goto GetTarget;          
            if (dist(x,y,c->x,c->y) > range)
                goto GetTarget;
            if (c->HasMFlag(M_DEAF) || c->HasMFlag(M_MINDLESS) || m->FieldAt(c->x,c->y,FI_SILENCE) ||
                c->ResistLevel(AD_PLYS) == -1 || dist(x,y,c->x,c->y) > range) {
                c->IDPrint("You are unaffected.","The <Obj> is unaffected.",c);
                break;
            }
            if (!c->SavingThrow(WILL,10+AbilityLevel(CA_BARDIC_MUSIC)/2+Mod(A_CHA),SA_ENCH|SA_PARA)) {
                c->IDPrint("You are transfixed by the music!",
                    "The <Obj> is transfixed by the music!",c);
                c->GainPermStati(PARALYSIS,this,SS_SONG,1);
            } else {
                c->IDPrint("You resist the song's effects.",
                    "The <Obj> resists the song's effects.",c);
                RemoveStati(SINGING);
            }
            break;
        case BARD_MASS:
            MapIterate(m,c,i) {
                if (!c->isCreature())    
                    continue;          
                if (dist(x,y,c->x,c->y) > range)
                    continue;
                if (!c->isHostileTo(this))
                    continue;
                if (!(c->Perceives(this) & (PER_VISUAL|PER_INFRA))) {
                    if (XPerceives(c))
                        IPrint("The <Obj> cannot see you, and is thus unaffected.");
                    continue;
                }
                if (c->HasMFlag(M_DEAF) || c->HasMFlag(M_MINDLESS) || 
                    m->FieldAt(c->x,c->y,FI_SILENCE) ||
                    c->ResistLevel(AD_PLYS) == -1 || dist(x,y,c->x,c->y) > range)
                {
                    c->IDPrint("You are unaffected.","The <Obj> is unaffected.",c);
                    continue;
                }
                if (!c->SavingThrow(WILL,10+AbilityLevel(CA_BARDIC_MUSIC)/2+Mod(A_CHA),SA_ENCH|SA_PARA)) {
                    c->IDPrint("You are transfixed by the music!",
                        "The <Obj> is transfixed by the music!",c);
                    c->GainPermStati(PARALYSIS,this,SS_SONG,1);
                } else {
                    c->IDPrint("You resist the song's effects.",
                        "The <Obj> resists the song's effects.",c);
                    RemoveStati(SINGING);
                }
            }
            break;
        case BARD_COMP:
            MapIterate(m,c,i)
                if (c->isCreature() && c != this)
                    if (dist(x,y,c->x,c->y) <= range)
                        if (!c->isHostileTo(this)) {
                            if (c->HasMFlag(M_DEAF) || c->HasMFlag(M_MINDLESS))
                                continue;
                            c->IPrint("You feel skillful!", "The <Obj> takes on a determined look!", c);
                            c->GainPermStati(ADJUST_COMP,this,SS_SONG,A_HIT,bonus);
                            c->GainPermStati(ADJUST_COMP,this,SS_SONG,A_SPD,bonus*3);
                            c->GainPermStati(ADJUST_COMP,this,SS_SONG,A_DEF,bonus*2);
                            c->GainPermStati(ADJUST_COMP,this,SS_SONG,A_SAV,bonus);
                        }
            break;
        default:
            IPrint("That bardic music facet isn't implemented yet."); 
        }
        return;
    case CA_TURNING:
    case CA_GREATER_TURNING:
    case CA_COMMAND:
        e.Clear();
        e.EParam = pa;
        e.AType = ab;
        e.EActor = this;
        e.vDmg = SumStatiMag(ab == CA_COMMAND ? COMMAND_ABILITY : TURN_ABILITY,pa);                            
        ReThrow(EV_TURNING,e);
        break;
    case CA_PHASE: 
        Throw(EV_PHASE,this);
        return;
    case CA_DWARVEN_FOCUS:
        targ = thisp->MyTerm->ChooseTarget();
        if (targ == NULL || targ == this)
            break;
        if (!targ->isCreature()) {
            IPrint("There is no honour in slaying inanimate opponents.");
            break;
        }
        if (ChallengeRating() < ((Creature*)targ)->ChallengeRating()) {
            IPrint("That's hardly a worthy foe for you!");
            break;
        }

        if (HasStati(DWARVEN_FOCUS)) {
            IPrint(GetStatiObj(DWARVEN_FOCUS) ? "You already have a focus at this time." :
                "You need to rest before choosing another focus.");
            break;
        }
        GainPermStati(DWARVEN_FOCUS,targ,SS_MISC,0,0,0);
        targ->Flags |= F_HILIGHT;
        IPrint("You swear a vow to slay the <Obj>.",targ);
        break;
    case CA_WHOLENESS_OF_BODY:
        if (cHP == mHP+Attr[A_THP]) {
            IPrint("You're already fully healed.");
            return;
        }
        if (!LoseFatigue(1,true))
            return;
        cHP = min(mHP+Attr[A_THP],cHP+TotalLevel()*3);
        IPrint("Your wounds heal<Str>.", (cHP == mHP+Attr[A_THP]) ? " fully" : "");
        IDPrint(NULL,"The <Obj>'s wounds heal<Str>.", this,
            (cHP == mHP+Attr[A_THP]) ? " fully" : "");
        break;      
    case CA_FLURRY_OF_BLOWS:
        if (HasStati(FLURRYING)) {
            IPrint("You're already performing a flurry.");
            return;
        }
        if (!LoseFatigue(1,true))
            return;
        GainTempStati(FLURRYING,NULL,10+max(0,Mod(A_CON)),SS_MISC);
        return;   
    case CA_WILD_SHAPE:
        if (isPlayer())
            thisp->WildShape(); 
        break;
    case CA_SPECIES_AFFINITY:
        UseSkill(SK_ANIMAL_EMP);
        break;
    case CA_ARCANE_TRICKERY:
        int16 sp;
        if (!HasStati(SPEC_TIMEOUT,CA_ARCANE_TRICKERY))
            GainPermStati(SPEC_TIMEOUT,NULL,SS_MISC,0,0);
        i = GetStatiMag(SPEC_TIMEOUT,CA_ARCANE_TRICKERY);
        if (i >= 3) {
            IPrint("You've already expended all your uses of Arcane " \
                "Trickery for the day.");
            return;
        }

        ASSERT(isPlayer());
        sp = thisp->MyTerm->SpellManager(SM_CAST|SM_TRICKERY);
        if (sp == -1)
            return;
        SetStatiMag(SPEC_TIMEOUT,CA_ARCANE_TRICKERY,NULL,i+1);

        e.Clear();
        e.EActor = this;
        e.eID = theGame->SpellID(sp);
        e.isArcaneTrickery = true;
        if (!thisp->MyTerm->EffectPrompt(e,TEFF(e.eID)->ef.qval |
            ((MMFeats(sp) & MM_PROJECT) ? Q_DIR : 0)))
            break;
        if (Spells[sp] & SP_INNATE) 
            ReThrow(EV_INVOKE,e);
        else 
            ReThrow(EV_CAST,e);
        return;
    case CA_ANCESTRAL_MEMORY:
        if (HasStati(ANC_MEM)) {
            IPrint("You're already in an ancestral trance.");
            return;
        }
        for (i=1;i!=SK_LAST;i++)
            if (SkillInfo[i].imp)
                thisp->MyTerm->LOption(SkillInfo[i].name,
                SkillInfo[i].sk,
                DescribeSkill(SkillInfo[i].sk));
        j = (int16)thisp->MyTerm->LMenu(MENU_SORTED|MENU_3COLS|MENU_DESC|MENU_BORDER,
            "Choose a skill to augment:",WIN_MENUBOX,"help::skills");
        if (j == -1)
            return;
        if (!LoseFatigue(2,true))
            return;
        IDPrint("You center yourself and call upon the talents of "
            "those who came before you...", "The <Obj1> closes "
            "<his:Obj1> eyes and meditates for a period.", this);
        GainTempStati(ANC_MEM,NULL, 30 + SkillLevel(SK_CONCENT)*5, SS_MISC,j,0,0,0); 
        break;
    case CA_FLAWLESS_DODGE:
        if (!HasStati(FLAWLESS_DODGE))
            GainPermStati(FLAWLESS_DODGE,NULL,SS_MISC,1,0);
        else if (GetStatiMag(FLAWLESS_DODGE) >= AbilityLevel(CA_FLAWLESS_DODGE) + Mod(A_DEX)) {
            IPrint("You have no flawless dodges left today.");
            break;
        } else if (HasStati(FLAWLESS_DODGE,1)) {
            IPrint("You're already dodging.");
            break;
        } else 
            SetStatiVal(FLAWLESS_DODGE,NULL,1);
        IPrint("You drop into a dangerous, fluid pose...");
        break;
    case CA_STORYCRAFT:
        CraftItem(CA_STORYCRAFT + ABIL_VAL);
        break; 
    case CA_WEAPONCRAFT:
        CraftItem(CA_WEAPONCRAFT + ABIL_VAL);
        break; 
    default:
        IPrint("That ability isn't implemented yet.");
    }
}

bool Creature::UseLimitedFA()
  {
    int16 i, j;
    i = AbilityLevel(CA_LIMITED_FREE_ACTION);
    if (HasFeat(FT_RESIST_PARALYSIS))
      i += Mod(A_CON)*2;
    j = GetStatiVal(LFA_COUNT);
    if (max(j,0) >= i)
      return false;
    IDPrint("You overcome the paralysis effect. (<Num> left).",
            "The <Obj2> overcomes the paralysis effect.",
            i - (j+1), this);
    if (!HasStati(LFA_COUNT))
      GainPermStati(LFA_COUNT,NULL,SS_MISC,1,0);
    else
      SetStatiVal(LFA_COUNT,NULL,j+1);
    return true; // ww: previously no return value! 
  }
  
TextVal Races[] = {
  { MA_HUMAN,       "human" },
  { MA_ELF,         "elf" },
  { MA_DWARF,       "dwarf" },
  { MA_GNOME,       "gnome" },
  { MA_HALFLING,    "halfling" },
  { MA_REPTILE+MA_HUMANOID*256, "lizardman" },
  { MA_REPTILE+MA_HUMANOID*256, "locathah" },
  { MA_ORC,         "cave orc" },
  { MA_ORC,         "hill orc" },
  { MA_GOBLIN,      "goblin" },
  { MA_KOBOLD,      "kobold" },
  { MA_GOBLIN,      "hobgoblin" },
  { MA_GOBLIN,      "bugbear" },
  { MA_UNDEAD,      "ghoul" },      
  { MA_UNDEAD,      "wight" },      
  { 0,              "tiefling" },
  { 0,              "aasimar" },
  { 0,              "grimlock" },
  { 0,              "sahuagin" },
  { 0,              "medusa" },
  { 0,              "leprechaun" },
  { MA_ILLITHID,    "mind flayer" },
  { MA_FAERIE,      "satyr" },
  { MA_FAERIE,      "nymph" },
  { 0,              NULL } };

int16 Similarities[][2] = {
  { MA_HUMAN, MA_ELF },
  { MA_HUMAN, MA_DWARF },
  { MA_HUMAN, MA_HALFLING },
  { MA_HUMAN, MA_GNOME },
  { MA_HUMAN, MA_PLANETOUCHED },
  { MA_ORC,   MA_GOBLIN },
  { MA_ELF,   MA_FAERIE },
  { MA_ELF,   MA_DROW },
  { MA_HALFLING, MA_GNOME },
  { MA_REPTILE+MA_HUMANOID*256, MA_KOBOLD },
  { 0, 0 } };
  
int16 DisguiseDC(Creature *me, rID disID)
  {
    int16 DC, sizediff, i; 
    const char *name = NAME(disID);
    
    DC = 16;
      
    for (i=0;Similarities[i][0];i++) {
      if (me->isMType(Similarities[i][0]) && 
            TMON(disID)->isMType(disID,Similarities[i][1]))
        DC = 13;
      if (me->isMType(Similarities[i][1]) && 
            TMON(disID)->isMType(disID,Similarities[i][0]))
        DC = 13;
      }
            
    for (i=0;Races[i].Text;i++)
      if (!stricmp(Races[i].Text,name))
        if (Races[i].Val && me->isMType(Races[i].Val))
          DC = 10;
    
    // no reason you can't be a gnome ghoul or hill giant zombie...
    if (TMON(disID)->isMType(disID,MA_UNDEAD))
      goto SkipSizeMod;
    
    sizediff = abs(me->GetAttr(A_SIZ) - TMON(disID)->Size);
    if (sizediff > 1)
      return -1;
    else if (sizediff)
      DC += 7;
    
    SkipSizeMod:
    
    if (TMON(disID)->HasFlag(M_ALL_FEMALE) &&
         !(me->StateFlags & MS_FEMALE))
      DC += 4;
    if (TMON(disID)->HasFlag(M_ALL_MALE) &&
         (me->StateFlags & MS_FEMALE))
      DC += 4;
    
    return DC;
  
  }
  
EvReturn Creature::DonDisguise(EventInfo &e)
  {
    Creature *cr; rID spID, nmID; 
    int16 i, n, DC, sk; bool found;
    
    if (HasStati(DISGUISED))
      {
        IPrint("You scrap your old disguise.");
        RemoveStati(DISGUISED);
      }
      
    if (!HasMFlag(M_HUMANOID))
      {
        IPrint("Only humanoids can use the Disguise skill effectively.");
        return ABORT;
      }
      
    if (GetStatiDur(POLYMORPH) > 0 ||
        GetStatiDur(ADJUST,A_SIZ) > 0)
      {
        IPrint("You won't stay changed long enough to adopt a disguise "
          "in this form.");
        return ABORT;
      }
      
    found = false; n = 0;
    for (i=0;Races[i].Text;i++)
      {
        nmID = FIND(Races[i].Text);
        if (!nmID)
          continue;
        DC = DisguiseDC(this,nmID);
        if (DC == -1)
          continue;
        found = true;
        if (isPlayer())
          thisp->MyTerm->LOption(Format("%s (DC %d)", 
            NAME(nmID), DC),nmID,DESC(nmID),DC);
        else
          Candidates[n++] = nmID;
      }
    
    if (!found) 
      {
        IPrint("Your current form gives you no viable disguise choices.");
        return ABORT;
      }
    if (isPlayer())
      nmID = thisp->MyTerm->LMenu(MENU_BORDER|MENU_ESC|MENU_SORTED, "Choose a Disguise:");
    else
      nmID = Candidates[random(n)];
    
    if (nmID == -1)
      return ABORT;
    
    IPrint("You begin work on your disguise...");

    spID = GetStatiEID(SKILL_BONUS,SK_DISGUISE);
    sk   = SkillLevel(SK_DISGUISE);
    
    if (isThreatened())
      {
        IPrint("You can't don a disguise with hostile creatures nearby!");
        return ABORT;
      }
      
    if (isPlayer() && !spID)
      if (thisp->SpendHours(1,1) != DONE)
        return ABORT;
    
    if (!SkillCheck(SK_DISGUISE,DisguiseDC(this,nmID),true,
          GetStatiMag(TRIED,SK_DISGUISE),"retry"))
      {
        IPrint("You fail to come up with what you consider a "
          "viable disguise after an hour's worth of work.");
        IncStatiMag(TRIED,SS_MISC,SK_DISGUISE,NULL,-2);
        SetStatiDur(TRIED,SK_DISGUISE,NULL,-2);
        return DONE;
      }
    
    IPrint("You disguise yourself as a <Res>.", nmID);
    
    cr = new Monster(nmID);
    cr->CalcValues();
    GainPermStati(DISGUISED,cr,spID ? SS_ENCH : SS_MISC,
      LastSkillCheckResult, sk, spID, spID ? CasterLev() : 0);
    if (sk >= 15)
      ;
    else
      GainPermStati(ADJUST_CIRC,cr,SS_MISC,A_AID, sk >= 10 ? -1 : -2,
        spID, spID ? CasterLev() : 0);
    return DONE;
  }
    
    
EvReturn Creature::Research(EventInfo &e)
  {
    Item *it; Creature *cr; hObj h;
    int16 cLevel, cCount, tx, ty, c;
    if (!HasSkill(SK_KNOW_MAGIC))
      {
        IPrint("You must have the Knowledge (Magic) skill to do research.");
        return ABORT;
      }
    if (m->RegionAt(x,y) != FIND("ancient library"))
      {
        IPrint("You need to find a library to do research.");
        return ABORT;
      }
  
    switch (ChoicePrompt("Research item or curse?","ic", 'c'))
      {
        case -1:
          return ABORT;
        case 'c':
          cCount = 0; cLevel = 0;
          StatiIter(this)
            if (S->Source == SS_CURS) {
                cLevel = max(cLevel,(int16)S->CLev);
                cCount++;
            }
          StatiIterEnd(this)
          
          for (it=FirstInv();it;it=NextInv())
            if (it->IFlags & IF_CURSED)
              if (it->IFlags & IF_WORN)
                { cLevel = it->ItemLevel(); cCount++; }

          if (!cCount)
            {
              IPrint("You have no curse to research.");
              return ABORT;
            }
          if (HasEffStati(TRIED,1000+SK_KNOW_MAGIC))
            {
              IPrint("You've already tried to research your curse<str>; you "
                "can do so again tommorow.", cCount > 1 ? "s" : "");
              return ABORT;
            }
          if (isPlayer())
            if (thisp->SpendHours(4,4) != DONE)
              return ABORT;
              
          if (GetStatiObj(TRIED,1000+SK_KNOW_MAGIC))
            {
              IPrint("Your fated adversary, <str>, still haunts you.",
                (const char*) GetStatiObj(TRIED,1000+SK_KNOW_MAGIC)->Name(NA_THE));
              return ABORT;
            
            }
              
          if (!SkillCheck(SK_KNOW_MAGIC,10+cLevel,true))
            {
              IPrint("You fail to turn up any useful information on "
                "your curse<str>.", cCount > 1 ? "s" : "");
              GainTempStati(TRIED,NULL,-2,SS_MISC,1000+SK_KNOW_MAGIC);
              return ABORT;
            }
          
          THROW(EV_ENGEN,
            e.enFlags = EN_SINGLE|EN_NOPLACE;
            e.enCR = ChallengeRating() + cLevel/2;
            e.enDepth = m->Depth;
            e.enAlign = AL_EVIL;
            e.enDesAmt = 1;
            if (e.enCR < 7 || !random(2))
              { e.enID = FIND("Generic Summons");
                e.enConstraint = (MA_UNDEAD+MA_EVIL*256); }
            else
              e.enID = FIND("lone template undead");
            );
          cr = m->GetEncounterCreature(0);
          if (!cr)
            return ABORT;
          IPrint("You learn that the origin of your curse<str> lie<str> with "
            "<str> not far from here.", cCount > 1 ? "s" : "",cCount > 1 ? "" : "s", 
            (const char *) cr->Name(NA_A));
          
          do {
            tx = x - 30 + random(60);
            ty = y - 30 + random(60);
            if (!m->InBounds(tx,ty))
              continue;
            if (m->SolidAt(tx,ty))
              continue;
            if (TTER(m->TerrainAt(tx,ty))->HasFlag(TF_WARN))
              continue;
            if (m->FallAt(tx,ty))
              continue;
            cr->PlaceAt(m,tx,ty);
            cr->Initialize(true);
            break;
            }
          while (1);
          
          cr->GainPermStati(TRAP_EVENT,this,SS_MISC,POST(EVICTIM(EV_DEATH)),0,
               FIND("Curse Research"));
          GainPermStati(TRIED,cr,SS_MISC,SK_KNOW_MAGIC+1000);
          return DONE;
        case 'i':
          c=0;
          for (it=FirstInv();it;it=NextInv())
            if (!it->isKnown(KN_MAGIC|KN_NATURE|KN_CURSE))
              { c++; //Candidates[c++] = it->myHandle;
                thisp->MyTerm->LOption(it->Name(0),it->myHandle); }
          if (!c)
            {
              IPrint("You have no unknown items to research.");
              return ABORT;
            }
          h = thisp->MyTerm->LMenu(MENU_2COLS|MENU_ESC|MENU_BORDER, 
            "Research Which Item?");
          if (h == -1)
            return ABORT;
          it = oItem(h);
          
          if (it->HasStati(TRIED,SK_KNOW_MAGIC))
            {
              IPrint("You've already tried, and failed, to "
                "learn about that item.");
              return ABORT;
            }
          
          if (isPlayer())
            if (thisp->SpendHours(1,1) != DONE)   
              return ABORT;
          if (!SkillCheck(SK_KNOW_MAGIC,10+it->ItemLevel(),true))
            {
              IPrint("You fail to turn up any useful "
                "information about the <Obj>.",it);
              it->GainPermStati(TRIED,e.EActor,SS_MISC,SK_KNOW_MAGIC);
              return DONE;
            }
          IPrint("After careful research, you discover that the <Obj> is "
            "in fact <str>!", it,(const char*) it->Name(NA_A|NA_IDENT));
          it->MakeKnown(KN_MAGIC|KN_CURSE|KN_NATURE|KN_PLUS);
          return DONE;
      }
    return ERROR;
  }
  
EvReturn Creature::PickPocket(EventInfo &e)
  {
    int16 DC1, DC2, c, mod, i; Item *it;
    bool success, noticed;
    if (!e.EVictim)
      return ABORT;
    if (!e.EVictim->isCreature())
      {
        IPrint("You must choose a creature to pickpocket.");
        return ABORT;
      }
    if (onPlane() != e.EVictim->onPlane())
      {
        IPrint("You can't even touch that creature!");
        return ABORT;
      }
    if (!isBeside(e.EVictim))
      {
        IPrint("You must be beside a creature to pick-pocket it.");
        return ABORT;
      }
    if (e.EVictim->HasStati(SUMMONED))
      {
        IPrint("You cannot pickpocket a summoned creature.");
        return ABORT;
      }

    DC1 = 15 + e.EVictim->GetAttr(A_SAV_REF) +
               e.EVictim->SumStatiMag(SAVE_BONUS,SN_THEFT) +
               GetStatiMag(TRIED,SK_PICK_POCKET,e.EVictim) +
               (e.EVictim->HasStati(DISTRACTED) ? -10 : 0);
    DC2 = 15 + e.EVictim->SkillLevel(SK_SPOT) +
               GetStatiMag(TRIED,SK_PICK_POCKET,e.EVictim) +
               (e.EVictim->HasStati(DISTRACTED) ? -10 : 0);
               
    if (!e.EVictim->Perceives(e.EActor))
      { DC1 -= 5; DC2 -= 5; }
    
    c = 0;
    for (it=e.EVictim->FirstInv();it;it=e.EVictim->NextInv())
      if ((!(it->IFlags & IF_WORN)) || (e.EVictim->HasStati(DISTRACTED) &&
                                         !it->isType(T_ARMOUR)))
        {
          Candidates[c++] = it->myHandle;
          if (isPlayer())
            {
              mod = max(0,(it->Size()-SZ_SMALL)*3);
              if (it->IFlags & IF_WORN)
                mod += 10;
              if (e.EVictim->HasStati(DISTRACTED))
                mod -= 10;
              // later, by container
              if (it->GetParent() != e.EVictim)
                mod += 5;
              if (it == e.EVictim->InSlot(SL_WEAPON) ||
                  it == e.EVictim->InSlot(SL_READY) ||
                  it == e.EVictim->InSlot(SL_CLOAK) ||
                  it == e.EVictim->InSlot(SL_GAUNTLETS))
                thisp->MyTerm->LOption(Format("%s (DC %d/**)",
                  (const char*)it->Name(0), DC1 + mod), it->myHandle);
              else
                thisp->MyTerm->LOption(Format("%s (DC %d/%d)",
                  (const char*)it->Name(0), DC1 + mod, DC2 + mod), 
                  it->myHandle);
            }
        }
    if (!c)
      {
        IPrint("That creature has nothing you can steal.");
        return ABORT;
      }
    hObj h;
    if (isPlayer())
      {
        
        h = thisp->MyTerm->LMenu(MENU_ESC|MENU_BORDER,"Steal Which Item?");
        if (h == -1)
          return ABORT;
      }
    else
      h = Candidates[random(c)];      
    it = oItem(h);
    
    mod = max(0,(it->Size()-SZ_SMALL)*3);
    if (it->IFlags & IF_WORN)
      mod += 10;
    if (e.EVictim->HasStati(DISTRACTED))
      mod -= 10;
    // later, by container
    if (it->GetParent() != e.EVictim)
      mod += 5;    
  
    Timeout += 40 - SkillLevel(SK_PICK_POCKET);
    success = SkillCheck(SK_PICK_POCKET, DC1 + mod, true);
    noticed = LastSkillCheckResult < DC2 + mod;
    
    for (i=ADJUST;i!=ADJUST_LAST+1;i++)
      {
        e.EVictim->RemoveOnceStati(i,A_SAV);
        e.EVictim->RemoveOnceStati(i,A_SAV_REF);
      }  
    
    if (isLegalPersonTo(e.EActor,e.EVictim))
      e.EActor->AlignedAct(AL_NONLAWFUL,3,"theft");
    if (!e.EVictim->isMType(MA_EVIL) &&
         e.EVictim->isMType(MA_SAPIENT))
      e.EActor->AlignedAct(AL_NONGOOD,1,
        "stealing from non-evil creatures");
      
    if (success)
      {
        it = it->TakeOne();
        IPrint("You snare the <Obj>'s <Obj>!", e.EVictim, it);
        it->Remove(false);
        GainItem(it,false);
      }
    else 
      {
        IPrint("You fail to slip the <Obj> away from the <Obj>.",
          it, e.EVictim);
        IncStatiMag(TRIED,SS_MISC,SK_PICK_POCKET,e.EVictim,+3);
      }
    
    if (noticed)
      {
        IPrint("The <Obj> noticed!", e.EVictim);
        e.EVictim->IPrint("The <Obj> just <Str> you <Obj>!", 
          this, success ? "stole" : "tried to steal", it);
        if (e.EVictim->isHostileTo(this))
          ProvokeAoO(e.EVictim,false);
        else {
          e.EVictim->ts.TurnHostileTo(e.EVictim,this);
          e.EVictim->IDPrint("", "The <Obj> gets angry!", e.EVictim);
          }
      }
    
    return DONE;
      
  }
            
             

EvReturn Creature::Hide(EventInfo &e) {
    uint16 i;
    Creature *c;

    if (HasStati(HIDING)) {
      IPrint("You're already hiding!");
      return ABORT;
      }
    if (HasStati(MANIFEST)) {
      IPrint("Hiding in shadows is simply not compatible with being a radiant beacon "
             "of pure faerie grandeur.");
      return ABORT;
      }
    if (HasStati(MOUNT)) {
      IPrint("You can't hide while something is riding you.");
      return ABORT;
    } 
    if (HasStati(ILLUMINATED)) {
      IPrint("You can't hide while glowing.");
      return ABORT;
      }

    if (!HasAbility(CA_HIDE_IN_PLAIN_SIGHT))
    switch(e.EParam) {
      case HI_SHADOWS:
        /* ww: now that we have cross-class skills, anyone can "try"  */
        if (m->BrightAt(x,y))
          {
            IPrint("It's too bright to hide effectively here.");
            return ABORT;
          }

        MapIterate(m,c,i)
          if (c != this)
            if (c->isCreature())
              if (c->DistFrom(this) < 15)
                if (c->Perceives(this) && !c->HasStati(DISTRACTED,-1,this))
                  if (c->isHostileTo(this) || (c->isPlayer() && !isLedBy(c)))
                    {
                      IPrint("You can't hide when in plain view to hostile creatures.");
                      return ABORT;
                    }
       break;
      case HI_WATER:
        if (!TTER(m->TerrainAt(x,y))->HasFlag(TF_WATER))
          {
            IPrint("There's no water to hide under here.");
            return ABORT;
          }
       break;
      case HI_UNDER:
      case HI_CEILING:
        break;
    }

    Timeout += 15;
    GainPermStati(HIDING,NULL,SS_CLAS,(int16)e.EParam,SkillLevel(SK_HIDE));
    HideVal = random(11)-5;
    return DONE;
  }

EvReturn Creature::Search(EventInfo &e)
  {
    int16 dx, dy, i; Thing *t; Glyph g;
    bool found = false, tfound = false, think = false;
    for(dx = x-5; dx <= x+5; dx++)
      for(dy = y-5; dy <= y+5; dy++)
        if ((dist(x,y,dx,dy) <= 5) && m->InBounds(dx,dy))
          {
            g = T1->GetGlyph(dx,dy);
            g = (g & 0x00FF) | (PINK << 8);
            T1->PutGlyph(dx,dy,g);
          }
    T1->Update();
    if (isPlayer())
      T1->StopWatch(500);
    T1->RefreshMap();
    T1->Update();

    MapIterate(m,t,i)
      if (dist(t->x,t->y,x,y) <= 5) {
        if (t->isType(T_DOOR)) 
          {
            if (((Door*)t)->DoorFlags & DF_SEARCHED)
              { think = true; continue; }
            if (!(((Door*)t)->DoorFlags & DF_SECRET))
              continue;            

            /* Mark it as not secret, so we can check if we *would* see
               it if it weren't secret, then put the flag back to normal. */
            ((Door*)t)->DoorFlags &= ~DF_SECRET;
            if (!Perceives(t))
              continue;
            ((Door*)t)->DoorFlags |= DF_SECRET;

            if (!SkillCheck(SK_SEARCH,15,true,
                  t->GetStatiMag(RETRY_BONUS,SK_SEARCH,this),"retry"))
              { 
                think = true; 
                ((Door*)t)->DoorFlags |= DF_SEARCHED;
                ((Feature*)t)->BoostRetry(SK_SEARCH,this);
                continue;
              }
            ((Door*)t)->DoorFlags &= ~(DF_SECRET|DF_SEARCHED);
            t->SetImage();
            found = true;
          }
        else if (t->isType(T_TRAP)) 
          {
            if (((Trap*)t)->TrapFlags & TS_SEARCHED)
              continue;
            if (((Trap*)t)->TrapFlags & TS_FOUND)
              continue;
            if (isPlayer())
              if (!(m->At(t->x,t->y).Visibility & VI_VISIBLE))
                continue;
            if (!SkillCheck(SK_SEARCH,20 - TEFF(((Trap*)t)->tID)->HasFlag(EF_MUNDANE)*5,
                  true,t->GetStatiMag(RETRY_BONUS,SK_SEARCH,this),"retry"))
              { 
                ((Trap*)t)->TrapFlags |= TS_SEARCHED;
                ((Feature*)t)->BoostRetry(SK_SEARCH,this);
                continue;
              }
            ((Trap*)t)->TrapFlags |= TS_FOUND | TS_SEARCHED;
            t->SetImage();
            tfound = true;
          }
        }



    if (found)
      IDPrint("A secret door!", "The <Obj> finds a secret door!",this);
    else if (tfound)
      IDPrint("A trap!", "The <Obj> finds a trap!",this);
    else if (think)
      IPrint("You think there may be something here, but you can't find it.");
    else
      IPrint("You find nothing of significance.");

    return DONE;
  }

void Creature::DevourMonster(Monster * m)
  {
    int16 i, j, lev, curr;
    static int8 Resists[] = {
      AD_FIRE, AD_COLD, AD_ACID, AD_ELEC, 
      AD_TOXI, AD_NECR, AD_PSYC, AD_MAGC,
      AD_SUNL, AD_SONI,
      AD_DISE, 0 };
    static int8 AttrMTypes[] = {
                        // 7 dragons: avg 24, best 33
      MA_GIANT,         // 6 giants: avg 23, best 29
                        // 14 quadrupeds: avg 20, best 30

                        // 3 hominids: avg 15, best 17
      MA_CAT,           // 8 cats: avg 15, best 19
                        // 6 faeries: avg 15, best 8

      MA_TROLL,         // 2 trolls: avg 23, best 23
                        // 1 quylthulg: avg 22
                        // 7 dragons: avg 20, best 30
                        // 14 quadrupes: avg 17, best 21

      MA_ILLITHID,      // 1 illithid: 19
                        // 7 dragons: avg 16, best 23

      MA_NAGA,          // 5 nagas: avg 17, best 19
                        // 1 illithid: 17
                        // 2 eyes: avg 15, best 16
                        // 10 snakes: avg 14, best 19

      MA_FAERIE,        // 1 illithid: 17
                        // 4 nagas: avg 16, best 18
                        // 6 faeries: avg 16, best 18

      MA_MYTHIC,        // 1 hobbit: 18
                        // 10 hybrids: avg 15, best 20
      0 };
    int CR = m->ChallengeRating(); 
      
    // THEOLOGY HARDCODE
    if (m->isMType(MA_SAPIENT)) {
      if (!isMType(MA_ORC) || !m->isMType(MA_ORC))
        Transgress(FIND("Mara"),7,false,"eating sapient creatures");
      Transgress(FIND("Erich"),4,false,"eating sapient creatures");
      Transgress(FIND("Immotian"),6,false,"eating sapient creatures");
      Transgress(FIND("Xavias"),3,false,"eating sapient creatures");
      Transgress(FIND("Hesani"),3,false,"eating sapient creatures");
      gainFavour(FIND("Khasrach"),10*m->ChallengeRating(),false,true);
      if (!(isMType(MA_ORC) || isMType(MA_REPTILE)))
        AlignedAct(AL_NONLAWFUL,3,"eating sapient creatures");
      }
    gainFavour(FIND("Zurvash"),5*m->ChallengeRating(),false,true);
      
    if (m->isMType(MA_REPTILIAN))
      RemoveStati(STONING);
      
    if (!HasAbility(CA_DEVOURING))
      return;
    
      
    if (m->isMType(MA_UNDEAD)) {
      IPrint("You gain nothing from devouring undead flesh.");
      return; 
    } 

    for(i=0;Resists[i];i++)
      {
        lev = m->ResistLevel(Resists[i]);
        // ResistLevel tends to give values like 7+CR/2
        if (lev == -1) // immunity
                lev = max(CR,0) + 3;
        else if (lev > 0)
                lev = max(CR,0) + 1;
        else 
                continue; 
        
        curr = 0;
        StatiIterNature(this,RESIST)
            if (S->Source == SS_RACE)
              if (S->Val == Resists[i])
                curr = S->Mag;
        StatiIterEnd(this)
        if (TMON(tmID)->Res & BIT(Resists[i]))
          curr = max(curr, 7 + (ChallengeRating() * 2) / 3);
        if (TMON(tmID)->Imm & BIT(Resists[i]))
          curr = -1; 

        if (curr < lev && curr > -1) {
          // ww: the old logic here never happened in real life, so let's
          // just simplify it to +1 always
          IPrint(Format("You gained <11>%s<7> resistance from %s flesh.",
                                  Lookup(DTypeNames,Resists[i]),
                                  (const char*)m->Name(NA_THE|NA_POSS)));
          StatiIterNature(this,RESIST)
              if (S->Source == SS_RACE)
                if (S->Val == Resists[i])
                  { S->Mag++ ; StatiIterBreakout(this,goto Changed) }
          StatiIterEnd(this)
          GainPermStati(RESIST,NULL,SS_RACE,Resists[i],1,0);
          Changed:;
          }
      }

    CalcValues();
    for (i=0;AttrMTypes[i];i++)
      if (m->isMType(AttrMTypes[i]))
        if (m->Attr[i] > AttrAdj[i][BONUS_BASE] +
                         SumStatiMag(ADJUST_INH,i) &&
                         AttrAdj[i][BONUS_BASE] != 0)
          {
            if (SumStatiMag(ADJUST_INH,i) >= 
                            5 + AbilityLevel(CA_INHERANT_POTENTIAL))
              continue;
            GainInherentBonus(i,+1,true);
          }

    /* we can now figure out the dragon age category */
    if (m->isMType(MA_DRAGON)) {
      // hatchling power is +1, etc. -- 10x because by the time you are
      // fighting vaguely adult dragons, you naturally have around 100 mana
      // anyway, so getting 2 or 3 is not worth it 
      lev = 10 * m->GetPower(0); 

      curr = 0;
      StatiIterNature(this,ADJUST_INH)
          if (S->Source == SS_RACE)
            if (S->Val == A_MAN)
              curr = S->Mag*5;
      StatiIterEnd(this)

      if (curr < lev) {
        IPrint(Format("You feel magical energies flow through you as you devour %s flesh.", (const char*)m->Name(NA_THE|NA_POSS)));
        StatiIterNature(this,ADJUST_INH)
            if (S->Source == SS_RACE)
              if (S->Val == A_MAN) { 
                S->Mag++;
                StatiIterBreakout(this,goto ChangedMana) 
              }
        StatiIterEnd(this)
        GainPermStati(ADJUST_INH,this,SS_RACE,A_MAN,1,0);
ChangedMana:;
        CalcValues();
      }
    } 

    // ww: there was a bug (?) here where the player's CR was used
    // instead of the corpse's
    i = CR*50;
    for (j = ChallengeRating(); j >= CR; j--)
      i /= 2;
    GainXP(i);
    Corpse * c = (Corpse *)m->GetStatiObj(MY_CORPSE_IS); 
    if (c) {
      c->Remove(true); 
      m->RemoveStati(MY_CORPSE_IS);
    } 
} 

void Creature::Devour(Corpse * c)
  {
    // ww: now that we're taking Templates into account for corpses, it's
    // easier just to make a fake monster
    Monster *m = new Monster(c->mID);
    StatiIterNature(this,TEMPLATE)
      m->AddTemplate(S->eID);
    StatiIterEnd(this)
    // ww: not necessary, plus things with fields get annoyed when they are
    // not on the map: TMON(m->tmID)->PEvent(EV_BIRTH,m,m->tmID);
    // m->Initialize(false);
    m->CalcValues();
    
    DevourMonster(m); 

    delete m; 
  }

/* Just a note about this: I'm probably NOT going to implement magic
   item creation as it appears in the SRD, even though it wouldn't be
   too hard to do using this function as a base. The rationate behind
   this choice is that game balance in a single-player Roguelike is
   very different from game balance in a party-based tabletop RPG.
   There are a few distinst points here:
   1. In roguelikes, replayability is key. If PCs can manufacture items,
      it really streamlines the path they need to follow to get the
      "best possible kit", and makes every game much more similar.
      The emphasis should be on FINDING magic items, not MAKING them,
      because the former is more interesting, and more different on
      every replay.
   2. I want the abilities of dwarves and bards to be truly unique
      in the game, to help differentiate those character types. If
      it basically amounts to a bonus feat, the ability feels a lot
      less special. Also, limiting item creation to one or two 
      character types makes games with those types play more uniquely
      than games with other character types.
   3. The SRD paradigm of item creation is centered around wizards
      and clerics making items, but it's assumed that PC casters will
      make some for their PC warrior-type party-mates. In Incursion,
      SRD item creation would mean that wizards and priests would end
      up with the best magic swords and armour, and the fighter types
      would be left out cold. I don't want that.
   ----
     This said, there are some exceptions to this. Definitely, I want
   Brew Potion and Scribe Scroll (and /maybe/ Craft Wand) available,
   because those are just useful to casters. Brew Potion may be based
   on the Alchemy skill instead of caster level, and may require various
   ingredients, etc., to better fit a roguelike paradigm. I'm considering
   scrolls being written with monster blood, too. I may consider some other
   magic item creation system in the future, but it would have to A) be
   decidedly inferior to the dwarves and bards abilities, or at least not
   directly intersect it, B) not favour the spellcasters so immensely, C)
   be less predictable and require more effort (components, quests, etc.)
   and creativity then the SRD version, which just drives the player into
   scumming for gold and XP. 
   
   Addenda: It would be nice to do some kind of Fletchery feat that lets
     you duplicate existing magic arrows. So if you find 12 +3 shocking
     arrows of accuracy in the dungeon, and then use up nine of them, you
     can raise the it->Quantity of the remaining 3 back up to 50 at a cost
     in experience.
    
   We do ALCHEMY using CraftItem the EXACT SAME WAY we do Brew Potion, 
     Craft (create item), etc.
     
*/
EvReturn Character::CraftItem(int16 abil) {
    int8 i;
    int16 new_lev, c, max_lev, qual, old_lev, 
      repairDC, craftDC, itemType, itemSource, acqVal,
      useSkill, minSkill, hours, quan; int32 gpCost, XPCost;
    Feature *ft; hObj hItem; char ch; Item *it, *it2;
    
    int16 XPCostTable[] = {
      0, 80, 160, 320, 540, 720, 1280, 1670,
      2000, 2880, 3920, 5120, 6480, 8165,
      12050, 15400, 17200, 19400, 21300,
      23750, 25000 };
      
      
    bool improveOnly = false,
         weaponsOnly = false,
         foundForge  = false,
         isBardic    = false,
         addQuality  = false,
         workSlowly  = false;
    /* For now, this is players-only. Later, players may be able to
       get friendly NPCs or party members to do these things, hence
       it being in Character. */
    
    for (ft=m->FFeatureAt(x,y);ft;ft=m->NFeatureAt(x,y))
      if (ft->fID == FIND("forge"))
        foundForge = true;

    if (HasStati(INNATE_KIT,SK_CRAFT))
      foundForge = true;

    if (!isPlayer())
      return ABORT;
      
    switch (abil)
      {       
        case SK_ALCHEMY + SKILL_VAL:
          itemType   = T_POTION;
          itemSource = AI_ALCHEMY;
          acqVal     = ACQ_KNOWN;
          useSkill   = SK_ALCHEMY;
          max_lev = SkillLevel(SK_ALCHEMY);
          goto Create;
        case SK_POISON_USE + SKILL_VAL:
          itemType   = T_POTION;
          itemSource = AI_POISON;
          acqVal     = ACQ_KNOWN;
          if (SkillLevel(SK_POISON_USE) <= SkillLevel(SK_ALCHEMY))
            useSkill   = SK_POISON_USE;
          else
            useSkill   = SK_ALCHEMY;
          max_lev = min(SkillLevel(SK_POISON_USE),SkillLevel(SK_ALCHEMY));
          goto Create;
        case FT_SCRIBE_SCROLL + FEAT_VAL:
          itemType   = T_SCROLL;
          itemSource = AI_SCROLL;
          acqVal     = ACQ_KNOWN;
          useSkill   = 0;
          max_lev = SkillLevel(SK_KNOW_MAGIC);
          goto Create;
        case FT_BREW_POTION + FEAT_VAL:
          itemType   = T_POTION;
          itemSource = AI_POTION;
          acqVal     = ACQ_KNOWN;
          useSkill   = SK_ALCHEMY;
          max_lev = SkillLevel(SK_ALCHEMY);
          goto Create;        
        case SK_CRAFT + SKILL_VAL:
          if (SkillLevel(SK_CRAFT) >= 12)
            ch = ChoicePrompt("Repair item, temper masterwork or create new item?", "rtc", 'r');
          else
            ch = ChoicePrompt("Repair item or create new item?", "rc", 'r');
          if (ch == -1)
            return ABORT;
          max_lev = SkillLevel(SK_CRAFT);
          if (ch == 'r')
            goto Repair;
          if (ch == 't')
            goto Temper;
          if (ch == 'c')
            {
              itemType   = -1;
              itemSource = -1;
              acqVal     = ACQ_CRAFT;
              useSkill   = SK_CRAFT;
            }
          goto Create;
        case CA_WEAPONCRAFT + ABIL_VAL:
          if (!foundForge)
            {
              IPrint("You need to find a forge to create weapons or armour.");
              return ABORT;
            }
          do
            ch = ChoicePrompt("Add quality or improve plus?",
                                       "qp",'p');
          while (ch == -1);
          addQuality = (ch == 'q');
          improveOnly = false;
          weaponsOnly = true;
          max_lev     = SkillLevel(SK_CRAFT);
          goto Augment;
        case CA_STORYCRAFT + ABIL_VAL:     
          if (aStoryPluses >= tStoryPluses)
            {
              IPrint("You cannot augment any more items until you advance "
                     "farther as a bard.");
              return ABORT;
            }
          improveOnly = true;
          weaponsOnly = false;
          isBardic    = true;
          max_lev     = AbilityLevel(CA_STORYCRAFT) + Mod(A_CHA) + 1;
          goto Augment;
      }
    
    Error("Unimplemented ability sent to CraftItem!");
    return ABORT;
    
    
    Temper:
    if (!foundForge)
      {
        IPrint("You need to find a forge to temper blades.");
        return ABORT;
      }
    for (c=0,it=FirstInv();it;it=NextInv())
      {
        if (!it->isType(T_WEAPON))
          continue;
        if (!it->isGroup(WG_LBLADES|WG_SBLADES|WG_DAGGERS))
          continue;
        if (!it->isMetallic())
          continue;
        Candidates[c++] = it->myHandle;
      }
      
    if (!c)
      {
        IPrint("You have no blades to temper.");
        return ABORT;
      }
    for(i=0;i!=c;i++) {
      if (oItem(Candidates[i])->isKnown(KN_PLUS))
        thisp->MyTerm->LOption(Format("%s (DC %d)",
          (const char*) oThing(Candidates[i])->Name(NA_A|NA_MECH), 
          15 + oItem(Candidates[i])->ItemLevel()),
          Candidates[i] );
      else
        thisp->MyTerm->LOption(Format("%s",(const char*)
          oThing(Candidates[i])->Name(NA_A|NA_MECH)),
          Candidates[i] );
      }
    
    hItem = thisp->MyTerm->LMenu(MENU_ESC|MENU_BORDER,
      "Temper which item?", WIN_MENUBOX);
    if (hItem == -1)
      return ABORT;
    it = oItem(hItem);

    it->RemoveStati(MASTERWORK);
        
    if (thisp->SpendHours(6,6) != DONE)
      return ABORT;

    if (!SkillCheck(SK_CRAFT,15 + it->ItemLevel(),true))
      {
        IPrint("You fail to properly temper the <Obj>.",it);
        if (LastSkillCheckResult < 10 + it->ItemLevel())
          {
            int16 dmg = Dice::Roll(1,8);
            if (it->GetHP() <= dmg)
              {
                IPrint("Your failed tempering attempt destroys the <Obj>!",it);
                it->Remove(true);
              }
            else
              {
                it->MendHP(-dmg);
                IPrint("Your failed tempering attempt damages the <Obj>!",it);
              }
          }
        return DONE;
      }
      
    Exercise(A_STR,Dice::Roll(1,12),ESTR_FORGE,30);
    Exercise(A_INT,Dice::Roll(1,12),EINT_CREATION,40);
    
    it->GainPermStati(MASTERWORK,NULL,SS_MISC,(SkillLevel(SK_CRAFT)-7)/5,
                                               LastSkillCheckResult*3);
    IPrint("You temper the <Obj> until it gleams razor-sharp!", it);
    return DONE;
    
    Augment:
    for (c=0,it=FirstInv();it;it=NextInv())
      {
        if (improveOnly && !it->GetInherentPlus())
          continue;
        if (!it->isKnown(KN_MAGIC|KN_PLUS|KN_BLESS))
          continue;
        if (it->isCursed())
          continue;
        if (it->isType(T_POTION) || it->isType(T_SCROLL))
          continue;
        if (it->GetInherentPlus() >= 5)
          continue;
        if (weaponsOnly && !(it->isType(T_WEAPON) || 
                             it->isType(T_ARMOUR) ||
                             it->isType(T_SHIELD)))
          continue;
        
          
        Candidates[c++] = it->myHandle;
      }
      
    if (!c)
      {
        IPrint("You have no suitable items to augment.");
        return ABORT;
      }
    for(i=0;i!=c;i++)
      thisp->MyTerm->LOption(oThing(Candidates[i])->
        Name(NA_A|NA_MECH), Candidates[i] );
    
    hItem = thisp->MyTerm->LMenu(MENU_ESC|MENU_BORDER,
      "Augment which item?", WIN_MENUBOX);
    if (hItem == -1)
      return ABORT;
    it = oItem(hItem);
    
      
    if (addQuality)
      {
        TextVal *desc, *pre, *post;
        bool isWeapon = false;
        if (it->isType(T_SHIELD) || it->isType(T_ARMOUR))
          { desc = AQualityDescs; pre = APreQualNames; post = APostQualNames; }
        else if (it->isType(T_WEAPON))
          { desc = QualityDescs; pre = PreQualNames; 
            post = PostQualNames; isWeapon = true; }
        else {
          IPrint("That item cannot have qualities granted to it.");
          return ABORT;
          }
          
        for (i=0;i!=WQ_LAST;i++)
          {
            if (!LookupOnly(desc,i))
              continue;
            if (!it->QualityOK(i))
              continue;
            
            if (isWeapon) 
              {
                if (i == WQ_HOLY && !isMType(MA_GOOD))
                  continue;
                if (i == WQ_UNHOLY && !isMType(MA_EVIL))
                  continue;
                if (i == WQ_CHAOTIC && !isMType(MA_CHAOTIC))
                  continue;
                if (i == WQ_LAWFUL && !isMType(MA_LAWFUL))
                  continue;
                if (i == WQ_BALANCE)
                  if (isMType(MA_LAWFUL) || isMType(MA_CHAOTIC) ||
                      isMType(MA_GOOD) || isMType(MA_EVIL) )
                    continue;
                    
                /* Let's not have low-level characters running around
                   with +1 vorpal long swords. */
                if (i == WQ_VORPAL && TotalLevel() < 15)
                  continue;
              }
            
            thisp->MyTerm->LOption(LookupOnly(pre,i) ? Lookup(pre,i) :
              (LookupOnly(post,i) ? Lookup(post,i) : "???"),i,
              Lookup(desc,i));
          }
            
        qual = (int16)thisp->MyTerm->LMenu(MENU_DESC|MENU_BORDER|MENU_ESC|MENU_3COLS,
          "Pick a quality to imbue:", WIN_MENUBOX);
        if (qual == -1)
          return ABORT;
      }    
    else
      qual = 0;
            
      
    old_lev = it->ItemLevel();
    
    it->Quantity++;
    it2 = it->TakeOne();
    if (addQuality)
        it2->AddQuality((int8)qual);
    else
        it2->SetInherentPlus(it->GetInherentPlus() + 1);
    
    if (it2->ItemLevel() > 20) {
        IPrint("That enchantment is beyond the ability of mortal magics.");
        delete it2;
        return ABORT;
    }
    XPCost = XPCostTable[it2->ItemLevel()] - XPCostTable[it->ItemLevel()];
    new_lev = it2->ItemLevel();
    ASSERT(XPCost > 0);
    delete it2;
    
    if (new_lev > max_lev)
      {
        IPrint(Format("That enchantment is beyond your current skill."
          " (You can create items of up to level %d, and that would be "
          "a level %d item.)",max_lev,new_lev));
        return ABORT;
      }
      
    gpCost = XPCost * 25;
    
    if (isBardic)
      {
        aStoryPluses++;
        IPrint("You have a sense of mythic grandeur about your <Obj>.",it);      
      }
    else
      {
        if ((uint32)XPCost > (XP - XP_Drained))
          {
            IPrint(Format("You don't have enough available experience; "
              "you have %d and need %d.",(XP - XP_Drained),XPCost));
            return ABORT;
          }
          
        
        if (HasAbility(CA_MASTER_ARTISAN))
          gpCost = (gpCost * max(4,20 - AbilityLevel(CA_MASTER_ARTISAN))) / 20;
          
        if (getTotalMoney()/100 < gpCost)
          {
            IPrint(Format("That would cost %d gp in materials, but you only have %d gp.",
              gpCost, getTotalMoney()/100));
            return ABORT;
          }      
          
        hours = min(10,2 + (new_lev - old_lev)*2);
        if (HasFeat(FT_ARTIFICER))
          XPCost = (XPCost * 2) / 3;
        if (!yn(Format("That will cost %d XP, %d gp and take %d hours. Proceed?",
                  XPCost, gpCost, hours)))
          return ABORT;
        if (thisp->SpendHours(hours,hours) != DONE)
          return ABORT;
        LoseXP(XPCost);
        LoseMoneyTo(gpCost,NULL);
        thisp->MyTerm->ShowStatus();
        IPrint("You forge magic into the <Obj>.",it);
        Exercise(A_STR,Dice::Roll(1,12)+4,ESTR_FORGE,70);
        Exercise(A_INT,Dice::Roll(1,12)+4,EINT_CREATION,70);
        Exercise(A_WIS,Dice::Roll(1,12)+4,EWIS_CREATION,70);
      }
    
    if (addQuality)
      {
        it->AddQuality((int8)qual);
      }
    else
      {
        it->SetInherentPlus(
          it->GetInherentPlus() + 1);
      }
      
    return DONE;
    
    Create:
    
    /*
    if (itemType != -1) 
      for (i=0,iID=0;DungeonItems[i].Type;i++)
        if (DungeonItems[i].Type == itemType)
          if (DungeonItems[i].Prototype)
            {
              iID = FIND(DungeonItems[i].Prototype);
              if (!iID)
                {
                  Error("Item prototype from DungeonItems[] not found!");
                  return ABORT;
                }
              break;
            }
    if (iID)
      it = Item::Create(iID)
    else*/
    
    it = (Item*) thisp->MyTerm->AcquisitionPrompt((int8)acqVal,0,(int8)max_lev,(int8)itemSource);
    if (!it)
      return ABORT;
    
    craftDC = 10;
    
    if (acqVal != ACQ_CRAFT)
      if (it->eID && TEFF(it->eID)->HasFlag(EF_NEEDS_PLUS))
        it->SetInherentPlus((int8)MaxItemPlus(max_lev,it->eID));
    
    if (acqVal == ACQ_CRAFT) {
      minSkill = 0;
      minSkill = max(minSkill,(int16)TITEM(it->iID)->GetConst(MIN_CRAFT_LEVEL));
      if (it->eID)
        minSkill = max(minSkill,(int16)TITEM(it->eID)->GetConst(MIN_CRAFT_LEVEL));   
      if (it->isMetallic()) {
        minSkill = max(minSkill, 10);
        if (!foundForge)
          {
            delete it;
            IPrint("You need a forge to create metallic items.");
            return ABORT;
          }
        }
      if (minSkill > SkillLevel(SK_CRAFT))
        {
          delete it;
          IPrint(Format("You need Craft %+d to make that item, but have only Craft %+d.",
                   minSkill, SkillLevel(SK_CRAFT)));
          return ABORT;
        }
      craftDC = max(craftDC, minSkill+7);
      }
    
    quan = 1;
    if (it->isType(T_POTION) || it->isType(T_SCROLL) || 
          it->isType(T_MISSILE) || itemSource == AI_POISON ||
          itemSource == AI_ALCHEMY)
      {
        
        quan = atoi(thisp->MyTerm->StringPrompt(PINK,"Enter Quantity:"));
        if (quan <= 0)
          { it->Remove(true); return ABORT; }
        if (it->HasStati(POISONED))
          it->SetStatiMag(POISONED,-1,NULL,quan);
        else
          it->Quantity = quan;
      }
    
    
    /* There should be a LOT more detail here later. We should
       base the DC off the gold cost, and we should require a
       skill kit and raw materials... etc. For now, let's just
       get it working. Also, provisions for making, say, 10
       potions of healing at the same time. */
    
    if (itemSource == AI_SCROLL)
      craftDC += (TEFF(it->eID)->Level-1)*2;
    else if (itemSource == AI_POISON)
      craftDC += TEFF(it->GetStatiEID(POISONED))->Level;
    else
      craftDC += it->ItemLevel();
    
    XPCost = XPCostTable[min(20,max(it->ItemLevel(),1))];
    rID icList[10];
    if (it->eID && TEFF(it->eID)->GetList(ITEM_COST,icList,9))
      XPCost = icList[max(0,it->GetInherentPlus()-1)] / 25;
    
    if (itemType == T_MISSILE)
      XPCost /= 20;
    if (itemType == T_SCROLL)
      XPCost /= 10;
    if (itemSource == AI_POTION)
      XPCost /= 5;
      
    XPCost *= quan;
    
    gpCost = XPCost * 8; /* Was 25 */


    if (itemSource == AI_ALCHEMY ||
        itemSource == AI_POISON ||
        acqVal     == ACQ_CRAFT)
      { XPCost = 0; gpCost /= 5; }
    
    if (acqVal == ACQ_CRAFT)
      gpCost = it->getShopCost(NULL,NULL) * quan;
    
    if (itemType == T_MISSILE)
      hours = 2 + quan / 5;
    else
      hours = 2 + quan;
    
    if (hours > 11 + Mod(A_CON))
      {
        IPrint("There isn't enough time in a full day to craft "
          "that many items.");
        { it->Remove(true); return ABORT; }
      }
    
    if (HasFeat(FT_ARTIFICER))
      XPCost = (XPCost * 2) / 3;
    
    if ((uint32)XPCost > (XP - XP_Drained))
      {
        IPrint(Format("You don't have enough available experience; "
          "you have %d and need %d.",(XP - XP_Drained),XPCost));
        return ABORT;
      }

    if (HasAbility(CA_MASTER_ARTISAN))
      gpCost = (gpCost * max(4,20 - AbilityLevel(CA_MASTER_ARTISAN))) / 20;
      
    if (getTotalMoney()/100 < gpCost)
      {
        IPrint(Format("That would cost %d gp in materials, but you only have %d gp.",
          gpCost, getTotalMoney()/100));
        return ABORT;
      }      
       
    if (useSkill > 0 && !XPCost) {
      if (!yn(Format("That takes %d gp, %d hours and is DC %d. Proceed?",
                        gpCost, hours, craftDC)))
        { it->Remove(true); return ABORT; }   
      }    
    else if (useSkill > 0)
      if (!yn(Format("That takes %d XP, %d gp, %d hours and is DC %d. Proceed?",
                        XPCost, gpCost, hours, craftDC)))
        { it->Remove(true); return ABORT; }
    if (useSkill <= 0)
      if (!yn(Format("That takes %d XP, %d gp and %d hours. Proceed?",
                        XPCost, gpCost, hours)))
        { it->Remove(true); return ABORT; }
    
    if (thisp->SpendHours(hours,hours) != DONE)
      { it->Remove(true); return ABORT; }
    
    // lose half in either case
    LoseXP(XPCost / 2);
    LoseMoneyTo(gpCost / 2,NULL);
    
    if (useSkill > 0)
      if (!SkillCheck(useSkill,craftDC,true))
        {
          IPrint("You fail to craft the <Obj> properly.",it);
          it->Remove(true); 
          return ABORT;
        }
        
    LoseXP((XPCost+1)/2);
    LoseMoneyTo((gpCost+1)/2,NULL);
        
    if (itemSource == AI_ALCHEMY && SkillLevel(SK_ALCHEMY) > 10)
      it->GainPermStati(ALCHEMY_LEV,NULL,SS_MISC,0,SkillLevel(SK_ALCHEMY));
        
    it->MakeKnown(0xFF);
    GainItem(it,false);
    
    if (it->isMetallic())
      Exercise(A_STR,Dice::Roll(1,12)+4,ESTR_FORGE,70);
    Exercise(A_INT,Dice::Roll(1,10)+2,EINT_CREATION,50);
    
    return DONE;
    
    Repair:
    
    for (c=0,it=FirstInv();it;it=NextInv())
      if (it->GetHP() < it->MaxHP())
        {
          c++;
          thisp->MyTerm->LOption(Format("%s (%d/%d)",
                   (const char*)it->Name(NA_A|NA_MECH),
                   it->GetHP(), it->MaxHP()),it->myHandle);
        }
    if (!c)
      {
        IPrint("You have no damaged items.");
        return ABORT;
      }
        
    hItem = thisp->MyTerm->LMenu(MENU_ESC|MENU_BORDER,
      "Repair which item?", WIN_MENUBOX); 
    if (hItem == -1)
      return ABORT;
    it = oItem(hItem);
    
    if (it->isMetallic())
      if (!foundForge)
        {
          IPrint("You need to find a forge to repair a metallic item.");
          return ABORT;
        }
    
    // base 10
    repairDC = 10;
    
    // Plus the item level for magical items
    if (it->isMagic())
      repairDC += it->ItemLevel();
      
    // +1 for every 10% of its HP the item is down
    repairDC += 10 - ((it->GetHP()*10)/it->MaxHP());
    
    // retry penalties -- +2 per failed check
    repairDC += it->GetStatiMag(TRIED,SK_CRAFT);
    
    gpCost = it->getShopCost(NULL,NULL);
    gpCost *= ((it->GetHP()*10)/it->MaxHP());
    gpCost /= 30;
    
    if (HasAbility(CA_MASTER_ARTISAN))
      gpCost = (gpCost * max(4,20 - AbilityLevel(CA_MASTER_ARTISAN))) / 20;
          
    if (getTotalMoney()/100 < gpCost)
      {
        IPrint(Format("That would cost %d gp in materials, but you only have %d gp.",
          gpCost, getTotalMoney()/100));
        return ABORT;
      }      
        
    
    ch = thisp->MyTerm->ChoicePrompt(Format(
      "Craft %+d, DC %d. Proceed, work slowly or cancel?",
      SkillLevel(SK_CRAFT), repairDC),"psc",CYAN,SKYBLUE);
    
    if (ch == 'c' || ch == -1)
      return ABORT;
    if (ch == 'p')
      workSlowly = false;
    if (ch == 's')
      workSlowly = true;
      
    if (thisp->SpendHours(workSlowly ? 8 : 2, 
                  workSlowly ? 8 : 2) != DONE)
      return ABORT;
      
    LoseMoneyTo(gpCost, NULL);
      
    if (SkillCheck(SK_CRAFT,repairDC,true, workSlowly ? 4 : 0,
                     workSlowly ? "patience" : NULL))
      {
        it->MendHP(Dice::Roll(2,8,SkillLevel(SK_CRAFT)/3));
        it->RemoveStati(TRIED,-1,SK_CRAFT);
        IPrint(Format("You %srepair your damaged %s. (%d/%d).",
          it->GetHP() != it->MaxHP() ? "partially " : "",
          (const char*) it->Name(0), it->GetHP(), it->MaxHP()));
        if (it->isMetallic())
          Exercise(A_STR,Dice::Roll(1,12)+4,ESTR_FORGE,30);
        Exercise(A_INT,Dice::Roll(1,6),EINT_CREATION,40);
        return DONE;
      }
      
    it->IncStatiMag(TRIED,SS_MISC,SK_CRAFT,NULL,+2);
    
    int16 dmg = random(8)+1;
    
    if (LastSkillCheckResult < (repairDC - 5))
      {
        if (it->GetHP() <= dmg)
          {
            IPrint("Your failed repair attempt destroys the <Obj>!",it);
            it->Remove(true);
          }
        else
          {
            it->MendHP(-dmg);
            IPrint("Your failed repair attempt aggravates the damage!");
          }
        return DONE;
      }
      
    IPrint("You fail to repair the damage.");
    return DONE;
        
  }

void Character::RecalcCraftFormulas()
  {
    int16 CraftSkills[] = { SK_ALCHEMY,       AI_ALCHEMY, SK_ALCHEMY,    1,
                            FT_BREW_POTION,   AI_POTION,  SK_KNOW_MAGIC, 2,
                            FT_SCRIBE_SCROLL, AI_SCROLL,  SK_KNOW_MAGIC, 4,
                            0, 0, 0, 0};
    int16 sk, sklev, src, last, cKnown, i, j, k, c, prereq, tKnown;
    TEffect *te; bool outOfFormulas; time_t t; Player *p;
    
    if (p = theGame->GetPlayer(0))
      srand(p->formulaSeed);
    
    outOfFormulas = false;
    
    last = theGame->LastSpell();
    
    for (i=0;CraftSkills[i];i+=4)
      {
        sk = CraftSkills[i+2];
        src = CraftSkills[i+1];
        prereq = CraftSkills[i];
        sklev = SkillLevel(sk); 
        
        if (prereq < SK_LAST)
          {
            if (!HasSkill(prereq))
              continue;
          }
        else
          {
            if (!HasFeat(sk))
              continue;
          }
          
          
          
        /* Remove magical bonuses -- wearing a +10 alchemy
           ring shouldn't get you 10 new formulas! */
        StatiIterNature(this,SKILL_BONUS)
          if (S->Val == sk && (S->Source == 
                SS_ENCH || S->Duration != 0))
            sklev -= S->Mag;
        StatiIterEnd(this)
          
        tKnown = (sklev - 3) * CraftSkills[i+3];
        
        /* Count Known Formulas */
        cKnown = 0;
        for (j=0;j!=last;j++)
          {
            te = TEFF(theGame->SpellID(j));
            if (te->HasSource((int8)src)) {
              /* If you know the actual spell as a spellcaster,
                 you automatically are able to scribe a scroll
                 of it, and it should not count as one of your
                 known scroll formulas. If you had it as one of
                 your known scroll formulas and then learned it
                 as a spell, remove the formula flag. */
              if (Spells[j] & SP_KNOWN)
                Spells[j] &= ~SP_CREATE;
              if (Spells[j] & SP_CREATE)
                cKnown++;
              }
          }
          
        if (cKnown < tKnown)
          {
            for (k=cKnown;k!=tKnown;k++)
              {
                c = 0;
                for (j=0;j!=last;j++)
                  {
                    te = TEFF(theGame->SpellID(j));
                    if (te->HasSource((int8)src))
                      if (!(Spells[j] & (SP_CREATE|SP_KNOWN)))
                        Candidates[c++] = j;
                  }
                if (!c)
                  {
                    /* The game only has X alchemical items, but you
                       have X+Y total formula slots. So for now, you
                       just get gypped. Sorry. */ 
                    outOfFormulas = true;
                    break;
                  }
                j = random(c);
                Spells[Candidates[j]] |= SP_CREATE;
              }
            
          }
      }
    //if (outOfFormulas)
    //  IPrint("Out of formulas!");
    time(&t);
    srand((unsigned long)t);
  }  

int16 Creature::getFavEnemyBonus(Creature *cr)
  {
    int16 bonus = 0, v;
   
    StatiIterNature(this,FAV_ENEMY)  
      if (cr->isMType(S->Val))
        {
          v = S->Mag;
          bonus = max(bonus,CalcFEBonus(S->Val,S->Mag));
        }
    StatiIterEnd(this)
    return bonus;
  }
  
void Creature::wieldSoulblade(Item *blade)
  {
    int16 plus;
    rID sbID = FIND("Soulblade");
    
    if (!blade->isMagic())
      return;
    if (blade->eID)
      return;
    if (blade->isCursed())
      return;
      
    plus = blade->GetInherentPlus();
  
    if (!HasFeat(FT_GREATER_SOULBLADE))  
      {
        if (plus + 1 <= Mod(A_CHA))
          blade->GainPermStati(BOOST_PLUS,this,SS_ENCH,0,plus+1,sbID);
        return;
      }
      
  }


void Creature::Reveal(bool intentional)
  {
    Status *s; int8 form;
    while (s = GetStati(HIDING)) {
      form = s->Val;
      RemoveStati(HIDING);
      switch(form) {
        case HI_SHADOWS:
          ShadowsMessage:
          IDPrint(intentional ? NULL : "<14>You are exposed!<7>",
            "From the shadows, an <Obj>!",this);
         break;
        case HI_WATER:
          IDPrint(intentional ? NULL : "You rise to the surface!",
            "A <Obj> rises to the surface!",this);
         break;
        case HI_UNDER:
          if (!m->FItemAt(x,y))
            goto ShadowsMessage;
          IDPrint(intentional ? NULL : "<14>You are exposed!<7>",
            "There's an <Obj> under <str> <Obj>!",this,
            m->FItemAt(x,y)->isPlural() ? "those" : "that",
            m->FItemAt(x,y));
         break;
        case HI_CEILING:
          IDPrint(intentional ? NULL : "You drop down!",
            "An <Obj> drops from above!",this);
         break;
        default:
          Error(Format("Strange form of hiding %d!",form));
        }
      }
  }

void Creature::Awaken(int32 st)
  {
    if (Flags & F_DELETE)
      return;

  if (HasStati(SLEEPING,(int16)st)) {
    if (HasStati(SLEEPING,SLEEP_PSYCH) ||
        HasStati(SLEEPING,SLEEP_DEEP))
      return;
    RemoveStati(SLEEPING);
    IDPrint("You are shocked awake!",
      (const char*)Format("The <Obj> wakes up!",st),this);
      Timeout += 30;
      GainTempStati(EXPOSED,NULL,4,SS_MISC);
      }
  }

EvReturn Creature::Ascend(EventInfo &e)
  {
    Feature *ft;

    if (HasStati(STUCK) || HasStati(GRAPPLED) || HasStati(ENGULFED) ||
        HasStati(GRAPPLING) || HasStati(GRABBED))
      {
        IPrint("You can't ascend while stuck, grappling or engulfed.");
        return ABORT;
      }

    for (ft=m->FFeatureAt(x,y);ft;ft=m->NFeatureAt(x,y))
      if (ft->Type == T_PORTAL)
        if (((Portal*)ft)->EnterDir(UP))
          {
            if (Throw(EV_ENTER,this,ft) == DONE)
              return DONE;
            return ABORT;
          }
    
    if (!m->At(x,y).isSkylight && SkillLevel(SK_CLIMB) >= 20)
      if (yn("Climb onto the ceiling?"))
        {
          int16 i;
          if (m->SolidAt(x,y))
            {
              IPrint("You're immersed in rock -- there's no ceiling here!");
              return ABORT;
            }
          for (i=0;i!=8;i++)
            if (m->SolidAt(x+DirX[i],y+DirY[i]))
              goto FoundWall;
              
          IPrint("You need to be next to a wall to climb up.");
          return ABORT;
          
          FoundWall:
          if (!LoseFatigue(1,true))
            return ABORT;
          IDPrint("You scale up the wall and onto the ceiling.",
            "The <Obj> scales the wall and climbs along the ceiling.", this);
          GainPermStati(ELEVATED,NULL,SS_MISC,ELEV_CEILING,0); 
          return DONE;
        }
    
    if (m->At(x,y).isSkylight && m->Depth > 1)
      {
        Map *m2 = theGame->GetDungeonMap(
          m->dID,m->Depth-1,oPlayer(m->pl[0]),NULL);
        int16 r, tx, ty, ox, oy;
        Item *Rope;

        if (m->TreeAt(x,y)) {
          char ch = ChoicePrompt("Climb tree or ascend level?","tl",'t');
          if (ch == -1)
            return ABORT;
          if (ch == 't')
            goto ClimbTree;
          }
           
        if (HasStati(LEVITATION))
          {
            IPrint("You float upwards.");
            MoveDepth(m->Depth-1,true);
            Timeout += 30;
            return DONE;
          }
          
        if (SkillLevel(SK_CLIMB) < 12)
          {
            IPrint("You need a Climb skill of at least +12 to ascend "
              "dungeon levels in this manner.");
            return ABORT;
          }
          
        if (!HasSkillKit(SK_CLIMB))
          {
            IPrint("You must have a skill kit (or a <9>spider climb<7> spell) "
              "to ascend in that manner.");
            return ABORT;
          }
          
        for (r=1;r<=5;r++)
          for (tx=x-r;tx<=x+r;tx++)
            for (ty=y-r;ty<=y+r;ty++)
              if (abs(x-tx)==r || abs(y-ty)==r)
                if (m2->InBounds(tx,ty) && !(m2->FallAt(tx,ty)
                     || m2->SolidAt(tx,ty) || m2->FCreatureAt(tx,ty)))
                  goto FoundSpot;
                  
        /* Later, check for the potential to climb 2+ levels at a time */
                  
        IPrint("Looking up, you realize there's no nearby unoccupied flat ground to "
              "climb up onto -- just vertical chasm walls for 40 feet all around "
              "you.");
        return ABORT;
          
        FoundSpot:   
              
        if (HasStati(MOUNTED))
          {
            IPrint("You'd need to dismount first.");
            return ABORT;
          }      
        
        IPrint("You will need <Num> feet of rope to make that climb from "
               " where you are currently standing.", r * 10 + 10);
        
          
        if (!yn(Format("Climb up through the opening? (Climb %+d vs. DC %d)",
          SkillLevel(SK_CLIMB),20),true))
          return ABORT;
        
        if (!(Rope = ConsumeRope(r*10+10)))
          return ABORT; 
        Rope->Remove(true);
        
        Timeout += 60;
        IDPrint("You begin climbing up...",
                "The <Obj> climbs up into the rent ceiling above...",this);
        ox = x; oy = y;
        MoveDepth(m->Depth-1,true);
        if (SkillCheck(SK_CLIMB,20,true))
          PlaceNear(x,y);
        else
          {
            Move(ox,oy,false);
            TerrainEffects();
            return DONE;
          }
        IPrint("You complete your ascent successfully!");
        return DONE; 
            
      }
        
    if (m->TreeAt(x,y))
      {
        ClimbTree:
        if (HasStati(MOUNTED))
          {
            IPrint("You'd need to dismount first.");
            return ABORT;
          }
        if (HasStati(ELEVATED))
          {
            IPrint("You're already elevated.");
            return ABORT;
          }
        if (SkillLevel(SK_CLIMB) < 8 && !HasFeat(FT_BRACHIATION))
          {
            IPrint("You don't feel confidant enough in your climbing "
              "ability to climb the <Res>.", m->TerrainAt(x,y));
            return ABORT;
          }
        DPrint(e,"You climb up into the trees.", 
                 "The <EActor> climbs up into the trees.", m->TerrainAt(x,y));
        GainPermStati(ELEVATED,NULL,SS_MISC,ELEV_TREE,0);
        if (!HasFeat(FT_BRACHIATION))
          Timeout += 30;
        return DONE;
      }
          
    
    IPrint("You can't go up here.");
    return ABORT;
  }       

EvReturn Creature::Descend(EventInfo &e)
  {
    int16 i;
    Feature *ft;
    
    if (HasStati(STUCK) || HasStati(GRAPPLED) || HasStati(ENGULFED) ||
        HasStati(GRAPPLING) || HasStati(GRABBED))
      {
        IPrint("You can't descend while stuck, grappling or engulfed.");
        return ABORT;
      }
    if (HasStati(ELEVATED))
      {
        rID treeID;
        treeID = m->FFeatureAt(x,y) ?
                 m->FFeatureAt(x,y)->fID : 0;
        if (!treeID)
          treeID = m->TerrainAt(x,y);
        DPrint(e,"You climb down from the <Res>.", 
                 "The <EActor> climbs down from the <Res>.", treeID);
        RemoveStati(ELEVATED);
        if (!HasFeat(FT_BRACHIATION))
          Timeout += 15;
        return DONE;
      }
    
    for (ft=m->FFeatureAt(x,y);ft;ft=m->NFeatureAt(x,y))
      if (ft->Type == T_PORTAL)
        if (((Portal*)ft)->EnterDir(DOWN))
          {
            /* Disable reincarnation when the player actively decides
              to descend to a lower dungeon level on level 4 or greater
              -- this is here so that getting bull rushed into a chasm
              and dying doesn't cost you the ability to reincarnate the
              character. */
            if (isPlayer() && m->Depth >= 4)
              thisp->DisableReincarnation();
            if (Throw(EV_ENTER,this,ft) == DONE)
              return DONE;
            return ABORT;
          }
          
      
    /* This should eventually go in Creature::Descend() */
    if (HasStati(LEVITATION))
      if (m->FallAt(x,y))
        if (theGame->GetDungeonMap(m->dID,m->Depth+1,oPlayer(m->pl[0]),NULL))
          {
            IPrint("You float downwards.");
            MoveDepth(m->Depth+1,true);
            Timeout += 30;
            return DONE;
          }
    
    for (i=0;i!=8;i++)
      if (m->FallAt(x+DirX[i],y+DirY[i]))
        {
          if (!yn(Format("Climb down the chasm? (Climb %+d vs. DC %d)",
            SkillLevel(SK_CLIMB),15),true))
            return ABORT;
          Timeout += 60;
          IDPrint("You begin climbing down...",
                  "The <Obj> climbs down the chasm.",this);
          if (!SkillCheck(SK_CLIMB,15,true))
            {
              Move(x+DirX[i],y+DirY[i],false);
              TerrainEffects();
              return DONE;
            }
          MoveDepth(m->Depth+1,true);
          IPrint("You complete your descent successfully!");
          return DONE;
        }


    IPrint("You can't go down here.");
    return ABORT;
  }

EvReturn Creature::Mount(EventInfo &e) {
    if (HasStati(MOUNTED)) {
        IPrint("You're already mounted.");
        return ABORT;
    }

    if (!(e.EVictim && e.EVictim->isCreature() && e.EActor != e.EVictim)) { 
        IPrint("Don't be ridiculous!"); 
        return ABORT; 
    } else if (e.EVictim->m && e.EVictim->x != -1) {
        // ww: else we are mounting something in a script somewhere (e.g., to
        // make a death knight or a paladin), so ignore all of this flim-flam

        if (!HasSkill(SK_RIDE)) { 
            IPrint("You don't know how to ride!");
            return ABORT; 
        } else if (!HasMFlag(M_HUMANOID) || HasMFlag(M_NOLIMBS)) {
            IPrint("You can't ride in this form.");
            return ABORT; 
        } else if (!e.EVictim->HasMFlag(M_MOUNTABLE)) { 
            IPrint("That sort of creature cannot be ridden.");
            return ABORT; 
        } else if (e.EVictim->isMType(MA_VERMIN) && !e.EActor->isMType(MA_KOBOLD) && !(e.EVictim->isMType(MA_SPIDER) && e.EActor->isMType(MA_DROW))) {
            IPrint("Only kobolds can ride vermin.");
            return ABORT;
        } else if (e.EVictim->m && e.EVictim->isHostileTo(this)) { 
            IPrint("You cannot ride a hostile creature.");
            return ABORT; 
        } else if (e.EVictim->onPlane() != e.EActor->onPlane()) { 
            IPrint("You cannot ride a creature on another plane.");
            return ABORT; 
        } else if (e.EVictim->HasStati(PRONE) || e.EVictim->HasStati(STUCK) || e.EVictim->HasStati(GRAPPLED) || e.EVictim->HasStati(ASLEEP)) { 
            IPrint("You cannot ride a creature that is prone, stuck, grappled or asleep.");
            return ABORT; 
        } else if (e.EVictim->isPlayer()) {
            IPrint("You cannot use another player character as a mount.");
            return ABORT; 
        } else if (e.EVictim->m && !(e.EVictim->isMType(MA_ANIMAL) || e.EVictim->isFriendlyTo(this))) {
            /* You can ride any neutral horse you find, but just because you meet
             * a friendly gold dragon (on the basis of matching alignment) does not
             * mean that you can claim it as a mount! Checking "m" is a kludge for
             * narzugons and other monsters created with a mount, so that they can
             * get on their mounts before we've calculated the monster
             * hostilities.*/
            IPrint("The <Obj1> will not let you ride <him:Obj1>, at least not yet.", e.EVictim); return ABORT; 
        } else if (e.EVictim->ChallengeRating() > 2 && e.EVictim->ChallengeRating() > max(1,SkillLevel(SK_RIDE)/2) && !(e.EVictim->GetStatiObj(ANIMAL_COMPANION) == this)) { 
            IPrint("You fear that creature is too mighty to be handled well with your current degree of riding skill."); 
            return ABORT; 
        } else if (e.EVictim->GetAttr(A_SIZ) <= e.EActor->GetAttr(A_SIZ) &&
                   (e.EVictim->GetAttr(A_SIZ) != SZ_SMALL || e.EActor->GetAttr(A_SIZ) != SZ_SMALL) &&
                   !(e.EVictim->GetAttr(A_SIZ) == SZ_MEDIUM && e.EActor->isMType(MA_DWARF))) { 
            IPrint(Format("That %s %s is not large enough to support your (%s) weight.",
                Lookup(SizeNames,e.EVictim->GetAttr(A_SIZ)),
                (const char*)e.EVictim->Name(0),
                Lookup(SizeNames,e.EActor->GetAttr(A_SIZ))));
            return ABORT; 
        } else if (e.EVictim->GetAttr(A_SIZ) > e.EActor->GetAttr(A_SIZ)+1 && e.EVictim->isMType(MA_ANIMAL) && e.EVictim->isMType(MA_QUADRUPED)) { 
            /* Anyone can ride dragons, gryphons, et al., but halflings and kobolds
             * must use warponies or war dogs instead of warhorses. Maybe this should
             * just be a penalty of some kind? */
            IPrint("That kind of animal is too large for you to ride effectively.");
            return ABORT; 
        } else if (TREG(e.EActor->m->RegionAt(e.EActor->x,e.EActor->y))->Size <= e.EActor->GetAttr(A_SIZ)) {
            IPrint("Space is too limited here to allow mounting.");
            return ABORT;
        }
    }

    e.EVictim->Remove(false);
    e.EVictim->x = x;
    e.EVictim->y = y;
    e.EVictim->m = m;
    e.EVictim->GainPermStati(MOUNT,e.EActor,SS_MISC,0,0);
    e.EVictim->RemoveStati(HIDING);
    e.EActor->GainPermStati(MOUNTED,e.EVictim,SS_MISC,0,0);
    //Throw(EV_INIT_FIELDS,e.EVictim);
    if (e.EVictim->ts.getLeader() != e.EActor) {
        /* VERY IMPORTANT CORRECTION -- add the mount target only if
        we are riding a creature that is neutral to us, so that
        we are its master for only so long as we ride it. Do NOT
        add the mount target to creatures who were are allies
        already before we rode them! */
        if (!e.EVictim->ts.GetTarget(e.EActor))
            e.EVictim->ts.addCreatureTarget(e.EActor, TargetMount);
        e.EVictim->ts.Retarget(e.EVictim);
        e.EVictim->SetImage();
    }
    DPrint(e,"You mount the <EVictim>.",
        "The <EActor> mounts the <EVictim>.");
    return DONE;
}

EvReturn Creature::Dismount(EventInfo &e)
  {         
    e.ETarget = GetStatiObj(MOUNTED);
    if (!e.EVictim)
      { if (e.EParam == DSM_CHOICE)
          IPrint("You aren't mounted at the moment.");
        return ABORT; }
    e.EActor->RemoveStati(MOUNTED);
    e.EActor->RemoveStati(CHARGING);
    e.EVictim->RemoveStati(MOUNT);
    e.EVictim->m = NULL;
    e.EVictim->x = -1;
    e.EVictim->y = -1;
    e.EVictim->Remove(false);
    if (e.EVictim->ts.GetTarget(e.EActor) &&
        e.EVictim->ts.GetTarget(e.EActor)->type == TargetMount)
      e.EVictim->ts.removeCreatureTarget(e.EActor, TargetMount);
    e.EVictim->SetImage();
    e.EVictim->PlaceAt(m,x,y);
    e.EVictim->ts.Retarget(this);
    switch (e.EParam)
      {
        case 0:
          // silent dismount
          break;
        case DSM_CHOICE:
          DPrint(e,"You dismount from your <EVictim>.",
                   "The <EActor> gets off <his:EActor> <EVictim>."); 
         break;
        case DSM_THROWN:
          DPrint(e,"You are thrown from your mount.",
                   "The <EActor> is thrown from <his:EActor> mount.");
         break;
        case DSM_FALL:
          DPrint(e,"Your mount lands beside you.",
                   "An <EVictim> lands beside the <EActor>.");
         break;
        case DSM_FLOAT:
          DPrint(e,"You float off of your mount.",
                   "The <EActor> floats off <his:EActor> mount.");
         break;
        case DSM_INCOR:
          DPrint(e,"You pass through your mount.",
                   "The <EActor> passes through <his:EActor> mount.");
         break;
      }
    return DONE;
  }


void Creature::FocusCheck(Creature *killer)
{
  int16 i; Player *p; String msg;
  for(i=0;i!=MAX_PLAYERS;i++)
    if (p = oPlayer(theGame->p[i]))
      if (p->GetStatiObj(DWARVEN_FOCUS) == this)
      {
        if (killer && (p == killer || 
            GetStatiObj(HAS_ANIMAL,TA_LEADER) == killer)) {
          msg = Format("You have fulfilled your sworn vow and killed %c%s%c.", -RED, (const char*) this->Name(NA_THE), -GREY);
          p->MyTerm->Message(msg);
          p->MyTerm->Box(msg);
          // old formula of 50*CR made it really useful at level 1 and 2
          // and a big waste of time a depths
          p->GainXP(ChallengeRating()*ChallengeRating()*25);
        }
        else {
          msg = Format("You feel your heart grown leaden as you realize true vengeance against %s will forever elude you.",(const char*) this->Name(NA_THE));
          p->MyTerm->Message(msg);
          p->MyTerm->Box(msg);
          p->LoseXP(ChallengeRating()*200);
        }
        p->RemoveStati(DWARVEN_FOCUS,-1,-1,-1,this);              
        // p->SetStatiObj(DWARVEN_FOCUS,-1,NULL);              
      }
}

EvReturn Creature::Turn(EventInfo &e)
  {
    bool printed, found; 
    int16 mag, i; String s, t;

    found = false;

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
    DPrint(e,"You need an appropriate Holy Symbol to affect <Str>.",
              "The <EActor> lacks a Holy Symbol and fails to affect <Str>.",
              (const char*)Pluralize(Lookup(MTypeNames,e.EParam)).Lower());
    return ABORT; 
    
    HasComponent:

    if (isCharacter()) {
      if (!thisp->GodID) {
        IPrint("You must have a god to channel holy power.");
        return ABORT;
        }
      if (!thisp->isWorthyOf(thisp->GodID,false)) {
        IPrint("You must have <Res>'s favour to channel <Str> holy power.",
                 thisp->GodID, GodPronoun(thisp->GodID, true));
        return ABORT;
        }
      }


    
    if (e.AType == CA_TURNING || e.AType == CA_GREATER_TURNING)
      if (e.EActor->HasFeat(FT_DIVINE_ARMOUR) || e.EActor->HasFeat(FT_DIVINE_CLEANSING) ||
          e.EActor->HasFeat(FT_DIVINE_MIGHT) || e.EActor->HasFeat(FT_DIVINE_RESISTANCE) ||
          e.EActor->HasFeat(FT_DIVINE_VENGEANCE) || e.EActor->HasFeat(FT_DIVINE_VIGOR) ||
          e.EActor->HasFeat(FT_DIVINE_AEGIS))
        e.EActor->GainTempStati(CHANNELING,NULL,e.EActor->GetAttr(A_CHA)*2,SS_MISC);
    

    s = Format("%cTurn Check:%c %d",-13,-7,e.vDmg);

    if (e.EActor->HasFeat(FT_IMPROVED_TURNING)) {
      e.vDmg += 4; 
      s += " +4 feat";
    }

    // SRD: A cleric with 5 or more ranks in Knowledge (religion) gets a +2
    // bonus on turning checks ...
    if (e.EActor->SkillLevel(SK_KNOW_THEO) > 5) {
      e.vDmg += 2; 
      s += " +2 theo";
    }

    e.vDmg += Mod(A_CHA);
    s += Format(" %+d cha",Mod(A_CHA));

    int max_dist = 10 + e.vDmg;
    bool hitNeutral;
    hitNeutral = false;
    for (int dx = -max_dist; dx <= max_dist; dx++) 
      for (int dy = -max_dist; dy <= max_dist; dy++) {
        int xx = x + dx;
        int yy = y + dy; 
        if (!m->InBounds(xx,yy) || dist(x,y,xx,yy) > max_dist)
          continue; 
        for (Thing * it = m->FirstAt(xx,yy) ; it ; it = m->NextAt(xx,yy)) {
          if (!it->isCreature() || it == this) continue; 
          Creature * c = (Creature *)it; 
          bool valid = c->isMType(e.EParam);
          int i = 0; 
          StatiIterNature(this,ALSO_TURN_MA_TYPE)
              if (c->isMType(S->Val) && e.AType != CA_GREATER_TURNING) 
                valid = true; 
          StatiIterEnd(this)
          if (valid && !c->isHostileTo(e.EActor) && (e.EActor->isMType(MA_GOOD) ||
                (e.EActor->isCharacter() && (e.EPActor->desiredAlign & AL_GOOD))))
            hitNeutral = true;
          }
        }
        
    if (hitNeutral)
      if (!yn("You feel uncertain about this. Continue?"))
        return ABORT;



    // WW: from the SRD: "A cleric may attempt to turn undead a number of
    // times per day equal to 3 + his Charisma modifier" -- this fatigue
    // cost is way, way too high for that: a starting cleric with 18 CHR
    // could turn 7 times per day, which is up there with the number of
    // magic missiles a level 3 mage can cast. 
    // Let's experiment Turning coming out of Mana -- a level 2 cleric
    // turning undead pays as per a level 2 spell ...
    /* FJM: Turning is intended to be somewhat different, and a lot more
       powerful, in Incursion. In SRD, you can't even affect moderately
       powerful undead; it's for getting rid of popcorn. Anyway, it was
       not all that, so I amped up the ratio a bit and lowered the fatigue
       cost. Let me know if this is now a good/worthwhile ability. */
    if (!LoseFatigue(e.AType == CA_GREATER_TURNING ? 4 : 2,true))
      return ABORT;
      
    Timeout += 30;

    if (e.AType == CA_COMMAND)
      DPrint(e,"You exercise divine authority over <Str>.",
               "The <EActor> compels the <Str> to obey <him:EActor>.",
               (const char*)Pluralize(Lookup(MTypeNames,e.EParam)).Lower());
    else
      DPrint(e,"You channel the sacred energies of <Str>.",
               "The <EActor> waves <his:EActor> holy symbol and prays.",
               (isPlayer() && thisp->GodID) ? NAME(thisp->GodID) : "the gods");

    for (int dx = -max_dist; dx <= max_dist; dx++) 
      for (int dy = -max_dist; dy <= max_dist; dy++) {
        int xx = x + dx;
        int yy = y + dy; 
        if (!m->InBounds(xx,yy) || dist(x,y,xx,yy) > max_dist)
          continue; 
        for (Thing * it = m->FirstAt(xx,yy) ; it ; it = m->NextAt(xx,yy)) {
          if (!it->isCreature() || it == this) continue; 
          Creature * c = (Creature *)it; 
          bool valid = c->isMType(e.EParam);
          int i = 0; 
          StatiIterNature(this,ALSO_TURN_MA_TYPE)
              if (c->isMType(S->Val) && e.AType != CA_GREATER_TURNING) 
                valid = true; 
          StatiIterEnd(this)
          if (valid && c != this && !c->isLedBy(this)) {
              found = true;
              printed = false;
              e.EVictim = c;
              int roll = Dice::Roll(1,20);
              int resist = (max(e.EVictim->ChallengeRating()+
                         e.EVictim->AbilityLevel(CA_TURN_RESISTANCE),1));

              mag = ((e.vDmg+roll) * 750) / (resist * 100);

              t = Format("%s +1d20 (%d) = %d vs %s (%d).",
                  (const char*)s,
                  roll,e.vDmg + roll,
                  (const char*)c->Name(0),
                  resist /*(resist * 3) / 2)*/);
              t = XPrint(t);
              if (e.EActor->isPlayer())
                {
                  e.EPActor->MyTerm->SetWin(WIN_NUMBERS3);
                  e.EPActor->MyTerm->Clear();
                  e.EPActor->MyTerm->Write(t);
                  if (e.EPActor->Opt(OPT_STORE_ROLLS))
                    e.EPActor->MyTerm->AddMessage(t);
                }

              if (mag > 5) {
                Exercise(A_WIS,random(mag/5)+1,EWIS_TURNING,50);
                Exercise(A_CHA,random(mag/5)+1,ECHA_TURNING,50);
                }
                
              if (e.AType == CA_COMMAND) {
                if (c->HasStati(CHARMED,-1,this))
                  continue;
                
                if (mag > 40) { /* Four times CR */
                  TPrint(e,"The <EVictim> yields to your will.",
                           "You fall under the <EActor>'s thrall.",
                           "The <EVictim> bows down to the <EActor>.");
                  e.EVictim->GainPermStati(CHARMED,e.EActor,SS_MISC,CH_COMMAND,0,0);
                  }
                else if (mag > 10) {
                  if (mag > 20) {
                    TPrint(e,"The <EVictim> is paralyzed with awe.",
                             "The <EActor>'s authority pins you in place.",
                             "The <EVictim> is paralyzed with awe.");
                    printed = true;
                    e.EVictim->GainTempStati(PARALYSIS,NULL,Dice::Roll(1,4),SS_MISC);
                    }
                  else if (!printed)
                    TPrint(e,"The <EVictim> is justly rebuked.",
                             "The <EActor>'s authority terrifies you.",
                             "The <EVictim> is terrified of the <EActor>.");
                  e.EVictim->GainTempStati(AFRAID,NULL,Dice::Roll(1,4),SS_MISC,
                                             FEAR_PANIC);
                  }
                continue;
                }

              if (mag > 60 || (e.AType == CA_GREATER_TURNING && mag > 10)) { /* Six Times CR */
                TPrint(e,"An <EVictim> is blasted out of existence!",
                         "An <EActor>'s holy power tears you apart!",
                         "An <EVictim> is blasted out of existence!");
                printed = true; 
                ThrowDmg(EV_DEATH, 0, 0, "holy power", e.EActor, e.ETarget);
                continue;
                }
              if (mag > 30) { /* Three times CR */
                if (!printed)
                TPrint(e,"An <EVictim> is blasted by holy energy!",
                         "An <EActor>'s holy power blasts you!",
                         "An <EVictim> is blasted by holy energy!");
                int dmg = Dice::Roll(e.vDmg + roll,4);
                ThrowDmg(EV_DAMAGE, AD_HOLY, dmg, "being turned", e.EActor,e.EVictim);
                printed = true;
                }
              if (mag > 20) { /* Twice CR */
                if (!printed)
                TPrint(e,"An <EVictim> is overcome with revulsion!",
                         "An <EActor>'s holy power overpowers you with revulsion!",
                         "An <EVictim> is overcome with revulsion!");
                e.EVictim->GainTempStati(STUNNED, NULL, (e.vDmg+roll)*1+5,SS_MISC,0,0,0);
                printed = true;
                }
              if (mag > 10) { /* Equal CR */
                if (!printed)
                TPrint(e,"An <EVictim> becomes terrified!",
                         "An <EActor>'s holy power terrifies you!",
                         "An <EVictim> becomes terrified!");
                e.EVictim->GainTempStati(AFRAID, NULL, (e.vDmg+roll)*3+10,SS_MISC,FEAR_PANIC,0,0);
                printed = true;
                }
              if (mag > 5) { /* 1/2 CR */
                if (!printed)
                TPrint(e,"An <EVictim> is badly shaken.",
                         "An <EActor>'s holy power shakes your morale.",
                         "An <EVictim> is badly shaken.");
                e.EVictim->GainTempStati(ADJUST_MOR, NULL, (e.vDmg+roll)*6+20,SS_MISC,A_AID,-2,0);
                printed = true;
                }
              if (!printed)
                TPrint(e,"An <EVictim> stands firm in the face of an <EActor>'s holy power.",
                         "You resist an <EActor>'s holy power.",
                         "An <EVictim> stands firm in the face of an <EActor>'s holy power.");
            }
        }
      }
    if (!found)
      IPrint("Nothing happens.");
    return DONE;
  }

EvReturn Creature::Phase(EventInfo &e)
  {               
    const char * Planes[8] = { "material plane", "astral plane", 
      "ethereal plane", "plane of shadow", "negative energy plane",
      "positive energy plane", "???", "???" };       
    int16 i; bool vis[MAX_PLAYERS];
    int8 phase_type = (int8)AbilityLevel(CA_PHASE);
    ASSERT(phase_type);
    if (HasStati(PHASED,phase_type))
      {
        IPrint("You return to the material plane.");
        for (i=0;i!=MAX_PLAYERS;i++)
          if (m->pl[i] && m->pl[i] != myHandle)
            vis[i] = oCreature(m->pl[i])->Perceives(this) != 0;
        RemoveStati(PHASED);
        SetImage();
        for (i=0;i!=MAX_PLAYERS;i++)
          if (m->pl[i] && m->pl[i] != myHandle)
            if (oCreature(m->pl[i])->Perceives(this))
              oCreature(m->pl[i])->IPrint( vis[i] ?
                "The <Obj> shifts back to the material plane." :
                "An <Obj> appears out of nowhere!", this );
        Timeout += 12;
        return DONE;
      }
    if (HasStati(PHASED))
      {
        IPrint("Your ability won't function on this plane.");
        return ABORT;
      }

    IPrint("You phase into the <Str>.", Planes[phase_type]);

    for (i=0;i!=MAX_PLAYERS;i++)
      if (m->pl[i] && m->pl[i] != myHandle)
        vis[i] = oCreature(m->pl[i])->Perceives(this) != 0;
    
    GainPermStati(PHASED, NULL, SS_MISC, phase_type);
    SetImage();
    for (i=0;i!=MAX_PLAYERS;i++)
      if (m->pl[i] && m->pl[i] != myHandle)
        if (vis[i])
          oCreature(m->pl[i])->IPrint( oCreature(m->pl[i])->Perceives(this) ?
            "The <Obj> shifts into the <Str>." :
            "An <Obj> winks out of existence!", this , Planes[phase_type] );
    
    Timeout += 12;
    return DONE;
  }
    
void Creature::SetupDig(Dir d)
  {

  }        
              
EvReturn Creature::Dig(EventInfo &e)      
  {
    Item *it; TTerrain *tt;
    Feature *feat;
    int16 tx, ty, fat, time, percent;
    rID regID, terID;
    
    int16 hard, diff, pow;
    it = InSlot(SL_WEAPON);
    
    if (!it || !it->HasIFlag(WT_DIGGER))
      {
        IPrint("You need to wield a digging implement first.");
        return ABORT;
      }
    tx = e.EXVal; ty = e.EYVal;
    tt = TTER(m->TerrainAt(tx,ty));
    hard = MaterialHardness(tt->Material,AD_PIERCE);
    pow  = SkillLevel(SK_MINING) + it->GetPlus();

    if (feat = m->FFeatureAt(tx,ty))
      {
        if (feat->Type == T_DOOR)
          if (((Door*)feat)->DoorFlags & DF_SECRET)
            { hard = 10; goto DoDig; }
        return ABORT;
      }
 
    DoDig:
    if (hard > pow)
      {
        IPrint("That area is too hard for you to dig through.");
        return ABORT;
      }
    diff = hard*2 - pow;
    
    
    fat  = (diff+5)*3;
    time = 1 + diff/6;
    percent = 100 / SkillLevel(SK_MINING);
    if (m->PercentSI + percent > 100)
      if (!yn(Format("There is a %d~ chance of collapse! Continue?",
                       min(100,(m->PercentSI + percent) - 100))))
        return ABORT;

    if (Dice::Roll(1,20) <= fat)
      if (!LoseFatigue(1+(fat-20)/10,true))
        return ABORT;

    IDPrint("You begin digging...",
            "The <Obj> starts tunneling...",this);
      
    if (isPlayer()) {
      if (thisp->SpendHours(time,time) != DONE)
        return DONE;
      }
    else
      return ABORT;

    if (m->PercentSI + percent > 100 + random(100))
      {
        IPrint("The rock collapses around you!");
        ThrowDmg(EV_DAMAGE,AD_BLUNT,Dice::Roll(10,10),
          "a mining accident", e.EActor, e.EActor);
        return DONE;
      }
    m->PercentSI += percent;

    Exercise(A_STR,Dice::Roll(1,12)+max(0,hard-10)/2,ESTR_MINING,50);

    if (feat = m->FFeatureAt(tx,ty))
      {
        if (feat->Type == T_DOOR)
          if (((Door*)feat)->DoorFlags & DF_SECRET)
            { 
              IPrint("Chipping away stone, you find a secret door!");
              ((Door*)feat)->DoorFlags &= ~DF_SECRET;
              ((Door*)feat)->SetImage();
              return DONE;
            }
        return DONE;
      }

    regID = RES(m->dID)->GetConst(DIGGING_REGION);
    terID = TREG(regID)->Floor;

    if (!m->SolidAt(tx,ty))
      return ABORT;

    
    ThrowXY(EV_MINE,tx,ty,this);
    m->At(tx,ty).Memory = 0;
    Rect r;
    r.Set((uint8)tx,(uint8)ty,(uint8)tx,(uint8)ty);
    m->WriteAt(r,tx,ty,terID,regID,100,true);
    m->At(tx,ty).Memory = 0;
    m->VUpdate(tx,ty);
    IPrint("Done. (<Num>~ structural integrity remaining.)", 
              max(0,100-m->PercentSI));
    return DONE; // ww: previously no return value 

  }

  void Character::LegendIdent(Item *it) {
      bool anything = false, isRunic;
      int16 i; String flav;
      /* Later, legend lore should apply to monsters as well as
      items, upon first striking or being struck by an unidentified
      enemy. For now, in a low-level game, it's plenty useful. */

      if (isPlayer()) {
          if (thisp->Opt(OPT_AUTOID)) {
              if (!it->isKnown(KN_MAGIC))
                  anything = true;
              it->MakeKnown(0xFF);
              /* ww: this isn't necessary
              if (anything)
              IPrint("WizIdent: <Str>.", it->Name(NA_MECH|NA_LONG));
              */
              return;
          } else {
              /* ww: there is too much junk in incursion (especially daggers and
              * rapiers and whatnot) that sleeping with all of it just to be
              * able to separate the wheat from the chaff is not reasonable.
              * Other roguelikes (e.g., Angband, Baldur's Gate) have big ways to
              * easily identify stuff on the floor. For now, we apply Intuition
              * to an item when you first see it. This makes it easy to tell
              * what you should be spending time on. We also tone down Intuition
              * so that you get less info (mostly just mundane vs. magical). 
              *
              * fjm: I recognize this argument as valid, but from a game aesthetic
              * sense I like Intuition working when you rest. Since your change is
              * likely cruical to playability, however, I've decided to add it as
              * an option that is by default "on", at least for the things that
              * have the greatest "junk to magic" ratio -- weapons and armour. Let
              * me know what you think. I've left auto-intuit off for wands, potions,
              * and other things that are A) almost always magical, and B) have
              * flavours that allow easy ID-ing.
              */
              switch (thisp->Opt(OPT_EASY_INTUIT)) {
              case 0: break; /* No Easy Intuit */
              case 1: thisp->IntuitItem(it); break;
              case 2: if ( it->isType(T_WEAPON) || it->isType(T_ARMOUR) ||
                          it->isType(T_MISSILE) || it->isType(T_BOW) ||
                          it->isType(T_SHIELD) )
                          thisp->IntuitItem(it); break;
              }
          }
          if (HasAbility(CA_FATESENSE) && !it->isKnown(KN_CURSE)) {
              it->MakeKnown(KN_CURSE);
              if (it->isCursed())
                  IPrint("You feel a faint shiver from the <Obj>.",it);
          }
      }

      if (HasSkill(SK_DECIPHER) && !(it->isKnown(KN_MAGIC) && 
          it->isKnown(KN_PLUS) && it->isKnown(KN_CURSE)) && 
          !(it->IFlags & IF_DECIPHERED)) {
          const char* RunicWords[] = { "rune", "runic", "runed",
              "inscribed", "written", "symbol", "glyph", "engraved",
              "ancient", "script", "iconic", NULL };
          isRunic = false;
          if (it->IFlags & IF_RUNIC)
              isRunic = true;
          if (it->eID) {
              flav = NAME(EFFMEM(it->eID,thisp)->FlavorID);
              for (i=0;RunicWords[i];i++)
                  if (strstr(flav,RunicWords[i]))
                      isRunic = true;
          }
          if (isRunic) {
              if (SkillCheck(SK_DECIPHER,10+it->ItemLevel()*2,true)) {
                  IPrint("By translating the runes covering the <Obj>, "
                      "you discern that it is in fact <str>!",
                      it, (const char*) it->Name(NA_A|NA_IDENT));
                  it->MakeKnown(KN_MAGIC|KN_CURSE|KN_PLUS);
                  Exercise(A_INT,random(6)+max(1,it->ItemLevel()),EINT_IDENT,75);
              } else
                  IPrint("You fail to translate the runes covering the <Obj>.", it);

          }
          it->IFlags |= IF_DECIPHERED;
      }


      if (it->isType(T_MUSH) || it->isType(T_HERB))
          if (HasAbility(CA_NATURE_SENSE)) {
              it->MakeKnown(0xFF);
              Exercise(A_INT,random(6)+max(1,it->ItemLevel()),EINT_IDENT,60);
              IPrint("With your flawless knowledge of nature, you immediately "
                  "recognize this <Str> as an <Obj>.",it->isType(T_HERB) ? "herb" :
                  "mushroom", it);
              return;
          }

      if (!HasAbility(CA_LEGEND_LORE))
          return;
      if (it->isType(T_CORPSE) ||
          it->isType(T_CHEST) ||
          it->isType(T_COIN) ||
          it->isType(T_STATUE)
          )
          return; 

      if (it->isMagic()) {
          i = (10 + AbilityLevel(CA_LEGEND_LORE) + Mod(A_INT)) - it->ItemLevel();

          if (i > 0 && !it->isKnown(KN_NATURE)) {
              it->MakeKnown(KN_NATURE);
              anything = true;
          }
          if (i > 5 && !it->isKnown(KN_CURSE)) {
              it->MakeKnown(KN_CURSE);
              anything = true;
          }
          if (i > 8 && !it->isKnown(KN_BLESS)) {
              it->MakeKnown(KN_BLESS);
              anything = true;
          }
          if (i > 10 && !it->isKnown(KN_MAGIC)) {
              it->MakeKnown(KN_MAGIC);
              anything = true;
          }
          if (i > 15 && !it->isKnown(KN_PLUS)) {
              it->MakeKnown(KN_PLUS);
              anything = true;
          }
          if (i > 20 && !it->isKnown(KN_PLUS2)) {
              it->MakeKnown(KN_PLUS2);
              anything = true;
          }
          if (i > 30 && !it->isKnown(KN_ARTI)) {
              it->MakeKnown(KN_ARTI);
              anything = true;
          }
          if (anything) {
              IPrint("Drawing on old legends and stories, you recognize this as an <Obj>.",it);
              if (i >= 10)
                  Exercise(A_INT,Dice::Roll(2,10),EINT_IDENT,60);
          }
      }
  }
  
/* We call this from Map::Update in a kinda-kludge that calls it for
   every unscrutinized monster when the glyph for that monster is first
   drawn on the screen. Thus, we don't have to worry about things like
   LOS or being blind here. */
void Player::ScrutinizeMon(Creature *cr)
  {
    String name1, name2;
    int16 sk, lv; bool knowClass, unseenClass, knowRace, unseenRace;
    if (!XPerceives(cr))
      return;
    if (cr->StateFlags & MS_SCRUTINIZED)
      return;
    cr->StateFlags |= MS_SCRUTINIZED;
  
    if (!(cr->HasAttk(A_SEEM) || cr->HasAttk(A_FORM) || cr->HasStati(TEMPLATE)))
      return;
      
    knowClass = false; unseenClass = false;
    knowRace = false; unseenRace = false;
     
    if (cr->HasAttk(A_SEEM) || (cr->HasAttk(A_FORM) || cr->HasStati(POLYMORPH)))
      unseenRace = true;
  
    sk = 0; lv = 0; 
    
    if (cr->isMType(MA_UNDEAD) && HasSkill(SK_KNOW_UNDEAD))
      if (SkillLevel(SK_KNOW_UNDEAD) > lv)
        { lv = SkillLevel(SK_KNOW_UNDEAD); sk = SK_KNOW_UNDEAD; }
    
    if (cr->isMType(MA_AQUATIC) && HasSkill(SK_KNOW_OCEANS))
      if (SkillLevel(SK_KNOW_OCEANS) > lv)
        { lv = SkillLevel(SK_KNOW_OCEANS); sk = SK_KNOW_OCEANS; }
    
    if ((cr->isMType(MA_PLANT) || cr->isMType(MA_FUNGI) || cr->isMType(MA_NATURAL) ||
        cr->isMType(MA_ANIMAL) || cr->isMType(MA_SYLVAN) || cr->isMType(MA_BEAST)) 
        && HasSkill(SK_KNOW_NATURE))
      if (SkillLevel(SK_KNOW_NATURE) > lv)
        { lv = SkillLevel(SK_KNOW_NATURE); sk = SK_KNOW_NATURE; }
    
    if ((cr->isMType(MA_MYTHIC) || cr->isMType(MA_FAERIE) || cr->isMType(MA_NAGA) ||
         cr->isMType(MA_LYCANTHROPE) || cr->isMType(MA_DRAGON)) && HasSkill(SK_KNOW_MYTH))
      if (SkillLevel(SK_KNOW_MYTH) > lv)
        { lv = SkillLevel(SK_KNOW_MYTH); sk = SK_KNOW_MYTH; }
    
    if ((cr->isMType(MA_EYE) || cr->isMType(MA_VORTEX) || cr->isMType(MA_MAGC))
           && HasSkill(SK_KNOW_MAGIC))
      if (SkillLevel(SK_KNOW_MAGIC) > lv)
        { lv = SkillLevel(SK_KNOW_MAGIC); sk = SK_KNOW_MAGIC; }
    
    if ((cr->isMType(MA_OUTSIDER) || cr->isMType(MA_ELEMENTALKIN) || 
          cr->isMType(MA_PLANETOUCHED)) && !(cr->isMType(MA_DEMON) ||
          cr->isMType(MA_DEVIL)) && HasSkill(SK_KNOW_PLANES))
      if (SkillLevel(SK_KNOW_PLANES) > lv)
        { lv = SkillLevel(SK_KNOW_PLANES); sk = SK_KNOW_PLANES; }    

    if ((cr->isMType(MA_DEMON) || cr->isMType(MA_DEVIL)) && HasSkill(SK_KNOW_INF))
      if (SkillLevel(SK_KNOW_INF) > lv)
        { lv = SkillLevel(SK_KNOW_INF); sk = SK_KNOW_INF; }

    if ((sk != 0) && SkillCheck(sk,10 + cr->ChallengeRating(),false))
      knowRace = true;
    if (SkillCheck(SK_APPRAISE,10 + cr->ChallengeRating(),false))
      knowClass = true;

    
    
    name1 = cr->Name(0);

    cr->StateFlags |= MS_KNOWN;
    if (cr->HasAttk(A_FORM))
      cr->StateFlags |= MS_POLY_KNOWN;
    
    StatiIterNature(cr,TEMPLATE)
      if (S->Mag)
        continue;
      if (TTEM(S->eID)->TType & (TM_CLASS|TM_CHIEFTAN|TM_SHAMAN|TM_SKILL))
        {
          unseenClass = true;
          if (!knowClass)
            continue;
          S->Mag = 1;
        }
      else if (TTEM(S->eID)->TType & (TM_PLANAR))
        {
          unseenRace = true;
          if (!SkillCheck(SK_KNOW_PLANES,10 + cr->ChallengeRating(),false))
            continue;
          sk = SK_KNOW_PLANES;
          knowRace = true;
          S->Mag = 1;
        }
      else if (knowRace)
        S->Mag = 1;
    StatiIterEnd(cr)
    
    cr->SetImage();
    
    name2 = cr->Name(0);
    
    if (stricmp(name1,name2) == 0)
      return;
  
    TextVal KnowTypes[] = {
      { SK_KNOW_PLANES, "knowledge of cosmology" },
      { SK_KNOW_INF, "knowledge of fiends" },
      { SK_KNOW_OCEANS, "knowledge of aquatic life" },
      { SK_KNOW_MAGIC, "knowledge of magical phenomina" },
      { SK_KNOW_MYTH, "knowledge of mythology" },
      { SK_KNOW_NATURE, "knowledge of nature" },
      { SK_KNOW_UNDEAD, "knowledge of the undead" },
      { 0, NULL } };
    if (unseenRace && knowRace && unseenClass && knowClass) {
      IPrint("Appraising the <str1> with your <Str2>, you realize that <she:obj3> "
               "is really an <Obj3>!",
             (const char*)name1,
             Lookup(KnowTypes,sk),
             cr);
      Exercise(A_INT,random(8)+1,EINT_IDENT,35);
      }
    else if (unseenRace && knowRace) {
      IPrint("Using your <Str>, you realize that this <str> is really an <obj>!",
        Lookup(KnowTypes,sk), (const char *)name1, cr);
      Exercise(A_INT,random(4)+1,EINT_IDENT,35);
      }
    else if (unseenClass && knowClass) {
      IPrint("Appraising the <str>, you realize that <she:obj2> is really an <Obj2>!",
             (const char*)name1, cr);
      Exercise(A_INT,random(6)+1,EINT_IDENT,35);
      }
  }

// ww: following the SRD, somewhat
void Player::WildShape()
{
  String s;
  rID direID = FIND("dire"), mID;

  /*
  if (ResistLevel(AD_POLY) == -1) {
    IPrint("You find you cannot change form!");
    return;
  }
  */
  int lev = AbilityLevel(CA_WILD_SHAPE);
  // already includes feats like FT_WILD_SHAPE_MASTERY
  if (lev < 3) {
    IPrint("You need three druid levels to assume a Wild Shape!");
    return;
  } 

  for (int modIdx = 0; modIdx < 1; modIdx++) {
    Module *mod = theGame->Modules[modIdx];
    for (int idx = 0; idx < mod->szMon; idx++) {
      mID = mod->MonsterID(idx);
      if (okWildShape(lev,mID,0)) {
        Monster *mon = new Monster(mID);
        mon->CalcValues(); 
        s = Format("%c%s",-14,NAME(mID));
        MyTerm->LOption(s,mID,mon->Describe(this),
            999 - mod->QMon[idx].HitDice);
        delete mon; 
      } else if (okWildShape(lev+20,mID,0)) {
        Monster *mon = new Monster(mID);
        mon->CalcValues(); 
        s = Format("%c%s",-8,NAME(mID));
        MyTerm->LOption(s,-idx,mon->Describe(this),1000 +
          mod->QMon[idx].HitDice);
        delete mon; 
        }
    }
  }
  mID = MyTerm->LMenu(MENU_ESC|MENU_DESC|MENU_3COLS|MENU_SORTED,
      "Wild Shape Into What Type of Creature?", WIN_MAP,0);
  if ((long)mID <= 0 || mID == (rID)-1) return;

  if (!LoseFatigue(1,true))
    return; 

  IPrint("You concentrate and wildshape into a <Res>.",mID);
  if (!HasFeat(FT_FAST_WILD_SHAPE))
  Timeout += 30; 
  // ww: true healing on a shapeshift is too powerful in Incursion since
  // you can shapeshift whenever you want 
  double hp_percent = (double) cHP / (double)(mHP + Attr[A_THP]);
  Shapeshift(mID,true,NULL);   
  GainPermStati(POLYMORPH,this,SS_ENCH,-1,-1,0);
  CalcValues(true);
  CalcValues(false);
  cHP = (int16) ( hp_percent * (mHP + Attr[A_THP]) ) ; 
  CalcValues(true);
  CalcValues(false);
  return; 
}

// ww: following the SRD, somewhat ...
void Player::SummonAnimalCompanion(bool mount)
{
  rID mIDs[128], tIDs[128]; bool found = false;
  rID MountList[1025]; int16 cMountList; String s;
  Monster *mn; String mname;
  int16 lev = AbilityLevel(mount ? CA_SACRED_MOUNT : CA_ANIMAL_COMP);
  if (lev <= 0) return; 
  int16 ft_bonus = HasFeat(mount ? FT_IMPROVED_MOUNT : FT_ANIMAL_BOND) ? 
                      max(0,Mod(A_CHA) / 3) : (mount ? 0 : -2); 
  int16 i, n, j;
  Creature *c;
  rID tID = mount ? FIND("celestial") : FIND("dire");

  // have we already done this today? 
  if (HasStati(TRIED,ANIMAL_COMPANION,NULL)) {
    IPrint("You may only summon one such creature per day; rest first.");
    return;
  } 

  // Wait, do we already have a companion? 
  MapIterate(m,c,i)    
    if (c->isMonster())          
      if (c->HasStati(ANIMAL_COMPANION,TA_LEADER,this))
      {
        FoundComp:
        s = Format("Dismiss %s, your current %s?",
                     (const char*)c->Name(0),
                     mount ? "mount" : "companion");
        if (yn(s,false)) {
          // shouldn't really be death, but this is easier for now ...
          ThrowDmg(EV_DEATH,AD_NORM,0,"Druidic whim",c,c);
        } else {
          IPrint("You may only have one such creature!");
          return; 
        }
      }
  if (c = (Creature*) GetStatiObj(MOUNTED))
    if (c->HasStati(ANIMAL_COMPANION))
      goto FoundComp;
  c = NULL;


  // find all Animals with CR <= 'lev'
  n = 0;
  
  /* New, race-based paladin mounts */
  if (mount && TEFF(RaceID)->GetList(PALADIN_MOUNTS,MountList,1024))
    {
      bool skip; int16 tc;
      Monster *ml[128];
      for(n=0;n!=1020 && (MountList[n] || 
           MountList[n+1] || MountList[n+2]);n++)
        ;
      cMountList = n; tc = 0; n = 0;
      skip = false;
      for(i=0;i!=cMountList;i++)
        {
          if (MountList[i] == FIND("unimplemented"))
            { tc = 0; continue; }
          if(MountList[i] < 50)
            {
              if (AbilityLevel(CA_SACRED_MOUNT) < (int16)MountList[i])
                skip=true;
              else
                skip=false;
              continue;
            }
            
          if (RES(MountList[i])->Type == T_TTEMPLATE)
            {
              tIDs[tc++] = MountList[i];
              continue;
            }
            
          ASSERT(RES(MountList[i]) && RES(
            MountList[i])->Type == T_TMONSTER);
            
          if (skip)
            {
              tc = 0;
              continue;
            }
            
          ml[n] = new Monster(MountList[i]);
          
          for(j=0;j!=tc;j++)
            ml[n]->AddTemplate(tIDs[j]);
            
          MyTerm->LOption(ml[n]->Name(0),n,ml[n]->Describe(this));
          
          n++;
          tc = 0;
        }
      
      if (!n)
        {
          IPrint("There are no suitable creatures for you to summon. This "
                 "is probably a bug in your race entry's PALADIN_MOUNTS "
                 "list.");
          return;
        }    
      i = (int16)MyTerm->LMenu(MENU_ESC|MENU_DESC|MENU_2COLS|MENU_SORTED,
          mount ? "What type of Bonded Mount?" :
                  "What Type of Animal Companion?", WIN_MAP,0);
      if (i == -1) return;
      
      mn = ml[i];
      
      for(j=0;j!=n;j++)
        if (ml[j] != mn)
          delete ml[j];

      int diff = lev + ft_bonus - mn->ChallengeRating(); 
      if (diff < 1) diff = 1;
      else if (diff > 8) diff = 8;
      rID tid = FIND(Format("sacred;%d",diff));
      mn->AddTemplate(tid);

      
      goto MountMade;
    }
  
  for (int modIdx = 0; modIdx < 1; modIdx++) {
    Module *mod = theGame->Modules[modIdx];
    for (int idx = 0; idx < mod->szMon; idx++) {
      rID mID = mod->MonsterID(idx);
      if (mount) {
        if (TMON(mID)->isMType(mID,MA_DRAGON)) {
          const char * AgeCats[7] = { "hatchling", "very young", "young"
                                      "juvenile", "young adult", "adult",
                                      NULL };
          for (int16 i=0;AgeCats[i];i++)
            if (tID = FIND(AgeCats[i]))
              if (okBondedMount(lev,mID,tID)) {
                Monster *mon = new Monster(mID);
                mon->AddTemplate(tID);
                MyTerm->LOption(mon->Name(0),n,mon->Describe(this),
                                  mod->QMon[idx].CR);
                mIDs[n] = mID; tIDs[n] = tID; n++;
                delete mon; 
                }
          }
        else if (okBondedMount(lev,mID,0)) {      
          Monster *mon = new Monster(mID);
          int diff = lev + ft_bonus; 
          if (diff < 1) diff = 1;
          else if (diff > 8) diff = 8;
          rID tid = FIND(Format("sacred;%d",diff));
          mon->AddTemplate(tid);
          MyTerm->LOption(mon->Name(0),n,mon->Describe(this),
                            mod->QMon[idx].CR);
          mIDs[n] = mID; tIDs[n] = 0; n++;
          delete mon; 
          }
        }
      else {
        if (okAnimalComp(lev,mID,0)) {      
          Monster *mon = new Monster(mID);
          int diff = ((1 + lev + ft_bonus) - max(mon->ChallengeRating(),0));
          if (diff < 1) diff = 1;
          else if (diff > 12) diff = 12;
          rID tid = FIND(Format("companion;%d",diff));
          mon->AddTemplate(tid);
          mname = mon->Name(0);
          mname.Replace("Companion ", "");
          MyTerm->LOption(mname,n,mon->Describe(this),
                            mod->QMon[idx].CR);
          mIDs[n] = mID; tIDs[n] = 0; n++;
          delete mon; 
          }
        }
    }
  }
  if (!n) 
    {
      IPrint("There are no suitable creatures for you to summon.");
      return;
    }
  n = (int16)MyTerm->LMenu(MENU_ESC|MENU_DESC|MENU_2COLS|MENU_SORTED,
      mount ? "What type of Bonded Mount?" :
              "What Type of Animal Companion?", WIN_MAP,0);
  if (n == -1) return;



  mn = new Monster(mIDs[n]);
  if (tIDs[n])
    mn->AddTemplate(tIDs[n]);
    
  MountMade:
  mn->PlaceAt(m,x,y);
  if (!mount) {
    int diff = ((1 + lev + ft_bonus) - max(mn->ChallengeRating(),0));
    if (diff < 1) diff = 1;
    else if (diff > 12) diff = 12;
    rID tid = FIND(Format("companion;%d",diff));
    mn->AddTemplate(tid);
    }
  else {
    /*
    int diff = lev + ft_bonus; 
    if (diff < 1) diff = 1;
    else if (diff > 8) diff = 8;
    rID tid = FIND(Format("sacred;%d",diff));
    mn->AddTemplate(tid);
    */
  } 
  mn->Initialize(true);
  mn->Named = NAME(getAnimalName());
  mn->GainPermStati(ANIMAL_COMPANION,this,SS_MONI,TA_LEADER,0,0);
  RemoveStati(HAS_ANIMAL,SS_MONI,TA_LEADER,0,false);
  GainPermStati(HAS_ANIMAL,mn,SS_MONI,TA_LEADER,0,0);
  GainPermStati(TRIED,NULL,SS_MONI,ANIMAL_COMPANION,0,0);
  if (!mn->MakeCompanion(this,PHD_ANIMAL))
    {
      SetSilence();
      mn->Remove(true);
      UnsetSilence();
    }
  }

bool Creature::okWildShape(int16 lev, rID mID, rID tID)
  {    
    TMonster *tm = TMON(mID);
    static rID direID = 0;
    /* Here's the progression:
       Level 3: Medium-Size or Small non-flying mammals of
                  your level or less.
       Level 4: Allow birds / flying animals
       Level 5: Allow reptiles
       Level 6: Allow Large animals
       Level 7: Allow tiny animals
       Level 9: Allow huge animals
       Level 12: Allow Medium elementals
       Level 16: Allow Large elementals
       Level 20: Allow Huge elementals

    Plant Shapes (min lev 6) -- allow Plant Creatures
    Mythic Shapes (min lev 8) -- allow Mythic Hybrids
    Vermin Shapes (min lev 10, kobold only) -- allow vermin
    Elementalkin Shapes (min lev 16) -- allow elementalkin

    BIG Assumption: We never send this function templates which
      change the type of the creature.

    */    
    
    int16 CR  = tm->CR;

/*
    if (direID == 0)
      direID = FIND("dire");
      */

    if (tID)
      CR = TTEM(tID)->CR.Adjust(CR);

  if (CR > lev || tm->isMType(mID,MA_DRAGON))
      return false;

  int16 HitDice = tm->HitDice;
  if (tID)
    HitDice = TTEM(tID)->HitDice.Adjust(HitDice);

  if (HitDice > lev + 1 ) return false; 

  if (tm->HasFlag(M_NOPOLY)) return false;

  if (tm->isMType(mID,MA_ANIMAL))
      goto typeOk;

  if (HasFeat(FT_WILD_SHAPE_PLANT) && tm->isMType(mID,MA_PLANT)) goto typeOk;

  // ww: mindless non-plants = not ok
  if (tm->HasFlag(M_MINDLESS)) return false;

  if (HasFeat(FT_WILD_SHAPE_MYTHIC) && tm->isMType(mID,MA_MYTHIC)) goto typeOk;
  if (HasFeat(FT_WILD_SHAPE_VERMIN) && tm->isMType(mID,MA_VERMIN)) goto typeOk;
  if (HasFeat(FT_WILD_SHAPE_ELEMENTALKIN) && tm->isMType(mID,MA_ELEMENTALKIN)) goto typeOk;

    #if 0
    if (HasFeat(FT_MYTHIC_SHAPES))
      if (tm->isMType(MA_MYTHIC))
        goto typeOk;
    if (HasFeat(FT_DRAGON_SHAPES))
      if (tm->isMType(MA_DRAGON))
        { lev -= 3; goto typeOk; }
    if (HasFeat(FT_VERMIN_SHAPES))
      if (tm->isMType(MA_VERMIN))
        goto typeOk;
    if (HasFeat(FT_PLANT_SHAPES))
      if (tm->isMType(MA_PLANT))
        goto typeOk;
    if (HasFeat(FT_BEAST_SHAPES))
      if (tm->isMType(MA_BEAST) && !tm->isMType(MA_SAPIENT))
        goto typeOk;
    #endif

  if (tm->isMType(mID,MA_ELEMENTAL))
      if (lev >= 12 + max(0,tm->Size - SZ_MEDIUM)*4)
        goto typeOk;

    return false;

    typeOk:

    if (tm->HasFlag(M_FLYER))
      if (lev < 4)
        return false;
  if (tm->isMType(mID,MA_REPTILE))
      if (lev < 5)
        return false;
    switch (tID ? TTEM(tID)->Size.Adjust(tm->Size) : tm->Size)
      {
        case SZ_MEDIUM: case SZ_SMALL:
          return true;
        case SZ_LARGE:
          return lev >= 6;
        case SZ_TINY: 
          return lev >= 7;
        case SZ_HUGE:
          return lev >= 8;
        default:
          return false;
      }

    return false;

  }

bool Creature::okAnimalComp(int16 lev, rID mID, rID tID)
  {    
    TMonster *tm = TMON(mID);
    static rID direID = 0;

    int16 CR  = tm->CR;

    if (direID == 0)
      direID = FIND("dire");

    if (tID)
      CR = TTEM(tID)->CR.Adjust(CR);

    if (CR > lev)
      return false;

    if (!tm->isMType(mID,MA_ANIMAL))
      return false;
    if (tm->HasFlag(M_NOGEN) || tm->HasFlag(M_UNIQUE))
      return false;
    // ww: sorry, no animal companions that can hold the +5 sword of
    // death for you ...
    if (!tm->HasFlag(M_NOHANDS))
      return false;
    if (tm->isMType(mID,MA_SAPIENT))
      return false;
  
    return true;
  }

bool Creature::okBondedMount(int16 lev, rID mID, rID tID)
  {    
    TMonster *tm = TMON(mID);
    static rID celID = 0;

    int16 CR   = tm->CR,
          Size = tm->Size;

    if (celID == 0)
      celID = FIND("celestial");

    if (!tm->isMType(mID,MA_MOUNT))
      return false;

    if (tID) {
      CR = TTEM(tID)->CR.Adjust(CR);
      Size = TTEM(tID)->Size.Adjust(Size);
      }

    /* Bonded mounts by default allow a wider range of creatures (i.e.,
         mythic beasts, dragons, celestial animals, etc.) then do animal
         companions; furthermore, they can be ridden, which is a tactical
         advantage. To preserve druid and ranger uniqueness, we thus make
         them somewhat less powerful. */
    if (CR-2 > (lev*2)/3)
      return false;

    if (!(tm->isMType(mID,MA_ANIMAL) || tm->isMType(mID,MA_MYTHIC) || tm->isMType(mID,MA_DRAGON)
           || (tm->isMType(mID,MA_KOBOLD) && tm->isMType(mID,MA_VERMIN))))
      return false;

    if (tm->HasFlag(M_NOGEN) || tm->HasFlag(M_UNIQUE))
      return false;

    /* Summon a mount appropriate to your size */
    /* ww: no -- let the player see the full list. They won't be able to
     * ride it anyway, so it's not a problem. 
    if (tm->Size < Attr[A_SIZ]+1)
      return false;
    if (Size > Attr[A_SIZ]+1)
      if (!tm->isMType(MA_SAPIENT))
        return false; */

    // ww: sorry, no animal companions that can hold the +5 sword of
    // death for you ...
    if (!tm->HasFlag(M_NOHANDS))
      return false;
    
    /* Sapient mounts (dragons, etc.) must be good-aligned. */
    if (tm->isMType(mID,MA_SAPIENT))
      if (!tm->HasFlag(M_GOOD))
        return false;
  
    return true;
  }

bool Creature::ItemPrereq(rID xID, int16 ReqLevel, int16 TrickDC)
  {
    #define IS_VAL(xID,v) (xID >= v && xID <= (v+999))
    if (HasAbility(CA_TRICK_MAGIC))
      if (SkillLevel(SK_BLUFF) + SkillLevel(SK_USE_MAGIC) >= TrickDC)
        return true;
  
    if (IS_VAL(xID,ABIL_VAL))
      if (HasAbility((int16)(xID - ABIL_VAL)))
        if (AbilityLevel((int16)(xID - ABIL_VAL)) >= ReqLevel)
          return true;
          
    if (IS_VAL(xID,FEAT_VAL))
      if (HasFeat((int16)(xID - FEAT_VAL)))
        return true;
        
    if (IS_VAL(xID,SKILL_VAL))
      if (HasSkill((int16)(xID - SKILL_VAL)))
        if (SkillLevel((int16)(xID - SKILL_VAL)) >= ReqLevel)
          return true;
    
    if (IS_VAL(xID,MTYPE_VAL))
      if (isMType(xID - MTYPE_VAL))
        return true;
        
    if (IS_VAL(xID,MFLAG_VAL))
      if (HasMFlag((int16)(xID - MFLAG_VAL)))
        return true;
        
    if (IS_VAL(xID,ATTR_VAL))
      {
        ASSERT((xID - ATTR_VAL) <= A_LUC)
        return GetAttr((int8)(xID - ATTR_VAL)) >= ReqLevel;
      }
      
    if (IS_VAL(xID,CLEV_VAL)) {
      int16 clev[6], j, types[] = {
        SP_ARCANE, SP_DIVINE, SP_BARDIC, SP_SORCERY, SP_PRIMAL, 0 };
      memset (clev,0,sizeof(int16)*6);
      StatiIterNature(this,SPELL_ACCESS)
        for (j=0;types[j];j++)
          if (S->Val & (xID - CLEV_VAL) & types[j])
            clev[j] = max(clev[j],S->Mag);
      StatiIterEnd(this);
      return (clev[0] + clev[1] + clev[2] +
              clev[3] + clev[4]) >= ReqLevel;
      }        
        
    if (xID > 0xFFFF)
      switch (RES(xID)->Type)
        {
          case T_TMONSTER:
            return tmID == xID;
          case T_TGOD:
            if (isCharacter())
              if (thisc->GodID == xID)
                return true;
            //if HasEffStati(MINION_OF,xID)
            //  return true;
            return false; 
          case T_TCLASS:
            if (isCharacter())
              if (thisc->LevelAs(xID) >= ReqLevel)
                return true;
            // later, check for class templates, etc.
            return false;
          case T_TRACE:
            if (isCharacter())
              if (thisc->RaceID == xID)
                return true;
            return false;
          case T_TTEMPLATE:
            return HasEffStati(TEMPLATE,xID);
          case T_TREGION:
            if (m && m->RegionAt(x,y) == xID)
              return true;
            return false;
          case T_TTERRAIN:
            if (m && m->TerrainAt(x,y) == xID)
              return true;
            return false;     
        }
        
    return false;     
  }
  
int16 Creature::getBestKnowSkill(Creature *cr, bool isCombat)
  {
    int16 best;
    best = 0;
    if (cr->isMType(MA_UNDEAD) && HasSkill(SK_KNOW_UNDEAD))
      best = max(best,SkillLevel(SK_KNOW_UNDEAD));
    if (cr->isMType(MA_AQUATIC) && HasSkill(SK_KNOW_OCEANS))
      best = max(best,SkillLevel(SK_KNOW_OCEANS));
    if (cr->isMType(MA_OUTSIDER) && HasSkill(SK_KNOW_PLANES))
      if (!(cr->isMType(MA_DEMON) || cr->isMType(MA_DEVIL) || 
                                     cr->isMType(MA_CELESTIAL)))
        best = max(best,SkillLevel(SK_KNOW_PLANES));
    if ((cr->isMType(MA_DEMON) || cr->isMType(MA_DEVIL)) && 
                                          HasSkill(SK_KNOW_INF))
      best = max(best,SkillLevel(SK_KNOW_INF));
    if ((cr->isMType(MA_PLANT) || cr->isMType(MA_ANIMAL)) &&
                                          HasSkill(SK_KNOW_NATURE))
      best = max(best,SkillLevel(SK_KNOW_NATURE));
    if (cr->isMType(MA_MYTHIC) && HasSkill(SK_KNOW_MYTH))
      best = max(best,SkillLevel(SK_KNOW_MYTH));
    if (cr->isMType(MA_NLIVING) && cr->HasMFlag(M_HUMANOID))
      if (HasSkill(SK_FIND_WEAKNESS) && isCombat)
        best = max(best,SkillLevel(SK_FIND_WEAKNESS));
    return best;
  }
