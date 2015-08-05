/* CREATE.CPP -- Copyright 1999-2003 (c) Julian Mensch
Routines describing character creation, attribute determination,
feat selection and the advancement of class-based and racial abilities.
Also, general functions relating specifically to the Character class
that describe traits of full characters not shared by monsters.

String PickName(const char*NameList)
void Player::Create()
void Player::RollAttributes()
void Player::ChooseDomains()
void Character::AddAbilities(rID resID, int16 Lev)
uint32 Character::NextLevXP()
void Character::GainXP(uint32 _XP)
void Character::LoseXP(uint32 _XP)
void Character::GainXP(Creature *kill)
int16 Character::XPPenalty()
void Character::DrainXP(int32 amt)
void Player::AdvanceLevel()
void Player::GainFeat(int16 feat, int32 param)
void Player::GainAbility(int16 ab, uint32 pa, rID sourceID)
bool Character::FeatPrereq(int16 feat)
int8 Character::WepSkill(rID wID)
int8 Creature::WepSkill(rID wID)
bool Character::HasFeat(int16 ft)
int16 Character::MaxRanks(int16 sk)
bool Creature::HasSkill(int16 sk)
bool Character::HasSkill(int16 sk)
int16 Character::LevelAs(rID cl)
int16 Character::IAttr(int8 a)
int16 Character::CasterLev()
int16 Creature::SkillLevel(int16 sk)
int16 Creature::SkillAttr(int16 sk)
void  Character::TrainAs(rID cID)
void  Player::LearnSpell()
*/


#include "Incursion.h"
#define RF(a) (a^2)

int16 Studies[9][4] =
//    Study Index, Class Ability, Increase, Rate Ability is Gained
{ { STUDY_TURNING, CA_TURNING, 2, 1 } ,
{ STUDY_SMITE, CA_SMITE, 4, 1 },
{ STUDY_UNARMED, CA_UNARMED_STRIKE, 2, 1 },
{ STUDY_SNEAK, CA_SNEAK_ATTACK, 1, 2 },
{ STUDY_CASTING, CA_SPELLCASTING, 1, 1 },
{ STUDY_BARDIC, CA_LEGEND_LORE, 2, 1 },
{ STUDY_SHAPES, CA_WILD_SHAPE, 2, 1 },
{ STUDY_MOUNT, CA_SACRED_MOUNT, 3, 1 },
{ 0, 0 } };


int16 s_racial, s_enhance, s_feat, s_domain, s_item, s_train, s_syn,
s_armour, s_comp, s_circ, s_inh, s_size, s_ins, s_kit, s_focus;    

ReincarnationInfo RInf;

String & DescribeSkill(int16 sk);
static void GrantPerks(PerkSet& p, Player *target);

String & PickName(const char*NameList) {
    int16 i, NamePos[128], n;
    String Name;
    if (!NameList)
        return SC("Bob");

    n = 1; NamePos[0] = 0;
    for(i=0;NameList[i];i++)
        if (NameList[i] == '/')
            NamePos[n++] = i+1;

    n = random(n);
    for(i=NamePos[n];NameList[i] && NameList[i]!='/';i++)
        Name += NameList[i];

    return *tmpstr(Name.Trim());
}

hObj excessItems[30];

bool isExploreMode(Player *p) { 
    if (p->Opt(OPT_BEGINKIT) ||
        p->Opt(OPT_MAX_MANA) ||
        p->Opt(OPT_MAX_HP) ||
        (p->Opt(OPT_EASY_INTUIT) == 1) ||
        (p->Opt(OPT_POWER_STATS) >= 2) ||
        (p->Opt(OPT_DIFFICULTY) < 2) ||
        (p->Opt(OPT_ELUDE_DEATH) == 4))
        return true;
    return false;
}

void Player::Create(bool reincarnate) {
    rID xID, endID; int16 i, q, rv; char ch;
    static rID Choices[32]; bool b; time_t tm;
    ClassID[0] = ClassID[1] = ClassID[2] = 0;
    RaceID = 0; BAttr[A_STR] = -1;
    Level[0] = 1; Level[1] = Level[2] = 0;
    memset(excessItems,0,sizeof(excessItems));
    NotifiedLevel = 1;
    XP = 0;

    x = -1; 
    for(i=0;i!=(FT_LAST/8)+1;i++)
        Feats[i] = 0;
    for(i=0;i!=(MAX_SPELLS/8)+1;i++)
        Spells[i] = 0;
    for(i=0;i!=MAX_SPELLS;i++)
        MMArray[i] = 0;
    for(i=0;i!=SK_LAST+1;i++)
        SkillRanks[i] = 0;

    MyTerm->SetMode(MO_PLAY);
    if (!reincarnate) {
        if (yn("Alter the character generation options?")) {
RedoOptions:
            MyTerm->SetMode(MO_SPLASH);
            MyTerm->OptionManager(OPC_CHARGEN);
            UpdateOptions(true);
            MyTerm->Clear();
            MyTerm->SetMode(MO_CREATE);
        }
        if (isExploreMode(this))
            if (!yn("Your options will place the game in Explore Mode. Continue?"))
                goto RedoOptions;
    }

    if (isExploreMode(this))
        ExploreMode = true;

    for(i=0;i!=CA_LAST;i++)
        Abilities[i] = 0;

    if (reincarnate == false) {
        MyTerm->SetMode(MO_CREATE);
        memset(&RInf,0,sizeof(ReincarnationInfo)); 
        time(&tm);
        formulaSeed = RInf.formulaSeed = (int32)tm;
    } else {
        MyTerm->SetMode(MO_RECREATE);
        formulaSeed = RInf.formulaSeed;
        memcpy(&(Options[OPC_CHARGEN]),RInf.CharGenOpts,sizeof(int8)*100);
    }

    if (Opt(OPT_ATTR_FIRST)) {
        RollAttributes();
        CalcValues(true);
        MyTerm->ShowTraits(); MyTerm->ShowStatus();
        MyTerm->SetWin(WIN_CREATION);
    }

    switch (Opt(OPT_GENDER)) {
    case 0:
        do
            ch = MyTerm->ChoicePrompt("Is this character male or female?", "mf");
        while(ch == -1);
        if (ch == 'f')
            StateFlags |= MS_FEMALE;
        else
            StateFlags &= ~MS_FEMALE;
        break;
    case 1:
        StateFlags |= MS_FEMALE;
        break;
    case 2:
        StateFlags &= ~MS_FEMALE;
        break;
    case 3: // random, please!
        if (random(100) < 50)
            StateFlags |= MS_FEMALE;
        else 
            StateFlags &= ~MS_FEMALE;
        break;
    }

RepeatRace:
    MyTerm->Clear();
    MyTerm->ShowTraits(); MyTerm->ShowStatus();
    MyTerm->SetWin(WIN_CREATION);

    i = 0;
    for (q=0;q!=MAX_MODULES;q++)
        if (theGame->Modules[q]) {
            xID = xID=theGame->Modules[q]->RaceID(0);
            endID = xID + theGame->Modules[q]->szRac;
            for(;xID!=endID;xID++) {            
                /* This seems strange, but it needs to be here for the drow gender thing. */        
                rv = TRACE(xID)->PEvent(EV_PREREQ,this,this,xID);
                if (rv == ABORT)
                    continue;            
                if (rv != DONE)
                    if (TRACE(xID)->BaseRace)
                        continue;      
                Choices[i] = xID;
                MyTerm->LOption(NAME(xID),i,DESC(xID) ? 
                    DESC(xID) : "Description not found.");
                i++;
            }
        }
    if (!i)
        Fatal("No core races to choose from among the loaded resources!");
    i = (int16)MyTerm->LMenu(MENU_3COLS | MENU_DESC,
        "<15>  Your race grants you a number of special abilities in the game, "
        "including modifiers to your Attributes, additional skills and "
        "often potent supernatural powers. Newer players are recommended "
        "to take a hardy race that increases physical toughness as these "
        "require the least understanding of game nuances to play effectively "
        "and tend to increase character survivability. Orcs are a good choice, "
        "as are dwarves and lizardfolk. Elves make good archers. Small races "
        "have a unique set of challenges that may be difficult for a new "
        "player, but halflings start with items allowing safe rest, which "
        "can be valuable to a new player learning about the game. You can "
        "scroll through the descriptions of races using [PGUP] and [PGDN].",
        WIN_CREATION,"help::chargen,R");
    RaceID = Choices[i];
    if (!TRACE(RaceID)->mID) {
        MyTerm->Message("That race isn't implemented yet. Please choose another.");
        goto RepeatRace;
    }
    if (!Opt(OPT_SUBRACES))
        goto SkipSubraces;

    i = 0;
    Choices[i++] = RaceID;
    MyTerm->LOptionClear();
    MyTerm->LOption(Format("%s (Standard)",NAME(RaceID)), 0,
        DESC(RaceID) ? DESC(RaceID) : "Description Not Found.");
    for (q=0;q!=MAX_MODULES;q++)
        if (theGame->Modules[q]) {
            xID = xID=theGame->Modules[q]->RaceID(0);
            endID = xID + theGame->Modules[q]->szRac;
            for(;xID!=endID;xID++) {
                if (TRACE(xID)->BaseRace != RaceID)
                    continue;              
                /* This seems strange, but it needs to be here for the
                drow gender thing. */        
                rv = TRACE(xID)->PEvent(EV_PREREQ,this,this,xID);
                if (rv == ABORT)
                    continue;            
                Choices[i] = xID;
                MyTerm->LOption(NAME(xID),i,DESC(xID) ? 
                    DESC(xID) : "Description not found.");
                i++;
            }
        }
    if (i == 1) {
        MyTerm->LOptionClear();
        goto SkipSubraces;
    }
    i = (int16)MyTerm->LMenu(MENU_2COLS | MENU_DESC,
        "<15>__Subraces provide additional permutations of the existing "
        "set of races, opening new challenges and play styles for advanced "
        "players. The race you have chosen has one or more available subraces "
        "as shown in the list below. Unless otherwise noted, a subrace shares "
        "all traits with its base race. You can scroll through the descriptions "
        "of races using [PGUP] and [PGDN].", WIN_CREATION,"help::chargen,R");
    RaceID = Choices[i];


SkipSubraces:
    mID  = TRACE(RaceID)->mID;
    tmID = TRACE(RaceID)->mID;
    GainStatiFromBody(mID);
    resChance = (int16)TRACE(RaceID)->GetConst(RES_SURVIVAL_CHANCE);
    if (!resChance)
        resChance = 100;

    if (StateFlags & MS_FEMALE)
        Named = PickName(theGame->GetText(RaceID,TRACE(RaceID)->FNames));
    else
        Named = PickName(theGame->GetText(RaceID,TRACE(RaceID)->MNames));

    if (TRACE(RaceID)->SNames)
        Named += SC(" ") + PickName(theGame->GetText(RaceID,TRACE(RaceID)->SNames));

    for(i=0;i!=12;i++)
        if (TRACE(RaceID)->Skills[i] == SK_ANY) {
            b = HasFeat(FT_NATURAL_APTITUDE);
            GainFeat(FT_NATURAL_APTITUDE);
            if (!b)
                Feats[FT_NATURAL_APTITUDE/8] &= ~(1 << (FT_NATURAL_APTITUDE%8));
        }

    CalcValues(true);

    MyTerm->ShowTraits(); MyTerm->ShowStatus();
    MyTerm->SetWin(WIN_CREATION);

RepeatClass:
    i = 0;
    for (q=0;q!=MAX_MODULES;q++)
        if (theGame->Modules[q]) {
            xID = xID=theGame->Modules[q]->ClassID(0);
            endID = xID + theGame->Modules[q]->szCla;
            for(;xID!=endID;xID++)
            {
                if (TCLASS(xID)->HasFlag(CF_PRESTIGE))
                    continue;
                if (TCLASS(xID)->HasFlag(CF_PSEUDO))
                    continue;
                Choices[i] = xID;
                MyTerm->LOption(NAME(xID),i,DESC(xID) ? 
                    DESC(xID) : "Description not found.");
                i++;
            }
        }
    if (!i)
        Fatal("No classes to choose from among the loaded resources!");
    i = (int16)MyTerm->LMenu(MENU_3COLS | MENU_DESC,
        "<15>  Your class describes the fundamental abilities your character "
        "possesses, and will determine your overall strategy in the game. "
        "Incursion characters can become multi-classed later in the game, "
        "investing earned levels in up to three different classes in total. "
        "The easiest class for a completely new player is probably ranger. "
        "Mages and heavy-armour wearers can be more difficult to keep alive. "
        "For players who understand the skill system, magic system and "
        "religion system, rogue and priest are probably the easiest classes "
        "to win the game with. Human Warriors are useful for experimenting with the "
        "feat system, and any flavour of Rogue likewise for the skill system. "
        "Once a player understands the skill system, it is commonly believed "
        "that Rogue is the easiest class to complete the game with. "
        "In Incursion, every race/class combination is intended "
        "to be viable, though some may be less optimal. You can scroll the "
        "class descriptions using [PGUP] and [PGDN].", 
        WIN_CREATION, "help::chargen,C");
    ClassID[0] = Choices[i];
    if (!TCLASS(ClassID[0])->HitDie)
    {
        MyTerm->Message("That class isn't implemented yet. Please choose another.");
        goto RepeatClass;
    }
    if (TCLASS(ClassID[0])->PEvent(EV_ISTARGET,this,ClassID[0]) == ABORT) {
        MyTerm->Message("You do not meet the requirements for that class.");
        goto RepeatClass;
    } 
    CalcValues(true);
    MyTerm->ShowTraits(); MyTerm->ShowStatus();
    MyTerm->SetWin(WIN_CREATION);

    GodID = 0;

    if (TCLASS(ClassID[0])->HasFlag(CF_RELIGIOUS))
        ChooseGod(true);

    if (!Opt(OPT_ATTR_FIRST)) { 
        RollAttributes();
        CalcValues(true);
        MyTerm->ShowTraits(); MyTerm->ShowStatus();
        MyTerm->SetWin(WIN_CREATION);
    }

    if (RInf.usedPerks)
        GrantPerks(RInf.Perks,this);

    Level[0] = 0;

    for (i=0;i<9;i++) {
        if (TCLASS(ClassID[0])->HasFlag(CF_LAWFUL) &&
            !(AlignmentInfo[i].align & AL_LAWFUL)) 
            continue;
        if (TCLASS(ClassID[0])->HasFlag(CF_CHAOTIC) &&
            !(AlignmentInfo[i].align & AL_CHAOTIC)) 
            continue;
        if (TCLASS(ClassID[0])->HasFlag(CF_EVIL) &&
            !(AlignmentInfo[i].align & AL_EVIL)) 
            continue;
        if (TCLASS(ClassID[0])->HasFlag(CF_GOOD) &&
            !(AlignmentInfo[i].align & AL_GOOD)) 
            continue;

        if (TCLASS(ClassID[0])->HasFlag(CF_NONLAWFUL) &&
            (AlignmentInfo[i].align & AL_LAWFUL)) 
            continue;
        if (TCLASS(ClassID[0])->HasFlag(CF_NONCHAOTIC) &&
            (AlignmentInfo[i].align & AL_CHAOTIC)) 
            continue;
        if (TCLASS(ClassID[0])->HasFlag(CF_NONEVIL) &&
            (AlignmentInfo[i].align & AL_EVIL)) 
            continue;
        if (TCLASS(ClassID[0])->HasFlag(CF_NONGOOD) &&
            (AlignmentInfo[i].align & AL_GOOD)) 
            continue;

        if (GodID && TGOD(GodID)->HasFlag(GF_LAWFUL) &&
            !(AlignmentInfo[i].align & AL_LAWFUL)) 
            continue;
        if (GodID && TGOD(GodID)->HasFlag(GF_CHAOTIC) &&
            !(AlignmentInfo[i].align & AL_CHAOTIC)) 
            continue;
        if (GodID && TGOD(GodID)->HasFlag(GF_EVIL) &&
            !(AlignmentInfo[i].align & AL_EVIL)) 
            continue;
        if (GodID && TGOD(GodID)->HasFlag(GF_GOOD) &&
            !(AlignmentInfo[i].align & AL_GOOD)) 
            continue;      

        if (GodID && TGOD(GodID)->HasFlag(GF_NO_LAWFUL) &&
            (AlignmentInfo[i].align & AL_LAWFUL)) 
            continue;
        if (GodID && TGOD(GodID)->HasFlag(GF_NO_CHAOTIC) &&
            (AlignmentInfo[i].align & AL_CHAOTIC)) 
            continue;
        if (GodID && TGOD(GodID)->HasFlag(GF_NO_EVIL) &&
            (AlignmentInfo[i].align & AL_EVIL)) 
            continue;
        if (GodID && TGOD(GodID)->HasFlag(GF_NO_GOOD) &&
            (AlignmentInfo[i].align & AL_GOOD)) 
            continue;            

        MyTerm->LOption(AlignmentInfo[i].name,i,AlignmentInfo[i].desc);
    } 

    i = (int16)MyTerm->LMenu(MENU_3COLS|MENU_DESC,"__Alignment determines a character's "
        "moral and ethical outlook on the world. In the game, many different "
        "actions influence alignment, and alignment in turn impacts how gods "
        "react to your character, the prerequisites or certain classes, feats "
        "and divine spells and the attitude other characters take to you. Unless "
        "you have a high Bluff skill rating, your aligned actions are assumed "
        "to have generated a reputation for you, and NPCs will react accordingly."
        "Good, lawful and to a lesser extent chaotic alignments essentially "
        "operate as conducts in Incursion -- you must be very careful what actions "
        "you take, and you receive bonuses or holding to this code. As such, "
        "new players might find it best to play True Neutral or Neutral Evil "
        "characters until they have a firm grasp on what things good and lawful "
        "characters are proscribed from doing. Press '?' to view the manual "
        "section on alignments for more information.",
        WIN_CREATION,"help::adventuring,AC");
    GainPermStati(ALIGNMENT,this,SS_PERM,AlignmentInfo[i].align);
    if (AlignmentInfo[i].align & AL_LAWFUL)
        alignLC = -50;
    if (AlignmentInfo[i].align & AL_CHAOTIC)
        alignLC = 50;
    if (AlignmentInfo[i].align & AL_GOOD)
        alignGE = -50;
    if (AlignmentInfo[i].align & AL_EVIL)
        alignGE = 50;
    desiredAlign = AlignmentInfo[i].align;

    AdvanceLevel();

    CalcValues(true);
    MyTerm->ShowTraits(); MyTerm->ShowStatus();
    MyTerm->SetWin(WIN_CREATION);

    if (!reincarnate)
        MyTerm->SkillManager(true);
    else {
        memcpy(SkillRanks,RInf.SkillRanks,sizeof(int8)*(SK_LASTSKILL));
        CalcSP();
        SpentSP[0] = TotalSP[0];
        SpentSP[1] = TotalSP[1];
        SpentSP[2] = TotalSP[2];
        SpentSP[3] = TotalSP[3];
        SpentSP[4] = TotalSP[4];
        SpentSP[5] = TotalSP[5];
    }

    mHP = cHP = 0;
    mMana = uMana = 0;
    CalcValues(true);
    MyTerm->ShowTraits(); MyTerm->ShowStatus();
    MyTerm->SetWin(WIN_CREATION);

    GainTempStati(HUNGER,NULL,SATIATED-10,SS_MISC);

    if (GodID)
        RES(GodID)->PEvent((PRE(EV_BIRTH)),this,ClassID[0]);
    TRACE(RaceID)->PEvent(PRE(EV_BIRTH),this,RaceID);
    TCLASS(ClassID[0])->PEvent(PRE(EV_BIRTH),this,ClassID[0]);
    RES(FIND("Universal Gear"))->PEvent(PRE(EV_BIRTH),this,FIND("Universal Gear"));


    TRACE(RaceID)->GrantGear(this,RaceID);
    TCLASS(ClassID[0])->GrantGear(this,ClassID[0]);

    if (FIND("Beginner's Kit"))
        if (Opt(OPT_BEGINKIT))
            RES(FIND("Beginner's Kit"))->GrantGear(this,FIND("Beginner's Kit"));

    if (FIND("Universal Gear"))
        RES(FIND("Universal Gear"))->GrantGear(this,FIND("Universal Gear"));



    /* ww: if you have weapon focus in a weapon or skill with an exotic
    * weapon, you get one to start the game -- so a rogue might start with
    * a sap, for example */
    StatiIterNature(this,WEP_SKILL)
        TItem * wep = TITEM(S->eID);
        if ((S->Val >= WS_FOCUSED || wep->Group & WG_EXOTIC) && !wep->HasFlag(IT_NOGEN)) {
            Item * item;

            /* An orc might not be strong enough to use the shotput. */
            if (WepSkill(S->eID) == WS_NOT_PROF &&
                S->Source == SS_RACE)
                goto SkipThisFocus;

            for (item=FirstInv();item;item=NextInv())
                if (item->iID == S->eID)
                    goto SkipThisFocus;

            item = Item::Create(S->eID);
            item->MakeKnown(-1);
            /* ww: halflings focus with the 'rock' which counts as a missile.
            * But having just one rock isn't that exciting. */
            if (item->isType(T_MISSILE))
                item->SetQuantity(80 + random(20));
            else if (item->isGroup(WG_THROWN))
                item->SetQuantity(4 + random(4));
            GainItem(item,true);
SkipThisFocus:;
        }
    StatiIterEnd(this)

    /* ww: give ammo for any bow you might have! */
    Item *bows[6], *bow; int16 c = 0;
    for (bow = FirstInv(); bow != NULL; bow = NextInv()) {
        if (!(bow->isType(T_BOW))) continue; 
        bows[c++] = bow;
        if (c == 6)
            break;
    }
    for (i=0;i!=c;i++) {
        bow = bows[i];

        /* find the first ammo it can fire */
        int ammoID = TITEM(bow->iID)->FirstRes(AN_FIRES);
        if (ammoID <= 0) continue;

        Item * ammo = Item::Create(ammoID);
        /* 2d10 or whatnot is a bad idea here: no one would go into the
        * dungeon with one bow and only 3 arrows ...
        * FJM: the problem here is that rogues start with poisoned arrows,
        * and 180 arrows equals out to a lifetime supply of poison! Bad for
        * balance. Fourty arrows seems roughly like a sensible "full quiver"
        * anyway. */
        ammo->SetQuantity(30 + random(11));
        ammo->MakeKnown(-1);
        GainItem(ammo,true);
    }

    if (HasFeat(FT_CAPTIVATING))
    {
        Item *gold = Item::Create(FIND("gold piece"));
        gold->SetQuantity(250);
        GainItem(gold,true);
    }

#if 0
    for (q=0;q!=MAX_MODULES;q++)
        if (theGame->Modules[q]) {
            xID = theGame->Modules[q]->ItemID(0);
            endID = xID + theGame->Modules[q]->szItm;
            for(;xID!=endID;xID++) {
                if(RES(xID)->Type == T_TITEM && TITEM(xID)->IType == T_SHIELD &&
                    TITEM(xID)->Size <= Attr[A_SIZ] && 
                    WepSkill(xID) != WS_NOT_PROF && TITEM(xID)->Cost <= 500*100) { 
                        Armour * a = (Armour *)Item::Create(xID); 
                        a->MakeKnown(-1);
                        GainItem(a,true);
                        goto DoneShield; 
                }
            }
        }
#endif

//DoneShield:
    if (GodID)
        RES(GodID)->PEvent((EV_BIRTH),this,ClassID[0]);
    TRACE(RaceID)->PEvent(EV_BIRTH,this,RaceID);
    TCLASS(ClassID[0])->PEvent(EV_BIRTH,this,ClassID[0]);
    RES(FIND("Universal Gear"))->PEvent(EV_BIRTH,this,FIND("Universal Gear"));

    TextVal SkillKits[] = {
        { SK_HEAL,        "healing kit" },
        { SK_DISGUISE,    "disguise kit" },
        { SK_ALCHEMY,     "alchemy set" },
        { SK_MINING,      "pickaxe" },
        { SK_CRAFT,       "craftsman's tools" },
        { SK_HANDLE_DEV,  "mechanical tools" },
        //{ SK_CLIMB,       "climbing rig" },
        //{ SK_LOCKPICKING, "lockpicks" },
        { 0, NULL } };
    for (i=0;SkillKits[i].Val;i++)
        if (SkillRanks[SkillKits[i].Val]) {
            Item * it = Item::Create(FIND(SkillKits[i].Text));
            GainItem(it,true);
        }

    /* ww: if you start the game with a healing potion, you can recognize
    * healing potions. */
    for (Item *inv = FirstInv(); inv != NULL; inv = NextInv()) {
        inv->MakeKnown(-1);
        if (GodID)
            RES(GodID)->PEvent(EV_STARTING_ITEM,this,inv,ClassID[0]);
        TRACE(RaceID)->PEvent(EV_STARTING_ITEM,this,inv,RaceID);
        TCLASS(ClassID[0])->PEvent(EV_STARTING_ITEM,this,inv,ClassID[0]);
        /*
        if (GodID && inv->isType(T_SHIELD) && !inv->eID) {
        char buf[1024];
        sprintf(buf,"%s;symbol",NAME(GodID));
        rID eID = FIND(buf);
        ASSERT(eID);
        inv->eID = eID; 
        }
        */ 
    }

    // ww: currently the personality types, while cute and reminding one of
    // the Enneagram, don't serve a purpose. Let's replace this with an
    // alignment choice, which does matter (monsters have unholy weapons,
    // there are magic circles vs evil, etc.). 
    for(i=0;PersonalityNames[i];i++)
        MyTerm->LOption(PersonalityNames[i],i);
    Personality = MyTerm->LMenu(MENU_2COLS,
        "<15>__Your personality archetype has little direct influence on "
        "gameplay and is largely a 'colour' choice. In later releases, it "
        "will strongly colour how NPCs relate to you, as well as affecting "
        "psionic combat, the Greater Soulblade feat and follower loyalty.",
        WIN_CREATION,"help::chargen,P");

    /* ww: unlikely, but it could happen ... */
    if (isMType(MA_DEMON) || isMType(MA_LYCANTHROPE) || isMType(MA_DEVIL) || isMType(MA_UNDEAD))
        for(Item *it=FirstInv();it;it=NextInv()) {
            if (it->Material() == MAT_SILVER)
                it->RemoveQuality(IQ_SILVER);
            if (it->Material() == MAT_SILVER)
                it->Remove(true);
            if (it->isBlessed())
                it->IFlags &= ~IF_BLESSED; 
        } 

    if (onPlane() != PHASE_MATERIAL)
        for(Item *it=FirstInv();it;it=NextInv())
            it->GainPermStati(PHASED,NULL,SS_MISC,onPlane());

    if (getPrimaryMelee() && (InSlot(SL_WEAPON) != getPrimaryMelee()) && !getPrimaryMelee()->isGroup(WG_DAGGERS)) {
        Item *w, *w2;
        w = getPrimaryMelee();
        w = w->TakeOne();
        w2 = InSlot(SL_WEAPON);
        SetSilence();
        w->Remove(false);
        if (w2)
            w2->Remove(false);
        ThrowVal(EV_WIELD,SL_WEAPON,this,NULL,w);
        UnsetSilence();
        if (w->Owner() != this)
            GainItem(w,true);
        if (w2 && w2->Owner() != this)
            GainItem(w2,true);
    }

    while (SpellSlots[0] + BonusSlots[0] > SpellsLearned[0])
        LearnSpell(true);

    CalcValues();
    SetImage();
    // ww: mages start with extra mana items, so fix up the total
    cHP = mHP + KAttr[A_THP]; 
    LastRest = theGame->GetTurn();

    /* Hist.Create(this,MyTerm); */
    MyTerm->SetMode(MO_PLAY);

    time(&tm);
    storeSeed = (int32)tm;

    if (reincarnate) {
        Named = RInf.Name;
        WizardMode = RInf.WizMode;
        ExploreMode = RInf.ExpMode;
        storeSeed = RInf.storeSeed;
        rerolledPerks = RInf.rerolledPerks;
        rerollCount = RInf.rerollCount;
        statMethod = (int16)RInf.statMethod;
        MyTerm->ShowStatus();
        MyTerm->ShowTraits();
    } else {
        ReincarnationInfo *ri;
        size_t sz; 
        RInf.RaceID = RaceID;
        RInf.ClassID = ClassID[0];
        RInf.storeSeed = storeSeed;
        RInf.WizMode = WizardMode;
        RInf.ExpMode = ExploreMode;
        memcpy(RInf.SkillRanks,SkillRanks,sizeof(int8)*SK_LASTSKILL);
        memcpy(RInf.CharGenOpts,&(Options[OPC_CHARGEN]),sizeof(int8)*100);

        strncpy(RInf.Name,Named,31);
        for(i=0;i!=7;i++)
            RInf.Attr[i] = (int8)BAttr[i];
        time(&RInf.Touched);
        time(&RInf.Created);
        MyTerm->ChangeDirectory(
            MyTerm->SaveSubDir());
        if (MyTerm->Exists("gallery.dat")) {
            MyTerm->OpenRead("gallery.dat");
            MyTerm->Seek(0,SEEK_END);
            sz = MyTerm->Tell() / sizeof(ReincarnationInfo);
            ASSERT(sz);
            ri = new ReincarnationInfo[sz + 1]; 
            MyTerm->Seek(0,SEEK_SET);   
            MyTerm->FRead(ri,sz * sizeof(ReincarnationInfo));
            MyTerm->Close();

            if (sz >= MAX_GALLERY_SIZE) {
                int16 o_index; time_t oldest;
                /* Find the oldest, or any disabled */
                time(&oldest);
                for (i=0;i!=sz;i++) {
                    if (difftime(oldest,ri[i].Touched) >= 1)
                    { oldest = ri[i].Touched; o_index = i; }
                    if (ri[i].Disabled)
                        o_index = i;
                }

                /* Overwrite the Oldest */
                memcpy(&ri[o_index],&RInf,sizeof(ReincarnationInfo));
                GallerySlot = o_index;
            } else {
                memcpy(&ri[sz],&RInf,sizeof(ReincarnationInfo));
                GallerySlot = sz;
                sz++;
            }
        } else {
            ri = new ReincarnationInfo[1];
            memcpy(&ri[0],&RInf,sizeof(ReincarnationInfo));
            sz = 1;
            GallerySlot = 0;
        }
        MyTerm->OpenWrite("gallery.dat");
        MyTerm->FWrite(ri,sz * sizeof(ReincarnationInfo));
        MyTerm->Close();
        delete[] ri;
    }
}

static void NamePerk(Perk& p, String& str);

bool Player::Reincarnate()
{
    String desc, ps; int16 sz,i, j;
    ReincarnationInfo *ri;
    bool found;
    MyTerm->ChangeDirectory(
        MyTerm->SaveSubDir());
    if (!MyTerm->Exists("gallery.dat"))
    {
        IPrint("You need to turn the Use Reincarnation Gallery option on "
            "and then play one or more characters before you can "
            "reincarnate a character.");  
        return false;
    }
    MyTerm->OpenRead("gallery.dat");
    MyTerm->Seek(0,SEEK_END);
    sz = (int16)(MyTerm->Tell() / sizeof(ReincarnationInfo));
    MyTerm->Seek(0,SEEK_SET);
    ri = new ReincarnationInfo[sz];
    MyTerm->FRead(ri,sz * sizeof(ReincarnationInfo));
    MyTerm->Close();

    found = false;
    for (i=0;i!=sz;i++)
    {
        if (ri[i].Disabled)
            continue;
        found = true;
        desc = Format("%c%s -- %s %s%c\n",-YELLOW,ri[i].Name,
            NAME(ri[i].RaceID), NAME(ri[i].ClassID), -GREY);
        if (ri[i].Active)
            desc += "[ In Play ]\n";
        else {
            desc += "Last Played: ";
            desc += ctime(&ri[i].Touched);
        }
        for (j=0;j!=7;j++)
            desc += Format("%s %c%d%c%s ",
            AttrTitle[j],
            ri[i].Attr[j] >= 16 ? -EMERALD : -GREY,
            ri[i].Attr[j],-GREY,
            j == 6 ? ".\n" : ",");
        for (j=0;j!=3;j++)
            if (ri[i].Perks.elt[j].type != PERK_NONE)
            {
                NamePerk(ri[i].Perks.elt[j],ps);
                desc += ps;
                desc += "\n";
            }
            desc += "Created: ";
            desc += ctime(&ri[i].Created);
            desc += Format("Reincarnated %d times to date.", ri[i].Count);

            MyTerm->LOption(ri[i].Name,i,desc);
    }

    if (!found) {
        IPrint("All of the characters in your gallery are either "
            "currently being played, or have advanced beyond the level "
            "suitable for reincarnation and then died. You'll have to "
            "make a new character from scratch, or load a saved game.");
        delete[] ri;
        return false;  
    }
    i = (int16)MyTerm->LMenu(MENU_DESC|MENU_ESC|MENU_BORDER|MENU_WIDE|MENU_2COLS,"Choose a character:",WIN_MENUBOX);

    if (i == -1)
    {
        delete[] ri;
        return false;
    }

    /*
    if (ri[i].Active)
    {
    IPrint("You're already playing one incarnation of that character.");
    return false;
    }
    */


    memcpy(&RInf,&ri[i],sizeof(ReincarnationInfo));


    ri[i].Count++;
    if (ri[i].Count >= 9)
        ri[i].Disabled = true;

    ri[i].Active = true;
    time(&ri[i].Touched);
    MyTerm->OpenWrite("gallery.dat");
    MyTerm->FWrite(ri,sz * sizeof(ReincarnationInfo));
    MyTerm->Close();


    delete[] ri;


    GallerySlot = i;
    Create(true);

    Named += " ";
    Named += RomanNumerals[min(10,RInf.Count+2)];


    return true;

}  

void Player::TouchGallery(bool active)
{
    ReincarnationInfo *ri; int16 sz;

    if (GallerySlot < 0)
        return;
    MyTerm->ChangeDirectory(
        MyTerm->SaveSubDir());
    if (!MyTerm->Exists("gallery.dat"))
        return;

    MyTerm->OpenRead("gallery.dat");
    MyTerm->Seek(0,SEEK_END);
    sz = (int16)(MyTerm->Tell() / sizeof(ReincarnationInfo));
    MyTerm->Seek(0,SEEK_SET);
    ri = new ReincarnationInfo[sz];
    MyTerm->FRead(ri,sz * sizeof(ReincarnationInfo));
    MyTerm->Close();

    ri[GallerySlot].Active = active;
    time(&ri[GallerySlot].Touched);

    MyTerm->OpenWrite("gallery.dat");
    MyTerm->FWrite(ri,sz * sizeof(ReincarnationInfo));
    MyTerm->Close();    

}

void Player::DisableReincarnation()
{
    int16 sz; ReincarnationInfo *ri;

    /* Char not in gallery */
    if (GallerySlot == -1)
        return;

    MyTerm->ChangeDirectory(
        MyTerm->SaveSubDir());
    if (!MyTerm->Exists("gallery.dat"))
        return;
    MyTerm->OpenRead("gallery.dat");
    MyTerm->Seek(0,SEEK_END);
    sz = (int16)(MyTerm->Tell() / sizeof(ReincarnationInfo));
    MyTerm->Seek(0,SEEK_SET);
    ri = new ReincarnationInfo[sz];
    MyTerm->FRead(ri,sz * sizeof(ReincarnationInfo));
    MyTerm->Close();

    ASSERT(sz >= GallerySlot);
    ri[GallerySlot].Disabled = true;

    MyTerm->OpenWrite("gallery.dat");
    MyTerm->FWrite(ri,sz * sizeof(ReincarnationInfo));
    MyTerm->Close();    

    delete[] ri;

}

int16 AttrDice(int16 dice) {
    static int16 roll[12], best[3], i;
    for(i = 0; i != dice; i++)
        roll[i] = random(6) + 1;

    best[0] = 0;
    best[1] = 1;
    best[2] = 2;

    for (i=0;i!=dice;i++) {
        if (roll[i] > roll[best[0]] && i != best[1] && i != best[2])
            best[0] = i;
        if (roll[i] > roll[best[1]] && i != best[0] && i != best[2])
            best[1] = i;
        if (roll[i] > roll[best[2]] && i != best[0] && i != best[1])
            best[2] = i;
    }

    return roll[best[0]] + roll[best[1]] + roll[best[2]];
}

static char * DesiredPerk = NULL;

static void NamePerk(Perk& p, String& str) {
    String s2; Item *it;
    switch (p.type) {
    case PERK_ITEM: 
        it = Item::Create(p.p.i.iID);
        it->MakeMagical(p.p.i.eID,p.p.i.Plus);
        it->IFlags &= (~IF_CURSED);
        for (int16 q=0;q!=7;q++)
            if (p.p.i.q[q])
                it->AddQuality((int8)p.p.i.q[q]);
        if (p.p.i.Bane)
            it->SetBane(p.p.i.Bane); 
        if (it->isType(T_MISSILE))
            it->SetQuantity(30);
        s2 = it->Name(NA_MECH|NA_IDENT|NA_MONO|NA_NO_TERSE);
        delete it;
        s2.Replace("uncursed ","");
        str = Format("Starting Item: %s", (const char*) s2);
        return;
    case PERK_FEAT: 
        str = Format("   Bonus Feat: %s",FeatName(p.p.feat)); 
        return; 
    case PERK_ABILITY: 
        if (p.p.ability.ca_ability == CA_FAV_ENEMY)
            str = Format("Favoured Enemy: %s",(const char*)Pluralize(Capitalize(
            Lookup(MTypeNames,p.p.ability.ability_value & 0xffff))));
        else {
            str = Format("Extra Ability: %s",Lookup(ClassAbilities,
                p.p.ability.ca_ability)); 
            if (p.p.ability.ca_ability == CA_SMITE) {
                str += " "; str += Pluralize(Capitalize(Lookup(MTypeNames,
                    p.p.ability.ability_value)));
            }
            else if (p.p.ability.ability_value > 1)
                str += Format(" %+d",p.p.ability.ability_value);
        }
        return; 
    case PERK_SPELL:   
        str = Format(" Innate Spell: %s", NAME(p.p.spell)); 
        return; 
    case PERK_TALENT:   
        str = Format("  Wild Talent: %s", NAME(p.p.spell)); 
        return; 
    case PERK_RESIST: 
        if (p.p.resist < 0)
            str = Format("   Save Bonus: +4 vs. %s", (const char*)
            Capitalize(Lookup(SaveBonusNames, -p.p.resist))); 
        else
            str = Format("   Resistance: %s +10", Lookup(DTypeNames, p.p.resist)); 
        return; 
    case PERK_IMMUNE: 
        str = Format("     Immunity: %s", Lookup(DTypeNames, p.p.immune)); 
        return; 

    case PERK_NONE: str = "None?"; return; 
    } 
}

int16 MaxItemPlus(int16 maxlev, rID eID);

void CreatePerk(PerkSet& Perks, int i, PerkType type, Player *p)
{
    int redo = 0, others;

    Perks.elt[I(i,3)].type = type; 

    switch (type) {

    case PERK_IMMUNE: 
        do { 
            redo = 0; 
            int possibleImmune[7] = { 
                AD_TOXI, // "Poison" },
                AD_DISE, // "Disease" },
                AD_PLYS,
                AD_STUK,
                AD_CONF,
                AD_SLEE,
                AD_FEAR, // "Fear" },
            } ; 
            Perks.elt[I(i,3)].p.resist = possibleImmune[random(7)]; 
            for (others = 0; others < i; others++) 
                if (Perks.elt[I(others,3)].type == PERK_IMMUNE &&
                    Perks.elt[I(others,3)].p.immune == Perks.elt[I(i,3)].p.immune)
                    redo = 1;
        } while (redo); 
        break; 

    case PERK_RESIST: 
        do { 
            redo = 0; 
            int possibleResists[] = { 
                AD_FIRE, // "Fire" },
                AD_COLD, // "Cold" },
                AD_ACID, // "Acid" },
                AD_ELEC, // "Lightning" },
                AD_NECR, // "Life Drain" },
                AD_MAGC, // "Pure Magic" },
                AD_SONI, // "Sonic Damage" },
                AD_SUNL, // "Light Damage" },
                AD_DREX, // "Experience Drain" },
                /* fjm: Add bonuses to saving throws along with
                elemental resistances. Wes had AD_FEAR here,
                which doesn't work as an elemental resistance
                because there's no damage number. */
#ifndef WEIMER
                -SN_DISEASE, // Save vs. Disease
                -SN_EVIL,    // Save vs. Evil Magic
                -SN_PETRI,   // Save vs. Petrification
                -SN_POISON,  // Save vs. Poison
                -SN_ENCH,    // Save vs. Enchantment
                -SN_TRAPS,   // Save vs. Traps
                -SN_FEAR,    // Save vs. Fear
                -SN_ILLUS,   // Save vs. Illusion
                -SN_CONF,    // Save vs. Confusion
                -SN_COMP,    // Save vs. Compulsion
                -SN_THEFT,   // Save vs. Theft
                -SN_DISINT,  // Save vs. Disint
#endif
            } ; 
            Perks.elt[I(i,3)].p.resist = possibleResists[random(
                sizeof(possibleResists) / 
                sizeof(possibleResists[0])
                )]; 
            for (others = 0; others < i; others++) 
                if (Perks.elt[I(others,3)].type == PERK_RESIST &&
                    Perks.elt[I(others,3)].p.resist == Perks.elt[I(i,3)].p.resist)
                    redo = 1;

            /* fjm: Don't give away things gotten from race/class */
            if (Perks.elt[I(i,3)].p.resist > 0)
                if (p->ResistLevel((int16)Perks.elt[I(i,3)].p.resist) != 0)
                    redo = 1;
            if (Perks.elt[I(i,3)].p.resist < 0)
                if (p->HasStati(SAVE_BONUS,(int16)-Perks.elt[I(i,3)].p.resist))
                    redo = 1;

        } while (redo); 
        break; 

        /* start with a random item that might be found around depth 50' */
    case PERK_ITEM: 
        while (1) {
            Item *it = Item::GenItem(IG_KNOWN|IG_GOOD|IG_GREAT,0,5,(int8)p->Attr[A_LUC],DungeonItems);
            TItem *ti = TITEM(it->iID);

            redo = 0;
            if (it->isCursed())
                redo = 1;
            else if (it->Name(NA_LONG).GetLength() > 48) 
                redo = 1; 
            /* fjm: Why no shields? */
            else if (ti->IType == T_POTION || ti->IType == T_SCROLL ||
                ti->IType == T_TOME || ti->IType == T_FOUNTAIN /*||
                                                               ti->IType == T_THRONE || ti->IType == T_SHIELD*/) 
                                                               redo = 1;
            else if (it->ItemLevel() < 3) 
                redo = 1; 
            else if (it->eID && TEFF(it->eID)->HasFlag(EF_CURSED))
                redo = 1; 
            else if (p->ClassID[0] && TCLASS(p->ClassID[0])->HasFlag(CF_PALADIN) 
                && (it->HasQuality(WQ_UNHOLY) || it->HasQuality(WQ_CHAOTIC) ||
                it->HasQuality(WQ_CORROSIVE) || it->HasQuality(WQ_VAMPIRIC) ||
                it->HasQuality(WQ_SOULDRINK) || it->HasQuality(WQ_BALANCE) ||
                (it->eID && TEFF(it->eID)->HasFlag(EF_EVIL)) ||
                (it->eID && TEFF(it->eID)->HasFlag(EF_CHAOTIC))))
                redo = 1;
            else if (p->RaceID && p->isSmallRace() && it->Size() >= SZ_LARGE)
                redo = 1; 
            for (others = 0; others < i; others++) 
                if (Perks.elt[I(others,3)].type == PERK_ITEM &&
                    TITEM(Perks.elt[I(others,3)].p.i.iID)->IType == 
                    ti->IType)
                    redo = 1;

            /* fjm: just don't _give out_ unusable weapons */
            if (it->isType(T_WEAPON) || it->isType(T_BOW) || it->isType(T_MISSILE)
                || it->isType(T_ARMOUR) || it->isType(T_SHIELD) ) {
                    /* ww: this check just isn't working: it always returns false!
                    if (p->WepSkill(it->iID) == WS_NOT_PROF)
                    */
                    // redo = 1;

                    it->SetInherentPlus(min(3,it->GetInherentPlus()));

                    while ((!it->eID) && it->ItemLevel() <= 5 &&
                        it->GetPlus() < 5)
                        it->SetInherentPlus(it->GetInherentPlus()+1);

                    if (it->isType(T_MISSILE))
                        it->SetQuantity(30);

            }
            if (redo) {
                delete it; 
            } else { 
                /* fjm: +1 wands as being equal to bonus feats and innate
                resistances? I think the plus should be better. */
                if (it->GetPlus() && it->eID && !it->isType(T_WEAPON) &&
                    !it->isType(T_ARMOUR) &&
                    !it->isType(T_BOW) &&
                    !it->isType(T_MISSILE)) 
                    it->SetInherentPlus((int8)MaxItemPlus(6,it->eID));
                else if (it->GetPlus() && it->ItemLevel() <= 5)
                    it->SetInherentPlus(it->GetPlus()+1);
                Perks.elt[I(i,3)].p.i.iID    = it->iID;
                Perks.elt[I(i,3)].p.i.Plus   = it->GetInherentPlus();
                Perks.elt[I(i,3)].p.i.eID    = it->eID;
                for (int16 q=0;q!=8;q++)
                    Perks.elt[I(i,3)].p.i.q[I(q,8)] = it->GetQuality(i);
                Perks.elt[I(i,3)].p.i.Bane   = it->GetBane();
                delete it;
                break; 
            }
        } 
        break;

        /* start with a random feat */
    case PERK_FEAT: 
        do {
            redo = 0; 

            Perks.elt[I(i,3)].p.feat = random(FT_LAST - FT_FIRST) + FT_FIRST;
            if (FeatUnimplemented(Perks.elt[I(i,3)].p.feat))
                redo = 1; 

            for (others=0; badStartingFeats[others] ; others++)
                if (Perks.elt[I(i,3)].p.feat == badStartingFeats[others])
                    redo = 1; 

            for (others = 0; others < i; others++) 
                if (Perks.elt[I(others,3)].type == PERK_FEAT &&
                    Perks.elt[I(others,3)].p.feat == Perks.elt[I(i,3)].p.feat)
                    redo = 1;

            /* fjm: Check the prerequisites! */
            if (!p->FeatPrereq(Perks.elt[I(i,3)].p.feat))
                redo = 1;

        } while (redo); 
        break; 

        /* start with a class ability */
    case PERK_ABILITY: 
        do {
            int16 c;
            redo = 0; 
            for (c=0;okStartingAbils[c].ability_value;c++)
                ;
            Perks.elt[I(i,3)].p.ability = 
                okStartingAbils[random(c)];

            for (others = 0; others < i; others++) 
                if (Perks.elt[I(others,3)].type == PERK_ABILITY &&
                    Perks.elt[I(others,3)].p.ability.ca_ability == 
                    Perks.elt[I(i,3)].p.ability.ca_ability)
                    redo = 1;

            /* fjm: Don't give away things gotten from race/class */
            if (p->HasAbility(Perks.elt[I(i,3)].p.ability.ca_ability))
                redo = 1;
        } while (redo); 
        break; 

        /* start with the ability to cast some spell inherently */
    case PERK_SPELL: 
        /* start with a single psionic wild talent */
    case PERK_TALENT:
        do {
            redo = 0; 
            // ww: hack, one module
            Module * m = theGame->Modules[0];
            int idx = random(m->szEff);

            rID id = m->EffectID(idx);

            TEffect * eff = TEFF(id);

            if (!eff) redo = 1; 

            /* getting the "resist cold" spell always seems innately inferior
            * to just getting permanent cold resistance ... and other buffs
            * like mage armour can be very strong when cast with 0% fail
            * by fighters ... */
            if (type == PERK_SPELL)
                if (eff->Purpose & EP_BUFF ||
                    eff->Purpose & EP_RESIST) redo = 1; 

            if (eff->ef.eval == EA_NOTIMP) redo = 1; 

            if (type == PERK_SPELL)
                if (eff->ManaCost <= 0 || eff->BaseChance <= 0) redo = 1; 

            if (type == PERK_SPELL)
                if (!eff->HasSource(AI_WIZARDRY) && 
                    !eff->HasSource(AI_THEURGY) && 
                    !eff->HasSource(AI_DRUIDIC))
                    redo = 1;

            if (type == PERK_TALENT)
                if (!eff->HasSource(AI_PSIONIC))
                    redo = 1;

            if (type == PERK_SPELL)
                if (eff && eff->Level <= 0) redo = 1; 

            Perks.elt[I(i,3)].p.spell = id;

            for (others = 0; others < i; others++) 
                if ((Perks.elt[I(others,3)].type == PERK_SPELL || 
                    Perks.elt[I(others,3)].type == PERK_TALENT) &&
                    Perks.elt[I(others,3)].p.spell == Perks.elt[I(i,3)].p.spell)
                    redo = 1;

        } while (redo); 
        break; 

    default: 
        break; 


    } 
}

static void GrantPerks(PerkSet& p, Player *target)
{
    int32 i; Item *it; int16 nPerks;
    String journal;

    nPerks = 2;
    if (TRACE(target->RaceID)->BaseRace)
        nPerks--;
    if (target->isMType(MA_HUMAN))
        nPerks++;

    journal = "Starting Perks:";
    for (i=0; i<NUM_PERKS_PER_SET; i++) {
        String this_one;
        if (p.elt[I(i,3)].type == PERK_NONE)
            continue;
        if (i >= nPerks)
            continue;
        NamePerk(p.elt[I(i,3)], this_one);
        journal += Format("\n  %s.",(const char*)this_one);

        switch (p.elt[I(i,3)].type) {
      case PERK_ITEM: {
          it = Item::Create(p.elt[I(i,3)].p.i.iID);
          ASSERT(it->GetHP() == it->MaxHP());
          it->MakeMagical(p.elt[I(i,3)].p.i.eID,
              p.elt[I(i,3)].p.i.Plus);
          ASSERT(it->GetHP() == it->MaxHP());
          for (int16 q=0;q!=7;q++)
              if (p.elt[i].p.i.q[q])
                  it->AddQuality((int8)p.elt[I(i,3)].p.i.q[q]);
          if (p.elt[I(i,3)].p.i.Bane)
              it->SetBane(p.elt[I(i,3)].p.i.Bane); 
          ASSERT(it->GetHP() == it->MaxHP());
          if (it->isType(T_MISSILE))
              it->SetQuantity(30);
          target->GainItem(it,true);
#if 0 
          rID iID = p.elt[i].p.item->iID;
          /* fjm: Giving starting mages a +3 flaming greatsword AND
          proficiency in its use is just too excessive, especially
          when warriors still have to struggle with poor Use Magic
          on their shiny new wand. If you get a nice weapon as a
          perk, you have the option to invest feats in it, or to
          take a level of warrior. */
          if (TITEM(iID)->IType == T_WEAPON ||
              TITEM(iID)->IType == T_BOW ||
              (TITEM(iID)->IType == T_ARMOUR &&
              TITEM(iID)->Group))
              target->GainPermStati(WEP_SKILL,NULL,SS_PERM,WS_PROFICIENT,0,iID);
#endif
                      } break;

      case PERK_FEAT: target->GainFeat(p.elt[I(i,3)].p.feat,0); break;

      case PERK_ABILITY: target->GainAbility(p.elt[I(i,3)].p.ability.ca_ability,
                             p.elt[I(i,3)].p.ability.ability_value,0,SS_PERM); break;

      case PERK_SPELL:
      case PERK_TALENT: 
          target->GainAbility(CA_INNATE_SPELL,p.elt[I(i,3)].p.spell,0,SS_PERM); break;

      case PERK_RESIST: 
          if (p.elt[i].p.resist < 0)
              target->GainPermStati(SAVE_BONUS,NULL,SS_PERM,(int16)-p.elt[I(i,3)].p.resist,4,0);
          else
              target->GainPermStati(RESIST,NULL,SS_PERM,(int16)p.elt[I(i,3)].p.resist,10,0);
          break; 

      case PERK_IMMUNE: 
          target->GainPermStati(IMMUNITY,NULL,SS_PERM,(int16)p.elt[I(i,3)].p.immune,0,0);
          break; 
        } 
    } 

    target->AddJournalEntry(journal);
    return; 
} 

void Player::RollAttributes() {
    int16 nPerks, ch, wanted;
    bool do_redraw = false, perk_warn = 0;
    int8 BestStats[7][5], i, j, k, l, c, tot, Points, PCost;
    int8 PointCosts[] = { 0, 0, 0, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 8, 10, 13, 16 };
    PerkSet PerkSets[5];

    if (MyTerm->GetMode() == MO_RECREATE) {
        for (i = 0; i != 7; i++)
            BAttr[i] = RInf.Attr[i];
        return;
    }

    MyTerm->SetMode(MO_ROLLING);

    if (RaceID) {
        nPerks = 2;
        if (isMType(MA_HUMAN))
            nPerks = 3;
        if (TRACE(RaceID)->BaseRace)
            nPerks = 1;
    } else 
        nPerks = 3;

    memset(PerkSets, 0, sizeof(PerkSets));
    int linesPerSet = 3 + nPerks;
    statMethod = Opt(OPT_STATS);

Redraw:;
    MyTerm->SetWin(WIN_CREATION);
    MyTerm->Clear();

    uint32 milli0[5];
Reroll:;
    switch(statMethod) {
    case 0: linesPerSet = 3; /* 4d6 in order */
    case 1: /* 4d6 + Perks */

MethodI:
        MyTerm->Clear(); 
        MyTerm->Color(WHITE);
        MyTerm->Write(1,1,"Rolling attributes...");
        MyTerm->Write(2,3+linesPerSet*5,"[p]     Abort to Point Buy");
        if (Opt(OPT_REROLL)) {
            MyTerm->Write(2,3+linesPerSet*5+1,"[x]     Reroll Five More Sets");
            if (statMethod == 1)
                MyTerm->Write(2,3+linesPerSet*5+2,"[w]     Reroll Without Perks");
        }

        for (i=0,j=0;i!=5;i++) {
            if (do_redraw)
                goto JustDrawIt;

            milli0[i] = MyTerm->GetElapsedMilli();
RerollThisOne: 
            MyTerm->Color(BLUE+i);
            MyTerm->Write(10,3+linesPerSet*i,"STR DEX CON INT WIS CHA LUC");
            MyTerm->Write(2,4+i*linesPerSet,Format("[%c]",'a'+i));

            switch (Opt(OPT_POWER_STATS)) {
            case 0: wanted = 75; break;
            case 1: wanted = 100; break;
            case 2: wanted = 115; break;
            case 3: wanted = 125; break;
            }

            do {
                for (k = 0; k != 7; k++)
                    Attr[k] = BAttr[k] = AttrDice(4);

                if (MyTerm->GetElapsedMilli() - milli0[i] < 500)
                    if (Opt(OPT_ANIMATION) != 2) {
                        MyTerm->Write(10,4+linesPerSet*i,
                            Format("%2d  %2d  %2d  %2d  %2d  %2d  %2d  ", BAttr[A_STR],
                            BAttr[A_DEX], BAttr[A_CON], BAttr[A_INT], 
                            BAttr[A_WIS], BAttr[A_CHA], BAttr[A_LUC]));
                        MyTerm->Update();
                        MyTerm->StopWatch(3);
                    }

                tot = BAttr[A_STR] + BAttr[A_DEX] + BAttr[A_CON] + BAttr[A_INT] + BAttr[A_WIS] + BAttr[A_CHA] + BAttr[A_LUC];
                j++;
            } while (tot < 95 || tot > 100 || j < 100);

            MyTerm->Write(10,4+linesPerSet*i,
                Format("%2d  %2d  %2d  %2d  %2d  %2d  %2d  ", BAttr[A_STR],
                BAttr[A_DEX], BAttr[A_CON], BAttr[A_INT], 
                BAttr[A_WIS], BAttr[A_CHA], BAttr[A_LUC]));
            MyTerm->Update();

            for (j=0;j!=7;j++) {
                BestStats[j][i] = (int8)BAttr[j];
                /* We need to load the stat set into the player's data so that
                FeatPrereq() works properly with the bonus feats gained by
                perks. Thus, for example, a stat+perk set that has Strength
                7 will not also grant Power Attack, which has Strength 13 as
                a prerequisite. */
                Attr[j] = BAttr[j];
            }

JustDrawIt:

            if (statMethod == 1) {
                bool OK = false; 
                for (j=0; j < NUM_PERKS_PER_SET; j++) {
                    PerkSets[I(i,5)].elt[I(j,3)].type = PERK_NONE;
                    if (j >= nPerks)
                        continue;

                    int r;
                    PerkType type;
                    /* fjm: I changed the odds a little bit here, now that so
                    much of the ability array consists of favoured enemies
                    and smite choices instead of truly "wierd" things. */
                    // ability  25
                    // spell    15
                    // item     30
                    // feat     15
                    // resist   10
                    // immunity  5


                    static int16 PerkWeights[][2] = {
                        { PERK_ABILITY,  10 },
                        { PERK_SPELL,    5  },
                        { PERK_TALENT,   15 },
                        { PERK_RESIST,   10 },
                        { PERK_FEAT,     25 },
                        { PERK_ITEM,     30 },
                        { PERK_IMMUNE,   5  },
                        { 0,             0  }
                    };

                    for(tot=0,l=0;PerkWeights[l][0];l++)
                        tot += PerkWeights[l][1];

                    r = random(tot);
                    for(l=0;PerkWeights[l][0];l++) {
                        tot -= PerkWeights[l][1];
                        if (r >= tot)
                            break;
                    }
                    type = (PerkType) PerkWeights[l][0];

                    if (!do_redraw)
                        CreatePerk(PerkSets[I(i,5)],j,type,this);

                    String desc;
                    NamePerk(PerkSets[I(i,5)].elt[I(j,3)],desc);
                    /*
                    if (j == 0 && DesiredPerk) {
                        if (strstr((const char*)desc,DesiredPerk) == 0) {
                            j--; 
                            continue;
                        } 
                    } 
                    */
                    OK = true; 

                    MyTerm->Write(10, (5+j)+linesPerSet*i, "                                                                    ");
                    MyTerm->Write(10, (5+j)+linesPerSet*i, desc);
                } 
                if (!OK)
                    goto RerollThisOne;
            }

            MyTerm->Color(AZURE+i);
            MyTerm->Write(10,4+linesPerSet*i,
                Format("%2d  %2d  %2d  %2d  %2d  %2d  %2d  ", BAttr[A_STR],
                BAttr[A_DEX], BAttr[A_CON], BAttr[A_INT], 
                BAttr[A_WIS], BAttr[A_CHA], BAttr[A_LUC]));

        }
Reprompt:
        if (rerollCount) {
            MyTerm->Color(PINK);
            MyTerm->Write(MyTerm->WinSizeX()-12,MyTerm->WinSizeY()-1,Format(
                "(%d reroll%s)", rerollCount, rerollCount == 1 ? "" : "s"));
        }
        MyTerm->PrePrompt();
        MyTerm->SetWin(WIN_CREATION);
        MyTerm->Color(GREY);
        MyTerm->Write(1,1,Format("Which of the five attribute sets do you want? [abcdep%s%s?] ",
            Opt(OPT_REROLL) ? "x" : "",
            (Opt(OPT_REROLL) && statMethod == 1) ? "w" : ""));
        MyTerm->CursorOn();

        do_redraw = false;
        do
            ch = tolower(MyTerm->GetCharRaw());
        while (ch != KY_REDRAW && (ch < 'a' || ch > 'e') && ch != '?' && ch != 'p' && ((ch != 'x' && ch != 'w') || !Opt(OPT_REROLL)));
        PurgeStrings();
        MyTerm->CursorOff();
        if (ch == KY_REDRAW)
        { do_redraw = true; goto Redraw; }
        if (ch == '?')
        {
            MyTerm->HelpTopic("help::chargen","A");
            goto Reprompt;
        }
        if (ch == 'x') {
            if (Opt(OPT_STATS) == 1 && !perk_warn) {
                perk_warn = true; 
                MyTerm->Box(0,0,YELLOW,GREY,"  Rerolling stats with the perks option "
                    "enabled can be exceptionally abusive. They are intended to make "
                    "every game of Incursion unique and add an element of random excitement " 
                    "to the character creation process. As a result of this, rerolling perks "
                    "is not considered a 'legitimate' play strategy for a serious Incursion "
                    "game. If you want to be able to reroll your attributes and still gain "
                    "a legitimate win, you can turn perks off in the options panel. If you "
                    "just want to play a more powerful character for fun or to experiment, "
                    "please feel free (just select 'x' again to reroll). Rerolling perks will "
                    "place the game in Explore Mode.");
                // ww: this 'yn' messes the display up ... 
                /*
                if (!yn("Proceed with reroll?"))
                */
                goto Reprompt;
            }
            ExploreMode = true;
            rerollCount++;
            rerolledPerks = true;
            goto Reroll;
        }
        if (ch == 'p')
        {
            statMethod = 2;
            goto Redraw;
        }
        if (ch == 'w')
        {
            statMethod = 0;
            rerollCount++;
            goto Redraw;
        }

        for(j=0;j!=7;j++)
            BAttr[j] = BestStats[j][ch - 'a'];

        // did you want some starting perks? 
        if (Opt(OPT_STATS) == 1) { 
            i = ch - 'a'; 

            // remove unused perks
            for (j=0; j < 5; j++)
                if (i != j) 
                    for (int k=0;k<NUM_PERKS_PER_SET;k++)
                        PerkSets[I(j,5)].elt[I(k,3)].type = PERK_NONE;

            memcpy(&(RInf.Perks),&(PerkSets[I(i,5)]),sizeof(RInf.Perks));
            RInf.usedPerks = true;   
        }

        break;

    case 2: /* Point Buy Method */
        switch (Opt(OPT_POWER_STATS)) {
    case 0: Points = 30; break;
    case 1: Points = 42; break;
    case 2: Points = 54; break;
    case 3: Points = 75; break;
        }
        for (i=0;i!=7;i++)
            BAttr[i] = 8;
        c = 0;
        do {
RedrawPoints:
            MyTerm->SetWin(WIN_CREATION);
            MyTerm->Clear();
            PCost = 0;
            for (i=0;i!=7;i++)
                PCost += PointCosts[BAttr[i]];
            MyTerm->Color(EMERALD);
            MyTerm->Write(3,8,"Points Remaining: ");
            MyTerm->Color(WHITE);
            MyTerm->Write(Format("%d",Points - PCost));
            for (i=0;i!=7;i++) {
                MyTerm->Color(i == c ? PINK : RED);
                MyTerm->Write(5,10+i,AttrTitle[i]);
                MyTerm->Color(WHITE);
                MyTerm->Write(10,10+i,Format("%d",BAttr[i]));
            }
GetNewKey:
            ch = MyTerm->GetCharCmd(KY_CMD_ARROW_MODE);
            switch (ch) {
    case KY_REDRAW:
        goto RedrawPoints;
    case KY_CMD_SOUTH:
        if (c < 6)
            c++;
        break;
    case KY_CMD_NORTH:
        if (c)
            c--;
        break;
    case KY_CMD_EAST: 
    case '+': 
    case '=':
        if (BAttr[c] >= 18)
            break;
        // if (PCost - PointCosts[BAttr[c]] + PointCosts[BAttr[c]+1] > Points) break;
        BAttr[c]++;
        break;
    case KY_CMD_WEST:
    case '-':
    case '_':
        if (BAttr[c] <= 3)
            break;
        BAttr[c]--;
        break;

    default: 
        if (Points > PCost)
        {
            MyTerm->Color(MAGENTA);
            MyTerm->Write(1,6,Format("You still have %d points to spend.",Points-PCost));
            goto GetNewKey;
        }
        else if (Points < PCost && (Opt(OPT_STATS) != 4))
        {
            MyTerm->Color(MAGENTA);
            MyTerm->Write(1,6,Format("You have spent %d more points than you have.",PCost-Points));
            goto GetNewKey;
        }
        if (yn("Are these your final stats?"))
            goto Done;
        MyTerm->SetWin(WIN_CREATION);
            }
        }
        while(1);

    default:
        goto MethodI;
    }
Done:
    MyTerm->SetWin(WIN_CREATION);
    MyTerm->Clear();

    for (i=0;i!=7;i++)
        RInf.Attr[i] = (int8)BAttr[i];
    RInf.rerollCount = rerollCount;
    RInf.rerolledPerks = rerolledPerks;
    RInf.statMethod = statMethod;

    MyTerm->SetMode(MO_CREATE);

    return;
}

void Player::ChooseDomains()
{
    String name, desc;
    int16 i, j, n, q;
    int choices = 3; 
    static EventInfo xe;
    TGod * god = TGOD(GodID); 

    /* Multitude kludge */
    /* ww: there was a kludge for The Multitude that
    * involves lines like this: 
    case 0:  TGOD(GodID)->Domains[i] = FIND("Death"); break;
    * The problem is that this information is not saved with a saved
    * game, so if you load that game your priest is toast. Rather than
    * using this bitfield theory in Character, we'll use Stati (for
    * simplicity) to keep track of Domains. */
    if (GodID == FIND("the Multitude")) {
        // you get Evil for free
        GainPermStati(HAS_DOMAIN,this,SS_CLAS,0,0,FIND("Evil"));
        // and two other domains
        choices--;

        rID possDom[256]; 
        int max = 0;

        // enumerate all possibilities 
        for (q=0;q!=MAX_MODULES;q++)
            if (theGame->Modules[q]) {
                rID xID = theGame->Modules[q]->DomainID(0);
                rID endID = xID + theGame->Modules[q]->szDom;
                for (;xID != endID;xID++)
                    possDom[max++] = xID; 
            }

            // the following cannot be chosen at random 
            god->Domains[0] = FIND("Evil"); 
            god->Domains[1] = FIND("Good"); 
            srand(formulaSeed);
            for (i=2;i<2+8;) {
                rID choice = possDom[random(max)]; 
                bool already = false; 
                for (j=0;j<i;j++)
                    if (god->Domains[j] == choice)
                        already = true;
                if (!already) 
                    god->Domains[i++] = choice;
            } 
            god->Domains[0] = god->Domains[1] = 0; 
    } 

    // For almost everyone, choices=3. The Multitude offers more. 
    for (i=0;i!=choices;i++) {
        for (j=0;j!=12;j++)
            if (TGOD(GodID)->Domains[j])
                if (!HasEffStati(HAS_DOMAIN,god->Domains[j])) {
                    if (TDOM(god->Domains[j])->PEvent(EV_PREREQ,this,
                        god->Domains[j]) == -1)
                        continue;
                    name = NAME(god->Domains[j]);
                    desc = TDOM(god->Domains[j])->Describe(true);
                    MyTerm->LOption(name,j,desc);
                }

        n = (int16)MyTerm->LMenu(MENU_3COLS|MENU_DESC,"Choose a Domain:",WIN_CREATION,"help::domains");
        GainPermStati(HAS_DOMAIN,this,SS_CLAS,0,0,god->Domains[n]);
    }
}

void Character::AddAbilities(rID resID,int16 Lev) {
    Annotation *a; int16 i, ss;
    Resource *r = RES(resID);
    /* No FAnnot / NAnnot here, because it can be called recursively when processing Domains. */
    for (a = r->Annot(r->AnHead); a; a = r->Annot(a->Next)) {
        if (a->AnType == AN_ABILITY)
            for (i = 0; i != 4; i++)
                if (a->u.ab[i].AbType) {
                    if (a->u.ab[i].Lev2 == 0) {
                        if (a->u.ab[i].Lev1 != Lev)
                            continue;
                    } else {
                        if (a->u.ab[i].Lev1 > Lev)
                            continue;
                        if ((Lev-a->u.ab[i].Lev1) % a->u.ab[i].Lev2)
                            continue;
                    }

                    if (r->Type == T_TDOMAIN)
                        ss = SS_DOMA; 
                    else if (r->Type == T_TRACE)
                        ss = SS_RACE; 
                    else if (r->Type == T_TGOD)
                        ss = SS_BLES;
                    else
                        ss = SS_CLAS; 

                    switch (a->u.ab[i].AbType) {
                    case AB_ABILITY:
                        GainAbility(a->u.ab[i].Ability,a->u.ab[i].Param,resID,ss);
                        break;
                    case AB_FEAT:
                        GainFeat(a->u.ab[i].Ability,a->u.ab[i].Param);
                        break;
                    case AB_STATI:
                        StatiIterNature(this,a->u.ab[i].Ability)
                            if (S->Val == (a->u.ab[i].Param & 0xFFFF))
                                if (S->Source == ss && (a->u.ab[i].Ability != SPELL_ACCESS || S->eID == resID)) {
                                    if (!(a->u.ab[i].Param & 0xFFFF0000))
                                        S->Mag++;
                                    else
                                        S->Mag += a->u.ab[i].Param >> 16;
                                    StatiIterBreakout(this,goto DoneStati)         
                                }
                        StatiIterEnd(this)

                        GainPermStati(
                            (int16)a->u.ab[i].Ability, /* n */
                            NULL, /* Thing */
                            (int8)ss, /* Cause */
                            a->u.ab[i].Param & 0xFFFF, /* Val */
                            a->u.ab[i].Param & 0xFFFF0000 ? 
                            a->u.ab[i].Param >> 16 : 0, /* Mag */
                            resID /* eID */);
    DoneStati:;
                        break;
                    default:
                        Fatal("Wierd AbType in Character::AddAbilities!");
                        break;
                    }
                    CalcValues(true);
                    if (isPlayer()) { 
                        thisp->MyTerm->ShowTraits(); thisp->MyTerm->ShowStatus();
                    } 
                }
    }
}

void Character::GainBonusSkills(rID cID, int16 sp)
{
    int16 i;
    for (i=0;i!=6;i++)
        if (ClassID[i] == cID)
        { BonusSP[i] += sp; return; }
        for (i=3;i!=6;i++)
            if (!ClassID[i])
            { ClassID[i] = cID; BonusSP[i] += sp; return; }
            Error("Too many pseudoclass bonus skill allocations!");
}

uint32 Character::NextLevXP()
{
    return ExperienceChart[Level[0] + Level[1] + Level[2] + 1];
}


void Character::GainXP(uint32 _XP)
{
    int16 i; String msg;

    if (isPlayer())
        thisp->statiChanged = true;

    _XP -= (((int32)_XP) * XPPenalty()) / 100;

    XP += max(0,_XP);
    xpTicks++;
    for (i=0;i!=MAX_GODS;i++)
        if (PrayerTimeout[i])
            PrayerTimeout[i]--;

    /* Requiring an EID excludes this clause from working with
    druidic wildshape, but includes polymorph spells, traps,
    etc. */
    if (HasStati(POLYMORPH) &&
        GetStatiEID(POLYMORPH))
    {
        polyTicks++;
        if (polyTicks > 20)
            RemoveStati(POLYMORPH);
    }
    else
        polyTicks = 0;

    if (!random(10) && hMana >= 80) {
        int16 manaCost, buffCount, bleedCost,sp; 
        rID *buffList;
        getBuffInfo(&buffCount,&manaCost,&buffList);
        manaCost -= (80 + AbilityLevel(CA_MAGICAL_NATURE)*20);
        if (manaCost >= 0 && buffCount)
        {
            bleedCost = 1 + (manaCost / 50);
            if (cMana() < bleedCost) {
                sp = random(buffCount);
                IPrint("Lacking mana to bleed, your <9><Res><7> "
                    "spell collapses.", buffList[sp]);
                RemoveEffStati(buffList[sp]);
            }
            else {
                IPrint("You bleed mana.");
                LoseMana(bleedCost,true);
            }
        }
    }

    if (HasStati(TRIED,SK_LISTEN))
    {
        if (GetStatiMag(TRIED,SK_LISTEN))
            SetStatiMag(TRIED,SK_LISTEN,NULL,
            GetStatiMag(TRIED,SK_LISTEN) - 1);
        else
            RemoveStati(TRIED,-1,SK_LISTEN);
    }  

    if ((XP - XP_Drained) > NextLevXP())
        if ((XP - XP_Drained) > (uint32)ExperienceChart[NotifiedLevel+1])
        { 
            msg = Format("You have enough experience to advance to level %s.", NumberNames[NotifiedLevel+1]); 
            IPrint(msg);
            if (isPlayer()) {
                Player * p = (Player *)this;
                p->AddJournalEntry(msg);
            }
            NotifiedLevel++; 
            theGame->SetAutoSaveFlag(); 
        }

        // later, penalty and advancement

        if (Type == T_PLAYER)
            thisp->MyTerm->ShowTraits();

}

void Character::LoseXP(uint32 _XP)
{
    XP = max(0,(int32)XP - (int32)_XP);
    if (isPlayer())
        thisp->statiChanged = true;
}

// ww: If your druid animal kills something, you get the XP as well.
// Note that it might be tempted to divide it by two (or something), but
// that will just encourage players to waste time micromanaging things so
// that they get the last blow in. 
void Creature::KillXP(Creature *kill, int16 percent)
{
    Creature * c = ts.getLeader();
    if (c && kill != this)
        ((Creature *)c)->KillXP(kill);
    StatiIterNature(this,DIPLOMACY_TAG)
        if (S->h && oCreature(S->h))
            oCreature(S->h)->KillXP(kill,75);
    StatiIterEnd(this)
}

void Character::KillXP(Creature *kill, int16 percent)
{
    int16 s, Scale[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
        125, 150, 175, 200, 250, 300, 400, 500, 600, 800 },
        SlowAdvance[] = { 100, 100, 100, 125, 150, 200, 300 };
    int32 nXP;

    percent -= kill->SumStatiMag(XP_GAINED,-1,this);

    if (percent <= 0)
        return;

    kill->IncStatiMag(XP_GAINED,SS_MISC,0,this,percent);

    if (kill->HasStati(SUMMONED))
        return;
    switch (kill->ChallengeRating()) {
      case -6: nXP = 10; break;
      case -5: nXP = 15; break;
      case -4: nXP = 20; break;
      case -3: nXP = 25; break;
      case -2: nXP = 35; break;
      case -1: nXP = 50; break;
      case 0:  nXP = 75; break;
      default:
          if (kill->ChallengeRating() > 0)
              nXP = 100 * kill->ChallengeRating();
          else
              nXP = 5;
          break;
    }
    if (theGame->Opt(OPT_DIFFICULTY) == DIFF_EXPLORE)
        nXP *= 2;
    else if (theGame->Opt(OPT_DIFFICULTY) == DIFF_TRAINING)
        nXP = (nXP * 3) / 2;

    int16 ECL;
    ECL = ChallengeRating();
    if (HasStati(POLYMORPH))
        ECL += max(0,TMON(mID)->CR);
    s = kill->ChallengeRating() - ECL;
    if (s > 10)
        s = 10;
    if (s < -9)
        s = -9;
    nXP = (nXP * Scale[s+9]) / 100;

    nXP = (nXP * 100) / SlowAdvance[min(ChallengeRating(),6)];

    if (kill->HasStati(WAS_FRIENDLY,-1,this))
        nXP /= 3;

    if (kill->HasStati(GENERATION))
        nXP /= 1 + kill->GetStatiMag(GENERATION);

    nXP = (nXP * percent) / 100;

    GainXP(nXP);
}


int16 Character::XPPenalty()
{
    rID f1, f2, f3; int16 alignXP;
    /* Here's how we figure the XP penalty. Humans with Versatility and
    single classed characters never suffer a penalty. Multi-classed
    characters whose highest class (or one of their highest classes,
    for evenly matched characters) is a favoured class of their race
    also do not suffer a penalty. Other multi-class characters suffer
    a -25% experience penalty. */
    alignXP = 0;
    if ((-alignLC) > 40)
        alignXP = alignLC + 40;
    if (TotalLevel() < 3)
        alignXP = 0;

    if (HasAbility(CA_VERSATILITY) || 
        (isPlayer() && thisp->Opt(OPT_FAVOURED_CLASS) == 1))
        return 0 + SumStatiMag(XP_PENALTY) + alignXP;
    f1 = TRACE(RaceID)->FavouredClass[0];
    f2 = TRACE(RaceID)->FavouredClass[1];
    f3 = TRACE(RaceID)->FavouredClass[2];

    if (!Level[1] && !Level[2])
        return 0 + SumStatiMag(XP_PENALTY) + alignXP;

    if (ClassID[0] == f1 || ClassID[0] == f2 || ClassID[0] == f3 ||
        TCLASS(ClassID[0])->HasFlag(CF_FAVOURED))
        if (Level[0] >= Level[1] && Level[0] >= Level[2])
            return 0 + SumStatiMag(XP_PENALTY) + alignXP;

    if (ClassID[1] == f1 || ClassID[1] == f2 || ClassID[1] == f3 ||
        TCLASS(ClassID[1])->HasFlag(CF_FAVOURED))
        if (Level[1] >= Level[0] && Level[1] >= Level[2])
            return 0 + SumStatiMag(XP_PENALTY) + alignXP;

    if (ClassID[2] == f1 || ClassID[2] == f2 || ClassID[2] == f3 ||
        TCLASS(ClassID[2])->HasFlag(CF_FAVOURED))
        if (Level[2] >= Level[0] && Level[2] >= Level[1])
            return 0 + SumStatiMag(XP_PENALTY) + alignXP;

    return 25 + SumStatiMag(XP_PENALTY) + alignXP;
}

void Character::DrainXP(int32 amt)
{
    int8 neglev, lev;
    XP_Drained += amt;
    if (XP_Drained > XP) {
        ThrowDmg(EV_DEATH,AD_DREX,0,"negative energy",this,this);
        if (!(Flags & F_DELETE)) {
            /* We got raised -- wizard mode, AoLS, etc. */
            RemoveStati(ADJUST_DMG,-1,A_AID);
            XP_Drained = 0;
        }
        return;
    }
    lev = 0;
    while ((XP - XP_Drained) >= (uint32)ExperienceChart[lev+1])
        lev++;
    if (lev < TotalLevel()) {
        neglev = TotalLevel() - lev;
        if (HasStati(ADJUST_DMG,A_AID))
            GetStati(ADJUST_DMG,A_AID)->Mag = -neglev;
        else
            GainPermStati(ADJUST_DMG,NULL,SS_ATTK,A_AID,-neglev);
    }
    if (isPlayer())
        thisp->statiChanged = true;

}

void Creature::ThiefXP(rID regID)
{
    bool foundTreasure, foundMon;

    int16 i, mc, percent; 
    Monster *mn, *ml[2048]; 
    Thing *t;
    Container *cn;
    Item *it;

    foundTreasure = false;
    foundMon = false;
    mc = 0;

    if (!isCharacter())
        return;

    for (t=m->FirstThing();t;t=m->NextThing())
    {
        if (t->isItem())
            if (((Item*)t)->GetStatiEID(HOME_REGION) == regID)
            {
                it = (Item*)t;
                if (it->isMagic() || it->isType(T_COIN))
                {
                    foundTreasure = true;
                    if (!it->isKnown(KN_HANDLED))
                        return;
                }
                if (it->isType(T_CONTAIN))
                {
                    cn = (Container*)it;
                    for (it = oItem(cn->Contents);it;it = oItem(it->Next))
                        if (it->isMagic() || it->isType(T_COIN))
                        {
                            foundTreasure = true;
                            if (!it->isKnown(KN_HANDLED))
                                return; /* Untouched Item == Incomplete */
                        }
                }
            }
            if (t->isMonster() && !t->isDead())
                if (t->HasEffStati(HOME_REGION,regID)) {
                    mn = (Monster*)t;
                    if (!t->HasStati(XP_GAINED))
                        foundMon = true;
                    if (mn->ts.hasTargetThing(this))
                        if (!mn->isFriendlyTo(this))
                            return;  /* Aware Monster == Failure */
                    if (mc < 2000)
                        ml[mc++] = mn;            
                }
    }  

    if (foundTreasure && foundMon) {
        IDPrint("You slip out like a thief in the night!", "The <Obj> slips out like a thief in the night!",this);
        percent = (AbilityLevel(CA_THIEF_IN_THE_NIGHT) * 
            100) / max(1,ChallengeRating());
        for (i=0;i!=mc;i++)
            KillXP(ml[i],percent);
    }
}

#define CG_LIMIT 50

void Player::ChooseGod(bool required) {
    int i = 0, q;
    rID GodList[CG_LIMIT], xID;
    bool pally = false;
    String desc;
    bool worthy;

    i = 0;
    /* First look to the class for a restricted list of allowed gods, and if
       that doesn't exist, find and create a list of all gods in any module. */
    if (!TCLASS(ClassID[0])->GetList(ALLOWED_GODS, GodList, CG_LIMIT - 1)) {
        for (q = 0; q != MAX_MODULES && i < CG_LIMIT - 1; q++)
            if (theGame->Modules[q]) {
                rID xID = xID = theGame->Modules[q]->GodID(0);
                rID endID = xID + theGame->Modules[q]->szGod;
                for (; xID != endID; xID++) {
                    GodList[i++] = xID;
                    if (i == CG_LIMIT - 1)
                        break;
                }
            }
        GodList[i] = 0;
    }

    for (i = 0; GodList[i]; i++) {
        xID = GodList[i];

        SetSilence();
        worthy = isWorthyOf(xID, true);
        UnsetSilence();
        if (!worthy)
            continue;

        Candidates[i] = xID;
        desc = DESC(xID);
        for (int d = 0; d < 6; d++) {
            if (TGOD(xID)->Domains[d])
                desc += Format("\n\n%s", (const char*)
                TDOM(TGOD(xID)->Domains[d])->Describe());
        }
        MyTerm->LOption(NAME(xID), i, desc);
    }

    if (!i)
        Fatal("No suitable God to choose from among the loaded resources!");

    int flags = MENU_3COLS | MENU_DESC;
    if (!required)
        flags |= MENU_ESC;
    i = MyTerm->LMenu(flags, "<15>Choose a God:",
        thisp->MyTerm->GetMode() == MO_CREATE ? WIN_CREATION :
        WIN_SCREEN, "help::domains");

    if (i >= 0)
        GodID = Candidates[i];

    setGodFlags(GodID, GS_INVOLVED);
}

int16 TotalClassLevels(rID clID) {
    int16 tl;
    tl = (int16)TCLASS(clID)->GetConst(TOTAL_CLASS_LEVELS);
    if (tl)
        return tl;
    return TCLASS(clID)->HasFlag(CF_PRESTIGE) ? 10 : 20;
}

void Player::AdvanceLevel() {
    int16 c,i, j; TClass *tc;
    if (TotalLevel() >= MAX_CHAR_LEVEL) {
        IPrint(Format("The maximum character level in this version of Incursion is %d.",MAX_CHAR_LEVEL));
        return;
    }

    if (ClassID[1] && Level[0]) {
        MyTerm->LOption(NAME(ClassID[0]),0);
        MyTerm->LOption(NAME(ClassID[1]),1);
        if(ClassID[2])
            MyTerm->LOption(NAME(ClassID[2]),2);

        c = (int16)MyTerm->LMenu(MENU_3COLS | (TotalLevel() ? MENU_ESC : 0) | MENU_BORDER,
            "Which class do you wish to advance in?",WIN_MENUBOX,"help::chargen,C");
        if (c == -1)
            return; 
    } else
        c = 0;

    tc = TCLASS(ClassID[c]);

    if (TotalLevel() > 1) {
        if (LevelAs(ClassID[c]) >= TotalClassLevels(ClassID[c])) {
            IPrint("The <res> class only has <Num> levels. You must choose a "
                "different class to continue to advance.", ClassID[c],
                TotalClassLevels(ClassID[c]));
            return;
        }

        if (tc->HasFlag(CF_PALADIN) && isFallenPaladin) {
            IPrint("You have fallen. You may advance no farther as a paladin.");
            return;
        }

        if (tc->HasFlag(CF_RELIGIOUS) && !GodID) {
            IPrint("You must have a suitable god to advance in that class.");
            return;
        }

        uint16 align; bool bad_align;
        align = GetStatiVal(ALIGNMENT);
        bad_align = false;
        if (tc->HasFlag(CF_LAWFUL) && !(align & AL_LAWFUL)) 
            bad_align = true;
        if (tc->HasFlag(CF_CHAOTIC) && !(align & AL_CHAOTIC)) 
            bad_align = true;
        if (tc->HasFlag(CF_EVIL) && !(align & AL_EVIL)) 
            bad_align = true;
        if (tc->HasFlag(CF_GOOD) && !(align & AL_GOOD)) 
            bad_align = true;

        if (tc->HasFlag(CF_NONLAWFUL) && (align & AL_LAWFUL)) 
            bad_align = true;
        if (tc->HasFlag(CF_NONCHAOTIC) && (align & AL_CHAOTIC)) 
            bad_align = true;
        if (tc->HasFlag(CF_NONEVIL) && (align & AL_EVIL)) 
            bad_align = true;
        if (tc->HasFlag(CF_NONGOOD) && (align & AL_GOOD)) 
            bad_align = true;

        if (bad_align) {
            IPrint("You do not meet the alignment requirements of that class.");
            return;
        }

        rID GodList[CG_LIMIT];
        if (tc->GetList(ALLOWED_GODS, GodList, CG_LIMIT-1) && GodID) {
            for (int n=0;GodList[n];n++)
                if (GodList[n] == GodID)
                    goto GodIsOkay;
            IPrint("You cannot advance as a <Res> because <Res> is not a <Res> god.", ClassID[c], GodID, ClassID[c]);
            return;
GodIsOkay:;
        }
    }

    Level[c]++;
    Proficiencies |= tc->Proficiencies;


    if (Opt(OPT_ATTR_ON_LEVEL) && 
        ((TotalLevel() % 4 == 0) && (TotalLevel() >= 4))) {
            int16 MaxBonus, CurrBonus, foundOne = 0;
            static const char*AttrNames[] = { "Strength", "Dexterity", "Constitution", "Intelligence", "Wisdom", "Charisma", "Luck" };
            MaxBonus = 5 + AbilityLevel(CA_INHERANT_POTENTIAL);
            for (i=A_STR; i<= A_LUC; i++) {
                CurrBonus = SumStatiMag(ADJUST_INH,i);
                if (CurrBonus < MaxBonus) {
                    MyTerm->LOption(AttrNames[i],i);
                    foundOne = 1; 
                } 
            } 
            if (foundOne) {
                i = (int16)MyTerm->LMenu(MENU_2COLS,"-- Gain A Bonus To Which Attribute --");
                GainInherentBonus(i,1,true); 
                CalcValues(); 
            }
    }

    {
        EventInfo xe;
        xe.Clear();
        xe.Event = PRE(EV_ADVANCE);
        xe.eID = ClassID[c];
        xe.EActor = this;
        RES(RaceID)->Event(xe,RaceID);
        RES(ClassID[c])->Event(xe,ClassID[c]);

        if (GodID)
            RES(GodID)->Event(xe,ClassID[c]);

        StatiIterNature(this,HAS_DOMAIN)
            RES(S->eID)->Event(xe,ClassID[c]);
        StatiIterEnd(this)
    }

    /* In this order so human wizards can choose a metamagic feat
    as their bonus human feat at 1st level. */
    AddAbilities(ClassID[c],Level[c]);
    AddAbilities(RaceID,TotalLevel());
    CalcValues(); // For Attribute-based feat prereqs

#if 0
    // ww: give feats for following favoured classes, rather than an xp
    // penalty for avoiding them, as per the Conan d20 game
    // fjm: see TABLES.CPP for my rationale for removing this.
    if (Opt(OPT_FAVOURED_CLASS) == 1) { 
        int16 racialLevels = 0;
        for (i=0;i<3;i++) 
            if (ClassID[i] && 
                (ClassID[i] == TRACE(RaceID)->FavouredClass[0] ||
                ClassID[i] == TRACE(RaceID)->FavouredClass[1] ||
                ClassID[i] == TRACE(RaceID)->FavouredClass[2] ||
                HasAbility(CA_VERSATILITY)))
                racialLevels += Level[i];
        if ((racialLevels + 1) % 3 == 0) {
            GainFeat(FT_FULL_LIST);
            CalcValues(); 
        }
    }
#endif 

    {
        EventInfo xe;
        xe.Clear();
        xe.Event = EV_ADVANCE;
        xe.eID = ClassID[c];
        xe.EActor = this;
        RES(RaceID)->Event(xe,RaceID);
        RES(ClassID[c])->Event(xe,ClassID[c]);

        if (GodID)
            RES(GodID)->Event(xe,ClassID[c]);

        StatiIterNature(this,HAS_DOMAIN)
            RES(S->eID)->Event(xe,ClassID[c]);
        StatiIterEnd(this)
    }

    /* Moved after EV_ADVANCE so that War domain Shield Prof will
    count for the prereqs of your first two feats. */
    if ((TotalLevel() % 3 == 0) || (TotalLevel() == 1)) {
        GainFeat(FT_FULL_LIST);
        CalcValues(); 
    }

    /* Let's try out a 2nd feat at 1st level. */
    if (TotalLevel() == 1)
        GainFeat(FT_FULL_LIST);


    if (Level[c] == 1)                           
        for(i=0;i!=40 && tc->Skills[i];i++)
            Feats[tc->Skills[i]/8] |= 1 << (tc->Skills[i]%8);
    if (TotalLevel() == 1) {
        for(i=0;i!=12 && TRACE(RaceID)->Skills[i];i++)
            Feats[TRACE(RaceID)->Skills[i]/8] |= 1 << (TRACE(RaceID)->Skills[i]%8);
    }  

    tStoryPluses = AbilityLevel(CA_STORYCRAFT);


    /* ww: rather than just "random" or "full" hp, allow "half" (which is
    * no more powerful than "random" on average, but not in fact random)
    */
    int16 HD = tc->HitDie;

    /* Thaumaturges get d8's instead of d4's */
    if (HasEffStati(SPECIALTY_SCHOOL,ClassID[c],7/*SC_THA*/))
        HD = 8;

    switch (Opt(OPT_MAX_HP)) {
      case 1: /* half */ hpRolls[c][Level[c]-1] = (int8)(HD / 2); break; 
      case 2: /* full */ hpRolls[c][Level[c]-1] = (int8)HD ; break; 
      default: /* no */
          hpRolls[c][Level[c]-1] = max(random(HD)+1,(HD/2));
    }

    switch (Opt(OPT_MAX_MANA)) {
      case 1: /* half */ manaRolls[c][Level[c]-1] = tc->ManaDie / 2; break;
      case 2: /* full */ manaRolls[c][Level[c]-1] = tc->ManaDie; break;
      default: /* no */
          manaRolls[c][Level[c]-1] = max(random(tc->ManaDie)+1,(tc->ManaDie/2));
    } 

    for (i=0;GodIDList[i];i++)
        if (j = (int16)TGOD(GodIDList[i])->GetConst(LEVEL_BLEEDOFF))
            FavPenalty[theGame->GodNum(GodIDList[i])] = max(0,FavPenalty[theGame->GodNum(GodIDList[i])] - j);

    CalcValues();

    {
        EventInfo xe;
        xe.Clear();
        xe.Event = POST(EV_ADVANCE);
        xe.eID = ClassID[c];
        xe.EActor = this;
        RES(RaceID)->Event(xe,RaceID);
        RES(ClassID[c])->Event(xe,ClassID[c]);

        if (GodID)
            RES(GodID)->Event(xe,ClassID[c]);

        StatiIterNature(this,HAS_DOMAIN)
            RES(S->eID)->Event(xe,ClassID[c]);
        StatiIterEnd(this)
    }

    UpdateTattoos();

    if (TotalLevel() >= 5)
        DisableReincarnation();

    if (isPlayer())
        thisp->statiChanged = true;
}

// TODO(unreliable-code): Verify that the loss of abilities in a Paladin-fall is correct.
void Character::PaladinFall() {
    int16 lev;
    lev = LevelAs(FIND("Paladin"));
    IPrint("<1>You experience a wrenching, soul-destroying sense of loss: you are no longer a paladin.<7>");
    AddJournalEntry("You experience a wrenching, soul-destroying sense of loss: you are no longer a paladin.");
    isFallenPaladin = true;

    Abilities[CA_SACRED_MOUNT] = 0;
    Abilities[CA_DIVINE_GRACE] = 0;
    Abilities[CA_LAY_ON_HANDS] = 0;
    Abilities[CA_AURA_OF_VALOUR] = 0;

    Spells[theGame->SpellNum(FIND("Mount"))] &= ~(SP_INNATE);
    Spells[theGame->SpellNum(FIND("Cure Disease"))] &= ~(SP_INNATE);
    Spells[theGame->SpellNum(FIND("Magic Circle vs. Evil"))] &= ~(SP_INNATE);
    Spells[theGame->SpellNum(FIND("Protection from Evil"))] &= ~(SP_INNATE);
    Spells[theGame->SpellNum(FIND("Detect Evil"))] &= ~(SP_INNATE);

    if (lev >= 3)
        Abilities[CA_TURNING] -= (lev-2);
    RemoveEffStati(FIND("Paladin"));

    if (isPlayer())
        thisp->statiChanged = true;

}

// TODO(unimplemented): Paladin atonement.
void Character::PaladinAtone() {
}

inline bool CheckWeapon(Player *p, rID wID) {
    if (p->GetAttr(A_SIZ) + 1 < TITEM(wID)->Size)
        if (!p->yn("That weapon is too large for you normally. Continue?"))
            return false;

    {
        int16 mstr = 5;
        if (TITEM(wID)->HasFlag(WT_STR1))
            mstr += 8;
        if (TITEM(wID)->HasFlag(WT_STR2))
            mstr += 4;
        if (TITEM(wID)->HasFlag(WT_STR3))
            mstr += 2;
        if (p->GetAttr(A_STR) < mstr && mstr > 5)
            if (!p->yn("You lack that weapon's Minimum Strength. Continue?"))
                return false;
    }

    return true;
}

void Player::GainFeat(int16 feat, int32 param) {
    int16 *list; const char*title; String desc;
    int16 i,lv,q,j; int8 Win; int16 ofeat = feat;
    bool OneFeat, Found; rID wID, endID;
    String chooseWeaponStr, name; 
    static bool showAll = false; 
    bool bardic = false; 

Restart:
    Win = MyTerm->GetMode() == MO_CREATE ? WIN_CREATION : WIN_MENUBOX;
    //Win = WIN_SCREEN; 
    OneFeat = false;

    if (feat >= FT_FIRST)
        goto SelectedFeat;

    switch(ofeat) {
    case FT_WIZARD_LIST: 
        title = "Choose a bonus metamagic Feat:";
        break;
    case FT_WARRIOR_LIST: 
        title = "Advancing as a Warrior, choose a Warrior Feat:";
        break;
        /*
        case FT_RANGER_LIST: list = RangerFeats;
        title = "Choose a feats from the Ranger bonus feat list:";
        break;
        case FT_ROGUE_LIST: list = NULL;
        title = "Choose a feat from this list for high-level Rogues:";
        break;
        case FT_MONK_LIST: list = MonkFeats;
        title = "Choose a martial arts Feat from this list:";
        break;
        */
    case FT_BARD_LIST: list = NULL; bardic = true; 
        title = "Choose any feat without another feat as prerequisite:"; 
        break;
    case FT_FULL_LIST:
    default:
        list = NULL;
        title = "Choose a feat; the following are available:";
        break;
    }

pick_again: 
    {
        extern bool FeatUnimplemented(int16 ft);
        extern const char * FeatName(int16 ft);
        extern String & DescribeFeat(int16 ft); 

        for(i=FT_FIRST;i!=FT_LAST;i++) 
            if (/*!FeatUnimplemented(i)*/1) {
                for (j=0;FeatInfo[j].feat;j++)
                    if (FeatInfo[j].feat == i)
                        goto FoundFeat;
                continue;
FoundFeat: 
                /* is it on our list, if any? */
                bool onList;
                onList = false;
                switch (ofeat) {
                case FT_WIZARD_LIST:
                    if (FeatInfo[j].flags & (FF_META|FF_WIZARD))
                        onList = true;
                    break;
                case FT_WARRIOR_LIST:
                    if (FeatInfo[j].flags & (FF_WARRIOR))
                        onList = true; 
                    break;
                case FT_BARD_LIST:
                    onList = true;
                    bardic = true;
                    break;
                default:
                    onList = true;
                    break;
                }

                if (!onList) continue; 
                bool prereq = FeatPrereq(i,bardic); 
                bool unimp  = FeatUnimplemented(i);
                if (prereq || showAll) {
                    name = XPrint(Format("<%d>%s<7>",
                        unimp ? RED : (prereq ? YELLOW : SHADOW), 
                        FeatName(i)));
                    MyTerm->LOption( name , i, DescribeFeat(i), 2 - (2*prereq));
                } 
                if (prereq) OneFeat = true; 
            } 
    }
    if (!showAll) 
        MyTerm->LOption("(Show All Feats)",-1,"Show all feats, even those you do not currently have the prerequisites for. This can be useful for planning out your character development.",1);
    else
        MyTerm->LOption("(Hide Unavailable Feats)",-1,"Hide all feats for which you do not currently have the prerequisites. This removes clutter.",1);

    if (!OneFeat) /* No Feats on list available! */
        return;

    feat = (int16)MyTerm->LMenu(MENU_SORTED|MENU_3COLS|MENU_DESC|(Win == WIN_MENUBOX ? MENU_BORDER : 0),title,Win,"help::feats");
    if (feat == -1) {
        showAll = !showAll;
        goto pick_again;
    } 

    if (!FeatPrereq(feat)) {
        if (FeatUnimplemented(feat))
            MyTerm->Box("That feat isn't implemented (or isn't working correctly) in this version of Incursion. Please select another.");
        else
            MyTerm->Box("You do not have the prerequisites for that feat. Please select another.");
        goto pick_again; 
    } 

SelectedFeat:
    if (feat != FT_WEAPON_PROFICIENCY || OneFeat)
        Feats[feat/8] |= 1 << (feat%8);

//ChooseParam:
    switch(feat) {
    case FT_LION_HEART:
        GainPermStati(SAVE_BONUS,NULL,SS_MISC,SN_FEAR,+2);
        break;
    case FT_SKILL_FOCUS:
        if (param) 
            i = (int16)param;
        else {
            Found = false;
            for(i=1;i!=SK_LAST;i++)
                if (SkillInfo[i].imp) {
                    bool taken; taken = false;
                    StatiIterNature(this, SKILL_BONUS)
                        if (S->Val == i && S->Source == SS_PERM)
                            taken = true;
                    StatiIterEnd(this)
                        if (taken)
                            continue;
                    if (!SkillInfo[i].name)
                        continue;
                    MyTerm->LOption(SkillInfo[i].name,i,
                        DescribeSkill(SkillInfo[i].sk));
                    Found = true; 
                }
                if (!Found) {
                    if (ofeat != feat)
                        feat = ofeat; 
                    else
                        feat = FT_FULL_LIST;
                    goto Restart; 
                }
                i = (int16)MyTerm->LMenu(MENU_SORTED|MENU_3COLS|MENU_DESC|MENU_BORDER,
                    "Choose a skill:",WIN_MENUBOX,"help::skills");
        }
        GainPermStati(SKILL_BONUS,this,SS_PERM,i,2,0);
        break;
    case FT_NATURAL_APTITUDE:
        if (param) 
            i = (int16)param;
        else {
            Found = false;
            for(i=0;i!=SK_LAST;i++)
                if (!HasSkill(i))                
                    if (SkillInfo[i].name && SkillInfo[i].imp)
                        if (!HasStati(EXTRA_SKILL,i)) {
                            MyTerm->LOption(SkillInfo[i].name,i,DescribeSkill(SkillInfo[i].sk));
                            Found = true;
                        }
            if (!Found) {
                if (ofeat != feat)
                    feat = ofeat; 
                else
                    feat = FT_FULL_LIST;
                goto Restart; 
            }
            i = (int16)MyTerm->LMenu(MENU_SORTED|MENU_3COLS|MENU_BORDER|MENU_DESC,"Choose a skill:",WIN_MENUBOX,"help::skills");
        }
        GainPermStati(EXTRA_SKILL,NULL,SS_CLAS,i,0,0);
        break;
    case FT_INTENSIVE_STUDY:
        uint16 eli, choice;
        eli = getEligableStudies();
        for (i=0;Studies[i][0];i++)
            if (eli == XBIT(Studies[i][0])) {
                choice = Studies[i][0];
                goto ChosenStudy;
            }
            if (eli == XBIT(STUDY_BAB)) {
                choice = STUDY_BAB;
                goto ChosenStudy;
            }
            for (i=0;StudyNames[i].Val;i++)
                if (eli & XBIT(StudyNames[i].Val))
                    MyTerm->LOption(StudyNames[i].Text, StudyNames[i].Val);
            choice = (uint16)MyTerm->LMenu(MENU_SORTED|MENU_BORDER,"Choose an area to improve:",WIN_MENUBOX);
ChosenStudy:
            IntStudy[choice]++;
            tStoryPluses = AbilityLevel(CA_STORYCRAFT);
            break;
    case FT_SCHOOL_FOCUS:
        if (param)
            i = (int16)param;
        else {
            if (!HasEffStati(SCHOOL_FOCUS,SC_ABJ))
                MyTerm->LOption("Abjuration",SC_ABJ);
            if (!HasEffStati(SCHOOL_FOCUS,SC_ARC))
                MyTerm->LOption("Arcana",SC_ARC);
            if (!HasEffStati(SCHOOL_FOCUS,SC_DIV))
                MyTerm->LOption("Divination",SC_DIV);
            if (!HasEffStati(SCHOOL_FOCUS,SC_ENC))
                MyTerm->LOption("Enchantment",SC_ENC);
            if (!HasEffStati(SCHOOL_FOCUS,SC_EVO))
                MyTerm->LOption("Evocation",SC_EVO);
            if (!HasEffStati(SCHOOL_FOCUS,SC_ILL))
                MyTerm->LOption("Illusion",SC_ILL);
            if (!(HasEffStati(SCHOOL_FOCUS,SC_NEC) || isMType(MA_ELF)))
                MyTerm->LOption("Necromancy",SC_NEC);
            if (!HasEffStati(SCHOOL_FOCUS,SC_THA))
                MyTerm->LOption("Thaumaturgy",SC_THA);
            if (!HasEffStati(SCHOOL_FOCUS,SC_WEA))
                MyTerm->LOption("Weavecraft",SC_WEA);
            i = (int16)MyTerm->LMenu(MENU_3COLS|MENU_BORDER,"Choose a focus school:",
                WIN_MENUBOX,"help::magic,SC");
        }
        GainPermStati(SCHOOL_FOCUS,NULL,SS_MISC,0,i,0);
        break;

#if 0
    case FT_WILD_SHAPE_TYPE: 
        if (param)
            i = param;
        else {
            int16 poss[] = { 
                MA_GIANT,
                MA_ABERRATION,
                MA_MYTHIC,
                MA_TROLL,
                MA_HOMINID,
                MA_OOZE, 
                MA_FAERIE,
                MA_NAGA,
                0
            };
            for (i=0;poss[i];i++) {
                if (!HasStati(ADDITIONAL_WILD_SHAPE_MA_TYPE,poss[i])) {
                    desc = ""; 
                    for (int modIdx = 0; modIdx < 1; modIdx++) {
                        Module *mod = theGame->Modules[modIdx];
                        for (int idx = 0; idx < mod->szMon; idx++) {
                            if (!(mod->QMon[idx].isMType(poss[i])))
                                continue;
                            if (mod->QMon[idx].HasFlag(M_NOGEN))
                                continue;

							uint32 gid = GLYPH_ID_VALUE(mod->QMon[idx].Image);
                            int32 color = -(int32)GLYPH_COLOUR_VALUE(mod->QMon[idx].Image);
                            if (color == BLACK) color = -SHADOW;
                            desc += Format("%c%s (%c%c%c%c%c, CR %d, HD %d). ",
								-GREY, (const char*)(RES(idx)->GetName(0).Capitalize()),
								color, LITERAL_CHAR, LITERAL_CHAR1(gid), LITERAL_CHAR2(gid), -GREY,
                                mod->QMon[idx].CR,
                                mod->QMon[idx].HitDice);
                        }
                    } 
                    if (desc.GetLength())
                        MyTerm->LOption(Pluralize(Lookup(MTypeNames,poss[i])),
                        poss[i],desc,0);
                }
            }
            i = MyTerm->LMenu(MENU_3COLS|MENU_DESC|MENU_BORDER,
                "Choose a Wild Shape type:", WIN_MENUBOX);
        }
        GainPermStati(ADDITIONAL_WILD_SHAPE_MA_TYPE,NULL,SS_PERM,i,0,0);
        break;
#endif


    case FT_IMPROVED_CRIT:
        if (param)
            wID = param;
        else {
            Found = false;
ImpCritChoice:
            for (q=0;q!=MAX_MODULES;q++)
                if (theGame->Modules[q]) {
                    wID = theGame->Modules[q]->ItemID(0);
                    endID = wID + theGame->Modules[q]->szItm;
                    for(;wID!=endID;wID++)
                        if (TITEM(wID)->IType == T_WEAPON || TITEM(wID)->IType == T_BOW)
                            if (WepSkill(wID,true) >= WS_PROFICIENT) { 
                                Item * i = Item::Create(wID); 
                                Weapon * w = (Weapon *)i; 
                                if (TITEM(wID)->HasFlag(IT_NOGEN))
                                    name = Format("(%s)",NAME(wID));
                                else
                                    name = NAME(wID);
                                MyTerm->LOption(name,wID,w->Describe(this),TITEM(wID)->HasFlag(IT_NOGEN));
                                RemoveStatiFrom(w); 
                                delete i; 
                                Found = true; 
                            }
                }
                if (!Found) {
                    if (ofeat != feat)
                        feat = ofeat; 
                    else
                        feat = FT_FULL_LIST;
                    goto Restart; 
                }
                wID = MyTerm->LMenu(MENU_SORTED|MENU_3COLS|MENU_DESC|MENU_BORDER,
                    "Choose a Weapon for Improved Critical:",WIN_MENUBOX);
                if (!CheckWeapon(this,wID))
                    goto ImpCritChoice;
        }
        GainPermStati(IMP_CRIT,NULL,SS_CLAS,0,0,wID);
        break;
    case FT_WEAPON_PROFICIENCY:
        lv = WS_PROFICIENT;
        chooseWeaponStr = "Train in which Weapon:";
        goto ChooseWeapon;
    case FT_WEAPON_FOCUS:
        lv = WS_FOCUSED;
        chooseWeaponStr = "Focus on which Weapon:";
        goto ChooseWeapon;
    case FT_WEAPON_SPECIALIST:
        lv = WS_SPECIALIST;
        chooseWeaponStr = "Specialize in which Weapon:";
        goto ChooseWeapon;
    case FT_WEAPON_MASTERY:
        lv = WS_MASTERY;
        chooseWeaponStr = "Master which Weapon:";
        goto ChooseWeapon;
    case FT_WEAPON_HIGH_MASTERY:
        lv = WS_HIGH_MASTERY;
        chooseWeaponStr = "High Mastery in which Weapon:";
        goto ChooseWeapon;
    case FT_WEAPON_GRAND_MASTERY:
        lv = WS_GRAND_MASTERY;
        chooseWeaponStr = "Grand Mastery in which Weapon:";
        goto ChooseWeapon;

ChooseWeapon:
        if (param)
            wID = param;
        else {
            Found = false;
            for (q=0;q!=MAX_MODULES;q++)
                if (theGame->Modules[q]) {
                    wID = theGame->Modules[q]->ItemID(0);
                    endID = wID + theGame->Modules[q]->szItm;
                    for(;wID!=endID;wID++)
                        if (TITEM(wID)->IType == T_WEAPON || TITEM(wID)->IType == T_BOW)
                            if (WepSkill(wID,true) == lv-1)
                                if (lv != WS_PROFICIENT || !(TITEM(wID)->Group & WG_EXOTIC)) { 
                                    Item * i = Item::Create(wID); 
                                    Weapon * w = (Weapon *)i; 
                                    /* ww: for the purposes of this list, pretend that
                                    * you are one level better than you really are! */
                                    GainPermStati(WEP_SKILL,w,SS_MISC,lv,-1,wID);
                                    // GainTempStati(WEP_SKILL,NULL,1,SS_ONCE,lv,0,wID);
                                    if (TITEM(wID)->HasFlag(IT_NOGEN))
                                        name = Format("(%s)",NAME(wID));
                                    else
                                        name = NAME(wID);
                                    MyTerm->LOption(name,wID,w->Describe(this),TITEM(wID)->HasFlag(IT_NOGEN));
                                    RemoveStatiFrom(w); 
                                    delete i; 
                                    Found = true; 
                                }
                }
                if (!Found) {
                    if (ofeat != feat)
                        feat = ofeat; 
                    else
                        feat = FT_FULL_LIST;
                    goto Restart; 
                }
                wID = MyTerm->LMenu(MENU_SORTED|MENU_3COLS|MENU_DESC|MENU_BORDER,
                    chooseWeaponStr,WIN_MENUBOX);
                if (!CheckWeapon(this,wID))
                    goto ChooseWeapon;
        }
        if (HasEffStati(WEP_SKILL,wID))
            SetEffStatiVal(WEP_SKILL,wID,lv);
        else
            GainPermStati(WEP_SKILL,NULL,SS_CLAS,lv,0,wID);
        break;
    case FT_EXOTIC_WEAPON:
        lv = WS_PROFICIENT;
        if (param) 
            wID = param;
        else {
ChooseExotic:
            Found = false;
            for (q=0;q!=MAX_MODULES;q++)
                if (theGame->Modules[q]) {
                    wID = theGame->Modules[q]->ItemID(0);
                    endID = wID + theGame->Modules[q]->szItm;
                    for(;wID!=endID;wID++)
                        if (TITEM(wID)->IType == T_WEAPON || TITEM(wID)->IType == T_BOW)
                            if (TITEM(wID)->Group & WG_EXOTIC)
                                if (WepSkill(wID,true) == WS_NOT_PROF)
                                    if ((TITEM(wID)->Group & (~WG_EXOTIC) & Proficiencies) ||
                                        (TITEM(wID)->Group == WG_EXOTIC))
                                    { 
                                        Item * i = Item::Create(wID); 
                                        Weapon * w = (Weapon *)i; 
                                        /* ww: for the purposes of this list, pretend that
                                        * you are one level better than you really are! */
                                        GainPermStati(WEP_SKILL,w,SS_MISC,lv,-1,wID);
                                        if (TITEM(wID)->HasFlag(IT_NOGEN))
                                            name = Format("(%s)",NAME(wID));
                                        else
                                            name = NAME(wID);
                                        MyTerm->LOption(name,wID,w->Describe(this),TITEM(wID)->HasFlag(IT_NOGEN));
                                        RemoveStatiFrom(w); 
                                        delete i; 
                                        Found = true; 
                                    }
                }
                if (!Found)
                {
                    if (ofeat != feat)
                        feat = ofeat; 
                    else
                        feat = FT_FULL_LIST;
                    goto Restart; 
                }
                wID = MyTerm->LMenu(MENU_SORTED|MENU_3COLS|MENU_DESC,
                    "Choose a weapon:",Win);
                if (!CheckWeapon(this,wID))
                    goto ChooseExotic;
        }
        RemoveEffStati(wID);
        GainPermStati(WEP_SKILL,NULL,SS_CLAS,WS_PROFICIENT,0,wID);
        break;
    case FT_AP_LIGHT:
        Proficiencies |= WG_LARMOUR;
        break;
    case FT_AP_MEDIUM:
        Proficiencies |= WG_MARMOUR;
        break;
    case FT_AP_HEAVY:
        Proficiencies |= WG_HARMOUR;
        break;

    case FT_WILD_SHAPE_MASTERY: 
        GainAbility(CA_WILD_SHAPE,2,0,SS_PERM);
        break; 
    case FT_CIRCLE_KICK:
        GainPermStati(ACTIVATION,NULL,SS_MISC,0,0,FIND("Circle Kick"),0);
        break;

    default:
        break;
    }
}

void Player::GainAbility(int16 ab, uint32 pa, rID sourceID, int16 statiSource) {
    int16 st;
    const char* MTypePrompt;
    /* New spell levels -- the caster level at which a player
    receives access to a new level of spells. */
    static int16 NSL[] = { 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 0, 6, 0, 7, 0, 8, 0, 9, 0, 0 };
    String desc, category;
    int16 i,j;

    if (ab != CA_SMITE)
        pa = max(pa,1);

    switch (ab) {
    case CA_AURA_OF_VALOUR:
        Map *om;
        int16 ox,oy;

        Abilities[ab] += (uint8)pa;
        /* Refresh the field */
        om = m; ox = x; oy = y;
        Remove(false);
        PlaceAt(om,ox,oy);
        break;
    case CA_SPELLCASTING:
        Abilities[ab] += (uint8)pa;
        for(i=0; i!=9; i++) {
            if (SpellTable[Abilities[ab]][i] > SpellSlots[i])
                SpellSlots[i]++;
        }
        if (Abilities[ab] == 1) // 1st level
            BonusSlots[0] = BonusSpells[min(max(IAttr(A_INT)-9,0),21)][0];
        break;     
    case CA_DOMAINS:
        /* Learn the Spells */
        if (!Abilities[ab])
            ChooseDomains();
        Abilities[ab] += (uint8)pa;
        ASSERT(GodID)
        StatiIterNature(this,HAS_DOMAIN)
            TDomain *td = TDOM(S->eID);
            if (td->Spells[NSL[Abilities[ab]]-1])
                Spells[theGame->SpellNum(td->Spells[NSL[Abilities[ab]]-1])] |= SP_DOMAIN | SP_DIVINE | SP_KNOWN;
        StatiIterEnd(this)
        /* Gain the Perks */
        if (RES(sourceID)->Type == T_TDOMAIN)
            Error("Some idiot put CA_DOMAINS as a domain special ability!");
        StatiIterNature(this,HAS_DOMAIN)
            AddAbilities(S->eID,Abilities[CA_DOMAINS]);
        StatiIterEnd(this)
        break;
    case CA_INNATE_SPELL:
        Abilities[ab] = 1;
        i = theGame->SpellNum(pa);
        if (i != -1)
            Spells[i] |= SP_INNATE;
        break;
    case CA_STORYCRAFT:
        tStoryPluses += (int16)pa;
        Abilities[ab] += (uint8)pa;
        break; 
    case CA_TATTOOS:
        {
            bool found;
            int16 i, j;
            rID tatID;

            for (i = 0;i != theGame->LastSpell(); i++) {
                tatID = theGame->SpellID(i);
                if (!TEFF(tatID)->HasSource(AI_TATTOO))
                    continue;
                if (TEFF(tatID)->PEvent(EV_PREREQ,this,tatID) == ABORT)
                    continue;
                for (j = 0; j != 10; j++)
                    if (Tattoos[j] == tatID)
                        goto SkipThisOne;
                found = true;
                MyTerm->LOption(NAME(tatID),tatID, TEFF(tatID)->Describe(this));
SkipThisOne:;
            }
            if (!found) {
                IPrint("Unfortunately, there are no remaining tattoos that you are eligable to take, so you do not gain a new tattoo this level.");
                break;
            }
            tatID = MyTerm->LMenu(MENU_2COLS|MENU_SORTED|MENU_BORDER|MENU_DESC, "Choose a new Mystic Tattoo:");
            ASSERT(tatID);
            for (j =0 ; j != 10; j++)
                if (!Tattoos[j]) {
                    Tattoos[j] = tatID;
                    break;
                }
        }
        break;      
    case CA_FAV_ENEMY:
        MTypePrompt = "Choose a favoured enemy type:";
        st = FAV_ENEMY;
        goto DoCAStati;
    case CA_SMITE:
        MTypePrompt = NULL;
        st = SMITE_ABILITY;
        goto DoCAStati;
    case CA_TURNING:
        MTypePrompt = NULL;
        st = TURN_ABILITY;
        goto DoCAStati;
    case CA_GREATER_TURNING:
        MTypePrompt = NULL;
        st = GTURN_ABILITY;
        goto DoCAStati;
    case CA_COMMAND:
        MTypePrompt = NULL;
        st = COMMAND_ABILITY;
        goto DoCAStati;
    case CA_SPECIES_AFFINITY:
        MTypePrompt = NULL;
        st = SPECIES_AFFINITY;
        goto DoCAStati;

DoCAStati:
        Abilities[ab] = 1;
        StatiIterNature(this,st)
            if ((S->Source == statiSource) && (!(pa & 0x0000FFFF) || (S->Val == (pa & 0x0000FFFF)) || (S->CLev == (pa - MA_CHOICE1 + 1)))) {
                if (pa & 0xFFFF0000)
                    S->Mag += (pa >> 16);
                else
                    S->Mag++;
                StatiIterBreakout(this,return)
            }
        StatiIterEnd(this)

        if (pa >= MA_CHOICE1 && pa <= MA_CHOICE5) {
            // ww: let's restructure this a bit, because RangerEnemies
            // falls out of sync with what's really there ...
            for (i=0; FavEnemies[i]; i++) {
                /*if (i == MA_MISC_OUTSIDER || 
                i == MA_NATIVE ||
                i == MA_OUTSIDER ||
                i == MA_HUMANOID ||
                i == MA_NLIVING ||
                i == MA_ANIMAL ||
                i == MA_EARTH_ELEM ||
                i == MA_AIR_ELEM ||
                i == MA_FIRE_ELEM ||
                i == MA_WATER_ELEM ||
                i == MA_JELLY || // ww: covered by ooze = pudding + jelly
                i == MA_ELEMENTAL ||
                // i == MA_ADVENTURER ||
                (i >= MA_FORCE_D12 && i <=  MA_FORCE_D4))
                continue;      */
                desc = "";
                category = "";
                int count_in_this_ma = 0; 

                desc += -EMERALD;
                for (j=1;j!=12;j++) {
                    if (j != 1)
                        desc += "/";
                    desc += Format("%+d",CalcFEBonus(FavEnemies[i],j));
                }
                desc += "\n";
                desc += -GREY;

                for (int modIdx = 0; modIdx < 1; modIdx++) {
                    Module *mod = theGame->Modules[modIdx];
                    for (int idx = 0; idx < mod->szMon; idx++) { 
                        rID _mID = mod->MonsterID(idx);
                        if (!(mod->QMon[idx].isMType(_mID,FavEnemies[i])))
                            continue;
                        if (mod->QMon[idx].HasFlag(M_NOGEN))
                            continue;

						uint32 gid = GLYPH_ID_VALUE(mod->QMon[idx].Image);
                        int32 color = -(int32)GLYPH_COLOUR_VALUE(mod->QMon[idx].Image);
                        if (color == BLACK) color = -SHADOW;
                        desc += Format("%c%s (%c%c%c%c%c CR %d). ",
							-GREY, (const char*)Capitalize(NAME(_mID)),
							color, LITERAL_CHAR, LITERAL_CHAR1(gid), LITERAL_CHAR2(gid) , - GREY,
                            mod->QMon[idx].CR);
                        count_in_this_ma++;
                    }
                } 
                if (count_in_this_ma) {
                    const char *Rarities[] = { "", "Common", "Uncommon", "Rare", "Very Rare", "Obscure" };
                    for (j=0;MonGroupRarities[j];j+=2) 
                        if (MonGroupRarities[j] == FavEnemies[i]) {
                            category = Format("%s [%s]", (const char*)
                                Pluralize(Lookup(MTypeNames,FavEnemies[i])),
                                Rarities[MonGroupRarities[j+1]]);
                            goto Found;
                        }
                    Error("Favoured Enemy MA_TYPE not found in MonGroupRarities!");
                    continue;
Found: 
                    MyTerm->LOption(category,FavEnemies[i],desc);
                }
            } 
            j = (int16)MyTerm->LMenu(MENU_SORTED|MENU_2COLS|MENU_DESC|MENU_BORDER,MTypePrompt);
#if 0
            do { 
                j = MyTerm->MonsterTypePrompt(MTypePrompt,10,50);
            } 
            while (j <= 0);
#endif
        } else
            j = (int16)pa;                                                      
        /* was sourceID */
        /* ww: this looks a little wrong to me ... 
        * The source isn't kept correctly, yada yada. 
        GainPermStati(st,NULL,statiSource,j,
        (pa & 0xFFFF0000) ? (pa >> 16) : 1,0);
        */
        if (pa >= MA_CHOICE1 && pa <= MA_CHOICE5)
            GainPermStati(st,NULL,(int8)statiSource,j,
            (pa & 0xFFFF0000) ? (pa >> 16) : 1,sourceID,
            (int8)(pa - MA_CHOICE1 + 1));
        else
            GainPermStati(st,NULL,(int8)statiSource,j,
            (pa & 0xFFFF0000) ? (pa >> 16) : 1,sourceID,0);
        // GetStati(st,j)->Duration = (pa & 0xFFFF);
        break;
    case CA_SPECIALIST:
        if (!Abilities[ab]) {
            for(i=0;i!=9;i++) {
                if (isMType(MA_ELF) && XBIT(i) == SC_NEC)
                    continue;
                MyTerm->LOption(Lookup(SchoolNames,XBIT(i)),i,"");
            }
            i = (int16)MyTerm->LMenu(MENU_3COLS,
                "__As a Wizard, you are entitled to choose "
                "one of the nine schools of magic to be your special area of "
                "focus. Every specialty makes for a distinctly different style "
                "of spellcaster, so choose carefully. Type '?' for a description "
                "of the various specialties and their impact on spellcasting.",WIN_CREATION,
                "help::magic,SC");
            GainPermStati(SPECIALTY_SCHOOL,NULL,(int8)statiSource,i,0,sourceID);
        }
        Abilities[ab] += (uint8)pa;
        break;    
    case CA_UNCANNY_DODGE:
        Abilities[ab] += (uint8)pa;
        if (Abilities[ab] >= 6 && (Abilities[ab] % 3 == 0)) {
            StatiIterNature(this,SAVE_BONUS)
                if (S->Val == SN_TRAPS)
                    if (S->Source == SS_CLAS) {
                        S->Mag++;
                        StatiIterBreakout(this,goto DoneTrapBonus)         
                    }
            StatiIterEnd(this)
            GainPermStati(SAVE_BONUS,NULL,(int8)statiSource,SN_TRAPS,1);
DoneTrapBonus:;
        }
        break;
    case CA_GRANT_ITEM:
        Item *it;
        it = Item::Create(pa);
        if (it)
            GainItem(it);
        Abilities[ab] = 0;
        break;
    default:
        Abilities[ab] += (uint8)pa;
        break;
    }
}

void Character::UpdateTattoos() {
    int16 i, ocHP; EventInfo e;

    if (!Tattoos[0])
        return;

    SetSilence();

    /* Let's not drop dead on the spot upon losing a Con-
    granting tattoo! */
    ocHP = cHP;
    cHP  = mHP;

    for (i=0;Tattoos[i];i++)
        RemoveEffStati(Tattoos[i]);

    for (i=0;Tattoos[i];i++)
    {
        e.Clear();
        e.eID = Tattoos[i];
        e.EActor = this;
        e.EVictim = this;
        e.isWield = true;
        ReThrow(EV_EFFECT,e);
        if (TEFF(Tattoos[i])->HasFlag(EF_ACTIVATE) ||
            TEFF(Tattoos[i])->HasFlag(EF_ACTIVATE2) ||
            TEFF(Tattoos[i])->HasFlag(EF_ACTIVATE3) ||
            TEFF(Tattoos[i])->HasFlag(EF_ACTIVATE4) ||
            TEFF(Tattoos[i])->HasFlag(EF_ACTIVATE5))
            GainPermStati(ACTIVATION,NULL,SS_MISC,0,0,Tattoos[i],0);
    }

    cHP = ocHP;

    UnsetSilence();
}


bool Character::FeatPrereq(int16 feat, bool fail_if_feat_requires_a_feat)
{
    int8 BAB[5] = {0,0,0,0,0};
    int16 i; 

    if (feat == FT_INTENSIVE_STUDY)
        if (getEligableStudies() == 0)
            return false;

    /* Fix for War Domain & Shield feats */    
    if (feat == FT_AP_LIGHT)
        if (Proficiencies & WG_LARMOUR)
            return false;
    if (feat == FT_AP_MEDIUM)
        if (Proficiencies & WG_MARMOUR)
            return false;
    if (feat == FT_AP_HEAVY)
        if (Proficiencies & WG_HARMOUR)
            return false;

    for (i=0; FeatInfo[i].feat; i++) 
        if (FeatInfo[i].feat == feat) {
            if (FeatInfo[i].flags & (FF_UNIMP|FF_MONSTER)) 
                return false;
            else if (HasFeat(feat) && (!(FeatInfo[i].flags & FF_MULTIPLE)))
                return false; 
            else if (FeatInfo[i].flags & FF_META && CasterLev() == 0)
                return false;
            else if (FeatInfo[i].pre.Or[0].And[0].elt == FP_ALWAYS)
                return true;
            /* otherwise we have to interpret the conditions! */

            for (int16 d=0; d<FP_MAX_DISJUNCTS; d++) {
                if (FeatInfo[i].pre.Or[d].And[0].elt == FP_ALWAYS)
                    break; 
                bool all_true_so_far = true;
                for (int16 c=0; c<FP_MAX_CONJUNCTS; c++) {
                    struct FeatConjunct * fc = &FeatInfo[i].pre.Or[d].And[c];
                    if (fc->elt == FP_ALWAYS) break; 
                    switch (fc->elt) {
              case FP_FEAT: 
                  if (fail_if_feat_requires_a_feat)
                      all_true_so_far = false;
                  else if (!IHasFeat((int16)fc->arg))
                      all_true_so_far = false; 
                  break;
              case FP_ABILITY:
                  if ( (!IHasAbility((int16)fc->arg)) ||
                      (AbilityLevel((int16)fc->arg) < fc->val) )
                      all_true_so_far = false; 
                  break; 
              case FP_BAB: 
                  if (GetBAB((int16)fc->arg) < fc->val) 
                      all_true_so_far = false;
                  break; 
              case FP_NOT_PROF:
                  if (Proficiencies & fc->arg)
                      all_true_so_far = false;
                  break;
              case FP_PROF:
                  if (!(Proficiencies & fc->arg))
                      all_true_so_far = false;
                  break;
              case FP_CR: 
                  if (ChallengeRating() < fc->arg)
                      all_true_so_far = false;
                  break;
              case FP_ATTR: 
                  /* Do *not* let a player put on a Headband of Intellect
                  just long enough to learn a feat! */
                  if (IAttr((int8)fc->arg) < fc->val)
                      all_true_so_far = false;
                  break;
              case FP_SKILL: 
                  if (ISkillLevel((int16)fc->arg) < fc->val) 
                      all_true_so_far = false;
                  break; 
              case FP_CASTER_LEVEL: 
                  if (CasterLev() < fc->arg)
                      all_true_so_far = false;
                  break;
              case FP_WEP_SKILL: 
                  if (!HasStati(WEP_SKILL,(int16)fc->arg))
                      all_true_so_far = false;
                  break;
              case FP_ATTACK: 
                  if (!TMON(tmID)->HasAttk((int8)fc->arg))
                      all_true_so_far = false;
                  break;
              case FP_MTYPE: 
                  if (!TMON(tmID)->isMType(tmID,fc->arg))
                      all_true_so_far = false;
                  break;              
              default: Error("Character::FeatPrereq unknown FP_");
                  break; 
                    } 
                } 
                if (all_true_so_far)
                    return true; 
                // else try the next disjunct
            } 
            return false; 
        } 
        // should never get here
        return false; 
}


uint16 Character::getEligableStudies()
{
    uint16 eli;
    int16 clev = TotalLevel();
    int16 mlev = ((clev*4)+4)/5;
    int16 i, j;

    /* If you want to tweak the numbers for Intensive
    study, make sure they match both here and in
    Creature::AbilityLevel()! */

    eli = 0;
    for (i=0;Studies[i][0];i++)
        if (Abilities[Studies[i][1]])
            if (Studies[i][3] * (Abilities[Studies[i][1]] + 
                IntStudy[Studies[i][0]] * Studies[i][2]) < mlev)
                if (Abilities[Studies[i][1]] > IntStudy[Studies[i][0]]*Studies[i][2])
                    eli |= XBIT(Studies[i][0]);


    int8 warriorLevels[4] = { 0, 0, 0, 0 };

    for (i=0;i!=4;i++)
        for (j=0;j!=3 && ClassID[j];j++)
            if (TCLASS(ClassID[j])->AttkVal[i] >= 100)
                warriorLevels[i] += Level[j];

    for (i=0;i!=4;i++)
        if (warriorLevels[i] > IntStudy[STUDY_BAB])
            if (GetBAB(i) < TotalLevel())
                eli |= XBIT(STUDY_BAB);

    return eli;    
}

int8 Creature::WepSkill(Item *it)
{
    if (!it) return WepSkill((rID)0);
    else return WepSkill(it->iID) + it->HasQuality(IQ_PROFICIENT); 
} 
int8 Character::WepSkill(Item *it)
{
    if (!it) return WepSkill((rID)0);
    else return WepSkill(it->iID) + it->HasQuality(IQ_PROFICIENT); 
} 

int8 Character::WepSkill(rID wID, bool ignore_str)
{     
    int16 mstr;
    int8 best = 0;

    if (wID && !ignore_str) {
        mstr = 5;
        if (TITEM(wID)->HasFlag(WT_STR1))
            mstr += 8;
        if (TITEM(wID)->HasFlag(WT_STR2))
            mstr += 4;
        if (TITEM(wID)->HasFlag(WT_STR3))
            mstr += 2;
        if (GetAttr(A_STR) < mstr && mstr > 5)
            return WS_NOT_PROF;
    }

    StatiIterNature(this,WEP_SKILL)
        if (S->eID == wID)
            best = max(best,S->Val);
    StatiIterEnd(this)
        if (best) return best; 
    if (wID == 0) return WS_NOT_PROF; 
    if (TITEM(wID)->Group & WG_LARMOUR)
        if (HasFeat(FT_AP_LIGHT))
            return WS_PROFICIENT;
    if (TITEM(wID)->Group & WG_MARMOUR)
        if (HasFeat(FT_AP_MEDIUM))
            return WS_PROFICIENT;
    if (TITEM(wID)->Group & WG_HARMOUR)
        if (HasFeat(FT_AP_HEAVY))
            return WS_PROFICIENT;
    if (TITEM(wID)->Group & WG_SHIELDS)
        if (HasFeat(FT_SHIELD_PROFICIENCY))
            return WS_PROFICIENT;

    if (TITEM(wID)->Group & WG_EXOTIC)
        return WS_NOT_PROF;

    return (Proficiencies & TITEM(wID)->Group) ? 
WS_PROFICIENT : WS_NOT_PROF;
}

int8 Creature::WepSkill(rID wID, bool ignore_str)
{
    if (wID != 0 && TITEM(wID)->Group & WG_EXOTIC) 
        if (!HasFeat(FT_EXOTIC_WEAPON)) 
            return WS_NOT_PROF;
    if (HasFeat(FT_WEAPON_MASTERY))
        return WS_MASTERY; 
    else if (HasFeat(FT_WEAPON_SPECIALIST))
        return WS_SPECIALIST; 
    else if (HasFeat(FT_WEAPON_FOCUS))
        return WS_FOCUSED; 
    else 
        return WS_PROFICIENT;
}


bool Character::HasFeat(int16 ft, bool inh, bool list)
{
    /*
    if (ft <= SK_LAST)
    if (GetStatiMag(SKILL_BONUS,ft) >= 5)
    return true;
    */

    if (!list) {
        if (ft == FT_AP_LIGHT)
            if (Proficiencies & WG_LARMOUR)
                return true;
        if (ft == FT_AP_MEDIUM)
            if (Proficiencies & WG_MARMOUR)
                return true;
        if (ft == FT_AP_HEAVY)
            if (Proficiencies & WG_HARMOUR)
                return true;
        if (ft == FT_SHIELD_PROFICIENCY)
            if (Proficiencies & WG_SHIELDS)
                return true;
    }

    StatiIter(this) 
        if (S->Nature == EXTRA_FEAT && S->Val == ft)
            if ((!inh) || S->Source == SS_DOMA || S->Source == SS_BLES ||
                S->Source == SS_CLAS || S->Source == SS_PERM)
                StatiIterBreakout(this,return true)
                if (S->Nature == EXTRA_SKILL && S->Val == ft) 
                    if ((!inh) || S->Source == SS_DOMA || S->Source == SS_BLES ||
                        S->Source == SS_CLAS || S->Source == SS_PERM)
                        StatiIterBreakout(this,return true)
                        if (S->Nature == TEMPLATE && TTEM(S->eID)->HasFeat(ft) && !inh)
                            StatiIterBreakout(this,return true)
                            if (S->Nature == CONDITIONAL_FEAT && S->Val == ft && !inh)
                            {
                                EvReturn r;
                                PEVENT(EV_ISTARGET,this,S->eID,,r);
                                if (r)
                                    StatiIterBreakout(this,return true)
                            }   
                            /* Allow conditional feats from class or race (i.e., ranger
                            light armour feats) to serve as prerequisites, but not
                            conditional feats from items or spells. */
                            if (S->Nature == CONDITIONAL_FEAT && S->Val == ft && inh)
                                if (S->Source == SS_CLAS || S->Source == SS_RACE)
                                    StatiIterBreakout(this,return true);
                            StatiIterEnd(this)

#if 0
                                /* You *aren't* supposed to get a creature's feats when you
                                change into it. Exception for Natural Grapple? */
                                if (HasStati(POLYMORPH)) {
                                    if (TMON(mID)->HasFeat(ft))
                                        return true; 
                                } else if (TMON(tmID)->HasFeat(ft))
                                    return true; 
#endif

                                return (Feats[ft/8] & (1 << (ft%8))) != 0;
}

int16 Character::MaxRanks(int16 sk)
{    

    int16 mr,i,lv;
    /* The intent behind this is that the max ranks grow
    more slowly than the number of skills, allowing
    the player to broaden out as he increases in level,
    maxing out more skills as he advances while still
    having enough points to keep his original skills
    maxed. In the original OGL system, you had [Int + 2/4/8]
    skills maxed at first level, and that number could
    never increase if you wanted to keep those skills
    maxed -- I hated that as a player and a designer. */
    int8 ClassLevelRanks[] =
    { 0,  2,  4,  6,  7,  8,  9,  10,  10,  11, 12, 12, 
    13, 14, 14, 15, 16, 16, 17, 18, 18, 19, 

    /* Paranoia */
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20 };

    if (TRACE(RaceID)->HasSkill(sk))
        return ClassLevelRanks[TotalLevel()];


    mr = 0; lv = 0;
    for(i=0;i!=3;i++)
        if (ClassID[i])
            if (TCLASS(ClassID[i])->HasSkill(sk))
                lv += Level[i];
    mr += ClassLevelRanks[lv];

    if (isFamiliar(sk))
        mr = max(mr,min(TotalLevel(),5));

    /* Each Intensive Study (Sneak Attack & Stealth) increases
    your maximum Hide and Move Silently ranks by 2, up to a
    maximum of what a rogue of your character level would have. */
    if (sk == SK_HIDE || sk == SK_MOVE_SIL)
        if (mr < ClassLevelRanks[TotalLevel()])
            mr = min(mr + IntStudy[STUDY_SNEAK]*2,
            ClassLevelRanks[TotalLevel()]);

    StatiIter(this)
        if (S->Nature == EXTRA_SKILL)
            if (S->Val == sk) {
                if (S->Source == SS_DOMA)
                    mr += ClassLevelRanks[AbilityLevel(CA_DOMAINS)];
                else
                    StatiIterBreakout(this,return ClassLevelRanks[TotalLevel()])
            }
            StatiIterEnd(this)

                return mr;
}

bool Creature::HasSkill(int16 sk, bool check_allies)
{ if (HasStati(EXTRA_SKILL,sk))
return true;
return HasFeat(sk); }

bool Character::HasSkill(int16 sk, bool check_allies)
{
    int16 i; Creature *cr;
    if (check_allies)       
        if (m)
            MapIterate(m,cr,i)
            if (cr->isCreature() && cr != this)
                if (DistFrom(cr) < 6 && cr->HasSkill(sk))
                    if (cr->isFriendlyTo(this))
                        if (Perceives(this) &&
                            m->LineOfFire(x,y,cr->x,cr->y,cr))
                            return true;


    if (RaceID)
        if (TRACE(RaceID)->HasSkill(sk))
            return true;
    for(i=0;i!=3;i++)
        if (ClassID[i] && Level[i])
            if (TCLASS(ClassID[i])->HasSkill(sk))
                return true;
    if (HasStati(EXTRA_SKILL,sk))
        return true;
    return false;
}

int16 Character::LevelAs(rID cl)
{
    if (ClassID[0] == cl) return max(1,Level[0]);
    if (ClassID[1] == cl) return Level[1];
    if (ClassID[2] == cl) return Level[2];
    return 0;
}

int16 Character::IAttr(int8 a)
{
    return BAttr[a] + (RaceID ? TRACE(RaceID)->AttrAdj[a] : 0 ) +
        HasFeat(FT_IMPROVED_STRENGTH+a) +
        SumStatiMag(ADJUST_INH, a);
}

int16 Character::CasterLev()
{        
    int16 ab = AbilityLevel(CA_SPELLCASTING);
    /* Hack for 1st level bards */
    if (HasStati(BONUS_SLOTS))
        ab = max(ab,1);
    return ab;
}

int16 Creature::SkillLevel(int16 sk)
{
    int16 sr = 0, i,j,b = 0;
#define FEAT_SKILL_BONUS        3


    s_racial = s_feat = s_enhance = s_domain = s_armour = s_kit = s_syn = 
        s_item = s_comp = s_circ = s_inh = s_size = s_train = s_focus = s_ins = 0;

    if (sk == SK_LISTEN || sk == SK_SPOT)
        if (HasFeat(FT_ALERTNESS))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_JUMP || sk == SK_TUMBLE)
        if (HasFeat(FT_ACROBATIC))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_ATHLETICS || sk == SK_CLIMB || sk == SK_SWIM)
        if (HasFeat(FT_ATHLETIC))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_CRAFT || sk == SK_HANDLE_DEV)
        if (HasFeat(FT_ARTIFICER))
            s_feat = FEAT_SKILL_BONUS;
    if (sk >= SK_KNOW_FIRST && sk <= SK_KNOW_LAST)
        if (HasFeat(FT_EDUCATED))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_APPRAISE || sk == SK_INTUITION)
        if (HasFeat(FT_INSIGHTFUL)) /* Deprecated */
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_LOCKPICKING || sk == SK_PICK_POCKET)
        if (HasFeat(FT_LARCENOUS))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_DIPLOMACY || sk == SK_PERFORM)
        if (HasFeat(FT_CAPTIVATING))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_HIDE || sk == SK_MOVE_SILENTLY) {
        if (HasFeat(FT_SNEAKY))
            s_feat += FEAT_SKILL_BONUS;
        if (HasStati(CHARGING))
            s_circ -= 20; 
    }
    if (sk == SK_WILD_LORE || sk == SK_ANIMAL_EMP)
        if (HasFeat(FT_WOODSMAN))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_BALANCE || sk == SK_ESCAPE_ART || sk == SK_SWIM)
        if (HasFeat(FT_GRACEFUL))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_APPRAISE || sk == SK_SEARCH || sk == SK_GATHER_INF)
        if (HasFeat(FT_DETECTIVE))
            s_feat = FEAT_SKILL_BONUS;    
    if (sk == SK_DECIPHER || sk == SK_USE_MAGIC)
        if (HasFeat(FT_TALENTED))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_METAMAGIC || sk == SK_SPELLCRAFT)
        if (HasFeat(FT_GUILDMAGE))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_HEAL || sk == SK_ALCHEMY)
        if (HasFeat(FT_PHYSICIAN))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_FIND_WEAKNESS || sk == SK_POISON_USE)
        if (HasFeat(FT_MURDEROUS))
            s_feat = FEAT_SKILL_BONUS;      
    if (sk == SK_CONCENT || sk == SK_INTUITION)
        if (HasFeat(FT_CLEAR_MINDED))
            s_feat = FEAT_SKILL_BONUS;
    if (sk == SK_RIDE || sk == SK_SENESCHAL)
        if (HasFeat(FT_LANDED_NOBLE))
            s_feat = FEAT_SKILL_BONUS;      
    if (sk == SK_BLUFF || sk == SK_DISGUISE || sk == SK_ILLUSION)
        if (HasFeat(FT_DECIEVER))
            s_feat = FEAT_SKILL_BONUS;    

    if (sk == SK_INTUITION)
        if (HasAbility(CA_FATESENSE))
            s_domain += 4; 

    if (sk == SK_HIDE) {
        if (HasStati(ELEVATED,ELEV_TREE))
            s_circ += 2 + SkillLevel(SK_CLIMB)/3;
        else if (HasStati(ELEVATED,ELEV_CEILING))
            s_circ += 2 + SkillLevel(SK_CLIMB)/3;
    }

    if (sk == SK_SPOT || sk == SK_LISTEN)
        s_inh = AbilityLevel(CA_SHARP_SENSES);

    if (sk == SK_SEARCH)
        s_inh = AbilityLevel(CA_STONEWORK_SENSE);

    if (sk == SK_KNOW_GEO || sk == SK_KNOW_INF || sk == SK_KNOW_MAGIC ||
        sk == SK_KNOW_MYTH || sk == SK_KNOW_NATURE || sk == SK_KNOW_OCEANS ||
        sk == SK_KNOW_PLANES || sk == SK_KNOW_UNDEAD || sk == SK_KNOW_THEO)
        s_inh = AbilityLevel(CA_LEGEND_LORE) / 2;

    s_kit = SkillKitMod(sk);


    if (isCharacter()) {
        for (i=0;Synergies[i][0];i++)
            if (Synergies[i][0] == sk)
                if (thisp->SkillRanks[Synergies[i][1]])
                    s_syn += thisp->SkillRanks[Synergies[i][1]] /
                    Synergies[i][2];


        sr = thisp->SkillRanks[sk];
        if (HasStati(ANC_MEM,sk))
        {
            int16 mr = thisp->MaxRanks(sk);
            s_ins = max(s_ins,4 - abs(mr-sr));
            sr = mr;
        }
        if (thisp->RaceID)
            if (TRACE(thisp->RaceID)->HasSkill(sk))
                s_train += 1;
        for (i=0;i != 3 && thisp->ClassID[i];i++)  
            if (TCLASS(thisp->ClassID[i])->HasSkill(sk))
                s_train += 1;
        if (!s_train && HasStati(EXTRA_SKILL,sk))
            s_train = 1;
        if (s_train)
            s_train++;   
    }
    else if (HasFeat(sk)) {
        // assume monsters get Level+3 for class skills like we do!
        sr = max(1,ChallengeRating()) + 3;
        for (i=0;Synergies[i][0];i++)
            if (Synergies[i][0] == sk)
                if (HasFeat(Synergies[i][1]))
                    s_syn += (max(1,ChallengeRating())+3) /
                    Synergies[i][2];
        TMonster *tm = TMON(tmID);
        TTemplate *tt;
        for(i=0;i!=15;i++)
            if (tm->Feats[i] == FT_SKILL_FOCUS)
                if (tm->Feats[i+1] == sk)
                { 
                    sr += 2;           
                    goto FoundFocus; }

                StatiIter(this)
                    if (S->Nature == TEMPLATE) {
                        tt = TTEM(S->eID);
                        for(i=0;i!=16;i++)
                            if (tt->NewFeats[i] == FT_SKILL_FOCUS)
                                if (tt->NewFeats[i+1] == sk)
                                { 
                                    sr += 2;
                                    StatiIterBreakout(this,goto FoundFocus) 
                                }
                    }
                    StatiIterEnd(this)
FoundFocus:
                    ;      
    }
    else if (sk == SK_SPOT || sk == SK_LISTEN)
        sr = ChallengeRating() / 2;

    StatiIterNature(this,SKILL_BONUS)
        if (S->Val == sk || S->Val == -1)
            switch (S->Source) {
          case SS_CLAS:
              s_feat = max(s_feat,S->Mag);
              break;
          case SS_BODY:
          case SS_TMPL:
          case SS_RACE:
              s_racial = max(s_racial,S->Mag);
              break;
          case SS_DOMA:
              s_domain = max(s_domain,S->Mag);
              break;
          case SS_ITEM:
              s_item = max(s_item,S->Mag);
              break;
          case SS_PERM:
              s_focus = max(s_focus,S->Mag);
              break;
          default:
              s_enhance = max(s_enhance,S->Mag);
              break;
        }
        StatiIterEnd(this)

            if (sk == SK_HIDE)
                s_size = (SZ_MEDIUM - Attr[A_SIZ])*4;

        if (HasStati(SINGING) && sk != SK_PERFORM)
            s_circ -= 2;

        if (SkillInfo[sk].armour_penalty) {
            Item *it; 
            static int16 armourSlots[] = {
                SL_ARMOUR, SL_READY, SL_WEAPON, 0
            } ;
            for (j=0; armourSlots[j]; j++) {
                i = armourSlots[j];
                if ((it = InSlot((int8)i)) && it->activeSlot(i))  {
                    /* count a two-handed shield only once! */
                    if (i == SL_WEAPON) 
                        if (InSlot(SL_READY) == InSlot(SL_WEAPON))
                            continue;
                    if (it->isType(T_SHIELD) || it->isType(T_ARMOUR)) {
                        int pval = ((Armour *)it)->PenaltyVal(this,true) ;
                        if (sk == SK_SWIM && it->HasQuality(IQ_FEATHERLIGHT))
                            s_armour += pval;
                        else
                            s_armour += SkillInfo[sk].armour_penalty * pval;
                    } 
                } 
            }
        }

        sr += s_racial + s_feat + s_enhance + s_domain + s_item + s_ins + s_syn + 
            s_comp + s_circ + s_inh + s_size + s_armour + s_train + s_kit + s_focus;
        sr += Mod2((int8)SkillAttr(sk)); 

        return sr;
}

int16 Creature::ISkillLevel(int16 sk)
{
    int16 sr;
    /* Calculate the skill level with all mods */
    sr = SkillLevel(sk);

    /* Then subtract those that aren't inherent */
    sr -= (s_enhance + s_kit + s_syn + s_armour + 
        s_size + s_circ + s_comp + s_item);

    /* Remove Attribute Mod */
    sr -= Mod2((int8)SkillAttr(sk));

    /* Add Inherent Attribute Mod */
    sr += (IAttr((int8)SkillAttr(sk))-11)/2;

    return sr;
}

int16 Creature::SkillAttr(int16 sk) {
    int8 attr1, attr2; 
    attr1 = SkillInfo[sk].attr1;
    attr2 = SkillInfo[sk].attr2;
    if (HasFeat(FT_ONE_BODY_ONE_SOUL) || (Attr[A_CON] == 0))  {
        if (Attr[A_WIS] > Attr[A_CON]) {
            if (attr1 == A_CON) attr1 = A_WIS;
            if (attr2 == A_CON) attr2 = A_WIS;
        } else {
            if (attr1 == A_WIS) attr1 = A_CON;
            if (attr2 == A_WIS) attr2 = A_CON;
        } 
    } 
    if (attr1 == attr2) 
        return attr1;
    else {
        if (SkillInfo[sk].take_min) 
            return Attr[attr1] > Attr[attr2] ? attr2 : attr1;
        else 
            return Attr[attr1] > Attr[attr2] ? attr1 : attr2;
    } 
}


void Character::TrainAs(rID cID) {
    char ch;
    if (Level[2] && Level[1]) {
        IPrint("Sadly, you can possess at most three classes.");
        return;
    }
    if (!ClassID[1])
        ClassID[1] = cID;
    else if (!ClassID[2])
        ClassID[2] = cID;
    else {
        if (Type != T_PLAYER)
            return;
        if (Level[2]) {
            if (!yn(Format("Do you wish to replace your training as a %s? ", NAME(ClassID[1]))))
                return;
            ClassID[1] = cID;
            return;
        } else if (Level[1]) {
            if (!yn(Format("Do you wish to replace your training as a %s? ", NAME(ClassID[2]))))
                return;
            ClassID[2] = cID;
            return;
        } else {
            ch = thisp->MyTerm->ChoicePrompt(Format(
                "Replace your training as a %s [1], a %s [2] or [n]either? ",
                NAME(ClassID[1]), NAME(ClassID[2])), "12n");
            if (ch == '1')
                ClassID[1] = cID;
            if (ch == '2')
                ClassID[2] = cID;
            return;
        }
    }
}

int8 Creature::rateMeleeWeapon() {
    int8 rating = 0; Item *it;
    for (it=FirstInv();it;it=NextInv())
        if (it->isType(T_WEAPON) && !it->isType(T_BOW)
            && !it->isType(T_MISSILE))
        {
            if (it->iID == FIND("pickaxe"))
                continue;
            if (it->isGroup(WG_THROWN))
                continue;
            rating = max(1,rating);
            if (!it->isGroup(WG_SIMPLE) &&
                !it->isGroup(WG_DAGGERS) &&
                !it->isGroup(WG_LIGHT) &&
                stricmp("knife",NAME(it->iID)) &&
                (stricmp("short sword",NAME(it->iID))
                || isSmallRace()))
                rating = max(2,rating);
            if (WepSkill(it->iID) >= WS_FOCUSED)
                rating = max(3,rating);
        }
        return rating;
}

int8 Creature::rateRangedWeapon()
{
    int8 rating = 0; Item *it;
    for (it=FirstInv();it;it=NextInv())
        if (it->isType(T_BOW))
        {
            rating = max(1,rating);
            if (it->iID == FIND("cranquin") ||
                it->iID == FIND("long bow") ||
                it->iID == FIND("composite long bow"))
                rating = max(2,rating);
            if (WepSkill(it->iID) >= WS_FOCUSED)
                rating = max(3,rating);
        }
        return rating;
}  

Item* Creature::getPrimaryMelee()
{
    int8 brate = 0; Item *it, *best;
    best = NULL;
    for (it=FirstInv();it;it=NextInv())
        if (it->isType(T_WEAPON) && !it->isType(T_BOW)
            && !it->isType(T_MISSILE))
        {
            if (it->iID == FIND("pickaxe"))
                continue;
            if (it->isGroup(WG_THROWN) &&
                !it->isGroup(WG_DAGGERS))
                continue;
            if (it->eID)
                continue;
            if (brate < 1) {
                brate = 1;
                best = it;
            }
            if (it->iID == FIND("cranquin") ||
                it->iID == FIND("long bow") ||
                it->iID == FIND("composite long bow"))
                if (brate < 2) {
                    brate = 2;
                    best = it;
                }
                if (WepSkill(it->iID) >= WS_FOCUSED)
                    if (brate < 3) {
                        brate = 3;
                        best = it;
                    }
        }
        return best;
}

Item* Creature::getPrimaryRanged()
{
    int8 brate = 0; Item *it, *best;
    best = NULL;
    for (it=FirstInv();it;it=NextInv())
        if (it->isType(T_BOW))
        {
            if (it->eID)
                continue;
            if (brate < 1) {
                brate = 1;
                best = it;
            }
            if (!it->isGroup(WG_SIMPLE) &&
                !it->isGroup(WG_DAGGERS) &&
                !it->isGroup(WG_LIGHT)) 
                if (brate < 2) {
                    brate = 2;
                    best = it;
                }
                if (WepSkill(it->iID) >= WS_FOCUSED)
                    if (brate < 3) {
                        brate = 3;
                        best = it;
                    }
        }
        return best;
}

int CompareLSpells(const void *a, const void *b)
{
    LearnableSpell *A, *B;
    A = (LearnableSpell*) a;
    B = (LearnableSpell*) b;

    if (A->Level > B->Level)
        return 1;
    else if (A->Level < B->Level)
        return -1;
    else 
        return stricmp(NAME(A->spID),NAME(B->spID));
}

LearnableSpell* Character::CalcSpellAccess()
{
    int16 i, j, k, sp, lev, c; rID spID;
    rID SpellList[MAX_SPELLS+1];
    LearnableSpell *ls; bool skip, added;

    /* This is more than we'll ever need -- it assumes a player
    could have access to every spell in the game as arcane,
    divine, primal AND sorcerous -- but paranoid is better
    than obscure memory corruption errors. */
    ls = (LearnableSpell*) malloc(sizeof(LearnableSpell) * (MAX_SPELLS+1)*4);

/*
sp: rID
lv: uint8
typ: uint8
*/
#define ADD_SPELL(sp,lv,typ)                    \
    {                                             \
    Spells[theGame->SpellNum(sp)] |= typ;       \
    added = false;                              \
    for (k=0;k!=c;k++)                          \
    if (ls[k].spID == sp)                     \
    if (ls[k].Type == typ)                  \
    {                                     \
    ls[k].Level = min(ls[k].Level,lv);  \
    added = true;                       \
    }                                     \
    if (!added) {                               \
    ls[c].spID = sp;                          \
    ls[c].Level = lv;                         \
    ls[c].Type = typ;                         \
    c++;                                      \
    }                                         \
    ASSERT(c < (MAX_SPELLS*4));                 \
    }                                             

    for (i=0;i!=MAX_SPELLS;i++)
        Spells[i] &= ~(SP_ARCANE|SP_DIVINE|SP_PRIMAL|SP_BARDIC);      

    c = 0; lev = 1;
    StatiIter(this)
        if (S->Nature == SPELL_ACCESS)
        {
            if (RES(S->eID)->GetList(SPELL_LIST,SpellList,MAX_SPELLS+1))
            {
                skip = false;
                /* 3 zeros serve as the null terminator. We can't use just
                one, because if we have an illegal referance in a class
                spell list (i.e., a spell not implemented yet, or a
                misspelled spell name), we don't want to terminate the
                entire list at that point. */
                for (i=0;SpellList[i] || SpellList[i+1] || SpellList[i+2];i++)
                {
                    /* Less than 256 is a spell level rather than a 
                    spell entry. S->Mag stores the maximum level
                    of spell access. */
                    if (SpellList[i] == 0)
                        ;
                    else if (SpellList[i] < 50)
                    {
                        lev = (int16)SpellList[i];
                        if ((int)SpellList[i] > S->Mag)
                            skip = true;
                        else
                            skip = false;
                        continue;
                    }
                    else if (SpellList[i] > 1000 && SpellList[i] < 2000)
                    {
                        int16 Source = (int16)SpellList[i] - 1000;
                        for(sp=0;sp!=theGame->LastSpell();sp++)
                        {
                            spID = theGame->SpellID(sp);
                            if (TEFF(spID)->Level > S->Mag)
                                continue;
                            if (TEFF(spID)->HasSource((int8)Source))
                            {
                                /* If the spell is mentioned elsewhere on *this* list,
                                we want to avoid adding it twice. The rationale for
                                this is lists that alter levels and leave everything
                                else at its default level. For example, Call Light
                                might be a priest 1 spell and a wizard 2 spell, so
                                we want to be able to put in the wiz list:
                                ALL_SPELLS_FROM_SOURCE(AI_WIZARDRY)
                                2 $"call light"
                                and have call light be level two, not be entered twice
                                as both levels one and two. */
                                for (j=0;SpellList[j];j++)
                                    if (SpellList[j] == spID)
                                        goto SkipThisAdd;
                                ADD_SPELL(spID,TEFF(spID)->Level,S->Val);
SkipThisAdd:
                                ;
                            }
                        }

                    }
                    else if (SpellList[i] == 100)
                    {
                        for(sp=0;sp!=theGame->LastSpell();sp++)
                        {
                            spID = theGame->SpellID(sp);
                            if (TEFF(spID)->Level > S->Mag)
                                continue;
                            if (TEFF(spID)->HasSource((int8)SpellList[i+1]))
                                if (TEFF(spID)->Schools & SpellList[i+2])
                                {
                                    /* If the spell is mentioned elsewhere on *this* list,
                                    we want to avoid adding it twice. The rationale for
                                    this is lists that alter levels and leave everything
                                    else at its default level. For example, Call Light
                                    might be a priest 1 spell and a wizard 2 spell, so
                                    we want to be able to put in the wiz list:
                                    ALL_SPELLS_FROM_SOURCE(AI_WIZARDRY)
                                    2 $"call light"
                                    and have call light be level two, not be entered twice
                                    as both levels one and two, even thhough it's resource
                                    says level one. */
                                    for (j=0;SpellList[j];j++)
                                        if (SpellList[j] == spID)
                                            goto SkipThisAdd2;
                                    ADD_SPELL(spID,TEFF(spID)->Level,S->Val);
SkipThisAdd2:
                                    ;
                                }
                        }
                        i += 2;                      
                    }
                    else if (SpellList[i] && !skip)
                    {
                        ADD_SPELL(SpellList[i],(uint8)lev,(uint8)S->Val)
                    }
                }
            }          
        }
        StatiIterEnd(this)

            qsort(ls,c,sizeof(LearnableSpell),&CompareLSpells);

        /* double-null terminator */
        ls[c].spID  = 0;
        ls[c].Level = 0;
        ls[c].Type  = 0;
        c++;
        ls[c].spID  = 0;
        ls[c].Level = 0;
        ls[c].Type  = 0;
        c++;

        return ls;
}

void Player::LearnSpell(bool left) {
    bool areSpells, areBookSpells, noBook; 
    int16 i,j,c;
    Item *it;
    LearnableSpell *ls = CalcSpellAccess();
    String menu_name, typ_str;

    if (ls[0].spID == 0) {
        /* The more technically correct message would be, "You have no levels
           in classes that grant you a spell list.", but let's not confuse
           the new player. */
        if (CasterLev()) {
            IPrint("You have casting ability but no class spell "
                "list. This is a bug or an oversight in class design.");
            return;
        }

        IPrint("You have no spellcasting ability.");
        return;
    }

    areSpells = false;
    areBookSpells = false;
    for (i=0;ls[i].spID;i++) {
        /* Elves can't learn Necromancy spells. */
        if (isMType(MA_ELF) && (TEFF(ls[i].spID)->Schools & SC_NEC)
            && !(TEFF(ls[i].spID)->Schools & SC_ABJ))
            continue;

        if (ls[i].Level > 9 || ls[i].Level < 1)
            continue;

        /* Get set up for the spell Type */
        if (ls[i].Type == SP_ARCANE)
            typ_str = " (Arcane)";
        else if (ls[i].Type == SP_DIVINE)
            typ_str = " (Divine)"; 
        else if (ls[i].Type == SP_PRIMAL)
            typ_str = " (Primal)"; 
        else if (ls[i].Type == SP_SORCERY)
            typ_str = " (Sorcerous)"; 
        else if (ls[i].Type == SP_BARDIC)
            typ_str = " (Bardic)";
        else {
            Error("Spell with strange Type!");
            free(ls);
            return;
        }

        /* Do we have the chart level to even consider this
        spell as an option? */
        if ((SAL(ls[i].Level) > AbilityLevel(CA_SPELLCASTING))
            && !HasStati(BONUS_SLOTS,min(9,max(1,ls[i].Level))))
            continue;

        /* Can't learn a spell you've already learned */
        if (Spells[theGame->SpellNum(ls[i].spID)] & (SP_KNOWN|SP_DOMAIN))
            continue;

        /* Can't learn a spell if you have 0 open slots for that
        spell's level and type. */
        if (SpellSlots[ls[i].Level-1] + BonusSlots[ls[i].Level-1] <= 
            SpellsLearned[ls[i].Level-1])
            continue;

        /* Clerics and other divine casters can't learn spells
        that conflict with their own alignment. */
        if (ls[i].Type == SP_DIVINE) {
            TEffect *te = TEFF(ls[i].spID);
            if (te->HasFlag(EF_GOOD) && isMType(MA_EVIL))
                continue;
            if (te->HasFlag(EF_EVIL) && isMType(MA_GOOD))
                continue;
            if (te->HasFlag(EF_LAWFUL) && isMType(MA_CHAOTIC))
                continue;
            if (te->HasFlag(EF_CHAOTIC) && isMType(MA_LAWFUL))
                continue;
        }

        /* This is SLOW, SLOW, SLOW... but I don't think it's an issue
        with modern computers, since it happens only once when the
        player brings up the Learn Spell menu. If high-level mages
        get noticable delays when opening Learn Spell, I'll rewrite
        this test. */
        noBook = false;
        if (ls[i].Type == SP_ARCANE) {
            for (it = FirstInv(); it ;it=NextInv())
                if (it->HasSpell(theGame->SpellNum(ls[i].spID)))
                    goto HasComponent;
            noBook = true;
HasComponent:
            ;
        }

        areSpells = true;         
        if (!Opt(OPT_SHOW_ALL_SPELLS))
            if (noBook)
                continue;

        areBookSpells = true;

        /* Disambiguate shared spells, so that mage/priests get both
        Call Light (Arcane) and Call Light (Divine), avoiding them
        accidently wasting a 2nd-level arcane slot on it, as opposed
        to a 1st-level divine. */
        menu_name = NAME(ls[i].spID);
        for (j=0,c=0;ls[j].spID;j++)
            if (ls[j].spID == ls[i].spID)
                c++;
        if (c > 1)
            menu_name += typ_str;

        if (noBook)
            menu_name = Format("%c%s%c", -BLUE, (const char*)menu_name, -GREY);

        MyTerm->LOption(menu_name,
            noBook ? (-(i+10)) : i,
            TEFF(ls[i].spID)->Describe(thisp),
            Opt(OPT_SORT_SPELLS) ? ls[i].Level : 0);
    }

    for (j=1;j!=10;j++) {
        if (SpellSlots[j-1] + BonusSlots[j-1] > 
            SpellsLearned[j-1])
            goto OpenSlot;
    }

    IPrint("You have no unfilled spell slots.");
    MyTerm->LOptionClear();
    return;

OpenSlot:                        
    if (!areSpells) {
        IPrint("There are no remaining unlearned spells on your class "
            "spell list (or lists) that you have open slots for.");
        MyTerm->LOptionClear();
        return;
    }

    if (!areBookSpells) {
        IPrint("You have no spellbooks with unlearned spells for the "
            "spell slots you have open.");
        MyTerm->LOptionClear();
        return;
    }

    String prompt;
    if (left)
        prompt = Format("Learn which spell (%d left)?", 
            (int)(SpellSlots[0] + BonusSlots[0] - SpellsLearned[0]));
    else
        prompt = "Learn which spell?";

    i = (int16)MyTerm->LMenu(MENU_SORTED|MENU_3COLS|MENU_ESC|MENU_DESC|MENU_BORDER,prompt,WIN_MENUBOX,"help::magic,LE");    
    if (i == -1)
        return;

    if (i < 0) {
        IPrint("You don't have a spellbook for that spell.");
        return;
    }

    Spells[theGame->SpellNum(ls[i].spID)] |= SP_KNOWN | ls[i].Type;
    SpellsLearned[ls[i].Level-1]++;

    if (!left)
        IPrint("Learned.");

    /* If we learn a spell we previously knew how to scribe scrolls of, we get
       the slot back and learn a new scroll spell. */
    RecalcCraftFormulas();
}
