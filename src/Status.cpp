/* STATUS.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Code which governs the Stati possessed by creatures, items
   and other Things. Code for managing Fields (which are basically
   Stati for maps) are aqlso included here.

     void Thing::UpdateStati()
     void Thing::GainTempStati(int16 n, Thing *t, int16 Duration, int8 Cause, 
                                   int16 Val, int16 Mag, rID eID, int8 clev)
     void Thing::GainPermStati(int16 n,Thing *t,int8 Cause, int16 Val,int16 Mag,
                                   rID eID, int8 clev)
     void Thing::RemoveStati(int16 n,int8 Cause, int8 Val, int16 Mag,Thing *t, 
                                   bool elapsed)
     void Thing::RemoveOnceStati(int16 n, int16 Val)
     void Thing::RemoveEffStati(rID eID, int16 ev)
     void Thing::RemoveStatiFrom(Thing *t)
     bool Thing::__HasStati(int16 n,int8 Val, Thing *t)
     Status* Thing::GetStati(int16 n,int8 Val, Thing *t)
     Status* Thing::GetEffStati(int16 n,rID xID)
     void Thing::CleanupRefedStati()
     void Creature::StatiOn(Status *s)
     void Creature::StatiOff(Status *s)                            
     void Item::StatiOn(Status *s)
     void Item::StatiOff(Status *s)
     void Creature::StatiMessage(int16 n,int16 val, bool ending)

     void Map::UpdateFields()
     void Map::RemoveField(Field *f)
     void Map::NewField(int16 FType, int16 x, int16 y, uint8 rad,
                          Glyph Img, int16 Dur, rID eID, Creature *Creator)
     uint8 Field::FieldDC()
     EvReturn Map::MoveField(Field *f,int16 _cx, int16 _cy, bool is_walk)
     EvReturn Creature::FieldOn(EventInfo &e)
     EvReturn Creature::FieldOff(EventInfo &e)
*/

#include "Incursion.h"

void Thing::UpdateStati() {
    static EventInfo xe;
    int16 i, j; Status s; s.Nature = 0;

    StatiIter(this)
        // ww: we can die while updating stati, and then we end up 
        // placing something, but since our "m" field is null, we'll have
        // trouble placing it (this happened Thu Jan 15 10:16:04 PST 2004)
        if (isCreature() && (isDead() || !m || x == -1)) 
            StatiIterBreakout(this,return)
        if (S->Duration > 0) { 
            // ww: I've gotten an invalid object handle on the oThing() below,
            // so we add this sanity check 
            if (theRegistry->Exists(S->h) && HasStati(PERIODIC,-1,oThing(S->h)))
                continue;
            if (S->Nature == POISONED && HasStati(SLOW_POISON))
                continue;
            if (S->Nature == HUNGER && isCreature())
                thisc->GetHungrier(1);
            else  
                S->Duration--;

            if (S->Duration <= 0) {
                if (S->Nature == ACTING) {
                    s = *S;
                    if (s.h && oThing(s.h)->isCreature()) 
                        Throw(s.Val,this,oCreature(s.h));
                    else {
                        if (s.Mag)
                            ThrowDir(s.Val,s.Mag,this,NULL,oThing(s.h));
                        else
                            Throw(s.Val,this,NULL,oThing(s.h));
                    }
                }
                StatiIter_ElapseCurrent(this);
            }
        }
    StatiIterEnd(this)
}

int32 __max = 0;

void Thing::GainTempStati(int16 n, Thing *t, int16 Duration, int8 Cause, int16 Val, int16 Mag, rID eID, int8 clev) {
    if (isPlayer())
        thisp->statiChanged = true;

    if (n <= 0 || n >= LAST_STATI) {
        Error(Format("%s giving stati %d to %s with duration %d",
            (const char*)NAME(eID),n,(const char*)Name(),Duration));
    } 

    ASSERT(__Stati.Last < 10000);
    ASSERT(n != 204);

    if (n == GRAPPLING || n == GRABBED || n == GRAPPLED)
        if (!t) {
            Error("Giving GRAPPLING (or related) without Stati Object!"); 
            return;
        }

    hObj _h = t ? t->myHandle : 0;

    /* enchantment durations stack */
    if (Cause == SS_ENCH) {
        StatiIterNature(this,n)
            if (S->Source == SS_ENCH &&
                S->Val == Val &&
                S->Mag == Mag &&
                S->h == _h &&
                S->eID == eID &&
                Duration > 0) {
                    S->Duration += Duration; 
                    StatiIterBreakout(this,return) 
            }
        StatiIterEnd(this)
    }

    Status *s;
    if (__Stati.Nested) {
        int16 i;
        if (!__Stati.Added) { 
            __Stati.Added = (Status*)malloc(sizeof(Status)*ADDED_SIZE);
            memset(__Stati.Added,0,sizeof(Status)*ADDED_SIZE);
        }
        for (i=0;i!=ADDED_SIZE;i++)
            if (__Stati.Added[i].Nature == 0) {
                s = &(__Stati.Added[i]);
                if ((i+1) > __Stati.szAdded)
                    __Stati.szAdded = (i+1);
                else {
                    __Stati.Removed--;  
                    ASSERT (__Stati.Removed >= 0);
                    __Stati.Removed = max(0,
                    __Stati.Removed);
                }
                break;
            }
        if (i >= ADDED_SIZE)
        {
            Error("More than 128 Stati added inside a single StatIter!");
            return; 
        }
    } else {
        if (__Stati.Last >= __Stati.Allocated) {
            int newSize = 4; 
            if (__Stati.Allocated) {
                newSize = __Stati.Allocated * 2; 
                ASSERT(newSize); 
            } else {
                __Stati.Idx = (uint8 *)malloc(sizeof(__Stati.Idx[0]) * LAST_STATI);
                ASSERT(__Stati.Idx);
            } 
            if (!__Stati.S)
                __Stati.S = (Status *)malloc(sizeof(__Stati.S[0])*(newSize));
            else
                __Stati.S = (Status *)x_realloc(__Stati.S, sizeof(__Stati.S[0]),(newSize),__Stati.Allocated);
            ASSERT(__Stati.S);
            __Stati.Allocated = newSize;
            ASSERT(__Stati.Allocated <= 256); 
        }

        __max = max(__max,__Stati.Allocated);
        ASSERT(__Stati.Last < __Stati.Allocated);
        s = & __Stati.S[__Stati.Last];
        __Stati.Last++;
    }

    s->Nature=n;
    s->Duration=Duration;                                         
    s->Source=Cause;
    s->CLev = clev;
    s->Once = 0;
    s->Dis = 0;
    /* Bloody TRAP_EVENT Kludge (event codes need a full 16
    bits; because of how EffectValues work, we need to
    swap Val and Mag to avoid bigger kludges. */
    if (n == TRAP_EVENT) {
        s->Val = Mag;
        s->Mag = Val;
    } else {
        s->Val=Val;
        s->Mag=Mag;
    }
    s->h = _h;
    if (_h) {
        ASSERT(theRegistry->Exists(_h));
    }
    if (eID) {
        ASSERT(theGame->Get(eID));
    } 
    s->eID = eID;

    if (t && t!=this) 
        t->backRefs.Add(myHandle);

    Status _s;
    _s = *s;

    if (!__Stati.Nested) 
        _FixupStati();

    __Stati.Nested++;
    StatiOn(_s);
    EventInfo e;
    {
        e.Clear();
        e.EActor = thisc;
        e.EParam = n;
        e.vVal = Val;
        e.vMag = Mag;
        e.eID  = eID;
        e.vDuration = Duration;
        e.ETarget = t;
        ReThrow(EV_GAIN_STATI,e);
    }
    __Stati.Nested--; 

    if (!__Stati.Nested) 
        _FixupStati();

    if (EventSP >= 0)
        if (EventStack[EventSP].Terse)
            return;

    if (isCreature() && !e.Terse)
        thisc->StatiMessage(n,Val,false);

    return;
}
    

void Thing::GainPermStati(int16 n,Thing *t,int8 Cause, int16 Val,int16 Mag,rID eID, int8 clev) {
  GainTempStati(n,t,0,Cause,Val,Mag,eID,clev);
}

void Thing::CopyStati(Thing *t, int16 n) {
    ASSERT(n != TRAP_EVENT);
    StatiIter(t)
      GainTempStati(S->Nature,oThing(S->h),S->Duration,S->Source,S->Val,S->Mag,S->eID,S->CLev);
    StatiIterEnd(t)
  }

static int StatiSort(const void *a, const void *b) {
  Status *l = (Status *)a,
         *r = (Status *)b;
  const int ln = l->Nature ? l->Nature : LAST_STATI;
  const int rn = r->Nature ? r->Nature : LAST_STATI;
  return ln - rn; 
}

void Thing::_FixupStati() {
    int32 i;
    ASSERT(__Stati.Nested == 0); 

    /* Merge both lists, if needed */
    if (__Stati.Added) {
        int16 num, i, newSize;

        num = 0;
        for (i=0;i!=ADDED_SIZE;i++)
            if (__Stati.Added[i].Nature)
                num = (i+1);

        /* Odd case -- Stati is added inside a loop, then deleted
        in the same loop, therefore nothing to merge, but we
        still need to free Added. */
        if (!num)
            goto SkipMerge;

        newSize = max(2,__Stati.Allocated);
        while (newSize <= (__Stati.Last + num + 1))
            newSize *= 2;

        if (newSize != __Stati.Allocated) {
            Status *newStati;
            newStati = (Status*) x_realloc(__Stati.S,sizeof(Status),newSize,__Stati.Allocated);
            __Stati.S = newStati;
            __Stati.Allocated = newSize;

        }
        ASSERT(__Stati.Allocated <= 256);
        ASSERT(__Stati.Allocated > __Stati.Last + num);

        memcpy(__Stati.S+__Stati.Last,__Stati.Added,sizeof(Status)*num);
        __Stati.Last += num;

SkipMerge:
        free(__Stati.Added);
        __Stati.Added = NULL;
        __Stati.szAdded = 0;
    }

    if (__Stati.Removed >= __Stati.Last) {
        ASSERT(__Stati.Removed == __Stati.Last);
        __Stati.Removed = __Stati.Last = 0;
        memset(__Stati.Idx,-1,sizeof(__Stati.Idx[0]) * LAST_STATI);
        return; 
    } 

    /* sort the Stati */
    if (__Stati.Last > 1) 
        qsort(__Stati.S, __Stati.Last, sizeof(__Stati.S[0]), StatiSort);

    /* rebuild the index */
    memset(__Stati.Idx,-1,sizeof(__Stati.Idx[0]) * LAST_STATI);
    int last = -1; 
    for (i = 0; i < __Stati.Last; i++) {
        if (__Stati.S[i].Nature != last) {
            int nat = __Stati.S[i].Nature; 
            __Stati.Idx[nat] = i; 
            last = nat; 
        } 
    } 

    /* special kludge for ADJUST, to make StatIterAdjust work */
    for (i=ADJUST;i <= ADJUST_LAST; i++)
        if (__Stati.Idx[i] != 255) {
            __Stati.Idx[ADJUST_IDX] = __Stati.Idx[i];
            break;
        }    

    if (__Stati.Removed) {
        __Stati.Last -= __Stati.Removed;
        __Stati.Removed = 0; 
    } 
} 

void Thing::RemoveStati(int16 n,int8 Cause,int16 Val,int16 Mag,Thing *t) {
    ASSERT(n > 0 && n < LAST_STATI);

    int16 oNested = __Stati.Nested, c;
    c = 0;
    StatiIterNature(this,n)
        if ((S->Val == Val || Val == -1) &&
            (S->Mag == Mag || Mag == -1) &&
            (S->Source == Cause || Cause == -1) &&
            (!t || t->myHandle == S->h)) {
                StatiIter_RemoveCurrent(this); c++;
        } 
    StatiIterEnd(this)
    ASSERT(oNested == __Stati.Nested);  
}


/* If you want to remove a stati that has any Val,
   use the -1 wildcard as usual. If you want to
   remove a Stati with Val actually == -1 (i.e.,
   the Moment spell), call RemoveOnceStati with
   Val == -2. */
void Thing::RemoveOnceStati(int16 n, int16 Val) {
    ASSERT(n > 0 && n < LAST_STATI);

    if (n == ADJUST) {
        StatiIterAdjust(this)
            if ((S->Val == Val || Val == -1 ||
                (S->Val == -1 && Val == -2)) &&
                (S->Source == SS_ONCE)) {
                    StatiIter_RemoveCurrent(this); 
            } 
        StatiIterEnd(this)
    } else {
        StatiIterNature(this,n)
            if ((S->Val == Val || Val == -1 ||
                (S->Val == -1 && Val == -2)) &&
                (S->Source == SS_ONCE)) {
                    StatiIter_RemoveCurrent(this); 
            } 
        StatiIterEnd(this)
    }
}

void Thing::RemoveEffStati(rID eID, int16 ev, int16 butNotNature) {
    bool msg = false; 
    int16 oNested = __Stati.Nested, c;
    c = 0;

    StatiIter(this)
        if (S->Nature == INNATE_SPELL || S->Nature == butNotNature) 
            continue; 
        if (S->Source != SS_MONI && S->eID == eID && (S->h == 0 || butNotNature != -1)) {
            if (S->Nature == PERIODIC && isCreature())
                if (oThing(S->h) && oThing(S->h)->isItem() && !EventStack[EventSP].isRemove)
                    continue;

            if (!msg) {
                EvReturn r;
                PEVENT(ev,this,eID,
                    e.EParam = S->Nature; 
                    e.vVal = S->Val; 
                    e.vMag = S->Mag;
                    e.vObj = S->h,r);
                if (r == ABORT)
                    StatiIterBreakout(this,return)

                if (r == NOTHING && ev != EV_REMOVED) {
                    PEVENT(EV_REMOVED,this,eID,
                        e.EParam = S->Nature; 
                        e.vVal = S->Val; 
                        e.vMag = S->Mag;
                        e.vObj = S->h,r);
                    if (r == ABORT)
                        StatiIterBreakout(this,return)
                }
                if (r == NOTHING && isCreature())
                    thisc->StatiMessage(S->Nature,S->Val,true);                  
                msg = true; 
            } 
            /* We need to do this manually, because we want to send S with the
            eID intact to StatiOff, but StatiIter_RemoveCurrent calls 
            RemoveEffStati if S->eID is a TEffect, resulting in an infinite
            loop. */
            Status _S;
            _S = *S;
            __Stati.Removed++;      
            S->Nature = 0; 
            S->eID = 0;
            FixupBackrefs(S,this);
            StatiOff(_S);           
        } 
    StatiIterEnd(this);
    ASSERT(oNested == __Stati.Nested);
}

bool Thing::HasStatiFrom(Thing *t) {
    StatiIter(this)
        if (S->h == t->myHandle)
            StatiIterBreakout(this,return true);
    StatiIterEnd(this)
    return false;
}

void Thing::RemoveStatiSource(uint8 ss) {
    StatiIter(this)
        if (S->Source == ss)
            StatiIter_RemoveCurrent(this);
    StatiIterEnd(this)
} 

void Thing::RemoveStatiFrom(Thing *t, int16 ev) {
    int16 c,i; 
    static Status sLog[64]; 
    bool msg; 
    c = 0;
    StatiIter(this)
        if (S->h == t->myHandle) {
            /* This is cut and pasted from StatiIter_RemoveCurrent, which we
            can't use here because it will automatically redirect us to
            RemoveEffStati, and this function is used to handle the wierd
            "two stones of channeling with different plusses" case. */
            Status s; s = *S;                                
            __Stati.Removed++;                         
            S->Nature = 0; 
            S->eID = 0; 
            FixupBackrefs(S,this);

            for (i=0;i!=c;i++)
                if (sLog[i].eID == s.eID)
                    break;
            if (i == c) 
                sLog[c++] = s;

            StatiOff(s);
        } 
    StatiIterEnd(this)

    for (i=0;i!=c;i++) {
        EvReturn r;
        r = NOTHING;

        if (sLog[i].eID) {
            PEVENT(ev,this,sLog[i].eID,
                e.EParam = sLog[i].Nature; 
                e.vVal = sLog[i].Val; 
                e.vMag = sLog[i].Mag;
                e.vObj = sLog[i].h,r);
            if (r == ABORT)
                StatiIterBreakout(this,return)

            if (r == NOTHING && ev != EV_REMOVED) {
                PEVENT(EV_REMOVED,this,sLog[i].eID,
                    e.EParam = sLog[i].Nature; 
                    e.vVal = sLog[i].Val; 
                    e.vMag = sLog[i].Mag;
                    e.vObj = sLog[i].h,r);

                if (r == ABORT)
                    StatiIterBreakout(this,return)
            }
        }

        if (r == NOTHING && isCreature())
            thisc->StatiMessage(sLog[i].Nature,sLog[i].Val,true);                  
    }
}

/* Find all the objects that have a Stati where s->h
   points to us and either remove it or genericize it
   as appropriate. This will very likely cause some
   other objects to disappear (i.e., summoner dies
   and summoned creatures vanish). 
   I hope that this will fix up some crashiness and
   also other serious problems with regard to "orphan
   Stati". */

void Thing::CleanupRefedStati() {
    int32 i, j; Thing *t;
    static int32 flag = 0;
    bool removed_flag = false;

    /* We have to start again on every find, because removing
    a Stati from X may cause X to remove a Stati from us.
    For example, wiz summons creature which buffs all
    nearby spellcasters. Wiz dies, so creature winks out
    of existence, in the process removing the granted buff
    from Wiz. Complex, no? */
    flag = 1;

    StatiIter(this)
        FixupBackrefs(S,this);
    StatiIterEnd(this)

Restart:
    for (i=0;backRefs[i];i++) {
        if (theRegistry->Exists(backRefs[i]))
            if (!((t = oThing(backRefs[i]))->Flags & F_DELETE)) {
                StatiIter(t)
                    if (S->h == myHandle)
                        switch (S->Nature) {
                        /* There are surely going to be some cases where
                        we want to "genericize" the Stati instead of
                        removing it absolutely, by setting S->h = 0 
                        so we still have the Stati but don't have a
                        dangling, invalid hObj reference. I can't 
                        think what these might be right now, though,
                        so leave it as is for now and implement them
                        when the undesired behaviour shows up in play. */
                        default:
                            /* SS_ATTK stati come from the side effects of
                            attacks, and non-magical after-effects of
                            spells (choking on a cloud of conjured dust.
                            You don't stop being blind when you kill the
                            serpent that spat venom in your eyes, so we
                            genericize, rather than remove, the Stati. */
                            if (S->Source == SS_ATTK && (S->Nature != GRAPPLED &&
                                S->Nature != STUCK && S->Nature != GRABBED &&
                                S->Nature != GRAPPLING))
                                S->h = 0;
                            else {
                                StatiIter_RemoveCurrent(t); 
                            }                       
                    }
                StatiIterEnd(t)
                backRefs.Remove(i);
                goto Restart;
            }
    }

    backRefs.Clear();     
    flag = 0;
}

Status* Thing::GetStati(int16 n,int16 Val, Thing *t) {
  ASSERT(n > 0 && n < LAST_STATI);  
  StatiIterNature(this,n)
      if ((S->Val == Val || Val == -1) && (!t || t->myHandle == S->h))
          StatiIterBreakout(this,return S)
  StatiIterEnd(this)
  return NULL;
}

Status* Thing::GetEffStati(int16 n,rID xID, int16 Val, Thing *t) {
    if (n == -1) {
        StatiIter(this)
            if (S->eID == xID && S->Nature != INNATE_SPELL)
                if ((Val == -1) || (Val == S->Val))
                    if (!t || t->myHandle == S->h)
                        StatiIterBreakout(this,return S)
        StatiIterEnd(this)
    } else {
        StatiIterNature(this,n)
            if (S->eID == xID) 
                if ((Val == -1) || (Val == S->Val))
                    if (!t || t->myHandle == S->h)
                        StatiIterBreakout(this,return S)
        StatiIterEnd(this)
    } 
    return NULL;
}

void Creature::StatiOn(Status s) {
    int16 i; Creature *c;
    EventInfo *e;

    if (isPlayer())
        thisp->statiChanged = true;

    if (s.Nature == LEVITATION || s.Nature == POLYMORPH || s.Nature == STUNNED ||
        s.Nature == CONFUSED || s.Nature == GRAPPLED || s.Nature == GRABBED ||
        s.Nature == GRAPPLING || s.Nature == STUCK || s.Nature == PARALYSIS ||
        s.Nature == ENGULFED || s.Nature == PRONE)
        RemoveStati(SPRINTING);

    if (s.Nature == LEVITATION || s.Nature == POLYMORPH || s.Nature == PARALYSIS ||
        s.Nature == GRAPPLED || s.Nature == GRAPPLING || s.Nature == ENGULFED ||
        s.Nature == PRONE)
        ClimbFall();

    if (s.Nature == POISONED || s.Nature == STUNNED || s.Nature == DISEASED)
        LoseFatigue(1,false);

    switch(s.Nature) {
    case CHARGING:
        Reveal(true);
        break;
    case AFRAID:
        if (HasStati(ENGULFER))
            DropEngulfed();
        RemoveStati(RAGING);
        break;
    case RAGING:
        RemoveStati(AFRAID);
        break;
    case TEMPLATE:
        GainStatiFromTemplate(s.eID,true);
        break; 
    case INVIS:
    case INVIS_TO:
        RippleCheck(15);
        // drop through...
    case HIDING:
        RemoveStati(CHARGING);
        SetImage();
        if (m && x != -1)
            m->VUpdate(x,y);
        break;
    case POISONED:
    case DISEASED: {
            EventInfo e;
            SetEvent(e,EV_EFFECT,this,this);
            e.isWield = true;
            e.eID = s.eID;
            ReThrow(EV_EFFECT,e);
            break;
        }
    case ANCHORED:
        RemoveStati(PHASED);
        break;
    case MANIFEST:
        RemoveStati(HIDING);
        RemoveStati(INVIS);
        RemoveStati(INVIS_TO);
        RemoveStati(PHASED);
        // note fall-through
    case PHASED:
        Planeshift();
        if (onPlane() == PHASE_ETHERIAL)
            RippleCheck(15);
        break;
    case POLYMORPH:
        if (m) {
            MapIterate(m,c,i)
                if (c->isCreature() && !c->Percieves(this))
                    c->ts.MissedShapechange(c,this);
        }
        /* When player changes MType, redraw the whole map to
        account for INVIS_TO. */
        if (isPlayer()) {
            thisp->MyTerm->ShowMap();
            thisp->polyTicks = 0;
        }
        break;
    case BLIND:
        if (isPlayer())
            thisp->UpdateMap = true;
        break;
    case CHARMED:
        if (isMonster() && s.h && oThing(s.h)->isPlayer())
            if (s.Val == CH_DOMINATE || s.Val == CH_ALLY || s.Val == CH_COMMAND)
                thism->MakeCompanion(oPlayer(s.h),s.Val == CH_COMMAND ? PHD_COMMAND : PHD_MAGIC);
        ts.removeCreatureTarget(oCreature(s.h),TargetAny);
        ts.Retarget(this,true);
        break;
    case ILLUMINATED:
        if (HasStati(HIDING))
            Reveal(false);
        break;
    case PRONE:
        Creature *c;
        if (HasStati(MOUNTED)) {
            if (SkillCheck(SK_RIDE,20,true)) {
                if (HasStati(CHARGING)) {
                    RemoveStati(CHARGING);
                    IPrint("You slow your mount to avoid falling off it.");
                } else IPrint("You keep steady on your mount.");
                SetSilence();
                RemoveStati(PRONE);
                UnsetSilence();
                break;
            }
            ThrowVal(EV_DISMOUNT,DSM_THROWN,this,GetStatiObj(MOUNTED));
            RemoveStati(CHARGING);
        } else if (HasStati(MOUNT)) {
            c = (Creature*)GetStatiObj(MOUNT); // c == rider!
            if (c->SkillCheck(SK_RIDE,40,true)) {
                if (c->HasStati(CHARGING)) {
                    c->IPrint("By slowing down you miraculously keep your mount from falling.");
                    c->RemoveStati(CHARGING); 
                } else { 
                    c->IPrint("You miraculously keep your mount from falling.");
                }
                SetSilence();
                RemoveStati(PRONE);
                UnsetSilence();
                break;
            }
            ThrowVal(EV_DISMOUNT,DSM_THROWN,c,this);
        }
        RemoveStati(CHARGING);
        break;
    case LEVITATION:
        if (HasStati(MOUNTED))
            ThrowVal(EV_DISMOUNT,DSM_FLOAT,this);
        break;
    case TRUE_SIGHT:
    case DISBELIEVED:
        RemoveStati(ILLUS_DMG,-1,-1,-1,oThing(s.h));
        if (isMonster())
            thism->ts.Retarget(thism);
        break; 
    case ENRAGED:
    case CONFLICT:
    case DISGUISED:
        if (m)
            MapIterate(m,c,i)
            if (c->isCreature())
                c->ts.Retarget(c);
        SetImage();
        break;   
    case NEUTRAL_TO:
    case ENEMY_TO:
    case ALLY_TO:
        if (m)
            MapIterate(m,c,i)
            if (c->isCreature())
                if (c->isMType(s.Val))
                    c->ts.Retarget(c);
        break; 
    case INNATE_SPELL:
        if (isCharacter())
            thisp->setSpellFlags(s.eID,
            thisp->getSpellFlags(s.eID) | SP_INNATE);
        break;
    case ASLEEP:
        RemoveStati(HIDING);
        RemoveStati(CHARGING);
        RemoveStati(TUMBLING);
        break;
    case SPELL_ACCESS:
        if (!isPlayer())
            Error("Something other than the player character is gaining a "
            "SPELL_ACCESS stati. This is probably a bug. Please check "
            "the news section of the official Incursion webpage to see "
            "if a save has already been submitted for this; if one "
            "hasn't, please upload one to one of the discussion forums "
            "or to the bug database, where this relates to task #434.");
        break;

    }

    switch (s.Nature) {
      case ALIGNMENT:
      case ALLY_TO:
      case ALSO_TURN_MA_TYPE:
      case ALTER_PLUS:
      case ANCHORED:
      case ANIMAL_COMPANION:
      case ANIMAL_EMP_TRIED:
      case AVOID_MELEE:
      case BLEEDING:
      case BONUS_SCHOOL_CASTING_LEVEL:
      case BOOST_PLUS:
      case CAN_FORGET_SPELL:
      case CHARMED:
      case CHOKING:
      case COCKED:
      case COMMAND_ABILITY:
      case DETECTING:
      case DETECTING_TYPE:
      case ENRAGED:
      case DISBELIEVED:
      case DISPELLED:
      case ENEMY_TO:
      case BONUS_SLOTS:
      case FAV_ENEMY:
      case FORTIFICATION:
      case GAZE_REFLECTION:
      case GENERATION:
      case GTURN_ABILITY:
      case HALLU:
      case HAS_ANIMAL:
      case HAS_DOMAIN:
      case HOME_REGION:
      case HUNGER:
      case IGNORES_WEAPON_IMMUNITY:
      case ILLUMINATED:
      case INNATE_SPELL:
      case LIFESIGHT:
      case LOCKED:
      case MISS_CHANCE:
      case MY_CORPSE_IS:
      case NEUTRAL_TO:
      case NONDETECTION:
      case PRACTICE_MISS_CHANCE:
      case REGEN:
      case RETRY_BONUS:
      case SCHOOL_FOCUS:
      case SEE_INVIS:
      case SMITE_ABILITY:
      case SPECIALTY_SCHOOL:
      case SPECIES_AFFINITY:
      case SPEC_TIMEOUT:
      case SUMMONED:
      case SUSPEND_ATTK:
      case TELEPORTED:
      case TOUCH_ATTACK:
      case TRACKING:
      case TRIED:
      case TRUE_SIGHT:
      case TURN_ABILITY:
      case WARNING:
      case WATER_WALKING:
      case WIZLOCK:
      case STAFF_SPELLS:
      case SEEN_RIPPLE:
      case CRIT_WOUNDED_BY:
      case STORED_MM:
          // these stati are known to NOT require a CalcValues
          break;
      default: 
          if (theGame->InPlay() && m && m->InBounds(x,y)) {
              SetImage();
              CalcValues();
              if (isPlayer())
                  m->VUpdate(x,y);
          }
    }
}
              

void Creature::StatiOff(Status s, bool elapsed) {
    EventInfo *e; Thing *t;
    int16 i,j; Creature *c;
    Status _s; Item *it;

    switch(s.Nature) {
    case STONING:
        if (elapsed) {
            Creature * killer = NULL;
            Thing * corpse = NULL;
            Corpse * statue = NULL;
            if (theRegistry->Exists(s.h))
                killer = oCreature(s.h); 
            if (ResistLevel(AD_PLYS) == -1 || ResistLevel(AD_STON) == -1) {
                    IDPrint("You do NOT turn to stone!",
                        "The <Obj> does NOT turn to stone!",this);
            } else { 
                IDPrint("You turn to stone!",
                    "The <Obj> turns to stone!",this);
                statue = (Corpse*)Item::Create(FIND("statue"));
                statue->SetCorpseType(mID);
                ThrowDmg(EV_DEATH,0,0,"petrification",killer,this);
                corpse = GetStatiObj(MY_CORPSE_IS);
                if (corpse) { 
                    statue->PlaceAt(corpse->m,corpse->x,corpse->y);
                    corpse->Remove(true); 
                } 
            } 
        }
        else
            IPrint("You feel more limber now.");
        break;
    case MANIFEST:
    case PHASED:
        Planeshift();
        break;
    case CHARMED:
        if (!HasStati(CHARMED,-1,oCreature(s.h))) {
            Target *T;
            if (!ts.GetTarget(oCreature(s.h)))
                ts.addCreatureTarget(oCreature(s.h),TargetEnemy);
            T = ts.GetTarget(oCreature(s.h));
            if (T) {
                T->why.type = TargetResents;
                T->priority = max(T->priority,5);
                T->type = TargetEnemy;
            }
        } 
        ts.Retarget(this,true);
        break;
    case TEMPLATE:
        GainStatiFromTemplate(s.eID,false);
        break;
    case POISONED:
    case DISEASED: {
            EventInfo e;
            SetEvent(e,EV_EFFECT,this,this);
            e.isRemove = true;
            e.eID = s.eID;
            ReThrow(EV_EFFECT,e);
            break;
        }
    case INVIS:
    case HIDING:
    case INVIS_TO:
        SetImage();
        if (m && x != -1)
            m->VUpdate(x,y);
        break;
    case SUMMONED:
    case ILLUSION:
RestartDropItems:
        for(it=FirstInv();it;it=NextInv()) {
            if (!(it->IFlags & IF_PROPERTY)) {
                it->Remove(false);
                it->PlaceAt(m,x,y);
                goto RestartDropItems;
            } else
                it->Remove(true);
        }
        if (!isDead())
            Remove(true);
        break;
    case DETECTING:
        if (m && isPlayer()) 
            for(i=0;i!=m->Things.Total();i++)
                oThing(m->Things[i])->Flags &= ~F_HILIGHT;   /* Fix for multiplayer later. */
        break;
    case POLYMORPH:
        IPrint("You return to your normal form.");
        if (isPlayer())
            thisp->polyTicks = 0;
        Shapeshift(tmID,false,NULL);
        MapIterate(m,c,i)
            if (c->isCreature() && !c->Percieves(this))
                c->ts.MissedShapechange(c,this);
        /* When player changes MType, redraw the whole map to
        account for INVIS_TO. */
        if (isPlayer())
            thisp->MyTerm->ShowMap();
        break;
    case TRANSFORMED:
        Thing *t;
        t = oThing(s.h);
        ASSERT(t);
        if (!t->isDead()) {
            IDPrint(NULL,"The <Obj> changes back into <Str>.",
                this,(const char*)t->Name(NA_A));
            t->PlaceAt(m,x,y);
            Remove(true);
        } else
            IDPrint(NULL,"The <Obj> changes back into <Str>, which "
            "has been fully destroyed.",
            this,(const char*)t->Name(NA_A));
        break;
    case HUNGER:
        if (!elapsed)
            break;
        IPrint("You starve to death...");
        if (Type == T_PLAYER)
            SetSilence();
        ThrowDmg(EV_DEATH,0,0,"starvation",NULL,this);
        break;  
    case BLIND:
        if (isPlayer())
            thisp->UpdateMap = true;
        break;
    case SINGING:
        bool removed; int16 n;
        MapIterate(m,c,i) {
            StatiIter(c)
                if (S->h == myHandle && S->Source == SS_SONG) {
                    StatiIter_RemoveCurrent(c);
                } 
            StatiIterEnd(c)
        }
        break;
    case ILLUS_DMG:
        cHP = max(cHP + s.Mag, mHP + GetAttr(A_THP));
        break;
    case ENRAGED:
    case CONFLICT:
    case DISGUISED:
        if (m)
            MapIterate(m,c,i)
            if (c->isCreature())
                c->ts.Retarget(c);
        SetImage();
        break;   
    case INNATE_SPELL:
        if (isCharacter())
            thisp->setSpellFlags(s.eID,
            thisp->getSpellFlags(s.eID) &  (~SP_INNATE));
        break;  
    case SLIP_MIND: {
            rID enchID = theGame->SpellID(s.Val);

            /* We got enchanted, but something dispelled it before
            we should get our second saving throw, so do nothing. */
            if (!HasEffStati(-1,enchID))
                break;

            if (SavingThrow(WILL,s.Mag,SA_MAGIC|SA_SPELLS|
                ( TEFF(enchID)->HasFlag(EF_FEAR) ? SA_FEAR : 0))) {
                if (GetEffStatiObj(-1,enchID))
                    IPrint("Your mind slips free of the <Obj>'s <rID> effect!",
                    GetEffStatiObj(-1,enchID),enchID);
                else
                    IPrint("Your mind slips free of the <rID> effect!",enchID);
                RemoveEffStati(enchID);
            } else
                IPrint("Your slippery mind fails to escape the enchantment.");
        }
        break;
    case ALIGNMENT:
        if (this == theGame->GetPlayer(0))
            Error("The player character is losing her ALIGNMENT stati. "
            "Under normal circumstances, this shouldn't happen. "
            "If you are able to get this error repeatably, please "
            "upload a save to the bug database on the website, if "
            "no one else has already done so.");
        break;     
    case SPELL_ACCESS:
        if (s.eID == FIND("Paladin"))
            break;
        if (this != theGame->GetPlayer(0))
            break;
        Error("Your character is losing the SPELL_ACCESS stati. This is "
            "possibly the root of the 'losing spellcasting ability' "
            "Critical bug in the buglist. If you find "
            "you can produce this message repeatably, please upload a "
            "saved game and steps to reproduce to the Incursion bug "
            "database unless someone else has done this "
            "already. Thanks for helping to support Incursion!");
        break;
    }
    if (theGame->InPlay()) {
        SetImage();
        CalcValues();
        if (s.eID && m) {
            for (i=0;m->Fields[i];i++)
                if (m->Fields[i]->Creator == myHandle && 
                    m->Fields[i]->FType & FI_MOBILE &&
                    m->Fields[i]->eID == s.eID) {
                        m->RemoveField(m->Fields[i]);
                } 
        } 
        if (isPlayer() && m && y != -1)
            m->VUpdate(x,y);
    }

    if (isPlayer())
        thisp->statiChanged = true;

}

void Item::StatiOn(Status s) {
    switch (s.Nature) {
    case DISPELLED:
        ReApply();
        break;
    case BOOST_PLUS:
        ReApply();
        break;
    }
    return;
}
  
void Item::StatiOff(Status s, bool elapsed)
{
    EventInfo xe;
    switch (s.Nature) {
    case DISPELLED:
        if (!Owner() || !eID)
            break;
        Owner()->IPrint("The grey glow fades from your <Obj>.",this);
        ReApply();
        break;
    case SUMMONED:
    case ILLUSION:
        if (!isDead()) {
            if (!(Owner() && (Owner()->HasStati(SUMMONED) 
                || Owner()->isDead())))
                IDPrint("","The <Obj> winks out of existence.",this);
            Remove(true);
        }
        break;
    case BOOST_PLUS:
        ReApply();
        break;
    }
}

void Creature::StatiMessage(int16 n,int16 val, bool ending) {
    if (isDead())
        return;

    switch(n) {
    case CHANNELING:
        if (ending)
            IPrint("The warmth of channelled divine energy leaves you.");
        break;
    case SICKNESS:
        if (ending)
            IPrint("You feel much better now.");
        else
            IPrint("You feel sick.");
        break;
    case BLIND:
        if (!ending)
            IDPrint("The world goes dark!",
            "The <Obj> is struck blind!",this);
        else
            IDPrint("You can see again.",
            "The <Obj> seems to be able to see again.",this);
        break;
        /*case HALLU:
        if (ending)
        IPrint("Whoa! Check out this funky... everything!");
        else
        IPrint("The world seems more mundane, now.");
        break;*/
    case STUNNED:
        if (!ending)
            IDPrint("You reel...","The <Obj> reels...",this);
        else
            IDPrint("Your senses come back to you.",
            "The <Obj> is no longer stunned.",this);
        break;
    case CONFUSED:
        if (!ending)
            IDPrint("You suddenly feel very dizzy...",
            "The <Obj> staggers...",this);
        else
            IDPrint("You seem a bit steadier now.",
            "The <Obj> seems a bit steadier now.",this);
        break;
    case NAUSEA: 
        if (ending) 
            IDPrint("You feel less nauseated now.","The <Obj> seems less nauseated now.",this);
        else 
            IDPrint("You feel nauseated.","The <Obj> seems nauseated.",this);
        break; 
    case SLEEPING:
        if (!ending)
            IDPrint("The world fades into darkness...",
            "The <Obj> collapses into slumber!",this);
        else {
            IDPrint("You wake up.",
                "The <Obj> wakes up.",this);
            // ww: otherwise it looks bad 
            if (isPlayer())
                thisp->UpdateMap = true; 
        }
        break;
    case PARALYSIS:
        if (!ending)
            IDPrint("You find yourself unable to move!",
            "The <Obj> stops moving!",this);
        else
            IDPrint("You can move again.",
            "The <Obj> shakes off the paralysis.",this);
        break;
    case AFRAID:
        if (val == FEAR_SKIRMISH)
            break;
        if (!ending)
            IDPrint("You are overcome with terror...",
            "The <Obj> seems terrified.",this);
        else
            IDPrint("Your courage is returning to you now.",
            "The <Obj> shakes off the fear.",this);
        break;
    case STUCK:
        if (!ending)
            IDPrint("You've become stuck!",
            "The <Obj> seems to be stuck.",this);
        else
            IDPrint("You tear free.",
            "The <Obj> tears free.",this);
        break;
    case ILLUS_DMG:
        if (ending)
            IPrint("You realize that <str>your injuries are psychosomatic!",
            cHP >= mHP + GetAttr(A_THP) ? "" : "some of ");
        break;
    case BLEEDING:
        if (!ending)
            IDPrint("You're bleeding!",
            "The <Obj> is bleeding!",this);
        else
            IDPrint("You stop bleeding.",
            "The <Obj> stops bleeding.",this);
        break;
    case CHOKING:
        if (!ending)
            IDPrint("You start to choke!",
            "The <Obj> starts choking!",this);
        else
            IDPrint("You can breathe again.",
            "The <Obj> stops choking.",this);
        break;
    case PRONE:
        if (!ending)
            IDPrint("You are knocked from your feet!",
            "The <Obj1> is knocked from <his:Obj1> feet!",this);
        else
            IDPrint("You stand up.",
            "The <Obj> stands up.",this);
        break;
    case EXPOSED:
        break;
    case STONING:
        if (!ending)
            IPrint("You feel your joints stiffen!");
        else
            IPrint("You feel more limber now.");
        break;
    case DISEASED:
        if (!ending)
            IDPrint("You feel unclean!",
            "The <Obj> looks feverish.", this);
        else
            IPrint("You've overcome the disease.");
        break;
    case POISONED:
        if (!ending)
            IDPrint("You've been poisoned!",
            "The <Obj> shivers.",this);
        else
            IPrint("You've overcome the poison.");
        break;
    case HIDING:
        if (!ending && val == HI_SHADOWS)
            IDPrint("You slip into the shadows.",
            "The <Obj> slips into the shadows.",this);
        else if (!ending && val == HI_CEILING)
            IDPrint("You fly up to the ceiling.",
            "The <Obj> flies up to the ceiling.",this);
        else if (!ending && val == HI_UNDER)
            IDPrint("You slip under cover.",
            "The <Obj> slips under cover.",this);
        else if (!ending && val == HI_WATER)
            IDPrint("You slip beneath the surface of the water.",
            "The <Obj> slips beneath the surface of the water.",this);
        /*else if (ending)
        IDPrint("You're visible again.",
        "The <Obj> appears.",this);*/
        break;
    case RAGING:
        if (!ending)
            IDPrint("You fly into a berzerk rage!",
            "The <Obj> flies into a berzerk rage!",this);
        else
            IPrint("The rage leaves you.");
        break;
    case INVIS:
        if (!ending)
            IDPrint("You can't see yourself anymore!",
            "The <Obj> vanishes.",this);
        else
            IDPrint("You fade back into visibility.",
            "An <Obj> appears!",this);
        break;
    case INVIS_TO:
        /* Complex. Do this later. */
        break;
        //case PHASED:
    case REGEN:
        if (cHP == (mHP + Attr[A_THP]))
            break;
        if (!ending)
            IPrint("Your wounds slowly start healing!");
        else
            IPrint("Your rapid healing ceases.");
        break;
    case LEVITATION:
        if (!ending)
            IDPrint("You rise up off the ground and float!",
            "The <Obj> rises off the ground and floats in the air!",this);
        else
            IDPrint("You float down to the ground.",
            "The <Obj> floats back down.",this);
        break;
        /*se TELEPATHIC:
        if (!ending)
        IPrint("You can sense the presence of minds!");
        else
        IPrint("Your telepathy fades.");
        break;*/
    case WATER_WALKING:
        if (!(m && TTER(m->TerrainAt(x,y))->HasFlag(TF_WATER)))
            break;
        if (!ending)
            IPrint("You rise up to stand on the surface of the water!");
        else
            IPrint("You sink into the water.");
        break;
    case DETECTING:     
        if (ending) {
            IPrint(Format("Your detection ability for %s expires.",
                val ? (const char*)Pluralize(Lookup(MTypeNames,val),0) 
                : "monsters"));
        }
        break;     
    case ENRAGED:
        if (!ending)
            IDPrint("You feel fury rise in your heart!",
            "The <Obj> is overcome with bloodlust!",this);
        else
            IDPrint("Your bloodlust seems to have faded.",
            "The <Obj>'s bloodlust seems to have faded.",this);
        break;
    case TOUCH_ATTACK:
        if (ending)
            IDPrint("Your hands stop glowing.",
            "The <Obj>'s hands stop glowing.",this);
        break;       
    case SUMMONED:
    case ILLUSION:
        if (ending)
            IDPrint(NULL,"The <Obj> winks out of existence.",this);
        break;
    }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

void Map::UpdateFields() {
    Field *f; int16 i;
    for(i=0;Fields[i];i++) {
        /* Avoid permanent and persistent fields, whose duration is constant and negative. */
        if (Fields[i]->Dur > 1)
            Fields[i]->Dur--;
    }

StartAgain:
    for(i=0;Fields[i];i++)
        if (Fields[i]->Dur == 1) {
            RemoveField(Fields[i]);
            goto StartAgain;
        }
}

void Map::RemoveField(Field *f) {
    Thing *t;
    Field cf = *f;
    int16 ix,iy,i,j;
    for(i=0;Fields[i];i++)
        if (f == Fields[i]) {
            Fields.Remove(i);

            for(ix = max(0,cf.cx - cf.rad);ix!=min(sizeX,cf.cx + cf.rad + 1);ix++)
                for(iy = max(0,cf.cy - cf.rad);iy!=min(sizeY,cf.cy + cf.rad + 1);iy++)
                    if (InBounds(ix,iy)) {
                        At(ix,iy).hasField = FieldAt(ix,iy);
                        if (cf.FType & FI_DARKNESS)
                            At(ix,iy).Dark = FieldAt(ix,iy,FI_DARKNESS);
                        if (cf.FType & FI_LIGHT)
                            At(ix,iy).mLight = FieldAt(ix,iy,FI_LIGHT);
                        if (cf.FType & FI_FOG)
                            At(ix,iy).mObscure = FieldAt(ix,iy,FI_FOG);
                    }

            VUpdate(cf.cx,cf.cy);
            MapIterate(this,t,j)
                if (cf.inArea(t->x,t->y)) {
                    ThrowField(EV_FIELDOFF,&cf,t);
                }
            return;
        }
        Error("Map::RemoveField could not find the field!");
}

void Map::RemoveEffField(rID eID) {
    int16 i;
StartSearch:
    for(i=0;Fields[i];i++)
        if (Fields[i]->eID == eID) {
            RemoveField(Fields[i]);
            goto StartSearch;
        }
}

void Map::RemoveFieldFrom(hObj h) {
    int16 i;
StartSearch:
    for(i=0;Fields[i];i++)
        if (Fields[i]->Creator == h) {
            RemoveField(Fields[i]);
            goto StartSearch;
        }
}

void Map::RemoveEffFieldFrom(rID eID, hObj h)
{
    int16 i;
StartSearch:
    for(i=0;Fields[i];i++)
        if (Fields[i]->Creator == h && Fields[i]->eID == eID) {
            RemoveField(Fields[i]);
            goto StartSearch;
        }
}

void Map::NewField(int32 FType, int16 x, int16 y, uint8 rad,Glyph Img, int16 Dur, rID eID, Creature *Creator) {
    uint8 ix, iy; int16 i; Thing *t;
    Field *f = Fields.NewItem();
    ASSERT(f)

    f->FType = FType;
    f->cx = x; f->cy = y;
    f->rad = rad;
    f->Image = Img;
    f->eID = eID;
    f->Dur = Dur;
    f->Next = 0;
    if (Creator)
        f->Creator = Creator->myHandle;
    else
        f->Creator = 0;

    for(ix = max(0,x - rad);ix!=min(sizeX,x + rad + 1);ix++)
        for(iy = max(0,y - rad);iy!=min(sizeY,y + rad + 1);iy++)
            if (f->inArea(ix,iy)) {
                if (FType & FI_DARKNESS)
                    At(ix,iy).Dark = true;
                if (FType & FI_LIGHT)
                    At(ix,iy).mLight = true;
                if (FType & FI_FOG)
                    At(ix,iy).mObscure = true;
                At(ix,iy).hasField = true;
            }

    VUpdate(f->cx,f->cy);

    MapIterate(this,t,i)
        if (f->inArea(t->x,t->y))
            ThrowField(EV_FIELDON,f,t);
}

uint8 Field::FieldDC()
{
    if (!Creator || !eID)
        return 15;
    /*if (TEFF(eID)->SType & (ARCANE | DIVINE | PRIMAL))
    return 10 + TEFF(eID)->Level + (TEFF(eID)->SType & ARCANE ?
    Creator->Mod(A_INT) : Creator->Mod(A_WIS));*/
    return 10 + TEFF(eID)->Level;
}
  
bool Thing::HasEffField(rID eID)
{
    int16 i; Field *f;
    if (!m)
        return false;
    for (i=0;f = m->Fields[i];i++)
        if (f->Creator == myHandle && f->eID == eID)
            return true;
    return false;
}
  
bool Map::DispelField(int16 x, int16 y, uint32 FType, rID eID, int16 clev)
{
    int16 i; Field *f; bool found;
    found = false;
Restart:
    for (i=0;f=Fields[i];i++)
        if (dist(x,y,f->cx,f->cy) <= f->rad)
            if ((!FType) || (f->FType & FType))
                if ((!eID) || (f->eID == eID))
                    if ((!clev) || (!f->Creator) || !oThing(f->Creator)->isCreature() || clev > oCreature(f->Creator)->CasterLev()) {
                        found = true;
                        RemoveField(f);
                        goto Restart;
                    }
                    return found;
}

EvReturn Map::MoveField(Field *f,int16 _cx, int16 _cy, bool is_walk) {
    int16 i; Thing *t; uint8 lc=0, ec=0; uint16 ix,iy,ox,oy, n;
    static Thing *Leaving[128], *Entering[128];

Restart:

    if (!f->Creator || (!f->eID && !(f->FType & FI_SIZE)))
        return ABORT;

    if (At(_cx,_cy).Visibility)
        theGame->GetPlayer(0)->UpdateMap = true;

    /* We need to confirm we're not moving a size field somewhere that
    it can't fit -- i.e., a Gargantuan red dragon trying to squeeze
    into a 10' wide corridor. We'll give the creature some credit
    here, and allow it to merge up to (radius * 3) squares of its
    size field into something solid, as long as those squares are
    along the outer edge of the field. This allows a dragon to stick
    its head into a corridor, but not its entire bulk. */
    Thing * blocked_by = NULL; 
    if (f->FType & FI_SIZE) {
        Creature * me = oCreature(f->Creator);
        bool hasMeld = me->HasAbility(CA_EARTHMELD);
        ASSERT(me); 
        n = 0;
        for(ix = max(0,_cx - f->rad);ix!=min(sizeX,_cx + f->rad + 1);ix++)
            for(iy = max(0,_cy - f->rad);iy!=min(sizeY,_cy + f->rad + 1);iy++)
                if ((dist(ix,iy,_cx,_cy) <= f->rad) && !(me->canMoveThrough(NULL,ix,iy,blocked_by))) {
                    if (dist(_cx,_cy,ix,iy) == f->rad) {
                        if (blocked_by && blocked_by->isCreature()) {
                            me->IPrint("You cannot move through the <Obj>.",blocked_by);
                            return ABORT; 
                        } 
                        n++;
                    } else {
                        // ww: let's give the players a clue
                        if (blocked_by) 
                            me->IPrint("You cannot move through the <Obj>.",blocked_by);
                        else 
                            me->IPrint("Your bulk would not fit there.");
                        return ABORT;
                    }
                }
        if (n > f->rad * 3) {
            if (hasMeld)
                me->IPrint("You move through the stone.");
            else {
                me->IPrint("You cannot squeeze into a space that small.");
                return ABORT;
            }
        } else if (n > 0) 
            me->IPrint("You squeeze in to a small space.");
    }

    MapIterate(this,t,i) {
        if (!t->isCreature())
            continue;
        /* Avoid brief exit-enter sequence when a player with a field around them teleports. */
        if (t->myHandle == f->Creator)
            continue;
        if (dist(t->x,t->y,f->cx,f->cy) <= f->rad && dist(t->x,t->y,_cx,_cy) > f->rad)
            Leaving[lc++] = t;
        if (dist(t->x,t->y,f->cx,f->cy) > f->rad && dist(t->x,t->y,_cx,_cy) <= f->rad)
            Entering[ec++] = t;
    }

    if (ec && f->eID && TEFF(f->eID)->HasFlag(EF_NO_PRESS)) {
        EventInfo xe;
        xe.Clear();
        xe.eID = f->eID;
        xe.EActor = oCreature(f->Creator);
        for (i=0;i!=ec;i++) {
            if (!Entering[i]->isCreature())
                continue;
            xe.ETarget = Entering[i];
            if (xe.EActor->isTarget(xe,Entering[i])) {
                if ((!is_walk) || xe.EActor->yn(XPrint("You feel pressure against the <9><Res><5>. Keep going?",xe.eID))) {
                    xe.EActor->IPrint("Your <Res> collapses.",xe.eID);
                    RemoveField(f);
                    goto Restart;
                } else
                    return ABORT;
            }
        }
    }

    /* Set up the coordinates... */
    ox = f->cx; oy = f->cy;
    f->cx = _cx; f->cy = _cy;

    /* Remove the old field markers... */
    for(ix = max(0,ox - f->rad);ix!=min(sizeX,ox + f->rad + 1);ix++)
        for(iy = max(0,oy - f->rad);iy!=min(sizeY,oy + f->rad + 1);iy++)
            if (InBounds(ix,iy)) {
                At(ix,iy).hasField = FieldAt(ix,iy);
                if (f->FType & FI_DARKNESS)
                    At(ix,iy).Dark = FieldAt(ix,iy,FI_DARKNESS);
                if (f->FType & FI_LIGHT)
                    At(ix,iy).mLight = FieldAt(ix,iy,FI_LIGHT);
                if (f->FType & FI_FOG)
                    At(ix,iy).mObscure = FieldAt(ix,iy,FI_FOG);
            }

    /* And establish new markers. */
    for(ix = max(0,f->cx - f->rad);ix!=min(sizeX,f->cx + f->rad + 1);ix++)
        for(iy = max(0,f->cy - f->rad);iy!=min(sizeY,f->cy + f->rad + 1);iy++)
            if (f->inArea(ix,iy)) {
                if (f->FType & FI_DARKNESS)
                    At(ix,iy).Dark = true;
                if (f->FType & FI_LIGHT)
                    At(ix,iy).mLight = true;
                if (f->FType & FI_FOG)
                    At(ix,iy).mObscure = true;
                At(ix,iy).hasField = true;
            }

    /* Let everybody know what happened */
    for(i=0;i!=lc;i++)
        ThrowField(EV_FIELDOFF,f,Leaving[i]);
    for(i=0;i!=ec;i++)
        ThrowField(EV_FIELDON,f,Entering[i]);

    return DONE;
}

rID Map::PTerrainAt(int16 x, int16 y, Creature *watcher) {
    int16 i;
    Feature *f;

    for (f=FFeatureAt(x,y);f;f=NFeatureAt(x,y))
        if (f->isType(T_DOOR))
            if (((Door*)f)->DoorFlags & DF_SECRET)
                return (((Door*)f)->DoorFlags & DF_VERTICAL) ? TerrainAt(x,y+1) : TerrainAt(x+1,y);    

    if (!At(x,y).hasField)
        return TerrainAt(x,y);

    for (i=0;Fields[i];i++)
        if (Fields[i]->FType & FI_ITERRAIN)
            if (Fields[i]->inArea(x,y)) {
                EvReturn r; EventInfo *pe;
                if (!watcher)
                    goto noWatcher;
                if (watcher->HasStati(DISB_TERRAIN,-1, oCreature(Fields[i]->Creator)))
                    continue;
                if (!(TEFF(Fields[i]->eID)->ef.yval & IL_IMPROVED))
                    if (watcher->HasAbility(CA_BLINDSIGHT) ||
                        watcher->HasAbility(CA_SHARP_SENSES) ||
                        watcher->HasAbility(CA_TREMORSENSE) ||
                        watcher->HasAbility(CA_SCENT))
                        continue;
                if (watcher->HasStati(TRUE_SIGHT))
                    continue;
                if (watcher->HasMFlag(M_MINDLESS))
                    continue;
                if (watcher->myHandle == Fields[i]->Creator)
                    continue;

noWatcher:
                PEVENT(EV_ITERRAIN,
                    oCreature(Fields[i]->Creator),
                    Fields[i]->eID,e.EXVal = x; e.EYVal = y; pe = &e,r);
                return r == ABORT ? TerrainAt(x,y) : pe->eID;
            }

    return TerrainAt(x,y);          
}

EvReturn Creature::FieldOn(EventInfo &e) {
    for(int8 i=1;i!=17;i++)
        if (BIT(i) & e.EField->FType)
            switch(BIT(i)) {              
            case FI_LIGHT:
                if (HasStati(HIDING)) {
                    RemoveStati(HIDING);
                    DPrint(e,"You are exposed!",
                        "The light reveals an <EActor>, hidden in the shadows!");
                }
                if (HasMFlag(M_LIGHT_AVERSE))
                    DPrint(e,"You squint and stagger in the bright light.",
                    "The <EActor> squints and staggers in the bright light.");              
                break;
            case FI_SILENCE:
                IPrint("Unearthly silence envelops you!");
                RemoveStati(SINGING);
                break;
            case FI_DARKNESS:
                if (!isBlind())
                    IPrint("Unearthly darkness envelops you!");
                break;
            case FI_SHADOW:
                if (!isBlind())
                    IPrint("You enter a globe of pervasive shadows.");
                break;
            case FI_MODIFIER:
                EventInfo e2;
                e2.Clear();
                e2.eID = e.EField->eID;
                e2.isEnter = true;
                e2.EVictim = this;
                e2.EActor  = oCreature(e.EField->Creator);
                e2.Terse = e.Terse;
                ReThrow(EV_EFFECT,e2);
                break;

        }
        return DONE;
}

EvReturn Creature::FieldOff(EventInfo &e) {
    for(int8 i=1;i!=17;i++)
        if (BIT(i) & e.EField->FType)
            switch(BIT(i)) {              
            case FI_SILENCE:
                IPrint("Sounds return to the universe.");
                break;
            case FI_DARKNESS:
                if (!isBlind())
                    IPrint("You can see again.");
                break;
            case FI_SHADOW:
                if (!isBlind())
                    IPrint("You leave the unnaturally shadowed region.");
                break;
                /*case FI_WEBBING:
                if (HasStati(STUCK) && !inField(FI_WEBBING))
                RemoveStati(STUCK,SS_ATTK);
                if (Type == T_PLAYER)
                thisp->ShowStats();
                break;*/
            case FI_MODIFIER:
                EventInfo e2;
                e2.Clear();
                e2.eID = e.EField->eID;
                e2.isLeave = true;
                e2.EVictim = this;
                if (theRegistry->Exists(e.EField->Creator))
                    e2.EActor  = oCreature(e.EField->Creator);
                else 
                    return DONE; 
                e2.Terse = e.Terse;
                ReThrow(EV_EFFECT,e2);
                break;               
        }
    return DONE;
}


