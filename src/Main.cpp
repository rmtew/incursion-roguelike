/* MAIN.CPP -- Copyright (c) 1999-2003 Julian Mensch
     This file implements the main game loop, and also contains
   functions to handle gamestates other than normal play, such
   as the gravestone screen at the end and the opening menu.

     void main()
     Game::Game()
     void Game::NewGame(rID mID)
     void Game::Play()
     void Game::StartMenu()
     void Player::Gravestone()

     void BuildSpellList()

*/

#ifdef UNDEF_WIN32
#include <windows.h>
#include <Winbase.h>
#include <tchar.h>
#endif

#include "Incursion.h"
#include <time.h>

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WIN32
    #include <direct.h>
    #define platf_getcwd _getcwd
#else
    #include <unistd.h>
    #define platf_getcwd getcwd
#endif

Map* TheMainMap;
extern bool QuestMode;
Tile *MapLetterArray[127];

void BuildSpellList();

Game * theGame;
//#define BUILD_SPELL_LIST

extern String * StrBufDelQueue[STRING_QUEUE_SIZE];
extern int32    iStrBufDelQueue;

Game::Game() : Object(T_GAME) {
    memset(StrBufDelQueue,0,sizeof(String*)*STRING_QUEUE_SIZE);
    iStrBufDelQueue = 0;

    memset(Modules,0,sizeof(Module*)*MAX_MODULES);
    memset(MDataSeg,0,sizeof(const char*)*MAX_MODULES);
    memset(MDataSegSize,0,sizeof(uint32)*MAX_MODULES);

    memset(DungeonLevels,0,sizeof(hObj*)*MAX_DUNGEONS);
    memset(DungeonSize,0,sizeof(int16)*MAX_DUNGEONS);
    memset(DungeonID,0,sizeof(rID)*MAX_DUNGEONS);

    Timestopper = 0;
    PlayMode = false;
    ItemGenNum = 10;
    DestroyCount = 0;
    srand((unsigned)time(NULL));
}


extern hObj excessItems[30];

void Game::NewGame(rID mID, bool reincarnate) {
    int16 x, y, i;
    Map *mp; Player *pp;
    rID dID;

    T1->SetWin(WIN_SCREEN);
    T1->Clear();
    T1->SetMode(MO_CREATE);

    memset(DungeonLevels, 0, sizeof(hObj*) * MAX_DUNGEONS);
    memset(DungeonID, 0, sizeof(rID) * MAX_DUNGEONS);
    memset(DungeonSize, 0, sizeof(int16) * MAX_DUNGEONS);

    for (i = 0; i != MAX_MODULES; i++)
        if (Modules[i]) {
            MDataSegSize[i] = Modules[i]->szDataSeg + 
                (Modules[i]->szMon * sizeof(MonMem) +
                Modules[i]->szItm * sizeof(ItemMem) +
                Modules[i]->szEff * sizeof(EffMem) +
                Modules[i]->szReg * sizeof(RegMem)) * NumPlayers();
            MDataSeg[i] = (char*)malloc(MDataSegSize[i]);
            memset(MDataSeg[i], 0, MDataSegSize[i]);
        }

    Turn = 198000L;

    pp = new Player(T1);
    p[0] = pp->myHandle;
    pp->LoadOptions(true);

    SetFlavors();
    if (reincarnate) {
        if (!pp->Reincarnate())
            return;
    } else
        pp->Create(false);

    if (mID) {
        mp = new Map;
        mp->Load(mID);
        x = 7;
        y = 7;
        m[0] = mp->myHandle;
        (new Portal(FIND("ancient passage")))->
            PlaceAt(oMap(m[0]), 20, 20);
        mp->RegisterPlayer(p[0]);
    } else {
        dID = FIND("The Goblin Caves");
        // dID = FIND("Monster Evaluation Arena");
        if (!dID)
            Error("Can't load the default dungeon!");
        mp = GetDungeonMap(dID, 1, pp);
        m[0] = mp->myHandle;
        oMap(m[0])->RegisterPlayer(p[0]);

        x = y = 0;
        if (mp->EnterX) {
            x = mp->EnterX;
            y = mp->EnterY;
        } else
            while (oMap(m[0])->At(x, y).Solid || TTER(mp->TerrainAt(x, y))->HasFlag(TF_WATER) ||
                TTER(mp->TerrainAt(x, y))->HasFlag(TF_FALL) || TTER(mp->TerrainAt(x, y))->HasFlag(TF_WARN) ||
                TREG(mp->RegionAt(x, y))->HasFlag(RF_VAULT)) {
                x = random(mp->SizeX());
                y = random(mp->SizeY());
            }
    }
    pp->PlaceAt(oMap(m[0]), x, y);
    pp->CalcValues();

    pp->InitCompanions();

    if (pp->getGod())
        pp->GodMessage(pp->getGod(), MSG_SALUTATION);

    pp->NoteArrival();

    for (i = 0; i != 30; i++)
        if (excessItems[i]) {
            oItem(excessItems[i])->PlaceAt(pp->m, pp->x, pp->y);
            excessItems[i] = 0;
        }
}

void Game::Play() {
    Thing *t, *t2; int16 i,j,c;
    uint32 relative_turn; time_t tm;
    int16 MaxDist;
    Map    *mp = oMap(m[0]);
    Player *pp = oPlayer(p[0]);
    static int16 MonsterDensity[256];
    ASSERT(mp && pp)

    doSave = false;
    doLoad = true;
    doAutoSave = false;

Ressurected:
    PlayMode = true;
    //T1->RecreateViewList();

    MapIterate(mp,t,i)
        if (t->isCreature())
            ((Creature*)t)->CalcValues();

    pp->MyTerm->SetMode(MO_PLAY);
    pp->Timeout = 0;
    MaxDist = 30;
    pp->MyTerm->RefreshMap();

    time(&tm);
    srand((unsigned long)tm);

    do {
        /* If the player changed maps, we follow him around rather than
        * continuing to run through turns for a map that has only monsters
        * on it. We can cache the old map to disk at this point, if needed.
        * The map the player is going to visit should already have been
        * loaded by whatever moved the player.
        */
        PurgeStrings();
        if (pp->m != mp) {
            /* GameRegistry->UnloadGroup(m[0]); */
            m[0] = pp->m->myHandle;
            mp   = pp->m;
        }
        TheMainMap = mp;

        pp->MyTerm->AdjustMap(pp->x,pp->y);

        if (doAutoSave) {
            doAutoSave = false;
            pp->IPrint("Autosave... ");
            pp->MyTerm->Update();
            if (!SaveGame(*pp))
                pp->IPrint("failed.");
            else
                pp->IPrint("done.");
        }

        pp->MyTerm->ShowTime();
        if (pp->HasStati(PARALYSIS))
            pp->MyTerm->Update();

        if ((pp->Flags & F_DELETE) || doSave)
            break;

        MapIterate(mp,t,i) {
            if (Timestopper || doSave)
                break;

            if (doLoad && !t->isPlayer())
                continue;

            doLoad = false;

            Silence = 0;
            if (!t->m || t->x == -1)
                continue;
            ASSERT(theGame->inPerceive == 0);
            ASSERT(theGame->inCalcVal == 0);

            if ((!(t->Flags & F_DELETE)) && dist(t->x,t->y,pp->x,pp->y) < MaxDist) {
                if (t->Timeout < 0)
                    continue;
                if (t->isPlayer()) {
                    memset(MonsterDensity,0,128*sizeof(int16));
                    MapIterate(mp,t2,j)
                        if (t2->Type == T_MONSTER)
                            MonsterDensity[t2->DistFromPlayer()]++;

                    c = 0;
                    for(j=0;j!=60;j++) {
                        c += MonsterDensity[j];
                        if (c > MAX_MONSTER_ACTIONS)
                            break;
                    }

                    MaxDist = j;
                }
NowDoTheMount:
                if (t->Timeout) {
                    t->Timeout--;
                    /*
                    t->IPrint(Format("%s->Timeout = %d.",(const char*)t->Name() ,
                    t->Timeout));
                    */
                } else if (t->isCreature()) {
                    t->RemoveStati(TELEPORTED);
                    if (((Creature*)t)->AttrDeath) {
                        ((Creature*)t)->DoAttrDeath();
                        goto FauxContinue;
                    }

                    if ((t->HasStati(PARALYSIS) && !t->isPlayer()) || t->HasStati(SLEEPING))
                        goto FauxContinue;

                    if (t->HasStati(ACTING)) 
                        ((Creature*)t)->ExtendedAction();
                    else if (t->isMonster() && pp->Opt(OPT_FREEZE_MON))
                        t->Timeout += 30; 
                    else do {
                        ASSERT(t->__Stati.Nested == 0);
                        ((Creature*)t)->ChooseAction();
                        ASSERT(t->__Stati.Nested == 0);
                    } while (t->Type == T_PLAYER && !t->Timeout && !t->HasStati(ACTING));
                }
FauxContinue:
                if (t->HasStati(MOUNTED)) {
                    t = t->GetStatiObj(MOUNTED);
                    goto NowDoTheMount;
                }
            }
        }

        if (Timestopper) {
            relative_turn = 0;

            while(oCreature(Timestopper)->HasStati(TIMESTOP)) {
                Silence = 0;
                oCreature(Timestopper)->ChooseAction();
                relative_turn += oCreature(Timestopper)->Timeout;

                while (relative_turn > 10) {
                    relative_turn -= 10;
                    Throw(EV_TURN, oCreature(Timestopper));
                }
            }
            Timestopper = 0;
        }

        /* We needed to update this to prevent crashes when accounting for
        recursive object destruction -- i.e., destroying a monster also
        destroys everything that it has summoned (in RemoveStati). Hence
        the copying of the DestroyQueue into a new array and such -- the
        original DestroyQueue might be refilled at the same time as the
        cDestroyQueue is emptied. */
        bool skip_delete = Opt(OPT_NO_FREE) != 0;
        while(DestroyCount && !skip_delete) {
            memcpy(cDestroyQueue,DestroyQueue,sizeof(Thing*)*min(20048,DestroyCount+1));
            cDestroyCount = DestroyCount;
            DestroyCount = 0;            
            while (cDestroyCount) {
                cDestroyCount--;
                j = -1;
                if (!cDestroyQueue[cDestroyCount])
                    continue;

                MapIterate(mp,t,i) {
                    if (t == cDestroyQueue[cDestroyCount])
                        j = i;
                    ASSERT(cDestroyQueue[cDestroyCount])
                    t->NotifyGone(cDestroyQueue[cDestroyCount]->myHandle);
                    ASSERT(cDestroyQueue[cDestroyCount])
                }

                mp->NotifyGone(cDestroyQueue[cDestroyCount]->myHandle);
                if (j!=-1)
                    mp->Things.Remove(j);
                T1->NotifyGone(cDestroyQueue[cDestroyCount]->myHandle);
                delete cDestroyQueue[cDestroyCount];
                cDestroyQueue[cDestroyCount] = NULL;

#if 0
                /* Wes, I think you fixed the same bug I did? */

restartDestroyCount: 
                DestroyCount--; j = -1;
                if (!DestroyQueue[DestroyCount])
                    continue;
                MapIterate(mp,t,i) {
                    if (t == DestroyQueue[DestroyCount])
                        j = i;
                    ASSERT(DestroyQueue[DestroyCount]);
                    t->NotifyGone(DestroyQueue[DestroyCount]->myHandle);
                    if (!DestroyQueue[DestroyCount])
                        goto restartDestroyCount; 
#endif
            }
        }

        if (!(Turn%60)) { // refill AoO's; Terrain SPFX 
            mp->UpdateFields();
            mp->UpdateTerra();          
            for(i=0;mp->Things[i];i++) 
                Throw(EV_TURN, oThing(mp->Things[i]));
        }

        Turn++;
    } while(1);

    PlayMode = false;
    Silence = 0;
    pp->UpdateOptions(true);
    pp->Timeout = 0;

    if (doSave) {
        pp->IPrint("Saving game... ");
        pp->MyTerm->Update();
        if (!SaveGame(*pp)) {
            T1->Clear();
            return;
        }
        pp->IPrint("done.");

        pp->MyTerm->SetWin(WIN_INPUT);
        pp->MyTerm->Color(WHITE);
        if (theGame->getQuitFlag()) {
            T1->ShutDown();
            exit(0);
        }

        pp->MyTerm->CursorOn();
        pp->MyTerm->Write(0,0,"Press [ENTER] to continue... ");
        while(pp->MyTerm->GetCharRaw() != KY_ENTER)
            pp->MyTerm->SetWin(WIN_SCREEN);
        pp->MyTerm->Clear();
        return;
    } else {
        if (pp->VictoryFlag) {
            pp->IPrint("<14>You have won...<7>");
        } else
            pp->IPrint("You die...");
        pp->MyTerm->SetWin(WIN_INPUT);
        pp->MyTerm->Color(WHITE);

        pp->MyTerm->Write("Press [ENTER] to continue... ");
        pp->MyTerm->CursorOn();
        while(pp->MyTerm->GetCharRaw() != KY_ENTER)
            ;
        pp->MyTerm->CursorOff();
        if (!(pp->VictoryFlag || pp->QuitFlag)) {
            int16 res = NOTHING;
            if (pp->getGod())
                res = ThrowEff(EV_GOD_RAISE,pp->getGod(),pp);
            for(i=0;res == NOTHING && i != nGods;i++)
                if (GodIDList[i] != pp->getGod())
                    res = ThrowEff(EV_GOD_RAISE,GodIDList[i],pp);
        }
        if (!(pp->Flags & F_DELETE))
            goto Ressurected;

        if (SaveFile.GetLength()) { 
            T1->ChangeDirectory(T1->SaveSubDir());
            T1->Delete(SaveFile);
            T1->ChangeDirectory(T1->IncursionDirectory);
            SaveFile.Empty(); 
        }
        pp->Gravestone();
    }
}


void Init();
void DoResFile();
void Done();
void MakeDict(int);

static int ResourceNameSort(const void *a, const void *b)
{
  rID aa = *(rID *)a,
      bb = *(rID *)b;
  return (strcmp( theGame->GetText(aa,theGame->Get(aa)->Name),
                  theGame->GetText(bb,theGame->Get(bb)->Name)));
}


/* ww: Sun Nov  2 10:22:34 PST 2003
 * I have doubts about some of the challenge ratings for monsters and
 * templates in the context of Incursion. In pnp with creative GMs and
 * players, they probably fit. But in this combat-centric dungeon crawl,
 * they're often strange. I want to look for inconsistencies by comparing
 * them against each other. */ 
void Game::MonsterEvaluation(void)
{
  Thing *t;
  int i, mi; 
  String above_desc, below_desc;


  T1->SetWin(WIN_SCREEN); 

  if (!LoadModules())
    return; 

  int total_resources = 0; 
  rID mIdx; 
  for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
    Module * m = theGame->Modules[mIdx];
    total_resources += m->NumResources(); 
  }
  rID *r ; 
  r = (rID *)calloc(sizeof(*r),total_resources);
  int ri = 0;

  for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
    Module * m = theGame->Modules[mIdx];
    for (i=0; i<m->NumResources(); i++) 
      r[ri++] = m->MonsterID(i);
    }
  ASSERT(ri == total_resources);

  qsort(r,ri,sizeof(r[0]),ResourceNameSort);

  rID mID = T1->MonsterPrompt("Choose A Monster To Evaluate");

  if (mID <= 0) 
    return; 

  bool found_any = false; 
  for (mi=0; mi<total_resources; mi++) {
    Resource *resi = (Resource *)theGame->Get(r[mi]);
    if (resi->Type == T_TTEMPLATE) {
      TTemplate * t = TTEM(r[mi]);
      if (!TMON(mID)->isMType(mID,t->ForMType))
        continue; 
      T1->LOption(NAME(r[mi]),r[mi],DESC(r[mi]));
      found_any = true; 
    } 
  }


  rID tID = found_any ? T1->LMenu(MENU_3COLS|MENU_DESC|MENU_ESC,
      "Choose A Template (optional)", WIN_SCREEN) : 0;
  if (tID == (rID)-1) tID = 0; 

  T1->SetWin(WIN_SCREEN); 
  T1->Clear();
  T1->Color(WHITE); 
  T1->Write("Clearing data structures ...\n");
  T1->Update(); 

  memset(DungeonLevels,0,sizeof(hObj*)*MAX_DUNGEONS);
  memset(DungeonID,0,sizeof(rID)*MAX_DUNGEONS);
  memset(DungeonSize,0,sizeof(int16)*MAX_DUNGEONS);

  for(i=0;i!=MAX_MODULES;i++)
    if (Modules[i])
    {
      MDataSegSize[i] = Modules[i]->szDataSeg + 
        ( Modules[i]->szMon * sizeof(MonMem) +
          Modules[i]->szItm * sizeof(ItemMem) +
          Modules[i]->szEff * sizeof(EffMem) +
          Modules[i]->szReg * sizeof(RegMem) )
        * NumPlayers();
      MDataSeg[i] = (char*)malloc(Modules[i]->szDataSeg + 
          ( Modules[i]->szMon * sizeof(MonMem) +
            Modules[i]->szItm * sizeof(ItemMem) +
            Modules[i]->szEff * sizeof(EffMem) +
            Modules[i]->szReg * sizeof(RegMem) )
          * NumPlayers());
      memset(MDataSeg[i],0,Modules[i]->szDataSeg + 
          ( Modules[i]->szMon * sizeof(MonMem) +
            Modules[i]->szItm * sizeof(ItemMem) +
            Modules[i]->szEff * sizeof(EffMem) +
            Modules[i]->szReg * sizeof(RegMem) )
          * NumPlayers());
    } 

  SetFlavors();
  Silence = 1 ; 

  T1->Write("Generating arena ...\n");
  T1->Update(); 

  rID dID = FIND("Monster Evaluation Arena");
  if (!dID)
    Error("Can't load the arena!");
  Player *pp = new Player(T1); 
  p[0] = pp->myHandle; 
  pp->x = -1;
  pp->y = -1;
  pp->m = NULL;
  Map * mp   = GetDungeonMap(dID,1,pp);
  pp->m = mp;
  // mp->pl[0] = pp->myHandle; 
  m[0] = mp->myHandle; 
  int x,y;
  if (mp->EnterX) { 
    x = mp->EnterX; 
    y = mp->EnterY; 
  } else Error("Can't find arena start!");

  LocationInfo backupGrid[128*128]; // ww: HACK 
        while (mp->Things[0]) {
          oThing(mp->Things[0])->Remove(true);
        } 
        while (mp->Fields[0]) {
          mp->Fields.Remove(0); 
        } 
  memcpy(backupGrid, mp->Grid, 128 * 128 * sizeof(LocationInfo));

  T1->SetWin(WIN_SCREEN); 
  T1->Clear();
  T1->Write(Format("The %s (rID %d) %s (tID %d) fights...\n",NAME(mID),mID,
        tID ? NAME(tID) : "", tID));
  T1->Update(); 

  int row = 3; 

#define MAX_CR_CONSIDERED       25
  int WonAgainstCR[10+MAX_CR_CONSIDERED],
      LostAgainstCR[10+MAX_CR_CONSIDERED],
      CostOfMakingItCR[10+MAX_CR_CONSIDERED];
  memset(WonAgainstCR,0,sizeof(WonAgainstCR));
  memset(LostAgainstCR,0,sizeof(LostAgainstCR));
  memset(CostOfMakingItCR,0,sizeof(CostOfMakingItCR));

  for (int mj=0; mj<total_resources; mj++) {
    Resource *resj = (Resource *)theGame->Get(r[mj]);
    if (resj->Type == T_TMONSTER && 
        r[mj] != mID && 
        !((TMonster *)resj)->HasFlag(M_NOGEN) &&
        !((TMonster *)resj)->isMType(r[mj],MA_DRAGON)) {

      T1->SetWin(WIN_SCREEN); 
      T1->Color(EMERALD + (row & 1)); 
      T1->Write(0,row,resj->GetName(0));
      T1->Update(); 

      int wins = 0, losses = 0, ties = 0;
      for (int battle = 0; battle < 3; battle++) { 

        /*
         * (0) Clear the playing field.
         */

        while (mp->Things[0]) {
          oThing(mp->Things[0])->Remove(true);
        } 
        while (mp->Fields[0]) {
          mp->Fields.Remove(0); 
        } 
//RestartTerra:
        while (mp->TerraList[0]) {
          mp->RemoveTerra(mp->TerraList[0]->key); 
        }
        memcpy(mp->Grid, backupGrid, 128 * 128 * sizeof(LocationInfo));
        // T1->Write(Format("The playing field is clear (%d,%d).\n",x,y));
        PlayMode = true; 
        Turn= 0L;
        mp->BreedCount = 0; 

        /*
         * (1) Create the combatants. 
         */
        Monster *m1 = new Monster(mID); 
        Monster *m2 = new Monster(r[mj]);
        m1->GainPermStati(ENEMY_TO,m1,SS_PERM,-1,-1,0,9);
  //      if (m1->ChallengeRating() >= 6)
          m1->GainPermStati(EXTRA_ABILITY,m1,SS_PERM,CA_KI_STRIKE,2,0,9);
        m2->GainPermStati(ENEMY_TO,m2,SS_PERM,-1,-1,0,9);
  //      if (((TMonster *)resj)->CR >= 6)
          m2->GainPermStati(EXTRA_ABILITY,m1,SS_PERM,CA_KI_STRIKE,2,0,9);
        m1->PlaceAt(mp,x-3,y);
        m2->PlaceAt(mp,x+3,y);
        m1->m = mp;
        m2->m = mp;
        mp->Depth = m1->ChallengeRating(); 
        TMON(mID)->GrantGear(m1,mID,true);
        if (tID) {
          m1->AddTemplate(tID);
          mp->Depth = m1->ChallengeRating(); 
          RES(tID)->GrantGear(m1,tID,true);
        }
        mp->Depth = m2->ChallengeRating(); 
        resj->GrantGear(m2,r[mj],true);
        TMON(mID)->PEvent(EV_BIRTH,m1,m1->mID);
        resj->PEvent(EV_BIRTH,m2,m2->mID);
        m1->Initialize(true);
        m2->Initialize(true);
        m1->CalcValues();
        m2->CalcValues();
        m1->ts.Retarget(m1,true);
        m2->ts.Retarget(m2,true);
        m1->GainPermStati(IGNORES_WEAPON_IMMUNITY,m1,SS_MISC);
        m2->GainPermStati(IGNORES_WEAPON_IMMUNITY,m2,SS_MISC);
        m1->RemoveStati(TELEPORTED);
        m2->RemoveStati(TELEPORTED);
        int32 h1 = m1->HasStati(HIDING) ? 
            m1->SkillLevel(SK_HIDE) + m1->HideVal : 0;
        int32 h2 = m2->HasStati(HIDING) ? 
            m2->SkillLevel(SK_HIDE) + m2->HideVal : 0;
        /* ww: if neither of them can see the other, it would be a boring
         * fight so we make one of them visible */
        if (h1 > m2->SkillLevel(SK_SPOT) &&
            h2 > m1->SkillLevel(SK_SPOT)) {
          if (h1 > h2) 
            m2->RemoveStati(HIDING);
          else 
            m1->RemoveStati(HIDING);
        } 

        /* 
         * (2) Fight, fight, fight! 
         */
        bool done = false; 
        time_t start_time = time(NULL);
        time_t now_time; 
        SetSilence();
        while (!done) { 
          MapIterate(mp,t,i) {
            if (!t->m || t->x == -1)
              continue;
            if(!(t->Flags & F_DELETE)) {
              if (t->Timeout < 0)
                continue;
              if (t->Timeout)
                t->Timeout--;
              else if (t->isCreature()) {
                t->RemoveStati(TELEPORTED);
                if (((Creature*)t)->AttrDeath) {
                  ((Creature*)t)->DoAttrDeath();
                  continue;
                }
                if ((t->HasStati(PARALYSIS) && !t->isPlayer()) || t->HasStati(SLEEPING))
                  continue;
                if (t->HasStati(ACTING)) 
                  ((Creature*)t)->ExtendedAction();
                else do {
                  ((Creature*)t)->ChooseAction();
                  now_time = time(NULL);
                  if (now_time - start_time > 5) { ties++; done = true; goto done_jump; } 
                } while (t->Type == T_PLAYER && !t->Timeout && !t->HasStati(ACTING));
              }
            } 
          } 
          PurgeStrings();
          if(!(Turn%60)) { // refill AoO's; Terrain SPFX 
            MapIterate(mp,t,i) {
              Silence = 0;
              if(t->isCreature()) {
                ((Creature*)t)->AoO =
                  ((Creature*)t)->HasFeat(FT_COMBAT_REFLEXES) ?
                  max(1,1 + ((Creature*)t)->Mod2(A_DEX)) : 1;
                if (((Creature*)t)->HasAttk(A_GAZE))
                  ((Creature*)t)->DoGazeAttack();
                if (((Creature*)t)->HasAttk(A_PROX) || 
                    ((Creature*)t)->HasAttk(A_CPRX))
                  ((Creature*)t)->DoProxAttack();
              }
            }      

            mp->UpdateFields();
            mp->UpdateTerra();

            for(i=0;mp->Things[i];i++)
              Throw(EV_TURN, oThing(mp->Things[i]));
          }

          Turn++;
          now_time = time(NULL);
          if (now_time - start_time > 5) { ties++; done = true; } 

          if (Turn > 60000) { 
            ties++; done = true; 
          } else if (m1->cHP <= 0 || (m1->Flags & F_DELETE)) { 
            losses++; done = true; 
          } else if (m2->cHP <= 0 || (m2->Flags & F_DELETE)) { 
            wins++; done = true; 
          } 
done_jump: ;
        }
      }
      UnsetSilence();
      if (3 * wins > 2 * (wins + losses + ties)) {
        // we won! 
        WonAgainstCR[ 10 + ((TMonster *)resj)->CR ] ++;
      } else if (3 * losses > 2 * (wins + losses + ties)) {
        // we lost!
        LostAgainstCR[ 10 + ((TMonster *)resj)->CR ] ++;
      } 

      T1->Write(30,row,Format(" %d-%d-%d",wins,losses,ties));
      T1->Update(); 
      row++;
      if (row == 45) row = 3; 
      T1->Write(0,row,"                                                                             ");
      T1->Write(0,row+1,"                                                                             ");

      while(DestroyCount) {
        DestroyCount--; 
        int j = -1;
        if (!DestroyQueue[DestroyCount])
          continue;
        MapIterate(mp,t,i) {
          if (t == DestroyQueue[DestroyCount])
            j = i;
          // t->NotifyGone(DestroyQueue[DestroyCount]->myHandle);
        }
        //      mp->NotifyGone(DestroyQueue[DestroyCount]->myHandle);
        if (j!=-1)
          mp->Things.Remove(j);
        //       T1->NotifyGone(DestroyQueue[DestroyCount]->myHandle);
        // delete DestroyQueue[DestroyCount];
        DestroyQueue[DestroyCount] = NULL;
      }

    }
  }
  T1->SetWin(WIN_SCREEN);
  T1->Clear();
  T1->Write(Format("%s %s results ...\n",NAME(mID),
        tID ? NAME(tID) : "", tID));
  T1->Color(GREY);
  int j; 

  for (i=0;i<10+MAX_CR_CONSIDERED;i++) {
    for (j=0; j<i; j++)
      CostOfMakingItCR[i] += LostAgainstCR[j];
    for (j=i+1; j<10+MAX_CR_CONSIDERED; j++)
      CostOfMakingItCR[i] += WonAgainstCR[j];
  } 

  int best_CR = 0;
  int best_CR_cost = 999999;
  for (i=0;i<10+MAX_CR_CONSIDERED;i++) {
    if (CostOfMakingItCR[i] < best_CR_cost) {
      best_CR_cost = CostOfMakingItCR[i];
      best_CR = i; 
    }
  } 

  for (i=0;i<10+MAX_CR_CONSIDERED;i++) {
    if (WonAgainstCR[i] || LostAgainstCR[i])
      T1->Write(Format("Against CR %3d: Wins %3d  Losses %3d\n",
            i-10,
            WonAgainstCR[i],
            LostAgainstCR[i]
            ));
  } 
  T1->Color(YELLOW);
  T1->Write(Format("\nThese results suggest CR %d (with cost %d).\n",
      best_CR - 10,
      best_CR_cost));
  T1->Write(Format("Existing CR for %s is %d.\n", NAME(mID), TMON(mID)->CR));

//done: 
  Cleanup();
  Silence = 0 ; 
  T1->Color(GREY);
  T1->Write("\n\nPress [SPACE] to continue...");
  do
    ;
  while (T1->GetCharRaw() != KY_SPACE);
  T1->Clear();
} 

/* find more information about a specific challenge rating */
void Game::MonsterEvaluationCR(void)
{
  Thing *t;
  int i, mi; 
  String above_desc, below_desc;

  T1->SetWin(WIN_SCREEN); 

  if (!LoadModules())
    return; 

  int num_per_cr[10+MAX_CR_CONSIDERED];
  String desc_per_cr[10+MAX_CR_CONSIDERED];

  for (i=0;i<10+MAX_CR_CONSIDERED;i++) {
    num_per_cr[i] = 0;
    desc_per_cr[i] = "";
  } 

  int total_resources = 0; 
  rID mIdx; 
  for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
    Module * m = theGame->Modules[mIdx];
    total_resources += m->NumResources(); 
  }
  rID *r ; 
  r = (rID *)calloc(sizeof(*r),total_resources);
  int ri = 0;

  for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
    Module * m = theGame->Modules[mIdx];
    for (i=0; i<m->NumResources(); i++) 
      r[ri++] = m->MonsterID(i);
  }
  ASSERT(ri == total_resources);

  qsort(r,ri,sizeof(r[0]),ResourceNameSort);

  // massive hackery
#define WIN_BY_TM(x) (((TMonster *)x)->Weight)
#define LOSS_BY_TM(x) (((TMonster *)x)->Nutrition)


  for (mi=1; mi<total_resources; mi++) {
    Resource *resi = (Resource *)theGame->Get(r[mi]);
    if (resi->Type == T_TMONSTER) {
      TMonster * tm = ((TMonster *)resi);
      if (tm->isMType(r[mi],MA_DRAGON))
        continue; 
      if (tm->HasFlag(M_NOGEN))
        continue;
      int8 CR = tm->CR + 10;
      num_per_cr[CR]++;
      desc_per_cr[CR] = 
          Format("%s %s.",(const char*)desc_per_cr[CR],NAME(r[mi]));
      WIN_BY_TM(tm) = 0;
      LOSS_BY_TM(tm) = 0; 
    }
  }

  for (i=0; i<10+MAX_CR_CONSIDERED;i++) {
    if (num_per_cr[i]) {
      T1->LOption((const char*)Format("CR %2d (%2d monsters)",i-10,
            num_per_cr[i]),
          i,
          (const char*)desc_per_cr[i]);
    }
  } 

  int8 the_CR = (int8)T1->LMenu(MENU_DESC|MENU_ESC,
      "Choose A Monster CR To Disambiguate", WIN_SCREEN);

  if (the_CR == -1) 
    return; 

  the_CR -= 10; 

  for (i=0; i<10+MAX_CR_CONSIDERED;i++) {
    if (num_per_cr[i] && i != (the_CR+10)) {
      T1->LOption((const char*)Format("CR %2d (%2d monsters)",i-10,
            num_per_cr[i]),
          i,
          (const char*)desc_per_cr[i]);
    }
  } 

  int8 the_CR2 = (int8)T1->LMenu(MENU_DESC|MENU_ESC,
      "Choose A Second Monster CR To Disambiguate", WIN_SCREEN);

  the_CR2 -= 10; 

  T1->SetWin(WIN_SCREEN); 
  T1->Clear();
  T1->Color(WHITE); 
  T1->Write("Clearing data structures ...\n");
  T1->Update(); 

  memset(DungeonLevels,0,sizeof(hObj*)*MAX_DUNGEONS);
  memset(DungeonID,0,sizeof(rID)*MAX_DUNGEONS);
  memset(DungeonSize,0,sizeof(int16)*MAX_DUNGEONS);

  for(i=0;i!=MAX_MODULES;i++)
    if (Modules[i])
    {
      MDataSegSize[i] = Modules[i]->szDataSeg + 
        ( Modules[i]->szMon * sizeof(MonMem) +
          Modules[i]->szItm * sizeof(ItemMem) +
          Modules[i]->szEff * sizeof(EffMem) +
          Modules[i]->szReg * sizeof(RegMem) )
        * NumPlayers();
      MDataSeg[i] = (char*)malloc(Modules[i]->szDataSeg + 
          ( Modules[i]->szMon * sizeof(MonMem) +
            Modules[i]->szItm * sizeof(ItemMem) +
            Modules[i]->szEff * sizeof(EffMem) +
            Modules[i]->szReg * sizeof(RegMem) )
          * NumPlayers());
      memset(MDataSeg[i],0,Modules[i]->szDataSeg + 
          ( Modules[i]->szMon * sizeof(MonMem) +
            Modules[i]->szItm * sizeof(ItemMem) +
            Modules[i]->szEff * sizeof(EffMem) +
            Modules[i]->szReg * sizeof(RegMem) )
          * NumPlayers());
    } 

  SetFlavors();
  Silence = 1 ; 

  T1->Write("Generating arena ...\n");
  T1->Update(); 

  rID dID = FIND("Monster Evaluation Arena");
  if (!dID)
    Error("Can't load the arena!");
  Player *pp = new Player(T1); 
  p[0] = pp->myHandle; 
  pp->x = -1;
  pp->y = -1;
  pp->m = NULL;
  Map * mp   = GetDungeonMap(dID,1,pp);
  pp->m = mp;
  // mp->pl[0] = pp->myHandle; 
  m[0] = mp->myHandle; 
  int x,y;
  if (mp->EnterX) { 
    x = mp->EnterX; 
    y = mp->EnterY; 
  } else Error("Can't find arena start!");

  LocationInfo backupGrid[128*128]; // ww: HACK 
        while (mp->Things[0]) {
          oThing(mp->Things[0])->Remove(true);
        } 
        while (mp->Fields[0]) {
          mp->Fields.Remove(0); 
        } 
  memcpy(backupGrid, mp->Grid, 128 * 128 * sizeof(LocationInfo));

  T1->SetWin(WIN_SCREEN); 
  T1->Clear();
  T1->Write(Format("The CR %d and %d creatures fight...\n",the_CR,the_CR2));
  T1->Update(); 

  int row = 3; 

  for (int mk=0; mk<total_resources; mk++) {
    Resource *resk = (Resource *)theGame->Get(r[mk]);
    if (resk->Type == T_TMONSTER && 
        (((TMonster *)resk)->CR == the_CR || 
         ((TMonster *)resk)->CR == the_CR2)&& 
        !((TMonster *)resk)->HasFlag(M_NOGEN) &&
        !((TMonster *)resk)->isMType(r[mk],MA_DRAGON)) 
      ;
    else
      continue; 

    for (int mj=mk+1; mj<total_resources; mj++) {
      Resource *resj = (Resource *)theGame->Get(r[mj]);
      if (resj->Type == T_TMONSTER && 
          (((TMonster *)resj)->CR == the_CR || 
           ((TMonster *)resj)->CR == the_CR2)&& 
          !((TMonster *)resj)->HasFlag(M_NOGEN) &&
          !((TMonster *)resj)->isMType(r[mj],MA_DRAGON)) 
        ;
      else
        continue; 

      T1->SetWin(WIN_SCREEN); 
      T1->Color(EMERALD + (row & 1)); 
      T1->Write(0,row,(const char*)Format("%-30s %-30s", 
            (const char*)resk->GetName(0), (const char*)resj->GetName(0)));
      T1->Update(); 

      int wins = 0, losses = 0, ties = 0;
      for (int battle = 0; battle < 3; battle++) { 

        /*
         * (0) Clear the playing field.
         */

        while (mp->Things[0]) {
          oThing(mp->Things[0])->Remove(true);
        } 
        while (mp->Fields[0]) {
          mp->Fields.Remove(0); 
        } 
//RestartTerra:
        while (mp->TerraList[0]) {
          mp->RemoveTerra(mp->TerraList[0]->key); 
        }
        memcpy(mp->Grid, backupGrid, 128 * 128 * sizeof(LocationInfo));
        // T1->Write(Format("The playing field is clear (%d,%d).\n",x,y));
        PlayMode = true; 
        Turn= 0L;
        mp->BreedCount = 0; 

        /*
         * (1) Create the combatants. 
         */
        Monster *m1 = new Monster(r[mk]); 
        Monster *m2 = new Monster(r[mj]);
        m1->GainPermStati(ENEMY_TO,m1,SS_PERM,-1,-1,0,9);
  //      if (m1->ChallengeRating() >= 6)
          m1->GainPermStati(EXTRA_ABILITY,m1,SS_PERM,CA_KI_STRIKE,2,0,9);
        m2->GainPermStati(ENEMY_TO,m2,SS_PERM,-1,-1,0,9);
  //      if (((TMonster *)resj)->CR >= 6)
          m2->GainPermStati(EXTRA_ABILITY,m1,SS_PERM,CA_KI_STRIKE,2,0,9);
        m1->PlaceAt(mp,x-3,y);
        m2->PlaceAt(mp,x+3,y);
        m1->m = mp;
        m2->m = mp;
        mp->Depth = m1->ChallengeRating(); 
        ((TMonster *)resk)->GrantGear(m1,r[mk],true);
        mp->Depth = m2->ChallengeRating(); 
        ((TMonster *)resj)->GrantGear(m2,r[mj],true);
        resk->PEvent(EV_BIRTH,m1,m1->mID);
        resj->PEvent(EV_BIRTH,m2,m2->mID);
        m1->Initialize(true);
        m2->Initialize(true);
        m1->CalcValues();
        m2->CalcValues();
        m1->ts.Retarget(m1,true);
        m2->ts.Retarget(m2,true);
        m1->GainPermStati(IGNORES_WEAPON_IMMUNITY,m1,SS_MISC);
        m2->GainPermStati(IGNORES_WEAPON_IMMUNITY,m2,SS_MISC);
        m1->RemoveStati(TELEPORTED);
        m2->RemoveStati(TELEPORTED);
        int32 h1 = m1->HasStati(HIDING) ? 
            m1->SkillLevel(SK_HIDE) + m1->HideVal : 0;
        int32 h2 = m2->HasStati(HIDING) ? 
            m2->SkillLevel(SK_HIDE) + m2->HideVal : 0;
        /* ww: if neither of them can see the other, it would be a boring
         * fight so we make one of them visible */
        if (h1 > m2->SkillLevel(SK_SPOT) &&
            h2 > m1->SkillLevel(SK_SPOT)) {
          if (h1 > h2) 
            m2->RemoveStati(HIDING);
          else 
            m1->RemoveStati(HIDING);
        } 

        /* 
         * (2) Fight, fight, fight! 
         */
        bool done = false; 
        time_t start_time = time(NULL);
        time_t now_time; 
        while (!done) { 
          MapIterate(mp,t,i) {
            if (!t->m || t->x == -1)
              continue;
            if(!(t->Flags & F_DELETE)) {
              if (t->Timeout < 0)
                continue;
              if (t->Timeout)
                t->Timeout--;
              else if (t->isCreature()) {
                t->RemoveStati(TELEPORTED);
                if (((Creature*)t)->AttrDeath) {
                  ((Creature*)t)->DoAttrDeath();
                  continue;
                }
                if ((t->HasStati(PARALYSIS) && !t->isPlayer()) || t->HasStati(SLEEPING))
                  continue;
                if (t->HasStati(ACTING)) 
                  ((Creature*)t)->ExtendedAction();
                else do
                  ((Creature*)t)->ChooseAction();
                while (t->Type == T_PLAYER && !t->Timeout && !t->HasStati(ACTING));
              }
            } 
          } 
          PurgeStrings();
          if(!(Turn%60)) { // refill AoO's; Terrain SPFX 
            MapIterate(mp,t,i) {
              Silence = 0;
              if(t->isCreature()) {
                ((Creature*)t)->AoO =
                  ((Creature*)t)->HasFeat(FT_COMBAT_REFLEXES) ?
                  max(1,1 + ((Creature*)t)->Mod2(A_DEX)) : 1;
                if (((Creature*)t)->HasAttk(A_GAZE))
                  ((Creature*)t)->DoGazeAttack();
                if (((Creature*)t)->HasAttk(A_PROX) || 
                    ((Creature*)t)->HasAttk(A_CPRX))
                  ((Creature*)t)->DoProxAttack();
              }
            }      

            mp->UpdateFields();
            mp->UpdateTerra();

            for(i=0;mp->Things[i];i++)
              Throw(EV_TURN, oThing(mp->Things[i]));
          }

          Turn++;
          now_time = time(NULL);
          if (now_time - start_time > 5) { ties++; done = true; } 

          if (Turn > 60000) { 
            ties++; done = true; 
          } else if (m1->cHP <= 0 || (m1->Flags & F_DELETE)) { 
            losses++; done = true; 
          } else if (m2->cHP <= 0 || (m2->Flags & F_DELETE)) { 
            wins++; done = true; 
          } 
        }
      }

      if (3 * wins >= 2 * (wins + losses + ties)) {
        // resk won! 
        WIN_BY_TM(resk) += 1;
        LOSS_BY_TM(resj) += 1;
      } else if (3 * losses >= 2 * (wins + losses + ties)) {
        // resk lost!
        WIN_BY_TM(resj) += 1;
        LOSS_BY_TM(resk) += 1;
      } 

      T1->Write(62,row,Format(" %d-%d-%d",wins,losses,ties));
      T1->Update(); 
      row++;
      if (row == 45) row = 3; 
      T1->Write(0,row,"                                                                             ");
      T1->Write(0,row+1,"                                                                             ");

      while(DestroyCount) {
        DestroyCount--; 
        int j = -1;
        if (!DestroyQueue[DestroyCount])
          continue;
        MapIterate(mp,t,i) {
          if (t == DestroyQueue[DestroyCount])
            j = i;
          // t->NotifyGone(DestroyQueue[DestroyCount]->myHandle);
        }
        //      mp->NotifyGone(DestroyQueue[DestroyCount]->myHandle);
        if (j!=-1)
          mp->Things.Remove(j);
        //       T1->NotifyGone(DestroyQueue[DestroyCount]->myHandle);
        // delete DestroyQueue[DestroyCount];
        DestroyQueue[DestroyCount] = NULL;
      }

    }
  }
  T1->Color(GREY);
  T1->Write("\n\nPress [SPACE] to continue...");
  do
    ;
  while (T1->GetCharRaw() != KY_SPACE);
  T1->Clear();

  int with_x_wins[128];
  String desc_x_wins[128];
  for (i=0;i<128;i++) {
    with_x_wins[i] = 0;
    desc_x_wins[i] = "";
  } 

  int total_fighters = 0;

  for (mi=1; mi<total_resources; mi++) {
    Resource *resi = (Resource *)theGame->Get(r[mi]);
    if (resi->Type == T_TMONSTER) {
      TMonster * tm = ((TMonster *)resi);
      if (tm->isMType(r[mi],MA_DRAGON))
        continue; 
      if (tm->HasFlag(M_NOGEN))
        continue;
      if (tm->CR != the_CR && tm->CR != the_CR2)
        continue; 
      with_x_wins[ (WIN_BY_TM(tm)) ] += 1;
      total_fighters += 1; 
      desc_x_wins[ (WIN_BY_TM(tm)) ] += Format(" %s %d.",(const char*)NAME(r[mi]),
          tm->CR);
    }
  }

  int fighters_sofar = 0; 
  int fighters_halfway = 0; 
  bool did_barrier = false; 
  above_desc = ""; 
  below_desc = "";

  for (i=0;i<128;i++) 
    if (with_x_wins[i]) {
      T1->LOption((const char*)Format("%2d Wins (%2d monsters)",
            i, with_x_wins[i]),
          i,
          (const char*)desc_x_wins[i]);
      fighters_sofar += with_x_wins[i];
      if (!did_barrier) 
        above_desc += desc_x_wins[i]; 
      else
        below_desc += desc_x_wins[i]; 
      if ((fighters_sofar >= (total_fighters / 2)) && !did_barrier) {
        did_barrier = true; 
        fighters_halfway = fighters_sofar; 
        T1->LOption((const char*)Format("--- %d above of %d total ---",fighters_sofar, total_fighters),-fighters_sofar,above_desc);
      } 
    } 
  T1->LOption((const char*)Format("--- %d above of %d total ---",total_fighters - fighters_halfway, total_fighters),-total_fighters,below_desc);


  int8 foo = (int8)T1->LMenu(MENU_DESC|MENU_ESC,
      (const char*)Format("CR %d and %d Results",the_CR,the_CR2), WIN_SCREEN);

//done: 
  Cleanup();
  Silence = 0 ; 
  T1->Color(GREY);
  T1->Write("\n\nPress [SPACE] to continue...");
  do
    ;
  while (T1->GetCharRaw() != KY_SPACE);
  T1->Clear();
} 

/* ww: Sun Dec 14 09:32:20 PST 2003
 * Now I have doubts about the weapons. Some are clearly vastly 
 * superior to others, even given their differing costs and feat
 * requirements. Notably, a bunch of the exotic weapons seem 
 * really dippy (e.g., two-bladed flail) and some of the normal
 * ones aren't so hot (e.g., the axe) either. */ 
/* I should abstract away the "arena engine" here, but I'm too lazy,
 * so right now we get a massive copy-and-past. */
void Game::WeaponEvaluation(void)
{
  Thing *t;
  int i, mi; 

  T1->SetWin(WIN_SCREEN); 

  if (!LoadModules())
    return; 

  int total_resources = 0; 
  rID mIdx; 
  for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
    Module * m = theGame->Modules[mIdx];
    total_resources += m->NumResources(); 
  }
  rID *r ; 
  r = (rID *)calloc(sizeof(*r),total_resources);
  int ri = 0;

  for (mIdx=0;theGame->Modules[mIdx];mIdx++) {
    Module * m = theGame->Modules[mIdx];
    for (i=0; i<m->NumResources(); i++) 
      r[ri++] = m->MonsterID(i);
  }
  ASSERT(ri <= total_resources);

  qsort(r,total_resources,sizeof(r[0]),ResourceNameSort);

  for (mi=1; mi<total_resources; mi++) {
    Resource *resi = (Resource *)theGame->Get(r[mi]);
    if (resi->Type == T_TITEM &&
        ( ((TItem *)resi)->IType == T_WEAPON /* || 
          ((TItem *)resi)->IType == T_BOW */ )
        // && !((TMonster *)resi)->HasFlag(M_NOGEN) 
        // && !((TMonster *)resi)->isMType(MA_DRAGON)
        ) {
      T1->LOption(resi->GetName(0),r[mi],DESC(r[mi]));
    }
  }

  rID mID = T1->LMenu(MENU_3COLS|MENU_DESC|MENU_ESC,
      "Choose A Weapon To Evaluate", WIN_SCREEN);

  if (mID == (rID)-1) 
    return; 

  T1->SetWin(WIN_SCREEN); 
  T1->Clear();
  T1->Color(WHITE); 
  T1->Write("Clearing data structures ...\n");
  T1->Update(); 

  memset(DungeonLevels,0,sizeof(hObj*)*MAX_DUNGEONS);
  memset(DungeonID,0,sizeof(rID)*MAX_DUNGEONS);
  memset(DungeonSize,0,sizeof(int16)*MAX_DUNGEONS);

  for(i=0;i!=MAX_MODULES;i++)
    if (Modules[i])
    {
      MDataSegSize[i] = Modules[i]->szDataSeg + 
        ( Modules[i]->szMon * sizeof(MonMem) +
          Modules[i]->szItm * sizeof(ItemMem) +
          Modules[i]->szEff * sizeof(EffMem) +
          Modules[i]->szReg * sizeof(RegMem) )
        * NumPlayers();
      MDataSeg[i] = (char*)malloc(Modules[i]->szDataSeg + 
          ( Modules[i]->szMon * sizeof(MonMem) +
            Modules[i]->szItm * sizeof(ItemMem) +
            Modules[i]->szEff * sizeof(EffMem) +
            Modules[i]->szReg * sizeof(RegMem) )
          * NumPlayers());
      memset(MDataSeg[i],0,Modules[i]->szDataSeg + 
          ( Modules[i]->szMon * sizeof(MonMem) +
            Modules[i]->szItm * sizeof(ItemMem) +
            Modules[i]->szEff * sizeof(EffMem) +
            Modules[i]->szReg * sizeof(RegMem) )
          * NumPlayers());
    } 

  SetFlavors();
  Silence = 1 ; 

  T1->Write("Generating arena ...\n");
  T1->Update(); 

  rID dID = FIND("Monster Evaluation Arena");
  if (!dID)
    Error("Can't load the arena!");
  Player *pp = new Player(T1); 
  p[0] = pp->myHandle; 
  pp->x = -1;
  pp->y = -1;
  pp->m = NULL;
  Map * mp   = GetDungeonMap(dID,1,pp);
  pp->m = mp;
  // mp->pl[0] = pp->myHandle; 
  m[0] = mp->myHandle; 
  int x,y;
  if (mp->EnterX) { 
    x = mp->EnterX; 
    y = mp->EnterY; 
  } else Error("Can't find arena start!");

  LocationInfo backupGrid[128*128]; // ww: HACK 
        while (mp->Things[0]) {
          oThing(mp->Things[0])->Remove(true);
        } 
        while (mp->Fields[0]) {
          mp->Fields.Remove(0); 
        } 
  memcpy(backupGrid, mp->Grid, 128 * 128 * sizeof(LocationInfo));

  T1->SetWin(WIN_SCREEN); 
  T1->Clear();
  T1->Write(Format("The %s (rID %d) fights...\n",NAME(mID),mID));
  T1->Update(); 

  int row = 3; 

  rID my_monster = FIND("bugbear");
  rID temp1 = FIND("warrior;template");
  rID arm = FIND("scale mail");
  rID shield = FIND("kite shield");

  int total_wins = 0, total_losses =0, total_ties =0 ; 

  for (int mj=0; mj<total_resources; mj++) {
    Resource *resj = (Resource *)theGame->Get(r[mj]);
    if (resj->Type == T_TITEM && 
        r[mj] != mID && 
        ( ((TItem *)resj)->IType == T_WEAPON /* ||
          ((TItem *)resj)->IType == T_BOW */ )
        ) {

      T1->SetWin(WIN_SCREEN); 
      T1->Color(EMERALD + (row & 1)); 
      T1->Write(0,row,resj->GetName(0));
      T1->Update(); 

      int wins = 0, losses = 0, ties = 0;
      for (int battle = 0; battle < 13; battle++) { 

        /*
         * (0) Clear the playing field.
         */

        while (mp->Things[0]) {
          oThing(mp->Things[0])->Remove(true);
        } 
        while (mp->Fields[0]) {
          mp->Fields.Remove(0); 
        } 
//RestartTerra:
        while (mp->TerraList[0]) {
          mp->RemoveTerra(mp->TerraList[0]->key); 
        }
        memcpy(mp->Grid, backupGrid, 128 * 128 * sizeof(LocationInfo));
        // T1->Write(Format("The playing field is clear (%d,%d).\n",x,y));
        PlayMode = true; 
        Turn= 0L;
        mp->BreedCount = 0; 

        /*
         * (1) Create the combatants. 
         */
        Monster *m1 = new Monster(my_monster); 
        Monster *m2 = new Monster(my_monster);

        m1->GainPermStati(ENEMY_TO,m1,SS_PERM,-1,-1,0,9);
        m2->GainPermStati(ENEMY_TO,m2,SS_PERM,-1,-1,0,9);
        m1->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,CA_INFRAVISION,12);
        m2->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,CA_INFRAVISION,12);
        m1->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_MONKEY_GRIP,1);
        m2->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_MONKEY_GRIP,1);
        m1->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_TWO_WEAPON_STYLE,1);
        m2->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_TWO_WEAPON_STYLE,1);
        m1->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_AMBIDEXTERITY,1);
        m2->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_AMBIDEXTERITY,1);
        m1->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_EXPERTISE,1);
        m2->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_EXPERTISE,1);
        m1->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_GUARDED_STANCE,1);
        m2->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_GUARDED_STANCE,1);
        m1->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_WEAPON_SPECIALIST,1);
        m2->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_WEAPON_SPECIALIST,1);
        m1->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_DEFENSIVE_SHOT,1);
        m2->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_DEFENSIVE_SHOT,1);
        m1->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_SPEAR_AND_SHIELD,1);
        m2->GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,FT_SPEAR_AND_SHIELD,1);
        m1->PlaceAt(mp,x-6,y);
        m2->PlaceAt(mp,x+6,y);
        m1->m = mp;
        m2->m = mp;
        mp->Depth = m1->ChallengeRating(); 
          m1->AddTemplate(temp1);
        mp->Depth = m2->ChallengeRating(); 
          m2->AddTemplate(temp1);
        TMON(my_monster)->PEvent(EV_BIRTH,m1,m1->mID);
        TMON(my_monster)->PEvent(EV_BIRTH,m2,m2->mID);
        m1->Initialize(true);
        m2->Initialize(true);
        m1->CalcValues();
        m2->CalcValues();
        m1->ts.Retarget(m1,true);
        m2->ts.Retarget(m2,true);
        m1->RemoveStati(TELEPORTED);
        m2->RemoveStati(TELEPORTED);
        m1->RemoveStati(HIDING);
        m2->RemoveStati(HIDING);
        Item *i1 = new Weapon(mID);
        i1->MakeMagical(0,+3);
        if (TITEM(mID)->Group & WG_THROWN) {
          i1->SetQuantity(10);
        } 
        else if (TITEM(mID)->IType == T_BOW) {
          int ammoID = TITEM(mID)->FirstRes(AN_FIRES);
          Item * ammo = Item::Create(ammoID);
          ammo->SetQuantity(80 + random(20));
          m1->GainItem(ammo,true);
        } 
        Item *i2 = new Weapon(r[mj]);
        i2->MakeMagical(0,+3);
        if (TITEM(r[mj])->Group & WG_THROWN) {
          i2->SetQuantity(10);
        } 
        else if (TITEM(r[mj])->IType == T_BOW) {
          int ammoID = TITEM(r[mj])->FirstRes(AN_FIRES);
          Item * ammo = Item::Create(ammoID);
          ammo->SetQuantity(80 + random(20));
          m2->GainItem(ammo,true);
        } 
        m1->GainPermStati(EXTRA_FEAT,NULL,SS_CLAS,FT_EXOTIC_WEAPON);
        m2->GainPermStati(EXTRA_FEAT,NULL,SS_CLAS,FT_EXOTIC_WEAPON);
        m1->GainItem(i1,true);
        m1->GainItem(new Armour(arm), true);
        m1->GainItem(new Armour(shield), true);
        m2->GainItem(i2,true);
        m2->GainItem(new Armour(arm), true);
        m2->GainItem(new Armour(shield), true);


        /* 
         * (2) Fight, fight, fight! 
         */
        bool done = false; 
        while (!done) { 
          MapIterate(mp,t,i) {
            if (!t->m || t->x == -1)
              continue;
            if(!(t->Flags & F_DELETE)) {
              if (t->Timeout < 0)
                continue;
              if (t->Timeout)
                t->Timeout--;
              else if (t->isCreature()) {
                t->RemoveStati(TELEPORTED);
                if (((Creature*)t)->AttrDeath) {
                  ((Creature*)t)->DoAttrDeath();
                  continue;
                }
                if ((t->HasStati(PARALYSIS) && !t->isPlayer()) || t->HasStati(SLEEPING))
                  continue;
                if (t->HasStati(ACTING)) 
                  ((Creature*)t)->ExtendedAction();
                else do
                  ((Creature*)t)->ChooseAction();
                while (t->Type == T_PLAYER && !t->Timeout && !t->HasStati(ACTING));
              }
            } 
          } 
          PurgeStrings();
          if(!(Turn%60)) { // refill AoO's; Terrain SPFX 
            MapIterate(mp,t,i) {
              Silence = 0;
              if(t->isCreature()) {
                ((Creature*)t)->AoO =
                  ((Creature*)t)->HasFeat(FT_COMBAT_REFLEXES) ?
                  max(1,1 + ((Creature*)t)->Mod2(A_DEX)) : 1;
                if (((Creature*)t)->HasAttk(A_GAZE))
                  ((Creature*)t)->DoGazeAttack();
                if (((Creature*)t)->HasAttk(A_PROX) || 
                    ((Creature*)t)->HasAttk(A_CPRX))
                  ((Creature*)t)->DoProxAttack();
              }
            }      

            mp->UpdateFields();
            mp->UpdateTerra();

            for(i=0;mp->Things[i];i++)
              Throw(EV_TURN, oThing(mp->Things[i]));
          }

          Turn++;
          if (Turn > 60000) { 
            ties++; done = true; 
          } else if (m1->cHP <= 0 || (m1->Flags & F_DELETE)) { 
            losses++; done = true; 
          } else if (m2->cHP <= 0 || (m2->Flags & F_DELETE)) { 
            wins++; done = true; 
          } 
        }
      }

      if (3 * wins > 2 * (wins + losses + ties)) {
        // we won! 
      } else if (3 * losses > 2 * (wins + losses + ties)) {
        // we lost!
      } 
      total_wins += wins;
      total_losses += losses; 

      T1->Write(30,row,Format(" %2d-%2d-%2d",wins,losses,ties));
      T1->Update(); 
      row++;
      if (row == 45) row = 3; 
      T1->Write(0,row,"                                                                             ");
      T1->Write(0,row+1,"                                                                             ");

      while(DestroyCount) {
        DestroyCount--; 
        int j = -1;
        if (!DestroyQueue[DestroyCount])
          continue;
        MapIterate(mp,t,i) {
          if (t == DestroyQueue[DestroyCount])
            j = i;
          // t->NotifyGone(DestroyQueue[DestroyCount]->myHandle);
        }
        //      mp->NotifyGone(DestroyQueue[DestroyCount]->myHandle);
        if (j!=-1)
          mp->Things.Remove(j);
        //       T1->NotifyGone(DestroyQueue[DestroyCount]->myHandle);
        // delete DestroyQueue[DestroyCount];
        DestroyQueue[DestroyCount] = NULL;
      }

    }
  }
  T1->Color(YELLOW);
  T1->Write(40,5,Format("%s won ...",NAME(mID)));
  T1->Write(40,6,Format("%d%% of its battles.",
        (100 * total_wins) / (total_wins + total_losses)));
//done: 
  Cleanup();
  Silence = 0 ; 
  T1->Color(GREY);
  T1->Write(40,7,"Press [SPACE] to continue...");
  do
    ;
  while (T1->GetCharRaw() != KY_SPACE);
  T1->Clear();
} 

/* Look for some common problems ... */ 
void Game::CheckConsistency(void)
{
  int16 q, mc; String Report;
  Report = "";
  T1->SetWin(WIN_SCREEN); T1->Color(EMERALD); T1->Clear();
  T1->Write(0,0,"Incursion Module Consistency Check\n");

  T1->GotoXY(0,2); T1->Color(WHITE);
  T1->Write("Loading Modules ..."); 

  //T1->Scour("Incursion.Exe",true);
  if (!LoadModules()) {
    T1->Write(" failed!\n"); 
    goto done;
  }


  for (q=0,mc=0;q!=MAX_MODULES;q++)
    if (Modules[q])
      mc++;

  T1->Write(Format(" %d modules\n",mc));

  T1->Write("Counting resources ..."); 

  int total_resources; 
  total_resources = 0;
  int i,ri;

  for (q=0;theGame->Modules[q];q++) {
    Module * m = theGame->Modules[q];
    total_resources += m->NumResources(); 
  }


  T1->Write(Format(" %d (%d) resources\n",total_resources));

  rID *r ; 
  r = (rID *)calloc(sizeof(*r),total_resources);
  ri = 0;

  for (q=0;theGame->Modules[q];q++) {
    Module * m = theGame->Modules[q];
    for (i=0; i<m->NumResources(); i++) 
      r[ri++] = i + ((q+1) * 0x01000000);
  }

  T1->Write(Format("Sorting by name ..."));

  qsort(r,total_resources,sizeof(r[0]),ResourceNameSort);

  T1->Write(Format(" done\n"));

  T1->Write(Format("Checking for non-flavour duplicates.\n"));

  Report = "Non-Flavour Duplicate Resources:\n";

  for (i=0; i<total_resources-1; i++) {
    ASSERT(r[i])
    ASSERT(r[i+1])
    if (theGame->Get(r[i])->Name && 
        (theGame->Get(r[i])->Type != T_TFLAVOR || 
         theGame->Get(r[i+1])->Type != T_TFLAVOR) && 
        !strcmp( theGame->GetText(r[i],theGame->Get(r[i])->Name),
                 theGame->GetText(r[i+1],theGame->Get(r[i+1])->Name))) {
      Report += Format("  \"%s\" is used by rID %d and %d\n",
            NAME(r[i]),
            r[i], r[i+1]);
    } 
  } 
  T1->Write(Format("  done.\n"));

  T1->Write(Format("Checking for no-purpose effects."));
  Report += "Purposeless Effects:\n";
  
  for (i=0; i<total_resources; i++) {
    Resource *res = (Resource *)theGame->Get(r[i]);
    if (res->Type == T_TEFFECT) {
      TEffect *eff = (TEffect *)res;
      if (eff->ef.eval == EA_NOTIMP) continue; 
      else if (eff->ef.aval == AR_POISON ||
               eff->ef.aval == AR_DISEASE) continue; 
      if (strstr((const char*)NAME(r[i]),"quality::")) continue; 
      bool okay = true;
      for (int j =0; j < 4 ; j++)
        if (eff->Sources[j] > 0 && eff->Sources[j] < AI_WIZARDRY)
          okay = false;
      if (!okay) continue; 
      if (eff->Purpose == 0) {
        Report += Format(" \"%s\"", (const char*)NAME(r[i]));

      } 
    } 
  } 
  Report += "\n\n";
  T1->Write(Format(".\n"));

  T1->Write(Format("Checking for no-base, yes-purpose effects:\n "));
  Report += Format("Checking for no-base, yes-purpose effects:\n ");
  
  for (i=0; i<total_resources; i++) {
    Resource *res = (Resource *)theGame->Get(r[i]);
    if (res->Type == T_TEFFECT) {
      TEffect *eff = (TEffect *)res;
      if (eff->ef.eval == EA_NOTIMP) continue; 
      else if (eff->ef.aval == AR_POISON ||
               eff->ef.aval == AR_DISEASE) continue; 
      if (strstr((const char*)NAME(r[i]),"quality::")) continue; 
      bool okay = false;
      if (eff->HasSource(AI_WIZARDRY) || eff->HasSource(AI_THEURGY))
        okay = true;
      if (okay && eff->BaseChance == 0) 
        Report += Format(" \"%s\"", (const char*)NAME(r[i]));
    } 
  } 
  Report += "\n\n";
  T1->Write(Format(".\n"));

  T1->Write(Format("Checking for unreachable wizard spells effects:\n"));
  Report += Format("Unreachable wizard spells effects:\n");
  for (i=0; i<total_resources; i++) {
    Resource *res = (Resource *)theGame->Get(r[i]);
    if (res->Type == T_TEFFECT) {
      TEffect *eff = (TEffect *)res;
      if (eff->ef.eval == EA_NOTIMP) continue; 
      if (eff->HasSource(AI_WIZARDRY)) {
        // find a spellbook!
        bool found = false; 
        for (int j=0; !found && j<total_resources; j++) {
          Resource *resj = (Resource *)theGame->Get(r[j]);
          if (resj->Type == T_TITEM) {
            TItem * ti = (TItem *)resj;
            if (ti->IType == T_BOOK && ti->FirstRes(AN_SPELLS)) 
              for(rID sID=ti->FirstRes(AN_SPELLS);
                  sID;
                  sID=ti->NextRes(AN_SPELLS))
                if (sID == r[i]) found = true; 
          } 
        } 
        if (!found) 
          Report += Format("  \"%s\" (%d) is in no spellbook.\n",
            (const char*)NAME(r[i]), r[i]);
      } 
    } 
  } 
  T1->Write(Format("  done.\n"));
  Report += "\n\n";
  

  T1->Write(Format("Checking for < 50%% A_MOV\n"));
  Report += Format("Checking for < 50%% A_MOV\n");
  for (i=0; i<total_resources; i++) {
    Resource *res = (Resource *)theGame->Get(r[i]);
    if (res->Type == T_TMONSTER) {
      TMonster *mon = (TMonster *)res;
      if (100 + mon->Mov * 5 <= 50 && 
          100 + mon->Mov * 5 > 0) {
        Report += Format("  %s",
              (const char*)theGame->GetText(r[i],theGame->Get(r[i])->Name),
              r[i]);
      } 
    }
  }
  T1->Write(Format(".\n"));
  Report += "\n\n";

  T1->Write(Format("Checking for no-sight, no-torch:\n"));
  Report += Format("Checking for no-sight, no-torch:\n");
  for (i=0; i<total_resources; i++) {
    Resource *res = (Resource *)theGame->Get(r[i]);
    if (res->Type == T_TMONSTER) {
      TMonster *mon = (TMonster *)res;
      bool see = false; 
      for (int j=0;j<NUM_TMON_STATI;j++)
        if (mon->Stati[j].Nature == EXTRA_ABILITY && 
            (mon->Stati[j].Val == CA_INFRAVISION ||
             mon->Stati[j].Val == CA_TREMORSENSE ||
             mon->Stati[j].Val == CA_BLINDSIGHT))
            see = true; 
      if (mon->HasFlag(M_NOHANDS) && !see) {
        Report += Format(" %s",
              (const char*)theGame->GetText(r[i],theGame->Get(r[i])->Name),
              r[i]);
      } 
    }
  }
  T1->Write(Format(".\n"));
  Report += "\n\n";

done: 
  T1->Color(GREY);
  
  T1->Box(Report);
  return ; 
}

void Game::GenerateRarityTable()
  {

    int16 i, j, k, num[256];
    rID dID; Map *mp; Player *pp;

    memset(num,0,sizeof(num[0])*256);
  
    T1->SetWin(WIN_SCREEN); 
    T1->Clear();
    T1->Color(WHITE); 
    T1->Write("Clearing data structures ...\n");
    T1->Update(); 

    if (!LoadModules())
      Error("Can't load main module!");

    pp = new Player(T1);
    p[0] = pp->myHandle;

    memset(DungeonLevels,0,sizeof(hObj*)*MAX_DUNGEONS);
    memset(DungeonID,0,sizeof(rID)*MAX_DUNGEONS);
    memset(DungeonSize,0,sizeof(int16)*MAX_DUNGEONS);

    for(i=0;i!=MAX_MODULES;i++)
      if (Modules[i])
        {
          MDataSegSize[i] = Modules[i]->szDataSeg + 
            ( Modules[i]->szMon * sizeof(MonMem) +
              Modules[i]->szItm * sizeof(ItemMem) +
              Modules[i]->szEff * sizeof(EffMem) +
              Modules[i]->szReg * sizeof(RegMem) )
            * NumPlayers();
          MDataSeg[i] = (char*)malloc(Modules[i]->szDataSeg + 
              ( Modules[i]->szMon * sizeof(MonMem) +
                Modules[i]->szItm * sizeof(ItemMem) +
                Modules[i]->szEff * sizeof(EffMem) +
                Modules[i]->szReg * sizeof(RegMem) )
              * NumPlayers());
          memset(MDataSeg[i],0,Modules[i]->szDataSeg + 
              ( Modules[i]->szMon * sizeof(MonMem) +
                Modules[i]->szItm * sizeof(ItemMem) +
                Modules[i]->szEff * sizeof(EffMem) +
                Modules[i]->szReg * sizeof(RegMem) )
              * NumPlayers());
        } 

    SetFlavors();
    Silence = 1 ; 

    T1->Write("Generating dungeon ...\n");
    T1->Update();
      
    dID = FIND("The Goblin Caves");
    if (!dID)
      Error("Can't load the default dungeon!");
    

    for (i=1;i!=11;i++)
      {
        T1->Write(0,i+10,Format("Dungeon Level %d...\n",i));
        mp = GetDungeonMap(dID,i,pp);
        for(j=0;mp->Things[j];j++)
          if (oThing(mp->Things[j])->isMonster())
            {
              Monster *mn = (Monster*)oThing(mp->Things[j]);
              for(k=0;FavEnemies[k];k++)
                if (mn->isMType(FavEnemies[k]))
                  num[FavEnemies[k]]++;
            }
        T1->SetWin(WIN_SCREEN);
        T1->Color(15);
      }

    for (i=0,j=0;FavEnemies[i];i++)
      j += num[FavEnemies[i]];
    j /= i;

    T1->Write(0,25,"Done.");
    T1->GetCharRaw();

    T1->Clear();
    
    T1->Write("int16 MonGroupRarities[] = {\n");
    for (i=0;FavEnemies[i];i++)
      T1->Write(Format("  %15s, %s, /* %d */\n",
        Lookup(MA_CONSTNAMES,FavEnemies[i]),
        num[FavEnemies[i]] >= j*4 ? "MR_VCOMMON  " :
        num[FavEnemies[i]] >= j*2 ? "MR_COMMON   " :
        num[FavEnemies[i]] >= j   ? "MR_UNCOMMON " :
        num[FavEnemies[i]] >= j/2 ? "MR_RARE     " :
        num[FavEnemies[i]] >= j*4 ? "MR_VRARE    " :
                                    "MR_XRARE    ",
        num[FavEnemies[i]]));
    T1->Write("  0, 0 };");

    T1->GetCharRaw();
    Cleanup();
  } 
  
String & ItemNameFromEffect(rID eID)
  {
    String n, base; int16 i;
    if (TEFF(eID)->HasFlag(EF_NAMEONLY))
      return *tmpstr(NAME(eID));
      
    if (TEFF(eID)->ef.aval == AR_POISON)
      return *tmpstr(NAME(eID));
      
    base = "Item";
    for (i=0;DungeonItems[i].Type;i++)
      if (DungeonItems[i].Source && DungeonItems[i].Prototype)
        if (TEFF(eID)->HasSource(DungeonItems[i].Source))
          base = Capitalize(DungeonItems[i].Prototype);
        
    if (TEFF(eID)->GetConst(BASE_ITEM))
      base = Capitalize(NAME(
        TEFF(eID)->GetConst(BASE_ITEM)));
    
    if (TEFF(eID)->HasFlag(EF_PROPER))
      n = SC("The ") + base + SC(" '") +
            SC(NAME(eID)) + SC("'");
    else if (TEFF(eID)->HasFlag(EF_NAMEFIRST))
      n = SC(NAME(eID)) + SC(" ") + base;
    else if (TEFF(eID)->HasFlag(EF_POSTFIX))
      n = Capitalize(base + SC(", ") + SC(NAME(eID)),true);
    else
      n = Capitalize(base) + SC(" of ") + SC(NAME(eID));
      
    return *tmpstr(n);
  }
  
int StringCompare(const void *a, const void *b)
  {
    String *aa = ((String*)a);
    String *bb = ((String*)b);
    String A, B;
    A = aa->Left(3) + aa->Right(aa->GetLength() - 16);
    B = bb->Left(3) + bb->Right(bb->GetLength() - 16);
    
    return stricmp(A,B);
  }

void Game::ListItemsByLevel()
  {
    int total_resources; 
    total_resources = 0;
    int i,ri,c,q,j, t; 
    String *list;
    
    int16 IType[] = { AI_RING, AI_ROD, AI_STAFF, AI_WAND,
                      AI_WEAPON, AI_CLOAK, AI_BOOTS, AI_GAUNTLETS,
                      AI_STONE, AI_BRACERS, AI_EYES, AI_AMULET,
                      AI_GIRDLE, AI_HELM, AI_HEADBAND, AI_HORN,
                      AI_WONDER, 0 };
    
    theGame->LoadModules();

    for (q=0;theGame->Modules[q];q++) {
      Module * m = theGame->Modules[q];
      total_resources += m->NumResources(); 
    }
    
    rID *r ; 
    r = (rID *)calloc(sizeof(*r),total_resources);
    ri = 0;
    

    for (q=0;theGame->Modules[q];q++) {
      Module * m = theGame->Modules[q];
      for (i=0; i<m->NumResources(); i++) 
        r[ri++] = i + ((q+1) * 0x01000000);
      }
      
    list = new String[total_resources + 30];

     String full;
    
    for (t=0;IType[t];t++)
      {
        for (i=0,c=0;i!=total_resources;i++)
          if (RES(r[i])->Type == T_TEFFECT)
            {
              if (TEFF(r[i])->Level == 0)
                continue;
            
              TEffect *te = TEFF(r[i]);
              for (j=0;DungeonItems[j].Type;j++)
                if (DungeonItems[j].Source)
                  if (te->HasSource(DungeonItems[j].Source))
                    goto IsAnItem;
              
              /* It's a spell or special ability; skip it. */
              continue;
              
              IsAnItem:
              
              if (!te->HasSource((int8)IType[t]))
                continue;
              
              if (IType[t] == AI_WAND)               
                list[c] = Format("  [%2d/%2d/%2d/%2d/%2d] %s\n",
                    max(1,LevelAdjust(te->Level,0,1)-2),
                    max(1,LevelAdjust(te->Level,0,2)-2),
                    max(1,LevelAdjust(te->Level,0,3)-2),
                    max(1,LevelAdjust(te->Level,0,4)-2),
                    max(1,LevelAdjust(te->Level,0,5)-2),
                    (const char*)ItemNameFromEffect(r[i]) );
              else
                list[c] = Format("  [%2d/%2d/%2d/%2d/%2d] %s\n",
                    max(1,LevelAdjust(te->Level,0,1)),
                    max(1,LevelAdjust(te->Level,0,2)),
                    max(1,LevelAdjust(te->Level,0,3)),
                    max(1,LevelAdjust(te->Level,0,4)),
                    max(1,LevelAdjust(te->Level,0,5)),
                    (const char*)ItemNameFromEffect(r[i]) );

              c++;
            }  

      
      qsort(list,c,sizeof(String),StringCompare);
      
      full += Format("%c%s:%c\n", -SKYBLUE, Lookup(AI_CONSTNAMES, IType[t]), -GREY);
      
      for (i=0;i!=c;i++)
        full += list[i];
      }

    T1->Box(WIN_SCREEN,BOX_WIDEBOX,YELLOW,GREY,full);

    String path = T1->IncursionDirectory + "\\notes\\itemlist.txt";

    FILE * fp;
    fp = fopen((const char *)path,"wt");
    fwrite(((const char*)full),strlen((const char*)full),1,fp);
    fclose(fp);
    
    theGame->Cleanup();
          
  }
    
    
extern void TestEncounterGen(Term *t);

void Game::StartMenu() {                                      
    int16 i;

Redraw:
    do {
        T1->SetMode(MO_SPLASH);
        T1->Title();
        T1->LOption("Create a New Character",0);
        T1->LOption("Restore a Saved Game",1);
        T1->LOption("Reincarnate a Character",9);
        T1->LOption("Recover a Crashed Game",8);
#if DEBUG
        T1->LOption("Compile Resources",3);
#endif
        T1->LOption("Change System Options",4);
        /* T1->LOption("View the Scoreboard",5); */
        T1->LOption("Read the Introduction",6);
        T1->LOption("General Help",2);
        T1->LOption("Quit Incursion Completely",7);
#ifdef DEBUG
        T1->LOption("(Debugging Commands)",99);
#endif
        switch(i = (int16)T1->LMenu(MENU_2COLS|MENU_REDRAW,"-- Initial Choices -- ",WIN_CUSTOM)) {
        case 99:
            T1->LOption("Monster Evaluation",2);
            T1->LOption("Check Module Consistency",1);
            T1->LOption("Weapon Evaluation",3);
            T1->LOption("Name of Resource ID",4);
            T1->LOption("CR Disambiguation",5);
            T1->LOption("Generate HTML Manual",6);
            T1->LOption("Generate Rarity Table", 7);
            T1->LOption("List Items by Level",8);
            T1->LOption("Test New Encounter Gen",9);
            T1->LOption("Generate Encounter Stats",10);

            switch(T1->LMenu(MENU_2COLS|MENU_ESC|MENU_REDRAW,"-- Debugging Commands -- ",WIN_CUSTOM)) {
            case 1: CheckConsistency(); break; 
            case 2: MonsterEvaluation(); break; 
            case 5: MonsterEvaluationCR(); break; 
            case 3: WeaponEvaluation(); break; 
            case 4: {
                //T1->Scour("Incursion.Exe",true);
                if (!LoadModules())
                    break;
                T1->SetWin(WIN_INPUT);
                T1->Color(YELLOW);
                T1->Write(0,0,"Enter decimal Resource ID: ");
                T1->Color(MAGENTA);
                T1->ReadLine();
                T1->Clear();
                rID xID = atoi(T1->GetTypedString());
                T1->Box(NAME(xID));
                }
                break; 
            case 6: WriteHTMLHelp(); break;
            case 7: GenerateRarityTable(); break;
            case 8: ListItemsByLevel(); break;
            case 9:
                theGame->LoadModules();
                TestEncounterGen(T1); 
                theGame->Cleanup();
                break;
            case 10:
                theGame->LoadModules();
                Map *m;
                m = new Map();
                m->GenEncounterStats(T1);
                delete m;
                theGame->Cleanup();
            case -2:
                goto Redraw;
            } 
            break;
        case 0:
            if (!LoadModules())
                break;

#if 0
            for(i=0;i!=theGame->Modules[0]->szTem;i++)
            for(j=0;j!=6;j++)
	            if (theGame->Modules[0]->QTem[i].NewAttk[j].u.a.Dmg.Number == -52)
		            Error("-52 found!");
#endif

            NewGame(0,false);
            Play();
            Cleanup();
            break;
        case 9:
            if (!LoadModules())
                break;
            NewGame(0,true);
            if (m[0] && p[0])
                Play();
            Cleanup();
            break; 
        case 1:
            if (!LoadGame(false))
                break;
            Play();
            Cleanup();
            break;
        case 8:
            if (!LoadGame(true))
                break;
            Play();
            Cleanup();
            break;
        case 6:
            if (!LoadModules())
                break;
            T1->HelpTopic("help::intro");
            Cleanup();
            break;
        case 2:
            if (!LoadModules())
                break;
            T1->SetWin(WIN_SCREEN);
            T1->Clear();
            T1->HelpTopic(NULL);
            Cleanup();
            break;
#ifdef DEBUG
        case 3: //compile resources
            theGame->ResourceCompiler();
            Cleanup();
            break;
#endif
        case 4:
            T1->SetWin(WIN_SCREEN);
            T1->Clear();
            if (!LoadModules())
                break;
            T1->OptionManager();
            Cleanup();
            break;
        case 7:
            goto Quit;
        case -2:
            goto Redraw;
        default:
            T1->Message("Not implemented yet.");
            break;
        }
    } while(1);

Quit:
    Cleanup();
}

const char* GravestoneImage =
"                                             \n\
                                              \n\
                                              \n\
            -----------------------           \n\
           /                       \\         \n\
          /                         \\        \n\
         /                           \\       \n\
        /                             \\      \n\
       /                               \\     \n\
      |                                 |     \n\
      |       <8>REQUISCANT IN PACE,<7>       |     \n\
      |                                 |     \n\
      |                                 |     \n\
      |                                 |     \n\
      |               <8>A<7>                 |     \n\
      |                                 |     \n\
      |                                 |     \n\
      |                                 |     \n\
      |          <8>WAS KILLED BY<7>          |     \n\
      |                                 |     \n\
      |                                 |     \n\
      |                                 |     \n\
      |             <8>ON THE<7>              |     \n\
      |                                 |     \n\
      |                                 |     \n\
      |                                 |     \n\
      |                                 |     \n\
      |                                 |     \n\
      |                                 |<6>..<7>   \n\
  <2>~~~~<7>|                                 |<2>~~~~~<7>\n\
    <2>~~~~~<6>....<2>~~~~<8>--<2>~~~<8>--<2>~~~<6>\
...<2>~~~<6>..<2>~~~~<6>....<7>   \n";
void Player::Gravestone()
  {
    String DumpFile;
    char buff[40], ch = 'g';
    MyTerm->SetMode(MO_SPLASH);

    theGame->GetPlayer(0)->StoreLevelStats((uint8)m->Depth);

    TouchGallery(false);

    if (VictoryFlag) {
      MyTerm->SetWin(WIN_SCREEN);
      MyTerm->Clear();
      MyTerm->Box(XPrint("_______________<12>*** YOU HAVE WON ***<7>"
        "\n__Congratulations! You have successfully slain the goblin "
        "king and and lived to tell about it! This is the end of "
        "<5>Incursion: Halls of the Goblin King<7>, "
        "but don't despair; more adventures await! Assault is only the "
        "preview teaser-game for the roguelike epic, <5>Incursion: "
        "Return of the Forsaken<7>, due out sometime in 2011."));
      }

    for (int i=0; i<=9999; i++) {
      String filespec;

      filespec = Format("%s\\dump%04d.txt",(const char*)MyTerm->LogSubDir(),i);

      FILE * fp = fopen(filespec,"rt");
      if (fp) { 
        fclose(fp);
      } else {
        filespec = Format("dump%04d",i);
        MyTerm->DumpCharacter(filespec); 
        break; 
      } 
    } 

    do {
      switch (ch) {
        case 'v':
          /* ww: you should be able to review the messages when you die:
           * sometimes that's the only way to see what killed you! */
          MyTerm->ShowMessages();
          /* intentional fall-through, otherwise it looks bad */
        case 'g':
          MyTerm->SetWin(WIN_SCREEN); MyTerm->Clear();
          MyTerm->Color(WHITE);
          MyTerm->Write(0,0,XPrint(GravestoneImage));
          MyTerm->Write(23-strlen(Named)/2,12,Named);
          if (TotalLevel() >= 5)
            sprintf(buff,"Journeyman %s",NAME(ClassID[0]));
          else
            sprintf(buff,"Novice %s",NAME(ClassID[0]));
          MyTerm->Write(23-strlen(buff)/2,16,buff);
          if (!GraveText.GetLength())
            GraveText = "causes unknown";
          GraveText = GraveText.Left(30);
          MyTerm->Write((int16)(23-GraveText.GetLength()/2),20,GraveText);
          sprintf(buff,"12th of Suntide");
          MyTerm->Write(23-strlen(buff)/2,24,buff);
          if (VictoryFlag) {
            MyTerm->Color(PURPLE);
            MyTerm->Write(7,27,"* * * A TRUE HERO OF LEGEND * * *");
            MyTerm->Color(GREY);
            }
          //MyTerm->Write(0,35,"Guild Rankings:\n  None");
          //MyTerm->Write(0,38,"Quests Completed:\n  None");
          //MyTerm->Write(0,41,"Artifacts Handled:\n  None");
         break;
        case 'q':
          MyTerm->Clear();
          MyTerm->SetWin(WIN_SCREEN);
          MyTerm->CursorOn();
          MyTerm->GotoXY(0,0);
          MyTerm->ShutDown();
          exit(0);
         break;
        case 'm':
          return;
        case 's':
          MyTerm->Clear();
          /* theGame->Scoreboard(MyTerm); */
         break;
        case 'd':
          MyTerm->GotoXY(0,MyTerm->WinSizeY()-1); 
          MyTerm->Color(YELLOW);
          MyTerm->Write("Filename:                                                             ");
          MyTerm->GotoXY(10,MyTerm->WinSizeY()-1);
          MyTerm->ReadLine();

          MyTerm->DumpCharacter(SC(MyTerm->GetTypedString())); 
         break;
        }
         
      MyTerm->Color(MAGENTA);
      MyTerm->CursorOn();
      MyTerm->Write(0,MyTerm->WinSizeY()-1,Format("Dump character, reView Messages, Main menu, Quit or Scoreboard? [dvmqs%c%c] ",
        24,25));
      MyTerm->CursorOn();
      while (!strchr("dmvqs",ch = (char)MyTerm->GetCharRaw()))
        ;
      MyTerm->CursorOff();
    }
    while(1);
  }

#ifdef BUILD_SPELL_LIST

void BuildSpellList()
  {
    FILE *f1, *f2, *f3; int32 i;
    TEffect *te;
    //T1->Scour("Incursion.Exe",true);
    if (!theGame->LoadModules())
      return;
    f1 = fopen("sp_list1.txt","wt");
    //f2 = fopen("sp_list2.txt");
    //f3 = fopen("sp_list3.txt");

    fprintf(f1,"Text \"help::arcane spells\"\n  { \"\n");
    for(i=0;i!=theGame->LastSpell();i++)
      {
        te = TEFF(theGame->SpellID(i));
        if (!te->HasSource(AI_WIZARDRY))
          continue;
        fprintf(f1,"\n\n      \\n\\n\n      <7>%s (<1>",NAME(theGame->SpellID(i)));
        if (te->Schools & SC_ABJ)
          fprintf(f1,"Abjuration");
        if (te->Schools & SC_ARC)
          fprintf(f1,"Arcana");
        if (te->Schools & SC_DIV)
          fprintf(f1,"Divination");
        if (te->Schools & SC_ENC)
          fprintf(f1,"Enchantment");
        if (te->Schools & SC_EVO)
          fprintf(f1,"Evocation");
        if (te->Schools & SC_ILL)
          fprintf(f1,"Illusion");
        if (te->Schools & SC_THA)
          fprintf(f1,"Thaumaturgy");
        if (te->Schools & SC_NEC)
          fprintf(f1,"Necromancy");
        if (te->Schools & SC_WEA)
          fprintf(f1,"Weavecraft");
        fprintf(f1," <9>%d<7>)\\n\n      __<2>",te->Level);
        fprintf(f1,DESC(theGame->SpellID(i)));
      }
    fprintf(f1,"\"; }\n\n");

    fprintf(f1,"Text \"help::divine spells\"\n  { \"\n");
    for(i=0;i!=theGame->LastSpell();i++)
      {
        te = TEFF(theGame->SpellID(i));
        if (!te->HasSource(AI_THEURGY))
          continue;
        fprintf(f1,"\n\n      \\n\\n\n      <7>%s (<1>",NAME(theGame->SpellID(i)));
        if (te->Schools & SC_ABJ)
          fprintf(f1,"Abjuration");
        if (te->Schools & SC_ARC)
          fprintf(f1,"Arcana");
        if (te->Schools & SC_DIV)
          fprintf(f1,"Divination");
        if (te->Schools & SC_ENC)
          fprintf(f1,"Enchantment");
        if (te->Schools & SC_EVO)
          fprintf(f1,"Evocation");
        if (te->Schools & SC_ILL)
          fprintf(f1,"Illusion");
        if (te->Schools & SC_THA)
          fprintf(f1,"Thaumaturgy");
        if (te->Schools & SC_NEC)
          fprintf(f1,"Necromancy");
        if (te->Schools & SC_WEA)
          fprintf(f1,"Weavecraft");
        fprintf(f1," <9>%d<7>)\\n\n      __<2>",te->Level);
        fprintf(f1,DESC(theGame->SpellID(i)));
      }
    fprintf(f1,"\"; }\n\n");

    fprintf(f1,"Text \"help::druid spells\"\n  { \"\n");
    for(i=0;i!=theGame->LastSpell();i++)
      {
        te = TEFF(theGame->SpellID(i));
        if (!te->HasSource(AI_DRUIDIC))
          continue;
        fprintf(f1,"\n\n      \\n\\n\n      <7>%s (<1>",NAME(theGame->SpellID(i)));
        if (te->Schools & SC_ABJ)
          fprintf(f1,"Abjuration");
        if (te->Schools & SC_ARC)
          fprintf(f1,"Arcana");
        if (te->Schools & SC_DIV)
          fprintf(f1,"Divination");
        if (te->Schools & SC_ENC)
          fprintf(f1,"Enchantment");
        if (te->Schools & SC_EVO)
          fprintf(f1,"Evocation");
        if (te->Schools & SC_ILL)
          fprintf(f1,"Illusion");
        if (te->Schools & SC_THA)
          fprintf(f1,"Thaumaturgy");
        if (te->Schools & SC_NEC)
          fprintf(f1,"Necromancy");
        if (te->Schools & SC_WEA)
          fprintf(f1,"Weavecraft");
        fprintf(f1," <9>%d<7>)\\n\n      __<2>",te->Level);
        fprintf(f1,DESC(theGame->SpellID(i)));
      }
    fprintf(f1,"\"; }\n\n");


    fclose(f1);
  }

#endif



