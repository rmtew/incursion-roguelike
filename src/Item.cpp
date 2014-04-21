/* ITEM.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Contains all the member functions for the Item class that
   do not thematically fit elsewhere (GenItem in Encounter.Cpp),
   as well as the member functions for derived classes Weapon,
   Armour, Food, Corpse, etc.

     Item::Item(rID _iID,int8 _Type)
     Item::Item(Item *i)
     Item* Item::Create(rID iID)
     void Item::SetFlavors()
     Item* Item::TakeOne()
     Item* Item::TryStack(Item*i)
     Item* Item::TryStack(Map *m, int16 x, int16 y)
     bool Item::operator==(Item &i)
     void Item::MakeMagical(rID _eID, int16 spe)
     void Item::MakeKnown(uint8 k)
     void Item::VisibleID(Creature *user)
     EvReturn Item::Event(EventInfo &e)
     bool Item::isActivatable()
     bool Item::isEdible(Creature *p)
     int16 Item::Material()
     void Player::IdentByTrial(Item *it, int16 Quality)
     bool Item::allowedSlot(int16 slot)
     bool Item::activeSlot(int16 slot)
     int16 Item::Hardness(int8 DType)
     bool Item::isCorporeal()
     bool Item::isMetallic()
     bool Item::isOrganic()
     EvReturn Item::Damage(EventInfo &e)

     Corpse::Corpse(Creature *c, int8 _Type)
     int32 Corpse::Weight()
     EvReturn Food::Eat(EventInfo &e)
     EvReturn Corpse::Event(EventInfo &e)
     EvReturn Food::Event(EventInfo &e)
     EvReturn Weapon::Event(EventInfo &e)
     EvReturn Creature::LoadCrossbow(EventInfo &e)
     EvReturn Armour::Event(EventInfo &e)
     int16 Armour::ArmVal(int16 typ)
     uint32 Creature::ArmourType()
     Dice Weapon::SDmg()
     Dice Weapon::LDmg()
     bool Item::HasSpell(uint16 sp)
     int16 Item::ItemLevel()
     int16 Weapon::ItemLevel()
     bool Weapon::QualityOK(uint32 q,uint8 lev)
     void Weapon::AddQuality(int8 q)
     int16 Weapon::Material()
     int16 Weapon::Size(bool wield)
     int16 Weapon::Threat(Creature* cr)
     int16 Weapon::CritMult(Creature* cr)
     int16 Weapon::RangeInc()
     int16 Weapon::DamageType()
     int32 Weapon::Weight()
     int16 Weapon::Hardness(int8 DType)
     String Item::Describe(Player *p)
     String Weapon::Describe(Player *p)

*/

#include "Incursion.h"

int16 CorpseNut[] = 
  { 0, 1, 10, 25, 50, 100, 300, 500, 800 };

Item::Item(rID _iID,int8 _Type)
	: Thing(0,_Type)
	{
    ASSERT(TITEM(_iID));
    iID=_iID; Timeout = -1;
    cHP = MaxHP();
		Image=TITEM(iID)->Image;
    eID = 0; Quantity = 1;
    Known = IFlags = 0;
    Inscrip = NULL; Plus = 0;
    GenNum = theGame->ItemGenNum++;
    if (_Type == T_WAND)
      Charges = 30+random(21);
    else if (_Type == T_STAFF)
      Charges = 20 + random(10);
    if (_Type == T_LIGHT)
      Age = TITEM(iID)->u.l.Lifespan;
	}

Item::Item(Item *i)
  : Thing(i->Image,i->Type)
  {
    iID = i->iID;
    Known = i->Known; IFlags = i->IFlags;
    Plus  = i->Plus; Charges = i->Charges;
    GenNum = i->GenNum; Quantity = i->Quantity;
    eID = i->eID; Parent = i->Parent; 
    Flavor = i->Flavor; 
    Inscrip = i->Inscrip;
    Named = i->Named;
    DmgType = i->DmgType;
    cHP = i->cHP;
    Age = i->Age;
    swingCount = i->swingCount;
    eID = i->eID;
    // ww: must also copy over Stati, otherwise when you TakeOne from a
    // pile of poisoned arrows you get a non-poisoned arrow
    i->__Stati.CopyTo(& this->__Stati);
  }
  
void Item::ChangeIID(rID _iID, bool mult) {
    /* QItem / Item sanity */
    Item *it = Item::Create(_iID);
    ASSERT(it->isQItem() == isQItem() ||  it->Type == T_HELMET || it->Type == T_CLOTHES);
    delete it;

    iID   = _iID;  
    cHP   = MaxHP();
    Image = TITEM(iID)->Image;
    Type  = TITEM(iID)->IType;
    if (mult && TITEM(iID)->HasFlag(IT_GROUP))
        Quantity = 10 + random(10);
    if (mult && TITEM(iID)->HasFlag(IT_BGROUP))
        Quantity = 40 + random(40);

    int8 oldQual[8], c, i;
    int16 b;
    c = 0;
    if (!isQItem())
        return;

    b = GetBane();

    for (i=0;i!=100;i++)
        if (HasQuality(i))
            oldQual[c++] = i;
    PurgeAllQualities();
    for(i=0;i!=c;i++)
        if (QualityOK(oldQual[i]))
            AddQuality(oldQual[i]);

    if (b)
        SetBane(b);
}
  
int16 Item::MaxHP()
 { return TITEM(iID)->hp + 
     GetInherantPlus()*10 + (
     (eID && !GetInherantPlus() &&
      Type != T_SCROLL && Type != T_POTION) ?
       TEFF(eID)->Level*5 : 0); }

void Item::Initialize(bool in_play)
  {
    static rID QualList[33];
    int16 i;
    if (TITEM(iID)->HasFlag(IT_ROPE))
      SetQuantity(random(2) ? 20 : 6);
    if (eID)
      {
        if (TEFF(eID)->GetConst(BASE_ITEM))
          ChangeIID(TEFF(eID)->GetConst(BASE_ITEM),true);
        else if (TITEM(iID)->IType == T_WONDER)
          Error("T_WONDER item %s has no BASE_ITEM.",NAME(eID));
        if (TEFF(eID)->GetConst(INITIAL_PLUS))
          Plus = TEFF(eID)->GetConst(INITIAL_PLUS);
        if (TEFF(eID)->GetList(ITEM_QUALITIES,QualList,32))
          {
            for (i=0;i!=32 && QualList[i];i++)
              AddQuality((int16)QualList[i]);
          }
        if (isWeapon() && TEFF(eID)->GetList(BANE_LIST,QualList,32))
          SetBane(-2);
      } 
    TITEM(iID)->PEvent(EV_BIRTH,this,this,iID);
    if (eID)
      TEFF(eID)-> PEvent(EV_BIRTH,this,this,eID);
    
  }

void Item::ReApply()
  {
    Creature *cr;
    if (!(IFlags & IF_WORN))
      return;
    if (IFlags & IF_REAPPLYING)
      return;
    cr = Owner();
    if (!cr)
      return;
    if (cr->isCharacter())
      {
        bool inActSlot = false;
        for (int16 i=0;i!=SL_LAST;i++)
          if (((Character*)cr)->Inv[i] == myHandle)
            if (activeSlot(i))
              inActSlot = true;
        if (!inActSlot)
          return;
      }
      
    IFlags |= IF_REAPPLYING;
    SetSilence();
    RemoveStatiFrom(this);
    if (eID && !HasStati(DISPELLED))
      {
        EventInfo xe;
        xe.Clear();
        xe.isWield = true;
        xe.isItem = true;
        xe.EActor = cr;
        xe.EItem = this;
        xe.eID = eID;
        ReThrow(EV_EFFECT,xe);
      }
    if (isType(T_ARMOUR)) {
      String str;
      for (int16 i=0;i!=8;i++)
        if (GetQuality(i))
          {
            EventInfo xe;
            str = "quality::";
            if (LookupOnly(APreQualNames,GetQuality(i)))
              str += Lookup(APreQualNames,GetQuality(i));
            else
              str += Lookup(APostQualNames,GetQuality(i));
            xe.Clear();
            xe.eID = FIND(str);
            xe.EActor = cr;
            if (!xe.eID)
              continue;
            xe.EActor->RemoveEffStati(xe.eID);
            if (HasStati(DISPELLED))
              continue;
            xe.EItem = this;
            xe.isWield = true;
            xe.isItem = true;  
            xe.efNum = 0;
            ReThrow(EV_EFFECT,xe);
          }
      }
    UnsetSilence();
    IFlags &= ~IF_REAPPLYING;
  }

Item* Item::Create(rID iID)
	{
		Item *it; TItem *ti;
    if (!iID)
      return NULL;
    ti = TITEM(iID);
    
		switch(ti->IType)
			{   
        case T_STATUE:
        case T_FIGURE:               
				case T_CORPSE:
					it=new Corpse(iID, ti->IType);
				 break;
				case T_FOOD:
					it=new Food(iID,ti->IType);
				 break;
				case T_WEAPON:
				case T_BOW:
        case T_MISSILE:
        case T_STAFF:
        	it=new Weapon(iID,ti->IType);
				 break;
        case T_CONTAIN:
        case T_CHEST:
          it = new Container(iID);
         break;
        case T_ARMOUR:
        case T_BOOTS:
        case T_GAUNTLETS:
        case T_SHIELD:
          it = new Armour(iID,ti->IType);
         break;
        case T_AMULET:
        case T_BOOK:
        case T_TOME:
        case T_BRACERS:
        case T_CLOAK:
        case T_CROWN:
        case T_DECK:
        case T_DUST:
        case T_EYES:
        case T_GEM:
        case T_GIRDLE:
        case T_HERB:
        case T_LIGHT:
        case T_MUSH:
        case T_POTION:
        case T_RING:
        case T_ROD:
        case T_SCROLL:
        case T_TOOL:
        case T_WAND:
        case T_COIN:
        case T_WONDER:
        case T_FOUNTAIN:
        case T_SYMBOL:
        case T_CLOTHES:
        case T_HELMET:
          it = new Item(iID,ti->IType);
         break;
				default:
					Fatal("Unimplemented Item Type!");
					it=NULL;
			}
	  ASSERT(it->GetHP() == it->MaxHP());
		return it;
	}


void Game::SetFlavors()
{
  int16 pn,i,j,k,_k,m,c; 
  rID eID, fID;
  Player *pl;

  for (pn=0;p[pn];pn++) {
    pl = oPlayer(p[pn]);
    for (i=0;DungeonItems[i].Type;i++)
      if (DungeonItems[i].Flavored)
        {
          c = 0;
          /* List all the flavours for the item type in Candidates */
          for (m=0;theGame->Modules[m];m++)
            for (j=0;j!=theGame->Modules[m]->szFla;j++)
              if ((theGame->Modules[m]->QFla+j)->IType == DungeonItems[i].Source) {
                Candidates[c++] = theGame->Modules[m]->FlavorID(j);
                if (c >= 2000)
                  Fatal("Too many flavours!");
                }
          if (!c)
            continue;

          /* Scroll through every effect in the game that has the
             given source and assign it a flavour */
          for (m=0;theGame->Modules[m];m++)
            for (j=0;j!=theGame->Modules[m]->szEff;j++) 
              if ((theGame->Modules[m]->QEff+j)->
                HasSource(DungeonItems[i].Source)) {
                  
                  /* Determine the Effect ID */
                  eID = theGame->Modules[m]->EffectID(j);
                  
                  /* Pick a random unused flavour */
                  k = _k = random(c);
                  while (!Candidates[k]) {
                    k = (++k) % c;
                    if (k == _k)
                      break; 
                    }
                  if (!Candidates[k])
                    Error(Format("Insufficient flavours of type %s!",
                      Lookup(AI_CONSTNAMES, DungeonItems[i].Source)));
                  fID = Candidates[k];
                  Candidates[k] = 0;

                  /* Assign the Flavor to the Effect */
                  if (DungeonItems[i].Source == AI_POTION)
                    EFFMEM(eID,pl)->PFlavorID = fID;
                  else
                    EFFMEM(eID,pl)->FlavorID = fID;
                  }
        }
    }
}

Item* Item::TakeOne()
  {
    if (Quantity == 1)
      {
        Remove(false);
        return this;
      }
    else
      {
        Quantity--;
        Item *i;
        switch(Type) {
  				case T_CORPSE:
	  				i=new Corpse(this);
		  		 break;
			  	case T_FOOD:
  					i=new Food(this);
	  			 break;
          case T_BOW:
          case T_MISSILE:
          case T_WEAPON:
            i=new Weapon(this);
            i->SetBane(GetBane()); 
           break;
          case T_CONTAIN:
            i = new Container(this);
           break;
          /*case T_FIGURE:
          case T_STAFF:
            it = new Figure(x,y,_m,iID,ti);
           break;*/
          case T_COIN:
            i = new Coin(this); 
           break;

          case T_ARMOUR:
          case T_BOOTS:
          case T_GAUNTLETS:
          case T_SHIELD:
            i = new Armour(this);
           break;
          case T_CLOTHES:
          case T_HELMET:
          case T_AMULET:
          case T_BOOK:
          case T_BRACERS:
          case T_CLOAK:
          case T_CROWN:
          case T_DECK:
          case T_DUST:
          case T_EYES:
          case T_GEM:
          case T_GIRDLE:
          case T_HERB:
          case T_LIGHT:
          case T_MUSH:
          case T_POTION:
          case T_RING:
          case T_ROD:
          case T_SCROLL:
          case T_STAFF:
          case T_TOOL:
          case T_TOME:
          case T_WAND:
          case T_FOUNTAIN:
          case T_SYMBOL:
            i = new Item(this);
           break;
			  	default:
				  	Error("Unimplemented Item Type in Item::TakeOne!");
				  	return this;
        }

        i->Quantity = 1;
        i->Parent = 0;
        return i;
      }
  }

Item* Item::TryStack(Item*i)
  {
    if (*this == *i)
      //if (TITEM(iID)->HasFlag(IF_STACKABLE))
        {
          Quantity += i->Quantity;
          Known |= i->Known;
          homeID = i->homeID;
          
          if (GetStatiMag(POISONED) != i->GetStatiMag(POISONED))
            {
              int16 chance;
              chance = random(Quantity);
              if (i->Quantity > chance)
                SetStatiMag(POISONED,-1,NULL,i->GetStatiMag(POISONED));
            }
          i->Remove(true);
          return this;
        }
    return NULL;
  }

Item* Item::TryStack(Map *m, int16 x, int16 y)
  {
    Item *newp; int16 i; Thing *t;
    MapIterate(m,t,i)
    if (t->x == x)
      if (t->y == y)
        if (t != this)
          if (t->isItem())
            if (newp = ((Item*)t)->TryStack(this))
              return newp;
    return this;
  }

bool Item::operator==(Item &i)
  {
    bool easyStack;
    easyStack = false;
    if (Type != i.Type)
      return false;

    switch (Type)
      {
        case T_COIN:
          goto VeryEasyStack;
        case T_CONTAIN:
        case T_WAND:
          /* Never stack containers, due to contents */
          // ww: never stack wands unless we add code to look at the charges
          return false;
        case T_MISSILE:
          easyStack = true;
        case T_WEAPON:
        case T_BOW:
        case T_ARMOUR:
          for (int16 c=0;c!=8;c++)
            if (GetQuality(c) != i.GetQuality(c))
              return false;
         break;
        case T_TOOL:
        case T_FOOD:
        case T_POTION:
        case T_SCROLL:
          easyStack = true;
         break;
        case T_CORPSE:
          if (GetCorpseType() != i.GetCorpseType())
            return false;
         break;
      }
    
    /* Anything that doesn't easy-stack, or was generated in
       different groups initially, must be identified in order
       to stack. */
    if (GenNum != i.GenNum && !easyStack) { 
      if ((Known & i.Known & (KN_MAGIC|KN_PLUS|KN_BLESS)) != 
                             (KN_MAGIC|KN_PLUS|KN_BLESS))
        return false;
      }
    
    if (!easyStack) {
      if (Named.GetLength() && i.Named.GetLength())
        if (stricmp(Named,i.Named))
          return false;
      }

    static const uint8 exceptFor[] = { 
      POISONED,
      SUMMONED,
      BOOST_PLUS,
      EXTRA_QUALITY,
      HOME_REGION,
      0
    } ;

    if (!hasSameStati(&i,exceptFor))
      return false; 

    if (HasStati(POISONED) || i.HasStati(POISONED))
      {
        StatiIterNature(this, POISONED)
          if (!i.HasEffStati(POISONED,S->eID))
            StatiIterBreakout(this,return false);
          if (i.GetEffStatiMag(POISONED,S->eID,S->Val) != S->Mag)
            if (!theGame->Opt(OPT_POISON_MERGE))
              StatiIterBreakout(this,return false);
        StatiIterEnd(this)
        StatiIterNature((&i), POISONED)
          if (!HasEffStati(POISONED,S->eID))
            StatiIterBreakout((&i),return false);
          if (GetEffStatiMag(POISONED,S->eID,S->Val) != S->Mag)
            if (!theGame->Opt(OPT_POISON_MERGE))
              StatiIterBreakout((&i),return false);
        StatiIterEnd((&i))
        
      }
      
    if (isType(T_LIGHT) && i.isType(T_LIGHT))
      if (Age != i.Age)
        return false;

    // ww: don't check for Known again here -- we already did it above 
    VeryEasyStack:
    if (((IFlags|IF_WORN|IF_INTUITED|IF_DECIPHERED) == 
       (i.IFlags|IF_WORN|IF_INTUITED|IF_DECIPHERED)) &&
      (Plus == i.Plus) &&
      (cHP == i.cHP) &&
      (Flavor == i.Flavor) &&
      (iID == i.iID) &&
      (eID == i.eID)) {
        /* ww: merge inscriptions, handle Intuition. Without something like
         * this, after you sleep all of your healing potions are "pasty
         * potions {weak}" and all new pasty potions you pick up fail to
         * merge */

        if ((bool)Inscrip && !((bool)i.Inscrip))
          i.Inscrip = Inscrip;
        else if ((bool)i.Inscrip)
          Inscrip = i.Inscrip;
        return true; 

      } 
    return false; 
  }

void Item::MakeMagical(rID _eID, int16 spe)
	{
	
    if (_eID > 0) {
      int32 dmg;
      dmg = (GetHP() * 1000) / MaxHP();
		  eID = _eID;
		  cHP = (dmg * MaxHP()) / 1000;
      if (TEFF(eID)->HasFlag(EF_CURSED))
        IFlags |= IF_CURSED;
      if (TEFF(eID)->HasFlag(EF_HALF_CURSED))
        if (random(100) < 50)
          IFlags |= IF_CURSED;
      }
    if (spe > 0) 
      SetInherantPlus(spe);
    
    if (eID) {
      EffMem *em = EFFMEM(eID,theGame->GetPlayer(0));
      if (isType(T_POTION)) {
        if (em->PFlavorID)
          if (TFLA(em->PFlavorID)->Color != -1)
            Image = (Image & 0xF0FF) | 
              TFLA(em->PFlavorID)->Color << 8;
        }
      else {
        if (em->PFlavorID)
          if (TFLA(em->PFlavorID)->Color != -1)
            Image = (Image & 0xF0FF) | 
              TFLA(em->PFlavorID)->Color << 8;
        }
      }

	}

void Item::MakeKnown(uint8 k)
  {
    Known |= k;
    if ((Known & KN_MAGIC) && (Known & KN_PLUS))
      Inscrip.Empty();

    if (k & KN_MAGIC) {
      if (Type == T_SCROLL || Type == T_WAND   || Type == T_RING ||
          Type == T_GAUNTLETS || Type == T_GIRDLE || Type == T_CROWN ||
          Type == T_HELMET || Type == T_TOME || Type == T_CLOAK ||
          Type == T_BOOTS || Type == T_ROD || Type == T_STAFF)
        if (eID)
          EFFMEM(eID,theGame->GetPlayer(0))->Known = true;
      if (Type == T_POTION)
        if (eID)
          EFFMEM(eID,theGame->GetPlayer(0))->PKnown = true;
      }  
    if (theGame->GetPlayer(0))
      if (theGame->GetPlayer(0)->Percieves(this))
        theGame->GetPlayer(0)->JournalNoteItem(this);
  }


void Item::VisibleID(Creature *user)
  {
    int16 i; uint16 v; Player *p;
    ASSERT(user && this);
    // ww: This seems to fire when the player is nowhere nearby ... 
    // commenting it out for now. 
    for (i=0;i!=MAX_PLAYERS && (p = theGame->GetPlayer(i));i++) {
      if (p->x == -1)
        continue;
      v = p->Percieves(user);
      /*
        This seems to be the root of the "This must be a
          Potion of Fire Breath" messages that appear
          intermittedly with no discernable reason.
      
      if (!(Flags & F_DELETE))
        v |= p->Percieves(this);*/
        
      // ww: parens are really needed here ...
      if ((v & PER_DETECT) || (v & ~PER_SHADOW))
        p->IdentByTrial(this);
      }
  }
  
rID Item::GetCurrentRegion()
  {
    if (m)
      return m->RegionAt(x,y);
    if (Parent)
      {
        if (oThing(Parent)->isItem())
          return oItem(Parent)->GetCurrentRegion();
        if (oThing(Parent)->isCreature())
          if (oCreature(Parent)->m)
            return oCreature(Parent)->m->RegionAt(
              oCreature(Parent)->x, oCreature(Parent)->y);
     }
   return 0;
 }
      

EvReturn Item::Event(EventInfo &e)
{
  EvReturn res; static String str;

  if (eID && eID != e.eID) {
    e.Event = EITEM(e.Event);
    res = TEFF(eID)->Event(e,eID);
    e.Event -= EITEM(0);
    if (res == DONE || res == ERROR)
      return res;
    if (res == NOMSG)
      e.Terse = true;
  }
  res = TITEM(iID)->Event(e,iID);
  if (res != NOTHING)
    return res;
  if (res == NOMSG)
    e.Terse = true;

  switch(e.Event)
  {
    case EV_PICKLOCK:
      if (e.EItem == this)
        if (isType(T_CONTAIN) || isType(T_CHEST))
          return ((Container*)this)->PickLock(e);
      break;
    case EV_DAMAGE:
      if (e.ETarget == this)
        return Damage(e);
      break;
    case EV_EFFECT:
      res = MagicEvent(e);
      return res;
    case EV_MAGIC_HIT:
      return MagicHit(e);
    case EV_LOSEITEM:
    case EV_REMOVE:
        if (isCursed() && e.Event == EV_REMOVE)
          break;
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
            int16 q, i;
            for (i=0;i!=8;i++)
              if (q = GetQuality(i))
                {
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
      return NOTHING;
    case EV_PLACE:
    case POST(EV_LOSEITEM):
      IFlags &= ~IF_PROPERTY;
      return NOTHING;
    case EV_ZAP:
      if (Type == T_WAND)
        return ZapWand(e);
      if (Type == T_STAFF)
        return UseStaff(e);
      e.EActor->IPrint("That can't be zapped.");
      return ABORT;
    case EV_DRINK:
      if (Type == T_POTION)
        return DrinkPotion(e);
      if (Type == T_FOUNTAIN)
        return ReThrow(EV_EFFECT, e);
      e.EActor->IPrint("That can't be drank.");
      return ABORT;
    case EV_READ:
      if (Type == T_SCROLL)
        return ReadScroll(e);
      if (Type == T_BOOK)
      {
        /*
           if (yn("Do you want to learn new spells?",false))
           {
           thisp->LearnSpells(e.EItem);
           return DONE;
           }
           Examine(e.EItem);
         */
        return DONE;
      }
      e.EActor->IPrint("That's not readable.");
      return ABORT;
    case EV_ACTIVATE:
      if (e.EActor->inField(FI_SILENCE))
      { e.EActor->IPrint("You can't say the command word in this silence!");
        return ABORT; }
        if (isActivatable() || e.eID)
          return Activate(e);
        e.EActor->IPrint("That can't be activated.");
        return ABORT;
    case POST(EV_EFFECT):
        if (eID && TEFF(eID)->HasFlag(EF_USERID))
          e.EActor->IdentByTrial(this);
        if (eID && TEFF(eID)->HasFlag(EF_AUTOID))
          VisibleID(e.EActor);
        if (eID && e.EActor->isPlayer() && !(Known & KN_MAGIC))
        {
          /* Single-Player Kludge */
          if (isType(T_POTION))
            EFFMEM(eID,theGame->GetPlayer(0))->PTried = true;
          else
            EFFMEM(eID,theGame->GetPlayer(0))->Tried = true;
          if (!Inscrip.GetLength())
            Inscrip = "tried";
        }
        return NOTHING;   
    case POST(EV_MAGIC_HIT):
        if (eID && TEFF(eID)->HasFlag(EF_STRIKEID))
          if (!e.Resist && !e.Immune)
            VisibleID(e.EActor);
        break;                                  
    case POST(EV_REMOVE):
      swingCount = 0;
      return NOTHING;
    case EV_STRIKE:
      if (e.ETarget == this) {
        bool is_acting = e.EActor->HasStati(ACTING);
        if (isCursed() && (IFlags & IF_WORN) && Owner() == e.EActor)
          {
            IPrint("Strangely, you have no desire to damage that item.");
            MakeKnown(KN_CURSE);
            e.EActor->RemoveStati(ACTING);
            return DONE;
          }
        if (e.AType == A_KICK) {
          DPrint(e,is_acting ? "WHAMM!" : "You kick the <Obj>.",
                    "The <EActor> kicks the <Obj>.",this);
        }
        else if (e.eID) {
          String bname;
          bname = TEFF(e.eID)->GetMessages(MSG_BLASTNAME)[0];
          if (!bname.GetLength())
            bname = "blast";
          DPrint(e,"Your <str> strikes the <ETarget>.",
                   "The <EActor>'s <str> strikes the <ETarget>.",
                   (const char*) bname);
          } 
        else if (e.EItem)
          DPrint(e,is_acting ? "THWACK!" : "You strike the <ETarget> with your <EItem>.",
              "The <EActor> strikes the <ETarget> with his <EItem>.");
        else
          DPrint(e, is_acting ? "RRRACK!" : "You <Str> the <ETarget>.",
              "The <EActor> <Str2> the <ETarget>.", 
              Lookup(AttkVerbs1,e.AType), 
              Lookup(AttkVerbs2,e.AType));

        e.vDmg = e.Dmg.Roll();
        if (e.EActor->HasFeat(FT_SUNDER)) {
          e.vDmg *= 2; 
          e.strDmg += " (x2 Sunder)"; 
        }
        if (e.EItem && e.EItem->isWeapon() && 
            ((Weapon *)e.EItem)->HasQuality(WQ_SUNDERING)) {
          e.vDmg *= 2; 
          e.strDmg += " (x2 Sundering)"; 
        } 
        ReThrow(EV_DAMAGE,e);
        if (e.EItem2 && e.EItem2->isWeapon())  
          ((Weapon *)e.EItem2)->DoQualityDmgSingle(e);
        if (e.EItem && e.EItem->isWeapon())  
          ((Weapon *)e.EItem)->DoQualityDmgSingle(e);
        e.EActor->MakeNoise(max(10 - (e.EActor->SkillLevel(SK_MOVE_SIL) / 2),1));
        if (cHP <= 0) {
          e.Died = true; 
          e.EActor->RemoveStati(ACTING); 
        } else {
          if (e.EActor->isPlayer() && e.EPActor->Opt(OPT_REPEAT_KICK) &&
               (e.AType == A_KICK || e.AType == A_SWNG) && !e.EActor->GetStatiEID(ACTING)) {
            if (!e.EActor->HasStati(ACTING)) {
              e.EActor->GainPermStati(ACTING,this,SS_MISC,EV_SATTACK,20);
            } else {
              int16 mag = e.EActor->GetStatiMag(ACTING);
              if (mag <= 0) {
                e.EActor->HaltAction("not broken after 20 attacks", false); 
                if (e.EActor->HasStati(ACTING)) {
                  e.EActor->RemoveStati(ACTING); 
                  e.EActor->GainPermStati(ACTING,this,SS_MISC,EV_SATTACK,20);
                } else return ABORT;
              } else {
                e.EActor->SetStatiMag(ACTING,-1,NULL,mag-1);
              } 
            }
          } 
          
        } 
        return DONE;
      } else return NOTHING; 

    default:
        return NOTHING;
  }
  return NOTHING; 
}

bool Item::isActivatable()
  {
    int16 i=0;
    if (TITEM(iID)->HasFlag(IT_ACTIVATE))
      return true;
    if (!eID) 
      return false;
    if (!(Known & KN_MAGIC))
      return false;
    TEffect *te = TEFF(eID);
    return te->HasFlag(EF_ACTIVATE) ||
           te->HasFlag(EF_ACTIVATE2) ||
           te->HasFlag(EF_ACTIVATE3) ||
           te->HasFlag(EF_ACTIVATE4) ||
           te->HasFlag(EF_ACTIVATE5);

  }

bool Item::isEdible(Creature *p)
  {
    return (Type == T_FOOD || Type == T_CORPSE || HasIFlag(IT_EDIBLE));
  }

int16 Item::GetLightRange()
  {
    if (Type != T_LIGHT)
      return 0;
    if (Age == 0)
      return 0;
    return TITEM(iID)->u.l.LightRange;
  }

void Player::IdentByTrial(Item *it, int16 Quality)
  {
    if (!it || !it->isItem())
      return;
    if (it->isKnown(KN_MAGIC))
      return;
    it->MakeKnown(KN_MAGIC);
    /* We use "<str>" here because the scroll has already
       been removed from the player's inventory, thus <Obj>
       would produce "This must be something!" */
    if (TITEM(it->iID)->HasFlag(IT_PLURAL))
      IPrint("These must be <Str>!",(const char*)it->Name());
    else
      IPrint("This must be <Str>!",(const char*)it->Name(NA_A));
    Exercise(A_INT,random(6)+max(1,it->ItemLevel()/2),EINT_IDENT,60);
  }

bool Item::allowedSlot(int16 slot, Creature *me)
  {
    int16 Isz = Size(me);
    int16 Msz = me->Attr[A_SIZ];
    // ww: massive overhaul here -- if your size was not Medium, these
    // hard-coded values didn't make much sense
    if (Isz > Msz + 1 && slot != SL_LSHOULDER && slot != SL_RSHOULDER) 
      return false; 
    // medium man can put large longbow on shoulder, or large tower shield
    // in both hands, but that's the limit!
    switch(slot)
      {
        case SL_INAIR:
        case SL_READY:
        case SL_WEAPON:
          return true;
        case SL_LIGHT:
          return Type == T_LIGHT;
        case SL_LSHOULDER:
        case SL_RSHOULDER:
          /* I'm not sure what the point of all of this was -- it's clear
           * that you can sling a sword or a bow over your shoulder, and
           * that seems to be what you were going for, but if I'm wearing a
           * sash or a harness it's no problem to put a small knife or
           * stone on my shoulder either. Heck, I've had babies on my
           * shoulders. */
          return true; 
        case SL_BELT:
          return Type == T_GIRDLE;
        case SL_EYES:
          return Type == T_EYES;
        case SL_ARMOUR:
          return Type == T_ARMOUR;
        case SL_CLOTHES:
          return Type == T_CLOTHES;
        case SL_BOOTS:
          return Type == T_BOOTS;
        case SL_CLOAK:
          return Type == T_CLOAK;
        case SL_LRING:
        case SL_RRING:
          return Type == T_RING;
        case SL_AMULET:
          return Type == T_AMULET ||
                 Type == T_SYMBOL;
        case SL_GAUNTLETS:
          return Type == T_GAUNTLETS;
        case SL_HELM:
          return Type == T_HELMET || Type == T_CROWN;
        case SL_BRACERS:
          return Type == T_BRACERS;
        case SL_BELT1:
        case SL_BELT2:
        case SL_BELT3:
        case SL_BELT4:
        case SL_BELT5:
          return (Isz <= SZ_SMALL) || (Isz <= Msz - 1);
          // ww: a giant can put bigger things on its belt!
        case SL_PACK:
          // ww: this used to be medium, but that ruled out the kobold
          // backpack
          return (Type == T_CONTAIN) && (Isz <= Msz);
        default:
          Error("Unknown slot in Item::allowedSlots");
          return false;
      }

  }

bool Item::activeSlot(int16 slot)
  {
    /* Stones are active in any slot. */
    if (Type == T_TOOL || Type == T_GEM)
      if (eID && TEFF(eID)->HasSource(AI_STONE))
        return true;
    


    switch (slot)
      {
        case SL_READY:
        case SL_WEAPON:
          return Type == T_ROD || Type == T_STAFF ||
            Type == T_WEAPON || Type == T_SHIELD ||
            Type == T_BOW || Type == T_SYMBOL;
        case SL_LIGHT:
          return Type == T_LIGHT;
        case SL_BELT:
          return Type == T_GIRDLE;
        case SL_EYES:
          return Type == T_EYES;
        case SL_CLOTHES:
          return Type == T_CLOTHES;
        case SL_ARMOUR:
          return Type == T_ARMOUR;
        case SL_BOOTS:
          return Type == T_BOOTS;
        case SL_CLOAK:
          return Type == T_CLOAK;
        case SL_LRING:
        case SL_RRING:
          return Type == T_RING;
        case SL_AMULET:
          return Type == T_AMULET || 
                 Type == T_SYMBOL;
        case SL_GAUNTLETS:
          return Type == T_GAUNTLETS;
        case SL_HELM:
          return Type == T_CROWN || Type == T_HELMET;
        case SL_BRACERS:
          return Type == T_BRACERS;
        case SL_ARCHERY:
          return Type == T_BOW;
        default:
          return false;
      }
  }

int16 MaterialHardness(int8 mat, int8 DType)
{
  if (mat == MAT_INDESTRUCTABLE_STONE)
    return -1;
  else if ((DType >= AD_DREX && DType <= AD_TLPT) || 
      (DType >= AD_CURS && DType <= AD_DALU) || 
      DType == AD_PSYC || DType == AD_SLEE || DType == AD_STUN || 
      DType == AD_TOXI || DType == AD_POLY || DType == AD_CHRM || 
      DType == AD_BLND || DType == AD_SLOW || DType == AD_CONF || 
      DType == AD_DISE || DType == AD_FEAR || DType == AD_SUBD || 
      DType == AD_WERE || DType == AD_BLEE || DType == AD_POIS || 
      DType == AD_KNOC)
    return -1;
  else switch (DType) {

    case AD_RUST: // copper, silver, gold, platinum == no rust
      return (mat == MAT_IRON || mat == MAT_WROUGHT ||
          mat == MAT_METAL) ? 0 : -1; 


    case AD_SOAK: 
      switch (mat) {
        case MAT_ICE: return 5; 
        case MAT_OIL: case MAT_LIQUID: case MAT_MUD: return 0; 
        case MAT_PAPER: return 5;
        case MAT_GLASS: return 20;
        default: return -1; 
      } 

    case AD_DCAY: // hurts organices
      if (!MaterialIsOrganic(mat))
        return -1;
      else
        return 0; 
      break; 

    case AD_NECR: 
      if (!MaterialIsOrganic(mat))
        return -1;
      break;

    case AD_MAGC:
    case AD_DISN:
      return (mat == MAT_ADAMANT) ? 20 : 0;

    case AD_SUNL:
      if (mat != MAT_WAX)
        return -1;
      break; 

    case AD_ACID: 
      if (mat == MAT_GLASS || mat == MAT_GEMSTONE)
        return -1;
      break; 

    case AD_SONI: 
      if (mat == MAT_GLASS) 
        return 0;
      else if (MaterialIsOrganic(mat))
        return -1; 
      break; 
  } // end: switch (Dtype)

  switch (mat) {
    case MAT_PAPER:
      if (DType == AD_FIRE)
        return 0;
      else if (DType == AD_SLASH || DType == AD_SOAK)
        return 5;
      return -1;
    case MAT_WAX:
      if (DType == AD_FIRE)
        return 0;
      return 3;
    case MAT_LIQUID:
      if (DType == AD_COLD)
        return 3;
      return 10;
    case MAT_WEBBING:
      return (DType == AD_FIRE) ? 0 : 10;

    case MAT_CLOTH:
      return (DType == AD_FIRE) ? 3 : 5;
    case MAT_VEGGY:
    case MAT_FLESH:
      return 5;
    case MAT_BONE:
      return 6; 
    case MAT_LEATHER:
      return 10;
    case MAT_DRAGON_HIDE:
      return 15;

    case MAT_WOOD: // SRD: wood has 5 hardness
      return 5;

    case MAT_SILVER:
    case MAT_PLATINUM:
    case MAT_GOLD:
      return 5;
    case MAT_COPPER:
      return 7;
    case MAT_METAL:
      return 8;
    case MAT_IRONWOOD: 
    case MAT_IRON:
      return 10;
    case MAT_MITHRIL:
      return 15;
    case MAT_ADAMANT:
      return 20;
    case MAT_DARKWOOD:
      return (DType == AD_FIRE) ? 20 : 15;

    case MAT_GLASS:
      return (DType == AD_BLUNT || DType == AD_SONI) ? 0 : 5;
    case MAT_GEMSTONE:
      return (DType == AD_BLUNT || DType == AD_SONI) ? 5 : 10;
    case MAT_ICE:
      return (DType == AD_FIRE) ? 3 : 7;

    case MAT_MAGMA:
      return (DType == AD_COLD) ? 5 : -1;
    case MAT_MINERAL:
      return 5;
    case MAT_QUARTZ:
      return 7;
    case MAT_GRANITE:
      return 10;

    case MAT_FORCE:
      if (DType == AD_MAGC) return -1;
      return 20;
    case MAT_EMPTYNESS:
      return -1;
  }
  return -1; // ww: paranoia
} 

int16 Item::Hardness(int8 DType)
{
  return MaterialHardness(Material(),DType);
}

bool MaterialIsMetallic(int8 mat)
{
    return (mat == MAT_IRON || mat == MAT_COPPER || mat == MAT_GOLD ||
            mat == MAT_SILVER || mat == MAT_METAL || mat == MAT_MITHRIL ||
            mat == MAT_ADAMANT || mat == MAT_PLATINUM); 
}

bool MaterialIsWooden(int8 mat) 
{ 
  return (mat == MAT_WOOD || mat == MAT_IRONWOOD || mat == MAT_DARKWOOD);
} 

bool MaterialIsOrganic(int8 mat)
{
  return (mat == MAT_VEGGY || mat == MAT_WAX || mat == MAT_FLESH ||
      mat == MAT_CLOTH || mat == MAT_LEATHER || mat == MAT_PAPER ||
      mat == MAT_BONE || mat == MAT_DRAGON_HIDE ||
      mat == MAT_WEBBING || MaterialIsWooden(mat));
}

bool MaterialIsCorporeal(int8 mat)
{ 
  return  (mat != MAT_EMPTYNESS && mat != MAT_FORCE); 
} 

bool Thing::isMetallic()
{ 
  return MaterialIsMetallic(Material()); 
} 

bool Thing::isWooden()
{ 
  return MaterialIsWooden(Material());
}

bool Thing::isOrganic()
{ 
  return MaterialIsOrganic(Material());
}

bool Thing::isCorporeal()
{ 
  return MaterialIsCorporeal(Material());
}

bool Thing::isGhostTouch()
{ 
  return (!isCorporeal());
} 

bool Item::isGhostTouch()
{ 
  return (!isCorporeal() || (IFlags & IF_GHOST_TOUCH));
} 


EvReturn Item::Damage(EventInfo &e)
{
  int16 hard, dcount, avg, dmg, i, ox, oy; Map *om;
  const char *damage, *destroy, *name, *oname;
  String pre, s;
  int posthard;
  bool plural = false, destroyed = false;

  if (!isCorporeal())
    return DONE;
  
  bool no_msg;
  no_msg = false;
  if (e.EActor == Owner())
    if (e.EActor->GetStatiObj(ACTING) == this)
      no_msg = true;
    
  if (isType(T_WEAPON))
    if (HasQuality(WQ_ENERGY))
      return DONE;

  ox = x;
  oy = y;
  om = m;

  Creature * owner = Owner();

  if (owner) {
    if (e.DType == AD_SOAK || e.DType == AD_RUST ||
        e.DType == AD_DCAY)
      if (owner->ResistLevel(e.DType) == -1)
        return DONE; 
  }

  hard = Hardness(e.DType);

  if (owner)
    if ((e.DType != AD_SLASH && e.DType != AD_BLUNT &&
         e.DType != AD_PIERCE) || (owner->InSlot(SL_ARMOUR) &&
         (owner->InSlot(SL_AMULET) == this ||
          owner->InSlot(SL_BRACERS) == this ||
          owner->InSlot(SL_BELT) == this)))
      {
        if (owner->ResistLevel(e.DType) == -1)
          return DONE;
        hard += owner->ResistLevel(e.DType);
      }

  if (e.ignoreHardness == true)
    hard = 0;
  else if (e.halfHardness == true)
    hard /= 2;

  posthard = e.vDmg - hard; 

  /* If you are attacking your own items, ignore their hardness. */
  if ((e.EActor == owner) && !e.isTrap && !e.eID)
    { posthard = e.vDmg; hard = 0; }

  /*
  if (owner && owner->HasFeat(FT_SIGNATURE_ITEMS)) {
    if (posthard <= 1 || owner->SavingThrow(REF,posthard)) {
      owner->IDPrint((const char*)Format("You avoid harm to your %s.",
            (const char*)Name()),
          (const char*)Format("%s avoids harm to %s.",
                         (const char*)owner->Name(NA_THE),
                         (const char*)Name(NA_THE)));
      return DONE; 
    } 
    owner->IDPrint((const char*)Format("You reduce the damage to your %s.",
          (const char*)Name()),
        (const char*)Format("%s reduces the damage to %s.",
                       (const char*)owner->Name(NA_THE),
                       (const char*)Name(NA_THE)));
    posthard /= 2; 
  } 
  */

  if ((e.EActor && e.EActor->isPlayer()) || (owner && owner->isPlayer()))
    {
      s = Format("%c%s:%c %s%s %s",-13,(const char*)(Name(0).Capitalize()),
          -7,
          (e.Dmg.Number == 0 && e.Dmg.Sides == 0 && e.Dmg.Bonus == 0) ?
            "Residual" : (const char*)e.Dmg.Str(),
                e.strDmg ? (const char*)e.strDmg : "",
          Lookup(DTypeNames,e.DType));

      //if (owner && owner->HasFeat(FT_SIGNATURE_ITEMS)) 
      //  s += " / 2"; 

      s += Format(" = %d",e.vDmg);

      if (hard == -1) 
        s += Format(" vs. Immune (%s)",(const char*)Lookup(MaterialDescs,Material()));
      else 
        s += Format(" vs. %d (%s)",hard,(const char*)Lookup(MaterialDescs,Material()));

      e.aDmg = posthard; 

      s += Format(" <%d>[%s]<7>",
          (e.vDmg > hard) && (hard != -1) ? PINK : EMERALD,
          (e.vDmg > hard) && (hard != -1) ? (const char*)Format("%d damage",e.aDmg) : "unhurt");

      if (e.EActor && e.EActor->isPlayer())
        {
          e.EPActor->MyTerm->SetWin(WIN_NUMBERS);
          e.EPActor->MyTerm->Clear();
          e.EPActor->MyTerm->Write(0,0,XPrint(s));
        }
      else if (owner && owner->isPlayer())
        { 
          ((Player*)owner)->MyTerm->SetWin(WIN_NUMBERS);
          ((Player*)owner)->MyTerm->Clear();
          ((Player*)owner)->MyTerm->Write(0,0,XPrint(s));
        }
    }

  if (hard >= e.vDmg || hard == -1)
    return DONE;

  int totalHP = cHP + ((Quantity-1)*MaxHP()); 

  ASSERT(Quantity >= 1); 
  ASSERT(MaxHP()); 

  totalHP -= e.aDmg; 

  if (totalHP <= 0) {
    dcount = Quantity; 
  } else if (Quantity == 1) {
    if (totalHP < 1) {
      cHP = 0;
      dcount = 1;
    } else {
      cHP = totalHP;
      dcount = 0; 
    } 
  } else { 
    int oldQuantity = Quantity; 
    Quantity = (totalHP + (MaxHP()-1))/ MaxHP(); 
    cHP = totalHP - (Quantity * MaxHP());
    if (cHP <= 0) cHP = MaxHP(); 
    dcount = oldQuantity - Quantity; 
  } 

  if (dcount == 0) { 
    destroyed = false; plural = Quantity > 1; pre = ""; 
  } else if (dcount == Quantity) { 
    destroyed = true; plural = dcount > 1; pre = ""; 
  } else { 
    pre = XPrint("<Num> of ",dcount); 
    destroyed = true; plural = dcount > 1; 
  }

    damage = plural ? "are damaged" : "is damaged";
    destroy = plural ? "are destroyed" : "is destroyed";

    if (e.DType == AD_SONI || isType(T_POTION))
      {
        damage = plural ? "crack" : "cracks";
        destroy = plural ? "shatter" : "shatters";
      }

    if (e.DType == AD_FIRE)
      {
        if (isType(T_POTION))
          {
            damage = plural ? "boil" : "boils";
            destroy = plural ? "boil and explode" : "boils and explodes";
          }
        else if (isMetallic()) {
          damage = plural ? "melt" : "melts";
          destroy = plural ? "melt into slag" : "melts into slag";
          }
        else {
          damage = plural ? "burn" : "burns";
          destroy = plural ? "burn up" : "burns up";
          }
      }

    if (e.DType == AD_ACID)
      {                
        damage = plural ? "melt" : "melts";
        destroy = plural ? "melt away" : "melts away";
      }

    if (e.DType == AD_DISN)
      destroy = plural ? "disintegrate" : "disintegrates";

    if (e.DType == AD_NECR)
      {
        damage = plural ? "wither" : "withers";
        destroy = plural ? "wither away" : "withers away";
      }

    if (e.DType == AD_RUST)
      {
        damage = plural ? "rust" : "rusts";
        destroy = plural ? "rust away completely" : "rusts away completely";
      }

    /* Get the item name, but cut off the initial number */
    oname = name = strdup(Name());
    while(isdigit(name[0]) || name[0] == ' ')
      name++;

    if (destroyed)
      no_msg = false;

    if (Owner())
      Owner()->IDPrint(no_msg ? NULL : "<Str1>your <Str3> <Str4>.",
                       "<Str1>the <Obj2>'s <Str3> <Str4>.",
                        (const char*)pre,Owner(),name,
                        destroyed ? destroy : damage);
    else
      IDPrint(NULL,"<Str>the <Str> <Str>.",
                   (const char*)pre,name,
                   destroyed ? destroy : damage);

    free((void*)oname);
                       
  if (dcount == Quantity) {

    if ((isType(T_CONTAIN) || isType(T_CHEST)) && 
        ((Container*)this)->Contents) {
      hObj h = ((Container *)this)->Contents;
      int dmg = Hardness(e.DType) * 3 / 2; 
      if (h) {
        e.ETarget = this; 
        if (e.EActor)
          e.EActor->IPrint("The <hObj>'s contents are damaged and spill out.",myHandle);
      } 
      while (h) {
        Item *it = oItem(h);
        it->Remove(false);
        it->PlaceAt(owner ? owner->m : om,
                    owner ? owner->x : ox,
                    owner ? owner->y : oy);
        if (dmg > 0) { 
          EventInfo eCopy = e;
          eCopy.Dmg.Number = eCopy.Dmg.Sides = eCopy.Dmg.Bonus = 0; 
          eCopy.ETarget = it; 
          eCopy.vDmg = dmg; 
          ReThrow(EV_DAMAGE,eCopy);
        }
        h = ((Container *)this)->Contents; 
      }
    } 

    e.Died = true; 
    
    /* Animated Object Crash Prevention */
    for (i=0;backRefs[i];i++)
      if (isValidHandle(backRefs[i]))
        if (oThing(backRefs[i])->HasStati(TRANSFORMED,-1,this))
          {
            Flags |= F_DELETE;
            oThing(backRefs[i])->RemoveStati(TRANSFORMED);
            Flags &= ~F_DELETE;
            break;
          }
    Remove(true);
  } 

    if (DmgType == 0)
      DmgType = e.DType;
    else if (DmgType == e.DType)
      ;
    else
      DmgType = -1;

    if (e.Died && e.EActor && e.EActor->isCharacter())
      if (e.EActor->HasAbility(CA_SPELLBREAKER))
        {
          int32 Percent = (e.EActor->AbilityLevel(
            CA_SPELLBREAKER) * 100) / e.EPActor->TotalLevel();
          int32 GainedXP[] = { 2, 5, 10, 25, 50, 100, 250, 500, 1000,
                             2500, 5000 };
          int16 Factor;
          Factor = (ItemLevel() + 2) - e.EPActor->TotalLevel();
          Factor = min(Factor,10);
          if (Factor >= 0) {
            if (isType(T_POTION) || isType(T_SCROLL))
              e.EActor->GainXP((GainedXP[Factor] * Percent) / 1000);
            else
              e.EActor->GainXP((GainedXP[Factor] * Percent) / 100);
            }  
        }

    if (GetHP() != MaxHP())
      RemoveStati(MASTERWORK);

    return DONE;

  }
/////////////////////////////////////////////////////////////////////////////
//                          Specific Types of Items                        //
/////////////////////////////////////////////////////////////////////////////

Corpse::Corpse(Creature *c, int8 _Type)
	: Food(FIND("corpse"),_Type)
  {
    int i, t = 0;
    Image &= 0x00FF;
    if (c) {
      mID=c->tmID;
      StatiIterNature(c,TEMPLATE)
          GainPermStati(TEMPLATE,0,SS_MISC,1, 1, S->eID,0);
      StatiIterEnd(c)
      StatiIterNature(c,CORPSE_FLAG)
          GainPermStati(CORPSE_FLAG,oThing(S->h),
            S->Source,S->Val, S->Mag, S->eID,S->CLev);
      StatiIterEnd(c)


      Named = c->Named;
      Image |= (c->Image & 0x0F00);
    } else {
      mID = FIND("human");
      Image |= TMON(mID)->Image & 0x0F00;
    } 
    TurnCreated = theGame->Turn;
    LastDiseaseDCCheck = -1; 
    Type=T_CORPSE;
  }

/*
Corpse::Corpse(rID iID, int8 _Type)
  : Food(iID,_Type)
  {
    mID = FIND("human");
    if (Type == T_CORPSE) {
      Image &= 0x00FF;
  		Image |= (TMON(mID)->Image & 0xFF00);
      }
  }
  */

void Corpse::SetImage()
  {
    Image = TITEM(iID)->Image;
    if (Type == T_CORPSE) {
      Image &= 0x00FF;
  		Image |= (TMON(mID)->Image & 0xFF00);
      }
  }

int32 Item::Weight(bool psych_might)
  {
    int32 w;
    w = TITEM(iID)->Weight * Quantity; 
    if (!w)
      switch (Type)
        {
          case T_COIN:
            return Quantity / 500;
          case T_MISSILE:
            return Quantity / 5;
          default:
            return Quantity / 2;
        }
    return w;
  }

int32 Food::Weight(bool psych_might)
  { 
    if (psych_might)
      return Item::Weight(psych_might) / 3;
    else
      return Item::Weight();
  }  

int32 Corpse::Weight(bool psych_might)
  {
    int32 i = 1500;
    switch (Size()) {
      case SZ_MINISCULE: i = 5; break;
      case SZ_TINY:      i = 50; break;
      case SZ_SMALL:     i = 500; break;
      case SZ_MEDIUM:    i = 1500; break;
      case SZ_LARGE:     i = 4000; break;
      case SZ_HUGE:      i = 10000; break;
      case SZ_GARGANTUAN:i = 250000; break;
      case SZ_COLLOSAL:  i = 1000000; break;
      }
    switch(Type) {
      case T_FIGURE: return Item::Weight();
      case T_STATUE: return i*5 * Quantity;
      case T_CORPSE: return i * Quantity;
      default:       return i * Quantity; // ww: make the compiler happy
      }
  }


EvReturn Food::Eat(EventInfo &e)
  {
    int16 nut, full; rID dID;
    static bool confirm = false;

    Eaten += 5; 

    if (e.EActor->HasAbility(CA_BURNING_HUNGER)) {
      bool okFood = false;
      if (isType(T_CORPSE)) {
        Corpse *c = (Corpse *)this;
        Monster *m = new Monster(c->mID);
        StatiIterNature(c,TEMPLATE)
            m->AddTemplate(S->eID);
        StatiIterEnd(this)
        m->CalcValues();
        int CR = m->ChallengeRating(); 
        if (!c->isFresh())
          e.EActor->IPrint(Format("%s is no longer fresh!",(const char*)Name(NA_THE)));
        else if (!m->isMType(MA_HAS_BLOOD))
          e.EActor->IPrint(Format("%s has no blood!",(const char*)Name(NA_THE)));
        else if (!m->HasMFlag(M_CARNI))
          e.EActor->IPrint(Format("%s was not a carnivore!",(const char*)Name(NA_THE)));
        else 
          okFood = true;
        delete m; 
      } 
      if (!okFood) { 
        e.EActor->IPrint("Only the blood of a carnivorous creature will feed your burning hunger.");
        return ABORT; 
      } 
    } 

    if (isType(T_CORPSE))
      nut = CorpseNut[((Corpse *)this)->Size()];
    else
      nut = TITEM(iID)->Nutrition;

    /* Balance Adjustment */
    // ww: not needed for game balance, IMHO: 
    // nut *= 2;

    if (e.EActor->HasStati(HUNGER)) {
      full = e.EActor->GetStatiDur(HUNGER);
      if (e.EActor->HasAbility(CA_SLOW_METABOLISM))
        e.EActor->SetStatiDur(HUNGER,-1,NULL,full + max(nut / 3,1));
      else  
        e.EActor->SetStatiDur(HUNGER,-1,NULL,full + nut);
      }
    else
      full = CONTENT;

    if (TMON(e.EActor->mID)->Size >= SZ_LARGE) 
      full /= 2; // ww: large creatures can hold more food
    if (full > BLOATED + 500)
      if (e.EActor->AbilityLevel(CA_BURNING_HUNGER)) {
        full = BLOATED + 500;
      } else if (!random(20))
        {
          DPrint(e,"You vomit!",
                   "The <EActor> vomits!");
          e.EActor->SetStatiDur(HUNGER,-1,NULL,min(full/2,HUNGRY));
          if (!confirm && e.EActor->yn("Stop eating?"))
                  goto StopEating;
          confirm = true; 
        } 

    if (e.EActor->HungerState() >= SATIATED && !confirm) {
      if (e.EActor->yn("You are full. Stop eating?"))
        {
          DPrint(e,"You stop eating the <EItem>.",
                   "The <EActor> stop eating the <EItem>.");
          goto StopEating;
        }
      else
        confirm = true;
      }

    if (isType(T_CORPSE)) {
      Corpse * c = (Corpse *)this;
      
      if (TMON(c->mID)->isMType(c->mID,MA_SAPIENT) && !TMON(c->mID)->isMType(c->mID,MA_ORC) &&
          !c->HasStati(TRIED,SK_WILD_LORE + 1000*EV_ALIGNED,e.EActor))
        {
          bool isCannibal;
          isCannibal = false;
          if ((e.EActor->isMType(MA_HUMAN) && TMON(c->mID)->isMType(c->mID,MA_HUMAN)) ||
              (e.EActor->isMType(MA_DWARF) && TMON(c->mID)->isMType(c->mID,MA_DWARF)) ||
              (e.EActor->isMType(MA_GNOME) && TMON(c->mID)->isMType(c->mID,MA_GNOME)) ||
              (e.EActor->isMType(MA_HALFLING) && TMON(c->mID)->isMType(c->mID,MA_HALFLING)) ||
              (e.EActor->isMType(MA_ELF) && TMON(c->mID)->isMType(c->mID,MA_ELF)) ||
              (e.EActor->isMType(MA_DROW) && TMON(c->mID)->isMType(c->mID,MA_DROW)) ||
              (e.EActor->isMType(MA_KOBOLD) && TMON(c->mID)->isMType(c->mID,MA_KOBOLD)) ||
              (e.EActor->isMType(MA_REPTILE) && TMON(c->mID)->isMType(c->mID,MA_REPTILE)
                 && TMON(c->mID)->isMType(c->mID,MA_HUMANOID)))
            isCannibal = true;
          c->GainPermStati(TRIED,e.EActor,SS_MISC,SK_WILD_LORE + 1000*EV_ALIGNED);
          e.EActor->AlignedAct(AL_NONLAWFUL,2 + isCannibal*3,
            isCannibal ? "cannibalism" : "eating sapient creatures");
        } 
              
      
      int16 DC = c->noDiseaseDC();
      int16 res = e.EActor->ResistLevel(AD_DISE);

      if (res == -1 || res >= DC || c->LastDiseaseDCCheck == DC)
        ; // no disease, we're good!
      else if (DC > 60 || !e.EActor->SkillCheck(SK_WILD_LORE,DC - res,true)) {
        // yes, disease ...
        for (int trials = 0; trials < 5; trials++) { 
          dID = theGame->GetEffectID(PUR_DISEASE,0,10,0);
          if (dID && !e.EActor->HasEffStati(DISEASED,dID)) {
            e.EActor->GainPermStati(DISEASED,NULL,SS_MISC,0,0,dID);
            e.EActor->IPrint("You've contracted <Res>!",dID);
            break; 
          } 
        } 
      } 
      c->LastDiseaseDCCheck = DC; 
    } 

    /* M_POISON corpses */
    /* M_ACIDIC corpses */
        
    if (Eaten >= 100) { // ww: was == 100, that was an annoying bug!
      Eaten = 0; /* avoid 'finish eating the partly eaten food' */
      DPrint(e,"You finish eating the <EItem>.",
                 "The <EActor> finishes eating the <EItem>.");
      EvReturn res = RES(iID)->Event(e,iID);
      Remove(true);
      e.EActor->RemoveStati(ACTING);
      confirm = false; 
      if (isType(T_CORPSE))
        e.EActor->Devour(((Corpse*)this));
      return DONE;
      }

    return DONE;

    StopEating:                                
    confirm = false;
    e.EActor->RemoveStati(ACTING);     
    if (Owner() != e.EActor) {
      if (Weight() > e.EActor->MaxPress())
        PlaceAt(e.EMap,e.EActor->x,e.EActor->y);
      else
        e.EActor->GainItem(this,false);     
      }
    return DONE;

  }

EvReturn Corpse::Event(EventInfo &e)
	{
    return NOTHING;
	}
	
bool Corpse::isFresh()
  { 
    return (theGame->Turn - TurnCreated) / HOUR_TURNS < 12; 
  }
  
bool Corpse::fullyDecayed()
  {
    int16 hours;
    hours = (theGame->Turn - TurnCreated) / HOUR_TURNS;
    /* Takes two weeks for a corpse to decay fully. */
    return hours > 24 * 14;
  }
   
int16 Corpse::noDiseaseDC()
  { 
    int16 baseDC, hours;
    
    if (TMON(mID)->isMType(mID,MA_ANIMAL))
      baseDC = 8;
    else if (TMON(mID)->isMType(mID,MA_BEAST))
      baseDC = 10;
    else if (TMON(mID)->isMType(mID,MA_UNDEAD))
      baseDC = 25;
    else if (TMON(mID)->HasFlag(M_HUMANOID))
      baseDC = 20;
    else
      baseDC = 15;
    
    StatiIterNature(this,TEMPLATE)
      if (TTEM(S->eID)->TType & TM_UNDEAD)
        baseDC = max(baseDC,25);
    StatiIterEnd(this)
        
    hours = (theGame->Turn - TurnCreated) / HOUR_TURNS;
    
    if (hours <= 6)
      return baseDC;
      
    return baseDC + (hours - 6) / 2; 
  }

EvReturn Food::Event(EventInfo &e)
	{
    switch(e.Event)
      {
        case EV_EAT:   return Eat(e);
        default: return NOTHING;
      }
	}

EvReturn Weapon::Event(EventInfo &e)
	{
		return NOTHING;
	}

EvReturn Creature::LoadCrossbow(EventInfo &e)
  {
    if (e.EActor->HasMFlag(M_NOHANDS)) {
      IPrint("You need to have hands to do that.");
      return ABORT;
      }
    if (HasFeat(FT_RAPID_RELOAD)) {
      DPrint(e,"You rapidly crank your <Obj>.",
             "The <EActor> rapidly reloads <his:EActor> <Obj>.",e.EItem);
    e.EItem->GainTempStati(COCKED,NULL,20,SS_MISC);
      e.EActor->Timeout += 30; 
    e.EActor->ProvokeAoO();
    } else {
      DPrint(e,"You crank your <Obj>.",
             "The <EActor> reloads <his:EActor> <Obj>.",e.EItem);
      e.EActor->Timeout += 10; 
      e.EItem->GainTempStati(COCKED,NULL,40,SS_MISC);
    } 
    return DONE;
  }

EvReturn Armour::Event(EventInfo &e)
	{
    int16 i; String str; bool id;
    switch (e.Event)
      {
        case POST(EV_WIELD):
          if (!activeSlot(e.EParam))
            return NOTHING;
          if (!(IFlags & IF_WORN))
            return NOTHING;    
          id = false;
          for (i=0;i!=8;i++)
            if (Qualities[i])
              {
                str = "quality::";
                if (LookupOnly(APreQualNames,Qualities[i]))
                  str += Lookup(APreQualNames,Qualities[i]);
                else
                  str += Lookup(APostQualNames,Qualities[i]);
                e.eID = FIND(str);
                if (!e.eID)
                  continue;
                e.isWield = true;
                e.isItem = true;  
                e.efNum = 0;
                ReThrow(EV_EFFECT,e);
                if (i == AQ_RADIANT)
                  id = true;
              }
          if (id)
            VisibleID(e.EActor);
          return NOTHING;
        case EV_LOSEITEM:
        case EV_REMOVE:
          return NOTHING;
      }
		return NOTHING;
	}

int16 Item::ParryVal(Creature *c, bool knownOnly)
{
  return 0;
}

int16 Weapon::ParryVal(Creature *c, bool knownOnly)
{ 
  TItem * ti = TITEM(iID);
  if (!ti) return 0; 
  int val = 0; 
  val = ti->u.w.ParryMod;
  /*if (knownOnly || Known & KN_PLUS)
    val += GetPlus();*/
  // ww: adamant from the SRD 
  if (HasQuality(IQ_ELVEN)) val++;
  if (HasQuality(IQ_ORCISH)) val--;
  int8 sk = c->WepSkill(this);
  if (sk == WS_NOT_PROF) 
    val -= 4; /* if you're not trained, don't expect miracles */
  else
    val += (sk-WS_PROFICIENT) * 2; /* if you're a grand master, do expect 'em */
  /* WW: Important: no matter how amazing you are, you're not going to be
   * parrying bullets with a rock in your hand. */
  val = min(ti->u.w.ParryMod * 2,val); 
  return max(0,val);
}

bool Weapon::isBaneOf(Creature *c)
  {
    static rID BaneList[33]; int16 i;
    if (Bane == 0)
      return false;
    else if (Bane == -2)
      {
        if (!eID)
          return false;
        if (!TEFF(eID)->GetList(BANE_LIST,BaneList,32))
          { Error("Bane is -2, but eID has no BANE_LIST.");
            return false; }
        for (i=0;i!=32 && BaneList[i];i++)
          if (c->isMType(BaneList[i]))
            return true;
        return false;
      }
    else 
      return c->isMType(Bane);
  }

bool Weapon::isBaneOf(int16 MType)
  {
    static rID BaneList[33]; int16 i;
    if (Bane == 0)
      return false;
    else if (Bane == -2)
      {
        if (!eID)
          return false;
        if (!TEFF(eID)->GetList(BANE_LIST,BaneList,32))
          { Error("Bane is -2, but eID has no BANE_LIST.");
            return false; }
        for (i=0;i!=32 && BaneList[i];i++)
          if (BaneList[i] == MType)
            return true;
        return false;
      }
    else 
      return MType == Bane;
  }

        
int16 Item::ArmVal(int16 typ, bool knownOnly) { return 0; }
int16 Item::CovVal(Creature *c, bool knownOnly) { return 0; }
int16 Item::DefVal(Creature *c, bool knownOnly) { return 0; }

int16 Armour::ArmVal(int16 typ, bool knownOnly)
{ 
  TItem * ti = TITEM(iID);
  if (!ti) return 0; 
  int val = 0; 
  if (isType(T_ARMOUR)) val = ti->u.a.Arm[typ];
  if (knownOnly || Known & KN_PLUS)
    val += GetPlus();
  // ww: adamant from the SRD 
  if (HasQuality(IQ_ADAMANT)) {
    if (GetGroup() & WG_HARMOUR) val += 3; 
    else if (GetGroup() & WG_MARMOUR || isType(T_SHIELD)) val += 2; 
    else if (GetGroup() & WG_LARMOUR) val += 1; 
  } 
  if (HasQuality(IQ_ORCISH) || HasQuality(IQ_DWARVEN)) val += 1;
  return max(0,val);
}

int16 Armour::CovVal(Creature *c, bool knownOnly) 
{ 
  if (isType(T_SHIELD)) {
    int sSize = Size();
    int cSize = c ? c->GetAttr(A_SIZ) : SZ_MEDIUM; 
    int res; 
    if (c && !c->HasFeat(FT_SHIELD_EXPERT))
      return 0;
    if (sSize > cSize) res = 10;
    else if (sSize == cSize) res = 8;
    else if (sSize + 1 == cSize) res = 4;
    else if (sSize + 2 == cSize) res = 2;
    else res = 1;
    if (isKnown(KN_PLUS) || !knownOnly) 
      res += GetPlus(); 
    return res; 
  } else { 
    int base = TITEM(iID)->u.a.Cov;
    base += (isKnown(KN_PLUS) || !knownOnly) ? GetPlus() : 0; 
    return base; 
  }
}

int16 Armour::DefVal(Creature *c, bool knownOnly) 
{
  if (isType(T_SHIELD)) {
    int sSize = Size();
    int cSize = c ? c->GetAttr(A_SIZ) : SZ_MEDIUM; 
    int res; 
    if (sSize > cSize) res = 8;
    else if (sSize == cSize) res = 6;
    else if (sSize + 1 == cSize) res = 4;
    else if (sSize + 2 == cSize) res = 2;
    else res = 1;
    if (c && c->HasFeat(FT_SHIELD_FOCUS)) res += 2;
    if (isKnown(KN_PLUS) || !knownOnly) res += GetPlus(); 
    return res; 
  } else { 
    int base = TITEM(iID)->u.a.Def;
    base += (isKnown(KN_PLUS) || !knownOnly) ? GetPlus() : 0; 
    return base; 
  }
}

// ww: since things like adamant affect this and multiple places (e.g.,
// describe and calcvalues) need this, let's factor it out here
int16 Armour::PenaltyVal(Creature * c, bool for_skills)
{ 
  // note that penalties are stored negative, so 'cord armour' has '-2'
  TItem * ti = TITEM(iID);
  if (!ti) return 0; 
  int val = 0; 
  int sSize = Size();
  int cSize = c ? c->GetAttr(A_SIZ) : SZ_MEDIUM; 
  if (isType(T_SHIELD)) {
    if (sSize > cSize) val = -10;
    else if (sSize == cSize) val = -8;
    else if (sSize + 1 == cSize) val = -4;
    else if (sSize + 2 == cSize) val = -2;
    else val = -1;
    if (sSize > cSize) val *= 2; 
  } else val = ti->u.a.Penalty; 
  if (for_skills) val += 2; 
  // ww: magical plusses specifically don't make the armour more limber
  if (HasQuality(AQ_GRACEFUL)) 
    val /= 2;
  if (HasQuality(IQ_ELVEN))
    val += 2;
  if (HasQuality(IQ_ORCISH))
    val -= 1;
  if (HasQuality(IQ_DARKWOOD))
    // ww: from the SRD, penalty reduced by 2
    val += 2;
  if (HasQuality(IQ_MITHRIL)) 
    // ww: mithral from the SRD, armour check penalties reduced by 3
    val += 3; 
  if (HasQuality(AQ_AGILITY)) 
    val = 0;
  if (for_skills && c && c->HasFeat(FT_ARMOUR_OPTIMIZATION))
    val = (val-2)/3; 
  return min(0,val);
}

// ww: since things like adamant affect this and multiple places (e.g.,
// describe and calcvalues) need this, let's factor it out here
int32 QItem::GetGroup(void)
{ 
  TItem * ti = TITEM(iID);
  if (!ti) return 0; 
  int val = ti->Group;
  if (HasQuality(IQ_MITHRIL)) {
    // www: srd: mithril shifts heavy->med and med->light
    if (val & WG_MARMOUR) {
      val |= WG_LARMOUR; val &= ~WG_MARMOUR;
    } 
    if (val & WG_HARMOUR) {
      val |= WG_MARMOUR; val &= ~WG_HARMOUR;
    } 
  } 
  return val;
}

int32 Item::GetGroup(void)
{
  TItem * ti = TITEM(iID);
  if (!ti) return 0; 
  return ti->Group;
}

// ww: this should really be a separate field associated with the TItem,
// but we'll just use the Penalty for now
int16 Armour::MaxDexBonus(Creature * c)
{
  int pen = PenaltyVal(c,false);
  return max(0, 9 + pen);
}

uint32 Creature::ArmourType()
  {
    uint32 at = 0;
    for(uint8 i=0;i!=SL_LAST;i++)
      if (InSlot(i))
        if (InSlot(i)->Type == T_ARMOUR)
          if (InSlot(i)->GetGroup() > at)
            at = InSlot(i)->GetGroup();
    return at;
  }

Dice Weapon::SDmg()
  {
    static Dice d;
    d = TITEM(iID)->u.w.SDmg;
    // ww: already in CalcValues d.Bonus += Plus;
    return d;
  }

Dice Weapon::LDmg()
  {
    static Dice d;
    d = TITEM(iID)->u.w.LDmg;
    // ww: already in CalcValues d.Bonus += Plus;
    return d;
  }

bool Item::HasSpell(uint16 sp)
  {
    rID sID, sID2 = theGame->SpellID(sp);
    TItem *ti = TITEM(iID);
    for(sID = ti->FirstRes(AN_SPELLS); sID ;sID = ti->NextRes(AN_SPELLS))
      if (sID == sID2)
        return true;
    return false;
  }

int16 Item::PItemLevel(Creature *cr)
  {
    if (cr->HasStati(TRUE_SIGHT))
      return ItemLevel();
    return max(ItemLevel(),GetStatiMag(MAGIC_AURA));
  }

int16 Item::ItemLevel(bool bounded)
  {
    int16 eff_lev, cost_lev;

    /* fjm: I'm getting iID == 0 here, with a valid Item object.
       Need to track this down. For now, use this to prevent
       crashes. */
    if (!iID)
      return 1;

    cost_lev = TITEM(iID)->Level;

    if (eID > 0)
      eff_lev = LevelAdjust(TEFF(eID)->Level,0,Plus);
    else
      eff_lev = 0; 

    return max(eff_lev,cost_lev);
  }

int16 Weapon::ItemLevel(bool bounded)
  {
    int16 i,j,QPlus,base, last; 
    /* This is HUGELY generous by the standards of the original
       OGL system. It means that by the time a PC of 7th level
       has finished completely to the 10th level of the dungeon,
       they'll have a weapon with an effective plus of about +12.
       Then again, it bears noting that we give the spellcasters
       a similar huge boost with the mana system, warriors lose
       their multiple high attacks from high BAB, and in the base
       system, throwing a single lone character into a dungeon
       would be unplayable. So I think (hope) that this balances
       out. */    
       // ww: it *is* too generous ...
    int8 PlusLevels[] =
      { 0, 0, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
        13, 14, 15, 16, 17, 18, 19, 20 };
    base = Item::ItemLevel(bounded);
    /* Specific weapons have their level calculated taking their
       qualities into account. */
    if (eID)
      return base;
    QPlus = Plus;
    for(i=0;i!=8;i++)
      if (Qualities[i] && Qualities[i] < WQ_LAST) {
        for (j=0;QualityMods[j][0] != -1;j++)
          if (QualityMods[j][0] == Qualities[i])
            QPlus += QualityMods[j][1];
        }

    //base += PlusLevels[min(22,QPlus)];
    //base += min(22,(QPlus*150)/100);
    base = min(22,(QPlus*100)/100);
    
    // a +5 sword with no other qualities should be level
    // 10, even though a +5 flaming sword is also level
    // 10, otherwise high-bonus swords are too easy for
    // dwarves and bards to make.
    if (bounded)
      base = max(base,Plus*2);
    
    return base;
  }

int16 Armour::ItemLevel(bool bounded)
  {
    int16 i,j,QPlus,base, last; 
    int8 PlusLevels[] =
      { 0, 0, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
        13, 14, 15, 16, 17, 18, 19, 20 };
    base = Item::ItemLevel(bounded);
    /* Specific weapons have their level calculated taking their
       qualities into account. */
    if (eID)
      return base;
    if (Type != T_ARMOUR && Type != T_SHIELD)
      return base; 
    QPlus = Plus;
    for(i=0;i!=8;i++)
      if (Qualities[i] && Qualities[i] < AQ_LAST) {
        for (j=0;AQualityMods[j][0] != -1;j++)
          if (AQualityMods[j][0] == Qualities[i])
            QPlus += AQualityMods[j][1];
        }

    // base += PlusLevels[min(22,QPlus)];
    base += min(22,(QPlus*150)/100);
    
    // a +5 sword with no other qualities should be level
    // 10, even though a +5 flaming sword is also level
    // 10, otherwise high-bonus swords are too easy for
    // dwarves and bards to make.
    if (bounded)
      base = max(base,Plus*2);
    
    return base;
  }

int16 QItem::ItemLevel(bool bounded)
  {
    int16 i,j,QPlus,base, last; 
    int8 PlusLevels[] =
      { 0, 0, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
        13, 14, 15, 16, 17, 18, 19, 20 };
    base = Item::ItemLevel(bounded);
    if (Type != T_ARMOUR && Type != T_SHIELD)
      return base; 
    QPlus = Plus;
    for(i=0;i!=8;i++)
      if (Qualities[i] && Qualities[i] < AQ_LAST) {
        for (j=0;AQualityMods[j][0] != -1;j++)
          if (AQualityMods[j][0] == Qualities[i])
            QPlus += AQualityMods[j][1];
        }

    // base += PlusLevels[min(22,QPlus)];
    base += min(22,(QPlus*150)/100);
    return base;
  }

// ww: otherwise a +0 corrosive long bow of speed is "mundane" 
bool Weapon::isMagic() {
  return Item::isMagic() || ItemLevel() > Item::ItemLevel();
}
bool Armour::isMagic() {
  return Item::isMagic() || ItemLevel() > Item::ItemLevel();
}
// ww: can you use your Strength bonus with this weapon? 
bool Weapon::useStrength() {
  return (!HasIFlag(WT_NO_STRENGTH) || HasQuality(WQ_MIGHTY));
}
// ww: can you use this weapon in a grapple? 
bool Weapon::useInGrapple(Creature *c) {
  return (Size(c) <= SZ_TINY || 
      Size(c) <= c->Attr[A_SIZ] - 2 ||
      HasIFlag(WT_ENTANGLE) ||
      (c->HasStati(ENGULFED) && Size(c) <= c->Attr[A_SIZ]));
}

bool Weapon::QualityOK(uint32 q,uint8 lev)
  {

    /*
  theGame->GetPlayer(0)->IPrint(Format("Weapon::QOK(%d) %s HasQuality(q) = %d, IQ_SILV = %d.", q, (const char*)Name(), HasQuality(q), HasQuality(IQ_SILVER)));
  */

    if (Qualities[7])
      return false;
    if (HasQuality(q))
      return false;
    /* Don't generate orcish rocks or flaming boulders! */
    if (iID == FIND("rock") || iID == FIND("boulder"))
      return false;
    switch(q)
      {
        case WQ_QUICK_LOADING:
          if (!HasIFlag(WT_CROSSBOW))
            return false; 
          break; 

        case WQ_IMPACT: 
          if (TITEM(iID)->Weight < 40)
            return false; 
          break; 
          // long swords, light maces: yes
          // rapier, whip: no

        case WQ_VENOM:
        case WQ_NIGHT:
          if (!(TITEM(iID)->HasFlag(WT_SUPER_SNEAK)))
            return false; 
          break; 

        case WQ_DEFENDING:
          if (TITEM(iID)->u.w.ParryMod == 0) return false;
          else break; 

        case WQ_MIGHTY: 
          if (!HasIFlag(WT_NO_STRENGTH)) return false;
          else break; 

        case WQ_STUNNING: 
          if (HasIFlag(WT_STUNNING)) return false;
          else break; 

        case WQ_ENERGY: 
          if (HasIFlag(WT_PENETRATING)) return false;
          else break; 

        case IQ_FEATHERLIGHT: 
          /* ww: massive weapon size hack: there's no point in generating
           * featherlight daggers for any player or featherlight longswords
           * for a human player ... */
          /* fjm: They should still exist, in a "game world continuity" sense,
             but let's make them *rare*. */
          if (random(20)) {
            if (Size(NULL) <= TMON(theGame->GetPlayer(0)->mID)->Size)
              return false; 
            else if (!QItem::QualityOK(q,lev))
              return false;
          }
          break; 

        case WQ_FLAMING:
        case WQ_FROST:
        case WQ_THUNDERING:
        case WQ_CORROSIVE:
        case WQ_SHOCKING:
          if (HasQuality(WQ_FLAMING) || HasQuality(WQ_FROST) ||
                HasQuality(WQ_SHOCKING) || HasQuality(WQ_CORROSIVE) ||
                HasQuality(WQ_THUNDERING))
            return false;
         break;
        case WQ_LAWFUL:
          if (HasQuality(WQ_CHAOTIC) || HasQuality(WQ_BALANCE))
            return false;
         break;
        case WQ_CHAOTIC:
          if (HasQuality(WQ_LAWFUL) || HasQuality(WQ_BALANCE))
            return false;
         break;
        case WQ_DISRUPTION:
          if (!TITEM(iID)->HasFlag(WT_BLUNT))
            return false;
         break;
        case IQ_PROFICIENT: 
          if (isType(T_MISSILE))
            return false; 
          break; 
        case WQ_WARNING: 
          if (isType(T_MISSILE) || TITEM(iID)->HasFlag(WT_NO_MELEE))
            return false; 
          break; 
        case WQ_SUNDERING: 
          if (isType(T_MISSILE) || isType(T_BOW))
            return false;
          break; 
        case WQ_KEEN: 
          if (isType(T_MISSILE))
            return false; 
          /* fall through */
        case WQ_VORPAL:
          if (!TITEM(iID)->HasFlag(WT_SLASHING) &&
              !TITEM(iID)->HasFlag(WT_PIERCING))
            return false;
          if (theGame->GetPlayer(0))
            if (theGame->GetPlayer(0)->ChallengeRating() < 5)
              return false;
         break;
        case WQ_HOLY:
          if (HasQuality(WQ_UNHOLY) || HasQuality(WQ_SOULDRINK) ||
              HasQuality(WQ_VAMPIRIC) || HasQuality (WQ_UNHOLY) ||
              HasQuality(WQ_BALANCE))
            return false;
         break;
        case WQ_UNHOLY: case WQ_SOULDRINK: case WQ_VAMPIRIC:
          if (HasQuality(WQ_HOLY) || HasQuality(WQ_BALANCE))
            return false;
         break;
        case WQ_BURST:
          if (!(HasQuality(WQ_FLAMING) || HasQuality(WQ_FROST) ||
                HasQuality(WQ_SHOCKING) || HasQuality(WQ_CORROSIVE) ||
                HasQuality(WQ_THUNDERING)))
            return false;
         break;
        case WQ_SLEEP:
          return false; // not implemented yet ?
          if (!isType(T_MISSILE) && !isType(T_BOW))
            return false;
         break;
        case WQ_DANCING:
        case WQ_PARRYING:
        case WQ_ENTROPIC:
          /* Not Implemented Yet */
          return false;

        case WQ_SLAYING: 
          if (theGame->GetPlayer(0))
            if (theGame->GetPlayer(0)->ChallengeRating() < 5)
              return false;
         break;      

        case WQ_RETURNING:
          if (!(TITEM(iID)->Group & WG_THROWN) || isType(T_BOW))
            return false;
          break; 

        case WQ_DISTANCE:
          if (!TITEM(iID)->u.w.RangeInc)
            return false;
         break;

        // ww: previously cleaving and smiting keyed off RangeInc, but that
        // led to some non-intuitive rule-outs
        case WQ_CLEAVING:
        // cleaving involves swinging through one enemy and hitting
        // another: it makes sense with swords and mauls, but not with
        // spears -- rule out piercing and bows
          if (TITEM(iID)->Type == T_BOW || TITEM(iID)->Type == T_MISSILE ||
              HasIFlag(WT_NO_MELEE))
            return false; // no bows
          if (!HasIFlag(WT_SLASHING) && !HasIFlag(WT_BLUNT)) return false;
            return false; // must be a swing
          break; 

        case WQ_SPEED:  // ww: no missiles
          if (TITEM(iID)->Type == T_MISSILE)
            return false;
          break; 

        case WQ_SMITING:
          if (TITEM(iID)->Type == T_BOW || 
              TITEM(iID)->Type == T_MISSILE ||
              HasIFlag(WT_NO_MELEE))
            return false; // no bows
          break;

    case IQ_SILVER: case IQ_MITHRIL: case IQ_ADAMANT:
    case IQ_IRONWOOD: case IQ_DARKWOOD: 
    case IQ_ELVEN: case IQ_DWARVEN: case IQ_ORCISH:
          if (!QItem::QualityOK(q,lev))
            return false;
          break; 

      }
    Qualities[7] = q;
    if (ItemLevel() > lev)
      { Qualities[7] = 0; return false; }
    Qualities[7] = 0;
    return true;
  }

bool QItem::QualityOK(uint32 q,uint8 lev)
{
  if (isType(T_COIN) || isType(T_BOOK))
    return false;
  if (Qualities[7])
    return false;
  if (HasQuality(q))
    return false;

  const int t = TITEM(iID)->Type;
  if (t == T_POTION || t == T_COIN || t == T_FOOD || t == T_SCROLL || 
      t == T_TOOL)
    return false; 

  /*
  theGame->GetPlayer(0)->IPrint(Format("QItem::QOK %s HasQuality(q) = %d, IQ_SILV = %d.", (const char*)Name(), HasQuality(q), HasQuality(IQ_SILVER)));
  */

  switch (q) { 
    // first: the building materials! 
    case IQ_SILVER: 
      if (Material() == MAT_MITHRIL || Material() == MAT_ADAMANT)
        return false; 
    case IQ_MITHRIL:
    case IQ_ADAMANT:
      // ww: by magic fiat, you can make almost anything out of silver --
      // "mitrhil longbow", "silver spear", "adamant sling bullet" --
      // they all make sense. However, "silver hide armour" and
      // "adamant leather armour", "mitrhil void" don't work. 
      
      /* fjm: However, we want to rule out "silver robes", "adamant
         copper wand", "mithril embroidered cloak", etc. */
      if (isType(T_CLOAK) || isType(T_CLOTHES) || isType(T_WAND) ||
          isType(T_BOOTS) || isType(T_BOOK) || isType(T_STAFF) ||
          isType(T_GIRDLE) || isType(T_AMULET) || isType(T_RING) ||
          isType(T_DUST)|| isType(T_CONTAIN) || isType(T_SCROLL) ||
          isType(T_POTION) || !stricmp(NAME(iID),"hat"))
        return false;
      
      if (isWooden() && TITEM(iID)->Type==T_ARMOUR)
        return false; 
      {
        switch (Material()) {
          case MAT_GLASS:
          case MAT_GEMSTONE: 
          case MAT_INDESTRUCTABLE_STONE:
            return false;
        }
      }
      if (!isCorporeal() || (isOrganic() && !isWooden()) || Material() == MAT_IRONWOOD || Material() == MAT_DARKWOOD)
        return false; 
      else
        return true; 

    case IQ_IRONWOOD: 
      // ironwood turns iron into "wood" ...
      return ((TITEM(iID)->Material == MAT_IRON) && Material() == MAT_IRON);
    case IQ_DARKWOOD: 
      // darkwood turns wood into "wood++" 
      return ((TITEM(iID)->Material == MAT_WOOD) && Material() == MAT_WOOD);

    // first: the races materials! 
    case IQ_ELVEN:
    case IQ_DWARVEN:
    case IQ_ORCISH:
      if (!(isType(T_WEAPON) || isType(T_ARMOUR) || isType(T_BOW) ||
            isType(T_SHIELD) || isType(T_MISSILE)))
        return false;
      if (HasIFlag(WT_RACIAL))
        return false; 
      else 
        return true; 
      if (HasQuality(IQ_ELVEN) || HasQuality(IQ_DWARVEN) || HasQuality(IQ_ORCISH))
        return false;
      if (q == IQ_ELVEN && HasQuality(IQ_ADAMANT))
        return false;
      if (q == IQ_ORCISH && HasQuality(IQ_MITHRIL))
        return false;
      return true;
    case IQ_FEATHERLIGHT:
      return true; 

    default: 
      return false;
  } 
}


bool Armour::QualityOK(uint32 q,uint8 lev)
  {
    if (Qualities[7])
      return false;
    if (HasQuality(q))
      return false;
    switch (q)
      {
        case AQ_FEARSOME:
        case AQ_SHIFTING:
        case AQ_BANEBLIND:
        case AQ_SPIKED:
        case AQ_MAGIC_RES:
          return false; // ww: not yet implemented


        case AQ_LIGHT_FOR:
        case AQ_MEDIUM_FOR:
        case AQ_HEAVY_FOR:
          if (HasQuality(AQ_LIGHT_FOR) ||
              HasQuality(AQ_MEDIUM_FOR) ||
              HasQuality(AQ_HEAVY_FOR))
            return false;
        case AQ_ETHEREALNESS:
        case AQ_SLICK:
        case AQ_MIGHT:
        case AQ_ENDURANCE:
        case AQ_HEALING:
        case AQ_STABILITY:
        case AQ_COMMAND:
        case AQ_RADIANT:
          if (isType(T_SHIELD))
            return false;
         break;
        case AQ_GRACEFUL: // shield OK 
         break; 

        case AQ_ARROW_DEF:
        case AQ_ANIMATED:
        case AQ_BASHING:
        case AQ_REFLECTION:
          if (!isType(T_SHIELD))
            return false;
         return false; // ww: not yet implemented 
         break;

        case AQ_SHADOW:
          if (isType(T_SHIELD))
            return false;
        case AQ_BLINDING:
          if (HasQuality(AQ_BLINDING) ||
              HasQuality(AQ_SHADOW))
            return false;
         break;

        case AQ_INVULN:
        case AQ_GREAT_INV:
          if (isType(T_SHIELD))
            return false;
          if (HasQuality(AQ_INVULN) ||
              HasQuality(AQ_GREAT_INV))
            return false;
         break;

        case AQ_SILENT:
        case AQ_AGILITY:
          if (isType(T_SHIELD))
            return false;
          if (TITEM(iID)->Group != WG_LARMOUR)
            return false;
         break;

    case IQ_SILVER: case IQ_MITHRIL: case IQ_ADAMANT:
    case IQ_IRONWOOD: case IQ_DARKWOOD: 
    case IQ_ELVEN: case IQ_DWARVEN: case IQ_ORCISH:
          if (!QItem::QualityOK(q,lev))
            return false;
          break; 
      }
    Qualities[7] = q;
    if (ItemLevel() > lev)
      { Qualities[7] = 0; return false; }
    Qualities[7] = 0;
    return true;

  }

void QItem::RemoveQuality(int8 q)
{
  switch (q)
    {
      case IQ_MASTERWORK:
        IFlags &= ~IF_MASTERWORK;
        return;
      case IQ_GHOST_TOUCH:
        IFlags &= ~IF_GHOST_TOUCH;
        return;
      case IQ_BLESSED:
        IFlags &= ~IF_BLESSED;
        return;
    }

  for(int16 i=0;i!=8;i++)
    if (Qualities[i] == q) 
      Qualities[i] = 0; 
  if (isType(T_WEAPON) || isType(T_MISSILE) || isType(T_BOW)) 
    if (!HasQuality(WQ_BANE) && !HasQuality(WQ_SLAYING))
      SetBane(0); 
  return; 
} 

void QItem::PurgeAllQualities()
  {
    memset(Qualities,0,sizeof(Qualities));
    if (isType(T_WEAPON))
      SetBane(0);
  }

void Weapon::RandomBane()
  {
    int16 Targets[] = {
      MA_HUMAN, MA_ELF, MA_ILLITHID, MA_ABERRATION, MA_ANIMAL, MA_DWARF,
      MA_DROW, MA_FAERIE, MA_GOBLIN, MA_KOBOLD, MA_ORC, MA_GOBLINOID,
      MA_LYCANTHROPE, MA_NAGA, MA_CELESTIAL, MA_BEAST, MA_CONSTRUCT,
      MA_GOLEM, MA_DRAGON, MA_ELEMENTAL, MA_GIANT, MA_GENIE, MA_UNDEAD,
      MA_MYTHIC, MA_OUTSIDER, MA_QUYLTHULG, MA_REPTILE, MA_TROLL,
      MA_VAMPIRE, MA_DEMON, MA_DEMON, MA_DEVIL, MA_DEVIL, MA_UNDEAD, 
      MA_ORC, MA_TROLL, 0 };
    int16 i = 0;
    while (Targets[i])
      i++;
      
    SetBane(Targets[random(i)]);
    ASSERT(GetBane());
  }

void QItem::AddQuality(int8 q, int16 param)
  { 


    switch (q) {
      case IQ_MASTERWORK:
        IFlags |= IF_MASTERWORK;
        return;
      case IQ_GHOST_TOUCH:
        IFlags |= IF_GHOST_TOUCH;
        return;
      case IQ_BLESSED:
        IFlags |= IF_BLESSED;
        return;
        
      case IQ_SILVER: 
      case IQ_MITHRIL: 
      case IQ_ADAMANT:
      case IQ_IRONWOOD: 
      case IQ_DARKWOOD: 
        RemoveQuality(IQ_SILVER);
        RemoveQuality(IQ_MITHRIL);
        RemoveQuality(IQ_ADAMANT);
        RemoveQuality(IQ_IRONWOOD);
        RemoveQuality(IQ_DARKWOOD);
        break;

      case IQ_ELVEN: 
      case IQ_DWARVEN: 
      case IQ_ORCISH:
        RemoveQuality(IQ_ELVEN);
        RemoveQuality(IQ_DWARVEN);
        RemoveQuality(IQ_ORCISH);
        break; 
    }
    
    if (isType(T_WEAPON) || isType(T_MISSILE) || isType(T_BOW)) 
      if (q == WQ_SLAYING || q == WQ_BANE) {
        if (param && !GetBane())
          SetBane(param);
        else if (GetBane() == 0)
          RandomBane();
        }
    
    for(int16 i=0;i!=8;i++)
      if (Qualities[i] == q) 
        return;
      else if (!Qualities[i])
        { Qualities[i] = q; return; }
    //Error(">8 qualities added to one weapon!"); 
    return;
  }



int8 QItem::Material()
  {
    if (HasQuality(IQ_MITHRIL))
      return MAT_MITHRIL;
    else if (HasQuality(IQ_ADAMANT))
      return MAT_ADAMANT;
    else if (HasQuality(IQ_DARKWOOD))
      return MAT_DARKWOOD;
    else if (HasQuality(IQ_IRONWOOD))
      return MAT_IRONWOOD;
    else if (HasQuality(IQ_SILVER))
      return MAT_SILVER;
    else if (HasQuality(IQ_WROUGHT))
      return MAT_WROUGHT;
    else if (HasQuality(IQ_ORCISH) && (TITEM(iID)->Material == MAT_IRON))
      return MAT_WROUGHT;
      
    return TITEM(iID)->Material;
  }


int16 Weapon::Size(Creature * wield)
  {
    int sofar = TITEM(iID)->Size; 
    if (HasQuality(IQ_FEATHERLIGHT) && wield)
      sofar = max(SZ_TINY,sofar - 1);
    if (wield && wield->HasFeat(FT_SPEAR_AND_SHIELD) && 
        wield->InSlot(SL_READY) &&
        wield->InSlot(SL_READY)->isType(T_SHIELD) && 
        isGroup(WG_SPEARS|WG_LANCES|WG_POLEARMS))
      sofar = max(SZ_TINY,sofar - 1);
    return sofar; 
  }

int16 Weapon::Threat(Creature* cr)
  {                            
    int8 Threat, Mult = 1;
    Threat = TITEM(iID)->u.w.Threat;
    if (HasQuality(WQ_KEEN))
      Mult += 1;
    if (cr && cr->HasFeat(FT_IMPROVED_CRIT))
      if (cr->isMonster() || cr->HasEffStati(IMP_CRIT,iID))
        Mult += 1;
    if (cr && cr->WepSkill(this) >= WS_GRAND_MASTERY)
      Mult += 1; 
    return 21 - (Threat*Mult);
  }

int16 Weapon::CritMult(Creature* cr)
  {
    int16 Mult = TITEM(iID)->u.w.Crit;
    if (cr->WepSkill(this) >= WS_HIGH_MASTERY)
      Mult += 1; 
    return Mult; 
  } 

int16 Weapon::RangeInc(Creature *cr)
  {
    int16 rinc = 0;
    if (!(TITEM(iID)->Group & WG_THROWN))
      if (!isType(T_BOW))
        return 0;      
    rinc = max ( TITEM(iID)->u.w.RangeInc, 1 ) ;
    if (HasQuality(WQ_DISTANCE))
      rinc *= 2; 
    switch (cr->WepSkill(this)) {
      case WS_GRAND_MASTERY: rinc *= 3; break;
      case WS_HIGH_MASTERY:  rinc *= 2; break;
    }
    return rinc; 
  }

int16 Weapon::DamageType(Thing * target)
{
  if (TITEM(iID)->HasFlag(WT_FIRE_DAMAGE))
    return AD_FIRE;
  int def =  
      TITEM(iID)->HasFlag(WT_SLASHING) ? AD_SLASH :
      TITEM(iID)->HasFlag(WT_PIERCING) ? AD_PIERCE : AD_BLUNT;
  if (!target || !target->isCreature())
    return def;
  Creature * c = (Creature *)target;
  int def_res = c->ResistLevel(def);
  int res; 
  if (def_res == -1) def_res = 999999;


  if (TITEM(iID)->HasFlag(WT_SLASHING) &&
      (res = c->ResistLevel(AD_SLASH)) != -1 && 
      res < def_res) {
    def = AD_SLASH;
    def_res = res; 
  }
  if (TITEM(iID)->HasFlag(WT_PIERCING) &&
      (res = c->ResistLevel(AD_PIERCE)) != -1 && 
      res < def_res) {
    def = AD_PIERCE;
    def_res = res; 
  }
  if (TITEM(iID)->HasFlag(WT_BLUNT) &&
      (res = c->ResistLevel(AD_BLUNT)) != -1 && 
      res < def_res) {
    def = AD_BLUNT;
    def_res = res; 
  }
  return def; 
}

bool Item::canFinesse() { return false; }

bool Weapon::canFinesse()
  {
    return isGroup(WG_DAGGERS|WG_SBLADES|WG_LIGHT|WG_STAVES|WG_FLEXIBLE|WG_MARTIAL);
  }

int32 QItem::Weight(bool psych_might)
  {
    int base = Item::Weight(psych_might);

    if (HasQuality(IQ_ELVEN))
      base = (base * 75) / 100;
    if (HasQuality(IQ_DWARVEN))
      base = (base * 125) / 100;
    if (HasQuality(IQ_DARKWOOD) || HasQuality(IQ_MITHRIL))
      base = base / 2;
    if (HasQuality(IQ_FEATHERLIGHT))
      base = base / 4; 
    /* ww: I don't see anything in the description about this stuff being
     * heavier ... 
    if (HasQuality(WQ_ORCISH))
      return (TITEM(iID)->Weight*150)/100;
      */
    return base; 
  }

int16 QItem::Hardness(int8 DType)
  {
    int8 hd = Item::Hardness(DType);
    if (hd <= 0) {
      if (DType == AD_RUST && hd == 0 && GetPlus() > 0)
        hd += GetPlus() * 5;
      return hd;
    } 
    if (HasQuality(IQ_DWARVEN))
      hd += 10;
    if (HasQuality(IQ_ORCISH) || HasQuality(IQ_SILVER))
      hd /= 2;
    if (HasQuality(IQ_ADAMANT) || HasQuality(IQ_DARKWOOD))
      hd *= 2;
    if (HasQuality(IQ_MITHRIL))
      hd = (hd * 150) / 100;
    if (GetPlus() >= 0)
      hd += GetPlus()*5;
    else
      hd += 50;     /* Cursed weapons are hard to destroy */
    return hd;
  }

