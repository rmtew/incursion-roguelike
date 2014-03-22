/* SHEET.CPP -- Copyright (c) 1999-2003 Julian Mensch
     This file contains the functions for composing the character
   sheet as an abstract memory structure, and for dumping the
   character to a file.
     
     void TextTerm::CreateCharSheet(CharSheet &cs)
     static void ListInventory(String &is, Item *i, int depth, bool descend)
     String Detable(String s, int8 mode)
     void TextTerm::CreateCharDump(String &str)
     void TextTerm::DumpCharacter(String filebase)
     
*/

#include "Incursion.h"

void TextTerm::CreateCharSheet(CharSheet &cs)
{
  int16 i, j, k, first; String str, str2; String skname;
  bool HasInnate = false , HasResist = false, HasImmunity = false; 

  p->CalcValues(true);

  cs.Banner = Format("%c%s, ", -EMERALD, (const char*)p->Named);
  cs.Banner += PersonalityNames[p->Personality];
  if (p->WizardMode)
    cs.Banner += Format(" %c(Wizard Mode)%c",-PINK,-EMERALD);
  if (p->ExploreMode)
    cs.Banner += Format(" %c(Explore Mode)%c",-SKYBLUE,-EMERALD);
  
  if (p->VictoryFlag)
    cs.Banner += Format(" %c%s%c\n",-YELLOW,"[WINNER]",-WHITE);
  cs.Banner += "\n" ; 

  cs.Basics = Format("%cGender %c%s\n",-WHITE,-PURPLE,
      p->StateFlags & MS_FEMALE ? "Female" : "Male");
  cs.Basics += Format("%cRace   %c%s\n",-WHITE,-PURPLE,
      NAME(p->RaceID));
  StatiIterNature(p,TEMPLATE)
      cs.Basics += Format("%cTemp.  %c%s\n",-WHITE,-PURPLE,
          (const char*) RES(S->eID)->GetName(0).Capitalize());
  StatiIterEnd(p)
  if (p->HasStati(POLYMORPH))
    cs.Basics += Format("%cBody   %c%s\n",-WHITE,-PURPLE,
        (const char*) RES(p->mID)->GetName(0).Capitalize());
  cs.Basics += Format("%cClass  %c%s %c%d\n",-WHITE,-PURPLE,
      NAME(p->ClassID[0]),-MAGENTA,p->Level[0]);
  if (p->ClassID[1]) 
    cs.Basics += Format("       %c%s %c%d\n",-PURPLE,
        NAME(p->ClassID[1]),-MAGENTA,p->Level[1]);
  if (p->ClassID[2]) 
    cs.Basics += Format("       %c%s %c%d\n",-PURPLE,
        NAME(p->ClassID[2]),-MAGENTA,p->Level[2]);
  if (p->GodID)
    cs.Basics += Format("%cFaith  %c%s\n",-WHITE,-PURPLE,
        NAME(p->GodID));
  cs.Basics += Format("%cXP     %c%d\n",-WHITE,-PURPLE,
      max(0,p->XP - p->XP_Drained));
  if (p->XP_Drained) 
    cs.Basics += Format("%c Drain %c%d\n",-WHITE,-PURPLE,
        p->XP_Drained);
  if (p->XPPenalty() > 0)
    cs.Basics += Format("%c Penal %c%d~\n",-WHITE,-PURPLE,
        p->XPPenalty());
  else if (p->XPPenalty() < 0)
    cs.Basics += Format("%c Bonus %c%d~\n",-WHITE,-PURPLE,
        -p->XPPenalty());
  cs.Basics += Format("%cNext @ %c%d\n",-WHITE,-PURPLE,
      p->NextLevXP());
  if (p->HasStati(MOUNTED))
    cs.Basics += Format("%cMount  %c",-WHITE,-PURPLE) +
      Left(Capitalize(p->GetStatiObj(MOUNTED)->Name(0),true),14) + "\n";

    cs.Basics += Format("%cAlign  %c",-WHITE,-PURPLE);
    for (i=0;i!=9;i++)
      if (AlignmentInfo[i].align == p->GetStatiVal(ALIGNMENT))
        cs.Basics += AlignmentInfo[i].name;
    cs.Basics += "\n"; 
  
  /* ww: skipping reputation and leadership at this point */
  /* ww: skipping 'burden', which was always 0.0 */

  cs.Basics += "\n" ; 

  for(i=0;i!=7;i++)
    {
      int16 percentile, j;
      percentile = 0;
      for (j=0;j!=10;j++)
        percentile += p->GainAttr[i][j];
      cs.sAttr[i].Empty();
      cs.sAttr[i] += Format("%c%s: %c%2d/%s%d %c[%+d/%+d] %c%c%s",
        -WHITE,
        AttrTitle[i],
        -MAGENTA,
        p->KAttr[i],
        percentile >= 10 ? "" : "0",
        percentile,
        -PURPLE,
        p->Mod(i),p->Mod2(i),
        -GREY, WRAP_INDENT,
        (const char*) p->BonusBreakdown(i,60));
    }

  for(i=0;i!=3;i++)
    {
      cs.sSaves[i].Empty();
      char *save_name[3] = { "Fortitude", "Reflex", "Will" };
      cs.sSaves[i] = Format("%c%-9s %c%+4d%c%c%s",
        -WHITE,
        save_name[i], 
        -MAGENTA,
        p->KAttr[A_SAV_FORT+i],
        -GREY, WRAP_INDENT,
        (const char*) p->BonusBreakdown(A_SAV_FORT+i, 60));
    }

  {
    int16 b;    
    for (i=0;i!=31;i++)
      {
        b = 0;
        StatiIterNature(p,SAVE_BONUS)
          if (i == S->Val)
            b += S->Mag;
        StatiIterEnd(p)
        if (i == SN_THEFT)
          if (p->HasSkill(SK_PICK_POCKET))
            b += p->SkillLevel(SK_PICK_POCKET) / 3;
        if (i == SN_POISON) 
          if (p->HasSkill(SK_POISON_USE))
            b += p->SkillLevel(SK_POISON_USE) / 4;
        if (i == SN_POISON || i == SN_DISEASE)
          if (p->HasFeat(FT_HARDINESS))
            b += 2;
        if (i == SN_KNOCKDOWN)
          if (p->HasSkill(SK_BALANCE))
            b += p->SkillLevel(SK_BALANCE) / 2;
        if (b) {
          if (str2.GetLength())
            str2 += ", ";
          str2 += Format("%+d vs. %s",b,
            Lookup(SaveBonusNames, i));
          }
      } 
    if (str2.GetLength())
      cs.sSaves[2] += Format("\n           %c(%s)",
        WRAP_INDENT,(const char*)str2);
  } 


  for(i=0;i!=5;i++)
  {
    static char* STypes[] = {
      "Archery", "Brawl  ", "Melee  ",  "Thrown ", "Offhand" };
    if ((i == S_DUAL) && !p->offhandWep)
      continue;
    if ((i == S_ARCHERY) && !p->missileWep)
      continue;
    if ((i == S_MELEE) && !p->meleeWep)
      continue;
    if ((i == S_THROWN) && !p->thrownWep)
      continue;  
    if ((i == S_BRAWL) && p->InSlot(SL_WEAPON) && 
        p->InSlot(SL_WEAPON)->isType(T_WEAPON))
      {
        cs.sCombat[i][0].Empty();
        cs.sCombat[i][1].Empty();
        cs.sCombat[i][2].Empty();
        continue;
      }


    cs.sCombat[i][0] =  Format("%c%s %c+toHit%c ", 
                          -AZURE,STypes[i],
                          -YELLOW,
                          -GREY);
    cs.sCombat[i][0] += Format("%c%+-3d %c%c%s\n",
                          -MAGENTA,
                          p->KAttr[A_HIT_ARCHERY+i],
                          -GREY, WRAP_INDENT,
                          (const char*) p->BonusBreakdown(A_HIT_ARCHERY+i));

    cs.sCombat[i][1] =  Format("        %cDamage%c ",
                          -YELLOW,
                          -GREY);
    cs.sCombat[i][1] += Format("%c%+-3d %c%c%s\n",
                          -MAGENTA,
                          p->KAttr[A_DMG_ARCHERY+i],
                          -GREY, WRAP_INDENT,
                          (const char*) p->BonusBreakdown(A_DMG_ARCHERY+i));

    cs.sCombat[i][2] =  Format("        %cSpeed%c  ",
                          -YELLOW,
                          -GREY);
    cs.sCombat[i][2] += Format("%c%d~%s%c%c%s\n",
                          -MAGENTA,
                          p->KAttr[A_SPD_ARCHERY+i]*5 + 100,
                          p->KAttr[A_SPD_ARCHERY+i] >= 0 ? "" : " ",
                          -GREY, WRAP_INDENT,
                          (const char*) p->BonusBreakdown(A_SPD_ARCHERY+i));

  }

  cs.sSize    = Format("%cSize           %c%-3s%c%c%s\n",
                  -YELLOW, -MAGENTA, 
                  (const char*)
                  SC(Lookup(SizeNames,p->KAttr[A_SIZ])).Capitalize(), 
                          WRAP_INDENT,
                  -GREY, (const char*) p->BonusBreakdown(A_SIZ));

  cs.sDefense = Format("%cDefense Class  %c%-3d %c%c%s\n",
                  -YELLOW, -MAGENTA, p->KAttr[A_DEF], WRAP_INDENT,
                  -GREY, (const char*) p->BonusBreakdown(A_DEF));
  if (p->CasterLev())
    cs.sDefense += Format("_______________(%d while casting)\n",p->KAttr[A_CDEF]);

  cs.sCoverage = Format("%cCoverage       %c%-3d %c%c%s\n",
                  -YELLOW, -MAGENTA, p->KAttr[A_COV], WRAP_INDENT,
                  -GREY, (const char*) p->BonusBreakdown(A_COV));

  cs.sMove    = Format("%cMovement Rate  %c%d~%s%c%c%s\n",
                  -YELLOW, -MAGENTA, 
                  p->KAttr[A_MOV]*5+100, 
                  p->KAttr[A_MOV] >= 0 ? "" : " ", WRAP_INDENT,
                  -GREY, (const char*) p->BonusBreakdown(A_MOV));
  p->Encumbrance();
  cs.sEncum   = Format("%cEncumbrance    %c%s%c%c (Light %d.%ds, Moderate %d.%ds, "
                       "Heavy %d.%ds, Extreme %d.%ds, Max Press %d.%ds)\n",                   
                -YELLOW, -MAGENTA, 
                EncumbranceNames[p->Encumbrance()],
                WRAP_INDENT, -GREY,
                EncumValues[0] / 10, EncumValues[0] % 10, 
                EncumValues[1] / 10, EncumValues[1] % 10, 
                EncumValues[2] / 10, EncumValues[2] % 10, 
                EncumValues[3] / 10, EncumValues[3] % 10, 
                p->MaxPress() / 10,  p->MaxPress() % 10);
                
  cs.sHitDice = Format("%cHit Dice Rolls %c%-3d%c%c", 
      -YELLOW,
      -MAGENTA,
      p->mHP + p->GetAttr(A_THP),
      WRAP_INDENT,
      -GREY);
  if (p->HasFeat(FT_TOUGHNESS))
    cs.sHitDice += " [20";
  else
    cs.sHitDice += " 20";

  int a_con = A_CON;
  bool one_body = p->HasFeat(FT_ONE_BODY_ONE_SOUL) || (p->Attr[A_CON] == 0);
  if (one_body && p->Mod(A_WIS) > p->Mod(A_CON))
    a_con = A_WIS;
  int HDType = p->BestHDType();

  if (!p->HasStati(POLYMORPH)) {
    int lev = p->Level[0] + p->Level[1] + p->Level[2]; 
    int templateHD = lev;
    StatiIterNature(p,TEMPLATE)
        templateHD = TTEM(S->eID)->HitDice.Adjust(templateHD);
    StatiIterEnd(p)
    templateHD -= lev;
    if (templateHD > 0) {
      cs.sHitDice += Format(" + %dd%d (templates)", templateHD, HDType);
    } else templateHD = 0; 

    if (HDType > 8) {
      cs.sHitDice += Format(" + %dd%d", lev , HDType);
    } else { 
    for(i=0;i!=3;i++)
      if (p->Level[i])
      {
        
        cs.sHitDice += SC(" + ") + Format("%dd%d",p->Level[i],
            /* Kludge -- thaumaturges get d8's, not d4's. */
            (p->HasEffStati(SPECIALTY_SCHOOL,p->ClassID[i],7/*SC_THA*/) ? 8 :
            TCLASS(p->ClassID[i])->HitDie)) + SC(" (");
        for(j=0;j!=p->Level[i];j++)
          cs.sHitDice += Format("%s%d",j ? ", " : "", 
              p->hpRolls[i][j]);
        cs.sHitDice += ")";

      }  
    } 
    if (p->Mod(a_con))
      cs.sHitDice += Format(" %c %d (%s bonus)", p->Mod(a_con) > 0 
          ? '+' : '-',(p->TotalLevel() + templateHD + 
            p->AbilityLevel(CA_TOUGH_AS_HELL)) * abs(p->Mod(a_con)),
          AttrTitle[a_con]);
  } else {
    int numHD = TMON(p->mID)->HitDice; 
    StatiIterNature(p,TEMPLATE)
        numHD = TTEM(S->eID)->HitDice.Adjust(numHD);
    StatiIterEnd(p)

    cs.sHitDice += Format(" + %dd%d", numHD, HDType);

    if (p->Mod(a_con))
      cs.sHitDice += Format(" %c %d (%s bonus)", p->Mod(a_con) > 0 
          ? '+' : '-',(numHD + 
            p->AbilityLevel(CA_TOUGH_AS_HELL)) * abs(p->Mod(a_con)),
          AttrTitle[a_con]);
  } 

  switch(p->Attr[A_SIZ]) {
    case SZ_MINISCULE:      cs.sHitDice += " x 50~ (Size)" ; break; 
    case SZ_TINY:           cs.sHitDice += " x 70~ (Size)" ; break;
    case SZ_SMALL:          cs.sHitDice += " x 90~ (Size)" ; break;
    case SZ_MEDIUM:         break;
    case SZ_LARGE:          cs.sHitDice += " x 110~ (Size)" ; break;
    case SZ_HUGE:           cs.sHitDice += " x 130~ (Size)" ; break;
    case SZ_GARGANTUAN:     cs.sHitDice += " x 160~ (Size)" ; break;
    case SZ_COLLOSAL:       cs.sHitDice += " x 200~ (Size)" ; break;
  }

  if (p->HasFeat(FT_TOUGHNESS))
    cs.sHitDice += "] + 25~ (Toughness)";
  if (p->GetAttr(A_THP))
    cs.sHitDice += p->BonusBreakdown(A_THP);

  cs.sFatigue = Format("%cFatigue Points %c%-3d %c%c%s\n",
      -YELLOW, -MAGENTA, p->KAttr[A_FAT], WRAP_INDENT, -GREY,
      (const char*) p->BonusBreakdown(A_FAT));

  cs.sMana = Format("%cMana Rolls     %c%-3d%c%c", 
                      -YELLOW,
                      -MAGENTA,
                      p->mMana,
                      WRAP_INDENT,
                      -GREY);

  cs.sMana += Format(" %d",10+p->AbilityLevel(CA_MAGICAL_NATURE));
          
  for(i=0;i!=3;i++)
    if (p->Level[i]) {
      cs.sMana += SC(" + ") + Format("%dd%d",p->Level[i],
          TCLASS(p->ClassID[i])->ManaDie) + SC(" (");
      for(j=0;j!=p->Level[i];j++)
        cs.sMana += Format("%s%d",j ? ", " : "", p->manaRolls[i][j]);
      cs.sMana += Format(") x %d", ManaMultiplier[p->TotalLevel()]);

    }  

  int a_wis = A_WIS;
  if (one_body & p->Mod(A_CON) > p->Mod(A_WIS))
    a_wis = A_CON;

  if (p->Mod(a_wis))
    cs.sMana += Format(" %c %d (%s bonus)", p->Mod(a_wis) > 0 
        ? '+' : '-',p->TotalLevel() * abs(p->Mod(a_wis)),
        AttrTitle[a_wis]);
  cs.sMana += "\n";
  // if you're been eating a dragon corpse or using an item of channeling,
  // tell us! Or getting a permanent penalty!
  if (p->Attr[A_MAN] != 0)
    cs.sMana += Format("__(Bonus Mana)______%s", (const char*)p->BonusBreakdown(A_MAN,60));

  if (p->KAttr[A_MR] != 0)
    cs.sMagicRes = Format("%cMagic Resist   %c%d~%s%c%c%s\n",
                  -YELLOW, -MAGENTA, 
                  p->KAttr[A_MR], 
                  p->KAttr[A_MR] >= 100 ? "" : " ", WRAP_INDENT,
                  -GREY, (const char*)p->BonusBreakdown(A_MR));
                  
  if (p->CasterLev()) {
    i = p->AbilityLevel(CA_SPELLCASTING);
    cs.sSpells[0] = Format("%cSpell Slots    %c%d%s%s%c%c", 
      -YELLOW,
      -MAGENTA,
      i, NumPrefix(i),
      i > 9 ? "" : " ",
      WRAP_INDENT,-GREY); 
    for(i=0;i!=9;i++) {
      if (!(p->SpellSlots[i] || p->BonusSlots[i]))
        break;
      if (i)
        cs.sSpells[0] += " / ";
      cs.sSpells[0] += Format("%d",p->SpellSlots[i]);
      if (p->BonusSlots[i])
        cs.sSpells[0] += Format("+%d",p->BonusSlots[i]);
      if (p->SpellSlots[i] + p->BonusSlots[i] > p->SpellsLearned[i])
        cs.sSpells[0] += Format(" (%c%d%c)", 
            -MAGENTA,
            (p->SpellSlots[i] + p->BonusSlots[i]) 
            - p->SpellsLearned[i], -GREY);
    }
    cs.sSpells[0] += "\n" ; 
  }

  cs.sSpecials = Format("%cSpecial Abilities:%c\n",-WHITE,-GREY);

  for(i=0;i!=CA_LAST;i++)
    if(k = p->AbilityLevel(i)) {
      const char* WSkillLevels[] = { NULL, "Specialist", "Mastery", 
        "High Mastery", "Grand Mastery", "Grand Mastery", "Grand Mastery" 
      };
      str = Lookup(ClassAbilities,i); 
      switch(i) {
        case CA_LEGEND_LORE:
        case CA_SPELLCASTING: 
          str += Format(" (%d%s)",k, NumPrefix(k));
          break;
        case CA_PSIONICS:
          str += Format(" (level %d)",k);
          break;
        case CA_WILD_SHAPE:
          if (k < 3) 
            str += Format(" (Level %d, Latent)",k);
          else 
            str += Format(" (%d HD)",k);
          break;
        case CA_UNARMED_STRIKE:
          str += Format(" (%dd%d)",
              MonkDamage[p->AbilityLevel(i)].Number,
              MonkDamage[p->AbilityLevel(i)].Sides);
          break;
          /*case CA_FAMILIAR:
            sprintf(&(Text[line++][2]),Lookup(ClassAbilities,i),"Pixel the cat");
            break;*/
        case CA_SNEAK_ATTACK:
          str = Format("Sneak Attack +%dd6",k);
          break;
        case CA_THIEF_IN_THE_NIGHT: 
          str += Format(" (%d~)", 
                  (p->AbilityLevel(i)*100) / p->TotalLevel());
         break;
        case CA_REGEN:
          str = Format("Regeneration (%d/round)",k);
          break;            

        case CA_TRACKING:
          str = Format("Tracking (%d feet)",k*10);
          break;            
        case CA_FAV_ENEMY:
          cs.sSpecials += "  Favored Enemies:\n" ; 
          StatiIterNature(p,FAV_ENEMY)
              if (LookupOnly(MTypeNames,S->Val))
                cs.sSpecials += Format("    %+d vs. %c%s%c\n",
                    CalcFEBonus(S->Val,S->Mag),
                    -WHITE,
                    (const char*)Pluralize(Lookup(MTypeNames,S->Val)),
                    -GREY);
          StatiIterEnd(p)
          continue;
        case CA_TATTOOS:
          cs.sSpecials += "  Mystic Tattoos:\n";
          for (i=0;i!=10 && p->Tattoos[i];i++)
            cs.sSpecials += Format("    %s", (const char*)NAME(p->Tattoos[i]));
          continue;
        case CA_GREATER_TURNING:
          {
            bool mentioned[MA_CHOICE5] = { false } ; 
            StatiIterNature(p,GTURN_ABILITY)
              if (!mentioned[S->Val]) {
                mentioned[S->Val] = true; 
              if (LookupOnly(MTypeNames,S->Val))
                cs.sSpecials += Format("  Greater Turn %c%s: %d%c\n",
                    -PURPLE,
                    (const char*)Pluralize(Lookup(MTypeNames,S->Val)),
                    p->SumStatiMag(GTURN_ABILITY,S->Val,0),
                    -GREY);                  
              }
            StatiIterEnd(p)
          continue; 
          }

        case CA_TURNING:
          {
            bool mentioned[MA_CHOICE5] = { false } ; 
            StatiIterNature(p,TURN_ABILITY)
                if (!mentioned[S->Val]) {
                  mentioned[S->Val] = true; 
                  if (LookupOnly(MTypeNames,S->Val))
                    cs.sSpecials += Format("  Turn %c%s: %d%c\n",
                        -WHITE,
                        (const char*)Pluralize(Lookup(MTypeNames,S->Val)),
                        p->SumStatiMag(TURN_ABILITY,S->Val,0),
                        -GREY);                  
                }
            StatiIterEnd(p)
            StatiIterNature(p,ALSO_TURN_MA_TYPE)
                if (!mentioned[S->Val]) {
                  mentioned[S->Val] = true; 
                  if (LookupOnly(MTypeNames,S->Val))
                    cs.sSpecials += Format("   and %c%s%c\n",
                        -WHITE,
                        (const char*)Pluralize(Lookup(MTypeNames,S->Val)),
                        -GREY);                  
                }
            StatiIterEnd(p)
            continue;
          }
        case CA_COMMAND:
          StatiIterNature(p,COMMAND_ABILITY)
              if (LookupOnly(MTypeNames,S->Val))
                cs.sSpecials += Format("  Command %c%s%c\n",
                    -WHITE,
                    (const char*)Pluralize(Lookup(MTypeNames,S->Val)),
                    -GREY);                  
          StatiIterEnd(p)
          continue;
        case CA_SMITE:
          StatiIterNature(p,SMITE_ABILITY)
              if (!S->Val)
                cs.sSpecials += Format("  Smite Enemies\n");
              else if (LookupOnly(MTypeNames,S->Val))
                cs.sSpecials += Format("  Smite %c%s%c\n",
                    -WHITE,
                    (const char*)Pluralize(Lookup(MTypeNames,S->Val)),
                    -GREY);
              else 
                cs.sSpecials += Format("  Smite %d?\n",S->Val);
          StatiIterEnd(p)
          continue;
        case CA_SPECIALIST:
          cs.sSpecials += Format("  Specialist:\n    %c%s%c\n", 
            -WHITE,
            SchoolNames[p->GetStatiVal(SPECIALTY_SCHOOL)].Text,
            -GREY);
          continue;
        case CA_STORYCRAFT:
          if (p->tStoryPluses > p->aStoryPluses)
            str += Format(" (%+d unallocated)",p->tStoryPluses - p->aStoryPluses);
         break;
        case CA_WEAPON_IMMUNITY:
          if (k <= 1) 
            str += ": non-magical weapons";
          else
            str += Format(": +0-%+d weapons",k-1);
          break;

        case CA_WEAPON_SKILL:
          str += Format(":\n  %s",WSkillLevels[min(k,6)]);
         break;
        case CA_LORE_OF_ARMS:
          str += Format(" (%d hit%s)",NeededSwings[ min(9,k) ],
            NeededSwings[ min(9,k) ] > 1 ? "s" : "");
         break; 
        case CA_INFRAVISION: case CA_LOWLIGHT:
        case CA_TELEPATHY:   case CA_BLINDSIGHT:
        case CA_SCENT:       case CA_TREMORSENSE:
          str += Format(" (%d ft)",k*10);
          break;
        case CA_INHERANT_POTENTIAL:
        case CA_SHARP_SENSES:
        case CA_ARCANE_WILL: 
        case CA_KI_STRIKE: 
        case CA_MAGICAL_NATURE:
        case CA_STONEWORK_SENSE:
        case CA_RETRIBUTIVE_STRIKE:
          str += Format(" %+d",k);
          break;
        case CA_STUN_ATTACK:
          str += Format(" (DC %d)",10+k/2+p->Mod(A_WIS)+(p->HasFeat(FT_BREAK_THE_BREATH) ? 2 : 0));
          break;
        case CA_DOMAINS:
          cs.sSpecials += "  Domains:\n"; 
          StatiIterNature(p,HAS_DOMAIN)
              cs.sSpecials += Format("    %s\n", NAME(S->eID));
          StatiIterEnd(p)
          continue;
        case CA_SPECIES_AFFINITY:
          cs.sSpecials += "  Species Affinity:\n" ; 
          StatiIterNature(p,SPECIES_AFFINITY)
              if (LookupOnly(MTypeNames,S->Val))
                cs.sSpecials += Format("    %c%s%c\n",
                    -WHITE,
                    (const char*)Pluralize(Lookup(MTypeNames,S->Val)),
                    -GREY);
          StatiIterEnd(p)
          continue;
        case CA_BARDIC_MUSIC:
          cs.sSpecials += "  Bardic Music:\n    Inspire Courage\n" ;
          if (p->AbilityLevel(i) >= 3) cs.sSpecials += "    Fascination\n";
          if (p->AbilityLevel(i) >= 5) cs.sSpecials += "    Countersong\n";
          if (p->AbilityLevel(i) >= 7) cs.sSpecials += "    Inspire Competence\n";
          if (p->AbilityLevel(i) >= 9) cs.sSpecials += "    Hymn of the Phoenix Arisen\n";
          if (p->AbilityLevel(i) >= 10) cs.sSpecials += "    Mass Fascination\n";
          if (p->AbilityLevel(i) >= 11) cs.sSpecials += "    Spellbreaker Chant\n";
          continue; 
        case CA_UNCANNY_DODGE:
          if (k >= 4)
            str = SC("Improved ") + str;
          break;
        case CA_EVASION:
          if (k >= 9)
            str = SC("Improved ") + str;
          break;
        case CA_INNATE_SPELL:
          HasInnate = false;
          for(j=0;j!=MAX_SPELLS;j++)
            if (p->Spells[j] & SP_INNATE) {
              if (!HasInnate)
                cs.sSpecials += "  Innate Spells:\n" ;
              HasInnate = true;
              cs.sSpecials += Format("    %s\n", NAME(theGame->SpellID(j)));
            }
          continue;
        case CA_DEVOURING:
        //case CA_DISARM_MAGIC_TRAP:
        case CA_NATURE_SENSE:
        case CA_WOODLAND_STRIDE:
        case CA_DIVINE_GRACE:
        case CA_UNEARTHLY_LUCK:
        case CA_VERSATILITY:
        case CA_FATESENSE:
          break; 
        case CA_PREPATORY_MAGIC:
          if (k) str += Format(" (-%d%%)",k*5);
          break;
        case CA_COMMAND_AUTHORITY:
          if (k) str += Format(" (+%d CR)",k);
          break;
        case CA_NEGATE_CRIT:
          if (k) str += Format(" (%d%%)",k*5);
          break;
        case CA_UNBIND:
          str += Format(" (%d feet)", max(1,k+p->Mod(A_CHA))*10);
        default: 
          ;

      }
      cs.sSpecials += SC("  ") + str + SC("\n"); 
    }
  StatiIter(p)
    k = S->Mag; 
    
    /* This is for effects that are actually constant, non-
       spell-like special abilities, such as a paladin's 
       Aura of Valor, a rogue's Crippling Strike, a 
       celestial's Aura of Menace, etc. */
    if (S->eID && (RES(S->eID)->Type == T_TEFFECT))
      if (TEFF(S->eID)->HasFlag(EF_SPECABIL))
        if (S->Source == SS_CLAS || S->Source == SS_RACE)
          if (S->Duration == 0)
            {
              EventInfo xe;
              xe.Clear();
              xe.EActor = p;
              xe.EVictim = p;
              xe.nBase = NAME(S->eID);
              TEFF(S->eID)->Event(xe,S->eID,EV_GETNAME);
              cs.sSpecials += Format("  %s\n", (const char *)
                (xe.nPrefix + xe.nBase + xe.nAppend + xe.nMech));
              continue;
            }
          
    switch (S->Nature) {
      case GAZE_REFLECTION: 
        switch (S->Val) {
          case GR_IMMUNE: cs.sSpecials += ("  Gaze Immunity\n");break;
          case GR_REFLECT: cs.sSpecials += ("  Gaze Reflection\n");break;
          default: cs.sSpecials += ("  ?Gaze Reflection?\n");break;
        }
        break; 
      case WATER_WALKING: 
        cs.sSpecials += ("  Water Walking\n");
        break;
      case SEE_INVIS: 
        cs.sSpecials += ("  See Invisibility\n");
        break;
      case MISS_CHANCE: 
        cs.sSpecials += Format("  %s: %d~\n",NAME(S->eID),k);
        break;
      case DWARVEN_FOCUS: 
        cs.sSpecials += Format("  Dwarven Focus: %c%s%c\n",-WHITE,
            (const char*)oCreature(S->h)->Name(0),
            -GREY);
        break;
      case NEUTRAL_TO:
        cs.sSpecials += Format("  Neutral: %c%s%c\n",-WHITE,
            (const char*)Pluralize(Lookup(MTypeNames,S->Val),0),
            -GREY);
        break; 
      case ENEMY_TO:
        cs.sSpecials += Format("  Aggravate: %c%s%c\n",-WHITE,
            (const char*)Pluralize(Lookup(MTypeNames,S->Val),0),
            -GREY);
        break; 
      case ALLY_TO:
        cs.sSpecials += Format("  Ally: %c%s%c\n",-WHITE,
            (const char*)Pluralize(Lookup(MTypeNames,S->Val),0),
            -GREY);
        break; 
      case BONUS_SCHOOL_CASTING_LEVEL: 
        cs.sSpecials += Format("  Caster Level: %c%s %+d%c\n",-WHITE,
            (const char*)Lookup(SchoolNames, S->Val),
            S->Mag,
            -GREY);
        break; 
      case ADDITIONAL_WILD_SHAPE_MA_TYPE:
        cs.sSpecials += Format("  Wild Shape: %c%s%c\n",-WHITE,
            (const char*)Pluralize(Lookup(MTypeNames,S->Val),0),
            -GREY);
        break; 

    }
  StatiIterEnd(p)

  cs.sPerception = Format("%cPerception and Vision:%c\n",-WHITE,-GREY);
  if (p->LightRange) cs.sPerception += 
    Format("  Light Range:        %3d ft\n", p->LightRange * 10);
  if (p->ShadowRange) cs.sPerception += 
    Format("  Shadow Range:       %3d ft\n", p->ShadowRange * 10);
  if (p->SightRange) cs.sPerception += 
    Format("  Sight Range:        %3d ft\n", p->SightRange * 10);
  if (p->InfraRange) cs.sPerception += 
    Format("  Infravision Range:  %3d ft\n", p->InfraRange * 10);
  if (p->TelepRange) cs.sPerception += 
    Format("  Telepathy Range:    %3d ft\n", p->TelepRange * 10);
  if (p->BlindRange) cs.sPerception += 
    Format("  Blindsight Range:   %3d ft\n", p->BlindRange * 10);
  if (p->TremorRange) cs.sPerception += 
    Format("  Tremorsense Range:  %3d ft\n", p->TremorRange * 10);
  if (p->ScentRange) cs.sPerception += 
    Format("  Scent Range:        %3d ft\n", p->ScentRange * 10);
  if (p->PercepRange) cs.sPerception += 
    Format("  Wizard Sight Range: %3d ft\n", p->PercepRange * 10);
  if (p->AbilityLevel(CA_TRACKING)) cs.sPerception +=
    Format("  Tracking Range:     %3d ft\n", p->AbilityLevel(CA_TRACKING) * 10);
  StatiIterNature(p,DETECTING)
        cs.sPerception += Format("  Detect: %c%s%c (%d ft)\n",-WHITE,
            S->Val ? 
            (const char*)Pluralize(Lookup(MTypeNames,S->Val),0)
            : "Monsters" 
            ,
            -GREY, S->Mag * 10);
  StatiIterEnd(p)
  StatiIterNature(p,DETECTING_TYPE)
        cs.sPerception += Format("  Detect: %c%s%c (%d ft)\n",
            -WHITE,
            S->Val != T_ITEM ?
            (const char*)Lookup(ITypeNames,S->Val)
            : "Items",
            -GREY, 
            S->Mag * 10);
  StatiIterEnd(p)

  if (p->HasStati(SEE_INVIS) || p->HasMFlag(M_SEE_INVIS)) 
    cs.sPerception += ("  Sees Invisibility\n");
  if (p->HasAbility(CA_NATURE_SENSE))
    cs.sPerception += ("  Sees Through Obscuring Terrain\n");

  if (p->HasStati(NONDETECTION)) { 
    i = p->GetStatiVal(NONDETECTION);
    if (i & PER_VISUAL) cs.sPerception += "  Nondetection: Visual\n";
    if (i & PER_INFRA) cs.sPerception += "  Nondetection: Infravision\n";
    if (i & PER_SCENT) cs.sPerception += "  Nondetection: Scent\n";
    if (i & PER_BLIND) cs.sPerception += "  Nondetection: Blindsight\n";
    if (i & PER_PERCEPT) cs.sPerception += "  Nondetection: Wizard Sight\n";
    if (i & PER_TREMOR) cs.sPerception += "  Nondetection: Tremorsense\n";
    if (i & PER_SHADOW) cs.sPerception += "  Nondetection: Shadow\n";
    if (i & PER_TELE) cs.sPerception += "  Nondetection: Telepathy\n";
    if (i & PER_DETECT) cs.sPerception += "  Nondetection: Detection\n";
    if (i & PER_TRACK) cs.sPerception += "  Nondetection: Tracking\n";
  } 

  if (p->m->FieldAt(p->x,p->y,FI_LIGHT) ||
      p->m->FieldAt(p->x,p->y,FI_SILENCE) ||
      p->m->FieldAt(p->x,p->y,FI_FOG) ||
      p->m->FieldAt(p->x,p->y,FI_DARKNESS) ||
      p->m->FieldAt(p->x,p->y,FI_SHADOW)) {
    cs.sPerception += Format(" \n%cInside Fields:%c\n",-WHITE,-GREY);
    if (p->m->FieldAt(p->x,p->y,FI_LIGHT))
      cs.sPerception += "  Light\n";
    if (p->m->FieldAt(p->x,p->y,FI_SILENCE))
      cs.sPerception += "  Silence\n";
    if (p->m->FieldAt(p->x,p->y,FI_FOG))
      cs.sPerception += "  Fog\n";
    if (p->m->FieldAt(p->x,p->y,FI_DARKNESS))
      cs.sPerception += "  Darkness\n";
    if (p->m->FieldAt(p->x,p->y,FI_SHADOW))
      cs.sPerception += "  Shadow\n";
  } 

  if (p->AutoBuffs[0])
    {
      cs.sAutoBuff = Format("%cAutoBuff List:%c\n",-WHITE,-GREY);
      for(i=0;p->AutoBuffs[i];i++)
        cs.sAutoBuff += Format("  %s\n",
          NAME(theGame->SpellID(p->AutoBuffs[i])));
    }
  else
    cs.sAutoBuff = "";

  cs.sFeats = Format("%cFeats:%c\n",-WHITE,-GREY);

  for(i=FT_FIRST;i!=FT_LAST;i++)
    if (p->HasFeat(i,false,true)) 
      switch (i) {
        case FT_INTENSIVE_STUDY:
          cs.sFeats += "  Intensive Study:\n";
          if (p->IntStudy[STUDY_BAB])
            cs.sFeats += Format("    %s %+d\n",
              Lookup(StudyNames,STUDY_BAB), p->IntStudy[STUDY_BAB]);
          for (j=0;Studies[j][0];j++)
            if (p->IntStudy[Studies[j][0]])
              cs.sFeats += Format("    %s %+d\n",
                Lookup(StudyNames,Studies[j][0]),
                p->IntStudy[Studies[j][0]] * Studies[j][2]);
         break; 
        case FT_SCHOOL_FOCUS: 
          cs.sFeats += "  School Focus:\n" ;
          StatiIterNature(p,SCHOOL_FOCUS)
              cs.sFeats += Format("    %s\n", Lookup(SchoolNames, S->Mag));
          StatiIterEnd(p)
          break; 

        case FT_SKILL_FOCUS: 
          cs.sFeats += "  Skill Focus:\n" ; 
          StatiIterNature(p,SKILL_BONUS)
              if (S->Source == SS_PERM)
              cs.sFeats += Format("    %s\n", 
                  SkillInfo[ S->Val ] .name);
          StatiIterEnd(p)
          break; 

        default: 
          cs.sFeats += Format("  %s\n", FeatName(i));
      } 

  cs.sResists = Format("%cResistances and Armor:%c\n",-WHITE,-GREY);

  int8 ResistsShown[] = {
    AD_SLASH, AD_PIERCE, AD_BLUNT, 
    AD_FIRE, AD_COLD, AD_SONI, AD_ACID, AD_ELEC, AD_TOXI, AD_NECR, AD_PSYC,
    AD_MAGC, AD_SLEE, AD_STUN, AD_SLOW, AD_PLYS, AD_STUK, AD_STON, AD_POLY,
    AD_CHRM, AD_DISN, AD_CRIT, AD_DISE, AD_FEAR, AD_RUST, AD_HOLY, AD_EVIL,
    AD_LAWF, AD_CHAO, AD_SOAK, AD_SUNL, AD_DCAY, 0 
  };

  for (i=0;ResistsShown[i];i++)
    if (p->ResistLevel(ResistsShown[i]))
    {

      if (p->ResistLevel(ResistsShown[i]) == -1)
        HasImmunity = true;
      else {
        cs.sResists += Format("  %s %d\n", Lookup(DTypeNames,ResistsShown[i]),
            p->ResistLevel(ResistsShown[i]));
        HasResist = true;
      }
    }
  if (!HasResist)
    cs.sResists += "  None\n" ; 

  cs.sImmune.Empty();
  if (HasImmunity) {
    cs.sImmune = Format("%cComplete Immunities:%c\n",-WHITE,-GREY);

    for (i=0;ResistsShown[i];i++)
      if (p->ResistLevel(ResistsShown[i]) == -1)
        cs.sImmune += Format("  %s\n",Lookup(DTypeNames,ResistsShown[i]));
  }

  cs.sProfs = Format("%cProficiencies %c%c",-YELLOW,WRAP_INDENT,-GREY);

  /* j will count the number of prof atoms */
  j = 0; 
  for(i=0;i!=32;i++)
    if (p->Proficiencies & (1L << i))
      if (LookupOnly(WeaponGroupNames,1L << i))
        j++;
  StatiIterNature(p,WEP_SKILL)
    j++;
  StatiIterEnd(p)

  first = 1; 
  for(i=0;i!=32;i++)
    if (p->Proficiencies & (1L << i))
      if (LookupOnly(WeaponGroupNames,1L << i)) {
        if (!first) {
          if (j-- == 2) cs.sProfs += " and " ;
          else cs.sProfs += ", " ;
        } 
        first = 0; 
        cs.sProfs += Lookup(WeaponGroupNames,1L << i); 
      }

  StatiIterNature(p,WEP_SKILL)
      if (!first) {
        if (j-- == 2) cs.sProfs += " and " ;
        else cs.sProfs += ", " ;
      } 
      first = 0; 
      ASSERT(S->eID)
        cs.sProfs += NAME(S->eID);
      switch(S->Val) {
        case WS_PROFICIENT:    break;
        case WS_FOCUSED:       cs.sProfs += " (focused)"; break;
        case WS_SPECIALIST:    cs.sProfs += " (specialized)"; break;
        case WS_MASTERY:       cs.sProfs += " (weapon master)"; break;
        case WS_HIGH_MASTERY:  cs.sProfs += " (high master)"; break;
        case WS_GRAND_MASTERY: cs.sProfs += " (grand master)"; break;
      }
  StatiIterEnd(p)

  cs.sProfs += "\n";

  cs.sSkills = Format("%cSkill Ratings:%c\n",-WHITE,-GREY);

  for(i=1;i!=SK_LAST;i++) {
    if (!SkillInfo[i].name)
      continue;
    j = p->SkillLevel(i);
    if (j != p->Mod2(p->SkillAttr(i)) &&
        (SkillInfo[i].imp || 1)) {
        int color; 
        if (j > 25) color = 15;
        else if (j > 20) color = 14; 
        else if (j > 15) color = 12; 
        else if (j > 12) color = 4; 
        else if (j > 9) color = 6; 
        else if (j > 6) color = 5; 
        else if (j > 3) color = 9; 
        else if (j > 1) color = 1; 
        else color = 8; 

        skname = SkillInfo[i].name;
        skname.Replace("Knowledge","Know");

        cs.sSkills += Format("  %c%-16s %c%+d%s %c(%c%d%c ranks",
            -color, (const char*) skname,
            -15, j, abs(j) > 9 ? "" : " ", -6,
            p->SkillRanks[i] ? (p->SkillRanks[i] == 
              p->MaxRanks(i) ? -15 : -14) : -6,
             p->SkillRanks[i], -6);

        if (p->Mod2(p->SkillAttr(i)))
          cs.sSkills += Format(", %+d %s",
            p->Mod2(p->SkillAttr(i)),
            AttrTitle[p->SkillAttr(i)] );

        if (s_train)
          cs.sSkills += Format(", %+d train",s_train);
        if (s_focus)
          cs.sSkills += Format(", %+d focus",s_focus);
        if (s_feat)
          cs.sSkills += Format(", %+d feat",s_feat);
        if (s_ins)
          cs.sSkills += Format(", %+d insight",s_ins);
        if (s_racial)
          cs.sSkills += Format(", %+d racial",s_racial);
        if (s_enhance)
          cs.sSkills += Format(", %+d enhance",s_enhance);
        if (s_domain)
          cs.sSkills += Format(", %+d domain",s_domain);
        if (s_item)
          cs.sSkills += Format(", %+d magic",s_item);
        if (s_armor)
          cs.sSkills += Format(", %+d armor",s_armor);
        if (s_comp)
          cs.sSkills += Format(", %+d competence",s_comp);
        if (s_circ)
          cs.sSkills += Format(", %+d circumstance",s_circ);
        if (s_inh)
          cs.sSkills += Format(", %+d inherent",s_inh);
        if (s_size)
          cs.sSkills += Format(", %+d size",s_size);
        if (s_kit)
          cs.sSkills += Format(", %+d kit",s_kit);
        if (s_syn)
          cs.sSkills += Format(", %+d synergy",s_syn);
        cs.sSkills += ")\n";
        /*
        cs.sSkills += Format("  %c%-15s (%s %+d, rnk %+d, oth %+d)\n",
            -color,
            SkillInfo[i].name,
            AttrTitle[ p->SkillAttr(i) ] , 
            p->Mod2(p->SkillAttr(i)),
            p->SkillRanks[i],
            j - (p->Mod2(p->SkillAttr(i)) + p->SkillRanks[i])
            );
        */
        /*
        if (j > 25)
          cs.sSkills += XPrint(" <15>Unearthly<7>  "); 
        else if (j > 20)
          cs.sSkills += XPrint(" <14>Legendary<7>  ");
        else if (j > 15)
          cs.sSkills += XPrint(" <12>Superior<7>   "); 
        else if (j > 12)
          cs.sSkills += XPrint(" <4>Excellent<7>  ");
        else if (j > 9)
          cs.sSkills += XPrint(" <6>Great<7>      "); 
        else if (j > 6)
          cs.sSkills += XPrint(" <5>Good<7>       "); 
        else if (j >= 3)
          cs.sSkills += XPrint(" <9>Average<7>    ");
        else if (j >= 0)
          cs.sSkills += XPrint(" <1>Poor<7>       ");
        else
          cs.sSkills += XPrint(" <8>Abyssal<7>    ");
          */

      }
  }

  bool header = false; 
  
  cs.sRacial.Empty();
  StatiIterNature(p,SKILL_BONUS)
      if (S->Source == SS_RACE || S->Source == SS_BODY ||
          S->Source == SS_TMPL) { 
        if (!header) {
          cs.sRacial += Format("%cRacial Skill Bonuses:%c\n",-WHITE,-GREY);
          header = true;
        } 
        cs.sRacial += Format("  %-15s (%+d, %s)\n",
            SkillInfo[S->Val].name, S->Mag,
            NAME(S->eID));
      } 
  StatiIterEnd(p)
  
  if (!p->WizardMode)
    { cs.sBody = ""; goto SkipBody; }
    
  cs.sBody = Format("%cGroups and Subtypes:%c\n",-WHITE,-GREY);
  for (i=0;i<MA_CHOICE1;i++) 
    if (i != 101 && p->isMType(i)) {
      const char * c = LookupOnly(MTypeNames,i);
      if (c && c[0]) 
        cs.sBody += Format("  %s\n",c);
      } 

  cs.sBody += Format("_\n%cBody Attributes:%c\n",-WHITE,-GREY);
  if (p->HasMFlag(M_AMORPH)) cs.sBody += "  Amorphous\n"; 
  if (p->HasMFlag(M_AMPHIB)) cs.sBody += "  Amphibious\n"; 
  if (p->HasMFlag(M_AQUATIC)) cs.sBody += "  Aquatic\n"; 
  if (p->HasMFlag(M_BLIND)) cs.sBody += "  Blind\n"; 
  if (p->HasMFlag(M_DEAF)) cs.sBody += "  Deaf\n"; 
  if (p->HasMFlag(M_FLYER)) cs.sBody += "  Flying\n"; 
  if (p->HasMFlag(M_LIGHT_AVERSE)) cs.sBody += "  Light-Averse\n"; 
  if (p->HasMFlag(M_MFLYER)) cs.sBody += "  Magically Flying\n"; 
  if (p->HasMFlag(M_MOUNTABLE)) cs.sBody += "  Mountable\n"; 
  if (p->HasMFlag(M_NO_CARRY_LOADS)) cs.sBody += "  Poor Carrying Capacity\n"; 
  if (p->HasMFlag(M_SWARM)) cs.sBody += "  Swarm\n"; 
  if (p->HasMFlag(M_INCOR)) cs.sBody += "  Incorporeal\n"; 
  else cs.sBody += "  Corporeal\n"; 

  SkipBody:

  cs.sDepths = Format("%cDepths Reached:%c\n", -WHITE, -GREY);
  for (i=0;i!=MAX_DUNGEONS;i++)
    if (p->MaxDepths[i])
      cs.sDepths += XPrint("__<Res>: <Num>'\n", 
           theGame->DungeonID[i], p->MaxDepths[i]*10);

  cs.sSpiritual = Format("%cSpiritual State:%c\n",-WHITE,-GREY);
  const char * AlignStrength[] = {
    "You are perfectly neutral with\n____regard to <Str3>.", // 0-3
    "You have slight <Str> tendancies.",                // 4-7
    "You have <Str> tendancies.",                       // 8-11
    "You have strong <Str> tendancies.",                // 12-15
    "You have very strong <Str>\n____tendancies.",      // 16-19
    "You are haltingly <Str>.",                         // 20-23
    "You are haltingly <Str>.",                         // 24-27
    "You are nominally <Str>.",                         // 28-31
    "You are nominally <Str>.",                         // 32-25
    "You are moderately <Str>.",                        // 36-39
    "You are moderately <Str>.",                        // 40-43
    "You are decisively <Str>.",                        // 44-47
    "You are decisively <Str>.",                        // 48-51
    "You are devotedly <Str>.",                         // 52-55
    "You are devotedly <Str>.",                         // 56-59
    "You are zealously <Str>.",                         // 60-63
    "You are zealously <Str>.",                         // 64-67
    "You flawlessly embody <Str2>." };                  // 68+   
  const char * FavorStrength[] = {
    "<Res> is noncommital.",
    "<Res> has noticed your\n____worship.",
    "<Res> is cautiously pleased\n____with your worship.",
    "<Res> is pleased with your\n____worship.",
    "<Res> is very pleased with\n____your worship.",
    "<Res> is exalted by your\n____worship.",
    "You are a minor champion of\n____<Res>.",
    "You are a champion of <Res>.",
    "You are among the greatest\n____of <Res>'s champions.",
    "You are the hand of <Res>\n____on Theyra.",
    "You have been crowned by <Res>."
    };
  
  const char * FavorPen[] = {
    "<Res> has not aided you.",                                   // 0 
    "<Res> aided you trivially.",                                 // 1-9
    "<Res> aided you minorly.",                                   // 10-19
    "<Res> aided you notably.",                                   // 20-29
    "You have come to depend\n____on <Res>' aid.",                // 30-39
    "You have come to depend\n____on <Res>' aid overmuch.",       // 40-49
    "The aid <Res> grants you\n____has become a strain.",         // 50-59
    "The aid <Res> grants you\n____has become a serious strain.", // 60-69
    "You are a burden to <Res>.",                                 // 70-79
    "<Res> will not aid you\n____much further.",                  // 80-89
    "<Res> aid is all but\n____depleted."                         // 90+
    };
    
    
  if (p->GodID)
    cs.sSpiritual += XPrint("__You are devoted to <Res>.\n",p->GodID);
  else
    cs.sSpiritual += "__You are not devoted to a\n____single god.\n";
  int16 favPen;
  for (i=-1;i!=nGods;i++)
    {
      rID gID, favChart[15]; 
      int16 lev; int32 fv, fl;
      if (i==-1)
        gID = p->GodID;
      else if (GodIDList[i] == p->GodID)
        continue;
      else
        gID = GodIDList[i];
      if (!gID)
        continue;
      
      fl = p->getGodFlags(gID);
      
      if (!(fl & GS_INVOLVED))
        continue;
      
      if (fl & GS_ANATHEMA) {
        cs.sSpiritual += XPrint("__You are anathema to <Res>.\n", gID);
        continue;
        }
        
      if (fl & GS_FORSAKEN) {
        cs.sSpiritual += XPrint("__<Res> has forsaken you.\n", gID);
        continue;
        }
      
      if (fl & GS_ABANDONED)
        cs.sSpiritual += XPrint("__You abandoned <Res>.\n", gID);
      
      
      
      fv = p->calcFavor(gID);
      favPen = p->FavPenalty[theGame->GodNum(gID)];
      
      TGOD(gID)->GetList(FAVOR_CHART,favChart,15);
      
      cs.sSpiritual += "__";
      cs.sSpiritual += XPrint(FavorStrength[p->getGodLevel(gID)],gID);
      cs.sSpiritual += "\n__";
      if (favPen + TGOD(gID)->GetConst(INTERVENTION_COST) > 100)
        cs.sSpiritual += XPrint("<Res> will aid you no further.",gID);
      else
        cs.sSpiritual += XPrint(FavorPen[min(10,(favPen+9)/10)],gID);
      cs.sSpiritual += Format("\n____(Favor %d, Lev %d, Pen %d~)",
                              fv, p->getGodLevel(gID), favPen);
      cs.sSpiritual += "\n";
  
      if (fl & GS_KNOWN_ANGER) {
        /* HACKFIX */
        int i = p->getGodAnger(gID); 
        if (i)
          cs.sSpiritual += XPrint("__At present, you have angered <Res>.\n", gID);
        else
          cs.sSpiritual += XPrint("__At present, you have displeased <Res>.\n", gID);
        }


    }
  
  cs.sSpiritual += "__";
  cs.sSpiritual += XPrint(AlignStrength[min(17,abs(p->alignGE)/4)],
        p->alignGE > 0 ? "Evil" : "Good",
        p->alignGE > 0 ? "Evil" : "Good",
        "Good and Evil");
  cs.sSpiritual += "\n__";
  cs.sSpiritual += XPrint(AlignStrength[min(17,abs(p->alignLC)/4)],
        p->alignLC > 0 ? "Chaotic" : "Lawful",
        p->alignLC > 0 ? "Chaos" : "Law",
        "Law and Chaos");   
  cs.sSpiritual += "\n";
  
  if (p->desiredAlign & AL_GOOD)
    if (!(p->GetStatiVal(ALIGNMENT) & AL_GOOD))
      cs.sSpiritual += "__You seek to become Good.\n";
  if (p->desiredAlign & AL_LAWFUL)
    if (!(p->GetStatiVal(ALIGNMENT) & AL_LAWFUL))
      cs.sSpiritual += "__You seek to become Lawful.\n";
  if (p->desiredAlign & AL_CHAOTIC)
    if (!(p->GetStatiVal(ALIGNMENT) & AL_CHAOTIC))
      cs.sSpiritual += "__You seek to become Chaotic.\n";
  if (p->desiredAlign & AL_EVIL)
    if (!(p->GetStatiVal(ALIGNMENT) & AL_EVIL))
      cs.sSpiritual += "__You seek to become Evil.\n";
  if (!(p->desiredAlign & (AL_GOOD|AL_EVIL)))
    if (p->GetStatiVal(ALIGNMENT) & AL_GOOD)
      cs.sSpiritual += "__You no longer strive to be Good.\n";
  if (!(p->desiredAlign & (AL_GOOD|AL_EVIL)))
    if (p->GetStatiVal(ALIGNMENT) & AL_EVIL)
      cs.sSpiritual += "__You are no longer content to be Evil.\n";
  if (!(p->desiredAlign & (AL_LAWFUL|AL_CHAOTIC)))
    if (p->GetStatiVal(ALIGNMENT) & AL_CHAOTIC)
      cs.sSpiritual += "__You no longer strive to be Chaotic.\n";
  if (!(p->desiredAlign & (AL_LAWFUL|AL_CHAOTIC)))
    if (p->GetStatiVal(ALIGNMENT) & AL_LAWFUL)
      cs.sSpiritual += "__You no longer strive to be Lawful.\n";
  if (p->isFallenPaladin)
    cs.sSpiritual += "__You are a fallen paladin.\n";
    
  cs.sCharGen = Format("%cCharGen Options and Choices:%c\n",-WHITE,-GREY);
  if (p->WizardMode)
    cs.sCharGen += "__You are in Wizard Mode.\n";
  else if (p->ExploreMode)
    cs.sCharGen += "__You are in Explore Mode.\n";
  if (p->rerollCount && p->rerollCount < 3)
    cs.sCharGen += Format("__You rerolled %s%s.\n",
      p->rerollCount == 1 ? "once" : "twice",
      p->rerolledPerks ? " (with perks)" : "");
  else if (p->rerollCount)
    cs.sCharGen += Format("__You rerolled %d times%s.\n",
      p->rerollCount, p->rerolledPerks ? " (with perks)" : "");
  if (p->deathCount && p->deathCount < 3)
    cs.sCharGen += Format("__You eluded death %s.\n",
      p->deathCount == 1 ? "once" : "twice");
  else if (p->deathCount)
    cs.sCharGen += Format("__You eluded death %d times.\n",
      p->deathCount);
  for (i=0;OptionList[i].Val;i++)
    if (OptionList[i].Val >= OPC_CHARGEN &&
        OptionList[i].Val <  (OPC_CHARGEN+99))
      {
        cs.sCharGen += Format("__%s: %s\n",
          OptionList[i].Name,
          OptChoice(OptionList[i].Choices,
                    p->Opt(OptionList[i].Val)));
        if (OptionList[i].Val == OPT_STATS)
          if (p->statMethod != p->Opt(OPT_STATS))
            cs.sCharGen += Format("____(actually used %s)\n",
                 OptChoice(OptionList[i].Choices,p->statMethod));
      }
              
  
    
  return; 
} 

/* helper recursive function for listing inventory and containers */
void ListInventory(String &is, Item *i, int depth, bool descend)
{
  Player *p = theGame->GetPlayer(0);
  if ((!p) || p->isDead() || p->VictoryFlag || p->QuitFlag)
    i->SetKnown(-1); 
  is += Format("%*s%s\n",depth," ",(const char*)
          i->Name(NA_LONG|NA_MECH|NA_INSC));
  // ww: can't be done currently! i->SetKnown(oldKnown);  
  if (descend && i->Type == T_CONTAIN) {
    Container * conn = (Container *)i; 
    for (int j=0;(*conn)[j];j++) {
      ListInventory(is,(*conn)[j],depth+2,descend);
    }
  }
}

String & Detable(const char*_s, int8 mode)
  {
    String s, s2; int16 i;
    s = _s;
    Again:
    i = max(s.strchr(WRAP_INDENT),s.strchr(WRAP_BREAK));

    if (!i)
      return *tmpstr(s);
    switch (mode)
      {        
        case 0: /* Discard the header */
          s2.Empty();
         break;
        case 1: /* Print as seperate line */
          s2 = s.Left(i).Trim();
          s2 += ":\n  ";
         break;
        case 2: /* Print on the same line */
          s2 = s.Left(i);
         break;
        default:
          Error("Strange Detable mode!");
         break;
      }
    s2 += s.Right(s.GetLength()-(i+1));  
    s = s2;
    goto Again;
  }


void TextTerm::CreateCharDump(String &str)
{
  CharSheet cs; static String cd, is, js, ls, ss, mons, ms;
  int16 i,j;

  /* character dump:
   * (1) char sheet 
   * (2) inventory
   * (3) spells
   * (4) journal
   * (5) game/level info
   * (6) messages
   */
  CreateCharSheet(cs);

  cd = SC("                        ") + cs.Banner + SC("\n");
  cd += Detable(cs.Basics,2) + SC("\n");
  for (i=0;i<7;i++) 
    cd += Detable(cs.sAttr[i],2) + SC("\n");
  for (i=0;i<3;i++)
    cd += Detable(cs.sSaves[i],2) + SC("\n");

  for (i=0;i!=5;i++)
    if (cs.sCombat[i][0]) {
      cd += Detable(cs.sCombat[i][0],2);
      cd += Detable(cs.sCombat[i][1],2);
      cd += Detable(cs.sCombat[i][2],2);
      }

  cd += Detable(cs.sSize,1) + "\n";
  cd += Detable(cs.sDefense,1) + "\n";
  cd += Detable(cs.sCoverage,1) + "\n";
  cd += Detable(cs.sMove,1) + "\n";
  cd += Detable(cs.sHitDice,1) + "\n";
  cd += Detable(cs.sMana, 1) + "\n";
  cd += Detable(cs.sEncum, 1) + "\n";
  if (cs.sMagicRes)
    cd += Detable(cs.sMagicRes, 1) + "\n";
  
  for (i=0;i!=3;i++)
    if (cs.sSpells[i])
      cd += Detable(cs.sSpells[i],1) + "\n";
  
  cd += Detable(cs.sSpecials,2) + "\n";
  cd += Detable(cs.sFeats,2) + "\n";
  cd += Detable(cs.sResists,2) + "\n";
  if (cs.sImmune)
    cd += Detable(cs.sImmune,2) + "\n";
  cd += Detable(cs.sProfs,2) + "\n";
  cd += Detable(cs.sSkills,2) + "\n";
  cd += Detable(cs.sSpiritual,2) + "\n";
  cd += Detable(cs.sCharGen,2) + "\n";

  is = Format("\n%cInventory:%c\n",-WHITE,-GREY);
  for (Item *it = p->FirstInv(); it; it = p->NextInv()) {
          ListInventory(is,it,2,false);
  }

  if (m->InBounds(p->x,p->y)) {
    is += Format("\n%cOn The Ground:%c\n",-WHITE,-GREY);
    for (Thing *t = oThing(m->At(p->x,p->y).Contents) ; t ; 
                t = oThing(t->Next)) 
      if (t->isItem()) 
        ListInventory(is,(Item *)t,2,true);
  }

  ss = Format("\n%cKnown Spells:%c\n",-WHITE,-GREY);

  for (int16 ii=0;ii!=theGame->LastSpell();ii++) 
    if ((j = p->SpellRating(theGame->SpellID(ii),0,true)) != -1) {
      int mana = (TEFF(theGame->SpellID(ii))->ManaCost);
      ss += Format("  %c%-30s%c %2d  %3d~ %4d %s\n",
                      (p->Spells[ii] & SP_DOMAIN) ? -5 : -1,
                      NAME(theGame->SpellID(ii)), -7,
                      TEFF(theGame->SpellID(ii))->Level, 
                      j, mana, 
                      (const char*)
                      (TEFF(theGame->SpellID(ii))->Power(p->CasterLev(),p,theGame->SpellID(ii)))
                  );
        }

  js = Format("\n%cJournal:%c\n",-WHITE,-GREY);

  js += XPrint(p->GetJournal());

  mons = Format("\n%cMonsters Encountered:%c\n",-WHITE,-GREY);

  mons += p->GetMonstersEncountered(); 

  ls = Format("\n%cLevel Statistics:%c\n",-WHITE,-GREY);

  ls += XPrint(p->GetLevelStats());

  ms = Format("\n%cMessages:%c\n",-WHITE,-GREY);
          
  ms += SC(XPrint(MsgHistory)).Right(80*10);

  str = XPrint(cd + is + ss + js + mons + ls + ms);

}

void TextTerm::DumpCharacter(String & filebase)
{
  String cd;
  String stemp; 

  CreateCharDump(cd);
  
  ChangeDirectory(LogSubDir());
  
  try {
    OpenWrite(filebase + ".txt");

    for (char * s = (char*)((const char*)cd); *s; s++) {
      if (*s == '~') *s = '%'; 
      if (*s <= 0) continue; 
      FWrite(s,1);
    } 

    Close();

    OpenWrite(filebase + ".html");

    stemp = "<TT><body BGCOLOR=\"#000000\" text=\"white\">";
    FWrite((const char*)stemp,stemp.GetLength());

    for (char * hs = (char*)((const char*)cd); *hs; hs++) {
      switch (*hs) {
        case -1: stemp = "</FONT><FONT COLOR=\"blue\">" ; break;
        case -2: stemp = "</FONT><FONT COLOR=\"green\">" ; break;
        case -3: stemp = "</FONT><FONT COLOR=\"cyan\">" ; break;
        case -4: stemp = "</FONT><FONT COLOR=\"red\">" ; break;
        case -5: stemp = "</FONT><FONT COLOR=\"purple\">" ; break;
        case -6: stemp = "</FONT><FONT COLOR=\"brown\">" ; break;
        case -7: stemp = "</FONT><FONT COLOR=\"grey\">" ; break;
        case -8: stemp = "</FONT><FONT COLOR=\"darkgrey\">" ; break;
        case -9: stemp = "</FONT><FONT COLOR=\"azure\">" ; break;
        case -10: stemp = "</FONT><FONT COLOR=\"emerald\">" ; break;
        case -11: stemp = "</FONT><FONT COLOR=\"skyblue\">" ; break;
        case -12: stemp = "</FONT><FONT COLOR=\"pink\">" ; break;
        case -13: stemp = "</FONT><FONT COLOR=\"magenta\">" ; break;
        case -14: stemp = "</FONT><FONT COLOR=\"yellow\">" ; break;
        case -15: stemp = "</FONT><FONT COLOR=\"white\">" ; break;
        case ' ': case '\t': stemp = "&nbsp;" ; break;
        case '\n': case '\r': stemp = "<br>" ; break; 
        case '<': stemp = "[" ; break;
        case '>': stemp = "]" ; break;
        default: FWrite(hs,1); continue;
        } 
      FWrite((const char*)stemp,stemp.GetLength());
      } 

    Close();
    }
  catch(int error_number)
    {
      Error("Error writing character dump (%s).",
        Lookup(FileErrors,error_number));
    }
}
