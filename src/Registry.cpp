/* REGISTRY.CPP -- See the Incursion LICENSE file for copyright information.

     Implements the object registry, data block registry and all of
   the functionality of saving and loading related groups of objects.
   Also includes the higher-level functions for saving and loading
   games and modules.

     Registry::Registry()
     void Registry::Empty()
     Registry::~Registry()
     bool Thing::inGroup(hObj h)
     size_t typeSize(int8 Type)
     inline bool hasData(int8 Type)
     void Registry::Block(void **Block, size_t sz)
     Object * Registry::Get(hObj h)
     bool Registry::Exists(hObj h)
     void * Registry::GetData(hData h)
     hObj Registry::RegisterObject(Object *o, bool loaded)
     hData Registry::RegisterBlock(void *o, hObj Owner, size_t sz, hData h)
     void Registry::ClearDataTable()
     void Registry::RemoveObject(Object *o)
     int16 Registry::SaveGroup(Term &t, hObj hGroup,bool newFile)
     int16 Registry::LoadGroup(Term &t, hObj hGroup)
     bool Game::SaveGame(Player &p)
     bool Game::LoadGame()
     void Game::SaveModule(int16 mn)
     bool Game::LoadModules()

*/

#include "Incursion.h"
#include <sys/stat.h>
#include <time.h>

Registry MainRegistry;
Registry ResourceRegistry;

Registry *theRegistry = &MainRegistry;

extern void InitGodArrays();

/* Save maps when we leave them, load maps when we enter them. */

struct fileHeader
  {
    uint32 Sig;
    char Version[12];
    char Name[72];
    int16 numGroups;
    int16 Compression;
    int16 numDependencies;
  };

struct dependHeader
  {
    char Filename[32];
  };

struct groupHeader
  {
    uint32 Signature;
    hObj   hGroup;
    int32  groupSize;
    int32  compSize;
    int32  objCount;
    int32  dataCount;
    int32  LastHandle;
  };

Registry::Registry()
  {
    memset(ObjTable,0,sizeof(RegNode)*OBJ_TABLE_SIZE);
    memset(DataTable,0,sizeof(DataNode)*DATA_TABLE_SIZE);
    LastUsedHandle = 128;
    #ifdef DEBUG_OBJECTS
    String path = T1->IncursionDirectory + "\\reglog.txt";
    reg_log = fopen((const char *)path,"wt");
    #endif
  }

void Registry::Empty()
  {
    /* This should tail after Game::Cleanup() */
    memset(ObjTable,0,sizeof(RegNode)*OBJ_TABLE_SIZE);
    memset(DataTable,0,sizeof(DataNode)*DATA_TABLE_SIZE);
    LastUsedHandle = 128;
  }

Registry::~Registry()
  {
    #ifdef DEBUG_OBJECTS
    fclose(reg_log);
    #endif
  }

bool Thing::inGroup(hObj h)
  { return (h == 0) || (h == m->myHandle); }      

size_t typeSize(int8 Type)
  {

    switch(Type)
      {
        /* Core game objects */
        case T_GAME:     return sizeof(Game);
        case T_TERM:     return sizeof(Term);
        case T_MAP:      return sizeof(Map);
        case T_REGISTRY: return sizeof(Registry);
        case T_MONSTER:  return sizeof(Monster);
        case T_PLAYER:   return sizeof(Player);
        //case T_NPC:      return sizeof(NPC);
        case T_MODULE:    return sizeof(Module);
        /* Features */
        case T_PORTAL:   return sizeof(Portal);
        case T_DOOR:     return sizeof(Door);
        case T_TRAP:     return sizeof(Trap);
        case T_FEATURE:  return sizeof(Feature);
        //case T_ALTAR:    return sizeof(Altar);

        /* Containers */
        case T_CHEST:    
        case T_CONTAIN:  return sizeof(Container);

        /* Items */
        case T_FOOD:     return sizeof(Food);

        case T_STATUE:   case T_FIGURE:
        case T_CORPSE:   return sizeof(Corpse);
        
        case T_MISSILE:  case T_BOW:
        case T_WEAPON:   case T_STAFF:
                         return sizeof(Weapon);
        case T_ARMOUR:    case T_SHIELD:
        case T_GAUNTLETS:
        case T_BOOTS:    
                         return sizeof(Armour);

        case T_ANNOT:    return sizeof(Annotation);
        //case T_CONSTRUCT:return sizeof(Construct);
        //case T_ILLUSION: return sizeof(Illusion);
        default:
          if (Type >= T_FIRSTITEM && Type <= T_LASTITEM)
            return sizeof(Item);
      }
    Error("Strange Type ID in typeSize!");
    return 400;
  }
              


inline bool hasData(int16 Type) {
    switch(Type) {
    case T_MODULE:   /* Resource arrays; text/data/code segments */
    case T_PLAYER:  /* Name, Notes, History, other Strings */
    case T_MAP:     /* Map Grid */
        return true;
    default:
        return false;
    }
}

void Registry::Block(void **Block, size_t sz)
  {
    ASSERT(theRegistry->Get(hCurrent));

    if (saveMode)
      *((int*)Block) = RegisterBlock(*Block,hCurrent,sz);
     else
      *Block = GetData(*((int*)Block));
  } 


Object * Registry::Get(hObj h)
  {
    RegNode *r ; 
    ASSERT(h <= LastUsedHandle);
    if (h <= 0) // ww: ouch, use to be !h || h<-1 ! 
      return NULL;
    else if (h < 128) // ww: && h > -1) -> implied by above "if" 
      return theGame->VM.GetSystemObject(h);
    r = &(ObjTable[h & OBJ_TABLE_MASK]);
    do {
      if (r->pObj && r->pObj->myHandle == h)
          return r->pObj;
      else 
          r = r->Next;
    } while (r); 
    //LoadObject(h);
    Error("Registry::Get -- invalid object handle (%d)!",h);
    return NULL;
  }

bool Registry::Exists(hObj h)
  {
    RegNode *r = &(ObjTable[h & OBJ_TABLE_MASK]);
    if (h > LastUsedHandle)
      return false;
    if (!h)
      return false;
    if (h < -1)
      return false;
    if (h < 128)
      return (theGame->VM.GetSystemObject(h) != 0);
    
    while(r) {
      if (r->pObj)
        if (r->pObj->myHandle == h)
          return true;
      r = r->Next;
      }
    return false;
  }


void * Registry::GetData(hData h)
  {
    DataNode *d = &(DataTable[h & DATA_TABLE_MASK]);
    ASSERT(h <= LastUsedHandle);
    if (!h)
      return NULL;
    while(d) {
      if (d->myHandle == h)
        return d->pData;
      d = d->Next;
      }
    //LoadObject(d);
    Fatal("Registry::GetData -- invalid object handle (%d)!",h);
    return NULL; 
  }


hObj Registry::RegisterObject(Object *o, bool loaded)
  {
    
    hObj h; RegNode *r;
    if (loaded) {
      h = o->myHandle;
      LastUsedHandle = max(LastUsedHandle,h+1);
      }
    else
      h = LastUsedHandle++;
    if (o->Type == T_MODULE)
      hModule = h;
    hData hm = h & OBJ_TABLE_MASK;
    r = &(ObjTable[hm]);
    if (!r->pObj) 
      ObjTable[hm].pObj = o;
    else {
      while(r->Next)
        r = r->Next;
      r->Next = new RegNode;
      r = r->Next;
      r->pObj = o;
      r->Next = NULL;
      }
    #ifdef DEBUG_OBJECTS
      fprintf(reg_log,"Object #%d -- %s (%d)\n",h,(const char*)o->Name(0),o->myHandle);
    #endif
    if (h > LastUsedHandle)
      Error("Incorrect handle assigned!");

    return h;
  }

hData Registry::RegisterBlock(void *o, hObj Owner, size_t sz, hData h)
  {
    if (!o)
      return 0;
    if (!h)
      h = LastUsedHandle++;
    ASSERT(theRegistry->Get(Owner));
    hData hm = h & DATA_TABLE_MASK; 
    DataNode *r = &(DataTable[hm]);
    if (!r->pData)
      {
        DataTable[hm].pData    = o;
        DataTable[hm].Size     = sz;
        DataTable[hm].hOwner   = Owner;
        DataTable[hm].myHandle = h;
      }
    else {
      /* We need to fix this to allow registering the same block
         twice. */
      while(r->Next)
        r = r->Next;
      r->Next = new DataNode;
      r = r->Next;
      r->pData    = o;
      r->Size     = sz;
      r->hOwner   = Owner;
      r->myHandle = h;
      r->Next = NULL;
      }
    return h;
  }

void Registry::ClearDataTable()
  {
    int32 i; DataNode *d, *d2;
    for (i=0;i!=DATA_TABLE_SIZE;i++)
      {
        d = DataTable[i].Next;
        while (d) {
          d2 = d->Next;
          delete d;
          d = d2;
          };
      }
    memset(DataTable,0,sizeof(DataNode)*DATA_TABLE_SIZE);
  } 



void Registry::RemoveObject(Object *o)
  {
    hObj h = o->myHandle; int32 i;
    RegNode *r, *last;
    
    #if 0
    if (TheMainMap)
      for (i=0;TheMainMap->Things[i];i++) {
        if (TheMainMap->Things[i] == o->myHandle)
          Error("Object being deleted before being remove from map!");
        if (oThing(TheMainMap->Things[i])->HasStati(TARGET,TA_POTENTIAL,(Thing*)o))
          Error(Format("Thing %s (%d) has object-in-deletion %s as potential target!",
            (const char*)oThing(TheMainMap->Things[i])->Name(0),TheMainMap->Things[i],
            (const char*)o->Name(0)));
        }
    #endif

    h = o->myHandle;
    r = &(ObjTable[h & OBJ_TABLE_MASK]);
    last = NULL;

    while(r) {
      if (r->pObj && r->pObj->myHandle == h)
        {
          if (!last && !r->Next) 
            r->pObj = NULL;
          else if (!last) {
            last = r->Next;
            r->pObj = r->Next->pObj;
            r->Next = r->Next->Next;
            delete last; }
          else {
            last->Next = r->Next;
            delete r;
            }
          return;
        }
      last = r;
      r = r->Next;
      }

    if (hasData(o->Type))
      {
        DataNode *d;
        for(i=0;i!=DATA_TABLE_SIZE;i++)
          {
            RestartDataRemove:
            d = &(DataTable[i]);
            while(r) {
              if (r->pObj->myHandle == h)
                {
                  if (!last && !r->Next) 
                    r->pObj = NULL;
                  else if (!last) {
                    last = r->Next;
                    r->pObj = r->Next->pObj;
                    r->Next = r->Next->Next;
                    delete last; }
                  else {
                    last->Next = r->Next;
                    delete r;
                    }
                  goto RestartDataRemove;
                }
              last = r;
              r = r->Next;
              }
          }
      }

    //LoadObject(h);
    Error("Registry::UnlinkObject -- invalid object handle!");
  }

int16 Registry::SaveGroup(Term &t, hObj hGroup, bool use_lz, bool newFile) {
    int32 i; uint32 sig;
    groupHeader gh; fileHeader fh;
    RegNode *r; DataNode *d;
    uint32 ghPos;

    ClearDataTable();

    t.Seek(0,SEEK_SET);   

    if (!newFile) {
        t.FRead(&fh,sizeof(fh));
        if (fh.Sig != SIGNATURE)
            throw ENOTARC;
        if (strcmp(fh.Version,VERSION_STRING))
            throw EBADVER;

        /* Run through a list of group headers. When/if we find the header
        for the block we're about to save, delete the old saved version
        so that it's not saved twice. */
        for(i=0;i!=fh.numGroups;i++) {
            fh.numGroups++;
            t.Seek(0,SEEK_SET);
            t.FWrite(&fh,sizeof(fh));

            t.FRead(&gh,sizeof(gh));
            if (gh.Signature != SIGNATURE)
                throw ECORRUPT;
            if (gh.hGroup == hGroup) {
                // ww: was: t.Seek(-sizeof(gh),SEEK_CUR);
                // unary minus applied to unsigned type still results in 
                // unsigned type! 
                t.Seek(0 - (signed)sizeof(gh),SEEK_CUR);
                /* Remove the old save-image of the group */
                t.Cut(sizeof(gh) + gh.groupSize);
                goto DoneDeletion;
            } else
                t.Seek(gh.groupSize,SEEK_CUR);
        }

DoneDeletion:
        ;
    }
    /* Move us right to the very end of the file, where we're going
    to append the group we're saving right now. */
    t.Seek(0,SEEK_END);

    /* We can't write the real group header yet, because we don't know
    how many objects we are saving. So we'll write an undefined group
    header to fill the space, then go back and write the real one
    later. */ 
    ghPos = t.Tell();

    t.FWrite(&gh,sizeof(gh));

    /* Initialize the Memory File */
    CFile *cf = new CFile(&t);

    /* Write the objects in sequential order, each with a single byte in
    front of it to tell its type. */
    gh.objCount = 0;
    saveMode = true;
    for(i=0;i!=OBJ_TABLE_SIZE;i++) {
        if (ObjTable[i].pObj) {
            r = &(ObjTable[i]);
            while (r) {
                if (!r->pObj->inGroup(hGroup)) {
                    r = r->Next;
                    continue;
                }

                hCurrent = r->pObj->myHandle;
                r->pObj->Serialize(*this,true);

                gh.objCount++;
                /* Write the object type */
                cf->FWrite(&(r->pObj->Type),1);
                /* Write the object itself */
                cf->FWrite(r->pObj,typeSize((int8)r->pObj->Type));

                r = r->Next;
            }
        }
    }

    /* We write a special signature DWORD inbetween the objects and
    the data in order to assure that we're actually reading a
    valid Incursion data file accurately. This is mostly paranoia,
    but it will likely help versus otherwise-untracable bugs. */
    cf->FWrite(&(sig = SIGNATURE_TWO),4);

    /* Now write the data, in a manner similar to how the objects were
    written, but substituting size value for type index. */
    gh.dataCount = 0;
    for(i=0;i!=DATA_TABLE_SIZE;i++) {
        if (DataTable[i].pData) {
            d = &(DataTable[i]);
            while (d) {
                if (!Exists(d->hOwner)) {
                    d = d->Next;
                    continue;
                }
                if (!Get(d->hOwner)->inGroup(hGroup)) {
                    d = d->Next;
                    continue;
                }
                gh.dataCount++;
                ASSERT(d->hOwner > 127);
                ASSERT(d->myHandle > 127);
                /* Write the handle of the data */
                cf->FWrite(&(d->myHandle),4);
                /* Write the handle of the data's owner */
                cf->FWrite(&(d->hOwner),4);
                /* Write the size in bytes of the data */
                cf->FWrite(&(d->Size),4);
                /* And now write the data proper */
                cf->FWrite(d->pData,d->Size); // ww: segfault here, this=0 (?)

                d = d->Next;
            }
        }
    }

    int32 csz = cf->CommitCompressed(t.Tell(), use_lz);

    /* Now that we have all the needed information, jump back in the file
    and write out the group header. */
    gh.groupSize  = cf->FSize();
    gh.compSize   = csz;
    gh.Signature  = SIGNATURE;
    gh.hGroup     = hGroup;
    gh.LastHandle = LastUsedHandle;
    t.Seek(ghPos,SEEK_SET);
    t.FWrite(&gh,sizeof(gh));

    delete cf;

    /* Fix all the pointers in memory to data blocks, so that they point
    properly again. */
    saveMode = 0;
    for(i=0;i!=OBJ_TABLE_SIZE;i++) {
        if (ObjTable[i].pObj) {
            r = &(ObjTable[i]);
            while (r) {
                if (!r->pObj->inGroup(hGroup)) {
                    r = r->Next;
                    continue;
                }

                r->pObj->Serialize(*this,false);
                r = r->Next;
            }
        }
    }

    return 0;
}

#pragma warning(disable:4291) // no matching operator delete found; memory will not be freed if initialization throws an exception

int16 Registry::LoadGroup(Term &t, hObj hGroup, bool use_lz) {
    fileHeader fh;
    groupHeader gh;
    OArray<Object*,10,10> LoadedObjects;
    Object *o; void *pData;
    int8 oType; int32 i;
    hObj   DataOwner;
    hData  DataHandle;
    uint32 DataSize;
    uint32 Seperator;

    ClearDataTable();

    loadMode = true;

    /* Read Header */
    t.FRead(&fh,sizeof(fh));

    /* Check Version Number */
    if (strcmp(fh.Version,VERSION_STRING))
      throw EBADVER;
    
    /* Scan through the saved groups for the specific one that we're
       looking to restore. */
    for(i=0;i!=fh.numGroups;i++)
      {
        t.FRead(&gh,sizeof(gh));
        if (gh.Signature != SIGNATURE)
          throw ECORRUPT;
        if ((gh.hGroup == hGroup) || !hGroup)
          goto foundGroup;

        /* This is the wrong group, so skip over it. */
        t.Seek(gh.groupSize,SEEK_CUR);
      }
    /* The chunk is missing! */
    throw ENOCHUNK;

foundGroup:
    CFile *cf = new CFile(&t);
    cf->LoadCompressed(t.Tell(), gh.compSize, gh.groupSize, use_lz);

    LastUsedHandle = max(LastUsedHandle, gh.LastHandle);
    for (i=0; i != gh.objCount; i++) {
        /* What type of object is this? */
        cf->FRead(&oType,1);                   

        /* Allocate the object. Account for the
           special case of the (singular) Game
           object, which is not reallocated but
           always overwritten. */
        if (oType == T_GAME)
          o = theGame;
        else
          o = (Object*)malloc(typeSize(oType));
        if (!o)
          throw EMEMORY;
         
        /* ...read in the object... */
        cf->FRead(o,typeSize(oType));

        /* ...reattach its virtual table pointer by calling
              a kludged, overridden new() operator in Object
              that "allocates" the space we've allocated, and
              then goes to a constructor that does nothing. */
        switch(oType)
          {
            /* Core game objects */
            case T_GAME:     new(o) Game(this); break;
            //case T_TERM:     new(o) Term(this); break;
            case T_MAP:      new(o) Map(this); break;
            //case T_REGISTRY: new(o) Registry(this); break;
            case T_MONSTER:  new(o) Monster(this); break;
            case T_PLAYER:   new(o) Player(this); break;
            //case T_NPC:      new(o) NPC(this); break;
            case T_MODULE:   new(o) Module(this); break;
            /* Features */
            case T_PORTAL:   new(o) Portal(this); break;
            case T_DOOR:     new(o) Door(this); break;
            case T_TRAP:     new(o) Trap(this); break;
            case T_FEATURE:  new(o) Feature(this); break;
            //case T_ALTAR:    new(o) Altar(this); break;
        
            /* Containers */
            case T_CHEST:
            case T_CONTAIN:  new(o) Container(this); break;

            /* Items */
            case T_COIN:     new(o) Coin(this); break;
            case T_FOOD:     new(o) Food(this); break;
            case T_STATUE:
            case T_FIGURE:
            case T_CORPSE:   new(o) Corpse(this); break;
            case T_MISSILE:  case T_BOW:
            case T_WEAPON:   new(o) Weapon(this); break;
            case T_ARMOUR:    case T_SHIELD:
            case T_GAUNTLETS:
            case T_BOOTS:    
                             new(o) Armour(this); break;

            //case T_CONSTRUCT:new(o) Construct(this); break;
            //case T_ILLUSION: new(o) Illusion(this); break;
            default:
              if (oType >= T_FIRSTITEM && oType <= T_LASTITEM)
                new(o) Item(this); 
              else
                Fatal("Wrong Type right after call to typeSize?!");
          }

        /* Verify it is in fact the correct type. */
        if (o->Type != oType)
          throw ECORRUPT;

        /* And register our new object in the Registry. */
        RegisterObject(o,true);

        /* Add it to the serialization list */
        LoadedObjects.Add(o);
      }

    /* Read and confirm the special objects-data seperator to assure
       that the data file is not corrupt. */
    cf->FRead(&Seperator,4);
    if (Seperator != SIGNATURE_TWO)
      throw ECORRUPT;

    for(i=0;i!=gh.dataCount;i++)
      {
        /* Read data handle */
        cf->FRead(&DataHandle,4);
        /* Read data owner's handle */
        cf->FRead(&DataOwner,4);
        /* Read data size */
        cf->FRead(&DataSize,4);

        ASSERT(DataHandle > 127);
        ASSERT(DataOwner  > 127);

        /* Allocate space for data */
        pData = malloc(DataSize);
        if (!pData)
          throw EMEMORY;

        /* Read in the data... */
        cf->FRead(pData,DataSize);

        /* Add it to the Registry */
        RegisterBlock(pData,DataOwner,DataSize,DataHandle);
      }

    /* Restore the objects' pointers to their data. */
    saveMode = false;
    for(i=0;i!=gh.objCount;i++) {
      hCurrent = (*(LoadedObjects[i]))->myHandle;
      (*(LoadedObjects[i]))->Serialize(*this,false);
      }

    loadMode = false;
    
    delete cf;
    
    return 0;
}

bool Game::SaveGame(Player &p) {
      fileHeader fh; int32 i;
      String fn, base, desc;

      p.TouchGallery(true);

      T1->ChangeDirectory(T1->SaveSubDir());

      memset(p.MessageQueue,0,sizeof(String)*8);

      if (SaveFile.GetLength()) { 
          // ww: I'm still getting that hideous savefile corruption bug, so
          // let's make a backup copy instead of just deleting this!
          FILE *fin = NULL, *fout = NULL;
          char buf[4096]; 
          if (!T1->Exists(SaveFile) || !(fin = fopen((const char*)SaveFile,"rb")))
            { p.MyTerm->Message("Save File Backup: Failed (#1)"); goto failed; }
          fout = fopen((const char*)(SaveFile + BACKUP_SUFFIX),"wb");
          if (!fout)
            { p.MyTerm->Message("Save File Backup: Failed (#2)"); goto failed; }
          struct stat statbuf;
          if (stat(SaveFile,&statbuf)) 
            { p.MyTerm->Message("Save File Backup: Failed (#3)"); goto failed; }
          off_t sofar;
          for (sofar = 0; sofar < statbuf.st_size;) {
              size_t this_read = fread(buf,1,min(sizeof(buf),statbuf.st_size - sofar),fin);
              if (this_read == 0) 
                { p.MyTerm->Message("Save File Backup: Failed (#4)"); goto failed; }
              size_t this_write ;
              this_write = fwrite(buf,1,this_read,fout);
              if (this_write == 0 || this_read != this_write) 
                { p.MyTerm->Message("Save File Backup: Failed (#5)"); goto failed; }
              sofar += this_read; 
          } 
failed: 
          if (fin) fclose(fin);
          if (fout) fclose(fout);
          T1->Delete(SaveFile);
          //unlink(SaveFile);
          SaveFile.Empty(); 
      }

//Retry:
      base = p.Named;
      while (base.strchr(' '))
          base = base.Left(base.strchr(' '));
      if (!base.GetLength())
          base = "Unnamed";
      for(i=0;i!=base.GetLength();i++)
          if (!isalpha(base[i]))
              base.SetAt(i,'_');
      fn = Format("%s.sav",(const char*)base);
      SaveFile = fn; 
      if (T1->Exists(fn)) 
          for(i=2;i!=255;i++) {
              SaveFile = Format("%s_%d.sav", (const char*)base,i);
              fn = SaveFile; 
              if (!T1->Exists(fn))
                  break;
          }

      desc = p.Named + SC(", ");
      desc += NAME(p.RaceID);
      for(i=0;i!=3 && p.ClassID[i];i++)
          desc += Format("%s %s %d", i ? " /" : "",
          NAME(p.ClassID[i]), p.Level[i]);

#if 0
      if (RES(p.m->dID)->Type == T_TDUNGEON)
          desc += Format(" (%s, at %03dm)",
          RES(p.m->dID)->RandMessage(MSG_SHORTNAME), p.m->Depth*10);
      else
          desc += Format(" (%s)",
          RES(p.m->dID)->RandMessage(MSG_SHORTNAME));
#endif

      desc = desc.Left(63);

      // weimer: I'm not sure how to hack this in, but it should happen
      // before we serialize the player
      theGame->GetPlayer(0)->StoreLevelStats((uint8)(theGame->GetPlayer(0)->m->Depth));

      try {  
          T1->OpenWrite(fn);

          memset(&fh,0,sizeof(fh));
          strcpy(fh.Version,VERSION_STRING);
          fh.Sig = SIGNATURE;
          fh.numGroups = 1;
          strncpy(fh.Name,desc,71);
          T1->FWrite(&fh,sizeof(fh));
          theRegistry->SaveGroup(*T1,0,false,true);
          T1->Close();
          T1->ChangeDirectory(T1->IncursionDirectory);
      }
      catch (int error_number) {
          Error("Error writing save file (%s).",
              Lookup(FileErrors,error_number));
          T1->ChangeDirectory(T1->IncursionDirectory);
          return false;

      }
      return true;
}

bool Game::LoadGame(bool backup) {
    int32 i;
    String Filenames[128], Filedescs[128], fn;
    fileHeader fh; bool found;
    int patternsize = backup ? strlen(SAVE_BACKUP_PATTERN) : strlen(SAVE_PATTERN);
    char *pattern = (char *)alloca(patternsize + 1);
    strncpy(pattern, backup ? SAVE_BACKUP_PATTERN : SAVE_PATTERN, patternsize);
    pattern[patternsize] = '\0';

    T1->ChangeDirectory(T1->SaveSubDir());

    try {
        if (!T1->FirstFile(pattern)) {
NoSaved:
            T1->LOptionClear();
            T1->Box(Format("There are currently no %s available to load.",
                backup ? "backup files" : "saved games"));
            return false;
        }

        T1->SetWin(WIN_SCREEN);
        T1->Clear(); T1->GotoXY(0, 0);
        i = 0;
        found = false;
        do {
            T1->FRead(&fh, sizeof(fh));
            T1->Close();
            if (stricmp(VERSION_STRING, fh.Version))
                continue;

            Filenames[i] = T1->GetFileName();
            Filedescs[i] = fh.Name;
            if (backup) {
                struct stat statbuf;
                stat(Filenames[i], &statbuf);
                Filedescs[i] += Format(" (BACKUP at %s)", ctime(&statbuf.st_ctime));
            }
            T1->LOption(Filedescs[i], i);
            found = true;

            i++;
        } while (T1->NextFile() && i < 128);

        if (!found)
            goto NoSaved;

        T1->Clear();
        if (!i)
            goto NoSaved;

        i = T1->LMenu(MENU_SORTED | MENU_ESC, "Choose a character:\n\n", WIN_SCREEN);
        if (i < 0) {
            T1->ChangeDirectory(T1->IncursionDirectory);
            return false;
        }

        fn = Filenames[i];
        T1->Clear();
        T1->GotoXY(0, 0);
        T1->Color(WHITE);
        T1->Write("Loading... ");

        T1->OpenRead(fn);
        /* The central Game object gets overwritten. */
        MainRegistry.RemoveObject(theGame);
        MainRegistry.LoadGroup(*T1, 0, false);
        T1->Close();
        SaveFile = fn;
        T1->ChangeDirectory(T1->IncursionDirectory);
    } catch (int error_number) {
        Error("Error reading saved game (%s).", Lookup(FileErrors, error_number));
        T1->ChangeDirectory(T1->IncursionDirectory);
        return false;
    }

    /* LoadGroup should have overwritten theGame with the
    restored version from disk; thus, p[0] points to the
    player and m[0] points to the current map. The dungeon
    is described by the arrays in theGame as well. Rather
    than crash horribly, however, let us just confirm this. */

    if (!p[0] || !m[0]) {
        Error("Null map/player after saved game loaded!");
        return false;
    }
    /* Here, we look what resource files this saved game depends
    on, and restore them each in order. */
    theRegistry = &ResourceRegistry;
    memset(Modules, 0, sizeof(Module*)*MAX_MODULES);
    for (i = 0; ModFiles[i]; i++) {
        try {
            char * filespec = ModFiles[i]->FName;
            T1->ChangeDirectory(T1->ModuleSubDir());
            //T1->Scour(ModFiles[i]->FName,true);
            T1->OpenRead(filespec);
            ResourceRegistry.LoadGroup(*T1, ModFiles[i]->hMod, true);
            T1->Close();
            if (!oThing(ModFiles[i]->hMod))
                throw EHANDLE;

            Modules[ModFiles[i]->Slot] = oModule(ModFiles[i]->hMod);
            T1->ChangeDirectory(T1->IncursionDirectory);
        } catch (int error_number) {
            Error("Error loading module '%s' (%s).", ModFiles[i]->FName, Lookup(FileErrors, error_number));
            T1->ChangeDirectory(T1->IncursionDirectory);
        }
    }

    theRegistry = &MainRegistry;
    memset(oPlayer(p[0])->MessageQueue, 0, sizeof(String) * 8);
    InitGodArrays();

    oPlayer(p[0])->MyTerm = T1;
    T1->SetPlayer(oPlayer(p[0]));
    T1->SetMap(oMap(m[0]));
    T1->SetMode(MO_PLAY);
    oPlayer(p[0])->CalcValues();
    oPlayer(p[0])->CalcVision();
    T1->AdjustMap(oPlayer(p[0])->x, oPlayer(p[0])->y, true);
    T1->ShowStatus();
    T1->ShowTraits();
    oPlayer(p[0])->LoadOptions(false);
    oPlayer(p[0])->ResetLevelStats();
    oPlayer(p[0])->TouchGallery(true);
    oPlayer(p[0])->IPrint("Welcome back to Incursion, <Str>!", (const char*)oPlayer(p[0])->Name(0));
    if (oPlayer(p[0])->GodID)
        oPlayer(p[0])->GodMessage(oPlayer(p[0])->GodID, MSG_SALUTATION);
    doQuit = false;

    return true;
}

void Game::SaveModule(int16 mn) {
    fileHeader fh;
    if (!Modules[mn]) {
        Error("Attempt to save non-existent module!");
        return;
    }
    if (!Modules[mn]->FName) {
        Error("Module has no filename!");
        return;
    }

    try {
        const char * filespec = Modules[mn]->GetText(Modules[mn]->FName);
        T1->ChangeDirectory(T1->ModuleSubDir());
        T1->OpenWrite(filespec);
        memset(&fh, 0, sizeof(fh));
        strcpy(fh.Version, VERSION_STRING);
        fh.Sig = SIGNATURE;
        fh.numGroups = 1;
        strncpy(fh.Name, Modules[mn]->GetText(Modules[mn]->Name), 71);
        T1->FWrite(&fh, sizeof(fh));
        theRegistry = &ResourceRegistry;
        theRegistry->SaveGroup(*T1, Modules[mn]->myHandle, true, true);
        theRegistry = &MainRegistry;
        T1->Close();
        T1->ChangeDirectory(T1->IncursionDirectory);
    } catch (int error_number) {
        Error("Error writing module (%s).", Lookup(FileErrors, error_number));
        T1->ChangeDirectory(T1->IncursionDirectory);
        return;
    }

    return;
}

bool Game::LoadModules() {
    String fn;
    ModuleRecord mr;
    ModuleRecord mrBacklog[MAX_MODULES];
    uint8 sl, i, backlogCount = 0, moduleCount = 0;

    Cleanup();

    T1->ChangeDirectory(T1->ModuleSubDir());

    theRegistry = &ResourceRegistry;
    try {
        if (!T1->FirstFile("*.Mod")) {
            Fatal(Format("%s\nCan't find any game data modules!", (const char*)T1->ModuleSubDir()));
        }

        do {
            strcpy(mr.FName, T1->GetFileName());
            ResourceRegistry.LoadGroup(*T1, 0, true);
            T1->Close();
            mr.hMod = ResourceRegistry.GetModuleHandle();
            if (!oModule(mr.hMod))
                throw EHANDLE;

            if ((backlogCount + moduleCount) == MAX_MODULES) {
                Error("Only %d modules can be loaded at once!", MAX_MODULES);
                delete oModule(mr.hMod);
                break;
            }

            if (oModule(mr.hMod)->Slot == -1) {
                mrBacklog[backlogCount++] = mr;
            } else {
                sl = (uint8)oModule(mr.hMod)->Slot;
                if (Modules[sl]) {
                    Error("Two fixed-slot modules (%s and %s) conflict over slot %d!",
                        oModule(mr.hMod)->GetText(oModule(mr.hMod)->Name),
                        Modules[sl]->GetText(Modules[sl]->Name), sl);
                    delete oModule(mr.hMod);
                    continue;
                }

                Modules[sl] = oModule(mr.hMod);
                mr.Slot = sl;
                ModFiles.Add(mr);

                moduleCount++;
            }
        } while (T1->NextFile());

        // Fill in the empty module slots with the backlog of floating slot modules.
        sl = 0;
        for (i = 0; i < backlogCount; i++) {
            while (Modules[sl])
                sl++;
            ASSERT(sl < MAX_MODULES);
            mr = mrBacklog[i];
            Modules[sl] = oModule(mr.hMod);
            mr.Slot = sl;
            ModFiles.Add(mr);
        }
    } catch (int error_number) {
        Error("Error loading module '%s' (%s).", mr.FName, Lookup(FileErrors, error_number));
        theRegistry = &MainRegistry;
        T1->ChangeDirectory(T1->IncursionDirectory);
        return false;
    }

    theRegistry = &MainRegistry;
    T1->ChangeDirectory(T1->IncursionDirectory);
    InitGodArrays();

    return true;
}
