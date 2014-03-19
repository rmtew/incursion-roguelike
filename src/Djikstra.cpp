/* DJIKSTRA.CPP -- Copyright (c) 1999-2003 Julian Mensch
     An implementation of Djikstra's algorithm used for the
   running code, but available for any other (AI?) purposes
   that it might be needed as well.

     void Map::PQInsert(uint16 Node, int16 Weight)
     int32 Map::PQPeekMin()          
     bool Map::PQPopMin()
     bool Map::ShortestPath(uint8 sx, uint8 sy, uint8 tx, uint8 ty)
     uint16 Map::PathPoint(int16 n)
     bool Map::RunOver(uint8 x, uint8 y)

*/

#include "Incursion.h"


struct PQueue
  {
    int16 Weight;
    NArray<uint16,200,200> Elements;
    PQueue *Next;
    PQueue(uint16 FirstNode, int16 _Weight)
      { Next = NULL;
        Weight = Weight;
        Elements.Set(FirstNode,0);  }
  };

PQueue *PHead;

uint16 ThePath[MAX_PATH_LENGTH];

void Map::PQInsert(uint16 Node, int16 Weight)
  {
    PQueue *pq, *pq2;
    if (PHead == NULL || PHead->Weight > Weight)
      {
        PHead = new PQueue(Node, Weight);
        return;
      }

    for(pq = PHead; pq->Next && pq->Next->Weight < Weight; pq = pq->Next) 
      ;
      
    if (pq->Weight == Weight)
      {
        pq->Elements.Add(Node);
        return;
      }
    pq2 = new PQueue(Node, Weight);
    pq2->Next = pq->Next;
    pq->Next = pq2;

    return;
  }
  
int32 Map::PQPeekMin()          
  {
    if (!PHead) 
      return -1;
    if (PHead->Elements.Total() == 0) 
      return -1;
    return PHead->Elements[0];
  }

bool Map::PQPopMin()
  {
    PQueue *pq;
    if (PQPeekMin() == -1)
      return false;
    
    /* Remove the node from the first stack */
    PHead->Elements.Remove(0);

    /* Remove the stack if it's empty */
    if (PHead->Elements.Total() == 0)
      {
        pq = PHead;
        PHead = PHead->Next;
        delete pq;
      }
    return true;
  }


bool Map::ShortestPath(uint8 sx, uint8 sy, uint8 tx, uint8 ty,
                       Creature *runner, int32 dangerFactor, 
                       uint16 *ThePath)
  {
    int32 xy, i, c; int16 x, y, nx, ny;
    static int16 Dist[256][256];
    static uint16 Parent[256][256];

    if (!ThePath)
      ThePath = ::ThePath;

    ASSERT(InBounds(sx,sy))
    ASSERT(InBounds(tx,ty))
    PHead = NULL;

    bool Incor = 
      runner->HasMFlag(M_INCOR) ||
      runner->HasStati(PHASED);
    bool Meld = 
      runner->HasAbility(CA_EARTHMELD);

    for (x=0;x!=256;x++)
      for(y=0;y!=256;y++)
        {
          Dist[x][y]   = 30000;
          Parent[x][y] = 0;
        }

    Dist[sx][sy] = 0;

    PQInsert(sx+sy*256,0);

    while ((xy = PQPeekMin()) != -1)
      {
        PQPopMin();
        x = xy % 256;
        y = xy / 256;

        for (i=0;i!=8;i++) {
          nx = x + DirX[i];
          ny = y + DirY[i];
          /* KLUDGE: We consider (0,0) to be offmap so that we can use
             node 0 as a special empty/unmarked value. */
          if (nx == 0 && ny == 0)
            continue;
          if (!InBounds(nx,ny))
            continue;
          int baseCost = RunOver(nx,ny,true,runner,dangerFactor,Incor,Meld);
          if (!baseCost)
            continue;
          if (DirX[i] && DirY[i])
            baseCost = (baseCost * 3) / 2;

          if (Dist[x][y] + baseCost < Dist[nx][ny])
            {
              Dist[nx][ny] = min(30000,Dist[x][y] + baseCost);
              Parent[nx][ny] = x+y*256;
              PQInsert(nx+ny*256,Dist[nx][ny]);
            }
          }
      }

    x = tx;
    y = ty;
    c = 0;

    if (Dist[tx][ty] == 30000) {
      #ifdef DEBUG_DJIKSTRA
      for (x = min(0,sx-30);x!=max(127,sx+30);x++)
        for (y = min(0,sx-30);y!=max(127,sy+30);y++)
          if (Dist[x][y] != 30000)
            {
              At(x,y).Glyph =
                GLYPH_FLOOR2 | (EMERALD*256);
              At(x,y).Memory =
                GLYPH_FLOOR2 | (EMERALD*256);
              At(x,y).Shade = false;
            }              
      #endif
      return false;
      }

    do {
      c++;
      xy = Parent[x][y];
      x = xy % 256;
      y = xy / 256;
      }
    while (xy);

    x = tx;
    y = ty;
    i = 0; c--;
    do {
      ThePath[c - i] = x + y*256;
      xy = Parent[x][y];
      x = xy % 256;
      y = xy / 256;
      i++;
      }
    while (xy);

    ThePath[c+1] = 0;

    #ifdef DEBUG_DJIKSTRA
    for (i=0;i==0 || ThePath[i];i++) {
      At(ThePath[i]%256,ThePath[i]/256).Glyph =
        GLYPH_FLOOR2 | (MAGENTA*256);
      At(ThePath[i]%256,ThePath[i]/256).Memory =
        GLYPH_FLOOR2 | (MAGENTA*256);
      At(ThePath[i]%256,ThePath[i]/256).Shade = false;
      }
    #endif

    return true;



  }

uint16 Map::PathPoint(int16 n)
  { return ThePath[n]; }

uint16 Map::RunOver(uint8 x, uint8 y, bool memonly, Creature *c,
                  int32 dangerFactor, bool Incor, bool Meld)
{
  if (!Incor && SolidAt(x,y)) {
    if (Meld) {
      int i = TTER(PTerrainAt(x,y,c))->Material;
      if (i == MAT_GRANITE || i == MAT_MAGMA || i == MAT_QUARTZ ||
          i == MAT_GEMSTONE || i == MAT_MINERAL)
        ; // we can walk here
      else
        return 0; 
    } else return 0;
  } 
  if (memonly && !At(x,y).Memory)
    return 0;
  if (At(x,y).Contents) {
    Creature *ca; Trap *tr; Door *dr;
    for (ca=FCreatureAt(x,y);ca;ca=NCreatureAt(x,y))
      if (ca && c->Percieves(ca) && ca->isHostileTo(c))
        return 0;
    for (tr=FTrapAt(x,y);tr;tr=NTrapAt(x,y)) {
      if (tr->TrapFlags & TS_FOUND)
        if (!(tr->TrapFlags & TS_DISARMED)) {
          if (dangerFactor & DF_IGNORE_TRAPS)
            return (sizeX * 3) + (tr->TrapLevel() * 10);
          else
            return 0;
        } 
    } 
    for (dr=FDoorAt(x,y);dr;dr=NDoorAt(x,y))
      if (!(dr->DoorFlags & DF_OPEN))
        return 0;
  }
  rID stickyID = StickyAt(x,y); 
  if (stickyID && 
      !c->HasStati(STUCK) && 
      !c->HasMFlag(M_AMORPH) && 
      !c->isAerial() && 
      c->onPlane() == PHASE_MATERIAL && 
      c->ResistLevel(AD_STUK) != -1 && 
      !c->HasAbility(CA_WOODLAND_STRIDE)) {
    if (dangerFactor & DF_IGNORE_TERRAIN) 
      return (sizeX * 3);
    else
      return 0; 
  } 

  Feature * f; 
  for (f=FFeatureAt(x,y);f;f=NFeatureAt(x,y)) 
    if (TFEAT(f->fID)->PEvent(EV_MON_CONSIDER,c,f,f->fID) == ABORT) {
      if (dangerFactor & DF_IGNORE_TERRAIN) 
        return (sizeX * 3);
      else
        return 0; 
    } 
  if (At(x,y).Terrain) { 
    rID t = PTerrainAt(x,y,c);
    if (TTER(t)->HasFlag(TF_WARN)) {
      if (TTER(t)->PEvent(EV_MON_CONSIDER,c,t) == ABORT) {
        if (dangerFactor & DF_IGNORE_TERRAIN) 
            return (sizeX * 3);
        else
            return 0; 
      } 
    } 
    if (TTER(t)->MoveMod)
      return 4;
    else
      return 2; 
  }
  return 2;
}
