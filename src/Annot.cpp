/* ANNOT.CPP -- See the Incursion LICENSE file for copyright information.

     Contains all of the code and functions for creating and
   manipulating resource annotations, including dungeon design
   constant lists, race and monster equipment lists, map tile
   type lists, class ability lists and so forth.
     
     void Resource::AddResID(int8 at,rID sID)
     void Resource::AddAbility(int8 AbType,int8 Abil,uint32 Param,int8 Lev1,int8 Lev2)
     void Resource::AddElement(rID xID,int8 x,int8 y,uint16 fl)
     void Resource::AddEquip(uint8 Chance,Dice amt,rID iID,rID eID,int8 spec,uint32 qual,uint8 fl)
     void Resource::AddConstant(int8 con,int16 val) 
     void Resource::AddList(int8 ln, rID *lv)
     void Resource::AddTile(char ch, Glyph img, uint8 fl, rID tID, rID xID, rID xID2)
     Tile* Resource::GetTile(char ch)
     bool Resource::GetList(int16 ln, rID *lv,int16 max)
     uint32 Resource::GetConst(int16 cn)
     void Resource::GrantGear(Creature *c, rID xID, bool doRanged)
     void Character::GainItem(Item *it, bool starting)
     void Monster::GainItem(Item *it, bool starting)
     EvReturn Resource::PEvent(int16 ev, Thing *t, rID xID, int16 con)
     EvReturn Resource::Event(EventInfo &e, rID xID, int16 context)
     String* Resource::GetMessages(int16 ev)
     String & Resource::RandMessage(int16 ev)
     void Resource::AddEvent(int16 ev, uint32 moc) 
     rID Resource::GetRes(int16 at, bool first)
     void Resource::AddChatter(uint8,const char*m1,const char*m2,const char*m3,
                              const char*m4,const char*m5) 
     void Resource::AddParam(int8 p,int16 v) 
     void Resource::AddArray(int8 p,int16 *list) 
     void Resource::AddSpecial(rID xID, int16 Chance, int16 Lev) 
     void Resource::AddSpecial(rID xID, int16 Chance, Dice& Lev)
     void Resource::AddPower(int8 apt,int8 pw,uint32 x,int16 pa,Dice*charge)
*/


#include "Incursion.h"

Annotation * Resource::cAnnot;
Annotation * Resource::cAnnot2;
String       Resource::EvMsg[64];

extern ReincarnationInfo RInf;

extern hObj excessItems[30];

void Resource::AddResID(int8 at,rID sID)
  {
    Annotation *a = Annot(AnHead); int8 i;
    while(a)
      {
        if (a->AnType == at)
          if (!a->u.sp[I(7,8)].sID)
            {
              Found:
              for (i=0;i!=8;i++)
                if (!a->u.sp[I(i,8)].sID)
                  {
                    a->u.sp[I(i,8)].sID = sID;
                    return;
                  }
              Fatal("Fatal: Annotation wierdness.");
            }
        if (!a->Next)
          break;
        a = Annot(a->Next);
      }
    if (a) 
      a = NewAnnot(at,&(a->Next));
    else 
      a = NewAnnot(at,&AnHead);
    goto Found;
  }

void Resource::AddAbility(int8 AbType,int16 Abil,uint32 Param,int8 Lev1,int8 Lev2)
  {
    Annotation *a; int8 i;
    //return;
    if (!AnHead) {
      a = NewAnnot(AN_ABILITY,&AnHead);
      goto Found;
      }
    a = Annot(AnHead);
    while(a)
      {
        if (a->AnType == AN_ABILITY)
          if (!a->u.ab[I(3,4)].AbType)
            {
              Found:
              for (i=0;i!=4;i++)
                if (!a->u.ab[I(i,4)].AbType)
                  {
                    a->u.ab[I(i,4)].AbType  = AbType;
                    a->u.ab[I(i,4)].Ability = Abil;
                    a->u.ab[I(i,4)].Param   = Param;
                    a->u.ab[I(i,4)].Lev1    = Lev1;
                    a->u.ab[I(i,4)].Lev2    = Lev2;
                    return;
                  }
              Fatal("Fatal: Annotation wierdness.");
            }
        if (!a->Next)
          break;
        a = Annot(a->Next);
      }
    a = NewAnnot(AN_ABILITY,&a->Next);
    goto Found;
  }

void Resource::AddElement(rID xID,int8 x,int8 y,uint16 fl)
  {
    Annotation *a = Annot(AnHead); int8 i;
    while(a)
      {      
        if (a->AnType == AN_ELEMENT)
          if (!a->u.me[3].xID)
            {
              Found:
              for (i=0;i!=4;i++)
                if (!a->u.me[I(i,4)].xID)
                  {
                    a->u.me[I(i,4)].xID     = xID;
                    a->u.me[I(i,4)].x       = x;
                    a->u.me[I(i,4)].y       = y;
                    a->u.me[I(i,4)].EFlags  = fl;
                    return;
                  }
              Fatal("Fatal: Annotation wierdness.");
            }
        if (!a->Next)
          break;
        a = Annot(a->Next);
      }
    a = NewAnnot(AN_ELEMENT,a ? &a->Next : &AnHead);
    goto Found;
  }

void Resource::AddEquip(uint8 Chance,Dice amt,rID iID,rID eID,int8 spec,
                          uint8 qual[4],uint8 fl, hCode cond)
  {
    Annotation *a = Annot(AnHead); 
    if (!AnHead)
      a = NewAnnot(AN_EQUIP,&AnHead);
    else {
      while(a->Next)
        a = Annot(a->Next);
      a = NewAnnot(AN_EQUIP,&a->Next);
      }
    a->u.eq.Amount = amt;
    a->u.eq.Chance = Chance;
    a->u.eq.Condition = 0;
    a->u.eq.eID = eID;
    a->u.eq.Flags = fl;
    a->u.eq.iID = iID;
    a->u.eq.Qualities[0] = qual[0];
    a->u.eq.Qualities[1] = qual[1];
    a->u.eq.Qualities[2] = qual[2];
    a->u.eq.Qualities[3] = qual[3];
    a->u.eq.Spec = spec;
    a->u.eq.Condition = cond;
  }

void Resource::AddConstant(int8 con,int32 val) 
  {
    Annotation *a; int8 i;
    if (!AnHead)
      {
        a = NewAnnot(AN_DUNCONST,&AnHead);
        a->u.dc[0].Const = con;
        a->u.dc[0].Value = val;
        return;
      }
    else
      {
        a = Annot(AnHead);
        do {
          if (a->AnType == AN_DUNCONST)
            for(i=0;i!=6;i++)
              if (!a->u.dc[I(i,6)].Const)
                {
                  a->u.dc[I(i,6)].Const = con;
                  a->u.dc[I(i,6)].Value = val;
                  return;
                }
            if (a->Next)        
              a = Annot(a->Next);
            else
              break;
          }
        while(1);
        a = NewAnnot(AN_DUNCONST,&a->Next);
        a->u.dc[I(0,6)].Const = con;
        a->u.dc[I(0,6)].Value = val;
      }
  }

void Resource::AddList(int8 ln, rID *lv)
  {
    Annotation *a; int8 i;
    do {
      if (!AnHead)
        {
          a = NewAnnot(AN_DUNLIST,&AnHead);
          memset(a->u.dl.xID,0xEC,sizeof(rID)*7);
          a->u.dl.Const = ln;
          a->u.dl.xID[I(0,7)] = *lv;          
          a->AnType = AN_DUNLIST;
          a->Next = 0;
        }
      else
        {
          a = Annot(AnHead);
          do {
            if (a->AnType == AN_DUNLIST && a->u.dl.Const == ln)
              for(i=0;i!=7;i++)
                if (a->u.dl.xID[I(i,7)] == 0xECECECEC)
                  {
                    a->u.dl.xID[I(i,7)] = *lv;
                    goto Done;
                  }
            if (a->Next)
              a = Annot(a->Next);
            else
              break;
            }
          while(1);
          a = NewAnnot(AN_DUNLIST,&a->Next);
          memset(a->u.dl.xID,0xEC,sizeof(rID)*7);
          a->u.dl.Const = ln;
          a->u.dl.xID[I(0,7)] = *lv;
        }
      Done:
      lv++;
      }
    while(*(lv-1) || *lv || *(lv+1));
    /* Write one zero always. */
    for(i=0;i!=7;i++)
      if (a->u.dl.xID[I(i,7)] == 0xECECECEC)
        a->u.dl.xID[I(i,7)] = 0;
  }

void Resource::AddTile(char ch, Glyph img, uint8 fl, rID tID, rID xID, rID xID2)
  {
    Annotation *a = Annot(AnHead); int8 i;
    while(a)
      {      
        if (a->AnType == AN_TILE)
          if (!a->u.ti[I(1,2)].tID)
            {
              Found:
              for (i=0;i!=2;i++)
                if (!a->u.ti[I(i,2)].tID)
                  {
                    a->u.ti[I(i,2)].ch      = ch;
                    a->u.ti[I(i,2)].fl      = fl;
                    a->u.ti[I(i,2)].Image   = img;
                    a->u.ti[I(i,2)].tID     = tID;
                    a->u.ti[I(i,2)].xID     = xID;
                    a->u.ti[I(i,2)].xID2    = xID2;
                    return;
                  }
              Fatal("Fatal: Annotation wierdness.");
            }
        if (!a->Next)
          break;
        a = Annot(a->Next);
      }
    a = NewAnnot(AN_TILE,a ? &a->Next : &AnHead);
    goto Found;
  }

Tile* Resource::GetTile(char ch)
  {
    Annotation *a = Annot(AnHead); int8 i;
    while(a)
      {      
        if (a->AnType == AN_TILE)
          for(i=0;i!=2;i++)
            if (a->u.ti[I(i,2)].ch == ch)
              return &(a->u.ti[I(i,2)]);
        if (!a->Next)
          break;
        a = Annot(a->Next);
      }
    return NULL;
  }


bool Resource::GetList(int16 ln, rID *lv,int16 max) {
    Annotation *a;
    int32 i, q; 
    bool found = false;
    rID xID, endID, *ls = lv;
    static uint32 RM_Weights[] = {
        RM_NORMAL,   10,
        RM_NOROOM,   1,
        RM_LARGE,    1,
        RM_CROSS,    1,
        RM_OVERLAP,  1,
        RM_ADJACENT, 1,
        RM_AD_ROUND, 2,
        RM_AD_MIXED, 2,
        RM_CIRCLE,   4,
        RM_OCTAGON,  5,
        RM_DIAMONDS, 4,
        RM_DOUBLE,   2, 
        RM_PILLARS,  3,
        //RM_CASTLE, 3, 
        RM_CHECKER,  1,
        RM_BUILDING, 3,
        //RM_DESTROYED, 1,
        RM_GRID,     1,
        RM_LIFECAVE, 10,
        RM_RCAVERN,  4,
        RM_MAZE,     2,   
        RM_LCIRCLE,  1,
        RM_SHAPED,   2,
        //RM_2ROOMS,  3,
        //RM_3ROOMS,  4,
        0,           0
    };
    uint32 RC_Weights[] = {
        RC_EMPTY,    1,
        RC_SINGLE,   4,
        RC_SINGLE_T, 2,
        RC_DIRTY,    1,
        RC_MULTI,    1,
        0,           0
    };
    uint32 EmptyList[] = { 0,0,0,0 };

    // The end of the list has three reserved blank tail slots.  This should cover all list row sizes (expected to be <= 3).
    max--; max--; max--;
    if (AnHead == 0)
        goto DoDefaults;

    a = Annot(AnHead);
    ASSERT(max > 0)
    do {
        if (a->AnType == AN_DUNLIST && a->u.dl.Const == ln)
            for(i=0;i!=7 && max;i++) {
                *lv++ = a->u.dl.xID[I(i,7)];
                max--;
                found = true;
            }
        a = Annot(a->Next);
    } while(a && max);

    if (found) {
        /* We want to make sure that every list has at least
        three terminating zeros, because certain loops use
        values three at a time. */
        *lv++ =0; *lv++ =0; *lv++ =0;
        return true;
    }

DoDefaults:
    switch(ln) {
    case RM_WEIGHTS: 
        memcpy(lv,RM_Weights,std::min(max*sizeof(rID),sizeof(RM_Weights)));
        return true;
    case RC_WEIGHTS:
        memcpy(lv,RC_Weights,std::min(max*sizeof(rID),sizeof(RC_Weights)));
        return true;
    case ROOM_WEIGHTS:
        for (q=0;q!=MAX_MODULES;q++)
            if (theGame->Modules[I(q,MAX_MODULES)]) {
                xID = xID=theGame->Modules[I(q,MAX_MODULES)]->RegionID(0);
                endID = xID + theGame->Modules[I(q,MAX_MODULES)]->szReg;
                for(;xID!=endID && max;xID++)
                    if (TREG(xID)->HasFlag(RF_ROOM))
                        if (!TREG(xID)->HasFlag(RF_NOGEN)) {
                            *lv++ = xID;
                            *lv++ = 1;
                            max--;
                            max--;
                        }
            }
        *lv++ = 0; *lv++ = 0; *lv++ = 0; 
        return true;
    case CORRIDOR_WEIGHTS:
        for (q=0;q!=MAX_MODULES;q++)
            if (theGame->Modules[I(q,MAX_MODULES)]) {
                xID = xID=theGame->Modules[I(q,MAX_MODULES)]->RegionID(0);
                endID = xID + theGame->Modules[I(q,MAX_MODULES)]->szReg;
                for(;xID!=endID && max;xID++)
                    if (TREG(xID)->HasFlag(RF_CORRIDOR))
                        if (!TREG(xID)->HasFlag(RF_NOGEN)) {
                            *lv++ = xID; 
                            *lv++ = TREG(xID)->HasFlag(RF_STAPLE) ? 16 : 1;
                            max--;
                            max--;
                        }
            }
        *lv++ = 0; *lv++ = 0; *lv++ = 0; 
        return true;
    case VAULT_WEIGHTS:
        for (q=0;q!=MAX_MODULES;q++)
            if (theGame->Modules[I(q,MAX_MODULES)]) {
                xID = xID=theGame->Modules[I(q,MAX_MODULES)]->RegionID(0);
                endID = xID + theGame->Modules[I(q,MAX_MODULES)]->szReg;
                for(;xID!=endID && max;xID++)
                    if (TREG(xID)->HasFlag(RF_VAULT))
                        if (!TREG(xID)->HasFlag(RF_NOGEN)) {
                            *lv++ = xID;
                            max--;
                        }
            }
        *lv++ = 0; *lv++ = 0; *lv++ = 0; 
        return true;
    case STREAMER_WEIGHTS:
        for (q=0;q!=MAX_MODULES;q++)
            if (theGame->Modules[I(q,MAX_MODULES)]) {
                xID = xID=theGame->Modules[I(q,MAX_MODULES)]->RegionID(0);
                endID = xID + theGame->Modules[I(q,MAX_MODULES)]->szReg;
                for(;xID!=endID && max;xID++)
                    if (TREG(xID)->HasFlag(RF_RIVER) || TREG(xID)->HasFlag(RF_ROCKTYPE) || TREG(xID)->HasFlag(RF_STREAMER) || TREG(xID)->HasFlag(RF_CHASM))
                        if (!TREG(xID)->HasFlag(RF_NOGEN)) {
                            *lv++ = xID;
                            *lv++ = 1;
                            max--;
                            max--;
                        }
            }
        *lv++ = 0; *lv++ = 0; *lv++ = 0; 
        return true;
    default:
        memcpy(lv,EmptyList,std::min(max*sizeof(rID),sizeof(EmptyList)));
        return false;
    }
}

bool Resource::HasList(int16 ln)
  {
    Annotation *a;
    
    if (AnHead == 0)
      return false;
    a = Annot(AnHead);
    do {
      if (a->AnType == AN_DUNLIST && a->u.dl.Const == ln)
        return true;
      a = Annot(a->Next);
      }
    while(a);
    return false;
  }


bool Resource::ListHasItem(int16 ln, int32 look_for)
  {
    rID List[64]; int16 i;
    memset(List,0,sizeof(int32)*64);
    if (!GetList(ln,List,64))
      return false;
    for (i=0;List[i];i++)
      if (List[i] == look_for)
        return true;
    return false;
  }


uint32 Resource::GetConst(int16 cn)
  {
    Annotation *a; int16 i;
    if (!AnHead)
      goto DoDefaults;
    if (theGame->Opt(OPT_DIFFICULTY) >= DIFF_CHALLENGE)
      if (cn == DUN_DEPTH)
        return 15;
    a = Annot(AnHead);
    while (a)
      {
        if (a->AnType == AN_DUNCONST)
          for (i=0;i!=6;i++)
            if (a->u.dc[I(i,6)].Const == cn)
              /* HACKFIX */
              return a->u.dc[I(i,6)].Value +
                ((cn == TOLERANCE_VAL) ? 3 : 0);
        a = Annot(a->Next);
      }
    DoDefaults:
    switch(cn)
      {
        case LEVEL_SIZEX:         return 128;
        case LEVEL_SIZEY:         return 128;
        case PANEL_SIZEX:         return 32;
        case PANEL_SIZEY:         return 32;  
        case MAX_STREAMER_WIDTH:  return 5;  
        case MIN_STREAMERS:       return 2;  
        case MAX_STREAMERS:       return 4;  
        case TERRAIN_MAPEDGE:     return FIND("indestructable rock");  
        case TERRAIN_ROCK:        return FIND("solid rock");  
        case TERRAIN_TORCH:       return FIND("wall torch");
        case BASE_REGION:         return FIND("unworked stone");  
        case DIGGING_REGION:      return FIND("rough-hewn tunnel");  
        case TERRAFORM_REGION:    return FIND("smooth cavity");  
        case REDUCE_CHASM_CHANCE: return 50; 
        case MIN_STAIRS:          return 4;  
        case MAX_STAIRS:          return 4;  
        case ROOM_MINX:           return 4;  
        case ROOM_MAXX:           return 12;  
        case ROOM_MINY:           return 4;  
        case ROOM_MAXY:           return 12;  
        case TURN_CHANCE:         return 10;  
        case STUBBORN_CORRIDOR:   return 30;  
        case SEGMENT_MINLEN:      return 4;
        case SEGMENT_MAXLEN:      return 10;
        case STREAMER_CHANCE:     return 70;  
        case MONSTER_COUNT:       return 10;  
        case MONSTER_FREQ:        return 500;  
        case REST_SAFETY:         return 50;  
        case DEAD_MAGIC:          return 0;  
        case WILD_MAGIC:          return 0; 
        case PSYCHIC_TRAUMA:      return 0;  
        case DESTROYED_LEV:       return 0;  
        case NATURAL_LEV:         return 0;  
        case VAULT_CHANCE:        return 0;  
        case GVAULT_CHANCE:       return 0;  
        case MAX_ARTIFACTS:       return 0;  
        case MIN_ARTIFACTS:       return 0;  
        case ARTI_LEVEL:          return 0;  
        case ARTI_CHANCE:         return 0;  
        case ARTI_LIST:           return 0;  
        case ARTI_REQUIRED:       return 0;  
        case DUN_DEPTH:           return Type == T_TDUNGEON ? 20 : 1;  
        case INITIAL_CR:          return 1;  
        case DUN_SPEED:           return 100; 
        case SUBMAP_CHANCE:       return 50;  
        case MAX_SUBMAPS:         return 3;  
        case CORRIDOR_REGION:     return FIND("winding corridor");  
        case BIG_CORRIDOR_REGION: return FIND("vaulted corridor");  
        case STAIRS_UP:           return FIND("up stairs");  
        case STAIRS_DOWN:         return FIND("down stairs");  
        case LIFE_PERCENT:        return 55;
        case LIFE_ITERATIONS:     return 20;
        case TERRAIN_PILLAR:      return FIND("pillar");  
        case MIN_RIVER_DEPTH:     return 3;
        case MIN_CHASM_DEPTH:     return 5;
        case TREASURE_CHANCE:     return 30;
        case CHEST_CHANCE:        return 7;
        case CHEST_MIN_ITEMS:     return 6;
        case CHEST_MAX_ITEMS:     return 15;
        case CURSED_CHANCE:       return 20;
        // ww: streamer monster density was way too high -- any level with
        // a river became a battle royale on the water
        case STREAMER_MON_DENSITY:return 75;
        case MONSTER_EQUILIBRIUM_BASE: return 50;
        case MONSTER_EQUILIBRIUM_INC: return 5;
        case TRAP_CHANCE:         return 200;
        case MIN_VAULT_DEPTH:     return 5;
        case ROOM_LIT_CHANCE:     return 50;
        case LIT_CHANCE_DEPTH_MOD:return 4;
        case TORCH_DENSITY:       return 10;
        case SKILL_KIT_MOD:       return 2;
        default:                  return 0;

      }
  }

Creature *__Buy, *__Sell;

int SortGear(const void *A, const void *B)
  {
    Item *a = *((Item**)A),
         *b = *((Item**)B);
    return (a->getShopCost(__Buy,__Sell) / a->GetQuantity()) <
           (b->getShopCost(__Buy,__Sell) / b->GetQuantity());
  }

void Resource::GrantGear(Creature *c, rID xID, bool doRanged)
  {
    time_t t;
    rID unID = FIND("unimplemented");
    rID iID, eID;
    Annotation *a; Item *it, *itl[256]; 
    int16 plus, ic, i, seed;
    int8 sc;

    bool last_given = true;
    bool random_depth_effect = false; 
    ic = 0;

    time(&t);


    switch(T1->GetMode())
      {
        case MO_CREATE:
          seed = t % 100;
          RInf.EquipRolls[RInf.cEquipRolls++] = (int8)seed;
          srand(seed);
          ASSERT(RInf.cEquipRolls < 256);
         break;
        case MO_RECREATE:
          seed = RInf.EquipRolls[RInf.nEquipRolls++];
          srand(seed);
          ASSERT(RInf.nEquipRolls < 256);
         break;
        default:
         break; 
      }
    
    Player *p;
    if (c->HasMFlag(M_SELLER))
      if (p = theGame->GetPlayer(0))
        srand(p->storeSeed);

    if (Type == T_TMONSTER)
      if (((TMonster*)this)->HasFlag(M_AVOID_MELEE))
        doRanged = true;
    if (Type == T_TTEMPLATE)
      if (((TTemplate*)this)->AddsFlag(M_AVOID_MELEE))
        doRanged = true;

    

    for(a=FAnnot2();a;a=NAnnot2())
      {
        /* Resource::Event has a FAnnot()/NAnnot() loop. We need
        to use this FAnnot2 kludge to maintain the loop that we are
        in at the moment. Nested FAnnot/NAnnot doesn't normally
        work. */
        if (a->AnType == AN_EQUIP)
          {
            it = NULL;

            if (a->u.eq.Flags & GF_ELSE)
              if (last_given)
                continue;

            if (a->u.eq.Chance) {
              int16 roll;
              switch(T1->GetMode())
                {
                  case MO_CREATE:
                    roll = random(100)+1;
                    RInf.EquipRolls[RInf.cEquipRolls++] = (int8)roll;
                    ASSERT(RInf.cEquipRolls < 256);
                   break;
                  case MO_RECREATE:
                    roll = RInf.EquipRolls[RInf.nEquipRolls++];
                    ASSERT(RInf.nEquipRolls < 256);
                   break;
                  default:
                    roll = random(100)+1; 
                }
              if (roll > a->u.eq.Chance)
                { last_given = false; continue; }
              }
              
                
            if (a->u.eq.Condition)
              {
                EventInfo e; int32 res;
                e.Clear();
                e.EActor = c;
                e.EVictim = c;
                res = theGame->VM.Execute(&e, xID, a->u.eq.Condition);
                if (!res)
                  { last_given = false; continue; }
              }

            last_given = true;
            if ((a->u.eq.Flags & GF_RANDOM_ITEM))
              {
                /*maxlev = a->u.eq.eID / 256;
                minlev = a->u.eq.eID % 256; */
                int trials = 20;
                do { 
                  if (a->u.eq.iID) {
                    iID = theGame->GetItemID(PUR_EQUIP,0,20,a->u.eq.iID & 0xFF);
                  } else
                    iID = theGame->GetItemID(PUR_EQUIP,0,20,-1);     
                } while (trials-- > 0 && !c->isPlayer() && (!iID ||
                         (TITEM(iID)->Level > 0 ||
                          TITEM(iID)->Size > TMON(c->mID)->Size)));
                // ww: no generating random great mauls for halflings! 
              }
            else 
              iID = a->u.eq.iID;
            random_depth_effect = false; 
            if (a->u.eq.Flags & GF_RANDOM_EFFECT)
              {
                if (iID)
                  switch(TITEM(iID)->IType)
                    {
                      case T_AMULET: sc = AI_AMULET; break;
                      case T_RING:   sc = AI_RING; break;
                      case T_WAND:   sc = AI_WAND; break;
                      case T_POTION: sc = AI_POTION; break;
                      case T_SCROLL: sc = AI_SCROLL; break;
                      default: sc = -1; break;
                    }
                else
                  sc = -1;
                /* THIS IS THE SOURCE OF THE GENERIC ITEM BUG. It needs to be
                   examined in more detail, and i.e. expanded to create magic
                   weapons as granted gear. */
                eID = theGame->GetEffectID(PUR_EQUIP,a->u.eq.eID%256,(a->u.eq.eID/256)&0xFF,sc);
                if (!eID)
                  eID = theGame->GetEffectID(PUR_EQUIP,0,5,sc);
                if ((!eID) && (iID && (TITEM(iID)->IType == T_ARMOUR ||
                             TITEM(iID)->IType == T_WEAPON ||
                             TITEM(iID)->IType == T_SHIELD ||
                             TITEM(iID)->IType == T_MISSILE ||
                             TITEM(iID)->IType == T_BOW)))
                  // ww: we were given a gear list like:
                  // random T_WEAPON of random 1 to 2,
                  // so now we should generate random weapon of depth 1-2
                  random_depth_effect = true;
              }
            else
              eID = a->u.eq.eID;
            if (!iID)
              continue;
            if (TITEM(iID)->IType == T_BOW || TITEM(iID)->IType == T_MISSILE)
              if (!doRanged)
                continue;
            if (random_depth_effect) {
                int max_lev = a->u.eq.eID / 256; 
                int min_lev = a->u.eq.eID % 256; 
                // TODO(design-decision-difficulty): Scale item difficulty in proportion to depth.
                if (theGame->GetPlayer(0) &&
                    theGame->GetPlayer(0)->m) 
                  max_lev = theGame->GetPlayer(0)->m->Depth;
                int the_lev = min_lev + random((max_lev + 1) - min_lev);
                int luck = c->GetAttr(A_LUC);
                if (!luck) luck = 10; 
                // ww: if you play a super-lucky character, you will find
                // that all of your opponents have powerful items
                if (theGame->GetPlayer(0))
                  luck = std::max<int>(luck,theGame->GetPlayer(0)->GetAttr(A_LUC));
                it = Item::GenItem(IG_MONEQUIP,iID,the_lev,luck,MonsterItems);
                if (!it)
                  continue;
                while (!(it->IFlags & IF_CURSED) && it->ItemLevel() < min_lev &&
                        it->GetInherentPlus() < 5) 
                  it->SetInherentPlus(it->GetInherentPlus() + 1);
              } 
            else if (iID == unID)
              ;
            else
              it = Item::Create(iID);

            if (it && (a->u.eq.Flags & GF_RANDOM_EFFECT))
              {
                EventInfo xe;
                xe.Clear();
                xe.ETarget = it;
                xe.EItem   = it;
                xe.vDepth  = (a->u.eq.eID/256) & 0xFFFF;
                xe.vLevel  = (a->u.eq.eID/256) & 0xFFFF;
                xe.Event   = EV_GENITEM;
                switch (TITEM(iID)->Event(xe,iID))
                  {
                    case DONE: eID = 0; break;
                    case ABORT: delete it; it = NULL; break;
                    case ERROR: Error("EV_GENITEM returned ERROR!");
                    case NOTHING: break;
                  }
              }
            if (!it) 
              continue;

            plus = a->u.eq.Spec;
            it->MakeMagical(eID, it->GetInherentPlus() + plus);
            it->SetQuantity(a->u.eq.Amount.Roll());
            if (a->u.eq.Flags & GF_CURSED)
              it->IFlags |= IF_CURSED;
            else if (a->u.eq.Flags & GF_BLESSED)
              it->IFlags |= IF_BLESSED;
            if (it->isType(T_WAND))
              it->SetCharges(50);
            else if (it->isType(T_STAFF))
              it->SetCharges(30);
            else if (it->isType(T_ROD))
              it->SetCharges(10);
            if (eID && TEFF(eID)->HasFlag(EF_NEEDS_PLUS) && !it->GetPlus())
              it->MakeMagical(eID,1);

            for (int8 q=0;q!=4;q++) 
              if (a->u.eq.Qualities[q])
                it->AddQuality(a->u.eq.Qualities[q]);

            if (c->isPlayer())
              it->MakeKnown(-1);
            else if (TITEM(it->iID)->IType == T_WEAPON &&
                     TITEM(it->iID)->Group & WG_EXOTIC) {
              /* ww: if an NPC is created with Weapon X, make sure that it
               * can use Weapon X */
              c->GainPermStati(EXTRA_FEAT,NULL,SS_CLAS,FT_EXOTIC_WEAPON);
            }
            itl[I(ic++,256)] = it;
          }
      }

    /* Some common-sense items */
    if (c->isMonster())
      {
        if (c->HasMFlag(M_HUMANOID))
          if (!(c->HasAbility(CA_INFRAVISION) || 
                c->HasAbility(CA_TELEPATHY) ||
                c->HasAbility(CA_TREMORSENSE) ||
                c->HasAbility(CA_BLINDSIGHT)))
            {
              it = Item::Create(FIND("torch"));
              it->Quantity = Dice::Roll(1,4,1);
              itl[I(ic++,256)] = it;
            }
      }
       
    if (c->HasMFlag(M_SELLER)) {
      __Buy = theGame->GetPlayer(0);
      __Sell = c;
      qsort(itl,ic,sizeof(Item*),SortGear);
      }
      
    for(i=0;i!=ic;i++) {
      if (c->isMonster())
        itl[I(i,256)]->IFlags |= IF_PROPERTY;
      c->GainItem(itl[I(i,256)],true);
      }
    
    if (c->HasMFlag(M_SELLER))
      c->StateFlags |= MS_INVEN_GOOD;
    
  }

void Character::GainItem(Item *it, bool starting)
  {
    int8 prefslot,i;
    Item *cn, *it2; EvReturn r;
    if (!it)
      return;
    it->Remove(false);
    
    it->MakeKnown(KN_HANDLED);

    for (it2=FirstInv();it2;it2=NextInv())
      if (it2->TryStack(it))
        return;

    // ww: we already own it! 
    if (it->GetParent() == this) {
      for (i=0;i<SL_LAST;i++)
        if (InSlot(i) == it)
          ThrowVal(EV_WIELD,i,this,NULL,it);
      return; 
    }

    if (!theGame->InPlay() || x == -1)
      starting = true; 

    switch(it->Type) {
      case T_WEAPON: prefslot = SL_WEAPON; 
                     if (it->isGroup(WG_DAGGERS) || InSlot(SL_WEAPON))
                       if (HasFeat(FT_TWO_WEAPON_STYLE))
                         prefslot = SL_READY;
                     break;
      case T_BOW:    prefslot = SL_LSHOULDER; break;
//    case T_GEM:    prefslot = SL_RSHOULDER; break;
      case T_MISSILE:prefslot = SL_BELT3; break;
      case T_TOOL:   prefslot = it->ItemLevel() ? SL_BELT4 : SL_BELT5; break;
      case T_ARMOUR:  prefslot = SL_ARMOUR; break;
      case T_BRACERS:prefslot = SL_BRACERS; break;
      case T_CONTAIN:/*if (its a quiver)
                       prefslot = SL_LSHOULDER;*/
                     prefslot = SL_PACK; break;
      case T_RING:   prefslot = InSlot(SL_LRING) ? SL_RRING : SL_LRING; break;
      case T_AMULET: prefslot = SL_AMULET; break;
      case T_SYMBOL: prefslot = SL_AMULET; break;
      case T_BOOTS:  prefslot = SL_BOOTS; break;
      case T_HELMET: prefslot = SL_HELM; break;
      case T_CLOTHES:prefslot = SL_CLOTHES; break;
      case T_CLOAK:  prefslot = SL_CLOAK; break;
      case T_EYES:   prefslot = SL_EYES; break;
      case T_GAUNTLETS: prefslot = SL_GAUNTLETS; break;
      default:       prefslot = -1; break;
      }
    
    /* Don't enter play wielding a pickaxe. */
    if (starting && it->iID == FIND("pickaxe"))
      prefslot = -1;

    /* Don't start a Small character with a Large weapon that s/he
       can't even use. */
    if (starting && it->isType(T_WEAPON) &&
        (! (TITEM(it->iID)->Group & WG_THROWN)))
      if (TMON(tmID)->Size < SZ_MEDIUM && it->Size(this) > SZ_MEDIUM)
        {
          it->Remove(true);
          return;
        }

    if (!starting && isPlayer() /*&& Opt(OPT_PLACE_NEW_EQUIP)*/)
      {
        Inv[SL_INAIR] = it->myHandle;
        it->SetOwner(myHandle);
        thisp->MyTerm->PrePrompt();
        thisp->MyTerm->InventoryManager(true);
        return;
      }
    
    if (prefslot != -1)
      if (!InSlot(prefslot))
        {
          SetSilence();
          if (ThrowVal(EV_WIELD,prefslot,this,NULL,it)!=ABORT)
            {
              UnsetSilence();
              return;
            }
          UnsetSilence();
        } 
    
    if (it->Type == T_LIGHT && !InSlot(SL_LIGHT)) {
      if (it->Quantity == 1)
        {    
          SetSilence();
          if (ThrowVal(EV_WIELD,SL_LIGHT,this,NULL,it)!=ABORT)
            {
              UnsetSilence();
              return;
            }
          UnsetSilence();
        }
      else 
        {
          it2 = it->TakeOne();
          SetSilence();
          if (ThrowVal(EV_WIELD,SL_LIGHT,this,NULL,it2) == ABORT)
            {
              it->Quantity++;
              it2->Remove(true);
            }
          UnsetSilence();
        }
      }       
    
    for(i=2;i!=SL_LAST;i++)
      if ((cn = InSlot(i)) && cn->isType(T_CONTAIN))
        if (TITEM(cn->iID)->u.c.CType)
          if (it->isType(TITEM(cn->iID)->u.c.CType))
            {
              SetSilence();
              r = Throw(EV_INSERT, this,NULL,cn,it);
              UnsetSilence();
              if (r == DONE)                          
                return;
            }
    
    if ((it->Type == T_FOOD || it->Type == T_TOOL || it->Type == T_BOOK ||
         it->Type == T_LIGHT) 
          && InSlot(SL_PACK))
      if (InSlot(SL_PACK)->Type == T_CONTAIN)
        {
          SetSilence();
          r = Throw(EV_INSERT,this,NULL,InSlot(SL_PACK),it);
          UnsetSilence();
          if (r == DONE)
            return;
        }

    if (it->Type != T_FOOD && it->Type != T_TOOL)
      if (TITEM(it->iID)->Size <= Attr[A_SIZ]-1)
        for(i=SL_BELT1;i<=SL_BELT5;i++)
          if(!InSlot(i))
            {
              SetSilence();
              r = ThrowVal(EV_WIELD,i,this,NULL,it);
              UnsetSilence();
              if (r == DONE)
                return;
            }
    
    if (it->Size() >= Attr[A_SIZ])
      for(i=SL_LSHOULDER;i!=SL_RSHOULDER+1;i++)
        if(!InSlot(i))
          {
            SetSilence();
            r = ThrowVal(EV_WIELD,i,this,NULL,it);
            UnsetSilence();
            if (r == DONE)
              return;
          }
    
    #if 0
    if (InSlot(SL_PACK))
      if (InSlot(SL_PACK)->Type == T_CONTAIN)
        {
          SetSilence();
          r = Throw(EV_INSERT,this,NULL,InSlot(SL_PACK),it);
          UnsetSilence();
          if (r == DONE)
            return;
        }
    #endif
    
    int8 TrySlots[] = {
      SL_LSHOULDER, SL_RSHOULDER, SL_BELT1, SL_BELT2, SL_BELT3, 
      SL_BELT4, SL_BELT5, SL_WEAPON, -1 };
    for (i=1;TrySlots[i] != -1;i++)
      if(it->allowedSlot(TrySlots[i],this) && !InSlot(TrySlots[i]))
        {
          SetSilence();
          r = ThrowVal(EV_WIELD,TrySlots[i],this,NULL,it);
          UnsetSilence();
          if (r == DONE)
            return;
        }

    for(i=0;i!=SL_LAST;i++)
      if ((cn = InSlot(i)) && cn->isType(T_CONTAIN))
        if (TITEM(cn->iID)->u.c.CType)
          if (it->isType(TITEM(cn->iID)->u.c.CType))
            {
              SetSilence();
              r = Throw(EV_INSERT, this,NULL,cn,it);
              UnsetSilence();
              if (r == DONE)                          
                return;
            }

    if (InSlot(SL_PACK))
      if (InSlot(SL_PACK)->Type == T_CONTAIN)
        {
          SetSilence();
          r = Throw(EV_INSERT,this,NULL,InSlot(SL_PACK),it);
          UnsetSilence();
          if (r == DONE && it->GetParent() == InSlot(SL_PACK))
            return;
        }
                
                
    if (theGame->InPlay() && m && x != -1)
      {
        it->PlaceAt(m,x,y);
        return;
      }

    /* Such a shame... */
    if (starting && InSlot(SL_PACK) && InSlot(SL_PACK)->Type == T_CONTAIN) { 
      for (i=0;i!=30;i++)
        if (!excessItems[i])
          { excessItems[i] = it->myHandle; return; }
      }
      
    it->Remove(true);

  }

void Monster::GainItem(Item *it, bool starting)
  {
    ASSERT(isType(T_MONSTER));
    it->Remove(false);
    it->Next = Inv; 
    Inv = it->myHandle;
    it->SetOwner(myHandle);
    StateFlags &= ~MS_INVEN_GOOD; 

  }
EvReturn Resource::PEvent(int16 ev, Thing *actor, Thing *item, rID xID, int16 con)
  {
    EventInfo e;
    e.Clear();
    e.Event = ev;
    e.EActor = (Creature*)actor;
    e.EVictim = (Creature*)actor;
    e.EItem = (Item*)item;
    e.EMap = e.EActor ? e.EActor->m :
             e.EVictim ? e.EVictim->m :
             NULL;
    if (actor) { 
      e.EXVal = actor->x;
      e.EXVal = actor->y;
    }
    return Event(e,xID,con);
  }

EvReturn Resource::PEvent(int16 ev, Thing *t, rID xID, int16 con)
  {
    return PEvent(ev,t,NULL,xID,con); 
  }

bool Resource::HandlesEvent(uint8 e)
{
  Annotation *a; 
  for(a=FAnnot();a;a=NAnnot())  
    if (a->AnType == AN_EVENT) 
      for (int i=0;i!=5;i++)
        if (a->u.ev[I(i,5)].Event == e)
          return true;
  return false; 
} 
extern EventInfo *ev;


EvReturn Resource::Event(EventInfo &e, rID xID, int16 Event)
  {
    Annotation *a; 
    int8 MsgNum = false;
    int16 i, oev; uint32 res;
    if (!xID)
      return NOTHING;

    oev = e.Event;
    if (Event)
      e.Event = Event;

    if (!(EventMask & BIT((e.Event%16)+1)))
      { e.Event = oev; return NOTHING; }

    for(a=FAnnot();a;a=NAnnot())  
      if (a->AnType == AN_EVENT) {
        for (i=0;i!=5;i++)
          if (a->u.ev[I(i,5)].Event == e.Event) {
            
            //PurgeStrings();
                  
            res = theGame->VM.Execute(&e, xID, a->u.ev[I(i,5)].MsgOrCode);
            /*if (res == ERROR) 
              Error("Event %s returned ERROR in VCode routine "
                "in resource [%s].",Lookup(EV_CONSTNAMES,e.Event),
                theGame->Modules[ModNum]->QTextSeg + Name);*/
            return (EvReturn) res; 
            }
          else if (a->u.ev[I(i,5)].Event == -e.Event)
            EvMsg[I(MsgNum++,64)] = theGame->Modules[I(ModNum,MAX_MODULES)]->QTextSeg +
              a->u.ev[I(i,5)].MsgOrCode;
        }

    e.Event = oev;

    /* This kludge avoids multiple EV_MAGIC_HIT messages being
       printed for the same target on multi-segment effects. */
    if (e.MHMessageDone && e.Event == EV_MAGIC_HIT)
      return NOMSG;
    if (e.Event == EV_MAGIC_HIT && MsgNum)
      e.MHMessageDone = true;

    switch(MsgNum)
      {
        case 1:
          ev = &e;
          if (e.Event == EV_DAMAGE || e.Event == EV_DEATH || 
              e.Event == EV_MAGIC_STRIKE || e.Event == EV_MAGIC_HIT)
            e.EVictim->IPrint(EvMsg[I(0,64)]);
          else if (e.Event == EV_FIELDON || e.Event == EV_FIELDOFF)
            e.EActor->IPrint(EvMsg[I(0,64)]);
          else
            APrint(e,EvMsg[I(0,64)]);
          e.Terse = true;
         break;
        case 2:
          if (e.Event == EV_DAMAGE || e.Event == EV_DEATH || 
              e.Event == EV_MAGIC_STRIKE || e.Event == EV_MAGIC_HIT)
            VPrint(e,EvMsg[I(0,64)],EvMsg[I(1,64)]);
          else
            DPrint(e,EvMsg[I(0,64)],EvMsg[I(1,64)]);
          e.Terse = true;
         break;
        case 3:                         
          TPrint(e,EvMsg[I(0,64)],EvMsg[I(1,64)],EvMsg[I(2,64)]);
          e.Terse = true;
         break;
      }
    return e.Terse ? NOMSG : NOTHING;
  }

String* Resource::GetMessages(int16 ev)
  {
    Annotation *a; 
    int8 MsgNum = false;
    int16 i;
    if (!(EventMask & BIT((ev%16)+1)))
      {
        EvMsg[I(0,64)].Empty();
        return EvMsg;
      }

    for(a=FAnnot();a;a=NAnnot())
      if (a->AnType == AN_EVENT) {
        for (i=0;i!=5;i++)
          if (a->u.ev[I(i,5)].Event == ev) {
            /*
            res = theVM->Execute(&e, theGame->Modules[ModNum]->
              QCodeSeg + a->e.er[i].CodePtr);
            if (res == ERROR) 
              Error("Event %s returned ERROR in VCode routine "
                "in resource [%s].",Lookup(EventNames,e.Event),
                theGame->Modules[ModNum]->QTextSeg + Name);
            EvMsg[I(MsgNum++,64)] = theGame->Modules[ModNum]->QTextSeg + res */
            }
          else if (a->u.ev[I(i,5)].Event == -ev)
            EvMsg[I(MsgNum++,64)] = theGame->Modules[I(ModNum,MAX_MODULES)]->QTextSeg +
              a->u.ev[I(i,5)].MsgOrCode;
        }

    /* Make sure we have a null-term. */
    EvMsg[I(MsgNum,64)].Empty();

    return EvMsg;
  }

String & Resource::RandMessage(int16 ev)
  {
    Annotation *a; 
    int8 MsgNum = false;
    int16 i;
    if (!(EventMask & BIT((ev%16)+1)))
      {
        EvMsg[I(0,64)].Empty();
        return EvMsg[I(0,64)];
      }

    for(a=FAnnot();a;a=NAnnot())
      if (a->AnType == AN_EVENT) {
        for (i=0;i!=5;i++)
          if (a->u.ev[I(i,5)].Event == ev) {
            /*res = theVM->Execute(&e, theGame->Modules[ModNum]->
              QCodeSeg + a->e.er[i].CodePtr);
            if (res == ERROR) 
              Error("Event %s returned ERROR in VCode routine "
                "in resource [%s].",Lookup(EventNames,e.Event),
                theGame->Modules[ModNum]->QTextSeg + Name);
            EvMsg[I(MsgNum++,64)] = theGame->Modules[ModNum]->QTextSeg + res */
            }
          else if (a->u.ev[I(i,5)].Event == -ev)
            EvMsg[I(MsgNum++,64)] = theGame->Modules[ModNum]->QTextSeg +
              a->u.ev[I(i,5)].MsgOrCode;
        }

    /* Make sure we have a null-term. */
    EvMsg[I(MsgNum,64)].Empty();


    return *tmpstr(EvMsg[I(random(MsgNum),64)]);
  }


void Resource::AddEvent(int16 ev, uint32 moc) 
  {
    /* DEEPLY BROKEN -- FIX!!!! */          
    Annotation *a;
    int16 i;
    EventMask |= BIT((MYABS(ev) % 16)+1);
    a = Annot(AnHead);
    if (!a)
      {
        a = NewAnnot(AN_EVENT,&AnHead);
        a->u.ev[I(0,5)].Event = ev;
        a->u.ev[I(0,5)].MsgOrCode = moc;
        return;
      }
    do {
      if (a->AnType == AN_EVENT)
        for(i=0;i!=5;i++) 
          if (!a->u.ev[I(i,5)].Event) {
            a->u.ev[I(i,5)].Event = ev;
            a->u.ev[I(i,5)].MsgOrCode = moc;
            return;
            }
      if (!a->Next) {
        a = NewAnnot(AN_EVENT,&a->Next);
        a->u.ev[I(0,5)].Event = ev;
        a->u.ev[I(0,5)].MsgOrCode = moc;
        return;
        }
      a = Annot(a->Next);
      }
    while(1);
  }

rID Resource::GetRes(int16 at, bool first)
  {
    static Annotation *a; static int8 n;
    if (first)
      { a = Annot(AnHead); n = 0; }
    if (a == NULL)
      return 0;
    do {
      if ((a->AnType == at) && n!=8 && a->u.sp[I(n,8)].sID) {
        n++;
        return a->u.sp[I(n-1,8)].sID;
        }
      else {
        n = 0;
        a = Annot(a->Next);
        }
      }
    while(a);
    return 0;
  }
          
        
      
void Resource::AddChatter(uint8,const char*m1,const char*m2,const char*m3,const char*m4,const char*m5) {}
void Resource::AddParam(int8 p,int16 v) {}
void Resource::AddArray(int8 p,int16 *list) {}

void Resource::AddSpecial(rID xID, int16 Chance, int16 Lev) {
    Dice d;
    d.Set(0,0,Lev&0xFF);
    AddSpecial(xID, Chance, d);
}

void Resource::AddSpecial(rID xID, int16 Chance, Dice& Lev) {
    Annotation *a;
    if (!AnHead) {
        a = NewAnnot(AN_DUNSPEC,&AnHead);
        goto Found;
    }
    a = Annot(AnHead);
    while(a) {
        if (a->AnType == AN_DUNSPEC)
            if (!a->u.ds[I(3,4)].xID) {
Found:
                for (int8 i=0;i!=4;i++)
                    if (!a->u.ds[I(i,4)].xID) {
                        a->u.ds[I(i,4)].xID     = xID;
                        a->u.ds[I(i,4)].Chance  = Chance & 0xFF;
                        a->u.ds[I(i,4)].Lev     = Lev;
                        return;
                    }
                Fatal("Fatal: Annotation wierdness.");
            }
        if (!a->Next)
            break;
        a = Annot(a->Next);
    }
    a = NewAnnot(AN_DUNSPEC,&a->Next);
    goto Found;
}

void Resource::AddPower(int8 apt,int8 pw,uint32 x,int16 pa,Dice*charge) {}


