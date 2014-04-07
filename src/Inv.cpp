/* INV.CPP -- Copyright (c) 1999-2003 Julian Mensch
     This file contains all of the routines for manipulating
   both Character inventory (using slots) and Monster inventory
   (uasing a linked list), along with routines for handling
   containers.

     Item* Monster::InSlot(int8 slot)
     EvReturn Monster::Wield(EventInfo &e)
     EvReturn Monster::TakeOff(EventInfo &e)
     EvReturn Monster::PickUp(EventInfo &e)
     EvReturn Monster::Drop(EventInfo &e)
     void Monster::DropAll()
     Item* Monster::GetInv(bool first)

     Item* Character::InSlot(int8 sl)
     EvReturn Character::Wield(EventInfo &e)
     EvReturn Character::TakeOff(EventInfo &e)
     EvReturn Character::PickUp(EventInfo &e)
     EvReturn Character::Drop(EventInfo &e)
     void Character::DropAll()
     bool Character::Swap(int8 s)
     Item* Character::GetInv(bool first)
     void Character::Exchange()

     EvReturn Container::Event(EventInfo &e)
     Item* Container::operator[](int16 i)
     void Container::Unlist(Item *it)
     EvReturn Container::Insert(EventInfo &e)
     bool Container::XInsert(Item *it)
     EvReturn Container::TakeOut(EventInfo &e)
     int32 Container::Weight()
     Item* Creature::RandInv()

*/

#include "Incursion.h"

bool isExchange = false;

/*****************************************************************************\
*                             MONSTER INVENTORY                               *
\*****************************************************************************/

#define QD(time) (time / (e.EActor->HasFeat(FT_QUICK_DRAW) ? 4 : 1))
#define FE(time) (time / (e.EActor->HasFeat(FT_FASTER_THAN_THE_EYE) ? 4 : 1))


Item* Monster::InSlot(int8 slot, bool eff)
{
  /* Later, tweak this for left and right rings, etc. */
  Item *it = oItem(Inv);
  if (eff && HasStati(ALT_ITEM,slot)) {
    ASSERT (GetStatiObj(ALT_ITEM,slot)->isItem());
    return (Item*) (GetStatiObj(ALT_ITEM,slot));
    }
  if (!TMON(mID)->HasSlot(slot))
    return NULL;
  while(it) {
    if (it->IFlags & IF_WORN) {
      switch (slot) {
        case SL_WEAPON: if (it->isType(T_WEAPON)) return it; break; 
        case SL_READY: if (it->isType(T_SHIELD)) return it; break;
        case SL_ARCHERY: if (it->isType(T_BOW)) return it; break;
        default: if (it->activeSlot(slot)) return it; break; 
      }
    }
    it = oItem(it->Next);
  }
  return NULL;
}

EvReturn Monster::Wield(EventInfo &e)
  {
    Item *it;
    if (!TMON(mID)->HasSlot(e.EParam))
      return ABORT;
    if (e.EParam == SL_WEAPON || e.EParam == SL_ARCHERY)
      if (e.EItem->Size(this) > e.EActor->GetAttr(A_SIZ)+1)
        return ABORT;
      
    if (e.EItem->GetQuantity() > 1 && e.EItem->activeSlot(e.EParam) && 
         !e.EItem->isGroup(WG_THROWN))
      {
        it = e.EItem->TakeOne();
        it->Next = Inv;
        Inv = it->myHandle;
        it->SetOwner(e.EActor->myHandle);
        it->IFlags &= ~IF_WORN;
        e.EItem = it;
      }
    if (it = InSlot(e.EParam))
      {
        if (Throw(EV_TAKEOFF,this,NULL,it) == ABORT)
          return ABORT;
        ASSERT(!InSlot(e.EParam))
      }
    

    if (e.EParam == SL_ARMOR && m && isThreatened())
      {
        IPrint("You cannot put on armor while threatened by hostile creatures.");
        return ABORT;
      }

    /* Monsters can only wield things in their active slots,
       which is fine because that's all they need. We don't
       keep track of whether Joe kobold has his Wand of Striking
       in his belt slot or his ready hand slot -- we just want
       to make sure he can't get the benefit of two suits of
       armor at once. */
    if (!e.EItem->activeSlot(e.EParam))
      return DONE;
    
    DPrint(e,NULL,"An <EActor> readies an <EItem>."); 
    e.EItem->IFlags |= IF_WORN;
		if (HasFeat(FT_SOULBLADE) && e.EItem->activeSlot(e.EParam))
		  wieldSoulblade(e.EItem);
    
    if (e.EItem->eID && e.EItem->activeSlot(e.EParam)) {
      e.eID = e.EItem->eID;
      e.isWield = true;  
      if (TEFF(e.EItem->eID)->HasFlag(EF_CURSED)) {
        if (e.EItem->isKnown(KN_MAGIC|KN_BLESS)) 
          e.EActor->IPrint("The <EITem> curses itself!");
        e.EItem->IFlags &= ~(IF_BLESSED);
        e.EItem->IFlags |= (IF_CURSED);
        } 
      ReThrow(EV_EFFECT,e);
      ASSERT(e.EActor); 
      } 


    Timeout += QD(2000) / (100 + 10 * Mod(A_DEX));
    return DONE;
  }

EvReturn Monster::TakeOff(EventInfo &e)
  {
    if (e.EItem->IFlags & IF_CURSED)
      return ABORT;
      
    if (e.EParam == SL_ARMOR && m && isThreatened())
      {
        IPrint("You cannot take off armor while threatened by hostile creatures.");
        return ABORT;
      }
      
    e.EItem->IFlags &= ~IF_WORN;
		e.EItem->RemoveEffStati(FIND("soulblade"));
		if (e.EItem->isType(T_ARMOR) || e.EItem->isType(T_SHIELD))
      if (e.EItem->HasQuality(AQ_ETHEREALNESS))
		    RemoveStati(PHASED);
		Timeout += QD(2000) / (100 + 10 * Mod(A_DEX));
    return DONE;
  }

EvReturn Monster::PickUp(EventInfo &e)
  {
    if (HasMFlag(M_NOHANDS))
      return ABORT;
    if (HasStati(DO_NOT_PICKUP))
      return ABORT;
    if (e.EItem->Weight() > MaxPress())
      return ABORT;
    if ((e.EItem->Material() == MAT_SILVER || e.EItem->isBlessed()) && 
        (isMType(MA_DEMON) || isMType(MA_LYCANTHROPE) ||
         isMType(MA_DEVIL) || isMType(MA_UNDEAD)))
      return ABORT; 
    if(onPlane() != e.EItem->onPlane()) {
      if (!e.EItem->isGhostTouch() && !e.EActor->HasStati(MANIFEST)) {
        return ABORT; 
      } 
    } 
    
      

    if (e.EItem->isIllusion() && !e.EItem->isShade())
      {
        Thing *t; int16 i;
        IDPrint("As you reach to pick up the <Obj1>, your hand passes "
                "through it, and it winks out of existence!",
                "As the <Obj2> reaches to pick up the <Obj1>, <his:Obj2> hand passes "
                "through it, and it winks out of existence!", e.EItem, this);
        
        /* This warrants a disbelief check! */
        MapIterate(m,t,i)
          if (t->isCreature() && ((Creature*)t)->Percieves(e.EItem))
            e.EItem->DisbeliefCheck((Creature*)t);
        e.EItem->Remove(true);
        if (!HasFeat(FT_FASTER_THAN_THE_EYE)) {
          e.EActor->ProvokeAoO(); // WW: SRD Combat I
          if (e.EActor->isDead()) 
            return DONE;
          }
        Timeout += FE(1000) / (100 + 10 * Mod(A_DEX));        
        return DONE;
      }

    DPrint(e,NULL,"An <EActor> picks up an <EItem>."); 
    if (!HasFeat(FT_FASTER_THAN_THE_EYE)) {
      e.EActor->ProvokeAoO(); // WW: SRD Combat I
      if (e.EActor->isDead()) return ABORT; 
      }
    Timeout += FE(1500) / (100 + 10 * Mod(A_DEX));
    
    /* Remove the Item from the Map */
    e.EItem->Remove(false);

    /* Place the newly picked up item at the head
       of our Inventory linked list. */
    e.EItem->Next = Inv;
    Inv = e.EItem->myHandle;
    
    e.EItem->SetOwner(e.EActor->myHandle);
    return DONE;
  }

EvReturn Monster::Drop(EventInfo &e)
{
  Item *it;
  if (HasMFlag(M_NOHANDS))
    return ABORT;
  /* Take off the item, if necessary */
  if (e.EItem->IFlags & IF_WORN)
    if (Throw(EV_TAKEOFF,this,NULL,e.EItem) == ABORT)
      return ABORT;

  /* Place it on the map */
  e.EItem->Remove(false);
  e.EItem->PlaceAt(m,x,y);
  Timeout += FE(1000) / (100 + 10 * Mod(A_DEX));
  DPrint(e,NULL,"An <EActor> drops an <EItem>."); 
  return DONE; // ww: had no return value!
}

void Monster::DropAll()
  {
    Item *it;
    while(Inv) {
      it = oItem(Inv);
      it->Remove(false);
      it->IFlags &= ~IF_PROPERTY;
      if (Throw(EV_TAKEOFF,this,NULL,it) != ABORT)
        it->PlaceAt(m,x,y);
      }
  }

Item* Monster::GetInv(bool first)
  {
    static Item *it, *it2;
    Thing *t;

    if (first)
      { it = oItem(Inv); }
    
    if (!it)
      return NULL;

    it2 = it;
    if (it->isType(T_CONTAIN) && ((Container*)it)->Contents) {
      it = oItem(((Container*)it)->Contents);
      return it2;
      }

    if (it->Next) {
      it = oItem(it->Next);
      return it2;
      }

    t = oThing(it->Parent);
    if (!t)
      it = NULL;
    else if (t->isItem())
      it = oItem(t->Next);
    else if (t->isMonster())
      it = NULL;
    else
      Error("Glitch in Monster::GetInv!");

    return it2;
  }

        
      

/*****************************************************************************\
 *                            CHARACTER INVENTORY                            *
\*****************************************************************************/


Item* Character::InSlot(int8 sl, bool eff)
  {
    if (eff && HasStati(ALT_ITEM,sl)) {
      ASSERT (GetStatiObj(ALT_ITEM,sl)->isItem());
      return (Item*) (GetStatiObj(ALT_ITEM,sl));
      }
    if (sl != SL_LIGHT && sl != SL_INAIR && !TMON(mID)->HasSlot(sl))
      return NULL;
    return sl > SL_LAST ? NULL : oItem(Inv[sl]);
  }

EvReturn Character::Wield(EventInfo &e) {
    if (!TMON(mID)->HasSlot(e.EParam)) {
        IPrint("You can't use that inventory slot in your current form.");
        return ABORT;
    }
    if (e.EItem->GetQuantity() > 1 && e.EItem->activeSlot(e.EParam) && !e.EItem->isGroup(WG_THROWN)) {
        IPrint("You can't wield multiple items in an active slot.");
        return ABORT;
    }
    if (e.EItem->isType(T_CLOTHES) && e.EParam == SL_CLOTHES && e.EItem->eID)
        if (InSlot(SL_CLOAK) && InSlot(SL_CLOAK)->eID) {
            IPrint("You can't wear both a magical cloak and magical "
                "clothing; both occupy the same metaphysical 'niche'.");
            return ABORT;
        }
    if (e.EItem->isType(T_CLOAK) && e.EParam == SL_CLOAK && e.EItem->eID)
        if (InSlot(SL_CLOTHES) && InSlot(SL_CLOTHES)->eID) {
            IPrint("You can't wear both a magical cloak and magical "
                "clothing; both occupy the same metaphysical 'niche'.");
            return ABORT;
        }

    Item *other;
    if ((e.EParam == SL_WEAPON && (other = InSlot(SL_READY))) ||
        (e.EParam == SL_READY && (other = InSlot(SL_WEAPON))))
        if (e.EItem->HasIFlag(WT_REACH) != other->HasIFlag(WT_REACH))
            if (e.EItem->isType(T_WEAPON) && other->isType(T_WEAPON))
            {
                IPrint("You cannot wield a reach weapon along with a non-reach weapon.");
                return ABORT;
            }

    if (e.EParam == SL_ARMOR && m && isThreatened()) {
        IPrint("You cannot put on armor while threatened by hostile creatures.");
        return ABORT;
    }
    if (e.EParam == SL_ARMOR && m && (HasStati(STUCK) || HasStati(PRONE))) {
        IPrint("You cannot put on armor while stuck or prone.");
        return ABORT;
    }


    if (m && theGame->InPlay() && x != -1) { 
        if((isMType(MA_DEMON) || isMType(MA_LYCANTHROPE) || isMType(MA_DEVIL) || isMType(MA_UNDEAD))) {
            if (e.EItem->Material() == MAT_SILVER) {
                IPrint("You are too fiendish to use silver items."); 
                return ABORT; 
            } else if (e.EItem->isBlessed()) {
                e.EItem->MakeKnown(KN_BLESS);
                IPrint("You are too unholy to use blessed items."); 
                return ABORT; 
            } 
        }
        if(onPlane() != PHASE_MATERIAL) {
            if (!e.EItem->isGhostTouch()) {
                IPrint("You are incorporeal and cannot use material items."); 
                return ABORT; 
            } 
        } 
    }

    if (e.EParam == SL_ARCHERY)
        Error("Character attempting to use the fake SL_ARCHERY slot!");
    if (e.EParam == SL_WEAPON || e.EParam == SL_READY) 
        if (e.EItem->isType(T_WEAPON) || e.EItem->isType(T_BOW) || e.EItem->isType(T_SHIELD)) {
            if (e.EItem->Size(this) > Attr[A_SIZ]+1) {
                IPrint("The <Obj> is too large for you to wield effectively.",e.EItem);
                return ABORT;
            }
            if ((TITEM(e.EItem->iID)->HasFlag(WT_EXOTIC_1H) &&
                !HasEffStati(WEP_SKILL,e.EItem->iID)) ||
                (e.EItem->Size(this) + 
                TITEM(e.EItem->iID)->HasFlag(WT_TWO_HANDED) 
                > (Attr[A_SIZ]+
                (e.EItem->isType(T_SHIELD) ? 0 : HasFeat(FT_MONKEY_GRIP))))) {
               if (Inv[SL_WEAPON] || Inv[SL_READY]) {
                   IPrint("You grip the <Obj> experimentally, and find you would "
                       " need both hands free to wield it well.",e.EItem);
                   return ABORT;
               }
               Inv[SL_WEAPON] = e.EItem->myHandle;
               Inv[SL_READY]  = e.EItem->myHandle;
            } else if (!Inv[SL_WEAPON] && !Inv[SL_READY] && theGame->InPlay() && e.EItem->isType(T_WEAPON) &&
                (isExchange || yn(XPrint("Wield the <Obj> two-handed?",e.EItem)))) {
                Inv[SL_WEAPON] = e.EItem->myHandle;
                Inv[SL_READY]  = e.EItem->myHandle;
            }
        }

    Inv[e.EParam] = e.EItem->myHandle;
    e.EItem->SetOwner(e.EActor->myHandle);
    if (e.EItem->activeSlot(e.EParam))
        e.EItem->IFlags |= IF_WORN;
    if (theGame->InPlay()) {
        Timeout += QD(2000) / (100 + 10 * Mod(A_DEX));
        DPrint(e,NULL,"An <EActor> readies an <EItem>."); 
    }

    if ((e.EItem->HasQuality(WQ_UNHOLY) && isMType(MA_GOOD)) ||
        (e.EItem->HasQuality(WQ_HOLY) && isMType(MA_EVIL)) ||
        (e.EItem->HasQuality(WQ_CHAOTIC) && isMType(MA_LAWFUL)) ||
        (e.EItem->HasQuality(WQ_LAWFUL) && isMType(MA_CHAOTIC)) ||
        (e.EItem->HasQuality(WQ_BALANCE) && !isMType(MA_NEUTRAL))) {
        IPrint("Wielding the <Obj> makes you feel uneasy.",e.EItem);
    }

    if (HasFeat(FT_SOULBLADE) && e.EItem->activeSlot(e.EParam))
        wieldSoulblade(e.EItem);		  

    if (e.EItem->eID && e.EItem->activeSlot(e.EParam)) {
        e.eID = e.EItem->eID;
        e.isWield = true;  
        if (TEFF(e.EItem->eID)->HasFlag(EF_CURSED)) {
            if (e.EItem->isKnown(KN_MAGIC|KN_BLESS)) 
                e.EActor->IPrint("The <EITem> curses itself!");
            e.EItem->IFlags &= ~(IF_BLESSED);
            e.EItem->IFlags |= (IF_CURSED);
        } 
        ReThrow(EV_EFFECT,e);
        ASSERT(e.EActor); 
    }

    if (e.EParam == SL_LIGHT && isPlayer())
        thisp->UpdateMap = true;
    return DONE;
}

  EvReturn Character::TakeOff(EventInfo &e) {
      int16 i;
      for (i=0;i!=SL_LAST;i++)
          if (Inv[i] == e.EItem->myHandle)
              if (!TMON(mID)->HasSlot(i)) { 
                  IPrint("You can't access that inventory slot in your current form.");
                  return ABORT;
              }

      if (e.EParam == SL_ARMOR && m && isThreatened()) {
          IPrint("You cannot take off armor while threatened by hostile creatures.");
          return ABORT;
      }
      if (e.EParam == SL_ARMOR && m && (HasStati(STUCK) || HasStati(PRONE))) {
          IPrint("You cannot remove armor while stuck or prone.");
          return ABORT;
      }

      if (isPlayer() && e.EParam == SL_WEAPON)
          if (thisp->DigMode) {
              IPrint("You stop digging.");
              thisp->DigMode = false;
          }

      Timeout += QD(2000) / (100 + 10 * Mod(A_DEX));
      if (e.EItem->IFlags & IF_CURSED) {
          for(i=0;i!=SL_LAST;i++)
              if (Inv[i] == e.EItem->myHandle)
                  if (e.EItem->activeSlot(i)) {
                      if (i == SL_WEAPON || i == SL_READY)
                          IPrint("Try as you might, you can't seem to "
                          "let go of your <Obj>.", e.EItem);
                      else
                          IPrint("Try as you might, you can't seem to "
                          "get the <Obj> off.", e.EItem);
                      e.EItem->MakeKnown(KN_CURSE);
                      return ABORT;
                  }
      }

      for(i=0;i!=SL_LAST;i++)
          if (Inv[i] == e.EItem->myHandle)
              Inv[i] = 0;
      e.EItem->IFlags &= ~IF_WORN;
      e.EItem->RemoveEffStati(FIND("soulblade"));
      if (e.EItem->isType(T_ARMOR) || e.EItem->isType(T_SHIELD))
          if (e.EItem->HasQuality(AQ_ETHEREALNESS))
              RemoveStati(PHASED);
      if (e.EItem->isType(T_LIGHT) && isPlayer())
          thisp->UpdateMap = true;
      return DONE;
  }

EvReturn Character::PickUp(EventInfo &e) {
    uint8 i;
    if (HasMFlag(M_NOHANDS)) {
        IPrint("But you have no hands!");
        return ABORT;
    }

    if (HasStati(GRAPPLED) || HasStati(GRABBED) || HasStati(GRAPPLING) || HasStati(STUCK)) {
        IPrint("You can't pick something up while grappling or stuck.");
        return ABORT;
    }
    if (HasStati(CHARGING) && !HasFeat(FT_SPIRITED_CHARGE)) {
        if (HasStati(AUTO_CHARGE)) {
            RemoveStati(CHARGING);
        } else {
            IPrint("You can't pick something up while charging unless you have the Spirited Charge feat.");
            return ABORT;
        } 
    } 

    if (e.EItem->isIllusion() && !e.EItem->isShade()) {
        Thing *t; int16 i;
        if (!e.EItem->isRealTo(e.EActor)) {
            IPrint("You can't pick up an illusionary item.");
            return ABORT;
        }
        IDPrint("As you reach to pick up the <Obj>, your hand passes "
            "through it, and it winks out of existence!",
            "As the <Obj2> reaches to pick up the <Obj>, <his:Obj2> hand passes "
            "through it, and it winks out of existence!", e.EItem, this);

        /* This warrants a disbelief check! */
        MapIterate(m,t,i)
            if (t->isCreature() && ((Creature*)t)->Percieves(e.EItem))
                e.EItem->DisbeliefCheck((Creature*)t);
        e.EItem->Remove(true);
        if (!HasFeat(FT_FASTER_THAN_THE_EYE)) {
            e.EActor->ProvokeAoO(); // WW: SRD Combat I
            if (e.EActor->isDead()) 
                return DONE;
        } 
        Timeout += FE(1000) / (100 + 10 * Mod(A_DEX));
        return DONE;
    }

    if (e.EItem->Weight() > MaxPress()) {
        IPrint(Format("%s weighs %d and you cannot lift more than %d.",
            (const char*)e.EItem->Name(NA_THE),
            e.EItem->Weight() / 10, 
            MaxPress() / 10));
        return ABORT;
    }
    if((isMType(MA_DEMON) || isMType(MA_LYCANTHROPE) ||
        isMType(MA_DEVIL) || isMType(MA_UNDEAD))) {
            if (e.EItem->Material() == MAT_SILVER) {
                IPrint("You are too fiendish to pick up silver items."); 
                return ABORT; 
            } else if (e.EItem->isBlessed()) {
                e.EItem->MakeKnown(KN_BLESS);
                IPrint("You are too unholy to pick up blessed items."); 
                return ABORT; 
            } 
    }
    if(onPlane() != e.EItem->onPlane()) {
        TextVal PhaseDesc2[] = {
            { PHASE_MATERIAL, "material" },
            { PHASE_ETHERIAL, "ethereal" },
            { PHASE_ASTRAL,   "astral" },
            { PHASE_SHADOW,   "in the Demiplane of Shadows" },
            { PHASE_NEGATIVE, "in the Negative Material Plane" },
            { 0, NULL } };      

            if (!e.EItem->isGhostTouch() && !e.EActor->HasStati(MANIFEST)) {
                IPrint("You are <str> and thus the <Obj> <Str> intangible to you.",
                    Lookup(PhaseDesc2,onPlane()),e.EItem,
                    e.EItem->isPlural() ? "are" : "is"); 
                return ABORT; 
            } 
    } 

    DPrint(e,NULL,"An <EActor> picks up an <EItem>.");
    e.EItem->MakeKnown(KN_HANDLED);

    if (!HasFeat(FT_FASTER_THAN_THE_EYE)) {
        e.EActor->ProvokeAoO(); // WW: SRD Combat I
        if (e.EActor->isDead()) return ABORT; 
    }
    Timeout += FE(1500) / (100 + 10 * Mod(A_DEX));

    e.EItem->Remove(false);
    e.EItem->SetOwner(e.EActor->myHandle);
    LegendIdent(e.EItem); 
    /* ww: do this before stacking, otherwise if you have a healing potion and
    * you have wiz-ident you pick up an unidentified one */
    for(i=0;i!=SL_LAST;i++)
        if (InSlot(i) && !e.EItem->activeSlot(i))
            if (InSlot(i)->TryStack(e.EItem)) {
                e.EItem = NULL;
                return DONE;
            }
    if (this->isPlayer()) {
        int ap = thisp->Opt(OPT_AUTOPICKUP);
        Container *pack; 
        if (ap == 2 && Inv[SL_PACK] && 
            oItem(Inv[SL_PACK])->Type == T_CONTAIN &&
            (InSlot(SL_PACK) || AbilityLevel(CA_WILD_SHAPE))) { 
                // also check pack
                pack = (Container*)oItem(Inv[SL_PACK]);
                if (pack)
                    for (int j=0; (*pack)[j] != NULL; j++) {
                        if (*((*pack)[j]) == *e.EItem) {
                            IPrint("AutoStowing: <Obj>.",e.EItem);
                            if (Throw(EV_INSERT,this,NULL,pack,e.EItem)==DONE)
                                return DONE;
                        }
                    } 
        } 
    }

    e.EItem->RemoveStati(DROPPED,SS_MISC,-1,-1,e.EActor);
    Inv[SL_INAIR] = e.EItem->myHandle;
    e.EItem->SetOwner(e.EActor->myHandle);
    if (isPlayer() && !HasEffStati(ACTING,FIND("AutoLoot")))
        thisp->MyTerm->InventoryManager(true);
    else if (!isPlayer())
        Error("NPC Inventory?!");
    return DONE;
}

EvReturn Character::Drop(EventInfo &e)
  {
    uint16 i;
    if (HasMFlag(M_NOHANDS) && (Inv[SL_INAIR] != e.EItem->myHandle))
      {
        IPrint("But you have no hands!");
        return ABORT;
      }
      
    if (HasStati(ENGULFED))
      {
        IPrint("You can't drop items while engulfed.");
        return ABORT;
      }
      
    e.EItem->SetOwner(0);
    for(i=0;i!=SL_LAST;i++)
      if (Inv[i] == e.EItem->myHandle)
        Inv[i] = 0;

    e.EItem->PlaceAt(m,x,y);
    Timeout += FE(1000) / (100 + 10 * Mod(A_DEX));
    DPrint(e,NULL,"An <EActor> drops an <EItem>."); 
    e.EItem->GainPermStati(DROPPED,e.EActor,SS_MISC);
    
    return DONE;
  }

void Character::DropAll()
  {
    Item *it; int16 i;
    for(i=0;i!=SL_LAST;i++)
      if (Inv[i]) {
        it = oItem(Inv[i]);
        it->Remove(false);
        it->PlaceAt(m,x,y);
        }
  }

bool Character::Swap(int8 s) {
    Item *i, *i2; bool split = false;
    ASSERT(s != SL_INAIR);
    i = i2 = NULL;
    static TextVal BadSlotMsg[] = {
        { SL_READY, "That's too big for you to grip properly." },
        { SL_WEAPON, "That's too big for you to grip properly." },
        { SL_LSHOULDER, "That won't sit well on your shoulder." },
        { SL_RSHOULDER, "That won't sit well on your shoulder." },
        { SL_BELT, "Only girdles and belts can go there." },
        { SL_LIGHT, "Only a light source can go there." },
        { SL_EYES, "Only eyes, monocles and masks can go there." },
        { SL_CLOTHES, "Only robes and outfits can go in that slot." },
        { SL_ARMOR, "Only body armor can go there." },
        { SL_BOOTS, "Only footwear can go there." },
        { SL_CLOAK, "Only cloaks can go in that slot.",  },
        { SL_CLOTHES, "Only clothing can go there." },
        { SL_LRING, "Only rings can be worn on your left finger." },
        { SL_RRING, "Only rings can be work on yout right finger." },
        { SL_AMULET, "Only amulets, necklaces, periapts and brooches can go in that slot." },
        { SL_GAUNTLETS, "Only gauntlets or gloves can be worn on your hands." },
        { SL_HELM, "Only caps, helms, circlets, tiaras, headbands and helmets go there." },
        { SL_BRACERS, "Only bracers can go in that slot." },
        { SL_BELT1, "That's too large to fit well on your belt." },
        { SL_BELT2, "That's too large to fit well on your belt." },
        { SL_BELT3, "That's too large to fit well on your belt." },
        { SL_BELT4, "That's too large to fit well on your belt." },
        { SL_BELT5, "That's too large to fit well on your belt." },
        { SL_PACK, "Only a backpack or bag can serve as your pack." },
        { 0, NULL } };
        /* Assumption: everything can go in the 'inair' slot, and everything
        can be taken out of the 'inair' slot without problems. Further,
        items have no special behaviour when placed in/taken out of this
        slot. This makes sense, really. */

    if (!TMON(mID)->HasSlot(s)) {
        IPrint("You can't use that inventory slot in your current form.");
        return false;
    }

    if (s == SL_ARMOR || s == SL_CLOAK || s == SL_CLOTHES)
        if (HasStati(RAGING)) {
            IPrint("You can't do that while raging.");
            return false;
        }

    if (Inv[SL_INAIR]) {
        i2 = oItem(Inv[SL_INAIR]);
        if (!i2->allowedSlot(s,this)) {
            IPrint(Lookup(BadSlotMsg,s));
            return false;
        }
    }

    if (i2 && i2->GetQuantity() > 1)
        if (i2->activeSlot(s) && !i2->isGroup(WG_THROWN)) {
            if (Inv[s]) {
                IPrint("You need to put the <Obj> somewhere else first.", oThing(Inv[s]));
                return false;
            }
            split = true;
            i2 = i2->TakeOne();
        }

    if (Inv[s]) {
        i = oItem(Inv[s]);
        if (i2 && !i2->activeSlot(s))
            if (i->TryStack(i2)) {
                Inv[SL_INAIR] = 0;
                return true;
            }
        if (i->activeSlot(s)) {
            if (ThrowVal(EV_REMOVE,s,this,NULL,i,NULL)==ABORT)
                return false;
            if (Inv[s])
                return false;
        } else
            Inv[s] = 0;
    }

    if (Inv[SL_INAIR] && !i2->isDead()) 
        if (ThrowVal(EV_WIELD,s,this,NULL,i2,NULL)==ABORT) {
            if (split) {
                /* Add i2 back to the stack in SL_INAIR */
                oItem(Inv[SL_INAIR])->Quantity++;
                i2->Remove(true);
                return false;
            }
            /* Assumption: anything that can be removed from a slot
            can be placed back into it as well. */
            if (!i || ThrowVal(EV_WIELD,s,this,NULL,i) != ABORT)
                Inv[s] = i ? i->myHandle : 0;
            Inv[SL_INAIR] = i2 ? i2->myHandle: 0;

            return false;
        }

    if (i && thisp->MyTerm->AltDown() && i->GetQuantity() > 1 && !Inv[SL_INAIR]) {
        Item *_i;
        _i = OneSomeAll(i,this);
        if (_i != i) {
            Inv[SL_INAIR] = _i->myHandle;
            _i->SetOwner(myHandle);
            Inv[s]        = i->myHandle;
            i->SetOwner(myHandle);
            CalcValues();
            return true;
        }
        i = _i;
    } else if (i2 && thisp->MyTerm->AltDown() && i2->GetQuantity() > 1 && !Inv[s]) {
        Item *_i2;
        _i2 = OneSomeAll(i2,this);
        if (_i2 != i2) {
            Inv[s] = _i2->myHandle;
            _i2->SetOwner(myHandle);
            Inv[SL_INAIR]        = i2->myHandle;
            i2->SetOwner(myHandle);
            CalcValues();
            return true;
        }
        i2 = _i2;
    }

    if (!split)
        Inv[SL_INAIR] = (i && !i->isDead()) ? i->myHandle : 0;
    Inv[s]       = (i2 && !i2->isDead()) ? i2->myHandle : 0;
    CalcValues();
    return true;
}

// ww: return the best item of the given iID (or make one) ... used by Gods
// in IRH files to make your best matching weapon a chosen weapon, etc. 
Item* Creature::FindBestItemOrCreate(rID iID)
{
  Item * best = NULL;
  int best_level = 0; 

  Item * it;
  for(it=FirstInv();it;it=NextInv()) {
    if (it->iID == iID) {
      if (!best || it->ItemLevel() > best_level) {
        best = it; best_level = it->ItemLevel();
      } 
    } 
  } 
  if (!best)
    best = Item::Create(iID);

  return best; 
}

Item* Character::GetInv(bool first)
  {
    static Item *it, *it2; 
    static int8 sl;
    Thing *t;

    if (first)
      { sl = 0; it = oItem(Inv[sl]); }
    
    if (!it)
      {
        if (sl == SL_LAST)
          return NULL;
        do
          sl++;
        while ( (sl != SL_LAST && !Inv[sl]) ||
            // ww: previously we would return a two-handed weapon twice!
                (sl == SL_READY && Inv[sl] == Inv[sl+1]) );
        if (sl == SL_LAST)
          return NULL;
        it = oItem(Inv[sl]);
      }

    it2 = it;
    if (it->isType(T_CONTAIN) && ((Container*)it)->Contents) {
      it = oItem(((Container*)it)->Contents);
      return it2;
      }

    if (it->Next) {
      it = oItem(it->Next);
      return it2;
      }

    t = oThing(it->Parent);
    
    if (!t)
      it = NULL;
    else if (t->isItem())
      it = oItem(t->Next);
    else if (t->isCharacter())
      it = NULL;
    else
      Error("Glitch in Character::GetInv!");

    return it2;
  }

void Character::Exchange()
  {
    Item *new1, *new2, *old1, *old2, *it; int16 i, sl1, sl2;

    if (!theRegistry->Exists(defMelee))   defMelee = 0;
    if (!theRegistry->Exists(defOffhand)) defOffhand = 0;
    if (!theRegistry->Exists(defRanged))  defRanged = 0;
    if (!theRegistry->Exists(defAmmo))    defAmmo = 0;

    new1 = new2 = old1 = old2 = NULL; sl1 = sl2 = 0;
    isExchange = true;

    if (defRanged == 0 && defMelee == 0)
      { IPrint("You need to set your default melee and ranged weapons "
               "before you can use the swap weapons command effectively. "
               "Type '?' on the inventory window for more information.");
        goto XAbort; }
        
    if (Inv[SL_WEAPON] && oItem(Inv[SL_WEAPON])->isCursed())
      {
        IPrint("You try to change weapons, but find that you cannot "
               "release your grip on the <Obj>!", oItem(Inv[SL_WEAPON]));
        oItem(Inv[SL_WEAPON])->MakeKnown(KN_CURSE);
        return;
      }
    if (Inv[SL_READY] && oItem(Inv[SL_READY])->isCursed())
      {
        IPrint("You try to change weapons, but find that you cannot "
               "release your grip on the <Obj>!", oItem(Inv[SL_READY]));
        oItem(Inv[SL_READY])->MakeKnown(KN_CURSE);
        return;
      }
      
    if (Inv[SL_WEAPON] == defMelee) {
      /* Try to switch to ranged */
      if (defRanged) {  
        for (i=0;i!=SL_LAST;i++)
          if (Inv[i] == defRanged)
            { sl1 = i; goto foundRanged; }
        IPrint("Your default ranged weapon isn't easily accessible right now.");
        goto XAbort;

        foundRanged:
        new1 = oItem(defRanged);
        new2 = NULL; sl2 = 0;
        old1 = InSlot(SL_WEAPON);
        if (Inv[SL_READY] && (new1->HasIFlag(WT_TWO_HANDED) + new1->Size() > Attr[A_SIZ]))
          old2 = InSlot(SL_READY);
        else
          old2 = 0;
        }
      else {
        IPrint("You have no default ranged weapon selected.");
        goto XAbort;
        }
      }
    else {
      if (defMelee) {
        for (i=0;i!=SL_LAST;i++)
          if (Inv[i] == defMelee)
            { sl1 = i; goto foundMelee; }
        IPrint("Your default melee weapon isn't easily accessible right now.");
        goto XAbort;

        foundMelee:
        if (!defOffhand)
          goto foundBoth;
        for (i=0;i!=SL_LAST;i++)
          if (Inv[i] == defOffhand)
            { sl2 = i; goto foundBoth; }
        IPrint("Your default offhand weapon isn't easily accessible right now.");
        goto XAbort;

        foundBoth:
        new1 = oItem(defMelee);
        new2 = oItem(defOffhand);
        old1 = InSlot(SL_WEAPON);
        if (Inv[SL_READY] && (new1->HasIFlag(WT_TWO_HANDED) + new1->Size() > Attr[A_SIZ] || new2))
          old2 = InSlot(SL_READY);
        else
          old2 = 0;
        }
      else if (HasFeat(FT_MARTIAL_MASTERY) || HasAbility(CA_UNARMED_STRIKE)) {

        if (!defOffhand)
          goto foundOffOnly;
        for (i=0;i!=SL_LAST;i++)
          if (Inv[i] == defOffhand)
            { sl2 = i; goto foundOffOnly; }
        IPrint("Your default offhand weapon isn't easily accessible right now.");
        goto XAbort;

        foundOffOnly:
        new1 = NULL;
        new2 = oItem(defOffhand);
        old1 = InSlot(SL_WEAPON);
        old2 = defOffhand ? InSlot(SL_READY) : NULL;

        }
      else {
        IPrint("You have no default melee weapon selected.");
        goto XAbort;
        }
      }

    ASSERT(!Inv[SL_INAIR]);
    /* Take the new weapons out of their current slots. */
    if (new1) {
      if (!Swap(sl1))
        goto XAbort;
      if (Inv[SL_INAIR] != new1->myHandle)
        Error("Weapon Exchange wierdness!");
      Inv[SL_INAIR] = 0;
      }
    if (new2) {
      if (!Swap(sl2))
        goto XAbort;
      if (Inv[SL_INAIR] != new2->myHandle)
        Error("Weapon Exchange wierdness!");
      Inv[SL_INAIR] = 0;
      }

    /* Find some place to put the first old weapon */
    if (old1) {

      if (!Inv[SL_LSHOULDER] && old1->allowedSlot(SL_LSHOULDER,this))
        i = SL_LSHOULDER;
      else if (!Inv[SL_RSHOULDER] && old1->allowedSlot(SL_RSHOULDER,this))
        i = SL_RSHOULDER;
      else if (!Inv[SL_BELT1] && old1->allowedSlot(SL_BELT1,this))
        i = SL_BELT1;
      else if (!Inv[SL_BELT2] && old1->allowedSlot(SL_BELT2,this))
        i = SL_BELT2;
      else if (!Inv[SL_BELT3] && old1->allowedSlot(SL_BELT3,this))
        i = SL_BELT3;
      else if (!Inv[SL_BELT4] && old1->allowedSlot(SL_BELT4,this))
        i = SL_BELT4;
      else if (!Inv[SL_BELT5] && old1->allowedSlot(SL_BELT5,this))
        i = SL_BELT5;
      else {
        IPrint("You have no slot open to hold your <Obj>.",old1);
        goto XAbort;
        }
      if (!Swap(SL_WEAPON))
        goto XAbort;
      if (!Swap(i))
        { Swap(SL_WEAPON);
          goto XAbort; }
      }

    // ww: problem with holding a small weapon (like a rapier) in both
    // hands ...
    if (old2 == old1) old2 = NULL;

    /* Find some place to put the second old weapon */
    if (old2) {

      if (!Inv[SL_LSHOULDER] && old2->allowedSlot(SL_LSHOULDER,this))
        i = SL_LSHOULDER;
      else if (!Inv[SL_RSHOULDER] && old2->allowedSlot(SL_RSHOULDER,this))
        i = SL_RSHOULDER;
      else if (!Inv[SL_BELT1] && old2->allowedSlot(SL_BELT1,this))
        i = SL_BELT1;
      else if (!Inv[SL_BELT2] && old2->allowedSlot(SL_BELT2,this))
        i = SL_BELT2;
      else if (!Inv[SL_BELT3] && old2->allowedSlot(SL_BELT3,this))
        i = SL_BELT3;
      else if (!Inv[SL_BELT4] && old2->allowedSlot(SL_BELT4,this))
        i = SL_BELT4;
      else if (!Inv[SL_BELT5] && old2->allowedSlot(SL_BELT5,this))
        i = SL_BELT5;
      else {
        IPrint("You have no slot open to hold your <Obj>!",old2);
        goto XAbort;
        }

      if (!Swap(SL_READY))
        goto XAbort;
      if (!Swap(i))
        { Swap(SL_READY);
          goto XAbort; }
      }

    /* Wield the offhand item, if any */
    if (new2) {
      if (Inv[SL_INAIR] || Inv[SL_READY] || Inv[SL_WEAPON])
        Error("Weapon Exchange wierdness");
      Inv[SL_INAIR] = new2->myHandle;
      if (!Swap(SL_READY))
        goto Abort;
      Inv[SL_WEAPON] = 0;
      if ((Inv[SL_READY] != new2->myHandle) || Inv[SL_INAIR])
        Error("Weapon Exchange wierdness");
      }

    /* Wield the primary weapon */
    if (new1) {
      if (Inv[SL_INAIR] || Inv[SL_WEAPON])
        Error("Weapon Exchange wierdness");
      Inv[SL_INAIR] = new1->myHandle;
      if (!Swap(SL_WEAPON))
        { Inv[SL_INAIR] = 0; goto Abort; }
      if ((Inv[SL_WEAPON] != new1->myHandle) || Inv[SL_INAIR])
        Error("Weapon Exchange wierdness");
      }

    if (new1 && new2)
      IPrint("You are now wielding an <Obj> and an <Obj>.",new1,new2);
    else if (new1)
      IPrint("You are now wielding an <Obj>.",new1);
    else if (new2)
      IPrint("You are now fighting unarmed, with an <Obj> in your off hand.",new2);
    else
      IPrint("You are now fighting unarmed.");

    Timeout += 3000 / (100 + 10 * Mod(A_DEX));
    if (HasFeat(FT_QUICK_DRAW))
      Timeout /= 4;
      
    isExchange = false;
    if (isPlayer())
      thisp->MyTerm->ShowTraits();
    return;

    Abort:

    Timeout += 3000 / (100 + 10 * Mod(A_DEX));
    if (HasFeat(FT_QUICK_DRAW))
      Timeout /= 4;
      
    XAbort:

    /* Put everything back where it was by the brute force method. */
    IPrint("You fumble the items you were trying to exchange, dropping them!");
    if (old1)
      old1->PlaceAt(m,x,y);
    if (old2)
      old2->PlaceAt(m,x,y);
    if (new1)
      new1->PlaceAt(m,x,y);
    if (new2)
      new2->PlaceAt(m,x,y);
    isExchange = false;
    return;

  }


/*****************************************************************************\
*                             CONTAINER INVENTORY                            *
\*****************************************************************************/

void Creature::AccessTime(Item *it)
  {
    Thing *t; int16 n;
    t = it; n = 0;
    do {
      t = ((Item*)t)->GetParent();
      if (!t || !t->isItem())
        break;
      if (t->isType(T_CONTAIN))
        n += TITEM(((Item*)t)->iID)->u.c.Timeout;
      }
    while(1);
    if (HasFeat(FT_FASTER_THAN_THE_EYE))
      Timeout += n / 4;
    else
      Timeout += n;
  }


EvReturn Container::Event(EventInfo &e)
  {
    if (e.EItem != this)
      return NOTHING;
    switch(e.Event)
      {
        case EV_INSERT:   return Insert(e,false);
        case EV_TAKEOUT:  return TakeOut(e);
        default: return NOTHING;
      }
  }

Item* Container::operator[](int16 i)
  {
    hObj ho = Contents;
    while(ho && i)
      {
        i--;
        ho = oThing(ho)->Next;
      }
    return oItem(ho);
  }

void Container::Unlist(Item *it)
  {
    Item *c;
    if (Contents == it->myHandle)
      {
        Contents = it->Next;
        return;
      }
    c = oItem(Contents);
    if (!c)
      {
        Error("Miscall of Container::Unlist()!");
        return;
      }
    while(c->Next)
      {
        if (c->Next == it->myHandle)
          {
            c->Next = oThing(c->Next)->Next;
            return;
          }
        c = oItem(c->Next);
      }
    Error("Miscall of Container::Unlist()!");
  }
    
EvReturn Container::PickLock(EventInfo &e)
{
  if (!(HasStati(LOCKED)))
    return NOTHING;
  if (e.EActor->HasMFlag(M_NOHANDS)) { 
    e.EActor->IPrint("But you have no hands."); return ABORT; 
  } else if (e.EActor->onPlane() != PHASE_MATERIAL) { 
    e.EActor->IPrint("Your hands pass through the <Obj>.",this); 
    return ABORT; 
  } 
  if (HasStati(TRIED, SK_LOCKPICKING, e.EActor)) { 
    e.EActor->IPrint("You have already tried to pick the lock on the <Obj>.",this);
    return ABORT; 
  } 
  if (!((e.EActor->isPlayer() &&
          ((Player *)e.EActor)->Opt(OPT_AUTOOPEN)) || 
        e.EActor->yn(XPrint("Pick the <Obj>'s lock?",this),true)))
    return ABORT;
  e.EActor->Timeout += 30;
  int diff = 19 + m->Depth;
  if (HasStati(WIZLOCK) && !HasStati(WIZLOCK,-1,e.EActor)) {
    e.EActor->IPrint("The <Obj> is more difficult to pick.",this);
    diff += 10; 
  }
  if (e.EActor->SkillCheck(SK_LOCKPICKING,diff,true,
        GetStatiMag(RETRY_BONUS,SK_LOCKPICKING,e.EActor),"retry")) { 
    e.EActor->IDPrint("You pick the lock!",
        "The <Obj> picks the lock on the <Obj>.",
        e.EActor, this);
    RemoveStati(LOCKED); 
    RemoveStati(TRIED,SS_MISC,SK_LOCKPICKING); 
    if (!HasStati(TRIED,DF_LOCKED,this) && 
        !HasStati(SUMMONED,-1,this)) {
      // see DisarmTrap() 
      e.EActor->GainXP(90 + (diff - 14) * 10);
      GainPermStati(TRIED,this,SS_ATTK,DF_LOCKED);
    } 
    return NOTHING; 
  } else { 
    e.EActor->IDPrint("You fail to pick the lock on the <Obj2>. (You can try again after resting.)",
        "The <Obj1> tries to pick the lock on the <Obj2>, but fails.",
        e.EActor, this);
    BoostRetry(SK_LOCKPICKING,e.EActor);
    GainTempStati(TRIED,e.EActor,-2,SS_MISC,SK_LOCKPICKING); 
    return ABORT; 
  }
}

EvReturn Container::Insert(EventInfo &e, bool force)
{
  String s; Item *it; TItem *ti = TITEM(iID);

  if (!force && e.EActor->HasMFlag(M_NOHANDS))
  {
    e.EActor->IPrint("You need to have hands to do that.");
    return ABORT;
  }
  
  if (e.EActor->HasStati(RAGING))
    {
      e.EActor->IPrint("You can't do that while raging.");
      return ABORT;
    }

  if (e.EItem == e.EItem2) 
    {
      e.EActor->IPrint("You are not authorized to fold spacetime.");
      return ABORT;
    }

  if (!force) { 
    if (((e.EItem2->Material() == MAT_SILVER || e.EItem2->isBlessed()) || 
        (e.EItem->Material() == MAT_SILVER || e.EItem->isBlessed())) && 
        (e.EActor->isMType(MA_DEMON) || e.EActor->isMType(MA_LYCANTHROPE) ||
         e.EActor->isMType(MA_DEVIL) || e.EActor->isMType(MA_UNDEAD))) {
      if (e.EActor->isCharacter()) {
        e.EItem2->MakeKnown(KN_BLESS);
        e.EItem->MakeKnown(KN_BLESS);
      }
      e.EActor->IPrint("You cannot manipulate silver or blessed items.");
      return ABORT; 
    } 
    if(e.EActor->onPlane() != PHASE_MATERIAL) {
      if (!e.EItem2->isGhostTouch() || !e.EItem->isGhostTouch()) {
        e.EActor->IPrint("You cannot manipulate material items.");
        return ABORT; 
      } 
    } 
  }

  if (!force && (HasStati(LOCKED))) {
    e.EActor->IPrint("The <obj> is locked.",this);
    if (PickLock(e) == ABORT) return ABORT; 
  }

  bool packrat = e.EActor && e.EActor->HasFeat(FT_FASTER_THAN_THE_EYE); 

  if (force || (Owner() && Owner()->x == -1))
    ; // when we are creating a character for the first time, always 
  // put anything in the backpack ... because there isn't really
  // anywhere to drop it if we fail! 
  else { 
    /* Check Capacity, Weight, etc. */
    if (ti->u.c.WeightLim)
      if (Weight() + e.EItem2->Weight() > 
          (TITEM(iID)->u.c.WeightLim * (packrat ? 2 : 1)))
      {
        e.EActor->IPrint("The <Obj> can't hold that much weight.",e.EItem);
        return ABORT;
      }
    int cap = ti->u.c.Capacity * (packrat ? 2 : 1); 
    if (cap)
      if ((*this)[cap])
      {
        e.EActor->IPrint("The <Obj> can only hold <Num> item<Str>.",
            e.EItem,cap,cap == 1 ? "" : "s"); 
        return ABORT;
      }
    if (e.EItem2->Size() > ti->u.c.MaxSize + packrat)
    {
      e.EActor->IPrint("The <Obj> is too large to fit in the <Obj2>.",
          e.EItem2, e.EItem);
      return ABORT;
    }

    if (ti->u.c.CType && !e.EItem2->isType(ti->u.c.CType))
    {
      s = Lookup(ITypeNames, ti->u.c.CType);
      s = s.Lower();
      e.EActor->IPrint("The <Obj> can only hold <Str>.",
          e.EItem, (const char*)Lower(Pluralize(s)));
      return ABORT;
    }
  }

  e.EItem2->Remove(false);

  if (!Contents)
    Contents = e.EItem2->myHandle;
  else {
    it = oItem(Contents);
    if (it->TryStack(e.EItem2))
      goto Done;
    while(it->Next) {
      if (it->TryStack(e.EItem2))
        goto Done;
      it = oItem(it->Next);
    }
    if (it->TryStack(e.EItem2))
      goto Done;
    it->Next = e.EItem2->myHandle;
  }
  e.EItem2->Next   = 0;
  e.EItem2->Parent = myHandle;

Done:
  if (e.EActor && !packrat); 
  e.EActor->Timeout += ti->u.c.Timeout;
  Sort(NULL);

  return DONE;
}    

bool Container::XInsert(Item *it)
  {
    Thing *t; TItem *ti = TITEM(iID);

    if (ti->u.c.WeightLim)
      if ((Weight() + it->Weight() - ti->Weight) > ti->u.c.WeightLim)
        return false;

    if (ti->u.c.Capacity)
      if ((*this)[ti->u.c.Capacity])
        return false;
        
    if (it->Size() > ti->u.c.MaxSize)
      return false;

    if (ti->u.c.CType && !it->isType(ti->u.c.CType))
      return false;
         
    it->Remove(false);
    
    Item *it2;
    for (it2=oItem(Contents);it2;it2=oItem(it2->Next))
      if (it2->TryStack(it))
        goto Done;
    
    if (!Contents)
      Contents = it->myHandle;
    else {
      t = oThing(Contents);
      while(t->Next)
        t = oThing(t->Next);
      t->Next = it->myHandle;
      }
    it->Next   = 0;
    it->Parent = myHandle;

    Done:

    Sort(NULL);
    return true;
  }    


EvReturn Container::TakeOut(EventInfo &e)
  {
    /* Needed as an event to allow a container (i.e., a bag
       of tricks) to _prevent_ you from removing something. */

    if (e.EActor->HasMFlag(M_NOHANDS))
      {
        e.EActor->IPrint("You need to have hands to do that.");
        return ABORT;
      }

    if (e.EActor->HasStati(RAGING))
      {
        e.EActor->IPrint("You can't do that while raging.");
        return ABORT;
      }


    if (HasStati(LOCKED)) {
      e.EActor->IPrint("The <obj> is locked.",this);
      if (PickLock(e) == ABORT) return ABORT; 
      }

    e.EItem2->Remove(false);
    DPrint(e,NULL,"An <EActor> takes an <EItem2> out of an an <EItem>.");
    if (!e.EActor->HasFeat(FT_FASTER_THAN_THE_EYE)) {
      if (TITEM(iID)->u.c.Timeout > 0) {
         
        e.EActor->ProvokeAoO(); // WW: SRD Combat I
        if (e.EActor->isDead()) return ABORT; 
      }
    }
    e.EActor->Timeout += FE(TITEM(iID)->u.c.Timeout);
    if (e.EActor->isPlayer())
      e.EPActor->LegendIdent(e.EItem2);
    Sort(NULL);
    return DONE;
  }

/* ww: if you think about it, upending a backpack and dumping everything
 * onto the ground takes about as much time as fishing for any particular
 * object */
EvReturn Container::DumpOutContents(EventInfo &e)
  {
    /* Needed as an event to allow a container (i.e., a bag
       of tricks) to _prevent_ you from removing something. */

    if (e.EActor->HasMFlag(M_NOHANDS))
      {
        e.EActor->IPrint("You need to have hands to do that.");
        return ABORT;
      }

    if (e.EActor->HasStati(RAGING))
      {
        e.EActor->IPrint("You can't do that while raging.");
        return ABORT;
      }

    if (HasStati(LOCKED)) {
      e.EActor->IPrint("The <obj> is locked.",this);
      if (PickLock(e) == ABORT) return ABORT; 
    }

    hObj h = Contents;

    DPrint(e,NULL,"An <EActor> dumps out an <EItem>."); 
    if (h && !e.EActor->HasFeat(FT_FASTER_THAN_THE_EYE))  {
      e.EActor->ProvokeAoO();
      if (e.EActor->isDead()) return ABORT; 
      }
    e.EActor->Timeout += FE(TITEM(iID)->u.c.Timeout*2);

    int count = 0; 
    while (h) {
      Item *it = oItem(h);
      if (e.EActor->isPlayer())
        e.EPActor->LegendIdent(it);
      it->Remove(false);
      it->PlaceAt(e.EPActor->m,e.EPActor->x,e.EPActor->y);
      count++;
      h = Contents; 
    } 
    return DONE;
  }

/* ww: similarly, pouring stuff from one backpack into a chest is
 * not all the time-consuming */ 
EvReturn Container::PourInContents(EventInfo &e)
  {
    /* Needed as an event to allow a container (i.e., a bag
       of tricks) to _prevent_ you from removing something. */

    if (e.EActor->HasMFlag(M_NOHANDS))
      {
        e.EActor->IPrint("You need to have hands to do that.");
        return ABORT;
      }

    if (e.EActor->HasStati(RAGING))
      {
        e.EActor->IPrint("You can't do that while raging.");
        return ABORT;
      }

    if (HasStati(LOCKED)) {
      e.EActor->IPrint("The <obj> is locked.",this);
      if (PickLock(e) == ABORT) return ABORT; 
    }

    if ((Material() == MAT_SILVER || isBlessed()) && 
        (e.EActor->isMType(MA_DEMON) || e.EActor->isMType(MA_LYCANTHROPE) ||
         e.EActor->isMType(MA_DEVIL) || e.EActor->isMType(MA_UNDEAD))) {
      if (e.EActor->isCharacter())
        MakeKnown(KN_BLESS);
      e.EActor->IPrint("You cannot dump items into a silver or blessed container.");
      return ABORT; 
    } 
    if(e.EActor->onPlane() != PHASE_MATERIAL) {
      if (!isGhostTouch()) {
        e.EActor->IPrint("You cannot dump items into a material container.");
        return ABORT; 
      } 
    } 

    Thing * it = e.EItem2; 
    int count = 0; 
    int32 origTimeout = e.EPActor->Timeout; 
    EvReturn ret; 

    while (it) { 
      Item * it2 = (Item *)oThing(it->Next); 
      if (it->isItem() && it != this) {
        Item * i = (Item *)it; 
        if (e.EActor->isPlayer())
          e.EPActor->LegendIdent(i);

    if ((i->Material() == MAT_SILVER || i->isBlessed()) && 
        (e.EActor->isMType(MA_DEMON) || e.EActor->isMType(MA_LYCANTHROPE) ||
         e.EActor->isMType(MA_DEVIL) || e.EActor->isMType(MA_UNDEAD))) 
      continue; 
     
    if(e.EActor->onPlane() != PHASE_MATERIAL) 
      if (!i->isGhostTouch()) 
        continue;

        EventInfo e2 = e;
        e2.EItem2 = i; 
        ret = Insert(e2,false); 
        if (ret != ABORT) count++; 
      } 
      it = it2; 
    } 

    if (count) 
      e.EActor->Timeout = origTimeout + FE(TITEM(iID)->u.c.Timeout);

    return DONE; 
  }

int32 Container::Weight(bool psych_might)
  {
    int32 j; hObj h;
    j = 0;
    h = Contents;
    while (h) {
      j += oItem(h)->Weight(psych_might);
      h = oItem(h)->Next;
      }
    j -= ((TITEM(iID)->u.c.WeightMod * j) / 100);
    j += TITEM(iID)->Weight;
    return j;
  }

static int ContainerItemSort(const void *a, const void *b)
{
  hObj aa = *(hObj *)a,
       bb = *(hObj *)b;
  Item *l = oItem(aa),
       *r = oItem(bb);
  int it_l = TITEM(l->iID)->IType,
      it_r = TITEM(r->iID)->IType;
  PurgeStrings();
  if (it_l == it_r) 
          return (strcmp((const char*)l->Name(NA_LONG|NA_SINGLE),
                                  (const char*)r->Name(NA_LONG|NA_SINGLE)));
  else
          return it_l - it_r; 
}

void Container::Sort(int comparison(const void *,const void *))
{
   hObj h;
   hObj CArray[1024];
   int max = 0;

   h = Contents;

   while (h) {
           if (max == 1024) return; // cannot sort, too many, hack 
           CArray[max++] = h;
           h = oItem(h)->Next;
   }
   CArray[max] = 0;

   if (comparison == NULL) 
           qsort(CArray,max,sizeof(CArray[0]),ContainerItemSort);
   else
           qsort(CArray,max,sizeof(CArray[0]),comparison);


   for (int i=0;i<max;i++) {
     oItem(CArray[i])->Next = (CArray[i+1]);
   }

   Contents = CArray[0]; 

   return;
}


Item* Creature::RandInv()
  {
    Item *it; 
    int16 i = 0, j = 0;
    for(it=FirstInv();it;it=NextInv())
      i++;
    if (!i)
      return NULL;
    i = random(i);
    for(it=FirstInv();it;it=NextInv())
      if (j++ == i)
        break;
    return it;
  }
  
Item* Creature::RandGoodInv(int32 minval)
  {
    Item *it; 
    int16 i = 0, j = 0;
    for(it=FirstInv();it;it=NextInv())
      if (it->getShopCost(NULL,NULL) >= minval &&
           !it->isType(T_POTION) && !it->isType(T_SCROLL))
        i++;
    if (!i)
      return NULL;
    i = random(i);
    for(it=FirstInv();it;it=NextInv())
      if (it->getShopCost(NULL,NULL) >= minval &&
           !it->isType(T_POTION) && !it->isType(T_SCROLL))
        if (j++ == i)
          break;
    return it;
  }

