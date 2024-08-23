/* RES.CPP -- See the Incursion LICENSE file for copyright information.

     Implements the resource system, and the code used to change 
   resource IDs (rID) into pointers to the relevant derived type 
   of Resource object. Also, code for selecting a random resource
   (monster, map, item, etc.) within a given set of parameters,
   and code for managing the R/W memory assigned to each resource
   for a given Player (i.e., the monster memory, list of potion
   types known, etc.)

     String Resource::GetName(uint16 fl)
     bool Module::inGroup(hObj h)
     int16 Module::SpellNum(Resource *r)
     Resource* Module::GetResource(rID r)
     rID Module::FindResource(const char*n)
     Resource* Game::Get(rID r)
     rID Game::Find(const char*str)
     rID Game::SpellID(uint16 sp)
     uint16 Game::LastSpell()
     rID Game::GetItemID(int16 Purpose, int8 minlev, int8 maxlev, int8 IType)
     rID Game::GetMonID(int16 Purpose, int8 minlev, int8 maxlev, int8 Depth, int16 MType)
     rID Game::GetTempID(uint16 Types, rID mID, int8 maxCR)
     rID Game::GetEffectID(int16 Purpose, int8 minlev, int8 maxlev, int8 Source)
     rID Game::RandomResource(uint8 RType, int8 Level, int8 Range)
     rID Module::RandomResource(uint8 RType, int8 Level, int8 Range)
     void* Game::GetMemory(rID xID,Player *pl)
     uint32 Module::GetMemoryPtr(rID xID,int8 pn)
     int16 ResourceLevel(rID xID)
     bool ResourceHasFlag(rID xID, int16 fl)

     void Game::Cleanup()
     Map::~Map()                   
     Module::~Module()

*/

#include "Incursion.h"

#ifdef WIN32
//#include <typeinfo.h>
#endif

rID Candidates[2048]; 
int16 nCandidates;

Module* Game::Modules[MAX_MODULES];
Thing * Game::DestroyQueue[20480];
uint16  Game::DestroyCount;
Thing * Game::cDestroyQueue[20480];
uint16  Game::cDestroyCount;

String & Resource::GetName(uint16 fl)
  {
    String *n;
    n = tmpstr(theGame->Modules[ModNum]->QTextSeg + Name);
    if (n->strchr(';'))
      (*n) = n->Left(n->strchr(';'));
    return *n;
  }

bool Module::inGroup(hObj h)
  { return h == myHandle; }

int16 Module::SpellNum(Resource *r)
  {
    if (r >= ((TEffect*)QEff) && r < (((TEffect*)QEff) + szEff))
      return (((TEffect*)r) - ((TEffect*)QEff));
    return -1;
  }

int16 Module::GodNum(Resource *r)
  {
    if (r >= ((TGod*)QGod) && r < (((TGod*)QGod) + szGod))
      return (((TGod*)r) - ((TGod*)QGod));
    return -1;
  }

int16 Module::TemplateNum(Resource *r)
  {
    if (r >= ((TTemplate*)QTem) && r < (((TTemplate*)QTem) + szTem))
      return (((TTemplate*)r) - ((TTemplate*)QTem));
    return -1;
  }

/* The Core Incursion module has around 2500 resources at this time
 * (Sun Nov 28 10:11:53 PST 2004) and GetResouce(rID) is one of the
 * top functions in the performance profile. No reason not to cache it.
 */
inline Resource* Module::GetResource(rID r)
  {
    Resource * res; 

   if (r && (GetResourceIndex[r % GET_RESOURCE_CACHE_SIZE] == r)) 
      return GetResourceCache[r % GET_RESOURCE_CACHE_SIZE];

    res = __GetResource(r);
    GetResourceCache[r % GET_RESOURCE_CACHE_SIZE] = res;
    GetResourceIndex[r % GET_RESOURCE_CACHE_SIZE] = r;
    return res;
  } 

inline Resource* Module::__GetResource(rID r_) {
    uint32 r = r_ & 0x00FFFFFF;

    if (r < (uint32)szMon)
      return &QMon[r];
    else
      r -= szMon;

    if (r < (uint32)szItm)
      return &QItm[r];
    else
      r -= szItm;

    if (r < (uint32)szFea)
      return &QFea[r];
    else
      r -= szFea;

    if (r < (uint32)szEff)
      return &QEff[r];
    else
      r -= szEff;

    if (r < (uint32)szArt)
      return &QArt[r];
    else
      r -= szArt;

    if (r < (uint32)szQue)
      return &QQue[r];
    else
      r -= szQue;

    if (r < (uint32)szDgn)
      return &QDgn[r];
    else
      r -= szDgn;

    if (r < (uint32)szRou)
      return &QRou[r];
    else
      r -= szRou;

    if (r < (uint32)szNPC)
      return &QNPC[r];
    else
      r -= szNPC;

    if (r < (uint32)szCla)
      return &QCla[r];
    else
      r -= szCla;

    if (r < (uint32)szRac)
      return &QRac[r];
    else
      r -= szRac;

    if (r < (uint32)szDom)
      return &QDom[r];
    else
      r -= szDom;

    if (r < (uint32)szGod)
      return &QGod[r];
    else
      r -= szGod;

    if (r < (uint32)szReg)
      return &QReg[r];
    else
      r -= szReg;

    if (r < (uint32)szTer)
      return &QTer[r];
    else
      r -= szTer;

    if (r < (uint32)szTxt)
      return &QTxt[r];
    else
      r -= szTxt;

    if (r < (uint32)szVar)
      return &QVar[r];
    else
      r -= szVar;

    if (r < (uint32)szTem)
      return &QTem[r];
    else
      r -= szTem;

    if (r < (uint32)szFla)
      return &QFla[r];
    else
      r -= szFla;

    if (r < (uint32)szBev)
      return &QBev[r];
    else
      r -= szBev;

    if (r < (uint32)szEnc)
      return &QEnc[r];
    else
      r -= szEnc;

    /*
    if (r < szQue)
      return &MQue[r];
    */          
    
    return NULL;

  }

rID Module::FindResource(const char*n)
  {
    int16 i;
    for(i=0;i!=szMon;i++)
      if (stricmp(n,GetText(QMon[i].Name))==0)
        return MonsterID(i);

    for(i=0;i!=szItm;i++)
      if (stricmp(n,GetText(QItm[i].Name))==0)
        return ItemID(i);

    for(i=0;i!=szFea;i++)
      if (stricmp(n,GetText(QFea[i].Name))==0)
        return FeatureID(i);

    for(i=0;i!=szEff;i++)
      if (stricmp(n,GetText(QEff[i].Name))==0)
        return EffectID(i);

    for(i=0;i!=szArt;i++)
      if (stricmp(n,GetText(QArt[i].Name))==0)
        return ArtifactID(i);

    for(i=0;i!=szQue;i++)
      if (stricmp(n,GetText(QQue[i].Name))==0)
        return QuestID(i);

    for(i=0;i!=szDgn;i++)
      if (stricmp(n,GetText(QDgn[i].Name))==0)
        return DungeonID(i);

    for(i=0;i!=szRou;i++)
      if (stricmp(n,GetText(QRou[i].Name))==0)
        return RoutineID(i);

    for(i=0;i!=szNPC;i++)
      if (stricmp(n,GetText(QNPC[i].Name))==0)
        return NPCID(i);

    for(i=0;i!=szCla;i++)
      if (stricmp(n,GetText(QCla[i].Name))==0)
        return ClassID(i);

    for(i=0;i!=szRac;i++)
      if (stricmp(n,GetText(QRac[i].Name))==0)
        return RaceID(i);

    for(i=0;i!=szDom;i++)
      if (stricmp(n,GetText(QDom[i].Name))==0)
        return DomainID(i);

    for(i=0;i!=szGod;i++)
      if (stricmp(n,GetText(QGod[i].Name))==0)
        return GodID(i);

    for(i=0;i!=szReg;i++)
      if (stricmp(n,GetText(QReg[i].Name))==0)
        return RegionID(i);

    for(i=0;i!=szTer;i++)
      if (stricmp(n,GetText(QTer[i].Name))==0)
        return TerrainID(i);

    for(i=0;i!=szTxt;i++)
      if (stricmp(n,GetText(QTxt[i].Name))==0)
        return TextID(i);

    for(i=0;i!=szVar;i++)
      if (stricmp(n,GetText(QVar[i].Name))==0)
        return VariableID(i);

    for(i=0;i!=szTem;i++)
      if (stricmp(n,GetText(QTem[i].Name))==0)
        return TemplateID(i);

    for(i=0;i!=szFla;i++)
      if (stricmp(n,GetText(QFla[i].Name))==0)
        return FlavorID(i);

    for(i=0;i!=szBev;i++)
      if (stricmp(n,GetText(QBev[i].Name))==0)
        return BehaviourID(i);

    for(i=0;i!=szEnc;i++)
      if (stricmp(n,GetText(QEnc[i].Name))==0)
        return EncounterID(i);


    return 0;
  }

Resource* Game::Get(rID xID)
	{
    if (!xID) return NULL;
    ASSERT(Modules[(xID >> 24)-1]);
    return Modules[(xID >> 24)-1]->GetResource(xID);
	}


rID Game::Find(const char*str)
	{
    rID xID; int16 i;
    
    for (i=0;i!=szFindCache;i++)
      if (stricmp(FindCache[i].str,str) == 0)
        return FindCache[i].xID;
    
    for(int16 q=0;q!=MAX_MODULES;q++)
      if (Modules[q])
        if (xID = Modules[q]->FindResource(str))
          {
            memmove(&FindCache[1],&FindCache[0],sizeof(__FindCache)*127);
            szFindCache = std::min(127,szFindCache+1);
            strncpy(FindCache[0].str,str,31);
            FindCache[0].xID = xID;
            return xID;
          }
    return 0;
	}



rID Game::SpellID(uint16 sp)
  {
    /* HUGE KLUDGE NEEDS FIXING!!! */
    return Modules[0]->EffectID(sp);
  }

rID Game::GodID(uint16 sp)
  {
    return Modules[0]->GodID(sp);
  }
  
rID Game::TemplateID(uint16 sp)
  {
    return Modules[0]->TemplateID(sp);
  }

uint16 Game::LastSpell()
  {
    /* HUGE KLUDGE NEEDS FIXING!!! */
    return Modules[0]->szEff;
  }

uint16 Game::LastGod()
  {
    /* HUGE KLUDGE NEEDS FIXING!!! */
    return Modules[0]->szGod;
  }

uint16 Game::LastTemplate()
  {
    /* HUGE KLUDGE NEEDS FIXING!!! */
    return Modules[0]->szTem;
  }



rID Game::GetItemID(int16 Purpose, int8 minlev, int8 maxlev, int8 IType)
  {
    int16 i,q,n; n = 0; 
    bool isListing = false;
    if (Purpose >= PUR_LISTING) {
      Purpose -= PUR_LISTING;
      isListing = true;
      }
    for (q=0;q!=1;q++) /* Only one Module at the moment */
      for(i=0;i!=Modules[q]->szItm;i++)
        {
          if ((Modules[q]->QItm[i].IType != IType) && IType > 0)
            continue;
          if (Modules[q]->QItm[i].HasFlag(IT_NOGEN))
            continue;
          if (LevelAdjust(Modules[q]->QItm[i].Level,0,1) > maxlev)
            continue;
          if (LevelAdjust(Modules[q]->QItm[i].Level,0,5) < minlev)
            continue;
          if (Purpose == PUR_STAPLE)
            if (!Modules[q]->QItm[i].HasFlag(IT_STAPLE))
              continue;
          if (Purpose == PUR_MUNDANE || Purpose == PUR_MINOR || 
                Purpose == PUR_MAJOR)
            if (Modules[q]->QEff[i].HasFlag(IT_MAGICAL))
              continue;
          if (Purpose == PUR_MINOR || Purpose == PUR_MAJOR)
            {
              if (Modules[q]->QItm[i].IType == T_BOOK)
                continue;
              if (Purpose == PUR_MINOR)
                if (!MaterialIsOrganic(Modules[q]->QItm[i].Material))
                  continue;
            }
          if (Purpose == PUR_CRAFT)
            { 
              if (Modules[q]->QItm[i].IType == T_BOOK ||
                  Modules[q]->QItm[i].IType == T_COIN ||
                  Modules[q]->QItm[i].IType == T_TOOL ||
                  Modules[q]->QItm[i].IType == T_FOOD)
                continue;
              if (Modules[q]->QItm[i].Material == MAT_FORCE ||
                  Modules[q]->QItm[i].Material == MAT_ADAMANT ||
                  Modules[q]->QItm[i].Material == MAT_MITHRIL ||
                  Modules[q]->QItm[i].Material == MAT_DARKWOOD ||
                  Modules[q]->QItm[i].Material == MAT_IRONWOOD)
                continue;
            }
          Candidates[n++] = Modules[q]->ItemID(i);
        }
    Candidates[n] = 0;
    if (!n)
      return 0;
    return Candidates[random(n)];
  }

rID Game::GetMonID(int16 Purpose, int8 minlev, int8 maxlev, int8 Depth, int32 MType) {
    int16 i,q,n,j,k, rarity;
    bool isListing;

    isListing = false;
    if (Purpose >= PUR_LISTING) {
        Purpose -= PUR_LISTING;
        isListing = true;
    }

Retry:
    n = 0; 
    for (q=0; q!=1; q++) /* Only one Module at the moment */
        for(i=0; i!=Modules[q]->szMon; i++) {
            rID mID = Modules[q]->MonsterID(i);
            if (MType > 0 && MType < 100000  && !Modules[q]->QMon[i].isMType(mID,MType))
                continue;
            /* Kludge for AcquisitionPrompt */
            if (MType >= 100000 && (GLYPH_ID_VALUE(Modules[q]->QMon[i].Image) != (MType-100000)))
                continue;
            if (Modules[q]->QMon[i].HasFlag(M_AQUATIC))
                if (MType != MA_AQUATIC && Purpose != PUR_ANYMON)
                    continue;
            if (Modules[q]->QMon[i].HasFlag(M_NOGEN) && Purpose != PUR_ANYMON)
                continue;
            if (Modules[q]->QMon[i].HasFlag(M_NOPOLY) && Purpose == PUR_POLY)
                continue;
            if (Modules[q]->QMon[i].HasFlag(M_UNIQUE) && (Purpose != PUR_DUNGEON) && (Purpose != PUR_ANYMON))
                continue;
            if (Modules[q]->QMon[i].CR > maxlev)
                continue;
            if (Modules[q]->QMon[i].CR < minlev)
                continue;
            if (Modules[q]->QMon[i].Depth > Depth)
                continue;
            if (Modules[q]->QMon[i].HasFlag(M_PLAYER) || Modules[q]->QMon[i].HasFlag(M_UNKNOWN))
                continue;

            if (!isListing) {
                if (Modules[q]->QMon[i].HasFlag(M_VERY_RARE))
                    rarity = random(7) ? 0 : 1;
                else if (Modules[q]->QMon[i].HasFlag(M_RARE))
                    rarity = random(3) ? 0 : 1;
                else if (Modules[q]->QMon[i].HasFlag(M_UNCOMMON))
                    rarity = 3;
                else if (Modules[q]->QMon[i].HasFlag(M_COMMON))
                    rarity = 7;
                else
                    rarity = 1;
            } else
                rarity = 1;

            for (k=0; k!=rarity; k++) 
                Candidates[n++] = Modules[q]->MonsterID(i);
        }

    Candidates[n] = 0;
    if (!n && !isListing) {
        isListing = true;
        goto Retry;
    }
    if (!n)
        return 0;
    j = random(n);

    /* For people, it's good to return low CR monsters to a high
    CR request -- we want cave orcs generated at DLev 15 because
    the become veteran cave orc master paladins. For beasts,
    though, the generation of a grid bug at DLevel 15 should
    be very rare compared to the generation of a purple worm,
    as a majority of heavily templated grid bugs ends up looking
    very strange. */
    if (TMON(Candidates[j])->isMType(Candidates[j],MA_PERSON))
        return Candidates[j];
    for (i=0;i!=4;i++) {
        k = random(n);
        if (TMON(Candidates[k])->CR > TMON(Candidates[j])->CR)
            j = k;
    }

    return Candidates[j];
}

rID Game::GetTempID(uint16 Types, rID mID, int8 maxCR)
  {
    TMonster *tm = TMON(mID);
    int16 i,q,n; 
    int spread; 
     
    n = 0; 
    for (q=0;q!=1;q++) /* Only one Module at the moment */
      for(i=0;i!=Modules[q]->szTem;i++)
        {
          if (!(Modules[q]->QTem[i].TType & Types))
            continue;
          if (Modules[q]->QTem[i].ForMType && 
              !tm->isMType(mID,Modules[q]->QTem[i].ForMType))
            continue;
          if (Modules[q]->QTem[i].HasFlag(TMF_NOGEN))
            continue;
          if (Modules[q]->QTem[i].CR.Adjust(tm->CR) > maxCR)
            continue;
          // ww: if you are trying to modify a CR 1 warrior and max CR is
          // 5, we'd prefer to take VeryExperience (CR+4) over RankAmateur
          // (CR+1). Thus we weight by duplicating a template for a
          // 'spread' equal to its resulting CR.
          /*
          theGame->GetPlayer(0)->IPrint(
            Format("TempID %d %s %d %s %d.",
                    Types, NAME(mID), maxCR,
                    NAME(Modules[q]->TemplateID(i)),
                    Modules[q]->QTem[i].CR.Adjust(tm->CR)));
                    */

          for (spread = 0; 
               spread < std::max<int>(Modules[q]->QTem[i].CR.Adjust(tm->CR),1);
               spread++)
                Candidates[n++] = Modules[q]->TemplateID(i);
        }
    Candidates[n] = 0;
    if (!n)
      return 0;
    i = random(n);
    /*
    theGame->GetPlayer(0)->IPrint(
           Format("N = %d, i = %d = %s.",n,i,NAME(Candidates[i])));
           */
    return Candidates[i];
}

rID Game::GetEffectID(int16 Purpose, int8 minlev, int8 maxlev, int8 Source) {
    int16 i, q, n;
    Player *p;
    bool isListing = false;

    if (Purpose >= PUR_LISTING) {
        Purpose -= PUR_LISTING;
        isListing = true;
    }
    n = 0;

    /* This is a horrible, but presently necessary kludge. We aren't
       likely to go multiplayer anytime soon, anyway. */
    if (Purpose == PUR_KNOWN)
        p = theGame->GetPlayer(0);

    if (Source == AI_WAND)
        maxlev += 2;

    /* TODO(rmtew): Requires change if truly supporting more than one module. */
    for (q = 0; q != 1; q++) /* Only one Module at the moment */
        for (i = 0; i != Modules[q]->szEff; i++) {
            if (Source == AI_POISON) {
                if (Modules[q]->QEff[i].ef.aval != AR_POISON)
                    continue;
            } else if (!(Modules[q]->QEff[i].HasSource(Source)) && Source)
                continue;

            if (Purpose == PUR_DUNGEON)
                if (Modules[q]->QEff[i].HasFlag(EF_NOGEN))
                    continue;
            if (LevelAdjust(Modules[q]->QEff[i].Level, 0, 1) > maxlev)
                continue;
            if (LevelAdjust(Modules[q]->QEff[i].Level, 0, 5) < minlev)
                continue;
            if (Purpose == PUR_STAPLE)
                if (!Modules[q]->QEff[i].HasFlag(EF_STAPLE))
                    continue;
            if (Modules[q]->QEff[i].HasFlag(EF_CURSED)) {
                if (Purpose != PUR_CURSED && Purpose != PUR_ACQUIRE)
                    continue;
            } else if (Purpose == PUR_CURSED)
                continue;
            if (Purpose == PUR_POISON)
                if (Modules[q]->QEff[i].ef.aval != AR_POISON)
                    continue;
            if (Purpose == PUR_DISEASE)
                if (Modules[q]->QEff[i].ef.aval != AR_DISEASE)
                    continue;
            if (Purpose == PUR_MUNDANE) {
                if (!Modules[q]->QEff[i].HasFlag(EF_MUNDANE))
                    continue;
                if (Modules[q]->QEff[i].HasSource(AI_ALCHEMY))
                    continue;
            }

            if (Purpose == PUR_KNOWN) {
                rID effID = Modules[q]->EffectID(i);
                if (!p)
                    continue;

                if (TEFF(effID)->ef.aval == AR_POISON) {
                    if (p->HasSkill(SK_ALCHEMY) && p->HasSkill(SK_POISON_USE) && (std::min(p->SkillLevel(SK_ALCHEMY), p->SkillLevel(SK_POISON_USE)) >= TEFF(effID)->ef.sval))
                        ;
                    else
                        continue;
                } else if (!(p->getSpellFlags(effID) & (SP_CREATE | SP_KNOWN)))
                    continue;
            }

            if (Purpose == PUR_MINOR || Purpose == PUR_MAJOR) {
                if (!(Modules[q]->QEff[i].ef.aval == AR_POISON ||
                    Modules[q]->QEff[i].HasSource(AI_TRAP)))
                    continue;
                if (!Modules[q]->QEff[i].HasFlag(EF_MUNDANE))
                    continue;
            }
            Candidates[n++] = Modules[q]->EffectID(i);
        }
    Candidates[n] = 0;
    nCandidates = n;
    if (!n)
        return 0;

    return Candidates[random(n)];
}


rID Game::RandomResource(uint8 RType, int8 Level, int8 Range) {
    return Modules[0]->RandomResource(RType,Level,Range);
}

rID Module::RandomResource(uint8 RType, int8 Level, int8 Range)
  {
    switch(RType)
      {
        case T_TMONSTER: return MonsterID(random(szMon));
        case T_TITEM: return ItemID(random(szItm));
        case T_TFEATURE: return FeatureID(random(szFea));
        case T_TEFFECT: return EffectID(random(szEff));
        default: Fatal("RandomResource: invalid RType"); return 0; 
      }
  }

void* Game::GetMemory(rID xID,Player *pl)
  {
    int8 i;
    for (i=0;i!=MAX_PLAYERS;i++)
      if (p[i] == pl->myHandle)
        goto Found;
    Error("Unregistered player asking for resource memory!");
    return theGame->MDataSeg[(xID >> 24)-1];
    Found:
    return theGame->MDataSeg[(xID >> 24)-1] + theGame->Modules[(xID >> 24)-1]->GetMemoryPtr(xID,i);
  }

uint32 Module::GetMemoryPtr(rID xID,int8 pn)
  {                                                      
    uint32 ptr;
    ptr = szDataSeg;
    if (xID >= MonsterID(0) && xID <= MonsterID(szMon-1))
      {
        ptr += (xID - MonsterID(0)) * sizeof(MonMem) * 
          theGame->NumPlayers();
        return ptr;
      }
    else
      ptr += sizeof(MonMem) * szMon;

    if (xID >= ItemID(0) && xID <= ItemID(szItm-1))
      {
        ptr += (xID - ItemID(0)) * sizeof(ItemMem) * 
          theGame->NumPlayers();
        return ptr;
      }
    else
      ptr += sizeof(ItemMem) * szItm;

    if (xID >= EffectID(0) && xID <= EffectID(szEff-1))
      {
        ptr += (xID - EffectID(0)) * sizeof(EffMem) * 
          theGame->NumPlayers();
        return ptr;
      }
    else
      ptr += sizeof(EffMem) * szEff;

    if (xID >= RegionID(0) && xID <= RegionID(szReg-1))
      {
        ptr += (xID - RegionID(0)) * sizeof(RegMem) * 
          theGame->NumPlayers();
        return ptr;
      }
    else
      ptr += sizeof(RegMem) * szReg;


    Error("Illegal rID for resource memory retrieval!");
    return 0;
  }

int16 ResourceLevel(rID xID)
  {
    switch(RES(xID)->Type) {
      case T_TMONSTER:
        return TMON(xID)->CR;
      case T_TITEM:
        return TITEM(xID)->Level;
      case T_TEFFECT:
        return TEFF(xID)->Level;
      default:
        return 1;
      }
  }


bool ResourceHasFlag(rID xID, int16 fl)
  {
    switch(RES(xID)->Type) {
      case T_TMONSTER:
        return TMON(xID)->HasFlag(fl);
      case T_TITEM:
        return TITEM(xID)->HasFlag(fl);
      /*case T_TFEATURE:
        return TFEAT(xID)->HasFlag(fl);*/
      case T_TEFFECT:
        return TEFF(xID)->HasFlag(fl);
      /*case T_TARTIFACT:
        return TART(xID)->HasFlag(fl);
      case T_TQUEST:
        return TQUE(xID)->HasFlag(fl);
      case T_TDUNGEON:
        return TDUN(xID)->HasFlag(fl);
      case T_TNPC:
        return TNPC(xID)->HasFlag(fl); */
      case T_TCLASS:
        return TCLASS(xID)->HasFlag(fl);
      case T_TRACE:
        return TRACE(xID)->HasFlag(fl);
      case T_TDOMAIN:
        return TDOM(xID)->HasFlag(fl);
      case T_TGOD:
        return TGOD(xID)->HasFlag(fl);
      case T_TREGION:
        return TREG(xID)->HasFlag(fl);
      case T_TTERRAIN:
        return TTER(xID)->HasFlag(fl);
      case T_TENCOUNTER:
        return TENC(xID)->HasFlag(fl);
      default:
        return false;
      }
  }


/*******************************************************************\
 *            Memory Cleanup Routines and Destructors              *
\*******************************************************************/


void Game::Cleanup()
  {
    int16 i,j,n;

    /* Later, this should be adapted to scan through the registry
       to delete /all/ maps still in memory, including ones like
       interiors of buildings in the town. For now, this is good.
    */

    if (Modules[0])
      for(i=0;i!=MAX_DUNGEONS;i++)
        if (DungeonID[i]) {
          n = (int16)TDUN(DungeonID[i])->GetConst(DUN_DEPTH);
          for(j=0;j!=n;j++) {
            //if (theRegistry->GroupInMemory(DungeonLevels[i][j]))
            if (theRegistry->Exists(DungeonLevels[i][j]))
              delete oMap(DungeonLevels[i][j]);
            }
          free(DungeonLevels[i]);
          DungeonLevels[i] = NULL;
          }

    memset(DungeonID,0,sizeof(rID)*MAX_DUNGEONS);
    memset(DungeonSize,0,sizeof(int16)*MAX_DUNGEONS);


    for(i=0;i!=MAX_MODULES;i++)
      {
        if (Modules[i])
          delete Modules[i];
        if (MDataSeg[i])
          free(MDataSeg[i]);
        Modules[i] = NULL;
        MDataSeg[i] = NULL;
      }

    MainRegistry.Empty();
    ResourceRegistry.Empty();
    while (ModFiles[0])
      ModFiles.Remove(0);
    while (Limbo[0])
      Limbo.Remove(0);
    ItemGenNum = 0;
    BarrierCount = 0;
    SaveFile.Empty();
    m[0] = m[1] = m[2] = m[3] = 0;
    p[0] = p[1] = p[2] = p[3] = 0;
    memset(Modules,0,sizeof(Module*)*MAX_MODULES);
    Turn = 0; Day = 0;    
    PlayMode = false;
    doSave = false;
    T1->ClearPlayer(); 
    

    /* Re-register the main Game object */
    myHandle = MainRegistry.RegisterObject(this);
  }

Map::~Map()                   
  {
    Thing *t; int16 i;
    MapIterate(this,t,i)
      if (t)
        delete t;
    free(Grid);
  }

Module::~Module()
  {
    if (QMon) 
      free(QMon);
    if (QItm) 
      free(QItm);
    if (QFea)
      free(QFea);
    if (QEff)
      free(QEff);
    if (QArt)
      free(QArt);
    if (QQue)
      free(QQue);
    if (QDgn)
      free(QDgn);
    if (QRou)
      free(QRou);
    if (QNPC)
      free(QNPC);
    if (QCla)
      free(QCla);
    if (QRac)
      free(QRac);
    if (QDom)
      free(QDom);
    if (QGod)
      free(QGod);
    if (QReg)
      free(QReg);
    if (QTer)
      free(QTer);
    if (QTxt)
      free(QTxt);
    if (QVar)
      free(QVar);
    if (QTem)
      free(QTem);
    if (QFla)
      free(QFla);
    if (QEnc)
      free(QEnc);
    if (QBev)
      free(QBev);


    if (QTextSeg)
      free((void*)QTextSeg);
    if (QCodeSeg)
      free(QCodeSeg);

  }

