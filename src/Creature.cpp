/* CREATURE.CPP -- Copyright (c) 1999-2003 Julian Mensch
     General repository for member functions of the Creature class,
   including functions which govern stati (which were initially
   exclusive to Creatures, then extended to apply to all Things),
   fields and so forth.

     Creature::Creature(rID mID, int8 Type)
     void Creature::SetImage()
     void Creature::IdentifyMon()
     void Creature::IdentifyTemp(rID tID)
     void Creature::MakeNoise(int16 radius)
     void Creature::Multiply(int16 val, bool split, bool msg)
     void Creature::DoAttrDeath()
     bool Creature::isFlanking(Creature *c)
     int16 Creature::GetAgeCatagory()
     EvReturn Creature::Event(EventIndo &e)
     void Creature::ExtendedAction()
     void Creature::HaltAction(bool force_halt)
     void Player::RunDir(Dir d)
     bool Player::RunTo(int16 tx, int16 ty)
     void Creature::DoTurn()
     EvReturn Character::Event(EventInfo &e)
     bool Creature::isBoosted(int8 at)
     bool Creature::isDrained(int8 at)
     void Creature::GainInherantBonus(int16 at, int16 mag, bool msg)
     int16 Creature::HungerState()
     int16 Creature::ChallengeRating()
     bool Creature::LoseFatigue(int16 amt,bool avoid)
     int16 Creature::onPlane()
     bool Creature::isFlying()
     bool Creature::isInvisTo()
     bool Creature::isRealTo()
     uint8 Creature::Encumbrance()
     int32 Creature::MaxPress()
     EvReturn Creature::Talk(EventInfo &e)
     rID Creature::Race_mID()
     void Creature::AddTemplate(rID tID)
     TAttack* Creature::__GetNAttk(int8 v)
     TAttack* Creature::__GetNAttk2(int8 v)
     TAttack* Creature::GetAttk(int8 typ)
     bool Creature::HasMFlag(uint8 fl)
     bool Creature::HasFeat(int16 n)
     bool Creature::HasAbility(int16 n)
     int16 Creature::AbilityLevel(int16 n)
      
*/


#include "Incursion.h"

Creature::Creature(rID _mID, int8 _Type):
    Thing(0,_Type)
{
  tmID = mID = _mID;
  ASSERT(TMON(mID));
  Flags = F_SOLID;

  ts.tCount = 0; 

  if (HasMFlag(M_ALL_FEMALE))
    StateFlags |= MS_FEMALE;
  else if (random(100) > 49)
    if (!(HasMFlag(M_NEUTER) || HasMFlag(M_ALL_MALE)))
      StateFlags |= MS_FEMALE;
  cFP = Attr[A_FAT] = 0;
  ManaPulse = 0; 
}
                                    
void Creature::SetImage()         
  {
    int16 i, j;
    Player *pl = m ? oPlayer(m->pl[0]) : theGame->GetPlayer(0);
    Glyph oImg = Image;
    int16 oFlags = Flags;
    Image=TMON(mID)->Image;

    if (isPlayer() && !HasStati(POLYMORPH))
      Image = GLYPH_PLAYER | (Image & 0xFF00);

    if (HasStati(DISGUISED))
      Image = TMON(((Monster*)GetStatiObj(DISGUISED))->mID)->Image;
    
    Flags &= ~F_INVIS;

    if (!(StateFlags & MS_KNOWN)) {
      TAttack *ta = TMON(mID)->GetAttk(A_SEEM);
      if (ta) 
        Image = TMON(ta->u.xID)->Image;
      }

    StatiIterNature(this,TEMPLATE)
        if (S->Mag) {
          TTemplate *tt = TTEM(S->eID);
          if (tt->NewImage & 0x00FF)
            Image = (Image & 0xFF00) | (tt->NewImage & 0x00FF);
          if (tt->NewImage & 0xFF00)
            Image = (Image & 0x00FF) | (tt->NewImage & 0xFF00);
          }
    StatiIterEnd(this)



    if (isPlayer())
      if (thisp->Opt(OPT_PLAYER_COL) != 16)
        Image = (Image & 0xF0FF) | thisp->Opt(OPT_PLAYER_COL)*256;

    if (pl && pl->Opt(OPT_HIGH_FRIEND) && pl != this) 
      if (!isHostileTo(pl))
        Image |= (ts.getLeader() == pl) ?  0xB000 : 0x3000;
      
      
    if (HasStati(INVIS)) {
      /* This would obviously need to be rewritten for multiplayer */
      if (pl == this)
        Image = (Image & 0xF0FF) | 0x0100;
      else {
        Flags |= F_INVIS;
        if (pl && pl->Opt(OPT_HIGH_INVIS))
          if (!(Image & 0xF000))
            Image |= 0x1000;
        }
      }
    
    if (HasStati(HIDING)) {
      if (pl == this)
        Image = (Image & 0x0FFF) | 0x1000;
      else {
        Flags |= F_INVIS;
        if (pl && pl->Opt(OPT_HIGH_HIDE))
          if (!(Image & 0xF000))
            Image |= 0x1000;
        }
      }
    if (pl && pl->Opt(OPT_HIGH_SLEEP) && pl != this) {
      if (HasStati(SLEEPING) || HasStati(PARALYSIS,PARA_HELD))
        if (!(Image & 0xF000))
          Image |= 0x2000;
    }

    if (pl && pl->Opt(OPT_HIGH_MALADY) && pl != this)
      if (!(Image & 0xF000)) {
        bool stuck = false;
        bool prone = false; 
        int badStati[] = { BLIND, STUNNED, CONFUSED, PARALYSIS,
          STUCK, PRONE, NAUSEA, BLEEDING, CHOKING, STONING, 0 } ;
        for (i=0;badStati[i];i++) {
          if (HasStati(badStati[i])) {
            stuck |= (badStati[i] == STUCK);
            prone |= (badStati[i] == PRONE);
            Image |= 0x6000;
          }
        } 
        if (stuck && prone)
          Image = (Image & 0x0FFF) | 0xe000;
      }

    if (pl && (onPlane() != pl->onPlane())) {
      if ((!isMType(MA_VORTEX) && !isMType(MA_SPIRIT) &&
          !isMType(MA_WRAITH)) || pl->onPlane() != PHASE_MATERIAL)
        switch (onPlane())
          {
            case PHASE_ETHERIAL:
              Image = (Image & 0xF0FF) + (BROWN * 256);
            break;
            case PHASE_ASTRAL:
              Image = (Image & 0xF0FF) + (AZURE * 256);
            break;
            case PHASE_NEGATIVE:
              Image = (Image & 0xF0FF) + (BLUE * 256);
            break;
            case PHASE_MATERIAL:
              Image = (Image & 0xF0FF) + (WHITE * 256);
            break;
            case PHASE_SHADOW:
              Image = (Image & 0xF0FF) + (SHADOW * 256);
            break;
          }
      if (pl->Opt(OPT_HIGH_INVIS) && onPlane() == PHASE_ETHERIAL)
        if (!(Image & 0xF000))
          Image |= 0x1000;
      }


    #if 0
    /* Handled in Map::Update() */
    if ((Image & 0x0F00)*16 == (Image & 0xF000)) {
      if (Image & 0xF000)
        Image &= 0xF0FF;
      else
        Image |= 0x0800;
      }
    #endif

    
    if (theGame->InPlay() && x > -1)
      if (Image != oImg || oFlags != Flags)
        m->Update(x,y);
    
  }

void Map::ResetImages()
  {
    Thing *t; int16 i;
    MapIterate(this,t,i)
      if (t->isCreature()) {
        t->SetImage();
        if (T1->getMap() == this &&
            T1->GetMode() == MO_PLAY)
          Update(t->x, t->y);
        }
  }

void Creature::IdentifyMon()
  {
    int16 i; String oldname;
    if (m) {
      for (i=0;i!=MAX_PLAYERS && m->pl[i];i++)
        if (oPlayer(m->pl[i])->Percieves(this))
          goto IsSeen;
      return;
      }
    IsSeen:

    if (!(StateFlags & MS_KNOWN)) {
      IsUnknown:
      oldname = Name(0);
      StateFlags |= MS_KNOWN;
      SetImage();
      if (stricmp(oldname,Name(0)) != 0)
        IDPrint(NULL, "That <str> must be an <Obj>!",
                 (const char*)oldname, this);
      }
  }
  
Item* Creature::ConsumeRope(int16 feet)
  {
    Item *it; int16 c; bool found; 
    found = false; c = 0;
    ASSERT(feet % 5 == 0);
    for (it=FirstInv();it;it=NextInv())
      if (TITEM(it->iID)->HasFlag(IT_ROPE)) {
        found = true;
        if (it->Quantity*5 >= feet)
          {
            Candidates[c++] = it->myHandle;
            if (isPlayer())
              thisp->MyTerm->LOption(it->Name(NA_LONG|NA_MECH),it->myHandle);
          }
        }
    if (c == 0)
      {
        IPrint(found ? "You don't have enough contigious rope." :
          "You don't have any rope.");
        return NULL;
      }
    if (c == 1)
      {
        if (isPlayer())
          thisp->MyTerm->LOptionClear();
        it = oItem(Candidates[0]);
        goto FoundRope;
      }
    if (isPlayer())
      { 
        hObj hRope;
        hRope = thisp->MyTerm->LMenu(MENU_ESC|MENU_BORDER, "Choose a Rope:");
        if (hRope == -1)
          return NULL;
        it = oItem(hRope);
      }
    else
      it = oItem(Candidates[random(c)]);
      
    FoundRope:
    
    if (it->Quantity*5 > feet)
      {
        Item *it2;
        it2 = it->TakeOne();
        it2->SetQuantity(feet/5);
        it->SetQuantity(it->GetQuantity() + 1 - (feet/5));
        it = it2;
      }
    
    it->Remove(false);
    return it;
  }
    

void Creature::IdentifyTemp(rID tID)
  {
    int16 i;
    StatiIterNature(this,TEMPLATE)
      if (S->eID == tID)
        if (S->Mag == 0) {
          if (!m || isDead()) {
            S->Mag = 1;
            SetImage();
          } else {
            for(int16 j=0;j!=MAX_PLAYERS;j++)
              if (Player *p = oPlayer(m->pl[j]))
                if (p != this)
                  if (p->XPercieves(this)) {
                    S->Mag = 1;
              SetImage();
              IDPrint(NULL, "This must be an <Obj>!",this);
            }
          }
        } 
    StatiIterEnd(this)
}

void Map::MakeNoiseXY(int16 x, int16 y, int16 radius)
{
  Creature *cr;
  if (radius <= 0) return; 
  if (FieldAt(x,y,FI_SILENCE)) return; 
  for (int dx = -radius; dx <= radius; dx++) 
    for (int dy = -radius; dy <= radius; dy++) {
      int xx = x + dx;
      int yy = y + dy; 
      if (!InBounds(xx,yy) || dist(x,y,xx,yy) > radius ||
          FieldAt(x,y,FI_SILENCE))
        continue; 
      
      /* Nested FCreatureAt loop issue -- fix later */
      for (cr=FCreatureAt(xx,yy);cr;cr=NULL /* NCreatureAt(xx,yy) */) {
        /* Should this be more then just SLEEP_NATURAL? A Sleep
           spell should be disrupted by noise, perhaps? */
        cr->Awaken(SLEEP_NATURAL);
        /* Reveal location to monsters */
        if (!cr->HasStati(ASLEEP))
          cr->ts.HearNoise(cr,x,y);
      }
    }
  }
    
void Creature::MakeNoise(int16 radius)
  {
    Creature *c; int16 i;
    if (Flags & F_DELETE)
      return;
    c = (Creature *)GetStatiObj(MOUNT);
    if (!c) c = this; 
    if (!c || !c->m || c->x == -1)
      return;
    if (c->inField(FI_SILENCE)) 
      return;
    Reveal(true);
    if (HasStati(INVIS) || HasStati(INVIS_TO))
      RippleCheck(radius);
    c->m->MakeNoiseXY(c->x,c->y,radius); 
  }

void Creature::RippleCheck(int16 range)
  {
    int16 cx, cy; Creature *cr;
    if (HasStati(INVIS) || (onPlane() == PHASE_ETHERIAL))
      for (cx=x-range;cx!=(x+1+range);cx++)
        for (cy=y-range;cy!=(y+1+range);cy++)
          if (m && m->InBounds(cx,cy))
            for(cr=m->FCreatureAt(cx,cy);cr;
                cr=m->NCreatureAt(cx,cy))
              if (cr->ChallengeRating() >= 5 && cr != this &&
                    m->LineOfSight(cr->x,cr->y,x,y,cr))
                {
                  if (cr->HasStati(SEEN_RIPPLE,-1,this))
                    continue;
                  if (cr->HasStati(TRIED,SK_SPOT,this))
                    continue;
                  if (!HasStati(INVIS))
                    if (cr->onPlane() == PHASE_ETHERIAL)
                      continue;
                  if (cr->SkillCheck(SK_SPOT,20,true)) {
                    IPrint("The <Obj> spots your ripple!", cr);
                    cr->IPrint("You spot the shimmering outline of an invisible creature!");
                    cr->GainTempStati(SEEN_RIPPLE,this,-2,SS_MISC);
                    }
                  else
                    cr->GainTempStati(TRIED,this,-2,SS_MISC,SK_SPOT);
                }
    StatiIterNature(this,INVIS_TO)
      for (cx=x-range;cx!=(x+1+range);cx++)
        for (cy=y-range;cy!=(y+1+range);cy++)
          if (m && m->InBounds(cx,cy))
            for(cr=m->FCreatureAt(cx,cy);cr;
                cr=m->NCreatureAt(cx,cy))
              if (cr->ChallengeRating() >= 6 && cr != this &&
                    cr->isMType(S->Val) &&
                    m->LineOfSight(cr->x,cr->y,x,y,cr))
                {
                  if (cr->HasStati(SEEN_RIPPLE,-1,this))
                    continue;
                  if (cr->HasStati(TRIED,SK_SPOT,this))
                    continue;
                  if (cr->SkillCheck(SK_SPOT,20,true)) {
                    IPrint("The <Obj> spots your ripple!", cr);
                    cr->IPrint("You spot the shimmering outline of an invisible creature!");
                    cr->GainTempStati(SEEN_RIPPLE,this,-2,SS_MISC);
                    }
                  else
                    cr->GainTempStati(TRIED,this,-2,SS_MISC,SK_SPOT);
                }
    StatiIterEnd(this)
  }
    

void Creature::Multiply(int16 val, bool split, bool msg)
  { 
    int16 i, c;
    Monster *mn;
    if (!m)
      return;
    if (HasStati(POLYMORPH))
      return;
    if (HasStati(NEGATED))
      return;
    // ww: these things tend to dominate the world otherwise ...
    if (GetStatiMag(GENERATION) > 1)
      return;
    for (c=0;c!=val;c++)
      {
        if (m->BreedCount >= 50) // ww: this is much more reasonable!
          return;
        // BreedCountMax of 10 plus Generation Max of 2 still means
        // that we (gen 0) can have 10 kids, each of which (gen 1) have 10
        // kids, each of which have ~10 sterile kids (gen 2). 
        // ~50 blue jellies from one starting blue jelly is really good
        // enough!
        m->BreedCount++;

        mn = new Monster(tmID);
        
        if (!mn)
          return;
      
        Creature * leader = ts.getLeader(); 
        if (leader) {
          Target * t = ts.GetTarget(leader); 
          ASSERT(t); 
          mn->ts.addCreatureTarget(leader,t->type);
        } else {
          mn->ts.addCreatureTarget(this,TargetMaster);
        } 
        mn->PartyID = PartyID;
        mn->mMana = mMana;
        mn->uMana = mMana - ((mMana - uMana) / 2);

        TMON(mID)->GrantGear(mn,mID,true);
        StatiIterNature(this,TEMPLATE)
            mn->AddTemplate(S->eID);
            RES(S->eID)->GrantGear(mn,S->eID,true);
        StatiIterEnd(this)

        /* ww: PlaceNear doesn't "initialize" things the same way that
         * PlaceAt() does, which was causing that Weirdness in Remove() */ 
        /* ww: ok, The Weirdness is back! */
        mn->PlaceAt(m,x,y);
        mn->m = m; // ww: important! 
        mn->PlaceNear(x,y);
        mn->GainPermStati(GENERATION,NULL,SS_MISC,0,GetStatiMag(GENERATION)+val);
        // ww: we had forgotten this important call ...
        mn->Initialize(true);

        mn->mHP = max(1,mHP / (split ? 2 : 1));
        mn->cHP = max(1,mHP / (split ? 2 : 1));        
      }
    if (!msg)
      ;
    else if (split)
      IDPrint("You split!", "The <Obj> splits!",this);
    else
      IDPrint("You multiply!", "The <Obj> multiplies!",this);
                 
  }

void Creature::DoAttrDeath()
  {
    int16 i;
    const char* CauseOfDeath[] =
      { "withering", "numbing", "unhealth",
        "brain damage", "catatonia", "ego annihilation",
        "the Hand of Fate" };
    for(i=0;i!=7;i++)
      if (AttrDeath & XBIT(i))
        {
          ThrowDmg(EV_DEATH,AD_DAST+i,0,CauseOfDeath[i],this,this);
          return;
        }
  }

bool Creature::isFlanking(Creature *c)
  {
    /* In the OGL system, it's technically possible to flank a
       creature with a ranged weapon. This is just a horrible,
       ugly complication that we're going to leave out for now.
       Canonically, it's only possible to flank if you have an
       ally directly opposite the target; each of the orcs shown
       here are flanking the PC:
            ########
            #......#
            #..o@o.#
            #......#
            ########
       I think it's desirable to allow flanking by just being on
       the opposite side, so that the orcs would get flanking in
       the configuration below. Presently, this is how it works.
       (IOW, a creature can flank for an ally if it is beside the 
       ally's melee target, but not beside the ally.) This partly 
       compensates for the lack of ranged flanking in Incursion.
            ########
            #....o.#
            #..o@..#
            #....o.#
            ########   
       Another possibility is flanking with reach weapons. I'm not
       touching that one for now, though it's certainly possible
       to add in the future. 
       
         Finally, we only check for hostility here, not for being
       an ally -- in other words, in a room with three creatures
       all trying to kill each other in melee, flanking can happen.
       This benefits the player in general, rogue players in par-
       ticular, and rogues with Rings of Conflict most of all. I
       don't see large problems with this at present.*/
       
    Creature *ally; 
    int16 i, fx, fy;

    /* Characters with Uncanny Dodge at 4 or higher (Barbarians at
       level 5+, Rogues at 6+) cannot be flanked by anyone that
       does not have Uncanny Dodge at four or more ranks higher
       then their own score. */
       
    if (m != c->m)
      return false;
    if (x == -1 || c->x == -1)
      return false;
       
    if (c->AbilityLevel(CA_UNCANNY_DODGE) >= 4)
      if (AbilityLevel(CA_UNCANNY_DODGE) < c->AbilityLevel(CA_UNCANNY_DODGE)+4)
        return false;

    for(i=0;i!=8;i++)
      {
        fx = c->x + DirX[i];
        fy = c->y + DirY[i];

        if (MYABS(fx-x) <= 1 && MYABS(fy-y) <= 1)
          continue;

        
        for(ally=m->FCreatureAt(fx,fy);ally;ally=m->NCreatureAt(fx,fy))
          if (ally->isHostileTo(c))
            return true;
      }
    return false;
  }

int16 Creature::GetAgeCatagory()
  {
    int16 i;
    StatiIterNature(this,TEMPLATE)
        if (TTEM(S->eID)->TType == TM_AGECAT)
          StatiIterBreakout(this,return 
            TTEM(S->eID)->Power.Adjust(0))
    StatiIterEnd(this)

    if (isMType(MA_DRAGON)) {
      AddTemplate(FIND("young adult"));
      return 5;
      //Error("Dragon (hObj %d) without age catagory template!",myHandle);
      }
    return 0;
  }

EvReturn Creature::Event(EventInfo &e)
	{
		int16 i; EvReturn res;

    if (e.EVictim == this) {
      e.Event = EVICTIM(e.Event);
      res = TMON(mID)->Event(e,mID);
      if (res == DONE || res == ERROR || res == ABORT)
        return res;
      if (res == NOMSG)
        e.Terse = true;
      StatiIterNature(this,TEMPLATE)
          {
            res = TTEM(S->eID)->Event(e,S->eID);
            if (res == DONE || res == ERROR || res == ABORT)
              StatiIterBreakout(this,return res)
            if (res == NOMSG)
              e.Terse = true;
          }
      StatiIterEnd(this)
      e.Event -= EVICTIM(0);
      }

    if (e.EActor == this) {
      res = TMON(mID)->Event(e,mID);
      if (res == DONE || res == ERROR || res == ABORT)
        return res;
      if (res == NOMSG)
        e.Terse = true;
      StatiIterNature(this,TEMPLATE)
            res = TTEM(S->eID)->Event(e,S->eID);
            if (res == DONE || res == ERROR || res == ABORT)
              StatiIterBreakout(this,return res)
            if (res == NOMSG)
              e.Terse = true;
      StatiIterEnd(this)
      }

		switch(e.Event)
			{
        case PRE(EV_TALK):
          if (e.EActor == this)
            return PreTalk(e);      
          return NOTHING;
        case EV_TALK:
          if (e.EActor == this && e.EParam)
            return ReThrow(e.EParam,e);
          return NOTHING;
        case EV_BARTER:
          if (e.EActor == this)
            return Barter(e);
          return NOTHING;
        case EV_COW:
          if (e.EActor == this)
            return Cow(e);
          return NOTHING;
        case EV_DISMISS:
          if (e.EActor == this)
            return Dismiss(e);
          return NOTHING;
        case EV_DISTRACT:
          if (e.EActor == this)
            return Distract(e);
          return NOTHING;
        case EV_ENLIST:
          if (e.EActor == this)
            return Enlist(e);
          return NOTHING;
        case EV_FAST_TALK:
          if (e.EActor == this)
            return FastTalk(e);
          return NOTHING;
        case EV_GREET:
          if (e.EActor == this)
            return Greet(e);
          return NOTHING;
        case EV_ORDER:
          if (e.EActor == this)
            return Order(e);
          return NOTHING;
        case EV_QUELL:
          if (e.EActor == this)
            return Quell(e);
          return NOTHING;
        case EV_REQUEST:
          if (e.EActor == this)
            return Request(e);
          return NOTHING;
        case EV_SURRENDER:
          if (e.EActor == this)
            return Surrender(e);
          return NOTHING;
        case EV_TAUNT:
          if (e.EActor == this)
            return Taunt(e);
          return NOTHING;      
        case EV_TERMS:
          if (e.EActor == this)
            return OfferTerms(e);
          return NOTHING;    
        case EV_CAST:
          if (e.EActor==this)
            return Cast(e);
          else
            return NOTHING;
        case EV_INVOKE:
          if (e.EActor==this)
            return Invoke(e);
          else
            return NOTHING;
        case EV_DISGUISE:
          if (e.EActor==this)
            return DonDisguise(e);
          else
            return NOTHING;
        case EV_RESEARCH:
          if (e.EActor==this)
            return Research(e);
          else
            return NOTHING;
				case EV_MOVE:
        case EV_PUSH:
        case EV_JUMP:
					EVerify(T_CREATURE,0,0,0)
					if(e.EActor==this)
						return Walk(e);
					else
						return NOTHING;
        case EV_TURNING:
 					EVerify(T_CREATURE,0,0,0)
					if(e.EActor==this)
						return Turn(e);
					else
						return NOTHING;
				case EV_PICKUP:
					EVerify(T_CREATURE, 0,T_ITEM,0)
					if(e.EActor==this)
						return PickUp(e);
				 break;
				case EV_DROP:
					EVerify(T_CREATURE, 0,T_ITEM,0)
					if(e.EActor==this)
						return Drop(e);
				 break;
        case PRE(EV_EFFECT):
          /* Chromatic Orb kludge */
          e.EMagic = TEFF(e.eID)->Vals(0);
          return NOTHING;
        case EV_EFFECT:
          return MagicEvent(e);
				case EV_NATTACK:
          //EVerify(T_CREATURE, T_CREATURE, 0, 0);
					if(e.EActor==this)
						return NAttack(e);
				 break;
        case EV_WATTACK:
          //EVerify(T_CREATURE, T_CREATURE, 0, 0);
					if(e.EActor==this)
						return WAttack(e);
				 break;
        case EV_RATTACK:
					//EVerify(T_CREATURE, 0, T_ITEM, T_ITEM);
					if(e.EActor==this)
						return RAttack(e);
				 break;
        case EV_LOADXBOW:
          //EVerify(T_CREATURE,0,T_ITEM,0);
          if (e.EActor == this)
            return LoadCrossbow(e);
         break;
        case EV_SATTACK:
					//EVerify(T_CREATURE, 0, T_ITEM, T_ITEM);
					if(e.EActor==this)
						return SAttack(e);
				 break;
        case EV_OATTACK:
					EVerify(T_CREATURE, T_CREATURE, 0, 0);
					if(e.EActor==this)
						return OAttack(e);
				 break;
        case PRE(EV_STRIKE):
					EVerify(T_CREATURE, 0, 0, 0);
          if(e.EActor==this && (!e.ETarget || e.ETarget->isCreature()))
						return PreStrike(e);
				 break;
				case EV_STRIKE:
					EVerify(T_CREATURE, 0, 0, 0);
					if(e.EActor==this)
						return Strike(e);
				 break;
        case POST(EV_STRIKE):
          /* Monsters with M_SKIRMISH will run away after landing a single
             successful blow, then return after a short period. This should
             really be somewhere in the monster AI, but it's just so simple
             to put it here... */
          if (e.EActor != this)
            break;

          if (e.isHit && e.aDmg)
            if (e.EActor->isMonster() && e.EActor->HasMFlag(M_SKIRMISH) &&
                (e.EActor->cHP < e.EActor->mHP))
              if (!HasStati(AFRAID) && !random(2))
                e.EActor->GainTempStati(AFRAID,NULL,10+random(10),SS_MONI,
                                          FEAR_SKIRMISH);
          
          /* ww: add xDmg to this calc so that Sneak Attack damage counts
           * -- certainly in character for priests of pain! */
          if (HasAbility(CA_FEED_UPON_PAIN))
            if ((e.aDmg + e.xDmg)> 10 && cHP < mHP + Attr[A_THP])
              cHP = min(mHP + Attr[A_THP],cHP + (e.aDmg + e.xDmg) / 10);
            
         break;
				case EV_HIT:
					//EVerify(T_CREATURE, T_CREATURE, 0, 0);
					if(e.EActor==this)
						return Hit(e);
				 break;
				case EV_MISS:
					EVerify(T_CREATURE, T_CREATURE, 0, 0);
					if(e.EActor==this)
						return Miss(e);
				 break;
        case EV_DAMAGE:
          if (e.EVictim == this) {
            EVerify(0, T_CREATURE, 0, 0);
            return Damage(e);
            }
         break;
        case EV_PDAMAGE:
          if (e.EVictim == this) {
            EVerify(0, T_CREATURE, 0, 0);
            return PDamage(e);
            }
         break;
        case POST(EV_DAMAGE):
          if (isPlayer() && theGame->InPlay())
            thisp->ShowStats();
          if (e.DType == AD_SLEE)
            break; 
      // fall through
        case POST(EV_PUSH):
          if (e.EVictim == this)
            if (!(Flags & F_DELETE))
              Awaken();
          return NOTHING;
        case EV_DEATH:
          EVerify(0, T_CREATURE, 0, 0);
          if (e.EVictim == this)
            return Death(e);
         break;
				case EV_ATTACKMSG:
					EVerify(T_CREATURE, 0, 0, 0);
					if(e.EActor==this)
						return AttackMsg(e);
				 break;
				case EV_WIELD:
				 return Wield(e);
				case EV_REMOVE:
          return TakeOff(e);
        case EV_SHIFT:
          if (ResistLevel(AD_POLY) == -1)
            {
              IPrint("You find you cannot change form!");
              return ABORT;
            }
          SetSilence();
          RemoveStati(POLYMORPH);
          UnsetSilence();
          if (e.eID == tmID) {
            StateFlags |= MS_KNOWN;
            IDPrint(NULL,"The <Obj1> changes back into <his:Obj1> natural form.",this);
            }
          else
            IDPrint(NULL,"The <Obj> changes into a <Res>.",this,e.eID);
          Shapeshift(e.eID,true,NULL);
          GainPermStati(POLYMORPH,NULL,SS_MISC,1,0,e.eID);
          return DONE;
        case EV_HIDE:
          return Hide(e);
        case EV_SEARCH:
          return Search(e);
        case EV_DIG:
          return Dig(e);
        case EV_PICK_POCKET:
          EVerify(T_CREATURE,0,0,0);
          if (e.EActor == this)
            return PickPocket(e);
         break;
        case EV_ASCEND:
          EVerify(T_CREATURE,0,0,0);
          if (e.EActor == this)
            return Ascend(e);
         break;
        case EV_DESCEND:
          EVerify(T_CREATURE,0,0,0);
          if (e.EActor == this)
            return Descend(e);
         break;
        case EV_MOUNT:
          if (e.EActor == this)
            return Mount(e);
         break;
        case EV_DISMOUNT:
          if (e.EActor == this)
            return Dismount(e);
         break;
				case EV_TURN:
					DoTurn();
				 return DONE;
				case EV_MAGIC_HIT:
					if(e.EActor==this)
						return MagicHit(e);
         break;
				case EV_MAGIC_STRIKE:
					if(e.EActor==this) 
            return MagicStrike(e);                                       
         break;
        case EV_PHASE:
          if (e.EActor==this)
            return Phase(e);
         break;
        case EV_BREAK:
          if (e.EActor == this) {  
            if (e.EItem)
              e.ETarget = e.EItem;
            if (!e.EVictim)
              { IPrint("Break what?");
                return ABORT; }
            if (e.EVictim->isCreature())
              { IPrint("Use the normal attack interface for that.");
                return ABORT; }
            if (e.EActor->AttackMode() == S_MELEE ||
                e.EActor->AttackMode() == S_DUAL)
              Throw(EV_WATTACK,e.EActor,e.ETarget);
            else
              ThrowVal(EV_SATTACK,A_KICK,e.EActor,e.ETarget);              
            return DONE;
            }
         break;
			}
		if (e.isVerb && (e.EActor == this))
			return HandleVerb(e);
  	return NOTHING;
	}

void Creature::ExtendedAction()
  {
    EvReturn r; String why;
    Creature *c; int16 i, n, d; uint16 xy;  
    Status *s = GetStati(ACTING);
    
    if (isPlayer())
      if (thisp->MyTerm->CheckEscape()) {
        HaltAction("pressed escape",false);
        if (!HasStati(ACTING))
          return;
        }
    
    MapIterate(m,c,i) {
      if (dist(c->x,c->y,x,y) <= 18) {
        if (c->isCreature() && c != this) 
          if (m->At(c->x,c->y).Visibility & VI_DEFINED)
            if (Percieves(c) & (PER_VISUAL|PER_INFRA|PER_BLIND|PER_TREMOR) &&
                c->m->LineOfFire(x,y,c->x,c->y,this)) {
              if (c->isHostileTo(this) && !c->HasStati(ASLEEP) &&
                  c->Attr[A_MOV] > 0)
              {
                why = Format("hostile %s",(const char*)c->Name(0));
                HaltAction(why,true);
                return;
              }
              else if (this->isHostileTo(c) && !c->HasStati(ASLEEP)) 
              {
                why = Format("hostile to %s",(const char*)c->Name(0));
                HaltAction(why,true);
                return;
              } 
            }
        if (c->isType(T_PORTAL) && s->Val == EV_MOVE)
          if (Percieves(c))
            if (isPlayer() && thisp->Opt(OPT_STOP_STAIRS))
            {
              HaltAction("stairs",true);
              return;
            }
      }
    }

    if (HungerState() <= STARVING)
      if (s->Val != EV_EAT)
        { HaltAction("starving",true); return; }
    
    if (s->Duration)
      { Timeout += 10; return; }

    if (s->Val == EV_REST) {
      int badStati[] = {
        CONFUSED,
        STUNNED,
        AFRAID,
        HALLU,
        EXPOSED,
        BLIND,
        NAUSEA,
        0
      };

      StatiIter(this)
        if (S->Duration > 0 && S->Duration < 1000) {
          for (int j=0;badStati[j];j++)
            if (S->Nature == badStati[j]) {
              Timeout += 10; 
              StatiIterBreakout(this,return)
            }
          if ((S->Nature == POLYMORPH && S->h != myHandle) ||
              (S->Nature != WEP_SKILL && S->eID && 
               !TEFF(S->eID)->HasFlag(EF_NOTBAD))) {
            Timeout += 10; 
            StatiIterBreakout(this,return)
          }
        } 
      StatiIterEnd(this)
      
      if (!(uMana*2 >= nhMana())) { 
        HaltAction("too little mana to rest", true); 
        return; 
      }
      if (uMana == 0) { 
        HaltAction("mana fully recovered", true); 
        return; 
      } 
      Timeout += 10;
      return;
      }

    /* Running into Corners and Intersections */
    if (s->Val == EV_MOVE && s->Mag < 50) {
      if (isPlayer())
        thisp->AutoPickupFloor();

      if (isPlayer() && thisp->Opt(OPT_STOP_PHASE)) {
        if (m->SolidAt(x,y) != m->SolidAt(x+DirX[s->Mag],y+DirY[s->Mag])) {
          bool incor = onPlane() != PHASE_MATERIAL;
          bool meld = HasAbility(CA_EARTHMELD);
          if (m->RunOver(x,y,false,this,DF_ALL_SAFE,incor,meld) &&
              m->RunOver(x+DirX[s->Mag],y+DirY[s->Mag],false,this,DF_ALL_SAFE,incor,meld)) { 
            HaltAction("phase boundary", true);
            return;
          }
        }
      }

      if (!m->RunOver(x + DirX[s->Mag], y + DirY[s->Mag], false, this,
                      DF_ALL_SAFE, false, false))
        {
          if (isPlayer() && !thisp->Opt(OPT_STOP_CORNER)) {
            int16 diag_count, straight_count;
            d = -1; diag_count = straight_count = 0;
            for(i=0;i!=8;i++) 
              if (i != OppositeDir(s->Mag))
                if (m->RunOver(x+DirX[i],y+DirY[i], false, this, DF_ALL_SAFE,
                      false, false))
                  {
                    /* If diagonal movement is possible (i.e.,
                       we ran up against the wall of an open
                       space like a room), stop running. */
                    if (i >= 4)
                      diag_count++;
                    else { 
                      straight_count++;
                      d = i;
                      }
                    
                  }
            if (d == -1 && !diag_count) {
              /* Dead End; stop running */
              HaltAction("dead end", true);
              return;
              }
            else if (diag_count > 1 || d == -1) {
              HaltAction("direction needed", true);
              return;
              }
            else if (straight_count > 1) {
              HaltAction("fork in the tunnel", true);
              return;
              }
            s->Mag = d;
            }
        }
      else if (isPlayer() && thisp->Opt(OPT_STOP_INTER) && 
           TREG(m->RegionAt(x,y))->HasFlag(RF_CORRIDOR))
        {
          d = 0;
          for (i=0;i!=4;i++)
            if (i != s->Mag && i != OppositeDir(s->Mag)) {
              if (!m->SolidAt(x+DirX[i],y+DirY[i]))
                d = 1;
              if (m->FDoorAt(x+DirX[i],y+DirY[i]))
                if (!(m->FDoorAt(x+DirX[i],y+DirY[i])->DoorFlags & DF_SECRET))
                  d = 1;
              }
          if (d) {
            /* Intersection; stop running */
            HaltAction("intersection", true);
            return;
            }
        }
      if (isPlayer() && thisp->Opt(OPT_STOP_REGION))
        if (m->RegionAt(x,y) != m->RegionAt(x+DirX[s->Mag],y+DirY[s->Mag]))
          {
            /* At Region Boundry; stop running */
            HaltAction("region boundry", true);
            return;
          }
      }
    else if (s->Val == EV_ZAP) {
      // ww: wand-identify items
      Item * wand = NULL, *target = NULL;
      Item * th;
      rID ident = FIND("identify;wand"); // HACK
      for (th = FirstInv();th && !wand;th = NextInv()) 
        if (th->eID == ident && th->isKnown(KN_MAGIC) &&
            th->Type == T_WAND) wand = th;
      if (!wand) {
        HaltAction("no Wands of Identify", true);
        return;
      } 
      for (th = FirstInv();th && !target;th = NextInv()) 
        if (ThrowEff(EV_RATETARG,ident,th,th,th,th) != ABORT)
          target = th;
      if (!target) {
        HaltAction("nothing left to identify", true);
        return;
      }
      r = ThrowEff(EV_ZAP, ident, this, target, wand); 
      if (r == ABORT)
        HaltAction("failed to zap wand", true);
      return; 
    } else if (s->Val == EV_CAST) {
      // ww: 
      // ww: cast all "detect" spells
      if (!isPlayer()) {
        HaltAction("only players can use this feature", true);
        return;
      } 
      if (cMana() > nhMana()/2) { 
        // look for detect spells
        for(i=0;i!=theGame->LastSpell();i++)
          if (SpellRating(theGame->SpellID(i),0,true) != -1) {
            rID eID = theGame->SpellID(i); 
            TEffect * te = TEFF(eID); 
            EffectValues * ef = te->Vals(0); 
            if (ef && (te->ManaCost < cMana()) && 
                ((ef->eval == EA_INFLICT && ef->xval == DETECTING &&
                  !HasStati(DETECTING,ef->yval)) ||
                (te->Purpose & EP_BUFF && 
                 (te->HasFlag(EF_DLONG) || te->HasFlag(EF_DXLONG) ||
                 te->HasFlag(EF_PERSISTANT)) &&
                 !HasEffStati(-1,eID) &&
                 te->PEvent(EV_ISTARGET,this,eID) == SHOULD_CAST_IT))
                ) 
                {
              // we have a winner!
              EventInfo e; 
              EvReturn r; 
              e.Clear();
              e.EActor = this;
              e.eID = eID; 
              ASSERT(cMana() >= te->ManaCost); 
              if (thisp->Spells[i] & SP_INNATE) 
                r =ReThrow(EV_INVOKE,e);
              else 
                r = ReThrow(EV_CAST,e);
              if (r == ABORT) {
                HaltAction(Format("unable to cast %s",NAME(eID)),false);
              } 
              return; 
            } 
          } 
        HaltAction("no more detection or buffing spells", true); 
        return; 
      } else { 
        // wait for more mana
        Timeout += 10;
        return; 
      } 
    } 
    
    if (s->Val == EV_MACRO) {
      r = ThrowEff(EV_MACRO,s->eID,this);
      if (r == ABORT)
        RemoveStati(ACTING);
      if (T1->GetMode() == MO_INV)
        ChooseAction();
      return;
      }
    
    if (s->Val == EV_SATTACK) {
      r = TryToDestroyThing(oThing(s->h));
        // ThrowVal(s->Val,A_KICK,this,oThing(s->h));
    } else if (s->h && oThing(s->h)->isCreature()) 
      r = Throw(s->Val,this,oCreature(s->h));
    else {
      if (s->Val == EV_MOVE && s->Mag >= 50) {
        xy = m->PathPoint(s->Mag-50);
        if (!xy)
          { RemoveStati(ACTING); return; }
        i = DirTo(xy%256,xy/256);
        r = ThrowDir(s->Val,i,this,NULL,oThing(s->h));
        s->Mag++;
        }
      else if (s->Mag || s->Val == EV_MOVE)
        r = ThrowDir(s->Val,s->Mag,this,NULL,oThing(s->h));
      else
        r = Throw(s->Val,this,NULL,oThing(s->h));
      }
    if (r == ABORT)
      RemoveStati(ACTING);
  }

const char* Creature::ActingVerb()
  {
    static String s;
    if (GetStatiEID(ACTING))
      s = NAME(GetStatiEID(ACTING)); 
    else
      s = Lookup(ActionVerbs,GetStatiVal(ACTING));   
    return s;
  }

void Creature::HaltAction(const char *why, bool force_halt)
  {
    rID eID; 
    ASSERT(HasStati(ACTING));
    if (!force_halt) {
      if (!yn(XPrint("Do you want to stop <Str> (<Str>)?",
               (const char*)ActingVerb(),why),true))
        return;
      }
    IPrint("You stop <Str> (<Str>).",(const char*)ActingVerb(),
        why);
               
    RemoveStati(ACTING);
    if (isPlayer())
      thisp->MyTerm->LOptionClear();
  }

void Player::RunDir(Dir d)
  {
    if (d == CENTER)
      return;
    if (ThrowDir(EV_MOVE,d,this) == ABORT)
      return;
    GainPermStati(ACTING,NULL,SS_MISC,EV_MOVE,d,0);
  }


bool Player::RunTo(int16 tx, int16 ty)
  {
    if (HasStati(ACTING))
      return false;
    if (!m->ShortestPath(x,y,tx,ty,this,DF_ALL_SAFE) &&
        !m->ShortestPath(x,y,tx,ty,this,DF_IGNORE_TRAPS) &&
        !m->ShortestPath(x,y,tx,ty,this,DF_IGNORE_TRAPS|DF_IGNORE_TERRAIN))
      return false;
    GainPermStati(ACTING,NULL,SS_MISC,EV_MOVE,50,0);
    Timeout++;
    return true;
  }

/* Notes of Poison and Disease:
     Poison and disease each have iterative effects over time in Incursion,
   rather then just the normal "one minute later" check. Essentially, a
   poison or disease is an effect, like a spell, that repeats every N turns
   if a fortitude save is failed. The Effect has the same values as a normal
   effect, with the following exceptions:
   * aval -- either AR_POISON or AR_DISEASE. Yes, poison/disease is technically
     a type of area effect; practically, we can use this field for the indicator
     because no sensible poison or disease will ever affect an area.
   * cval -- the iteration period of the poison or disease -- every [cval] turns,
     a new saving throw must be made to avoid further damage from the poison.
   * sval -- all poisons and diseases require a Fortitude save, so instead of
     storing the save *type*, this field stores the save DC.
   * lval -- the number of successful saving throws that must be made for the
     poison or disease to pass safely through the victim's system. Particulairly
     lethal poisons require this number of *consecutive* successful saving throws,
     as indicated by the EF_LETHAL flag.
   
     The POISONED and DISEASED statuses each have the effect's eID in their eID
   field. Val holds the number of turns since the last saving throw, while
   Mag holds the number of successful [consecutive] saving throws to date.
*/


void Creature::DoTurn()
  {
    int16 i,j; Status *s; uint8 dmg; 
    bool isEngulfed = HasStati(ENGULFED);
    hObj h; Item *it;
    if (!m || x == -1)
      return;

    /* This should really be the saving throw DC of the effect,
       but that's a lot of extra work, so for now... */
    if (HasStati(PARALYSIS) && max(SkillLevel(SK_CONCENT),SkillLevel(SK_ESCAPE_ART)) >= 15)
      if (SavingThrow(FORT,18,SA_MAGIC|SA_PARA))
        {
          IPrint("You break the paralysis with Concentration!");
          RemoveStati(PARALYSIS);
        }
    
    if (m->SolidAt(x,y) && onPlane() == PHASE_MATERIAL)
      if (!HasAbility(CA_EARTHMELD) && (!isPlayer() || !thisp->Opt(OPT_GHOSTWALK)))
        {
          IPrint("You are being crushed alive!");
          ThrowDmg(EV_DAMAGE,AD_NORM,random(20)+1,"being crushed alive",this,this);
          if (isDead())
            return;
        }
    
    if (isMType(MA_AQUATIC) && !HasMFlag(M_AMPHIB))
      if (!TTER(m->TerrainAt(x,y))->HasFlag(TF_WATER))
        {
          IDPrint("You are suffocating!",
                  "The <Obj> suffocates!", this);
          ThrowDmg(EV_DAMAGE,AD_NORM,random(6)+1,"being a fish out of water",this,this);
          if (isDead())
            return;
        }
        
    if ((theGame->GetTurn() / 60) % 3 == 1)
      if (HasFeat(FT_RESILIENT))
        if (cHP < (mHP + GetAttr(A_THP)))
          if (cHP >= ((mHP + GetAttr(A_THP)) * (100 - (Mod(A_CON)+3)*5)) / 100)
            {
              cHP++;
              if (cHP == (mHP + GetAttr(A_THP)))
                IPrint("You shrug off the effects of sundry scrapes and bruises.");
            }
            
    if (HasStati(INVIS) || HasStati(INVIS_TO) ||
          onPlane() == PHASE_ETHERIAL)
      RippleCheck(3);
        
    if (isEngulfed)
      {
        Creature *cr = (Creature*) GetStatiObj(ENGULFED);
        ASSERT (cr && cr->isCreature())
        
        if (onPlane() != cr->onPlane())
          {
            IPrint("You pass through the <Obj>'s <Str>!",
              cr, GetStatiVal(ENGULFED) == EG_ENGULF ? "mass" : "belly");
            cr->IPrint("The <Obj> passes through your <Str>!",
              this, GetStatiVal(ENGULFED) == EG_ENGULF ? "mass" : "belly");
            cr->DropEngulfed(this);
            goto DoneEngulf;
          }
        
        if (HasSkill(SK_ESCAPE_ART))
          if (!(HasStati(PARALYSIS) || HasStati(AFRAID) ||
                HasStati(STUNNED) || HasStati(ASLEEP)))
            if (SkillCheck(SK_ESCAPE_ART, max( 
                  10 + cr->ChallengeRating()*2,
                  10 + cr->GetAttr(A_STR) ),true,false))
              {
                // better messages later
                IPrint("You escape the <Obj>!",cr);
                cr->DropEngulfed(this);
                goto DoneEngulf;
              }
         
        
        /* Logically, this should be in the ENGULFER's DoTurn,
           but that would require a second HasStati call for
           EVERY monster EVERY turn, and also not guarantee the 
           player an Escape Artist check BEFORE suffering the
           potentially very bad A_DGST attack. Let's avoid the
           timing quirk there. */
        if (cr->HasAttk(A_DGST))
          ThrowVal(EV_SATTACK,A_DGST,cr,this);
        if (cr->HasAttk(A_CRUS))
          ThrowVal(EV_SATTACK,A_CRUS,cr,this);
      }
    DoneEngulf:

    if (HasStati(GRAPPLING))
      {
        EventInfo xe;
        if (HasFeat(FT_CHOKE_HOLD)) {
          StatiIterNature(this,GRAPPLING)            
            Creature *cr;
            cr = oCreature(S->h);
            if (cr->isDead() || cr->HasStati(SLEEPING))
              continue;
            if (!cr->HasMFlag(M_HUMANOID))
              continue;
            if (!cr->isMType(MA_LIVING))
              continue;
            if (cr->cHP*2 <= cr->mHP + cr->GetAttr(A_THP))
              {
                if (!cr->SavingThrow(FORT,StunAttackDC(),SA_PARA|SA_GRAB))
                  { IPrint("You fail to render the <Obj> unconscious.", cr);
                    continue; }
                IPrint("You knock out the <Obj> with a sleeper hold.", cr);
                cr->IPrint("The world goes dark...");
                cr->GainTempStati(SLEEPING,NULL,-1,SS_ATTK,SLEEP_DEEP);
                RemoveStati(GRAPPLING,-1,-1,-1,cr);
                cr->RemoveStati(GRAPPLED,-1,-1,-1,this);
                KillXP(cr,100);
              }
          StatiIterEnd(this)
          }
        
        if (HasFeat(FT_EARTHS_EMBRACE)) {
          StatiIterNature(this,GRAPPLING)
            
            xe.Clear();
            xe.Dmg = DmgVal(S_BRAWL,oCreature(S->h)->GetAttr(A_SIZ) >= SZ_LARGE);
            xe.vDmg = xe.Dmg.Roll();
            xe.AType = A_CRUS;
            xe.DType = AD_BLUNT;
            xe.isHit = true;
            xe.EActor = this;
            xe.EVictim = oCreature(S->h);
            if (xe.EVictim->isDead() ||
                xe.EVictim->HasStati(SLEEPING))
              continue;
            ReThrow(EV_DAMAGE,xe);
            ReThrow(EV_ATTACKMSG,xe);
          StatiIterEnd(this)
          }
      
      }

    if (!random(30) && isCharacter())
      {
        static int oldXP = 0;
        switch (HungerState())
          {
            case SATIATED:
              if (random(10))
                break;
            case BLOATED:
              Abuse(A_CON,1);
             break;
            case HUNGRY:
              if (random(5))
                break;
              if (HasAbility(CA_FASTING))
                {
                  if (oldXP < GetXP())
                    Exercise(A_CON,random(3)+1,ECON_HUNGER,45);
                  break;
                }
            
            case WEAK:
            case FAINTING:
              Abuse(A_CON,random(2)+1);
            case STARVING:
              Abuse(A_CON, 1);
             break;
            default:
              if (oldXP != GetXP() && !random(10))
                Exercise(A_CON,1,ECON_HUNGER,15);
             break;
          }
        switch (Encumbrance())
          {
            case EN_EXTREME:
              if (oldXP < GetXP())
                Exercise(A_STR,random(3)+1,ESTR_BURDEN,50);
             break;
            case EN_HEAVY:
              if (oldXP < GetXP())
                Exercise(A_STR,1,ESTR_BURDEN,30);
             break;            
          }
        if (InSlot(SL_ARMOR) && InSlot(SL_ARMOR)->isGroup(WG_HARMOR))
          if (oldXP < GetXP())
            {
              Exercise(A_STR,random(2)+1,ESTR_ARMOR,50);
              Exercise(A_CON,1,ECON_ARMOR,35);
              Abuse(A_DEX,1);
            }
        if (m && m->Depth > ChallengeRating())
          if (oldXP < GetXP())
            Exercise(A_LUC,random((m->Depth-ChallengeRating())*5)+1,ELUC_RISK,100);
        oldXP = GetXP();
      }
    if (isCharacter() && !random(10))
      {
        rID gID; bool isAnger;
        int16 i, gNum, interval;
        
        /* Hung Intervention? */
        if (HasStati(HUNG_PULSE) && isThreatened())
          { gNum = GetStatiVal(HUNG_PULSE);
            gID = theGame->GodID(gNum);
            isAnger = (thisp->Anger[gNum] != 0);
            goto DoIntervention; }
        
        
        /* 3/4 chance of our patron god, otherwise any god you have
           prayed to at any point in the game. */
        i=0;
        if (random(4) && thisp->GodID)
          gID = thisp->GodID;
        else do { 
          gID = GodIDList[random(nGods)];
          if (thisp->getGodFlags(gID) & GS_INVOLVED)
            break;
          gID = 0;
          }
        while (i++ != 30);
        
        if (!gID)
          goto NoIntervention;
          
        gNum = theGame->GodNum(gID);
        isAnger = (thisp->Anger[gNum] != 0);
        interval = TGOD(gID)->GetConst(isAnger ?
          GODANGER_INTERVAL : GODPULSE_INTERVAL);
        
        if (thisp->xpTicks - thisp->lastPulse[gNum] < interval *
               ((gID == thisp->GodID) ? 1 : 5))
          goto NoIntervention;
        
        
        if (TGOD(gID)->HasFlag(isAnger ?
              GF_COMBAT_ANGER : GF_COMBAT_PULSE))
          if (!isThreatened())
            {
              GainPermStati(HUNG_PULSE,NULL,SS_MISC,gNum);
              goto NoIntervention;
            }
        
        DoIntervention:
        if (!thisp->Anger[gNum])
          if (thisp->getGodFlags(gID) & (GS_ANATHEMA|GS_FORSAKEN))
            goto NoIntervention;
        if (ThrowEff(isAnger ? EV_ANGER_PULSE : EV_GODPULSE,gID,this) != NOTHING)
          {
            thisp->lastPulse[gNum] = thisp->xpTicks;
            if (isAnger)
              thisp->setGodFlags(gID,GS_KNOWN_ANGER);
            else
              thisp->resetGodFlags(gID,GS_KNOWN_ANGER);
            RemoveStati(HUNG_PULSE,-1,gNum);
            /* If a god /other than/ your patron is giving anger pulses,
               the anger /must/ reduce over time, otherwise, frex, a 
               character who forsook Xel and turned Good is trapped
               between sacrificing sapients for penance and being drained
               by an increasingly angry Xel until she dies. */ 
            if (isAnger && (gID != thisp->GodID)) {
              thisp->Anger[gNum] = max(0,thisp->Anger[gNum] - 2);
              if (!thisp->Anger[gNum])
                thisp->GodMessage(gID,MSG_EXPENDED);
              }
          }
        NoIntervention:;
      }
    bool wasFF = isFlatFooted();
    FFCount++;
    if (isFlatFooted())
      {
        if (isPlayer() && !wasFF)
          thisp->statiChanged = true;
        if (HasStati(RAGING))
          {
            IPrint("Free of foes, your rage winds down.");
            RemoveStati(RAGING);
          }
        if (HasStati(FLAWLESS_DODGE,1))
          {
            IPrint("Free of foes, you cease your flawless dodge.");
            /* Don't kill the whole Stati; it has the number of dodges
               stored in the Mag field. Instead, just set the flag to
               zero to indicate inactive. */
            SetStatiVal(FLAWLESS_DODGE,NULL,0);
          }
      }
      
    if (FFCount > 50) {
      int fatRegen = AbilityLevel(CA_FATIGUE_REGEN);
      if (fatRegen) {
        if (FFCount > 50 + (50 / fatRegen)) {
          if (cFP < Attr[A_FAT]) 
            cFP++;
          FFCount = 50; 
        } 
      } else  FFCount = 30;
    } 

    AoO = HasFeat(FT_COMBAT_REFLEXES) ? max(1,1 + Mod2(A_DEX)) : 1;
    if (HasFeat(FT_MOBILITY)) AoO++; 
    if (HasAttk(A_GAZE))
      DoGazeAttack();
    if (HasAttk(A_PROX) || HasAttk(A_CPRX))
      DoProxAttack();


    if (uMana > 0) {
      // ww: currently the monster AI is not smart enough to conserve mana,
      // so monster mages almost always run out of it fighting each other
      // before you arrive ...
      if (!isPlayer() || cMana() >= ((nhMana()*min(35+SkillLevel(SK_CONCENT)*2,80))/100)) {
        /* 
         * Before it took about 60 turns to get 1 point back if you were
         * down one, and 60X turns if you were down X points. 
         * 
         * It now takes X turns to regenerate 1% of your mana if you are at
         * 99%. It takes 2X to regenerate 1% if you are at 98%, etc.
         * The old system dramatically favored low-mana mages. This new one
         * hurts them. To equalize, we "shift" the curve to the right by
         * padding current and total mana by a constant displacement. (See
         * examples below). 
         *
         * Thus the total time is quadratic: if you're down 5% it takes 25
         * times as long as being down 1% to recover fully. 
         *
         * ManaPulse stores the number of turns until we get a point 
         * (not a percentage point) back. 
         */
        if (ManaPulse > 0) {
          ManaPulse--;
          // IPrint(Format("ManaPulse -- %d, %d.",ManaPulse,theGame->Turn));
        } else {
          uMana--; 
          // if you are down 5*N%, it takes you N turns to get back 1%
          int N = (tMana() - (cMana() + hMana)) * 20 / max(1,tMana());
          ManaPulse = N * 100 / tMana(); 
        } 
      } 
    }

    UpdateStati();
    if (x == -1 || !m)
      return;
    if (!isEngulfed)
      TerrainEffects();
      
    if (HasStati(ANIMAL_COMPANION) && isMonster())
      ((Monster *)this)->ts.Retarget(this); 
    
    for (it = FirstInv();it;it = NextInv()) {
      it->UpdateStati();
      if (it == InSlot(SL_LIGHT)) {
        if (it->Age <= 1) {
          IDPrint("Your <Obj2> burns out!",
              "The <Obj1>'s <Obj2> burns out!",this,it);
          it->Remove(true);
          if (isPlayer())
            thisp->UpdateMap = true;
          }
        else
          it->Age--;
        }
      }
      
    if (isPlayer() && HasStati(ACTING))
      { 
        thisp->MyTerm->ShowTraits();
        thisp->MyTerm->Update();
      }

    if (HasStati(MOUNTED)) {
      Creature *cr = (Creature*)GetStatiObj(MOUNTED);
      if (cr && cr->HasStati(AFRAID))
        {
          if (SkillCheck(SK_RIDE,15 + cr->ChallengeRating()))
            {
              IPrint("You calm your frightened mount.");
              cr->RemoveStati(AFRAID);
            }
          else
            {
              IPrint("You are thrown from your panicked <Obj>!",cr);
              ThrowVal(EV_DISMOUNT,DSM_THROWN,this,cr);
            }
        }
      Throw(EV_TURN,cr);
      }

	  // ww: if a monster is generated over a chasm in a sleep encounter, 
	  // we can get here with x=y=-1
    if (x == -1 || !m) return;

    if (m->FieldAt(x,y,FI_CONTINUAL) && !isEngulfed)
      {
        TEffect *te; EventInfo e;
        for(i=0;x != -1 && m && m->Fields[i];i++)
          if (m->Fields[i]->FType & FI_CONTINUAL)
            if (dist(x,y,m->Fields[i]->cx,m->Fields[i]->cy) <= m->Fields[i]->rad)
              {
                te = TEFF(m->Fields[i]->eID);
                j = 0;
                while (te->Vals(j)->aval != AR_FIELD && 
                          te->Vals(j)->aval != AR_MFIELD)
                  {
                    j++;
                    ASSERT(te->Vals(j));
                  }
                e.Clear();
                e.EActor = oCreature(m->Fields[i]->Creator);
                e.EVictim = this;
                e.eID = m->Fields[i]->eID;
                e.efNum = j;
                ReThrow(EV_MAGIC_STRIKE,e);
              }
      }

    if (x == -1 || !m) return;

    StatiIterNature(this,POISONED)
          if (HasStati(SLOW_POISON))
            continue;
          if (ResistLevel(AD_TOXI) == -1 ||
              ResistLevel(AD_POIS) == -1)
            {
              RemoveStati(POISONED);
              continue;
            }
          if (S->Val >= TEFF(S->eID)->ef.cval)
            {
              S->Val = 0;
              if (SavingThrow(FORT,TEFF(S->eID)->ef.sval,SA_POISON))
                {
                  S->Mag++;
                  if (S->Mag >= TEFF(S->eID)->ef.lval - HasFeat(FT_HARDINESS)*2)
                    {
                      IPrint("You have overcome the <Res>.",S->eID);
                      Exercise(A_CON,random(12)+1,ECON_OVERCOME,50);
                      RemoveEffStati(S->eID);
                      break;
                    }
                  IPrint("You bear up under the poison.");
                  
                }
              else {
                if (TEFF(S->eID)->HasFlag(EF_LETHAL))
                  S->Mag = 0;
                IPrint("The <Res> courses through your veins...",S->eID);
                ThrowEff(EV_EFFECT,S->eID,this,this);
                if (HasStati(ACTING)) HaltAction("poison",false); 
                }
            }
          else
            S->Val++;
    StatiIterEnd(this)
    
    DiseasePulse(false, false);
    
    if (ResistLevel(AD_FEAR) == -1)
      RemoveStati(AFRAID);
    if (isMType(MA_HAS_BLOOD)) {
      StatiIterNature(this,BLEEDING)
          IPrint("Your wounds bleed.");
          ThrowDmg(EV_DAMAGE,AD_NORM,S->Mag,"bleeding",this,this);
      StatiIterEnd(this)
      }
    StatiIterNature(this,REGEN)
          if (cHP < mHP+Attr[A_THP])
            if (S->Mag > random(100))
              cHP = min(mHP+Attr[A_THP],cHP + max(1,GetStati(REGEN)->Mag/100));
    StatiIterEnd(this)
    StatiIterNature(this,PERIODIC)
      ASSERT(S->Val);
      if (!(++S->Mag % S->Val)) {
        S->Mag = 1;
        EventInfo e;
        e.Clear();
        if (oThing(S->h) && oThing(S->h)->isItem()) {
          SetEvent(e,EV_EFFECT,this,NULL,oItem(S->h));
          e.isItem = true;
          }
        else
          SetEvent(e,EV_EFFECT,this,oThing(S->h));
        
        e.isPeriodic = true;
        e.eID = S->eID;
        ReThrow(EV_EFFECT,e);
        }
    StatiIterEnd(this)
    if ((i = AbilityLevel(CA_BURNING_HUNGER)))
      GetHungrier(i*2);
      
    if (HasAbility(CA_REGEN)) {
      // ww: constant regeneration makes you hungry!
      int old_cHP = cHP;
      if (AbilityLevel(CA_REGEN) > 0) { 
        cHP = min(mHP+Attr[A_THP],cHP+AbilityLevel(CA_REGEN));

        if (old_cHP < cHP) {
          int curHunger = GetStatiDur(HUNGER);
          if (GetStatiObj(REGEN) && GetStatiObj(REGEN)->isItem())
           IdentByTrial((Item*)GetStatiObj(REGEN));
          IDPrint("You regenerate<Str2>.",
                  "The <Obj> regenerates<Str2>.",
                  this, cHP == (mHP + Attr[A_THP]) ?
                  " fully" : "");
          GetHungrier(5);
          }
        }
      }
  }
  
void Creature::DiseasePulse(bool force, bool rest)
  {
    StatiIterNature(this,DISEASED)
      if (ResistLevel(AD_DISE) == -1) {
        RemoveStati(DISEASED);
        continue;
        } 
      if (S->Val >= TEFF(S->eID)->ef.cval || force)
        {
          S->Val = 0;
          // ww: just in case you somehow have +2 resistance to
          // disease, it should help you here ...
          int16 DC = TEFF(S->eID)->ef.sval -
                                  ResistLevel(AD_DISE);
          if (DC < 0 || SavingThrow(FORT,DC,SA_DISEASE | (rest ? SA_REST : 0)))
            {
              S->Mag++;
              if (S->Mag >= TEFF(S->eID)->ef.lval - HasFeat(FT_HARDINESS)*3)
                {
                  IPrint("You have overcome the <Res>.",S->eID);
                  Exercise(A_CON,random(12)+1,ECON_OVERCOME,50);
                  RemoveEffStati(S->eID);
                  break;
                }
              IPrint("You bear up under your <Res>.",S->eID);
            }
          else {
            if (TEFF(S->eID)->HasFlag(EF_LETHAL))
              S->Mag = 0;
            IPrint("Your <Res> progresses...",S->eID);
            ThrowEff(EV_EFFECT,S->eID,this,this);
            if (HasStati(ACTING)) HaltAction("disease",false); 
            }
        }
      else
        S->Val++;
    StatiIterEnd(this)  
  }

void Creature::GetHungrier(int16 amt)
  {
    int16 fast_percent, hunger_val;
    if (!HasStati(HUNGER))
      return;
    fast_percent = 100 - (HasAbility(CA_FASTING) ? 
      ((66 * AbilityLevel(CA_FASTING)) / ChallengeRating()) : 0);
      
    hunger_val = GetStatiDur(HUNGER);
    
    if (amt < 3)
      {
        if (Dice::Roll(1,100) > fast_percent)
          hunger_val -= amt;
      }
    else
      hunger_val -= max((amt*fast_percent)/100,1);
    
    hunger_val = max(0,hunger_val);
    
    SetStatiDur(HUNGER,-1,NULL,hunger_val);
  }

EvReturn Character::Event(EventInfo &e)
	{
	  if (e.EActor != this)
	    return NOTHING;
	    
	  switch (e.Event)
	    {
	      case PRE(EV_PRAY):
	        return PrePray(e);
	      case EV_PRAY:
	        return Pray(e);
	      case EV_SACRIFICE:
	        return Sacrifice(e);
	      case EV_IBLESSING:
	        return IBlessing(e);
	      case EV_CONVERT:
	        return Convert(e);
	      case EV_INSIGHT:
	        return Insight(e);
	      case EV_CONVERT_ALTAR:
	        return ConvertAltar(e);
	      case EV_RETRIBUTION:
	        return Retribution(e);
	      case EV_JEALOUSY:
	        return Jealousy(e);
	      case EV_GIVE_AID:
	        return GiveAid(e);
	      case EV_ALIGNED:
	        return AlignedAct(e);
	      case EV_CHANGE_ALIGN:
	        return ChangeAlign(e);
	    }
    return NOTHING;
	}




bool Creature::isBoosted(int8 at)
  {
    StatiIterAdjust(this)
        if (S->Nature != ADJUST_INH)
          if (S->Mag > 0)
            if (S->Val == at)
              StatiIterBreakout(this,return true)
    StatiIterEnd(this)
    return false;
  }

bool Creature::isDrained(int8 at)
  {
    StatiIterAdjust(this)
        if (S->Nature != ADJUST_INH)
          if (S->Mag < 0)
            if (S->Val == at)
              StatiIterBreakout(this,return true)
    StatiIterEnd(this)
    return false;
  }

void Creature::GainInherantBonus(int16 at, int16 mag, bool msg)
  {
    int16 MaxBonus, CurrBonus;

    MaxBonus = 5 + AbilityLevel(CA_INHERANT_POTENTIAL);

    CurrBonus = SumStatiMag(ADJUST_INH,at);

    if (CurrBonus == MaxBonus) {
      if (msg)
        IPrint("You feel a profound sense of wastefulness.");
      return;
      }

    if (HasStati(ADJUST_INH,at))
      SetStatiMag(ADJUST_INH,at,NULL,min(MaxBonus,CurrBonus+mag));
    else
      GainPermStati(ADJUST_INH,NULL,SS_MISC,at,min(MaxBonus,mag),0);

    if (msg) switch (at) {
      case A_STR: IPrint("You feel stronger."); break;
      case A_DEX: IPrint("You feel more agile."); break;
      case A_CON: IPrint("You feel hardier."); break;
      case A_INT: IPrint("You feel smarter."); break;
      case A_WIS: IPrint("You feel sharper."); break;
      case A_CHA: IPrint("You feel more charismatic."); break;
      case A_LUC: IPrint("You feel luckier."); break;
      }

   if (isPlayer())
     thisp->statiChanged = true;

  }
  
bool Creature::hasAccessToSpell(rID spID)
  {
    rID spList[128]; int16 i, lv, ns, c;
    Annotation *a;
       
    if (HasEffStati(INNATE_SPELL,spID))
      return true;
       
    StatiIterNature(this,TEMPLATE)
      lv = 1;
      if (TTEM(S->eID)->GetList(SPELL_LIST,spList,127))
        for (i=0;spList[i] || spList[i+1] || spList[i+2];i++)
          {
            if (spList[i] == spID)
              if (SAL(lv) <= CasterLev())
                StatiIterBreakout(this,return true);
            if (spList[i] >= 1 && spList[i] <= 9)
              lv = spList[i];
            if (spList[i] >= 1000 && spList[i] <= 2000)
              if (TEFF(spID)->HasSource(spList[i]-1000))
                if (CasterLev() >= SAL(TEFF(spID)->Level))
                  StatiIterBreakout(this,return true);
            if (spList[i] == 100) {
              if (TEFF(spID)->HasSource(spList[i+1]))
                if (TEFF(spID)->Schools & spList[i+2])
                  StatiIterBreakout(this,return true);
              i += 2;
              }
          }
      TTemplate *tt = TTEM(S->eID);
      a = tt->Annot(tt->AnHead);
      while(a) {
        if (a->AnType == AN_INNATE)
          for(i=0;i!=8 && a->u.sp[i].sID;i++)
            if (a->u.sp[i].sID == spID)
              StatiIterBreakout(this,return true);
        a = tt->Annot(a->Next);
        }
    StatiIterEnd(this)
    
    lv = 1;
    if (TMON(tmID)->GetList(SPELL_LIST,spList,127))
      for (i=0;spList[i] || spList[i+1] || spList[i+2];i++)
        {
          if (spList[i] == spID)
            if (SAL(lv) <= CasterLev())
              return true;
          if (spList[i] >= 1 && spList[i] <= 9)
            lv = spList[i];
          if (spList[i] >= 1000 && spList[i] <= 2000)
            if (TEFF(spID)->HasSource(spList[i]-1000))
              if (CasterLev() >= SAL(TEFF(spID)->Level))
                return true;
          if (spList[i] == 100) {
            if (TEFF(spID)->HasSource(spList[i+1]))
              if (TEFF(spID)->Schools & spList[i+2])
                return true;
            i += 2;
            }
        }

    TMonster *tm = TMON(tmID);
    a = tm->Annot(tm->AnHead);
    /* Dragons each have a list of 12 spell-like abilities, but can only
    use the first N, where N is a number from 1 to 12 based on their
    age catagory. All other creatures can use their full allotment of
    spell-like abilities. */
    if (isMType(MA_DRAGON))
      ns = GetAgeCatagory();
    else
      ns = 1000;
    c = 0;
    while(a) {
      if (a->AnType == AN_INNATE)
        for(i=0;i!=8 && a->u.sp[i].sID;i++) {
          if (a->u.sp[i].sID == spID)
            return true;
          c++;
          if (c >= ns)
            break;
          }
      a = tm->Annot(a->Next);
      }
        
    return false;
  }          
  


int16 Creature::HungerState()
  {
    int16 i;
    if (Type != T_PLAYER || isMType(MA_UNDEAD) || isMType(MA_CONSTRUCT) ||
        isMType(MA_PLANT))
      return CONTENT;
    if (!GetStati(HUNGER))
      return isPlayer() ? STARVED : CONTENT;
    i = GetStati(HUNGER)->Duration;
    if (TMON(mID)->Size >= SZ_LARGE) 
      i /= 2; // ww: large creatures can hold more food
    if (i > SATIATED)
      return BLOATED;
    if (i > CONTENT)
      return SATIATED;
    if (i > PECKISH)
      return CONTENT;
    if (i > HUNGRY)
      return PECKISH;
    if (i > STARVING)
      return HUNGRY;
    if (i > WEAK)
      return STARVING;
    if (i > FAINTING)
      return WEAK;
    if (i > STARVED)
      return FAINTING;
    return STARVED;
  }

int16 Creature::ChallengeRating()
  {
    int16 CR = TMON(mID)->CR, i;
    StatiIterNature(this,TEMPLATE)
      CR = TTEM(S->eID)->CR.Adjust(CR);
    StatiIterEnd(this)
    return max(0,CR);
  }



/*
uint16 Creature::SightRange()
  {
    uint16 sr;

    sr = max(3,4 + Mod(A_WIS));
    if (HasAbility(CA_SHARP_SENSES))
      sr += AbilityLevel(CA_SHARP_SENSES);

    return sr;
  }

uint16 Creature::LightRange()
  {
    int16 lr;
    return 4;
  }

uint16 Creature::ShadowRange()
  { 
    return SightRange()*2; 
  }

*/

bool Creature::LoseFatigue(int16 amt,bool avoid)
  {
    int16 oFP = cFP; bool stat_change = false;
    
    if ((isPlayer() && thisp->Opt(OPT_NO_FATIGUE)) ||
         isMType(MA_UNDEAD) || isMType(MA_CONSTRUCT))
        return true;
    
    if (avoid && cFP < -Attr[A_FAT])
      if (!yn("You are in danger of passing out! Proceed?",true))  
        return false;
    
    if (avoid)
      Exercise(A_CON,random(amt)+1,ECON_FATIGUE,35);

    if (cFP < -Attr[A_FAT])
      if (!SavingThrow(FORT,10 - (cFP/2)))
        {
          IDPrint("You pass out... ",
            "The <Obj> keels over unconscious.",this);
          GainTempStati(ASLEEP,NULL,Dice::Roll(2,10),SS_ENCH,SLEEP_FATIGUE);
          return false;
        }

    cFP -= amt;
    
    /* Hardcoded Essiah Fatigue Clause */
    if (avoid && cFP < 0 && isCharacter() && isThreatened() && isMType(MA_GOOD))
      thisc->gainFavor(FIND("Essiah"),25 * abs(cFP - min(0,oFP)));
    
    if (cFP <= 0 && oFP > 0)
      stat_change = true;
    if (cFP < -Attr[A_FAT] && oFP >= -Attr[A_FAT])
      stat_change = true;
    if (stat_change)
      CalcValues();
    if (isPlayer())
      thisp->MyTerm->ShowStatus();
    return true;
  }

int16 Creature::onPlane()
  {
    if (HasStati(PHASED))       
      return GetStatiVal(PHASED);
    if (HasMFlag(M_INCOR)) {
      if (isMType(MA_SPIRIT))
        return PHASE_ETHERIAL;
      if (isMType(MA_UNDEAD))
        return PHASE_NEGATIVE;
      if (HasMFlag(M_ASTRAL))
        return PHASE_ASTRAL;
      if (isMType(MA_VORTEX))
        return PHASE_VORTEX;
      return PHASE_ETHERIAL;
      }
    return PHASE_MATERIAL;
  }
  
int16 Thing::onPlane()
  {
    if (HasStati(PHASED))
      return GetStatiVal(PHASED);
    if (isType(T_ITEM))
      if (thisi->Owner())
        return thisi->Owner()->onPlane();
    return PHASE_MATERIAL; 
  } 

bool Creature::isAerial()
  {
    if (HasMFlag(M_FLYER) || HasMFlag(M_MFLYER) || HasAbility(CA_FLIGHT) || 
         HasStati(LEVITATION) || HasMFlag(M_INCOR) || HasStati(ELEVATED) ||
         HasStati(PHASED))
      return true;
    Creature * mount = (Creature *)GetStatiObj(MOUNTED);
    if (mount && mount->isAerial()) 
      return true; 
    return false;
  }
  
bool Creature::isSpecialistIn(uint32 School)
  {
    int16 i;
    for(i=0;i!=16;i++)
      if (XBIT(i) & School)
        if (HasStati(SPECIALTY_SCHOOL,i))
          return true;
    return false;
  }

bool Creature::isInvisTo(Creature *watcher)
  {
    if (HasStati(INVIS) || onPlane() != PHASE_MATERIAL)
      return true;
    if (HasStati(INVIS_TO))
      StatiIter(this)
        if (watcher->isMType(S->Mag))
          StatiIterBreakout(this,return true)
      StatiIterEnd(this)
    return false;
  }
  
bool Thing::isRealTo(Creature *watcher)
  {
    Status *s = GetStati(ILLUSION);
    if (!s)
      return true;
    
    /* If you disbelieve the illusionary kobold, you
       disbelieve his illusionary sword, too. */
    if (s->h && oThing(s->h)->isIllusion())
      return oThing(s->h)->isRealTo(watcher);
    
    if (watcher->HasStati(TRUE_SIGHT))
      return false;
    if (watcher->HasMFlag(M_MINDLESS))
      return false;
      
    if (!(s->Val & IL_IMPROVED))
      {
        int32 P;
        P = watcher->Percieves(this);
        
        if (P & (PER_SCENT|PER_BLIND|PER_TREMOR))
          return false;
        if (watcher->HasAbility(CA_SHARP_SENSES))
          return false;
      }
    if (watcher->HasStati(DISBELIEVED,-1,this))
      return false;
    return true;
  }
  
uint16 Thing::getIllusionFlags()
  {
    Status *s = GetStati(ILLUSION);
    ASSERT(s);
    TEffect *te = TEFF(s->eID);
    return te->ef.yval;
  }
  
int16 Thing::getIllusioncraft()
  {
    Thing *t = GetStatiObj(ILLUSION);
    if (t->isIllusion())
      return t->getIllusioncraft();
    else
      return ((Creature*)t)->SkillLevel(SK_ILLUSION);
  }
  
void Thing::DisbeliefCheck(Creature *watcher)
  {
    int16 i; uint32 saveFlags;
    uint16 fl = getIllusionFlags();
    Status *s = GetStati(ILLUSION);
    ASSERT(s);
    
    if (!watcher)
      {
        MapIterate(m,watcher,i)
          if (watcher->isCreature())
            if (watcher != this)
              if (watcher->DistFrom(this) <= 10)
                if (watcher->Percieves(this))
                  DisbeliefCheck(watcher);
        return;
      }
    
    saveFlags = SA_ILLUS | SA_MAGIC;
    if (s->eID) {
      if (TEFF(s->eID)->HasSource(AI_WIZARDRY) ||
          TEFF(s->eID)->HasSource(AI_SORCERY) ||
          TEFF(s->eID)->HasSource(AI_THEURGY) ||
          TEFF(s->eID)->HasSource(AI_BARDIC) ||
          TEFF(s->eID)->HasSource(AI_WITCHCRAFT) )
        saveFlags |= SA_SPELLS;
      if (TEFF(s->eID)->HasFlag(EF_EVIL))
        saveFlags |= SA_EVIL;
      if (TEFF(s->eID)->Schools & SC_NEC)
        saveFlags |= SA_NECRO;
      }
    if (watcher->HasStati(DISBELIEVED,-1,this))
      return;  
    if (watcher->SavingThrow(WILL,s->Val,SA_ILLUS))
      {
        watcher->IPrint("You realize that the <Obj> is an illusion.", this);
        if (s->h && oThing(s->h)->isCreature())
          if (oCreature(s->h)->Percieves(watcher))
            oCreature(s->h)->IPrint("The <Obj> disbelieves your <Obj>.",
              watcher, this);
        watcher->GainTempStati(DISBELIEVED, this, -2, SS_MISC);
        
        /* Later, add here "Hey everyone, the <Obj> is an illusion!"
           and have all the watcher's party members make a disbelief
           check as well. */
           
        SetStatiMag(ILLUSION,-1,NULL, GetStatiMag(ILLUSION)+1);
        /*
          Let's leave the "disbelief destroys illusions" out for
          now and see how balance works. 
        
        if (s->Mag > (fl & (IL_SPECTRAL) ? 0 :
              oCreature(s->h)->Mod(A_INT)+1))
          RemoveStati(ILLUSION);*/
      } 
    
 }
 
void Map::TerrainDisbelief(int16 x, int16 y, Creature * watcher)
  {
    int16 i; Status *s; uint32 saveFlags;
     
       
    if (!watcher)
      {
        MapIterate(this,watcher,i)
          if (watcher->isCreature())
            if (LineOfSight(watcher->x,watcher->y,x,y,watcher))
              TerrainDisbelief(x,y,watcher);
        return;
      }
    
    for (i=0;Fields[i];i++)
      if (Fields[i]->FType & FI_ITERRAIN)
        {
          s = oCreature(Fields[i]->Creator)->
                GetEffStati(EFF_FLAG1, Fields[i]->eID);

          saveFlags = SA_ILLUS | SA_MAGIC;
          if (s->eID) {
            if (TEFF(s->eID)->HasSource(AI_WIZARDRY) ||
                TEFF(s->eID)->HasSource(AI_SORCERY) ||
                TEFF(s->eID)->HasSource(AI_THEURGY) ||
                TEFF(s->eID)->HasSource(AI_BARDIC) ||
                TEFF(s->eID)->HasSource(AI_WITCHCRAFT) )
              saveFlags |= SA_SPELLS;
            if (TEFF(s->eID)->HasFlag(EF_EVIL))
              saveFlags |= SA_EVIL;
            if (TEFF(s->eID)->Schools & SC_NEC)
              saveFlags |= SA_NECRO;
            }
          if (watcher->HasStati(DISB_TERRAIN,-1,oCreature(Fields[i]->Creator)))
            return;  
          if (watcher->SavingThrow(WILL,s->Mag,SA_ILLUS))
            {
              watcher->IPrint("You realize that the <Res> is an illusion.", 
                                PTerrainAt(x,y,NULL));
              if (s->h && oThing(s->h)->isCreature())
                if (oCreature(s->h)->Percieves(watcher))
                  oCreature(s->h)->IPrint("The <Obj> disbelieves your <Res>.",
                    watcher, PTerrainAt(x,y,NULL));
              watcher->GainTempStati(DISB_TERRAIN, 
                oCreature(Fields[i]->Creator), -2, SS_MISC);
            
              /* Later, add here "Hey everyone, the <Obj> is an illusion!"
                 and have all the watcher's party members make a disbelief
                 check as well. */
            }  
        }  
   }

bool Thing::IllusionLOSCheck(bool test, int16 ix, int16 iy, int16 cx, int16 cy)
  {
    Creature *cr;
    if (!isIllusion())
      return false;
    if (getIllusionFlags() & IL_SPECTRAL)
      return false;
    if (ix == -1 || cx == -1)
      return false;
    cr = (Creature*) GetStatiObj(ILLUSION);
    ASSERT(cr->isCreature());
    if (ix == -1)
      { ix = x; iy = y; }
    if (cx == -1)
      { cx = cr->x; cy = cr->y; }
      
    if (!m->LineOfSight(cx,cy,ix,iy,cr))
      {
        if (!test)
          {
            cr->IPrint("You lose LOS with the <Obj>.",this);
            IDPrint(NULL,"The <Obj> winks out of existance.",this);
            Remove(true);
          }
        return true;
      }
    return false;
  }

int32 EncumValues[4];

uint8 Creature::Encumbrance()
  {
    uint16 Burden;
    int16 i, EStr;
    bool psych_might = HasAbility(CA_PSYCH_MIGHT);
    EStr = Attr[A_STR]; 
    double Mult = 1.0; 
    if (EStr == 0) EStr = 27; 

    /* ww: Wed Jan 28 21:26:36 PST 2004
     * I realize that this table is literally what the SRD says, but I
     * think that it's a bit low. Harper's Index 2003 gives the "pounds of
     * gear carried by a US special operations soldier in Iraq" as 135, 
     * which is presumably right on the border between a medium and a heavy
     * load in the d20 system (since the soldiers are still apparently able
     * to accomplish dex-based tasks like firing missile weapons). To make
     * that work here, every such soldier needs at least STR 15, which is a
     * little unreasonable for humans (do we really have that many people
     * with STR 15?). Let's assume that the weakest soldier has STR 12. 
     */
    EStr += 3; 

    while(EStr > 29)
      { EStr -= 10; Mult *= 4.0; }

    switch(Attr[A_SIZ])
      {
        case SZ_MINISCULE: Mult /= 8.00; break;
        case SZ_TINY:      Mult /= 4.00; break;
        case SZ_SMALL:     Mult *= 0.75; break;
        case SZ_MEDIUM:             break;
        case SZ_LARGE:     Mult *= 2.00; break;
        case SZ_HUGE:      Mult *= 4.00; break;
        case SZ_GARGANTUAN:Mult *= 8.00; break;
        case SZ_COLLOSAL:  Mult *= 16.00; break;
      }
    Burden=0;
    for(i=0;i!=SL_LAST;i++)
      // ww: support for floating disk
      if (InSlot(i) && !HasStati(IGNORE_SLOT_WEIGHT,i,NULL))
        Burden += InSlot(i)->Weight(psych_might);

        // don't count it twice just because we're using two hands ...
    if (InSlot(SL_WEAPON) && InSlot(SL_WEAPON) == InSlot(SL_READY))
        Burden -= InSlot(SL_WEAPON)->Weight(psych_might); 

    if (HasMFlag(M_NO_CARRY_LOADS)) {
      Burden *= 4; 
    } 

    EncumValues[3] = (int32) (EncumbranceTable[EStr*3 + 2] * Mult);
    EncumValues[2] = (int32) (EncumbranceTable[EStr*3 + 1] * Mult);
    EncumValues[1] = (int32) (EncumbranceTable[EStr*3] * Mult);
    EncumValues[0] = (int32) ((EncumbranceTable[EStr*3]/3)* Mult);
    
    if (Burden > (EncumbranceTable[EStr*3 + 2] * Mult))
      return EN_EXTREME;
    if (Burden > (EncumbranceTable[EStr*3 + 1] * Mult))
      return EN_HEAVY;
    if (Burden > (EncumbranceTable[EStr*3] * Mult))
      return EN_MODERATE;
    if (Burden > ((EncumbranceTable[EStr*3]/3)* Mult))
      return EN_LIGHT;

    return EN_NONE;
  }

int32 Creature::MaxPress()
  {
    int16 i, Mult, Div, EStr;
    EStr = Attr[A_STR]; 
    Mult = 2;

    if (EStr == 0) EStr = 30; 

    while(EStr > 29)
      { EStr -= 10; Mult *= 4; }
    Div = 1;
    switch(Attr[A_SIZ])
      {
        case SZ_MINISCULE: Div = 8; break;
        case SZ_TINY:      Div = 4; break;
        case SZ_SMALL:     Div = 2; break;
        case SZ_MEDIUM:             break;
        case SZ_LARGE:     Mult *= 2; break;
        case SZ_HUGE:      Mult *= 4; break;
        case SZ_GARGANTUAN:Mult *= 8; break;
        case SZ_COLLOSAL:  Mult *= 16; break;
      }

    return (EncumbranceTable[EStr*3 + 2] * Mult) / Div;
  }

rID Creature::Race_mID()
  {
    /* Generic race monster -- casting 'animate dead' on a
       kobold skirmisher should give you a kobold zombie, 
       not a kobold skirmisher zombie, and so forth. */
    if (isMType(MA_HUMAN))
      return FIND("human;temp");
    if (isMType(MA_ELF))
      return FIND("elf;temp");
    if (isMType(MA_DWARF))
      return FIND("dwarf;temp");
    if (isMType(MA_GNOME))
      return FIND("gnome;temp");
    if (isMType(MA_DROW))
      return FIND("drow;temp");
    if (isMType(MA_ORC))
      return FIND("orc;temp");
    if (isMType(MA_DROW))
      return FIND("drow;temp");
    if (isMType(MA_HALFLING))
      return FIND("halfling;temp");
    return mID;
  }

bool Creature::CanAddTemplate(rID tID)
{
    TTemplate *tt = TTEM(tID); 

    if (HasEffStati(TEMPLATE,tID))
      return false;

    if (tt->ForMType && !isMType(tt->ForMType))  
      return false; 

    if (tt->PEvent(EV_ISTARGET,this,tID) == -1)
      return false;

    return true; 
} 

void Creature::AddTemplate(rID tID)
  {
    TTemplate *tt; int16 i;
    TMonster *tm; 
    TAttack *at, *at2;
    ASSERT(tID);
    tt = TTEM(tID);

    if (!isPlayer() && !CanAddTemplate(tID)) {
      CanAddTemplate(tID);
      return;
      }

    if (tt->HasFlag(TMF_OBVIOUS))
      GainPermStati(TEMPLATE,NULL,SS_MISC,0,1,tID);
    else
      GainPermStati(TEMPLATE,NULL,SS_MISC,0,0,tID);
   
    /* Resolve conflicting attacks between template and base
       monster. */ 
    tm = TMON(tmID);
    for (i=0;i!=12;i++)
      if (tm->Attk[i].AType && tt->HasAttk(tm->Attk[i].AType))
        {
          at = &tm->Attk[i];
          at2 = tt->GetNewAttk(at->AType);
          /* If the template claws for 1d8 and the base monster
             claws for 2d6, use the 2d6 claws instead of the 1d8. */
          if (is_standard_attk(at2->AType) || is_postgrab_attk(at2->AType))  
            if (is_wepdmg(at2->DType))
              if ( (at->u.a.Dmg.Number*at->u.a.Dmg.Sides+at->u.a.Dmg.Bonus) >
                   (at2->u.a.Dmg.Number*at2->u.a.Dmg.Sides+at2->u.a.Dmg.Bonus) )
                { /* Suspend the template's attack */
                  GainPermStati(SUSPEND_ATTK,NULL,SS_MISC,-at->AType,0,tID);
                  continue; }
          /* In all other cases, suspend the base monster's attack */
          GainPermStati(SUSPEND_ATTK,NULL,SS_MISC,at->AType,0,tID);
        }
    
    // ww: previously templates could not actually grant Feats
    for (i=0;i<16;i++) {
      if (tt->NewFeats[i] && tt->NewFeats[i] >= FT_FIRST && 
                             tt->NewFeats[i] <= FT_LAST)
        GainPermStati(EXTRA_FEAT,NULL,SS_MISC,tt->NewFeats[i],1,tID);
      if (tt->NewFeats[i] && tt->NewFeats[i] >= 1 && 
                             tt->NewFeats[i] <= SK_LAST)
        GainPermStati(EXTRA_SKILL,NULL,SS_MISC,tt->NewFeats[i],1,tID);
      }
      
    SetImage();
    CalcValues();

    if (isMonster()) {
      if (HasMFlag(M_MAGE))
        if (InSlot(SL_ARMOR))
          InSlot(SL_ARMOR)->IFlags &= ~IF_WORN;     
      thism->Inventory();
      Timeout = 0; 
    } 
    if (theGame->InPlay() && m && x != -1)
      m->Update(x,y);
  }

int Creature::ListAttacks(TAttack * buf, int max)
{
  int i = 0, out = 0, db = 0;
  TMonster * tm;

  tm = TMON(mID);  // mID = polymorphed monster type

  bool hasSuspend = HasStati(SUSPEND_ATTK); 

  /* Base requirement -- you need hands and a humanoid form
     to do martial arts. A lizardman or kobold can benefit 
     from monk training; a wolf or beholder can't. Maybe a 
     feat to overcome this for monk/druids? Hmmm...*/
  if (HasAbility(CA_UNARMED_STRIKE) && !HasMFlag(M_NOHANDS)
                                    && HasMFlag(M_HUMANOID))
    {
      if (isPlayer() && ( thisp->Opt(OPT_MONK_MODE) == 0 ) )
        for (i=0; i<NUM_TMON_ATTK && tm->Attk[i].AType; i++) 
          {
            if (hasSuspend && HasStati(SUSPEND_ATTK,tm->Attk[i].AType))
              continue; 
            if (tm->Attk[i].AType == A_PUNC)
              continue;
            if (is_standard_attk(tm->Attk[i].AType))
              if (tm->Attk[i].DType == AD_SLASH ||
                  tm->Attk[i].DType == AD_PIERCE ||
                  tm->Attk[i].DType == AD_BLUNT)     
                {
                  db = ( AbilityLevel(CA_UNARMED_STRIKE) + 1 ) / 2;
                  goto UseNaturalAttacks;
                }
          }
      
      buf[0].AType = A_PUNC;
      buf[0].DType = AD_BLUNT;
      buf[0].u.a.Dmg = MonkDamage[ AbilityLevel(CA_UNARMED_STRIKE) ];
      buf[0].u.a.DC = 0;
      return 1;
    }

  UseNaturalAttacks:
  for (i=0; i<NUM_TMON_ATTK && tm->Attk[i].AType; i++) {
    if (hasSuspend && HasStati(SUSPEND_ATTK,tm->Attk[i].AType))
      continue; 
    buf[out] = tm->Attk[i];
    if (is_standard_attk(buf[out].AType))
      if (buf[out].DType == AD_SLASH ||
          buf[out].DType == AD_PIERCE ||
          buf[out].DType == AD_BLUNT)
        buf[out].u.a.Dmg.Bonus += db;   
    out++;
    if (out == max) return out; 
  } 
  if (!HasStati(POLYMORPH))
    {
      StatiIterNature(this,TEMPLATE)
        TTemplate * tt = TTEM(S->eID); 
        for (i=0;i<16 && tt->NewAttk[i].AType;i++) {
          if (hasSuspend && HasStati(SUSPEND_ATTK,tt->NewAttk[i].AType))
            continue; 
          buf[out++] = tt->NewAttk[i];
          if (out == max) StatiIterBreakout(this,return out) 
        }
      StatiIterEnd(this)
    }
  return out; 
} 

  
TAttack* Creature::GetAttk(int8 typ)
  {
    int16 i,j;
    if (!HasStati(POLYMORPH))
      {
        StatiIterNature(this,TEMPLATE)
              TTemplate *tt = TTEM(S->eID);
              for(j=0;j!=16;j++)
                if (tt->NewAttk[j].AType == typ)
                  StatiIterBreakout(this,return &(tt->NewAttk[j]))
        StatiIterEnd(this)
      }

    if (HasAbility(CA_SACRED_AURA) && typ == A_AURA) {
      static TAttack SacredAura;
      SacredAura.AType = A_AURA;
      SacredAura.DType = AD_HOLY;
      SacredAura.u.a.DC = 0;
      SacredAura.u.a.Dmg.Set(
        AbilityLevel(CA_SACRED_AURA),4,max(0,Mod(A_CHA)));
      //Fatal("ww: returning the address of a local stack variable");
      return &SacredAura;
      }
    

    /*
    if (is_ability_attk(typ))
      return TMON(tmID)->GetAttk(typ);
    else
    */
      return TMON(mID)->GetAttk(typ);
  }

bool Creature::HasMFlag(int16 fl)
  {
    static int16 i; 
    static TTemplate *tt;
    bool anyTemplates = false; 

    i = 0;
     
    if (fl == M_NEUTER)
      if (!isMType(MA_SAPIENT))
        return true;
    
    if (fl < 0)
      {
        StatiIterNature(this,TEMPLATE)
            tt = TTEM(S->eID);
            if (tt->HasFlag(fl))
              StatiIterBreakout(this,return true) 
        StatiIterEnd(this)
        return false; 
      } 

  StatiIterNature(this,TEMPLATE)
      tt = TTEM(S->eID);
      // first, do any templates remove it? 
      if (tt->SubsFlag(fl))
        StatiIterBreakout(this,return false)
  StatiIterEnd(this)

  StatiIterNature(this,TEMPLATE)
      tt = TTEM(S->eID);
      // do any templates add it? 
      if (tt->AddsFlag(fl))
        StatiIterBreakout(this,return true)
  StatiIterEnd(this)

  if (HasStati(POLYMORPH)) {
    /* WTF?
    if (fl == M_NOHANDS && AbilityLevel(CA_WILD_SHAPE))
      return false; 
    */
    return TMON(mID)->HasFlag(fl); 
  } else return TMON(tmID)->HasFlag(fl);
  }

bool Creature::HasFeat(int16 n, bool inh, bool list)
  {
    int16 i; bool b = false;
    bool hasPoly = false; 

    if (inh)
      return TMON(tmID)->HasFeat(n);

    if (HasStati(EXTRA_FEAT,n)) 
      return true; 

    StatiIterNature(this,TEMPLATE)
          TTemplate *tt = TTEM(S->eID);
          b |= tt->HasFeat(n);
    StatiIterEnd(this)

    if (HasStati(POLYMORPH))
        b |= TMON(mID)->HasFeat(n);
    else
        b |= TMON(tmID)->HasFeat(n);
    return b;
  }

    

bool Creature::HasAbility(int16 n, bool inh)
  {
    int16 i;

    if (n == CA_SPELLCASTING)
      return AbilityLevel(n) > 0;

    if (n == CA_SPECIES_AFFINITY)
      if (HasStati(SPECIES_AFFINITY))
        return true;

    StatiIterNature(this,EXTRA_ABILITY)
        if (inh && (S->Source != SS_RACE && S->Source != SS_CLAS &&
                    S->Source != SS_PERM && S->Source != SS_BLES &&
                    S->Source != SS_DOMA))
          continue;
        if (S->Val == n) {
          if (S->h)
            if (theRegistry->Exists(S->h) && oThing(S->h)->isItem())
              if (!oItem(S->h)->isKnown(KN_MAGIC))
                continue;
          StatiIterBreakout(this,return true)
        }
    StatiIterEnd(this)

    Status *s;
    if (n == CA_TURNING)
      if (s = GetStati(TURN_ABILITY))
        if ((!inh) || s->Source == SS_RACE || s->Source == SS_CLAS ||
                    s->Source == SS_PERM || s->Source == SS_BLES ||
                    s->Source == SS_DOMA)
          return true;
    if (n == CA_COMMAND)
      if (s = GetStati(COMMAND_ABILITY))
        if ((!inh) || s->Source == SS_RACE || s->Source == SS_CLAS ||
                    s->Source == SS_PERM || s->Source == SS_BLES ||
                    s->Source == SS_DOMA)
          return true;
    if (n == CA_GREATER_TURNING)
      if (s = GetStati(GTURN_ABILITY))
        if ((!inh) || s->Source == SS_RACE || s->Source == SS_CLAS ||
                    s->Source == SS_PERM || s->Source == SS_BLES ||
                    s->Source == SS_DOMA)
          return true;
    if (n == CA_SMITE)
      if (s = GetStati(SMITE_ABILITY))
        if ((!inh) || s->Source == SS_RACE || s->Source == SS_CLAS ||
                    s->Source == SS_PERM || s->Source == SS_BLES ||
                    s->Source == SS_DOMA)
          return true;
    if (n == CA_FAV_ENEMY)
      if (s = GetStati(FAV_ENEMY))
        if ((!inh) || s->Source == SS_RACE || s->Source == SS_CLAS ||
                    s->Source == SS_PERM || s->Source == SS_BLES ||
                    s->Source == SS_DOMA)
          return true;
    if (n == CA_SPECIES_AFFINITY)
      if (s = GetStati(SPECIES_AFFINITY))
        if ((!inh) || s->Source == SS_RACE || s->Source == SS_CLAS ||
                    s->Source == SS_PERM || s->Source == SS_BLES ||
                    s->Source == SS_DOMA)
          return true;

    if (isCharacter())
      if (thisp->Abilities[n])
        return true;
    return false;
  }

/* Skills are stored in the same array as feats, and the number
   index for the first feat is one higher then for the last skill.
   This simplifies things immensely, and lets monsters be either
   feat-heavy or skill-heavy without needing to allocate seperate
   space for each. */

int16 Creature::AbilityLevel(int16 n) 
  {
    int16 lv = 0, i;

    if (isCharacter()) {
      if (n == CA_TURNING)
        if (HasStati(TURN_ABILITY))
          lv += HighStatiMag(TURN_ABILITY);
      if (n == CA_COMMAND)
        if (HasStati(COMMAND_ABILITY))
          lv += HighStatiMag(COMMAND_ABILITY);
      if (n == CA_GREATER_TURNING)
        if (HasStati(GTURN_ABILITY))
          lv += HighStatiMag(GTURN_ABILITY);
      if (n == CA_SMITE)
        if (HasStati(SMITE_ABILITY))
          lv += HighStatiMag(SMITE_ABILITY);
      if (n == CA_FAV_ENEMY)
        if (HasStati(FAV_ENEMY))
          lv += HighStatiMag(FAV_ENEMY);
      if (n == CA_SPECIES_AFFINITY)
        if (HasStati(SPECIES_AFFINITY))
          lv += HighStatiMag(SPECIES_AFFINITY);

      lv += thisp->Abilities[n];
      
      
      /* If you want to tweak the numbers for Intensive
         study, make sure they match both here and in
         Character::getEligableStudies()! */
      if (n == CA_SPELLCASTING)
        lv += thisp->IntStudy[STUDY_CASTING];
      if (n == CA_SNEAK_ATTACK)
        lv = max(lv,min(lv + thisp->IntStudy[STUDY_SNEAK],
              min(lv*2,((thisp->TotalLevel()*4+4)/10))));
      if (n == CA_TURNING || n == CA_COMMAND)
        lv = max(lv,min(lv + thisp->IntStudy[STUDY_TURNING]*2,
              min(lv*2,((thisp->TotalLevel()*4+4)/5))));
      if (n == CA_WILD_SHAPE)
        lv = max(lv,min(lv + thisp->IntStudy[STUDY_SHAPES]*2,
              min(lv*2,((thisp->TotalLevel()*4+4)/5))));
      if (n == CA_SACRED_MOUNT)
        lv = max(lv,min(lv + thisp->IntStudy[STUDY_MOUNT]*3,
              min(lv*2,((thisp->TotalLevel()*4+4)/5))));
      if (n == CA_SMITE)
        lv = max(lv,min(lv + thisp->IntStudy[STUDY_SMITE]*4,
              min(lv*2,((thisp->TotalLevel()*4+4)/5))));
      if (n == CA_UNARMED_STRIKE || n == CA_STUN_ATTACK)
        lv = max(lv,min(lv + thisp->IntStudy[STUDY_UNARMED]*2,
              min(lv*2,((thisp->TotalLevel()*4+4)/5))));
      if (n == CA_LEGEND_LORE || n == CA_BARDIC_MUSIC || n == CA_STORYCRAFT)
        lv = max(lv,min(lv + thisp->IntStudy[STUDY_BARDIC]*2,
              min(lv*2,((thisp->TotalLevel()*4+4)/5))));
      
        if (HasStati(POLYMORPH))
          if (n == CA_SPELLCASTING ||
              n == CA_PSIONICS)
            return lv;
      }


    StatiIterNature(this,EXTRA_ABILITY)
        if (S->Val == n) {
          if (S->h)
            if (theRegistry->Exists(S->h) && 
                oThing(S->h)->isItem())
              if (!oItem(S->h)->isKnown(KN_MAGIC))
                continue; 
          lv += S->Mag; 
          }
    StatiIterEnd(this)
    
    if (n == CA_SPELLCASTING) {
      StatiIterNature(this,TEMPLATE)
        lv = TTEM(S->eID)->CasterLev.Adjust(lv);
      StatiIterEnd(this)
      }

    return max(0,lv);
  }


int16 Creature::Mod(int8 a)  { 
  int res = Attr[a] ? (Attr[a]-10)/2 : 0; 
  if (a == A_DEX && InSlot(SL_ARMOR) && InSlot(SL_ARMOR)->Type == T_ARMOR) 
    res = min(res, ((Armor *)InSlot(SL_ARMOR))->MaxDexBonus(this));
  return res;
} 
int16 Creature::Mod2(int8 a) { 
  int res = Attr[a] ? (Attr[a]-11)/2 : 0; 
  if (a == A_DEX && InSlot(SL_ARMOR) && InSlot(SL_ARMOR)->Type == T_ARMOR) 
    res = min(res, ((Armor *)InSlot(SL_ARMOR))->MaxDexBonus(this));
  return res;
}

int16 Creature::KMod(int8 a)  { return Mod(a); } 
int16 Creature::KMod2(int8 a)  { return Mod2(a); } 

int16 Character::KMod(int8 a)  { 
  int res = Attr[a] ? (KAttr[a]-10)/2 : 0; 
  if (a == A_DEX && InSlot(SL_ARMOR)) 
    res = min(res, ((Armor *)InSlot(SL_ARMOR))->MaxDexBonus(this));
  return res;
} 
int16 Character::KMod2(int8 a) { 
  int res = Attr[a] ? (KAttr[a]-11)/2 : 0; 
  if (a == A_DEX && InSlot(SL_ARMOR)) 
    res = min(res, ((Armor *)InSlot(SL_ARMOR))->MaxDexBonus(this));
  return res;
}

// ww: if I'm playing a summoner druid, I want to see Fluffy's rolls too
// ... helps me know if it's time to pull her out of combat, lets me see
// Magic Fang in action, whatever. 
Term * Creature::ShowCombatNumbers() {
  if (isPlayer()) 
    return thisp->MyTerm;
  Creature * druid = (Creature *)GetStatiObj(ANIMAL_COMPANION,TA_LEADER);
  if (druid && druid->isPlayer())
    return ((Player *)druid)->MyTerm;
  if (theGame->GetPlayer(0)->Opt(OPT_ALL_ROLLS))
    return theGame->GetPlayer(0)->MyTerm; 
  return false; 
} 

int32 Creature::Weight(bool includeEquipment)
  {
    int32 w = 1500, Burden = 0;
    switch (Attr[A_SIZ]) {
      case SZ_MINISCULE: w = 5; break;
      case SZ_TINY:      w = 50; break;
      case SZ_SMALL:     w = 500; break;
      case SZ_MEDIUM:    w = 1500; break;
      case SZ_LARGE:     w = 4000; break;
      case SZ_HUGE:      w = 10000; break;
      case SZ_GARGANTUAN:w = 250000; break;
      case SZ_COLLOSAL:  w = 1000000; break;
      }
    if (includeEquipment) {
      for(int i=0;i!=SL_LAST;i++)
        if (InSlot(i) && !HasStati(IGNORE_SLOT_WEIGHT,i,NULL))
          Burden += InSlot(i)->Weight();
    } 
    return (w + Burden);
  }

      /* ww: in the SRD, Charging gives a flat +2 to hit and lances deal
       * double damage in a charge.
       *
       * In "old incursion", the charge bonus to hit and damage is
       * determined by the number of squares you have already been running.
       * The maximum base charge bonus is 8, the best you get in common
       * play is about 6. Lances double the charge bonus. 
       *
       * In "real life", I'm not sure about the hit bonus, but the
       * additional damage from a charge is related to the additional
       * momentum (or kinetic energy, but let's use momentum) of the
       * attacker. Momentum = Mass * Velocity. A man charging you with
       * a spear at 10 mph is less fatal than a man charging you with
       * a spear at 20 mph. Presumably this is what Old Incursion's 
       * "build up the charge bonus over time" theory is trying to
       * simulate, but since your A_MOV is never consulted or modified,
       * it gets very weird around the edges: an extremely encumbered
       * halfling lumbering toward you for 10 rounds gets a bigger bonus
       * than someone on a warhorse who has been charging for two. 
       *
       * In Incursion, riding a massive dragon with a lance should do more
       * damage than riding a warpony and a great maul. The effects of a
       * fast, massive mount and the primacy of mounted weapons (no one
       * attacks from horseback with a dagger ...) are understated. 
       * Thus, new experimental charging code. 
       */
int32 Creature::ChargeBonus()
{
      int32 weight = Weight(true); /* Small: 500, Med: 1500, Large: 4000 */
      Creature * mount = (Creature *)GetStatiObj(MOUNTED);
      if (mount) {
        weight += mount->Weight(true);
      } 
      int32 bonus = (weight * (Attr[A_MOV]*5+100)) / 150000;

      // Finally, your lance itself can only transfer so much of this
      // energy before it snaps! 
      int32 wep_hardness = 
        InSlot(SL_WEAPON) ? InSlot(SL_WEAPON)->Hardness(AD_BLUNT) + 5 : bonus;

      /* From the SRD, we assume that a Medium Creature running at 200%
       * normal speed gets a +2 bonus. So the formula is:
       *
       *   Weight * (A_MOV*5 + 100) = Bonus * 150,000
       *
       * Or:
       *
       *   Bonus = Weight * (A_MOV*5 + 100) / 150,000
       */
      /* Examples:
       *   Charging Man:                                                +2
       *   Charging Hafling on Warpony: (500+1500)*(300)/150,000 =      +4
       *   Charging Goblin on Warg:     (500+1500)*(300)/150,000 =      +4
       *   Charging Ogre:                     4000*(240)/150,000 =      +6
       *   Charging Man on Warhorse:   (1500+4000)*(350)/150,000 =     +12
       *   Good Man Rider, Armor, Warhorse:   6500*(375)/150,000 =     +16
       *   Charging Man on Huge Dragon:(1500+10000)*(400)/150000 =     +30
       *
       * To me, this seems more realistic: it should be very difficult to
       * survive having a spear pushed by a quickly flying dragon go
       * through you. Now, if we were being ultra realistic, charing
       * weapons would break all the time -- medieval jousts broke lances
       * all the time (that was how you scored points, in fact, but they
       * were also using specifically-weakened weapons). Let's have the 
       * weapon take damage equal to the charge bonus. Then a mundane heavy
       * lance (hardness 10, 15 hp) will break after 8 warhorse charges. 
       * Get a magical lance if you want it (but not the thing it is
       * hitting) to survive massive impacts. 
       *
       * To keep a bit of the flavour of the old "ramp up the bonus"
       * method, we'll say that you get 1/2 of the bonus if you
       * just started charging. 
       */
      
      bonus = min(bonus,wep_hardness); 
      
      bonus = max(0,min(10, bonus / 2)-1);
      
      return ChargeBonusTable[bonus][min(29,GetStatiMag(CHARGING))];
}

bool Creature::canMoveThrough(EventInfo *e, int16 tx, int16 ty, 
    Thing * &blocked_by)
{
  bool Incor = false,
       Huge  = false;

  blocked_by = NULL; 

  if (Type == T_PLAYER && thisp->Opt(OPT_GHOSTWALK))
    goto SkipSolidityTest;

  if (onPlane() > PHASE_VORTEX)
    Incor = true;
  if (GetAttr(A_SIZ) > SZ_LARGE)
    Huge = true;

  if (m->SolidAt(tx,ty) && !Incor) {
    if (HasAbility(CA_EARTHMELD)) {
      int8 mat = TTER(m->TerrainAt(tx,ty))->Material;
      if (mat == MAT_GRANITE || 
          mat == MAT_GEMSTONE || 
          mat == MAT_MINERAL ||
          mat == MAT_MAGMA ||
          mat == MAT_QUARTZ)
        ; // good, you can move through it 
      else return false; 
    } else {
      blocked_by = m->FDoorAt(tx,ty);
      if (blocked_by && ((Door*)blocked_by)->DoorFlags & DF_SECRET)
        blocked_by = NULL;
      return false; 
    }
  }

  Thing * t; 

  for(t=oThing(m->At(tx,ty).Contents);t;t=oThing(t->Next))
    if (!t->isCreature()) {
      if (e && e->Event == EV_PUSH && t == e->EVictim)
        continue;
      if ( ((t->Flags & F_SOLID) && !Incor) || 
            (t->Flags & F_XSOLID) ) {
        blocked_by = t; return false;
      } 
    }

  for(t=m->FFeatureAt(tx,ty);t;t=m->NFeatureAt(tx,ty)) 
    if (Throw(EV_WALKON,this,t,t)==ABORT) {
      blocked_by = t; 
      return false;
    }

SkipSolidityTest:

  Creature * cr; 

  for (cr=m->FCreatureAt(tx,ty);cr;cr=m->NCreatureAt(tx,ty)) {
    if (cr->isIllusion() && !cr->isRealTo(this))
      ;
    else if (cr->HasStati(HIDING) && !Percieves(cr) && 
               cr->GetAttr(A_SIZ) < SZ_LARGE && GetAttr(A_SIZ) < SZ_LARGE)
      ;
    else if (HasStati(HIDING) && !cr->Percieves(this)  && 
               cr->GetAttr(A_SIZ) < SZ_LARGE && GetAttr(A_SIZ) < SZ_LARGE)
      ;
    else if (HasStati(ELEVATED) != cr->HasStati(ELEVATED)  && 
               cr->GetAttr(A_SIZ) < SZ_LARGE && GetAttr(A_SIZ) < SZ_LARGE)
      ;
    else if (!cr->isHostileTo(this)  && 
               cr->GetAttr(A_SIZ) < SZ_LARGE && GetAttr(A_SIZ) < SZ_LARGE)
      ;
    else if ( (e && e->isLoc && cr == e->ETarget) ||
         (cr == this) || /* Our Size Field */
         HasMFlag(M_SWARM) || cr->HasMFlag(M_SWARM) || 
         (cr->onPlane() != onPlane()) )
      ;
    else {
      blocked_by = cr;
      return false;
    } 
  }

  return true; 
}

EvReturn Creature::TryToDestroyThing(Thing *f)
  {
    if (isPlayer() && thisp->Opt(OPT_AUTOKNOCK)) {
      // do we know a 'knock' spell?
      for(int i=0;i!=theGame->LastSpell();i++)
        if (thisp->SpellRating(theGame->SpellID(i),0,true) != -1) {
          TEffect *te = TEFF(theGame->SpellID(i));
          if (te->Purpose & EP_PASSAGE) {
            EventInfo e; 
            e.Clear();
            e.EActor = this;
            e.eID = theGame->SpellID(i);
            if (te->PEvent(EV_ISTARGET,this,e.eID) == SHOULD_CAST_IT) {
              IPrint("You attempt to cast <Res>.",e.eID);
              if (thisp->Spells[i] & SP_INNATE) 
                return ReThrow(EV_INVOKE,e); 
              else 
                return ReThrow(EV_CAST,e); 
            }
          }
        } 
    } 
    /* I want to depricate the idea of using weapon attacks
       to break down/open doors and chests. There are several
       reasons for this: first, magic swords are already SO
       GOOD in any D&D-like setting, it seems cheesy to make
       them the optimal solution to locked doors as well.
       Visually, shooting arrows into a door makes NO sense
       in terms of getting it open, and hacking it with a
       sword still seems less effective then the classical
       bodycheck maneuver, in terms of applying force to a
       wall-like barrier. 
       Finally, this leads to some wierd situations like
       archers blowing all their ammo trying to get through
       doors, and some of it ending up on the other side.
       So now, weapon attacks against Features inflict 1/3
       damage, as described in Feature::Event, and here we
       default to kicking to break features */
    if (f->isType(T_DOOR))
      return ThrowVal(EV_SATTACK,A_KICK,this,f);     
    
    if ((AttackMode() == S_MELEE || AttackMode() == S_DUAL) &&
        isBeside(f))
      return ThrowVal(EV_WATTACK,A_SWNG,this,f);
    
    
    /*else if (AttackMode() == S_ARCHERY) {
      if (isCharacter()) {
        EventInfo e;
        e.Clear();
        e.isDir = true;
        e.EActor = this;
        e.ETarget = f; 
        e.EItem = InSlot(SL_WEAPON);
        for(int n=0;n!=SL_LAST;n++)
          if (InSlot(n) && InSlot(n)->isType(T_MISSILE))
            if (TITEM(InSlot(SL_WEAPON)->iID)->HasRes(InSlot(n)->iID,AN_FIRES))
              e.EItem2 = InSlot(n); 
        if (e.EItem2) {
          e.EDir = DirTo(f);
          return ReThrow(EV_RATTACK,e); 
        }
      }
    }*/ 
    else if (isBeside(f)) // brawl
      return ThrowVal(EV_NATTACK,0,this,f);
    return ABORT;
  } 

/* yields a number like 130% or 90% */
int32 Creature::MoveAttr(int from_x, int from_y)
{
  int32 Mov = Attr[A_MOV];
  int i = TTER(m->TerrainAt(from_x,from_y))->MoveMod;
  i = (i*5)+100;
  hObj from_c = m->At(from_x,from_y).Contents;
  if (from_c) { 
    Thing *t = oThing(from_c); 
    while (t) {
      if (t->isFeature()) {
        int this_i = (((Feature *)t)->MoveMod * 5) + 100;
        i = i * this_i / 100; 
      } 
      t = oThing(t->Next);
    }
  }
  
  if (TTER(m->TerrainAt(from_x,from_y))->HasFlag(TF_WATER) && !isAerial()) {
    int16 water_mod;
    water_mod = isMType(MA_AQUATIC) ? 100 : 70;
    if (HasSkill(SK_SWIM) && SkillLevel(SK_SWIM) > 10)
      water_mod += (SkillLevel(SK_SWIM)-10)*10;
    i = (i * water_mod) / 100;
    }
  
  if (TREG(m->RegionAt(from_x,from_y))->Size <= GetAttr(A_SIZ))
    i /= 2;
  
  if (i < 100)
    if (HasAbility(CA_WOODLAND_STRIDE) || onPlane() != PHASE_MATERIAL ||
        isAerial())
      i = 100;
  int32 bad_blind = 1 + ((isBlind() || m->At(x,y).Dark) && 
      !HasFeat(FT_BLIND_FIGHT) && !HasMFlag(M_BLIND));
  return ((((Mov*5)+100) * i) / 100);
} 

inline bool Creature::SavingThrow(int16 type, int16 DC, uint32 Subtype,
                                    int16 cmod, const char *cmod_desc)
{ 
  int16 Bonus, i;
  static const char *save_name[3] = { "Fortitude", "Reflex", "Will" };
  if (DC <= 0)
    return false;
  bool show = false; 

  if ((isPlayer() || theGame->GetPlayer(0)->XPercieves(this)))
    show = true; 

  int16 roll = Dice::Roll(1,20); 

  Bonus = Attr[A_SAV_FORT + type];

  String bStr ; 
  if (show) { 
    bStr = Format("%c%s Save:%c 1d20 (%d)",
        -AZURE,save_name[type],-7,roll);
    if (Bonus)
      bStr += Format(" %+d base",Bonus);
  }

  StatiIterNature(this,SAVE_BONUS)
      if (BIT(S->Val) & Subtype) {
        Bonus += S->Mag;        
        if (show) bStr += Format(" %+d (%s)",S->Mag,
            Lookup(SaveBonusNames, S->Val));
      }
  StatiIterEnd(this)
  
  if (Subtype & SA_REST)
    {
      Bonus += 4;
      bStr += " +4 rest";
    }
  
  if (Subtype & SA_KNOCKDOWN)
    if (HasSkill(SK_BALANCE))
      {
        int16 b;
        b = SkillLevel(SK_BALANCE) / 2;
        if (b > 0) {
          Bonus += b;        
          if (show) bStr += Format(" %+d Balance",b);
          }            
      }
  if (Subtype & SA_POISON)
    if (HasSkill(SK_POISON_USE))
      {
        int16 b;
        b = SkillLevel(SK_POISON_USE) / 4;
        if (b > 0) {
          Bonus += b;        
          if (show) bStr += Format(" %+d Poison Use",b);
          }            
      }  
  if (Subtype & SA_THEFT)
    if (HasSkill(SK_PICK_POCKET))
      {
        int16 b;
        b = SkillLevel(SK_PICK_POCKET) / 3;
        if (b > 0) {
          Bonus += b;        
          if (show) bStr += Format(" %+d Pick Pockets",b);
          }            
      }  
  if (Subtype & (SA_POISON|SA_DISEASE))
    if (HasFeat(FT_HARDINESS))
      {
        Bonus += 2;
        if (show)
          bStr += " +2 Hardiness";
      }
      
  if (cmod) {
    Bonus += cmod;
    bStr += Format(" %+d %s", cmod, cmod_desc);
    }
      
  if (type == REF && HasStati(STUCK)) {
    Bonus -= 7;
    if (show) bStr += " -7 (entangled)";
    // ww: if you can't move, you normally can't make a reflex save ...
    // but if by some miracle you can overcome a -30 penalty, more power
    // to you!
    } 
  for (i=ADJUST;i!=ADJUST_LAST+1;i++)
    {
      RemoveOnceStati(i,A_SAV);
      if (type == FORT)
        RemoveOnceStati(i,A_SAV_FORT);
      if (type == REF)
        RemoveOnceStati(i,A_SAV_REF);
      if (type == WILL)
        RemoveOnceStati(i,A_SAV_WILL);
    }
  if (show) {
    bStr += Format(" = %d vs DC %d %c[%s]%c.",
        Bonus + roll, DC, 
        (Bonus + roll >= DC) ? -EMERALD: -PINK,
        (Bonus + roll >= DC) ? "success" : "failure",
        -7);
    Term * term;
    if (isPlayer()) 
      term = thisp->MyTerm;
    else {
      term = theGame->GetPlayer(0)->MyTerm;
      bStr = this->Name(NA_CAPS|NA_POSS) + SC(" ") + bStr;
    }
      term->SetWin(WIN_NUMBERS);
      term->Clear();
      term->Write(0,0,bStr);
      if (theGame->Opt(OPT_STORE_ROLLS))
        term->AddMessage(bStr);
      /* why is this here?  
        term->Message(bStr);
      */
  }

  if (isCharacter() && (Bonus + roll >= DC) && DC >= 11)
    {
      int16 die, col, cap;
      col = EXXX_SAVE;
      die = (DC-10)*2;
      cap = 60;
      // things the player can scum
      if (Subtype & (SA_POISON|SA_DISEASE|SA_TRAPS|SA_CLOSE|SA_GRAB))
        cap = 30;
      // risk of instant death
      if (Subtype & (SA_DISINT|SA_DEATH))
        die *= 3;
      // made 3-4 times a relevant combat
      if (Subtype & SA_CLOSE)
        die /= 3;
      switch (type) {
        case FORT:
          Exercise(A_CON,Dice::Roll(1,die),col,cap);
         break;
        case REF:
          Exercise(A_DEX,Dice::Roll(1,die),col,cap);
         break;
        case WILL:
          Exercise(A_WIS,Dice::Roll(1,die),col,cap);
         break;
        }
    }
      

  return Bonus + roll >= DC;
}

bool Creature::HasInnateSpell(rID spID)
  {
    rID xID; TMonster *tm; int16 i, lim; TTemplate *tt;
    if (HasEffStati(INNATE_SPELL,spID))
      return true;
    lim = isMType(MA_DRAGON) ? GetAgeCatagory() : 100;
    tm = TMON(tmID);
    for(xID = tm->FirstRes(AN_INNATE),i=0;xID;xID = tm->NextRes(AN_INNATE))
      {
        if (xID == spID)
          return true;
        i++;
        if (i > lim)
          break;
      }
    StatiIterNature(this,TEMPLATE)
      tt = TTEM(S->eID);
      for(xID = tt->FirstRes(AN_INNATE),i=0;xID;xID = tt->NextRes(AN_INNATE))
        if (xID == spID)
          return true;
    StatiIterEnd(this)
    return false;
  }            
      

