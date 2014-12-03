/* VALUES.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Contains all the code for calculating a Creature's attributes
   and associated values, like resist levels, hit points, monster
   groupings and hostilities. For players, will calculate either
   the known or actual versions of these values on request.

     void Creature::CalcValues(bool KnownOnly)
     void Character::CalcValues(bool KnownOnly)
     int16 Creature::ResistLevel(int16 DType, bool bypass_armour)
     int8 MonHDType(int8 MType)
     int8 MonGoodSaves(int8 MType)
     void Creature::CalcHP()
     int16 Creature::GetPower(int16 base)
     bool Creature::isHostileTo(Creature*c)
     bool Creature::isFriendlyTo(Creature*c)
     bool TMonster::isMType(int8 mt) 
     bool Creature::isMType(int8 mt) 
     String Player::BonusBreakdown(int8 at, int16 maxlen)
     String Player::SkillBreakdown(int8 sk, int16 maxlen)
     String Player::SpellBreakdown(int8 sp, int16 maxlen)
     String Player::ResistBreakdown(int8 dt, int16 maxlen)

*/

/*   There is a known issue here with how *penalties* stack
   (currently, they always do, even if from the same source)
   and the fact that this leads to two slow traps making a
   PC unable to move.
     The temporary solution was to make PCs unable to be
   halted, but it doesn't solve the issue: that penalties
   from two instances of the same eID really shouldn't stack,
   but do -- but we don't store eID in AttrAdj[][], so this
   will take some refactoring to get working truly right.
   
     For RotF, fix this.
     
     Later, we need two seperate sets of the AttrAdj array --
   AttrBon and AttrPen -- each of which will hold the largest
   attribute modifier in each category. Most penalties stack,
   but few bonuses. 


*/

#include "Incursion.h"

#define A  (*((int16*)a))
#define B  (*((int16*)b))
int compare_int16(const void *a, const void *b) { return B>A ? -1 : (A>B ? 1 : 0); }

int16 concentTotal;

void Creature::StackBonus(int8 btype, int8 attr,int16 bonus) { 
    if (attr == A_MAG && bonus < 0 && concentUsed < concentTotal)
        if (btype != BONUS_TACTIC && btype != BONUS_ARMOUR && btype != BONUS_SHIELD) {
            if (concentUsed + (-bonus) <= concentTotal) {
                concentUsed += (-bonus);
                return;
            }

            bonus += (concentTotal - concentUsed);
            concentUsed = (int8)concentTotal;
        }

    switch (attr) {
    case A_HIT:                               
        AttrAdj[A_HIT_BRAWL][btype] += bonus;
        AttrAdj[A_HIT_MELEE][btype] += bonus;
        AttrAdj[A_HIT_OFFHAND][btype] += bonus;
        AttrAdj[A_HIT_ARCHERY][btype] += bonus;
        AttrAdj[A_HIT_THROWN][btype] += bonus;
        break;
    case A_DMG:          
        AttrAdj[A_DMG_BRAWL][btype] += bonus;          
        AttrAdj[A_DMG_MELEE][btype] += bonus;          
        AttrAdj[A_DMG_OFFHAND][btype] += bonus;          
        AttrAdj[A_DMG_ARCHERY][btype] += bonus;          
        AttrAdj[A_DMG_THROWN][btype] += bonus;          
        break;
    case A_SPD:
        AttrAdj[A_SPD_BRAWL][btype] += bonus;          
        AttrAdj[A_SPD_MELEE][btype] += bonus;          
        AttrAdj[A_SPD_OFFHAND][btype] += bonus;          
        AttrAdj[A_SPD_ARCHERY][btype] += bonus;          
        AttrAdj[A_SPD_THROWN][btype] += bonus;          
        break;
    case A_AID:
        AttrAdj[A_HIT_BRAWL][btype] += bonus;          
        AttrAdj[A_HIT_MELEE][btype] += bonus;          
        AttrAdj[A_HIT_OFFHAND][btype] += bonus;          
        AttrAdj[A_HIT_ARCHERY][btype] += bonus;          
        AttrAdj[A_HIT_THROWN][btype] += bonus;          

        AttrAdj[A_DEF][btype] += bonus;                  
        AttrAdj[A_SAV_FORT][btype] += bonus;             
        AttrAdj[A_SAV_REF ][btype] += bonus;             
        AttrAdj[A_SAV_WILL][btype] += bonus;             
        AttrAdj[A_ARC][btype] += bonus;                  
        AttrAdj[A_DIV][btype] += bonus;                  
        AttrAdj[A_SOR][btype] += bonus;
        AttrAdj[A_PRI][btype] += bonus;
        AttrAdj[A_BAR][btype] += bonus;
        break;
    case A_SAV:
        AttrAdj[A_SAV_FORT][btype] += bonus;             
        AttrAdj[A_SAV_REF][btype] += bonus;             
        AttrAdj[A_SAV_WILL][btype] += bonus;    
        break;         
    case A_MAG:
        AttrAdj[A_ARC][btype] += bonus;                  
        AttrAdj[A_DIV][btype] += bonus;
        AttrAdj[A_SOR][btype] += bonus;
        AttrAdj[A_PRI][btype] += bonus;
        AttrAdj[A_BAR][btype] += bonus;
        break;                 
    default:
        ASSERT(attr <= ATTR_LAST)
        AttrAdj[attr][btype] += bonus; 
    }
}

void Creature::AddBonus(int8 btype,int8 attr,int16 bonus) {
    // ww: it seems like this would work, but in fact it doesn't. 
    // Imagine that you have 
    //   Spell1 = +3 STR, -3 DEX
    //   Spell2 = -3 STR, +3 DEX
    //
    // If you cast S1 and then S2, you end up with +0 STR, +3 DEX.
    // If you cast S2 and then S1, you end up with +3 STR, +0 DEX.
    // In the real game, this ends up being a problem with Speed of the
    // Wind, Persistance of the Waves, etc. From the player's perspective
    // the results are random. 
    //
    // The problem here is that adding the positive bonus just takes the
    // max, so max(3,-3) = 3 instead of foo(3,-3) = 0. 
    // Thus:
#define WESMAX(attr,bonus) ((attr < 0) ? attr + bonus : max(attr,bonus))
    // This doesn't completely fix it, because
    //  Spell1 = -4 STR
    //  Spell2 = +3 STR
    //  Spell3 = +2 STR
    // With all of these you'll end up with +1 or -1, depending on the
    // order, but it's better than before and I can't really fix it without
    // massive surgery.
    //
    // FIXME. 

    if ((bonus < 0) || btype == BONUS_DODGE || btype == BONUS_CIRC) {
        StackBonus(btype, attr, bonus);
        return;
    }

    switch (attr) {
    case A_HIT:
        AttrAdj[A_HIT_BRAWL][btype] = WESMAX(AttrAdj[A_HIT_BRAWL][btype], bonus);       
        AttrAdj[A_HIT_MELEE][btype] = WESMAX(AttrAdj[A_HIT_MELEE][btype], bonus);       
        AttrAdj[A_HIT_OFFHAND][btype] = WESMAX(AttrAdj[A_HIT_OFFHAND][btype], bonus);     
        AttrAdj[A_HIT_ARCHERY][btype] = WESMAX(AttrAdj[A_HIT_ARCHERY][btype], bonus);    
        AttrAdj[A_HIT_THROWN][btype] = WESMAX(AttrAdj[A_HIT_THROWN][btype], bonus); 
        break;        
    case A_DMG:
        AttrAdj[A_DMG_BRAWL][btype] = WESMAX(AttrAdj[A_DMG_BRAWL][btype], bonus);       
        AttrAdj[A_DMG_MELEE][btype] = WESMAX(AttrAdj[A_DMG_MELEE][btype], bonus);       
        AttrAdj[A_DMG_OFFHAND][btype] = WESMAX(AttrAdj[A_DMG_OFFHAND][btype], bonus);     
        AttrAdj[A_DMG_ARCHERY][btype] = WESMAX(AttrAdj[A_DMG_ARCHERY][btype], bonus);     
        AttrAdj[A_DMG_THROWN][btype] = WESMAX(AttrAdj[A_DMG_THROWN][btype], bonus);      
        break;
    case A_SPD:
        AttrAdj[A_SPD_BRAWL][btype] = WESMAX(AttrAdj[A_SPD_BRAWL][btype], bonus);       
        AttrAdj[A_SPD_MELEE][btype] = WESMAX(AttrAdj[A_SPD_MELEE][btype], bonus);       
        AttrAdj[A_SPD_OFFHAND][btype] = WESMAX(AttrAdj[A_SPD_OFFHAND][btype], bonus);     
        AttrAdj[A_SPD_ARCHERY][btype] = WESMAX(AttrAdj[A_SPD_ARCHERY][btype], bonus);     
        AttrAdj[A_SPD_THROWN][btype] = WESMAX(AttrAdj[A_SPD_THROWN][btype], bonus);      
        break;
    case A_AID:
        AttrAdj[A_HIT_BRAWL][btype] = WESMAX(AttrAdj[A_HIT_BRAWL][btype], bonus);       
        AttrAdj[A_HIT_MELEE][btype] = WESMAX(AttrAdj[A_HIT_MELEE][btype], bonus);       
        AttrAdj[A_HIT_OFFHAND][btype] = WESMAX(AttrAdj[A_HIT_OFFHAND][btype], bonus);     
        AttrAdj[A_HIT_ARCHERY][btype] = WESMAX(AttrAdj[A_HIT_ARCHERY][btype], bonus);     
        AttrAdj[A_HIT_THROWN][btype] = WESMAX(AttrAdj[A_HIT_THROWN][btype], bonus);      

        AttrAdj[A_DEF][btype] = WESMAX(AttrAdj[A_DEF][btype], bonus);             
        AttrAdj[A_SAV_FORT][btype] = WESMAX(AttrAdj[A_SAV_FORT][btype], bonus);        
        AttrAdj[A_SAV_REF][btype] = WESMAX(AttrAdj[A_SAV_REF][btype], bonus);         
        AttrAdj[A_SAV_WILL][btype] = WESMAX(AttrAdj[A_SAV_WILL][btype], bonus);        
        AttrAdj[A_ARC][btype] = WESMAX(AttrAdj[A_ARC][btype], bonus);             
        AttrAdj[A_DIV][btype] = WESMAX(AttrAdj[A_DIV][btype], bonus);             
        break;
    case A_SAV:
        AttrAdj[A_SAV_FORT][btype] = WESMAX(AttrAdj[A_SAV_FORT][btype], bonus);        
        AttrAdj[A_SAV_REF][btype] = WESMAX(AttrAdj[A_SAV_REF][btype], bonus);         
        AttrAdj[A_SAV_WILL][btype] = WESMAX(AttrAdj[A_SAV_WILL][btype], bonus);        
        break;
    case A_MAG:
        AttrAdj[A_ARC][btype] = WESMAX(AttrAdj[A_ARC][btype], bonus);             
        AttrAdj[A_DIV][btype] = WESMAX(AttrAdj[A_DIV][btype], bonus);             
        AttrAdj[A_PRI][btype] = WESMAX(AttrAdj[A_PRI][btype], bonus);             
        AttrAdj[A_SOR][btype] = WESMAX(AttrAdj[A_SOR][btype], bonus);             
        AttrAdj[A_BAR][btype] = WESMAX(AttrAdj[A_BAR][btype], bonus);             
        break;
    default:
        AttrAdj[attr][btype] = WESMAX(AttrAdj[attr][btype], bonus); 
    }
}

/*
at:
tm: 
*/
#define APPLY_TEMPLATE_MODIFIER(at,tm)                                                  \
    if (tm.VType == MVAL_SET) {                                                         \
        AttrAdj[at][BONUS_BASE] = (int8)tm.Value;                                       \
        AttrAdj[at][BONUS_TEMP] = 0;                                                    \
    } else if (tm.VType == MVAL_PERCENT && percent_attr(at))                            \
        AttrAdj[at][BONUS_TEMP] = (int8)(((((AttrAdj[at][BONUS_TEMP]*5+100L)*           \
                                  ((long)tm.Adjust(100))) / 100L) - 100) / 5);          \
     else                                                                               \
         AttrAdj[at][BONUS_TEMP] = tm.Adjust(AttrAdj[at][BONUS_BASE]                    \
       + AttrAdj[at][BONUS_TEMP]) - (AttrAdj[at][BONUS_BASE] /*+ AttrAdj[at][BONUS_TEMP]*/);  

#define NAT_ARMOUR_TEMPLATE_MOD(at,tm)                                                  \
   if (tm.VType == MVAL_SET) {                                                          \
       AttrAdj[at][BONUS_BASE] = tm.Value;                                              \
       AttrAdj[at][BONUS_TEMP] = 0;                                                     \
   } else                                                                               \
       AttrAdj[at][BONUS_TEMP] = tm.Adjust(TMON(mID)->Arm                               \
       + AttrAdj[at][BONUS_TEMP]) - (TMON(mID)->Arm /*+ AttrAdj[at][BONUS_TEMP]*/);  
                                                                                       

int8 MonGoodSaves(int8 MType);

int8 Creature::AttrAdj[ATTR_LAST][BONUS_LAST]; 
Item *Creature::missileWep, *Creature::thrownWep, *Creature::offhandWep, *Creature::meleeWep;

void Creature::CalcValues(bool KnownOnly, Item *thrown) {
#define XMod(a) (KnownOnly ? KMod(a) : Mod(a))
#define XMod2(a) (KnownOnly ? KMod2(a) : Mod2(a))
    int8 i;
    int16 j, tsav, oFP, oHP, oldSize;
    int32 restart_count = 0; 
    static int16 MRVals[BONUS_LAST], MRC;
    bool isHalted = false;
    Item *it;

    oldSize = Attr[A_SIZ];
    oHP  = cHP;
    cFP -= Attr[A_FAT];
    cHP -= Attr[A_THP];

    /* HACKFIX -- no recursive calcval looping */
    if (theGame->inCalcVal)
        return;
    theGame->inCalcVal++;

Restart:
    memset(AttrAdj, 0, sizeof(int8) * BONUS_LAST * ATTR_LAST);

    bool grappling = false; 

    oFP = Attr[A_FAT];

    concentUsed = 0;
    concentTotal = ConcentBuffer();

    ASSERT(isCharacter() || !KnownOnly);
    ASSERT(TMON(mID));
    /* Figure out our weapons. For monsters, we only need to
    account for the weapons immediately in their hands. For
    the player, setting all the static weapon variables is
    important so that, for example, a player still sees his
    character's archery stats on the character sheet when he
    has his bow slung over his shoulder. */


    meleeWep   = EInSlot(SL_WEAPON);
    offhandWep = EInSlot(SL_READY);
    missileWep = EInSlot(SL_ARCHERY);
    thrownWep  = thrown;
    if (meleeWep && meleeWep->isType(T_BOW)) {
        missileWep = meleeWep;
        meleeWep = offhandWep = NULL;
        if (it = EInSlot(SL_LSHOULDER))
            if (it->isType(T_WEAPON) && !it->isType(T_BOW) && !it->thrownOnly())
                meleeWep = it;
    }
    if (meleeWep && meleeWep->thrownOnly()) {
        thrownWep = meleeWep;
        meleeWep = offhandWep = NULL;
        if (it = EInSlot(SL_LSHOULDER))
            if (it->isType(T_WEAPON) && !it->isType(T_BOW) && !it->thrownOnly())
                meleeWep = it;
    } else if (meleeWep && meleeWep->HasIFlag(IT_THROWABLE) && !thrownWep)
        thrownWep = meleeWep;

    if (!offhandWep || !offhandWep->isType(T_WEAPON) || ((offhandWep==meleeWep) && !TITEM(meleeWep->iID)->HasFlag(WT_DOUBLE)))
        offhandWep = NULL;

#if 0
    /* This is a nice concept, but currently horribly buggy
    -- the player gets parry bonus from quarterstaff in
    the shoulder slot, etc. */

    if (!meleeWep)
        meleeWep = getPrimaryMelee();
    if (!missileWep && getPrimaryRanged())
        if (getPrimaryRanged()->isType(T_BOW))
            missileWep = getPrimaryRanged();
    if (!thrownWep && getPrimaryRanged())
        if (getPrimaryRanged()->HasIFlag(IT_THROWABLE))
            thrownWep = getPrimaryRanged();

    if (it = EInSlot(SL_LSHOULDER))
        if (!missileWep && it->isType(T_BOW))
            missileWep = it;
#endif

    if (isCharacter()) {
        for(i=0; i!=7; i++) 
            AddBonus(BONUS_BASE, i, thisc->BAttr[i]);

        if (HasStati(POLYMORPH)) {
            TMonster * tm = TMON(mID);
            // ww: polymorphing into an ooze (or whatever, something stupid)
            // doesn't cause you to become stupid -- otherwise mages and druids
            // would never think to stop their polymorph spells. You're
            // changing the body: the physical attributes change. An orc
            // polymorphed into a mind flayer doesn't suddenly become smart. 
            // Here's quote from the SRD: "The subject gains the Strength,
            // Dexterity, and Constitution scores of the new form but retains
            // its own Intelligence, Wisdom, and Charisma scores."
            for(i = A_STR; i <= A_CON; i++) 
                if (tm->Attr[i]) {
                    AddBonus(BONUS_NATURAL,i,(tm->Attr[i]-10));
                } else {
                    AttrAdj[i][BONUS_BASE] = 0; 
                    AttrAdj[i][BONUS_TEMP] = 0; 
                } 
            for(i=A_INT; i <= A_LUC; i++)
                AddBonus(BONUS_NATURAL,i,TRACE(thisc->RaceID)->AttrAdj[i]);
            // ww: these were forgotten somewhere
            for(i = 0; i < 5; i++)  {
                AddBonus(BONUS_NATURAL,A_SPD_ARCHERY+i,tm->Spd);
                AddBonus(BONUS_NATURAL,A_HIT_ARCHERY+i,tm->Hit);
            }
        } else {
            if (thisc->RaceID) {
                TRace * tr = TRACE(thisc->RaceID);
                for(i=0; i != 7; i++)
                    if (tr->AttrAdj[i] == -99) {
                        AttrAdj[i][BONUS_BASE] = 0; 
                        AttrAdj[i][BONUS_TEMP] = 0; 
                    } else { 
                        AddBonus(BONUS_NATURAL, i, tr->AttrAdj[i]);
                    } 
            }
        }

        for(i=0;i!=3 && thisc->ClassID[i];i++) {
            StackBonus(BONUS_BASE+i,A_HIT_ARCHERY, (TCLASS(thisc->ClassID[i])->AttkVal[S_ARCHERY] * thisc->Level[i])/100);
            StackBonus(BONUS_BASE+i,A_HIT_BRAWL, (TCLASS(thisc->ClassID[i])->AttkVal[S_BRAWL] * thisc->Level[i])/100);
            StackBonus(BONUS_BASE+i,A_HIT_MELEE, (TCLASS(thisc->ClassID[i])->AttkVal[S_MELEE] * thisc->Level[i])/100);
            StackBonus(BONUS_BASE+i,A_HIT_THROWN, (TCLASS(thisc->ClassID[i])->AttkVal[S_THROWN] * thisc->Level[i])/100);
            StackBonus(BONUS_BASE+i,A_HIT_OFFHAND, (TCLASS(thisc->ClassID[i])->AttkVal[S_MELEE] * thisc->Level[i])/100);

            StackBonus(BONUS_BASE+i,A_SPD_ARCHERY, max(0,AttrAdj[A_HIT_ARCHERY][BONUS_BASE+i] - (thisc->Level[i]/2)));
            StackBonus(BONUS_BASE+i,A_SPD_BRAWL, max(0,AttrAdj[A_HIT_ARCHERY][BONUS_BASE+i] - (thisc->Level[i]/2)));
            StackBonus(BONUS_BASE+i,A_SPD_MELEE, max(0,AttrAdj[A_HIT_ARCHERY][BONUS_BASE+i] - (thisc->Level[i]/2)));
            StackBonus(BONUS_BASE+i,A_SPD_THROWN, max(0,AttrAdj[A_HIT_ARCHERY][BONUS_BASE+i] - (thisc->Level[i]/2)));
            StackBonus(BONUS_BASE+i,A_SPD_OFFHAND, max(0,AttrAdj[A_HIT_ARCHERY][BONUS_BASE+i] - (thisc->Level[i]/2)));

            StackBonus(BONUS_BASE+i,A_SAV_FORT, TCLASS(thisc->ClassID[i])->HasFlag(CF_GOOD_FORT) ? GoodSave[thisc->Level[i]] : PoorSave[thisc->Level[i]]);
            StackBonus(BONUS_BASE+i,A_SAV_REF, TCLASS(thisc->ClassID[i])->HasFlag(CF_GOOD_REF) ? GoodSave[thisc->Level[i]] : PoorSave[thisc->Level[i]]);
            StackBonus(BONUS_BASE+i,A_SAV_WILL, TCLASS(thisc->ClassID[i])->HasFlag(CF_GOOD_WILL) ? GoodSave[thisc->Level[i]] : PoorSave[thisc->Level[i]]);

            StackBonus(BONUS_BASE+i,A_DEF, thisc->Level[i] / TCLASS(thisc->ClassID[i])->DefMod);

        }

        int16 BAB;
        for(i = 0;i != 5; i++) {
            BAB = AttrAdj[A_HIT_ARCHERY+i][BONUS_BASE] + AttrAdj[A_HIT_ARCHERY+i][BONUS_CLASS2] + AttrAdj[A_HIT_ARCHERY+i][BONUS_CLASS3];
            if (GetBAB(i) > BAB) {
                AttrAdj[A_HIT_ARCHERY+i][BONUS_STUDY] = GetBAB(i) - BAB;
                AttrAdj[A_HIT_ARCHERY+i][BONUS_STUDY] = GetBAB(i) - BAB;
            }
        }
    } else {
        /* When monsters use a *natural* shapechanging ability (a dragon's human
        form, a lycanthrope, etc.), they use their natural attributes or those
        of the assumed form, whichever is better. Magical polymorph spells
        always give the attributes of the new form, OTOH. */
        if (GetStatiEID(POLYMORPH)) { 
            for(i = 0;i != 7; i++)
                AddBonus(BONUS_BASE,i,TMON(mID)->Attr[i]);
        } else {
            for (i = 0; i != 7; i++) 
                AddBonus(BONUS_BASE,i,max(TMON(tmID)->Attr[i],TMON(mID)->Attr[i]));
        }

        AddBonus(BONUS_BASE,A_HIT,TMON(mID)->Hit);

        StackBonus(BONUS_NATURAL,A_SPD,TMON(mID)->Spd);      
        /* Account for template here... */
        tsav = MonGoodSaves((int8)TMON(mID)->MType[0]) | MonGoodSaves((int8)TMON(mID)->MType[1]) | MonGoodSaves((int8)TMON(mID)->MType[2]);

        if (tsav & XBIT(FORT))
            AddBonus(BONUS_BASE,A_SAV_FORT, GoodSave[max(ChallengeRating(),0)]);
        else 
            AddBonus(BONUS_BASE,A_SAV_FORT, PoorSave[max(ChallengeRating(),0)]);

        if (tsav & XBIT(REF))
            AddBonus(BONUS_BASE,A_SAV_REF, GoodSave[max(ChallengeRating(),0)]);
        else 
            AddBonus(BONUS_BASE,A_SAV_REF, PoorSave[max(ChallengeRating(),0)]);

        if (tsav & XBIT(WILL))
            AddBonus(BONUS_BASE,A_SAV_WILL, GoodSave[max(ChallengeRating(),0)]);
        else 
            AddBonus(BONUS_BASE,A_SAV_WILL, PoorSave[max(ChallengeRating(),0)]);

        // mMana = TMON(mID)->Mana;
    }

    StackBonus(BONUS_NATURAL,A_DEF,TMON(mID)->Def);
    StackBonus(BONUS_BASE,A_SIZ,TMON(mID)->Size);
    StackBonus(BONUS_BASE,A_MOV,TMON(mID)->Mov);
    StackBonus(BONUS_NATURAL,A_ARM,TMON(mID)->Arm);

    if (HasFeat(FT_IRON_SKIN)) { 
        StackBonus(BONUS_NATURAL,A_ARM,5); 
    } 

    AddBonus(BONUS_NATURAL,A_FAT,4);

    if (isCharacter()) {
        for (i = 0; i < 3; i++) 
            if (thisc->Level[i] > 0) {
                TClass *tc = TCLASS(thisc->ClassID[i]);
                if (!tc)
                    continue; 
                int amt = thisc->Level[i] * tc->HitDie / 12; 
                { AddBonus(BONUS_BASE+i,A_FAT,amt); }
            } 
    } else {
        int amt = max( ChallengeRating(), 0) / 2; 
        AddBonus(BONUS_BASE,A_FAT,amt); 
    } 

    for(i=0;i!=5;i++) {
        switch(i) {
        case S_ARCHERY: it = missileWep; break;
        case S_BRAWL: it = NULL;
        case S_MELEE: it = meleeWep; break;
        case S_DUAL: it = offhandWep; break;
        case S_THROWN: it = thrownWep; break;
        }

        // ww: it is entirely possible to have Weapon Focus - Tentacle
        // and want to get a A_HIT_BRAWL bonus, even though there is no item
        switch(WepSkill(it)) {
        case WS_NOT_PROF:
            if (i != S_BRAWL) { 
                AddBonus(BONUS_SKILL, A_HIT_ARCHERY+i, -4);
                AddBonus(BONUS_SKILL, A_SPD_ARCHERY+i, -10);
            } 
            break;
        case WS_PROFICIENT:
            /* the default, no mods */
            break;
        case WS_FOCUSED:
            AddBonus(BONUS_SKILL, A_HIT_ARCHERY+i, +1);
            break;
        case WS_SPECIALIST:
            AddBonus(BONUS_SKILL, A_HIT_ARCHERY+i, +1);
            AddBonus(BONUS_SKILL, A_SPD_ARCHERY+i, +2);
            AddBonus(BONUS_SKILL, A_DMG_ARCHERY+i, +2);
            break;
        case WS_MASTERY:
            AddBonus(BONUS_SKILL, A_HIT_ARCHERY+i, +2);
            AddBonus(BONUS_SKILL, A_DMG_ARCHERY+i, +2);
            AddBonus(BONUS_SKILL, A_SPD_ARCHERY+i, +4);
            if (EInSlot(SL_WEAPON) == it && (it == offhandWep || it == meleeWep)) {
                StackBonus(BONUS_SKILL, A_DEF, +2);
            }
            /* Also, the weapon does the next highest die
            of damage. This is figured in Creature::WAttack()
            Missile weapons benefit from increased range.
            */
            break;
        case WS_HIGH_MASTERY:
            AddBonus(BONUS_SKILL,A_HIT_ARCHERY+i,+2);
            AddBonus(BONUS_SKILL,A_DMG_ARCHERY+i,+3);
            AddBonus(BONUS_SKILL,A_SPD_ARCHERY+i,+6);
            if (EInSlot(SL_WEAPON) == it && (it == offhandWep || it == meleeWep)) {
                StackBonus(BONUS_SKILL, A_DEF, +3);
            }
            // +6 Spd, +3 Dmg, +2 Hit, +3 Def, +1 Crit Mult, Negate Fumbles 
            break; 
        case WS_GRAND_MASTERY:
            AddBonus(BONUS_SKILL,A_HIT_ARCHERY+i,+3);
            AddBonus(BONUS_SKILL,A_DMG_ARCHERY+i,+4);
            AddBonus(BONUS_SKILL,A_SPD_ARCHERY+i,+10);
            if (EInSlot(SL_WEAPON) == it && (it == offhandWep || it == meleeWep)) {
                StackBonus(BONUS_SKILL, A_DEF, +4);
            }
            //  +10 Spd, +4 Dmf, +3 Hit, +4 Def, +1 CM, 
            // ww: todo! Chance of death blow/bisection
            break; 
        }
        if (!it)
            continue;
        AddBonus(BONUS_WEAPON, A_HIT_ARCHERY+i, TITEM(it->iID)->u.w.Acc + it->HasQuality(IQ_MITHRIL) - it->HasQuality(IQ_ORCISH));
        AddBonus(BONUS_WEAPON, A_SPD_ARCHERY+i, TITEM(it->iID)->u.w.Spd + it->HasQuality(IQ_ELVEN)*2);
        if (it->HasQuality(IQ_ORCISH) || it->HasQuality(IQ_ADAMANT))
            AddBonus(BONUS_WEAPON, A_DMG_ARCHERY+i, 1); 

        if (!KnownOnly || it->isKnown(KN_PLUS)) {
            AddBonus(BONUS_ENHANCE,A_HIT_ARCHERY+i, it->GetPlus() + (it->HasQuality(WQ_ACCURACY) ? 4 : 0));
            AddBonus(BONUS_ENHANCE,A_SPD_ARCHERY+i, it->GetPlus());
            AddBonus(BONUS_ENHANCE,A_DMG_ARCHERY+i, it->GetPlus());
        }
        if (it == offhandWep || it == meleeWep)
            if (it->isMetallic()) {
                StackBonus(BONUS_ARMOUR, A_PRI, -16);
            }

        if (it->HasQuality(WQ_HOLY) && isMType(MA_EVIL)) {
            AddBonus(BONUS_NEGLEV,A_HIT_ARCHERY+i,-2);
            AddBonus(BONUS_NEGLEV,A_DMG_ARCHERY+i,-2);
            AddBonus(BONUS_NEGLEV,A_SPD_ARCHERY+i,-4);
        }

        if (it->HasQuality(WQ_UNHOLY) && isMType(MA_GOOD)) {
            AddBonus(BONUS_NEGLEV,A_HIT_ARCHERY+i,-2);  
            AddBonus(BONUS_NEGLEV,A_DMG_ARCHERY+i,-2);
            AddBonus(BONUS_NEGLEV,A_SPD_ARCHERY+i,-4);
        }

        if (it->HasQuality(WQ_CHAOTIC) && isMType(MA_LAWFUL)) {
            AddBonus(BONUS_NEGLEV,A_HIT_ARCHERY+i,-2); 
            AddBonus(BONUS_NEGLEV,A_DMG_ARCHERY+i,-2);
            AddBonus(BONUS_NEGLEV,A_SPD_ARCHERY+i,-4);
        }

        if (it->HasQuality(WQ_LAWFUL) && isMType(MA_CHAOTIC)) {
            AddBonus(BONUS_NEGLEV,A_HIT_ARCHERY+i,-2);   
            AddBonus(BONUS_NEGLEV,A_DMG_ARCHERY+i,-2);
            AddBonus(BONUS_NEGLEV,A_SPD_ARCHERY+i,-4);
        }

        if (it->HasQuality(WQ_BALANCE) && (isMType(MA_LAWFUL) || isMType(MA_CHAOTIC)) && (isMType(MA_GOOD) || isMType(MA_EVIL))) {
            AddBonus(BONUS_NEGLEV,A_HIT_ARCHERY+i,-2);   
            AddBonus(BONUS_NEGLEV,A_DMG_ARCHERY+i,-2);
            AddBonus(BONUS_NEGLEV,A_SPD_ARCHERY+i,-4);
        } 
    }

    if (meleeWep && meleeWep->HasQuality(WQ_SPEED))
        StackBonus(BONUS_ENHANCE,A_SPD_MELEE,offhandWep ? 5 : 10);
    if (offhandWep && offhandWep->HasQuality(WQ_SPEED))
        StackBonus(BONUS_ENHANCE,A_SPD_MELEE,5);
    if (missileWep && missileWep->HasQuality(WQ_SPEED))
        StackBonus(BONUS_ENHANCE,A_SPD_ARCHERY,10);

    StateFlags &= ~(MS_HAS_REACH | MS_REACH_ONLY);
    if (InSlot(SL_WEAPON) && InSlot(SL_WEAPON)->HasIFlag(WT_REACH)) {
        StateFlags |= MS_HAS_REACH;
        if (!InSlot(SL_WEAPON)->HasIFlag(WT_STRIKE_NEAR))
            StateFlags |= MS_REACH_ONLY;
    } else if (InherentCreatureReach())
        StateFlags |= MS_HAS_REACH;

#define KNOWN \
    if (KnownOnly && S->h && oThing(S->h)->isItem() && !oItem(S->h)->isKnown(KN_PLUS)) continue

    if (HasStati(PRONE)) {
        AddBonus(BONUS_STATUS,A_HIT,-4);
        AddBonus(BONUS_STATUS,A_DEF,-4);
    }
    if (HasStati(CONFUSED)) {
        AddBonus(BONUS_STATUS,A_INT,-4);
        AddBonus(BONUS_STATUS,A_WIS,-4);
        AddBonus(BONUS_STATUS,A_MAG,-15);
    }
    grappling = (HasStati(GRABBED) || HasStati(GRAPPLED) || HasStati(GRAPPLING) || HasStati(STUCK));
    if (HasStati(STUNNED) || HasStati(NAUSEA)) { 
        AddBonus(BONUS_STATUS,A_DEX,-6);
        AddBonus(BONUS_STATUS,A_SPD,-10);
        AddBonus(BONUS_STATUS,A_MAG,-10);
    }
    if (HasStati(DISTRACTED))
        AddBonus(BONUS_CIRC,A_MAG,HighStatiMag(DISTRACTED));
    switch (HungerState()) {
    case BLOATED:
        AddBonus(BONUS_HUNGER, A_SPD,-2);
        break;
    case HUNGRY:
        AddBonus(BONUS_HUNGER, A_FAT, -1);
        AddBonus(BONUS_HUNGER, A_STR, -1);
        AddBonus(BONUS_HUNGER, A_CON, -1);
    case STARVING:
        AddBonus(BONUS_HUNGER, A_MAG,-3);
        AddBonus(BONUS_HUNGER, A_FAT, -2);
        AddBonus(BONUS_HUNGER, A_STR, -2);
        AddBonus(BONUS_HUNGER, A_CON, -2);
        break;
    case WEAK:
    case FAINTING:
        AddBonus(BONUS_HUNGER, A_MAG,-6);
        AddBonus(BONUS_HUNGER, A_FAT, -2);
        AddBonus(BONUS_HUNGER, A_STR, -4);
        AddBonus(BONUS_HUNGER, A_CON, -4);
        break;
    }

    if (HasStati(SINGING))
        AddBonus(BONUS_STATUS,A_HIT,-2);

    StatiIterNature(this,ADJUST)
        KNOWN;
        AddBonus(BONUS_ENHANCE,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_SAC)
        KNOWN;
        AddBonus(BONUS_SACRED,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_INS)
        KNOWN;
        AddBonus(BONUS_INSIGHT,S->Val,S->Mag);
    StatiIterEnd(this)
        StatiIterNature(this,ADJUST_COMP)
        KNOWN;
    AddBonus(BONUS_COMP,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_ART)
        KNOWN;
        AddBonus(BONUS_ARTI,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_DEFL)
        KNOWN;
        AddBonus(BONUS_DEFLECT,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_DMG)
        KNOWN;
        if (S->Val == A_AID) 
            StackBonus(BONUS_NEGLEV,S->Val,S->Mag);
        else 
            StackBonus(BONUS_DAMAGE,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_INH)
        KNOWN;
        StackBonus(BONUS_INHERANT,S->Val,S->Mag);
    StatiIterEnd(this)
     StatiIterNature(this,ADJUST_MOR)
        KNOWN;
        AddBonus(BONUS_MORALE,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_ARM)
        KNOWN;
        AddBonus(BONUS_ARMOUR,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_DODG)
        KNOWN;
        StackBonus(BONUS_DODGE,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_CIRC)
        KNOWN;
        StackBonus(BONUS_CIRC,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_NAT)
        KNOWN;
        AddBonus(BONUS_NATURAL,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_LUCK)
        KNOWN;
        AddBonus(BONUS_LUCK,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_RES)
        KNOWN;
        AddBonus(BONUS_RESIST,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_PAIN)
        KNOWN;
        AddBonus(BONUS_PAIN,S->Val,S->Mag);
    StatiIterEnd(this)
    StatiIterNature(this,ADJUST_SIZE)
        KNOWN;
        AddBonus(BONUS_SIZE,S->Val,S->Mag);
    StatiIterEnd(this)

    {
        int16 n;
        // Concentration cancels pain
        if (HasSkill(SK_CONCENT) && ((n = (SkillLevel(SK_CONCENT)-8)/2) > 0))
            for (i = 0; i != ATTR_LAST; i++)
                if (AttrAdj[i][BONUS_PAIN] < 0)
                    AttrAdj[i][BONUS_PAIN] = min(0, AttrAdj[i][BONUS_PAIN] + n);
    }

    if (HasStati(MANIFEST))
        AddBonus(BONUS_ENHANCE,A_CHA,4);

    if (HasStati(SPRINTING)) { 
        // ww: in the SRD, Charging increases your movement rate by 100%
        // and costs no fatigue ... I've never used sprinting in the
        // game, so let's make it a bit cooler! We'll also assume that
        // it stacks with Charging. 

        AddBonus(BONUS_TACTIC,A_MOV,20);
        AddBonus(BONUS_TACTIC,A_DEF,-2);
    }
    if (HasStati(CHARGING)) {
        // ww: SRD: Charging is x2 movement, -2 AC
        // fjm: In SRD, charging also prevents you from getting
        // full-attack and does all kinds of other stuff. Charging
        // is already strong in Incursion; let's not make it mean
        // you automatically outrun all monsters -- that's what
        // sprinting is for!
        AddBonus(BONUS_TACTIC,A_MOV,10);
        AddBonus(BONUS_TACTIC,A_DEF,-2);
    }
    if (HasStati(DEFENSIVE)) {
        AddBonus(BONUS_TACTIC,A_DEF,+4);
        AddBonus(BONUS_TACTIC,A_MAG,-4);
        AddBonus(BONUS_TACTIC,A_MOV,-5);
        AddBonus(BONUS_TACTIC,A_HIT,-4);
        AddBonus(BONUS_TACTIC,A_DMG,-2);
    }
    if (HasStati(HIDING))
        if (!HasFeat(FT_SNEAKY))
            AddBonus(BONUS_HIDE,A_MOV,-10);

    StatiIterNature(this,RAGING)
        if (isSmallRace()) {
            AddBonus(BONUS_RAGE, A_STR, S->Val/2);
            AddBonus(BONUS_RAGE, A_DEX, S->Val/2);
        } else {
            AddBonus(BONUS_RAGE, A_STR, S->Val);
        }

        AddBonus(BONUS_RAGE, A_CON, S->Val);
        AddBonus(BONUS_RAGE, A_SPD, S->Val);
        AddBonus(BONUS_RAGE, A_SAV_WILL, S->Val/2);
    StatiIterEnd(this)
    StatiIterNature(this,DWARVEN_FOCUS)
        if (!S->h)
            break;
        AddBonus(BONUS_MORALE, A_SAV_WILL, +4);
        AddBonus(BONUS_MORALE, A_FAT, 3);
    StatiIterEnd(this)
    StatiIterNature(this,TEMPLATE)
        TTemplate *tt;
        tt = TTEM(S->eID);
        for (j=0;j!=6;j++)
            APPLY_TEMPLATE_MODIFIER(j,tt->Attr[j])
        APPLY_TEMPLATE_MODIFIER(A_HIT_ARCHERY,tt->Hit)
        APPLY_TEMPLATE_MODIFIER(A_HIT_BRAWL,tt->Hit)
        APPLY_TEMPLATE_MODIFIER(A_HIT_MELEE,tt->Hit)
        APPLY_TEMPLATE_MODIFIER(A_HIT_OFFHAND,tt->Hit)
        APPLY_TEMPLATE_MODIFIER(A_HIT_THROWN,tt->Hit)
        APPLY_TEMPLATE_MODIFIER(A_DEF,tt->Def)
        APPLY_TEMPLATE_MODIFIER(A_MOV,tt->Mov)
        APPLY_TEMPLATE_MODIFIER(A_SPD_ARCHERY,tt->Spd)
        APPLY_TEMPLATE_MODIFIER(A_SPD_BRAWL,tt->Spd)
        APPLY_TEMPLATE_MODIFIER(A_SPD_MELEE,tt->Spd)
        APPLY_TEMPLATE_MODIFIER(A_SPD_OFFHAND,tt->Spd)
        APPLY_TEMPLATE_MODIFIER(A_SPD_THROWN,tt->Spd)
        APPLY_TEMPLATE_MODIFIER(A_SIZ,tt->Size)
        NAT_ARMOUR_TEMPLATE_MOD(A_ARM,tt->Arm)
        StackBonus(BONUS_TEMP,A_DMG_BRAWL,tt->DmgMod);
    StatiIterEnd(this)

    if (HasStati(TUMBLING)) {
        AddBonus(BONUS_STATUS,A_MOV,(SkillLevel(SK_TUMBLE)+1)/2);
        AddBonus(BONUS_STATUS,A_DEF,(SkillLevel(SK_TUMBLE)+1)/2);
        AddBonus(BONUS_STATUS,A_SPD,-5);
        AddBonus(BONUS_STATUS,A_MAG,-5);
    }
    if (HasSkill(SK_TUMBLE) && SkillLevel(SK_TUMBLE) > 5)
        AddBonus(BONUS_COMP,A_SAV_REF,(SkillLevel(SK_TUMBLE)-3)/3);
    if (HasStati(ELEVATED)) {
        AddBonus(BONUS_ELEV,A_DEF,SkillLevel(SK_CLIMB)/6+2);
        AddBonus(BONUS_ELEV,A_HIT,SkillLevel(SK_CLIMB)/6+2);
        if (GetStatiVal(ELEVATED) == ELEV_CEILING)
            AddBonus(BONUS_ELEV,A_MOV, -14 + max(0,SkillLevel(SK_CLIMB)-20));
        else if (HasFeat(FT_BRACHIATION))
            ;
        else
            AddBonus(BONUS_ELEV,A_MOV, -10);
    } 
    if (HasStati(MOUNTED)) {
        AddBonus(BONUS_ELEV,A_DEF,SkillLevel(SK_RIDE)/6+2);
        AddBonus(BONUS_ELEV,A_HIT_MELEE,SkillLevel(SK_RIDE)/5+1);
        AddBonus(BONUS_ELEV,A_DMG_MELEE,SkillLevel(SK_RIDE)/5);
    }

    static int armourSlots[] = {
        SL_ARMOUR, SL_READY, SL_WEAPON, 0
    } ;
    for (j=0; armourSlots[j]; j++) {
        i = armourSlots[j];
        if ((it = EInSlot(i)) && it->activeSlot(i)) {
            /* count a two-handed shield only once! */
            if (i == SL_WEAPON) 
                if (EInSlot(SL_READY) == EInSlot(SL_WEAPON))
                    continue;

            /* don't count a suit of armour held in a hand */
            if (!it->activeSlot(i))
                continue; 

            if (it->isType(T_SHIELD)) {
                int penalty = ((Armour *)it)->PenaltyVal(this,true);

                if (!grappling) {
                    StackBonus(BONUS_SHIELD, A_COV,it->CovVal(this,KnownOnly));
                    AddBonus(BONUS_SHIELD,A_DEF,it->DefVal(this,KnownOnly));
                    // ww: to make shields more attractive, magical shields also
                    // absorb damage (based largely on their Plus 
                    AddBonus(BONUS_SHIELD,A_ARM, it->ArmVal(0,KnownOnly)); 
                } 
                StackBonus(BONUS_SHIELD,A_ARC,penalty * 2);
                if (it->isMetallic())
                    StackBonus(BONUS_ARMOUR, A_PRI, -16);

                // ww: not bonus skill because if you're wearing armour *and* a
                // shield, the penalties stack! 
                if (WepSkill(it) == WS_NOT_PROF) {
                    StackBonus(BONUS_SHIELD,A_SPD,penalty);
                    StackBonus(BONUS_SHIELD,A_HIT,penalty);
                    StackBonus(BONUS_SHIELD,A_SAV_REF,penalty);
                }

                AddBonus(BONUS_SHIELD, A_MOV, (penalty / 2));
            } else if (it->isType(T_ARMOUR)) {
                int penalty = ((Armour *)it)->PenaltyVal(this,true) ;
                StackBonus(BONUS_ARMOUR, A_COV, it->CovVal(this, KnownOnly));
                StackBonus(BONUS_ARMOUR, A_ARC, penalty * 2);
                if (it->isMetallic())
                    StackBonus(BONUS_ARMOUR, A_PRI, -16);
                if (!it->isGroup(WG_LARMOUR))
                    StackBonus(BONUS_ARMOUR, A_BAR, penalty * 2);

                if (WepSkill(it) == WS_NOT_PROF) {
                    StackBonus(BONUS_ARMOUR,A_SPD,penalty);
                    StackBonus(BONUS_ARMOUR,A_HIT,penalty);
                    StackBonus(BONUS_ARMOUR,A_SAV_REF,penalty);
                }
                AddBonus(BONUS_ARMOUR, A_MOV, penalty);
            } 
        } 
    } 

    if (onPlane() != PHASE_MATERIAL) {
        // SRD: An incorporeal creature has no natural armour bonus but has a
        // deflection bonus equal to its Charisma bonus (always at least +1,
        // even if the creature's Charisma score does not normally provide a
        // bonus).
        StackBonus(BONUS_CIRC,A_DEF, max(1,Mod2(A_CHA)));
    } 

    switch (Encumbrance()) {
    case EN_NONE:
        break;
    case EN_LIGHT:
        if (HasFeat(FT_LOADBEARER)) 
            break;
        AddBonus(BONUS_ENCUM,A_MOV,-2);
        break;
    case EN_MODERATE:
        if (HasFeat(FT_LOADBEARER)) 
            break;
        AddBonus(BONUS_ENCUM,A_DEF,-1);
        AddBonus(BONUS_ENCUM,A_MOV,-4);
        AddBonus(BONUS_ENCUM,A_FAT,-1);
        break;
    case EN_HEAVY:
        AddBonus(BONUS_ENCUM,A_DEF,-2 / (1 + HasFeat(FT_LOADBEARER)));
        AddBonus(BONUS_ENCUM,A_MOV,-6 / (1 + HasFeat(FT_LOADBEARER)));
        AddBonus(BONUS_ENCUM,A_SAV_REF,-2 / (1 + HasFeat(FT_LOADBEARER)));
        AddBonus(BONUS_ENCUM,A_SPD,-2 / (1 + HasFeat(FT_LOADBEARER)));
        AddBonus(BONUS_ENCUM,A_FAT,-2 / (1 + HasFeat(FT_LOADBEARER)));
        break;
    case EN_EXTREME:
        AddBonus(BONUS_ENCUM,A_DEF,-4 / (1 + HasFeat(FT_LOADBEARER)));
        AddBonus(BONUS_ENCUM,A_MOV,-10 / (1 + HasFeat(FT_LOADBEARER)));
        AddBonus(BONUS_ENCUM,A_SAV_REF,-4 / (1 + HasFeat(FT_LOADBEARER)));
        AddBonus(BONUS_ENCUM,A_SPD,-5 / (1 + HasFeat(FT_LOADBEARER)));
        AddBonus(BONUS_ENCUM,A_FAT,-3 / (1 + HasFeat(FT_LOADBEARER)));
        break;
    }

    for (i = 0;i != 7; i++)
        if (HasFeat(FT_IMPROVED_STRENGTH + i))
            AddBonus(BONUS_FEAT,i,1);

    if (HasFeat(FT_ATHLETIC))
        AddBonus(BONUS_COMP,A_SAV_FORT,1);
    if (HasFeat(FT_CLEAR_MINDED))
        AddBonus(BONUS_COMP,A_SAV_WILL,1);

    if (HasFeat(FT_LIGHTNING_REFLEXES))
        AddBonus(BONUS_FEAT,A_SAV_REF,3);
    if (HasFeat(FT_GREAT_FORTITUDE))
        AddBonus(BONUS_FEAT,A_SAV_FORT,3);
    if (HasFeat(FT_IRON_WILL))
        AddBonus(BONUS_FEAT,A_SAV_WILL,3);

    if (HasFeat(FT_RUN) )
        AddBonus(BONUS_FEAT,A_MOV,4);

    /*
    if (HasFeat(FT_FAST_STRIDE) &&
    (Encumbrance() <= EN_MODERATE) &&
    ((ArmourType() == 0) || !(ArmourType() & WG_HARMOUR)))
    { AddBonus(BONUS_ENHANCE,A_MOV,10); }
    */

    {
        int16 feat_fp;
        feat_fp = 0;
        if (HasFeat(FT_ENDURANCE))
            feat_fp += 5;
        if (HasFeat(FT_WOODSMAN))
            feat_fp += 2;
        AddBonus(BONUS_FEAT,A_FAT,feat_fp);
    }
    if (HasFeat(FT_TALENTED))
        AddBonus(BONUS_FEAT,A_MAG,+1);

    if (HasFeat(FT_ZEN_ARCHERY))
        AddBonus(BONUS_INSIGHT,A_HIT_ARCHERY,Mod(A_WIS));

    if (HasFeat(FT_ZEN_DEFENSE)) { 
        uint32 at = getArmourType(true);
        if (!at)
            AddBonus(BONUS_INSIGHT,A_DEF,Mod(A_WIS)); 
        else if (!(at & (WG_MARMOUR|WG_HARMOUR)))
            AddBonus(BONUS_INSIGHT,A_DEF,(Mod(A_WIS)+1)/2); 
    }

    if (HasSkill(SK_ATHLETICS)) {
        AddBonus(BONUS_SKILL, A_MOV, max(0,SkillLevel(SK_ATHLETICS)/2)); 
        AddBonus(BONUS_SKILL,A_FAT,max(0,SkillLevel(SK_ATHLETICS)/3));
    }

    if (isMType(MA_UNDEAD) || isMType(MA_CONSTRUCT) || isMType(MA_PLANT)) {
        cFP = 0; 
        if (isCharacter())
            SetStatiDur(HUNGER,-1,NULL,CONTENT*2);
    } 

    if ((cFP >= -oFP) || AttrAdj[A_STR][BONUS_RAGE])
        ;
    else if (cFP >= -(oFP*2)) /* Fatigued */ {
        AddBonus(BONUS_FATIGUE,A_STR,-2);
        AddBonus(BONUS_FATIGUE,A_DEX,-2);
        AddBonus(BONUS_FATIGUE,A_CHA,-2);
        AddBonus(BONUS_FATIGUE,A_MOV,-2);
        AddBonus(BONUS_FATIGUE,A_SPD,-2);
    } else {
        AddBonus(BONUS_FATIGUE,A_STR,-6);
        AddBonus(BONUS_FATIGUE,A_DEX,-6);
        AddBonus(BONUS_FATIGUE,A_CHA,-6);
        AddBonus(BONUS_FATIGUE,A_MOV,-5);
        AddBonus(BONUS_FATIGUE,A_SPD,-5);
    }

    /* Precalculate the true attributes (Str, Con, Wis, etc.) early, so that
    * we can calculate other things that depend on them properly. */
    if (isCharacter() && KnownOnly)
        for(i=0; i!=7; i++) {
            thisc->KAttr[i] = 0;
            if (AttrAdj[i][BONUS_BASE] == 0)
                ;
            else for (j = 0; j != BONUS_LAST; j++)
                thisc->KAttr[i] += AttrAdj[i][j];

            if (Attr[i] <= 0) Attr[i] = 0;
        }
    else
        for(i = 0;i != 7; i++) {
            Attr[i] = 0;
            if (AttrAdj[i][BONUS_BASE] == 0)
                ;
            else for (j = 0;j != BONUS_LAST; j++)
                Attr[i] += AttrAdj[i][j];
            if (Attr[i] <= 0) Attr[i] = 0;
        }
    if (Attr[A_FAT] <= 0)
        Attr[A_FAT] = 1;

    if (HasFeat(FT_ONE_BODY_ONE_SOUL) || (Attr[A_CON] == 0)) {
        int16 one_mod = max(XMod(A_CON),XMod(A_WIS));
        int16 best = max(IAttr(A_CON),IAttr(A_WIS));
        AddBonus(BONUS_ATTR,A_SAV_FORT, one_mod);
        AddBonus(BONUS_ATTR,A_SAV_WILL, one_mod);
        AddBonus(BONUS_ATTR,A_FAT,(best-11)/2); 
    } else { 
        AddBonus(BONUS_ATTR,A_SAV_FORT, XMod(A_CON));
        AddBonus(BONUS_ATTR,A_SAV_WILL, XMod(A_WIS));
        AddBonus(BONUS_ATTR,A_FAT,(IAttr(A_CON)-11)/2); 
    } 
    AddBonus(BONUS_ATTR,A_SAV_REF , XMod(A_DEX));

    AddBonus(BONUS_ATTR,A_HIT_ARCHERY, XMod(A_DEX));
    AddBonus(BONUS_ATTR,A_HIT_THROWN,  XMod(A_DEX));

    if (HasFeat(FT_WEAPON_FINESSE))
        AddBonus(BONUS_ATTR,A_HIT_BRAWL,max(XMod(A_STR),XMod(A_DEX)));
    else
        AddBonus(BONUS_ATTR,A_HIT_BRAWL,XMod(A_STR));

    if (HasFeat(FT_WEAPON_FINESSE) && meleeWep && meleeWep->canFinesse()) 
        AddBonus(BONUS_ATTR,A_HIT_MELEE,max(XMod(A_STR),XMod(A_DEX))); 
    else
        AddBonus(BONUS_ATTR,A_HIT_MELEE,XMod(A_STR));

    if (HasFeat(FT_WEAPON_FINESSE) && offhandWep && offhandWep->canFinesse()) 
        AddBonus(BONUS_ATTR,A_HIT_OFFHAND,max(XMod(A_STR),XMod(A_DEX)));
    else
        AddBonus(BONUS_ATTR,A_HIT_OFFHAND,XMod(A_STR));

    if (HasFeat(FT_WEAPON_FINESSE))
        AddBonus(BONUS_ATTR,A_DMG_BRAWL,max(0,XMod(A_STR)));
    else
        AddBonus(BONUS_ATTR,A_DMG_BRAWL,XMod(A_STR));

    StackBonus(BONUS_SIZE,A_DEF,SZ_MEDIUM - Attr[A_SIZ]);
    StackBonus(BONUS_SIZE,A_HIT,SZ_MEDIUM - Attr[A_SIZ]);

    /* Wielding a weapon two-handed gives you 1.5 times Strength bonus */
    if (meleeWep && !meleeWep->useStrength())
        ;
    else if (meleeWep && (EInSlot(SL_WEAPON) == EInSlot(SL_READY)) && !meleeWep->HasIFlag(WT_DOUBLE))      
        AddBonus(BONUS_ATTR,A_DMG_MELEE,max(0,((XMod(A_STR)*3)+1)/2));
    else if (XMod(A_STR) >= 0 || !HasFeat(FT_WEAPON_FINESSE) || (meleeWep && !meleeWep->canFinesse()))
        AddBonus(BONUS_ATTR,A_DMG_MELEE,XMod(A_STR));

    /* Figure out WT_NO_STRENGTH at attack time */
    AddBonus(BONUS_ATTR,A_DMG_THROWN,max(0,XMod(A_STR)));
    if (missileWep && missileWep->useStrength())
        AddBonus(BONUS_ATTR,A_DMG_ARCHERY,XMod(A_STR));

    /* The offhand weapon gets only 0.5 times Strength bonus */
    if (offhandWep && offhandWep->useStrength()) {
        if (XMod(A_STR) <= 0)
            AddBonus(BONUS_ATTR,A_DMG_OFFHAND,XMod(A_STR));
        else if (offhandWep == meleeWep && HasFeat(FT_POWER_DOUBLE_WEAPON))
            AddBonus(BONUS_ATTR,A_DMG_OFFHAND,(XMod(A_STR))); 
        else
            AddBonus(BONUS_ATTR,A_DMG_OFFHAND,(XMod(A_STR)+1)/2); 
    }

    if (HasFeat(FT_MONKEY_GRIP))
        if (EInSlot(SL_WEAPON) != EInSlot(SL_READY)) {
            if ((it = EInSlot(SL_WEAPON)) && it->isType(T_WEAPON))
                if (it->Size(this) > Attr[A_SIZ])
                    AddBonus(BONUS_CIRC,A_HIT_MELEE,-2);
            if ((it = EInSlot(SL_READY)) && it->isType(T_WEAPON))
                if (it->Size(this) > Attr[A_SIZ])
                    AddBonus(BONUS_CIRC,A_HIT_OFFHAND,-2);
        }

    if (HasFeat(FT_LIGHTNING_FISTS)) //  && !HasStati(POLYMORPH))
        AddBonus(BONUS_FEAT,A_SPD_BRAWL,max(0,XMod(A_DEX)));
    if (HasFeat(FT_FISTS_OF_IRON)) 
        AddBonus(BONUS_FEAT,A_DMG_BRAWL,1); 

    if (HasFeat(FT_RAPID_SHOT))
        AddBonus(BONUS_FEAT,A_SPD_ARCHERY,+5);

    if (HasFeat(FT_ELEGANT_DEFENSE))
        AddBonus(BONUS_ATTR,A_DEF,XMod(A_DEX)*2);
    else
        AddBonus(BONUS_ATTR,A_DEF,XMod(A_DEX));

    // ww: "no dodge while grappling" is handled in fight.cpp
    if (HasFeat(FT_DODGE)) {
        // ww: +2/+1 is too weak compared to the phenominal power of armour in this game! 
        if (!(getArmourType(true) & WG_MARMOUR) && (Encumbrance() <= EN_LIGHT))
            AddBonus(BONUS_DODGE, A_DEF, +3);
        else
            AddBonus(BONUS_DODGE, A_DEF, +1);
    }

    if (HasFeat(FT_EXPERTISE)) {
        int16 a, b = 0;
        // ww: a +10 sword should be better at parrying than a +0 sword
        if (meleeWep) 
            a = meleeWep->ParryVal(this); 
        else {
            /* ww: incredible parry values from enchanted weapons give
            * monks a severe disadvantage! In martial arts flicks you're
            * always seeing monks turn aside knife blows and whatnot barehanded.
            */
            a = AbilityLevel(CA_UNARMED_STRIKE) * 2; 
        } 

        if (offhandWep) 
            b = offhandWep->ParryVal(this); 
        else if (meleeWep && TITEM(meleeWep->iID)->HasFlag(WT_DOUBLE)) 
            b = a; 
        else if (!EInSlot(SL_READY))
            b = AbilityLevel(CA_UNARMED_STRIKE) * 2; 

        // ww: this logic was reversed! ouch!
        if (!HasFeat(FT_DEFENSIVE_SYNERGY))
            AddBonus(BONUS_WEAPON,A_DEF,max(a,b));
        else
            AddBonus(BONUS_WEAPON,A_DEF,max(a,b) + min(a,b)/2);
    } 

    if (meleeWep && meleeWep->HasQuality(WQ_DEFENDING))
        StackBonus(BONUS_WEAPON,A_DEF,meleeWep->GetPlus());
    if (offhandWep && offhandWep->HasQuality(WQ_DEFENDING))
        StackBonus(BONUS_WEAPON,A_DEF,offhandWep->GetPlus());

    // ww: monk Flurry of Blows ability: make an extra attack each round
    // but at -2 penalty ... -1 at 5th level, -0 at ninth, but only with
    // unarmed strikes or martial weapons. To balance things, this only
    // works if you are wearing nothing in the armour slot. 
    if (HasAbility(CA_FLURRY_OF_BLOWS) && HasStati(FLURRYING) && (!isCharacter() || !thisc->EInSlot(SL_ARMOUR)) && !HasStati(POLYMORPH)) {
        int penalty = -3 + AbilityLevel(CA_FLURRY_OF_BLOWS);

        AddBonus(BONUS_CLASS,A_SPD_BRAWL,+20); 
        AddBonus(BONUS_CLASS,A_HIT_BRAWL,penalty);

        if (EInSlot(SL_WEAPON) && (TITEM(EInSlot(SL_WEAPON)->iID)->Group & WG_MARTIAL) || HasFeat(FT_NON_STANDARD_FLURRY)) {
            AddBonus(BONUS_CLASS,A_SPD_MELEE,+20);
            AddBonus(BONUS_CLASS,A_HIT_MELEE,penalty);
        }

        if (EInSlot(SL_READY) && (TITEM(EInSlot(SL_READY)->iID)->Group & WG_MARTIAL) || HasFeat(FT_NON_STANDARD_FLURRY)) {
            AddBonus(BONUS_CLASS,A_SPD_OFFHAND,+20); 
            AddBonus(BONUS_CLASS,A_HIT_OFFHAND,penalty);
        } 
    }

    if (HasAbility(CA_DIVINE_GRACE) && XMod(A_CHA) > 0)
        AddBonus(BONUS_GRACE, A_SAV, XMod(A_CHA));

    if (isMType(MA_CHAOTIC) && isCharacter())
        AddBonus(BONUS_MORALE, A_SAV_WILL, max(0,(thisc->alignLC - 35) / 5));
    else if (isMType(MA_CHAOTIC))
        AddBonus(BONUS_MORALE, A_SAV_WILL, HasMFlag(M_IALIGN) ? 6 : 3);

    if (HasStati(CHANNELING)) {
        if (HasFeat(FT_DIVINE_MIGHT) && XMod2(A_CHA) > 0) { 
            AddBonus(BONUS_SACRED, A_DMG_BRAWL, XMod2(A_CHA)); 
            AddBonus(BONUS_SACRED, A_DMG_MELEE, XMod2(A_CHA)); 
            AddBonus(BONUS_SACRED, A_DMG_OFFHAND, XMod2(A_CHA)); 
        }

        if (HasFeat(FT_DIVINE_VIGOR) && XMod2(A_CHA) > 0) {
            AddBonus(BONUS_SACRED, A_CON, 2);
            AddBonus(BONUS_SACRED, A_MOV, XMod2(A_CHA)); 
        }
    } 

    if (HasAbility(CA_UNEARTHLY_LUCK) && XMod(A_LUC) > 0)
        AddBonus(BONUS_LUCK, A_SAV, XMod(A_LUC));

    if (HasAbility(CA_INCREASED_MOVE))
        AddBonus(BONUS_CLASS, A_MOV, AbilityLevel(CA_INCREASED_MOVE));

    if (offhandWep && meleeWep) {
        if (!HasFeat(FT_AMBIDEXTERITY) && !(offhandWep == meleeWep && HasFeat(FT_DOUBLE_WEAPON_FIGHTING))){
            StackBonus(BONUS_DUAL,A_HIT_OFFHAND,-2);
            StackBonus(BONUS_DUAL,A_SPD_OFFHAND,-2);
        }

        if (!HasFeat(FT_TWO_WEAPON_STYLE) && !(offhandWep == meleeWep && HasFeat(FT_DOUBLE_WEAPON_FIGHTING))) {
            StackBonus(BONUS_DUAL,A_HIT_MELEE  ,-2);
            StackBonus(BONUS_DUAL,A_HIT_OFFHAND,-2);
        }

        if (!HasFeat(FT_TWIN_WEAPON_STYLE) && offhandWep->Size(this) >= meleeWep->Size(this)) { 
            StackBonus(BONUS_DUAL,A_HIT_MELEE,  -2);
            StackBonus(BONUS_DUAL,A_HIT_OFFHAND,-2);
        }

        if (HasFeat(FT_TWO_WEAPON_TEMPEST)) {
            StackBonus(BONUS_DUAL,A_SPD, +10);
            StackBonus(BONUS_DUAL,A_SPD_OFFHAND, +10);
        }
        /*
        The real d20 system gives a -2 penalty to all TWF,
        regardless of how many feats one invests. However, it
        is also commonly agreed that d20 TWF sucks at lower
        levels (and just in general), so we don't use this
        penalty and leave it commented out.

        StackBonus(BONUS_DUAL, A_HIT_MELEE, -2);
        StackBonus(BONUS_DUAL, A_HIT_OFFHAND, -2);
        */
        /*
        Obsolete due to ParryMod and Expertise?
        if (HasFeat(FT_DEFENSIVE_SYNERGY) && (offhandWep != meleeWep)) 
        { AddBonus(BONUS_WEAPON, A_DEF, TITEM(offhandWep->iID)->Def +
        TITEM(meleeWep->iID)->Def) }
        else
        {AddBonus(BONUS_WEAPON, A_DEF, max(TITEM(offhandWep->iID)->Def,
        TITEM(meleeWep->iID)->Def)); }
        */
    }
    /*else if (meleeWep)
    { AddBonus(BONUS_WEAPON, A_DEF, TITEM(meleeWep->iID)->Def); } */

    if (HasStati(MOUNTED)) {
        // ww: your base walking speed is your mount's speed, not 
        // yours! that's the main point of riding ...
        for (i = 0;i < BONUS_LAST; i++)
            if (i != BONUS_CIRC)
                AttrAdj[A_MOV][i] = 0; 

        Creature *mount = (Creature*)GetStatiObj(MOUNTED);
        AddBonus(BONUS_BASE,A_MOV,mount->Attr[A_MOV]);
        AddBonus(BONUS_SKILL,A_MOV,(SkillLevel(SK_RIDE)/2));
        if (HasAbility(CA_RAPID_RIDING)) { 
            AddBonus(BONUS_CLASS,A_MOV,AbilityLevel(CA_RAPID_RIDING)); 
        } 
    } 

    /* Animal Companions receive a bonus to Movement Rate when
    not being ridden, so they can keep up. */
    if (HasStati(ANIMAL_COMPANION) && !HasStati(MOUNT))
        AddBonus(BONUS_CLASS,A_MOV,15);

    if (HasStati(POLYMORPH))
        for (i=A_STR; i<=A_CON; i++) {
            AttrAdj[i][BONUS_ENHANCE] = min(0,AttrAdj[i][BONUS_ENHANCE]);
            AttrAdj[i][BONUS_FEAT] = min(0,AttrAdj[i][BONUS_FEAT]);
            AttrAdj[i][BONUS_INHERANT] = min(0,AttrAdj[i][BONUS_INHERANT]);
        }

    if (HasFeat(FT_LION_HEART)) {
        for (i = 0;i != ATTR_LAST; i++)
            if (AttrAdj[i][BONUS_MORALE] < 0)
                AttrAdj[i][BONUS_MORALE] /= 2;
    }

    if (isCharacter() && KnownOnly) {
        for(i = 0;i != ATTR_LAST; i++) {
            thisc->KAttr[i] = 0;

            /* WW: this is too confusing for now 
            FJM: It's needed for balance. Two sources of 50% MR -- say, a drow with a Mantle of MR -- shouldn't grant 100% MR in a
            game where spellcasters are intended to be supremely dangerous. I'll add a more clear bonus breakdown to the CharSheet
            for MR soon. */
            if (i == A_MR) {
                MRC = 0;
                for(j = 0;j != BONUS_LAST; j++)
                    if (AttrAdj[i][j])
                        MRVals[MRC++] = AttrAdj[i][j];
                if (!MRC)
                    continue;
                qsort(MRVals, MRC, sizeof(int16),compare_int16);
                for(j = 0;j != MRC; j++)
                    thisc->KAttr[A_MR] += ((100 - thisc->KAttr[A_MR]) * MRVals[j]) / 100;
                continue;
            }

            for (j = 0;j != BONUS_LAST; j++)
                if (!(percent_attr(i) && bonus_is_mult(i,j)))
                    thisc->KAttr[i] += AttrAdj[i][j];

            if (percent_attr(i))
                for (j=0;j!=BONUS_LAST;j++)
                    if (bonus_is_mult(i,j)) {
                        thisc->KAttr[i] = ((((thisc->KAttr[i]*5 + 100) *
                            (max(-16,AttrAdj[i][j])*5 + 100) ) / 100)-100)/5;          

                        if (i == A_MOV && AttrAdj[i][j] <= -20 && !isPlayer())
                            isHalted = true;
                    }

            thisc->KAttr[A_CDEF] = thisc->KAttr[A_DEF] - 
                (max(0,AttrAdj[A_DEF][BONUS_WEAPON]) + max(0,AttrAdj[A_DEF][BONUS_INSIGHT]) + max(0,AttrAdj[A_DEF][BONUS_DODGE]) + (HasFeat(FT_COMBAT_CASTING) ? 2 : 4));
        } 
    } else {
        for(i=0; i!=ATTR_LAST; i++) {
            Attr[i] = 0;

            if (i == A_MR) {
                MRC = 0;
                for(j = 0;j != BONUS_LAST; j++)
                    if (AttrAdj[i][j])
                        MRVals[MRC++] = AttrAdj[i][j];
                if (!MRC)
                    continue;

                qsort(MRVals,MRC,sizeof(int16),compare_int16);
                for(j=0; j!=MRC; j++)
                    Attr[A_MR] += ((100 - Attr[A_MR]) * MRVals[j]) / 100;
                continue;
            }

            for (j = 0;j != BONUS_LAST; j++)
                if (!(percent_attr(i) && bonus_is_mult(i,j)))
                    Attr[i] += AttrAdj[i][j];

            if (percent_attr(i))
                for (j=0; j != BONUS_LAST; j++)
                    if (bonus_is_mult(i, j)) {
                        Attr[i] = ((((Attr[i] * 5 + 100) * (AttrAdj[i][j] * 5 + 100)) / 100) - 100) / 5;
                        if (i == A_MOV && AttrAdj[i][j] <= -20 && !isPlayer())
                            isHalted = true;
                    }

            Attr[A_CDEF] = Attr[A_DEF] -
                (max(0,AttrAdj[A_DEF][BONUS_WEAPON]) + max(0,AttrAdj[A_DEF][BONUS_INSIGHT]) + max(0,AttrAdj[A_DEF][BONUS_DODGE]) + (HasFeat(FT_COMBAT_CASTING) ? 2 : 4));
        }
    }

    if (theGame->InPlay() && !KnownOnly) {
        /* Having one's attribute score reduced to 0 by poison, disease or
        the special attacks of an undead is lethal. Having an attribute
        score of 0 naturally (golems have Int 0, undead have Con 0, etc.),
        or having an attribute reduced to 0 by fatigue modifiers, attribute
        magic penalties (ray of enfeeblement/bestow curse) or such is not.
        This explains the logic here. */
        for(i = 0; i != 6; i++) {
            if (AttrAdj[i][BONUS_BASE] && (AttrAdj[i][BONUS_BASE] + AttrAdj[i][BONUS_TEMP] > 0))
                if (Attr[i] <= 0)
                    if (AttrAdj[i][BONUS_BASE] + AttrAdj[i][BONUS_TEMP] + AttrAdj[i][BONUS_FEAT] + AttrAdj[i][BONUS_DAMAGE] <= 0)            
                        AttrDeath |= XBIT(i);
        }

    }

    if (isCharacter() && KnownOnly) {
        for(i = 0;i != 7; i++) 
            if (AttrAdj[i][BONUS_BASE] && (AttrAdj[i][BONUS_BASE] + AttrAdj[i][BONUS_TEMP] > 0)) {
                if (thisp->KAttr[i] <= 0 && !(AttrDeath & XBIT(i)))
                    thisp->KAttr[i] = 1;
            } else
                thisp->KAttr[i] = 0;

        if (thisp->KAttr[A_FAT] <= 0)
            thisp->KAttr[A_FAT] = 1;

        if (!isHalted)
            thisp->KAttr[A_MOV]       = max(min(-15,TMON(mID)->Mov),thisp->KAttr[A_MOV]);
        else
            thisp->KAttr[A_MOV]       = -20;
        thisp->KAttr[A_SPD_MELEE]   = max(-15,thisp->KAttr[A_SPD_MELEE]);
        thisp->KAttr[A_SPD_BRAWL]   = max(-15,thisp->KAttr[A_SPD_BRAWL]);
        thisp->KAttr[A_SPD_ARCHERY] = max(-15,thisp->KAttr[A_SPD_ARCHERY]);
        thisp->KAttr[A_SPD_THROWN]  = max(-15,thisp->KAttr[A_SPD_THROWN]);
        thisp->KAttr[A_SPD_OFFHAND] = max(-15,thisp->KAttr[A_SPD_OFFHAND]);
    } else {
        /* If you do not naturally have an attribute of 0, and are not about
        to die as a result of having that attribute at 0, set it to a minimum
        of one. Also, do not let modifiers raise a natural 0 to a higher
        value -- you can't make a clay golem sapient by playing a Headband
        of Intellect on its forehead. */
        for(i = 0; i != 7; i++) 
            if (AttrAdj[i][BONUS_BASE] && (AttrAdj[i][BONUS_BASE] + AttrAdj[i][BONUS_TEMP] > 0)) {
                if (Attr[i] <= 0 && !(AttrDeath & XBIT(i)))
                    Attr[i] = 1;
            } else
                Attr[i] = 0;

        if (Attr[A_FAT] <= 0)
            Attr[A_FAT] = 1;

        if (!isHalted)
            thisp->Attr[A_MOV]       = max(min(-15,TMON(mID)->Mov),thisp->Attr[A_MOV]);
        else
            thisp->Attr[A_MOV]       = -20;
        thisp->Attr[A_SPD_MELEE]   = max(-15,thisp->Attr[A_SPD_MELEE]);
        thisp->Attr[A_SPD_BRAWL]   = max(-15,thisp->Attr[A_SPD_BRAWL]);
        thisp->Attr[A_SPD_ARCHERY] = max(-15,thisp->Attr[A_SPD_ARCHERY]);
        thisp->Attr[A_SPD_THROWN]  = max(-15,thisp->Attr[A_SPD_THROWN]);
        thisp->Attr[A_SPD_OFFHAND] = max(-15,thisp->Attr[A_SPD_OFFHAND]);

        Creature *lead;
        if ((lead = getLeader()) && !isHalted)
            if (lead != this && lead->GetAttr(A_MOV) > GetAttr(A_MOV))
                if (lead->HasFeat(FT_COORDINATED_TACTICS))
                    Attr[A_MOV] = lead->GetAttr(A_MOV);

        SightRange = max(12,15+Mod(A_WIS)*3) + AbilityLevel(CA_SHARP_SENSES)*2;

        LightRange = EInSlot(SL_LIGHT) ? EInSlot(SL_LIGHT)->GetLightRange() : 0;
        if (InSlot(SL_WEAPON) && InSlot(SL_WEAPON)->HasQuality(WQ_GLOWING))
            LightRange = max(LightRange,InSlot(SL_WEAPON)->GetPlus()*3);
        if (InSlot(SL_READY) && InSlot(SL_READY)->HasQuality(WQ_GLOWING))
            LightRange = max(LightRange,InSlot(SL_READY)->GetPlus()*3);
        if (LightRange)
            LightRange += AbilityLevel(CA_LOWLIGHT);

        ShadowRange = LightRange*2;
        ScentRange = AbilityLevel(CA_SCENT) + (HasFeat(FT_WILD_SHAPE_SCENT) ? 3 : 0);
        InfraRange   = (uint8)AbilityLevel(CA_INFRAVISION);
        if (isBlind())
            SightRange = ShadowRange = InfraRange = 0;  

        if (HasFeat(FT_ACUTE_SENSES)) {
            SightRange  = (SightRange * 3) / 2;
            ShadowRange = (ShadowRange * 3) / 2;
            InfraRange  = (InfraRange * 3) / 2;
            ScentRange  = (ScentRange * 3) / 2; 
        } 
        TelepRange   = (uint8)AbilityLevel(CA_TELEPATHY);
        TremorRange  = (uint8)AbilityLevel(CA_TREMORSENSE);
        BlindRange   = AbilityLevel(CA_BLINDSIGHT) + HasFeat(FT_BLINDSIGHT);
        if (BlindRange) {
            if (m && m->InBounds(x,y) && m->FieldAt(x,y,FI_SILENCE))
                BlindRange = 0; 
            if (InSlot(SL_HELM) && EInSlot(SL_HELM)->isMetallic())
                BlindRange /= 2;
            if (InSlot(SL_WEAPON)) {
                int pen = InSlot(SL_WEAPON)->Size(this) - SZ_TINY;
                if (pen > 0) {
                    if (BlindRange < pen)
                        BlindRange = 0;
                    else
                        BlindRange -= pen; 
                }
            }
            if (InSlot(SL_READY) && InSlot(SL_READY) != InSlot(SL_WEAPON)) {
                int pen = InSlot(SL_READY)->Size(this) - SZ_TINY;
                if (pen > 0) {
                    if (BlindRange < pen)
                        BlindRange = 0;
                    else
                        BlindRange -= pen; 
                }
            }
            BlindRange = max(1,BlindRange);
        }
        NatureSight  = HasAbility(CA_NATURE_SENSE);
        PercepRange  = (uint8)HighStatiMag(PERCEPTION);
        /* We're assuming for now that animals don't need light to see, so
        that they can exist in the dungeon as effective companions to
        druids and rangers. */
        if (!isPlayer() && InfraRange + LightRange + TelepRange + TremorRange + BlindRange + ScentRange == 0)
            InfraRange = max(6,InfraRange);
    }

    if (Attr[A_FAT] != oFP) {
        if (restart_count++ < 10) 
            goto Restart;
        // ww: you lose! 
        Attr[A_FAT] = min(Attr[A_FAT],oFP);
        // ww: fixme, this is hideous! 
    }

    cFP += Attr[A_FAT];
    cHP += Attr[A_THP];
    /* Gaining temporary maximum hit points increases your current
    hit points, but losing them only decreases current hit points
    enough to put them equal to or lower then the new maximum
    hit points. */       
    if (oHP > cHP)
        cHP = min (mHP+Attr[A_THP], oHP);

    SetImage();
    if (m && x != -1)
        m->Update(x,y);

    /* Here, we cause the player to drop weapons when they change
    size catagories as a result of things like Divine Power or
    Enlarge wearing off. */
    if ((!m) || (oldSize == Attr[A_SIZ])) {
        theGame->inCalcVal--;
        return;
    }

    if (HasStati(DISGUISED)) {
        IPrint("Your change in size ruins your disguise!");
        RemoveStati(DISGUISED);
    }

    Creature *mount, *rider;
    if (HasStati(MOUNT)) {
        mount = this;
        rider = (Creature*) GetStatiObj(MOUNT);
        goto MountChecks;
    }
    if (HasStati(MOUNTED)) {
        mount = (Creature*) GetStatiObj(MOUNTED);
        rider = this;

MountChecks:
        if (mount->GetAttr(A_SIZ) <= rider->GetAttr(A_SIZ) && (mount->GetAttr(A_SIZ) != SZ_SMALL || mount->GetAttr(A_SIZ) != SZ_SMALL))
            ThrowVal(EV_DISMOUNT,DSM_THROWN,rider,mount);
        else if (mount->GetAttr(A_SIZ) > rider->GetAttr(A_SIZ)+1 && mount->isMType(MA_ANIMAL))
            ThrowVal(EV_DISMOUNT,DSM_THROWN,rider,mount);
    }


    if (theGame->InPlay()) { 
        static Item *w1, *w2;
        w1 = InSlot(SL_WEAPON);
        w2 = InSlot(SL_READY);
        if (w1 && w1 == w2) {
            if (w1->Size(this) > Attr[A_SIZ]+1 && !w1->isCursed()) {
                IDPrint("You drop your <Obj1>!",
                    "The <Obj2> drops <his:Obj1> <Obj1>.",w1,this);
                Throw(EV_LOSEITEM,this,NULL,w1);
                Throw(EV_DROP,this,NULL,w1);
            }
        } else {
            if (w1 && w1->Size(this) > Attr[A_SIZ] && !w1->isCursed()) {
                IDPrint("You drop your <Obj1>!",
                    "The <Obj2> drops <his:Obj1> <Obj1>.",w1,this);
                Throw(EV_LOSEITEM,this,NULL,w1);
                Throw(EV_DROP,this,NULL,w1);
            }
            if (w2 && w2->Size(this) > Attr[A_SIZ] && !w2->isCursed()) {
                IDPrint("You drop your <Obj1>!",
                    "The <Obj2> drops <his:Obj1> <Obj1>.",w2,this);
                Throw(EV_LOSEITEM,this,NULL,w2);
                Throw(EV_DROP,this,NULL,w2);
            }
        }
    }

    if (FaceRadius[Attr[A_SIZ]] && !FaceRadius[oldSize]) {
        m->NewField(FI_SIZE|FI_MOBILE,x,y,FaceRadius[Attr[A_SIZ]], GLYPH_BULK | (Image & 0x0F00),-1,0,this);
        PlaceNear(x,y);
    } else {
        for (i=0;m->Fields[i];i++)
            if (m->Fields[i]->Creator == myHandle)
                if (m->Fields[i]->FType & FI_SIZE) {
                    if (FaceRadius[Attr[A_SIZ]])
                        m->Fields[i]->rad = FaceRadius[Attr[A_SIZ]];
                    else {
                        m->RemoveField(m->Fields[i]);
                        i--;
                    }
                }
    }
    theGame->inCalcVal--;
}

bool Creature::InherentCreatureReach() {
  if (HasMFlag(M_REACH))
    return true;
  else if (Attr[A_SIZ] >= SZ_LARGE && HasMFlag(M_HUMANOID))
    return true;
  else if (Attr[A_SIZ] >= SZ_HUGE)
    return true;
  return false;
}

void Character::CalcValues(bool KnownOnly, Item *thrown)
{
  int16 i,j;
  bool one_body = HasFeat(FT_ONE_BODY_ONE_SOUL) ||
       (Attr[A_CON] == 0);
       
  /* HACKFIX */
  if (theGame->inCalcVal)
    return;
       
  /* Calculate both, but calculate the requested set of
     stats first so that those values remain in the AttrAdj
     array. */
  if (!KnownOnly) {
    Creature::CalcValues(true,thrown);
    Creature::CalcValues(false,thrown);
  }
  else {
    Creature::CalcValues(false,thrown);
    Creature::CalcValues(true,thrown);
  }

  theGame->inCalcVal++;

  int8 a_hp = A_CON;
  if (one_body && Mod(A_WIS) > Mod(A_CON))
      a_hp = A_WIS; 

  cHP -= mHP; mHP = 20;
  int HDType = BestHDType();


  if (!HasStati(POLYMORPH)) {
    int lev = Level[0] + Level[1] + Level[2]; 
    int templateHD = lev;
    StatiIterNature(this,TEMPLATE)
        templateHD = TTEM(S->eID)->HitDice.Adjust(templateHD);
    StatiIterEnd(this)
    templateHD -= lev;

    if (templateHD > 0) {
      if (isPlayer() && thisp->Opt(OPT_MAX_HP) == 2) // max hp
        mHP += templateHD * max(1, Mod(a_hp) + HDType);
      else 
        mHP += templateHD * max(1, Mod(a_hp) + HDType/2);
    }

    if (HDType > 8) {
      if (isPlayer() && thisp->Opt(OPT_MAX_HP) == 2) // max hp
        mHP += lev * max(1, Mod(a_hp) + HDType);
      else 
        mHP += lev * max(1, Mod(a_hp) + HDType/2);
    } else {
      for (i=0;i!=3;i++)
        if (Level[i])
        {
          for (j=0;j!=Level[i];j++)
            mHP += max(1,Mod(a_hp) + thisp->hpRolls[i][j]);
        }
    } 
  } else {
    int numHD = TMON(mID)->HitDice; 
    StatiIterNature(this,TEMPLATE)
        numHD = TTEM(S->eID)->HitDice.Adjust(numHD);
    StatiIterEnd(this)

    // sadly, we can't really "roll" here 
    if (isPlayer() && thisp->Opt(OPT_MAX_HP) == 2) // max hp
      mHP += numHD * max(1, Mod(a_hp) + HDType);
    else 
      mHP += numHD * max(1, Mod(a_hp) + HDType/2);
  } 


  if (HasAbility(CA_TOUGH_AS_HELL))
    mHP += AbilityLevel(CA_TOUGH_AS_HELL) * max(0,Mod(a_hp));
  if (HasFeat(FT_TOUGHNESS))
    mHP += (mHP / 4);

  if (!HasMFlag(M_NO_SIZE_HP))
  switch(Attr[A_SIZ]) {
    case SZ_MINISCULE:      mHP /= 2; break;
    case SZ_TINY:           mHP = (mHP*7)/10; break;
    case SZ_SMALL:          mHP = (mHP*9)/10; break;
    case SZ_MEDIUM:         break;
    case SZ_LARGE:          mHP = (mHP*11)/10; break;
    case SZ_HUGE:           mHP = (mHP*13)/10; break;
    case SZ_GARGANTUAN:     mHP = (mHP*16)/10; break;
    case SZ_COLLOSAL:       mHP = (mHP*20)/10; break;
  }

  cHP += mHP;

  if (cHP <= 0 && !isDead()) {
    if (SavingThrow(FORT,20,SA_DEATH)) {
      IPrint("You stagger from the renewed seriousness of your injuries!");
      cHP = 1;
      }
    else {
      IDPrint("Belatedly, you drop dead.", "Belatedly, the <Obj> drops dead.", this);
      Throw(EV_DEATH,this,this);
      }
  }

  int16 a_mana = A_WIS;
  if (one_body && Mod(A_CON) > Mod(A_WIS))
      a_mana = A_CON; 

  mMana = 0;
  for (i=0;i!=3;i++)
    if (Level[i])
    {
      for (j=0;j!=Level[i];j++)
        mMana += thisp->manaRolls[i][j];
    }
    
  mMana *= ManaMultiplier[TotalLevel()];
  mMana += 10;
  mMana += Mod((int8)a_mana) * TotalLevel();
    
  mMana += AbilityLevel(CA_MAGICAL_NATURE);

  if (isPlayer() && theGame->InPlay() && thisp->MyTerm)
    thisp->ShowStats();

  // ww: calculate bonus spell slots ... this used to be in GainAbility,
  // but players can get frustrated if they read a Tome of Super Int +2 and
  // don't immediately get those bonus spells!
  uint8 intScaled = min(max(IAttr(A_INT)-9,0),21);
  // while were here, change the logic a bit (I know, I know ...) so that
  // your maximum number of bonus spells for spell level X is equal to the
  // number of normal spells you could get for level X (unless level 1, at
  // which point all) (and still limited by INT). Why? When a mage gets her
  // first 3rd level spell, she only gets one, and there's not much
  // variety. Typically there's some "best" spell that everyone has to take
  // (e.g., flame strike, stoneskin) first at high levels. This leads to
  // very little mage variety. And at high levels it takes a long time to
  // gain two more levels to get one more spell slot. Basically, with the
  // old algorithm I'm not sure I ever saw a caster get a bonus 3rd level
  // spell slot (you'd need to be 8th level). Anyway, now you'll get a
  // bonus spell right away and you'll have 4 total by the time you go up
  // two levels. 
  // fjm: Slight tweak here -- it's very possible to get more bonus spells
  //      at a level then you will ever get of regular spell. Change this
  //      to bonus slots == max(normal slots, number of levels you've
  //      advanced since first getting access to that specific slot.
  
    for (i=1;i<9;i++)
      BonusSlots[0] = 0;
  
    /* Give all bonus slots at 1st */
    if (SpellSlots[0]) {
      BonusSlots[0] = BonusSpells[intScaled][0];
      if (HasFeat(FT_GUILDMAGE))
        BonusSlots[0]++;
      }
    
    
    /* Scale slots for spell levels 2-9 */
    for (i=1;i<9;i++) { 
      BonusSlots[i] = min( BonusSpells[intScaled][i] ,
                               max( SpellSlots[i] ,
                               CasterLev() - SAL(i+1) ) ) ; 

      // back to auto-learning domain spells.
      // if (x == 1) XBonusSlots[x][i]++; // cleric domain slot!
      }
  
  StatiIterNature(this,BONUS_SLOTS)
    BonusSlots[S->Val-1] += S->Mag;
  StatiIterEnd(this)
  
  if (Attr[A_STR] != -1 && RaceID && ClassID[0])  
    RecalcCraftFormulas();

  CalcSP();

  theGame->inCalcVal--;

}



int16 Resists[16], ResistCount;

int16 Creature::ResistLevel(int16 DType, bool bypass_armour)
  {
    uint32 Res = TMON(mID)->Res, Imm = TMON(mID)->Imm;
    uint8  StatiResists[16]; int16 i,highest,highval,total;
    Item *it;
    ResistCount = 0;

    if (isIllusion())
      if (DType == AD_MIND || DType == AD_PLYS ||
          DType == AD_STON || DType == AD_STUN ||
          DType == AD_CONF || DType == AD_VAMP ||
          DType == AD_HOLY || DType == AD_EVIL ||
          DType == AD_LAWF || DType == AD_CHAO ||
          DType == AD_POIS || DType == AD_DISE ||
          DType == AD_SLEE || DType == AD_FEAR)
        return -1;

    StatiIterNature(this,TEMPLATE)
          TTemplate *tt = TTEM(S->eID);
          Res |= tt->AddRes;
          Res &= ~tt->SubRes;
          Imm |= tt->AddImm;
          Imm &= ~tt->SubImm;
    StatiIterEnd(this)


    switch (DType)
      {
        case AD_POIS:
          DType = AD_TOXI;
         break;
        case AD_DREX:
          DType = AD_NECR;
         break;
      }
    if (HasStati(IMMUNITY,DType))
      return -1;
    /* ww: unfortunate logic error here results in largely everything being
     * immune to my arrows (with attack * type 65)! was >= 32 ... */
    if (DType < 32)
      if (Imm & BIT(DType))
        return -1;
    if (DType < 32)
      if (Res & BIT(DType))
        {
          Resists[ResistCount] = 7  + (ChallengeRating()*2)/3;
          if (HasAbility(CA_BOOST_RESISTS))
            Resists[ResistCount] += AbilityLevel(CA_BOOST_RESISTS);
          ResistCount++;
        }
    
    for(i=0;i!=16;i++)
      StatiResists[i] = 0;
    
    StatiIterNature(this,RESIST)
      if (!S->Dis)
          if (S->Val == DType)
            StatiResists[S->Source] = 
              max(StatiResists[S->Source],S->Mag);
    StatiIterEnd(this)
    
    for(i=0;i!=16;i++)
      if (StatiResists[i])
        Resists[ResistCount++] = StatiResists[i];

    if((DType == AD_NECR ||
        DType == AD_HOLY ||
        DType == AD_LAWF ||
        DType == AD_CHAO ||
        DType == AD_EVIL) && HasFeat(FT_DIVINE_ARMOUR)
                          && HasStati(CHANNELING)) {
      Resists[ResistCount++] = Mod(A_CHA)*2;
    }
    if((DType == AD_FIRE ||
        DType == AD_COLD ||
        DType == AD_ELEC) && HasFeat(FT_DIVINE_RESISTANCE)
                          && HasStati(CHANNELING)) {
      Resists[ResistCount++] = Mod(A_CHA); 
    }

    if((DType == AD_TOXI || 
        DType == AD_POIS ||
        DType == AD_DISE) && HasFeat(FT_HARDINESS)) {
      Resists[ResistCount++] = 5; 
    } 

    if (DType == AD_CONF && HasStati(RAGING))
      if (HighStatiMag(RAGING) >= 7)
        return -1;
    if (DType == AD_STUN && HasStati(RAGING))
      if (HighStatiMag(RAGING) >= 13)
        return -1;
    
    if (is_wepdmg(DType)) {
      if (Attr[A_ARM] && !bypass_armour)
        Resists[ResistCount++] = max(0,Attr[A_ARM]);
      if ((it = EInSlot(SL_ARMOUR)) && !bypass_armour)
        Resists[ResistCount++] = ((Armour *)it)->ArmVal(DType - AD_SLASH);
      }

    // ww: a sword that comes in with AD_SLASH won't get a skeletons
    // resistance to DF_SLASH ...
    /*
    switch (DType)
      {
        case AD_SLASH: Resists[ResistCount++] =
                       ResistLevel(DF_SLASH,bypass_armour); break;
        case AD_PIERCE: Resists[ResistCount++] =
                       ResistLevel(DF_PIERCE,bypass_armour); break;
        case AD_BLUNT: Resists[ResistCount++] =
                       ResistLevel(DF_BLUNT,bypass_armour); break;
      }
      */
            
    if (!ResistCount)
      return 0;
    else if (ResistCount == 1)
      return Resists[0];

    total = 0;

    for(i=0,highest=0,highval=0;i!=ResistCount;i++)
      if (Resists[i] > highval)
        { highval = Resists[i]; highest = i; }

    total += highval;
    Resists[highest] = 0;
   
    for(i=0,highest=0,highval=0;i!=ResistCount;i++)
      if (Resists[i] > highval)
        { highval = Resists[i]; highest = i; }

    total += highval / 2;
    Resists[highest] = 0;
    
    

    for(i=0;i!=ResistCount;i++)
      total += Resists[i] / 3;

    return total;
}

int8 MonHDType(int8 MType)
  {
    switch (MType) {
      case MA_DEMON: 
      case MA_DEVIL: 
      case MA_CELESTIAL:
        return 20;
      //case MA_DINOSAUR:
      case MA_LICH:
        return 16;
      case MA_CONSTRUCT:
      case MA_DRAGON:
      case MA_VAMPIRE:  // ww: srd
      // case MA_SPIRIT: // ww: srd
      case MA_WRAITH: // ww: srd
      case MA_REVENANT:
      case MA_ZOMBIE:
        return 12;
      case MA_BEAST:
      case MA_ELEMENTAL: 
      case MA_OOZE: case MA_PUDDING: case MA_JELLY:  // ww: srd
        return 10;
      case MA_FAERIE:
      case MA_FUNGI:
      case MA_GOBLIN:
      case MA_YUAN_TI:
      case MA_ELEMENTALKIN:
        return 6;
      case MA_KOBOLD:
        return 4;
      case MA_FORCE_D4:
        return 104;
      case MA_FORCE_D6:
        return 106;
      case MA_FORCE_D8:
        return 108;
      case MA_FORCE_D10:
        return 110;
      case MA_FORCE_D12:
        return 112;
      case 0:
        return 0;
      default:
        return 8;
      }
  }

int8 MonGoodSaves(int8 MType)
  {
    switch (MType)
      {
        case MA_OUTSIDER: case MA_DEMON: case MA_DEVIL:
        case MA_DRAGON: case MA_GENIE:
          return XBIT(FORT) | XBIT(REF) | XBIT(WILL);
        case MA_BEAST: case MA_MIMIC:
          return XBIT(FORT) | XBIT(REF);
        case MA_ILLITHID: case MA_EYE: // case MA_SPIRIT:
        case MA_UNDEAD: 
          return XBIT(WILL);
        case MA_HUMANOID:
        case MA_NAGA: 
          return XBIT(REF) | XBIT(WILL);
        case MA_PLANT:
        case MA_ANIMAL:
        case MA_WORM:
        case MA_QUADRUPED:
        case MA_ELEMENTAL:
          return XBIT(FORT);        
        case MA_GOBLIN:
        case MA_KOBOLD:
        case MA_SPIDER:
        case MA_SNAKE:
        case MA_TRAPPER:
        case MA_VORTEX:
        case MA_FAERIE:
        case MA_BIRD:
          return XBIT(REF);
        case MA_FUNGI:
        default:
          return 0;
      }
  }
   
                            


void Creature::CalcHP()
{
  int8 HDType, HD; int16 i;
  bool one_body = HasFeat(FT_ONE_BODY_ONE_SOUL) || Attr[A_CON] == 0;
  if (TMON(tmID)->HasFlag(M_FIXED_HP))
  {
    mHP = TMON(mID)->HitDice;
    return;
  }
  HDType = BestHDType(); 

  if (mID)
    HD = (int8)TMON(mID)->HitDice; 
  else 
    HD = (int8)TMON(tmID)->HitDice;

  // IDPrint(NULL,Format("%s has %d hit dice.",(const char*)Name(NA_THE),HD));

  StatiIterNature(this,TEMPLATE)
    HD = (int8)TTEM(S->eID)->HitDice.Adjust(HD);
  StatiIterEnd(this)

  // ww: just like the "player max hp" option, we have a "monster max hp"
  // option
  int hpOpt = theGame->Opt(OPT_MON_MAX_HP);
  switch (hpOpt) {
    case 1: mHP= HD * HDType / 2; break;
    case 2: mHP= HD * HDType; break;
    case 3: mHP= HD * HDType * 2; break;
    default: mHP=Dice::Roll(HD,HDType); break;
  }

  // ww: this was in CalcValues with a !isCharacter guard ...
  mMana = TMON(mID)->Mana + AbilityLevel(CA_MAGICAL_NATURE) +
          GetAttr(A_MAN)*5;

  int manaDie = HasAttk(A_CAST) ||
    HasAbility(CA_SPELLCASTING) ? 12 : 4; 

  int effectiveManaLevel = 
    max(ChallengeRating() ,
        AbilityLevel(CA_SPELLCASTING));

  for (i=0; i<effectiveManaLevel; i++) {
    switch (hpOpt) {
      case 1: mMana += ManaMultiplier[i] * (manaDie / 2); break;
      case 2: mMana += ManaMultiplier[i] * manaDie; break;
      case 3: mMana += ManaMultiplier[i] * manaDie * 2; break;
      default:mMana += ManaMultiplier[i] * Dice::Roll(1,manaDie); break; 
    }
  }
  int8 a_hp = A_CON;
  if (one_body && Mod(A_WIS) > Mod(A_CON))
    a_hp = A_WIS;

  mHP = max(mHP/2,mHP + (HD * Mod(a_hp)));
  if (HasAbility(CA_TOUGH_AS_HELL))
    mHP += Mod(a_hp) * AbilityLevel(CA_TOUGH_AS_HELL);
  if (HasFeat(FT_TOUGHNESS))
    mHP += (mHP / 4);
  if (!HasMFlag(M_NO_SIZE_HP))
  switch(Attr[A_SIZ]) {
    case SZ_MINISCULE:      mHP /= 2; break;
    case SZ_TINY:           mHP = (mHP*7)/10; break;
    case SZ_SMALL:          mHP = (mHP*9)/10; break;
    case SZ_MEDIUM:         break;
    case SZ_LARGE:          mHP = (mHP*11)/10; break;
    case SZ_HUGE:           mHP = (mHP*13)/10; break;
    case SZ_GARGANTUAN:     mHP = (mHP*16)/10; break;
    case SZ_COLLOSAL:       mHP = (mHP*20)/10; break;
  }
  mHP = max(1,mHP);
  
  /* Knowledge skills give bonuses to summoned creatures'
     hit points. */
  if (Creature *summ = (Creature*) GetStatiObj(SUMMONED))
    if (int32 sr = summ->getBestKnowSkill(this,false)) {
        mHP += (mHP * (int16)sr * 5) / 100;
    }
      
  if (HasStati(HEROIC_QUALITY))
    mHP += 20;
  
}

int16 Creature::GetPower(int16 base)
  {
    int16 p;
    p = base;
    StatiIterNature(this,TEMPLATE)
        p = TTEM(S->eID)->Power.Adjust(base);
    StatiIterEnd(this)
    return p;
  }

void Character::CalcSP()
  {
    int16 i;
    for (i=0;i!=3;i++) {
      TClass *tc = TCLASS(ClassID[i]);
      if (tc) {
        if (tc->SkillPoints >= 8)
          TotalSP[i] = max(1, (tc->SkillPoints) + ((IAttr(A_INT)-10)/2)*2);
        else      
          TotalSP[i] = max(1, (tc->SkillPoints) + ((IAttr(A_INT)-10)/2));
        }
      else
        TotalSP[i] = 0;
      TotalSP[i] *= Level[i];
      TotalSP[i] += BonusSP[i];
      }
    TotalSP[3] = BonusSP[3];
    TotalSP[4] = BonusSP[4];
    TotalSP[5] = BonusSP[5];
  }

void TMonster::InitializeMTypeCache(rID tmid)
{
  int i;
  Monster * m = new Monster(tmid);
  m->CalcValues();
  memset(MTypeCache,0,sizeof(MTypeCache));
  for (i=0; i<MA_LAST; i++) {
    bool res = false;
    if (i == MType[0] || i == MType[1] || i == MType[2])
      res = true;
    else 
      res = m->isMType(i);
    
    if (res) 
      MTypeCache[i/8] |= (1 << (i % 8));
  } 
  delete m;
  return; 
} 

bool Creature::isMType(int32 mt) 
{
  /* IMPORTANT NOTE!
     The encounter generator sets up a fake Monster and calls its
     isMType() function when determining the suitability of templates
     and various other things. As such, it is important that this
     function not look at any member variables of Creature other
     then mID, TEMPLATE Stati, Attr[A_INT] and Attr[A_SIZ] without
     making sure that MonGroup::isMType is modified accordingly. */

  if (!mt)
    return true;

  if (mt > 256)
    return isMType(mt % 256) && isMType(mt / 256);

  /* Assigning some types to a creature (via templates, usually)
     removes others. A zombie rat is not subject to animal empathy,
     a skeleton dwarf is not living and an aqueous bandit cannot be 
     treated as a native creature. */
  if ((mt == MA_ANIMAL || mt == MA_SYLVAN ||
        mt == MA_LIVING || mt == MA_NLIVING ||
        mt == MA_PLANT) && HasStati(TEMPLATE)) {
    if (isMType(MA_UNDEAD))
      return false;
    if (isMType(MA_CONSTRUCT))
      return false;
    if (isMType(MA_OUTSIDER) && mt == MA_NLIVING)
      return false;
    if (isMType(MA_ELEMENTAL))
      return false;
    }
  
  /* Undead elves should NOT be friendly to an elf PC! */
  if (mt == MA_HUMAN || mt == MA_DWARF || mt == MA_GNOME ||
      mt == MA_HALFLING || mt == MA_GOBLINOID || mt == MA_GOBLIN ||
      mt == MA_KOBOLD || mt == MA_PERSON)
    if (isMType(MA_UNDEAD) || isMType(MA_CONSTRUCT))
      return false;
    

  if (TMON(tmID)->MType[0] == mt || 
      TMON(tmID)->MType[1] == mt ||
      TMON(tmID)->MType[2] == mt)
    return true;

  StatiIterNature(this,TEMPLATE)
      if (TTEM(S->eID)->AddMType == mt)
        StatiIterBreakout(this,return true)
      /* Zombie orcs are not "really" orcs anymore */
      /* ww: this is a big problem, 'cause it means that zombie orcs
       * aren't really "evil" anymore either! -- I expect many
       * subtle bugs to come from this design choice ... */
      if (TTEM(S->eID)->HasFlag(TMF_BODY_ONLY))
        if (mt != MA_UNDEAD && mt != MA_ZOMBIE && 
            mt != MA_OOZE && mt != MA_CONSTRUCT && 
            mt != MA_EVIL && mt != MA_GOOD)
          StatiIterBreakout(this,return false)
  StatiIterEnd(this)

  switch(mt) {
    case MA_LIVING:
      /* We include fungi here to avoid things like fungi zombies */
      if (isMType(MA_CONSTRUCT) || isMType(MA_VORTEX) || 
          isMType(MA_ELEMENTAL) || isMType(MA_ELEMENTALKIN) || 
          isMType(MA_FUNGI) || isMType(MA_UNDEAD))
        return false;
      return true;

    case MA_UNDEAD:     return HasMFlag(M_UNDEAD);

    case MA_HAS_BLOOD:
      if (HasMFlag(M_HAS_BLOOD))
        return true;
      else if (HasMFlag(M_NO_BLOOD)) 
        return false;
      else if (isMType(MA_UNDEAD) || isMType(MA_OOZE) ||
          isMType(MA_PLANT)  || isMType(MA_CONSTRUCT) ||
          isMType(MA_VORTEX) || isMType(MA_LIGHT) ||
          isMType(MA_ELEMENTAL)) 
        return false;
      else
        return true; 

    case MA_BREATHER:
      if (HasMFlag(M_NO_BREATH))
        return false; 
      else if (!isMType(MA_LIVING))
        return false; 
      else if (isMType(MA_PUDDING) || isMType(MA_JELLY) || 
          isMType(MA_FUNGI) || 
          /* ww: SRD: plants breathe:  isMType(MA_PLANT) || */ 
          isMType(MA_MIMIC))
        return false;
      return true;

    case MA_BURROW:     return HasMFlag(M_BURROW);
    case MA_AQUATIC:    return HasMFlag(M_AQUATIC) ||
                               HasMFlag(M_AMPHIB);
    case MA_SYLVAN:
      return (HasMFlag(M_NATURAL) || isMType(MA_ANIMAL)) &&
              HasMFlag(M_FOREST); 
    case MA_AVIAN:      return HasMFlag(M_FLYER);
    case MA_REPTILIAN:
      return isMType(MA_REPTILE) || isMType(MA_DRAGON) || isMType (MA_KOBOLD);
    case MA_GOBLINOID:
      return isMType(MA_ORC) || isMType(MA_GOBLIN) || isMType(MA_KOBOLD);

    case MA_SAPIENT:
      if (isMType(MA_ANIMAL))
        return false;
      if (HasMFlag(M_MINDLESS))
        return false;
      return Attr[A_INT] >= 3;

    case MA_PERSON:
      if (isMType(MA_FAERIE)) /* Fey aren't people */
        return false;
      if (!HasMFlag(M_HUMANOID))
        return false;
      /* Believe it or not, this is correct. A human is a person. A
         dragon is not. A human polymorphed into a dragon is also not,
         and thus is immune to charm person. However, a human who has
         benefitted from Enlarge or Righteous Might is still a person,
         and can still be charmed. Strange, neh? */
      if (TMON(tmID)->Size > SZ_MEDIUM)
        return false;
      if (HasMFlag(M_UNDEAD))
        return false;
      if (HasMFlag(M_MINDLESS))
        return false;
      return true;
    case MA_NONEVIL:
      return !isMType(MA_EVIL);
    case MA_NONGOOD:
      return !isMType(MA_GOOD);
    case MA_NONLAWFUL:
      return !isMType(MA_LAWFUL);
    case MA_NONCHAOTIC:
      return !isMType(MA_CHAOTIC);
    
    case MA_MOUNT:      return HasMFlag(M_MOUNTABLE);
    case MA_NATURAL:    return HasMFlag(M_NATURAL);
    case MA_NATIVE:     return (!isMType(MA_OUTSIDER));
    case MA_NLIVING:    return (!isMType(MA_OUTSIDER)) && isMType(MA_LIVING);

    case MA_WATER:      return HasMFlag(M_WATER);
    case MA_AIR:        return HasMFlag(M_AIR);
    case MA_EARTH:      return HasMFlag(M_EARTH);
    case MA_FIRE:       return HasMFlag(M_FIRE);
    case MA_COLD:       return HasMFlag(M_COLD);
    case MA_ACID:       return HasMFlag(M_ACID);
    case MA_MAGC:       return HasMFlag(M_MAGC);
    case MA_ELEC:       return HasMFlag(M_ELEC);
    case MA_TOXI:       return HasMFlag(M_TOXI);
    case MA_SUNL:       return HasMFlag(M_SUNL);
    case MA_SONI:       return HasMFlag(M_SONI);

    case MA_EVIL:
      if (HasStati(ALIGNMENT)) 
        return (GetStatiVal(ALIGNMENT) & AL_EVIL) != 0;
      else return HasMFlag(M_EVIL);
    case MA_GOOD:
      if (HasStati(ALIGNMENT)) 
        return (GetStatiVal(ALIGNMENT) & AL_GOOD) != 0;
      else return HasMFlag(M_GOOD);
    case MA_LAWFUL:
      if (HasStati(ALIGNMENT)) 
        return (GetStatiVal(ALIGNMENT) & AL_LAWFUL) != 0;
      else return HasMFlag(M_LAWFUL);
    case MA_CHAOTIC:
      if (HasStati(ALIGNMENT)) 
        return (GetStatiVal(ALIGNMENT) & AL_CHAOTIC) != 0;
      else return HasMFlag(M_CHAOTIC);
    case MA_NEUTRAL:
      return (!isMType(MA_GOOD) &&
              !isMType(MA_EVIL) &&
              !isMType(MA_LAWFUL) &&
              !isMType(MA_CHAOTIC));
    case MA_SUMMONED:           return HasStati(SUMMONED);

      /* basic MTypes */

    case MA_OUTSIDER:
      return isMType(MA_DEMON) || isMType(MA_DEVIL) ||
        isMType(MA_ELEMENTAL) || 
        isMType(MA_CELESTIAL) || isMType(MA_ELEMENTALKIN) ||
        isMType(MA_GENIE);
    case MA_VERMIN:
      if (isMType(MA_SPIDER) || isMType(MA_SNAKE))
        return true;
      return false;
    case MA_REPTILE:
      return isMType(MA_NAGA) || isMType(MA_SNAKE);
    case MA_ELEMENTALKIN:
      return isMType(MA_ELEMENTAL) || isMType(MA_GENIE);
    case MA_OOZE:
      return isMType(MA_PUDDING) || isMType(MA_JELLY);
    case MA_HUMANOID:
      return HasMFlag(M_HUMANOID);
    case MA_CONSTRUCT:
      return isMType(MA_GOLEM);
    case MA_PLANT:
      return isMType(MA_FUNGI);
    case MA_DEMIHUMAN:
      return isMType(MA_HUMAN) || isMType(MA_DWARF) ||
             isMType(MA_ELF) || isMType(MA_GNOME) ||
             isMType(MA_HALFLING) || isMType(MA_PLANETOUCHED);
    case MA_LAST:
      return false; 
    default:
      if (mt >= MA_LAST)
        Error("Strange MA_XXX constant sent to TMonster::isMType!");
      return false;
  }
}

String & Creature::BonusBreakdown(int8 at, int16 maxlen)
  {
    String Long, Short; int16 i; 
    Long.Empty(); Short.Empty();
    bool baseIsNatural = false; 
    bool one_body = HasFeat(FT_ONE_BODY_ONE_SOUL) || (Attr[A_CON] == 0);

    if (AttackMode() == S_THROWN && !thrownWep)
      if (at == A_SPD || at == A_HIT || at == A_DMG)
        return *tmpstr("(cannot find thrownWep)");
    if (AttackMode() == S_ARCHERY && !missileWep)
      if (at == A_SPD || at == A_HIT || at == A_DMG)
        return *tmpstr("(cannot find missileWep)");
        
    if (at == A_SPD) at = A_SPD_ARCHERY + AttackMode();
    if (at == A_HIT) at = A_HIT_ARCHERY + AttackMode();
    if (at == A_DMG) at = A_DMG_ARCHERY + AttackMode();
    if (at == A_DMG) at = A_SAV_FORT;
    

    String c[3];
    for (i=0;i<3;i++)
      c[i] = "Class"; 
    if (isCharacter()) {
      for (i=0;i<3;i++) 
        c[i] = NAME(thisc->ClassID[i]);
    } 
    Item *wp;
    switch(at) {
      case A_THP:
        break;
      case A_SIZ: 
        Long  = Format("(base %s",Lookup(SizeNames,AttrAdj[at][BONUS_BASE]));
        Short = Format("(B %s",Lookup(SizeNames,AttrAdj[at][BONUS_BASE]));
        break; 
      case A_STR: case A_DEX: case A_CON:
      case A_INT: case A_WIS: case A_CHA:
      case A_LUC: default: MonBase:
        Long  = Format("(base %d",AttrAdj[at][BONUS_BASE]);
        Short = Format("(B%d",AttrAdj[at][BONUS_BASE]);
       break;
      case A_HIT_ARCHERY: case A_HIT_MELEE:
      case A_HIT_THROWN:  case A_HIT_BRAWL:
      case A_HIT_OFFHAND:
      case A_SAV_FORT:    case A_SAV_REF:
      case A_SAV_WILL:
        if (!isCharacter())
          goto MonBase;
        Long  = Format("(%s %+d", (const char*)c[0], AttrAdj[at][BONUS_BASE]);
        Short = Format("(%+d", AttrAdj[at][BONUS_BASE]);
        if (isCharacter() && thisc->Level[1]) {
          Long  += Format("/ %s %+d", (const char*)c[1], AttrAdj[at][BONUS_CLASS2]);
          Short += Format("/%+d", AttrAdj[at][BONUS_CLASS2]);
          }
        if (isCharacter() && thisc->Level[2]) {
          Long  += Format("/ %s %+d", (const char*)c[2], AttrAdj[at][BONUS_CLASS3]);
          Short += Format("/%+d", AttrAdj[at][BONUS_CLASS3]);
          }
       break;
      case A_FAT: 
      case A_DEF:
        Long  = Format("(base %d",AttrAdj[at][BONUS_NATURAL]);
        Short = Format("(B%d",AttrAdj[at][BONUS_NATURAL]);
        baseIsNatural = true; 
        for(i=0;i!=3;i++) {
          if (!AttrAdj[at][BONUS_BASE+i])
            continue;
          Long  += Format(", %+d %s", AttrAdj[at][BONUS_BASE+i],(const char*)c[i]);
          Short += Format(", %+d", AttrAdj[at][BONUS_BASE+i]);
          }
       break;        
      case A_DMG_ARCHERY: 
        wp = missileWep;
        goto DamageBreakdown;
      case A_DMG_MELEE:
        wp = meleeWep;
        DamageBreakdown:
        Long = Format("(base %s / %s",(const char*)TITEM(wp->iID)->u.w.SDmg.Str(),
                                      (const char*)TITEM(wp->iID)->u.w.LDmg.Str());
        Short = Format("(%s/%s",      (const char*)TITEM(wp->iID)->u.w.SDmg.Str(),
                                      (const char*)TITEM(wp->iID)->u.w.LDmg.Str());
       break;
      case A_DMG_THROWN:  
        wp = thrownWep;
        goto DamageBreakdown;
      case A_DMG_OFFHAND:
        wp = offhandWep;
        goto DamageBreakdown;
      case A_MOV:
        Long = Format("(base %d~",AttrAdj[at][BONUS_BASE]*5+100);
        Short = Format("(%d~",AttrAdj[at][BONUS_BASE]*5+100);
       break;
      case A_SPD_ARCHERY:
        wp = missileWep;
        goto SpeedBreakdown;
      case A_SPD_MELEE:
        wp = meleeWep;
        goto SpeedBreakdown;
      case A_SPD_OFFHAND:
        wp = offhandWep;
        goto SpeedBreakdown;
      case A_SPD_THROWN:
        wp = thrownWep;
        goto SpeedBreakdown;
      case A_SPD_BRAWL:
        wp = NULL;
        SpeedBreakdown:
        baseIsNatural = true; 
        Long = Format("(base %d~", AttrAdj[at][BONUS_NATURAL]*5+100);
        Short = Format("(base %d~", AttrAdj[at][BONUS_NATURAL]*5+100);
        if (isCharacter()) {
          if (thisc->Level[0] && AttrAdj[at][BONUS_BASE]) {
            Long += Format(" %+d~ %s", AttrAdj[at][BONUS_BASE]*5,(const char*)c[0]);
            Short += Format(" %+d~",AttrAdj[at][BONUS_BASE]*5);
            }
          if (thisc->Level[1] && AttrAdj[at][BONUS_CLASS2]) {
            Long += Format(" %+d~ %s", AttrAdj[at][BONUS_CLASS2]*5,(const char*)c[1]);
            Short += Format(" %+d~",AttrAdj[at][BONUS_CLASS2]*5);
            }
          if (thisc->Level[2] && AttrAdj[at][BONUS_CLASS3]) {
            Long = Format(" %+d~ %s", AttrAdj[at][BONUS_CLASS3]*5,(const char*)c[2]);
            Short = Format(" %+d~",AttrAdj[at][BONUS_CLASS3]*5);
            }
          }
        else {
          }
       break;
      case A_MAN:
        Long = Format("(base %d", mMana);
        Short = Format("(base %d", mMana);
       break; 
      }
    for(i=3;i!=BONUS_LAST;i++) {
      if (baseIsNatural && i == BONUS_NATURAL) continue; 
      if (AttrAdj[at][i]) {
        if (percent_attr(at))
          {
            if (bonus_is_mult(at,i) || i == BONUS_NATURAL)
              continue;
            Long += Format(" %+d~ %s", AttrAdj[at][i]*5, Lookup(BonusNames,i));
            Short += Format(" %+d~%s", AttrAdj[at][i]*5, Lookup(BonusNicks,i));
          }
        else if (at == A_MAN)
          {
            Long += Format(", %+d %s", AttrAdj[at][i] * 5, Lookup(BonusNames,i));
            Short += Format(" %+d%s", AttrAdj[at][i] * 5, Lookup(BonusNicks,i));
          }
        else
          {
            if (i == BONUS_NATURAL && at == A_DEF)
              continue;
            Long += Format(", %+d %s", AttrAdj[at][i], Lookup(BonusNames,i));
            Short += Format(" %+d%s", AttrAdj[at][i], Lookup(BonusNicks,i));
          }
        if (i == BONUS_ATTR)
          switch(at) {
            case A_DMG_ARCHERY: case A_DMG_BRAWL:
            case A_DMG_MELEE: case A_DMG_OFFHAND:
            case A_DMG_THROWN:
              Long += "Str"; Short += "St";
             break;
            case A_HIT_ARCHERY: case A_HIT_THROWN:
            case A_SAV_REF: case A_SPD_ARCHERY:
            case A_SPD_BRAWL: case A_SPD_MELEE:
            case A_SPD_THROWN: case A_SPD_OFFHAND:
            case A_DEF:
              Long += "Dex"; Short += "Dx";
             break;
            case A_HIT_BRAWL: 
              if (HasFeat(FT_WEAPON_FINESSE) && Mod(A_DEX) >= Mod(A_STR))
                { Long += "Dex"; Short += "Dx"; }
              else
                { Long += "Str"; Short += "St"; }
             break;
            case A_HIT_MELEE: 
              if (HasFeat(FT_WEAPON_FINESSE) && Mod(A_DEX) >= Mod(A_STR) &&
                   meleeWep && meleeWep->canFinesse())
                { Long += "Dex"; Short += "Dx"; }
              else
                { Long += "Str"; Short += "St"; }
             break;
            case A_HIT_OFFHAND:
              if (HasFeat(FT_WEAPON_FINESSE) && Mod(A_DEX) >= Mod(A_STR) &&
                   offhandWep && offhandWep->canFinesse())
                { Long += "Dex"; Short += "Dx"; }
              else
                { Long += "Str"; Short += "St"; }
             break;


            case A_FAT: 
            case A_SAV_FORT:
            case A_SAV_WILL:
              if (at == A_SAV_WILL || (one_body && Mod(A_WIS) > Mod(A_CON)))
                { Long += "Wis"; Short += "Ws"; }
              else if (at == A_SAV_FORT || at == A_FAT || (one_body && Mod(A_CON) > Mod(A_WIS)))
                { Long += "Con"; Short += "Cn"; }
             break;
            }

        }
    }
    if (at != A_THP) {
      Long += ")"; 
      Short += ")";
      }
    if (percent_attr(at))
      for(i=3;i!=BONUS_LAST;i++) {
        if (baseIsNatural && i == BONUS_NATURAL) continue; 
        if (AttrAdj[at][i]) 
          if (bonus_is_mult(at,i)) {
            Long += Format(" x %d~ %s", 100+AttrAdj[at][i]*5, Lookup(BonusNames,i));
            Short += Format(" x %d~%s", 100+AttrAdj[at][i]*5, Lookup(BonusNicks,i));
            }
      }

    return *tmpstr((Long.GetLength() > maxlen) ? Short : Long);
  }

String & Player::SkillBreakdown(int8 sk, int16 maxlen)
  {
    return *tmpstr("");
  }

String & Player::SpellBreakdown(int8 sp, int16 maxlen)
  {
    return *tmpstr("");
  }

String & Player::ResistBreakdown(int8 dt, int16 maxlen)
  {
    return *tmpstr("");
  }
  
int16 Creature::GetBAB(int16 mode)
  {
    int16 BAB;
    BAB = TMON(tmID)->Hit;
    StatiIterNature(this,TEMPLATE)
      BAB = TTEM(S->eID)->Hit.Adjust(BAB);
    StatiIterEnd(this);
    return BAB; 
  }
  
int16 Character::GetBAB(int16 mode)
  {
    int16 i, BAB, sBAB, warriorLevels;
    BAB = 0; warriorLevels = 0;
    for(i=0;i!=3 && ClassID[i];i++) {
      BAB += (TCLASS(ClassID[i])->AttkVal[mode] * Level[i])/100;
      if (TCLASS(ClassID[i])->AttkVal[mode] >= 100)
        warriorLevels += Level[i];
      }
    sBAB = min(TotalLevel(), BAB + 
      min(warriorLevels,IntStudy[STUDY_BAB]));
    return max(BAB,sBAB);
  }

uint32 Creature::getArmourType(bool count_shield)
  {
    Item *arm; uint32 at;
    int16 i, sl[] = { SL_ARMOUR, SL_READY, SL_WEAPON, 0 };
    at = 0;
    
    for (i=0;sl[i];i++)
      {
        arm = InSlot(SL_ARMOUR);
        if (arm && (arm->isType(T_ARMOUR) || arm->isType(T_SHIELD)))
          at |= TITEM(arm->iID)->Group;
        if (!count_shield)
          break;
      }
      
    StatiIter(this)
      if (S->eID && (RES(S->eID)->Type == T_TEFFECT))
        {
          TEffect *te = TEFF(S->eID);
          if (te->HasFlag(EF_LARMOUR))
            at |= WG_LARMOUR;
          if (te->HasFlag(EF_MARMOUR))
            at |= WG_MARMOUR;
          if (te->HasFlag(EF_HARMOUR))
            at |= WG_HARMOUR;
        }
    StatiIterEnd(this)
    
    if (at & WG_HARMOUR)
      at |= WG_LARMOUR|WG_MARMOUR;
    else if (at & WG_MARMOUR)
      at |= WG_LARMOUR;
    
    return at;
  }
          
    