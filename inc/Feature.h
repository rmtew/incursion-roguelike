/* FEATURE.H -- Copyright (c) 1999-2003 Julian Mensch
     This file contains class definitions for Feature,
   Door, Portal and Trap. 
*/


/* Features are dungeon features too large for the player
	 to pick up. They include Door, Trap, Trigger, Stair,
	 Throne, Mirror, Sink, Fountain, Pit, Rubble and so on. */
class Feature: public Thing
	{
    ARCHIVE_CLASS(Feature,Thing,r)
    END_ARCHIVE
		public:
    int16 cHP, mHP; 
		rID fID;
    int8 MoveMod; 
    Feature(int16 Image,rID _fID, int16 _Type) : Thing(Image, _Type) 
    { 
      fID = _fID; 
      Timeout = -1; 
      Flags = TFEAT(_fID)->Flags; 
      MoveMod = TFEAT(_fID)->MoveMod; 
      cHP = mHP = TFEAT(_fID)->hp;
    }
    Feature(rID _fID) : Thing( TFEAT(_fID)->Image, T_FEATURE) 
    { 
      fID = _fID; 
      Timeout = -1; 
      Flags = TFEAT(_fID)->Flags; 
      MoveMod = TFEAT(_fID)->MoveMod; 
      cHP = mHP = TFEAT(_fID)->hp;
    }
		virtual String & Name(int16 Flags=0);
		virtual String & Describe(Player *p);
		virtual void StatiOn(Status s);
    virtual void StatiOff(Status s);
    EvReturn Event(EventInfo &e);
    virtual int8 Material() {
      return TFEAT(fID)->Material;
    }
	};

class Door: public Feature
	{
    ARCHIVE_CLASS(Door,Feature,r)
    END_ARCHIVE

		public:
		Door(rID fID);
    ~Door();
		EvReturn Event(EventInfo &e);
		void SetImage();
		int8 DoorFlags;
    virtual bool isDead() { 
      return (Flags & F_DELETE) || (DoorFlags & DF_BROKEN); 
    }
    Glyph SecretSavedGlyph;
    virtual String & Describe(Player *p);
		EvReturn Zapped(EventInfo &e);
	};

class Trap: public Feature
  { 
    ARCHIVE_CLASS(Trap,Feature,r)
    END_ARCHIVE
    public:
      uint8 TrapFlags; rID tID;
      Trap(rID _fID,rID _tID) :
        Feature(TFEAT(_fID)->Image,_fID,T_TRAP)
        { TrapFlags = 0;
          tID = _tID; SetImage(); }
      EvReturn Event(EventInfo &e);
      virtual String & Name(int16 Flags=0);   
      ~Trap();
      void SetImage();
      int16 TrapLevel()
        { return TEFF(tID)->Level; }
      EvReturn TriggerTrap(EventInfo &e, bool foundBefore);
  };


/* A Portal is anything that can transport the player to a different
   map;, other then scrolls of level teleport, teletraps, pits, etc.
   They can also be special-function map spots that can be entered,
   like stores or guilds. Portals include:
   * literal magic portals
   * stairs
   * dungeon entrances
   * sub-dungeon entrances
   * stores
   * guilds
   * entrance to a building (i.e., the tavern)
   * the 'Rope Trick' rope
   * the 'Mordenkainen's Mansion' door
*/
class Portal: public Feature
  {
    ARCHIVE_CLASS(Portal,Feature,r)
    END_ARCHIVE
    public: 
    Portal(rID pID);
    bool isDownStairs()
      { return TFEAT(fID)->xval == POR_DOWN_STAIR; }
    EvReturn Event(EventInfo &e);
    EvReturn Enter(EventInfo &e);
    bool EnterDir(Dir d);
  };

