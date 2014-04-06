/* MOVE.CPP -- Copyright (c) 1 999-2003 Julian Mensch
     Contains two *long* functions that handle all the specifics
   of creatures moving from square to square on the map, whether
   by walking, jumping or being pushed.

     EvReturn Creature::Walk(EventInfo &e)
     void Creature::TerrainEffects()

*/

#include "Incursion.h"
extern Attack Punch;
int8 DirX[] = { 0,  0, 1,-1,  1, -1, 1,-1, 0 };
int8 DirY[] = { -1, 1, 0, 0, -1, -1, 1, 1, 0 };

/* North is similar to North, Northeast, Northwest;
   Southeast is similar to South, Southeast, East. */
bool isSimilarDir(Dir d, Dir d2)
  {
    return ((DirX[d] == DirX[d2] && abs(DirY[d] - DirY[d2]) < 2) ||
            (DirY[d] == DirY[d2] && abs(DirX[d] - DirX[d2]) < 2));
  }

EvReturn Creature::Walk(EventInfo &e)
	{
		int16 tx,ty,ox,oy; Feature *fe; Thing *t; Status *s; int8 szChange = 0;
    String Buff; int16 i; bool jcheck; Trap *tr;
    Creature *cr, *dis; char ch, MoveSilRoll; int16 max_range, range, CloseDC,
                                                                      SizeMod;
    static Creature *ClosingWith[8]; int8 cwc, DC; bool CloseConfirm = false;
    Door *d; bool found, think;

    dis = NULL;
    
    if (!m) return ABORT; 
    if (HasStati(MOUNT)) 
      {
        if (e.Event != EV_MOVE)
          return ABORT;
        e.ETarget = GetStatiObj(MOUNT);
        i = ReThrow(EV_MOVE,e);
        e.ETarget->Timeout = Timeout;
        Timeout = 0;
        e.ETarget = this;
        return i;
      }

  if (e.Event != EV_PUSH) {
    if (e.isDir) 
      LastMoveDir = e.EDir;
    else if (e.isLoc)
      LastMoveDir = DirTo(e.EXVal,e.EYVal);
  }

  switch (e.Event) {
    case EV_JUMP:
      if (HasStati(GRAPPLED) || HasStati(GRABBED) || HasStati(GRAPPLING) || HasStati(PRONE) ||
          HasStati(STUCK))
      {
        IPrint("You can't jump while grappling, prone or stuck.");
        return ABORT;
      }
      if (isAerial())
      {
        IPrint("You can't jump while already in the air.");
        return ABORT;
      }
      if (!m->LineOfFire(x,y,e.EXVal,e.EYVal,this))
      {
        IPrint("You don't have an unobstructed path for that jump!");
        return ABORT;
      } 
      if (ThrowXY(EV_JUMP_OFF,x,y,e.EActor,NULL,NULL,NULL) == ABORT) 
        return ABORT; 

      /* it was 3+X/5 -- 30 feet is a *long* way for a non-monk
       * standing broad jump! */
      max_range = 2 + e.EActor->SkillLevel(SK_JUMP)/4;
      /* but you might want to jump anyway, if you have Mantis Leap 
         if (dist(x,y,e.EXVal,e.EYVal) <= 1)
         {
         IPrint("You can just walk that distance.");
         return ABORT;
         }         */
      DC = 10 + dist(x,y,e.EXVal,e.EYVal)*5;
      if (HasStati(MOUNTED)) {
        int the_dist = dist(x,y,e.EXVal,e.EYVal);
        Creature * mount = (Creature *)GetStatiObj(MOUNTED);
        if (the_dist > 2 + mount->SkillLevel(SK_JUMP)/5) {
          IPrint("That square is beyond your maximum possible mounted jumping distance.");
          return ABORT;
        } 
        if (isPlayer())
          if (!yn(Format("You have a %d~ chance of convincing your mount and a %d~ chance of making the jump. Jump?",
                  100-max(0,(DC-SkillLevel(SK_RIDE))*5),
                  100-max(0,(DC-mount->SkillLevel(SK_JUMP))*5)
                  )))
            return ABORT;

        if (!SkillCheck(SK_RIDE,DC,true,false)) {
          Timeout += 30; 
          IPrint("You fail to convince your mount to make the jump.");
          return ABORT;
        } 
        jcheck = (mount->SkillCheck(SK_JUMP,DC)); 
      } else { 
        if (dist(x,y,e.EXVal,e.EYVal) > max_range) {
          IPrint("That square is beyond even your maximum possible jumping distance.");
          return ABORT;
        }
        if (isPlayer())
          if (!yn(Format("You have a %d~ chance of success. Jump?",
                  100-max(0,(DC-SkillLevel(SK_JUMP))*5))))
            return ABORT;
        jcheck = (SkillCheck(SK_JUMP,DC)); 
      } 

      if (!jcheck) { 
        IPrint("You stumble instead of leaping.");
        int16 count;
        count = 0;
        if (m->FTrapAt(x + DirX[DirTo(e.EXVal,e.EYVal)],y + DirY[DirTo(e.EXVal,e.EYVal)]) ||
            m->FallAt(x + DirX[DirTo(e.EXVal,e.EYVal)],y + DirY[DirTo(e.EXVal,e.EYVal)]))
          {
            tx = x + DirX[DirTo(e.EXVal,e.EYVal)];
            ty = y + DirY[DirTo(e.EXVal,e.EYVal)];
          }
        else do {
          i = random(100);
          count++;
          if (count == 50)
            return ABORT;
          tx = (e.EXVal*i + x*(100-i)) / 100;
          ty = (e.EYVal*i + y*(100-i)) / 100;
        }
        while((dist(x,y,tx,ty) > max_range) || !m->OpenAt(tx,ty));
        e.EXVal = tx; e.EYVal = ty;
      }
      e.isLoc = true; 
      Reveal(true);
      IDPrint(NULL,"An <Obj> leaps through the air!",e.EActor);
      ThrowXY(EV_LAND,e.EXVal,e.EYVal,e.EActor,NULL,NULL,NULL);
      if (!(HasStati(TUMBLING) && SkillCheck(SK_TUMBLE,20,false)))
        if (!HasFeat(FT_ACROBATIC))
          ProvokeAoO();
      break;
    case EV_MOVE:
      if (HasStati(GRABBED) || HasStati(GRAPPLED) || HasStati(GRAPPLING))
      {
        ThrowVal(EV_SATTACK,A_ESCA,this);
        if (HasStati(GRABBED) || HasStati(GRAPPLED) || HasStati(GRAPPLING))
          return ABORT;
      }

      if (HasStati(STUCK)) {
        int power = GetStatiMag(STUCK);

        if (SkillCheck(SK_ESCAPE_ART,15 + power,true,false)) {
          IPrint("You tear free!");
          Exercise(A_STR,random(6)+1,ESTR_UNSTUCK,25);
          Timeout += 1500 / (100 + Mod(A_DEX)*5);
          while (HasStati(STUCK))
            RemoveStati(STUCK);
        } else {  
          IPrint("You remain stuck fast!");
          Timeout += 20;
        }
        return DONE;
      }

      if (HasStati(PRONE)) {
        bool instant = false;
        if (!((this->isPlayer() && thisp->Opt(OPT_AUTO_STAND)) ||
              yn("You can't move while prone. Stand up?",true)))
          return ABORT;
        if (HasFeat(FT_INSTANT_STAND)) {
          if (e.EActor->GetAttr(A_SAV_REF) > e.EActor->SkillLevel(SK_BALANCE)) {
            if (e.EActor->SavingThrow(REF,15))
              instant = true;
            }
          else if (e.EActor->SkillCheck(SK_BALANCE,15,true))
            instant = true;
          }
        do
          RemoveStati(PRONE); 
        while (HasStati(PRONE));
        
        if (!instant)
          { 
            DPrint(e,"You stand up.", "The <EActor> stands up.");   
            Timeout += 2500 / (100 + Mod(A_DEX)*5);
            ProvokeAoO();
            if (isDead()) 
              return ABORT; 
          }
        else
          DPrint(e,"You leap gracefully to your feet!",
                   "The <EActor> leaps gracefully to <her:EActor> feet!");    
        return DONE; 
        }

      if (e.isDir && e.EDir == CENTER)
      {
        if (!Timeout)
          Timeout += 30;
        return DONE;
      }
      break;
    case EV_PUSH:
      if (HasStati(STUCK)) {
        /* A hostile effect causing knockback (force bolt, bull rush, etc.)
           can't tear you free from webbing, but it does a little extra damage
           because you have no "give". */
        if (e.EVictim->isHostileTo(this))
          ThrowDmg(EV_DAMAGE,AD_BLUNT,Dice::Roll(1,6),"being battered",e.EVictim,this);
        return ABORT;
      }
      /* One of the benefits of being prone is that you don't get moved
         by bull rushes or force bolts. */
      if (HasStati(PRONE))
        return ABORT;
      /* This could become more elaborate later, of course. */
      if (HasStati(GRAPPLED) || HasStati(GRABBED))
        return ABORT;
      if (HasStati(GRAPPLING))
      {
        cr = oCreature(GetStati(GRAPPLING)->h);
        if (!cr)
          {
            Status *s = GetStati(GRAPPLING);
            Error("Creature has GRAPPLING without a grappler!");
            return ABORT;
          }
        cr->RemoveStati(GRABBED,SS_ATTK,-1,-1,this);
        cr->RemoveStati(GRAPPLED,SS_ATTK,-1,-1,this);
        RemoveStati(GRAPPLING,SS_ATTK,-1,-1,cr);
      }      
      break;
  }

  if ((Attr[A_MOV]*5+100) <= 0)
    return ABORT;


  if (HasStati(CONFUSED) && !SkillCheck(SK_BALANCE,18,false)) {
    if (HasStati(CHARGING)) {
      if (!SavingThrow(REF,20))
      {
        IDPrint("You stumble!", "The <Obj> stumbles!",this);
        RemoveStati(CHARGING);
        ThrowDmg(EV_DAMAGE,AD_TRIP,-1,"charging while confused",e.EActor,e.EActor,NULL,NULL);
        e.EActor->Timeout += 12;
        return DONE;
      }
    }
    else if (e.isLoc) {
      e.EXVal = e.EXVal - 2 + random(4);
      e.EYVal = e.EYVal - 2 + random(4);
    }
    else  
      e.EDir = FIRST_DIR + random(LAST_DIR - FIRST_DIR);
  }

  if (e.isDir) {
    tx=x+DirX[e.EDir];
    ty=y+DirY[e.EDir];
  }
  else if (e.isLoc) {
    tx=e.EXVal;
    ty=e.EYVal;
  }
  else
    Error("No destination in Creature::Walk.");

  //Even ghosts can't cross the outer walls of the Dungeon.
  if(tx <= 0 || ty <= 0 || tx >= m->SizeX()-1 || ty >= m->SizeY()-1)
    /* Later, include stuff for player walking off the edge of an
       open-edged map. */
    return ABORT;

  bool isIllTer = m->TerrainAt(tx,ty) != m->PTerrainAt(tx,ty,this);

  Thing * blocked_by = NULL;

  if (!canMoveThrough(&e,tx,ty,blocked_by)) {
    if (blocked_by) {
      if (blocked_by->isCreature()) {
        cr = (Creature *)blocked_by; 
        if (!Percieves(cr)) {
          if (cr->HasStati(HIDING)) {
            /* Presumably, you turn in the direction you want to move,
               and then you are directly facing the concealed creature,
               thereby exposing it in such a manner that you can't miss
               it. Ergo, no 'bump', it's jusy revealed as if it had
               taken a surprise attack. */
            cr->Reveal();
            // ww: there was a little logic problem with hiding and
            // invisible creatures ...
          }
          String msg; 
          if (!Percieves(cr)) {
            msg = "You bump into something you cannot see! Attack?";
            Timeout += 10;
          } else {
            msg = Format("You find %s! Attack?", (const char*)cr->Name(NA_A));
            Timeout += 3;
          }
          bool the_default = isHostileTo(cr); 
          m->Update(cr->x,cr->y);
          if (e.Event == EV_MOVE && yn(msg,the_default)) {
            if (AttackMode() == S_BRAWL)
              return Throw(EV_NATTACK,this,cr);
            else if (AttackMode() == S_MELEE || AttackMode() == S_DUAL)
              return Throw(EV_WATTACK,this,cr,InSlot(SL_WEAPON));
            else if (AttackMode() == S_ARCHERY || AttackMode() == S_THROWN)
              return Throw(EV_RATTACK,this,cr,InSlot(SL_WEAPON));
          } else return ABORT; 
          return DONE;
        }
        if (e.Event == EV_MOVE && 
            (!cr->isPlayer() || isLedBy(cr))) {
          if (cr->onPlane() != onPlane()) 
          { dis = cr; goto IgnoreCreature; }
          else if (cr->isFriendlyTo(this))
          { dis = cr; goto IgnoreCreature; }
          else if (ChallengeRating() >= min(6,cr->ChallengeRating()+5))
            if (isMonster() && cr->isMonster())
              if (GetAttr(A_STR) >= cr->GetAttr(A_STR)+4)
                if (cr->GetAttr(A_MOV))
                { dis = cr; goto IgnoreCreature; }
        }
      } else if (blocked_by->Type == T_DOOR) {
        Door * d = (Door *)blocked_by;
        if (d->DoorFlags & DF_SECRET) 
          return ABORT; 
          switch (isPlayer() ? thisp->Opt(OPT_AUTOOPEN) : 2)
            {
              case 0: break;
              case 1: if (!yn(Format("Open %s?",(const char*)blocked_by->Name(NA_THE)),true))
                        break;
              case 2: ;
          return Throw(EV_OPEN,this,blocked_by);
        }
      } 
      IPrint("You cannot move through the <Obj>.",blocked_by);
    } 
    if (isPlayer() && !(m->At(tx,ty).Memory)) {
      Timeout += 6;
      IPrint("You bump against something.");
      if (!m->At(tx,ty).Memory)
        m->At(tx,ty).Memory = GLYPH_ROCK | (SHADOW*256);
      thisp->UpdateMap = true;
    } 
    return ABORT;
  } 
  
  for (cr=m->FCreatureAt(tx,ty);cr;cr=m->NCreatureAt(tx,ty))
    if (cr->isIllusion() && !cr->isRealTo(this))
      dis = cr;
      
IgnoreCreature:

  /* The monster AI inherently won't move monsters closer to things 
     that they are currently afraid of, so don't waste time checking
     this here. */
  if ((e.Event == EV_MOVE) && isPlayer() && !HasFeat(FT_LION_HEART)) {
      StatiIterNature(this,AFRAID)
        t = oThing(S->h);
        if (!t)
          continue;
        if (t->isCreature())
          if (dist(x,y,t->x,t->y) >= 5 + ((Creature*)t)->Mod(A_CHA))
            continue;
        if (dist(x,y,t->x,t->y) > dist(tx,ty,t->x,t->y))
        {
          IPrint("You're too scared to approach the <Obj>.",t);
          StatiIterBreakout(this,return ABORT)
        }
      StatiIterEnd(this)
    }

  if (e.Event != EV_MOVE || e.EActor->HasStati(CONFUSED))
    goto SkipConfirms;

  if (!isAerial() && !isIllusion()) 
    for (tr=m->FTrapAt(tx,ty);tr;tr=m->NTrapAt(tx,ty))
      {  
        if (!tr->HasStati(RESET_BY))
          if (HasEffStati(HOME_REGION,m->RegionAt(tx,ty)))
            continue;
        if ((tr->TrapFlags & TS_FOUND) && isPlayer()) {
          if (isPlayer() && thisp->SkillRanks[SK_HANDLE_DEV] &&
              (thisp->Opt(OPT_AUTODISARM)) && !tr->HasStati(RESET_BY,-1,this)) {
            if (thisp->DisarmTrap(tr,false))
              return DONE; 
            } 
          if (!(tr->TrapFlags & TS_DISARMED) && !HasFeat(FT_FEATHERFOOT) &&
               !(tr->GetStatiObj(RESET_BY) == this && !isThreatened())) {
            if (!yn(XPrint("Confirm move over the <Obj>?",tr),false))
              return ABORT;
            }
          }
        else if (!(tr->TrapFlags & TS_SEARCHED)) {
          if (SkillCheck(SK_SEARCH,20 + TEFF(tr->tID)->Level
                + (-5 * (TEFF(tr->tID)->HasFlag(EF_MUNDANE)))
                ,true,tr->GetStatiMag(RETRY_BONUS,SK_SEARCH,this),"retry")) {
            if (oPlayer(m->pl[0])->Percieves(this))
              tr->TrapFlags |= TS_FOUND;
            IDPrint("You find an <Obj2>!",
                "The <Obj> finds an <Obj2>!",this,tr);
            tr->SetImage();
            Timeout += 30;
            return DONE;
            }
          else {
            tr->TrapFlags |= TS_SEARCHED;
            tr->BoostRetry(SK_SEARCH,this);
            }
          }
        
      }
  {
    rID stickyID = m->StickyAt(tx,ty); 
    if (stickyID && TTER(stickyID)->Material == MAT_LIQUID)
      if (HasAbility(CA_PASS_SLIME))
        goto SkipStickyConfirm;
    if (stickyID && 
        !HasStati(STUCK) && 
        !HasMFlag(M_AMORPH) && 
        !isAerial() && 
        onPlane() <= PHASE_VORTEX && 
        ResistLevel(AD_STUK) != -1 && 
        !HasAbility(CA_WOODLAND_STRIDE)) {
      if (SkillLevel(SK_BALANCE) < 14 + m->Depth && 
          !yn(XPrint("Confirm move over the <Res>?",stickyID)))
        return ABORT;
      }
    SkipStickyConfirm:
    ;
  }

  if (isPlayer() && m->BrightAt(tx,ty))
    if (HasStati(HIDING,HI_SHADOWS))
      if (!yn("Confirm enter the brightly-lit area?"))
        return ABORT;
  
  if (isPlayer() && TTER(m->TerrainAt(tx,ty))->HasFlag(TF_WATER))
    if (HasStati(HIDING,HI_SHADOWS) && !HasFeat(FT_FEATHERFOOT) && !isAerial())
        if (!yn("Confirm enter the revealing terrain?"))
          return ABORT;
  

  {
    Thing *t = oThing(m->At(tx,ty).Contents);
    while (t) {
      if (t->isFeature()) {
        Feature * fe = (Feature *)t; 
        if ((fe->Flags & (F_WARN)) &&
            (TFEAT(fe->fID)->PEvent(EV_MON_CONSIDER,this,fe,fe->fID) == ABORT)){
          if (isPlayer()) {
            if (!yn(XPrint("Confirm move over the <Res>?",fe->fID,true)))
              return ABORT;
          } else {
            // monster ai: don't go there
            return ABORT;
          } 
        }
      } 
      t = oThing(t->Next);
    }
  }

  if (m->At(x,y).Terrain != m->At(tx,ty).Terrain || (m->FieldAt(tx,ty,FI_ITERRAIN) &&
        (m->PTerrainAt(x,y,this) != m->PTerrainAt(tx,ty,this)))) {
    if (TTER(m->PTerrainAt(tx,ty,this))->HasFlag(TF_WARN)) {
      if (isPlayer() && e.Event == EV_MOVE && 
          (TTER(m->PTerrainAt(tx,ty,this))->PEvent(EV_MON_CONSIDER,
                                             this,m->TerrainAt(tx,ty)) == ABORT)) { 
        if (!yn(XPrint("Confirm enter the <Res>?",m->TerrainAt(tx,ty)),true))
          return ABORT;
      } else if (!isPlayer()) {
        // monster AI says: don't go there!
        if (TTER(m->PTerrainAt(tx,ty,this))->PEvent(EV_MON_CONSIDER,
              this,((Monster *)this)->mID) == ABORT)
          return ABORT;
      } 
    } 

    if (!HasMFlag(M_AMPHIB)) {
      if (HasMFlag(M_AQUATIC)) {
        if (!TTER(m->TerrainAt(x,y))->HasFlag(TF_WATER))
        {
          
          IPrint(isIllTer ? "You can't seem to move that direction!" : "You can't move on land!");
          return ABORT;
        }
        else if (!TTER(m->TerrainAt(tx,ty))->HasFlag(TF_WATER))
        {
          IPrint(isIllTer ? "You can't seem to move that direction!" : "You can't leave the water!");
          return ABORT;
        }  
      }
      else {
        /* Water is a barrier to monsters who do not have the
           Swim skill, but not to a player character. Water
           will eventually have at least some effect on a PC,
           but not an absolute barrier. */
        if (TTER(m->PTerrainAt(tx,ty,this))->HasFlag(TF_WATER))
          if (isMonster()) {
            if (TTER(m->PTerrainAt(tx,ty,this))->PEvent(EV_MON_CONSIDER,
                  this,((Monster *)this)->mID) == ABORT)
              return ABORT;
          }
          else
            RemoveStati(HIDING,-1,HI_WATER);
      }
    }
  }

  if (HasStati(ELEVATED)) 
    {
      if (e.Event == EV_PUSH) { 
        ClimbFall(); 
        if (isDead()) 
          return DONE; 
        goto NoLongerElevated;
        }
        
      if (HasStati(ELEVATED,ELEV_CEILING)) {
        if (m->At(tx,ty).isSkylight)
          { IPrint("There's no more ceiling there!");
            return ABORT; }
      
        if (!random(30)) {
          if (SkillCheck(SK_CLIMB,25,true)) {
            if (LastSkillCheckResult < 35) { 
              IPrint("You maintain your perch, with effort.");
              LoseFatigue(1,false);
              }
            else
              IPrint("You maintain your perch effortlessly.");
            }
          else {
            ClimbFall();
            if (isDead()) 
              return DONE; 
            goto NoLongerElevated;
            }
          }
        }

      if (HasStati(ELEVATED,ELEV_TREE) && !m->TreeAt(tx,ty))
        {
          rID res = 0;
          if (m->FFeatureAt(x,y))
            res = m->FFeatureAt(x,y)->fID;
          if (!res) 
            res = m->TerrainAt(x,y);
          DPrint(e,"You climb down from the <res>.", 
            "The <EActor> climbs down from the <res>.", res);
          RemoveStati(ELEVATED);
          if (!HasFeat(FT_BRACHIATION)) 
            Timeout += 15;
          return DONE;
        }
      
      if (!SkillCheck(SK_CLIMB,e.Event == EV_JUMP ? 25 : 13,false))
        {
          ClimbFall(); 
          Timeout += 10;
          return DONE;
        }
        
      NoLongerElevated:
      ;
    }

  if (isMonster()) {
    if (m->FieldAt(tx,ty)) {
      // Badfields() is expensive!
      if (m->FieldAt(tx,ty,thism->BadFields()))
        return ABORT;
    } 
  }
  
  /* Illusions don't walk out of their creator's LOS under
     their own volition. */
  if (isIllusion())
    if (IllusionLOSCheck(true,tx,ty))
      return ABORT;
  
  if (isPlayer()) {
    Thing *il; int16 i;
    for(i=0;backRefs[i];i++)
      if (theRegistry->Exists(backRefs[i]))
        if ((il = oThing(backRefs[i]))->isIllusion())
          if (il->GetStatiObj(ILLUSION) == this)
            if (il->IllusionLOSCheck(true,il->x,il->y,tx,ty))
              if (!yn(XPrint("Confirm break LOS with your <Obj>?",il)))
                return ABORT;
    }

SkipConfirms:

  if (m->FieldAt(tx,ty,FI_BLOCKER)) 
    for (i=0;m->Fields[i];i++)
      if (m->Fields[i]->FType & FI_BLOCKER) {
        if (m->Fields[i]->inArea(tx,ty) && !m->Fields[i]->inArea(x,y))
        {
          if (m->Fields[i]->eID) {
            e.eID = m->Fields[i]->eID;
            e.EMagic = TEFF(m->Fields[i]->eID)->Vals(0);
            if (!isTarget(e,this))
            {
              e.eID = 0; e.EMagic = NULL;
              continue;
            }
            e.eID = 0;
            e.EMagic = NULL;
          }
          DPrint(e,"You press against an unseen barrier (<Res>)!",
              "The <EActor> seems unable to advance (<Res>).",
              m->Fields[i]->eID);
          Timeout += 6;
          return ABORT;
        }
      }

  if ((m->At(x,y).Terrain != m->At(tx,ty).Terrain) ||
      (isIllTer && (m->PTerrainAt(tx,ty,this) != m->PTerrainAt(x,y,this))))
  {
    i = e.Event; e.isLoc = true;
    e.Event = EV_LEAVE;
    e.EXVal = x; e.EYVal = y;
    if (TTER(m->TerrainAt(x,y))->Event(e,m->TerrainAt(x,y)) == ABORT)
      return ABORT;
    if ((Flags & F_DELETE) || !m)
      return DONE;
    if (m->TerrainAt(x,y) != m->PTerrainAt(x,y,this))
      if (TTER(m->PTerrainAt(x,y,this))->Event(e,m->PTerrainAt(x,y,this)) == ABORT)
        return ABORT;
    if ((Flags & F_DELETE) || !m)
      return DONE;
    e.Event = EV_ENTER;
    e.EXVal = tx; e.EYVal = ty;
    if (TTER(m->TerrainAt(tx,ty))->Event(e,m->TerrainAt(tx,ty)) == ABORT)
      return ABORT;
    if ((Flags & F_DELETE) || !m)
      return DONE;
    if (isIllTer && TTER(m->PTerrainAt(tx,ty,this))->Event(e,m->PTerrainAt(tx,ty,this)) == ABORT)
        return ABORT;
    e.Event = i;
    if ((Flags & F_DELETE) || !m)
      return DONE;
  }

  if (m->At(x,y).Region != m->At(tx,ty).Region)
  {
    i = e.Event; e.isLoc = true;
    
      
    e.Event = EV_LEAVE;
    e.EXVal = x; e.EYVal = y;
    if (TREG(m->RegionAt(x,y))->Event(e,m->RegionAt(x,y)) == ABORT)
      return ABORT;
    if ((Flags & F_DELETE) || !m)
      return DONE;
      
    if (onPlane() == PHASE_MATERIAL && !HasAbility(CA_EARTHMELD)) {
      if (TREG(m->RegionAt(tx,ty))->Size < GetAttr(A_SIZ))
        {
          IPrint("You are too large to enter the <Res>.",m->RegionAt(tx,ty));
          return ABORT;
        }
      else if (TREG(m->RegionAt(tx,ty))->Size == GetAttr(A_SIZ) &&
              TREG(m->RegionAt(x,y))->Size > GetAttr(A_SIZ))
        szChange = -1;
      else if (TREG(m->RegionAt(x,y))->Size == GetAttr(A_SIZ) &&
              TREG(m->RegionAt(tx,ty))->Size > GetAttr(A_SIZ))
        szChange = 1;
      if (szChange == -1 && HasStati(MOUNTED))
        {
          IPrint("That passage is too tiny to enter while mounted.");
          return ABORT;
        }
      }
    
    
    /* isCharacter() is just an optimization -- non-player
       creatures don't get XP (yet). */
    if (isCharacter() && AbilityLevel(CA_THIEF_IN_THE_NIGHT))
      ThiefXP(m->RegionAt(x,y));
    
      
    e.Event = EV_ENTER;
    e.EXVal = tx; e.EYVal = ty;
    if (TREG(m->RegionAt(tx,ty))->Event(e,m->RegionAt(tx,ty)) == ABORT)
      return ABORT;
    e.Event = i;
    if ((Flags & F_DELETE) || !m)
      return DONE;
  }

  for(fe=m->FFeatureAt(x,y);fe;fe=m->NFeatureAt(x,y))
    if (fe && TFEAT(fe->fID)->PEvent(EV_LEAVE,this,fe,fe->fID) == ABORT) 
      return ABORT; 
   
  if ((Flags & F_DELETE) || !m)
    return DONE;
    
  for(fe=m->FFeatureAt(tx,ty);fe;fe=m->NFeatureAt(tx,ty))
    if (fe && TFEAT(fe->fID)->PEvent(EV_WALKON,this,fe,fe->fID) == ABORT) 
      return ABORT; 
   
  if ((Flags & F_DELETE) || !m)
    return DONE;


  /* Here, we need to check if we are attempting to close with any
     creatures wielding a reach weapon, or that naturally have reach.
     In that case, we need to make a closing check in order to finish
     the movement. There's no mechanic like this in the base OGL
     system, but it does make sense that, say, a polearm can be used
     to keep someone out of melee range with you. */
  cwc = 0; CloseConfirm = false;
  for(i=0;i!=8;i++) {
    if (isDead() || !m) // ww: we can die while trying to close 
      return DONE;
    for (cr=m->FCreatureAt(tx+DirX[i],ty+DirY[i]);cr;cr=m->NCreatureAt(tx+DirX[i],ty+DirY[i]))
      if ((cr->StateFlags & MS_HAS_REACH) && !cr->HasStati(CHARGING)
            && cr->InSlot(SL_WEAPON) && cr->InSlot(SL_WEAPON)->HasIFlag(WT_REACH))
        if (cr != this && cr->canMakeAoO(this) && 
            ( cr->isHostileTo(this) || this->isHostileTo(cr)) &&
            cr->onPlane() == this->onPlane())
          if (cr->Percieves(this) & ~(PER_SHADOW))
          {
            if (cr->isIllusion())
              if (!cr->isRealTo(this))
                continue;
            ClosingWith[cwc++] = cr;
            e.EVictim = cr;
            if (Percieves(cr) && (!CloseConfirm) && e.Event == EV_MOVE)
              if (!yn(XPrint("Confirm close with the <Obj>?",cr),true))
                return ABORT;
            CloseConfirm = true;

            /* Weimer: Special rule for charging. Historically, the ability
             * to set a weapon to receive a charge was very important --
             * horsemen charging into a line of pikes got killed fast. 
             */
            if (HasStati(CHARGING)) {
              int before_hp = cr->cHP; 
              cr->AoO++; 
              TPrint(e, "The <EVictim> prepares for your charge.",
                    "You prepare for the <EActor>'s charge.",
                    "The <EVictim> prepares for the <EActor>'s charge.");
              /*
              cr->IPrint(Format("Bonus is %d %d %d.",
                    GetStatiVal(CHARGING),
                    GetStatiMag(CHARGING),
                    ChargeBonus()));
                    */
              ProvokeAoO(cr,true);
              if (isDead()) return ABORT; 
              if (cr->cHP < before_hp) { 
                ThrowDmg(EV_DAMAGE,AD_TRIP,30,"stopped charge");
              } 
              if (!HasStati(CHARGING)) 
                return ABORT; 
            } else {  
              /* This is *supposed* to be based on Base Attack Bonus,
                 not challenge rating. Unfortunately, we have no way
                 to easily access the creature's BAB or trivially
                 calculate it, so we substitute CR. Later, we'll write
                 an overloaded GetBAB(S_MELEE) function that will calc
                 the correct BAB for both monsters and characters. It
                 is also quite intentional that it's easier to close
                 with natural reach creatures then polearm-wielders --
                 the monster with reach can keep attacking you once
                 you've closed; the polearm wielder has to either
                 disengage or drop his weapon. */
              CloseDC = cr->WeaponSaveDC(cr->InSlot(SL_WEAPON),WT_REACH);
              SizeMod = (cr->GetAttr(A_SIZ) - GetAttr(A_SIZ))*4;
              if (CloseDC > 0 && !SavingThrow(REF,CloseDC,SA_CLOSE,SizeMod,"size")) {
                e.EVictim = cr;
                if (Percieves(cr))
                  TPrint(e, (const char*)Format(
                        "You fail to close with the <EVictim> (DC %d).",
                        CloseDC),
                      "The <EActor> fails to close with you.",
                      "The <EActor> is held at bay by the <EVictim>.");
                else
                  IPrint("You are held at bay!");
                if (cr->HasFeat(FT_GUARDED_STANCE) && !cr->isFlatFooted() &&
                    cr->InSlot(SL_WEAPON) &&
                    cr->InSlot(SL_WEAPON)->HasIFlag(WT_REACH)) {
                  ProvokeAoO(cr,true);
                  if (isDead()) return ABORT; 
                }
                Timeout += 3000 / (100 + Mod(A_DEX));
                if (isMonster())
                  GainTempStati(AVOID_MELEE,cr,Dice::Roll(3,6),SS_MISC);
                return ABORT;
              }
            }
            TPrint(e,"You close with the <EVictim>.",
                "The <EActor> slips past your defenses!",
                "The <EActor> slips past the <EVictim>'s defenses.");
          }
  }

  // SRD, Combat I -- running provoke an AoO
  if (GetStatiMag(CHARGING)) {
    ProvokeAoO(NULL,true);
    if (isDead()) return ABORT; 
  } 


  /* Threat ranges are square, not circular -- it's very important
     that the player understand when they are entering or leaving
     a monster's threatened area, and vice versa. Hence, we use this
     macro instead of dist(). */
#define sdist(vx,vy,t) max(MYABS(vx - t->x),MYABS(vy - t->y))

  bool failed = false;
  int8 myHit, crHit; ch = 0;
RestartThreatCalc:
  for(i=0;i!=8;i++)
    for (cr=m->FCreatureAt(x+DirX[i],y+DirY[i]);cr && !isDead();
               cr=m->NCreatureAt(x+DirX[i],y+DirY[i]))
      if (sdist(tx,ty,cr) > 1) 
        if (cr->isHostileTo(this) && !cr->HasStati(HIDING) && cr->Percieves(this) &&
            !cr->HasStati(PRONE) && !cr->HasStati(SLEEPING) && !cr->HasStati(STUNNED) &&
            !cr->HasStati(GRAPPLING) && !cr->HasStati(GRAPPLED) && !cr->HasStati(GRABBED) &&
            !cr->HasStati(PARALYSIS) && !cr->isBlind() && !cr->HasStati(AFRAID)) {
          /* We now know we're leaving somebody's threat range. Do
             we disengage or flee? */
          if (!ch) {
            if (cr->isIllusion() && !cr->isShade())
              if (!cr->isRealTo(this))
                continue;
            if (isPlayer() && !HasStati(CHARGING)) {
RepeatPrompt:
              if (e.Event != EV_MOVE || !e.EActor->Percieves(cr))
                ch = 'f';
              else
                ch = thisp->MyTerm->ChoicePrompt("You are in a threatened area. Abort, Flee or Disengage?", "afd?");
              if (ch == '?') {
                thisp->MyTerm->HelpTopic("help::combat","LE");
                goto RepeatPrompt;
                }
              if (ch == 'f' && e.Event == EV_MOVE)
                IPrint("You turn and run.");
            }
            else
              ch = 'f';

          }
          if (ch == 'a' || ch == -1)
            return ABORT;


          if (ch == 'f') {
            /* Avoid Bull Rush giving the rusher an attack of opportunity
               upon her target as she's forced back. */
            if (e.Event != EV_PUSH || e.EVictim != cr) {
              ProvokeAoO(cr,true);
              if (isDead()) return ABORT; 
            }
          }
          else {
            myHit = max(Attr[A_HIT_BRAWL],Attr[A_HIT_MELEE]);
            if (HasFeat(FT_TACTICAL_WITHDRAWL))
              myHit += 8;
            crHit = max(cr->Attr[A_HIT_BRAWL],cr->Attr[A_HIT_MELEE]);
            if (myHit + (random(20)+1) >= crHit + 11) 
              /* Give the player a hope of actually escaping. */
              cr->Timeout = max(cr->Timeout,15);
            else {
              if (!failed)
                DPrint(e,"You fail to break off melee safely.", 
                    "The <EActor> tries to leave melee, but the <Obj> persists!", cr);
              failed = true;
              ProvokeAoO(cr,true);
              if (isDead()) return ABORT; 
              t = cr;
            }
          } 
        }

  if (ch) {
    /* Don't move or print irrelevant messages if the creature died as
       a result of attacks of opportunity. */
    if (Flags & F_DELETE)
      return DONE;

    /* Some attacks of opportunity -- frex, a ghoul's paralyzing touch --
       can prevent the player from finishing the movement action that
       initially provoked them. */
    if (HasStati(PARALYSIS) || HasStati(GRABBED) || HasStati(GRAPPLED))
      return DONE;

    if (failed) {
      Timeout += 15;
      return DONE;
    }
    else if (ch == 'd')
      DPrint(e,"You leave melee safely.", "The <EActor> breaks off melee.");    
  }

  ox = x; oy = y;

  /* WW: Monsters with Mov 10% (like the Dark Tree) get beaten up massively
   * because after one move they sit motionless for 7 turns and get pecked
   * to death by ducks!
   *
   * Unfortunately, Incursion doesn't have an event system where things
   * happen later, so we'll store little "Timeouts Paid" in a Stati and
   * only give for 1500 time units, max. For monsters only. 
   *
   * A typical lizardman (or whatever) with A_MOV = 1 (== 105%) takes
   * 9 Timeout units. A dark tree with A_MOV = -17 (== 15%) takes 
   * 66 Timeout units. 
   *
   * FJM: This proved to swing the other direction, ending up massively
   * unfair to players because they could never get the "first hit" on
   * monsters, thus being struck flat-footed by a kind of spring attack
   * innate ability all monsters had. To fix this while retaining the
   * fix to the above problem, monsters can not only store up to 
   * (Timeout - 12) segments of movement, and thus slow monsters must
   * be paused for a minimum of 12 segments after movement, thereby
   * allowing the player to hit before they can. However, a dark tree
   * will only timeout for 12-15 segments after movement rather than 66,
   * so it can still respond to attacks.
   */
#define BANK_AMOUNT     5
  if (isMonster()) {
    int32 needed_timeout = MoveTimeout(ox,oy);

    /*
    IDPrint(NULL,(const char*)Format("%s (banked %d, needed %d).",
          (const char*)Name(),StoredMovementTimeout,needed_timeout));
          */
    if (needed_timeout - 10 > StoredMovementTimeout + BANK_AMOUNT) {
      // we can't get there this turn, so bank some this turn and come back
      // later
      StoredMovementTimeout += BANK_AMOUNT; 
      Timeout += BANK_AMOUNT;
      return DONE; 
    } 
  } 

  Move(tx,ty,e.Event == EV_MOVE);
  if (x != tx || y != ty)
    return ABORT;

  if (m->SolidAt(ox,oy)!= m->SolidAt(tx,ty))
    if (HasAbility(CA_EARTHMELD) && onPlane() <= PHASE_VORTEX)
    {
      if (m->SolidAt(ox,oy))
        IDPrint("You step out of the rock.",
            "From out of solid stone, an <Obj>!", this);
      else
        IDPrint("You meld into the rock.",
            "The <Obj> melds into the rock.", this);
    }

  cr = e.EVictim;

  if (dis) {
    e.EVictim = dis;
    if (!(dis->isIllusion() && !dis->isRealTo(this)) &&
        !dis->onPlane() != onPlane() &&
        !dis->isFriendlyTo(this))
      {
        /* You're moving into the same square as dis. */
        if (!Percieves(dis))
          ; /* No message -- don't give away dis's presence */
        else if (!dis->Percieves(this))
          IPrint("You slip past the <Obj> unnoticed.", dis);
        else if (!dis->isHostileTo(this))
          IPrint("You walk past an <Obj>.", dis);
      }
    else
      {
        dis->Reveal(); dis->Awaken(SLEEP_NATURAL); 
        if (ThrowXY(EV_PUSH,ox,oy,dis,e.EActor) == ABORT) { 
          Move(ox,oy); 
          return ABORT;
        }
        if (dis->isIllusion() && !dis->isRealTo(this))
          {
            TPrint(e,"You pass through the <EVictim>.",
                "The <EActor> passes through you.",
                "The <EActor> passes through the <EVictim>.");
            dis->DisbeliefCheck();
          }
        else if (dis->onPlane() != onPlane())
          TPrint(e,"You pass through the <EVictim>.",
              "The <EActor> passes through you.",
              "The <EActor> passes through the <EVictim>.");
        else if (dis->isFriendlyTo(this)) {
          if (dis->GetStatiObj(ANIMAL_COMPANION,TA_LEADER) != this)
            TPrint(e,"You displace the <EVictim>.",
                "The <EActor> displaces you.",
                "The <EActor> displaces the <EVictim>.");
          }
        else {
          TPrint(e,"You push past the <EVictim>.",
              "The <EActor> push past you.",
              "The <EActor> push past the <EVictim>.");
          ProvokeAoO(dis,true);
          if (isDead() || !m)
            return DONE;
          }
      }
  }                    

#if 0
    for(i=0;i!=cwc;i++)
      {
        e.EVictim = ClosingWith[i];
        ProvokeAoO(ClosingWith[i],true);
        if ((Flags & F_DELETE) || !m)
          return DONE;
        if (!(ClosingWith[i]->Flags & F_DELETE) && Percieves(ClosingWith[i]))
          TPrint(e,"You close with the <EVictim>.",
                   "The <EActor> slips past your defenses!",
                   "The <EActor> slips past the <EVictim>'s defenses.");
      }
#endif
  e.EVictim = cr;

  /* Why skill rating + 10 + 1d10 instead of skill + 1d20? Because
     it gives beginning rogues a bit better chance to sneak up on
     or past things, and more importantly gives a rogue a little
     bit of surety in their abilities before reaching a very, very
     high level. */
  MoveSilRoll = SkillLevel(SK_MOVE_SIL) + 10 + Dice::Roll(1,10,0);
  for(i=0;i!=9;i++)
    for (cr=m->FCreatureAt(tx+DirX[i],ty+DirY[i]);cr;cr=m->NCreatureAt(tx+DirX[i],ty+DirY[i]))
      if (cr->isHostileTo(this) && !cr->HasMFlag(M_DEAF)) 
        if (onPlane() == cr->onPlane())
          if (cr->SkillLevel(SK_LISTEN) + Dice::Roll(1,20,0) > MoveSilRoll)
            {
              if (HasStati(HIDING)) {
                if (!Percieves(cr))
                  IPrint("You catch a flash of movement -- something unseen heard you!");
                else
                  IPrint("The <Obj> hears you!", cr);
                cr->Awaken();
                Reveal(false);
                }
              else if (cr->HasStati(ASLEEP)) {
                if (cr->GetStatiVal(ASLEEP) == SLEEP_PSYCH ||
                    cr->GetStatiVal(ASLEEP) == SLEEP_DEEP)
                  continue;
                IDPrint("The <Obj> hears you!",
                        "The <Obj> hears the <Obj>!",
                        cr, this);
                cr->Awaken();
                }
              /*
                if (cr->FFCount > 5)
                cr->IDPrint(NULL,"The <Obj> stands to alertness!");
                cr->FFCount = 0;
                if (cr->isPlayer())
                  ((Player*)cr)->statiChanged = true;
              */
            }

  if (HasStati(CHARGING)) 
    GetStati(CHARGING)->Mag++;

  /* Yes, this should be here, not in TerrainEffects(). If you stay 
     standing on the square of a dart trap, it won't keep shooting
     you again and again. This is how traps generally work in roguelike
     games. We're using a Dexterity check (DC 20) to avoid triggering
     a known trap when stepping over it. */
  /* WW: let's modify this so that the dex check is folded into the
   * generic trap avoidance check under TriggerTrap */
  if (!isAerial() && !isIllusion())
    for(tr=m->FTrapAt(tx,ty);tr;tr=m->NTrapAt(tx,ty)) {
      if (!tr->HasStati(RESET_BY))
        if (HasEffStati(HOME_REGION,m->RegionAt(x,y)))
          continue;
      if (tr->HasStati(RESET_BY,-1,this) && !isThreatened())
        {
          IPrint("You step carefully over your <Obj>.", tr);
          Timeout += 15;
        }
      else if (!(tr->TrapFlags & TS_DISARMED))
        {
          int foundBefore = tr->TrapFlags & TS_FOUND;
          tr->TriggerTrap(e,foundBefore);
          if (Flags & F_DELETE)
            return DONE;
        }      
      }

  if (Type == T_PLAYER)
    if (m->MItemAt(tx,ty) && !HasStati(ACTING)) { 
      Buff = XPrint("<14>You see here:<7>");
      for(t=m->FirstAt(tx,ty);t;t=m->NextAt(tx,ty))
        if (t != this) {
          if (t->isItem()) thisp->LegendIdent(oItem(t->myHandle));
          Buff += Format("\n  %s  ",(const char*)t->Name(NA_LONG|NA_A|NA_INSC));
        } 
      thisp->MyTerm->Box(WIN_SCREEN,BOX_SAVECHAR|BOX_WIDEBOX,EMERALD,GREY,Buff);
    }



  switch (e.Event) {
    case EV_MOVE:
      {
        int32 needed_timeout = MoveTimeout(ox,oy);
        needed_timeout = min (needed_timeout,
          max(12, needed_timeout - StoredMovementTimeout)); 
        Timeout += needed_timeout; 
        StoredMovementTimeout = 0; 
      }
      break;
    case EV_JUMP:
      Timeout += 20;
      break;
  }

  if ((e.Event == EV_JUMP) && HasFeat(FT_MANTIS_LEAP) && (jcheck == true))
  {
    i = dist(ox,oy,x,y);
    if (HasStati(CHARGING)) {
      SetStatiVal(CHARGING,NULL,CENTER);
      SetStatiMag(CHARGING,-1,NULL,GetStatiMag(CHARGING)+i);
    }
    else
      GainPermStati(CHARGING,NULL,SS_MISC,CENTER,i,0);  
  }

  if (isPlayer())
    if (m->At(ox,oy).Terrain != m->At(x,y).Terrain)
      if (TTER(m->PTerrainAt(ox,oy,this))->MoveMod != TTER(m->PTerrainAt(x,y,this))->MoveMod)
        thisp->MyTerm->ShowTraits();


  TerrainEffects();

  if (szChange < 0)
    IDPrint("You crouch down to enter the <Str1>. It's cramped in here!",
            "The <Obj2> crouches down to enter the <Str1>. <His:Obj2> "
            "freedom of movement is limited.", (const char*)
            Lower(NAME(m->RegionAt(x,y))), this);
  else if (szChange > 0)
    IDPrint("You leave the cramped <Str1>, and have more freedom to move now.",
            "The <Obj2> leaves the cramped <Str1>, and has more freedom to "
            "move now.",(const char*) Lower(NAME(m->RegionAt(ox,oy))), this);
  

  {
    Thing *il; int16 i;
    for(i=0;backRefs[i];i++)
      if (theRegistry->Exists(backRefs[i]))
        if ((il = oThing(backRefs[i]))->isIllusion())
          if (il->GetStatiObj(ILLUSION) == this)
            il->IllusionLOSCheck(false,il->x,il->y,tx,ty);

  }


  if (isDead() || !m || !m->InBounds(x,y))
    return DONE;

  if (HasFeat(FT_BRACHIATION) && m->TreeAt(x,y) && !HasStati(MOUNTED) &&
      !HasStati(ELEVATED) && !isIllTer) {
    rID res = 0;
    if (m->FFeatureAt(x,y))
      res = m->FFeatureAt(x,y)->fID;
    if (!res) res = m->TerrainAt(x,y);
    DPrint(e,"You climb up into the <Res>.", 
        "The <EActor> climbs up into the <Res>.", res); 
    GainPermStati(ELEVATED,NULL,SS_MISC,ELEV_TREE,0);
    } 

  if (HasStati(INVIS) || HasStati(INVIS_TO) ||
       onPlane() == PHASE_ETHERIAL)
    RippleCheck(3);

  /* Elves and drow sense secret doors just by walking past. */
  found = think = false;
  if (isPlayer() && e.Event == EV_MOVE)
    if (HasAbility(CA_SHARP_SENSES) || HasAbility(CA_STONEWORK_SENSE) ||
        HasStati(PERCEPTION))
      for (i=0;i!=8;i++)
        if (d = m->FDoorAt(x+DirX[i],y+DirY[i]))
        {
          if (!(d->DoorFlags & DF_SECRET))
            continue;
          if (d->DoorFlags & DF_SEARCHED)
          { continue; }

          /* Mark it as not secret, so we can check if we *would* see
             it if it weren't secret, then put the flag back to normal. */
          d->DoorFlags &= ~DF_SECRET;
          if (!Percieves(d))
            continue;
          d->DoorFlags |= DF_SECRET;

          /* ww: this logic was reversed! ouch! */
          if (!SkillCheck(SK_SEARCH,15,true,
                d->GetStatiMag(RETRY_BONUS,SK_SEARCH,this),"retry"))
          { 
            d->DoorFlags |= DF_SEARCHED;
            // ww: only "active" searching was using the Retry bonus
            // before!
            d->BoostRetry(SK_SEARCH,this);
            think = true;
            continue;
          }
          d->DoorFlags &= ~(DF_SECRET|DF_SEARCHED);
          d->SetImage();
          found = true;
        }
  if (found)
    IPrint("A secret door!");
  if (think)
    IPrint("You think there may be something here, but you can't find it.");

  if (isPlayer() && HasSkill(SK_MINING) && m->dID)
    {
      int16 mx, my, range; rID rockID;
      rockID = TDUN(m->dID)->GetConst(TERRAIN_ROCK);
      range = SkillLevel(SK_MINING)/2 + 
                AbilityLevel(CA_STONEWORK_SENSE)*2;
      /* The first two if's are optimization only -- we do this
         every single step the player takes, so we don't want it
         to have too many TTER calls. */
      for (mx=x-range;mx<=x+range;mx++)
        for (my=y-range;my<=y+range;my++)
          if (m->InBounds(mx, my) && (m->At(mx,my).Glyph & 
                                          0xFF) == GLYPH_ROCK)
            if (m->TerrainAt(mx,my) != rockID)
              if (TTER(m->TerrainAt(mx,my))->HasFlag(TF_DEPOSIT)) {
                if (m->At(mx,my).Memory == 0)
                  IPrint("Studying the geological formations here, you "
                    "hypothesize the location of a <Res>!", m->TerrainAt(mx,my));
                m->MarkAsSeen(0, mx, my, 0);
                }
    }
    
  if (isPlayer())
    {
      Creature *cr; int16 cx, cy,sz;
      sz = FaceRadius[ GetAttr(A_SIZ) ];
      for (cx=x-sz;cx<=x+sz;cx++)
        for (cy=y-sz;cy<=y+sz;cy++)
          if (m->InBounds(cx,cy) && (sz==1 ||
                 dist(x,y,cx,cy) <= sz))
            for (cr=m->FCreatureAt(cx,cy);cr;cr=m->NCreatureAt(cx,cy))
              if (cr!=this && !(cr->StateFlags & MS_SEEN_CLOSE))
                if (Percieves(cr) & (~PER_SHADOW))
                  {
                    cr->StateFlags |= MS_SEEN_CLOSE;
                    if (TMON(cr->tmID)->HasFlag(M_CLOSEID))
                      cr->IdentifyMon();
                    StatiIterNature(cr,TEMPLATE)
                      if (S->Mag == 0 && TTEM(S->eID)->AddsFlag(M_CLOSEID))
                        cr->IdentifyTemp(S->eID);
                    StatiIterEnd(cr)
                  }
    
    }
  else if (DistFrom(theGame->GetPlayer(0)) <= 1 + FaceRadius[ GetAttr(A_SIZ) ] +
             FaceRadius[ theGame->GetPlayer(0)->GetAttr(A_SIZ) ])
    {
      if (!(StateFlags & MS_SEEN_CLOSE))
        if (theGame->GetPlayer(0)->Percieves(this) & (~PER_SHADOW))
          {
            StateFlags |= MS_SEEN_CLOSE;
            if (TMON(tmID)->HasFlag(M_CLOSEID))
              IdentifyMon();
            StatiIterNature(this,TEMPLATE)
              if (S->Mag == 0 && TTEM(S->eID)->AddsFlag(M_CLOSEID))
                IdentifyTemp(S->eID);
            StatiIterEnd(this)
          }
    }
          
  if (isPlayer())
    if (m->RegionAt(ox,oy) != m->RegionAt(x,y) ||
        m->TerrainAt(ox,oy) != m->TerrainAt(x,y))
      thisp->MyTerm->ShowTraits();
    

  return DONE;
}

int16 FallDepth = 0; 
Creature *Falling = NULL;
rID FallTerrain = 0;

void Creature::TerrainEffects()
  {
    Feature *ft;
    if (!m || x == -1)
      return;
    if (isIllusion())
      return;
    bool isIllTer = m->PTerrainAt(x,y, this) !=
                      m->TerrainAt(x,y);
    
    /* Bridge features allow creatures to walk over terrain without
       suffering the terrain effects. */
    for (ft=m->FFeatureAt(x,y);ft;ft=m->NFeatureAt(x,y))
      if (ft->Flags & F_BRIDGE)
        {
          switch (TFEAT(ft->fID)->PEvent(EV_BRIDGE,this,ft->fID))
            {
              case DONE:
                return;
               break;
            }        
        }
    
    TTerrain *tt = TTER(m->PTerrainAt(x,y,this));
    TTerrain *tt2;
    if (tt->HandlesEvent(EV_SPECIAL)) { 
      EventInfo e;
      e.Clear();
      e.Event = EV_SPECIAL;
      e.isLoc = true;
      e.EXVal = x;
      e.EYVal = y;
      e.EMap = m;
      e.EActor = e.EVictim = this;
      if (tt->Event(e,m->PTerrainAt(x,y,this)) != NOTHING)
        return;
      }
    if (isDead() || x == -1)
      return;    
    if (isIllTer)
      {
        tt2 = TTER(m->PTerrainAt(x,y,this));
        tt = TTER(m->TerrainAt(x,y));
        if (tt2->HandlesEvent(EV_SPECIAL)) {
          EventInfo e; EvReturn r;
          e.Clear();
          e.Event = EV_SPECIAL;
          e.isLoc = true;
          e.EXVal = x;
          e.EYVal = y;
          e.EMap = m;
          e.EActor = e.EVictim = this;
          SetSilence();
          r = tt->Event(e,m->TerrainAt(x,y)) != NOTHING;
          UnsetSilence();
          if (isDead()) {
            IDPrint("Too late, you realize there's really <Res1> all around you... You die...",
              "The <Obj2> falls to the ground, dead.", m->TerrainAt(x,y), this);
            m->TerrainDisbelief(x,y,NULL);
            }
          else if (HasStati(STUCK)) {
            IDPrint("You're stuck in something you can't see!",
                    "The <Obj> seems to be stuck, but there's nothing there!", this); 
            m->TerrainDisbelief(x,y,NULL);
            }
          else if (r == ABORT)
            IPrint("You find yourself unable to move forward!");
          if (r != NOTHING)
            return;
          }
      }
    if (isDead() || x == -1)
      return;

    
    if (HasStati(HIDING)) {
      if (m->BrightAt(x,y))
        if (HasStati(HIDING,HI_SHADOWS))
          Reveal(false);
      if (tt->HasFlag(TF_WATER) && !HasFeat(FT_FEATHERFOOT) && !isAerial())
        if (HasStati(HIDING,HI_SHADOWS))
          Reveal(true);
      if (HasStati(HIDING,HI_WATER))
        if (!tt->HasFlag(TF_WATER))
          Reveal(true);
      }
            
    if (tt->HasFlag(TF_FALL))
      if (!isAerial())
        {
          if (isIllTer && !tt2->HasFlag(TF_FALL)) {
            IPrint("You fall though the <Res>...", m->PTerrainAt(x,y,this));
            Thing *t; int16 i;
            MapIterate(m,t,i)
              if (t->isPlayer() && t != this)
                if (((Player*)t)->Percieves(this))
                  t->IPrint(m->TerrainAt(x,y) == m->PTerrainAt(x,y,(Player*)t) ?
                    "An <Obj1> falls into the <Res1>!" :
                    "An <Obj1> falls through the <Res2>!",
                    this, m->TerrainAt(x,y), m->PTerrainAt(x,y,this));
            m->TerrainDisbelief(x,y,NULL);
            }
          else
            IDPrint("You fall...", "An <Obj> falls into the <Res>!",
                       this, m->TerrainAt(x,y));    
          RemoveStati(CHARGING);      
          if (isPlayer()) {
            thisp->MyTerm->SetWin(WIN_INPUT);
            thisp->MyTerm->Color(15);
            thisp->MyTerm->CursorOn();
            thisp->MyTerm->Write(0,0,"Press [ENTER] to continue... ");
            thisp->MyTerm->CursorOn();
            while (thisp->MyTerm->GetCharRaw() != KY_ENTER)
              ;
            thisp->MyTerm->CursorOff();
            thisp->MyTerm->Clear();
            }
          FallDepth += 1;
          Falling = this;
          FallTerrain = m->TerrainAt(x,y);
          if (isPlayer())
            thisp->MyTerm->ShowStatus();
          MoveDepth(m->Depth+1);
          return;
        }
    
    if (FallDepth && Falling == this) {
      ThrowDmg(EV_DAMAGE,AD_FALL,Dice::Roll(3*FallDepth,6),
        XPrint("falling into a <Res>",FallTerrain),this,this);
      FallDepth = 0; 
      Falling = NULL;
      FallTerrain = 0;
      }          
        
    {
      rID stickyID = m->StickyAt(x,y); 
      if (stickyID && TTER(stickyID)->Material == MAT_LIQUID)
        if (HasAbility(CA_PASS_SLIME))
          goto SkipStickTest;
      if (stickyID && 
          !HasStati(STUCK) && 
          !HasMFlag(M_AMORPH) && 
          !isAerial() && 
          onPlane() <= PHASE_VORTEX && 
          ResistLevel(AD_STUK) != -1 && 
          !HasAbility(CA_WOODLAND_STRIDE)) {
        /*
        if (SkillLevel(SK_BALANCE) < 4 + m->Depth && 
            !yn(XPrint("Confirm move over the <Res>?",stickyID)))
          return ABORT;
          */
        if (!SkillCheck(SK_BALANCE,14 + m->Depth,true,false)) {
          DAMAGE(this,this,AD_STUK,-1,NAME(stickyID),
            xe.EParam = TTER(stickyID)->GetConst(STICK_TYPE));
          if (HasStati(STUCK)) {
            IDPrint("You get stuck in the <Res2>!",
                "The <Obj1> gets stuck in the <Res2>!",this,stickyID);
          }
        }
        SkipStickTest:
        ;
      }
    }

    if (isPlayer() && thisp->Opt(OPT_WAIT_FOR_PETS))
      {
        bool hostile, pet; Creature *cr; int16 i;
        hostile = pet = 0;
        MapIterate(m,cr,i) {
          if (cr == this)
            continue;
          if (!cr->isCreature())
            continue;
          if (cr->isFriendlyTo(this))
            pet = true;
          if (cr->isHostileTo(this))
            if (Percieves(cr))
              hostile = true;
          }
        if (pet && !hostile)
          Timeout += thisp->Opt(OPT_WAIT_FOR_PETS)*10;
      }
        

    if (isPlayer() && !isBlind() && (LightRange || InfraRange))
      if (TREG(m->RegionAt(x,y))->Desc) {
        bool show_it = false;
        switch (thisp->Opt(OPT_DESC_ROOM)) {
          case 0: /* NO */ break;
          case 1: /* Yes */ 
                           show_it = true; break;
          case 2: /* New */ 
                           show_it = !REGMEM(m->RegionAt(x,y),thisp)->Seen;
                           break;
        }
        if (show_it)  {
          REGMEM(m->RegionAt(x,y),thisp)->Seen = true;
          String desc;
          desc = XPrint("<13><Str1><7>\n__<Str2>", 
              NAME(m->RegionAt(x,y)),
              DESC(m->RegionAt(x,y)));
          thisp->MyTerm->Box(WIN_SCREEN,0,EMERALD,GREY,desc);
        }
      }


  }

void Creature::ClimbFall()
  {
    if (!HasStati(ELEVATED))
      return;
    int8 typ = GetStatiVal(ELEVATED);
    IDPrint("You fall!","The <Obj> falls!",this);
    RemoveStati(ELEVATED);
    
    ThrowDmg(EV_DAMAGE,AD_FALL,Dice::Roll(2,6),
        XPrint(typ == ELEV_TREE ? "falling out of a <Res>" :
          "falling off the ceiling", m->TerrainAt(x,y)),this,this);
  }

