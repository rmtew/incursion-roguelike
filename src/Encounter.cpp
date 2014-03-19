/* ENCOUNTER.CPP -- Copyright (c) g1999-2003 Julian Mensch
     Contains the code for generating groups of monsters or random
     items within a given set of specifications; this is used by 
     both summoning effects and the dungeon generator.

     int16 Map::GenEncounter(uint16 fl, int8 CR, int8 Depth, uint32 mon, 
                int8 AType, rID xID, int16 ex, int16 ey, Creature *creator)
     Item* Item::GenItem(uint16 Flags, rID xID, int16 Deptht, int8 Luck, 
                ItemGen *Gen)
*/

#include "Incursion.h"

#define MG_BASE     1
#define MG_LEADER   2
#define MG_POPCORN  3
#define MG_ALLY     4
#define MG_SHAMAN   5

#define MGF_RANGED  0x0001
#define MGF_MAGIC   0x0002
#define MGF_EQUIP   0x0004
#define MGF_POISON  0x0008
#define MGF_SLEEP1  0x0010
#define MGF_SLEEP2  0x0020

/* These are for debugging use, to facilitate getting the text
   name of a mID in Visual Studio's watch window */
const char *txt;
TMonster   *tmm;
int32      tmi;


/* Kludge to convey extra info */
int16 summDuration;
int16 illDuration, 
      illSaveDC,
      illCasterLev;
rID   illEID;



/* This constant determines how far an encounter can deviate
   from what was requested, both in desired xCR and in terms
   of how many monsters were requested (to fill a large room
   of a given size) versus how many are actually generated.
   Higher values result in quicker, more innaccurate encounter
   generation, whereas lower values result in longer hashing
   periods, more accurate encounters and more circumstances
   where the encounter generator gives up and doesn't build
   any encounter at all.
     Note that this has nothing to do with generating out-of-
   depth encounters. That's based on player Luck, and is
   determined in MakeLev.Cpp before GenEncounter is ever
   called. */
#define DEVIANCE_THRESHOLD   20

int16 HomogenousMixedGroups[] = {
  MA_DEMON, MA_DEVIL, MA_CELESTIAL, MA_EARTH, MA_AIR, 
  MA_FIRE, MA_WATER, MA_ILLITHID, MA_SPIRIT, MA_UNDEAD,
  MA_JELLY, MA_PLANT, 
  MA_ADVENTURER, MA_MYTHIC, MA_CONSTRUCT, 0 };
  
  
/*   We are making the following variables global to this file
   only in order to avoid an excessive amount of parameters and
   pointers in different functions. They aren't really program-
   level global variables, and cannot be guaranteed to hold any
   meaningful values when GenEncounter() or its sub-functions 
   are not executing. */
int16 mgCount;        /* # of monster groups */
int16 max_mCount;     /* max # of monsters total */
int16 des_mCount;     /* desired #, based on room size */
int16 cur_mCount;     /* current # of monsters generated */
uint8 enAlign;        /* encounter group alignment stricture */
int32 enMType;        /* encounter overall MA_TYPE */
int16 enDepth;        /* the Deptht of the overall encounter */
int16 enTries;        /* Iterations of main loop */
int16 enPartyID;      /* PartyID for encounter */
uint32 enFlags;       /* Encounter Flags */
int16 enSleep;        /* Encounter Sleep Percentage */
int16 enCR;           /* Encounter desired CR */
int32 enFreakFactor;  /* Encounter "freak factor" */
int32 enXCR;          /* Encounter XCR, adjusted for room size */
rID   enRegID;        /* Region ID for encounter */
rID   enMonID;        /* Monster ID for encounter */
int16 enPurpose;      /* Encounter Generation Purpose */
bool  enIrregular;    /* Generate strange/unlikely groupings? */
bool  enParty;        /* Generate an adventuring party */
bool  enSimilar;      /* Party of all same races */
bool  enPlanarOK;     /* Planar Templates Acceptable */
int16 Deviance,       /* Deviance of Current Group */
      bDeviance;      /* Deviance of best group yet */ 
uint32 RList[33];     /* Resource List Buffer */
Creature *enCreator;

struct MonGroup
  {
    uint8 mgType;
    rID mID, tID[6];
    uint32 TempTypes;
    uint8  mCount, tCount;
    uint16 Flags;
    hObj mItems[10];        
    bool isMType(int32 MType);
    #define VerifyTemplate(x) isMType(-((signed)(x)))
  } mGroups[12], bestGroups[12];



MonGroup* AddGroup(int16 GType, int16 xCR);
rID AppropriateTemplate(uint16 Types, int16 CR, MonGroup *mg);

uint16 GroupAlign(MonGroup *mg)
  {
    uint16 al, i;

    al = ResAlign(mg->mID);
    for (i=0;i!=6;i++)
      if (mg->tID[i])
        {
          if (TTEM(mg->tID[i])->HasFlag(TMF_BODY_ONLY))
            al = AL_NONGOOD;
          if (TTEM(mg->tID[i])->SubFlags[M_GOOD/8] & (M_GOOD % 8))
            al &= ~AL_GOOD;
          if (TTEM(mg->tID[i])->SubFlags[M_EVIL/8] & (M_EVIL % 8))
            al &= ~AL_EVIL;
          if (TTEM(mg->tID[i])->SubFlags[M_LAWFUL/8] & (M_LAWFUL % 8))
            al &= ~AL_LAWFUL;
          if (TTEM(mg->tID[i])->SubFlags[M_CHAOTIC/8] & (M_CHAOTIC % 8))
            al &= ~AL_CHAOTIC;
          if (TTEM(mg->tID[i])->AddFlags[M_GOOD/8] & (M_GOOD % 8))
            al |= AL_GOOD;
          if (TTEM(mg->tID[i])->AddFlags[M_EVIL/8] & (M_EVIL % 8))
            al |= AL_EVIL;
          if (TTEM(mg->tID[i])->AddFlags[M_LAWFUL/8] & (M_LAWFUL % 8))
            al |= AL_LAWFUL;
          if (TTEM(mg->tID[i])->AddFlags[M_CHAOTIC/8] & (M_CHAOTIC % 8))
            al |= AL_CHAOTIC;
        }

    return al;
  }


int16 TotalMCount(rID mID=0)
  {
    int16 n = 0;
    for (int8 i=0;mGroups[i].mID;i++)
      if ((!mID) || mID == mGroups[i].mID)
        n += mGroups[i].mCount;
    return n;
  }

int32 CalcXCR(int16 CR)
  {
    int32 xCR;
    if (CR <= 0)
      xCR = 27 / ((-CR)+2);
    else
      xCR = (CR+2)*(CR+2)*(CR+2);
    return xCR;
  }

int16 CalcCR(int32 xCR)
  {
    int32 i, Cubes[] = {
      27/9,     27/8,     27/7,     27/6,
      27/5,     27/4,     27/3,     27/2,

      3*3*3,    4*4*4,    5*5*5,    6*6*6,    
      7*7*7,    8*8*8,    9*9*9,    10*10*10, 
      11*11*11, 12*12*12, 13*13*13, 14*14*14, 
      15*15*15, 16*16*16, 17*17*17, 18*18*18, 
      19*19*19, 20*20*20, 21*21*21, 22*22*22, 
      23*23*23, 24*24*24, 25*25*25, 26*26*26, 
      27*27*27, 28*28*28, 29*29*29, 30*30*30,
      31*31*31, 32*32*32 };
    
    for (i=0;i!=39;i++)
      if (Cubes[i] > xCR)
        return i - 8;
    return 30;
  }

int32 GroupXCR(MonGroup *mg)
  {
    int16 CR, n, i; int32 xCR;

    CR = TMON(mg->mID)->CR;

    for (i=0;i!=mg->tCount;i++)
      CR = TTEM(mg->tID[i])->CR.Adjust(CR);

    if (mg->Flags & MGF_POISON)
      CR++;
    if (mg->Flags & MGF_MAGIC)
      CR++;

    xCR = CalcXCR(CR);
    
    xCR *= mg->mCount;

    return xCR;
  }

int32 TotalXCR()
  {
    int32 xCR = 0; int16 i;
    for (i=0;mGroups[i].mID;i++)
      xCR += GroupXCR(&mGroups[i]);
    return xCR;
  }

// ww: Return the creatures from the last GenEncounter. Helpful if it was
// EN_NOPLACE. 
Creature * CandidateCreatures[2048]; 

Creature * Map::GetEncounterCreature(int32 i)
{
  if (i < 0 || i >= (sizeof(CandidateCreatures)/sizeof(CandidateCreatures[0]))) return NULL;
  else return (CandidateCreatures[i]);
} 


int16 Map::GenEncounter(uint32 fl, int8 CR, int8 Depth, uint32 mon, int8 AType, rID xID,
                          int16 ex, int16 ey, Creature*creator)
  {
    static uint8 PartyID = 100, i;
    MonGroup *mg;
    bool doneLeader, doneShaman;

    txt = theGame->Modules[0]->QTextSeg;
    tmm = theGame->Modules[0]->QMon;
    tmi = theGame->Modules[0]->MonsterID(0);

    memset(Candidates,0,sizeof(Candidates));
    memset(CandidateCreatures,0,sizeof(CandidateCreatures));

    Restart:
    enCR = CR;
    enFlags = fl;
    enDepth = Depth;
    enAlign = AType;
    enCreator = creator;
    enPurpose = PUR_DUNGEON;
    enFreakFactor = random(5000);
    enIrregular = !random(6);
    if (!(fl & (EN_SUMMON|EN_STREAMER|EN_CREATOR|EN_ILLUSION))
           || (fl & EN_HOSTILE)) {
      PartyID++;
      if (PartyID < (MAX_PLAYERS+4))
        PartyID = MAX_PLAYERS+4;
      enPartyID = PartyID;
      }
    else if ((fl & (EN_SUMMON|EN_CREATOR|EN_ILLUSION)) && creator)
      enPartyID = creator->PartyID;
    else if (fl & EN_STREAMER)
      enPartyID = MAX_PLAYERS + 1;
    else
      enPartyID = 0;
    enRegID = (xID && RES(xID)->Type == T_TREGION) ? xID : 0;
    enMonID = (enFlags & EN_MONID) ? mon : 0;
    enMType = (enFlags & EN_MTYPE) ? mon : 0;
    enPlanarOK = !random(4);

    if (fl & EN_SINGLE) {
      max_mCount = 1;
      des_mCount = 1;
      }
    else if (fl & EN_ANYOPEN) {
      max_mCount = min(32,OpenC/3);
      if (Depth > 2)
        des_mCount = OpenC / 30;
      else if (Depth == 2)
        des_mCount = OpenC / 50;
      else
        des_mCount = OpenC / 75;
      }
    else if (fl & EN_MULTIPLE) {
      max_mCount = 32;
      des_mCount = 12;
      }
    else {
      max_mCount = 32;
      des_mCount = 1;
      }

    /* Are we building a party of adventurers? */
    if ((enMType == MA_ADVENTURER || (!random(6) && !enMType && !enMonID)) && CR >= 3)
      { enMType = MA_ADVENTURER; enParty = true;
        enSimilar = random(2);         
        des_mCount = max(des_mCount,3+random(4)); }
    else
      { enParty = false; enSimilar = false; }

    // ww: far too many monsters in Incursion are generated asleep! 
    enSleep = (!random(5)) ? 100 : ((!random(4)) ? 0 : (20+random(61)));
    enXCR = CalcXCR(CR);
    
    /* We need to increase the xCR in large rooms with a high number
       of desired monsters, else we end up with wierd imbalances and
       hashing. To balance this, we increase the chances that more of
       the monsters will be asleep, initially. */
    if (des_mCount >= 4) {
      enXCR *= (des_mCount / 3);   
      for (i=0;i!=(des_mCount / 3);i++)
        enSleep = max(enSleep,random(100)+1);
      }
    
    bDeviance = 1000;

    /* Generate Single Specific Monster? */
    if (enFlags & EN_SPECIFIC)
      {
        ASSERT(enMonID > 0xFFFF)
        memset(&mGroups[0],0,sizeof(mGroups[0]));
        mGroups[0].mgType = MG_BASE;
        mGroups[0].mID = enMonID;
        mGroups[0].mCount = 1;
        if (xID && RES(xID)->Type == T_TTEMPLATE)
          {
            mGroups[0].tID[0] = xID;
            mGroups[0].tCount = 1;
          }
        mgCount = 1;
        enTries = 1;
      }
    else for (enTries=0;enTries!=100;enTries++)
      {
        RestartGen:
        enTries++;
        //theGame->GetPlayer(0)->IPrint(".");
        //T1->Update();
        mgCount = 0; mg = NULL;
        memset(mGroups,0,sizeof(MonGroup)*12);
        for(i=0;i!=10 && !mg;i++)
          mg = AddGroup(MG_BASE,enXCR / (enParty ? 4 : 1)); 
        if (!mg)
          continue;
        mGroups[mgCount++] = *mg;
               
        doneLeader = doneShaman = 0;
        
        while ((TotalXCR() < (enXCR*2)/3 || ((TotalMCount()*3)/2) < des_mCount) && random(10))
          {
            if (mgCount >= 11)
              break;
            if ((des_mCount > TotalMCount()) && !enParty)
              {
                mg = AddGroup(MG_POPCORN,max(enXCR/4,enXCR - TotalXCR()));
                if (mg && (TotalMCount() + mg->mCount <= max_mCount)) {
                  mGroups[mgCount++] = *mg;
                  continue;
                  }
              }

            if (TMON(mGroups[0].mID)->HasFlag(M_CHIEFTAN) && !enParty)
              if (TotalMCount(mGroups[0].mID) >= 4 && !doneLeader)
                {
                  mg = AddGroup(MG_LEADER,max(enXCR/4,enXCR - TotalXCR()));
                  if (mg && (TotalMCount() + mg->mCount <= max_mCount)) {
                    mGroups[mgCount++] = *mg;
                    doneLeader = true;
                    continue;
                    }
                }

            if (TMON(mGroups[0].mID)->HasFlag(M_SHAMAN) && !enParty)
              if (TotalMCount(mGroups[0].mID) >= 6 && !doneShaman)
                {
                  mg = AddGroup(MG_SHAMAN,max(enXCR/4,enXCR - TotalXCR()));
                  if (mg && (TotalMCount() + mg->mCount <= max_mCount)) {
                    mGroups[mgCount++] = *mg;
                    doneShaman = true;
                    continue;
                    }
                }
      
            if (TMON(mGroups[0].mID)->CR <= min(CR-3,CR/3) && !doneLeader){
              mg = AddGroup(MG_LEADER,max(enXCR/4,enXCR - TotalXCR()));
              if (mg && (TotalMCount() + mg->mCount <= max_mCount))
                { mGroups[mgCount++] = *mg; doneLeader = true; }
              }
            else {
              mg = AddGroup(MG_ALLY,max(enXCR/4,enXCR - TotalXCR()));
              if (mg && (TotalMCount() + mg->mCount <= max_mCount))
                mGroups[mgCount++] = *mg;
              }

            if (TotalMCount() > max_mCount)
              break;
          } 
      
      Deviance = abs(enXCR-TotalXCR())*10 / min(enXCR,TotalXCR());
      if (des_mCount != 1)
        Deviance += abs(TotalMCount() - des_mCount);
      

      if (bDeviance > Deviance)
        {
          bDeviance = Deviance;
          memcpy(bestGroups,mGroups,sizeof(MonGroup)*12);
        }

      if (bDeviance > DEVIANCE_THRESHOLD)
        continue;
      
      if (enTries > 6)
        break;
          
      }  

    if (enTries == 100)
      {
        //memset(mGroups,0,sizeof(MonGroup)*12);
        return 0;
      }
    if (!(enFlags & EN_SPECIFIC))
      memcpy(mGroups,bestGroups,sizeof(MonGroup)*12);

    if (!(enFlags & EN_NOBUILD))
      PlaceEncounter(ex,ey,creator);

    return 1;
  }

String & Map::PrintEncounter()
  {
    String str, n; int16 i, j;
    
    str = Format("%cCR %d Encounter (xCR %d/%d, Depth %d, Dev %d, enTries %d, enSleep %d~):\n%c",
              -PINK,enCR, TotalXCR(),enXCR,Depth,bDeviance,enTries,enSleep,-GREY);
    if (enSimilar)
      str += "(homogenous encounter) ";
    else if (enIrregular)
      str += "(irregular encounter) ";
    if (enParty)
      str += "(adventuring party) ";
    if (enIrregular || enSimilar || enParty)
      str += "\n";
    if (enTries == 100)
      {
        str += Format("  %c[Encounter failed to generate properly]\n",
                    -RED,-GREY);
      }

    for (i=0;mGroups[i].mID;i++)
      {
        str += -7; str += "-- ";                        
        if (mGroups[i].mCount != 1)
          str += Format("%d ",mGroups[i].mCount);
        n.Empty();
        for (j=0;j!=6;j++)
          if (mGroups[i].tID[j] && !TTEM(mGroups[i].tID[j])->HasFlag(TMF_POSTSCRIPT))
            { n += NAME(mGroups[i].tID[j]); n += " "; }
        n += NAME(mGroups[i].mID);
        for (j=0;j!=6;j++)
          if (mGroups[i].tID[j] && TTEM(mGroups[i].tID[j])->HasFlag(TMF_POSTSCRIPT))
            { n += " "; n += NAME(mGroups[i].tID[j]); }
        
        if (mGroups[i].mCount != 1)
          n = Pluralize(n);

        str += n;

        str += Format(" [CR %d, xCR %d]",
              CalcCR(GroupXCR(&mGroups[i])/mGroups[i].mCount),
              GroupXCR(&mGroups[i]));


        switch(mGroups[i].mgType)
          {
            case MG_POPCORN:
              str += " (popcorn)";
             break;
            case MG_LEADER:
              str += " (leader)";
             break;
            case MG_ALLY:
              str += " (ally)";
             break;
            case MG_SHAMAN:
              str += " (magic support)";
             break;
          }

        if (mGroups[i].Flags & MGF_MAGIC) {
          if ((mGroups[i].mCount*2/3) == 1)
            str += Format("\n     [with extra magic items]");
          else
            str += Format("\n     [1 in %d with extra magic items]",
                          1+(mGroups[i].mCount*2/3));

          }
        if (mGroups[i].Flags & MGF_POISON)
          str += "\n     [with poisoned weapons]";
        str += "\n";
      }
      
    return *tmpstr(str);
  }       

void Map::PlaceEncounter(int16 ex, int16 ey, Creature *creator)              
  {  
    int16 i, j, k, c, Tries;
    Monster *mn, *mna[1024];
    Item *it; rID xID;

    c = 0; // ww: paranoia! 
    for (i=0;mGroups[i].mID;i++)
      {
        
        for (j=0;j!=mGroups[i].mCount;j++)
          {
            if (RES(mGroups[i].mID)->Type != T_TMONSTER) {
              Error(XPrint("Non-Monster resource <Res> in PlaceEncounter!", 
                              mGroups[i].mID));
              continue;
              }
            mn = new Monster(mGroups[i].mID);
            mn->PartyID = enPartyID;
            TMON(mGroups[i].mID)->GrantGear(mn,mGroups[i].mID,true);
            for (k=0;mGroups[i].tID[k];k++) {
              mn->AddTemplate(mGroups[i].tID[k]);
              if (!mn->HasEffStati(TEMPLATE,mGroups[i].tID[k]))
                mn->GainPermStati(ILLUMINATED,NULL,SS_MISC);
              TTEM(mGroups[i].tID[k])->GrantGear(mn,mGroups[i].tID[k],true); 
              TTEM(mGroups[i].tID[k])->PEvent(EV_BIRTH,mn,mGroups[i].tID[k]);  
              }
            // weimer paranoia: I'm getting non undead from kicking
            // gravestones. 
            if (enMType && !mn->isMType(enMType)) {
              if (enMType == MA_UNDEAD) {
                
                //mn->AddTemplate(FIND("graveborn;template"));
                }
            } 
            TMON(mGroups[i].mID)->PEvent(EV_BIRTH,mn,mGroups[i].mID);  
            mn->MonsterGear();
            if ((enFlags & EN_DUNGEON) && random(100)+1 <= enSleep &&
                !mn->ResistLevel(AD_SLEE)) {
              mn->GainPermStati(SLEEPING,NULL,SS_MISC,SLEEP_NATURAL);
              }
            if (mGroups[i].Flags & MGF_POISON) {
              xID = theGame->GetEffectID(PUR_POISON,0,max(3,enCR));
              for(it = mn->FirstInv();it;it = mn->NextInv())
                if (it->isType(T_WEAPON) || it->isType(T_MISSILE))
                  if (it->HasIFlag(WT_PIERCING) || it->HasIFlag(WT_SLASHING)) 
                    it->GainPermStati(POISONED,NULL,SS_MISC,3,0,xID);
              }


            if (mGroups[i].Flags & MGF_MAGIC) 
              if (!random(1 + (mGroups[i].mCount*2)/3)) {
                it = Item::GenItem(0,dID,(Depth*2)/3,10,MonsterItems);
                if (it)
                  mn->GainItem(it,true);
                }

            mna[c++] = mn;
            ASSERT(c < (sizeof(mna)/sizeof(mna[0])));
          }
      }    

    if (enFlags & (EN_SUMMON|EN_CREATOR|EN_ILLUSION)) {
      for (i=0;i!=c;i++)
        {
          if (!(enFlags & EN_HOSTILE))
            mna[i]->PartyID = creator->PartyID;
          Creature * true_leader = creator->getLeader();
          if (!true_leader) true_leader = creator; 
          if (enFlags & EN_SUMMON) {
            mna[i]->GainTempStati(SUMMONED, creator,summDuration, SS_MISC,
                                     0,0,EventStack[EventSP].eID); 
            if (!(enFlags & EN_HOSTILE))
              mna[i]->ts.addCreatureTarget(true_leader,TargetSummoner);
          } else if (enFlags & EN_ILLUSION) {
            mna[i]->GainTempStati(ILLUSION, creator, illDuration, SS_ENCH,
              illSaveDC, 0, illEID, illCasterLev);
            creator->GainPermStati(DISBELIEVED,mna[i],SS_MISC);
            mna[i]->ts.addCreatureTarget(true_leader,TargetMaster);
          } else {
            if (!(enFlags & EN_HOSTILE))
              mna[i]->ts.addCreatureTarget(true_leader,TargetMaster);
          } 
          
          if (creator->isPlayer() && !(enFlags & EN_HOSTILE))              
            mna[i]->MakeCompanion((Player*)creator,PHD_MAGIC);
          else 
            mna[i]->PartyID = enPartyID;
          
          if (enFlags & EN_HOSTILE)
            mna[i]->TurnHostileTo(creator);
          
          if (!(enFlags & (EN_NOPLACE|EN_NOBUILD)))
            mna[i]->IDPrint(NULL,"An <Obj> appears!",mna[i]);

        }
      }
            

    /* Place the created things where needed */
    for(i=0;i<c;i++) {
      CandidateCreatures[i] = mna[i];
      Candidates[i] = mna[i]->myHandle; 
      /*
      if (theGame->GetPlayer(0)->x > 0)
        theGame->GetPlayer(0)->IPrint(Format("Monster[%d] = %s (%d).",i,
              (const char*)mna[i]->Name(0),mna[i]->myHandle));
              */
    }
    CandidateCreatures[i] = NULL;
    Candidates[i] = 0;
    if (enFlags & EN_NOPLACE) {
    }
    else if (enFlags & EN_ANYOPEN) {
      for(i=0;i!=c;i++) {
        Tries = 0;
        Retry:

        if (Tries >= 10)
          { mna[i]->Remove(true); continue; }

        /* Place aquatic monsters in water, and other monsters on dry
           land. No exceptions -- even amphibians start in their favored
           terrain. */
        j = random(OpenC);
        if (mna[i]->HasMFlag(M_AQUATIC)) {
          if (!TTER(TerrainAt(OpenX[j],OpenY[j]))->HasFlag(TF_WATER))
            { Tries++; goto Retry; }
        } else {
          // not aquatic 
          if (TTER(TerrainAt(OpenX[j],OpenY[j]))->HasFlag(TF_WATER))
            { Tries++; goto Retry; }
        } 
        // ww: spiders can start in 'sticky' web
        if (TTER(TerrainAt(OpenX[j],OpenY[j]))->HasFlag(TF_STICKY)) {
          if (!mna[i]->isMType(MA_SPIDER)) 
            { Tries++; goto Retry; }
        } else  
        /* Don't place anything on dangerous terrain, unless it's deep
           water and an aquatic monster. */
        if (TTER(TerrainAt(OpenX[j],OpenY[j]))->HasFlag(TF_WARN)) {
          // ww: terrain with TF_WARN now support a special event that says
          // if it is OK to put the monster there ...
          if (TTER(TerrainAt(OpenX[j],OpenY[j]))->
              PEvent(EV_MON_CONSIDER,mna[i],mna[i]->mID) == ABORT)
            { Tries++; goto Retry; }
        }
        Feature * f;
        for (f=FFeatureAt(OpenX[j],OpenY[j]);f;f=NFeatureAt(OpenX[j],OpenY[j]))
          if (f && (TFEAT(f->fID)->PEvent(EV_MON_CONSIDER,mna[i],f,mna[i]->mID) 
                  == ABORT)) 
              { Tries++; goto Retry; }

        mna[i]->PlaceAt(this,OpenX[j],OpenY[j]);
        mna[i]->Initialize(true);
        }
      }
    else {
      for(i=0;i!=c;i++) {
        mna[i]->PlaceAt(this,ex,ey);
        mna[i]->Initialize(true);
      }
    }
    
    
    /* Initialize calls CustomAlign(), which may give us a party
       with an alignment conflict -- fix that here. */
    bool isBad;
    for (i=0;i!=c;i++) {
      k = 0;
      
      if (mna[i]->HasMFlag(M_IALIGN) ||
          !mna[i]->isMType(MA_SAPIENT))
        continue;
      
      RetryAlign:
      
      isBad = false;
      if (AlignConflict(mna[i]->getAlignment(),enAlign))
        isBad = true;
      for (j=0;j!=c;j++)
        if (i > j || mna[j]->HasMFlag(M_IALIGN))
          if (AlignConflict(mna[i]->getAlignment(),enAlign))
            isBad = true;
      
      if (isBad) {
        k++;
        if (k > 15)
          continue;
        mna[i]->CustomAlign();
        goto RetryAlign;
        }
      }
      
    return; 
  } 

void Monster::MonsterGear()
  {
    Item *it;
    if (!(HasAbility(CA_INFRAVISION) ||
          HasAbility(CA_TREMORSENSE) ||
          HasAbility(CA_BLINDSIGHT) ) ) {
      if (!HasMFlag(M_NOHANDS)) {
          it = Item::Create(FIND("torch"));
          // ww: since monster torches do not currently wear off, giving
          // a monster more than one just adds clutter
          it->SetQuantity(Dice::Roll(1,1,0));
          if (it)
            GainItem(it,true);
      } else {
        // many animals (e.g., brown bear) cannot see in the dark *and*
        // have no hands ... as a massive hack to prevent them from being
        // sitting ducks, we give them infravision!
        GainPermStati(EXTRA_ABILITY,NULL,SS_MISC,CA_INFRAVISION,6);
      } 
    }

    if (!random(20))
      if (HasMFlag(M_HUMANOID) && !HasMFlag(M_NOHANDS))
        {
          it = Item::GenItem(IG_MONEQUIP,0,ChallengeRating()-1,10,
            CasterLev() ? MageItems : MonsterItems);
          if (it)
            GainItem(it,true);
        }


  }

/* Appropriate Group Rules:
   1. Elementals are paired with things that share their
      elemental subtype.
   2. Dragons come either solitary or as mated pairs, and
      sometimes have humanoid servants or younger dragons
      with them.
   3. Humanoids may work together, alignment permitting
      (i.e., a fire giant and orcs, or a troll with kobold
      lackies.)
   4. Jellies are found in multiples with skill-template
      leaders.
   5. Demons, devils and celestials are found in completely
      mixed groups of their own kind.
   6. Monsters have lists of allies (and typical mounts)
      in their writeup.
   7. Humanoids get chieftans and shamen.
   8. Fungi are generated in much larger groups then other
      creature types. Same for other immobiles.
   10. Undead are found 30% in completely mixed groups, and
      otherwise in groups of their own kind/template or with
      specifically listed allies.
   11. Solitary or near-Depth CR monsters are rerolled for
      big rooms; to compensate, small rooms (without an MA_TYPE)
      roll twice to keep the high CRs equally common.
   12. Monsters with M_EXCLUSIVE only form groups with themselves
      and their listed pets/allies. Grimlocks, frex, don't team
      with trolls the way orcs might.
   13. Vermin form big groups -- as big as possible -- with only
      their listed allies / templates. Same for misc. Outsiders.   
   14. Requesting groups of MA_UNDEAD may in fact return a group
      with a base mID of type MA_NLIVING, with the skeleton or
      zombie template applied.
   15. Requesting groups of MA_OUTSIDERS may return MA_ANIMAL
      groups with the celestial or fiendish templates.

   Monster Lists:
     GROUP_LEADER
     GROUP_FOLLOWER
     GROUP_MOUNT
     GROUP_PETS

*/

int16 MaxGroupSize(MonGroup *mg, int16 xCR)
  {
    int32 cxCR, /* Current CR */ 
          nxCR, /* Needed  CR */
          ixCR; /* Individual xCR */

    cxCR = GroupXCR(mg);
    nxCR = xCR - cxCR;
    ixCR = cxCR / mg->mCount;

    return nxCR / ixCR;
  }


MonGroup* AddGroup(int16 GType, int16 xCR)
  {
    int32 xMType = enMType, i, m, c = 0, j, Tries; 
    rID xID, xID2; int16 baseCR, CR;
    static MonGroup mg; bool isHomog = false;

    //theGame->GetPlayer(0)->IPrint(Format("AddGroup(%d,%d).",GType,xCR));
    //T1->Update();
            
    /* What this line does is determine the maximum possible base CR
       for the mID to be selected, based on the overall requested CR 
       of the encounter. The remainder of the encounter's CR will be
       made up by increasing numbers, adding templates or magic, and
       so forth. */
    if (enParty)
      baseCR = min(random(CalcCR(xCR))+1,3);
    else
      baseCR = max(random(CalcCR(xCR))+1,random(CalcCR(xCR))+1);
    if (baseCR > enCR)
      baseCR = enCR;

    baseCR = max(baseCR,0);
    
    /* Allocate a new MonGroup */
    if (mgCount >= 12)
      return NULL;
     
    Tries = 0; 
     
    BadTempRestart:
      
    Tries++;
      
    memset(&mg,0,sizeof(MonGroup));
    mg.mgType = GType;

    /* Rule 14. Templated undead are generated in the following circumstances:
         50% of the time if the encounter is EL 3 or below, 66% of the time if
         we're generating a popcorn group, or 25% of the time otherwise. */
    if (enMType == MA_ZOMBIE && (baseCR < 6 || random(3)))
      goto ZombieTemplate;
    if (!enMonID && (!enMType || enMType == MA_UNDEAD))
      if (((enMType == MA_UNDEAD) || (GType == MG_BASE && !enParty)) && !random(2))
        if ((enMType == MA_UNDEAD) || (baseCR < 4 && random(2)) || (GType == MG_POPCORN && !random(3)) || !random(5))
          {
            ZombieTemplate:
            xID = theGame->GetTempID(TM_UNDEAD,FIND("human"),max(2,(baseCR*2)/3));
            if (!xID) 
              xID = FIND("graveborn;template");
            mg.tID[mg.tCount++] = xID;
            xMType = MA_NLIVING;
            enMType = MA_UNDEAD;
          }
    
    /* Rule 15. When we want outsiders, we sometimes generate celestial or fiendish
         animals instead. The chances are 100% at CR 2 or lower, 50% at CR 3-4 and
         20% thereafter. This is mostly so clerical Holy Summoning spells can give
         you a celestial badger et al., just like in canon. */
    if (enMType == MA_HOLY_SUMM && enCreator) {
      if (enCreator->isMType(MA_GOOD))
        xMType = MA_CELESTIAL;
      else if (enCreator->isMType(MA_EVIL) &&
          enCreator->isMType(MA_LAWFUL))
        xMType = MA_DEVIL;
      else if (enCreator->isMType(MA_EVIL) &&
          enCreator->isMType(MA_CHAOTIC))
        xMType = MA_DEMON;
      else if (enCreator->isMType(MA_EVIL))
        xMType = MA_OUTSIDER + MA_EVIL*256;
      else if (enCreator->isMType(MA_CHAOTIC))
        xMType = MA_COUTSIDER;
      else if (enCreator->isMType(MA_LAWFUL))
        xMType = MA_LOUTSIDER;
      else
        xMType = MA_ELEMENTAL;
                
      if ((baseCR < 5 && !random(2)) || (baseCR >= 5 && !random(5)))
        {
          if (enCreator->isMType(MA_EVIL))
            xID = FIND("fiendish");
          else
            xID = FIND("celestial");
          mg.tID[mg.tCount++] = xID;
          if (enCreator->isMType(MA_GOOD))
            xMType = MA_NLIVING + MA_NONEVIL*256;
          else if (enCreator->isMType(MA_EVIL))
            xMType = MA_NLIVING + MA_EVIL*256;
          else
            xMType = MA_NLIVING;
        }
      baseCR++;
      }
    else if (enMType == MA_HOLY_SUMM)
      xMType = MA_OUTSIDER;

    switch (GType) 
      {
        case MG_BASE:
          if (enMonID)
            mg.mID = enMonID;
          if (enMType && !mg.mID) {
            mg.mID = theGame->GetMonID(enPurpose,-10,baseCR,enDepth,xMType ? xMType : enMType);
            if (!mg.mID)
              return NULL;
            }
          if (!mg.mID)
            mg.mID = theGame->GetMonID(enPurpose,-10,baseCR,enDepth,0);
          if (!mg.mID) {
            Error("Cannot find mID for MG_BASE!");
            mg.mID = FIND("human");
            }
         break;
        case MG_LEADER:
        case MG_POPCORN:
        case MG_ALLY:
        case MG_SHAMAN:
          TMonster *tm; bool isPet, isTyped;
          ASSERT(mGroups[0].mID);
          tm = TMON(mGroups[0].mID);
          if (!tm->isMType(mGroups[0].mID,MA_UNDEAD))
            if (mGroups[0].isMType(MA_UNDEAD))
              // ww: you had '== MA_UNDEAD' here! 
              { if (!xMType) 
                  xMType = MA_UNDEAD;
                goto UndeadGroup; }
          
          if (GType == MG_LEADER && tm->HasFlag(M_CHIEFTAN) && TotalMCount() >= 4) {
              mg.mID = mGroups[0].mID;
              xID = AppropriateTemplate(TM_CHIEFTAN,CalcCR(xCR),&mg);
              if (!xID)
                mg.mID = 0;
              else {
                mg.tID[mg.tCount++] = xID;
                goto GotMID;
                }
              }

          if (GType == MG_SHAMAN && tm->HasFlag(M_SHAMAN) && TotalMCount() >= 4) {
              mg.mID = mGroups[0].mID;
              xID = AppropriateTemplate(TM_SHAMAN,CalcCR(xCR),&mg);
              if (!xID)
                mg.mID = 0;
              else {
                mg.tID[mg.tCount++] = xID;
                goto GotMID;
                }
              }
          if (GType == MG_SHAMAN)
            return NULL;              

          if (GType == MG_ALLY && tm->GetList(GROUP_ALLY,RList,32)) 
            mg.mID = RList[random(ListLength)];
          else if (GType != MG_POPCORN && tm->GetList(GROUP_LEADER,RList,32)) 
            mg.mID = RList[random(ListLength)];
          else if (GType != MG_LEADER && tm->GetList(GROUP_FOLLOWER,RList,32)) 
            mg.mID = RList[random(ListLength)];
          else {
            if (tm->HasFlag(M_EXCLUSIVE) || enSimilar || 
                (tm->isMType(mGroups[0].mID,MA_DRAGON) && !enIrregular))
              { mg.mID = mGroups[0].mID; ASSERT(mg.mID); goto GotMID; }
            for (i=0;HomogenousMixedGroups[i];i++)
              if (mGroups[0].isMType(HomogenousMixedGroups[i]))
                if (!tm->isMType(mGroups[0].mID,MA_DRAGON))
                  { xMType = HomogenousMixedGroups[i]; isHomog = true; }
            if (!xMType && random(2))
              { mg.mID = mGroups[0].mID; ASSERT(mg.mID); goto GotMID; }


            UndeadGroup:
            c = 0;
            isPet = mGroups[0].isMType(MA_PERSON) ||
                    mGroups[0].isMType(MA_DRAGON);

            /* This is kludged to avoid making troll caves be too hard.
               MA_TROLL is one of the only MA_TYPEs that is requested to
               fill large rooms at reasonably low depths, yet has no low-
               level monsters. */
            isTyped = (enMType != 0) && (enMType != MA_TROLL || enDepth >= 10);
            Retry:
            
            for (m=0;m!=MAX_MODULES && theGame->Modules[m]
                     && c < 2048;m++)
              for (i=0;i!=theGame->Modules[m]->szMon 
                     && c < 2048;i++)
                {      
                  tm = &(theGame->Modules[m]->QMon[i]);
                  if (xMType && !tm->isMType(mGroups[0].mID, xMType))
                    continue;
                  if (tm->Depth > enDepth)
                    continue;
                  if (tm->HasFlag(M_NOGEN) || tm->HasFlag(M_UNIQUE))
                    continue;
                  if (tm->HasFlag(M_EXCLUSIVE) || tm->HasFlag(M_UNIQUE))
                    continue;
                  if (!(isPet || enIrregular || xMType)) {
                    if (!tm->HasFlag(M_HUMANOID))
                      continue;
                    }
                  if (isPet) {
                    if (!tm->HasFlag(M_HUMANOID))
                      if (!tm->isMType(mGroups[0].mID,MA_NLIVING))
                        continue;
                    }
                  if (GType == MG_POPCORN) {
                    if (tm->CR > min(baseCR - 3, baseCR / 3))
                      continue;
                    }
                  if (GType == MG_LEADER)
                    if (tm->CR > baseCR || tm->CR < baseCR/2)
                      continue;
                  if (GType == MG_ALLY)
                    if (tm->CR > baseCR-2)
                      continue;
                  if (isTyped)
                    if (!tm->isMType(mGroups[0].mID,enMType))
                      continue;
                  
                  for (j=0;j!=mgCount;j++) {
                    if (AlignConflict(ResAlign(theGame->Modules[m]->
                      MonsterID(i)),GroupAlign(&mGroups[j])))
                        continue;
                    }

                  /* We might generate groups of elementals and their
                     kin (pech, living holocausts, aerial servants,
                     etc.) but not dragons, whose ties to the elements
                     aren't that strong. */
                  if (tm->isMType(mGroups[0].mID,MA_DRAGON))
                    if (xMType == MA_EARTH || xMType == MA_FIRE ||
                        xMType == MA_WATER || xMType == MA_AIR )
                      continue;
                  Candidates[c] = theGame->Modules[m]->MonsterID(i);
                  ASSERT(Candidates[c]);
                  c++;
                }
            
            if (!c && isTyped)
              { 

                /* Try again, looking outside the type. */
                isTyped = false;
                goto Retry;
              }
            if (!c)
              return NULL; 
            mg.mID = Candidates[random(c)];
            // ww: I have gotten here with c == -20820 ...
            // presumably because your "goto UndeadGroup" skipped the
            // initialization of 'c=0' ...
            ASSERT(mg.mID);
            }

      }
  

    
    ASSERT (mg.mID);
    GotMID:
    ASSERT (mg.mID);
    mg.mCount = 1;

    if (TMON(mg.mID)->HasFlag(M_GROUP))
      mg.mCount = 3 + random(4);

    /* Reduce popcorn in homogenous groups in order to increase
       the overall diversity of the group. */
    if (isHomog && GType == MG_POPCORN && random(4))
      return NULL;

    /* Can't create dragons without age catagory templates! */
    if (TMON(mg.mID)->isMType(mg.mID,MA_DRAGON)) {
      /* Make sure we select a high age catagory template; we
         don't want rooms full of hatchlings of DLev 7.  */
      mg.tID[mg.tCount] = 0;
      for (i=0;i!=4;i++) {
        xID = theGame->GetTempID(TM_AGECAT,mg.mID,enCR);
        if (mg.tID[mg.tCount] == 0 || (TTEM(xID)->CR.Adjust(7) > 
              TTEM(mg.tID[mg.tCount])->CR.Adjust(7)))
          mg.tID[mg.tCount] = xID;
        if (GType == MG_POPCORN)
          break;
        }
      if(!mg.tID[mg.tCount])
        return NULL;     
      mg.tCount++;
      }    

    /* Planar Templates apply to entire encounter */
    for (i=0;i!=6 && mGroups[0].tID[i]; i++)
      if (TTEM(mGroups[0].tID[i])->TType & TM_PLANAR)
        {
          for (j=0;j!=6 && j!=mg.tCount;j++)
            if (mg.isMType(MA_UNDEAD)) {
              if ( mGroups[0].tID[i] == FIND("fiendish") )
                goto DonePlanarTemplate;
              /* We've generated an unusable undead -- a zombie
                 badger in a group of celestial elves, for ex. */
              return NULL;  
              }
          if (j == 6)
            /* No room for plane template */
            return NULL;
          if (!mg.isMType(TTEM(mGroups[0].tID[i])->ForMType))
            return NULL;
          mg.tID[j] = mGroups[0].tID[i];
          break;
        }
    DonePlanarTemplate:


    
    /* First off, we try to add a skill template if the CR of our chosen
       monster is more than 2d2 points below the desired CR of the encounter
       we're creating. */
    if (!random(7) || (CalcCR(GroupXCR(&mg) < ((CalcCR(xCR)-(2+random(2))) / 
         (GType == MG_POPCORN ? 3 : 1) )))) {
      xID = 0;                                           
      if (enParty)
        xID = AppropriateTemplate(TM_CLASS,CalcCR(xCR),&mg);
      else if (TMON(mg.mID)->HasFlag(M_HUMANOID) && !enMonID &&
                (TMON(mg.mID)->isMType(mg.mID,MA_GOBLINOID) || !random(4)))
        xID = AppropriateTemplate(TM_SKILL,CalcCR(xCR),&mg);
      if (xID) 
        mg.tID[mg.tCount++] = xID;
      }

    /* Now that we have found the group's base mID, we can amp the
       group's CR in several ways: increasing the number of creatures
       in the group, adding templates, adding magical items, etc. We
       use the following loop to do that. */
    if (GType == MG_POPCORN && ! isHomog) {
      i = MaxGroupSize(&mg,xCR);
      if (i < 3)
        return NULL;
      mg.mCount = max(1,min(des_mCount - TotalMCount(),i));
      }
    else if (GType == MG_SHAMAN) {
      mg.mCount = 1 + random(mGroups[0].mCount/3);
      if (!random(2))
        mg.Flags |= MGF_MAGIC;
      }
      

    while (GroupXCR(&mg) < xCR && random(GType == MG_SHAMAN ? 2 : 8)) 
      {
        switch(enMonID ? 0 : random(6)) {
          case 0: // Raise the EL by increasing mCount
          case 1:
            if (GType == MG_LEADER || GType == MG_ALLY)
              break;
            if (enParty || isHomog)
              break;
            if (TMON(mg.mID)->HasFlag(M_SOLITARY))
              break;
            if (enFlags & EN_SINGLE)
              break;
            if (enFlags & EN_ANYOPEN)
              if (max_mCount + mg.mCount >= max_mCount)
                break;
            mg.mCount += random((xCR / max(1,GroupXCR(&mg)/max(1,mg.mCount))) - mg.mCount);
           break;
          case 2: // Add magical gear. 
            if (!(mg.Flags & MGF_MAGIC) )
              if (!TMON(mg.mID)->HasFlag(M_MINDLESS) && 
                    !TMON(mg.mID)->HasFlag(M_NOHANDS) && 
                      mg.isMType(MA_SAPIENT))
                mg.Flags |= MGF_MAGIC;
           break;
          case 3: // Add a template to the creature or some of the creatures
          case 4: // to make them tougher. 
            uint16 tt;
            if (random(mg.tCount) || mg.tCount >= 12)
              break; 
            tt = TM_CLASS | TM_SKILL | TM_NATURE | TM_ATTR | TM_DESCENT;
            if (GType == MG_BASE && enPlanarOK)
              tt |= TM_PLANAR;
            if (!(enParty || xMType))
              tt |= TM_UNDEAD;
            xID = AppropriateTemplate(tt,CalcCR(xCR),&mg);
            if (!xID)
              break;
            mg.tID[mg.tCount] = xID;
            
            /* Here, include a check to make sure the new template does not violate
               either enMType or xMType. This is a lot of trouble mostly to ensure
               that zombie orcs aren't added to parties of normal orcs, and a request
               to populate a holy druidic grove doesn't give us animal skeletons. */            
            if (xMType && !mg.isMType(xMType))
              { mg.tID[mg.tCount] = 0; break; }
            if (enMType && !mg.isMType(enMType))
              { mg.tID[mg.tCount] = 0; break; }
            if (TTEM(xID)->TType & TM_UNDEAD) {
              if (GType != MG_BASE)
                if (!mGroups[0].isMType(MA_UNDEAD))
                  { mg.tID[mg.tCount] = 0; break; }
              enMType = MA_UNDEAD;
              }
              
            mg.tCount++;
           break;
        }
      }
    
    if ((random(100) < (enCR*15)))
      if (TMON(mg.mID)->HasFeat(SK_POISON_USE) || (!random(8) &&
           mg.isMType(MA_SAPIENT) && (!TMON(mg.mID)->HasFlag(M_NOHANDS))))
        mg.Flags |= MGF_POISON;

    /* Catch Skeletons/Zombies with Extra Magic Items */
    if (mg.Flags & MGF_MAGIC)
      if (!mg.isMType(MA_SAPIENT))
        mg.Flags &= ~MGF_MAGIC;

    ASSERT (mg.mID);
    
    Monster *mn; bool missing_temp;
    mn = new Monster(mg.mID);
    for (i=0;i!=mg.tCount;i++)
      mn->AddTemplate(mg.tID[i]);
    missing_temp=false;
    for (i=0;i!=mg.tCount;i++)
      if (!mn->HasEffStati(TEMPLATE,mg.tID[i]))
        missing_temp = true;
    delete mn;
    if (missing_temp && Tries < 30)
      goto BadTempRestart;
    
    return &mg;
  }

/* These are the rules for appropriate templates:

   1. Plants, lights, vortices and fungi never gain templates.
   2. 75% of the time we try first to assign a
        skill-increasing or power-increasing
        template appropriate to the creature's
        type first. These include:
        * Dire/Legendary for animals
        * Ancient for undead
        * Experienced/Veteran/Etc. for sapient humanoids
        * Large/Huge/Immense for oozes and jellies
        * (Other) Large/Huge/Primal for beasts
   3. Creatures with M_PROFESSION cannot recieve a
        TM_CLASS template, but are the only creatures
        who can recieve a TM_RACE template.
   4. Summoned creatures will never gain the undead
        or nature-changing templates.
   5. Freaky templates must be applied to all creatures
        in a group, or none. A normal grizzly bear will
        not bond with an aqueous or zombie bear.
   6. Mixed groups may get two tiers of power templates
        (i.e., a veteran goblin chieftan and his 6 skilled
         goblins).
   7. Outsiders and non-humanoid aberrations don't get 
        freaky templates.
   8. Oozes and jellies only get large / huge.
   9. Undead templates don't get added to things that 
        already have an undead template, or to things that
        have a template restricted to living things.
   10. Outsiders, oozes, jellies and undead don't get the
         TM_PLANAR (i.e., aqueous, celestial, axiomatic,
         etc.) templates.
   11. Templates should not induce alignment conflicts.
         Undead templates override the base monster type's
         alignment, but other templates must not conflict
         with alignment from either the base monster or
         any other templates.
   12. Attribute templates only get added to creatures
         that have a base of 8 or more in the affected 
         attribute.
   13. All humanoids whose CR is three or more levels
         below the depth, or other monsters whose CR is
         five or more below Depth, get the best of two
         random TM_SKILL templates assigned to them
         automatically. Underlings get this too, for
         their adjusted depth (i.e., underlings at DL 12
         should be CR 4, so they'll be elite goblins led
         by a paragon goblin.)
   14. Awakened template conveys eligability (and 50%
         chance to gain) TM_CLASS template.
   15. If one monster in a given encounter has a TM_PLANAR
         or TM_UNDEAD template, that privlidge should be
         extended to every monster in the encounter.
   16. Creatures never have both a TM_PLANAR template and
         a TM_UNDEAD template; this is largely handled by
         the Types those templates require.

Partial Template List:

  TM_RACE   TM_CLASS    TM_UNDEAD  TM_SKILL  TM_NATURE   TM_PLANAR
  Elven     Rogue       Zombie     Skilled   Karmic      Aqueous
  Dwarven   Sorceror    Skeleton   Experien. Cloaked     Stone
  Orcish    Skirmisher  Ghost      Veteran   Corrupted   Celestial
  Drow      Scorcher    Wraith     Elite     Gelatinous  Fiendish
  Gnomish   Warrior     Vampire    Paragon   Spellstit.  Anarchic
            Priesty     etc.                 Phase
            Blademaster                      Psionic
  TM_SKILL        TM_DESCENT                 Antimagical
  Dire            Half-Celestial             Titanic
  Legendary       Half-Fiend                 Awakened
  Ancient         Half-Dragon                Pale
  Antediluvian                               Nightbreed
                                             Pseudonatural

*/

rID AppropriateTemplate(uint16 Types, int16 maxCR, MonGroup *mg)
  {
    uint16 hasTypes = 0;
    int32 l1, l2, l3;
    TMonster *tm = TMON(mg->mID);
    int16 i,q,n,j, CR, gal; 
    EvReturn r;
    rID bestSkill, bestCR = -10;
    bool doSkill = false,
         doClass = false;  
    
    CR = tm->CR;
    gal = GroupAlign(mg);

    for (i=0;i!=6 && mg->tID[i];i++) {
      CR = TTEM(mg->tID[i])->CR.Adjust(CR);
      Types &= ~(TTEM(mg->tID[i])->TType);
      hasTypes |= TTEM(mg->tID[i])->TType;
      /* Don't add any other templates to zombies or skeletons, for
         believability reasons. (Maybe "ancient" or "spellstitched", 
         later? Even "dire"?) */
      if (TTEM(mg->tID[i])->HasFlag(TMF_BODY_ONLY))
        return 0;
      }
    
    if (!Types)
      return 0;

    if (!(Types & TM_UNDEAD))
      Types &= ~TM_PLANAR;
    if (!(Types & TM_PLANAR))
      Types &= ~TM_UNDEAD;

    if (hasTypes & TM_AGECAT)
      Types &= ~TM_SKILL;
    if (hasTypes & (TM_CHIEFTAN|TM_SHAMAN))
      Types &= ~TM_CLASS;
    if (hasTypes & TM_CLASS)
      Types &= ~(TM_CHIEFTAN|TM_SHAMAN);

    if (Types & TM_CLASS)
      if (enParty)
        doClass = true;
    if (Types & TM_SKILL)
      if (random(4) && !doClass)
        if (!TMON(mg->mID)->HasFlag(M_CHIEFTAN))
        doSkill = true; 
    
    Retry: 
    n = 0; 
    for (q=0;q!=1;q++) /* Only one Module at the moment */
      for(i=0;i!=theGame->Modules[q]->szTem;i++)
        {
          /* We never add a BODY_ONLY template (i.e., mindless
             undead) to something that already has one or more
             other templates, for sanity reasons. */
          if (theGame->Modules[q]->QTem[i].HasFlag(TMF_BODY_ONLY))
            if (mg->tID[0])
              continue;

          if (!(theGame->Modules[q]->QTem[i].TType & Types))
            continue;
          /* TM_UNDEAD templates not only add the MA_UNDEAD template,
             they remove all other types. Thus, an aqueous badger is
             both MA_OUTSIDER, MA_WATER and MA_ANIMAL, but a zombie
             black bear is *not* MA_ANIMAL, only MA_ZOMBIE. */
          if (hasTypes & TM_UNDEAD) {
            if (theGame->Modules[q]->QTem[i].ForMType != MA_UNDEAD)
              continue;
            if (AlignConflict(ResAlign(theGame->Modules[q]->TemplateID(i)),AL_EVIL))
              continue;                
            }
          else {
            if (theGame->Modules[q]->QTem[i].ForMType && !mg->isMType(theGame->Modules[q]->QTem[i].ForMType))
              continue;
            if (AlignConflict(ResAlign(theGame->Modules[q]->TemplateID(i)),gal))
              continue;
            }

          if (theGame->Modules[q]->QTem[i].TType & TM_SKILL) {
            /* Tiny humanoids have low CRs, like -4 or so. Avoid applying
               skill templates to them at DLev 1. i.e., 4 goblins is a good
               DLev 1 encounter, but a single skilled goblin is not. To
               avoid this, try the template against a hypothetical CR 1
               creature and discard it if the result is over max CR. */            
            if (theGame->Modules[q]->QTem[i].CR.Adjust(1) >= maxCR)
              continue;
            if (theGame->Modules[q]->QTem[i].CR.Adjust(tm->CR) <= maxCR)
              if (theGame->Modules[q]->QTem[i].CR.Adjust(tm->CR) > bestCR)
                if (mg->VerifyTemplate(theGame->Modules[q]->TemplateID(i)))
                  {
                    bestCR = theGame->Modules[q]->QTem[i].CR.Adjust(tm->CR);
                    bestSkill = theGame->Modules[q]->TemplateID(i);
                  }
            }
            

          if (doClass && !(theGame->Modules[q]->QTem[i].TType & TM_CLASS))
            continue;
                  
          if (theGame->Modules[q]->QTem[i].HasFlag(TMF_NOGEN))
            continue;
          if (theGame->Modules[q]->QTem[i].CR.Adjust(CalcCR(GroupXCR(mg))) > min(maxCR,enCR))
            continue;

          if (theGame->Modules[q]->QTem[i].TType & TM_ATTR)
            for (j=0;j!=7;j++)
              if (theGame->Modules[q]->QTem[i].Attr[j].Adjust(10) > 10)
                if (tm->Attr[j] < 8)
                  continue;
          Candidates[n++] = theGame->Modules[q]->TemplateID(i);
        }
    Candidates[n] = 0;
    
    if (doSkill)
      if (bestCR != -10)
        return bestSkill;
    if (!n && doClass)
      {
        doClass = 0;
        goto Retry;
      }
    
    if (!n)
      return 0;

    do {
      i = random(n);
      if (!mg->VerifyTemplate(Candidates[i])) {
        memmove(&Candidates[i],&Candidates[i+1],(2048 - (i+1))*sizeof(rID));
        n--;
      }
      else
        break;
    }
    while (n);
     
    if (!n)
      return 0;


    return Candidates[i];
  }    

bool AlignConflict(uint16 al1, uint16 al2)
  {
    if ((al1|al2) & AL_GOOD)
      if ((al1|al2) & (AL_EVIL|AL_NONGOOD))
        return true;
    if ((al1|al2) & AL_EVIL)
      if ((al1|al2) & (AL_GOOD|AL_NONEVIL))
        return true;
    if ((al1|al2) & AL_LAWFUL)
      if ((al1|al2) & (AL_CHAOTIC|AL_NONLAWFUL))
        return true;
    if ((al1|al2) & AL_CHAOTIC)
      if ((al1|al2) & (AL_LAWFUL|AL_NONCHAOTIC))
        return true;
    return false;
  }

/* This is slow and very, very kludgy, but it does seem to be
   the most correct way to do this without a lot of excess
   code. I doubt there will ever be enough templates for this
   function to be a real speed bottleneck. */

Monster *isMType_mn = NULL; 


bool MonGroup::isMType(int32 MType)
  {
    int16 i; 
    if (!isMType_mn) {
      isMType_mn = new Monster(mID);
      }
    else {
      if (!theRegistry->Exists(isMType_mn->myHandle)) {
        delete isMType_mn;
        isMType_mn = new Monster(mID);
        }
      isMType_mn->tmID = mID;
      isMType_mn->mID = mID;
      }
    
    /* Creature::isMType uses Int to figure out whether a
       creature is MA_SAPIENT or not. Similarly for Size
       and MA_PERSON. */

    isMType_mn->Attr[A_INT] = TMON(mID)->Attr[A_INT];
    isMType_mn->Attr[A_SIZ] = TMON(mID)->Size;

    isMType_mn->RemoveStati(TEMPLATE);
    for(i=0;i!=tCount;i++) 
      isMType_mn->AddTemplate(tID[i]);

    if (MType < 0)
      return (TTEM(-MType)->PEvent(EV_ISTARGET,isMType_mn,-MType) != -1);
    else
      return isMType_mn->isMType(MType);
  }

uint16 ResAlign(rID xID)
  {
    uint16 al = 0;
    switch (RES(xID)->Type)
      {
        case T_TMONSTER:
          if (TMON(xID)->HasFlag(M_GOOD))
            al |= AL_GOOD;
          if (TMON(xID)->HasFlag(M_EVIL))
            al |= AL_EVIL;
          if (TMON(xID)->HasFlag(M_LAWFUL))
            al |= AL_LAWFUL;
          if (TMON(xID)->HasFlag(M_CHAOTIC))
            al |= AL_CHAOTIC;
         break;
        case T_TTEMPLATE:
          if (TTEM(xID)->TType & TM_UNDEAD)
            al = AL_NONGOOD;
          if (TTEM(xID)->AddFlags[M_GOOD/8] & XBIT(M_GOOD % 8))
            al |= AL_GOOD;
          if (TTEM(xID)->AddFlags[M_EVIL/8] & XBIT(M_EVIL % 8))
            al |= AL_EVIL;
          if (TTEM(xID)->AddFlags[M_LAWFUL/8] & XBIT(M_LAWFUL % 8))
            al |= AL_LAWFUL;
          if (TTEM(xID)->AddFlags[M_CHAOTIC/8] & XBIT(M_CHAOTIC % 8))
            al |= AL_CHAOTIC;
         break;
        default:
          Error("Strange resource sent to ResAlign!");
         break;
      }
    return al;
  }


/* List 20 Encounters by depth, ala objects */
/* Turn GenEncounter variables into globals fpr AppropriateTemplate et. al. */
/* Add Mount support to GenEncounter; worgs for goblins;  */



int16 MaxItemPlus(int16 MaxLev,rID eID)
  {
    int16 i = 2, l = TEFF(eID)->Level;
    
    if (TEFF(eID)->HasSource(AI_WAND))
      MaxLev += 2;
    
    while (i != 6 && LevelAdjust(l,0,i) <= MaxLev)
      i++;

    return max(1,i - 1);
  }

/* ww: currently we generate really horrible random-looking magical weapons
 * and armor ... let's see what we can do do improve that */

static int16 NumQualities(int plusLevel)
{
  int16 numQual;
  // first, decide how many qualities we're going to have
  int16 i = random(100);
  if (plusLevel <= 0) 
    numQual = 0;
  else if (i > 45 + plusLevel * 5)
    numQual = 0;
  else if (plusLevel > 2 && i < plusLevel * 3)
    numQual = 2;
  else if (plusLevel > 4 && i < plusLevel * 5)
    numQual = 3;
  else if (plusLevel > 6 && i < plusLevel * 6)
    numQual = 4;
  else 
    numQual = 1;
  return numQual; 
}

// this procedure gives It some combination of true plusses and weapon
// qualities that equal exactly plusLevel
static void MakeMagicWeaponArmor(Item *it, int _plusLevel, 
      int16 QualityMods[][2], int reserve)
{
  int numQual;
  int i,j;
  int table[2048];
  int max;
  int plusLevel = _plusLevel;
  if (plusLevel <= 0) return; 

  plusLevel -= reserve; 

  numQual = NumQualities(plusLevel);
  
  while (numQual > 0) {
    // now consturct a weighted table: a weapon quality worth 3 is 3 times
    // more common than a weapon quality worth 1 *if you can afford it*,
    // basically
    max = 0;
    for (i=0; QualityMods[i][0] != -1; i++) {
      int qual = QualityMods[i][0];
      int cost = QualityMods[i][1];
      if (plusLevel >= cost && it->QualityOK(qual)) {
        if (qual == WQ_MIGHTY || qual == WQ_BURST) cost += 20; 
        for (j=0; j < cost + 1 ; j++)  {
          table[max++] = qual; 
        }
      }
    } 
    if (max == 0) break; // paranoia
    ASSERT(max < 2048);

    int choice = random(max);
    int qual = table[choice]; 
    if (it->QualityOK(qual))
      it->AddQuality(qual);
    for (i=0; QualityMods[i][0] != -1; i++) 
      if (QualityMods[i][0] == qual)
        { plusLevel -= QualityMods[i][1]; break; }
    numQual--;
    } 

  plusLevel += reserve; 

  ASSERT(plusLevel >= 0); 

  if (plusLevel > 0) 
    it->MakeMagical(0, max(1,min(5,plusLevel)));

  return; 
}

Item* Item::GenItem(uint16 Flags, rID xID, int16 Depth, int8 Luck, ItemGen *Gen)
  {
    int16 i,j,k, minlev, maxlev, adjlev, restart_count, roll, IType = 0;
    uint32 mag; bool proto = false; String s;
    int16 magicLevel = 0;
    bool trulyCursed = false; /* generates EF_CURSED eID */
    bool isCursed = false; /* generates IF_CURSED item */
    rID iID, eID, aID; 
    Item* theItem;
    int16 qv = 0;

    /* Explanation here: high Luck occasionally turns good items
       into very good (i.e., out of depth) items. Logically, this
       would mean that low luck occasionally turns medium-level
       items into very low level items -- but that's very little
       effect on gameplay to be the primary impact of a very low
       stat. So, in addition, very low Luck will also reduce the
       maximum level of 80% of all items created by it's attribute
       modifier -- effectively, 80% of the treasure a character
       with Luck 3 finds on DL10 will be typical of DL6. The other
       20% will be normal DL10 treasure. */   
    //maxlev = ((Depth + 1)* 2) / 3;
    maxlev = Depth;
    
    /* New idea -- generate treasure for dlev 1-3 all as if they
       were dlev 3. My rationale for this is as follows:
       * Compared to, say, Angband or even NetHack, Incursion has
         a VERY short timeframe for getting "needed" equipment.
         The player can never scum for gear or hang out on a level
         to get more treasure -- everything that will be generated
         is genned when the level is. There needs to be more "chances"
         to get certain staple equipment before the midgame.
       * The character NEEDS equipment the most at the beginning,
         and it doesn't come fast enough.
       * I'm finding Wes' comments about there being "too much junk"
         in Incursion very, very true -- a million +1 minor bonus
         items are just irritating, while a few mixed in with better
         stuff are quirky.
       * This should strongly curtail the "every helm on dlev 1 is a
         helm of sound resistance" issue, because item level 3 has a
         LOT more variety they ILev 1.
       Wes, let me know if this is making characters way too powerful
       too early, or if it's even noticable, or what.
    */
    maxlev = max(3,Depth);
    
    if (random(5) && Luck <= 9)
      { maxlev += (Luck-11)/2;
        maxlev = max(min(1,Depth),maxlev); }
        
    /* For high-luck characters, 20% of items are adjusted by luck
       -- their maxlev is increased by 1d[LuckMod]. When generating
       specific weapons, this always happens. */    
    adjlev = maxlev + (Luck <= 9 ? 0 : random((Luck-9)/2));
    if (!random(5))
      maxlev = adjlev; 
        
    /* The old algorithm -- less dramatic luck effects.      
      
      while (((Luck*3) > (random(100)+1)) && maxlev < Depth)
        maxlev++;
        
    */

    /* Later, the chance of generating an artifact will be based on the
       IG_GOOD and IG_GREAT flags as well, decreasing exponentially with
       the number of artifacts already generated. */
    if (Flags & IG_GREAT)
      minlev = max(0,maxlev-2);
    else if (Flags & IG_GOOD)
      minlev = max(0,maxlev-4);
    else {
      if (xID && RES(xID)->Type == T_TDUNGEON)
        if (random(100)+1 < TDUN(xID)->GetConst(CURSED_CHANCE)) {
          isCursed = true;
        } else if (random(100)+1 < TDUN(xID)->GetConst(CURSED_CHANCE)) {
          isCursed = true;
          trulyCursed = true; 
        }
      minlev = 0;
    }
      

    
    maxlev = max(maxlev, 1);
    Depth  = max(Depth, 1);
    minlev = max(maxlev - 4,0);
    restart_count = 0;
    /* To prevent the "1000 restarts" message, minlev MUST be
       zero, because most staple items are very low level! */

    if (Flags & IG_STAPLE)
      minlev = 0;


    if (xID && RES(xID)->Type == T_TITEM)
      { iID = xID; mag = 0;
        IType = TITEM(iID)->IType;
        for (i=0;DungeonItems[i].Prob;i++)
          // ww: was 'type', but I'm pretty sure IType is what we want here
          if (IType == DungeonItems[i].Type)
            mag = DungeonItems[i].Source;
        goto Got_iID;
      }
    
    Restart:
    if (restart_count++ > 1000)
      {
        Error("1000 restarts on the Item Generator?!");
        theItem = Item::Create(FIND("food ration"));
        return theItem;
      }
    roll = 0;

    for(i=0;Gen[i].Prob;i++)
      roll += Gen[i].Prob;

    roll = random(roll);
    
    for(i=0;Gen[i].Prob;i++)
      if (roll < Gen[i].Prob)
        break;
      else
        roll -= Gen[i].Prob;

    if (!Gen[i].Prob)
      Fatal("Damaged item probability chart!");
    IType = Gen[i].Type;
    
    RetryCoinType:
    
    if (Gen[i].Prototype)
      { iID = FIND(Gen[i].Prototype); proto = true; }
    else if (Flags & IG_STAPLE)
      iID = theGame->GetItemID(PUR_STAPLE,0,maxlev,Gen[i].Type);
    else
      iID = theGame->GetItemID(PUR_DUNGEON,0,maxlev,Gen[i].Type);
    mag   = Gen[i].Source;

    if (!iID)
      goto Restart;

    // no matter how lucky you are, you won't find a Depth 2 item at 10'
    if (TITEM(iID)->Depth > Depth) 
      goto Restart;
    
    qv++;
    
    Got_iID:

    theItem = Item::Create(iID);
    if (theItem->isType(T_COIN))
      {
        int32 val;
        val = WealthByLevel[max(0,min(20,Depth))] / 4;
        val *= 50 + random(100); // 50% to 150%
        val /= TITEM(iID)->Cost;
        theItem->Quantity = val;
        /* Don't generate 1,000,000 cp on dlev 10! */
        int32 maxCoins[] = {25000, 10000, 5000, 2500} ;
        if (theItem->Quantity > maxCoins[random(4)])
          {
            delete theItem;
            goto RetryCoinType;
          }
      }
    else if (TITEM(iID)->HasFlag(IT_BGROUP))
      theItem->Quantity = 40 + random(41);
    else if (TITEM(iID)->HasFlag(IT_GROUP))
      theItem->Quantity = 1 + random(4);
    else if ((Flags & IG_CHEST) && (TITEM(iID)->Type == T_POTION ||
                TITEM(iID)->Type == T_SCROLL))
      theItem->Quantity = 1 + random(4);

    eID = 0;  

    if (!maxlev)
      goto SkipMagic;

    {
      EventInfo xe;
      xe.Clear();
      xe.ETarget = theItem;
      xe.EItem   = theItem;
      xe.vDepth  = Depth;
      xe.vLevel  = maxlev;
      xe.Event   = EV_GENITEM;
      switch (TITEM(iID)->Event(xe,iID))
        {
          case DONE: goto DoneItem;
          case ABORT: delete theItem; return NULL;
          case ERROR: Error("EV_GENITEM returned ERROR!");
          case NOTHING: break;
        }
    }

    if ((IType == T_WEAPON || IType == T_BOW || IType == T_MISSILE || 
         IType == T_ARMOR || IType == T_SHIELD) && !(Flags & IG_STAPLE)) {
      
      /* Generate a fully mundane item? */
      if (!random(maxlev+2))
        goto SkipMagic;
        
      /* 5% of magic weapons/armor are specific weapons (i.e.,
         Sunsword, Frost Brand) as opposed to randomly generated
         weapons (+2 keen pickaxe, +1 ghost touch rapier) -- but
         only at depth 6+! */ 
      if (Depth > 5 && !random(8))
        goto GrantEffect;
        
      NoSpecificWeaponFound:
      int isArmor = IType == T_ARMOR || IType == T_SHIELD;
      int isShield = IType == T_SHIELD;
      magicLevel = max(random(1 + maxlev - minlev) + minlev,
                       random(1 + maxlev - minlev) + minlev);
      //magicLevel += random(4) + random(4) - 3; // fake std dev ...
      //magicLevel *= 100;
      //magicLevel /= 150;

      if (magicLevel >= 0) { 
        MakeMagicWeaponArmor(theItem, magicLevel,
            isArmor ? AQualityMods : QualityMods,0);
      } 
    } 
    else if (mag)
      {
        GrantEffect:
        if (trulyCursed) 
          eID = theGame->GetEffectID(PUR_CURSED,minlev,maxlev,mag);
        else if (Flags & IG_STAPLE)
          eID = theGame->GetEffectID(PUR_STAPLE,minlev,maxlev,mag);
        else if (mag == AI_WEAPON || mag == AI_ARMOR) 
          /* Specific Weapons ALWAYS have maxlev adjusted by luck. */
          eID = theGame->GetEffectID(PUR_DUNGEON,minlev,adjlev,mag);
        else
          eID = theGame->GetEffectID(PUR_DUNGEON,minlev,maxlev,mag);
        
        if (mag == AI_ARMOR)
          if (eID && !TEFF(eID)->HasSource(AI_ARMOR))
            Error("Bad armor effect bug!");
            
        if (!eID)
          {
            if (mag == AI_WEAPON || mag == AI_ARMOR ||
                mag == AI_BOW || mag == AI_SHIELD)
              {
                /*
                if (theGame->GetPlayer(0)->WizardMode)
                  theGame->GetPlayer(0)->IPrint(
                    "No specific found (mag <str>, minlev <num>, maxlev <num>, adjlev <num>).",
                    Lookup(AI_CONSTNAMES,mag),minlev,maxlev,adjlev);
                */
                goto NoSpecificWeaponFound;
              }
              
            delete theItem;
            if (xID && RES(xID)->Type == T_TITEM)
              return NULL;
            goto Restart;
          }
        if (eID && TEFF(eID)->HasFlag(EF_NEEDS_PLUS))
          theItem->MakeMagical(eID,MaxItemPlus(Depth,eID));
      }

    if (eID > 0)      
      theItem->MakeMagical(eID,theItem->Plus);
      
    if (theItem->eID || theItem->Plus)
      if (random(theItem->eID ? 150 : 100) < (xID ? TDUN(xID)->GetConst(CURSED_CHANCE) : 25))
        if (!(theItem->isType(T_POTION) || theItem->isType(T_SCROLL) || theItem->isType(T_WAND) ||
                theItem->isType(T_COIN) || theItem->isType(T_TOOL) || theItem->isType(T_TOME)))
          theItem->IFlags |= IF_CURSED;

    if (isCursed) 
      theItem->IFlags |= IF_CURSED;

    if (theItem->Plus && theItem->eID == 0 && (theItem->isType(T_WEAPON) ||
         theItem->isType(T_ARMOR) || theItem->isType(T_SHIELD) ||
         theItem->isType(T_BOW) || theItem->isType(T_MISSILE)))
      if (!random(4))
        theItem->IFlags |= IF_RUNIC;

    SkipMagic:

    if (theItem->isType(T_BOOK) || theItem->isType(T_COIN) ||
        theItem->isType(T_TOOL) || theItem->isType(T_FOOD))
      theItem->IFlags &= ~(IF_CURSED|IF_BLESSED);
      
    if ((theItem->isType(T_POTION) || theItem->isType(T_WAND) ||
           theItem->isType(T_SCROLL)) && !trulyCursed)
      theItem->IFlags &= ~(IF_CURSED|IF_BLESSED);    
      
    if (!theItem->isMagic())  
      theItem->IFlags &= ~(IF_CURSED|IF_BLESSED);    

    if (theItem)
      if (!random(3)) {
        switch(random(8)) {
          case 0:
            if (theItem->QualityOK(IQ_ELVEN))
              theItem->AddQuality(IQ_ELVEN);
           break;
          case 1:
            if (theItem->QualityOK(IQ_DWARVEN))
              theItem->AddQuality(IQ_DWARVEN);
           break;
          case 2:
          case 3:
            if (theItem->QualityOK(IQ_ORCISH))
              theItem->AddQuality(IQ_ORCISH);
           break;
          case 4:
            if (theItem->QualityOK(IQ_ADAMANT))
              theItem->AddQuality(IQ_ADAMANT);
            else if (theItem->QualityOK(IQ_DARKWOOD))
              theItem->AddQuality(IQ_DARKWOOD);
           break;
          case 5:
            if (theItem->QualityOK(IQ_MITHRIL))
              theItem->AddQuality(IQ_MITHRIL);
            else if (theItem->QualityOK(IQ_DARKWOOD))
              theItem->AddQuality(IQ_DARKWOOD);
           break;
          case 6:
            if (theItem->QualityOK(IQ_IRONWOOD))
              theItem->AddQuality(IQ_IRONWOOD);
           break;
          case 7:
            if (theItem->QualityOK(IQ_SILVER))
              theItem->AddQuality(IQ_SILVER);
           break;
          }
        }  

    if (Flags & IG_KNOWN) {
      theItem->Known = 0xff; 
      if (theItem->isQItem())
        ((QItem *)theItem)->KnownQualities = 0xff; 
    }

    if (random(100) < max(maxlev,5) && !theItem->isType(T_TOOL) &&
         !theItem->isType(T_COIN)) {
      theItem->IFlags &= ~IF_CURSED;
      theItem->IFlags |= IF_BLESSED;
    }

    if (theItem->isType(T_POTION) || theItem->isType(T_SCROLL))
      { s = theItem->Name(NA_IDENT);
        ASSERT(!theItem->isQItem()); }
    
    DoneItem:
    
    theItem->GenStats = Format("[ minlev = %d, maxlev = %d, magicLevel = %d ]",
      minlev, maxlev, magicLevel);
    
    theItem->Initialize();

    /*
    if (theItem->isType(T_WEAPON) ||
        theItem->isType(T_ARMOR) ||
        theItem->isType(T_BOW) ||
        theItem->isType(T_SHIELD))
      if (theItem->eID && theGame->GetPlayer(0)->WizardMode)
        theGame->GetPlayer(0)->IPrint("Generated <Res> (<num>/<num>).",
          theItem->eID, minlev, adjlev);
    */
    
    return theItem;

  }

