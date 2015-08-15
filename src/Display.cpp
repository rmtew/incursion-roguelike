/* DISPLAY.CPP -- See the Incursion LICENSE file for copyright information.

     Includes all functions governing the placement of Things
   on a Map, and moving them about once they're placed. Also, 
   miscellaneous Map functions that do not relate to dungeon 
   generation, especially the code to handle animated Overlays.

     Thing::Thing(Glyph img, int8 Type)
     void Thing::PlaceAt(Map *m, int16 x, int16 y)
     int16 Thing::DistFromPlayer()
     int16 Thing::DistFrom(Thing *t)
     void Thing::PlaceNear(int16 x, int16 y)
     Map::Map()
     void Thing::NotifyGone(hObj h)
     void Player::NotifyGone(hObj h)
     void Map::NotifyGone(hObj h)
     Thing* Map::GetAt(int16 x,int16 y)
     bool Map::OpenAt(int16 x, int16 y)
     bool Map::besideWall(int16 x,int16 y)
     bool Creature::isBeside(Thing *t);
     void Map::RegisterPlayer(hObj h)
     void Map::VUpdate(int16 x, int16 y)
     void Map::Load(rID mID)

     void Overlay::Activate()
     void Overlay::DeActivate()
     void Overlay::AddGlyph(int16 x,int16 y, Glyph g)
     void Overlay::RemoveGlyph(int16 x,int16 y)
     bool Overlay::IsGlyphAt(int16 x, int16 y)
     void Overlay::RemoveGlyph(int16 n)
     void Overlay::ShowGlyphs()

     void Thing::DoTurn()
     void Thing::Move(int16 newx,int16 newy, bool is_walk)
     Thing* Thing::ProjectTo(int16 tx, int16 ty, int8 range)
     void Thing::Show()
     EvReturn Thing::Event(EventInfo &e)
     void Thing::Remove(bool isDelete)
     void Item::Remove(bool isDelete)

*/ 

#include "Incursion.h"

Thing::Thing(Glyph _Image,int16 _Type)
    : Object(_Type)
  {
    x=y=-1;
    Image=_Image;
    m=NULL; 
    Timeout=0;
    Flags=0;
    __Stati.Initialize(); 
  }

void Thing::PlaceAt(Map*_m,int16 _x,int16 _y, bool share_square)              
  {            
    bool isBig = false;
    Field MyFields[12]; 
    int16 i, fc; Creature *mount;

  const int ox = x;
  const int oy = y; 

  if (!_m) return;

    if (isCreature())
      {
        /* ASSUMPTION: Templates, magic, et al., won't cause creatures
           to grow more than one size category. This is wrong in a few
           cases (dire animal drinks Potion of Enlargement), but its
           good enough in general. */
        if (thisc->GetAttr(A_SIZ) == 0)
          if (TMON(thisc->mID)->Size >= SZ_LARGE)
            thisc->CalcValues();
        if (thisc->GetAttr(A_SIZ) >= SZ_HUGE)
          isBig = true;
      }

    /* Temporary assumption: mounts don't emit mobile fields.
       This is a very, very shaky assumption (dragons casting
       Spook, etc.) Fix soon. */

    if (HasStati(MOUNTED))
      mount = (Creature*)GetStatiObj(MOUNTED);
    else
      mount = NULL;

    if (isType(T_CREATURE) && thisc->isMType(MA_DRAGON))
      thisc->GetAgeCatagory();

    /* This is a patchup to address the "generic item" bug. */
    if (Type == T_POTION || Type == T_SCROLL || Type == T_RING || Type == T_WAND ||
        Type == T_TOME || Type == T_AMULET || Type == T_CLOAK || Type == T_BRACERS)
      if (!thisi->eID)
        {
          Remove(true);
          return;
        }

    if (m == _m)
      { 
        Move(_x,_y); 
        Monster::AlertNew(this);
        if (this->isCreature()) {
          ((Creature *)this)->ts.Retarget((Creature *)this);
        }
        if (isCreature())
          thisc->TerrainEffects();
        return; 
      }

    if (ThrowXY(EV_PLACE,_x,_y,this) == ABORT)
      { m = NULL; return; }
    if (isDead())
      return;

    fc = 0;
    if (isCreature() && m)
      for (i=0;m->Fields[i];i++)
        if (m->Fields[i]->Creator == myHandle || (mount && 
              m->Fields[i]->Creator == mount->myHandle))
          if (m->Fields[i]->FType & FI_MOBILE)
            {
              MyFields[fc++] = *(m->Fields[i]);
              m->RemoveField(m->Fields[i]);
              i--;
            }
    ASSERT(fc < sizeof(MyFields) / sizeof(MyFields[0]));

    Remove(false);
    m  = _m;

    if (ThrowXY(EV_PLACE,_x,_y,this) == ABORT)
      { m = NULL; return; }
    if (mount && ThrowXY(EV_PLACE,_x,_y,mount) == ABORT)
      { m = NULL; return; }
    if (isDead())
      return;
     
    if (isPlayer()) {
      if (m->Day != theGame->Day)
        m->DaysPassed();
      m->ResetImages();
      }



    ASSERT(m->InBounds(_x,_y));
    /* Patch to prevent crashes */
    if (!m->InBounds(_x,_y))
      _x = _y = 1;
    
    if (isCreature() && ((m->FCreatureAt(_x,_y) && !share_square) || 
          m->SolidAt(_x,_y) || isBig || m->FieldAt(_x,_y,FI_SIZE)))
        x = y = 1;
      else {
        x = _x;
        y = _y;
        }
    ASSERT(!oThing(myHandle)->isType(T_MAP));
    int where = m->Things.Add(myHandle);

    if (m->At(x,y).Contents)
      if (oThing(m->At(x,y).Contents)->isCreature())
        {
          Next = oThing(m->At(x,y).Contents)->Next;
          oThing(m->At(x,y).Contents)->Next = myHandle;
          goto DoneContentsAdd;
        }

    Next = m->At(x,y).Contents;
    m->At(x,y).Contents = myHandle;
    DoneContentsAdd:

    if (isPlayer()) {
      m->RegisterPlayer(myHandle);
      thisp->MyTerm->SetMap(m);
			thisp->MyTerm->AdjustMap(_x,_y,true);
      thisp->UpdateMap = true;
      }
    if (isFeature())
      ;
    else if (_x != x || y != _y)
      PlaceNear(_x,_y);
    if (!m)
      return;

    Monster::AlertNew(this);
    if (this->isCreature())
      ((Creature *)this)->ts.Retarget((Creature *)this);

    if (m && m->At(x,y).hasField)
      for(i=0;m && m->Fields[i];i++)
        if (m->Fields[i]->inArea(x,y)) {
          ThrowField(EV_FIELDON,m->Fields[i],this);
          if (isDead())
            return;
          if (mount)
            ThrowField(EV_FIELDON,m->Fields[i],mount);
          }

    if (isCreature() && m) {
      for(i=0;i!=fc;i++)
        m->NewField(MyFields[i].FType,x,y,MyFields[i].rad,MyFields[i].Image,
          MyFields[i].Dur, MyFields[i].eID,(Creature*)this);
      if (thisc->HasAbility(CA_AURA_OF_VALOUR)) {
        rID vID = FIND("Aura of Valour");
        ASSERT(vID);
        for(i=0;m->Fields[i];i++)
          if (m->Fields[i]->Creator == myHandle)
            if (m->Fields[i]->eID == vID)
              goto ValourExists;
        m->NewField(FI_MOBILE|FI_MODIFIER,x,y,3,GLYPH_VALUE(GLYPH_FLOOR2, WHITE),-1,vID,thisc);
        ValourExists:;
        }
      if (m == NULL)
        return;
      thisc->TerrainEffects();
      if (mount)
        mount->TerrainEffects();
      if (isDead())
        return;

    }

    if (!HasStati(HOME_REGION) && !isPlayer()) {
      if (m->InBounds(x,y)) {
        rID rid = m->RegionAt(x,y);
        if (rid) {
          GainPermStati(HOME_REGION,NULL,SS_MISC,x+y*256,0,rid);
          if (isType(T_CHEST)) {
            Item *it = oItem(((Container*)this)->Contents);
            while (it) {
              if (!it->HasStati(HOME_REGION))
                it->GainPermStati(HOME_REGION,NULL,SS_MISC,x+y*256,0,rid);
              it = oItem(it->Next);    
              }
            }
          } 
        }
      }

      
    /* PlaceNear calls Move, which calls AlertNew */
    if (isType(T_DOOR))
      ((Door*)this)->SetImage();

    if (theGame->InPlay() && !isDead())
      m->Update(x,y);
    
  }                              


/*inline*/ int16 Thing::DistFromPlayer()
 	{
    int16 dx,dy;
    
    dx = abs(x - oPlayer(m->pl[0])->GetX());
    dy = abs(y - oPlayer(m->pl[0])->GetY());
    return x==-1?0:(dx > dy ? dx+dy/2 : dy+dx/2);
  }

/*inline*/ int16 Thing::DistFrom(Thing*t)
 	{
    int16 dx,dy;
    dx = abs(x - t->GetX());
    dy = abs(y - t->GetY());
    return x==-1?0:(dx > dy ? dx+dy/2 : dy+dx/2);
  }

void Thing::PlaceNear(int16 x,int16 y)
  {
    static Creature* Displace[64], *cr; 
    bool isBig = false;
    uint8 i, ix, iy, sz, dc = 0;
    int16 r, tx, ty, c, cx[120], cy[120];
    
    ASSERT(m); 

    if (isCreature())
      {
        /* ASSUMPTION: Templates, magic, et al., won't cause creatures
           to grow more than one size category. This is wrong in a few
           cases (dire animal drinks Potion of Enlargement), but its
           good enough in general. */
        if (thisc->GetAttr(A_SIZ) == 0)
          if (TMON(thisc->mID)->Size >= SZ_LARGE)
            thisc->CalcValues();
        if (thisc->GetAttr(A_SIZ) >= SZ_HUGE)
          { isBig = true;
            sz = (uint8)thisc->GetAttr(A_SIZ); }
      }

    bool isRetry = false;
    
    TryAgain:

    for (r=0;r<=(isPlayer() ? 40 : 6);r++) {
      c = 0;
      for (tx=(x-r);tx<=(x+r);tx++)
        for (ty=(y-r);ty<=(y+r);ty++)
          if (abs(x-tx)==r || abs(y-ty)==r)
            {
              PurgeStrings();
              if (!m->InBounds(tx,ty))
                continue;
              if (m->SolidAt(tx,ty))
                  continue;
              if (m->FallAt(tx,ty) && !(isCreature() && thisc->isAerial()))
                continue;
              if (TTER(m->TerrainAt(tx,ty))->HasFlag(TF_WARN)  && 
                  ((!isCreature()) ||
                  TTER(m->TerrainAt(tx,ty))->PEvent(EV_MON_CONSIDER,this,
                  m->TerrainAt(tx,ty)) == ABORT))
                continue;
              if (m->FCreatureAt(tx,ty))
                if (m->FCreatureAt(tx,ty) != this)
                  if (!isRetry || m->FCreatureAt(tx,ty)->GetAttr(A_SIZ) > SZ_LARGE ||
                        isBig)
                    continue;
              if (isBig) {
                dc = 0;
                for (ix = tx - FaceRadius[sz]; ix <= tx + FaceRadius[sz]; ix++)
                  for (iy = ty - FaceRadius[sz]; iy <= ty + FaceRadius[sz]; iy++)
                    if (m->InBounds(ix,iy))  
                      if (dist(ix,iy,tx,ty) <= FaceRadius[sz])
                        {                      
                          /* Can't place here if bulk intersects walls */
                          if (m->SolidAt(ix,iy))
                            goto NotAnOption;
                          /* Can't place here if bulk intersects another size field */
                          /*
                          if (m->FieldAt(ix,iy,FI_SIZE))
                            if (m->FCreatureAt(ix,iy) != this)
                              goto NotAnOption;
                          */
                          for (cr = m->FCreatureAt(ix,iy);cr;cr=m->NCreatureAt(ix,iy))
                            if (cr->GetAttr(A_SIZ) >= SZ_HUGE)
                              if (cr != this)
                                goto NotAnOption;                   
                        }
                }
              /* This spot would be OK */
              cx[c] = tx;
              cy[c] = ty;
              c++;           
              NotAnOption:
              ;
            }
            
        
            
        if (c)
          {
            
            c = random(c);
            tx = cx[c];
            ty = cy[c];
            dc = 0;
            if (isBig)
              for (ix = tx - FaceRadius[sz]; ix <= tx + FaceRadius[sz]; ix++)
                for (iy = ty - FaceRadius[sz]; iy <= ty + FaceRadius[sz]; iy++)
                  if (m->InBounds(ix,iy))  
                    if (dist(ix,iy,tx,ty) <= FaceRadius[sz])
                      for (cr = m->FCreatureAt(ix,iy);cr;cr=m->NCreatureAt(ix,iy))
                        {
                          if (cr == this)
                            continue;
                          ASSERT(cr->GetAttr(A_SIZ) < SZ_HUGE);
                          Displace[dc++] = cr;
                        }
            goto FoundGoodPlace;
          } 
      }
      
    if (!isRetry) { 
        isRetry = true; 
        goto TryAgain;
    }
      
    /* So we've finished the loop and have not found any good
    place to put ourselves. */
    if (isPlayer()) {
        IPrint("[ Deleting obstacles. ]");
        while (m->FirstAt(x,y))
            m->FirstAt(x,y)->Remove(true);
        Move(x,y);
    } else {
        /*
        if (theGame->GetPlayer(0)->WizardMode)
        theGame->GetPlayer(0)->IPrint("You hear a disturbing *crunch*. (<Str> "
        "deleted due to lack of open space to place it.)",(const char *)Name(0));        
        */
        Remove(true);
    }
    return;
      
FoundGoodPlace:
    if (isBig)
        for (i=0;i!=dc;i++)
            Displace[i]->Remove(false);
    Move(tx,ty);
    
    // ww: this can cause a stack overflow when PlaceAt calls
    // PlaceNear calls PlaceAt -> perhaps two big creatures
    // ping-ponging each other? 
    if (isBig && dc) {
        for (i=0;i!=dc;i++) {
            ASSERT(Displace[i]->GetAttr(A_SIZ) < SZ_HUGE);
            Displace[i]->PlaceAt(m,tx,ty);
        }
    }
    
    {
        Thing *il; int16 i;
        for(i=0;backRefs[i];i++)
            if (theRegistry->Exists(backRefs[i]))
                if ((il = oThing(backRefs[i]))->isIllusion())
                    if (il->GetStatiObj(ILLUSION) == this)
                        il->IllusionLOSCheck(false);
    }
}

Map::Map() : Object(T_MAP), ov(myHandle)
	{
          nextAvailableTerraKey = 0; 
	}

void Thing::NotifyGone(hObj h)
  {
    RemoveStatiFrom(oThing(h));
  }

void Player::NotifyGone(hObj h)
  {
    RemoveStatiFrom(oThing(h));
    MyTerm->NotifyGone(h);
    if (Inv[0] == h)
      Inv[0] = 0;
  }

/* rmt: I think this is to deal with the fallout of a bug when remove(isDelete=true) was called
   and only mobile fields were removed.  That made sense for remove(isDelete=false) of course.
   TODO: Work out if this ever serves a purpose anymore.  It was buggy anyhow, as the creator
   no longer existed which meant that the event could not go out as there was no way to set the
   EActor value to the originating creator object. */
void Map::NotifyGone(hObj h)
  { 
    int16 i;
    StartSearch:
    for(i=0;Fields[i];i++)
      if (Fields[i]->Creator == h)
        { RemoveField(Fields[i]); goto StartSearch; }
  }

#if 0

bool Map::PileAt(int16 x, int16 y)
  {
    Thing *t = oThing(At(x,y).Contents); 
    int16 ic = 0;
    while (t) {
      if (t->isItem())
        ic++;
      if (ic >= 2)
        return true;
      t = oThing(t->Next);
      }
    return false;
  }

bool Map::MultiAt(int16 x, int16 y)
  {
    Thing *t = oThing(At(x,y).Contents); 
    int16 cc = 0;
    while (t) {
      if (t->isCreature())
        cc++;
      if (cc >= 2)
        return true;
      t = oThing(t->Next);
      }
    if (FieldAt(x,y,FI_SIZE))
      if (cc)
        return true;
    return false;
  }


Container* Map::ChestAt(int16 x, int16 y)
  {
    Thing *t = oThing(At(x,y).Contents); 
    int16 ic = 0;
    while (t) {
      if (t->isType(T_CHEST))
        return (Container*)t;
      t = oThing(t->Next);
      }
    return false;
  }

Trap* Map::TrapAt(int16 x, int16 y)
  {
    Thing *t = oThing(At(x,y).Contents); 
    int16 ic = 0;
    while (t) {
      if (t->isType(T_TRAP))
        return (Trap*)t;
      t = oThing(t->Next);
      }
    return false;
  }


Item* Map::ItemAt(int16 x,int16 y)
	{
		Thing *t = oThing(At(x,y).Contents);
    while (t) {
      if (t->isItem())
        return (Item*)t;
      t = oThing(t->Next);
      }
    return NULL;
	}

Feature* Map::FeatureAt(int16 x,int16 y)
	{
		Thing *t = oThing(At(x,y).Contents);
    while (t) {
      if (t->isFeature())
        return (Feature*)t;
      t = oThing(t->Next);
      }
    return NULL;
	}

Door* Map::DoorAt(int16 x,int16 y)
	{
		Thing *t = oThing(At(x,y).Contents);
    while (t) {
      if (t->isType(T_DOOR))
        return (Door*)t;
      t = oThing(t->Next);
      }
    return NULL;
	}


Creature* Map::CreatureAt(int16 x,int16 y)
	{
  if (!InBounds(x,y)) return NULL;
		int16 i;
		Thing *t;

    if (FieldAt(x,y,FI_SIZE)) 
      {
        for(i=0;Fields[i];i++)
          if (Fields[i]->inArea(x,y))
            if (Fields[i]->FType & FI_SIZE)
              return oCreature(Fields[i]->Creator);
        Error("Corrupted data for FI_SIZE field!");
      }
        
    t = oThing(At(x,y).Contents);
    while (t) {
      if (t->isCreature())
        return (Creature*)t;
      t = oThing(t->Next);
      }
    return NULL;
	}


#endif

bool Map::PMultiAt(int16 x, int16 y, Creature *POV)
  {
    Thing *t = oThing(At(x,y).Contents); 
    int16 cc = 0;
    while (t) {
      if (t->isCreature() && POV->Perceives(t))
        cc++;
      if (cc >= 2)
        return true;
      t = oThing(t->Next);
      }
    if (FieldAt(x,y,FI_SIZE))
      if (cc)
        return true;
    return false;
  }


Feature* Map::KnownFeatureAt(int16 x,int16 y)
{
  Thing *t = oThing(At(x,y).Contents);
  while (t) {
    if (t->isFeature()) {
      if (t->Type == T_TRAP && ! (((Trap *)t)->TrapFlags & TS_FOUND))
        ;
      else if (t->Type == T_DOOR && (((Door *)t)->DoorFlags & DF_SECRET))
        ;
      else return (Feature*)t;
    }
    t = oThing(t->Next);
  }
  return NULL;
}

	
Thing* Map::GetAt(int16 x, int16 y, int16 t, bool first)
  {
    static bool doneflag = false;
    static Thing* curr = NULL;
    
    static int32 count = 0;
    
    //count++;
    //if (count > 1000000L)
    //  __asm int 3;
    
    if (first == true) 
      {
        doneflag = false;
        curr = oThing(At(x,y).Contents);
        if (curr)
          while (curr && !curr->isType(t))
            curr = oThing(curr->Next);
        if (curr == NULL) 
          goto SizeFieldCheck;
        return curr;
      }
      
    if (doneflag || curr == NULL)
      return NULL;
      
    
      
    do
      {
        curr = oThing(curr->Next);
      }
    while (curr && !curr->isType(t));
    
    if (curr)
      return curr;

    /* Assumption: only one size field possible in any one
       square of the map. */
    SizeFieldCheck:
    /* Avoid returning a creature with a size field twice when
        asking what is in its center square. */
    if (oThing(At(x,y).Contents) &&
        oThing(At(x,y).Contents)->isCreature() &&
        oCreature(At(x,y).Contents)->GetAttr(A_SIZ) > SZ_LARGE)
      return NULL;
    if ((t == T_THING || t == T_CREATURE || t == T_CHARACTER ||
         t == T_MONSTER || t == T_PLAYER) && FieldAt(x,y,FI_SIZE)) 
      {
        for(int16 i=0;Fields[i];i++)
          if (Fields[i]->inArea(x,y))
            if (Fields[i]->FType & FI_SIZE)
              {
                doneflag = true;
                curr = NULL;
                return oCreature(Fields[i]->Creator);
              }
        Error("Corrupted data for FI_SIZE field!");
      }
      
    return NULL;
  }


bool Map::OpenAt(int16 x, int16 y)
  {
    return !(SolidAt(x,y) || FCreatureAt(x,y));
  }

bool Map::FallAt(int16 x, int16 y)
  {
    return TTER(TerrainAt(x,y))->HasFlag(TF_FALL);
  }

bool Map::besideWall(int16 x,int16 y)
  {
    /* ww: someone at (0,122) will do an out-of-bounds At() check
     * unless we put this here -- yes, this happened to me! */
    return (x == 0 || y == 0 || x >= sizeX-1 || y >= sizeY-1) ||
           At(x,y-1).isWall || At(x,y+1).isWall ||
           At(x-1,y).isWall || At(x+1,y).isWall ||
           At(x-1,y-1).isWall || At(x+1,y+1).isWall ||
           At(x+1,y-1).isWall || At(x-1,y+1).isWall;
  }

bool Creature::isBeside(Thing *t, int extra_dist)
  { 
    int16 k;
    k = extra_dist + 1 + FaceRadius[Attr[A_SIZ]];
    if (t->isItem() && ((Item*)t)->Owner() == this)
      return true;
    if (m != t->m)
      return false;
    if (t->isCreature())
      k += FaceRadius[((Creature*)t)->Attr[A_SIZ]];
    // if (k <= 1) 
      return (abs(x - t->GetX()) <= k) &&
                      (abs(y - t->GetY()) <= k); 
    // else return dist(x,y,t->x,t->y) <= k;
  
  }


void Map::RegisterPlayer(hObj h)
	{
  	if(PlayerCount==4)
    	Fatal("Too Many Players!");
    for(int8 i=0;i!=4;i++)
      if (pl[i] == h)
        return;
  	pl[PlayerCount]=h;
    PlayerCount++;
  }

void Map::VUpdate(int16 x, int16 y)
 {
   
   for(int i=0;i!=PlayerCount;i++)
     if (abs(x-oPlayer(pl[i])->x)<60 && abs(y-oPlayer(pl[i])->y)<40) 
       oPlayer(pl[i])->UpdateMap = true;
   Update(x,y);
 }

void Map::Load(rID mID)
  {
    Rect r;
    dID = mID;
    sizeX = TREG(mID)->sx;
    sizeY = TREG(mID)->sy;
    Grid  = new LocationInfo[sizeX * sizeY];
    r.Set(0,0,sizeX-1,sizeY-1);
    WriteMap(r,mID);

  }


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void Overlay::Activate()
	{
		int16 i;
		for(i=0;i!=MAX_OVERLAY_GLYPHS;i++)
			GlyphX[i]=GlyphY[i]=-1;
		GlyphCount=0;
		Active=true;
	}
void Overlay::DeActivate()
	{
		GlyphCount=0;
  	Active=false;
    for(int i=0;i!=MAX_OVERLAY_GLYPHS;i++)
    	if(GlyphX[i]!=-1)
      	oMap(m)->Update(GlyphX[i],GlyphY[i]);
  }

void Overlay::AddGlyph(int16 x,int16 y, Glyph g)
	{
    if (GlyphCount >= MAX_OVERLAY_GLYPHS)
      return;
		GlyphX[GlyphCount]=x;
		GlyphY[GlyphCount]=y;
		GlyphImage[GlyphCount]=g;
		GlyphCount++;
	}
void Overlay::RemoveGlyph(int16 x,int16 y)
	{
		for(int16 i=0;i!=MAX_OVERLAY_GLYPHS;i++)
			if(GlyphX[i]==x && GlyphY[i]==y)
				GlyphX[i]=GlyphY[i]=-1;

	}

bool Overlay::IsGlyphAt(int16 x, int16 y)
  {
		for(int16 i=0;i!=MAX_OVERLAY_GLYPHS;i++)
			if(GlyphX[i]==x && GlyphY[i]==y)
        return true;
    return false;
  }

void Overlay::RemoveGlyph(int16 n)
	{
		GlyphX[n]=GlyphY[n]=0;
	}
void Overlay::ShowGlyphs()
	{
		for(int16 i=0;i!=GlyphCount;i++)
      if (GlyphX[i] != -1)
			  oMap(m)->Update(GlyphX[i],GlyphY[i]);
    T1->Update();
	}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void Thing::DoTurn()
	{
	}
	
void Thing::Move(int16 newx,int16 newy, bool is_walk)
	{
		int16 ox,oy,i; Thing *th; Creature *mount;
		ox=x; oy=y; 
    Map *M = m;

    if (!m)
      return;
      
    if (HasStati(ENGULFED))
      return;

    if (HasStati(MOUNTED))
      mount = (Creature*)GetStatiObj(MOUNTED);
    else
      mount = NULL;

    if (x == -1 || HasStati(MOUNT))
      goto AddToList;


    if (M->At(x,y).hasField || M->At(newx,newy).hasField || !is_walk)
      {
        for(i=0;M->Fields[i];i++)
          if (M->Fields[i]->Creator == myHandle || (mount && 
                M->Fields[i]->Creator == mount->myHandle))
            if (M->Fields[i]->FType & FI_MOBILE)
              {
                if (M->MoveField(M->Fields[i],newx,newy,is_walk) == ABORT)
                  return;
              }
        for(i=0;M->Fields[i];i++)
          {
            if (M->Fields[i]->FType && FI_MOBILE)
              if (M->Fields[i]->Creator == myHandle)
                continue;
            if (M->Fields[i] && M->Fields[i]->inArea(x,y) && !M->Fields[i]->inArea(newx,newy))
              {
                ThrowField(EV_FIELDOFF,M->Fields[i],this);
                if (mount && M->Fields[i])
                  ThrowField(EV_FIELDOFF,M->Fields[i],mount);
              }
            if (M->Fields[i] && (!M->Fields[i]->inArea(x,y)) && M->Fields[i]->inArea(newx,newy))
              {
                ThrowField(EV_FIELDON,M->Fields[i],this);
                if (mount && M->Fields[i])
                  ThrowField(EV_FIELDON,M->Fields[i],mount);
              }
          }
      }

    /* If moving a field caused our death, don't put us back on the map! */
    if (isDead() || !m)
      goto DoneContentsAdd;

		x=newx; y=newy;
    /* Remove this Thing from the old Contents list */
    if (M->At(ox,oy).Contents == myHandle)
      M->At(ox,oy).Contents = Next;
    else
      {
        th = oThing(M->At(ox,oy).Contents);
        while(th && th->Next != myHandle) {
          if (!th->Next)
            Fatal("Contents list wierdless in Thing::Move!");
          th = oThing(th->Next);
          }
    if (th) 
        th->Next = Next;
      }
    /* ... and add it to the new one. */
    AddToList:
		x=newx; y=newy;
    if (M->At(x,y).Contents)
      if (oThing(M->At(x,y).Contents)->isCreature())
        {
          Next = oThing(M->At(x,y).Contents)->Next;
          oThing(M->At(x,y).Contents)->Next = myHandle;
          goto DoneContentsAdd;
        }
    Next = M->At(x,y).Contents;
    M->At(x,y).Contents = myHandle;
    DoneContentsAdd:

    if (mount) {
      mount->m = M;
      mount->x = x;
      mount->y = y;
      }

    if (theGame->InPlay()) {
      M->Update(ox,oy);
		  M->Update(x,y);
      }

  /* When we move, any engulfed creatures move with us. */
  if (HasStati(ENGULFER))
    {
      StatiIterNature(this,ENGULFER)
        Thing *t = oThing(S->h);
        t->m = M;
        t->x = x;
        t->y = y;
      StatiIterEnd(this)
    }

  if (Type==T_PLAYER && theGame->InPlay()) 
    thisp->UpdateMap = true;

}

Thing* Thing::ProjectTo(int16 tx, int16 ty, int8 range)
  {
    int8 dirX = (tx >= x) ? 1 : -1,
         dirY = (ty >= y) ? 1 : -1;
    Feature *f;
    Fraction slope, test; int16 sx,sy,ix,iy;
    ASSERT(m)
    bool stop = false;
    if (oPlayer(m->pl[0])) {
      Player * p = oPlayer(m->pl[0]);
      if (m->LineOfSight(p->x,p->y,tx,ty,p))
        stop = true; 
    } 
    if (x == tx)
      for (;;) {
        if (m->At(x,y+dirY).Solid)
          goto HitWall;
        for (f=m->FFeatureAt(x,y+dirY);f;f=m->NFeatureAt(x,y+dirY))
          if (f->Flags & F_SOLID)
            goto HitFeature;
        /*if (m->FieldAt(x,y+dirY,FI_MISSLE)) {
          if (Throw(EV_FIELDON,this) == ABORT)
        */
        if (!m->InBounds(x,y+dirY))
          goto HitWall;
        Move(x,y+dirY);
        if (stop) { 
          T1->StopWatch(5);
          T1->Update();
        } 
        if (m->FCreatureAt(x,y+dirY))
          goto HitCreature;

      }

    sx = x*2; sy = y * 2; tx *= 2; ty *= 2;
    slope.Set(abs(ty - y), abs((tx - dirX) - x));

    for(ix=sx,iy=sy;;) {

      test.Set(abs((iy+dirY)-sy),abs((ix+dirX)-sx));

      if (test > slope)
        ix += 2 * dirX;
      else if (test < slope)
        iy += 2 * dirY;
      else {
        ix += 2 * dirX;
        iy += 2 * dirY;
        }

      if (m->At(ix/2,iy/2).Solid)
        goto HitWall;
      for (f=m->FFeatureAt(ix/2,iy/2);f;f=m->NFeatureAt(ix/2,iy/2))
        if (f->Flags & F_SOLID)
          goto HitFeature;

      if (!m->InBounds(ix/2,iy/2))
        goto HitWall;

      Move(ix/2,iy/2);
      if (stop) { 
        T1->StopWatch(5);
        T1->Update();
      } 
      if (m->FCreatureAt(ix/2,iy/2))
        goto HitCreature;

      }

    HitWall:

      return NULL;

    HitFeature:
      return NULL;

    HitCreature:
      Creature *cr; int16 i;
      for (cr=m->FCreatureAt(x,y),i=0;cr;cr=m->NCreatureAt(x,y))
        Candidates[i] = cr->myHandle;
      return oThing((hObj)Candidates[random(i)]);


  }

void Thing::Show() {
    m->Update(x,y);
}

EvReturn Thing::Event(EventInfo &e) {
    switch(e.Event) {
    case EV_TURN:
        return DONE;
    case EV_PLACE:
        return DONE;
    case PRE(EV_FIELDON):
    case PRE(EV_FIELDOFF):
        /* Catch this case _before_ the field effect resource is
        allowed to override it, avoiding messages like "the
        stairs seem unsettled" from spook. */
        if (e.EField)
            if (e.EField->eID) {
                /* Polymorph / selective target paranoia */
                if (e.Event == PRE(EV_FIELDOFF) && HasEffStati(-1,e.EField->eID))
                    return NOTHING;
                /* If the thing entering the field isn't affected
                by it, abort now. */
                EventInfo e2;
                e2.Clear();

                if (theRegistry->Exists(e.EField->Creator))
                    e2.EActor = oCreature(e.EField->Creator);
                else
                    return NOTHING; 
                e2.ETarget = this;
                e2.eID = e.EField->eID;
                e2.EMagic = TEFF(e.EField->eID)->Vals(0);
                /* Make sure *every segment* ignores this; c.f. MCvsE */
                for (e2.efNum=0;e2.EMagic = TEFF(e2.eID)->Vals(e2.efNum);e2.efNum++)
                    if (thisp->isTarget(e2,this))
                        return NOTHING;
                return ABORT;
            }
            return NOTHING;
    case EV_FIELDON:
        if (e.EActor==this)
            return FieldOn(e);
        break;
    case EV_FIELDOFF:
        if (e.EActor==this)
            return FieldOff(e);
        break;
    default:
        return NOTHING;
    }
    return ERROR;
}

void Thing::Remove(bool isDelete) {
    int16 ox = x, oy = y,i; Thing *th;
    Creature *mount;

    if (isCreature() && isDelete)
        thisc->FocusCheck(NULL);

    if (HasStati(ENGULFED)) {
        Thing *en = GetStatiObj(ENGULFED);
        en->RemoveStati(ENGULFER,-1,-1,-1,this);
        RemoveStati(ENGULFED);
        for (i=0;m->Things[i];i++)
            if (m->Things[i] == myHandle)
            {
                m->Things.Remove(i);
                goto FoundAndRemoved2;
            }
        Error("Cannot remove engulfed creature from Things!");
FoundAndRemoved2:
        m = NULL;
        x = -1;
        y = -1;
    }

    if (HasStati(ENGULFER)) {
        ASSERT(isCreature());
        /* Later, some Remove(false) calls should *not* drop our
        engulfed creatures; for example, if a dragon swallows
        an adventurer, the adventurer shouldn't pop out if the
        dragon casts /teleport/, or even /teleport level/. But
        that involves going through the code to verify that
        each individual Revove(false) call won't potentially
        end with the Player having m==NULL, so for now any
        remove drops engulfed creatures. */
        if (isDelete || 1)
            thisc->DropEngulfed();
    } 

    if (HasStati(MOUNTED))
        mount = (Creature*)GetStatiObj(MOUNTED);
    else
        mount = NULL;

    if (isType(T_DOOR) && m) {
        ((Door*)this)->DoorFlags &= ~DF_SECRET;
        ((Door*)this)->DoorFlags |= DF_OPEN;
        SetImage();
    }

    if (m)
        for (i=0;m->Fields[i];i++)
            if (m->Fields[i]->Creator == myHandle || (mount && m->Fields[i]->Creator == mount->myHandle))
                if (isDelete || m->Fields[i]->FType & FI_MOBILE) {
                    m->RemoveField(m->Fields[i]);
                    i--;
                }

    if (m && m->At(x,y).hasField)
        for(i=0;m->Fields[i];i++)
            if (m->Fields[i]->inArea(x,y)) {
                ThrowField(EV_FIELDOFF,m->Fields[i],this);
                if (mount)
                    ThrowField(EV_FIELDOFF,m->Fields[i],mount);
            }

    if (ox > -1 && m != NULL && !HasStati(MOUNT)) {
        for(i=0;m->Things[i];i++)
            if(m->Things[i]==myHandle) {
                m->Things.Remove(i);
                goto FoundAndRemoved;
            }
        Fatal("Could not find & remove Thing from m->Things!");

FoundAndRemoved:
        if (m->At(ox,oy).Contents == myHandle)
            m->At(ox,oy).Contents = Next;
        else {
            th = oThing(m->At(ox,oy).Contents);
            while(th && th->Next != myHandle) {
                if (!th->Next) {
                    // I'm getting this on vampire spawn vs. blue jelly every
                    // time, and I don't know why. I'm changing it to Error so
                    // it doesn't kill the game. -- Julian
                    Error("Contents list wierdless in Thing::Remove!");
                    return;
                }
                th = oThing(th->Next);
            }
            if (th) 
                th->Next = Next;
        }
        if (theGame->InPlay())
            m->Update(ox,oy);
    }
    m = NULL; x = y = -1; Next = 0;
    if (mount) {
        mount->m = NULL;
        mount->x = -1;
        mount->y = -1;
        mount->Next = 0;
    }

    /* Catch the case where a Creature is killed but we
    keep its Creature object to attach to the corpse
    in case of resurrection, i.e., for players and
    player companions. */
    if (isDelete || (isCreature() && thisc->isDead()))
        CleanupRefedStati();

    if (isDelete) {
        Flags |= F_DELETE;
        if (Type != T_PLAYER) {
            if (theGame->DestroyCount)
                for (i=0;i!=theGame->DestroyCount;i++)
                    if (theGame->DestroyQueue[i] == this)
                    {
                        /* Error("Object being destroyed twice!"); */
                        goto SkipQueueAdd;
                    }
            theGame->DestroyQueue[theGame->DestroyCount++] = this;
SkipQueueAdd:;
        }
        if (theGame->DestroyCount > 20450)
            Error("Too many objects being destroyed at once!");
    }
}

  void Item::Remove(bool isDelete) {
      int16 i; Character *c; Monster *m; String str;
      if (Parent) {
          if (oThing(Parent)->isCreature()) {
              if (IFlags & IF_WORN) {
                  EventInfo e;
                  e.Clear();
                  e.eID = eID;
                  e.EActor = Owner();
                  e.EItem = this;
                  e.isRemove = true;
                  e.isItem = true;
                  e.efNum = 0;
                  if (e.eID)
                      ReThrow(EV_EFFECT,e);

                  if (isType(T_ARMOUR)) {
                      int16 q;
                      for (i=0;i!=8;i++)
                          if (q = GetQuality(i)) {
                              str = "quality::";
                              if (LookupOnly(APreQualNames,q))
                                  str += Lookup(APreQualNames,q);
                              else
                                  str += Lookup(APostQualNames,q);
                              e.eID = FIND(str);
                              if (!e.eID)
                                  continue;
                              e.isRemove = true;  
                              e.isItem = true;
                              e.efNum = 0;
                              ReThrow(EV_EFFECT,e);
                          }
                  }
                  /* If you get disarmed, lose the parry bonus! */
                  e.EActor->CalcValues();                    
              }
              if (!Parent)
                  goto NoMoreParent;
              if (oThing(Parent)->isCharacter()) {
                  c = (Character*) (Owner());
                  for (i=0;i!=SL_LAST;i++)
                      if (c->Inv[i] == myHandle) {
                          c->Inv[i] = 0;
                      }
              } else {
                  IFlags &= ~IF_WORN;
                  m = (Monster*)Owner();
                  if (m->Inv == myHandle)
                      m->Inv = Next;
                  else {
                      Item *it = oItem(m->Inv);
                      while(it && it->Next != myHandle)
                          it = oItem(it->Next);
                      if (!it)
                          Error("Can't find item to remove in monster inv list!");
                      it->Next = Next;
                  }
              }
          }
          else if (oThing(Parent)->isItem())
              ((Container*)oItem(Parent))->Unlist(this);
          Parent = Next = 0;    
      }
NoMoreParent:
      IFlags &= ~IF_WORN;
      RemoveEffStati(FIND("soulblade"));
      Thing::Remove(isDelete);
  }

void Creature::DoEngulf(Creature *engulfer)
  {
    if (!engulfer->m)
      return;
    /* Can't be engulfed by two creatures. */
    ASSERT(!HasStati(ENGULFED))
    
    /* Let's not complicate this with nested engulfs. */
    if(HasStati(ENGULFER))
      return;
      
    if (HasStati(MOUNT))
      return;
  
    /* Remove us from the map, so that we are pulled out
       of the relevant Contents lists, fields cease to
       have an effect on us, etc. */
    Remove(false);
    
    /* Now, manually reset m, x and y so that we are in
       the same map position as the engulfer -- but we
       are NOT in the contents list for that map position.
       This actually makes sense -- we WANT the map to
       show up when people read our locals, but we don't
       want to show up to calls like m->FirstAt(x,y),
       which is why we aren't in Contents. We ARE on the
       map, but nothing can interact with us. */
    x = engulfer->x;
    y = engulfer->y;
    m = engulfer->m;
    m->Things.Add(myHandle);
    if (isPlayer()) {
      m->RegisterPlayer(myHandle);
      thisp->MyTerm->SetMap(m);
			thisp->MyTerm->AdjustMap(x,y,true);
      thisp->UpdateMap = true;
      }
    
    engulfer->GainPermStati(ENGULFER,this,SS_ATTK,EG_ENGULF);
    GainPermStati(ENGULFED,engulfer,SS_ATTK,EG_ENGULF);
  
  }

void Creature::DropEngulfed(Creature *droponly)
  {
    int32 i; Thing *t;
    while (t = GetStatiObj(ENGULFER,-1,droponly)) 
      {
        t->RemoveStati(ENGULFED);
        RemoveStati(ENGULFER,-1,-1,-1,t);
        if (t->m) {
          for (i=0;i!=t->m->Things.Total();i++)
            if (t->m->Things[i] == t->myHandle)
              {
                t->m->Things.Remove(i);
                goto RemovedFromThings;
              }
          Error("Cannot find engulfed creature in t->m!");
          RemovedFromThings:
          t->m = NULL;
          }
        t->x = 0;
        t->y = 0;
       
        t->PlaceAt(m,x,y);
      }  
  }
