/* HELP.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Contains the implementation of the online help system,
   as well as any functions which provide descriptions of
   things in plain English, such as the monster memory. */

#include "Incursion.h"

extern KeySetItem StandardKeySet[], RoguelikeKeySet[]; 
extern int16 CorpseNut[];
String & DescribeFeat(int16 ft);

rID rList[1024];

bool isHTML = 0;

String & BoxIt(String Header,int16 col, int16 col2)
  {
    String s; int16 i;
    
    //if (isHTML)
    //  return Format("<H1>%s</H1><BR>\n\n",Header);
    
    s = Format("%c",-col);
    for(i=0;i!=(Header.GetLength()+4);i++)
      s += "=";
    s += "\n";
    s += "= ";
    s += Upper(Header);
    s += " =\n";
    for(i=0;i!=(Header.GetLength()+4);i++)
      s += "=";
    s += Format("%c\n\n",-col2); 
    return *tmpstr(s); 
  }

int HelpResourceSort(const void *a, const void *b)
{
  Resource * aa = RES(*(rID *)a),
           * bb = RES(*(rID *)b);
    return (stricmp( theGame->GetText(0x01000000,aa->Name),
                    theGame->GetText(0x01000000,bb->Name)));
}

int HelpEffectSort(const void *a, const void *b)
{
  TEffect  * aa = *(TEffect **)a,
           * bb = *(TEffect **)b;
  if (aa->Level == bb->Level) {
    return (stricmp( theGame->GetText(0x01000000,aa->Name),
                    theGame->GetText(0x01000000,bb->Name)));
  } else 
    return aa->Level - bb->Level; 
}

int HelpEffectFlatSort(const void *a, const void *b)
{
  TEffect  * aa = *(TEffect **)a,
           * bb = *(TEffect **)b;
  return (stricmp( theGame->GetText(0x01000000,aa->Name),
                    theGame->GetText(0x01000000,bb->Name)));
}

int HelpEffectIDSort(const void *a, const void *b)
{
  TEffect  * aa = TEFF(*((rID*)a)),
           * bb = TEFF(*((rID*)b));
  if (aa->Level == bb->Level) {
    return (strcmp( theGame->GetText(0x01000000,aa->Name),
                    theGame->GetText(0x01000000,bb->Name)));
  } else 
    return aa->Level - bb->Level; 
}


static void HelpTClass(String & helpText, TClass *tc)
{
  String h;
  int i;

  h = Format("<15>HitDie:_<1>d%2d_<15>Mana:<1>_d%2d_<15>Skills:<1>_%d_<15>Saves:<1>_%s%s%s_<15>Defense:_<1>1/%d\n<15>+toHit: <1>Archery <9>%d~<1>, Brawl <9>%d~<1>, Melee <9>%d~<1>, Thrown <9>%d~<1>\n",
      tc->HitDie,
      tc->ManaDie,
      tc->SkillPoints,
      (tc->Saves[0] ? "<9>F<1>" : "f"),
      (tc->Saves[1] ? "<9>R<1>" : "r"),
      (tc->Saves[2] ? "<9>W<1>" : "w"),
      tc->DefMod, 
      tc->AttkVal[0],
      tc->AttkVal[1],
      tc->AttkVal[2],
      tc->AttkVal[3]);
  h += Format("\n\n<10>CLASS SKILLS<2>\n__The class skills of the %s are ",
      (const char*)tc->GetName(0));
  int scount = 0;
  for (i=1;i!=SK_LAST;i++)
    if (tc->HasSkill(i)) 
      scount++;
  for (i=1;i!=SK_LAST;i++)
    if (tc->HasSkill(i)) {
      scount--;
      if (!scount)
        h += ("and "); 
      if (SkillInfo[i].imp)
        h += Format("%s",SkillInfo[i].name);
      else 
        h += Format("<8>%s<7>",SkillInfo[i].name);
      if (!scount)
        h += (".");
      else
        h += (", ");
    } 
  h += Format("\n\n<10>CLASS FEATURES<2>\n__All of the following are class features of the %s.", (const char*)tc->GetName(0));

  int pcount = 0; 
  for(i=0;i!=32;i++)
    if (tc->Proficiencies & (1L << i))
      if (LookupOnly(WeaponGroupNames,1L << i))
        pcount++;
  h += Format("__*_<7>Proficiencies<2> -- The %s is proficient with ",
      (const char*)(tc->GetName(0)));

  for(i=0;i!=32;i++)
    if (tc->Proficiencies & (1L << i))
      if (LookupOnly(WeaponGroupNames,1L << i)) {
        pcount--;
        h += Format("%s, ",Lookup(WeaponGroupNames,1L << i));
      } 

  helpText += h;
  return; 

} 

void HelpSpells(String & helpText, int32 source, Player *p)
{
  const char *Levs[] = { "ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE","SIX", "SEVEN", "EIGHT", "NINE", "EPIC" };
  TEffect * array[2048];
  String a, b, Header;
  int16 count = 0;

  for (int mIdx=0;theGame->Modules[mIdx];mIdx++) {
    Module * m = theGame->Modules[mIdx];

    for (int i=0; i<m->szEff; i++) {
      rID id = m->EffectID(i);
      TEffect * eff = TEFF(id);
      if (eff->HasSource((int8)source)) 
        array[count++] = eff;
      ASSERT(count < 2040);
    } 
  } 
  qsort(array,count,sizeof(array[0]),HelpEffectSort);
  for (int i=0;i<count;i++) {
    if (array[i]->Level > 9 || array[i]->Level < 1)
      continue;
    if (array[i]->HasFlag(EF_SPECABIL))
      continue;
    PurgeStrings();
    if (!i || array[i]->Level != array[i-1]->Level)
      {
        int16 lev = array[i]->Level, j;
        Header = Format("= LEVEL %s %s SPELLS {%d} =", Levs[lev],
          source == AI_THEURGY ? "DIVINE" :
          source == AI_DRUIDIC ? "DRUIDIC" : "???", lev);
        helpText += XPrint("\n\n<12>");
        for (j=0;j!=Header.GetLength();j++) 
          helpText += "=";
        helpText += "\n";
        helpText += Header;
        helpText += "\n";
        for (j=0;j!=Header.GetLength();j++) 
          helpText += "=";
        helpText += "\n\n";
      }

    if ((!i) || array[i]->GetName(0)[0] != array[i-1]->GetName(0)[0])
      {
        a = Format(" {%d%c}", array[i]->Level,
                                         array[i]->GetName(0)[0]);
        b = array[i]->Describe(NULL);
        helpText += XPrint(b.Upto("\n"));
        helpText += a;
        helpText += XPrint(b.After("\n"));
        helpText += "\n\n";
      }     
    else
      helpText += Format("%s\n\n",XPrint(array[i]->Describe(NULL)));
    }

  helpText += "\n\n{MM:mainmenu} Return to Main Help Menu\n";


  return;
}

void HelpSpellIndex(String &helpText)
  {
    String l;
    static TextVal Groups[] = 
      {
        { 0, "ARCANE SPELLS BY SCHOOL" },
        { SC_ABJ, "Abjuration" },
        { SC_ARC, "Arcana" },
        { SC_DIV, "Divination" },
        { SC_ENC, "Enchantment" },
        { SC_EVO, "Evocation" },
        { SC_ILL, "Illusion" },
        { SC_NEC, "Necromancy" },
        { SC_THA, "Thaumaturgy" },
        { SC_WEA, "Weavecraft" },
        { SC_FIRE, "Fire Magic" },
        { SC_EARTH, "Earth Magic" },
        { SC_AIR, "Air Magic" },
        { SC_WATER, "Water Magic" },
        { SC_LIGHT, "Light Spells" },
        { SC_DARKNESS, "Darkness Spells" },
        
        { 0, "SPELLS BY CLASS" },
        { -AI_WIZARDRY, "Wizard Spells" },
        { -AI_THEURGY, "Priest Spells" },
        { -AI_DRUIDIC, "Druid Spells" },
        { 0, NULL }
      };
    int16 i,j,k,c, dlev; String s, Books;
    rID sID, Spells[4096];
    Item *it;
    it = Item::Create(FIND("spellbook [Adventurers' Arts]"));
    helpText.Empty();
    for (i=0;Groups[i].Text;i++)
      {
        if (Groups[i].Val == 0)
          {
            if (i)
              helpText += "\n";
            helpText += BoxIt(Groups[i].Text,SKYBLUE,GREY);
            continue;
          }
        
        helpText += Format("\n%c%s\n",-YELLOW,Groups[i].Text);
        for (j=0;j!=strlen(Groups[i].Text);j++)
          helpText += '=';
        helpText += '\n';
        helpText += -7;
        for(j=0,c=0;j!=theGame->LastSpell();j++)
          {
            sID = theGame->SpellID(j);
            if (Groups[i].Val < 0)
              if (TEFF(sID)->HasSource((int8)-Groups[i].Val))
                Spells[c++] = sID;
            if (Groups[i].Val > 0)
              if (TEFF(sID)->HasSource(AI_WIZARDRY) || Groups[i].Val > SC_WEA)
                if (TEFF(sID)->Schools & Groups[i].Val)
                  Spells[c++] = sID;
          }
        qsort(Spells,c,sizeof(rID),HelpEffectIDSort);
        for(j=0;j!=c;j++)
          {
            bool found = false;
            if (j == 0 || TEFF(Spells[j])->Level != 
                          TEFF(Spells[j-1])->Level)
                          helpText += Format("%c%c%c_", -YELLOW, '0' + TEFF(Spells[j])->Level, -GREY);
            else
              helpText += "__";
            
            Books = ""; dlev = 20;
            
            if (Groups[i].Val > 0 || Groups[i].Val == -AI_WIZARDRY)
              {
                for (int mIdx=0;theGame->Modules[mIdx];mIdx++) {
                  Module * m = theGame->Modules[mIdx];
                  for (k=0; k<m->szEff; k++) {
                    rID iID;
                    iID = m->ItemID(k);
                    if (TITEM(iID)->IType != T_BOOK)
                      continue;
                    it->iID = iID;
                    if (it->HasSpell(theGame->SpellNum(Spells[j])))
                      {
                        dlev = min(TITEM(iID)->Level,dlev);
                        s = *TITEM(it->iID)->GetMessages(MSG_STATINAME);
                        if (!s.GetLength())
                          {
                            s = it->Name(0);
                            s = s.After("[");
                            s = s.Right(s.GetLength() - 1);
                            s = s.Left(s.GetLength() - 1);
                          }
                        if (found) {
                          l = SC("___________________________") + Books;
                          while (l.strchr('\n')) {
                            l = l.After("\n");
                            l = l.Right(l.GetLength()-1);
                            }
                          if (l.GetTrueLength() + s.GetTrueLength() > 60)
                            Books += ",\n____________________________";
                          else
                            Books += ",_";
                          }
                        found = true;
                        Books += s;
                      }
                    }
                  }
              }
            
            
            helpText += found ? -13 : -6;
            
            
            
            s = NAME(Spells[j]);
            s += -7;
            while (s.GetLength() < 24)
              s += "_";
            s += "_";
            #if 0  
            if (Groups[i].Val > 0 || Groups[i].Val == -AI_WIZARDRY)
              s += Format("_[%d]",dlev);
            #endif            

            helpText += s;
            helpText += Books;
            helpText += "\n";
          }
      }
    helpText += "\n{MM:mainmenu} Return to Main Help Menu";
  }
                



void HelpWizSpells(String & helpText, Player *p)
{
  TEffect * array[256]; String Header;
  uint16 Schools[] = { SC_ABJ, SC_ARC, SC_DIV, 
                       SC_ENC, SC_EVO, SC_ILL,
                       SC_NEC, SC_THA, SC_WEA, 0 };
  char SchoolChars[] = { 'A', 'R', 'D', 'E', 'V',
                         'I', 'N', 'T', 'W' };
  const char *Levs[] = { "ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE",
                         "SIX", "SEVEN", "EIGHT", "NINE", "EPIC" },
             *Schs[] = { "ABJURATION", "ARCANA", "DIVINATION",
                         "ENCHANTMENT", "EVOCATION", "ILLUSION",
                         "NECROMANCY", "THAUMATURGY", "WEAVECRAFT" };

  int16 count = 0, lev = 1, sch = 0, i;

  for (lev = 1; lev <= 6; lev++)
    for (sch = 0; Schools[sch]; sch++)
      {
        PurgeStrings();
        if (!sch)
          {
            Header = Format("= LEVEL %s ARCANE SPELLS {%d} =", Levs[lev],lev);
            helpText += XPrint("\n\n<12>");
            for (i=0;i!=Header.GetLength();i++) 
              helpText += "=";
            helpText += "\n";
            helpText += Header; 
            helpText += "\n";
            for (i=0;i!=Header.GetLength();i++) 
              helpText += "=";
            helpText += "\n\n";
          }

        helpText += XPrint(Format("<10>-- %s %d --<7> {%d%c}\n\n", Schs[sch], lev, lev, 
                                                                     SchoolChars[sch]));

        count = 0;
        for (int mIdx=0;theGame->Modules[mIdx];mIdx++) {
          Module * m = theGame->Modules[mIdx];

          for (int i=0; i<m->szEff; i++) {
            rID id = m->EffectID(i);
            TEffect * eff = TEFF(id);
            if (eff->HasSource(AI_WIZARDRY))
              if (eff->Level == lev)
                if (eff->Schools & Schools[sch])
                  array[count++] = eff;
            }
          } 

        qsort(array,count,sizeof(array[0]),HelpEffectSort);
        for (int i=0;i<count;i++) 
          helpText += Format("%s\n\n",XPrint(array[i]->Describe(NULL)));


      } 
  helpText += "\n\n{MM:mainmenu} Return to Main Help Menu\n";

  return;
}

void HelpOtherSpells(String & helpText)
  {
    const char *Levs[] = { "ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE",
                           "SIX", "SEVEN", "EIGHT", "NINE", "EPIC" };    
    Player *p;
    int16 i, j, lv, q;
    bool link; 
    rID clID, lcID, spList[64], UnimpID;
    LearnableSpell *ls; String s;
    p = new Player(NULL,T_PLAYER);
    UnimpID = FIND("Unimplemented");
    
    s = BoxIt("OTHER CLASSES' SPELLS",SKYBLUE,CYAN);
    s += XPrint("<7>-- {MM:mainmenu} Return to the Main Menu\n");

    lcID = 0;
    for (q=0;q!=1;q++) /* Only one Module at the moment */
      for(i=0;i!=theGame->Modules[q]->szCla;i++)
        {
          clID = theGame->Modules[q]->ClassID(i);
          if (!TCLASS(clID)->GetList(SPELL_LIST,spList,64))
            continue;
          if (!stricmp(NAME(clID),"mage") ||
              !stricmp(NAME(clID),"priest") ||
              !stricmp(NAME(clID),"druid") ||
              !stricmp(NAME(clID),"abjurer") ||
              !stricmp(NAME(clID),"arcanist") ||
              !stricmp(NAME(clID),"diviner") ||
              !stricmp(NAME(clID),"enchanter") ||
              !stricmp(NAME(clID),"evoker") ||
              !stricmp(NAME(clID),"illusionist") ||
              !stricmp(NAME(clID),"thaumaturge") ||
              !stricmp(NAME(clID),"necromancer") ||
              !stricmp(NAME(clID),"weaver"))
            continue;
          link = true;
          if (lcID && stricmp(Left(NAME(lcID),2),Left(NAME(clID),2)) == 0)
            link = false;
          if (link)
            s += Format("-- {%s} %s Spells\n", (const char*) Upper(Left(NAME(clID),2)),
                   (const char*) NAME(clID));
          else
            s += Format("-- _____%s Spells\n", (const char*) NAME(clID));
            
          lcID = clID;
        }
    s += "\n";
    
    lcID = 0;
    for (q=0;q!=1;q++) /* Only one Module at the moment */
      for(i=0;i!=theGame->Modules[q]->szCla;i++)
        {
          clID = theGame->Modules[q]->ClassID(i);
          if (!TCLASS(clID)->GetList(SPELL_LIST,spList,64))
            continue;
          if (!stricmp(NAME(clID),"mage") ||
              !stricmp(NAME(clID),"priest") ||
              !stricmp(NAME(clID),"druid") ||
              !stricmp(NAME(clID),"abjurer") ||
              !stricmp(NAME(clID),"arcanist") ||
              !stricmp(NAME(clID),"diviner") ||
              !stricmp(NAME(clID),"enchanter") ||
              !stricmp(NAME(clID),"evoker") ||
              !stricmp(NAME(clID),"illusionist") ||
              !stricmp(NAME(clID),"thaumaturge") ||
              !stricmp(NAME(clID),"necromancer") ||
              !stricmp(NAME(clID),"weaver"))
            continue;
          p->RemoveStati(SPELL_ACCESS);
          p->GainPermStati(SPELL_ACCESS,NULL,SS_MISC,-1,20,clID);
          ls = p->CalcSpellAccess();
          
          
          if (!lcID || stricmp(Left(NAME(lcID),2),Left(NAME(clID),2)))
            s += BoxIt(Format("%s SPELLS {%s}",(const char*)Upper(NAME(clID)),
                  (const char*)Upper(Left(NAME(clID),2))),SKYBLUE,CYAN);
          else
            s += BoxIt(Format("%s SPELLS",(const char*)Upper(NAME(clID))),SKYBLUE,CYAN);
          
          lcID = clID;
          
          lv = 0;
          
          for (j=0;ls[j].spID;j++)
            {
            
              if (lv != ls[j].Level)
                s += Format("\n%c= LEVEL %s %s SPELLS =%c\n\n", -SKYBLUE, Levs[ls[j].Level], (const char*)Upper(NAME(clID)),-CYAN);
              lv = ls[j].Level;
              if (ls[j].spID == UnimpID)
                continue;
              s += XPrint(TEFF(ls[j].spID)->Describe(NULL));
              s += "\n\n";
              PurgeStrings();
            }
          s += "\n\n";
        }
    helpText += s;    
    helpText += "\n\n{MM:mainmenu} Return to Main Help Menu\n";
  }

void HelpPowers(String & helpText)
  {  
    int16 i, n, c; rID tID; TEffect *Effs[256];
    String s;
        
    s = BoxIt("MISCELLANEOUS REFERENCE",SKYBLUE,CYAN);
    s += XPrint("<7>-- {AL:alchemy} Alchemical Items\n");
    s += XPrint("<7>-- {PS:psionics} Psionic Wild Talents\n");
    s += XPrint("<7>-- {PO:poisons} Poisons\n");
    s += XPrint("-- {MM:mainmenu} Return to the Main Menu\n\n");

    TextVal types[] = {
      { AI_ALCHEMY, "Alchemical Items {AL}" },
      { AI_PSIONIC, "Psionic Powers {PS}" },
      { AI_POISON, "Poisons {PO}" },
      { 0, NULL } };
      
      for (i=0;types[i].Val;i++)
        {
          s += -11;
          s += types[i].Text;
          s += "\n";
          for (n=0;n!=strlen(types[i].Text);n++)
            s += "=";
          s += "\n\n";
          s += -3;
          c = 0;
          for (n=0;n!=theGame->LastSpell();n++) { 
              tID = theGame->SpellID(n);
              if (!(TEFF(tID)->HasSource((int8)types[i].Val) ||
                    (types[i].Val == AI_POISON && 
                    TEFF(tID)->ef.aval == AR_POISON)))
                continue;
              Effs[c++] = TEFF(tID);
          }
          Effs[c] = NULL;
          qsort(Effs,c,sizeof(Effs[0]),HelpEffectFlatSort);
          for (n=0;n!=c;n++)
            { s += Effs[n]->Describe(NULL); 
              if (types[i].Val != AI_POISON)
                s += "\n";
              s += "\n"; }
        }
    helpText += XPrint(s);    
    helpText += "\n\n{MM:mainmenu} Return to Main Help Menu\n";
  }


bool HelpDomains(String & helpText)
{
  String a, b, c, c2, Contents;
  int16 mIdx, i, j, count; 
  rID dID, gID, spID, UnimpID, RetribID, ProtectID;
  rID dsID[256]; int16 cds;
  if (theGame->Opt(OPT_HELP_MODE))
    {
      for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (i=0; i<m->szDom; i++) {
          rID id = m->DomainID(i);
          TDomain * dom = TDOM(id);
          T1->LOption(NAME(id),id,XPrint(dom->Describe(true)));
        }
      }
      T1->LMenu(MENU_3COLS|MENU_DESC|MENU_SORTED|MENU_ESC|MENU_BORDER|MENU_LARGEBOX,
          "-- Domain Descriptions --",WIN_MENUBOX);
      return true;
    }
  else
    {
      Contents  = "<11>============\n";
      Contents +=     "= CONTENTS =\n";
      Contents +=     "============<7>\n";
      Contents += "-- {MM:mainmenu} Return to the Main Menu\n";
      Contents += "-- {DA} Domain Allocations";
      
      for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (i=0; i<m->szDom; i++) {
          dID = theGame->Modules[mIdx]->DomainID(i);
          if (!(i%3))
            Contents += "\n-- ";
          Contents += Format("{%c%c} %s",
                  toupper(NAME(dID)[0]),
                  toupper(NAME(dID)[1]),
                  NAME(dID));
          if ((i+1)%3)
            for (j=strlen(NAME(dID));j!=12;j++)
              Contents += "_";
          }
        }
      
      Contents += "\n-- {MM:mainmenu} Return to Main Help Menu\n\n";

      helpText = Contents + BoxIt("DOMAIN ALLOCATIONS",SKYBLUE,CYAN);
      
      helpText += "\n<15>God_______Domain List<1>\n";
      RetribID = FIND("Retribution");
      ProtectID = FIND("Protection;domain");
      for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (i=0; i<m->szGod; i++) {
          gID = theGame->Modules[mIdx]->GodID(i);
          if (!stricmp(NAME(gID),"The Multitude"))
            continue;
          helpText += SC("<9>") + SC(NAME(gID)) + SC("<1>");
          for (j=strlen(NAME(gID));j!=10;j++)
            helpText += "_";  
          for (j=0;j != 12 && TGOD(gID)->Domains[j];j++)
            {
              /*if (j == 3 || j == 6 || j == 9)
                helpText += "\n_______________";*/
              helpText += Format("%s",
                TGOD(gID)->Domains[j] == RetribID ? "Retrib." :
                TGOD(gID)->Domains[j] == ProtectID ? "Protect." :
                NAME(TGOD(gID)->Domains[j]),
                NAME(TGOD(gID)->Domains[j])[0],
                NAME(TGOD(gID)->Domains[j])[1]);
              if (TGOD(gID)->Domains[j+1])
                {
                  helpText += ", ";
                  /*for (k = strlen(NAME(TGOD(gID)->Domains[j]));k<12;k++)
                    helpText += "_";*/
                }
            }
          helpText += "\n";
          }
        }
      
      helpText += "\n";

      helpText += BoxIt("DOMAIN DESCRIPTIONS",SKYBLUE,CYAN);

      c2.Empty(); count = 0;
      for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (int16 i=0; i<m->szDom; i++) {
          dID = m->DomainID(i);
          rList[count++] = dID;
          }
        }
      qsort(rList,count,sizeof(rList[0]),HelpResourceSort);

      for(i=0;i!=count;i++)
        {
          PurgeStrings();
          dID = rList[i];
          a = TDOM(dID)->Describe(false);
          c = NAME(dID);
          if (c2.GetLength() && c2[0] == c[0] && c2[1] == c[1])
            {
              helpText += a;
              Contents += "--      ";
              Contents += c;
              Contents += "\n";
            }
          else
            {
              helpText += a.Upto("\n");
              helpText += " {";
              helpText += toupper(c[0]);
              helpText += toupper(c[1]);
              helpText += "}";
              helpText += a.After("\n");

              Contents += "-- {";
              Contents += toupper(c[0]);
              Contents += toupper(c[1]);
              Contents += "} ";
              Contents += c;
              Contents += "\n";
            }
          helpText += "\n"; 
          c2 = c;
        }
      helpText += "\n";
      helpText += BoxIt("UNIQUE DOMAIN SPELLS",SKYBLUE,CYAN);
      
      UnimpID = FIND("Unimplemented"); cds = 0;
      for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (int16 i=0; i<m->szDom; i++) {
          dID = m->DomainID(i);
          for (int16 n=0;n!=9;n++)
            if ((spID = TDOM(dID)->Spells[n]) && (spID != UnimpID))
              {
                if (TEFF(spID)->HasSource(AI_WIZARDRY) ||
                    TEFF(spID)->HasSource(AI_DRUIDIC) ||
                    TEFF(spID)->HasSource(AI_THEURGY) )
                  continue;
                dsID[cds++] = spID;
              }
          }
        }
      dsID[cds] = 0;
      
      qsort(dsID,cds,sizeof(rID),HelpResourceSort);
      for (i=0;dsID[i];i++) {
        helpText += XPrint(TEFF(dsID[i])->Describe(NULL));
        helpText += "\n\n"; 
        }       
      
      helpText = XPrint(helpText);
      return false;
    }
}

static bool HelpRaces(String & helpText)
{
  String n, n2, Contents; 
  int16 count, i, mIdx; 
  rID xID;
  if (theGame->Opt(OPT_HELP_MODE))
    {
      for (int mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (int i=0; i<m->szRac; i++) {
          rID id = m->RaceID(i);
          T1->LOption(NAME(id),id,DESC(id));
        }
      }
      T1->LMenu(MENU_3COLS|MENU_DESC|MENU_SORTED|MENU_ESC|MENU_BORDER|MENU_LARGEBOX,
          "-- Race Descriptions --",WIN_MENUBOX);
      return true;
    }
  else
    {
      Contents  = "<11>============\n";
      Contents +=     "= CONTENTS =\n";
      Contents +=     "============\n";
      Contents += "-- {MM:mainmenu} Return to Main Help Menu\n\n";
      Contents += "<14>CORE RACES<7>\n";
      
      helpText  = "<11>=====================\n";
      helpText +=     "= RACE DESCRIPTIONS =\n";
      helpText +=     "=====================<7>\n\n";
      n2.Empty(); count = 0;
      for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (int16 i=0; i<m->szRac; i++) {
          xID = m->RaceID(i);
          if (TRACE(xID)->BaseRace)
            continue;
          rList[count++] = xID;
          }
        }
      qsort(rList,count,sizeof(rList[0]),HelpResourceSort);
      for (i=0;i!=count;i++)
        {
          PurgeStrings();
          xID = rList[i];

          n = NAME(xID);
          
          helpText += Format("%c%s%c",
            -YELLOW, (const char*)Upper(n), -GREY);
          if (!(n2.GetLength() && n2[0] == n[0] && n2[1] == n[1]))
            {
              Contents += "-- {";
              Contents += toupper(n[0]);
              Contents += toupper(n[1]);
              Contents += "} ";
              Contents += n;
              Contents += "\n";

              helpText += " {";
              helpText += toupper(n[0]);
              helpText += toupper(n[1]);
              helpText += "}";
            }
          else {
            Contents += "--      ";
            Contents += n;
            Contents += "\n";
            }
          helpText += "\n";
          helpText += XPrint(DESC(xID));
          helpText += "\n\n"; 
          n2 = n;
        }
      helpText += "\n\n";
      helpText += "<11>============\n";
      helpText +=     "= SUBRACES =\n";
      helpText +=     "============<7>\n\n";
      Contents += "\n<14>SUBRACES<7>\n";

      n2.Empty(); count = 0;
      for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (int16 i=0; i<m->szRac; i++) {
          xID = m->RaceID(i);
          if (!TRACE(xID)->BaseRace)
            continue;
          rList[count++] = xID;
          }
        }
      qsort(rList,count,sizeof(rList[0]),HelpResourceSort);

      for(i=0;i!=count;i++)
        {
          PurgeStrings();
          xID = rList[i];
          n = NAME(xID);
          
          helpText += Format("%c%s%c",
            -YELLOW, (const char*)Upper(n), -GREY);
          if (!(n2.GetLength() && n2[0] == n[0]))
            {
              Contents += "-- {1";
              Contents += toupper(n[0]);
              Contents += "} ";
              Contents += n;
              Contents += "\n";
             
              helpText += " {1";
              helpText += toupper(n[0]);
              helpText += "}";
            }
          else {
            Contents += "--      ";
            Contents += n;
            Contents += "\n";
            }

          helpText += "\n";
          helpText += XPrint(DESC(xID));
          helpText += "\n\n"; 
          n2 = n;
        }
      Contents += "\n";
      helpText = XPrint(Contents + helpText);
      return false;
    }
      
}

static bool HelpClasses(String & helpText)
{
  String n, n2, Contents; 
  int16 count, i, mIdx; 
  rID xID;
  if (theGame->Opt(OPT_HELP_MODE))
    {
      for (int mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (int i=0; i<m->szCla; i++) {
          rID id = m->ClassID(i);
          if (!TCLASS(id)->HasFlag(CF_PSEUDO))
            T1->LOption(NAME(id),id,DESC(id));
        }
      }

      T1->LMenu(MENU_3COLS|MENU_DESC|MENU_SORTED|MENU_ESC|MENU_BORDER|MENU_LARGEBOX,
          "-- Class Descriptions --",WIN_MENUBOX);
      return true;
    }
  else
    {
      Contents  = "<11>============\n";
      Contents +=     "= CONTENTS =\n";
      Contents +=     "============\n";
      Contents += "-- {MM:mainmenu} Return to Main Help Menu\n\n";
      Contents += "<14>CORE CLASSES<7>\n";
      
      helpText  = "<11>======================\n";
      helpText +=     "= CLASS DESCRIPTIONS =\n";
      helpText +=     "======================<7>\n\n";
      n2.Empty(); count = 0;
      for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (int16 i=0; i<m->szCla; i++) {
          xID = m->ClassID(i);
          if (TCLASS(xID)->HasFlag(CF_PRESTIGE) ||
              TCLASS(xID)->HasFlag(CF_PSEUDO))
            continue;
          rList[count++] = xID;
          }
        }
      qsort(rList,count,sizeof(rList[0]),HelpResourceSort);

      for(i=0;i!=count;i++)
        {
          PurgeStrings();
          xID = rList[i];
          n = NAME(xID);
                    
          helpText += Format("%c%s%c",
            -YELLOW, (const char*)Upper(n), -GREY);
          if (!(n2.GetLength() && n2[0] == n[0] && n2[1] == n[1]))
            {
              Contents += "-- {";
              Contents += toupper(n[0]);
              Contents += toupper(n[1]);
              Contents += "} ";
              Contents += n;
              Contents += "\n";
             
              helpText += " {";
              helpText += toupper(n[0]);
              helpText += toupper(n[1]);
              helpText += "}";
            }
          else {
            Contents += "--______";
            Contents += n;
            Contents += "\n";
            }
          helpText += "\n";
          helpText += XPrint(DESC(xID));
          helpText += "\n\n"; 
          n2 = n;
        }
      helpText += "\n\n";
      helpText += "<11>====================\n";
      helpText +=     "= PRESTIGE CLASSES =\n";
      helpText +=     "====================<7>\n\n";
      Contents += "\n<14>PRESTIGE CLASSES<7>\n";

      n2.Empty(); count = 0;
      for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (int16 i=0; i<m->szCla; i++) {
          xID = m->ClassID(i);
          if (!TCLASS(xID)->HasFlag(CF_PRESTIGE) ||
               TCLASS(xID)->HasFlag(CF_PSEUDO))
            continue;
          rList[count++] = xID;
          }
        }
      qsort(rList,count,sizeof(rList[0]),HelpResourceSort);

      for(i=0;i!=count;i++)
        {
          PurgeStrings();
          xID = rList[i];
          n = NAME(xID);
          
          helpText += Format("%c%s%c",
            -YELLOW, (const char*)Upper(n), -GREY);
          if (!(n2.GetLength() && n2[0] == n[0]))
            {
              Contents += "-- {1";
              Contents += toupper(n[0]);
              Contents += "} ";
              Contents += n;
              Contents += "\n";
             
              helpText += " {1";
              helpText += toupper(n[0]);
              helpText += "}";
            }
          else {
            Contents += "--______";
            Contents += n;
            Contents += "\n";
            }

          helpText += "\n";
          helpText += XPrint(DESC(xID));
          helpText += "\n\n"; 
          n2 = n;
        }
      Contents += "\n";
      helpText = XPrint(Contents + helpText);
      return false;
    }

}

static bool HelpPantheon(String & helpText)
{
  String a, b, c, c2, Contents, desc;
  int16 mIdx, i, count; rID gID;
  if (theGame->Opt(OPT_HELP_MODE))
    {

      T1->LOption("-- General --",-1,helpText);
      for (int mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (int i=0; i<m->szGod; i++) {
          rID id = m->GodID(i);
          desc = DESC(id); 
          for (int d=0;d<6;d++) {
            if (TGOD(id)->Domains[d])
              desc += Format("\n\n%s",(const char*)
                  TDOM(TGOD(id)->Domains[d])->Describe());
          } 
          T1->LOption(NAME(id),id,XPrint(desc));
        }
      }
      T1->SetWin(WIN_SCREEN);
      T1->SizeWin(WIN_CUSTOM,T1->WinSizeX()-16,T1->WinSizeY()-8);
      T1->LMenu(MENU_3COLS|MENU_DESC|MENU_SORTED|MENU_ESC|MENU_BORDER|MENU_LARGEBOX,
          "-- The Theyran Pantheon --",WIN_CUSTOM);
      return true;
    }
  else
    {
      Contents  = "<11>============\n";
      Contents +=     "= CONTENTS =\n";
      Contents +=     "============<7>\n";
      Contents += "-- {MM:mainmenu} Return to Main Help Menu\n";
      Contents += "-- {PA} General Information\n";

      a = helpText;
      
      helpText  = "<11>========================\n";
      helpText += "= THE THEYRAN PANTHEON = {PA}\n";
      helpText += "========================<7>\n\n";
      helpText += a;
      helpText += "\n";

      helpText += "<11>======================\n";
      helpText += "= DEITY DESCRIPTIONS =\n";
      helpText += "======================<7>\n\n";


      c2.Empty(); count = 0;
      for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (int16 i=0; i<m->szGod; i++) {
          gID = m->GodID(i);
          rList[count++] = gID;
          }
        }
      qsort(rList,count,sizeof(rList[0]),HelpResourceSort);

      for(i=0;i!=count;i++)
        {
          PurgeStrings();
          gID = rList[i];
          //a = TGOD(gID)->Describe();
          c = NAME(gID);
          if (c2.GetLength() && c2[0] == c[0] && c2[1] == c[1])
            {
              helpText += "<14>";
              helpText += Upper(c);
              helpText += "<7>\n";
              helpText += DESC(gID);
              helpText += "\n";

              Contents += "--______";
              Contents += c;
              Contents += "\n";
            }
          else
            {
              helpText += "<14>";
              helpText += Upper(c);
              helpText += " {";
              helpText += toupper(c[0]);
              helpText += toupper(c[1]);
              helpText += "}";
              helpText += "<7>\n";
              helpText += DESC(gID);
              helpText += "\n";

              Contents += "-- {";
              Contents += toupper(c[0]);
              Contents += toupper(c[1]);
              Contents += "} ";
              Contents += c;
              Contents += "\n";
            }
          helpText += "\n"; 
          c2 = c;
        }
      Contents += "\n";
      helpText = XPrint(Contents + helpText);
      return false;
    }



}

String & DescribeSkill(int16 sk);

static bool HelpSkills(String & helpText)
{
  bool are_unimp = false; int16 i;
  for (i = 1; i < SK_LAST; i++) 
    {
      PurgeStrings();
      helpText += XPrint(Format("\n<11>%s<3>\n%s\n",
        SkillInfo[i].name,
        (const char*)DescribeSkill(i)));
    }

  if (!are_unimp)
    return false;
  /*
  helpText += XPrint("\n\n<12>UNIMPLEMENTED SKILLS\n====================\n\n");  
  for (i = 1; i < SK_LAST; i++) 
    if (!SkillInfo[i].imp) 
      helpText += XPrint(Format("\n<11>%s<3>\n%s\n",
        SkillInfo[i].name,
        (const char*)DescribeSkill(i)));
  */
  return false;
}

static bool HelpFeats(String & helpText)
  {
    extern const char * FeatName(int16 ft);
    String c, c2, a;
    rID tID = FIND("help::feats");
    helpText = XPrint(DESC(tID));

    if (theGame->Opt(OPT_HELP_MODE))
      {
        T1->LOption("-- Feats Explained --",-1,helpText);

        for (int16 i=FT_FIRST;i<FT_LAST;i++) 
          if (!FeatUnimplemented(i)) {
            T1->LOption(FeatName(i),i, DescribeFeat(i));
            PurgeStrings();
            }
        T1->SetWin(WIN_SCREEN);
        T1->SizeWin(WIN_CUSTOM,T1->WinSizeX()-16,T1->WinSizeY()-8);
        T1->LMenu(MENU_3COLS|MENU_DESC|MENU_SORTED|MENU_ESC|MENU_BORDER|MENU_LARGEBOX,
          "-- Feats --",WIN_CUSTOM);
        return true;
      }
    else
      {
        c2.Empty(); c.Empty();
        for (int16 i=FT_FIRST;i < FT_LAST;i++)
          if (!FeatUnimplemented(i))
            {
              PurgeStrings();
              c2 = c;
              c = FeatName(i);
              a = XPrint(DescribeFeat(i));
              helpText += "\n";
              if (c2.GetLength() && c2[0] == c[0] && c2[1] == c[1])
                helpText += a;
              else
                {
                  helpText += a.Upto("\n");
                  helpText += " {";
                  helpText += toupper(c[0]);
                  helpText += toupper(c[1]);
                  helpText += "}";
                  helpText += a.After("\n");
                }

            }
        return false;
      }
  }

void HelpWeaponTable(String &helpText)
  {
    int16 mIdx, i, j, g, count; rID wepList[1024], wID;
    bool first; String line;


    TextVal WTypes[] = {
      { T_WEAPON, "<15>Melee Weapon__" },
      { T_WEAPON, "<15>Ranged Weapon_" },
      { T_BOW,    "<15>Bow/Crossbow__" },
      { T_MISSILE,"<15>Projectile____" },
      { 0,        NULL } };
    
    helpText += "\n";
    for (g=0;WTypes[g].Val;g++) {

      count = 0;
      for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
        Module * m = theGame->Modules[mIdx];
        for (i=0; i<m->szItm; i++) {
          wID = m->ItemID(i);
          if (TITEM(wID)->IType == WTypes[g].Val &&
              !TITEM(wID)->HasFlag(IT_NOGEN))
            wepList[count++] = wID;
          }
        }
      
      qsort(wepList,count,sizeof(rID),HelpResourceSort);
      
         //0        90        90        90        90        90        90         90
      helpText += XPrint(WTypes[g].Text);
      helpText += XPrint("Type_S_SDmg_LDmg_Ac_Pa_Spd__Ra_Threat_Crit_Spec<3>\n");
      
      for (i=0;i!=count;i++) {
        PurgeStrings();
        TItem *ti = TITEM(wepList[i]);
        
        if ( ((g == 0) && ti->u.w.RangeInc) ||
             ((g == 1) && !ti->u.w.RangeInc) )
          continue;
        
        line = Left(NAME(wepList[i]),(ti->Group & WG_EXOTIC) ? 12 :13);
        if ((ti->Group & WG_EXOTIC))
          line += "*";
        while (line.GetLength() < 14)
          line += "_";
        
        first = false;
        if (ti->HasFlag(WT_SLASHING))
          { line += "S"; first = true; }
        if (ti->HasFlag(WT_PIERCING))
          { if (first) 
              line += "/";
            line += "P"; 
            first = true; }
        if (ti->HasFlag(WT_BLUNT))
          { if (first) 
              line += "/";
            line += "B"; 
            first = true; }
        while (line.GetLength() < 19)
          line += "_";
          
        line += "**TSMLHGC****"[ti->Size];
        line += "_";
          
        line += ti->u.w.SDmg.Str();
        while (line.GetLength() < 26)
          line += "_";
        
        line += ti->u.w.LDmg.Str();
        while (line.GetLength() < 31)
          line += "_";      
          
        line += Format("%+d",ti->u.w.Acc);
        while (line.GetLength() < 34)
          line += "_";      
        
        line += Format("%+d",ti->u.w.ParryMod);
        while (line.GetLength() < 37)
          line += "_";      
          
        line += Format("%d~",(ti->u.w.Spd+20)*5);
        while (line.GetLength() < 42)
          line += "_";      
          
        if (ti->u.w.RangeInc)
          line += Format("%d",ti->u.w.RangeInc);
        else
          line += "--";
        while (line.GetLength() < 45)
          line += "_"; 
                    
        if (ti->u.w.Threat == 1)
          line += "20";
        else
          line += Format("%d-20",21 - ti->u.w.Threat);
        while (line.GetLength() < 52)
          line += "_";            
        
        line += Format("x%d",ti->u.w.Crit);
        while (line.GetLength() < 57)
          line += "_";      
          
        if (ti->HasFlag(WT_REACH))
          line += "R";
        if (ti->HasFlag(WT_STUNNING))
          line += "S";
        if (ti->HasFlag(WT_ENTANGLE))
          line += "E";
        if (ti->HasFlag(WT_KNOCKDOWN))
          line += "K";
        if (ti->HasFlag(WT_CHARGE))
          line += "C";
        if (ti->HasFlag(WT_MOUNTED_ONLY))
          line += "M";
        if (ti->HasFlag(WT_PENETRATING))
          line += "P";
        if (ti->HasFlag(WT_SUPER_TRIP))
          line += "T";
        if (ti->HasFlag(WT_SUPER_DISARM))
          line += "D";
        if (ti->HasFlag(WT_DOUBLE))
          line += "2";
        if (ti->HasFlag(WT_RETURNING))
          line += "X";
          
        line += "\n";
        
        helpText += line;
        
        }
      }
    helpText += XPrint(
      "<15>R:<3> This is a reach weapon.\n"
      "<15>S:<3> On a critical hit or great blow, or on 1 in 3 normal attacks, "
                     "this weapon will stun a foe who fails a Fortitude save "
                     "for 1d4+1 rounds.\n"
      "<15>E:<3> On a critical hit, or on 1 in 3 normal attacks, "
                     "this weapon will entangle a foe of its size category "
                     "or smaller who fails a Reflex save.\n" 
      "<15>K:<3> On a critical hit or great blow, or on 1 in 3 normal attacks, "
                     "this weapon will knock prone a character of its size category "
                     "or smaller who fails a Fortitude save when struck.\n"
      "<15>C:<3> This weapon deals double damage when used in a charge attack.\n"
      "<15>M:<3> This weapon can only be used while mounted unless you are one size "
                    "larger than it is.\n"
      "<15>P:<3> This weapon grants a x2 bonus to penetrating armour.\n"
      "<15>T:<3> This weapon grants a +4 bonus to trip attempts.\n"
      "<15>D:<3> This weapon grants a +4 bonus to disarm attempts.\n"
      "<15>2:<3> This is a double weapon, and may be used as if wielding two "
              "different weapons at the same time.\n"
      "<15>X:<3> This weapon returns to the user when thrown.\n\n"
      "<15>Weapon Groups_____Weapons<3>\n");


    count = 0;
    for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
      Module * m = theGame->Modules[mIdx];
      for (i=0; i<m->szItm; i++) {
        wID = m->ItemID(i);
        if (TITEM(wID)->IType == T_WEAPON ||
            TITEM(wID)->IType == T_BOW ||
            TITEM(wID)->IType == T_MISSILE)
          if (!TITEM(wID)->HasFlag(IT_NOGEN))
            wepList[count++] = wID;
        }
      }
      
    bool newline;
    for (i=0;WeaponGroupNames[i].Text;i++)
      {
        if (WeaponGroupNames[i].Val == WG_SHIELDS)
          break;
        line = WeaponGroupNames[i].Text;
        newline = true; first = true;
        for (j=0;j!=count;j++)
          if (TITEM(wepList[j])->Group & WeaponGroupNames[i].Val)
            {
              if (!first)
                line += ", ";
              first = false;
              if (line.GetTrueLength() + strlen(NAME(wepList[j])) > 59)
                { helpText += line; helpText += "\n";
                  line = ""; newline = true; }
              while(line.GetTrueLength() < 18)
                line += "_";
              line += NAME(wepList[j]);
            }
        helpText += line;
        helpText += "\n";
      }
    
  }

void HelpCustom(String &helpText, Player *p)
  {
    int16 i;
    if (!p) {
        helpText = BoxIt("No Character Currently Generated",SKYBLUE,CYAN);
        return;
    }
                                                                                                 
    helpText = BoxIt("My Race {R}",YELLOW,GREY);
    helpText += -EMERALD;
    helpText += Upper(NAME(p->RaceID));
    helpText += -7;
    helpText += "\n";
    helpText += XPrint(DESC(p->RaceID));
    helpText += "\n\n";

    helpText += BoxIt(p->ClassID[1] ? "My Classes {C}" : "My Class {C}",YELLOW,GREY);
    for (i=0;i!=3 && p->ClassID[i];i++)
      {
        helpText += -EMERALD;
        helpText += Upper(NAME(p->ClassID[i]));
        helpText += -GREY;
        helpText += "\n";
        helpText += XPrint(DESC(p->ClassID[i]));
        helpText += "\n\n";
      }

    // My Alignment {A}
    helpText += BoxIt("My Alignment {A}",YELLOW,GREY);
    for(i=0;i!=9;i++)
      if (AlignmentInfo[i].align == p->GetStatiVal(ALIGNMENT))
        {
          helpText += -EMERALD;
          helpText += Upper(AlignmentInfo[i].name);
          helpText += -GREY;
          helpText += "\n__";
          helpText += XPrint(AlignmentInfo[i].desc);
          helpText += "\n\n";
        }

    // My God {G}
    if (p->GodID)
      {
        helpText += BoxIt("My God {G}",YELLOW,GREY);
        helpText += -EMERALD;
        helpText += Upper(NAME(p->GodID));
        helpText += -GREY;
        helpText += "\n";
        helpText += XPrint(DESC(p->GodID));
        helpText += "\n\n";
      }

    // My Domains {D}
    if (p->HasAbility(CA_DOMAINS))
      {
        helpText += BoxIt("My Domains {D}",YELLOW,GREY);
        helpText += -EMERALD;
        StatiIter(p)
          if (S->Nature == HAS_DOMAIN)
            {
              helpText += XPrint(TDOM(S->eID)->Describe(false));
              helpText += "\n";
            }
        StatiIterEnd(p)
      }


    helpText += BoxIt("My Feats {F}",YELLOW,GREY);
    for (i=FT_FIRST;i!=FT_LAST;i++)
      {
        if (!p->HasFeat(i))
          continue;
        helpText += XPrint(DescribeFeat(i));
        helpText += "\n";
      }

    helpText += BoxIt("My Skills {K}",YELLOW,GREY);
    for (i=0;i!=SK_LAST;i++)
      {
        if (!p->GetSkillRanks(i))
          continue;
        for (int16 j=0;j!=SK_LAST;j++)
          if (SkillInfo[j].sk == i)
            helpText += XPrint("<13><Str><5>\n",
                          (const char*) Upper(SkillInfo[i].name));
        helpText += XPrint(DescribeSkill(i));
        helpText += "\n\n";
      }


    if (p->CasterLev())
      {
        int16 l; bool first, found;
        const char * LNames[] = { "Cantrips", "One", "Two", "Three",
                                  "Four", "Five", "Six", "Seven",
                                  "Eight", "Nine" };
        LearnableSpell *ls;
        helpText += BoxIt("My Spell Access {S}",YELLOW,GREY);
        ls = p->CalcSpellAccess();
        found = false;
        for (l=1;l<=9;l++)
          {
            first = true;
            for (i=0;ls[i].spID;i++)
              if (ls[i].Level == l)
                {
                  if (first)
                    {
                      first = false;
                      if (l != 1) {
                        helpText = helpText.Left(helpText.GetLength() - 2);
                        helpText += ".\n";
                        }
                      helpText += XPrint("<15>Level <Str> -- <1>",LNames[l]);
                    }
                  found = true;
                  helpText += NAME(ls[i].spID);
                  helpText += ", ";
                }
          }
        if (found) 
          helpText = helpText.Left(helpText.GetLength() - 2);
        helpText += found ? ".\n\n" : XPrint("<1>None.\n\n");
        
        helpText += BoxIt("My Spells {P}", YELLOW, GREY);
        rID spList[512]; int16 n;
        for(i=0,n=0;i!=theGame->LastSpell();i++)
          if (p->getSpellFlags(theGame->SpellID(i)) & 
                  (SP_KNOWN|SP_INNATE|SP_TINNATE|SP_DOMAIN))
            spList[n++] = theGame->SpellID(i);
        
        qsort(spList,n,sizeof(rID),HelpResourceSort);
        for (i=0;i!=n;i++)
          {
            helpText += XPrint(TEFF(spList[i])->Describe(p));
            helpText += "\n\n";
          }
        
        
      }
      
      
      
    // My Magic Items {M}
    
    // Known Items {K}
    
  }  

int SortKeys(const void *A, const void *B) {
    TextVal *a = (TextVal*)A, *b = (TextVal*)B;
    return a->Val > b->Val;
}

size_t countskipchars(const char *cs) {
    size_t len = 0, i = 0;
    while (i < strlen(cs)) {
        if (cs[i] == LITERAL_CHAR) {
            i += 3;
            len += 3-1;
        }  else
            i += 1;
    }
    return len;
}

static void DescribeKeys(String &s) {
    int16 i, j, n;
    TextVal KStr[KY_CMD_LAST];
    String Keys[KY_CMD_LAST];
    extern TextVal KeyCmdDescs[];
    KeySetItem * ks = theGame->Opt(OPT_ROGUELIKE) ? RoguelikeKeySet : StandardKeySet;
    int time_for_ret = 0;

    s = Format("            %c-- %cIncursion Key Bindings%c --\n", -GREY, -PINK, -GREY);
    n = 0;

    memset(KStr, 0, sizeof(TextVal) * KY_CMD_LAST);

    for (i = KY_HEADER_DIR; i < KY_CMD_LAST; i++) {
        if (i == KY_HEADER_DIR) {
            Keys[n] = Format("     %cDirection Keys%c     ", -PINK, -GREY);
            KStr[n].Text = Keys[n];
            KStr[n].Val = i;
            n++;
            continue;
        } else if (i == KY_HEADER_PROMPT) {
            Keys[n] = Format("  %cSelection Prompt Keys%c ", -PINK, -GREY);
            KStr[n].Text = Keys[n];
            KStr[n].Val = i;
            n++;
            continue;
        }

        if (i == KY_HEADER_GAME) {
            Keys[n] = Format("   %cGameplay Mode Keys%c   ", -PINK, -GREY);
            KStr[n].Text = Keys[n];
            KStr[n].Val = i;
            n++;
            continue;
        }

        for (j = 0; ks[j].cmd != KY_CMD_LAST; j++) {
            if (ks[j].cmd == i) {
                Keys[n] = "";

                if (ks[j].raw_key_flags == -1) {
                    if (ks[j].raw_key >= 200)
                        goto PrintSpecialKey;
                    Keys[n] += (char)ks[j].raw_key;
                    goto DoneKey;
                }
                if (ks[j].raw_key_flags & KY_FLAG_CONTROL)
                    Keys[n] += "C-";
                if (ks[j].raw_key_flags & KY_FLAG_ALT)
                    Keys[n] += "A-";
                if (isalpha(ks[j].raw_key)) {
                    if (ks[j].raw_key_flags & KY_FLAG_SHIFT)
                        Keys[n] += Format("%c", toupper(ks[j].raw_key));
                    else
                        Keys[n] += Format("%c", tolower(ks[j].raw_key));
                } else {
PrintSpecialKey:
                    switch (ks[j].raw_key) {
                    case KY_TAB:    Keys[n] += "Tab"; break;
                    case KY_UP:     Keys[n] += Format("%c%c%c", LITERAL_CHAR, LITERAL_CHAR1(GLYPH_ARROW_UP), LITERAL_CHAR2(GLYPH_ARROW_UP)); break;
                    case KY_DOWN:   Keys[n] += Format("%c%c%c", LITERAL_CHAR, LITERAL_CHAR1(GLYPH_ARROW_DOWN), LITERAL_CHAR2(GLYPH_ARROW_DOWN)); break;
                    case KY_LEFT:   Keys[n] += Format("%c%c%c", LITERAL_CHAR, LITERAL_CHAR1(GLYPH_ARROW_LEFT), LITERAL_CHAR2(GLYPH_ARROW_LEFT)); break;
                    case KY_RIGHT:  Keys[n] += Format("%c%c%c", LITERAL_CHAR, LITERAL_CHAR1(GLYPH_ARROW_RIGHT), LITERAL_CHAR2(GLYPH_ARROW_RIGHT)); break;
                    case KY_PGUP:   Keys[n] += "PgUp"; break;
                    case KY_PGDN:   Keys[n] += "PgDn"; break;
                    case KY_HOME:   Keys[n] += "Home"; break;
                    case KY_END:    Keys[n] += "End"; break;
                    case KY_ESC:    Keys[n] += "Esc"; break;
                    case KY_HELP:   Keys[n] += (ks[j].raw_key_flags & KY_FLAG_SHIFT) ? "?" : "/"; break;
                    case KY_ENTER:  Keys[n] += "Enter"; break;
                    case KY_COMMA:  Keys[n] += (ks[j].raw_key_flags & KY_FLAG_SHIFT) ? "<" : ","; break;
                    case KY_PERIOD: Keys[n] += (ks[j].raw_key_flags & KY_FLAG_SHIFT) ? ">" : "."; break;
                    case KY_ONE:    Keys[n] += "1"; break;
                    case KY_EIGHT:  Keys[n] += "8"; break;
                    case KY_MINUS:  Keys[n] += "-"; break;
                    case KY_EQUALS: Keys[n] += "="; break;
                    default:
                        if (ks[j].raw_key >= KY_F1 && ks[j].raw_key <= KY_F12)
                            Keys[n] += Format("F%d", 1 + ks[j].raw_key - KY_F1);
                        else
                            Keys[n] += Format("(%d)", ks[j].raw_key); break;
                    }
                }
DoneKey:;
            }
        }

        if (Keys[n].GetTrueLength() == 0)
            continue;
        const char * desc = LookupOnly(KeyCmdDescs, i);
        if (!desc)
            continue;
        int padcnt = countskipchars(Keys[n]);
        KStr[n].Text = Format("%c%s%c%*s%c", -GREY, desc, -YELLOW, 24 + padcnt - strlen(desc), (const char *)Keys[n], -GREY);
        KStr[n].Val = i;
        n++;
    }

    qsort(KStr, n, sizeof(TextVal), &SortKeys);

    for (i = 0; i != (n + 1) / 2; i++) {
        s += KStr[i].Text;
        s += " | ";
        s += KStr[i + n / 2].Text;
        s += "\n";
    }

    return;
}

void TextTerm::GetHelp(String & helpText, const char *topic) {
    Player *p; rID tID;
    p = theGame->GetPlayer(0);
    helpText.Empty();
    if (!strcmp(topic,"help::arcane spells")) {
      helpText = "";
      HelpWizSpells(helpText, p);
      }
    else if (!strcmp(topic,"help::divine spells")) {
      helpText = "";
      HelpSpells(helpText, AI_THEURGY,p);
      }
    else if (!strcmp(topic,"help::druid spells")) {
      helpText = "";
      HelpSpells(helpText, AI_DRUIDIC,p);
      }
    else if (!strcmp(topic,"help::other spells")) {
      helpText = "";
      HelpOtherSpells(helpText);
      }
    else if (!strcmp(topic,"help::powers")) {
      helpText = "";
      HelpPowers(helpText);
      }
    else if (!strcmp(topic,"help::spell index")) {
      helpText = "";
      HelpSpellIndex(helpText);
      }
    else if (!strcmp(topic,"help::custom")) {
      helpText = "";
      HelpCustom(helpText,&p[0]);
      }
    else if (!strcmp(topic,"help::domains")) {
      if (HelpDomains(helpText))
        { isHelp = false; return; }
      }
    else if (!strcmp(topic,"help::races")) {
      if (HelpRaces(helpText))
        { isHelp = false; return; }
      }
    else if (!strcmp(topic,"help::classes")) {
      if (HelpClasses(helpText))
        { isHelp = false; return; }
      }
    else if (!strcmp(topic,"help::skills")) {
      tID = FIND("help::skills");
      helpText = XPrint(DESC(tID));
      if (HelpSkills(helpText))
        { isHelp = false; return; }
      }
    else if (!strcmp(topic,"help::pantheon")) {
      tID = FIND("help::pantheon");
      helpText = XPrint(DESC(tID));
      if (HelpPantheon(helpText))
        { isHelp = false; return; }
      }
    else if (!strcmp(topic,"help::feats")) {
      if (HelpFeats(helpText))
        { isHelp = false; return; }
      }
    else { 
      tID = FIND(topic);
      if (!tID || !RES(tID)->Desc) {
        Box(Format("The help topic %c%s%c cannot be found among any of the "
              "currently loaded resources. Please contact Richard Tew "
              "at richard.m.tew@gmail.com if this is unexpected.",-YELLOW,topic,-GREY));
        isHelp = false; return;
        }
      helpText = XPrint(DESC(tID));
      }
}  

void TextTerm::HelpTopic(const char*topic, const char*link) {
    rID xID = 0;
    char ln[3], xln[2];
    int32 ch;
    char TopicName[52];
    String helpText, s;     
    int16 oldMode;

    oldMode = GetMode();
    SetMode(MO_HELP);

NewTopic:
    isHTML = false;
    isHelp = true;

    if (topic == NULL) { 
      DescribeKeys(helpText);
      helpText += Format("\n%c---------------------------------------------------"
                         "\n                    %cFurther Help%c\n",-WHITE,-PINK,-GREY);
      LOption("Help Contents",0);
      LOption("My Character_________",1);
      //LOption("Command Descriptions",2);
      //LOption("The Combat System",3);
      //LOption("The Selection Prompt",4);
      //LOption("The Inventory Manager",5);
      if (!theGame->Opt(OPT_CONTEXT_HELP) || !p)
        {
          LOption("Race Descriptions",6);
          LOption("Class Descriptions",7);
          LOption("Domain Descriptions",8);
        }
      else
        {
          LOption(Format("The %s Race",NAME(p->GetRaceID())),9);
          LOption(Format("The %s Class",NAME(p->GetClassID(0))),10);
          if (p->GetClassID(1))
            LOption(Format("The %s Class",NAME(p->GetClassID(1))),11);
          if (p->GetClassID(2))
            LOption(Format("The %s Class",NAME(p->GetClassID(2))),12);
          if (p->HasAbility(CA_DOMAINS))
            LOption("Domain Descriptions", 8);
        }

      LOption("Skill Descriptions",13);
      LOption("Feat Descriptions",14);
      //LOption("Schools of Magic",15);
      //LOption("Spellcasting",16);
      //LOption("Domain Descriptions", 24);
      LOption("Arcane Spells",17);
      LOption("Divine Spells",18);
      LOption("Druid Spells",19);
      LOption("Other Spells",20);
      LOption("The Theryan Pantheon",21);
      //LOption("Monster Memory", 22);
      LOption("View the OGL",23);
      switch (LMenu(MENU_SWRAPWRITE|MENU_ESC|
            MENU_2COLS|MENU_RAW|MENU_BORDER,helpText,WIN_MENUBOX)) {
        case 0: HelpTopic("help::mainmenu"); break;
        case 1: HelpTopic("help::custom"); break;
        case 2: HelpTopic("help::commands"); break;
        case 3: HelpTopic("help::combat"); break;
        case 4: HelpTopic("help::effectprompt"); break;
        case 5: HelpTopic("help::inventory"); break;
        case 6: HelpTopic("help::races"); break;
        case 7: HelpTopic("help::classes"); break;
        case 8: HelpTopic("help::domains"); break;
        case 9: helpText = XPrint(DESC(p->GetRaceID())); goto DoHelp;
        case 10: helpText = XPrint(DESC(p->GetClassID(0))); goto DoHelp;
        case 11: helpText = XPrint(DESC(p->GetClassID(1))); goto DoHelp;
        case 12: helpText = XPrint(DESC(p->GetClassID(2))); goto DoHelp;
        case 13: HelpTopic("help::skills"); break;
        case 14: HelpTopic("help::feats"); break;
        case 15: HelpTopic("help::schools"); break;
        case 16: HelpTopic("help::spellcasting"); break;
        case 17: HelpTopic("help::arcane spells"); break;
        case 18: HelpTopic("help::divine spells"); break;
        case 19: HelpTopic("help::druid spells"); break;
        case 20: HelpTopic("help::other spells"); break;
        case 21: HelpTopic("help::pantheon"); break;
        case 22: Monster *mn;
                 Save();
                 SetMode(MO_PLAY);
                 SetWin(WIN_MESSAGE);
                 Clear();
                 SetWin(WIN_INPUT);
                 Clear();
                 SetWin(WIN_SCREEN);
                 isHelp = false;
                 mn = (Monster*)AcquisitionPrompt(ACQ_RECALL,-10,20,0);
                 SetMode(MO_CHARVIEW);
                 Restore();
                 if (mn) {
                   Box(WIN_SCREEN,0,EMERALD,GREY,mn->Describe(p));
                   delete mn;
                   }
                 SetMode(oldMode);
                 return;
        case 23: HelpTopic("help::OGL"); break;
        default:
          isHelp = false;
          SetMode(oldMode);
          return;
      } 
      isHelp = false;
      SetMode(oldMode);
      return;
      }
    else 
      GetHelp(helpText,topic);
      
    if (!helpText.GetLength()) {
      SetMode(oldMode);
      return;
      }

    DoHelp:
    if (theGame->Opt(OPT_MONO_HELP))
      {
        int32 p;
        for (p=0;helpText[p];p++)
          if (helpText[p] <= -BLUE && helpText[p] >= -SHADOW)
            helpText.SetAt(p,-GREY);
      }
    SetWin(WIN_SCREEN);
    Save();
    SizeWin(WIN_CUSTOM,WinSizeX()-16,WinSizeY()-8);
    DrawBorder(WIN_CUSTOM,MAGENTA);
    ClearScroll(true);
    SetWin(WIN_CUSTOM); Color(YELLOW);
    SWrite((WinSizeX()/2)-6,0," -- HELP -- ");
    Color(GREY);
    SWrapWrite(1,1,helpText,WinSizeX()-2,WIN_CUSTOM);
    Color(GREY);
    UpdateScrollArea(0,WIN_CUSTOM); 
    ln[0] = ln[1] = ln[2] = xln[0] = xln[1] = 0; 
    do {
      if (link)
        {
          ASSERT(strlen(link) == 1 || strlen(link) == 2)
          ln[0] = link[0];
          ln[1] = link[1];
          ln[2] = 0;
          link = NULL;
          ch = ln[0];
          goto JumpToLink;
        }
      Color(GREY);
      UpdateScrollArea(offset,WIN_CUSTOM);
      switch(ch=GetCharCmd(KY_CMD_ARROW_MODE)) {
        case KY_CMD_NORTH:
          UpdateScrollArea(offset-1,WIN_CUSTOM); break;
        case KY_CMD_SOUTH:
          UpdateScrollArea(offset+1,WIN_CUSTOM); break;
        case KY_CMD_NORTHEAST:
        case KY_CMD_NORTHWEST:
          UpdateScrollArea(offset-(WinSizeY()-6),WIN_CUSTOM); break;
        case KY_CMD_SOUTHEAST:
        case KY_CMD_SOUTHWEST:
          UpdateScrollArea(offset+(WinSizeY()-6),WIN_CUSTOM); break;
        case KY_ESC:           
          ClearScroll(true);
          Restore();
          isHelp = false;
          SetMode(oldMode);
          return;
        case KY_TAB:
          HyperTab();
          break;
        case KY_ENTER:
          if (SelectedLink == -1)
            break;
          if (!HL[SelectedLink].Text[0])
            {
              for (int16 i=LinkCount-1;i!=-1;i--)
                if (!stricmp(HL[i].Link,HL[SelectedLink].Link))
                  {
                    UpdateScrollArea(HL[i].LineNo, WIN_CUSTOM);
                    goto DoneJump;
                  }
             }
          strcpy(TopicName,"help::");
          strcat(TopicName,HL[SelectedLink].Text);
          ClearScroll(true);
          Restore();
          topic = TopicName;
          goto NewTopic;         
          DoneJump:
          break;    
        default:
          if (isalnum(ch) && LinkCount)
            {
              int16 i;
              ln[0] = ln[1];
              ln[1] = (char)ch;
              ln[2] = 0;
              JumpToLink:
              for (i=LinkCount-1;i!=-1;i--)
                if (!stricmp(HL[i].Link,ln))
                  {
                    if (HL[i].Text[0])
                      { strcpy(TopicName,"help::");
                        strcat(TopicName,HL[i].Text);
                        ClearScroll(true);
                        Restore();
                        topic = TopicName;
                        goto NewTopic; }
                    UpdateScrollArea(HL[i].LineNo, WIN_CUSTOM);
                    break;
                  }
              xln[0] = (char)ch;
              xln[1] = 0;
              for (i=LinkCount-1;i!=-1;i--)
                if (!stricmp(HL[i].Link,xln))
                  {
                    if (HL[i].Text[0])
                      { strcpy(TopicName,"help::");
                        strcat(TopicName,HL[i].Text);
                        ClearScroll(true);
                        Restore();
                        topic = TopicName;
                        goto NewTopic; }
                    UpdateScrollArea(HL[i].LineNo, WIN_CUSTOM);
                    break;
                  }
            }
        }
      }
    while(1);
  }

struct HelpFile
  {
    const char *fn;
    const char *topic;
  };

void Game::WriteHTMLHelp()
  {
    String HTML, s, stemp, link, dest; int16 i, j;
    int32 Tags[128], nTags; 
    memset(Tags,0,128*4);

    const char * HTMLHeader = 
      "<html>"
      "  <head>"
      "    <title>%s</title>"
      "    <link rel=\"Stylesheet\" type=\"text/css\" href=\"Incursion.css\" />"
      "  </head>"
      "  <body>"
      "    <table width=100~ height=100~ cellspacing=0 cellpadding=0>"
      "      <tr>"
      "        <td style=\"background-image: url(waterleft.jpg);\">"
      "          <img src=\"thinbar.gif\">"
      "        </td>"
      "        <td BGCOLOR=\"#E2CAE0\" text=\"black\">"
      "          <SPAN STYLE=\"font-family: monospace\">"
      ;
    const char * HTMLFooter =
      "          </span>"
      "        </td>"
      "        <td style=\"background-image: url(waterright.jpg);\">"
      "          <img src=\"thinbar.gif\">"
      "        </td>"
      "      </tr>"
      "    </table>"
      "  </body>"
      "</html>";
      

    HelpFile HFs[] = {
      { "Manual.html",    "mainmenu" },
      { "Intro.html",     "intro" },
      { "CharGen.html",   "chargen" },
      { "Commands.html",  "commands" },
      { "Interface.html", "interface" },
      { "Advent.html",    "adventuring" },
      { "Combat.html",    "combat" },
      { "Magic.html",     "magic" },
      { "Overland.html",  "overland" },
      
      { "Races.html",     "races" },
      { "Classes.html",   "classes" },
      { "Feats.html",     "feats" },
      { "Skills.html",    "skills" },
      { "Pantheon.html",  "pantheon" },
      { "Domains.html",   "domains" },
      { "SpellIndex.html","spell index" },
      { "Arcane.html",    "arcane spells" },
      { "Divine.html",    "divine spells" },
      { "Druid.html",     "druid spells" },
      { "Other.html",     "other spells" },
      { "Powers.html",    "powers" },
      { "OGL.html",       "OGL" },
      { NULL, NULL } };
    
    isHTML = true;

    ASSERT(!Opt(OPT_HELP_MODE))
    
    T1->SetWin(WIN_SCREEN);
    T1->Clear();

    T1->Write("Loading modules...  ");
    if (!LoadModules())
      Error("Cannot load modules!");
    T1->Write("Done.\n\nWriting help...  ");    
    T1->ChangeDirectory(T1->ManualSubDir());
    try
      {
        for (i=0;HFs[i].fn;i++)
          {
            
            HTML = Format(HTMLHeader,"Incursion: Halls of the Goblin King");
            isHTML = true;
            ((TextTerm*)T1)->GetHelp(s,SC("help::") + SC(HFs[i].topic));
            isHTML = false;
            HTML += "</b></b>";
            HTML += s;
            HTML += HTMLFooter;
            nTags = 0;
            T1->ChangeDirectory(T1->ManualSubDir());
            T1->OpenWrite(HFs[i].fn);
            for (const char * hs = (const char *)HTML; *hs; hs++) {
              switch (*hs) {
                case -1: case -2: case -3:
                case -4: case -5: case -6:
                  stemp = "</b></b></b></b>";
                 break;
                case -7:
                  if (strstr(HFs[i].topic,"races") ||
                      strstr(HFs[i].topic,"classes"))
                    stemp = "<b>";
                  else
                    stemp = "</b></b></b>";
                 break;
                case -9: case -10: case -11:
                case -12: case -13: case -14:
                case -15:
                  stemp = "<b>";
                break;
                case '_': stemp = "&nbsp;"; break;
                case -8: continue;
                //case ' ': case '\t': stemp = "&nbsp;" ; break;
                case '\n': case '\r': stemp = "<br>\n" ; break; 
                //case '<': stemp = "[" ; break;
                //case '>': stemp = "]" ; break;
                case '~': stemp = "%"; break;
                case '{':
                  ASSERT(isalnum(hs[1]));
                  link = ""; dest = ""; stemp = "";
                  hs++;
                  while (*hs != ':' && *hs != '}')
                    link += *hs++;
                  if (*hs == ':') // cross-file link
                    {
                      hs++;
                      while (isalnum(*hs) || iswhite(*hs))
                        dest += *hs++;
                      for (j=0;HFs[j].fn;j++)
                        if (stricmp(HFs[j].topic,dest)==0)
                          stemp = Format("<A href=\"%s\">{%s}</A>",
                            HFs[j].fn, (const char *)link);
                    }
                  else
                    {
                      for (j=0;j!=nTags;j++)
                        if (Tags[j] == link[0] + link[1]*256)
                          goto Found;
                      
                      Tags[nTags++] = link[0] + link[1]*256;
                      stemp = Format("<A href=\"#%s\">{%s}</A>", 
                          (const char*) link, (const char*) link);
                      break;
                      Found:
                      stemp = Format("<A name=\"%s\">{%s}</A>", 
                          (const char*) link, (const char*) link);
                    }
                 break;
                default: T1->FWrite((void*)hs,1); continue;
                } 
              T1->FWrite((const char*)stemp,stemp.GetLength());
              } 
        
            T1->Close();
          }
      }
    catch(int error_number)
      {
        Error("Error writing HTML manual (%s).",
          Lookup(FileErrors,error_number));
      }

    T1->Write("Done.\n\n\nPress any key to continue...");
    T1->GetCharRaw();
    T1->Clear();
  }

String & Creature::Describe(Player *p)
  { return Name(NA_LONG); }

const char* LookupDType(int16 DType, rID xID)
  {
    const char *txt; String *msgs;
    txt = Lookup(MMDTypeNames,DType);
    if (DType == AD_SPE1 || DType == AD_SPE2) {
      msgs = (RES(xID)->GetMessages(MSG_SPE1DESC + (DType - AD_SPE1)));
      if (msgs[0].GetLength() > 0)
        txt = XPrint(msgs[0]);
      else
        txt = "(something special)"; 
    }
    return txt;
  }

String & Monster::Describe(Player *p)
  {
    String str, s2, s3, *sl;
    static String List[512]; rID sID, xID; Dice d; hObj h;
    int8 MType1, MType2, MType3, lc, sv, n, feats,skills,CR;
    uint8 i;
    String groupStr; 
    /* HACKFIX */
    static MonMem PerfectMem = {
      255, 255, 255, 255, 511, 255, 65535, 1, 1, 65535, 31 };
    TextVal AbilDescs[] = {
      { CA_AURA_OF_MENACE, "It radiates an aura of menace in a %d0-foot radius. " },
      { CA_BERSERK_RAGE, "It can fly into a berserk rage. " },
      { CA_BLINDSIGHT, "It has blindsight to a range of %d0 ft. " },
      { CA_BOOST_RESISTS, "It has magical resistance of %+d to all attack forms. " },
      { CA_COMMAND, "It can command undead as a level %d cleric would. " },
      { CA_DIVINE_GRACE, "It has an aura of divine grace. " },
      { CA_EARTHMELD, "It can phase through earth and stone as easily as walking through air. " },
      { CA_EVASION, "It can evade area attacks just as a rogue or monk does. " },
      { CA_FLIGHT, "It can fly very fast. " },
      { CA_INFRAVISION, "It has infravision to a range of %d0 ft. " },
      { CA_KI_STRIKE, "Its natural attacks count as magical %+d weapons. " },
      { CA_LAY_ON_HANDS, "It can lay on hands as a level %d paladin would. " },
      { CA_LOWLIGHT, "It has lowlight vision to a range of %d0 ft. " },
      { CA_NATURE_SENSE, "It sees nature as a druid would. " },
      { CA_PHASE, "It can phase in and out of the %s at will. " },
      { CA_REGEN, "It regenerates %d hit points every round. " },
      { CA_SCENT, "It can track foes by smell to a range of %d0 ft. " },
      { CA_SENSE_MAGIC, "It innately senses the presence of magic. " },
      { CA_SMITE, "It can smite as a level %d paladin would. " },
      { CA_SNEAK_ATTACK, "It can sneak attack for +%dd6 damage. " },
      //{ CA_SPAWN, "A living creature it kills will later rise as a %s. " },
      { CA_STALKER, "It will pursue its quarry, even across levels. " },
      { CA_TELEPATHY, "It has telepathy to a range of %d0 ft. " },
      { CA_TOUGH_AS_HELL, "It is receives double Con bonus to hit points, like a barbarian does. " },
      { CA_TREMORSENSE, "It has tremorsense to a range of %d0 ft. " },
      { CA_TUNNEL, "It can burrow through earth and stone. " },
      { CA_TURNING, "It can turn undead as a level %d cleric would. " },
      { CA_UNCANNY_DODGE, "It avoids being caught flat-footed or flanked. " },
      { CA_WALLWALK, "It can walk along walls and ceilings. " },
      { CA_WEAPON_IMMUNITY, "It is unaffected by weapons with a plus less than %d. " },
      { CA_WOODLAND_STRIDE, "It is unaffected by adverse terrain like webs or brimstone. " },
      { -M_AMORPH, "It is amorphious. " },
      { -M_AMPHIB, "It is amphibious, crossing freely from land to water. " },
      { -M_AQUATIC, "It is an aquatic creature and breathes water naturally. " },
      { -M_AVOID_MELEE, "It avoids melee when possible, prefering to fight at range. " },
      { -M_BLIND, "It has no natural vision. " },
      { -M_BURROW, "It is a burrowing mammal, and gnomes can befriend it. " },
      { -M_CASTER, "It relies heavily on spellcasting, and flees when low on mana. " },
      { -M_COVETOUS, "It pursues anyone who steals its items relentlessly. " },
      { -M_DOMESTIC, "It can be easily domesticated. " },
      { -M_FLYER, "It flies naturally. " },
      { -M_FRIENDLY, "It is often created peaceful. " },
      { -M_GEMS, "It values gems far more highly than most monsters do. " },
      { -M_GREEDY, "It values items more highly than most monsters do. " },
      { -M_HOSTILE, "It is never created peaceful. " },
      { -M_INCOR, "It is incorporeal. " },
      { -M_LIGHT_AVERSE, "It avoids light, suffering penalties in lit areas and double damage from light-based attacks. " },
      { -M_MFLYER, "It flies magically. " },
      { -M_MINDLESS, "It is mindless, and is not detected by telepathy. " },
      { -M_MINION, "It is always created in service to a deity. " },
      { -M_MOUNTABLE, "It can be used as a mount by humanoids. " },
      { -M_NOGENO, "It cannot be genocided. " },
      { -M_NOPOLY, "Polymorph spells cannot turn you into this creature. " },
      { -M_NO_CORPSE, "It does not leave a normal corpse when killed. " },
      { -M_OLD_CORPSE, "It leaves an old, disease-ridden corpse when killed. " },
      { -M_POISON, "Its corpse is poisonous. " },
      { -M_PSYCHO, "It never flees in combat. " },
      { -M_REACH, "It has natural reach, and may attack creatures two squares distant in melee. " },
      { -M_SEE_INVIS, "It can see invisible creatures. " },
      { -M_SKIRMISH, "It prefers hit and run tactics in combat. " },
      { -M_SOLITARY, "It is never encountered in groups. " },
      { -M_UNIQUE, "It is a unique, singular being. " },
      { -M_VARGROUP, "It is encountered in mixed groups of its creature type. " },
      { -M_XTRA_TOUCH, "It gets a bonus touch attack along with every melee attack it makes. " },
      { 0, NULL } };
    const char* SaveNames[] = { "Fortitude", "Reflex", "Will" };
    int8        SaveAttrs[] = { A_CON, A_DEX, A_WIS };

    MonMem * mm; TMonster *tm; TTemplate *tt; xID = mID;
    if (HasStati(POLYMORPH) && !(StateFlags & MS_POLY_KNOWN))
      xID = mID;
    else if (!(StateFlags & MS_KNOWN)) {
      if (TMON(xID)->HasAttk(A_SEEM))
        xID = GetAttk(A_SEEM)->u.xID;
      }
    else
      xID = tmID;

    tm = TMON(xID);

    //if (p->WizOpt(OPT_PERFECT_MONMEM))
      mm = &PerfectMem;
    //else      
    //  mm = MONMEM(mID,p);
    
    CR = tm->CR;
    if (isMType(MA_DRAGON))
      CR += 9;
	uint32 gid = GLYPH_ID_VALUE(tm->Image);
    str = XPrint(
        Format("<5>The <Str><Str><7> ('<%d>%c%c%c<7>')\n__", GLYPH_COLOUR_VALUE(tm->Image), LITERAL_CHAR, LITERAL_CHAR1(gid), LITERAL_CHAR2(gid)),
        isMType(MA_DRAGON) ? "(Adult) " : "", NAME(xID));
    for(i=1;i!=str.GetLength();i++)
      if (isalpha(max(1,str[i])) && str[i-1] == ' ')
        str.SetAt(i,toupper(str[i]));
    
    if (tm->Desc) {
      str += DESC(xID);
      str += "\n__";
      }
    
    if (tm->HasFlag(M_UNKNOWN))
      goto SkipMonsterStats;

    /*
    if (mm->Kills == 0 && mm->Deaths == 0)
      str += "No battles to the death are recalled";
    else if (mm->Kills)
      str += Format("Your fellow heros have killed %d of these creatures. ",mm->Kills);
    if (mm->Deaths)
      str += Format(", and it has claimed the %s of %d of their number. ",
        mm->Deaths == 1 ? "life" : "lives", mm->Deaths);
        */
        
    str += Format("This %s ",Lookup(SizeNames,tm->Size));

    if (tm->HasFlag(M_CARNI) && HasMFlag(M_HERBI))
      str += "omnivorous ";
    else if (tm->HasFlag(M_CARNI))
      str += "carnivorous ";
    else if (tm->HasFlag(M_HERBI))
      str += "herbivorous ";

    if (tm->HasFlag(M_NATURAL))
      str += "natural ";

    MType1 = (int8)tm->MType[0];
    MType2 = (int8)tm->MType[1];
    MType3 = (int8)tm->MType[2];

    s2 = Lower(Lookup(MTypeNames,MType1));
    

    str += s2;

    str += " is found ";
    lc = 0;
    if (tm->HasFlag(M_SWAMP))
      List[lc++] = "swamps";
    if (tm->HasFlag(M_DESERT))
      List[lc++] = "deserts";
    if (tm->HasFlag(M_FOREST))
      List[lc++] = "forests";
    if (tm->HasFlag(M_PLAINS))
      List[lc++] = "plains";
    if (tm->HasFlag(M_CITY))
      List[lc++] = "cities";
    if (tm->HasFlag(M_MOUNTAIN))
      List[lc++] = "mountains";
    if (tm->HasFlag(M_ARCTIC))
      List[lc++] = "tundra and arctic regions";
    if (tm->HasFlag(M_OCEAN))
      List[lc++] = "oceans";
    //if (HasFlag(M_VILLAGE))
    //  List[lc++] = "villages";

    if (lc)
      str += "in ";

    for(i=0;i<lc-1;i++) {
      str += List[i];
      if (i>lc-2)
        str += ", ";
      }
    if (lc > 1)
      str += " and ";
    if (lc)
      str += List[lc-1];

    if (!tm->HasFlag(M_NOGEN))
      str += Format("%sat depths of %dm in an average dungeon",
        lc ? ", as well as " : "", max(10,CR*10));
    else if (!lc)
      str += "only in very specific places";
    
    str += ". It is ";

    if (tm->HasFlag(M_IALIGN))
      str += "always ";
    else if (isMType(MA_SAPIENT))
      str += "by default ";
    else
      str += "usually ";
    if (!tm->HasFlag(M_LAWFUL) && !tm->HasFlag(M_CHAOTIC) &&
        !tm->HasFlag(M_EVIL) && !tm->HasFlag(M_GOOD))
      str += "True Neutral";
    else {
      if (tm->HasFlag(M_LAWFUL))
        str += "Lawful ";
      else if (tm->HasFlag(M_CHAOTIC))
        str += "Chaotic ";
      else
        str += "Neutral ";
      if (tm->HasFlag(M_GOOD))
        str += "Good";
      else if (tm->HasFlag(M_EVIL))
        str += "Evil";
      else
        str += "Neutral";
      }
    str += ". ";
        
    if (CR <= 0) {
      str += -SKYBLUE;
      switch (CR) {
        case 0: str += "Two"; break;
        case -1: str += "Three"; break;
        case -2: str += "Four"; break;
        case -3: str += "Five"; break;
        case -4: str += "Six"; break;
        case -5: str += "Seven"; break;
        case -6: str += "Eight"; break;
        }
      str += -GREY;
      str += " of them together are similar in power to a 1st level character. ";
      }        
    else
      str += Format("It is similar in power to a %c%d%s%c level character. ",
        -SKYBLUE, CR, NumPrefix(CR), -GREY);

    if (mm->Kills >= 3) {
      if (tm->HasFlag(M_FIXED_HP))
        str += Format("It has %c%d%c hit points",-WHITE,tm->HitDice,-GREY);
      else {
        int attr = A_CON;
        if (tm->Attr[A_CON] == 0) attr = A_WIS; 

        if ((tm->Attr[attr]-10)/2 == 0 || tm->Attr[attr] == 0)
          str += Format("It has %c%dd%d%c hit points",-YELLOW,tm->HitDice, BestHDType(),-GREY);
        else
          str += Format("It has %c%dd%d%+d%c hit points",-YELLOW,tm->HitDice, BestHDType(), tm->HitDice*((tm->Attr[attr]-10)/2),-GREY);
        
        if (tm->Size != SZ_MEDIUM && !tm->HasFlag(M_NO_SIZE_HP)) {
          str += ", with a ";
          switch(tm->Size) {
            case SZ_MINISCULE:
              str += "50~ decrease";
             break;
            case SZ_TINY:
              str += "30~ decrease";
             break;
            case SZ_SMALL:
              str += "10~ decrease";
             break;
            case SZ_LARGE:
              str += "10~ increase";
             break;
            case SZ_HUGE:
              str += "30~ increase";
             break;
            case SZ_GARGANTUAN:
              str += "60~ increase";
             break;
            case SZ_COLLOSAL:
              str += "100~ increase";
             break;
            }
          str += " due to size";
          }
        }
      str += Format(", a base attack bonus of %c%+d%c, and a defense class of %c%d%c. ",
          -YELLOW, tm->Hit, -GREY, -YELLOW, tm->Def, -GREY);
      
      if (tm->Arm)
          str += Format("It has a natural armour rating of %c%d%c. ", -YELLOW, tm->Arm, -GREY);

      }

 

    /* Movement and Speed */
    if (tm->Mov == -20)
      str += Format("It is stationary, and attacks at %c%d~%c speed with natural attacks. ",
                     -YELLOW,tm->Spd*5+100,-GREY);
    else
      str += Format("It moves at %c%d~%c of normal human speed, and attacks at "
                    "%c%d~%c speed with natural attacks. ",-YELLOW,tm->Mov*5+100,-GREY,-YELLOW,tm->Spd*5+100,-GREY);

    /* Saving Throws */
    sv = MonGoodSaves((int8)tm->MType[0]);
    if (tm->MType[1])
      sv |= MonGoodSaves((int8)tm->MType[1]);
    if (tm->MType[2])
      sv |= MonGoodSaves((int8)tm->MType[2]);
    str += "It has ";
    if (sv) {
      str += "naturally strong ";
      for(i=0;i!=3;i++)            
        if (sv & XBIT(i)) {
          str += SaveNames[i];
          if (mm->Kills > 50) {
            n =  max(CR,0)*3/2;
            n += (tm->Attr[SaveAttrs[i]]-10) / 2;
            str += Format(" (%+d)",n);
            }
          if (i == 0 && sv == 7)
            str += ", ";
          else if (sv >= XBIT(i+1))
            str += " and ";
          }
      }
    if (sv != 7) {
      if (sv)
        str += ", but weak ";
      else
        str += "naturally weak ";
      for(i=0;i!=3;i++)
        if (!(sv & XBIT(i))) {
          str += SaveNames[i];
          if (mm->Kills > 50) {
            n =  max(CR,0)*3/4;
            n += (tm->Attr[SaveAttrs[i]]-10) / 2;
            str += Format(" (%+d)",n);
            }
          if (i == 0 && !sv)
            str += ", ";
          else if ((7 & ~sv) >= XBIT(i+1))
            str += " and ";
          }
      }
    str += ". ";

    if (mm->Kills >= 10) {
      str += "It has ";
      for(i=0;;i++) {
        if (tm->Attr[i] == 0)
          str += "no measurable";
        else if (tm->Attr[i] <= 4)
          str += "exceedingly low";
        else if (tm->Attr[i] <= 7)
          str += "very low";
        else if (tm->Attr[i] <= 9)
          str += "low";
        else if (tm->Attr[i] <= 12)
          str += "average";
        else if (tm->Attr[i] <= 14)
          str += "above average";
        else if (tm->Attr[i] <= 16)
          str += "high";
        else if (tm->Attr[i] <= 18)
          str += "exceptional";
        else if (tm->Attr[i] <= 22)
          str += "superhuman";
        else if (tm->Attr[i] <= 26)
          str += "unearthly";
        else
          str += "godlike";
        str += Format("%c",-SKYBLUE);
        switch (i) {
          case A_STR: str += " strength";  break;
          case A_DEX: str += " dexterity"; break;
          case A_CON: str += " constitution"; break;
          case A_INT: str += " intelligence"; break;
          case A_WIS: str += " wisdom"; break;
          case A_CHA: str += " charisma"; break;
        }

        if (mm->Kills - (20+i*2) >= 0 && tm->Attr[i] != 0)
          str += Format(" (%d)",tm->Attr[i]);
        str += Format("%c",-GREY);

        if ((i == 1 && mm->Kills < 15) || i == 4)
          str += " and ";
        else if ((mm->Kills < 15 && i == 2) || i == 5)
          { str += ". "; break; }
        else
          str += ", ";
        }
      }


    if (tm->Feats[0]) {
      str += "It has the ";
      feats = skills = 0;
      for(i=0;tm->Feats[i] && i!=16;i++)
        if (tm->Feats[i] > SK_LAST)
          feats++;
        else
          skills++;
      if (feats) {
        str += feats == 1 ? "feat " : "feats ";
        lc = 0;
        for(i=0;tm->Feats[i] && i!=16;i++)
          if (tm->Feats[i] > SK_LAST)
            {
              str += XPrint("<5><str><7>",FeatName(tm->Feats[i]));
              if (tm->Feats[i] == FT_SKILL_FOCUS)
                str += XPrint(" <5>(<str>)<7>",
                    tm->Feats[i+1] < SK_LASTSKILL ?
                    SkillInfo[tm->Feats[i+1]].name : "invalid");
              lc++;
              if (lc >= 511)
                break;
              if (feats == 1 || feats == lc)
                continue;
              if (feats-1 == lc)
                str += " and ";
              else
                str += ", ";
            }

        if (skills) 
          str += ", and the ";
        }
      if (skills) {
        str += skills == 1 ? "skill " : "skills ";
        lc = 0;
        for(i=0;tm->Feats[i] && i!=16;i++)
          if (tm->Feats[i] <= SK_LAST)
            {
              str += XPrint("<2><str><7>",
                    SkillInfo[tm->Feats[i]].name);
              /* Kludge! */
              if (mm->Kills > 20)
                str += Format(" (%+d)",SkillLevel(tm->Feats[i]) - 
                                       ChallengeRating() + CR -
                                       Mod((int8)SkillAttr(tm->Feats[i])) +
                                       (tm->Attr[(int8)SkillAttr(tm->Feats[i])]-10)/2);
              lc++;
              if (lc >= 511)
                break;
              if (skills == 1 || skills == lc)
                continue;
              if (skills-1 == lc)
                str += " and ";
              else
                str += ", ";
            }
        }
      str += ". ";
      }

    /* Resistances & Immunities */
    if (tm->Res) {
      lc = 0;
      for(i=1;i!=17;i++)
        if (tm->Res & BIT(i))
          if (LookupOnly(DTypeNames,i)) 
            List[lc++] = Lookup(DTypeNames,i);
            
      ASSERT(lc);
      str += "It is resistant to ";
      if (lc > 1) {
        for(i=0;i!=lc;i++)
          if (i == lc-1)
            str += List[i].Lower();
          else if (i == lc-2)
            str += List[i].Lower() + SC(" and ");
          else
            str += List[i].Lower() + SC(", ");
        }
      else
        str += List[0].Lower();
      str += tm->Imm ? ", and immune to " : ". ";
      }
    if (tm->Imm) {
      if (!tm->Res)
        str += "It is immune to ";
      lc = 0;
      for(i=1;i!=33;i++)
        if (tm->Imm & BIT(i)) {
          if (LookupOnly(DTypeNames,i)) 
            List[lc++] = Lookup(DTypeNames,i);
          else
            Error("Cannot look up immune type %d!",i);
          }
            
      ASSERT(lc);
      if (lc > 1) {
        for(i=0;i!=lc;i++)
          if (i == lc-1)
            str += List[i].Lower();
          else if (i == lc-2)
            str += List[i].Lower() + " and ";
          else
            str += List[i].Lower() + ", ";
        }
      else
        str += List[0];
      str += ". ";
      }




    /* Innate Abilities */
    if (tm->FirstRes(AN_INNATE)) {
      if (isMType(MA_EYE))
        str += "Its eye rays allow it the innate function";
      else
        str += "At will, it can use the innate spell";
      lc = 0;
      for(sID = tm->FirstRes(AN_INNATE);sID;sID = tm->NextRes(AN_INNATE))
        List[lc++] = XPrint("<1><str><7>",NAME(sID));
      ASSERT(lc);
      if (lc > 1) {
        str += isMType(MA_EYE) ? "s of " : "s ";
        for(i=0;i!=lc;i++)
          if (i == lc-1)
            str += List[i];
          else if (i == lc-2)
            str += List[i] + " and ";
          else
            str += List[i] + ", ";
      } else { str += " "; str += List[0]; }
      str += ". ";
    }
    
    if (tm->HasAttk(A_FORM)) {
      lc = 0;
      for(i=0;i!=NUM_TMON_ATTK;i++)
        if (tm->Attk[i].AType == A_FORM) 
          List[lc++] = SC("a ") + SC(NAME(tm->Attk[i].u.xID));
      str += "It can take the form of ";
      for(i=0;i<lc-1;i++) {
        str += List[i];
        if (i<lc-2)
          str += ", ";
        }
      if (lc > 1)
        str += " and ";
      if (lc)
        str += List[lc-1];
      if (lc)
        str += ". ";
      }

    for (i=0;i<NUM_TMON_STATI;i++)
      if (tm->Stati[i].Nature == ADJUST_NAT && tm->Stati[i].Val == A_MR)
          str += Format("It has %+d~ magic resistance. ", tm->Stati[i].Val);

    lc = 0; 
    for (i=0;i<NUM_TMON_STATI;i++) {
      Status * S = & tm->Stati[i]; 
        if (S->Nature == SKILL_BONUS) {
          List[lc++] = XPrint(Format("<2>%s<7> (%+d)",
              SkillInfo[S->Val].name, S->Mag));
          if (lc >= 511)
            break;
          }
    }
    if (lc) { 
      str += "It has a racial skill bonus to ";
      for(i=0;i<lc-1;i++) {
        str += List[i];
        if (i<lc-2)
          str += ", ";
        }
      if (lc > 1)
        str += " and ";
      if (lc)
        str += List[lc-1];
      if (lc)
        str += ". ";
    } 
    

    for (i=0; i<NUM_TMON_STATI; i++) {
      Status * S = & tm->Stati[i]; 
      if (S->Nature == EXTRA_ABILITY) { 
          if (S->Val == CA_SPELLCASTING)
            {
              rID spList[512]; String cl, sc;
              cl = "caster, from its own list";
              memset(spList,0,sizeof(spList));
              tm->GetList(SPELL_LIST,spList,511);
              sc = "";
              for (int16 n=0;n!=511;n++)
                switch (spList[n])
                  {
                    case 1000 + AI_THEURGY:  cl = "priest"; break;
                    case 1000 + AI_WIZARDRY: cl = "mage"; break;
                    case 1000 + AI_DRUIDIC:  cl = "druic"; break;
                    case 100:
                      spList[n+1] += 1000;
                      if (sc.GetLength())
                        sc += "and ";
                      sc += Lower(Lookup(SchoolNames,spList[n+2]));
                      sc += " ";
                     break;
                  }
              str += Format("In can cast %c%s%cspells as a %c%d%s%c level %s.",
                -YELLOW, (const char*) sc, -GREY,
                -SKYBLUE, S->Mag, (S->Mag % 10) == 1 ? "st" :
                                  (S->Mag % 10) == 2 ? "nd" :
                                  (S->Mag % 10) == 3 ? "rd" : "th", -GREY,
                (const char *) cl);
                
            }
          else if (S->Val == CA_PHASE)
            str += Format(s2,S->Mag == PHASE_ASTRAL ?
              "astral plane" : S->Mag == PHASE_ETHEREAL ?
              "ethereal plane" : "plane of shadow");
          else if (S->Val == CA_WEAPON_IMMUNITY) {
            s2 = S->Mag <= 1 ? "mundane" :
              Format("%+d or lower",S->Mag-1);
            if (isMType(MA_DEVIL))
              str += SC("It is immune to ") + s2 + SC(" weapons, but blessed weapons strike true. ");
            else if (isMType(MA_LYCANTHROPE))
              str += SC("It is immune to ") + s2 + SC(" weapons, but silver weapons strike true. ");
            else if (isMType(MA_UNDEAD) || isMType(MA_DEMON))
              str += SC("It is immune to ") + s2 + SC(" weapons, but blessed or silver weapons strike true. ");
            else
              str += SC("It is immune to ") + s2 + SC(" weapons. ");
            }
          else if (s2 = LookupOnly(AbilDescs,S->Val))
            str += Format(s2,S->Mag);
        }
    }

    for(i=0;i!=NUM_TMON_ATTK;i++)
      switch (tm->Attk[i].AType)
      {
          case A_GATE:
            str += Format("It can gate in %s worth CR %d, with a %d~ chance of success. ",
              (const char*)Pluralize(Lookup(MTypeNames, tm->Attk[i].DType)), 
              tm->Attk[i].u.a.DC, tm->Attk[i].u.a.Dmg.Bonus);
           break;
        }
    
    int16 fearDC;
    if (fearDC = (int16)tm->GetConst(DRAGON_FEAR_DC))
      str += Format("Saving throws against its fear attack are %s by %+d. ",
               fearDC > 0 ? "increased" : "decreased", fearDC);

    sl = tm->GetMessages(MSG_SPECABIL);
    if (*sl)
      if (sl->GetLength()) {
        str += XPrint(*sl);
        str += " ";
      }

    for (i=0;AbilDescs[i].Val;i++)
      if (AbilDescs[i].Val < 0)
        if (TMON(mID)->HasFlag((uint16)-AbilDescs[i].Val))
          str += AbilDescs[i].Text;
              
      
    /* Attacks */
    lc = 0;
    int16 true_attk;
    for(i=0;i!=NUM_TMON_ATTK;i++)
      if (!is_ability_attk(tm->Attk[i].AType) && tm->Attk[i].AType) {
        if ((!LookupOnly(MMAttkVerbs,tm->Attk[i].AType) 
               && tm->Attk[i].AType != A_ALSO)||
            !LookupDType(tm->Attk[i].DType, xID))
          continue;
        if (tm->Attk[i].AType == A_ALSO)
          { if (!lc) continue; 
            List[--lc] += " and "; }
        else {
          List[lc] = Lookup(MMAttkVerbs,tm->Attk[i].AType);
          List[lc] += " ";
          true_attk = tm->Attk[i].AType;
          if (true_attk == A_BREA || true_attk == A_BRE2)
            { 
              String *s; int16 n;
              s = tm->GetMessages(MSG_BLASTNAME);
              n = (true_attk == A_BREA) ? 0 : 1;
              if (s[n].GetLength()) {
                if (strstr("aeiou",s[n].Left(1)))
                  List[lc] += SC("an ") + s[n] + SC(" ");
                else
                  List[lc] += SC("a ") + s[n] + SC(" ");
                }
            }
          }
        d = tm->Attk[i].u.a.Dmg;
        if (isMType(MA_DRAGON) && (true_attk == A_BREA
               || true_attk == A_BRE2))
          d.Number += 12; 
        else if (isMType(MA_DRAGON))
          d.Bonus += 6; 
        List[lc] += Format(LookupDType(tm->Attk[i].DType,xID),(const char*)d.Str());
        if (tm->Attk[i].u.a.DC)
          List[lc] += Format(" (DC %d)", tm->Attk[i].u.a.DC +
            (isMType(MA_DRAGON) ? 6 : 0));
        lc++;
        if (lc >= 511)
          break;
        }
    if (!lc)
      str += "It has no active attacks. ";
    else {
      str += "To attack, it ";   
      for(i=0;i<lc-1;i++) {
        str += List[i];
        if (i<lc-2)
          str += ", ";
        }
      if (lc > 1)
        str += " and ";
      if (lc)
        str += List[lc-1];
      if (lc)
        str += ". ";
      }
    
    for(i=0;i!=NUM_TMON_ATTK;i++)
      if (tm->Attk[i].AType) {
        if (!LookupOnly(MMAttkVerbs2,tm->Attk[i].AType) ||
            !LookupOnly(DTypeNames,tm->Attk[i].DType))
          continue;
        s2 = Lookup(MMAttkVerbs2,tm->Attk[i].AType);
        s3 = "";
        
        true_attk = tm->Attk[i].AType;
        
        SpecAlso:

        d = tm->Attk[i].u.a.Dmg;
        if (isMType(MA_DRAGON) && (true_attk == A_BREA
               || true_attk == A_BRE2))
          d.Number += 12; 
        else if (isMType(MA_DRAGON))
          d.Bonus += 6; 
        s3 += Format(LookupDType(tm->Attk[i].DType,xID),(const char*)d.Str());
        if (tm->Attk[i].u.a.DC)
          s3 += Format(" (DC %d)", tm->Attk[i].u.a.DC +
            (isMType(MA_DRAGON) ? 6 : 0));
        lc++;
        if (lc >= 511)
          break;
        if (i != 8 && tm->Attk[i+1].AType == A_ALSO)
          { i++; s3 += " and "; goto SpecAlso; }

        str += Format(s2, (const char*)s3);

        }


    if (Inv) {
      h = Inv; lc = 0;
      while (h) {
        if (p && p->Perceives(oThing(h)))
          {
            if (oItem(h)->IFlags & IF_WORN)
              List[lc] = Format("%c%s (equipped)%c",-SKYBLUE,(const char*)oThing(h)->Name(NA_A),-GREY);
            else 
              List[lc] = oThing(h)->Name(NA_A);
            lc++;
            if (lc >= 511)
              break;
          }
        h = oThing(h)->Next;
        }
      if (lc) {
        str += XPrint("\n__<14>Equipment:<7> This particular <Obj> carries ",this);
        for(i=0;i<lc-1;i++) {
          str += List[i];
          if (i<lc-2)
            str += ", ";
          }
        if (lc > 1)
          str += " and ";
        if (lc)
          str += List[lc-1];
        if (lc)
          str += ". ";
        }
      }

    if (p && p->WizardMode)
      {
        groupStr = XPrint("\n__<14>Groups:<7> ");
        // ww: players need to know this info so that they can tell when Bane
        // will work (etc.)
        // fjm: I want this to be concealed so that players think about the
        // monsters as creatures in an immersion sense, rather than as sets
        // of numbers. Usually, you can tell from a monster's description if
        // it bleeds, has blood, etc.
        for (i=0;i<MA_CHOICE1;i++) 
          if (i != 101 && this->isMType(i)) {
            const char * c = LookupOnly(MTypeNames,i);
            if (c) 
              groupStr += Format("%s, ",(const char*)c);
          }
          /* TODO: Correct workaround, as strrchr cannot be implemented in the custom String class. RICHARD */
          const char *str1 = groupStr.GetData();
          const char *str2 = strrchr(str1, ',');
          groupStr.SetAt(str2-str1, ',');

        //* (strrchr((char*)tmpstr,',')) = '.';
        str += groupStr;
      }

    for (xID=tm->FirstRes(AN_POISON);xID;xID=tm->NextRes(AN_POISON))
      str += XPrint(Format("\n__<12>%s:<7> %s",
               (const char*)Capitalize(NAME(xID),true),
               (const char*)DESC(xID)));
    for (xID=tm->FirstRes(AN_DISEASE);xID;xID=tm->NextRes(AN_DISEASE))
      str += XPrint(Format("\n__<12>%s:<7> %s",
               (const char*)Capitalize(NAME(xID),true),
               (const char*)DESC(xID)));
    for (xID=tm->FirstRes(AN_INNATE);xID;xID=tm->NextRes(AN_INNATE))
      str += XPrint(Format("\n\n%s",
               (const char*)TEFF(xID)->Describe(p)));

          #define DESCRIBE_TEMPLATE_MOD(mod,name)                     \
            if (tt->mod.VType !=  MVAL_NONE) {                        \
              switch (tt->mod.VType) {                                \
                case MVAL_ADD:                                        \
                  s2 = Format("%+d %s", (int)tt->mod.Value,name);     \
                 break;                                               \
                case MVAL_SET:                                        \
                  s2 = Format("%s set to %d",name,(int)tt->mod.Value);\
                 break;                                               \
                case MVAL_PERCENT:                                    \
                  s2 = Format("%d~ %s",(int)tt->mod.Value, name);     \
                 break;                                               \
                }                                                     \
              switch(tt->mod.BType) {                                 \
                case MBOUND_MIN:                                      \
                  s2 += Format(" (minimum %d)",(int)tt->mod.Bound);       \
                 break;                                               \
                case MBOUND_MAX:                                      \
                  s2 += Format(" (maximum %d)",(int)tt->mod.Bound);       \
                 break;                                               \
                case MBOUND_NEAR:                                     \
                  s2 += Format(" (adjusted toward %d)",(int)tt->mod.Bound);\
                 break;                                               \
                }                                                     \
              List[lc++] = s2;                                        \
              if (lc >= 511)                                          \
                break;                                                \
              }

    SkipMonsterStats:

    StatiIterNature(this,TEMPLATE)
      if (S->Mag) {
          tt = TTEM(S->eID);
          s3 = "\n\n<5>The ";
          if (tt->HasFlag(TMF_NAMEONLY))
            s3 += NAME(S->eID);
          else {
            if (!tt->HasFlag(TMF_POSTSCRIPT))
              { s3 += NAME(S->eID); s3 += " "; }
            switch(tt->ForMType) {
              case MA_ANIMAL:   s3 += "Animal"; break;
              case MA_HUMANOID: s3 += "Humanoid"; break;
              case MA_UNDEAD:   s3 += "Undead"; break;
              case MA_GOBLINOID: s3 += "Goblinoid"; break;
              case MA_DRAGON:   s3 += "Dragon"; break;
              default:          s3 += "Creature"; break;
              }
            if (tt->HasFlag(TMF_POSTSCRIPT))
              { s3 += " "; s3 += NAME(S->eID); }
            }
          s3 += " Template<7> (";
          lc = 0;
          if (tt->TType & TM_AGECAT)
            { List[0] = Format("%+d CR",tt->CR.Value-9); lc = 1; }
          else 
            { DESCRIBE_TEMPLATE_MOD(CR,"CR") }
          if (lc)
            s3 += List[0];
          else
            s3 += "(CR unchanged";
          s3 += ")\n__";
          
          for(i=1;i!=s3.GetLength();i++)
            if (isalpha(s3[i]) && s3[i-1] == ' ')
              s3.SetAt(i,toupper(s3[i]));
          str += XPrint(s3);

          if (tt->Desc)
            str += SC(DESC(S->eID)) + SC("\n__");


          lc = 0;
          DESCRIBE_TEMPLATE_MOD(Attr[A_STR],"Strength")
          DESCRIBE_TEMPLATE_MOD(Attr[A_DEX],"Dexterity")
          DESCRIBE_TEMPLATE_MOD(Attr[A_CON],"Constitution")
          DESCRIBE_TEMPLATE_MOD(Attr[A_INT],"Intelligence")
          DESCRIBE_TEMPLATE_MOD(Attr[A_WIS],"Wisdom")
          DESCRIBE_TEMPLATE_MOD(Attr[A_CHA],"Charisma")
          DESCRIBE_TEMPLATE_MOD(HitDice,"hit dice")
          DESCRIBE_TEMPLATE_MOD(Hit,"attack bonus")
          DESCRIBE_TEMPLATE_MOD(Def,"defense class")
          DESCRIBE_TEMPLATE_MOD(Arm,"natural armour")
          DESCRIBE_TEMPLATE_MOD(Mov,"movement rate")
          DESCRIBE_TEMPLATE_MOD(Spd,"natural speed")
          DESCRIBE_TEMPLATE_MOD(Size,"size category")
          DESCRIBE_TEMPLATE_MOD(CasterLev,"caster level")
          DESCRIBE_TEMPLATE_MOD(Power,"special attack power and save DC")
          if (tt->DmgMod)
            List[lc++] = Format("%+d natural attack damage",tt->DmgMod);
          if (lc) {
            str += "It has the following modifiers: ";
            for(i=0;i<lc-1;i++) {
              str += List[i];
              if (i<lc-2)
                str += ", ";
              }
            if (lc > 1)
              str += " and ";
            str += List[lc-1];
            str += ". ";
            }
          if (tt->AddMType)
            str += Format("It gains the %s type. ",Lookup(MTypeNames,tt->AddMType));



          if (tt->NewFeats[0]) {
            str += "It has the bonus ";
            feats = skills = 0;
            for(i=0;tt->NewFeats[i] && i!=16;i++)
              if (tt->NewFeats[i] > SK_LAST)
                feats++;
              else
                skills++;
            if (feats) {
              str += feats == 1 ? "feat " : "feats ";
              lc = 0;
              for(i=0;tt->NewFeats[i] && i!=16;i++)
                if (tt->NewFeats[i] > SK_LAST)
                  {
                    str += XPrint("<5><str><7>",FeatName(tt->NewFeats[i]));
                    if (tt->NewFeats[i] == FT_SKILL_FOCUS)
                      str += XPrint(" <5>(<Str>)<7>",
                          SkillInfo[tt->NewFeats[i+1]].name);
                    lc++;
                    if (lc >= 511)
                      break;
                    if (feats == 1 || feats == lc)
                      continue;
                    if (feats-1 == lc)
                      str += " and ";
                    else
                      str += ", ";
                  }

              if (skills) 
                str += ", and the bonus ";
              }
            if (skills) {
              str += skills == 1 ? "skill " : "skills ";
              lc = 0;
              for(i=0;tt->NewFeats[i] && i!=16;i++)
                if (tt->NewFeats[i] <= SK_LAST)
                  {
                    str += XPrint("<2><str><7>",
                        SkillInfo[tt->NewFeats[i]].name);
                    lc++;
                    if (lc >= 511)
                      break;
                    if (skills == 1 || skills == lc)
                      continue;
                    if (skills-1 == lc)
                      str += " and ";
                    else
                      str += ", ";
                  }
              }
            str += ". ";
            }

    for (i=0;i<NUM_TMON_STATI;i++)
      if (tt->Stati[i].Nature == ADJUST_NAT && tt->Stati[i].Val == A_MR)
          str += Format("It gains %+d~ magic resistance. ", tt->Stati[i].Mag);

    lc = 0;
    for (i=0;i<NUM_TMON_STATI;i++)
      if (tt->Stati[i].Nature == SKILL_BONUS) {
        List[lc++] = Format("<2>%s<7> (%+d)",
            SkillInfo[tt->Stati[i].Val].name, tt->Stati[i].Mag);
      } 
    if (lc) { 
      str += "It gains a racial skill bonus to ";
      for(i=0;i<lc-1;i++) {
        str += XPrint(List[i]);
        if (i<lc-2)
          str += ", ";
        }
      if (lc > 1)
        str += " and ";
      if (lc)
        str += XPrint(List[lc-1]);
      if (lc)
        str += ". ";
    } 

          /* Resistances & Immunities */
          if (tt->AddRes) {
            str += "It gains resistance to ";
            lc = 0;
            for(i=1;i!=17;i++)
              if (tt->AddRes & BIT(i))
                if (LookupOnly(DTypeNames,i)) 
                  List[lc++] = Lookup(DTypeNames,i);
            
            ASSERT(lc);
            if (lc > 1) {
              for(i=0;i!=lc;i++)
                if (i == lc-1)
                  str += List[i].Lower();
                else if (i == lc-2)
                  str += List[i].Lower() + " and ";
                else
                  str += List[i].Lower() + ", ";
              }
            else
              str += List[0].Lower();
            str += (tt->SubRes|tt->SubImm) ? ", but loses any existing resistance to " : ". ";
            }
          if ((tt->SubRes|tt->SubImm)) {
            if (!tt->AddRes)
              str += "It loses any existing resistance to ";
            lc = 0;
            for(i=1;i!=33;i++)
              if ((tt->SubRes|tt->SubImm) & BIT(i))
                if (LookupOnly(DTypeNames,i)) 
                  List[lc++] = Lookup(DTypeNames,i);
            
            ASSERT(lc);
            if (lc > 1) {
              for(i=0;i!=lc;i++)
                if (i == lc-1)
                  str += List[i].Lower();
                else if (i == lc-2)
                  str += List[i].Lower() + " and ";
                else
                  str += List[i].Lower() + ", ";
              }
            else
              str += List[0].Lower();
            str += ". ";
            }

          if (tt->AddImm) {
            str += "It gains immunity to ";
            lc = 0;
            for(i=1;i!=33;i++)
              if (tt->AddImm & BIT(i))
                if (LookupOnly(DTypeNames,i)) 
                  List[lc++] = Lookup(DTypeNames,i);
            
            ASSERT(lc);
            if (lc > 1) {
              for(i=0;i!=lc;i++)
                if (i == lc-1)
                  str += List[i].Lower();
                else if (i == lc-2)
                  str += List[i].Lower() + " and ";
                else
                  str += List[i].Lower() + ", ";
              }
            else
              str += List[0].Lower();
            str += ". ";
            }
          

          /* Innate Abilities */
          if (tt->FirstRes(AN_INNATE)) {
            str += "At will, it can use the innate spell";
            lc = 0;
            for(sID = tt->FirstRes(AN_INNATE);sID;sID = tt->NextRes(AN_INNATE))
              List[lc++] = XPrint("<1><str><7>",NAME(sID));
            ASSERT(lc);
            if (lc > 1) {
              str += "s ";
              for(i=0;i!=lc;i++)
                if (i == lc-1)
                  str += List[i];
                else if (i == lc-2)
                  str += List[i] + " and ";
                else
                  str += List[i] + ", ";
              }
            else
              str += SC(" ") + List[0];
            str += ". ";
            }
    
          if (tt->HasAttk(A_FORM)) {
            lc = 0;
            for(i=0;i!=16;i++)
              if (tt->NewAttk[i].AType == A_FORM) 
                List[lc++] = SC("a ") + SC(NAME(tt->NewAttk[i].u.xID));
            str += "It can take the form of ";
            for(i=0;i<lc-1;i++) {
              str += List[i];
              if (i<lc-2)
                str += ", ";
              }
            if (lc > 1)
              str += " and ";
            if (lc)
              str += List[lc-1];
            if (lc)
              str += ". ";
            }


          for (i=0;i<NUM_TMON_STATI;i++)
            if (tt->Stati[i].Nature == EXTRA_ABILITY) { 
              if (s2 = LookupOnly(AbilDescs,tt->Stati[i].Val)) {
                Status *S = &tt->Stati[i];
                if (S->Val == CA_SPELLCASTING)
                  {
                    rID spList[512]; String cl, sc;
                    cl = "caster, from its own list";
                    memset(spList,0,sizeof(spList));
                    tm->GetList(SPELL_LIST,spList,511);
                    sc = "";
                    for (int16 n=0;n!=511;n++)
                      switch (spList[n])
                        {
                          case 1000 + AI_THEURGY:  cl = "priest"; break;
                          case 1000 + AI_WIZARDRY: cl = "mage"; break;
                          case 1000 + AI_DRUIDIC:  cl = "druic"; break;
                          case 100:
                            spList[n+1] += 1000;
                            if (sc.GetLength())
                              sc += "and ";
                            sc += Lower(Lookup(SchoolNames,spList[n+2]));
                            sc += " ";
                          break;
                        }
                    str += Format("In can cast %c%s%cspells as a %c%d%s%c level %s.",
                      -YELLOW, (const char*) sc, -GREY,
                      -SKYBLUE, S->Mag, (S->Mag % 10) == 1 ? "st" :
                                        (S->Mag % 10) == 2 ? "nd" :
                                        (S->Mag % 10) == 3 ? "rd" : "th", -GREY,
                      (const char *) cl);      
                  }
                else if (tt->Stati[i].Val == CA_PHASE)
                  str += Format(s2,tt->Stati[i].Mag == PHASE_ASTRAL ?
                    "astral plane" : tt->Stati[i].Mag == PHASE_ETHEREAL ?
                    "ethereal plane" : "plane of shadow");
                else
                  str += Format(s2,tt->Stati[i].Mag);
                }
            } 

          /* Special Abilities */
          for(i=0;i!=16;i++)
            switch (tt->NewAttk[i].AType) 
              {
                case A_GATE:
                  str += Format("It can gate in %s worth CR %d, with a %d~ chance of success. ",
                    (const char*)Pluralize(Lookup(MTypeNames, tt->NewAttk[i].DType)), 
                    tt->NewAttk[i].u.a.DC, tt->NewAttk[i].u.a.Dmg.Bonus);
                 break;

              }

        sl = tt->GetMessages(MSG_SPECABIL);
        if (*sl)
          if (sl->GetLength()) {
            str += XPrint(*sl);
            str += " ";
          }
              
      
        /* Attacks */
        lc = 0;
        for(i=0;i!=16;i++)
          if (!is_ability_attk(tt->NewAttk[i].AType) && tt->NewAttk[i].AType) {
            if ((!LookupOnly(MMAttkVerbs,tt->NewAttk[i].AType)
                   && tm->Attk[i].AType != A_ALSO)||
                !LookupDType(tt->NewAttk[i].DType,S->eID))
              continue;
            if (tt->NewAttk[i].AType == A_ALSO)
              List[--lc] += " and ";
            else {
              List[lc] = Lookup(MMAttkVerbs,tt->NewAttk[i].AType);
              List[lc] += " ";
              }
            d = tt->NewAttk[i].u.a.Dmg;
            List[lc] += Format(LookupDType(tt->NewAttk[i].DType,S->eID),(const char*)d.Str());
            if (tt->NewAttk[i].u.a.DC)
              List[lc] += Format(" (DC %d)", tt->NewAttk[i].u.a.DC);
            lc++;
            if (lc >= 511)
              break;
            }
          if (lc) {
            str += "In addition to its normal attacks, it can ";   
            for(i=0;i<lc-1;i++) {
              str += List[i];
              if (i<lc-2)
              str += ", ";
              }
            if (lc > 1)
              str += " and ";
            if (lc)
              str += List[lc-1];
            if (lc)
              str += ". ";
            }

          for (xID=tt->FirstRes(AN_POISON);xID;xID=tt->NextRes(AN_POISON))
            str += XPrint(Format("\n__<12>%s:<7> %s",
                     (const char*)Capitalize(NAME(xID),true),
                     (const char*)DESC(xID)));
          for (xID=tt->FirstRes(AN_DISEASE);xID;xID=tt->NextRes(AN_DISEASE))
            str += XPrint(Format("\n__<12>%s:<7> %s",
                     (const char*)Capitalize(NAME(xID),true),
                     (const char*)DESC(xID)));
          for (xID=tt->FirstRes(AN_INNATE);xID;xID=tt->NextRes(AN_INNATE))
            str += XPrint(Format("\n\n%s",
                     (const char*)TEFF(xID)->Describe(p)));
        }
    StatiIterEnd(this)

    return *tmpstr(str);
  }

#if 0

void Monster::UpdateMonsterMemory(int32 field, int32 value, bool obv)
  {
    int16 v;
    if (!(v = At(x,y).Visibility))
      return;

    for (i=0;i!=MAX_PLAYERS;i++)
      if (v * BIT(i))
        MONMEM(obv ? mID : tmID,i)->


  }

#endif

typedef struct mat_res {
  int dtype;
  int res;
} mat_res;

static int MRSort(const void *a, const void *b)
{
  mat_res *l = (mat_res *)a,
          *r = (mat_res *)b;
  return r->res - l->res; 
} 

String & ItemHardnessDesc(Item *it)
{
  mat_res mr[14] = {
    AD_ACID, 0, 
    AD_BLUNT, 0, 
    AD_COLD, 0, 
    AD_DCAY, 0, 
    AD_DISN, 0, 
    AD_FIRE, 0, 
    AD_MAGC, 0, 
    AD_NECR, 0, 
    AD_NORM, 0, 
    AD_RUST, 0, 
    AD_SLASH, 0, 
    AD_SOAK, 0, 
    AD_SONI, 0, 
    AD_SUNL, 0, 
  }; 
  int16 i;

  for (i=0; i<14; i++)
    mr[i].res = it->Hardness(mr[i].dtype);

  qsort(mr, 14, sizeof(mr[0]), MRSort);

  String s = "It has a resistance of "; 
  int last = -2; 
  int count_1 = 0; 
  int count_2 = 0; 

  for (i=0;i<14;i++) {
    if (mr[i].res < 0) break; 
    if (mr[i].res != last) {
      if (count_2)
        s += "; ";
      count_2++; 
      s += Format("%d to ",mr[i].res);
      last = mr[i].res; 
      count_1 = 0; 
    } 
    if (count_1) 
      s += ", ";
    count_1++;
    s += Lookup(DTypeNames,mr[i].dtype);
  } 
  s += "."; 
  if (i < 14) {
    s += " It is immune to ";
    count_1 = count_2 = 0; 
    for (; i<14; i++) {
      if (count_1) 
        s += ", ";
      count_1++;
      s += Lookup(DTypeNames,mr[i].dtype);
    } 
  }
  s += "."; 
  return *tmpstr(s); 
} 

String & MaterialHardnessDesc(int8 mat)
{
  mat_res mr[14] = {
    AD_ACID, 0, 
    AD_BLUNT, 0, 
    AD_COLD, 0, 
    AD_DCAY, 0, 
    AD_DISN, 0, 
    AD_FIRE, 0, 
    AD_MAGC, 0, 
    AD_NECR, 0, 
    AD_NORM, 0, 
    AD_RUST, 0, 
    AD_SLASH, 0, 
    AD_SOAK, 0, 
    AD_SONI, 0, 
    AD_SUNL, 0, 
  }; 
  int16 i;

  for (i=0; i<14; i++)
    mr[i].res = MaterialHardness(mat,mr[i].dtype);

  qsort(mr, 14, sizeof(mr[0]), MRSort);

  String s = "It has a resistance of "; 
  int last = -2; 
  int count_1 = 0; 
  int count_2 = 0; 

  for (i=0;i<14;i++) {
    if (mr[i].res < 0) break; 
    if (mr[i].res != last) {
      if (count_2)
        s += "; ";
      count_2++; 
      s += Format("%d to ",mr[i].res);
      last = mr[i].res; 
      count_1 = 0; 
    } 
    if (count_1) 
      s += ", ";
    count_1++;
    s += Lookup(DTypeNames,mr[i].dtype);
  } 
  s += "."; 
  if (i < 14) {
    s += " It is immune to ";
    count_1 = count_2 = 0; 
    for (; i<14; i++) {
      if (count_1) 
        s += ", ";
      count_1++;
      s += Lookup(DTypeNames,mr[i].dtype);
    } 
  }
  s += ".";
  return *tmpstr(s); 
} 

String & Item::Describe(Player *p)
{
  String Desc;
  TItem *ti = TITEM(iID);

  if (isType(T_BOOK) && ti->FirstRes(AN_SPELLS)) {
    int16 Illegible = 0; 
    rID sID;
    bool second; 
    String sn;

    Desc += "__<14>Spellbook:<7> It holds the spell";
    second = false; sn.Empty();
    for(sID=ti->FirstRes(AN_SPELLS);sID;sID=ti->NextRes(AN_SPELLS)) {
      /*if (TSPELL(theGame->SpellID(i))->Level > max casting level ||
        !(TSPELL(theGame->SpellID(i))->Sources & my sources))
        Illegible++
        else */
      if (!sn.GetLength())
        sn = NAME(sID);
      else {
        if (!second)
          Desc += SC("s <1>") + sn;
        else
          Desc += SC("<7>, <1>") + sn;
        second = true;
        sn = NAME(sID);
      }
    }
    if (second)
      Desc += SC("<7> and <1>") + sn + SC("<7>.\n");
    else
      Desc += SC(" <1>") + sn + SC("<7>.\n");
    /*
       if (Illegible)
       Desc += Format("It also holds %d spells that you "
       "cannot yet understand.", Illegible);
     */
  } else if (isType(T_LIGHT)) {
    Desc += Format("__<14>Light Source:<7> It provides <9>%d<7> feet of illumination.\n",GetLightRange() * 10);
  } 

  if (isKnown(KN_BLESS) && isBlessed()) 
    Desc += XPrint("__<14>Blessed:<7> A blessed item cannot be picked "
        "up by undead, demons, devils, or lycanthropes. "
        "A blessed shield or suit of armour cumulatively "
        "increases your Defense by +2 against the touch attacks of "
        "such creatures. A blessed weapon strikes true against such creatures, "
        "even if they have weapon immunity. A blessed weapon also strikes "
        "true against evil incorporeal creatures.\n"); 
  if (isKnown(KN_BLESS) && isCursed())
    Desc += XPrint("__<14>Cursed:<7> A cursed item cannot be removed once "
        "it has been equipped. Any magical enhancement on such an item is "
        "treated as a negative penalty rather than a bonus.\n");
  if (HasStati(MASTERWORK))
    Desc += XPrint("__<11>Masterwork:<7> This item has been tempered by a "
        "master craftsman, and receives a +<Num> bonus to damage. It will "
        "remain perfectly honed for <Num> more strokes.\n", 
        GetStatiVal(MASTERWORK),
        GetStatiMag(MASTERWORK));

  if (isGhostTouch()) {
    Desc += XPrint("__<14>Ghost Touch:<7> A ghost touch item is completely solid to incorporeal creatures and they may manipulate it normally. A ghost touch weapon affects incorporeal creatures fully (but must still deal with weapon immunity). An incorporeal creature may use a ghost touch weapon to strike corporeal creatures. A ghost touch shield provides its full defensive value against all touch attacks.\n");
  } 

  if (eID && isKnown(KN_MAGIC))
    if (TEFF(eID)->Desc) {
      Desc += Format("__%c%s:%c %s\n",
          -PINK,NAME(eID),-GREY,DESC(eID));
      if (TEFF(eID)->HasFlag(EF_CURSED)) {
        Desc += XPrint("__<14>Recurring Curse:<7> This maliciously-created item will re-curse itself whenever you equip it.\n");
      } 
    }
    
  StatiIterNature(this,BOOST_PLUS)
    if (!S->eID)
      continue;
    Desc += Format("__%c%s:%c Due to an outside enchantment, this item counts as a %c%+d%c magical item.\n",
        -PINK, NAME(S->eID), -GREY,
        -YELLOW,
        S->Mag,
        -GREY);
  StatiIterEnd(this)


  if (TITEM(iID)->Desc)
    Desc += Format("__%cDescription:%c %s\n",
        -BROWN, -GREY,
        (const char*)DESC(iID));
  if (isKnown(KN_MAGIC) && isKnown(KN_PLUS))
    Desc += Format("__%cCost:%c It has a base value of %d gp.\n",
          -WHITE, -GREY, getShopCost(NULL,NULL) / GetQuantity());
  if (HasStati(ALCHEMY_LEV))
    Desc += Format("__%cSuperior Alchemy:%c This item was crafted with "
                     "a superior Alchemy skill rating of %c%+d%c.\n",
                     -SKYBLUE, -GREY, -SKYBLUE,GetStatiMag(ALCHEMY_LEV),-GREY);

  if (HasStati(POISONED)) {
    bool unknown;
    unknown = false;
    if (isType(T_WEAPON) || isType(T_MISSILE))
      Desc += XPrint(Format("__<14>Poisoned:<7> This weapon has been coated with poison.\n")); 
    StatiIterNature(this,POISONED)
      if (!isKnown(KN_NATURE))
        if (!p->HasSkill(SK_POISON_USE) || !p->SkillLevel(
              SK_POISON_USE)+5 >= TEFF(S->eID)->ef.sval)
          { unknown = true; continue; }
      Desc += XPrint(Format("__<%d>%s <15>(<14>%d<15> doses)%s%s\n",
            isType(T_TOOL) ? PINK : WHITE, (const char*)Capitalize(NAME(S->eID),true),
            S->Mag, isType(T_TOOL) ? ": <7>" : " <7>-- ",
            (const char*)DESC(S->eID)));
    StatiIterEnd(this)
    if (unknown) {
      if (isType(T_WEAPON) || isType(T_MISSILE))
        Desc += "__There is poison on this weapon that you cannot identify.\n";
      else
        Desc += "__There is poison in this item that you cannot identify.\n"; 
      }
  }

  if (TITEM(iID)->HasFlag(IT_NOGEN) && !eID)
    Desc += Format("__%cNever Randomly Created:%c This item is never found randomly in dungeons, and is only created in specific instances.\n",
        -RED, -GREY,
        (const char*)DESC(iID));

  Desc += Format("__%cComposition:%c It weighs %d.%ds and is made of %c%s%c. ",
      -BROWN, -GREY, 
      (Weight() / 10) / Quantity, (Weight() % 10) / Quantity, 
      -BROWN, Lookup(MaterialDescs, Material()), -GREY);
  if (!isCorporeal()) 
    Desc += Format("It is %cincorporeal%c (and thus cannot be damaged). ", -BROWN, -GREY);
  if (isMetallic()) 
    Desc += Format("It is %cmetallic%c. ", -BROWN, -GREY);
  if (isOrganic()) 
    Desc += Format("It is %corganic%c. ", -BROWN, -GREY);
  if (isWooden()) 
    Desc += Format("It is %cwooden%c. ", -BROWN, -GREY);
  int h_s = Hardness(AD_SLASH),
  h_b = Hardness(AD_BLUNT),
  h_p = Hardness(AD_PIERCE),
  h_f = Hardness(AD_FIRE),
  h_c = Hardness(AD_COLD),
  h_ru= Hardness(AD_RUST),
  h_so = Hardness(AD_SOAK);
  Desc += Format("It has %c%d out of %d%c hit points. %s\n",
      -PURPLE, (Known & KN_PLUS) ? cHP : (((cHP*100)/MaxHP()) * TITEM(iID)->hp)/100, 
      (Known & KN_PLUS) ? MaxHP() : TITEM(iID)->hp, -GREY,
      (const char*)ItemHardnessDesc(this));

  if (p && p->WizardMode) {
    Desc += Format("__%cWizard Mode Info:%c It has an item level of <11>%d<7>. It has a Parent of %ld (<11>%s<7>).\n", -YELLOW,-GREY,ItemLevel(),
        Parent,
        Parent ? (const char*)(oThing(Parent))->Name() : "null"
        );
  } 
  return *tmpstr(XPrint(Desc));
}

String & QItem::Describe(Player *p)
{
  String Desc, tmp; int16 i;

  bool isArmour = false; 
  bool isWeapon = false; 
  switch (TITEM(iID)->IType) {
    case T_SHIELD: case T_ARMOUR: isArmour = true; break;
    case T_WEAPON: case T_BOW: isWeapon = true; break; 
  } 

  for (i=0;i!=8 && Qualities[i];i++) {
    if ((!KnownQuality(Qualities[i])) && Qualities[i] < IQ_DWARVEN)
      continue;

    if (LookupOnly(GenericPreQualNames,Qualities[i]))
        tmp = Format("__%c%s:%c ", -YELLOW, Lookup(GenericPreQualNames, Qualities[i]), -GREY);
    else if (LookupOnly(isArmour ? APreQualNames : PreQualNames,Qualities[i]))
        tmp = Format("__%c%s:%c ", -YELLOW, Lookup(isArmour ? APreQualNames : PreQualNames, Qualities[i]), -GREY);
    else
        tmp = Format("__%c%s:%c ", -YELLOW, Lookup(isArmour ? APostQualNames : PostQualNames, Qualities[i]), -GREY);
    tmp.SetAt(3,toupper(tmp[3]));
    if (LookupOnly(GenericQualityDescs,Qualities[i]))
      Desc += tmp + Lookup(GenericQualityDescs,Qualities[i]); 
    else 
      Desc += tmp + Format(Lookup(isArmour ? AQualityDescs : QualityDescs,Qualities[i]),ItemLevel()); 
    Desc += "\n";
  }

  StatiIterNature(this,EXTRA_QUALITY)
    {
      if (LookupOnly(GenericPreQualNames,S->Val))
          tmp = Format("__%c%s:%c ", -YELLOW, Lookup(GenericPreQualNames, S->Val), -GREY);
      else if (LookupOnly(isArmour ? APreQualNames : PreQualNames,S->Val))
          tmp = Format("__%c%s:%c ", -YELLOW, Lookup(isArmour ? APreQualNames : PreQualNames, S->Val), -GREY);
      else
          tmp = Format("__%c%s:%c ", -YELLOW, Lookup(isArmour ? APostQualNames : PostQualNames, S->Val), -GREY);
      tmp.SetAt(3,toupper(tmp[3]));
      if (LookupOnly(GenericQualityDescs,Qualities[S->Val]))
        Desc += tmp + Lookup(GenericQualityDescs,S->Val); 
      else 
        Desc += tmp + Lookup(isArmour ? AQualityDescs : QualityDescs,S->Val); 
      Desc += "\n";
    }
  StatiIterEnd(this)

  Desc = Item::Describe(p) + Desc; 

  return *tmpstr(XPrint(Desc));
}

// ww: let's be able to tell the difference between backpacks ...
String & Container::Describe(Player *p)
{
  String Desc; 
  TItem * ti = TITEM(iID);

  Desc += Format("__It is a container. It can hold <11>%d<7> items totalling at most <11>%d.%d<7>s, each of which must be <11>%s<7> or smaller. Moving an item in or out takes <11>%d<7> time units.",
      ti->u.c.Capacity,
      ti->u.c.WeightLim / 10,
      ti->u.c.WeightLim % 10,
      Lookup(SizeNames,ti->u.c.MaxSize),
      ti->u.c.Timeout
      );

  if (ti->u.c.WeightMod) {
    Desc += Format(" Items inside it weigh <11>%d~<7> of what they normally would.", 100 - ti->u.c.WeightMod);
  } 
  if (ti->u.c.CType) {
    Desc += Format(" It can only be used to store <11>%s<7>.",
        Lookup(ITypeNames,ti->u.c.CType));
  } 

  if (HasStati(LOCKED))
    Desc += Format("__<14>Locked:<7> Picking its lock requires a Skill Check against a DC of <11>%d<7>.",
        19 + p->m->Depth);

  if (HasStati(TRIED, SK_LOCKPICKING, p))
    Desc += Format("__<14>Tried:<7> You have already tried to pick its lock today.\n");

  if (HasStati(WIZLOCK))
    Desc += Format("__<14>Wizard-Locked:<7> It has double the hardness listed below and a +10 to the Lockpicking DC listed above.\n");

  Desc += SC("\n") + Item::Describe(p);

  return *tmpstr(XPrint(Desc));
}

String & Food::Describe(Player *p)
{
  String Desc; 
  TItem * ti = TITEM(iID);
  
  /* Statues descend from corpses, being essentially stone
     corpses, but the game shouldn't describe them as food! */
  if (isType(T_STATUE))
    return Item::Describe(p);
    
  if (isType(T_CORPSE))
    Desc += "__It is a corpse.";
  else
    Desc += "__It is food.";

  if (Material() == MAT_FLESH)
    Desc += " It contains <4>meat<7>."; 

  if (Type == T_CORPSE) {
    Corpse * c = (Corpse *)this; 
    Desc += Format(" It provides <11>%d<7> units of nutrition when consumed.",
      CorpseNut[((Corpse *)this)->Size()]);
    int16 DC = c->noDiseaseDC();
    if (DC > 30) 
      Desc += Format(" It is virtually <12>impossible<7> to eat this corpse without acquiring a disease.\n");
    else
      Desc += Format(" Eating this corpse without acquiring a disease requires a "
        "Wilderness Lore skill check (<11>DC %d<7>).\n",DC);
  } else { 
    Desc += Format(" It provides <11>%d<7> units of nutrition when consumed.\n",
        ti->Nutrition - Eaten);
  }

  Desc += Item::Describe(p);

  return *tmpstr(XPrint(Desc));
}

String & Armour::Describe(Player *p)
{
  String Desc; 
  TItem * ti = TITEM(iID);

  Desc += "__It is";

  int penalty = PenaltyVal(p,true); 

  int kplus = isKnown(KN_PLUS) ? GetPlus() : 0; 

  switch (TITEM(iID)->IType) {

    case T_SHIELD: {
      Desc += Format(" a %s shield. It provides a defense value of <11>%+d<7>; if a character has the Shield "
                     "Expert feat, it also grants a coverage value of <11>%+d<7>.",
          Lookup(SizeNames,Size(p)),
          DefVal(p,true),
          CovVal(NULL,true));

      if (ArmVal(0,isKnown(KN_PLUS)))
        Desc += Format(" This powerful shield also absorbs damage, providing an armour bonus of <11>%+d<7>.",ArmVal(0,isKnown(KN_PLUS)));

      if (Size(p) > p->Attr[A_SIZ]) {
        Desc += Format(" This shield is so large for you that it provides portable cover: attacks against you have a 50~ chance of missing.");
        penalty *= 2; 
      } 

      if (penalty) {
        Desc += Format(" While using it, your base movement rate is reduced by <11>%d~<7>, certain skills suffer a <11>%+d<7> penalty and your arcane spell failure rate increases by <11>%d~<7>.", 
            (penalty / 2) * 5, 
            penalty, 
            (penalty * -10));
      } 

      Desc += "\n"; 

      break; 
    }  

    case T_ARMOUR: {
      if (TITEM(iID)->Group & WG_LARMOUR) Desc += " light";
      else if (TITEM(iID)->Group & WG_MARMOUR) Desc += " medium";
      else if (TITEM(iID)->Group & WG_HARMOUR) Desc += " heavy";

      Desc += Format(" armour. It provides a coverage value of <11>%+d<7>.",
          CovVal(p,true));

      Desc += Format(" Your maximum dexterity bonus while wearing this armour is <11>%d<7>.", ((Armour *)this)->MaxDexBonus(p));

      if (penalty) {
        Desc += Format(" While wearing it, your base movement rate is reduced by <11>%d~<7>, certain skills suffer a <11>%+d<7> penalty and your arcane spell failure rate increases by <11>%d~<7>.",
            penalty * -5, 
            penalty,
            (penalty * -10));
      } 

      if (ArmVal(0,true) || ArmVal(1,true) || ArmVal(2, true))
        Desc += Format(" It provides an armour rating of <11>%d<7> against slashing "
                       "damage, <11>%d<7> against piercing damage and <11>%d<7> against "
                       "blunt damage.", ArmVal(0,true), ArmVal(1, true), ArmVal(2, true));


      Desc += "\n"; 
      
      #if 0
      /* I'm not sure this is true; it seems misleadingly simple to the
         player in comparison to how armour actually works, being scaled
         with damage. I'll add a section to the online help defining
         exactly what an armour rating means and does. */
      static const char * dmg_types[3] = { "Slashing", "Piercing", "Blunt" } ; 
      for (int i=0; i<=2; i++) {
        int val = ArmVal(i,true); 
        if (val <= 0) continue; 
        Desc += Format("__<13>%s %+d<7>: On average, <11>%d%%<7> of attacks will be absorbed completely. Other blows will have <11>%d%%<7> of their damage pentrate on average.\n",
            dmg_types[i],
            val,
            AbsorbTable[min(val,16)][2],
            ArmourTable[min(val,32)][2]);
        }
      #endif

      break; 
      }

    default: Desc += " wearable.\n" ; penalty = 0; break; 
  }

  if (((Creature *)p)->WepSkill((Item *)this) == WS_NOT_PROF && penalty) {
    Desc += Format("\n__Due to your <4>lack of proficiency<7> with this armour, your base weapon speed is reduced by <11>%d~<7> and you suffer a skill penalty of <11>%d<7> to your hit rolls and Reflex saving throws.\n", 
        (-5 * penalty),
        penalty);
  }

  Desc += QItem::Describe(p); 

  return *tmpstr(XPrint(Desc));
}


String & Weapon::Describe(Player *p)
  {
    String Desc, tmp;
    int16 th, Crit;
                                              
    Desc += Format("__It is a %s weapon that inflicts <11>%s<7>",
        Lookup(SizeNames,Size(p)),
        (const char*)TITEM(iID)->u.w.LDmg.Str());

    if (TITEM(iID)->HasFlag(WT_SLASHING) + TITEM(iID)->HasFlag(WT_PIERCING) + 
          TITEM(iID)->HasFlag(WT_BLUNT) <= 1)
      {
        if (TITEM(iID)->HasFlag(WT_SLASHING))
          Desc += " slashing";
        if (TITEM(iID)->HasFlag(WT_PIERCING))
          Desc += " piercing";
        if (TITEM(iID)->HasFlag(WT_BLUNT))
          Desc += " blunt";
      }
    else if (TITEM(iID)->HasFlag(WT_SLASHING) && TITEM(iID)->HasFlag(WT_PIERCING)
          && TITEM(iID)->HasFlag(WT_BLUNT))
      Desc += " slashing, piercing and blunt";
    else
      {
        if (TITEM(iID)->HasFlag(WT_SLASHING))
          Desc += " slashing";
        else if (TITEM(iID)->HasFlag(WT_PIERCING))
          Desc += " piercing";
        if (TITEM(iID)->HasFlag(WT_BLUNT))
          Desc += " and blunt";
        else
          Desc += " and piercing";
      }

    Desc += " damage to " ;

    if (TITEM(iID)->u.w.LDmg == TITEM(iID)->u.w.SDmg)
      Desc += "all ";
    else
      Desc += SC("large creatures and <11>") + TITEM(iID)->u.w.SDmg.Str() + 
                  "<7> damage to medium or small creatures. It scores a threat on a roll of <11>";
    th = Threat(p);
    if (th == 20)
      Desc += "20";
    else if (th == 19)
      Desc += "19 or 20";
    else if (th == 18)
      Desc += "18, 19 or 20";
    else
      Desc += Format("%d or higher", th);
    
    Crit = TITEM(iID)->u.w.Crit;
    Desc += Format("<7>, and inflicts <11>x%d<7> damage on a critical hit.",CritMult(p));

    Desc += Format(" It has a base speed of <11>%d~<7>, an accuracy modifier of <11>%+d<7>, and a parry modifier of <11>%+d<7>.", 
        100 + (TITEM(iID)->u.w.Spd) * 5, TITEM(iID)->u.w.Acc,
        ParryVal(p, (Known & KN_PLUS) != 0));

    if (p->AbilityLevel(CA_SNEAK_ATTACK))
      Desc += Format(" On a sneak attack, you inflict <11>+%dd%d<7> damage with it.",
        p->AbilityLevel(CA_SNEAK_ATTACK),TITEM(iID)->HasFlag(WT_SUPER_SNEAK)
         ? 8 : 6);
    else if (TITEM(iID)->HasFlag(WT_SUPER_SNEAK))
      Desc += " This weapon uses d8's for Sneak Attack damage.";

    if (useInGrapple(p)) {
      Desc += " This weapon can be <11>used while grappling<7>."; 
    } else {
      Desc += " This weapon is <11>too big for grappling<7> and cannot be used while you are grabbed."; 
    } 

    /*
    Desc += Format(" Weapons of this class have an effectiveness rank of <11>%d~<7>.", TITEM(iID)->Cost / 100);
    */
    if (TITEM(iID)->HasFlag(WT_NO_MELEE))
      Desc += " This item <11>cannot be used<7> as a melee weapon.";

    if (TITEM(iID)->HasFlag(WT_REACH)) {
      Desc += Format(" It is a <11>reach<7> weapon, with a range of 10 feet%s. <11>Closing<7> "
        "with you when you wield this weapon and are not charging requires a Reflex saving throw "
        "against <11>DC %d<7>.",
        (!TITEM(iID)->HasFlag(WT_STRIKE_NEAR)) ? ", but it cannot be used to attack"
          " opponents who have closed with the wielder" : "",p->WeaponSaveDC(this,WT_REACH));
      }
    else if (p->StateFlags & MS_HAS_REACH) {
        /* The player has reach for some reason, but it's not _this_ weapon.  If it is inherent for
           who they are as a creature, then annotate this weapon.  Otherwise, it shouldn't apply. */
        if (((Creature*)p)->InherentCreatureReach())
          Desc += Format(" <11>Closing<7> "
            "with you when you wield this weapon requires a Reflex saving throw "
            "against <11>DC %d<7>.",p->WeaponSaveDC(this,WT_REACH));
          }
    if (TITEM(iID)->HasFlag(WT_STUNNING)) 
      Desc += Format(" On a critical hit or great blow, or on 1 in 3 normal attacks, "
                     "this weapon will <11>stun<7> a foe who fails a Fortitude save (<11>DC %d<7>) "
                     "for 1d4+1 rounds.", p->WeaponSaveDC(this,WT_STUNNING));
    if (TITEM(iID)->HasFlag(WT_ENTANGLE) && TITEM(iID)->Group & WG_THROWN)
      Desc += Format(" This weapon will <11>entangle<7> a character of its size category "
                     "or smaller who fails a Reflex save (<11>DC %d<7>) when struck.", p->WeaponSaveDC(this,WT_ENTANGLE));
    else if (TITEM(iID)->HasFlag(WT_ENTANGLE)) 
      Desc += Format(" On a critical hit, or on 1 in 3 normal attacks, "
                     "this weapon will <11>entangle<7> a foe of its size category "
                     "or smaller who fails a Reflex save (<11>DC %d<7>).", p->WeaponSaveDC(this,WT_ENTANGLE));
    if (TITEM(iID)->HasFlag(WT_KNOCKDOWN) && TITEM(iID)->Group & WG_THROWN)
      Desc += Format(" This weapon will <11>knock prone<7> a character of its size category "
                     "or smaller who fails a Fortitude save (<11>DC %d<7>) when struck.", p->WeaponSaveDC(this,WT_KNOCKDOWN));
    else if (TITEM(iID)->HasFlag(WT_KNOCKDOWN))
      Desc += Format(" On a critical hit or great blow, or on 1 in 3 normal attacks, "
                     "this weapon will <11>knock prone<7> a foe who fails a Fortitude save "
                     "(<11>DC %d<7>) when struck.", p->WeaponSaveDC(this,WT_KNOCKDOWN));
    else if (p->HasFeat(FT_KNOCK_PRONE) && useStrength())
      Desc += Format(" On a critical hit or great blow, or on 1 in 3 normal attacks, "
                     "your <11>knock prone<7> feat will affect a foe who fails a Fortitude save "
                     "(<11>DC %d<7>) when struck.", p->WeaponSaveDC(this,WT_KNOCKDOWN));

    if (TITEM(iID)->HasFlag(WT_CHARGE))
      Desc += " This weapon deals double damage when used in a <11>charge<7>.";
    if (TITEM(iID)->HasFlag(WT_MOUNTED_ONLY))
      Desc += " This weapon can be used <11>only while mounted<7> unless you are one size larger than it is."; 
    if (TITEM(iID)->HasFlag(WT_SUBDUAL))
      Desc += " This weapon inflicts <11>subdual<7> damage instead of normal damage.";
    if (TITEM(iID)->HasFlag(WT_PENETRATING))
      Desc += " This weapon grants a x2 bonus to <11>penetrating<7> armour.";
    if (TITEM(iID)->HasFlag(WT_SUPER_TRIP))
      Desc += " This weapon grants a +4 bonus to <11>trip<7> attempts.";
    if (TITEM(iID)->HasFlag(WT_SUPER_DISARM))
      Desc += " This weapon grants a +4 bonus to <11>disarm<7> attempts.";
    if (TITEM(iID)->HasFlag(WT_DOUBLE))
      Desc += " This is a <11>double weapon<7>, and must be used as if wielding two "
              "different weapons at the same time.";
    if (TITEM(iID)->HasFlag(WT_CROSSBOW))
      Desc += " This weapon must be manually <11>reloaded<7> after being fired.";
    if (TITEM(iID)->HasFlag(WT_EXOTIC_1H))
      Desc += " This weapon can only be wielded in one hand if the Exotic "
              "Weapon Proficiency feat is bought for it.";
    if (TITEM(iID)->HasFlag(WT_TWO_HANDED))
      Desc += " This weapon is treated as if it were one size larger when determining if it must be wielded with <11>two hands<7>.";
    if (TITEM(iID)->HasFlag(WT_RETURNING))
      Desc += " This weapon <11>returns<7> to the user when thrown.";
    if (!useStrength())
      Desc += " This weapon gains <11>no Strength bonuses<7>.";
    if (TITEM(iID)->Group & WG_SIMPLE)
      Desc += " It is a <11>simple<7> weapon.";
    if (TITEM(iID)->Group & WG_EXOTIC)
      Desc += " It is an <11>exotic<7> weapon.";
    if (TITEM(iID)->Group & WG_SBLADES)
      Desc += " It is a <11>short blade<7>.";
    if (TITEM(iID)->Group & WG_LBLADES)
      Desc += " It is a <11>long blade<7>.";
    if (TITEM(iID)->Group & WG_AXES)
      Desc += " It is an <11>axe<7>.";
    if (TITEM(iID)->Group & WG_ARCHERY)
      Desc += " It is an <11>archery<7> weapon.";
    if (TITEM(iID)->Group & WG_STAVES)
      Desc += " It is a <11>staff<7>.";
    if (TITEM(iID)->Group & WG_IMPACT)
      Desc += " It is an <11>impact<7> weapon.";
    if (TITEM(iID)->Group & WG_THROWN)
      Desc += " It is a <11>thrown<7> weapon.";
    if (TITEM(iID)->Group & WG_POLEARMS)
      Desc += " It is a <11>polearm<7>.";
    if (TITEM(iID)->Group & WG_SPEARS)
      Desc += " It is a <11>spear<7>.";
    if (TITEM(iID)->Group & WG_LANCES)
      Desc += " It is a <11>lance<7>.";
    if (TITEM(iID)->Group & WG_DAGGERS)
      Desc += " It is a <11>dagger<7>.";
    if (TITEM(iID)->Group & WG_MARTIAL)
      Desc += " It is a <11>martial arts<7> (monk) weapon.";
    if (TITEM(iID)->Group & WG_FLEXIBLE)
      Desc += " It is a <11>flexible<7> weapon.";
    if (TITEM(iID)->Group & WG_FIREARMS)
      Desc += " It is a <11>firearm<7>.";
    if (TITEM(iID)->Group & WG_FLAILS)
      Desc += " It is a <11>flail<7>.";
    if (TITEM(iID)->Group & WG_LIGHT)
      Desc += " It is a <11>light<7> weapon.";

    int16 mstr = 5;
    if (TITEM(iID)->HasFlag(WT_STR1))
      mstr += 8;
    if (TITEM(iID)->HasFlag(WT_STR2))
      mstr += 4;
    if (TITEM(iID)->HasFlag(WT_STR3))
      mstr += 2;
    if (mstr > 5)
      Desc += Format(" It requires a minimum Strength of %c%d%c to use proficiently.",
        -PINK, mstr, -GREY);


    Desc += SC("\n") + QItem::Describe(p);
    
    if (p) {
        if (myHandle == p->defMelee)
            Desc += Format("__%cThis is your default melee weapon.%c\n", -WHITE, -GREY);
        if (myHandle == p->defOffhand)
            Desc += Format("__%cThis is your default offhand weapon.%c\n", -WHITE, -GREY);
        if (myHandle == p->defRanged)
            Desc += Format("__%cThis is your default ranged weapon.%c\n", -WHITE, -GREY);
        if (myHandle == p->defAmmo)
            Desc += Format("__%cThis is your default projectile.%c\n", -WHITE, -GREY);
      }    

  return *tmpstr(XPrint(Desc));
}

String & Character::Describe(Player *p) {
    return *tmpstr("You see yourself.");
}

String & Map::Describe(Player* p) {
    if (DESC(dID))
      return *tmpstr(DESC(dID));
    else
      return *tmpstr("Dark, cramped and fetid."); 
}

String & Feature::Describe(Player *p) {
  TFeature *tf = TFEAT(fID); 
  String Desc;
  Desc = "";
  if (tf->Desc) 
    Desc += Format("__%s\n",DESC(fID));

  if (Flags & (F_SOLID)) Desc += Format("__<14>Solid:<7> It is solid.\n");
  if (Flags & (F_OPAQUE)) Desc += Format("__<14>Opaque:<7> It is opaque.\n");
  if (Flags & (F_OBSCURE)) 
    Desc += Format("__<14>Opaque:<7> It obscures normal vision (unless you have the Nature Sense ability).\n");

  if (HasStati(SUMMONED))
    Desc += Format("__<14>Summoned:<7> It was magically created and will disappear in %d turns.\n",GetStatiDur(SUMMONED));

  if (MoveMod) {
    Desc += Format("__<14>Movement Modifier:<7> When moving through this terrain you movement rate is adjusted by "
      "%+d~ (unless you are aerial or have the Woodland Stride ability).\n",MoveMod*5);
  } 

  int8 mat = tf->Material; 
  extern String & MaterialHardnessDesc(int8 mat);

  Desc += Format("__<14>Composition:<7> It has %c%d out of %d%c hit points and is made of %s. %s\n",
      -PURPLE, cHP, mHP, -GREY,
      (const char*)Lookup(MaterialDescs,mat),
      (const char*)MaterialHardnessDesc(mat)
      );


  return *tmpstr(Desc);
}

String & Door::Describe(Player *p) {
  TFeature *tf = TFEAT(fID); 
  String Desc;

  Desc = Format("__<14>General:<7> It is %s, %s and %s. Picking its lock requires a Skill Check against a DC of %d.\n",
     ( (DoorFlags & DF_LOCKED) ? "locked" : "unlocked" ),
     ( (DoorFlags & DF_OPEN) ? "open" : "closed" ),
     ( (DoorFlags & DF_BROKEN) ? "broken" : "whole" ),
     14 + m->Depth
     );

  if (HasStati(TRIED, SK_LOCKPICKING, p))
    Desc += Format("__<14>Tried:<7> You have already tried to pick its lock today.\n");

  if (HasStati(WIZLOCK))
    Desc += Format("__<14>Wizard-Locked:<7> It has double the hardness listed below and a +10 to the Lockpicking DC listed above.\n");

  Desc += Feature::Describe(p); 

  return *tmpstr(Desc); 
}

extern String ps_circ;

String & TEffect::Describe(Player *p) {
    String s, s2, descs, levs; bool dual = false, first = false;
    int8 Sources[] = { AI_WIZARDRY, AI_THEURGY, AI_DRUIDIC, 0 };
    int i,j, mIdx; rID spID, dID, spList[MAX_SPELLS+1], clID;
    uint16 osf;

    spID = theGame->SpellID(this - theGame->Modules[0]->QEff);
    //ASSERT(this == TEFF(spID));

    if (TEFF(spID)->HasSource(AI_TATTOO)) {
        s = Format("<5>%s<7> (<2>%s Tattoo<7>)\n__%s",
            (const char*) Upper(NAME(spID)), 
            (TEFF(spID)->HasFlag(EF_ACTIVATE) ||
            TEFF(spID)->HasFlag(EF_ACTIVATE2) ||
            TEFF(spID)->HasFlag(EF_ACTIVATE3) ||
            TEFF(spID)->HasFlag(EF_ACTIVATE4) ||
            TEFF(spID)->HasFlag(EF_ACTIVATE5)) ?
            "Active" : "Passive",
            DESC(spID));
        return *tmpstr(s);
    }

    if (TEFF(spID)->HasSource(AI_ALCHEMY)) {
        s = Format("<5>%s<7>\n__%s",
            (const char*) Upper(NAME(spID)), 
            DESC(spID));
        return *tmpstr(s);
    }

    if (TEFF(spID)->ef.aval == AR_POISON || TEFF(spID)->ef.aval == AR_DISEASE) {
        s = Format("__<9>%s<7>: %s",
            (const char*) Capitalize(NAME(spID),true), 
            DESC(spID));
        return *tmpstr(s);
    }

    s += Format("<14>%s<7> (",NAME(spID)); 
    if (HasSource(AI_PSIONIC))
        s += "<4>Psionic Power<7>)";
    else if (Schools && Level) {
        for (i=0; SchoolNames[i].Val; i++) 
            if ((Schools & SchoolNames[i].Val) == SchoolNames[i].Val) {
                if (SchoolNames[i].Val > SC_NAT)
                    descs += Format(" %c[%s]",-GREY, (const char*)Upto(SchoolNames[i].Text," "));
                else            
                    s += Format("%s<4>%s<7>",dual ? "/" : "", SchoolNames[i].Text);
                dual = true;
            }

        for (i=0;i!=theGame->Modules[0]->szCla;i++) {
            int16 lv = 0;
            clID = theGame->Modules[0]->ClassID(i);
            TCLASS(clID)->GetList(SPELL_LIST, spList, MAX_SPELLS);
            for(j=0;spList[j] || spList[j+1] || spList[j+2];j++) {
                if (spList[j] > 0 && spList[j] < 20)
                    lv = (int16)spList[j];
                if (spList[j] == spID)
                    levs += Format(", <1>%s <9>%d<7>",
                    NAME(clID), lv);
            }
        }

        for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
            Module * m = theGame->Modules[mIdx];
            for (uint16 i=0; i<m->szDom; i++) {
                dID = m->DomainID(i);
                for (int16 n=0;n!=9;n++)
                    if (spID == TDOM(dID)->Spells[n])
                        levs = Format(", <1>%s <9>%d<7>", NAME(dID), n+1) + levs;
            }
        }

        s += ")";
    } else
        s += "<4>Special Ability<7>)";

    if (Vals(0)->eval == EA_NOTIMP)
        s += "\n<13> * * * NOT IMPLEMENTED YET * * *<7>"; 

    for (i=0;Sources[i];i++)
        if (HasSource(Sources[i])) {
            switch(Sources[i]) {
            case AI_WIZARDRY: s2 = "Mage"; break;
            case AI_THEURGY: s2 = "Priest"; break;
            case AI_DRUIDIC: s2 = "Druid"; break;
            }
            if (levs.GetLength())
                if (strstr(levs,s2))
                    continue;
            levs = Format(", <1>%s <9>%d<7>", (const char*)s2, Level) + levs;
        }

    if (HasFlag(EF_EVIL))
        descs += " [Evil]";
    if (HasFlag(EF_GOOD))
        descs += " [Good]";
    if (HasFlag(EF_LAWFUL))
        descs += " [Lawful]";      
    if (HasFlag(EF_CHAOTIC))
        descs += " [Chaotic]";      

    if (HasFlag(EF_FEAR))
        descs += " [Fear]";      
    if (HasFlag(EF_DEATH))
        descs += " [Death]";      
    if (HasFlag(EF_SOUND))
        descs += " [Sound-Based]";      
    if (HasFlag(EF_MENTAL))
        descs += " [Mind-Affecting]";      
    if (HasFlag(EF_COMPULSION))
        descs += " [Compulsion]";      

    if (descs.GetLength())
        s += descs;
    s += "\n";
    if (levs.GetLength()) {
        levs = levs.Right(levs.GetLength() - 2);
        s += Format("%cLevel: %s\n",-GREY, (const char*)levs);
    }


    switch (ef.sval) {
    case FORT:
        s += HasFlag(EF_PARTIAL) ? "<5>Fortitude<7> partial. " : "<5>Fortitude<7> negates. ";
        break;
    case REF:
        s += HasFlag(EF_PARTIAL) ? "<5>Reflex<7> partial. " : "<5>Reflex<7> negates. ";
        break;
    case WILL:
        s += HasFlag(EF_PARTIAL) ? "<5>Will<7> partial. " : "<5>Will<7> negates. ";
        break;
    }
    if (HasFlag(EF_VARMANA))
        s += Format("Mana: <11>Variable<7>. ");
    else if (ManaCost && p)
        s += Format("Mana: <11>%d <7>(base %d). ",
        p->getSpellMana(spID,0,NULL), ManaCost);
    else if (ManaCost)
        s += Format("Mana: <11>%d<7>. ",ManaCost);
    else
        s += "Mana: <11>None<7>. ";

    if (HasFlag(EF_D1ROUND)) s += ("1 round duration. ");
    else if (HasFlag(EF_DSHORT)) s += ("Short duration. ");
    else if (HasFlag(EF_DLONG)) s += ("Long duration. ");
    else if (HasFlag(EF_DXLONG)) s += ("Extended duration. ");
    else if (HasFlag(EF_PERSISTANT)) s += ("Persistant. ");
    if (HasFlag(EF_NO_BONUS_DMG)) s += ("No bonus damage. ");
    if (HasFlag(EF_DEFENSIVE)) s += "No AoO. ";

    if (BaseChance)
        s += Format("Base Success: <11>%d~<7>.\n__<1>",BaseChance);
    else
        s += "\n__<1>";

    s += theGame->GetText((ModNum+1)*0x01000000,Desc); 

    if (p) {
        osf = p->getSpellFlags(spID);
        p->setSpellFlags(spID, p->getSpellFlags(spID) | SP_KNOWN);
        int16 sc = p->SpellRating(spID,p->MMFeats(theGame->SpellNum(spID)),true);
        p->setSpellFlags(spID,osf);
        if (ps_perfect)
        {
            s += Format("\n<11>Chance: 100%% (%s)", ps_perfect);
        }
        else
        {
            s += Format("\n<11>Chance: <7>%d%% base", p_base);
            if (p->HasStati(SPECIALTY_SCHOOL))
                s += Format(" %+d%% specialist", p_spec);
            if (p_int)
                s += Format(" %+d%% Int", p_int);
            if (p_lev)
                s += Format(" %+d%% level", p_lev);
            if (p_calc)
                s += Format(" %+d%% %s", p_calc, ps_calc);
            if (p_meta)
                s += Format(" %+d%% meta", p_meta);
            if (p_circ)
                s += Format(" %+d%% circ (%s)", p_circ,
                (const char*) ps_circ.Left(ps_circ.GetLength()-1));
            if (p_conc)
                s += Format(" %+d%% concent", p_conc);

            s += Format(" = <11>%d%%<7>", sc);
        }
        if (ef.sval != NOSAVE)
        {
            int16 saveDC;
            saveDC = p->getSpellDC(spID,false,(p->MMFeats(theGame->SpellNum(spID)) & MM_HEIGHTEN) != 0);
            s += Format("\n<11>Save DC: <7>%d base", dc_base);
            if (dc_lev)
                s += Format(" %+d level", dc_lev);
            if (dc_attr)
                s += Format(" %+d %s", dc_attr, dcs_attr);
            if (dc_beguile)
                s += Format(" %+d Cha", dc_beguile);
            if (dc_trick)
                s += Format(" %+d trick", dc_trick);
            if (dc_height)
                s += Format(" %+d heighten", dc_height);
            if (dc_hard)
                s += Format(" %+d spell", dc_hard);
            if (dc_focus)
                s += Format(" %+d focus", dc_focus);
            if (dc_will)
                s += Format(" %+d will", dc_will);
            if (dc_will)
                s += Format(" %+d affinity", dc_affinity);
            s += Format(" = DC <11>%d<7>",saveDC);
        }
    }
    //PurgeStrings();
    return *tmpstr(s); 
}

String & TDomain::Describe(bool DescribeDomainSpells)
{
  String s; 
  int i,j,q;

  s += Format("<14>%s<7>\n", (const char*)Upper(GetName(0)));

  if (Desc) 
    s += Format("%s\n", (const char*)theGame->GetText((ModNum+1)*0x01000000,Desc)); 

  s += Format("<10>Offered By:<7>");
  int offerCount = 0; 
  for (q=0;q!=MAX_MODULES;q++)
    if (theGame->Modules[q]) {
      rID xID = theGame->Modules[q]->GodID(0);
      rID endID = xID + theGame->Modules[q]->szGod;
      for (;xID != endID;xID++) {
        for (j=0;j<12;j++) 
          if (TGOD(xID)->Domains[j] &&
              TDOM(TGOD(xID)->Domains[j]) == this) {
            if (offerCount > 0) s += ",";
            s += Format(" %s",NAME(xID));
            offerCount++; 
          }
      }
    }
  if (!offerCount)
    s += " ?";
  else
    s += ".";

  s += Format("\n<10>%s Domain Spells:<7>\n", (const char*)GetName(0));
  for (i=0;i<5;i++) {
    if (Spells[i] && Spells[i]) {
      if (DescribeDomainSpells) {
        TEffect * te = TEFF(Spells[i]);
        s += Format("__<13>%d<7>: %s\n",i+1,(const char*)te->Describe(NULL));
      } else 
        s += Format("__<13>%d<7>: %s\n",i+1,(const char*)NAME(Spells[i]));
    } 
  } 

  return *tmpstr(s); 
}

const char * FeatName(int16 ft)
{
  int below = -1;
  //PurgeStrings(); 
  for (int i=0; FeatInfo[i].feat; i++) 
    if (FeatInfo[i].feat == ft)
      return FeatInfo[i].name; 
    else if ((FeatInfo[i].feat < ft) &&
        (below == -1 || FeatInfo[i].feat > FeatInfo[below].feat))
      below = i;
  static char buf[1024];
  sprintf(buf,"---%d (below %d = %s)---",ft,
      FeatInfo[below].feat,FeatInfo[below].name);
  return buf;
} 

bool FeatUnimplemented(int16 ft)
{
  for (int i=0; FeatInfo[i].feat; i++) 
    if (FeatInfo[i].feat == ft)
      return (FeatInfo[i].flags & FF_UNIMP); 
  return true;
} 

String & DescribeFeat(int16 ft)
{
  int16 i,j,k,d,c;
  String s; 
  PurgeStrings();
  s = "";
  for (i=0; FeatInfo[i].feat; i++) {
    if (FeatInfo[i].feat == ft) {
      s += Format("<14>%s<7>",FeatInfo[i].name);
      if (FeatInfo[i].flags & FF_UNIMP) 
        s += (" <12>[Unimplemented]<7>");
      if (FeatInfo[i].flags & FF_META) 
        s += (" <2>[Metamagic]<7>");
      if (FeatInfo[i].flags & FF_MULTIPLE) 
        s += (" <2>[Multiple]<7>");
      if (FeatInfo[i].flags & FF_MONSTER) 
        s += (" <2>[Bestial]<7>");
      if (FeatInfo[i].flags & FF_WARRIOR) 
        s += (" <2>[Warrior]<7>");
      
      /*
      FJM: no ranger bonus feats anymore...
      for (listed = j=0;RangerFeats[j] && !listed;j++)
        if (RangerFeats[j] == FeatInfo[i].feat)
          listed = true;
      if (listed || FeatInfo[i].flags & FF_RANGER) 
        s += (" <2>(ranger list)<7>");
      */
      
      s += "\n"; 
      
      if (ft == FT_INTENSIVE_STUDY && theGame->GetPlayer(0))
        {
          uint16 eli;
          s += Format("<11>Eligible Studies:<7> ");
          eli = theGame->GetPlayer(0)->getEligableStudies();
          if (!eli)
            s += "None.\n";
          else {
            for (k=0;StudyNames[k].Val;k++)
              if (eli & XBIT(StudyNames[k].Val))
                { s += StudyNames[k].Text; s += ", "; }
            s = SC(s.Left(s.GetLength() - 2)) + SC(".\n");
            }
        }
      
      if (FeatInfo[i].pre.Or[0].And[0].elt == FP_ALWAYS) {
        // nothing 
      } else { 
        s += Format("<13>Prerequisite:<7> ");
        for (d=0; d<FP_MAX_DISJUNCTS; d++) {
          if (FeatInfo[i].pre.Or[d].And[0].elt == FP_ALWAYS)
            break; 
          if (d > 0) 
            s += Format("<13>__________OR:<7> ");
          for (c=0; c<FP_MAX_CONJUNCTS; c++) {
            struct FeatConjunct * fc = &FeatInfo[i].pre.Or[d].And[c];
            if (fc->elt == FP_ALWAYS) break; 
            if (c > 0) 
              s += ", ";
            switch (fc->elt) {
              case FP_FEAT: s += Format("<6>%s<7>",FeatName((int16)fc->arg)); break; 
              case FP_ABILITY: 
                            s += Format("<3>%s", Lookup(ClassAbilities,fc->arg)); 
                            if (fc->val) 
                              s += Format(" >= %d",fc->val);
                            s += Format("<7>");
                            break; 
              case FP_BAB: {
                             static char* BabNames[] = {
                               "Archery", "Brawl", "Melee",  "Thrown", "Offhand" };
                             s += Format("<4>%s BAB >= %d<7>", 
                                 BabNames[fc->arg], fc->val);
                             break; 
                           }
              case FP_SKILL:
                           s += Format("<2>%s >= %+d<7>",SkillInfo[fc->arg].name,fc->val);
                           break; 
              case FP_CR:
                           s += Format("<4>Challenge Rating >= %d<7>",fc->arg); break;
              case FP_ATTR:
                           s += Format("<2>%s >= %d<7>",AttrTitle[fc->arg],fc->val);
                           break; 
              case FP_PROF:
                           s += Format("<1>Proficiency with %s<7>", Lookup(WeaponGroupNames,fc->arg));
                           break;
              case FP_NOT_PROF:
                           s += Format("<1>Not Proficient with %s<7>", Lookup(WeaponGroupNames,fc->arg));
                           break;
              case FP_CASTER_LEVEL:
                           s += Format("<4>Caster Level >= %d<7>",fc->arg); break;
                           break; 
              case FP_WEP_SKILL: 
                           {
                             static char* WSNames[] = { "???",
                               "Weapon Proficiency", 
                               "Weapon Focus", 
                               "Weapon Specialist",  
                               "Weapon Mastery", 
                               "Weapon High Mastery",
                               "Weapon Grand Mastery" };
                               s += Format("<6>%s<7>",WSNames[fc->arg]); break;
                               break; 
                           }
              default: s += "???"; 
            } 
          } 
          s += ".\n"; 
        } 
      }
      s += Format("<13>Benefit:<7> %s\n", FeatInfo[i].desc);
#if 0
      // what does this feat immediately allow? 
      String a;
      int allow_count = 0; 
      for (j=0; FeatInfo[j].feat; j++) {
        for (d=0; d<FP_MAX_DISJUNCTS; d++) {
          for (c=0; c<FP_MAX_CONJUNCTS; c++) {
            if (FeatInfo[j].pre.Or[d].And[c].elt == FP_FEAT &&
                FeatInfo[j].pre.Or[d].And[c].arg == ft) {
              if (allow_count)
                a += ", ";
              allow_count++; 
              a += Format("<6>%s<7>",FeatInfo[j].name);
            }
          } 
        }
      }
      s += Format("<13>Allows:<7> %s.\n",a);
#endif

      // ww: this massive hack computes the transitive closure of all
      // feats eventually allowed by this one, so that you can see the feat
      // trees as you go. For example, this will tell you that Dodge allows
      // Mantis Leap, even though you must go through Mobility first. 
#define TC_SET_SIZE     80
      int16 allowed[TC_SET_SIZE] = { ft, 0};
      int16 is_imp[TC_SET_SIZE] = { 0 };
      int16 a_max = 1, poss;
      bool done = false; 
      while (!done) {
        done = true; 
        for (j=0; FeatInfo[j].feat; j++) 
          for (d=0; d<FP_MAX_DISJUNCTS; d++) 
            for (c=0; c<FP_MAX_CONJUNCTS; c++) 
              if (FeatInfo[j].pre.Or[d].And[c].elt == FP_FEAT)
                for (poss = a_max - 1; poss >= 0; poss--) 
                  if (FeatInfo[j].pre.Or[d].And[c].arg == allowed[poss]) {
                    bool already_in = false;
                    for (int16 poss2 = 0; poss2 < a_max; poss2++)
                      if (allowed[poss2] == FeatInfo[j].feat)
                        already_in = true;
                    if (!already_in) { 
                      done = false;
                      allowed[a_max] = FeatInfo[j].feat;
                      is_imp[a_max] = !(FeatInfo[j].flags & FF_UNIMP);
                      a_max++;
                    }
                  } 
      }
      if (a_max > 1) {
        s += Format("<13>Allows:<7> ");
        for (j=1; j<a_max; j++) {
          if (j > 1) s += ", ";
          if (!is_imp[j])
            s += Format("<6>%s<7> (unimp)",FeatName(allowed[j]));
          else
            s += Format("<6>%s<7>",FeatName(allowed[j]));
        }
        s += ".\n"; 
      } 
    } 
  } 
  return *tmpstr(s); 
}







