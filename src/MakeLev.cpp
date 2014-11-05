/* MAKELEV.CPP -- Copyright (c) 1999-2003 Julian Mensch
     This file contains the dungeon generator and all the functions
   used (almost) exclusively by it.

     inline void Map::calcLight(Rect &r)
     inline bool Map::isTorched(uint8 x,uint8 y,int16 t)
     void Map::WriteAt(int16 x, int16 y,rID terID,rID regID,int8 Pri,bool Force)
     void Map::WriteRoom(Rect &r,rID regID)
     void Map::WriteBox(Rect &r,rID regID)
     void Map::WriteCircle(Rect &r, rID regID)
     void Map::WriteWalls(Rect &r,rID regID)
     void Map::WriteLifeCave(Rect &r, rID regID)
     void Map::GeneratePrompt()
     void Map::WriteCastle(Rect &r, rID regID)
     void Map::WriteRCavern(Rect &r, rID regID)
     void Map::WriteOctagon(Rect &r,rID regID)
     void Map::WriteDiamond(int16 x, int16 y,rID regID)
     void Map::WriteCross(Rect &r, rID regID)
     void Map::WriteDestroyed(Rect &r,rID regID)
     void Map::WriteMaze(Rect &r,rID regID)
     void Map::WriteStreamer(uint8 sx, uint8 sy, Dir d, rID regID)
     void Map::WriteBlobs(Rect &r, rID bID)
     void Map::WriteMap(Rect &r,rID regID)
     void Map::MakeDoor(uint8 x,uint8 y,rID fID)
     void Map::MakeSecretDoor(uint8 x,uint8 y,rID fID)
     void Map::Generate(rID _dID, int16 _Depth, Map *Above,int8 Luck)
     void Map::DrawPanel(uint8 px, uint8 py)
     void Map::PopulatePanel(Rect *r)
     uint16 Map::Tunnel(uint8 sx, uint8 sy, uint8 dx, uint8 dy, 
                         uint8 TFlags, Dir StartDir, int8 TType)
     Dir Map::CorrectDir(int16 x, int16 y, int16 dx,int16 dy,Dir Curr)
     int32 Map::FloodConnectA(int16 x, int16 y,int32 fCount)
     int32 Map::FloodConnectB(int16 x, int16 y)
     String Map::Describe(Player* p)

*/

#include "Incursion.h"

LocationInfo * GridBackup;
char GenText[8192];



Fraction Map::slope1, Map::slope2, Map::slope3, Map::test;
int32 Map::TotalCorridorLength;
int32 Map::RoomsTouched[32];
int32 Map::PanelsDrawn[32];
rID   Map::RoomWeights[1024];
rID   Map::CorridorWeights[1024];
rID   Map::VaultWeights[1024];
rID   Map::StreamWeights[1024];
uint8 Map::OpenX[2048], Map::OpenY[2048];
int16 Map::OpenC;

uint16 Map::Corners[32], Map::nCorners;
uint16 Map::Centers[16], Map::nCenters;

Rect Map::cPanel, Map::cRoom;
uint32 Map::Con[LAST_DUNCONST];
uint32 Map::RM_Weights[RM_LAST*2];
uint32 Map::RC_Weights[RC_LAST*2];
int8  Map::panelsX, Map::panelsY, Map::disX, Map::disY, Map::mLuck;
uint8 *Map::FloodArray, *Map::FloodStack, *Map::EmptyArray;
bool Map::IndividualRooms;

static rID usedInThisLevel[1024] = { 0, };

/* LocationInfo has a number of flags that start at zero universally
 * when the map is generated. We use these as temporary stores for
 * information as the map is being built, then zero them all when we
 * are finished. These defines thus just add coherancy to the code
 * with no overall effect.
 */

#define LifeFlag  isSkylight
#define Priority  Memory
#define Connected Visibility

/* Constants that determine when a tunnel should terminate: */
#define TT_CONNECT  0x01  /* Terminate when reach area where Connected 
                             flag differs from its value at sx/sy. */
#define TT_DIRECT   0x02  /* Always take the most direct route to dx/dy */
#define TT_LENGTH   0x04  /* Terminate if length exceeds given value. */
#define TT_NOTOUCH  0x08  /* Don't "touch" rooms */
#define TT_EXACT    0x10  /* Go to *exact* destination */
#define TT_WANDER   0x20  /* Chance to end after touching 2 rooms. */
#define TT_NATURAL  0x40  /* Curved, natural, non-horizontal tunnels */

Rect PopulateQueue[1024];
int16 cRectPop;

EvReturn Map::Event(EventInfo &e)
  {
    #define SEND_TO(xxx) \
    if (e.xxx) { \
      EvReturn res; \
      res = TENC(e.xxx)->Event(e,e.xxx); \
      if (res == DONE || res == ERROR || res == ABORT) \
        return res; \
      if (res == NOMSG) \
        e.Terse = true; \
      } 
      
    SEND_TO(enID)
    SEND_TO(ep_mID)
    SEND_TO(ep_tID)
    SEND_TO(ep_tID2)
    SEND_TO(ep_tID3)
      
    switch (e.Event)
      {
        case EV_ENGEN:
          return enGenerate(e);
        case EV_ENGEN_PART:
          return enGenPart(e);
        case EV_ENGEN_MOUNT:
          return enGenMount(e);
        case EV_ENBUILD_MON:
          return enBuildMon(e);
        case EV_ENCHOOSE_MID:
          return enChooseMID(e);
        case EV_ENCHOOSE_TEMP:
          return enChooseTemp(e);
        case EV_ENSELECT_TEMPS:
          return enSelectTemps(e);
        case EV_ENGEN_ALIGN:
          return enGenAlign(e);
        default:
          return NOTHING;
      }
  }

void Map::calcLight(Rect &r)
  {
    uint8 x,y;
    int16 t, l;
    if (r.x2 >= sizeX)
      r.x2 = (uint8)sizeX;
    for(x=r.x1;x<=r.x2;x++)
      for(y=r.y1;y<=r.y2;y++)
        At(x,y).Lit = TTER(TerrainAt(x,y))->HasFlag(TF_LOCAL_LIGHT);
    for (t=0;TorchList[t];t++)
      if (r.Within(TorchList[t] % 256, TorchList[t] / 256))
        for(x=r.x1;x<=r.x2;x++)
          for(y=r.y1;y<=r.y2;y++)
            if (!At(x,y).Lit)
              if (l = isTorched(x,y,t)) {
                At(x,y).Lit = true;
                /* This is way too terminal-dependant at the
                   moment. It will need to be rewritten in a
                   more device-independant way if Incursion
                   ever comes to support graphical tiles. */
                if (((At(x,y).Glyph & 0x00FF) == GLYPH_FLOOR) && At(x,y).Shade) {
                  switch (l) {
                    case 3:
                      At(x,y).Glyph = GLYPH_FLOOR + (YELLOW*256);
                      At(x,y).Bright = true;
                     break;
                    case 2:
                      if ((At(x,y).Glyph & 0x0F00) != (YELLOW*256))
                        At(x,y).Glyph = GLYPH_FLOOR + (BROWN*256);
                     break;
                    }
                  }
                else if ((At(x,y).Glyph & 0x00FF) == GLYPH_FLOOR && l == 3)
                  At(x,y).Glyph |= 0x0800;

                }


  }

int16 Map::isTorched(uint8 x,uint8 y,int16 t)
  {
    uint8 tx,ty,d;
    tx = TorchList[t] % 256;
    ty = TorchList[t] / 256;
    if (abs(x - tx) < 8)
      if (abs(y - ty) < 8)               
        if ((d=(uint8)dist(x,y,tx,ty)) < 8)
          if (LineOfVisualSight(x,y,tx,ty,NULL)) {
            if (d < 3)
              return 3;
            else if (d < 5)
              return 2;
            else
              return 1;
            }
            
    return false;
  }

void Map::LightPanel(Rect &r, rID regID)
{
  int16 Den, x, y; uint16 xy;
  TRegion *tr = TREG(regID);

  if (tr->HasFlag(RF_NEVER_LIT))
    return;

  if (random(100)+1 > (int16)(Con[ROOM_LIT_CHANCE] - Con[LIT_CHANCE_DEPTH_MOD]))
    if (!tr->HasFlag(RF_ALWAYS_LIT))
      return;

  Den = (int16)tr->GetConst(TORCH_DENSITY);
  if (!Den)
    Den = (int16)Con[TORCH_DENSITY];

  for (x = r.x1; x != r.x2; x++)
    for (y = r.y1; y != r.y2; y++)
      if (TerrainAt(x,y) == tr->Walls && SolidAt(x,y) && !FDoorAt(x,y))
        if (!random(Den)) {
          WriteAt(r,x,y,Con[TERRAIN_TORCH],regID,PRIO_ROOM_WALL_TORCH);
          xy = x+y*256;
          if (RegionAt(x,y) == Con[TERRAIN_TORCH])
            TorchList.Add(xy);
        }
}

int16 AdjustCR(int16 BaselineCR)
  {
    static int16 ExploreCR[] = 
      { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10 };
    static int16 TrainingCR[] = 
      { 0, 1, 1, 2, 3, 4, 4, 5, 6, 7, 8, 8, 9, 10, 11, 12, 12, 13, 14, 15, 16 };
    if (BaselineCR < 0)
      return BaselineCR;
    switch (theGame->GetPlayer(0)->Opt(OPT_DIFFICULTY))
      {
        case DIFF_EXPLORE:   return ExploreCR[min(20,BaselineCR)];
        case DIFF_TRAINING:  return ExploreCR[min(20,BaselineCR)];
        case DIFF_NIGHTMARE: return BaselineCR + 2;  
        default: return BaselineCR;
      }
  }
    
/* SinkPanel (to fill room area with water)
 *   -- elevation == steps in any Dir to wall
 *   -- some areas don't count; choose three such
 *      circles (at random) for radius

 * Underground Waterflow (sunken tunnel)

 */
void Map::WriteAt(Rect &r, int16 x, int16 y,rID terID,rID regID,
    int32 Pri,bool Force)
{                           
  static uint8 lastTerrain = 0, lastRegion = 0; 
  uint8 TerrainVal, RegionVal, i; TTerrain *t;
  uint16 TorchPos;

  if (!InBounds(x,y))
    return;
  t = TTER(terID);
  if (TTER(terID)->Type == T_TTERRAIN)
    ;
  else if (TTER(terID)->Type == T_TFEATURE) {
    WriteAt(r,x,y,FIND("floor"),regID,Pri,Force);
    Feature *ft = new Feature(TFEAT(terID)->Image,terID,T_FEATURE);
    if (ft)                                
      ft->PlaceAt(this,x,y);               
    return ;
  } else {
    ASSERT(0);
    return; 
  } 

  if (At(x,y).Priority > (uint32)Pri && !Force)
      return;
  if (x == 0 || y == 0 || x == sizeX -1 || y == sizeY - 1)
    if (!Force && Pri < PRIO_MAX && !(RES(dID)->Type == T_TREGION))
      return;

  t = TTER(terID);
  if (OpenC < 2048 && !t->HasFlag(TF_SOLID))
  {
    OpenX[OpenC] = (uint8)x;
    OpenY[OpenC] = (uint8)y;
    OpenC++;
  }

  /* Odd case, but not impossible: ex, writing over a water streamer
     with a vault, leaving the poor kraken struggling for breath on
     dry land. */
  if (FCreatureAt(x,y))
    if (TTER(terID)->HasFlag(TF_SOLID) || (FCreatureAt(x,y)->HasMFlag(M_AQUATIC)
          && !TTER(terID)->HasFlag(TF_WATER)))
      FCreatureAt(x,y)->Remove(true);

  if (TerrainList[lastTerrain] == terID)
    TerrainVal = lastTerrain;
  else {
    for(i=0;TerrainList[i];i++)
      if (TerrainList[i] == terID)
      { TerrainVal = i; goto FoundTerrain; }
    if (i == 255)
      Fatal("Too many Terrain types on one map!");
    TerrainVal = i;
    TerrainList[i++] = terID;
FoundTerrain:;
  }

  if (RegionList[lastRegion] == regID)
    RegionVal = lastRegion;
  else {
    for(i=0;RegionList[i];i++)
      if (RegionList[i] == regID)
      { RegionVal = i; goto FoundRegion; }
    if (i == 255)
      Fatal("Too many Regions on one map!");
    RegionVal = i;
    RegionList[i++] = regID;
FoundRegion:;
  }

  At(x,y).Glyph   = t->Image;
  At(x,y).Region  = RegionVal;
  At(x,y).Terrain = TerrainVal;
  At(x,y).Opaque  = t->HasFlag(TF_OPAQUE);
  At(x,y).Obscure = t->HasFlag(TF_OBSCURE);
  At(x,y).Solid   = t->HasFlag(TF_SOLID);
  At(x,y).Shade   = t->HasFlag(TF_SHADE);      
  At(x,y).Priority  = Pri;
  At(x,y).isWall  = t->HasFlag(TF_WALL);      

  // ww: summoning circles throw up magic fields, etc. 
  if (t) { 
    if (t->HasFlag(TF_SPECIAL)) {
      EventInfo e; 
      e.Clear();
      e.EMap = this;
      e.EXVal = x;
      e.EYVal = y;
      e.cPanel = r; 
      e.Event = EV_INITIALIZE;
      t->Event(e,terID);
    }
    if (t->HasFlag(TF_TORCH)) {
      TorchPos = x+y*256;
      TorchList.Add(TorchPos);
    }
  }
}

void Map::WriteRoom(Rect &r,rID regID)
  {
    rID WallID, FloorID;
    if (r.x2 - r.x1 <= 1)
      return;
    if (r.y2 - r.y1 <= 1)
      return;
    WallID  = TREG(regID)->Walls;
    FloorID = TREG(regID)->Floor;
    for(int16 x=r.x1;x<=r.x2;x++)
      for(int16 y=r.y1;y<=r.y2;y++)
        if (x == r.x1 || x == r.x2 || y == r.y1 || y == r.y2)
          WriteAt(r,x,y,WallID,regID,PRIO_ROOM_WALL);
        else
          WriteAt(r,x,y,FloorID,regID,PRIO_ROOM_FLOOR);
    Corners[nCorners++] = (r.x1+1) + (r.y1+1)*256;
    Corners[nCorners++] = (r.x2-1) + (r.y1+1)*256;
    Corners[nCorners++] = (r.x1+1) + (r.y2-1)*256;
    Corners[nCorners++] = (r.x2-1) + (r.y2-1)*256;

    Centers[nCenters++] = (r.x1 + (r.x2-r.x1)/2) +
                          (r.y1 + (r.y2-r.y1)/2) * 256; 
  }

void Map::WriteBox(Rect &r,rID regID)
  {
    rID FloorID;
    if (r.x2 - r.x1 <= 1)
      return;
    if (r.y2 - r.y1 <= 1)
      return;
    FloorID = TREG(regID)->Floor;
    for(uint8 x=r.x1;x<=r.x2;x++)
      for(uint8 y=r.y1;y<=r.y2;y++)
        WriteAt(r,x,y,FloorID,regID,PRIO_ROOM_FLOOR);
  }


void Map::WriteCircle(Rect &r, rID regID)
  {
    rID WallID, FloorID;
    uint8 radius, cx, cy, x, y;
    WallID  = TREG(regID)->Walls;
    FloorID = TREG(regID)->Floor;
    if (r.x2 - r.x1 <= 4)
      return;
    if (r.y2 - r.y1 <= 4)
      return;
    if (r.x2 - r.x1 > r.y2 - r.y1)
      radius = r.y2 - r.y1;
    else
      radius = r.x2 - r.x1;

    radius /= 2; 

    if (radius % 2)
      radius++;

    cx = r.x1 + (r.x2 - r.x1) / 2;
    cy = r.y1 + (r.y2 - r.y1) / 2;

    for (x=r.x1;x<r.x2+1;x++)
      for(y=r.y1;y<r.y2+1;y++)
        if (dist(x,y,cx,cy) <= radius)
          WriteAt(r,x,y,FloorID,regID,PRIO_ROOM_FLOOR);

    radius--;
    Centers[nCenters++] = cx + cy*256;
    Corners[nCorners++] = (cx+radius) + cy*256;
    Corners[nCorners++] = (cx-radius) + cy*256;
    Corners[nCorners++] = cx + (cy+radius)*256;
    Corners[nCorners++] = cx + (cy-radius)*256;

  }

void Map::WriteWalls(Rect &r,rID regID)
  {
    int16 x,y;
    rID FloorID, WallID;
    FloorID = TREG(regID)->Floor;
    WallID  = TREG(regID)->Walls;

    for (x=max(1,r.x1-1);x<min(r.x2+2,sizeX-1);x++)
      for(y=max(1,r.y1-1);y<min(r.y2+2,sizeY-1);y++)
        if (TerrainAt(x,y) != FloorID)
          if (TerrainAt(x,y+1) == FloorID || TerrainAt(x+1,y) == FloorID ||
            TerrainAt(x,y-1) == FloorID || TerrainAt(x-1,y) == FloorID ||
            TerrainAt(x+1,y+1) == FloorID || TerrainAt(x-1,y-1) == FloorID ||
            TerrainAt(x-1,y+1) == FloorID || TerrainAt(x+1,y-1) == FloorID)
              WriteAt(r,x,y,WallID,regID,PRIO_ROOM_WALL);
  }



void Map::WriteLifeCave(Rect &r, rID regID)
{
  uint8 x,y,i,j;
  rID FloorID, WallID;
  FloorID = TREG(regID)->Floor;
  WallID  = TREG(regID)->Walls;

  const int buffer = 3; 

  for (x=r.x1;x<=r.x2;x++)
    for (y=r.y1;y<=r.y2;y++)         
      At(x,y).Visibility = 1;

  for (x=r.x1+buffer;x<=r.x2-buffer;x++)
    for (y=r.y1+buffer;y<=r.y2-buffer;y++)         
      At(x,y).Visibility = (random(100) > (int16)Con[LIFE_PERCENT]-1) ? 0 : 1;

  for(i=0;i!=20;i++) {
    for (x=r.x1+buffer;x<=r.x2-buffer;x++)
      for (y=r.y1+buffer;y<=r.y2-buffer;y++)
      {
        j = At(x+1,y+1).Visibility +
          At(x+1,y)  .Visibility +
          At(x+1,y-1).Visibility +
          At(x,  y-1).Visibility +
          At(x-1,y-1).Visibility +
          At(x-1,y)  .Visibility +
          At(x-1,y+1).Visibility +
          At(x,  y+1).Visibility;
        if (j >= 5) /* 5+ walls nearby */
          At(x,y).LifeFlag = true;
        else if (j <= 3) /* 4+ open spaces */
          At(x,y).LifeFlag = false;
      }

    for (x=r.x1+buffer;x<=r.x2-buffer;x++)
      for (y=r.y1+buffer;y<=r.y2-buffer;y++)
        At(x,y).Visibility = At(x,y).LifeFlag;
  }

  for (x=r.x1+buffer;x<=r.x2-buffer;x++)
    for (y=r.y1+buffer;y<=r.y2-buffer;y++)
    {
      At(x,y).LifeFlag = false;
      j = At(x,y).Visibility;
      if (!j)
        WriteAt(r,x,y,FloorID,regID,PRIO_ROOM_FLOOR);
    }

  for (x=r.x1;x<=r.x2;x++)
    for (y=r.y1;y<=r.y2;y++)         
      At(x,y).Visibility = 0;
}

void Map::GeneratePrompt()
  {
    const char Chars[] = "|\\-/";
    static int32 pc; 
    static int16 i;
    static Term *t;
    pc++;
    T1->SetWin(WIN_INPUT);
    T1->Clear();
    T1->Color(MAGENTA);
    T1->Write(Format("Generating level %d of %s... %c%c%c",
      Depth,dID ? NAME(dID) : "the dungeon",-YELLOW,Chars[pc%4],-MAGENTA));
    T1->Update();
  }
        




void Map::WriteCastle(Rect &r, rID regID)
{
  uint8 d,i,o,sx,sy; Rect r1,r2;
  rID FloorID, WallID;
  FloorID = TREG(regID)->Floor;
  WallID  = TREG(regID)->Walls;
  sx = r.x2 - r.x1;
  sy = r.y2 - r.y1;
  if (sx < 7 || sy < 7)
    {
      PopulateQueue[cRectPop++] = r;
      return;
    }

  if (!random(10) || (sx > sy && random(9))) {
    o = 0;
    do {
      o++;
      d = r.x1 + 2 + random(r.x2-(r.x1+4));
    }
    while((!At(d,r.y1).Solid || !At(d,r.y2).Solid) && o < 10);

    r1.y1 = r2.y1 = r.y1;
    r1.y2 = r2.y2 = r.y2; 

    r1.x1 = r.x1; r1.x2 = d;
    r2.x1 = d; r2.x2 = r.x2;

    o = r.y1 + 1 + random(r.y2-(r.y1+2));
    for(i=r.y1+1;i!=r.y2;i++) 
      if (i!=o)
        WriteAt(r,d,i,WallID,regID,PRIO_ROOM_FURNITURE);

    MakeDoor(d,o,TREG(regID)->Door);

    WriteCastle(r1,regID);
    WriteCastle(r2,regID);
  }
  else {
    o = 0;
    do {
      o++;
      d = r.y1 + 2 + random(r.y2-(r.y1+4));
    }
    while((!At(r.x1,d).Solid || !At(r.x2,d).Solid) && o < 10);

    r1.x1 = r2.x1 = r.x1;
    r1.x2 = r2.x2 = r.x2; 

    r1.y1 = r.y1; r1.y2 = d;
    r2.y1 = d; r2.y2 = r.y2;

    o = r.x1 + 1 + random(r.x2-(r.x1+2));
    for(i=r.x1+1;i!=r.x2;i++) 
      if (i!=o)
        WriteAt(r,i,d,WallID,regID,PRIO_ROOM_FURNITURE);
    MakeDoor(o,d,TREG(regID)->Door);

    WriteCastle(r1,regID);
    WriteCastle(r2,regID);
  }
}
      
void Map::WriteRCavern(Rect &r, rID regID)
{
  int16 i,j,k,x,y,sx,sy;
  const int buffer = 3; 
  sx = (r.x2 - r.x1) - buffer;
  sy = (r.y2 - r.y1) - buffer; 
  if (sx < 12 || sy < 12)
    return;

  rID FloorID, WallID;
  FloorID = TREG(regID)->Floor;
  WallID  = TREG(regID)->Walls;
  k = sx*sy/20;
  for(i=0;i!=k;i++)
  {
    x = buffer + r.x1 + 5 + random(sx-10);
    y = buffer + r.y1 + 5 + random(sy-10);
    switch(random(4))
    {
      case 0:
        for(j=0;j!=5;j++)
        {
          WriteAt(r,x+j,y,FloorID,regID,PRIO_ROOM_FLOOR);
          WriteAt(r,x,y-j,FloorID,regID,PRIO_ROOM_FLOOR);
        }
        break;
      case 1:
        for(j=0;j!=5;j++)
        {
          WriteAt(r,x+j,y,FloorID,regID,PRIO_ROOM_FLOOR);
          WriteAt(r,x,y+j,FloorID,regID,PRIO_ROOM_FLOOR);
        }
        break;
      case 2:
        for(j=0;j!=5;j++)
        {
          WriteAt(r,x-j,y,FloorID,regID,PRIO_ROOM_FLOOR);
          WriteAt(r,x,y-j,FloorID,regID,PRIO_ROOM_FLOOR);
        }
        break;
      case 3:
        for(j=0;j!=5;j++)
        {
          WriteAt(r,x-j,y,FloorID,regID,PRIO_ROOM_FLOOR);
          WriteAt(r,x,y+j,FloorID,regID,PRIO_ROOM_FLOOR);
        }
        break;
    }
  }

  rID defWall = Con[TERRAIN_ROCK]; 
  if (WallID && WallID != defWall) {
    for (i=r.x1;i<r.x2;i++)
      for (j=r.y1;j<r.y2;j++) {
        bool found = false;
        for (int zz = 0; !found && zz<9 ; zz++) {
          if (InBounds(i+DirX[zz],j+DirY[zz]) && 
              TerrainAt(i+DirX[zz],j+DirY[zz]) == FloorID)
            found = true; 
        } 
        if (found && TerrainAt(i,j) == defWall) {
          WriteAt(r,i,j,WallID,regID,PRIO_ROCK_STREAMER);
        }
      } 
  } 
}

void Map::WriteOctagon(Rect &r,rID regID)
{
  int16 cx, cy;

  rID FloorID;
  if (r.x2 - r.x1 <= 1)
    return;
  if (r.y2 - r.y1 <= 1)
    return;
  FloorID = TREG(regID)->Floor;

  int bad[48] = {
            r.x1+0,r.y1+0,
            r.x1+0,r.y1+1,
            r.x1+1,r.y1+0,
            r.x1+2,r.y1+0,
            r.x1+1,r.y1+1,
            r.x1+0,r.y1+2,

            r.x2-0,r.y1+0,
            r.x2-0,r.y1+1,
            r.x2-1,r.y1+0,
            r.x2-2,r.y1+0,
            r.x2-1,r.y1+1,
            r.x2-0,r.y1+2,

            r.x1+0,r.y2-0,
            r.x1+0,r.y2-1,
            r.x1+1,r.y2-0,
            r.x1+2,r.y2-0,
            r.x1+1,r.y2-1,
            r.x1+0,r.y2-2,

            r.x2-0,r.y2-0,
            r.x2-0,r.y2-1,
            r.x2-1,r.y2-0,
            r.x2-2,r.y2-0,
            r.x2-1,r.y2-1,
            r.x2-0,r.y2-2,
  }; 

  for(uint8 x=r.x1;x<=r.x2;x++)
    for(uint8 y=r.y1;y<=r.y2;y++) {
      bool isBad = false; 
      for (int b = 0; b < 24; b++) {
        if (bad[b*2] == x && bad[b*2+1] == y)
          isBad = true; 
      } 
      if (!isBad) 
        WriteAt(r,x,y,FloorID,regID,PRIO_ROOM_FLOOR);
    }

  cx = (r.x1 + (r.x2-r.x1)/2);
  cy = (r.y1 + (r.y2-r.y1)/2);

  Corners[nCorners++] = (r.x1+1) + cy*256;
  Corners[nCorners++] = (r.x2-1) + cy*256;
  Corners[nCorners++] = cx + (r.y1+1)*256;
  Corners[nCorners++] = cx + (r.y2-1)*256;

  Centers[nCenters++] = cx + cy*256;     

}

void Map::WriteDiamond(int16 x, int16 y, rID regID)
  {
    int8 i, DiamondCoords[41][2] = {
      { 0, -4 }, 
      { -1, -3 }, { 0, -3 }, { 1, -3 },
      { -2, -2 }, { -1, -2 }, { 0, -2 }, { 1, -2 }, { 2, -2 },
      { -3, -1 }, { -2, -1 }, { -1, -1 }, { 0, -1 }, { 1, -1 }, { 2, -1 }, { 3, -1 },
      { -4, 0 }, { -3, 0 }, { -2, 0 }, { -1, 0 }, { 0, 0 },
        { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 },      
      { -3, 1 }, { -2, 1 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 },
      { -2, 2 }, { -1, 2 }, { 0, 2 }, { 1, 2 }, { 2, 2 },
      { -1, 3 }, { 0, 3 }, { 1, 3 },
      { 0, 4 } };
    rID WallID, FloorID; 
    Rect r(x-4,y-4,x+4,y+4);
    WallID  = TREG(regID)->Walls;
    FloorID = TREG(regID)->Floor;
      
    for (i=0;i!=41;i++)
      WriteAt(r,x + DiamondCoords[i][0], y + DiamondCoords[i][1], FloorID, regID, 25);   

    Centers[nCenters++] = x + (y * 256); 


  }

void Map::WriteCross(Rect &r, rID regID)
  {
    int16 k; Rect r1,r2;
    k = random(2) ? 5 : 3;
    while ((r.x2 - r.x1) % k)
      if (random(2))
        r.x2--;
      else
        r.x1++;
    while ((r.y2 - r.y1) % k)
      if (random(2))
        r.y2--;
      else
        r.y1++;

    r1.y1 = r.y1+1;
    r1.y2 = r.y2-1;
    r1.x1 = r.x1 + ((r.x2 - r.x1) / k) * (k/2);
    r1.x2 = r.x1 + ((r.x2 - r.x1) / k) * (k/2+1);

    r2.x1 = r.x1+1;
    r2.x2 = r.x2-1;
    r2.y1 = r.y1 + ((r.y2 - r.y1) / k) * (k/2);
    r2.y2 = r.y1 + ((r.y2 - r.y1) / k) * (k/2+1);
    
    WriteRoom(r1,regID);
    WriteRoom(r2,regID);
  }

void Map::WriteDestroyed(Rect &r,rID regID)
  {
  }

void Map::WriteMaze(Rect &r,rID regID, int16 inset_count,...)
  {
    int16 visits, i,n,dirs, x, y, rx, ry, paths[4];
    TRegion *tr = TREG(regID); Rect hr;
    int32 oxy;
    
    /* Inset Algorithm (TODO):
         Place square inset map somewhere in maze area. Mark all
       squares covered by it the same way as we mark things beyond
       walls. For each square so marked, decrement visits. Write
       the maze normally. Then write the inset. Where the inset
       has a door, secret or otherwise, clear the spot in the right
       direction beside the always-open square, regardless of the
       maze values.
         Insets should always have an odd number of columns and
       rows, but should place their doors at even coordinates. We
       can use even 3x3 insets to place little fountains and such
       in the maze.
    */

    if (((r.x2-r.x1) % 2))
      r.x2--;
    if (((r.y2-r.y1) % 2))
      r.y2--;


    hr.x1 = r.x1;
    hr.x2 = r.x1 + (r.x2-r.x1)/2 - 1;
    hr.y1 = r.y1;
    hr.y2 = r.y1 + (r.y2-r.y1)/2 - 1;

    for (x=r.x1+1;x<r.x2;x++)
      for(y=r.y1+1;y<r.y2;y++)
          At(x,y).Visibility = 0;

    visits = ((hr.x2 - hr.x1)+1) * ((hr.y2 - hr.y1)+1) - 1;
    x = hr.x1; y = hr.y1; i = 0;
    
    while (visits > 0) {
      
      dirs = 0;
      if (x > hr.x1 && !At(x-1,y).Visibility)
        paths[dirs++] = WEST;
      if (y > hr.y1 && !At(x,y-1).Visibility)
        paths[dirs++] = NORTH;
      if (x < hr.x2 && !At(x+1,y).Visibility)
        paths[dirs++] = EAST;
      if (y < hr.y2 && !At(x,y+1).Visibility)
        paths[dirs++] = SOUTH;
       
      oxy = x+y*256;

      if (dirs) {
        visits--;
        dirs = random(dirs);
        At(x,y).Visibility |= XBIT((Dir)paths[dirs]);
        x += DirX[paths[dirs]]; 
        y += DirY[paths[dirs]];
        At(x,y).Visibility |= XBIT(OppositeDir((Dir)paths[dirs]));
        }
      else do {
        x++;
        if (x > hr.x2)
          { x = hr.x1; y++; }
        if (y > hr.y2)
          { y = hr.y1; }
        }
        while (!(At(x,y).Visibility & 0x000F));
      }

    for (x=hr.x1;x<=hr.x2;x++)
      for (y=hr.y1;y<=hr.y2;y++)
        {
          rx = hr.x1 + (x-hr.x1)*2+1;
          ry = hr.y1 + (y-hr.y1)*2+1;

          if ((rx+1 < r.x2) && !(At(x,y).Visibility & XBIT(EAST)))
            WriteAt(r,rx+1,ry,tr->Walls,regID,PRIO_ROOM_WALL); 
          if (ry+1 >= r.y2)
            continue;
          if (!(At(x,y).Visibility & XBIT(SOUTH)))
            WriteAt(r,rx,ry+1,tr->Walls,regID,PRIO_ROOM_WALL); 
          if (rx+1 < r.x2)
            WriteAt(r,rx+1,ry+1,tr->Walls,regID,PRIO_ROOM_WALL);
        }
    for(x=r.x1;x<=r.x2;x++) {
      WriteAt(r,x,r.y1,tr->Walls,regID,PRIO_ROOM_WALL);
      WriteAt(r,x,r.y2,tr->Walls,regID,PRIO_ROOM_WALL);
      }      
    for(y=r.y1;y<=r.y2;y++) {
      WriteAt(r,r.x1,y,tr->Walls,regID,PRIO_ROOM_WALL);
      WriteAt(r,r.x2,y,tr->Walls,regID,PRIO_ROOM_WALL);
      }      
    for(x=r.x1;x<=r.x2;x++) 
      for(y=r.y1;y<=r.y2;y++) {
        if (TerrainAt(x,y) != tr->Walls)
          WriteAt(r,x,y,tr->Floor,regID,PRIO_ROOM_FLOOR);
        At(x,y).Visibility = 0;
      }

    
    /* Secret Doors */
    n = min(7,Depth - 3);
    if (n <= 0)
      return;
    
    for (i=0;i!=n;i++) {
      int trials = 0; 
      do {
        x = r.x1 + 1 + random(r.x2-(r.x1+2));
        y = r.y1 + 1 + random(r.y2-(r.y1+2));
        if (At(x,y).Solid)
          continue;
        if (At(x+1,y).Solid && At(x-1,y).Solid &&
            !(At(x,y+1).Solid || At(x,y-1).Solid) )
          break;
        if (At(x,y+1).Solid && At(x,y-1).Solid &&
            !(At(x+1,y).Solid || At(x-1,y).Solid) )
          break;        
        }
      while (trials++ < 500);
      if (!At(x,y).Solid)
        continue;
      MakeSecretDoor((uint8)x,(uint8)y,tr->Door);
    }
     
  }

void Map::WriteStreamer(Rect &r, uint8 sx, uint8 sy, Dir d, rID regID)
{
  int16 rx,ry,Width,MWidth; int16 DepthCR;
  int16 ix,iy,mx,my; bool isRiver, isWater;   
  rID terID = TREG(regID)->Floor;
  isWater = TTER(terID)->HasFlag(TF_WATER);

  if (TREG(regID)->HasFlag(RF_RIVER)) {
    isRiver = true;
    Width = MWidth = 2 + random(4);
    if (sx > sy)
    {
      sy = 0;
      d = random(2) ? SOUTHEAST : SOUTHWEST;
    }
    else
    {
      sx = 0;
      d = random(2) ? SOUTHEAST : NORTHEAST;
    }
  }
  else {
    Width = 1; isRiver = false;
    MWidth = 2 + random((int16)Con[MAX_STREAMER_WIDTH]);
    if (d == -1)
      d = !random(4) ? SOUTHEAST :
        !random(3) ? SOUTHWEST :
        !random(2) ? NORTHEAST : NORTHWEST;
  }

  if (TREG(regID)->HasFlag(RF_CHASM))
    MWidth = max(4,MWidth+1);

  rx = 2 + random(10);
  ry = 2 + random(10);

  bool midpoint = false;
  while (Width && InBounds(sx,sy))
  {
    if ((!isRiver) && (!random(13))) {
      if (midpoint)
        Width--;
      else
        Width++;
    }

    if (random(rx+ry)+1 <= rx)
      sx += DirX[d];
    else
      sy += DirY[d];

    if (Width >= MWidth)
      midpoint = true;

    mx = sx - (Width / 2);
    my = sy - (Width / 2);
    //aquaID = FIND("lone aquatic monster");
    for(ix = mx;ix<mx+Width;ix++)
      for(iy = my;iy<my+Width;iy++) {
        int prio = PRIO_ROCK_STREAMER;
        if (TREG(regID)->HasFlag(RF_CHASM) || 
            TREG(regID)->HasFlag(RF_RIVER))
          prio = PRIO_RIVER_STREAMER; 
        WriteAt(r,ix,iy,terID,regID, prio);
        if ((!random((int16)Con[STREAMER_MON_DENSITY])) && InBounds(ix,iy) &&
              TREG(regID)->HasList(ENCOUNTER_LIST)) {
          DepthCR = AdjustCR((int16)Con[INITIAL_CR] + (Depth*(int16)Con[DUN_SPEED])/100 - 1); 
          THROW(EV_ENGEN,
            xe.EMap = this;
            xe.enFlags = EN_STREAMER|(isWater ? EN_AQUATIC : 0);
            xe.enRegID = regID;
            xe.EXVal = ix;
            xe.EYVal = iy;
            xe.isLoc = true;
            xe.enCR = DepthCR;
            xe.enDepth = DepthCR;);
        }
      }
  }
}

void Map::WriteBlobs(Rect &r, rID regID, rID bID)
{
  uint8 i,j,x,y; 

  for (x=r.x1+1;x<=r.x2-1;x++)
    for (y=r.y1+1;y<=r.y2-1;y++)     
      At(x,y).Visibility = (random(100) > (int16)Con[LIFE_PERCENT]-1) ? 0 : 1;

  for(i=0;i!=20;i++) {
    for (x=r.x1+1;x<r.x2;x++)
      for (y=r.y1+1;y<r.y2;y++)
      {
        j = At(x+1,y+1).Visibility +
          At(x+1,y)  .Visibility +
          At(x+1,y-1).Visibility +
          At(x,  y-1).Visibility +
          At(x-1,y-1).Visibility +
          At(x-1,y)  .Visibility +
          At(x-1,y+1).Visibility +
          At(x,  y+1).Visibility;
        if (j >= 5) /* 5+ walls nearby */
          At(x,y).LifeFlag = true;
        else if (j <= 3) /* 4+ open spaces */
          At(x,y).LifeFlag = false;
      }

    for (x=r.x1+1;x<r.x2;x++)
      for (y=r.y1+1;y<r.y2;y++)
        At(x,y).Visibility = At(x,y).LifeFlag;
  }

  for (x=r.x1+1;x<r.x2;x++)
    for (y=r.y1+1;y<r.y2;y++)
    {
      At(x,y).LifeFlag = false;
      j = At(x,y).Visibility;
      At(x,y).Visibility = 0;
      if (j)
        if (!At(x,y).Solid && (regID <= 0 || RegionAt(x,y) == regID))
          WriteAt(r,x,y,bID,RegionAt(x,y),PRIO_ROOM_FURNITURE);
    }
}




void Map::WriteMap(Rect &r,rID regID)
{
  TRegion *m = TREG(regID); Creature *mn; Item *it; Feature *ft;
  int16 i,x,y,maxlev, PartyID; Tile *t;
  int16 DepthCR = maxlev = AdjustCR((int16)(Con[INITIAL_CR] + (Depth*Con[DUN_SPEED])/100 - 1)); 

  const char *gr = theGame->GetText(regID,TREG(regID)->Grid);
  rID WallID   = m->Walls,
  FloorID  = m->Floor, 
  DoorID   = m->Door,
  mID, xID, xID2;

  char ch;

  if (TREG(regID)->Grid == 0)
    return;

  PartyID = MAX_PLAYERS + 10 + random(200);

  for(i=0;i!=127;i++)
    MapLetterArray[i] = m->GetTile((char)i);


  /* The default meanings of map-definition characters:
   * '#' Region's Wall, priority 80
   * '%' Standard Wall, priority 10
   * '1' to '9'  Monster N*2 levs OOD
   * '$' Pile of Gold
   * 'g' Good Item
   * 'G' Great Item
   * 't' Trap
   * 'T' *Badass* Trap
   * '.' Region's Floor
   * '+' Door
   * 'A' Altar
   * '~' Water
   * '_' Unworked Stone
   * ',' Floor, of Corridor Region
   * 'i' Potion or Scroll
   * 'I' Normal Item
   * '-' Trapped Door
   * '|' Tough Door
   * 'X' Indestructable Rock
   * 'V' Vault Door
   * 'v' 'Vault Warning' Region
   * 'G' City Guardspost
   * '@' Start the Player Here
   * 'S' Secret Door
   * '*' Call script EV_MAPSYMBOL
   */

  bool flipHoriz = random(2) != 0;
  bool flipVert = random(2) != 0;


  for(x=0;x<m->sx;x++) 
    for(y=0;y<m->sy;y++)
    {
      Thing * f; 
      while ((f = FirstAt(r.x1+x,r.y1+y))) {
        f->Remove(true); 
      }

      ch = gr[ (flipHoriz ? ((m->sx-1) - x) : x) + 
        (flipVert ? ((m->sy-1) - y) : y) * m->sx];
      At(r.x1 + x,r.y1 + y).isVault = true;
      if (t = MapLetterArray[ch]) {
        WriteAt(r,r.x1 + x,r.y1 + y,t->tID,regID,PRIO_VAULT);

        if (t->Image & 0xFF00) {
          At(r.x1 + x,r.y1 + y).Glyph &= 0x00FF;
          At(r.x1 + x,r.y1 + y).Glyph |= t->Image;
        }
        if (t->Image & 0x00FF) {
          At(r.x1 + x,r.y1 + y).Glyph &= 0xFF00;
          At(r.x1 + x,r.y1 + y).Glyph |= t->Image;
        }
        if ((t->Image & 0xFF00) != 0x0700)
          At(r.x1 + x,r.y1 + y).Shade = false;
        if (t->fl & TILE_ITEM || (!(t->fl & TILE_MONSTER) && 
              RES(t->xID) && RES(t->xID)->Type == T_TITEM))
        {
          if (t->fl & TILE_RANDOM)
            xID = theGame->GetItemID(PUR_DUNGEON,-2,(int8)(DepthCR+4),(int8)(t->xID ? t->xID : -1));
          else
            xID = t->xID;

          // ww: don't make corpses for NOCORPSE monsters
          if ((TITEM(xID)->IType == T_STATUE || TITEM(xID)->IType == T_CORPSE ||
                TITEM(xID)->IType == T_FIGURE) && (t->fl & TILE_RANDOM_EFF)) {
            do { 
              xID2 = theGame->GetMonID(PUR_DUNGEON,0,(int8)DepthCR,(int8)DepthCR,0);
            } while (TMON(xID2)->HasFlag(M_NO_CORPSE) ||
                TMON(xID2)->isMType(xID2,MA_DRAGON));
          }
          else if (t->fl & TILE_RANDOM_EFF)
            xID2 = theGame->GetEffectID(PUR_DUNGEON,(int8)(t->xID2 % 256),(int8)(t->xID2 / 256));
          else
            xID2 = t->xID2;

          if (t->fl & TILE_BLESSED)
            it->IFlags |= IF_BLESSED;
          if (t->fl & TILE_CURSED)
            it->IFlags |= IF_CURSED;
          if (t->fl & TILE_MASTERWORK)
            it->IFlags |= IF_MASTERWORK;
#if 0
          if (t->fl & TILE_PLUS1)
            it->Plus += 1;
          if (t->fl & TILE_PLUS2)
            it->Plus += 2;
          if (t->fl & TILE_PLUS4)
            it->Plus += 4;
          if (t->fl & TILE_KNOWN)
            it->SetKnown(0xFF);
#endif
          it = Item::Create(xID);
          if (t->fl & TILE_QUALITY)
            it->AddQuality((int8)xID2);
          else if (it->Type == T_CORPSE || it->Type == T_FIGURE ||
              it->Type == T_STATUE)
            ((Corpse*)it)->SetCorpseType(xID2);
          else if (xID2 && RES(xID2)->Type == T_TEFFECT)
            it->MakeMagical(xID2,TEFF(xID2)->HasFlag(EF_NEEDS_PLUS) ? random(3)+1 : 0);

          it->PlaceAt(this,r.x1+x,r.y1+y);                

        }
        else if ((t->fl & TILE_MONSTER) || ((!(t->fl & TILE_ITEM)) && 
              t->xID && (RES(t->xID)->Type == T_TMONSTER)))
        {
          /* Create the Monster */
          if (t->xID) {
            mn = new Monster(t->xID);
            TMON(t->xID)->GrantGear(mn,t->xID,true);
            TMON(t->xID)->PEvent(EV_BIRTH,mn,t->xID);

            if (t->xID2 && RES(t->xID2)->Type == T_TTEMPLATE)
              if (mn->isMType(TTEM(t->xID2)->ForMType))
                mn->AddTemplate(t->xID2);
            mn->PlaceAt(this,r.x1+x,r.y1+y);
            }
          else {
            THROW(EV_ENGEN,
              xe.enFlags = EN_DUNGEON|EN_VAULT|EN_SINGLE|EN_FREAKY;
              xe.enCR = DepthCR+4;
              xe.enRegID = regID;
              xe.enDepth = DepthCR;
              xe.EXVal = r.x1 + x;
              xe.EYVal = r.y1 + y;
              xe.isLoc = true;
              );
            mn = GetEncounterCreature(0);
            }
          
          if (t->fl & TILE_NO_PARTY)
            mn->PartyID = MAX_PLAYERS + 10 + random(200);
          else
            mn->PartyID = PartyID;
          
            
          if (t->fl & TILE_PEACEFUL)
            mn->StateFlags |= MS_PEACEFUL;
          if (t->fl & TILE_HOSTILE)
            mn->StateFlags &= ~MS_PEACEFUL;
          if (t->fl & TILE_GUARDING)
            mn->StateFlags |= MS_GUARDING;
          if (t->fl & TILE_ASLEEP)
            mn->GainPermStati(ASLEEP,NULL,SS_MISC,SLEEP_NATURAL);
          mn->Initialize();
        }
        else {
          if (t->xID && RES(t->xID)->Type == T_TFEATURE)
          {
            /* Create the Feature */
            if (TFEAT(t->xID)->FType == T_DOOR)
            {
              ft = new Door(t->xID);
              ((Door*)ft)->DoorFlags &= ~DF_VERTICAL;
              if (t->fl & TILE_HIDDEN)
                ((Door*)ft)->DoorFlags |= DF_SECRET;
              if (t->fl & TILE_LOCKED)
                ((Door*)ft)->DoorFlags |= DF_LOCKED;
              if (t->fl & TILE_STUCK)
                ((Door*)ft)->DoorFlags |= DF_STUCK;
              if (t->fl & TILE_OPEN)
                ((Door*)ft)->DoorFlags |= DF_OPEN;
            }
            else if (TFEAT(t->xID)->FType == T_PORTAL)
              ft = new Portal(t->xID);
            else if (TFEAT(t->xID)->FType == T_TRAP)
              ft = new Trap(FIND("trap"),t->xID);
            else
              ft = new Feature(TFEAT(t->xID)->Image,t->xID,T_FEATURE);
            if (t->xID2 && RES(t->xID2)->Type == T_TGOD)
              ft->GainPermStati(MY_GOD,NULL,SS_MISC,0,0,t->xID2);
            
            ft->PlaceAt(this,r.x1+x,r.y1+y);                
            {
              EventInfo e; 
              e.Clear();  
              e.Event = EV_INITIALIZE; 
              e.ETarget = (Thing *)ft; 
              e.EXVal = r.x1+x;
              e.EYVal = r.y1+y;
              e.EMap = this; 
              ft->Event(e);
            }
          }
          if (t->fl & TILE_START)
          { EnterX = x + r.x1;
            EnterY = y + r.y1; } 
        }

      }
      else switch(ch)
      {
        case '%':
          WriteAt(r,r.x1 + x,r.y1 + y,WallID,regID,PRIO_ROOM_WALL);
          At(r.x1 + x,r.y1 + y).isVault = false;
          break;
        case '#':
          WriteAt(r,r.x1 + x,r.y1 + y,WallID,regID,PRIO_VAULT);
          break;
        case '.':
          WriteAt(r,r.x1 + x,r.y1 + y,FloorID,regID,PRIO_VAULT);
          break;
        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9':
          WriteAt(r,r.x1 + x,r.y1 + y,FloorID,regID,PRIO_VAULT);
          maxlev = DepthCR + (ch-'0')*2; /* Later, by map constants */
          while (((50 - mLuck*3) > (random(100)+1)))
            maxlev++;
          mID = theGame->GetMonID(PUR_DUNGEON,(int8)max(0,maxlev-5),(int8)maxlev,(int8)DepthCR);
          if (mID) {
            mn = new Monster(mID);
            mn->PartyID = PartyID;
            TMON(mID)->GrantGear(mn,mID,true);
            TMON(mID)->PEvent(EV_BIRTH,mn,mID);
            if (mn->isMType(MA_DRAGON)) {
              xID = theGame->GetTempID(TM_AGECAT,mn->mID,(int8)maxlev);
              if (!xID)
                xID = FIND("young adult");
              mn->AddTemplate(xID);
            }
            mn->StateFlags |= MS_GUARDING;
            mn->PlaceAt(this,r.x1 + x,r.y1 + y);
            mn->Initialize();
          }
          break;
        case 'i':
          WriteAt(r,r.x1 + x,r.y1 + y,FloorID,regID,PRIO_VAULT);
          it = Item::GenItem(IG_STAPLE,mID,DepthCR,mLuck,StapleItems);
          it->PlaceAt(this,r.x1 + x,r.y1 + y);
          break;
        case 'I':
          WriteAt(r,r.x1 + x,r.y1 + y,FloorID,regID,PRIO_VAULT);
          it = Item::GenItem(0,mID,DepthCR,mLuck,DungeonItems);
          it->PlaceAt(this,r.x1 + x,r.y1 + y);
          break;
        case 'g':
          WriteAt(r,r.x1 + x,r.y1 + y,FloorID,regID,PRIO_VAULT);
          it = Item::GenItem(IG_GOOD,mID,DepthCR+2,mLuck+4,DungeonItems);
          it->PlaceAt(this,r.x1 + x,r.y1 + y);
          break;
        case 'G':
          WriteAt(r,r.x1 + x,r.y1 + y,FloorID,regID,PRIO_VAULT);
          it = Item::GenItem(IG_GOOD,mID,DepthCR+4,mLuck+8,DungeonItems);
          it->PlaceAt(this,r.x1 + x,r.y1 + y);
          break;
        case 'c': /* Good Chest */
        case 'C': /* Great Chest */
          WriteAt(r,r.x1 + x,r.y1 + y,FloorID,regID,PRIO_VAULT);
          break;
        case '+': 
          WriteAt(r,r.x1 + x,r.y1 + y,FloorID,regID,PRIO_VAULT);
          if (!FFeatureAt(r.x1 + x,r.y1 + y))
            {
              Door *d;
              ASSERT(DoorID);
              if (DoorID) {
                d = new Door(DoorID);
                d->DoorFlags &= DF_VERTICAL;
                d->PlaceAt(this,r.x1 + x,r.y1 + y);
                }
            }
          break;
        case 's':
          WriteAt(r,r.x1 + x,r.y1 + y,FloorID,regID,PRIO_VAULT);
          it = Item::Create(FIND("statue"));
          it->PlaceAt(this,r.x1 + x,r.y1 + y);
          break;
        case 'S': 
          WriteAt(r,r.x1 + x,r.y1 + y,FloorID,regID,PRIO_VAULT);
          MakeSecretDoor(r.x1 + x,r.y1 + y,DoorID);
          break;
        default:
          Error("Illegal char '%c' in map grid for \"%s\"!",
              ch,NAME(regID));
      }

    }

  /* Reset the doors after all the walls are drawn. */
  Thing *dr;
  MapIterate(this,dr,i)
    if (dr->isType(T_DOOR) && r.Within((uint8)dr->x,(uint8)dr->y))
      dr->SetImage();

  uint32 ColorList[16], c;
  if (TREG(regID)->GetList(WALL_COLOURS,ColorList,16)) {
    for(c=0;ColorList[c];c++)
      ;
    for(x=0;x<m->sx;x++) 
      for(y=0;y<m->sy;y++)
      {
        if (TerrainAt(r.x1+x,r.y1+y) == WallID &&
            (At(r.x1+x,r.y1+y).Priority == PRIO_ROOM_WALL ||
             At(r.x1+x,r.y1+y).Priority == PRIO_VAULT)) {
          At(r.x1+x,r.y1+y).Glyph = (At(r.x1+x,r.y1+y).Glyph & 0x00FF) | (ColorList[random((int16)c)] << 8);
          At(r.x1+x,r.y1+y).Shade = false;
        }
      }
  }
}

void Map::SetRegion(Rect r, rID regID)
  {
    int16 i, RegionVal, x, y;
    for(i=0;RegionList[i];i++)
      if (RegionList[i] == regID)
        { RegionVal = i; goto FoundRegion; }
    if (i == 255)
      Fatal("Too many Regions on one map!");
    RegionVal = i;
    RegionList[i++] = regID;
    FoundRegion:

    for(x=r.x1;x<=r.x2;x++)
      for(y=r.y1;y<=r.y2;y++)
        At(x,y).Region = RegionVal;
  }


void Map::MakeDoor(uint8 x,uint8 y,rID fID)
  {    
    Door *d;
    if (!fID)
      fID = FIND("oak door");
      
    /* Force all doors on dlev 1-3 to be wood -- some character
       types can't get through the other kinds of doors in the
       early game! */
    if (Depth <= 3 && TFEAT(fID)->Material != MAT_WOOD)
      fID = FIND("oak door");
    
      
    ASSERT(fID);
    if (FFeatureAt(x,y))
      return;
    d = new Door(fID);
    d->PlaceAt(this,x,y);
    d->SetImage();
  }

void Map::MakeSecretDoor(uint8 x,uint8 y,rID fID)
  {    
    Door *d;
    if (!fID)
      fID = FIND("oak door");
    ASSERT(fID);
    if (FFeatureAt(x,y))
      return;
    d = new Door(fID);
    d->DoorFlags &= (~DF_VERTICAL);
    d->DoorFlags |= DF_SECRET | DF_LOCKED;
    d->PlaceAt(this,x,y);
    d->SetImage();
  }

void Map::LoadFixedMap(rID mID, int16 _Depth, Map *Above, int8 _Luck)
  {
    int16 x,y;
    dID = mID; Depth = _Depth; mLuck = _Luck;
    
    sizeX = TREG(mID)->sx;
    sizeY = TREG(mID)->sy;
    panelsX = 1;
    panelsY = 1;
  
    Grid = (LocationInfo*) malloc(sizeof(LocationInfo) * (sizeX * sizeY + 1));
    if (Grid == NULL)
      Fatal("Error allocating map grid!");
    memset(Grid,0,sizeof(LocationInfo) * (sizeX * sizeY + 1));
  
    Rect r(0,0,(uint8)sizeX,(uint8)sizeY);
    
    WriteMap(r,mID);    

    r.Set(0,0,sizeX-1,sizeY-1);
    calcLight(r);

    for (x=0;x<sizeX;x++)
      for (y=0;y<sizeY;y++)
        At(x,y).Priority = 0;
    
}

void Map::Generate(rID _dID, int16 _Depth, Map *Above,int8 Luck) {
    int16 i, j, n, fCount, tot, c;
    int16 x, y, sx,sy,dx,dy,cx,cy,px,py, Streamers, Tries; 
    rID xtID, sID, xID;
    Rect r;
    Annotation *a;
    Thing *t, *t2;
    Item *it;
    Portal *st; 
    bool NarrowChasm = false;
    bool extraLevs = (theGame->Opt(OPT_DIFFICULTY) >= DIFF_CHALLENGE);

    if (RES(_dID)->Type == T_TREGION) {
        LoadFixedMap(_dID,_Depth,Above,Luck);
        return;
    }

    inGenerate = true;

    /* Step 1: Initialize the map */
    dID = _dID;
    Depth = _Depth;
    mLuck = Luck;

    GeneratePrompt();

    for (i=0; i!=LAST_DUNCONST; i++)
        Con[i] = TDUN(dID)->GetConst(i);

    int16 DepthCR = AdjustCR((int16)Con[INITIAL_CR] + (Depth*(int16)Con[DUN_SPEED])/100 - 1); 

    TDUN(dID)->GetList(RC_WEIGHTS,RC_Weights,RC_LAST*2);
    TDUN(dID)->GetList(STREAMER_WEIGHTS,StreamWeights,1024);
    TDUN(dID)->GetList(CORRIDOR_WEIGHTS,CorridorWeights,1024);
    TDUN(dID)->GetList(ROOM_WEIGHTS,RoomWeights,1024);
    TDUN(dID)->GetList(VAULT_WEIGHTS,VaultWeights,1024);

    if (Con[LEVEL_SIZEX] % Con[PANEL_SIZEX])
        Fatal("Mismatch between LEVEL_SIZEX and PANEL_SIZEX.");

    if (Con[LEVEL_SIZEY] % Con[PANEL_SIZEY])
        Fatal("Mismatch between LEVEL_SIZEY and PANEL_SIZEY.");

    sizeX = (int16)Con[LEVEL_SIZEX];
    sizeY = (int16)Con[LEVEL_SIZEY];
    panelsX = (int8)(Con[LEVEL_SIZEX] / Con[PANEL_SIZEX]);
    panelsY = (int8)(Con[LEVEL_SIZEY] / Con[PANEL_SIZEY]);
    if (panelsX > 32 || panelsY >32)
        Fatal("Panel grid exceeds maximum of 32 x 32.");

    /* Displacement of this level relative to the one above it. */
    if (Above) {
        disX = (sizeX - Above->sizeX) / 2;
        disY = (sizeY - Above->sizeY) / 2;
    } else
        disX = disY = 0;

    Grid = (LocationInfo*) malloc(sizeof(LocationInfo) * (sizeX * sizeY + 1));
    GridBackup = (LocationInfo*) malloc(sizeof(LocationInfo) * (sizeX * sizeY + 1));
    if (Grid == NULL || GridBackup == NULL)
        Fatal("Error allocating map grid!");

    memset(Grid,0,sizeof(LocationInfo) * (sizeX * sizeY + 1));
    memset(RoomsTouched,0,sizeof(int32)*32);
    memset(PanelsDrawn ,0,sizeof(int32)*32);
    memset(TerrainList,0,sizeof(rID)*255);
    memset(RegionList,0,sizeof(rID)*255);
    /* Yikes! We're allocating up to 448K here (for a worst-case scenario
    * of a 256 x 256 level) of temporary storage for the floodfilling.
    * This would be wasteful if we didn't immediately deallocate it after
    * doing the floodfill tunneling. As it is, on most Win32 systems, it's
    * not too big a deal.
    */
    FloodArray = new uint8[sizeX * sizeY * 2];
    EmptyArray = new uint8[sizeX * sizeY * 2];
    FloodStack = new uint8[sizeX * sizeY * 3];
    OpenC = 0;

    GeneratePrompt();

    for (x = 0; x < sizeX; x++)
        for (y = 0; y < sizeY; y++)
            if (!x || !y || x==sizeX-1 || y ==sizeY-1)
                WriteAt(r,x,y,Con[TERRAIN_MAPEDGE],Con[BASE_REGION],PRIO_MAX,true);
            else
                WriteAt(r,x,y,Con[TERRAIN_ROCK],Con[BASE_REGION],PRIO_EMPTY,true);

    /* Copy or create Dungeon-Specials depth/rID list */

    /* Step 2: Place Streamers */
    Streamers = 0;
    sID = 0;
    while (Streamers < (int16)Con[MIN_STREAMERS] || (random(100) < (int16)Con[STREAMER_CHANCE] && Streamers < (int16)Con[MAX_STREAMERS])) {
        GeneratePrompt();
        sx = 1 + random((int16)Con[LEVEL_SIZEX]-2);
        sy = 1 + random((int16)Con[LEVEL_SIZEY]-2);
        if (sID && random(10) > 5)
            goto SecondStreamerSame;

        tot=0;
        for(i=0; StreamWeights[i*2+1]; i++)
            tot += (int16)StreamWeights[i*2+1];
        if (!tot) /* No Streamers Available */
            break;

        for (j=0; j!=20; j++) {
            c = random(tot);
            for(i=0; StreamWeights[i*2+1]; i++)
                if (c < (int16)StreamWeights[i*2+1])
                    goto StreamChosen;
                else
                    c -= (int16)StreamWeights[i*2+1];
            Fatal("Room weights error B, %d %d/%d %d.", i, c, tot, (int16)StreamWeights[i*2+1]);

StreamChosen:
            sID = StreamWeights[i*2];
            if (Depth < (int16)Con[MIN_RIVER_DEPTH])
                if (TREG(sID)->HasFlag(RF_RIVER))
                    continue;
            if (Depth < (int16)Con[MIN_CHASM_DEPTH])
                if (TREG(sID)->HasFlag(RF_CHASM))
                    continue;
            /* No chasms on the last dungeon level, because there is nowhere to fall to! */
            if (Depth == Con[DUN_DEPTH])
                if (TREG(sID)->HasFlag(RF_CHASM))
                    continue;
            break;
        }
        if (j == 20)
            break;

SecondStreamerSame:
        WriteStreamer(r,(uint8)sx,(uint8)sy,-1,sID);
        Streamers++;
    }

    /* multi-level chasms: if last level has a chasm here, 1 in 2 chance
    of chasms narrowing for the entire level. If they do, put a chasm
    here only if the chasm Above has chasms all around it. Otherwise,
    put a chasm here automatically if there is one above. When we are
    later tunneling, there is a good chance a tunnel will turn into a
    rickety bridge, rope bridge, etc. as we cross the chasm. This chance
    decreases with depth, of course...
    */
    GeneratePrompt();
    if (random(100) < (int16)Con[REDUCE_CHASM_CHANCE])
        NarrowChasm = true;

    if (Above && (Depth != Con[DUN_DEPTH]))
        for (x=0;x<sizeX;x++)
            for (y=0;y<sizeY;y++) {
                xtID = Above->RegionAt(x,y);
                if (TREG(xtID)->HasFlag(RF_CHASM)) {
                    if (NarrowChasm) {
                        if (Above->RegionAt(x,y+1) != xtID || Above->RegionAt(x+1,y) != xtID ||
                            Above->RegionAt(x,y-1) != xtID || Above->RegionAt(x-1,y) != xtID)
                            continue;
                    }
                    WriteAt(r,x,y,Above->TerrainAt(x,y),Above->RegionAt(x,y),PRIO_RIVER_STREAMER);
                }
            }

    /* Count Chasm squares; if a lot, avoid making any new chasms this level. */

    /* Step 3: Place Large Submaps */
    if (Above)
        memcpy(SpecialsLevels,Above->SpecialsLevels,64);
    else
        memset(SpecialsLevels,-1,64);

    if (dID) {
        a = TDUN(dID)->Annot(TDUN(dID)->AnHead);
        n=-1;
        while (a) {
            if (a->AnType == AN_DUNSPEC)
                for(i=0;i!=4;i++)
                    if (a->u.ds[i].xID) {
                        rID regID;
                        n++;
                        if (SpecialsLevels[n] == -1) {
                            if (random(100)+1 > a->u.ds[i].Chance) {
                                SpecialsLevels[n] = -10;
                                continue;
                            }
                            SpecialsLevels[n] = (int8)a->u.ds[i].Lev.Roll();
                        }
                        if (Depth != (extraLevs ? (SpecialsLevels[n]*3)/2 : SpecialsLevels[n]))
                            continue;
                        switch(RES(a->u.ds[i].xID)->Type) {
                        case T_TMONSTER:
                        case T_TITEM:               
                        case T_TFEATURE:
                            /* These are added later. */
                            break;
                        case T_TREGION:
                            /* Size of the special room */
                            regID = a->u.ds[i].xID;
                            if (TREG(regID)->Grid) {
                                sx = TREG(regID)->sx;
                                sy = TREG(regID)->sy;
                                if ((sx+2) > sizeX || (sy+2) > sizeY) {
                                    Error("Special room larger than map!");
                                    continue;
                                }

                                /* Number of panels it will take up */
                                cx = 1 + (sx+2) / (int16)Con[PANEL_SIZEX];
                                cy = 1 + (sy+2) / (int16)Con[PANEL_SIZEY];
                                Tries = 0;
NewLocation:
                                /* Displacement into top-left panel */
                                dx = 1 + random((int16)Con[PANEL_SIZEX]*cx - (sx+2));
                                dy = 1 + random((int16)Con[PANEL_SIZEY]*cy - (sy+2));
                                /* Location of top-left panel */
                                px = random(panelsX - (cx-1));
                                py = random(panelsY - (cy-1));
                                /* Write the special region */
                                r.x1 = (uint8)(px*Con[PANEL_SIZEX] + dx);
                                r.y1 = (uint8)(py*Con[PANEL_SIZEY] + dy);
                                r.x2 = r.x1 + sx;
                                r.y2 = r.y1 + sy;
                                /* Avoid placing a special room where the up-stairs
                                need to go to match the down stairs on the level
                                above us. */
                                if (Above)
                                    MapIterate(Above,t,j)
                                        if (t->Type == T_PORTAL && ((Portal*)t)->isDownStairs() && r.Within((uint8)t->x,(uint8)t->y))
                                            if (Tries < 100) {
                                                Tries++;
                                                goto NewLocation;
                                            }
                                // ww: no vault overlap! (or vault / entrance overlaps, or whatever)
                                for (x=0;x<sx;x++)
                                    for (y=0;y<sy;y++)
                                        if (At(r.x1+x,r.y1+y).isVault && Tries < 100) {
                                            Tries++;
                                            goto NewLocation;
                                        }

                                WriteMap(r,a->u.ds[i].xID);
                                /* Touch the used panels */
                                for (x = px; x != px+cx; x++)
                                    for (y = py; y != py+cy; y++)
                                        PanelsDrawn[y] |= (1 << x);
                                {
                                    EventInfo xe;
                                    xe.Clear();
                                    xe.EMap = this;
                                    xe.cPanel = r;
                                    xe.EXVal = (r.x2 + r.x1) / 2; 
                                    xe.EYVal = (r.y2 + r.y1) / 2; 
                                    xe.Event = EV_BIRTH;
                                    rID regID = a->u.ds[i].xID;
                                    TREG(regID)->Event(xe,regID);
                                }
                            } else {
                                int16 xg,yg;
                                xg = (SizeX() + 1) / (int16)Con[PANEL_SIZEX];
                                yg = (SizeY() + 1) / (int16)Con[PANEL_SIZEY];
                                do {
                                    px = random(xg);
                                    py = random(yg);
                                } while (PanelsDrawn[py] & (1 << px));
                                DrawPanel((uint8)px,(uint8)py,regID);                
                            }
                            break;
                        default:
                            Error("Strange resource type as dungeon special!");
                        }
                    }
            a = TDUN(dID)->Annot(a->Next);
        }
    }

    /* Place Predefined Submaps for Dungeon */
    /* See if we place random Submaps as well. */

    /* Step 4: Draw Each Panel */
    for (x=0;x<panelsX;x++)
        for (y=0;y<panelsY;y++)
            if (!(PanelsDrawn[y] & (1 << x))) {
                GeneratePrompt();
                DrawPanel((uint8)x,(uint8)y);
                PanelsDrawn[y] |= (1 << x);
            } 

    /* Step 5: Connect Each Panel */
    {
        int numPanels = panelsX * panelsY;
        struct Point {
            int x;
            int y;
        };
        struct PanelInfo {
            struct Point * O;
            int count;
        } *PI;

        PI = (struct PanelInfo *)malloc(sizeof(struct PanelInfo) * numPanels);
        for (i=0;i<numPanels;i++) {
            PI[i].O = (struct Point*)malloc(sizeof(struct Point) * Con[PANEL_SIZEX] * Con[PANEL_SIZEY]);
            PI[i].count = 0; 
        }

        /* Find Open Spaces In Each Panel */
        for (px=0;px<panelsX;px++)
            for (py=0;py<panelsY;py++) {
                GeneratePrompt();
                int panelNum = py * panelsX + px; 
                for (int16 xx=0;xx<(int16)Con[PANEL_SIZEX];xx++)
                    for (int16 yy=0;yy<(int16)Con[PANEL_SIZEY];yy++) {
                        x = px * (int16)Con[PANEL_SIZEX] + xx;
                        y = py * (int16)Con[PANEL_SIZEY] + yy;
                        if (!SolidAt(x,y)) { 
                            int count = 
                                (!SolidAt(x-1,y)) +
                                (!SolidAt(x+1,y)) +
                                (!SolidAt(x,y+1)) +
                                (!SolidAt(x,y-1));
                            if (count < 4) { 
                                PI[panelNum].O[PI[panelNum].count].x = x;
                                PI[panelNum].O[PI[panelNum].count].y = y;
                                PI[panelNum].count++; 
                            }
                        }
                    }
            } 

        for (px=0;px<panelsX;px++)
            for (py=0;py<panelsY;py++) {
                GeneratePrompt();
                int pn1 = py * panelsX + px;
                int n[3];
                int nc = 0; 
                if (px > 0)
                    n[nc++] = py * panelsX + (px -1);
                if (py > 0)
                    n[nc++] = (py-1) * panelsX + px;
                if (py > 1 && px > 1)
                    n[nc++] = (py-1) * panelsX + (px -1);

                for (int k = 0; k<nc; k++) {
                    int pn2 = n[k]; 
                    int sx, sy, dx, dy;
                    int bestDist = sizeX * sizeY; 

                    /* find points to connect Panel pn1 to Panel pn2 */
                    for (i=0;i<PI[pn1].count;i++)
                        for (j=0;j<PI[pn2].count;j++) { 
                            int thisDist = dist(
                                PI[pn1].O[i].x,
                                PI[pn1].O[i].y,
                                PI[pn2].O[j].x,
                                PI[pn2].O[j].y);
                            if (thisDist < bestDist) { 
                                sx = PI[pn1].O[i].x;
                                sy = PI[pn1].O[i].y;
                                dx = PI[pn2].O[j].x;
                                dy = PI[pn2].O[j].y;
                                bestDist = thisDist; 
                            } 
                        } 
                    if (bestDist < sizeX * sizeY) {
                        Tunnel(sx,sy,dx,dy,TT_DIRECT|TT_WANDER,-1,0);
                    }
                }
            }

        for (i=0;i<numPanels;i++)
            free(PI[i].O);
        free(PI);
    }

    /* ww: don't put all the stairs in the same room */
    rID stairsAt[1024] = { 0, } ; 

    if (Above)
        MapIterate(Above,t,i)
            if (t->Type == T_PORTAL)
                if (((Portal*)t)->isDownStairs()) {
                    /* Paranoia, such as for the not-impossible case of two or more staircases on the same panel. */
#if WEIMER_TUNNEL
                    if (At(t->x,t->y).Solid) {
                        do {
                            dx = random(sizeX);
                            dy = random(sizeY);
                        } while(At(dx,dy).Solid);
                        Tunnel(t->x,t->y,dx,dy,true,random(2) ? NORTH : EAST,TU_NORMAL);

                        /* Make it not a dead-end at the stairs. */
                        if (random(3)) {
                            do {
                                dx = random(sizeX);
                                dy = random(sizeY);
                            } while(At(dx,dy).Solid);
                            Tunnel(t->x,t->y,dx,dy,true,random(2) ? SOUTH : WEST,TU_NORMAL);
                        }
                    }
#endif
                    st = new Portal(Con[STAIRS_UP]);
                    ASSERT(st);
                    if (SolidAt(t->x,t->y)) {
                        WriteAt(r,t->x,t->y,FIND("floor"),RegionAt(t->x,t->y),PRIO_FEATURE_FLOOR);
                        ASSERT(At(t->x,t->y).Solid == 0);
                        for (j=0;j<8;j++)
                            WriteAt(r,t->x+DirX[j],t->y+DirY[j],FIND("dungeon wall"),RegionAt(t->x,t->y),PRIO_CORRIDOR_WALL);
                    } 
                    st->PlaceAt(this,t->x,t->y);
                    if (RegionAt(t->x,t->y))  {
                        for (j=0;stairsAt[j];j++)
                            ;
                        stairsAt[j] = RegionAt(t->x,t->y);
                    }
                }

    /* Step 5: Final, Fix-Up Tunneling */
    { 
        struct P {
            int x;
            int y;
        } *C, *U; 
        struct Pair {
            int sx, sy;
            int dx, dy;
            int dist; 
            rID reg;
        } *best; 

        C = (struct P *)calloc(sizeof(C[0]),sizeX*sizeY);
        U = (struct P *)calloc(sizeof(U[0]),sizeX*sizeY);
        best = (struct Pair *)calloc(sizeof(best[0]),sizeX);
        ASSERT(C);
        ASSERT(U);
        ASSERT(best);
        int trials = 0;
        for (x=0;x<sizeX;x++)
            for (y=0;y<sizeY;y++) 
                At(x,y).Connected = false; 

        fCount = 0; 
        for (x=1;x<sizeX;x++)
            for (y=1;y<sizeY;y++) {
                if (!SolidAt(x,y) && !At(x,y).Connected) {
                    fCount = (int16)FloodConnectA(x,y,fCount);
                    x = sizeX;
                    y = sizeY; 
                }
            } 

        while (trials++ < 26) {
            GeneratePrompt();
            int cCount = 0; 
            int uCount = 0; 
            int bestCount = 0;
            int k;

            for (i=0;i<2;i++)
                best[i].reg = -1; 

            for (x=1;x<sizeX-1;x++)
                for (y=1;y<sizeY-1;y++) 
                    if (!SolidAt(x,y)) {
                        if (At(x,y).Connected) {
                            int count = 
                                (At(x-1,y).Connected != 0) +
                                (At(x+1,y).Connected != 0) +
                                (At(x,y+1).Connected != 0) +
                                (At(x,y-1).Connected != 0);
                            if (count < 4) {
                                C[cCount].x = x;
                                C[cCount++].y = y;
                            }
                        } else {
                            int count = 
                                (!SolidAt(x-1,y)) +
                                (!SolidAt(x+1,y)) +
                                (!SolidAt(x,y+1)) +
                                (!SolidAt(x,y-1));
                            if (count < 4) { 
                                U[uCount].x = x;
                                U[uCount++].y = y; 
                                bool found = false;
                                rID thisReg = RegionAt(x,y);
                                for (k=0; !found && k < bestCount; k++)
                                    if (best[k].reg == thisReg)
                                        found = true;
                                if (!found) {
                                    best[bestCount].reg = thisReg;
                                    best[bestCount].dist = sizeX * sizeY; 
                                    bestCount++;
                                }
                            }
                        } 
                    }

            if (uCount == 0) break; 

            for (i=0;i<cCount;i++)
                for (j=0;j<uCount;j++) { 
                    rID r = RegionAt(U[j].x,U[j].y);
                    for (k=0;k<bestCount;k++)
                        if (best[k].reg == r) {
                            int thisDist = dist(C[i].x,C[i].y,U[j].x,U[j].y);
                            if (thisDist < best[k].dist) {
                                best[k].sx = C[i].x;
                                best[k].sy = C[i].y;
                                best[k].dx = U[j].x;
                                best[k].dy = U[j].y;
                                best[k].dist = thisDist;
                            } 
                        } 
                }

            for (i=0;i<3;i++) {
                int md = sizeX * sizeY;
                int mi = -1;
                for (j=0;j<bestCount;j++)
                    if (best[j].dist < md) {
                        md = best[j].dist;
                        mi = j;
                    }
                if (mi == -1)
                    break;

                best[mi].dist = sizeX * sizeY;
                At(best[mi].sx,best[mi].sy).Connected = false; 
                /*
                theGame->GetPlayer(0)->IPrint("<Res> -> <Res2>.",
                RegionAt(best[mi].sx,best[mi].sy),
                RegionAt(best[mi].dx,best[mi].dy));
                */
                Tunnel(best[mi].sx, best[mi].sy,best[mi].dx, best[mi].dy,TT_DIRECT|TT_WANDER,-1,trials);
                fCount = (int16)FloodConnectA((int16)best[mi].sx,(int16)best[mi].sy,0);
            }
        }

        free(C);
        free(U);
        free(best);
    }

    /*
    for(x=1;x<sizeX-1;x++)
    for(y=1;y<sizeY-1;y++)
    if (At(x,y).Connected == false)
    if (!At(x,y).Solid)
    {
    At(x,y).Shade = 0;
    At(x,y).Glyph |= 0xB000;
    }
    */

    for(x=1;x<sizeX-1;x++)
        for(y=1;y<sizeY-1;y++)
            At(x,y).Connected = false;

    /* Step 6: Place Stairs & Required Monsters/Items */
    GeneratePrompt();    

    if ((int16)Con[MAX_STAIRS] && Depth < (int16)Con[DUN_DEPTH]) {
        j = (int16)Con[MIN_STAIRS] + random((int16)(Con[MAX_STAIRS] - Con[MIN_STAIRS]));
        for (i=0;i!=j;i++) {
            Tries = 0;
            do {
                x = random((int16)Con[LEVEL_SIZEX]);
                y = random((int16)Con[LEVEL_SIZEY]);
                if (Tries++ > 500)
                    break;
                bool already = false; 
                for (int k=0;stairsAt[k];k++)
                    if (RegionAt(x,y) == stairsAt[k])
                        already = true;
                if (already) continue; 
            }
            while(At(x,y).Solid || (At(x,y).Priority > PRIO_ROOM_FLOOR) ||
                TTER(TerrainAt(x,y))->HasFlag(TF_WATER) ||
                TTER(TerrainAt(x,y))->HasFlag(TF_FALL));
            st = new Portal(Con[STAIRS_DOWN]);
            ASSERT(st)
            st->PlaceAt(this,x,y);
        }
    }

    if (dID) {
        a = TDUN(dID)->Annot(TDUN(dID)->AnHead);
        n=-1;
        while (a) {
            if (a->AnType == AN_DUNSPEC)
                for(i=0;i!=4;i++)
                    if (a->u.ds[i].xID) {
                        n++;
                        if (SpecialsLevels[n] == -1) {
                            if (random(100)+1 > a->u.ds[i].Chance) {
                                SpecialsLevels[n] = -10;
                                continue;
                            }
                            SpecialsLevels[n] = (int8)a->u.ds[i].Lev.Roll();
                        }
                        if (SpecialsLevels[n] != Depth)
                            continue;
                        if (RES(a->u.ds[i].xID)->Type == T_TREGION)
                            continue;
                        Tries = 0;
                        do {
                            x = random((int16)Con[LEVEL_SIZEX]);
                            y = random((int16)Con[LEVEL_SIZEY]);
                            if (Tries++ > 100)
                                break;
                        }
                        while(At(x,y).Solid || 
                            (At(x,y).Priority > PRIO_ROOM_FLOOR) ||
                            TTER(TerrainAt(x,y))->HasFlag(TF_WATER) ||
                            TTER(TerrainAt(x,y))->HasFlag(TF_FALL));
                        if (RES(a->u.ds[i].xID)->Type == T_TMONSTER) {
                            THROW(EV_ENGEN,
                                xe.enID = FIND("unique encounter");
                                xe.enCR = DepthCR;
                                xe.enDepth = DepthCR;
                                xe.enRegID = dID;
                                xe.enConstraint = a->u.ds[i].xID;
                                xe.EXVal = x;
                                xe.EYVal = y;
                                xe.isLoc = true;
                            );
                        } else if (RES(a->u.ds[i].xID)->Type == T_TITEM) {
                            it = Item::Create(a->u.ds[i].xID);
                            if (!it)
                                continue;
                            it->PlaceAt(this,x,y);
                        }
                    }
            a = TDUN(dID)->Annot(a->Next);
        }
    }

    if (dID) {
        a = TDUN(dID)->Annot(TDUN(dID)->AnHead);
        n=-1;
        while (a) {
            if (a->AnType == AN_DUNSPEC)
                for (i=0;i!=4;i++)
                    if (a->u.ds[i].xID) {
                        n++;
                        xID = a->u.ds[i].xID;
                        if (SpecialsLevels[n] == -1) {
                            if (random(100)+1 > a->u.ds[i].Chance) {
                                SpecialsLevels[n] = -10;
                                continue;
                            }
                            SpecialsLevels[n] = (int8)a->u.ds[i].Lev.Roll();
                        }
                        if (SpecialsLevels[n] != Depth)
                            continue;
                        switch(RES(xID)->Type) {
                        case T_TMONSTER:
                        case T_TITEM:               
                        case T_TFEATURE:
                            do {
                                x = 2 + random(sizeX-4);
                                y = 2 + random(sizeY-4);
                                if (At(x,y).Solid)
                                    continue;
                                if (FFeatureAt(x,y))
                                    continue;
                                if (FCreatureAt(x,y))
                                    continue;
                                if (At(x,y).isVault)
                                    continue;
                                if (TTER(TerrainAt(x,y))->HasFlag(TF_WATER))
                                    continue;
                                if (TTER(TerrainAt(x,y))->HasFlag(TF_WARN))
                                    continue;
                                break;
                            } while (1);
                            t=NULL;
                            if (RES(xID)->Type == T_TFEATURE) {
                                if (TFEAT(xID)->FType == T_PORTAL)
                                    t = new Portal(xID);
                                else if (TFEAT(xID)->FType == T_TRAP)
                                    t = new Trap(xID,0);
                                else
                                    t = new Feature(TFEAT(xID)->Image,xID,T_FEATURE);
                            } else
                                Error("Unimplemented dungeon special!");
                            if (t) {
                                t->PlaceAt(this,x,y);
                            }
                            break;   
                        case T_TREGION:
                            /* These have been done already. */
                            break;
                        }
                    }
            a = TDUN(dID)->Annot(a->Next);
        }
    }

    GeneratePrompt();
    /* Validate the stairs */
    MapIterate(this,t,i)
        if (t->Type == T_PORTAL) {
            ASSERT(At(t->x,t->y).Solid == false);
            while (t->Next) {
                oThing(t->Next)->Remove(false);
            } 
        } 

    /* Validate the traps -- we don't want traps in water for aesthetic
    reasons, and also because water deals somewhat unfairly with
    dropped items at the moment. */
ValidateTraps:
    MapIterate(this,t,i)
        if (t->Type == T_TRAP || t->Type == T_FOUNTAIN)
            if (TTER(TerrainAt(t->x,t->y))->HasFlag(TF_WATER) || TTER(TerrainAt(t->x,t->y))->HasFlag(TF_DEEP_LIQ)) {
                t->Remove(true);
                goto ValidateTraps;
            }

    GeneratePrompt();
    /* Validate the doors */
StartAgain:
    MapIterate(this,t,i)
        if (t->Type == T_DOOR) {
            if (!At(t->x-1,t->y).Solid || !At(t->x+1,t->y).Solid)
                if (!At(t->x,t->y-1).Solid || !At(t->x,t->y+1).Solid) {
                    At(t->x,t->y).Solid = TTER(TerrainAt(t->x,t->y))->HasFlag(TF_SOLID);
                    t->Remove(false);
                    delete t;
                    goto StartAgain;
                }
            for(j=0;j!=8;j++)
                if (t2 = FDoorAt(t->x+DirX[j],t->y+DirY[j])) {
                    At(t->x,t->y).Solid =  TTER(TerrainAt(t->x,t->y))->HasFlag(TF_SOLID);
                    At(t2->x,t2->y).Solid = TTER(TerrainAt(t2->x,t2->y))->HasFlag(TF_SOLID);
                    t2->Remove(false);
                    delete t2;
                    t->Remove(false);
                    delete t;
                    goto StartAgain;
                }
            WriteAt(r, t->x, t->y,
                TREG(RegionAt(t->x,t->y))->Floor ? TREG(RegionAt(t->x,t->y))->Floor : FIND("floor"),
                RegionAt(t->x,t->y),PRIO_FEATURE_FLOOR);
            ((Door*)t)->SetImage();
        } else if (((t->Image & 0xFF) == '<') && Depth <= 5) {
            /* Remove all secret doors near the up stairs,
            to prevent the player from being 'boxed in'
            and starving on a new level. */
            MapIterate(this,t2,j)
                if (t2->isType(T_DOOR))
                    if (dist(t->x,t->y,t2->x,t2->y) <= 17) {
                        ((Door*)t2)->DoorFlags &= ~DF_SECRET;
                        t2->SetImage();
                    }
        }

    /* Assign Gods to unassigned altars */
    Thing *altar;
    for(altar=FirstThing();altar;altar=NextThing())
        if (altar->isFeature() && (altar->Flags & F_ALTAR))
            if (!altar->HasStati(MY_GOD)) {
                rID gID;
                switch(random(8)) {
                case 0: gID = FIND("Asherath"); break;
                case 1: gID = FIND("Khasrach"); break;
                case 2: gID = FIND("Xel"); break;
                case 3: gID = FIND("Aiswin"); break;
                case 4: gID = FIND("Zurvash"); break;
                case 5: gID = FIND("Kysul"); break;
                default: gID = FIND("the Multitude"); break;
                }
                altar->GainPermStati(MY_GOD,NULL,SS_MISC,0,0,gID);
            }

    /* Here, we find instances of the terrain "shallow water", and if it is
    surrounded on all sides by "shallow water", we replace it with "deep
    water". Generally, we avoid refering to terrain types directly, but
    in this case the speed factor (scanning the entire map) justifies not
    messing around with flags or somesuch. */
    GeneratePrompt();
    rID shID[3], deID[3];
    shID[0] = FIND("shallow water");
    deID[0] = FIND("deep water");
    // ww: and also with brimstone and magma
    shID[1] = FIND("brimstone");
    deID[1] = FIND("magma");
    rID fireWall = FIND("solid igneous rock"); 

    shID[2] = FIND("smooth igneous rock");
    deID[2] = FIND("obsidian");
    ASSERT(shID[0]);
    ASSERT(shID[1]);
    ASSERT(shID[2]);
    ASSERT(deID[0]);
    ASSERT(deID[1]);
    ASSERT(deID[2]);

    GeneratePrompt();

    for(x=0;x<(int16)Con[LEVEL_SIZEX];x++)
        for(y=0;y<(int16)Con[LEVEL_SIZEY];y++) {
            if (!FDoorAt(x,y)) {
                At(x,y).Solid = TTER(TerrainAt(x,y))->HasFlag(TF_SOLID);
            }
            if (TTER(TerrainAt(x,y))->HasFlag(TF_WALL)) {
                bool found = false; 
                for (int z=0;z<9 && !found;z++) {
                    if (InBounds(x+DirX[z],y+DirY[z])) {
                        rID t = TerrainAt(x+DirX[z],y+DirY[z]);
                        if (t == shID[1] || t == deID[1] || t == shID[2] || t == deID[2])
                            found = true;
                    } 
                } 
                if (found) {
                    WriteAt(r,x,y,fireWall,RegionAt(x,y),PRIO_ROOM_WALL_MODIFIER);  
                } 
            }
        }

    GeneratePrompt();

    /* fjm: Modified May 14 2006 so that corridors don't have deep water
    (or other "deep" terrains) in them for ease of gameplay. */ 
    for(x=0;x<(int16)Con[LEVEL_SIZEX];x++)
        for(y=0;y<(int16)Con[LEVEL_SIZEY];y++)
            for (i=0;i<3;i++) 
                if (TerrainAt(x,y) == shID[i] && !TREG(RegionAt(x,y))->HasFlag(RF_CORRIDOR))
                    if (!InBounds(x-1,y-1) || TerrainAt(x-1,y-1) == shID[i] || TerrainAt(x-1,y-1) == deID[i] || At(x-1,y-1).Solid)
                        if (!InBounds(x-1,y) || TerrainAt(x-1,y  ) == shID[i] || TerrainAt(x-1,y  ) == deID[i] || At(x-1,y  ).Solid)
                            if (!InBounds(x-1,y+1) || TerrainAt(x-1,y+1) == shID[i] || TerrainAt(x-1,y+1) == deID[i] || At(x-1,y+1).Solid)
                                if (!InBounds(x  ,y+1) || TerrainAt(x  ,y+1) == shID[i] || TerrainAt(x  ,y+1) == deID[i] || At(x  ,y+1).Solid)
                                    if (!InBounds(x+1,y+1) || TerrainAt(x+1,y+1) == shID[i] || TerrainAt(x+1,y+1) == deID[i] || At(x+1,y+1).Solid)
                                        if (!InBounds(x+1,y) || TerrainAt(x+1,y  ) == shID[i] || TerrainAt(x+1,y  ) == deID[i] || At(x+1,y  ).Solid)
                                            if (!InBounds(x+1,y-1) || TerrainAt(x+1,y-1) == shID[i] || TerrainAt(x+1,y-1) == deID[i] || At(x+1,y-1).Solid)
                                                if (!InBounds(x  ,y-1) || TerrainAt(x  ,y-1) == shID[i] || TerrainAt(x  ,y-1) == deID[i] || At(x  ,y-1).Solid)
                                                    WriteAt(r,x,y,deID[i],RegionAt(x,y),At(x,y).Priority,true);  

#ifndef OLD_TRAP_METHOD
    /* We put the trap at the T in the following places:
    *  .
    * #T#
    *  .
    *
    *  #
    * .T.
    *  #
    *
    * (where the T was assumed to be empty before). Theory: Kobolds are
    * smart, they'll trap bottlenecks. 
    */
    for(x=1;x<(int16)Con[LEVEL_SIZEX]-1;x++)
        for(y=1;y<(int16)Con[LEVEL_SIZEY]-1;y++) {
            if (FDoorAt(x,y) && (random((int16)Con[TRAP_CHANCE]) <= (int16)(DepthCR+10))) {
                rID tID = theGame->GetEffectID(PUR_DUNGEON,0,(int8)DepthCR,AI_TRAP);
                if (tID) {
                    Trap * tr = new Trap(FIND("trap"),tID);
                    tr->PlaceAt(this,x,y);
                }
            } else if (!At(x,y).Solid && !FFeatureAt(x,y)) {
                bool up,down,left,right;
                up =          At(x,y-1).Solid && !FDoorAt(x,y-1);
                down =        At(x,y+1).Solid && !FDoorAt(x,y+1);
                left =        At(x-1,y).Solid && !FDoorAt(x-1,y);
                right =       At(x+1,y).Solid && !FDoorAt(x+1,y);
                if ((!up && !down && left && right) || (up && down && !left &&!right)) {
                    if (random((int16)Con[TRAP_CHANCE]) <= (int16)((DepthCR+2)/3))  {
                        rID tID = theGame->GetEffectID(PUR_DUNGEON,0,(int8)DepthCR,AI_TRAP);
                        if (tID) {
                            Trap * tr = new Trap(FIND("trap"),tID);
                            tr->PlaceAt(this,x,y);
                        }
                    }
                }
            }
        } 

#endif
    GeneratePrompt();

    /* Place Treasure Deposits for Miners */
    int16 cDeposits;
    cDeposits = Dice::Roll(1,4,(int8)Depth);
    for (n=0;n!=cDeposits;n++) {
        rID depID;
        do {
            x = 1 + random(sizeX-1);
            y = 1 + random(sizeY-1);
            if ((TTER(TerrainAt(x,y))->Image & 0xFF) != GLYPH_ROCK)
                goto TryAgain;
            for (j=0;j!=8;j++)
                if (InBounds(x+DirX[j],y+DirY[j]) && !SolidAt(x+DirX[j],y+DirY[j]))
                    goto TryAgain;
            if ((TerrainAt(x,y) == Con[TERRAIN_ROCK]) && random(7))
                goto TryAgain;
            break;
TryAgain:;
        } while (1);

        nCandidates = 0;
        for (int mIdx=0;theGame->Modules[mIdx];mIdx++)
            for (i=0; i<theGame->Modules[mIdx]->szTer; i++) {
                depID = theGame->Modules[mIdx]->TerrainID(i);
                if (TTER(depID)->HasFlag(TF_DEPOSIT))
                    if ((int16)TTER(depID)->GetConst(DEPOSIT_DEPTH) <= Depth)
                        Candidates[nCandidates++] = depID;
            }
            if (!nCandidates)
                break;
            depID = Candidates[random(nCandidates)];

            WriteAt(r,x,y,depID,RegionAt(x,y),PRIO_DEPOSIT);
    }

    /* Calculate light */
    r.Set(0,0,sizeX-1,sizeY-1);
    calcLight(r);

    /* Mark Skylights */
    if (Depth > 1 && Above) {
        rID trID; bool isChasm;
        trID = 0;

        for (x=0;x!=sizeX;x++)
            for (y=0;y!=sizeY;y++) {
                if (trID != Above->TerrainAt(x,y)) {
                    trID = Above->TerrainAt(x,y);
                    isChasm = TTER(trID)->HasFlag(TF_FALL);
                }
                if (isChasm) {
                    At(x,y).isSkylight = true;
                    if ((At(x,y).Glyph & 0xFF) == GLYPH_FLOOR) {
                        At(x,y).Glyph &= 0xF0FF;
                        At(x,y).Glyph |= CYAN * 256;
                        At(x,y).Shade = false;
                    }
                }
                else
                    At(x,y).isSkylight = false;
            }
    } else {
        for (x=0;x!=sizeX;x++)
            for (y=0;y!=sizeY;y++)
                At(x,y).isSkylight = false;
    }

    /* Step 7: Deallocation & Return */
    free(GridBackup);
    delete[] FloodArray;
    delete[] FloodStack;
    delete[] EmptyArray;

    GeneratePrompt();
    MapIterate(this,t,i) 
        if (t->isType(T_MONSTER)) 
            ((Monster*)t)->Initialize();

    GeneratePrompt();        
    /* Remove Temporary Priority Values */
    for (x=0;x<sizeX;x++)
        for (y=0;y<sizeY;y++)
            At(x,y).Priority = 0;

    /* HACKFIX */
    int16 capCR = Depth + (theGame->Opt(OPT_OOD_MONSTERS) ? 3 : 1);
    rID campID = FIND("The Goblin Encampment");
RestartVerifyMon:
    if (theGame->Opt(OPT_DIFFICULTY) != DIFF_NIGHTMARE) {
        MapIterate(this,t,i)
            if (t->isType(T_MONSTER))
                if (((Creature*)t)->ChallengeRating() > capCR) {
                    if (theGame->GetPlayer(0))
                        if (((Creature*)t)->isFriendlyTo(theGame->GetPlayer(0)))
                            continue;
                    if (RegionAt(t->x,t->y) == campID)
                        continue;
                    t->Remove(true);
                    goto RestartVerifyMon;
                }
    }

    /* Kludge to make sure that no fortress enemies are neutral */
    if (Depth == 10) {
        Creature *cr; int32 i;
        MapIterate(this,cr,i)
            if (cr->isCreature() && RegionAt(cr->x,cr->y) == campID) {
                cr->RemoveStati(ALIGNMENT);
                cr->StateFlags &= ~(MS_PEACEFUL);
                cr->GainPermStati(ALIGNMENT,NULL,SS_MISC, AL_LAWFUL|AL_EVIL,AL_LAWFUL|AL_EVIL,0,0);
                cr->ts.Retarget(cr,true);
            }
    }

    inGenerate = false;

    T1->SetWin(WIN_INPUT);
    T1->Clear();
}

void Map::DrawPanel(uint8 px, uint8 py, rID regID) {
    int16 i,j,c,sx,sy,RType,Tries,x,y,weight,RM_WeightIndex;
    int16 weighting_sum;
    Rect cPanel, r,r2,r3,r4;
    rID WallID, FloorID, xID;
    static Rect NULL_RECT(0,0,0,0);
    static EventInfo xe;

    int16 DepthCR = AdjustCR((int16)(Con[INITIAL_CR] + (Depth*Con[DUN_SPEED])/100 - 1)); 

    cPanel.x1 = px * (uint8)Con[PANEL_SIZEX];
    cPanel.y1 = py * (uint8)Con[PANEL_SIZEY];
    cPanel.x2 = cPanel.x1 + (uint8)Con[PANEL_SIZEX] - 1;
    cPanel.y2 = cPanel.y1 + (uint8)Con[PANEL_SIZEY] - 1;
    PanelsDrawn[py] |= 1 << px;
    OpenC = 0;
    nCorners = 0;
    nCenters = 0;
    PurgeStrings();

    if (regID) {
        int16 RTypes[32], c;
        c = 0;
        for (i=0;i!=32;i++)
            if (BIT(i) & TREG(regID)->RoomTypes)
                RTypes[c++] = i;
        if (!c)
            return;
        RType = RTypes[random(c)];
        goto PresetRegion;
    }

    Tries = 0;
    TDUN(dID)->GetList(RM_WEIGHTS,RM_Weights,RM_LAST*2);

Reselect:
    if (Tries >= 200) {
        Error("Two hundred tries and no viable room/region combo.");
    }

    r.x1 = r2.x1 = r3.x1 = r4.x1 = -1; 
    r.x2 = r2.x2 = r3.x2 = r4.x2 = 0; 

    // Sum up all the room type weightings.
    weighting_sum = 0;
    for(i=0; RM_Weights[i*2+1]; i++)
        weighting_sum += max(0, (int16)RM_Weights[i*2+1]);

    // If all room types are used up, free them all up for reuse.
    if (weighting_sum == 0) {
        if (usedInThisLevel[0]) {
            if (theGame->GetPlayer(0)->WizardMode)
                theGame->GetPlayer(0)->IPrint("The air seems to shift.");
            memset(usedInThisLevel,0,sizeof(usedInThisLevel));
            /* Reset the weightings (which must be all marked invalid). */
            TDUN(dID)->GetList(RM_WEIGHTS,RM_Weights,RM_LAST*2);
            goto Reselect;
        } else {
            // None to use, none to reuse.  No recovery from this.
            Fatal("Complete failure to generate a room.");
        }
    }

    // Pick a room type, leave the selection index in 'i'.
    c = random(weighting_sum);
    for(i=0; RM_Weights[i*2+1]; i++) {
        weight = max(0, (int16)RM_Weights[i*2+1]);
        ASSERT(weight != -1)
        if (c < weight) {
            RM_WeightIndex = i*2;
            goto RM_Chosen;
        } else
            c -= weight;
    }
    Fatal("Room weights error A, %d %d %d %d.", i, c, weight, Tries);

RM_Chosen:
    RType = (int16)RM_Weights[RM_WeightIndex+0];

    /* Choose Region Type (ignoring weights for now) */
    for(i=c=0;RoomWeights[i*2];i++) {
        if (!TREG(RoomWeights[i*2])->RoomTypes || (TREG(RoomWeights[i*2])->RoomTypes & BIT(RType)))
            if (DepthCR >= TREG(RoomWeights[i*2])->Depth)
                if (!TREG(RoomWeights[i*2])->HasFlag(RF_VAULT) || DepthCR >= (int16)Con[MIN_VAULT_DEPTH]) {
                    if (!TREG(RoomWeights[i*2])->HasFlag(RF_CORRIDOR)) {
                        for (j=0; usedInThisLevel[j]; j++) {
                            if (usedInThisLevel[j] == RoomWeights[i*2])
                                goto SkipCandidate;
                        }
                        Candidates[c++] = RoomWeights[i*2];
                    }
                }
SkipCandidate:
        ;
    }

    if (!c) {
        // No candidates left, disregard this room type from selection.
        RM_Weights[RM_WeightIndex+1] = -1;
        Tries++;
        goto Reselect;
    }

    regID = Candidates[random(c)];
    for (i=0; usedInThisLevel[i]; i++)
        ;
    usedInThisLevel[i] = regID;

    /*regID = FIND("ice matrix");
    RType = RM_CASTLE;*/

PresetRegion:
    xe.Clear();
    xe.EMap = this;
    xe.cPanel = cPanel;
    xe.cRoom = NULL_RECT;

    for(x=cPanel.x1;x<=cPanel.x2;x++)
        for(y=cPanel.y1;y<=cPanel.y2;y++) {
            Thing *t;
            while (t = FirstAt(x,y))
                t->Remove(true);
        }

    r.x1 = r2.x1 = r3.x1 = r4.x1 = -1; 
    r.x2 = r2.x2 = r3.x2 = r4.x2 = 0; 

    switch(RType) {
    case RM_NOROOM:
        /* Simple, no? Remember that it doesn't need to be touched, though. */
        RoomsTouched[py] |= (1 << px);
        return;
    case RM_NORMAL:
DoBasicRoom:
        sx = (int16)(Con[ROOM_MINX] + random((int16)(Con[ROOM_MAXX] - Con[ROOM_MINX])));
        sy = (int16)(Con[ROOM_MINY] + random((int16)(Con[ROOM_MAXY] - Con[ROOM_MINY])));
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
        { if (sx == Con[ROOM_MAXX]) sx--; else sx++; }
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
        { if (sx == Con[ROOM_MAXY]) sy--; else sy++; }
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);          
        WriteRoom(r,regID);
        xe.cRoom = r;
        break; 
    case RM_CIRCLE:
        sx = (int16)(Con[ROOM_MINX] + random((int16)(Con[ROOM_MAXX] - Con[ROOM_MINX])));
        sy = (int16)(Con[ROOM_MINY] + random((int16)(Con[ROOM_MAXY] - Con[ROOM_MINY])));
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
        { if (sx == Con[ROOM_MAXX]) sx--; else sx++; }
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
        { if (sx == Con[ROOM_MAXY]) sy--; else sy++; }
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        WriteCircle(r,regID);
        WriteWalls(r,regID);
        xe.cRoom = r;
        break; 
    case RM_LCIRCLE:
        sx = (int16)max(Con[PANEL_SIZEX]/2,Con[PANEL_SIZEX] - (random(5)+5));
        sy = (int16)max(Con[PANEL_SIZEY]/2,Con[PANEL_SIZEY] - (random(5)+5));          
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
            sx++;
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
            sy++;
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        WriteCircle(r,regID);
        WriteWalls(r,regID);
        xe.cRoom = r;
        break; 
    case RM_LARGE:
        sx = (int16)max(Con[PANEL_SIZEX]/2,Con[PANEL_SIZEX] - (random(8)+2));
        sy = (int16)max(Con[PANEL_SIZEY]/2,Con[PANEL_SIZEY] - (random(8)+2));
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
            sx++;
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
            sy++;
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        WriteRoom(r,regID);
        xe.cRoom = r;
        break;
    case RM_MAZE:
        sx = (int16)max(Con[PANEL_SIZEX]/2,Con[PANEL_SIZEX] - (random(8)+2));
        sy = (int16)max(Con[PANEL_SIZEY]/2,Con[PANEL_SIZEY] - (random(8)+2));
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        WriteMaze(r,regID);
        xe.cRoom = r;
        break;
    case RM_ADJACENT:
    case RM_AD_ROUND:
    case RM_AD_MIXED:
        bool DrawnAny;
        /* Four boxes with a shared center-point compose a
        single room; one or more may be circles instead.
        ___
        |  |_______
        |r        |
        |      r2 |
        ---  +   ----
        | r3   r4|  
        |     ___|
        |____|
        */
        r.x2 = r2.x1 = r3.x2 = r4.x1 = cPanel.x1 + (uint8)Con[PANEL_SIZEX]/2;
        r.y2 = r2.y2 = r3.y1 = r4.y1 = cPanel.y1 + (uint8)Con[PANEL_SIZEY]/2;

        r.x1 = cPanel.x1 + (uint8)random((int16)Con[PANEL_SIZEX]/4); 
        r.y1 = cPanel.y1 + (uint8)random((int16)Con[PANEL_SIZEY]/4);

        r2.x2 = cPanel.x2 - (uint8)random((int16)Con[PANEL_SIZEX]/4);
        r2.y1 = cPanel.y1 + (uint8)random((int16)Con[PANEL_SIZEY]/4);

        r3.x1 = cPanel.x1 + (uint8)random((int16)Con[PANEL_SIZEX]/4);
        r3.y2 = cPanel.y2 - (uint8)random((int16)Con[PANEL_SIZEY]/4);

        r4.x2 = cPanel.x2 - (uint8)random((int16)Con[PANEL_SIZEX]/4);
        r4.y2 = cPanel.y2 - (uint8)random((int16)Con[PANEL_SIZEY]/4);

        r.x1 += 2;
        r.y1 += 2;

        r2.x2 -= 2;
        r2.y1 += 2;

        r3.x1 += 2;
        r3.y2 -= 2;

        r4.x2 -= 2;
        r4.y2 -= 2;

        DrawnAny = false;
        while (!DrawnAny) {
            if (random(4)) {
                DrawnAny = true;
                if (RType != RM_ADJACENT && (RType == RM_AD_ROUND || !random(2)))
                    WriteCircle(r,regID);
                else
                    WriteBox(r,regID);
            }

            if (random(4)) {
                DrawnAny = true;
                if (RType != RM_ADJACENT && (RType == RM_AD_ROUND || !random(2)))
                    WriteCircle(r2,regID);
                else
                    WriteBox(r2,regID);
            }

            if (random(4)) {
                DrawnAny = true;
                if (RType != RM_ADJACENT && (RType == RM_AD_ROUND || !random(2)))
                    WriteCircle(r3,regID);
                else
                    WriteBox(r3,regID);
            }

            if (random(4)) {
                DrawnAny = true;
                if (RType != RM_ADJACENT && (RType == RM_AD_ROUND || !random(2)))
                    WriteCircle(r4,regID);
                else
                    WriteBox(r4,regID);
            }

        }
        WriteWalls(cPanel,regID);

        break;
    case RM_OVERLAP:
        sx = (int16)((Con[ROOM_MINX] + random((int16)(Con[ROOM_MAXX] - Con[ROOM_MINX])))*2)/3;
        sy = (int16)((Con[ROOM_MINY] + random((int16)(Con[ROOM_MAXY] - Con[ROOM_MINY])))*2)/3;
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
        { if (sx == Con[ROOM_MAXX]) sx--; else sx++; }
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
        { if (sx == Con[ROOM_MAXY]) sy--; else sy++; }
        r = cPanel.PlaceWithinSafely((int8)sx,(int8)sy);
        WriteRoom(r,regID);

        sx = (int16)((Con[ROOM_MINX] + random((int16)(Con[ROOM_MAXX] - Con[ROOM_MINX])))*2)/3;
        sy = (int16)((Con[ROOM_MINY] + random((int16)(Con[ROOM_MAXY] - Con[ROOM_MINY])))*2)/3;
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
        { if (sx == Con[ROOM_MAXX]) sx--; else sx++; }
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
        { if (sx == Con[ROOM_MAXY]) sy--; else sy++; }
        do
        r2 = cPanel.PlaceWithinSafely((int8)sx,(int8)sy);
        while (!r.Overlaps(r2));
        WriteRoom(r2,regID);
        if (!random(3))
            break;

        sx = (int16)((Con[ROOM_MINX] + random((int16)(Con[ROOM_MAXX] - Con[ROOM_MINX])))*2)/3;
        sy = (int16)((Con[ROOM_MINY] + random((int16)(Con[ROOM_MAXY] - Con[ROOM_MINY])))*2)/3;
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
        { if (sx == Con[ROOM_MAXX]) sx--; else sx++; }
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
        { if (sx == Con[ROOM_MAXY]) sy--; else sy++; }
        do
        r3 = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        while (!r.Overlaps(r3) && !r2.Overlaps(r3));
        WriteRoom(r3,regID);
        if(!random(2))
            break;

        sx = (int16)((Con[ROOM_MINX] + random((int16)(Con[ROOM_MAXX] - Con[ROOM_MINX])))*2)/3;
        sy = (int16)((Con[ROOM_MINY] + random((int16)(Con[ROOM_MAXY] - Con[ROOM_MINY])))*2)/3;
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
        { if (sx == Con[ROOM_MAXX]) sx--; else sx++; }
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
        { if (sx == Con[ROOM_MAXY]) sy--; else sy++; }
        do
        r4 = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        while (!r.Overlaps(r4) && !r2.Overlaps(r4) && !r3.Overlaps(r4));
        WriteRoom(r4,regID);

        break;
    case RM_OCTAGON:
        sx = (int16)(Con[ROOM_MINX] + random((int16)(Con[ROOM_MAXX] - Con[ROOM_MINX])) + 3);
        sy = (int16)(Con[ROOM_MINY] + random((int16)(Con[ROOM_MAXY] - Con[ROOM_MINY])) + 3);
        sx = max(sx, 9);
        sy = max(sy, 9);
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
        { if (sx == Con[ROOM_MAXX]) sx--; else sx++; }
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
        { if (sx == Con[ROOM_MAXY]) sy--; else sy++; }
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        WriteOctagon(r,regID);
        WriteWalls(r,regID);
        xe.cRoom = r;
        break; 
    case RM_DIAMONDS:
        int16 d, i, c, dx[8], dy[8], dc;
        r.x1 = cPanel.x1 + 7;
        r.y1 = cPanel.y1 + 7;
        r.x2 = cPanel.x2 - 7;
        r.y2 = cPanel.y2 - 7;
        x = r.x1 + random(r.x2 - r.x1);
        y = r.y1 + random(r.y2 - r.y1);
        i = 1 + random(7); c = 0; dc = 0;
        do {
            WriteDiamond(x,y,regID);
            c++;
            do 
            d = NORTHEAST + random(4);
            while (!r.Within(x + DirX[d]*6, y + DirY[d]*6));

            x += DirX[d]*6;
            y += DirY[d]*6;
            if (c!=i) {
                sx = x - DirX[d]*3;
                sy = y - DirY[d]*3;
                dx[dc] = sx;
                dy[dc] = sy;
                dc++;
                WriteAt(cPanel,sx,sy,TREG(regID)->Floor,regID,25,true);
                MakeDoor((uint8)sx,(uint8)sy,TREG(regID)->Door); 
            }
        } while (c != i);

        WriteWalls(cPanel,regID);
        /* Now, make the inner walls immutable. */
        for (i=0;i!=dc;i++)
            for (sx = dx[i] - 2; sx <= dx[i] + 2; sx++)
                for (sy = dy[i] - 2; sy <= dy[i] + 2; sy++)
                    if (At(sx,sy).Solid)
                        At(sx,sy).Memory = 60;
        xe.cRoom = cPanel;
        break; 
    case RM_BUILDING:
    case RM_CASTLE:
        IndividualRooms = random(2) ? true : false;
        /* Later, Castles will have corridors *and* rooms. */
        sx = (int16)max(Con[PANEL_SIZEX]/2,Con[PANEL_SIZEX] - (random(6)+2));
        sy = (int16)max(Con[PANEL_SIZEY]/2,Con[PANEL_SIZEY] - (random(6)+2));
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        WriteRoom(r,regID);
        cRectPop = 0;
        WriteCastle(r,regID);
        FindOpenAreas(r,0,FOA_ALLOW_WATER|FOA_ALLOW_WARN|FOA_ALLOW_FALL|FOA_ALLOW_SPEC);
        xe.cRoom = r;
        break;
    case RM_CHECKER:
        sx = (int16)(Con[ROOM_MINX] + random((int16)(Con[ROOM_MAXX] - Con[ROOM_MINX])) + 2);
        sy = (int16)(Con[ROOM_MINY] + random((int16)(Con[ROOM_MAXY] - Con[ROOM_MINY])) + 2);
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        WriteRoom(r,regID);
        WallID = TREG(regID)->Walls;
        if (WallID)
            for (x=r.x1+1;x<r.x2;x++)
                for (y=r.y1+1;y<r.y2;y++)
                    if ((x+y)%2)
                        WriteAt(r,x,y,WallID,regID,PRIO_PILLARS);
        xe.cRoom = r;
        break; 
    case RM_PILLARS: case RM_GRID:
        sx = (int16)(Con[ROOM_MINX] + random((int16)(Con[ROOM_MAXX] - Con[ROOM_MINX])) + 3);
        sy = (int16)(Con[ROOM_MINY] + random((int16)(Con[ROOM_MAXY] - Con[ROOM_MINY])) + 3);
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
        { if (sx == Con[ROOM_MAXX]) sx--; else sx++; }
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
        { if (sx == Con[ROOM_MAXY]) sy--; else sy++; }
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        if (((r.x2-r.x1) % 2))
            r.x2--;
        if (((r.y2-r.y1) % 2))
            r.y2--;

        WriteRoom(r,regID);
        WallID = RType == RM_PILLARS ? Con[TERRAIN_PILLAR] : 
            TREG(regID)->Furnishings[0];
        if (WallID)
            if (RES(WallID)->Type == T_TTERRAIN)
                for (x=r.x1+1;x<r.x2;x++)
                    for (y=r.y1+1;y<r.y2;y++)
                        // ww: a few missing pillars will add variety
                        if (!((x-r.x1)%2) && !((y-r.y1)%2) && random(100) < 95)
                            if (!SolidAt(x,y))
                                WriteAt(r,x,y,WallID,regID,PRIO_PILLARS);
        xe.cRoom = r;
        break;
    case RM_CROSS:
        sx = (int16)max(Con[PANEL_SIZEX]/2,Con[PANEL_SIZEX] - (random(8)+5));
        sy = (int16)max(Con[PANEL_SIZEY]/2,Con[PANEL_SIZEY] - (random(8)+5));
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
        { if (sx == Con[ROOM_MAXX]) sx--; else sx++; }
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
        { if (sx == Con[ROOM_MAXY]) sy--; else sy++; }
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        WriteCross(r,regID);
        WriteWalls(r,regID);
        xe.cRoom = r;
        break;
    case RM_RCAVERN:
        WriteRCavern(cPanel,regID);
        //WriteWalls(cPanel,regID);
        break;
    case RM_LIFECAVE:
        WriteLifeCave(cPanel,regID);
        WriteWalls(cPanel,regID);
        break;
    case RM_DOUBLE:
        //sx = max(Con[PANEL_SIZEX]/2,Con[PANEL_SIZEX] - (random(8)+5));
        //sy = max(Con[PANEL_SIZEY]/2,Con[PANEL_SIZEY] - (random(8)+5));
        sx = (int16)(Con[ROOM_MINX] + random((int16)(Con[ROOM_MAXX] - Con[ROOM_MINX])) + 3);
        sy = (int16)(Con[ROOM_MINY] + random((int16)(Con[ROOM_MAXY] - Con[ROOM_MINY])) + 3);
        if (TREG(regID)->HasFlag(RF_ODD_WIDTH) && !(sx % 2))
        { if (sx == Con[ROOM_MAXX]) sx--; else sx++; }
        if (TREG(regID)->HasFlag(RF_ODD_HEIGHT) && !(sy % 2))
        { if (sx == Con[ROOM_MAXY]) sy--; else sy++; }
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        WriteRoom(r,regID);
        xe.cRoom = r;
        if (random(2)) {
            r.x1 += 2; r.y1 += 2;
            r.x2 -= 2; r.y2 -= 2;
        } else {
            r.x1 += 2; r.x2 -= 2;
            r.y1 += 2; r.y2 -= 2;
            sx -= 4 + random(5); 
            sy -= 4 + random(5); 
            r = r.PlaceWithin((uint8)sx,(uint8)sy);
        }
        if (r.x1 + 1 == r.x2) {
            if (random(2)) r.x1 --; else r.x2++;
        } 
        if (r.y1 + 1 == r.y2) {
            if (random(2)) r.y1 --; else r.y2++;
        }

        WallID = TREG(regID)->Walls;
        for(x=r.x1;x<=r.x2;x++) {
            WriteAt(r,x,r.y1,WallID,regID,PRIO_PILLARS);
            WriteAt(r,x,r.y2,WallID,regID,PRIO_PILLARS);
        }
        for(y=r.y1;y<=r.y2;y++) {
            WriteAt(r,r.x1,y,WallID,regID,PRIO_PILLARS);
            WriteAt(r,r.x2,y,WallID,regID,PRIO_PILLARS);
        }

        do {
            if (random(2))
            { x = random(2) ? r.x1 : r.x2;
            y = r.y1 + random(r.y2 - r.y1); }
            else
            { x = r.x1 + random(r.x2 - r.x1);
            y = random(2) ? r.y1 : r.y2; }
            MakeDoor((uint8)x,(uint8)y,TREG(regID)->Door);
            if (FDoorAt(x,y))
                WriteAt(r,x,y,TREG(regID)->Floor,regID,PRIO_FEATURE_FLOOR);
        } while(random(3));
        break;
    case RM_SHAPED:
        sx = TREG(regID)->sx;
        sy = TREG(regID)->sy;
        r = cPanel.PlaceWithinSafely((uint8)sx,(uint8)sy);
        WriteMap(r,regID);
        xe.cRoom = r;
        break;
    case RM_RANDTOWN:
    case RM_DESTROYED:
        goto DoBasicRoom;
    default:
        Error("Unimplemented room type in DrawPanel!");
        goto DoBasicRoom;
    }

    /* Now, check to be sure that we haven't buried the stairs. If we have,
    1 in 3 chance that we let it stand and corridor away from them later,
    otherwise we redo the room, filling the panel with rock and making it
    a simple room guaranteed to contain the stairs. */


    /* Rooms with coloured/changed walls or floors */

    uint32 ColorList[16];
    if (TREG(regID)->GetList(FLOOR_COLOURS,ColorList,16)) {
        for(c=0;ColorList[c];c++)
            ;
        FloorID = TREG(regID)->Floor;
        for(x=cPanel.x1;x<=cPanel.x2;x++)
            for(y=cPanel.y1;y<=cPanel.y2;y++)
                if (TerrainAt(x,y) == FloorID) {
                    At(x,y).Glyph = (At(x,y).Glyph & 0x00FF) |
                        (ColorList[random(c)] << 8);
                    At(x,y).Shade = true;
                }
    }
    if (TREG(regID)->GetList(WALL_COLOURS,ColorList,16)) {
        for(c=0;ColorList[c];c++)
            ;
        WallID = TREG(regID)->Walls;
        for(x=cPanel.x1;x<=cPanel.x2;x++)
            for(y=cPanel.y1;y<=cPanel.y2;y++)
                if (TerrainAt(x,y) == WallID) {
                    At(x,y).Glyph = (At(x,y).Glyph & 0x00FF) |
                        (ColorList[random(c)] << 8);
                    At(x,y).Shade = false;
                }
    }

    xe.EXVal = (cPanel.x2 + cPanel.x1) / 2; 
    xe.EYVal = (cPanel.y2 + cPanel.y1) / 2; 
    xe.Event = PRE(EV_BIRTH);
    switch (TREG(regID)->Event(xe,regID)) {
    case ABORT:
        /* Abort Creation of the room and retry. */
        for(x=cPanel.x1;x>=cPanel.x2;x++)
            for(y=cPanel.y1;y>=cPanel.y2;y++)
                WriteAt(r,x,y,Con[TERRAIN_ROCK],Con[BASE_REGION],PRIO_EMPTY,true);
        goto Reselect;
    case NOTHING:
        /* Populate the Panel */
        if (xID = TREG(regID)->GetConst(BLOB_WITH)) 
            WriteBlobs(cPanel,regID,xID);

        if (RType == RM_SHAPED)
            break;

        LightPanel(cPanel,regID);

        if ((RType == RM_CASTLE || RType == RM_BUILDING) /*&& IndividualRooms*/) {
            for (i=0;i!=cRectPop;i++) {
                if (TREG(regID)->HasList(ENCOUNTER_LIST))
                    if (random(3))
                        continue;
                FindOpenAreas(PopulateQueue[i],regID,
                    FOA_ALLOW_WATER|FOA_ALLOW_WARN|FOA_ALLOW_FALL|FOA_ALLOW_SPEC);
                /* HACKFIX */
                PopulatePanel(PopulateQueue[i],EN_SINGLE);
            }
            break; /* Encounters set up in WriteCastle. */
        }
        {
            bool anything = false; 
            FindOpenAreas(cPanel,regID,0);
            if (r.x1 < r.x2) { anything = true; FurnishArea(r); }
            if (r2.x1 < r2.x2) { anything = true; FurnishArea(r2);}
            if (r3.x1 < r3.x2) { anything = true; FurnishArea(r3); }
            if (r4.x1 < r4.x2) { anything = true; FurnishArea(r4); }


            if (!anything) 
                FurnishArea(NULL_RECT);
            FindOpenAreas(cPanel,regID,
                FOA_ALLOW_WATER|FOA_ALLOW_WARN|FOA_ALLOW_FALL|FOA_ALLOW_SPEC);
            PopulatePanel(cPanel);

        }
        break;
    case DONE:
        /* Script Populated it for us. */
        break;
    }

    xe.Event = EV_BIRTH;
    TREG(regID)->Event(xe,regID);

    { // ww: if there are two rooms in the castle, they should not be 
        // at war with each other! 
        int16 PartyID = MAX_PLAYERS + 10 + random(200);
        for(x=cPanel.x1;x<=cPanel.x2;x++)
            for(y=cPanel.y1;y<=cPanel.y2;y++)
                if (InBounds(x,y))
                    for(Creature *cr=FCreatureAt(x,y);cr;cr=NCreatureAt(x,y))
                        cr->PartyID = PartyID;
    }
}

void Map::PopulateChest(Container *ch) {
  int16 DepthCR = (int16)(Con[INITIAL_CR] + (Depth*Con[DUN_SPEED])/100 - 1); 
  int i, j = Con[CHEST_MIN_ITEMS];
  j += random((int16)((Con[CHEST_MAX_ITEMS] - j) + (mLuck-10)/2))+1;
  Item *it; 
  for (i=0;i!=j;i++)
    {
    if (random(100)+1 <= (int16)Con[TREASURE_CHANCE])
      it = Item::GenItem(IG_CHEST|IG_GOOD,dID,DepthCR+3,mLuck,ChestItems);
    else
      it = Item::GenItem(IG_CHEST,dID,DepthCR,mLuck,ChestItems);

    if (!it)
      continue;
    if (!((Container*)ch)->XInsert(it))
      delete it;
    }
  if (random(Depth) && random(Depth))
    ch->GainPermStati(LOCKED,ch,SS_MISC); 
  /* Put 1d2-1 staple items in every chest */
  j = random(2);
  for(i=0;i!=j;i++)
  {
    it = Item::GenItem(IG_STAPLE,dID,DepthCR,mLuck,StapleItems);
    if (!it)
      continue;
    if (!((Container*)ch)->XInsert(it))
      delete it;
  }
  
  /* KLUDGE: 1 in 3 chests have 1d6 scrolls of identify */
  if (!random(2))
    {
      it = Item::Create(FIND("scroll"));
      it->MakeMagical(FIND("identify;scroll"));
      if (it) {
        it->Quantity = Dice::Roll(1,5);
        if (!((Container*)ch)->XInsert(it))
          delete it;
        }
    }
  
} 

void Map::FurnishArea(Rect &r)
	{
    Item *it, *ch; Feature *ft;
	int16 x,y,c,i,j,k,maxlev, sz, tx, ty; 
    rID regID, iID; 
    int16 DepthCR = (int16)(Con[INITIAL_CR] + (Depth*Con[DUN_SPEED])/100 - 1); 
    static rID furn[32]; int32 roCount;

    /* Macro to _quickly_ give us a random open point
       in the room or subroom we're supposed to fill. */
    #define RANDOM_OPEN                          \
      roCount = 0;                               \
      do {                                       \
        if (r.x2) {                              \
          x = r.x1 + random(r.x2-r.x1);          \
          y = r.y1 + random(r.y2-r.y1);          \
          }                                      \
        else {                                   \
          c = random(OpenC);                     \
          x = OpenX[c];                          \
          y = OpenY[c];                          \
          }                                      \
        roCount++;                               \
        if (roCount == 100)                      \
          { x = -1; break; }                     \
        }                                        \
      while(SolidAt(x,y) || TTER(TerrainAt(x,y))->HasFlag(TF_FALL) || \
                            TTER(TerrainAt(x,y))->HasFlag(TF_DEEP_LIQ));   

    #define RANDOM_OPEN_NEAR_SOLID                          \
      roCount = 0;                               \
      do {                                       \
        if (r.x2) {                              \
          x = r.x1 + random(r.x2-r.x1);          \
          y = r.y1 + random(r.y2-r.y1);          \
          }                                      \
        else {                                   \
          c = random(OpenC);                     \
          x = OpenX[c];                          \
          y = OpenY[c];                          \
          }                                      \
        bool nearSolid = false;\
        for (c=0;c<9;c++) \
          if (!InBounds(x+DirX[c],y+DirY[c]) || \
               SolidAt(x+DirX[c],y+DirY[c])) nearSolid = true; \
        roCount++;                               \
        if (roCount == 400)                      \
          { x = -1; break; }                     \
        if (!nearSolid) continue; \
        }                                        \
      while(SolidAt(x,y) || TTER(TerrainAt(x,y))->HasFlag(TF_FALL) || \
                            TTER(TerrainAt(x,y))->HasFlag(TF_DEEP_LIQ));   

    #define DRAW_FURNISHING                      \
      switch (RES(furn[i+1])->Type) {            \
        case T_TTERRAIN:                         \
          if (TREG(regID)->HasFlag(RF_CHASM)) break; \
          if (SolidAt(x,y) && !TTER(furn[i+1])->HasFlag(TF_SOLID) && \
              furn[i] != FU_CENTER_CIRCLE_MEDIUM && \
              furn[i] != FU_CENTER_CIRCLE_SMALL) break;                \
          WriteAt(r,x,y,furn[i+1],RegionAt(x,y),PRIO_ROOM_FURNITURE);\
         break;                                  \
        case T_TITEM:                            \
          if (FItemAt(x,y) || SolidAt(x,y)) break;  \
          it = Item::Create(furn[i+1]);          \
          if (it->isType(T_CORPSE) ||            \
              it->isType(T_STATUE)) {            \
            rID cID;                             \
            do {                                 \
              cID = theGame->GetMonID(PUR_DUNGEON,0,(int8)DepthCR,(int8)DepthCR,0); \
            } while (TMON(cID)->HasFlag(M_NO_CORPSE) ||  \
                     TMON(cID)->isMType(cID,MA_DRAGON)    ); \
            ((Corpse*)it)->SetCorpseType(cID);   \
          } else if (it && it->isType(T_CHEST)) { \
            PopulateChest((Container *)it); \
          }  \
          if (it)                                \
            it->PlaceAt(this,x,y);               \
         break;                                  \
        case T_TFEATURE:                         \
          if (TREG(regID)->HasFlag(RF_CHASM)) break; \
          if (SolidAt(x,y)) break; \
          ASSERT(TFEAT(furn[i+1])->Type != T_DOOR);   \
          ASSERT(TFEAT(furn[i+1])->Type != T_PORTAL); \
          ft = new Feature(TFEAT(furn[i+1])->Image,   \
                      furn[i+1],T_FEATURE);      \
          if (ft)                                \
            ft->PlaceAt(this,x,y);               \
         break;                                  \
        default:                                 \
          Error(Format("Strange furnishing resource (%d)!", \
                    RES(furn[i+1])->Type)); \
        }

    if (r.x2)
      sz = (r.x2 - r.x1) * (r.y2 - r.y1);
    else
      sz = OpenC;

    if (!sz)        
      return;

    regID = RegionAt(OpenX[0],OpenY[0]);
    TREG(regID)->GetList(FURNISHINGS,furn,32);
    for(i=0;furn[i];i+=2) {
      switch(furn[i]) {
        case FU_SPLATTER:
          SplatterByDefault:
          if (r.x2)
            j = random(r.Volume())/5;
          else
            j = random(OpenC) / 8;
          for(k=0;k!=j;k++)
            {
              RANDOM_OPEN;
              if (x != -1)
                { DRAW_FURNISHING; }
            }
         break;

        case FU_CENTER_CIRCLE_LARGE: 
        case FU_CENTER_CIRCLE_MEDIUM: 
        case FU_CENTER_CIRCLE_SMALL: 
        {
          int xd = r.x2 - r.x1; 
          int yd = r.y2 - r.y1; 
          int xr = (xd / 2) ;
          int yr = (yd / 2) ;
          int cx = (r.x1 + r.x2) / 2;
          int cy = (r.y1 + r.y2) / 2;
          switch (furn[i]) {
            case FU_CENTER_CIRCLE_LARGE: break;
            case FU_CENTER_CIRCLE_MEDIUM: xr /= 2; yr /= 2; break;
            case FU_CENTER_CIRCLE_SMALL: xr /= 4; yr /= 4; break;
          } 
          if (xr < 1) xr = 1;
          if (yr < 1) yr = 1;
          /*
           theGame->GetPlayer(0)->IPrint(
               Format("[xr = %d yr = %d -> %s]",
                 xr,yr, (const char*)NAME(furn[i+1])));
                 */
          for(x = r.x1;x<=r.x2;x++)
            for(y = r.y1;y<=r.y2;y++) {
              int xx = abs(x - cx);
              int yy = abs(y - cy);
              /* ellipse: (x^2 / a^2) + (y^2 / b^2) <= 1 */
              if ((xx * xx * yr * yr) + (yy * yy * xr * xr) <
                  (xr * xr * yr * yr)) {
                DRAW_FURNISHING;
              }
            } 
          break; 
        } 

        case FU_LIFEBLOB:
          WriteBlobs(cPanel,regID,furn[i+1]);
         break;
        case FU_GRID:
          if (!r.x2)
            goto SplatterByDefault;
          for(x = r.x1+1;x<=r.x2-1;x+=2)
            for(y = r.y1+1;y<=r.y2-1;y+=2)
              DRAW_FURNISHING;
         break;
        case FU_SPACED_COLUMNS:
          if (!r.x2) {
            ASSERT(0); 
            break;
          }
          if (random(2)) {                      
            for(x = r.x1+1;x<=r.x2-1;x+=4)
              for(y = r.y1+1;y<=r.y2-1;y+=2)
                DRAW_FURNISHING;
            }
          else {
            for(x = r.x1+1;x<=r.x2-1;x+=2)
              for(y = r.y1+1;y<=r.y2-1;y+=4)
                DRAW_FURNISHING;
            }
         break;
        case FU_COLUMNS:
          if (!r.x2) {
            ASSERT(0); 
            break;
          }
          if (random(2)) {                      
            for(x = r.x1+1;x<=r.x2-1;x+=2)
              for(y = r.y1+1;y<=r.y2-1;y++)
                DRAW_FURNISHING;
            }
          else {
            for(x = r.x1+1;x<=r.x2-1;x++)
              for(y = r.y1+1;y<=r.y2-1;y+=2)
                DRAW_FURNISHING;
            }
         break;
        case FU_INNER_COLUMNS:
          if (!r.x2) {
            ASSERT(0); 
            break;
          }
          if (random(2)) {                      
            for(x = r.x1+2;x<=r.x2-2;x+=2)
              for(y = r.y1+2;y<=r.y2-2;y++)
                DRAW_FURNISHING;
            }
          else {
            for(x = r.x1+2;x<=r.x2-2;x++)
              for(y = r.y1+2;y<=r.y2-2;y+=2)
                DRAW_FURNISHING;
            }
         break;
        case FU_CENTER:
          for (j=0;j!=nCenters;j++)
            {
              x = Centers[j] % 256;
              y = Centers[j] / 256;
              DRAW_FURNISHING;
            }
         break;
        case FU_CORNERS:
          for (j=0;j!=nCorners;j++)
            {
              x = Corners[j] % 256;
              y = Corners[j] / 256;
              DRAW_FURNISHING;
            }
         break;
        case FU_XCORNERS:
          for (j=0;j!=nCorners;j++)
            {
              tx = Corners[j] % 256;
              ty = Corners[j] / 256;

              if (SolidAt(tx,ty-1))
                if (OpenAt(tx+1,ty+1) && OpenAt(tx-1,ty+1))
                  { x = tx; y = ty + 1; DRAW_FURNISHING; }
              if (SolidAt(tx,ty+1))
                if (OpenAt(tx+1,ty-1) && OpenAt(tx-1,ty-1))
                  { x = tx; y = ty - 1; DRAW_FURNISHING; }
              if (SolidAt(tx-1,ty))
                if (OpenAt(tx+1,ty+1) && OpenAt(tx+1,ty-1))
                  { x = tx + 1; y = ty; DRAW_FURNISHING; }
              if (SolidAt(tx+1,ty))
                if (OpenAt(tx-1,ty+1) && OpenAt(tx+1,ty+1))
                  { x = tx - 1; y = ty; DRAW_FURNISHING; }

              for (k = 4; k != 8; k++)
                {
                  if ( SolidAt(tx - DirX[k],ty - DirY[k]) &&
                         SolidAt(tx,ty - DirY[k]) &&
                         SolidAt(tx - DirX[k],ty) )
                    if (!SolidAt(tx + DirX[k], ty + DirY[k]))
                      {
                        x = tx + DirX[k];
                        y = ty + DirY[k];
                        DRAW_FURNISHING;
                      }
                }
            }
         break;
        case FU_RANDOM:
//RandomByDefault:
          RANDOM_OPEN;
          if (x != -1)
            { DRAW_FURNISHING; }
         break;
        case FU_TABLE:
        case FU_BED:
          break;
        }
      }

 
    if (random(100)+1 < (int16)Con[CHEST_CHANCE] + (1 * (mLuck-10)))
       {         
         iID = theGame->GetItemID(PUR_DUNGEON,0,(int8)DepthCR,T_CHEST);
         ch = Item::Create(iID);
         ASSERT(ch);
         PopulateChest((Container *)ch);
         RANDOM_OPEN_NEAR_SOLID;
         if (x != -1)
           ch->PlaceAt(this,x,y); 
         else
           ch->Remove(true);   
       }
    else if (random(100)+1 <= (int16)Con[TREASURE_CHANCE]) 
      {
        if (random(100)+1 <= (int16)Con[CURSED_CHANCE])
          it = Item::GenItem(IG_CURSED,dID,DepthCR,mLuck,DungeonItems);
        else
          it = Item::GenItem(IG_GOOD,dID,DepthCR,mLuck,DungeonItems);
        RANDOM_OPEN_NEAR_SOLID;
        if (x != -1)
          it->PlaceAt(this,x,y);
        else
          it->Remove(true);
      } else if (random(100)+1 < 50) {
        /* Generate a poor item */
        it = Item::GenItem(0,dID,max(0,DepthCR/2-4),10,DungeonItems);
        if (it) {
          RANDOM_OPEN_NEAR_SOLID;
          if (x != -1)
            it->PlaceAt(this,x,y);
          else
            it->Remove(true);
          }
      }

    if (random(100)+1 <= (int16)Con[STAPLE_CHANCE]) {
        it = Item::GenItem(IG_STAPLE,dID,DepthCR,10,DungeonItems);
        RANDOM_OPEN_NEAR_SOLID;
        if (x != -1)
          it->PlaceAt(this,x,y);
        else
          it->Remove(true);
      }

    DepthCR = maxlev = AdjustCR((int16)(Con[INITIAL_CR] + (Depth*Con[DUN_SPEED])/100 - 1)); 

    if (r.x2) {
      OpenC = 0;
      for(int16 ix=r.x1;ix<=r.x2;ix++)
        for(int16 iy=r.y1;iy<=r.y2;iy++)
          {
            OpenX[OpenC] = (uint8)ix;
            OpenY[OpenC] = (uint8)iy;
            OpenC++;
          }
      }
    
    regID = RegionAt(OpenX[0],OpenY[0]);
    ASSERT(RES(regID)->Type == T_TREGION);
    
    
  }
  
/* HACKFIX */
void Map::PopulatePanel(Rect &r, uint16 extraFlags) { 
    int16 DepthCR = (int16)(Con[INITIAL_CR] + (Depth*Con[DUN_SPEED])/100 - 1); 
    int16 realCR;
    rID regID = RegionAt(OpenX[0],OpenY[0]);
    if ((random(100) < 22 - mLuck) && DepthCR > 1 && theGame->Opt(OPT_OOD_MONSTERS))
      /* Generate Out-of-Depth Encounter */
      realCR = DepthCR + random(4) + 1;
    else
      realCR = DepthCR;
      
    THROW(EV_ENGEN,
      xe.enFlags = EN_DUNGEON|EN_ANYOPEN|((realCR==DepthCR)?0:(EN_OODMON|EN_SINGLE));
      xe.enFlags |= extraFlags;
      xe.enRegID = regID;
      xe.EMap = this;
      xe.enCR = realCR;
      xe.enDepth = DepthCR;
      if (TREG(regID)->HasFlag(RF_CENTER_ENC) && nCenters)
        {
          xe.enFlags &= ~EN_ANYOPEN;
          xe.isLoc = true;
          int16 i = random(nCenters);
          xe.EXVal = Centers[i] % 256;
          xe.EYVal = Centers[i] / 256;
        }
      );
    Creature *c;
    ASSERT(c=GetEncounterCreature(0));
    /*ASSERT(c->x >= r.x1 &&
           c->y >= r.y1 &&
           c->x <= r.x2 &&
           c->y <= r.y2);*/
 }

bool Map::FindOpenAreas(Rect r, rID regID, int16 Flags)
  {
    int16 x, y; int16 ter, reg; TTerrain *tt;
    
    OpenC = 0; ter = -1; reg = -1;
    
    bool isReg;
    
    isReg = false;
    tt = NULL;
    for (x=r.x1;x<=r.x2;x++)
      for (y=r.y1;y<=r.y2;y++)
        {
          /*if (tt && tt->HasFlag(TF_WATER) &&
                (Flags & FOA_ALLOW_WATER))
            __asm int 3;*/

          if (At(x,y).Solid != (Flags & FOA_SOLID_ONLY))
            continue;
          if (FFeatureAt(x,y))
            continue;
          if (FCreatureAt(x,y))
            continue;

          if (At(x,y).Terrain != ter)
            { ter = At(x,y).Terrain;
              tt = TTER(TerrainAt(x,y)); }
          if (At(x,y).Region != reg)
            {
              reg = At(x,y).Region;
              if (!regID)
                isReg = true;
              else if (RegionAt(x,y) == regID)
                isReg = true;
              else
                isReg = false;
            }
            
          if (!isReg)
            continue;

          if (tt->HasFlag(TF_SPECIAL) &&
               !tt->HasFlag(TF_WATER)) {
            if (!(Flags & FOA_ALLOW_SPEC))
              continue;
            }
          else if (Flags & FOA_SPEC_ONLY)
            continue;

          if (tt->HasFlag(TF_WARN) &&
               !tt->HasFlag(TF_WATER)) {
            if (!(Flags & FOA_ALLOW_WARN))
              continue;
            }
          else if (Flags & FOA_WARN_ONLY)
            continue;

          if (tt->HasFlag(TF_WATER)) {
            if (!(Flags & FOA_ALLOW_WATER))
              continue;
            }
          else if (Flags & FOA_WATER_ONLY)
            continue;

          /*if (Flags & FOA_DEEP_ONLY)
            if (!tt->HasFlag(TF_DEEP))
              continue;*/

          if (TreeAt(x,y)) {
            if (Flags & FOA_NO_TREES)
              continue;
          } else if (Flags & FOA_TREES_ONLY)
            continue;

          if (tt->HasFlag(TF_FALL)) {
            if (!(Flags & FOA_ALLOW_FALL))
              continue;
            }
          else if (Flags & FOA_FALL_ONLY)
            continue;        

          if (Flags & FOA_FLOOR_ONLY)
            if (TerrainAt(x,y) != TREG(RegionAt(x,y))->Floor)
              continue;
        
          OpenX[OpenC] = (uint8)x;
          OpenY[OpenC] = (uint8)y;
          OpenC++;
          if (OpenC == 2048)
            return true;
        } 
    return OpenC != 0;
  }

uint16 Map::GetOpenXY()
  {
    int16 n;
    if (!OpenC) {
      ASSERT(OpenC); 
      return 0;
    } 
    n = random(OpenC);
    return OpenX[n] + OpenY[n]*256;
  }

uint16 Map::Tunnel(uint8 sx, uint8 sy, uint8 dx, uint8 dy, uint8 TFlags, Dir StartDir, int8 TType) {
  int16 i,x,y,ox,oy;
  int16 ThisCorridorLength = 0, segLength = 0;
  int32 n;
  int16 regionConsts[] = {
    TURN_CHANCE, STUBBORN_CORRIDOR, 
    SEGMENT_MINLEN, SEGMENT_MAXLEN, 0 
  };

  rID regID = Con[CORRIDOR_REGION];  

  /* Choose Corridor Region Type */
  if (dist(sx,sy,dx,dy) > 5) { 
    int c=0;
    for (i=0; CorridorWeights[i*2]; i++)
      for (x=0; x < (int16)CorridorWeights[i*2+1]; x++)
        Candidates[c++] = CorridorWeights[i*2];
    if (c)  {
      int trials = 0;
reselect: 
      regID = Candidates[random(c)]; 
      for (i=0;trials < 100 && usedInThisLevel[i];i++)
        if (usedInThisLevel[i] == regID) {
          trials++; goto reselect;
        }
      if (! (TREG(regID)->HasFlag(RF_STAPLE)))
        usedInThisLevel[i] = regID;
    }
  }

  for (i=0;regionConsts[i];i++)
    if (n = TREG(regID)->GetConst(regionConsts[i]))
      Con[regionConsts[i]] = n;


  Dir CurrDir,OldDir; int16 RoomsFound = 0; uint16 *Corr; int16 cc;
  bool stop_flag=0,force_turn=0, startConnected;
  int16 DepthCR = (int16)(Con[INITIAL_CR] + (Depth*Con[DUN_SPEED])/100 - 1); 
  ASSERT(regID);
#define NEXTX (x + DirX[CurrDir])
#define NEXTY (y + DirY[CurrDir])
#define NEXTX2 (x + DirX[CurrDir]*2)
#define NEXTY2 (y + DirY[CurrDir]*2)

  ASSERT(InBounds(sx,sy));
  ASSERT(InBounds(dx,dy));

  if (StartDir == -1)
  {
    if (MYABS(sx-dx) > MYABS(sy-dy)) {
      if (sx > dx)
        StartDir = WEST;
      else
        StartDir = EAST;
    }
    else if (sy > dy)
      StartDir = NORTH;
    else
      StartDir = SOUTH;
  }
  Corr = new uint16[MAX_CORRIDOR_LENGTH*sizeof(uint16)];
RestartTunnel:
  startConnected = At(sx,sy).Connected != 0;
  CurrDir = StartDir;
  x = sx; y = sy;

  cc = 0;

  while(!stop_flag)
  {
    if (!(TFlags & TT_NOTOUCH))
      if (At(x,y).Priority == PRIO_ROOM_FLOOR)
        if (!(RoomsTouched[y / Con[PANEL_SIZEY]] & (1 << (x / Con[PANEL_SIZEX]))))
        {
          RoomsFound++;
          RoomsTouched[y / Con[PANEL_SIZEY]] |= 1 << (x / Con[PANEL_SIZEX]);
          if (TFlags & TT_WANDER)
            if (random(RoomsFound) && random(RoomsFound))
              goto WriteCorridor;
        }

    if (At(NEXTX,NEXTY).Solid)
      if (At(NEXTX2,NEXTY2).Priority == PRIO_CORRIDOR_WALL)
        MakeDoor(NEXTX,NEXTY,TREG(RegionAt(NEXTX,NEXTY))->Door);

    Corr[cc++] = x+y*256;
    segLength++;

    if (TFlags & TT_CONNECT)
      if ((At(x,y).Connected != 0) != startConnected)
        if (!At(x,y).Solid)
          stop_flag = true;

    ox = x; oy = y;
    x += DirX[CurrDir];
    y += DirY[CurrDir];

    force_turn = false;

    if ((x + DirX[CurrDir]*4) > (sizeX-1) || (x + DirX[CurrDir]*4) < 0)
      force_turn = true;
    if ((y + DirY[CurrDir]*4) > (sizeY-1) || (y + DirY[CurrDir]*4) < 0)
      force_turn = true;

    if (At(x,y).Priority >= PRIO_VAULT)
      /* Map or vault edges */
      force_turn = true;

    if (At(NEXTX,NEXTY).Priority == PRIO_CORRIDOR_WALL && 
        At(NEXTX2,NEXTY2).Priority == PRIO_CORRIDOR_WALL)
      /* Writing over another corridor wall */
      force_turn = true;


    if (At(x,y).Priority == PRIO_ROOM_WALL && 
        At(NEXTX,NEXTY).Priority == PRIO_ROOM_WALL)
      /* Writing over a room wall */
      force_turn = true;

    else if (At(NEXTX,NEXTY).Priority == PRIO_ROOM_WALL) 
      /* Intersecting Room Wall */
    {
      if (!random(2))
        MakeDoor((uint8)x,(uint8)y,TREG(RegionAt(NEXTX,NEXTY))->Door);
      else 
        MakeDoor(NEXTX,NEXTY,TREG(RegionAt(NEXTX,NEXTY))->Door);


    }

    if (x == dx && DirX[CurrDir])
      force_turn = true;
    if (y == dy && DirY[CurrDir])
      force_turn = true;

    if (segLength > (int16)Con[SEGMENT_MAXLEN])
      force_turn = true;

    if (force_turn || (segLength > (int16)Con[SEGMENT_MINLEN] && (random(100) < (int16)Con[TURN_CHANCE]))) {
      segLength = 0;
      if(TFlags & TT_DIRECT || random(100) > (int16)Con[STUBBORN_CORRIDOR])
        CurrDir = CorrectDir(x,y,dx,dy,CurrDir);
      else {
        OldDir=CurrDir;
        while(CurrDir==OldDir)
          CurrDir = !random(4) ? EAST :
            !random(3) ? WEST :
            !random(2) ? NORTH : SOUTH;
      }
    }
    if (x <= 2)
      CurrDir = EAST;
    else if (y <= 2)
      CurrDir = SOUTH;
    else if (x >= sizeX-3)
      CurrDir = WEST;
    else if (y >= sizeY-3)
      CurrDir = NORTH;

    /* Tunneling is done when:
     * - We're within 9 squares of destination
     * - We're on the same panel as the destination
     * - We're in the same region as the destination
     * - We're not somewhere in solid rock
     * Some of these are unnecessary, but paranoia is good...
     */
    if (!(TFlags & TT_CONNECT)) 
      if (dist(x,y,dx,dy) < 10)
        if (!At(x,y).Solid)
          if (RegionAt(x,y) == RegionAt(dx,dy))
            if (x / Con[PANEL_SIZEX] == dx / Con[PANEL_SIZEX])
              if (y / Con[PANEL_SIZEY] == dy / Con[PANEL_SIZEY])
                stop_flag = true;

    if(!InBounds(x,y))
      goto RestartTunnel;


    TotalCorridorLength++; ThisCorridorLength++;
    if (cc >= MAX_CORRIDOR_LENGTH)
      break;
  }

WriteCorridor:

  rID last = 0; 

  for(i=0;i!=cc;i++)
  {
    Rect r;
    r.Set(x-1,y-1,x+1,y+1); 
    x = Corr[i] % 256;
    y = Corr[i] / 256;

    rID here = RegionAt(x,y); 
    rID rw = regID; 

    if (At(x,y).Solid) {  
        // (here != last || here == Con[BASE_REGION] || here == regID) 
      last = here; 
      const int f_prio = PRIO_ROOM_FLOOR; 
      const int w_prio = PRIO_CORRIDOR_WALL;
      WriteAt(r,x,y,TREG(regID)->Floor,rw,f_prio);
      for (int w=0;w<8;w++)
        WriteAt(r,x+DirX[w],y+DirY[w],TREG(regID)->Walls,rw,w_prio);
    } 
    if (startConnected)
      At(x,y).Connected = true;
#if 0
    /* Highlight the two types of corridors' paths in different
       colours, for purposes of debugging the dungeon design
       algorithm. */
    if (TFlags & TT_CONNECT) {
      At(x,y).Glyph = GLYPH_FLOOR2 | PINK*256;
      At(x,y).Shade = false;
    }
    else {
      At(x,y).Glyph = GLYPH_FLOOR2 | WHITE*256;
      At(x,y).Shade = false;
    }
#endif
  }
  //delete[] Corr;
  /* Satisfy Purify */
  free (Corr);
  for (i=0;regionConsts[i];i++)
    Con[regionConsts[i]] = 
      TDUN(dID)->GetConst(regionConsts[i]);

  return x + y*256;

}

Dir Map::CorrectDir(int16 x, int16 y, int16 dx,int16 dy,Dir Curr)
	{
		int16 lx,ly;
		Dir Option1, Option2,tmp;
    #define SWAP(a,b) { tmp=a; a=b; b=tmp; }
    #define DIAMETRIC(a,b) ((DirX[a]+DirX[b]==0) && (DirY[a]+DirY[b]==0))
		Option1=CENTER;
		if(x==dx)
			Option1 = y>dy ? NORTH : SOUTH;
		if(y==dy)
			Option1 = x>dx ? WEST  : EAST;
		if (Option1!=CENTER && !DIAMETRIC(Option1,Curr))
			return Option1;

		lx=MYABS(x-dx);
		ly=MYABS(y-dy);   

		Option1=y>dy ? NORTH : SOUTH;
		Option2=x>dx ? WEST  : EAST;

		if(lx>ly)
			SWAP(Option1,Option2)
		if(!random(4))
			SWAP(Option1,Option2)

		if(DIAMETRIC(Option1,Curr))
			return Option2;
		else
			return Option1;
	}

int32 Map::FloodConnectA(int16 x, int16 y,int32 fCount) {
    uint8 *FloodSP = FloodStack;

    FloodSP[0] = (uint8)x;
    FloodSP[1] = (uint8)y;

DoCall:
    x = FloodSP[0];
    y = FloodSP[1];
    ASSERT(FloodSP+6 < FloodStack+(sizeX*sizeY*3)); 
    /* Special Kludge: treat both inner 'building' walls and terrain
    streamers like chasms as open with regard to flood-connection. */
#if 0
    if(!InBounds(x,y) || 
        At(x,y).Solid && (At(x,y).Priority < 60 
        || At(x,y).Priority == 100))
#endif
        // ww: this is intentionally not "SolidAt" for Features
        if(At(x,y).Solid && !FDoorAt(x,y))
            goto DoReturn;
    if (At(x,y).Connected)
        goto DoReturn;

    At(x,y).Connected = true;
    //At(x,y).Glyph = GLYPH_FLOOR2 + PINK*256;
    FloodArray[fCount*2]   = (uint8)x;
    FloodArray[fCount*2+1] = (uint8)y;
    fCount++;

    for(FloodSP[2]=0;FloodSP[2]!=8;FloodSP[2]++) {
        FloodSP += 3;
        FloodSP[0] = DirX[FloodSP[-1]] + x;
        FloodSP[1] = DirY[FloodSP[-1]] + y;
        goto DoCall;

ReturnAddress:
        FloodSP -= 3;
        x = FloodSP[0];
        y = FloodSP[1];
    }

DoReturn:
    if (FloodSP == FloodStack)
        return fCount;

    goto ReturnAddress;
}

int32 Map::FloodConnectB(int16 x, int16 y) {
    Thing *t; int16 i;
    uint8 *FloodSP; int32 fCount = 0;

    FloodArray = new uint8[sizeX * sizeY * 2];
    FloodStack = new uint8[sizeX * sizeY * 3];
    ASSERT(FloodArray && FloodStack);

    FloodSP = FloodStack;

    FloodSP[0] = (uint8)x;
    FloodSP[1] = (uint8)y;

    MapIterate(this,t,i)
        if (t->isType(T_DOOR)) {
            /* Later, percentile chance that each door "holds". */
            if (((Door*)t)->DoorFlags & DF_LOCKED) {
                if (!random(5))
                    continue;
            } else if (((Door*)t)->DoorFlags & DF_STUCK) {
                if (!random(3))
                    continue;
            } else if (!(((Door*)t)->DoorFlags & DF_OPEN)) {
                if (!random(2))
                    continue;
            } else
                continue;
            At(t->x,t->y).Solid = true;
        }

DoCall:
    x = FloodSP[0];
    y = FloodSP[1];

    ASSERT(FloodSP+6 < FloodStack+(sizeX*sizeY*3)); 

    if (!InBounds(x,y))
        goto DoReturn;
    if(At(x,y).Solid || At(x,y).Visibility)
        goto DoReturn;

    At(x,y).Connected = true;
    /*At(x,y).Glyph &= 0x00FF;
    At(x,y).Glyph |= EMERALD*256; 
    At(x,y).Shade = 0;           */


    FloodArray[fCount*2]   = (uint8)x;
    FloodArray[fCount*2+1] = (uint8)y;
    fCount++;

    for(FloodSP[2]=0;FloodSP[2]!=8;FloodSP[2]++) {
        FloodSP += 3;
        FloodSP[0] = DirX[FloodSP[-1]] + x;
        FloodSP[1] = DirY[FloodSP[-1]] + y;
        goto DoCall;

ReturnAddress:
        FloodSP -= 3;
        x = FloodSP[0];
        y = FloodSP[1];
    }

DoReturn:
    if (FloodSP == FloodStack) {
        delete FloodStack;
        delete FloodArray;
        MapIterate(this,t,i)
            if (t->isType(T_DOOR)) {
                /* ASSUMPTION: Doors never exist in solid squares */
                At(t->x,t->y).Solid = false;
            }
        return fCount;
    }

    goto ReturnAddress;
}

bool Map::TreeAt(int16 x, int16 y)
{ 
  if (TTER(TerrainAt(x,y))->HasFlag(TF_TREE))
    return true;
  else {
    Thing *t = oThing(At(x,y).Contents);
    while (t) {
      if (t->isFeature()) {
        if (t->Flags & F_TREE) return true; 
      } 
      t = oThing(t->Next);
    }
  } 
  return false; 
}

bool Map::SolidAt(int16 x, int16 y)
{ 
  if (At(x,y).Solid)
    return true;
  else {
    Thing *t = oThing(At(x,y).Contents);
    while (t) {
      if (t->isFeature()) {
        if (t->Flags & (F_SOLID|F_XSOLID)) return true; 
      } 
      t = oThing(t->Next);
    }
  } 
  return false; 
}

bool Map::OpaqueAt(int16 x, int16 y)
{ 
  if (At(x,y).Opaque || At(x,y).cOpaque)
    return true;
  else {
    Thing *t = oThing(At(x,y).Contents);
    while (t) {
      if (t->isFeature()) {
        if (t->Flags & (F_OPAQUE)) return true; 
      } 
      t = oThing(t->Next);
    }
  } 
  return false; 
}

bool Map::ObscureAt(int16 x, int16 y)
{ 
  if (At(x,y).Obscure || At(x,y).mObscure)
    return true;
  else {
    Thing *t = oThing(At(x,y).Contents);
    while (t) {
      if (t->isFeature()) {
        if (t->Flags & (F_OBSCURE)) return true; 
      } 
      t = oThing(t->Next);
    }
  } 
  return false; 
}

rID Map::StickyAt(int16 x, int16 y)
{ 
  if (TTER(TerrainAt(x,y))->HasFlag(TF_STICKY))
    return TerrainAt(x,y);
  else {
    Thing *t = oThing(At(x,y).Contents);
    while (t) {
      if (t->isFeature()) {
        if (t->Flags & (F_STICKY)) return ((Feature *)t)->fID; 
      } 
      t = oThing(t->Next);
    }
  } 
  return false; 
}








