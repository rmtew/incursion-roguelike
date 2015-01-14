/* VISION.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Includes all the code governing vision, perception and line
   of sight/fire, for both monsters and players.

      inline void Map::MarkAsSeen(int8 pn, int16 lx, int16 ly, int16 dist)
      bool Map::LineOfSight(int16 sx, int16 sy, int16 tx, int16 ty)
      bool Map::LineOfFire(int16 sx, int16 sy, int16 tx, int16 ty)
      void Map::VisionPath(int8 pn, int16 tx, int16 ty)
      void Player::CalcVision()
      void Map::VisionThing(int16 pn)
      bool Player::Seen(int16 x,int16 y)
      uint16 Creature::Perceives(Thing *t)
*/

#include "Incursion.h"

/* WW. Things like SightRange and BlindRange used to be Vision global
 * variables, but it was giving bad results when a monster would want to
 * check LineOfSight, etc. */

/* Typical Valeus:
 *
 * Sight Range = 15
 * Light Range = 4 (torch)
 * Shadow Range = 8 (torch)
 * Infra Range = 6
 */

uint8 lastRegion, playerRegion;

// returns true if this blocks are view and we should stop looking
// in this direction 
inline bool Map::MarkAsSeen(const int8 pn, const int16 lx, const int16 ly, 
    const int16 dist, const int16 SightRange, const int16 LightRange, 
    const int16 ShadowOrBlindRange)
{
  static uint16 Mask;
  LocationInfo & Here = At(lx,ly); 
  if (SightRange) {
    if (//Here.Dark ||
        dist > SightRange || // beyond your maximum range, period 
        (dist > ShadowOrBlindRange && !Here.Lit && !Here.mLight))
      return true; 
    else if (dist > LightRange && !Here.Lit && !Here.mLight)
      Mask = VI_DEFINED << (pn*4);
    else
      Mask = (VI_VISIBLE | VI_DEFINED) << (pn*4);
  } else {
    if (dist > ShadowOrBlindRange)
      return true;
    else
      Mask = VI_DEFINED << (pn*4);
  } 

  Here.Visibility |= Mask;

  if (TTER(TerrainAt(lx,ly))->HasFlag(TF_INTERIOR))
    if (Here.Region != lastRegion)
      Here.Visibility |= VI_EXTERIOR;

  Here.Memory      = Here.Glyph;

  if (!SightRange && SolidAt(lx,ly))
    return true; 

  /*
  if (OpaqueAt(lx,ly))
    return true;
  else if (SightRange && (ObscureAt(lx,ly)))
    return false; 
  else if (!SightRange && (SolidAt(lx,ly)))
    return false; 
    */

  return false; 
  /* ww: this looks like a handy optimization but leads to inconsistencies
   * (especially about obscuring terrain) 
   */

  {LocationInfo & l = At(lx+1,ly); 
    l.Visibility |= Mask;
    l.Memory      = l.Glyph;
  }

  {LocationInfo & l = At(lx-1,ly); 
    l.Visibility |= Mask;
    l.Memory      = l.Glyph;
  }

  {LocationInfo & l = At(lx,ly+1); 
    l.Visibility |= Mask;
    l.Memory      = l.Glyph;
  }

  {LocationInfo & l = At(lx,ly-1); 
    l.Visibility |= Mask;
    l.Memory      = l.Glyph;
  }

  {LocationInfo & l = At(lx+1,ly+1); 
    l.Visibility |= Mask;
    l.Memory      = l.Glyph;
  }

  {LocationInfo & l = At(lx+1,ly-1); 
    l.Visibility |= Mask;
    l.Memory      = l.Glyph;
  }

  {LocationInfo & l = At(lx-1,ly-1); 
    l.Visibility |= Mask;
    l.Memory      = l.Glyph;
  }

  {LocationInfo & l = At(lx-1,ly+1); 
    l.Visibility |= Mask;
    l.Memory      = l.Glyph;
  }

  return false; 
}

/* ww: if this were a real language I'd use a higher-order function. 
 * And believe me, I'd really like to, 'cause this code also appears
 * in RAttack() and ABallBeamBolt() ... Anyway, I want to pull this
 * out as a macro here because it occurs three times in this file and
 * I'm tired of maintaining those instances separately. */

// pass this macro in as the last argument to HUGE_PATH_MACRO if you want
// it be a "vision" check that cares about opaque/obscure stuff
#define CARE_ABOUT_SEEING \
            if  (OpaqueAt(cx,cy)\
              || (Here.Dark && !ignoreDark)\
              || (ObscureAt(cx,cy) && !ignoreObscure))\
            return false;
#define CARE_ABOUT_SOLID \
            if  (SolidAt(cx,cy) || Here.isWall)\
            return false;

#define HUGE_PATH_MACRO(DO_THING0,DO_THING1,DO_THING2,DO_THING3,DO_THING4,SEEING_CHECK) \
    cx = sx; cy = sy; \
    if (cx == tx) { \
      if (cy == ty) return true; \
      while (1) { \
          if (!InBounds(cx,cy)) return false;\
          LocationInfo & Here = At(cx,cy); \
          DO_THING0; \
          DO_THING1; \
          if (cy == ty) return true;\
          if (cy != sy) {SEEING_CHECK;} \
          cy += dirY;\
        }\
      }\
    else if (cy == ty) {\
      while (1) { \
          if (!InBounds(cx,cy)) return false;\
          LocationInfo & Here = At(cx,cy); \
          DO_THING0; \
          DO_THING2; \
          if (cx == tx) return true;\
          if (cx != sx) {SEEING_CHECK;} \
          cx += dirX;\
        }\
      } \
    Fraction slope, test;\
    slope.Set(abs((ty*2 - dirY) - cy*2), abs((tx*2 - dirX) - cx*2)); \
    while (InBounds(cx,cy)) { \
      LocationInfo & Here = At(cx,cy); \
      DO_THING3; \
      DO_THING4; \
      if (cx == tx && cy == ty /* dist(cx,cy,tx,ty) <= 1*/)\
        return true;\
      if (cx != sx || cy != sy) { SEEING_CHECK; } \
      {\
        test.Set(abs((cy*2 + dirY)-sy*2), abs((cx*2 + dirX)-sx*2));\
        if (test > slope) {\
          cx += dirX;\
        } else if (test < slope) {\
          cy += dirY;\
        } else {\
          cx += dirX;\
          cy += dirY;\
        }\
      }\
    } return false; 

bool Map::LineOfVisualSight(int16 sx, int16 sy, int16 tx, int16 ty, Creature *c)
{
  int16 cx,cy;
  const int8 dirX = (tx >= sx) ? 1 : -1, dirY = (ty >= sy) ? 1 : -1;
  const bool ignoreObscure = c && c->NatureSight; 
  const bool ignoreDark = false; 
  HUGE_PATH_MACRO(;, ;, ;, ;, ;, CARE_ABOUT_SEEING)
} 

bool Map::LineOfFire(int16 sx, int16 sy, int16 tx, int16 ty, Creature *c)
{
  int16 cx,cy;
  const int8 dirX = (tx >= sx) ? 1 : -1, dirY = (ty >= sy) ? 1 : -1;
  HUGE_PATH_MACRO(;, ;, ;, ;, ;, CARE_ABOUT_SOLID)
} 

bool Map::LineOfSight(int16 sx, int16 sy, int16 tx, int16 ty, Creature *c)
{
  return 
    ((!c || c->SightRange || c->InfraRange) ? 
     LineOfVisualSight(sx,sy,tx,ty,c) : false) ||
    ((c && c->BlindRange) ?
     LineOfFire(sx,sy,tx,ty,c) : false);
} 

// ww: 'c' is the one doing the looking
bool Map::VisionPath(int8 pn, 
    const int16 sx, const int16 sy, 
    const int16 tx, const int16 ty, 
    Creature *c, const int16 SiR, const int16 LiR, const int16 ShR)

{
  int16 cx, cy;
  const int8 dirX = (tx >= sx) ? 1 : -1, dirY = (ty >= sy) ? 1 : -1;
  lastRegion = At(sx,sy).Region;

  bool ignoreObscure = c && c->NatureSight; 
  bool ignoreDark = false; 
  HUGE_PATH_MACRO(
      if (!Here.Solid) lastRegion = Here.Region,
      if (MarkAsSeen(pn,cx,cy,abs(sy-cy),SiR,LiR,ShR)) return false,
      if (MarkAsSeen(pn,cx,cy,abs(sx-cx),SiR,LiR,ShR)) return false,
      if (!Here.Solid) lastRegion = Here.Region,
      if (MarkAsSeen(pn,cx,cy,dist(sx,sy,cx,cy),SiR,LiR,ShR)) return false,
      CARE_ABOUT_SEEING)
}

bool Map::BlindsightVisionPath(int8 pn, 
    const int16 sx, const int16 sy, 
    const int16 tx, const int16 ty, 
    Creature * c, const int16 BlR)
{
  const int16 SiR = 0, LiR = 0;

  int16 cx, cy;
  const int8 dirX = (tx >= sx) ? 1 : -1, dirY = (ty >= sy) ? 1 : -1;
  lastRegion = At(sx,sy).Region;

  HUGE_PATH_MACRO(
      if (!Here.Solid) lastRegion = Here.Region,
      if (MarkAsSeen(pn,cx,cy,abs(sy-cy),SiR,LiR,BlR)) return false,
      if (MarkAsSeen(pn,cx,cy,abs(sx-cx),SiR,LiR,BlR)) return false,
      if (!Here.Solid) lastRegion = Here.Region,
      if (MarkAsSeen(pn,cx,cy,dist(sx,sy,cx,cy),SiR,LiR,BlR)) return false,
      CARE_ABOUT_SOLID)
}


void Player::CalcVision()
{
  ASSERT(m);
  if (!m)
    return;
  m->VisionThing(0,this,true);
  StatiIterNature(this,HAS_ANIMAL)
      m->VisionThing(0,oCreature(S->h),false);
  StatiIterEnd(this)
}

void Map::VisionThing(int16 pn, Creature *c, bool do_clear)
{
    int8 pn8 = (int8)pn;
  int16 i,cx,cy;
  Player *p = oPlayer(pl[pn]);

  // ww: this can apparently happen while we are changing levels
  if (!InBounds(c->x,c->y))
    return; 

  const int16 sx = c->x;
  const int16 sy = c->y; 
  const int16 LightRange = max(c->LightRange,c->InfraRange);
  const int16 SightRange = c->SightRange;
  const int16 ShadowRange = max(c->ShadowRange,LightRange); 
  const int16 BlindRange = c->BlindRange; 
  const int16 PercepRange = c->PercepRange; 
  const int16 TremorRange = 
    max(c->TremorRange, c->AbilityLevel(CA_STONEWORK_SENSE)); 

  const int16 x1 = max(p->MyTerm->OffsetX()-5,0);
  const int16 y1 = max(p->MyTerm->OffsetY()-5,0);
  const int16 x2 = min(x1 + p->MyTerm->MSizeX()+10,sizeX-1);
  const int16 y2 = min(y1 + p->MyTerm->MSizeY()+10,sizeY-1);

  if (do_clear) 
    for (cx=x1;cx<=x2;cx++)
      for(cy=y1;cy<=y2;cy++)
        At(cx,cy).Visibility &= 
          ~(VI_VISIBLE | VI_DEFINED | VI_EXTERIOR); /* >> player*4 */

  playerRegion = At(sx,sy).Region;
  lastRegion   = At(sx,sy).Region;

  if ((c->isBlind() && !BlindRange) || 
      (!LightRange && !LightAt(sx,sy))) {
    At(sx,sy).Visibility |= VI_VISIBLE | VI_DEFINED;
    At(sx,sy).Memory      = At(sx,sy).Glyph;
  }

  if (SightRange) { 
    MarkAsSeen(pn8,sx,sy,0,SightRange,LightRange,ShadowRange);
    for (cx=x1;cx<=x2;cx+=1) {
      VisionPath(pn8,sx,sy,cx,y1,c,SightRange,LightRange,ShadowRange);
      VisionPath(pn8,sx,sy,cx,y2,c,SightRange,LightRange,ShadowRange);
    }
    for (cy=y1;cy<=y2;cy+=1) {
      VisionPath(pn8,sx,sy,x1,cy,c,SightRange,LightRange,ShadowRange);
      VisionPath(pn8,sx,sy,x2,cy,c,SightRange,LightRange,ShadowRange);
    }
  }

  if (BlindRange) {
    MarkAsSeen(pn8,sx,sy,0,0,0,BlindRange);
    for (cx=x1;cx<=x2;cx+=1) {
      BlindsightVisionPath(pn8,sx,sy,cx,y1,c,BlindRange);
      BlindsightVisionPath(pn8,sx,sy,cx,y2,c,BlindRange);
    }
    for (cy=y1;cy<=y2;cy+=1) {
      BlindsightVisionPath(pn8,sx,sy,x1,cy,c,BlindRange);
      BlindsightVisionPath(pn8,sx,sy,x2,cy,c,BlindRange);
    }
  } 

  if (PercepRange) {
    for(cx=sx - PercepRange;cx<=sx + PercepRange ;cx++)
      for(cy=sy - PercepRange;cy<=sy + PercepRange ;cy++)
        if (InBounds(cx,cy))
          if (dist(cx,cy,sx,sy) <= PercepRange) {
            At(cx,cy).Visibility |= (VI_VISIBLE | VI_DEFINED);
            // ww: previously you could see it, but you forgot!
            At(cx,cy).Memory = At(cx,cy).Glyph;
          }
  }

  if (p->Opt(OPT_WIZ_SIGHT))
    for (cx=x1;cx<=x2;cx++)
      for(cy=y1;cy<=y2;cy++)
        At(cx,cy).Visibility |= (VI_VISIBLE | VI_DEFINED); /* >> player*4 */

  if (TremorRange) {
    for(cx=sx - TremorRange;cx<=sx + TremorRange ;cx++)
      for(cy=sy - TremorRange;cy<=sy + TremorRange ;cy++)
        // ww: you know the stone: the rest you'll have to figure
        // out. before you could use stonework sense to see through
        // mushroom forest or to see a river on the other side of a
        // wall ...
        if (InBounds(cx,cy) && SolidAt(cx,cy))
          At(cx,cy).Memory = At(cx,cy).Glyph;
  }

  Thing *t;
  MapIterate(this,t,i) {
    if (t && 
        ( (t->isFeature() && !(t->Flags & (F_NO_MEMORY|F_INVIS))) ||
          (t->isItem() && (((Item *)t)->isType(T_CHEST))) ||
          (t->isItem() && (((Item *)t)->isType(T_FOUNTAIN)))
        ) 
       )
      if (At(t->x,t->y).Memory != 0)
        At(t->x,t->y).Memory = (At(t->x,t->y).Memory & GLYPH_BACK_MASK) | 
		(t->Image & ~GLYPH_BACK_MASK);
  } 
}

bool Player::Seen(int16 x,int16 y)
  {
    return m->At(x,y).Memory != 0;
  }

// If 'assertLOS' is true, we are sure that 'this' can see the square that
// 't' is standing on ... but 't' might still be hiding or invisible or
// something. 
uint16 Creature::Perceives(Thing *t, bool assertLOS)
{
  int16 i, tx, ty; 
  uint16 Per; bool isBlind;
  static int16 limit = 0; 
  // ww: somewhat like a mutex to prevent infinite recursion

  //return PER_VISUAL;

  if (!t)
    return 0;
    
  if (t->HasStati(MOUNT))
    return (Perceives(t->GetStatiObj(MOUNT),assertLOS));
    
  if (m && t->m && (t->m != m))
    return 0;

  tx = t->x;
  ty = t->y;

    
  if (t->isCreature() && ((Monster*)t)->HasMFlag(M_SELLER))
    if (isPlayer() && !t->isDead() && !t->m)
      return PER_VISUAL;
    
  /* This is a horrid kludge to allow summoners and party-builders
     to have a remotely enjoyable game despite the fact that the
     monster AI is currently abysmally stupid, to the point that
     pets constantly get lost. */
  if (isMonster() && t->isPlayer())
    if (getLeader() == t || ts.hasTargetOfType(OrderWalkNearMe))
      Per |= PER_VISUAL;
    
  if (isIllusion())
    {
      Creature *cr;
      cr = (Creature*) GetStatiObj(ILLUSION);
      return cr->Perceives(t,assertLOS);
    } 
   

  theGame->inPerceive++;

  if (HasStati(SLEEPING)) 
    { theGame->inPerceive--; return 0; } 
  
  if (t->HasStati(ENGULFED))
    {
      if (t->GetStatiObj(ENGULFED) == this)
        { theGame->inPerceive--; return PER_VISUAL; }
      theGame->inPerceive--;
      return 0;
    }
  
  const bool HasStati_ENGULFED = HasStati(ENGULFED);  
  const bool HasStati_BLIND = HasStati(BLIND);
  const bool HasStati_SEE_INVIS = HasStati(SEE_INVIS);
  const int  StatiVal_PERCEPTION = GetStatiVal(PERCEPTION); 
  Creature *anim = (Creature *)GetStatiObj(HAS_ANIMAL); 
  Creature *druid = (Creature *)GetStatiObj(ANIMAL_COMPANION); 
  Creature *focus = (Creature *)GetStatiObj(DWARVEN_FOCUS); 
  int16 detectingMAType[32];
  int16 detectingMARange[32];
  rID   detectingMAeID[32];
  int16 detectingMAMax = 0; 
  int16 detectingType[32];
  int16 detectingRange[32];
  rID   detectingEID[32];
  int16 detectingMax = 0; 

  Per = 0;

  
  StatiIterNature(this,DETECTING)
      detectingMAType[detectingMAMax] = S->Val; 
      detectingMARange[detectingMAMax] = S->Mag; 
      detectingMAeID[detectingMAMax] = S->eID;
      detectingMAMax++; 
      if (detectingMAMax > 31)
        break;
  StatiIterEnd(this)
  ASSERT(detectingMAMax <= 32); 
  StatiIterNature(this,DETECTING_TYPE)
      detectingType[detectingMax] = S->Val; 
      detectingRange[detectingMax] = S->Mag; 
      detectingEID[detectingMax] = S->eID;
      detectingMax++; 
      if (detectingMAMax > 31)
        break;
  StatiIterEnd(this)
  ASSERT(detectingMax <= 32); 

  /* You always perceive yourself */
  if (t == this)
    Per |= PER_VISUAL;
  if (HasStati_ENGULFED)
    if (t == GetStatiObj(ENGULFED))
      Per |= PER_VISUAL;
  
  
  if (!theGame->InPlay())
    { theGame->inPerceive--; return Per; }


  if (t->Flags & F_DELETE)
    {
      tx = t->GetStatiVal(DEATH_LOC) % 256;
      ty = t->GetStatiVal(DEATH_LOC) / 256;
      if ((!m) || (t->GetStatiMag(DEATH_LOC) != m->Depth))
        { theGame->inPerceive--; return 0; }
    }
  else {
    tx = t->x;
    ty = t->y;
    }

  const int16 Dist = max(1,dist(x,y,tx,ty));

  if (!HasStati_ENGULFED)
    StatiIterNature(this,TRACKING)
        if (S->h == t->myHandle && Dist <= S->Mag)
        Per |= PER_TRACK; 
    StatiIterEnd(this)


  if (t->HasStati(TELEPORTED)) { theGame->inPerceive--; return 0; } 
  const bool t_HasStati_PARALYSIS = t->HasStati(PARALYSIS);
  const bool t_HasStati_HIDING = t->HasStati(HIDING);
  const bool t_HasStati_INVIS = t->HasStati(INVIS) || 
              ((t->onPlane() == PHASE_ETHEREAL) && (onPlane() != PHASE_ETHEREAL)
                 && !HasStati(MANIFEST));
  const bool t_HasStati_INVIS_TO = t->HasStati(INVIS_TO);
  const bool t_HasStati_ILLUMINATED = t->HasStati(ILLUMINATED);
  const bool t_isCreature = t->isCreature(); 
  const int16 NonDet = t->XORStatiVal(NONDETECTION);

  if (t_isCreature && !(NonDet & PER_DETECT)) {
    for (i=0; i<detectingMAMax; i++) 
      if (Dist <= detectingMARange[i] &&
          (detectingMAType[i] == 0 || 
           ((Creature *)t)->isMType(detectingMAType[i])))
        if ((!detectingMAeID[i]) || TEFF(detectingMAeID[i])->
                      PEvent(EV_ISDETECT,t,detectingMAeID[i]) != -1)
          Per |= PER_DETECT; 
  } else if (!t_isCreature && !(NonDet & PER_DETECT)) {
    for (i=0; i<detectingMax; i++) 
      if (Dist <= detectingRange[i] &&
          (detectingType[i] == 0 || 
           t->isType(detectingType[i])))
        if (TEFF(detectingEID[i])->PEvent(EV_ISDETECT,t,detectingEID[i]) != -1)
          Per |= PER_DETECT; 
  } 


  if (t->m != m && t->isItem()) {
    if (((Item*)t)->Owner() == this)
      { theGame->inPerceive--; return PER_VISUAL; }
    if (((Item*)t)->Owner() == NULL)
      { theGame->inPerceive--; return PER_VISUAL; }
    if (Per |= Perceives(((Item*)t)->Owner()))
      { theGame->inPerceive--; return Per; }
    theGame->inPerceive--; return 0;
  }

  if (Dist > 30)
    { theGame->inPerceive--; return Per; }


  /* Check Telepathy, Tremorsense, etc. */
  if (t->isType(T_DOOR))
    if (((Door*)t)->DoorFlags & DF_SECRET)
      { theGame->inPerceive--; return Per & (~PER_DETECT); }
  if (t->isType(T_TRAP))
    if (!(((Trap*)t)->TrapFlags & TS_FOUND))
      { theGame->inPerceive--; return Per & (~PER_DETECT); }


  if (!HasStati_ENGULFED)
    if (TremorRange >= Dist && !(NonDet & PER_TREMOR)) {
      if (t_isCreature) {
        if (!t_HasStati_PARALYSIS && !((Creature*)t)->isAerial())
          {
            int16 lv;
            if (((Creature*)t)->HasSkill(SK_BALANCE) && (lv = 
                   ((Creature*)t)->SkillLevel(SK_BALANCE)) >= 20)
              if (t->HasStati(HIDING) || lv >= 30)
                goto InvisToTremor;
            Per |= PER_TREMOR;
            InvisToTremor:;
          }
        }
      else 
        Per |= PER_TREMOR;
    }

  if (!HasStati_ENGULFED)
    if (BlindRange >= Dist && !(NonDet & PER_BLIND)) 
      if (!m->FieldAt(tx,ty,FI_SILENCE)) {
        int tPlane = t->isCreature() ? ((Creature *)t)->onPlane() :
          PHASE_MATERIAL;
        if (onPlane() == tPlane)
          if (assertLOS || (m->LineOfFire(x,y,tx,ty,this)))
            if (!t_HasStati_HIDING || (SkillLevel(SK_LISTEN) >= 
                  ((Creature*)t)->SkillLevel(SK_MOVE_SIL)))
              Per |= PER_BLIND;
      }

  /* No engulfed -- Penetrating Sight lets you see through the
     creature's stomach, so works normally. */
  if (PercepRange >= Dist && !(NonDet & PER_PERCEPT))
    if (StatiVal_PERCEPTION)
      Per |= PER_PERCEPT;

  if (isPlayer()) {
    if (anim && !(anim->isDead()) && !limit) {
      limit = 1;
      uint16 res = anim->Perceives(t);
      if (res) Per |= res | PER_SHARED; 
      limit = 0;
    }
    if (t == focus || t == anim)
      Per |= PER_TRACK | PER_TELE;

    if (thisp->Opt(OPT_WIZ_SIGHT))
      if (Dist < 30)
        Per |= PER_VISUAL;

  } else {
    if (druid && !(druid->isDead()) && !limit)  {
      limit = 1;
      uint16 res = druid->Perceives(t);
      if (res) Per |= res | PER_SHARED; 
      limit = 0;
    }
  }

  if (TelepRange >= Dist && !(NonDet & PER_TELE))
    if (t_isCreature)
      if (!((Creature*)t)->HasMFlag(M_MINDLESS))
        if (!((Creature*)t)->isMType(MA_UNDEAD))
          Per |= PER_TELE;

  /* Later, you should have to 'turn on' scent, and it should
     slow you to half-movement through its use. */
  if (HasStati_ENGULFED)
    if (ScentRange >= Dist && !(NonDet & PER_SCENT))
      /*if (HasStati(SCENT_ON))*/
      if (t_isCreature)
        if (assertLOS || (m->LineOfFire(x,y,tx,ty,this)))
          if (onPlane() == ((Creature*)t)->onPlane()) 
            Per |= PER_SCENT;

  if (isMonster() && this == thism->CurrAI)
    isBlind = thism->isBlind;
  else
    isBlind = HasMFlag(M_BLIND) || HasStati_BLIND;

  if (isBlind && (Dist > BlindRange))
    { theGame->inPerceive--; return Per; }

  if (max(abs(x-tx),abs(y-ty)) > max(SightRange,BlindRange))
    { theGame->inPerceive--; return Per; }

  // ww: we need InBounds here to avoid crashes
  /* ww: I disagree with this logic on game-annoyance reasons :-). A
   * player with a ring of invisibility can see a Shadow in the middle
   * of a room, but as soon as the shadow moves into the wall and starts
   * attacking the player, the player cannot see the shadow. The player
   * can, however, attack the shadow, and can get messages like "you're
   * about to leave a threatened area, confirm?", so what actually
   * happens is that you just try all 8 directions until you find the
   * right one. 
   if (!m || !m->InBounds(t->x,t->y) || 
   (m->At(t->x,t->y).Opaque && !t->isFeature()))
   goto SkipVisual;
   */

  if (HasStati_ENGULFED)
    goto SkipVisual;

  if (t_HasStati_HIDING && t_isCreature)
    if (((Creature*)t)->SkillLevel(SK_HIDE) +
        ((Creature*)t)->HideVal > SkillLevel(SK_SPOT)) {
      /* ww: in isHostileTo, a hiding player is never hostile!
       * thus, enemies always saw through the PC's Hide In Shadows! */
      if (!((Creature*)t)->isLedBy(this) ||
          t->isPlayer())
        goto SkipVisual;
    } 

  if (t_HasStati_INVIS && !HasStati_SEE_INVIS)    /* later, update */
    goto SkipVisual;
  if (t_HasStati_INVIS_TO && !HasStati_SEE_INVIS)
    StatiIterNature(t,INVIS_TO) {
      if (S->Mag == 0 && S->eID) 
        { EventInfo xe;
          xe.Clear();
          xe.EActor = this;
          xe.ETarget = t;
          xe.eID = S->eID;
          if (isTarget(xe,t))
            StatiIterBreakout(t,goto SkipVisual)
        }
      else if (isMType(S->Mag))
        StatiIterBreakout(t,goto SkipVisual)
      }
    StatiIterEnd(t)


  if (isPlayer()) {
    if (Dist <= SightRange) { 
      if (m->At(tx,ty).Visibility & VI_VISIBLE)
        Per |= PER_VISUAL | PER_SHADOW;
      else if (m->At(tx,ty).Visibility & VI_DEFINED)
        Per |= ( t_HasStati_ILLUMINATED ? PER_VISUAL : 0 ) | PER_SHADOW;
    }
  } else {
    if (assertLOS || 
        (assertLOS = m->LineOfVisualSight(x,y,tx,ty,this)))
      Per |= PER_VISUAL | PER_SHADOW;
  }
  if (Dist > (LightRange*2) && !m->At(tx,ty).Lit && !m->At(tx,ty).mLight &&
       t != this) {
    Per &= ~(PER_VISUAL|PER_SHADOW);
  }          

  /* For now, we are treating "infravision" as darkvision rather
     then heat-sight. This may change later, depending on human/
     demihuman balance and power levels. */
  if (Dist <= InfraRange &&
      (assertLOS || 
       (assertLOS = m->LineOfVisualSight(x,y,tx,ty,this))
      ))
    Per |= PER_INFRA;

  if (!t->isShadowShape())
    Per &= ~PER_SHADOW;

SkipVisual:
  /*
     if (t->Flags & F_HILIGHT)
     if (Dist <= DetectRange && !(NonDet & PER_DETECT))
     Per |= (StatiVal_DETECTING & DET_VISIBLE) ? PER_DETECT : PER_SHADOW;
   */

  { theGame->inPerceive--; return Per; }
}





