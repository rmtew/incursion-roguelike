/* FEATURE.CPP -- Copyright 1999-2003 (c) Julian Mensch
     All of the member functions of Feature and its derived classes
   Door, Portal and Trap. By thematic extension, this file also
   includes many of the functions governing moving from level to
   level, as stairs permit.

     EvReturn Feature::Event(EventInfo &e)

     Portal::Portal(rID _pID) 
     EvReturn Portal::Event(EventInfo &e)
     EvReturn Portal::Enter(EventInfo &e)
     bool Portal::EnterDir(Dir d)

     Door::Door(rID fID)
     Door::~Door()
     void Door::SetImage()
     EvReturn Door::Event(EventInfo &e)

     void Trap::SetImage()
     Trap::~Trap()
     EvReturn Trap::Event(EventInfo &e)
     EvReturn Trap::TriggerTrap(EventInfo &e)

     void Game::EnterLimbo(hObj h, uint8 x, uint8 y, rID mID, 
            int8 Depth, int8 OldDepth, int32 Arrival, const char* Message)
     void Game::LimboCheck(Map *m)
     void Thing::MoveDepth(int16 NewDepth)
     void Player::MoveDepth(int16 NewDepth)
     Map* Game::GetDungeonMap(rID dID, int16 Depth, Player *pl, Map*TownLevel)

*/

#include "Incursion.h"

EvReturn Feature::Event(EventInfo &e) {
    EvReturn res;
    res = TFEAT(fID)->Event(e,fID);
    if (res == DONE || res == ERROR)
      return res;
    if (res == NOMSG)
      e.Terse = true;
    
  switch (e.Event) {
    case EV_TURN: 
      UpdateStati();
      return DONE; 

    case EV_PLACE:
      x = e.EXVal;
      y = e.EXVal;
      break; 

    case EV_STRIKE:
      if (e.ETarget != this) return NOTHING; 
      bool is_acting;
      is_acting = e.EActor->HasStati(ACTING);
      if (e.AType == A_KICK) {
        DPrint(e,NULL,"The <EActor> kicks the <Obj>.",this);
      }
      else if (e.EItem)
        DPrint(e,is_acting ? NULL : "You strike the <Obj> with your <EItem>.",
            "The <EActor> strikes the <Obj> with his <EItem>.",this);
      else
        DPrint(e, is_acting ? NULL : "You <Str> the door.",
            "The <EActor> <Str2> the door.", 
            Lookup(AttkVerbs1,e.AType), 
            Lookup(AttkVerbs2,e.AType));

      e.vDmg = e.Dmg.Roll();

      if (e.EActor->HasFeat(FT_SUNDER)) {
        e.vDmg *= 2; 
        e.strDmg += " (x2 Sunder)"; 
      }
      if (e.EItem && e.EItem->isWeapon() && 
          ((Weapon *)e.EItem)->HasQuality(WQ_SUNDERING)) {
        e.vDmg *= 2; 
        e.strDmg += " (x2 Sundering)"; 
      } 

      res = ReThrow(EV_DAMAGE,e);
      if (e.EItem2 && e.EItem2->isWeapon()) {
        ((Weapon *)e.EItem2)->DoQualityDmgSingle(e);
      }
      if (e.EItem && e.EItem->isWeapon()) {
        ((Weapon *)e.EItem)->DoQualityDmgSingle(e);
      }
      e.EActor->MakeNoise(max(10 - (e.EActor->SkillLevel(SK_MOVE_SIL) / 2),1));
      if (isDead())
        e.Died = true; 
      if (res != NOTHING) return res; 
      return DONE;

    case EV_DAMAGE:     
      if (e.ETarget != this) return NOTHING; 
      {
        int16 hard;
        String s;
      if (mHP == 0 || cHP == 0)
        return DONE; 
        
      hard = MaterialHardness(TFEAT(fID)->Material,e.DType);
      if (hard <= -1) {
//Immune:
        e.EActor->IPrint("The <Obj> is immune to <Str>.",
            this,Lookup(DTypeNames,e.DType));
        return DONE; 
      } 

      if (e.EActor->isPlayer())
        {
          if (e.Dmg.Number == 0 && e.Dmg.Sides == 0 && e.Dmg.Bonus == 0) {
            s = Format("%c%s:%c %d %s",-13,(const char*)(Name(0).Capitalize()),
                -7,
                e.vDmg,
                Lookup(DTypeNames,e.DType));
          } else { 
            s = Format("%c%s:%c %s%s %s",-13,(const char*)(Name(0).Capitalize()),
                -7,
                (const char*)e.Dmg.Str(),
                e.strDmg ? (const char*)e.strDmg : "",
                Lookup(DTypeNames,e.DType));
            s += Format(" = %d",e.vDmg);
          }

          s += Format(" vs. %d (%s)",hard,(const char*)Lookup(MaterialDescs,TFEAT(fID)->Material));

          e.aDmg = e.vDmg - hard; 

          s += Format(" <%d>[%s]<7>",
              e.vDmg > hard ? EMERALD : PINK,
              e.vDmg > hard ? (const char*)Format("%d damage",e.aDmg) : "unhurt");
          
      
          if (e.EPActor->Opt(OPT_STORE_ROLLS))
            e.EPActor->MyTerm->AddMessage(SC(XPrint(s)) + SC("\n"));
          e.EPActor->MyTerm->SetWin(WIN_NUMBERS3);
          e.EPActor->MyTerm->Clear();
          e.EPActor->MyTerm->Write(0,0,XPrint(s));
        }
      
      // ww: you can get stuck here if your STR is low kicking a door
      // forever ... no way to interrupt actions
      if (e.aDmg > 0)
      { 
        cHP -= e.aDmg;

        if (cHP <= 0) { 
          cHP = 0; 

          switch (e.DType) {
            case AD_SONI: VPrint(e,NULL,"The <Obj> shatters!",this); break;
            case AD_ACID: VPrint(e,NULL,"The <Obj> is eaten away!",this); break;
            case AD_FIRE: VPrint(e,NULL,"The <Obj> burns down!",this); break;
            default:      VPrint(e,NULL,"The <Obj> is destroyed!",this); break;
          }
          SetImage();
          Hear(e,20,"You hear a loud *crack*.");
          e.EActor->RemoveStati(ACTING); 
          e.Died = true; 

          if (e.EActor && e.EMap->FTrapAt(x,y) && e.EActor->isBeside(this)) {
            e.EActor->IPrint("The <Obj> was trapped!",this);
            e.EMap->FTrapAt(x,y)->TriggerTrap(e,false);
          } 
          Remove(true);
          return NOTHING; 
        } 
      } 
      VPrint(e,NULL,"The <Obj> holds!",this);
      //Message spam...
      //Hear(e,20,"You hear a muffled *thud*.");                         

      if (e.EActor->isPlayer() && e.EPActor->Opt(OPT_REPEAT_KICK) && e.AType == A_KICK) {
        if (!e.EActor->HasStati(ACTING,EV_SATTACK)) {
          e.EActor->RemoveStati(ACTING);
          e.EActor->GainPermStati(ACTING,this,SS_MISC,EV_SATTACK,20);
        } else {
          int16 mag = e.EActor->GetStatiMag(ACTING);
          if (mag <= 0) {
            e.EActor->HaltAction("not broken after 20 attacks", false); 
            if (e.EActor->HasStati(ACTING)) {
              e.EActor->RemoveStati(ACTING); 
              e.EActor->GainPermStati(ACTING,this,SS_MISC,EV_SATTACK,20);
            } else return ABORT; 
          } else {
            e.EActor->SetStatiMag(ACTING,-1,NULL,mag-1);
          } 
        } 
      }
      return DONE;
      }
  } 
		return NOTHING;
	}

void Thing::BoostRetry(int16 sk, Creature *c)
  {
    if (HasStati(RETRY_BONUS,sk,c))
      SetStatiMag(RETRY_BONUS,sk,c,
        GetStatiMag(RETRY_BONUS,sk,c)+2);
    else
      GainPermStati(RETRY_BONUS,c,SS_MISC,sk,+4);
  }


Portal::Portal(rID _pID) 
  : Feature(TFEAT(_pID)->Image,_pID,T_PORTAL)
  { }

EvReturn Portal::Event(EventInfo &e)
  {
    Creature *c; EvReturn res;
    switch(e.Event)
      {
        case PRE(EV_ENTER):
          res = TFEAT(fID)->Event(e,fID);
          if (res == DONE || res == ERROR)
            return res;
          if (res == NOMSG)
            e.Terse = true;
          if (c = (Creature*) e.EActor->GetStatiObj(DWARVEN_FOCUS))
            if (c->m == e.EActor->m)
              {
                e.EActor->IPrint("You will not flee while the <Obj> still lives!",c);
                return ABORT;
              }
          return NOTHING;
        case EV_ENTER:
          return Enter(e);
        default:
          return NOTHING;
      }
  }

EvReturn Portal::Enter(EventInfo &e)
  {
    int8 DepthMod; Map * new_m; 
    EvReturn res;
    
    res = TFEAT(fID)->Event(e,fID);
    if (res == DONE || res == ERROR)
      return res;
    if (res == NOMSG)
      e.Terse = true;

    e.EActor->Timeout += 30;

    switch(TFEAT(fID)->xval) {
        case POR_DOWN_STAIR:
        case POR_UP_STAIR:
            DepthMod = TFEAT(fID)->xval == POR_DOWN_STAIR ? 1 : -1;
            if (!m->dID) {
                Error("Up/down stairs located outside dungeon!");
                return ABORT;
            }
            if (e.EActor->isPlayer() && DepthMod == 1) {
                new_m = theGame->GetDungeonMap(m->dID,m->Depth+DepthMod,e.EPActor,NULL);
                if (new_m != NULL){
                    int16 nx = e.EActor->x, ny = e.EActor->y;
                    /* Out of bounds will result in random placement. Should be safe. HACK ALERT. */
                    if (new_m->InBounds(nx, ny)) {
                        //if ((TTER(new_m->PTerrainAt(nx,ny,e.EActor))->PEvent(EV_MON_CONSIDER,e.EActor,new_m->TerrainAt(nx,ny)) == ABORT))
                            if (!e.EActor->yn(XPrint("The stair leads to <Res>. Confirm unsafe action?",new_m->TerrainAt(nx,ny)),true))
                                return ABORT;
                    }
                }
            }

            e.EActor->MoveDepth(m->Depth + DepthMod,true);
            return DONE;
        case POR_DUN_ENTRY:
          if (!e.EActor->isPlayer())
            return ABORT;
          /* Changed TownLevel from m to NULL for Rope Trick; fix later */
          new_m = theGame->GetDungeonMap(TFEAT(fID)->xID,1,(Player*)e.EActor,NULL);
//          e.EActor->PlaceAt(new_m,e.EActor->GetX(),e.EActor->GetY());
          e.EActor->RemoveEffStati(fID);
          e.EActor->GainPermStati(ENTERED_AT,this,SS_MISC,0,0,fID);
          e.EActor->PlaceAt(new_m,new_m->EnterX,new_m->EnterY);
          return DONE;
        case POR_RETURN:
          Status *S;
          S = e.EActor->GetEffStati(
            ENTERED_AT,TFEAT(fID)->xID);
          if (S) 
            {
              Thing * t = oThing(S->h);
              ASSERT(t->m);
              e.EActor->PlaceAt(t->m,t->x,t->y);
              e.EActor->RemoveEffStati(((Feature*)t)->fID);
              return DONE;
            }
          else
            {
              /* Later, scan the entire game and place the player at
                 the first found Portal object that matches the Portal's
                 target. */
              Error("Returning to a portal you never entered!");
              return ABORT;
            }
         break;
        default:
          Error("Strange Portal xval (%d).");
          return DONE;
      }
  }

bool Portal::EnterDir(Dir d)
  {
    switch (TFEAT(fID)->xval)
      {
        case POR_UP_STAIR:      return d == UP;
        case POR_DOWN_STAIR:    return d == DOWN;
        default:                
          if ((Image & 0xFF) == '>')
            return d == DOWN;
          if ((Image & 0xFF) == '<')
            return d == UP;
          return d == CENTER;
      }
  }



Door::Door(rID fID):
	Feature(TFEAT(fID)->Image, fID,T_DOOR)
	{
		#ifndef WEIMER
		DoorFlags=0;
		if (!random(10))
			DoorFlags |= DF_OPEN;
		else
			{
				Flags |= F_SOLID;
				if (!random(2))
					DoorFlags |= DF_LOCKED;
			}
		if(!random(7))
			{
				DoorFlags &= ~DF_OPEN;
				DoorFlags |= DF_SECRET;
				if (!random(2))
					DoorFlags |= DF_LOCKED;
			}
		#else
    DoorFlags = DF_LOCKED;
    Flags |= F_SOLID; 

    // ww: all doors start out closed and locked
    // fjm: WHY?!
    
    if(!random(7)) 
      DoorFlags |= DF_SECRET;
  
    #endif
  
	}

Door::~Door()
  {
  }


void Door::SetImage()
	{
		if (!m || x == -1)
      return;

    Image = TFEAT(fID)->Image; 
      Image &= 0xFF00;

    if (HasStati(WIZLOCK)) 
      Image |= (RED << 12);
     

    if (DoorFlags & DF_OPEN)
      DoorFlags &= ~(DF_LOCKED|DF_SECRET);

    if (m->SolidAt(x,y-1) && m->SolidAt(x,y+1))
			DoorFlags |= DF_VERTICAL;
    else if (m->SolidAt(x-1,y) && m->SolidAt(x+1,y))
      DoorFlags &= ~DF_VERTICAL;
    else
      DoorFlags |= DF_BROKEN;

		if(DoorFlags & DF_SECRET)
			{

  			Flags |= F_INVIS;

				if(DoorFlags & DF_VERTICAL) {
          m->At(x,y).Glyph = m->At(x,y+1).Glyph;
          m->At(x,y).Shade = m->At(x,y+1).Shade;
          m->At(x,y).Solid = true;
          m->At(x,y).Opaque = true;
          }
  		  else {
          m->At(x,y).Glyph = m->At(x+1,y).Glyph;
          m->At(x,y).Shade = m->At(x+1,y).Shade;
          m->At(x,y).Solid = true;
          m->At(x,y).Opaque = true;
          }
			}
		else
			{
        Flags &= ~F_INVIS;

        m->At(x,y).Glyph = TTER(m->TerrainAt(x,y))->Image;
        m->At(x,y).Shade = TTER(m->TerrainAt(x,y))->HasFlag(TF_SHADE);
        if (DoorFlags & (DF_OPEN|DF_BROKEN)) {
          /* Used to refer to Terrain -- why not? */
          m->At(x,y).Solid = false;
          m->At(x,y).Opaque = false; 
          Flags &= ~F_SOLID;
        }
        else {
          m->At(x,y).Solid = true;
          m->At(x,y).Opaque = true;
          }

        if (DoorFlags & DF_BROKEN)
          Image += GLYPH_BDOOR;				
        else if(DoorFlags & DF_VERTICAL)
					Image += ((DoorFlags & DF_OPEN) ? '+' : 179);
				else
					Image += ((DoorFlags & DF_OPEN) ? '+' : 196);

			}

    if (theGame->InPlay())
      m->VUpdate(x,y);

	}


EvReturn Door::Event(EventInfo &e) {
    EvReturn res;
    int16 hard, hardwizlock, diff;
    String s;
    res = TFEAT(fID)->Event(e,fID);
    if (res == DONE || res == ERROR)
        return res;
    if (res == NOMSG)
        e.Terse = true;

    switch(e.Event) {
    case EV_TURN: 
        UpdateStati();
        return DONE; 
    case EV_OPEN:
        if (e.EActor->HasMFlag(M_NOHANDS)) { 
            e.EActor->IPrint("But you have no hands."); return ABORT; 
        } else if (e.EActor->onPlane() != PHASE_MATERIAL) { 
            e.EActor->IPrint("Your hands pass through the <Obj>.",this); 
            return ABORT; 
        } else if (DoorFlags & DF_BROKEN) { 
            e.EActor->IPrint("It's open, permanently.",this); 
            // ww: sanity
            Flags &= ~F_SOLID;
            DoorFlags |= DF_OPEN;
            DoorFlags &= ~DF_SECRET;
            m->At(x,y).Glyph = TTER(m->TerrainAt(x,y))->Image;
            m->At(x,y).Shade = TTER(m->TerrainAt(x,y))->HasFlag(TF_SHADE);
            m->At(x,y).Solid = false; 
            m->At(x,y).Opaque = TTER(m->TerrainAt(x,y))->HasFlag(TF_OPAQUE);
            SetImage();
            return ABORT; 
        } else if (DoorFlags & DF_OPEN) { 
            e.EActor->IPrint("The <Obj> is already open.",this); return ABORT; 
        } else if (DoorFlags & DF_LOCKED && !HasStati(WIZLOCK,-1,e.EActor) &&
            !e.EActor->HasEffStati(HOME_REGION,m->RegionAt(x,y))) {

                if (e.EActor->HasStati(RAGING))
                    goto TryKicking;

                if (HasStati(TRIED, SK_LOCKPICKING, e.EActor)) { 
                    e.EActor->IPrint("You have already tried to pick the lock on the <Obj>.",this);
                    if (e.EActor->isPlayer() && e.EPActor->Opt(OPT_AUTOKICK))
                        return e.EActor->TryToDestroyThing(this);
                    else
                        return DONE;
                } 
                e.EActor->IPrint("The <Obj> is locked.",this);
                /* Players can attempt to pick locks without the skill; monsters
                can not. This avoids players herding around charmed creatures
                as lockpicks, and other abuses. 
                ww: That is not a problem, really! 
                fjm: It may be, once I add the Diplomacy stuff. Also, monsters
                currently move to the player too quickly -- you meet a whole
                bunch, and then you wander through a bunch of empty rooms. This
                might be part of the issue.
                */
                if (e.EActor->isMonster() && !e.EActor->HasSkill(SK_LOCKPICKING))
                    return ABORT;

                if (!((e.EActor->isPlayer() && ((Player *)e.EActor)->Opt(OPT_AUTOOPEN)) || e.EActor->yn("Pick the lock?",true)))
                    return ABORT;
                // ww: trying to pick it takes time: a full-round action, pass or
                // fail


                e.EActor->Timeout += 30;
                diff = 14 + m->Depth;
                if (HasStati(WIZLOCK) && !HasStati(WIZLOCK,-1,e.EActor)) {
                    e.EActor->IPrint("The <Obj> is more difficult to pick.",this);
                    diff += 10; 
                }
                if (e.EActor->SkillCheck(SK_LOCKPICKING,diff,true,
                    GetStatiMag(RETRY_BONUS,SK_LOCKPICKING,e.EActor),"retry")) { 
                        e.EActor->IDPrint("You pick the lock!",
                            "The <Obj> picks the lock on the <Obj>.",
                            e.EActor, this);
                        DoorFlags &= ~DF_LOCKED;
                        RemoveStati(TRIED,SS_MISC,SK_LOCKPICKING); 

                        if (!HasStati(TRIED,DF_LOCKED,this) && 
                            !HasStati(SUMMONED,-1,this)) {
                                // see DisarmTrap() 
                                e.EActor->GainXP(90 + (diff - 14) * 10);
                                GainPermStati(TRIED,this,SS_ATTK,DF_LOCKED);
                        }

                } else { 
                    e.EActor->IDPrint("You fail to pick the lock on the <Obj2>. (You can try again after resting.)",
                        "The <Obj1> tries to pick the lock on the <Obj2>, but fails.",
                        e.EActor, this);
                    BoostRetry(SK_LOCKPICKING,e.EActor);
                    GainTempStati(TRIED,e.EActor,-2,SS_MISC,SK_LOCKPICKING); 

TryKicking:
                    if (e.EActor->isPlayer() && 
                        ((Player *)e.EActor)->Opt(OPT_AUTOKICK)) {
                            return e.EActor->TryToDestroyThing(this); 
                    } 
                    return ABORT; 
                }
        }
        // else: just open it
        if (HasStati(WIZLOCK,-1,e.EActor)) {
            e.EActor->Timeout += 0;
            e.EActor->IPrint("The <Obj> opens at your mental command.",this);
        } else {
            e.EActor->Timeout += 15;
            e.EActor->IPrint("The <Obj> opens.",this);
        }
        e.EActor->IPrint("The door opens.");
        Flags &= ~F_SOLID;
        DoorFlags |= DF_OPEN;
        DoorFlags &= ~DF_SECRET;
        m->At(x,y).Glyph = TTER(m->TerrainAt(x,y))->Image;
        m->At(x,y).Shade = TTER(m->TerrainAt(x,y))->HasFlag(TF_SHADE);
        m->At(x,y).Solid = false; 
        m->At(x,y).Opaque = TTER(m->TerrainAt(x,y))->HasFlag(TF_OPAQUE);

        SetImage();
        /*
        if (e.EActor->isPlayer())
        if (TREG(m->RegionAt(x,y))->HasFlag(RF_AUTODESC))
        if (!REGMEM(m->RegionAt(x,y))->Seen) {
        REGMEM(m->RegionAt(x,y))->Seen = true;
        ((Player*)e.EActor)->MyTerm->Box(WIN_SCREEN,0,AZURE,GREY,
        m->DescribeReg(x,y));
        }*/
        return DONE;
    case EV_CLOSE:
        if (e.EActor->HasMFlag(M_NOHANDS)) {
            e.EActor->IPrint("But you have no hands.");
            return ABORT;
        }
        if (e.EActor->onPlane() != PHASE_MATERIAL) {
            e.EActor->IPrint("Your hands pass through the <Obj>.",this);
            return ABORT;
        }
        if (DoorFlags & DF_BROKEN) {
            e.EActor->IPrint("That door is broken, and no longer closes.");
            return ABORT;
        }
        if (!(DoorFlags & DF_OPEN)) {
            e.EActor->IPrint("It's already closed.");
            return ABORT;
        }
        e.EActor->IPrint("The <Obj> closes.",this);
        e.EActor->Timeout += 15;
        Flags |= F_SOLID;
        DoorFlags &= ~DF_OPEN;
        SetImage();
        return DONE; 
    case EV_DAMAGE:     
        if (e.ETarget != this)
            return NOTHING;

        if (e.EActor->onPlane() != onPlane()) {
            DPrint(e, "Your attack passes through the <ETarget>.",
                "The <EActor>'s attack passes through the <ETarget>.");
            return DONE;
        }
        if ((DoorFlags & (DF_OPEN|DF_BROKEN)) && e.AType == A_KICK)
            return DONE; 
        if (e.AType == A_SWNG && (!e.EItem || !e.EItem->HasIFlag(WT_BLUNT)))
            e.vDmg /= 3;
        if (e.DType == AD_PIERCE && !(e.EItem && e.EItem->HasIFlag(WT_BLUNT))
            && !(e.EItem && e.EItem->HasIFlag(WT_SLASHING)))
            goto Immune;

        hard = MaterialHardness(TFEAT(fID)->Material,e.DType);
        if (hard <= -1) {
Immune:
            e.EActor->IPrint("The <Obj> is immune to <Str>.",
                this,Lookup(DTypeNames,e.DType));
            return DONE; 
        } 

        hardwizlock = 0;
        if (HasStati(WIZLOCK))
            hardwizlock = 2;

        if (e.EActor->isPlayer()) {
            if (e.Dmg.Number == 0 && e.Dmg.Sides == 0 && e.Dmg.Bonus == 0) {
                s = Format("%c%s:%c %d %s",-13,(const char*)(Name(0).Capitalize()),
                    -7,
                    e.vDmg,
                    Lookup(DTypeNames,e.DType));
            } else { 
                s = Format("%c%s:%c %s%s %s",-13,(const char*)(Name(0).Capitalize()),
                    -7,
                    (const char*)e.Dmg.Str(),
                    e.strDmg ? (const char*)e.strDmg : "",
                    Lookup(DTypeNames,e.DType));
                s += Format(" = %d",e.vDmg);
            }

            s += Format(" vs. %d (%s)",hard,(const char*)Lookup(MaterialDescs,TFEAT(fID)->Material));

            if (hardwizlock) {
                hard *= hardwizlock;
                s += Format(" x%d (wizlock) = %d",hardwizlock,hard);
            }

            s += Format(" <%d>[%s]<7>",
                e.vDmg > hard ? EMERALD : PINK,
                e.vDmg > hard ? (const char*)Format("%d damage",e.aDmg) : "unhurt");

            if (e.EPActor->Opt(OPT_STORE_ROLLS))
                e.EPActor->MyTerm->AddMessage(SC(XPrint(s)) + SC("\n"));
            e.EPActor->MyTerm->SetWin(WIN_NUMBERS3);
            e.EPActor->MyTerm->Clear();
            e.EPActor->MyTerm->Write(0,0,XPrint(s));
        }

        e.aDmg = e.vDmg - hard; 

        // ww: you can get stuck here if your STR is low kicking a door
        // forever ... no way to interrupt actions
        if (e.aDmg > 0) { 
            cHP -= e.aDmg;

            if (cHP <= 0) { 
                cHP = 0; 

                switch (e.DType) {
                case AD_SONI: VPrint(e,NULL,"The <Obj> shatters!",this); break;
                case AD_ACID: VPrint(e,NULL,"The <Obj> is eaten away!",this); break;
                case AD_FIRE: VPrint(e,NULL,"The <Obj> burns down!",this); break;
                default:      VPrint(e,NULL,"The <Obj> breaks open!",this); break;
                }
                DoorFlags &= (DF_LOCKED | DF_SECRET);
                DoorFlags |= DF_OPEN | DF_BROKEN; 
                SetImage();
                Hear(e,20,"You hear a loud *crack*.");
                e.EActor->RemoveStati(ACTING); 
                e.Died = true; 

                if (e.EActor->isCharacter() && !HasStati(TRIED,945))
                    e.EActor->Exercise(A_STR,random(12)+1,ESTR_DOOR,35);

                if (e.EActor && e.EMap->FTrapAt(x,y) && e.EActor->isBeside(this)) {
                    e.EActor->IPrint("The <Obj> was trapped!",this);
                    e.EMap->FTrapAt(x,y)->TriggerTrap(e,false);
                } 
                return NOTHING; 
            } 
        } 

        if (e.EActor->isCharacter() && !HasStati(TRIED,945))
            GainPermStati(TRIED,e.EActor,SS_MISC,945);

        // fjm: Short, onomatopic messages for repeating strikes
        // to objects avoid annoying message spam when battering
        // on doors in the dungeon.
        e.EActor->IPrint("WHAMM!");
        if (e.EActor->isPlayer() && !e.EPActor->Opt(OPT_REPEAT_KICK))
            e.EActor->IPrint("The <Obj> holds!",this);

        //Hear(e,20,"You hear a muffled *thud*.");                         

        if (e.EActor->isPlayer() && e.EPActor->Opt(OPT_REPEAT_KICK) &&
            e.AType == A_KICK) {
                if (!e.EActor->HasStati(ACTING,EV_SATTACK)) {
                    e.EActor->RemoveStati(ACTING);
                    e.EActor->GainPermStati(ACTING,this,SS_MISC,EV_SATTACK,20);
                } else {
                    int16 mag = e.EActor->GetStatiMag(ACTING);
                    if (mag <= 0) {
                        e.EActor->HaltAction("not broken after 20 attacks", false); 
                        if (e.EActor->HasStati(ACTING)) {
                            e.EActor->RemoveStati(ACTING); 
                            e.EActor->GainPermStati(ACTING,this,SS_MISC,EV_SATTACK,20);
                        } else return ABORT; 
                    } else {
                        e.EActor->SetStatiMag(ACTING,-1,NULL,mag-1);
                    } 
                } 
        }
        return DONE;

    default:
        return NOTHING;
    }
}

/***************************************************************\
 *                              TRAPS                          *
\***************************************************************/

void Trap::SetImage()
  {
    if (TrapFlags & TS_DISARMED)
      Image = GLYPH_DISARMED | (TEFF(tID)->ef.cval * 256);
    else
      Image = GLYPH_TRAP | (TEFF(tID)->ef.cval * 256);
    if (TrapFlags & TS_FOUND)
			Flags &= ~F_INVIS;
    else
			Flags |= F_INVIS;
    if (theGame->InPlay() && x != -1)
      m->Update(x,y);
  
  }



Trap::~Trap()
  {
    TrapFlags |= TS_FOUND;
    SetImage();
  }

EvReturn Trap::Event(EventInfo &e)
  {
    return NOTHING;
  }

EvReturn Trap::TriggerTrap(EventInfo &e, bool foundBefore)
  {
    EvReturn r; Creature *cr;
    // ww: if the trap is not mundane, you get a saving throw vs. magic to
    // dodge it -- otherwise the polymorph trap always works on the drow
    // elf with +6 million magic resistance, which is not expected
    TEffect *te = TEFF(tID);

    if (TrapFlags & TS_DISARMED)
      return NOTHING;

    if (!foundBefore) 
      for (int i=0;i!=MAX_PLAYERS;i++)
        if (theGame->GetPlayer(i))
          if (theGame->GetPlayer(i)->Perceives(e.EActor))
          {
            TrapFlags |= TS_FOUND;
            SetImage();
          }

    uint32 saveType = SA_TRAPS;
    if (!(te->HasFlag(EF_MUNDANE))) saveType |= SA_MAGIC;

    int16 trapDC = 15 + te->Level;
    if (!foundBefore) trapDC += 5; 

    if ((foundBefore && e.EActor->HasFeat(FT_FEATHERFOOT)) ||
        (te->ef.sval != NOSAVE && 
         e.EActor->SavingThrow(te->ef.sval,trapDC,saveType))) {
        e.EActor->IDPrint("You avoid the effects of a <Obj2>!",
             "The <Obj> avoids the effects of a <Obj2>!",e.EActor,this);
        return DONE;
    }
    e.EActor->IDPrint("You fall victim to a <Obj2>!",
             "The <Obj> falls victim to a <Obj2>!",e.EActor,this);

    /* Move this up here so that dispelling traps don't go into an
       infinite loop dispelling themselves! */
    TrapFlags |= TS_DISARMED;
    SetImage();
    

    e.isTrap = true;
    e.vCasterLev = 12;
    e.eID = tID;

    e.EItem = (Item*)this;
    e.EVictim = e.EActor;

    e.EVictim->Reveal(false);
    e.EVictim->MakeNoise(30);
    if (e.EVictim->HasStati(ACTING))
      e.EVictim->HaltAction("trap triggered");
    r = ReThrow(EV_EFFECT,e);
    if (r != DONE)
      return r;
    
    if (cr = (Creature*) GetStatiObj(RESET_BY))
      if (e.EActor->isHostileTo(cr))
        {
          /* If your trap killed a hostile monster, get kill XP... */
          if (e.EActor->isDead()) {
            cr->KillXP(e.EActor);
            cr->gainFavour(FIND("Semirath"),e.EActor->ChallengeRating()*50,false,true);
            }
          /* ...but most traps don't kill, they weaken, so give a
             special trap award for using them -- but only give it
             once per trap! */
          if (!HasStati(XP_GAINED,-1,cr))
            {
              cr->GainXP(100 + 25*e.EActor->ChallengeRating());
              cr->gainFavour(FIND("Semirath"),e.EActor->ChallengeRating()*50,false,true);
              GainPermStati(XP_GAINED,cr,SS_MISC);
            }
        }
    
    if (m && x != -1)
      m->Update(x,y);
    return r;  
    
  }


/***************************************************************
 * Limbo:                                                      *
 *   Limbo is where monsters go when they leave a level. A     *
 * monster who falls into a level-dropping pit enters limbo.   *
 * Monsters that pursue the player across levels enter limbo   *
 * when the player does, and emerge on the player's new level  *
 * a few turns later. When the player climbs the stairs and    *
 * there are monsters nearby that have him as their current    *
 * target, there is a chance each decides to use the stairs,   *
 * and is thus tossed into limbo, following the player to the  *
 * next level. And so forth.                                   *
 ***************************************************************/

void Game::EnterLimbo(hObj h, uint8 x, uint8 y, rID mID, 
            int8 Depth, int8 OldDepth, int32 Arrival, const char* Message)
  {
    LimboEntry *lr;
    lr = Limbo.NewItem();
    lr->h = h; lr->x = x; lr->y = y;
    lr->mID = mID; lr->Depth = Depth;
    lr->OldDepth = lr->OldDepth;
    lr->Arrival = Arrival;
    lr->Message = Message;
    oThing(h)->Remove(false);
  }

void Game::LimboCheck(Map *m) {
    int16 i;
    Player *p;

Restart:
    for(i=0;Limbo[i];i++)
      if (Limbo[i]->Arrival >= Turn) {
        if (Limbo[i]->Target)
          if (oThing(Limbo[i]->Target)->isPlayer())
            {
              p = oPlayer(Limbo[i]->Target);
              if (Limbo[i]->x > 0)
                oThing(Limbo[i]->h)->PlaceAt(p->m, Limbo[i]->x,Limbo[i]->y);
              else
                /* PlaceAt calls PlaceNear automatically. */
                oThing(Limbo[i]->h)->PlaceAt(p->m,p->x,p->y);
              //oThing(Limbo[i]->h)->IDPrint(NULL,Limbo[i]->Message);
              Limbo.Remove(i);
              goto Restart;
            }
        if (/*m->mID == Limbo[i]->mID ||*/ m->dID == Limbo[i]->mID) 
          if (m->Depth == Limbo[i]->Depth)
            {
              oThing(Limbo[i]->h)->PlaceAt(p->m, Limbo[i]->x,Limbo[i]->y);
              //oThing(Limbo[i]->h)->IDPrint(NULL,Limbo[i]->Message);
              Limbo.Remove(i);
              goto Restart;
            }
        }
  }

void Thing::MoveDepth(int16 NewDepth, bool safe)
  {
    Remove(true);
  }

void Player::MoveDepth(int16 NewDepth, bool safe)
  {
    static Thing *GoWith[64]; Creature *c;
    rID mID; Map *new_m = NULL; int16 nx,ny,i, gwc;
    StoreLevelStats((uint8)m->Depth);
    { theGame->SaveGame(*this); }

    RemoveStati(SPRINTING);
    RemoveStati(ENGULFED);
    RemoveStati(GRAPPLED);
    RemoveStati(GRAPPLING);
    RemoveStati(ELEVATED);

    if (!RES(m->dID) || RES(m->dID)->Type != T_TDUNGEON)
      {
        IPrint("Apparently, the fall was fatal.");
        ThrowDmg(EV_DEATH,AD_BLUNT,0,"falling",this,this);
        return;
      }
    if (NewDepth <= 0)
      {
        mID = RES(m->dID)->GetConst(ABOVE_DUNGEON);
        if (!mID)
          return;       
        if (RES(mID)->Type == T_TDUNGEON) {
          NewDepth += (int16)(RES(mID)->GetConst(DUN_DEPTH));
          new_m = theGame->GetDungeonMap(mID,NewDepth,this);
          }
        else if (RES(mID)->Type == T_TREGION)
          new_m = NULL; /* Map::LoadMap(mID); */
        else
          Error("Strange resource as ABOVE_DUNGEON in MoveDepth!");          
      }
    else if (RES(m->dID)->GetConst(DUN_DEPTH) &&
      (int16)(RES(m->dID)->GetConst(DUN_DEPTH)) < NewDepth) {
        NewDepth -= (int16)(RES(m->dID)->GetConst(DUN_DEPTH));
        mID = RES(m->dID)->GetConst(BELOW_DUNGEON);
        if (RES(mID)->Type == T_TDUNGEON)
          new_m = theGame->GetDungeonMap(mID,NewDepth,this);
        else if (RES(mID)->Type == T_TREGION)
          new_m = NULL; /* Map::LoadMap(mID); */
        else
          Error("Strange resource as BELOW_DUNGEON in MoveDepth!");          
      }
    else
      new_m = theGame->GetDungeonMap(m->dID,NewDepth,this);

    if (new_m) {
      /* Later, compensate for dimensions. */
      nx = x;
      ny = y;
      /* Later, allow maps to choose their own entry points from
         any other map, so that, for example, when entering $"Erisia"
         from $"Mines of Moria", the player is always placed at the
         feature $"ancient passage". */
      
      gwc = 0;
      MapIterate(m,c,i)
        if (c->isMonster()) 
          if (c->ts.isLeader(this))
            {
              GoWith[gwc++] = c;
              c->Remove(false);
            }
      
      if (c = (Creature*) GetStatiObj(DWARVEN_FOCUS))
        if (c->m == m)
          {
            LoseXP(TotalLevel()*200);
            IPrint("You feel your heart grow leaden, as you realize "
                   "true vengeance will forever elude you.");
            SetStatiObj(DWARVEN_FOCUS,-1,NULL);            
          }    

      while ((!new_m->InBounds(nx,ny)) || 
                new_m->SolidAt(nx,ny) || 
                new_m->At(nx,ny).isVault) 
        {
          nx = 2 + random(new_m->SizeX()-4);
          ny = 2 + random(new_m->SizeY()-4);
        }

      if (safe && new_m->FallAt(nx,ny))
        for(i=0;i!=8;i++)
          if (!new_m->FallAt(nx+DirX[i],ny+DirY[i]))
            { nx += DirX[i]; ny += DirY[i]; break; }

      PlaceAt(new_m,nx,ny,true);

      for(i=0;i!=gwc;i++)
        GoWith[i]->PlaceAt(new_m,nx,ny);


      }

    MyTerm->RefreshMap();

    NoteArrival();

    if (safe) {
      MapIterate(m,c,i)
        if (c->isPlayer())
          c->Timeout = 0;
        else if (c->isCreature())
          c->Timeout = max(c->Timeout,1);
      }

  }
  
void Player::NoteArrival()
  {
    int16 dn, i;
    dn = -1;
    for (i=0;i!=MAX_DUNGEONS;i++)
      if (m && m->dID && m->dID == theGame->DungeonID[i])
        dn = i;
    if (dn != -1)
      if (MaxDepths[dn] < m->Depth)
        {
          MaxDepths[dn] = m->Depth;
          AddJournalEntry(XPrint("You arrive at <Num>' in the <Res>.",
            m->Depth * 10, m->dID));
        }
  }

Map* Game::GetDungeonMap(rID dID, int16 Depth, Player *pl, Map*TownLevel)
  {
    int16 i,n; Map *m; bool ip;

    ip = PlayMode; PlayMode = false;

    for(i=1;i!=MAX_DUNGEONS && DungeonID[i];i++)
      if (DungeonID[i] == dID)
        goto Found;

    if (i == MAX_DUNGEONS)
      Fatal("Too many dungeons; max specified by MAX_DUNGEONS is %d.",MAX_DUNGEONS);
    if (Depth > MAX_DUNGEON_LEVELS)
      Fatal("Too deep in dungeon; MAX_DUNGEON_LEVELS is %d.",MAX_DUNGEON_LEVELS);

    DungeonID[i] = dID;
    DungeonSize[i] = min(MAX_DUNGEON_LEVELS, (int16)(RES(dID)->GetConst(DUN_DEPTH)));
    DungeonLevels[i] = (hObj*)malloc(sizeof(hObj)*min(MAX_DUNGEON_LEVELS,
                          RES(dID)->GetConst(DUN_DEPTH)+1));
    memset(DungeonLevels[i],0,sizeof(hObj)*(RES(dID)->GetConst(DUN_DEPTH)+1));
    Found:
    n = i;

    /* Set DungeonLevels[n][0] to whatever is directly, geographically
       above the first dungeon level. This will allow us to match up stair
       locations with the city (or whatever) above the dungeon. */
    if (TownLevel)
      DungeonLevels[n][0] = TownLevel->myHandle;

    
    /* If we jump straight from dungeon level 4 to level 15, we have to
       generate (and then save) all the levels between, in order to match
       up stair locations, chasms, and other cross-level features. */

    for(i=1;i<=Depth;i++)
      if (!DungeonLevels[n][i])
        {
          m = new Map();
          DungeonLevels[n][i] = m->myHandle;
          m->Generate(dID,i,oMap(DungeonLevels[n][i-1]),(int8)pl->GetAttr(A_LUC));
        }

    /*
    for(i=1;i!=Depth;i++)
      theGame->Detach(DungeonLevels[n][i]);
    */

    
    PlayMode = ip;
    return oMap(DungeonLevels[n][Depth]);
  }

void Feature::StatiOn(Status s)
  {
    int16 col;
    if (s.Nature == MY_GOD && (Flags & F_ALTAR))
      if (col = (int16)TGOD(s.eID)->GetConst(ALTAR_COLOUR))
      {
        Image &= 0xFF;
        Image |= col * 256;
        if (m)
          m->Update(x,y);
      }
  }

void Feature::StatiOff(Status s)
  {
    EventInfo xe;
    switch (s.Nature)
      {
        case SUMMONED:
          IDPrint(NULL,"The <Obj> winks out of existence.",this);
          if (m && m->InBounds(x,y)) {
            m->At(x,y).Glyph = TTER(m->TerrainAt(x,y))->Image;
            m->At(x,y).Shade = TTER(m->TerrainAt(x,y))->HasFlag(TF_SHADE);
            m->At(x,y).Solid = TTER(m->TerrainAt(x,y))->HasFlag(TF_SOLID);
            m->At(x,y).Opaque = TTER(m->TerrainAt(x,y))->HasFlag(TF_OPAQUE);
          }
          Remove(true);
         break;

      }
    if (theGame->InPlay()) {
      SetImage();
    }
  }
