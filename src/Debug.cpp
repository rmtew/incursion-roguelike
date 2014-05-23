/* DEBUG.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Implements the wizard mode functions and options, along with
   all other code used explicitly for debugging purposes.

     void Player::WizardOptions()
     void Player::WizardSwitches()
     void Player::BrowseThings(bool nearby)
     void Object::Dump()
     void Thing::Dump()
     void Creature::Dump()
     void Character::Dump()
     void Player::Dump()
     void Monster::Dump()
     void Item::Dump()
     void Food::Dump()
     void Corpse::Dump()
     void Container::Dump()
     void Weapon::Dump()
     void Map::Dump()
     void Resource::Dump()
     void TMonster::Dump()
     void Game::MemoryStats()
     void Game::DumpCode()
     void TextTerm::TraceWindow(...)

*/

#include "Incursion.h"


#ifdef DEBUG_MEMORY_CORRUPTION

/* Allocate a block of memory with a buffer of scratch
   space at either end in order to prevent the program
   from crashing for minor buffer overwrites. */

#undef malloc
#undef free
#undef strdup
#undef realloc

#define SAFETY_SIZE 48

#ifdef INTENSIVE_MEMORY_DEBUG
int32 BlockCount = 0;
void* Blocks[2000000L];
int32 BlockSize[2000000L];
bool BlockFreed[2000000L];
int32 AmtAlloced;
#endif

void * _malloc(size_t sz) {
    void *vp;
    char *cp;

    vp = malloc(sz + SAFETY_SIZE*2);
    memset(vp,0xAA,sz + SAFETY_SIZE*2);

    cp = (char*)vp;
    cp += SAFETY_SIZE;

#ifdef INTENSIVE_MEMORY_DEBUG
    Blocks[BlockCount] = (void*)cp;
    BlockSize[BlockCount] = sz;
    BlockFreed[BlockCount] = false;
    BlockCount++;
    ASSERT(BlockCount < 1999000);
    AmtAlloced += sz;
#endif

    return (void*) cp;
}

void _free(void *vp) {
    char *cp;
    int32 i,c;

    c = 0;
#ifdef INTENSIVE_MEMORY_DEBUG
    for (i=0;i!=BlockCount;i++)
        if (!BlockFreed[i])
            if (Blocks[i] == vp) {
                c++;
                BlockFreed[i] = true;
                AmtAlloced -= BlockSize[i];
            }
            ASSERT(c == 1);
#endif

    cp = (char*) vp;
    cp -= SAFETY_SIZE;

    free((void*)cp);
}

char* _incursion_strdup(const char* str) {
    int32 len = strlen(str);
    char *nstr = (char*)_malloc(len+1);
    strcpy(nstr,str);
    return nstr;
}

void* _realloc(void *vp, size_t sz) {
    char *cp;
    int32 i, c;

#ifdef INTENSIVE_MEMORY_DEBUG
    c = 0;
    for (i=0;i!=BlockCount;i++)
        if (!BlockFreed[i])
            if (Blocks[i] == vp) {
                c++;
                BlockFreed[i] = true;
                AmtAlloced -= BlockSize[i];
            }
    ASSERT(c == 1);
#endif

    cp = (char*)vp;
    cp -= SAFETY_SIZE;

    cp = (char*)realloc(cp,sz + SAFETY_SIZE*2);
    cp += SAFETY_SIZE;

#ifdef INTENSIVE_MEMORY_DEBUG
    Blocks[BlockCount] = (void*)cp;
    BlockSize[BlockCount] = sz;
    BlockFreed[BlockCount] = false;
    BlockCount++;
    ASSERT(BlockCount < 1999000);
    AmtAlloced += sz;
#endif

    return (void*) cp;
}

void *operator new(size_t size) {
  //...
    void *p = _malloc(size);
    if(!p)
        throw "error calling new";
    return p;
}

void operator delete(void * p) throw() {
    _free(p);
}

void *operator new[](size_t size) {
    void *p = _malloc(size);
    if (!p) 
        throw "error calling new[]";
    return p;
}

void operator delete[](void *p) {
    _free(p);
}

#define malloc  _malloc
#define free    _free
#define realloc _realloc
#define strdup  _incursion_strdup

long array_index(long index, long size) {
    ASSERT(index >= 0)
    ASSERT(size  >  0)
    ASSERT(size > index)
    return max(0,min(index,size));
}

#endif

void TestEncounterGen(Term *MyTerm)
  {
    String str; bool mul; int16 i, mt, dp,des; rID enID;
    MyTerm->SetWin(WIN_INPUT);
    MyTerm->Color(SKYBLUE);
    MyTerm->Write(0,0,"Desired # of Monsters: ");
    MyTerm->Color(MAGENTA);
    MyTerm->ReadLine();
    MyTerm->Clear();
    des = atoi(MyTerm->GetTypedString());
    
    MyTerm->SetWin(WIN_INPUT);
    MyTerm->Color(SKYBLUE);
    MyTerm->Write(0,0,"Enter Challenge Rating/Depth: ");
    MyTerm->Color(MAGENTA);
    MyTerm->ReadLine();
    MyTerm->Clear();
    dp = atoi(MyTerm->GetTypedString());
    MyTerm->SetWin(WIN_INPUT);
    MyTerm->Color(SKYBLUE);
    MyTerm->Write(0,0,"Enter Encounter Name: ");
    MyTerm->Color(MAGENTA);
    MyTerm->ReadLine();
    MyTerm->Clear();
    enID = FIND(MyTerm->GetTypedString());
    str.Empty();
    Map *m = new Map();
    for(i=0;i!=30;i++)
      {
        EventInfo e;
        e.Clear();
        if (!enID || RES(enID)->Type == T_TENCOUNTER)
          e.enID = enID;
        else
          e.enRegID = enID;
        e.enCR = dp;
        e.enDepth = dp;
        e.enDesAmt = des;
        e.EMap = m;
        e.enFlags = EN_NOBUILD|EN_DUNGEON|EN_DUMP;
        ReThrow(EV_ENGEN,e);
        str += e.enDump;
        PurgeStrings();
      }
    delete m;
    MyTerm->Box(WIN_SCREEN,BOX_WIDEBOX,YELLOW,GREY,str);
  }

void FPrint(Term *t, const char *msg)
  {
    const char* _msg = Decolorize(msg);
    int16 ln = strlen(_msg);
    t->FWrite(_msg,ln);
    t->FWrite("\n",1);
  }

int SortByCR(const void *A, const void *B)
  {
    rID aID = *(rID*)A,
        bID = *(rID*)B;
    return TMON(aID)->CR - TMON(bID)->CR;
  }

void Map::GenEncounterStats(Term *t)
  {
    int16 CR, enc, i, j, q, mt; EventInfo e;
    String eStat;
    int32 mID_count[2048],
          mType_count[MA_LAST];
    rID first_mID;
    t->ChangeDirectory(".");
    t->SetWin(WIN_SCREEN);
    t->Clear();
    t->Color(YELLOW);
    t->Write(0,0,"Writing .\\EncStats.txt...\n");
    t->Color(BROWN);
    
    first_mID = theGame->Modules[0]->MonsterID(0);
    memset(mID_count,0,2048*4);
    memset(mType_count,0,MA_LAST*4);
    eStat = "";
    for (CR=1;CR!=16;CR++) {
      for (enc=0;enc!=500;enc++) {       
        e.Clear();
        e.enCR = CR;
        e.enDepth = CR;
        e.EMap = this;
        e.enFlags = EN_NOBUILD|EN_DUNGEON;
        if (enc < 50)
          e.enFlags |= EN_DUMP;
        ReThrow(EV_ENGEN,e);
        if (enc < 50)
          eStat += Decolorize(e.enDump);
        PurgeStrings();
        for (i=0;i!=cEncMem;i++) {
          if (!EncMem[i].mID)
            continue;
          mID_count[EncMem[i].mID - first_mID]++;
          Monster *mn;
          mn = new Monster(EncMem[i].mID);
          if (EncMem[i].tID)
            mn->AddTemplate(EncMem[i].tID);
          if (EncMem[i].tID2)
            mn->AddTemplate(EncMem[i].tID2);
          if (EncMem[i].tID3)
            mn->AddTemplate(EncMem[i].tID3);
          for (mt=0;mt!=MA_LAST;mt++)
            if (mn->isMType(mt))
              mType_count[mt]++;
          delete mn;
          }
        }
      eStat += "\n\n----";
      t->Write(Format("Done CR %d...\n",CR));
      t->Update();
      }    
    
    t->OpenWrite("EncStats.txt");
    t->FWrite((const char*)eStat,strlen(eStat));
    
    for (i=0;i!=theGame->Modules[0]->szMon;i++)
      {
        FPrint(t,Format("%d %s",mID_count[i],
          (const char*)Pluralize(NAME(theGame->Modules[0]->MonsterID(i)))));
      }
    FPrint(t,"\n\n----");
    for (mt=0;mt!=MA_LAST;mt++)
      {
        FPrint(t,Format("%d %s",mType_count[mt],
          (const char*)Pluralize(Lookup(MTypeNames,mt))));
      }
      
    for (i=0;i!=MA_LAST;i++)
      if (LookupOnly(MTypeNames,i))
        {
          rID monList[4096]; int16 n;
          n = 0;
          for (int16 q=0;q!=1;q++)
            for (j=0;j!=theGame->Modules[0]->szMon;j++)
              {
                rID mID = theGame->Modules[0]->MonsterID(j);
                if (TMON(mID)->isMType(mID,i))
                  monList[n++] = mID;
              }
          if (!n)
            continue;
          qsort(monList,n,sizeof(rID),SortByCR);
          FPrint(t,Format("%s (%d):", (const char*)
            Pluralize(Lookup(MTypeNames,i)), n));
          for (j=0;j!=n;j++)
            FPrint(t,Format("  %2d %s", TMON(monList[j])->CR,
              (const char*) NAME(monList[j]))); 
        }
      
    t->Close();
    t->Color(WHITE);
    t->Write("\n\nDone.\n\nPress any key to continue...");
    t->GetCharRaw();
    t->Clear();
  }
          
void Player::WizardOptions() {
    int16 i,j,k,l,ch; 
    Item *it; Monster *mn; int16 n; rID xID; Thing *t;
    EventInfo e; hObj h; String str;

    MyTerm->LOption("Examine Player Data", 0);
    MyTerm->LOption("Examine Nearby Things", 1);
    MyTerm->LOption("Examine All Things", 2);
    MyTerm->LOption("Browse Resources", 3);
    MyTerm->LOption("Item Acquisition", 4);
    MyTerm->LOption("Acquire Unknown Item", 5);
    MyTerm->LOption("Monster Summoning", 6);
    MyTerm->LOption("Paralyze the Player", 7);
    MyTerm->LOption("Dump Event Code", 8);
    MyTerm->LOption("Polymorph the PC", 9);
    MyTerm->LOption("Genocide Everything", 10);
    MyTerm->LOption("Identify Whole Pack", 11);
    MyTerm->LOption("Add Quality to Weapon", 12);
    MyTerm->LOption("Add Quality to Armour", 13);
    MyTerm->LOption("Add Quality to Shield", 14);
    MyTerm->LOption("Ascend / Descend Depth",15);
    MyTerm->LOption("Add Template to Creature",16);
    MyTerm->LOption("Learn Any Spell",17);
    MyTerm->LOption("Lookup Resource Number",18);
    MyTerm->LOption("Lookup Object Number",19);
    MyTerm->LOption("Generate Random Object",20);
    MyTerm->LOption("Generate 30 Random Objects",21);
    MyTerm->LOption("Wizard Mode Switches",22);
    MyTerm->LOption("Generate 30 Encounters",23);
    MyTerm->LOption("Create Trap",24);
    MyTerm->LOption("All Forget All Targets",25);
    MyTerm->LOption("All Monsters Retarget",26);
    MyTerm->LOption("All Creatures CalcValues",27);
    MyTerm->LOption("Make Player Master of Monster",28);
    MyTerm->LOption("All Monster Aggravate",29);
    MyTerm->LOption("Make Monster Prebuff",30);
    MyTerm->LOption("List All Treasure on Level",31);
    MyTerm->LOption("List Chests' Contents", 33);
    MyTerm->LOption("Display Resource Statistics",32);
    MyTerm->LOption("Become Divine Champion", 34);
    MyTerm->LOption("Generate 50 XP Ticks", 35);
    MyTerm->LOption("Change terrain type", 36);

    switch(MyTerm->LMenu(MENU_ESC|MENU_BORDER|MENU_2COLS,"Wizard Mode Options:",WIN_MENUBOX)) {
    case -1:
        return;
    case 0:
        MyTerm->SetWin(WIN_DEBUG);
        MyTerm->SizeWin(WIN_CUSTOM,
            ((TextTerm*)MyTerm)->WinSizeX()-2,
            ((TextTerm*)MyTerm)->WinSizeY()-2);
        MyTerm->Clear();
        MyTerm->ClearScroll(); 
        MyTerm->SGotoXY(0,0);
        Dump();
        MyTerm->UpdateScrollArea(0,WIN_DEBUG);
        while (1) {
            switch(MyTerm->GetCharCmd(KY_CMD_ARROW_MODE)) {
            case KY_CMD_NORTH:
            case KY_CMD_NORTHEAST: 
            case KY_CMD_NORTHWEST: 
                MyTerm->ScrollUp(WIN_DEBUG);
                break;
            case KY_CMD_SOUTH:
            case KY_CMD_SOUTHEAST: 
            case KY_CMD_SOUTHWEST: 
                MyTerm->ScrollDown(WIN_DEBUG);
                break;
            default:
                MyTerm->Clear();
                MyTerm->RefreshMap();
                return;
            }
        }
    case 1:
        BrowseThings(true);
        break;
    case 2:
        BrowseThings(false);
        break;
    case 3:
        break;
    case 4:
        it = (Item*)MyTerm->AcquisitionPrompt(ACQ_ANY_ITEM,0,20,0);
        if (it) {
            if (it->isItem() && it->GetInherantPlus()) {
                MyTerm->SetWin(WIN_INPUT);
                MyTerm->Color(YELLOW);
                MyTerm->Write(0,0,"Enter Item Level: ");
                MyTerm->Color(MAGENTA);
                MyTerm->ReadLine();
                MyTerm->Clear();
                i = atoi(MyTerm->GetTypedString());
                if (i)
                    it->SetInherantPlus(i);
            }
            if (it->isType(T_TRAP))
                it->PlaceAt(m,x,y-1);
            else 
#if 0
            {
                {
                    EventInfo xe;
                    xe.Clear();
                    xe.ETarget = it;
                    xe.EItem   = it;
                    xe.vDepth  = it->ItemLevel();
                    xe.vLevel  = it->ItemLevel();
                    xe.Event   = EV_GENITEM;
                    switch (TITEM(it->iID)->Event(xe,it->iID))
                    {
                    case DONE: break;
                    case ABORT: delete it; return;
                    case ERROR: Error("EV_GENITEM returned ERROR!");
                    case NOTHING: break;
                    }
                    if (it->eID) switch (TITEM(it->eID)->Event(xe,it->eID))
                    {
                    case DONE: break;
                    case ABORT: delete it; return;
                    case ERROR: Error("EV_GENITEM returned ERROR!");
                    case NOTHING: break;
                    }
                }
#endif
                Throw(EV_PICKUP,this,NULL,it,NULL);
        }
        break;
    case 5:
        it = (Item*)MyTerm->AcquisitionPrompt(ACQ_ANY_ITEM,0,20,0);
        if (it) {
            if (it->isType(T_TRAP))
                it->PlaceAt(m,x,y-1);
            else {
                it->SetKnown(0);
                Throw(EV_PICKUP,this,NULL,it,NULL);
            }
        }
        break;
    case 6:
        {
            rID mID = MyTerm->MonsterPrompt("Choose A Monster To Summon");
            Monster * mn;
            if (mID > 0) {
                mn = new Monster(mID); 
                e.Clear();
                if (MyTerm->EffectPrompt(e,Q_LOC,false,"Summon where?") == false)
                { 
                    IPrint("Aborting Monster Summoning.");
                    delete mn; break; 
                }
                TMON(mn->tmID)->GrantGear(mn,mn->tmID,true);
                TMON(mn->tmID)->PEvent(EV_BIRTH,mn,mn->tmID);
                mn->PlaceAt(m,e.EXVal,e.EYVal);
                mn->Initialize(true);
            }
        } 
        break;
    case 7:
        if (!yn("Confirm permanently paralyze player to test game speeds?"))
            break;
        GainPermStati(PARALYSIS,NULL,SS_MISC);
        break;
    case 8:
        theGame->DumpCode();
        break;
    case 9:
        if (ResistLevel(AD_POLY) == -1)
        {
            IPrint("You find you cannot change form!");
            return;
        }
        mn = (Monster*) MyTerm->AcquisitionPrompt(ACQ_POLYMORPH,-10,30);
        if (!mn)
            break;
        Shapeshift(mn->mID,false,NULL);
        GainTempStati(POLYMORPH,NULL,100,SS_MISC,0,0,mn->mID);
        break;
    case 10:
Restart:
        MapIterate(m,t,i)
            if (t->isMonster() && !(t->Flags & F_DELETE)) {
                ThrowDmg(EV_DEATH,AD_NORM,0,"debug genocide",t,t);
                goto Restart;
            }
            break;
    case 11:
        for(it=FirstInv();it;it=NextInv())
            it->MakeKnown(0xFF);
        break;
    case 12:
        it = InSlot(SL_WEAPON);
        if (!it)
        {
            IPrint("Wield a weapon first!");
            break;
        }
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(YELLOW);
        MyTerm->Write(0,0,"Enter Quality Constant: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        i = LookupStr(WQ_CONSTNAMES,MyTerm->GetTypedString());
        if (!i)
            i = LookupStr(IQ_CONSTNAMES,MyTerm->GetTypedString());
        if (!i)
            break;
        if (!it->QualityOK(i)) {
            IPrint("Unacceptable quality.");
            break;
        }
        it->AddQuality(i);
        IPrint("Quality added.");
        break;
    case 13:
        it = InSlot(SL_ARMOUR);
        if (!it) {
            IPrint("Wear some armour first!");
            break;
        }
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(YELLOW);
        MyTerm->Write(0,0,"Enter Quality Constant: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        i = LookupStr(AQ_CONSTNAMES,MyTerm->GetTypedString());
        if (!i)
            i = LookupStr(IQ_CONSTNAMES,MyTerm->GetTypedString());
        if (!i)
            break;
        if (!it->QualityOK(i)) {
            IPrint("Unacceptable quality.");
            break;
        }
        it->AddQuality(i);
        IPrint("Quality added.");
        break;
    case 14:
        it = InSlot(SL_READY);
        if (!it) {
            IPrint("Ready a shield first!");
            break;
        }
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(YELLOW);
        MyTerm->Write(0,0,"Enter Quality Constant: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        i = LookupStr(AQ_CONSTNAMES,MyTerm->GetTypedString());
        if (!i)
            break;
        if (!it->QualityOK(i))
        {
            IPrint("Unacceptable quality.");
            break;
        }
        it->AddQuality(i);
        IPrint("Quality added.");
        break;
    case 15:
        if (!m->dID)
        {
            IPrint("Not in dungeon.");
            break;
        }
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(YELLOW);
        MyTerm->Write(0,0,"Enter New Depth: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        i = atoi(MyTerm->GetTypedString());
        if (i < 1 || RES(m->dID)->Type != T_TDUNGEON ||
            i > TDUN(m->dID)->GetConst(DUN_DEPTH))
        {
            IPrint("Invalid depth.");
            break;
        }
        MoveDepth(i);
        IPrint("Done.");
        break;  
    case 16:
        mn = (Monster*)MyTerm->ChooseTarget();
        if (!mn)
            break;
        if (!mn->isCreature()) {
            IPrint("Not a creature.");
            break;
        }
        {
            Module *m = theGame->Modules[0];
            for (i=0;i<m->szTem;i++) {
                rID tid = m->TemplateID(i);
                TTemplate *t = TTEM(tid);
                if (!mn->isMType(t->ForMType))
                    continue; 
                MyTerm->LOption(NAME(tid),tid,DESC(tid),0);
            }
            xID = MyTerm->LMenu(MENU_ESC|MENU_DESC|MENU_3COLS|MENU_BORDER,
                "Add Which Template?",WIN_MENUBOX);
            if (xID > 0) {
                mn->AddTemplate(xID);
                //mn->IdentifyTemp(xID);
                RES(xID)->GrantGear(mn,xID,true);
                // ww: without initialize, templates that add hitpoints or
                // whatnot won't be noticed
                mn->Initialize(true);
                mn->CalcValues();
                mn->SetImage(); 
                int16 _x, _y; Map *_m;
                _x = mn->x; _y = mn->y; _m = mn->m;
                mn->Remove(false);
                mn->PlaceAt(_m,_x,_y);
                IPrint("Template added."); 
            }
        }
        break;         
    case 17:
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(SKYBLUE);
        MyTerm->Write(0,0,"Enter Spell Name: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        xID = FIND(MyTerm->GetTypedString());
        if (!xID)
        { IPrint("Can't find spell."); break; }
        if (!RES(xID) || RES(xID)->Type != T_TEFFECT)
        { IPrint("Not a spell."); break; }
        Spells[theGame->SpellNum(RES(xID))] |= SP_KNOWN|SP_PRIMAL|SP_INNATE;
        IPrint("Learned."); 
        break;
    case 18:
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(SKYBLUE);
        MyTerm->Write(0,0,"Enter Resource Number: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        xID = atoi(MyTerm->GetTypedString());
        if (!xID)
            break;
        IPrint(Format("%s (%s)",
            NAME(xID),
            Lookup(T_CONSTNAMES,RES(xID)->Type)));
        break;
    case 19:
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(SKYBLUE);
        MyTerm->Write(0,0,"Enter Object Number: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        h = atoi(MyTerm->GetTypedString());
        if (!MainRegistry.Exists(h))
        { IPrint("No such object."); break; }
        IPrint(Format("Object %d: %s",h,(const char*)oThing(h)->Name(0)));
        if (yn("Inspect?")) {
            MyTerm->SetWin(WIN_DEBUG);
            MyTerm->SizeWin(WIN_CUSTOM,
                ((TextTerm*)MyTerm)->WinSizeX(),
                ((TextTerm*)MyTerm)->WinSizeY());
            MyTerm->Clear();
            MyTerm->ClearScroll(); 
            MyTerm->SGotoXY(0,0);
            oThing(h)->Dump();
            MyTerm->UpdateScrollArea(0,WIN_DEBUG);
            while (1) {
                switch(MyTerm->GetCharCmd(KY_CMD_ARROW_MODE)) {
                case KY_CMD_NORTH:
                case KY_CMD_NORTHEAST: 
                case KY_CMD_NORTHWEST: 
                    MyTerm->ScrollUp(WIN_DEBUG);
                    break;
                case KY_CMD_SOUTH:
                case KY_CMD_SOUTHEAST: 
                case KY_CMD_SOUTHWEST: 
                    MyTerm->ScrollDown(WIN_DEBUG);
                    break;
                default:
                    MyTerm->Clear();
                    MyTerm->RefreshMap();
                    return;
                }
            }
        }
        break;        
    case 20:
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(SKYBLUE);
        MyTerm->Write(0,0,"Enter Item Type: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        i = LookupStr(T_CONSTNAMES,MyTerm->GetTypedString());
        MyTerm->Color(SKYBLUE);
        MyTerm->Write(0,0,"Enter Max Level: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        j = atoi(MyTerm->GetTypedString());
        k = 0;
        do {
            k++;
            it = Item::GenItem(0,m->dID,j,10,DungeonItems);
            if (it->Type == i)
                break;
            delete it;
        }
        while (k < 50);

        if (k == 50)
            break;
        GainItem(it,false);
        break;
    case 21:
        ch = MyTerm->ChoicePrompt("[D]ungeon items, [S]taple items or [C]hest items?","dsc");
        if (ch == -1)
            break;
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(SKYBLUE);
        MyTerm->Write(0,0,"Enter Dungeon Level: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        j = atoi(MyTerm->GetTypedString());
        MyTerm->Color(SKYBLUE);
        MyTerm->Write(0,0,"Enter Character Luck: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        l = atoi(MyTerm->GetTypedString());

        str.Empty();
        for(i=0;i!=30;i++) {
            switch (ch) {
            case 'd': it = Item::GenItem(0,m->dID,j,l,DungeonItems); break;
            case 's': it = Item::GenItem(0,m->dID,j,l,StapleItems); break;
            case 'c': it = Item::GenItem(0,m->dID,j,l,ChestItems); break;
            }
            it->SetKnown(0xFF);
            str += Format("\n[%d] %s", it->ItemLevel(),
                (const char*) it->Name(NA_A|NA_LONG|NA_MECH));
            delete it;
        }
        MyTerm->Box(str);
        break;
    case 22:
        MyTerm->OptionManager(OPC_WIZARD);
        break;

    case 25: // forget all targets
        MapIterate(m,t,i) {
            if (t->isCreature() && !t->isPlayer())
                ((Creature *)t)->ts.Clear(); 
        }
        break; 

    case 26: // retarget
        MapIterate(m,t,i) {
            if (t->isCreature() && !t->isPlayer())
                ((Creature *)t)->ts.Retarget((Creature *)t);
        }
        break;

    case 27: 
        MapIterate(m,t,i) {
            if (t->isCreature())
                ((Creature *)t)->CalcValues();
        }
        break;

    case 29: 
        MapIterate(m,t,i) {
            if (t->isMonster())
                ((Monster *)t)->GainPermStati(ENEMY_TO,t,SS_PERM,-1,-1,
                myHandle,9);
        }
        break;

    case 28: 
        mn = (Monster*)MyTerm->ChooseTarget();
        if (!mn)
            break;
        if (!mn->isCreature())
        { IPrint("Not a creature."); break; }
        {
            mn->MakeCompanion(this,PHD_FREEBIE);
        }
        break; 

    case 30: 
        mn = (Monster*)MyTerm->ChooseTarget();
        if (!mn)
            break;
        if (!mn->isMonster())
        { IPrint("Not a monster."); break; }
        {
            mn->PreBuff();
        }
        break; 

        // ww: summon traps for testing
    case 24: 
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(YELLOW);
        MyTerm->Write(0,0,"Enter Trap Name: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        xID = FIND(MyTerm->GetTypedString());
        if (!xID)
        { IPrint("Can't find trap."); break; }
        e.Clear();
        if (MyTerm->EffectPrompt(e,Q_LOC,false,"Summon trap where?")) { 
            Trap * t = new Trap( FIND("trap") , xID);
            t->PlaceAt(m,e.EXVal,e.EYVal);
        } else IPrint("Aborting Trap Summoning.");
        break;
    case 23:       
        /*
        bool mul; int16 mt, dp;
        mul = yn("Generate for large room?");
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(SKYBLUE);
        MyTerm->Write(0,0,"Enter Challenge Rating/Depth: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        dp = atoi(MyTerm->GetTypedString());
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(SKYBLUE);
        MyTerm->Write(0,0,"Enter Encounter MType: ");
        MyTerm->Color(MAGENTA);
        MyTerm->ReadLine();
        MyTerm->Clear();
        mt = LookupStr(MA_CONSTNAMES,MyTerm->GetTypedString());
        str.Empty();
        for(i=0;i!=30;i++)
        {
        m->GnEncounter(EN_NOBUILD|EN_DUNGEON|(mul ? EN_MULTIPLE : 0)|
        (mt ? EN_MTYPE : 0), dp,dp,mt,0,0,0,0,NULL);
        //m->GnEncounter(EN_NOBUILD|EN_SINGLE|EN_MTYPE|EN_STREAMER,
        //  dp,dp,mt, AL_NONGOOD,(rID)0,0,0,NULL);
        str += m->PrintEncounter();
        }
        MyTerm->Box(WIN_SCREEN,BOX_WIDEBOX,YELLOW,GREY,str);
        */
        TestEncounterGen(T1);
        break;
    case 31:
        ListLevelTreasure();
        break;
    case 32:
        MyTerm->Box(WIN_SCREEN,BOX_WIDEBOX,PINK,GREY,
            theGame->CompileStatistics()); 
        break;         
    case 33:
        ListChestContents();
        break;
    case 34:
        MyTerm->SetWin(WIN_INPUT);
        MyTerm->Color(YELLOW);
        MyTerm->Write(0,0,"Enter God: ");
        MyTerm->Color(SKYBLUE);
        MyTerm->ReadLine();
        MyTerm->Clear();
        xID = FIND(MyTerm->GetTypedString());
        if (!xID || RES(xID)->Type != T_TGOD) {
            IPrint("No such god.");
            break;
        }
        GodID = xID;
        setGodFlags(xID,GS_INVOLVED);
        SacVals[theGame->GodNum(xID)][8] = 30000;
        for (i=0;i!=10;i++)
            gainedFavour(xID);
        break;
    case 35:
        for (i=0;i!=50;i++)
            GainXP(1);
        IPrint("Done.");
        break;
    case 36:
        e.Clear();
        if (MyTerm->EffectPrompt(e,Q_LOC,false,"Change where?") == false) { 
            IPrint("Aborting terrain type change.");
            break; 
        }
        xID = MyTerm->ChooseResource("Which terrain type?",T_TTERRAIN,NULL);
        m->WriteTerra(e.EXVal,e.EYVal,xID);
        // mn->PlaceAt(m,e.EXVal,e.EYVal);
    }
    MyTerm->RefreshMap();
}
	
void Player::ListLevelTreasure()
  {
    String items[T_LAST][128];
    int icount[128], i, j;
    Thing *t; String str;
    
    memset(icount,0,sizeof(int) * T_LAST);
    str.Empty();
    for (i=0;m->Things[i];i++)
      {
        #define MAYBE_LIST(it)                                     \
          { it->MakeKnown(0xFF);                                 \
            if (it->GetPlus() || it->eID)                        \
              if (icount[it->Type] < 127)                        \
                items[it->Type][icount[it->Type]++] =            \
                  Format("[%2d] %s (%s)\n", it->ItemLevel(),          \
                  (const char*)it->Name(NA_A|NA_LONG),          \
                  it->GetParent() ? (const char*)it->GetParent()->Name() : "floor" ); }
                  
                  
        t = oThing(m->Things[i]);
        if (t->isItem())
          {
            MAYBE_LIST(((Item*)t))
            if (t->isType(T_CHEST))
              {
                Container *c = (Container*) t;
                for (j=0;(*c)[j];j++)
                  MAYBE_LIST(((*c)[j]))
              }
          }
        else if (t->isMonster())
          {
            Monster *mn = (Monster*)t;
            for (t=mn->FirstInv();t;t=mn->NextInv())
              MAYBE_LIST(((Item*)t))
          }
      }
      
    for (i=0;i!=T_LAST;i++)
      if (icount[i])
        {
          if (icount[i] > 1)
            qsort(items[i],icount[i],sizeof(String),StringCompare);
          str += XPrint("<11><Str>:<7>\n",Lookup(T_CONSTNAMES,i));
          for (j=0;j!=icount[i];j++)
            str += SC("  ") + items[i][j];
        }
    
    
      
    MyTerm->Box(WIN_SCREEN,BOX_WIDEBOX,YELLOW,GREY,str);  
  
  }

void Player::ListChestContents()
  {
    String str; int16 i, j; Thing *t; int32 gp;
    str.Empty(); gp = 0;
    for (i=0;m->Things[i];i++)
      if ((t = oThing(m->Things[i]))->isType(T_CHEST))
        {
          Container *c = (Container*)t;
          str += XPrint("<11><Str>:<7>\n",
            (const char*) t->Name(0));
          for (j=0;(*c)[j];j++) {
            str += Format("__[%d] %s\n"/*"________%s\n"*/, ((*c)[j])->ItemLevel(),
              (const char*) ((*c)[j])->Name(NA_IDENT),
              (const char*) ((*c)[j])->GenStats);
            if (((*c)[j])->isType(T_COIN))
              gp += ((*c)[j])->getShopCost(NULL,NULL);
            }
        }
    if (!str.GetLength())
      IPrint("No chests on level.");
    else {
      str += Format("%cTotal Chest Gold:%c %d (baseline %d)", 
        -14, -7, gp, WealthByLevel[m->Depth]);
      MyTerm->Box(WIN_SCREEN,BOX_WIDEBOX,YELLOW,GREY,str);  
      }
  
  }


void Player::BrowseThings(bool nearby)
{
  int16 i,ch;
  i=0;
  for(i=0;m->Things[i];i++)
    if (m->Things[i] == myHandle)
      break;
  if (!m->Things[i])
    i = 0;

Redisplay:
  //MyTerm->SizeWin(WIN_DEBUG,0,0,80,50);
  MyTerm->SetWin(WIN_DEBUG);
  MyTerm->Clear(); 
  MyTerm->ClearScroll(); 
  MyTerm->SGotoXY(0,0);
  oThing(m->Things[i])->Dump();
  MyTerm->UpdateScrollArea(0,WIN_DEBUG);
  do
  {
    ch=MyTerm->GetCharCmd(KY_CMD_ARROW_MODE);				
    switch(ch)
    {
      case KY_CMD_TAB: 
      case 'm': case 'M':
        if (oThing(m->Things[i])->HasStati(MOUNTED))
          {
            MyTerm->SizeWin(WIN_DEBUG,0,0,80,50);
            MyTerm->SetWin(WIN_DEBUG);
            MyTerm->Clear(); 
            MyTerm->ClearScroll(); 
            MyTerm->SGotoXY(0,0);
            oThing(m->Things[i])->GetStatiObj(MOUNTED)->Dump();
            MyTerm->UpdateScrollArea(0,WIN_DEBUG);
          }
       break;     
      case KY_CMD_EAST: 
        i++;
        if (!m->Things[i])
          i=0;
        while (nearby && oThing(m->Things[i])->DistFromPlayer() > 10)
        {
          i++;
          if (!m->Things[i])
            i = 0;
        }
        goto Redisplay;
      case KY_CMD_WEST: 
        i--;
        if (i==-1)
          i=m->Things.Total()-1;
        while (nearby && oThing(m->Things[i])->DistFromPlayer() > 10)
        {
          i--;
          if (i < 0)
            i = m->Things.Total()-1;
        }							
        goto Redisplay;
      case KY_CMD_NORTH:
      case KY_CMD_NORTHEAST: 
      case KY_CMD_NORTHWEST: 
        MyTerm->ScrollUp(WIN_DEBUG);
        break;
      case KY_CMD_SOUTH:
      case KY_CMD_SOUTHEAST: 
      case KY_CMD_SOUTHWEST: 
        MyTerm->ScrollDown(WIN_DEBUG);
        break;

      default: 
        MyTerm->Clear();
        MyTerm->ShowTraits();
        MyTerm->ShowStatus();
        MyTerm->ShowMap();
        return;
    }
  }
  while(1);
}

void Object::Dump()
	{
    T1->Color(GREY);
  }

void Thing::Dump()
  {
    uint16 i,n;
    Object::Dump();
    n = -1;
    if (m) {
      n = -2;
      for(i=0;i!=m->Things.Total();i++)
        if (m->Things[i] == myHandle)
          n = i;
      }
    if (HasStati(MOUNT))
      T1->SWrite(Format("'%c' -- #%d: %s (class %s, hObj %d)\n",
          Image & 0x7f, n,(const char*)Name(NA_LONG|NA_MECH), 
        Lookup(T_CONSTNAMES,Type),myHandle));
    else
      T1->SWrite(0,0, Format("'%c' -- #%d: %s (class %s, hObj %d)\n",
          Image & 0x7f, n,(const char*)Name(NA_LONG|NA_MECH), 
        Lookup(T_CONSTNAMES,Type),myHandle));
    
    // T1->PutChar(1,0,Image);
		T1->SWrite(Format("X:%3d Y:%3d Timeout:%3d (%d) Flags:%s%s%s%s%s%s\n\n",x,y,Timeout,StoredMovementTimeout,
 		 Flags & F_SOLID ? "F_SOLID " : "",
		 Flags & F_HILIGHT ? "F_HILIGHT " : "",
		 Flags & F_INVIS ? "F_INVIS " : "",
		 Flags & F_DELETE ? "F_DELETE " : "",
     Flags & F_XSOLID ? "F_XSOLID " : "",
     Flags & F_OPAQUE ? "F_OPAQUE " : "",
     Flags & F_OPAQUE ? "F_OPAQUE " : ""));
	}

void Creature::Dump()
  {
    int16 i,j, k; String ss, ts_dump; TAttack *a; Item *it;
    CalcValues();
    static TextVal PrintAttr[] = {
      { A_STR , "A_STR " },
      { A_DEX , "A_DEX " },
      { A_CON , "A_CON " },
      { A_INT , "A_INT " },
      { A_WIS , "A_WIS " },
      { A_CHA , "A_CHA " },
      { A_LUC , "A_LUC " },
      { A_SPD , "A_SPD " },
      { A_MOV , "A_MOV " },
      { A_HIT , "A_HIT " },
      { A_DEF , "A_DEF " },
      { A_ARM , "A_ARM " },
      { A_DMG , "A_DMG " },
      { A_MAN , "A_MAN " },
      { A_SIZ , "A_SIZ " },
      { A_ARC , "A_ARC " },
      { A_DIV , "A_DIV " },
      { A_FAT , "A_FAT " },
      { A_THP , "A_THP " },
      { A_SAV_FORT, "A_FORT" },
      { A_SAV_REF , "A_REF "},
      { A_SAV_WILL, "A_WILL" },
      { A_COV , "A_COV " },
      { A_MR  , "A_MR  " },
      { 0,      NULL } };

    Thing::Dump(); 
    T1->SWrite(Format("%cHP:%c%d/%d%s %cMana:%c%d/%d %cFP:%c%d/%d %cSubdual:%c%d %cPartyID:%c%d %cCR:%c%d\n",
      -14, -7, cHP, mHP, Attr[A_THP] ? (const char*)Format("%+d",Attr[A_THP]) : "",
      -14 ,-7, cMana(), tMana(), 
      -14 ,-7, cFP, Attr[A_FAT], 
      -14, -7, Subdual, -14, -7, PartyID,
      -14, -7, ChallengeRating()));
    T1->SWrite(Format("%cStateFlags:%c",-14,-7));
    for(i=0;i!=16;i++)
      if (StateFlags & XBIT(i))
        T1->SWrite(Format("  %s",Lookup(MS_CONSTNAMES,XBIT(i))));
    if (!StateFlags)
      T1->SWrite(" None.");
    T1->SWrite("\n");

    T1->SWrite(Format("%cM_Flags:%c",-14,-7));
    for(i=0;i!=M_LAST;i++)
      if (HasMFlag(i))
        T1->SWrite(Format("  %s",Lookup(M_CONSTNAMES,(i))));
    T1->SWrite("\n\n");

    for(i=0,j=0;i!=60;i++)
      if (LookupOnly(PrintAttr,i)) {
        /*if (j++ % 2)
          T1->SGotoXY(31,(j/2)+3);
        else
          T1->SGotoXY(0,(j/2)+4);*/
        k = (i == A_SPD) ? A_SPD_ARCHERY + AttackMode() :
            (i == A_HIT) ? A_HIT_ARCHERY + AttackMode() :
            (i == A_DMG) ? A_DMG_ARCHERY + AttackMode() : i;
        T1->SWrite(Format(XPrint("<10>%s %3d <2>%s<7>\n"), Lookup(PrintAttr,i),
          (i == A_SPD || i == A_MOV) ? Attr[k]*5+100 : Attr[k], 
          (const char*)BonusBreakdown(i,45)));
        }
    
    T1->Color(AZURE); 
    T1->SWrite("\nAttacks:\n"); 
    T1->Color(GREY);
    TAttack buf[1024];
    int num = ListAttacks(buf,1024);
    for (i=0;i<num;i++) {
      TAttack * a = & buf[i]; 
      if (a->AType == A_FORM)
        T1->SWrite(Format("  %s for $\"%s\"",Lookup(A_CONSTNAMES,a->AType),
          NAME(a->u.xID)));
      else if (a->AType == A_GATE)
        T1->SWrite(Format("  %s for %d~ %s (CR %d)",Lookup(A_CONSTNAMES,a->AType),
          a->u.a.Dmg.Bonus, Lookup(CA_CONSTNAMES,a->DType),a->u.a.DC));
      else {  
        T1->SWrite(Format("  %s for %s %s",Lookup(A_CONSTNAMES,a->AType),
          (const char*)a->u.a.Dmg.Str(), Lookup(AD_CONSTNAMES,a->DType)));
        if (a->u.a.DC)
          T1->SWrite(Format(" (DC %d)",a->u.a.DC));
        }
      T1->SWrite("\n");
      }

    T1->Color(AZURE); 
    T1->SWrite("Resists:\n"); 
    T1->Color(GREY);

  int8 ResistsShown[] = {
    AD_SLASH, AD_PIERCE, AD_BLUNT, 
    AD_FIRE, AD_COLD, AD_SONI, AD_ACID, AD_ELEC, AD_TOXI, AD_NECR, AD_PSYC,
    AD_MAGC, AD_SLEE, AD_STUN, AD_PLYS, AD_STON, AD_POLY, AD_CHRM, AD_DISN,
    AD_CRIT, AD_DISE, AD_FEAR, AD_RUST, AD_HOLY, AD_EVIL, AD_LAWF, AD_CHAO, 
    AD_SOAK, 0 
  };

    for (i=0;ResistsShown[i];i++) {
      int l1, l2;
      l1 = ResistLevel(ResistsShown[i],false);
      l2 = ResistLevel(ResistsShown[i],true);
      if (l1 != 0 || l2 != 0)
        T1->SWrite(Format("  %c%s%c %d (bypass = %d)\n", -14, 
              (const char*)Lookup(DTypeNames,ResistsShown[i]),-7,l1,l2));
    } 

    T1->Color(AZURE); 
    int total = 0;
    StatiIter(this)
      total++;
    StatiIterEnd(this)
    T1->SWrite(Format("Stati (%d):\n",total));
    T1->Color(GREY);
    StatiIter(this) 
        ss = "  <14>";
        ss += Lookup(STATI_CONSTNAMES,S->Nature);
        ss += "<7> from ";
        ss += Lookup(SS_CONSTNAMES,S->Source);
        if (S->Nature == SKILL_BONUS || S->Nature == EXTRA_SKILL)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(SK_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == CHARGING)
          ss += Format(" (Val:%d Mag:%d CB:%d)",
              S->Val, S->Mag, ChargeBonus());
        else if (S->Nature == EXTRA_FEAT)
          ss += Format(" (Val:%s Mag:%d)",
              FeatName(S->Val), S->Mag);
        else if (S->Nature == EXTRA_ABILITY)
          ss += Format(" (Val:%s Mag:%d)",
              Lookup(CA_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == SAVE_BONUS)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(SN_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == HIDING)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(HI_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == CHARMED)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(CH_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == INVIS)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(INV_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == INVIS_TO)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(INV_CONSTNAMES,S->Val), 
            Lookup(MA_CONSTNAMES,S->Mag));
        else if (S->Nature == PHASED)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(PHASE_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == SINGING)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(BARD_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == WEP_SKILL)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(WS_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature >= ADJUST && S->Nature <= SUSTAIN)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(ATTR_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == IMMUNITY || S->Nature == RESIST)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(AD_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == NEUTRAL_TO ||
                 S->Nature == ENEMY_TO ||
                 S->Nature == ALLY_TO ||
                 S->Nature == ALSO_TURN_MA_TYPE ||
                 S->Nature == DETECTING ||
                 S->Nature == FAV_ENEMY)
          ss += Format(" (%s, %d)",
            Lookup(MTypeNames,S->Val),
            S->Mag);
        else
          ss += Format(" (Val:%d Mag:%d)",
            S->Val, S->Mag);

        if (S->Dis)
          ss += Format(" [Dis %d]",S->Duration);        
        else if (S->Duration)
          ss += Format(" [Dur %d]",S->Duration);
        if (S->CLev)
          ss += Format(" [CL %d]",S->CLev);
        ss += "\n";
        { 
          if (S->eID || S->h) {
            ss += ("           ");
            Status temp = * (S); 
            if (S->h) {
              ASSERT(theRegistry->Exists(S->h));
              ss += Format(" (h:%s)",
                  (const char*)(oThing(S->h)->Name()));
            } 
            if (S->eID) {
              ASSERT(theGame->Get(S->eID));
              ss += Format(" (eID:%s)",
                  NAME(S->eID));
            } 
            ss += "\n"; 
          } 
        }
        T1->SWrite(XPrint(ss));
    StatiIterEnd(this)
    T1->Color(AZURE); 
    T1->SWrite("Inventory:");
    T1->Color(GREY);
    if (!FirstInv())
      T1->SWrite("\n  Nothing.");
    else 
      for(it=FirstInv();it;it=NextInv()) {
        T1->SWrite("\n  * ");
        T1->SWrite(it->Name(NA_IDENT|NA_A|NA_LONG|NA_MECH|NA_STATI));
        if (it->IFlags & IF_WORN)
          T1->SWrite(" (equipped)");
        }
    T1->SWrite("\n");
    ts_dump = ts.Dump();
    T1->SWrite(Format("\n%c---------- Target System:%c\n%s",-13,-7,
          (const char*)XPrint(ts_dump)));
    T1->SWrite(Format("\n%c---------- Hostility:%c\n",-13,-7));
    Thing *t;
    if (m != 0 && x != -1)  {
    MapIterate(m,t,i) {
      if (t != this && t->isCreature() && Percieves(t)) {
        Hostility h;
        h = ts.SpecificHostility(this,(Creature *)t);
        T1->SWrite(Format("%s: %s\n",
              (const char*)t->Name(0),
              (const char*)XPrint(h.Dump((const char*)t->Name(0)))));
      } 
    } 
    }

    T1->SWrite(Format("\n%c---------- tmID:%c %s (%d)\n",-13,-7,NAME(tmID),tmID));
    TMON(tmID)->Dump();
    if (tmID != mID)
      {
        T1->SWrite(Format("\n%c---------- mID:%c %s (%d) \n",-13,-7,NAME(mID),mID));
        TMON(mID)->Dump();
      }
    StatiIterNature(this,TEMPLATE)
          TTemplate * tt = TTEM(S->eID);
          T1->SWrite(Format("\n%c---------- Template:%c %s (%d)\n",-13,-7,NAME(S->eID),S->eID));
          tt->Dump(); 
    StatiIterEnd(this)

    T1->SWrite("\n----------\n");
    T1->Color(AZURE); 
    T1->SWrite(Format("Vision:\n"));
    T1->Color(GREY);
    T1->SWrite(Format("  LightRange:  %d\n",LightRange));
    T1->SWrite(Format("  ShadowRange: %d\n",ShadowRange));
    T1->SWrite(Format("  SightRange:  %d\n",SightRange));
    T1->SWrite(Format("  InfraRange:  %d\n",InfraRange));
    T1->SWrite(Format("  PercepRange: %d\n",PercepRange));
    T1->SWrite(Format("  TelepRange:  %d\n",TelepRange));
    T1->SWrite(Format("  BlindRange:  %d\n",BlindRange));
    T1->SWrite(Format("  TremorRange: %d\n",TremorRange));
    T1->SWrite(Format("  ScentRange:  %d\n",ScentRange));


  }

void Character::Dump()
  {
    int16 i,j; rID gID;
    Creature::Dump();
    T1->SWrite(XPrint("<9>Skill Ranks:<7>\n"));
    for(i=0;i!=SK_LAST;i++)
      if (SkillRanks[i])
        T1->SWrite(Format("  %s: %d (rating %+d)\n",
          Lookup(SK_CONSTNAMES,i),SkillRanks[i],SkillLevel(i)));
    T1->SWrite(XPrint("<10>Spell Flags:<7>\n"));
    for(i=0;i!=MAX_SPELLS;i++)
      if (Spells[i]) {
        T1->SWrite(Format("  Spell #%d (rID %d:%s):",
          i, theGame->SpellID(i),NAME(theGame->SpellID(i))));
        for(j=0;j!=8;j++)
          if (Spells[i] & XBIT(j)) {
            T1->SWrite(" ");           
            T1->SWrite(Lookup(SP_CONSTNAMES,XBIT(j)));
            }
        T1->SWrite("\n");
        }
        
    T1->SWrite(XPrint("<10>Divine Standing:<7>\n"));
    for(i=0;i!=nGods;i++) {
      gID = GodIDList[i];
      T1->SWrite(Format("  %c%13s%c   Lev %d, Fav %5d, Ang %2d, Flags: %s%s%s%s%s%s%s\n",
                   gID == GodID ? -EMERALD : -GREY,
                   NAME(gID),
                   -GREY,
                   getGodLevel(gID),
                   calcFavour(gID),
                   getAnger(gID),
                   getGodFlags(gID) & GS_ANATHEMA ? "Ana" : "",
                   getGodFlags(gID) & GS_INVOLVED ? "Inv" : "",
                   getGodFlags(gID) & GS_FORSAKEN ? "For" : "",
                   getGodFlags(gID) & GS_ABANDONED ? "Aban" : "",
                   getGodFlags(gID) & GS_CROWNED ? "Crown" : "",
                   getGodFlags(gID) & GS_PENANCE ? "Pen" : "",
                   getGodFlags(gID) & GS_KNOWN_ANGER ? "KAng" : ""));
      }
    
    if (HasStati(MOUNTED)) {
      T1->Color(PINK);
      T1->SWrite("\n\n----MOUNT----\n");
      T1->Color(GREY);
      GetStatiObj(MOUNTED)->Dump();
      }
  }

void Player::Dump()
  {
    Character::Dump();
    if (m != NULL && x != -1) 
    T1->SWrite(XPrint("\n<9>Group CR Totals:<7>\n"
                      "  PHD_PARTY   <Num> / <Num>\n"
                      "  PHD_ANIMAL  <Num> / <Num>\n"
                      "  PHD_MAGIC   <Num> / <Num>\n"
                      "  PHD_COMMAND <Num> / <Num>\n",
                      GetGroupCR(PHD_PARTY),   MaxGroupCR(PHD_PARTY),
                      GetGroupCR(PHD_ANIMAL),  MaxGroupCR(PHD_ANIMAL),
                      GetGroupCR(PHD_MAGIC),   MaxGroupCR(PHD_MAGIC),
                      GetGroupCR(PHD_COMMAND), MaxGroupCR(PHD_COMMAND)));


                                            
  }

void Monster::Dump()
  {
    const char* DirNames[] = { "NORTH", "SOUTH", "EAST", "WEST", "NORTHEAST",
                               "NORTHWEST", "SOUTHEAST", "SOUTHWEST", "CENTER",
                               "UP", "DOWN" };
    Creature::Dump();
    T1->SWrite(Format("%cLastMoveDir:%c %s\n%cBuffCount:%c %d\n %cFoilCount:%c %d\n"
                      -11,-7,DirNames[LastMoveDir],-11,-7,BuffCount,-11,-7,FoilCount));

  }

void Item::Dump()
  {
    int16 i; String ss;
    Thing::Dump();
    T1->SWrite(Format("%cHP:%c %d/%d %cPlus:%c %+d %cCharges:%c %d %cFlavor:%c %d %cQuantity:%c %d\n",
      -14, -7, cHP,TITEM(iID) ? MaxHP() : -1, -14, -7, Plus, -14 ,-7, Charges, -14, -7, Flavor, -14, -7, Quantity));
    T1->SWrite(Format("%cIFlags:%c ",-14,-7));
    for(i=0;i!=8;i++)
      if (IFlags & XBIT(i))
        T1->SWrite(Format(" %s",Lookup(IF_CONSTNAMES,XBIT(i))));
    if (!IFlags)
      T1->SWrite(" None.");
    T1->SWrite("\n\n");

    T1->Color(AZURE); 
    int total = 0;
    StatiIter(this)
      total++;
    StatiIterEnd(this)
    T1->SWrite(Format("Stati (%d):\n",total));
    T1->Color(GREY);
    StatiIter(this)
        ss = "  <14>";
        ss += Lookup(STATI_CONSTNAMES,S->Nature);
        ss += "<7> from ";
        ss += Lookup(SS_CONSTNAMES,S->Source);
        if (S->Nature == SKILL_BONUS || S->Nature == EXTRA_SKILL)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(SK_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == SAVE_BONUS)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(SN_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == HIDING)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(HI_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == CHARMED)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(CH_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == INVIS)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(INV_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == INVIS_TO)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(INV_CONSTNAMES,S->Val), 
            Lookup(MA_CONSTNAMES,S->Mag));
        else if (S->Nature == PHASED)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(PHASE_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == SINGING)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(BARD_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == WEP_SKILL)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(WS_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature >= ADJUST && S->Nature <= SUSTAIN)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(ATTR_CONSTNAMES,S->Val), S->Mag);
        else if (S->Nature == IMMUNITY || S->Nature == RESIST)
          ss += Format(" (Val:%s Mag:%d)",
            Lookup(AD_CONSTNAMES,S->Val), S->Mag);
        else
          ss += Format(" (Val:%d Mag:%d)",
            S->Val, S->Mag);
        
        if (S->Dis)
          ss += Format(" [Dis %d]",S->Duration);
        else if (S->Duration)
          ss += Format(" [Dur %d]",S->Duration);
        if (S->CLev)
          ss += Format(" [CL %d]",S->CLev);
        ss += "\n";
        if (S->eID || S->h)
          ss += Format("            (h:%s eID:%s)\n",
            S->h ? (const char*)(oThing(S->h)->Name()) 
          : "0", S->eID ? NAME(S->eID) : "0");
        T1->SWrite(XPrint(ss));
    StatiIterEnd(this)

    T1->SWrite(Format("\n%c---------- iID:%c %s (%d)\n",-13,-7,RES(iID) ? NAME(iID) : "?",iID));
    if (TITEM(iID))
            TITEM(iID)->Dump();
    if (eID && RES(eID))
      {
        T1->SWrite(Format("\n%c---------- eID:%c %s (%d) \n",-13,-7,NAME(eID),eID));
        TEFF(eID)->Dump();
      }

    T1->SWrite("\n----------\n");

  }

void Food::Dump()
  {
    Item::Dump();
    T1->SWrite(Format("\n%cEaten:%c%d %cAge:%c%d\n",
      -14, -7, Eaten, -14 ,-7, Age));
  }

void Corpse::Dump()
  {
    Food::Dump();
    T1->SWrite(Format("\n%cMonster:%c%d (%s)\n",
      -14, -7, mID, NAME(mID)));
  }

void Container::Dump()
  {
    hObj h;
    Item::Dump();
    T1->Color(AZURE);
    T1->SWrite("\nContains:\n");
    T1->Color(GREY);
    if (!Contents)
      T1->SWrite("  Nothing.");
    else {
      h = Contents;
      while (h) {
        T1->SWrite(Format("  * %s\n",(const char *)oThing(h)->Name(NA_MECH|NA_LONG)));
        h = oThing(h)->Next;
        }
      }
  }

void Weapon::Dump()
  {
    int16 i;
    Item::Dump();
    T1->SWrite(Format("%cQualities:%c ",-14,-7));
    for(i=0;i!=8;i++)
      if (Qualities[i])
        T1->SWrite(Format(" %s",Lookup(WQ_CONSTNAMES,Qualities[i])));
    if (!Qualities[0])
      T1->SWrite(" None.");
    T1->SWrite("\n");
  }

void Map::Dump()
  {
    Object::Dump();
  }

void Resource::Dump()
  {
    //Object::Dump();
    T1->SWrite(Format("  Resource: \"%s\"\n",theGame->Modules[ModNum]->GetText(Name)));
  }

void TTemplate::Dump()
  {
    uint16 i;
    Resource::Dump();
    T1->SWrite("  NewImage ");
    T1->Color((NewImage & 0x0F00) >> 8);
    T1->SWrite(Format("%c",NewImage & 0x00FF));
    T1->Color(7);
    T1->SWrite("\n  NewAttk: (constant names only vaguely correct)");
    for(i=0;i<16;i++) {
      TAttack * a = &NewAttk[i]; 
      if (a->AType == 0) continue; 
      T1->SWrite(Format("\n  %d = %s for %s %s",i,Lookup(A_CONSTNAMES,a->AType),
            (const char*)a->u.a.Dmg.Str(), Lookup(AD_CONSTNAMES,a->DType)));
      if (a->u.a.DC)
        T1->SWrite(Format(" (DC %d)",a->u.a.DC));
    } 
    // ww: this is not all the information, but it's what I needed now
    T1->SWrite("\n  Subs Flags:");
    for(i=0;i!=M_LAST;i++)
      if (SubsFlag(i))
        T1->SWrite(Format("  %s",Lookup(M_CONSTNAMES,i)));
    T1->SWrite("\n  Adds Flags:");
    for(i=0;i!=M_LAST;i++)
      if (AddsFlag(i))
        T1->SWrite(Format("  %s",Lookup(M_CONSTNAMES,i)));
    {
      static Annotation *a;
      T1->SWrite(Format("\n  AnHead: %d",AnHead));
      for ( a = Annot(AnHead) ; a ; a = Annot(a->Next)) 
        a->Dump(); 
    }
    T1->SWrite("\n");
  }

void Annotation::Dump()
{
  T1->SWrite(Format("\n  Annot: %s",Lookup(AnnotationNames,AnType)));
  switch (AnType) {
    case AN_SPELLS: 
    case AN_INNATE:
      { for (int j=0;j<5;j++) if (u.sp[j].sID)
        T1->SWrite(Format(" (%s)",NAME(u.sp[j].sID)));
      } 
      break; 
    case AN_EQUIP: 
      if (u.eq.iID <= theRegistry->LastUsedHandle && 
          u.eq.iID >= 128 && 
          theRegistry->Exists(u.eq.iID))
        T1->SWrite(Format(" (%s)",NAME(u.eq.iID))); break; 
    case AN_EVENT: 
      { for (int j=0;j<5;j++) if (u.ev[j].Event)
        T1->SWrite(Format(" (%s)",
              Lookup(EV_CONSTNAMES,u.ev[j].Event)));
      } 
      break; 
  } 
} 

void TMonster::Dump()
  {
    uint16 i;
    Resource::Dump();
    T1->SWrite("  Image '");
    T1->Color((Image & 0x0F00) >> 8);
    T1->SWrite(Format("%c",Image & 0x00FF));
    T1->Color(7);
    T1->SWrite(Format("' Challenge Rating: %d MType %s", CR, Lookup(MA_CONSTNAMES,MType[0])));
    if (MType[1])
      T1->SWrite(Format(" / %s",Lookup(MA_CONSTNAMES,MType[1])));
    if (MType[2])
      T1->SWrite(Format(" / %s",Lookup(MA_CONSTNAMES,MType[2])));


    T1->SWrite(Format("\n  Hit:%d Def:%d Mov:%d Spd:%d HitDice:%d\n",
      Hit,Def,Mov, Spd, HitDice));
    T1->SWrite(Format("  STR:%d DEX:%d CON:%d INT:%d WIS:%d CHA:%d\n",
      Attr[0],Attr[1],Attr[2],Attr[3],Attr[4],Attr[5]));
    T1->SWrite(Format("  Weight: %d lbs Nutrition: %d Size:%s\n",
      Weight, Nutrition, Lookup(SizeNames,Size)));
    /*
    T1->SWrite("Attacks\n");
    for(i=0;i!=6 && Attk[i].Act)
      { T1->SWrite("  "); Attk[i].Dump(); }
    */
    T1->SWrite(Format("  Resists %s%s%s%s%s%s%s%s%s\n", Res == 0 ? "Nothing" : "",
      Res & DF_FIRE ? "Fire " : "",  Res & DF_COLD ? "Cold " : "",
      Res & DF_ACID ? "Acid " : "",  Res & DF_ELEC ? "Elec " : "",
      Res & DF_DISN ? "Disint " : "",  Res & DF_NECR ? "Necro " : "",
      Res & DF_PSYC ? "Psychic " : "",  Res & DF_MAGC ? "Magic " : ""));

    T1->SWrite(Format("  Immune to %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n\n", Imm == 0 ? "Nothing" : "",
      Imm & DF_FIRE ? "Fire " : "",  Imm & DF_COLD ? "Cold " : "",
      Imm & DF_ACID ? "Acid " : "",  Imm & DF_ELEC ? "Elec " : "",
      Imm & DF_POIS ? "Poison " : "",  Imm & DF_NECR ? "Necro " : "",
      Imm & DF_PSYC ? "Psychic " : "",  Imm & DF_MAGC ? "Magic " : "",

      Imm & DF_STON ? "Petri " : "",  Imm & DF_POLY ? "Poly " : "",
      Imm & DF_SLEE ? "Sleep " : "",  Imm & DF_MIND ? "Charm " : "",
      Imm & DF_STUN ? "Stunning " : "", Imm & DF_PLYS ? "Paralysis " : "",
      Imm & DF_CRIT ? "Criticals " : "",  Imm & DF_DISN ? "Disint" : ""));
    T1->SWrite(Format("  TMonster Stati:\n"));
    for(i=0;i<NUM_TMON_STATI && Stati[i].Nature;i++)
        T1->SWrite(Format("    %s (Val:%d Mag:%d)\n",
              Lookup(STATI_CONSTNAMES,Stati[i].Nature),
              Stati[i].Val,
              Stati[i].Mag));
    for(i=0;i!=M_LAST;i++)
      if (HasFlag(i))
        T1->SWrite(Format("  %s ",Lookup(M_CONSTNAMES,i)));
    {
      static Annotation *a;
      T1->SWrite(Format("\n  AnHead: %d",AnHead));
      for ( a = Annot(AnHead) ; a ; a = Annot(a->Next)) 
        a->Dump(); 
    }
  }

void Game::MemoryStats()
  {
    uint32 ResK, MapK, ThingsK; 
    uint16 i; Thing *t;

    ResK = Modules[0]->szMon * sizeof(TMonster) +
             Modules[0]->szItm * sizeof(TItem) +
               Modules[0]->szEff * sizeof(TEffect) +
                 Modules[0]->szFea * sizeof(TFeature);

    MapK = sizeof(Map) + sizeof(LocationInfo) * 
      oMap(m[0])->SizeX() * oMap(m[0])->SizeY();

    ThingsK = 0;
    MapIterate(oMap(m[0]),t,i)
      switch(t->Type) {
        case T_MONSTER: ThingsK += sizeof(Monster); break;
        case T_PLAYER: ThingsK += sizeof(Player); break;
        default: ThingsK += (sizeof(Item)+30); break;
        }

    T1->Write(Format("Resources %ldK, Map %ldK, Things %ldK",ResK/1024,
      MapK/1024, ThingsK/1024));
  }

  
String & EventName(int16 Ev)
  {
    String pre, post, str;
    pre = ""; post = ""; str = "";
    if (Ev >= 20000) 
      {
        pre += "GODWATCH(";
        post += ")";
        Ev -= 20000;
      }
    
    if (Ev >= 10000) 
      {
        pre += "META(";
        post += ")";
        Ev -= 10000;
      }
    if (Ev >= 4000)
      {
        pre += "EITEM(";
        post += ")";
        Ev -= 4000;
      }
    if (Ev >= 2000)
      {
        pre += "EVICTIM(";
        post += ")";
        Ev -= 2000;
      }
    if (Ev >= 1000)
      {
        pre += "POST(";
        post += ")";
        Ev -= 1000;
      }
    if (Ev >= 500)
      {
        pre += "PRE(";
        post += ")";
        Ev -= 500;
      }
    str = Lookup(EV_CONSTNAMES,Ev);
    return *tmpstr(pre + str + post);
  }

void Game::DumpCode()
  {
    Annotation *a; VCode *vc, *start; int32 pv;
    String res, msg; rID xID; int16 i,j, pc, ev;
    // weimer: the old version of this absolutely did not work for me
    // (never displayed anything) ... which is weird, because I'm sure you
    // must have used it a million times ...
    // fjm: I hope this works for both of us now; it's been updated.
    T1->SetWin(WIN_INPUT);
    T1->Color(14);
    T1->Clear(); 

    do {
      xID = 0;
      T1->Clear();
      T1->Write("Resource Name: ");
      T1->ReadLine();
      res = T1->GetTypedString();
      xID = FIND(res);
      }
    while (!xID);

    for(a=RES(xID)->FAnnot();a;a=RES(xID)->NAnnot())  
      if (a->AnType == AN_EVENT) {
        for (i=0;i!=5;i++)
          if (a->u.ev[i].Event != 0) {
            ev = a->u.ev[i].Event; 
            T1->LOption(EventName(ev),ev);
          } 
      } 

    ev = T1->LMenu(MENU_ESC|MENU_2COLS|MENU_BORDER,"Choose an event:",WIN_MENUBOX);

    if (ev == -1) return; 

    ASSERT(RES(xID)->EventMask & BIT((ev%16)+1));

    T1->TraceWindow(xID, ev, &theGame->VM, 0, false);

  }
    
  
String & VMachine::LookupParam(uint16 Type,int32 Value, rID xID, 
							   int16 Ev, int16 op, int16 pn)
  {
    int32 i, best, pri;
    OArray<DebugInfo,1000,1000> &sym = 
              theGame->Modules[0]->Symbols;
   
    if ((op == CMEM || op == GVAR || op == SVAR) && Type == 0)
	    switch (pn)
	      {
          case 1:
            switch (Value)
              {
                case SOBJ_E:         return SC("e");
                case SOBJ_EPLAYER:   return SC("EPlayer");
                case SOBJ_EACTOR:    return SC("EActor");
                case SOBJ_ETARGET:   return SC("ETarget");
                case SOBJ_EVICTIM:   return SC("EVictim");
                case SOBJ_EITEM:     return SC("EItem");
                case SOBJ_EITEM2:    return SC("EItem2");
                case SOBJ_EMAP:      return SC("EMap");
                case SOBJ_GAME:      return SC("theGame");
              }
           break;
          case 2:
            for (i=0;sym[i];i++)
              if ((op == CMEM && (sym[i]->BType == MEM_FUNC || sym[i]->BType == SYS_FUNC))
                    || (op != CMEM && sym[i]->BType == MEM_VAR))
                if (sym[i]->Address == Value)
                  return SC(sym[i]->Ident);
           break;
        }
              
    if ((Type & 3) == RT_REGISTER)
      return SC(Format(Value == 63 ? "SP" : "R%d", Value));
    else if ((Type & 3) == RT_MEMORY) {
      pri = best = 0;
      for (i=0;sym[i];i++)
        if ((sym[i]->xID == xID || sym[i]->BType == GLOB_VAR) &&
            (sym[i]->Event == Ev || sym[i]->BType == RES_VAR || sym[i]->BType == GLOB_VAR))
          {
            if (Value < 0 && sym[i]->BType == LOC_VAR)
              if ((-Value) == sym[i]->Address)
                { pri = 3; best = i; }
            if (Value > 0  && (sym[i]->BType == RES_VAR ||
                              sym[i]->BType == GLOB_VAR))
              if (Value == sym[i]->Address)
                if (((sym[i]->BType == GLOB_VAR) ? 1 : 2) > pri)
                  { pri = (sym[i]->BType == GLOB_VAR) ? 1 : 2;
                    best = i; }
          }
      if (pri)
        return SC(sym[best]->Ident);
      return SC(Format("[%d]", Value));
      }
    else if ((Type & 3) == (RT_REGISTER | RT_MEMORY))
      return SC(Format(Value == 63 ? "[SP]" : "[R%d]", Value));
    else  
      return SC(Format("%d", Value));
   
  } 
  
void TextTerm::TraceWindow(rID xID, int16 Event, VMachine *VM, hCode CP, bool running)
  {
    OArray<DebugInfo,1000,1000> &sym = 
              theGame->Modules[0]->Symbols;
    VCode *start, *vc; 
    int16 sy, y, line, i, j, pc;
    int32 pv; 
    Annotation *a;
    bool first_flag;
    
    if (VM)
      VM->isTracing = true;
    
 
    first_flag = true;
    
    
    Redraw:
    T1->SetWin(WIN_DEBUG);
    sy = T1->WinSizeY();
    T1->Clear(); 
    T1->Color(9); 
    T1->Write(0,0,(const char*)Format("$\"%s\": %s\n\n",
      NAME(xID), (const char*)EventName(Event)));
    T1->Color(7);
    T1->Color(PINK);
    T1->Write(0,sy-2,"[F2] Toggle Breakpoint [F5] Run [F10] Step Into [F11] Step Over");
    T1->Write(0,sy-1,"[PGUP/PGDN] Scroll Code [F6] Execute to Cursor [F7] Abort Script");
    
    
    
    /* Recreate the text dump of the bytecode. */
    if (xID != trace_xID || Event != trace_Event)
      {
        trace_xID = xID;
        trace_Event = Event;
        trace_cursor = 0;
        for(a=RES(xID)->FAnnot();a;a=RES(xID)->NAnnot())  
          if (a->AnType == AN_EVENT) {
            for (i=0;i!=5;i++)
              if (a->u.ev[i].Event == Event) {
                start = theGame->Modules[RES(xID)->ModNum]->QCodeSeg + 
                  a->u.ev[i].MsgOrCode;
                trace_start = a->u.ev[i].MsgOrCode; 
                goto DumpIt;
                }
              /*else if (a->u.ev[i].Event == -Event)
                T1->SWrite(Format("* %s\n",theGame->Modules[RES(xID)->ModNum]->QTextSeg +
                  a->u.ev[i].MsgOrCode));*/
            }
        if (running)
          {
            Fatal("TraceWindow cannot locate code that seems to be currently executing." \
                  " This is very strange, and probably a bug in the debugger itself.");
            return;
          }
        else
          {
            T1->Color(7);
            T1->Write(0,4,"No code to display.");
            T1->Color(15);
            T1->Write(0,10,"\nPress any key to continue...");
            T1->GetCharRaw();
            return;
          }
        DumpIt:
        szDump = 0;
        for(vc=start;vc->Opcode!=HALT;vc++)
          {
            CodeDump[szDump] = Format("%4s ",
              Lookup(OPCODE_CONSTNAMES,vc->Opcode));
            switch (vc->Opcode) {
              case MOV: case ADD: case SUB:
              case MULT: case DIV: case MOD:
              case BSHL: case BSHR: case JTRU:
              case JFAL: case CMEQ: case CMNE:
              case CMGT: case CMLT: case CMGE:
              case CMLE: case NEG: case BAND:
              case BOR: case CMEM: case GVAR:
              case SVAR: case GOBJ: case JTAB:
              case INC: case DEC:  case ROLL:
              case MIN: case MAX:  case LAND:
              case LOR: case WSTR: case ASTR:
              case CSTR: case MSTR:
                pc = 2; break;
              case RET: case CALL: case JUMP:
              case PUSH: case POP: case SBRK:
              case ESTR:
                pc = 1; break;
              case LAST: case FBRK: case JBRK:
                pc = 0; break;
              }

            if (pc) {
              if (vc->P1Type & RT_EXTENDED)
                pv = *((int32*)(vc+1));
              else
                pv = vc->Param1;
              
              CodeDump[szDump] += VM->LookupParam(vc->P1Type,pv,xID,Event,vc->Opcode,1);
              
              /*
              if ((vc->P1Type & 3) == RT_REGISTER)
                CodeDump[szDump] += Format(pv == 63 ? "SP" : "R%d", pv);
              else if ((vc->P1Type & 3) == RT_MEMORY)
                CodeDump[szDump] += Format("[%d]", pv);
              else if ((vc->P1Type & 3) == (RT_REGISTER | RT_MEMORY))
                CodeDump[szDump] += Format(pv == 63 ? "[SP]" : "[R%d]", pv);
              else  
                CodeDump[szDump] += Format("%d", pv);
              */
              }

            if (pc == 2) {
              if (vc->P2Type & RT_EXTENDED) {
                if (vc->P1Type & RT_EXTENDED)
                  pv = *((int32*)(vc+2));
                else
                  pv = *((int32*)(vc+1));
                }
              else
                pv = vc->Param2;
                
              CodeDump[szDump] += ", ";
              CodeDump[szDump] += VM->LookupParam(vc->P2Type,pv,xID,Event,vc->Opcode,2);
              
              /*
              if ((vc->P2Type & 3) == RT_REGISTER)
                CodeDump[szDump] += Format(pv == 63 ? ", SP" : ", R%d", pv);
              else if ((vc->P2Type & 3) == RT_MEMORY)
                CodeDump[szDump] += Format(", [%d]", pv);
              else if ((vc->P2Type & 3) == (RT_REGISTER | RT_MEMORY))
                CodeDump[szDump] += Format(pv == 63 ? ", [SP]" : ", [R%d]", pv);
              else
                CodeDump[szDump] += Format(", %d", pv);
              */
              }

            i = vc->Opcode;
            if (vc->P1Type & RT_EXTENDED && vc->P2Type & RT_EXTENDED)
              { vc += 2; 
                CodeDump[++szDump] = "  [DATA]";
                CodeDump[++szDump] = "  [DATA]"; }
            else if (vc->P1Type & RT_EXTENDED || vc->P2Type & RT_EXTENDED)
              { vc++;    
                CodeDump[++szDump] = "  [DATA]"; }
            if (i == JTAB) {
              for (i=0;i!=pv;i++) {
                CodeDump[++szDump] = Format("  [Jump Table] case %d: JUMP %d\n",
                  *((int32*)(vc+1)), *((int32*)(vc+2)));
                vc += 2;
                }
              CodeDump[++szDump] = Format("  [Jump Table] default: JUMP %d\n", 
                *((int32*)(vc+1)));
              vc++;    
              }
            szDump++;

          }
        CodeDump[szDump++] = "HALT";
        
        
        cLocals = 0;
        for (i=0;sym[i];i++)
          if (sym[i]->xID == xID)
            if (sym[i]->Event == Event)
              Locals[cLocals++] = i;
        
        
      }  
    
    if (running)
      trace_curr = CP - trace_start;
    else
      trace_curr = 0;
    
    if (running && first_flag == true)
      { first_flag = false;
        trace_cursor = trace_curr; }    
    
    /* Now, display the bytecode dump */
    int16 mid_screen = (sy-8)/2;
    int16 first_line = max(0,trace_cursor - mid_screen);
    int16 watch_size = cLocals ? (cLocals+2) : 0;
    for(y=2;y!=(sy-(4+watch_size));y++)
      {
        line = first_line + (y - 2);
        T1->Color(GREY);
        if (line == trace_cursor)
          {
            T1->Color(AZURE);
            T1->PutChar(1,y,'>');
            T1->PutChar(35,y,'<');
            T1->Color(YELLOW);
          }
        for (i=0;i!=VM->nBreakpoints;i++)
          if (VM->Breakpoints[i].xID == xID &&
                 VM->Breakpoints[i].Event == Event)
            if (VM->Breakpoints[i].Location == trace_start + line)
              if (VM->Breakpoints[i].Active)
                T1->Color(RED);
        if (line == trace_curr && running)
          T1->Color(EMERALD);
        T1->Write(3,y,Format("%3d: %s", line, (const char *) CodeDump[line])); 
        T1->Color(GREY);
        if (CodeDump[line+1].GetLength() <= 1)
          break;
      }
      
    T1->Color(11);
    T1->Write(48,2,"Registers:");
    T1->Color(3);
    for (y=3;y!=20;y++)
      {
        T1->Write(48,y,Format("R%2d = %3d",
                        y-3,VM->Regs[y-3]));
      }
    T1->Write(48,22,Format("SP  = %3d",VM->Regs[63]));
    
    if (cLocals)
      {
        int32 Val;
        T1->Color(13);
        T1->Write(0,(sy-(3+watch_size)),"Local Variables:");
        T1->Color(5);
        for (i=0;i!=cLocals;i++)
          {
            y = (sy-(2+watch_size)) + i;
            if (running)
              Val = VM->Stack[VM->Regs[63]-sym[Locals[i]]->Address];
            else
              Val = 0; 
            T1->Write(2,y,Format("%9s = %d", 
                  sym[Locals[i]]->Ident,Val));
            
            switch (sym[Locals[i]]->DataType)
              {
                case DT_HOBJ:
                  T1->Write(Format(" {%s%c}",
                    Val == 0 ? "NULL" : 
                    (Val > 128 && theRegistry->Exists(Val)) ? (const char *) oThing(Val)->Name(0) : "???",-5));
                 break;
                case DT_RID:
                  T1->Write(Format(" {%s}",
                    Val == 0 ? "NULL_ID" : 
                    (Val > 0 && ((Val >> 24) != 0)) ?
                    NAME(Val) : "???"));
                 break;
                case DT_STRING:
                  if (Val > theGame->Modules[0]->szTextSeg ||
                      Val < -64)
                    T1->Write(" [Invalid String]");
                  else
                    T1->Write(Format(" {%s}",
                      (const char*) theGame->VM.getStringSafe(Val)));
              }
          }
      }
   
    
    
    GetKey:    
    switch (T1->GetCharRaw()) 
      {
        case 200 + NORTHEAST:
          if (trace_cursor > 0)
            trace_cursor--;
          goto Redraw;
        case 200 + SOUTHEAST:
          if (trace_cursor < (szDump-1))
            trace_cursor++;
          goto Redraw;
        case FN(2):
          for(i=0;i!=VM->nBreakpoints;i++)
            if (VM->Breakpoints[i].Location ==
                  trace_start + trace_cursor)
              {
                if (i+1 != VM->nBreakpoints)
                  memmove(&VM->Breakpoints[i],
                          &VM->Breakpoints[i+1],
                          sizeof(Breakpoint) * (VM->nBreakpoints-(i+1)));
                VM->nBreakpoints--;
                goto Redraw;
              }
          VM->Breakpoints[VM->nBreakpoints].Location =
            trace_start + trace_cursor;
          VM->Breakpoints[VM->nBreakpoints].Active = true;
          VM->Breakpoints[VM->nBreakpoints].xID = trace_xID;
          VM->Breakpoints[VM->nBreakpoints].Event = trace_Event;
          VM->nBreakpoints++;
          goto Redraw;
         case FN(10): /* Step Through */
         case FN(11): /* Step Into */
           if (running)
             return;
          break;
         case FN(5): /* Run Till Breakpoint */
           if (running) {
             VM->isTracing = false;
             return;
             }
          break;
         case KY_ESC:
           if (!running)
             return;
         default:
           goto GetKey;
      }

  }
        
