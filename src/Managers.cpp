/* MANAGERS.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Includes the higher-level GUI functions for a TextTerm,
   including the Character Sheet and Managers (Spell Manager,
   Skill Manager, Inventory Manager, Barter Manager, Option
   Manager).

     int16 TextTerm::SpellManager(int16 Purpose)
     void TextTerm::InvShowSlots()
     int16 TextTerm::InventoryManager(Container *_theChest)
     void TextTerm::BarterManager(Creature *seller)
     void TextTerm::SkillManager()
     void TextTerm::OptionManager(int16 start_cat)
     

*/

#include "Incursion.h"

/*****************************************************************************\
*                                 TEXTTERM                                   *
*                             Various Managers                               *
\*****************************************************************************/

int16 TextTerm::SpellManager(int16 Purpose) {
    String FeatList, FeatKeys; EventInfo e; String sch, the_name, desc; int16 Key, abKey;
    static const char * SaveNames[] = { "Fort", "Ref ", "Will", "----" };
    static const char * WinTitle = " == Spell Manager == ";
    int16 NumSpells, Line,i,j,k,z,sp,curr,offset, mult, mana, mm_lev, oldMode;
    char ch; int16 Spells[1024];
    NumSpells = 0;
    rID autobuffID = FIND("Autobuff");
    abKey = 0;
    for(i=0;i!=MAX_MACROS;i++)
        if (p->Macros[i] == autobuffID)
            abKey = KY_CMD_MACRO1 + i;

    oldMode = Mode;
    SetMode(MO_INV);

    String spell_line[1024]; 

    for(i=0;i!=theGame->LastSpell();i++)
        if (p->SpellRating(theGame->SpellID(i),0,true) != -1)
            NumSpells++;
    if(!NumSpells)
    {
        SetMode(oldMode);
        p->IPrint("You do not know any spells.");
        return -1;
    }
Redraw:
    Save();
    SizeWin(WIN_SPELLS,-1,5,-1,5+min(11 + NumSpells,(sizeY*39)/50));
    SetWin(WIN_SPELLS); 
    SizeWin(WIN_CUSTOM,2,WinBottom()-8,WinSizeX()-2,WinBottom()-3);

    FeatKeys = "";
    FeatList = "";
    if (p->HasFeat(FT_AMPLIFY_SPELL))
    { FeatKeys += "A"; FeatList += "<15>A<9>mplify/"; }
    if (p->HasFeat(FT_AUGMENT_SUMMONING))
    { FeatKeys += "G"; FeatList += "Au<15>g<9>ment/"; }
    if (p->HasFeat(FT_ANCHOR_SPELL))
    { FeatKeys += "Z"; FeatList += "Anchor/"; }
    if (p->HasFeat(FT_BIND_SPELL))
    { FeatKeys += "B"; FeatList += "<15>B<9>ind/"; }
    if (p->HasFeat(FT_CONTROL_SPELL))
    { FeatKeys += "C"; FeatList += "<15>C<9>ontrol/"; }
    if (p->HasFeat(FT_CONSECRATE_SPELL))
    { FeatKeys += "N"; FeatList += "Co<15>n<9>secrate/"; }
    if (p->HasFeat(FT_DEFENSIVE_SPELL))
    { FeatKeys += "D"; FeatList += "<15>D<9>efensive/"; }
    if (p->HasFeat(FT_JUDICIOUS_SPELL))
    { FeatKeys += "J"; FeatList += "<15>J<9>udicious/"; }
    if (p->HasFeat(FT_EMPOWER_SPELL))
    { FeatKeys += "E"; FeatList += "<15>E<9>mpower/"; }
    if (p->HasFeat(FT_ENLARGE_SPELL))
    { FeatKeys += "L"; FeatList += "En<15>l<9>arge/"; }
    if (p->HasFeat(FT_EXTEND_SPELL))
    { FeatKeys += "X"; FeatList += "E<15>x<9>tend/"; }
    if (p->HasFeat(FT_FOCUS_SPELL))
    { FeatKeys += "O"; FeatList += "F<15>o<9>cus/"; }
    if (p->HasFeat(FT_FORTIFY_SPELL))
    { FeatKeys += "F"; FeatList += "<15>F<9>ortify/"; }
    if (p->HasFeat(FT_HEIGHTEN_SPELL))
    { FeatKeys += "H"; FeatList += "<15>H<9>eighten/"; }
    if (p->HasFeat(FT_INHERANT_SPELL))
    { FeatKeys += "I"; FeatList += "<15>I<9>nherant/"; }
    if (p->HasFeat(FT_MAXIMIZE_SPELL))
    { FeatKeys += "M"; FeatList += "<15>M<9>aximize/"; }
    if (p->HasFeat(FT_PROJECT_SPELL))
    { FeatKeys += "P"; FeatList += "<15>P<9>roject/"; }
    if (p->HasFeat(FT_QUICKEN_SPELL))
    { FeatKeys += "Q"; FeatList += "<15>Q<9>uicken/"; }
    if (p->HasFeat(FT_REPEAT_SPELL))
    { FeatKeys += "R"; FeatList += "<15>R<9>epeat/"; }
    if (p->HasFeat(FT_SURE_SPELL))
    { FeatKeys += "U"; FeatList += "S<15>u<9>re/"; }
    if (p->HasFeat(FT_STILL_SPELL))
    { FeatKeys += "S"; FeatList += "<15>S<9>till/"; }
    if (p->HasFeat(FT_TRANSMUTE_SPELL))
    { FeatKeys += "T"; FeatList += "<15>T<9>ransmute/"; }
    if (p->HasFeat(FT_VOCALIZE_SPELL))
    { FeatKeys += "V"; FeatList += "<15>V<9>ocalize/"; }
    if (p->HasFeat(FT_VILE_SPELL))
    { FeatKeys += "Y"; FeatList += "Vile/"; }
    if (p->HasFeat(FT_WARP_SPELL))
    { FeatKeys += "W"; FeatList += "<15>W<9>arp/"; }
    if (FeatList) {
        FeatList = FeatList.Left(FeatList.GetLength()-1);
        FeatList = XPrint(Format("[%s] %s Spell",
            (const char*)FeatKeys, (const char*)FeatList));
        if (FeatList.GetTrueLength() > WinSizeX())
            FeatList = FeatList.TrueLeft(WinSizeX());
    }


    curr = 0; offset = 0;

ProcessSpells:
    Line = 0; ch = 'a';
    e.Clear(); e.EActor = p;
    for (int desiredLev = 9; desiredLev >= 0; desiredLev--)
        for (i=0;i!=theGame->LastSpell();i++)
            if (TEFF(theGame->SpellID(i))->Level == desiredLev)
                if ((j = p->SpellRating(theGame->SpellID(i),p->MMFeats(i),true)) != -1) {
                    e.eID = theGame->SpellID(i);
                    // ww: no meta-magic for innate abilities
                    if (p->Spells[i] & SP_KNOWN) 
                        e.MM = p->MMFeats(i);
                    if (Purpose & SM_TRICKERY)
                        e.isArcaneTrickery = true;
                    e.EActor->CalcEffect(e);
                    Spells[Line] = i;
                    Color(GREY);
                    mana = p->getSpellMana(e.eID,e.MM,NULL);
                    /*
                    switch(TEFF(theGame->SpellID(i))->Schools)
                    {
                    case SC_ABJ: sch = "Abj"; break;
                    case SC_ARC: sch = "Arc"; break;
                    case SC_DIV: sch = "Div"; break;
                    case SC_ENC: sch = "Enc"; break;
                    case SC_EVO: sch = "Evo"; break;
                    case SC_ILL: sch = "Ill"; break;
                    case SC_NEC: sch = "Nec"; break;
                    case SC_THA: sch = "Tha"; break;
                    case SC_WEA: sch = "Wea"; break;
                    case SC_THE: sch = "The"; break;
                    default: sch = " * " ; break; 
                    }
                    */
                    Key = 0;
                    for(k=0;k!=MAX_QKEYS;k++) {
                        if (p->QuickKeys[k].Type == QKY_SPELL)
                            if (p->QuickKeys[k].Value == i)
                                Key = '0' + k;
                    }
                    // Write(0,1,"  Key Spell                  Mana Time Succ CLev Save DC Power & Feats");
                    String myline;
                    String save_string;
                    int KY_color = Key ? -14 : -7; 
                    int name_color = ((p->Spells[i] & SP_DOMAIN) ? -5 : -7);
                    int time_cost = 3000 / max(25,100+10*(1 + e.EActor->Mod(A_INT) -
                        TEFF(e.eID)->Level));


                    if (p->Spells[i] & SP_INNATE) {
                        name_color = -6; 
                        time_cost = 20; 
                    } 
                    if (p->Spells[i] & SP_STAFF) {
                        name_color = (p->Spells[i] & (~SP_STAFF)) ? -10 : -2;
                    }

                    for (z=0;p->AutoBuffs[z] && z!=63;z++)
                        if (p->AutoBuffs[z] == i)
                            name_color = -SKYBLUE;

                    if (p->HasMM(MM_QUICKEN,i)) time_cost /= 2; 

                    int time_color = (time_cost <= 20 ? -GREEN :
                        (time_cost >= 30 ? -RED : -YELLOW));

                    int succ_color = (j == 100 ? -GREEN :
                        (j >= 90 ? -YELLOW : -RED));
                    int clev_color = (e.vCasterLev > e.EActor->CasterLev() ? -GREEN :
                        (e.vCasterLev < e.EActor->CasterLev() ? -RED : -YELLOW));

                    int total_mana = e.EActor->tMana();
                    int cur_mana = e.EActor->cMana();
                    int N = ((total_mana - cur_mana)+mana) * 20 / total_mana;
                    int ManaPulse = N * 100 / total_mana;
                    int mana_color = (ManaPulse == 0 ? -GREEN :
                        (ManaPulse == 1 ? -YELLOW : -RED));

                    int savedc = 10 + TEFF(e.eID)->Level;
                    if ((p->Spells[i] & (SP_ARCANE|SP_INNATE))) 
                        savedc = max(savedc,10+TEFF(e.eID)->Level+e.EActor->Mod(A_INT));
                    if ((p->Spells[i] & (SP_DIVINE|SP_INNATE))) 
                        savedc = max(savedc,10+TEFF(e.eID)->Level+e.EActor->Mod(A_WIS));
                    if ((p->Spells[i] & (SP_PRIMAL|SP_INNATE))) 
                        savedc = max(savedc,10+TEFF(e.eID)->Level+e.EActor->Mod(A_WIS));
                    if ((p->Spells[i] & (SP_SORCERY|SP_INNATE))) 
                        savedc = max(savedc,10+TEFF(e.eID)->Level+e.EActor->Mod(A_CHA));


                    int save_color = (e.saveDC > savedc ? -GREEN :
                        (e.saveDC < savedc ? -RED : -YELLOW));

                    if (TEFF(e.eID)->ef.sval == NOSAVE) {
                        save_string = "       ";
                    } else {
                        save_string = Format("%4s %2d",
                            SaveNames[min(3,TEFF(theGame->SpellID(i))->ef.sval % 10)],
                            e.saveDC);
                    } 

                    the_name = NAME(theGame->SpellID(i));

                    /* Yes, we want == here -- the fatigue cost does not 
                    apply if you know the spell AND get it from the
                    staff. But you pay fatigue for spells you don't
                    know. */
                    if (p->Spells[i] == SP_STAFF) {
                        int16 fat = p->GetStaffFatigueCost(theGame->SpellID(i));
                        if (fat)
                            the_name += Format(" {%d}",fat);
                    }

                    the_name = the_name.Left(22); 
                    if (TEFF(e.eID)->HasFlag(EF_VARMANA))
                        myline = Format("[%c%c%c] %c%-22s%c    %cV%c %c%4d%c %c%3d~%c %c%4d%c %c%.7s%c %s",
                        KY_color, Key ? Key : (ch <= 'z' ? ch : ' '), -7,

                        name_color, (const char*)the_name, -7,

                        -AZURE, -7,

                        time_color, time_cost, -7, 

                        succ_color, j, -7,

                        clev_color, e.vCasterLev, -7,

                        save_color, (const char*)save_string, -7,

                        (const char*)(TEFF(theGame->SpellID(i))->Power(0,p,theGame->SpellID(i)))
                        );
                    else
                        myline = Format("[%c%c%c] %c%-22s%c %c%4d%c %c%4d%c %c%3d~%c %c%4d%c %c%.7s%c %s",
                        KY_color, Key ? Key : (ch <= 'z' ? ch : ' '), -7,

                        name_color, (const char*)the_name, -7,

                        mana_color, mana, -7,

                        time_color, time_cost, -7, 

                        succ_color, j, -7,

                        clev_color, e.vCasterLev, -7,

                        save_color, (const char*)save_string, -7,

                        (const char*)(TEFF(theGame->SpellID(i))->Power(0,p,theGame->SpellID(i)))
                        );
                    myline += Format("%c",-EMERALD);
                    if (p->HasMM(MM_ANCHOR,i)) myline +=  " An";/* Amplify Spell */
                    if (p->HasMM(MM_AMPLIFY,i)) myline +=  " Am";/* Amplify Spell */
                    if (p->HasMM(MM_AUGMENT,i)) myline +=  " Au";/* Augment Summoning */
                    if (p->HasMM(MM_BIND,i)) myline +=  " Bi";/* Bind Spell */
                    if (p->HasMM(MM_CONSECRATE,i)) myline +=  " Cons";/* Consecrate Spell */
                    if (p->HasMM(MM_CONTROL,i)) myline +=  " Cont ";/* Control Spell */
                    if (p->HasMM(MM_DEFENSIVE,i)) myline +=  " De";/* Defensive Spell */
                    if (p->HasMM(MM_DEFILING,i)) myline +=  " Dl";/* Defiling Spell */
                    if (p->HasMM(MM_EMPOWER,i)) myline +=  " Em";/* Empower Spell */
                    if (p->HasMM(MM_ENLARGE,i)) myline +=  " En";/* Enlarge Spell */
                    if (p->HasMM(MM_EXTEND,i)) myline +=  " Ex";/* Persistant Spell */
                    if (p->HasMM(MM_FOCUS,i)) myline +=  " Fc";/* Focused Spell */
                    if (p->HasMM(MM_FORTIFY,i)) myline +=  " Fr";/* Fortify Spell */
                    if (p->HasMM(MM_HEIGHTEN,i)) myline +=  " He";/* Heighten Spell */
                    if (p->HasMM(MM_INHERANT,i)) myline +=  " In";/* Inherant Spell */
                    if (p->HasMM(MM_JUDICIOUS,i)) myline +=  " Ju";/* Judicious Spell */
                    if (p->HasMM(MM_MAXIMIZE,i)) myline +=  " Ma";/* Maximize Spell */
                    if (p->HasMM(MM_PROJECT,i)) myline +=  " Pr";/* Project Spell */
                    if (p->HasMM(MM_QUICKEN,i)) myline +=  " Qu";/* Quicken Spell */
                    if (p->HasMM(MM_REPEAT,i)) myline +=  " Re";/* Repeat Spell */
                    if (p->HasMM(MM_STILL,i)) myline +=  " St";/* Still Spell */
                    if (p->HasMM(MM_SURE,i)) myline +=  " Su";/* Sure Spell */
                    if (p->HasMM(MM_TRANSMUTE,i)) myline +=  " Tr";/* Transmute Spell */
                    if (p->HasMM(MM_UNSEEN,i)) myline +=  " Un";/* Unseen Spell */
                    if (p->HasMM(MM_VILE,i)) myline +=  " Vi";/* Vile Spell */
                    if (p->HasMM(MM_VOCALIZE,i)) myline +=  " Vo";/* Vocalize Spell */
                    if (p->HasMM(MM_WARP,i)) myline +=  " Wa";/* Warp Spell */
                    myline += Format("%c",-GREY);

                    spell_line[Line] = myline;
                    Line++;
                    // _snprintf(Text[Line],160,"%s%c",(const char*)myline,0);
                    if (ch <= 'z')
                        ch++;
                }

    Line++;
    do {
        Clear(); Color(EMERALD);
        for(i=0;i!=WinSizeX();i++) {
            PutChar(i,0,205);
            PutChar(i,WinSizeY()-1,205);
        }
        Color(YELLOW);
        PutChar(0,0,219);
        PutChar(0,WinSizeY()-1,219);
        PutChar(WinSizeX()-1,0,219);
        PutChar(WinSizeX()-1,WinSizeY()-1,219);
        Write(WinLeft() + (WinSizeX() / 2) -
            (strlen(WinTitle)/2),0,WinTitle);

        Color(WHITE);
        Write(0,1,"  Key Spell                  Mana Time Succ CLev Save DC Power & Metamagic");
        Color(GREY);
        for(i=2;i!=WinSizeY()-10;i++)
            // Write(2,i,Text[i + offset - 2]);
            Write(2,i,(const char*)spell_line[i + offset - 2]);
        Color(EMERALD);
        PutChar(0,(curr-offset)+2,16);
        PutChar(79,(curr-offset)+2,17);
        Color(BLUE);

        ClearScroll();
        if (DESC(theGame->SpellID(Spells[curr]))) {
            SGotoXY(0,0);
            desc = XPrint(TEFF(theGame->SpellID(Spells[curr]))->Describe(p));
            if (theGame->Opt(OPT_MONO_HELP)) {
                int32 p;
                for (p=0;desc[p];p++)
                    if (desc[p] <= -1 && desc[p] >= -8)
                        desc.SetAt(p,-7);
            }
            SWrapWrite(0,0,desc,WinSizeX()-2);
        }
        UpdateScrollArea(0);
        SetWin(WIN_SPELLS);

        Color(AZURE);
        if(FeatList)
            Write(0,WinSizeY()-3,FeatList);
        Write(0,WinSizeY()-2,Format("[%c%c] Select Spell [ENTER] Cast %c[0-9] Set Spellkey%c [?] Help",
            GLYPH_ARROW_UP,GLYPH_ARROW_DOWN,-8,-9)); 
GetKey:
        ClearKeyBuff();
        switch(sp = GetCharRaw()) {
        case KY_REDRAW:
            goto Redraw;
        case '?':
            Restore();
            HelpTopic("help::magic","SM");
            goto Redraw;
        case KY_ESC:
            Restore();
            SetMode(oldMode);
            return -1;
        case 200 + NORTH:
            if (curr <= 0)
                break;
            curr--;
            if (((curr-offset) < 0) && offset)
                offset--;
            break;
        case 200 + SOUTH:
            if (curr >= NumSpells-1)
                break;
            curr++;
            if ((curr-offset) > (WinSizeY()-13))
                offset++;
            break;
        case 200 + NORTHEAST: ScrollUp();   SetWin(WIN_SPELLS); goto GetKey;
        case 200 + SOUTHEAST: ScrollDown(); SetWin(WIN_SPELLS); goto GetKey;
        case KY_ENTER:
            Restore();
            SetMode(oldMode);
            return Spells[curr];
            break;
        case 'A': p->ToggleMM(MM_AMPLIFY,Spells[curr]); goto ProcessSpells;
        case 'B': p->ToggleMM(MM_BIND,Spells[curr]); goto ProcessSpells;
        case 'C': p->ToggleMM(MM_CONTROL,Spells[curr]); goto ProcessSpells;      
        case 'D': p->ToggleMM(MM_DEFENSIVE,Spells[curr]); goto ProcessSpells;
        case 'E': p->ToggleMM(MM_EMPOWER,Spells[curr]); goto ProcessSpells;
        case 'F': p->ToggleMM(MM_FORTIFY,Spells[curr]); goto ProcessSpells;
        case 'G': p->ToggleMM(MM_AUGMENT,Spells[curr]); goto ProcessSpells;    
        case 'H': p->ToggleMM(MM_HEIGHTEN,Spells[curr]); goto ProcessSpells;
        case 'I': p->ToggleMM(MM_INHERANT,Spells[curr]); goto ProcessSpells;
        case 'J': p->ToggleMM(MM_JUDICIOUS,Spells[curr]); goto ProcessSpells;
        case 'L': p->ToggleMM(MM_ENLARGE,Spells[curr]); goto ProcessSpells;          
        case 'M': p->ToggleMM(MM_MAXIMIZE,Spells[curr]); goto ProcessSpells;
        case 'N': p->ToggleMM(MM_CONSECRATE,Spells[curr]); goto ProcessSpells;
        case 'O': p->ToggleMM(MM_FOCUS,Spells[curr]); goto ProcessSpells;
        case 'P': p->ToggleMM(MM_PROJECT,Spells[curr]); goto ProcessSpells;
        case 'Q': p->ToggleMM(MM_QUICKEN,Spells[curr]); goto ProcessSpells;
        case 'R': p->ToggleMM(MM_REPEAT,Spells[curr]); goto ProcessSpells;
        case 'S': p->ToggleMM(MM_STILL,Spells[curr]); goto ProcessSpells;
        case 'T': p->ToggleMM(MM_TRANSMUTE,Spells[curr]); goto ProcessSpells;
        //case 'U': p->ToggleMM(MM_UNSEEN,Spells[curr]); goto ProcessSpells;
        case 'U': p->ToggleMM(MM_SURE,Spells[curr]); goto ProcessSpells;
        case 'V': p->ToggleMM(MM_VOCALIZE,Spells[curr]); goto ProcessSpells;
        case 'Y': p->ToggleMM(MM_VILE,Spells[curr]); goto ProcessSpells;
        case 'W': p->ToggleMM(MM_WARP,Spells[curr]); goto ProcessSpells;
        case 'X': p->ToggleMM(MM_EXTEND,Spells[curr]); goto ProcessSpells;      
        default:
            if (sp == abKey) {
                if (TEFF(theGame->SpellID(Spells[curr]))->Vals(0)->qval &&
                    !(TEFF(theGame->SpellID(Spells[curr]))->Vals(0)->qval & Q_TAR))
                    goto DoneAB;
                for (z=0;p->AutoBuffs[z] && z!=63;z++)
                    if (p->AutoBuffs[z] == Spells[curr]) {
                        memmove(&p->AutoBuffs[z],&p->AutoBuffs[z+1],
                            sizeof(p->AutoBuffs[0]) * (63 - z));
                        p->AutoBuffs[63] = 0; 
                        //p->IPrint("<Res> removed from autobuff list.",
                        //          theGame->SpellID(Spells[curr]));
                        goto DoneAB;
                    }       
                for (z=0;z!=63;z++)
                    if (!p->AutoBuffs[z]) {
                        p->AutoBuffs[z] = Spells[curr];
                        //p->IPrint("<Res> added to autobuff list.",
                        //          theGame->SpellID(Spells[curr]));
                        goto DoneAB;  
                    }
                    //p->IPrint("No room left in autobuff list.");
DoneAB:
                goto ProcessSpells;
            }

            if (sp >= '0' && sp <= '9') {
                p->QuickKeys[sp-'0'].Value = Spells[curr];
                p->QuickKeys[sp-'0'].Type = QKY_SPELL;
                goto ProcessSpells;
            }
            if (sp >= 'a' && sp <= min('z',ch-1)) {
                Restore();
                SetMode(oldMode);
                return Spells[sp - 'a'];
            }
            break;
        }
    }
    while(1);
}


const char * SlotLetters =
  "abcefghijklmnopqrtuvwyz";

int16 itemGroups[] = 
  { T_WEAPON, T_BOW, T_MISSILE, T_ARMOUR, T_SHIELD,
    T_POTION, T_SCROLL, T_WAND, T_TOOL, T_COIN, T_FOOD, T_ITEM, 0 };

inline int16 sortOrder(Item *it)
  {
    int16 i;
    for (i=0;itemGroups[i];i++)
      if (it->isType(itemGroups[i]))
        return i;
    return 50;
  }

int sortItemsGrouped(const void *a, const void *b)
  {
    Item *A = *(Item**)a,
         *B = *(Item**)b;
    int16 oA = sortOrder(A),
          oB = sortOrder(B);
    if (oA != oB)
      return oA - oB;
    return A->myHandle > B->myHandle;
    //return stricmp(A->Name(NA_LONG|NA_MECH),
    //               B->Name(NA_LONG|NA_MECH));
  }

int sortItemsAlpha(const void *a, const void *b)
  {
    Item *A = *(Item**)a,
         *B = *(Item**)b;
    return A->myHandle > B->myHandle;
    //return stricmp(A->Name(NA_LONG|NA_MECH),
    //               B->Name(NA_LONG|NA_MECH));
  }

void TextTerm::InvShowSlots(bool changed)
	{
    Container *con;
		int16 i,j,k,y;
		String str, wgt;
		bool psych_might = p->HasAbility(CA_PSYCH_MIGHT),
		     isGrouped = theGame->Opt(OPT_ORG_CONTAIN);
    SetWin(WIN_INVEN); 
    if (changed)
      Clear(); 
    Color(GREY);
    if(NUM_SLOTS>WinSizeY()-3)
			Fatal("Too many inv. slots for the window!");
			
		for(i=0;i!=NUM_SLOTS && changed;i++)
			{  
        if(p->Inv[i])
          {
            k = oItem(p->Inv[i])->Weight(psych_might);
            wgt = Format("%d.%ds", k / 10, k % 10);
					  str = oItem(p->Inv[i])->Name(NA_LONG|NA_MECH);
          }
				else {
          wgt.Empty();
					str = "Empty";
          }
        
        if (i)
          str = Format("%c)%c%s%c :%s",SlotLetters[i-1],TMON(p->mID)->HasSlot(i) ? -7 : -1,
            SlotNames[i],-7,(const char*)str);
        else
          str = SC("'In the Air':") + str;




        if (str.GetLength()+2 > WinSizeX()-wgt.GetLength())
          str = str.Left((WinSizeX()-5)-wgt.GetLength()) + 
                  Format("...%c",-7);

				Write(2,0+i,str);
        if (wgt)
          Write(WinSizeX()-wgt.GetLength(),0+i,wgt);
        PurgeStrings();


			}
     
    if (changed) {
      j = 0;
      SizeWin(WIN_CUSTOM,WinLeft(),WinTop()+1+NUM_SLOTS,WinRight(),WinBottom()-2);
      ClearScroll();
      }
    
    if (currCon && changed)
      {
    
        Color(PURPLE); 
        con = (Container*)(p->Inv[i]);
        SGotoXY(0,0);
        if (currCon == theChest)
          SWrite(Format("In the %s:\n",(const char*)currCon->Name(NA_INSC|NA_MONO)));        
        else
          SWrite(Format("In your %s:\n",(const char*)currCon->Name(NA_INSC|NA_MONO)));        
        Color(GREY);
        memset(Contents,0,sizeof(Item*)*256);
        memset(yContents,0,sizeof(int16)*256);
        if (currCon->HasStati(LOCKED))
          SWrite("  It is locked.\n");
        else if (!(*currCon)[0])
          SWrite("  Nothing\n");
        else {
          for(j=0;(*currCon)[j];j++) {
            p->LegendIdent((*currCon)[j]);
            Contents[j] = (*currCon)[j];
            }
          Contents[j] = NULL;
            
          if (isGrouped)
            qsort(Contents,j,sizeof(Item*),sortItemsGrouped);
          else
            qsort(Contents,j,sizeof(Item*),sortItemsAlpha);
          PurgeStrings();
          
          for(j=0,y=1;Contents[j];j++,y++) {
            if (isGrouped)
              if (j == 0 || (sortOrder(Contents[j]) != sortOrder(Contents[j-1])))
                {
                  y++;
                  Color(YELLOW);
                  if (itemGroups[sortOrder(Contents[j])] == T_ITEM)
                    SWrite(0,y,"Other Items");
                  else if (itemGroups[sortOrder(Contents[j])] == T_ARMOUR)
                    SWrite(0,y,"Suits of Armour");
                  else
                    SWrite(0,y,Lookup(ITypeNames,itemGroups[sortOrder(Contents[j])]));
                  Color(GREY);
                  y++;
                }
            str = Format("  %02d) %s\n", j+1,
                (const char*)Contents[j]->Name(NA_LONG|NA_MECH));

            k = Contents[j]->Weight(psych_might);
            wgt = Format("%d.%ds", k / 10, k % 10);
            if (str.GetLength() > WinSizeX() - wgt.GetLength())
              str = str.Left((WinSizeX()-3)-wgt.GetLength()) + Format("...%c",-7);
            SWrite(0,y,str);          
            SWrite(WinSizeX()-wgt.GetLength(),y,wgt);
            yContents[j] = y;
            if (j == 98)
              goto StopListing;
          }
        }
      }
    StopListing:

    if (!changed)
	    {
	      for (i=0;i!=NUM_SLOTS;i++)
	        Write(0,i,"  ");
	      if (currCon)
	        for(i=0;(*currCon)[i];i++)
	          SWrite(0,yContents[i],"  ");
	    }

		if(CurrSlot)
			{
        Color(PINK);
        
        if (CurrSlot >= NUM_SLOTS) {
          if (!Contents[CurrSlot - NUM_SLOTS])
            CurrSlot = 0;
          else
            SWrite(0,yContents[CurrSlot-NUM_SLOTS],">>"); 
          }
        else
				  Write(0,0+CurrSlot,">>");
			}

    UpdateScrollArea(offset,WIN_CUSTOM);
    SetWin(WIN_INVEN);             
		GotoXY(0,WinSizeY()-(1 + (currCon ? 1 : 0))); Color(PINK);
		Write(Format("[%c/%c:Up/Down a-%c:Slot SPACE:Swap x:Examine ESC:Done 'd':Drop%c\n",
			24,25,'a'+NUM_SLOTS-1, currCon ? ' ' : ']'));
    if (currCon) {
      for(j=0;Contents[j];j++)
        ;
      Write(Format(" PGUP/PGDN: Scroll TAB: Next Cont. 01-%02d:%s s:Stow ALT:Some]",j,
              isGrouped ? "Select" : "Take Out"));
      }
    
      
}

int16 TextTerm::InventoryManager(bool first, Container *_theChest) {
    uint8 ch; int8 cs, ocs;
    int8 sl, digit = 0, num;
    Item *i; bool changed;
    bool conCursor = theGame->Opt(OPT_CONTAINER_CURSOR);

#define INV(sl) (sl < NUM_SLOTS ? p->Inv[sl] : Contents[sl-NUM_SLOTS]->myHandle)

    if (first) {
        theChest = _theChest;
        CurrSlot=1;
        offset = 0;
    }

    /* Player gets Bull Rushed while looting a chest */
    if (theChest)
        if (theChest->x != p->x ||
            theChest->y != p->y ||
            theChest->m != p->m)
            theChest = NULL;

    if (p->isDead()) {
        Item *it;
        if (it = p->InSlot(SL_INAIR))
        {
            it->Remove(false);
            p->Inv[SL_INAIR] = 0;
            if (p->m && p->x != -1)
                it->PlaceAt(p->m,p->x,p->y);
            else
                it->Remove(true);
        }
        if (Mode == MO_INV)
            SetMode(MO_PLAY);
        return 0;
    }

    if (Mode == MO_INV)
        ;
    else if (Mode != MO_PLAY && Mode != MO_CREATE) {
        SetWin(WIN_SCREEN);
        Clear();
        SetMode(MO_INV);
        ShowTraits();
        ShowStatus();
    } else
        SetMode(MO_INV);

    /* ww: automatically move the currently slot point to the first place
    * that cold hold our up-in-the-air item */
    if (p->Inv[SL_INAIR] && first) {
        Item *inair = oItem(p->Inv[SL_INAIR]);
        int match_slot;
        for (CurrSlot=SL_LAST-1; 
            CurrSlot > 1 && 
            ((!inair->allowedSlot(CurrSlot,p)) || INV(CurrSlot)) ;
        CurrSlot--)
            ;;
    }

ResetCurrCon:
    if (theChest) {
        currCon = theChest;
        cs = -1;
    } else {
        currCon = NULL; cs = SL_PACK;
        if ((TMON(p->mID)->HasSlot(SL_PACK) ||
            p->AbilityLevel(CA_WILD_SHAPE)) && p->Inv[SL_PACK])
            if (oItem(p->Inv[SL_PACK])->Type == T_CONTAIN)
                currCon = (Container*)(oItem(p->Inv[SL_PACK]));
        if (currCon == NULL) {
            for (cs=0;cs!=SL_LAST;cs++)
                if (p->Inv[cs] && oItem(p->Inv[cs])->isType(T_CONTAIN) &&
                    (p->InSlot(cs) || p->AbilityLevel(CA_WILD_SHAPE)))
                {
                    currCon = (Container*)oItem(p->Inv[cs]);
                    break;
                }
        }
        if (cs == SL_LAST)
            cs = SL_PACK;
    }
    SetMode(MO_INV);
    changed = true;
    while(1) {
        if (p->Timeout)
            return 0;
        if (p->isThreatened())
            p->IPrint("You are threatened.");
        InvShowSlots(changed);
        p->CalcValues();
        ClearKeyBuff();
        ch=GetCharRaw();
        PurgeStrings();
        if (!isdigit(ch))
            digit = 0;
        switch(ch) {
        case 200 + NORTHEAST:
            if (currCon)
                UpdateScrollArea(offset - (1+p->Opt(OPT_SCROLL_MAG)));
            break;
        case 200 + SOUTHEAST:
            if (currCon)
                UpdateScrollArea(offset + (1+p->Opt(OPT_SCROLL_MAG)));
            break;
        case KY_TAB:  
            changed = true;
            if (cs > SL_PACK)
                cs = 0;
            ocs = cs;
            do {
                cs++;
                if (cs == -1) {
                    if (theChest) { 
                        currCon = theChest;
                        break;
                    }
                } 
                else if (p->Inv[cs] && oItem(p->Inv[cs])->isType(T_CONTAIN) && (p->InSlot(cs))) {
                    currCon = (Container*)oItem(p->Inv[cs]);
                    if (CurrSlot >= NUM_SLOTS)
                        CurrSlot = 1;
                    break;
                }
                if (cs >= SL_LAST)
                    cs = -2;
            } while(cs != ocs);
            break;
        case KY_HELP:
            HelpTopic("help::interface","IM");
            break;
        case KY_ENTER: 
        case KY_ESC: 
            if(p->Inv[SL_INAIR]) {
                p->IPrint("You cannot leave Inventory Mode with an item "
                    "in your In-the-Air slot.");
                break;
            }
            changed = true;
            SetWin(WIN_INVEN);
            Clear();
            SetMode(MO_PLAY);
            RefreshMap();
            ShowTraits();
            ShowStatus();
            return 0;
        case 'x':
            if (INV(CurrSlot)) {
                p->LegendIdent(oItem(INV(CurrSlot)));
                Box(WIN_SCREEN,0,EMERALD,GREY,oItem(INV(CurrSlot))->DescribeWithTitle(p));
            }
            else if (p->Inv[SL_INAIR]) {
                p->LegendIdent(oItem(p->Inv[SL_INAIR]));
                Box(WIN_SCREEN,0,EMERALD,GREY,oItem(p->Inv[SL_INAIR])->DescribeWithTitle(p));                
            }
            break;
        case 'e':
            if (ControlKeys & CONTROL) { 
                if (CurrSlot && INV(CurrSlot)) {
                    hObj v1 = INV(CurrSlot);
                    Item *i1 = oItem(v1);
                    if (i1) {
                        p->AddJournalEntry(Format("Found: %s",
                            (const char*)i1->Name(NA_LONG)));
                        Message("Journal entry added.");
                    }
                }
            } else
                goto default_label;
            break;
        case 'd':
            changed = true;
            if (p->Inv[SL_INAIR]) {
                Item *it;
                it = oItem(p->Inv[SL_INAIR]);
                if (!it)
                    break;
                if (it->GetQuantity() > 1 && AltDown())
                    it = OneSomeAll(it,p);
                if (p->Opt(OPT_WARN_DROP))
                    if (!yn(XPrint("Confirm drop <Str>? ", (const char*)
                        Decolorize(oItem(p->Inv[SL_INAIR])->Name(NA_THE|NA_SINGLE)))))
                        break;
                // ww: I don't know how this happens, but at this point the
                // object has no parent! this causes the game to crash later
                // because it won't be removed from Inv[SL_INAIR] even though it
                // will be deleted
                it->SetOwner(p->myHandle);          
                Throw(EV_DROP,p,NULL,it);
                if (p->Inv[SL_INAIR])
                    if (oItem(p->Inv[SL_INAIR])->Owner() != p)
                        p->Inv[SL_INAIR] = 0;
                break;
            }
            if (CurrSlot)
                if (INV(CurrSlot)) {
                    Item *it = oItem(INV(CurrSlot)), *it2;
                    if (it->GetQuantity() > 1 && AltDown() && p->Inv[SL_INAIR]) {
                        p->IPrint("You need your in-air slot free to divide a slotted pile.");
                        break;
                    }
                    if (p->Opt(OPT_WARN_DROP))
                        if (!yn(XPrint("Confirm drop <Str>? ", (const char*)
                            Decolorize(oItem(INV(CurrSlot))->Name(NA_THE)))))
                            break;
                    // ww: without this remove, dropping an equipped ring leaves
                    // with you with the benefits! so dropping an equipped ring of
                    // polymorph means you will still randomly morph later!
                    if (CurrSlot < NUM_SLOTS) { 
                        if (ThrowVal(EV_REMOVE,CurrSlot,p,NULL,it,NULL) == ABORT)
                            break;
                    } else {
                        if (Throw(EV_TAKEOUT,p,NULL,currCon,it) != DONE)
                            break; 
                    }
                    if (it->GetQuantity() > 1 && AltDown()) {
                        it2 = OneSomeAll(it,p);
                        if (it2 != it)
                            p->Inv[SL_INAIR] = it->myHandle;
                    } else
                        it2 = it;  
                    Throw(EV_DROP,p,NULL,it2);
                    if (it != it2 && CurrSlot < SL_LAST && !p->Inv[CurrSlot])
                        p->Swap(CurrSlot);
                    else if (it != it2 && CurrSlot >= SL_LAST) {
                        if (Throw(EV_INSERT,p,NULL,currCon,it)==DONE)
                            if (it->myHandle == p->Inv[SL_INAIR])
                                p->Inv[SL_INAIR] = 0;              
                    }
                }
                break;
        case 's':
            if (!currCon)
                Message("Nothing to stow in.");
            else if (CurrSlot >= NUM_SLOTS && !p->Inv[SL_INAIR])
                Message("Item already stowed.");
            else if (p->Inv[SL_INAIR]) {
                changed = true;
                Item *it = oItem(p->Inv[SL_INAIR]);
                if (it->GetQuantity() > 1 && AltDown())
                    it = OneSomeAll(it,p);
                if (Throw(EV_INSERT,p,NULL,currCon,it)==DONE)
                    if (it->myHandle == p->Inv[SL_INAIR])
                        p->Inv[SL_INAIR] = 0;
            } else if (p->Inv[CurrSlot]) {
                if (oItem(p->Inv[CurrSlot]) == currCon)
                    break;
                changed = true;
                Item *it = oItem(p->Inv[CurrSlot]), *it2; 
                if (ThrowVal(EV_REMOVE,CurrSlot,p,NULL,it,NULL) == ABORT)
                    break; 
                if (it->GetQuantity() > 1 && AltDown())
                    it2 = OneSomeAll(it,p);
                else
                    it2 = it;
                if (it2 != it)
                    p->Inv[CurrSlot] = it->myHandle;

                if (Throw(EV_INSERT,p,NULL,currCon,it2)==DONE) {
                    if (p->Inv[CurrSlot] == it2->myHandle)
                        p->Inv[CurrSlot] = 0;
                } else if (p->Inv[CurrSlot]) {
                    if (!it->TryStack(it2))
                        it2->PlaceAt(p->m,p->x,p->y);
                } else
                    p->Inv[CurrSlot] = it2->myHandle;
            } else
                Message("Nothing to stow.");
            break;
        case KY_SPACE:
            if(!CurrSlot || CurrSlot==SL_INAIR)
                break;
            changed = true;
            if (AltDown() && p->Inv[SL_INAIR] && INV(CurrSlot)) {
                p->IPrint("You cannot swap two items and divide a pile "
                    "at the same time.");
                break;
            }
            if (CurrSlot >= NUM_SLOTS) {
                Item *it = Contents[CurrSlot - NUM_SLOTS];
                if (it && AltDown() && it->GetQuantity() > 1)
                    it = OneSomeAll(it,p);
                if (p->Inv[SL_INAIR]) {
                    if (Throw(EV_INSERT,p,NULL,currCon,oItem(p->Inv[SL_INAIR]))==DONE)
                        p->Inv[SL_INAIR] = 0;
                    else
                        break;
                }
                if (Throw(EV_TAKEOUT,p,NULL,currCon,it)==DONE)
                    p->Inv[SL_INAIR] = it->myHandle;    
                else if (it != Contents[CurrSlot - NUM_SLOTS])
                    if (!Contents[CurrSlot - NUM_SLOTS]->TryStack(it))
                        it->PlaceAt(p->m,p->x, p->y);  
            } else    
                p->Swap(CurrSlot);
            InvShowSlots(true);
            changed = false;
            p->CalcValues();
            Update();
            PrePrompt();
            break;
        case 200+SOUTH:
            CurrSlot++;
            if(CurrSlot>=NUM_SLOTS) {
                if (conCursor && currCon && yContents[CurrSlot-NUM_SLOTS])
                    ;
                else
                    CurrSlot=1;
            }
            if (currCon && conCursor && CurrSlot >= NUM_SLOTS) {
                int16 o_off = offset;
                SetWin(WIN_INVEN);
                if (offset > yContents[CurrSlot - NUM_SLOTS])
                    offset = max(0,yContents[CurrSlot - NUM_SLOTS]-4);
                else if (offset + (WinSizeY()-(NUM_SLOTS)) < yContents[CurrSlot - NUM_SLOTS]+4)
                    offset = max(0,(yContents[CurrSlot - NUM_SLOTS]+4) - (WinSizeY()-(NUM_SLOTS)));
                if (offset != o_off)
                    UpdateScrollArea(offset);
            }
            break;
        case 200+NORTH:
            CurrSlot--;
            if(CurrSlot<=0) {
                if (conCursor && currCon && Contents[0]) {
                    CurrSlot = NUM_SLOTS;
                    while (yContents[(CurrSlot-NUM_SLOTS)+1])
                        CurrSlot++;
                } else
                    CurrSlot=NUM_SLOTS-1;
            }
            if (currCon && conCursor && CurrSlot >= NUM_SLOTS) {
                int16 o_off = offset;
                SetWin(WIN_INVEN);
                if (offset > yContents[CurrSlot - NUM_SLOTS])
                    offset = max(0,yContents[CurrSlot - NUM_SLOTS] - 4);
                else if (offset + (WinSizeY()-(NUM_SLOTS)) < yContents[CurrSlot - NUM_SLOTS])
                    offset = max(0,(yContents[CurrSlot - NUM_SLOTS]+4) - (WinSizeY()-(NUM_SLOTS)));
                if (offset != o_off)
                    UpdateScrollArea(offset);
            }
            else if (currCon && conCursor && CurrSlot == SL_PACK)
                UpdateScrollArea(offset = 0);        
            break;
        case 'M':
            changed = true;
            if (oItem(INV(CurrSlot)))
                if (oItem(INV(CurrSlot))->isType(T_WEAPON)) {
                    if (INV(CurrSlot) == p->defMelee)
                        p->defMelee = 0;
                    else
                        p->defMelee = INV(CurrSlot);
                    if (INV(CurrSlot) == p->defOffhand)
                        p->defOffhand = 0;
                    if (INV(CurrSlot) == p->defAmmo)
                        p->defAmmo = 0;
                }
                break;
        case 'O':
            changed = true;
            if (oItem(INV(CurrSlot)))
                if (oItem(INV(CurrSlot))->isType(T_WEAPON) || oItem(INV(CurrSlot))->isType(T_SHIELD) ) {
                    if (INV(CurrSlot) == p->defOffhand)
                        p->defOffhand = 0;
                    else
                        p->defOffhand = INV(CurrSlot);
                    if (INV(CurrSlot) == p->defMelee)
                        p->defMelee = 0;
                    if (INV(CurrSlot) == p->defAmmo)
                        p->defAmmo = 0;
                }
                break;
        case 'R':
            changed = true;
            if (oItem(INV(CurrSlot)))
                if (oItem(INV(CurrSlot))->isType(T_BOW)) {
                    if (INV(CurrSlot) == p->defRanged)
                        p->defRanged = 0;
                    else
                        p->defRanged = INV(CurrSlot);
                }
                break;                           
        case 'A':
            changed = true;
            if (oItem(INV(CurrSlot)))
                if (oItem(INV(CurrSlot))->isType(T_MISSILE) || 
                    oItem(INV(CurrSlot))->RangeInc(p) || 
                    oItem(INV(CurrSlot))->HasIFlag(IT_THROWABLE)) {
                    if (INV(CurrSlot) == p->defAmmo)
                        p->defAmmo = 0;
                    else
                        p->defAmmo = INV(CurrSlot);
                    if (INV(CurrSlot) == p->defMelee)
                        p->defMelee = 0;
                    if (INV(CurrSlot) == p->defOffhand)
                        p->defOffhand = 0;
                }
                break;
        default:
default_label: 
            if (strchr(SlotLetters,ch))
                CurrSlot = (strchr(SlotLetters,ch) - SlotLetters)+1;
            if (isdigit(ch) && currCon) {
                if (p->Inv[SL_INAIR]) {
                    Message("You need the 'in air' slot free to do that.");
                    break;
                }
                if (!digit) {
                    digit = ch;
                    break;
                }
                num = (digit-'0')*10 + (ch-'0') - 1;
                if (Contents[num] && !(currCon->HasStati(LOCKED))) {
                    if (conCursor) {
                        CurrSlot = num + NUM_SLOTS;
                        if (currCon && conCursor && CurrSlot >= NUM_SLOTS) {
                            int16 o_off = offset;
                            SetWin(WIN_INVEN);
                            if (offset > yContents[CurrSlot - NUM_SLOTS])
                                offset = max(0,yContents[CurrSlot - NUM_SLOTS] - 4);
                            else if (offset + (WinSizeY()-(NUM_SLOTS)) < yContents[CurrSlot - NUM_SLOTS])
                                offset = max(0,(yContents[CurrSlot - NUM_SLOTS]+4) - (WinSizeY()-(NUM_SLOTS)));
                            if (offset != o_off)
                                UpdateScrollArea(offset);
                        }
                    } else {
                        changed = true;
                        Item *it = Contents[num];
                        if (Throw(EV_TAKEOUT,p,NULL,currCon,it)==DONE)
                            p->Inv[SL_INAIR] = it->myHandle;
                    }
                }
            }
        }
        if (!currCon || cs < 0 || !p->Inv[cs] || (oItem(p->Inv[cs])) != currCon)
            goto ResetCurrCon;

    }
}

Item* OneSomeAll(Item *it, Creature *cr, bool assume_some)
  {
    char ch; Term *t; int32 n; Item *it2;
    if (it->GetQuantity() == 1)
      return it;
    retry:
    if (assume_some)
      ch = 's';
    else if (it->HasIFlag(IT_ROPE))
      ch = cr->ChoicePrompt(Format("%d feet total. Some or all?", 
                          it->GetQuantity()*5), "sa", 'a', CYAN, SKYBLUE, false);    
    else if (it->GetQuantity() == 2)
      ch = cr->ChoicePrompt("2 total. One or all?", "oa", 'a', CYAN, SKYBLUE, false);
    else
      ch = cr->ChoicePrompt(Format("%d total. One, some or all?", 
                          it->GetQuantity()), "osa", 'a', CYAN, SKYBLUE, false);
    switch (ch)
      {
        case -1:
          goto retry;
        case 'a': return it;
        case 'o': return it->TakeOne();
        case 's':
          t = ((Player*)cr)->MyTerm;
          Retry:
          t->SetWin(WIN_INPUT);
          t->Clear();
          t->Color(MAGENTA);
          n = atoi(t->StringPrompt(MAGENTA,it->HasIFlag(IT_ROPE) ?
            "Enter Number of Feet: " : "Enter Quantity: "));
          t->Clear();
          
          if (it->HasIFlag(IT_ROPE))
            n = (n+4)/5;
          if (n < 1 || n > it->GetQuantity())
            {
              cr->IPrint("Invalid input.");
              goto Retry;
            }
          if (n == it->GetQuantity())
            return it;
          it2 = it->TakeOne();
          it2->SetQuantity(n);
          it->SetQuantity(it->GetQuantity() - (n-1));
          return it2;
        default:
          return it;
      }
  }

void TextTerm::BarterManager(Creature *Seller)
  {
    hObj Wares[500]; rID ssList[128]; Item *it; rID spID;
    int16 i, j, c, n, width, lItem, lSpell, off, curr;
    String ShopName, SellerName; int16 ch, digit; 
    static String WareNames[500];
    int32 cost;
    if (!Seller)
      return;
      
    if (Seller->HasStati(SUMMONED))
      {
        p->IPrint("You cannot barter with summoned creatures.");
        return;
      }
            
    for (c=0,it=Seller->FirstInv();it;it=Seller->NextInv())
      if (Seller->HasMFlag(M_SELLER) || !(it->IFlags & IF_WORN))
        if ((!it->eID) || stricmp(NAME(it->eID),"Item Preservation"))
          if (!it->isType(T_COIN))  
            Wares[c++] = it->myHandle;

    for (i=0;i<c;i++)
      if (oItem(Wares[i])->getShopCost(p,Seller) <= 0)
        {
          memmove(&Wares[i],&Wares[i+1],(500 - (i+1)) * sizeof(hObj));
          c--;
        }

    lItem = c-1;
    
    for(i=0;i!=theGame->LastSpell();i++)
      {
        spID = theGame->SpellID(i);
        if (TEFF(spID)->GetConst(SERVICE_SPELL_COST))
          if (Seller->hasAccessToSpell(spID))
            Wares[c++] = spID;
      }
    lSpell = c-1;
        
    SetMode(MO_BARTER);
    
    width = WinSizeX();
    if (Seller->HasMFlag(M_SELLER)) {
      for (i=0;i!=lItem+1;i++) 
        WareNames[i] = Decolorize(oItem(Wares[i])->Name(NA_A|NA_LONG|NA_IDENT)).Left(width-18);
      }
    else {
      for (i=0;i!=lItem+1;i++) 
        WareNames[i] = Decolorize(oItem(Wares[i])->Name(NA_A|NA_LONG)).Left(width-18);
      }
    
    Redraw:
    
    SetWin(WIN_INVEN);
    Clear();
        
    SizeWin(WIN_CUSTOM,WinLeft(),WinTop()+4,WinRight(),WinBottom()-3);
    
    SGotoXY(0,0);
    
    if (Seller->HasMFlag(M_SELLER)) 
      {
        ShopName = TMON(Seller->mID)->RandMessage(MSG_SHOPNAME);
        SellerName = SC(Capitalize(Seller->Name(0),true)) + SC(", Proprietor");
      }
    else
      {
        ShopName = "Bartering";
        SellerName = Capitalize(Seller->Name(0),true);
      }
    SetWin(WIN_INVEN);
    
    GotoXY(width/2 - ShopName.GetLength()/2,0);
    Color(EMERALD);
    Write(ShopName);
    GotoXY(width/2 - SellerName.GetLength()/2,1);
    Color(YELLOW);
    Write(SellerName);
    GotoXY(2,3);
    Color(WHITE);
    Write("Merchandise for Sale");
    GotoXY(width-10,3);
    Write("Cost");
    GotoXY(0, WinSizeY()-1);
    Color(EMERALD);
    Write(Format("[Num] Buy Item [B]uy Selected [S]ell Item [ESC] Quit %s",
           Seller->HasMFlag(M_SELLER) ? "Shopping" : "Bartering"));
    
    curr = 0;
    offset = 0;
    digit = 0;
    
    PartialRedraw:
    
    ClearScroll();
    SetWin(WIN_INVEN);
    SizeWin(WIN_CUSTOM,WinLeft(),WinTop()+4,WinRight(),WinBottom()-3);
    SetWin(WIN_CUSTOM);
    
    
    for (i=0;i!=c;i++)
      {
        SGotoXY(2,i);
        if (i <= lItem)
          {
            Color(GREY);
            SWrite(Format("%s%d) %c%s%c", i >= 10 ? "" : "0", i, (i == curr) ? -YELLOW : -GREY,
              (const char*)WareNames[i], -7));
            SGotoXY(width-10,i);
            Color(GREY);
            SWrite(Format("%d gp",oItem(Wares[i])->getShopCost(p,Seller) /
                                    oItem(Wares[i])->GetQuantity()));
          }
        else
          {
            cost = TEFF(Wares[i])->GetConst(SERVICE_SPELL_COST);
            if (Seller && Seller->getLeader() == p)
              cost = 0;
            SWrite(Format("%s%d) Service Spell: %c%s%c", i >= 10 ? "" : "0", i,
              (i == curr) ? -YELLOW : (TEFF(Wares[i])->ManaCost > Seller->cMana() ? -8 : -9), 
              NAME(Wares[i]), -7));
            SGotoXY(width-10,i);
            SWrite(Format("%d gp",cost));
          }
      }
    
    UpdateScrollArea(offset,WIN_CUSTOM);
    do
      {
        ClearKeyBuff();
        ch=GetCharRaw();
        PurgeStrings();
        if (!isdigit(ch))
          digit = 0;
        switch (ch)
          {
            case KY_REDRAW:
              ShowTraits();
              ShowStatus();
              goto Redraw;
            case '?':
              HelpTopic("help::adventuring","SB");
              goto PartialRedraw;
            case 200 + NORTHEAST:
              if (currCon)
                UpdateScrollArea(offset - (1+p->Opt(OPT_SCROLL_MAG)));
              break;
            case 200 + SOUTHEAST:
              if (currCon)
                UpdateScrollArea(offset + (1+p->Opt(OPT_SCROLL_MAG)));
              break;    
            case 200 + NORTH:
              if (curr > 0)
                curr--;
              if (curr < offset) {
                offset = curr;
                UpdateScrollArea(offset);
                }
             goto PartialRedraw;
            case 200 + SOUTH:
              if (curr < lSpell)
                curr++;
              if (curr > offset + 32) {
                offset = curr - 32;
                UpdateScrollArea(offset);
                }
             goto PartialRedraw;
            case 'b': case KY_ENTER:
              if (curr <= lSpell)
                goto BuyItem;
             break;
            case 's':
              goto SellItem;
            case KY_ESC:
              SetWin(WIN_INVEN);    
              Clear();
              SetMode(MO_PLAY);
              return;
            case 'x':
              if (curr <= lItem) {
                uint32 k; Item *it;
                it = oItem(Wares[curr]);
                k = it->Known; 
                it->SetKnown(0xFF);
                Box(oItem(Wares[curr])->Describe(p));
                it->SetKnown(k);
                }
              else
                Box(TEFF(Wares[curr])->Describe(NULL));
              SetWin(WIN_INVEN);
              SizeWin(WIN_CUSTOM,WinLeft(),WinTop()+4,WinRight(),WinBottom()-3);
              goto PartialRedraw;
            default:
              if (isdigit(ch))
                {
                  if (digit == 0)
                    digit = ch;
                  else {
                    n = (digit - '0')*10 + (ch - '0');
                    digit = 0;
                    if (n > lSpell)
                      { p->IPrint("Sorry; no such item or spell available.");
                        continue; }
                    curr = n;
                    goto BuyItem;
                    }
                }
             break;
          }
      }
    while (1);
    
    BuyItem:
    if (curr > lItem)
      goto BuySpell;
    
    it = oItem(Wares[curr]);
    ASSERT(it);
    it = OneSomeAll(it,p);
    
    cost = it->getShopCost(p,Seller);
    if (cost > p->getTotalMoney() / 100)
      {
        p->IPrint("You don't have enough gold!");
        Seller->GainItem(it,false);
        goto Redraw;
      }
      
    if (!yn(Format("Confirm purchase %s?",(const char*)it->Name(0))))
      {
        Seller->GainItem(it,false);
        goto Redraw;
      }
          
    p->LoseMoneyTo(cost,Seller);
    it->Remove(false);
    it->MakeKnown(0xFF);
    p->GainItem(it,false);
    if (Seller->HasMFlag(M_SELLER))
      p->AlignedAct(AL_NONCHAOTIC,2,"mercantilism");
    p->Timeout += 30;
    SetWin(WIN_INVEN);
    Clear();
    SetMode(p->InSlot(SL_INAIR) ? MO_INV : MO_PLAY);
    return;
    
    BuySpell:
    if (curr > lSpell)
      {
        Error("Buying non-existant wares entry!");
        SetMode(MO_PLAY);
        return;
      }
      
    spID = Wares[curr];
    cost = TEFF(spID)->GetConst(SERVICE_SPELL_COST);
    ASSERT(cost);
    if (Seller && Seller->getLeader() == p)
      cost = 0;
    if (cost > p->getTotalMoney())
      {
        p->IPrint("You don't have enough gold!");
        goto Redraw;
      }
    if (TEFF(spID)->ManaCost > Seller->cMana())
      {
        p->IPrint("The <Obj> doesn't have the mana to cast "
          "that right now -- come back later!", Seller);
        goto Redraw;
      }
    p->IPrint("The <Obj> casts <9><Res><7> on your behalf.",
               Seller, spID);
    p->LoseMoneyTo(cost,Seller);           
    /* Later, this can get more complex, to account for spell
       failure, and the fact that the Seller can run out of
       mana while trying to cast a spell repeatedly; for now, 
       let's just fudge that little detail... */
    
    Seller->Timeout += 30;
    Seller->LoseMana(TEFF(spID)->ManaCost,TEFF(spID)->HasFlag(EF_LOSEMANA));
    p->Timeout += 30;
    {
      EventInfo xe;
      xe.Clear();
      xe.Event = EV_EFFECT;
      xe.EActor = p;
      xe.eID = spID;
      if (TEFF(spID)->ef.qval) {
        SetMode(MO_PLAY);
        ShowMap();
        EffectPrompt(xe,TEFF(spID)->ef.qval, false, "Target Service Spell");
        }
      ReThrow(EV_EFFECT,xe); 
    }

    SetWin(WIN_INVEN);
    Clear();
    SetMode(MO_PLAY);
    if (Seller->HasMFlag(M_SELLER))
      p->AlignedAct(AL_NONCHAOTIC,2,"mercantilism");
    return;
    
    SellItem:
    
    if (!p->FirstInv()) {
      p->IPrint("You have nothing to sell!");
      SetWin(WIN_INVEN);
      Clear();
      SetMode(MO_PLAY);
      return;
      }
    
    for (it=p->FirstInv();it;it=p->NextInv())
      LOption(it->Name(0),it->myHandle);
      
    hObj hItem;
    hItem = LMenu(MENU_2COLS|MENU_ESC|MENU_BORDER,
              "Sell Which Item?", WIN_MENUBOX);
    if (hItem == -1)
      {
        SetWin(WIN_INVEN);
        Clear();
        SetMode(MO_PLAY);        
        return;
      }
    it = oItem(hItem);
    
    if (it->isType(T_COIN))
      {
        p->IPrint("No, see, you don't sell coins; you buy things using them.");
        SetWin(WIN_INVEN);
        Clear();
        SetMode(MO_PLAY);
        return;
      }
    
    switch (Throw(EV_WILLBUY,p,Seller,it))
      {
        case DONE:
          break;
        case NOMSG:
          SetWin(WIN_INVEN);
          Clear();
          SetMode(MO_PLAY);
          return;
        default:
          p->IPrint("The <Obj> has no interest in that item.", Seller);
          SetWin(WIN_INVEN);
          Clear();
          SetMode(MO_PLAY);
          return;
      }
      
    it = OneSomeAll(it,p);
    
    if ((!it->isKnown(KN_MAGIC|KN_CURSE|KN_PLUS)) && !it->isType(T_GEM))
      {
        p->IPrint("The <Obj> won't buy un-identified items.", Seller);
        p->GainItem(it,false);
        SetWin(WIN_INVEN);
        Clear();
        SetMode(MO_PLAY);
        return;
      }
    
    if (it->isCursed())
      {
        p->IPrint("The <Obj> won't buy cursed items.", Seller);
        p->GainItem(it,false);
        SetWin(WIN_INVEN);
        Clear();
        SetMode(MO_PLAY);
        return;
      }
    
    if (Seller->getTotalMoney() < it->getShopCost(Seller,p) / 3)
      {
        p->IPrint("The <Obj> doesn't have the capital to buy that.", Seller);
        p->GainItem(it,false);
        SetWin(WIN_INVEN);
        Clear();
        SetMode(MO_PLAY);
        return;
      }
    
    if (!yn(XPrint("<Obj> offers <Num> gp for that. Accept?",Seller,
             min(Seller->getTotalMoney(),it->getShopCost(Seller,p)))))
      {
        p->GainItem(it,false);
        SetWin(WIN_INVEN);
        Clear();
        SetMode(MO_PLAY);
        return;
      }
      
    it->Remove(false);
    Seller->GainItem(it,false);
    Seller->LoseMoneyTo(min(Seller->getTotalMoney(),
               it->getShopCost(Seller,p)),p);
    p->IPrint("You sell the <Obj> a <Obj>.",Seller,it);
    SetWin(WIN_INVEN);
    Clear();
    SetMode(MO_PLAY);
    if (Seller->HasMFlag(M_SELLER))
      p->AlignedAct(AL_NONCHAOTIC,2,"mercantilism");
    return;
  }



void TextTerm::SkillManager(bool initial)
  {
    #define SKILLS_SHOWN 15
    int8 height, width, cc, i,j,k, x1, x2, y1, y2,n, sl,ln, old_curr, usp[6];
    int8 Alloc[SK_LAST][6], Ranks[SK_LAST], Show[SK_LAST*2],
           MRanks[SK_LAST], Spent[6], offset,
           last_class, last_universal, last_imp;
    int16 ch; bool maxed; String desc;

    TClass *tc; TRace *tr; const char *title;
    cc = 0; k = 0;

    p->CalcSP();

    Reset:

    memset(Alloc,0,SK_LAST*6);
    memset(Ranks,0,SK_LAST);
    memset(Show, 0,SK_LAST*2);
    memset(MRanks,0,SK_LAST);
    Spent[0] = Spent[1] = Spent[2] =
      Spent[3] = Spent[4] = Spent[5] = 0;
    usp[0] = usp[1] = usp[2] = 
      usp[3] = usp[4] = usp[5] = 0;


    usp[0] = p->UnspentSP(0);
    usp[1] = p->UnspentSP(1);
    usp[2] = p->UnspentSP(2);
    usp[3] = p->UnspentSP(3);
    usp[4] = p->UnspentSP(4);
    usp[5] = p->UnspentSP(5);

    Recount:
    /* Count the total number of skills that can ever be displayed
       at once -- the race skills plus the skills of whichever single
       class the player has that offers the most skills. */
    
    for(i=0;i!=6;i++)
      if (p->ClassID[i]) {
        tc = TCLASS(p->ClassID[i]);
        for(j=0;tc->Skills[j];j++)
          ;
        if (j > k)
          k = j;
      }

    tr = TRACE(p->RaceID);
    for(i=0;tr->Skills[i];i++)
      k++;

    StatiIter(p)
      if (S->Nature == EXTRA_SKILL)
        k++;
    StatiIterEnd(p)

    width = 62;
    height = SKILLS_SHOWN + 15;

    SetWin(WIN_SCREEN);
    x1 = WinRight()/2 - width/2;
    y1 = WinBottom()/2 - height/2;
    x2 = WinRight()/2 + width/2;
    y2 = WinBottom()/2 + height/2 + height%2;
    

    /* Set the dimensions. */
    Save();
    SetWin(WIN_SCREEN);

    /* Draw the window border */

    Color(SKYBLUE);
    for(i=x1;i<=x2;i++)
      {
        PutChar(i,y1-1,205);
        PutChar(i,y2+1,205);
      }
    for(i=y1;i<=y2;i++)
      {
        PutChar(x1-1,i,186);
        PutChar(x2+1,i,186);
      }
    Color(CYAN);
    PutChar(x1-1,y1-1,219);
    PutChar(x1-1,y2+1,219);
    PutChar(x2+1,y1-1,219);
    PutChar(x2+1,y2+1,219);

    Color(AZURE);
    title = " -- Skill Manager -- ";
    Write(WinRight()/2-strlen(title)/2,y1-1,title);
    SizeWin(WIN_CUSTOM,x1,y1,x2,y2);
    offset = 0;
    tc = TCLASS(p->ClassID[cc]);
    do {

      j = 0;
      for(i=1;i!=SK_LAST;i++)
        if (tc->HasSkill(i) && (SkillInfo[i].imp || 1))
          Show[j++] = i;
      last_class = j;

      for (i=1;i!=SK_LAST;i++)
        if (tr->HasSkill(i) || p->HasStati(EXTRA_SKILL,i) ||
              isFamiliar(i))
          if (!tc->HasSkill(i))
            if (SkillInfo[i].imp || 1)
              Show[j++] = i;
      last_universal = j;

      for (i=1;i!=SK_LAST;i++)
        if (!(tc->HasSkill(i) || tr->HasSkill(i) || isFamiliar(i) ||
                p->HasStati(EXTRA_SKILL,i)))
          if (SkillInfo[i].imp || 1)
            Show[j++] = i;
      last_imp = j;

      for (i=1;i!=SK_LAST;i++)
        if(0 && !SkillInfo[i].imp)
          Show[j++] = i;
      Show[j] = 0;  
      ASSERT(SK_LAST >= j)
      ln = 0;
      
      ReDraw:

      SetWin(WIN_CUSTOM);
      Clear();
      Color(WHITE);
      Write(0,0,Format("Unspent Skill Ranks (%s):",NAME(p->ClassID[cc])));
      Color(AZURE);
      sl = (usp[cc])/50 + 3;
    
      for(i=0;i!=usp[cc];i++)
        PutChar((i%50)+2,1+i/50,'*');

        
      Color(SKYBLUE);
      Write(0,sl-1,"Skill                 Ranks  Max  Att Rating      Points to Add");

      for(i=offset,j=0;Show[i] && j < SKILLS_SHOWN;i++)
        if (Show[i])
          {
            PurgeStrings();
            maxed = p->SkillRanks[Show[i]] == p->MaxRanks(Show[i]);

            if (i < last_class)
              Color(maxed ? GREEN : EMERALD);
            else if (i < last_universal)
              Color(maxed ? BROWN : YELLOW);
            else if (i < last_imp)
              Color(GREY);
            else
              Color(SHADOW);
            Write(2,sl+j,SkillInfo[Show[i]].name);
            Color(GREY);
            Write(24,sl+j,Format("%d",p->SkillRanks[Show[i]] /*+ 
                            Alloc[i][0] + Alloc[i][1] + Alloc[i][2]*/));
            Write(30,sl+j,Format("%d",p->MaxRanks(Show[i])));
            static const char*AttrNames[] =
	            { "Str", "Dex", "Con", "Int", "Wis", "Cha", "Luc" };

            Write(34,sl+j,Format("%s",AttrNames[p->SkillAttr(Show[i])]));
            Write(40,sl+j,Format("%+d",p->SkillLevel(Show[i])));
            Color(AZURE);
            for(k=0;k!=Alloc[Show[i]][cc];k++)
              PutChar(50+k,sl+j,'*');
            j++;
          }
      Color(GREEN);
      Write(0,WinSizeY()-1,Format("[%c%c] Select Skill [%c%c] Allocate Ranks [ESC] Abort [ENTER] Done",
        GLYPH_ARROW_UP, GLYPH_ARROW_DOWN, GLYPH_ARROW_LEFT, GLYPH_ARROW_RIGHT));
      Write(0,WinSizeY()-2,Format("[?] Skill Manager Help / Skill Descriptions [TAB] Change Class"));
      
      SizeWin(WIN_MENUDESC,x1+2,y1+sl+SKILLS_SHOWN+1,x2-2,y2-3);
      
      j--;
      Color(EMERALD);
      PutChar(0,sl+ln,'>');
      old_curr = -1;
      do {
        if (old_curr != ln + offset)
          {
            SetWin(WIN_MENUDESC);
            ClearScroll();
            desc = Upper(Format("%c%s%c ",
              -12,SkillInfo[Show[offset+ln]].name,-7));
            desc += XPrint(DescribeSkill(Show[offset+ln]));
            SWrapWrite(0,0,desc,WinSizeX()-2,WIN_MENUDESC);
            UpdateScrollArea(0,WIN_MENUDESC);
            SetWin(WIN_CUSTOM);
          }
        old_curr = ln + offset;
        ch = GetCharCmd(KY_CMD_ARROW_MODE);
        PurgeStrings();
        switch(ch) {
          case KY_REDRAW:
            goto Recount;
          case KY_CMD_TAB:
          case KY_TAB:
            int8 occ; occ = cc;
            do {
              cc++;
              if (cc == 6)
                cc = 0;
              }
            while (!p->ClassID[cc] && cc!=occ);
            if (cc != occ) {
              Restore();
              goto Recount;
              }
           break;
          case KY_HELP:
            Restore();
            HelpTopic("help::skills","SM");
            goto Recount;  
          case KY_CMD_NORTH:
            if (!ln) {
              if (offset > 0)
                { offset--; goto ReDraw; }
              break;
              }
            PutChar(0,sl+ln,' ');
            ln--;
            Color(EMERALD);
            PutChar(0,sl+ln,'>');
           break; 
          case KY_CMD_SOUTH:
            if (ln >= min(j,SKILLS_SHOWN)) {
              if (offset + SKILLS_SHOWN < (SK_LAST-1))
                { offset++; goto ReDraw; }
              break;
              }
            PutChar(0,sl+ln,' ');
            ln++;
            Color(EMERALD);
            PutChar(0,sl+ln,'>');
           break;
          case KY_CMD_EAST: 
            /* Do not allow ranks to be placed in out-of-class/race skills,
               or in skills that have not yet been implemented. */
            if ((offset + ln) > last_universal)
              break;
              
            if (!usp[cc] || p->SkillRanks[Show[offset + ln]] + Alloc[Show[offset + ln]][0] +
                  Alloc[Show[offset + ln]][1] + Alloc[Show[offset + ln]][2] >= 
                  p->MaxRanks(Show[offset + ln]))
              break;
            usp[cc]--;
            PutChar((usp[cc]%50)+2,1+usp[cc]/50,' ');
            Alloc[Show[offset + ln]][cc]++;
            Color(AZURE);
            PutChar(49+Alloc[Show[offset+ln]][cc],sl+ln,'*');
           break;
          case KY_CMD_WEST: 
            if (!Alloc[Show[offset + ln]][cc])
              break;
            Color(AZURE);
            PutChar((usp[cc]%50)+2,1+usp[cc]/50,'*');
            usp[cc]++;
            Alloc[Show[offset + ln]][cc]--;
            PutChar(50+Alloc[Show[offset + ln]][cc],sl+ln,' ');
           break;
          case KY_CMD_NORTHEAST:
            ScrollUp(WIN_MENUDESC);
            SetWin(WIN_CUSTOM);
           break;
          case KY_CMD_SOUTHEAST:
            ScrollDown(WIN_MENUDESC);
            SetWin(WIN_CUSTOM);
           break;
          case KY_ENTER:
            Color(YELLOW);
            Write(0,WinSizeY()-1,"                                                                  ");
            Write(0,WinSizeY()-1,"Confirm keep changes to skill ranks? [yn] ");
            CursorOn();
            do 
              ch = tolower(GetCharRaw());
            while(ch != 'y' && ch != 'n' && ch != KY_REDRAW);
            CursorOff();
            if (ch == KY_REDRAW)
              goto Recount;
            if (ch == 'n') {
              Color(GREEN);
              Write(0,WinSizeY()-1,Format("[%c%c] Select Skill [%c%c] Allocate Ranks [ESC] Abort [ENTER] Done",
                GLYPH_ARROW_UP, GLYPH_ARROW_DOWN, GLYPH_ARROW_LEFT, GLYPH_ARROW_RIGHT));
              break;
              }
            for(i=0;i!=SK_LAST;i++)
              p->SkillRanks[i] += Alloc[i][0] + 
                                  Alloc[i][1] +
                                  Alloc[i][2] +
                                  Alloc[i][3] +                              
                                  Alloc[i][4] +
                                  Alloc[i][5];
            
            for (i=0;i!=6;i++)
              p->SpentSP[i] = p->TotalSP[i] - usp[i];
            /* Fall Through */
          default: 
            if (initial)
              if (p->UnspentSP(0) || p->UnspentSP(3))
                goto Reset;
            Restore();
            SetWin(WIN_SCREEN);
            Clear();
            return;
          }
          

        }
      while(1);
      }
    while(1);
    


  

  }

const char* OptChoice(const char *text, int16 num)
  {
    static char sel[30]; int16 i = 0, j = 0, n = 0;
    for (i=0;text[i];i++) {
      if (text[i] == '/')
        n++;
      else if (n == num)
        sel[j++] = text[i];
      }
    sel[j] = 0;
    return j ? sel : "???";
  }

void TextTerm::OptionManager(int16 start_cat)
  {
    int16 OptsShown[100], i, c, n, Off, Sec, ch, oldMode;
    int16 oldVid[5];
    const char *banner;
    Sec = start_cat;
    oldMode = Mode;
    SetMode(MO_DIALOG);
    
    Player *pp;
    
    if (isValidHandle(theGame->p[0]))
      pp = oPlayer(theGame->p[0]);
    else {
      pp = new Player(this);
      pp->LoadOptions(true);
      }
    
    oldVid[0] = pp->Opt(OPT_FULL_RES);
    oldVid[1] = pp->Opt(OPT_FULL_FONT);
    oldVid[2] = pp->Opt(OPT_WIND_RES);
    oldVid[3] = pp->Opt(OPT_WIND_FONT);
    oldVid[4] = pp->Opt(OPT_SOFT_PALETTE);
    do
      {
        n = 0; c = 0; Off = 0;
        for (i=0;OptionList[i].Val;i++)
          if (OptionList[i].Val >= Sec && 
                OptionList[i].Val <= Sec+99)
            OptsShown[n++] = i;

        RedrawList:

        SetWin(WIN_OPTIONS);
        SizeWin(WIN_CUSTOM,2,WinSizeY()-8,WinSizeX()-2,WinSizeY());
        Clear(); Color(PINK);
        switch (Sec)
          {
            case OPC_CHARGEN:
              banner = "Character Generation Options";
             break;
            case OPC_WIZARD:
              banner = "Wizard Mode Options";
             break;
            case OPC_LIMITS:
              banner = "Limits and Warnings";
             break;
            case OPC_INPUT:
              banner = "Input Options";
             break;
            case OPC_DISPLAY:
              banner = "Output Options";
             break;
            case OPC_TACTICAL:
              banner = "Tactics and Combat Options";
             break;
            case OPC_STABILITY:
              banner = "Stability Options";
             break; 
            case OPC_MACROS:
              banner = "Macro Bindings";
             break;
            default:
              banner = "???";
          }
        Write((WinSizeX()-strlen(banner))/2,1,banner);
        Color(RED);
        if (Sec == OPC_MACROS || Sec == OPC_INPUT ||
            Sec == OPC_DISPLAY || Sec == OPC_STABILITY ||
            Sec == OPC_LIMITS)
          banner = "(Universal Selections)";
        else if (Sec == OPC_CHARGEN) {
          if (!isValidHandle(theGame->p[0]))
            banner = "(Default CharGen for All New Characters)";
          else if (pp->Attr[0] <= 0)
            banner = "(Options for Current Character)";
          else
            banner = XPrint("(<Obj>'s Choices)",theGame->GetPlayer(0));
          }
        else if (isValidHandle(theGame->p[0]))
          banner = XPrint("(<Obj>'s Preferences)",theGame->GetPlayer(0));
        else
          banner = "(Initial Defaults for New Characters)";
        Write((WinSizeX()-strlen(banner))/2,2,banner);
        Color(GREY);
        if (Sec == OPC_MACROS)
          {
            for (i=Off;i < min(Off+(WinSizeY()-13),MAX_MACROS);i++)
              {
                Color(pp->Macros[i] ? EMERALD : GREY);
                Write(4,3+(i-Off),Format(i <= 12 ? "F%d" :
                                         i <= 24 ? "CTRL+F%d" :
                                         i <= 36 ? "ALT+F%d" :
                                                   "SHIFT+F%d",i+1));
                Color(WHITE);
                if (pp->Macros[i])
                  Write (14,3+(i-Off),NAME(pp->Macros[i]));
              }
          }
        else if (!n)
          Write(4,4,"[No Options Yet]");
        else for (i=Off;i < min(Off+(WinSizeY()-13),n);i++)
          {
            Color(i == c ? EMERALD : GREY);
            Write(4,4+(i-Off),OptionList[OptsShown[i]].Name);
            Color(YELLOW);
            Write(WinSizeX()-17,4+(i-Off),OptChoice(OptionList[OptsShown[i]].Choices,
                                 pp->Options[OptionList[OptsShown[i]].Val]));
          }

        ClearScroll();
        if (OptionList[OptsShown[c]].Desc) {
          Color(GREEN);
          SGotoXY(0,0);
          SWrapWrite(0,0,SC("__")+SC(XPrint(OptionList[OptsShown[c]].Desc)),WinSizeX()-2);
          }
        UpdateScrollArea(0);
        SetWin(WIN_OPTIONS);

        Color(EMERALD);
        Write(0,WinSizeY()-1,Format("[%c%c] Select [SPACE] Change Option [%c%c TAB] Change Group",
          GLYPH_ARROW_UP, GLYPH_ARROW_DOWN, GLYPH_ARROW_LEFT, GLYPH_ARROW_RIGHT
            ));
        Write(0,WinSizeY()-2,"[R] Reset All Options [PGUP/PGDN] Scroll Option Descriptions");    
        BadChar:
        SetWin(WIN_OPTIONS);
        ch = GetCharRaw();

        switch(ch)
          {
            case KY_REDRAW:
              goto RedrawList;
            case '?':
              HelpTopic("help::interface","OP");
              goto RedrawList;
            case KY_ESC:
            case KY_ENTER:
              pp->statiChanged = true;
              pp->UpdateOptions(isValidHandle(theGame->p[0]));
              if (oldVid[0] != pp->Opt(OPT_FULL_RES) ||
                  oldVid[1] != pp->Opt(OPT_FULL_FONT) ||
                  oldVid[2] != pp->Opt(OPT_WIND_RES) ||
                  oldVid[3] != pp->Opt(OPT_WIND_FONT) ||
                  oldVid[4] != pp->Opt(OPT_SOFT_PALETTE))
                Reset();
              Clear();
              if (oldMode == MO_PLAY && m) {
                RefreshMap();
                m->ResetImages();     
                }
              SetMode(oldMode);
              if (!isValidHandle(theGame->p[0]))
                delete pp;
             return;
            case KY_SPACE:
              if (Sec == OPC_CHARGEN)
                if (pp->m && !pp->WizardMode) {
                  Message("Character generation options cannot be changed "
                    "once a game has already begun. To alter these options, "
                    "you must choose to do so at the time you are creating "
                    "a new character.");
                  goto RedrawList;
                  }
              pp->Options[OptionList[OptsShown[c]].Val]++;
              if (pp->Options[OptionList[OptsShown[c]].Val] >
                  OptionList[OptsShown[c]].Maximum)
                pp->Options[OptionList[OptsShown[c]].Val] = 0;        
             goto RedrawList;
            case 200 + NORTH:
              if (c > 0) 
                c--; 
              if (Off > c) 
                Off = c;
             goto RedrawList;
            case 200 + SOUTH:
              if (c < n-1)
                c++;
              if (c - (WinSizeY() - 13) > Off)
                Off = c - (WinSizeY() - 13);
             goto RedrawList;
            #if 0
            case 'c': case 'C':
              Sec = OPC_CHARGEN;
              c = Off = 0;
             break; 
            case 'u': case 'U':
              Sec = OPC_TERMINAL;
              c = Off = 0;
             break; 
            case 'g': case 'G':
              Sec = OPC_GAMEPLAY;
              c = Off = 0;
             break; 
            case 'd': case 'D':
              Sec = OPC_DISPLAY;
              c = Off = 0;
             break; 
            case 't': case 'T':
              Sec = OPC_TACTICAL;
              c = Off = 0;
             break; 
            case 'w': case 'W':
              if (!pp->WizardMode)
                goto BadChar;
              Sec = OPC_WIZARD;
              c = Off = 0;
             break; 
            #endif
            case KY_TAB:
            case 200 + EAST:
              Sec += 100;
              if (Sec == OPC_STABILITY)
                Sec += 100;
              if (Sec >= (pp->WizardMode ? OPC_WIZARD+100 : OPC_WIZARD))
                Sec = 100;
             break;
            case 200 + WEST:
              Sec -= 100;
              if (Sec == OPC_STABILITY)
                Sec -= 100;
              if (Sec <= 0)
                Sec = (pp->WizardMode ? OPC_WIZARD+100 : OPC_WIZARD) - 100;
             break;
            case 200 + NORTHEAST:
              ScrollUp();
              goto BadChar;
            case 200 + SOUTHEAST:
              ScrollDown();
              goto BadChar;
            case 'r': case 'R':
              if (yn("Confirm reset all options to hardcoded defaults?"))
                {
                  pp->ResetOptions();
                  memset(p->Macros,0,sizeof(rID)*MAX_MACROS);
                  pp->Macros[2] = FIND("AutoRest");
                  pp->Macros[4] = FIND("AutoBuff");
                  pp->Macros[7] = FIND("AutoLoot");
                  pp->Macros[8] = FIND("AutoDrop");
                }
             break;
            default:
              if (Sec == OPC_MACROS && ch >= FN(1) && ch <= FN(12))
                {
                  rID mID; int16 k;
                  theGame->GetEffectID(PUR_WIZARD,0,20,AI_MACRO);
                  if (!Candidates[0])
                    goto BadChar;
                  for (i=0;Candidates[i];i++)
                    LOption(NAME(Candidates[i]),
                            Candidates[i],
                            DESC(Candidates[i]));
                  mID = LMenu(MENU_2COLS|MENU_DESC|MENU_BORDER|MENU_ESC,
                                "Bind which Macro?");
                  if (mID == -1)
                    goto BadChar;
                  k = ch - FN(1);
                  if (ControlKeys & CONTROL)
                    k += 12;
                  else if (ControlKeys & ALT)
                    k += 24;
                  else if (ControlKeys & SHIFT)
                    k += 36;
                  if (k >= MAX_MACROS)
                    goto BadChar;
                  for (i=0;i!=MAX_MACROS;i++)
                    if (pp->Macros[i] == mID)
                      pp->Macros[i] = 0;
                  pp->Macros[k] = mID;
                  break;  
                }
              goto BadChar;
          }
      }
    while(1);


  }

/*****************************************************************************\
*                                 TEXTTERM                                   *
*                          Character Sheet Screen                            *
\*****************************************************************************/


void TextTerm::DisplayCharSheet()
{
  CharSheet cs; String s; Monster *mn;
  int16 i, y, y1, y2, y3, q; rID xID, endID; 
  int update = 1;  /* ww: recalculate character sheet? */
  
  do {
    if (update) {
      CreateCharSheet(cs); 
      update = 0;
    } 
    SetMode(MO_CHARVIEW);
    SetWin(WIN_SCREEN);
    Clear();
    Color(EMERALD);
    Write(0,WinSizeY()-3,"[P]restige Classes [PGUP/PGDN] Scroll [W]rite Dump [ESC] Return to Game");
    Write(0,WinSizeY()-2,"[G]ain Levels [L]earn Spells [S]kill Manager [M]ulticlass [C]onditions [Q]uests");
    Write(0,WinSizeY()-1,"[K]nown Items [F]eat Descriptions [J]ournal [R]ecall Monster [D] Alignment");
    SizeWin(WIN_CSHEET,0,0,WinSizeX(),WinSizeY() - 4);
    SetWin(WIN_CSHEET);
    Clear();
    ClearScroll();
    SWrite(WinSizeX()/2-cs.Banner.GetLength()/2,0,cs.Banner,WIN_CSHEET);
    y = 2;    
    y1 = y + SWrapWrite(1,y,cs.Basics,25,WIN_CSHEET);
    y2 = y; 
    for (i=0;i<7;i++) {
      y2 += SWrapWrite(24,y2,cs.sAttr[i],79,WIN_CSHEET); 
    }
    y = max(y1,y2) + 1;

    for (i=0;i<3;i++)
      y += SWrapWrite(1,y,cs.sSaves[i],79,WIN_CSHEET);

    y++;

    for (i=0;i!=5;i++)
      if (cs.sCombat[i][0])
        {
          y += SWrapWrite(1,y,cs.sCombat[i][0],79,WIN_CSHEET);
          y += SWrapWrite(1,y,cs.sCombat[i][1],79,WIN_CSHEET);
          y += SWrapWrite(1,y,cs.sCombat[i][2],79,WIN_CSHEET);
        }
    y += SWrapWrite(1,y,cs.sSize,79,WIN_CSHEET);  
    y += SWrapWrite(1,y,cs.sDefense,79,WIN_CSHEET);  
    y += SWrapWrite(1,y,cs.sCoverage,79,WIN_CSHEET);  
    y += SWrapWrite(1,y,cs.sMove,79,WIN_CSHEET);  
    y += SWrapWrite(1,y,cs.sHitDice,79,WIN_CSHEET);  
    y += SWrapWrite(1,y,cs.sFatigue,79,WIN_CSHEET);  
    y += SWrapWrite(1,y,cs.sMana,79,WIN_CSHEET);  
    y += SWrapWrite(1,y,cs.sEncum,79,WIN_CSHEET);
    if (cs.sMagicRes)
      y += SWrapWrite(1,y,cs.sMagicRes,79,WIN_CSHEET);
    
    
    for (i=0;i!=4;i++)
      if (cs.sSpells[i])
        y += SWrapWrite(1,y,cs.sSpells[i],79,WIN_CSHEET);
    y += SWrapWrite(1,y,cs.sProfs,79,WIN_CSHEET);
    y++;
    
    y += SWrapWrite(1,y,cs.sSkills,79,WIN_CSHEET);
    y++;

    y1 = y + SWrapWrite(1,y,cs.sFeats, 40, WIN_CSHEET); y1++;
    
    y2 = y + SWrapWrite(40,y,cs.sSpecials,79,WIN_CSHEET); y2++;
    y2 += SWrapWrite(40,y2,cs.sResists,79,WIN_CSHEET); y2++;
    if (cs.sImmune) {
      y2 += SWrapWrite(40,y2,cs.sImmune,79,WIN_CSHEET); y2++;
    }
    if (cs.sDepths) {
      y2 += SWrapWrite(40,y2,cs.sDepths,79,WIN_CSHEET); y2++;
    }
    
    if (cs.sRacial.GetLength()) {
      if (y1 > y2) { 
        y2 += SWrapWrite(40,y2,cs.sRacial,79,WIN_CSHEET); y2++;
        } 
      else { 
        y1 += SWrapWrite(1,y1,cs.sRacial,40,WIN_CSHEET); y1++;
        }
      } 
    
    
    if (cs.sBody.GetLength()) {
      if (y1 > y2) { 
        y2 += SWrapWrite(40,y2,cs.sBody,79,WIN_CSHEET); y2++;
        } 
      else { 
        y1 += SWrapWrite(1,y1,cs.sBody,40,WIN_CSHEET); y1++;
        }
      } 
    if (y1 > y2) { 
      y2 += SWrapWrite(40,y2,cs.sPerception,79,WIN_CSHEET); y2++;
    } else { 
      y1 += SWrapWrite(1,y1,cs.sPerception,40,WIN_CSHEET); y1++;
    } 
    
    if (cs.sAutoBuff.GetLength()) {
      if (y1 > y2) { 
        y2 += SWrapWrite(40,y2,cs.sAutoBuff,79,WIN_CSHEET); y2++;
        } 
      else { 
        y1 += SWrapWrite(1,y1,cs.sAutoBuff,40,WIN_CSHEET); y1++;
        }
      } 
    
    if (y1 > y2) { 
      y2 += SWrapWrite(40,y2,cs.sSpiritual,79,WIN_CSHEET); y2++;
    } else { 
      y1 += SWrapWrite(1,y1,cs.sSpiritual,40,WIN_CSHEET); y1++;
    } 

    if (y1 > y2) { 
      y2 += SWrapWrite(40,y2,cs.sCharGen,79,WIN_CSHEET); y2++;
    } else { 
      y1 += SWrapWrite(1,y1,cs.sCharGen,40,WIN_CSHEET); y1++;
    } 
    
    UpdateScrollArea(offset,WIN_CSHEET);
    int16 ch;
    switch(ch = toupper(GetCharCmd(KY_CMD_ARROW_MODE))) {
      case KY_REDRAW:
        update = 1;
        break;
      case KY_CMD_NORTHEAST: offset--; break;
      case KY_CMD_SOUTHEAST: offset++; break; 
      case KY_CMD_NORTH: offset -= (WinSizeY() - 6); break; 
      case KY_CMD_SOUTH: offset += (WinSizeY() - 6); break; 
      case 'G': 
      case 'A':
        if (p->NextLevXP() > (p->XP - p->XP_Drained) && !p->Opt(OPT_FREE_ADV)) {
          Message(Format("You have %d experience, and you need %d to reach level %d.",
                (p->XP - p->XP_Drained), p->NextLevXP(), p->TotalLevel()+1));
          break;
        }
        p->AdvanceLevel();
        update = 1; 
        break; 
      case 'D':
        for (i=0;i!=9;i++)
          LOption(AlignmentInfo[i].name,i,XPrint(AlignmentInfo[i].desc));
        i = LMenu(MENU_ESC|MENU_BORDER|MENU_3COLS|MENU_DESC, "Desired Alignment",
              WIN_MENUBOX, "State the alignment your character aspires "
              "to live in accord with and Incursion will evaluate your "
              "actions in that context.");
        if (i != -1)
          p->desiredAlign = AlignmentInfo[i].align;
        update = true;
       break;
      case 'S': 
        SkillManager(); 
        update = 1; 
        break;

      case 'L':
      case 'E':
        if (!(p->CasterLev())) {
          Message("You are not a spellcaster.");
          break;
        }
        p->LearnSpell();
        update = 1; 
        break; 

      case 'P':
        if (p->TotalLevel() < 3)
          { p->IPrint("You need at least three levels in core classes "
              "before you can select a prestige class."); break;}
        if ((p->ClassID[2] && TCLASS(p->ClassID[2])->HasFlag(CF_PRESTIGE)) ||
            (p->ClassID[1] && TCLASS(p->ClassID[1])->HasFlag(CF_PRESTIGE)) )
          { p->IPrint("You already have a prestige class; you can only have one.");
            break; }
        /* Fall Through */
      case 'M':
        for (q=0;q!=MAX_MODULES;q++)
          if (theGame->Modules[q]) {
            xID = theGame->Modules[q]->ClassID(0);
            endID = xID + theGame->Modules[q]->szCla;
            for(;xID!=endID;xID++)
              {
                if (xID == p->ClassID[0] || xID == p->ClassID[1] ||
                    xID == p->ClassID[2])
                  continue;
                if (ch == 'P' != TCLASS(xID)->HasFlag(CF_PRESTIGE))
                  continue;
                if (TCLASS(xID)->HasFlag(CF_PSEUDO))
                  continue;
                Candidates[i] = xID;
                LOption((const char*)NAME(xID),i,DESC(xID) ? 
                    (const char*)DESC(xID) : "Description not found.");
                i++;
              }
            }
        if (!i)
          break;
        i = LMenu(MENU_ESC|MENU_DESC|MENU_3COLS|MENU_BORDER|MENU_LARGEBOX,
            "Choose a class:", WIN_MENUBOX);

        if (i == -1)
          break;

        if (p->GodID)
          {
            rID GodList[50];
            if (TCLASS(Candidates[i])->GetList(ALLOWED_GODS,GodList,49))
              {
                for (int16 j=0;GodList[j];j++)
                  if (p->GodID == GodList[j])
                    goto GodIsOkay;
                p->IPrint("Your worship of <Res> precludes you from that class.",
                    p->GodID);
                goto CannotTrain;
                GodIsOkay:;
              }
          }
        
        if (!TCLASS(Candidates[i])->HitDie) {
          p->IPrint("That class isn't done yet.");
          break;
        } else if (TCLASS(Candidates[i])->HasFlag(CF_RELIGIOUS) && !p->getGod()) {
          p->IPrint("You must pledge to a suitable god to gain levels in that class.");
          break;
        } else if (TCLASS(Candidates[i])->
            PEvent(EV_ISTARGET,p,Candidates[i]) == ABORT) {
          p->IPrint("You do not meet the requirements for that class.");
          break;
        } else if (
            (TCLASS(Candidates[i])->HasFlag(CF_GOOD) && !p->isMType(MA_GOOD))||
            (TCLASS(Candidates[i])->HasFlag(CF_EVIL) && !p->isMType(MA_EVIL))||
            (TCLASS(Candidates[i])->HasFlag(CF_LAWFUL) && !p->isMType(MA_LAWFUL))||
            (TCLASS(Candidates[i])->HasFlag(CF_CHAOTIC) && !p->isMType(MA_CHAOTIC))||
            (TCLASS(Candidates[i])->HasFlag(CF_NONGOOD) && p->isMType(MA_GOOD))||
            (TCLASS(Candidates[i])->HasFlag(CF_NONEVIL) && p->isMType(MA_EVIL))||
            (TCLASS(Candidates[i])->HasFlag(CF_NONLAWFUL) && p->isMType(MA_LAWFUL))||
            (TCLASS(Candidates[i])->HasFlag(CF_NONCHAOTIC) && p->isMType(MA_CHAOTIC))) {
          p->IPrint("You do not meet the alignment requirements for that class.");
          break; 
        }
        p->TrainAs(Candidates[i]);
        update = 1; 
        CannotTrain:
        break; 
      case 'K':
      case 'I':
        /*
        Message("This function is currently disabled.");
        break; 
        */
        // ww: it segfaults on me!
        DisplayKnownItems();
       break;
      case 'F':
        s.Empty(); 
        if (!p->Opt(OPT_HELP_MODE))
          {
            for(i=SK_LAST;i!=FT_LAST;i++)
              if (p->HasFeat(i)) {
                s += Format("%s\n", XPrint(DescribeFeat(i)) );
              }
            Box(WIN_CSHEET,0,YELLOW,GREY,s);
          }
        else
          {
            for(i=SK_LAST;i<FT_LAST;i++)
              if (p->HasFeat(i)) {
                LOption(FeatName(i),i,DescribeFeat(i));
              }
            LMenu(MENU_DESC|MENU_ESC|MENU_3COLS,"Your Current Feats",WIN_MENUBOX,"help::feats",0);
          }	
       break;
      case 'J':
      case 'O': 
        if (!p->GetJournal().GetLength())
          Message("You have no journal entries yet.");
        else {
          p->StoreLevelStats(m->Depth); 
          String s = p->GetJournal() + p->GetLevelStats();
          Box(WIN_SCREEN,BOX_WIDEBOX,EMERALD,GREY,XPrint(s));
        }
       break;
      case 'R':
        Save();
        SetMode(MO_PLAY);
        SetWin(WIN_MESSAGE);
        Clear();
        SetWin(WIN_INPUT);
        Clear();
        SetWin(WIN_SCREEN);
        mn = (Monster*)AcquisitionPrompt(ACQ_RECALL,-10,20,0);
        SetMode(MO_CHARVIEW);
        Restore();
        if (mn) {
          mn->StateFlags |= MS_KNOWN;
          Box(WIN_SCREEN,0,EMERALD,GREY,mn->Describe(p));
          delete mn;
          }
       break;
      case 'C':
       {
        /* Later, this should list little text descriptions for Stunned,
             Blind, Hungry, Confused, etc. */
        rID mentioned[1024];
        int num_men = 0; 
        s = "";
        StatiIter(p)
          if (S->Nature == POISONED || S->Nature == DISEASED)
            {
              s += Format("%c%s: %c%s\n%c  %s",
                -PINK,   (S->Nature == POISONED) ? "Poison" : "Disease",
                -PINK,   (const char*)Capitalize(NAME(S->eID),true),
                -GREY,   XPrint(DESC(S->eID)) );
              s += Format(" You have made %d successful saving throws against "
                          "this affliction to date.\n\n",S->Mag);
            }
          if (S->eID) {
            bool found = false;
            for (i=0; i<num_men; i++)
              if (mentioned[i] == S->eID)
                found = true;
            if (found) continue; 
            mentioned[num_men++] = S->eID; 
          } 
          if (S->Source == SS_ENCH && S->eID && S->h) { 
            s += Format("%cSpell Effect: %c%s (from %s, %s)\n%c  %s\n\n",
                -SKYBLUE, -SKYBLUE, NAME(S->eID),
                TEFF(S->eID)->HasSource(AI_TRAP) ? "a trap" : (
                (S->h == p->myHandle) ? "yourself" : (const char*)oThing(S->h)->Name(0) ),
                (S->Duration > 0 ? 
                  (const char*)Format("duration %d",S->Duration)
                  : 
                  (S->Duration == -2 ? "persistant" : "permanent")),

                -GREY, (const char*)XPrint(DESC(S->eID)) );
            } 
          else if (S->Source == SS_ENCH && S->eID) {
            s += Format("%cSpell Effect: %c%s\n%c  %s\n\n",
                -SKYBLUE, -SKYBLUE, NAME(S->eID),
                -GREY,    XPrint(DESC(S->eID)) );
            }
          else if (S->Source == SS_CURS && S->eID && S->h) { 
            s += Format("%cCurse: %c%s (from %s, %s)\n%c  %s\n\n",
                -SKYBLUE, -SKYBLUE, NAME(S->eID),
                TEFF(S->eID)->HasSource(AI_TRAP) ? "a trap" : (
                (S->h == p->myHandle) ? "yourself" : (const char*)oThing(S->h)->Name(0) ),
                (S->Duration > 0 ? 
                  (const char*)Format("duration %d",S->Duration)
                  : 
                  (S->Duration == -2 ? "persistant" : "permanent")),

                -GREY, (const char*)XPrint(DESC(S->eID)) );
            } 
          else if (S->Source == SS_CURS && S->eID) {
            s += Format("%cCurse: %c%s\n%c  %s\n\n",
                -SKYBLUE, -SKYBLUE, NAME(S->eID),
                -GREY, (const char*) XPrint(DESC(S->eID)) );
            }
        StatiIterEnd(p)
        if (!s.GetLength())
          s = "You have no special conditions at present.";
        Box(WIN_CSHEET,0,YELLOW,GREY,s);
       }
       break;
      case 'W':
        SetWin(WIN_SCREEN);
        GotoXY(0,WinSizeY()-1); 
        Color(YELLOW);
        Write("Filename:                                                             ");
        GotoXY(10,WinSizeY()-1);
        ReadLine(cx,cy);
        if (Input[0])
          DumpCharacter(SC(Input));
       break;
      case KY_CMD_HELP:
      case '?':
        HelpTopic("help::chargen");
        break; 
      default: 
        goto DoneChar; 
    }
  }
  while(1);

DoneChar:
  SetWin(WIN_SCREEN);
  Clear();
  SetMode(MO_PLAY);
  RefreshMap();
  p->CalcValues(false);
} 
