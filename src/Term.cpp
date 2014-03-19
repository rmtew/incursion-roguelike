/* TERM.CPP -- Copyright (c) 1999-2003 Julian Mensch
     This file contains all the member functions for the TextTerm
   class, which describes in a platform-independant manner any
   terminal which can display IBM extended ASCII characters in a
   80x50 text mode with the standard IBM 16 foreground and 16
   background colors. It would be used by text-mode Incursion ports
   to Win32 Console mode, Linux, et al., but not by a more GUI-enabled 
   version of the game, such as a true Windows GUI port, or a Mac
   version. Currently, many functions that should really be here
   are still in Win32.Cpp.

   Main Gameplay Screen
     void TextTerm::ShowStatus()
     void TextTerm::ShowTraits()
     void TextTerm::ShowTime()
     void TextTerm::ShowDamage(const char*str, Item *w, int8 Mode)
   Rendering the Map
     inline int16 Priority(int16 t)
     void Map::Update(int16 x,int16 y)
     void TextTerm::ShowMap()
     void TextTerm::AdjustMap(int16 vx, int16 vy,bool newmap)
     void Term::ShowThings()
     Glyph TextTerm::FloorShading(int16 x, int16 y, Glyph g)
     void TextTerm::PutGlyph(int16 x, int16 y,Glyph g) 
     Glyph TextTerm::GetGlyph(int16 x, int16 y)
     void TextTerm::ShowMapOverview()
   View List Code
     virtual void ShowViewList()=0;
     virtual void ResetWViewList()=0;
     virtual void WViewThing(Thing *t, int32 dist, bool assertSeen)=0;
   EffectPrompt
     bool TextTerm::EffectPrompt(EventInfo &e, uint16 fl,bool is_look, String PromptText)
     Thing* TextTerm::ChooseTarget(String Prompt, int16 rType)
     Thing * Term::ExamineSquare(int x, int y)
   Other Prompts
     char TextTerm::ChoicePrompt(const char*msg,const char*choices,int8 col1, int8 col2)
     Thing* TextTerm::AcquisitionPrompt(int8 Reason, int8 minlev, int8 maxlev, int8 MType)
     virtual rID  ChooseResource(const char*prompt, int16 RType, rID eID)=0;
     virtual int32 MonsterTypePrompt(const char * prompt, int minCount = 1, int maxCount = 99999)=0;
     virtual rID MonsterOfTypePrompt(int32 type, const char * prompt)=0;
     virtual rID MonsterPrompt(const char * prompt)=0; 
     String TextTerm::StringPrompt(int8 col, const char*msg)
   Splash Screens
     void TextTerm::Title()
     void TextTerm::DisplayLegend()
     void TextTerm::DisplayKnownItems();

*/



#include "Incursion.h"
#define GLYPHS_SHOWN 25

/*****************************************************************************\
*                                  TEXTTERM                                  *
*                                Main Display                                *
\*****************************************************************************/

void TextTerm::ShowStatus()
	{
    int16 i,j, mColor, statiLen; rID spID;
		String LStatLine, MStatLine, SStatLine, Loc, *sp, add;
    
    if (Mode != MO_PLAY && Mode != MO_INV && Mode != MO_CREATE &&
        Mode != MO_BARTER)
      return;
    
    Color(AZURE); 
    SetWin(WIN_STATUS);
    Clear();
    SetWin(WIN_SCREEN);
		for(i=0;i<=WinBottom();i++)
      PutChar(WinRight()-15,i,179);
    SetWin(WIN_STATUS);
		Color(7);
    if (!theGame->InPlay())
      Write(0,0,Format("Character Generation: %s",p->Named.GetLength()==0 ? "unnamed" :
                                            (const char*)p->Named));
    else {
      if (m->dID == m->RegionAt(p->x,p->y))
        Loc = NAME(m->dID);
      else
		    Loc = Format("%s: %s",
			    NAME(m->dID),
          NAME(m->RegionAt(p->x,p->y)));
      if (p->LightRange == 0 || p->isBlind())
        ;
      else if (TTER(m->PTerrainAt(p->x,p->y,p))->HasFlag(TF_SHOWNAME) && 
            (p->Next && p->Percieves(oThing(p->Next))))
        Loc += Format(" (%s; %s)",NAME(m->PTerrainAt(p->x,p->y,p)), 
          (const char*)oThing(p->Next)->Name(NA_INSC));
      else if (TTER(m->PTerrainAt(p->x,p->y,p))->HasFlag(TF_SHOWNAME))
       Loc += Format(" (%s)",NAME(m->PTerrainAt(p->x,p->y,p)));
      else if (p->m->PileAt(p->x,p->y))
        Loc += " (pile of objects)";
      else if (p->Next && isValidHandle(p->Next))
        Loc += Format(" (%s)",(const char*)oThing(p->Next)->Name(NA_INSC));
      if (m->At(p->x,p->y).isSkylight && p->LightRange != 0 && p->isBlind())
        Loc += " (open above)";
      
      if (p->HasStati(ENGULFED))
        Loc = Format("%s: %s", p->GetStatiVal(ENGULFED) == EG_ENGULF ?
                "Engulfed" : "Swallowed", (const char*) 
                Capitalize(p->GetStatiObj(ENGULFED)->Name(0),true));
      
      if (Loc.GetLength() > WinRight()-4)
        Loc = Loc.Left(WinRight()-8) + Format("...%c%s",-7,
                   p->HasStati(ENGULFED) ? "" : ")");
      Write(0,0,Loc);

      if (p->m && p->m->Depth)
		    Write(WinRight()-3,0,Format("%03dm",10 * p->m->Depth));
      else
        Write(WinRight()-4,0,"Study");
      }
    if (p->kcMana() + p->hMana >= p->ktMana())
      mColor = EMERALD; /* Full Mana */
    else if ((p->kcMana()+p->hMana)*2 >= p->ktMana())
      mColor = YELLOW; /* Regenerating Mana */
    else
      mColor = RED; /* No Regen Till Rest */
      
    statiLen = WinSizeX() - 3;
    if (!p->Opt(OPT_SIDEBAR)) {
      String s;
		  s = Format("HP:%d/%d Mana:%c%d%c/%d FP:%d/%d GP:%d",
			  (int16)p->cHP+p->KAttr[A_THP]-p->Attr[A_THP],
        (int16)p->mHP+p->Attr[A_THP],
			  (int16)-mColor,
        (int16)p->kcMana(),
        (int16)-7,(int16)p->ktMana(),
        p->cFP - (p->Attr[A_FAT] - p->KAttr[A_FAT]),
        p->KAttr[A_FAT],
        p->getTotalMoney()/100);
      Write(0,1,s);
      statiLen = WinRight() - (2 + s.TrueLength());
    }

    LStatLine.Empty();
    MStatLine.Empty();
    SStatLine.Empty();

  rID shownIDs[1024];
  int lastShown = 0; 

  StatiIter(p)
    if (S->eID && S->h == p->myHandle)
      for (j=0;p->AutoBuffs[j];j++)
        if (theGame->SpellID(p->AutoBuffs[j]) == S->eID)
          goto SkipThisOne;
        
    if (LookupOnly(StatiLineStats,S->Nature)) {
      if (S->Nature == FLAWLESS_DODGE)
        if (!S->Val)
          continue;
      LStatLine += XPrint(Lookup(StatiLineStats,S->Nature));
      MStatLine += XPrint(Lookup(StatiLineStats,S->Nature));
      if (S->Nature == CHARGING && S->Mag) {
                    int32 bonus = p->ChargeBonus(); 
                    LStatLine += Format("%c%+d%c ", -12,bonus,-7);
                    MStatLine += Format("%c%+d%c ", -12,bonus,-7);
                  }
      if (S->Nature == FLAWLESS_DODGE)
        {
          LStatLine += Format("%c(%d left)%c ",-10,
            p->AbilityLevel(CA_FLAWLESS_DODGE) + p->Mod(A_DEX) - S->Mag);
          MStatLine += Format("%c(%d)%c ",-10,
            p->AbilityLevel(CA_FLAWLESS_DODGE) + p->Mod(A_DEX) - S->Mag);
        }  
      if (LookupOnly(StatiLineShorts,S->Nature)) {
        SStatLine += XPrint(Lookup(StatiLineShorts,S->Nature));
              }
    } else if (S->eID && 
        S->Nature != WEP_SKILL && S->Nature != TEMPLATE && 
        S->Nature != SUSPEND_ATTK && 
        (S->Duration > 0 || S->Duration < 0) && 
        S->Source != SS_RACE &&
        S->Source != SS_CLAS &&
        S->Source != SS_TMPL &&
        S->Source != SS_BODY &&
        RES(S->eID)->Type == T_TEFFECT) {
      TEffect * te = TEFF(S->eID); 
      if (te->HasFlag(EF_SHOWNAME) || te->GetMessages(MSG_STATINAME)[0].GetLength()) {
        bool found = false;
        for (int j=0; !found && j<lastShown; j++)
          if (S->eID == shownIDs[j])
            found = true;
        if (!found) { 
          shownIDs[lastShown++] = S->eID; 

          bool isBad = !te->HasFlag(EF_NOTBAD);

          String LS; 
          String MS; 
          String SS;

          if (isBad) {
            LS = MS = SS = XPrint(Format("<12>%s<7>",NAME(S->eID)));
          } else {
            sp = TEFF(S->eID)->GetMessages(MSG_STATINAME);
            if (sp[0].GetLength()) {
              LS = sp[0]; 
              if (sp[1].GetLength()) {
                MS = SS = sp[1];
              } else {
                MS = SS = sp[0];
              } 
            } else {
              LS = MS = SS = NAME(S->eID); 
            } 
          } 
          if (TEFF(S->eID)->HasFlag(EF_SHOWVAL)) {
            add = Format(" (%d)",S->Val); 
            LS += add;
            MS += add;
            SS += add; 
          }
          LStatLine += LS + " ";
          MStatLine += MS + " ";
          SStatLine += SS + " ";
        }
      } 
    }
    SkipThisOne:;
  StatiIterEnd(p)
  if (p->AutoBuffs[0])
    {
      bool all, some;
      all = true; some = false;
      for(j=0;p->AutoBuffs[j];j++) {
        if (p->HasEffStati(-1,theGame->SpellID(p->AutoBuffs[j])) ||
            p->HasEffField(theGame->SpellID(p->AutoBuffs[j])))
          some = true;
        else 
          all = false;
        }
      if (all)
        { LStatLine += XPrint("<10>Buffed<7> ");
          MStatLine += XPrint("<10>Buffed<7> ");
          SStatLine += XPrint("<10>Bufd<7> "); }
      else if (some)
        { LStatLine += XPrint("<14>Semibuffed<7> ");
          MStatLine += XPrint("<14>Semibuffed<7> ");
          SStatLine += XPrint("<14>Semi<7> "); }
      else
        { LStatLine += XPrint("<12>UNBUFFED<7> ");
          MStatLine += XPrint("<12>UNBUFFED<7> ");
          SStatLine += XPrint("<12>UNBUFF<7> "); }   
    }
  
    if (p->Flags & F_DELETE) {
      LStatLine += XPrint("<4>Dead<7>");
      MStatLine += XPrint("<4>Dead<7>");
      }
    if (LStatLine.GetLength() == 0)
      goto DoneStati;
      
    LStatLine = Trim(LStatLine);
    MStatLine = Trim(MStatLine);
    SStatLine = Trim(SStatLine);
      
    {
       
      if (LStatLine.TrueLength() < statiLen)
        Write(max(1,WinRight()-LStatLine.TrueLength()),1,LStatLine);
      else if (MStatLine.TrueLength() < statiLen)
        Write(max(1,WinRight()-MStatLine.TrueLength()),1,MStatLine);
      else {
        SStatLine = SStatLine.TrueLeft(statiLen);
        Write(max(1,WinRight()-SStatLine.TrueLength()),1,SStatLine);
        }
    }
    DoneStati:

    if (theGame->InPlay()) {
      if (p->ktMana() > p->kcMana()*3)
        j = RED*16;
      else
        j = 0;
      Color(WHITE|j);
      PutChar(sizeX-15,0,'[');
      PutChar(sizeX-1,0,']');
      for(i=sizeX-14;i!=sizeX-1;i++) {
        if (((p->hMana)*(14)/(p->ktMana())) > i-(sizeX-14))
          Color(BROWN|j);        
        else if (((p->ktMana()-p->cMana())*(13)/
                       p->ktMana()) > i-(sizeX-14))
          Color(EMERALD|j);
        else
          Color(GREY|j);
        PutChar(i,0,'*');
        }
      
      if ((p->mHP+p->Attr[A_THP]) > p->cHP*3)
        j = RED*16 + BRIGHT_MASK(0);
      else
        j = 0;
      Color(WHITE|j);
      PutChar(sizeX-15,1,'[');
      PutChar(sizeX-1,1,']');
      for(i=sizeX-14;i!=sizeX-1;i++) {
        if ((((p->mHP+p->Attr[A_THP])-p->cHP)*(14)/(p->mHP+p->Attr[A_THP])) > i-(sizeX-14))
          Color(RED|j);
        else
          Color(GREY|j);
        PutChar(i,1,'*');
        }
      }
  }

void TextTerm::ShowTraits()
  {
    int i; bool SuperSneak;
    String FName, s, form, attks, dstr;

    if (Mode != MO_PLAY && Mode != MO_INV && Mode != MO_CREATE &&
        Mode != MO_BARTER)
      return;

    p->statiChanged = false;

    SetWin(WIN_TRAITS);
    Clear();
    Color(7);
    GotoXY(0,0);

    TimeLine = 2;

    if (!p->Opt(OPT_SIDEBAR)) {
      FName = p->Named;
      if (FName.strchr(' ') && strlen(FName) > (WinSizeX()-1))
        if (stricmp(NAME(p->ClassID[0]),"Monk"))
          FName = FName.Left(FName.strchr(' '));
      FName = FName.Left(WinSizeX()-1);
	    TimeLine = 4;
	    if ((p->Image & 0x0F00)/256 == 15)
	      Write(Format("%c%s%c\n",-AZURE,(const char*)FName,-7));
	    else
	      Write(Format("%c%s%c\n",-((p->Image & 0x0F00)/256),(const char*)FName,-7));
      if (p->RaceID) {
        if (strlen(NAME(p->RaceID)) >= WinSizeX() - 7)
          Write(Format("%s %s\n",p->StateFlags & MS_FEMALE ? "F" : "M",NAME(p->RaceID)));
        else
	        Write(Format("%s %s\n",p->StateFlags & MS_FEMALE ? "Female" : "Male",NAME(p->RaceID)));
        }
      else
        Write("Gender Race\n");

      if (p->ClassID[0])
        Write(Format("%s %d\n",NAME(p->ClassID[0]),p->Level[0]));
      else
        Write("Class Level\n");
      
      
      if (p->ClassID[1])
        { Write(Format("%s %d\n",NAME(p->ClassID[1]),p->Level[1])); TimeLine++; }
      if (p->ClassID[2])
        { Write(Format("%s %d\n",NAME(p->ClassID[2]),p->Level[2])); TimeLine++; }
      
      }
    
    if (p->XP_Drained)
      Color(RED);
    else 
      Color(SKYBLUE);
    Write(Format("%ld/%ld",max(0,p->XP - p->XP_Drained), p->NextLevXP()));
    Color (GREY);
    Write(" XP\n");
    if (!p->Opt(OPT_SIDEBAR))
      Write("\n");
    TimeLine++;
    
    Write("\n\n");
    ShowTime();
    Write("\n");
    
    if (p->m && !(GetMode() == MO_CREATE))
      {
        p->shownFF = p->isFlatFooted();
        if (p->HasStati(ACTING))
          { Color(p->GetStatiEID(ACTING) ? YELLOW : EMERALD);
            s = Capitalize(p->ActingVerb()); 
            Write(s); }
        else if (p->DigMode)
          { Color(RED); Write("Digging"); }
        else if (p->shownFF)
          { Color(GREY); Write("Exploring"); }
        else
          { Color(WHITE); Write("Fighting"); }
        Write("\n");
        Color(GREY);

        s1 = s2 = 0;
        if (p->Opt(OPT_SIDEBAR) && p->HungerState() >= CONTENT &&
            p->HungerState() <= SATIATED)
          ;
        else switch(p->HungerState())
          { 
            case BLOATED:   Color(CYAN); Write("Bloated\n"); break;
            case SATIATED:  Write("Satiated\n"); break;
            case CONTENT:   Write("Content\n"); break;
            case PECKISH:   Write("Peckish\n"); break;
            case HUNGRY:    Color(BROWN); Write("Hungry\n"); break;
            case STARVING:  Color(PINK); Write("Starving\n"); break;
            case WEAK:      Color(RED | WHITE << 4); Write("Weak\n"); break;
            case FAINTING:  Color(RED | WHITE << 4); Write("Fainting\n"); break;
            case STARVED:  Color(RED | WHITE << 4); Write("Starved!\n"); break;
          }
        Color(GREY);

        /* Locomotion */
        if (p->HasStati(LEVITATION))
          { Write(p->GetStatiVal(LEVITATION) ? 
              "Flying\n" : "Levitating\n"); s1 = LEVITATION; }
        else if (p->HasStati(ELEVATED))
          { Write("Climbing\n"); s1 = ELEVATED; }         
        else if (p->HasStati(MOUNTED))
          Write("Mounted\n");
        else if (!p->Opt(OPT_SIDEBAR))
          Write("Afoot\n");
        
        /* Health State */
        if (p->HasStati(PARALYSIS))
          { Color(BLUE); Write("Paralyzed\n"); s2 = PARALYSIS; }
        else if (p->HasStati(POISONED)) {
          if (p->HasStati(SLOW_POISON))
            Write("(Poisoned)\n");
          else
            { Color(GREEN); Write("Poisoned\n"); }
          s2 = POISONED;
          }
        else if (p->HasStati(DISEASED))
          { Color(GREEN); Write("Diseased\n"); s2 = DISEASED; }
        else if (p->HasStati(STUNNED))
          { Color(YELLOW); Write("Stunned\n"); s2 = STUNNED; }
        else if (p->HasStati(SICKNESS))
          { Color(RED);    Write("Sickly\n"); s2 = SICKNESS; }
        else if (p->HasStati(CONFUSED))
          { Color(PINK);   Write("Confused\n"); s2 = CONFUSED; }
        else if (p->HasStati(BLINDNESS))
          { Color(BLUE);   Write("Blind\n"); s2 = BLINDNESS; }
        else if (!p->Opt(OPT_SIDEBAR))
          Write("Healthy\n");
        
        /* Fatigue State */
        if ((p->cFP >= 0) || p->HasStati(RAGING))         
          { 
            if (!p->Opt(OPT_SIDEBAR)) {
              Color(GREY); Write("Vigorous\n"); 
            }
          }
        else if (p->cFP >= -p->Attr[A_FAT]) 
          { Color(RED); Write("Fatigued\n"); }
        else
          { Color(RED | WHITE << 4); Write("Exhausted\n"); }
        Color(GREY);
        Write("\n");
        
      }
    else
      Write("Readyness\nHunger State\nLocomotion\nHealth State\nFatigue State\n\n");

    for(i=0;i!=7;i++)
    {
      Color(GREY);
      if (p->BAttr[A_STR] == -1)
        Write ("??\n");
      else {
        Write(Format("%s: ",AttrTitle[i]));
        if (p->HasStati(ADJUST_DMG,i))
          Color(RED);
        else if (p->IAttr(i) > p->KAttr[i])
          Color(BROWN);
        else if (p->IAttr(i) < p->KAttr[i])
          Color(EMERALD);
        else
          Color(GREY);
        Write(Format("%2d",p->KAttr[i]));
        if (p->Opt(OPT_SIDEBAR) && i >= 0 && i <= 2) {
          int16 a,b;
          if (i == 0) {
            a = (int16)p->cHP+p->KAttr[A_THP]-p->Attr[A_THP];
            b = (int16)p->mHP+p->Attr[A_THP];
          } else if (i == 1) {
            a = (int16)p->kcMana();
            b = (int16)p->ktMana();
          } else if (i == 2) {
            a = p->cFP - (p->Attr[A_FAT] - p->KAttr[A_FAT]);
            b = p->KAttr[A_FAT];
          }
          s;
          if (a == b) 
            s = (Format(" %c--%d--%c",-7,a,-7));
          else if (a * 2 > b)
            s = (Format(" %c%d/%d%c",-YELLOW,a,b,-7));
          else 
            s = (Format(" %c%d/%d%c",-RED,a,b,-7));
          Write(s);
          if (s.GetTrueLength() <= 7)
            Write("\n");
        } else Write("\n"); 
      }
    }
    Color(7);

    if (p->BAttr[A_STR] == -1)
      { SetWin(WIN_SCREEN); return; }

    Write("\n");
    i=p->AttackMode();
    Write(Format("Mod:%s\r",
          i==S_BRAWL ? "Brawl" :
          i==S_MELEE ? "Melee" :
          i==S_THROWN ? "Thrown" :
          i==S_ARCHERY ? "Archery" :
          i==S_DUAL && (p->InSlot(SL_WEAPON) == p->InSlot(SL_READY)) ? "Double" :
          i==S_DUAL ? "Two-Wep" :
          "Unknown"));
      
    if (i == S_DUAL) {
      Write(Format("Hit:%d / %d\r",p->KAttr[A_HIT_MELEE],p->KAttr[A_HIT_OFFHAND]));
      ShowDamage("Dmg",p->InSlot(SL_WEAPON),S_MELEE);
      ShowDamage("Off",p->InSlot(SL_READY),S_DUAL);
      Write(Format("Spd:%c%d~%c / %c%d~%c\r",  
        (p->WepSkill(p->InSlot(SL_WEAPON)) == WS_NOT_PROF) ? -4 : -7,
        100 + p->KAttr[A_SPD_MELEE]*5, -7,
        (p->WepSkill(p->InSlot(SL_READY)) == WS_NOT_PROF) ? -4 : -7,
        100 + p->KAttr[A_SPD_OFFHAND]*5, -7));
    } else if (i == S_BRAWL) {
      int count = 0; bool multi = false;
      TAttack buf[1024];
      int max = p->ListAttacks(buf,1024);
      attks = "";
      for (i=0;i<max;i++) {
        TAttack * at = &buf[i]; 
        if (is_standard_attk(at->AType) || is_postgrab_attk(at->AType)
            || at->AType == A_ALSO) {

          Dice Dmg = at->u.a.Dmg;
          dstr = Dmg.Str(); 
          if (Dmg.Number == 0 && Dmg.Sides == 0 && Dmg.Bonus == 0) {
            if (at->AType == A_ALSO)
              dstr = Format("DC %d",at->u.a.DC);
            else
            continue; 
          } 
          if (at->AType != A_ALSO) {
            Dmg.Bonus += p->KAttr[A_DMG_BRAWL];
            if (count && !p->HasFeat(FT_REND))
              Dmg.Bonus -= p->KMod(A_STR)/2;
            dstr = Dmg.Str(); 
          } 
          form = SC(Lookup(AttkVerbs1,at->AType)).Capitalize();
          if (at->DType != AD_SLASH &&
              at->DType != AD_BLUNT &&
              at->DType != AD_PIERCE) {
            dstr += Format(" %.4s", Lookup(DTypeNames,at->DType));
          } 
          if (is_postgrab_attk(at->AType)) {
            attks += Format("%c(%.*s:%s)%c\r",-SHADOW,
                  12 - dstr.GetLength(), (const char*)form, (const char*)dstr,-GREY);
          } else if (at->AType == A_ALSO) {
            attks += Format(" +%s\r", (const char*)dstr);
          } else { 
            if (count == 1) {
              multi = true;
            } 
            count++;
            attks += Format("%.*s:%s\r",
                  14 - dstr.GetLength(), (const char*)form, (const char*)dstr);
          } 
        }
      }
      // ww: the old brawl show was vastly wrong for polymorphed PCs
      if (multi)
        Write(Format("Hit:%d / %d\r",p->KAttr[A_HIT_BRAWL],
                       p->KAttr[A_HIT_BRAWL] -
                      (p->HasFeat(FT_MULTIATTACK) ? 2 : 5)));
      else
        Write(Format("Hit:%d\r",p->KAttr[A_HIT_BRAWL]));
      Write(attks);
      Color(GREY);
      Write(Format("Spd:%d~\r", 100 + p->KAttr[A_SPD_BRAWL]* 5));
    } else {
      Write(Format("Hit:%d\r",p->KAttr[A_HIT_ARCHERY+i]));
      ShowDamage("Dmg",p->InSlot(SL_WEAPON),i);
      Write(Format("Spd:%c%d~%c\n", (p->InSlot(SL_WEAPON) && 
        p->WepSkill(p->InSlot(SL_WEAPON)) == WS_NOT_PROF) ? -4 : -7,
        100 + p->KAttr[A_SPD_ARCHERY+i]*5, -7));
      }
    if (p->Opt(OPT_SIDEBAR) && p->Encumbrance() <= EN_LIGHT)
      ;  // do nothing, don't display this
    else 
      Write(Format("Enc:%s\r",EncumbranceNames[p->Encumbrance()]));
    if (p->StateFlags & MS_CASTING)
      Write(Format("Def:%d (cast)\r",p->KAttr[A_CDEF]));
    else if (p->KAttr[A_COV])
      Write(Format("Def:%d Cov:%d\r",p->KAttr[A_DEF],p->KAttr[A_COV]));
    else
      Write(Format("Def:%d\r",p->KAttr[A_DEF]));
    
    Write(Format("Arm:%c%2d Slash\r    %2d Pierce\r    %2d Blunt%c\r", (p->InSlot(SL_ARMOR) 
            && p->WepSkill(p->InSlot(SL_ARMOR)) == WS_NOT_PROF) ? -4 : -7,
          p->ResistLevel(AD_SLASH),p->ResistLevel(AD_PIERCE),p->ResistLevel(AD_BLUNT),-7));
    if (theGame->InPlay() && p->m) {
      int32 attr = p->MoveAttr(p->x,p->y);
      Write(Format("Mov:%c%d%c~\r",
        p->HasStati(SPRINTING) ? -10 : -7,attr,-7));
    } else Write(Format("Mov:%d~\r",p->KAttr[A_MOV]*5+100));

    if (theGame->InPlay() && Mode == MO_PLAY) {
      SizeWin(WIN_VIEWLIST,-1,CursorY()+1,-1,-1);
      ShowViewList();
      }
    SetWin(WIN_SCREEN);
	}

void TextTerm::ShowTime()
  {
    uint8 hour, min, sec, day;
    const char *Months[] = { "Suntide", "Harvest", "Leafdry", "Softwind",
                             "Thincold", "Deepcold", "Midwint", "Arvester", 
                             "Reprise", "Icemelt", "Turnleaf", "Blossom" };
    const uint32 turn = theGame->GetTurn();
    hour = (turn/18000) % 24;
    min  = (turn/300) % 60;
    sec  = (turn/5) % 60;
    day  = (turn/432000L);
    SetWin(WIN_TRAITS); Color(GREY);
    switch (p->Opt(OPT_PRECISE_TIME)) {
      case 0:  // minutes 
        Write(0,TimeLine,Format("%d:%02d %s  \n", (hour%12)+1, min,
              (hour > 12) && (hour != 23) ? "PM" : "AM"));
        if (!p->Opt(OPT_SIDEBAR)) // omit the day part of the date
          Write(Format("%d%s of %s\n", (day % 30)+1, NumPrefix((day%30)+1),
                         Months[(day/30)%12]));
        break;
      case 1: // seconds 
        Write(0,TimeLine,Format("%d:%02d:%02d %s  \n", 
              (hour%12)+1, min, sec, (hour > 12) && (hour != 23) ? "PM" : "AM"));
        break;
      case 2: // turns
        Write(0,TimeLine,Format("%d\n",turn));
        break; 
    }
  }

void TextTerm::ShowDamage(const char*str, Item *w, int8 Mode)
  {
    bool SuperSneak = false;
    Write(Format("%s:%s\n", (const char*)str, (const char*)(p->DmgVal(Mode,false)).Str()));
    if ((p->HasStati(HIDING) || p->HasStati(INVIS) || p->HasStati(INVIS_TO)) 
            && p->AbilityLevel(CA_SNEAK_ATTACK))
      {
        SuperSneak = false;
        if (w && w->isType(T_WEAPON))
          if (TITEM(w->iID)->HasFlag(WT_SUPER_SNEAK))
            SuperSneak = true;
        Write(Format("    +%dd%d SA\n",p->AbilityLevel(CA_SNEAK_ATTACK),
          SuperSneak ? 8 : (p->HasFeat(FT_MARTIAL_MASTERY) || w) ? 6 : 4));
      }

    if (w && w->isType(T_WEAPON)) {
      if (w->KnownQuality(WQ_FLAMING))
        Write("    +1d6 Fire\n"); 
      if (w->KnownQuality(WQ_SHOCKING))
        Write("    +1d6 Elec\n"); 
      if (w->KnownQuality(WQ_FROST))
        Write("    +1d6 Cold\n"); 
      if (w->KnownQuality(WQ_THUNDERING))
        Write("    +1d6 Sonic\n"); 
      if (w->KnownQuality(WQ_CORROSIVE))
        Write("    +1d6 Acid\n"); 


      if (w->KnownQuality(WQ_HOLY))
        Write("    +2d6 vs. E\n"); 
      if (w->KnownQuality(WQ_UNHOLY))
        Write("    +2d6 vs. G\n"); 
      if (w->KnownQuality(WQ_LAWFUL))
        Write("    +2d6 vs. C\n"); 
      if (w->KnownQuality(WQ_CHAOTIC))
        Write("    +2d6 vs. L\n"); 
      }
  }



/*****************************************************************************\
*                                  TEXTTERM                                  *
*                              Rendering the Map                             *
\*****************************************************************************/

inline int16 Priority(int16 t)
	{
		if(t == T_MONSTER || t == T_NPC || t == T_PLAYER)
			return 3;
		if(t >= T_FIRSTITEM && t <= T_LASTITEM)
			return 2;
		return 1;
	}


void Map::Update(int16 x,int16 y)
{
  int16 i; uint8 items, creatures, players; bool hi = false;
  int16 Pri; Thing *t, *bt = NULL;
  uint16 Vis, CVis; Player *p = oPlayer(pl[0]);
  Glyph g,hg,mg;
  if (x == -1 || !p)
    return;
  if (T1->GetMode() != MO_PLAY)
    return;
  if (T1->getMap() != this)
    return;
  const bool isEngulfed = p->HasStati(ENGULFED);
  
  if (isEngulfed)
    {
      uint16 col = p->GetStatiObj(ENGULFED)->Image & 0x0F00,
             ex  = p->GetStatiObj(ENGULFED)->x,
             ey  = p->GetStatiObj(ENGULFED)->y;
      /*
      if (x == ex && (y == ey+1 || y == ey-1))
        { T1->PutChar(x-T1->OffsetX(),y-T1->OffsetY(),'-' | col); return; }
      if (y == ey && (x == ex+1 || x == ex-1))
        { T1->PutChar(x-T1->OffsetX(),y-T1->OffsetY(),'|' | col); return; }
      if ( (x == ex+1 && y == ey+1) ||
           (x == ex-1 && y == ey-1) )
        { T1->PutChar(x-T1->OffsetX(),y-T1->OffsetY(),'/' | col); return; }
      if ( (x == ex+1 && y == ey-1) ||
           (x == ex-1 && y == ey+1) )
        { T1->PutChar(x-T1->OffsetX(),y-T1->OffsetY(),'\\' | col); return; }
      */
    }

  // ww: what is this block doing?
  // fjm: It was there to give me something to breakpoint
  //   on while debugging. Then it got forgotten. :(
  /*
     if (ThingAt(x,y) && ThingAt(x,y)->isPlayer())
     {
     i++;
     i--;
     }
   */

  g=mg=GlyphAt(x,y);

  for(i=0;i != MAX_PLAYERS && pl[i];i++)
    if (oPlayer(pl[i])->UpdateMap)
    { 
      oPlayer(pl[i])->MyTerm->ShowMap(); 
      return; 
    }
  Pri=0; items = 0; Vis = 0; creatures = 0;

  if (At(x,y).hasField)
    if (FieldAt(x,y,FI_SIZE) && !At(x,y).Solid) {
      bool found;
      found = false;
      for (i=0;Fields[i];i++)
        if (Fields[i]->FType & FI_SIZE)
          if (Fields[i]->inArea(x,y))
            if (p->Percieves(oThing(Fields[i]->Creator)))
              found = true;
      if (found && ((!At(x,y).Contents) || (!oThing(At(x,y).Contents)->isCreature())))
        goto DoDraw;
      }
      
  Nowhere:

  for(t=oThing(At(x,y).Contents);t;t=oThing(t->Next)) {
    CVis = p->Percieves(t);
    if (t->isMonster())
      if (!(((Creature*)t)->StateFlags & MS_SCRUTINIZED))
        if (CVis & (~PER_SHADOW))
          p->ScrutinizeMon((Creature*)t);
    
    if (!CVis) continue;
    /* ww: before, detect monsters was not revealing hiding monsters, etc. */
    /*
    if (t->Flags & F_INVIS && !(CVis & ~(PER_SHADOW)))
      continue;
      */
    if (t->isItem())
      items++;
    if (t->isCreature())
      creatures++;
    if (Priority(t->Type)>Pri)
    {
      Pri=Priority(t->Type);
      g=t->Image; bt = t;
      Vis = CVis;
      if (t->Flags & F_HILIGHT)
      { 
        if ((g & 0x0F00) == 0x0A00)
          g = (g & 0x00FF);
        hg = (g |= 0xA000); 
        hi = true;
      }
      #if 0
      else if (!(g & 0xF000)) {
        /* If the glyph foreground has the same color as the map
           background, change the glyph's foreground color. Then,
           apply the background color. */
      #endif
      else {
        if ((mg & 0xF000) == (g & 0x0F00) << 4)
          g = g & 0x00FF;
        g |= (mg & 0xF000);
      }
    }
#ifdef OLD_TRACKING
    for(i=0;pl[i] && i!=MAX_PLAYERS;i++)
      if (oPlayer(pl[i])->HasStati(TRACKING,-1,t))
      {
        if (!t->isCreature() && CreatureAt(t->x,t->y))
          continue;
        /* By definition, if they are onscreen they are in range
           for tracking. This may change if a graphical interface
           implements an exceptionally large view window, so this
           code is left here, commented out. */
        /*if (dist(oPlayer(pl[i])->x,oPlayer(pl[i])->y,t->x,t->y) 
          > oPlayer(pl[i])->GetStati(TRACKING,-1,t)->Mag)
          break;*/
        g = (t->Image & 0x0FFF) | 0xE000;
        goto DoDraw;
      }
#endif
  }

  if (Vis == PER_SHADOW) {
    g = (g & 0xF000) | GLYPH_UNKNOWN | (SHADOW*256);
  }

  /* If the background and foreground are the same color,
     change the foreground so that the glyph is visible. */
  if ((g & 0xF000) == ((g & 0x0F00) << 4))
    g = (g & 0x0F00) ? (g & 0xF0FF) : (g | 0x0800);

  
  if(!((At(x,y).Visibility & VI_VISIBLE) || Vis) &&
        T1->getMap() == this) /*later, >> player*4 */
  {
    g = hi ? hg : (isEngulfed ? 0 : At(x,y).Memory);
    if (g)
      T1->PutGlyph(x,y,g);
    else
      T1->PutGlyph(x,y,' ');
    mg = 0;
    goto DoOverlay;
  }

DoDraw:
  if (creatures > 1 && Pri == 3)
    T1->PutGlyph(x,y,GLYPH_MULTI | (15 << 8) | (mg & 0xF000) );
  else if (items > 1 && Pri < 3) {
    Container * c = FChestAt(x,y); 
    if (c) {
      if (g & 0xF000)
        T1->PutGlyph(x,y,(g & 0xF000) | (c->Image & 0x0FFF));
    else
        T1->PutGlyph(x,y,c->Image);
    } else
      T1->PutGlyph(x,y,GLYPH_PILE | (7 << 8) | (mg & 0xF000) );
  } else
    T1->PutGlyph(x,y,g);

DoOverlay:
  if(ov.Active && !isEngulfed)
    for(i=0;i!=ov.GlyphCount;i++)
      if(ov.GlyphX[i]==x && ov.GlyphY[i]==y)
        T1->PutGlyph(x,y,ov.GlyphImage[i] | (mg & 0xF000));
}

void TextTerm::ShowMap()
{
  int16 x,y,i;
  Glyph g;
  if (Mode != MO_PLAY || !p->m) 
    return; 
  
  if (p->HasStati(ENGULFED))
    {
      Creature *en = (Creature*) p->GetStatiObj(ENGULFED);
      ASSERT(en && en->isCreature());
      Glyph col = en->Image & 0xFF00;
      SetWin(WIN_MAP);
      Clear();
    
      /* Draw the player */
      PutGlyph(p->x,p->y,p->Image);
      
      /* Draw the creature around the player */
      PutChar(p->x  -XOff,p->y-1-YOff,'-' | col);
      PutChar(p->x  -XOff,p->y+1-YOff,'-' | col);
      PutChar(p->x-1-XOff,p->y  -YOff,'|' | col);
      PutChar(p->x+1-XOff,p->y  -YOff,'|' | col);
      PutChar(p->x+1-XOff,p->y+1-YOff,'/' | col);
      PutChar(p->x+1-XOff,p->y-1-YOff,'\\' | col);
      PutChar(p->x-1-XOff,p->y+1-YOff,'\\' | col);
      PutChar(p->x-1-XOff,p->y-1-YOff,'/' | col);
      
      /* Later, draw other creature percieved by 
         telepathy, etc -- but get this working
         first! */
      return;
    }
  Check(m,"Null Map in ShowMap?!");
  Color(GREY);
  if (p->UpdateMap)
    p->CalcVision();
  p->UpdateMap = 0;
  SetWin(WIN_MAP);
  //const int16 sx = min(MSizeX(),m->SizeX() - XOff);
  //const int16 sy = min(MSizeY(),m->SizeY() - YOff);
  const int16 sx = MSizeX();
  const int16 sy = MSizeY();
  const int16 msx= m->SizeX();
  ResetWViewList(); 
  for(y=0;y<sy;y++)
    for(x=0;x<sx;x++)
    {
      if (!m->InBounds(x+XOff,y+YOff))
        {
          PutChar(x,y,' ');
          continue;
        }
      LocationInfo & mAt = m->At(x+XOff,y+YOff); 
      if(mAt.Visibility & VI_EXTERIOR) /*later, >> player*4 */
        PutGlyph(x+XOff, y+YOff, GLYPH_WALL + GREY * 256);
      else if(mAt.Visibility & VI_VISIBLE) { /*later, >> player*4 */
        PutGlyph(x+XOff, y+YOff, gr[(y+YOff)*msx + (x+XOff)].Glyph);
        WViewThing(m->FirstAt(x+XOff,y+YOff) , 
            dist(x+XOff,y+YOff,p->x,p->y), true);
      } else {
        g = mAt.Memory;
        if (g) {
          PutGlyph(x+XOff,y+YOff,g);
          WViewThing(m->FirstAt(x+XOff,y+YOff), 
                dist(x+XOff,y+YOff,p->x,p->y), false);
        } else
          PutGlyph(x+XOff,y+YOff,' ');
      }
    }
  ShowThings();
  if(m->ov.Active)
    m->ov.ShowGlyphs();
}

void TextTerm::AdjustMap(int16 vx, int16 vy,bool newmap)
	{
		int16 rx,ry;   //Position of player (Screen Coord. System)
		int16 ox,oy;
    int16 lx,ly;   // How close player must be to edge to cause scrolling
    if (!p->m->InBounds(vx,vy))
      return; 
      
    if (Mode != MO_PLAY)
      return;
      
    ox=XOff;
		oy=YOff;
		rx=vx-XOff;
		ry=vy-YOff;
		lx = min(6,MSizeX() / 6);
    ly = min(5,MSizeY() / 5);
    
    if (newmap)
      {
        XOff = vx - MSizeX() / 2;
        YOff = vy - MSizeY() / 2;
      }

    //Special case: MapX smaller than Windowx
    if (m->SizeX() <= MSizeX())
      XOff = -((MSizeX() - m->SizeX())/2);
		else
      {
        // Always Center the map?
        if (p->Opt(OPT_CENTER_MAP))
          XOff = vx - (MSizeX()/2);
        // Player too close to right edge?
        else if (rx+lx > MSizeX())
			    XOff += lx*2;
        // Player too close to left edge?
		    else if(rx < lx)
			    XOff-=lx*2;
      }

    //Special Case: MapY smaller than WindowY
    if (m->SizeY() <= MSizeY())
      YOff = -((MSizeY() - m->SizeY())/2);
    else
      {
        // Always Center the map?
        if (p->Opt(OPT_CENTER_MAP))
          YOff = vy - (MSizeY()/2);
        // Player too close to bottom edge?
		    if(ry+ly > MSizeY())
			    YOff+=ly*2;
		    // Player too close to top edge?
        else if(ry < ly)
			    YOff-=ly*2;
      }
		
		/*
    if(XOff<0 &&
			XOff=0;

		if(YOff<0 &&
			YOff=0;
		
    if((XOff > (m->SizeX()-MSizeX())) &&
        (m->SizeX() > MSizeX()))
			XOff = (m->SizeX()-MSizeX());

		if((YOff > (m->SizeY()-MSizeY())) &&
		    (m->SizeY() > MSizeY()))
			YOff = (m->SizeY()-MSizeY());
		*/
		
    if(ox!=XOff || oy!=YOff || p->UpdateMap)
			ShowMap();
	}

void TextTerm::ShowThings()
	{
		int16 i,fx,fy, Per; Thing *t; bool tracked; 
    double slope; int16 yint;
    Rect ViewRange(max(XOff-15,0),max(YOff-15,0),
      min(XOff+MSizeX()+15,m->SizeX()),min(YOff+MSizeY()+15,m->SizeY()));
    Rect ScreenRect(XOff,YOff,XOff+MSizeX(),YOff+MSizeY());
     
    /* TODO: Allow Selection of Offscreen Glyphs with EffectPrompt:
         The Term object will contain an array of 64 OffscreenGlyph
       objects, defined as follows:

       struct OffscreenGlyph {
         uint8 rx, ry; // Real Location 
         uint8 sx, sy; // Screen Location
         Thing *t;     // Object Pointer
         };
       
         Every call to ShowThings() will zero this array with memset,
       then fill it with the relevant info as the glyphs are drawn.
       Then, EffectPrompt can use this info to allow the player to
       look at offscreen objects. */

    /* Remove old offscreen monsters */
    for(i=0;i!=OffscreenC;i++)
      m->Update(OffscreenX[i],OffscreenY[i]);
    OffscreenC = 0;

    int16 sx = min(MSizeX(),m->SizeX() - XOff);
    int16 sy = min(MSizeY(),m->SizeY() - YOff);
    for(int16 y=0;y<sy;y++)
      for(int16 x=0;x<sx;x++) {
        if (!m->InBounds(x+XOff,y+YOff))
          continue;
        t = m->FirstAt(x+XOff,y+YOff);
        if (!t) continue; 
        /*
         * WW: this is it! my great contribution to Incursion speed: the
         * incredible bottleneck from beyond the grave ...
         *
         * (drumroll ...)
         *
         * A call to MapIterate inside ShowThings!
         *
         * In particular, this meant that if there were 10 items on a
         * single square (loot from a dead monster, say) we would be inside
         * this loop once for each item and then (horror!) we would call
         * m->Update() on that square 10 times!
         *
           MapIterate(m,t,i) 
           if(ViewRange.Within(t->x, t->y) || (t->Flags & F_HILIGHT))
           if (ScreenRect.Within(t->x, t->y))
           m->Update(t->x, t->y);
           else 
         */
        fx = x+XOff;
        fy = y+YOff;
        if(ViewRange.Within(t->x, t->y) || (t->Flags & F_HILIGHT)) {
          m->Update(fx,fy);
        } else if ((t->isCreature() || (t->Flags & F_HILIGHT)) && 
            (Per = p->Percieves(t)))
        {
          ASSERT(0);
          /* A preemptive fix to Angband's "off-screen breathers"
           * glitch: if a monster is within the player's LOS and
           * range of vision, but is not in the screen area, show
           * it, highlighted red, on the farthest edge of the 
           * screen in the direction which it is found.
           */
          /*
          fx = t->x;
          fy = t->y;
          if (fx < XOff)
            fx = XOff;
          if (fx > XOff+MSizeX()-1)
            fx = XOff+MSizeX()-1;
          if (fy < YOff)
            fy = YOff;
          if (fy > YOff+MSizeY()-1)
            fy = YOff+MSizeY()-1;
            */

#ifdef OLD_TRACKING
          if (p->HasStati(TRACKING,-1,t))
            ;
          else 
#endif
          if (t->Flags & F_HILIGHT) 
            PutGlyph(fx,fy,((Per == PER_SHADOW) ? ('?' + 256*7) : 
                            (t->Image & 0x0FFF)) | 0xA000);
          else if((m->At(t->x,t->y).Visibility & VI_VISIBLE) == false) /*later, >> player*4 */
          {
            if (t->isShadowShape())
              PutGlyph(fx,fy,GLYPH_UNKNOWN | 0x4800);
          }
          else
            PutGlyph(fx,fy,(t->Image & 0x0FFF) | 0x4000);
          if (OffscreenC < 64) {
            OffscreenX[OffscreenC] = fx;
            OffscreenY[OffscreenC] = fy;
            OffscreenC++;
          }
        }
      }
	}

int8 ColorShadingPriority[] = {
  10, 1, 10, 10, 10, 10, 3, 2, 10, 10, 10, 10, 10, 10, 5, 4 };

static int8 BrightenOnce[] = {
/* BLACK   becomes */     SHADOW,
/* BLUE    becomes */     AZURE,
/* GREEN   becomes */     EMERALD,
/* CYAN    becomes */     SKYBLUE,
/* RED     becomes */     PINK,
/* PURPLE  becomes */     MAGENTA,
/* BROWN   becomes */     YELLOW,
/* GREY    becomes */     BROWN,
/* SHADOW  becomes */     GREY,
/* AZURE   becomes */     BROWN,
/* EMERALD becomes */     BROWN,
/* SKYBLUE becomes */     BROWN,
/* PINK    becomes */     BROWN,
/* MAGENTA becomes */     BROWN,
/* YELLOW  becomes */     YELLOW,
/* WHITE   becomes */     BROWN, }; 

Glyph TextTerm::FloorShading(int16 x, int16 y, Glyph g)
  {
    int8 c;
    int32 b; 

    if (m->At(x,y).Dark)
      if (!m->SolidAt(x,y))
        return ' ';

    if (g != m->At(x,y).Glyph)
      return g;

    int Dist = dist(x,y,p->x,p->y); 
      
    if (!(m->At(x,y).Visibility & VI_VISIBLE) || Dist > p->SightRange) {
      if (m->At(x,y).Shade)
        return 
          ((m->At(x,y).Memory & 0xF000) == 0x1000 ? 
            (m->At(x,y).Memory & 0xF0FF) + (BLACK << 8) : 
            (m->At(x,y).Memory & 0xF0FF) + (BLUE << 8) );
      else
        return (m->At(x,y).Memory & 0x0F00) == 0x0700 ? 
          (m->At(x,y).Memory & 0xF0FF + (SHADOW << 8)) : 
          (m->At(x,y).Memory & 0xF7FF);     
      }
    if (!m->At(x,y).Shade)
      return g;

    c = (g & 0x0F00) / 256; 
    b = (g & 0xF000); 
    // if (abs(p->x - x) <= 1 && abs(p->y - y) <= 1 && p->LightRange >= 3) 
    if (Dist <= 1 && p->LightRange >= 3)
      c = BrightenOnce[BrightenOnce[c]];
    // else if (abs(p->x - x) <= p->LightRange/2 && abs(p->y - y) <= p->LightRange/2)
    else if (Dist <= p->LightRange / 2)
      c = BrightenOnce[c];
    return ((g & 0x00FF) + (c * 256)) | b;

    /*
    if (abs(p->x - x) <= 1 && abs(p->y - y) <= 1 && p->LightRange >= 3)
      c = 14;
    else if (abs(p->x - x) <= p->LightRange/2 && abs(p->y - y) <= p->LightRange/2)
      c = 6;
    else
      c = 7;

    if (ColorShadingPriority[c] > ColorShadingPriority[(g & 0x0F00)/256])
      return (g & 0x00FF) + (c * 256);
    else
      return g;
      */
  }

void TextTerm::PutGlyph(int16 x, int16 y,Glyph g) 
  {
    if(x<XOff || y<YOff || !m)
      return;
    if (x>XOff+Windows[WIN_MAP].Right || 
        y>YOff+Windows[WIN_MAP].Bottom-Windows[WIN_MAP].Top)
      return;
    if (m->At(x,y).Shade)
      g = FloorShading(x,y,g);

    if (m->At(x,y).hasField && ((g & 0xF000) != 0xF000)) {
      g = m->FieldGlyph(x,y,g);
    }
    
    APutChar(x-XOff + Windows[WIN_MAP].Left,
             y-YOff + Windows[WIN_MAP].Top,g);
    updated = false;
  }

Glyph TextTerm::GetGlyph(int16 x, int16 y)
  {
    if(x<XOff || y<YOff)
      return 0;
	  if(x>XOff+Windows[WIN_MAP].Right || y>YOff+Windows[WIN_MAP].Bottom-Windows[WIN_MAP].Top)
      return 0;
    return AGetChar(x-XOff+Windows[WIN_MAP].Left,y-YOff + Windows[WIN_MAP].Top);
  }

void TextTerm::ShowMapOverview()
{
  int16 sx, sy, x, y, mx, my, x1, x2, y1, y2, vx, vy, tx, ty;
  bool open, seen, onmap; int16 ch, mag, magn; uint16 xy;
  Glyph force; Creature *mn;
  int force_priority;

  bool wizsight = p && p->Opt(OPT_WIZ_SIGHT); 

  // ww: allow the player to cycle through things on the overview map
  // rapidly -- this is a nice convenience feature because usually when
  // you're done exploring a level you want to run to the down stair case
  // ... unfortunately, you still have to hunt for it! 
  enum {
    SEARCH_NONE,
    SEARCH_UP,
    SEARCH_DOWN,
    SEARCH_UNEXPLORED,
  } searching = SEARCH_NONE; 

  SetWin(WIN_BIGMAP);
  SetMode(MO_SPLASH);

  vx = p->x;
  vy = p->y;
  mag = 1;
  x1 = x2 = p->x;
  y1 = y2 = p->y;
  do {
    if (vx < x1) vx = x1; else if (vx > x2) vx = x2;
    if (vy < y1) vy = y1; else if (vy > y2) vy = y2; 

    Clear();
    x1 = x2 = p->x;
    y1 = y2 = p->y;
    for (sx=0;sx!=WinSizeX();sx++)
      for (sy=0;sy!=WinSizeY()-1;sy++)
      {
        open = false;
        seen = false;
        onmap = false;
        force = 0;
        force_priority = T_LAST;
        /* ww: we prefer features in their #define'd order ... which
         * happens to work because stairs (portals) are early */
        mx = vx - (WinSizeX()/2)*mag + sx*mag;
        my = vy - (WinSizeY()/2)*mag + sy*mag;

        for(x = mx; x!=mx+mag;x++)
          for (y = my; y!=my+mag;y++)
          {
            if (!m->InBounds(x,y))
              continue;

            if (m->At(x,y).Memory || wizsight)
              seen = true;

            onmap = true;

            /* ww: show stairs (portals) over doors, etc. */
            if (m->At(x,y).Memory && m->KnownFeatureAt(x,y)) {
              force_priority = m->KnownFeatureAt(x,y)->Type ;
              force = m->At(x,y).Memory ;
            } else if (wizsight && m->KnownFeatureAt(x,y)) {
              force_priority = m->KnownFeatureAt(x,y)->Type ;
              force = m->KnownFeatureAt(x,y)->Image;
            } 

            /* if all else fails, show walls and floors */
            if (force_priority == T_LAST && !force && wizsight)
              force = m->At(x,y).Glyph;

            if (force_priority == T_LAST && !force)
              force = m->At(x,y).Memory;
          }

        if ((onmap && seen) || force)
        {
          x1 = min(x1,mx);
          x2 = max(x2,mx);
          y1 = min(y1,my);
          y2 = max(y2,my);
        }

        if (p->x >= mx && p->y >= my && p->x < mx+mag && p->y < my+mag)
          PutChar(sx,sy,p->Image);
        else if (vx >= mx && vy >= my && vx < mx+mag && vy < my+mag)
          PutChar(sx,sy,'*' | YELLOW*256);
        else if (force)
          PutChar(sx,sy,force);
        else
          PutChar(sx,sy,' '+GREY*256);
      }

    GotoXY(0,WinSizeY()-1);
    Color(YELLOW);                                                                  
    Write(Format("[%s] Move Viewpoint [R] Run to Point [+] Zoom In [-] Zoom Out [ESC] Exit",Arrows));

BadChar:
    ch = GetCharCmd(KY_CMD_ARROW_MODE);

    magn = (ControlKeys & SHIFT) ? mag * 5 : mag; 

    switch(ch)
    {
      // ww: find Stairs going up ...
      case KY_CMD_UP:          
      case KY_COMMA: 
        searching = SEARCH_UP; break; 
      case KY_CMD_DOWN:        
      case KY_PERIOD: 
        searching = SEARCH_DOWN; break; 

      case KY_TAB: 
        searching = SEARCH_UNEXPLORED; break; 

      case '-': case '_': /* ww: + and - were flipped */
        if (mag < 5)
          mag++;
        else
          goto BadChar;
        break;
      case '+': case '=':
        if (mag > 1) /* ww: we should be able to zoom all the way in:
                      * it's currently the only way to scroll the
                      * viewport with detail */
          mag--;
        else
          goto BadChar;
        break;
      case 'R': case 'r':
        /* All kinds of checks... */
        tx = ty = -1;
        seen = open = false;
        for (x=vx;x!=vx+mag;x++)
          for (y=vy;y!=vy+mag;y++)
          {
            if (!m->InBounds(x,y))
              continue;
            if (!m->At(x,y).Memory)
              continue;
            seen = true;
            if (m->SolidAt(x,y))
              continue;
            open = true;
            if (tx != -1)
              if (m->KnownFeatureAt(tx,ty))
                if (m->KnownFeatureAt(tx,ty)->isType(T_PORTAL))
                  continue;
            tx = x;
            ty = y;
          }
        if (!seen)
        { Message("You can't run to an unexplored area."); break; }
        if (!open)
        { Message("You can't run to an area filled with stone."); break; }
        if (tx == -1)
          break;
        if (!p->RunTo(tx,ty))                                                          
          { Message("There's no clear, safe and explored route to that area."); break; }

      default: 
        SetWin(WIN_BIGMAP);
        Clear();
        SetWin(WIN_MAP);
        Clear();
        SetMode(MO_PLAY);
        ShowMap();
        ShowStatus();
        ShowTraits();
        linenum = linepos = 0;
        return; 

      case KY_CMD_NORTH:       vy -= magn; break; 
      case KY_CMD_NORTHEAST:   vy -= magn; vx += magn; break; 
      case KY_CMD_EAST:        vx += magn; break; 
      case KY_CMD_SOUTHEAST:   vy += magn; vx += magn; break; 
      case KY_CMD_SOUTH:       vy += magn; break; 
      case KY_CMD_SOUTHWEST:   vx -= magn; vy += magn; break; 
      case KY_CMD_WEST:        vx -= magn; break; 
      case KY_CMD_NORTHWEST:   vy -= magn; vx -= magn; break; 

    }

    if (searching != SEARCH_NONE) {
      int wx,wy,iter;
      int good_x = -1, good_y = -1; 
      // go through the map twice, first looking for things *after* this
      // point, then looking for things *before* this point -- this
      // allows the player to cycle through all things matching this
      // description by pressing the key over and over 
      int current_index = vy * m->SizeX() + vx; 
      for (iter=0; iter<2 && good_x == -1; iter++) 
        for (wy=0; wy<m->SizeY() && good_x == -1; wy++) 
          for (wx=0; wx<m->SizeX() && good_x == -1; wx++) {

            if (!m->At(wx,wy).Memory) continue; // don't know this place

            int w_index = wy * m->SizeX() + wx; 

            if      (iter == 0 && w_index <= current_index) continue; 
            else if (iter == 1 && w_index >= current_index) continue; 

            Feature * f = m->KnownFeatureAt(wx,wy);

            if (searching == SEARCH_UP) 
              if (!f || f->Type != T_PORTAL || 
                  TFEAT(f->fID)->xval != POR_UP_STAIR) continue; 

            if (searching == SEARCH_DOWN) 
              if (!f || f->Type != T_PORTAL || 
                  TFEAT(f->fID)->xval != POR_DOWN_STAIR) continue; 

            if (searching == SEARCH_UNEXPLORED) {
              // find a place we do know that 
              // (1) is not solid
              // (2) touches a place we don't know ...
              // (3) ... or touches a door that touches a place we don't know
              //        (4) massive convenience hack: (2) is not solid
                
              if (m->SolidAt(wx,wy)) continue; // couldn't walk here
              Door * d = m->FDoorAt(wx,wy);
              if (d && d->DoorFlags & DF_SECRET) continue; 
              // now check neighbors
              bool foundUnexplored = false; 
              for (int i=0;i<8;i++) {
                if (! m->At(wx + DirX[i], wy + DirY[i]).Memory &&
                    ! m->SolidAt(wx + DirX[i], wy + DirY[i]))
                  foundUnexplored = true;
                // *OR* (important special case) this is a door and we
                // don't know what is on the other side! 
                d = m->FDoorAt(wx + DirX[i], wy + DirY[i]);
                if (d && !(d->DoorFlags & DF_SECRET))
                  for (int j=0; j<8; j++) 
                    if (! m->At(wx + DirX[i] + DirX[j], 
                                wy + DirY[i] + DirY[j]).Memory)
                      foundUnexplored = true;
              }

              if (!foundUnexplored) continue; 
            } 

            good_x = wx;
            good_y = wy; 
          } 
      if (good_x == -1) {
        switch (searching) {
          case SEARCH_NONE: Message("Not found!"); break;  // !? 
          case SEARCH_UP: Message("No ascending stairs found!"); break; 
          case SEARCH_DOWN: Message("No descending stairs found!"); break; 
          case SEARCH_UNEXPLORED: 
                            Message("No explored areas found!"); break; 
        }
      } else {
        x1 = x2 = vx = good_x;
        y1 = y2 = vy = good_y; 
      } 
    } 
    searching = SEARCH_NONE; 

    SetWin(WIN_BIGMAP);
    SetMode(MO_SPLASH);
  }
  while (1);
}

/*****************************************************************************\
*                                 TextTerm                                   *
*                              View List  Code                                *
\*****************************************************************************/

static int16 ViewListPriorityMod(Thing *t)
  {
    if (t->isItem()) {
      if (t->isType(T_WEAPON) || t->isType(T_ARMOR))
        {
          if ((!((Item*)t)->Inscrip.GetLength()) ||
              ((Item*)t)->Inscrip == "{mundane}")
            return 15; /* Junk */
        }
      if (t->isType(T_MISSILE) || t->isType(T_FOOD) ||
          t->isType(T_CORPSE) || t->isType(T_LIGHT) )
        return 15; /* Probably Junk */

      return 5;
      }
    else if (t->isCreature())
      return max(0,-((Creature*)t)->ChallengeRating()/3);
    else if (t->isFeature())
      {
        if ((t->isType(T_DOOR)) ||
            (t->Image & 0xFF == GLYPH_STATUE))
          return 20;
        return 7;
      }
    else
      return 0;
  }

static int WViewListSortA(const void *a, const void *b)
{
  WViewListElt *aa = (WViewListElt *)a,
               *bb = (WViewListElt *)b;
 
  /* Creatures first */
  if (aa->t->isCreature() && !bb->t->isCreature())
    return aa->dist - (bb->dist+10);
  if (!aa->t->isCreature() && bb->t->isCreature())
    return (aa->dist+10) - bb->dist; 

  return (aa->dist + ViewListPriorityMod(aa->t)) - 
         (bb->dist + ViewListPriorityMod(bb->t)); 
}

static int WViewListSortB(const void *a, const void *b)
{
  WViewListElt *aa = (WViewListElt *)a,
               *bb = (WViewListElt *)b;

    return aa->dist - bb->dist; 
}



void TextTerm::ShowViewList()
  {
    int8 ResistsShown[] = {
      AD_FIRE, AD_COLD, AD_SONI, 
      AD_ACID, AD_ELEC, AD_TOXI, AD_NECR, AD_PSYC, AD_MAGC,
      AD_SLEE, AD_STUN, AD_PLYS, AD_STON, AD_POLY, AD_CHRM,
      AD_DISN, AD_CRIT, AD_DISE, AD_FEAR, 0 
      };
    int16 y,i,j; 
    String nm;  
    Container *pack;
    /*if (GetMode() != MO_PLAY)
      return;
    ListChanged = false;*/
    ShowMap();
    SetWin(WIN_VIEWLIST);
    Clear();
    Color(YELLOW);
    switch (p->Opt(OPT_LOWERSCREEN))
      {
        case 0: /* View List */
          {
            int linesWeCanUse = activeWin->Bottom - activeWin->Top;
            Write("Things in View:");
            if (p->HasStati(ENGULFED)) {
              Color(RED);
              PutChar(0,1,p->GetStatiObj(ENGULFED)->Image & 0x0FFF);
              WrapWrite(2,1,p->GetStatiObj(ENGULFED)->Name(NA_MONO),16,
                    linesWeCanUse - 2);
              break;
              }            
            /* 1st Priority: display all WViewListCount things in view,
             *               giving everything at least 1 line
             * 2nd Priority: if we have extra lines, give all of them
             *               to the Nearest Thing that needs more lines
             * 3rd Priority: if we still have some, repeat #2
             */
            y = 1;
            //#ifdef WEIMER_LIST
            qsort(WViewList,WViewListCount,sizeof(WViewList[0]),WViewListSortA);
            //#else
            //qsort(WViewList,WViewListCount,sizeof(WViewList[0]),WViewListSortB);
            //#endif
            int spareLines = linesWeCanUse - WViewListCount; 
            for (i=0;i<WViewListCount;i++) {
              if (y > linesWeCanUse)
                return;
              GotoXY(0,y);
              if (WViewList[i].t->x == -1)
                continue;
              PutChar(m->MultiAt( WViewList[i].t->x, WViewList[i].t->y ) ?
                (GLYPH_MULTI | WHITE*256) : WViewList[i].glyph);
              Color(WViewList[i].color);
              int code = WViewList[i].color == GREY ? 0 : NA_MONO;
              int linesUsed = 
                WrapWrite(2,y,WViewList[i].t->Name(code),16,
                    1 + (spareLines > 0 ? spareLines : 0));
              y += linesUsed;
              spareLines -= (linesUsed - 1); 
            }
          }
         break;

        case 1: /* Resists */
          Write("\nResistances:\n");
          y = 2; Color(GREY);
          for (i=0;ResistsShown[i];i++)
            if (j = p->ResistLevel(ResistsShown[i]))
              {
                nm = Lookup(DTypeNames,ResistsShown[i]);
                if (nm.Right(6) == "Damage")
                  nm = nm.Left(nm.GetLength() - 7);
                if (j == -1)
                  Write(Format("  %s (Immune)\n", (const char*)nm).Left(WinSizeX()-1));
                else
                  Write(Format("  %s %d\n", (const char*)nm,j).Left(WinSizeX()-1));
                y++;
                if (y > activeWin->Bottom)
                  break;
              }
          if (y == 2)
            Write("  None\n");
         break;
        case 2: /* Pack */
          Write("\nIn your Pack:\n");
          y = 2; Color(GREY);
          pack = (Container*) p->InSlot(SL_PACK);
          if (!pack || !pack->isType(T_CONTAIN))
            { Write("  (No Pack Equipped)"); break; }
          for(i=0;(*pack)[i];i++) {
            Write(Format("  %s",(const char*)(((*pack)[i])->Name(0))).Left(WinSizeX()-1));
            Write("\n");
            y++;
            if (y > activeWin->Bottom)
              break;
            }
          if (y == 2)
            Write("  Pack Empty.\n");
         break;   
        case 3: /* Equipment */
          Write("\nEquipment:\n");
          y = 2; Color(GREY);
          for(i=0;i!=SL_LAST;i++) 
            if (p->InSlot(i) && p->InSlot(i)->activeSlot(i)) {
              Write(Format("  %s",(const char*)p->InSlot(i)->Name(0)).Left(WinSizeX()-1));
              Write("\n");
              y++;
              if (y > activeWin->Bottom)
                break;
              }
          if (y == 2)
            Write("  Nothing.\n");
         break;   
        case 4: /* Party */
          Write("\nParty:\n");
          y = 2; Color(GREY);
          Write("  None");
         break;
        case 5: /* Quickkeys */
          Write("\nQuick Keys:\n");
          y = 2; Color(GREY);
          Write("  None Set.\n");
         break;
        case 6: /* Spellkeys */
          Write("\nSpellkeys:\n");
          y = 2; Color(GREY);
          for(i=0;i!=10;i++) 
            if (p->SpellKeys[i] >= 0) {
              Write(Format("%c%c%c %s",-SKYBLUE,'0'+i,-GREY,
                NAME(theGame->SpellID(p->SpellKeys[i]))).TrueLeft(WinSizeX()-1));
              Write("\n");

              y++;
              if (y > activeWin->Bottom)
                break;
              }
          if (y == 2)
            Write("  None Set.\n");
         break;
      }

  }

void TextTerm::ResetWViewList()
{
  WViewListCount=0;
  return;
}

void TextTerm::WViewThing(Thing *t, int32 dist, bool assertSeen)
{
  int itemCount = 0;
  int chestCount = 0;
  Glyph chestGlyph = 0; 
  Thing *tt;

  // we need to know if there is a pile or a chest here before we display
  // the first item
  for (tt = t; tt; tt = oThing(tt->Next))
    if (tt->isItem()) {
      itemCount++;
      if (tt->isType(T_CHEST)) {
        if (!chestGlyph) chestGlyph = tt->Image; 
        chestCount++;
    } 
    } 
  if (t) {
    int per = p->Percieves(t,assertSeen);
    if (!(per & ~PER_SHADOW)) 
      return; 
  } 

  // yeah, I'm sorry about the gotos as well ...
look_again: 
  if (!t)
    return;
  
  if (t == p ) 
    goto look_at_next; 
  if (t->isFeature())
    if ((t->Image & 0xFF) == GLYPH_TREE ||
        (t->Image & 0xFF) == GLYPH_BULK ||
        (t->Image & 0xFF) == GLYPH_PILLAR ||
        (t->Image & 0xFF) == GLYPH_HEDGE ||
        (t->Image & 0xFF) == GLYPH_FLOOR2 ||
        (t->Image & 0xFF) == GLYPH_FLOOR)
      goto look_at_next;
  
  if (t->isMonster()) {
    p->JournalNoteMonster((Monster *)t);
    ((Monster *)t)->ts.Retarget((Creature *)t); // ww: help out the AI
  }

  {
    int big_dist = -1;  // index into WViewList w/ most distant Thing
    int big_idx = -1;
    // ww: find somewhere to put it in this list ...
    // we have to scan through the whole thing anyway for duplicates, so
    // we'll just remember the least important one and replace it
    for (int i=0;i<WViewListCount;i++) {
      if (WViewList[i].t == t)
        goto look_at_next;
      if (WViewList[i].dist > big_dist) {
        big_dist = WViewList[i].dist;
        big_idx = i;
      } 
    } 
    if (WViewListCount < 16) 
      big_idx = WViewListCount++; 
    if (big_idx == -1) // not good enough to make the list, sorry!
      return; // and nothing else at this distance can be good enough ...
    int color ; 
    color = GREY;
    // Green for 'good' things, like sleeping monsters and magic items
    // Red for 'bad' things, like tough monsters
    // GREY (or whatever Name() gives) for the rest
    if (t->isCreature()) {
      Hostility h = ((Creature *)t)->ts.SpecificHostility((Creature *)t,p);
      switch (h.quality) {
        case Enemy: color = RED; break;
        case Ally: color = GREEN; break; 
        default: color = YELLOW; break; 
      }
    } else if (t->isItem())  {
      bool good = false;
      if (p->onPlane() != PHASE_MATERIAL)
        good = ((Item*)t)->isGhostTouch();
      else if (((Item*)t)->ItemLevel() > 0)
        good = true;
      if (good)  
        color = GREEN;
    } 
    // String name = t->Name(0); 
    Glyph g ; 
    if (itemCount > 1) {
      if (chestCount > 0)
        g = chestGlyph;
      else
        g = GREY * 256 + '*';
    } else g = t->Image;
    WViewList[big_idx].color = color; 
    WViewList[big_idx].glyph = g  ;
    WViewList[big_idx].dist = dist;
    WViewList[big_idx].t = t; 
  }

look_at_next: 
  t = oThing(t->Next);
  goto look_again;
}


/*****************************************************************************\
*                                  TEXTTERM                                  *
*                               Effect Prompt                                *
\*****************************************************************************/

inline String & DescribeResource(rID xID)
  {
    if (RES(xID)->Desc)
      return *tmpstr(DESC(xID));
    else
      return *tmpstr(XPrint("You see nothing exceptional about the <Res>.",xID));
  }

bool TextTerm::EffectPrompt(EventInfo &e, 
      uint16 fl,bool is_look, const char* PromptText)
{
  int16 i, ch, t, dist, best, bestd, typ, mod, tx, ty, range;
  Thing *th, *cr; hObj hi; String Prompt; Item *wp;
  bool first = true, found, cycled; 
  bool isEngulfed = p->HasStati(ENGULFED);
  cr = NULL; cycled = false;

  if (fl == 0 && e.eID && RES(e.eID)->Type == T_TEFFECT)
    fl = TEFF(e.eID)->ef.qval;
  
  /* Needed to calculate range so magic missile improper targets
     show as out-of-range and can't be selected. */
  if (e.eID)
    p->CalcEffect(e);

Reprompt:

  typ = fl & 0x000F;
  mod = Q_DIR;
  if (!typ)
    return true;



  if (is_look)
    Prompt = Format("%cLooking [rfmoxltn%s*?]:%c ",-9,Arrows,-7);
  else {
    if (typ == Q_TAR)
    {
SelectTarget:
      Prompt = Format("%cChoose your target",-9,Arrows,-7);
      Prompt += Format(": [%s%s%s%s%s%s%s]%c ", Arrows, "*", (fl & Q_DIR) ? "d" : "", 
          fl & Q_LOC ? "l" : "", fl & Q_TAR ? "tn" : "", fl & Q_INV ? "i" : "",
          fl & Q_ALL ? "a" : "", -7);
      mod = Q_TAR; cr = p;
    }
    else if (typ == Q_INV)
    {
SelectItem:
      found = false;
      for (th = p->FirstInv();th;th = p->NextInv())
        if (ThrowEff(EV_RATETARG,e.eID,th,th,th,th) != ABORT)
          //if (((Item*)th)->Parent == p->myHandle)
        {
          LOption(th->Name(NA_MECH|NA_LONG),th->myHandle);
          found = true;
        }
      if (!found) {
        Box("You have nothing suitable in your inventory to use that on.");
        if (typ != Q_INV) 
          goto Reprompt;
        e.ETarget = NULL; /* InventoryManager */
        return false;
        }
      if (typ != Q_INV)
        LOption("(non-inventory target)",-2);
      hi = LMenu(MENU_BORDER,PromptText ? PromptText : "Choose an item:",WIN_MENUBOX);
      if (hi == -2)
        goto Reprompt;
      if (hi == -1) {
        e.ETarget = NULL;
        return false;
      }
      e.ETarget = oItem(hi);
      return e.ETarget ? true : false;
    }
    else if (typ == Q_LOC)
    {
SelectLocation:
      Prompt = Format("%cChoose a location",-9);
      Prompt += Format(": [%s%s%s%s%s%s%s]%c ", Arrows, "*", (fl & Q_DIR) ? "d" : "", 
          fl & Q_LOC ? "l" : "", fl & Q_TAR ? "tn" : "", fl & Q_INV ? "i" : "",
          fl & Q_ALL ? "a" : "", -7);

      e.isLoc = true;
      mod = Q_LOC; 
      tx = p->x; 
      ty = p->y;
    }
    else
    {
      Prompt += "Select ";
      if (typ & Q_DIR) {
        Prompt += "direction";
        first = false;
      }
      if (typ & Q_LOC) {
        if (!first) {
          if (!(typ & (Q_TAR | Q_INV)))
            Prompt += " or ";
          else
            Prompt += ", ";
        }
        Prompt += "location";
        first = false;
      }
      if (typ & Q_TAR) {
        if (!first) {
          if (!(typ & Q_INV))
            Prompt += " or ";
          else
            Prompt += ", ";
        }
        Prompt += "target";
        first = false;
      }
      if (typ & Q_INV) {
        if (!first)
          Prompt += " or ";
        Prompt += "inventory item";
        first = false;
      }
      Prompt += Format(": [%s%s%s%s%s%s%s]%c ", Arrows, "*", (fl & Q_DIR) ? "d" : "", 
          fl & Q_LOC ? "l" : "", fl & Q_TAR ? "tn" : "", fl & Q_INV ? "i" : "",
          fl & Q_ALL ? "a" : "", -7);
    }
  }
  if (PromptText) 
    Prompt = SC(PromptText) + Prompt.Right(Prompt.GetLength()-(Prompt.strchr('[')-1));

  if ((fl == Q_DIR) || ((fl & Q_DIR) && theGame->Opt(OPT_SELECT_DIR)))
    mod = Q_DIR;
  else if (theGame->Opt(OPT_SELECT_JUMP) && (fl & Q_TAR)) {
    cr = p; mod = Q_TAR;
  }
  else if (fl & Q_LOC) {
    tx = p->x;
    ty = p->y;
    mod = Q_LOC;
  }
  
  range = e.vRange ? e.vRange : 80;
  if (fl & Q_NEAR)
    range = 1;
  if (isEngulfed)
    range = 1;

  while (1) {
    if (isEngulfed && mod == Q_TAR) {
      mod = Q_LOC;
      tx = cr->x;
      ty = cr->y;
      }
    /* Display the inverted box cursor */
    if (mod == Q_LOC) {
      if (isEngulfed) {
        if (tx == p->x && ty == p->y)
          cr = p;
        else
          cr = p->GetStatiObj(ENGULFED);
        }
      else if (m->InBounds(tx,ty))
        cr = m->FirstAt(tx,ty);
      m->Update(tx,ty);
      PutGlyph(tx,ty, (GetGlyph(tx,ty) & 0xFF) | 0xF000);
      }
    else if (mod == Q_TAR) {
      m->Update(cr->x,cr->y);
      PutGlyph(cr->x,cr->y, (GetGlyph(cr->x,cr->y) & 0xFF) | 0xF000);
      }
    SetWin(WIN_MESSAGE); Clear();
    linenum = linepos = 0;
    if (cr && (mod == Q_LOC || mod == Q_TAR)) {
      if (p->Percieves(cr) & (~PER_SHADOW)) {
        Write(cr->Name(NA_CAPS|NA_A|NA_LONG|NA_MECH|(is_look ? NA_STATI : 0)));
        if (cr->isCreature() && ((Creature*)cr)->StateFlags & MS_HAS_REACH)
          Write(" (reach)");
        }
      else if (p->Percieves(cr) & PER_SHADOW)
        Write(cr->Name(NA_CAPS|NA_A|NA_SHADOW));
      else
        goto CantSee;
      if ((p->HasStati(LIFESIGHT)) && cr->isCreature()
            && p->Percieves(cr)) 
        Write(Format(" (%d/%d %d/%d)",
              ((Creature*)cr)->cHP,
              ((Creature*)cr)->mHP+((Creature*)cr)->Attr[A_THP],
              ((Creature*)cr)->cMana(),
              ((Creature*)cr)->tMana()
              ));
      Write(0, 1, "  ");
              
      if (cr->HasStati(MOUNTED) && (p->Percieves(cr) & (~PER_SHADOW)))
        Write(Format("(riding %s)",(const char*)cr->GetStatiObj(MOUNTED)->
                       Name(NA_A|NA_LONG|NA_MECH|(is_look ? NA_STATI : 0))));
      else if (cr->HasStati(MOUNTED) && (p->Percieves(cr) & PER_SHADOW))
        Write("(riding something)");
      if (cr->isCreature() && (wp=((Creature*)cr)->InSlot(SL_WEAPON)) && p->Percieves(wp))
        Write(XPrint("(wielding a <Obj>)", wp));
      
      Write(0,2,"");
      if (p->Opt(OPT_SHOW_HOW_SEE) && p != cr) {
        int per = p->Percieves(cr); 
        bool first_mode = true;
        Write(0,2,XPrint("<14>Seen With:<7> "));
        for (int i=1; i<=PER_TRACK ; i<<=1) {
          if (per & i) {
            if (!first_mode)
              Write(", ");
            Write(Format("%s",Lookup(PerDescs,i)));
            first_mode = false;
            }
        
          }
        Write(". ");
        }
      if (!p->isTarget(e,cr))
        Write(XPrint("<4>[Invalid Target]<7>"));
      else if (::dist(p->x,p->y,cr->x,cr->y) > range) 
        Write(XPrint("<4>[Out of Range]<7>"));
      }
    CantSee:
    SetWin(WIN_INPUT);
    Clear(); Color(12);
    Write(0,0,Prompt);
    Color(7);

    if (mod == Q_LOC && ((m->At(tx,ty).Visibility & VI_DEFINED) ||
                          TTER(m->PTerrainAt(tx,ty,p))->HasFlag(TF_DEPOSIT)))
      Write(NAME(m->PTerrainAt(tx,ty,p)));
    else if ((mod == Q_TAR) && ((m->At(cr->x,cr->y).Visibility & VI_DEFINED) ||
                          TTER(m->PTerrainAt(cr->x,cr->y,p))->HasFlag(TF_DEPOSIT)))
      Write(NAME(m->PTerrainAt(cr->x,cr->y,p)));
    else if (mod == Q_LOC || mod == Q_TAR)
      Write(Format("unseen area"));


    if (mod == Q_DIR)  
      CursorOn();

    {
      int16 oldMode;
      oldMode = GetMode();
      SetMode(MO_DIALOG);
      ch=GetCharCmd(KY_CMD_TARGET_MODE);
      SetMode(oldMode);
    }

    CursorOff();
    Clear();

    /* Hide the cursor */
    if (mod == Q_LOC)
      m->Update(tx,ty);
    else if (mod == Q_TAR)
      m->Update(cr->x,cr->y);

    if (ch == KY_CMD_ESCAPE)
      return false;
    else if (ch == KY_CMD_WIZMODE &&
             theGame->GetPlayer(0) &&
             theGame->GetPlayer(0)->WizardMode && cr) {
      // ww: convenience feature! 
      if (!m->FirstAt(cr->x,cr->y)) break;
      SetWin(WIN_SCREEN);
      Save();
      SizeWin(WIN_CUSTOM,WinSizeX()-16,WinSizeY()-8);
      DrawBorder(WIN_CUSTOM,MAGENTA);
      ClearScroll();
      SetWin(WIN_CUSTOM); Color(YELLOW);
      m->FirstAt(cr->x,cr->y)->Dump(); 
      // (m->ThingAt(cr->x,cr->y)->GetStatiObj(MOUNTED))->Dump(); 
      int offset = 0, done = 0;
      do { 
        UpdateScrollArea(offset,WIN_CUSTOM);  
        switch(GetCharCmd(KY_CMD_ARROW_MODE)) {
          case KY_CMD_NORTH: offset--; break;
          case KY_CMD_SOUTH: offset++; break; 
          case KY_CMD_NORTHEAST:
          case KY_CMD_NORTHWEST: offset-= WinSizeY() - 6; break;
          case KY_CMD_SOUTHEAST:
          case KY_CMD_SOUTHWEST: offset+= WinSizeY() - 6; break; 
          default: done = 1; 
        }
      } while (!done); 
      ClearScroll();
      Restore();
    }
    else if (ch == KY_CMD_ALL_ALLIES && (fl & Q_ALL))
      { e.EVictim = NULL;
        e.isDir = e.isLoc = false;
        e.isAllAllies = true;
        return true; }
    else if (ch == KY_CMD_LOOK && (typ & (Q_LOC|Q_TAR)))
      { mod = Q_LOC; tx = p->x; ty = p->y; }
    else if (ch == KY_CMD_INVENTORY && (typ & Q_INV))
      goto SelectItem;  
    else if (ch == KY_CMD_TARGET && (typ & Q_TAR))
    { mod = Q_TAR; cr = p; }
    else if (ch == KY_CMD_DIRECTION && (typ & Q_DIR))
      mod = Q_DIR;
    else if (ch == KY_CMD_EXAMINE && isEngulfed)
      { if (cr) 
          if (p->Percieves(cr) & ~PER_SHADOW)
             Box(WIN_SCREEN,0,EMERALD, GREY, cr->Describe(p)); }
    else if (ch == KY_CMD_EXAMINE && cr)
      ExamineSquare(cr->x,cr->y);
    else if (ch == KY_CMD_EXAMINE && mod == Q_LOC)
      ExamineSquare(tx,ty);
    else if (ch == KY_CMD_EXAMINE_REGION && mod == Q_LOC)
      Box(WIN_SCREEN,0,PINK,GREY,DescribeResource(m->RegionAt(tx,ty)));
    else if (ch == KY_CMD_EXAMINE_REGION && mod == Q_TAR) 
      Box(WIN_SCREEN,0,PINK,GREY,DescribeResource(m->RegionAt(cr->x,cr->y)));
    else if (ch == KY_CMD_EXAMINE_FEATURE && mod == Q_LOC)
      Box(WIN_SCREEN,0,MAGENTA,GREY,DescribeResource(m->TerrainAt(tx,ty)));
    else if (ch == KY_CMD_EXAMINE_FEATURE && mod == Q_TAR)
      Box(WIN_SCREEN,0,MAGENTA,GREY,DescribeResource(m->TerrainAt(cr->x,cr->y)));
    else if (ch == KY_CMD_EXAMINE_MAP)
      Box(WIN_SCREEN,0,WHITE,GREY,m->Describe(p));
    else if (ch == KY_CMD_OVERVIEW_MAP && is_look)
    { ShowMapOverview(); return false; }
    else if (ch == KY_CMD_TARGET_SELF && (typ & Q_TAR))
    { e.ETarget = p; return true; }
    else if (ch == KY_CMD_TARGET_SELF && (typ & Q_DIR))
    { e.isDir = true; e.EDir = CENTER; return true; }    
    else if (ch == KY_CMD_HELP)
    { HelpTopic("help::interface","SP"); }
    else if (ch == KY_CMD_ENTER || ch == KY_CMD_TARGET_MOUNT)
    {
      if (mod == Q_TAR)
        { 
          Thing *cr2;
          e.EActor = p;
          
          if ((cr->isCreature() && m->MCreatureAt(cr->x,cr->y)) ||
              ((!cr->isCreature() && m->MultiAt(cr->x, cr->y))) && !cycled
                && !isEngulfed) 
            cr2 = ExamineSquare(cr->x,cr->y);
          else 
            cr2 = cr;
          
          if (!cr2)
            cr2 = cr;
            
          if (e.eID && !TEFF(e.eID)->HasFlag(EF_BLIND_PROMPT)) {
            if (!p->isTarget(e,cr2))
              continue;
            if (::dist(p->x,p->y,cr2->x,cr2->y) > range)
              continue;
            }
          e.ETarget = cr2;
          cr = cr2; 
          tx = cr->x;
          ty = cr->y;
          goto TargetChosen; 
        }
      if (mod == Q_LOC)
        { 
          if (!m->InBounds(tx,ty))
            continue;
          if (!(m->At(tx,ty).Visibility & VI_DEFINED))
            {
              Message("That square is blocked.");
              continue;
            }            
          if (!(typ & Q_LOC)) {
            if (isEngulfed) {
              if (tx == p->x && ty == p->y)
                cr = p;
              else
                cr = p->GetStatiObj(ENGULFED);
              }  
            else {
              cr = m->FirstAt(tx,ty);
              if (!cr)
                continue;

              if (m->FCreatureAt(cr->x,cr->y))
                if (p->Percieves(m->FCreatureAt(cr->x,cr->y))) 
                  cr = m->FCreatureAt(cr->x,cr->y);

              if (!p->Percieves(cr))
                continue;
              e.EActor = p;
              if (e.eID && (!TEFF(e.eID)->HasFlag(EF_BLIND_PROMPT))) {
                if (!p->isTarget(e,cr))
                  { cr = NULL; continue; }
                if (::dist(p->x,p->y,cr->x,cr->y) > range)
                  { cr = NULL; continue; }
                }
            
              }
            e.ETarget = cr;
            e.isLoc = false;
            tx = cr->x;
            ty = cr->y;
            goto TargetChosen;
            }
          if (ch == KY_CMD_TARGET_MOUNT)
            continue;
          e.isLoc = true; 
          
          TargetChosen:
          if (ch == KY_CMD_TARGET_MOUNT) 
            {
              if (e.isLoc)
                continue;
              if (!e.ETarget->HasStati(MOUNTED))
                { Message("That creature isn't mounted.");
                  continue; }
              e.ETarget = e.ETarget->GetStatiObj(MOUNTED);
            }          
          e.EXVal = tx;
          e.EYVal = ty;
          return true; 
        }
      }
    else if (ch >= KY_CMD_FIRST_ARROW && ch <= KY_CMD_LAST_ARROW) {
      cycled = false;
      switch (mod) {
        case Q_DIR:
          e.EDir = DIR_OF_KY_CMD(ch); 
          e.isDir = true;
          return true;
        case Q_LOC:
          if (::dist(p->x,p->y,tx+DirX[DIR_OF_KY_CMD(ch)],
                ty+DirY[DIR_OF_KY_CMD(ch)]) > range)
            continue;
          tx += DirX[DIR_OF_KY_CMD(ch)];
          ty += DirY[DIR_OF_KY_CMD(ch)];
          if (!m->InBounds(tx,ty))
            {
              tx -= DirX[DIR_OF_KY_CMD(ch)];
              ty -= DirY[DIR_OF_KY_CMD(ch)];
            }
          if (tx < XOff)
            tx = XOff;
          if (ty < YOff)
            ty = YOff;
          SetWin(WIN_MAP);
          if (tx >= XOff+WinSizeX())
            tx = XOff+WinSizeX()-1;
          if (ty >= YOff+WinSizeY())
            ty = YOff+WinSizeY()-1;
          break;
        case Q_TAR:
          best = -1; bestd = 1000;
          MapIterate(m,th,i)
            if (XOff < th->x && YOff < th->y)
              if (XOff+Windows[WIN_MAP].Right >= th->x && YOff+
                  (Windows[WIN_MAP].Bottom-Windows[WIN_MAP].Top) >= th->y)
                if ((p->Percieves(th) || (m->At(th->x,th->y).Visibility & VI_DEFINED)))
                  if ((th->Image & 0x00FF) != GLYPH_TREE &&
                       (th->Image & 0x00FF) != GLYPH_FLOOR2 &&
                      (::dist(p->x,p->y,th->x,th->y) <= range ||
                      /* Kludge for reach weapons */
                      (range == 2 && abs(p->x-th->x) <= 2 && abs(p->y-th->y) <= 2)))
                  {
                    if (!p->Percieves(th))
                      continue;
                    if (!(th->isCreature() || !e.eID || p->isTarget(e,th)))
                      continue;
                    /* ww: don't skip to something unless it's a valid
                     * target! 
                     * fjm: let's make this more intuitive for the player,
                     * so they /realize/ it is an invalid target.
                     *
                    if (e.eID && !p->isTarget(e,th))
                      continue;
                     */
                    if ((fl & Q_CRE) && !th->isCreature())
                      continue; 

                    dist = 0;
                    if (DirX[DIR_OF_KY_CMD(ch)] == 1)
                    {
                      if (th->x <= cr->x)
                        continue;
                      dist += (th->x - cr->x);
                    }
                    if (DirX[DIR_OF_KY_CMD(ch)] == -1)
                    {
                      if (th->x >= cr->x)
                        continue;
                      dist += -(th->x - cr->x);
                    }
                    if (DirY[DIR_OF_KY_CMD(ch)] == 1)
                    {
                      if (th->y <= cr->y)
                        continue;
                      dist += (th->y - cr->y);
                    }
                    if (DirY[DIR_OF_KY_CMD(ch)] == -1)
                    {
                      if (th->y >= cr->y)
                        continue;
                      dist += -(th->y - cr->y);
                    }

                    if (dist < bestd)
                    { best = i; bestd = dist; }
                  }
          if (best != -1)
          {
            t = best; 
            cr = oThing(m->Things[t]);
            if (m->FCreatureAt(cr->x,cr->y))
              cr = m->FCreatureAt(cr->x,cr->y);
          }
        }
      }    
    else if (ch == KY_CMD_NEXT) {
      /* 'n' command -- skip to the next eligable target in the 
         same location as the current target. */
      if (!cr)
        continue;
      cycled = true;
      th = m->FirstAt(cr->x,cr->y);
      while (th != cr)
        th = m->NextAt(cr->x,cr->y);
      
      do
        th = m->NextAt(cr->x, cr->y);
      while (th && ! /*p->isTarget(e,th)*/ 1);
      if (!th) {
        th = m->FirstAt(cr->x, cr->y);
        /*
        while (!p->isTarget(e,th));
          th = m->NextAt(cr->x, cr->y);
        */
        }
      cr = th;
      }
  }
  // ww: paranoia
  return false; 
}



Thing* TextTerm::ChooseTarget(const char* Prompt, int16 rType)
  {
    EventInfo e; e.Clear();
    return EffectPrompt(e,Q_TAR|Q_CRE,false,Prompt) ? e.ETarget : NULL;
  }

Thing * TextTerm::ExamineSquare(int x, int y)
{
  Thing * retvals[1024]; 

  int c = 0, i; 

  Thing * t = oThing(m->At(x,y).Contents);
  while (t) {
    if (t->isCreature()) {
      if (p->Percieves(t) & ~PER_SHADOW) {
        retvals[c] = t;
        LOption(Format("%s",(const char*)t->Name()),c++,t->Describe(p),0);
        }
    } 
    t = oThing(t->Next);
  }
  t = oThing(m->At(x,y).Contents);
  while (t) {
    if (t->isItem()) {
      if (p->Percieves(t) & ~PER_SHADOW) {
        retvals[c] = t; 
        LOption(Format("%s",(const char*)t->Name()),c++,((Item *)t)->DescribeWithTitle(p),1);
        }
    } 
    t = oThing(t->Next);
  }
  t = oThing(m->At(x,y).Contents);
  while (t) {
    if (t->isFeature()) {
      if (t->Type == T_TRAP && ! (((Trap *)t)->TrapFlags & TS_FOUND))
        ;
      else if (t->Type == T_DOOR && (((Door *)t)->DoorFlags & DF_SECRET))
        ;
      else {
        if (p->Percieves(t) & ~PER_SHADOW) {
          retvals[c] = t;
          LOption(Format("%s",(const char*)t->Name()),c++,t->Describe(p),2);
          }
      }
    } 
    t = oThing(t->Next);
  }

  Field * f; 
  for (i=0;f = m->Fields[i];i++) {
    if (!(f->FType & FI_SIZE) && p->PercievesField(x,y,f)) {
      retvals[c] = NULL;
      LOption(Format("%s",NAME(f->eID)),c++,DESC(f->eID),3);
    } 
  } 

  retvals[c] = NULL;
  Player *p = theGame->GetPlayer(0);
  LOption(Format("%s",NAME(m->PTerrainAt(x,y,p))),c++,
      DescribeResource(m->TerrainAt(x,y)),4);
      
  if (m->PTerrainAt(x,y,NULL) != m->PTerrainAt(x,y,p))
    LOption(Format("%s",NAME(m->PTerrainAt(x,y,NULL))),c++,
      DescribeResource(m->PTerrainAt(x,y,NULL)),4);

  retvals[c] = NULL;
  LOption(Format("%s",NAME(m->RegionAt(x,y))),c++,
      DescribeResource(m->RegionAt(x,y)),5);

  retvals[c] = NULL;
  LOption(Format("%s",NAME(m->dID)),c++,
      DescribeResource(m->dID),6);

  int32 res = T1->LMenu(MENU_DESC|MENU_ESC|MENU_SORTED|MENU_LARGEBOX|MENU_BORDER,
      "-- Examine --",WIN_MENUBOX);
  if (res < 0 || res > 1024)
    return NULL;
  else 
    return retvals[res]; 
} 


/*****************************************************************************\
*                                  TEXTTERM                                  *
*                               Other Prompts                                *
\*****************************************************************************/

char TextTerm::ChoicePrompt(const char*msg,const char*choices,int8 col1, int8 col2, bool
                             preprompt)
  {
    String str; int16 ch; 
    if (Mode == MO_RECREATE)
      {
        ASSERT(strncmp(RInf.Rsp[RInf.cRsp].Question,msg,31) == 0);
        return RInf.Rsp[RInf.cRsp++].Answer;
      }
    str = Format("<%d>%s [<%d>%s<%d>] ",col1,msg,col2,choices,col1);
    Redraw:
    SetWin(WIN_INPUT); Clear(); 
    if (preprompt)
      PrePrompt();
    Write(0,0,XPrint(str));
    CursorOn();
    do
      ch = tolower(GetCharRaw());
    while (ch != KY_ESC && !strchr(choices,ch) && ch != KY_REDRAW);
    if (ch == KY_REDRAW)
      goto Redraw;
    CursorOff();
    Clear();
  
    if (Mode == MO_CREATE)
      {
        strncpy(RInf.Rsp[RInf.nRsp].Question,msg,31);
        RInf.Rsp[RInf.nRsp].Answer = ch;
        RInf.nRsp++;
      }
    if (ch == KY_ESC)
      return -1;
    return ch;
  }     
int32 TextTerm::MonsterTypePrompt(const char * prompt, int minCount, int maxCount)
{
  int i; String desc;
  for (i=1; i<=MA_LAST_REAL; i++) {
    desc = "";
    int count_in_this_ma = 0; 
    for (int modIdx = 0; modIdx < 1; modIdx++) {
      Module *mod = theGame->Modules[modIdx];
      for (int idx = 0; idx < mod->szMon; idx++) {
        rID mID = mod->MonsterID(idx); 
        if (!(mod->QMon[idx].isMType(mID,i)))
          continue;
        if (mod->QMon[idx].HasFlag(M_NOGEN))
          continue;

        int8 color = - (mod->QMon[idx].Image >> 8);
        if (color == 0) color = -SHADOW;
        desc += Format("%c%s (%c%c%c CR %d). ",-7,
            (const char*)Capitalize(Upto(NAME(mID),";"),true),
            color,
            (mod->QMon[idx].Image & 0xff),
            -7,
            mod->QMon[idx].CR);
        count_in_this_ma++;
      }
    } 
    if (count_in_this_ma >= minCount && count_in_this_ma <= maxCount
           && LookupOnly(MTypeNames,i)) {
      LOption((const char*)Format("%s (%d)",
          (const char*)Pluralize(Lookup(MTypeNames,i)),
          count_in_this_ma),i,desc);
    }
  } 
  return LMenu(MENU_ESC|MENU_SORTED|MENU_2COLS|MENU_DESC,prompt,
      WIN_SCREEN);
} 


rID TextTerm::MonsterOfTypePrompt(int32 type, const char * prompt) 
{
  int count = 0; 
  rID mID = 0; 
  for (int modIdx = 0; modIdx < 1; modIdx++) {
    Module *mod = theGame->Modules[modIdx];
    for (int idx = 0; idx < mod->szMon; idx++) {
      rID mID = mod->MonsterID(idx); 
      if (!(mod->QMon[idx].isMType(mID,type)))
        continue;
      if (mod->QMon[idx].HasFlag(M_NOGEN))
        continue;
      mID = mod->MonsterID(idx); 
      Monster * m = new Monster(mID);


      m->StateFlags |= MS_KNOWN;
      LOption(m->Name(),mID,m->Describe(NULL));

      delete m; 

      count++;
    }
  } 
  if (!count) return 0; 
  mID = LMenu(MENU_ESC|MENU_DESC|MENU_3COLS|MENU_SORTED,
      prompt, WIN_SCREEN, 0); 
  if (mID == 0xFFFFFFFF) return 0;
  else return mID; 
} 


rID TextTerm::MonsterPrompt(const char * prompt)
{
  int matype;
  rID res;
  res = 0;
  do {
    matype = MonsterTypePrompt(prompt);
    if (matype <= 0) return 0;
    res = MonsterOfTypePrompt(matype, prompt);
  } while (res <= 0); 
  return res; 
} 
Thing* TextTerm::AcquisitionPrompt(int8 Reason, int8 minlev, int8 maxlev, int8 MType)
  {                            
    int16 BlankGlyphs[4];
    int16 cp,ocp,n,i,j,k; const int16 *Glyphs, *GOffset; 
    rID xID; Item *it; Monster *mn; String str;
    
    PrePrompt();
    
    Redraw:
    
    SetWin(WIN_INPUT);
    Glyphs = (Reason >= ACQ_MONSTER) ?  ( MType == MA_ANIMAL ? 
      WildShapeGlyphs : MonsterGlyphs ) : (Reason == ACQ_MAJOR ||
      Reason == ACQ_MINOR || Reason == ACQ_CRAFT) ? CreationGlyphs : ItemGlyphs;
    GOffset = Glyphs;
    cp = 0; ocp = -1;

    if (Glyphs == ItemGlyphs && MType > 0)
      {
        for (i=0;ItemGlyphs[i];i += 2)
          if (ItemGlyphs[i+1] == -MType)
            {
              BlankGlyphs[0] = ItemGlyphs[i];
              BlankGlyphs[1] = ItemGlyphs[i+1];
              BlankGlyphs[2] = 0;
              BlankGlyphs[3] = 0;
              Glyphs = BlankGlyphs;
              GOffset = BlankGlyphs;
              break;
            }
      }

    do {
      Clear(); Color(10);
      switch(Reason) {
        case ACQ_ANY_ITEM: 
        case ACQ_ACQUIRE: Write("Acquisition -- ["); break;
        case ACQ_IDENT_ALL: Write("Identify -- ["); break;
        case ACQ_MINOR: Write("Minor Creation -- ["); break;
        case ACQ_MAJOR: Write("Major Creation -- ["); break;
        case ACQ_ANYMON:
        case ACQ_SUMMON: Write("Summon -- ["); break;
        case ACQ_POLYMORPH: Write("Polymorph -- ["); break;
        case ACQ_GENOCIDE: Write("Genocide -- ["); break;   
        case ACQ_RECALL: Write("Recall -- ["); break; 
        case ACQ_CRAFT:   Write("Craft Item -- ["); break;  
        case ACQ_KNOWN:   Write("Create Item -- ["); break;
        }
      for(i=0;GOffset[i*2] && i < GLYPHS_SHOWN;i++) {
        Color(cp - ((GOffset - Glyphs)/2) == i ? 15 : 2);
        PutChar(GOffset[i*2]);
        }
      Color(10); Write("]: "); Color(7);
      if (Reason >= ACQ_MONSTER) {
        if (LookupOnly(MTypeNames,Glyphs[cp*2+1]))
          Write(Pluralize(Lookup(MTypeNames,Glyphs[cp*2+1])));
        }
      else if (Glyphs[cp*2+1] < 0)
        { Write(Lookup(SourceNames,-Glyphs[cp*2+1]));  }
      else
        { Write(Lookup(ITypeNames,Glyphs[cp*2+1]));  }

      if (cp != ocp) {
        n = xID = 0; ocp = cp;
        memset(Candidates,0,sizeof(rID)*2047);
        if (strchr("ahxBCKORX",Glyphs[cp*2]) || Glyphs[cp*2] == GLYPH_LDEMON ||
             Glyphs[cp*2] == GLYPH_LDEVIL || Glyphs[cp*2] == GLYPH_GDEMON ||
             Glyphs[cp*2] == GLYPH_GDEVIL)
          k = Glyphs[cp*2] + 100000;
        else
          k = Glyphs[cp*2+1];
        
        if (Glyphs[cp*2+1] > 0) {
        
          switch(Reason) {
            case ACQ_ANY_ITEM: 
              if (k == T_TRAP)
                { 
                  xID = FIND("trap");          
                  theGame->GetEffectID(PUR_ACQUIRE + PUR_LISTING,minlev,maxlev,AI_TRAP);
                }
              else
                theGame->GetItemID(-1,minlev,maxlev,k); 
             break;
            case ACQ_ACQUIRE: 
              theGame->GetItemID(PUR_ACQUIRE + PUR_LISTING,minlev,maxlev,k); break;
            case ACQ_MINOR: 
              theGame->GetItemID(PUR_MINOR + PUR_LISTING,minlev,maxlev,k); break;
            case ACQ_MAJOR: 
              theGame->GetItemID(PUR_MAJOR + PUR_LISTING,minlev,maxlev,k); break;
            case ACQ_CRAFT: 
              theGame->GetItemID(PUR_CRAFT + PUR_LISTING,minlev,maxlev,k); break;
            case ACQ_IDENT_ALL: 
              Candidates[0] = 0; break;
            case ACQ_RECALL:
            case ACQ_ANYMON:
              theGame->GetMonID(PUR_ANYMON + PUR_LISTING,minlev,maxlev,maxlev,k); 
             break;
            case ACQ_SUMMON: 
              theGame->GetMonID(PUR_SUMMON + PUR_LISTING,minlev,maxlev,maxlev,k); break;
            case ACQ_POLYMORPH: 
              theGame->GetMonID(PUR_POLY + PUR_LISTING,minlev,maxlev,maxlev,k); break;
            case ACQ_GENOCIDE:  
              theGame->GetMonID(PUR_SUMMON + PUR_LISTING,minlev,maxlev,maxlev,k); break;
            }
          if (Candidates[0] && MType && RES(Candidates[0])->Type == T_TMONSTER)
            for (i=0;Candidates[i];i++)
              if (!TMON(Candidates[i])->isMType(Candidates[i],MType))
                {
                  memmove(Candidates+i,Candidates+i+1,2047-i);
                  i--;
                }
          }
        else 
          {
            

          for (i=0;DungeonItems[i].Type;i++)
            if (Glyphs[cp*2+1] == -DungeonItems[i].Source) {
              if (DungeonItems[i].Prototype)
                { xID = FIND(DungeonItems[i].Prototype); goto FoundProto; }
              if (DungeonItems[i].Source == AI_WEAPON)
                { xID = FIND("short sword"); goto FoundProto; }
              if (DungeonItems[i].Source == AI_ARMOR)
                { xID = FIND("chainmail"); goto FoundProto; }
              if (DungeonItems[i].Source == AI_SHIELD)
                { xID = FIND("kite shield"); goto FoundProto; }
              }
          xID = 0;

          FoundProto:
          switch(Reason)
            {
              case ACQ_CRAFT: 
                theGame->GetEffectID(PUR_MUNDANE,minlev,maxlev,-Glyphs[cp*2+1]);
               break;
              case ACQ_KNOWN: 
                theGame->GetEffectID(PUR_KNOWN,minlev,maxlev,-Glyphs[cp*2+1]);
               break;
              case ACQ_MAJOR: 
                theGame->GetEffectID(PUR_MAJOR,minlev,maxlev,-Glyphs[cp*2+1]);
               break;
              case ACQ_MINOR: 
                theGame->GetEffectID(PUR_MINOR,minlev,maxlev,-Glyphs[cp*2+1]);
               break;
              default:
                theGame->GetEffectID(PUR_ACQUIRE,minlev,maxlev,-Glyphs[cp*2+1]);
               break;
            }
          if (!Candidates[0] && !Glyphs[2])
            {
              SetWin(WIN_INPUT);
              Clear();
              Message("You don't know any suitable formulas.");
              return NULL;
            }  
                
               
          }
        }
      SetWin(WIN_MESSAGE); Clear(); GotoXY(0,0);
      j = WinSizeY()/2;
      for(i=0;i!=WinSizeY();i++)
        {
          Color(i == j ? AZURE : BLUE);
          if (n - j + i < 0)
            continue;
          if (!Candidates[n-j+i])
            continue;
            
          if (xID)
            str = ItemNameFromEffect(Candidates[n-j+i]);
          else
            str = NAME(Candidates[n-j+i]);
          /*
          if (xID && Candidates[n-j+i] && TEFF(Candidates[n-j+i])->HasFlag(EF_NAMEONLY)) 
            str = Format("%s",NAME(Candidates[n-j+i]));
          else if (TEFF(Candidates[n-j+i])->GetConst(BASE_ITEM))
            str = Format("%s of %s",NAME(TEFF(Candidates[n-j+i])->GetConst(BASE_ITEM)),NAME(Candidates[n-j+i]));
          else if (xID && Candidates[n-j+i]) 
            str = Format("%s of %s",NAME(xID),NAME(Candidates[n-j+i]));
          else 
            str = Format("%s",NAME(Candidates[n-j+i]));*/
          str.SetAt(0,toupper(str[0]));
          Write(0,i,str);
        }
      SetWin(WIN_INPUT);

      PurgeStrings();  
      switch(GetCharCmd(KY_CMD_ARROW_MODE))
        {
          case KY_REDRAW:
            goto Redraw;
          case KY_CMD_WEST:
            if (cp)
              cp--;
            if (cp*2 + Glyphs < GOffset)
              GOffset = Glyphs + cp*2;
           break;
          case KY_CMD_EAST:
            if (Glyphs[cp*2+2])
              cp++;
            if (cp - ((GOffset - Glyphs)/2) > GLYPHS_SHOWN-1)
              GOffset = (Glyphs + cp*2 + 2) - GLYPHS_SHOWN*2;
           break;
          case KY_CMD_NORTH:
            if (n)
              n--;
           break;
          case KY_CMD_SOUTH:
            if (Candidates[n+1])
              n++;
           break;
          case KY_ENTER: 
          case KY_CMD_ENTER: 
            if (!Candidates[n])
              break;
            SetWin(WIN_MESSAGE); Clear();
            SetWin(WIN_INPUT); Clear();
            if (Reason >= ACQ_MONSTER) {
              mn = new Monster(Candidates[n]);
              if (mn->isMType(MA_DRAGON))
                goto DragonAgeCat;
              return mn; 
              }
            if (xID) {
              if (xID == FIND("trap")) {
                it = (Item*) new Trap(xID,Candidates[n]);
                return it;
                }
              else {
                it = Item::Create(xID);
                /*
                if (TEFF(Candidates[n])->HasFlag(EF_NEEDS_PLUS))
                  it->MakeMagical(Candidates[n],1+random(5));
                else
                  it->MakeMagical(Candidates[n],1+random(5));
                  */
                if (TEFF(Candidates[n])->ef.aval == AR_POISON) {
                  it->GainPermStati(POISONED,NULL,SS_MISC,0,10,Candidates[n]);
                  it->eID = 0;
                  }
                else if (TEFF(Candidates[n])->HasFlag(EF_NEEDS_PLUS))
                  it->MakeMagical(Candidates[n],MaxItemPlus(maxlev,Candidates[n]));
                else
                  it->MakeMagical(Candidates[n],0);
                it->Initialize();                
                }
              }
            else
              it = Item::Create(Candidates[n]);
            it->SetKnown(0xFF);
            if (it->HasIFlag(IT_BGROUP))
              it->SetQuantity(100);
            else if (it->HasIFlag(IT_GROUP) || it->HasIFlag(IT_ROPE))
              it->SetQuantity(12);          
            return it;
          default: 
            SetWin(WIN_INPUT); Clear();
            SetWin(WIN_MESSAGE); Clear();
            Clear();
            return NULL;
        }
      }
    while(1);
    DragonAgeCat:
    /* Assumption: all age catagories are in Module 0. */
    for(i=0;i!=theGame->Modules[0]->szTem;i++)
      if (theGame->Modules[0]->QTem[i].TType & TM_AGECAT)
        if (theGame->Modules[0]->QTem[i].CR.Adjust(mn->ChallengeRating()) <= maxlev)
          LOption(NAME(theGame->Modules[0]->TemplateID(i)),theGame->Modules[0]->TemplateID(i));
    xID = LMenu(MENU_BORDER,"Choose an age catagory:",WIN_MENUBOX);
    mn->AddTemplate(xID);
    return mn;
  } 

String & TextTerm::StringPrompt(int8 col, const char*msg)
  {
    SetWin(WIN_INPUT);
    Clear(); PrePrompt(); Color(col);
    Write(msg); Color(7);
    ReadLine(cx+1,cy,0); 
    SetWin(WIN_INPUT);
    Clear();
    return *tmpstr(Input);
  }

const char* TextTerm::ReadLine(int16 x,int16 y,int16 first_ch)
	{
    int16 loc, ch;
    SetWin(WIN_SCREEN);
    Write(x,y,"                                                                        ");
    if (first_ch)
      { Input[0] = first_ch; loc = 1;
        PutChar(x,y,Input[0]);
      }
    else
      loc = 0;
    Input[loc] = 0;
    CursorOn();
    GotoXY(x+loc,y);
    do {
      ch = GetCharRaw();
      if (ch == KY_BACKSPACE) {
        if (loc == 0)
          continue;
        loc--;
        Input[loc] = 0;
        }
      else if (ch == KY_ENTER) {
        Input[loc] = 0;
        CursorOff();
        Write(x,y,"                                                                              ");
        return (const char*) &(Input[0]);
        }
      else if (isprint(ch) || ch==' ' || ch=='.' || ch=='_' || ch=='\'')
        Input[loc++] = ch;
      if (loc)
        PutChar((x+loc)-1,y,Input[loc-1]);
      PutChar(x+loc,y,' ');
      GotoXY(x+loc,y);
    }
  while(1);
  }


rID TextTerm::ChooseResource(const char*prompt, int16 RType, rID eID)
  {
    rID xID; int16 i,m; EventInfo e;

    LOptionClear();

    #define MAKE_LIST(ResourceID,szRes)                    \
    for(m=0;theGame->Modules[m];m++)                       \
      for(i=0;i!=theGame->Modules[0]->szRes;i++)           \
        {                                                  \
          xID = theGame->Modules[0]->ResourceID(i);        \
          if (eID) {                                       \
            e.Clear();                                     \
            e.eID = xID;                                   \
            e.Event = EV_CRITERIA;                         \
            if (TEFF(eID)->Event(e,eID) == 0)              \
              continue;                                    \
            }                                              \
          LOption(NAME(xID),xID);                          \
        }   
              
    if (RType == T_TEFFECT) 
      MAKE_LIST(EffectID,szEff)
    else if (RType == T_TITEM) 
      MAKE_LIST(ItemID,szItm)
    else if (RType == T_TFEATURE) 
      MAKE_LIST(FeatureID,szFea)
          
    xID = LMenu(MENU_3COLS|MENU_SORTED|MENU_ESC|MENU_BORDER,prompt);
    
    return xID; 
    
  }


bool TextTerm::yn(const char*msg)
	{
  	int16 ch='a';
    if (Mode == MO_RECREATE)
      {
        ASSERT(strncmp(RInf.Rsp[RInf.cRsp].Question,msg,31) == 0);
        return RInf.Rsp[RInf.cRsp++].Answer;
      }
    Update();
    PrePrompt();
    Redraw:
    /*
    fjm: Changed so that the cursor appears in the right place.
    SetWin(WIN_MESSAGE); Clear();
    WrapWrite(0,0,Format("%c%s [%cyn%c] ",-5,msg,-13,-5)); */
    SetWin(WIN_INPUT); GotoXY(0,0); Clear();
    Write(Format("%c%s [%cyn%c] ",-5,msg,-13,-5));    
    CursorOn();
    while (ch!='y' && ch!='n' && ch != KY_REDRAW)
      ch=tolower(GetCharRaw());
    if (ch == KY_REDRAW)
      { ch = 0; goto Redraw; }
    CursorOff();
    Clear();

    if (Mode == MO_CREATE)
      {
        strncpy(RInf.Rsp[RInf.nRsp].Question,msg,31);
        RInf.Rsp[RInf.nRsp].Answer = (ch=='y' ? true : false);
        RInf.nRsp++;
      }
    
		
		return ch=='y' ? true : false;
	}

/*****************************************************************************\
*                                  TEXTTERM                                  *
*                                Splash Screens                              *
\*****************************************************************************/

const char* IntroScreen[] = {
/*01234567890123456789012345678901234567890123456789012345678901234567890123456789*/
 "                                                                               ", 
 "                                                                               ", 
 "                <9>*********************************************<7>                    ", 
 "                <9>*********************************************<7>                    ", 
 "                <9>**                                         **<1>*<7>                  ", 
 "                <9>**                <15>INCURSION<9>                **<1>*<7>                  ", 
 "                <9>**                                         **<1>*<7>                  ", 
 "                <9>**        <13>Halls of the Goblin King<9>         **<1>*<7>                  ", 
 "                <9>**                                         **<1>*<7>                  ", 
 "                <9>*********************************************<1>*<7>                  ", 
 "                <9>*********************************************<1>*<7>                  ", 
 "                  <1>********************************************<7>                  ", 
 "                                                                               ", 
 "                                                                               ", 
 "                               By Julian Mensch                                ", 
 "          with additional concepts and material by Westley Weimer              ",
 "                                                                               ", 
 "                      Copyright 1999-2007 Julian Mensch                        ", 
 "                                                                               ", 
 "                          See Help for License Info                            ", 
 "                                                                               ", 
 "                                 <14>Version " VERSION_STRING "<7>                                ", 
 "                                                                               ", 
 "                                                                               ", 
 "                                                                               ", 
 "     <15>With Inspiration and Derivative Source Material drawn from:<7>               ", 
 "       * <13>The Mines of Moria<7>, by Robert Alan Koenkhe                            ", 
 "       * <13>The Pits of Angband<7>, maint. Robert Ruhlmann                           ", 
 "           and its many great variant games                                   ", 
 "       * <13>Nethack<7>, by the Nethack DevTeam and many others                       ", 
 "       * <13>Ancient Domains of Mystery<7>, by Thomas Biskup                          ",
 "       * <13>Linley's Dungeon Crawl<7>, by Linley Henzell                             ", 
 "       * <13>Omega<7>, by Laurence Brothers                                           ", 
 "       * and the materials placed under the Open Gaming License by             ",
 "            Wizards of the Coast and others                                    ", 
 "                                                                               ", 
 "     Created using the ACCENT Compiler-Compiler system.                        ", 
 "     Incorporates the source code for the Decus CPP public-domain              ", 
 "       C preprocessor.                                                         ", 
 "                                                                               ", 
 NULL };

void TextTerm::Title()
  {
    int16 i, ox, oy;
    
    ox = (sizeX - 80) / 2;
    oy = max(0,sizeY - 50) / 2;
    
    SetWin(WIN_SCREEN); Clear();
    for(i=0;IntroScreen[i];i++)
      Write(ox,oy+i,XPrint(IntroScreen[i]));
    SizeWin(WIN_CUSTOM,WinLeft(),min(i+2,WinBottom()-8),WinRight(),WinBottom());
    SetWin(WIN_CUSTOM);
    Clear();
  }



void TextTerm::DisplayLegend()
  {
    String gl;
    int8 x1,x2,y1,y2;
    Save();
    SetWin(WIN_SCREEN);
    x1 = x2 = WinSizeX()/2;
    y1 = y2 = WinSizeY()/2;
    x1 -= 34; x2 += 34;
    y1 -= 16; y2 += 17;
    SizeWin(WIN_MENUBOX,x1,y1,x2,y2);
    DrawBorder(WIN_MENUBOX,PINK);
    SetWin(WIN_MENUBOX);
    Color(GREY); Clear();
    gl = XPrint(GlyphLegend1);
    gl += XPrint(GlyphLegend2);
    Write(0,0,gl);
    PrePrompt();
    GetCharRaw();
    Restore();
  }

void TextTerm::DisplayKnownItems()
  {
    String list; rID eID, endID; 
    EffMem *em; int16 i, q;
    Item *it;
    
    list.Empty();

    for(i=0;DungeonItems[i].Type;i++) {
      if (!DungeonItems[i].Prototype)
        continue;
      if (!FIND(DungeonItems[i].Prototype))
        continue;
      it = Item::Create(FIND(DungeonItems[i].Prototype));
      for (q=0;q!=MAX_MODULES;q++)
        if (theGame->Modules[q]) {
          eID = theGame->Modules[q]->EffectID(0);
          endID = eID + theGame->Modules[q]->szEff;
          for(;eID!=endID;eID++)
            if (TEFF(eID)->HasSource(DungeonItems[i].Source))
              {
                if (DungeonItems[i].Source == AI_TOME)
                  continue;
                em = EFFMEM(eID,p);
                if (em->PKnown && (DungeonItems[i].Source == AI_POTION))
                  {
                    it->MakeMagical(eID);
                    it->SetKnown(0);
                    em->PKnown = false;
                    if (it->isPlural())
                      list += Trim(it->Name().Capitalize());
                    else
                      list += Trim(Pluralize(it->Name()).Capitalize());
                    em->PKnown = true;
                    list += " are ";
                    it->SetKnown(KN_MAGIC);
                    if (it->isPlural())
                      list += it->Name();
                    else
                      list += Pluralize(it->Name());
                    list += ".\n";
                  }
                if (em->Known && (DungeonItems[i].Source != AI_POTION))
                  {
                    it->MakeMagical(eID);
                    it->SetKnown(0);
                    em->Known = false;
                    if (it->isPlural())
                      list += Trim(it->Name().Capitalize());
                    else
                      list += Trim(Pluralize(it->Name()).Capitalize());
                    em->Known = true;
                    list += " are ";
                    it->SetKnown(KN_MAGIC);
                    if (it->isPlural())
                      list += it->Name();
                    else
                      list += Pluralize(it->Name());
                    list += ".\n";
                  }
                PurgeStrings();
              }
          }
      delete it;
      }
  
    list.Replace("bootses", "boots");
    list.Replace("Bootses", "Boots");
    list.Replace("lenseses", "lenses");
    list.Replace("Lenseses", "Lenses");
    list.Replace("gauntletses", "gauntlets");
    list.Replace("Gauntletses", "Gauntlets");
    list.Replace("gloveses", "gloves");
    list.Replace("Gloveses", "Gloves");
    SetWin(WIN_SCREEN);
    Save();
    SizeWin(WIN_CUSTOM,WinSizeX()-16,WinSizeY()-8);
    DrawBorder(WIN_CUSTOM,MAGENTA);
    ClearScroll();
    SetWin(WIN_CUSTOM); Color(YELLOW);
    SWrite((WinSizeX()/2)-18,0," -- Item Enchantment Discoveries -- ");
    Color(GREY);
    if (list.GetLength() == 0)
      list = "None Yet.";
    SWrapWrite(1,1,list,WinSizeX()-2,WIN_CUSTOM);
    Color(GREY);
    UpdateScrollArea(0,WIN_CUSTOM);  
    do {
      Color(GREY);
      UpdateScrollArea(offset,WIN_CUSTOM);
      switch(GetCharRaw()) {
        case 200 + NORTH:
          UpdateScrollArea(offset-1,WIN_CUSTOM);
         break;
        case 200 + SOUTH:
          UpdateScrollArea(offset+1,WIN_CUSTOM);
         break;
        case 200 + NORTHEAST:
          UpdateScrollArea(offset-(WinSizeY()-6),WIN_CUSTOM);
         break;
        case 200 + SOUTHEAST:
          UpdateScrollArea(offset+(WinSizeY()-6),WIN_CUSTOM);
         break;
        case KY_ESC:
        case KY_ENTER:
          ClearScroll();
          Restore();
          return;
        }
      }
    while(1);
  }    


