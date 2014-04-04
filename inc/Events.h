/* EVENTS.H -- Copyright (c) 1999-2003 Julian Mensch
     This file contains class definitions for EventInfo,
   and certain other definitions and macros relating to
   Incursion's event system.
*/



union EvParam {
    Thing *t;
    Item *i;
    Creature *c;
    Player *pl;
    Attack *at;
    Feature *f;
    Map   *m;
    Object *o;
    Weapon *wp;
};

#define EMatch(th,typ)\
	((th->Type==typ) || (typ==T_ITEM && th->isItem()) || \
    (typ==T_CREATURE && th->isCreature()))

#define EVerify(p1,p2,p3,p4)                             \
	{                                                      \
		if(p1)                                               \
			{                                                  \
				if(!e.p[0].t)                                    \
					Fatal("Unexpected NULL Event Parameter!");     \
				else                                             \
					if(!EMatch(e.p[0].t,p1))                       \
						Fatal("Incorrect Parameter type");           \
			}                                                  \
																												 \
		if(p2)                                               \
			{                                                  \
				if(!e.p[1].t)                                    \
					Fatal("Unexpected NULL Event Parameter!");     \
				else                                             \
					if(!EMatch(e.p[1].t,p2))                       \
						Fatal("Incorrect Parameter type");           \
			}                                                  \
																												 \
		if(p3)                                               \
			{                                                  \
				if(!e.p[2].t)                                    \
					Fatal("Unexpected NULL Event Parameter!");     \
				else                                             \
					if(!EMatch(e.p[2].t,p3))                       \
						Fatal("Incorrect Parameter type");           \
			}                                                  \
																												 \
		if(p4)                                               \
			{                                                  \
				if(!e.p[3].t)                                    \
					Fatal("Unexpected NULL Event Parameter!");     \
				else                                             \
					if(!EMatch(e.p[3].t,p4))                       \
						Fatal("Incorrect Parameter type");           \
			}                                                  \
	}

#define PEVENT(ev, actor, xID, set_val, r)               \
  {                                                      \
    EventInfo e;                                         \
    e.Clear();                                           \
    e.Event = ev;                                        \
    e.EActor = (Creature*)actor;                         \
    e.EVictim = (Creature*)actor;                        \
    e.EMap = e.EActor ? e.EActor->m :                    \
             e.EVictim ? e.EVictim->m :                  \
             NULL;                                       \
    set_val;                                             \
    if (actor) {                                         \
      e.EXVal = actor->x;                                \
      e.EXVal = actor->y;                                \
      }                                                  \
    r = TEFF(xID)->Event(e,xID,0);                       \
  }

#define DAMAGE(act, vic, dtype, amt, str, set_val)       \
  {                                                      \
    EventInfo xe;                                        \
    xe.Clear();                                          \
    xe.Event = EV_DAMAGE;                                \
    xe.EActor = (Creature*)act;                          \
    xe.EVictim = (Creature*)vic;                         \
    xe.DType = dtype;                                    \
    xe.vDmg = amt;                                       \
    xe.GraveText = str;                                  \
    xe.EMap = xe.EActor ? xe.EActor->m :                 \
             xe.EVictim ? xe.EVictim->m :                \
             NULL;                                       \
    set_val;                                             \
    if (act) {                                           \
      xe.EXVal = act->x;                                 \
      xe.EXVal = act->y;                                 \
      }                                                  \
    ReThrow(EV_DAMAGE,xe);                               \
  }

#define XTHROW(ev, e, set_val)  \
  {                             \
    EventInfo xe;               \
    xe.Clear();                 \
    xe = e;                     \
    set_val;                    \
    ReThrow(ev,xe);             \
    e.chResult = xe.chResult;   \
  }

#define RXTHROW(ev, e, r, set_val)  \
  {                             \
    EventInfo xe;               \
    xe.Clear();                 \
    xe = e;                     \
    set_val;                    \
    r = ReThrow(ev,xe);             \
    e.chResult = xe.chResult;   \
  }

#define THROW(ev, set_val)       \
  {                              \
    EventInfo xe;                \
    xe.Clear();                  \
    set_val;                     \
    ReThrow(ev, xe);             \
  }

struct EventInfo {
    /* We do some ugly tricks here to make copying EventInfo
       structures quick. They depend on several things that
       might later change -- that String has no virtual members,
       that the compiler puts all the struct members in memory
       in the same order we list them, and that a String that
       is memset(0)ed is effectively an empty string. All of
       these assumptions hold up... for now. It's important
       that this be noted, though, in case any of this ever
       changes. */
    EventInfo& operator=(EventInfo &e) { /* Memory image copy for the int/bool/ptr members */
        memcpy(this,&e,sizeof(EventInfo));
        /* Blank out the Strings using memset, as they contain
        pointers to string buffers that properly belong to
        the other EventInfo's Strings. */
        memset(&GraveText,0,sizeof(String)*24);
        /* Copy the Strings properly */
        GraveText = e.GraveText;
        strDmg    = e.strDmg;
        strXDmg   = e.strXDmg;
        strHit    = e.strHit;
        strDef    = e.strDef;
        strOpp1   = e.strOpp1;
        strOpp2   = e.strOpp2;
        strBlastDmg = e.strBlastDmg;

        nPrefix    = e.nPrefix;
        nCursed    = e.nCursed;
        nPrequal   = e.nPrequal;
        nPostqual  = e.nPostqual;
        nNamed     = e.nNamed;
        nBase      = e.nBase;
        nAppend    = e.nAppend;
        nOf        = e.nOf;
        nAdjective = e.nAdjective;
        nFlavor    = e.nFlavor;
        nInscrip   = e.nInscrip;
        nMech      = e.nMech;
        nArticle   = e.nArticle;
        nPlus      = e.nPlus;
        enDump     = e.enDump;
        Text       = e.Text;
        // ww: what about nPrefix, etc.? 
        return e;
    }
    void inline Clear() {
        GraveText = NULL; strDmg  = NULL;
        strXDmg   = NULL; strHit  = NULL;
        strDef    = NULL; strOpp1 = NULL;
        strOpp2   = NULL; strBlastDmg = NULL;

        nPrefix   = NULL; nCursed = NULL;
        nPrequal  = NULL; nPostqual = NULL;
        nNamed    = NULL; nBase = NULL;
        nAppend   = NULL; nOf = NULL;
        nAdjective= NULL; nFlavor = NULL;
        nInscrip  = NULL; nMech = NULL;
        nArticle  = NULL; nPlus = NULL;
        Text      = NULL; enDump = NULL;
        /* Now that no Strings have non-NULL pointers
        within them to leave behind dangling strdup
        blocks, we can safely use memset. */
        memset(this,0,sizeof(EventInfo));
    }
    void SetParam(int16 n,Thing *t) { p[n].t = t; }

    uint16 Event;
    EvParam p[5];
    Field *EField;
    Map   *EMap;
#define EActor      p[0].c
#define EPActor     p[0].pl
#define EItem       p[2].i
#define EWeapon     p[2].wp
#define ETarget     p[1].t
#define EVictim     p[1].c
#define EPVictim    p[1].pl
#define EItem1      p[2].i
#define EItem2      p[3].i
#define EFeat       p[1].f
#define EMisc       p[3]
#define EDir        z
#define EXVal       x
#define EYVal       y
#define EString     s
#define ESpellNum   sp
    int16  x,y,z,sp;
    int16  EvFlags; int32 EParam, EParam2;
    Item *remainingAmmo;
    int8 vRoll, vtRoll, AType, DType, vHit, vDef, vThreat, vCrit, vArm, vPen,
        efNum, saveDC, vRange, vRadius, vCasterLev, vOpp1, vOpp2, vRideCheck,
        vChainCount, vChainMax, vPenetrateBonus, vAlchemy, modStr,
        Transmute;
    int16 vDmg, bDmg, aDmg, xDmg, vMult, vDuration; Dice Dmg; uint32 MM;
    int16 vDepth, vLevel, vAlign, vDefRoll;
    int16 terraKey, terrainListIndex;
    uint16 illFlags; char illType; int16 godNum;
    rID eID, ill_eID;
    bool  isHit, isCrit, isFumble, Died, ADied, Blocked, Saved, Immune, MagicRes,
        Resist, Absorb, wasFriendly, actUnseen, vicUnseen, vicCharging, Whirlwind, Graze,
        Stun, actIncor, vicIncor, Ranged, vicHeld, isDir, isWImmune, isTelekinetic,
        isLoc, isActivation, isSomething, isWield, isRemove, isEnter, effDisbelieved, 
        isLeave, isPeriodic, isProper, actIllusion, vicIllusion, effIllusion, 
        isAoO, isCleave, isSurprise, isFirstBlastXY, isRepeatSpell, isStaffSpell,
        isBlessed, isCursed, isConf, isHallu, isItem, isTrap, isSpell, isNAttack,
        isGreatBlow, isMoveAoO, isWildMiss, isBypass, isFlatFoot, isFlanking, isDefRoll,
        isGhostTouch, isSneakAttack, isSeeking, isOffhand, doVUpdate, notFullyImmune, 
        MHMessageDone, BlastMessageDone, isEvaded, isPartiallyEvaded, isArcaneTrickery,
        isActOfGod, ignoreHardness, halfHardness, isVerb, subStr, isPrecision,
        isSharedSpell, isCounterTrip, isAllAllies,
        vicReady,
        vicNotFleeing;
    bool Silence,  /* Don't print *anything*! */
        Terse;    /* Don't print anything other than the
                  unexpected/ancilliary elements. */
    EffectValues *EMagic; hObj vObj; int32 vMag, vVal;
    String GraveText, strDmg, strXDmg, strHit, strDef, strOpp1, strOpp2, strBlastDmg;    

    /* In the future, these will offer support for custom construction
    of item or monster names in events. Perhaps these should be
    #defined to be synonyms of the above? */
    String nPrefix, nCursed, nPrequal, nPostqual, nNamed, nBase, 
        nAppend, nOf, nAdjective, nFlavor, nInscrip, nMech,
        nArticle, nPlus, Text, enDump;

    Rect cPanel, cMap, cRoom;

    /* Encounter Gen stuff */
    uint32 enTerrain;
    int16 cPart, 
        cMember,
        enAlign,
        enCR,
        enDepth,
        enFreaky,
        enPurpose,
        enSleep,
        enDesAmt,
        enType,
        enPartyID,
        enDriftGE,
        enDriftLC,
        epMinAmt, 
        epMaxAmt,
        epAmt,
        epFreaky, 
        epWeight,
        epMType,
        ep_monCR,
        ep_mountCR,
        epSkillRoll,
        epClassRoll,
        epCurrXCR,
        epTries,
        enTries;
    rID   enID,
        enRegID,
        ep_mID,
        ep_tID,
        ep_tID2,
        ep_tID3,
        ep_hmID,
        ep_htID,
        ep_htID2,
        ep_pID,
        ep_iID;
    int32 enXCR,  // Total XCR of the entire Encounter
        epXCR,  // Total XCR of the current Part
        eimXCR,  // Individual XCR of one monster in current Part
        enConstraint;
    bool  enSkipPart[MAX_PARTS],
        enIsFormation,
        epFailed,
        isGetMinCR,
        isGetMaxCR,
        isAquaticContext;
    uint32 enFlags;
    uint32 chType;
    uint16 chList;
    bool   chMaximize,
        chBestOfTwo;
    rID    chResult,
        chSource;
    bool (*chCriteria)(EventInfo &e, rID tID);

};








