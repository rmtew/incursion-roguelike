/* API.H -- Copyright (c) 1999-2003 Julian Mensch
     This file contains declarations for all of the publically
   accessible IncursionScript API calls and variables. */

/* IMPORTANT! If this file is ever altered, then dispatch.h
   must be rebuilt by the resource compiler, and VMachine.cpp
   must be recompiled. If this is not done, strange errors
   and glitches will result! Do not alter the system function
   definitions unless you have also altered the C++ source code
   of Incursion.Exe. Even changing the _order_ of the functions
   listed here will cause the virtual machine to fail! */

/* The script engine has very limited data types at the
   moment. We use these defines so that we won't have to
   rewrite this file (much) when it gets more advanced. */

#define char       int32
#define EvReturn   int32
#define uint32     int32
//#define uint16     int32
#define NULL_ID    ((rID)0)
#define NULL_RECT  ((Rect)0)
#define NULL_OBJ   ((hObj)0)

/*****************************************************************************
 *                               System Objects                              *
 *****************************************************************************/

system T_EVENTINFO e        == SOBJ_E;
system T_TERM      ETerm    == SOBJ_ETERM;
system T_PLAYER    EPlayer  == SOBJ_EPLAYER;
system T_CREATURE  EActor   == SOBJ_EACTOR;
system T_THING     ETarget  == SOBJ_ETARGET;
system T_CREATURE  EVictim  == SOBJ_EVICTIM;
system T_ITEM      EItem    == SOBJ_EITEM; 
system T_ITEM      EItem2   == SOBJ_EITEM2;
system T_MAP       EMap     == SOBJ_EMAP;
system T_GAME      theGame  == SOBJ_GAME;

/*****************************************************************************
 *                                Class Object                               *
 *****************************************************************************/

system string  T_OBJECT::Name(int16 fl=0);
system string  T_OBJECT::Describe(hObj:T_PLAYER);

/*****************************************************************************
 *                                 Class Map                                 *
 *****************************************************************************/

system rID     T_MAP::dID;
system int16   T_MAP::Depth;

system bool    T_MAP::InBounds(int16 x,int16 y);
system int32   T_MAP::GlyphAt(int16 x, int16 y);
system int32   T_MAP::FlagsAt(int16 x, int16 y);
system bool    T_MAP::SolidAt(int16 x, int16 y);
system bool    T_MAP::TreeAt(int16 x, int16 y);
system bool    T_MAP::FallAt(int16 x, int16 y);
system bool    T_MAP::OpaqueAt(int16 x, int16 y);
system bool    T_MAP::LightAt(int16 x, int16 y);
system bool    T_MAP::BrightAt(int16 x, int16 y);
system rID     T_MAP::RegionAt(int16 x, int16 y);
system rID     T_MAP::TerrainAt(int16 x, int16 y);
system int16   T_MAP::SizeX();
system int16   T_MAP::SizeY();

system hObj    T_MAP::FirstAt(int16 x, int16 y);
system hObj    T_MAP::NextAt(int16 x, int16 y);
system bool    T_MAP::MultiAt(int16 x, int16 y);

system hObj    T_MAP::FCreatureAt(int16 x, int16 y);
system hObj    T_MAP::NCreatureAt(int16 x, int16 y);
system bool    T_MAP::MCreatureAt(int16 x, int16 y);

system hObj    T_MAP::FItemAt(int16 x, int16 y);
system hObj    T_MAP::NItemAt(int16 x, int16 y);
system bool    T_MAP::MItemAt(int16 x, int16 y);

system hObj    T_MAP::FFeatureAt(int16 x, int16 y);
system hObj    T_MAP::NFeatureAt(int16 x, int16 y);
system bool    T_MAP::MFeatureAt(int16 x, int16 y);

system hObj    T_MAP::FDoorAt(int16 x, int16 y);
system hObj    T_MAP::NDoorAt(int16 x, int16 y);
system bool    T_MAP::MDoorAt(int16 x, int16 y);

system hObj    T_MAP::FTrapAt(int16 x, int16 y);
system hObj    T_MAP::NTrapAt(int16 x, int16 y);
system bool    T_MAP::MTrapAt(int16 x, int16 y);

system hObj    T_MAP::FChestAt(int16 x, int16 y);
system hObj    T_MAP::NChestAt(int16 x, int16 y);
system bool    T_MAP::MChestAt(int16 x, int16 y);

/* system hObj    T_MAP::FirstAt(int32 x, int32 y); */
/* system hObj    T_MAP::NextAt(); */
system hObj    T_MAP::FirstThing();
system hObj    T_MAP::NextThing();

system void    T_MAP::WriteAt(Rect r,int16 x, int16 y, rID terID, rID regID, int32 Pri, bool Force);
system void    T_MAP::WriteBox(Rect r, rID regID);
system void    T_MAP::WriteRoom(Rect r, rID regID);
system void    T_MAP::WriteCircle(Rect r, rID regID);
system void    T_MAP::WriteWalls(Rect r, rID regID);
system void    T_MAP::WriteLifeCave(Rect r, rID regID);
system void    T_MAP::WriteCastle(Rect r, rID regID);
system void    T_MAP::WriteRCavern(Rect r, rID regID);
system void    T_MAP::WriteOctagon(Rect r, rID regID);
system void    T_MAP::WriteDestroyed(Rect r, rID regID);
system void    T_MAP::WriteMaze(Rect r, rID regID);
system void    T_MAP::WriteCross(Rect r, rID regID);
system void    T_MAP::WriteMap(Rect r, rID mID);
system void    T_MAP::WriteBlobs(Rect r, rID regID, rID bID);
system void    T_MAP::Generate(rID dunID, int16 Depth, hObj:T_MAP above, int8 Luck); 
system void    T_MAP::DrawPanel(uint8 x, uint8 y);
system void    T_MAP::PopulatePanel(Rect r,uint16 extraFlags=0);
system uint16  T_MAP::Tunnel(uint8 sx, uint8 sy, uint8 dx, uint8 dy, uint8 TFlags, int32 StartDir, int8 TType);
system void    T_MAP::MakeDoor(uint8 x, uint8 y, rID fID);
system bool    T_MAP::FindOpenAreas(Rect Area, uint16 Flags=0);
system void    T_MAP::SetGlyphAt(int16 x, int16 y, uint16 g);
system void    T_MAP::SetRegion(Rect r, rID regID);
system uint16  T_MAP::GetOpenXY();

system bool    T_MAP::LineOfSight(int16 sx, int16 sy, int16 tx, int16 ty, hObj:T_CREATURE c); 
system void    T_MAP::Update(int16 x,int16 y);
system void    T_MAP::VUpdate(int16 x, int16 y);
system void    T_MAP::MarkAsSeen(int8 pn, int16 x, int16 y, int16 dist);

system void    T_MAP::NewField(int32 FType, int16 x, int16 y, uint8 rad,
                          int32 img, int16 dur, rID eID, hObj:T_CREATURE Creator);
system bool    T_MAP::FieldAt(int16 x,int16 y, int32 FType=-1);
system void    T_MAP::RemoveEffField(rID eID);
system void    T_MAP::RemoveFieldFrom(hObj h);
system void    T_MAP::RemoveEffFieldFrom(rID eID, hObj h);
system bool    T_MAP::DispelField(int16 x, int16 y, uint32 FType, rID eID, int16 clev);

system void    T_MAP::SetQueue(int16 Queue);
system void    T_MAP::UnsetQueue(int16 Queue);
system void    T_MAP::PrintQueue(int16 Queue);
system int16   T_MAP::QueueNum();

system int8    T_MAP::GetTerraDC(int16 x,int16 y);
system hObj    T_MAP::GetTerraCreator(int16 x,int16 y);
system int8    T_MAP::GetTerraDType(int16 x, int16 y);
system int16   T_MAP::GetTerraDmg(int16 x,int16 y);
system void    T_MAP::RemoveTerra(int16 key);
system void    T_MAP::RemoveTerraXY(int16 x, int16 y,rID xID=0);
system void    T_MAP::WriteTerra(int16 x, int16 t,rID tID);
system void    T_MAP::MakeNoiseXY(int16 x, int16 y, int16 radius);

system hObj    T_MAP::GetEncounterCreature(int32 i);

system bool    T_MAP::LineOfFire(int16 x, int16 y, int16 x2, int16 y2, hObj:T_CREATURE cr);

system rID     T_MAP::enUniformGet(int32 key);
system void    T_MAP::enUniformAdd(int32 key, rID choice);
system void    T_MAP::enAddMon(hObj:T_EVENTINFO e);
system void    T_MAP::enAddTemp(hObj:T_EVENTINFO e, rID tID);
system void    T_MAP::enAddMountTemp(hObj:T_EVENTINFO e, rID tID);
system bool    T_MAP::enTemplateOk(hObj:T_EVENTINFO e, rID tID, bool force=false);
system void    T_MAP::enWarn(String msg,...);

system EvReturn T_MAP::thEnGen(rID xID, uint32 fl, int8 CR, uint16 enAlign);
system EvReturn T_MAP::thEnGenXY(rID xID, uint32 fl, int8 CR, uint16 enAlign, int16 x, int16 y);
system EvReturn T_MAP::thEnGenSummXY(rID xID, uint32 fl, int8 CR, uint16 enAlign, hObj:T_CREATURE hCrea, int16 x, int16 y);
system EvReturn T_MAP::thEnGenMon(rID xID, rID mID, uint32 fl, int8 CR, uint16 enAlign);
system EvReturn T_MAP::thEnGenMonXY(rID xID, rID mID, uint32 fl, int8 CR, uint16 enAlign, int16 x, int16 y);
system EvReturn T_MAP::thEnGenMType(rID xID, int16 mt, uint32 fl, int8 CR, uint16 enAlign);
system EvReturn T_MAP::thEnGenMTypeXY(rID xID, int16 mt, uint32 fl, int8 CR, uint16 enAlign, int16 x, int16 y);
system EvReturn T_MAP::thEnGenMonSummXY(rID xID, rID mID, uint32 fl, int8 CR, uint16 enAlign, hObj:T_CREATURE hCrea, int16 x, int16 y);
system EvReturn T_MAP::thEnGenMTypeSummXY(rID xID, int16 mt, uint32 fl, int8 CR, uint16 enAlign, hObj:T_CREATURE hCrea, int16 x, int16 y);

system EvReturn T_MAP::rtEnGen(hObj:T_EVENTINFO e, rID xID, uint32 fl, int8 CR, uint16 enAlign);
    

/*****************************************************************************
 *                                 Class Thing                               *
 *****************************************************************************/

/* Technically in Object, not Thing, but who cares? */
              
system bool   T_OBJECT::isType(int16 ty);
system bool   T_THING::isCreature();
system bool   T_THING::isDead();
system bool   T_THING::isPlayer();
system bool   T_THING::isMonster();
system bool   T_THING::isWeapon();
system bool   T_THING::isArmour();
//system bool   T_THING::isItem();
system bool   T_THING::isFeature();
system bool   T_THING::isCharacter();
system int8   T_THING::Material();
system bool    T_THING::isMetallic();
system bool    T_THING::isWooden();
system bool    T_THING::isOrganic();
system bool    T_THING::isCorporeal();

/* Not needed: ctor, dtor, NotifyGone, DoTurn, Event, FieldOn, FieldOff, 
               UpdateStati, StatiOn, StatiOff, __HasStati, QuickStati */
/* Not usable: GetStati (returns struct Status*), Stati (OArray) */

system hObj   T_THING::m;
system hObj   T_THING::Next;
system int16  T_THING::x;
system int16  T_THING::y;
system int32  T_THING::Image;
system int16  T_THING::Timeout;
system int32  T_THING::Flags;
system string T_THING::Named;

system void   T_THING::Initialize(bool inplay = false);
system void   T_THING::SetImage();
system void   T_THING::IPrint(String msg,...);
system void   T_THING::IDPrint(String msg1, String msg2,...);
system String T_THING::His();
system String T_THING::his();
system String T_THING::Him();
system String T_THING::him();
system String T_THING::He();
system String T_THING::he();
system bool   T_THING::isPlural();
system bool   T_THING::isShadowShape();
//system void   T_THING::Move(int16 x,int16 y);
system void   T_THING::MoveDepth(int16 NewDepth);
system void   T_THING::PlaceNear(int16 x, int16 y);
system void   T_THING::PlaceAt(hObj:T_MAP,int16 x,int16 y);
system void   T_THING::PlaceOpen(hObj:T_MAP);
system void   T_THING::Remove(bool);
system int32  T_THING::DirTo(hObj:T_THING);
system int32  T_THING::DirToXY(int16 x,int16 y);
system int16  T_THING::DistFrom(hObj:T_THING);
system int16  T_THING::DistFromPlayer();
system bool   T_THING::isIllusion();
system bool   T_THING::isRealTo(hObj:T_CREATURE);
system int16  T_THING::onPlane();

system bool   T_THING::inField(int16 FType = -1);
system void   T_THING::GainPermStati(int16 n,hObj:T_THING h, int8 Cause, int16 Val=-1,int16 Mag=-1,rID eID=0, int8 clev=0);
system void   T_THING::GainTempStati(int16 n,hObj:T_THING h, int16 Duration, int8 Cause, int16 Val=-1,int16 Mag=-1,rID eID=0, int8 clev=0);
system void   T_THING::CopyStati(hObj:T_THING h, int16 n);
system void   T_THING::RemoveStati(int16 n, int8 Cause=-1, int16 val=-1, int16 Mag=-1, hObj:T_THING h = 0);
system void   T_THING::RemoveEffStati(rID xID);
system void   T_THING::RemoveStatiFrom(hObj:T_THING);
system void   T_THING::RemoveStatiSource(uint8 src);
system int16  T_THING::HighSourceCLev(uint8 src);
system bool   T_THING::HasStatiFromSource(uint8 src);
system bool   T_THING::HasStati(int16 n, int16 val=-1, hObj:T_THING t=0);
system bool   T_THING::HasEffStati(int16 n, rID xID, int16 Val=-1, hObj:T_THING t=0);
system bool   T_THING::HasEffField(rID eID);

system int8   T_THING::GetStatiVal(int16 n, hObj:T_THING t=0);
system int16  T_THING::GetStatiMag(int16 n, int16 val=-1, hObj:T_THING t=0);
system uint8  T_THING::GetStatiCause(int16 n, int16 val=-1, hObj:T_THING t=0);
system hObj   T_THING::GetStatiObj(int16 n, int16 val=-1, hObj:T_THING t=0);
system int16  T_THING::GetStatiDur(int16 n, int16 val=-1, hObj:T_THING t=0);
system rID    T_THING::GetStatiEID(int16 n, int16 val=-1, hObj:T_THING t=0);
system int16  T_THING::GetStatiCLev(int16 n,int16 val=-1, hObj:T_THING t=0);

system void   T_THING::SetStatiVal(int16 n, hObj:T_THING t,int16 val);
system void   T_THING::SetStatiMag(int16 n, int16 val, hObj:T_THING t,int16 Mag);
system void   T_THING::SetStatiCause(int16 n, int16 val, hObj:T_THING t,uint8 Cause);
system void   T_THING::SetStatiObj(int16 n, int16 val, hObj:T_THING t);
system void   T_THING::SetStatiDur(int16 n, int16 val, hObj:T_THING t, int16 Dur);
system void   T_THING::SetStatiEID(int16 n, int16 val, hObj:T_THING t,rID eID);
system void   T_THING::IncStatiMag(int16 n, int8 Cause,int16 Val, hObj:T_THING t, int16 Inc);
      

system int8   T_THING::GetEffStatiVal(int16 n, rID xID, int16 Val=-1, hObj:T_THING t=0);
system int16  T_THING::GetEffStatiMag(int16 n, rID xID, int16 Val=-1, hObj:T_THING t=0);
system uint8  T_THING::GetEffStatiCause(int16 n, rID xID, int16 Val=-1, hObj:T_THING t=0);
system hObj   T_THING::GetEffStatiObj(int16 n, rID xID, int16 Val=-1, hObj:T_THING t=0);
system int16  T_THING::GetEffStatiDur(int16 n, rID xID, int16 Val=-1, hObj:T_THING t=0);
system int16  T_THING::GetEffStatiCLev(int16 n,rID xID, int16 Val=-1, hObj:T_THING t=0);
system void   T_THING::SetEffStatiVal(int16 n, rID xID,int16 val, hObj:T_THING t=0);
system void   T_THING::SetEffStatiMag(int16 n, rID xID,int16 Mag, int16 Val=-1, hObj:T_THING t=0);
system void   T_THING::SetEffStatiCause(int16 n, rID xID,uint8 Cause, int16 Val=-1, hObj:T_THING t=0);
system void   T_THING::SetEffStatiObj(int16 n, rID xID, hObj:T_THING t, int16 Val=-1);
system void   T_THING::SetEffStatiDur(int16 n, rID xID, int16 Dur, int16 Val=-1, hObj:T_THING t=0);
system void   T_THING::IncEffStatiMag(int16 n,rID xID, int8 Cause,hObj:T_THING t,int16 Inc, int16 Val=-1);
      
system int16  T_THING::SumStatiMag(int16 n, int16 val=-1, hObj:T_THING t = 0);
system int16  T_THING::HighStatiMag(int16 n, int16 val=-1, hObj:T_THING t = 0);
system int16  T_THING::SumStatiVal(int16 n, hObj:T_THING t = 0);
system int16  T_THING::XORStatiVal(int16 n, hObj:T_THING t = 0);

system int16  T_THING::CountStati(int16 n, int16 val=-1, hObj:T_THING t=0);
system int16  T_THING::CountEffStati(int16 n, rID xID,int16 Val=-1);

/*
Needs a special typecast:
Currently Broken:
  hObj   T_THING::ProjectTo(int16,int16,int8 range=1000);
  hObj   T_THING::ProjectDir(int32 dir,int8 range=1000);
*/

/*****************************************************************************
 *                               Class Creature                              *
 *****************************************************************************/

/*
  Not Needed: Dump(), Event, DoTurn, MMFeats, various action functions

  Not Usable: Attr[] (array), AttrAdj[] (static array), other static CalcValues
    member variables, DmgVal (returns Dice&), FirstAttk, NextAttk, PeekAttk,
    GetAttk;
*/

system rID    T_CREATURE::mID;
system rID    T_CREATURE::tmID;
system int16  T_CREATURE::PartyID;
system int16  T_CREATURE::cHP;
system int16  T_CREATURE::mHP;
system int16  T_CREATURE::Subdual;
system int16  T_CREATURE::cFP;
system int32  T_CREATURE::mMana;
system int32  T_CREATURE::uMana;
system int32  T_CREATURE::hMana;
system int32  T_CREATURE::cMana();
system int32  T_CREATURE::tMana();
system int32  T_CREATURE::nhMana();
system void   T_CREATURE::LoseMana(int32 amt, bool hold=false);
system void   T_CREATURE::GainMana(int32 amt);
system int8  T_CREATURE::AoO;
system int8  T_CREATURE::FFCount;
system int16  T_CREATURE::StateFlags;

system int32  T_CREATURE::AttackMode();
system int32  T_CREATURE::GetAttr(int8 att);
system int32  T_CREATURE::KnownAttr(int8 att);
system void   T_CREATURE::Exercise(int16 att, int16 amt, int16 col, int16 cap);
system void   T_CREATURE::Abuse(int16 att, int16 amt);
system bool   T_CREATURE::isBlind(); 

system void   T_CREATURE::ChooseAction();
system void   T_CREATURE::ExtendedAction();
// system void   T_CREATURE::HaltAction(bool force_halt = true);
system int32  T_CREATURE::ChallengeRating(bool allow_neg = false);
system void   T_CREATURE::GainXP(int32);
system void   T_CREATURE::KillXP(hObj:T_CREATURE);
system int32  T_CREATURE::XPDrained();
system void   T_CREATURE::RestoreXP(int32 xp);
system void   T_CREATURE::DevourMonster(hObj:T_MONSTER);
system uint16 T_CREATURE::Perceives(hObj:T_THING);
system bool   T_CREATURE::isBeside(hObj:T_THING);
system bool   T_CREATURE::isSmallRace();
system bool   T_CREATURE::ItemPrereq(rID xID, int16 ReqLevel, int16 TrickDC);
system bool   T_CREATURE::yn(string msg,bool defc=true);
system bool   T_CREATURE::canTalk();
system int32  T_CREATURE::ArmourType();
system int32  T_CREATURE::Encumbrance();
system bool   T_CREATURE::HasAttk(int8 att);
system bool   T_CREATURE::HasMFlag(int16 mf);
system bool   T_CREATURE::HasFeat(int16 ft);
system bool   T_CREATURE::HasAbility(int16 ft);
system bool   T_CREATURE::IHasFeat(int16 ft);
system bool   T_CREATURE::IHasAbility(int16 ft);
system int16  T_CREATURE::AbilityLevel(int16 n);
system int16  T_CREATURE::SkillLevel(int16 n);
system int16  T_CREATURE::ISkillLevel(int16 n);
system int16  T_CREATURE::SkillCheck(int16 sk, int16 DC, bool show, int16 retry=0);
system int8   T_CREATURE::WepSkill(rID wID);
system void   T_CREATURE::CalcValues();
system void   T_CREATURE::CalcHP();
system int16  T_CREATURE::Mod(int8 a);
system int16  T_CREATURE::Mod2(int8 a);
system int16  T_CREATURE::KMod(int8 a);
system int16  T_CREATURE::KMod2(int8 a);
system int8   T_CREATURE::SkillAttr(int16 sk);
system bool   T_CREATURE::SavingThrow(int16 st, int16 SaveDC, int32 subtypes=0);
system void   T_CREATURE::TrainAs(rID cID);
system int16  T_CREATURE::HungerState();
system hObj   T_CREATURE::ConsumeRope(uint32 feet);
system bool   T_CREATURE::LoseFatigue(int16 amt, bool avoid = false);
system void   T_CREATURE::Shapeshift(rID mID, bool merge = false, hObj:T_ITEM PolySource = 0);
system int16  T_CREATURE::ResistLevel(int16 DType);
system int16  T_CREATURE::SpellRating(rID eID,int32 mm=0,bool perceived=false);
system int16  T_CREATURE::CasterLev();
system rID    T_CREATURE::getGod();
system int16  T_CREATURE::GetAgeCatagory();
system void   T_CREATURE::DropAll();
system hObj   T_CREATURE::InSlot(int8 sl);
system hObj   T_CREATURE::FirstInv();
system hObj   T_CREATURE::NextInv();
system bool   T_CREATURE::HasSkill(int16 sk);     
system bool   T_CREATURE::HasInnateSpell(rID spID);
system int16  T_CREATURE::GetBAB(int16 mode);
system hObj   T_CREATURE::FindBestItemOrCreate(rID iID);     
system void   T_CREATURE::GainItem(hObj:T_ITEM it,bool st=false);
system void   T_CREATURE::IdentByTrial(hObj:T_ITEM it);
system void   T_CREATURE::GainInherentBonus(int16 attr, int16 mag, bool msg=true);
system void   T_CREATURE::Reveal(bool intentional = false);
system void   T_CREATURE::ProvokeAoO();
//system void   T_CREATURE::ProvokeAoO(hObj:T_CREATURE);
system void   T_CREATURE::ManeuverCheck(hObj:T_EVENTINFO);
system bool   T_CREATURE::isHostileTo(hObj:T_CREATURE);
system bool   T_CREATURE::isHostileToPartyOf(hObj:T_CREATURE);
system bool   T_CREATURE::isFriendlyTo(hObj:T_CREATURE);
system bool   T_CREATURE::isThreatened();
system hObj   T_CREATURE::getLeader();
system hObj   T_CREATURE::getLeaderOrFirst();
system bool   T_CREATURE::isLedBy(hObj:T_CREATURE);
system int16  T_CREATURE::worstTrouble();
system void   T_CREATURE::Transgress(rID gID, int16 mag, bool doWrath, string reason);
system void   T_CREATURE::IdentifyMon();
system void   T_CREATURE::IdentifyTemp(rID tID);
system bool   T_CREATURE::isMType(int32 mt);
system bool   T_CREATURE::isAerial();
system bool   T_CREATURE::isFlatFooted();
system int8   T_CREATURE::ChoicePrompt(string msg, string choices, int8 df=0, int8 col1=BROWN, int8 col2=YELLOW);
system void   T_CREATURE::AddTemplate(rID tID);
system bool   T_CREATURE::CanAddTemplate(rID tID);
system void   T_CREATURE::Awaken();
system int16  T_CREATURE::getSocialMod(hObj:T_CREATURE cr, bool inc_str);
system void   T_CREATURE::MakeNoise(int16 radius);
system void   T_CREATURE::Multiply(int16 count,bool split, bool msg=true); 
system void   T_CREATURE::StatiMessage(int16 n, int16 val=0, bool ending=0);
system hObj   T_CREATURE::RandInv();
system hObj   T_CREATURE::RandGoodInv(int32 minval);

system int8   T_CREATURE::SightRange;
system int8   T_CREATURE::LightRange;
system int8   T_CREATURE::ShadowRange;
system int8   T_CREATURE::InfraRange;
system int8   T_CREATURE::TremorRange;
system int8   T_CREATURE::BlindRange;
system int8   T_CREATURE::TelepRange;
system int8   T_CREATURE::PercepRange;

system int8   T_CREATURE::rateMeleeWeapon();
system int8   T_CREATURE::rateRangedWeapon();
system hObj   T_CREATURE::getPrimaryMelee();
system hObj   T_CREATURE::getPrimaryRanged();

system void   T_CREATURE::HaltAction(string why, bool force_halt);

/*****************************************************************************
 *                              Class Character                              *
 *****************************************************************************/

/* Very few unique new functions make it over for Characters. Most
   functions are either for internal use only, subject to change
   soon or virtual functions declared earlier in base classes. */

system int32  T_CHARACTER::kcMana();
system int32  T_CHARACTER::ktMana();

system int32  T_CHARACTER::Proficiencies;
system rID    T_CHARACTER::GodID;

system bool   T_CHARACTER::Swap(int8 sl);
system void   T_CHARACTER::AddAbilities(rID xID,int16 lv);
system void   T_CHARACTER::GainFeat(int16 ft);
system void   T_CHARACTER::GainAbility(int16 ab, int32 pa, rID sourceID,int16 statiSource);
system void   T_CHARACTER::GainBonusSkills(rID cID, int16 sp);
system void   T_CHARACTER::FeatPrereq(int16 ft);
system int32  T_CHARACTER::NextLevXP();
system int16  T_CHARACTER::XPPenalty();  
system void   T_CHARACTER::LoseXP(uint32 amt);
system int32  T_CHARACTER::TotalXP();
system int16  T_CHARACTER::IAttr(int8 a);
system int16  T_CHARACTER::LevelAs(rID cID);
system int16  T_CHARACTER::TotalLevel();
system int32  T_CHARACTER::SpellKnown(int16 spNum);
system uint16 T_CHARACTER::getSpellFlags(rID spID);
system void   T_CHARACTER::setSpellFlags(rID spID, uint16 fl);
system int16  T_CHARACTER::GetSkillRanks(int16 sk);
system void   T_CHARACTER::SetInvSlot(int16 sl, hObj:T_ITEM it);


  /* CasterLev() et al. awaiting revision */

system void   T_CHARACTER::UseFeat(int16 ft);
system void   T_CHARACTER::UseSkill(uint8 sk);
system void   T_CHARACTER::UseAbility(uint8 ab,int16 pa);

system void   T_CHARACTER::GodMessage(rID gID, int16 msg,...);
system int32  T_CHARACTER::calcFavour(rID gID);
system void   T_CHARACTER::gainFavour(rID gID, int32 amt, bool advance=false, bool stack=true);
system void   T_CHARACTER::lowerAnger(rID gID, int16 amt);
system void   T_CHARACTER::AlignedAct(uint16 type, int16 mag, string reason);

system uint16 T_CHARACTER::getGodFlags(rID gID);
system int16  T_CHARACTER::getGodLevel(rID gID);
system int16  T_CHARACTER::getGodAnger(rID gID);
system void   T_CHARACTER::setGodFlags(rID gID, uint16 fl);
system void   T_CHARACTER::resetGodFlags(rID gID, uint16 fl);
system void   T_CHARACTER::Forsake();
system bool   T_CHARACTER::isWorthyOf(rID gID, bool penanceOK);
system int16  T_CHARACTER::getAnger(rID gID);
system void   T_CHARACTER::SwapAttributes(int16 n);
system int32  T_CHARACTER::TotalExploreFavour();

system bool   T_MONSTER::MakeCompanion(hObj:T_PLAYER p,int16 CompType);
system void   T_MONSTER::Pacify(hObj:T_CREATURE);
system void   T_MONSTER::TurnHostileTo(hObj:T_CREATURE);
system void   T_MONSTER::TurnNeutralTo(hObj:T_CREATURE);

system rID    T_PLAYER::RaceID;
system bool   T_PLAYER::WizardMode;

system void   T_PLAYER::GainSpell(rID sID, int16 fl);

system int16  T_PLAYER::FirstAutoBuff();
system int16  T_PLAYER::NextAutoBuff();
system int16  T_PLAYER::SpendHours(int16 minHours, int16 maxHours);
system int32  T_PLAYER::formulaSeed;
system int16  T_PLAYER::FixSummonCR(int16 enCR, int16 CompType=PHD_MAGIC);
system int16  T_PLAYER::getMaxDepth(rID dID);

/*****************************************************************************
 *                               Class Player                                *
 *****************************************************************************/

/* Again, the vast majority of the Player class's functionality
   is hidden in virtual functions and internal calls. The idea is
   to keep the player as similar to NPCs in terms of game systems
   as possible, after all. Here are a few functions to handle the
   special cases that apply only to a player. */

system string  T_PLAYER::BonusBreakdown(int8 a, int16 maxlen);
system bool    T_PLAYER::Seen(int16 x, int16 y);
system bool    T_PLAYER::Opt(int16 option);

/*****************************************************************************
 *                               Class Monster                               *
 *****************************************************************************/

system void    T_MONSTER::AddAct(int8 act, int8 pri, hObj:T_THING tar = 0, int16 subval = 0);
// system int16   T_MONSTER::RateAsTarget(hObj:T_THING t);
// system void    T_MONSTER::Retarget();

/*****************************************************************************
 *                               Class Item                                  *
 *****************************************************************************/

/* Not Needed: SetOwner() */

system rID     T_ITEM::iID;
system rID     T_ITEM::ieID;
system int16   T_ITEM::IFlags;
system int16   T_ITEM::Age;

system hObj    T_ITEM::Owner();
system int16   T_ITEM::Size(hObj:T_CREATURE wielder=0);
system hObj    T_ITEM::GetParent();
system bool    T_ITEM::isActivatable();
system void    T_ITEM::ChangeIID(rID xID);
system bool    T_ITEM::allowedSlot(int16 sl, hObj:T_CREATURE);
system bool    T_ITEM::activeSlot(int16 sl);
system int8    T_ITEM::GetCharges();
system void    T_ITEM::SetCharges(int8 ch);
system void    T_ITEM::MakeKnown(uint8 k);
system void    T_ITEM::SetKnown(uint8 k);
system bool    T_ITEM::isEdible(hObj:T_CREATURE);
system bool    T_ITEM::isMagic();
system void    T_ITEM::MakeMagical(rID eID, int16 plus=0);
system int16   T_ITEM::Weight();          /* == Item::Weight() */
system bool    T_ITEM::HasIFlag(uint8 fl);
system bool    T_ITEM::useStrength();
system bool    T_ITEM::HasSpell(int16 sp);
system bool    T_ITEM::HasQuality(int16 q);
system bool    T_ITEM::KnownQuality(int16 q);
system bool    T_ITEM::QualityOK(int8 q,uint8 lv=200);
system void    T_ITEM::AddQuality(int8 q, int16 param=0);
system void    T_ITEM::RemoveQuality(int8 q); 
system void    T_ITEM::PurgeAllQualities();
system int16   T_ITEM::ItemLevel();
system bool    T_ITEM::isGroup(uint32 gr);
system rID     T_ITEM::GetCorpseType();
system int16   T_ITEM::GetLightRange();
system uint16  T_ITEM::GetQuantity();
system void    T_ITEM::SetQuantity(uint16 q);
system hObj    T_ITEM::TakeOne();
system hObj    T_ITEM::TryStack(hObj:T_ITEM);
//system hObj    T_ITEM::TryStack(hObj:T_MAP,int16 x,int16 y);
system bool    T_ITEM::isBlessed();
system bool    T_ITEM::isCursed();
system bool    T_ITEM::isMaster();
system bool    T_ITEM::isKnown(int16 kn);
system void    T_ITEM::ReApply();
system int8    T_ITEM::GetPlus();
system int8    T_ITEM::GetInherentPlus();
system void    T_ITEM::SetInherentPlus(int8 newPlus);
system int16   T_ITEM::ArmVal(int16 ty);
system int16   T_ITEM::ArmourPenalty(bool for_skill);  /* == Item::Penalty() */
system int16   T_ITEM::Hardness(int8 DType);
system int16   T_ITEM::MaxHP();
system void    T_ITEM::MendHP(int16 hp);
system int16   T_ITEM::GetHP();
system void    T_ITEM::VisibleID(hObj:T_CREATURE);

system bool    T_ITEM::isBaneOf(int16 MT);
system void    T_ITEM::SetBane(int16 b);
system int16   T_ITEM::GetBane();

system hObj    T_CONTAIN::getItem(int16 n);

system int8    T_DOOR::DoorFlags;
system uint8   T_TRAP::TrapFlags;
system rID     T_TRAP::tID; 
system EvReturn T_TRAP::TriggerTrap(hObj:T_EVENTINFO, bool found_before=false);
system int16   T_TRAP::TrapLevel();
system rID     T_FEATURE::fID;

system bool    T_PORTAL::EnterDir(int16 dir);

/*****************************************************************************
 *                               Class Term                                  *
 *****************************************************************************/

/* Since the terminal API isn't remotely finalized yet, and isn't
   really strongly necessary in most scripts, we give only a few
   rudimentry function calls here. */

system void    T_TERM::StopWatch(int16 milli);
system void    T_TERM::SetMode(int16 mode);
system int16   T_TERM::GetMode();
//system int16   T_TERM::SizeX();
//system int16   T_TERM::SizeY();
system int16   T_TERM::OffsetX();
system int16   T_TERM::OffsetY();
system void    T_TERM::SetWin(int16 win);
system int16   T_TERM::GetCharRaw();
system void    T_TERM::PutChar(int16 x,int16 y, int32 g);
system void    T_TERM::PutGlyph(int16 x,int16 y, int32 g);
system int32   T_TERM::GetGlyph(int16 x,int16 y);
system void    T_TERM::GotoXY(int16 x, int16 y);
system void    T_TERM::Clear();
system void    T_TERM::SetColor(int16 col);
system void    T_TERM::Write(string msg);
system void    T_TERM::WriteXY(int16 x,int16 y, string msg);
system void    T_TERM::CursorOn();
system void    T_TERM::CursorOff();
system hObj    T_TERM::ChooseTarget(String prompt,int16 rType = 0);
system bool    T_TERM::EffectPrompt(hObj:T_EVENTINFO,int16 qv,bool islook, String str);
system hObj    T_TERM::AcquisitionPrompt(int8 Reason, int8 minlev, int8 maxlev, int8 MType);
system string  T_TERM::StringPrompt(int8 col, string msg);
system rID     T_TERM::ChooseResource(string prompt, int16 RType, rID eID);
system void    T_TERM::LOption(string txt, int16 val, string des=0);
system void    T_TERM::LOptionClear();
system int32   T_TERM::LMenu(int16 fl, string title, int8 MWin=WIN_MENUBOX,string help=0);
system bool    T_TERM::LMultiSelect(int16 fl, string title, int8 MWin=WIN_MENUBOX,string help=0);
system int32   T_TERM::FirstSelected();
system int32   T_TERM::NextSelected();
system void    T_TERM::HelpTopic(string topic);
system void    T_TERM::RefreshMap();
system void    T_TERM::TermUpdate();
system void    T_TERM::Box(string msg);
system void    T_TERM::InventoryManager(bool first, hObj:T_CONTAIN theChest=0);

/*
system int16   T_TERM::WinTop();
system int16   T_TERM::WinBottom();
system int16   T_TERM::WinLeft();
system int16   T_TERM::WinRight();
system int16   T_TERM::WinSizeX();
system int16   T_TERM::WinSizeY();
*/

system rID     T_GAME::GetEffectID(int16 pur, int8 minlev, int8 maxlev, int8 Source=0);
system rID     T_GAME::GetMonID(int16 pur, int8 minlev, int8 maxlev, int8 Depth, int16 MType=-1);
system rID     T_GAME::GetItemID(int16 pur, int8 minlev, int8 maxlev, int8 IType=-1);
system rID     T_GAME::GetMapID(int16 Purpose, uint8 depth, int8 MType=-1);
system rID     T_GAME::GetTempID(uint16 Types,rID mID, int8 MaxCR);

system hObj    T_GAME::GetPlayer(int8 n);
system void    T_GAME::EnterLimbo(hObj h, uint8 x, uint8 y, rID mID, int8 Depth,
                 int8 OldDepth, int32 Arrival, String msg);

system int16   T_GAME::SpellNum(rID spID);
system rID     T_GAME::SpellID(int16 spNum);
system int16   T_GAME::LastSpell();

system int32   T_GAME::GetTurn();

/*****************************************************************************
 *                             Struct EventInfo                              *
 *****************************************************************************/

system void    T_EVENTINFO::SetParam(int16 i,hObj:T_THING t);

system int16   T_EVENTINFO::Event;
system int16   T_EVENTINFO::EXVal;
system int16   T_EVENTINFO::EYVal;
system int16   T_EVENTINFO::ESpellNum;
system int16   T_EVENTINFO::EParam;
system int16   T_EVENTINFO::EDir;
system int8    T_EVENTINFO::vRoll;
system int8    T_EVENTINFO::AType;
system int8    T_EVENTINFO::DType;
system int8    T_EVENTINFO::vHit;
system int8    T_EVENTINFO::vDef;
system int8    T_EVENTINFO::vThreat;
system int8    T_EVENTINFO::vCrit;
system int8    T_EVENTINFO::vPenetrateBonus;
system int8    T_EVENTINFO::vRange;
system int8    T_EVENTINFO::vRadius;
system int16   T_EVENTINFO::vDuration;
system int8    T_EVENTINFO::vCasterLev;
system int8    T_EVENTINFO::vAlchemy;
system int8    T_EVENTINFO::vChainCount;
system int8    T_EVENTINFO::vChainMax;
system int8    T_EVENTINFO::efNum;
system int8    T_EVENTINFO::saveDC;
system int16   T_EVENTINFO::vDmg;
system int16   T_EVENTINFO::bDmg;
system int16   T_EVENTINFO::aDmg;
system int16   T_EVENTINFO::xDmg;
system int16   T_EVENTINFO::vMult;
system int16   T_EVENTINFO::sp;
system rID     T_EVENTINFO::eID;
system bool    T_EVENTINFO::isHit;
system bool    T_EVENTINFO::isCrit;
system bool    T_EVENTINFO::isFumble;
system bool    T_EVENTINFO::isBypass;
system bool    T_EVENTINFO::Died;
system bool    T_EVENTINFO::ADied;
system bool    T_EVENTINFO::Blocked;
system bool    T_EVENTINFO::Saved;
system bool    T_EVENTINFO::Immune;
system bool    T_EVENTINFO::Resist;
system bool    T_EVENTINFO::wasFriendly;
system bool    T_EVENTINFO::actUnseen;
system bool    T_EVENTINFO::vicUnseen;
system bool    T_EVENTINFO::Whirlwind;
system bool    T_EVENTINFO::Graze;
system bool    T_EVENTINFO::Stun;
system bool    T_EVENTINFO::actIncor;
system bool    T_EVENTINFO::vicIncor;
system bool    T_EVENTINFO::Ranged;
//system bool    T_EVENTINFO::Spell;
system bool    T_EVENTINFO::vicHeld;
system bool    T_EVENTINFO::isDir;
system bool    T_EVENTINFO::isLoc;
system bool    T_EVENTINFO::isActivation;
system bool    T_EVENTINFO::isSomething;
system bool    T_EVENTINFO::isWield;
system bool    T_EVENTINFO::isRemove;
system bool    T_EVENTINFO::isPeriodic;
system bool    T_EVENTINFO::isAoO;
system bool    T_EVENTINFO::isCleave;
system bool    T_EVENTINFO::isSurprise;
system bool    T_EVENTINFO::isFlatFoot;
system bool    T_EVENTINFO::isFlanking;
system bool    T_EVENTINFO::isGhostTouch;
system bool    T_EVENTINFO::actIllusion;
system bool    T_EVENTINFO::vicIllusion;
system bool    T_EVENTINFO::effIllusion;
system bool    T_EVENTINFO::illType;
system bool    T_EVENTINFO::isSeeking;
system bool    T_EVENTINFO::isFirstBlastXY;
system bool    T_EVENTINFO::isRepeatSpell;
system bool    T_EVENTINFO::isEBlessed;
system bool    T_EVENTINFO::isECursed;
system bool    T_EVENTINFO::isConf;
system bool    T_EVENTINFO::isHallu;
system bool    T_EVENTINFO::isItem;
system bool    T_EVENTINFO::isTrap;
system bool    T_EVENTINFO::isActOfGod;
system bool    T_EVENTINFO::isOffhand;
system bool    T_EVENTINFO::isEvaded;
system bool    T_EVENTINFO::isSneakAttack;
system bool    T_EVENTINFO::isWildMiss;
system bool    T_EVENTINFO::ignoreHardness;
system bool    T_EVENTINFO::halfHardness;
system bool    T_EVENTINFO::Silence;
system bool    T_EVENTINFO::Terse;
system uint32  T_EVENTINFO::MM;

system int16   T_EVENTINFO::vDepth;
system int16   T_EVENTINFO::vLevel;
system int16   T_EVENTINFO::vAlign;

system string  T_EVENTINFO::GraveText;
system string  T_EVENTINFO::strDmg;
system string  T_EVENTINFO::strXDmg;
system string  T_EVENTINFO::strHit;
system string  T_EVENTINFO::strDef;
system string  T_EVENTINFO::strOpp1;
system string  T_EVENTINFO::strOpp2;
system string  T_EVENTINFO::strBlastDmg;

system string  T_EVENTINFO::nPrefix;
system string  T_EVENTINFO::nCursed;
system string  T_EVENTINFO::nPrequal;
system string  T_EVENTINFO::nPostqual;
system string  T_EVENTINFO::nNamed;
system string  T_EVENTINFO::nBase;
system string  T_EVENTINFO::nAppend;
system string  T_EVENTINFO::nOf;
system string  T_EVENTINFO::nAdjective;
system string  T_EVENTINFO::nFlavour;
system string  T_EVENTINFO::nInscrip;
system string  T_EVENTINFO::nMech;
system string  T_EVENTINFO::nArticle;
system string  T_EVENTINFO::nPlus;
system string  T_EVENTINFO::Text;

/* These actually represent the EffectValues struct that
   EventInfo has a pointer to in the C++ core code. We
   treat them as normal members here because the script
   engine can't handle non-Object devived structs well. 
   We also can't access pval directly... yet. */
system int8    T_EVENTINFO::eval;
system int8    T_EVENTINFO::dval;
system int8    T_EVENTINFO::aval;
system int8    T_EVENTINFO::tval;
system int8    T_EVENTINFO::qval;
system int8    T_EVENTINFO::sval;
system int8    T_EVENTINFO::lval;
system int8    T_EVENTINFO::cval;
system uint8   T_EVENTINFO::xval;
system int16   T_EVENTINFO::yval;
system rID     T_EVENTINFO::rval;
/*system void    T_EVENTINFO::SetEFlag(int16 fl);
system void    T_EVENTINFO::UnsetEFlag(int16 fl);
system bool    T_EVENTINFO::HasEFlag(int16 fl);
system Dice    T_EVENTINFO::pval; */

system Rect    T_EVENTINFO::cPanel;
system Rect    T_EVENTINFO::cMap;
system Rect    T_EVENTINFO::cRoom;

system int32   T_EVENTINFO::vVal;
system int32   T_EVENTINFO::vMag;
system hObj    T_EVENTINFO::vObj;

system int16   T_EVENTINFO::cPart;
system int16   T_EVENTINFO::enAlign;
system int16   T_EVENTINFO::enCR;
system int16   T_EVENTINFO::enDepth;
system int16   T_EVENTINFO::enFreaky;
system int16   T_EVENTINFO::enPurpose;
system int16   T_EVENTINFO::enSleep;
system uint32  T_EVENTINFO::enTerrain;
system int16   T_EVENTINFO::enDesAmt;
system int16   T_EVENTINFO::enType;
system int16   T_EVENTINFO::epMinAmt;
system int16   T_EVENTINFO::epMaxAmt;
system int16   T_EVENTINFO::epAmt;
system int16   T_EVENTINFO::epFreaky;
system int16   T_EVENTINFO::epWeight;
system int16   T_EVENTINFO::epMType;
system int16   T_EVENTINFO::epSkillRoll;
system int16   T_EVENTINFO::epClassRoll;
system int16   T_EVENTINFO::epCurrXCR;
system rID     T_EVENTINFO::enID;
system rID     T_EVENTINFO::ep_mID;
system rID     T_EVENTINFO::ep_tID;
system rID     T_EVENTINFO::ep_tID2;
system rID     T_EVENTINFO::ep_tID3;
system rID     T_EVENTINFO::ep_hmID;
system rID     T_EVENTINFO::ep_htID;
system rID     T_EVENTINFO::ep_htID2;
system rID     T_EVENTINFO::ep_iID;
system rID     T_EVENTINFO::ep_pID;
system int32   T_EVENTINFO::enXCR;
system int32   T_EVENTINFO::epXCR;
system int32   T_EVENTINFO::eimXCR;
system int32   T_EVENTINFO::enFlags;
system int32   T_EVENTINFO::enConstraint;
system bool    T_EVENTINFO::chMaximize;
system bool    T_EVENTINFO::chBestOfTwo;
system int16   T_EVENTINFO::chList;
system rID     T_EVENTINFO::chSource;
system rID     T_EVENTINFO::chResult;
system bool    T_EVENTINFO::isAquaticContext;
          
/*****************************************************************************
 *                             Resource Functions                            *
 *****************************************************************************/

system bool    isResType(rID xID, int16 rt);

system int32   T_TDUNGEON::GetConst(int16 cn);

system uint16  T_TMONSTER::Image;
system int8    T_TMONSTER::CR;
system int8    T_TMONSTER::Depth;
system uint32  T_TMONSTER::Terrains;
system int8    T_TMONSTER::Hit;
system int8    T_TMONSTER::Def;
system int8    T_TMONSTER::Arm;
system int8    T_TMONSTER::Mov;
system int8    T_TMONSTER::Spd;
system int8    T_TMONSTER::MSize;
//system void    T_TMONSTER::GrantGear(hObj:T_CREATURE mn, rID xID, bool doRanged);
// system bool    T_TMONSTER::isMType(rID mID, int32 mt);

system uint16  T_TITEM::Image;
system int16   T_TITEM::RIType;

system uint16  T_TFEATURE::Image;

system uint32  T_TEFFECT::Schools;
system bool    T_TEFFECT::HasSource(int8 src);
system uint32  T_TEFFECT::Purpose;
system int16   T_TEFFECT::Level;
system bool    T_TEFFECT::HasFlag(int16 tf);

system uint16  T_TTERRAIN::Image;
/* ww: this shouldn't be necessary, but currently IncursionScript can't
 * handle the name collision */
system bool    T_TTERRAIN::TerrainHasFlag(int16 tf);

system rID     T_TREGION::Floor;
system rID     T_TREGION::Walls;

system rID     T_TRACE::BaseRace;

system rID     T_TTEMPLATE::TType;

/*****************************************************************************
 *                              String Functions                             *
 *****************************************************************************/


system String  Capitalize(String s, bool all=false);
system String  Replace(String str, String find, String rep);

system String  T_STRING::Capitalize(bool all=false);
system String  T_STRING::Replace(String find, String rep);
system void    T_STRING::Empty();
system int32   T_STRING::strchr(int8 ch);
system int32   T_STRING::GetLength();
system int32   T_STRING::GetTrueLength();
system String  Left(String s, int32 sz);
system String  T_STRING::Left(int32 sz); 
system String  Right(String s, int32 sz);
system String  T_STRING::Right(int32 sz); 
system String  Mid(String s, int32 start, int32 end);
system String  T_STRING::Mid(int32 start, int32 end); 
system String  Trim(String s);
system String  T_STRING::Trim(); 
system String  Upto(String s, String chlist);
system String  T_STRING::Upto(String chlist);
system String  Upper(String s);
system String  T_STRING::Upper(); 
system String  Lower(String s);
system String  T_STRING::Lower(); 
system String  Decolorize(String s);
system String  T_STRING::Decolorize(); 

system String  Format(String fmt,...);
system String  XPrint(String str,...);
system String  Pluralize(String str, rID iID=0);

system String  NAME(rID xID);
system String  DESC(rID xID);
system String  SkillName(int16 sk);
system String  DescribeSkill(int16 sk);

system void    PrintRect(Rect r);
system bool    WithinRect(Rect r, int16 x, int16 y);

/*****************************************************************************
 *                             Global Functions                              *
 *****************************************************************************/

system int16   dist(int16, int16, int16, int16);
system int16   random(int16 mx);                /* == random(n) */
system hObj    GetHandle(hObj:T_THING h);
system bool    isValidHandle(hObj h);

system void    DPrint(hObj:T_EVENTINFO, string msg1, string msg2,...);
system void    VPrint(hObj:T_EVENTINFO, string msg1, string msg2,...);
system void    TPrint(hObj:T_EVENTINFO, string msg1, string msg2, string msg3,...);
system void    APrint(hObj:T_EVENTINFO, string msg,...);
system void    SinglePrintXY(hObj:T_EVENTINFO, string msg,...);
//system void    BDPrint(hObj:T_EVENTINFO, string msg1,...);
//system void    BTPrint(hObj:T_EVENTINFO, string msg1, string msg2,...);
//system void    Hear(hObj:T_EVENTINFO, string msg1,...);
system int16   ResourceLevel(rID xID);

system void    Error(string msg1,...);
system void    Fatal(string msg1,...);

system int16   DirX(int16);
system int16   DirY(int16);

system bool    ResourceHasFlag(rID xID, int16 flagno);
system bool    mID_isMType(rID mID, uint32 mt);
system int16   PoisonDC(rID pID);

system int16   XCRtoCR(int32 XCR);
system int32   XCR(int16 CR);

system void    SetSilence();
system void    UnsetSilence();

system void SetETarget(hObj:T_EVENTINFO e, hObj:T_THING t);
system void SetEVictim(hObj:T_EVENTINFO e, hObj:T_CREATURE v);
system void SetEActor (hObj:T_EVENTINFO e, hObj:T_CREATURE a);
system void SetEItem  (hObj:T_EVENTINFO e, hObj:T_ITEM it);
system void SetEItem2 (hObj:T_EVENTINFO e, hObj:T_ITEM it);

system int32   nCandidates();
system int32   GetCandidate(int16 n);
system void    AddCandidate(int32 h);
system int32   RandCandidate();
system void    ClearCandidates();

system EvReturn Throw(int16 Ev, hObj:T_OBJECT=0,hObj:T_OBJECT=0,
                                hObj:T_OBJECT=0,hObj:T_OBJECT=0);

system EvReturn ThrowXY(int16 Ev, int16 x, int16 y, hObj:T_OBJECT=0,hObj:T_OBJECT=0,
                                hObj:T_OBJECT=0,hObj:T_OBJECT=0);

system EvReturn ThrowVal(int16 Ev,int16 n, hObj:T_OBJECT=0,hObj:T_OBJECT=0,
                                           hObj:T_OBJECT=0,hObj:T_OBJECT=0);

system EvReturn ThrowDir(int16 Ev,int8 dr, hObj:T_OBJECT=0,hObj:T_OBJECT=0,
                                          hObj:T_OBJECT=0,hObj:T_OBJECT=0);

system EvReturn ThrowEff(int16 Ev,rID eID, hObj:T_OBJECT=0,hObj:T_OBJECT=0,
                                           hObj:T_OBJECT=0,hObj:T_OBJECT=0);

system EvReturn ThrowEffDir(int16 Ev,rID eID,int8 dr, hObj:T_OBJECT=0,hObj:T_OBJECT=0,
                                                    hObj:T_OBJECT=0,hObj:T_OBJECT=0);

system EvReturn ThrowEffXY(int16 Ev,rID eID,int16 x,int16 y, hObj:T_OBJECT=0,
                            hObj:T_OBJECT=0, hObj:T_OBJECT=0,hObj:T_OBJECT=0);

system EvReturn ThrowLoc(int16 Ev,int16 x,int16 y, hObj:T_OBJECT=0,hObj:T_OBJECT=0,
                                                   hObj:T_OBJECT=0,hObj:T_OBJECT=0);

system EvReturn ThrowDmg(int16 Ev,int16 dt,int16 amt, string s,
                                hObj:T_OBJECT=0,hObj:T_OBJECT=0,
                                hObj:T_OBJECT=0,hObj:T_OBJECT=0);

system EvReturn ThrowTerraDmg(int16 Ev,int16 dt,int16 amt, string s,
                                hObj:T_OBJECT=0, rID terID); 

system EvReturn ThrowDmgEff(int16 Ev,int16 dt,int16 amt, string s, rID eID,
                                hObj:T_OBJECT=0,hObj:T_OBJECT=0,
                                hObj:T_OBJECT=0,hObj:T_OBJECT=0);

system EvReturn ReThrow(int16 ev, hObj:T_EVENTINFO);
system EvReturn RedirectEff(hObj:T_EVENTINFO,rID eID,int16 Ev=EV_EFFECT);

system hObj   CreateItem(rID iID);
system hObj   CreateMonster(rID mID);
system hObj   CreateFeature(rID fID);
system hObj   CreateDoor(rID fID);
system hObj   CreatePortal(rID fID);
system hObj   CreateTrap(rID fID, rID tID);
system hObj   GenDungeonItem(uint16 fl, rID xID, int16 Depth, int8 Luck);
system hObj   GenChestItem(uint16 fl, rID xID, int16 Depth, int8 Luck);
system hObj   InitShopkeeper(rID mID, int32 gold);
system void   SystemBreak();

system bool effectGivesStati(rID eID);

system int16  MMFeatLevels(uint32 MM);

system int32  LevelAdjust(int16 val, int16 level, int16 spec = 1);

system int16  LastSkillCheckResult();

system void    SetPVal(hObj:T_EVENTINFO e, int16 b, int16 n=0, int16 s=0);
 
 
/* functions with variable-type parameters handled by macro type-cast */
#define ItemPrereq(a,b,c) ItemPrereq((rID)(a),b,c)