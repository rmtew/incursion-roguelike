/* INLINE.H -- Copyright (c) 1999-2003 Julian Mensch
     This file contains definitions for all of Incursion's
   inline functions.
   
     inline String::String()                 
     inline int16 random(int16 mx)
     inline int16 LevelAdjust(int16 val, int16 level, int16 spec=1)
     inline const char* Lookup(TextVal*list,int32 val)
     inline const char* LookupOnly(TextVal*list,int32 val)
     inline int32 LookupStr(TextVal *list, const char*str)
     inline Glyph AdjustGlyphDir(Glyph G, Dir d)
     inline int16 dist(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
     inline bool Thing::HasStati(int16 n,int8 Val,Thing *t)
     inline bool Thing::HasEffStati(int16 n,rID eID)
     inline bool Creature::SavingThrow(int16 type, int16 DC, uint32 Subtype)
     inline bool Map::FieldAt(int16 x,int16 y, uint16 FType)
     inline Glyph Map::FieldGlyph(int16 x,int16 y, Glyph og) 
     inline void Player::ShowStats()
     inline bool Player::yn(const char*msg,bool def)
     inline Annotation* Resource::Annot(int16 i)
     inline Annotation * Resource::NewAnnot(int8 at,int16 *num)
     inline Player* Game::GetPlayer(int8 n)
     inline Object::Object(int8 _Type)
     inline Object::~Object()
     inline void Game::StopTime(Creature *caster)
     inline Dir OppositeDir(Dir d)
     inline int8 Player::ChoicePrompt(const char*msg, const char*choices, char def, int8 col1,int8 col2)
     inline Thing* GetHandle(Thing *t)
     inline SetSilence()
     inline UnsetSilence()
     inline int16 MMFeatLevels(uint32 MM)

    NOTE: What functions are and are not included in this file should be 
  SERIOUSLY reevaluated in the near future.
*/

inline int16 random(int16 mx)
  {
    if (!mx) return 0;
    return (genrand_int32() % mx);
  }
  
#define srand(n) init_genrand(n)




inline int16 LevelAdjust(int16 val, int16 level, int16 spec=1)
  {
    if (val >= LEVEL_LAST)
      return val;
    switch(val) 
      {
        case LEVEL_1PER1: return level;    
        case LEVEL_2PER1: return level*2;  
        case LEVEL_3PER1: return level*3;  
        case LEVEL_4PER1: return level*4;  
        case LEVEL_5PER1: return level*5;  
    
        case LEVEL_1PER2: return level/2;  
        case LEVEL_1PER3: return level/3;  
        case LEVEL_1PER4: return level/4;  
        case LEVEL_1PER5: return level/5;  

        case LEVEL_2PER3: return (level*2)/3;  
        case LEVEL_3PER2: return (level*3)/2;  
        case LEVEL_3PER4: return (level*3)/4;  
        case LEVEL_4PER3: return (level*4)/3;  


        case LEVEL_EVERY2: return (level+1)/2;  
        case LEVEL_EVERY3: return (level+2)/3;  
        case LEVEL_EVERY4: return (level+3)/4;  
        case LEVEL_EVERY5: return (level+4)/5;  

        case LEVEL_2EVERY2: return ((level+1)/2)*2;  
        case LEVEL_2EVERY3: return ((level+2)/3)*2;  
        case LEVEL_2EVERY4: return ((level+3)/4)*2;  
        case LEVEL_2EVERY5: return ((level+4)/5)*2;  

        case LEVEL_MAX5:    return min(5,level);  
        case LEVEL_MAX10:   return min(10,level);  
        case LEVEL_MAX15:   return min(15,level);  
        case LEVEL_MAX20:   return min(20,level);  

        case LEVEL_SCALED:
          if (level <= 9)
            return level;
          else
            return 9 + (level-9)/3;
        case LEVEL_SCALED2: 
          if (level <= 10)
            return level/2;
          else
            return 5 + (level-10)/5;
        case LEVEL_SCALED3:
          if (level <= 12)
            return level;
          else
            return 12 + (level-12)/2;
        
        case LEVEL_PLUS5: return level+5;
        case LEVEL_PLUS10: return level+10;
        case LEVEL_PLUS20: return level+20;
        case LEVEL_HALF_PLUS3: return 3+(level/2);

        case PLUS_1PER1: return spec;
        case PLUS_2PER1: return spec*2;
        case PLUS_3PER1: return spec*3;
        case PLUS_4PER1: return spec*4;
        case PLUS_5PER1: return spec*5;

        case PLUS_10PER1: return spec*10;
        case PLUS_15PER1: return spec*15;
        case PLUS_20PER1: return spec*20;

        case PLUS_MINUS2: return max(spec-2,0);
        case PLUS_MINUS3: return max(spec-3,0);

        case LEVEL_MIN3: return max(3,level);
        case LEVEL_MIN5: return max(5,level);
        case LEVEL_MIN10: return max(10,level);

        case PLUS_ADD1:  return spec+1;
        case PLUS_ADD2:  return spec+2;
        case PLUS_ADD3:  return spec+3;
        case PLUS_ADD4:  return spec+4;
        case PLUS_ADD5:  return spec+5;
        case PLUS_ADD7:  return spec+7;
        case PLUS_ADD10: return spec+10;
        case PLUS_ADD15: return spec+15;
        
        case PLUS_2PER1_ADD1:  return spec*2+1;
        case PLUS_2PER1_ADD2:  return spec*2+2;
        case PLUS_2PER1_ADD3:  return spec*2+3;
        case PLUS_2PER1_ADD4:  return spec*2+4;
        case PLUS_2PER1_ADD5:  return spec*2+5;
        case PLUS_2PER1_ADD7:  return spec*2+7;
        case PLUS_2PER1_ADD10: return spec*2+10;
        case PLUS_2PER1_ADD15: return spec*2+15;

        case PLUS_3PER1_ADD1:  return spec*3+1;
        case PLUS_3PER1_ADD2:  return spec*3+2;
        case PLUS_3PER1_ADD3:  return spec*3+3;
        case PLUS_3PER1_ADD4:  return spec*3+4;
        case PLUS_3PER1_ADD5:  return spec*3+5;
        case PLUS_3PER1_ADD7:  return spec*3+7;
        case PLUS_3PER1_ADD10: return spec*3+10;
        case PLUS_3PER1_ADD15: return spec*3+15;

        case PLUS_4PER1_ADD1:  return spec*4+1;
        //case PLUS_4PER1_ADD2:  return spec*4+2;
        case PLUS_4PER1_ADD3:  return spec*4+3;
        //case PLUS_4PER1_ADD4:  return spec*4+4;
        case PLUS_4PER1_ADD5:  return spec*4+5;
        case PLUS_4PER1_ADD7:  return spec*4+7;
        case PLUS_4PER1_ADD10: return spec*4+10;
        case PLUS_4PER1_ADD15: return spec*4+15;

        case PLUS_5PER1_ADD1:  return spec*5+1;
        //case PLUS_5PER1_ADD2:  return spec*5+2;
        case PLUS_5PER1_ADD3:  return spec*5+3;
        //case PLUS_5PER1_ADD4:  return spec*5+4;
        case PLUS_5PER1_ADD5:  return spec*5+5;
        case PLUS_5PER1_ADD7:  return spec*5+7;
        case PLUS_5PER1_ADD10: return spec*5+10;
        case PLUS_5PER1_ADD15: return spec*5+15;


        default:
          Fatal("Strange level constant in LevelAdjust!");
          return 1;
      }
  }


inline const char* Lookup(TextVal*list,int32 val)
  {
    static char Buff[15];
    while(list->Text)
      {
        if(list->Val == val || (list->Val == -1))
          return list->Text;
        list++;
      }
    sprintf(Buff,"(undefined %d)",val);
    return Buff;
  }

inline const char* LookupOnly(TextVal*list,int32 val)
  {
    while(list->Text)
      {
        if(list->Val == val)
          return list->Text;
        list++;
      }
    return NULL;
  }

inline int32 LookupStr(TextVal *list, const char*str)
  {
    while(list->Text)
      {
        if(stricmp(list->Text,str)==0)
          return list->Val;
        list++;
      }
    return 0;
  }

inline Glyph AdjustGlyphDir(Glyph G, Dir d)
  {
    Glyph g = G & 0x00FF;
    switch (g)
      {
        case GLYPH_ARROW_UP:
        case GLYPH_ARROW_DOWN:
        case GLYPH_ARROW_LEFT:
        case GLYPH_ARROW_RIGHT:
          switch(d) {
            case NORTH: return GLYPH_ARROW_UP | (G & 0xFF00);
            case SOUTH: return GLYPH_ARROW_DOWN | (G & 0xFF00);
            case EAST:  return GLYPH_ARROW_RIGHT | (G & 0xFF00);
            case WEST:  return GLYPH_ARROW_LEFT | (G & 0xFF00);
            case NORTHEAST: case SOUTHWEST: return '/' | (G & 0xFF00);
            case NORTHWEST: case SOUTHEAST: return '\\' | (G & 0xFF00);
            default:                        return '*' | (G & 0xFF00);
            }
         break;
        default:
          return G;
      }
  }

inline const int16 dist(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
  { 
    return abs(x1 - x2) > abs(y1 - y2) ? 
      abs(x1 - x2) + abs(y1 - y2) / 2 :
      abs(y1 - y2) + abs(x1 - x2) / 2; 
  }

inline bool Thing::HasStati(int16 n,int16 Val,Thing *t)
  {
    if (Val == -1)
      theGame->ccHasNatStati++;
    else
      theGame->ccHasValStati++;
    if (n != -1 && n < 256)
      theGame->ccStatiNat[n]++;
    if (theGame->inPercieve)
      theGame->ccHSPercieve++;
    if (theGame->inCalcVal)
      theGame->ccHSCalcVal++;
    
      
    StatiIterNature(this,n)
      if ((Val == -1 || S->Val == Val) &&
          (t == NULL || S->h == t->myHandle))
        { theGame->ccHasStatiTrue++; 
          StatiIterBreakout(this,return true) }
    StatiIterEnd(this)
    theGame->ccHasStatiFalse++;
    return false; 
  }

inline int16 Thing::CountStati(int16 n,int16 Val,Thing *t)
  {
    int16 c;
    c=0;
    StatiIterNature(this,n)
      if ((Val == -1 || S->Val == Val) &&
          (t == NULL || S->h == t->myHandle))
        c++; 
    StatiIterEnd(this)
    return c; 
  }


inline bool Thing::HasEffStati(int16 n,rID eID, int16 Val, Thing *t)
  {
    theGame->ccHasEffStati++;
    if (n == -1) {
      StatiIter(this)
        if ((n == INNATE_SPELL || S->Nature != INNATE_SPELL) &&
            (S->eID == eID)) 
          if ((Val == -1) || (Val == S->Val) ||
              (S->Nature == TRAP_EVENT && S->Mag == Val))
            if ((!t) || (t->myHandle == S->h))
              StatiIterBreakout(this,return true)
      StatiIterEnd(this)
      } 
    else 
      {
        StatiIterNature(this,n)
          if (S->eID == eID) 
            if ((Val == -1) || (Val == S->Val))
              if ((!t) || (t->myHandle == S->h))
                StatiIterBreakout(this,return true)
        StatiIterEnd(this)
      }
    return false;
  } 

inline int16 Thing::CountEffStati(int16 n,rID eID, int16 Val)
  {
    int16 c;
    c=0;
    if (n == -1) {
      StatiIter(this)
        if ((n == INNATE_SPELL || S->Nature != INNATE_SPELL) &&
            (S->eID == eID)) 
          if ((Val == -1) || (Val == S->Val))
            c++;
      StatiIterEnd(this)
      } 
    else 
      {
        StatiIterNature(this,n)
          if (S->eID == eID) 
            if ((Val == -1) || (Val == S->Val))
              c++;
        StatiIterEnd(this)
      }
    return c;
  } 


#if 0

inline bool Creature::SavingThrow(int16 type, int16 DC, uint32 Subtype)
  { 
    int16 Bonus, i;
    static const char *save_name[3] = { "Fortitude", "Reflex", "Will" };
    if (DC <= 0)
      return false;
    bool show = false; 

    if (theGame->Opt(OPT_SHOWSAVES) &&
        (isPlayer() || theGame->GetPlayer(0)->XPercieves(this)))
      show = true; 

    int16 roll = Dice::Roll(1,20); 

    Bonus = Attr[A_SAV_FORT + type];

    String bStr ; 
    if (show) { 
      bStr = Format("%c%s Save:%c 1d20 (%d)",
          -13,save_name[type],-7,roll);
      if (Bonus)
        bStr += Format(" %+d base",Bonus);
    }

    for(i=0;Stati[i];i++) 
      if (Stati[i]->Nature == SAVE_BONUS)  
        if (BIT(Stati[i]->Val) & Subtype) {
          Bonus += Stati[i]->Mag;        
          if (show) bStr += Format(" %+d (%s)",Stati[i]->Mag,
            Lookup(SaveBonusNames, Stati[i]->Val));
        }
    if (type == REF && HasStati(STUCK)) {
      Bonus -= 30;
      if (show) bStr += " -30 (entangled)";
      // ww: if you can't move, you normally can't make a reflex save ...
      // but if by some miracle you can overcome a -30 penalty, more power
      // to you!
    } 
    RemoveOnceStati(ADJUST,A_SAV);
    if (type == FORT)
      RemoveOnceStati(ADJUST,A_SAV_FORT);
    if (type == REF)
      RemoveOnceStati(ADJUST,A_SAV_REF);
    if (type == WILL)
      RemoveOnceStati(ADJUST,A_SAV_WILL);
    if (show) {
      bStr += Format(" = %d vs DC %d %c[%s]%c.",
          Bonus + roll, DC, 
          (Bonus + roll >= DC) ? -EMERALD: -PINK,
          (Bonus + roll >= DC) ? "success" : "failure",
          -7);
      Term * term;
      if (isPlayer()) 
        term = thisp->MyTerm;
      else {
        term = theGame->GetPlayer(0)->MyTerm;
        bStr = this->Name(NA_CAPS|NA_POSS) + " " + bStr;
      }
      term->SetWin(WIN_NUMBERS);
      term->Clear();
      term->Write(0,0,bStr);
      if (theGame->Opt(OPT_STORE_ROLLS))
        term->AddMessage(bStr);
      /* why is this here?  
        term->Message(bStr);
      */
    }

    return Bonus + roll >= DC;
  }

#endif

inline bool Map::FieldAt(int16 x,int16 y, uint32 FType)
  {
    int16 i;
    if (!At(x,y).hasField)
      return false;
    for(i=0;Fields[i];i++)
      if (Fields[i]->inArea(x,y))
        if ((FType == -1) || (FType & Fields[i]->FType))
          return true;
    return false;
  }

inline Glyph Map::FieldGlyph(int16 x,int16 y, Glyph og) 
  {
    int16 i; Glyph g; uint32 best = 0;
    if (!At(x,y).Visibility & VI_VISIBLE)
      return og;

    Field * f; 
    Creature * seer = oCreature(pl[0]);  // HACK

    for(i=0;f = Fields[i];i++) {
      if (seer->PercievesField(x,y,f)) {
        if ((f->FType > best) && 
            (f->FType != FI_SILENCE) &&
            (f->FType != (FI_SILENCE|FI_MOBILE))) {
          if (f->FType & FI_ITERRAIN)
            {
              rID trueID, illID, perID;
              trueID = TerrainAt(x,y);
              illID  = PTerrainAt(x,y,NULL);
              perID  = PTerrainAt(x,y,seer);
              g = TTER(illID)->Image;
              if (illID != perID)
                g = (g & 0xF0FF) | (11 * 0xFF);
            }
          else
            g = Fields[i]->Image; 
          best = Fields[i]->FType; 
        }
      } 
    } 
    if (best == 0)
      return og; 

    /* Size Glyphs should not be displayed over solid stone, since
       stone/size overlaps are supposed to represent large creatures
       squeezing into smaller spaces. */
    if ((g & 0xFF) == GLYPH_BULK)
      if (SolidAt(x,y))
        return og;

    if ((og & 0xFF) != GLYPH_FLOOR && 
        (og & 0xFF) != GLYPH_FLOOR2 &&
        (((g & 0xFF) != GLYPH_BULK) || 
         (At(x,y).Contents && oThing(At(x,y).Contents)->isCreature()))) {
      return og;
      //return (og & 0x00FF) | (g & 0xFF00); 
    }

    if ((g & 0x00FF) == GLYPH_FLOOR2)
      if (((og & 0x00FF) != GLYPH_FLOOR) && ((og & 0x00FF) != GLYPH_FLOOR2))
        return og;
    if ((g & 0xF0FF) == 0) {
      /* No Glyph Proper */
      if ((og & 0x00FF) != GLYPH_FLOOR)
        return og;
      return (g & 0x0F00) | (At(x,y).Glyph & 0xF0FF);
      }
    else
      return (g & 0x0FFF) | (At(x,y).Glyph & 0xF000);

  }

inline int16 CalcFEBonus(int16 MType, int16 lev)
  {
    for (int16 i=0;MonGroupRarities[i];i+=2)
      if (MonGroupRarities[i] == MType)
        switch (MonGroupRarities[i+1])
          {
            case MR_XRARE:
              return ((lev+2)*10/3)/5;
            case MR_VRARE:
              return ((lev+1)/2);
            case MR_RARE:
              return ((lev+2)/3);
            case MR_UNCOMMON:
              return ((lev+3)/4);
            case MR_COMMON:
              return ((lev+5)/6);
            default:
              Error("Strange Monster Rarity in CalcFEBonus!");
          }
    return ((lev+3)/4);
  }

inline void Player::ShowStats()
  { MyTerm->ShowTraits(); MyTerm->ShowStatus(); }
inline bool Player::yn(const char*msg,bool def)
  { return MyTerm->yn(msg); }

inline Annotation* Resource::Annot(int16 i)
  { return i ? theGame->Modules[ModNum]->Annotations[i] : NULL; }

inline Annotation * Resource::NewAnnot(int8 at,int16 *num, int16 *num2)
  { *num = theGame->Modules[ModNum]->Annotations.Total();
    if (num2)
      *num2 = *num; 
    Annotation * a = theGame->Modules[ModNum]->Annotations.NewItem();
    ASSERT(a);
    memset(a,0,sizeof(Annotation)); a->AnType = at; return a; }

inline Player* Game::GetPlayer(int8 n)
  { return isValidHandle(p[n]) ? oPlayer(p[n]) : NULL; }

inline Object::Object(int8 _Type)
  {
    Type = _Type;
    #ifdef DEBUG
    if (!theRegistry)
      Fatal("theReg not set before first Object creation!");
    #endif
    if (Type == T_MODULE)
      myHandle = ResourceRegistry.RegisterObject(this);
    else
      myHandle = MainRegistry.RegisterObject(this);
  }

inline Object::~Object()
  { if (Type == T_MODULE)
      ResourceRegistry.RemoveObject(this);
    else
      MainRegistry.RemoveObject(this); }

inline void Game::StopTime(Creature *caster)
  { Timestopper = caster->myHandle; }

inline Dir OppositeDir(Dir d)
  { switch(d) {
      case NORTH:     return SOUTH;
      case SOUTH:     return NORTH;
      case EAST:      return WEST;
      case WEST:      return EAST;
      case NORTHEAST: return SOUTHWEST;
      case NORTHWEST: return SOUTHEAST;
      case SOUTHWEST: return NORTHEAST;
      case SOUTHEAST: return NORTHWEST;
      case UP:        return DOWN;
      case DOWN:      return UP;
      default:
        return d;
    }
  }

inline int8 Player::ChoicePrompt(const char*msg, const char*choices, char def, 
                      int8 col1,int8 col2, bool preprompt)
  { return MyTerm->ChoicePrompt(msg,choices,col1,col2,preprompt); }

inline Thing* GetHandle(Thing *t)
  { return t; }

inline void SetSilence()
  { Silence++; }

inline void UnsetSilence()
  { Silence = max(0,Silence-1); }

inline int16 MMFeatLevels(uint32 MM)
  {
    int16 i = 0;
    if (MM & MM_AMPLIFY) 
      i += 2;
    if (MM & MM_AUGMENT)
      i += 2;
    if (MM & MM_ANCHOR)
      i += 1;
    if (MM & MM_BIND)
      i += 1;
    if (MM & MM_CONSECRATE)
      i += 2;
    if (MM & MM_CONTROL)
      i += 3;
    if (MM & MM_DEFENSIVE)
      i += 2;
    if (MM & MM_DEFILING)
      i += 1;
    if (MM & MM_JUDICIOUS)
      i += 2;
    if (MM & MM_EMPOWER)
      i += 2;
    if (MM & MM_ENLARGE)
      i += 1;
    if (MM & MM_EXTEND)
      i += 2;
    if (MM & MM_FOCUS)
      i += 1;
    if (MM & MM_FORTIFY)
      i += 1;
    if (MM & MM_HEIGHTEN)
      i += 2;
    if (MM & MM_INHERANT)
      i += 1;
    if (MM & MM_MAXIMIZE)
      i += 3;
    if (MM & MM_PROJECT)
      i += 2;
    if (MM & MM_QUICKEN)
      i += 4;
    if (MM & MM_REPEAT)
      i += 3;
    if (MM & MM_STILL)
      i += 1;
    if (MM & MM_SURE)
      i += 2;
    if (MM & MM_TRANSMUTE)
      i += 1;
    if (MM & MM_UNSEEN)
      i += 2;
    if (MM & MM_VILE)
      i += 2;
    if (MM & MM_VOCALIZE)
      i += 1;
    if (MM & MM_WARP)
      i += 2;
  
    return i;
  }

inline bool Map::LightAt(int16 x, int16 y)
  { if (FieldAt(x,y,FI_DARKNESS|FI_SHADOW))
      if (!FieldAt(x,y,FI_LIGHT) || FieldAt(x,y,FI_DARKNESS))
        return false;
    return At(x,y).Lit || FieldAt(x,y,FI_LIGHT); }

inline bool Map::BrightAt(int16 x, int16 y)
  { if (FieldAt(x,y,FI_DARKNESS|FI_SHADOW))
      return false;
    return At(x,y).Bright || FieldAt(x,y,FI_LIGHT); }

inline const char* NumPrefix(int16 n)
  { if (n >= 10 && n <= 19) return "th";
    if (n % 10 == 1)        return "st";
    if (n % 10 == 2)        return "nd";
    if (n % 10 == 3)        return "rd";
    return "th"; }

inline Dir DirTo(int16 sx, int16 sy, int16 tx, int16 ty)
  {
    if (tx == sx) {
      if (ty > sy) return SOUTH;
      if (ty < sy) return NORTH;
      return CENTER; }
    if (tx < sx) {
      if (ty > sy) return SOUTHWEST;
      if (ty < sy) return NORTHWEST;
      return WEST; }
    if (ty > sy) return SOUTHEAST;
    if (ty < sy) return NORTHEAST;
    return EAST;
  }

inline int16 Thing::SumStatiMag(int16 n,int16 Val, Thing *t)
  { 
    ASSERT(n > 0 && n < LAST_STATI); 
    int16 m = 0; 
    StatiIterNature(this,n)
        if (S->Val == Val || Val == -1) 
          if (!t || S->h == t->myHandle)
            m += S->Mag;
    StatiIterEnd(this)
    return m; 
  }
inline int16 Thing::HighStatiMag(int16 n,int16 Val, Thing *t)
  { 
    ASSERT(n > 0 && n < LAST_STATI); 
    int16 m = 0; 
    StatiIterNature(this,n)
        if (S->Val == Val || Val == -1) 
          if (!t || S->h == t->myHandle)
            m = max(m,S->Mag);
    StatiIterEnd(this)
    return m; 
  }
inline int16 Thing::SumStatiVal(int16 n, Thing *t)
  { 
    ASSERT(n > 0 && n < LAST_STATI); 
    int16 v = 0; 
    StatiIterNature(this,n)
        if (!t || S->h == t->myHandle)
          v += S->Val;
    StatiIterEnd(this)
    return v; 
  }
inline int16 Thing::XORStatiVal(int16 n, Thing *t)
  { 
    int16 v = 0; 
    ASSERT(n > 0 && n < LAST_STATI); 
    StatiIterNature(this,n)
        if (!t || S->h == t->myHandle)
          v |= S->Val;
    StatiIterEnd(this)
    return v; 
  }

inline int16 Thing::HighSourceCLev(uint8 src)
  {
    int16 clev = 0;
    StatiIter(this)
      if (S->Source == src)
        clev = max(0,clev);
    StatiIterEnd(this)
    return clev;
  }
  
inline bool Thing::HasStatiFromSource(uint8 src)
  {
    bool found=false;
    StatiIter(this)
      if (S->Source == src)
        found = true;
    StatiIterEnd(this)
    return found;
  }
  
inline bool Thing::containsAllStatiFrom(Thing *t, const uint8 *IgnoreNatures)
  {
    bool missing, found; Status *S2;
    int16 n; missing = false;
    StatiIter(this)
      if (strchr((const char*)IgnoreNatures,S->Nature))
        continue;
        
      S2 = S; found = false;
      StatiIterNature(t,S2->Nature)
        if (S2->Val == S->Val && S2->Source == S->Source &&
            S2->eID == S->eID && S2->h == S->h)
          found = true;
      StatiIterEnd(t)
      S = S2;
      if (!found)
        missing = true;
    StatiIterEnd(this)
    return missing ? false : true;
  }
  
inline int32 XCR(int16 CR)
  {
    if (CR >= 1)
      return ((CR+3L)*(CR+3L)*(CR+3L));
    switch (CR)
      {
        case 0:  return 55;
        case -1: return 44;
        case -2: return 33;
        case -3: return 26;
        case -4: return 22;
        case -5: return 18;
        case -6: return 15;
        case -7: return 12;
        case -8: return 10;
      }
    return 6;
  }
  
inline int16 XCRtoCR(int32 XCR)
  {
    int16 i;
    static int32 Cubes[] = {
      10, 12, 15, 18, 22, 26, 33,
      44, 55, 4*4*4,  5*5*5,    6*6*6,    
      7*7*7,    8*8*8,    9*9*9,    10*10*10, 
      11*11*11, 12*12*12, 13*13*13, 14*14*14, 
      15*15*15, 16*16*16, 17*17*17, 18*18*18, 
      19*19*19, 20*20*20, 21*21*21, 22*22*22, 
      23*23*23, 24*24*24, 25*25*25, 26*26*26, 
      27*27*27, 28*28*28, 29*29*29, 30*30*30,
      31*31*31, 32*32*32, 33*33*33, 34*34*34,
      35*35*35, 36*36*36, 37*37*37, 38*38*38,
      39*39*39 };

    for (i=0;i!=43;i++)
      if (Cubes[i] >= XCR)
        return i-8;
    return 36;
  }

inline bool isFamiliar(int16 sk)
  { return (sk == SK_DECIPHER) ||
           (sk == SK_KNOW_THEO) ||
           (sk == SK_KNOW_GEO) ||
           (sk == SK_ATHLETICS) ||
           (sk == SK_APPRAISE) ||
           (sk == SK_SEARCH) ||
           (sk == SK_SPOT) ||
           (sk == SK_CRAFT) ||
           (sk == SK_RIDE); }
          