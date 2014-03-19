/* PLAYER.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Contains miscellaneous functions of the Player class,
   including the huge function that accepts the player's
   input and turn it into event calls. Also has the code
   governing resting and updating the dungeon over time,
   animal companions and party HD totals, and the options
   and switches subsystem.

     Player::Player(Term *t,int8 _Type)
     Character::Character(rID mID,int8 _Type)
     void Player::ChooseAction()
     EvReturn Player::Event(EventInfo &e)
     EvReturn Player::Rest(EventInfo &e)
     void Map::DaysPassed()
*/

#include "Incursion.h"

#ifdef WIN32
  #define OPT_FILE "\\Options.Dat"
#else
  #define OPT_FILE "/Options.Dat"
#endif


Player::Player(Term *t,int8 _Type)
	: Character(FIND("human;temp"),_Type)
{
  int16 i;
  for(i=0;i!=5;i++)
    RecentVerbs[i] = -1;
  GainStatiFromBody(mID);
  CalcHP();
  CalcValues();
  FFCount = 20;
  Journal = ""; 
  JournalInfo.bestItemVal = -1;
  JournalInfo.bestMonsterVal = -1;
  MyTerm=t;
  if (t)
    t->SetPlayer(this);
  memset(SpellKeys,-1,sizeof(SpellKeys));
  ResetLevelStats(); 
  if (m)
    m->RegisterPlayer(myHandle);
  ts.addCreatureTarget(this,TargetMaster); // PCs have free will!
  
  Macros[2] = FIND("Autorest");
  Macros[4] = FIND("Autobuff");
  Macros[7] = FIND("AutoLoot");
  Macros[8] = FIND("AutoDrop");
  
  StateFlags |= MS_KNOWN|MS_POLY_KNOWN|MS_SEEN_CLOSE|MS_SCRUTINIZED;
  
}

Player::~Player()
  {
    MyTerm->ClearPlayer();
    /* m->RemovePlayer(this); */
  }

void Player::UpdateOptions(bool specific_game)
  {
    int8 tOptions[OPT_LAST]; int16 i;
    MyTerm->ChangeDirectory(".");
    if (MyTerm->Exists(SC(MyTerm->OptionsSubDir()) + OPT_FILE))
      {
        MyTerm->OpenRead(SC(MyTerm->OptionsSubDir()) + OPT_FILE);
        MyTerm->FRead(tOptions,OPT_LAST);
        MyTerm->Close();
      }
    else
      {
        memset(tOptions,0,OPT_LAST);
        for (i=0;OptionList[i].Val;i++)
          tOptions[OptionList[i].Val] = OptionList[i].Default;
      }
    
    memcpy(&tOptions[OPC_INPUT],&Options[OPC_INPUT],100);
    memcpy(&tOptions[OPC_LIMITS],&Options[OPC_LIMITS],100);
    memcpy(&tOptions[OPC_DISPLAY],&Options[OPC_DISPLAY],100);
    if (!specific_game) {
      memcpy(&tOptions[OPC_TACTICAL],&Options[OPC_TACTICAL],100);
      memcpy(&tOptions[OPC_CHARGEN],&Options[OPC_CHARGEN],100);
      }
      
    MyTerm->OpenWrite(SC(MyTerm->OptionsSubDir()) + OPT_FILE);
    MyTerm->FWrite(tOptions,OPT_LAST);
    MyTerm->Close();
  }

void Player::LoadOptions(bool new_game)
  {
    int8 vOptions[OPT_LAST]; int16 i;
    memcpy(vOptions,Options,OPT_LAST);
    MyTerm->ChangeDirectory(".");
    if (MyTerm->Exists(SC(MyTerm->OptionsSubDir()) + OPT_FILE)) {
      MyTerm->OpenRead(SC(MyTerm->OptionsSubDir()) + OPT_FILE);
      MyTerm->FRead(Options,OPT_LAST);
      MyTerm->Close();
      }
    else {
      memset(Options,0,OPT_LAST);
      for (i=0;OptionList[i].Val;i++)
        Options[OptionList[i].Val] = OptionList[i].Default;
      } 

    if (new_game) {
      for (i=0;OptionList[i].Val;i++)
        #define in_cat(a) ((OptionList[i].Val >= a) && (OptionList[i].Val < (a+100)))
        if (in_cat(OPC_WIZARD) /*|| in_cat(OPC_TACTICAL)*/)
          Options[OptionList[i].Val] = OptionList[i].Default;
      }
    else {
      memcpy(&Options[OPC_WIZARD],&vOptions[OPC_WIZARD],100);
      memcpy(&Options[OPC_TACTICAL],&vOptions[OPC_TACTICAL],100);
      memcpy(&Options[OPC_CHARGEN],&vOptions[OPC_CHARGEN],100);
      }
  }

void Player::ResetOptions()
  {
    int16 i;
    memset(Options,0,OPT_LAST);
    for (i=0;OptionList[i].Val;i++)
      Options[OptionList[i].Val] = OptionList[i].Default;
    MyTerm->ChangeDirectory(".");
    MyTerm->OpenWrite(SC(MyTerm->OptionsSubDir()) + OPT_FILE);
    MyTerm->FWrite(Options,OPT_LAST);
    MyTerm->Close();
  }    

Character::Character(rID mID,int8 _Type)
	: Creature(mID,_Type)
	{

	}



void Player::ChooseAction()
{
  int16 k,ch,sp,cdir,n,itype,ev,gx,gy; int16 dx,dy; const char *which, *haveno;
  Item *i; Thing *targ; Feature *feat; bool found; String oName, s; hObj h;
  static int16 oldHP; String str, msg;
  EventInfo e; static int16 LastAutoSave = 0;

  int16 look_dx = 0, look_dy = 0;

  GameTimeInfo.actions++; 

  //IPrint(Format("Chill Shield: %d/%d %d %d.",
  //  TEFF(FIND("chill shield")) - theGame->Modules[0]->QEff,
  //  TEFF(FIND("chill shield"))->Vals(0)->aval,
  //  TEFF(FIND("chill shield"))->Vals(1)->aval,
  //  TEFF(FIND("chill shield"))->Vals(2)->aval));

  // ww: one problem with the old approach was that you could spend five
  // minutes shuffling your backpack but not have it count as any "moves" 
  // now we save every keystroke, which is closer to saving ever 2 minutes
  // or whatnot
  MyTerm->ActionsSinceLastAutoSave++;
  if (MyTerm->ActionsSinceLastAutoSave >= 200) {
    MyTerm->ActionsSinceLastAutoSave = 0;
    // AddJournalEntry("Checkpoint.");
    theGame->SetAutoSaveFlag();
    } 
  
  memset(AngerThisTurn,0,sizeof(AngerThisTurn[0])*MAX_GODS);
  
  e.Clear();
  SinglePrintXY(e,NULL);

  // ww: each option gradient is 10% 
  bool lowHP = cHP * 10 <= (mHP + Attr[A_THP]) * Opt(OPT_LOWHP_WARN);
  if (lowHP) {
    str = Format("Low Hitpoint Warning! \n%d / %d = %d%% ",
        cHP, (mHP + Attr[A_THP]),
        100 * cHP / (mHP + Attr[A_THP]));
    if (Opt(OPT_LOWHP_AGG)) {
      str += "\n\nPress [ENTER] to continue.";
      MyTerm->Box(WIN_SCREEN,BOX_MUST_PRESS_ENTER,RED,GREY,str);
      oldHP = cHP;
      } 
    else if (oldHP != cHP) {
      MyTerm->Box(WIN_SCREEN,0,RED,GREY,str);
      oldHP = cHP;
      }
    } 
  if (!EInSlot(SL_WEAPON) && Opt(OPT_WARN_EMPTY_HAND)) {
    if (HasAbility(CA_UNARMED_STRIKE) || HasStati(POLYMORPH))
      ;
    else {
      MyTerm->Box(WIN_SCREEN,BOX_MUST_PRESS_ENTER,RED,GREY,
          "You are empty-handed!\n\nPress [ENTER] to continue.");
    } 
  } 

  if (HasStati(CHARMED,CH_COMPEL))
    StatiIterNature(this,CHARMED)
      if (S->Val == CH_COMPEL)
        {
          EvReturn r;
          PEVENT(EV_COMPULSION,this,S->eID,0,r);
          if (Timeout)
            return;
        }
    StatiIterEnd(this)

  if (HasStati(PARALYSIS)) {
    IsParalyzed:
    switch(ChoicePrompt("You are paralyzed. Pray, magic or wait 10/20/30?", "pm123", '1'))
      {
        case 'p': goto DoPrayer;
        case 'm': goto DoMagic;
        case '1': Timeout += 10; return;
        case '2': Timeout += 20; return;
        case '3': Timeout += 30; return;
        case -1:
          goto IsParalyzed;
      }
    return;
    }

  if (InSlot(SL_INAIR))
    MyTerm->SetMode(MO_INV);

  if (MyTerm->GetMode() == MO_INV)
    {
      MyTerm->InventoryManager(false);
      if (Timeout)
        return;
    }
    
  if (Opt(OPT_AUTOHIDE) && HasSkill(SK_HIDE) && !HasStati(HIDING)
       && !m->BrightAt(x,y) && !HasStati(ILLUMINATED)
       && !(TTER(m->TerrainAt(x,y))->HasFlag(TF_WATER) ||
               HasFeat(FT_FEATHERFOOT))
       && !HasStati(MANIFEST))
    {
      Creature *cr; int32 i;
      /* Replacement isThreatened() prevents you from AutoHiding
         when obsreved by enemies you can't see. */
      MapIterate(m,cr,i)
        if (cr->isCreature() && cr->DistFrom(this) <= 16)
          if (cr->isHostileTo(this))
            if (m->LineOfFire(cr->x,cr->y,x,y,cr))
              goto SkipAutoHide;

      IPrint("AutoHide:");
      ThrowVal(EV_HIDE,HI_SHADOWS,this);
      if (Timeout)
        return;
      SkipAutoHide:;
    }

  do
  {
    if (HasStati(PARALYSIS))
      goto IsParalyzed;
    AutoPickupFloor();
    // MyTerm->RecreateViewList();

    MyTerm->ShowStatus();
    MyTerm->ShowViewList();
    if (shownFF != isFlatFooted())
      MyTerm->ShowTraits();
    if (Flags & F_DELETE)
    {
      Timeout += 1;
      return;
    }
    if (HungerShown != HungerState()) {
      HungerShown = HungerState();
      MyTerm->ShowTraits();
    }
    Silence = 0;
    if (m && m->QueueNum())
    Error("Messages (still) queued at beginning of turn.");
    MyTerm->ClearKeyBuff();
    ch = MyTerm->GetCharCmd();
    if (StateFlags & MS_CASTING)
      {
        StateFlags &= ~(MS_CASTING|MS_STILL_CAST);
        statiChanged = true;
      }
    cdir = -1;
    if (HasStati(CHARGING)) {
      cdir = GetStati(CHARGING)->Val;
      if ((ch < KY_CMD_FIRST_ARROW || ch > KY_CMD_LAST_ARROW) && ch != KY_ESC && 
          ch != KY_SPACE && ch != KY_CMD_NAME && KY_CMD_WIZMODE &&
          ch != KY_CMD_SHOW_MESSAGES && ch != KY_CMD_CHAR_SHEET && ch != KY_CMD_HELP 
          && ch != KY_CMD_OPTIONS && ch != KY_CMD_ATTACK_MENU)
      {
        if (HasStati(AUTO_CHARGE))
          ;
        else if (!yn("Break off your charge?"))
          continue;
        IDPrint(NULL,"The <Obj1> breaks off <his:Obj1> charge.",this);
        RemoveStati(CHARGING);
      }
    }                     
    switch (ch)
    {
      case KY_CMD_LOOK_NORTH: 
      case KY_CMD_LOOK_NORTHWEST: 
      case KY_CMD_LOOK_NORTHEAST: 
      case KY_CMD_LOOK_SOUTH: 
      case KY_CMD_LOOK_SOUTHWEST: 
      case KY_CMD_LOOK_SOUTHEAST: 
      case KY_CMD_LOOK_EAST: 
      case KY_CMD_LOOK_WEST: 
              look_dx += DirX[DIR_OF_KY_CMD_LOOK(ch)] * 5;
              look_dy += DirY[DIR_OF_KY_CMD_LOOK(ch)] * 5;
              MyTerm->AdjustMap(x + look_dx, y + look_dy, true); 
              break; 

      case KY_CMD_NAME:
        e.Clear();
        if (!MyTerm->EffectPrompt(e,Q_TAR | Q_INV))
          break;
        if (!(targ = e.ETarget))
          break;
        if (targ->Named.GetLength())
          if (!yn(Format("Confirm rename %s? [yn]",(const char*)targ->Named),true))
            break;
        if (targ->isCreature()) {
          if (TMON(((Creature*)targ)->mID)->HasFlag(M_PROPER) ||
              TMON(((Creature*)targ)->mID)->HasFlag(M_PROPER))
          {
            IPrint("<Obj> already has a name.",targ);
            break;
          } 
        } 
        if (targ->isItem()) {
          /*if (((Item*)targ)->aID || TEFF(((Item*)targ)->eID)->HasFlag(EF_UNIQUE))
            {
            IPrint("<Obj> already has a name.",targ);
            break;
            } 
            */ }
          oName = MyTerm->StringPrompt(YELLOW,XPrint("What do you want to name the <Obj>? ",targ));
        if (oName.GetLength())
        {
          oName.SetAt(0,toupper(oName[0]));
          targ->Named = oName;
        }
        break;
      case KY_CMD_JOURNAL:
          oName = MyTerm->StringPrompt(YELLOW,"Journal Entry? ");
        if (oName.GetLength())
          AddJournalEntry(oName);
        break; 
      case KY_CMD_UP:
        Throw(EV_ASCEND,this);
        break;
      case KY_CMD_DOWN:
        Throw(EV_DESCEND,this);
        break;
      case KY_CMD_ENTER:
        if (m->KnownFeatureAt(x,y))
          if (m->KnownFeatureAt(x,y)->Type == T_PORTAL)
            if (((Portal*)m->KnownFeatureAt(x,y))->EnterDir(CENTER))
            {
              if (Throw(EV_ENTER,this,m->KnownFeatureAt(x,y)) == DONE)
                return;
              break;
            }
        IPrint("You can't go in anything here.");
        break;
      case KY_CMD_HELP:
        MyTerm->HelpTopic(NULL); 
        break;
      case KY_CMD_LEGEND: 
        MyTerm->DisplayLegend();
        break; 
      /*
      case KY_CMD_DEBUG_MESSAGES:
        MyTerm->ShowDebugMessages();
        break;
      */
      case KY_CMD_LOOK:
        e.Clear();
        if (isBlind())
          // "No, see, 'cause you're *blind*."
          MyTerm->EffectPrompt(e,Q_TAR | Q_LOC,true,"Sensing --");
        else
          MyTerm->EffectPrompt(e,Q_TAR | Q_LOC,true,"Looking --");
        break; 
      case KY_CMD_TREAT_MONSTER:
        e.Clear();
        if(!MyTerm->EffectPrompt(e,Q_TAR|Q_CRE))
          break;
        targ =e.ETarget;
        if (!targ || !targ->isCreature()) {
          IPrint("There's nothing there to become hostile to.");
          break;
        } else {
          IPrint("You now view the <Obj> as an enemy.",targ);
          ts.addCreatureTarget((Creature *)targ, TargetEnemy);
          Thing *t; int i;
          MapIterate(m,t,i) 
            if (t->isCreature())
              ((Creature *)t)->ts.Retarget((Creature *)t);
        } 
        break; 
      case KY_CMD_TALK:
        e.Clear();
        if(!MyTerm->EffectPrompt(e,Q_TAR|Q_CRE|Q_ALL))
          break;
        if (e.isDir) {
          targ = m->FCreatureAt(x + DirX[e.EDir], y + DirY[e.EDir]);
          if (!targ) {
            IPrint("There's nothing there to talk to.");
            break;
            }
          }
        else if (e.isAllAllies) {
          e.EActor = this;
          ReThrow(EV_ORDER,e);
          break;
          }
        else
          targ =e.ETarget;
          
        Throw(EV_TALK,this,targ);
        break;
      case KY_CMD_EAT:
        UseItemMenu("What do you want to eat?", "You have no comestables.", -2);
        break;
      case KY_CMD_ACTIVATE:
        UseItemMenu("Which magic item do you wish to activate?", 
            "You have no actively usable wonderous items.", -1);
        break;
      case KY_CMD_READ:
        UseItemMenu("Which scroll do you wish to read?", 
            "You have no scrolls to read.", T_SCROLL);
        break;
      case KY_CMD_BLAST_WAND:
        UseItemMenu("Which staff or wand do you wish to blast?", 
            "You have no staves or wands to blast.", T_WAND);
        break;
      case KY_CMD_QUAFF:
        UseItemMenu("Which potion do you wish to quaff?", 
            "You have no potions to quaff.", T_POTION);
        break;
      case KY_CMD_YUSE:
        YuseMenu();
        break;

      case KY_CMD_OPEN:
        if (HasMFlag(M_NOHANDS))
          {
            IPrint("But you have no hands!");
            break;
          }
        if (i = m->FChestAt(x,y))  
          if (yn("Open a container?"))
          { // ww: unfortunately, there may be many chests
OpenChest:
            Thing *t = oThing(m->At(x,y).Contents); 
            int32 ic = 1;
            while (t) {
              if (t->isType(T_CHEST)) {
                LegendIdent(oItem(t->myHandle));
                ((Item *)t)->Inscrip = NumberNames[ic++];
              }
              t = oThing(t->Next);
            }
            if (ic > 2) {
              t = oThing(m->At(x,y).Contents); 
              while (t) {
                if (t->isType(T_CHEST))
                  MyTerm->LOption(t->Name(NA_LONG|NA_MECH),t->myHandle,
                                0, ic);
                t = oThing(t->Next);
              }
              ic = MyTerm->LMenu(MENU_ESC|MENU_BORDER,
                              "Loot which chest?",WIN_MENUBOX);
              if (ic == -1) break;
              i = theRegistry->GetItem(ic);
            }
            e.Clear();
            e.EActor = this; 
            if (i) {
              ((Container*)i)->PickLock(e); 
              MyTerm->InventoryManager(true,(Container*)i);
              }
            break;
          }
        e.Clear();
        e.EDir = -1;
        for(n=0;n!=8;n++)
          if (feat = m->FDoorAt(x + DirX[n], y + DirY[n]))
            if (feat->Type == T_DOOR)
            {
              if (((Door*)feat)->DoorFlags & DF_SECRET)
                continue;
              if (e.EDir != -1)
              {
                if(!MyTerm->EffectPrompt(e,Q_DIR))
                  break;

                goto DoOpen;
              }
              e.EDir = n;
            }
        if (e.EDir == -1)
        {
NoDoor:
          IPrint("There's nothing there to open.");
          break;
        }

DoOpen:      
        feat = m->FDoorAt(x + DirX[e.EDir], y + DirY[e.EDir]);
        if (!feat)
          goto NoDoor;
        if (((Door*)feat)->DoorFlags & DF_OPEN)
          Throw(EV_CLOSE,this,feat);
        else
          Throw(EV_OPEN,this,feat);
        break;
      case KY_CMD_FIRE:
ShiftDirFire:
        if (cdir != -1)
        {
          if (HasStati(AUTO_CHARGE))
            ;
          if (!yn("Break off your charge?"))
            continue;
          IDPrint(NULL,"The <Obj1> breaks off <his:Obj1> charge.",this);
          RemoveStati(CHARGING);
        }
        e.Clear();
        e.EActor = this;
        found = false;
        if (ch >= KY_CMD_FIRST_ARROW && ch <= KY_CMD_LAST_ARROW)
        {
          if (!defAmmo) {
            IPrint("You must select a default projectile before using the ranged attack shortcut.");
            break;
          }
          for (k=0;k!=SL_LAST;k++)
            if (defAmmo == Inv[k])
              goto FoundDefAmmo;
          IPrint("Your default projectile isn't easily accessable right now.");
          break;
FoundDefAmmo:
          i = oItem(Inv[k]);
          e.isDir = true;
          e.EDir = DIR_OF_KY_CMD(ch);

          if (i->isType(T_MISSILE))
          {
            if (!(EInSlot(SL_WEAPON) && EInSlot(SL_WEAPON)->isType(T_BOW) &&
                  TITEM(EInSlot(SL_WEAPON)->iID)->HasRes(i->iID,AN_FIRES)))
            {
              IPrint("You don't have the proper weapen equipped to fire <Str>.",
                  (const char*)i->Name(NA_A|NA_SINGLE));
              break;
            }
            if (EInSlot(SL_WEAPON)->needsToBeCocked())
              {
                Throw(EV_LOADXBOW,this,NULL,InSlot(SL_WEAPON));
                return;
              }
            e.EItem  = EInSlot(SL_WEAPON);
            e.EItem2 = i;
          }
          else
          {
            /*
            if (EInSlot(SL_WEAPON) && EInSlot(SL_READY))
              if (EInSlot(SL_WEAPON) != i && EInSlot(SL_READY) != i)
                {
                  IPrint("You need a free hand to throw things!");
                  return;
                }*/
            e.EItem  = NULL;
            e.EItem2 = i;
          }
        }
        else
        {
          if (EInSlot(SL_WEAPON) && EInSlot(SL_WEAPON)->isType(T_BOW))
          {
      // ww: in the exceptionally common case, you have only one stack of
      // valid ammo and we can skip this question 
            int foundCount = 0; 
            Item * foundItem = NULL;
            if (InSlot(SL_WEAPON)->needsToBeCocked())
              {
                Throw(EV_LOADXBOW,this,NULL,InSlot(SL_WEAPON));
                return;
              }

            for(n=0;n!=SL_LAST;n++)
              if (EInSlot(n) && EInSlot(n)->isType(T_MISSILE))
                if (TITEM(EInSlot(SL_WEAPON)->iID)->HasRes(EInSlot(n)->iID,AN_FIRES))
                {
                  foundCount++;
                  foundItem = EInSlot(n);
                  MyTerm->LOption(InSlot(n)->Name(0),EInSlot(n)->myHandle);
                }

            if (foundCount == 0) {
              IPrint("You have no projectiles ready for your <Obj>.",
                  EInSlot(SL_WEAPON));
              break;
            } else if (foundCount == 1) {
              MyTerm->LOptionClear();
              e.EItem2 = foundItem; 
            } else { 
              h = MyTerm->LMenu(MENU_ESC|MENU_BORDER,XPrint("Fire what from your <Obj>?",
                    EInSlot(SL_WEAPON)), WIN_MENUBOX, "help::commands");
              if (h == -1)
                break;
              e.EItem2 = oItem(h);
            }
            e.EItem  = EInSlot(SL_WEAPON);
          }
          else
          {
            for(n=0;n!=SL_LAST;n++)
              if (EInSlot(n) && (InSlot(n)->HasIFlag(IT_THROWABLE) ||
                    (EInSlot(n)->RangeInc(this) && !EInSlot(n)->isType(T_BOW))))
              {
                found = true;
                MyTerm->LOption(EInSlot(n)->Name(0),EInSlot(n)->myHandle);
              }

            if (!found)
            {
              IPrint("You have no ranged weapons ready.");
              break;
            }
            /*
                if (EInSlot(SL_WEAPON) && EInSlot(SL_READY))
                if (EInSlot(SL_WEAPON) != i && EInSlot(SL_READY) != i)
                {
                IPrint("You need a free hand to throw things!");
                return;
                }
              */
            h = MyTerm->LMenu(MENU_ESC|MENU_BORDER,"Throw what?", WIN_MENUBOX, "help::commands");
            if (h == -1)
              break;
            e.EItem  = NULL;
            e.EItem2 = oItem(h);
          }

          if (!MyTerm->EffectPrompt(e,Q_DIR | Q_TAR | Q_LOC |Q_CRE))
            break;
        }

        ReThrow(EV_RATTACK,e);
        return;
      case KY_CMD_PRAY:
        DoPrayer:
        Throw(EV_PRAY,this);
        break;
      case KY_CMD_OVERVIEW_MAP:
        MyTerm->ShowMapOverview(); 
        break; 

      case KY_CMD_GET:
        gx = x; gy = y;
        if (HasMFlag(M_NOHANDS))
          {
            IPrint("But you have no hands!");
            break;
          }
        if (i = m->FChestAt(x,y)) {
          if (yn("Open a container?"))
            goto OpenChest;
          }
        if (HasStati(TELEKINETIC))
          {
            e.Clear();
            e.vRange = GetStatiMag(TELEKINETIC);
            if (thisp->MyTerm->EffectPrompt(e,Q_LOC,false,"Get an item:"))
              { gx = e.EXVal; gy = e.EYVal; }
            else
              return;
          }
                  
        

        if (m->MItemAt(gx,gy)) {
          targ = m->FItemAt(gx,gy);
          while (targ) {
            if (targ->isItem()) {
              LegendIdent(oItem(targ->myHandle));
              int32 sortKey = 0;
              switch (Opt(OPT_SORT_PILE)) {
                case 1: /* type */ sortKey = targ->Type; break;
                case 2: /* quality */ 
                        sortKey = -10 - ((Item *)targ)->PItemLevel(this); break; 
                default: /* name */ break;
              } 
              MyTerm->LOption(targ->Name(NA_LONG|NA_MECH),targ->myHandle,
                              0, sortKey);
            }
            targ = m->NItemAt(gx,gy);
          }
          h = MyTerm->LMenu(MENU_ESC|MENU_SORTED|MENU_BORDER,
                          "Pick up what?",WIN_MENUBOX);
          if (h == -1)
            break;
          i = oItem(h); 
        }
        else if (!(i=m->FItemAt(gx,gy)))
          break;
        ChosenGetItem:
        if (i->GetQuantity() > 1 && MyTerm->AltDown())
          i = OneSomeAll(i,this);
        if(Throw(EV_PICKUP,this,NULL,i)!=ABORT)
          { if (gx!=x || gy!=y)
              IPrint("The <Obj> flies through the air and into your grip!",i);
            return; }
        break;
      case KY_CMD_HIDE:
        if (HasStati(HIDING)) {
          IPrint("You step out of the shadows.");
          RemoveStati(HIDING,-1,-1);
          }
        else 
          ThrowVal(EV_HIDE,HI_SHADOWS,this);
        return;
      case KY_CMD_WIZMODE:
        if (!WizardMode)
        {
          #ifndef DEBUG
          if (Opt(OPT_DISALLOW)) {
            IPrint("Wizard mode has been disallowed in this game.");
            break;
            }
          #endif
          if (!yn("Enter wizard mode?",true))
            break;
          WizardMode = true;
        }
        WizardOptions();
        break;
      case KY_CMD_INVENTORY:
        MyTerm->InventoryManager(true,NULL);
        return;
        break;
      case KY_CMD_SHOW_MESSAGES:
        MyTerm->ShowMessages();
        break; 
      case KY_CMD_MAGIC:
        /* ww: true, but let them look at the spell manager anyway
        if (inField(FI_SILENCE) && !HasFeat(FT_VOCALIZE_SPELL))
        {
          IPrint("You can't cast spells in areas of magical silence.");
          break;
        }
        */
        DoMagic:
        sp = MyTerm->SpellManager(SM_CAST);
        if (sp == -1)
          return;
CastSpell:
        e.Clear();
        e.EActor = this;
        e.eID = theGame->SpellID(sp);
        if (!MyTerm->EffectPrompt(e,TEFF(e.eID)->ef.qval |
              ((MMFeats(sp) & MM_PROJECT) ? Q_DIR : 0)))
          break;
        if (Spells[sp] & SP_INNATE) 
          ReThrow(EV_INVOKE,e);
        else 
          ReThrow(EV_CAST,e);
        return;
      case KY_CMD_CHAR_SHEET:
        MyTerm->DisplayCharSheet();
        break;
      case KY_CMD_QUICK_QUIT:
        theGame->SetSaveFlag();
        Timeout = 1;
        return;
      case KY_CMD_ESCAPE:
        MyTerm->LOption("Save and Quit",1);
        MyTerm->LOption("Save and Continue",2);
        MyTerm->LOption("Quit Game",3);
        switch (MyTerm->LMenu(MENU_ESC|MENU_BORDER,"System Menu"))
        {
          case 1:
            theGame->SetSaveFlag();
            Timeout = 1;
            return;
          case 2:
            theGame->SetAutoSaveFlag();
            Timeout = 1;
            return;
          case 3:
            if(MyTerm->yn("Really Quit?"))
            {
              SetSilence();
              Flags |= F_DELETE;
              QuitFlag = true;
              Timeout = 60;
              return;
            }
            break;
          default:
            break;
        }
        break;
      case KY_CMD_SEARCH:
        GainTempStati(ACTING,NULL,25,SS_MISC,EV_SEARCH);
        Timeout++; 
        break;
      case KY_CMD_ATTACK_MENU:
        MyTerm->LOption("Attack",A_SWNG, "Use this option to perform a normal"
            " attack against a creature who is not yet hostile to you (Walking"
            " into these characters will normally just displace them) or is"
            " invisible or otherwise imperceptable.");
        MyTerm->LOption("Break Grapple",A_ESCA,"You attempt to break out of an"
            " enemy's grab or grapple with a grapple check. If successful, this"
            " maneuver takes no time to complete, giving you an opportunity to"
            " get out of melee range, but if failed it takes up a full round.");
        MyTerm->LOption("Bull Rush",A_BULL, "Attempt to force an opponent"
            " back one square by charging into them. This is resolved with an"
            " oppposed Strength roll modified by +4/-4 for every difference in"
            " size catagory between you and your opponent. A bull rush normally"
            " provokes an attack of oppurtonity from the target.");
        MyTerm->LOption("Called Shot",A_CALL,XPrint("You aim an attack at a specific"
              " part of an opponent's body, hoping to inflict a debilitating injury"
              " rather then just going for the kill. <13>Not implemented yet.<2>"));
#if 1
        MyTerm->LOption("Charge",A_CHAR,
            "__You run (or ride) in a straight line, gaining a +2 bonus to hit and "
            "accumulating plusses to damage due to momentum as you move. Your damage "
            "bonus is based on your mass (size, equipment and mount) times your velocity "
            "(movement rate). These bonuses apply only to melee attacks. Some weapons "
            "and feats double or triple the damage bonus that charging grants, and being "
            "mounted also increases the effect. Hitting something with a weapon while "
            "charging deals blunt damage to that weapon equal to your charge damage bonus."
            "\n__You are always subject to Sneak Attacks while charging. You may not make "
            "attacks of opportunity while charging. You are denied your Dexterity bonus "
            "to Defense while charging."
            "\n__If you charge near an opponent that has reach, it will set itself to "
            "receive your charge, gaining a free attack of opportunity against you. In "
            "this attack it gains your charge bonus to damage against you (momentum is "
            "symmetric). If you take any damage in that attack, you must save versus DC "
            "30 to avoid being tripped. If you do not take any damage, you automatically "
            "close with that opponent.");
#else
        MyTerm->LOption("Charge",A_CHAR,"You run in a straight line, accumulating"
            " plusses to hit and damage due to momentum as you move. You are considered" 
            " to be exposed while charging, and for one turn thereafter. Some weapons"
            " double or triple the damage bonus that charging grants, and being mounted"
            " also increases the effect.");
#endif
        MyTerm->LOption("Charge, Automatic",AUTO_CHARGE,
            " You automatically Charge when moving. Whenever you change directions you break off your current charge and start a new one. This can be convenient if you are mounted and trying to run a fleeing enemy down with a lance, for example.");
        MyTerm->LOption("Coup de Grace",A_COUP,"Deliver a killing blow to a"
            " paralyzed or sleeping living humanoid opponent within 5 feet of"
            " you. The attack is an automatic critical, and even if the foe"
            " survives the damage they must make a Fortitude save (DC 10 + half"
            " of the damage dealt) or die anyway. Performing a Coup de Grace"
            " provokes attacks of oppourtunity from every non-paralyzed or"
            " sleeping enemy within 1 square.");
        MyTerm->LOption("Disarm",A_DISA,"You cause an opponent to lose their"
            " weapon. An opposed attack roll is made, using only Base Attack "
            " Bonus, modified by both Strength and Dexterity, as well as +/- 4 "
            "for weapon size differences. Success"
            " causes the opponent to lose their weapon; failure means they are"
            " allowed to make an immediate Disarm attempt against you, without"
            " it costing them an action. A Disarm attempt normally provokes an"
            " attack of opportunity.");
        MyTerm->LOption("Fight Defensively",A_DEFN, "By devoting your full attention"
            " to defense, you can gain a +4 circumstance bonus to your defense class"
            " at the expense of suffering a -4 circumstance penalty to attack rolls,"
            " -2 to damage, 75~ normal movement rate and a -20% spell success chance."
            " Once fighting defensively, you can stop by using the 'x' command."); 
        MyTerm->LOption("Grapple",A_GRAB,"You grab your opponent and initate a"
            " grapple with him. Grappling is resolved with grapple checks, which"
            " are opposed contests of Brawl attack bonus plus Strength modifier"
            " plus or minus 4 for every divergance in size catagory, favouring"
            " the larger character. To begin grappling, however, you must hit your"
            " target's DC as per a normal attack. Initiating a grapple provokes an"
            " attack of opportunity from your target.");
        MyTerm->LOption("Great Blow",A_GREA,"You execute a titanically powerful"
            " blow with your weapon, allowing you to double your Strength modifier"
            " to damage, granting a +2 bonus to both the attack roll and damage and"
            " lowering the threat range of your attack"
            " by 3 (i.e., 19-20 becomes 16-20). However, a great blow takes 1 1/2 times"
            " as long as a normal attack, costs two Fatigue points and leaves you"
            " off-balance and Exposed for a full round after you finish it.");
        MyTerm->LOption("Sprint",A_SPRT, "You put on a desperate burst of speed,"
            " and increase you movement rate by 50% for [1 + Constitution modifier]"
            " rounds (minimum 1) at the cost of one fatigue point. Since you are"
            " running full out, Sprint also decreases your defense class by two.");
        MyTerm->LOption("Sunder Weapon",A_SUND,"You strike an opponent's weapon"
            " (or, at your option, any unattended object) and attempt to damage or"
            " destroy it. This provokes an attack of opportunity from the person"
            " using the weapon. Weapons with a magical plus can not be destroyed" 
            " by any weapon with a lower magical plus then their own.");
        MyTerm->LOption("Throw",A_THRO,"You throw an enemy you are grappling"
            " with already one or more squares away from you. You must succeed"
            " in a grapple check against them, and this action ends the grapple."
            " You inflict your normal unarmed damage value in subdual damage, plus"
            " one point per square the enemy is thrown.");
        MyTerm->LOption("Trip",A_TRIP,"You attempt to knock an opponent prone."
            " You must first make a successful attack roll against his DC, and"
            " then you make a Strength check opposed by his Strength or Dexterity,"
            " whichever favors him more. Success knocks the target prone. Failure"
            " allows the target to make an immediate, free attempt to trip you in"
            " return. Some weapons grant a bonus to trip attempts, such as the"
            " quarterstaff. If you do not have the Improved Trip feat, you provoke"
            " an attack of opportunity when you attempt to trip a foe.");
        if (HasSkill(SK_FIND_WEAKNESS))
          MyTerm->LOption("Precision Strike", A_PREC, "Facing an opponent with near-"
              "insurmountable defenses, you cleverly gauge his defensive pattern and"
              " search for an opening or weakness. You add 1/2 your Find Weakness "
              " skill rating to the attack roll, but you also add 45 segments to the "
              " attack's timecost.");

        if (HasFeat(FT_SPRING_ATTACK))
          MyTerm->LOption("Spring Attack",A_SPRI, "You are able to execute a rapid"
              " melee attack against an opponent that is up to two squares away from"
              " you by darting in quickly to strike. This does not provoke an attack"
              " of opportunity the way leaving a hostile creature's threatened area"
              " normally would.");
        if (HasFeat(FT_WHIRLWIND_ATTACK))
          MyTerm->LOption("Whirlwind Attack", A_WHIR, "You execute a rapid flurry"
              " of attacks, striking every creature within one square of you at the"
              " same time. A seperate attack is rolled against each creature at your"
              " full normal attack bonus and damage rating. Performing a Whirlwind"
              " Attack costs you one Fatigue point.");
          MyTerm->LOption("Avert Eyes", A_AEYE, 
              "You avert your eyes from all opponents and attempt instead to look at their faces, shadows, torsos, and so on. This gives all gaze attacks against you a 50~ miss chance. However, it also gives you a 50~ miss chance in combat (25~ if you have the Blind Fighting feat). ");
          MyTerm->LOption("Close Eyes", BLINDNESS, 
              "You close your eyes. This makes it difficult to see the dungeon around you (unless you have Stonework Sense, for example) and makes it difficult to see monsters around you (unless you have Blindsight or Telepathy or are already Tracking them, for example). It makes you immune to gaze attacks and blinding effects. It gives you a 50~ miss chance in combat (25~ if you have the Blind Fighting feat). You may not make attacks of opportunity. Unless you have the Blind Fighting feat, every attack against you is a surprise."); 
          MyTerm->LOption("Movement", A_MOVE,
              "Move into a square adjacent to your own. This command can be used to move into the "
              "same square as a monster instead of attacking it as the arrow keys would. You can "
              "only pass into the square of an incorporeal or non-hostile creature.");
        /* Feint */
        /* Read Opponent */

        MyTerm->SetQKeyType(QuickKeys,QKY_TACTIC);
        n = MyTerm->LMenu(MENU_ESC|MENU_DESC|MENU_2COLS|MENU_BORDER|MENU_QKEY,
            " -- Tactical Options -- ",WIN_MENUBOX);
        if (n == -1)
          break; 
        DoTacticalOption:
        e.Clear(); 
        switch(n) {
          case A_MOVE:
            if (!MyTerm->EffectPrompt(e,Q_DIR,false,"Movement Direction"))
              break;
            ThrowDir(EV_MOVE,e.EDir,this);
            break;
          case A_TRIP:
          case A_THRO:
          case A_SUND:
          case A_GREA:
          case A_GRAB:
          case A_BULL:
          case A_DISA:
          case A_COUP:
          case A_SWNG:
          case A_PREC:
            if (HasStati(TELEKINETIC)) {
              e.vRange = HighStatiMag(TELEKINETIC);
              if (!MyTerm->EffectPrompt(e,Q_TAR|Q_CRE))
                break;
              goto FindNonReachVictim;
              }
            else if ((StateFlags & MS_HAS_REACH) && n != A_THRO &&
                n != A_GRAB && n != A_BULL) {
              /* ww: if you have a reach weapon, you should be
                * able to great-blow something far away */
              e.vRange = 2 + FaceRadius[Attr[A_SIZ]];
              if (!MyTerm->EffectPrompt(e,Q_TAR|Q_LOC|Q_CRE))
                break;
              if (e.ETarget && !e.ETarget->isCreature() && !e.ETarget->isFeature() && !e.ETarget->isItem())
                break;
              if (!e.EVictim) 
                e.EVictim = m->FCreatureAt(e.EXVal,e.EYVal);
              if (!e.EVictim) 
                e.EVictim = m->FCreatureAt(x+DirX[e.EDir],y+DirY[e.EDir]);
              if (!e.EVictim) 
                e.EVictim = m->FCreatureAt(x+2*DirX[e.EDir],y+2*DirY[e.EDir]);
              if (!e.ETarget) 
                e.ETarget = m->KnownFeatureAt(e.EXVal,e.EYVal);
              if (!e.ETarget) 
                e.ETarget = m->KnownFeatureAt(x+DirX[e.EDir],y+DirY[e.EDir]);
              if (!e.ETarget) 
                e.ETarget = m->KnownFeatureAt(x+2*DirX[e.EDir],y+2*DirY[e.EDir]);
              if (!e.ETarget) 
                e.ETarget = m->FChestAt(e.EXVal,e.EYVal);
              if (!e.ETarget) 
                e.ETarget = m->FItemAt(e.EXVal,e.EYVal);
              if (!e.ETarget) 
                e.ETarget = m->FChestAt(x+DirX[e.EDir],y+DirY[e.EDir]);
              if (!e.ETarget) 
                e.ETarget = m->FItemAt(x+DirX[e.EDir],y+DirY[e.EDir]);
              if (!e.ETarget) 
                e.ETarget = m->FChestAt(x+2*DirX[e.EDir],y+2*DirY[e.EDir]);
              if (!e.ETarget) 
                e.ETarget = m->FItemAt(x+2*DirX[e.EDir],y+2*DirY[e.EDir]);
            }
            else {
              if (!MyTerm->EffectPrompt(e,Q_DIR|Q_TAR|Q_LOC|Q_CRE))
                break;
              if (e.ETarget && !e.ETarget->isCreature() && !e.ETarget->isFeature() && !e.ETarget->isItem())
                break;
              FindNonReachVictim:
              if (!e.EVictim) 
                e.EVictim = m->FCreatureAt(e.EXVal,e.EYVal);
              if (!e.EVictim) 
                e.EVictim = m->FCreatureAt(x+DirX[e.EDir],y+DirY[e.EDir]);
              if (!e.ETarget) 
                e.ETarget = m->KnownFeatureAt(e.EXVal,e.EYVal);
              if (!e.ETarget) 
                e.ETarget = m->KnownFeatureAt(x+DirX[e.EDir],y+DirY[e.EDir]);
              if (!e.ETarget) 
                e.ETarget = m->FChestAt(e.EXVal,e.EYVal);
              if (!e.ETarget) 
                e.ETarget = m->FItemAt(e.EXVal,e.EYVal);
              if (!e.ETarget) 
                e.ETarget = m->FChestAt(x+DirX[e.EDir],y+DirY[e.EDir]);
              if (!e.ETarget) 
                e.ETarget = m->FItemAt(x+DirX[e.EDir],y+DirY[e.EDir]);
            }

            if (!e.ETarget)
              {
                IPrint("There's nothing there to use a tactic on!");
                break; 
              }
            if (!e.EVictim->isCreature())
              if (n != A_MOVE && n != A_SWNG)
                {
                  IPrint("That action only applies to creatures.");
                  break;
                }
            if (e.EVictim == this)
            {
              IPrint("Attacking oneself is counterproductive.");
              break;
            }
            if (dist(x,y,e.EVictim->x,e.EVictim->y) <= 1+FaceRadius[Attr[A_SIZ]])
              if ((StateFlags & MS_REACH_ONLY) && e.ETarget->isCreature())
              {
                IPrint("You cannot attack adjacent targets with that weapon.");
                break;
              }
            targ = e.EVictim; 
            if (n == A_SWNG) goto NormalAttack;
                /* ww: Dwarven Auto-Focus */
                if (Opt(OPT_DWARVEN_AUTOFOCUS) && 
                e.ETarget->isCreature() && 
                    ((Creature *)targ)->ChallengeRating() >= 
                                        ChallengeRating() && 
                    HasAbility(CA_DWARVEN_FOCUS) && 
                    !GetStati(DWARVEN_FOCUS,-1,0)) {
                  GainPermStati(DWARVEN_FOCUS,targ,SS_RACE,0,0,0);
                  targ->Flags |= F_HILIGHT;
                  msg = Format("You swear a vow to slay %c%s%c.",
                      -RED, (const char*)targ->Name(NA_THE), -GREY);
                  MyTerm->Message(msg);
                  MyTerm->Box(msg);
                } 
            ThrowVal(EV_SATTACK,n,this,e.EVictim);
            break;
          case AUTO_CHARGE:
            if (HasStati(AUTO_CHARGE))
            {
              IPrint("You're already automatically charging!");
              break;
            }
            GainPermStati(AUTO_CHARGE,NULL,SS_MISC,0,0,0);
            break;
          case BLINDNESS: 
            if (isBlind())
            {
              IPrint("You're already not seeing anything!");
              break; 
            } 
            IPrint("You close your eyes."); 
            GainPermStati(BLINDNESS,this,SS_MISC,BLIND_EYES_CLOSED,0,0);
            break;
          case A_CHAR:
            if (HasStati(CHARGING))
              {
                IPrint("You're already charging!");
                break;
              }
            if (!MyTerm->EffectPrompt(e,Q_DIR))
              break;
            DigMode = false;
            if (!canChargeInDir(e.EDir))
              {
                IPrint("There's nothing to charge at in that direction!");
                break;
              }
            GainPermStati(CHARGING,NULL,SS_MISC,e.EDir,0,0);
            break;
          case A_SPRI:
            if (!MyTerm->EffectPrompt(e,Q_TAR|Q_CRE))
              break;
            if (dist(x,y,e.ETarget->x,e.ETarget->y) > 4)
            {
              IPrint("The <Obj> is too far away to Spring Attack.",e.ETarget);
              break;
            }
            ThrowVal(EV_SATTACK,n,this,e.ETarget);
            return;
          case A_WHIR:
          case A_ESCA:
          case A_SPRT:
          case A_AEYE: 
          case A_DEFN:
            ThrowVal(EV_SATTACK,n,this);
            return;
          case A_CALL:
            IPrint("Not implemented yet.");
            return;
          default:
            Error("Strange tactical option!");
        }  
        return;    
      case KY_CMD_USE:
        UseMenu();
        break;
      case KY_CMD_CANCEL_MENU:
        CancelMenu();
        break;
      case KY_CMD_JUMP:
        e.Clear();
        e.EActor = this;
        if (!MyTerm->EffectPrompt(e,Q_LOC))
          break;
        ReThrow(EV_JUMP,e);
        break;
      case KY_CMD_KICK:
        e.Clear();
        e.vRange = 1 + FaceRadius[Attr[A_SIZ]];
        /* Do not change from default == Q_DIR */
        if (!MyTerm->EffectPrompt(e, Q_DIR|Q_LOC))
          break;
        if (e.isDir)
          {
            e.x = x + DirX[e.EDir];
            e.y = y + DirY[e.EDir];
            e.isDir = false;
            if (e.x < 0 || e.y < 0 || e.x > m->SizeX() || e.y > m->SizeY())
              break;
          }
        
        targ = m->FCreatureAt(e.x,e.y);
        if (targ == this) {
          IPrint("No need to kick yourself (it's not that bad)!");
          break; 
        }
        if (!targ)
          targ = m->FDoorAt(e.x,e.y);
        if (!targ)
          targ = m->KnownFeatureAt(e.x,e.y);
        if (!targ) { 
          IPrint("There's nothing there to kick!"); 
          break; 
        }
          ThrowVal(EV_SATTACK,A_KICK,this,targ);
          break;
      case KY_CMD_OPTIONS:
          MyTerm->OptionManager();
          break; 
      /*
      case KY_CMD_WAND_IDENT:
          GainPermStati(ACTING,NULL,SS_MISC,EV_ZAP);
          Timeout++; 
          break; 
      case KY_CMD_CAST_DETECT_SPELLS:
          GainPermStati(ACTING,NULL,SS_MISC,EV_CAST);
          Timeout++; 
          break; 
      */
      case KY_CMD_REST:
          /*if (Opt(OPT_REST_BEHAVE)) 
            GainPermStati(ACTING,NULL,SS_MISC,EV_REST);
          else*/
            Timeout += 15;
          return;
      case KY_CMD_RUN:
          e.Clear();
          if (!MyTerm->EffectPrompt(e,Q_DIR,false,"Run"))
            break;
          RunDir(e.EDir);
          break;
      case KY_CMD_EXCHANGE:
          Exchange();
          return;
      case KY_CMD_DUMP_CONTAINER:
          MyTerm->DumpContainers();
          break; 
      case KY_CMD_SLEEP:
          if (Opt(OPT_SAFEREST))
            ThrowVal(EV_REST,REST_DUNGEON|REST_SAFE|REST_INSTANT,this);
          else
            ThrowVal(EV_REST,REST_DUNGEON,this);
          break;
      default:
          if (isdigit(ch)) {
            switch (QuickKeys[ch-'0'].Type)
              {
                case QKY_SKILL: 
                  UseMenu(QuickKeys[ch-'0'].Value); 
                  break;
                case QKY_VERB:
                  YuseMenu(QuickKeys[ch-'0'].Value);
                  break;
                case QKY_SPELL:
                  sp = QuickKeys[ch-'0'].Value;
                  goto CastSpell;
                  break;
                case QKY_TACTIC:
                  n = QuickKeys[ch-'0'].Value;
                  goto DoTacticalOption;
              }
            break;
          }
          if (ch >= KY_CMD_MACRO1 && ch <= KY_CMD_MACRO12)
            {
              if (!Macros[ch - KY_CMD_MACRO1])
                break;
              RES(Macros[ch - KY_CMD_MACRO1])->PEvent(EV_INITIALIZE,
                this,Macros[ch - KY_CMD_MACRO1],0);
              GainPermStati(ACTING,NULL,SS_MISC,EV_MACRO,0,Macros[ch - KY_CMD_MACRO1]);
              Timeout++; 
              break;
            }
          if (ch >= KY_CMD_FIRST_ARROW && ch <= KY_CMD_LAST_ARROW) {
            if (HasStati(ENGULFED))
              {
                targ = GetStatiObj(ENGULFED);
                goto NormalAttack;
              }               
            if (MyTerm->ShiftDown()) {
              if (Opt(OPT_SHIFTARROW) == 0)
                      goto ShiftDirFire;
              else {
                      RunDir(DIR_OF_KY_CMD(ch));
                      break;
              }
            }
            dx = DirX[DIR_OF_KY_CMD(ch)];
            dy = DirY[DIR_OF_KY_CMD(ch)];
            if (cdir != -1 && !isSimilarDir(DIR_OF_KY_CMD(ch),cdir) && (cdir != CENTER || 
                        !m->FCreatureAt(x+dx,y+dy)))
            {
              if (cdir != CENTER) {
                if (HasStati(AUTO_CHARGE)) {
                  // automatically break off and charge in the new
                  // direction 
                  RemoveStati(CHARGING);
                  if (canChargeInDir((Dir)(DIR_OF_KY_CMD(ch)))) {
                    if (cdir == -1)
                      IPrint("You charge forward!");
                    else
                      IPrint("You swerve around and charge forward!");
                    GainPermStati(CHARGING,NULL,SS_MISC,
                        (Dir)(DIR_OF_KY_CMD(ch)),0,0);
                    goto ChargeMoveOk;
                    } 
                } else if (!yn("Break off your charge?"))
                  continue;
              } 
              IDPrint(NULL,"The <Obj1> breaks off <his:Obj1> charge.",this);
              RemoveStati(CHARGING);
            }
            if (HasStati(AUTO_CHARGE) && !HasStati(CHARGING) &&
                  canChargeInDir((Dir)(DIR_OF_KY_CMD(ch)))) {
              GainPermStati(CHARGING,NULL,SS_MISC,
                  (Dir)(DIR_OF_KY_CMD(ch)),0,0);
            } 

            ChargeMoveOk:
            if (!m->InBounds(x+dx*2,y+dy*2))
              break;
            if (m->SolidAt(x+dx,y+dy)&& DigMode)
            {
              ThrowXY(EV_DIG,x+dx,y+dy,this);
              return;
            }
            k = ((StateFlags & MS_HAS_REACH) ? 2 : 1) + FaceRadius[Attr[A_SIZ]];
            if (k == 2 && !(StateFlags & MS_REACH_ONLY) && !m->FCreatureAt(x+dx*k,y+dy*k))
              k = 1;
            if (m->InBounds(x+dx*k,y+dy*k) && (targ = m->FCreatureAt(x+dx*k,y+dy*k)))
              if (Percieves(targ) && !((Creature*)targ)->isFriendlyTo(this) && !MyTerm->CtrlDown()) {
NormalAttack:
                if (targ->isCreature() && !((Creature*)targ)->isHostileToPartyOf(this))
                  if (!yn(XPrint("Confirm attack the <Obj>?",targ),true))
                    break;
                /* ww: Dwarven Auto-Focus */
                if (targ->isCreature() && Opt(OPT_DWARVEN_AUTOFOCUS) && 
                    ((Creature *)targ)->ChallengeRating() >= 
                                        ChallengeRating() && 
                    HasAbility(CA_DWARVEN_FOCUS) && 
                    !GetStati(DWARVEN_FOCUS,-1,0)) {
                  GainPermStati(DWARVEN_FOCUS,targ,SS_RACE,0,0,0);
                  targ->Flags |= F_HILIGHT;
                  msg = Format("You swear a vow to slay %c%s%c.",
                      -RED, (const char*)targ->Name(NA_THE), -GREY);
                  MyTerm->Message(msg);
                  MyTerm->Box(msg);
                } 
                bool conf;
                switch(AttackMode()) {
                  case S_BRAWL:
                    if (Opt(OPT_KICK))
                      ThrowVal(EV_SATTACK,A_KICK,this,targ);
                    else
                      Throw(EV_NATTACK,this,targ);
                    return;
                  case S_DUAL:
                  case S_MELEE:
                    if (EInSlot(SL_WEAPON)->Type != T_WEAPON &&
                        stricmp(NAME(EInSlot(SL_WEAPON)->iID),"wooden rod")) {
                      conf = yn(XPrint("Confirm bludgeon with <Obj>?",EInSlot(SL_WEAPON)),false);
                      if (conf == false)
                        return;
                      }
                    Throw(EV_WATTACK,this,targ);
                    return;
                  case S_THROWN: case S_ARCHERY:
                    conf = yn(XPrint("Confirm bludgeon with <Obj>?",EInSlot(SL_WEAPON)),false);
                    if (conf == false)
                      return;
                    Throw(EV_WATTACK,this,targ);
                    return;
                  default:
                    return;
                }
              }
            if(ThrowDir(EV_MOVE,(Dir)(DIR_OF_KY_CMD(ch)),this)!=ABORT)
              return;
          }
          break;
      }

    }
  while(!Timeout);
}

void Player::UseItemMenu(const char *which, const char *haveno, int16 itype)
{
  EventInfo e; String str, desc; Item *it;
  int16 ev,i,n,c; Item *iList[64];
  rID eList[64], qID; 
  Thing *th;

  e.Clear();
  e.isItem = true;
  e.EActor = this;
  c=0;  
  // ww: so far this only allows the *first* item on a square ...
  for (th = m->FirstAt(x,y) ; th ; th = m->NextAt(x,y))
    if (th->isItem()) {
      it = (Item *)th;
      if((isMType(MA_DEMON) || isMType(MA_LYCANTHROPE) ||
            isMType(MA_DEVIL) || isMType(MA_UNDEAD))) {
        if (it->Material() == MAT_SILVER ||
            it->isBlessed())
          continue;
      }
      if(onPlane() != PHASE_MATERIAL) 
        if (!it->isGhostTouch())
          continue; 

      if (it->isType(itype) || 
          (itype == -1 && it->isActivatable()) ||
          (itype == -2 && it->isEdible(this)) ||
          (it->isType(T_FOUNTAIN) && itype == T_POTION) ||
          (it->isType(T_STAFF) && itype == T_WAND)) {

        desc = Format("(ground) %s",(const char*)it->Name(NA_LONG));
        MyTerm->LOption(desc,c,NULL,it->Named.GetLength() ? 1 : 997);
        iList[c] = it; eList[c] = it->eID; c++;
        /* ww: otherwise you couldn't eat something on the
         * ground unless you also had something in the pack
         * *AND* the thing on the ground would be added as
         * a menu option to the next menu that came up! 
         */ 
      }
    }
    
  RestartInvScan:
  for (it = FirstInv() ; it ; it = NextInv())
    if ((itype > 0 && it->isType(itype)) ||
        (itype == -1 && it->isActivatable()) ||
        (itype == -2 && it->isEdible(this)) ||
        (it->Type == T_TOME && itype == T_SCROLL))
    {
      for (i=0;i!=c;i++)
        if (iList[i] == it)
          goto AlreadyDone;
      if (itype == -1)
        if ( !(it->IFlags & IF_WORN) &&
            (it->isType(T_CLOAK) ||
             it->isType(T_BOOTS) ||
             it->isType(T_AMULET) ||
             it->isType(T_RING) ||
             it->isType(T_ROD) ||
             it->isType(T_BRACERS) ||
             it->isType(T_HELMET)) )
          continue;
      if (it->GetParent() == this)
        MyTerm->LOption(it->Name(NA_LONG),c,NULL,it->Named.GetLength()?1:996);
      else {
        desc = Format("(in pack) %s",(const char*)it->Name(NA_LONG));
        MyTerm->LOption(desc,c,NULL,it->Named.GetLength()?1:998);
      }
      iList[c] = it; eList[c] = it->eID; c++;
      goto RestartInvScan;
      AlreadyDone:;
    }
    
  /* Non-Item Activations */
  if (itype == -1)
    StatiIterNature(this,ACTIVATION)
      MyTerm->LOption(NAME(S->eID),c);
      iList[c] = NULL; eList[c] = S->eID; c++;
    StatiIterEnd(this)
      

  for(n=0;n!=SL_LAST;n++)
    if (n == SL_ARMOR || n == SL_READY)
      if (it = InSlot(n))
        if (it->isType(T_ARMOR) || it->isType(T_SHIELD))
        {
          for (i=0;i!=8;i++)
            if (((QItem*)it)->Qualities[i])
            {
              String str; rID eID;
              str = "quality::";
              if (LookupOnly(APreQualNames,((QItem*)it)->Qualities[i]))
                str += Lookup(APreQualNames,((QItem*)it)->Qualities[i]);
              else
                str += Lookup(APostQualNames,((QItem*)it)->Qualities[i]);
              qID = FIND(str);
              if (!qID)
                continue;
              if (TEFF(qID)->HasFlag(EF_ACTIVATE) ||
                  TEFF(qID)->HasFlag(EF_ACTIVATE2) ||
                  TEFF(qID)->HasFlag(EF_ACTIVATE3) ||
                  TEFF(qID)->HasFlag(EF_ACTIVATE4) ||
                  TEFF(qID)->HasFlag(EF_ACTIVATE5) )
              {
                str = it->Name(NA_LONG);
                str += " (";
                if (LookupOnly(APreQualNames,((QItem*)it)->Qualities[i]))
                  str += Lookup(APreQualNames,((QItem*)it)->Qualities[i]);
                else
                  str += Lookup(APostQualNames,((QItem*)it)->Qualities[i]);
                str += ")";
                MyTerm->LOption(str,c,NULL,it->Named.GetLength()?1:996);
                iList[c] = it; eList[c] = qID; c++;
              }
            }
        }


  if (!c)            
  {
    IPrint(haveno);
    return;
  }

  n = MyTerm->LMenu(MENU_SORTED|MENU_ESC|MENU_BORDER,which,WIN_MENUBOX);
  if (n == -1)
    return;
  it = iList[n];
  if (it && it->GetParent() != this && it->GetParent() && 
      it->GetParent()->Type == T_CONTAIN)
    Timeout += TITEM( ((Item *)it->GetParent())->iID)->u.c.Timeout;

  /*
     if (it == m->ItemAt(x,y)) {
     if (it->Weight() < MaxPress() || itype != -2)      
     if(Throw(EV_PICKUP,this,NULL,it)==ABORT)
     return;
     }
   */
   
  e.EItem = it;
  
  
  switch (itype) {
    case T_POTION: ev = EV_DRINK; break;
    case T_WAND:   ev = EV_ZAP; break;
    case T_SCROLL: ev = EV_READ; break;
    case -1:       ev = EV_ACTIVATE; break;
    case -2:
                   if (it->GetQuantity() > 1)
                     it = it->TakeOne();
                   GainPermStati(ACTING,it,SS_MISC,EV_EAT); 
                   Timeout++;
                   return;
  }

  e.eID = eList[n];

  if (ev == EV_ACTIVATE && !it)
    {
      ev = EV_EFFECT;
      e.isActivation = true;
      e.isItem = false;
    }

  if (it && !it->isKnown(KN_MAGIC) && it->Type == T_WAND) 
  {
    if(!MyTerm->EffectPrompt(e,Q_DIR))
      return;
  }
  else if (it && it->Type == T_POTION) 
  {
    /* I guess I put the "& (Q_DIR|Q_INV)" in on the assumption
       that potions can't be used on targets, but doing so badly
       messed up the Remove Curse potion, preventing it from
       working on inherant curses, so don't uncomment this
       without addressing that! */
    if (it->eID && (TEFF(it->eID)->ef.qval /*& (Q_DIR|Q_INV)*/))
      if(!MyTerm->EffectPrompt(e,TEFF(it->eID)->ef.qval /*& (Q_DIR|Q_INV)*/))
        return;
  }
  else 
  {
    if(e.eID && !MyTerm->EffectPrompt(e,TEFF(e.eID)->ef.qval))
      return;
  }                  

  e.EActor = this;
  e.EItem  = it;
  e.eID    = eList[n];
  
  
  ReThrow(ev, e);
}

void Player::YuseMenu(int32 SelectedIndex)
  {
    int16 i, c;
    EventInfo e;

    if (SelectedIndex)
      { c = SelectedIndex;
        goto DoYuse; }

    for (i=0;i!=5;i++)
      if (RecentVerbs[i] != -1)
        MyTerm->LOption(YuseCommands[RecentVerbs[i]].Verb,RecentVerbs[i]);
    
    for (i=0;YuseCommands[i].Event;i++)
      if (i != RecentVerbs[0] && i != RecentVerbs[1] && i != RecentVerbs[2]
            && i != RecentVerbs[4] && i != RecentVerbs[4])
        MyTerm->LOption(YuseCommands[i].Verb,i);

    MyTerm->SetQKeyType(QuickKeys,QKY_VERB);
    c = MyTerm->LMenu(MENU_3COLS|MENU_ESC|MENU_BORDER|MENU_QKEY,
      "What do you want to do?");

    if (c == -1)
      return;
      
    DoYuse:

    e.Clear();
    e.EActor = this;
    e.EMap = m;

    if (YuseCommands[c].Flags & YU_REVERSE) 
      {
        if (!YusePrompt(YuseCommands[c].QItem2,
                        YuseCommands[c].I2Msg,
                        e, 3))
          return;
        if (!YusePrompt(YuseCommands[c].QItem1,
                        YuseCommands[c].I1Msg,
                        e, 2))
          return;
        if (!YusePrompt(YuseCommands[c].QTarget,
                        YuseCommands[c].TMsg,
                        e, 1))
          return;
      }
    else
      {
        if (!YusePrompt(YuseCommands[c].QTarget,
                        YuseCommands[c].TMsg,
                        e, 1))
          return;
        if (!YusePrompt(YuseCommands[c].QItem1,
                        YuseCommands[c].I1Msg,
                        e, 2))
          return;
        if (!YusePrompt(YuseCommands[c].QItem2,
                        YuseCommands[c].I2Msg,
                        e, 3))
          return;
      }
      
    if (YuseCommands[c].Event == EV_SURRENDER ||
        YuseCommands[c].Event == EV_ENLIST ||
        YuseCommands[c].Event == EV_TALK ||
        YuseCommands[c].Event == EV_GREET ||
        YuseCommands[c].Event == EV_ORDER ||
        YuseCommands[c].Event == EV_QUELL ||
        YuseCommands[c].Event == EV_COW ||
        YuseCommands[c].Event == EV_DISMISS ||
        YuseCommands[c].Event == EV_FAST_TALK ||
        YuseCommands[c].Event == EV_DISTRACT)
      if (!e.ETarget || !e.ETarget->isCreature())
        { IPrint("Don't socialize with the furnature.");
          return; }
      
    if (e.ETarget && !e.EItem)
      if (e.ETarget->isItem()) {
        e.EItem = (Item*) e.ETarget;
        e.ETarget = NULL;
        }

    e.isVerb = true;
    ReThrow(YuseCommands[c].Event,e);

    if (RecentVerbs[0] == c)
      return;

    for(i=0;i!=5;i++)
      if (RecentVerbs[i] == c)
        {          
          if (i != 4)
            memmove(&RecentVerbs[i], &RecentVerbs[i+1], (4-i) * sizeof(RecentVerbs[0]));
          RecentVerbs[4] = -1;
        }
    
    memmove(&RecentVerbs[1],&RecentVerbs[0],4 * sizeof(RecentVerbs[0]));
    RecentVerbs[0] = c;  

  }

bool Player::YusePrompt(int16 qval, const char *msg, EventInfo &e, int8 p)
  {
    Thing *t, *t2;

    if (!qval)
      return true;

    t = e.ETarget;
    if (!MyTerm->EffectPrompt(e,qval,false,msg))
      return false;

    t2 = e.ETarget;
    e.ETarget = t;
    e.p[p].t = t2;
    return true;
  }
  

EvReturn Creature::HandleVerb(EventInfo &e)
  {
    switch (e.Event)
      {
        case EV_DIVIDE:
          Item *it;
          if (e.EItem->GetQuantity() == 1)
            {
              IPrint("Singular items cannot be divided.");
              return ABORT;
            }
          it = OneSomeAll(e.EItem,this,true);
          if (it && it != e.EItem)
            {
              it->GainTempStati(DROPPED,NULL,10,SS_MISC,0,0);
              e.EActor->GainItem(it,false);
            }
          return DONE;
      }
    if (e.Event >= POST(0) && e.Event <= POST(300)) {
      IPrint("That verb can't be used that way.");
      return ABORT;  
      }
    return NOTHING;
  }       

EvReturn Player::Event(EventInfo &e)
	{
    switch (e.Event)
      {
        case EV_EFFECT:
        case EV_STRIKE:
          /* EActor or EVictim */
          if (DigMode) {
            DigMode = false;
            MyTerm->ShowTraits();
            }
         break;
        case EV_DEATH:
          if (e.EVictim == this)
            return Death(e);
        case POST(EV_MOVE):
          UpdateMap = true;
         break;
        case EV_VICTORY:
          VictoryFlag = true;
          Flags |= F_DELETE|F_INVIS;
          m->Update(x,y);
          MyTerm->ShowStatus();
          SetSilence();
          if (e.GraveText)
            GraveText = e.GraveText;
          else
            GraveText = "ripe old age";
         break;
        case EV_REST:
          return Rest(e);
	      case EV_GOD_RAISE:
	        return GodRaise(e);
        default:
          ;
      }
		return NOTHING;
	}

void Player::IntuitItem(Item *it)
{
  if ((it->isKnown(KN_MAGIC) &&
      it->isKnown(KN_PLUS)) ||
      it->IFlags & IF_INTUITED)
    return; 
  it->IFlags |= IF_INTUITED;

  // the previous system didn't actually do what it looked like it was
  // doing: legendary items were almost always identified by the fact that
  // no inscription came up for them. Mundane items were almost always
  // identified, so they could be thrown away rapidly. 

  // now there are starting to be too many items in Incursion -- we need
  // Intuition to work on items you are just glancing at ... we'll also
  // change it so that it gives a range of answers, with a better Intuit
  // check yielding a smaller range.
  int precision = random(20)+1+SkillLevel(SK_INTUITION);

  if (precision < 0) precision = 0; 

  const char * levels[] = { 
    "<7>mundane<7>",
    "<3>magical<7>",
    "<5>good<7>",
    "<6>great<7>",
    "<4>excellent<7>",
    "<12>superior<7>",
    "<14>legendary<7>",
    "<15>unearthly<7>"
  };

  /* If you want absolute ability to tell _anything_ about an
     item, you should have at least Intuition +5. */
  if (precision < 5)
    return;

  if (precision >= 15)
    it->MakeKnown(KN_CURSE);

  if (!it->isMagic())
    it->Inscrip = levels[0];
  else 
    it->Inscrip = levels[1 + 
            min( min(it->PItemLevel(this) / 3, (precision / 4)) , 6 ) ];
  // itemlevel ranges from around 0 to 25

  it->Inscrip = XPrint(it->Inscrip);

  if (precision >= 10 && it->eID) {
      int32 sch = TEFF(it->eID)->Schools;
      if (sch & SC_ABJ)
        it->Inscrip += "/abj";
      if (sch & SC_ARC)
        it->Inscrip += "/arc";
      if (sch & SC_DIV)
        it->Inscrip += "/div";
      if (sch & SC_ENC)
        it->Inscrip += "/enc";
      if (sch & SC_EVO)
        it->Inscrip += "/evo";
      if (sch & SC_ILL)
        it->Inscrip += "/ill";
      if (sch & SC_NEC)
        it->Inscrip += "/nec";
      if (sch & SC_THA)
        it->Inscrip += "/tha";
      if (sch & SC_WEA)
        it->Inscrip += "/wea";
      if (it->Inscrip[0] == '/')
        it->Inscrip = it->Inscrip.Right(it->Inscrip.GetLength()-1);
    }

  /* A reward for high Intuition. */
  if (precision >= 25 && !random(3))
    if ((!it->isKnown(KN_MAGIC)) && it->eID)
    {
      String s1, s2;
      s1 = it->Name(0);
      it->MakeKnown(KN_MAGIC);
      s2 = it->Name(NA_A);
      IPrint("In a flash of insight, you feel almost certain that "
             "your <Str> <Str> really <Str>!",
             (const char *)s1,
             it->isPlural() ? "are" : "is",
             (const char *)s2);
    }
    
  

  JournalNoteItem(it);
}

// ww: returns the chance of an encounter
void Character::RestEncounterChance(int16 maxDist, 
    int16 &posseID, Creature * &in_sight, int16 &Chance)
{
  int16 PartyID[64], cParty, bParty;
  int32 PartyXCR[64], bXCR;
  int32 i, j, tx; 
  Thing * t; 
  Creature *c;

  memset(PartyID,0,64*sizeof(int16));
  memset(PartyXCR,0, 64*sizeof(int32));
  Chance = 0; 
  cParty = 0;

  for(tx=0;tx!=m->SizeX();tx++)
    for(int16 ty=0;ty!=m->SizeY();ty++)
      m->At(tx,ty).Visibility = 0;

  m->FloodConnectB(x,y);
  
  maxDist += 15 + m->Depth*2;

  MapIterate(m,t,j)
    if (t->Type == T_MONSTER)
      if ((c = (Creature*)t) != this && c->isHostileTo(this)) {
        if (!m->At(c->x,c->y).Visibility)
          continue;
        else if (c->GetAttr(A_MOV) <= -15)
          continue;
        else if (dist(x,y,c->x,c->y) > maxDist)
          continue; 
        else if (m->At(c->x,c->y).isVault)
          continue; 
        // ww: otherwise, if you're a drow with big infravision, 
        // a monster in another room that is not hiding in shadows
        // will prevent you from sleeping here
        // fjm: This had previously made it impossible to have
        // encounters at all. I think this is what you actually
        // wanted it to do.
        if (Percieves(c) & ~(PER_TELE|PER_DETECT|PER_PERCEPT|PER_TRACK))
          if (m->LineOfSight(x,y,c->x,c->y,this)) {                
            in_sight = c; 
            continue; 
          } 

        /* Later, include checks that monster is sufficient
           level for the dungeon level you are at. */

        // ww: monsters in vaults do not come out to smite you
        // otherwise resting near the Goblin Encampment, say, is a
        // great way to pull Murgash out largely alone!

        for(i=0;i!=64 && PartyID[i];i++)
          if (PartyID[i] == c->PartyID)
            { PartyXCR[i] += XCR(c->ChallengeRating());
              goto Grouped; }
              
        if (i == 64)
          goto Grouped;
          
        PartyID[i] = c->PartyID;
        PartyXCR[i] = XCR(c->ChallengeRating());
        cParty = i + 1;         
        
        Grouped:
        Chance += (250 + c->ChallengeRating() * 10 + c->Mod(A_INT) * 20) /
          dist(x,y,c->x,c->y);
      }

  for(tx=0;tx!=m->SizeX();tx++)
    for(int16 ty=0;ty!=m->SizeY();ty++)
      m->At(tx,ty).Visibility = 0;
    
  if (isPlayer())
    thisp->CalcVision();

  if (!cParty) {
    posseID = 0;
    return;
    }

  bParty = bXCR = 0;
  for (i=0;i!=cParty;i++)
    if (PartyXCR[i] > bXCR)
      { bXCR = PartyXCR[i];
        bParty = PartyID[i]; }
        
  posseID = bParty;

  return; 
} 

EvReturn Player::SpendHours(int16 minHours, int16 maxHours, int16 &hoursSpent, bool isHeal)
  {
    int32 AwakeTime, AllowedTime;
    Creature *in_sight; int16 Chance, pc, PartyID; 
    
    
    if ((!isHeal) && (HasStati(POISONED) || 
           HasStati(STONING)/* || HasStati(DISEASED) */))
      {
        IPrint("You're too busy dying at the moment to spend that kind of time.");
        return ABORT;
      }
    if (GetStatiDur(HUNGER) < STARVING)
      {
        IPrint("You're too hungry at the moment to focus.");
        return ABORT;
      }
    if (HasEffStati(-1,FIND("Strangulation")))
      {
        IPrint("Not while you're being strangled!");
        return ABORT;
      }

    theGame->SetAutoSaveFlag();
  

    in_sight = NULL;
    RestEncounterChance(0,PartyID,in_sight,Chance);
    if(in_sight) {
      IPrint(Format("You can't work while there's "
        "hostiles (e.g., %s) in plain sight!",(const char*)in_sight->Name(0)));
      return ABORT;
      }
      
    /* Later, there should be a chance to have your work interrupted while
       working by an encounter, and there should be monster recovery, and
       basically all the other things that happen in Rest (except recovery
       for the PC, 'cause you don't recover while working). To do this, we
       should really revise and rewrite the resting code, and generalize it
       to work with SpendHours too. For now, though, we'll go easy on the
       player as he works. */
    
    
    AwakeTime = theGame->GetTurn() - LastRest;
    AllowedTime = (12 + Mod(A_CON)) * HOUR_TURNS;
    
    if (AwakeTime + (minHours * HOUR_TURNS) > AllowedTime)
      {
        IPrint("You need to get a full night's rest before you can "
               "focus solidly on work again.");
        return ABORT;
      }
    
    hoursSpent = min(maxHours,(AllowedTime - AwakeTime) / HOUR_TURNS);
    
    theGame->Turn += hoursSpent * HOUR_TURNS;
    if (hoursSpent == 1)
      IPrint("An hour passes.");
    else
      IPrint("<Num> hours pass.", hoursSpent);
    
    return DONE;
  }


EvReturn Player::Rest(EventInfo &e)
  {
    int16 i,j,pc,tx,ty, num,Roll, Chance, Percent; 
    Creature *in_sight = NULL; const char* wakeSafely;
    Creature *c; Thing *t; Item *it; uint32 sch;
    Chance = pc = 0; int16 PartyID;
    wakeSafely = NULL;

    if (!(e.EParam & REST_PLOT)) {

      if (GetStatiObj(DWARVEN_FOCUS))
        {
          IPrint("You shall know no reprieve, so long as the <Obj> lives!",
            GetStatiObj(DWARVEN_FOCUS));
          return ABORT;
        }
        
      if (m->SolidAt(x,y))
        {
          IPrint("Nice try. No.");
          return ABORT;
        }
    
      if (HasStati(POISONED) || HasStati(STONING)/* || HasStati(DISEASED) */)
        {
          IPrint("You're too busy dying at the moment to think about rest.");
          return ABORT;
        }
      if (GetStatiDur(HUNGER) < STARVING)
        {
          IPrint("You're too hungry at the moment to break camp.");
          return ABORT;
        }
      /*
      if (ResistLevel(AD_SLEE) == -1)
        if (!HasAbility(CA_REVERIE))
          {
            // Philter of Wakefulness, et al. 
            IPrint("Strangely, rest eludes you.");
            return ABORT;
          }
    
          */
      }
    
    /* Later: +5% per dungeon level */

    if (e.EParam & REST_DUNGEON) {

      RestEncounterChance(0,PartyID,in_sight,Chance);

      if(in_sight) {
        IPrint(Format("You can't break camp while there's hostiles (e.g., %s) in plain sight!",(const char*)in_sight->Name(0)));
        return ABORT;
      } else if(!MyTerm->yn("Confirm rest in dungeon?"))
        return ABORT;
    }

    if (HasStati(MOUNTED))
      IPrint("You dismount to rest.");
    
    if (InSlot(SL_ARMOR) && !InSlot(SL_ARMOR)->isCursed())
      IPrint("You take off your armor to rest.");

    theGame->SetAutoSaveFlag();

    Roll = random(100)+1;

    {
      Creature *Allies[16], *cr; 
      int16 i, j, sumSpot, oChance;
      j = 0;
      sumSpot = SkillLevel(SK_SPOT);
      MapIterate(m,cr,i)
        if (cr->isCreature() && cr != this)
          if (cr->DistFrom(this) <= 6)
            if (Percieves(cr))
              if (cr->getLeader() == this || 
                  cr->ts.hasTargetOfType(OrderTakeWatches))
                {
                  /* Mindless Undead make lousy watchmen */
                  if (cr->isMType(MA_UNDEAD) &&
                      !cr->isMType(MA_SAPIENT))
                    continue;
                    
                  /* Will it last the night? */
                  if (cr->HasStati(SUMMONED))
                    if (cr->GetStatiDur(SUMMONED) > 0)
                      continue;
                  if (cr->HasStati(TRANSFORMED))
                    if (cr->GetStatiDur(TRANSFORMED) > 0)
                      continue;

                  Allies[j++] = cr;
                  sumSpot += cr->SkillLevel(SK_SPOT);
                }
       if (j)
         {
           if (j > 5)
             IPrint("You and your allies take watches.");
           else {
             String msg;
             msg = "You";
             for (i=0;i!=j;i++)
               {
                 if (i == j - 1)
                   msg += " and ";
                 else
                   msg += ", ";
                 msg += Allies[i]->Name(NA_THE);
               }
             msg += " take watches.";
             IPrint(msg);
             }
           oChance = Chance;
           Chance = max(0,Chance - sumSpot);
           if (Roll < oChance && !(e.EParam & REST_SAFE))
             if (Roll >= Chance)
               IPrint("Your feel you are evaluated, but your " \
                 "vigilance wards away encounters!");
           if (sumSpot > m->Depth * 3)
             wakeSafely = "Your vigilance provides forewarning!";
                 
         }
    
    }

    if (Roll < Chance && !(e.EParam & REST_SAFE))
      Percent = 10+random(81);
    else
      Percent = 100;

    Thing *th;
    int32 ii,di;
    for (di = 1; di< MAX_DUNGEONS && theGame->DungeonID[di] ; di++) {
      rID dID = theGame->DungeonID[di]; 

      ASSERT(dID); 

      int32 maxDepthHere = min(MAX_DUNGEON_LEVELS,
          TDUN(dID)->GetConst(DUN_DEPTH)+1);
      int32 depth;

      for (depth = 0; depth < maxDepthHere; depth++) {
        if (theGame->DungeonLevels[di][depth]) {
          Map * m = oMap(theGame->DungeonLevels[di][depth]);

          MapIterate(m,th,ii) {
            if (th->isCreature()) {
              Creature * t = (Creature *)th;
              bool one_body = (t->HasFeat(FT_ONE_BODY_ONE_SOUL) ||
                  t->Attr[A_CON] == 0);
              int16 a_hp = A_CON;

              t->RemoveStati(DWARVEN_FOCUS);
              t->RemoveStati(TRIED,-1,ANIMAL_COMPANION);

              /* Later, adapt this so that you always sleep over the night
               * -- if you rest at any time between 4:00 AM and 6:00 PM,
               * add eight hours as you pass the rest of the day and wait
               * for night.
               */
              if (!(e.EParam & REST_INSTANT)) {
                if (t == this)  {
                  theGame->Turn += (((HOUR_TURNS*8) / 100) * Percent);
                  theGame->Day++;
                  m->DaysPassed();
                } 

                /* Hunger should be halved, or 4/5ths for elves with
                 * Reverie.  Repopulate dungeon if # monsters lower than
                 * MONSTER_EQUILIBRIUM, adding REPLENISH_MONSTERS monsters
                 * to distant squares at a time.
                 */
                if (e.EParam & REST_FOOD || t != this)
                  t->SetStatiDur(HUNGER,-1,NULL,SATIATED);
                else if (t->HasAbility(CA_SLOW_METABOLISM))
                  t->SetStatiDur(HUNGER,-1,NULL,(t->GetStatiDur(HUNGER)*85)/100);
                else if (t->HasAbility(CA_REVERIE))
                  t->SetStatiDur(HUNGER,-1,NULL,(t->GetStatiDur(HUNGER)*75)/100);
                else
                  t->SetStatiDur(HUNGER,-1,NULL,(t->GetStatiDur(HUNGER)*65)/100);

                t->RemoveStati(LFA_COUNT);
                
                if (t->isCharacter())
                  for (i=0;i!=7;i++)
                    t->Abuse(i,Dice::Roll(1,4));
                
              }

              if (!(e.EParam & REST_NOHEAL) || t != this) {

                j = t->HasAbility(CA_STRONG_SOUL) ? 2 : 1;
                if (t->HasStati(DISEASED))
                  j--;
                if (!j)
                  goto SkipHeal;
                for(i=0;i!=7;i++)
                  if (t->HasStati(ADJUST_DMG,i))
                  {
                    if ((-t->SumStatiMag(ADJUST_DMG,i)) <= j)
                      t->RemoveStati(ADJUST_DMG,-1,i);
                    else
                      t->SetStatiMag(ADJUST_DMG,i,NULL,
                          t->GetStatiMag(ADJUST_DMG,i) + j);
                  }
                if (j == 2 && Percent == 100)      
                  t->RestoreXP(100 + 100*t->AbilityLevel(CA_STRONG_SOUL));      

                if (one_body && t->Mod(A_WIS) > t->Mod(A_CON))
                  a_hp = A_WIS; 

                if (!(t->isMType(MA_UNDEAD) || t->isMType(MA_CONSTRUCT)))
                  {
                    t->cHP = min(t->mHP+t->Attr[A_THP],t->cHP +
                      ((t->ChallengeRating()+3) * t->Attr[a_hp] * Percent) /
                      800);
                    if (t->cHP < t->mHP + t->Attr[A_THP] && t->HasSkill(SK_HEAL,true))
                      {
                        Creature *healer;
                        healer = t->MostSkilledAlly(SK_HEAL);
                        if (t == healer)
                          t->IPrint("You tend your wounds with your skill as a healer.");
                        else
                          t->IPrint("The <Obj1> tends your wounds with <his:Obj1> skill as a healer.",healer);
                        t->cHP = min(t->mHP+t->Attr[A_THP],t->cHP +
                          ((healer->SkillLevel(SK_HEAL)) * t->Attr[a_hp] * Percent) /
                              500);
                      }
                  }
                SkipHeal:
                if (t->HasStati(FLAWLESS_DODGE))
                  t->SetStatiMag(FLAWLESS_DODGE,-1,NULL,0);
                  
                t->DiseasePulse(true, true);
                  
                }

              if (!(e.EParam & REST_NOMANA) || t != this) {
                t->uMana = (t->uMana*(100-Percent))/100;
                t->hMana = (t->hMana*(100-Percent))/100;
              }

              #if 0
              // ww: heal every other creature in the dungeon (notably including your
              // mounts and animal companions) fully! Previously you could win about
              // any fight through attrition because the other monsters did not heal
              // when you went to rest. 
              // fjm: yeah, and resting makes more monsters. Only heal the creatures that
              // have means to heal themselves. You *should* be able to win some fights
              // (e.g., against unintellient beasts) by attrition! Besides, I REALLY
              // want to avoid the arbitrary kind of "this makes no sense in story terms
              // but let's do it for gameplay" kind of constructs like this. It feels
              // tremendously artificial.
              if (e.EParam & REST_FULL || t != this) {
                t->uMana = t->hMana = 0;
                t->cHP = t->mHP+Attr[A_THP];
                t->cFP = t->Attr[A_FAT];
                RemoveStati(ADJUST_DMG);
              }
              #endif

              if (e.EParam & REST_RESTLESS && t != this)
                ;
              else if (t->cFP >= 0)              /* Vigorous */
                t->cFP = t->Attr[A_FAT];
              else if (t->cFP >= -t->Attr[A_FAT]) { /* Fatigued */
                if (Percent == 100)
                  t->cFP = t->Attr[A_FAT];
                else
                  // ww: the problem with this approach is that you are
                  // taking CON damage from the fatigue! my 16-CON orc
                  // warrior who got down to -10 fatigue after taking a
                  // bunch of crits from a vrock still only got 1 fatigue
                  // point back per rest, and resting 10 times is *way*
                  // boring. New theory: a full night's sleep always gets
                  // you back to at least 1 point of fatigue. 
                  t->cFP = (min(t->Attr[A_FAT],t->cFP+max(1,t->Mod(a_hp))));
              }
              else {                          /* Exhausted */
                if (Percent == 100)
                  t->cFP =
                    max(min(t->Attr[A_FAT],t->cFP+max(1,t->Mod(a_hp))),1);
                else
                  t->cFP += 1;
                }
              if (t->InSlot(SL_ARMOR) && t->InSlot(SL_ARMOR)->isCursed())
                {
                  t->IPrint("Resting in your cursed armor prevents decent sleep.");
                  t->cFP = min(t->cFP,-1);
                }

              // ww: just before you wake up they all cast Stoneskin ...
              if (t->isMonster()) {
                ((Monster *)t)->PreBuff(); 
              }
              if (t->isMonster())
                for(it=t->FirstInv();it;it=t->NextInv())
                  it->IFlags |= IF_PROPERTY;
              

            } else if (th->isItem()) {
              if (th->isType(T_CORPSE) && ((Corpse *)th)->fullyDecayed()) {
                th->Remove(true);
                ii--;
              }
            } else if (th->Type == T_TRAP) {
              ((Trap *)th)->TrapFlags &= (~TS_NODISARM);
              if (random(10) < m->Depth) {
                ((Trap *)th)->TrapFlags &= (~TS_DISARMED);
                th->RemoveStati(RETRY_BONUS);
                th->SetImage();
              }
            } 
          } 

          if (!(e.EParam & REST_NOIDENT))
            for(it = FirstInv(); it ; it = NextInv())
              IntuitItem(it); 

          for(it = FirstInv(); it ; it = NextInv()) {
            if (it->eID && (TEFF(it->eID)->HasFlag(EF_3PERDAY) ||
                  TEFF(it->eID)->HasFlag(EF_7PERDAY)  ||
                  TEFF(it->eID)->HasFlag(EF_1PERDAY)) ) {
              // ww: more importantly, however, reset the charges! otherwise
              // it's 3-per-life, not 3-per-day!
              it->SetCharges ( 0 );
              if (it->eID && it->isKnown(KN_MAGIC))
                it->MakeKnown(KN_PLUS2);
            }
          }
        } 
      } 
    } 

    if (isDead())
      return DONE;  

    if ((Roll < Chance) && !(e.EParam & REST_SAFE))
      {
        rID xID;
        IPrint("An encounter!");
        RemoveStati(DEFENSIVE);
        if (HasStati(MOUNTED))
          ThrowVal(EV_DISMOUNT,DSM_SILENT,this);
        if (InSlot(SL_ARMOR) && !InSlot(SL_ARMOR)->isCursed())
          {
            Item *ar = InSlot(SL_ARMOR);
            ar->Remove(false);
            ar->PlaceAt(m,x,y);
          }
        #if 0
        num = 2 + (Chance-Roll)/10;
        PartyID = MAX_PLAYERS + 10 + random(200);
        while(num)
          {
            i = random(pc);
            if (posse[i])
              {
                posse[i]->PlaceNear(x,y);
                posse[i]->PartyID = PartyID;
                posse[i] = NULL;
              }
            else {
              xID = theGame->GetMonID(PUR_DUNGEON,m->Depth-3,m->Depth+3,m->Depth,0);
              c = new Monster(xID);
              if (c->isMType(MA_DRAGON)) {
                xID = theGame->GetTempID(TM_AGECAT,c->mID,m->Depth+2);
                if (!xID)
                  xID = FIND("young adult");
                c->AddTemplate(xID);
                }
              c->PartyID = PartyID;
              c->PlaceAt(m,1,1);
              c->PlaceNear(x,y);
              c->Initialize();
              }

            num--;
          }
        #endif
        i = random(pc);
        if (PartyID)
          {
            MapIterate(m,c,j)
              if (c->isCreature())
                if (c->PartyID == PartyID)
                  c->PlaceNear(x,y);
          }
        
        if (AbilityLevel(CA_UNCANNY_DODGE) >= 3 && !wakeSafely)
          wakeSafely = "Your uncanny instincts provide forewarning!";
        else if (HasFeat(FT_LIGHT_SLEEPER) && !wakeSafely)
          wakeSafely = "As danger looms, you wake alert and ready!"; 
        
        if (wakeSafely)
          {
            IPrint(wakeSafely);
            FFCount = 0;
          }
        else
          {
            SetSilence();
            GainTempStati(SLEEPING,NULL,20,SS_ATTK,SLEEP_NATURAL);
            UnsetSilence();
            Timeout = /*HasFeat(FT_LIGHT_SLEEPER) ? 30 :*/ 60;
          }
        return DONE;
      }
    CalcValues();
    statiChanged = true;
    LastRest = theGame->GetTurn();
    
    if (!(e.EParam & REST_NOMSG))
      IPrint("You awaken feeling well rested and recovered.");
    
    if (InSlot(SL_ARMOR) && !InSlot(SL_ARMOR)->isCursed())
      IPrint("You put your armor back on.");
    if (HasStati(MOUNTED))
      IPrint("You get back on your mount.");
    
    return DONE;
  }


void Map::DaysPassed()
  {
    int16 i, c, j, k, x, y, x2, y2, n, pc, MonCount, Tries, MonEquil, numRegen; 
    Thing *t; bool removed; Item *it; Player *p[MAX_PLAYERS]; Creature *cr;
    static Thing *list[2048];
    /* This function updates the map when the player rests, adding new
       monsters and such. */
    int8 DepthCR = TDUN(dID)->GetConst(INITIAL_CR) + 
                   (Depth*TDUN(dID)->GetConst(DUN_SPEED))/100 - 1; 

    if (Day == theGame->Day)
      return;

    inDaysPassed = true;

    MonCount = 0; pc = 0;
  
    MapIterate(this,t,c) 
      list[c] = t;
      
    for (j=0;j!=c;j++)
      {
      t = list[j];
      Restart1:
      StatiIter(t)
          if (S->Nature == HUNGER) continue; 
          if (S->Duration > 0 || S->Duration == -2) {
            StatiIter_RemoveCurrent(t);
            StatiIterBreakout(t,goto Restart1);
            }
          else if (S->Duration < -2)
            S->Duration++;
          if (t->isDead()) 
            StatiIterBreakout(t,goto Done1); 
      StatiIterEnd(t)
      Done1:;
      
      Item *it;
      if (t->isCreature()) {
        for (it=((Creature*)t)->FirstInv();it;
                    it=((Creature*)t)->NextInv())
          {
            Restart2:
            StatiIter(it)
              if (S->Duration > 0 || S->Duration == -2) {
                StatiIter_RemoveCurrent(it);
                StatiIterBreakout(it,goto Restart2);
                }
              else if (S->Duration < -2)
                S->Duration++;
              if (t->isDead())
                StatiIterBreakout(it,goto Done2) 
            StatiIterEnd(it)
            Done2:;
          }
        }
      
      if (t->isDead()) 
        continue; 

      if (t->isType(T_DOOR))
        ((Door*)t)->DoorFlags &= ~DF_SEARCHED;
      if (t->isMonster())
        if (!((Monster*)t)->HasMFlag(M_BREEDER))
          if (((Monster*)t)->ChallengeRating() + 4 >= Depth)
            MonCount++;
      if (t->isPlayer())
        p[pc++] = (Player*) t;
      }

    RestartFields:
    for(i=0;Fields[i];i++)
      if (Fields[i]->Dur >= 1 || Fields[i]->Dur >= -2)
        {
          RemoveField(Fields[i]);
          goto RestartFields;
        }

    RestartTerra:
    for(i=0;i!=TerraList.Total();i++) {
      if (TerraList[i]->Duration >= 1 || TerraList[i]->Duration == -2)
        { RemoveTerra(TerraList[i]->key); goto RestartTerra; }
      }

    /* There really should be a TDUN flag for this, so we don't
       regen monsters in a city map, but for now this should be
       sufficient as a kludge for Rope Trick; we'll do this in
       more depth when we do cities and wilderness for RotF. */
    if (SizeX() < 30 && SizeY() < 30)
      goto SkipMonUpdate;

    if (dID) {
      
      MonEquil = TDUN(dID)->GetConst(MONSTER_EQUILIBRIUM_BASE) +
                   Depth * TDUN(dID)->GetConst(MONSTER_EQUILIBRIUM_INC);

      numRegen = max(3,DepthCR);

      for (i=Day*6;i!=theGame->Day*6;i++)
        if (MonEquil > MonCount)
          {
            Tries = 0;
            do {
              Tries++;
              x = 2 + random(sizeX-4);
              y = 2 + random(sizeY-4);
              if (Tries == 100)
                goto SkipMonUpdate;
              if (SolidAt(x,y))
                continue;
              if (At(x,y).isVault)
                continue;
              if (TTER(TerrainAt(x,y))->HasFlag(TF_FALL))
                continue;
              if (TTER(TerrainAt(x,y))->HasFlag(TF_WARN))
                continue;
              if (TTER(TerrainAt(x,y))->HasFlag(TF_SPECIAL))
                continue;
              for(k=0;k!=pc;k++)
                if (dist(x,y,p[k]->x,p[k]->y) <= 25)
                  continue;
              break;
              }
            while(1);
            if (TTER(TerrainAt(x,y))->HasFlag(TF_WATER))
              GenEncounter(EN_DUNGEON,DepthCR,DepthCR,MA_AQUATIC,0,dID,x,y);
            else {
              n = GenEncounter(EN_DUNGEON|EN_NOPLACE,DepthCR,DepthCR,0,0,dID,x,y);
              for (j=0;j!=n;j++) {
                Tries = 0;
                do {
                  Tries++;
                  x2 = x - 5 + random(11);
                  y2 = y - 5 + random(11);
                  if (Tries == 100)
                    goto SkipMonUpdate;
                  if (!InBounds(x2,y2))
                    continue;
                  if (SolidAt(x2,y2))
                    continue;
                  if (At(x2,y2).isVault)
                    continue;
                  if (TTER(TerrainAt(x2,y2))->HasFlag(TF_FALL))
                    continue;
                  if (TTER(TerrainAt(x2,y2))->HasFlag(TF_WARN))
                    continue;
                  if (TTER(TerrainAt(x2,y2))->HasFlag(TF_SPECIAL))
                    continue;
                  break;
                  }
                while(1);
                if (cr=GetEncounterCreature(j)) { 
                  cr->PlaceAt(this,x2,y2);
                  cr->Initialize(); 
                  /* Make sure monsters don't have good items, so that
                     the player can't "scum" a level for items through
                     repetition. */
                  for (it=cr->FirstInv();it;it=cr->NextInv())
                    if (it->isMagic() && it->ItemLevel() > min(Depth / 2, Depth - 4))
                      {
                        if (it->isType(T_WEAPON) || it->isType(T_ARMOR) ||
                            it->isType(T_SHIELD) || it->isType(T_MISSILE) ||
                            it->isType(T_CONTAIN))
                          { it->eID = 0; it->SetInherantPlus(0);
                            it->IFlags &= ~IF_CURSED;
                            it->PurgeAllQualities();
                            it->MendHP(1000); }
                        else
                          it->Remove(true);
                      }
				          }
                }
              }
          }
        j = 2 + random(4);
        for(i=0;j!=5;j++)
          {
            Tries = 0;
            do {
              Tries++;
              x = 2 + random(sizeX-4);
              y = 2 + random(sizeY-4);
              if (Tries == 100)
                goto SkipMonUpdate;
        if (SolidAt(x,y))
                continue;
              if (At(x,y).isVault)
                continue;
              if (TTER(TerrainAt(x,y))->HasFlag(TF_FALL))
                continue;
              if (TTER(TerrainAt(x,y))->HasFlag(TF_WARN))
                continue;
              if (TTER(TerrainAt(x,y))->HasFlag(TF_SPECIAL))
                continue;
              if (TTER(TerrainAt(x,y))->HasFlag(TF_WATER))
                continue;
              for(k=0;k!=pc;k++)
                if (dist(x,y,p[k]->x,p[k]->y) <= 25)
                  continue;
              break;
              }
            while(1);

            it = Item::GenItem(IG_STAPLE,dID,Depth,10,StapleItems);
            it->PlaceAt(this,x,y);
          }

      }

    SkipMonUpdate:
    Day = theGame->Day;
    inDaysPassed = false;

  }



int16 Player::Opt(int16 op)
  { return Options[op]; }

int16 Game::Opt(int16 op)
  { 
    int16 i;
    static int8 Options[OPT_LAST];
    if (isValidHandle(p[0]))
      return oPlayer(p[0])->Opt(op); 
    else {
      if (T1->Exists(T1->IncursionDirectory + SC(T1->OptionsSubDir()) + OPT_FILE)) {
        T1->OpenRead(T1->IncursionDirectory + SC(T1->OptionsSubDir()) + OPT_FILE);
        T1->FRead(Options,OPT_LAST);
        T1->Close();
        return Options[op];
        }      
      for (i=0;OptionList[i].Val;i++)
        if (OptionList[i].Val == op)
          return OptionList[i].Default;
      return 0;
      }
  }

/*  Journal Functions */
String & Player::GetJournal()
  {
    return Journal; 
  }

const char* Player::GetMonstersEncountered()
{
  static String mons; 
  mons = Format("  Total: %d\n",JournalInfo.numMonSeen);
  for (int i=1;i<MA_LAST_REAL;i++)
    if (JournalInfo.numMonOfType[i] > 0 && LookupOnly(MTypeNames,i))
      mons += Format("  %4d %s\n", JournalInfo.numMonOfType[i],
          (const char*)Pluralize(Lookup(MTypeNames,i),0));
  return mons; 
} 

void Player::JournalNoteItem(Item *i)
{
  if (!i) return;
  if (!i->isKnown(KN_MAGIC|KN_PLUS))
    return;
  if (m && m->inDaysPassed)
    return;
  if (i->ItemLevel() > JournalInfo.bestItemVal) {
    JournalInfo.bestItemVal = i->ItemLevel();
    JournalInfo.bestItem = i->Name(NA_LONG|NA_IDENT);
  } 
}

void Player::JournalNoteMonster(Monster *m)
{
  String msg;
  if (!m || m->Flags & F_DELETE || m->cHP <= 0) return;
  if (Player::m->inDaysPassed)
    return;
  int cr = m->ChallengeRating();
  if (m->isHostileTo(this)) {
    if (cr > JournalInfo.bestMonsterVal) {
      JournalInfo.bestMonsterVal = m->ChallengeRating();
      JournalInfo.bestMonster = m->Name(NA_LONG);
    } 
  } 
  int16 track = AbilityLevel(CA_TRACKING); 
  if (track > 0) { 
    StatiIterNature(this,TRACKING)
        if (S->h == m->myHandle) {
          S->Duration = track * 10; 
          S->Mag = track; 
        StatiIterBreakout(this,goto found_it)
      } 
    StatiIterEnd(this)
    GainTempStati(TRACKING,m,track*10,SS_MISC,0,track);
found_it: ;
  } 
  if (m->isHostileTo(this) || this->isHostileTo(m))
    /* ww: Dwarven Auto-Focus */
    if (Opt(OPT_DWARVEN_AUTOFOCUS) && 
        ((Creature *)m)->ChallengeRating() >= ChallengeRating() && 
        HasAbility(CA_DWARVEN_FOCUS) && 
        !GetStati(DWARVEN_FOCUS,-1,0)) {
      GainPermStati(DWARVEN_FOCUS,m,SS_RACE,0,0,0);
      m->Flags |= F_HILIGHT;
      msg = Format("You swear a vow to slay %c%s%c.",
          -RED, (const char*)m->Name(NA_THE), -GREY);
      MyTerm->Message(msg);
      MyTerm->Box(msg);
    } 

  if (!m->HasStati(HALLU)) {
    m->GainPermStati(HALLU,0,SS_MISC);
    JournalInfo.numMonSeen++; 
    for (int i=1;i<MA_LAST_REAL;i++)
      if (m->isMType(i))
        JournalInfo.numMonOfType[i]++; 
  } 
}

void Player::AddJournalEntry(const char* s)
{
    uint8 hour, min, sec, day; 
    String when, where, stat, best;
    hour = (theGame->GetTurn()/18000) % 24;
    min  = (theGame->GetTurn()/300) % 60;
    sec  = ((theGame->GetTurn()/5) % 60);
    day  = (theGame->GetTurn()/432000L);

    when = Format("\n<15>%d:%02d %s, %d%s of Suntide:<7> ", 
                    (hour%12)+1, min,
        (hour > 12) && (hour != 23) ? "PM" : "AM", day+1,
        day%10==0 ? "st" :
        day%10==1 ? "nd" :
        day%10==2 ? "rd" : "th");

    if (m && m->InBounds(x,y) && TREG(m->RegionAt(x,y))->Name)    
            where = Format("%s, %d' ",NAME(m->RegionAt(x,y)),m->Depth * 10);
    else
            where = "Unknown Location ";

    stat = Format("(H:<15>%d<7>/<15>%d<7> M:<15>%d<7>/<15>%d<7> XP:<15>%d<7>) ",
      (int16)cHP+KAttr[A_THP]-Attr[A_THP],
      (int16)mHP+Attr[A_THP],
      (int16)cMana(),
      (int16)tMana(),
      XP);

    best = "\n  ";
    if (JournalInfo.bestMonsterVal >= 0) {
      best += Format("Seen: %s (CR %d)\n  ",(const char*)JournalInfo.bestMonster,
                      JournalInfo.bestMonsterVal);
      JournalInfo.bestMonsterVal = -1; 
    }
    if (JournalInfo.bestItemVal >= 0) {
      best += Format("Seen: %s (IL %d)\n  ",(const char*)JournalInfo.bestItem,
                      JournalInfo.bestItemVal);
      JournalInfo.bestItemVal = -1; 
    }

    Journal = Journal + (when + where + stat + best + SC(s) + SC("\n"));
}

/* pick up all items that would stack with something we are carrying */
void Player::AutoPickupFloor()
{
  Thing * t;
  Item * i; 
  Container *pack;
  int j;
  int ap = Opt(OPT_AUTOPICKUP);
  int kc = Opt(OPT_KILL_CHEST); 
  int oc = Opt(OPT_AUTOCHEST); 
  bool did_anything = false; 

  if (!ap) return;  
  
  if (HasStati(RAGING) || HasMFlag(M_NOHANDS) ||
      isThreatened())
    return;  
  
  if (!m->InBounds(x,y)) return; 

start_again: 

  for (t = m->FirstAt(x,y) ; t ; t = m->NextAt(x,y))  {
 
    if (t->isIllusion())
      continue;
 
    /* Don't autopickup really heavy things; more notably, quit
       trying to autostow fountains and thrones! */
    if (t->isItem() && t->Type != T_CHEST)
      if (((Item*)t)->Weight(false) > 1000)
        continue;
        
    if (t->HasStati(DROPPED,-1,this))
      continue;
 
    if (t->isItem() && t->Type == T_CHEST) {
      Container *c = (Container *)oItem(t->myHandle); 
      if (c->Contents == 0) {
        if (kc) { 
          IPrint("KillChest: <Obj>.",c);
          t->Remove(true);
          goto start_again;
        }
      } else if (oc) {
        EventInfo e;
        e.Clear();
        e.EPActor = this;
        if (c->DumpOutContents(e) == DONE) goto start_again;
      } 
    } 

    if (t->isItem() && t->Type != T_LIGHT &&
         !t->isType(T_TOOL) && !t->isType(T_SYMBOL) &&
         !t->isType(T_SHIELD)) {
      i = oItem(t->myHandle);
      int checkSlots[] = { 
        SL_LSHOULDER ,
        SL_RSHOULDER ,
        SL_BELT1     ,
        SL_BELT2     ,
        SL_BELT3     ,
        SL_BELT4     ,
        SL_BELT5     ,
        0
      } ;
      for (int jj=0;checkSlots[jj];jj++) {
        j = checkSlots[jj];
        if (InSlot(j) && *InSlot(j) == *i) {
          IPrint("AutoPickup: <Obj>.",i);
          SetSilence();
          if (Throw(EV_PICKUP,this,NULL,i) == ABORT)
            UnsetSilence(); // return;
          else { 
          did_anything = true; 
            UnsetSilence();
            LegendIdent(i);
          goto start_again; // because Remove() will change the linked 
          } 
          // list of items here 
        } 
      }
      if (ap == 2 && Inv[SL_PACK] && 
          oItem(Inv[SL_PACK])->Type == T_CONTAIN && InSlot(SL_PACK)) { 
              // also check pack
        pack = (Container*)oItem(Inv[SL_PACK]);
        if (pack)
         for (j=0; (*pack)[j] != NULL; j++) {
           if (*((*pack)[j]) == *i) {
             IPrint("AutoPickup: <Obj>.",i);
             SetSilence();
             if (Throw(EV_PICKUP,this,NULL,i) == ABORT)
               UnsetSilence(); // return;
             else { 
               UnsetSilence(); // return;
               did_anything = true; 
               LegendIdent(i);
               goto start_again; // because Remove() will change the linked 
             } 
              // list of items here 
           }
         } 
      }
    }
    if (t->isItem()) {
      i = oItem(t->myHandle);
      if (i->PItemLevel(this) >= 0 && i->eID &&
          !i->isKnown()) {
        if (ap == 2 && Inv[SL_PACK] && 
            oItem(Inv[SL_PACK])->Type == T_CONTAIN &&
            (InSlot(SL_PACK) || AbilityLevel(CA_WILD_SHAPE))) { 
          pack = (Container*)oItem(Inv[SL_PACK]);
          if (pack) {
            IPrint("AutoStowing: <Obj>.",i);
             SetSilence();
            if (Throw(EV_INSERT,this,NULL,pack,i) == ABORT) {
              UnsetSilence(); // return;
              IPrint("AutoStowing failed.");
              }
            else { 
              UnsetSilence(); // return;
              did_anything = true; 
              LegendIdent(i);
              goto start_again; // because Remove() will change the linked 
            } 
          } 
        }
      } 
      LegendIdent(i);
    }

  }
}

#include <time.h>

void Player::ResetLevelStats()
{
  GameTimeInfo.start_turn = theGame->Turn;
  GameTimeInfo.start_second = time(NULL);
  GameTimeInfo.start_xp = XP;
  GameTimeInfo.keystrokes = 0;
  GameTimeInfo.actions = 0;
}

void Player::StoreLevelStats(uint8 depth)
{
  GameTimeInfo.LTI[depth].turns += theGame->Turn - GameTimeInfo.start_turn;
  GameTimeInfo.LTI[depth].seconds += time(NULL) - GameTimeInfo.start_second;
  GameTimeInfo.LTI[depth].keystrokes += GameTimeInfo.keystrokes;
  GameTimeInfo.LTI[depth].actions += GameTimeInfo.actions;
  GameTimeInfo.LTI[depth].xp += XP - GameTimeInfo.start_xp;
  ResetLevelStats(); 
}

String & Player::GetLevelStats()
{
  String s;
  s = "\n";

  s+="Lvl | Act | Key | Turns | Secs |  XP | Explored\n";
  s+="----+-----+-----+-------+------+-----+------------------\n";

  // rID mID = RES(m->dID)->GetConst(BELOW_DUNGEON);

  for (int i=1; i<=10; i++) 
    if (GameTimeInfo.LTI[i].keystrokes) { 
      rID dID = m->dID; 
      Map * m = theGame->GetDungeonMap(dID,i,this);

      uint16 open = 0;
      uint16 explored = 0;
      for (int x=0; x<m->SizeX(); x++)
        for (int y=0; y<m->SizeY(); y++) {
          if (!m->SolidAt(x,y)) { open++;
            if (m->At(x,y).Memory) explored++;
          }
        } 

      s += Format("%-4d|%5d|%5d|%7d|%6ld|%5d|%4d / %4d = %d~\n",
          i,
          (int32)GameTimeInfo.LTI[i].actions,
          (int32)GameTimeInfo.LTI[i].keystrokes,
          (int32)GameTimeInfo.LTI[i].turns,
          GameTimeInfo.LTI[i].seconds,
          (int32)GameTimeInfo.LTI[i].xp,
          explored,
          open,
          100 * explored / open);
  } 
  return *tmpstr(s); 
}


void Character::Exercise(int16 at, int16 amt, int16 col, int16 cap)
  {
    int16 total, needed, sk, i, adding;
    ASSERT(col < 15);
    ASSERT(at < 7);
    
    if (SumStatiMag(ADJUST_INH,at) >= 5 + AbilityLevel(CA_INHERANT_POTENTIAL))
      return;
    
    if (amt < 0)
      {
        amt = -amt;
        for (i=0;i!=15;i++)
          {
            if (GainAttr[at][i] >= amt)
              {
                GainAttr[at][i] -= amt;
                return;
              }
            else
              {
                amt -= GainAttr[at][i];
                GainAttr[at][i] = 0;
              }
          }
        return;
      }

    if (GainAttr[at][col] >= cap)
      return;
  
    switch (at)
      {
        case A_STR: case A_DEX: case A_CON:
          needed = 100 - max(0,SkillLevel(SK_ATHLETICS));
         break;
        case A_WIS:
          needed = 100 - max(0,SkillLevel(SK_CONCENT));
         break;
        case A_CHA:
          needed = 100 - max(0,SkillLevel(SK_PERFORM));
         break;
        case A_INT:
          sk = 0;
          for (i=SK_KNOW_FIRST;i<=SK_KNOW_LAST;i++)
            if (sk < SkillLevel(i))
              sk = SkillLevel(i);
          needed = 100 - sk;
         break;
        case A_LUC:
          needed = 100;
         break;
      }
      
    total = 0;
    for (i=0;i!=15;i++)
      total += GainAttr[at][i];
      
    adding = min(cap - GainAttr[at][col], amt);
    
    if (total + adding > needed)
      {
        adding = amt - (needed - total);
        for (i=0;i!=15;i++)
          GainAttr[at][i] = 0;
        
        GainInherantBonus(at,+1,true);
      }
      
    GainAttr[at][col] += amt;
  }
    
int16 Player::getMaxDepth(rID dID)
  {
    int16 i;
    for (i=0;i!=MAX_DUNGEONS;i++)
      if (theGame->DungeonID[i] == dID)
        if (MaxDepths[i] > 0)
          return MaxDepths[i];
    return 0;
  }