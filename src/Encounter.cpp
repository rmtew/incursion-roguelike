/* ENCOUNTER.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Contains the code for generating groups of monsters or random
     items within a given set of specifications; this is used by 
     both summoning effects and the dungeon generator.

     
*/
#include "Incursion.h"

/* TODO:

  HIGH:
  -- make "Ancient Barracks" monsters more thematic
  -- Encounters for:
       pseudonatural
       clockwork
       dire animals
       cave animals
       pseudonaturals
       eye creatures
       band of mephits
       dryad/nymph, with animals (add to forest room)
       animal family, with baby template
       
       Multitude Cult encounter:
       2-6 Multitude cultists
       50% 1 blackguard 
       30% 1 dark sorceror
       30% 1 lone vampire
       1   wraith or spectre with "demon ghost" template
       2d4 low-level demons
       possibly special themed servitor monsters 
       
       Lone Multitude Cultist:
       1 Multitude cultist
       1-3 demon followers
       
  -- Rename generic encounters --
       A/M/Si/H Any/Multiple/Single/Horde
       U/D   Uniform/Diverse
       E/Sk/C/F Exact/Skilled/Classed/Freaky
       Aligned - script to copy enConstraint to enAlign
       
       "Generic SUE"
       "Generic MDSC"
  -- MINION / priests
     -- M_MINION for templates that need a god
     -- ALLOWED_GODS list in TMonster / TTemplate
     -- Choose god based on alignment in enGenAlign
     -- Monster::Name returns "lizardfolk priest of Immotian"
     -- MonsterGear gives creatures with hands and M_MINION
        a holy symbol.
        
  MODERATE:
  -- enSleep
  -- poisoned weapons
  -- look at probability dump
  -- Formation, Starting Orders
  -- death knights with living mounts
  -- FREAKY_LIST, add to more encounters
     with more specific limits, and with
     "freak factors" listed in the weighted
     list for rarity.
  -- add higher age categories
  -- add higher skill templates
  -- statue garden == basilisk, cockatrice or
                        medusa, or dwarves
  -- call FindOpenAreas in script in fire/ice
       prison regions to make sure encounter
       ends up INSIDE the prison -- call it in
       PRE(EV_ENGEN), and also increase CR?
  -- special encounter -- mixed elements? --
       for elemental chamber, with high
       MIN_XCR_MULT
  -- Templates:
       "of Legend"
       gelatinous
       insectile
       runecarved (for constructs, like spellstitched)
       psionic
       karmic
       ethereal
       titanic
  -- Magma streamers get a special encounter that searches
       for fire-immune monsters, esp. magmin and magma creepers
  -- CR 1 ice room encounters:
     -- adapt kruthic as arctic beast
     -- cave husky
     -- ice beetle
     -- ice mephit
     -- ice paraelementals
          (standardize paras in general)
     -- hatchling crystal spider / crysmal
     -- later, mote of frost
     -- wights, skeleton
     -- snow angel -- feminine form made of wind-swept snow,
                        low level celestial
     -- arctic lemming
      
  
   -- verify manditory templates can go on chosen mID --
      we still get errors here, frex 'bison'
   * In Holy Summons encounter, trap PRE(EV_ENGEN_PART) and
       set e.cPart based on CR, and set e.enAlign based on
       EActor. Grant in NF_STAGGERED, but the event overrides
       the random choice, different parts for celestial animals,
       fiendish animals, devils, celestials, demons, etc.
       Now we can do different summons for different gods,
       too!
     In PRE(EV_ENGEN), redirect the summons to an aquatic
       equivalent if e.EXVal,e.EYVal is aquatic, or add a
       flag, NF_CHECK_WATER, to do this automatically.
   -- NF_VAULT, EN_VAULT
   * enSleep
   * M_RARE, M_COMMON, etc. as multipliers to ChooseMID weight,
       but only when choosing from all mIDs, not specified ones
   * Fix duplicate monsters caused by ANY(MA_TYPE) in weighted
       lists.
   * getMaxCR and getMinCR should also calculate max and min
       TEMPLATE CR, which will then be taken into account by
       the later "real" call to enChooseMID().
   * Monster::PseudoCalcValues();
   
   * BuildMon  
     -- ENCOUNTER stati
     -- NF_SHOWDESC
     -- illusions, summons, etc.
   * Purify for Memory Corruption
   * Fix EP_ELSE known issues for multi-else
   * Iterate 3-5 times, save results and choose best if
       finished encounter XCR exceeds specified XCR by
       more then ALLOWED_DEVIANCE
   * When choosing mID, figure out the CR adjust of the
       required/specified templates, and take it into
       account in the maximum mID CR allowed.
   * Recalc part XCR by taking total desired XCR, subtracting
       the XCR what was ACTUALLY created to date, calculating
       total REM  AINING weight dividing remaining XCR by the
       fraction of this part's weight from the REMAINING total
       weight. 
   * Confirm that required templates can be applied to all
       chosen mIDs -- if they can't, discard the mID, so that
       we don't get, frex, living things in a zombie horde.
   * Optimizations
       -- generate the mID weighted list once and
          then store it, so that we don't repeat the gen (and the
          EV_CRITERIA call) 10+ times in getMinCR()
       -- separate EV_CRITERIA from EV_TCRITERIA, and set a bool
          value outside the loop for whether a given encounter
          uses either.
   * Encounter Name MSG_ENCNAME == "pack of <res1:plural>"
   * Generate all the fixed-Amt parts of an Encounter first, then
       calculate remaining XCR, allocate by weight and do the
       variable amount parts.
       
   ENCOUNTERS & TERRAIN
     When we do EncStats, add a second part to try each encounter
   100 times and check the terrain of the mIDs that result. Then,
     (1) Make a list of Terrain Types the EncType can provide
         encounters for.
     (2) Calculate the percent of total used mIDs that are of the
         needed terrain type, and use this as a multiplier for the
         encounter's terrain type.
     (3) Call the encounter for each terrain type and see which
         ones it fails at.
   Based on this, build a .h autogen static table listing the name
   of each encounter, the relevant terrains, a 0 in rID and the
   percent of its normal weight to apply in an encounter which
   uses the terrain type in question. Leave the xID column empty,
   and then fill it out on initialization of modules by calling
   FIND(name) for each encounter ahead of time as a form of
   caching.
   THE DOWNSIDE of this is that is goes against the "plug in new
   modules at design time" philospohy - think about this.    
   
       
*/

int32 Map::uniformKey[50];
rID   Map::uniformChoice[50];
int16 Map::cUniform;
EncMember Map::EncMem[MAX_ENC_MEMBERS];
int16 Map::cEncMem; 

Creature * CandidateCreatures[2048]; 
void UpdateAlignRestrict(EventInfo &e);


#define PTEMP_MOUNTED 10001
#define PTEMP_POISON  10002
#define PTEMP_MAGIC   10003

/* Throw Functions */
EvReturn Map::thEnGen(rID xID, uint32 fl, int8 CR, uint16 enAlign)
  {
    EventInfo e;
    e.Clear();
    e.EMap = this;
    if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enID = xID;
    else if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enRegID = xID;
    e.enFlags = fl;
    e.enCR = CR;
    e.enDepth = Depth;
    e.enAlign = enAlign;
    return ReThrow(EV_ENGEN,e);
  }

EvReturn Map::thEnGenXY(rID xID, uint32 fl, int8 CR, uint16 enAlign, int16 x, int16 y)
  {
    EventInfo e;
    e.Clear();
    e.EMap = this;
    if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enID = xID;
    else if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enRegID = xID;
    e.enFlags = fl;
    e.enCR = CR;
    e.enDepth = Depth;
    e.enAlign = enAlign;
    e.EXVal = x;
    e.EYVal = y;
    e.isLoc = true;
    return ReThrow(EV_ENGEN,e);
  }
  
EvReturn Map::thEnGenSummXY(rID xID, uint32 fl, int8 CR, uint16 enAlign, Creature* crea, int16 x, int16 y)
  {
    EventInfo e;
    e.Clear();
    e.EMap = this;
    if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enID = xID;
    else if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enRegID = xID;
    e.enFlags = fl;
    e.enCR = CR;
    e.enDepth = Depth;
    e.enAlign = enAlign;
    e.EXVal = x;
    e.EYVal = y;
    e.EActor = crea;
    e.isLoc = true;
    return ReThrow(EV_ENGEN,e);
  }

EvReturn Map::thEnGenMon(rID xID, rID mID, uint32 fl, int8 CR, uint16 enAlign)
  {
    EventInfo e;
    e.Clear();
    e.EMap = this;
    if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enID = xID;
    else if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enRegID = xID;
    e.enConstraint = mID;
    e.enFlags = fl;
    e.enCR = CR;
    e.enDepth = Depth;
    e.enAlign = enAlign;
    return ReThrow(EV_ENGEN,e);
  }


EvReturn Map::thEnGenMonXY(rID xID, rID mID, uint32 fl, int8 CR, uint16 enAlign, int16 x, int16 y)
  {
    EventInfo e;
    e.Clear();
    e.EMap = this;
    if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enID = xID;
    else if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enRegID = xID;
    e.enFlags = fl;
    e.enCR = CR;
    e.enDepth = Depth;
    e.enAlign = enAlign;
    e.EXVal = x;
    e.EYVal = y;
    e.enConstraint = mID;
    e.isLoc = true;
    return ReThrow(EV_ENGEN,e);
  }

EvReturn Map::thEnGenMonSummXY(rID xID, rID mID, uint32 fl, int8 CR, uint16 enAlign, Creature *crea, int16 x, int16 y)
  {
    EventInfo e;
    e.Clear();
    e.EMap = this;
    if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enID = xID;
    else if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enRegID = xID;
    e.enFlags = fl;
    e.enCR = CR;
    e.enDepth = Depth;
    e.enAlign = enAlign;
    e.EXVal = x;
    e.EYVal = y;
    e.enConstraint = mID;
    e.isLoc = true;
    e.EActor = crea;
    return ReThrow(EV_ENGEN,e);
  }


EvReturn Map::thEnGenMType(rID xID, int16 mt, uint32 fl, int8 CR, uint16 enAlign)
  {
    EventInfo e;
    e.Clear();
    e.EMap = this;
    if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enID = xID;
    else if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enRegID = xID;
    e.enConstraint = mt;
    e.enFlags = fl;
    e.enCR = CR;
    e.enDepth = Depth;
    e.enAlign = enAlign;
    return ReThrow(EV_ENGEN,e);
  }

EvReturn Map::thEnGenMTypeXY(rID xID, int16 mt, uint32 fl, int8 CR, uint16 enAlign, int16 x, int16 y)
  {
    EventInfo e;
    e.Clear();
    e.EMap = this;
    if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enID = xID;
    else if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enRegID = xID;
    e.enFlags = fl;
    e.enCR = CR;
    e.enDepth = Depth;
    e.enAlign = enAlign;
    e.EXVal = x;
    e.EYVal = y;
    e.enConstraint = mt;
    e.isLoc = true;
    return ReThrow(EV_ENGEN,e);
  }

EvReturn Map::rtEnGen(EventInfo &_e, rID xID, uint32 fl, int8 CR, uint16 enAlign)
  {
    EventInfo e;
    e.Clear();
    e = _e;
    e.EMap = this;
    if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enID = xID;
    else if (xID && RES(xID)->isType(T_TENCOUNTER))
      e.enRegID = xID;
    e.enFlags = fl;
    e.enCR = CR;
    e.enDepth = Depth;
    e.enAlign = enAlign;
    return ReThrow(EV_ENGEN,e);
  }


Creature * Map::GetEncounterCreature(int32 i)
  {
    if (i < 0 || i >= (sizeof(CandidateCreatures)/sizeof(CandidateCreatures[0]))) 
      return NULL;
    else 
      return (CandidateCreatures[i]);
  }

void Map::enWarn(const char* msg,...)
  {
    va_list ap;
    if (!theGame->GetPlayer(0))
      {
        Player *p = new Player(T1);
        va_start(ap, msg);
        T1->Box(__XPrint(p,msg,ap));
		    va_end(ap);
		    delete p;
		  }
		else if (1)//theGame->GetPlayer(0)->WizardMode)
		  {
		    va_start(ap, msg);
        T1->Box(__XPrint(theGame->GetPlayer(0),msg,ap));
		    va_end(ap);
      }
  }

void Map::enUniformAdd(int32 key, rID choice)
  {
    uniformKey[cUniform] = key;
    uniformChoice[cUniform] = choice;
    cUniform++;
    ASSERT(cUniform < 50);
  }

rID Map::enUniformGet(int32 key)
  {
    for (int32 i=0;i!=cUniform;i++)
      if (uniformKey[i] == key)
        return uniformChoice[i];
    return 0;
  }

void Map::enAddMon(EventInfo &e)
  {
    ASSERT(cEncMem < MAX_ENC_MEMBERS);
    EncMem[cEncMem].mID   = e.ep_mID;
    EncMem[cEncMem].tID   = e.ep_tID;
    EncMem[cEncMem].tID2  = e.ep_tID2;
    EncMem[cEncMem].tID3  = e.ep_tID3;
    EncMem[cEncMem].hmID  = e.ep_hmID;
    EncMem[cEncMem].htID  = e.ep_htID;
    EncMem[cEncMem].htID2 = e.ep_htID2;
    EncMem[cEncMem].pID   = e.ep_pID;
    EncMem[cEncMem].iID   = e.ep_iID;
    EncMem[cEncMem].Part =  e.cPart;
    cEncMem++;
  }

/*7398

  9594
  9558
  9571
  9560

*/

void Map::enAddTemp(EventInfo &e, rID tID)
  {
    ASSERT(TTEM(tID)->Type == T_TTEMPLATE);
    if (e.ep_tID && e.ep_tID2 && e.ep_tID3)
      {
        enWarn("Monster generated with 4+ templates!");
        return;
      }
    if (e.ep_tID && e.ep_tID2)
      e.ep_tID3 = tID;
    else if (e.ep_tID)
      e.ep_tID2 = tID;
    else
      e.ep_tID = tID;
    enCalcCurrPartXCR(e);
  }

void Map::enAddMountTemp(EventInfo &e, rID tID)
  {
    ASSERT(TTEM(tID)->Type == T_TTEMPLATE);
    if (e.ep_htID && e.ep_htID2)
      {
        enWarn("Mount generated with 3+ templates!");
        return;
      }
    if (e.ep_htID)
      e.ep_htID2 = tID;
    else
      e.ep_htID = tID;
    enCalcCurrPartXCR(e);
  }

void Map::enCalcCurrPartXCR(EventInfo &e)
  {
    e.ep_monCR = 0;
    e.ep_mountCR = 0;
    if (!e.ep_mID)
      { e.epCurrXCR = 0; return; }
    e.ep_monCR = TMON(e.ep_mID)->CR;
    if (e.ep_tID)
      e.ep_monCR = TTEM(e.ep_tID)->CR.Adjust(e.ep_monCR);
    if (e.ep_tID2)
      e.ep_monCR = TTEM(e.ep_tID2)->CR.Adjust(e.ep_monCR);
    if (e.ep_tID3)
      e.ep_monCR = TTEM(e.ep_tID3)->CR.Adjust(e.ep_monCR);
    e.epCurrXCR = XCR(e.ep_monCR);
    
    if (e.ep_hmID) {
      e.ep_mountCR = TMON(e.ep_hmID)->CR;
      if (e.ep_htID)
        e.ep_mountCR = TTEM(e.ep_htID)->CR.Adjust(e.ep_mountCR);
      if (e.ep_htID2)
        e.ep_mountCR = TTEM(e.ep_htID2)->CR.Adjust(e.ep_mountCR);
      e.epCurrXCR += XCR(e.ep_mountCR);
      }
      
    if (e.ep_pID)
      e.epCurrXCR += XCR(TEFF(e.ep_pID)->Level-2);
    if (e.ep_iID)
      e.epCurrXCR += XCR(TEFF(e.ep_iID)->Level-2);
  }




bool okDesAmt(TEncounter *te, int16 des);

int16 maxAmtByCR(int16 CR)
  {
    switch (CR)
      {
        case 1: return 5;
        case 2: return 7;
        case 3: return 10;
        case 4: return 12;
        case 5: return 15;
        default:
          return (CR > 1) ? 50 : 4;
      }
  }

EvReturn Map::enGenerate(EventInfo &e)
  {
    int32 q, c, i, w, j, n, enWeight[1024], tWeight;
    int16 nParts, regTries; uint16 old_enAlign;
    rID enList[1024], xID, wgtCurve[30]; 
    
    e.enTries = 1; 
                 
    regTries = 0;
   
    old_enAlign = e.enAlign;
    DevianceRestart:
    e.enAlign = old_enAlign;
    
    //********* Stage 0: Clear And Initialize Variables
    if (!(e.enFlags & EN_NESTED))
      {
        memset(EncMem,0,sizeof(EncMember)*MAX_ENC_MEMBERS);
        cEncMem = 0;
        cUniform = 0;
        memset(uniformKey,0,sizeof(uniformKey));
        memset(uniformChoice,0,sizeof(uniformChoice));
        memset(CandidateCreatures,0,sizeof(Creature*)*2048);
      }

    if (e.enDesAmt)
      ;
    else if (e.enFlags & EN_SINGLE) 
      e.enDesAmt = 1;
    else if (e.enFlags & EN_ANYOPEN) {
      if (e.enDepth > 2)
        e.enDesAmt = max(1,OpenC / 30);
      else if (e.enDepth == 2)
        e.enDesAmt = max(1,OpenC / 50);
      else
        e.enDesAmt = max(1,OpenC / 75);
      }
    else if (e.enFlags & EN_MULTIPLE)
      e.enDesAmt = 4 + random(5);

    e.enDesAmt = min(e.enDesAmt, maxAmtByCR(e.enCR));

    if (e.enConstraint == MA_AQUATIC ||
        (e.enConstraint > 0x01000000 &&
           RES(e.enConstraint)->isType(T_TMONSTER) &&
           TMON(e.enConstraint)->HasFlag(M_AQUATIC)))
      e.enFlags |= EN_AQUATIC;
    
    if (e.enID)
      goto PresetEncounter;
   
    //********* Stage 1: Choose Encounter ID from Region List
    if (e.enRegID && TREG(e.enRegID)->HasList(ENCOUNTER_LIST) && !e.enID)
      {
        rID wtList[512]; int32 cWeight, enConst[512];
        TREG(e.enRegID)->GetList(ENCOUNTER_LIST,wtList,510);
        cWeight = 10; n = 0; memset(enConst,0,512*sizeof(int32));
        enWeight[0] = 0;
        for (i=0;wtList[i] || wtList[i+1];i++)
          {
            if (wtList[i] == -2)
              {
                ASSERT(RES(wtList[i+1])->Type == T_TENCOUNTER);
                if (TENC(wtList[i+1])->minCR > e.enCR)
                  { i += 2; continue; }
                if (AlignConflict(TENC(wtList[i+1])->Align,e.enAlign,true))
                  { i += 2; continue; }
                enList[n] = wtList[i+1];
                enWeight[n+1] = enWeight[n] + cWeight;
                enConst[n] = wtList[i+2];
                i += 2; n++;
                continue;
              }
            else if (wtList[i] == -3)
              {
                ASSERT(RES(wtList[i+1])->Type == T_TENCOUNTER);
                if (TENC(wtList[i+1])->minCR > e.enCR)
                  { i += 4; continue; }
                if (wtList[i+3] > e.enCR)
                  { i += 4; continue; }
                if (wtList[i+4] < e.enCR)
                  { i += 4; continue; }
                if (AlignConflict(TENC(wtList[i+1])->Align,e.enAlign,true))
                  { i += 3; continue; }
                enList[n] = wtList[i+1];
                enWeight[n+1] = enWeight[n] + cWeight;
                enConst[n] = wtList[i+2];
                i += 4; n++;
                continue;
              }
            else if (wtList[i] < 0x01000000)
              cWeight = wtList[i];
            else {
              ASSERT(RES(wtList[i])->Type == T_TENCOUNTER);
              if (TENC(wtList[i])->minCR > e.enCR)
                continue;
              if (AlignConflict(TENC(wtList[i])->Align,e.enAlign,true))
                continue;
              enList[n] = wtList[i];
              enWeight[n+1] = enWeight[n] + cWeight;
              enConst[n] = 0;
              n++;
              continue;
              }
          }
        if (n) {
          w = random(enWeight[n]);
          for (i=0;i!=n;i++)
            if (enWeight[i+1] > w)
              {
                e.enID = enList[i];
                e.enConstraint = enConst[i];
                goto PresetEncounter;
              }
          }
      }
  
    //********* Stage 2: Build Potential Encounter List  
    enWeight[0] = 0; 
    for (q=0,c=0;q!=1;q++) 
      for(i=0;i!=theGame->Modules[q]->szEnc;i++)
        {
          TEncounter *te = &theGame->Modules[q]->QEnc[i];
          if (te->minCR > e.enCR)
            continue;
          if (te->maxCR < e.enCR)
            continue;
          if (te->HasFlag(NF_NOGEN))
            continue;
          if (!(e.enFlags & EN_AQUATIC)) {
            if (te->HasFlag(NF_AQUATIC))
              continue;
            }
          else {
            if (!(te->HasFlag(NF_AQUATIC) ||
                  te->HasFlag(NF_CONTEXT_AQUATIC)))
              continue;
            }
          if (((e.enFlags & EN_VAULT) == EN_VAULT) != 
              te->HasFlag(NF_VAULT))
            continue;
          if (e.enTerrain) {
            if (!(e.enTerrain & te->Terrain))
              continue;
            if (!(e.enTerrain & TT_SURFACE))
              if (te->Terrain & TT_SURFACE)
                continue;
            }
          if (e.enType)
            if (!(e.enType & te->Type))
              continue;
          if (e.enDesAmt && e.enDesAmt > 1)
            if (te->HasFlag(NF_SINGLE))
              continue;
          if (e.enDesAmt && e.enDesAmt < 2)
            if (te->HasFlag(NF_MULTIPLE))
              continue;
          if (e.enDesAmt && e.enDesAmt > 10)
            if (!te->HasFlag(NF_HORDE))
              continue;
          if (e.enDesAmt && e.enDesAmt < 10)
            if (te->HasFlag(NF_HORDE))
              continue;
          if (e.enDesAmt && !okDesAmt(te,e.enDesAmt))
            continue;
          if (AlignConflict(te->Align,e.enAlign,true))
            continue;
          xID = theGame->Modules[q]->EncounterID(i);
          if (TENC(xID)->Event(e,xID,EV_ISTARGET) == CANNOT_CAST_IT)
            continue;
          enList[c++] = xID;
          if (te->GetList(WEIGHT_CURVE_BY_CR,wgtCurve,30)) {
            for (int16 j=0;;j++)
              if (!wgtCurve[j+1] || (j == e.enCR - te->minCR))
                {
                  enWeight[c] = enWeight[c-1] + wgtCurve[j];
                  break;
                }
            }
          else
            enWeight[c] = enWeight[c-1] + te->Weight;
        }
    
    if (!c)
      {
        enWarn("No suitable encounters found!");
        return ABORT;
      }      
  
    //********* Stage 3: Choose Encounter from Weighted List
    w = random(enWeight[c]);
    for (i=0;i!=c;i++)
      if (enWeight[i+1] > w)
        {
          e.enID = enList[i];
          break;
        }
    
    PresetEncounter:
    //********* Stage 4: Eliminate Inapplicabe/Percentile Parts
    // -- eliminate any with overhigh CR minimums
    // -- choose only one from each OR list
    // -- roll each percentile chance
    ASSERT(e.enID);
    TEncounter *te = TENC(e.enID);
    e.enAlign |= te->Align;
    if (te->HasFlag(NF_CONTEXT_AQUATIC) && e.isLoc)
      if (ResourceHasFlag(e.EMap->TerrainAt(e.EXVal,e.EYVal),TF_WATER) &&
           (e.enConstraint < 0x01000000))
        e.isAquaticContext = true;
    if (te->HasFlag(NF_CONTEXT_AQUATIC))
      if (e.enFlags & EN_AQUATIC)
        e.isAquaticContext = true;
    if (te->HasFlag(NF_AQUATIC))
      e.enFlags |= EN_AQUATIC;
    for (i=0;i!=MAX_PARTS && (te->Parts[i].xID || te->Parts[i].Flags);i++)
      {
        if (te->Parts[i].minCR > e.enCR)
          e.enSkipPart[i] = true;
        if (te->Parts[i].Chance)
          if (random(100)+1 > te->Parts[i].Chance)
            e.enSkipPart[i] = true;
        if (i >= 1 && te->Parts[i].Flags & EP_ELSE)
          if (!e.enSkipPart[i-1]) 
            /* Need to fix for multi-else */
            e.enSkipPart[i] = true;
      }
    nParts = i;
      
    for (i=1;i<nParts;i++)
      if (te->Parts[i].Flags & EP_OR)
        {
          j = i;
          while (j != MAX_PARTS && (te->Parts[j].Flags & EP_OR))
            j++;
          n = (i-1) + random(j-(i-1));
          for(i=i;i!=j;i++)
            if (i != n)
              e.enSkipPart[i] = true;
        }
              
         
    
    //********* Stage 5: Calculate Total Weight; divide XCR
    e.enXCR = XCR(e.enCR);
    e.enSleep = random(100)+1;
    /* We need to increase the xCR in large rooms with a high number
       of desired monsters, else we end up with wierd imbalances and
       hashing. To balance this, we increase the chances that more of
       the monsters will be asleep, initially. */
    if (e.enDesAmt >= 4) {
      e.enXCR += (e.enDesAmt / 3) * (e.enXCR/2);   
      if (!(e.enFlags & EN_NOSLEEP))
        for (i=0;i!=(e.enDesAmt / 3);i++)
          e.enSleep = max(e.enSleep,random(100)+1);
      }
    e.enXCR = max(e.enXCR, XCR(e.enCR) * te->GetConst(MIN_XCR_MULT));
    e.enXCR = max(e.enXCR, te->GetConst(MIN_XCR));
    
    tWeight = 0;
    for(i=0;i!=nParts;i++)
      if (!e.enSkipPart[i])
        tWeight += te->Parts[i].Weight;
    
    
    
    //********* Stage 6: Create Parts in Order
    // if uniform, one mID for everyone, otherwise new one
    // every time
    // a) determine max mID CR
    // b) choose mID
    // c) Fill XCR with numbers and/or templates
    
    /* Some universal preparations for building the encounter */
    if (te->GetList(UNIVERSAL_TEMPLATE,enList,1020))
      {
        rID tID;
        XTHROW(EV_ENCHOOSE_TEMP,xe,
          e.chList = UNIVERSAL_TEMPLATE;
          e.chType = 0;
          );
        if (e.chResult)
          enUniformAdd(UNIVERSAL_TEMPLATE,e.chResult);
      }
      
    e.enFreaky = random(10)+1;
    
    /* Handle NF_STAGGERED encounters differently from most */
    if (te->HasFlag(NF_STAGGERED))
      {
        int16 mCount, nOkParts, okParts[MAX_PARTS], 
                partWeight[MAX_PARTS+1], w, i, j;
        if (e.enDesAmt)
          mCount = e.enDesAmt;
        //else if (te->GetConst(MIN_MCOUNT))
        //  ...
        else
          mCount = 3 + random(5);
        
        nOkParts = 0; partWeight[0] = 0;
        for (i=0;te->Parts[i].xID || te->Parts[i].Flags;i++)
          if (!e.enSkipPart[i])
            {
              okParts[nOkParts] = i;
              partWeight[nOkParts+1] = partWeight[nOkParts]
                + te->Parts[i].Weight;
              nOkParts++;
            }
        
        for (i=0;i!=mCount;i++)
          {
            w = random(partWeight[nOkParts]);
            for (j=0;j!=nOkParts;j++)
              if (partWeight[j+1] > w)
                { e.cPart = okParts[j];
                  break; }
            ASSERT(j < nOkParts);
            
            e.epMinAmt = e.epMaxAmt = 1;
            e.epXCR = e.enXCR / mCount;
            ReThrow(EV_ENGEN_PART,e);
          }
        goto DoneParts;
      }
    
    /* Choose mID, tID for each MON_LIST, TEMP_LIST here first
       if encounter or part is uniform */
    EvReturn r;
    for(i=0;i!=nParts;i++)
      if (!e.enSkipPart[i])
        {
          /* Possible combos of constraints:
           * base CR
           * # of creatures
           * fixed CR
           * -- if too many constraints are true, we can't match
           *    intended XCR, thus just give up and ignore intended
           *    XCR. There may also be _no_ intended XCR (EP_IGNORE_XCR).
           */
          
          EncPart &ep = te->Parts[i];
          if (tWeight && te->Weight)
            e.epXCR = (e.enXCR * ep.Weight) / tWeight;
          else
            e.epXCR = 0;
          if (ep.Amt.Sides)
            e.epMinAmt = e.epMaxAmt = ep.Amt.Roll();
          else if (ep.Amt.Number && ep.Amt.Bonus)
            { e.epMinAmt = ep.Amt.Number;
              e.epMaxAmt = ep.Amt.Bonus; }
          else if (ep.Amt.Bonus)
            e.epMinAmt = e.epMaxAmt = ep.Amt.Bonus;
          else if (TENC(e.enID)->HasFlag(NF_SINGLE))
            e.epMinAmt = e.epMaxAmt = 1;
          else if (TENC(e.enID)->HasFlag(NF_HORDE))
            { e.epMinAmt = 10; e.epMaxAmt = 50; }
          else
            e.epMinAmt = e.epMaxAmt = 0;
          e.cPart = i;
          r = ReThrow(EV_ENGEN_PART,e);
          if (r == ABORT) {
            if (e.enConstraint && e.enRegID)
              {
                e.enID = 0;
                if (regTries++ > 100) {
                  Error(XPrint("Region <Res> has no working encounter at CR "
                    "<Num> after 100 tries!"), e.enRegID, e.enCR);
                  e.enRegID = 0;
                  }
                    
                goto DevianceRestart;
              }
            if (e.enConstraint)
              e.enConstraint = 0;
            e.enTries = 0;
            e.enID = 0;
            goto DevianceRestart;
            } 
        }
    DoneParts:
  
    /* If we're building a nested sub-encounter, skip the
       deviance testing and the actual building of the
       encounter itself. */
    if (e.enFlags & EN_NESTED)
      return DONE;
      
    int16 Deviance; 
    int32 totXCR = 0;
    for (i=0;i!=cEncMem;i++)
      {
        int16 CR;
        CR = TMON(EncMem[i].mID)->CR;
        if (EncMem[i].tID)
          CR = TTEM(EncMem[i].tID)->CR.Adjust(CR);
        if (EncMem[i].tID2)
          CR = TTEM(EncMem[i].tID2)->CR.Adjust(CR);
        if (EncMem[i].tID3)
          CR = TTEM(EncMem[i].tID3)->CR.Adjust(CR);
        totXCR += XCR(CR);
      }
    Deviance = (abs(e.enXCR-totXCR) * 100L) / e.enXCR;
    if (e.enDesAmt)
      Deviance += max(0,((abs(e.enDesAmt-cEncMem)*100L)/e.enDesAmt)-50);
    if (cEncMem > maxAmtByCR(e.enCR))
      Deviance += (cEncMem - maxAmtByCR(e.enCR))*100 / maxAmtByCR(e.enCR);
    if (Deviance > 50 && e.enTries < 5)
      {
        e.enTries++;
        goto DevianceRestart;
      }
      
    //********* Stage 7: Build the Encounter, if Requested
    
    /* Determine the most likely alignment leaning of the
       entire group, and exclude 'bad' alignment accordingly. */
    if (cEncMem)
      e.enDriftGE = e.enDriftGE*(100 / cEncMem);
    if (e.enAlign & (AL_NONGOOD|AL_NONEVIL|AL_GOOD|AL_EVIL))
      ;
    else if (e.enDriftGE - 25 + random(50) > 0)
      e.enAlign |= AL_NONGOOD;
    else
      e.enAlign |= AL_NONEVIL;
    
    e.enIsFormation = false;
    if (te->HasFlag(NF_FORMATION) ||
        (te->HasFlag(NF_FORM50) && !random(2)))
      e.enIsFormation = true;
    
    /* Cap # of creatures at CR max */
    cEncMem = min(cEncMem,maxAmtByCR(e.enCR));
    /* HACKFIX */
    cEncMem = min(cEncMem,5);
    
    if (e.enFlags & EN_DUMP)
      {
        int16 c, j; 
        String str;
        e.enDump = XPrint("<14><Res> (CR <Num>, Dev <Num>%, <Num> tries):<7>\n",
          e.enID, e.enCR, Deviance, e.enTries);
        totXCR = 0;
        for (i=0;i!=cEncMem;i++)
          {
            Monster *mn;
            if (!EncMem[i].mID)
              continue;
            mn = new Monster(EncMem[i].mID);
            mn->CalcValues();
            if (EncMem[i].tID) {
              mn->AddTemplate(EncMem[i].tID);
              mn->IdentifyTemp(EncMem[i].tID);
              }
            mn->CalcValues();
            if (EncMem[i].tID2) {
              mn->AddTemplate(EncMem[i].tID2);
              mn->IdentifyTemp(EncMem[i].tID2);
              }
            mn->CalcValues();
            if (EncMem[i].tID3) {
              mn->AddTemplate(EncMem[i].tID3);
              mn->IdentifyTemp(EncMem[i].tID3);
              }
            mn->CalcValues();
            Creature *old_actor = e.EActor;
            e.EActor = mn;
            ReThrow(EV_ENGEN_ALIGN, e);
            e.EActor = old_actor;
            EncMem[i].Align = mn->getAlignment();
            EncMem[i].hMon = mn->myHandle;
          }
        for (i=0;i!=cEncMem;i++)
          { 
            if (!EncMem[i].mID)
              continue;
            Monster *mn = oMonster(EncMem[i].hMon), *mt;
            for (j=i+1,c=1;j<cEncMem;j++)
              if (EncMem[i].mID == EncMem[j].mID &&
                  EncMem[i].tID == EncMem[j].tID &&
                  EncMem[i].tID2 == EncMem[j].tID2 &&
                  EncMem[i].tID3 == EncMem[j].tID3 &&
                  EncMem[i].hmID == EncMem[j].hmID &&
                  EncMem[i].htID == EncMem[j].htID &&
                  EncMem[i].htID2 == EncMem[j].htID2 &&
                  EncMem[i].iID == EncMem[j].iID &&
                  EncMem[i].pID == EncMem[j].pID &&
                  EncMem[i].Flags == EncMem[j].Flags &&
                  EncMem[i].Align == EncMem[j].Align)
                { EncMem[j].mID = 0; c++; }
            mn->StateFlags |= MS_KNOWN;
            str = "> ";
            if (c > 1)
              str += Format("%d %s", c, (const char*)
                Pluralize(mn->Name(NA_MONO|NA_IDENT)));
            else
              str += mn->Name(NA_MONO|NA_IDENT);
            
            if (!EncMem[i].Align)
              str += " (TN)";
            else
              str += Format(" (%c%c)",
                       (EncMem[i].Align & AL_LAWFUL) ? 'L' :
                       (EncMem[i].Align & AL_CHAOTIC) ? 'C' : 'N',
                       (EncMem[i].Align & AL_GOOD) ? 'G' :
                       (EncMem[i].Align & AL_EVIL) ? 'E' : 'N');
            
            mt = NULL;
            if (EncMem[i].hmID)
              {
                String mstr;
                mt = new Monster(EncMem[i].hmID);
                mt->CalcValues();
                if (EncMem[i].htID) {
                  mt->AddTemplate(EncMem[i].htID);
                  ASSERT(mt->HasEffStati(TEMPLATE,EncMem[i].htID));
                  mt->IdentifyTemp(EncMem[i].htID);
                  }
                mt->CalcValues();
                if (EncMem[i].htID2) {
                  mt->AddTemplate(EncMem[i].htID2);
                  ASSERT(mt->HasEffStati(TEMPLATE,EncMem[i].htID));
                  mt->IdentifyTemp(EncMem[i].htID2);
                  }
                mt->CalcValues();
                mt->StateFlags |= MS_KNOWN;
                mstr = mt->Name(NA_MONO|NA_IDENT);
                if (c > 1)
                  mstr = Pluralize(mstr);
                str += Format("\n    (riding %s%s)", 
                  c > 1 ? "" : "a ", (const char*)mstr);
                delete mt;
              }
            if (EncMem[i].pID)
              str += XPrint("\n    (with <Res> on weapons)", EncMem[i].pID);
            
            //if (flags & sleep) s+=" [Asleep]" etc.
            e.enDump += str;
            int32 theXCR;
            {
              EventInfo e;
              e.Clear();
              e.ep_mID   = EncMem[i].mID;
              e.ep_tID   = EncMem[i].tID;
              e.ep_tID2  = EncMem[i].tID2;
              e.ep_tID3  = EncMem[i].tID3;
              e.ep_hmID  = EncMem[i].hmID;
              e.ep_htID  = EncMem[i].htID;
              e.ep_htID2 = EncMem[i].htID2;
              e.ep_iID   = EncMem[i].iID;
              e.ep_pID   = EncMem[i].pID;
              enCalcCurrPartXCR(e);
              theXCR = e.epCurrXCR * c;
            }
              
            e.enDump += Format(" [%d/%d]\n", mn->ChallengeRating(true),theXCR);
            totXCR += theXCR;
            delete mn;
          }
        for (i=0;i!=cEncMem;i++)
          if (EncMem[i].hMon &&
                isValidHandle(EncMem[i].hMon))
            delete oThing(EncMem[i].hMon);
            
        e.enDump += XPrint("<6>XCR <Num>, desired <Num> (CR <Num>).\n\n",totXCR,
            e.enXCR, e.enCR);
        return DONE;
        //theGame->GetPlayer(0)->AddJournalEntry(e.enDump);
      }
    
    if (!(e.enFlags & EN_NOBUILD))
      {
        Thing *targ = e.ETarget;
        for (i=0;i<cEncMem;i++)
          {
            e.cMember = i;
            ASSERT(i < MAX_ENC_MEMBERS);
            ReThrow(EV_ENBUILD_MON,e);
          }
        e.ETarget = targ;
      
        Creature *leader, *best;
        int16 bestCR;
        bestCR = -6; best = NULL; leader = NULL;
        for (i=0;i!=cEncMem;i++)
          {
            Creature *mn = oCreature(EncMem[i].hMon);
            if (te->Parts[EncMem[i].Part].Flags & EP_LEADER)
              { 
                leader = mn;
                break;
              }
            if (mn->ChallengeRating() > bestCR);
              if (mn->isMType(MA_SAPIENT))
                { best = mn; bestCR = mn->ChallengeRating(); }
          }
        if (best && !leader)
          leader = best;
          
        if (leader)
          for (i=0;i!=cEncMem;i++)
            {
              Creature *mn = oCreature(EncMem[i].hMon);
              if (mn == leader)
                continue;
              mn->ts.addCreatureTarget(leader, TargetLeader);
              if (e.enIsFormation)
                mn->ts.giveOrder(mn,leader,OrderWalkNearMe);
              mn->ts.Retarget(mn);
            }
      }
            
    return DONE;
  }
  
bool okDesAmt(TEncounter *te, int16 des)
  {
    int16 mn, mx, i, A, B, C;
    mn = 0; mx = 0;
    for (i=0;te->Parts[i].xID || te->Parts[i].Flags;i++)
      {
        EncPart *ep = &te->Parts[i];
        if (ep->Amt.Sides)
          { A = ep->Amt.Roll();
            B = ep->Amt.Roll();
            C = ep->Amt.Roll();
            mn += min(A,min(B,C));
            mx += max(A,max(B,C)); }
        else if (ep->Amt.Number && ep->Amt.Bonus)
          { mn += ep->Amt.Number;
            mx += ep->Amt.Bonus; }
        else if (ep->Amt.Bonus)
          { mn += ep->Amt.Bonus;
            mx += ep->Amt.Bonus; }
        else if (te->HasFlag(NF_SINGLE))
          { mn++; mx++; }
        else if (te->HasFlag(NF_HORDE))
          { mn += 10; mx += 50; }
        else
          { mx += 50; }
      }
    if (des >= mn && des <= mx)
      return true;
    return false;
  }

int16 getMinCR(EventInfo &_e)
  {
    int16 i;
    EventInfo e;
    e.Clear();
    e = _e;
    e.eimXCR = 1;    
    e.ep_mID = 
      e.ep_tID =
      e.ep_tID2 =
      e.ep_tID3 = 
      e.ep_hmID =
      e.ep_htID =
      e.ep_htID2 =
      e.ep_iID =
      e.ep_pID = 0;
    e.isGetMinCR = true;
    XTHROW(EV_ENCHOOSE_MID,e,);
    if (!e.chResult)
      return 0;
    e.ep_mID = e.chResult;
    ReThrow(EV_ENSELECT_TEMPS,e);
    e.EMap->enCalcCurrPartXCR(e);
    return XCRtoCR(e.epCurrXCR);
  }

int16 getMaxCR(EventInfo &_e)
  {
    int16 i;
    EventInfo e;
    e.Clear();
    e = _e;
    e.eimXCR = 1000000L;    
    e.ep_mID = 
      e.ep_tID =
      e.ep_tID2 =
      e.ep_tID3 = 
      e.ep_hmID =
      e.ep_htID =
      e.ep_htID2 =
      e.ep_iID =
      e.ep_pID = 0;
    e.isGetMaxCR = true;
    XTHROW(EV_ENCHOOSE_MID,e,);
    if (!e.chResult)
      return 36;
    e.ep_mID = e.chResult;
    ReThrow(EV_ENSELECT_TEMPS,e);
    e.EMap->enCalcCurrPartXCR(e);
    return XCRtoCR(e.epCurrXCR);
  }

  
EvReturn Map::enGenPart(EventInfo &e)
  {
    /* -- If there's no XCR, generate exactly what's asked for
          without regard to how much it "costs". 
       -- If we have XCR and a fixed amount, divide the XCR by
          the amount, and then generate N monsters as close to
          that XCR as possible.
       -- If we have fixed monster CR (set mID, no EP_SKILLED
          or EP_CLASSED, etc.) but unspecified or ranged Amt,
          calculate individual monster XCR, divide by total XCR 
          to find ideal Amt, then if needed raise Amt to minAmt
          or lower it to maxAmt.
       -- If we have variable monster CR and unspecified or
          ranged Amt, do these steps:
          a) find minimum and maximum monster CR
          b) -- If EP_SKEW_FOR_AMT,
             -- If EP_SKEW_FOR_XCR,
             
       Also, check M_GROUP, M_SOLITARY, etc. if we chose an mID
       and it doesn't override explicit settings in the encounter
       
       We might have NF_GENERIC single-Part encounters that can
       respect GROUP_LEADER, GROUP_PET, GROUP_MOUNT, etc. lists
       in the monster resource. We could also kludge this to allow
       building encounters around a specific mID again.
       (We could also scan through encounters to see which have a
        match for a desired mID in any of their Parts, and then
        force that mID, for use in, eg., generating an adlet tribe
        in a cold room with "Goblinoid Warband" unmodified.) 
       
       
       */
    int16 i, uBound, lBound;
    TEncounter *te = TENC(e.enID);
    EncPart *ep = &te->Parts[e.cPart];
    
    e.epClassRoll = random(100)+1;
    
    #define RNDAMT (e.epMinAmt + random((e.epMaxAmt+1) - e.epMinAmt))
    {
      int16 A, B, C, D, E;
      A = RNDAMT;
      B = RNDAMT;
      C = RNDAMT;
      D = RNDAMT;
      E = RNDAMT;
      uBound = max(A,max(B,max(C,max(D,E))));
      lBound = min(A,min(B,min(C,min(D,E))));
    }
    if (ep->Flags & EP_NOXCR)
      {
        if (e.epMinAmt)
          e.epAmt = RNDAMT;
        else
          e.epAmt = 1;
      }
    else if (e.epMinAmt != 0 &&
             e.epMinAmt == e.epMaxAmt)
      {
        e.eimXCR = min(XCR(e.enCR),e.epXCR / e.epMinAmt);
        e.epAmt = e.epMinAmt;
      }
    else 
      {
        int16 minCR, maxCR;
        minCR = getMinCR(e);
        maxCR = getMaxCR(e);
        maxCR = min(maxCR,e.enCR);
        if (minCR == maxCR)
          {
            e.eimXCR = XCR(minCR);
            e.epAmt = max(1,e.epXCR / e.eimXCR);
            if (e.epMaxAmt)
              if (e.epAmt > e.epMaxAmt)
                e.epAmt = e.epMaxAmt;
            if (e.epMinAmt)
              if (e.epAmt < e.epMinAmt)
                e.epAmt = e.epMinAmt;
          }
        else
          {
            if (ep->Flags & EP_SKEW_FOR_AMT)
              e.eimXCR = (XCR(minCR)*3+XCR(maxCR)*1)/4;
            else if (ep->Flags & EP_SKEW_FOR_XCR)
              e.eimXCR = (XCR(minCR)*1+XCR(maxCR)*4)/4;
            else
              e.eimXCR = (XCR(minCR)*2+XCR(maxCR)*2)/4;
            e.eimXCR = min(XCR(e.enCR),e.eimXCR);
            e.epAmt = max(1,e.epXCR / e.eimXCR);
            
            if (e.epMaxAmt)
              if (e.epAmt > e.epMaxAmt)
                {
                  /* Don't gen *exactly* the max, include some randomness
                     by taking highest-of-four. */
                  e.epAmt = uBound;
                  e.eimXCR = e.epXCR / e.epAmt;
                  if (e.eimXCR < XCR(minCR))
                    e.eimXCR = XCR(minCR);
                }
            if (e.epMinAmt)
              if (e.epAmt < e.epMinAmt)
                {
                  /* Don't gen *exactly* the min, include some randomness
                     by taking lowest-of-four. */
                  e.epAmt = lBound;
                  e.eimXCR = e.epXCR / e.epAmt;
                  if (e.eimXCR < XCR(minCR))
                    e.eimXCR = XCR(minCR);
                }    
            e.eimXCR = min(XCR(e.enCR),e.eimXCR);
          }
      }


    int32 totalXCR; 
    int16 old_cEncMem, old_cUniform;
    totalXCR = 0; e.epTries = 0;
    EvReturn r;
    
    for (i=0;i!=e.epAmt;i++)
      {
        old_cEncMem = cEncMem;
        old_cUniform = cUniform;
        RetryPart:
        e.epFailed = false;
        e.ep_mID = 
          e.ep_tID =
          e.ep_tID2 =
          e.ep_tID3 = 
          e.ep_hmID =
          e.ep_htID =
          e.ep_htID2 =
          e.ep_iID =
          e.ep_pID = 0;
        if (te->HasFlag(NF_FREAKY) || ep->Flags & EP_FREAKY)
          e.epFreaky = min(e.enFreaky, random(10)+1);
        else
          e.epFreaky = 0;
          
        RXTHROW(EV_ENCHOOSE_MID,e,r,);
        if (r == ABORT)
          return ABORT;
        e.ep_mID = e.chResult;
        ASSERT(e.ep_mID);
        if (RES(e.ep_mID)->Type == T_TENCOUNTER)
          {
            EventInfo ne;
            ne.Clear();
            ne = e;
            ne.enXCR = e.eimXCR;
            ne.enCR = XCRtoCR(ne.enXCR);
            ne.enID = e.ep_mID;
            ne.cPart = 0;
            ne.enFlags = (e.enFlags | EN_NESTED);
            ReThrow(EV_ENGEN,ne);
            
            for (int j=old_cEncMem;j!=cEncMem;j++)
              {
                e.ep_mID   = EncMem[j].mID;
                e.ep_tID   = EncMem[j].tID;
                e.ep_tID2  = EncMem[j].tID2;
                e.ep_tID3  = EncMem[j].tID3;
                e.ep_hmID  = EncMem[j].hmID;
                e.ep_htID  = EncMem[j].htID;
                e.ep_htID2 = EncMem[j].htID2;
                e.ep_pID   = EncMem[j].pID;
                e.ep_iID   = EncMem[j].iID;
                enCalcCurrPartXCR(e);
                totalXCR += e.epCurrXCR;
              }
            
            goto DoneNestedEnc;
          }
        
        e.ep_mID = e.chResult;
        ReThrow(EV_ENSELECT_TEMPS,e);
        if (e.epFailed && e.epTries++ < 20) {
          cEncMem = old_cEncMem;
          cUniform = old_cUniform;
          goto RetryPart;
          }
        Player *p;
        if (e.epFailed && (p = theGame->GetPlayer(0))) {
          String str;
          str = XPrint("e.epFailed == true after 20 tries: Enc '<Res>', CR <Num>, Part <Num>,"
            " eimXCR <Num>", e.enID, e.enCR, e.cPart, e.eimXCR);
          if (e.eID)
            str += XPrint(", eID '<Res>'", e.eID);
          if (e.enRegID && RES(e.enRegID)->HasList(ENCOUNTER_LIST))
            str += XPrint(", enRegID '<Res>'", e.enRegID);
          if (e.enConstraint && e.enConstraint >= 0x01000000)
            str += XPrint(", e.enConstraint '<Res>'", e.enConstraint);
          else if (e.enConstraint)
            str += XPrint(", e.enConstraint '<Str>'", Lookup(MTypeNames,e.enConstraint));
          str += ".";
          if (e.enFlags & EN_OODMON)
            str += " [EN_OODMON]";
          if (e.enFlags & EN_SINGLE)
            str += " [EN_SINGLE]";
          if (e.enFlags & EN_AQUATIC)
            str += " [EN_AQUATIC]";
          if (e.enFlags & EN_STREAMER)
            str += " [EN_STREAMER]";
          if (e.enFlags & EN_CREATOR)
            str += " [EN_CREATOR]";
          if (e.enFlags & EN_SUMMON)
            str += " [EN_SUMMON]";
          if (e.enFlags & EN_VAULT)
            str += " [EN_VAULT]";
          if (e.enFlags & EN_DUNREGEN)
            str += " [EN_DUNREGEN]";          
          p->AddJournalEntry(str);
          }
        e.epFailed = false;
        
        ASSERT(!e.ep_tID  || (RES(e.ep_tID )->Type == T_TTEMPLATE))
        ASSERT(!e.ep_tID2 || (RES(e.ep_tID2)->Type == T_TTEMPLATE))
        ASSERT(!e.ep_tID3 || (RES(e.ep_tID3)->Type == T_TTEMPLATE))

        if (e.ep_hmID) {
          bool riderIsUndead, mountIsUndead;
          riderIsUndead = mountIsUndead = false;
          if (TMON(e.ep_mID)->isMType(e.ep_mID, MA_UNDEAD))
            riderIsUndead = true;
          if (e.ep_tID && (TTEM(e.ep_tID)->TType & TM_UNDEAD))
            riderIsUndead = true;
          if (e.ep_tID2 && (TTEM(e.ep_tID2)->TType & TM_UNDEAD))
            riderIsUndead = true;
          if (e.ep_tID3 && (TTEM(e.ep_tID3)->TType & TM_UNDEAD))
            riderIsUndead = true;
            
          if (TMON(e.ep_hmID)->isMType(e.ep_hmID, MA_UNDEAD))
            mountIsUndead = true;
          if (e.ep_htID && (TTEM(e.ep_htID)->TType & TM_UNDEAD))
            mountIsUndead = true;
          if (e.ep_htID2 && (TTEM(e.ep_htID2)->TType & TM_UNDEAD))
            mountIsUndead = true;
          
          if (riderIsUndead && !mountIsUndead)
            switch(random(3))
              {
                case 0:
                  enAddMountTemp(e,FIND("graveborn;template"));
                break;
                case 1:
                  enAddMountTemp(e,FIND("zombie"));
                break;
                case 2:
                  enAddMountTemp(e,FIND("skeleton"));
                break;      
              }
          }

        
        enAddMon(e);
        UpdateAlignRestrict(e);
        
        /* Adaptive Filling -- reappraise our judgement of how
           many monsters we need as we continue to build them! */
        enCalcCurrPartXCR(e);
        totalXCR += e.epCurrXCR;
        
        DoneNestedEnc:
        
        if (totalXCR >= e.epXCR && i >= (lBound-1))
          break;
        if (i == (e.epAmt - 1) && e.epAmt < uBound &&
             (totalXCR+(totalXCR/(i+1))) <= e.epXCR)
          e.epAmt++; 
      }
    
    return DONE; 
    
  }
  
EvReturn Map::enChooseMID(EventInfo &e)
  {
    TEncounter *te = TENC(e.enID);
    EncPart *ep = &te->Parts[e.cPart];  
    rID wmList[512], monList[512], xID;
    int32 monWeights[512];
    int16 cWeight, nMon, i, q, x, j;
    nMon = 0;
    bool isSingle = te->HasFlag(NF_SINGLE);
    Restart:
    int16 maxCR = XCRtoCR(e.eimXCR);
    
    /* Kludge: leave room for class template! */
    if (ep->Flags & EP_CLASSED)
      maxCR = max(1,maxCR-2);
      
      
    if (e.enConstraint > 0x01000000)
      maxCR = max(maxCR,min(e.enCR,TMON(e.enConstraint)->CR));
      
    maxCR = max(1,maxCR);
    
    /* If we have mandatory templates, keep lowering the maxCR
       on consecutive tries until we have room for all of them. */
    if (e.epTries > 5)
      maxCR = max(0,maxCR - (e.epTries-5)/2);
    
      
    if (ep->xID >= 0x01000000)
      {
        e.chResult = ep->xID;
        return DONE;
      }
    else if (ep->Flags & EP_ANYMON)
      {
        if (te->HasFlag(NF_UNIFORM))
          if (e.chResult = enUniformGet(ep->xID | 0x0F000000))
            return DONE;
        if (ep->Flags & EP_UNIFORM)
          if (e.chResult = enUniformGet(e.cPart))
            return DONE;
        monWeights[0] = 0; nMon = 0;
        bool isAquatic = te->HasFlag(NF_AQUATIC) ||
                         e.isAquaticContext ||
                         e.enConstraint == MA_AQUATIC;
        if ((ep->xID2 > 0x01000000) && !stricmp(NAME(ep->xID2),"aqueous"))
          isAquatic = false;
        for (q=0;q!=1;q++) 
          for(i=0;i!=theGame->Modules[q]->szMon;i++)
            {
              TMonster *tm = &theGame->Modules[q]->QMon[i];
              if (tm->CR > max(1,maxCR) && !e.isGetMinCR)
                continue;
              if (tm->HasFlag(M_NOGEN) && e.enConstraint < 0x01000000)
                continue;
              if (tm->Depth > e.enDepth)
                continue;
              if (tm->HasFlag(M_AQUATIC) != isAquatic)
                if (!tm->HasFlag(M_AMPHIB))
                  continue;
              if (tm->HasFlag(M_PLAYER) || tm->HasFlag(M_UNKNOWN))
                continue;
              if (tm->HasFlag(M_SOLITARY) && !isSingle)
                continue;
              rID mID = theGame->Modules[q]->MonsterID(i);
              if (tm->HasFlag(M_IALIGN) && tm->isMType(mID,MA_SAPIENT))
                if (AlignConflict(ResAlign(mID),e.enAlign,true))
                  continue;
              if (!tm->isMType(mID,ep->xID))
                continue;
              if (e.enConstraint) {
                if (e.enConstraint > 0x01000000)
                  if (mID != e.enConstraint)
                    continue;
                if (e.enConstraint < 0x01000000)
                  if (!tm->isMType(mID,e.enConstraint))
                    continue;
                }
              {
                rID old_eID = e.eID;
                e.eID = mID;
                if (TENC(e.enID)->Event(e,e.enID,EV_CRITERIA) == CANNOT_CAST_IT)
                  { e.eID = old_eID; continue; }
                e.eID = old_eID;
              }
              if (e.chCriteria)
                if ((*e.chCriteria)(e,mID) == false)
                  continue;
              monList[nMon++] = mID;
              monWeights[nMon] = monWeights[nMon-1]+1;
            }
        goto ChooseFromList;
      }
    else
      {
        if (te->HasFlag(NF_UNIFORM))
          if (e.chResult = enUniformGet(ep->xID))
            return DONE;
        if (ep->Flags & EP_UNIFORM)
          if (e.chResult = enUniformGet(e.cPart))
            return DONE;
          
        if (te->GetList(ep->xID,wmList,510)) {
          monWeights[0] = 0; nMon = 0; cWeight = 10;
          bool isAquatic = te->HasFlag(NF_AQUATIC) ||
                           e.isAquaticContext ||
                         e.enConstraint == MA_AQUATIC;
          if ((ep->xID2 > 0x01000000) && !stricmp(NAME(ep->xID2),"aqueous"))
            isAquatic = false;
          e.chList = ep->xID;
          for(i=0;wmList[i] || wmList[i+1];i++)
            {
              if (wmList[i] == -1) // ANY TM_PLANAR
                { 
                  i++; 
                  for (q=0;q!=1;q++) 
                    for(j=0;j!=theGame->Modules[q]->szMon;j++)
                      {
                        TMonster *tm = &theGame->Modules[q]->QMon[j];
                        if (tm->CR > maxCR && !e.isGetMinCR)
                          continue;
                        if (tm->HasFlag(M_NOGEN) && e.enConstraint < 0x01000000)
                          continue;
                        if (tm->Depth > e.enDepth)
                          continue;
                        if (tm->HasFlag(M_AQUATIC) != isAquatic)
                          if (!tm->HasFlag(M_AMPHIB))
                            continue;
                        if (tm->HasFlag(M_SOLITARY) && !isSingle)
                          continue;
                        if (tm->HasFlag(M_PLAYER) || tm->HasFlag(M_UNKNOWN))
                          continue;
                        rID mID = theGame->Modules[q]->MonsterID(j);
                        if (!tm->isMType(mID,wmList[i]))
                          continue;
                        if (tm->HasFlag(M_IALIGN) && tm->isMType(mID,MA_SAPIENT))
                          if (AlignConflict(ResAlign(mID),e.enAlign,true))
                            continue;
                        if (e.enConstraint) {
                          if (e.enConstraint > 0x01000000)
                            if (mID != e.enConstraint)
                              continue;
                          if (e.enConstraint < 0x01000000)
                            if (!tm->isMType(mID,e.enConstraint))
                              continue;
                          }
                        {
                          rID old_eID = e.eID;
                          e.eID = mID;
                          if (TENC(e.enID)->Event(e,e.enID,EV_CRITERIA) == CANNOT_CAST_IT)
                            { e.eID = old_eID; continue; }
                          e.eID = old_eID;
                        }
                        if (e.chCriteria)
                          if ((*e.chCriteria)(e,mID) == false)
                            continue;
                        monList[nMon++] = mID;
                        monWeights[nMon] = monWeights[nMon-1]+cWeight;
                      }
                  e.chList = 0;
                }
              else if (wmList[i] < 0x01000000)
                cWeight = wmList[i];
              else 
                {
                  TMonster *tm = TMON(wmList[i]);
                  if (tm->Type == T_TENCOUNTER)
                    {
                      TEncounter *nte = (TEncounter*)tm;
                      if (nte->minCR > maxCR && !e.isGetMinCR)
                        continue;
                      if (nte->HasFlag(NF_AQUATIC) && !isAquatic)
                        continue;
                      if (nte->Depth > e.enDepth)
                        continue;
                      goto GenericTests;
                    }
                  if (tm->CR > maxCR && !e.isGetMinCR)
                    continue;
                  if (tm->Depth > e.enDepth)
                    continue;
                  if (tm->HasFlag(M_AQUATIC) && !isAquatic)
                    if (!tm->HasFlag(M_AMPHIB))
                      continue;
                  if (tm->HasFlag(M_PLAYER) || tm->HasFlag(M_UNKNOWN))
                    continue;
                  if (e.enConstraint) {
                    if (e.enConstraint > 0x01000000)
                      if (wmList[i] != e.enConstraint)
                        continue;
                    if (e.enConstraint < 0x01000000)
                      if (!tm->isMType(wmList[i],e.enConstraint))
                        continue;
                    }
                  if (tm->HasFlag(M_IALIGN) && tm->isMType(wmList[i],MA_SAPIENT))
                    if (AlignConflict(ResAlign(wmList[i]),e.enAlign,true))
                      continue;
              
                  GenericTests:
                  {
                    rID old_eID = e.eID;
                    e.eID = wmList[i];
                    if (TENC(e.enID)->Event(e,e.enID,EV_CRITERIA) == CANNOT_CAST_IT)
                      { e.eID = old_eID; continue; }
                    e.eID = old_eID;
                  }
                  if (e.chCriteria)
                    if ((*e.chCriteria)(e,wmList[i]) == false)
                      continue;
                  monList[nMon++] = wmList[i];
                  monWeights[nMon] = monWeights[nMon-1]+cWeight;
                }
              if (nMon >= 510)
                break;
            }
          goto ChooseFromList;
          }
      }
            
    ChooseFromList:
    if (!nMon)
      {
        Player *p;   
        if (p = theGame->GetPlayer(0)) {
          String str;
          str = XPrint("enChooseMID Failed: Enc '<Res>', CR <Num>, Part <Num>,"
            " eimXCR <Num>", e.enID, e.enCR, e.cPart, e.eimXCR);
          if (e.eID)
            str += XPrint(", eID '<Res>'", e.eID);
          if (e.enRegID && RES(e.enRegID)->HasList(ENCOUNTER_LIST))
            str += XPrint(", enRegID '<Res>'", e.enRegID);
          if (e.enConstraint && e.enConstraint >= 0x01000000)
            str += XPrint(", e.enConstraint '<Res>'", e.enConstraint);
          else if (e.enConstraint)
            str += XPrint(", e.enConstraint '<Str>'", Lookup(MTypeNames,e.enConstraint));
          str += ".";
          if (e.enFlags & EN_OODMON)
            str += " [EN_OODMON]";
          if (e.enFlags & EN_SINGLE)
            str += " [EN_SINGLE]";
          if (e.enFlags & EN_AQUATIC)
            str += " [EN_AQUATIC]";
          if (e.enFlags & EN_STREAMER)
            str += " [EN_STREAMER]";
          if (e.enFlags & EN_CREATOR)
            str += " [EN_CREATOR]";
          if (e.enFlags & EN_SUMMON)
            str += " [EN_SUMMON]";
          if (e.enFlags & EN_VAULT)
            str += " [EN_VAULT]";
          if (e.enFlags & EN_DUNREGEN)
            str += " [EN_DUNREGEN]";          
          p->AddJournalEntry(str);
          }
        e.chResult = FIND("human");
        return ABORT;    
      }

    if (e.isGetMinCR)
      {
        int16 bestCR; rID bestID;
        bestID = 0; bestCR = 100;
        for(i=0;i!=nMon;i++)
          if (TMON(monList[i])->CR < bestCR)
            { bestCR = TMON(monList[i])->CR;
              bestID = monList[i]; }
        e.chResult = bestID;
        return DONE;
      }
    else if (e.isGetMaxCR)
      {
        int16 bestCR; rID bestID;
        bestID = 0; bestCR = -100;
        for(i=0;i!=nMon;i++)
          if (TMON(monList[i])->CR > bestCR)
            { bestCR = TMON(monList[i])->CR;
              bestID = monList[i]; }
        e.chResult = bestID;
        return DONE;
      }
    else if (ep->Flags & EP_SKEW_FOR_MID)
      {
        int16 bestCR, j; rID bestID;
        bestID = 0; bestCR = -100;
        for (j=0;j!=4;j++) {
          x = random(monWeights[nMon]);
          for (i=0;monWeights[i+1];i++)
            if (monWeights[i+1]>x) {
              if (TMON(monList[i])->CR > bestCR)
                { bestID = monList[i]; 
                  bestCR = TMON(monList[i])->CR; }
              break;
              }
              
          }
        e.chResult = bestID;
      }
    else
      {
        x = random(monWeights[nMon]);
        for (i=0;monWeights[i+1];i++)
          if (monWeights[i+1]>x)
            { e.chResult = monList[i]; 
              break; }
        ASSERT(e.chResult);
      }
    if (te->HasFlag(NF_UNIFORM))
      enUniformAdd(ep->xID | ((ep->Flags & EP_ANYMON) ? 0x0F000000 : 0),e.chResult);
    if (ep->Flags & EP_UNIFORM)
      enUniformAdd(e.cPart,e.chResult);
    
    return DONE;
  }

  
EvReturn Map::enGenMount(EventInfo &e)
  {
    rID wtList[512], xID; int16 mtIdxList[512];
    int32 mtWeights[512]; TMonster *tm;
    int16 cWeight, cFreaky, nMount, i, j, x, Idx;
    int16 maxCR, lowCR, highCR, lowIdx, highIdx;
    
    enCalcCurrPartXCR(e);
    maxCR = XCRtoCR(e.eimXCR - e.epCurrXCR);
    maxCR = max(1,maxCR);
    lowCR = 36; lowIdx = -1;
    highCR = -6; highIdx = -1;
    
    mtWeights[0] = 0;
    
    tm = TMON(e.ep_mID);
    if (!tm->HasFlag(M_HUMANOID) ||
         tm->HasFlag(M_NOLIMBS))
      return DONE;
      
    if (!(e.chSource && e.chList))
      { 
        e.ep_hmID = FIND("warhorse");
        return DONE;
      }
      
    RES(e.chSource)->GetList(e.chList,wtList,511);
    nMount = 0; cWeight = 10;
    for(i=0;wtList[i] || wtList[i+1];i++)
      {
        if (((int32)wtList[i]) <= -10) 
          cFreaky = -wtList[i];
        else if (wtList[i] < 0x01000000)
          cWeight = wtList[i];
        else if (RES(wtList[i])->Type == T_TTEMPLATE)
          continue;
        else if (RES(wtList[i])->Type == T_TMONSTER)
          {
            int16 theCR;
            theCR = TMON(wtList[i])->CR;
            for (j=i-1;j>=0;j--)
              {
                if ((wtList[j] < 0x01000000) || 
                    (RES(wtList[j])->Type != T_TTEMPLATE))
                  break;
                theCR = TTEM(wtList[j])->CR.Adjust(theCR);
              }
            lowCR = min(theCR,lowCR);
            highCR = max(theCR,highCR);
            if (theCR == lowCR)
              lowIdx = i;
            if (theCR == highCR)
              highIdx = i;
            if (theCR > maxCR)
              continue;
            mtIdxList[nMount++] = i;
            mtWeights[nMount] = mtWeights[nMount-1]+cWeight;
          }
        if (nMount >= 510)
          break;
      }
    if (!nMount)
      return DONE;
      
    if (e.isGetMinCR)
      {
        if (lowIdx == -1)
          return DONE;
        Idx = lowIdx;
        goto RenderMount;
      }
    else if (e.isGetMaxCR)
      {
        if (highIdx == -1)
          return DONE;
        Idx = highIdx;
        goto RenderMount;
      }
      
      
    if (e.chMaximize)
      {          
        //
      }  
    else if (e.chBestOfTwo)
      {
        //
      }     
    x = random(mtWeights[nMount]);
    for (i=0;mtWeights[i+1];i++)
      if (mtWeights[i+1]>x)
        {
          Idx = mtIdxList[i]; 
          
          RenderMount:
          e.ep_hmID = wtList[Idx];
          for (j=Idx-1;j>=0;j--)
            {
              if ((wtList[j] < 0x01000000) || 
                  (RES(wtList[j])->Type != T_TTEMPLATE))
                break;
              enAddMountTemp(e,wtList[j]);
            }
          break;
        }
        
          
    return DONE;
  }  

bool hasTempType(EventInfo &e, uint32 type)
  {
    uint32 types;
    types = 0;
    if (e.ep_tID)
      types |= TTEM(e.ep_tID)->Type;
    if (e.ep_tID2)
      types |= TTEM(e.ep_tID2)->Type;
    if (e.ep_tID3)
      types |= TTEM(e.ep_tID3)->Type;
    if (type & types)
      return true;
    return false;
  }

EvReturn Map::enSelectTemps(EventInfo &e)
  {
  
    /*
    
      1) Apply any templates explicitly specified in the Part
        object.
      2) Apply the UNIVERSAL_TEMPLATE for the encounter, if
        any, choosing from a weighted list.
      3) After this, return after each step if equal to or over
        90% of the allotted XCR.
      4) If EP_CLASSED (or EP_10CLASS, or EP_25CLASS, or EP_50CLASS
        and we roll above), choose a template from the weighted
        CLASS_LIST if it exists, otherwise call listClassTemplates() 
        and choose one.
      5) If NF_SINGLE or NF_FREAKY and FF >= 19 (or whatever), then
        call listOtherTemplates and choose one (which might be
        TM_NATURE, TM_DESCENT or (with careful checks) TM_PLANAR.
      6) If EP_SKILLED, call listSkillTemplates() and choose whichever
        one brings the XCR closest to the desired XCR.
    
    
      The listTemplates functions takes a parameter, a template type,
      and returns a list of all the templates that can fit the chosen
      mID, don't put the current CR over desired XCR and match the
      type.
      
      Testing Template Appropriateness:
          
      or:
      rID enChooseTemp(uint32 TTypes, int16 OverrideList, bool maximize)
    
      Remember that we call this also if the template is 
        "any TM_PLANAR" or whatever
        
      rID processWeightedList(rID xID, uint32 listNum, bool (*criteria)(EventInfo &e))
      -- remember that this has to work for both the specified
        criteria func and also test with EV_CRITERIA
      -- for mon lists, evaluate some kind of "any MA_ANIMAL"
        construct properly
      
    
      Choosing mID:
        -- nothing with M_IALIGN and conflicting alignment with any
          previous parts
        -- calculate max base CR, considering explicit and universal
          template adjustments and allowed XCR.
    
    */
    rID tID, tmList[512]; int32 q, i, c, chance;
    TEncounter *te = TENC(e.enID); bool isRider, hasRide;
    EncPart *ep = &(te->Parts[e.cPart]);
    bool isDragon = TMON(e.ep_mID)->isMType(e.ep_mID,MA_DRAGON);
    
    /* (1) Apply Explicitly Specified Templates */
    if (ep->xID2 && ep->xID2 >= 0x01000000) 
      {
        if (enTemplateOk(e,ep->xID2,true))
          enAddTemp(e,ep->xID2);
        else
          e.epFailed = true;
      }
    else if (ep->xID2 && (ep->Flags & EP_ANYTEMP))
      {
        c = 0;
        for (q=0;q!=1;q++) 
          for(i=0;i!=theGame->Modules[q]->szTem;i++)
            {
              TTemplate *tt = &theGame->Modules[q]->QTem[i];
              if (!(tt->TType & ep->xID2))
                continue;
              if (tt->HasFlag(TMF_NOGEN))
                continue;
              tID = theGame->Modules[q]->TemplateID(i);
              if (!enTemplateOk(e, tID))
                continue; 
              if (c >= 510)
                break;
              tmList[c++] = tID;
            }
        if (c && e.isGetMinCR)
          {
            int16 bestCR; rID bestID;
            bestID = 0; bestCR = 100;
            for(i=0;i!=c;i++)
              if (TTEM(tmList[i])->CR.Adjust(TMON(e.ep_mID)->CR) < bestCR)
                { bestCR = TTEM(tmList[i])->CR.Adjust(TMON(e.ep_mID)->CR);
                  bestID = tmList[i]; }
            if (bestID)
              enAddTemp(e,bestID);
          }
        else if (c && e.isGetMaxCR)
          {
            int16 bestCR; rID bestID;
            bestID = 0; bestCR = -100;
            for(i=0;i!=c;i++)
              if (TTEM(tmList[i])->CR.Adjust(TMON(e.ep_mID)->CR) > bestCR)
                { bestCR = TTEM(tmList[i])->CR.Adjust(TMON(e.ep_mID)->CR);
                  bestID = tmList[i]; }
            if (bestID)
              enAddTemp(e,bestID);
          }
        else if (c) 
          enAddTemp(e,tmList[random(c)]);
        else
          e.epFailed = true;
      }  
    else if (ep->xID2)
      {
        e.chList = ep->xID2;
        e.chType = 0;
        e.chCriteria = NULL;
        e.chMaximize = false;
        ReThrow(EV_ENCHOOSE_TEMP,e);
        if (e.chResult)
          enAddTemp(e,e.chResult);
        else
          e.epFailed = true;
      }        
    
    /* (2) Apply universal template */
    
    if (enUniformGet(UNIVERSAL_TEMPLATE)) {
      if (!enTemplateOk(e,enUniformGet(UNIVERSAL_TEMPLATE),true))
        e.epFailed = true;
      enAddTemp(e,enUniformGet(UNIVERSAL_TEMPLATE));
      }
    /* (4) Apply Class Template, if needed */
    
    if (!(ep->Flags & EP_NOXCR))
      if (e.eimXCR < e.epCurrXCR &&
           !(ep->Flags & EP_CLASSED))
        goto SkipClassed;
    
    chance = 0;
    if (ep->Flags & EP_CLASS10)
      chance += 10;
    if (ep->Flags & EP_CLASS25)
      chance += 25;
    if (ep->Flags & EP_CLASS50)
      chance += 50;
    if ((ep->Flags & (EP_CLASSED|EP_MAYCLASS)) ||
        chance >= e.epClassRoll)
      {
        int32 oldXCR = e.eimXCR;
        RetryClass:
        e.chList = CLASS_LIST;
        e.chType = TM_CLASS;
        e.chCriteria = NULL;
        e.chMaximize = false;
        ReThrow(EV_ENCHOOSE_TEMP,e);
        if ((ep->Flags & EP_CLASSED) && !e.chResult)
          {
            e.eimXCR = XCR(XCRtoCR(e.eimXCR)+1);
            if (XCRtoCR(e.eimXCR) < 35)
              goto RetryClass;
          }
        e.eimXCR = oldXCR;
        if (e.chResult)
          enAddTemp(e,e.chResult);
        if ((ep->Flags & EP_CLASSED) && !e.chResult)
          e.epFailed = true;
      }
    
    SkipClassed:
    
    /* (5) Generate Mounts */
    isRider = false;
    if (e.ep_mID && TMON(e.ep_mID)->HasFlag(M_RIDER))
      isRider = true;
    if (e.ep_tID && TTEM(e.ep_tID)->AddsFlag(M_RIDER))
      isRider = true;
    if (e.ep_tID2 && TTEM(e.ep_tID2)->AddsFlag(M_RIDER))
      isRider = true;
    if (e.ep_tID3 && TTEM(e.ep_tID3)->AddsFlag(M_RIDER))
      isRider = true;
    
    hasRide = false;
    if (e.ep_mID && TMON(e.ep_mID)->HasFeat(SK_RIDE))
      hasRide = true;
    if (e.ep_tID && TTEM(e.ep_tID)->HasFeat(SK_RIDE))
      hasRide = true;
    if (e.ep_tID2 && TTEM(e.ep_tID2)->HasFeat(SK_RIDE))
      hasRide = true;
    if (e.ep_tID3 && TTEM(e.ep_tID3)->HasFeat(SK_RIDE))
      hasRide = true;
    
    if (!(ep->Flags & EP_NOXCR))
      if (e.eimXCR < e.epCurrXCR && !isRider &&
           !(ep->Flags & EP_MOUNTED))
        goto SkipMount;
        
    if (isRider || (ep->Flags & EP_MOUNTED) ||
        ((ep->Flags & EP_MOUNTABLE) && !random(2)) ||
        (hasRide && !random(3)))
      {
        e.chSource = 0;
        e.chList = MOUNT_LIST;
        if (e.ep_mID && RES(e.ep_mID)->HasList(MOUNT_LIST))
          e.chSource = e.ep_mID;
        if (e.ep_tID  && RES(e.ep_tID)->HasList(MOUNT_LIST))
          e.chSource = e.ep_tID;
        if (e.ep_tID2 && RES(e.ep_tID2)->HasList(MOUNT_LIST))
          e.chSource = e.ep_tID2;
        if (e.ep_tID3 && RES(e.ep_tID3)->HasList(MOUNT_LIST))
          e.chSource = e.ep_tID3;
        ReThrow(EV_ENGEN_MOUNT,e);
      }
    
    SkipMount:
    
    /* (6) Apply Freaky Template, if appropriate */
    if (!(ep->Flags & EP_NOXCR))
      if (e.eimXCR < e.epCurrXCR)
        goto SkipFreaky;

    if (e.epFreaky >= 7)
      {
        e.chList = 0;
        e.chType = TM_NATURE|TM_DESCENT;
        if (te->HasFlag(NF_SINGLE))
          e.chType |= TM_PLANAR;
        e.chCriteria = NULL;
        e.chMaximize = false;
        ReThrow(EV_ENCHOOSE_TEMP,e);
        if (e.chResult)
          enAddTemp(e,e.chResult);
      }
        
    SkipFreaky:
    
    /* (7) Poison Weapons; Grant Extra Magic Items */
    
    /* (8) Apply Skill Template, if needed */
    if (!(ep->Flags & EP_NOXCR))
      if (e.eimXCR < e.epCurrXCR)
        goto SkipSkilled;
    
    if (ep->Flags & (EP_SKILLED|EP_SKILLMAX|EP_SKILLHIGH))
      {
        e.chList = SKILL_LIST;
        e.chType = TM_SKILL;
        e.chCriteria = NULL;
        if (ep->Flags & EP_SKILLMAX) 
          e.chMaximize = true;
        else if (ep->Flags & EP_SKILLHIGH)
          e.chBestOfTwo = true;
        else
          e.chMaximize = false;
        ReThrow(EV_ENCHOOSE_TEMP,e);
        if (e.chResult)
          enAddTemp(e,e.chResult);
      }
      
    SkipSkilled:
      
      
    /* (9) Give dragons age categories */
    if (isDragon && !hasTempType(e,TM_AGECAT))
      {
        e.chList = 0;
        e.chType = TM_AGECAT;
        e.chCriteria = NULL;
        e.chMaximize = true;
        ReThrow(EV_ENCHOOSE_TEMP,e);
        if (!e.chResult)
          e.chResult = FIND("young");
        enAddTemp(e,e.chResult);
      }
      
    return DONE;
  }  
  
  
EvReturn Map::enChooseTemp(EventInfo &e)
  {
    rID wtList[512], tmList[512], xID;
    int32 tmWeights[512];
    int16 cWeight, nTemp, i, q, x;

    if (TENC(e.enID)->HasFlag(NF_UNIFORM))
      if (e.chResult = enUniformGet((e.chList+e.cPart*10000) | 0x0D000000))
        return DONE;
    
    enCalcCurrPartXCR(e);
    tmWeights[0] = 0;
    if (e.chList && TENC(e.enID)->GetList(e.chList,wtList,511))
      {
        nTemp = 0; cWeight = 10;
        for(i=0;wtList[i] || wtList[i+1];i++)
          {
            if (wtList[i] == -1) // ANY TM_PLANAR
              { i++; Error("ANY in TEMP_LIST not implemented!"); }
            else if (wtList[i] < 0x01000000)
              cWeight = wtList[i];
            else if (enTemplateOk(e, wtList[i]))
              {
                tmList[nTemp++] = wtList[i];
                tmWeights[nTemp] = tmWeights[nTemp-1]+cWeight;
              }
            if (nTemp >= 510)
              break;
          }
      }  
    else
      {
        nTemp = 0;
        for (q=0;q!=1;q++) 
          for(i=0;i!=theGame->Modules[q]->szTem;i++)
            {
              TTemplate *tt = &theGame->Modules[q]->QTem[i];
              if (!(tt->TType & e.chType))
                continue;
              if (tt->HasFlag(TMF_NOGEN))
                continue;
              xID = theGame->Modules[q]->TemplateID(i);
              if (!enTemplateOk(e, xID))
                continue; 
              if (nTemp >= 510)
                break;
              tmList[nTemp++] = xID;
              tmWeights[nTemp] = tmWeights[nTemp-1]+1;

            }
      }
    if (!nTemp) {
      e.chResult = 0;
      return DONE;
      }
    if (e.isGetMinCR)
      {
        int16 bestCR; rID bestID;
        bestID = 0; bestCR = 100;
        for(i=0;i!=nTemp;i++)
          if (TTEM(tmList[i])->CR.Adjust(TMON(e.ep_mID)->CR) < bestCR)
            { bestCR = TTEM(tmList[i])->CR.Adjust(TMON(e.ep_mID)->CR);
              bestID = tmList[i]; }        
        e.chResult = bestID;
        return DONE;
      }
    else if (e.isGetMaxCR)
      {
        int16 bestCR; rID bestID;
        bestID = 0; bestCR = -100;
        for(i=0;i!=nTemp;i++)
          if (TTEM(tmList[i])->CR.Adjust(TMON(e.ep_mID)->CR) > bestCR)
            { bestCR = TTEM(tmList[i])->CR.Adjust(TMON(e.ep_mID)->CR);
              bestID = tmList[i]; }        
        e.chResult = bestID;
        return DONE;
      }
      
      
    if (e.chMaximize)
      {          
        int16 bestCR = -100, n; rID highlist[32];
        for (i=0;i!=nTemp;i++)
          if (TTEM(tmList[i])->CR.Adjust(TMON(e.ep_mID)->CR) > bestCR)
            bestCR = TTEM(tmList[i])->CR.Adjust(TMON(e.ep_mID)->CR);
        for (i=0,n=0;i!=nTemp;i++)
          if (TTEM(tmList[i])->CR.Adjust(TMON(e.ep_mID)->CR) >= bestCR)
            highlist[n++] = tmList[i];
        if (n)
          e.chResult = highlist[random(n)];
        else
          e.chResult = 0;
        enUniformAdd(((e.chList+e.cPart*10000) | 0x0D000000), e.chResult);
        ASSERT(e.chResult >= 0x01000000);
        return DONE;
      }  
    else if (e.chBestOfTwo)
      {
        rID aID, bID;
        aID = tmList[random(nTemp)];
        bID = tmList[random(nTemp)];
        if (TTEM(aID)->CR.Adjust(0) >
            TTEM(bID)->CR.Adjust(0))
          e.chResult = aID;
        else
          e.chResult = bID;
        ASSERT(e.chResult >= 0x01000000);
        enUniformAdd(((e.chList+e.cPart*10000) | 0x0D000000), e.chResult);
        return DONE;
      }     
    x = random(tmWeights[nTemp]);
    for (i=0;tmWeights[i+1];i++)
      if (tmWeights[i+1]>x)
        {
          e.chResult = tmList[i];
          ASSERT(e.chResult >= 0x01000000);
          enUniformAdd(((e.chList+e.cPart*10000) | 0x0D000000), e.chResult);
          return DONE;
        }
    e.chResult = 0;
    return DONE;
  }


  
bool Map::enTemplateOk(EventInfo &e, rID tID, bool force)
  {
    /*
    1) Is the basemon mID the appropriate MTYPE for the Template?
       _Only test this if this is the first template being applied._
    2) Would the adjusted CR violate the desired XCR?
    3) Does the template get disqualified on freak factor grounds?
    4) Does the template do M_IALIGN and get disqualified based on
       alignment conflict?
    4) If we get this far, create the Monster object, apply any
       prior templates and then call EV_ISTARGET for the template
       in question. We can use the same monster object to test all
       the templates in a given call to listTemplates.
    */
    
    int32 newXCR;
    TTemplate *tt = TTEM(tID);
    

    if (force)
      goto MinTest;
    
    if (!TMON(e.ep_mID)->isMType(e.ep_mID,tt->ForMType))
      return false;
    
    newXCR = e.epCurrXCR -
             XCR(e.ep_monCR) +
             XCR(tt->CR.Adjust(e.ep_monCR));
    if (newXCR > e.eimXCR)
      return false;
      
    /* Add freak factor tests here later */
    
    if (tt->AddsFlag(M_IALIGN))
      if (AlignConflict(ResAlign(tID),e.enAlign,true))
        return false;        
      
    if (e.chCriteria)
      if ((*e.chCriteria)(e,tID) == false)
        return false;
      
    {
      rID old_eID = e.eID;
      e.eID = tID;
      if (TENC(e.enID)->Event(e,e.enID,EV_CRITERIA) == CANNOT_CAST_IT)
        { e.eID = old_eID; return false; }
      e.eID = old_eID;
    }
      
    {
      MinTest:
      Monster *mn = new Monster(e.ep_mID);
      if (e.ep_tID)
        mn->AddTemplate(e.ep_tID);
      if (e.ep_tID2)
        mn->AddTemplate(e.ep_tID2);
      if (e.ep_tID3)
        mn->AddTemplate(e.ep_tID3);
      mn->CalcValues();
      if (!mn->CanAddTemplate(tID))
        { delete mn; return false; }
      delete mn;
    }
    return true;
  }

EvReturn Map::enBuildMon(EventInfo &e)
  {
    Monster *mn, *mt;
    EncMember *em = &EncMem[e.cMember];
    mt = NULL;
    if (!em->mID)
      return ABORT;
      
    /* HACKFIX */
    if (e.cMember >= 5)
      return ABORT;
      
      
    mn = new Monster(em->mID);
    mn->PartyID = e.enPartyID;
    em->hMon = mn->myHandle;
    TMON(em->mID)->GrantGear(mn,em->mID,true);
    if (em->tID && em->tID > 0x01000000) {
      mn->CalcValues();
      mn->AddTemplate(em->tID);
      #ifdef DEBUG
      ASSERT(mn->HasEffStati(TEMPLATE,em->tID));
      #endif
      TTEM(em->tID)->GrantGear(mn,em->tID,true);
      TTEM(em->tID)->PEvent(EV_BIRTH,mn,em->tID);
      }
    if (em->tID2 && em->tID2 > 0x01000000) {
      mn->CalcValues();
      mn->AddTemplate(em->tID2);
      #ifdef DEBUG
      ASSERT(mn->HasEffStati(TEMPLATE,em->tID2));
      #endif
      TTEM(em->tID2)->GrantGear(mn,em->tID2,true);
      TTEM(em->tID2)->PEvent(EV_BIRTH,mn,em->tID2);
      }
    if (em->tID3 && em->tID3 > 0x01000000) {
      mn->CalcValues();
      mn->AddTemplate(em->tID3);
      #ifdef DEBUG
      ASSERT(mn->HasEffStati(TEMPLATE,em->tID3));
      #endif
      TTEM(em->tID3)->GrantGear(mn,em->tID3,true);
      TTEM(em->tID3)->PEvent(EV_BIRTH,mn,em->tID3);
      }      
    TMON(em->mID)->PEvent(EV_BIRTH,mn,em->mID);
    mn->MonsterGear();
    if ((e.enFlags & EN_DUNGEON) && random(100)+1 <= e.enSleep &&
        !mn->ResistLevel(AD_SLEE) && !em->hmID)
      mn->GainPermStati(SLEEPING,NULL,SS_MISC,SLEEP_NATURAL);
    /* LATER...
    if (mGroups[i].Flags & MGF_POISON) {
      xID = theGame->GetEffectID(PUR_POISON,0,max(3,enCR));
      for(it = mn->FirstInv();it;it = mn->NextInv())
        if (it->isType(T_WEAPON) || it->isType(T_MISSILE))
          if (it->HasIFlag(WT_PIERCING) || it->HasIFlag(WT_SLASHING)) 
            it->GainPermStati(POISONED,NULL,SS_MISC,3,0,xID);
      }
    */
    
    if (em->hmID)
      {
        mt = new Monster(em->hmID);
        mt->PartyID = e.enPartyID;
        TMON(em->hmID)->GrantGear(mt,em->hmID,true);
        if (em->htID && em->htID > 0x01000000) {
          mt->CalcValues();
          mt->AddTemplate(em->htID);
          #ifdef DEBUG
          ASSERT(mt->HasEffStati(TEMPLATE,em->htID));
          #endif
          TTEM(em->htID)->GrantGear(mt,em->htID,true);
          TTEM(em->htID)->PEvent(EV_BIRTH,mt,em->htID);
          }
        if (em->htID2 && em->htID2 > 0x01000000) {
          mt->CalcValues();
          mt->AddTemplate(em->htID2);
          #ifdef DEBUG
          ASSERT(mt->HasEffStati(TEMPLATE,em->htID2));
          #endif
          TTEM(em->htID2)->GrantGear(mt,em->htID2,true);
          TTEM(em->htID2)->PEvent(EV_BIRTH,mt,em->htID2);
          }
        TMON(em->hmID)->PEvent(EV_BIRTH,mt,em->hmID);
        mt->MonsterGear();
        mn->GainPermStati(MOUNTED,mt,SS_MISC,0,0);
        mt->GainPermStati(MOUNT,mn,SS_MISC,0,0);
        mt->Initialize();
      }
      
    Creature *old_actor = e.EActor;
    e.EActor = mn;
    ReThrow(EV_ENGEN_ALIGN, e);
    e.EActor = old_actor;
    em->Align = mn->getAlignment();
      
    if (e.enFlags & (EN_SUMMON|EN_CREATOR|EN_ILLUSION)) {
      if (!(e.enFlags & EN_HOSTILE))
        mn->PartyID = e.EActor->PartyID;
      Creature *true_leader = NULL;
      if (e.EActor)
        true_leader = e.EActor->getLeader();
      if (!true_leader)
        true_leader = e.EActor;
      if (e.enFlags & EN_SUMMON) {
        mn->GainTempStati(SUMMONED, e.EActor,e.vDuration, SS_MISC,
                            0, 0, e.eID, e.vCasterLev);
        if (mt)
          mt->GainTempStati(SUMMONED, e.EActor,e.vDuration, SS_MISC,
                              0, 0, e.eID, e.vCasterLev);
          
        if (true_leader && !(e.enFlags & EN_HOSTILE)) {
          mn->ts.addCreatureTarget(true_leader,TargetSummoner);
          if (mt)
            mt->ts.addCreatureTarget(true_leader,TargetSummoner);
          }
        }
      else if (e.enFlags & EN_ILLUSION) {
        mn->GainTempStati(ILLUSION, e.EActor, e.vDuration, SS_ENCH,
          e.saveDC, 0, e.eID, e.vCasterLev);
        e.EActor->GainPermStati(DISBELIEVED,mn,SS_MISC);
        if (mt) {
          mt->GainTempStati(ILLUSION, e.EActor, e.vDuration, SS_ENCH,
            e.saveDC, 0, e.eID, e.vCasterLev);
          e.EActor->GainPermStati(DISBELIEVED,mn,SS_MISC);
          if (true_leader) 
            mt->ts.addCreatureTarget(true_leader,TargetMaster);
          }
        if (true_leader)
          mn->ts.addCreatureTarget(true_leader,TargetMaster);
        }
      else {
        if (true_leader && !(e.enFlags & EN_HOSTILE)) {
          mn->ts.addCreatureTarget(true_leader,TargetMaster);
          if (mt)
            mt->ts.addCreatureTarget(true_leader,TargetMaster);
          }
        }
        
      if (e.EActor && e.EActor->isPlayer() && !(e.enFlags & EN_HOSTILE)) {
        mn->MakeCompanion(e.EPActor,PHD_MAGIC);
        if (mt)
          mt->MakeCompanion(e.EPActor,PHD_MAGIC);
        }
      else {
        mn->PartyID = e.EActor->PartyID;
        if (mt)
          mt->PartyID = e.EActor->PartyID;
        }
        
      if (e.enFlags & EN_HOSTILE) {
        mn->TurnHostileTo(e.EActor);
        if (mt)
          mt->TurnHostileTo(e.EActor);
        }
        
      }
      
    CandidateCreatures[e.cMember] = mn;
    Candidates[e.cMember] = mn->myHandle;
  
    /*if (mn->mID == FIND("grippli"))
      __asm int 3;*/
  
    if (e.enFlags & EN_NOPLACE)
      ;
    else if (e.enFlags & EN_ANYOPEN) {
      int16 Tries = 0, j;
      
      if (e.enIsFormation)
        if (e.EXVal)
          goto PlaceAtSpot;
      
      rID Terrains[256];
      for (int i=0;i!=min(256,OpenC);i++)
        Terrains[i] = TerrainAt(OpenX[i],OpenY[i]);
      
      Retry:
      if (Tries >= 50)
        { 
          /*
          String s;
          s = XPrint("Monster <Obj> deleted due to lack of suitable terrain at (<Num>,<Num>).",
              mn, OpenX[j],OpenY[j]);
          for (int i=0;i!=OpenC;i++)
            s += Format("\n[%d,%d]: %s",
                   OpenX[i],OpenY[i],(const char*)
                   NAME(TerrainAt(OpenX[i],OpenY[i])));
          theGame->GetPlayer(0)->AddJournalEntry(s);
          s.Empty();
          */
          mn->Remove(true); return ABORT; }
      
      /* Place aquatic monsters in water, and other monsters on dry
         land. No exceptions -- even amphibians start in their favored
         terrain. */  
      j = random(OpenC);
      
      if (mn->HasMFlag(M_AMPHIB))
        ;
      else if (mn->isMType(MA_AQUATIC)) {
        if (!TTER(TerrainAt(OpenX[j], OpenY[j]))->HasFlag(TF_WATER))
          { 
            //if (GlyphAt(OpenX[j],OpenY[j]) & 0xFF == GLYPH_WATER)
            //  __asm int 3; 
            Tries++; 
            goto Retry; 
          }
        }
      else {
        if (TTER(TerrainAt(OpenX[j], OpenY[j]))->HasFlag(TF_WATER))
          { Tries++; goto Retry; }
        }
      
      if (TTER(TerrainAt(OpenX[j],OpenY[j]))->HasFlag(TF_STICKY))
        if (!mn->isMType(MA_SPIDER))
          { Tries++; goto Retry; }
      if (TTER(TerrainAt(OpenX[j],OpenY[j]))->HasFlag(TF_FALL))
        if (!mn->isAerial())
          { Tries++; goto Retry; }
      
      

      /* Don't place anything on dangerous terrain, unless it's deep
          water and an aquatic monster. */
      if (TTER(TerrainAt(OpenX[j],OpenY[j]))->HasFlag(TF_WARN) &&
          !TTER(TerrainAt(OpenX[j],OpenY[j]))->HasFlag(TF_WATER) &&
          !TTER(TerrainAt(OpenX[j],OpenY[j]))->HasFlag(TF_FALL) &&
          !TTER(TerrainAt(OpenX[j],OpenY[j]))->HasFlag(TF_STICKY)) {
        // ww: terrain with TF_WARN now support a special event that says
        // if it is OK to put the monster there ...
        // Is this the wrong way to call PEvent?!
        if (TTER(TerrainAt(OpenX[j],OpenY[j]))->
            PEvent(EV_MON_CONSIDER,mn,mn->mID) == ABORT)
          { Tries++; goto Retry; }
        }
      Feature * f;
      for (f=FFeatureAt(OpenX[j],OpenY[j]);f;f=NFeatureAt(OpenX[j],OpenY[j]))
        if (f && (TFEAT(f->fID)->PEvent(EV_MON_CONSIDER,mn,f,mn->mID) 
                == ABORT)) 
          { Tries++; goto Retry; }
      
      mn->PlaceAt(this,OpenX[j],OpenY[j]);
      /*ASSERT(mn->x > 0 && mn->y > 0);
      mn->PlaceAt(this,OpenX[j],OpenY[j]);*/
      if (!(e.enFlags & EN_VAULT))
        mn->Initialize(true);
      if (e.enIsFormation)
        {
          e.EXVal = OpenX[j];
          e.EYVal = OpenY[j];
        }
      }
    else {
      PlaceAtSpot:
      mn->PlaceAt(this, e.EXVal, e.EYVal);
      if (mn->x == -1) 
        theGame->GetPlayer(0)->AddJournalEntry(
          XPrint("Failed to place <Obj> at <Num>,<Num>.",mn,e.EXVal,e.EYVal));
      /*ASSERT(mn->x > 0 && mn->y > 0);
      mn->Flags &= ~F_DELETE;
      mn->PlaceAt(this, e.EXVal, e.EYVal);*/
      if (!(e.enFlags & EN_VAULT))
        mn->Initialize(true);
      }
      
    mn->GainPermStati(ENCOUNTER,NULL,SS_MISC,e.cPart,e.enDesAmt,e.enID);
    
    Player *p;
    if (mn->ChallengeRating() > max(2,e.enCR))
      if (p = theGame->GetPlayer(0)) {
        String str;
        str = XPrint("OOD monster <Obj> (CR <Num>) generated on "
          "dlev <Num> by Encounter '<Res>'", mn, mn->ChallengeRating(), Depth, e.enID);
        if (e.eID)
          str += XPrint(" and the eID '<Res>'", e.eID);
        if (e.enRegID && RES(e.enRegID)->HasList(ENCOUNTER_LIST))
          str += XPrint(" and the ENCOUNTER_LIST from '<Res>'", e.enRegID);
        if (e.enConstraint && e.enConstraint >= 0x01000000)
          str += XPrint(" with the constraint '<Res>'", e.enConstraint);
        else if (e.enConstraint)
          str += XPrint(" with the constraint '<Str>'", Lookup(MTypeNames,e.enConstraint));
        str += ".";
        if (e.enFlags & EN_OODMON)
          str += " [EN_OODMON]";
        if (e.enFlags & EN_SINGLE)
          str += " [EN_SINGLE]";
        if (e.enFlags & EN_AQUATIC)
          str += " [EN_AQUATIC]";
        if (e.enFlags & EN_STREAMER)
          str += " [EN_STREAMER]";
        if (e.enFlags & EN_CREATOR)
          str += " [EN_CREATOR]";
        if (e.enFlags & EN_SUMMON)
          str += " [EN_SUMMON]";
        if (e.enFlags & EN_VAULT)
          str += " [EN_VAULT]";
        if (e.enFlags & EN_DUNREGEN)
          str += " [EN_DUNREGEN]";          
        p->AddJournalEntry(str);
        }
      
    return NOTHING;
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

bool AlignConflict(uint16 al1, uint16 al2, bool strict)
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
    if (strict) {
      if ((al2 & AL_EVIL) && !(al1 & AL_EVIL))
        return true;
      if ((al2 & AL_CHAOTIC) && !(al1 & AL_CHAOTIC))
        return true;
      if ((al2 & AL_LAWFUL) && !(al1 & AL_LAWFUL))
        return true;
      if ((al2 & AL_GOOD) && !(al1 & AL_GOOD))
        return true;
      }  
    return false;
  }

void UpdateAlignRestrict(EventInfo &e)
  {
    Monster *mn = new Monster(e.ep_mID);
    if (e.ep_tID)
      mn->AddTemplate(e.ep_tID);
    if (e.ep_tID2)
      mn->AddTemplate(e.ep_tID2);
    if (e.ep_tID3)
      mn->AddTemplate(e.ep_tID3);
      
    if (mn->HasMFlag(M_IALIGN))
      {
        if (mn->HasMFlag(M_GOOD))
          e.enAlign |= AL_NONEVIL;
        if (mn->HasMFlag(M_EVIL))
          e.enAlign |= AL_NONGOOD;
        if (mn->HasMFlag(M_CHAOTIC))
          e.enAlign |= AL_NONLAWFUL;
        if (mn->HasMFlag(M_LAWFUL))
          e.enAlign |= AL_NONCHAOTIC;
      }
    if (mn->HasMFlag(M_GOOD))
      e.enDriftGE -= 2;
    if (mn->HasMFlag(M_EVIL))
      e.enDriftGE += 2;
    if (mn->HasMFlag(M_LAWFUL))
      e.enDriftLC -= 2;
    if (mn->HasMFlag(M_CHAOTIC))
      e.enDriftLC += 2;
    delete mn;
  }

EvReturn Map::enGenAlign(EventInfo &e)
  {
    int16 align, i;
    Creature *mn = e.EActor;
    mn->RemoveStati(ALIGNMENT);
    
    if (!mn->isMType(MA_SAPIENT))
      return DONE;
    if (mn->HasMFlag(M_IALIGN))
      return DONE;
    
    if (mn->isMType(MA_DRAGON) && random(3))
      if (!AlignConflict(mn->getAlignment(),e.enAlign,true))
        return DONE;
    else if (mn->isMType(MA_GOBLINOID) && random(2))
      if (!AlignConflict(mn->getAlignment(),e.enAlign,true))
        return DONE;
     
    align = 0;
    if (e.enAlign & AL_EVIL)
      align |= AL_EVIL;
    else if (e.enAlign & AL_GOOD)
      align |= AL_GOOD;
    else if ((e.enAlign & AL_NONGOOD) &&
             (e.enAlign & AL_NONEVIL))
      align |= 0;
    else if (e.enAlign & AL_NONGOOD) {
      if (mn->HasMFlag(M_EVIL))
        align |= random(5) ? AL_EVIL : 0;
      else if (mn->HasMFlag(M_GOOD))
        align |= random(2) ? 0 : AL_EVIL;
      else
        align |= random(3) ? AL_EVIL : 0;
      }
    else if (e.enAlign & AL_NONEVIL) {
      if (mn->HasMFlag(M_EVIL))
        align |= random(5) ? AL_EVIL : 0;
      else if (mn->HasMFlag(M_GOOD))
        align |= random(2) ? 0 : AL_EVIL;
      else
        align |= random(3) ? AL_EVIL : 0;
      }
    else if (mn->HasMFlag(M_EVIL))
      align |= (random(100)+1 < 70) ? AL_EVIL :
               (random(30)+1 < 23) ? 0 : AL_GOOD;
    else if (mn->HasMFlag(M_GOOD))
      align |= (random(100)+1 < 70) ? AL_GOOD :
               (random(30)+1 < 23) ? 0 : AL_EVIL;
    else
      align |= (random(100)+1 < 70) ? 0 :
               (random(30)+1 < 15) ? AL_GOOD : AL_EVIL;
    
    if (e.enAlign & AL_LAWFUL)
      align |= AL_LAWFUL;
    else if (e.enAlign & AL_CHAOTIC)
      align |= AL_CHAOTIC;
    else if ((e.enAlign & AL_NONLAWFUL) &&
             (e.enAlign & AL_NONCHAOTIC))
      align |= 0;
    else if (e.enAlign & AL_NONLAWFUL) {
      if (mn->HasMFlag(M_CHAOTIC))
        align |= random(5) ? AL_CHAOTIC : 0;
      else if (mn->HasMFlag(M_LAWFUL))
        align |= random(2) ? 0 : AL_CHAOTIC;
      else
        align |= random(3) ? AL_CHAOTIC : 0;
      }
    else if (e.enAlign & AL_NONCHAOTIC) {
      if (mn->HasMFlag(M_LAWFUL))
        align |= random(5) ? AL_LAWFUL : 0;
      else if (mn->HasMFlag(M_CHAOTIC))
        align |= random(2) ? 0 : AL_LAWFUL;
      else
        align |= random(3) ? AL_LAWFUL : 0;
      }
    else if (mn->HasMFlag(M_LAWFUL))
      align |= (random(100)+1 < 70) ? AL_LAWFUL :
               (random(30)+1 < 23) ? 0 : AL_CHAOTIC;
    else if (mn->HasMFlag(M_CHAOTIC))
      align |= (random(100)+1 < 70) ? AL_CHAOTIC :
               (random(30)+1 < 23) ? 0 : AL_LAWFUL;
    else
      align |= (random(100)+1 < 70) ? 0 :
               (random(30)+1 < 15) ? AL_LAWFUL : AL_CHAOTIC;
    
    if (align & AL_GOOD)
      e.enAlign |= AL_NONEVIL;
    if (align & AL_EVIL)
      e.enAlign |= AL_NONGOOD;
    
    mn->GainPermStati(ALIGNMENT,NULL,SS_MISC,align);

    return DONE;
  }

  
/***************************************************************
 *                    ITEM GENERATION CODE                     *
 ***************************************************************/
 

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

