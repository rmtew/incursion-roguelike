/* See the Incursion LICENSE file for copyright information.

     This file contains class definitions for Item, Food,
   Corpse, Container, Weapon, Armour, Coin and ItemGen.
*/

int16 MaterialHardness(int8 Mat, int8 DType);
bool MaterialIsMetallic(int8 mat);
bool MaterialIsWooden(int8 mat);
bool MaterialIsOrganic(int8 mat);
bool MaterialIsCorporeal(int8 mat);

class Item: public Thing, public Magic
	{
		friend class Container;
    friend Item* Monster::GetInv(bool first);
    friend Item* Character::GetInv(bool first);
    friend class TextTerm;
    friend void Player::JournalNoteItem(Item *i);
    ARCHIVE_CLASS(Item,Thing,r)
      Inscrip.Serialize(r); 
      GenStats.Serialize(r);
    END_ARCHIVE
    protected:
			uint16 Known;
			int8 Plus, Charges, DmgType; 
      int16 GenNum;
      
      hObj Parent;
      rID  homeID;
      int16 Flavor;
    
      int16 cHP;
		public:
      int16 Age;
      uint8 swingCount;
			uint32 Quantity;
      String Inscrip;
      String GenStats;
      rID iID, eID;
			uint16 IFlags;
      Item(Item *i);

      /* General Functions */
			Item(rID iID,int16 _Type);
      virtual void Initialize(bool in_play = false);
      virtual bool isQItem() { return false; }
      virtual void ChangeIID(rID _iID, bool mult=false);
      virtual void Dump();
			virtual String & Name(int16 Flags=0);
      virtual bool isPlural() { return Quantity != 1 ||
                                  (iID && TITEM(iID)->HasFlag(IT_PLURAL)); }
      virtual String & Describe(Player *p);
	  virtual String & DescribeWithTitle(Player *p) {
		  uint32 gid = GLYPH_ID_VALUE(Image);
		  // To understand LITERAL_CHAR mangling, search for it elsewhere.
		  return Format("%c%s%c ('%c%c%c%c%c')\n%s",
			  -GREEN, (const char*)Name(NA_SINGLE | NA_XCAPS | NA_THE | NA_LONG | NA_MECH | NA_MONO | NA_NO_TERSE), -GREY,
			  -(char)GLYPH_FORE_VALUE(Image), LITERAL_CHAR, LITERAL_CHAR1(gid), LITERAL_CHAR2(gid), -GREY,
			  (const char *)Describe(p));
	  }
      Creature* Owner() 
        { // ww: maybe this is fixed once and for all now ...
          if ((!Parent) || (!theRegistry->Exists(Parent)) || (!oThing(Parent))) 
            return NULL;
          return oThing(Parent)->isCreature() ? 
            oCreature(Parent) : oItem(Parent)->Owner(); }
      virtual Thing* GetParent() { return oThing(Parent); }
      virtual void Remove(bool isDelete);
      virtual rID  GetCurrentRegion();
      virtual rID  GetHomeRegion() { return homeID; }
      virtual bool isActivatable();
      virtual bool canFinesse();
      virtual void ReApply();
      bool allowedSlot(int16 slot, Creature *me);
      bool activeSlot(int16 slot);
      virtual bool betterFor(Creature *c, Item *curr);

      EvReturn Event(EventInfo &e);

      /* Item Creation, Probability, Level, Knowledge */
      static Item* GenItem(uint16 Flags, rID xID, int16 Depth, int8 Luck, ItemGen *Gen);
      static Item* Create(rID r);
      void SetCharges(int8 ch) { Charges = ch; }
      int8 GetCharges()        { return Charges; }
      virtual void MakeKnown(uint8 k);
      virtual void SetKnown(uint8 k)   { Known = k; }


      /* General Properties */
      bool isEdible(Creature *p);
      bool isMagic() { return eID || Plus; }
      virtual bool useStrength() { return false; } 
      void MakeMagical(rID eID, int16 spe=0);
      virtual int32 Weight(bool psych_might = false);
      virtual int16 Hardness(int8 DType);
      virtual bool HasSpell(uint16 sp);
      virtual rID  GetCorpseType() { return 0; }
      virtual bool HasQuality(int16 q) { return false; }
      virtual bool HasIFlag(uint8 fl) { return TITEM(iID)->HasFlag(fl); }
      virtual bool isGroup(uint32 gr) { return false; }
      virtual bool thrownOnly() { return false; }
      virtual bool KnownQuality(int16 q) { return false; }
      virtual void RemoveQuality(int8 q) 
        { Error("Removing quality to non-Weapon/Armour Item!"); }
      virtual void PurgeAllQualities() 
        { Error("Purging all qualities from non-Weapon/Armour Item!"); }
      virtual void AddQuality(int8 q, int16 param=0) 
        { Error("Adding quality to non-Weapon/Armour Item!"); }
      virtual void SetBane(int16 _Bane) { Error("SetBane on non-Weapon."); }
      virtual int16 GetBane() { return 0; }
      virtual void RandomBane() { }
      virtual bool QualityOK(int8 q,uint8 lev=200) {return false; }
      virtual int16 GetQuality(int16 i) { return 0; }
      virtual int32 getShopCost(Creature *Buyer, Creature *Seller);
      virtual bool isBaneOf(int16 MType) { return false; }
      virtual bool isBaneOf(Creature *c) { return false; }
      virtual int32 GetGroup();
      virtual int16 ItemLevel(bool bounded=true);
      virtual int16 PItemLevel(Creature *cr);
      virtual int16 GetLightRange();
      uint32 GetQuantity() { return Quantity; }
      void SetQuantity(uint32 _q) { Quantity = _q; }
      void SetOwner(hObj h) { Parent = h; }
      Item* TakeOne();
      Item* TryStack(Item *i);
      Item* TryStack(Map *m, int16 x, int16 y);
      int16 MaxHP();
      void  MendHP(int16 hp) { cHP = min(MaxHP(),cHP+hp); }
      int16 GetHP() { return cHP; }
      virtual bool operator==(Item &i);

      /* Properties */
      bool isBlessed() { return (IFlags & IF_BLESSED) != 0; }
      bool isCursed()  { return (IFlags & IF_CURSED) != 0;  }
      bool isMaster()  { return (IFlags & IF_MASTERWORK) != 0; }
      bool isKnown(uint16 kn = KN_MAGIC) { return (Known & kn) == kn; }
      int8 GetPlus()   { 
        if (IFlags & IF_CURSED)
          return HighStatiMag(BOOST_PLUS)-Plus;
        // ww: check TEFF(eiD) before TEFF(eID)->Foo 
        /* ww: I don't think this is right. 
        else if (eID && TEFF(eID) && TEFF(eID)->HasFlag(EF_CURSED))
          return 0;
          */
        else
          return max(Plus,HighStatiMag(BOOST_PLUS));
      }
      int8 GetInherentPlus() { return Plus; }
      void SetInherentPlus(int8 i) { int32 dmg;
          dmg = (cHP*1000) / MaxHP();
          Plus = i; 
          cHP =  (int16)((dmg * MaxHP())/1000);
          ReApply();
      }
      
      virtual int8 Material() {
        return TITEM(iID)->Material;
      }
      virtual bool isGhostTouch(); 
      
      /* is this a crossbow that needs to be cocked? */
      bool needsToBeCocked()
      {
        return (HasIFlag(WT_CROSSBOW) && !HasStati(COCKED) && !HasQuality(WQ_QUICK_LOADING));
      } 
      bool isMetallic()   { return MaterialIsMetallic(Material()); }
      bool isOrganic()    { return MaterialIsOrganic(Material()); }
      bool isCorporeal()  { return MaterialIsCorporeal(Material()); }

      /* Responses to Manipulation */
      EvReturn Damage(EventInfo &e);
			EvReturn ZapWand(EventInfo &e);
      EvReturn Activate(EventInfo &e);
      EvReturn ReadScroll(EventInfo &e);
      EvReturn DrinkPotion(EventInfo &e);
      EvReturn UseStaff(EventInfo &e);
      virtual void StatiOn(Status s);
      virtual void StatiOff(Status s, bool elapsed=false);

      /* Virtuals for derived classes */
      virtual int16 ArmVal(int16 ty, bool knownOnly = false);
      virtual int16 CovVal(Creature *c, bool knownOnly = false);
      virtual int16 DefVal(Creature *c, bool knownOnly = false);
      virtual int16 ParryVal(Creature *c, bool knownOnly = false);
      virtual Dice SDmg()           { Dice d; d.Set(1,3,0);                                   
                                       return d; }
      virtual Dice LDmg()           { Dice d; d.Set(1,1,0);                                   
                                       return d; }
      virtual int16 Penalty(bool for_skill)        { return 0; }
      virtual int16 Size(Creature * wield=NULL) { return TITEM(iID)->Size; } 
      virtual int16 DamageType(Thing * target)      { return AD_BLUNT; }
      virtual int16 Threat(Creature*) { return 20; }
      virtual int16 CritMult(Creature*)        { return 2; }
      virtual int16 RangeInc(Creature*)        { return 0; }
      void VisibleID(Creature *user);
	};


class QItem: public Item
  {
    friend void Player::UseItemMenu(const char*, const char*, int16);
    friend Item* Item::GenItem(uint16 Flags, rID xID, int16 Depth, int8 Luck, ItemGen *Gen);
    public:
      QItem(rID _iID, int16 _Type) : Item(_iID,_Type) {}
      QItem(Item*i) : Item(i) 
        { memcpy(Qualities,((QItem*)i)->Qualities,8);
        KnownQualities = ((QItem*)i)->KnownQualities; }
      virtual bool HasQuality(int16 q) {  if (HasStati(EXTRA_QUALITY,q))
                                            return true;
                                          return Qualities[0] == q ||
                                                   Qualities[1] == q ||
                                                   Qualities[2] == q ||
                                                   Qualities[3] == q ||
                                                   Qualities[4] == q ||
                                                   Qualities[5] == q ||
                                                   Qualities[6] == q ||
                                                   Qualities[7] == q; }
      virtual bool KnownQuality(int16 q)
        { if (HasStati(EXTRA_QUALITY,q))
            return true;                       
          if (q >= 50)
            return HasQuality(q);       
          return (Qualities[0] == q && (KnownQualities & 0x01)) ||
                 (Qualities[1] == q && (KnownQualities & 0x02)) ||
                 (Qualities[2] == q && (KnownQualities & 0x04)) ||
                 (Qualities[3] == q && (KnownQualities & 0x08)) ||
                 (Qualities[4] == q && (KnownQualities & 0x10)) ||
                 (Qualities[5] == q && (KnownQualities & 0x20)) ||
                 (Qualities[6] == q && (KnownQualities & 0x40)) ||
                 (Qualities[7] == q && (KnownQualities & 0x80)); }
      virtual void MakeKnown(uint8 k) { Item::MakeKnown(k);
                                        if (k & KN_MAGIC)
                                          KnownQualities = 0xFF; }
      virtual void SetKnown(uint8 k)  { Item::SetKnown(k);
                                        KnownQualities = 
                                          k & KN_MAGIC ? 0xFF : 0x00; }
      virtual void AddQuality(int8 q, int16 param=0); 
      virtual void RemoveQuality(int8 q); 
      virtual void PurgeAllQualities() ;
      virtual bool QualityOK(int8 q,uint8 lev=200);
      virtual int8 Material();
      virtual bool isQItem() { return true; }
      virtual int32 Weight(bool psych_might = false);
      virtual int16 Hardness(int8 DType);
      virtual int16 GetQuality(int16 i) { return Qualities[i]; }
      virtual int32 GetGroup();
      virtual int16 ItemLevel(bool bounded=true);
      virtual String & Describe(Player *p); 
    protected:
      int8  Qualities[8];
      uint8 KnownQualities;  
    ARCHIVE_CLASS(QItem,Item,r)
    END_ARCHIVE	
  };

class Food : public QItem {
    friend String & Item::Name(int16 Flags);
protected:
    //  Counter in Thing represents age, higher is newer, when it reaches 0, destroy object.
    int16 Eaten;
public:
    virtual void Dump();
    Food(rID iID, int16 _Type = T_FOOD) : QItem(iID, _Type) {
        Eaten = Age = 0;
    }
    String & Describe(Player *p);
    Food(Item*i) : QItem(i) {
        Eaten = ((Food*)i)->Eaten;
        Age = ((Food*)i)->Age;
    }
    virtual int32 Weight(bool psych_might = false);
    EvReturn Event(EventInfo &e);
    EvReturn Eat(EventInfo &e);

    ARCHIVE_CLASS(Food, QItem, r)
    END_ARCHIVE
};

class Corpse: public Food
	{
		public:
			rID mID;
                        uint32 TurnCreated; // ww: how fresh? 
                        int16 LastDiseaseDCCheck;
		public:
      virtual void Dump();
      virtual void SetImage();
      EvReturn Event(EventInfo &e);
      Corpse(Creature *c, int16 _Type=T_CORPSE);
      Corpse(rID iID, int16 _Type) : Food(iID,_Type) {} 
      Corpse(Item*i) : Food(i) { mID = ((Corpse*)i)->mID; }
      virtual String & Name(int16 Flags=0);
      virtual rID GetCorpseType() { return mID; }
      virtual void SetCorpseType(rID _mID) { mID = _mID; SetImage(); }
      virtual int32 Weight(bool psych_might = false);
      virtual bool isFresh();
      virtual bool fullyDecayed();
      virtual int16 noDiseaseDC();
      virtual int16 Size(Creature * wield=NULL)
        { if (!mID) return SZ_MEDIUM;
          int sz = TMON(mID)->Size;
          StatiIterNature(this,TEMPLATE)
            sz = TTEM(S->eID)->Size.Adjust(sz);
          StatiIterEnd(this)
          return sz; }
      EvReturn Eat(EventInfo &e);
      ARCHIVE_CLASS(Corpse,Food,r)
      END_ARCHIVE	
	};


class Container: public QItem
	{
    friend Item* Monster::GetInv(bool first);
    friend Item* Character::GetInv(bool first);
    private:
  	public:
      hObj Contents;
      virtual void Dump();
      Container(rID iID) : QItem(iID,TITEM(iID)->IType) { Contents = 0; }
      Container(Item*i) : QItem(i) { Contents = 0; }
      String & Describe(Player *p);
      void Unlist(Item *);
      void Sort(int comparison(const void *, const void *));
      bool XInsert(Item*);
      EvReturn PickLock(EventInfo &e);
      EvReturn Insert(EventInfo &e, bool force);
      EvReturn TakeOut(EventInfo &e);
      EvReturn DumpOutContents(EventInfo &e); // ww: onto ground
      EvReturn PourInContents(EventInfo &e);  // starting from EItem2
      virtual int32 Weight(bool psych_might = false);
      Item * operator[](int16 i);
      Item * getItem(int16 n) { return (*this)[n]; }
      EvReturn Event(EventInfo &e);
      ARCHIVE_CLASS(Container,QItem,r)
      END_ARCHIVE

  };


class Weapon: public QItem
	{
		public:
			Weapon(rID iID,int16 _Type = T_WEAPON) :
        QItem(iID,_Type) { }
      Weapon(Item*i) : QItem(i) {}
      virtual bool isWeapon() { return true; } 
      virtual void Dump();
			EvReturn Event(EventInfo &e);
			EvReturn Wield(Creature*);
			EvReturn Unwield(Creature*);
      EvReturn QualityDmg(EventInfo &e);
      void QualityDmgSingle(EventInfo &e, int32 q);
      void DoQualityDmgSingle(EventInfo &e, int32 skip = -1);
      virtual bool betterFor(Creature *c, Item *curr);
      virtual bool isGroup(uint32 gr) { return (TITEM(iID)->Group & gr) != 0; }
      virtual bool thrownOnly() { return !(TITEM(iID)->Group & ~(WG_THROWN|WG_EXOTIC|WG_ARCHERY)); }
      virtual bool canFinesse();
      virtual bool QualityOK(int8 q,uint8 lev=200);
      virtual int16 ItemLevel(bool bounded=true);
      virtual int16 ParryVal(Creature *c, bool knownOnly = false);
      virtual Dice SDmg();
      virtual Dice LDmg();
      virtual int16 Size(Creature * wielder);
      virtual int16 DamageType(Thing * target);
      virtual int16 Threat(Creature*);
      virtual int16 CritMult(Creature*); 
      virtual int16 RangeInc(Creature *);
      virtual bool isMagic();
      virtual bool isBaneOf(int16 MType);
      virtual bool isBaneOf(Creature *c);
      virtual void SetBane(int16 _Bane) { Bane = _Bane; }
      virtual int16 GetBane() { return Bane; }
      virtual void RandomBane();
      virtual bool useStrength(); 
      virtual bool useInGrapple(Creature *c); 
      virtual String & Describe(Player *p);
      private:
      int16 Bane;
      ARCHIVE_CLASS(Weapon,QItem,r)
      END_ARCHIVE	

	};

class Coin: public Item
	{
  	public:
      Coin(Item*i) : Item(i) {}
    	Coin(rID iID,int16 _Type = T_COIN) :
        Item(iID,_Type) {}
      Coin(Inventory*i,rID iID);
      EvReturn Event(EventInfo &e);
      ARCHIVE_CLASS(Coin,Item,r)
      END_ARCHIVE	

  };

class Armour: public QItem
	{
		public:
			Armour(rID iID,int16 _Type = T_ARMOUR) :
        QItem(iID,_Type) {}
      Armour(Item*i) : QItem(i) {}
			EvReturn Event(EventInfo &e);
			EvReturn Wear(Creature*);
			EvReturn Remove(Creature*);
      virtual bool isArmour() { return true; } 
      virtual int16 ArmVal(int16 ty, bool knownOnly = false);
      virtual int16 CovVal(Creature *c, bool knownOnly = false);
      virtual int16 DefVal(Creature *c, bool knownOnly = false);
      virtual int16 PenaltyVal(Creature *c, bool for_skill);
      virtual int16 MaxDexBonus(Creature *c);
      virtual bool isGroup(uint32 gr) { return (TITEM(iID)->Group & gr) != 0; }
      virtual bool betterFor(Creature *c, Item *curr);
      virtual bool QualityOK(int8 q,uint8 lev=200);
      virtual bool isMagic();
      virtual String & Describe(Player *p);
      virtual int16 ItemLevel(bool bounded=true);
      ARCHIVE_CLASS(Armour,QItem,r)
      END_ARCHIVE	

	};


struct ItemGen {
  uint8 Prob;
  uint8 Type;
  uint8 Source;
  bool NeverGood;
  uint8 MinLevel;
  uint16 CostMult;
  const char *Prototype;
  bool Flavored;
};

extern ItemGen DungeonItems[],
               MonsterItems[],
               IllusionItems[],
               ChestItems[],
               MageItems[],
               StapleItems[];
