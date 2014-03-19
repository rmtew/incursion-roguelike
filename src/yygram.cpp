#line 1 "grammar.acc"

/* GRAMMAR.ACC -- Copyright (c) 1999-2003 Julian Mensch

     Contains the BNF+ grammar for the IncursionScript compiler,

	 along with semantic actions that essentially compose the bulk

	 of the compiler. Designed to be fed into the ACCENT compiler

	 compiler system to produce yygram.cpp.

*/

#include "Incursion.h"
#include <setjmp.h>

#define printf T1->Write

#define GETBIND_MEMFUNC(id)         ((BMemFunc*)(theSymTab.GetBinding(id,0,0,MEM_FUNC)))
#define GETBIND_SYSFUNC(id)         ((BSysFunc*)(theSymTab.GetBinding(id,0,0,SYS_FUNC)))
#define GETBIND_MEMVAR(id)          ((BMemVar*)(theSymTab.GetBinding(id,0,1,MEM_VAR)))
#define GETBIND_RESMEM(id)          ((BResMem*)(theSymTab.GetBinding(id,0,1,RES_MEM)))
#define GETBIND_RESFUNC(id)         ((BResFunc*)(theSymTab.GetBinding(id,0,0,RES_FUNC)))
#define GETBIND_GLOBALVAR(id)       ((BGlobalVar*)(theSymTab.GetBinding(id,0,0,GLOB_VAR)))
#define GETBIND_LOCALVAR(id,xID,Ev) ((BLocalVar*)(theSymTab.GetBinding(id,xID,Ev,LOC_VAR)))
#define GETBIND_RESVAR(id,xID)   ((BResVar*)(theSymTab.GetBinding(id,xID,0,RES_VAR)))
#define GETBIND_SYSOBJ(id)          ((BSysObj*)(theSymTab.GetBinding(id,0,SYS_OBJ)))
#define IDENT_DEFINED(id)           (theSymTab.GetBinding(id,0,ANY))

#define pBSysFunc                 BSysFunc*
int16 currMap;
int16 ArtPowerType;
uint32 TempList[2048]; 
int16 TempListLoc;
extern int16 Errors;
extern long yypos;
extern uint32 CompilerFlags, MemFuncID;
extern String Unresolved;

int yyselect(), yymallocerror(), yyparse(),
  yylex();
void get_lexval();

extern uint16 yyMapSize[512];
extern SymbolTable theSymTab;

int32    CurrAttk, CurrFeat;
int file(), resource(), monster_def(), item_def(), map_def(),
  room_def(), feature_def(), quest_def(), spell_def(), mon_entry(),
  mflag_entry(), color(long*), save_entry(), attack_entry(), attrib_entry(),
  arm_type(), item_entry(), arm_entry(), iflag_entry(), eff_entry(),
  feat_entry(), numlist(long*), effect_def(), map_entry(), tile_entry(),
  region_entry(), object_entry(), race_def(), race_entry(), class_def(),
  class_entry(),  abil_level(), dungeon_def(), god_def(), arti_entry(),
  art_power(), dun_entry(), dconst_entry(), list_entry(), artifact_def(), 
  region_def(), domain_def(), domain_entry(), temp_attack_entry(), flavor_def(),
  flavor_entry(),
  terrain_def(), template_def(), domain_entry(), god_entry(), terrain_entry(),
  template_entry(), behaviour_def(), behaviour_entry(), encounter_def(),
  encounter_entry(), encounter_part();
  
int  event_entry(), event_desc(), prototype(), s_prototype(), 
  s_object_decl(), l_declaration(VBlock*), s_declaration(), g_declaration(), 
  r_declaration(), sg_prototype();

int  res_ref(long*),
     abil_param(long*),
     abil_level(long*,long*),
     special_ability(long*,long*,long*,long*,long*);

int cexpr(long*), cexpr2(long*), cexpr3(long*), cexpr4(long*),
    dice_val(Dice*), mval(MVal*);

int expr(PExp*), expr150(PExp*), expr140(PExp*), expr130(PExp*), 
      expr120(PExp*), expr110(PExp*), expr100(PExp*), expr90(PExp*),
      expr80(PExp*), expr70(PExp*), expr60(PExp*), expr50(PExp*),
      expr40(PExp*), expr30(PExp*), expr20(PExp*), expr10(PExp*),
      fund_expr(PExp*), lvalue(PExp*);
int block(VBlock*), statement(VBlock*), routine(VBlock*),
      param_list(BSysFunc*,long,VBlock*,int*), sep_expr(hCode *hc),
      sep_stat(hCode *hc);

extern jmp_buf jb;
extern TMonster * theMon, * firstMon;
extern TItem * theItem, * firstItem;
extern TFeature * theFeat, * firstFeat;
extern TEffect * theEff, * firstEff;
extern TQuest * theQuest, * firstQuest;
extern TArtifact *theArti, *firstArti;
extern TDungeon *theDgn, *firstDgn;
extern TRoutine *theRou, *firstRou;
extern TNPC *theNPC, *firstNPC;
extern TClass *theClass, *firstClass;
extern TRace *theRace, *firstRace;
extern TDomain *theDom, *firstDom;
extern TGod *theGod, *firstGod;
extern TRegion *theReg, *firstReg;
extern TTerrain *theTer, *firstTer;
extern TText *theTxt, *firstTxt;
extern TVariable *theVar, *firstVar;
extern TTemplate *theTemp, *firstTemp;
extern TFlavor *theFlavor, *firstFlavor;
extern TBehaviour *theBev, *firstBev;
extern TEncounter *theEnc, *firstEnc;
extern TText *theText, *firstText;
extern Module * theModule;
extern Resource *theRes;

extern char preprocFilename[];

extern LocationInfo __Tiles__[];

extern LocationInfo *firstTile, *theTile;

int32 theValsIdx;
#define ESTABLISH_VALS                          \
  EffectValues * theVals = (theValsIdx == 0) ?  \
    (&theEff->ef) : &(theEff->Annot(theValsIdx)->u.ef);  \

int32 theEvent = 0;
int16 currPart = 0;
int16 cText = 0;

EffectValues * ChillShieldSeg3 = NULL;

EffectValues ValArray[32];

#pragma warning (disable : 4508)

int yyerror(const char*msg)
{
   Errors++;
   T1->Color(RED);
   T1->CWrite(Format("\"%s\" (%d): %s\n", preprocFilename, yypos, msg));
   longjmp(jb,1);
}

void yywarning(const char*msg)
{
   T1->Color(GREY);
   T1->CWrite(Format("\"%s\" (%d): %s\n", preprocFilename,  yypos, msg));
}


extern "C" int yywrap()
{
   return 1;
}



# line 154 "yygram.cpp"
#include "yygram.h"

int YYSTART ()
{
   switch(yyselect()) {
   case 474: {
      file();
      get_lexval();
      } break;
   }
return 0;
}

int file ()
{
   switch(yyselect()) {
   case 1: {
#line 195 "grammar.acc"
 currMap = 0; MemFuncID = 0; MemVarID = 0; 
# line 173 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 475: {
            resource();
            } break;
         default: goto yy1;
         }
      }
      yy1: ;
      } break;
   }
return 0;
}

int resource ()
{
   YYSTYPE name;
   YYSTYPE n;
   YYSTYPE fname;
   YYSTYPE nam;
   YYSTYPE text;
   switch(yyselect()) {
   case 2: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
#line 199 "grammar.acc"
 theModule->Name = name; 
# line 202 "yygram.cpp"
      } break;
   case 3: {
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 200 "grammar.acc"
 theModule->Slot = n; 
# line 210 "yygram.cpp"
      } break;
   case 4: {
      get_lexval();
      get_lexval();
      fname = yylval;
      get_lexval();
#line 201 "grammar.acc"
 theModule->FName = fname; 
# line 219 "yygram.cpp"
      } break;
   case 5: {
      monster_def();
      } break;
   case 6: {
      item_def();
      } break;
   case 7: {
      feature_def();
      } break;
   case 8: {
      artifact_def();
      } break;
   case 9: {
      dungeon_def();
      } break;
   case 10: {
      quest_def();
      } break;
   case 11: {
      effect_def();
      } break;
   case 12: {
      race_def();
      } break;
   case 13: {
      class_def();
      } break;
   case 14: {
      god_def();
      } break;
   case 15: {
      domain_def();
      } break;
   case 16: {
      region_def();
      } break;
   case 17: {
      terrain_def();
      } break;
   case 18: {
      template_def();
      } break;
   case 19: {
      flavor_def();
      } break;
   case 20: {
      behaviour_def();
      } break;
   case 21: {
      encounter_def();
      } break;
   case 22: {
      prototype();
      } break;
   case 23: {
      s_prototype();
      } break;
   case 24: {
      sg_prototype();
      } break;
   case 25: {
      g_declaration();
      } break;
   case 26: {
      s_declaration();
      } break;
   case 27: {
      s_object_decl();
      } break;
   case 28: {
      get_lexval();
      switch (yyselect()) {
      case 476: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      cexpr(&n);
#line 225 "grammar.acc"
 CompilerFlags |= n; 
# line 301 "yygram.cpp"
      } break;
   case 29: {
      get_lexval();
      get_lexval();
      nam = yylval;
      get_lexval();
      switch (yyselect()) {
      case 477: {
         get_lexval();
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      text = yylval;
      switch (yyselect()) {
      case 478: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 227 "grammar.acc"
 theRes = theTxt; theTxt->Name = nam; theTxt->Desc = text; theTxt++; 
# line 326 "yygram.cpp"
      } break;
   }
return 0;
}

int res_ref (   rID *res)
{
   YYSTYPE lit;
   YYSTYPE n;
   switch(yyselect()) {
   case 30: {
      get_lexval();
      get_lexval();
      lit = yylval;
#line 231 "grammar.acc"
 PurgeStrings();
                         *res = FIND(theModule->GetText(lit)); 
                         if (!*res) {
                           if (Unresolved.GetLength())
                             Unresolved += ", ";
                           Unresolved += Capitalize(theModule->GetText(lit),true);
                           *res = FIND("unimplemented");
                           }
                        
# line 350 "yygram.cpp"
      } break;
   case 31: {
      get_lexval();
      cexpr3(&n);
#line 240 "grammar.acc"
 *res = n; 
# line 357 "yygram.cpp"
      } break;
   }
return 0;
}

int cexpr (   YYSTYPE *val)
{
   YYSTYPE x;
   YYSTYPE y;
   YYSTYPE val2;
   switch(yyselect()) {
   case 32: {
      cexpr(&x);
      get_lexval();
      cexpr2(&y);
#line 244 "grammar.acc"
 *val = x+y; 
# line 374 "yygram.cpp"
      } break;
   case 33: {
      cexpr(&x);
      get_lexval();
      cexpr2(&y);
#line 245 "grammar.acc"
 *val = x-y; 
# line 382 "yygram.cpp"
      } break;
   case 34: {
      cexpr2(&val2);
#line 246 "grammar.acc"
 *val = val2; 
# line 388 "yygram.cpp"
      } break;
   }
return 0;
}

int cexpr2 (   YYSTYPE *val)
{
   YYSTYPE x;
   YYSTYPE y;
   YYSTYPE val2;
   switch(yyselect()) {
   case 35: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 250 "grammar.acc"
 *val = x * y; 
# line 405 "yygram.cpp"
      } break;
   case 36: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 251 "grammar.acc"
 *val = x / y; 
# line 413 "yygram.cpp"
      } break;
   case 37: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 252 "grammar.acc"
 *val = x % y; 
# line 421 "yygram.cpp"
      } break;
   case 38: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 253 "grammar.acc"
 *val = x | y; 
# line 429 "yygram.cpp"
      } break;
   case 39: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 254 "grammar.acc"
 *val = x & y; 
# line 437 "yygram.cpp"
      } break;
   case 40: {
      cexpr3(&val2);
#line 256 "grammar.acc"
 *val = val2; 
# line 443 "yygram.cpp"
      } break;
   }
return 0;
}

int cexpr3 (   YYSTYPE *val)
{
   YYSTYPE val2;
   YYSTYPE n;
   YYSTYPE val3;
   switch(yyselect()) {
   case 41: {
      get_lexval();
      cexpr4(&val2);
#line 260 "grammar.acc"
 *val = val2; 
# line 459 "yygram.cpp"
      } break;
   case 42: {
      get_lexval();
      cexpr4(&n);
#line 261 "grammar.acc"
 *val = -n; 
# line 466 "yygram.cpp"
      } break;
   case 43: {
      cexpr4(&val3);
#line 262 "grammar.acc"
 *val = val3; 
# line 472 "yygram.cpp"
      } break;
   }
return 0;
}

int cexpr4 (   YYSTYPE *val)
{
   YYSTYPE val2;
   YYSTYPE val3;
   YYSTYPE val4;
   switch(yyselect()) {
   case 44: {
      get_lexval();
      cexpr(&val2);
      get_lexval();
#line 266 "grammar.acc"
 * val = val2; 
# line 489 "yygram.cpp"
      } break;
   case 45: {
      get_lexval();
      val3 = yylval;
#line 267 "grammar.acc"
 *val = val3; 
# line 496 "yygram.cpp"
      } break;
   case 46: {
      get_lexval();
#line 268 "grammar.acc"
 *val = 1; 
# line 502 "yygram.cpp"
      } break;
   case 47: {
      get_lexval();
#line 269 "grammar.acc"
 *val = 0; 
# line 508 "yygram.cpp"
      } break;
   case 48: {
      get_lexval();
      val4 = yylval;
#line 270 "grammar.acc"
 * val = val4; 
# line 515 "yygram.cpp"
      } break;
   }
return 0;
}

int dice_val (   Dice *d)
{
   YYSTYPE num;
   YYSTYPE sid;
   YYSTYPE bon;
   YYSTYPE x;
   switch(yyselect()) {
   case 49: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
      get_lexval();
      cexpr4(&bon);
#line 275 "grammar.acc"
 d->Number = num; d->Sides = sid; d->Bonus = bon; 
# line 535 "yygram.cpp"
      } break;
   case 50: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
      get_lexval();
      get_lexval();
      cexpr4(&bon);
#line 277 "grammar.acc"
 d->Number = num; d->Sides = sid; d->Bonus = -bon; 
# line 546 "yygram.cpp"
      } break;
   case 51: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
#line 279 "grammar.acc"
 d->Number = num; d->Sides = sid; d->Bonus = 0; 
# line 554 "yygram.cpp"
      } break;
   case 52: {
      cexpr3(&x);
#line 281 "grammar.acc"
 d->Number = 0; d->Sides = 0; d->Bonus = x; 
# line 560 "yygram.cpp"
      } break;
   }
return 0;
}

int dice_only (   Dice *d)
{
   YYSTYPE num;
   YYSTYPE sid;
   YYSTYPE bon;
   switch(yyselect()) {
   case 53: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
      get_lexval();
      cexpr4(&bon);
#line 286 "grammar.acc"
 d->Number = num; d->Sides = sid; d->Bonus = bon; 
# line 579 "yygram.cpp"
      } break;
   case 54: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
#line 288 "grammar.acc"
 d->Number = num; d->Sides = sid; d->Bonus = 0; 
# line 587 "yygram.cpp"
      } break;
   }
return 0;
}

int gear_entry (   int16 *chance,    Dice *amt,    rID *iID,    rID *eID,    uint16 *spec,    uint32 *qual,    uint32 *fl,    hCode *cond)
{
   hCode hc;
   YYSTYPE n;
   Dice dv;
   rID rr;
   YYSTYPE wq;
   YYSTYPE a;
   YYSTYPE b;
   YYSTYPE x;
   YYSTYPE y;
   switch(yyselect()) {
   case 55: {
#line 293 "grammar.acc"
 *chance = *iID = *eID = *spec = *qual = *fl = *cond = 0; 
    int16 q; q=0;
    amt->Set(1,1,0); 
# line 609 "yygram.cpp"
      switch (yyselect()) {
      case 479: {
         get_lexval();
         get_lexval();
         sep_expr(&hc);
         get_lexval();
#line 296 "grammar.acc"
 *cond = hc; 
# line 618 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 480: {
         get_lexval();
         n = yylval;
         get_lexval();
#line 297 "grammar.acc"
 *chance = n; 
# line 629 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 481: {
         dice_only(&dv);
#line 298 "grammar.acc"
 *amt = yyDice(dv); 
# line 638 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 482: {
         get_lexval();
#line 299 "grammar.acc"
 *fl |= GF_CURSED; 
# line 647 "yygram.cpp"
         } break;
      case 483: {
         get_lexval();
#line 300 "grammar.acc"
 *fl |= GF_BLESSED; 
# line 653 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 484: {
         res_ref(&rr);
#line 301 "grammar.acc"
 *iID = rr; 
# line 662 "yygram.cpp"
         } break;
      case 485: {
         get_lexval();
         cexpr3(&n);
#line 302 "grammar.acc"
 *iID = n; *fl |= GF_RANDOM_ITEM; 
# line 669 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 486: {
         get_lexval();
         get_lexval();
         n = yylval;
#line 303 "grammar.acc"
 *spec = n; 
# line 680 "yygram.cpp"
         } break;
      case 487: {
         get_lexval();
         get_lexval();
         n = yylval;
#line 303 "grammar.acc"
 *spec = -n; 
# line 688 "yygram.cpp"
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 490: {
            get_lexval();
            get_lexval();
            while (1) {
               switch (yyselect()) {
               case 489: {
                  cexpr(&wq);
                  switch (yyselect()) {
                  case 488: {
                     get_lexval();
                     } break;
                  default: ;
                  }
#line 304 "grammar.acc"
 if (q >= 4)
                                   yyerror("Max 4 qualities in gear lists!");
                                else
                                  ((uint8*)qual)[q++] = wq;
# line 712 "yygram.cpp"
                  } break;
               default: goto yy3;
               }
            }
            yy3: ;
            get_lexval();
            } break;
         case 491: {
            get_lexval();
            res_ref(&rr);
#line 308 "grammar.acc"
 *eID = rr; 
# line 725 "yygram.cpp"
            } break;
         case 492: {
            get_lexval();
            get_lexval();
            cexpr3(&a);
            get_lexval();
            cexpr3(&b);
#line 310 "grammar.acc"
 *eID = a + b*256; *fl |= GF_RANDOM_EFFECT; 
# line 735 "yygram.cpp"
            } break;
         case 493: {
            get_lexval();
            get_lexval();
            cexpr3(&b);
#line 312 "grammar.acc"
 *eID = b*256; *fl |= GF_RANDOM_EFFECT; 
# line 743 "yygram.cpp"
            } break;
         case 494: {
            get_lexval();
            get_lexval();
#line 314 "grammar.acc"
 *eID = 0; *fl |= GF_RANDOM_EFFECT; 
# line 750 "yygram.cpp"
            } break;
         case 495: {
            get_lexval();
            get_lexval();
            cexpr3(&x);
            get_lexval();
            cexpr3(&y);
            get_lexval();
            } break;
         default: goto yy2;
         }
      }
      yy2: ;
      } break;
   }
return 0;
}

int gear_desc ()
{
   int16 chance;
   Dice amt;
   rID iID;
   rID eID;
   uint16 spec;
   uint32 qual;
   uint32 fl;
   hCode cond;
   switch(yyselect()) {
   case 56: {
#line 320 "grammar.acc"
 int16 is_else = 0; 
# line 782 "yygram.cpp"
      get_lexval();
      switch (yyselect()) {
      case 496: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      while (1) {
         switch (yyselect()) {
         case 499: {
            gear_entry(&chance, &amt, &iID, &eID, &spec, &qual, &fl, &cond);
#line 323 "grammar.acc"
 theRes->AddEquip(chance,amt,iID,eID,spec,(uint8*)(&qual),fl | is_else,cond); 
# line 796 "yygram.cpp"
            switch (yyselect()) {
            case 497: {
               get_lexval();
#line 324 "grammar.acc"
 is_else = 0; 
# line 802 "yygram.cpp"
               } break;
            case 498: {
               get_lexval();
#line 324 "grammar.acc"
 is_else = GF_ELSE; 
# line 808 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy4;
         }
      }
      yy4: ;
      gear_entry(&chance, &amt, &iID, &eID, &spec, &qual, &fl, &cond);
#line 326 "grammar.acc"
 theRes->AddEquip(chance,amt,iID,eID,spec,(uint8*)(&qual),fl | is_else,cond); 
# line 820 "yygram.cpp"
      get_lexval();
      } break;
   }
return 0;
}

int glyph_entry (   uint16 *img)
{
   YYSTYPE col;
   YYSTYPE n;
   YYSTYPE back;
   switch(yyselect()) {
   case 57: {
      switch (yyselect()) {
      case 500: {
         get_lexval();
         } break;
      case 501: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 502: {
         get_lexval();
         } break;
      default: ;
      }
#line 330 "grammar.acc"
 *img = 15 << 8; 
# line 850 "yygram.cpp"
      switch (yyselect()) {
      case 503: {
         color(&col);
#line 331 "grammar.acc"
 *img = col << 8; 
# line 856 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 504: {
         get_lexval();
         n = yylval;
#line 332 "grammar.acc"
 *img |= n; 
# line 866 "yygram.cpp"
         } break;
      case 505: {
         get_lexval();
         n = yylval;
#line 333 "grammar.acc"
 *img |= n; 
# line 873 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 506: {
         get_lexval();
         color(&back);
#line 335 "grammar.acc"
 *img |= back << 12; 
# line 883 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      } break;
   }
return 0;
}

int glyph_desc (   uint16 *img)
{
   YYSTYPE col;
   YYSTYPE n;
   YYSTYPE back;
   switch(yyselect()) {
   case 58: {
#line 339 "grammar.acc"
 *img = 15 << 8; 
# line 901 "yygram.cpp"
      switch (yyselect()) {
      case 507: {
         color(&col);
#line 340 "grammar.acc"
 *img = col << 8; 
# line 907 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 508: {
         get_lexval();
         n = yylval;
#line 341 "grammar.acc"
 *img |= n; 
# line 917 "yygram.cpp"
         } break;
      case 509: {
         get_lexval();
         n = yylval;
#line 342 "grammar.acc"
 *img |= n; 
# line 924 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 510: {
         get_lexval();
         color(&back);
#line 343 "grammar.acc"
 *img |= back << 12; 
# line 934 "yygram.cpp"
         } break;
      default: ;
      }
      } break;
   }
return 0;
}

int event_desc ()
{
   switch(yyselect()) {
   case 59: {
      get_lexval();
      get_lexval();
      event_entry();
      while (1) {
         switch (yyselect()) {
         case 511: {
            get_lexval();
            event_entry();
            } break;
         default: goto yy5;
         }
      }
      yy5: ;
      get_lexval();
      } break;
   }
return 0;
}

int event_entry ()
{
   YYSTYPE n;
   YYSTYPE lv;
   VBlock vc;
   switch(yyselect()) {
   case 60: {
#line 351 "grammar.acc"
 int16 EvList[32]; int16 i, cEvent = 0; hCode hc; 
# line 973 "yygram.cpp"
      cexpr(&n);
#line 352 "grammar.acc"
 EvList[cEvent++] = n; theEvent = n; 
# line 977 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 512: {
            get_lexval();
            cexpr(&n);
#line 353 "grammar.acc"
 EvList[cEvent++] = n; 
# line 985 "yygram.cpp"
            } break;
         default: goto yy6;
         }
      }
      yy6: ;
      switch (yyselect()) {
      case 514: {
         get_lexval();
         lv = yylval;
#line 354 "grammar.acc"
 for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(-EvList[i],lv); 
# line 998 "yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 513: {
               get_lexval();
               get_lexval();
               lv = yylval;
#line 356 "grammar.acc"
 for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(-EvList[i],lv); 
# line 1008 "yygram.cpp"
               } break;
            default: goto yy7;
            }
         }
         yy7: ;
         } break;
      case 515: {
         routine(&vc);
#line 361 "grammar.acc"
 hCode hc = theCodeSeg.Add(&vc);
                            theCodeSeg.Generate(HALT);
                            for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(EvList[i],hc);
                            theEvent = 0; 
# line 1023 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      } break;
   }
return 0;
}

int sep_expr (   hCode *hc)
{
   PExp ex;
   switch(yyselect()) {
   case 61: {
#line 370 "grammar.acc"
 theEvent = EV_CONDITION; 
# line 1038 "yygram.cpp"
      expr(&ex);
#line 372 "grammar.acc"
 *hc = theCodeSeg.Add(ex.Code);
      theCodeSeg.Generate(RET,ex.Storage,ex.Value); 
# line 1043 "yygram.cpp"
#line 374 "grammar.acc"
 theEvent = 0; 
# line 1046 "yygram.cpp"
      } break;
   }
return 0;
}

int sep_stat (   hCode *hc)
{
   VBlock st;
   switch(yyselect()) {
   case 62: {
#line 378 "grammar.acc"
 theEvent = EV_CONDITION; 
# line 1058 "yygram.cpp"
      statement(&st);
#line 380 "grammar.acc"
 *hc = theCodeSeg.Add(&st);
      theCodeSeg.Generate(RET,0,0); 
# line 1063 "yygram.cpp"
#line 382 "grammar.acc"
 theEvent = 0; 
# line 1066 "yygram.cpp"
      } break;
   }
return 0;
}

int mval (   MVal *mv)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 63: {
#line 386 "grammar.acc"
 mv->BType = mv->Bound = 0; 
# line 1078 "yygram.cpp"
      switch (yyselect()) {
      case 517: {
         switch (yyselect()) {
         case 516: {
            get_lexval();
            } break;
         default: ;
         }
         cexpr4(&n);
#line 387 "grammar.acc"
 mv->Value = n;  mv->VType = MVAL_ADD; 
# line 1090 "yygram.cpp"
         } break;
      case 518: {
         get_lexval();
         cexpr4(&n);
#line 388 "grammar.acc"
 mv->Value = -n; mv->VType = MVAL_ADD; 
# line 1097 "yygram.cpp"
         } break;
      case 519: {
         get_lexval();
         cexpr4(&n);
#line 389 "grammar.acc"
 mv->Value = n;  mv->VType = MVAL_SET; 
# line 1104 "yygram.cpp"
         } break;
      case 520: {
         cexpr3(&n);
         get_lexval();
#line 390 "grammar.acc"
 mv->Value = n;  mv->VType = MVAL_PERCENT; 
# line 1111 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 524: {
         get_lexval();
         switch (yyselect()) {
         case 521: {
            get_lexval();
            cexpr4(&n);
#line 392 "grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_MIN; 
# line 1124 "yygram.cpp"
            } break;
         case 522: {
            get_lexval();
            cexpr4(&n);
#line 393 "grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_MAX; 
# line 1131 "yygram.cpp"
            } break;
         case 523: {
            get_lexval();
            cexpr4(&n);
#line 394 "grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_NEAR; 
# line 1138 "yygram.cpp"
            } break;
         default: printf("???\n"); exit(1);
         }
         get_lexval();
         } break;
      default: ;
      }
      } break;
   }
return 0;
}

int monster_def ()
{
   YYSTYPE name;
   YYSTYPE n;
   YYSTYPE n2;
   YYSTYPE n3;
   switch(yyselect()) {
   case 64: {
      get_lexval();
#line 399 "grammar.acc"
 theMon->Attr[0] = theMon->Attr[1] =
                       theMon->Attr[2] = theMon->Attr[3] =
                         theMon->Attr[4] = theMon->Attr[5] =
                         0; CurrAttk = 0; CurrFeat = 0; theRes = theMon; 
# line 1164 "yygram.cpp"
      get_lexval();
      name = yylval;
#line 403 "grammar.acc"
 theMon->Name = name; 
# line 1169 "yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 404 "grammar.acc"
 theMon->MType[0] = n; 
# line 1174 "yygram.cpp"
      switch (yyselect()) {
      case 526: {
         get_lexval();
         cexpr3(&n2);
#line 405 "grammar.acc"
 theMon->MType[1] = n2; 
# line 1181 "yygram.cpp"
         switch (yyselect()) {
         case 525: {
            get_lexval();
            cexpr3(&n3);
#line 406 "grammar.acc"
 theMon->MType[2] = n3; 
# line 1188 "yygram.cpp"
            } break;
         default: ;
         }
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 527: {
            mon_entry();
            } break;
         default: goto yy8;
         }
      }
      yy8: ;
      get_lexval();
#line 408 "grammar.acc"
 theMon++; 
# line 1208 "yygram.cpp"
      } break;
   }
return 0;
}

int mon_entry ()
{
   YYSTYPE lv;
   YYSTYPE n;
   YYSTYPE n2;
   YYSTYPE n1;
   uint16 img;
   YYSTYPE st;
   YYSTYPE stV;
   YYSTYPE stM;
   rID rr;
   YYSTYPE res;
   YYSTYPE imm;
   YYSTYPE l;
   switch(yyselect()) {
   case 65: {
      get_lexval();
      switch (yyselect()) {
      case 528: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 411 "grammar.acc"
 theMon->Desc = lv; 
# line 1241 "yygram.cpp"
      } break;
   case 66: {
      get_lexval();
      switch (yyselect()) {
      case 529: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 412 "grammar.acc"
 theMon->Depth = n; 
# line 1255 "yygram.cpp"
      } break;
   case 67: {
      switch (yyselect()) {
      case 530: {
         get_lexval();
         } break;
      case 531: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 532: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 413 "grammar.acc"
 theMon->CR = n; 
# line 1277 "yygram.cpp"
      } break;
   case 68: {
      switch (yyselect()) {
      case 533: {
         get_lexval();
         } break;
      case 534: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 535: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n2);
      get_lexval();
      cexpr3(&n1);
      get_lexval();
#line 415 "grammar.acc"
 if (n2 != 1 || n1 > 8 || n1 < 2)
                                        yyerror("Fractional CR error!");
                                      theMon->CR = (-n1) + 2; 
# line 1303 "yygram.cpp"
      } break;
   case 69: {
      get_lexval();
      switch (yyselect()) {
      case 536: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
#line 418 "grammar.acc"
 theMon->Terrains |= n; 
# line 1316 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 537: {
            get_lexval();
            cexpr(&n);
#line 419 "grammar.acc"
 theMon->Terrains |= n; 
# line 1324 "yygram.cpp"
            } break;
         default: goto yy9;
         }
      }
      yy9: ;
      get_lexval();
      } break;
   case 70: {
      get_lexval();
      switch (yyselect()) {
      case 538: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 420 "grammar.acc"
 theMon->HitDice = n; 
# line 1344 "yygram.cpp"
      } break;
   case 71: {
      get_lexval();
      switch (yyselect()) {
      case 539: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 421 "grammar.acc"
 theMon->HitDice = n; 
                                       theMon->SetFlag(M_FIXED_HP); 
# line 1359 "yygram.cpp"
      } break;
   case 72: {
      switch (yyselect()) {
      case 540: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      switch (yyselect()) {
      case 541: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 423 "grammar.acc"
 theMon->Hit = n; 
# line 1379 "yygram.cpp"
      } break;
   case 73: {
      glyph_entry(&img);
#line 424 "grammar.acc"
 theMon->Image = img; 
# line 1385 "yygram.cpp"
      } break;
   case 74: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      get_lexval();
#line 426 "grammar.acc"
 if (!theMon->GainStati(st)) yyerror("too many Stati in monster"); 
# line 1395 "yygram.cpp"
      } break;
   case 75: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      get_lexval();
#line 428 "grammar.acc"
 if (!theMon->GainStati(st,stV)) yyerror("too many Stati in monster"); 
# line 1407 "yygram.cpp"
      } break;
   case 76: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      cexpr3(&stM);
      get_lexval();
      get_lexval();
#line 430 "grammar.acc"
 if (!theMon->GainStati(st,stV,stM)) 
        yyerror("too many Stati in monster"); 
# line 1422 "yygram.cpp"
      } break;
   case 77: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      cexpr3(&stM);
      get_lexval();
      res_ref(&rr);
      get_lexval();
      get_lexval();
#line 433 "grammar.acc"
 if (!theMon->GainStati(st,stV,stM,rr)) 
        yyerror("too many Stati in monster"); 
# line 1439 "yygram.cpp"
      } break;
   case 78: {
      get_lexval();
      switch (yyselect()) {
      case 542: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 544: {
            cexpr3(&res);
#line 436 "grammar.acc"
 theMon->Res |= res; 
# line 1455 "yygram.cpp"
            switch (yyselect()) {
            case 543: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy10;
         }
      }
      yy10: ;
      get_lexval();
      } break;
   case 79: {
      get_lexval();
      switch (yyselect()) {
      case 545: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 547: {
            cexpr3(&imm);
#line 437 "grammar.acc"
 theMon->Imm |= imm; 
# line 1483 "yygram.cpp"
            switch (yyselect()) {
            case 546: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy11;
         }
      }
      yy11: ;
      get_lexval();
      } break;
   case 80: {
      get_lexval();
      switch (yyselect()) {
      case 548: {
         get_lexval();
         } break;
      default: ;
      }
      attack_entry();
      while (1) {
         switch (yyselect()) {
         case 549: {
            get_lexval();
            attack_entry();
            } break;
         default: goto yy12;
         }
      }
      yy12: ;
      get_lexval();
      } break;
   case 81: {
      get_lexval();
      get_lexval();
      l = yylval;
      get_lexval();
#line 439 "grammar.acc"
 for(TMonster*m=firstMon;m!=theMon;m++)
                                         if(stricmp(theModule->GetText(l),theModule->GetText(m->Name))==0)
                                           { memcpy(theMon,m,sizeof(TMonster));
                                             goto DoneCopy; }
                                       Fatal("Monster '%s' not found!",theModule->GetText(l));
                                       DoneCopy:; 
# line 1530 "yygram.cpp"
      } break;
   case 82: {
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 445 "grammar.acc"
 if (n > (theMon - firstMon) || n < 0)
                                         Fatal("Bad number in Monster:as!");
                                       memcpy(theMon,&(firstMon[n]),sizeof(TMonster)); 
# line 1540 "yygram.cpp"
      } break;
   case 83: {
      get_lexval();
      switch (yyselect()) {
      case 550: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 448 "grammar.acc"
 theMon->Def = n; 
# line 1554 "yygram.cpp"
      } break;
   case 84: {
      get_lexval();
      switch (yyselect()) {
      case 551: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 449 "grammar.acc"
 theMon->Arm = n; 
# line 1568 "yygram.cpp"
      } break;
   case 85: {
      get_lexval();
      switch (yyselect()) {
      case 552: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 450 "grammar.acc"
 theMon->Mana = n; 
# line 1582 "yygram.cpp"
      } break;
   case 86: {
      get_lexval();
      switch (yyselect()) {
      case 553: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 554: {
            cexpr3(&n);
            get_lexval();
#line 451 "grammar.acc"
 theMon->Feats[CurrFeat++] = n; 
    if (CurrFeat > 16) Fatal("Too many feats for '%s' (max 16)",
                                theModule->QTextSeg + theMon->Name);
    
# line 1602 "yygram.cpp"
            } break;
         default: goto yy13;
         }
      }
      yy13: ;
      cexpr3(&n);
      get_lexval();
#line 455 "grammar.acc"
 theMon->Feats[CurrFeat++] = n; 
    if (CurrFeat > 16) Fatal("Too many feats for '%s' (max 16)",
                                theModule->QTextSeg + theMon->Name);
    
# line 1615 "yygram.cpp"
      } break;
   case 87: {
      get_lexval();
      switch (yyselect()) {
      case 555: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 556: {
            mflag_entry();
            } break;
         default: goto yy14;
         }
      }
      yy14: ;
      get_lexval();
      } break;
   case 88: {
      attrib_entry();
      while (1) {
         switch (yyselect()) {
         case 557: {
            get_lexval();
            attrib_entry();
            } break;
         default: goto yy15;
         }
      }
      yy15: ;
      get_lexval();
      } break;
   case 89: {
      get_lexval();
      switch (yyselect()) {
      case 558: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 461 "grammar.acc"
 theMon->Size = n; 
# line 1662 "yygram.cpp"
      } break;
   case 90: {
      get_lexval();
      switch (yyselect()) {
      case 559: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 560: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 462 "grammar.acc"
 theMon->Spd = (n-100)/5; 
# line 1682 "yygram.cpp"
      } break;
   case 91: {
      get_lexval();
      switch (yyselect()) {
      case 561: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 562: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 463 "grammar.acc"
 theMon->Mov = (n-100)/5; 
# line 1702 "yygram.cpp"
      } break;
   case 92: {
      get_lexval();
      switch (yyselect()) {
      case 563: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 565: {
            res_ref(&rr);
#line 464 "grammar.acc"
 theMon->SetFlag(M_INNATE);
                                       theMon->AddResID(AN_INNATE, rr); 
# line 1719 "yygram.cpp"
            switch (yyselect()) {
            case 564: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy16;
         }
      }
      yy16: ;
      get_lexval();
      } break;
   case 93: {
      get_lexval();
      switch (yyselect()) {
      case 566: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 568: {
            res_ref(&rr);
#line 467 "grammar.acc"
 theMon->AddResID(AN_DISEASE, rr); 
# line 1747 "yygram.cpp"
            switch (yyselect()) {
            case 567: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy17;
         }
      }
      yy17: ;
      get_lexval();
      } break;
   case 94: {
      get_lexval();
      switch (yyselect()) {
      case 569: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 571: {
            res_ref(&rr);
#line 469 "grammar.acc"
 theMon->AddResID(AN_POISON, rr); 
# line 1775 "yygram.cpp"
            switch (yyselect()) {
            case 570: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy18;
         }
      }
      yy18: ;
      get_lexval();
      } break;
   case 95: {
      gear_desc();
      } break;
   case 96: {
      event_desc();
      } break;
   case 97: {
      dconst_entry();
      } break;
   case 98: {
      r_declaration();
      } break;
   }
return 0;
}

int color (   YYSTYPE *col)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 99: {
      get_lexval();
      n = yylval;
#line 478 "grammar.acc"
 *col = n; 
# line 1813 "yygram.cpp"
      } break;
   case 100: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 479 "grammar.acc"
 *col = n | 8; 
# line 1821 "yygram.cpp"
      } break;
   case 101: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 480 "grammar.acc"
 *col = n | 8; 
# line 1829 "yygram.cpp"
      } break;
   case 102: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 481 "grammar.acc"
 *col = n & ~8; 
# line 1837 "yygram.cpp"
      } break;
   }
return 0;
}

int attack_entry ()
{
   YYSTYPE at;
   Dice dam;
   YYSTYPE chance;
   YYSTYPE n;
   YYSTYPE dc;
   Dice dam2;
   YYSTYPE n2;
   rID rr;
   YYSTYPE dt;
   switch(yyselect()) {
   case 103: {
#line 485 "grammar.acc"
 uint8 dt = AD_NORM; dam.Set(0,0,0);
                                 theMon->Attk[CurrAttk].u.a.Dmg.Set(0,0,0); 
# line 1858 "yygram.cpp"
      cexpr3(&at);
      get_lexval();
      switch (yyselect()) {
      case 572: {
         dice_only(&dam);
#line 488 "grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg = yyDice(dam); 
# line 1866 "yygram.cpp"
         } break;
      case 573: {
         cexpr4(&chance);
         get_lexval();
#line 489 "grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg.Bonus = chance; 
# line 1873 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 574: {
         cexpr4(&n);
#line 490 "grammar.acc"
 dt = n; 
# line 1882 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 575: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 579: {
         get_lexval();
         switch (yyselect()) {
         case 576: {
            get_lexval();
            } break;
         case 577: {
            get_lexval();
            } break;
         case 578: {
            get_lexval();
            } break;
         default: printf("???\n"); exit(1);
         }
         cexpr3(&dc);
#line 493 "grammar.acc"
 theMon->Attk[CurrAttk].u.a.DC = dc; 
# line 1910 "yygram.cpp"
         get_lexval();
         } break;
      default: ;
      }
#line 494 "grammar.acc"
 theMon->Attk[CurrAttk].AType = at;
                                 theMon->Attk[CurrAttk].DType = dt;
                                 /*
                                 if (at == A_ABIL)

                                   theMon->Attk[CurrAttk].u.a.DC = max(

                                        theMon->Attk[CurrAttk].u.a.DC, 1);

                                        */
                                 CurrAttk++;
                                 if (CurrAttk >= 32)
                                   yyerror("> 32 attacks on a single monster!");
                               
# line 1930 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 587: {
            get_lexval();
#line 504 "grammar.acc"
 dt = AD_NORM; dam2.Set(0,0,0); theMon->Attk[CurrAttk].u.a.Dmg.Set(0,0,0); 
# line 1937 "yygram.cpp"
            switch (yyselect()) {
            case 580: {
               dice_only(&dam2);
#line 505 "grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg = yyDice(dam2); 
# line 1943 "yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 581: {
               cexpr3(&n2);
#line 506 "grammar.acc"
 dt = n2; 
# line 1952 "yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 582: {
               get_lexval();
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 586: {
               get_lexval();
               switch (yyselect()) {
               case 583: {
                  get_lexval();
                  } break;
               case 584: {
                  get_lexval();
                  } break;
               case 585: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&dc);
#line 508 "grammar.acc"
 theMon->Attk[CurrAttk].u.a.DC = dc; 
# line 1980 "yygram.cpp"
               get_lexval();
               } break;
            default: ;
            }
#line 509 "grammar.acc"
 theMon->Attk[CurrAttk].AType = A_ALSO;
                                 theMon->Attk[CurrAttk].DType = dt;
                                 CurrAttk++;
                                 if (CurrAttk >= 12)
                                   Fatal("> 12 attacks on a single monster!");
                               
# line 1992 "yygram.cpp"
            } break;
         default: goto yy19;
         }
      }
      yy19: ;
      } break;
   case 104: {
      cexpr3(&at);
      get_lexval();
      res_ref(&rr);
#line 516 "grammar.acc"
 theMon->Attk[CurrAttk].AType = at;
                                  theMon->Attk[CurrAttk].u.xID = rr; 
# line 2006 "yygram.cpp"
      switch (yyselect()) {
      case 588: {
         cexpr3(&dt);
#line 518 "grammar.acc"
 theMon->Attk[CurrAttk].DType = dt; 
# line 2012 "yygram.cpp"
         } break;
      default: ;
      }
#line 519 "grammar.acc"

                                 CurrAttk++;
                                 if (CurrAttk >= 32)
                                   yyerror("> 32 attacks on a single monster!");
                               
# line 2022 "yygram.cpp"
      } break;
   }
return 0;
}

int mflag_entry ()
{
   YYSTYPE mf;
   switch(yyselect()) {
   case 105: {
      cexpr3(&mf);
      switch (yyselect()) {
      case 589: {
         get_lexval();
         } break;
      default: ;
      }
#line 527 "grammar.acc"
 theMon->SetFlag(mf); 
# line 2041 "yygram.cpp"
      } break;
   case 106: {
      get_lexval();
      cexpr3(&mf);
      switch (yyselect()) {
      case 590: {
         get_lexval();
         } break;
      default: ;
      }
#line 528 "grammar.acc"
 theMon->UnsetFlag(mf); 
# line 2054 "yygram.cpp"
      } break;
   }
return 0;
}

int attrib_entry ()
{
   YYSTYPE at;
   YYSTYPE v;
   switch(yyselect()) {
   case 107: {
      get_lexval();
      at = yylval;
      switch (yyselect()) {
      case 591: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&v);
#line 532 "grammar.acc"
 theMon->Attr[at] = v; 
# line 2076 "yygram.cpp"
      } break;
   }
return 0;
}

int item_def ()
{
   YYSTYPE name;
   YYSTYPE n;
   switch(yyselect()) {
   case 108: {
      get_lexval();
#line 536 "grammar.acc"
 theRes = theItem; 
                               theItem->u.w.Crit = 2; 
                               theItem->u.w.Threat = 1; 
                               theItem->hp = 15; 
# line 2093 "yygram.cpp"
      get_lexval();
      name = yylval;
#line 540 "grammar.acc"
 theItem->Name = name; 
# line 2098 "yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 541 "grammar.acc"
 theItem->IType = n; 
# line 2103 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 592: {
            item_entry();
            } break;
         default: goto yy20;
         }
      }
      yy20: ;
      get_lexval();
#line 542 "grammar.acc"
 theItem++; 
# line 2117 "yygram.cpp"
      } break;
   }
return 0;
}

int item_entry ()
{
   YYSTYPE lv;
   YYSTYPE n;
   uint16 img;
   Dice dv;
   YYSTYPE m;
   YYSTYPE unit;
   rID rr;
   switch(yyselect()) {
   case 109: {
      get_lexval();
      switch (yyselect()) {
      case 593: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 546 "grammar.acc"
 theItem->Desc = lv; 
# line 2145 "yygram.cpp"
      } break;
   case 110: {
      get_lexval();
      switch (yyselect()) {
      case 594: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 547 "grammar.acc"
 theItem->Level = n; 
# line 2159 "yygram.cpp"
      } break;
   case 111: {
      glyph_entry(&img);
#line 548 "grammar.acc"
 theItem->Image = img; 
# line 2165 "yygram.cpp"
      } break;
   case 112: {
      get_lexval();
      switch (yyselect()) {
      case 595: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&dv);
      get_lexval();
#line 549 "grammar.acc"
 theItem->u.w.SDmg = yyDice(dv); 
# line 2179 "yygram.cpp"
      } break;
   case 113: {
      get_lexval();
      switch (yyselect()) {
      case 596: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&dv);
      get_lexval();
#line 550 "grammar.acc"
 theItem->u.w.LDmg = yyDice(dv); 
# line 2193 "yygram.cpp"
      } break;
   case 114: {
      get_lexval();
      switch (yyselect()) {
      case 597: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 551 "grammar.acc"
 theItem->u.w.Crit = n; 
# line 2207 "yygram.cpp"
      } break;
   case 115: {
      get_lexval();
      switch (yyselect()) {
      case 598: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 552 "grammar.acc"
 theItem->u.w.Threat = n; 
# line 2221 "yygram.cpp"
      } break;
   case 116: {
      get_lexval();
      switch (yyselect()) {
      case 599: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 553 "grammar.acc"
 theItem->u.w.Acc = n; 
# line 2235 "yygram.cpp"
      } break;
   case 117: {
      get_lexval();
      switch (yyselect()) {
      case 600: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 554 "grammar.acc"
 theItem->u.w.Spd = n; 
# line 2249 "yygram.cpp"
      } break;
   case 118: {
      get_lexval();
      switch (yyselect()) {
      case 601: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr4(&n);
      get_lexval();
      get_lexval();
#line 555 "grammar.acc"
 theItem->u.w.Spd = n > 0 ? ((n-100)/5) : n/5; 
# line 2264 "yygram.cpp"
      } break;
   case 119: {
      get_lexval();
      switch (yyselect()) {
      case 602: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      cexpr4(&n);
      get_lexval();
      get_lexval();
#line 556 "grammar.acc"
 theItem->u.w.Spd = n/5; 
# line 2280 "yygram.cpp"
      } break;
   case 120: {
      get_lexval();
      switch (yyselect()) {
      case 603: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 557 "grammar.acc"
 theItem->u.w.ParryMod = n; 
# line 2294 "yygram.cpp"
      } break;
   case 121: {
      get_lexval();
      switch (yyselect()) {
      case 604: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 558 "grammar.acc"
 theItem->u.w.RangeInc = n; 
# line 2308 "yygram.cpp"
      } break;
   case 122: {
      get_lexval();
      switch (yyselect()) {
      case 605: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&m);
      get_lexval();
#line 559 "grammar.acc"
 theItem->Material = m; 
# line 2322 "yygram.cpp"
      } break;
   case 123: {
      get_lexval();
      switch (yyselect()) {
      case 606: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
#line 560 "grammar.acc"
 theItem->Cost = n * 100; 
# line 2335 "yygram.cpp"
      switch (yyselect()) {
      case 607: {
         cexpr3(&unit);
#line 561 "grammar.acc"
 switch (unit)
                                         { case PLATINUM: theItem->Cost *= 5; break;
                                           case GOLD:     break;
                                           case ELECTRUM: theItem->Cost /= 2; break;
                                           case SILVER:   theItem->Cost /= 10; break;
                                           case COPPER:   theItem->Cost /= 100; break;
                                           
                                         }
                                     
# line 2349 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      } break;
   case 124: {
      get_lexval();
      switch (yyselect()) {
      case 608: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 571 "grammar.acc"
 theItem->Weight = n; 
    /* ww: according to the SRD, one inch (thickness) of iron has 30

     * hit point. We assume that the canonical long sword (at 4 lbs = 40

     * weight) is 1/2 inch thick. So a base weight of 80 gets you 30 hit

     * points. */
                               theItem->hp = (n * 30) / 80; 
                               if (theItem->hp < 1) theItem->hp = 1; 
# line 2376 "yygram.cpp"
      } break;
   case 125: {
      get_lexval();
      switch (yyselect()) {
      case 609: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 578 "grammar.acc"
 theItem->Size = n; 
# line 2390 "yygram.cpp"
      } break;
   case 126: {
      get_lexval();
      switch (yyselect()) {
      case 610: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 579 "grammar.acc"
 theItem->Nutrition = n; 
# line 2404 "yygram.cpp"
      } break;
   case 127: {
      get_lexval();
      switch (yyselect()) {
      case 611: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 580 "grammar.acc"
 theItem->u.a.Def = n; 
# line 2418 "yygram.cpp"
      } break;
   case 128: {
      get_lexval();
      switch (yyselect()) {
      case 612: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 581 "grammar.acc"
 theItem->u.a.Cov = n; 
# line 2432 "yygram.cpp"
      } break;
   case 129: {
      get_lexval();
      switch (yyselect()) {
      case 613: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 582 "grammar.acc"
 theItem->u.a.Penalty = n; 
# line 2446 "yygram.cpp"
      } break;
   case 130: {
      get_lexval();
      switch (yyselect()) {
      case 614: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 615: {
            arm_entry();
            } break;
         default: goto yy21;
         }
      }
      yy21: ;
      get_lexval();
      } break;
   case 131: {
      get_lexval();
      switch (yyselect()) {
      case 616: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 584 "grammar.acc"
 theItem->hp = n; 
# line 2479 "yygram.cpp"
      } break;
   case 132: {
      get_lexval();
      switch (yyselect()) {
      case 617: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 619: {
            res_ref(&rr);
#line 585 "grammar.acc"
 theItem->AddResID(AN_SPELLS, rr); 
# line 2495 "yygram.cpp"
            switch (yyselect()) {
            case 618: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy22;
         }
      }
      yy22: ;
      get_lexval();
      } break;
   case 133: {
      get_lexval();
      switch (yyselect()) {
      case 620: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 622: {
            res_ref(&rr);
#line 587 "grammar.acc"
 theItem->AddResID(AN_FIRES, rr); 
# line 2523 "yygram.cpp"
            switch (yyselect()) {
            case 621: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy23;
         }
      }
      yy23: ;
      get_lexval();
      } break;
   case 134: {
      get_lexval();
      switch (yyselect()) {
      case 623: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 624: {
            iflag_entry();
            } break;
         default: goto yy24;
         }
      }
      yy24: ;
      get_lexval();
      } break;
   case 135: {
      get_lexval();
      switch (yyselect()) {
      case 625: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 628: {
            cexpr3(&n);
            switch (yyselect()) {
            case 626: {
               get_lexval();
               } break;
            case 627: {
               get_lexval();
               } break;
            default: ;
            }
#line 590 "grammar.acc"
 theItem->Group |= n; 
# line 2579 "yygram.cpp"
            } break;
         default: goto yy25;
         }
      }
      yy25: ;
      get_lexval();
      } break;
   case 136: {
      get_lexval();
      switch (yyselect()) {
      case 629: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 591 "grammar.acc"
 theItem->u.c.Capacity = n; 
# line 2599 "yygram.cpp"
      } break;
   case 137: {
      get_lexval();
      switch (yyselect()) {
      case 630: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 592 "grammar.acc"
 theItem->u.c.WeightLim = n; 
# line 2613 "yygram.cpp"
      } break;
   case 138: {
      get_lexval();
      switch (yyselect()) {
      case 631: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 593 "grammar.acc"
 theItem->u.c.WeightMod = n; 
# line 2627 "yygram.cpp"
      } break;
   case 139: {
      get_lexval();
      switch (yyselect()) {
      case 632: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 594 "grammar.acc"
 theItem->u.c.MaxSize = n; 
# line 2641 "yygram.cpp"
      } break;
   case 140: {
      get_lexval();
      switch (yyselect()) {
      case 633: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 595 "grammar.acc"
 theItem->u.c.Timeout = n; 
# line 2655 "yygram.cpp"
      } break;
   case 141: {
      get_lexval();
      switch (yyselect()) {
      case 634: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 596 "grammar.acc"
 theItem->u.c.CType = n; 
# line 2669 "yygram.cpp"
      } break;
   case 142: {
      get_lexval();
      get_lexval();
      switch (yyselect()) {
      case 635: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 597 "grammar.acc"
 theItem->u.l.LightRange = n; 
# line 2684 "yygram.cpp"
      } break;
   case 143: {
      get_lexval();
      switch (yyselect()) {
      case 636: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 598 "grammar.acc"
 theItem->u.l.Lifespan = n; 
# line 2698 "yygram.cpp"
      } break;
   case 144: {
      get_lexval();
      switch (yyselect()) {
      case 637: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 599 "grammar.acc"
 theItem->u.l.Fuel = rr; 
# line 2712 "yygram.cpp"
      } break;
   case 145: {
      dconst_entry();
      } break;
   case 146: {
      event_desc();
      } break;
   case 147: {
      r_declaration();
      } break;
   }
return 0;
}

int arm_entry ()
{
   YYSTYPE n;
   YYSTYPE wt;
   switch(yyselect()) {
   case 148: {
      cexpr3(&n);
      switch (yyselect()) {
      case 638: {
         get_lexval();
         } break;
      default: ;
      }
#line 606 "grammar.acc"
 if (theItem->u.a.Arm[0] == 0) theItem->u.a.Arm[0] = n;
                            if (theItem->u.a.Arm[1] == 0) theItem->u.a.Arm[1] = n;
                            if (theItem->u.a.Arm[2] == 0) theItem->u.a.Arm[2] = n;
                         
# line 2744 "yygram.cpp"
      } break;
   case 149: {
      get_lexval();
      wt = yylval;
      cexpr3(&n);
      switch (yyselect()) {
      case 639: {
         get_lexval();
         } break;
      default: ;
      }
#line 610 "grammar.acc"
 theItem->u.a.Arm[wt] = n; 
# line 2758 "yygram.cpp"
      } break;
   }
return 0;
}

int iflag_entry ()
{
   YYSTYPE itf;
   switch(yyselect()) {
   case 150: {
      cexpr3(&itf);
      switch (yyselect()) {
      case 640: {
         get_lexval();
         } break;
      default: ;
      }
#line 614 "grammar.acc"
 theItem->SetFlag(itf); 
# line 2777 "yygram.cpp"
      } break;
   case 151: {
      get_lexval();
      cexpr3(&itf);
      switch (yyselect()) {
      case 641: {
         get_lexval();
         } break;
      default: ;
      }
#line 615 "grammar.acc"
 theItem->UnsetFlag(itf); 
# line 2790 "yygram.cpp"
      } break;
   }
return 0;
}

int feature_def ()
{
   YYSTYPE name;
   YYSTYPE n;
   switch(yyselect()) {
   case 152: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 621 "grammar.acc"
 theRes = theFeat; theFeat->Name = name; theRes = theFeat; 
# line 2806 "yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 622 "grammar.acc"
 theFeat->FType = n; 
# line 2811 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 642: {
            feat_entry();
            } break;
         default: goto yy26;
         }
      }
      yy26: ;
      get_lexval();
#line 623 "grammar.acc"
 theFeat++; 
# line 2825 "yygram.cpp"
      } break;
   }
return 0;
}

int feat_entry ()
{
   YYSTYPE lv;
   YYSTYPE m;
   uint16 img;
   YYSTYPE mov;
   YYSTYPE n;
   rID rr;
   Dice d;
   switch(yyselect()) {
   case 153: {
      get_lexval();
      switch (yyselect()) {
      case 643: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 627 "grammar.acc"
 theFeat->Desc = lv; 
# line 2853 "yygram.cpp"
      } break;
   case 154: {
      get_lexval();
      switch (yyselect()) {
      case 644: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&m);
      get_lexval();
#line 628 "grammar.acc"
 theFeat->Material = m; 
# line 2867 "yygram.cpp"
      } break;
   case 155: {
      glyph_entry(&img);
#line 629 "grammar.acc"
 theFeat->Image = img; 
# line 2873 "yygram.cpp"
      } break;
   case 156: {
      get_lexval();
      switch (yyselect()) {
      case 645: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mov);
      get_lexval();
      get_lexval();
#line 630 "grammar.acc"
 theFeat->MoveMod = (mov-100)/5; 
# line 2888 "yygram.cpp"
      } break;
   case 157: {
      get_lexval();
      switch (yyselect()) {
      case 646: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 631 "grammar.acc"
 theFeat->hp = n; 
# line 2902 "yygram.cpp"
      } break;
   case 158: {
      get_lexval();
      switch (yyselect()) {
      case 647: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 650: {
            cexpr3(&n);
            switch (yyselect()) {
            case 648: {
               get_lexval();
               } break;
            case 649: {
               get_lexval();
               } break;
            default: ;
            }
#line 632 "grammar.acc"
 theFeat->Flags |= n; 
# line 2927 "yygram.cpp"
            } break;
         default: goto yy27;
         }
      }
      yy27: ;
      get_lexval();
      } break;
   case 159: {
      switch (yyselect()) {
      case 651: {
         get_lexval();
         } break;
      case 652: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 653: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 633 "grammar.acc"
 theFeat->xID = rr; 
# line 2955 "yygram.cpp"
      } break;
   case 160: {
      get_lexval();
      switch (yyselect()) {
      case 654: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 634 "grammar.acc"
 theFeat->xval = n; 
# line 2969 "yygram.cpp"
      } break;
   case 161: {
      get_lexval();
      switch (yyselect()) {
      case 655: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&d);
      get_lexval();
#line 635 "grammar.acc"
 theFeat->Factor = d; 
# line 2983 "yygram.cpp"
      } break;
   case 162: {
      event_desc();
      } break;
   case 163: {
      dconst_entry();
      } break;
   case 164: {
      r_declaration();
      } break;
   }
return 0;
}

int effect_def ()
{
   YYSTYPE n;
   YYSTYPE name;
   switch(yyselect()) {
   case 165: {
#line 641 "grammar.acc"
 theRes = theEff; theEff->ef.sval = NOSAVE; 
# line 3005 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 657: {
            cexpr3(&n);
#line 642 "grammar.acc"
 if (theEff->Sources[2])
                                        theEff->Sources[3] = n;
                                      else if (theEff->Sources[1])
                                        theEff->Sources[2] = n;
                                      else if (theEff->Sources[0])
                                        theEff->Sources[1] = n; 
                                      else
                                        theEff->Sources[0] = n; 
# line 3019 "yygram.cpp"
            switch (yyselect()) {
            case 656: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy28;
         }
      }
      yy28: ;
      switch (yyselect()) {
      case 658: {
         get_lexval();
         } break;
      case 659: {
         get_lexval();
         } break;
      case 660: {
         get_lexval();
#line 652 "grammar.acc"
 theEff->ef.aval = AR_POISON; 
# line 3042 "yygram.cpp"
         } break;
      case 661: {
         get_lexval();
#line 653 "grammar.acc"
 theEff->ef.aval = AR_DISEASE; 
# line 3048 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      name = yylval;
#line 654 "grammar.acc"
 theEff->Name = name; 
# line 3056 "yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 655 "grammar.acc"
 theValsIdx = 0; 
                                      theEff->ef.eval = n; 
# line 3062 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 662: {
            eff_entry();
            } break;
         default: goto yy29;
         }
      }
      yy29: ;
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 664: {
            get_lexval();
            cexpr3(&n);
#line 658 "grammar.acc"
  Annotation *a; EffectValues tmp; int16 t;
                               ESTABLISH_VALS
							                 memcpy(&tmp,theVals,sizeof(EffectValues));
                               if (!theEff->AnHead) {
                                 a = theEff->NewAnnot(AN_EFFECT, &theEff->AnHead);
                                 theValsIdx = theEff->AnHead;
                                 ASSERT(theValsIdx > 0);
                                 }
                               else {
                                 a = theEff->Annot(theEff->AnHead);
                                 while (a->Next) 
                                   a = theEff->Annot(a->Next);
                                 a = theEff->NewAnnot(AN_EFFECT,&a->Next,&t);
                                 theValsIdx = t;
                                 ASSERT(theValsIdx > 0);
                                 }
                               a->Next = 0;
                               memcpy(&(a->u.ef),&tmp,sizeof(EffectValues));
                               a->u.ef.eval = n;
                             
# line 3100 "yygram.cpp"
            get_lexval();
            while (1) {
               switch (yyselect()) {
               case 663: {
                  eff_entry();
                  } break;
               default: goto yy31;
               }
            }
            yy31: ;
            get_lexval();
            } break;
         default: goto yy30;
         }
      }
      yy30: ;
#line 680 "grammar.acc"
 theEff++; 
# line 3119 "yygram.cpp"
      } break;
   }
return 0;
}

int eff_entry ()
{
   YYSTYPE lv;
   YYSTYPE sc;
   YYSTYPE n;
   YYSTYPE n2;
   YYSTYPE sv;
   Dice d;
   rID rr;
   switch(yyselect()) {
   case 166: {
      get_lexval();
      switch (yyselect()) {
      case 665: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 684 "grammar.acc"
 theEff->Desc = lv; 
# line 3147 "yygram.cpp"
      } break;
   case 167: {
      cexpr(&sc);
      get_lexval();
#line 685 "grammar.acc"
 theEff->Schools |= sc; 
# line 3154 "yygram.cpp"
      } break;
   case 168: {
      get_lexval();
      switch (yyselect()) {
      case 666: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 686 "grammar.acc"
 theEff->Level = n; 
# line 3168 "yygram.cpp"
      } break;
   case 169: {
      get_lexval();
      switch (yyselect()) {
      case 667: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 687 "grammar.acc"
 theEff->ManaCost = n; 
# line 3182 "yygram.cpp"
      } break;
   case 170: {
      get_lexval();
      switch (yyselect()) {
      case 668: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 669: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 670: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 688 "grammar.acc"
 theEff->BaseChance = n; 
# line 3208 "yygram.cpp"
      } break;
   case 171: {
      get_lexval();
      switch (yyselect()) {
      case 671: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
#line 689 "grammar.acc"
 theEff->Purpose = n; 
# line 3221 "yygram.cpp"
      switch (yyselect()) {
      case 672: {
         get_lexval();
         cexpr(&n2);
         get_lexval();
#line 690 "grammar.acc"
 theEff->Purpose |= n2<<16; 
# line 3229 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      } break;
   case 172: {
      get_lexval();
      switch (yyselect()) {
      case 673: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&sv);
      switch (yyselect()) {
      case 674: {
         get_lexval();
#line 691 "grammar.acc"
 theEff->SetFlag(EF_PARTIAL); 
# line 3249 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
#line 692 "grammar.acc"
 ESTABLISH_VALS; theVals->sval = sv; 
# line 3256 "yygram.cpp"
      } break;
   case 173: {
      get_lexval();
      switch (yyselect()) {
      case 675: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 693 "grammar.acc"
 ESTABLISH_VALS; theVals->qval = n; 
# line 3270 "yygram.cpp"
      } break;
   case 174: {
      get_lexval();
      switch (yyselect()) {
      case 676: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 694 "grammar.acc"
 ESTABLISH_VALS; theVals->xval = n; 
# line 3284 "yygram.cpp"
      } break;
   case 175: {
      get_lexval();
      switch (yyselect()) {
      case 677: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 695 "grammar.acc"
 ESTABLISH_VALS; theVals->yval = n; 
# line 3298 "yygram.cpp"
      } break;
   case 176: {
      get_lexval();
      switch (yyselect()) {
      case 678: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 696 "grammar.acc"
 ESTABLISH_VALS; theVals->dval = n; 
# line 3312 "yygram.cpp"
      } break;
   case 177: {
      get_lexval();
      switch (yyselect()) {
      case 679: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&d);
      get_lexval();
#line 697 "grammar.acc"
 ESTABLISH_VALS; theVals->pval = d; 
# line 3326 "yygram.cpp"
      } break;
   case 178: {
      get_lexval();
      switch (yyselect()) {
      case 680: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 698 "grammar.acc"
 ESTABLISH_VALS; theVals->aval = n; 
# line 3340 "yygram.cpp"
      } break;
   case 179: {
      get_lexval();
      switch (yyselect()) {
      case 681: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 699 "grammar.acc"
 ESTABLISH_VALS; theVals->lval = n; 
# line 3354 "yygram.cpp"
      } break;
   case 180: {
      get_lexval();
      switch (yyselect()) {
      case 682: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 700 "grammar.acc"
 ESTABLISH_VALS; theVals->tval = n; 
# line 3368 "yygram.cpp"
      } break;
   case 181: {
      get_lexval();
      switch (yyselect()) {
      case 683: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 701 "grammar.acc"
 ESTABLISH_VALS; theVals->rval = rr; 
# line 3382 "yygram.cpp"
      } break;
   case 182: {
      get_lexval();
      switch (yyselect()) {
      case 684: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 702 "grammar.acc"
 ESTABLISH_VALS; theVals->rval = n; 
# line 3396 "yygram.cpp"
      } break;
   case 183: {
      get_lexval();
      switch (yyselect()) {
      case 685: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 703 "grammar.acc"
 ESTABLISH_VALS; theVals->cval = n; 
# line 3410 "yygram.cpp"
      } break;
   case 184: {
      get_lexval();
      switch (yyselect()) {
      case 686: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      n = yylval;
      get_lexval();
#line 704 "grammar.acc"
 ESTABLISH_VALS; theVals->cval = n; 
# line 3425 "yygram.cpp"
      } break;
   case 185: {
      get_lexval();
      switch (yyselect()) {
      case 687: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 690: {
            cexpr3(&n);
#line 705 "grammar.acc"
 theEff->SetFlag(n); 
# line 3441 "yygram.cpp"
            switch (yyselect()) {
            case 688: {
               get_lexval();
               } break;
            case 689: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy32;
         }
      }
      yy32: ;
      get_lexval();
      } break;
   case 186: {
      event_desc();
      } break;
   case 187: {
      r_declaration();
      } break;
   case 188: {
      dconst_entry();
      } break;
   }
return 0;
}

int numlist (   YYSTYPE *val)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 189: {
#line 712 "grammar.acc"
 *val = 0; 
# line 3477 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 693: {
            cexpr3(&n);
#line 713 "grammar.acc"
 *val |= n; 
# line 3484 "yygram.cpp"
            switch (yyselect()) {
            case 691: {
               get_lexval();
               } break;
            case 692: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy33;
         }
      }
      yy33: ;
      } break;
   }
return 0;
}

int object_entry ()
{
   switch(yyselect()) {
   case 190: {
      get_lexval();
      } break;
   }
return 0;
}

int race_def ()
{
   YYSTYPE name;
   rID base;
   switch(yyselect()) {
   case 191: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 722 "grammar.acc"
 theRes = theRace; theRace->Name = name;
                                    theRace->BaseRace = 0; 
# line 3524 "yygram.cpp"
      switch (yyselect()) {
      case 694: {
         get_lexval();
         res_ref(&base);
#line 724 "grammar.acc"
 TRace tr; 
                                    theRace->BaseRace = base;                                        
                                    if (TRACE(base) > theRace)
                                      Fatal("Subrace definition before def of base race!"); 
# line 3534 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 695: {
            race_entry();
            } break;
         default: goto yy34;
         }
      }
      yy34: ;
      get_lexval();
#line 729 "grammar.acc"
 theRace++; 
# line 3551 "yygram.cpp"
      } break;
   }
return 0;
}

int race_entry ()
{
   YYSTYPE lv;
   YYSTYPE at;
   YYSTYPE v;
   YYSTYPE AbT;
   YYSTYPE Abil;
   YYSTYPE p;
   YYSTYPE l1;
   YYSTYPE l2;
   rID rr;
   rID rr2;
   rID rr3;
   YYSTYPE sk;
   YYSTYPE n;
   switch(yyselect()) {
   case 192: {
      get_lexval();
      switch (yyselect()) {
      case 696: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 733 "grammar.acc"
 theRace->Desc = lv; 
# line 3585 "yygram.cpp"
      } break;
   case 193: {
      while (1) {
         switch (yyselect()) {
         case 699: {
            get_lexval();
            at = yylval;
            switch (yyselect()) {
            case 697: {
               get_lexval();
               } break;
            default: ;
            }
            cexpr(&v);
#line 734 "grammar.acc"
 theRace->AttrAdj[at] = v; 
# line 3602 "yygram.cpp"
            switch (yyselect()) {
            case 698: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy35;
         }
      }
      yy35: ;
      get_lexval();
      } break;
   case 194: {
      get_lexval();
      switch (yyselect()) {
      case 700: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 702: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 701: {
               get_lexval();
               } break;
            default: ;
            }
#line 737 "grammar.acc"
 theRace->AddAbility(AbT,Abil,p,l1,l2); 
# line 3636 "yygram.cpp"
            } break;
         default: goto yy36;
         }
      }
      yy36: ;
      get_lexval();
      } break;
   case 195: {
      get_lexval();
      switch (yyselect()) {
      case 703: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 738 "grammar.acc"
 theRace->mID = rr; 
# line 3656 "yygram.cpp"
      } break;
   case 196: {
      get_lexval();
      switch (yyselect()) {
      case 704: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 705: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
      res_ref(&rr2);
      get_lexval();
#line 740 "grammar.acc"
 theRace->FavoredClass[0] = rr;
                                  theRace->FavoredClass[1] = rr2; 
# line 3679 "yygram.cpp"
      } break;
   case 197: {
      get_lexval();
      switch (yyselect()) {
      case 706: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 707: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
      res_ref(&rr2);
      get_lexval();
      res_ref(&rr3);
      get_lexval();
#line 743 "grammar.acc"
 theRace->FavoredClass[0] = rr;
                                  theRace->FavoredClass[1] = rr2; 
                                  theRace->FavoredClass[2] = rr3; 
# line 3705 "yygram.cpp"
      } break;
   case 198: {
      get_lexval();
      switch (yyselect()) {
      case 708: {
         get_lexval();
         } break;
      default: ;
      }
#line 746 "grammar.acc"
int8 skn = 0;
# line 3717 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 710: {
            cexpr3(&sk);
#line 747 "grammar.acc"
 theRace->Skills[skn++] = sk; 
# line 3724 "yygram.cpp"
            switch (yyselect()) {
            case 709: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy37;
         }
      }
      yy37: ;
      get_lexval();
      } break;
   case 199: {
      cexpr3(&n);
      switch (yyselect()) {
      case 711: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 748 "grammar.acc"
 switch(n) {
                                         case MALE_NAMES  : theRace-> MNames = lv; break;
                                         case FEMALE_NAMES: theRace-> FNames = lv; break;
                                         case FAMILY_NAMES: theRace-> SNames = lv; break;
                                         default: yyerror("Illegal name const in race!");
                                         } 
# line 3756 "yygram.cpp"
      } break;
   case 200: {
      get_lexval();
      switch (yyselect()) {
      case 712: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 715: {
            cexpr3(&n);
            switch (yyselect()) {
            case 713: {
               get_lexval();
               } break;
            case 714: {
               get_lexval();
               } break;
            default: ;
            }
#line 754 "grammar.acc"
 theRace->SetFlag(n); 
# line 3781 "yygram.cpp"
            } break;
         default: goto yy38;
         }
      }
      yy38: ;
      get_lexval();
      } break;
   case 201: {
      gear_desc();
      } break;
   case 202: {
      dconst_entry();
      } break;
   case 203: {
      event_desc();
      } break;
   case 204: {
      r_declaration();
      } break;
   }
return 0;
}

int class_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 205: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 762 "grammar.acc"
 theRes = theClass; theClass->Name = name; 
# line 3814 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 716: {
            class_entry();
            } break;
         default: goto yy39;
         }
      }
      yy39: ;
      get_lexval();
#line 763 "grammar.acc"
 theClass++; 
# line 3828 "yygram.cpp"
      } break;
   }
return 0;
}

int class_entry ()
{
   YYSTYPE lv;
   YYSTYPE n;
   YYSTYPE x;
   YYSTYPE typ;
   YYSTYPE l;
   YYSTYPE AbT;
   YYSTYPE Abil;
   YYSTYPE p;
   YYSTYPE l1;
   YYSTYPE l2;
   YYSTYPE sp;
   YYSTYPE sk;
   YYSTYPE pr;
   switch(yyselect()) {
   case 206: {
      get_lexval();
      switch (yyselect()) {
      case 717: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 767 "grammar.acc"
 theClass->Desc = lv; 
# line 3862 "yygram.cpp"
      } break;
   case 207: {
      get_lexval();
      switch (yyselect()) {
      case 718: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 719: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 768 "grammar.acc"
 theClass->HitDie = n; 
# line 3882 "yygram.cpp"
      } break;
   case 208: {
      get_lexval();
      switch (yyselect()) {
      case 720: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 721: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 769 "grammar.acc"
 theClass->ManaDie = n; 
# line 3902 "yygram.cpp"
      } break;
   case 209: {
      get_lexval();
      switch (yyselect()) {
      case 722: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&x);
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 770 "grammar.acc"
 theClass->DefMod = n;
                                             if (x != 1) yyerror("class defense mod must by 1 per N."); 
# line 3919 "yygram.cpp"
      } break;
   case 210: {
      get_lexval();
      switch (yyselect()) {
      case 723: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 725: {
            cexpr3(&typ);
            cexpr3(&l);
#line 773 "grammar.acc"
 theClass->AttkVal[typ] = l; 
# line 3936 "yygram.cpp"
            switch (yyselect()) {
            case 724: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy40;
         }
      }
      yy40: ;
      get_lexval();
      } break;
   case 211: {
      get_lexval();
      switch (yyselect()) {
      case 726: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 728: {
            cexpr3(&typ);
            cexpr3(&l);
#line 775 "grammar.acc"
 theClass->Saves[typ] = l; 
# line 3965 "yygram.cpp"
            switch (yyselect()) {
            case 727: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy41;
         }
      }
      yy41: ;
      get_lexval();
      } break;
   case 212: {
      get_lexval();
      switch (yyselect()) {
      case 729: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 731: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 730: {
               get_lexval();
               } break;
            default: ;
            }
#line 777 "grammar.acc"
 theClass->AddAbility(AbT,Abil,p,l1,l2); 
# line 3999 "yygram.cpp"
            } break;
         default: goto yy42;
         }
      }
      yy42: ;
      get_lexval();
      } break;
   case 213: {
      get_lexval();
      switch (yyselect()) {
      case 732: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      cexpr3(&sp);
      switch (yyselect()) {
      case 733: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
#line 778 "grammar.acc"
 theClass->SkillPoints = sp; int8 skn = 0; 
# line 4024 "yygram.cpp"
      switch (yyselect()) {
      case 734: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 736: {
            cexpr3(&sk);
#line 779 "grammar.acc"
 theClass->Skills[skn++] = sk; 
# line 4037 "yygram.cpp"
            switch (yyselect()) {
            case 735: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy43;
         }
      }
      yy43: ;
      get_lexval();
      } break;
   case 214: {
      get_lexval();
      switch (yyselect()) {
      case 737: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 739: {
            cexpr3(&pr);
#line 781 "grammar.acc"
 theClass->Proficiencies |= pr; 
# line 4065 "yygram.cpp"
            switch (yyselect()) {
            case 738: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy44;
         }
      }
      yy44: ;
      get_lexval();
      } break;
   case 215: {
      get_lexval();
      switch (yyselect()) {
      case 740: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 743: {
            cexpr3(&n);
            switch (yyselect()) {
            case 741: {
               get_lexval();
               } break;
            case 742: {
               get_lexval();
               } break;
            default: ;
            }
#line 782 "grammar.acc"
 theClass->SetFlag(n); 
# line 4102 "yygram.cpp"
            } break;
         default: goto yy45;
         }
      }
      yy45: ;
      get_lexval();
      } break;
   case 216: {
      gear_desc();
      } break;
   case 217: {
      event_desc();
      } break;
   case 218: {
      dconst_entry();
      } break;
   case 219: {
      r_declaration();
      } break;
   }
return 0;
}

int special_ability (   YYSTYPE *AbT,    YYSTYPE *Abil,    YYSTYPE *p,    YYSTYPE *l1,    YYSTYPE *l2)
{
   YYSTYPE ca;
   YYSTYPE xp;
   YYSTYPE xl1;
   YYSTYPE xl2;
   YYSTYPE st;
   YYSTYPE ft;
   switch(yyselect()) {
   case 220: {
#line 790 "grammar.acc"
 *p = 0; 
# line 4137 "yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&ca);
      switch (yyselect()) {
      case 744: {
         abil_param(&xp);
#line 790 "grammar.acc"
 *p = xp; 
# line 4146 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 791 "grammar.acc"
 *AbT = AB_ABILITY; *Abil = ca; *l1 =xl1; *l2 = xl2; 
# line 4155 "yygram.cpp"
      } break;
   case 221: {
#line 792 "grammar.acc"
 *p = 0; 
# line 4160 "yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&st);
      switch (yyselect()) {
      case 745: {
         abil_param(&xp);
#line 792 "grammar.acc"
 *p = xp; 
# line 4169 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 793 "grammar.acc"
 *AbT = AB_STATI;   *Abil = st; *l1 = xl1; *l2 = xl2; 
# line 4178 "yygram.cpp"
      } break;
   case 222: {
#line 794 "grammar.acc"
 *p = 0; 
# line 4183 "yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&ft);
      switch (yyselect()) {
      case 746: {
         abil_param(&xp);
#line 794 "grammar.acc"
 *p = xp; 
# line 4192 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 795 "grammar.acc"
 *AbT = AB_FEAT;    *Abil = ft; *l1 = xl1; *l2 = xl2; 
# line 4201 "yygram.cpp"
      } break;
   }
return 0;
}

int abil_param (   YYSTYPE *p)
{
   YYSTYPE pa;
   rID rr;
   YYSTYPE pa2;
   switch(yyselect()) {
   case 223: {
      get_lexval();
      cexpr3(&pa);
#line 799 "grammar.acc"
 *p = pa; 
# line 4217 "yygram.cpp"
      } break;
   case 224: {
      get_lexval();
      res_ref(&rr);
#line 800 "grammar.acc"
 *p = rr; 
# line 4224 "yygram.cpp"
      } break;
   case 225: {
      get_lexval();
      cexpr3(&pa);
      get_lexval();
      cexpr3(&pa2);
#line 801 "grammar.acc"
 *p = pa | (pa2 << 16); 
# line 4233 "yygram.cpp"
      } break;
   }
return 0;
}

int abil_level (   YYSTYPE *l1,    YYSTYPE *l2)
{
   YYSTYPE xl1;
   YYSTYPE xl2;
   switch(yyselect()) {
   case 226: {
      cexpr3(&xl1);
      switch (yyselect()) {
      case 747: {
         get_lexval();
         } break;
      default: ;
      }
#line 806 "grammar.acc"
 *l1 = xl1; *l2 = 0; 
# line 4253 "yygram.cpp"
      } break;
   case 227: {
      get_lexval();
      cexpr3(&xl2);
      get_lexval();
#line 808 "grammar.acc"
 *l1 = 1;  *l2 = xl2; 
# line 4261 "yygram.cpp"
      } break;
   case 228: {
      get_lexval();
      get_lexval();
#line 810 "grammar.acc"
 *l1 = 1;  *l2 = 1; 
# line 4268 "yygram.cpp"
      } break;
   case 229: {
      get_lexval();
      get_lexval();
      get_lexval();
      get_lexval();
      cexpr3(&xl1);
      switch (yyselect()) {
      case 748: {
         get_lexval();
         } break;
      default: ;
      }
#line 812 "grammar.acc"
 *l1 = xl1; *l2 = 1; 
# line 4284 "yygram.cpp"
      } break;
   case 230: {
      get_lexval();
      cexpr3(&xl2);
      get_lexval();
      get_lexval();
      get_lexval();
      cexpr3(&xl1);
      switch (yyselect()) {
      case 749: {
         get_lexval();
         } break;
      default: ;
      }
#line 814 "grammar.acc"
 *l1 = xl1; *l2 = xl2; 
# line 4301 "yygram.cpp"
      } break;
   }
return 0;
}

int dungeon_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 231: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 818 "grammar.acc"
 theRes = theDgn; theDgn->Name = name; 
# line 4316 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 750: {
            dun_entry();
            } break;
         default: goto yy46;
         }
      }
      yy46: ;
      get_lexval();
#line 819 "grammar.acc"
 theDgn++; 
# line 4330 "yygram.cpp"
      } break;
   }
return 0;
}

int dun_entry ()
{
   YYSTYPE lv;
   switch(yyselect()) {
   case 232: {
      get_lexval();
      switch (yyselect()) {
      case 751: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 823 "grammar.acc"
 theDgn->Desc = lv; 
# line 4352 "yygram.cpp"
      } break;
   case 233: {
      dconst_entry();
      } break;
   case 234: {
      event_desc();
      } break;
   case 235: {
      r_declaration();
      } break;
   }
return 0;
}

int dconst_entry ()
{
   YYSTYPE con;
   YYSTYPE val;
   rID rr;
   YYSTYPE lis;
   YYSTYPE n;
   Dice dv;
   switch(yyselect()) {
   case 236: {
      get_lexval();
      switch (yyselect()) {
      case 752: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 754: {
            get_lexval();
            cexpr3(&con);
            cexpr3(&val);
#line 831 "grammar.acc"
 theRes->AddConstant(con,val); 
# line 4391 "yygram.cpp"
            switch (yyselect()) {
            case 753: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         case 756: {
            get_lexval();
            cexpr3(&con);
            res_ref(&rr);
#line 832 "grammar.acc"
 theRes->AddConstant(con,rr); 
# line 4405 "yygram.cpp"
            switch (yyselect()) {
            case 755: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy47;
         }
      }
      yy47: ;
      get_lexval();
      } break;
   case 237: {
      get_lexval();
      switch (yyselect()) {
      case 757: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 761: {
            get_lexval();
            cexpr4(&lis);
#line 835 "grammar.acc"
 TempListLoc = 0; 
# line 4434 "yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 758: {
                  list_entry();
                  } break;
               default: goto yy49;
               }
            }
            yy49: ;
            switch (yyselect()) {
            case 759: {
               get_lexval();
               } break;
            case 760: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
#line 836 "grammar.acc"
 TempList[TempListLoc++] = 0;
        TempList[TempListLoc++] = 0;
        TempList[TempListLoc++] = 0;
        ASSERT(TempListLoc < 1023);
        theRes->AddList(lis,TempList); 
# line 4459 "yygram.cpp"
            } break;
         default: goto yy48;
         }
      }
      yy48: ;
      } break;
   case 238: {
      get_lexval();
      switch (yyselect()) {
      case 762: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 768: {
#line 843 "grammar.acc"
 int16 Chance = 100; 
# line 4479 "yygram.cpp"
            get_lexval();
            switch (yyselect()) {
            case 763: {
               cexpr4(&n);
               get_lexval();
#line 843 "grammar.acc"
 Chance = n; 
# line 4487 "yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 764: {
               res_ref(&rr);
               get_lexval();
               get_lexval();
               cexpr3(&n);
#line 844 "grammar.acc"
 theRes->AddSpecial(rr,Chance,n); 
# line 4499 "yygram.cpp"
               } break;
            case 765: {
               res_ref(&rr);
               get_lexval();
               cexpr3(&n);
               get_lexval();
#line 845 "grammar.acc"
 theRes->AddSpecial(rr,Chance,n); 
# line 4508 "yygram.cpp"
               } break;
            case 766: {
               res_ref(&rr);
               get_lexval();
               get_lexval();
               dice_only(&dv);
#line 846 "grammar.acc"
 theRes->AddSpecial(rr,Chance,yyDice(dv)); 
# line 4517 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 767: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy50;
         }
      }
      yy50: ;
      get_lexval();
      } break;
   }
return 0;
}

int list_entry ()
{
   rID rr;
   YYSTYPE n;
   YYSTYPE text;
   switch(yyselect()) {
   case 239: {
      res_ref(&rr);
#line 851 "grammar.acc"
 TempList[TempListLoc++] = rr; ASSERT(TempListLoc < 1023); 
# line 4547 "yygram.cpp"
      } break;
   case 240: {
      cexpr4(&n);
#line 852 "grammar.acc"
 TempList[TempListLoc++] = n;  ASSERT(TempListLoc < 1023); 
# line 4553 "yygram.cpp"
      } break;
   case 241: {
      get_lexval();
      text = yylval;
#line 853 "grammar.acc"
 TempList[TempListLoc++] = text; ASSERT(TempListLoc < 1023); 
# line 4560 "yygram.cpp"
      } break;
   }
return 0;
}

int artifact_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 242: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 857 "grammar.acc"
 theRes = theArti; theArti->Name = name; 
# line 4575 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 769: {
            arti_entry();
            } break;
         default: goto yy51;
         }
      }
      yy51: ;
      get_lexval();
#line 858 "grammar.acc"
 theArti++; 
# line 4589 "yygram.cpp"
      } break;
   }
return 0;
}

int arti_entry ()
{
   YYSTYPE lv;
   rID rr;
   YYSTYPE at;
   YYSTYPE v;
   YYSTYPE res;
   YYSTYPE qu;
   switch(yyselect()) {
   case 243: {
      get_lexval();
      switch (yyselect()) {
      case 770: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 862 "grammar.acc"
 theRes = theArti; theArti->Desc = lv; 
# line 4616 "yygram.cpp"
      } break;
   case 244: {
      get_lexval();
      switch (yyselect()) {
      case 771: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 863 "grammar.acc"
 theArti->iID = rr; 
# line 4630 "yygram.cpp"
      } break;
   case 245: {
      while (1) {
         switch (yyselect()) {
         case 774: {
            get_lexval();
            at = yylval;
            switch (yyselect()) {
            case 772: {
               get_lexval();
               } break;
            default: ;
            }
            cexpr3(&v);
#line 864 "grammar.acc"
 theArti->AttrAdjLow[at] = v; 
# line 4647 "yygram.cpp"
            switch (yyselect()) {
            case 773: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy52;
         }
      }
      yy52: ;
      get_lexval();
      } break;
   case 246: {
      get_lexval();
      switch (yyselect()) {
      case 775: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 777: {
            cexpr3(&res);
#line 865 "grammar.acc"
 theArti->Resists |= res; 
# line 4675 "yygram.cpp"
            switch (yyselect()) {
            case 776: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy53;
         }
      }
      yy53: ;
      get_lexval();
      } break;
   case 247: {
      get_lexval();
      switch (yyselect()) {
      case 778: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 780: {
            get_lexval();
            at = yylval;
#line 866 "grammar.acc"
 theArti->Sustains |= 1 << (at-1); 
# line 4704 "yygram.cpp"
            switch (yyselect()) {
            case 779: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy54;
         }
      }
      yy54: ;
      get_lexval();
      } break;
   case 248: {
      get_lexval();
      switch (yyselect()) {
      case 781: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 783: {
            cexpr3(&qu);
#line 867 "grammar.acc"
 theArti->Qualities |= qu; 
# line 4732 "yygram.cpp"
            switch (yyselect()) {
            case 782: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy55;
         }
      }
      yy55: ;
      get_lexval();
      } break;
   case 249: {
      get_lexval();
#line 869 "grammar.acc"
 ArtPowerType = AN_ART_EQUIP; 
# line 4750 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 785: {
            art_power();
            switch (yyselect()) {
            case 784: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy56;
         }
      }
      yy56: ;
      get_lexval();
      } break;
   case 250: {
      get_lexval();
#line 870 "grammar.acc"
 ArtPowerType = AN_ART_WIELD; 
# line 4773 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 787: {
            art_power();
            switch (yyselect()) {
            case 786: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy57;
         }
      }
      yy57: ;
      get_lexval();
      } break;
   case 251: {
      get_lexval();
#line 871 "grammar.acc"
 ArtPowerType = AN_ART_HIT;   
# line 4796 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 789: {
            art_power();
            switch (yyselect()) {
            case 788: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy58;
         }
      }
      yy58: ;
      get_lexval();
      } break;
   case 252: {
      get_lexval();
#line 872 "grammar.acc"
 ArtPowerType = AN_ART_INVOKE;
# line 4819 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 791: {
            art_power();
            switch (yyselect()) {
            case 790: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy59;
         }
      }
      yy59: ;
      get_lexval();
      } break;
   case 253: {
      event_desc();
      } break;
   case 254: {
      r_declaration();
      } break;
   }
return 0;
}

int art_power ()
{
   YYSTYPE st;
   YYSTYPE p;
   rID rr;
   Dice dv;
   YYSTYPE n;
   YYSTYPE ft;
   switch(yyselect()) {
   case 255: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&p);
      get_lexval();
#line 879 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_STATI,st,p,NULL); 
# line 4865 "yygram.cpp"
      } break;
   case 256: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
#line 881 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_STATI,st,0,NULL); 
# line 4874 "yygram.cpp"
      } break;
   case 257: {
      res_ref(&rr);
      switch (yyselect()) {
      case 792: {
         get_lexval();
         get_lexval();
         } break;
      default: ;
      }
#line 883 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_EFFECT,rr,0,NULL); 
# line 4887 "yygram.cpp"
      } break;
   case 258: {
      res_ref(&rr);
      get_lexval();
      dice_val(&dv);
      get_lexval();
#line 885 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_EFFECT,rr,0,&yyDice(dv)); 
# line 4896 "yygram.cpp"
      } break;
   case 259: {
      res_ref(&rr);
      cexpr3(&n);
      switch (yyselect()) {
      case 793: {
         get_lexval();
         } break;
      case 794: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
#line 887 "grammar.acc"
 Dice d; d.Set(0,n,1);
        theArti->AddPower(ArtPowerType,AB_EFFECT,rr,0,&d); 
# line 4914 "yygram.cpp"
      } break;
   case 260: {
      res_ref(&rr);
      cexpr3(&n);
      switch (yyselect()) {
      case 795: {
         get_lexval();
         } break;
      case 796: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
#line 890 "grammar.acc"
 Dice d; d.Set(0,n,7);
        theArti->AddPower(ArtPowerType,AB_EFFECT,rr,0,&d); 
# line 4932 "yygram.cpp"
      } break;
   case 261: {
      get_lexval();
      res_ref(&rr);
#line 893 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_INNATE,rr,0,NULL); 
# line 4939 "yygram.cpp"
      } break;
   case 262: {
      get_lexval();
      cexpr3(&ft);
#line 895 "grammar.acc"
 theArti->AddPower(ArtPowerType,AB_FEAT,ft,0,NULL); 
# line 4946 "yygram.cpp"
      } break;
   }
return 0;
}

int domain_def ()
{
   YYSTYPE name;
   YYSTYPE type;
   switch(yyselect()) {
   case 263: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
      cexpr3(&type);
#line 899 "grammar.acc"
 theRes = theDom; theDom->Name = name; theDom->DType = type; 
# line 4964 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 797: {
            domain_entry();
            } break;
         default: goto yy60;
         }
      }
      yy60: ;
      get_lexval();
#line 901 "grammar.acc"
 theDom++; 
# line 4978 "yygram.cpp"
      } break;
   }
return 0;
}

int domain_entry ()
{
   rID rr;
   YYSTYPE AbT;
   YYSTYPE Abil;
   YYSTYPE p;
   YYSTYPE l1;
   YYSTYPE l2;
   YYSTYPE lv;
   switch(yyselect()) {
   case 264: {
#line 905 "grammar.acc"
 int8 sp = 0; 
# line 4996 "yygram.cpp"
      get_lexval();
      switch (yyselect()) {
      case 798: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 800: {
            res_ref(&rr);
#line 906 "grammar.acc"
 theDom->Spells[min(sp,8)] = rr; sp++; 
# line 5010 "yygram.cpp"
            switch (yyselect()) {
            case 799: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy61;
         }
      }
      yy61: ;
      get_lexval();
#line 907 "grammar.acc"
 if (sp > 9) 
      { yyerror(Format("Error: Domain %s has %d spells; must have at most nine.",
        theModule->QTextSeg + theDom->Name,sp)); } 
# line 5027 "yygram.cpp"
      } break;
   case 265: {
      get_lexval();
      switch (yyselect()) {
      case 801: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 803: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 802: {
               get_lexval();
               } break;
            default: ;
            }
#line 911 "grammar.acc"
 theDom->AddAbility(AbT,Abil,p,l1,l2); 
# line 5049 "yygram.cpp"
            } break;
         default: goto yy62;
         }
      }
      yy62: ;
      get_lexval();
      } break;
   case 266: {
      get_lexval();
      switch (yyselect()) {
      case 804: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 912 "grammar.acc"
 theDom->Desc = lv; 
# line 5070 "yygram.cpp"
      } break;
   case 267: {
      event_desc();
      } break;
   case 268: {
      dconst_entry();
      } break;
   case 269: {
      r_declaration();
      } break;
   }
return 0;
}

int god_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 270: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 918 "grammar.acc"
 theRes = theGod; theGod->Name = name; 
# line 5094 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 805: {
            god_entry();
            } break;
         default: goto yy63;
         }
      }
      yy63: ;
      get_lexval();
#line 920 "grammar.acc"
 theGod++; 
# line 5108 "yygram.cpp"
      } break;
   }
return 0;
}

int god_entry ()
{
   rID rr;
   YYSTYPE n;
   YYSTYPE lv;
   YYSTYPE AbT;
   YYSTYPE Abil;
   YYSTYPE p;
   YYSTYPE l1;
   YYSTYPE l2;
   switch(yyselect()) {
   case 271: {
#line 924 "grammar.acc"
 int8 dm = 0; 
# line 5127 "yygram.cpp"
      get_lexval();
      switch (yyselect()) {
      case 806: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 808: {
            res_ref(&rr);
#line 925 "grammar.acc"
 theGod->Domains[min(dm,5)] = rr; dm++; 
# line 5141 "yygram.cpp"
            switch (yyselect()) {
            case 807: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy64;
         }
      }
      yy64: ;
      get_lexval();
#line 926 "grammar.acc"
 if (dm < 3 || dm > 6) 
      { yyerror(Format("Error: God %s has %d domains; must have between 3 and 6.",
        theModule->QTextSeg + theGod->Name,dm)); } 
# line 5158 "yygram.cpp"
      } break;
   case 272: {
      get_lexval();
      get_lexval();
      } break;
   case 273: {
      get_lexval();
      switch (yyselect()) {
      case 809: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 812: {
            cexpr3(&n);
            switch (yyselect()) {
            case 810: {
               get_lexval();
               } break;
            case 811: {
               get_lexval();
               } break;
            default: ;
            }
#line 930 "grammar.acc"
 theGod->SetFlag(n); 
# line 5187 "yygram.cpp"
            } break;
         default: goto yy65;
         }
      }
      yy65: ;
      get_lexval();
      } break;
   case 274: {
      get_lexval();
      switch (yyselect()) {
      case 813: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 931 "grammar.acc"
 theGod->Desc = lv; 
# line 5208 "yygram.cpp"
      } break;
   case 275: {
      get_lexval();
      switch (yyselect()) {
      case 814: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 816: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 815: {
               get_lexval();
               } break;
            default: ;
            }
#line 933 "grammar.acc"
 theGod->AddAbility(AbT,Abil,p,l1,l2); 
# line 5230 "yygram.cpp"
            } break;
         default: goto yy66;
         }
      }
      yy66: ;
      get_lexval();
      } break;
   case 276: {
      event_desc();
      } break;
   case 277: {
      dconst_entry();
      } break;
   case 278: {
      r_declaration();
      } break;
   }
return 0;
}

int terrain_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 279: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 939 "grammar.acc"
 theRes = theTer; 
                                      theTer->Name = name; 
# line 5261 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 817: {
            terrain_entry();
            } break;
         default: goto yy67;
         }
      }
      yy67: ;
      get_lexval();
#line 942 "grammar.acc"
 theTer++; 
# line 5275 "yygram.cpp"
      } break;
   }
return 0;
}

int terrain_entry ()
{
   YYSTYPE lv;
   uint16 img;
   YYSTYPE mov;
   YYSTYPE pen;
   YYSTYPE mat;
   YYSTYPE n;
   switch(yyselect()) {
   case 280: {
      get_lexval();
      switch (yyselect()) {
      case 818: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 946 "grammar.acc"
 theTer->Desc = lv; 
# line 5302 "yygram.cpp"
      } break;
   case 281: {
      glyph_entry(&img);
#line 947 "grammar.acc"
 theTer->Image = img; 
# line 5308 "yygram.cpp"
      } break;
   case 282: {
      get_lexval();
      switch (yyselect()) {
      case 819: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mov);
      get_lexval();
      get_lexval();
#line 948 "grammar.acc"
 theTer->MoveMod = (mov-100)/5; 
# line 5323 "yygram.cpp"
      } break;
   case 283: {
      get_lexval();
      switch (yyselect()) {
      case 820: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&pen);
      get_lexval();
#line 949 "grammar.acc"
 theTer->Penalty = pen; 
# line 5337 "yygram.cpp"
      } break;
   case 284: {
      get_lexval();
      switch (yyselect()) {
      case 821: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mat);
      get_lexval();
#line 950 "grammar.acc"
 theTer->Material = mat; 
# line 5351 "yygram.cpp"
      } break;
   case 285: {
      get_lexval();
      switch (yyselect()) {
      case 822: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 825: {
            cexpr3(&n);
            switch (yyselect()) {
            case 823: {
               get_lexval();
               } break;
            case 824: {
               get_lexval();
               } break;
            default: ;
            }
#line 951 "grammar.acc"
 theTer->SetFlag(n); 
# line 5376 "yygram.cpp"
            } break;
         default: goto yy68;
         }
      }
      yy68: ;
      get_lexval();
      } break;
   case 286: {
      dconst_entry();
      } break;
   case 287: {
      event_desc();
      } break;
   case 288: {
      r_declaration();
      } break;
   }
return 0;
}

int region_def ()
{
   YYSTYPE name;
   YYSTYPE regflag;
   switch(yyselect()) {
   case 289: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
      cexpr3(&regflag);
#line 958 "grammar.acc"
 theRes = theReg; theReg->Name = name; theReg->SetFlag(regflag); 
    theReg->Size = SZ_COLLOSAL; 
# line 5410 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 826: {
            region_entry();
            } break;
         default: goto yy69;
         }
      }
      yy69: ;
      get_lexval();
#line 961 "grammar.acc"
 theReg++; 
# line 5424 "yygram.cpp"
      } break;
   }
return 0;
}

int region_entry ()
{
   YYSTYPE lv;
   rID rr;
   YYSTYPE n;
   YYSTYPE mg;
   YYSTYPE ch;
   YYSTYPE v;
   rID rr2;
   YYSTYPE v2;
   uint16 g;
   switch(yyselect()) {
   case 290: {
      get_lexval();
      switch (yyselect()) {
      case 827: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 965 "grammar.acc"
 theReg->Desc = lv; 
# line 5454 "yygram.cpp"
      } break;
   case 291: {
      get_lexval();
      switch (yyselect()) {
      case 828: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 966 "grammar.acc"
 theReg->Walls = rr; 
# line 5468 "yygram.cpp"
      } break;
   case 292: {
      get_lexval();
      switch (yyselect()) {
      case 829: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 967 "grammar.acc"
 theReg->Floor = rr; 
# line 5482 "yygram.cpp"
      } break;
   case 293: {
      get_lexval();
      switch (yyselect()) {
      case 830: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 968 "grammar.acc"
 theReg->Door = rr; 
# line 5496 "yygram.cpp"
      } break;
   case 294: {
      get_lexval();
      switch (yyselect()) {
      case 831: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 969 "grammar.acc"
 theReg->Depth = n; 
# line 5510 "yygram.cpp"
      } break;
   case 295: {
      get_lexval();
      switch (yyselect()) {
      case 832: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 970 "grammar.acc"
 theReg->Size = n; 
# line 5524 "yygram.cpp"
      } break;
   case 296: {
      get_lexval();
      switch (yyselect()) {
      case 833: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 836: {
            cexpr3(&n);
#line 971 "grammar.acc"
 theReg->RoomTypes |= BIT(n); 
# line 5540 "yygram.cpp"
            switch (yyselect()) {
            case 834: {
               get_lexval();
               } break;
            case 835: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy70;
         }
      }
      yy70: ;
      get_lexval();
      } break;
   case 297: {
      get_lexval();
      switch (yyselect()) {
      case 837: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 840: {
            cexpr3(&n);
            switch (yyselect()) {
            case 838: {
               get_lexval();
               } break;
            case 839: {
               get_lexval();
               } break;
            default: ;
            }
#line 973 "grammar.acc"
 theReg->SetFlag(n); 
# line 5580 "yygram.cpp"
            } break;
         default: goto yy71;
         }
      }
      yy71: ;
      get_lexval();
      } break;
   case 298: {
      get_lexval();
      get_lexval();
      get_lexval();
      mg = yylval;
      get_lexval();
#line 974 "grammar.acc"
 theReg->sx   = yyMapSize[currMap] % 256; 
                                    theReg->sy   = yyMapSize[currMap++] / 256;
                                    theReg->Grid = mg; 
# line 5598 "yygram.cpp"
      } break;
   case 299: {
      get_lexval();
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 853: {
#line 977 "grammar.acc"
 rID tID, xID, xID2; Glyph Img; uint8 fl; 
                                    tID = xID = xID2 = 0; Img = 0; fl = 0; 
# line 5609 "yygram.cpp"
            get_lexval();
            ch = yylval;
            get_lexval();
            res_ref(&rr);
#line 979 "grammar.acc"
 tID = rr; 
# line 5616 "yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 841: {
                  get_lexval();
                  cexpr(&n);
                  get_lexval();
#line 980 "grammar.acc"
 fl |= n; 
# line 5625 "yygram.cpp"
                  } break;
               case 850: {
                  get_lexval();
                  switch (yyselect()) {
                  case 842: {
                     res_ref(&rr);
#line 981 "grammar.acc"
 xID = rr; 
# line 5634 "yygram.cpp"
                     } break;
                  case 844: {
                     get_lexval();
                     get_lexval();
                     switch (yyselect()) {
                     case 843: {
                        cexpr(&v);
#line 982 "grammar.acc"
 xID = v; 
# line 5644 "yygram.cpp"
                        } break;
                     default: ;
                     }
#line 983 "grammar.acc"
 fl |= TILE_RANDOM | TILE_ITEM; 
# line 5650 "yygram.cpp"
                     } break;
                  case 846: {
                     get_lexval();
                     get_lexval();
                     switch (yyselect()) {
                     case 845: {
                        cexpr(&v);
#line 984 "grammar.acc"
 xID = v; 
# line 5660 "yygram.cpp"
                        } break;
                     default: ;
                     }
#line 985 "grammar.acc"
 fl |= TILE_RANDOM | TILE_MONSTER; 
# line 5666 "yygram.cpp"
                     } break;
                  default: printf("???\n"); exit(1);
                  }
                  switch (yyselect()) {
                  case 847: {
                     get_lexval();
                     res_ref(&rr2);
#line 986 "grammar.acc"
 xID2 = rr2; 
# line 5676 "yygram.cpp"
                     } break;
                  case 848: {
                     get_lexval();
                     get_lexval();
                     cexpr(&v);
                     get_lexval();
                     cexpr(&v2);
#line 988 "grammar.acc"
 fl |= TILE_RANDOM_EFF; xID2 = v+v2*256; 
# line 5686 "yygram.cpp"
                     } break;
                  case 849: {
                     get_lexval();
                     cexpr(&v);
#line 989 "grammar.acc"
 xID2 = v; 
# line 5693 "yygram.cpp"
                     } break;
                  default: ;
                  }
                  } break;
               case 851: {
                  get_lexval();
                  glyph_desc(&g);
#line 990 "grammar.acc"
 Img = g; 
# line 5703 "yygram.cpp"
                  } break;
               default: goto yy73;
               }
            }
            yy73: ;
#line 991 "grammar.acc"
 theReg->AddTile(ch,Img,fl,tID,xID,xID2); 
# line 5711 "yygram.cpp"
            switch (yyselect()) {
            case 852: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy72;
         }
      }
      yy72: ;
      get_lexval();
      } break;
   case 300: {
      get_lexval();
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 854: {
            region_entry();
            } break;
         default: goto yy74;
         }
      }
      yy74: ;
      } break;
   case 301: {
      get_lexval();
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 855: {
            object_entry();
            } break;
         default: goto yy75;
         }
      }
      yy75: ;
      } break;
   case 302: {
      dconst_entry();
      } break;
   case 303: {
      event_desc();
      } break;
   case 304: {
      r_declaration();
      } break;
   }
return 0;
}

int template_def ()
{
   YYSTYPE name;
   YYSTYPE ty;
   YYSTYPE mt;
   switch(yyselect()) {
   case 305: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 1001 "grammar.acc"
 theRes = theTemp; theTemp->Name = name; CurrFeat = 0; CurrAttk = 0; 
# line 5775 "yygram.cpp"
      get_lexval();
      cexpr3(&ty);
#line 1002 "grammar.acc"
 theTemp->TType = ty; 
# line 5780 "yygram.cpp"
      switch (yyselect()) {
      case 856: {
         get_lexval();
         cexpr3(&mt);
#line 1003 "grammar.acc"
 theTemp->ForMType = mt; 
# line 5787 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 857: {
            template_entry();
            } break;
         default: goto yy76;
         }
      }
      yy76: ;
      get_lexval();
#line 1005 "grammar.acc"
 theTemp++; 
# line 5804 "yygram.cpp"
      } break;
   }
return 0;
}

int template_entry ()
{
   YYSTYPE lv;
   YYSTYPE mt;
   uint16 img;
   MVal mv;
   YYSTYPE n;
   YYSTYPE at;
   YYSTYPE st;
   YYSTYPE stV;
   YYSTYPE stM;
   rID rr;
   YYSTYPE res;
   YYSTYPE fl;
   switch(yyselect()) {
   case 306: {
      get_lexval();
      switch (yyselect()) {
      case 858: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 1009 "grammar.acc"
 theTemp->Desc = lv; 
# line 5837 "yygram.cpp"
      } break;
   case 307: {
      get_lexval();
      switch (yyselect()) {
      case 859: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mt);
      get_lexval();
#line 1010 "grammar.acc"
 theTemp->ForMType = mt; 
# line 5851 "yygram.cpp"
      } break;
   case 308: {
      get_lexval();
      switch (yyselect()) {
      case 860: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mt);
      get_lexval();
#line 1011 "grammar.acc"
 theTemp->AddMType = mt; 
# line 5865 "yygram.cpp"
      } break;
   case 309: {
      glyph_entry(&img);
#line 1012 "grammar.acc"
 theTemp->NewImage = img; 
# line 5871 "yygram.cpp"
      } break;
   case 310: {
      switch (yyselect()) {
      case 861: {
         get_lexval();
         } break;
      case 862: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 863: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1013 "grammar.acc"
 theTemp->CR = mv; 
# line 5893 "yygram.cpp"
      } break;
   case 311: {
      get_lexval();
      switch (yyselect()) {
      case 864: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1014 "grammar.acc"
 theTemp->HitDice = mv; 
# line 5907 "yygram.cpp"
      } break;
   case 312: {
      get_lexval();
      switch (yyselect()) {
      case 865: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1015 "grammar.acc"
 theTemp->Hit = mv; 
# line 5921 "yygram.cpp"
      } break;
   case 313: {
      get_lexval();
      switch (yyselect()) {
      case 866: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1016 "grammar.acc"
 theTemp->Def = mv; 
# line 5935 "yygram.cpp"
      } break;
   case 314: {
      get_lexval();
      switch (yyselect()) {
      case 867: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1017 "grammar.acc"
 theTemp->Arm = mv; 
# line 5949 "yygram.cpp"
      } break;
   case 315: {
      get_lexval();
      switch (yyselect()) {
      case 868: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 1018 "grammar.acc"
 theTemp->DmgMod = n; 
# line 5963 "yygram.cpp"
      } break;
   case 316: {
      get_lexval();
      switch (yyselect()) {
      case 869: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1019 "grammar.acc"
 theTemp->Power = mv; 
# line 5977 "yygram.cpp"
      } break;
   case 317: {
      get_lexval();
      get_lexval();
      switch (yyselect()) {
      case 870: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1020 "grammar.acc"
 theTemp->CasterLev = mv; 
# line 5992 "yygram.cpp"
      } break;
   case 318: {
      get_lexval();
      switch (yyselect()) {
      case 871: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1021 "grammar.acc"
 theTemp->Size = mv; 
# line 6006 "yygram.cpp"
      } break;
   case 319: {
      get_lexval();
      switch (yyselect()) {
      case 872: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1022 "grammar.acc"
 theTemp->Spd = mv; 
# line 6020 "yygram.cpp"
      } break;
   case 320: {
      get_lexval();
      switch (yyselect()) {
      case 873: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1023 "grammar.acc"
 theTemp->Mov = mv; 
# line 6034 "yygram.cpp"
      } break;
   case 321: {
      get_lexval();
      at = yylval;
      switch (yyselect()) {
      case 874: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      switch (yyselect()) {
      case 875: {
         get_lexval();
         } break;
      case 876: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
#line 1024 "grammar.acc"
 theTemp->Attr[at] = mv; 
# line 6057 "yygram.cpp"
      } break;
   case 322: {
      get_lexval();
      switch (yyselect()) {
      case 877: {
         get_lexval();
         } break;
      default: ;
      }
      temp_attack_entry();
      while (1) {
         switch (yyselect()) {
         case 878: {
            get_lexval();
            temp_attack_entry();
            } break;
         default: goto yy77;
         }
      }
      yy77: ;
      get_lexval();
      } break;
   case 323: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      get_lexval();
#line 1028 "grammar.acc"
 if (!theTemp->GainStati(st)) yyerror("too many Stati in template"); 
# line 6088 "yygram.cpp"
      } break;
   case 324: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      get_lexval();
#line 1030 "grammar.acc"
 if (!theTemp->GainStati(st,stV)) yyerror("too many Stati in template"); 
# line 6100 "yygram.cpp"
      } break;
   case 325: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      cexpr3(&stM);
      get_lexval();
      get_lexval();
#line 1032 "grammar.acc"
 if (!theTemp->GainStati(st,stV,stM)) 
        yyerror("too many Stati in template"); 
# line 6115 "yygram.cpp"
      } break;
   case 326: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      cexpr3(&stM);
      get_lexval();
      res_ref(&rr);
      get_lexval();
      get_lexval();
#line 1035 "grammar.acc"
 if (!theTemp->GainStati(st,stV,stM,rr)) 
        yyerror("too many Stati in template"); 
# line 6132 "yygram.cpp"
      } break;
   case 327: {
      get_lexval();
      switch (yyselect()) {
      case 879: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 885: {
            switch (yyselect()) {
            case 880: {
               cexpr3(&res);
#line 1038 "grammar.acc"
 theTemp->AddRes |= res; 
# line 6150 "yygram.cpp"
               } break;
            case 883: {
               switch (yyselect()) {
               case 881: {
                  get_lexval();
                  } break;
               case 882: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&res);
#line 1039 "grammar.acc"
 theTemp->SubRes |= res; 
# line 6165 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 884: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy78;
         }
      }
      yy78: ;
      get_lexval();
      } break;
   case 328: {
      get_lexval();
      switch (yyselect()) {
      case 886: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 892: {
            switch (yyselect()) {
            case 887: {
               cexpr3(&res);
#line 1041 "grammar.acc"
 theTemp->AddImm |= res; 
# line 6198 "yygram.cpp"
               } break;
            case 890: {
               switch (yyselect()) {
               case 888: {
                  get_lexval();
                  } break;
               case 889: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&res);
#line 1042 "grammar.acc"
 theTemp->SubImm |= res; 
# line 6213 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 891: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy79;
         }
      }
      yy79: ;
      get_lexval();
      } break;
   case 329: {
      get_lexval();
      switch (yyselect()) {
      case 893: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 894: {
            cexpr3(&n);
            get_lexval();
#line 1044 "grammar.acc"
 theTemp->NewFeats[CurrFeat++] = n; 
# line 6245 "yygram.cpp"
            } break;
         default: goto yy80;
         }
      }
      yy80: ;
      cexpr3(&n);
      get_lexval();
#line 1045 "grammar.acc"
 theTemp->NewFeats[CurrFeat++] = n; 
# line 6255 "yygram.cpp"
      } break;
   case 330: {
      get_lexval();
      switch (yyselect()) {
      case 895: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 897: {
            res_ref(&rr);
#line 1046 "grammar.acc"
 theTemp->AddsFlag(M_INNATE);
                                       theTemp->AddResID(AN_INNATE, rr); 
# line 6272 "yygram.cpp"
            switch (yyselect()) {
            case 896: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy81;
         }
      }
      yy81: ;
      get_lexval();
      } break;
   case 331: {
      get_lexval();
      switch (yyselect()) {
      case 898: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 900: {
            res_ref(&rr);
#line 1049 "grammar.acc"
 theTemp->AddResID(AN_DISEASE, rr); 
# line 6300 "yygram.cpp"
            switch (yyselect()) {
            case 899: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy82;
         }
      }
      yy82: ;
      get_lexval();
      } break;
   case 332: {
      get_lexval();
      switch (yyselect()) {
      case 901: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 903: {
            res_ref(&rr);
#line 1051 "grammar.acc"
 theTemp->AddResID(AN_POISON, rr); 
# line 6328 "yygram.cpp"
            switch (yyselect()) {
            case 902: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy83;
         }
      }
      yy83: ;
      get_lexval();
      } break;
   case 333: {
      get_lexval();
      switch (yyselect()) {
      case 904: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 911: {
            switch (yyselect()) {
            case 905: {
               cexpr3(&fl);
#line 1054 "grammar.acc"
 if (fl < 0) 
                                         theTemp->Flags[(-fl)/8] |= 1 << ((-fl)%8);
                                       else
                                         theTemp->AddFlags[fl/8] |= 1 << (fl % 8); 
                                     
# line 6362 "yygram.cpp"
               } break;
            case 908: {
               switch (yyselect()) {
               case 906: {
                  get_lexval();
                  } break;
               case 907: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&fl);
#line 1059 "grammar.acc"
 theTemp->SubFlags[fl/8] |= 1 << (fl % 8); 
# line 6377 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 909: {
               get_lexval();
               } break;
            case 910: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy84;
         }
      }
      yy84: ;
      } break;
   case 334: {
      gear_desc();
      } break;
   case 335: {
      event_desc();
      } break;
   case 336: {
      dconst_entry();
      } break;
   case 337: {
      r_declaration();
      } break;
   }
return 0;
}

int temp_attack_entry ()
{
   YYSTYPE at;
   Dice dam;
   YYSTYPE chance;
   YYSTYPE n;
   YYSTYPE dc;
   Dice dam2;
   YYSTYPE n2;
   rID rr;
   YYSTYPE dt;
   switch(yyselect()) {
   case 338: {
#line 1067 "grammar.acc"
 uint8 dt = AD_NORM; dam.Set(0,0,0);
                                 theTemp->NewAttk[CurrAttk].u.a.Dmg.Set(0,0,0);
# line 6427 "yygram.cpp"
      cexpr3(&at);
      get_lexval();
      switch (yyselect()) {
      case 912: {
         dice_only(&dam);
#line 1070 "grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam); 
# line 6435 "yygram.cpp"
         } break;
      case 913: {
         cexpr3(&chance);
         get_lexval();
#line 1071 "grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg.Bonus = chance; 
# line 6442 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 914: {
         cexpr3(&n);
#line 1072 "grammar.acc"
 dt = n; 
# line 6451 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 915: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 919: {
         get_lexval();
         switch (yyselect()) {
         case 916: {
            get_lexval();
            } break;
         case 917: {
            get_lexval();
            } break;
         case 918: {
            get_lexval();
            } break;
         default: printf("???\n"); exit(1);
         }
         cexpr3(&dc);
#line 1075 "grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.DC = dc; 
# line 6479 "yygram.cpp"
         get_lexval();
         } break;
      default: ;
      }
#line 1076 "grammar.acc"
 theTemp->NewAttk[CurrAttk].AType = at;
                                 theTemp->NewAttk[CurrAttk].DType = dt;
                                 /*
                                 if (at == A_ABIL)

                                   theTemp->NewAttk[CurrAttk].u.a.DC = max(

                                        theTemp->NewAttk[CurrAttk].u.a.DC, 1);

                                        */
                                 CurrAttk++;
                                 if (CurrAttk >= 16)
                                   yyerror("> 16 attacks on a single template!");
                               
# line 6499 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 927: {
            get_lexval();
#line 1086 "grammar.acc"
 dt = AD_NORM; theTemp->NewAttk[CurrAttk].u.a.Dmg.Set(0,0,0); dam2.Set(0,0,0); 
# line 6506 "yygram.cpp"
            switch (yyselect()) {
            case 920: {
               dice_only(&dam2);
#line 1087 "grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam2); 
# line 6512 "yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 921: {
               cexpr3(&n2);
#line 1088 "grammar.acc"
 dt = n2; 
# line 6521 "yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 922: {
               get_lexval();
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 926: {
               get_lexval();
               switch (yyselect()) {
               case 923: {
                  get_lexval();
                  } break;
               case 924: {
                  get_lexval();
                  } break;
               case 925: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&dc);
#line 1090 "grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.DC = dc; 
# line 6549 "yygram.cpp"
               get_lexval();
               } break;
            default: ;
            }
#line 1091 "grammar.acc"
 theTemp->NewAttk[CurrAttk].AType = A_ALSO;
                                 theTemp->NewAttk[CurrAttk].DType = dt;
                                 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam2);
                                 CurrAttk++;
                                 if (CurrAttk >= 16)
                                   Fatal("> 16 attacks on a single monster!");
                               
# line 6562 "yygram.cpp"
            } break;
         default: goto yy85;
         }
      }
      yy85: ;
      } break;
   case 339: {
      cexpr3(&at);
      get_lexval();
      res_ref(&rr);
#line 1099 "grammar.acc"
 theTemp->NewAttk[CurrAttk].AType = at;
                                  theTemp->NewAttk[CurrAttk].u.xID = rr; 
# line 6576 "yygram.cpp"
      switch (yyselect()) {
      case 928: {
         cexpr3(&dt);
#line 1101 "grammar.acc"
 theTemp->NewAttk[CurrAttk].DType = dt; 
# line 6582 "yygram.cpp"
         } break;
      default: ;
      }
      } break;
   }
return 0;
}

int quest_def ()
{
   switch(yyselect()) {
   case 340: {
      get_lexval();
      } break;
   }
return 0;
}

int flavor_def ()
{
   YYSTYPE name;
   YYSTYPE it;
   switch(yyselect()) {
   case 341: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
      cexpr3(&it);
#line 1108 "grammar.acc"
 theRes = theFlavor; 
                                         theFlavor->Name = name;
                                         theFlavor->IType = it;
                                         theFlavor->Material = -1;
                                         theFlavor->Color = -1;
                                         theFlavor->Weight = 20; 
# line 6617 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 929: {
            flavor_entry();
            } break;
         default: goto yy86;
         }
      }
      yy86: ;
      get_lexval();
#line 1114 "grammar.acc"
 theFlavor++; 
# line 6631 "yygram.cpp"
      } break;
   }
return 0;
}

int flavor_entry ()
{
   YYSTYPE lv;
   YYSTYPE n;
   YYSTYPE col;
   switch(yyselect()) {
   case 342: {
      get_lexval();
      switch (yyselect()) {
      case 930: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 1118 "grammar.acc"
 theFlavor->Desc     = lv; 
# line 6655 "yygram.cpp"
      } break;
   case 343: {
      get_lexval();
      switch (yyselect()) {
      case 931: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 1119 "grammar.acc"
 theFlavor->Material = n; 
# line 6669 "yygram.cpp"
      } break;
   case 344: {
      get_lexval();
      switch (yyselect()) {
      case 932: {
         get_lexval();
         } break;
      default: ;
      }
      color(&col);
      get_lexval();
#line 1120 "grammar.acc"
 theFlavor->Color    = col; 
# line 6683 "yygram.cpp"
      } break;
   case 345: {
      get_lexval();
      switch (yyselect()) {
      case 933: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
      get_lexval();
#line 1121 "grammar.acc"
 theFlavor->Weight   = n/5; 
# line 6698 "yygram.cpp"
      } break;
   }
return 0;
}

int encounter_def ()
{
   YYSTYPE name;
   YYSTYPE n;
   switch(yyselect()) {
   case 346: {
      get_lexval();
#line 1126 "grammar.acc"
 theRes = theEnc; 
                     theEnc->Weight = 10; 
# line 6713 "yygram.cpp"
      get_lexval();
      name = yylval;
#line 1128 "grammar.acc"
 theEnc->Name = name; 
# line 6718 "yygram.cpp"
      switch (yyselect()) {
      case 934: {
         get_lexval();
         cexpr3(&n);
#line 1129 "grammar.acc"
 theEnc->Terrain |= n; 
# line 6725 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 935: {
            encounter_entry();
            } break;
         default: goto yy87;
         }
      }
      yy87: ;
      get_lexval();
#line 1132 "grammar.acc"
 theEnc++; 
# line 6742 "yygram.cpp"
      } break;
   }
return 0;
}

int encounter_entry ()
{
   YYSTYPE n;
   YYSTYPE n2;
   switch(yyselect()) {
   case 347: {
      get_lexval();
      get_lexval();
      cexpr(&n);
      get_lexval();
#line 1135 "grammar.acc"
 theEnc->Terrain |= n; 
# line 6759 "yygram.cpp"
      } break;
   case 348: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1136 "grammar.acc"
 theEnc->Depth = n; 
# line 6768 "yygram.cpp"
      } break;
   case 349: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1137 "grammar.acc"
 theEnc->Weight = n; 
# line 6777 "yygram.cpp"
      } break;
   case 350: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1138 "grammar.acc"
 theEnc->minCR = theEnc->maxCR = n; 
# line 6786 "yygram.cpp"
      } break;
   case 351: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
      get_lexval();
#line 1139 "grammar.acc"
 theEnc->minCR = n; theEnc->maxCR = 36; 
# line 6796 "yygram.cpp"
      } break;
   case 352: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      switch (yyselect()) {
      case 936: {
         get_lexval();
         } break;
      case 937: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      cexpr3(&n2);
      get_lexval();
#line 1141 "grammar.acc"
 theEnc->minCR = n; theEnc->maxCR = n2; 
# line 6815 "yygram.cpp"
      } break;
   case 353: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1142 "grammar.acc"
 theEnc->Align = n; 
# line 6824 "yygram.cpp"
      } break;
   case 354: {
      get_lexval();
      get_lexval();
#line 1143 "grammar.acc"
 currPart = 0; 
# line 6831 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 938: {
            encounter_part();
            } break;
         default: goto yy88;
         }
      }
      yy88: ;
      get_lexval();
      } break;
   case 355: {
      get_lexval();
      switch (yyselect()) {
      case 939: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 942: {
            cexpr3(&n);
#line 1146 "grammar.acc"
 theEnc->SetFlag(n); 
# line 6857 "yygram.cpp"
            switch (yyselect()) {
            case 940: {
               get_lexval();
               } break;
            case 941: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy89;
         }
      }
      yy89: ;
      get_lexval();
      } break;
   case 356: {
      gear_desc();
      } break;
   case 357: {
      event_desc();
      } break;
   case 358: {
      dconst_entry();
      } break;
   case 359: {
      r_declaration();
      } break;
   }
return 0;
}

int encounter_part ()
{
   YYSTYPE n;
   hCode hc;
   YYSTYPE n2;
   Dice d;
   rID rr;
   switch(yyselect()) {
   case 360: {
#line 1155 "grammar.acc"
 ASSERT(currPart < MAX_PARTS); 
# line 6900 "yygram.cpp"
      switch (yyselect()) {
      case 943: {
         get_lexval();
         } break;
      case 944: {
         get_lexval();
#line 1157 "grammar.acc"
 theEnc->Parts[currPart].Flags |= EP_ELSE; 
# line 6909 "yygram.cpp"
         } break;
      case 945: {
         get_lexval();
#line 1158 "grammar.acc"
 theEnc->Parts[currPart].Flags |= EP_OR; 
# line 6915 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
#line 1160 "grammar.acc"
 theEnc->Parts[currPart].Weight = 10; 
# line 6921 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 946: {
            cexpr4(&n);
            get_lexval();
#line 1162 "grammar.acc"
 theEnc->Parts[currPart].Chance = n; 
# line 6929 "yygram.cpp"
            } break;
         case 947: {
            get_lexval();
            get_lexval();
            cexpr4(&n);
            get_lexval();
            get_lexval();
#line 1163 "grammar.acc"
 theEnc->Parts[currPart].minCR = n; 
# line 6939 "yygram.cpp"
            } break;
         case 948: {
            get_lexval();
            cexpr4(&n);
            get_lexval();
#line 1164 "grammar.acc"
 theEnc->Parts[currPart].Weight = n; 
# line 6947 "yygram.cpp"
            } break;
         case 949: {
            get_lexval();
            sep_expr(&hc);
            get_lexval();
#line 1165 "grammar.acc"
 theEnc->Parts[currPart].Condition = hc; 
# line 6955 "yygram.cpp"
            } break;
         case 952: {
            cexpr4(&n);
            switch (yyselect()) {
            case 950: {
               get_lexval();
               } break;
            case 951: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            cexpr4(&n2);
#line 1166 "grammar.acc"
 theEnc->Parts[currPart].Amt.Number = n;
                                      theEnc->Parts[currPart].Amt.Bonus  = n2;
                                      theEnc->Parts[currPart].Amt.Sides  = 0; 
# line 6973 "yygram.cpp"
            } break;
         case 953: {
            cexpr4(&n);
#line 1169 "grammar.acc"
 theEnc->Parts[currPart].Amt.Number = n;
                                      theEnc->Parts[currPart].Amt.Bonus  = n;
                                      theEnc->Parts[currPart].Amt.Sides  = 0; 
# line 6981 "yygram.cpp"
            } break;
         case 954: {
            cexpr4(&n);
            get_lexval();
#line 1172 "grammar.acc"
 theEnc->Parts[currPart].Amt.Number = n;
                                      theEnc->Parts[currPart].Amt.Bonus  = n+50;
                                      theEnc->Parts[currPart].Amt.Sides  = 0; 
# line 6990 "yygram.cpp"
            } break;
         case 955: {
            dice_only(&d);
#line 1175 "grammar.acc"
 theEnc->Parts[currPart].Amt        = d; 
# line 6996 "yygram.cpp"
            } break;
         default: goto yy90;
         }
      }
      yy90: ;
      get_lexval();
      switch (yyselect()) {
      case 956: {
         res_ref(&rr);
#line 1181 "grammar.acc"
 theEnc->Parts[currPart].xID = rr; 
# line 7008 "yygram.cpp"
         } break;
      case 957: {
         cexpr3(&n);
#line 1182 "grammar.acc"
 theEnc->Parts[currPart].xID = n; 
# line 7014 "yygram.cpp"
         } break;
      case 958: {
         get_lexval();
         cexpr3(&n);
#line 1183 "grammar.acc"
 theEnc->Parts[currPart].xID = n;
                                      theEnc->Parts[currPart].Flags |= EP_ANYMON; 
# line 7022 "yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 959: {
         res_ref(&rr);
#line 1187 "grammar.acc"
 theEnc->Parts[currPart].xID2 = rr; 
# line 7031 "yygram.cpp"
         } break;
      case 960: {
         cexpr3(&n);
#line 1188 "grammar.acc"
 theEnc->Parts[currPart].xID2 = n; 
# line 7037 "yygram.cpp"
         } break;
      case 961: {
         get_lexval();
         cexpr3(&n);
#line 1189 "grammar.acc"
 theEnc->Parts[currPart].xID = n;
                                      theEnc->Parts[currPart].Flags |= EP_ANYTEMP; 
# line 7045 "yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 965: {
         get_lexval();
         cexpr3(&n);
#line 1193 "grammar.acc"
 theEnc->Parts[currPart].Flags |= n; 
# line 7055 "yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 964: {
               switch (yyselect()) {
               case 962: {
                  get_lexval();
                  } break;
               case 963: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&n);
#line 1194 "grammar.acc"
 theEnc->Parts[currPart].Flags |= n; 
# line 7071 "yygram.cpp"
               } break;
            default: goto yy91;
            }
         }
         yy91: ;
         get_lexval();
         } break;
      default: ;
      }
#line 1197 "grammar.acc"
 currPart++; 
# line 7083 "yygram.cpp"
      } break;
   }
return 0;
}

int behaviour_def ()
{
   YYSTYPE name;
   YYSTYPE c;
   switch(yyselect()) {
   case 361: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
      cexpr3(&c);
#line 1201 "grammar.acc"
 theRes = theBev; 
                                           theBev->Name = name;
                                           theBev->Conditions = c; 
# line 7103 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 966: {
            behaviour_entry();
            } break;
         default: goto yy92;
         }
      }
      yy92: ;
      get_lexval();
#line 1204 "grammar.acc"
 theBev++; 
# line 7117 "yygram.cpp"
      } break;
   }
return 0;
}

int behaviour_entry ()
{
   rID rr;
   YYSTYPE n;
   switch(yyselect()) {
   case 362: {
      get_lexval();
      switch (yyselect()) {
      case 967: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 1208 "grammar.acc"
 theBev->spID = rr; 
# line 7139 "yygram.cpp"
      } break;
   case 363: {
      get_lexval();
      switch (yyselect()) {
      case 968: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 971: {
            cexpr3(&n);
            switch (yyselect()) {
            case 969: {
               get_lexval();
               } break;
            case 970: {
               get_lexval();
               } break;
            default: ;
            }
#line 1209 "grammar.acc"
 theBev->SetFlag(n); 
# line 7164 "yygram.cpp"
            } break;
         default: goto yy93;
         }
      }
      yy93: ;
      get_lexval();
      } break;
   case 364: {
      event_desc();
      } break;
   case 365: {
      dconst_entry();
      } break;
   case 366: {
      r_declaration();
      } break;
   }
return 0;
}

int text_def ()
{
   YYSTYPE name;
   YYSTYPE text;
   rID rr;
   hCode hc;
   switch(yyselect()) {
   case 367: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 1216 "grammar.acc"
 theRes = theText;
                         theText->Name = name;
                         cText = 0; 
# line 7199 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 972: {
            get_lexval();
            text = yylval;
#line 1219 "grammar.acc"
 TempList[cText++] = text; 
# line 7207 "yygram.cpp"
            } break;
         case 973: {
            res_ref(&rr);
#line 1220 "grammar.acc"
 TempList[cText++] = rr; 
# line 7213 "yygram.cpp"
            } break;
         case 974: {
            get_lexval();
#line 1221 "grammar.acc"
 TempList[cText++] = TC_LPAREN; 
# line 7219 "yygram.cpp"
            } break;
         case 975: {
            get_lexval();
#line 1222 "grammar.acc"
 TempList[cText++] = TC_RPAREN; 
# line 7225 "yygram.cpp"
            } break;
         case 976: {
            get_lexval();
#line 1223 "grammar.acc"
 TempList[cText++] = TC_CHOICE; 
# line 7231 "yygram.cpp"
            } break;
         case 977: {
            get_lexval();
#line 1224 "grammar.acc"
 TempList[cText++] = TC_TERM; 
# line 7237 "yygram.cpp"
            } break;
         case 978: {
            get_lexval();
            sep_expr(&hc);
            get_lexval();
            get_lexval();
#line 1226 "grammar.acc"
 TempList[cText++] = TC_CASE;
                      TempList[cText++] = hc; 
# line 7247 "yygram.cpp"
            } break;
         case 979: {
            get_lexval();
            sep_stat(&hc);
            get_lexval();
#line 1229 "grammar.acc"
 TempList[cText++] = TC_ACTION;
                      TempList[cText++] = hc; 
# line 7256 "yygram.cpp"
            } break;
         default: goto yy94;
         }
      }
      yy94: ;
      get_lexval();
#line 1231 "grammar.acc"
 TempList[cText] = 0;
             theText->AddList(TEXT_LIST,TempList);
             theText++; 
# line 7267 "yygram.cpp"
      } break;
   }
return 0;
}

int prototype ()
{
   switch(yyselect()) {
   case 368: {
      get_lexval();
      } break;
   }
return 0;
}

int type_name (   YYSTYPE *ty)
{
   switch(yyselect()) {
   case 369: {
      get_lexval();
#line 1239 "grammar.acc"
 *ty = DT_VOID; 
# line 7288 "yygram.cpp"
      } break;
   case 370: {
      get_lexval();
#line 1240 "grammar.acc"
 *ty = DT_INT32; 
# line 7294 "yygram.cpp"
      } break;
   case 371: {
      get_lexval();
#line 1241 "grammar.acc"
 *ty = DT_HOBJ; 
# line 7300 "yygram.cpp"
      } break;
   case 372: {
      get_lexval();
#line 1242 "grammar.acc"
 *ty = DT_HTEXT; 
# line 7306 "yygram.cpp"
      } break;
   case 373: {
      get_lexval();
#line 1243 "grammar.acc"
 *ty = DT_RID; 
# line 7312 "yygram.cpp"
      } break;
   case 374: {
      get_lexval();
#line 1244 "grammar.acc"
 *ty = DT_STRING; 
# line 7318 "yygram.cpp"
      } break;
   case 375: {
      get_lexval();
#line 1245 "grammar.acc"
 *ty = DT_RECT; 
# line 7324 "yygram.cpp"
      } break;
   }
return 0;
}

int s_prototype ()
{
   YYSTYPE ty;
   YYSTYPE ot;
   YYSTYPE fn;
   YYSTYPE ty_1;
   YYSTYPE ot_1;
   YYSTYPE paramname;
   YYSTYPE def;
   YYSTYPE ty_n;
   YYSTYPE ot_n;
   switch(yyselect()) {
   case 376: {
#line 1249 "grammar.acc"
 BSysFunc *b; 
# line 7344 "yygram.cpp"
      get_lexval();
      type_name(&ty);
      cexpr3(&ot);
      get_lexval();
      get_lexval();
      fn = yylval;
      get_lexval();
#line 1251 "grammar.acc"
 if (is_res(ot)) {
        if (theSymTab.GetBinding(fn,0,0,RES_FUNC))
          if (GETBIND_RESFUNC(fn)->RType == ot)
            yyerror(Format("Resource member function '%s' declared for same resource type twice!",theSymTab[fn]));
        b = new BResFunc(MemFuncID++,ty,ot);
        theSymTab.Bind(fn,b);
        }
      else {
        if (theSymTab.GetBinding(fn,0,0,MEM_FUNC))
          yyerror(Format("System member function '%s' declared twice!",theSymTab[fn]));
        /*else if (theSymTab.GetBinding(fn,1,0,ANY))

          yyerror(Format("Identifier conflict: '%s'.", theSymTab[fn]));*/
        b = new BMemFunc(MemFuncID++,ty,ot);
        theSymTab.Bind(fn,b); 
      }
    
# line 7370 "yygram.cpp"
      switch (yyselect()) {
      case 987: {
         type_name(&ty_1);
         switch (yyselect()) {
         case 980: {
            get_lexval();
            cexpr3(&ot_1);
#line 1267 "grammar.acc"
 b->ParamOType[0] = ot_1; 
# line 7380 "yygram.cpp"
            } break;
         default: ;
         }
#line 1268 "grammar.acc"
 b->ParamType[0] = ty_1; b->ParamCount++; 
# line 7386 "yygram.cpp"
         switch (yyselect()) {
         case 981: {
            get_lexval();
            paramname = yylval;
            } break;
         default: ;
         }
         switch (yyselect()) {
         case 982: {
            get_lexval();
            cexpr(&def);
#line 1269 "grammar.acc"
 b->Default[0] = def; b->HasDefault |= BIT(2); 
# line 7400 "yygram.cpp"
            } break;
         default: ;
         }
         while (1) {
            switch (yyselect()) {
            case 986: {
               get_lexval();
               type_name(&ty_n);
               switch (yyselect()) {
               case 983: {
                  get_lexval();
                  cexpr3(&ot_n);
#line 1271 "grammar.acc"
 b->ParamOType[b->ParamCount] = ot_n; 
# line 7415 "yygram.cpp"
                  } break;
               default: ;
               }
#line 1272 "grammar.acc"
 b->ParamType[b->ParamCount] = ty_n; b->ParamCount++; 
# line 7421 "yygram.cpp"
               switch (yyselect()) {
               case 984: {
                  get_lexval();
                  paramname = yylval;
                  } break;
               default: ;
               }
               switch (yyselect()) {
               case 985: {
                  get_lexval();
                  cexpr(&def);
#line 1273 "grammar.acc"
 b->Default[b->ParamCount] = def; b->HasDefault |= BIT(1+b->ParamCount); 
# line 7435 "yygram.cpp"
                  } break;
               default: ;
               }
               } break;
            default: goto yy95;
            }
         }
         yy95: ;
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 988: {
         get_lexval();
         get_lexval();
#line 1275 "grammar.acc"
 b->isVarParam = true; 
# line 7453 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      } break;
   }
return 0;
}

int sg_prototype ()
{
   YYSTYPE ty;
   YYSTYPE fn;
   YYSTYPE ty_1;
   YYSTYPE ot_1;
   YYSTYPE paramname;
   YYSTYPE def;
   YYSTYPE ty_n;
   YYSTYPE ot_n;
   switch(yyselect()) {
   case 377: {
#line 1278 "grammar.acc"
 BSysFunc *b; 
# line 7477 "yygram.cpp"
      get_lexval();
      type_name(&ty);
      get_lexval();
      fn = yylval;
      get_lexval();
#line 1280 "grammar.acc"
 if (theSymTab.GetBinding(fn,0,0,SYS_FUNC))
        yyerror(Format("System global function '%s' declared twice!",theSymTab[fn]));
      else if (theSymTab.GetBinding(fn,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[fn]));
      b = new BSysFunc(MemFuncID++,ty);
      theSymTab.Bind(fn,b); 
# line 7490 "yygram.cpp"
      switch (yyselect()) {
      case 996: {
         type_name(&ty_1);
         switch (yyselect()) {
         case 989: {
            get_lexval();
            cexpr3(&ot_1);
#line 1286 "grammar.acc"
 b->ParamOType[0] = ot_1; 
# line 7500 "yygram.cpp"
            } break;
         default: ;
         }
#line 1287 "grammar.acc"
 b->ParamType[0] = ty_1; b->ParamCount++; 
# line 7506 "yygram.cpp"
         switch (yyselect()) {
         case 990: {
            get_lexval();
            paramname = yylval;
            } break;
         default: ;
         }
         switch (yyselect()) {
         case 991: {
            get_lexval();
            cexpr(&def);
#line 1288 "grammar.acc"
 b->Default[0] = def; b->HasDefault |= BIT(2); 
# line 7520 "yygram.cpp"
            } break;
         default: ;
         }
         while (1) {
            switch (yyselect()) {
            case 995: {
               get_lexval();
               type_name(&ty_n);
               switch (yyselect()) {
               case 992: {
                  get_lexval();
                  cexpr3(&ot_n);
#line 1290 "grammar.acc"
 b->ParamOType[b->ParamCount] = ot_n; 
# line 7535 "yygram.cpp"
                  } break;
               default: ;
               }
#line 1291 "grammar.acc"
 b->ParamType[b->ParamCount] = ty_n; b->ParamCount++; 
# line 7541 "yygram.cpp"
               switch (yyselect()) {
               case 993: {
                  get_lexval();
                  paramname = yylval;
                  } break;
               default: ;
               }
               switch (yyselect()) {
               case 994: {
                  get_lexval();
                  cexpr(&def);
#line 1292 "grammar.acc"
 b->Default[b->ParamCount] = def; b->HasDefault |= BIT(1+b->ParamCount); 
# line 7555 "yygram.cpp"
                  } break;
               default: ;
               }
               } break;
            default: goto yy96;
            }
         }
         yy96: ;
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 997: {
         get_lexval();
         get_lexval();
#line 1294 "grammar.acc"
 b->isVarParam = true; 
# line 7573 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      } break;
   }
return 0;
}

int s_declaration ()
{
   YYSTYPE ty;
   YYSTYPE ot;
   YYSTYPE vn;
   switch(yyselect()) {
   case 378: {
#line 1297 "grammar.acc"
 BMemVar *b; BResMem *b2; 
# line 7592 "yygram.cpp"
      get_lexval();
      type_name(&ty);
      cexpr3(&ot);
      get_lexval();
      get_lexval();
      vn = yylval;
      get_lexval();
#line 1299 "grammar.acc"
 
      if (is_res(ot)) {
        if (theSymTab.GetBinding(vn,0,RES_MEM))
          if (((BResMem*)theSymTab.GetBinding(vn,0,0,RES_MEM))->RType == ot)
            yyerror(Format("System resource member '%s' declared twice!",theSymTab[vn]));
        b2 = new BResMem();
        b2->varid = MemVarID++;
        b2->VarType = ty;
        b2->RType = ot;
        b2->xID   = 0;
        b2->Event = 1;
        b2->type  = RES_MEM;
        theSymTab.Bind(vn,b2);
        }
      else {
        if (theSymTab.GetBinding(vn,0,0,MEM_VAR))
          yyerror(Format("System member variable '%s' declared twice!",theSymTab[vn]));
        else if (theSymTab.GetBinding(vn,0,1,ANY))
          yyerror(Format("Identifier conflict: '%s'.", theSymTab[vn]));
        b = new BMemVar();
        b->varid = MemVarID++; 
        b->VarType = ty; 
        b->OType = ot;
        b->xID   = 0;
        b->Event = 1;
        b->type  = MEM_VAR;
        theSymTab.Bind(vn,b); 
        }
    
# line 7630 "yygram.cpp"
      } break;
   }
return 0;
}

int s_object_decl ()
{
   YYSTYPE ot;
   YYSTYPE oname;
   YYSTYPE oid;
   switch(yyselect()) {
   case 379: {
#line 1330 "grammar.acc"
 BSysObj *b; 
# line 7644 "yygram.cpp"
      get_lexval();
      cexpr3(&ot);
      get_lexval();
      oname = yylval;
      get_lexval();
      cexpr3(&oid);
      get_lexval();
#line 1332 "grammar.acc"
 if (theSymTab.GetBinding(oname,0,0,SYS_OBJ))
        yyerror(Format("System object '%s' declared twice!",theSymTab[oname]));
      else if (theSymTab.GetBinding(oname,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[oname]));
      b = new BSysObj();
      b->ObjNum = oid;
      b->OType  = ot;
      b->xID   = 0;
      b->Event = 0;
      b->type  = SYS_OBJ;
      theSymTab.Bind(oname,b); 
# line 7664 "yygram.cpp"
      } break;
   }
return 0;
}

int g_declaration ()
{
   YYSTYPE ty;
   YYSTYPE vn;
   switch(yyselect()) {
   case 380: {
#line 1346 "grammar.acc"
 BGlobalVar *b; 
# line 7677 "yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1348 "grammar.acc"
 if (ty == DT_STRING)
        Error("Global variables of type String cannot be created!");
      if (theSymTab.GetBinding(vn,0,0,GLOB_VAR))
        Error("Global variable '%s' declared twice!",theSymTab[vn]);
      else if (theSymTab.GetBinding(vn,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[vn]));
      b = new BGlobalVar();
      b->VarType = ty; 
      b->Address = HeapHead++;
      b->xID     = 0;
      b->Event   = 0;
      
      b->type    = GLOB_VAR;
      theSymTab.Bind(vn,b); 
# line 7696 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 998: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1363 "grammar.acc"
 if (theSymTab.GetBinding(vn,0,0,GLOB_VAR))
        Error("Global variable '%s' declared twice!",theSymTab[vn]);
      else if (theSymTab.GetBinding(vn,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[vn]));
      b = new BGlobalVar();
      b->VarType = ty; 
      b->Address = HeapHead++;
      b->xID     = 0;
      b->Event   = 0;
      b->type    = GLOB_VAR;
      theSymTab.Bind(vn,b); 
# line 7715 "yygram.cpp"
            } break;
         default: goto yy97;
         }
      }
      yy97: ;
      get_lexval();
      } break;
   }
return 0;
}

int l_declaration (   VBlock *code)
{
   YYSTYPE ty;
   YYSTYPE vn;
   switch(yyselect()) {
   case 381: {
#line 1376 "grammar.acc"
 BLocalVar *b; int32 scope, i; String scope_id; 
# line 7734 "yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1378 "grammar.acc"
 b = new BLocalVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      ASSERT(theEvent);
      b->Event = theEvent;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,LOC_VAR))
        Error("Local variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = ty; 
      b->StackOffset = ++StackHead;
      b->type    = LOC_VAR;
      if (ty == DT_STRING) {
        i = AllocString();
        LockString(i);
        code->Generate(MOV,RT_MEMORY,-(b->StackOffset),RT_CONSTANT,-i);
        code->Generate(ESTR,RT_CONSTANT,-i);
        }
      theSymTab.Bind(vn,b); 
# line 7755 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 999: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1395 "grammar.acc"
 b = new BLocalVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      ASSERT(theEvent);
      b->Event = theEvent;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,LOC_VAR))
        Error("Local variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = ty; 
      b->StackOffset = ++StackHead;
      b->type    = LOC_VAR;
      if (ty == DT_STRING) {
        i = AllocString();
        LockString(i);
        code->Generate(MOV,RT_MEMORY,-(b->StackOffset),RT_CONSTANT,-i);
        code->Generate(ESTR,RT_CONSTANT,-i);
        }
      theSymTab.Bind(vn,b); 
# line 7779 "yygram.cpp"
            } break;
         default: goto yy98;
         }
      }
      yy98: ;
      get_lexval();
      } break;
   }
return 0;
}

int r_declaration ()
{
   YYSTYPE ty;
   YYSTYPE vn;
   switch(yyselect()) {
   case 382: {
#line 1413 "grammar.acc"
 BResVar *b; int32 scope; String scope_id; 
# line 7798 "yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1415 "grammar.acc"
 if (ty == DT_STRING)
        yyerror("Resource variables of type String cannot be allocated.");
      b = new BResVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      b->Event = 0;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,RES_VAR))
        Error("Resource variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = ty; 
      b->Address = HeapHead++;
      b->type    = RES_VAR;
      theSymTab.Bind(vn,b); 
# line 7814 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1000: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1427 "grammar.acc"
 b = new BResVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      b->Event = 0;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,RES_VAR))
        Error("Resource variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = ty; 
      b->Address = HeapHead++;
      b->type    = RES_VAR;
      theSymTab.Bind(vn,b); 
# line 7831 "yygram.cpp"
            } break;
         default: goto yy99;
         }
      }
      yy99: ;
      get_lexval();
      } break;
   }
return 0;
}

int statement (   VBlock *st)
{
   PExp ex;
   VBlock st1;
   VBlock st2;
   PExp ex1;
   PExp ex2;
   PExp ex3;
   YYSTYPE val;
   rID rr;
   VBlock st_n;
   VBlock bl;
   switch(yyselect()) {
   case 383: {
      expr(&ex);
      get_lexval();
#line 1441 "grammar.acc"

      st->Add(ex.Code);
      if (ex.Storage == RT_REGISTER) {
        st->TrimGratuitousMOV();
        FreeRegister(ex.Value);
        }
      if (ex.Type == DT_STRING && ex.Storage == RT_CONSTANT)
        if (ex.Value < -1)
          FreeString(-ex.Value);
    
# line 7869 "yygram.cpp"
      } break;
   case 384: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
      get_lexval();
      statement(&st2);
#line 1452 "grammar.acc"

      st->Add(ex.Code);
      /* Here, we compute the address to include the size

         of the JUMP opcode below, which may be one or two

         words, depending upon if it needs data space. */
      st->Generate(JFAL, ex.Storage, ex.Value, 
        RT_CONSTANT, st1.GetSize()+ (st2.GetSize()+1> 511?
          3 : 2));
      if (ex.Storage == RT_REGISTER)
        FreeRegister(ex.Value);                  
      st->Add(&st1);                            
      st->Generate(JUMP, RT_CONSTANT, st2.GetSize()+1);
      st->Add(&st2);
    
# line 7896 "yygram.cpp"
      } break;
   case 385: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
#line 1467 "grammar.acc"

      st->Add(ex.Code);
      st->Generate(JFAL, ex.Storage, ex.Value, 
        RT_CONSTANT, st1.GetSize()+1);
      if (ex.Storage == RT_REGISTER)
        FreeRegister(ex.Value);
      st->Add(&st1);
    
# line 7913 "yygram.cpp"
      } break;
   case 386: {
      get_lexval();
      get_lexval();
      expr(&ex1);
      get_lexval();
      expr(&ex2);
      get_lexval();
      expr(&ex3);
      get_lexval();
      statement(&st1);
#line 1477 "grammar.acc"

      int16 jumpback;
      ex1.Code->TrimGratuitousMOV();
      ex3.Code->TrimGratuitousMOV();
      st->Add(ex1.Code);
      if (ex1.Storage == RT_REGISTER)
        FreeRegister(ex1.Value);
      jumpback = st->GetSize();
      st->Add(ex2.Code);
      /* Here again, we compensate for the fact that the lower JUMP

         opcode may be either one or two words; there has to be a

         less kludgy way to do this... */
      st->Generate(JFAL, ex2.Storage,ex2.Value,
        RT_CONSTANT,st1.GetSize() + (ex3.Code ? ex3.Code->GetSize() : 0) + 
        (st1.GetSize() + (ex3.Code ? ex3.Code->GetSize() : 0) + 
                         (ex2.Code ? ex2.Code->GetSize() : 0) > 511 ? 3 : 2));
      if (ex2.Storage == RT_REGISTER)
        FreeRegister(ex2.Value);
      st1.ProcessContinues();
      st->Add(&st1);
      st->Add(ex3.Code);
      if (ex3.Storage == RT_REGISTER)
        FreeRegister(ex3.Value);
      st->Generate(JUMP,RT_CONSTANT,jumpback - st->GetSize());
    
# line 7953 "yygram.cpp"
      } break;
   case 387: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      get_lexval();
#line 1503 "grammar.acc"

        int16 CaseAddr[256], DefaultAddr;
        int32 CaseVal[256];
        VBlock *CaseCode[256], *DefaultCode;
        int8 CaseCount, i;
        CaseCount = 0;
        DefaultAddr = 0;
        CaseAddr[0] = 0; 
      
# line 7971 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1004: {
            get_lexval();
            switch (yyselect()) {
            case 1001: {
               cexpr(&val);
               } break;
            case 1002: {
               res_ref(&rr);
#line 1512 "grammar.acc"
 val = rr; 
# line 7984 "yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            get_lexval();
#line 1513 "grammar.acc"
 CaseCode[CaseCount] = new VBlock; 
# line 7991 "yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 1003: {
#line 1514 "grammar.acc"
 st_n.Clear(); 
# line 7997 "yygram.cpp"
                  statement(&st_n);
#line 1514 "grammar.acc"
 CaseCode[CaseCount]->Add(&st_n);  
# line 8001 "yygram.cpp"
                  } break;
               default: goto yy101;
               }
            }
            yy101: ;
#line 1515 "grammar.acc"

        /* We know where *this* case ends, which is synonimous with where

           the *next* case starts... */
        CaseAddr[CaseCount + 1] = CaseAddr[CaseCount] +
          CaseCode[CaseCount]->GetSize();
        CaseVal[CaseCount] = val;
        CaseCount++;  
      
# line 8017 "yygram.cpp"
            } break;
         default: goto yy100;
         }
      }
      yy100: ;
      switch (yyselect()) {
      case 1006: {
         get_lexval();
         get_lexval();
#line 1524 "grammar.acc"
 DefaultCode = new VBlock; 
# line 8029 "yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 1005: {
#line 1525 "grammar.acc"
 st_n.Clear(); 
# line 8035 "yygram.cpp"
               statement(&st_n);
#line 1525 "grammar.acc"
 DefaultCode->Add(&st_n);  
# line 8039 "yygram.cpp"
               } break;
            default: goto yy102;
            }
         }
         yy102: ;
#line 1526 "grammar.acc"
 
        DefaultAddr = CaseAddr[CaseCount]; 
        CaseAddr[CaseCount] = DefaultAddr + DefaultCode->GetSize(); 
      
# line 8050 "yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
#line 1533 "grammar.acc"

        /* Now we code the *entire* switch statement! */
        
        /* Evaluate the Expression */
        st->Add(ex.Code);

        /* Set the break location */
        st->Generate(SBRK, RT_CONSTANT, CaseAddr[CaseCount]+(CaseCount*2+3));

        /* Jump to the proper case */
        st->Generate(JTAB, ex.Storage, ex.Value, RT_CONSTANT, CaseCount);
        
        /* Write out the Jump Table */
        for(i=0;i!=CaseCount;i++) {
          st->GenDWord(CaseVal[i]);
          st->GenDWord(CaseAddr[i] + (CaseCount*2+1));
          }
        /* And then add the default case */
        st->GenDWord((DefaultAddr ? DefaultAddr : CaseAddr[i]) + (CaseCount*2+1));

        if (ex.Storage == RT_REGISTER)
          FreeRegister(ex.Value);

        /* Copy in each case's code, in order */
        for (i=0;i!=CaseCount;i++)
          {
            st->Add(CaseCode[i]);
            delete CaseCode[i];
          } 

        /* Don't forget the default case */
        if (DefaultAddr) {
          st->Add(DefaultCode);
          delete DefaultCode;
          }

        /* Free the break location */
        st->Generate(FBRK);
        
      
# line 8096 "yygram.cpp"
      } break;
   case 388: {
      get_lexval();
      get_lexval();
#line 1574 "grammar.acc"

        st->Generate(JBRK);
      
# line 8105 "yygram.cpp"
      } break;
   case 389: {
      get_lexval();
      get_lexval();
#line 1578 "grammar.acc"

        st->Generate(CONT);
      
# line 8114 "yygram.cpp"
      } break;
   case 390: {
      get_lexval();
      statement(&st1);
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      get_lexval();
#line 1582 "grammar.acc"

        st->Add(&st1);
        st->Add(ex.Code);
        st->Generate(JTRU,ex.Storage,ex.Value,
          RT_CONSTANT, -st->GetSize());
        if (ex.Storage == RT_REGISTER)
          FreeRegister(ex.Value);
      
# line 8133 "yygram.cpp"
      } break;
   case 391: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
#line 1591 "grammar.acc"

        st->Add(ex.Code);
        st->Generate(JFAL,ex.Storage,ex.Value,
          RT_CONSTANT, st1.GetSize()+
            (st->GetSize() > 511 ? 3 : 2));
        st->Add(&st1);
        st->Generate(JUMP,RT_CONSTANT, - st->GetSize());
        if (ex.Storage == RT_REGISTER)
          FreeRegister(ex.Value);
      
# line 8152 "yygram.cpp"
      } break;
   case 392: {
      block(&bl);
#line 1602 "grammar.acc"
 
        st->Add(&bl); 
      
# line 8160 "yygram.cpp"
      } break;
   case 393: {
      get_lexval();
      expr(&ex);
      get_lexval();
#line 1606 "grammar.acc"

        st->Add(ex.Code);
        if (StackHead) {
          st->Generate(MOV,RT_REGISTER,0,ex.Storage,ex.Value);
          st->Generate(DEC,RT_REGISTER,63,RT_CONSTANT,StackHead);
          st->Generate(RET,RT_REGISTER,0);
          }
        else
          st->Generate(RET,ex.Storage,ex.Value);
        if (ex.Storage == RT_REGISTER)
          FreeRegister(ex.Value);
      
# line 8179 "yygram.cpp"
      } break;
   case 394: {
      get_lexval();
      get_lexval();
#line 1619 "grammar.acc"

        if (StackHead)
          st->Generate(DEC,RT_REGISTER,63,RT_CONSTANT,StackHead);
        st->Generate(RET);
      
# line 8190 "yygram.cpp"
      } break;
   }
return 0;
}

int block (   VBlock *bl)
{
   VBlock st;
   switch(yyselect()) {
   case 395: {
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 1007: {
#line 1627 "grammar.acc"
 st.Clear(); 
# line 8206 "yygram.cpp"
            statement(&st);
#line 1627 "grammar.acc"
 bl->Add(&st); 
# line 8210 "yygram.cpp"
            } break;
         default: goto yy103;
         }
      }
      yy103: ;
      get_lexval();
      } break;
   }
return 0;
}

int routine (   VBlock *bl)
{
   VBlock code;
   VBlock st;
   switch(yyselect()) {
   case 396: {
#line 1631 "grammar.acc"
 VBlock decl_code; StackHead = 0; ClearStrings(); 
# line 8229 "yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 1008: {
#line 1632 "grammar.acc"
 code.Clear(); 
# line 8236 "yygram.cpp"
            l_declaration(&code);
#line 1632 "grammar.acc"
 decl_code.Add(&code); 
# line 8240 "yygram.cpp"
            } break;
         case 1009: {
            get_lexval();
            r_declaration();
            } break;
         default: goto yy104;
         }
      }
      yy104: ;
#line 1634 "grammar.acc"
 if (StackHead)
          bl->Generate(INC,RT_REGISTER,63,RT_CONSTANT,StackHead);
        bl->Add(&decl_code); 
# line 8254 "yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1010: {
#line 1637 "grammar.acc"
 st.Clear(); 
# line 8260 "yygram.cpp"
            statement(&st);
#line 1637 "grammar.acc"
 bl->Add(&st); 
# line 8264 "yygram.cpp"
            } break;
         default: goto yy105;
         }
      }
      yy105: ;
      get_lexval();
#line 1638 "grammar.acc"
 if (StackHead)
          bl->Generate(DEC,RT_REGISTER,63,RT_CONSTANT,StackHead); 
      
        /* This code is being inserted by FJM May 7 2006 to suppress

           the bug where registers fail to deallocate. Remove it to

           cause that bug to manifest again for debugging purposes,

           or insert a warning here to track WHICH scripts leave

           registers still allocated. */
        int16 i;
        for (i=0;i!=64;i++)
          FreeRegister(i);
      
# line 8288 "yygram.cpp"
      } break;
   }
return 0;
}

int fund_expr (   PExp *ex)
{
   YYSTYPE n;
   YYSTYPE lv;
   rID rr;
   PExp ex2;
   YYSTYPE id;
   PExp obj;
   YYSTYPE fn;
   pBSysFunc b;
   VBlock bl;
   int narg;
   PExp rect;
   switch(yyselect()) {
   case 397: {
      get_lexval();
      n = yylval;
#line 1674 "grammar.acc"
 ex->Type = DT_INT32; ex->Storage = RT_CONSTANT; ex->Value = n; ex->Code = NULL; 
# line 8312 "yygram.cpp"
      } break;
   case 398: {
      get_lexval();
      n = yylval;
#line 1676 "grammar.acc"
 ex->Type = DT_INT32; ex->Storage = RT_CONSTANT; ex->Value = n; ex->Code = NULL; 
# line 8319 "yygram.cpp"
      } break;
   case 399: {
      get_lexval();
      lv = yylval;
#line 1678 "grammar.acc"
 ex->Type = DT_HTEXT; ex->Storage = RT_CONSTANT; ex->Value = lv; ex->Code = NULL; 
# line 8326 "yygram.cpp"
      } break;
   case 400: {
      get_lexval();
#line 1680 "grammar.acc"
 ex->Type = DT_BOOL; ex->Storage = RT_CONSTANT; ex->Value = 1; ex->Code = NULL; 
# line 8332 "yygram.cpp"
      } break;
   case 401: {
      get_lexval();
#line 1682 "grammar.acc"
 ex->Type = DT_BOOL; ex->Storage = RT_CONSTANT; ex->Value = 0; ex->Code = NULL; 
# line 8338 "yygram.cpp"
      } break;
   case 402: {
      get_lexval();
#line 1684 "grammar.acc"
 ex->Type = DT_UNKNOWN; ex->Storage = RT_CONSTANT; ex->Value = 0; ex->Code = NULL; 
# line 8344 "yygram.cpp"
      } break;
   case 403: {
      res_ref(&rr);
#line 1686 "grammar.acc"
 ex->Type = DT_RID;   ex->Storage = RT_CONSTANT; ex->Value = rr; ex->Code = NULL; 
# line 8350 "yygram.cpp"
      } break;
   case 404: {
      get_lexval();
      expr(&ex2);
      get_lexval();
#line 1688 "grammar.acc"
 ex->Code = ex2.Code; 
      ex->Type = ex2.Type; ex->Storage = ex2.Storage; ex->Value = ex2.Value; 
# line 8359 "yygram.cpp"
      } break;
   case 405: {
      get_lexval();
      id = yylval;
#line 1691 "grammar.acc"
 BGlobalVar *b; BLocalVar *b2; BSysObj *b3; BResVar *b4; rID resID;
      resID = FIND(theModule->GetText(theRes->Name));
      ASSERT(resID);
      ASSERT(theEvent);
      if (b2 = GETBIND_LOCALVAR(id,resID,theEvent))
        {
          ex->Type = b2->VarType;
          ex->Storage = RT_MEMORY;
          ex->Value = -(b2->StackOffset);
        }
      else if (b4 = GETBIND_RESVAR(id,resID))
        {
          ex->Type = b4->VarType;
          ex->Storage = RT_MEMORY;
          ex->Value = b4->Address;
        }
      else if (b = GETBIND_GLOBALVAR(id))
        {
          ex->Type = b->VarType;
          ex->Storage = RT_MEMORY;
          ex->Value = b->Address;
        }
      else if (b3 = GETBIND_SYSOBJ(id))
        {
          ex->Type = DT_HOBJ;
          ex->Storage = RT_CONSTANT;
          ex->Value   = b3->ObjNum;
          ex->Code = NULL;
        }
      else
        {
          ex->Type = DT_INT32;
          ex->Storage = RT_CONSTANT; ex->Value = 0;
          if (IDENT_DEFINED(id))
            yyerror(Format("Improper use of identifier '%s'",theSymTab[id]));
          else
            yyerror(Format("Undeclared identifier '%s'",theSymTab[id]));

        }
    
# line 8405 "yygram.cpp"
      } break;
   case 406: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 1011: {
         get_lexval();
         } break;
      case 1012: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      id = yylval;
#line 1732 "grammar.acc"

      int8 reg; BMemVar *b; BResMem *b2;
      if (obj.Type == DT_RID) {
        if (!(b2 = GETBIND_RESMEM(id))) {
          yyerror(Format("Expected resource member in place of '%s'",theSymTab[id]));
          ex->Type = DT_VOID;
          ex->Storage = RT_CONSTANT;
          ex->Value = 0;
          }
        else {
          ex->Code = obj.Code;
          ex->Type = b2->VarType;
          if (obj.Storage == RT_REGISTER)
            FreeRegister(obj.Value);
          ex->Value = AllocRegister();
          ex->Storage = RT_REGISTER;
          if (!ex->Code)
            ex->Code = new VBlock;
          ex->Code->Generate(GVAR,obj.Storage,obj.Value,
            RT_CONSTANT, b2->varid);
          ex->Code->Generate(MOV,ex->Storage,ex->Value,RT_REGISTER,0);
          }
        }
      else {
        if (obj.Type != DT_HOBJ && obj.Type != DT_STRING && obj.Type != DT_HTEXT)
          yyerror("Type musmatch: non-hObj on left side of '.' operator.");
        if (!(b = GETBIND_MEMVAR(id))) {
          yyerror(Format("Expected Member in place of '%s'",theSymTab[id]));
          ex->Type = DT_VOID;
          ex->Storage = RT_CONSTANT;
          ex->Value = 0;
          }
        else {
          ex->Code = obj.Code;
          ex->Type = b->VarType;
          if (obj.Storage == RT_REGISTER)
            FreeRegister(obj.Value);

          if (!ex->Code)
            ex->Code = new VBlock;
          ex->Code->Generate(GVAR,obj.Storage,obj.Value,
            RT_CONSTANT, b->varid);
          if (ex->Type == DT_STRING) {
            ex->Value   = -AllocString();
            ex->Storage = RT_CONSTANT;
            ex->Code->Generate(WSTR,ex->Storage,ex->Value,RT_CONSTANT,-1);
            }
          else {
            ex->Value = AllocRegister();
            ex->Storage = RT_REGISTER;
            ex->Code->Generate(MOV,ex->Storage,ex->Value,RT_REGISTER,0);
            }
          }
        }
    
# line 8476 "yygram.cpp"
      } break;
   case 407: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 1013: {
         get_lexval();
         } break;
      case 1014: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      fn = yylval;
#line 1788 "grammar.acc"
 BSysFunc *b;
        if (obj.Type == DT_RID) {
          if (!(b = GETBIND_RESFUNC(fn)))
            yyerror(Format("Expected: member function name after '.' (%s).",
              theSymTab[fn]));
          }
        else if (obj.Type == DT_HOBJ || obj.Type == DT_STRING ||
                 obj.Type == DT_HTEXT) {
          if (!(b = GETBIND_MEMFUNC(fn)))
            yyerror(Format("Expected: member function name after '.' (%s).",
              theSymTab[fn]));
          }
        else
          yyerror("Type mismatch: non-hObj on left side of '.' operator.");
        ex->Code = new VBlock;
        /* Allocate a string, if necessary */
        if (obj.Type == DT_STRING || obj.Type == DT_HTEXT)
          if (obj.Storage == RT_CONSTANT && obj.Value >= 0)
            {
              int16 j = -AllocString();
              if (!obj.Code)
                obj.Code = new VBlock;
              obj.Code->Generate(WSTR,RT_CONSTANT,j,RT_CONSTANT,obj.Value);
              obj.Value = j;
            }          
      
# line 8518 "yygram.cpp"
      get_lexval();
      param_list(b, fn, &bl, &narg);
      get_lexval();
#line 1815 "grammar.acc"

        if (obj.Code)
          ex->Code->Add(obj.Code);
        ex->Code->Add(&bl);
        /* Adjust any stack offset to account for PUSH opcodes contained in

           bl. */
        if (obj.Storage == RT_MEMORY)
          if (obj.Value < 0)
            obj.Value -= narg;


        /* Call the Function */
        ex->Code->Generate(CMEM,obj.Storage,obj.Value,RT_CONSTANT,b->funcid);
        /* Clean up the Stack */
        ex->Code->Generate(DEC,RT_REGISTER,63,RT_CONSTANT,narg);
        ex->Type = b->ReturnType;
        if (ex->Type == DT_STRING) {
          ex->Value = -AllocString();
          ex->Storage = RT_CONSTANT;
          ex->Code->Generate(WSTR,ex->Storage,ex->Value,RT_CONSTANT,-1);
          }
        else {
          ex->Value = AllocRegister();
          ex->Storage = RT_REGISTER;
          ex->Code->Generate(MOV,RT_REGISTER,ex->Value,RT_REGISTER,0);
          }
      
# line 8551 "yygram.cpp"
      } break;
   case 408: {
      get_lexval();
      fn = yylval;
#line 1843 "grammar.acc"
 BSysFunc *b;
        if (!(b = GETBIND_SYSFUNC(fn)))
          yyerror(Format("Expected: global function name before '(' (%s).",
            theSymTab[fn]));
        ex->Code = new VBlock;
      
# line 8563 "yygram.cpp"
      get_lexval();
      param_list(b, fn, &bl, &narg);
      get_lexval();
#line 1850 "grammar.acc"

        ex->Code->Add(&bl);
        /* Call the Function */
        ex->Code->Generate(CMEM,RT_CONSTANT,0,RT_CONSTANT,b->funcid);
        /* Clean up the Stack */
        ex->Code->Generate(POP,RT_CONSTANT,narg);
        ex->Type = b->ReturnType;
        if (ex->Type == DT_STRING) {
          ex->Value = -AllocString();
          ex->Storage = RT_CONSTANT;
          ex->Code->Generate(WSTR,ex->Storage,ex->Value,RT_CONSTANT,-1);
          }
        else {
          ex->Value = AllocRegister();
          ex->Storage = RT_REGISTER;
          ex->Code->Generate(MOV,RT_REGISTER,ex->Value,RT_REGISTER,0);
          }
      
# line 8586 "yygram.cpp"
      } break;
   case 409: {
      fund_expr(&rect);
      get_lexval();
      get_lexval();
      id = yylval;
#line 1869 "grammar.acc"

      if (rect.Type != DT_RECT)
        yyerror("Type mismatch: '@' operator used on non-Rect target.");
      *ex = CodeRectMember(&rect,id);
      
    
# line 8600 "yygram.cpp"
      } break;
   }
return 0;
}

int lvalue (   LExp *lv)
{
   YYSTYPE id;
   LExp rect;
   PExp obj;
   switch(yyselect()) {
   case 410: {
      get_lexval();
      id = yylval;
#line 1879 "grammar.acc"
 BGlobalVar *b; BLocalVar *b2; BResVar *b3; rID resID;
      resID = FIND(theModule->GetText(theRes->Name));
      ASSERT(resID);
      ASSERT(theEvent);
      
      if (b2 = GETBIND_LOCALVAR(id,resID,theEvent))
        {
          lv->Type = b2->VarType;
          lv->Storage = RT_MEMORY;
          lv->Value = -(b2->StackOffset);
          lv->RCode = NULL;
          lv->WCode = new VBlock;
          lv->WCode->Generate(MOV,RT_MEMORY,-b2->StackOffset,RT_REGISTER,0);
        }
      else if (b3 = GETBIND_RESVAR(id,resID))
        {
          lv->Type = b3->VarType;
          lv->Storage = RT_MEMORY;
          lv->Value = b3->Address;
          lv->RCode = NULL;
          lv->WCode = new VBlock;
          lv->WCode->Generate(MOV,RT_MEMORY,b3->Address,RT_REGISTER,0);
        }
      else if (b = GETBIND_GLOBALVAR(id))
        {
          lv->Type = b->VarType;
          lv->Storage = RT_MEMORY;
          lv->Value = b->Address;
          lv->RCode = NULL;
          lv->WCode = new VBlock;
          lv->WCode->Generate(MOV,RT_MEMORY,b->Address,RT_REGISTER,0);
        }
      else
        {
          lv->Type = DT_UNKNOWN;
          lv->Storage = RT_MEMORY; 
          lv->Value = 0;
          if (IDENT_DEFINED(id))
            yyerror(Format("Improper use of identifier '%s'",theSymTab[id]));
          else
            yyerror(Format("Undeclared identifier '%s'",theSymTab[id]));

        }
    
# line 8659 "yygram.cpp"
      } break;
   case 411: {
      lvalue(&rect);
      get_lexval();
      get_lexval();
      id = yylval;
#line 1924 "grammar.acc"

      if (rect.Type != DT_RECT)
        yyerror("Type mismatch: '@' operator used on non-Rect target.");
      *lv = CodeRectLVal(&rect,id);
    
# line 8672 "yygram.cpp"
      } break;
   case 412: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 1015: {
         get_lexval();
         } break;
      case 1016: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      id = yylval;
#line 1930 "grammar.acc"

      int8 reg, i; BMemVar *b;
      if (obj.Type != DT_HOBJ)
        yyerror("Type mismatch: non-hObj on left side of '.' operator.");
      if (!(b = GETBIND_MEMVAR(id))) {
        yyerror(Format("Expected Member in place of '%s'",theSymTab[id]));
        lv->Type = DT_VOID;
        lv->Storage = RT_CONSTANT;
        lv->Value = 0;
        }
      else {
        lv->Type = b->VarType;
        if (obj.Storage == RT_REGISTER)
          FreeRegister(obj.Value);
        lv->RCode = new VBlock;
        lv->RCode->Add(obj.Code);
        lv->RCode->Generate(GVAR,obj.Storage,obj.Value,
          RT_CONSTANT, b->varid);

        if (lv->Type == DT_STRING) {
          i = -AllocString();
          lv->Storage = RT_CONSTANT;
          lv->Value   = i;
          lv->RCode->Generate(WSTR,RT_CONSTANT,i,RT_CONSTANT,-1);
          }
        else {
          lv->Value = AllocRegister();
          lv->Storage = RT_REGISTER;
          lv->RCode->Generate(MOV,lv->Storage,lv->Value,RT_REGISTER,0);      
          }
        lv->WCode = new VBlock;
        lv->WCode->Add(obj.Code);
        lv->WCode->Generate(MOV,lv->Storage,lv->Value,RT_REGISTER,0);
        lv->WCode->Generate(SVAR,obj.Storage,obj.Value,
          RT_CONSTANT, b->varid);
        }
    
# line 8725 "yygram.cpp"
      } break;
   }
return 0;
}

int expr10 (   PExp *ex)
{
   LExp lv;
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 413: {
      lvalue(&lv);
      get_lexval();
#line 1972 "grammar.acc"
 *ex = CodeAssignment('i',lv,NULL); 
# line 8741 "yygram.cpp"
      } break;
   case 414: {
      lvalue(&lv);
      get_lexval();
#line 1974 "grammar.acc"
 *ex = CodeAssignment('d',lv,NULL); 
# line 8748 "yygram.cpp"
      } break;
   case 415: {
      get_lexval();
      get_lexval();
      expr150(&ex1);
      get_lexval();
      expr150(&ex2);
      get_lexval();
#line 1976 "grammar.acc"
 *ex = CodeOperator('m',ex1,ex2); 
# line 8759 "yygram.cpp"
      } break;
   case 416: {
      get_lexval();
      get_lexval();
      expr150(&ex1);
      get_lexval();
      expr150(&ex2);
      get_lexval();
#line 1978 "grammar.acc"
 *ex = CodeOperator('M',ex1,ex2); 
# line 8770 "yygram.cpp"
      } break;
   case 417: {
      fund_expr(&ex1);
#line 1980 "grammar.acc"
 *ex = ex1; 
# line 8776 "yygram.cpp"
      } break;
   }
return 0;
}

int expr20 (   PExp *ex)
{
   LExp lv;
   PExp ex1;
   switch(yyselect()) {
   case 418: {
      get_lexval();
      lvalue(&lv);
#line 1985 "grammar.acc"
 *ex = CodeAssignment('I',lv,NULL); 
# line 8791 "yygram.cpp"
      } break;
   case 419: {
      get_lexval();
      lvalue(&lv);
#line 1987 "grammar.acc"
 *ex = CodeAssignment('D',lv,NULL); 
# line 8798 "yygram.cpp"
      } break;
   case 420: {
      get_lexval();
      expr20(&ex1);
#line 1989 "grammar.acc"
 *ex = ex1; 
# line 8805 "yygram.cpp"
      } break;
   case 421: {
      get_lexval();
      expr20(&ex1);
#line 1991 "grammar.acc"
 *ex = CodeOperator('-',ex1,ex1); 
# line 8812 "yygram.cpp"
      } break;
   case 422: {
      get_lexval();
      expr20(&ex1);
#line 1993 "grammar.acc"
 *ex = CodeOperator('!',ex1,ex1); 
# line 8819 "yygram.cpp"
      } break;
   case 423: {
      get_lexval();
      expr20(&ex1);
#line 1995 "grammar.acc"
 *ex = CodeOperator('~',ex1,ex1); 
# line 8826 "yygram.cpp"
      } break;
   case 424: {
      get_lexval();
      expr20(&ex1);
      get_lexval();
#line 1997 "grammar.acc"
 *ex = CodeOperator(':',ex1,ex1); 
# line 8834 "yygram.cpp"
      } break;
   case 425: {
      expr10(&ex1);
#line 1999 "grammar.acc"
 *ex = ex1; 
# line 8840 "yygram.cpp"
      } break;
   }
return 0;
}

int expr30 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 426: {
      expr20(&ex1);
      get_lexval();
      expr20(&ex2);
#line 2004 "grammar.acc"
 *ex = CodeOperator('d',ex1,ex2); 
# line 8856 "yygram.cpp"
      } break;
   case 427: {
      expr20(&ex1);
#line 2006 "grammar.acc"
 *ex = ex1; 
# line 8862 "yygram.cpp"
      } break;
   }
return 0;
}

int expr40 (   PExp *ex)
{
   YYSTYPE tn;
   PExp ex1;
   switch(yyselect()) {
   case 428: {
      get_lexval();
      type_name(&tn);
      get_lexval();
      expr40(&ex1);
#line 2011 "grammar.acc"
 *ex = ex1; ex->Type = tn; 
# line 8879 "yygram.cpp"
      } break;
   case 429: {
      expr30(&ex1);
#line 2013 "grammar.acc"
 *ex = ex1; 
# line 8885 "yygram.cpp"
      } break;
   }
return 0;
}

int expr50 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 430: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 2019 "grammar.acc"
 *ex = CodeOperator('*',ex1,ex2); 
# line 8901 "yygram.cpp"
      } break;
   case 431: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 2021 "grammar.acc"
 *ex = CodeOperator('/',ex1,ex2); 
# line 8909 "yygram.cpp"
      } break;
   case 432: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 2023 "grammar.acc"
 *ex = CodeOperator('%',ex1,ex2); 
# line 8917 "yygram.cpp"
      } break;
   case 433: {
      expr40(&ex1);
#line 2025 "grammar.acc"
 *ex = ex1; 
# line 8923 "yygram.cpp"
      } break;
   }
return 0;
}

int expr60 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 434: {
      expr60(&ex1);
      get_lexval();
      expr50(&ex2);
#line 2030 "grammar.acc"
 *ex = CodeOperator('+',ex1,ex2); 
# line 8939 "yygram.cpp"
      } break;
   case 435: {
      expr60(&ex1);
      get_lexval();
      expr50(&ex2);
#line 2032 "grammar.acc"
 *ex = CodeOperator('-',ex1,ex2); 
# line 8947 "yygram.cpp"
      } break;
   case 436: {
      expr50(&ex1);
#line 2034 "grammar.acc"
 *ex = ex1; 
# line 8953 "yygram.cpp"
      } break;
   }
return 0;
}

int expr70 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 437: {
      expr70(&ex1);
      get_lexval();
      expr60(&ex2);
#line 2039 "grammar.acc"
 *ex = CodeOperator(LSHIFT_OP,ex1,ex2); 
# line 8969 "yygram.cpp"
      } break;
   case 438: {
      expr70(&ex1);
      get_lexval();
      expr60(&ex2);
#line 2041 "grammar.acc"
 *ex = CodeOperator(RSHIFT_OP,ex1,ex2); 
# line 8977 "yygram.cpp"
      } break;
   case 439: {
      expr60(&ex1);
#line 2043 "grammar.acc"
 *ex = ex1; 
# line 8983 "yygram.cpp"
      } break;
   }
return 0;
}

int expr80 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 440: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2052 "grammar.acc"
 *ex = CodeOperator('<',ex1,ex2); 
# line 8999 "yygram.cpp"
      } break;
   case 441: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2054 "grammar.acc"
 *ex = CodeOperator('>',ex1,ex2); 
# line 9007 "yygram.cpp"
      } break;
   case 442: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2056 "grammar.acc"
 *ex = CodeOperator(GE_OP,ex1,ex2); 
# line 9015 "yygram.cpp"
      } break;
   case 443: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2058 "grammar.acc"
 *ex = CodeOperator(LE_OP,ex1,ex2); 
# line 9023 "yygram.cpp"
      } break;
   case 444: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2060 "grammar.acc"
 *ex = CodeOperator(EQ_OP,ex1,ex2); 
# line 9031 "yygram.cpp"
      } break;
   case 445: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2062 "grammar.acc"
 *ex = CodeOperator(NE_OP,ex1,ex2); 
# line 9039 "yygram.cpp"
      } break;
   case 446: {
      expr70(&ex1);
#line 2064 "grammar.acc"
 *ex = ex1; 
# line 9045 "yygram.cpp"
      } break;
   }
return 0;
}

int expr90 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 447: {
      expr90(&ex1);
      get_lexval();
      expr80(&ex2);
#line 2069 "grammar.acc"
 *ex = CodeOperator('&',ex1,ex2); 
# line 9061 "yygram.cpp"
      } break;
   case 448: {
      expr80(&ex1);
#line 2071 "grammar.acc"
 *ex = ex1; 
# line 9067 "yygram.cpp"
      } break;
   }
return 0;
}

int expr100 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 449: {
      expr100(&ex1);
      get_lexval();
      expr90(&ex2);
#line 2076 "grammar.acc"
 *ex = CodeOperator('^',ex1,ex2); 
# line 9083 "yygram.cpp"
      } break;
   case 450: {
      expr90(&ex1);
#line 2078 "grammar.acc"
 *ex = ex1; 
# line 9089 "yygram.cpp"
      } break;
   }
return 0;
}

int expr110 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 451: {
      expr110(&ex1);
      get_lexval();
      expr100(&ex2);
#line 2083 "grammar.acc"
 *ex = CodeOperator('|',ex1,ex2); 
# line 9105 "yygram.cpp"
      } break;
   case 452: {
      expr100(&ex1);
#line 2085 "grammar.acc"
 *ex = ex1; 
# line 9111 "yygram.cpp"
      } break;
   }
return 0;
}

int expr120 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 453: {
      expr120(&ex1);
      get_lexval();
      expr110(&ex2);
#line 2090 "grammar.acc"
 *ex = CodeOperator(AND_OP,ex1,ex2); 
# line 9127 "yygram.cpp"
      } break;
   case 454: {
      expr110(&ex1);
#line 2092 "grammar.acc"
 *ex = ex1; 
# line 9133 "yygram.cpp"
      } break;
   }
return 0;
}

int expr130 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 455: {
      expr130(&ex1);
      get_lexval();
      expr120(&ex2);
#line 2097 "grammar.acc"
 *ex = CodeOperator(OR_OP,ex1,ex2); 
# line 9149 "yygram.cpp"
      } break;
   case 456: {
      expr120(&ex1);
#line 2099 "grammar.acc"
 *ex = ex1; 
# line 9155 "yygram.cpp"
      } break;
   }
return 0;
}

int expr140 (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   PExp ex3;
   switch(yyselect()) {
   case 457: {
      expr130(&ex1);
      get_lexval();
      expr(&ex2);
      get_lexval();
      expr140(&ex3);
#line 2104 "grammar.acc"

      VBlock bl;
      if (!AllowedCast(ex1.Type,DT_BOOL))
        yyerror("Type mismatch: first operand of conditional operator "
                "must cast to bool.");
      if (!AllowedCast(ex2.Type,ex3.Type))
        yyerror("Type mismatch: conditional operator with incompatible "
                "types for true and false.");
      ex->Type = ex2.Type;
      ex->Code = new VBlock;
      ex->Code->Add(ex1.Code);
      ex->Code->Add(ex2.Code);
      ex->Code->Add(ex3.Code);
      
      if (ex2.Type == DT_STRING || ex3.Type == DT_STRING) {
        ex->Storage = RT_CONSTANT;
        ex->Value   = -AllocString();
        bl.Clear();
        ex->Code->Generate(CMEQ,ex1.Storage,ex1.Value,RT_CONSTANT,0);
        bl.Generate(WSTR, ex->Storage,ex->Value,ex2.Storage,ex2.Value);
        ex->Code->Generate(JTRU,RT_REGISTER, 0, RT_CONSTANT,2 + bl.GetSize());
        ex->Code->Add(&bl);
        bl.Clear();
        bl.Generate(WSTR, ex->Storage,ex->Value,ex3.Storage,ex3.Value);        
        ex->Code->Generate(JUMP,RT_CONSTANT,1 + bl.GetSize());
        ex->Code->Add(&bl);
        if (ex2.Storage == RT_CONSTANT && ex2.Type == DT_STRING && ex2.Value < -1)
          FreeString(-ex2.Value);
        if (ex3.Storage == RT_CONSTANT && ex3.Type == DT_STRING && ex3.Value < -1)
          FreeString(-ex3.Value);
        }
      else {
        ex->Storage = RT_REGISTER;
        ex->Value   = AllocRegister();
        bl.Clear();
        ex->Code->Generate(CMEQ,ex1.Storage,ex1.Value,RT_CONSTANT,0);
        bl.Generate(MOV, ex->Storage,ex->Value,ex2.Storage,ex2.Value);
        ex->Code->Generate(JTRU,RT_REGISTER, 0, RT_CONSTANT,2 + bl.GetSize());
        ex->Code->Add(&bl);
        bl.Clear();
        bl.Generate(MOV, ex->Storage,ex->Value,ex3.Storage,ex3.Value);        
        ex->Code->Generate(JUMP,RT_CONSTANT,1 + bl.GetSize());
        ex->Code->Add(&bl);
        }        
      if (ex2.Storage == RT_REGISTER)
        FreeRegister(ex2.Value);
      if (ex3.Storage == RT_REGISTER)
        FreeRegister(ex3.Value);
    
# line 9222 "yygram.cpp"
      } break;
   case 458: {
      expr130(&ex1);
#line 2154 "grammar.acc"
 *ex = ex1; 
# line 9228 "yygram.cpp"
      } break;
   }
return 0;
}

int expr150 (   PExp *ex)
{
   LExp lv;
   PExp ex2;
   PExp ex1;
   switch(yyselect()) {
   case 459: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2162 "grammar.acc"
 *ex = CodeAssignment('=',lv,&ex2); 
# line 9245 "yygram.cpp"
      } break;
   case 460: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2164 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_ADD,lv,&ex2); 
# line 9253 "yygram.cpp"
      } break;
   case 461: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2166 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_SUB,lv,&ex2); 
# line 9261 "yygram.cpp"
      } break;
   case 462: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2168 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_MULT,lv,&ex2); 
# line 9269 "yygram.cpp"
      } break;
   case 463: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2170 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_DIV,lv,&ex2); 
# line 9277 "yygram.cpp"
      } break;
   case 464: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2172 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_MOD,lv,&ex2); 
# line 9285 "yygram.cpp"
      } break;
   case 465: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2174 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_AND,lv,&ex2); 
# line 9293 "yygram.cpp"
      } break;
   case 466: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2176 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_OR,lv,&ex2); 
# line 9301 "yygram.cpp"
      } break;
   case 467: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2178 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_LSHIFT,lv,&ex2); 
# line 9309 "yygram.cpp"
      } break;
   case 468: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2180 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_RSHIFT,lv,&ex2); 
# line 9317 "yygram.cpp"
      } break;
   case 469: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2182 "grammar.acc"
 *ex = CodeAssignment(ASSIGN_NEG,lv,&ex2); 
# line 9325 "yygram.cpp"
      } break;
   case 470: {
      expr140(&ex1);
#line 2184 "grammar.acc"
 *ex = ex1; 
# line 9331 "yygram.cpp"
      } break;
   }
return 0;
}

int expr (   PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 471: {
      expr(&ex1);
      get_lexval();
      expr150(&ex2);
#line 2189 "grammar.acc"

      ex->Code = ex1.Code;
      if (!ex->Code)
        ex->Code = new VBlock;
      if (ex2.Code) {
        ex->Code->Add(ex2.Code);
        delete ex2.Code;
        }
      ex->Storage = ex1.Storage;
      ex->Value   = ex1.Value;
      ex->Type    = ex1.Type;
    
# line 9358 "yygram.cpp"
      } break;
   case 472: {
      expr150(&ex1);
#line 2202 "grammar.acc"
 *ex = ex1; 
# line 9364 "yygram.cpp"
      } break;
   }
return 0;
}

int param_list (   pBSysFunc b,    YYSTYPE fn,    VBlock *bl,    int *narg)
{
   PExp p;
   switch(yyselect()) {
   case 473: {
#line 2210 "grammar.acc"
 *narg = 0; int16 i,j; bl->Clear();
    int16 PStorage[10]; int32 PValue[10]; 
# line 9377 "yygram.cpp"
      switch (yyselect()) {
      case 1018: {
         expr150(&p);
#line 2213 "grammar.acc"

      if (!b)
        ;
      else {
        if (*narg < b->ParamCount)
          if (!AllowedCast(p.Type,b->ParamType[*narg]))
            yyerror(Format("Type mismatch in parameter %d of call to function %s (%s/%s).",
              (*narg)+1, theSymTab[fn], Lookup(DataTypeNames, b->ParamType[*narg]),
                Lookup(DataTypeNames,p.Type))); 
        bl->Add(p.Code);
        if (p.Type == DT_STRING && p.Storage == RT_CONSTANT)
          if (p.Value < 0)
            FreeString(-p.Value);
        if (p.Storage == RT_MEMORY && p.Value < 0) {
          j = AllocRegister();
          bl->Generate(MOV,RT_REGISTER,j,p.Storage,p.Value);
          PStorage[*narg] = RT_REGISTER;
          PValue[*narg] = j;
          }
        else { 
          PStorage[*narg] = p.Storage;
          PValue[*narg]   = p.Value;
          }
        (*narg)++;
        }
    
# line 9408 "yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 1017: {
               get_lexval();
               expr150(&p);
#line 2240 "grammar.acc"

      if (!b)
        ;
      else if (*narg == 10)
        yyerror("More than maximum of 10 parameters in function call!");
      else if (*narg > 10)
        ;
      else {
        if (*narg < b->ParamCount)
          if (!AllowedCast(p.Type,b->ParamType[*narg]))
            yyerror(Format("Type mismatch in parameter %d of call to function %s (%s/%s).",
              narg+1, theSymTab[fn], Lookup(DataTypeNames, b->ParamType[*narg]),
                Lookup(DataTypeNames,p.Type))); 
        if (*narg > b->ParamCount && !b->isVarParam)
          yyerror(Format("Too many parameters in call to function %s.",theSymTab[fn]));
        else {
          bl->Add(p.Code);
          if (p.Type == DT_STRING && p.Storage == RT_CONSTANT)
            if (p.Value < 0)
              FreeString(-p.Value);
          if (p.Storage == RT_MEMORY && p.Value < 0) {
            j = AllocRegister();
            bl->Generate(MOV,RT_REGISTER,j,p.Storage,p.Value);
            PStorage[*narg] = RT_REGISTER;
            PValue[*narg] = j;
            }
          else { 
            PStorage[*narg] = p.Storage;
            PValue[*narg]   = p.Value;
            }
          }
        (*narg)++;
        }
    
# line 9449 "yygram.cpp"
               } break;
            default: goto yy106;
            }
         }
         yy106: ;
         } break;
      default: ;
      }
#line 2275 "grammar.acc"

      if ((*narg) <= b->ParamCount)
        for(i=b->ParamCount;i!=(*narg);i--) {
          if (b->HasDefault & BIT(i+1)) {
            bl->Generate(PUSH,RT_CONSTANT,b->Default[i]);
            }
          else {
            yyerror(Format("Too few parameters in call to function %s.",theSymTab[fn]));
            goto Done;
            }
          }
      for(i=(*narg)-1;i!=-1;i--) {
        bl->Generate(PUSH,PStorage[i],PValue[i]);
        if (PStorage[i] == RT_REGISTER)
          FreeRegister(PValue[i]);
        }
      *narg = max(*narg,b->ParamCount);
      Done:;
      
# line 9478 "yygram.cpp"
      } break;
   }
return 0;
}

extern YYSTYPE yylval;
YYSTYPE yylval;
extern long yypos;

typedef struct LEXELEMSTRUCT {
   YYSTYPE val;
   long pos;
   #ifdef TRACK_FILE
   char file[32];
   #endif
   struct LEXELEMSTRUCT *next;
} LEXELEM;
   
LEXELEM *first_lexelem, *cur_lexelem;

void init_lexelem()
{
   cur_lexelem = first_lexelem;
}

void first_lexval () {
   LEXELEM *p;
   p = (LEXELEM *)malloc(sizeof(LEXELEM));
   if (! p) yymallocerror();
   p->val = yylval;
   p->pos = yypos;
   #ifdef TRACK_FILE
     strncpy (p->file,preprocFilename,31);
   #endif
   p->next = 0;
   cur_lexelem = p;
   first_lexelem = p;
}

void next_lexval() {
   LEXELEM *p;
   p = (LEXELEM *)malloc(sizeof(LEXELEM));
   if (! p) yymallocerror();
   cur_lexelem-> next = p;
   p->val = yylval;
   p->pos = yypos;
   #ifdef TRACK_FILE
     strncpy (p->file,preprocFilename,31);
   #endif
   p->next = 0;
   cur_lexelem = p;
}

void get_lexval() {
   yylval = cur_lexelem->val;
   yypos = cur_lexelem->pos;
   #ifdef TRACK_FILE
     strncpy (preprocFilename,cur_lexelem->file,31);
   #endif
   cur_lexelem = cur_lexelem->next;
}

extern int c_length;
int c_length = 6729;
extern int yygrammar[];
int yygrammar[] = {
0,
/* 1 */ 0,
/* 2 */ 6,
/* 3 */ 50000,
/* 4 */ -1,
/* 5 */ 474,
/* 6 */ 0,
/* 7 */ 10,
/* 8 */ -6,
/* 9 */ 1,
/* 10 */ 15,
/* 11 */ 18,
/* 12 */ 10,
/* 13 */ -10,
/* 14 */ 475,
/* 15 */ 0,
/* 16 */ -10,
/* 17 */ 1019,
/* 18 */ 24,
/* 19 */ 50379,
/* 20 */ 50257,
/* 21 */ 50059,
/* 22 */ -18,
/* 23 */ 2,
/* 24 */ 30,
/* 25 */ 50432,
/* 26 */ 223,
/* 27 */ 50059,
/* 28 */ -18,
/* 29 */ 3,
/* 30 */ 36,
/* 31 */ 50351,
/* 32 */ 50257,
/* 33 */ 50059,
/* 34 */ -18,
/* 35 */ 4,
/* 36 */ 40,
/* 37 */ 652,
/* 38 */ -18,
/* 39 */ 5,
/* 40 */ 44,
/* 41 */ 1431,
/* 42 */ -18,
/* 43 */ 6,
/* 44 */ 48,
/* 45 */ 2085,
/* 46 */ -18,
/* 47 */ 7,
/* 48 */ 52,
/* 49 */ 3583,
/* 50 */ -18,
/* 51 */ 8,
/* 52 */ 56,
/* 53 */ 3381,
/* 54 */ -18,
/* 55 */ 9,
/* 56 */ 60,
/* 57 */ 5320,
/* 58 */ -18,
/* 59 */ 10,
/* 60 */ 64,
/* 61 */ 2260,
/* 62 */ -18,
/* 63 */ 11,
/* 64 */ 68,
/* 65 */ 2697,
/* 66 */ -18,
/* 67 */ 12,
/* 68 */ 72,
/* 69 */ 2954,
/* 70 */ -18,
/* 71 */ 13,
/* 72 */ 76,
/* 73 */ 4015,
/* 74 */ -18,
/* 75 */ 14,
/* 76 */ 80,
/* 77 */ 3911,
/* 78 */ -18,
/* 79 */ 15,
/* 80 */ 84,
/* 81 */ 4279,
/* 82 */ -18,
/* 83 */ 16,
/* 84 */ 88,
/* 85 */ 4156,
/* 86 */ -18,
/* 87 */ 17,
/* 88 */ 92,
/* 89 */ 4596,
/* 90 */ -18,
/* 91 */ 18,
/* 92 */ 96,
/* 93 */ 5324,
/* 94 */ -18,
/* 95 */ 19,
/* 96 */ 100,
/* 97 */ 5689,
/* 98 */ -18,
/* 99 */ 20,
/* 100 */ 104,
/* 101 */ 5399,
/* 102 */ -18,
/* 103 */ 21,
/* 104 */ 108,
/* 105 */ 5822,
/* 106 */ -18,
/* 107 */ 22,
/* 108 */ 112,
/* 109 */ 5854,
/* 110 */ -18,
/* 111 */ 23,
/* 112 */ 116,
/* 113 */ 5944,
/* 114 */ -18,
/* 115 */ 24,
/* 116 */ 120,
/* 117 */ 6050,
/* 118 */ -18,
/* 119 */ 25,
/* 120 */ 124,
/* 121 */ 6032,
/* 122 */ -18,
/* 123 */ 26,
/* 124 */ 128,
/* 125 */ 6041,
/* 126 */ -18,
/* 127 */ 27,
/* 128 */ 134,
/* 129 */ 50349,
/* 130 */ 144,
/* 131 */ 173,
/* 132 */ -18,
/* 133 */ 28,
/* 134 */ 0,
/* 135 */ 50450,
/* 136 */ 50257,
/* 137 */ 50123,
/* 138 */ 148,
/* 139 */ 50257,
/* 140 */ 156,
/* 141 */ 50125,
/* 142 */ -18,
/* 143 */ 29,
/* 144 */ 0,
/* 145 */ 50058,
/* 146 */ -144,
/* 147 */ 476,
/* 148 */ 153,
/* 149 */ 50325,
/* 150 */ 50058,
/* 151 */ -148,
/* 152 */ 477,
/* 153 */ 0,
/* 154 */ -148,
/* 155 */ 1020,
/* 156 */ 160,
/* 157 */ 50059,
/* 158 */ -156,
/* 159 */ 478,
/* 160 */ 0,
/* 161 */ -156,
/* 162 */ 1021,
/* 163 */ 168,
/* 164 */ 50036,
/* 165 */ 50257,
/* 166 */ -163,
/* 167 */ 30,
/* 168 */ 0,
/* 169 */ 50036,
/* 170 */ 223,
/* 171 */ -163,
/* 172 */ 31,
/* 173 */ 179,
/* 174 */ 173,
/* 175 */ 50043,
/* 176 */ 189,
/* 177 */ -173,
/* 178 */ 32,
/* 179 */ 185,
/* 180 */ 173,
/* 181 */ 50045,
/* 182 */ 189,
/* 183 */ -173,
/* 184 */ 33,
/* 185 */ 0,
/* 186 */ 189,
/* 187 */ -173,
/* 188 */ 34,
/* 189 */ 195,
/* 190 */ 189,
/* 191 */ 50042,
/* 192 */ 223,
/* 193 */ -189,
/* 194 */ 35,
/* 195 */ 201,
/* 196 */ 189,
/* 197 */ 50047,
/* 198 */ 223,
/* 199 */ -189,
/* 200 */ 36,
/* 201 */ 207,
/* 202 */ 189,
/* 203 */ 50037,
/* 204 */ 223,
/* 205 */ -189,
/* 206 */ 37,
/* 207 */ 213,
/* 208 */ 189,
/* 209 */ 50124,
/* 210 */ 223,
/* 211 */ -189,
/* 212 */ 38,
/* 213 */ 219,
/* 214 */ 189,
/* 215 */ 50038,
/* 216 */ 223,
/* 217 */ -189,
/* 218 */ 39,
/* 219 */ 0,
/* 220 */ 223,
/* 221 */ -189,
/* 222 */ 40,
/* 223 */ 228,
/* 224 */ 50043,
/* 225 */ 237,
/* 226 */ -223,
/* 227 */ 41,
/* 228 */ 233,
/* 229 */ 50045,
/* 230 */ 237,
/* 231 */ -223,
/* 232 */ 42,
/* 233 */ 0,
/* 234 */ 237,
/* 235 */ -223,
/* 236 */ 43,
/* 237 */ 243,
/* 238 */ 50040,
/* 239 */ 173,
/* 240 */ 50041,
/* 241 */ -237,
/* 242 */ 44,
/* 243 */ 247,
/* 244 */ 50256,
/* 245 */ -237,
/* 246 */ 45,
/* 247 */ 251,
/* 248 */ 50457,
/* 249 */ -237,
/* 250 */ 46,
/* 251 */ 255,
/* 252 */ 50344,
/* 253 */ -237,
/* 254 */ 47,
/* 255 */ 0,
/* 256 */ 50259,
/* 257 */ -237,
/* 258 */ 48,
/* 259 */ 267,
/* 260 */ 237,
/* 261 */ 50100,
/* 262 */ 237,
/* 263 */ 50043,
/* 264 */ 237,
/* 265 */ -259,
/* 266 */ 49,
/* 267 */ 276,
/* 268 */ 237,
/* 269 */ 50100,
/* 270 */ 237,
/* 271 */ 50043,
/* 272 */ 50045,
/* 273 */ 237,
/* 274 */ -259,
/* 275 */ 50,
/* 276 */ 282,
/* 277 */ 237,
/* 278 */ 50100,
/* 279 */ 237,
/* 280 */ -259,
/* 281 */ 51,
/* 282 */ 0,
/* 283 */ 223,
/* 284 */ -259,
/* 285 */ 52,
/* 286 */ 294,
/* 287 */ 237,
/* 288 */ 50100,
/* 289 */ 237,
/* 290 */ 50043,
/* 291 */ 237,
/* 292 */ -286,
/* 293 */ 53,
/* 294 */ 0,
/* 295 */ 237,
/* 296 */ 50100,
/* 297 */ 237,
/* 298 */ -286,
/* 299 */ 54,
/* 300 */ 0,
/* 301 */ 310,
/* 302 */ 320,
/* 303 */ 328,
/* 304 */ 335,
/* 305 */ 346,
/* 306 */ 355,
/* 307 */ 368,
/* 308 */ -300,
/* 309 */ 55,
/* 310 */ 317,
/* 311 */ 50366,
/* 312 */ 50040,
/* 313 */ 588,
/* 314 */ 50041,
/* 315 */ -310,
/* 316 */ 479,
/* 317 */ 0,
/* 318 */ -310,
/* 319 */ 1022,
/* 320 */ 325,
/* 321 */ 50256,
/* 322 */ 50037,
/* 323 */ -320,
/* 324 */ 480,
/* 325 */ 0,
/* 326 */ -320,
/* 327 */ 1023,
/* 328 */ 332,
/* 329 */ 286,
/* 330 */ -328,
/* 331 */ 481,
/* 332 */ 0,
/* 333 */ -328,
/* 334 */ 1024,
/* 335 */ 339,
/* 336 */ 50316,
/* 337 */ -335,
/* 338 */ 482,
/* 339 */ 343,
/* 340 */ 50299,
/* 341 */ -335,
/* 342 */ 483,
/* 343 */ 0,
/* 344 */ -335,
/* 345 */ 1025,
/* 346 */ 350,
/* 347 */ 163,
/* 348 */ -346,
/* 349 */ 484,
/* 350 */ 0,
/* 351 */ 50418,
/* 352 */ 223,
/* 353 */ -346,
/* 354 */ 485,
/* 355 */ 360,
/* 356 */ 50043,
/* 357 */ 50256,
/* 358 */ -355,
/* 359 */ 486,
/* 360 */ 365,
/* 361 */ 50045,
/* 362 */ 50256,
/* 363 */ -355,
/* 364 */ 487,
/* 365 */ 0,
/* 366 */ -355,
/* 367 */ 1026,
/* 368 */ 376,
/* 369 */ 50470,
/* 370 */ 50091,
/* 371 */ 417,
/* 372 */ 50093,
/* 373 */ 368,
/* 374 */ -368,
/* 375 */ 490,
/* 376 */ 382,
/* 377 */ 50398,
/* 378 */ 163,
/* 379 */ 368,
/* 380 */ -368,
/* 381 */ 491,
/* 382 */ 391,
/* 383 */ 50398,
/* 384 */ 50418,
/* 385 */ 223,
/* 386 */ 50456,
/* 387 */ 223,
/* 388 */ 368,
/* 389 */ -368,
/* 390 */ 492,
/* 391 */ 398,
/* 392 */ 50398,
/* 393 */ 50418,
/* 394 */ 223,
/* 395 */ 368,
/* 396 */ -368,
/* 397 */ 493,
/* 398 */ 404,
/* 399 */ 50398,
/* 400 */ 50418,
/* 401 */ 368,
/* 402 */ -368,
/* 403 */ 494,
/* 404 */ 414,
/* 405 */ 50294,
/* 406 */ 50040,
/* 407 */ 223,
/* 408 */ 50044,
/* 409 */ 223,
/* 410 */ 50041,
/* 411 */ 368,
/* 412 */ -368,
/* 413 */ 495,
/* 414 */ 0,
/* 415 */ -368,
/* 416 */ 1029,
/* 417 */ 423,
/* 418 */ 173,
/* 419 */ 426,
/* 420 */ 417,
/* 421 */ -417,
/* 422 */ 489,
/* 423 */ 0,
/* 424 */ -417,
/* 425 */ 1028,
/* 426 */ 430,
/* 427 */ 50044,
/* 428 */ -426,
/* 429 */ 488,
/* 430 */ 0,
/* 431 */ -426,
/* 432 */ 1027,
/* 433 */ 0,
/* 434 */ 50355,
/* 435 */ 441,
/* 436 */ 445,
/* 437 */ 300,
/* 438 */ 50059,
/* 439 */ -433,
/* 440 */ 56,
/* 441 */ 0,
/* 442 */ 50058,
/* 443 */ -441,
/* 444 */ 496,
/* 445 */ 451,
/* 446 */ 300,
/* 447 */ 454,
/* 448 */ 445,
/* 449 */ -445,
/* 450 */ 499,
/* 451 */ 0,
/* 452 */ -445,
/* 453 */ 1030,
/* 454 */ 458,
/* 455 */ 50044,
/* 456 */ -454,
/* 457 */ 497,
/* 458 */ 0,
/* 459 */ 50334,
/* 460 */ -454,
/* 461 */ 498,
/* 462 */ 0,
/* 463 */ 471,
/* 464 */ 479,
/* 465 */ 486,
/* 466 */ 493,
/* 467 */ 501,
/* 468 */ 50059,
/* 469 */ -462,
/* 470 */ 57,
/* 471 */ 475,
/* 472 */ 50367,
/* 473 */ -471,
/* 474 */ 500,
/* 475 */ 0,
/* 476 */ 50356,
/* 477 */ -471,
/* 478 */ 501,
/* 479 */ 483,
/* 480 */ 50058,
/* 481 */ -479,
/* 482 */ 502,
/* 483 */ 0,
/* 484 */ -479,
/* 485 */ 1031,
/* 486 */ 490,
/* 487 */ 1247,
/* 488 */ -486,
/* 489 */ 503,
/* 490 */ 0,
/* 491 */ -486,
/* 492 */ 1032,
/* 493 */ 497,
/* 494 */ 50259,
/* 495 */ -493,
/* 496 */ 504,
/* 497 */ 0,
/* 498 */ 50256,
/* 499 */ -493,
/* 500 */ 505,
/* 501 */ 506,
/* 502 */ 50399,
/* 503 */ 1247,
/* 504 */ -501,
/* 505 */ 506,
/* 506 */ 0,
/* 507 */ -501,
/* 508 */ 1033,
/* 509 */ 0,
/* 510 */ 515,
/* 511 */ 522,
/* 512 */ 530,
/* 513 */ -509,
/* 514 */ 58,
/* 515 */ 519,
/* 516 */ 1247,
/* 517 */ -515,
/* 518 */ 507,
/* 519 */ 0,
/* 520 */ -515,
/* 521 */ 1034,
/* 522 */ 526,
/* 523 */ 50259,
/* 524 */ -522,
/* 525 */ 508,
/* 526 */ 0,
/* 527 */ 50256,
/* 528 */ -522,
/* 529 */ 509,
/* 530 */ 535,
/* 531 */ 50399,
/* 532 */ 1247,
/* 533 */ -530,
/* 534 */ 510,
/* 535 */ 0,
/* 536 */ -530,
/* 537 */ 1035,
/* 538 */ 0,
/* 539 */ 50399,
/* 540 */ 50341,
/* 541 */ 555,
/* 542 */ 546,
/* 543 */ 50059,
/* 544 */ -538,
/* 545 */ 59,
/* 546 */ 552,
/* 547 */ 50044,
/* 548 */ 555,
/* 549 */ 546,
/* 550 */ -546,
/* 551 */ 511,
/* 552 */ 0,
/* 553 */ -546,
/* 554 */ 1036,
/* 555 */ 0,
/* 556 */ 173,
/* 557 */ 561,
/* 558 */ 570,
/* 559 */ -555,
/* 560 */ 60,
/* 561 */ 567,
/* 562 */ 50044,
/* 563 */ 173,
/* 564 */ 561,
/* 565 */ -561,
/* 566 */ 512,
/* 567 */ 0,
/* 568 */ -561,
/* 569 */ 1037,
/* 570 */ 575,
/* 571 */ 50257,
/* 572 */ 579,
/* 573 */ -570,
/* 574 */ 514,
/* 575 */ 0,
/* 576 */ 6245,
/* 577 */ -570,
/* 578 */ 515,
/* 579 */ 585,
/* 580 */ 50047,
/* 581 */ 50257,
/* 582 */ 579,
/* 583 */ -579,
/* 584 */ 513,
/* 585 */ 0,
/* 586 */ -579,
/* 587 */ 1038,
/* 588 */ 0,
/* 589 */ 6699,
/* 590 */ -588,
/* 591 */ 61,
/* 592 */ 0,
/* 593 */ 6098,
/* 594 */ -592,
/* 595 */ 62,
/* 596 */ 0,
/* 597 */ 601,
/* 598 */ 628,
/* 599 */ -596,
/* 600 */ 63,
/* 601 */ 606,
/* 602 */ 621,
/* 603 */ 237,
/* 604 */ -601,
/* 605 */ 517,
/* 606 */ 611,
/* 607 */ 50045,
/* 608 */ 237,
/* 609 */ -601,
/* 610 */ 518,
/* 611 */ 616,
/* 612 */ 50061,
/* 613 */ 237,
/* 614 */ -601,
/* 615 */ 519,
/* 616 */ 0,
/* 617 */ 223,
/* 618 */ 50037,
/* 619 */ -601,
/* 620 */ 520,
/* 621 */ 625,
/* 622 */ 50043,
/* 623 */ -621,
/* 624 */ 516,
/* 625 */ 0,
/* 626 */ -621,
/* 627 */ 1039,
/* 628 */ 634,
/* 629 */ 50040,
/* 630 */ 637,
/* 631 */ 50041,
/* 632 */ -628,
/* 633 */ 524,
/* 634 */ 0,
/* 635 */ -628,
/* 636 */ 1040,
/* 637 */ 642,
/* 638 */ 50386,
/* 639 */ 237,
/* 640 */ -637,
/* 641 */ 521,
/* 642 */ 647,
/* 643 */ 50383,
/* 644 */ 237,
/* 645 */ -637,
/* 646 */ 522,
/* 647 */ 0,
/* 648 */ 50390,
/* 649 */ 237,
/* 650 */ -637,
/* 651 */ 523,
/* 652 */ 0,
/* 653 */ 50387,
/* 654 */ 50257,
/* 655 */ 50058,
/* 656 */ 223,
/* 657 */ 663,
/* 658 */ 50123,
/* 659 */ 680,
/* 660 */ 50125,
/* 661 */ -652,
/* 662 */ 64,
/* 663 */ 669,
/* 664 */ 50044,
/* 665 */ 223,
/* 666 */ 672,
/* 667 */ -663,
/* 668 */ 526,
/* 669 */ 0,
/* 670 */ -663,
/* 671 */ 1042,
/* 672 */ 677,
/* 673 */ 50044,
/* 674 */ 223,
/* 675 */ -672,
/* 676 */ 525,
/* 677 */ 0,
/* 678 */ -672,
/* 679 */ 1041,
/* 680 */ 685,
/* 681 */ 688,
/* 682 */ 680,
/* 683 */ -680,
/* 684 */ 527,
/* 685 */ 0,
/* 686 */ -680,
/* 687 */ 1043,
/* 688 */ 695,
/* 689 */ 50325,
/* 690 */ 932,
/* 691 */ 50257,
/* 692 */ 50059,
/* 693 */ -688,
/* 694 */ 65,
/* 695 */ 702,
/* 696 */ 50324,
/* 697 */ 939,
/* 698 */ 223,
/* 699 */ 50059,
/* 700 */ -688,
/* 701 */ 66,
/* 702 */ 709,
/* 703 */ 946,
/* 704 */ 954,
/* 705 */ 223,
/* 706 */ 50059,
/* 707 */ -688,
/* 708 */ 67,
/* 709 */ 718,
/* 710 */ 961,
/* 711 */ 969,
/* 712 */ 223,
/* 713 */ 50047,
/* 714 */ 223,
/* 715 */ 50059,
/* 716 */ -688,
/* 717 */ 68,
/* 718 */ 726,
/* 719 */ 50449,
/* 720 */ 976,
/* 721 */ 173,
/* 722 */ 983,
/* 723 */ 50059,
/* 724 */ -688,
/* 725 */ 69,
/* 726 */ 733,
/* 727 */ 50363,
/* 728 */ 992,
/* 729 */ 173,
/* 730 */ 50059,
/* 731 */ -688,
/* 732 */ 70,
/* 733 */ 740,
/* 734 */ 50364,
/* 735 */ 999,
/* 736 */ 173,
/* 737 */ 50059,
/* 738 */ -688,
/* 739 */ 71,
/* 740 */ 748,
/* 741 */ 1006,
/* 742 */ 50365,
/* 743 */ 1013,
/* 744 */ 173,
/* 745 */ 50059,
/* 746 */ -688,
/* 747 */ 72,
/* 748 */ 752,
/* 749 */ 462,
/* 750 */ -688,
/* 751 */ 73,
/* 752 */ 760,
/* 753 */ 50439,
/* 754 */ 50091,
/* 755 */ 223,
/* 756 */ 50093,
/* 757 */ 50059,
/* 758 */ -688,
/* 759 */ 74,
/* 760 */ 770,
/* 761 */ 50439,
/* 762 */ 50091,
/* 763 */ 223,
/* 764 */ 50044,
/* 765 */ 223,
/* 766 */ 50093,
/* 767 */ 50059,
/* 768 */ -688,
/* 769 */ 75,
/* 770 */ 782,
/* 771 */ 50439,
/* 772 */ 50091,
/* 773 */ 223,
/* 774 */ 50044,
/* 775 */ 223,
/* 776 */ 50044,
/* 777 */ 223,
/* 778 */ 50093,
/* 779 */ 50059,
/* 780 */ -688,
/* 781 */ 76,
/* 782 */ 796,
/* 783 */ 50439,
/* 784 */ 50091,
/* 785 */ 223,
/* 786 */ 50044,
/* 787 */ 223,
/* 788 */ 50044,
/* 789 */ 223,
/* 790 */ 50044,
/* 791 */ 163,
/* 792 */ 50093,
/* 793 */ 50059,
/* 794 */ -688,
/* 795 */ 77,
/* 796 */ 803,
/* 797 */ 50423,
/* 798 */ 1020,
/* 799 */ 1027,
/* 800 */ 50059,
/* 801 */ -688,
/* 802 */ 78,
/* 803 */ 810,
/* 804 */ 50368,
/* 805 */ 1043,
/* 806 */ 1050,
/* 807 */ 50059,
/* 808 */ -688,
/* 809 */ 79,
/* 810 */ 818,
/* 811 */ 50295,
/* 812 */ 1066,
/* 813 */ 1266,
/* 814 */ 1073,
/* 815 */ 50059,
/* 816 */ -688,
/* 817 */ 80,
/* 818 */ 824,
/* 819 */ 50293,
/* 820 */ 50257,
/* 821 */ 50059,
/* 822 */ -688,
/* 823 */ 81,
/* 824 */ 830,
/* 825 */ 50293,
/* 826 */ 223,
/* 827 */ 50059,
/* 828 */ -688,
/* 829 */ 82,
/* 830 */ 837,
/* 831 */ 50322,
/* 832 */ 1082,
/* 833 */ 173,
/* 834 */ 50059,
/* 835 */ -688,
/* 836 */ 83,
/* 837 */ 844,
/* 838 */ 50291,
/* 839 */ 1089,
/* 840 */ 173,
/* 841 */ 50059,
/* 842 */ -688,
/* 843 */ 84,
/* 844 */ 851,
/* 845 */ 50380,
/* 846 */ 1096,
/* 847 */ 173,
/* 848 */ 50059,
/* 849 */ -688,
/* 850 */ 85,
/* 851 */ 859,
/* 852 */ 50345,
/* 853 */ 1103,
/* 854 */ 1110,
/* 855 */ 223,
/* 856 */ 50059,
/* 857 */ -688,
/* 858 */ 86,
/* 859 */ 866,
/* 860 */ 50349,
/* 861 */ 1119,
/* 862 */ 1126,
/* 863 */ 50059,
/* 864 */ -688,
/* 865 */ 87,
/* 866 */ 872,
/* 867 */ 1418,
/* 868 */ 1134,
/* 869 */ 50059,
/* 870 */ -688,
/* 871 */ 88,
/* 872 */ 879,
/* 873 */ 50430,
/* 874 */ 1143,
/* 875 */ 223,
/* 876 */ 50059,
/* 877 */ -688,
/* 878 */ 89,
/* 879 */ 887,
/* 880 */ 50434,
/* 881 */ 1150,
/* 882 */ 223,
/* 883 */ 1157,
/* 884 */ 50059,
/* 885 */ -688,
/* 886 */ 90,
/* 887 */ 895,
/* 888 */ 50388,
/* 889 */ 1164,
/* 890 */ 223,
/* 891 */ 1171,
/* 892 */ 50059,
/* 893 */ -688,
/* 894 */ 91,
/* 895 */ 902,
/* 896 */ 50436,
/* 897 */ 1178,
/* 898 */ 1185,
/* 899 */ 50059,
/* 900 */ -688,
/* 901 */ 92,
/* 902 */ 909,
/* 903 */ 50329,
/* 904 */ 1201,
/* 905 */ 1208,
/* 906 */ 50059,
/* 907 */ -688,
/* 908 */ 93,
/* 909 */ 916,
/* 910 */ 50408,
/* 911 */ 1224,
/* 912 */ 1231,
/* 913 */ 50059,
/* 914 */ -688,
/* 915 */ 94,
/* 916 */ 920,
/* 917 */ 433,
/* 918 */ -688,
/* 919 */ 95,
/* 920 */ 924,
/* 921 */ 538,
/* 922 */ -688,
/* 923 */ 96,
/* 924 */ 928,
/* 925 */ 3423,
/* 926 */ -688,
/* 927 */ 97,
/* 928 */ 0,
/* 929 */ 6082,
/* 930 */ -688,
/* 931 */ 98,
/* 932 */ 936,
/* 933 */ 50058,
/* 934 */ -932,
/* 935 */ 528,
/* 936 */ 0,
/* 937 */ -932,
/* 938 */ 1044,
/* 939 */ 943,
/* 940 */ 50058,
/* 941 */ -939,
/* 942 */ 529,
/* 943 */ 0,
/* 944 */ -939,
/* 945 */ 1045,
/* 946 */ 950,
/* 947 */ 50373,
/* 948 */ -946,
/* 949 */ 530,
/* 950 */ 0,
/* 951 */ 50306,
/* 952 */ -946,
/* 953 */ 531,
/* 954 */ 958,
/* 955 */ 50058,
/* 956 */ -954,
/* 957 */ 532,
/* 958 */ 0,
/* 959 */ -954,
/* 960 */ 1046,
/* 961 */ 965,
/* 962 */ 50373,
/* 963 */ -961,
/* 964 */ 533,
/* 965 */ 0,
/* 966 */ 50306,
/* 967 */ -961,
/* 968 */ 534,
/* 969 */ 973,
/* 970 */ 50058,
/* 971 */ -969,
/* 972 */ 535,
/* 973 */ 0,
/* 974 */ -969,
/* 975 */ 1047,
/* 976 */ 980,
/* 977 */ 50058,
/* 978 */ -976,
/* 979 */ 536,
/* 980 */ 0,
/* 981 */ -976,
/* 982 */ 1048,
/* 983 */ 989,
/* 984 */ 50044,
/* 985 */ 173,
/* 986 */ 983,
/* 987 */ -983,
/* 988 */ 537,
/* 989 */ 0,
/* 990 */ -983,
/* 991 */ 1049,
/* 992 */ 996,
/* 993 */ 50058,
/* 994 */ -992,
/* 995 */ 538,
/* 996 */ 0,
/* 997 */ -992,
/* 998 */ 1050,
/* 999 */ 1003,
/* 1000 */ 50058,
/* 1001 */ -999,
/* 1002 */ 539,
/* 1003 */ 0,
/* 1004 */ -999,
/* 1005 */ 1051,
/* 1006 */ 1010,
/* 1007 */ 50043,
/* 1008 */ -1006,
/* 1009 */ 540,
/* 1010 */ 0,
/* 1011 */ -1006,
/* 1012 */ 1052,
/* 1013 */ 1017,
/* 1014 */ 50058,
/* 1015 */ -1013,
/* 1016 */ 541,
/* 1017 */ 0,
/* 1018 */ -1013,
/* 1019 */ 1053,
/* 1020 */ 1024,
/* 1021 */ 50058,
/* 1022 */ -1020,
/* 1023 */ 542,
/* 1024 */ 0,
/* 1025 */ -1020,
/* 1026 */ 1054,
/* 1027 */ 1033,
/* 1028 */ 223,
/* 1029 */ 1036,
/* 1030 */ 1027,
/* 1031 */ -1027,
/* 1032 */ 544,
/* 1033 */ 0,
/* 1034 */ -1027,
/* 1035 */ 1056,
/* 1036 */ 1040,
/* 1037 */ 50044,
/* 1038 */ -1036,
/* 1039 */ 543,
/* 1040 */ 0,
/* 1041 */ -1036,
/* 1042 */ 1055,
/* 1043 */ 1047,
/* 1044 */ 50058,
/* 1045 */ -1043,
/* 1046 */ 545,
/* 1047 */ 0,
/* 1048 */ -1043,
/* 1049 */ 1057,
/* 1050 */ 1056,
/* 1051 */ 223,
/* 1052 */ 1059,
/* 1053 */ 1050,
/* 1054 */ -1050,
/* 1055 */ 547,
/* 1056 */ 0,
/* 1057 */ -1050,
/* 1058 */ 1059,
/* 1059 */ 1063,
/* 1060 */ 50044,
/* 1061 */ -1059,
/* 1062 */ 546,
/* 1063 */ 0,
/* 1064 */ -1059,
/* 1065 */ 1058,
/* 1066 */ 1070,
/* 1067 */ 50058,
/* 1068 */ -1066,
/* 1069 */ 548,
/* 1070 */ 0,
/* 1071 */ -1066,
/* 1072 */ 1060,
/* 1073 */ 1079,
/* 1074 */ 50044,
/* 1075 */ 1266,
/* 1076 */ 1073,
/* 1077 */ -1073,
/* 1078 */ 549,
/* 1079 */ 0,
/* 1080 */ -1073,
/* 1081 */ 1061,
/* 1082 */ 1086,
/* 1083 */ 50058,
/* 1084 */ -1082,
/* 1085 */ 550,
/* 1086 */ 0,
/* 1087 */ -1082,
/* 1088 */ 1062,
/* 1089 */ 1093,
/* 1090 */ 50058,
/* 1091 */ -1089,
/* 1092 */ 551,
/* 1093 */ 0,
/* 1094 */ -1089,
/* 1095 */ 1063,
/* 1096 */ 1100,
/* 1097 */ 50058,
/* 1098 */ -1096,
/* 1099 */ 552,
/* 1100 */ 0,
/* 1101 */ -1096,
/* 1102 */ 1064,
/* 1103 */ 1107,
/* 1104 */ 50058,
/* 1105 */ -1103,
/* 1106 */ 553,
/* 1107 */ 0,
/* 1108 */ -1103,
/* 1109 */ 1065,
/* 1110 */ 1116,
/* 1111 */ 223,
/* 1112 */ 50044,
/* 1113 */ 1110,
/* 1114 */ -1110,
/* 1115 */ 554,
/* 1116 */ 0,
/* 1117 */ -1110,
/* 1118 */ 1066,
/* 1119 */ 1123,
/* 1120 */ 50058,
/* 1121 */ -1119,
/* 1122 */ 555,
/* 1123 */ 0,
/* 1124 */ -1119,
/* 1125 */ 1067,
/* 1126 */ 1131,
/* 1127 */ 1393,
/* 1128 */ 1126,
/* 1129 */ -1126,
/* 1130 */ 556,
/* 1131 */ 0,
/* 1132 */ -1126,
/* 1133 */ 1068,
/* 1134 */ 1140,
/* 1135 */ 50044,
/* 1136 */ 1418,
/* 1137 */ 1134,
/* 1138 */ -1134,
/* 1139 */ 557,
/* 1140 */ 0,
/* 1141 */ -1134,
/* 1142 */ 1069,
/* 1143 */ 1147,
/* 1144 */ 50058,
/* 1145 */ -1143,
/* 1146 */ 558,
/* 1147 */ 0,
/* 1148 */ -1143,
/* 1149 */ 1070,
/* 1150 */ 1154,
/* 1151 */ 50058,
/* 1152 */ -1150,
/* 1153 */ 559,
/* 1154 */ 0,
/* 1155 */ -1150,
/* 1156 */ 1071,
/* 1157 */ 1161,
/* 1158 */ 50037,
/* 1159 */ -1157,
/* 1160 */ 560,
/* 1161 */ 0,
/* 1162 */ -1157,
/* 1163 */ 1072,
/* 1164 */ 1168,
/* 1165 */ 50058,
/* 1166 */ -1164,
/* 1167 */ 561,
/* 1168 */ 0,
/* 1169 */ -1164,
/* 1170 */ 1073,
/* 1171 */ 1175,
/* 1172 */ 50037,
/* 1173 */ -1171,
/* 1174 */ 562,
/* 1175 */ 0,
/* 1176 */ -1171,
/* 1177 */ 1074,
/* 1178 */ 1182,
/* 1179 */ 50058,
/* 1180 */ -1178,
/* 1181 */ 563,
/* 1182 */ 0,
/* 1183 */ -1178,
/* 1184 */ 1075,
/* 1185 */ 1191,
/* 1186 */ 163,
/* 1187 */ 1194,
/* 1188 */ 1185,
/* 1189 */ -1185,
/* 1190 */ 565,
/* 1191 */ 0,
/* 1192 */ -1185,
/* 1193 */ 1077,
/* 1194 */ 1198,
/* 1195 */ 50044,
/* 1196 */ -1194,
/* 1197 */ 564,
/* 1198 */ 0,
/* 1199 */ -1194,
/* 1200 */ 1076,
/* 1201 */ 1205,
/* 1202 */ 50058,
/* 1203 */ -1201,
/* 1204 */ 566,
/* 1205 */ 0,
/* 1206 */ -1201,
/* 1207 */ 1078,
/* 1208 */ 1214,
/* 1209 */ 163,
/* 1210 */ 1217,
/* 1211 */ 1208,
/* 1212 */ -1208,
/* 1213 */ 568,
/* 1214 */ 0,
/* 1215 */ -1208,
/* 1216 */ 1080,
/* 1217 */ 1221,
/* 1218 */ 50044,
/* 1219 */ -1217,
/* 1220 */ 567,
/* 1221 */ 0,
/* 1222 */ -1217,
/* 1223 */ 1079,
/* 1224 */ 1228,
/* 1225 */ 50058,
/* 1226 */ -1224,
/* 1227 */ 569,
/* 1228 */ 0,
/* 1229 */ -1224,
/* 1230 */ 1081,
/* 1231 */ 1237,
/* 1232 */ 163,
/* 1233 */ 1240,
/* 1234 */ 1231,
/* 1235 */ -1231,
/* 1236 */ 571,
/* 1237 */ 0,
/* 1238 */ -1231,
/* 1239 */ 1083,
/* 1240 */ 1244,
/* 1241 */ 50044,
/* 1242 */ -1240,
/* 1243 */ 570,
/* 1244 */ 0,
/* 1245 */ -1240,
/* 1246 */ 1082,
/* 1247 */ 1251,
/* 1248 */ 50274,
/* 1249 */ -1247,
/* 1250 */ 99,
/* 1251 */ 1256,
/* 1252 */ 50302,
/* 1253 */ 50274,
/* 1254 */ -1247,
/* 1255 */ 100,
/* 1256 */ 1261,
/* 1257 */ 50375,
/* 1258 */ 50274,
/* 1259 */ -1247,
/* 1260 */ 101,
/* 1261 */ 0,
/* 1262 */ 50319,
/* 1263 */ 50274,
/* 1264 */ -1247,
/* 1265 */ 102,
/* 1266 */ 1276,
/* 1267 */ 223,
/* 1268 */ 50352,
/* 1269 */ 1283,
/* 1270 */ 1295,
/* 1271 */ 1302,
/* 1272 */ 1309,
/* 1273 */ 1331,
/* 1274 */ -1266,
/* 1275 */ 103,
/* 1276 */ 0,
/* 1277 */ 223,
/* 1278 */ 50352,
/* 1279 */ 163,
/* 1280 */ 1386,
/* 1281 */ -1266,
/* 1282 */ 104,
/* 1283 */ 1287,
/* 1284 */ 286,
/* 1285 */ -1283,
/* 1286 */ 572,
/* 1287 */ 1292,
/* 1288 */ 237,
/* 1289 */ 50037,
/* 1290 */ -1283,
/* 1291 */ 573,
/* 1292 */ 0,
/* 1293 */ -1283,
/* 1294 */ 1084,
/* 1295 */ 1299,
/* 1296 */ 237,
/* 1297 */ -1295,
/* 1298 */ 574,
/* 1299 */ 0,
/* 1300 */ -1295,
/* 1301 */ 1085,
/* 1302 */ 1306,
/* 1303 */ 50318,
/* 1304 */ -1302,
/* 1305 */ 575,
/* 1306 */ 0,
/* 1307 */ -1302,
/* 1308 */ 1086,
/* 1309 */ 1316,
/* 1310 */ 50040,
/* 1311 */ 1319,
/* 1312 */ 223,
/* 1313 */ 50041,
/* 1314 */ -1309,
/* 1315 */ 579,
/* 1316 */ 0,
/* 1317 */ -1309,
/* 1318 */ 1087,
/* 1319 */ 1323,
/* 1320 */ 50321,
/* 1321 */ -1319,
/* 1322 */ 576,
/* 1323 */ 1327,
/* 1324 */ 50306,
/* 1325 */ -1319,
/* 1326 */ 577,
/* 1327 */ 0,
/* 1328 */ 50373,
/* 1329 */ -1319,
/* 1330 */ 578,
/* 1331 */ 1340,
/* 1332 */ 50287,
/* 1333 */ 1343,
/* 1334 */ 1350,
/* 1335 */ 1357,
/* 1336 */ 1364,
/* 1337 */ 1331,
/* 1338 */ -1331,
/* 1339 */ 587,
/* 1340 */ 0,
/* 1341 */ -1331,
/* 1342 */ 1092,
/* 1343 */ 1347,
/* 1344 */ 286,
/* 1345 */ -1343,
/* 1346 */ 580,
/* 1347 */ 0,
/* 1348 */ -1343,
/* 1349 */ 1088,
/* 1350 */ 1354,
/* 1351 */ 223,
/* 1352 */ -1350,
/* 1353 */ 581,
/* 1354 */ 0,
/* 1355 */ -1350,
/* 1356 */ 1089,
/* 1357 */ 1361,
/* 1358 */ 50318,
/* 1359 */ -1357,
/* 1360 */ 582,
/* 1361 */ 0,
/* 1362 */ -1357,
/* 1363 */ 1090,
/* 1364 */ 1371,
/* 1365 */ 50040,
/* 1366 */ 1374,
/* 1367 */ 223,
/* 1368 */ 50041,
/* 1369 */ -1364,
/* 1370 */ 586,
/* 1371 */ 0,
/* 1372 */ -1364,
/* 1373 */ 1091,
/* 1374 */ 1378,
/* 1375 */ 50321,
/* 1376 */ -1374,
/* 1377 */ 583,
/* 1378 */ 1382,
/* 1379 */ 50306,
/* 1380 */ -1374,
/* 1381 */ 584,
/* 1382 */ 0,
/* 1383 */ 50373,
/* 1384 */ -1374,
/* 1385 */ 585,
/* 1386 */ 1390,
/* 1387 */ 223,
/* 1388 */ -1386,
/* 1389 */ 588,
/* 1390 */ 0,
/* 1391 */ -1386,
/* 1392 */ 1093,
/* 1393 */ 1398,
/* 1394 */ 223,
/* 1395 */ 1404,
/* 1396 */ -1393,
/* 1397 */ 105,
/* 1398 */ 0,
/* 1399 */ 50126,
/* 1400 */ 223,
/* 1401 */ 1411,
/* 1402 */ -1393,
/* 1403 */ 106,
/* 1404 */ 1408,
/* 1405 */ 50044,
/* 1406 */ -1404,
/* 1407 */ 589,
/* 1408 */ 0,
/* 1409 */ -1404,
/* 1410 */ 1094,
/* 1411 */ 1415,
/* 1412 */ 50044,
/* 1413 */ -1411,
/* 1414 */ 590,
/* 1415 */ 0,
/* 1416 */ -1411,
/* 1417 */ 1095,
/* 1418 */ 0,
/* 1419 */ 50276,
/* 1420 */ 1424,
/* 1421 */ 223,
/* 1422 */ -1418,
/* 1423 */ 107,
/* 1424 */ 1428,
/* 1425 */ 50058,
/* 1426 */ -1424,
/* 1427 */ 591,
/* 1428 */ 0,
/* 1429 */ -1424,
/* 1430 */ 1096,
/* 1431 */ 0,
/* 1432 */ 50371,
/* 1433 */ 50257,
/* 1434 */ 50058,
/* 1435 */ 223,
/* 1436 */ 50123,
/* 1437 */ 1441,
/* 1438 */ 50125,
/* 1439 */ -1431,
/* 1440 */ 108,
/* 1441 */ 1446,
/* 1442 */ 1449,
/* 1443 */ 1441,
/* 1444 */ -1441,
/* 1445 */ 592,
/* 1446 */ 0,
/* 1447 */ -1441,
/* 1448 */ 1097,
/* 1449 */ 1456,
/* 1450 */ 50325,
/* 1451 */ 1715,
/* 1452 */ 50257,
/* 1453 */ 50059,
/* 1454 */ -1449,
/* 1455 */ 109,
/* 1456 */ 1463,
/* 1457 */ 50373,
/* 1458 */ 1722,
/* 1459 */ 173,
/* 1460 */ 50059,
/* 1461 */ -1449,
/* 1462 */ 110,
/* 1463 */ 1467,
/* 1464 */ 462,
/* 1465 */ -1449,
/* 1466 */ 111,
/* 1467 */ 1474,
/* 1468 */ 50429,
/* 1469 */ 1729,
/* 1470 */ 259,
/* 1471 */ 50059,
/* 1472 */ -1449,
/* 1473 */ 112,
/* 1474 */ 1481,
/* 1475 */ 50372,
/* 1476 */ 1736,
/* 1477 */ 259,
/* 1478 */ 50059,
/* 1479 */ -1449,
/* 1480 */ 113,
/* 1481 */ 1488,
/* 1482 */ 50315,
/* 1483 */ 1743,
/* 1484 */ 223,
/* 1485 */ 50059,
/* 1486 */ -1449,
/* 1487 */ 114,
/* 1488 */ 1495,
/* 1489 */ 50451,
/* 1490 */ 1750,
/* 1491 */ 223,
/* 1492 */ 50059,
/* 1493 */ -1449,
/* 1494 */ 115,
/* 1495 */ 1502,
/* 1496 */ 50283,
/* 1497 */ 1757,
/* 1498 */ 173,
/* 1499 */ 50059,
/* 1500 */ -1449,
/* 1501 */ 116,
/* 1502 */ 1509,
/* 1503 */ 50434,
/* 1504 */ 1764,
/* 1505 */ 173,
/* 1506 */ 50059,
/* 1507 */ -1449,
/* 1508 */ 117,
/* 1509 */ 1517,
/* 1510 */ 50434,
/* 1511 */ 1771,
/* 1512 */ 237,
/* 1513 */ 50037,
/* 1514 */ 50059,
/* 1515 */ -1449,
/* 1516 */ 118,
/* 1517 */ 1526,
/* 1518 */ 50434,
/* 1519 */ 1778,
/* 1520 */ 50043,
/* 1521 */ 237,
/* 1522 */ 50037,
/* 1523 */ 50059,
/* 1524 */ -1449,
/* 1525 */ 119,
/* 1526 */ 1533,
/* 1527 */ 50403,
/* 1528 */ 1785,
/* 1529 */ 173,
/* 1530 */ 50059,
/* 1531 */ -1449,
/* 1532 */ 120,
/* 1533 */ 1540,
/* 1534 */ 50419,
/* 1535 */ 1792,
/* 1536 */ 173,
/* 1537 */ 50059,
/* 1538 */ -1449,
/* 1539 */ 121,
/* 1540 */ 1547,
/* 1541 */ 50382,
/* 1542 */ 1799,
/* 1543 */ 223,
/* 1544 */ 50059,
/* 1545 */ -1449,
/* 1546 */ 122,
/* 1547 */ 1555,
/* 1548 */ 50314,
/* 1549 */ 1806,
/* 1550 */ 223,
/* 1551 */ 1813,
/* 1552 */ 50059,
/* 1553 */ -1449,
/* 1554 */ 123,
/* 1555 */ 1562,
/* 1556 */ 50465,
/* 1557 */ 1820,
/* 1558 */ 223,
/* 1559 */ 50059,
/* 1560 */ -1449,
/* 1561 */ 124,
/* 1562 */ 1569,
/* 1563 */ 50430,
/* 1564 */ 1827,
/* 1565 */ 223,
/* 1566 */ 50059,
/* 1567 */ -1449,
/* 1568 */ 125,
/* 1569 */ 1576,
/* 1570 */ 50395,
/* 1571 */ 1834,
/* 1572 */ 223,
/* 1573 */ 50059,
/* 1574 */ -1449,
/* 1575 */ 126,
/* 1576 */ 1583,
/* 1577 */ 50322,
/* 1578 */ 1841,
/* 1579 */ 223,
/* 1580 */ 50059,
/* 1581 */ -1449,
/* 1582 */ 127,
/* 1583 */ 1590,
/* 1584 */ 50311,
/* 1585 */ 1848,
/* 1586 */ 223,
/* 1587 */ 50059,
/* 1588 */ -1449,
/* 1589 */ 128,
/* 1590 */ 1597,
/* 1591 */ 50406,
/* 1592 */ 1855,
/* 1593 */ 223,
/* 1594 */ 50059,
/* 1595 */ -1449,
/* 1596 */ 129,
/* 1597 */ 1604,
/* 1598 */ 50291,
/* 1599 */ 1862,
/* 1600 */ 1869,
/* 1601 */ 50059,
/* 1602 */ -1449,
/* 1603 */ 130,
/* 1604 */ 1611,
/* 1605 */ 50364,
/* 1606 */ 1877,
/* 1607 */ 223,
/* 1608 */ 50059,
/* 1609 */ -1449,
/* 1610 */ 131,
/* 1611 */ 1618,
/* 1612 */ 50436,
/* 1613 */ 1884,
/* 1614 */ 1891,
/* 1615 */ 50059,
/* 1616 */ -1449,
/* 1617 */ 132,
/* 1618 */ 1625,
/* 1619 */ 50348,
/* 1620 */ 1907,
/* 1621 */ 1914,
/* 1622 */ 50059,
/* 1623 */ -1449,
/* 1624 */ 133,
/* 1625 */ 1632,
/* 1626 */ 50349,
/* 1627 */ 1930,
/* 1628 */ 1937,
/* 1629 */ 50059,
/* 1630 */ -1449,
/* 1631 */ 134,
/* 1632 */ 1639,
/* 1633 */ 50360,
/* 1634 */ 1945,
/* 1635 */ 1952,
/* 1636 */ 50059,
/* 1637 */ -1449,
/* 1638 */ 135,
/* 1639 */ 1646,
/* 1640 */ 50303,
/* 1641 */ 1972,
/* 1642 */ 223,
/* 1643 */ 50059,
/* 1644 */ -1449,
/* 1645 */ 136,
/* 1646 */ 1653,
/* 1647 */ 50467,
/* 1648 */ 1979,
/* 1649 */ 223,
/* 1650 */ 50059,
/* 1651 */ -1449,
/* 1652 */ 137,
/* 1653 */ 1660,
/* 1654 */ 50466,
/* 1655 */ 1986,
/* 1656 */ 223,
/* 1657 */ 50059,
/* 1658 */ -1449,
/* 1659 */ 138,
/* 1660 */ 1667,
/* 1661 */ 50378,
/* 1662 */ 1993,
/* 1663 */ 223,
/* 1664 */ 50059,
/* 1665 */ -1449,
/* 1666 */ 139,
/* 1667 */ 1674,
/* 1668 */ 50455,
/* 1669 */ 2000,
/* 1670 */ 223,
/* 1671 */ 50059,
/* 1672 */ -1449,
/* 1673 */ 140,
/* 1674 */ 1681,
/* 1675 */ 50312,
/* 1676 */ 2007,
/* 1677 */ 223,
/* 1678 */ 50059,
/* 1679 */ -1449,
/* 1680 */ 141,
/* 1681 */ 1689,
/* 1682 */ 50375,
/* 1683 */ 50419,
/* 1684 */ 2014,
/* 1685 */ 223,
/* 1686 */ 50059,
/* 1687 */ -1449,
/* 1688 */ 142,
/* 1689 */ 1696,
/* 1690 */ 50374,
/* 1691 */ 2021,
/* 1692 */ 223,
/* 1693 */ 50059,
/* 1694 */ -1449,
/* 1695 */ 143,
/* 1696 */ 1703,
/* 1697 */ 50354,
/* 1698 */ 2028,
/* 1699 */ 163,
/* 1700 */ 50059,
/* 1701 */ -1449,
/* 1702 */ 144,
/* 1703 */ 1707,
/* 1704 */ 3423,
/* 1705 */ -1449,
/* 1706 */ 145,
/* 1707 */ 1711,
/* 1708 */ 538,
/* 1709 */ -1449,
/* 1710 */ 146,
/* 1711 */ 0,
/* 1712 */ 6082,
/* 1713 */ -1449,
/* 1714 */ 147,
/* 1715 */ 1719,
/* 1716 */ 50058,
/* 1717 */ -1715,
/* 1718 */ 593,
/* 1719 */ 0,
/* 1720 */ -1715,
/* 1721 */ 1098,
/* 1722 */ 1726,
/* 1723 */ 50058,
/* 1724 */ -1722,
/* 1725 */ 594,
/* 1726 */ 0,
/* 1727 */ -1722,
/* 1728 */ 1099,
/* 1729 */ 1733,
/* 1730 */ 50058,
/* 1731 */ -1729,
/* 1732 */ 595,
/* 1733 */ 0,
/* 1734 */ -1729,
/* 1735 */ 1100,
/* 1736 */ 1740,
/* 1737 */ 50058,
/* 1738 */ -1736,
/* 1739 */ 596,
/* 1740 */ 0,
/* 1741 */ -1736,
/* 1742 */ 1101,
/* 1743 */ 1747,
/* 1744 */ 50058,
/* 1745 */ -1743,
/* 1746 */ 597,
/* 1747 */ 0,
/* 1748 */ -1743,
/* 1749 */ 1102,
/* 1750 */ 1754,
/* 1751 */ 50058,
/* 1752 */ -1750,
/* 1753 */ 598,
/* 1754 */ 0,
/* 1755 */ -1750,
/* 1756 */ 1103,
/* 1757 */ 1761,
/* 1758 */ 50058,
/* 1759 */ -1757,
/* 1760 */ 599,
/* 1761 */ 0,
/* 1762 */ -1757,
/* 1763 */ 1104,
/* 1764 */ 1768,
/* 1765 */ 50058,
/* 1766 */ -1764,
/* 1767 */ 600,
/* 1768 */ 0,
/* 1769 */ -1764,
/* 1770 */ 1105,
/* 1771 */ 1775,
/* 1772 */ 50058,
/* 1773 */ -1771,
/* 1774 */ 601,
/* 1775 */ 0,
/* 1776 */ -1771,
/* 1777 */ 1106,
/* 1778 */ 1782,
/* 1779 */ 50058,
/* 1780 */ -1778,
/* 1781 */ 602,
/* 1782 */ 0,
/* 1783 */ -1778,
/* 1784 */ 1107,
/* 1785 */ 1789,
/* 1786 */ 50058,
/* 1787 */ -1785,
/* 1788 */ 603,
/* 1789 */ 0,
/* 1790 */ -1785,
/* 1791 */ 1108,
/* 1792 */ 1796,
/* 1793 */ 50058,
/* 1794 */ -1792,
/* 1795 */ 604,
/* 1796 */ 0,
/* 1797 */ -1792,
/* 1798 */ 1109,
/* 1799 */ 1803,
/* 1800 */ 50058,
/* 1801 */ -1799,
/* 1802 */ 605,
/* 1803 */ 0,
/* 1804 */ -1799,
/* 1805 */ 1110,
/* 1806 */ 1810,
/* 1807 */ 50058,
/* 1808 */ -1806,
/* 1809 */ 606,
/* 1810 */ 0,
/* 1811 */ -1806,
/* 1812 */ 1111,
/* 1813 */ 1817,
/* 1814 */ 223,
/* 1815 */ -1813,
/* 1816 */ 607,
/* 1817 */ 0,
/* 1818 */ -1813,
/* 1819 */ 1112,
/* 1820 */ 1824,
/* 1821 */ 50058,
/* 1822 */ -1820,
/* 1823 */ 608,
/* 1824 */ 0,
/* 1825 */ -1820,
/* 1826 */ 1113,
/* 1827 */ 1831,
/* 1828 */ 50058,
/* 1829 */ -1827,
/* 1830 */ 609,
/* 1831 */ 0,
/* 1832 */ -1827,
/* 1833 */ 1114,
/* 1834 */ 1838,
/* 1835 */ 50058,
/* 1836 */ -1834,
/* 1837 */ 610,
/* 1838 */ 0,
/* 1839 */ -1834,
/* 1840 */ 1115,
/* 1841 */ 1845,
/* 1842 */ 50058,
/* 1843 */ -1841,
/* 1844 */ 611,
/* 1845 */ 0,
/* 1846 */ -1841,
/* 1847 */ 1116,
/* 1848 */ 1852,
/* 1849 */ 50058,
/* 1850 */ -1848,
/* 1851 */ 612,
/* 1852 */ 0,
/* 1853 */ -1848,
/* 1854 */ 1117,
/* 1855 */ 1859,
/* 1856 */ 50058,
/* 1857 */ -1855,
/* 1858 */ 613,
/* 1859 */ 0,
/* 1860 */ -1855,
/* 1861 */ 1118,
/* 1862 */ 1866,
/* 1863 */ 50058,
/* 1864 */ -1862,
/* 1865 */ 614,
/* 1866 */ 0,
/* 1867 */ -1862,
/* 1868 */ 1119,
/* 1869 */ 1874,
/* 1870 */ 2035,
/* 1871 */ 1869,
/* 1872 */ -1869,
/* 1873 */ 615,
/* 1874 */ 0,
/* 1875 */ -1869,
/* 1876 */ 1120,
/* 1877 */ 1881,
/* 1878 */ 50058,
/* 1879 */ -1877,
/* 1880 */ 616,
/* 1881 */ 0,
/* 1882 */ -1877,
/* 1883 */ 1121,
/* 1884 */ 1888,
/* 1885 */ 50058,
/* 1886 */ -1884,
/* 1887 */ 617,
/* 1888 */ 0,
/* 1889 */ -1884,
/* 1890 */ 1122,
/* 1891 */ 1897,
/* 1892 */ 163,
/* 1893 */ 1900,
/* 1894 */ 1891,
/* 1895 */ -1891,
/* 1896 */ 619,
/* 1897 */ 0,
/* 1898 */ -1891,
/* 1899 */ 1124,
/* 1900 */ 1904,
/* 1901 */ 50044,
/* 1902 */ -1900,
/* 1903 */ 618,
/* 1904 */ 0,
/* 1905 */ -1900,
/* 1906 */ 1123,
/* 1907 */ 1911,
/* 1908 */ 50058,
/* 1909 */ -1907,
/* 1910 */ 620,
/* 1911 */ 0,
/* 1912 */ -1907,
/* 1913 */ 1125,
/* 1914 */ 1920,
/* 1915 */ 163,
/* 1916 */ 1923,
/* 1917 */ 1914,
/* 1918 */ -1914,
/* 1919 */ 622,
/* 1920 */ 0,
/* 1921 */ -1914,
/* 1922 */ 1127,
/* 1923 */ 1927,
/* 1924 */ 50044,
/* 1925 */ -1923,
/* 1926 */ 621,
/* 1927 */ 0,
/* 1928 */ -1923,
/* 1929 */ 1126,
/* 1930 */ 1934,
/* 1931 */ 50058,
/* 1932 */ -1930,
/* 1933 */ 623,
/* 1934 */ 0,
/* 1935 */ -1930,
/* 1936 */ 1128,
/* 1937 */ 1942,
/* 1938 */ 2060,
/* 1939 */ 1937,
/* 1940 */ -1937,
/* 1941 */ 624,
/* 1942 */ 0,
/* 1943 */ -1937,
/* 1944 */ 1129,
/* 1945 */ 1949,
/* 1946 */ 50058,
/* 1947 */ -1945,
/* 1948 */ 625,
/* 1949 */ 0,
/* 1950 */ -1945,
/* 1951 */ 1130,
/* 1952 */ 1958,
/* 1953 */ 223,
/* 1954 */ 1961,
/* 1955 */ 1952,
/* 1956 */ -1952,
/* 1957 */ 628,
/* 1958 */ 0,
/* 1959 */ -1952,
/* 1960 */ 1132,
/* 1961 */ 1965,
/* 1962 */ 50044,
/* 1963 */ -1961,
/* 1964 */ 626,
/* 1965 */ 1969,
/* 1966 */ 50124,
/* 1967 */ -1961,
/* 1968 */ 627,
/* 1969 */ 0,
/* 1970 */ -1961,
/* 1971 */ 1131,
/* 1972 */ 1976,
/* 1973 */ 50058,
/* 1974 */ -1972,
/* 1975 */ 629,
/* 1976 */ 0,
/* 1977 */ -1972,
/* 1978 */ 1133,
/* 1979 */ 1983,
/* 1980 */ 50058,
/* 1981 */ -1979,
/* 1982 */ 630,
/* 1983 */ 0,
/* 1984 */ -1979,
/* 1985 */ 1134,
/* 1986 */ 1990,
/* 1987 */ 50058,
/* 1988 */ -1986,
/* 1989 */ 631,
/* 1990 */ 0,
/* 1991 */ -1986,
/* 1992 */ 1135,
/* 1993 */ 1997,
/* 1994 */ 50058,
/* 1995 */ -1993,
/* 1996 */ 632,
/* 1997 */ 0,
/* 1998 */ -1993,
/* 1999 */ 1136,
/* 2000 */ 2004,
/* 2001 */ 50058,
/* 2002 */ -2000,
/* 2003 */ 633,
/* 2004 */ 0,
/* 2005 */ -2000,
/* 2006 */ 1137,
/* 2007 */ 2011,
/* 2008 */ 50058,
/* 2009 */ -2007,
/* 2010 */ 634,
/* 2011 */ 0,
/* 2012 */ -2007,
/* 2013 */ 1138,
/* 2014 */ 2018,
/* 2015 */ 50058,
/* 2016 */ -2014,
/* 2017 */ 635,
/* 2018 */ 0,
/* 2019 */ -2014,
/* 2020 */ 1139,
/* 2021 */ 2025,
/* 2022 */ 50058,
/* 2023 */ -2021,
/* 2024 */ 636,
/* 2025 */ 0,
/* 2026 */ -2021,
/* 2027 */ 1140,
/* 2028 */ 2032,
/* 2029 */ 50058,
/* 2030 */ -2028,
/* 2031 */ 637,
/* 2032 */ 0,
/* 2033 */ -2028,
/* 2034 */ 1141,
/* 2035 */ 2040,
/* 2036 */ 223,
/* 2037 */ 2046,
/* 2038 */ -2035,
/* 2039 */ 148,
/* 2040 */ 0,
/* 2041 */ 50275,
/* 2042 */ 223,
/* 2043 */ 2053,
/* 2044 */ -2035,
/* 2045 */ 149,
/* 2046 */ 2050,
/* 2047 */ 50044,
/* 2048 */ -2046,
/* 2049 */ 638,
/* 2050 */ 0,
/* 2051 */ -2046,
/* 2052 */ 1142,
/* 2053 */ 2057,
/* 2054 */ 50044,
/* 2055 */ -2053,
/* 2056 */ 639,
/* 2057 */ 0,
/* 2058 */ -2053,
/* 2059 */ 1143,
/* 2060 */ 2065,
/* 2061 */ 223,
/* 2062 */ 2071,
/* 2063 */ -2060,
/* 2064 */ 150,
/* 2065 */ 0,
/* 2066 */ 50126,
/* 2067 */ 223,
/* 2068 */ 2078,
/* 2069 */ -2060,
/* 2070 */ 151,
/* 2071 */ 2075,
/* 2072 */ 50044,
/* 2073 */ -2071,
/* 2074 */ 640,
/* 2075 */ 0,
/* 2076 */ -2071,
/* 2077 */ 1144,
/* 2078 */ 2082,
/* 2079 */ 50044,
/* 2080 */ -2078,
/* 2081 */ 641,
/* 2082 */ 0,
/* 2083 */ -2078,
/* 2084 */ 1145,
/* 2085 */ 0,
/* 2086 */ 50347,
/* 2087 */ 50257,
/* 2088 */ 50058,
/* 2089 */ 223,
/* 2090 */ 50123,
/* 2091 */ 2095,
/* 2092 */ 50125,
/* 2093 */ -2085,
/* 2094 */ 152,
/* 2095 */ 2100,
/* 2096 */ 2103,
/* 2097 */ 2095,
/* 2098 */ -2095,
/* 2099 */ 642,
/* 2100 */ 0,
/* 2101 */ -2095,
/* 2102 */ 1146,
/* 2103 */ 2110,
/* 2104 */ 50325,
/* 2105 */ 2176,
/* 2106 */ 50257,
/* 2107 */ 50059,
/* 2108 */ -2103,
/* 2109 */ 153,
/* 2110 */ 2117,
/* 2111 */ 50382,
/* 2112 */ 2183,
/* 2113 */ 223,
/* 2114 */ 50059,
/* 2115 */ -2103,
/* 2116 */ 154,
/* 2117 */ 2121,
/* 2118 */ 462,
/* 2119 */ -2103,
/* 2120 */ 155,
/* 2121 */ 2129,
/* 2122 */ 50388,
/* 2123 */ 2190,
/* 2124 */ 223,
/* 2125 */ 50037,
/* 2126 */ 50059,
/* 2127 */ -2103,
/* 2128 */ 156,
/* 2129 */ 2136,
/* 2130 */ 50364,
/* 2131 */ 2197,
/* 2132 */ 223,
/* 2133 */ 50059,
/* 2134 */ -2103,
/* 2135 */ 157,
/* 2136 */ 2143,
/* 2137 */ 50349,
/* 2138 */ 2204,
/* 2139 */ 2211,
/* 2140 */ 50059,
/* 2141 */ -2103,
/* 2142 */ 158,
/* 2143 */ 2150,
/* 2144 */ 2231,
/* 2145 */ 2239,
/* 2146 */ 163,
/* 2147 */ 50059,
/* 2148 */ -2103,
/* 2149 */ 159,
/* 2150 */ 2157,
/* 2151 */ 50471,
/* 2152 */ 2246,
/* 2153 */ 173,
/* 2154 */ 50059,
/* 2155 */ -2103,
/* 2156 */ 160,
/* 2157 */ 2164,
/* 2158 */ 50343,
/* 2159 */ 2253,
/* 2160 */ 259,
/* 2161 */ 50059,
/* 2162 */ -2103,
/* 2163 */ 161,
/* 2164 */ 2168,
/* 2165 */ 538,
/* 2166 */ -2103,
/* 2167 */ 162,
/* 2168 */ 2172,
/* 2169 */ 3423,
/* 2170 */ -2103,
/* 2171 */ 163,
/* 2172 */ 0,
/* 2173 */ 6082,
/* 2174 */ -2103,
/* 2175 */ 164,
/* 2176 */ 2180,
/* 2177 */ 50058,
/* 2178 */ -2176,
/* 2179 */ 643,
/* 2180 */ 0,
/* 2181 */ -2176,
/* 2182 */ 1147,
/* 2183 */ 2187,
/* 2184 */ 50058,
/* 2185 */ -2183,
/* 2186 */ 644,
/* 2187 */ 0,
/* 2188 */ -2183,
/* 2189 */ 1148,
/* 2190 */ 2194,
/* 2191 */ 50058,
/* 2192 */ -2190,
/* 2193 */ 645,
/* 2194 */ 0,
/* 2195 */ -2190,
/* 2196 */ 1149,
/* 2197 */ 2201,
/* 2198 */ 50058,
/* 2199 */ -2197,
/* 2200 */ 646,
/* 2201 */ 0,
/* 2202 */ -2197,
/* 2203 */ 1150,
/* 2204 */ 2208,
/* 2205 */ 50058,
/* 2206 */ -2204,
/* 2207 */ 647,
/* 2208 */ 0,
/* 2209 */ -2204,
/* 2210 */ 1151,
/* 2211 */ 2217,
/* 2212 */ 223,
/* 2213 */ 2220,
/* 2214 */ 2211,
/* 2215 */ -2211,
/* 2216 */ 650,
/* 2217 */ 0,
/* 2218 */ -2211,
/* 2219 */ 1153,
/* 2220 */ 2224,
/* 2221 */ 50124,
/* 2222 */ -2220,
/* 2223 */ 648,
/* 2224 */ 2228,
/* 2225 */ 50044,
/* 2226 */ -2220,
/* 2227 */ 649,
/* 2228 */ 0,
/* 2229 */ -2220,
/* 2230 */ 1152,
/* 2231 */ 2235,
/* 2232 */ 50447,
/* 2233 */ -2231,
/* 2234 */ 651,
/* 2235 */ 0,
/* 2236 */ 50333,
/* 2237 */ -2231,
/* 2238 */ 652,
/* 2239 */ 2243,
/* 2240 */ 50058,
/* 2241 */ -2239,
/* 2242 */ 653,
/* 2243 */ 0,
/* 2244 */ -2239,
/* 2245 */ 1154,
/* 2246 */ 2250,
/* 2247 */ 50058,
/* 2248 */ -2246,
/* 2249 */ 654,
/* 2250 */ 0,
/* 2251 */ -2246,
/* 2252 */ 1155,
/* 2253 */ 2257,
/* 2254 */ 50058,
/* 2255 */ -2253,
/* 2256 */ 655,
/* 2257 */ 0,
/* 2258 */ -2253,
/* 2259 */ 1156,
/* 2260 */ 0,
/* 2261 */ 2272,
/* 2262 */ 2288,
/* 2263 */ 50257,
/* 2264 */ 50058,
/* 2265 */ 223,
/* 2266 */ 50123,
/* 2267 */ 2304,
/* 2268 */ 50125,
/* 2269 */ 2312,
/* 2270 */ -2260,
/* 2271 */ 165,
/* 2272 */ 2278,
/* 2273 */ 223,
/* 2274 */ 2281,
/* 2275 */ 2272,
/* 2276 */ -2272,
/* 2277 */ 657,
/* 2278 */ 0,
/* 2279 */ -2272,
/* 2280 */ 1158,
/* 2281 */ 2285,
/* 2282 */ 50047,
/* 2283 */ -2281,
/* 2284 */ 656,
/* 2285 */ 0,
/* 2286 */ -2281,
/* 2287 */ 1157,
/* 2288 */ 2292,
/* 2289 */ 50435,
/* 2290 */ -2288,
/* 2291 */ 658,
/* 2292 */ 2296,
/* 2293 */ 50333,
/* 2294 */ -2288,
/* 2295 */ 659,
/* 2296 */ 2300,
/* 2297 */ 50408,
/* 2298 */ -2288,
/* 2299 */ 660,
/* 2300 */ 0,
/* 2301 */ 50329,
/* 2302 */ -2288,
/* 2303 */ 661,
/* 2304 */ 2309,
/* 2305 */ 2332,
/* 2306 */ 2304,
/* 2307 */ -2304,
/* 2308 */ 662,
/* 2309 */ 0,
/* 2310 */ -2304,
/* 2311 */ 1159,
/* 2312 */ 2321,
/* 2313 */ 50287,
/* 2314 */ 223,
/* 2315 */ 50123,
/* 2316 */ 2324,
/* 2317 */ 50125,
/* 2318 */ 2312,
/* 2319 */ -2312,
/* 2320 */ 664,
/* 2321 */ 0,
/* 2322 */ -2312,
/* 2323 */ 1161,
/* 2324 */ 2329,
/* 2325 */ 2332,
/* 2326 */ 2324,
/* 2327 */ -2324,
/* 2328 */ 663,
/* 2329 */ 0,
/* 2330 */ -2324,
/* 2331 */ 1160,
/* 2332 */ 2339,
/* 2333 */ 50325,
/* 2334 */ 2486,
/* 2335 */ 50257,
/* 2336 */ 50059,
/* 2337 */ -2332,
/* 2338 */ 166,
/* 2339 */ 2344,
/* 2340 */ 173,
/* 2341 */ 50059,
/* 2342 */ -2332,
/* 2343 */ 167,
/* 2344 */ 2351,
/* 2345 */ 50373,
/* 2346 */ 2493,
/* 2347 */ 223,
/* 2348 */ 50059,
/* 2349 */ -2332,
/* 2350 */ 168,
/* 2351 */ 2358,
/* 2352 */ 50314,
/* 2353 */ 2500,
/* 2354 */ 223,
/* 2355 */ 50059,
/* 2356 */ -2332,
/* 2357 */ 169,
/* 2358 */ 2367,
/* 2359 */ 50297,
/* 2360 */ 2507,
/* 2361 */ 2514,
/* 2362 */ 223,
/* 2363 */ 2521,
/* 2364 */ 50059,
/* 2365 */ -2332,
/* 2366 */ 170,
/* 2367 */ 2375,
/* 2368 */ 50412,
/* 2369 */ 2528,
/* 2370 */ 173,
/* 2371 */ 2535,
/* 2372 */ 50059,
/* 2373 */ -2332,
/* 2374 */ 171,
/* 2375 */ 2383,
/* 2376 */ 50444,
/* 2377 */ 2544,
/* 2378 */ 173,
/* 2379 */ 2551,
/* 2380 */ 50059,
/* 2381 */ -2332,
/* 2382 */ 172,
/* 2383 */ 2390,
/* 2384 */ 50416,
/* 2385 */ 2558,
/* 2386 */ 173,
/* 2387 */ 50059,
/* 2388 */ -2332,
/* 2389 */ 173,
/* 2390 */ 2397,
/* 2391 */ 50471,
/* 2392 */ 2565,
/* 2393 */ 173,
/* 2394 */ 50059,
/* 2395 */ -2332,
/* 2396 */ 174,
/* 2397 */ 2404,
/* 2398 */ 50472,
/* 2399 */ 2572,
/* 2400 */ 173,
/* 2401 */ 50059,
/* 2402 */ -2332,
/* 2403 */ 175,
/* 2404 */ 2411,
/* 2405 */ 50332,
/* 2406 */ 2579,
/* 2407 */ 173,
/* 2408 */ 50059,
/* 2409 */ -2332,
/* 2410 */ 176,
/* 2411 */ 2418,
/* 2412 */ 50413,
/* 2413 */ 2586,
/* 2414 */ 259,
/* 2415 */ 50059,
/* 2416 */ -2332,
/* 2417 */ 177,
/* 2418 */ 2425,
/* 2419 */ 50296,
/* 2420 */ 2593,
/* 2421 */ 173,
/* 2422 */ 50059,
/* 2423 */ -2332,
/* 2424 */ 178,
/* 2425 */ 2432,
/* 2426 */ 50377,
/* 2427 */ 2600,
/* 2428 */ 173,
/* 2429 */ 50059,
/* 2430 */ -2332,
/* 2431 */ 179,
/* 2432 */ 2439,
/* 2433 */ 50460,
/* 2434 */ 2607,
/* 2435 */ 173,
/* 2436 */ 50059,
/* 2437 */ -2332,
/* 2438 */ 180,
/* 2439 */ 2446,
/* 2440 */ 50427,
/* 2441 */ 2614,
/* 2442 */ 163,
/* 2443 */ 50059,
/* 2444 */ -2332,
/* 2445 */ 181,
/* 2446 */ 2453,
/* 2447 */ 50427,
/* 2448 */ 2621,
/* 2449 */ 173,
/* 2450 */ 50059,
/* 2451 */ -2332,
/* 2452 */ 182,
/* 2453 */ 2460,
/* 2454 */ 50317,
/* 2455 */ 2628,
/* 2456 */ 173,
/* 2457 */ 50059,
/* 2458 */ -2332,
/* 2459 */ 183,
/* 2460 */ 2467,
/* 2461 */ 50317,
/* 2462 */ 2635,
/* 2463 */ 50274,
/* 2464 */ 50059,
/* 2465 */ -2332,
/* 2466 */ 184,
/* 2467 */ 2474,
/* 2468 */ 50349,
/* 2469 */ 2642,
/* 2470 */ 2649,
/* 2471 */ 50059,
/* 2472 */ -2332,
/* 2473 */ 185,
/* 2474 */ 2478,
/* 2475 */ 538,
/* 2476 */ -2332,
/* 2477 */ 186,
/* 2478 */ 2482,
/* 2479 */ 6082,
/* 2480 */ -2332,
/* 2481 */ 187,
/* 2482 */ 0,
/* 2483 */ 3423,
/* 2484 */ -2332,
/* 2485 */ 188,
/* 2486 */ 2490,
/* 2487 */ 50058,
/* 2488 */ -2486,
/* 2489 */ 665,
/* 2490 */ 0,
/* 2491 */ -2486,
/* 2492 */ 1162,
/* 2493 */ 2497,
/* 2494 */ 50058,
/* 2495 */ -2493,
/* 2496 */ 666,
/* 2497 */ 0,
/* 2498 */ -2493,
/* 2499 */ 1163,
/* 2500 */ 2504,
/* 2501 */ 50058,
/* 2502 */ -2500,
/* 2503 */ 667,
/* 2504 */ 0,
/* 2505 */ -2500,
/* 2506 */ 1164,
/* 2507 */ 2511,
/* 2508 */ 50307,
/* 2509 */ -2507,
/* 2510 */ 668,
/* 2511 */ 0,
/* 2512 */ -2507,
/* 2513 */ 1165,
/* 2514 */ 2518,
/* 2515 */ 50058,
/* 2516 */ -2514,
/* 2517 */ 669,
/* 2518 */ 0,
/* 2519 */ -2514,
/* 2520 */ 1166,
/* 2521 */ 2525,
/* 2522 */ 50037,
/* 2523 */ -2521,
/* 2524 */ 670,
/* 2525 */ 0,
/* 2526 */ -2521,
/* 2527 */ 1167,
/* 2528 */ 2532,
/* 2529 */ 50058,
/* 2530 */ -2528,
/* 2531 */ 671,
/* 2532 */ 0,
/* 2533 */ -2528,
/* 2534 */ 1168,
/* 2535 */ 2541,
/* 2536 */ 50040,
/* 2537 */ 173,
/* 2538 */ 50041,
/* 2539 */ -2535,
/* 2540 */ 672,
/* 2541 */ 0,
/* 2542 */ -2535,
/* 2543 */ 1169,
/* 2544 */ 2548,
/* 2545 */ 50058,
/* 2546 */ -2544,
/* 2547 */ 673,
/* 2548 */ 0,
/* 2549 */ -2544,
/* 2550 */ 1170,
/* 2551 */ 2555,
/* 2552 */ 50404,
/* 2553 */ -2551,
/* 2554 */ 674,
/* 2555 */ 0,
/* 2556 */ -2551,
/* 2557 */ 1171,
/* 2558 */ 2562,
/* 2559 */ 50058,
/* 2560 */ -2558,
/* 2561 */ 675,
/* 2562 */ 0,
/* 2563 */ -2558,
/* 2564 */ 1172,
/* 2565 */ 2569,
/* 2566 */ 50058,
/* 2567 */ -2565,
/* 2568 */ 676,
/* 2569 */ 0,
/* 2570 */ -2565,
/* 2571 */ 1173,
/* 2572 */ 2576,
/* 2573 */ 50058,
/* 2574 */ -2572,
/* 2575 */ 677,
/* 2576 */ 0,
/* 2577 */ -2572,
/* 2578 */ 1174,
/* 2579 */ 2583,
/* 2580 */ 50058,
/* 2581 */ -2579,
/* 2582 */ 678,
/* 2583 */ 0,
/* 2584 */ -2579,
/* 2585 */ 1175,
/* 2586 */ 2590,
/* 2587 */ 50058,
/* 2588 */ -2586,
/* 2589 */ 679,
/* 2590 */ 0,
/* 2591 */ -2586,
/* 2592 */ 1176,
/* 2593 */ 2597,
/* 2594 */ 50058,
/* 2595 */ -2593,
/* 2596 */ 680,
/* 2597 */ 0,
/* 2598 */ -2593,
/* 2599 */ 1177,
/* 2600 */ 2604,
/* 2601 */ 50058,
/* 2602 */ -2600,
/* 2603 */ 681,
/* 2604 */ 0,
/* 2605 */ -2600,
/* 2606 */ 1178,
/* 2607 */ 2611,
/* 2608 */ 50058,
/* 2609 */ -2607,
/* 2610 */ 682,
/* 2611 */ 0,
/* 2612 */ -2607,
/* 2613 */ 1179,
/* 2614 */ 2618,
/* 2615 */ 50058,
/* 2616 */ -2614,
/* 2617 */ 683,
/* 2618 */ 0,
/* 2619 */ -2614,
/* 2620 */ 1180,
/* 2621 */ 2625,
/* 2622 */ 50058,
/* 2623 */ -2621,
/* 2624 */ 684,
/* 2625 */ 0,
/* 2626 */ -2621,
/* 2627 */ 1181,
/* 2628 */ 2632,
/* 2629 */ 50058,
/* 2630 */ -2628,
/* 2631 */ 685,
/* 2632 */ 0,
/* 2633 */ -2628,
/* 2634 */ 1182,
/* 2635 */ 2639,
/* 2636 */ 50058,
/* 2637 */ -2635,
/* 2638 */ 686,
/* 2639 */ 0,
/* 2640 */ -2635,
/* 2641 */ 1183,
/* 2642 */ 2646,
/* 2643 */ 50058,
/* 2644 */ -2642,
/* 2645 */ 687,
/* 2646 */ 0,
/* 2647 */ -2642,
/* 2648 */ 1184,
/* 2649 */ 2655,
/* 2650 */ 223,
/* 2651 */ 2658,
/* 2652 */ 2649,
/* 2653 */ -2649,
/* 2654 */ 690,
/* 2655 */ 0,
/* 2656 */ -2649,
/* 2657 */ 1186,
/* 2658 */ 2662,
/* 2659 */ 50044,
/* 2660 */ -2658,
/* 2661 */ 688,
/* 2662 */ 2666,
/* 2663 */ 50124,
/* 2664 */ -2658,
/* 2665 */ 689,
/* 2666 */ 0,
/* 2667 */ -2658,
/* 2668 */ 1185,
/* 2669 */ 0,
/* 2670 */ 2673,
/* 2671 */ -2669,
/* 2672 */ 189,
/* 2673 */ 2679,
/* 2674 */ 223,
/* 2675 */ 2682,
/* 2676 */ 2673,
/* 2677 */ -2673,
/* 2678 */ 693,
/* 2679 */ 0,
/* 2680 */ -2673,
/* 2681 */ 1188,
/* 2682 */ 2686,
/* 2683 */ 50044,
/* 2684 */ -2682,
/* 2685 */ 691,
/* 2686 */ 2690,
/* 2687 */ 50124,
/* 2688 */ -2682,
/* 2689 */ 692,
/* 2690 */ 0,
/* 2691 */ -2682,
/* 2692 */ 1187,
/* 2693 */ 0,
/* 2694 */ 50261,
/* 2695 */ -2693,
/* 2696 */ 190,
/* 2697 */ 0,
/* 2698 */ 50417,
/* 2699 */ 50257,
/* 2700 */ 2706,
/* 2701 */ 50123,
/* 2702 */ 2714,
/* 2703 */ 50125,
/* 2704 */ -2697,
/* 2705 */ 191,
/* 2706 */ 2711,
/* 2707 */ 50058,
/* 2708 */ 163,
/* 2709 */ -2706,
/* 2710 */ 694,
/* 2711 */ 0,
/* 2712 */ -2706,
/* 2713 */ 1189,
/* 2714 */ 2719,
/* 2715 */ 2722,
/* 2716 */ 2714,
/* 2717 */ -2714,
/* 2718 */ 695,
/* 2719 */ 0,
/* 2720 */ -2714,
/* 2721 */ 1190,
/* 2722 */ 2729,
/* 2723 */ 50325,
/* 2724 */ 2807,
/* 2725 */ 50257,
/* 2726 */ 50059,
/* 2727 */ -2722,
/* 2728 */ 192,
/* 2729 */ 2734,
/* 2730 */ 2814,
/* 2731 */ 50059,
/* 2732 */ -2722,
/* 2733 */ 193,
/* 2734 */ 2741,
/* 2735 */ 50358,
/* 2736 */ 2839,
/* 2737 */ 2846,
/* 2738 */ 50059,
/* 2739 */ -2722,
/* 2740 */ 194,
/* 2741 */ 2748,
/* 2742 */ 50387,
/* 2743 */ 2862,
/* 2744 */ 163,
/* 2745 */ 50059,
/* 2746 */ -2722,
/* 2747 */ 195,
/* 2748 */ 2758,
/* 2749 */ 50339,
/* 2750 */ 2869,
/* 2751 */ 2876,
/* 2752 */ 163,
/* 2753 */ 50044,
/* 2754 */ 163,
/* 2755 */ 50059,
/* 2756 */ -2722,
/* 2757 */ 196,
/* 2758 */ 2770,
/* 2759 */ 50339,
/* 2760 */ 2883,
/* 2761 */ 2890,
/* 2762 */ 163,
/* 2763 */ 50044,
/* 2764 */ 163,
/* 2765 */ 50044,
/* 2766 */ 163,
/* 2767 */ 50059,
/* 2768 */ -2722,
/* 2769 */ 197,
/* 2770 */ 2777,
/* 2771 */ 50431,
/* 2772 */ 2897,
/* 2773 */ 2904,
/* 2774 */ 50059,
/* 2775 */ -2722,
/* 2776 */ 198,
/* 2777 */ 2784,
/* 2778 */ 223,
/* 2779 */ 2920,
/* 2780 */ 50257,
/* 2781 */ 50059,
/* 2782 */ -2722,
/* 2783 */ 199,
/* 2784 */ 2791,
/* 2785 */ 50349,
/* 2786 */ 2927,
/* 2787 */ 2934,
/* 2788 */ 50059,
/* 2789 */ -2722,
/* 2790 */ 200,
/* 2791 */ 2795,
/* 2792 */ 433,
/* 2793 */ -2722,
/* 2794 */ 201,
/* 2795 */ 2799,
/* 2796 */ 3423,
/* 2797 */ -2722,
/* 2798 */ 202,
/* 2799 */ 2803,
/* 2800 */ 538,
/* 2801 */ -2722,
/* 2802 */ 203,
/* 2803 */ 0,
/* 2804 */ 6082,
/* 2805 */ -2722,
/* 2806 */ 204,
/* 2807 */ 2811,
/* 2808 */ 50058,
/* 2809 */ -2807,
/* 2810 */ 696,
/* 2811 */ 0,
/* 2812 */ -2807,
/* 2813 */ 1191,
/* 2814 */ 2822,
/* 2815 */ 50276,
/* 2816 */ 2825,
/* 2817 */ 173,
/* 2818 */ 2832,
/* 2819 */ 2814,
/* 2820 */ -2814,
/* 2821 */ 699,
/* 2822 */ 0,
/* 2823 */ -2814,
/* 2824 */ 1194,
/* 2825 */ 2829,
/* 2826 */ 50058,
/* 2827 */ -2825,
/* 2828 */ 697,
/* 2829 */ 0,
/* 2830 */ -2825,
/* 2831 */ 1192,
/* 2832 */ 2836,
/* 2833 */ 50044,
/* 2834 */ -2832,
/* 2835 */ 698,
/* 2836 */ 0,
/* 2837 */ -2832,
/* 2838 */ 1193,
/* 2839 */ 2843,
/* 2840 */ 50058,
/* 2841 */ -2839,
/* 2842 */ 700,
/* 2843 */ 0,
/* 2844 */ -2839,
/* 2845 */ 1195,
/* 2846 */ 2852,
/* 2847 */ 3257,
/* 2848 */ 2855,
/* 2849 */ 2846,
/* 2850 */ -2846,
/* 2851 */ 702,
/* 2852 */ 0,
/* 2853 */ -2846,
/* 2854 */ 1197,
/* 2855 */ 2859,
/* 2856 */ 50044,
/* 2857 */ -2855,
/* 2858 */ 701,
/* 2859 */ 0,
/* 2860 */ -2855,
/* 2861 */ 1196,
/* 2862 */ 2866,
/* 2863 */ 50058,
/* 2864 */ -2862,
/* 2865 */ 703,
/* 2866 */ 0,
/* 2867 */ -2862,
/* 2868 */ 1198,
/* 2869 */ 2873,
/* 2870 */ 50308,
/* 2871 */ -2869,
/* 2872 */ 704,
/* 2873 */ 0,
/* 2874 */ -2869,
/* 2875 */ 1199,
/* 2876 */ 2880,
/* 2877 */ 50058,
/* 2878 */ -2876,
/* 2879 */ 705,
/* 2880 */ 0,
/* 2881 */ -2876,
/* 2882 */ 1200,
/* 2883 */ 2887,
/* 2884 */ 50308,
/* 2885 */ -2883,
/* 2886 */ 706,
/* 2887 */ 0,
/* 2888 */ -2883,
/* 2889 */ 1201,
/* 2890 */ 2894,
/* 2891 */ 50058,
/* 2892 */ -2890,
/* 2893 */ 707,
/* 2894 */ 0,
/* 2895 */ -2890,
/* 2896 */ 1202,
/* 2897 */ 2901,
/* 2898 */ 50058,
/* 2899 */ -2897,
/* 2900 */ 708,
/* 2901 */ 0,
/* 2902 */ -2897,
/* 2903 */ 1203,
/* 2904 */ 2910,
/* 2905 */ 223,
/* 2906 */ 2913,
/* 2907 */ 2904,
/* 2908 */ -2904,
/* 2909 */ 710,
/* 2910 */ 0,
/* 2911 */ -2904,
/* 2912 */ 1205,
/* 2913 */ 2917,
/* 2914 */ 50044,
/* 2915 */ -2913,
/* 2916 */ 709,
/* 2917 */ 0,
/* 2918 */ -2913,
/* 2919 */ 1204,
/* 2920 */ 2924,
/* 2921 */ 50058,
/* 2922 */ -2920,
/* 2923 */ 711,
/* 2924 */ 0,
/* 2925 */ -2920,
/* 2926 */ 1206,
/* 2927 */ 2931,
/* 2928 */ 50058,
/* 2929 */ -2927,
/* 2930 */ 712,
/* 2931 */ 0,
/* 2932 */ -2927,
/* 2933 */ 1207,
/* 2934 */ 2940,
/* 2935 */ 223,
/* 2936 */ 2943,
/* 2937 */ 2934,
/* 2938 */ -2934,
/* 2939 */ 715,
/* 2940 */ 0,
/* 2941 */ -2934,
/* 2942 */ 1209,
/* 2943 */ 2947,
/* 2944 */ 50124,
/* 2945 */ -2943,
/* 2946 */ 713,
/* 2947 */ 2951,
/* 2948 */ 50044,
/* 2949 */ -2943,
/* 2950 */ 714,
/* 2951 */ 0,
/* 2952 */ -2943,
/* 2953 */ 1208,
/* 2954 */ 0,
/* 2955 */ 50308,
/* 2956 */ 50257,
/* 2957 */ 50123,
/* 2958 */ 2962,
/* 2959 */ 50125,
/* 2960 */ -2954,
/* 2961 */ 205,
/* 2962 */ 2967,
/* 2963 */ 2970,
/* 2964 */ 2962,
/* 2965 */ -2962,
/* 2966 */ 716,
/* 2967 */ 0,
/* 2968 */ -2962,
/* 2969 */ 1210,
/* 2970 */ 2977,
/* 2971 */ 50325,
/* 2972 */ 3063,
/* 2973 */ 50257,
/* 2974 */ 50059,
/* 2975 */ -2970,
/* 2976 */ 206,
/* 2977 */ 2985,
/* 2978 */ 50363,
/* 2979 */ 3070,
/* 2980 */ 3077,
/* 2981 */ 223,
/* 2982 */ 50059,
/* 2983 */ -2970,
/* 2984 */ 207,
/* 2985 */ 2993,
/* 2986 */ 50380,
/* 2987 */ 3084,
/* 2988 */ 3091,
/* 2989 */ 223,
/* 2990 */ 50059,
/* 2991 */ -2970,
/* 2992 */ 208,
/* 2993 */ 3002,
/* 2994 */ 50322,
/* 2995 */ 3098,
/* 2996 */ 223,
/* 2997 */ 50047,
/* 2998 */ 223,
/* 2999 */ 50059,
/* 3000 */ -2970,
/* 3001 */ 209,
/* 3002 */ 3009,
/* 3003 */ 50295,
/* 3004 */ 3105,
/* 3005 */ 3112,
/* 3006 */ 50059,
/* 3007 */ -2970,
/* 3008 */ 210,
/* 3009 */ 3016,
/* 3010 */ 50428,
/* 3011 */ 3129,
/* 3012 */ 3136,
/* 3013 */ 50059,
/* 3014 */ -2970,
/* 3015 */ 211,
/* 3016 */ 3023,
/* 3017 */ 50358,
/* 3018 */ 3153,
/* 3019 */ 3160,
/* 3020 */ 50059,
/* 3021 */ -2970,
/* 3022 */ 212,
/* 3023 */ 3033,
/* 3024 */ 50431,
/* 3025 */ 3176,
/* 3026 */ 223,
/* 3027 */ 3180,
/* 3028 */ 3184,
/* 3029 */ 3191,
/* 3030 */ 50059,
/* 3031 */ -2970,
/* 3032 */ 213,
/* 3033 */ 3040,
/* 3034 */ 50410,
/* 3035 */ 3207,
/* 3036 */ 3214,
/* 3037 */ 50059,
/* 3038 */ -2970,
/* 3039 */ 214,
/* 3040 */ 3047,
/* 3041 */ 50349,
/* 3042 */ 3230,
/* 3043 */ 3237,
/* 3044 */ 50059,
/* 3045 */ -2970,
/* 3046 */ 215,
/* 3047 */ 3051,
/* 3048 */ 433,
/* 3049 */ -2970,
/* 3050 */ 216,
/* 3051 */ 3055,
/* 3052 */ 538,
/* 3053 */ -2970,
/* 3054 */ 217,
/* 3055 */ 3059,
/* 3056 */ 3423,
/* 3057 */ -2970,
/* 3058 */ 218,
/* 3059 */ 0,
/* 3060 */ 6082,
/* 3061 */ -2970,
/* 3062 */ 219,
/* 3063 */ 3067,
/* 3064 */ 50058,
/* 3065 */ -3063,
/* 3066 */ 717,
/* 3067 */ 0,
/* 3068 */ -3063,
/* 3069 */ 1211,
/* 3070 */ 3074,
/* 3071 */ 50058,
/* 3072 */ -3070,
/* 3073 */ 718,
/* 3074 */ 0,
/* 3075 */ -3070,
/* 3076 */ 1212,
/* 3077 */ 3081,
/* 3078 */ 50100,
/* 3079 */ -3077,
/* 3080 */ 719,
/* 3081 */ 0,
/* 3082 */ -3077,
/* 3083 */ 1213,
/* 3084 */ 3088,
/* 3085 */ 50058,
/* 3086 */ -3084,
/* 3087 */ 720,
/* 3088 */ 0,
/* 3089 */ -3084,
/* 3090 */ 1214,
/* 3091 */ 3095,
/* 3092 */ 50100,
/* 3093 */ -3091,
/* 3094 */ 721,
/* 3095 */ 0,
/* 3096 */ -3091,
/* 3097 */ 1215,
/* 3098 */ 3102,
/* 3099 */ 50058,
/* 3100 */ -3098,
/* 3101 */ 722,
/* 3102 */ 0,
/* 3103 */ -3098,
/* 3104 */ 1216,
/* 3105 */ 3109,
/* 3106 */ 50058,
/* 3107 */ -3105,
/* 3108 */ 723,
/* 3109 */ 0,
/* 3110 */ -3105,
/* 3111 */ 1217,
/* 3112 */ 3119,
/* 3113 */ 223,
/* 3114 */ 223,
/* 3115 */ 3122,
/* 3116 */ 3112,
/* 3117 */ -3112,
/* 3118 */ 725,
/* 3119 */ 0,
/* 3120 */ -3112,
/* 3121 */ 1219,
/* 3122 */ 3126,
/* 3123 */ 50044,
/* 3124 */ -3122,
/* 3125 */ 724,
/* 3126 */ 0,
/* 3127 */ -3122,
/* 3128 */ 1218,
/* 3129 */ 3133,
/* 3130 */ 50058,
/* 3131 */ -3129,
/* 3132 */ 726,
/* 3133 */ 0,
/* 3134 */ -3129,
/* 3135 */ 1220,
/* 3136 */ 3143,
/* 3137 */ 223,
/* 3138 */ 223,
/* 3139 */ 3146,
/* 3140 */ 3136,
/* 3141 */ -3136,
/* 3142 */ 728,
/* 3143 */ 0,
/* 3144 */ -3136,
/* 3145 */ 1222,
/* 3146 */ 3150,
/* 3147 */ 50044,
/* 3148 */ -3146,
/* 3149 */ 727,
/* 3150 */ 0,
/* 3151 */ -3146,
/* 3152 */ 1221,
/* 3153 */ 3157,
/* 3154 */ 50058,
/* 3155 */ -3153,
/* 3156 */ 729,
/* 3157 */ 0,
/* 3158 */ -3153,
/* 3159 */ 1223,
/* 3160 */ 3166,
/* 3161 */ 3257,
/* 3162 */ 3169,
/* 3163 */ 3160,
/* 3164 */ -3160,
/* 3165 */ 731,
/* 3166 */ 0,
/* 3167 */ -3160,
/* 3168 */ 1225,
/* 3169 */ 3173,
/* 3170 */ 50044,
/* 3171 */ -3169,
/* 3172 */ 730,
/* 3173 */ 0,
/* 3174 */ -3169,
/* 3175 */ 1224,
/* 3176 */ 0,
/* 3177 */ 50091,
/* 3178 */ -3176,
/* 3179 */ 732,
/* 3180 */ 0,
/* 3181 */ 50093,
/* 3182 */ -3180,
/* 3183 */ 733,
/* 3184 */ 3188,
/* 3185 */ 50058,
/* 3186 */ -3184,
/* 3187 */ 734,
/* 3188 */ 0,
/* 3189 */ -3184,
/* 3190 */ 1226,
/* 3191 */ 3197,
/* 3192 */ 223,
/* 3193 */ 3200,
/* 3194 */ 3191,
/* 3195 */ -3191,
/* 3196 */ 736,
/* 3197 */ 0,
/* 3198 */ -3191,
/* 3199 */ 1228,
/* 3200 */ 3204,
/* 3201 */ 50044,
/* 3202 */ -3200,
/* 3203 */ 735,
/* 3204 */ 0,
/* 3205 */ -3200,
/* 3206 */ 1227,
/* 3207 */ 3211,
/* 3208 */ 50058,
/* 3209 */ -3207,
/* 3210 */ 737,
/* 3211 */ 0,
/* 3212 */ -3207,
/* 3213 */ 1229,
/* 3214 */ 3220,
/* 3215 */ 223,
/* 3216 */ 3223,
/* 3217 */ 3214,
/* 3218 */ -3214,
/* 3219 */ 739,
/* 3220 */ 0,
/* 3221 */ -3214,
/* 3222 */ 1231,
/* 3223 */ 3227,
/* 3224 */ 50044,
/* 3225 */ -3223,
/* 3226 */ 738,
/* 3227 */ 0,
/* 3228 */ -3223,
/* 3229 */ 1230,
/* 3230 */ 3234,
/* 3231 */ 50058,
/* 3232 */ -3230,
/* 3233 */ 740,
/* 3234 */ 0,
/* 3235 */ -3230,
/* 3236 */ 1232,
/* 3237 */ 3243,
/* 3238 */ 223,
/* 3239 */ 3246,
/* 3240 */ 3237,
/* 3241 */ -3237,
/* 3242 */ 743,
/* 3243 */ 0,
/* 3244 */ -3237,
/* 3245 */ 1234,
/* 3246 */ 3250,
/* 3247 */ 50124,
/* 3248 */ -3246,
/* 3249 */ 741,
/* 3250 */ 3254,
/* 3251 */ 50044,
/* 3252 */ -3246,
/* 3253 */ 742,
/* 3254 */ 0,
/* 3255 */ -3246,
/* 3256 */ 1233,
/* 3257 */ 3267,
/* 3258 */ 50281,
/* 3259 */ 50091,
/* 3260 */ 223,
/* 3261 */ 3287,
/* 3262 */ 50093,
/* 3263 */ 50294,
/* 3264 */ 3325,
/* 3265 */ -3257,
/* 3266 */ 220,
/* 3267 */ 3277,
/* 3268 */ 50439,
/* 3269 */ 50091,
/* 3270 */ 223,
/* 3271 */ 3294,
/* 3272 */ 50093,
/* 3273 */ 50294,
/* 3274 */ 3325,
/* 3275 */ -3257,
/* 3276 */ 221,
/* 3277 */ 0,
/* 3278 */ 50346,
/* 3279 */ 50091,
/* 3280 */ 223,
/* 3281 */ 3301,
/* 3282 */ 50093,
/* 3283 */ 50294,
/* 3284 */ 3325,
/* 3285 */ -3257,
/* 3286 */ 222,
/* 3287 */ 3291,
/* 3288 */ 3308,
/* 3289 */ -3287,
/* 3290 */ 744,
/* 3291 */ 0,
/* 3292 */ -3287,
/* 3293 */ 1235,
/* 3294 */ 3298,
/* 3295 */ 3308,
/* 3296 */ -3294,
/* 3297 */ 745,
/* 3298 */ 0,
/* 3299 */ -3294,
/* 3300 */ 1236,
/* 3301 */ 3305,
/* 3302 */ 3308,
/* 3303 */ -3301,
/* 3304 */ 746,
/* 3305 */ 0,
/* 3306 */ -3301,
/* 3307 */ 1237,
/* 3308 */ 3313,
/* 3309 */ 50044,
/* 3310 */ 223,
/* 3311 */ -3308,
/* 3312 */ 223,
/* 3313 */ 3318,
/* 3314 */ 50044,
/* 3315 */ 163,
/* 3316 */ -3308,
/* 3317 */ 224,
/* 3318 */ 0,
/* 3319 */ 50044,
/* 3320 */ 223,
/* 3321 */ 50044,
/* 3322 */ 223,
/* 3323 */ -3308,
/* 3324 */ 225,
/* 3325 */ 3330,
/* 3326 */ 223,
/* 3327 */ 3360,
/* 3328 */ -3325,
/* 3329 */ 226,
/* 3330 */ 3336,
/* 3331 */ 50342,
/* 3332 */ 223,
/* 3333 */ 50373,
/* 3334 */ -3325,
/* 3335 */ 227,
/* 3336 */ 3341,
/* 3337 */ 50342,
/* 3338 */ 50373,
/* 3339 */ -3325,
/* 3340 */ 228,
/* 3341 */ 3350,
/* 3342 */ 50342,
/* 3343 */ 50373,
/* 3344 */ 50438,
/* 3345 */ 50294,
/* 3346 */ 223,
/* 3347 */ 3367,
/* 3348 */ -3325,
/* 3349 */ 229,
/* 3350 */ 0,
/* 3351 */ 50342,
/* 3352 */ 223,
/* 3353 */ 50373,
/* 3354 */ 50438,
/* 3355 */ 50294,
/* 3356 */ 223,
/* 3357 */ 3374,
/* 3358 */ -3325,
/* 3359 */ 230,
/* 3360 */ 3364,
/* 3361 */ 50373,
/* 3362 */ -3360,
/* 3363 */ 747,
/* 3364 */ 0,
/* 3365 */ -3360,
/* 3366 */ 1238,
/* 3367 */ 3371,
/* 3368 */ 50373,
/* 3369 */ -3367,
/* 3370 */ 748,
/* 3371 */ 0,
/* 3372 */ -3367,
/* 3373 */ 1239,
/* 3374 */ 3378,
/* 3375 */ 50373,
/* 3376 */ -3374,
/* 3377 */ 749,
/* 3378 */ 0,
/* 3379 */ -3374,
/* 3380 */ 1240,
/* 3381 */ 0,
/* 3382 */ 50331,
/* 3383 */ 50257,
/* 3384 */ 50123,
/* 3385 */ 3389,
/* 3386 */ 50125,
/* 3387 */ -3381,
/* 3388 */ 231,
/* 3389 */ 3394,
/* 3390 */ 3397,
/* 3391 */ 3389,
/* 3392 */ -3389,
/* 3393 */ 750,
/* 3394 */ 0,
/* 3395 */ -3389,
/* 3396 */ 1241,
/* 3397 */ 3404,
/* 3398 */ 50325,
/* 3399 */ 3416,
/* 3400 */ 50257,
/* 3401 */ 50059,
/* 3402 */ -3397,
/* 3403 */ 232,
/* 3404 */ 3408,
/* 3405 */ 3423,
/* 3406 */ -3397,
/* 3407 */ 233,
/* 3408 */ 3412,
/* 3409 */ 538,
/* 3410 */ -3397,
/* 3411 */ 234,
/* 3412 */ 0,
/* 3413 */ 6082,
/* 3414 */ -3397,
/* 3415 */ 235,
/* 3416 */ 3420,
/* 3417 */ 50058,
/* 3418 */ -3416,
/* 3419 */ 751,
/* 3420 */ 0,
/* 3421 */ -3416,
/* 3422 */ 1242,
/* 3423 */ 3430,
/* 3424 */ 50310,
/* 3425 */ 3443,
/* 3426 */ 3450,
/* 3427 */ 50059,
/* 3428 */ -3423,
/* 3429 */ 236,
/* 3430 */ 3436,
/* 3431 */ 50376,
/* 3432 */ 3483,
/* 3433 */ 3490,
/* 3434 */ -3423,
/* 3435 */ 237,
/* 3436 */ 0,
/* 3437 */ 50433,
/* 3438 */ 3517,
/* 3439 */ 3524,
/* 3440 */ 50059,
/* 3441 */ -3423,
/* 3442 */ 238,
/* 3443 */ 3447,
/* 3444 */ 50058,
/* 3445 */ -3443,
/* 3446 */ 752,
/* 3447 */ 0,
/* 3448 */ -3443,
/* 3449 */ 1243,
/* 3450 */ 3458,
/* 3451 */ 50042,
/* 3452 */ 223,
/* 3453 */ 223,
/* 3454 */ 3469,
/* 3455 */ 3450,
/* 3456 */ -3450,
/* 3457 */ 754,
/* 3458 */ 3466,
/* 3459 */ 50042,
/* 3460 */ 223,
/* 3461 */ 163,
/* 3462 */ 3476,
/* 3463 */ 3450,
/* 3464 */ -3450,
/* 3465 */ 756,
/* 3466 */ 0,
/* 3467 */ -3450,
/* 3468 */ 1246,
/* 3469 */ 3473,
/* 3470 */ 50044,
/* 3471 */ -3469,
/* 3472 */ 753,
/* 3473 */ 0,
/* 3474 */ -3469,
/* 3475 */ 1244,
/* 3476 */ 3480,
/* 3477 */ 50044,
/* 3478 */ -3476,
/* 3479 */ 755,
/* 3480 */ 0,
/* 3481 */ -3476,
/* 3482 */ 1245,
/* 3483 */ 3487,
/* 3484 */ 50058,
/* 3485 */ -3483,
/* 3486 */ 757,
/* 3487 */ 0,
/* 3488 */ -3483,
/* 3489 */ 1247,
/* 3490 */ 3498,
/* 3491 */ 50042,
/* 3492 */ 237,
/* 3493 */ 3501,
/* 3494 */ 3509,
/* 3495 */ 3490,
/* 3496 */ -3490,
/* 3497 */ 761,
/* 3498 */ 0,
/* 3499 */ -3490,
/* 3500 */ 1249,
/* 3501 */ 3506,
/* 3502 */ 3571,
/* 3503 */ 3501,
/* 3504 */ -3501,
/* 3505 */ 758,
/* 3506 */ 0,
/* 3507 */ -3501,
/* 3508 */ 1248,
/* 3509 */ 3513,
/* 3510 */ 50044,
/* 3511 */ -3509,
/* 3512 */ 759,
/* 3513 */ 0,
/* 3514 */ 50059,
/* 3515 */ -3509,
/* 3516 */ 760,
/* 3517 */ 3521,
/* 3518 */ 50058,
/* 3519 */ -3517,
/* 3520 */ 762,
/* 3521 */ 0,
/* 3522 */ -3517,
/* 3523 */ 1250,
/* 3524 */ 3532,
/* 3525 */ 50042,
/* 3526 */ 3535,
/* 3527 */ 3543,
/* 3528 */ 3564,
/* 3529 */ 3524,
/* 3530 */ -3524,
/* 3531 */ 768,
/* 3532 */ 0,
/* 3533 */ -3524,
/* 3534 */ 1253,
/* 3535 */ 3540,
/* 3536 */ 237,
/* 3537 */ 50037,
/* 3538 */ -3535,
/* 3539 */ 763,
/* 3540 */ 0,
/* 3541 */ -3535,
/* 3542 */ 1251,
/* 3543 */ 3550,
/* 3544 */ 163,
/* 3545 */ 50294,
/* 3546 */ 50373,
/* 3547 */ 223,
/* 3548 */ -3543,
/* 3549 */ 764,
/* 3550 */ 3557,
/* 3551 */ 163,
/* 3552 */ 50294,
/* 3553 */ 223,
/* 3554 */ 50373,
/* 3555 */ -3543,
/* 3556 */ 765,
/* 3557 */ 0,
/* 3558 */ 163,
/* 3559 */ 50294,
/* 3560 */ 50373,
/* 3561 */ 286,
/* 3562 */ -3543,
/* 3563 */ 766,
/* 3564 */ 3568,
/* 3565 */ 50044,
/* 3566 */ -3564,
/* 3567 */ 767,
/* 3568 */ 0,
/* 3569 */ -3564,
/* 3570 */ 1252,
/* 3571 */ 3575,
/* 3572 */ 163,
/* 3573 */ -3571,
/* 3574 */ 239,
/* 3575 */ 3579,
/* 3576 */ 237,
/* 3577 */ -3571,
/* 3578 */ 240,
/* 3579 */ 0,
/* 3580 */ 50257,
/* 3581 */ -3571,
/* 3582 */ 241,
/* 3583 */ 0,
/* 3584 */ 50292,
/* 3585 */ 50257,
/* 3586 */ 50123,
/* 3587 */ 3591,
/* 3588 */ 50125,
/* 3589 */ -3583,
/* 3590 */ 242,
/* 3591 */ 3596,
/* 3592 */ 3599,
/* 3593 */ 3591,
/* 3594 */ -3591,
/* 3595 */ 769,
/* 3596 */ 0,
/* 3597 */ -3591,
/* 3598 */ 1254,
/* 3599 */ 3606,
/* 3600 */ 50325,
/* 3601 */ 3675,
/* 3602 */ 50257,
/* 3603 */ 50059,
/* 3604 */ -3599,
/* 3605 */ 243,
/* 3606 */ 3613,
/* 3607 */ 50371,
/* 3608 */ 3682,
/* 3609 */ 163,
/* 3610 */ 50059,
/* 3611 */ -3599,
/* 3612 */ 244,
/* 3613 */ 3618,
/* 3614 */ 3689,
/* 3615 */ 50059,
/* 3616 */ -3599,
/* 3617 */ 245,
/* 3618 */ 3625,
/* 3619 */ 50423,
/* 3620 */ 3714,
/* 3621 */ 3721,
/* 3622 */ 50059,
/* 3623 */ -3599,
/* 3624 */ 246,
/* 3625 */ 3632,
/* 3626 */ 50443,
/* 3627 */ 3737,
/* 3628 */ 3744,
/* 3629 */ 50059,
/* 3630 */ -3599,
/* 3631 */ 247,
/* 3632 */ 3639,
/* 3633 */ 50414,
/* 3634 */ 3760,
/* 3635 */ 3767,
/* 3636 */ 50059,
/* 3637 */ -3599,
/* 3638 */ 248,
/* 3639 */ 3646,
/* 3640 */ 50337,
/* 3641 */ 50352,
/* 3642 */ 3783,
/* 3643 */ 50059,
/* 3644 */ -3599,
/* 3645 */ 249,
/* 3646 */ 3653,
/* 3647 */ 50469,
/* 3648 */ 50352,
/* 3649 */ 3796,
/* 3650 */ 50059,
/* 3651 */ -3599,
/* 3652 */ 250,
/* 3653 */ 3660,
/* 3654 */ 50365,
/* 3655 */ 50352,
/* 3656 */ 3809,
/* 3657 */ 50059,
/* 3658 */ -3599,
/* 3659 */ 251,
/* 3660 */ 3667,
/* 3661 */ 50370,
/* 3662 */ 50352,
/* 3663 */ 3822,
/* 3664 */ 50059,
/* 3665 */ -3599,
/* 3666 */ 252,
/* 3667 */ 3671,
/* 3668 */ 538,
/* 3669 */ -3599,
/* 3670 */ 253,
/* 3671 */ 0,
/* 3672 */ 6082,
/* 3673 */ -3599,
/* 3674 */ 254,
/* 3675 */ 3679,
/* 3676 */ 50058,
/* 3677 */ -3675,
/* 3678 */ 770,
/* 3679 */ 0,
/* 3680 */ -3675,
/* 3681 */ 1255,
/* 3682 */ 3686,
/* 3683 */ 50058,
/* 3684 */ -3682,
/* 3685 */ 771,
/* 3686 */ 0,
/* 3687 */ -3682,
/* 3688 */ 1256,
/* 3689 */ 3697,
/* 3690 */ 50276,
/* 3691 */ 3700,
/* 3692 */ 223,
/* 3693 */ 3707,
/* 3694 */ 3689,
/* 3695 */ -3689,
/* 3696 */ 774,
/* 3697 */ 0,
/* 3698 */ -3689,
/* 3699 */ 1259,
/* 3700 */ 3704,
/* 3701 */ 50058,
/* 3702 */ -3700,
/* 3703 */ 772,
/* 3704 */ 0,
/* 3705 */ -3700,
/* 3706 */ 1257,
/* 3707 */ 3711,
/* 3708 */ 50044,
/* 3709 */ -3707,
/* 3710 */ 773,
/* 3711 */ 0,
/* 3712 */ -3707,
/* 3713 */ 1258,
/* 3714 */ 3718,
/* 3715 */ 50058,
/* 3716 */ -3714,
/* 3717 */ 775,
/* 3718 */ 0,
/* 3719 */ -3714,
/* 3720 */ 1260,
/* 3721 */ 3727,
/* 3722 */ 223,
/* 3723 */ 3730,
/* 3724 */ 3721,
/* 3725 */ -3721,
/* 3726 */ 777,
/* 3727 */ 0,
/* 3728 */ -3721,
/* 3729 */ 1262,
/* 3730 */ 3734,
/* 3731 */ 50044,
/* 3732 */ -3730,
/* 3733 */ 776,
/* 3734 */ 0,
/* 3735 */ -3730,
/* 3736 */ 1261,
/* 3737 */ 3741,
/* 3738 */ 50058,
/* 3739 */ -3737,
/* 3740 */ 778,
/* 3741 */ 0,
/* 3742 */ -3737,
/* 3743 */ 1263,
/* 3744 */ 3750,
/* 3745 */ 50276,
/* 3746 */ 3753,
/* 3747 */ 3744,
/* 3748 */ -3744,
/* 3749 */ 780,
/* 3750 */ 0,
/* 3751 */ -3744,
/* 3752 */ 1265,
/* 3753 */ 3757,
/* 3754 */ 50044,
/* 3755 */ -3753,
/* 3756 */ 779,
/* 3757 */ 0,
/* 3758 */ -3753,
/* 3759 */ 1264,
/* 3760 */ 3764,
/* 3761 */ 50058,
/* 3762 */ -3760,
/* 3763 */ 781,
/* 3764 */ 0,
/* 3765 */ -3760,
/* 3766 */ 1266,
/* 3767 */ 3773,
/* 3768 */ 223,
/* 3769 */ 3776,
/* 3770 */ 3767,
/* 3771 */ -3767,
/* 3772 */ 783,
/* 3773 */ 0,
/* 3774 */ -3767,
/* 3775 */ 1268,
/* 3776 */ 3780,
/* 3777 */ 50044,
/* 3778 */ -3776,
/* 3779 */ 782,
/* 3780 */ 0,
/* 3781 */ -3776,
/* 3782 */ 1267,
/* 3783 */ 3789,
/* 3784 */ 3835,
/* 3785 */ 3792,
/* 3786 */ 3783,
/* 3787 */ -3783,
/* 3788 */ 785,
/* 3789 */ 0,
/* 3790 */ -3783,
/* 3791 */ 1269,
/* 3792 */ 0,
/* 3793 */ 50044,
/* 3794 */ -3792,
/* 3795 */ 784,
/* 3796 */ 3802,
/* 3797 */ 3835,
/* 3798 */ 3805,
/* 3799 */ 3796,
/* 3800 */ -3796,
/* 3801 */ 787,
/* 3802 */ 0,
/* 3803 */ -3796,
/* 3804 */ 1270,
/* 3805 */ 0,
/* 3806 */ 50044,
/* 3807 */ -3805,
/* 3808 */ 786,
/* 3809 */ 3815,
/* 3810 */ 3835,
/* 3811 */ 3818,
/* 3812 */ 3809,
/* 3813 */ -3809,
/* 3814 */ 789,
/* 3815 */ 0,
/* 3816 */ -3809,
/* 3817 */ 1271,
/* 3818 */ 0,
/* 3819 */ 50044,
/* 3820 */ -3818,
/* 3821 */ 788,
/* 3822 */ 3828,
/* 3823 */ 3835,
/* 3824 */ 3831,
/* 3825 */ 3822,
/* 3826 */ -3822,
/* 3827 */ 791,
/* 3828 */ 0,
/* 3829 */ -3822,
/* 3830 */ 1272,
/* 3831 */ 0,
/* 3832 */ 50044,
/* 3833 */ -3831,
/* 3834 */ 790,
/* 3835 */ 3844,
/* 3836 */ 50439,
/* 3837 */ 50091,
/* 3838 */ 223,
/* 3839 */ 50044,
/* 3840 */ 223,
/* 3841 */ 50093,
/* 3842 */ -3835,
/* 3843 */ 255,
/* 3844 */ 3851,
/* 3845 */ 50439,
/* 3846 */ 50091,
/* 3847 */ 223,
/* 3848 */ 50093,
/* 3849 */ -3835,
/* 3850 */ 256,
/* 3851 */ 3856,
/* 3852 */ 163,
/* 3853 */ 3887,
/* 3854 */ -3835,
/* 3855 */ 257,
/* 3856 */ 3863,
/* 3857 */ 163,
/* 3858 */ 50342,
/* 3859 */ 259,
/* 3860 */ 50458,
/* 3861 */ -3835,
/* 3862 */ 258,
/* 3863 */ 3870,
/* 3864 */ 163,
/* 3865 */ 223,
/* 3866 */ 3895,
/* 3867 */ 50320,
/* 3868 */ -3835,
/* 3869 */ 259,
/* 3870 */ 3877,
/* 3871 */ 163,
/* 3872 */ 223,
/* 3873 */ 3903,
/* 3874 */ 50464,
/* 3875 */ -3835,
/* 3876 */ 260,
/* 3877 */ 3882,
/* 3878 */ 50369,
/* 3879 */ 163,
/* 3880 */ -3835,
/* 3881 */ 261,
/* 3882 */ 0,
/* 3883 */ 50346,
/* 3884 */ 223,
/* 3885 */ -3835,
/* 3886 */ 262,
/* 3887 */ 3892,
/* 3888 */ 50294,
/* 3889 */ 50278,
/* 3890 */ -3887,
/* 3891 */ 792,
/* 3892 */ 0,
/* 3893 */ -3887,
/* 3894 */ 1273,
/* 3895 */ 3899,
/* 3896 */ 50047,
/* 3897 */ -3895,
/* 3898 */ 793,
/* 3899 */ 0,
/* 3900 */ 50407,
/* 3901 */ -3895,
/* 3902 */ 794,
/* 3903 */ 3907,
/* 3904 */ 50047,
/* 3905 */ -3903,
/* 3906 */ 795,
/* 3907 */ 0,
/* 3908 */ 50407,
/* 3909 */ -3903,
/* 3910 */ 796,
/* 3911 */ 0,
/* 3912 */ 50327,
/* 3913 */ 50257,
/* 3914 */ 50058,
/* 3915 */ 223,
/* 3916 */ 50123,
/* 3917 */ 3921,
/* 3918 */ 50125,
/* 3919 */ -3911,
/* 3920 */ 263,
/* 3921 */ 3926,
/* 3922 */ 3929,
/* 3923 */ 3921,
/* 3924 */ -3921,
/* 3925 */ 797,
/* 3926 */ 0,
/* 3927 */ -3921,
/* 3928 */ 1274,
/* 3929 */ 3936,
/* 3930 */ 50436,
/* 3931 */ 3962,
/* 3932 */ 3969,
/* 3933 */ 50059,
/* 3934 */ -3929,
/* 3935 */ 264,
/* 3936 */ 3943,
/* 3937 */ 50358,
/* 3938 */ 3985,
/* 3939 */ 3992,
/* 3940 */ 50059,
/* 3941 */ -3929,
/* 3942 */ 265,
/* 3943 */ 3950,
/* 3944 */ 50325,
/* 3945 */ 4008,
/* 3946 */ 50257,
/* 3947 */ 50059,
/* 3948 */ -3929,
/* 3949 */ 266,
/* 3950 */ 3954,
/* 3951 */ 538,
/* 3952 */ -3929,
/* 3953 */ 267,
/* 3954 */ 3958,
/* 3955 */ 3423,
/* 3956 */ -3929,
/* 3957 */ 268,
/* 3958 */ 0,
/* 3959 */ 6082,
/* 3960 */ -3929,
/* 3961 */ 269,
/* 3962 */ 3966,
/* 3963 */ 50058,
/* 3964 */ -3962,
/* 3965 */ 798,
/* 3966 */ 0,
/* 3967 */ -3962,
/* 3968 */ 1275,
/* 3969 */ 3975,
/* 3970 */ 163,
/* 3971 */ 3978,
/* 3972 */ 3969,
/* 3973 */ -3969,
/* 3974 */ 800,
/* 3975 */ 0,
/* 3976 */ -3969,
/* 3977 */ 1277,
/* 3978 */ 3982,
/* 3979 */ 50044,
/* 3980 */ -3978,
/* 3981 */ 799,
/* 3982 */ 0,
/* 3983 */ -3978,
/* 3984 */ 1276,
/* 3985 */ 3989,
/* 3986 */ 50058,
/* 3987 */ -3985,
/* 3988 */ 801,
/* 3989 */ 0,
/* 3990 */ -3985,
/* 3991 */ 1278,
/* 3992 */ 3998,
/* 3993 */ 3257,
/* 3994 */ 4001,
/* 3995 */ 3992,
/* 3996 */ -3992,
/* 3997 */ 803,
/* 3998 */ 0,
/* 3999 */ -3992,
/* 4000 */ 1280,
/* 4001 */ 4005,
/* 4002 */ 50044,
/* 4003 */ -4001,
/* 4004 */ 802,
/* 4005 */ 0,
/* 4006 */ -4001,
/* 4007 */ 1279,
/* 4008 */ 4012,
/* 4009 */ 50058,
/* 4010 */ -4008,
/* 4011 */ 804,
/* 4012 */ 0,
/* 4013 */ -4008,
/* 4014 */ 1281,
/* 4015 */ 0,
/* 4016 */ 50357,
/* 4017 */ 50257,
/* 4018 */ 50123,
/* 4019 */ 4023,
/* 4020 */ 50125,
/* 4021 */ -4015,
/* 4022 */ 270,
/* 4023 */ 4028,
/* 4024 */ 4031,
/* 4025 */ 4023,
/* 4026 */ -4023,
/* 4027 */ 805,
/* 4028 */ 0,
/* 4029 */ -4023,
/* 4030 */ 1282,
/* 4031 */ 4038,
/* 4032 */ 50328,
/* 4033 */ 4076,
/* 4034 */ 4083,
/* 4035 */ 50059,
/* 4036 */ -4031,
/* 4037 */ 271,
/* 4038 */ 4043,
/* 4039 */ 50310,
/* 4040 */ 50059,
/* 4041 */ -4031,
/* 4042 */ 272,
/* 4043 */ 4050,
/* 4044 */ 50349,
/* 4045 */ 4099,
/* 4046 */ 4106,
/* 4047 */ 50059,
/* 4048 */ -4031,
/* 4049 */ 273,
/* 4050 */ 4057,
/* 4051 */ 50325,
/* 4052 */ 4126,
/* 4053 */ 50257,
/* 4054 */ 50059,
/* 4055 */ -4031,
/* 4056 */ 274,
/* 4057 */ 4064,
/* 4058 */ 50358,
/* 4059 */ 4133,
/* 4060 */ 4140,
/* 4061 */ 50059,
/* 4062 */ -4031,
/* 4063 */ 275,
/* 4064 */ 4068,
/* 4065 */ 538,
/* 4066 */ -4031,
/* 4067 */ 276,
/* 4068 */ 4072,
/* 4069 */ 3423,
/* 4070 */ -4031,
/* 4071 */ 277,
/* 4072 */ 0,
/* 4073 */ 6082,
/* 4074 */ -4031,
/* 4075 */ 278,
/* 4076 */ 4080,
/* 4077 */ 50058,
/* 4078 */ -4076,
/* 4079 */ 806,
/* 4080 */ 0,
/* 4081 */ -4076,
/* 4082 */ 1283,
/* 4083 */ 4089,
/* 4084 */ 163,
/* 4085 */ 4092,
/* 4086 */ 4083,
/* 4087 */ -4083,
/* 4088 */ 808,
/* 4089 */ 0,
/* 4090 */ -4083,
/* 4091 */ 1285,
/* 4092 */ 4096,
/* 4093 */ 50044,
/* 4094 */ -4092,
/* 4095 */ 807,
/* 4096 */ 0,
/* 4097 */ -4092,
/* 4098 */ 1284,
/* 4099 */ 4103,
/* 4100 */ 50058,
/* 4101 */ -4099,
/* 4102 */ 809,
/* 4103 */ 0,
/* 4104 */ -4099,
/* 4105 */ 1286,
/* 4106 */ 4112,
/* 4107 */ 223,
/* 4108 */ 4115,
/* 4109 */ 4106,
/* 4110 */ -4106,
/* 4111 */ 812,
/* 4112 */ 0,
/* 4113 */ -4106,
/* 4114 */ 1288,
/* 4115 */ 4119,
/* 4116 */ 50124,
/* 4117 */ -4115,
/* 4118 */ 810,
/* 4119 */ 4123,
/* 4120 */ 50044,
/* 4121 */ -4115,
/* 4122 */ 811,
/* 4123 */ 0,
/* 4124 */ -4115,
/* 4125 */ 1287,
/* 4126 */ 4130,
/* 4127 */ 50058,
/* 4128 */ -4126,
/* 4129 */ 813,
/* 4130 */ 0,
/* 4131 */ -4126,
/* 4132 */ 1289,
/* 4133 */ 4137,
/* 4134 */ 50058,
/* 4135 */ -4133,
/* 4136 */ 814,
/* 4137 */ 0,
/* 4138 */ -4133,
/* 4139 */ 1290,
/* 4140 */ 4146,
/* 4141 */ 3257,
/* 4142 */ 4149,
/* 4143 */ 4140,
/* 4144 */ -4140,
/* 4145 */ 816,
/* 4146 */ 0,
/* 4147 */ -4140,
/* 4148 */ 1292,
/* 4149 */ 4153,
/* 4150 */ 50044,
/* 4151 */ -4149,
/* 4152 */ 815,
/* 4153 */ 0,
/* 4154 */ -4149,
/* 4155 */ 1291,
/* 4156 */ 0,
/* 4157 */ 50449,
/* 4158 */ 50257,
/* 4159 */ 50123,
/* 4160 */ 4164,
/* 4161 */ 50125,
/* 4162 */ -4156,
/* 4163 */ 279,
/* 4164 */ 4169,
/* 4165 */ 4172,
/* 4166 */ 4164,
/* 4167 */ -4164,
/* 4168 */ 817,
/* 4169 */ 0,
/* 4170 */ -4164,
/* 4171 */ 1293,
/* 4172 */ 4179,
/* 4173 */ 50325,
/* 4174 */ 4224,
/* 4175 */ 50257,
/* 4176 */ 50059,
/* 4177 */ -4172,
/* 4178 */ 280,
/* 4179 */ 4183,
/* 4180 */ 462,
/* 4181 */ -4172,
/* 4182 */ 281,
/* 4183 */ 4191,
/* 4184 */ 50388,
/* 4185 */ 4231,
/* 4186 */ 223,
/* 4187 */ 50037,
/* 4188 */ 50059,
/* 4189 */ -4172,
/* 4190 */ 282,
/* 4191 */ 4198,
/* 4192 */ 50406,
/* 4193 */ 4238,
/* 4194 */ 223,
/* 4195 */ 50059,
/* 4196 */ -4172,
/* 4197 */ 283,
/* 4198 */ 4205,
/* 4199 */ 50382,
/* 4200 */ 4245,
/* 4201 */ 223,
/* 4202 */ 50059,
/* 4203 */ -4172,
/* 4204 */ 284,
/* 4205 */ 4212,
/* 4206 */ 50349,
/* 4207 */ 4252,
/* 4208 */ 4259,
/* 4209 */ 50059,
/* 4210 */ -4172,
/* 4211 */ 285,
/* 4212 */ 4216,
/* 4213 */ 3423,
/* 4214 */ -4172,
/* 4215 */ 286,
/* 4216 */ 4220,
/* 4217 */ 538,
/* 4218 */ -4172,
/* 4219 */ 287,
/* 4220 */ 0,
/* 4221 */ 6082,
/* 4222 */ -4172,
/* 4223 */ 288,
/* 4224 */ 4228,
/* 4225 */ 50058,
/* 4226 */ -4224,
/* 4227 */ 818,
/* 4228 */ 0,
/* 4229 */ -4224,
/* 4230 */ 1294,
/* 4231 */ 4235,
/* 4232 */ 50058,
/* 4233 */ -4231,
/* 4234 */ 819,
/* 4235 */ 0,
/* 4236 */ -4231,
/* 4237 */ 1295,
/* 4238 */ 4242,
/* 4239 */ 50058,
/* 4240 */ -4238,
/* 4241 */ 820,
/* 4242 */ 0,
/* 4243 */ -4238,
/* 4244 */ 1296,
/* 4245 */ 4249,
/* 4246 */ 50058,
/* 4247 */ -4245,
/* 4248 */ 821,
/* 4249 */ 0,
/* 4250 */ -4245,
/* 4251 */ 1297,
/* 4252 */ 4256,
/* 4253 */ 50058,
/* 4254 */ -4252,
/* 4255 */ 822,
/* 4256 */ 0,
/* 4257 */ -4252,
/* 4258 */ 1298,
/* 4259 */ 4265,
/* 4260 */ 223,
/* 4261 */ 4268,
/* 4262 */ 4259,
/* 4263 */ -4259,
/* 4264 */ 825,
/* 4265 */ 0,
/* 4266 */ -4259,
/* 4267 */ 1300,
/* 4268 */ 4272,
/* 4269 */ 50124,
/* 4270 */ -4268,
/* 4271 */ 823,
/* 4272 */ 4276,
/* 4273 */ 50044,
/* 4274 */ -4268,
/* 4275 */ 824,
/* 4276 */ 0,
/* 4277 */ -4268,
/* 4278 */ 1299,
/* 4279 */ 0,
/* 4280 */ 50421,
/* 4281 */ 50257,
/* 4282 */ 50058,
/* 4283 */ 223,
/* 4284 */ 50123,
/* 4285 */ 4289,
/* 4286 */ 50125,
/* 4287 */ -4279,
/* 4288 */ 289,
/* 4289 */ 4294,
/* 4290 */ 4297,
/* 4291 */ 4289,
/* 4292 */ -4289,
/* 4293 */ 826,
/* 4294 */ 0,
/* 4295 */ -4289,
/* 4296 */ 1301,
/* 4297 */ 4304,
/* 4298 */ 50325,
/* 4299 */ 4391,
/* 4300 */ 50257,
/* 4301 */ 50059,
/* 4302 */ -4297,
/* 4303 */ 290,
/* 4304 */ 4311,
/* 4305 */ 50463,
/* 4306 */ 4398,
/* 4307 */ 163,
/* 4308 */ 50059,
/* 4309 */ -4297,
/* 4310 */ 291,
/* 4311 */ 4318,
/* 4312 */ 50350,
/* 4313 */ 4405,
/* 4314 */ 163,
/* 4315 */ 50059,
/* 4316 */ -4297,
/* 4317 */ 292,
/* 4318 */ 4325,
/* 4319 */ 50326,
/* 4320 */ 4412,
/* 4321 */ 163,
/* 4322 */ 50059,
/* 4323 */ -4297,
/* 4324 */ 293,
/* 4325 */ 4332,
/* 4326 */ 50324,
/* 4327 */ 4419,
/* 4328 */ 173,
/* 4329 */ 50059,
/* 4330 */ -4297,
/* 4331 */ 294,
/* 4332 */ 4339,
/* 4333 */ 50430,
/* 4334 */ 4426,
/* 4335 */ 173,
/* 4336 */ 50059,
/* 4337 */ -4297,
/* 4338 */ 295,
/* 4339 */ 4346,
/* 4340 */ 50425,
/* 4341 */ 4433,
/* 4342 */ 4440,
/* 4343 */ 50059,
/* 4344 */ -4297,
/* 4345 */ 296,
/* 4346 */ 4353,
/* 4347 */ 50349,
/* 4348 */ 4460,
/* 4349 */ 4467,
/* 4350 */ 50059,
/* 4351 */ -4297,
/* 4352 */ 297,
/* 4353 */ 4360,
/* 4354 */ 50359,
/* 4355 */ 50058,
/* 4356 */ 50260,
/* 4357 */ 50059,
/* 4358 */ -4297,
/* 4359 */ 298,
/* 4360 */ 4367,
/* 4361 */ 50454,
/* 4362 */ 50058,
/* 4363 */ 4487,
/* 4364 */ 50059,
/* 4365 */ -4297,
/* 4366 */ 299,
/* 4367 */ 4373,
/* 4368 */ 50422,
/* 4369 */ 50058,
/* 4370 */ 4580,
/* 4371 */ -4297,
/* 4372 */ 300,
/* 4373 */ 4379,
/* 4374 */ 50397,
/* 4375 */ 50058,
/* 4376 */ 4588,
/* 4377 */ -4297,
/* 4378 */ 301,
/* 4379 */ 4383,
/* 4380 */ 3423,
/* 4381 */ -4297,
/* 4382 */ 302,
/* 4383 */ 4387,
/* 4384 */ 538,
/* 4385 */ -4297,
/* 4386 */ 303,
/* 4387 */ 0,
/* 4388 */ 6082,
/* 4389 */ -4297,
/* 4390 */ 304,
/* 4391 */ 4395,
/* 4392 */ 50058,
/* 4393 */ -4391,
/* 4394 */ 827,
/* 4395 */ 0,
/* 4396 */ -4391,
/* 4397 */ 1302,
/* 4398 */ 4402,
/* 4399 */ 50058,
/* 4400 */ -4398,
/* 4401 */ 828,
/* 4402 */ 0,
/* 4403 */ -4398,
/* 4404 */ 1303,
/* 4405 */ 4409,
/* 4406 */ 50058,
/* 4407 */ -4405,
/* 4408 */ 829,
/* 4409 */ 0,
/* 4410 */ -4405,
/* 4411 */ 1304,
/* 4412 */ 4416,
/* 4413 */ 50058,
/* 4414 */ -4412,
/* 4415 */ 830,
/* 4416 */ 0,
/* 4417 */ -4412,
/* 4418 */ 1305,
/* 4419 */ 4423,
/* 4420 */ 50058,
/* 4421 */ -4419,
/* 4422 */ 831,
/* 4423 */ 0,
/* 4424 */ -4419,
/* 4425 */ 1306,
/* 4426 */ 4430,
/* 4427 */ 50058,
/* 4428 */ -4426,
/* 4429 */ 832,
/* 4430 */ 0,
/* 4431 */ -4426,
/* 4432 */ 1307,
/* 4433 */ 4437,
/* 4434 */ 50058,
/* 4435 */ -4433,
/* 4436 */ 833,
/* 4437 */ 0,
/* 4438 */ -4433,
/* 4439 */ 1308,
/* 4440 */ 4446,
/* 4441 */ 223,
/* 4442 */ 4449,
/* 4443 */ 4440,
/* 4444 */ -4440,
/* 4445 */ 836,
/* 4446 */ 0,
/* 4447 */ -4440,
/* 4448 */ 1310,
/* 4449 */ 4453,
/* 4450 */ 50044,
/* 4451 */ -4449,
/* 4452 */ 834,
/* 4453 */ 4457,
/* 4454 */ 50124,
/* 4455 */ -4449,
/* 4456 */ 835,
/* 4457 */ 0,
/* 4458 */ -4449,
/* 4459 */ 1309,
/* 4460 */ 4464,
/* 4461 */ 50058,
/* 4462 */ -4460,
/* 4463 */ 837,
/* 4464 */ 0,
/* 4465 */ -4460,
/* 4466 */ 1311,
/* 4467 */ 4473,
/* 4468 */ 223,
/* 4469 */ 4476,
/* 4470 */ 4467,
/* 4471 */ -4467,
/* 4472 */ 840,
/* 4473 */ 0,
/* 4474 */ -4467,
/* 4475 */ 1313,
/* 4476 */ 4480,
/* 4477 */ 50124,
/* 4478 */ -4476,
/* 4479 */ 838,
/* 4480 */ 4484,
/* 4481 */ 50044,
/* 4482 */ -4476,
/* 4483 */ 839,
/* 4484 */ 0,
/* 4485 */ -4476,
/* 4486 */ 1312,
/* 4487 */ 4496,
/* 4488 */ 50259,
/* 4489 */ 50058,
/* 4490 */ 163,
/* 4491 */ 4499,
/* 4492 */ 4573,
/* 4493 */ 4487,
/* 4494 */ -4487,
/* 4495 */ 853,
/* 4496 */ 0,
/* 4497 */ -4487,
/* 4498 */ 1319,
/* 4499 */ 4506,
/* 4500 */ 50091,
/* 4501 */ 173,
/* 4502 */ 50093,
/* 4503 */ 4499,
/* 4504 */ -4499,
/* 4505 */ 841,
/* 4506 */ 4513,
/* 4507 */ 50470,
/* 4508 */ 4522,
/* 4509 */ 4552,
/* 4510 */ 4499,
/* 4511 */ -4499,
/* 4512 */ 850,
/* 4513 */ 4519,
/* 4514 */ 50293,
/* 4515 */ 509,
/* 4516 */ 4499,
/* 4517 */ -4499,
/* 4518 */ 851,
/* 4519 */ 0,
/* 4520 */ -4499,
/* 4521 */ 1317,
/* 4522 */ 4526,
/* 4523 */ 163,
/* 4524 */ -4522,
/* 4525 */ 842,
/* 4526 */ 4532,
/* 4527 */ 50418,
/* 4528 */ 50371,
/* 4529 */ 4538,
/* 4530 */ -4522,
/* 4531 */ 844,
/* 4532 */ 0,
/* 4533 */ 50418,
/* 4534 */ 50387,
/* 4535 */ 4545,
/* 4536 */ -4522,
/* 4537 */ 846,
/* 4538 */ 4542,
/* 4539 */ 173,
/* 4540 */ -4538,
/* 4541 */ 843,
/* 4542 */ 0,
/* 4543 */ -4538,
/* 4544 */ 1314,
/* 4545 */ 4549,
/* 4546 */ 173,
/* 4547 */ -4545,
/* 4548 */ 845,
/* 4549 */ 0,
/* 4550 */ -4545,
/* 4551 */ 1315,
/* 4552 */ 4557,
/* 4553 */ 50398,
/* 4554 */ 163,
/* 4555 */ -4552,
/* 4556 */ 847,
/* 4557 */ 4565,
/* 4558 */ 50398,
/* 4559 */ 50418,
/* 4560 */ 173,
/* 4561 */ 50456,
/* 4562 */ 173,
/* 4563 */ -4552,
/* 4564 */ 848,
/* 4565 */ 4570,
/* 4566 */ 50398,
/* 4567 */ 173,
/* 4568 */ -4552,
/* 4569 */ 849,
/* 4570 */ 0,
/* 4571 */ -4552,
/* 4572 */ 1316,
/* 4573 */ 4577,
/* 4574 */ 50044,
/* 4575 */ -4573,
/* 4576 */ 852,
/* 4577 */ 0,
/* 4578 */ -4573,
/* 4579 */ 1318,
/* 4580 */ 4585,
/* 4581 */ 4297,
/* 4582 */ 4580,
/* 4583 */ -4580,
/* 4584 */ 854,
/* 4585 */ 0,
/* 4586 */ -4580,
/* 4587 */ 1320,
/* 4588 */ 4593,
/* 4589 */ 2693,
/* 4590 */ 4588,
/* 4591 */ -4588,
/* 4592 */ 855,
/* 4593 */ 0,
/* 4594 */ -4588,
/* 4595 */ 1321,
/* 4596 */ 0,
/* 4597 */ 50448,
/* 4598 */ 50257,
/* 4599 */ 50058,
/* 4600 */ 223,
/* 4601 */ 4607,
/* 4602 */ 50123,
/* 4603 */ 4615,
/* 4604 */ 50125,
/* 4605 */ -4596,
/* 4606 */ 305,
/* 4607 */ 4612,
/* 4608 */ 50352,
/* 4609 */ 223,
/* 4610 */ -4607,
/* 4611 */ 856,
/* 4612 */ 0,
/* 4613 */ -4607,
/* 4614 */ 1322,
/* 4615 */ 4620,
/* 4616 */ 4623,
/* 4617 */ 4615,
/* 4618 */ -4615,
/* 4619 */ 857,
/* 4620 */ 0,
/* 4621 */ -4615,
/* 4622 */ 1323,
/* 4623 */ 4630,
/* 4624 */ 50325,
/* 4625 */ 4850,
/* 4626 */ 50257,
/* 4627 */ 50059,
/* 4628 */ -4623,
/* 4629 */ 306,
/* 4630 */ 4637,
/* 4631 */ 50352,
/* 4632 */ 4857,
/* 4633 */ 223,
/* 4634 */ 50059,
/* 4635 */ -4623,
/* 4636 */ 307,
/* 4637 */ 4644,
/* 4638 */ 50389,
/* 4639 */ 4864,
/* 4640 */ 223,
/* 4641 */ 50059,
/* 4642 */ -4623,
/* 4643 */ 308,
/* 4644 */ 4648,
/* 4645 */ 462,
/* 4646 */ -4623,
/* 4647 */ 309,
/* 4648 */ 4655,
/* 4649 */ 4871,
/* 4650 */ 4879,
/* 4651 */ 596,
/* 4652 */ 50059,
/* 4653 */ -4623,
/* 4654 */ 310,
/* 4655 */ 4662,
/* 4656 */ 50363,
/* 4657 */ 4886,
/* 4658 */ 596,
/* 4659 */ 50059,
/* 4660 */ -4623,
/* 4661 */ 311,
/* 4662 */ 4669,
/* 4663 */ 50365,
/* 4664 */ 4893,
/* 4665 */ 596,
/* 4666 */ 50059,
/* 4667 */ -4623,
/* 4668 */ 312,
/* 4669 */ 4676,
/* 4670 */ 50322,
/* 4671 */ 4900,
/* 4672 */ 596,
/* 4673 */ 50059,
/* 4674 */ -4623,
/* 4675 */ 313,
/* 4676 */ 4683,
/* 4677 */ 50291,
/* 4678 */ 4907,
/* 4679 */ 596,
/* 4680 */ 50059,
/* 4681 */ -4623,
/* 4682 */ 314,
/* 4683 */ 4690,
/* 4684 */ 50318,
/* 4685 */ 4914,
/* 4686 */ 173,
/* 4687 */ 50059,
/* 4688 */ -4623,
/* 4689 */ 315,
/* 4690 */ 4697,
/* 4691 */ 50409,
/* 4692 */ 4921,
/* 4693 */ 596,
/* 4694 */ 50059,
/* 4695 */ -4623,
/* 4696 */ 316,
/* 4697 */ 4705,
/* 4698 */ 50305,
/* 4699 */ 50373,
/* 4700 */ 4928,
/* 4701 */ 596,
/* 4702 */ 50059,
/* 4703 */ -4623,
/* 4704 */ 317,
/* 4705 */ 4712,
/* 4706 */ 50430,
/* 4707 */ 4935,
/* 4708 */ 596,
/* 4709 */ 50059,
/* 4710 */ -4623,
/* 4711 */ 318,
/* 4712 */ 4719,
/* 4713 */ 50434,
/* 4714 */ 4942,
/* 4715 */ 596,
/* 4716 */ 50059,
/* 4717 */ -4623,
/* 4718 */ 319,
/* 4719 */ 4726,
/* 4720 */ 50388,
/* 4721 */ 4949,
/* 4722 */ 596,
/* 4723 */ 50059,
/* 4724 */ -4623,
/* 4725 */ 320,
/* 4726 */ 4733,
/* 4727 */ 50276,
/* 4728 */ 4956,
/* 4729 */ 596,
/* 4730 */ 4963,
/* 4731 */ -4623,
/* 4732 */ 321,
/* 4733 */ 4741,
/* 4734 */ 50295,
/* 4735 */ 4971,
/* 4736 */ 5193,
/* 4737 */ 4978,
/* 4738 */ 50059,
/* 4739 */ -4623,
/* 4740 */ 322,
/* 4741 */ 4749,
/* 4742 */ 50439,
/* 4743 */ 50091,
/* 4744 */ 223,
/* 4745 */ 50093,
/* 4746 */ 50059,
/* 4747 */ -4623,
/* 4748 */ 323,
/* 4749 */ 4759,
/* 4750 */ 50439,
/* 4751 */ 50091,
/* 4752 */ 223,
/* 4753 */ 50044,
/* 4754 */ 223,
/* 4755 */ 50093,
/* 4756 */ 50059,
/* 4757 */ -4623,
/* 4758 */ 324,
/* 4759 */ 4771,
/* 4760 */ 50439,
/* 4761 */ 50091,
/* 4762 */ 223,
/* 4763 */ 50044,
/* 4764 */ 223,
/* 4765 */ 50044,
/* 4766 */ 223,
/* 4767 */ 50093,
/* 4768 */ 50059,
/* 4769 */ -4623,
/* 4770 */ 325,
/* 4771 */ 4785,
/* 4772 */ 50439,
/* 4773 */ 50091,
/* 4774 */ 223,
/* 4775 */ 50044,
/* 4776 */ 223,
/* 4777 */ 50044,
/* 4778 */ 223,
/* 4779 */ 50044,
/* 4780 */ 163,
/* 4781 */ 50093,
/* 4782 */ 50059,
/* 4783 */ -4623,
/* 4784 */ 326,
/* 4785 */ 4792,
/* 4786 */ 50423,
/* 4787 */ 4987,
/* 4788 */ 4994,
/* 4789 */ 50059,
/* 4790 */ -4623,
/* 4791 */ 327,
/* 4792 */ 4799,
/* 4793 */ 50368,
/* 4794 */ 5027,
/* 4795 */ 5034,
/* 4796 */ 50059,
/* 4797 */ -4623,
/* 4798 */ 328,
/* 4799 */ 4807,
/* 4800 */ 50345,
/* 4801 */ 5067,
/* 4802 */ 5074,
/* 4803 */ 223,
/* 4804 */ 50059,
/* 4805 */ -4623,
/* 4806 */ 329,
/* 4807 */ 4814,
/* 4808 */ 50436,
/* 4809 */ 5083,
/* 4810 */ 5090,
/* 4811 */ 50059,
/* 4812 */ -4623,
/* 4813 */ 330,
/* 4814 */ 4821,
/* 4815 */ 50329,
/* 4816 */ 5106,
/* 4817 */ 5113,
/* 4818 */ 50059,
/* 4819 */ -4623,
/* 4820 */ 331,
/* 4821 */ 4828,
/* 4822 */ 50408,
/* 4823 */ 5129,
/* 4824 */ 5136,
/* 4825 */ 50059,
/* 4826 */ -4623,
/* 4827 */ 332,
/* 4828 */ 4834,
/* 4829 */ 50349,
/* 4830 */ 5152,
/* 4831 */ 5159,
/* 4832 */ -4623,
/* 4833 */ 333,
/* 4834 */ 4838,
/* 4835 */ 433,
/* 4836 */ -4623,
/* 4837 */ 334,
/* 4838 */ 4842,
/* 4839 */ 538,
/* 4840 */ -4623,
/* 4841 */ 335,
/* 4842 */ 4846,
/* 4843 */ 3423,
/* 4844 */ -4623,
/* 4845 */ 336,
/* 4846 */ 0,
/* 4847 */ 6082,
/* 4848 */ -4623,
/* 4849 */ 337,
/* 4850 */ 4854,
/* 4851 */ 50058,
/* 4852 */ -4850,
/* 4853 */ 858,
/* 4854 */ 0,
/* 4855 */ -4850,
/* 4856 */ 1324,
/* 4857 */ 4861,
/* 4858 */ 50058,
/* 4859 */ -4857,
/* 4860 */ 859,
/* 4861 */ 0,
/* 4862 */ -4857,
/* 4863 */ 1325,
/* 4864 */ 4868,
/* 4865 */ 50058,
/* 4866 */ -4864,
/* 4867 */ 860,
/* 4868 */ 0,
/* 4869 */ -4864,
/* 4870 */ 1326,
/* 4871 */ 4875,
/* 4872 */ 50373,
/* 4873 */ -4871,
/* 4874 */ 861,
/* 4875 */ 0,
/* 4876 */ 50306,
/* 4877 */ -4871,
/* 4878 */ 862,
/* 4879 */ 4883,
/* 4880 */ 50058,
/* 4881 */ -4879,
/* 4882 */ 863,
/* 4883 */ 0,
/* 4884 */ -4879,
/* 4885 */ 1327,
/* 4886 */ 4890,
/* 4887 */ 50058,
/* 4888 */ -4886,
/* 4889 */ 864,
/* 4890 */ 0,
/* 4891 */ -4886,
/* 4892 */ 1328,
/* 4893 */ 4897,
/* 4894 */ 50058,
/* 4895 */ -4893,
/* 4896 */ 865,
/* 4897 */ 0,
/* 4898 */ -4893,
/* 4899 */ 1329,
/* 4900 */ 4904,
/* 4901 */ 50058,
/* 4902 */ -4900,
/* 4903 */ 866,
/* 4904 */ 0,
/* 4905 */ -4900,
/* 4906 */ 1330,
/* 4907 */ 4911,
/* 4908 */ 50058,
/* 4909 */ -4907,
/* 4910 */ 867,
/* 4911 */ 0,
/* 4912 */ -4907,
/* 4913 */ 1331,
/* 4914 */ 4918,
/* 4915 */ 50058,
/* 4916 */ -4914,
/* 4917 */ 868,
/* 4918 */ 0,
/* 4919 */ -4914,
/* 4920 */ 1332,
/* 4921 */ 4925,
/* 4922 */ 50058,
/* 4923 */ -4921,
/* 4924 */ 869,
/* 4925 */ 0,
/* 4926 */ -4921,
/* 4927 */ 1333,
/* 4928 */ 4932,
/* 4929 */ 50058,
/* 4930 */ -4928,
/* 4931 */ 870,
/* 4932 */ 0,
/* 4933 */ -4928,
/* 4934 */ 1334,
/* 4935 */ 4939,
/* 4936 */ 50058,
/* 4937 */ -4935,
/* 4938 */ 871,
/* 4939 */ 0,
/* 4940 */ -4935,
/* 4941 */ 1335,
/* 4942 */ 4946,
/* 4943 */ 50058,
/* 4944 */ -4942,
/* 4945 */ 872,
/* 4946 */ 0,
/* 4947 */ -4942,
/* 4948 */ 1336,
/* 4949 */ 4953,
/* 4950 */ 50058,
/* 4951 */ -4949,
/* 4952 */ 873,
/* 4953 */ 0,
/* 4954 */ -4949,
/* 4955 */ 1337,
/* 4956 */ 4960,
/* 4957 */ 50058,
/* 4958 */ -4956,
/* 4959 */ 874,
/* 4960 */ 0,
/* 4961 */ -4956,
/* 4962 */ 1338,
/* 4963 */ 4967,
/* 4964 */ 50044,
/* 4965 */ -4963,
/* 4966 */ 875,
/* 4967 */ 0,
/* 4968 */ 50059,
/* 4969 */ -4963,
/* 4970 */ 876,
/* 4971 */ 4975,
/* 4972 */ 50058,
/* 4973 */ -4971,
/* 4974 */ 877,
/* 4975 */ 0,
/* 4976 */ -4971,
/* 4977 */ 1339,
/* 4978 */ 4984,
/* 4979 */ 50044,
/* 4980 */ 5193,
/* 4981 */ 4978,
/* 4982 */ -4978,
/* 4983 */ 878,
/* 4984 */ 0,
/* 4985 */ -4978,
/* 4986 */ 1340,
/* 4987 */ 4991,
/* 4988 */ 50058,
/* 4989 */ -4987,
/* 4990 */ 879,
/* 4991 */ 0,
/* 4992 */ -4987,
/* 4993 */ 1341,
/* 4994 */ 5000,
/* 4995 */ 5003,
/* 4996 */ 5020,
/* 4997 */ 4994,
/* 4998 */ -4994,
/* 4999 */ 885,
/* 5000 */ 0,
/* 5001 */ -4994,
/* 5002 */ 1343,
/* 5003 */ 5007,
/* 5004 */ 223,
/* 5005 */ -5003,
/* 5006 */ 880,
/* 5007 */ 0,
/* 5008 */ 5012,
/* 5009 */ 223,
/* 5010 */ -5003,
/* 5011 */ 883,
/* 5012 */ 5016,
/* 5013 */ 50126,
/* 5014 */ -5012,
/* 5015 */ 881,
/* 5016 */ 0,
/* 5017 */ 50033,
/* 5018 */ -5012,
/* 5019 */ 882,
/* 5020 */ 5024,
/* 5021 */ 50044,
/* 5022 */ -5020,
/* 5023 */ 884,
/* 5024 */ 0,
/* 5025 */ -5020,
/* 5026 */ 1342,
/* 5027 */ 5031,
/* 5028 */ 50058,
/* 5029 */ -5027,
/* 5030 */ 886,
/* 5031 */ 0,
/* 5032 */ -5027,
/* 5033 */ 1344,
/* 5034 */ 5040,
/* 5035 */ 5043,
/* 5036 */ 5060,
/* 5037 */ 5034,
/* 5038 */ -5034,
/* 5039 */ 892,
/* 5040 */ 0,
/* 5041 */ -5034,
/* 5042 */ 1346,
/* 5043 */ 5047,
/* 5044 */ 223,
/* 5045 */ -5043,
/* 5046 */ 887,
/* 5047 */ 0,
/* 5048 */ 5052,
/* 5049 */ 223,
/* 5050 */ -5043,
/* 5051 */ 890,
/* 5052 */ 5056,
/* 5053 */ 50126,
/* 5054 */ -5052,
/* 5055 */ 888,
/* 5056 */ 0,
/* 5057 */ 50033,
/* 5058 */ -5052,
/* 5059 */ 889,
/* 5060 */ 5064,
/* 5061 */ 50044,
/* 5062 */ -5060,
/* 5063 */ 891,
/* 5064 */ 0,
/* 5065 */ -5060,
/* 5066 */ 1345,
/* 5067 */ 5071,
/* 5068 */ 50058,
/* 5069 */ -5067,
/* 5070 */ 893,
/* 5071 */ 0,
/* 5072 */ -5067,
/* 5073 */ 1347,
/* 5074 */ 5080,
/* 5075 */ 223,
/* 5076 */ 50044,
/* 5077 */ 5074,
/* 5078 */ -5074,
/* 5079 */ 894,
/* 5080 */ 0,
/* 5081 */ -5074,
/* 5082 */ 1348,
/* 5083 */ 5087,
/* 5084 */ 50058,
/* 5085 */ -5083,
/* 5086 */ 895,
/* 5087 */ 0,
/* 5088 */ -5083,
/* 5089 */ 1349,
/* 5090 */ 5096,
/* 5091 */ 163,
/* 5092 */ 5099,
/* 5093 */ 5090,
/* 5094 */ -5090,
/* 5095 */ 897,
/* 5096 */ 0,
/* 5097 */ -5090,
/* 5098 */ 1351,
/* 5099 */ 5103,
/* 5100 */ 50044,
/* 5101 */ -5099,
/* 5102 */ 896,
/* 5103 */ 0,
/* 5104 */ -5099,
/* 5105 */ 1350,
/* 5106 */ 5110,
/* 5107 */ 50058,
/* 5108 */ -5106,
/* 5109 */ 898,
/* 5110 */ 0,
/* 5111 */ -5106,
/* 5112 */ 1352,
/* 5113 */ 5119,
/* 5114 */ 163,
/* 5115 */ 5122,
/* 5116 */ 5113,
/* 5117 */ -5113,
/* 5118 */ 900,
/* 5119 */ 0,
/* 5120 */ -5113,
/* 5121 */ 1354,
/* 5122 */ 5126,
/* 5123 */ 50044,
/* 5124 */ -5122,
/* 5125 */ 899,
/* 5126 */ 0,
/* 5127 */ -5122,
/* 5128 */ 1353,
/* 5129 */ 5133,
/* 5130 */ 50058,
/* 5131 */ -5129,
/* 5132 */ 901,
/* 5133 */ 0,
/* 5134 */ -5129,
/* 5135 */ 1355,
/* 5136 */ 5142,
/* 5137 */ 163,
/* 5138 */ 5145,
/* 5139 */ 5136,
/* 5140 */ -5136,
/* 5141 */ 903,
/* 5142 */ 0,
/* 5143 */ -5136,
/* 5144 */ 1357,
/* 5145 */ 5149,
/* 5146 */ 50044,
/* 5147 */ -5145,
/* 5148 */ 902,
/* 5149 */ 0,
/* 5150 */ -5145,
/* 5151 */ 1356,
/* 5152 */ 5156,
/* 5153 */ 50058,
/* 5154 */ -5152,
/* 5155 */ 904,
/* 5156 */ 0,
/* 5157 */ -5152,
/* 5158 */ 1358,
/* 5159 */ 5165,
/* 5160 */ 5168,
/* 5161 */ 5185,
/* 5162 */ 5159,
/* 5163 */ -5159,
/* 5164 */ 911,
/* 5165 */ 0,
/* 5166 */ -5159,
/* 5167 */ 1359,
/* 5168 */ 5172,
/* 5169 */ 223,
/* 5170 */ -5168,
/* 5171 */ 905,
/* 5172 */ 0,
/* 5173 */ 5177,
/* 5174 */ 223,
/* 5175 */ -5168,
/* 5176 */ 908,
/* 5177 */ 5181,
/* 5178 */ 50126,
/* 5179 */ -5177,
/* 5180 */ 906,
/* 5181 */ 0,
/* 5182 */ 50033,
/* 5183 */ -5177,
/* 5184 */ 907,
/* 5185 */ 5189,
/* 5186 */ 50044,
/* 5187 */ -5185,
/* 5188 */ 909,
/* 5189 */ 0,
/* 5190 */ 50059,
/* 5191 */ -5185,
/* 5192 */ 910,
/* 5193 */ 5203,
/* 5194 */ 223,
/* 5195 */ 50352,
/* 5196 */ 5210,
/* 5197 */ 5222,
/* 5198 */ 5229,
/* 5199 */ 5236,
/* 5200 */ 5258,
/* 5201 */ -5193,
/* 5202 */ 338,
/* 5203 */ 0,
/* 5204 */ 223,
/* 5205 */ 50352,
/* 5206 */ 163,
/* 5207 */ 5313,
/* 5208 */ -5193,
/* 5209 */ 339,
/* 5210 */ 5214,
/* 5211 */ 286,
/* 5212 */ -5210,
/* 5213 */ 912,
/* 5214 */ 5219,
/* 5215 */ 223,
/* 5216 */ 50037,
/* 5217 */ -5210,
/* 5218 */ 913,
/* 5219 */ 0,
/* 5220 */ -5210,
/* 5221 */ 1360,
/* 5222 */ 5226,
/* 5223 */ 223,
/* 5224 */ -5222,
/* 5225 */ 914,
/* 5226 */ 0,
/* 5227 */ -5222,
/* 5228 */ 1361,
/* 5229 */ 5233,
/* 5230 */ 50318,
/* 5231 */ -5229,
/* 5232 */ 915,
/* 5233 */ 0,
/* 5234 */ -5229,
/* 5235 */ 1362,
/* 5236 */ 5243,
/* 5237 */ 50040,
/* 5238 */ 5246,
/* 5239 */ 223,
/* 5240 */ 50041,
/* 5241 */ -5236,
/* 5242 */ 919,
/* 5243 */ 0,
/* 5244 */ -5236,
/* 5245 */ 1363,
/* 5246 */ 5250,
/* 5247 */ 50321,
/* 5248 */ -5246,
/* 5249 */ 916,
/* 5250 */ 5254,
/* 5251 */ 50306,
/* 5252 */ -5246,
/* 5253 */ 917,
/* 5254 */ 0,
/* 5255 */ 50373,
/* 5256 */ -5246,
/* 5257 */ 918,
/* 5258 */ 5267,
/* 5259 */ 50287,
/* 5260 */ 5270,
/* 5261 */ 5277,
/* 5262 */ 5284,
/* 5263 */ 5291,
/* 5264 */ 5258,
/* 5265 */ -5258,
/* 5266 */ 927,
/* 5267 */ 0,
/* 5268 */ -5258,
/* 5269 */ 1368,
/* 5270 */ 5274,
/* 5271 */ 286,
/* 5272 */ -5270,
/* 5273 */ 920,
/* 5274 */ 0,
/* 5275 */ -5270,
/* 5276 */ 1364,
/* 5277 */ 5281,
/* 5278 */ 223,
/* 5279 */ -5277,
/* 5280 */ 921,
/* 5281 */ 0,
/* 5282 */ -5277,
/* 5283 */ 1365,
/* 5284 */ 5288,
/* 5285 */ 50318,
/* 5286 */ -5284,
/* 5287 */ 922,
/* 5288 */ 0,
/* 5289 */ -5284,
/* 5290 */ 1366,
/* 5291 */ 5298,
/* 5292 */ 50040,
/* 5293 */ 5301,
/* 5294 */ 223,
/* 5295 */ 50041,
/* 5296 */ -5291,
/* 5297 */ 926,
/* 5298 */ 0,
/* 5299 */ -5291,
/* 5300 */ 1367,
/* 5301 */ 5305,
/* 5302 */ 50321,
/* 5303 */ -5301,
/* 5304 */ 923,
/* 5305 */ 5309,
/* 5306 */ 50306,
/* 5307 */ -5301,
/* 5308 */ 924,
/* 5309 */ 0,
/* 5310 */ 50373,
/* 5311 */ -5301,
/* 5312 */ 925,
/* 5313 */ 5317,
/* 5314 */ 223,
/* 5315 */ -5313,
/* 5316 */ 928,
/* 5317 */ 0,
/* 5318 */ -5313,
/* 5319 */ 1369,
/* 5320 */ 0,
/* 5321 */ 50261,
/* 5322 */ -5320,
/* 5323 */ 340,
/* 5324 */ 0,
/* 5325 */ 50340,
/* 5326 */ 50257,
/* 5327 */ 50058,
/* 5328 */ 223,
/* 5329 */ 50123,
/* 5330 */ 5334,
/* 5331 */ 50125,
/* 5332 */ -5324,
/* 5333 */ 341,
/* 5334 */ 5339,
/* 5335 */ 5342,
/* 5336 */ 5334,
/* 5337 */ -5334,
/* 5338 */ 929,
/* 5339 */ 0,
/* 5340 */ -5334,
/* 5341 */ 1370,
/* 5342 */ 5349,
/* 5343 */ 50325,
/* 5344 */ 5371,
/* 5345 */ 50257,
/* 5346 */ 50059,
/* 5347 */ -5342,
/* 5348 */ 342,
/* 5349 */ 5356,
/* 5350 */ 50382,
/* 5351 */ 5378,
/* 5352 */ 223,
/* 5353 */ 50059,
/* 5354 */ -5342,
/* 5355 */ 343,
/* 5356 */ 5363,
/* 5357 */ 50309,
/* 5358 */ 5385,
/* 5359 */ 1247,
/* 5360 */ 50059,
/* 5361 */ -5342,
/* 5362 */ 344,
/* 5363 */ 0,
/* 5364 */ 50465,
/* 5365 */ 5392,
/* 5366 */ 223,
/* 5367 */ 50037,
/* 5368 */ 50059,
/* 5369 */ -5342,
/* 5370 */ 345,
/* 5371 */ 5375,
/* 5372 */ 50058,
/* 5373 */ -5371,
/* 5374 */ 930,
/* 5375 */ 0,
/* 5376 */ -5371,
/* 5377 */ 1371,
/* 5378 */ 5382,
/* 5379 */ 50058,
/* 5380 */ -5378,
/* 5381 */ 931,
/* 5382 */ 0,
/* 5383 */ -5378,
/* 5384 */ 1372,
/* 5385 */ 5389,
/* 5386 */ 50058,
/* 5387 */ -5385,
/* 5388 */ 932,
/* 5389 */ 0,
/* 5390 */ -5385,
/* 5391 */ 1373,
/* 5392 */ 5396,
/* 5393 */ 50058,
/* 5394 */ -5392,
/* 5395 */ 933,
/* 5396 */ 0,
/* 5397 */ -5392,
/* 5398 */ 1374,
/* 5399 */ 0,
/* 5400 */ 50335,
/* 5401 */ 50257,
/* 5402 */ 5408,
/* 5403 */ 50123,
/* 5404 */ 5416,
/* 5405 */ 50125,
/* 5406 */ -5399,
/* 5407 */ 346,
/* 5408 */ 5413,
/* 5409 */ 50058,
/* 5410 */ 223,
/* 5411 */ -5408,
/* 5412 */ 934,
/* 5413 */ 0,
/* 5414 */ -5408,
/* 5415 */ 1375,
/* 5416 */ 5421,
/* 5417 */ 5424,
/* 5418 */ 5416,
/* 5419 */ -5416,
/* 5420 */ 935,
/* 5421 */ 0,
/* 5422 */ -5416,
/* 5423 */ 1376,
/* 5424 */ 5431,
/* 5425 */ 50449,
/* 5426 */ 50058,
/* 5427 */ 173,
/* 5428 */ 50059,
/* 5429 */ -5424,
/* 5430 */ 347,
/* 5431 */ 5438,
/* 5432 */ 50324,
/* 5433 */ 50058,
/* 5434 */ 223,
/* 5435 */ 50059,
/* 5436 */ -5424,
/* 5437 */ 348,
/* 5438 */ 5445,
/* 5439 */ 50465,
/* 5440 */ 50058,
/* 5441 */ 223,
/* 5442 */ 50059,
/* 5443 */ -5424,
/* 5444 */ 349,
/* 5445 */ 5452,
/* 5446 */ 50306,
/* 5447 */ 50058,
/* 5448 */ 223,
/* 5449 */ 50059,
/* 5450 */ -5424,
/* 5451 */ 350,
/* 5452 */ 5460,
/* 5453 */ 50306,
/* 5454 */ 50058,
/* 5455 */ 223,
/* 5456 */ 50043,
/* 5457 */ 50059,
/* 5458 */ -5424,
/* 5459 */ 351,
/* 5460 */ 5469,
/* 5461 */ 50306,
/* 5462 */ 50058,
/* 5463 */ 223,
/* 5464 */ 5506,
/* 5465 */ 223,
/* 5466 */ 50059,
/* 5467 */ -5424,
/* 5468 */ 352,
/* 5469 */ 5476,
/* 5470 */ 50284,
/* 5471 */ 50058,
/* 5472 */ 223,
/* 5473 */ 50059,
/* 5474 */ -5424,
/* 5475 */ 353,
/* 5476 */ 5483,
/* 5477 */ 50405,
/* 5478 */ 50058,
/* 5479 */ 5514,
/* 5480 */ 50059,
/* 5481 */ -5424,
/* 5482 */ 354,
/* 5483 */ 5490,
/* 5484 */ 50349,
/* 5485 */ 5522,
/* 5486 */ 5529,
/* 5487 */ 50059,
/* 5488 */ -5424,
/* 5489 */ 355,
/* 5490 */ 5494,
/* 5491 */ 433,
/* 5492 */ -5424,
/* 5493 */ 356,
/* 5494 */ 5498,
/* 5495 */ 538,
/* 5496 */ -5424,
/* 5497 */ 357,
/* 5498 */ 5502,
/* 5499 */ 3423,
/* 5500 */ -5424,
/* 5501 */ 358,
/* 5502 */ 0,
/* 5503 */ 6082,
/* 5504 */ -5424,
/* 5505 */ 359,
/* 5506 */ 5510,
/* 5507 */ 50456,
/* 5508 */ -5506,
/* 5509 */ 936,
/* 5510 */ 0,
/* 5511 */ 50045,
/* 5512 */ -5506,
/* 5513 */ 937,
/* 5514 */ 5519,
/* 5515 */ 5549,
/* 5516 */ 5514,
/* 5517 */ -5514,
/* 5518 */ 938,
/* 5519 */ 0,
/* 5520 */ -5514,
/* 5521 */ 1377,
/* 5522 */ 5526,
/* 5523 */ 50058,
/* 5524 */ -5522,
/* 5525 */ 939,
/* 5526 */ 0,
/* 5527 */ -5522,
/* 5528 */ 1378,
/* 5529 */ 5535,
/* 5530 */ 223,
/* 5531 */ 5538,
/* 5532 */ 5529,
/* 5533 */ -5529,
/* 5534 */ 942,
/* 5535 */ 0,
/* 5536 */ -5529,
/* 5537 */ 1380,
/* 5538 */ 5542,
/* 5539 */ 50044,
/* 5540 */ -5538,
/* 5541 */ 940,
/* 5542 */ 5546,
/* 5543 */ 50124,
/* 5544 */ -5538,
/* 5545 */ 941,
/* 5546 */ 0,
/* 5547 */ -5538,
/* 5548 */ 1379,
/* 5549 */ 0,
/* 5550 */ 5558,
/* 5551 */ 5570,
/* 5552 */ 50398,
/* 5553 */ 5633,
/* 5554 */ 5646,
/* 5555 */ 5662,
/* 5556 */ -5549,
/* 5557 */ 360,
/* 5558 */ 5562,
/* 5559 */ 50042,
/* 5560 */ -5558,
/* 5561 */ 943,
/* 5562 */ 5566,
/* 5563 */ 50334,
/* 5564 */ -5558,
/* 5565 */ 944,
/* 5566 */ 0,
/* 5567 */ 50124,
/* 5568 */ -5558,
/* 5569 */ 945,
/* 5570 */ 5576,
/* 5571 */ 237,
/* 5572 */ 50037,
/* 5573 */ 5570,
/* 5574 */ -5570,
/* 5575 */ 946,
/* 5576 */ 5585,
/* 5577 */ 50060,
/* 5578 */ 50306,
/* 5579 */ 237,
/* 5580 */ 50043,
/* 5581 */ 50062,
/* 5582 */ 5570,
/* 5583 */ -5570,
/* 5584 */ 947,
/* 5585 */ 5592,
/* 5586 */ 50091,
/* 5587 */ 237,
/* 5588 */ 50093,
/* 5589 */ 5570,
/* 5590 */ -5570,
/* 5591 */ 948,
/* 5592 */ 5599,
/* 5593 */ 50123,
/* 5594 */ 588,
/* 5595 */ 50125,
/* 5596 */ 5570,
/* 5597 */ -5570,
/* 5598 */ 949,
/* 5599 */ 5606,
/* 5600 */ 237,
/* 5601 */ 5625,
/* 5602 */ 237,
/* 5603 */ 5570,
/* 5604 */ -5570,
/* 5605 */ 952,
/* 5606 */ 5611,
/* 5607 */ 237,
/* 5608 */ 5570,
/* 5609 */ -5570,
/* 5610 */ 953,
/* 5611 */ 5617,
/* 5612 */ 237,
/* 5613 */ 50043,
/* 5614 */ 5570,
/* 5615 */ -5570,
/* 5616 */ 954,
/* 5617 */ 5622,
/* 5618 */ 286,
/* 5619 */ 5570,
/* 5620 */ -5570,
/* 5621 */ 955,
/* 5622 */ 0,
/* 5623 */ -5570,
/* 5624 */ 1381,
/* 5625 */ 5629,
/* 5626 */ 50045,
/* 5627 */ -5625,
/* 5628 */ 950,
/* 5629 */ 0,
/* 5630 */ 50456,
/* 5631 */ -5625,
/* 5632 */ 951,
/* 5633 */ 5637,
/* 5634 */ 163,
/* 5635 */ -5633,
/* 5636 */ 956,
/* 5637 */ 5641,
/* 5638 */ 223,
/* 5639 */ -5633,
/* 5640 */ 957,
/* 5641 */ 0,
/* 5642 */ 50288,
/* 5643 */ 223,
/* 5644 */ -5633,
/* 5645 */ 958,
/* 5646 */ 5650,
/* 5647 */ 163,
/* 5648 */ -5646,
/* 5649 */ 959,
/* 5650 */ 5654,
/* 5651 */ 223,
/* 5652 */ -5646,
/* 5653 */ 960,
/* 5654 */ 5659,
/* 5655 */ 50288,
/* 5656 */ 223,
/* 5657 */ -5646,
/* 5658 */ 961,
/* 5659 */ 0,
/* 5660 */ -5646,
/* 5661 */ 1382,
/* 5662 */ 5669,
/* 5663 */ 50091,
/* 5664 */ 223,
/* 5665 */ 5672,
/* 5666 */ 50093,
/* 5667 */ -5662,
/* 5668 */ 965,
/* 5669 */ 0,
/* 5670 */ -5662,
/* 5671 */ 1384,
/* 5672 */ 5678,
/* 5673 */ 5681,
/* 5674 */ 223,
/* 5675 */ 5672,
/* 5676 */ -5672,
/* 5677 */ 964,
/* 5678 */ 0,
/* 5679 */ -5672,
/* 5680 */ 1383,
/* 5681 */ 5685,
/* 5682 */ 50124,
/* 5683 */ -5681,
/* 5684 */ 962,
/* 5685 */ 0,
/* 5686 */ 50044,
/* 5687 */ -5681,
/* 5688 */ 963,
/* 5689 */ 0,
/* 5690 */ 50298,
/* 5691 */ 50257,
/* 5692 */ 50058,
/* 5693 */ 223,
/* 5694 */ 50123,
/* 5695 */ 5699,
/* 5696 */ 50125,
/* 5697 */ -5689,
/* 5698 */ 361,
/* 5699 */ 5704,
/* 5700 */ 5707,
/* 5701 */ 5699,
/* 5702 */ -5699,
/* 5703 */ 966,
/* 5704 */ 0,
/* 5705 */ -5699,
/* 5706 */ 1385,
/* 5707 */ 5714,
/* 5708 */ 50435,
/* 5709 */ 5733,
/* 5710 */ 163,
/* 5711 */ 50059,
/* 5712 */ -5707,
/* 5713 */ 362,
/* 5714 */ 5721,
/* 5715 */ 50349,
/* 5716 */ 5740,
/* 5717 */ 5747,
/* 5718 */ 50059,
/* 5719 */ -5707,
/* 5720 */ 363,
/* 5721 */ 5725,
/* 5722 */ 538,
/* 5723 */ -5707,
/* 5724 */ 364,
/* 5725 */ 5729,
/* 5726 */ 3423,
/* 5727 */ -5707,
/* 5728 */ 365,
/* 5729 */ 0,
/* 5730 */ 6082,
/* 5731 */ -5707,
/* 5732 */ 366,
/* 5733 */ 5737,
/* 5734 */ 50058,
/* 5735 */ -5733,
/* 5736 */ 967,
/* 5737 */ 0,
/* 5738 */ -5733,
/* 5739 */ 1386,
/* 5740 */ 5744,
/* 5741 */ 50058,
/* 5742 */ -5740,
/* 5743 */ 968,
/* 5744 */ 0,
/* 5745 */ -5740,
/* 5746 */ 1387,
/* 5747 */ 5753,
/* 5748 */ 223,
/* 5749 */ 5756,
/* 5750 */ 5747,
/* 5751 */ -5747,
/* 5752 */ 971,
/* 5753 */ 0,
/* 5754 */ -5747,
/* 5755 */ 1389,
/* 5756 */ 5760,
/* 5757 */ 50124,
/* 5758 */ -5756,
/* 5759 */ 969,
/* 5760 */ 5764,
/* 5761 */ 50044,
/* 5762 */ -5756,
/* 5763 */ 970,
/* 5764 */ 0,
/* 5765 */ -5756,
/* 5766 */ 1388,
/* 5767 */ 0,
/* 5768 */ 50450,
/* 5769 */ 50257,
/* 5770 */ 5774,
/* 5771 */ 50059,
/* 5772 */ -5767,
/* 5773 */ 367,
/* 5774 */ 5779,
/* 5775 */ 50257,
/* 5776 */ 5774,
/* 5777 */ -5774,
/* 5778 */ 972,
/* 5779 */ 5784,
/* 5780 */ 163,
/* 5781 */ 5774,
/* 5782 */ -5774,
/* 5783 */ 973,
/* 5784 */ 5789,
/* 5785 */ 50040,
/* 5786 */ 5774,
/* 5787 */ -5774,
/* 5788 */ 974,
/* 5789 */ 5794,
/* 5790 */ 50041,
/* 5791 */ 5774,
/* 5792 */ -5774,
/* 5793 */ 975,
/* 5794 */ 5799,
/* 5795 */ 50124,
/* 5796 */ 5774,
/* 5797 */ -5774,
/* 5798 */ 976,
/* 5799 */ 5804,
/* 5800 */ 50059,
/* 5801 */ 5774,
/* 5802 */ -5774,
/* 5803 */ 977,
/* 5804 */ 5812,
/* 5805 */ 50091,
/* 5806 */ 588,
/* 5807 */ 50093,
/* 5808 */ 50058,
/* 5809 */ 5774,
/* 5810 */ -5774,
/* 5811 */ 978,
/* 5812 */ 5819,
/* 5813 */ 50091,
/* 5814 */ 592,
/* 5815 */ 50093,
/* 5816 */ 5774,
/* 5817 */ -5774,
/* 5818 */ 979,
/* 5819 */ 0,
/* 5820 */ -5774,
/* 5821 */ 1390,
/* 5822 */ 0,
/* 5823 */ 50261,
/* 5824 */ -5822,
/* 5825 */ 368,
/* 5826 */ 5830,
/* 5827 */ 50264,
/* 5828 */ -5826,
/* 5829 */ 369,
/* 5830 */ 5834,
/* 5831 */ 50267,
/* 5832 */ -5826,
/* 5833 */ 370,
/* 5834 */ 5838,
/* 5835 */ 50263,
/* 5836 */ -5826,
/* 5837 */ 371,
/* 5838 */ 5842,
/* 5839 */ 50271,
/* 5840 */ -5826,
/* 5841 */ 372,
/* 5842 */ 5846,
/* 5843 */ 50272,
/* 5844 */ -5826,
/* 5845 */ 373,
/* 5846 */ 5850,
/* 5847 */ 50273,
/* 5848 */ -5826,
/* 5849 */ 374,
/* 5850 */ 0,
/* 5851 */ 50473,
/* 5852 */ -5826,
/* 5853 */ 375,
/* 5854 */ 0,
/* 5855 */ 50446,
/* 5856 */ 5826,
/* 5857 */ 223,
/* 5858 */ 50262,
/* 5859 */ 50258,
/* 5860 */ 50040,
/* 5861 */ 5867,
/* 5862 */ 5936,
/* 5863 */ 50041,
/* 5864 */ 50059,
/* 5865 */ -5854,
/* 5866 */ 376,
/* 5867 */ 5875,
/* 5868 */ 5826,
/* 5869 */ 5878,
/* 5870 */ 5886,
/* 5871 */ 5893,
/* 5872 */ 5901,
/* 5873 */ -5867,
/* 5874 */ 987,
/* 5875 */ 0,
/* 5876 */ -5867,
/* 5877 */ 1398,
/* 5878 */ 5883,
/* 5879 */ 50058,
/* 5880 */ 223,
/* 5881 */ -5878,
/* 5882 */ 980,
/* 5883 */ 0,
/* 5884 */ -5878,
/* 5885 */ 1391,
/* 5886 */ 5890,
/* 5887 */ 50258,
/* 5888 */ -5886,
/* 5889 */ 981,
/* 5890 */ 0,
/* 5891 */ -5886,
/* 5892 */ 1392,
/* 5893 */ 5898,
/* 5894 */ 50061,
/* 5895 */ 173,
/* 5896 */ -5893,
/* 5897 */ 982,
/* 5898 */ 0,
/* 5899 */ -5893,
/* 5900 */ 1393,
/* 5901 */ 5910,
/* 5902 */ 50044,
/* 5903 */ 5826,
/* 5904 */ 5913,
/* 5905 */ 5921,
/* 5906 */ 5928,
/* 5907 */ 5901,
/* 5908 */ -5901,
/* 5909 */ 986,
/* 5910 */ 0,
/* 5911 */ -5901,
/* 5912 */ 1397,
/* 5913 */ 5918,
/* 5914 */ 50058,
/* 5915 */ 223,
/* 5916 */ -5913,
/* 5917 */ 983,
/* 5918 */ 0,
/* 5919 */ -5913,
/* 5920 */ 1394,
/* 5921 */ 5925,
/* 5922 */ 50258,
/* 5923 */ -5921,
/* 5924 */ 984,
/* 5925 */ 0,
/* 5926 */ -5921,
/* 5927 */ 1395,
/* 5928 */ 5933,
/* 5929 */ 50061,
/* 5930 */ 173,
/* 5931 */ -5928,
/* 5932 */ 985,
/* 5933 */ 0,
/* 5934 */ -5928,
/* 5935 */ 1396,
/* 5936 */ 5941,
/* 5937 */ 50044,
/* 5938 */ 50494,
/* 5939 */ -5936,
/* 5940 */ 988,
/* 5941 */ 0,
/* 5942 */ -5936,
/* 5943 */ 1399,
/* 5944 */ 0,
/* 5945 */ 50446,
/* 5946 */ 5826,
/* 5947 */ 50258,
/* 5948 */ 50040,
/* 5949 */ 5955,
/* 5950 */ 6024,
/* 5951 */ 50041,
/* 5952 */ 50059,
/* 5953 */ -5944,
/* 5954 */ 377,
/* 5955 */ 5963,
/* 5956 */ 5826,
/* 5957 */ 5966,
/* 5958 */ 5974,
/* 5959 */ 5981,
/* 5960 */ 5989,
/* 5961 */ -5955,
/* 5962 */ 996,
/* 5963 */ 0,
/* 5964 */ -5955,
/* 5965 */ 1407,
/* 5966 */ 5971,
/* 5967 */ 50058,
/* 5968 */ 223,
/* 5969 */ -5966,
/* 5970 */ 989,
/* 5971 */ 0,
/* 5972 */ -5966,
/* 5973 */ 1400,
/* 5974 */ 5978,
/* 5975 */ 50258,
/* 5976 */ -5974,
/* 5977 */ 990,
/* 5978 */ 0,
/* 5979 */ -5974,
/* 5980 */ 1401,
/* 5981 */ 5986,
/* 5982 */ 50061,
/* 5983 */ 173,
/* 5984 */ -5981,
/* 5985 */ 991,
/* 5986 */ 0,
/* 5987 */ -5981,
/* 5988 */ 1402,
/* 5989 */ 5998,
/* 5990 */ 50044,
/* 5991 */ 5826,
/* 5992 */ 6001,
/* 5993 */ 6009,
/* 5994 */ 6016,
/* 5995 */ 5989,
/* 5996 */ -5989,
/* 5997 */ 995,
/* 5998 */ 0,
/* 5999 */ -5989,
/* 6000 */ 1406,
/* 6001 */ 6006,
/* 6002 */ 50058,
/* 6003 */ 223,
/* 6004 */ -6001,
/* 6005 */ 992,
/* 6006 */ 0,
/* 6007 */ -6001,
/* 6008 */ 1403,
/* 6009 */ 6013,
/* 6010 */ 50258,
/* 6011 */ -6009,
/* 6012 */ 993,
/* 6013 */ 0,
/* 6014 */ -6009,
/* 6015 */ 1404,
/* 6016 */ 6021,
/* 6017 */ 50061,
/* 6018 */ 173,
/* 6019 */ -6016,
/* 6020 */ 994,
/* 6021 */ 0,
/* 6022 */ -6016,
/* 6023 */ 1405,
/* 6024 */ 6029,
/* 6025 */ 50044,
/* 6026 */ 50494,
/* 6027 */ -6024,
/* 6028 */ 997,
/* 6029 */ 0,
/* 6030 */ -6024,
/* 6031 */ 1408,
/* 6032 */ 0,
/* 6033 */ 50446,
/* 6034 */ 5826,
/* 6035 */ 223,
/* 6036 */ 50262,
/* 6037 */ 50258,
/* 6038 */ 50059,
/* 6039 */ -6032,
/* 6040 */ 378,
/* 6041 */ 0,
/* 6042 */ 50446,
/* 6043 */ 223,
/* 6044 */ 50258,
/* 6045 */ 50480,
/* 6046 */ 223,
/* 6047 */ 50059,
/* 6048 */ -6041,
/* 6049 */ 379,
/* 6050 */ 0,
/* 6051 */ 5826,
/* 6052 */ 50258,
/* 6053 */ 6057,
/* 6054 */ 50059,
/* 6055 */ -6050,
/* 6056 */ 380,
/* 6057 */ 6063,
/* 6058 */ 50044,
/* 6059 */ 50258,
/* 6060 */ 6057,
/* 6061 */ -6057,
/* 6062 */ 998,
/* 6063 */ 0,
/* 6064 */ -6057,
/* 6065 */ 1409,
/* 6066 */ 0,
/* 6067 */ 5826,
/* 6068 */ 50258,
/* 6069 */ 6073,
/* 6070 */ 50059,
/* 6071 */ -6066,
/* 6072 */ 381,
/* 6073 */ 6079,
/* 6074 */ 50044,
/* 6075 */ 50258,
/* 6076 */ 6073,
/* 6077 */ -6073,
/* 6078 */ 999,
/* 6079 */ 0,
/* 6080 */ -6073,
/* 6081 */ 1410,
/* 6082 */ 0,
/* 6083 */ 5826,
/* 6084 */ 50258,
/* 6085 */ 6089,
/* 6086 */ 50059,
/* 6087 */ -6082,
/* 6088 */ 382,
/* 6089 */ 6095,
/* 6090 */ 50044,
/* 6091 */ 50258,
/* 6092 */ 6089,
/* 6093 */ -6089,
/* 6094 */ 1000,
/* 6095 */ 0,
/* 6096 */ -6089,
/* 6097 */ 1411,
/* 6098 */ 6103,
/* 6099 */ 6699,
/* 6100 */ 50059,
/* 6101 */ -6098,
/* 6102 */ 383,
/* 6103 */ 6113,
/* 6104 */ 50366,
/* 6105 */ 50040,
/* 6106 */ 6699,
/* 6107 */ 50041,
/* 6108 */ 6098,
/* 6109 */ 50334,
/* 6110 */ 6098,
/* 6111 */ -6098,
/* 6112 */ 384,
/* 6113 */ 6121,
/* 6114 */ 50366,
/* 6115 */ 50040,
/* 6116 */ 6699,
/* 6117 */ 50041,
/* 6118 */ 6098,
/* 6119 */ -6098,
/* 6120 */ 385,
/* 6121 */ 6133,
/* 6122 */ 50352,
/* 6123 */ 50040,
/* 6124 */ 6699,
/* 6125 */ 50059,
/* 6126 */ 6699,
/* 6127 */ 50059,
/* 6128 */ 6699,
/* 6129 */ 50041,
/* 6130 */ 6098,
/* 6131 */ -6098,
/* 6132 */ 386,
/* 6133 */ 6144,
/* 6134 */ 50445,
/* 6135 */ 50040,
/* 6136 */ 6699,
/* 6137 */ 50041,
/* 6138 */ 50123,
/* 6139 */ 6187,
/* 6140 */ 6214,
/* 6141 */ 50125,
/* 6142 */ -6098,
/* 6143 */ 387,
/* 6144 */ 6149,
/* 6145 */ 50301,
/* 6146 */ 50059,
/* 6147 */ -6098,
/* 6148 */ 388,
/* 6149 */ 6154,
/* 6150 */ 50313,
/* 6151 */ 50059,
/* 6152 */ -6098,
/* 6153 */ 389,
/* 6154 */ 6164,
/* 6155 */ 50330,
/* 6156 */ 6098,
/* 6157 */ 50468,
/* 6158 */ 50040,
/* 6159 */ 6699,
/* 6160 */ 50041,
/* 6161 */ 50059,
/* 6162 */ -6098,
/* 6163 */ 390,
/* 6164 */ 6172,
/* 6165 */ 50468,
/* 6166 */ 50040,
/* 6167 */ 6699,
/* 6168 */ 50041,
/* 6169 */ 6098,
/* 6170 */ -6098,
/* 6171 */ 391,
/* 6172 */ 6176,
/* 6173 */ 6231,
/* 6174 */ -6098,
/* 6175 */ 392,
/* 6176 */ 6182,
/* 6177 */ 50424,
/* 6178 */ 6699,
/* 6179 */ 50059,
/* 6180 */ -6098,
/* 6181 */ 393,
/* 6182 */ 0,
/* 6183 */ 50424,
/* 6184 */ 50059,
/* 6185 */ -6098,
/* 6186 */ 394,
/* 6187 */ 6195,
/* 6188 */ 50304,
/* 6189 */ 6198,
/* 6190 */ 50058,
/* 6191 */ 6206,
/* 6192 */ 6187,
/* 6193 */ -6187,
/* 6194 */ 1004,
/* 6195 */ 0,
/* 6196 */ -6187,
/* 6197 */ 1413,
/* 6198 */ 6202,
/* 6199 */ 173,
/* 6200 */ -6198,
/* 6201 */ 1001,
/* 6202 */ 0,
/* 6203 */ 163,
/* 6204 */ -6198,
/* 6205 */ 1002,
/* 6206 */ 6211,
/* 6207 */ 6098,
/* 6208 */ 6206,
/* 6209 */ -6206,
/* 6210 */ 1003,
/* 6211 */ 0,
/* 6212 */ -6206,
/* 6213 */ 1412,
/* 6214 */ 6220,
/* 6215 */ 50323,
/* 6216 */ 50058,
/* 6217 */ 6223,
/* 6218 */ -6214,
/* 6219 */ 1006,
/* 6220 */ 0,
/* 6221 */ -6214,
/* 6222 */ 1415,
/* 6223 */ 6228,
/* 6224 */ 6098,
/* 6225 */ 6223,
/* 6226 */ -6223,
/* 6227 */ 1005,
/* 6228 */ 0,
/* 6229 */ -6223,
/* 6230 */ 1414,
/* 6231 */ 0,
/* 6232 */ 50123,
/* 6233 */ 6237,
/* 6234 */ 50125,
/* 6235 */ -6231,
/* 6236 */ 395,
/* 6237 */ 6242,
/* 6238 */ 6098,
/* 6239 */ 6237,
/* 6240 */ -6237,
/* 6241 */ 1007,
/* 6242 */ 0,
/* 6243 */ -6237,
/* 6244 */ 1416,
/* 6245 */ 0,
/* 6246 */ 50123,
/* 6247 */ 6252,
/* 6248 */ 6266,
/* 6249 */ 50125,
/* 6250 */ -6245,
/* 6251 */ 396,
/* 6252 */ 6257,
/* 6253 */ 6066,
/* 6254 */ 6252,
/* 6255 */ -6252,
/* 6256 */ 1008,
/* 6257 */ 6263,
/* 6258 */ 50440,
/* 6259 */ 6082,
/* 6260 */ 6252,
/* 6261 */ -6252,
/* 6262 */ 1009,
/* 6263 */ 0,
/* 6264 */ -6252,
/* 6265 */ 1417,
/* 6266 */ 6271,
/* 6267 */ 6098,
/* 6268 */ 6266,
/* 6269 */ -6266,
/* 6270 */ 1010,
/* 6271 */ 0,
/* 6272 */ -6266,
/* 6273 */ 1418,
/* 6274 */ 6278,
/* 6275 */ 50256,
/* 6276 */ -6274,
/* 6277 */ 397,
/* 6278 */ 6282,
/* 6279 */ 50259,
/* 6280 */ -6274,
/* 6281 */ 398,
/* 6282 */ 6286,
/* 6283 */ 50257,
/* 6284 */ -6274,
/* 6285 */ 399,
/* 6286 */ 6290,
/* 6287 */ 50457,
/* 6288 */ -6274,
/* 6289 */ 400,
/* 6290 */ 6294,
/* 6291 */ 50344,
/* 6292 */ -6274,
/* 6293 */ 401,
/* 6294 */ 6298,
/* 6295 */ 50394,
/* 6296 */ -6274,
/* 6297 */ 402,
/* 6298 */ 6302,
/* 6299 */ 163,
/* 6300 */ -6274,
/* 6301 */ 403,
/* 6302 */ 6308,
/* 6303 */ 50040,
/* 6304 */ 6699,
/* 6305 */ 50041,
/* 6306 */ -6274,
/* 6307 */ 404,
/* 6308 */ 6312,
/* 6309 */ 50258,
/* 6310 */ -6274,
/* 6311 */ 405,
/* 6312 */ 6318,
/* 6313 */ 6274,
/* 6314 */ 6340,
/* 6315 */ 50258,
/* 6316 */ -6274,
/* 6317 */ 406,
/* 6318 */ 6327,
/* 6319 */ 6274,
/* 6320 */ 6348,
/* 6321 */ 50258,
/* 6322 */ 50040,
/* 6323 */ 6709,
/* 6324 */ 50041,
/* 6325 */ -6274,
/* 6326 */ 407,
/* 6327 */ 6334,
/* 6328 */ 50258,
/* 6329 */ 50040,
/* 6330 */ 6709,
/* 6331 */ 50041,
/* 6332 */ -6274,
/* 6333 */ 408,
/* 6334 */ 0,
/* 6335 */ 6274,
/* 6336 */ 50064,
/* 6337 */ 50258,
/* 6338 */ -6274,
/* 6339 */ 409,
/* 6340 */ 6344,
/* 6341 */ 50046,
/* 6342 */ -6340,
/* 6343 */ 1011,
/* 6344 */ 0,
/* 6345 */ 50495,
/* 6346 */ -6340,
/* 6347 */ 1012,
/* 6348 */ 6352,
/* 6349 */ 50046,
/* 6350 */ -6348,
/* 6351 */ 1013,
/* 6352 */ 0,
/* 6353 */ 50495,
/* 6354 */ -6348,
/* 6355 */ 1014,
/* 6356 */ 6360,
/* 6357 */ 50258,
/* 6358 */ -6356,
/* 6359 */ 410,
/* 6360 */ 6366,
/* 6361 */ 6356,
/* 6362 */ 50064,
/* 6363 */ 50258,
/* 6364 */ -6356,
/* 6365 */ 411,
/* 6366 */ 0,
/* 6367 */ 6274,
/* 6368 */ 6372,
/* 6369 */ 50258,
/* 6370 */ -6356,
/* 6371 */ 412,
/* 6372 */ 6376,
/* 6373 */ 50046,
/* 6374 */ -6372,
/* 6375 */ 1015,
/* 6376 */ 0,
/* 6377 */ 50495,
/* 6378 */ -6372,
/* 6379 */ 1016,
/* 6380 */ 6385,
/* 6381 */ 6356,
/* 6382 */ 50474,
/* 6383 */ -6380,
/* 6384 */ 413,
/* 6385 */ 6390,
/* 6386 */ 6356,
/* 6387 */ 50475,
/* 6388 */ -6380,
/* 6389 */ 414,
/* 6390 */ 6399,
/* 6391 */ 50386,
/* 6392 */ 50040,
/* 6393 */ 6629,
/* 6394 */ 50044,
/* 6395 */ 6629,
/* 6396 */ 50041,
/* 6397 */ -6380,
/* 6398 */ 415,
/* 6399 */ 6408,
/* 6400 */ 50383,
/* 6401 */ 50040,
/* 6402 */ 6629,
/* 6403 */ 50044,
/* 6404 */ 6629,
/* 6405 */ 50041,
/* 6406 */ -6380,
/* 6407 */ 416,
/* 6408 */ 0,
/* 6409 */ 6274,
/* 6410 */ -6380,
/* 6411 */ 417,
/* 6412 */ 6417,
/* 6413 */ 50474,
/* 6414 */ 6356,
/* 6415 */ -6412,
/* 6416 */ 418,
/* 6417 */ 6422,
/* 6418 */ 50475,
/* 6419 */ 6356,
/* 6420 */ -6412,
/* 6421 */ 419,
/* 6422 */ 6427,
/* 6423 */ 50043,
/* 6424 */ 6412,
/* 6425 */ -6412,
/* 6426 */ 420,
/* 6427 */ 6432,
/* 6428 */ 50045,
/* 6429 */ 6412,
/* 6430 */ -6412,
/* 6431 */ 421,
/* 6432 */ 6437,
/* 6433 */ 50033,
/* 6434 */ 6412,
/* 6435 */ -6412,
/* 6436 */ 422,
/* 6437 */ 6442,
/* 6438 */ 50126,
/* 6439 */ 6412,
/* 6440 */ -6412,
/* 6441 */ 423,
/* 6442 */ 6448,
/* 6443 */ 50058,
/* 6444 */ 6412,
/* 6445 */ 50058,
/* 6446 */ -6412,
/* 6447 */ 424,
/* 6448 */ 0,
/* 6449 */ 6380,
/* 6450 */ -6412,
/* 6451 */ 425,
/* 6452 */ 6458,
/* 6453 */ 6412,
/* 6454 */ 50100,
/* 6455 */ 6412,
/* 6456 */ -6452,
/* 6457 */ 426,
/* 6458 */ 0,
/* 6459 */ 6412,
/* 6460 */ -6452,
/* 6461 */ 427,
/* 6462 */ 6469,
/* 6463 */ 50040,
/* 6464 */ 5826,
/* 6465 */ 50041,
/* 6466 */ 6462,
/* 6467 */ -6462,
/* 6468 */ 428,
/* 6469 */ 0,
/* 6470 */ 6452,
/* 6471 */ -6462,
/* 6472 */ 429,
/* 6473 */ 6479,
/* 6474 */ 6473,
/* 6475 */ 50042,
/* 6476 */ 6462,
/* 6477 */ -6473,
/* 6478 */ 430,
/* 6479 */ 6485,
/* 6480 */ 6473,
/* 6481 */ 50047,
/* 6482 */ 6462,
/* 6483 */ -6473,
/* 6484 */ 431,
/* 6485 */ 6491,
/* 6486 */ 6473,
/* 6487 */ 50037,
/* 6488 */ 6462,
/* 6489 */ -6473,
/* 6490 */ 432,
/* 6491 */ 0,
/* 6492 */ 6462,
/* 6493 */ -6473,
/* 6494 */ 433,
/* 6495 */ 6501,
/* 6496 */ 6495,
/* 6497 */ 50043,
/* 6498 */ 6473,
/* 6499 */ -6495,
/* 6500 */ 434,
/* 6501 */ 6507,
/* 6502 */ 6495,
/* 6503 */ 50045,
/* 6504 */ 6473,
/* 6505 */ -6495,
/* 6506 */ 435,
/* 6507 */ 0,
/* 6508 */ 6473,
/* 6509 */ -6495,
/* 6510 */ 436,
/* 6511 */ 6517,
/* 6512 */ 6511,
/* 6513 */ 50476,
/* 6514 */ 6495,
/* 6515 */ -6511,
/* 6516 */ 437,
/* 6517 */ 6523,
/* 6518 */ 6511,
/* 6519 */ 50477,
/* 6520 */ 6495,
/* 6521 */ -6511,
/* 6522 */ 438,
/* 6523 */ 0,
/* 6524 */ 6495,
/* 6525 */ -6511,
/* 6526 */ 439,
/* 6527 */ 6533,
/* 6528 */ 6527,
/* 6529 */ 50060,
/* 6530 */ 6511,
/* 6531 */ -6527,
/* 6532 */ 440,
/* 6533 */ 6539,
/* 6534 */ 6527,
/* 6535 */ 50062,
/* 6536 */ 6511,
/* 6537 */ -6527,
/* 6538 */ 441,
/* 6539 */ 6545,
/* 6540 */ 6527,
/* 6541 */ 50478,
/* 6542 */ 6511,
/* 6543 */ -6527,
/* 6544 */ 442,
/* 6545 */ 6551,
/* 6546 */ 6527,
/* 6547 */ 50479,
/* 6548 */ 6511,
/* 6549 */ -6527,
/* 6550 */ 443,
/* 6551 */ 6557,
/* 6552 */ 6527,
/* 6553 */ 50480,
/* 6554 */ 6511,
/* 6555 */ -6527,
/* 6556 */ 444,
/* 6557 */ 6563,
/* 6558 */ 6527,
/* 6559 */ 50481,
/* 6560 */ 6511,
/* 6561 */ -6527,
/* 6562 */ 445,
/* 6563 */ 0,
/* 6564 */ 6511,
/* 6565 */ -6527,
/* 6566 */ 446,
/* 6567 */ 6573,
/* 6568 */ 6567,
/* 6569 */ 50038,
/* 6570 */ 6527,
/* 6571 */ -6567,
/* 6572 */ 447,
/* 6573 */ 0,
/* 6574 */ 6527,
/* 6575 */ -6567,
/* 6576 */ 448,
/* 6577 */ 6583,
/* 6578 */ 6577,
/* 6579 */ 50094,
/* 6580 */ 6567,
/* 6581 */ -6577,
/* 6582 */ 449,
/* 6583 */ 0,
/* 6584 */ 6567,
/* 6585 */ -6577,
/* 6586 */ 450,
/* 6587 */ 6593,
/* 6588 */ 6587,
/* 6589 */ 50124,
/* 6590 */ 6577,
/* 6591 */ -6587,
/* 6592 */ 451,
/* 6593 */ 0,
/* 6594 */ 6577,
/* 6595 */ -6587,
/* 6596 */ 452,
/* 6597 */ 6603,
/* 6598 */ 6597,
/* 6599 */ 50482,
/* 6600 */ 6587,
/* 6601 */ -6597,
/* 6602 */ 453,
/* 6603 */ 0,
/* 6604 */ 6587,
/* 6605 */ -6597,
/* 6606 */ 454,
/* 6607 */ 6613,
/* 6608 */ 6607,
/* 6609 */ 50483,
/* 6610 */ 6597,
/* 6611 */ -6607,
/* 6612 */ 455,
/* 6613 */ 0,
/* 6614 */ 6597,
/* 6615 */ -6607,
/* 6616 */ 456,
/* 6617 */ 6625,
/* 6618 */ 6607,
/* 6619 */ 50063,
/* 6620 */ 6699,
/* 6621 */ 50058,
/* 6622 */ 6617,
/* 6623 */ -6617,
/* 6624 */ 457,
/* 6625 */ 0,
/* 6626 */ 6607,
/* 6627 */ -6617,
/* 6628 */ 458,
/* 6629 */ 6635,
/* 6630 */ 6356,
/* 6631 */ 50061,
/* 6632 */ 6617,
/* 6633 */ -6629,
/* 6634 */ 459,
/* 6635 */ 6641,
/* 6636 */ 6356,
/* 6637 */ 50485,
/* 6638 */ 6617,
/* 6639 */ -6629,
/* 6640 */ 460,
/* 6641 */ 6647,
/* 6642 */ 6356,
/* 6643 */ 50493,
/* 6644 */ 6617,
/* 6645 */ -6629,
/* 6646 */ 461,
/* 6647 */ 6653,
/* 6648 */ 6356,
/* 6649 */ 50484,
/* 6650 */ 6617,
/* 6651 */ -6629,
/* 6652 */ 462,
/* 6653 */ 6659,
/* 6654 */ 6356,
/* 6655 */ 50486,
/* 6656 */ 6617,
/* 6657 */ -6629,
/* 6658 */ 463,
/* 6659 */ 6665,
/* 6660 */ 6356,
/* 6661 */ 50487,
/* 6662 */ 6617,
/* 6663 */ -6629,
/* 6664 */ 464,
/* 6665 */ 6671,
/* 6666 */ 6356,
/* 6667 */ 50488,
/* 6668 */ 6617,
/* 6669 */ -6629,
/* 6670 */ 465,
/* 6671 */ 6677,
/* 6672 */ 6356,
/* 6673 */ 50489,
/* 6674 */ 6617,
/* 6675 */ -6629,
/* 6676 */ 466,
/* 6677 */ 6683,
/* 6678 */ 6356,
/* 6679 */ 50491,
/* 6680 */ 6617,
/* 6681 */ -6629,
/* 6682 */ 467,
/* 6683 */ 6689,
/* 6684 */ 6356,
/* 6685 */ 50492,
/* 6686 */ 6617,
/* 6687 */ -6629,
/* 6688 */ 468,
/* 6689 */ 6695,
/* 6690 */ 6356,
/* 6691 */ 50490,
/* 6692 */ 6617,
/* 6693 */ -6629,
/* 6694 */ 469,
/* 6695 */ 0,
/* 6696 */ 6617,
/* 6697 */ -6629,
/* 6698 */ 470,
/* 6699 */ 6705,
/* 6700 */ 6699,
/* 6701 */ 50044,
/* 6702 */ 6629,
/* 6703 */ -6699,
/* 6704 */ 471,
/* 6705 */ 0,
/* 6706 */ 6629,
/* 6707 */ -6699,
/* 6708 */ 472,
/* 6709 */ 0,
/* 6710 */ 6713,
/* 6711 */ -6709,
/* 6712 */ 473,
/* 6713 */ 6718,
/* 6714 */ 6629,
/* 6715 */ 6721,
/* 6716 */ -6713,
/* 6717 */ 1018,
/* 6718 */ 0,
/* 6719 */ -6713,
/* 6720 */ 1420,
/* 6721 */ 6727,
/* 6722 */ 50044,
/* 6723 */ 6629,
/* 6724 */ 6721,
/* 6725 */ -6721,
/* 6726 */ 1017,
/* 6727 */ 0,
/* 6728 */ -6721,
/* 6729 */ 1419,
0
};
extern int yyannotation[];
int yyannotation[] = {
0,
/* 1 */ 0,
/* 2 */ -1,
/* 3 */ 50000,
/* 4 */ -1,
/* 5 */ 0,
/* 6 */ 0,
/* 7 */ -1,
/* 8 */ -6,
/* 9 */ -1,
/* 10 */ 15,
/* 11 */ -1,
/* 12 */ -1,
/* 13 */ -10,
/* 14 */ -1,
/* 15 */ 0,
/* 16 */ -10,
/* 17 */ -1,
/* 18 */ 24,
/* 19 */ 50379,
/* 20 */ 50257,
/* 21 */ 50059,
/* 22 */ -18,
/* 23 */ -1,
/* 24 */ 30,
/* 25 */ 50432,
/* 26 */ -1,
/* 27 */ 50059,
/* 28 */ -18,
/* 29 */ -1,
/* 30 */ 36,
/* 31 */ 50351,
/* 32 */ 50257,
/* 33 */ 50059,
/* 34 */ -18,
/* 35 */ -1,
/* 36 */ 40,
/* 37 */ -1,
/* 38 */ -18,
/* 39 */ -1,
/* 40 */ 44,
/* 41 */ -1,
/* 42 */ -18,
/* 43 */ -1,
/* 44 */ 48,
/* 45 */ -1,
/* 46 */ -18,
/* 47 */ -1,
/* 48 */ 52,
/* 49 */ -1,
/* 50 */ -18,
/* 51 */ -1,
/* 52 */ 56,
/* 53 */ -1,
/* 54 */ -18,
/* 55 */ -1,
/* 56 */ 60,
/* 57 */ -1,
/* 58 */ -18,
/* 59 */ -1,
/* 60 */ 64,
/* 61 */ -1,
/* 62 */ -18,
/* 63 */ -1,
/* 64 */ 68,
/* 65 */ -1,
/* 66 */ -18,
/* 67 */ -1,
/* 68 */ 72,
/* 69 */ -1,
/* 70 */ -18,
/* 71 */ -1,
/* 72 */ 76,
/* 73 */ -1,
/* 74 */ -18,
/* 75 */ -1,
/* 76 */ 80,
/* 77 */ -1,
/* 78 */ -18,
/* 79 */ -1,
/* 80 */ 84,
/* 81 */ -1,
/* 82 */ -18,
/* 83 */ -1,
/* 84 */ 88,
/* 85 */ -1,
/* 86 */ -18,
/* 87 */ -1,
/* 88 */ 92,
/* 89 */ -1,
/* 90 */ -18,
/* 91 */ -1,
/* 92 */ 96,
/* 93 */ -1,
/* 94 */ -18,
/* 95 */ -1,
/* 96 */ 100,
/* 97 */ -1,
/* 98 */ -18,
/* 99 */ -1,
/* 100 */ 104,
/* 101 */ -1,
/* 102 */ -18,
/* 103 */ -1,
/* 104 */ 108,
/* 105 */ -1,
/* 106 */ -18,
/* 107 */ -1,
/* 108 */ 112,
/* 109 */ -1,
/* 110 */ -18,
/* 111 */ -1,
/* 112 */ 116,
/* 113 */ -1,
/* 114 */ -18,
/* 115 */ -1,
/* 116 */ 120,
/* 117 */ -1,
/* 118 */ -18,
/* 119 */ -1,
/* 120 */ 124,
/* 121 */ -1,
/* 122 */ -18,
/* 123 */ -1,
/* 124 */ 128,
/* 125 */ -1,
/* 126 */ -18,
/* 127 */ -1,
/* 128 */ 134,
/* 129 */ 50349,
/* 130 */ -1,
/* 131 */ -1,
/* 132 */ -18,
/* 133 */ -1,
/* 134 */ 0,
/* 135 */ 50450,
/* 136 */ 50257,
/* 137 */ 50123,
/* 138 */ -1,
/* 139 */ 50257,
/* 140 */ -1,
/* 141 */ 50125,
/* 142 */ -18,
/* 143 */ -1,
/* 144 */ 0,
/* 145 */ 50058,
/* 146 */ -144,
/* 147 */ -1,
/* 148 */ 153,
/* 149 */ 50325,
/* 150 */ 50058,
/* 151 */ -148,
/* 152 */ -1,
/* 153 */ 0,
/* 154 */ -148,
/* 155 */ -1,
/* 156 */ 160,
/* 157 */ 50059,
/* 158 */ -156,
/* 159 */ -1,
/* 160 */ 0,
/* 161 */ -156,
/* 162 */ -1,
/* 163 */ 168,
/* 164 */ 50036,
/* 165 */ 50257,
/* 166 */ -163,
/* 167 */ -1,
/* 168 */ 0,
/* 169 */ 50036,
/* 170 */ -1,
/* 171 */ -163,
/* 172 */ -1,
/* 173 */ 179,
/* 174 */ -1,
/* 175 */ 50043,
/* 176 */ -1,
/* 177 */ -173,
/* 178 */ -1,
/* 179 */ 185,
/* 180 */ -1,
/* 181 */ 50045,
/* 182 */ -1,
/* 183 */ -173,
/* 184 */ -1,
/* 185 */ 0,
/* 186 */ -1,
/* 187 */ -173,
/* 188 */ -1,
/* 189 */ 195,
/* 190 */ -1,
/* 191 */ 50042,
/* 192 */ -1,
/* 193 */ -189,
/* 194 */ -1,
/* 195 */ 201,
/* 196 */ -1,
/* 197 */ 50047,
/* 198 */ -1,
/* 199 */ -189,
/* 200 */ -1,
/* 201 */ 207,
/* 202 */ -1,
/* 203 */ 50037,
/* 204 */ -1,
/* 205 */ -189,
/* 206 */ -1,
/* 207 */ 213,
/* 208 */ -1,
/* 209 */ 50124,
/* 210 */ -1,
/* 211 */ -189,
/* 212 */ -1,
/* 213 */ 219,
/* 214 */ -1,
/* 215 */ 50038,
/* 216 */ -1,
/* 217 */ -189,
/* 218 */ -1,
/* 219 */ 0,
/* 220 */ -1,
/* 221 */ -189,
/* 222 */ -1,
/* 223 */ 228,
/* 224 */ 50043,
/* 225 */ -1,
/* 226 */ -223,
/* 227 */ -1,
/* 228 */ 233,
/* 229 */ 50045,
/* 230 */ -1,
/* 231 */ -223,
/* 232 */ -1,
/* 233 */ 0,
/* 234 */ -1,
/* 235 */ -223,
/* 236 */ -1,
/* 237 */ 243,
/* 238 */ 50040,
/* 239 */ -1,
/* 240 */ 50041,
/* 241 */ -237,
/* 242 */ -1,
/* 243 */ 247,
/* 244 */ 50256,
/* 245 */ -237,
/* 246 */ -1,
/* 247 */ 251,
/* 248 */ 50457,
/* 249 */ -237,
/* 250 */ -1,
/* 251 */ 255,
/* 252 */ 50344,
/* 253 */ -237,
/* 254 */ -1,
/* 255 */ 0,
/* 256 */ 50259,
/* 257 */ -237,
/* 258 */ -1,
/* 259 */ 267,
/* 260 */ -1,
/* 261 */ 50100,
/* 262 */ -1,
/* 263 */ 50043,
/* 264 */ -1,
/* 265 */ -259,
/* 266 */ -1,
/* 267 */ 276,
/* 268 */ -1,
/* 269 */ 50100,
/* 270 */ -1,
/* 271 */ 50043,
/* 272 */ 50045,
/* 273 */ -1,
/* 274 */ -259,
/* 275 */ -1,
/* 276 */ 282,
/* 277 */ -1,
/* 278 */ 50100,
/* 279 */ -1,
/* 280 */ -259,
/* 281 */ -1,
/* 282 */ 0,
/* 283 */ -1,
/* 284 */ -259,
/* 285 */ -1,
/* 286 */ 294,
/* 287 */ -1,
/* 288 */ 50100,
/* 289 */ -1,
/* 290 */ 50043,
/* 291 */ -1,
/* 292 */ -286,
/* 293 */ -1,
/* 294 */ 0,
/* 295 */ -1,
/* 296 */ 50100,
/* 297 */ -1,
/* 298 */ -286,
/* 299 */ -1,
/* 300 */ 0,
/* 301 */ -1,
/* 302 */ -1,
/* 303 */ -1,
/* 304 */ -1,
/* 305 */ -1,
/* 306 */ -1,
/* 307 */ -1,
/* 308 */ -300,
/* 309 */ -1,
/* 310 */ 317,
/* 311 */ 50366,
/* 312 */ 50040,
/* 313 */ -1,
/* 314 */ 50041,
/* 315 */ -310,
/* 316 */ -1,
/* 317 */ 0,
/* 318 */ -310,
/* 319 */ -1,
/* 320 */ 325,
/* 321 */ 50256,
/* 322 */ 50037,
/* 323 */ -320,
/* 324 */ -1,
/* 325 */ 0,
/* 326 */ -320,
/* 327 */ -1,
/* 328 */ 332,
/* 329 */ -1,
/* 330 */ -328,
/* 331 */ -1,
/* 332 */ 0,
/* 333 */ -328,
/* 334 */ -1,
/* 335 */ 339,
/* 336 */ 50316,
/* 337 */ -335,
/* 338 */ -1,
/* 339 */ 343,
/* 340 */ 50299,
/* 341 */ -335,
/* 342 */ -1,
/* 343 */ 0,
/* 344 */ -335,
/* 345 */ -1,
/* 346 */ 350,
/* 347 */ -1,
/* 348 */ -346,
/* 349 */ -1,
/* 350 */ 0,
/* 351 */ 50418,
/* 352 */ -1,
/* 353 */ -346,
/* 354 */ -1,
/* 355 */ 360,
/* 356 */ 50043,
/* 357 */ 50256,
/* 358 */ -355,
/* 359 */ -1,
/* 360 */ 365,
/* 361 */ 50045,
/* 362 */ 50256,
/* 363 */ -355,
/* 364 */ -1,
/* 365 */ 0,
/* 366 */ -355,
/* 367 */ -1,
/* 368 */ 376,
/* 369 */ 50470,
/* 370 */ 50091,
/* 371 */ -1,
/* 372 */ 50093,
/* 373 */ -1,
/* 374 */ -368,
/* 375 */ -1,
/* 376 */ 382,
/* 377 */ 50398,
/* 378 */ -1,
/* 379 */ -1,
/* 380 */ -368,
/* 381 */ -1,
/* 382 */ 391,
/* 383 */ 50398,
/* 384 */ 50418,
/* 385 */ -1,
/* 386 */ 50456,
/* 387 */ -1,
/* 388 */ -1,
/* 389 */ -368,
/* 390 */ -1,
/* 391 */ 398,
/* 392 */ 50398,
/* 393 */ 50418,
/* 394 */ -1,
/* 395 */ -1,
/* 396 */ -368,
/* 397 */ -1,
/* 398 */ 404,
/* 399 */ 50398,
/* 400 */ 50418,
/* 401 */ -1,
/* 402 */ -368,
/* 403 */ -1,
/* 404 */ 414,
/* 405 */ 50294,
/* 406 */ 50040,
/* 407 */ -1,
/* 408 */ 50044,
/* 409 */ -1,
/* 410 */ 50041,
/* 411 */ -1,
/* 412 */ -368,
/* 413 */ -1,
/* 414 */ 0,
/* 415 */ -368,
/* 416 */ -1,
/* 417 */ 423,
/* 418 */ -1,
/* 419 */ -1,
/* 420 */ -1,
/* 421 */ -417,
/* 422 */ -1,
/* 423 */ 0,
/* 424 */ -417,
/* 425 */ -1,
/* 426 */ 430,
/* 427 */ 50044,
/* 428 */ -426,
/* 429 */ -1,
/* 430 */ 0,
/* 431 */ -426,
/* 432 */ -1,
/* 433 */ 0,
/* 434 */ 50355,
/* 435 */ -1,
/* 436 */ -1,
/* 437 */ -1,
/* 438 */ 50059,
/* 439 */ -433,
/* 440 */ -1,
/* 441 */ 0,
/* 442 */ 50058,
/* 443 */ -441,
/* 444 */ -1,
/* 445 */ 451,
/* 446 */ -1,
/* 447 */ -1,
/* 448 */ -1,
/* 449 */ -445,
/* 450 */ -1,
/* 451 */ 0,
/* 452 */ -445,
/* 453 */ -1,
/* 454 */ 458,
/* 455 */ 50044,
/* 456 */ -454,
/* 457 */ -1,
/* 458 */ 0,
/* 459 */ 50334,
/* 460 */ -454,
/* 461 */ -1,
/* 462 */ 0,
/* 463 */ -1,
/* 464 */ -1,
/* 465 */ -1,
/* 466 */ -1,
/* 467 */ -1,
/* 468 */ 50059,
/* 469 */ -462,
/* 470 */ -1,
/* 471 */ 475,
/* 472 */ 50367,
/* 473 */ -471,
/* 474 */ -1,
/* 475 */ 0,
/* 476 */ 50356,
/* 477 */ -471,
/* 478 */ -1,
/* 479 */ 483,
/* 480 */ 50058,
/* 481 */ -479,
/* 482 */ -1,
/* 483 */ 0,
/* 484 */ -479,
/* 485 */ -1,
/* 486 */ 490,
/* 487 */ -1,
/* 488 */ -486,
/* 489 */ -1,
/* 490 */ 0,
/* 491 */ -486,
/* 492 */ -1,
/* 493 */ 497,
/* 494 */ 50259,
/* 495 */ -493,
/* 496 */ -1,
/* 497 */ 0,
/* 498 */ 50256,
/* 499 */ -493,
/* 500 */ -1,
/* 501 */ 506,
/* 502 */ 50399,
/* 503 */ -1,
/* 504 */ -501,
/* 505 */ -1,
/* 506 */ 0,
/* 507 */ -501,
/* 508 */ -1,
/* 509 */ 0,
/* 510 */ -1,
/* 511 */ -1,
/* 512 */ -1,
/* 513 */ -509,
/* 514 */ -1,
/* 515 */ 519,
/* 516 */ -1,
/* 517 */ -515,
/* 518 */ -1,
/* 519 */ 0,
/* 520 */ -515,
/* 521 */ -1,
/* 522 */ 526,
/* 523 */ 50259,
/* 524 */ -522,
/* 525 */ -1,
/* 526 */ 0,
/* 527 */ 50256,
/* 528 */ -522,
/* 529 */ -1,
/* 530 */ 535,
/* 531 */ 50399,
/* 532 */ -1,
/* 533 */ -530,
/* 534 */ -1,
/* 535 */ 0,
/* 536 */ -530,
/* 537 */ -1,
/* 538 */ 0,
/* 539 */ 50399,
/* 540 */ 50341,
/* 541 */ -1,
/* 542 */ -1,
/* 543 */ 50059,
/* 544 */ -538,
/* 545 */ -1,
/* 546 */ 552,
/* 547 */ 50044,
/* 548 */ -1,
/* 549 */ -1,
/* 550 */ -546,
/* 551 */ -1,
/* 552 */ 0,
/* 553 */ -546,
/* 554 */ -1,
/* 555 */ 0,
/* 556 */ -1,
/* 557 */ -1,
/* 558 */ -1,
/* 559 */ -555,
/* 560 */ -1,
/* 561 */ 567,
/* 562 */ 50044,
/* 563 */ -1,
/* 564 */ -1,
/* 565 */ -561,
/* 566 */ -1,
/* 567 */ 0,
/* 568 */ -561,
/* 569 */ -1,
/* 570 */ 575,
/* 571 */ 50257,
/* 572 */ -1,
/* 573 */ -570,
/* 574 */ -1,
/* 575 */ 0,
/* 576 */ -1,
/* 577 */ -570,
/* 578 */ -1,
/* 579 */ 585,
/* 580 */ 50047,
/* 581 */ 50257,
/* 582 */ -1,
/* 583 */ -579,
/* 584 */ -1,
/* 585 */ 0,
/* 586 */ -579,
/* 587 */ -1,
/* 588 */ 0,
/* 589 */ -1,
/* 590 */ -588,
/* 591 */ -1,
/* 592 */ 0,
/* 593 */ -1,
/* 594 */ -592,
/* 595 */ -1,
/* 596 */ 0,
/* 597 */ -1,
/* 598 */ -1,
/* 599 */ -596,
/* 600 */ -1,
/* 601 */ 606,
/* 602 */ -1,
/* 603 */ -1,
/* 604 */ -601,
/* 605 */ -1,
/* 606 */ 611,
/* 607 */ 50045,
/* 608 */ -1,
/* 609 */ -601,
/* 610 */ -1,
/* 611 */ 616,
/* 612 */ 50061,
/* 613 */ -1,
/* 614 */ -601,
/* 615 */ -1,
/* 616 */ 0,
/* 617 */ -1,
/* 618 */ 50037,
/* 619 */ -601,
/* 620 */ -1,
/* 621 */ 625,
/* 622 */ 50043,
/* 623 */ -621,
/* 624 */ -1,
/* 625 */ 0,
/* 626 */ -621,
/* 627 */ -1,
/* 628 */ 634,
/* 629 */ 50040,
/* 630 */ -1,
/* 631 */ 50041,
/* 632 */ -628,
/* 633 */ -1,
/* 634 */ 0,
/* 635 */ -628,
/* 636 */ -1,
/* 637 */ 642,
/* 638 */ 50386,
/* 639 */ -1,
/* 640 */ -637,
/* 641 */ -1,
/* 642 */ 647,
/* 643 */ 50383,
/* 644 */ -1,
/* 645 */ -637,
/* 646 */ -1,
/* 647 */ 0,
/* 648 */ 50390,
/* 649 */ -1,
/* 650 */ -637,
/* 651 */ -1,
/* 652 */ 0,
/* 653 */ 50387,
/* 654 */ 50257,
/* 655 */ 50058,
/* 656 */ -1,
/* 657 */ -1,
/* 658 */ 50123,
/* 659 */ -1,
/* 660 */ 50125,
/* 661 */ -652,
/* 662 */ -1,
/* 663 */ 669,
/* 664 */ 50044,
/* 665 */ -1,
/* 666 */ -1,
/* 667 */ -663,
/* 668 */ -1,
/* 669 */ 0,
/* 670 */ -663,
/* 671 */ -1,
/* 672 */ 677,
/* 673 */ 50044,
/* 674 */ -1,
/* 675 */ -672,
/* 676 */ -1,
/* 677 */ 0,
/* 678 */ -672,
/* 679 */ -1,
/* 680 */ 685,
/* 681 */ -1,
/* 682 */ -1,
/* 683 */ -680,
/* 684 */ -1,
/* 685 */ 0,
/* 686 */ -680,
/* 687 */ -1,
/* 688 */ 695,
/* 689 */ 50325,
/* 690 */ -1,
/* 691 */ 50257,
/* 692 */ 50059,
/* 693 */ -688,
/* 694 */ -1,
/* 695 */ 702,
/* 696 */ 50324,
/* 697 */ -1,
/* 698 */ -1,
/* 699 */ 50059,
/* 700 */ -688,
/* 701 */ -1,
/* 702 */ 709,
/* 703 */ -1,
/* 704 */ -1,
/* 705 */ -1,
/* 706 */ 50059,
/* 707 */ -688,
/* 708 */ -1,
/* 709 */ 718,
/* 710 */ -1,
/* 711 */ -1,
/* 712 */ -1,
/* 713 */ 50047,
/* 714 */ -1,
/* 715 */ 50059,
/* 716 */ -688,
/* 717 */ -1,
/* 718 */ 726,
/* 719 */ 50449,
/* 720 */ -1,
/* 721 */ -1,
/* 722 */ -1,
/* 723 */ 50059,
/* 724 */ -688,
/* 725 */ -1,
/* 726 */ 733,
/* 727 */ 50363,
/* 728 */ -1,
/* 729 */ -1,
/* 730 */ 50059,
/* 731 */ -688,
/* 732 */ -1,
/* 733 */ 740,
/* 734 */ 50364,
/* 735 */ -1,
/* 736 */ -1,
/* 737 */ 50059,
/* 738 */ -688,
/* 739 */ -1,
/* 740 */ 748,
/* 741 */ -1,
/* 742 */ 50365,
/* 743 */ -1,
/* 744 */ -1,
/* 745 */ 50059,
/* 746 */ -688,
/* 747 */ -1,
/* 748 */ 752,
/* 749 */ -1,
/* 750 */ -688,
/* 751 */ -1,
/* 752 */ 760,
/* 753 */ 50439,
/* 754 */ 50091,
/* 755 */ -1,
/* 756 */ 50093,
/* 757 */ 50059,
/* 758 */ -688,
/* 759 */ -1,
/* 760 */ 770,
/* 761 */ 50439,
/* 762 */ 50091,
/* 763 */ -1,
/* 764 */ 50044,
/* 765 */ -1,
/* 766 */ 50093,
/* 767 */ 50059,
/* 768 */ -688,
/* 769 */ -1,
/* 770 */ 782,
/* 771 */ 50439,
/* 772 */ 50091,
/* 773 */ -1,
/* 774 */ 50044,
/* 775 */ -1,
/* 776 */ 50044,
/* 777 */ -1,
/* 778 */ 50093,
/* 779 */ 50059,
/* 780 */ -688,
/* 781 */ -1,
/* 782 */ 796,
/* 783 */ 50439,
/* 784 */ 50091,
/* 785 */ -1,
/* 786 */ 50044,
/* 787 */ -1,
/* 788 */ 50044,
/* 789 */ -1,
/* 790 */ 50044,
/* 791 */ -1,
/* 792 */ 50093,
/* 793 */ 50059,
/* 794 */ -688,
/* 795 */ -1,
/* 796 */ 803,
/* 797 */ 50423,
/* 798 */ -1,
/* 799 */ -1,
/* 800 */ 50059,
/* 801 */ -688,
/* 802 */ -1,
/* 803 */ 810,
/* 804 */ 50368,
/* 805 */ -1,
/* 806 */ -1,
/* 807 */ 50059,
/* 808 */ -688,
/* 809 */ -1,
/* 810 */ 818,
/* 811 */ 50295,
/* 812 */ -1,
/* 813 */ -1,
/* 814 */ -1,
/* 815 */ 50059,
/* 816 */ -688,
/* 817 */ -1,
/* 818 */ 824,
/* 819 */ 50293,
/* 820 */ 50257,
/* 821 */ 50059,
/* 822 */ -688,
/* 823 */ -1,
/* 824 */ 830,
/* 825 */ 50293,
/* 826 */ -1,
/* 827 */ 50059,
/* 828 */ -688,
/* 829 */ -1,
/* 830 */ 837,
/* 831 */ 50322,
/* 832 */ -1,
/* 833 */ -1,
/* 834 */ 50059,
/* 835 */ -688,
/* 836 */ -1,
/* 837 */ 844,
/* 838 */ 50291,
/* 839 */ -1,
/* 840 */ -1,
/* 841 */ 50059,
/* 842 */ -688,
/* 843 */ -1,
/* 844 */ 851,
/* 845 */ 50380,
/* 846 */ -1,
/* 847 */ -1,
/* 848 */ 50059,
/* 849 */ -688,
/* 850 */ -1,
/* 851 */ 859,
/* 852 */ 50345,
/* 853 */ -1,
/* 854 */ -1,
/* 855 */ -1,
/* 856 */ 50059,
/* 857 */ -688,
/* 858 */ -1,
/* 859 */ 866,
/* 860 */ 50349,
/* 861 */ -1,
/* 862 */ -1,
/* 863 */ 50059,
/* 864 */ -688,
/* 865 */ -1,
/* 866 */ 872,
/* 867 */ -1,
/* 868 */ -1,
/* 869 */ 50059,
/* 870 */ -688,
/* 871 */ -1,
/* 872 */ 879,
/* 873 */ 50430,
/* 874 */ -1,
/* 875 */ -1,
/* 876 */ 50059,
/* 877 */ -688,
/* 878 */ -1,
/* 879 */ 887,
/* 880 */ 50434,
/* 881 */ -1,
/* 882 */ -1,
/* 883 */ -1,
/* 884 */ 50059,
/* 885 */ -688,
/* 886 */ -1,
/* 887 */ 895,
/* 888 */ 50388,
/* 889 */ -1,
/* 890 */ -1,
/* 891 */ -1,
/* 892 */ 50059,
/* 893 */ -688,
/* 894 */ -1,
/* 895 */ 902,
/* 896 */ 50436,
/* 897 */ -1,
/* 898 */ -1,
/* 899 */ 50059,
/* 900 */ -688,
/* 901 */ -1,
/* 902 */ 909,
/* 903 */ 50329,
/* 904 */ -1,
/* 905 */ -1,
/* 906 */ 50059,
/* 907 */ -688,
/* 908 */ -1,
/* 909 */ 916,
/* 910 */ 50408,
/* 911 */ -1,
/* 912 */ -1,
/* 913 */ 50059,
/* 914 */ -688,
/* 915 */ -1,
/* 916 */ 920,
/* 917 */ -1,
/* 918 */ -688,
/* 919 */ -1,
/* 920 */ 924,
/* 921 */ -1,
/* 922 */ -688,
/* 923 */ -1,
/* 924 */ 928,
/* 925 */ -1,
/* 926 */ -688,
/* 927 */ -1,
/* 928 */ 0,
/* 929 */ -1,
/* 930 */ -688,
/* 931 */ -1,
/* 932 */ 936,
/* 933 */ 50058,
/* 934 */ -932,
/* 935 */ -1,
/* 936 */ 0,
/* 937 */ -932,
/* 938 */ -1,
/* 939 */ 943,
/* 940 */ 50058,
/* 941 */ -939,
/* 942 */ -1,
/* 943 */ 0,
/* 944 */ -939,
/* 945 */ -1,
/* 946 */ 950,
/* 947 */ 50373,
/* 948 */ -946,
/* 949 */ -1,
/* 950 */ 0,
/* 951 */ 50306,
/* 952 */ -946,
/* 953 */ -1,
/* 954 */ 958,
/* 955 */ 50058,
/* 956 */ -954,
/* 957 */ -1,
/* 958 */ 0,
/* 959 */ -954,
/* 960 */ -1,
/* 961 */ 965,
/* 962 */ 50373,
/* 963 */ -961,
/* 964 */ -1,
/* 965 */ 0,
/* 966 */ 50306,
/* 967 */ -961,
/* 968 */ -1,
/* 969 */ 973,
/* 970 */ 50058,
/* 971 */ -969,
/* 972 */ -1,
/* 973 */ 0,
/* 974 */ -969,
/* 975 */ -1,
/* 976 */ 980,
/* 977 */ 50058,
/* 978 */ -976,
/* 979 */ -1,
/* 980 */ 0,
/* 981 */ -976,
/* 982 */ -1,
/* 983 */ 989,
/* 984 */ 50044,
/* 985 */ -1,
/* 986 */ -1,
/* 987 */ -983,
/* 988 */ -1,
/* 989 */ 0,
/* 990 */ -983,
/* 991 */ -1,
/* 992 */ 996,
/* 993 */ 50058,
/* 994 */ -992,
/* 995 */ -1,
/* 996 */ 0,
/* 997 */ -992,
/* 998 */ -1,
/* 999 */ 1003,
/* 1000 */ 50058,
/* 1001 */ -999,
/* 1002 */ -1,
/* 1003 */ 0,
/* 1004 */ -999,
/* 1005 */ -1,
/* 1006 */ 1010,
/* 1007 */ 50043,
/* 1008 */ -1006,
/* 1009 */ -1,
/* 1010 */ 0,
/* 1011 */ -1006,
/* 1012 */ -1,
/* 1013 */ 1017,
/* 1014 */ 50058,
/* 1015 */ -1013,
/* 1016 */ -1,
/* 1017 */ 0,
/* 1018 */ -1013,
/* 1019 */ -1,
/* 1020 */ 1024,
/* 1021 */ 50058,
/* 1022 */ -1020,
/* 1023 */ -1,
/* 1024 */ 0,
/* 1025 */ -1020,
/* 1026 */ -1,
/* 1027 */ 1033,
/* 1028 */ -1,
/* 1029 */ -1,
/* 1030 */ -1,
/* 1031 */ -1027,
/* 1032 */ -1,
/* 1033 */ 0,
/* 1034 */ -1027,
/* 1035 */ -1,
/* 1036 */ 1040,
/* 1037 */ 50044,
/* 1038 */ -1036,
/* 1039 */ -1,
/* 1040 */ 0,
/* 1041 */ -1036,
/* 1042 */ -1,
/* 1043 */ 1047,
/* 1044 */ 50058,
/* 1045 */ -1043,
/* 1046 */ -1,
/* 1047 */ 0,
/* 1048 */ -1043,
/* 1049 */ -1,
/* 1050 */ 1056,
/* 1051 */ -1,
/* 1052 */ -1,
/* 1053 */ -1,
/* 1054 */ -1050,
/* 1055 */ -1,
/* 1056 */ 0,
/* 1057 */ -1050,
/* 1058 */ -1,
/* 1059 */ 1063,
/* 1060 */ 50044,
/* 1061 */ -1059,
/* 1062 */ -1,
/* 1063 */ 0,
/* 1064 */ -1059,
/* 1065 */ -1,
/* 1066 */ 1070,
/* 1067 */ 50058,
/* 1068 */ -1066,
/* 1069 */ -1,
/* 1070 */ 0,
/* 1071 */ -1066,
/* 1072 */ -1,
/* 1073 */ 1079,
/* 1074 */ 50044,
/* 1075 */ -1,
/* 1076 */ -1,
/* 1077 */ -1073,
/* 1078 */ -1,
/* 1079 */ 0,
/* 1080 */ -1073,
/* 1081 */ -1,
/* 1082 */ 1086,
/* 1083 */ 50058,
/* 1084 */ -1082,
/* 1085 */ -1,
/* 1086 */ 0,
/* 1087 */ -1082,
/* 1088 */ -1,
/* 1089 */ 1093,
/* 1090 */ 50058,
/* 1091 */ -1089,
/* 1092 */ -1,
/* 1093 */ 0,
/* 1094 */ -1089,
/* 1095 */ -1,
/* 1096 */ 1100,
/* 1097 */ 50058,
/* 1098 */ -1096,
/* 1099 */ -1,
/* 1100 */ 0,
/* 1101 */ -1096,
/* 1102 */ -1,
/* 1103 */ 1107,
/* 1104 */ 50058,
/* 1105 */ -1103,
/* 1106 */ -1,
/* 1107 */ 0,
/* 1108 */ -1103,
/* 1109 */ -1,
/* 1110 */ 1116,
/* 1111 */ -1,
/* 1112 */ 50044,
/* 1113 */ -1,
/* 1114 */ -1110,
/* 1115 */ -1,
/* 1116 */ 0,
/* 1117 */ -1110,
/* 1118 */ -1,
/* 1119 */ 1123,
/* 1120 */ 50058,
/* 1121 */ -1119,
/* 1122 */ -1,
/* 1123 */ 0,
/* 1124 */ -1119,
/* 1125 */ -1,
/* 1126 */ 1131,
/* 1127 */ -1,
/* 1128 */ -1,
/* 1129 */ -1126,
/* 1130 */ -1,
/* 1131 */ 0,
/* 1132 */ -1126,
/* 1133 */ -1,
/* 1134 */ 1140,
/* 1135 */ 50044,
/* 1136 */ -1,
/* 1137 */ -1,
/* 1138 */ -1134,
/* 1139 */ -1,
/* 1140 */ 0,
/* 1141 */ -1134,
/* 1142 */ -1,
/* 1143 */ 1147,
/* 1144 */ 50058,
/* 1145 */ -1143,
/* 1146 */ -1,
/* 1147 */ 0,
/* 1148 */ -1143,
/* 1149 */ -1,
/* 1150 */ 1154,
/* 1151 */ 50058,
/* 1152 */ -1150,
/* 1153 */ -1,
/* 1154 */ 0,
/* 1155 */ -1150,
/* 1156 */ -1,
/* 1157 */ 1161,
/* 1158 */ 50037,
/* 1159 */ -1157,
/* 1160 */ -1,
/* 1161 */ 0,
/* 1162 */ -1157,
/* 1163 */ -1,
/* 1164 */ 1168,
/* 1165 */ 50058,
/* 1166 */ -1164,
/* 1167 */ -1,
/* 1168 */ 0,
/* 1169 */ -1164,
/* 1170 */ -1,
/* 1171 */ 1175,
/* 1172 */ 50037,
/* 1173 */ -1171,
/* 1174 */ -1,
/* 1175 */ 0,
/* 1176 */ -1171,
/* 1177 */ -1,
/* 1178 */ 1182,
/* 1179 */ 50058,
/* 1180 */ -1178,
/* 1181 */ -1,
/* 1182 */ 0,
/* 1183 */ -1178,
/* 1184 */ -1,
/* 1185 */ 1191,
/* 1186 */ -1,
/* 1187 */ -1,
/* 1188 */ -1,
/* 1189 */ -1185,
/* 1190 */ -1,
/* 1191 */ 0,
/* 1192 */ -1185,
/* 1193 */ -1,
/* 1194 */ 1198,
/* 1195 */ 50044,
/* 1196 */ -1194,
/* 1197 */ -1,
/* 1198 */ 0,
/* 1199 */ -1194,
/* 1200 */ -1,
/* 1201 */ 1205,
/* 1202 */ 50058,
/* 1203 */ -1201,
/* 1204 */ -1,
/* 1205 */ 0,
/* 1206 */ -1201,
/* 1207 */ -1,
/* 1208 */ 1214,
/* 1209 */ -1,
/* 1210 */ -1,
/* 1211 */ -1,
/* 1212 */ -1208,
/* 1213 */ -1,
/* 1214 */ 0,
/* 1215 */ -1208,
/* 1216 */ -1,
/* 1217 */ 1221,
/* 1218 */ 50044,
/* 1219 */ -1217,
/* 1220 */ -1,
/* 1221 */ 0,
/* 1222 */ -1217,
/* 1223 */ -1,
/* 1224 */ 1228,
/* 1225 */ 50058,
/* 1226 */ -1224,
/* 1227 */ -1,
/* 1228 */ 0,
/* 1229 */ -1224,
/* 1230 */ -1,
/* 1231 */ 1237,
/* 1232 */ -1,
/* 1233 */ -1,
/* 1234 */ -1,
/* 1235 */ -1231,
/* 1236 */ -1,
/* 1237 */ 0,
/* 1238 */ -1231,
/* 1239 */ -1,
/* 1240 */ 1244,
/* 1241 */ 50044,
/* 1242 */ -1240,
/* 1243 */ -1,
/* 1244 */ 0,
/* 1245 */ -1240,
/* 1246 */ -1,
/* 1247 */ 1251,
/* 1248 */ 50274,
/* 1249 */ -1247,
/* 1250 */ -1,
/* 1251 */ 1256,
/* 1252 */ 50302,
/* 1253 */ 50274,
/* 1254 */ -1247,
/* 1255 */ -1,
/* 1256 */ 1261,
/* 1257 */ 50375,
/* 1258 */ 50274,
/* 1259 */ -1247,
/* 1260 */ -1,
/* 1261 */ 0,
/* 1262 */ 50319,
/* 1263 */ 50274,
/* 1264 */ -1247,
/* 1265 */ -1,
/* 1266 */ 1276,
/* 1267 */ -1,
/* 1268 */ 50352,
/* 1269 */ -1,
/* 1270 */ -1,
/* 1271 */ -1,
/* 1272 */ -1,
/* 1273 */ -1,
/* 1274 */ -1266,
/* 1275 */ -1,
/* 1276 */ 0,
/* 1277 */ -1,
/* 1278 */ 50352,
/* 1279 */ -1,
/* 1280 */ -1,
/* 1281 */ -1266,
/* 1282 */ -1,
/* 1283 */ 1287,
/* 1284 */ -1,
/* 1285 */ -1283,
/* 1286 */ -1,
/* 1287 */ 1292,
/* 1288 */ -1,
/* 1289 */ 50037,
/* 1290 */ -1283,
/* 1291 */ -1,
/* 1292 */ 0,
/* 1293 */ -1283,
/* 1294 */ -1,
/* 1295 */ 1299,
/* 1296 */ -1,
/* 1297 */ -1295,
/* 1298 */ -1,
/* 1299 */ 0,
/* 1300 */ -1295,
/* 1301 */ -1,
/* 1302 */ 1306,
/* 1303 */ 50318,
/* 1304 */ -1302,
/* 1305 */ -1,
/* 1306 */ 0,
/* 1307 */ -1302,
/* 1308 */ -1,
/* 1309 */ 1316,
/* 1310 */ 50040,
/* 1311 */ -1,
/* 1312 */ -1,
/* 1313 */ 50041,
/* 1314 */ -1309,
/* 1315 */ -1,
/* 1316 */ 0,
/* 1317 */ -1309,
/* 1318 */ -1,
/* 1319 */ 1323,
/* 1320 */ 50321,
/* 1321 */ -1319,
/* 1322 */ -1,
/* 1323 */ 1327,
/* 1324 */ 50306,
/* 1325 */ -1319,
/* 1326 */ -1,
/* 1327 */ 0,
/* 1328 */ 50373,
/* 1329 */ -1319,
/* 1330 */ -1,
/* 1331 */ 1340,
/* 1332 */ 50287,
/* 1333 */ -1,
/* 1334 */ -1,
/* 1335 */ -1,
/* 1336 */ -1,
/* 1337 */ -1,
/* 1338 */ -1331,
/* 1339 */ -1,
/* 1340 */ 0,
/* 1341 */ -1331,
/* 1342 */ -1,
/* 1343 */ 1347,
/* 1344 */ -1,
/* 1345 */ -1343,
/* 1346 */ -1,
/* 1347 */ 0,
/* 1348 */ -1343,
/* 1349 */ -1,
/* 1350 */ 1354,
/* 1351 */ -1,
/* 1352 */ -1350,
/* 1353 */ -1,
/* 1354 */ 0,
/* 1355 */ -1350,
/* 1356 */ -1,
/* 1357 */ 1361,
/* 1358 */ 50318,
/* 1359 */ -1357,
/* 1360 */ -1,
/* 1361 */ 0,
/* 1362 */ -1357,
/* 1363 */ -1,
/* 1364 */ 1371,
/* 1365 */ 50040,
/* 1366 */ -1,
/* 1367 */ -1,
/* 1368 */ 50041,
/* 1369 */ -1364,
/* 1370 */ -1,
/* 1371 */ 0,
/* 1372 */ -1364,
/* 1373 */ -1,
/* 1374 */ 1378,
/* 1375 */ 50321,
/* 1376 */ -1374,
/* 1377 */ -1,
/* 1378 */ 1382,
/* 1379 */ 50306,
/* 1380 */ -1374,
/* 1381 */ -1,
/* 1382 */ 0,
/* 1383 */ 50373,
/* 1384 */ -1374,
/* 1385 */ -1,
/* 1386 */ 1390,
/* 1387 */ -1,
/* 1388 */ -1386,
/* 1389 */ -1,
/* 1390 */ 0,
/* 1391 */ -1386,
/* 1392 */ -1,
/* 1393 */ 1398,
/* 1394 */ -1,
/* 1395 */ -1,
/* 1396 */ -1393,
/* 1397 */ -1,
/* 1398 */ 0,
/* 1399 */ 50126,
/* 1400 */ -1,
/* 1401 */ -1,
/* 1402 */ -1393,
/* 1403 */ -1,
/* 1404 */ 1408,
/* 1405 */ 50044,
/* 1406 */ -1404,
/* 1407 */ -1,
/* 1408 */ 0,
/* 1409 */ -1404,
/* 1410 */ -1,
/* 1411 */ 1415,
/* 1412 */ 50044,
/* 1413 */ -1411,
/* 1414 */ -1,
/* 1415 */ 0,
/* 1416 */ -1411,
/* 1417 */ -1,
/* 1418 */ 0,
/* 1419 */ 50276,
/* 1420 */ -1,
/* 1421 */ -1,
/* 1422 */ -1418,
/* 1423 */ -1,
/* 1424 */ 1428,
/* 1425 */ 50058,
/* 1426 */ -1424,
/* 1427 */ -1,
/* 1428 */ 0,
/* 1429 */ -1424,
/* 1430 */ -1,
/* 1431 */ 0,
/* 1432 */ 50371,
/* 1433 */ 50257,
/* 1434 */ 50058,
/* 1435 */ -1,
/* 1436 */ 50123,
/* 1437 */ -1,
/* 1438 */ 50125,
/* 1439 */ -1431,
/* 1440 */ -1,
/* 1441 */ 1446,
/* 1442 */ -1,
/* 1443 */ -1,
/* 1444 */ -1441,
/* 1445 */ -1,
/* 1446 */ 0,
/* 1447 */ -1441,
/* 1448 */ -1,
/* 1449 */ 1456,
/* 1450 */ 50325,
/* 1451 */ -1,
/* 1452 */ 50257,
/* 1453 */ 50059,
/* 1454 */ -1449,
/* 1455 */ -1,
/* 1456 */ 1463,
/* 1457 */ 50373,
/* 1458 */ -1,
/* 1459 */ -1,
/* 1460 */ 50059,
/* 1461 */ -1449,
/* 1462 */ -1,
/* 1463 */ 1467,
/* 1464 */ -1,
/* 1465 */ -1449,
/* 1466 */ -1,
/* 1467 */ 1474,
/* 1468 */ 50429,
/* 1469 */ -1,
/* 1470 */ -1,
/* 1471 */ 50059,
/* 1472 */ -1449,
/* 1473 */ -1,
/* 1474 */ 1481,
/* 1475 */ 50372,
/* 1476 */ -1,
/* 1477 */ -1,
/* 1478 */ 50059,
/* 1479 */ -1449,
/* 1480 */ -1,
/* 1481 */ 1488,
/* 1482 */ 50315,
/* 1483 */ -1,
/* 1484 */ -1,
/* 1485 */ 50059,
/* 1486 */ -1449,
/* 1487 */ -1,
/* 1488 */ 1495,
/* 1489 */ 50451,
/* 1490 */ -1,
/* 1491 */ -1,
/* 1492 */ 50059,
/* 1493 */ -1449,
/* 1494 */ -1,
/* 1495 */ 1502,
/* 1496 */ 50283,
/* 1497 */ -1,
/* 1498 */ -1,
/* 1499 */ 50059,
/* 1500 */ -1449,
/* 1501 */ -1,
/* 1502 */ 1509,
/* 1503 */ 50434,
/* 1504 */ -1,
/* 1505 */ -1,
/* 1506 */ 50059,
/* 1507 */ -1449,
/* 1508 */ -1,
/* 1509 */ 1517,
/* 1510 */ 50434,
/* 1511 */ -1,
/* 1512 */ -1,
/* 1513 */ 50037,
/* 1514 */ 50059,
/* 1515 */ -1449,
/* 1516 */ -1,
/* 1517 */ 1526,
/* 1518 */ 50434,
/* 1519 */ -1,
/* 1520 */ 50043,
/* 1521 */ -1,
/* 1522 */ 50037,
/* 1523 */ 50059,
/* 1524 */ -1449,
/* 1525 */ -1,
/* 1526 */ 1533,
/* 1527 */ 50403,
/* 1528 */ -1,
/* 1529 */ -1,
/* 1530 */ 50059,
/* 1531 */ -1449,
/* 1532 */ -1,
/* 1533 */ 1540,
/* 1534 */ 50419,
/* 1535 */ -1,
/* 1536 */ -1,
/* 1537 */ 50059,
/* 1538 */ -1449,
/* 1539 */ -1,
/* 1540 */ 1547,
/* 1541 */ 50382,
/* 1542 */ -1,
/* 1543 */ -1,
/* 1544 */ 50059,
/* 1545 */ -1449,
/* 1546 */ -1,
/* 1547 */ 1555,
/* 1548 */ 50314,
/* 1549 */ -1,
/* 1550 */ -1,
/* 1551 */ -1,
/* 1552 */ 50059,
/* 1553 */ -1449,
/* 1554 */ -1,
/* 1555 */ 1562,
/* 1556 */ 50465,
/* 1557 */ -1,
/* 1558 */ -1,
/* 1559 */ 50059,
/* 1560 */ -1449,
/* 1561 */ -1,
/* 1562 */ 1569,
/* 1563 */ 50430,
/* 1564 */ -1,
/* 1565 */ -1,
/* 1566 */ 50059,
/* 1567 */ -1449,
/* 1568 */ -1,
/* 1569 */ 1576,
/* 1570 */ 50395,
/* 1571 */ -1,
/* 1572 */ -1,
/* 1573 */ 50059,
/* 1574 */ -1449,
/* 1575 */ -1,
/* 1576 */ 1583,
/* 1577 */ 50322,
/* 1578 */ -1,
/* 1579 */ -1,
/* 1580 */ 50059,
/* 1581 */ -1449,
/* 1582 */ -1,
/* 1583 */ 1590,
/* 1584 */ 50311,
/* 1585 */ -1,
/* 1586 */ -1,
/* 1587 */ 50059,
/* 1588 */ -1449,
/* 1589 */ -1,
/* 1590 */ 1597,
/* 1591 */ 50406,
/* 1592 */ -1,
/* 1593 */ -1,
/* 1594 */ 50059,
/* 1595 */ -1449,
/* 1596 */ -1,
/* 1597 */ 1604,
/* 1598 */ 50291,
/* 1599 */ -1,
/* 1600 */ -1,
/* 1601 */ 50059,
/* 1602 */ -1449,
/* 1603 */ -1,
/* 1604 */ 1611,
/* 1605 */ 50364,
/* 1606 */ -1,
/* 1607 */ -1,
/* 1608 */ 50059,
/* 1609 */ -1449,
/* 1610 */ -1,
/* 1611 */ 1618,
/* 1612 */ 50436,
/* 1613 */ -1,
/* 1614 */ -1,
/* 1615 */ 50059,
/* 1616 */ -1449,
/* 1617 */ -1,
/* 1618 */ 1625,
/* 1619 */ 50348,
/* 1620 */ -1,
/* 1621 */ -1,
/* 1622 */ 50059,
/* 1623 */ -1449,
/* 1624 */ -1,
/* 1625 */ 1632,
/* 1626 */ 50349,
/* 1627 */ -1,
/* 1628 */ -1,
/* 1629 */ 50059,
/* 1630 */ -1449,
/* 1631 */ -1,
/* 1632 */ 1639,
/* 1633 */ 50360,
/* 1634 */ -1,
/* 1635 */ -1,
/* 1636 */ 50059,
/* 1637 */ -1449,
/* 1638 */ -1,
/* 1639 */ 1646,
/* 1640 */ 50303,
/* 1641 */ -1,
/* 1642 */ -1,
/* 1643 */ 50059,
/* 1644 */ -1449,
/* 1645 */ -1,
/* 1646 */ 1653,
/* 1647 */ 50467,
/* 1648 */ -1,
/* 1649 */ -1,
/* 1650 */ 50059,
/* 1651 */ -1449,
/* 1652 */ -1,
/* 1653 */ 1660,
/* 1654 */ 50466,
/* 1655 */ -1,
/* 1656 */ -1,
/* 1657 */ 50059,
/* 1658 */ -1449,
/* 1659 */ -1,
/* 1660 */ 1667,
/* 1661 */ 50378,
/* 1662 */ -1,
/* 1663 */ -1,
/* 1664 */ 50059,
/* 1665 */ -1449,
/* 1666 */ -1,
/* 1667 */ 1674,
/* 1668 */ 50455,
/* 1669 */ -1,
/* 1670 */ -1,
/* 1671 */ 50059,
/* 1672 */ -1449,
/* 1673 */ -1,
/* 1674 */ 1681,
/* 1675 */ 50312,
/* 1676 */ -1,
/* 1677 */ -1,
/* 1678 */ 50059,
/* 1679 */ -1449,
/* 1680 */ -1,
/* 1681 */ 1689,
/* 1682 */ 50375,
/* 1683 */ 50419,
/* 1684 */ -1,
/* 1685 */ -1,
/* 1686 */ 50059,
/* 1687 */ -1449,
/* 1688 */ -1,
/* 1689 */ 1696,
/* 1690 */ 50374,
/* 1691 */ -1,
/* 1692 */ -1,
/* 1693 */ 50059,
/* 1694 */ -1449,
/* 1695 */ -1,
/* 1696 */ 1703,
/* 1697 */ 50354,
/* 1698 */ -1,
/* 1699 */ -1,
/* 1700 */ 50059,
/* 1701 */ -1449,
/* 1702 */ -1,
/* 1703 */ 1707,
/* 1704 */ -1,
/* 1705 */ -1449,
/* 1706 */ -1,
/* 1707 */ 1711,
/* 1708 */ -1,
/* 1709 */ -1449,
/* 1710 */ -1,
/* 1711 */ 0,
/* 1712 */ -1,
/* 1713 */ -1449,
/* 1714 */ -1,
/* 1715 */ 1719,
/* 1716 */ 50058,
/* 1717 */ -1715,
/* 1718 */ -1,
/* 1719 */ 0,
/* 1720 */ -1715,
/* 1721 */ -1,
/* 1722 */ 1726,
/* 1723 */ 50058,
/* 1724 */ -1722,
/* 1725 */ -1,
/* 1726 */ 0,
/* 1727 */ -1722,
/* 1728 */ -1,
/* 1729 */ 1733,
/* 1730 */ 50058,
/* 1731 */ -1729,
/* 1732 */ -1,
/* 1733 */ 0,
/* 1734 */ -1729,
/* 1735 */ -1,
/* 1736 */ 1740,
/* 1737 */ 50058,
/* 1738 */ -1736,
/* 1739 */ -1,
/* 1740 */ 0,
/* 1741 */ -1736,
/* 1742 */ -1,
/* 1743 */ 1747,
/* 1744 */ 50058,
/* 1745 */ -1743,
/* 1746 */ -1,
/* 1747 */ 0,
/* 1748 */ -1743,
/* 1749 */ -1,
/* 1750 */ 1754,
/* 1751 */ 50058,
/* 1752 */ -1750,
/* 1753 */ -1,
/* 1754 */ 0,
/* 1755 */ -1750,
/* 1756 */ -1,
/* 1757 */ 1761,
/* 1758 */ 50058,
/* 1759 */ -1757,
/* 1760 */ -1,
/* 1761 */ 0,
/* 1762 */ -1757,
/* 1763 */ -1,
/* 1764 */ 1768,
/* 1765 */ 50058,
/* 1766 */ -1764,
/* 1767 */ -1,
/* 1768 */ 0,
/* 1769 */ -1764,
/* 1770 */ -1,
/* 1771 */ 1775,
/* 1772 */ 50058,
/* 1773 */ -1771,
/* 1774 */ -1,
/* 1775 */ 0,
/* 1776 */ -1771,
/* 1777 */ -1,
/* 1778 */ 1782,
/* 1779 */ 50058,
/* 1780 */ -1778,
/* 1781 */ -1,
/* 1782 */ 0,
/* 1783 */ -1778,
/* 1784 */ -1,
/* 1785 */ 1789,
/* 1786 */ 50058,
/* 1787 */ -1785,
/* 1788 */ -1,
/* 1789 */ 0,
/* 1790 */ -1785,
/* 1791 */ -1,
/* 1792 */ 1796,
/* 1793 */ 50058,
/* 1794 */ -1792,
/* 1795 */ -1,
/* 1796 */ 0,
/* 1797 */ -1792,
/* 1798 */ -1,
/* 1799 */ 1803,
/* 1800 */ 50058,
/* 1801 */ -1799,
/* 1802 */ -1,
/* 1803 */ 0,
/* 1804 */ -1799,
/* 1805 */ -1,
/* 1806 */ 1810,
/* 1807 */ 50058,
/* 1808 */ -1806,
/* 1809 */ -1,
/* 1810 */ 0,
/* 1811 */ -1806,
/* 1812 */ -1,
/* 1813 */ 1817,
/* 1814 */ -1,
/* 1815 */ -1813,
/* 1816 */ -1,
/* 1817 */ 0,
/* 1818 */ -1813,
/* 1819 */ -1,
/* 1820 */ 1824,
/* 1821 */ 50058,
/* 1822 */ -1820,
/* 1823 */ -1,
/* 1824 */ 0,
/* 1825 */ -1820,
/* 1826 */ -1,
/* 1827 */ 1831,
/* 1828 */ 50058,
/* 1829 */ -1827,
/* 1830 */ -1,
/* 1831 */ 0,
/* 1832 */ -1827,
/* 1833 */ -1,
/* 1834 */ 1838,
/* 1835 */ 50058,
/* 1836 */ -1834,
/* 1837 */ -1,
/* 1838 */ 0,
/* 1839 */ -1834,
/* 1840 */ -1,
/* 1841 */ 1845,
/* 1842 */ 50058,
/* 1843 */ -1841,
/* 1844 */ -1,
/* 1845 */ 0,
/* 1846 */ -1841,
/* 1847 */ -1,
/* 1848 */ 1852,
/* 1849 */ 50058,
/* 1850 */ -1848,
/* 1851 */ -1,
/* 1852 */ 0,
/* 1853 */ -1848,
/* 1854 */ -1,
/* 1855 */ 1859,
/* 1856 */ 50058,
/* 1857 */ -1855,
/* 1858 */ -1,
/* 1859 */ 0,
/* 1860 */ -1855,
/* 1861 */ -1,
/* 1862 */ 1866,
/* 1863 */ 50058,
/* 1864 */ -1862,
/* 1865 */ -1,
/* 1866 */ 0,
/* 1867 */ -1862,
/* 1868 */ -1,
/* 1869 */ 1874,
/* 1870 */ -1,
/* 1871 */ -1,
/* 1872 */ -1869,
/* 1873 */ -1,
/* 1874 */ 0,
/* 1875 */ -1869,
/* 1876 */ -1,
/* 1877 */ 1881,
/* 1878 */ 50058,
/* 1879 */ -1877,
/* 1880 */ -1,
/* 1881 */ 0,
/* 1882 */ -1877,
/* 1883 */ -1,
/* 1884 */ 1888,
/* 1885 */ 50058,
/* 1886 */ -1884,
/* 1887 */ -1,
/* 1888 */ 0,
/* 1889 */ -1884,
/* 1890 */ -1,
/* 1891 */ 1897,
/* 1892 */ -1,
/* 1893 */ -1,
/* 1894 */ -1,
/* 1895 */ -1891,
/* 1896 */ -1,
/* 1897 */ 0,
/* 1898 */ -1891,
/* 1899 */ -1,
/* 1900 */ 1904,
/* 1901 */ 50044,
/* 1902 */ -1900,
/* 1903 */ -1,
/* 1904 */ 0,
/* 1905 */ -1900,
/* 1906 */ -1,
/* 1907 */ 1911,
/* 1908 */ 50058,
/* 1909 */ -1907,
/* 1910 */ -1,
/* 1911 */ 0,
/* 1912 */ -1907,
/* 1913 */ -1,
/* 1914 */ 1920,
/* 1915 */ -1,
/* 1916 */ -1,
/* 1917 */ -1,
/* 1918 */ -1914,
/* 1919 */ -1,
/* 1920 */ 0,
/* 1921 */ -1914,
/* 1922 */ -1,
/* 1923 */ 1927,
/* 1924 */ 50044,
/* 1925 */ -1923,
/* 1926 */ -1,
/* 1927 */ 0,
/* 1928 */ -1923,
/* 1929 */ -1,
/* 1930 */ 1934,
/* 1931 */ 50058,
/* 1932 */ -1930,
/* 1933 */ -1,
/* 1934 */ 0,
/* 1935 */ -1930,
/* 1936 */ -1,
/* 1937 */ 1942,
/* 1938 */ -1,
/* 1939 */ -1,
/* 1940 */ -1937,
/* 1941 */ -1,
/* 1942 */ 0,
/* 1943 */ -1937,
/* 1944 */ -1,
/* 1945 */ 1949,
/* 1946 */ 50058,
/* 1947 */ -1945,
/* 1948 */ -1,
/* 1949 */ 0,
/* 1950 */ -1945,
/* 1951 */ -1,
/* 1952 */ 1958,
/* 1953 */ -1,
/* 1954 */ -1,
/* 1955 */ -1,
/* 1956 */ -1952,
/* 1957 */ -1,
/* 1958 */ 0,
/* 1959 */ -1952,
/* 1960 */ -1,
/* 1961 */ 1965,
/* 1962 */ 50044,
/* 1963 */ -1961,
/* 1964 */ -1,
/* 1965 */ 1969,
/* 1966 */ 50124,
/* 1967 */ -1961,
/* 1968 */ -1,
/* 1969 */ 0,
/* 1970 */ -1961,
/* 1971 */ -1,
/* 1972 */ 1976,
/* 1973 */ 50058,
/* 1974 */ -1972,
/* 1975 */ -1,
/* 1976 */ 0,
/* 1977 */ -1972,
/* 1978 */ -1,
/* 1979 */ 1983,
/* 1980 */ 50058,
/* 1981 */ -1979,
/* 1982 */ -1,
/* 1983 */ 0,
/* 1984 */ -1979,
/* 1985 */ -1,
/* 1986 */ 1990,
/* 1987 */ 50058,
/* 1988 */ -1986,
/* 1989 */ -1,
/* 1990 */ 0,
/* 1991 */ -1986,
/* 1992 */ -1,
/* 1993 */ 1997,
/* 1994 */ 50058,
/* 1995 */ -1993,
/* 1996 */ -1,
/* 1997 */ 0,
/* 1998 */ -1993,
/* 1999 */ -1,
/* 2000 */ 2004,
/* 2001 */ 50058,
/* 2002 */ -2000,
/* 2003 */ -1,
/* 2004 */ 0,
/* 2005 */ -2000,
/* 2006 */ -1,
/* 2007 */ 2011,
/* 2008 */ 50058,
/* 2009 */ -2007,
/* 2010 */ -1,
/* 2011 */ 0,
/* 2012 */ -2007,
/* 2013 */ -1,
/* 2014 */ 2018,
/* 2015 */ 50058,
/* 2016 */ -2014,
/* 2017 */ -1,
/* 2018 */ 0,
/* 2019 */ -2014,
/* 2020 */ -1,
/* 2021 */ 2025,
/* 2022 */ 50058,
/* 2023 */ -2021,
/* 2024 */ -1,
/* 2025 */ 0,
/* 2026 */ -2021,
/* 2027 */ -1,
/* 2028 */ 2032,
/* 2029 */ 50058,
/* 2030 */ -2028,
/* 2031 */ -1,
/* 2032 */ 0,
/* 2033 */ -2028,
/* 2034 */ -1,
/* 2035 */ 2040,
/* 2036 */ -1,
/* 2037 */ -1,
/* 2038 */ -2035,
/* 2039 */ -1,
/* 2040 */ 0,
/* 2041 */ 50275,
/* 2042 */ -1,
/* 2043 */ -1,
/* 2044 */ -2035,
/* 2045 */ -1,
/* 2046 */ 2050,
/* 2047 */ 50044,
/* 2048 */ -2046,
/* 2049 */ -1,
/* 2050 */ 0,
/* 2051 */ -2046,
/* 2052 */ -1,
/* 2053 */ 2057,
/* 2054 */ 50044,
/* 2055 */ -2053,
/* 2056 */ -1,
/* 2057 */ 0,
/* 2058 */ -2053,
/* 2059 */ -1,
/* 2060 */ 2065,
/* 2061 */ -1,
/* 2062 */ -1,
/* 2063 */ -2060,
/* 2064 */ -1,
/* 2065 */ 0,
/* 2066 */ 50126,
/* 2067 */ -1,
/* 2068 */ -1,
/* 2069 */ -2060,
/* 2070 */ -1,
/* 2071 */ 2075,
/* 2072 */ 50044,
/* 2073 */ -2071,
/* 2074 */ -1,
/* 2075 */ 0,
/* 2076 */ -2071,
/* 2077 */ -1,
/* 2078 */ 2082,
/* 2079 */ 50044,
/* 2080 */ -2078,
/* 2081 */ -1,
/* 2082 */ 0,
/* 2083 */ -2078,
/* 2084 */ -1,
/* 2085 */ 0,
/* 2086 */ 50347,
/* 2087 */ 50257,
/* 2088 */ 50058,
/* 2089 */ -1,
/* 2090 */ 50123,
/* 2091 */ -1,
/* 2092 */ 50125,
/* 2093 */ -2085,
/* 2094 */ -1,
/* 2095 */ 2100,
/* 2096 */ -1,
/* 2097 */ -1,
/* 2098 */ -2095,
/* 2099 */ -1,
/* 2100 */ 0,
/* 2101 */ -2095,
/* 2102 */ -1,
/* 2103 */ 2110,
/* 2104 */ 50325,
/* 2105 */ -1,
/* 2106 */ 50257,
/* 2107 */ 50059,
/* 2108 */ -2103,
/* 2109 */ -1,
/* 2110 */ 2117,
/* 2111 */ 50382,
/* 2112 */ -1,
/* 2113 */ -1,
/* 2114 */ 50059,
/* 2115 */ -2103,
/* 2116 */ -1,
/* 2117 */ 2121,
/* 2118 */ -1,
/* 2119 */ -2103,
/* 2120 */ -1,
/* 2121 */ 2129,
/* 2122 */ 50388,
/* 2123 */ -1,
/* 2124 */ -1,
/* 2125 */ 50037,
/* 2126 */ 50059,
/* 2127 */ -2103,
/* 2128 */ -1,
/* 2129 */ 2136,
/* 2130 */ 50364,
/* 2131 */ -1,
/* 2132 */ -1,
/* 2133 */ 50059,
/* 2134 */ -2103,
/* 2135 */ -1,
/* 2136 */ 2143,
/* 2137 */ 50349,
/* 2138 */ -1,
/* 2139 */ -1,
/* 2140 */ 50059,
/* 2141 */ -2103,
/* 2142 */ -1,
/* 2143 */ 2150,
/* 2144 */ -1,
/* 2145 */ -1,
/* 2146 */ -1,
/* 2147 */ 50059,
/* 2148 */ -2103,
/* 2149 */ -1,
/* 2150 */ 2157,
/* 2151 */ 50471,
/* 2152 */ -1,
/* 2153 */ -1,
/* 2154 */ 50059,
/* 2155 */ -2103,
/* 2156 */ -1,
/* 2157 */ 2164,
/* 2158 */ 50343,
/* 2159 */ -1,
/* 2160 */ -1,
/* 2161 */ 50059,
/* 2162 */ -2103,
/* 2163 */ -1,
/* 2164 */ 2168,
/* 2165 */ -1,
/* 2166 */ -2103,
/* 2167 */ -1,
/* 2168 */ 2172,
/* 2169 */ -1,
/* 2170 */ -2103,
/* 2171 */ -1,
/* 2172 */ 0,
/* 2173 */ -1,
/* 2174 */ -2103,
/* 2175 */ -1,
/* 2176 */ 2180,
/* 2177 */ 50058,
/* 2178 */ -2176,
/* 2179 */ -1,
/* 2180 */ 0,
/* 2181 */ -2176,
/* 2182 */ -1,
/* 2183 */ 2187,
/* 2184 */ 50058,
/* 2185 */ -2183,
/* 2186 */ -1,
/* 2187 */ 0,
/* 2188 */ -2183,
/* 2189 */ -1,
/* 2190 */ 2194,
/* 2191 */ 50058,
/* 2192 */ -2190,
/* 2193 */ -1,
/* 2194 */ 0,
/* 2195 */ -2190,
/* 2196 */ -1,
/* 2197 */ 2201,
/* 2198 */ 50058,
/* 2199 */ -2197,
/* 2200 */ -1,
/* 2201 */ 0,
/* 2202 */ -2197,
/* 2203 */ -1,
/* 2204 */ 2208,
/* 2205 */ 50058,
/* 2206 */ -2204,
/* 2207 */ -1,
/* 2208 */ 0,
/* 2209 */ -2204,
/* 2210 */ -1,
/* 2211 */ 2217,
/* 2212 */ -1,
/* 2213 */ -1,
/* 2214 */ -1,
/* 2215 */ -2211,
/* 2216 */ -1,
/* 2217 */ 0,
/* 2218 */ -2211,
/* 2219 */ -1,
/* 2220 */ 2224,
/* 2221 */ 50124,
/* 2222 */ -2220,
/* 2223 */ -1,
/* 2224 */ 2228,
/* 2225 */ 50044,
/* 2226 */ -2220,
/* 2227 */ -1,
/* 2228 */ 0,
/* 2229 */ -2220,
/* 2230 */ -1,
/* 2231 */ 2235,
/* 2232 */ 50447,
/* 2233 */ -2231,
/* 2234 */ -1,
/* 2235 */ 0,
/* 2236 */ 50333,
/* 2237 */ -2231,
/* 2238 */ -1,
/* 2239 */ 2243,
/* 2240 */ 50058,
/* 2241 */ -2239,
/* 2242 */ -1,
/* 2243 */ 0,
/* 2244 */ -2239,
/* 2245 */ -1,
/* 2246 */ 2250,
/* 2247 */ 50058,
/* 2248 */ -2246,
/* 2249 */ -1,
/* 2250 */ 0,
/* 2251 */ -2246,
/* 2252 */ -1,
/* 2253 */ 2257,
/* 2254 */ 50058,
/* 2255 */ -2253,
/* 2256 */ -1,
/* 2257 */ 0,
/* 2258 */ -2253,
/* 2259 */ -1,
/* 2260 */ 0,
/* 2261 */ -1,
/* 2262 */ -1,
/* 2263 */ 50257,
/* 2264 */ 50058,
/* 2265 */ -1,
/* 2266 */ 50123,
/* 2267 */ -1,
/* 2268 */ 50125,
/* 2269 */ -1,
/* 2270 */ -2260,
/* 2271 */ -1,
/* 2272 */ 2278,
/* 2273 */ -1,
/* 2274 */ -1,
/* 2275 */ -1,
/* 2276 */ -2272,
/* 2277 */ -1,
/* 2278 */ 0,
/* 2279 */ -2272,
/* 2280 */ -1,
/* 2281 */ 2285,
/* 2282 */ 50047,
/* 2283 */ -2281,
/* 2284 */ -1,
/* 2285 */ 0,
/* 2286 */ -2281,
/* 2287 */ -1,
/* 2288 */ 2292,
/* 2289 */ 50435,
/* 2290 */ -2288,
/* 2291 */ -1,
/* 2292 */ 2296,
/* 2293 */ 50333,
/* 2294 */ -2288,
/* 2295 */ -1,
/* 2296 */ 2300,
/* 2297 */ 50408,
/* 2298 */ -2288,
/* 2299 */ -1,
/* 2300 */ 0,
/* 2301 */ 50329,
/* 2302 */ -2288,
/* 2303 */ -1,
/* 2304 */ 2309,
/* 2305 */ -1,
/* 2306 */ -1,
/* 2307 */ -2304,
/* 2308 */ -1,
/* 2309 */ 0,
/* 2310 */ -2304,
/* 2311 */ -1,
/* 2312 */ 2321,
/* 2313 */ 50287,
/* 2314 */ -1,
/* 2315 */ 50123,
/* 2316 */ -1,
/* 2317 */ 50125,
/* 2318 */ -1,
/* 2319 */ -2312,
/* 2320 */ -1,
/* 2321 */ 0,
/* 2322 */ -2312,
/* 2323 */ -1,
/* 2324 */ 2329,
/* 2325 */ -1,
/* 2326 */ -1,
/* 2327 */ -2324,
/* 2328 */ -1,
/* 2329 */ 0,
/* 2330 */ -2324,
/* 2331 */ -1,
/* 2332 */ 2339,
/* 2333 */ 50325,
/* 2334 */ -1,
/* 2335 */ 50257,
/* 2336 */ 50059,
/* 2337 */ -2332,
/* 2338 */ -1,
/* 2339 */ 2344,
/* 2340 */ -1,
/* 2341 */ 50059,
/* 2342 */ -2332,
/* 2343 */ -1,
/* 2344 */ 2351,
/* 2345 */ 50373,
/* 2346 */ -1,
/* 2347 */ -1,
/* 2348 */ 50059,
/* 2349 */ -2332,
/* 2350 */ -1,
/* 2351 */ 2358,
/* 2352 */ 50314,
/* 2353 */ -1,
/* 2354 */ -1,
/* 2355 */ 50059,
/* 2356 */ -2332,
/* 2357 */ -1,
/* 2358 */ 2367,
/* 2359 */ 50297,
/* 2360 */ -1,
/* 2361 */ -1,
/* 2362 */ -1,
/* 2363 */ -1,
/* 2364 */ 50059,
/* 2365 */ -2332,
/* 2366 */ -1,
/* 2367 */ 2375,
/* 2368 */ 50412,
/* 2369 */ -1,
/* 2370 */ -1,
/* 2371 */ -1,
/* 2372 */ 50059,
/* 2373 */ -2332,
/* 2374 */ -1,
/* 2375 */ 2383,
/* 2376 */ 50444,
/* 2377 */ -1,
/* 2378 */ -1,
/* 2379 */ -1,
/* 2380 */ 50059,
/* 2381 */ -2332,
/* 2382 */ -1,
/* 2383 */ 2390,
/* 2384 */ 50416,
/* 2385 */ -1,
/* 2386 */ -1,
/* 2387 */ 50059,
/* 2388 */ -2332,
/* 2389 */ -1,
/* 2390 */ 2397,
/* 2391 */ 50471,
/* 2392 */ -1,
/* 2393 */ -1,
/* 2394 */ 50059,
/* 2395 */ -2332,
/* 2396 */ -1,
/* 2397 */ 2404,
/* 2398 */ 50472,
/* 2399 */ -1,
/* 2400 */ -1,
/* 2401 */ 50059,
/* 2402 */ -2332,
/* 2403 */ -1,
/* 2404 */ 2411,
/* 2405 */ 50332,
/* 2406 */ -1,
/* 2407 */ -1,
/* 2408 */ 50059,
/* 2409 */ -2332,
/* 2410 */ -1,
/* 2411 */ 2418,
/* 2412 */ 50413,
/* 2413 */ -1,
/* 2414 */ -1,
/* 2415 */ 50059,
/* 2416 */ -2332,
/* 2417 */ -1,
/* 2418 */ 2425,
/* 2419 */ 50296,
/* 2420 */ -1,
/* 2421 */ -1,
/* 2422 */ 50059,
/* 2423 */ -2332,
/* 2424 */ -1,
/* 2425 */ 2432,
/* 2426 */ 50377,
/* 2427 */ -1,
/* 2428 */ -1,
/* 2429 */ 50059,
/* 2430 */ -2332,
/* 2431 */ -1,
/* 2432 */ 2439,
/* 2433 */ 50460,
/* 2434 */ -1,
/* 2435 */ -1,
/* 2436 */ 50059,
/* 2437 */ -2332,
/* 2438 */ -1,
/* 2439 */ 2446,
/* 2440 */ 50427,
/* 2441 */ -1,
/* 2442 */ -1,
/* 2443 */ 50059,
/* 2444 */ -2332,
/* 2445 */ -1,
/* 2446 */ 2453,
/* 2447 */ 50427,
/* 2448 */ -1,
/* 2449 */ -1,
/* 2450 */ 50059,
/* 2451 */ -2332,
/* 2452 */ -1,
/* 2453 */ 2460,
/* 2454 */ 50317,
/* 2455 */ -1,
/* 2456 */ -1,
/* 2457 */ 50059,
/* 2458 */ -2332,
/* 2459 */ -1,
/* 2460 */ 2467,
/* 2461 */ 50317,
/* 2462 */ -1,
/* 2463 */ 50274,
/* 2464 */ 50059,
/* 2465 */ -2332,
/* 2466 */ -1,
/* 2467 */ 2474,
/* 2468 */ 50349,
/* 2469 */ -1,
/* 2470 */ -1,
/* 2471 */ 50059,
/* 2472 */ -2332,
/* 2473 */ -1,
/* 2474 */ 2478,
/* 2475 */ -1,
/* 2476 */ -2332,
/* 2477 */ -1,
/* 2478 */ 2482,
/* 2479 */ -1,
/* 2480 */ -2332,
/* 2481 */ -1,
/* 2482 */ 0,
/* 2483 */ -1,
/* 2484 */ -2332,
/* 2485 */ -1,
/* 2486 */ 2490,
/* 2487 */ 50058,
/* 2488 */ -2486,
/* 2489 */ -1,
/* 2490 */ 0,
/* 2491 */ -2486,
/* 2492 */ -1,
/* 2493 */ 2497,
/* 2494 */ 50058,
/* 2495 */ -2493,
/* 2496 */ -1,
/* 2497 */ 0,
/* 2498 */ -2493,
/* 2499 */ -1,
/* 2500 */ 2504,
/* 2501 */ 50058,
/* 2502 */ -2500,
/* 2503 */ -1,
/* 2504 */ 0,
/* 2505 */ -2500,
/* 2506 */ -1,
/* 2507 */ 2511,
/* 2508 */ 50307,
/* 2509 */ -2507,
/* 2510 */ -1,
/* 2511 */ 0,
/* 2512 */ -2507,
/* 2513 */ -1,
/* 2514 */ 2518,
/* 2515 */ 50058,
/* 2516 */ -2514,
/* 2517 */ -1,
/* 2518 */ 0,
/* 2519 */ -2514,
/* 2520 */ -1,
/* 2521 */ 2525,
/* 2522 */ 50037,
/* 2523 */ -2521,
/* 2524 */ -1,
/* 2525 */ 0,
/* 2526 */ -2521,
/* 2527 */ -1,
/* 2528 */ 2532,
/* 2529 */ 50058,
/* 2530 */ -2528,
/* 2531 */ -1,
/* 2532 */ 0,
/* 2533 */ -2528,
/* 2534 */ -1,
/* 2535 */ 2541,
/* 2536 */ 50040,
/* 2537 */ -1,
/* 2538 */ 50041,
/* 2539 */ -2535,
/* 2540 */ -1,
/* 2541 */ 0,
/* 2542 */ -2535,
/* 2543 */ -1,
/* 2544 */ 2548,
/* 2545 */ 50058,
/* 2546 */ -2544,
/* 2547 */ -1,
/* 2548 */ 0,
/* 2549 */ -2544,
/* 2550 */ -1,
/* 2551 */ 2555,
/* 2552 */ 50404,
/* 2553 */ -2551,
/* 2554 */ -1,
/* 2555 */ 0,
/* 2556 */ -2551,
/* 2557 */ -1,
/* 2558 */ 2562,
/* 2559 */ 50058,
/* 2560 */ -2558,
/* 2561 */ -1,
/* 2562 */ 0,
/* 2563 */ -2558,
/* 2564 */ -1,
/* 2565 */ 2569,
/* 2566 */ 50058,
/* 2567 */ -2565,
/* 2568 */ -1,
/* 2569 */ 0,
/* 2570 */ -2565,
/* 2571 */ -1,
/* 2572 */ 2576,
/* 2573 */ 50058,
/* 2574 */ -2572,
/* 2575 */ -1,
/* 2576 */ 0,
/* 2577 */ -2572,
/* 2578 */ -1,
/* 2579 */ 2583,
/* 2580 */ 50058,
/* 2581 */ -2579,
/* 2582 */ -1,
/* 2583 */ 0,
/* 2584 */ -2579,
/* 2585 */ -1,
/* 2586 */ 2590,
/* 2587 */ 50058,
/* 2588 */ -2586,
/* 2589 */ -1,
/* 2590 */ 0,
/* 2591 */ -2586,
/* 2592 */ -1,
/* 2593 */ 2597,
/* 2594 */ 50058,
/* 2595 */ -2593,
/* 2596 */ -1,
/* 2597 */ 0,
/* 2598 */ -2593,
/* 2599 */ -1,
/* 2600 */ 2604,
/* 2601 */ 50058,
/* 2602 */ -2600,
/* 2603 */ -1,
/* 2604 */ 0,
/* 2605 */ -2600,
/* 2606 */ -1,
/* 2607 */ 2611,
/* 2608 */ 50058,
/* 2609 */ -2607,
/* 2610 */ -1,
/* 2611 */ 0,
/* 2612 */ -2607,
/* 2613 */ -1,
/* 2614 */ 2618,
/* 2615 */ 50058,
/* 2616 */ -2614,
/* 2617 */ -1,
/* 2618 */ 0,
/* 2619 */ -2614,
/* 2620 */ -1,
/* 2621 */ 2625,
/* 2622 */ 50058,
/* 2623 */ -2621,
/* 2624 */ -1,
/* 2625 */ 0,
/* 2626 */ -2621,
/* 2627 */ -1,
/* 2628 */ 2632,
/* 2629 */ 50058,
/* 2630 */ -2628,
/* 2631 */ -1,
/* 2632 */ 0,
/* 2633 */ -2628,
/* 2634 */ -1,
/* 2635 */ 2639,
/* 2636 */ 50058,
/* 2637 */ -2635,
/* 2638 */ -1,
/* 2639 */ 0,
/* 2640 */ -2635,
/* 2641 */ -1,
/* 2642 */ 2646,
/* 2643 */ 50058,
/* 2644 */ -2642,
/* 2645 */ -1,
/* 2646 */ 0,
/* 2647 */ -2642,
/* 2648 */ -1,
/* 2649 */ 2655,
/* 2650 */ -1,
/* 2651 */ -1,
/* 2652 */ -1,
/* 2653 */ -2649,
/* 2654 */ -1,
/* 2655 */ 0,
/* 2656 */ -2649,
/* 2657 */ -1,
/* 2658 */ 2662,
/* 2659 */ 50044,
/* 2660 */ -2658,
/* 2661 */ -1,
/* 2662 */ 2666,
/* 2663 */ 50124,
/* 2664 */ -2658,
/* 2665 */ -1,
/* 2666 */ 0,
/* 2667 */ -2658,
/* 2668 */ -1,
/* 2669 */ 0,
/* 2670 */ -1,
/* 2671 */ -2669,
/* 2672 */ -1,
/* 2673 */ 2679,
/* 2674 */ -1,
/* 2675 */ -1,
/* 2676 */ -1,
/* 2677 */ -2673,
/* 2678 */ -1,
/* 2679 */ 0,
/* 2680 */ -2673,
/* 2681 */ -1,
/* 2682 */ 2686,
/* 2683 */ 50044,
/* 2684 */ -2682,
/* 2685 */ -1,
/* 2686 */ 2690,
/* 2687 */ 50124,
/* 2688 */ -2682,
/* 2689 */ -1,
/* 2690 */ 0,
/* 2691 */ -2682,
/* 2692 */ -1,
/* 2693 */ 0,
/* 2694 */ 50261,
/* 2695 */ -2693,
/* 2696 */ -1,
/* 2697 */ 0,
/* 2698 */ 50417,
/* 2699 */ 50257,
/* 2700 */ -1,
/* 2701 */ 50123,
/* 2702 */ -1,
/* 2703 */ 50125,
/* 2704 */ -2697,
/* 2705 */ -1,
/* 2706 */ 2711,
/* 2707 */ 50058,
/* 2708 */ -1,
/* 2709 */ -2706,
/* 2710 */ -1,
/* 2711 */ 0,
/* 2712 */ -2706,
/* 2713 */ -1,
/* 2714 */ 2719,
/* 2715 */ -1,
/* 2716 */ -1,
/* 2717 */ -2714,
/* 2718 */ -1,
/* 2719 */ 0,
/* 2720 */ -2714,
/* 2721 */ -1,
/* 2722 */ 2729,
/* 2723 */ 50325,
/* 2724 */ -1,
/* 2725 */ 50257,
/* 2726 */ 50059,
/* 2727 */ -2722,
/* 2728 */ -1,
/* 2729 */ 2734,
/* 2730 */ -1,
/* 2731 */ 50059,
/* 2732 */ -2722,
/* 2733 */ -1,
/* 2734 */ 2741,
/* 2735 */ 50358,
/* 2736 */ -1,
/* 2737 */ -1,
/* 2738 */ 50059,
/* 2739 */ -2722,
/* 2740 */ -1,
/* 2741 */ 2748,
/* 2742 */ 50387,
/* 2743 */ -1,
/* 2744 */ -1,
/* 2745 */ 50059,
/* 2746 */ -2722,
/* 2747 */ -1,
/* 2748 */ 2758,
/* 2749 */ 50339,
/* 2750 */ -1,
/* 2751 */ -1,
/* 2752 */ -1,
/* 2753 */ 50044,
/* 2754 */ -1,
/* 2755 */ 50059,
/* 2756 */ -2722,
/* 2757 */ -1,
/* 2758 */ 2770,
/* 2759 */ 50339,
/* 2760 */ -1,
/* 2761 */ -1,
/* 2762 */ -1,
/* 2763 */ 50044,
/* 2764 */ -1,
/* 2765 */ 50044,
/* 2766 */ -1,
/* 2767 */ 50059,
/* 2768 */ -2722,
/* 2769 */ -1,
/* 2770 */ 2777,
/* 2771 */ 50431,
/* 2772 */ -1,
/* 2773 */ -1,
/* 2774 */ 50059,
/* 2775 */ -2722,
/* 2776 */ -1,
/* 2777 */ 2784,
/* 2778 */ -1,
/* 2779 */ -1,
/* 2780 */ 50257,
/* 2781 */ 50059,
/* 2782 */ -2722,
/* 2783 */ -1,
/* 2784 */ 2791,
/* 2785 */ 50349,
/* 2786 */ -1,
/* 2787 */ -1,
/* 2788 */ 50059,
/* 2789 */ -2722,
/* 2790 */ -1,
/* 2791 */ 2795,
/* 2792 */ -1,
/* 2793 */ -2722,
/* 2794 */ -1,
/* 2795 */ 2799,
/* 2796 */ -1,
/* 2797 */ -2722,
/* 2798 */ -1,
/* 2799 */ 2803,
/* 2800 */ -1,
/* 2801 */ -2722,
/* 2802 */ -1,
/* 2803 */ 0,
/* 2804 */ -1,
/* 2805 */ -2722,
/* 2806 */ -1,
/* 2807 */ 2811,
/* 2808 */ 50058,
/* 2809 */ -2807,
/* 2810 */ -1,
/* 2811 */ 0,
/* 2812 */ -2807,
/* 2813 */ -1,
/* 2814 */ 2822,
/* 2815 */ 50276,
/* 2816 */ -1,
/* 2817 */ -1,
/* 2818 */ -1,
/* 2819 */ -1,
/* 2820 */ -2814,
/* 2821 */ -1,
/* 2822 */ 0,
/* 2823 */ -2814,
/* 2824 */ -1,
/* 2825 */ 2829,
/* 2826 */ 50058,
/* 2827 */ -2825,
/* 2828 */ -1,
/* 2829 */ 0,
/* 2830 */ -2825,
/* 2831 */ -1,
/* 2832 */ 2836,
/* 2833 */ 50044,
/* 2834 */ -2832,
/* 2835 */ -1,
/* 2836 */ 0,
/* 2837 */ -2832,
/* 2838 */ -1,
/* 2839 */ 2843,
/* 2840 */ 50058,
/* 2841 */ -2839,
/* 2842 */ -1,
/* 2843 */ 0,
/* 2844 */ -2839,
/* 2845 */ -1,
/* 2846 */ 2852,
/* 2847 */ -1,
/* 2848 */ -1,
/* 2849 */ -1,
/* 2850 */ -2846,
/* 2851 */ -1,
/* 2852 */ 0,
/* 2853 */ -2846,
/* 2854 */ -1,
/* 2855 */ 2859,
/* 2856 */ 50044,
/* 2857 */ -2855,
/* 2858 */ -1,
/* 2859 */ 0,
/* 2860 */ -2855,
/* 2861 */ -1,
/* 2862 */ 2866,
/* 2863 */ 50058,
/* 2864 */ -2862,
/* 2865 */ -1,
/* 2866 */ 0,
/* 2867 */ -2862,
/* 2868 */ -1,
/* 2869 */ 2873,
/* 2870 */ 50308,
/* 2871 */ -2869,
/* 2872 */ -1,
/* 2873 */ 0,
/* 2874 */ -2869,
/* 2875 */ -1,
/* 2876 */ 2880,
/* 2877 */ 50058,
/* 2878 */ -2876,
/* 2879 */ -1,
/* 2880 */ 0,
/* 2881 */ -2876,
/* 2882 */ -1,
/* 2883 */ 2887,
/* 2884 */ 50308,
/* 2885 */ -2883,
/* 2886 */ -1,
/* 2887 */ 0,
/* 2888 */ -2883,
/* 2889 */ -1,
/* 2890 */ 2894,
/* 2891 */ 50058,
/* 2892 */ -2890,
/* 2893 */ -1,
/* 2894 */ 0,
/* 2895 */ -2890,
/* 2896 */ -1,
/* 2897 */ 2901,
/* 2898 */ 50058,
/* 2899 */ -2897,
/* 2900 */ -1,
/* 2901 */ 0,
/* 2902 */ -2897,
/* 2903 */ -1,
/* 2904 */ 2910,
/* 2905 */ -1,
/* 2906 */ -1,
/* 2907 */ -1,
/* 2908 */ -2904,
/* 2909 */ -1,
/* 2910 */ 0,
/* 2911 */ -2904,
/* 2912 */ -1,
/* 2913 */ 2917,
/* 2914 */ 50044,
/* 2915 */ -2913,
/* 2916 */ -1,
/* 2917 */ 0,
/* 2918 */ -2913,
/* 2919 */ -1,
/* 2920 */ 2924,
/* 2921 */ 50058,
/* 2922 */ -2920,
/* 2923 */ -1,
/* 2924 */ 0,
/* 2925 */ -2920,
/* 2926 */ -1,
/* 2927 */ 2931,
/* 2928 */ 50058,
/* 2929 */ -2927,
/* 2930 */ -1,
/* 2931 */ 0,
/* 2932 */ -2927,
/* 2933 */ -1,
/* 2934 */ 2940,
/* 2935 */ -1,
/* 2936 */ -1,
/* 2937 */ -1,
/* 2938 */ -2934,
/* 2939 */ -1,
/* 2940 */ 0,
/* 2941 */ -2934,
/* 2942 */ -1,
/* 2943 */ 2947,
/* 2944 */ 50124,
/* 2945 */ -2943,
/* 2946 */ -1,
/* 2947 */ 2951,
/* 2948 */ 50044,
/* 2949 */ -2943,
/* 2950 */ -1,
/* 2951 */ 0,
/* 2952 */ -2943,
/* 2953 */ -1,
/* 2954 */ 0,
/* 2955 */ 50308,
/* 2956 */ 50257,
/* 2957 */ 50123,
/* 2958 */ -1,
/* 2959 */ 50125,
/* 2960 */ -2954,
/* 2961 */ -1,
/* 2962 */ 2967,
/* 2963 */ -1,
/* 2964 */ -1,
/* 2965 */ -2962,
/* 2966 */ -1,
/* 2967 */ 0,
/* 2968 */ -2962,
/* 2969 */ -1,
/* 2970 */ 2977,
/* 2971 */ 50325,
/* 2972 */ -1,
/* 2973 */ 50257,
/* 2974 */ 50059,
/* 2975 */ -2970,
/* 2976 */ -1,
/* 2977 */ 2985,
/* 2978 */ 50363,
/* 2979 */ -1,
/* 2980 */ -1,
/* 2981 */ -1,
/* 2982 */ 50059,
/* 2983 */ -2970,
/* 2984 */ -1,
/* 2985 */ 2993,
/* 2986 */ 50380,
/* 2987 */ -1,
/* 2988 */ -1,
/* 2989 */ -1,
/* 2990 */ 50059,
/* 2991 */ -2970,
/* 2992 */ -1,
/* 2993 */ 3002,
/* 2994 */ 50322,
/* 2995 */ -1,
/* 2996 */ -1,
/* 2997 */ 50047,
/* 2998 */ -1,
/* 2999 */ 50059,
/* 3000 */ -2970,
/* 3001 */ -1,
/* 3002 */ 3009,
/* 3003 */ 50295,
/* 3004 */ -1,
/* 3005 */ -1,
/* 3006 */ 50059,
/* 3007 */ -2970,
/* 3008 */ -1,
/* 3009 */ 3016,
/* 3010 */ 50428,
/* 3011 */ -1,
/* 3012 */ -1,
/* 3013 */ 50059,
/* 3014 */ -2970,
/* 3015 */ -1,
/* 3016 */ 3023,
/* 3017 */ 50358,
/* 3018 */ -1,
/* 3019 */ -1,
/* 3020 */ 50059,
/* 3021 */ -2970,
/* 3022 */ -1,
/* 3023 */ 3033,
/* 3024 */ 50431,
/* 3025 */ -1,
/* 3026 */ -1,
/* 3027 */ -1,
/* 3028 */ -1,
/* 3029 */ -1,
/* 3030 */ 50059,
/* 3031 */ -2970,
/* 3032 */ -1,
/* 3033 */ 3040,
/* 3034 */ 50410,
/* 3035 */ -1,
/* 3036 */ -1,
/* 3037 */ 50059,
/* 3038 */ -2970,
/* 3039 */ -1,
/* 3040 */ 3047,
/* 3041 */ 50349,
/* 3042 */ -1,
/* 3043 */ -1,
/* 3044 */ 50059,
/* 3045 */ -2970,
/* 3046 */ -1,
/* 3047 */ 3051,
/* 3048 */ -1,
/* 3049 */ -2970,
/* 3050 */ -1,
/* 3051 */ 3055,
/* 3052 */ -1,
/* 3053 */ -2970,
/* 3054 */ -1,
/* 3055 */ 3059,
/* 3056 */ -1,
/* 3057 */ -2970,
/* 3058 */ -1,
/* 3059 */ 0,
/* 3060 */ -1,
/* 3061 */ -2970,
/* 3062 */ -1,
/* 3063 */ 3067,
/* 3064 */ 50058,
/* 3065 */ -3063,
/* 3066 */ -1,
/* 3067 */ 0,
/* 3068 */ -3063,
/* 3069 */ -1,
/* 3070 */ 3074,
/* 3071 */ 50058,
/* 3072 */ -3070,
/* 3073 */ -1,
/* 3074 */ 0,
/* 3075 */ -3070,
/* 3076 */ -1,
/* 3077 */ 3081,
/* 3078 */ 50100,
/* 3079 */ -3077,
/* 3080 */ -1,
/* 3081 */ 0,
/* 3082 */ -3077,
/* 3083 */ -1,
/* 3084 */ 3088,
/* 3085 */ 50058,
/* 3086 */ -3084,
/* 3087 */ -1,
/* 3088 */ 0,
/* 3089 */ -3084,
/* 3090 */ -1,
/* 3091 */ 3095,
/* 3092 */ 50100,
/* 3093 */ -3091,
/* 3094 */ -1,
/* 3095 */ 0,
/* 3096 */ -3091,
/* 3097 */ -1,
/* 3098 */ 3102,
/* 3099 */ 50058,
/* 3100 */ -3098,
/* 3101 */ -1,
/* 3102 */ 0,
/* 3103 */ -3098,
/* 3104 */ -1,
/* 3105 */ 3109,
/* 3106 */ 50058,
/* 3107 */ -3105,
/* 3108 */ -1,
/* 3109 */ 0,
/* 3110 */ -3105,
/* 3111 */ -1,
/* 3112 */ 3119,
/* 3113 */ -1,
/* 3114 */ -1,
/* 3115 */ -1,
/* 3116 */ -1,
/* 3117 */ -3112,
/* 3118 */ -1,
/* 3119 */ 0,
/* 3120 */ -3112,
/* 3121 */ -1,
/* 3122 */ 3126,
/* 3123 */ 50044,
/* 3124 */ -3122,
/* 3125 */ -1,
/* 3126 */ 0,
/* 3127 */ -3122,
/* 3128 */ -1,
/* 3129 */ 3133,
/* 3130 */ 50058,
/* 3131 */ -3129,
/* 3132 */ -1,
/* 3133 */ 0,
/* 3134 */ -3129,
/* 3135 */ -1,
/* 3136 */ 3143,
/* 3137 */ -1,
/* 3138 */ -1,
/* 3139 */ -1,
/* 3140 */ -1,
/* 3141 */ -3136,
/* 3142 */ -1,
/* 3143 */ 0,
/* 3144 */ -3136,
/* 3145 */ -1,
/* 3146 */ 3150,
/* 3147 */ 50044,
/* 3148 */ -3146,
/* 3149 */ -1,
/* 3150 */ 0,
/* 3151 */ -3146,
/* 3152 */ -1,
/* 3153 */ 3157,
/* 3154 */ 50058,
/* 3155 */ -3153,
/* 3156 */ -1,
/* 3157 */ 0,
/* 3158 */ -3153,
/* 3159 */ -1,
/* 3160 */ 3166,
/* 3161 */ -1,
/* 3162 */ -1,
/* 3163 */ -1,
/* 3164 */ -3160,
/* 3165 */ -1,
/* 3166 */ 0,
/* 3167 */ -3160,
/* 3168 */ -1,
/* 3169 */ 3173,
/* 3170 */ 50044,
/* 3171 */ -3169,
/* 3172 */ -1,
/* 3173 */ 0,
/* 3174 */ -3169,
/* 3175 */ -1,
/* 3176 */ 0,
/* 3177 */ 50091,
/* 3178 */ -3176,
/* 3179 */ -1,
/* 3180 */ 0,
/* 3181 */ 50093,
/* 3182 */ -3180,
/* 3183 */ -1,
/* 3184 */ 3188,
/* 3185 */ 50058,
/* 3186 */ -3184,
/* 3187 */ -1,
/* 3188 */ 0,
/* 3189 */ -3184,
/* 3190 */ -1,
/* 3191 */ 3197,
/* 3192 */ -1,
/* 3193 */ -1,
/* 3194 */ -1,
/* 3195 */ -3191,
/* 3196 */ -1,
/* 3197 */ 0,
/* 3198 */ -3191,
/* 3199 */ -1,
/* 3200 */ 3204,
/* 3201 */ 50044,
/* 3202 */ -3200,
/* 3203 */ -1,
/* 3204 */ 0,
/* 3205 */ -3200,
/* 3206 */ -1,
/* 3207 */ 3211,
/* 3208 */ 50058,
/* 3209 */ -3207,
/* 3210 */ -1,
/* 3211 */ 0,
/* 3212 */ -3207,
/* 3213 */ -1,
/* 3214 */ 3220,
/* 3215 */ -1,
/* 3216 */ -1,
/* 3217 */ -1,
/* 3218 */ -3214,
/* 3219 */ -1,
/* 3220 */ 0,
/* 3221 */ -3214,
/* 3222 */ -1,
/* 3223 */ 3227,
/* 3224 */ 50044,
/* 3225 */ -3223,
/* 3226 */ -1,
/* 3227 */ 0,
/* 3228 */ -3223,
/* 3229 */ -1,
/* 3230 */ 3234,
/* 3231 */ 50058,
/* 3232 */ -3230,
/* 3233 */ -1,
/* 3234 */ 0,
/* 3235 */ -3230,
/* 3236 */ -1,
/* 3237 */ 3243,
/* 3238 */ -1,
/* 3239 */ -1,
/* 3240 */ -1,
/* 3241 */ -3237,
/* 3242 */ -1,
/* 3243 */ 0,
/* 3244 */ -3237,
/* 3245 */ -1,
/* 3246 */ 3250,
/* 3247 */ 50124,
/* 3248 */ -3246,
/* 3249 */ -1,
/* 3250 */ 3254,
/* 3251 */ 50044,
/* 3252 */ -3246,
/* 3253 */ -1,
/* 3254 */ 0,
/* 3255 */ -3246,
/* 3256 */ -1,
/* 3257 */ 3267,
/* 3258 */ 50281,
/* 3259 */ 50091,
/* 3260 */ -1,
/* 3261 */ -1,
/* 3262 */ 50093,
/* 3263 */ 50294,
/* 3264 */ -1,
/* 3265 */ -3257,
/* 3266 */ -1,
/* 3267 */ 3277,
/* 3268 */ 50439,
/* 3269 */ 50091,
/* 3270 */ -1,
/* 3271 */ -1,
/* 3272 */ 50093,
/* 3273 */ 50294,
/* 3274 */ -1,
/* 3275 */ -3257,
/* 3276 */ -1,
/* 3277 */ 0,
/* 3278 */ 50346,
/* 3279 */ 50091,
/* 3280 */ -1,
/* 3281 */ -1,
/* 3282 */ 50093,
/* 3283 */ 50294,
/* 3284 */ -1,
/* 3285 */ -3257,
/* 3286 */ -1,
/* 3287 */ 3291,
/* 3288 */ -1,
/* 3289 */ -3287,
/* 3290 */ -1,
/* 3291 */ 0,
/* 3292 */ -3287,
/* 3293 */ -1,
/* 3294 */ 3298,
/* 3295 */ -1,
/* 3296 */ -3294,
/* 3297 */ -1,
/* 3298 */ 0,
/* 3299 */ -3294,
/* 3300 */ -1,
/* 3301 */ 3305,
/* 3302 */ -1,
/* 3303 */ -3301,
/* 3304 */ -1,
/* 3305 */ 0,
/* 3306 */ -3301,
/* 3307 */ -1,
/* 3308 */ 3313,
/* 3309 */ 50044,
/* 3310 */ -1,
/* 3311 */ -3308,
/* 3312 */ -1,
/* 3313 */ 3318,
/* 3314 */ 50044,
/* 3315 */ -1,
/* 3316 */ -3308,
/* 3317 */ -1,
/* 3318 */ 0,
/* 3319 */ 50044,
/* 3320 */ -1,
/* 3321 */ 50044,
/* 3322 */ -1,
/* 3323 */ -3308,
/* 3324 */ -1,
/* 3325 */ 3330,
/* 3326 */ -1,
/* 3327 */ -1,
/* 3328 */ -3325,
/* 3329 */ -1,
/* 3330 */ 3336,
/* 3331 */ 50342,
/* 3332 */ -1,
/* 3333 */ 50373,
/* 3334 */ -3325,
/* 3335 */ -1,
/* 3336 */ 3341,
/* 3337 */ 50342,
/* 3338 */ 50373,
/* 3339 */ -3325,
/* 3340 */ -1,
/* 3341 */ 3350,
/* 3342 */ 50342,
/* 3343 */ 50373,
/* 3344 */ 50438,
/* 3345 */ 50294,
/* 3346 */ -1,
/* 3347 */ -1,
/* 3348 */ -3325,
/* 3349 */ -1,
/* 3350 */ 0,
/* 3351 */ 50342,
/* 3352 */ -1,
/* 3353 */ 50373,
/* 3354 */ 50438,
/* 3355 */ 50294,
/* 3356 */ -1,
/* 3357 */ -1,
/* 3358 */ -3325,
/* 3359 */ -1,
/* 3360 */ 3364,
/* 3361 */ 50373,
/* 3362 */ -3360,
/* 3363 */ -1,
/* 3364 */ 0,
/* 3365 */ -3360,
/* 3366 */ -1,
/* 3367 */ 3371,
/* 3368 */ 50373,
/* 3369 */ -3367,
/* 3370 */ -1,
/* 3371 */ 0,
/* 3372 */ -3367,
/* 3373 */ -1,
/* 3374 */ 3378,
/* 3375 */ 50373,
/* 3376 */ -3374,
/* 3377 */ -1,
/* 3378 */ 0,
/* 3379 */ -3374,
/* 3380 */ -1,
/* 3381 */ 0,
/* 3382 */ 50331,
/* 3383 */ 50257,
/* 3384 */ 50123,
/* 3385 */ -1,
/* 3386 */ 50125,
/* 3387 */ -3381,
/* 3388 */ -1,
/* 3389 */ 3394,
/* 3390 */ -1,
/* 3391 */ -1,
/* 3392 */ -3389,
/* 3393 */ -1,
/* 3394 */ 0,
/* 3395 */ -3389,
/* 3396 */ -1,
/* 3397 */ 3404,
/* 3398 */ 50325,
/* 3399 */ -1,
/* 3400 */ 50257,
/* 3401 */ 50059,
/* 3402 */ -3397,
/* 3403 */ -1,
/* 3404 */ 3408,
/* 3405 */ -1,
/* 3406 */ -3397,
/* 3407 */ -1,
/* 3408 */ 3412,
/* 3409 */ -1,
/* 3410 */ -3397,
/* 3411 */ -1,
/* 3412 */ 0,
/* 3413 */ -1,
/* 3414 */ -3397,
/* 3415 */ -1,
/* 3416 */ 3420,
/* 3417 */ 50058,
/* 3418 */ -3416,
/* 3419 */ -1,
/* 3420 */ 0,
/* 3421 */ -3416,
/* 3422 */ -1,
/* 3423 */ 3430,
/* 3424 */ 50310,
/* 3425 */ -1,
/* 3426 */ -1,
/* 3427 */ 50059,
/* 3428 */ -3423,
/* 3429 */ -1,
/* 3430 */ 3436,
/* 3431 */ 50376,
/* 3432 */ -1,
/* 3433 */ -1,
/* 3434 */ -3423,
/* 3435 */ -1,
/* 3436 */ 0,
/* 3437 */ 50433,
/* 3438 */ -1,
/* 3439 */ -1,
/* 3440 */ 50059,
/* 3441 */ -3423,
/* 3442 */ -1,
/* 3443 */ 3447,
/* 3444 */ 50058,
/* 3445 */ -3443,
/* 3446 */ -1,
/* 3447 */ 0,
/* 3448 */ -3443,
/* 3449 */ -1,
/* 3450 */ 3458,
/* 3451 */ 50042,
/* 3452 */ -1,
/* 3453 */ -1,
/* 3454 */ -1,
/* 3455 */ -1,
/* 3456 */ -3450,
/* 3457 */ -1,
/* 3458 */ 3466,
/* 3459 */ 50042,
/* 3460 */ -1,
/* 3461 */ -1,
/* 3462 */ -1,
/* 3463 */ -1,
/* 3464 */ -3450,
/* 3465 */ -1,
/* 3466 */ 0,
/* 3467 */ -3450,
/* 3468 */ -1,
/* 3469 */ 3473,
/* 3470 */ 50044,
/* 3471 */ -3469,
/* 3472 */ -1,
/* 3473 */ 0,
/* 3474 */ -3469,
/* 3475 */ -1,
/* 3476 */ 3480,
/* 3477 */ 50044,
/* 3478 */ -3476,
/* 3479 */ -1,
/* 3480 */ 0,
/* 3481 */ -3476,
/* 3482 */ -1,
/* 3483 */ 3487,
/* 3484 */ 50058,
/* 3485 */ -3483,
/* 3486 */ -1,
/* 3487 */ 0,
/* 3488 */ -3483,
/* 3489 */ -1,
/* 3490 */ 3498,
/* 3491 */ 50042,
/* 3492 */ -1,
/* 3493 */ -1,
/* 3494 */ -1,
/* 3495 */ -1,
/* 3496 */ -3490,
/* 3497 */ -1,
/* 3498 */ 0,
/* 3499 */ -3490,
/* 3500 */ -1,
/* 3501 */ 3506,
/* 3502 */ -1,
/* 3503 */ -1,
/* 3504 */ -3501,
/* 3505 */ -1,
/* 3506 */ 0,
/* 3507 */ -3501,
/* 3508 */ -1,
/* 3509 */ 3513,
/* 3510 */ 50044,
/* 3511 */ -3509,
/* 3512 */ -1,
/* 3513 */ 0,
/* 3514 */ 50059,
/* 3515 */ -3509,
/* 3516 */ -1,
/* 3517 */ 3521,
/* 3518 */ 50058,
/* 3519 */ -3517,
/* 3520 */ -1,
/* 3521 */ 0,
/* 3522 */ -3517,
/* 3523 */ -1,
/* 3524 */ 3532,
/* 3525 */ 50042,
/* 3526 */ -1,
/* 3527 */ -1,
/* 3528 */ -1,
/* 3529 */ -1,
/* 3530 */ -3524,
/* 3531 */ -1,
/* 3532 */ 0,
/* 3533 */ -3524,
/* 3534 */ -1,
/* 3535 */ 3540,
/* 3536 */ -1,
/* 3537 */ 50037,
/* 3538 */ -3535,
/* 3539 */ -1,
/* 3540 */ 0,
/* 3541 */ -3535,
/* 3542 */ -1,
/* 3543 */ 3550,
/* 3544 */ -1,
/* 3545 */ 50294,
/* 3546 */ 50373,
/* 3547 */ -1,
/* 3548 */ -3543,
/* 3549 */ -1,
/* 3550 */ 3557,
/* 3551 */ -1,
/* 3552 */ 50294,
/* 3553 */ -1,
/* 3554 */ 50373,
/* 3555 */ -3543,
/* 3556 */ -1,
/* 3557 */ 0,
/* 3558 */ -1,
/* 3559 */ 50294,
/* 3560 */ 50373,
/* 3561 */ -1,
/* 3562 */ -3543,
/* 3563 */ -1,
/* 3564 */ 3568,
/* 3565 */ 50044,
/* 3566 */ -3564,
/* 3567 */ -1,
/* 3568 */ 0,
/* 3569 */ -3564,
/* 3570 */ -1,
/* 3571 */ 3575,
/* 3572 */ -1,
/* 3573 */ -3571,
/* 3574 */ -1,
/* 3575 */ 3579,
/* 3576 */ -1,
/* 3577 */ -3571,
/* 3578 */ -1,
/* 3579 */ 0,
/* 3580 */ 50257,
/* 3581 */ -3571,
/* 3582 */ -1,
/* 3583 */ 0,
/* 3584 */ 50292,
/* 3585 */ 50257,
/* 3586 */ 50123,
/* 3587 */ -1,
/* 3588 */ 50125,
/* 3589 */ -3583,
/* 3590 */ -1,
/* 3591 */ 3596,
/* 3592 */ -1,
/* 3593 */ -1,
/* 3594 */ -3591,
/* 3595 */ -1,
/* 3596 */ 0,
/* 3597 */ -3591,
/* 3598 */ -1,
/* 3599 */ 3606,
/* 3600 */ 50325,
/* 3601 */ -1,
/* 3602 */ 50257,
/* 3603 */ 50059,
/* 3604 */ -3599,
/* 3605 */ -1,
/* 3606 */ 3613,
/* 3607 */ 50371,
/* 3608 */ -1,
/* 3609 */ -1,
/* 3610 */ 50059,
/* 3611 */ -3599,
/* 3612 */ -1,
/* 3613 */ 3618,
/* 3614 */ -1,
/* 3615 */ 50059,
/* 3616 */ -3599,
/* 3617 */ -1,
/* 3618 */ 3625,
/* 3619 */ 50423,
/* 3620 */ -1,
/* 3621 */ -1,
/* 3622 */ 50059,
/* 3623 */ -3599,
/* 3624 */ -1,
/* 3625 */ 3632,
/* 3626 */ 50443,
/* 3627 */ -1,
/* 3628 */ -1,
/* 3629 */ 50059,
/* 3630 */ -3599,
/* 3631 */ -1,
/* 3632 */ 3639,
/* 3633 */ 50414,
/* 3634 */ -1,
/* 3635 */ -1,
/* 3636 */ 50059,
/* 3637 */ -3599,
/* 3638 */ -1,
/* 3639 */ 3646,
/* 3640 */ 50337,
/* 3641 */ 50352,
/* 3642 */ -1,
/* 3643 */ 50059,
/* 3644 */ -3599,
/* 3645 */ -1,
/* 3646 */ 3653,
/* 3647 */ 50469,
/* 3648 */ 50352,
/* 3649 */ -1,
/* 3650 */ 50059,
/* 3651 */ -3599,
/* 3652 */ -1,
/* 3653 */ 3660,
/* 3654 */ 50365,
/* 3655 */ 50352,
/* 3656 */ -1,
/* 3657 */ 50059,
/* 3658 */ -3599,
/* 3659 */ -1,
/* 3660 */ 3667,
/* 3661 */ 50370,
/* 3662 */ 50352,
/* 3663 */ -1,
/* 3664 */ 50059,
/* 3665 */ -3599,
/* 3666 */ -1,
/* 3667 */ 3671,
/* 3668 */ -1,
/* 3669 */ -3599,
/* 3670 */ -1,
/* 3671 */ 0,
/* 3672 */ -1,
/* 3673 */ -3599,
/* 3674 */ -1,
/* 3675 */ 3679,
/* 3676 */ 50058,
/* 3677 */ -3675,
/* 3678 */ -1,
/* 3679 */ 0,
/* 3680 */ -3675,
/* 3681 */ -1,
/* 3682 */ 3686,
/* 3683 */ 50058,
/* 3684 */ -3682,
/* 3685 */ -1,
/* 3686 */ 0,
/* 3687 */ -3682,
/* 3688 */ -1,
/* 3689 */ 3697,
/* 3690 */ 50276,
/* 3691 */ -1,
/* 3692 */ -1,
/* 3693 */ -1,
/* 3694 */ -1,
/* 3695 */ -3689,
/* 3696 */ -1,
/* 3697 */ 0,
/* 3698 */ -3689,
/* 3699 */ -1,
/* 3700 */ 3704,
/* 3701 */ 50058,
/* 3702 */ -3700,
/* 3703 */ -1,
/* 3704 */ 0,
/* 3705 */ -3700,
/* 3706 */ -1,
/* 3707 */ 3711,
/* 3708 */ 50044,
/* 3709 */ -3707,
/* 3710 */ -1,
/* 3711 */ 0,
/* 3712 */ -3707,
/* 3713 */ -1,
/* 3714 */ 3718,
/* 3715 */ 50058,
/* 3716 */ -3714,
/* 3717 */ -1,
/* 3718 */ 0,
/* 3719 */ -3714,
/* 3720 */ -1,
/* 3721 */ 3727,
/* 3722 */ -1,
/* 3723 */ -1,
/* 3724 */ -1,
/* 3725 */ -3721,
/* 3726 */ -1,
/* 3727 */ 0,
/* 3728 */ -3721,
/* 3729 */ -1,
/* 3730 */ 3734,
/* 3731 */ 50044,
/* 3732 */ -3730,
/* 3733 */ -1,
/* 3734 */ 0,
/* 3735 */ -3730,
/* 3736 */ -1,
/* 3737 */ 3741,
/* 3738 */ 50058,
/* 3739 */ -3737,
/* 3740 */ -1,
/* 3741 */ 0,
/* 3742 */ -3737,
/* 3743 */ -1,
/* 3744 */ 3750,
/* 3745 */ 50276,
/* 3746 */ -1,
/* 3747 */ -1,
/* 3748 */ -3744,
/* 3749 */ -1,
/* 3750 */ 0,
/* 3751 */ -3744,
/* 3752 */ -1,
/* 3753 */ 3757,
/* 3754 */ 50044,
/* 3755 */ -3753,
/* 3756 */ -1,
/* 3757 */ 0,
/* 3758 */ -3753,
/* 3759 */ -1,
/* 3760 */ 3764,
/* 3761 */ 50058,
/* 3762 */ -3760,
/* 3763 */ -1,
/* 3764 */ 0,
/* 3765 */ -3760,
/* 3766 */ -1,
/* 3767 */ 3773,
/* 3768 */ -1,
/* 3769 */ -1,
/* 3770 */ -1,
/* 3771 */ -3767,
/* 3772 */ -1,
/* 3773 */ 0,
/* 3774 */ -3767,
/* 3775 */ -1,
/* 3776 */ 3780,
/* 3777 */ 50044,
/* 3778 */ -3776,
/* 3779 */ -1,
/* 3780 */ 0,
/* 3781 */ -3776,
/* 3782 */ -1,
/* 3783 */ 3789,
/* 3784 */ -1,
/* 3785 */ -1,
/* 3786 */ -1,
/* 3787 */ -3783,
/* 3788 */ -1,
/* 3789 */ 0,
/* 3790 */ -3783,
/* 3791 */ -1,
/* 3792 */ 0,
/* 3793 */ 50044,
/* 3794 */ -3792,
/* 3795 */ -1,
/* 3796 */ 3802,
/* 3797 */ -1,
/* 3798 */ -1,
/* 3799 */ -1,
/* 3800 */ -3796,
/* 3801 */ -1,
/* 3802 */ 0,
/* 3803 */ -3796,
/* 3804 */ -1,
/* 3805 */ 0,
/* 3806 */ 50044,
/* 3807 */ -3805,
/* 3808 */ -1,
/* 3809 */ 3815,
/* 3810 */ -1,
/* 3811 */ -1,
/* 3812 */ -1,
/* 3813 */ -3809,
/* 3814 */ -1,
/* 3815 */ 0,
/* 3816 */ -3809,
/* 3817 */ -1,
/* 3818 */ 0,
/* 3819 */ 50044,
/* 3820 */ -3818,
/* 3821 */ -1,
/* 3822 */ 3828,
/* 3823 */ -1,
/* 3824 */ -1,
/* 3825 */ -1,
/* 3826 */ -3822,
/* 3827 */ -1,
/* 3828 */ 0,
/* 3829 */ -3822,
/* 3830 */ -1,
/* 3831 */ 0,
/* 3832 */ 50044,
/* 3833 */ -3831,
/* 3834 */ -1,
/* 3835 */ 3844,
/* 3836 */ 50439,
/* 3837 */ 50091,
/* 3838 */ -1,
/* 3839 */ 50044,
/* 3840 */ -1,
/* 3841 */ 50093,
/* 3842 */ -3835,
/* 3843 */ -1,
/* 3844 */ 3851,
/* 3845 */ 50439,
/* 3846 */ 50091,
/* 3847 */ -1,
/* 3848 */ 50093,
/* 3849 */ -3835,
/* 3850 */ -1,
/* 3851 */ 3856,
/* 3852 */ -1,
/* 3853 */ -1,
/* 3854 */ -3835,
/* 3855 */ -1,
/* 3856 */ 3863,
/* 3857 */ -1,
/* 3858 */ 50342,
/* 3859 */ -1,
/* 3860 */ 50458,
/* 3861 */ -3835,
/* 3862 */ -1,
/* 3863 */ 3870,
/* 3864 */ -1,
/* 3865 */ -1,
/* 3866 */ -1,
/* 3867 */ 50320,
/* 3868 */ -3835,
/* 3869 */ -1,
/* 3870 */ 3877,
/* 3871 */ -1,
/* 3872 */ -1,
/* 3873 */ -1,
/* 3874 */ 50464,
/* 3875 */ -3835,
/* 3876 */ -1,
/* 3877 */ 3882,
/* 3878 */ 50369,
/* 3879 */ -1,
/* 3880 */ -3835,
/* 3881 */ -1,
/* 3882 */ 0,
/* 3883 */ 50346,
/* 3884 */ -1,
/* 3885 */ -3835,
/* 3886 */ -1,
/* 3887 */ 3892,
/* 3888 */ 50294,
/* 3889 */ 50278,
/* 3890 */ -3887,
/* 3891 */ -1,
/* 3892 */ 0,
/* 3893 */ -3887,
/* 3894 */ -1,
/* 3895 */ 3899,
/* 3896 */ 50047,
/* 3897 */ -3895,
/* 3898 */ -1,
/* 3899 */ 0,
/* 3900 */ 50407,
/* 3901 */ -3895,
/* 3902 */ -1,
/* 3903 */ 3907,
/* 3904 */ 50047,
/* 3905 */ -3903,
/* 3906 */ -1,
/* 3907 */ 0,
/* 3908 */ 50407,
/* 3909 */ -3903,
/* 3910 */ -1,
/* 3911 */ 0,
/* 3912 */ 50327,
/* 3913 */ 50257,
/* 3914 */ 50058,
/* 3915 */ -1,
/* 3916 */ 50123,
/* 3917 */ -1,
/* 3918 */ 50125,
/* 3919 */ -3911,
/* 3920 */ -1,
/* 3921 */ 3926,
/* 3922 */ -1,
/* 3923 */ -1,
/* 3924 */ -3921,
/* 3925 */ -1,
/* 3926 */ 0,
/* 3927 */ -3921,
/* 3928 */ -1,
/* 3929 */ 3936,
/* 3930 */ 50436,
/* 3931 */ -1,
/* 3932 */ -1,
/* 3933 */ 50059,
/* 3934 */ -3929,
/* 3935 */ -1,
/* 3936 */ 3943,
/* 3937 */ 50358,
/* 3938 */ -1,
/* 3939 */ -1,
/* 3940 */ 50059,
/* 3941 */ -3929,
/* 3942 */ -1,
/* 3943 */ 3950,
/* 3944 */ 50325,
/* 3945 */ -1,
/* 3946 */ 50257,
/* 3947 */ 50059,
/* 3948 */ -3929,
/* 3949 */ -1,
/* 3950 */ 3954,
/* 3951 */ -1,
/* 3952 */ -3929,
/* 3953 */ -1,
/* 3954 */ 3958,
/* 3955 */ -1,
/* 3956 */ -3929,
/* 3957 */ -1,
/* 3958 */ 0,
/* 3959 */ -1,
/* 3960 */ -3929,
/* 3961 */ -1,
/* 3962 */ 3966,
/* 3963 */ 50058,
/* 3964 */ -3962,
/* 3965 */ -1,
/* 3966 */ 0,
/* 3967 */ -3962,
/* 3968 */ -1,
/* 3969 */ 3975,
/* 3970 */ -1,
/* 3971 */ -1,
/* 3972 */ -1,
/* 3973 */ -3969,
/* 3974 */ -1,
/* 3975 */ 0,
/* 3976 */ -3969,
/* 3977 */ -1,
/* 3978 */ 3982,
/* 3979 */ 50044,
/* 3980 */ -3978,
/* 3981 */ -1,
/* 3982 */ 0,
/* 3983 */ -3978,
/* 3984 */ -1,
/* 3985 */ 3989,
/* 3986 */ 50058,
/* 3987 */ -3985,
/* 3988 */ -1,
/* 3989 */ 0,
/* 3990 */ -3985,
/* 3991 */ -1,
/* 3992 */ 3998,
/* 3993 */ -1,
/* 3994 */ -1,
/* 3995 */ -1,
/* 3996 */ -3992,
/* 3997 */ -1,
/* 3998 */ 0,
/* 3999 */ -3992,
/* 4000 */ -1,
/* 4001 */ 4005,
/* 4002 */ 50044,
/* 4003 */ -4001,
/* 4004 */ -1,
/* 4005 */ 0,
/* 4006 */ -4001,
/* 4007 */ -1,
/* 4008 */ 4012,
/* 4009 */ 50058,
/* 4010 */ -4008,
/* 4011 */ -1,
/* 4012 */ 0,
/* 4013 */ -4008,
/* 4014 */ -1,
/* 4015 */ 0,
/* 4016 */ 50357,
/* 4017 */ 50257,
/* 4018 */ 50123,
/* 4019 */ -1,
/* 4020 */ 50125,
/* 4021 */ -4015,
/* 4022 */ -1,
/* 4023 */ 4028,
/* 4024 */ -1,
/* 4025 */ -1,
/* 4026 */ -4023,
/* 4027 */ -1,
/* 4028 */ 0,
/* 4029 */ -4023,
/* 4030 */ -1,
/* 4031 */ 4038,
/* 4032 */ 50328,
/* 4033 */ -1,
/* 4034 */ -1,
/* 4035 */ 50059,
/* 4036 */ -4031,
/* 4037 */ -1,
/* 4038 */ 4043,
/* 4039 */ 50310,
/* 4040 */ 50059,
/* 4041 */ -4031,
/* 4042 */ -1,
/* 4043 */ 4050,
/* 4044 */ 50349,
/* 4045 */ -1,
/* 4046 */ -1,
/* 4047 */ 50059,
/* 4048 */ -4031,
/* 4049 */ -1,
/* 4050 */ 4057,
/* 4051 */ 50325,
/* 4052 */ -1,
/* 4053 */ 50257,
/* 4054 */ 50059,
/* 4055 */ -4031,
/* 4056 */ -1,
/* 4057 */ 4064,
/* 4058 */ 50358,
/* 4059 */ -1,
/* 4060 */ -1,
/* 4061 */ 50059,
/* 4062 */ -4031,
/* 4063 */ -1,
/* 4064 */ 4068,
/* 4065 */ -1,
/* 4066 */ -4031,
/* 4067 */ -1,
/* 4068 */ 4072,
/* 4069 */ -1,
/* 4070 */ -4031,
/* 4071 */ -1,
/* 4072 */ 0,
/* 4073 */ -1,
/* 4074 */ -4031,
/* 4075 */ -1,
/* 4076 */ 4080,
/* 4077 */ 50058,
/* 4078 */ -4076,
/* 4079 */ -1,
/* 4080 */ 0,
/* 4081 */ -4076,
/* 4082 */ -1,
/* 4083 */ 4089,
/* 4084 */ -1,
/* 4085 */ -1,
/* 4086 */ -1,
/* 4087 */ -4083,
/* 4088 */ -1,
/* 4089 */ 0,
/* 4090 */ -4083,
/* 4091 */ -1,
/* 4092 */ 4096,
/* 4093 */ 50044,
/* 4094 */ -4092,
/* 4095 */ -1,
/* 4096 */ 0,
/* 4097 */ -4092,
/* 4098 */ -1,
/* 4099 */ 4103,
/* 4100 */ 50058,
/* 4101 */ -4099,
/* 4102 */ -1,
/* 4103 */ 0,
/* 4104 */ -4099,
/* 4105 */ -1,
/* 4106 */ 4112,
/* 4107 */ -1,
/* 4108 */ -1,
/* 4109 */ -1,
/* 4110 */ -4106,
/* 4111 */ -1,
/* 4112 */ 0,
/* 4113 */ -4106,
/* 4114 */ -1,
/* 4115 */ 4119,
/* 4116 */ 50124,
/* 4117 */ -4115,
/* 4118 */ -1,
/* 4119 */ 4123,
/* 4120 */ 50044,
/* 4121 */ -4115,
/* 4122 */ -1,
/* 4123 */ 0,
/* 4124 */ -4115,
/* 4125 */ -1,
/* 4126 */ 4130,
/* 4127 */ 50058,
/* 4128 */ -4126,
/* 4129 */ -1,
/* 4130 */ 0,
/* 4131 */ -4126,
/* 4132 */ -1,
/* 4133 */ 4137,
/* 4134 */ 50058,
/* 4135 */ -4133,
/* 4136 */ -1,
/* 4137 */ 0,
/* 4138 */ -4133,
/* 4139 */ -1,
/* 4140 */ 4146,
/* 4141 */ -1,
/* 4142 */ -1,
/* 4143 */ -1,
/* 4144 */ -4140,
/* 4145 */ -1,
/* 4146 */ 0,
/* 4147 */ -4140,
/* 4148 */ -1,
/* 4149 */ 4153,
/* 4150 */ 50044,
/* 4151 */ -4149,
/* 4152 */ -1,
/* 4153 */ 0,
/* 4154 */ -4149,
/* 4155 */ -1,
/* 4156 */ 0,
/* 4157 */ 50449,
/* 4158 */ 50257,
/* 4159 */ 50123,
/* 4160 */ -1,
/* 4161 */ 50125,
/* 4162 */ -4156,
/* 4163 */ -1,
/* 4164 */ 4169,
/* 4165 */ -1,
/* 4166 */ -1,
/* 4167 */ -4164,
/* 4168 */ -1,
/* 4169 */ 0,
/* 4170 */ -4164,
/* 4171 */ -1,
/* 4172 */ 4179,
/* 4173 */ 50325,
/* 4174 */ -1,
/* 4175 */ 50257,
/* 4176 */ 50059,
/* 4177 */ -4172,
/* 4178 */ -1,
/* 4179 */ 4183,
/* 4180 */ -1,
/* 4181 */ -4172,
/* 4182 */ -1,
/* 4183 */ 4191,
/* 4184 */ 50388,
/* 4185 */ -1,
/* 4186 */ -1,
/* 4187 */ 50037,
/* 4188 */ 50059,
/* 4189 */ -4172,
/* 4190 */ -1,
/* 4191 */ 4198,
/* 4192 */ 50406,
/* 4193 */ -1,
/* 4194 */ -1,
/* 4195 */ 50059,
/* 4196 */ -4172,
/* 4197 */ -1,
/* 4198 */ 4205,
/* 4199 */ 50382,
/* 4200 */ -1,
/* 4201 */ -1,
/* 4202 */ 50059,
/* 4203 */ -4172,
/* 4204 */ -1,
/* 4205 */ 4212,
/* 4206 */ 50349,
/* 4207 */ -1,
/* 4208 */ -1,
/* 4209 */ 50059,
/* 4210 */ -4172,
/* 4211 */ -1,
/* 4212 */ 4216,
/* 4213 */ -1,
/* 4214 */ -4172,
/* 4215 */ -1,
/* 4216 */ 4220,
/* 4217 */ -1,
/* 4218 */ -4172,
/* 4219 */ -1,
/* 4220 */ 0,
/* 4221 */ -1,
/* 4222 */ -4172,
/* 4223 */ -1,
/* 4224 */ 4228,
/* 4225 */ 50058,
/* 4226 */ -4224,
/* 4227 */ -1,
/* 4228 */ 0,
/* 4229 */ -4224,
/* 4230 */ -1,
/* 4231 */ 4235,
/* 4232 */ 50058,
/* 4233 */ -4231,
/* 4234 */ -1,
/* 4235 */ 0,
/* 4236 */ -4231,
/* 4237 */ -1,
/* 4238 */ 4242,
/* 4239 */ 50058,
/* 4240 */ -4238,
/* 4241 */ -1,
/* 4242 */ 0,
/* 4243 */ -4238,
/* 4244 */ -1,
/* 4245 */ 4249,
/* 4246 */ 50058,
/* 4247 */ -4245,
/* 4248 */ -1,
/* 4249 */ 0,
/* 4250 */ -4245,
/* 4251 */ -1,
/* 4252 */ 4256,
/* 4253 */ 50058,
/* 4254 */ -4252,
/* 4255 */ -1,
/* 4256 */ 0,
/* 4257 */ -4252,
/* 4258 */ -1,
/* 4259 */ 4265,
/* 4260 */ -1,
/* 4261 */ -1,
/* 4262 */ -1,
/* 4263 */ -4259,
/* 4264 */ -1,
/* 4265 */ 0,
/* 4266 */ -4259,
/* 4267 */ -1,
/* 4268 */ 4272,
/* 4269 */ 50124,
/* 4270 */ -4268,
/* 4271 */ -1,
/* 4272 */ 4276,
/* 4273 */ 50044,
/* 4274 */ -4268,
/* 4275 */ -1,
/* 4276 */ 0,
/* 4277 */ -4268,
/* 4278 */ -1,
/* 4279 */ 0,
/* 4280 */ 50421,
/* 4281 */ 50257,
/* 4282 */ 50058,
/* 4283 */ -1,
/* 4284 */ 50123,
/* 4285 */ -1,
/* 4286 */ 50125,
/* 4287 */ -4279,
/* 4288 */ -1,
/* 4289 */ 4294,
/* 4290 */ -1,
/* 4291 */ -1,
/* 4292 */ -4289,
/* 4293 */ -1,
/* 4294 */ 0,
/* 4295 */ -4289,
/* 4296 */ -1,
/* 4297 */ 4304,
/* 4298 */ 50325,
/* 4299 */ -1,
/* 4300 */ 50257,
/* 4301 */ 50059,
/* 4302 */ -4297,
/* 4303 */ -1,
/* 4304 */ 4311,
/* 4305 */ 50463,
/* 4306 */ -1,
/* 4307 */ -1,
/* 4308 */ 50059,
/* 4309 */ -4297,
/* 4310 */ -1,
/* 4311 */ 4318,
/* 4312 */ 50350,
/* 4313 */ -1,
/* 4314 */ -1,
/* 4315 */ 50059,
/* 4316 */ -4297,
/* 4317 */ -1,
/* 4318 */ 4325,
/* 4319 */ 50326,
/* 4320 */ -1,
/* 4321 */ -1,
/* 4322 */ 50059,
/* 4323 */ -4297,
/* 4324 */ -1,
/* 4325 */ 4332,
/* 4326 */ 50324,
/* 4327 */ -1,
/* 4328 */ -1,
/* 4329 */ 50059,
/* 4330 */ -4297,
/* 4331 */ -1,
/* 4332 */ 4339,
/* 4333 */ 50430,
/* 4334 */ -1,
/* 4335 */ -1,
/* 4336 */ 50059,
/* 4337 */ -4297,
/* 4338 */ -1,
/* 4339 */ 4346,
/* 4340 */ 50425,
/* 4341 */ -1,
/* 4342 */ -1,
/* 4343 */ 50059,
/* 4344 */ -4297,
/* 4345 */ -1,
/* 4346 */ 4353,
/* 4347 */ 50349,
/* 4348 */ -1,
/* 4349 */ -1,
/* 4350 */ 50059,
/* 4351 */ -4297,
/* 4352 */ -1,
/* 4353 */ 4360,
/* 4354 */ 50359,
/* 4355 */ 50058,
/* 4356 */ 50260,
/* 4357 */ 50059,
/* 4358 */ -4297,
/* 4359 */ -1,
/* 4360 */ 4367,
/* 4361 */ 50454,
/* 4362 */ 50058,
/* 4363 */ -1,
/* 4364 */ 50059,
/* 4365 */ -4297,
/* 4366 */ -1,
/* 4367 */ 4373,
/* 4368 */ 50422,
/* 4369 */ 50058,
/* 4370 */ -1,
/* 4371 */ -4297,
/* 4372 */ -1,
/* 4373 */ 4379,
/* 4374 */ 50397,
/* 4375 */ 50058,
/* 4376 */ -1,
/* 4377 */ -4297,
/* 4378 */ -1,
/* 4379 */ 4383,
/* 4380 */ -1,
/* 4381 */ -4297,
/* 4382 */ -1,
/* 4383 */ 4387,
/* 4384 */ -1,
/* 4385 */ -4297,
/* 4386 */ -1,
/* 4387 */ 0,
/* 4388 */ -1,
/* 4389 */ -4297,
/* 4390 */ -1,
/* 4391 */ 4395,
/* 4392 */ 50058,
/* 4393 */ -4391,
/* 4394 */ -1,
/* 4395 */ 0,
/* 4396 */ -4391,
/* 4397 */ -1,
/* 4398 */ 4402,
/* 4399 */ 50058,
/* 4400 */ -4398,
/* 4401 */ -1,
/* 4402 */ 0,
/* 4403 */ -4398,
/* 4404 */ -1,
/* 4405 */ 4409,
/* 4406 */ 50058,
/* 4407 */ -4405,
/* 4408 */ -1,
/* 4409 */ 0,
/* 4410 */ -4405,
/* 4411 */ -1,
/* 4412 */ 4416,
/* 4413 */ 50058,
/* 4414 */ -4412,
/* 4415 */ -1,
/* 4416 */ 0,
/* 4417 */ -4412,
/* 4418 */ -1,
/* 4419 */ 4423,
/* 4420 */ 50058,
/* 4421 */ -4419,
/* 4422 */ -1,
/* 4423 */ 0,
/* 4424 */ -4419,
/* 4425 */ -1,
/* 4426 */ 4430,
/* 4427 */ 50058,
/* 4428 */ -4426,
/* 4429 */ -1,
/* 4430 */ 0,
/* 4431 */ -4426,
/* 4432 */ -1,
/* 4433 */ 4437,
/* 4434 */ 50058,
/* 4435 */ -4433,
/* 4436 */ -1,
/* 4437 */ 0,
/* 4438 */ -4433,
/* 4439 */ -1,
/* 4440 */ 4446,
/* 4441 */ -1,
/* 4442 */ -1,
/* 4443 */ -1,
/* 4444 */ -4440,
/* 4445 */ -1,
/* 4446 */ 0,
/* 4447 */ -4440,
/* 4448 */ -1,
/* 4449 */ 4453,
/* 4450 */ 50044,
/* 4451 */ -4449,
/* 4452 */ -1,
/* 4453 */ 4457,
/* 4454 */ 50124,
/* 4455 */ -4449,
/* 4456 */ -1,
/* 4457 */ 0,
/* 4458 */ -4449,
/* 4459 */ -1,
/* 4460 */ 4464,
/* 4461 */ 50058,
/* 4462 */ -4460,
/* 4463 */ -1,
/* 4464 */ 0,
/* 4465 */ -4460,
/* 4466 */ -1,
/* 4467 */ 4473,
/* 4468 */ -1,
/* 4469 */ -1,
/* 4470 */ -1,
/* 4471 */ -4467,
/* 4472 */ -1,
/* 4473 */ 0,
/* 4474 */ -4467,
/* 4475 */ -1,
/* 4476 */ 4480,
/* 4477 */ 50124,
/* 4478 */ -4476,
/* 4479 */ -1,
/* 4480 */ 4484,
/* 4481 */ 50044,
/* 4482 */ -4476,
/* 4483 */ -1,
/* 4484 */ 0,
/* 4485 */ -4476,
/* 4486 */ -1,
/* 4487 */ 4496,
/* 4488 */ 50259,
/* 4489 */ 50058,
/* 4490 */ -1,
/* 4491 */ -1,
/* 4492 */ -1,
/* 4493 */ -1,
/* 4494 */ -4487,
/* 4495 */ -1,
/* 4496 */ 0,
/* 4497 */ -4487,
/* 4498 */ -1,
/* 4499 */ 4506,
/* 4500 */ 50091,
/* 4501 */ -1,
/* 4502 */ 50093,
/* 4503 */ -1,
/* 4504 */ -4499,
/* 4505 */ -1,
/* 4506 */ 4513,
/* 4507 */ 50470,
/* 4508 */ -1,
/* 4509 */ -1,
/* 4510 */ -1,
/* 4511 */ -4499,
/* 4512 */ -1,
/* 4513 */ 4519,
/* 4514 */ 50293,
/* 4515 */ -1,
/* 4516 */ -1,
/* 4517 */ -4499,
/* 4518 */ -1,
/* 4519 */ 0,
/* 4520 */ -4499,
/* 4521 */ -1,
/* 4522 */ 4526,
/* 4523 */ -1,
/* 4524 */ -4522,
/* 4525 */ -1,
/* 4526 */ 4532,
/* 4527 */ 50418,
/* 4528 */ 50371,
/* 4529 */ -1,
/* 4530 */ -4522,
/* 4531 */ -1,
/* 4532 */ 0,
/* 4533 */ 50418,
/* 4534 */ 50387,
/* 4535 */ -1,
/* 4536 */ -4522,
/* 4537 */ -1,
/* 4538 */ 4542,
/* 4539 */ -1,
/* 4540 */ -4538,
/* 4541 */ -1,
/* 4542 */ 0,
/* 4543 */ -4538,
/* 4544 */ -1,
/* 4545 */ 4549,
/* 4546 */ -1,
/* 4547 */ -4545,
/* 4548 */ -1,
/* 4549 */ 0,
/* 4550 */ -4545,
/* 4551 */ -1,
/* 4552 */ 4557,
/* 4553 */ 50398,
/* 4554 */ -1,
/* 4555 */ -4552,
/* 4556 */ -1,
/* 4557 */ 4565,
/* 4558 */ 50398,
/* 4559 */ 50418,
/* 4560 */ -1,
/* 4561 */ 50456,
/* 4562 */ -1,
/* 4563 */ -4552,
/* 4564 */ -1,
/* 4565 */ 4570,
/* 4566 */ 50398,
/* 4567 */ -1,
/* 4568 */ -4552,
/* 4569 */ -1,
/* 4570 */ 0,
/* 4571 */ -4552,
/* 4572 */ -1,
/* 4573 */ 4577,
/* 4574 */ 50044,
/* 4575 */ -4573,
/* 4576 */ -1,
/* 4577 */ 0,
/* 4578 */ -4573,
/* 4579 */ -1,
/* 4580 */ 4585,
/* 4581 */ -1,
/* 4582 */ -1,
/* 4583 */ -4580,
/* 4584 */ -1,
/* 4585 */ 0,
/* 4586 */ -4580,
/* 4587 */ -1,
/* 4588 */ 4593,
/* 4589 */ -1,
/* 4590 */ -1,
/* 4591 */ -4588,
/* 4592 */ -1,
/* 4593 */ 0,
/* 4594 */ -4588,
/* 4595 */ -1,
/* 4596 */ 0,
/* 4597 */ 50448,
/* 4598 */ 50257,
/* 4599 */ 50058,
/* 4600 */ -1,
/* 4601 */ -1,
/* 4602 */ 50123,
/* 4603 */ -1,
/* 4604 */ 50125,
/* 4605 */ -4596,
/* 4606 */ -1,
/* 4607 */ 4612,
/* 4608 */ 50352,
/* 4609 */ -1,
/* 4610 */ -4607,
/* 4611 */ -1,
/* 4612 */ 0,
/* 4613 */ -4607,
/* 4614 */ -1,
/* 4615 */ 4620,
/* 4616 */ -1,
/* 4617 */ -1,
/* 4618 */ -4615,
/* 4619 */ -1,
/* 4620 */ 0,
/* 4621 */ -4615,
/* 4622 */ -1,
/* 4623 */ 4630,
/* 4624 */ 50325,
/* 4625 */ -1,
/* 4626 */ 50257,
/* 4627 */ 50059,
/* 4628 */ -4623,
/* 4629 */ -1,
/* 4630 */ 4637,
/* 4631 */ 50352,
/* 4632 */ -1,
/* 4633 */ -1,
/* 4634 */ 50059,
/* 4635 */ -4623,
/* 4636 */ -1,
/* 4637 */ 4644,
/* 4638 */ 50389,
/* 4639 */ -1,
/* 4640 */ -1,
/* 4641 */ 50059,
/* 4642 */ -4623,
/* 4643 */ -1,
/* 4644 */ 4648,
/* 4645 */ -1,
/* 4646 */ -4623,
/* 4647 */ -1,
/* 4648 */ 4655,
/* 4649 */ -1,
/* 4650 */ -1,
/* 4651 */ -1,
/* 4652 */ 50059,
/* 4653 */ -4623,
/* 4654 */ -1,
/* 4655 */ 4662,
/* 4656 */ 50363,
/* 4657 */ -1,
/* 4658 */ -1,
/* 4659 */ 50059,
/* 4660 */ -4623,
/* 4661 */ -1,
/* 4662 */ 4669,
/* 4663 */ 50365,
/* 4664 */ -1,
/* 4665 */ -1,
/* 4666 */ 50059,
/* 4667 */ -4623,
/* 4668 */ -1,
/* 4669 */ 4676,
/* 4670 */ 50322,
/* 4671 */ -1,
/* 4672 */ -1,
/* 4673 */ 50059,
/* 4674 */ -4623,
/* 4675 */ -1,
/* 4676 */ 4683,
/* 4677 */ 50291,
/* 4678 */ -1,
/* 4679 */ -1,
/* 4680 */ 50059,
/* 4681 */ -4623,
/* 4682 */ -1,
/* 4683 */ 4690,
/* 4684 */ 50318,
/* 4685 */ -1,
/* 4686 */ -1,
/* 4687 */ 50059,
/* 4688 */ -4623,
/* 4689 */ -1,
/* 4690 */ 4697,
/* 4691 */ 50409,
/* 4692 */ -1,
/* 4693 */ -1,
/* 4694 */ 50059,
/* 4695 */ -4623,
/* 4696 */ -1,
/* 4697 */ 4705,
/* 4698 */ 50305,
/* 4699 */ 50373,
/* 4700 */ -1,
/* 4701 */ -1,
/* 4702 */ 50059,
/* 4703 */ -4623,
/* 4704 */ -1,
/* 4705 */ 4712,
/* 4706 */ 50430,
/* 4707 */ -1,
/* 4708 */ -1,
/* 4709 */ 50059,
/* 4710 */ -4623,
/* 4711 */ -1,
/* 4712 */ 4719,
/* 4713 */ 50434,
/* 4714 */ -1,
/* 4715 */ -1,
/* 4716 */ 50059,
/* 4717 */ -4623,
/* 4718 */ -1,
/* 4719 */ 4726,
/* 4720 */ 50388,
/* 4721 */ -1,
/* 4722 */ -1,
/* 4723 */ 50059,
/* 4724 */ -4623,
/* 4725 */ -1,
/* 4726 */ 4733,
/* 4727 */ 50276,
/* 4728 */ -1,
/* 4729 */ -1,
/* 4730 */ -1,
/* 4731 */ -4623,
/* 4732 */ -1,
/* 4733 */ 4741,
/* 4734 */ 50295,
/* 4735 */ -1,
/* 4736 */ -1,
/* 4737 */ -1,
/* 4738 */ 50059,
/* 4739 */ -4623,
/* 4740 */ -1,
/* 4741 */ 4749,
/* 4742 */ 50439,
/* 4743 */ 50091,
/* 4744 */ -1,
/* 4745 */ 50093,
/* 4746 */ 50059,
/* 4747 */ -4623,
/* 4748 */ -1,
/* 4749 */ 4759,
/* 4750 */ 50439,
/* 4751 */ 50091,
/* 4752 */ -1,
/* 4753 */ 50044,
/* 4754 */ -1,
/* 4755 */ 50093,
/* 4756 */ 50059,
/* 4757 */ -4623,
/* 4758 */ -1,
/* 4759 */ 4771,
/* 4760 */ 50439,
/* 4761 */ 50091,
/* 4762 */ -1,
/* 4763 */ 50044,
/* 4764 */ -1,
/* 4765 */ 50044,
/* 4766 */ -1,
/* 4767 */ 50093,
/* 4768 */ 50059,
/* 4769 */ -4623,
/* 4770 */ -1,
/* 4771 */ 4785,
/* 4772 */ 50439,
/* 4773 */ 50091,
/* 4774 */ -1,
/* 4775 */ 50044,
/* 4776 */ -1,
/* 4777 */ 50044,
/* 4778 */ -1,
/* 4779 */ 50044,
/* 4780 */ -1,
/* 4781 */ 50093,
/* 4782 */ 50059,
/* 4783 */ -4623,
/* 4784 */ -1,
/* 4785 */ 4792,
/* 4786 */ 50423,
/* 4787 */ -1,
/* 4788 */ -1,
/* 4789 */ 50059,
/* 4790 */ -4623,
/* 4791 */ -1,
/* 4792 */ 4799,
/* 4793 */ 50368,
/* 4794 */ -1,
/* 4795 */ -1,
/* 4796 */ 50059,
/* 4797 */ -4623,
/* 4798 */ -1,
/* 4799 */ 4807,
/* 4800 */ 50345,
/* 4801 */ -1,
/* 4802 */ -1,
/* 4803 */ -1,
/* 4804 */ 50059,
/* 4805 */ -4623,
/* 4806 */ -1,
/* 4807 */ 4814,
/* 4808 */ 50436,
/* 4809 */ -1,
/* 4810 */ -1,
/* 4811 */ 50059,
/* 4812 */ -4623,
/* 4813 */ -1,
/* 4814 */ 4821,
/* 4815 */ 50329,
/* 4816 */ -1,
/* 4817 */ -1,
/* 4818 */ 50059,
/* 4819 */ -4623,
/* 4820 */ -1,
/* 4821 */ 4828,
/* 4822 */ 50408,
/* 4823 */ -1,
/* 4824 */ -1,
/* 4825 */ 50059,
/* 4826 */ -4623,
/* 4827 */ -1,
/* 4828 */ 4834,
/* 4829 */ 50349,
/* 4830 */ -1,
/* 4831 */ -1,
/* 4832 */ -4623,
/* 4833 */ -1,
/* 4834 */ 4838,
/* 4835 */ -1,
/* 4836 */ -4623,
/* 4837 */ -1,
/* 4838 */ 4842,
/* 4839 */ -1,
/* 4840 */ -4623,
/* 4841 */ -1,
/* 4842 */ 4846,
/* 4843 */ -1,
/* 4844 */ -4623,
/* 4845 */ -1,
/* 4846 */ 0,
/* 4847 */ -1,
/* 4848 */ -4623,
/* 4849 */ -1,
/* 4850 */ 4854,
/* 4851 */ 50058,
/* 4852 */ -4850,
/* 4853 */ -1,
/* 4854 */ 0,
/* 4855 */ -4850,
/* 4856 */ -1,
/* 4857 */ 4861,
/* 4858 */ 50058,
/* 4859 */ -4857,
/* 4860 */ -1,
/* 4861 */ 0,
/* 4862 */ -4857,
/* 4863 */ -1,
/* 4864 */ 4868,
/* 4865 */ 50058,
/* 4866 */ -4864,
/* 4867 */ -1,
/* 4868 */ 0,
/* 4869 */ -4864,
/* 4870 */ -1,
/* 4871 */ 4875,
/* 4872 */ 50373,
/* 4873 */ -4871,
/* 4874 */ -1,
/* 4875 */ 0,
/* 4876 */ 50306,
/* 4877 */ -4871,
/* 4878 */ -1,
/* 4879 */ 4883,
/* 4880 */ 50058,
/* 4881 */ -4879,
/* 4882 */ -1,
/* 4883 */ 0,
/* 4884 */ -4879,
/* 4885 */ -1,
/* 4886 */ 4890,
/* 4887 */ 50058,
/* 4888 */ -4886,
/* 4889 */ -1,
/* 4890 */ 0,
/* 4891 */ -4886,
/* 4892 */ -1,
/* 4893 */ 4897,
/* 4894 */ 50058,
/* 4895 */ -4893,
/* 4896 */ -1,
/* 4897 */ 0,
/* 4898 */ -4893,
/* 4899 */ -1,
/* 4900 */ 4904,
/* 4901 */ 50058,
/* 4902 */ -4900,
/* 4903 */ -1,
/* 4904 */ 0,
/* 4905 */ -4900,
/* 4906 */ -1,
/* 4907 */ 4911,
/* 4908 */ 50058,
/* 4909 */ -4907,
/* 4910 */ -1,
/* 4911 */ 0,
/* 4912 */ -4907,
/* 4913 */ -1,
/* 4914 */ 4918,
/* 4915 */ 50058,
/* 4916 */ -4914,
/* 4917 */ -1,
/* 4918 */ 0,
/* 4919 */ -4914,
/* 4920 */ -1,
/* 4921 */ 4925,
/* 4922 */ 50058,
/* 4923 */ -4921,
/* 4924 */ -1,
/* 4925 */ 0,
/* 4926 */ -4921,
/* 4927 */ -1,
/* 4928 */ 4932,
/* 4929 */ 50058,
/* 4930 */ -4928,
/* 4931 */ -1,
/* 4932 */ 0,
/* 4933 */ -4928,
/* 4934 */ -1,
/* 4935 */ 4939,
/* 4936 */ 50058,
/* 4937 */ -4935,
/* 4938 */ -1,
/* 4939 */ 0,
/* 4940 */ -4935,
/* 4941 */ -1,
/* 4942 */ 4946,
/* 4943 */ 50058,
/* 4944 */ -4942,
/* 4945 */ -1,
/* 4946 */ 0,
/* 4947 */ -4942,
/* 4948 */ -1,
/* 4949 */ 4953,
/* 4950 */ 50058,
/* 4951 */ -4949,
/* 4952 */ -1,
/* 4953 */ 0,
/* 4954 */ -4949,
/* 4955 */ -1,
/* 4956 */ 4960,
/* 4957 */ 50058,
/* 4958 */ -4956,
/* 4959 */ -1,
/* 4960 */ 0,
/* 4961 */ -4956,
/* 4962 */ -1,
/* 4963 */ 4967,
/* 4964 */ 50044,
/* 4965 */ -4963,
/* 4966 */ -1,
/* 4967 */ 0,
/* 4968 */ 50059,
/* 4969 */ -4963,
/* 4970 */ -1,
/* 4971 */ 4975,
/* 4972 */ 50058,
/* 4973 */ -4971,
/* 4974 */ -1,
/* 4975 */ 0,
/* 4976 */ -4971,
/* 4977 */ -1,
/* 4978 */ 4984,
/* 4979 */ 50044,
/* 4980 */ -1,
/* 4981 */ -1,
/* 4982 */ -4978,
/* 4983 */ -1,
/* 4984 */ 0,
/* 4985 */ -4978,
/* 4986 */ -1,
/* 4987 */ 4991,
/* 4988 */ 50058,
/* 4989 */ -4987,
/* 4990 */ -1,
/* 4991 */ 0,
/* 4992 */ -4987,
/* 4993 */ -1,
/* 4994 */ 5000,
/* 4995 */ -1,
/* 4996 */ -1,
/* 4997 */ -1,
/* 4998 */ -4994,
/* 4999 */ -1,
/* 5000 */ 0,
/* 5001 */ -4994,
/* 5002 */ -1,
/* 5003 */ 5007,
/* 5004 */ -1,
/* 5005 */ -5003,
/* 5006 */ -1,
/* 5007 */ 0,
/* 5008 */ -1,
/* 5009 */ -1,
/* 5010 */ -5003,
/* 5011 */ -1,
/* 5012 */ 5016,
/* 5013 */ 50126,
/* 5014 */ -5012,
/* 5015 */ -1,
/* 5016 */ 0,
/* 5017 */ 50033,
/* 5018 */ -5012,
/* 5019 */ -1,
/* 5020 */ 5024,
/* 5021 */ 50044,
/* 5022 */ -5020,
/* 5023 */ -1,
/* 5024 */ 0,
/* 5025 */ -5020,
/* 5026 */ -1,
/* 5027 */ 5031,
/* 5028 */ 50058,
/* 5029 */ -5027,
/* 5030 */ -1,
/* 5031 */ 0,
/* 5032 */ -5027,
/* 5033 */ -1,
/* 5034 */ 5040,
/* 5035 */ -1,
/* 5036 */ -1,
/* 5037 */ -1,
/* 5038 */ -5034,
/* 5039 */ -1,
/* 5040 */ 0,
/* 5041 */ -5034,
/* 5042 */ -1,
/* 5043 */ 5047,
/* 5044 */ -1,
/* 5045 */ -5043,
/* 5046 */ -1,
/* 5047 */ 0,
/* 5048 */ -1,
/* 5049 */ -1,
/* 5050 */ -5043,
/* 5051 */ -1,
/* 5052 */ 5056,
/* 5053 */ 50126,
/* 5054 */ -5052,
/* 5055 */ -1,
/* 5056 */ 0,
/* 5057 */ 50033,
/* 5058 */ -5052,
/* 5059 */ -1,
/* 5060 */ 5064,
/* 5061 */ 50044,
/* 5062 */ -5060,
/* 5063 */ -1,
/* 5064 */ 0,
/* 5065 */ -5060,
/* 5066 */ -1,
/* 5067 */ 5071,
/* 5068 */ 50058,
/* 5069 */ -5067,
/* 5070 */ -1,
/* 5071 */ 0,
/* 5072 */ -5067,
/* 5073 */ -1,
/* 5074 */ 5080,
/* 5075 */ -1,
/* 5076 */ 50044,
/* 5077 */ -1,
/* 5078 */ -5074,
/* 5079 */ -1,
/* 5080 */ 0,
/* 5081 */ -5074,
/* 5082 */ -1,
/* 5083 */ 5087,
/* 5084 */ 50058,
/* 5085 */ -5083,
/* 5086 */ -1,
/* 5087 */ 0,
/* 5088 */ -5083,
/* 5089 */ -1,
/* 5090 */ 5096,
/* 5091 */ -1,
/* 5092 */ -1,
/* 5093 */ -1,
/* 5094 */ -5090,
/* 5095 */ -1,
/* 5096 */ 0,
/* 5097 */ -5090,
/* 5098 */ -1,
/* 5099 */ 5103,
/* 5100 */ 50044,
/* 5101 */ -5099,
/* 5102 */ -1,
/* 5103 */ 0,
/* 5104 */ -5099,
/* 5105 */ -1,
/* 5106 */ 5110,
/* 5107 */ 50058,
/* 5108 */ -5106,
/* 5109 */ -1,
/* 5110 */ 0,
/* 5111 */ -5106,
/* 5112 */ -1,
/* 5113 */ 5119,
/* 5114 */ -1,
/* 5115 */ -1,
/* 5116 */ -1,
/* 5117 */ -5113,
/* 5118 */ -1,
/* 5119 */ 0,
/* 5120 */ -5113,
/* 5121 */ -1,
/* 5122 */ 5126,
/* 5123 */ 50044,
/* 5124 */ -5122,
/* 5125 */ -1,
/* 5126 */ 0,
/* 5127 */ -5122,
/* 5128 */ -1,
/* 5129 */ 5133,
/* 5130 */ 50058,
/* 5131 */ -5129,
/* 5132 */ -1,
/* 5133 */ 0,
/* 5134 */ -5129,
/* 5135 */ -1,
/* 5136 */ 5142,
/* 5137 */ -1,
/* 5138 */ -1,
/* 5139 */ -1,
/* 5140 */ -5136,
/* 5141 */ -1,
/* 5142 */ 0,
/* 5143 */ -5136,
/* 5144 */ -1,
/* 5145 */ 5149,
/* 5146 */ 50044,
/* 5147 */ -5145,
/* 5148 */ -1,
/* 5149 */ 0,
/* 5150 */ -5145,
/* 5151 */ -1,
/* 5152 */ 5156,
/* 5153 */ 50058,
/* 5154 */ -5152,
/* 5155 */ -1,
/* 5156 */ 0,
/* 5157 */ -5152,
/* 5158 */ -1,
/* 5159 */ 5165,
/* 5160 */ -1,
/* 5161 */ -1,
/* 5162 */ -1,
/* 5163 */ -5159,
/* 5164 */ -1,
/* 5165 */ 0,
/* 5166 */ -5159,
/* 5167 */ -1,
/* 5168 */ 5172,
/* 5169 */ -1,
/* 5170 */ -5168,
/* 5171 */ -1,
/* 5172 */ 0,
/* 5173 */ -1,
/* 5174 */ -1,
/* 5175 */ -5168,
/* 5176 */ -1,
/* 5177 */ 5181,
/* 5178 */ 50126,
/* 5179 */ -5177,
/* 5180 */ -1,
/* 5181 */ 0,
/* 5182 */ 50033,
/* 5183 */ -5177,
/* 5184 */ -1,
/* 5185 */ 5189,
/* 5186 */ 50044,
/* 5187 */ -5185,
/* 5188 */ -1,
/* 5189 */ 0,
/* 5190 */ 50059,
/* 5191 */ -5185,
/* 5192 */ -1,
/* 5193 */ 5203,
/* 5194 */ -1,
/* 5195 */ 50352,
/* 5196 */ -1,
/* 5197 */ -1,
/* 5198 */ -1,
/* 5199 */ -1,
/* 5200 */ -1,
/* 5201 */ -5193,
/* 5202 */ -1,
/* 5203 */ 0,
/* 5204 */ -1,
/* 5205 */ 50352,
/* 5206 */ -1,
/* 5207 */ -1,
/* 5208 */ -5193,
/* 5209 */ -1,
/* 5210 */ 5214,
/* 5211 */ -1,
/* 5212 */ -5210,
/* 5213 */ -1,
/* 5214 */ 5219,
/* 5215 */ -1,
/* 5216 */ 50037,
/* 5217 */ -5210,
/* 5218 */ -1,
/* 5219 */ 0,
/* 5220 */ -5210,
/* 5221 */ -1,
/* 5222 */ 5226,
/* 5223 */ -1,
/* 5224 */ -5222,
/* 5225 */ -1,
/* 5226 */ 0,
/* 5227 */ -5222,
/* 5228 */ -1,
/* 5229 */ 5233,
/* 5230 */ 50318,
/* 5231 */ -5229,
/* 5232 */ -1,
/* 5233 */ 0,
/* 5234 */ -5229,
/* 5235 */ -1,
/* 5236 */ 5243,
/* 5237 */ 50040,
/* 5238 */ -1,
/* 5239 */ -1,
/* 5240 */ 50041,
/* 5241 */ -5236,
/* 5242 */ -1,
/* 5243 */ 0,
/* 5244 */ -5236,
/* 5245 */ -1,
/* 5246 */ 5250,
/* 5247 */ 50321,
/* 5248 */ -5246,
/* 5249 */ -1,
/* 5250 */ 5254,
/* 5251 */ 50306,
/* 5252 */ -5246,
/* 5253 */ -1,
/* 5254 */ 0,
/* 5255 */ 50373,
/* 5256 */ -5246,
/* 5257 */ -1,
/* 5258 */ 5267,
/* 5259 */ 50287,
/* 5260 */ -1,
/* 5261 */ -1,
/* 5262 */ -1,
/* 5263 */ -1,
/* 5264 */ -1,
/* 5265 */ -5258,
/* 5266 */ -1,
/* 5267 */ 0,
/* 5268 */ -5258,
/* 5269 */ -1,
/* 5270 */ 5274,
/* 5271 */ -1,
/* 5272 */ -5270,
/* 5273 */ -1,
/* 5274 */ 0,
/* 5275 */ -5270,
/* 5276 */ -1,
/* 5277 */ 5281,
/* 5278 */ -1,
/* 5279 */ -5277,
/* 5280 */ -1,
/* 5281 */ 0,
/* 5282 */ -5277,
/* 5283 */ -1,
/* 5284 */ 5288,
/* 5285 */ 50318,
/* 5286 */ -5284,
/* 5287 */ -1,
/* 5288 */ 0,
/* 5289 */ -5284,
/* 5290 */ -1,
/* 5291 */ 5298,
/* 5292 */ 50040,
/* 5293 */ -1,
/* 5294 */ -1,
/* 5295 */ 50041,
/* 5296 */ -5291,
/* 5297 */ -1,
/* 5298 */ 0,
/* 5299 */ -5291,
/* 5300 */ -1,
/* 5301 */ 5305,
/* 5302 */ 50321,
/* 5303 */ -5301,
/* 5304 */ -1,
/* 5305 */ 5309,
/* 5306 */ 50306,
/* 5307 */ -5301,
/* 5308 */ -1,
/* 5309 */ 0,
/* 5310 */ 50373,
/* 5311 */ -5301,
/* 5312 */ -1,
/* 5313 */ 5317,
/* 5314 */ -1,
/* 5315 */ -5313,
/* 5316 */ -1,
/* 5317 */ 0,
/* 5318 */ -5313,
/* 5319 */ -1,
/* 5320 */ 0,
/* 5321 */ 50261,
/* 5322 */ -5320,
/* 5323 */ -1,
/* 5324 */ 0,
/* 5325 */ 50340,
/* 5326 */ 50257,
/* 5327 */ 50058,
/* 5328 */ -1,
/* 5329 */ 50123,
/* 5330 */ -1,
/* 5331 */ 50125,
/* 5332 */ -5324,
/* 5333 */ -1,
/* 5334 */ 5339,
/* 5335 */ -1,
/* 5336 */ -1,
/* 5337 */ -5334,
/* 5338 */ -1,
/* 5339 */ 0,
/* 5340 */ -5334,
/* 5341 */ -1,
/* 5342 */ 5349,
/* 5343 */ 50325,
/* 5344 */ -1,
/* 5345 */ 50257,
/* 5346 */ 50059,
/* 5347 */ -5342,
/* 5348 */ -1,
/* 5349 */ 5356,
/* 5350 */ 50382,
/* 5351 */ -1,
/* 5352 */ -1,
/* 5353 */ 50059,
/* 5354 */ -5342,
/* 5355 */ -1,
/* 5356 */ 5363,
/* 5357 */ 50309,
/* 5358 */ -1,
/* 5359 */ -1,
/* 5360 */ 50059,
/* 5361 */ -5342,
/* 5362 */ -1,
/* 5363 */ 0,
/* 5364 */ 50465,
/* 5365 */ -1,
/* 5366 */ -1,
/* 5367 */ 50037,
/* 5368 */ 50059,
/* 5369 */ -5342,
/* 5370 */ -1,
/* 5371 */ 5375,
/* 5372 */ 50058,
/* 5373 */ -5371,
/* 5374 */ -1,
/* 5375 */ 0,
/* 5376 */ -5371,
/* 5377 */ -1,
/* 5378 */ 5382,
/* 5379 */ 50058,
/* 5380 */ -5378,
/* 5381 */ -1,
/* 5382 */ 0,
/* 5383 */ -5378,
/* 5384 */ -1,
/* 5385 */ 5389,
/* 5386 */ 50058,
/* 5387 */ -5385,
/* 5388 */ -1,
/* 5389 */ 0,
/* 5390 */ -5385,
/* 5391 */ -1,
/* 5392 */ 5396,
/* 5393 */ 50058,
/* 5394 */ -5392,
/* 5395 */ -1,
/* 5396 */ 0,
/* 5397 */ -5392,
/* 5398 */ -1,
/* 5399 */ 0,
/* 5400 */ 50335,
/* 5401 */ 50257,
/* 5402 */ -1,
/* 5403 */ 50123,
/* 5404 */ -1,
/* 5405 */ 50125,
/* 5406 */ -5399,
/* 5407 */ -1,
/* 5408 */ 5413,
/* 5409 */ 50058,
/* 5410 */ -1,
/* 5411 */ -5408,
/* 5412 */ -1,
/* 5413 */ 0,
/* 5414 */ -5408,
/* 5415 */ -1,
/* 5416 */ 5421,
/* 5417 */ -1,
/* 5418 */ -1,
/* 5419 */ -5416,
/* 5420 */ -1,
/* 5421 */ 0,
/* 5422 */ -5416,
/* 5423 */ -1,
/* 5424 */ 5431,
/* 5425 */ 50449,
/* 5426 */ 50058,
/* 5427 */ -1,
/* 5428 */ 50059,
/* 5429 */ -5424,
/* 5430 */ -1,
/* 5431 */ 5438,
/* 5432 */ 50324,
/* 5433 */ 50058,
/* 5434 */ -1,
/* 5435 */ 50059,
/* 5436 */ -5424,
/* 5437 */ -1,
/* 5438 */ 5445,
/* 5439 */ 50465,
/* 5440 */ 50058,
/* 5441 */ -1,
/* 5442 */ 50059,
/* 5443 */ -5424,
/* 5444 */ -1,
/* 5445 */ 5452,
/* 5446 */ 50306,
/* 5447 */ 50058,
/* 5448 */ -1,
/* 5449 */ 50059,
/* 5450 */ -5424,
/* 5451 */ -1,
/* 5452 */ 5460,
/* 5453 */ 50306,
/* 5454 */ 50058,
/* 5455 */ -1,
/* 5456 */ 50043,
/* 5457 */ 50059,
/* 5458 */ -5424,
/* 5459 */ -1,
/* 5460 */ 5469,
/* 5461 */ 50306,
/* 5462 */ 50058,
/* 5463 */ -1,
/* 5464 */ -1,
/* 5465 */ -1,
/* 5466 */ 50059,
/* 5467 */ -5424,
/* 5468 */ -1,
/* 5469 */ 5476,
/* 5470 */ 50284,
/* 5471 */ 50058,
/* 5472 */ -1,
/* 5473 */ 50059,
/* 5474 */ -5424,
/* 5475 */ -1,
/* 5476 */ 5483,
/* 5477 */ 50405,
/* 5478 */ 50058,
/* 5479 */ -1,
/* 5480 */ 50059,
/* 5481 */ -5424,
/* 5482 */ -1,
/* 5483 */ 5490,
/* 5484 */ 50349,
/* 5485 */ -1,
/* 5486 */ -1,
/* 5487 */ 50059,
/* 5488 */ -5424,
/* 5489 */ -1,
/* 5490 */ 5494,
/* 5491 */ -1,
/* 5492 */ -5424,
/* 5493 */ -1,
/* 5494 */ 5498,
/* 5495 */ -1,
/* 5496 */ -5424,
/* 5497 */ -1,
/* 5498 */ 5502,
/* 5499 */ -1,
/* 5500 */ -5424,
/* 5501 */ -1,
/* 5502 */ 0,
/* 5503 */ -1,
/* 5504 */ -5424,
/* 5505 */ -1,
/* 5506 */ 5510,
/* 5507 */ 50456,
/* 5508 */ -5506,
/* 5509 */ -1,
/* 5510 */ 0,
/* 5511 */ 50045,
/* 5512 */ -5506,
/* 5513 */ -1,
/* 5514 */ 5519,
/* 5515 */ -1,
/* 5516 */ -1,
/* 5517 */ -5514,
/* 5518 */ -1,
/* 5519 */ 0,
/* 5520 */ -5514,
/* 5521 */ -1,
/* 5522 */ 5526,
/* 5523 */ 50058,
/* 5524 */ -5522,
/* 5525 */ -1,
/* 5526 */ 0,
/* 5527 */ -5522,
/* 5528 */ -1,
/* 5529 */ 5535,
/* 5530 */ -1,
/* 5531 */ -1,
/* 5532 */ -1,
/* 5533 */ -5529,
/* 5534 */ -1,
/* 5535 */ 0,
/* 5536 */ -5529,
/* 5537 */ -1,
/* 5538 */ 5542,
/* 5539 */ 50044,
/* 5540 */ -5538,
/* 5541 */ -1,
/* 5542 */ 5546,
/* 5543 */ 50124,
/* 5544 */ -5538,
/* 5545 */ -1,
/* 5546 */ 0,
/* 5547 */ -5538,
/* 5548 */ -1,
/* 5549 */ 0,
/* 5550 */ -1,
/* 5551 */ -1,
/* 5552 */ 50398,
/* 5553 */ -1,
/* 5554 */ -1,
/* 5555 */ -1,
/* 5556 */ -5549,
/* 5557 */ -1,
/* 5558 */ 5562,
/* 5559 */ 50042,
/* 5560 */ -5558,
/* 5561 */ -1,
/* 5562 */ 5566,
/* 5563 */ 50334,
/* 5564 */ -5558,
/* 5565 */ -1,
/* 5566 */ 0,
/* 5567 */ 50124,
/* 5568 */ -5558,
/* 5569 */ -1,
/* 5570 */ 5576,
/* 5571 */ -1,
/* 5572 */ 50037,
/* 5573 */ -1,
/* 5574 */ -5570,
/* 5575 */ -1,
/* 5576 */ 5585,
/* 5577 */ 50060,
/* 5578 */ 50306,
/* 5579 */ -1,
/* 5580 */ 50043,
/* 5581 */ 50062,
/* 5582 */ -1,
/* 5583 */ -5570,
/* 5584 */ -1,
/* 5585 */ 5592,
/* 5586 */ 50091,
/* 5587 */ -1,
/* 5588 */ 50093,
/* 5589 */ -1,
/* 5590 */ -5570,
/* 5591 */ -1,
/* 5592 */ 5599,
/* 5593 */ 50123,
/* 5594 */ -1,
/* 5595 */ 50125,
/* 5596 */ -1,
/* 5597 */ -5570,
/* 5598 */ -1,
/* 5599 */ 5606,
/* 5600 */ -1,
/* 5601 */ -1,
/* 5602 */ -1,
/* 5603 */ -1,
/* 5604 */ -5570,
/* 5605 */ -1,
/* 5606 */ 5611,
/* 5607 */ -1,
/* 5608 */ -1,
/* 5609 */ -5570,
/* 5610 */ -1,
/* 5611 */ 5617,
/* 5612 */ -1,
/* 5613 */ 50043,
/* 5614 */ -1,
/* 5615 */ -5570,
/* 5616 */ -1,
/* 5617 */ 5622,
/* 5618 */ -1,
/* 5619 */ -1,
/* 5620 */ -5570,
/* 5621 */ -1,
/* 5622 */ 0,
/* 5623 */ -5570,
/* 5624 */ -1,
/* 5625 */ 5629,
/* 5626 */ 50045,
/* 5627 */ -5625,
/* 5628 */ -1,
/* 5629 */ 0,
/* 5630 */ 50456,
/* 5631 */ -5625,
/* 5632 */ -1,
/* 5633 */ 5637,
/* 5634 */ -1,
/* 5635 */ -5633,
/* 5636 */ -1,
/* 5637 */ 5641,
/* 5638 */ -1,
/* 5639 */ -5633,
/* 5640 */ -1,
/* 5641 */ 0,
/* 5642 */ 50288,
/* 5643 */ -1,
/* 5644 */ -5633,
/* 5645 */ -1,
/* 5646 */ 5650,
/* 5647 */ -1,
/* 5648 */ -5646,
/* 5649 */ -1,
/* 5650 */ 5654,
/* 5651 */ -1,
/* 5652 */ -5646,
/* 5653 */ -1,
/* 5654 */ 5659,
/* 5655 */ 50288,
/* 5656 */ -1,
/* 5657 */ -5646,
/* 5658 */ -1,
/* 5659 */ 0,
/* 5660 */ -5646,
/* 5661 */ -1,
/* 5662 */ 5669,
/* 5663 */ 50091,
/* 5664 */ -1,
/* 5665 */ -1,
/* 5666 */ 50093,
/* 5667 */ -5662,
/* 5668 */ -1,
/* 5669 */ 0,
/* 5670 */ -5662,
/* 5671 */ -1,
/* 5672 */ 5678,
/* 5673 */ -1,
/* 5674 */ -1,
/* 5675 */ -1,
/* 5676 */ -5672,
/* 5677 */ -1,
/* 5678 */ 0,
/* 5679 */ -5672,
/* 5680 */ -1,
/* 5681 */ 5685,
/* 5682 */ 50124,
/* 5683 */ -5681,
/* 5684 */ -1,
/* 5685 */ 0,
/* 5686 */ 50044,
/* 5687 */ -5681,
/* 5688 */ -1,
/* 5689 */ 0,
/* 5690 */ 50298,
/* 5691 */ 50257,
/* 5692 */ 50058,
/* 5693 */ -1,
/* 5694 */ 50123,
/* 5695 */ -1,
/* 5696 */ 50125,
/* 5697 */ -5689,
/* 5698 */ -1,
/* 5699 */ 5704,
/* 5700 */ -1,
/* 5701 */ -1,
/* 5702 */ -5699,
/* 5703 */ -1,
/* 5704 */ 0,
/* 5705 */ -5699,
/* 5706 */ -1,
/* 5707 */ 5714,
/* 5708 */ 50435,
/* 5709 */ -1,
/* 5710 */ -1,
/* 5711 */ 50059,
/* 5712 */ -5707,
/* 5713 */ -1,
/* 5714 */ 5721,
/* 5715 */ 50349,
/* 5716 */ -1,
/* 5717 */ -1,
/* 5718 */ 50059,
/* 5719 */ -5707,
/* 5720 */ -1,
/* 5721 */ 5725,
/* 5722 */ -1,
/* 5723 */ -5707,
/* 5724 */ -1,
/* 5725 */ 5729,
/* 5726 */ -1,
/* 5727 */ -5707,
/* 5728 */ -1,
/* 5729 */ 0,
/* 5730 */ -1,
/* 5731 */ -5707,
/* 5732 */ -1,
/* 5733 */ 5737,
/* 5734 */ 50058,
/* 5735 */ -5733,
/* 5736 */ -1,
/* 5737 */ 0,
/* 5738 */ -5733,
/* 5739 */ -1,
/* 5740 */ 5744,
/* 5741 */ 50058,
/* 5742 */ -5740,
/* 5743 */ -1,
/* 5744 */ 0,
/* 5745 */ -5740,
/* 5746 */ -1,
/* 5747 */ 5753,
/* 5748 */ -1,
/* 5749 */ -1,
/* 5750 */ -1,
/* 5751 */ -5747,
/* 5752 */ -1,
/* 5753 */ 0,
/* 5754 */ -5747,
/* 5755 */ -1,
/* 5756 */ 5760,
/* 5757 */ 50124,
/* 5758 */ -5756,
/* 5759 */ -1,
/* 5760 */ 5764,
/* 5761 */ 50044,
/* 5762 */ -5756,
/* 5763 */ -1,
/* 5764 */ 0,
/* 5765 */ -5756,
/* 5766 */ -1,
/* 5767 */ 0,
/* 5768 */ 50450,
/* 5769 */ 50257,
/* 5770 */ -1,
/* 5771 */ 50059,
/* 5772 */ -5767,
/* 5773 */ -1,
/* 5774 */ 5779,
/* 5775 */ 50257,
/* 5776 */ -1,
/* 5777 */ -5774,
/* 5778 */ -1,
/* 5779 */ 5784,
/* 5780 */ -1,
/* 5781 */ -1,
/* 5782 */ -5774,
/* 5783 */ -1,
/* 5784 */ 5789,
/* 5785 */ 50040,
/* 5786 */ -1,
/* 5787 */ -5774,
/* 5788 */ -1,
/* 5789 */ 5794,
/* 5790 */ 50041,
/* 5791 */ -1,
/* 5792 */ -5774,
/* 5793 */ -1,
/* 5794 */ 5799,
/* 5795 */ 50124,
/* 5796 */ -1,
/* 5797 */ -5774,
/* 5798 */ -1,
/* 5799 */ 5804,
/* 5800 */ 50059,
/* 5801 */ -1,
/* 5802 */ -5774,
/* 5803 */ -1,
/* 5804 */ 5812,
/* 5805 */ 50091,
/* 5806 */ -1,
/* 5807 */ 50093,
/* 5808 */ 50058,
/* 5809 */ -1,
/* 5810 */ -5774,
/* 5811 */ -1,
/* 5812 */ 5819,
/* 5813 */ 50091,
/* 5814 */ -1,
/* 5815 */ 50093,
/* 5816 */ -1,
/* 5817 */ -5774,
/* 5818 */ -1,
/* 5819 */ 0,
/* 5820 */ -5774,
/* 5821 */ -1,
/* 5822 */ 0,
/* 5823 */ 50261,
/* 5824 */ -5822,
/* 5825 */ -1,
/* 5826 */ 5830,
/* 5827 */ 50264,
/* 5828 */ -5826,
/* 5829 */ -1,
/* 5830 */ 5834,
/* 5831 */ 50267,
/* 5832 */ -5826,
/* 5833 */ -1,
/* 5834 */ 5838,
/* 5835 */ 50263,
/* 5836 */ -5826,
/* 5837 */ -1,
/* 5838 */ 5842,
/* 5839 */ 50271,
/* 5840 */ -5826,
/* 5841 */ -1,
/* 5842 */ 5846,
/* 5843 */ 50272,
/* 5844 */ -5826,
/* 5845 */ -1,
/* 5846 */ 5850,
/* 5847 */ 50273,
/* 5848 */ -5826,
/* 5849 */ -1,
/* 5850 */ 0,
/* 5851 */ 50473,
/* 5852 */ -5826,
/* 5853 */ -1,
/* 5854 */ 0,
/* 5855 */ 50446,
/* 5856 */ -1,
/* 5857 */ -1,
/* 5858 */ 50262,
/* 5859 */ 50258,
/* 5860 */ 50040,
/* 5861 */ -1,
/* 5862 */ -1,
/* 5863 */ 50041,
/* 5864 */ 50059,
/* 5865 */ -5854,
/* 5866 */ -1,
/* 5867 */ 5875,
/* 5868 */ -1,
/* 5869 */ -1,
/* 5870 */ -1,
/* 5871 */ -1,
/* 5872 */ -1,
/* 5873 */ -5867,
/* 5874 */ -1,
/* 5875 */ 0,
/* 5876 */ -5867,
/* 5877 */ -1,
/* 5878 */ 5883,
/* 5879 */ 50058,
/* 5880 */ -1,
/* 5881 */ -5878,
/* 5882 */ -1,
/* 5883 */ 0,
/* 5884 */ -5878,
/* 5885 */ -1,
/* 5886 */ 5890,
/* 5887 */ 50258,
/* 5888 */ -5886,
/* 5889 */ -1,
/* 5890 */ 0,
/* 5891 */ -5886,
/* 5892 */ -1,
/* 5893 */ 5898,
/* 5894 */ 50061,
/* 5895 */ -1,
/* 5896 */ -5893,
/* 5897 */ -1,
/* 5898 */ 0,
/* 5899 */ -5893,
/* 5900 */ -1,
/* 5901 */ 5910,
/* 5902 */ 50044,
/* 5903 */ -1,
/* 5904 */ -1,
/* 5905 */ -1,
/* 5906 */ -1,
/* 5907 */ -1,
/* 5908 */ -5901,
/* 5909 */ -1,
/* 5910 */ 0,
/* 5911 */ -5901,
/* 5912 */ -1,
/* 5913 */ 5918,
/* 5914 */ 50058,
/* 5915 */ -1,
/* 5916 */ -5913,
/* 5917 */ -1,
/* 5918 */ 0,
/* 5919 */ -5913,
/* 5920 */ -1,
/* 5921 */ 5925,
/* 5922 */ 50258,
/* 5923 */ -5921,
/* 5924 */ -1,
/* 5925 */ 0,
/* 5926 */ -5921,
/* 5927 */ -1,
/* 5928 */ 5933,
/* 5929 */ 50061,
/* 5930 */ -1,
/* 5931 */ -5928,
/* 5932 */ -1,
/* 5933 */ 0,
/* 5934 */ -5928,
/* 5935 */ -1,
/* 5936 */ 5941,
/* 5937 */ 50044,
/* 5938 */ 50494,
/* 5939 */ -5936,
/* 5940 */ -1,
/* 5941 */ 0,
/* 5942 */ -5936,
/* 5943 */ -1,
/* 5944 */ 0,
/* 5945 */ 50446,
/* 5946 */ -1,
/* 5947 */ 50258,
/* 5948 */ 50040,
/* 5949 */ -1,
/* 5950 */ -1,
/* 5951 */ 50041,
/* 5952 */ 50059,
/* 5953 */ -5944,
/* 5954 */ -1,
/* 5955 */ 5963,
/* 5956 */ -1,
/* 5957 */ -1,
/* 5958 */ -1,
/* 5959 */ -1,
/* 5960 */ -1,
/* 5961 */ -5955,
/* 5962 */ -1,
/* 5963 */ 0,
/* 5964 */ -5955,
/* 5965 */ -1,
/* 5966 */ 5971,
/* 5967 */ 50058,
/* 5968 */ -1,
/* 5969 */ -5966,
/* 5970 */ -1,
/* 5971 */ 0,
/* 5972 */ -5966,
/* 5973 */ -1,
/* 5974 */ 5978,
/* 5975 */ 50258,
/* 5976 */ -5974,
/* 5977 */ -1,
/* 5978 */ 0,
/* 5979 */ -5974,
/* 5980 */ -1,
/* 5981 */ 5986,
/* 5982 */ 50061,
/* 5983 */ -1,
/* 5984 */ -5981,
/* 5985 */ -1,
/* 5986 */ 0,
/* 5987 */ -5981,
/* 5988 */ -1,
/* 5989 */ 5998,
/* 5990 */ 50044,
/* 5991 */ -1,
/* 5992 */ -1,
/* 5993 */ -1,
/* 5994 */ -1,
/* 5995 */ -1,
/* 5996 */ -5989,
/* 5997 */ -1,
/* 5998 */ 0,
/* 5999 */ -5989,
/* 6000 */ -1,
/* 6001 */ 6006,
/* 6002 */ 50058,
/* 6003 */ -1,
/* 6004 */ -6001,
/* 6005 */ -1,
/* 6006 */ 0,
/* 6007 */ -6001,
/* 6008 */ -1,
/* 6009 */ 6013,
/* 6010 */ 50258,
/* 6011 */ -6009,
/* 6012 */ -1,
/* 6013 */ 0,
/* 6014 */ -6009,
/* 6015 */ -1,
/* 6016 */ 6021,
/* 6017 */ 50061,
/* 6018 */ -1,
/* 6019 */ -6016,
/* 6020 */ -1,
/* 6021 */ 0,
/* 6022 */ -6016,
/* 6023 */ -1,
/* 6024 */ 6029,
/* 6025 */ 50044,
/* 6026 */ 50494,
/* 6027 */ -6024,
/* 6028 */ -1,
/* 6029 */ 0,
/* 6030 */ -6024,
/* 6031 */ -1,
/* 6032 */ 0,
/* 6033 */ 50446,
/* 6034 */ -1,
/* 6035 */ -1,
/* 6036 */ 50262,
/* 6037 */ 50258,
/* 6038 */ 50059,
/* 6039 */ -6032,
/* 6040 */ -1,
/* 6041 */ 0,
/* 6042 */ 50446,
/* 6043 */ -1,
/* 6044 */ 50258,
/* 6045 */ 50480,
/* 6046 */ -1,
/* 6047 */ 50059,
/* 6048 */ -6041,
/* 6049 */ -1,
/* 6050 */ 0,
/* 6051 */ -1,
/* 6052 */ 50258,
/* 6053 */ -1,
/* 6054 */ 50059,
/* 6055 */ -6050,
/* 6056 */ -1,
/* 6057 */ 6063,
/* 6058 */ 50044,
/* 6059 */ 50258,
/* 6060 */ -1,
/* 6061 */ -6057,
/* 6062 */ -1,
/* 6063 */ 0,
/* 6064 */ -6057,
/* 6065 */ -1,
/* 6066 */ 0,
/* 6067 */ -1,
/* 6068 */ 50258,
/* 6069 */ -1,
/* 6070 */ 50059,
/* 6071 */ -6066,
/* 6072 */ -1,
/* 6073 */ 6079,
/* 6074 */ 50044,
/* 6075 */ 50258,
/* 6076 */ -1,
/* 6077 */ -6073,
/* 6078 */ -1,
/* 6079 */ 0,
/* 6080 */ -6073,
/* 6081 */ -1,
/* 6082 */ 0,
/* 6083 */ -1,
/* 6084 */ 50258,
/* 6085 */ -1,
/* 6086 */ 50059,
/* 6087 */ -6082,
/* 6088 */ -1,
/* 6089 */ 6095,
/* 6090 */ 50044,
/* 6091 */ 50258,
/* 6092 */ -1,
/* 6093 */ -6089,
/* 6094 */ -1,
/* 6095 */ 0,
/* 6096 */ -6089,
/* 6097 */ -1,
/* 6098 */ 6103,
/* 6099 */ -1,
/* 6100 */ 50059,
/* 6101 */ -6098,
/* 6102 */ -1,
/* 6103 */ 6113,
/* 6104 */ 50366,
/* 6105 */ 50040,
/* 6106 */ -1,
/* 6107 */ 50041,
/* 6108 */ -1,
/* 6109 */ 50334,
/* 6110 */ -1,
/* 6111 */ -6098,
/* 6112 */ -1,
/* 6113 */ 6121,
/* 6114 */ 50366,
/* 6115 */ 50040,
/* 6116 */ -1,
/* 6117 */ 50041,
/* 6118 */ -1,
/* 6119 */ -6098,
/* 6120 */ -1,
/* 6121 */ 6133,
/* 6122 */ 50352,
/* 6123 */ 50040,
/* 6124 */ -1,
/* 6125 */ 50059,
/* 6126 */ -1,
/* 6127 */ 50059,
/* 6128 */ -1,
/* 6129 */ 50041,
/* 6130 */ -1,
/* 6131 */ -6098,
/* 6132 */ -1,
/* 6133 */ 6144,
/* 6134 */ 50445,
/* 6135 */ 50040,
/* 6136 */ -1,
/* 6137 */ 50041,
/* 6138 */ 50123,
/* 6139 */ -1,
/* 6140 */ -1,
/* 6141 */ 50125,
/* 6142 */ -6098,
/* 6143 */ -1,
/* 6144 */ 6149,
/* 6145 */ 50301,
/* 6146 */ 50059,
/* 6147 */ -6098,
/* 6148 */ -1,
/* 6149 */ 6154,
/* 6150 */ 50313,
/* 6151 */ 50059,
/* 6152 */ -6098,
/* 6153 */ -1,
/* 6154 */ 6164,
/* 6155 */ 50330,
/* 6156 */ -1,
/* 6157 */ 50468,
/* 6158 */ 50040,
/* 6159 */ -1,
/* 6160 */ 50041,
/* 6161 */ 50059,
/* 6162 */ -6098,
/* 6163 */ -1,
/* 6164 */ 6172,
/* 6165 */ 50468,
/* 6166 */ 50040,
/* 6167 */ -1,
/* 6168 */ 50041,
/* 6169 */ -1,
/* 6170 */ -6098,
/* 6171 */ -1,
/* 6172 */ 6176,
/* 6173 */ -1,
/* 6174 */ -6098,
/* 6175 */ -1,
/* 6176 */ 6182,
/* 6177 */ 50424,
/* 6178 */ -1,
/* 6179 */ 50059,
/* 6180 */ -6098,
/* 6181 */ -1,
/* 6182 */ 0,
/* 6183 */ 50424,
/* 6184 */ 50059,
/* 6185 */ -6098,
/* 6186 */ -1,
/* 6187 */ 6195,
/* 6188 */ 50304,
/* 6189 */ -1,
/* 6190 */ 50058,
/* 6191 */ -1,
/* 6192 */ -1,
/* 6193 */ -6187,
/* 6194 */ -1,
/* 6195 */ 0,
/* 6196 */ -6187,
/* 6197 */ -1,
/* 6198 */ 6202,
/* 6199 */ -1,
/* 6200 */ -6198,
/* 6201 */ -1,
/* 6202 */ 0,
/* 6203 */ -1,
/* 6204 */ -6198,
/* 6205 */ -1,
/* 6206 */ 6211,
/* 6207 */ -1,
/* 6208 */ -1,
/* 6209 */ -6206,
/* 6210 */ -1,
/* 6211 */ 0,
/* 6212 */ -6206,
/* 6213 */ -1,
/* 6214 */ 6220,
/* 6215 */ 50323,
/* 6216 */ 50058,
/* 6217 */ -1,
/* 6218 */ -6214,
/* 6219 */ -1,
/* 6220 */ 0,
/* 6221 */ -6214,
/* 6222 */ -1,
/* 6223 */ 6228,
/* 6224 */ -1,
/* 6225 */ -1,
/* 6226 */ -6223,
/* 6227 */ -1,
/* 6228 */ 0,
/* 6229 */ -6223,
/* 6230 */ -1,
/* 6231 */ 0,
/* 6232 */ 50123,
/* 6233 */ -1,
/* 6234 */ 50125,
/* 6235 */ -6231,
/* 6236 */ -1,
/* 6237 */ 6242,
/* 6238 */ -1,
/* 6239 */ -1,
/* 6240 */ -6237,
/* 6241 */ -1,
/* 6242 */ 0,
/* 6243 */ -6237,
/* 6244 */ -1,
/* 6245 */ 0,
/* 6246 */ 50123,
/* 6247 */ -1,
/* 6248 */ -1,
/* 6249 */ 50125,
/* 6250 */ -6245,
/* 6251 */ -1,
/* 6252 */ 6257,
/* 6253 */ -1,
/* 6254 */ -1,
/* 6255 */ -6252,
/* 6256 */ -1,
/* 6257 */ 6263,
/* 6258 */ 50440,
/* 6259 */ -1,
/* 6260 */ -1,
/* 6261 */ -6252,
/* 6262 */ -1,
/* 6263 */ 0,
/* 6264 */ -6252,
/* 6265 */ -1,
/* 6266 */ 6271,
/* 6267 */ -1,
/* 6268 */ -1,
/* 6269 */ -6266,
/* 6270 */ -1,
/* 6271 */ 0,
/* 6272 */ -6266,
/* 6273 */ -1,
/* 6274 */ 6278,
/* 6275 */ 50256,
/* 6276 */ -6274,
/* 6277 */ -1,
/* 6278 */ 6282,
/* 6279 */ 50259,
/* 6280 */ -6274,
/* 6281 */ -1,
/* 6282 */ 6286,
/* 6283 */ 50257,
/* 6284 */ -6274,
/* 6285 */ -1,
/* 6286 */ 6290,
/* 6287 */ 50457,
/* 6288 */ -6274,
/* 6289 */ -1,
/* 6290 */ 6294,
/* 6291 */ 50344,
/* 6292 */ -6274,
/* 6293 */ -1,
/* 6294 */ 6298,
/* 6295 */ 50394,
/* 6296 */ -6274,
/* 6297 */ -1,
/* 6298 */ 6302,
/* 6299 */ -1,
/* 6300 */ -6274,
/* 6301 */ -1,
/* 6302 */ 6308,
/* 6303 */ 50040,
/* 6304 */ -1,
/* 6305 */ 50041,
/* 6306 */ -6274,
/* 6307 */ -1,
/* 6308 */ 6312,
/* 6309 */ 50258,
/* 6310 */ -6274,
/* 6311 */ -1,
/* 6312 */ 6318,
/* 6313 */ -1,
/* 6314 */ -1,
/* 6315 */ 50258,
/* 6316 */ -6274,
/* 6317 */ -1,
/* 6318 */ 6327,
/* 6319 */ -1,
/* 6320 */ -1,
/* 6321 */ 50258,
/* 6322 */ 50040,
/* 6323 */ -1,
/* 6324 */ 50041,
/* 6325 */ -6274,
/* 6326 */ -1,
/* 6327 */ 6334,
/* 6328 */ 50258,
/* 6329 */ 50040,
/* 6330 */ -1,
/* 6331 */ 50041,
/* 6332 */ -6274,
/* 6333 */ -1,
/* 6334 */ 0,
/* 6335 */ -1,
/* 6336 */ 50064,
/* 6337 */ 50258,
/* 6338 */ -6274,
/* 6339 */ -1,
/* 6340 */ 6344,
/* 6341 */ 50046,
/* 6342 */ -6340,
/* 6343 */ -1,
/* 6344 */ 0,
/* 6345 */ 50495,
/* 6346 */ -6340,
/* 6347 */ -1,
/* 6348 */ 6352,
/* 6349 */ 50046,
/* 6350 */ -6348,
/* 6351 */ -1,
/* 6352 */ 0,
/* 6353 */ 50495,
/* 6354 */ -6348,
/* 6355 */ -1,
/* 6356 */ 6360,
/* 6357 */ 50258,
/* 6358 */ -6356,
/* 6359 */ -1,
/* 6360 */ 6366,
/* 6361 */ -1,
/* 6362 */ 50064,
/* 6363 */ 50258,
/* 6364 */ -6356,
/* 6365 */ -1,
/* 6366 */ 0,
/* 6367 */ -1,
/* 6368 */ -1,
/* 6369 */ 50258,
/* 6370 */ -6356,
/* 6371 */ -1,
/* 6372 */ 6376,
/* 6373 */ 50046,
/* 6374 */ -6372,
/* 6375 */ -1,
/* 6376 */ 0,
/* 6377 */ 50495,
/* 6378 */ -6372,
/* 6379 */ -1,
/* 6380 */ 6385,
/* 6381 */ -1,
/* 6382 */ 50474,
/* 6383 */ -6380,
/* 6384 */ -1,
/* 6385 */ 6390,
/* 6386 */ -1,
/* 6387 */ 50475,
/* 6388 */ -6380,
/* 6389 */ -1,
/* 6390 */ 6399,
/* 6391 */ 50386,
/* 6392 */ 50040,
/* 6393 */ -1,
/* 6394 */ 50044,
/* 6395 */ -1,
/* 6396 */ 50041,
/* 6397 */ -6380,
/* 6398 */ -1,
/* 6399 */ 6408,
/* 6400 */ 50383,
/* 6401 */ 50040,
/* 6402 */ -1,
/* 6403 */ 50044,
/* 6404 */ -1,
/* 6405 */ 50041,
/* 6406 */ -6380,
/* 6407 */ -1,
/* 6408 */ 0,
/* 6409 */ -1,
/* 6410 */ -6380,
/* 6411 */ -1,
/* 6412 */ 6417,
/* 6413 */ 50474,
/* 6414 */ -1,
/* 6415 */ -6412,
/* 6416 */ -1,
/* 6417 */ 6422,
/* 6418 */ 50475,
/* 6419 */ -1,
/* 6420 */ -6412,
/* 6421 */ -1,
/* 6422 */ 6427,
/* 6423 */ 50043,
/* 6424 */ -1,
/* 6425 */ -6412,
/* 6426 */ -1,
/* 6427 */ 6432,
/* 6428 */ 50045,
/* 6429 */ -1,
/* 6430 */ -6412,
/* 6431 */ -1,
/* 6432 */ 6437,
/* 6433 */ 50033,
/* 6434 */ -1,
/* 6435 */ -6412,
/* 6436 */ -1,
/* 6437 */ 6442,
/* 6438 */ 50126,
/* 6439 */ -1,
/* 6440 */ -6412,
/* 6441 */ -1,
/* 6442 */ 6448,
/* 6443 */ 50058,
/* 6444 */ -1,
/* 6445 */ 50058,
/* 6446 */ -6412,
/* 6447 */ -1,
/* 6448 */ 0,
/* 6449 */ -1,
/* 6450 */ -6412,
/* 6451 */ -1,
/* 6452 */ 6458,
/* 6453 */ -1,
/* 6454 */ 50100,
/* 6455 */ -1,
/* 6456 */ -6452,
/* 6457 */ -1,
/* 6458 */ 0,
/* 6459 */ -1,
/* 6460 */ -6452,
/* 6461 */ -1,
/* 6462 */ 6469,
/* 6463 */ 50040,
/* 6464 */ -1,
/* 6465 */ 50041,
/* 6466 */ -1,
/* 6467 */ -6462,
/* 6468 */ -1,
/* 6469 */ 0,
/* 6470 */ -1,
/* 6471 */ -6462,
/* 6472 */ -1,
/* 6473 */ 6479,
/* 6474 */ -1,
/* 6475 */ 50042,
/* 6476 */ -1,
/* 6477 */ -6473,
/* 6478 */ -1,
/* 6479 */ 6485,
/* 6480 */ -1,
/* 6481 */ 50047,
/* 6482 */ -1,
/* 6483 */ -6473,
/* 6484 */ -1,
/* 6485 */ 6491,
/* 6486 */ -1,
/* 6487 */ 50037,
/* 6488 */ -1,
/* 6489 */ -6473,
/* 6490 */ -1,
/* 6491 */ 0,
/* 6492 */ -1,
/* 6493 */ -6473,
/* 6494 */ -1,
/* 6495 */ 6501,
/* 6496 */ -1,
/* 6497 */ 50043,
/* 6498 */ -1,
/* 6499 */ -6495,
/* 6500 */ -1,
/* 6501 */ 6507,
/* 6502 */ -1,
/* 6503 */ 50045,
/* 6504 */ -1,
/* 6505 */ -6495,
/* 6506 */ -1,
/* 6507 */ 0,
/* 6508 */ -1,
/* 6509 */ -6495,
/* 6510 */ -1,
/* 6511 */ 6517,
/* 6512 */ -1,
/* 6513 */ 50476,
/* 6514 */ -1,
/* 6515 */ -6511,
/* 6516 */ -1,
/* 6517 */ 6523,
/* 6518 */ -1,
/* 6519 */ 50477,
/* 6520 */ -1,
/* 6521 */ -6511,
/* 6522 */ -1,
/* 6523 */ 0,
/* 6524 */ -1,
/* 6525 */ -6511,
/* 6526 */ -1,
/* 6527 */ 6533,
/* 6528 */ -1,
/* 6529 */ 50060,
/* 6530 */ -1,
/* 6531 */ -6527,
/* 6532 */ -1,
/* 6533 */ 6539,
/* 6534 */ -1,
/* 6535 */ 50062,
/* 6536 */ -1,
/* 6537 */ -6527,
/* 6538 */ -1,
/* 6539 */ 6545,
/* 6540 */ -1,
/* 6541 */ 50478,
/* 6542 */ -1,
/* 6543 */ -6527,
/* 6544 */ -1,
/* 6545 */ 6551,
/* 6546 */ -1,
/* 6547 */ 50479,
/* 6548 */ -1,
/* 6549 */ -6527,
/* 6550 */ -1,
/* 6551 */ 6557,
/* 6552 */ -1,
/* 6553 */ 50480,
/* 6554 */ -1,
/* 6555 */ -6527,
/* 6556 */ -1,
/* 6557 */ 6563,
/* 6558 */ -1,
/* 6559 */ 50481,
/* 6560 */ -1,
/* 6561 */ -6527,
/* 6562 */ -1,
/* 6563 */ 0,
/* 6564 */ -1,
/* 6565 */ -6527,
/* 6566 */ -1,
/* 6567 */ 6573,
/* 6568 */ -1,
/* 6569 */ 50038,
/* 6570 */ -1,
/* 6571 */ -6567,
/* 6572 */ -1,
/* 6573 */ 0,
/* 6574 */ -1,
/* 6575 */ -6567,
/* 6576 */ -1,
/* 6577 */ 6583,
/* 6578 */ -1,
/* 6579 */ 50094,
/* 6580 */ -1,
/* 6581 */ -6577,
/* 6582 */ -1,
/* 6583 */ 0,
/* 6584 */ -1,
/* 6585 */ -6577,
/* 6586 */ -1,
/* 6587 */ 6593,
/* 6588 */ -1,
/* 6589 */ 50124,
/* 6590 */ -1,
/* 6591 */ -6587,
/* 6592 */ -1,
/* 6593 */ 0,
/* 6594 */ -1,
/* 6595 */ -6587,
/* 6596 */ -1,
/* 6597 */ 6603,
/* 6598 */ -1,
/* 6599 */ 50482,
/* 6600 */ -1,
/* 6601 */ -6597,
/* 6602 */ -1,
/* 6603 */ 0,
/* 6604 */ -1,
/* 6605 */ -6597,
/* 6606 */ -1,
/* 6607 */ 6613,
/* 6608 */ -1,
/* 6609 */ 50483,
/* 6610 */ -1,
/* 6611 */ -6607,
/* 6612 */ -1,
/* 6613 */ 0,
/* 6614 */ -1,
/* 6615 */ -6607,
/* 6616 */ -1,
/* 6617 */ 6625,
/* 6618 */ -1,
/* 6619 */ 50063,
/* 6620 */ -1,
/* 6621 */ 50058,
/* 6622 */ -1,
/* 6623 */ -6617,
/* 6624 */ -1,
/* 6625 */ 0,
/* 6626 */ -1,
/* 6627 */ -6617,
/* 6628 */ -1,
/* 6629 */ 6635,
/* 6630 */ -1,
/* 6631 */ 50061,
/* 6632 */ -1,
/* 6633 */ -6629,
/* 6634 */ -1,
/* 6635 */ 6641,
/* 6636 */ -1,
/* 6637 */ 50485,
/* 6638 */ -1,
/* 6639 */ -6629,
/* 6640 */ -1,
/* 6641 */ 6647,
/* 6642 */ -1,
/* 6643 */ 50493,
/* 6644 */ -1,
/* 6645 */ -6629,
/* 6646 */ -1,
/* 6647 */ 6653,
/* 6648 */ -1,
/* 6649 */ 50484,
/* 6650 */ -1,
/* 6651 */ -6629,
/* 6652 */ -1,
/* 6653 */ 6659,
/* 6654 */ -1,
/* 6655 */ 50486,
/* 6656 */ -1,
/* 6657 */ -6629,
/* 6658 */ -1,
/* 6659 */ 6665,
/* 6660 */ -1,
/* 6661 */ 50487,
/* 6662 */ -1,
/* 6663 */ -6629,
/* 6664 */ -1,
/* 6665 */ 6671,
/* 6666 */ -1,
/* 6667 */ 50488,
/* 6668 */ -1,
/* 6669 */ -6629,
/* 6670 */ -1,
/* 6671 */ 6677,
/* 6672 */ -1,
/* 6673 */ 50489,
/* 6674 */ -1,
/* 6675 */ -6629,
/* 6676 */ -1,
/* 6677 */ 6683,
/* 6678 */ -1,
/* 6679 */ 50491,
/* 6680 */ -1,
/* 6681 */ -6629,
/* 6682 */ -1,
/* 6683 */ 6689,
/* 6684 */ -1,
/* 6685 */ 50492,
/* 6686 */ -1,
/* 6687 */ -6629,
/* 6688 */ -1,
/* 6689 */ 6695,
/* 6690 */ -1,
/* 6691 */ 50490,
/* 6692 */ -1,
/* 6693 */ -6629,
/* 6694 */ -1,
/* 6695 */ 0,
/* 6696 */ -1,
/* 6697 */ -6629,
/* 6698 */ -1,
/* 6699 */ 6705,
/* 6700 */ -1,
/* 6701 */ 50044,
/* 6702 */ -1,
/* 6703 */ -6699,
/* 6704 */ -1,
/* 6705 */ 0,
/* 6706 */ -1,
/* 6707 */ -6699,
/* 6708 */ -1,
/* 6709 */ 0,
/* 6710 */ -1,
/* 6711 */ -6709,
/* 6712 */ -1,
/* 6713 */ 6718,
/* 6714 */ -1,
/* 6715 */ -1,
/* 6716 */ -6713,
/* 6717 */ -1,
/* 6718 */ 0,
/* 6719 */ -6713,
/* 6720 */ -1,
/* 6721 */ 6727,
/* 6722 */ 50044,
/* 6723 */ -1,
/* 6724 */ -1,
/* 6725 */ -6721,
/* 6726 */ -1,
/* 6727 */ 0,
/* 6728 */ -6721,
/* 6729 */ -1,
0
};
extern int yycoordinate[];
int yycoordinate[] = {
0,
/* 1 */ 9999,
/* 2 */ 194005,
/* 3 */ 9999,
/* 4 */ 9999,
/* 5 */ 194005,
/* 6 */ 9999,
/* 7 */ 196003,
/* 8 */ 9999,
/* 9 */ 195003,
/* 10 */ 9999,
/* 11 */ 196004,
/* 12 */ 196011,
/* 13 */ 9999,
/* 14 */ 196011,
/* 15 */ 9999,
/* 16 */ 9999,
/* 17 */ 196011,
/* 18 */ 9999,
/* 19 */ 9999,
/* 20 */ 9999,
/* 21 */ 9999,
/* 22 */ 9999,
/* 23 */ 199010,
/* 24 */ 9999,
/* 25 */ 9999,
/* 26 */ 200010,
/* 27 */ 9999,
/* 28 */ 9999,
/* 29 */ 200008,
/* 30 */ 9999,
/* 31 */ 9999,
/* 32 */ 9999,
/* 33 */ 9999,
/* 34 */ 9999,
/* 35 */ 201013,
/* 36 */ 9999,
/* 37 */ 202005,
/* 38 */ 9999,
/* 39 */ 202015,
/* 40 */ 9999,
/* 41 */ 203005,
/* 42 */ 9999,
/* 43 */ 203012,
/* 44 */ 9999,
/* 45 */ 204005,
/* 46 */ 9999,
/* 47 */ 204015,
/* 48 */ 9999,
/* 49 */ 205005,
/* 50 */ 9999,
/* 51 */ 205016,
/* 52 */ 9999,
/* 53 */ 206005,
/* 54 */ 9999,
/* 55 */ 206015,
/* 56 */ 9999,
/* 57 */ 207005,
/* 58 */ 9999,
/* 59 */ 207013,
/* 60 */ 9999,
/* 61 */ 208005,
/* 62 */ 9999,
/* 63 */ 208014,
/* 64 */ 9999,
/* 65 */ 209005,
/* 66 */ 9999,
/* 67 */ 209012,
/* 68 */ 9999,
/* 69 */ 210005,
/* 70 */ 9999,
/* 71 */ 210013,
/* 72 */ 9999,
/* 73 */ 211005,
/* 74 */ 9999,
/* 75 */ 211011,
/* 76 */ 9999,
/* 77 */ 212005,
/* 78 */ 9999,
/* 79 */ 212014,
/* 80 */ 9999,
/* 81 */ 213005,
/* 82 */ 9999,
/* 83 */ 213014,
/* 84 */ 9999,
/* 85 */ 214005,
/* 86 */ 9999,
/* 87 */ 214015,
/* 88 */ 9999,
/* 89 */ 215005,
/* 90 */ 9999,
/* 91 */ 215016,
/* 92 */ 9999,
/* 93 */ 216005,
/* 94 */ 9999,
/* 95 */ 216014,
/* 96 */ 9999,
/* 97 */ 217005,
/* 98 */ 9999,
/* 99 */ 217017,
/* 100 */ 9999,
/* 101 */ 218005,
/* 102 */ 9999,
/* 103 */ 218017,
/* 104 */ 9999,
/* 105 */ 219005,
/* 106 */ 9999,
/* 107 */ 219013,
/* 108 */ 9999,
/* 109 */ 220005,
/* 110 */ 9999,
/* 111 */ 220015,
/* 112 */ 9999,
/* 113 */ 221005,
/* 114 */ 9999,
/* 115 */ 221016,
/* 116 */ 9999,
/* 117 */ 222005,
/* 118 */ 9999,
/* 119 */ 222017,
/* 120 */ 9999,
/* 121 */ 223005,
/* 122 */ 9999,
/* 123 */ 223017,
/* 124 */ 9999,
/* 125 */ 224005,
/* 126 */ 9999,
/* 127 */ 224017,
/* 128 */ 9999,
/* 129 */ 9999,
/* 130 */ 225011,
/* 131 */ 225017,
/* 132 */ 9999,
/* 133 */ 225009,
/* 134 */ 9999,
/* 135 */ 9999,
/* 136 */ 9999,
/* 137 */ 9999,
/* 138 */ 226027,
/* 139 */ 9999,
/* 140 */ 226058,
/* 141 */ 9999,
/* 142 */ 9999,
/* 143 */ 226008,
/* 144 */ 9999,
/* 145 */ 9999,
/* 146 */ 9999,
/* 147 */ 225012,
/* 148 */ 9999,
/* 149 */ 9999,
/* 150 */ 9999,
/* 151 */ 9999,
/* 152 */ 226036,
/* 153 */ 9999,
/* 154 */ 9999,
/* 155 */ 226036,
/* 156 */ 9999,
/* 157 */ 9999,
/* 158 */ 9999,
/* 159 */ 226059,
/* 160 */ 9999,
/* 161 */ 9999,
/* 162 */ 226059,
/* 163 */ 9999,
/* 164 */ 9999,
/* 165 */ 9999,
/* 166 */ 9999,
/* 167 */ 231005,
/* 168 */ 9999,
/* 169 */ 9999,
/* 170 */ 240009,
/* 171 */ 9999,
/* 172 */ 240005,
/* 173 */ 9999,
/* 174 */ 244005,
/* 175 */ 9999,
/* 176 */ 244018,
/* 177 */ 9999,
/* 178 */ 244009,
/* 179 */ 9999,
/* 180 */ 245005,
/* 181 */ 9999,
/* 182 */ 245018,
/* 183 */ 9999,
/* 184 */ 245009,
/* 185 */ 9999,
/* 186 */ 246005,
/* 187 */ 9999,
/* 188 */ 246010,
/* 189 */ 9999,
/* 190 */ 250005,
/* 191 */ 9999,
/* 192 */ 250019,
/* 193 */ 9999,
/* 194 */ 250010,
/* 195 */ 9999,
/* 196 */ 251005,
/* 197 */ 9999,
/* 198 */ 251019,
/* 199 */ 9999,
/* 200 */ 251010,
/* 201 */ 9999,
/* 202 */ 252005,
/* 203 */ 9999,
/* 204 */ 252019,
/* 205 */ 9999,
/* 206 */ 252010,
/* 207 */ 9999,
/* 208 */ 253005,
/* 209 */ 9999,
/* 210 */ 253019,
/* 211 */ 9999,
/* 212 */ 253010,
/* 213 */ 9999,
/* 214 */ 254005,
/* 215 */ 9999,
/* 216 */ 254019,
/* 217 */ 9999,
/* 218 */ 254010,
/* 219 */ 9999,
/* 220 */ 256005,
/* 221 */ 9999,
/* 222 */ 256010,
/* 223 */ 9999,
/* 224 */ 9999,
/* 225 */ 260009,
/* 226 */ 9999,
/* 227 */ 260005,
/* 228 */ 9999,
/* 229 */ 9999,
/* 230 */ 261009,
/* 231 */ 9999,
/* 232 */ 261005,
/* 233 */ 9999,
/* 234 */ 262005,
/* 235 */ 9999,
/* 236 */ 262010,
/* 237 */ 9999,
/* 238 */ 9999,
/* 239 */ 266009,
/* 240 */ 9999,
/* 241 */ 9999,
/* 242 */ 266005,
/* 243 */ 9999,
/* 244 */ 9999,
/* 245 */ 9999,
/* 246 */ 267010,
/* 247 */ 9999,
/* 248 */ 9999,
/* 249 */ 9999,
/* 250 */ 268013,
/* 251 */ 9999,
/* 252 */ 9999,
/* 253 */ 9999,
/* 254 */ 269014,
/* 255 */ 9999,
/* 256 */ 9999,
/* 257 */ 9999,
/* 258 */ 270014,
/* 259 */ 9999,
/* 260 */ 274005,
/* 261 */ 9999,
/* 262 */ 274021,
/* 263 */ 9999,
/* 264 */ 274037,
/* 265 */ 9999,
/* 266 */ 274010,
/* 267 */ 9999,
/* 268 */ 276005,
/* 269 */ 9999,
/* 270 */ 276021,
/* 271 */ 9999,
/* 272 */ 9999,
/* 273 */ 276041,
/* 274 */ 9999,
/* 275 */ 276010,
/* 276 */ 9999,
/* 277 */ 278005,
/* 278 */ 9999,
/* 279 */ 278021,
/* 280 */ 9999,
/* 281 */ 278010,
/* 282 */ 9999,
/* 283 */ 280005,
/* 284 */ 9999,
/* 285 */ 280010,
/* 286 */ 9999,
/* 287 */ 285005,
/* 288 */ 9999,
/* 289 */ 285021,
/* 290 */ 9999,
/* 291 */ 285037,
/* 292 */ 9999,
/* 293 */ 285010,
/* 294 */ 9999,
/* 295 */ 287005,
/* 296 */ 9999,
/* 297 */ 287021,
/* 298 */ 9999,
/* 299 */ 287010,
/* 300 */ 9999,
/* 301 */ 296003,
/* 302 */ 297003,
/* 303 */ 298003,
/* 304 */ 299003,
/* 305 */ 301003,
/* 306 */ 303003,
/* 307 */ 304003,
/* 308 */ 9999,
/* 309 */ 293003,
/* 310 */ 9999,
/* 311 */ 9999,
/* 312 */ 9999,
/* 313 */ 296011,
/* 314 */ 9999,
/* 315 */ 9999,
/* 316 */ 296005,
/* 317 */ 9999,
/* 318 */ 9999,
/* 319 */ 296005,
/* 320 */ 9999,
/* 321 */ 9999,
/* 322 */ 9999,
/* 323 */ 9999,
/* 324 */ 297009,
/* 325 */ 9999,
/* 326 */ 9999,
/* 327 */ 297009,
/* 328 */ 9999,
/* 329 */ 298004,
/* 330 */ 9999,
/* 331 */ 298012,
/* 332 */ 9999,
/* 333 */ 9999,
/* 334 */ 298012,
/* 335 */ 9999,
/* 336 */ 9999,
/* 337 */ 9999,
/* 338 */ 299009,
/* 339 */ 9999,
/* 340 */ 9999,
/* 341 */ 9999,
/* 342 */ 300012,
/* 343 */ 9999,
/* 344 */ 9999,
/* 345 */ 300012,
/* 346 */ 9999,
/* 347 */ 301004,
/* 348 */ 9999,
/* 349 */ 301010,
/* 350 */ 9999,
/* 351 */ 9999,
/* 352 */ 302013,
/* 353 */ 9999,
/* 354 */ 302011,
/* 355 */ 9999,
/* 356 */ 9999,
/* 357 */ 9999,
/* 358 */ 9999,
/* 359 */ 303005,
/* 360 */ 9999,
/* 361 */ 9999,
/* 362 */ 9999,
/* 363 */ 9999,
/* 364 */ 303036,
/* 365 */ 9999,
/* 366 */ 9999,
/* 367 */ 303036,
/* 368 */ 9999,
/* 369 */ 9999,
/* 370 */ 9999,
/* 371 */ 304013,
/* 372 */ 9999,
/* 373 */ 304007,
/* 374 */ 9999,
/* 375 */ 304007,
/* 376 */ 9999,
/* 377 */ 9999,
/* 378 */ 308009,
/* 379 */ 308007,
/* 380 */ 9999,
/* 381 */ 308007,
/* 382 */ 9999,
/* 383 */ 9999,
/* 384 */ 9999,
/* 385 */ 309016,
/* 386 */ 9999,
/* 387 */ 309029,
/* 388 */ 309007,
/* 389 */ 9999,
/* 390 */ 309007,
/* 391 */ 9999,
/* 392 */ 9999,
/* 393 */ 9999,
/* 394 */ 311016,
/* 395 */ 311007,
/* 396 */ 9999,
/* 397 */ 311007,
/* 398 */ 9999,
/* 399 */ 9999,
/* 400 */ 9999,
/* 401 */ 313007,
/* 402 */ 9999,
/* 403 */ 313007,
/* 404 */ 9999,
/* 405 */ 9999,
/* 406 */ 9999,
/* 407 */ 315013,
/* 408 */ 9999,
/* 409 */ 315027,
/* 410 */ 9999,
/* 411 */ 315007,
/* 412 */ 9999,
/* 413 */ 315007,
/* 414 */ 9999,
/* 415 */ 9999,
/* 416 */ 315007,
/* 417 */ 9999,
/* 418 */ 304014,
/* 419 */ 304024,
/* 420 */ 304018,
/* 421 */ 9999,
/* 422 */ 304018,
/* 423 */ 9999,
/* 424 */ 9999,
/* 425 */ 304018,
/* 426 */ 9999,
/* 427 */ 9999,
/* 428 */ 9999,
/* 429 */ 304025,
/* 430 */ 9999,
/* 431 */ 9999,
/* 432 */ 304025,
/* 433 */ 9999,
/* 434 */ 9999,
/* 435 */ 321008,
/* 436 */ 322005,
/* 437 */ 325006,
/* 438 */ 9999,
/* 439 */ 9999,
/* 440 */ 320004,
/* 441 */ 9999,
/* 442 */ 9999,
/* 443 */ 9999,
/* 444 */ 321009,
/* 445 */ 9999,
/* 446 */ 322007,
/* 447 */ 324007,
/* 448 */ 322016,
/* 449 */ 9999,
/* 450 */ 322016,
/* 451 */ 9999,
/* 452 */ 9999,
/* 453 */ 322016,
/* 454 */ 9999,
/* 455 */ 9999,
/* 456 */ 9999,
/* 457 */ 324008,
/* 458 */ 9999,
/* 459 */ 9999,
/* 460 */ 9999,
/* 461 */ 324034,
/* 462 */ 9999,
/* 463 */ 330003,
/* 464 */ 330017,
/* 465 */ 331005,
/* 466 */ 332008,
/* 467 */ 335005,
/* 468 */ 9999,
/* 469 */ 9999,
/* 470 */ 330003,
/* 471 */ 9999,
/* 472 */ 9999,
/* 473 */ 9999,
/* 474 */ 330008,
/* 475 */ 9999,
/* 476 */ 9999,
/* 477 */ 9999,
/* 478 */ 330014,
/* 479 */ 9999,
/* 480 */ 9999,
/* 481 */ 9999,
/* 482 */ 330018,
/* 483 */ 9999,
/* 484 */ 9999,
/* 485 */ 330018,
/* 486 */ 9999,
/* 487 */ 331006,
/* 488 */ 9999,
/* 489 */ 331010,
/* 490 */ 9999,
/* 491 */ 9999,
/* 492 */ 331010,
/* 493 */ 9999,
/* 494 */ 9999,
/* 495 */ 9999,
/* 496 */ 332018,
/* 497 */ 9999,
/* 498 */ 9999,
/* 499 */ 9999,
/* 500 */ 333012,
/* 501 */ 9999,
/* 502 */ 9999,
/* 503 */ 335009,
/* 504 */ 9999,
/* 505 */ 335007,
/* 506 */ 9999,
/* 507 */ 9999,
/* 508 */ 335007,
/* 509 */ 9999,
/* 510 */ 340003,
/* 511 */ 341003,
/* 512 */ 343003,
/* 513 */ 9999,
/* 514 */ 339038,
/* 515 */ 9999,
/* 516 */ 340005,
/* 517 */ 9999,
/* 518 */ 340009,
/* 519 */ 9999,
/* 520 */ 9999,
/* 521 */ 340009,
/* 522 */ 9999,
/* 523 */ 9999,
/* 524 */ 9999,
/* 525 */ 341013,
/* 526 */ 9999,
/* 527 */ 9999,
/* 528 */ 9999,
/* 529 */ 342012,
/* 530 */ 9999,
/* 531 */ 9999,
/* 532 */ 343007,
/* 533 */ 9999,
/* 534 */ 343005,
/* 535 */ 9999,
/* 536 */ 9999,
/* 537 */ 343005,
/* 538 */ 9999,
/* 539 */ 9999,
/* 540 */ 9999,
/* 541 */ 347012,
/* 542 */ 347024,
/* 543 */ 9999,
/* 544 */ 9999,
/* 545 */ 347004,
/* 546 */ 9999,
/* 547 */ 9999,
/* 548 */ 347029,
/* 549 */ 347025,
/* 550 */ 9999,
/* 551 */ 347025,
/* 552 */ 9999,
/* 553 */ 9999,
/* 554 */ 347025,
/* 555 */ 9999,
/* 556 */ 352003,
/* 557 */ 353003,
/* 558 */ 354003,
/* 559 */ 9999,
/* 560 */ 351027,
/* 561 */ 9999,
/* 562 */ 9999,
/* 563 */ 353008,
/* 564 */ 353004,
/* 565 */ 9999,
/* 566 */ 353004,
/* 567 */ 9999,
/* 568 */ 9999,
/* 569 */ 353004,
/* 570 */ 9999,
/* 571 */ 9999,
/* 572 */ 356005,
/* 573 */ 9999,
/* 574 */ 354011,
/* 575 */ 9999,
/* 576 */ 361007,
/* 577 */ 9999,
/* 578 */ 361013,
/* 579 */ 9999,
/* 580 */ 9999,
/* 581 */ 9999,
/* 582 */ 356007,
/* 583 */ 9999,
/* 584 */ 356007,
/* 585 */ 9999,
/* 586 */ 9999,
/* 587 */ 356007,
/* 588 */ 9999,
/* 589 */ 371003,
/* 590 */ 9999,
/* 591 */ 370003,
/* 592 */ 9999,
/* 593 */ 379003,
/* 594 */ 9999,
/* 595 */ 378003,
/* 596 */ 9999,
/* 597 */ 387003,
/* 598 */ 391003,
/* 599 */ 9999,
/* 600 */ 386003,
/* 601 */ 9999,
/* 602 */ 387007,
/* 603 */ 387014,
/* 604 */ 9999,
/* 605 */ 387007,
/* 606 */ 9999,
/* 607 */ 9999,
/* 608 */ 388011,
/* 609 */ 9999,
/* 610 */ 388007,
/* 611 */ 9999,
/* 612 */ 9999,
/* 613 */ 389011,
/* 614 */ 9999,
/* 615 */ 389007,
/* 616 */ 9999,
/* 617 */ 390007,
/* 618 */ 9999,
/* 619 */ 9999,
/* 620 */ 390012,
/* 621 */ 9999,
/* 622 */ 9999,
/* 623 */ 9999,
/* 624 */ 387008,
/* 625 */ 9999,
/* 626 */ 9999,
/* 627 */ 387008,
/* 628 */ 9999,
/* 629 */ 9999,
/* 630 */ 391009,
/* 631 */ 9999,
/* 632 */ 9999,
/* 633 */ 391005,
/* 634 */ 9999,
/* 635 */ 9999,
/* 636 */ 391005,
/* 637 */ 9999,
/* 638 */ 9999,
/* 639 */ 392016,
/* 640 */ 9999,
/* 641 */ 392014,
/* 642 */ 9999,
/* 643 */ 9999,
/* 644 */ 393016,
/* 645 */ 9999,
/* 646 */ 393014,
/* 647 */ 9999,
/* 648 */ 9999,
/* 649 */ 394012,
/* 650 */ 9999,
/* 651 */ 394010,
/* 652 */ 9999,
/* 653 */ 9999,
/* 654 */ 9999,
/* 655 */ 9999,
/* 656 */ 404009,
/* 657 */ 405005,
/* 658 */ 9999,
/* 659 */ 407009,
/* 660 */ 9999,
/* 661 */ 9999,
/* 662 */ 399009,
/* 663 */ 9999,
/* 664 */ 9999,
/* 665 */ 405011,
/* 666 */ 406005,
/* 667 */ 9999,
/* 668 */ 405007,
/* 669 */ 9999,
/* 670 */ 9999,
/* 671 */ 405007,
/* 672 */ 9999,
/* 673 */ 9999,
/* 674 */ 406011,
/* 675 */ 9999,
/* 676 */ 406007,
/* 677 */ 9999,
/* 678 */ 9999,
/* 679 */ 406007,
/* 680 */ 9999,
/* 681 */ 407010,
/* 682 */ 407018,
/* 683 */ 9999,
/* 684 */ 407018,
/* 685 */ 9999,
/* 686 */ 9999,
/* 687 */ 407018,
/* 688 */ 9999,
/* 689 */ 9999,
/* 690 */ 411015,
/* 691 */ 9999,
/* 692 */ 9999,
/* 693 */ 9999,
/* 694 */ 411013,
/* 695 */ 9999,
/* 696 */ 9999,
/* 697 */ 412011,
/* 698 */ 412018,
/* 699 */ 9999,
/* 700 */ 9999,
/* 701 */ 412009,
/* 702 */ 9999,
/* 703 */ 413005,
/* 704 */ 413028,
/* 705 */ 413035,
/* 706 */ 9999,
/* 707 */ 9999,
/* 708 */ 413005,
/* 709 */ 9999,
/* 710 */ 414005,
/* 711 */ 414028,
/* 712 */ 414035,
/* 713 */ 9999,
/* 714 */ 414050,
/* 715 */ 9999,
/* 716 */ 9999,
/* 717 */ 414005,
/* 718 */ 9999,
/* 719 */ 9999,
/* 720 */ 418013,
/* 721 */ 418020,
/* 722 */ 419018,
/* 723 */ 9999,
/* 724 */ 9999,
/* 725 */ 418011,
/* 726 */ 9999,
/* 727 */ 9999,
/* 728 */ 420013,
/* 729 */ 420020,
/* 730 */ 9999,
/* 731 */ 9999,
/* 732 */ 420011,
/* 733 */ 9999,
/* 734 */ 9999,
/* 735 */ 421015,
/* 736 */ 421022,
/* 737 */ 9999,
/* 738 */ 9999,
/* 739 */ 421013,
/* 740 */ 9999,
/* 741 */ 423005,
/* 742 */ 9999,
/* 743 */ 423016,
/* 744 */ 423023,
/* 745 */ 9999,
/* 746 */ 9999,
/* 747 */ 423005,
/* 748 */ 9999,
/* 749 */ 424005,
/* 750 */ 9999,
/* 751 */ 424015,
/* 752 */ 9999,
/* 753 */ 9999,
/* 754 */ 9999,
/* 755 */ 425015,
/* 756 */ 9999,
/* 757 */ 9999,
/* 758 */ 9999,
/* 759 */ 425009,
/* 760 */ 9999,
/* 761 */ 9999,
/* 762 */ 9999,
/* 763 */ 427015,
/* 764 */ 9999,
/* 765 */ 427030,
/* 766 */ 9999,
/* 767 */ 9999,
/* 768 */ 9999,
/* 769 */ 427009,
/* 770 */ 9999,
/* 771 */ 9999,
/* 772 */ 9999,
/* 773 */ 429015,
/* 774 */ 9999,
/* 775 */ 429030,
/* 776 */ 9999,
/* 777 */ 429046,
/* 778 */ 9999,
/* 779 */ 9999,
/* 780 */ 9999,
/* 781 */ 429009,
/* 782 */ 9999,
/* 783 */ 9999,
/* 784 */ 9999,
/* 785 */ 432015,
/* 786 */ 9999,
/* 787 */ 432030,
/* 788 */ 9999,
/* 789 */ 432046,
/* 790 */ 9999,
/* 791 */ 432062,
/* 792 */ 9999,
/* 793 */ 9999,
/* 794 */ 9999,
/* 795 */ 432009,
/* 796 */ 9999,
/* 797 */ 9999,
/* 798 */ 436013,
/* 799 */ 436020,
/* 800 */ 9999,
/* 801 */ 9999,
/* 802 */ 436011,
/* 803 */ 9999,
/* 804 */ 9999,
/* 805 */ 437012,
/* 806 */ 437019,
/* 807 */ 9999,
/* 808 */ 9999,
/* 809 */ 437010,
/* 810 */ 9999,
/* 811 */ 9999,
/* 812 */ 438012,
/* 813 */ 438019,
/* 814 */ 438032,
/* 815 */ 9999,
/* 816 */ 9999,
/* 817 */ 438010,
/* 818 */ 9999,
/* 819 */ 9999,
/* 820 */ 9999,
/* 821 */ 9999,
/* 822 */ 9999,
/* 823 */ 439006,
/* 824 */ 9999,
/* 825 */ 9999,
/* 826 */ 445008,
/* 827 */ 9999,
/* 828 */ 9999,
/* 829 */ 445006,
/* 830 */ 9999,
/* 831 */ 9999,
/* 832 */ 448009,
/* 833 */ 448016,
/* 834 */ 9999,
/* 835 */ 9999,
/* 836 */ 448007,
/* 837 */ 9999,
/* 838 */ 9999,
/* 839 */ 449009,
/* 840 */ 449016,
/* 841 */ 9999,
/* 842 */ 9999,
/* 843 */ 449007,
/* 844 */ 9999,
/* 845 */ 9999,
/* 846 */ 450010,
/* 847 */ 450017,
/* 848 */ 9999,
/* 849 */ 9999,
/* 850 */ 450008,
/* 851 */ 9999,
/* 852 */ 9999,
/* 853 */ 451011,
/* 854 */ 451018,
/* 855 */ 455019,
/* 856 */ 9999,
/* 857 */ 9999,
/* 858 */ 451009,
/* 859 */ 9999,
/* 860 */ 9999,
/* 861 */ 459011,
/* 862 */ 459018,
/* 863 */ 9999,
/* 864 */ 9999,
/* 865 */ 459009,
/* 866 */ 9999,
/* 867 */ 460005,
/* 868 */ 460018,
/* 869 */ 9999,
/* 870 */ 9999,
/* 871 */ 460016,
/* 872 */ 9999,
/* 873 */ 9999,
/* 874 */ 461010,
/* 875 */ 461017,
/* 876 */ 9999,
/* 877 */ 9999,
/* 878 */ 461008,
/* 879 */ 9999,
/* 880 */ 9999,
/* 881 */ 462011,
/* 882 */ 462018,
/* 883 */ 462028,
/* 884 */ 9999,
/* 885 */ 9999,
/* 886 */ 462009,
/* 887 */ 9999,
/* 888 */ 9999,
/* 889 */ 463010,
/* 890 */ 463017,
/* 891 */ 463027,
/* 892 */ 9999,
/* 893 */ 9999,
/* 894 */ 463008,
/* 895 */ 9999,
/* 896 */ 9999,
/* 897 */ 464012,
/* 898 */ 464019,
/* 899 */ 9999,
/* 900 */ 9999,
/* 901 */ 464010,
/* 902 */ 9999,
/* 903 */ 9999,
/* 904 */ 467013,
/* 905 */ 467020,
/* 906 */ 9999,
/* 907 */ 9999,
/* 908 */ 467011,
/* 909 */ 9999,
/* 910 */ 9999,
/* 911 */ 469012,
/* 912 */ 469019,
/* 913 */ 9999,
/* 914 */ 9999,
/* 915 */ 469010,
/* 916 */ 9999,
/* 917 */ 471005,
/* 918 */ 9999,
/* 919 */ 471013,
/* 920 */ 9999,
/* 921 */ 472005,
/* 922 */ 9999,
/* 923 */ 472014,
/* 924 */ 9999,
/* 925 */ 473005,
/* 926 */ 9999,
/* 927 */ 473016,
/* 928 */ 9999,
/* 929 */ 474005,
/* 930 */ 9999,
/* 931 */ 474017,
/* 932 */ 9999,
/* 933 */ 9999,
/* 934 */ 9999,
/* 935 */ 411016,
/* 936 */ 9999,
/* 937 */ 9999,
/* 938 */ 411016,
/* 939 */ 9999,
/* 940 */ 9999,
/* 941 */ 9999,
/* 942 */ 412012,
/* 943 */ 9999,
/* 944 */ 9999,
/* 945 */ 412012,
/* 946 */ 9999,
/* 947 */ 9999,
/* 948 */ 9999,
/* 949 */ 413015,
/* 950 */ 9999,
/* 951 */ 9999,
/* 952 */ 9999,
/* 953 */ 413025,
/* 954 */ 9999,
/* 955 */ 9999,
/* 956 */ 9999,
/* 957 */ 413029,
/* 958 */ 9999,
/* 959 */ 9999,
/* 960 */ 413029,
/* 961 */ 9999,
/* 962 */ 9999,
/* 963 */ 9999,
/* 964 */ 414015,
/* 965 */ 9999,
/* 966 */ 9999,
/* 967 */ 9999,
/* 968 */ 414025,
/* 969 */ 9999,
/* 970 */ 9999,
/* 971 */ 9999,
/* 972 */ 414029,
/* 973 */ 9999,
/* 974 */ 9999,
/* 975 */ 414029,
/* 976 */ 9999,
/* 977 */ 9999,
/* 978 */ 9999,
/* 979 */ 418014,
/* 980 */ 9999,
/* 981 */ 9999,
/* 982 */ 418014,
/* 983 */ 9999,
/* 984 */ 9999,
/* 985 */ 419024,
/* 986 */ 419020,
/* 987 */ 9999,
/* 988 */ 419020,
/* 989 */ 9999,
/* 990 */ 9999,
/* 991 */ 419020,
/* 992 */ 9999,
/* 993 */ 9999,
/* 994 */ 9999,
/* 995 */ 420014,
/* 996 */ 9999,
/* 997 */ 9999,
/* 998 */ 420014,
/* 999 */ 9999,
/* 1000 */ 9999,
/* 1001 */ 9999,
/* 1002 */ 421016,
/* 1003 */ 9999,
/* 1004 */ 9999,
/* 1005 */ 421016,
/* 1006 */ 9999,
/* 1007 */ 9999,
/* 1008 */ 9999,
/* 1009 */ 423006,
/* 1010 */ 9999,
/* 1011 */ 9999,
/* 1012 */ 423006,
/* 1013 */ 9999,
/* 1014 */ 9999,
/* 1015 */ 9999,
/* 1016 */ 423017,
/* 1017 */ 9999,
/* 1018 */ 9999,
/* 1019 */ 423017,
/* 1020 */ 9999,
/* 1021 */ 9999,
/* 1022 */ 9999,
/* 1023 */ 436014,
/* 1024 */ 9999,
/* 1025 */ 9999,
/* 1026 */ 436014,
/* 1027 */ 9999,
/* 1028 */ 436021,
/* 1029 */ 436057,
/* 1030 */ 436026,
/* 1031 */ 9999,
/* 1032 */ 436026,
/* 1033 */ 9999,
/* 1034 */ 9999,
/* 1035 */ 436026,
/* 1036 */ 9999,
/* 1037 */ 9999,
/* 1038 */ 9999,
/* 1039 */ 436058,
/* 1040 */ 9999,
/* 1041 */ 9999,
/* 1042 */ 436058,
/* 1043 */ 9999,
/* 1044 */ 9999,
/* 1045 */ 9999,
/* 1046 */ 437013,
/* 1047 */ 9999,
/* 1048 */ 9999,
/* 1049 */ 437013,
/* 1050 */ 9999,
/* 1051 */ 437020,
/* 1052 */ 437056,
/* 1053 */ 437025,
/* 1054 */ 9999,
/* 1055 */ 437025,
/* 1056 */ 9999,
/* 1057 */ 9999,
/* 1058 */ 437025,
/* 1059 */ 9999,
/* 1060 */ 9999,
/* 1061 */ 9999,
/* 1062 */ 437057,
/* 1063 */ 9999,
/* 1064 */ 9999,
/* 1065 */ 437057,
/* 1066 */ 9999,
/* 1067 */ 9999,
/* 1068 */ 9999,
/* 1069 */ 438013,
/* 1070 */ 9999,
/* 1071 */ 9999,
/* 1072 */ 438013,
/* 1073 */ 9999,
/* 1074 */ 9999,
/* 1075 */ 438037,
/* 1076 */ 438033,
/* 1077 */ 9999,
/* 1078 */ 438033,
/* 1079 */ 9999,
/* 1080 */ 9999,
/* 1081 */ 438033,
/* 1082 */ 9999,
/* 1083 */ 9999,
/* 1084 */ 9999,
/* 1085 */ 448010,
/* 1086 */ 9999,
/* 1087 */ 9999,
/* 1088 */ 448010,
/* 1089 */ 9999,
/* 1090 */ 9999,
/* 1091 */ 9999,
/* 1092 */ 449010,
/* 1093 */ 9999,
/* 1094 */ 9999,
/* 1095 */ 449010,
/* 1096 */ 9999,
/* 1097 */ 9999,
/* 1098 */ 9999,
/* 1099 */ 450011,
/* 1100 */ 9999,
/* 1101 */ 9999,
/* 1102 */ 450011,
/* 1103 */ 9999,
/* 1104 */ 9999,
/* 1105 */ 9999,
/* 1106 */ 451012,
/* 1107 */ 9999,
/* 1108 */ 9999,
/* 1109 */ 451012,
/* 1110 */ 9999,
/* 1111 */ 451019,
/* 1112 */ 9999,
/* 1113 */ 451024,
/* 1114 */ 9999,
/* 1115 */ 451024,
/* 1116 */ 9999,
/* 1117 */ 9999,
/* 1118 */ 451024,
/* 1119 */ 9999,
/* 1120 */ 9999,
/* 1121 */ 9999,
/* 1122 */ 459012,
/* 1123 */ 9999,
/* 1124 */ 9999,
/* 1125 */ 459012,
/* 1126 */ 9999,
/* 1127 */ 459019,
/* 1128 */ 459029,
/* 1129 */ 9999,
/* 1130 */ 459029,
/* 1131 */ 9999,
/* 1132 */ 9999,
/* 1133 */ 459029,
/* 1134 */ 9999,
/* 1135 */ 9999,
/* 1136 */ 460023,
/* 1137 */ 460019,
/* 1138 */ 9999,
/* 1139 */ 460019,
/* 1140 */ 9999,
/* 1141 */ 9999,
/* 1142 */ 460019,
/* 1143 */ 9999,
/* 1144 */ 9999,
/* 1145 */ 9999,
/* 1146 */ 461011,
/* 1147 */ 9999,
/* 1148 */ 9999,
/* 1149 */ 461011,
/* 1150 */ 9999,
/* 1151 */ 9999,
/* 1152 */ 9999,
/* 1153 */ 462012,
/* 1154 */ 9999,
/* 1155 */ 9999,
/* 1156 */ 462012,
/* 1157 */ 9999,
/* 1158 */ 9999,
/* 1159 */ 9999,
/* 1160 */ 462029,
/* 1161 */ 9999,
/* 1162 */ 9999,
/* 1163 */ 462029,
/* 1164 */ 9999,
/* 1165 */ 9999,
/* 1166 */ 9999,
/* 1167 */ 463011,
/* 1168 */ 9999,
/* 1169 */ 9999,
/* 1170 */ 463011,
/* 1171 */ 9999,
/* 1172 */ 9999,
/* 1173 */ 9999,
/* 1174 */ 463028,
/* 1175 */ 9999,
/* 1176 */ 9999,
/* 1177 */ 463028,
/* 1178 */ 9999,
/* 1179 */ 9999,
/* 1180 */ 9999,
/* 1181 */ 464013,
/* 1182 */ 9999,
/* 1183 */ 9999,
/* 1184 */ 464013,
/* 1185 */ 9999,
/* 1186 */ 464021,
/* 1187 */ 466006,
/* 1188 */ 464027,
/* 1189 */ 9999,
/* 1190 */ 464027,
/* 1191 */ 9999,
/* 1192 */ 9999,
/* 1193 */ 464027,
/* 1194 */ 9999,
/* 1195 */ 9999,
/* 1196 */ 9999,
/* 1197 */ 466007,
/* 1198 */ 9999,
/* 1199 */ 9999,
/* 1200 */ 466007,
/* 1201 */ 9999,
/* 1202 */ 9999,
/* 1203 */ 9999,
/* 1204 */ 467014,
/* 1205 */ 9999,
/* 1206 */ 9999,
/* 1207 */ 467014,
/* 1208 */ 9999,
/* 1209 */ 467022,
/* 1210 */ 468006,
/* 1211 */ 467028,
/* 1212 */ 9999,
/* 1213 */ 467028,
/* 1214 */ 9999,
/* 1215 */ 9999,
/* 1216 */ 467028,
/* 1217 */ 9999,
/* 1218 */ 9999,
/* 1219 */ 9999,
/* 1220 */ 468007,
/* 1221 */ 9999,
/* 1222 */ 9999,
/* 1223 */ 468007,
/* 1224 */ 9999,
/* 1225 */ 9999,
/* 1226 */ 9999,
/* 1227 */ 469013,
/* 1228 */ 9999,
/* 1229 */ 9999,
/* 1230 */ 469013,
/* 1231 */ 9999,
/* 1232 */ 469021,
/* 1233 */ 470006,
/* 1234 */ 469027,
/* 1235 */ 9999,
/* 1236 */ 469027,
/* 1237 */ 9999,
/* 1238 */ 9999,
/* 1239 */ 469027,
/* 1240 */ 9999,
/* 1241 */ 9999,
/* 1242 */ 9999,
/* 1243 */ 470007,
/* 1244 */ 9999,
/* 1245 */ 9999,
/* 1246 */ 470007,
/* 1247 */ 9999,
/* 1248 */ 9999,
/* 1249 */ 9999,
/* 1250 */ 478007,
/* 1251 */ 9999,
/* 1252 */ 9999,
/* 1253 */ 9999,
/* 1254 */ 9999,
/* 1255 */ 479010,
/* 1256 */ 9999,
/* 1257 */ 9999,
/* 1258 */ 9999,
/* 1259 */ 9999,
/* 1260 */ 480009,
/* 1261 */ 9999,
/* 1262 */ 9999,
/* 1263 */ 9999,
/* 1264 */ 9999,
/* 1265 */ 481008,
/* 1266 */ 9999,
/* 1267 */ 487003,
/* 1268 */ 9999,
/* 1269 */ 488005,
/* 1270 */ 490003,
/* 1271 */ 492003,
/* 1272 */ 493005,
/* 1273 */ 504003,
/* 1274 */ 9999,
/* 1275 */ 485032,
/* 1276 */ 9999,
/* 1277 */ 516005,
/* 1278 */ 9999,
/* 1279 */ 516020,
/* 1280 */ 518019,
/* 1281 */ 9999,
/* 1282 */ 516010,
/* 1283 */ 9999,
/* 1284 */ 488007,
/* 1285 */ 9999,
/* 1286 */ 488015,
/* 1287 */ 9999,
/* 1288 */ 489009,
/* 1289 */ 9999,
/* 1290 */ 9999,
/* 1291 */ 489014,
/* 1292 */ 9999,
/* 1293 */ 9999,
/* 1294 */ 489014,
/* 1295 */ 9999,
/* 1296 */ 490004,
/* 1297 */ 9999,
/* 1298 */ 490009,
/* 1299 */ 9999,
/* 1300 */ 9999,
/* 1301 */ 490009,
/* 1302 */ 9999,
/* 1303 */ 9999,
/* 1304 */ 9999,
/* 1305 */ 492006,
/* 1306 */ 9999,
/* 1307 */ 9999,
/* 1308 */ 492006,
/* 1309 */ 9999,
/* 1310 */ 9999,
/* 1311 */ 493011,
/* 1312 */ 493040,
/* 1313 */ 9999,
/* 1314 */ 9999,
/* 1315 */ 493007,
/* 1316 */ 9999,
/* 1317 */ 9999,
/* 1318 */ 493007,
/* 1319 */ 9999,
/* 1320 */ 9999,
/* 1321 */ 9999,
/* 1322 */ 493018,
/* 1323 */ 9999,
/* 1324 */ 9999,
/* 1325 */ 9999,
/* 1326 */ 493026,
/* 1327 */ 9999,
/* 1328 */ 9999,
/* 1329 */ 9999,
/* 1330 */ 493037,
/* 1331 */ 9999,
/* 1332 */ 9999,
/* 1333 */ 505005,
/* 1334 */ 506005,
/* 1335 */ 507005,
/* 1336 */ 508005,
/* 1337 */ 504006,
/* 1338 */ 9999,
/* 1339 */ 504006,
/* 1340 */ 9999,
/* 1341 */ 9999,
/* 1342 */ 504006,
/* 1343 */ 9999,
/* 1344 */ 505007,
/* 1345 */ 9999,
/* 1346 */ 505015,
/* 1347 */ 9999,
/* 1348 */ 9999,
/* 1349 */ 505015,
/* 1350 */ 9999,
/* 1351 */ 506006,
/* 1352 */ 9999,
/* 1353 */ 506011,
/* 1354 */ 9999,
/* 1355 */ 9999,
/* 1356 */ 506011,
/* 1357 */ 9999,
/* 1358 */ 9999,
/* 1359 */ 9999,
/* 1360 */ 507008,
/* 1361 */ 9999,
/* 1362 */ 9999,
/* 1363 */ 507008,
/* 1364 */ 9999,
/* 1365 */ 9999,
/* 1366 */ 508011,
/* 1367 */ 508040,
/* 1368 */ 9999,
/* 1369 */ 9999,
/* 1370 */ 508007,
/* 1371 */ 9999,
/* 1372 */ 9999,
/* 1373 */ 508007,
/* 1374 */ 9999,
/* 1375 */ 9999,
/* 1376 */ 9999,
/* 1377 */ 508018,
/* 1378 */ 9999,
/* 1379 */ 9999,
/* 1380 */ 9999,
/* 1381 */ 508026,
/* 1382 */ 9999,
/* 1383 */ 9999,
/* 1384 */ 9999,
/* 1385 */ 508037,
/* 1386 */ 9999,
/* 1387 */ 518021,
/* 1388 */ 9999,
/* 1389 */ 518026,
/* 1390 */ 9999,
/* 1391 */ 9999,
/* 1392 */ 518026,
/* 1393 */ 9999,
/* 1394 */ 527003,
/* 1395 */ 527015,
/* 1396 */ 9999,
/* 1397 */ 527008,
/* 1398 */ 9999,
/* 1399 */ 9999,
/* 1400 */ 528009,
/* 1401 */ 528020,
/* 1402 */ 9999,
/* 1403 */ 528005,
/* 1404 */ 9999,
/* 1405 */ 9999,
/* 1406 */ 9999,
/* 1407 */ 527016,
/* 1408 */ 9999,
/* 1409 */ 9999,
/* 1410 */ 527016,
/* 1411 */ 9999,
/* 1412 */ 9999,
/* 1413 */ 9999,
/* 1414 */ 528021,
/* 1415 */ 9999,
/* 1416 */ 9999,
/* 1417 */ 528021,
/* 1418 */ 9999,
/* 1419 */ 9999,
/* 1420 */ 532017,
/* 1421 */ 532024,
/* 1422 */ 9999,
/* 1423 */ 532011,
/* 1424 */ 9999,
/* 1425 */ 9999,
/* 1426 */ 9999,
/* 1427 */ 532018,
/* 1428 */ 9999,
/* 1429 */ 9999,
/* 1430 */ 532018,
/* 1431 */ 9999,
/* 1432 */ 9999,
/* 1433 */ 9999,
/* 1434 */ 9999,
/* 1435 */ 541009,
/* 1436 */ 9999,
/* 1437 */ 542009,
/* 1438 */ 9999,
/* 1439 */ 9999,
/* 1440 */ 536006,
/* 1441 */ 9999,
/* 1442 */ 542010,
/* 1443 */ 542019,
/* 1444 */ 9999,
/* 1445 */ 542019,
/* 1446 */ 9999,
/* 1447 */ 9999,
/* 1448 */ 542019,
/* 1449 */ 9999,
/* 1450 */ 9999,
/* 1451 */ 546015,
/* 1452 */ 9999,
/* 1453 */ 9999,
/* 1454 */ 9999,
/* 1455 */ 546013,
/* 1456 */ 9999,
/* 1457 */ 9999,
/* 1458 */ 547017,
/* 1459 */ 547024,
/* 1460 */ 9999,
/* 1461 */ 9999,
/* 1462 */ 547015,
/* 1463 */ 9999,
/* 1464 */ 548005,
/* 1465 */ 9999,
/* 1466 */ 548015,
/* 1467 */ 9999,
/* 1468 */ 9999,
/* 1469 */ 549010,
/* 1470 */ 549017,
/* 1471 */ 9999,
/* 1472 */ 9999,
/* 1473 */ 549008,
/* 1474 */ 9999,
/* 1475 */ 9999,
/* 1476 */ 550010,
/* 1477 */ 550017,
/* 1478 */ 9999,
/* 1479 */ 9999,
/* 1480 */ 550008,
/* 1481 */ 9999,
/* 1482 */ 9999,
/* 1483 */ 551010,
/* 1484 */ 551017,
/* 1485 */ 9999,
/* 1486 */ 9999,
/* 1487 */ 551008,
/* 1488 */ 9999,
/* 1489 */ 9999,
/* 1490 */ 552012,
/* 1491 */ 552019,
/* 1492 */ 9999,
/* 1493 */ 9999,
/* 1494 */ 552010,
/* 1495 */ 9999,
/* 1496 */ 9999,
/* 1497 */ 553009,
/* 1498 */ 553016,
/* 1499 */ 9999,
/* 1500 */ 9999,
/* 1501 */ 553007,
/* 1502 */ 9999,
/* 1503 */ 9999,
/* 1504 */ 554011,
/* 1505 */ 554018,
/* 1506 */ 9999,
/* 1507 */ 9999,
/* 1508 */ 554009,
/* 1509 */ 9999,
/* 1510 */ 9999,
/* 1511 */ 555011,
/* 1512 */ 555018,
/* 1513 */ 9999,
/* 1514 */ 9999,
/* 1515 */ 9999,
/* 1516 */ 555009,
/* 1517 */ 9999,
/* 1518 */ 9999,
/* 1519 */ 556011,
/* 1520 */ 9999,
/* 1521 */ 556022,
/* 1522 */ 9999,
/* 1523 */ 9999,
/* 1524 */ 9999,
/* 1525 */ 556009,
/* 1526 */ 9999,
/* 1527 */ 9999,
/* 1528 */ 557011,
/* 1529 */ 557018,
/* 1530 */ 9999,
/* 1531 */ 9999,
/* 1532 */ 557009,
/* 1533 */ 9999,
/* 1534 */ 9999,
/* 1535 */ 558011,
/* 1536 */ 558018,
/* 1537 */ 9999,
/* 1538 */ 9999,
/* 1539 */ 558009,
/* 1540 */ 9999,
/* 1541 */ 9999,
/* 1542 */ 559014,
/* 1543 */ 559021,
/* 1544 */ 9999,
/* 1545 */ 9999,
/* 1546 */ 559012,
/* 1547 */ 9999,
/* 1548 */ 9999,
/* 1549 */ 560010,
/* 1550 */ 560017,
/* 1551 */ 561005,
/* 1552 */ 9999,
/* 1553 */ 9999,
/* 1554 */ 560008,
/* 1555 */ 9999,
/* 1556 */ 9999,
/* 1557 */ 571012,
/* 1558 */ 571019,
/* 1559 */ 9999,
/* 1560 */ 9999,
/* 1561 */ 571010,
/* 1562 */ 9999,
/* 1563 */ 9999,
/* 1564 */ 578010,
/* 1565 */ 578017,
/* 1566 */ 9999,
/* 1567 */ 9999,
/* 1568 */ 578008,
/* 1569 */ 9999,
/* 1570 */ 9999,
/* 1571 */ 579015,
/* 1572 */ 579022,
/* 1573 */ 9999,
/* 1574 */ 9999,
/* 1575 */ 579013,
/* 1576 */ 9999,
/* 1577 */ 9999,
/* 1578 */ 580009,
/* 1579 */ 580016,
/* 1580 */ 9999,
/* 1581 */ 9999,
/* 1582 */ 580007,
/* 1583 */ 9999,
/* 1584 */ 9999,
/* 1585 */ 581014,
/* 1586 */ 581021,
/* 1587 */ 9999,
/* 1588 */ 9999,
/* 1589 */ 581012,
/* 1590 */ 9999,
/* 1591 */ 9999,
/* 1592 */ 582013,
/* 1593 */ 582020,
/* 1594 */ 9999,
/* 1595 */ 9999,
/* 1596 */ 582011,
/* 1597 */ 9999,
/* 1598 */ 9999,
/* 1599 */ 583009,
/* 1600 */ 583016,
/* 1601 */ 9999,
/* 1602 */ 9999,
/* 1603 */ 583007,
/* 1604 */ 9999,
/* 1605 */ 9999,
/* 1606 */ 584015,
/* 1607 */ 584022,
/* 1608 */ 9999,
/* 1609 */ 9999,
/* 1610 */ 584013,
/* 1611 */ 9999,
/* 1612 */ 9999,
/* 1613 */ 585012,
/* 1614 */ 585019,
/* 1615 */ 9999,
/* 1616 */ 9999,
/* 1617 */ 585010,
/* 1618 */ 9999,
/* 1619 */ 9999,
/* 1620 */ 587011,
/* 1621 */ 587018,
/* 1622 */ 9999,
/* 1623 */ 9999,
/* 1624 */ 587009,
/* 1625 */ 9999,
/* 1626 */ 9999,
/* 1627 */ 589011,
/* 1628 */ 589018,
/* 1629 */ 9999,
/* 1630 */ 9999,
/* 1631 */ 589009,
/* 1632 */ 9999,
/* 1633 */ 9999,
/* 1634 */ 590011,
/* 1635 */ 590018,
/* 1636 */ 9999,
/* 1637 */ 9999,
/* 1638 */ 590009,
/* 1639 */ 9999,
/* 1640 */ 9999,
/* 1641 */ 591014,
/* 1642 */ 591021,
/* 1643 */ 9999,
/* 1644 */ 9999,
/* 1645 */ 591012,
/* 1646 */ 9999,
/* 1647 */ 9999,
/* 1648 */ 592016,
/* 1649 */ 592023,
/* 1650 */ 9999,
/* 1651 */ 9999,
/* 1652 */ 592014,
/* 1653 */ 9999,
/* 1654 */ 9999,
/* 1655 */ 593016,
/* 1656 */ 593023,
/* 1657 */ 9999,
/* 1658 */ 9999,
/* 1659 */ 593014,
/* 1660 */ 9999,
/* 1661 */ 9999,
/* 1662 */ 594014,
/* 1663 */ 594021,
/* 1664 */ 9999,
/* 1665 */ 9999,
/* 1666 */ 594012,
/* 1667 */ 9999,
/* 1668 */ 9999,
/* 1669 */ 595013,
/* 1670 */ 595020,
/* 1671 */ 9999,
/* 1672 */ 9999,
/* 1673 */ 595011,
/* 1674 */ 9999,
/* 1675 */ 9999,
/* 1676 */ 596011,
/* 1677 */ 596018,
/* 1678 */ 9999,
/* 1679 */ 9999,
/* 1680 */ 596009,
/* 1681 */ 9999,
/* 1682 */ 9999,
/* 1683 */ 9999,
/* 1684 */ 597017,
/* 1685 */ 597024,
/* 1686 */ 9999,
/* 1687 */ 9999,
/* 1688 */ 597009,
/* 1689 */ 9999,
/* 1690 */ 9999,
/* 1691 */ 598014,
/* 1692 */ 598021,
/* 1693 */ 9999,
/* 1694 */ 9999,
/* 1695 */ 598012,
/* 1696 */ 9999,
/* 1697 */ 9999,
/* 1698 */ 599010,
/* 1699 */ 599017,
/* 1700 */ 9999,
/* 1701 */ 9999,
/* 1702 */ 599008,
/* 1703 */ 9999,
/* 1704 */ 600005,
/* 1705 */ 9999,
/* 1706 */ 600016,
/* 1707 */ 9999,
/* 1708 */ 601005,
/* 1709 */ 9999,
/* 1710 */ 601014,
/* 1711 */ 9999,
/* 1712 */ 602005,
/* 1713 */ 9999,
/* 1714 */ 602017,
/* 1715 */ 9999,
/* 1716 */ 9999,
/* 1717 */ 9999,
/* 1718 */ 546016,
/* 1719 */ 9999,
/* 1720 */ 9999,
/* 1721 */ 546016,
/* 1722 */ 9999,
/* 1723 */ 9999,
/* 1724 */ 9999,
/* 1725 */ 547018,
/* 1726 */ 9999,
/* 1727 */ 9999,
/* 1728 */ 547018,
/* 1729 */ 9999,
/* 1730 */ 9999,
/* 1731 */ 9999,
/* 1732 */ 549011,
/* 1733 */ 9999,
/* 1734 */ 9999,
/* 1735 */ 549011,
/* 1736 */ 9999,
/* 1737 */ 9999,
/* 1738 */ 9999,
/* 1739 */ 550011,
/* 1740 */ 9999,
/* 1741 */ 9999,
/* 1742 */ 550011,
/* 1743 */ 9999,
/* 1744 */ 9999,
/* 1745 */ 9999,
/* 1746 */ 551011,
/* 1747 */ 9999,
/* 1748 */ 9999,
/* 1749 */ 551011,
/* 1750 */ 9999,
/* 1751 */ 9999,
/* 1752 */ 9999,
/* 1753 */ 552013,
/* 1754 */ 9999,
/* 1755 */ 9999,
/* 1756 */ 552013,
/* 1757 */ 9999,
/* 1758 */ 9999,
/* 1759 */ 9999,
/* 1760 */ 553010,
/* 1761 */ 9999,
/* 1762 */ 9999,
/* 1763 */ 553010,
/* 1764 */ 9999,
/* 1765 */ 9999,
/* 1766 */ 9999,
/* 1767 */ 554012,
/* 1768 */ 9999,
/* 1769 */ 9999,
/* 1770 */ 554012,
/* 1771 */ 9999,
/* 1772 */ 9999,
/* 1773 */ 9999,
/* 1774 */ 555012,
/* 1775 */ 9999,
/* 1776 */ 9999,
/* 1777 */ 555012,
/* 1778 */ 9999,
/* 1779 */ 9999,
/* 1780 */ 9999,
/* 1781 */ 556012,
/* 1782 */ 9999,
/* 1783 */ 9999,
/* 1784 */ 556012,
/* 1785 */ 9999,
/* 1786 */ 9999,
/* 1787 */ 9999,
/* 1788 */ 557012,
/* 1789 */ 9999,
/* 1790 */ 9999,
/* 1791 */ 557012,
/* 1792 */ 9999,
/* 1793 */ 9999,
/* 1794 */ 9999,
/* 1795 */ 558012,
/* 1796 */ 9999,
/* 1797 */ 9999,
/* 1798 */ 558012,
/* 1799 */ 9999,
/* 1800 */ 9999,
/* 1801 */ 9999,
/* 1802 */ 559015,
/* 1803 */ 9999,
/* 1804 */ 9999,
/* 1805 */ 559015,
/* 1806 */ 9999,
/* 1807 */ 9999,
/* 1808 */ 9999,
/* 1809 */ 560011,
/* 1810 */ 9999,
/* 1811 */ 9999,
/* 1812 */ 560011,
/* 1813 */ 9999,
/* 1814 */ 561007,
/* 1815 */ 9999,
/* 1816 */ 561012,
/* 1817 */ 9999,
/* 1818 */ 9999,
/* 1819 */ 561012,
/* 1820 */ 9999,
/* 1821 */ 9999,
/* 1822 */ 9999,
/* 1823 */ 571013,
/* 1824 */ 9999,
/* 1825 */ 9999,
/* 1826 */ 571013,
/* 1827 */ 9999,
/* 1828 */ 9999,
/* 1829 */ 9999,
/* 1830 */ 578011,
/* 1831 */ 9999,
/* 1832 */ 9999,
/* 1833 */ 578011,
/* 1834 */ 9999,
/* 1835 */ 9999,
/* 1836 */ 9999,
/* 1837 */ 579016,
/* 1838 */ 9999,
/* 1839 */ 9999,
/* 1840 */ 579016,
/* 1841 */ 9999,
/* 1842 */ 9999,
/* 1843 */ 9999,
/* 1844 */ 580010,
/* 1845 */ 9999,
/* 1846 */ 9999,
/* 1847 */ 580010,
/* 1848 */ 9999,
/* 1849 */ 9999,
/* 1850 */ 9999,
/* 1851 */ 581015,
/* 1852 */ 9999,
/* 1853 */ 9999,
/* 1854 */ 581015,
/* 1855 */ 9999,
/* 1856 */ 9999,
/* 1857 */ 9999,
/* 1858 */ 582014,
/* 1859 */ 9999,
/* 1860 */ 9999,
/* 1861 */ 582014,
/* 1862 */ 9999,
/* 1863 */ 9999,
/* 1864 */ 9999,
/* 1865 */ 583010,
/* 1866 */ 9999,
/* 1867 */ 9999,
/* 1868 */ 583010,
/* 1869 */ 9999,
/* 1870 */ 583017,
/* 1871 */ 583025,
/* 1872 */ 9999,
/* 1873 */ 583025,
/* 1874 */ 9999,
/* 1875 */ 9999,
/* 1876 */ 583025,
/* 1877 */ 9999,
/* 1878 */ 9999,
/* 1879 */ 9999,
/* 1880 */ 584016,
/* 1881 */ 9999,
/* 1882 */ 9999,
/* 1883 */ 584016,
/* 1884 */ 9999,
/* 1885 */ 9999,
/* 1886 */ 9999,
/* 1887 */ 585013,
/* 1888 */ 9999,
/* 1889 */ 9999,
/* 1890 */ 585013,
/* 1891 */ 9999,
/* 1892 */ 585021,
/* 1893 */ 586006,
/* 1894 */ 585027,
/* 1895 */ 9999,
/* 1896 */ 585027,
/* 1897 */ 9999,
/* 1898 */ 9999,
/* 1899 */ 585027,
/* 1900 */ 9999,
/* 1901 */ 9999,
/* 1902 */ 9999,
/* 1903 */ 586007,
/* 1904 */ 9999,
/* 1905 */ 9999,
/* 1906 */ 586007,
/* 1907 */ 9999,
/* 1908 */ 9999,
/* 1909 */ 9999,
/* 1910 */ 587012,
/* 1911 */ 9999,
/* 1912 */ 9999,
/* 1913 */ 587012,
/* 1914 */ 9999,
/* 1915 */ 587020,
/* 1916 */ 588006,
/* 1917 */ 587026,
/* 1918 */ 9999,
/* 1919 */ 587026,
/* 1920 */ 9999,
/* 1921 */ 9999,
/* 1922 */ 587026,
/* 1923 */ 9999,
/* 1924 */ 9999,
/* 1925 */ 9999,
/* 1926 */ 588007,
/* 1927 */ 9999,
/* 1928 */ 9999,
/* 1929 */ 588007,
/* 1930 */ 9999,
/* 1931 */ 9999,
/* 1932 */ 9999,
/* 1933 */ 589012,
/* 1934 */ 9999,
/* 1935 */ 9999,
/* 1936 */ 589012,
/* 1937 */ 9999,
/* 1938 */ 589019,
/* 1939 */ 589029,
/* 1940 */ 9999,
/* 1941 */ 589029,
/* 1942 */ 9999,
/* 1943 */ 9999,
/* 1944 */ 589029,
/* 1945 */ 9999,
/* 1946 */ 9999,
/* 1947 */ 9999,
/* 1948 */ 590012,
/* 1949 */ 9999,
/* 1950 */ 9999,
/* 1951 */ 590012,
/* 1952 */ 9999,
/* 1953 */ 590020,
/* 1954 */ 590030,
/* 1955 */ 590025,
/* 1956 */ 9999,
/* 1957 */ 590025,
/* 1958 */ 9999,
/* 1959 */ 9999,
/* 1960 */ 590025,
/* 1961 */ 9999,
/* 1962 */ 9999,
/* 1963 */ 9999,
/* 1964 */ 590031,
/* 1965 */ 9999,
/* 1966 */ 9999,
/* 1967 */ 9999,
/* 1968 */ 590035,
/* 1969 */ 9999,
/* 1970 */ 9999,
/* 1971 */ 590035,
/* 1972 */ 9999,
/* 1973 */ 9999,
/* 1974 */ 9999,
/* 1975 */ 591015,
/* 1976 */ 9999,
/* 1977 */ 9999,
/* 1978 */ 591015,
/* 1979 */ 9999,
/* 1980 */ 9999,
/* 1981 */ 9999,
/* 1982 */ 592017,
/* 1983 */ 9999,
/* 1984 */ 9999,
/* 1985 */ 592017,
/* 1986 */ 9999,
/* 1987 */ 9999,
/* 1988 */ 9999,
/* 1989 */ 593017,
/* 1990 */ 9999,
/* 1991 */ 9999,
/* 1992 */ 593017,
/* 1993 */ 9999,
/* 1994 */ 9999,
/* 1995 */ 9999,
/* 1996 */ 594015,
/* 1997 */ 9999,
/* 1998 */ 9999,
/* 1999 */ 594015,
/* 2000 */ 9999,
/* 2001 */ 9999,
/* 2002 */ 9999,
/* 2003 */ 595014,
/* 2004 */ 9999,
/* 2005 */ 9999,
/* 2006 */ 595014,
/* 2007 */ 9999,
/* 2008 */ 9999,
/* 2009 */ 9999,
/* 2010 */ 596012,
/* 2011 */ 9999,
/* 2012 */ 9999,
/* 2013 */ 596012,
/* 2014 */ 9999,
/* 2015 */ 9999,
/* 2016 */ 9999,
/* 2017 */ 597018,
/* 2018 */ 9999,
/* 2019 */ 9999,
/* 2020 */ 597018,
/* 2021 */ 9999,
/* 2022 */ 9999,
/* 2023 */ 9999,
/* 2024 */ 598015,
/* 2025 */ 9999,
/* 2026 */ 9999,
/* 2027 */ 598015,
/* 2028 */ 9999,
/* 2029 */ 9999,
/* 2030 */ 9999,
/* 2031 */ 599011,
/* 2032 */ 9999,
/* 2033 */ 9999,
/* 2034 */ 599011,
/* 2035 */ 9999,
/* 2036 */ 606003,
/* 2037 */ 606014,
/* 2038 */ 9999,
/* 2039 */ 606008,
/* 2040 */ 9999,
/* 2041 */ 9999,
/* 2042 */ 610018,
/* 2043 */ 610028,
/* 2044 */ 9999,
/* 2045 */ 610012,
/* 2046 */ 9999,
/* 2047 */ 9999,
/* 2048 */ 9999,
/* 2049 */ 606015,
/* 2050 */ 9999,
/* 2051 */ 9999,
/* 2052 */ 606015,
/* 2053 */ 9999,
/* 2054 */ 9999,
/* 2055 */ 9999,
/* 2056 */ 610029,
/* 2057 */ 9999,
/* 2058 */ 9999,
/* 2059 */ 610029,
/* 2060 */ 9999,
/* 2061 */ 614005,
/* 2062 */ 614018,
/* 2063 */ 9999,
/* 2064 */ 614010,
/* 2065 */ 9999,
/* 2066 */ 9999,
/* 2067 */ 615009,
/* 2068 */ 615021,
/* 2069 */ 9999,
/* 2070 */ 615005,
/* 2071 */ 9999,
/* 2072 */ 9999,
/* 2073 */ 9999,
/* 2074 */ 614019,
/* 2075 */ 9999,
/* 2076 */ 9999,
/* 2077 */ 614019,
/* 2078 */ 9999,
/* 2079 */ 9999,
/* 2080 */ 9999,
/* 2081 */ 615022,
/* 2082 */ 9999,
/* 2083 */ 9999,
/* 2084 */ 615022,
/* 2085 */ 9999,
/* 2086 */ 9999,
/* 2087 */ 9999,
/* 2088 */ 9999,
/* 2089 */ 622007,
/* 2090 */ 9999,
/* 2091 */ 623007,
/* 2092 */ 9999,
/* 2093 */ 9999,
/* 2094 */ 620009,
/* 2095 */ 9999,
/* 2096 */ 623008,
/* 2097 */ 623017,
/* 2098 */ 9999,
/* 2099 */ 623017,
/* 2100 */ 9999,
/* 2101 */ 9999,
/* 2102 */ 623017,
/* 2103 */ 9999,
/* 2104 */ 9999,
/* 2105 */ 627016,
/* 2106 */ 9999,
/* 2107 */ 9999,
/* 2108 */ 9999,
/* 2109 */ 627014,
/* 2110 */ 9999,
/* 2111 */ 9999,
/* 2112 */ 628015,
/* 2113 */ 628022,
/* 2114 */ 9999,
/* 2115 */ 9999,
/* 2116 */ 628013,
/* 2117 */ 9999,
/* 2118 */ 629005,
/* 2119 */ 9999,
/* 2120 */ 629015,
/* 2121 */ 9999,
/* 2122 */ 9999,
/* 2123 */ 630010,
/* 2124 */ 630017,
/* 2125 */ 9999,
/* 2126 */ 9999,
/* 2127 */ 9999,
/* 2128 */ 630008,
/* 2129 */ 9999,
/* 2130 */ 9999,
/* 2131 */ 631015,
/* 2132 */ 631022,
/* 2133 */ 9999,
/* 2134 */ 9999,
/* 2135 */ 631013,
/* 2136 */ 9999,
/* 2137 */ 9999,
/* 2138 */ 632011,
/* 2139 */ 632019,
/* 2140 */ 9999,
/* 2141 */ 9999,
/* 2142 */ 632009,
/* 2143 */ 9999,
/* 2144 */ 633005,
/* 2145 */ 633028,
/* 2146 */ 633035,
/* 2147 */ 9999,
/* 2148 */ 9999,
/* 2149 */ 633005,
/* 2150 */ 9999,
/* 2151 */ 9999,
/* 2152 */ 634010,
/* 2153 */ 634017,
/* 2154 */ 9999,
/* 2155 */ 9999,
/* 2156 */ 634008,
/* 2157 */ 9999,
/* 2158 */ 9999,
/* 2159 */ 635012,
/* 2160 */ 635019,
/* 2161 */ 9999,
/* 2162 */ 9999,
/* 2163 */ 635010,
/* 2164 */ 9999,
/* 2165 */ 636005,
/* 2166 */ 9999,
/* 2167 */ 636014,
/* 2168 */ 9999,
/* 2169 */ 637005,
/* 2170 */ 9999,
/* 2171 */ 637016,
/* 2172 */ 9999,
/* 2173 */ 638005,
/* 2174 */ 9999,
/* 2175 */ 638017,
/* 2176 */ 9999,
/* 2177 */ 9999,
/* 2178 */ 9999,
/* 2179 */ 627017,
/* 2180 */ 9999,
/* 2181 */ 9999,
/* 2182 */ 627017,
/* 2183 */ 9999,
/* 2184 */ 9999,
/* 2185 */ 9999,
/* 2186 */ 628016,
/* 2187 */ 9999,
/* 2188 */ 9999,
/* 2189 */ 628016,
/* 2190 */ 9999,
/* 2191 */ 9999,
/* 2192 */ 9999,
/* 2193 */ 630011,
/* 2194 */ 9999,
/* 2195 */ 9999,
/* 2196 */ 630011,
/* 2197 */ 9999,
/* 2198 */ 9999,
/* 2199 */ 9999,
/* 2200 */ 631016,
/* 2201 */ 9999,
/* 2202 */ 9999,
/* 2203 */ 631016,
/* 2204 */ 9999,
/* 2205 */ 9999,
/* 2206 */ 9999,
/* 2207 */ 632012,
/* 2208 */ 9999,
/* 2209 */ 9999,
/* 2210 */ 632012,
/* 2211 */ 9999,
/* 2212 */ 632020,
/* 2213 */ 632030,
/* 2214 */ 632025,
/* 2215 */ 9999,
/* 2216 */ 632025,
/* 2217 */ 9999,
/* 2218 */ 9999,
/* 2219 */ 632025,
/* 2220 */ 9999,
/* 2221 */ 9999,
/* 2222 */ 9999,
/* 2223 */ 632031,
/* 2224 */ 9999,
/* 2225 */ 9999,
/* 2226 */ 9999,
/* 2227 */ 632037,
/* 2228 */ 9999,
/* 2229 */ 9999,
/* 2230 */ 632037,
/* 2231 */ 9999,
/* 2232 */ 9999,
/* 2233 */ 9999,
/* 2234 */ 633016,
/* 2235 */ 9999,
/* 2236 */ 9999,
/* 2237 */ 9999,
/* 2238 */ 633025,
/* 2239 */ 9999,
/* 2240 */ 9999,
/* 2241 */ 9999,
/* 2242 */ 633029,
/* 2243 */ 9999,
/* 2244 */ 9999,
/* 2245 */ 633029,
/* 2246 */ 9999,
/* 2247 */ 9999,
/* 2248 */ 9999,
/* 2249 */ 634011,
/* 2250 */ 9999,
/* 2251 */ 9999,
/* 2252 */ 634011,
/* 2253 */ 9999,
/* 2254 */ 9999,
/* 2255 */ 9999,
/* 2256 */ 635013,
/* 2257 */ 9999,
/* 2258 */ 9999,
/* 2259 */ 635013,
/* 2260 */ 9999,
/* 2261 */ 642003,
/* 2262 */ 651003,
/* 2263 */ 9999,
/* 2264 */ 9999,
/* 2265 */ 655007,
/* 2266 */ 9999,
/* 2267 */ 657007,
/* 2268 */ 9999,
/* 2269 */ 658003,
/* 2270 */ 9999,
/* 2271 */ 641037,
/* 2272 */ 9999,
/* 2273 */ 642005,
/* 2274 */ 650005,
/* 2275 */ 642010,
/* 2276 */ 9999,
/* 2277 */ 642010,
/* 2278 */ 9999,
/* 2279 */ 9999,
/* 2280 */ 642010,
/* 2281 */ 9999,
/* 2282 */ 9999,
/* 2283 */ 9999,
/* 2284 */ 650006,
/* 2285 */ 9999,
/* 2286 */ 9999,
/* 2287 */ 650006,
/* 2288 */ 9999,
/* 2289 */ 9999,
/* 2290 */ 9999,
/* 2291 */ 651008,
/* 2292 */ 9999,
/* 2293 */ 9999,
/* 2294 */ 9999,
/* 2295 */ 651017,
/* 2296 */ 9999,
/* 2297 */ 9999,
/* 2298 */ 9999,
/* 2299 */ 652012,
/* 2300 */ 9999,
/* 2301 */ 9999,
/* 2302 */ 9999,
/* 2303 */ 653013,
/* 2304 */ 9999,
/* 2305 */ 657008,
/* 2306 */ 657016,
/* 2307 */ 9999,
/* 2308 */ 657016,
/* 2309 */ 9999,
/* 2310 */ 9999,
/* 2311 */ 657016,
/* 2312 */ 9999,
/* 2313 */ 9999,
/* 2314 */ 658009,
/* 2315 */ 9999,
/* 2316 */ 679008,
/* 2317 */ 9999,
/* 2318 */ 658007,
/* 2319 */ 9999,
/* 2320 */ 658007,
/* 2321 */ 9999,
/* 2322 */ 9999,
/* 2323 */ 658007,
/* 2324 */ 9999,
/* 2325 */ 679009,
/* 2326 */ 679017,
/* 2327 */ 9999,
/* 2328 */ 679017,
/* 2329 */ 9999,
/* 2330 */ 9999,
/* 2331 */ 679017,
/* 2332 */ 9999,
/* 2333 */ 9999,
/* 2334 */ 684015,
/* 2335 */ 9999,
/* 2336 */ 9999,
/* 2337 */ 9999,
/* 2338 */ 684013,
/* 2339 */ 9999,
/* 2340 */ 685005,
/* 2341 */ 9999,
/* 2342 */ 9999,
/* 2343 */ 685009,
/* 2344 */ 9999,
/* 2345 */ 9999,
/* 2346 */ 686016,
/* 2347 */ 686023,
/* 2348 */ 9999,
/* 2349 */ 9999,
/* 2350 */ 686014,
/* 2351 */ 9999,
/* 2352 */ 9999,
/* 2353 */ 687010,
/* 2354 */ 687017,
/* 2355 */ 9999,
/* 2356 */ 9999,
/* 2357 */ 687008,
/* 2358 */ 9999,
/* 2359 */ 9999,
/* 2360 */ 688010,
/* 2361 */ 688020,
/* 2362 */ 688027,
/* 2363 */ 688037,
/* 2364 */ 9999,
/* 2365 */ 9999,
/* 2366 */ 688008,
/* 2367 */ 9999,
/* 2368 */ 9999,
/* 2369 */ 689013,
/* 2370 */ 689020,
/* 2371 */ 690007,
/* 2372 */ 9999,
/* 2373 */ 9999,
/* 2374 */ 689011,
/* 2375 */ 9999,
/* 2376 */ 9999,
/* 2377 */ 691010,
/* 2378 */ 691017,
/* 2379 */ 691027,
/* 2380 */ 9999,
/* 2381 */ 9999,
/* 2382 */ 691008,
/* 2383 */ 9999,
/* 2384 */ 9999,
/* 2385 */ 693010,
/* 2386 */ 693017,
/* 2387 */ 9999,
/* 2388 */ 9999,
/* 2389 */ 693008,
/* 2390 */ 9999,
/* 2391 */ 9999,
/* 2392 */ 694010,
/* 2393 */ 694017,
/* 2394 */ 9999,
/* 2395 */ 9999,
/* 2396 */ 694008,
/* 2397 */ 9999,
/* 2398 */ 9999,
/* 2399 */ 695010,
/* 2400 */ 695017,
/* 2401 */ 9999,
/* 2402 */ 9999,
/* 2403 */ 695008,
/* 2404 */ 9999,
/* 2405 */ 9999,
/* 2406 */ 696010,
/* 2407 */ 696017,
/* 2408 */ 9999,
/* 2409 */ 9999,
/* 2410 */ 696008,
/* 2411 */ 9999,
/* 2412 */ 9999,
/* 2413 */ 697010,
/* 2414 */ 697017,
/* 2415 */ 9999,
/* 2416 */ 9999,
/* 2417 */ 697008,
/* 2418 */ 9999,
/* 2419 */ 9999,
/* 2420 */ 698010,
/* 2421 */ 698017,
/* 2422 */ 9999,
/* 2423 */ 9999,
/* 2424 */ 698008,
/* 2425 */ 9999,
/* 2426 */ 9999,
/* 2427 */ 699010,
/* 2428 */ 699017,
/* 2429 */ 9999,
/* 2430 */ 9999,
/* 2431 */ 699008,
/* 2432 */ 9999,
/* 2433 */ 9999,
/* 2434 */ 700010,
/* 2435 */ 700017,
/* 2436 */ 9999,
/* 2437 */ 9999,
/* 2438 */ 700008,
/* 2439 */ 9999,
/* 2440 */ 9999,
/* 2441 */ 701010,
/* 2442 */ 701017,
/* 2443 */ 9999,
/* 2444 */ 9999,
/* 2445 */ 701008,
/* 2446 */ 9999,
/* 2447 */ 9999,
/* 2448 */ 702010,
/* 2449 */ 702017,
/* 2450 */ 9999,
/* 2451 */ 9999,
/* 2452 */ 702008,
/* 2453 */ 9999,
/* 2454 */ 9999,
/* 2455 */ 703010,
/* 2456 */ 703017,
/* 2457 */ 9999,
/* 2458 */ 9999,
/* 2459 */ 703008,
/* 2460 */ 9999,
/* 2461 */ 9999,
/* 2462 */ 704010,
/* 2463 */ 9999,
/* 2464 */ 9999,
/* 2465 */ 9999,
/* 2466 */ 704008,
/* 2467 */ 9999,
/* 2468 */ 9999,
/* 2469 */ 705011,
/* 2470 */ 705018,
/* 2471 */ 9999,
/* 2472 */ 9999,
/* 2473 */ 705009,
/* 2474 */ 9999,
/* 2475 */ 707005,
/* 2476 */ 9999,
/* 2477 */ 707014,
/* 2478 */ 9999,
/* 2479 */ 708005,
/* 2480 */ 9999,
/* 2481 */ 708017,
/* 2482 */ 9999,
/* 2483 */ 709005,
/* 2484 */ 9999,
/* 2485 */ 709016,
/* 2486 */ 9999,
/* 2487 */ 9999,
/* 2488 */ 9999,
/* 2489 */ 684016,
/* 2490 */ 9999,
/* 2491 */ 9999,
/* 2492 */ 684016,
/* 2493 */ 9999,
/* 2494 */ 9999,
/* 2495 */ 9999,
/* 2496 */ 686017,
/* 2497 */ 9999,
/* 2498 */ 9999,
/* 2499 */ 686017,
/* 2500 */ 9999,
/* 2501 */ 9999,
/* 2502 */ 9999,
/* 2503 */ 687011,
/* 2504 */ 9999,
/* 2505 */ 9999,
/* 2506 */ 687011,
/* 2507 */ 9999,
/* 2508 */ 9999,
/* 2509 */ 9999,
/* 2510 */ 688016,
/* 2511 */ 9999,
/* 2512 */ 9999,
/* 2513 */ 688016,
/* 2514 */ 9999,
/* 2515 */ 9999,
/* 2516 */ 9999,
/* 2517 */ 688021,
/* 2518 */ 9999,
/* 2519 */ 9999,
/* 2520 */ 688021,
/* 2521 */ 9999,
/* 2522 */ 9999,
/* 2523 */ 9999,
/* 2524 */ 688038,
/* 2525 */ 9999,
/* 2526 */ 9999,
/* 2527 */ 688038,
/* 2528 */ 9999,
/* 2529 */ 9999,
/* 2530 */ 9999,
/* 2531 */ 689014,
/* 2532 */ 9999,
/* 2533 */ 9999,
/* 2534 */ 689014,
/* 2535 */ 9999,
/* 2536 */ 9999,
/* 2537 */ 690013,
/* 2538 */ 9999,
/* 2539 */ 9999,
/* 2540 */ 690009,
/* 2541 */ 9999,
/* 2542 */ 9999,
/* 2543 */ 690009,
/* 2544 */ 9999,
/* 2545 */ 9999,
/* 2546 */ 9999,
/* 2547 */ 691011,
/* 2548 */ 9999,
/* 2549 */ 9999,
/* 2550 */ 691011,
/* 2551 */ 9999,
/* 2552 */ 9999,
/* 2553 */ 9999,
/* 2554 */ 691035,
/* 2555 */ 9999,
/* 2556 */ 9999,
/* 2557 */ 691035,
/* 2558 */ 9999,
/* 2559 */ 9999,
/* 2560 */ 9999,
/* 2561 */ 693011,
/* 2562 */ 9999,
/* 2563 */ 9999,
/* 2564 */ 693011,
/* 2565 */ 9999,
/* 2566 */ 9999,
/* 2567 */ 9999,
/* 2568 */ 694011,
/* 2569 */ 9999,
/* 2570 */ 9999,
/* 2571 */ 694011,
/* 2572 */ 9999,
/* 2573 */ 9999,
/* 2574 */ 9999,
/* 2575 */ 695011,
/* 2576 */ 9999,
/* 2577 */ 9999,
/* 2578 */ 695011,
/* 2579 */ 9999,
/* 2580 */ 9999,
/* 2581 */ 9999,
/* 2582 */ 696011,
/* 2583 */ 9999,
/* 2584 */ 9999,
/* 2585 */ 696011,
/* 2586 */ 9999,
/* 2587 */ 9999,
/* 2588 */ 9999,
/* 2589 */ 697011,
/* 2590 */ 9999,
/* 2591 */ 9999,
/* 2592 */ 697011,
/* 2593 */ 9999,
/* 2594 */ 9999,
/* 2595 */ 9999,
/* 2596 */ 698011,
/* 2597 */ 9999,
/* 2598 */ 9999,
/* 2599 */ 698011,
/* 2600 */ 9999,
/* 2601 */ 9999,
/* 2602 */ 9999,
/* 2603 */ 699011,
/* 2604 */ 9999,
/* 2605 */ 9999,
/* 2606 */ 699011,
/* 2607 */ 9999,
/* 2608 */ 9999,
/* 2609 */ 9999,
/* 2610 */ 700011,
/* 2611 */ 9999,
/* 2612 */ 9999,
/* 2613 */ 700011,
/* 2614 */ 9999,
/* 2615 */ 9999,
/* 2616 */ 9999,
/* 2617 */ 701011,
/* 2618 */ 9999,
/* 2619 */ 9999,
/* 2620 */ 701011,
/* 2621 */ 9999,
/* 2622 */ 9999,
/* 2623 */ 9999,
/* 2624 */ 702011,
/* 2625 */ 9999,
/* 2626 */ 9999,
/* 2627 */ 702011,
/* 2628 */ 9999,
/* 2629 */ 9999,
/* 2630 */ 9999,
/* 2631 */ 703011,
/* 2632 */ 9999,
/* 2633 */ 9999,
/* 2634 */ 703011,
/* 2635 */ 9999,
/* 2636 */ 9999,
/* 2637 */ 9999,
/* 2638 */ 704011,
/* 2639 */ 9999,
/* 2640 */ 9999,
/* 2641 */ 704011,
/* 2642 */ 9999,
/* 2643 */ 9999,
/* 2644 */ 9999,
/* 2645 */ 705012,
/* 2646 */ 9999,
/* 2647 */ 9999,
/* 2648 */ 705012,
/* 2649 */ 9999,
/* 2650 */ 705019,
/* 2651 */ 706005,
/* 2652 */ 705024,
/* 2653 */ 9999,
/* 2654 */ 705024,
/* 2655 */ 9999,
/* 2656 */ 9999,
/* 2657 */ 705024,
/* 2658 */ 9999,
/* 2659 */ 9999,
/* 2660 */ 9999,
/* 2661 */ 706006,
/* 2662 */ 9999,
/* 2663 */ 9999,
/* 2664 */ 9999,
/* 2665 */ 706012,
/* 2666 */ 9999,
/* 2667 */ 9999,
/* 2668 */ 706012,
/* 2669 */ 9999,
/* 2670 */ 713003,
/* 2671 */ 9999,
/* 2672 */ 712015,
/* 2673 */ 9999,
/* 2674 */ 713004,
/* 2675 */ 713029,
/* 2676 */ 713009,
/* 2677 */ 9999,
/* 2678 */ 713009,
/* 2679 */ 9999,
/* 2680 */ 9999,
/* 2681 */ 713009,
/* 2682 */ 9999,
/* 2683 */ 9999,
/* 2684 */ 9999,
/* 2685 */ 713030,
/* 2686 */ 9999,
/* 2687 */ 9999,
/* 2688 */ 9999,
/* 2689 */ 713036,
/* 2690 */ 9999,
/* 2691 */ 9999,
/* 2692 */ 713036,
/* 2693 */ 9999,
/* 2694 */ 9999,
/* 2695 */ 9999,
/* 2696 */ 719009,
/* 2697 */ 9999,
/* 2698 */ 9999,
/* 2699 */ 9999,
/* 2700 */ 724005,
/* 2701 */ 9999,
/* 2702 */ 729007,
/* 2703 */ 9999,
/* 2704 */ 9999,
/* 2705 */ 722006,
/* 2706 */ 9999,
/* 2707 */ 9999,
/* 2708 */ 724011,
/* 2709 */ 9999,
/* 2710 */ 724007,
/* 2711 */ 9999,
/* 2712 */ 9999,
/* 2713 */ 724007,
/* 2714 */ 9999,
/* 2715 */ 729008,
/* 2716 */ 729017,
/* 2717 */ 9999,
/* 2718 */ 729017,
/* 2719 */ 9999,
/* 2720 */ 9999,
/* 2721 */ 729017,
/* 2722 */ 9999,
/* 2723 */ 9999,
/* 2724 */ 733014,
/* 2725 */ 9999,
/* 2726 */ 9999,
/* 2727 */ 9999,
/* 2728 */ 733012,
/* 2729 */ 9999,
/* 2730 */ 734004,
/* 2731 */ 9999,
/* 2732 */ 9999,
/* 2733 */ 734004,
/* 2734 */ 9999,
/* 2735 */ 9999,
/* 2736 */ 736012,
/* 2737 */ 736019,
/* 2738 */ 9999,
/* 2739 */ 9999,
/* 2740 */ 736010,
/* 2741 */ 9999,
/* 2742 */ 9999,
/* 2743 */ 738013,
/* 2744 */ 738020,
/* 2745 */ 9999,
/* 2746 */ 9999,
/* 2747 */ 738011,
/* 2748 */ 9999,
/* 2749 */ 9999,
/* 2750 */ 739013,
/* 2751 */ 739022,
/* 2752 */ 739029,
/* 2753 */ 9999,
/* 2754 */ 739045,
/* 2755 */ 9999,
/* 2756 */ 9999,
/* 2757 */ 739011,
/* 2758 */ 9999,
/* 2759 */ 9999,
/* 2760 */ 742013,
/* 2761 */ 742022,
/* 2762 */ 742029,
/* 2763 */ 9999,
/* 2764 */ 742045,
/* 2765 */ 9999,
/* 2766 */ 742062,
/* 2767 */ 9999,
/* 2768 */ 9999,
/* 2769 */ 742011,
/* 2770 */ 9999,
/* 2771 */ 9999,
/* 2772 */ 746012,
/* 2773 */ 747007,
/* 2774 */ 9999,
/* 2775 */ 9999,
/* 2776 */ 746010,
/* 2777 */ 9999,
/* 2778 */ 748005,
/* 2779 */ 748015,
/* 2780 */ 9999,
/* 2781 */ 9999,
/* 2782 */ 9999,
/* 2783 */ 748010,
/* 2784 */ 9999,
/* 2785 */ 9999,
/* 2786 */ 754011,
/* 2787 */ 754019,
/* 2788 */ 9999,
/* 2789 */ 9999,
/* 2790 */ 754009,
/* 2791 */ 9999,
/* 2792 */ 755005,
/* 2793 */ 9999,
/* 2794 */ 755013,
/* 2795 */ 9999,
/* 2796 */ 756005,
/* 2797 */ 9999,
/* 2798 */ 756016,
/* 2799 */ 9999,
/* 2800 */ 757005,
/* 2801 */ 9999,
/* 2802 */ 757014,
/* 2803 */ 9999,
/* 2804 */ 758005,
/* 2805 */ 9999,
/* 2806 */ 758017,
/* 2807 */ 9999,
/* 2808 */ 9999,
/* 2809 */ 9999,
/* 2810 */ 733015,
/* 2811 */ 9999,
/* 2812 */ 9999,
/* 2813 */ 733015,
/* 2814 */ 9999,
/* 2815 */ 9999,
/* 2816 */ 734019,
/* 2817 */ 734026,
/* 2818 */ 735003,
/* 2819 */ 734013,
/* 2820 */ 9999,
/* 2821 */ 734013,
/* 2822 */ 9999,
/* 2823 */ 9999,
/* 2824 */ 734013,
/* 2825 */ 9999,
/* 2826 */ 9999,
/* 2827 */ 9999,
/* 2828 */ 734020,
/* 2829 */ 9999,
/* 2830 */ 9999,
/* 2831 */ 734020,
/* 2832 */ 9999,
/* 2833 */ 9999,
/* 2834 */ 9999,
/* 2835 */ 735004,
/* 2836 */ 9999,
/* 2837 */ 9999,
/* 2838 */ 735004,
/* 2839 */ 9999,
/* 2840 */ 9999,
/* 2841 */ 9999,
/* 2842 */ 736013,
/* 2843 */ 9999,
/* 2844 */ 9999,
/* 2845 */ 736013,
/* 2846 */ 9999,
/* 2847 */ 736020,
/* 2848 */ 736054,
/* 2849 */ 736034,
/* 2850 */ 9999,
/* 2851 */ 736034,
/* 2852 */ 9999,
/* 2853 */ 9999,
/* 2854 */ 736034,
/* 2855 */ 9999,
/* 2856 */ 9999,
/* 2857 */ 9999,
/* 2858 */ 736055,
/* 2859 */ 9999,
/* 2860 */ 9999,
/* 2861 */ 736055,
/* 2862 */ 9999,
/* 2863 */ 9999,
/* 2864 */ 9999,
/* 2865 */ 738014,
/* 2866 */ 9999,
/* 2867 */ 9999,
/* 2868 */ 738014,
/* 2869 */ 9999,
/* 2870 */ 9999,
/* 2871 */ 9999,
/* 2872 */ 739018,
/* 2873 */ 9999,
/* 2874 */ 9999,
/* 2875 */ 739018,
/* 2876 */ 9999,
/* 2877 */ 9999,
/* 2878 */ 9999,
/* 2879 */ 739023,
/* 2880 */ 9999,
/* 2881 */ 9999,
/* 2882 */ 739023,
/* 2883 */ 9999,
/* 2884 */ 9999,
/* 2885 */ 9999,
/* 2886 */ 742018,
/* 2887 */ 9999,
/* 2888 */ 9999,
/* 2889 */ 742018,
/* 2890 */ 9999,
/* 2891 */ 9999,
/* 2892 */ 9999,
/* 2893 */ 742023,
/* 2894 */ 9999,
/* 2895 */ 9999,
/* 2896 */ 742023,
/* 2897 */ 9999,
/* 2898 */ 9999,
/* 2899 */ 9999,
/* 2900 */ 746013,
/* 2901 */ 9999,
/* 2902 */ 9999,
/* 2903 */ 746013,
/* 2904 */ 9999,
/* 2905 */ 747009,
/* 2906 */ 747053,
/* 2907 */ 747014,
/* 2908 */ 9999,
/* 2909 */ 747014,
/* 2910 */ 9999,
/* 2911 */ 9999,
/* 2912 */ 747014,
/* 2913 */ 9999,
/* 2914 */ 9999,
/* 2915 */ 9999,
/* 2916 */ 747054,
/* 2917 */ 9999,
/* 2918 */ 9999,
/* 2919 */ 747054,
/* 2920 */ 9999,
/* 2921 */ 9999,
/* 2922 */ 9999,
/* 2923 */ 748016,
/* 2924 */ 9999,
/* 2925 */ 9999,
/* 2926 */ 748016,
/* 2927 */ 9999,
/* 2928 */ 9999,
/* 2929 */ 9999,
/* 2930 */ 754012,
/* 2931 */ 9999,
/* 2932 */ 9999,
/* 2933 */ 754012,
/* 2934 */ 9999,
/* 2935 */ 754020,
/* 2936 */ 754030,
/* 2937 */ 754025,
/* 2938 */ 9999,
/* 2939 */ 754025,
/* 2940 */ 9999,
/* 2941 */ 9999,
/* 2942 */ 754025,
/* 2943 */ 9999,
/* 2944 */ 9999,
/* 2945 */ 9999,
/* 2946 */ 754031,
/* 2947 */ 9999,
/* 2948 */ 9999,
/* 2949 */ 9999,
/* 2950 */ 754037,
/* 2951 */ 9999,
/* 2952 */ 9999,
/* 2953 */ 754037,
/* 2954 */ 9999,
/* 2955 */ 9999,
/* 2956 */ 9999,
/* 2957 */ 9999,
/* 2958 */ 763007,
/* 2959 */ 9999,
/* 2960 */ 9999,
/* 2961 */ 762007,
/* 2962 */ 9999,
/* 2963 */ 763008,
/* 2964 */ 763018,
/* 2965 */ 9999,
/* 2966 */ 763018,
/* 2967 */ 9999,
/* 2968 */ 9999,
/* 2969 */ 763018,
/* 2970 */ 9999,
/* 2971 */ 9999,
/* 2972 */ 767015,
/* 2973 */ 9999,
/* 2974 */ 9999,
/* 2975 */ 9999,
/* 2976 */ 767013,
/* 2977 */ 9999,
/* 2978 */ 9999,
/* 2979 */ 768013,
/* 2980 */ 768020,
/* 2981 */ 768027,
/* 2982 */ 9999,
/* 2983 */ 9999,
/* 2984 */ 768011,
/* 2985 */ 9999,
/* 2986 */ 9999,
/* 2987 */ 769010,
/* 2988 */ 769017,
/* 2989 */ 769024,
/* 2990 */ 9999,
/* 2991 */ 9999,
/* 2992 */ 769008,
/* 2993 */ 9999,
/* 2994 */ 9999,
/* 2995 */ 770009,
/* 2996 */ 770016,
/* 2997 */ 9999,
/* 2998 */ 770030,
/* 2999 */ 9999,
/* 3000 */ 9999,
/* 3001 */ 770007,
/* 3002 */ 9999,
/* 3003 */ 9999,
/* 3004 */ 772012,
/* 3005 */ 773007,
/* 3006 */ 9999,
/* 3007 */ 9999,
/* 3008 */ 772010,
/* 3009 */ 9999,
/* 3010 */ 9999,
/* 3011 */ 774011,
/* 3012 */ 775007,
/* 3013 */ 9999,
/* 3014 */ 9999,
/* 3015 */ 774009,
/* 3016 */ 9999,
/* 3017 */ 9999,
/* 3018 */ 776012,
/* 3019 */ 776019,
/* 3020 */ 9999,
/* 3021 */ 9999,
/* 3022 */ 776010,
/* 3023 */ 9999,
/* 3024 */ 9999,
/* 3025 */ 778012,
/* 3026 */ 778018,
/* 3027 */ 778029,
/* 3028 */ 779007,
/* 3029 */ 779014,
/* 3030 */ 9999,
/* 3031 */ 9999,
/* 3032 */ 778010,
/* 3033 */ 9999,
/* 3034 */ 9999,
/* 3035 */ 780019,
/* 3036 */ 781007,
/* 3037 */ 9999,
/* 3038 */ 9999,
/* 3039 */ 780017,
/* 3040 */ 9999,
/* 3041 */ 9999,
/* 3042 */ 782011,
/* 3043 */ 782019,
/* 3044 */ 9999,
/* 3045 */ 9999,
/* 3046 */ 782009,
/* 3047 */ 9999,
/* 3048 */ 783005,
/* 3049 */ 9999,
/* 3050 */ 783013,
/* 3051 */ 9999,
/* 3052 */ 784005,
/* 3053 */ 9999,
/* 3054 */ 784014,
/* 3055 */ 9999,
/* 3056 */ 785005,
/* 3057 */ 9999,
/* 3058 */ 785016,
/* 3059 */ 9999,
/* 3060 */ 786005,
/* 3061 */ 9999,
/* 3062 */ 786017,
/* 3063 */ 9999,
/* 3064 */ 9999,
/* 3065 */ 9999,
/* 3066 */ 767016,
/* 3067 */ 9999,
/* 3068 */ 9999,
/* 3069 */ 767016,
/* 3070 */ 9999,
/* 3071 */ 9999,
/* 3072 */ 9999,
/* 3073 */ 768014,
/* 3074 */ 9999,
/* 3075 */ 9999,
/* 3076 */ 768014,
/* 3077 */ 9999,
/* 3078 */ 9999,
/* 3079 */ 9999,
/* 3080 */ 768021,
/* 3081 */ 9999,
/* 3082 */ 9999,
/* 3083 */ 768021,
/* 3084 */ 9999,
/* 3085 */ 9999,
/* 3086 */ 9999,
/* 3087 */ 769011,
/* 3088 */ 9999,
/* 3089 */ 9999,
/* 3090 */ 769011,
/* 3091 */ 9999,
/* 3092 */ 9999,
/* 3093 */ 9999,
/* 3094 */ 769018,
/* 3095 */ 9999,
/* 3096 */ 9999,
/* 3097 */ 769018,
/* 3098 */ 9999,
/* 3099 */ 9999,
/* 3100 */ 9999,
/* 3101 */ 770010,
/* 3102 */ 9999,
/* 3103 */ 9999,
/* 3104 */ 770010,
/* 3105 */ 9999,
/* 3106 */ 9999,
/* 3107 */ 9999,
/* 3108 */ 772013,
/* 3109 */ 9999,
/* 3110 */ 9999,
/* 3111 */ 772013,
/* 3112 */ 9999,
/* 3113 */ 773008,
/* 3114 */ 773020,
/* 3115 */ 773062,
/* 3116 */ 773013,
/* 3117 */ 9999,
/* 3118 */ 773013,
/* 3119 */ 9999,
/* 3120 */ 9999,
/* 3121 */ 773013,
/* 3122 */ 9999,
/* 3123 */ 9999,
/* 3124 */ 9999,
/* 3125 */ 773063,
/* 3126 */ 9999,
/* 3127 */ 9999,
/* 3128 */ 773063,
/* 3129 */ 9999,
/* 3130 */ 9999,
/* 3131 */ 9999,
/* 3132 */ 774012,
/* 3133 */ 9999,
/* 3134 */ 9999,
/* 3135 */ 774012,
/* 3136 */ 9999,
/* 3137 */ 775008,
/* 3138 */ 775020,
/* 3139 */ 775060,
/* 3140 */ 775013,
/* 3141 */ 9999,
/* 3142 */ 775013,
/* 3143 */ 9999,
/* 3144 */ 9999,
/* 3145 */ 775013,
/* 3146 */ 9999,
/* 3147 */ 9999,
/* 3148 */ 9999,
/* 3149 */ 775061,
/* 3150 */ 9999,
/* 3151 */ 9999,
/* 3152 */ 775061,
/* 3153 */ 9999,
/* 3154 */ 9999,
/* 3155 */ 9999,
/* 3156 */ 776013,
/* 3157 */ 9999,
/* 3158 */ 9999,
/* 3159 */ 776013,
/* 3160 */ 9999,
/* 3161 */ 776020,
/* 3162 */ 776054,
/* 3163 */ 776034,
/* 3164 */ 9999,
/* 3165 */ 776034,
/* 3166 */ 9999,
/* 3167 */ 9999,
/* 3168 */ 776034,
/* 3169 */ 9999,
/* 3170 */ 9999,
/* 3171 */ 9999,
/* 3172 */ 776055,
/* 3173 */ 9999,
/* 3174 */ 9999,
/* 3175 */ 776055,
/* 3176 */ 9999,
/* 3177 */ 9999,
/* 3178 */ 9999,
/* 3179 */ 778013,
/* 3180 */ 9999,
/* 3181 */ 9999,
/* 3182 */ 9999,
/* 3183 */ 778030,
/* 3184 */ 9999,
/* 3185 */ 9999,
/* 3186 */ 9999,
/* 3187 */ 779008,
/* 3188 */ 9999,
/* 3189 */ 9999,
/* 3190 */ 779008,
/* 3191 */ 9999,
/* 3192 */ 779016,
/* 3193 */ 779061,
/* 3194 */ 779021,
/* 3195 */ 9999,
/* 3196 */ 779021,
/* 3197 */ 9999,
/* 3198 */ 9999,
/* 3199 */ 779021,
/* 3200 */ 9999,
/* 3201 */ 9999,
/* 3202 */ 9999,
/* 3203 */ 779062,
/* 3204 */ 9999,
/* 3205 */ 9999,
/* 3206 */ 779062,
/* 3207 */ 9999,
/* 3208 */ 9999,
/* 3209 */ 9999,
/* 3210 */ 780020,
/* 3211 */ 9999,
/* 3212 */ 9999,
/* 3213 */ 780020,
/* 3214 */ 9999,
/* 3215 */ 781008,
/* 3216 */ 781054,
/* 3217 */ 781013,
/* 3218 */ 9999,
/* 3219 */ 781013,
/* 3220 */ 9999,
/* 3221 */ 9999,
/* 3222 */ 781013,
/* 3223 */ 9999,
/* 3224 */ 9999,
/* 3225 */ 9999,
/* 3226 */ 781055,
/* 3227 */ 9999,
/* 3228 */ 9999,
/* 3229 */ 781055,
/* 3230 */ 9999,
/* 3231 */ 9999,
/* 3232 */ 9999,
/* 3233 */ 782012,
/* 3234 */ 9999,
/* 3235 */ 9999,
/* 3236 */ 782012,
/* 3237 */ 9999,
/* 3238 */ 782020,
/* 3239 */ 782030,
/* 3240 */ 782025,
/* 3241 */ 9999,
/* 3242 */ 782025,
/* 3243 */ 9999,
/* 3244 */ 9999,
/* 3245 */ 782025,
/* 3246 */ 9999,
/* 3247 */ 9999,
/* 3248 */ 9999,
/* 3249 */ 782031,
/* 3250 */ 9999,
/* 3251 */ 9999,
/* 3252 */ 9999,
/* 3253 */ 782037,
/* 3254 */ 9999,
/* 3255 */ 9999,
/* 3256 */ 782037,
/* 3257 */ 9999,
/* 3258 */ 9999,
/* 3259 */ 9999,
/* 3260 */ 790027,
/* 3261 */ 790038,
/* 3262 */ 9999,
/* 3263 */ 9999,
/* 3264 */ 790076,
/* 3265 */ 9999,
/* 3266 */ 790003,
/* 3267 */ 9999,
/* 3268 */ 9999,
/* 3269 */ 9999,
/* 3270 */ 792029,
/* 3271 */ 792040,
/* 3272 */ 9999,
/* 3273 */ 9999,
/* 3274 */ 792078,
/* 3275 */ 9999,
/* 3276 */ 792005,
/* 3277 */ 9999,
/* 3278 */ 9999,
/* 3279 */ 9999,
/* 3280 */ 794029,
/* 3281 */ 794040,
/* 3282 */ 9999,
/* 3283 */ 9999,
/* 3284 */ 794078,
/* 3285 */ 9999,
/* 3286 */ 794005,
/* 3287 */ 9999,
/* 3288 */ 790039,
/* 3289 */ 9999,
/* 3290 */ 790048,
/* 3291 */ 9999,
/* 3292 */ 9999,
/* 3293 */ 790048,
/* 3294 */ 9999,
/* 3295 */ 792041,
/* 3296 */ 9999,
/* 3297 */ 792050,
/* 3298 */ 9999,
/* 3299 */ 9999,
/* 3300 */ 792050,
/* 3301 */ 9999,
/* 3302 */ 794041,
/* 3303 */ 9999,
/* 3304 */ 794050,
/* 3305 */ 9999,
/* 3306 */ 9999,
/* 3307 */ 794050,
/* 3308 */ 9999,
/* 3309 */ 9999,
/* 3310 */ 799007,
/* 3311 */ 9999,
/* 3312 */ 799003,
/* 3313 */ 9999,
/* 3314 */ 9999,
/* 3315 */ 800009,
/* 3316 */ 9999,
/* 3317 */ 800005,
/* 3318 */ 9999,
/* 3319 */ 9999,
/* 3320 */ 801009,
/* 3321 */ 9999,
/* 3322 */ 801024,
/* 3323 */ 9999,
/* 3324 */ 801005,
/* 3325 */ 9999,
/* 3326 */ 805003,
/* 3327 */ 805015,
/* 3328 */ 9999,
/* 3329 */ 805008,
/* 3330 */ 9999,
/* 3331 */ 9999,
/* 3332 */ 807011,
/* 3333 */ 9999,
/* 3334 */ 9999,
/* 3335 */ 807009,
/* 3336 */ 9999,
/* 3337 */ 9999,
/* 3338 */ 9999,
/* 3339 */ 9999,
/* 3340 */ 809009,
/* 3341 */ 9999,
/* 3342 */ 9999,
/* 3343 */ 9999,
/* 3344 */ 9999,
/* 3345 */ 9999,
/* 3346 */ 811034,
/* 3347 */ 811046,
/* 3348 */ 9999,
/* 3349 */ 811009,
/* 3350 */ 9999,
/* 3351 */ 9999,
/* 3352 */ 813011,
/* 3353 */ 9999,
/* 3354 */ 9999,
/* 3355 */ 9999,
/* 3356 */ 813046,
/* 3357 */ 813058,
/* 3358 */ 9999,
/* 3359 */ 813009,
/* 3360 */ 9999,
/* 3361 */ 9999,
/* 3362 */ 9999,
/* 3363 */ 805025,
/* 3364 */ 9999,
/* 3365 */ 9999,
/* 3366 */ 805025,
/* 3367 */ 9999,
/* 3368 */ 9999,
/* 3369 */ 9999,
/* 3370 */ 811056,
/* 3371 */ 9999,
/* 3372 */ 9999,
/* 3373 */ 811056,
/* 3374 */ 9999,
/* 3375 */ 9999,
/* 3376 */ 9999,
/* 3377 */ 813068,
/* 3378 */ 9999,
/* 3379 */ 9999,
/* 3380 */ 813068,
/* 3381 */ 9999,
/* 3382 */ 9999,
/* 3383 */ 9999,
/* 3384 */ 9999,
/* 3385 */ 819007,
/* 3386 */ 9999,
/* 3387 */ 9999,
/* 3388 */ 818009,
/* 3389 */ 9999,
/* 3390 */ 819008,
/* 3391 */ 819016,
/* 3392 */ 9999,
/* 3393 */ 819016,
/* 3394 */ 9999,
/* 3395 */ 9999,
/* 3396 */ 819016,
/* 3397 */ 9999,
/* 3398 */ 9999,
/* 3399 */ 823015,
/* 3400 */ 9999,
/* 3401 */ 9999,
/* 3402 */ 9999,
/* 3403 */ 823013,
/* 3404 */ 9999,
/* 3405 */ 824005,
/* 3406 */ 9999,
/* 3407 */ 824016,
/* 3408 */ 9999,
/* 3409 */ 825005,
/* 3410 */ 9999,
/* 3411 */ 825014,
/* 3412 */ 9999,
/* 3413 */ 826005,
/* 3414 */ 9999,
/* 3415 */ 826017,
/* 3416 */ 9999,
/* 3417 */ 9999,
/* 3418 */ 9999,
/* 3419 */ 823016,
/* 3420 */ 9999,
/* 3421 */ 9999,
/* 3422 */ 823016,
/* 3423 */ 9999,
/* 3424 */ 9999,
/* 3425 */ 830013,
/* 3426 */ 831005,
/* 3427 */ 9999,
/* 3428 */ 9999,
/* 3429 */ 830011,
/* 3430 */ 9999,
/* 3431 */ 9999,
/* 3432 */ 834011,
/* 3433 */ 835005,
/* 3434 */ 9999,
/* 3435 */ 834009,
/* 3436 */ 9999,
/* 3437 */ 9999,
/* 3438 */ 842014,
/* 3439 */ 843005,
/* 3440 */ 9999,
/* 3441 */ 9999,
/* 3442 */ 842012,
/* 3443 */ 9999,
/* 3444 */ 9999,
/* 3445 */ 9999,
/* 3446 */ 830014,
/* 3447 */ 9999,
/* 3448 */ 9999,
/* 3449 */ 830014,
/* 3450 */ 9999,
/* 3451 */ 9999,
/* 3452 */ 831011,
/* 3453 */ 831023,
/* 3454 */ 831071,
/* 3455 */ 831007,
/* 3456 */ 9999,
/* 3457 */ 831007,
/* 3458 */ 9999,
/* 3459 */ 9999,
/* 3460 */ 832011,
/* 3461 */ 832023,
/* 3462 */ 832070,
/* 3463 */ 832007,
/* 3464 */ 9999,
/* 3465 */ 832007,
/* 3466 */ 9999,
/* 3467 */ 9999,
/* 3468 */ 832007,
/* 3469 */ 9999,
/* 3470 */ 9999,
/* 3471 */ 9999,
/* 3472 */ 831072,
/* 3473 */ 9999,
/* 3474 */ 9999,
/* 3475 */ 831072,
/* 3476 */ 9999,
/* 3477 */ 9999,
/* 3478 */ 9999,
/* 3479 */ 832071,
/* 3480 */ 9999,
/* 3481 */ 9999,
/* 3482 */ 832071,
/* 3483 */ 9999,
/* 3484 */ 9999,
/* 3485 */ 9999,
/* 3486 */ 834012,
/* 3487 */ 9999,
/* 3488 */ 9999,
/* 3489 */ 834012,
/* 3490 */ 9999,
/* 3491 */ 9999,
/* 3492 */ 835011,
/* 3493 */ 835044,
/* 3494 */ 835058,
/* 3495 */ 835007,
/* 3496 */ 9999,
/* 3497 */ 835007,
/* 3498 */ 9999,
/* 3499 */ 9999,
/* 3500 */ 835007,
/* 3501 */ 9999,
/* 3502 */ 835045,
/* 3503 */ 835054,
/* 3504 */ 9999,
/* 3505 */ 835054,
/* 3506 */ 9999,
/* 3507 */ 9999,
/* 3508 */ 835054,
/* 3509 */ 9999,
/* 3510 */ 9999,
/* 3511 */ 9999,
/* 3512 */ 835059,
/* 3513 */ 9999,
/* 3514 */ 9999,
/* 3515 */ 9999,
/* 3516 */ 835063,
/* 3517 */ 9999,
/* 3518 */ 9999,
/* 3519 */ 9999,
/* 3520 */ 842015,
/* 3521 */ 9999,
/* 3522 */ 9999,
/* 3523 */ 842015,
/* 3524 */ 9999,
/* 3525 */ 9999,
/* 3526 */ 843035,
/* 3527 */ 844007,
/* 3528 */ 847011,
/* 3529 */ 843007,
/* 3530 */ 9999,
/* 3531 */ 843007,
/* 3532 */ 9999,
/* 3533 */ 9999,
/* 3534 */ 843007,
/* 3535 */ 9999,
/* 3536 */ 843037,
/* 3537 */ 9999,
/* 3538 */ 9999,
/* 3539 */ 843042,
/* 3540 */ 9999,
/* 3541 */ 9999,
/* 3542 */ 843042,
/* 3543 */ 9999,
/* 3544 */ 844011,
/* 3545 */ 9999,
/* 3546 */ 9999,
/* 3547 */ 844037,
/* 3548 */ 9999,
/* 3549 */ 844017,
/* 3550 */ 9999,
/* 3551 */ 845011,
/* 3552 */ 9999,
/* 3553 */ 845026,
/* 3554 */ 9999,
/* 3555 */ 9999,
/* 3556 */ 845017,
/* 3557 */ 9999,
/* 3558 */ 846011,
/* 3559 */ 9999,
/* 3560 */ 9999,
/* 3561 */ 846037,
/* 3562 */ 9999,
/* 3563 */ 846017,
/* 3564 */ 9999,
/* 3565 */ 9999,
/* 3566 */ 9999,
/* 3567 */ 847012,
/* 3568 */ 9999,
/* 3569 */ 9999,
/* 3570 */ 847012,
/* 3571 */ 9999,
/* 3572 */ 851005,
/* 3573 */ 9999,
/* 3574 */ 851011,
/* 3575 */ 9999,
/* 3576 */ 852005,
/* 3577 */ 9999,
/* 3578 */ 852010,
/* 3579 */ 9999,
/* 3580 */ 9999,
/* 3581 */ 9999,
/* 3582 */ 853011,
/* 3583 */ 9999,
/* 3584 */ 9999,
/* 3585 */ 9999,
/* 3586 */ 9999,
/* 3587 */ 858007,
/* 3588 */ 9999,
/* 3589 */ 9999,
/* 3590 */ 857010,
/* 3591 */ 9999,
/* 3592 */ 858008,
/* 3593 */ 858017,
/* 3594 */ 9999,
/* 3595 */ 858017,
/* 3596 */ 9999,
/* 3597 */ 9999,
/* 3598 */ 858017,
/* 3599 */ 9999,
/* 3600 */ 9999,
/* 3601 */ 862015,
/* 3602 */ 9999,
/* 3603 */ 9999,
/* 3604 */ 9999,
/* 3605 */ 862013,
/* 3606 */ 9999,
/* 3607 */ 9999,
/* 3608 */ 863010,
/* 3609 */ 863017,
/* 3610 */ 9999,
/* 3611 */ 9999,
/* 3612 */ 863008,
/* 3613 */ 9999,
/* 3614 */ 864005,
/* 3615 */ 9999,
/* 3616 */ 9999,
/* 3617 */ 864005,
/* 3618 */ 9999,
/* 3619 */ 9999,
/* 3620 */ 865013,
/* 3621 */ 865020,
/* 3622 */ 9999,
/* 3623 */ 9999,
/* 3624 */ 865011,
/* 3625 */ 9999,
/* 3626 */ 9999,
/* 3627 */ 866014,
/* 3628 */ 866021,
/* 3629 */ 9999,
/* 3630 */ 9999,
/* 3631 */ 866012,
/* 3632 */ 9999,
/* 3633 */ 9999,
/* 3634 */ 867015,
/* 3635 */ 867022,
/* 3636 */ 9999,
/* 3637 */ 9999,
/* 3638 */ 867013,
/* 3639 */ 9999,
/* 3640 */ 9999,
/* 3641 */ 9999,
/* 3642 */ 869049,
/* 3643 */ 9999,
/* 3644 */ 9999,
/* 3645 */ 869009,
/* 3646 */ 9999,
/* 3647 */ 9999,
/* 3648 */ 9999,
/* 3649 */ 870049,
/* 3650 */ 9999,
/* 3651 */ 9999,
/* 3652 */ 870009,
/* 3653 */ 9999,
/* 3654 */ 9999,
/* 3655 */ 9999,
/* 3656 */ 871049,
/* 3657 */ 9999,
/* 3658 */ 9999,
/* 3659 */ 871007,
/* 3660 */ 9999,
/* 3661 */ 9999,
/* 3662 */ 9999,
/* 3663 */ 872049,
/* 3664 */ 9999,
/* 3665 */ 9999,
/* 3666 */ 872010,
/* 3667 */ 9999,
/* 3668 */ 873005,
/* 3669 */ 9999,
/* 3670 */ 873014,
/* 3671 */ 9999,
/* 3672 */ 874005,
/* 3673 */ 9999,
/* 3674 */ 874017,
/* 3675 */ 9999,
/* 3676 */ 9999,
/* 3677 */ 9999,
/* 3678 */ 862016,
/* 3679 */ 9999,
/* 3680 */ 9999,
/* 3681 */ 862016,
/* 3682 */ 9999,
/* 3683 */ 9999,
/* 3684 */ 9999,
/* 3685 */ 863011,
/* 3686 */ 9999,
/* 3687 */ 9999,
/* 3688 */ 863011,
/* 3689 */ 9999,
/* 3690 */ 9999,
/* 3691 */ 864020,
/* 3692 */ 864027,
/* 3693 */ 864070,
/* 3694 */ 864014,
/* 3695 */ 9999,
/* 3696 */ 864014,
/* 3697 */ 9999,
/* 3698 */ 9999,
/* 3699 */ 864014,
/* 3700 */ 9999,
/* 3701 */ 9999,
/* 3702 */ 9999,
/* 3703 */ 864021,
/* 3704 */ 9999,
/* 3705 */ 9999,
/* 3706 */ 864021,
/* 3707 */ 9999,
/* 3708 */ 9999,
/* 3709 */ 9999,
/* 3710 */ 864071,
/* 3711 */ 9999,
/* 3712 */ 9999,
/* 3713 */ 864071,
/* 3714 */ 9999,
/* 3715 */ 9999,
/* 3716 */ 9999,
/* 3717 */ 865014,
/* 3718 */ 9999,
/* 3719 */ 9999,
/* 3720 */ 865014,
/* 3721 */ 9999,
/* 3722 */ 865021,
/* 3723 */ 865062,
/* 3724 */ 865026,
/* 3725 */ 9999,
/* 3726 */ 865026,
/* 3727 */ 9999,
/* 3728 */ 9999,
/* 3729 */ 865026,
/* 3730 */ 9999,
/* 3731 */ 9999,
/* 3732 */ 9999,
/* 3733 */ 865063,
/* 3734 */ 9999,
/* 3735 */ 9999,
/* 3736 */ 865063,
/* 3737 */ 9999,
/* 3738 */ 9999,
/* 3739 */ 9999,
/* 3740 */ 866015,
/* 3741 */ 9999,
/* 3742 */ 9999,
/* 3743 */ 866015,
/* 3744 */ 9999,
/* 3745 */ 9999,
/* 3746 */ 866074,
/* 3747 */ 866030,
/* 3748 */ 9999,
/* 3749 */ 866030,
/* 3750 */ 9999,
/* 3751 */ 9999,
/* 3752 */ 866030,
/* 3753 */ 9999,
/* 3754 */ 9999,
/* 3755 */ 9999,
/* 3756 */ 866075,
/* 3757 */ 9999,
/* 3758 */ 9999,
/* 3759 */ 866075,
/* 3760 */ 9999,
/* 3761 */ 9999,
/* 3762 */ 9999,
/* 3763 */ 867016,
/* 3764 */ 9999,
/* 3765 */ 9999,
/* 3766 */ 867016,
/* 3767 */ 9999,
/* 3768 */ 867023,
/* 3769 */ 867064,
/* 3770 */ 867028,
/* 3771 */ 9999,
/* 3772 */ 867028,
/* 3773 */ 9999,
/* 3774 */ 9999,
/* 3775 */ 867028,
/* 3776 */ 9999,
/* 3777 */ 9999,
/* 3778 */ 9999,
/* 3779 */ 867065,
/* 3780 */ 9999,
/* 3781 */ 9999,
/* 3782 */ 867065,
/* 3783 */ 9999,
/* 3784 */ 869050,
/* 3785 */ 869060,
/* 3786 */ 869058,
/* 3787 */ 9999,
/* 3788 */ 869058,
/* 3789 */ 9999,
/* 3790 */ 9999,
/* 3791 */ 869058,
/* 3792 */ 9999,
/* 3793 */ 9999,
/* 3794 */ 9999,
/* 3795 */ 869061,
/* 3796 */ 9999,
/* 3797 */ 870050,
/* 3798 */ 870060,
/* 3799 */ 870058,
/* 3800 */ 9999,
/* 3801 */ 870058,
/* 3802 */ 9999,
/* 3803 */ 9999,
/* 3804 */ 870058,
/* 3805 */ 9999,
/* 3806 */ 9999,
/* 3807 */ 9999,
/* 3808 */ 870061,
/* 3809 */ 9999,
/* 3810 */ 871050,
/* 3811 */ 871060,
/* 3812 */ 871058,
/* 3813 */ 9999,
/* 3814 */ 871058,
/* 3815 */ 9999,
/* 3816 */ 9999,
/* 3817 */ 871058,
/* 3818 */ 9999,
/* 3819 */ 9999,
/* 3820 */ 9999,
/* 3821 */ 871061,
/* 3822 */ 9999,
/* 3823 */ 872050,
/* 3824 */ 872060,
/* 3825 */ 872058,
/* 3826 */ 9999,
/* 3827 */ 872058,
/* 3828 */ 9999,
/* 3829 */ 9999,
/* 3830 */ 872058,
/* 3831 */ 9999,
/* 3832 */ 9999,
/* 3833 */ 9999,
/* 3834 */ 872061,
/* 3835 */ 9999,
/* 3836 */ 9999,
/* 3837 */ 9999,
/* 3838 */ 878015,
/* 3839 */ 9999,
/* 3840 */ 878030,
/* 3841 */ 9999,
/* 3842 */ 9999,
/* 3843 */ 878009,
/* 3844 */ 9999,
/* 3845 */ 9999,
/* 3846 */ 9999,
/* 3847 */ 880015,
/* 3848 */ 9999,
/* 3849 */ 9999,
/* 3850 */ 880009,
/* 3851 */ 9999,
/* 3852 */ 882005,
/* 3853 */ 882017,
/* 3854 */ 9999,
/* 3855 */ 882011,
/* 3856 */ 9999,
/* 3857 */ 884005,
/* 3858 */ 9999,
/* 3859 */ 884023,
/* 3860 */ 9999,
/* 3861 */ 9999,
/* 3862 */ 884011,
/* 3863 */ 9999,
/* 3864 */ 886005,
/* 3865 */ 886017,
/* 3866 */ 886027,
/* 3867 */ 9999,
/* 3868 */ 9999,
/* 3869 */ 886011,
/* 3870 */ 9999,
/* 3871 */ 889005,
/* 3872 */ 889017,
/* 3873 */ 889027,
/* 3874 */ 9999,
/* 3875 */ 9999,
/* 3876 */ 889011,
/* 3877 */ 9999,
/* 3878 */ 9999,
/* 3879 */ 892012,
/* 3880 */ 9999,
/* 3881 */ 892010,
/* 3882 */ 9999,
/* 3883 */ 9999,
/* 3884 */ 894010,
/* 3885 */ 9999,
/* 3886 */ 894008,
/* 3887 */ 9999,
/* 3888 */ 9999,
/* 3889 */ 9999,
/* 3890 */ 9999,
/* 3891 */ 882019,
/* 3892 */ 9999,
/* 3893 */ 9999,
/* 3894 */ 882019,
/* 3895 */ 9999,
/* 3896 */ 9999,
/* 3897 */ 9999,
/* 3898 */ 886028,
/* 3899 */ 9999,
/* 3900 */ 9999,
/* 3901 */ 9999,
/* 3902 */ 886034,
/* 3903 */ 9999,
/* 3904 */ 9999,
/* 3905 */ 9999,
/* 3906 */ 889028,
/* 3907 */ 9999,
/* 3908 */ 9999,
/* 3909 */ 9999,
/* 3910 */ 889034,
/* 3911 */ 9999,
/* 3912 */ 9999,
/* 3913 */ 9999,
/* 3914 */ 9999,
/* 3915 */ 898038,
/* 3916 */ 9999,
/* 3917 */ 900007,
/* 3918 */ 9999,
/* 3919 */ 9999,
/* 3920 */ 898018,
/* 3921 */ 9999,
/* 3922 */ 900008,
/* 3923 */ 900019,
/* 3924 */ 9999,
/* 3925 */ 900019,
/* 3926 */ 9999,
/* 3927 */ 9999,
/* 3928 */ 900019,
/* 3929 */ 9999,
/* 3930 */ 9999,
/* 3931 */ 906010,
/* 3932 */ 906017,
/* 3933 */ 9999,
/* 3934 */ 9999,
/* 3935 */ 905003,
/* 3936 */ 9999,
/* 3937 */ 9999,
/* 3938 */ 910012,
/* 3939 */ 910019,
/* 3940 */ 9999,
/* 3941 */ 9999,
/* 3942 */ 910010,
/* 3943 */ 9999,
/* 3944 */ 9999,
/* 3945 */ 912015,
/* 3946 */ 9999,
/* 3947 */ 9999,
/* 3948 */ 9999,
/* 3949 */ 912013,
/* 3950 */ 9999,
/* 3951 */ 913005,
/* 3952 */ 9999,
/* 3953 */ 913014,
/* 3954 */ 9999,
/* 3955 */ 914005,
/* 3956 */ 9999,
/* 3957 */ 914016,
/* 3958 */ 9999,
/* 3959 */ 915005,
/* 3960 */ 9999,
/* 3961 */ 915017,
/* 3962 */ 9999,
/* 3963 */ 9999,
/* 3964 */ 9999,
/* 3965 */ 906011,
/* 3966 */ 9999,
/* 3967 */ 9999,
/* 3968 */ 906011,
/* 3969 */ 9999,
/* 3970 */ 906018,
/* 3971 */ 906072,
/* 3972 */ 906024,
/* 3973 */ 9999,
/* 3974 */ 906024,
/* 3975 */ 9999,
/* 3976 */ 9999,
/* 3977 */ 906024,
/* 3978 */ 9999,
/* 3979 */ 9999,
/* 3980 */ 9999,
/* 3981 */ 906073,
/* 3982 */ 9999,
/* 3983 */ 9999,
/* 3984 */ 906073,
/* 3985 */ 9999,
/* 3986 */ 9999,
/* 3987 */ 9999,
/* 3988 */ 910013,
/* 3989 */ 9999,
/* 3990 */ 9999,
/* 3991 */ 910013,
/* 3992 */ 9999,
/* 3993 */ 910020,
/* 3994 */ 910054,
/* 3995 */ 910034,
/* 3996 */ 9999,
/* 3997 */ 910034,
/* 3998 */ 9999,
/* 3999 */ 9999,
/* 4000 */ 910034,
/* 4001 */ 9999,
/* 4002 */ 9999,
/* 4003 */ 9999,
/* 4004 */ 910055,
/* 4005 */ 9999,
/* 4006 */ 9999,
/* 4007 */ 910055,
/* 4008 */ 9999,
/* 4009 */ 9999,
/* 4010 */ 9999,
/* 4011 */ 912016,
/* 4012 */ 9999,
/* 4013 */ 9999,
/* 4014 */ 912016,
/* 4015 */ 9999,
/* 4016 */ 9999,
/* 4017 */ 9999,
/* 4018 */ 9999,
/* 4019 */ 919007,
/* 4020 */ 9999,
/* 4021 */ 9999,
/* 4022 */ 918012,
/* 4023 */ 9999,
/* 4024 */ 919008,
/* 4025 */ 919016,
/* 4026 */ 9999,
/* 4027 */ 919016,
/* 4028 */ 9999,
/* 4029 */ 9999,
/* 4030 */ 919016,
/* 4031 */ 9999,
/* 4032 */ 9999,
/* 4033 */ 925011,
/* 4034 */ 925018,
/* 4035 */ 9999,
/* 4036 */ 9999,
/* 4037 */ 924003,
/* 4038 */ 9999,
/* 4039 */ 9999,
/* 4040 */ 9999,
/* 4041 */ 9999,
/* 4042 */ 929013,
/* 4043 */ 9999,
/* 4044 */ 9999,
/* 4045 */ 930011,
/* 4046 */ 930018,
/* 4047 */ 9999,
/* 4048 */ 9999,
/* 4049 */ 930009,
/* 4050 */ 9999,
/* 4051 */ 9999,
/* 4052 */ 931015,
/* 4053 */ 9999,
/* 4054 */ 9999,
/* 4055 */ 9999,
/* 4056 */ 931013,
/* 4057 */ 9999,
/* 4058 */ 9999,
/* 4059 */ 932012,
/* 4060 */ 932019,
/* 4061 */ 9999,
/* 4062 */ 9999,
/* 4063 */ 932010,
/* 4064 */ 9999,
/* 4065 */ 934005,
/* 4066 */ 9999,
/* 4067 */ 934014,
/* 4068 */ 9999,
/* 4069 */ 935005,
/* 4070 */ 9999,
/* 4071 */ 935016,
/* 4072 */ 9999,
/* 4073 */ 936005,
/* 4074 */ 9999,
/* 4075 */ 936017,
/* 4076 */ 9999,
/* 4077 */ 9999,
/* 4078 */ 9999,
/* 4079 */ 925012,
/* 4080 */ 9999,
/* 4081 */ 9999,
/* 4082 */ 925012,
/* 4083 */ 9999,
/* 4084 */ 925019,
/* 4085 */ 925074,
/* 4086 */ 925025,
/* 4087 */ 9999,
/* 4088 */ 925025,
/* 4089 */ 9999,
/* 4090 */ 9999,
/* 4091 */ 925025,
/* 4092 */ 9999,
/* 4093 */ 9999,
/* 4094 */ 9999,
/* 4095 */ 925075,
/* 4096 */ 9999,
/* 4097 */ 9999,
/* 4098 */ 925075,
/* 4099 */ 9999,
/* 4100 */ 9999,
/* 4101 */ 9999,
/* 4102 */ 930012,
/* 4103 */ 9999,
/* 4104 */ 9999,
/* 4105 */ 930012,
/* 4106 */ 9999,
/* 4107 */ 930019,
/* 4108 */ 930029,
/* 4109 */ 930024,
/* 4110 */ 9999,
/* 4111 */ 930024,
/* 4112 */ 9999,
/* 4113 */ 9999,
/* 4114 */ 930024,
/* 4115 */ 9999,
/* 4116 */ 9999,
/* 4117 */ 9999,
/* 4118 */ 930030,
/* 4119 */ 9999,
/* 4120 */ 9999,
/* 4121 */ 9999,
/* 4122 */ 930036,
/* 4123 */ 9999,
/* 4124 */ 9999,
/* 4125 */ 930036,
/* 4126 */ 9999,
/* 4127 */ 9999,
/* 4128 */ 9999,
/* 4129 */ 931016,
/* 4130 */ 9999,
/* 4131 */ 9999,
/* 4132 */ 931016,
/* 4133 */ 9999,
/* 4134 */ 9999,
/* 4135 */ 9999,
/* 4136 */ 932013,
/* 4137 */ 9999,
/* 4138 */ 9999,
/* 4139 */ 932013,
/* 4140 */ 9999,
/* 4141 */ 932020,
/* 4142 */ 932054,
/* 4143 */ 932034,
/* 4144 */ 9999,
/* 4145 */ 932034,
/* 4146 */ 9999,
/* 4147 */ 9999,
/* 4148 */ 932034,
/* 4149 */ 9999,
/* 4150 */ 9999,
/* 4151 */ 9999,
/* 4152 */ 932055,
/* 4153 */ 9999,
/* 4154 */ 9999,
/* 4155 */ 932055,
/* 4156 */ 9999,
/* 4157 */ 9999,
/* 4158 */ 9999,
/* 4159 */ 9999,
/* 4160 */ 941007,
/* 4161 */ 9999,
/* 4162 */ 9999,
/* 4163 */ 939020,
/* 4164 */ 9999,
/* 4165 */ 941008,
/* 4166 */ 941020,
/* 4167 */ 9999,
/* 4168 */ 941020,
/* 4169 */ 9999,
/* 4170 */ 9999,
/* 4171 */ 941020,
/* 4172 */ 9999,
/* 4173 */ 9999,
/* 4174 */ 946015,
/* 4175 */ 9999,
/* 4176 */ 9999,
/* 4177 */ 9999,
/* 4178 */ 946013,
/* 4179 */ 9999,
/* 4180 */ 947005,
/* 4181 */ 9999,
/* 4182 */ 947015,
/* 4183 */ 9999,
/* 4184 */ 9999,
/* 4185 */ 948010,
/* 4186 */ 948017,
/* 4187 */ 9999,
/* 4188 */ 9999,
/* 4189 */ 9999,
/* 4190 */ 948008,
/* 4191 */ 9999,
/* 4192 */ 9999,
/* 4193 */ 949013,
/* 4194 */ 949020,
/* 4195 */ 9999,
/* 4196 */ 9999,
/* 4197 */ 949011,
/* 4198 */ 9999,
/* 4199 */ 9999,
/* 4200 */ 950014,
/* 4201 */ 950021,
/* 4202 */ 9999,
/* 4203 */ 9999,
/* 4204 */ 950012,
/* 4205 */ 9999,
/* 4206 */ 9999,
/* 4207 */ 951011,
/* 4208 */ 951019,
/* 4209 */ 9999,
/* 4210 */ 9999,
/* 4211 */ 951009,
/* 4212 */ 9999,
/* 4213 */ 952005,
/* 4214 */ 9999,
/* 4215 */ 952016,
/* 4216 */ 9999,
/* 4217 */ 953005,
/* 4218 */ 9999,
/* 4219 */ 953014,
/* 4220 */ 9999,
/* 4221 */ 954005,
/* 4222 */ 9999,
/* 4223 */ 954017,
/* 4224 */ 9999,
/* 4225 */ 9999,
/* 4226 */ 9999,
/* 4227 */ 946016,
/* 4228 */ 9999,
/* 4229 */ 9999,
/* 4230 */ 946016,
/* 4231 */ 9999,
/* 4232 */ 9999,
/* 4233 */ 9999,
/* 4234 */ 948011,
/* 4235 */ 9999,
/* 4236 */ 9999,
/* 4237 */ 948011,
/* 4238 */ 9999,
/* 4239 */ 9999,
/* 4240 */ 9999,
/* 4241 */ 949014,
/* 4242 */ 9999,
/* 4243 */ 9999,
/* 4244 */ 949014,
/* 4245 */ 9999,
/* 4246 */ 9999,
/* 4247 */ 9999,
/* 4248 */ 950015,
/* 4249 */ 9999,
/* 4250 */ 9999,
/* 4251 */ 950015,
/* 4252 */ 9999,
/* 4253 */ 9999,
/* 4254 */ 9999,
/* 4255 */ 951012,
/* 4256 */ 9999,
/* 4257 */ 9999,
/* 4258 */ 951012,
/* 4259 */ 9999,
/* 4260 */ 951020,
/* 4261 */ 951030,
/* 4262 */ 951025,
/* 4263 */ 9999,
/* 4264 */ 951025,
/* 4265 */ 9999,
/* 4266 */ 9999,
/* 4267 */ 951025,
/* 4268 */ 9999,
/* 4269 */ 9999,
/* 4270 */ 9999,
/* 4271 */ 951031,
/* 4272 */ 9999,
/* 4273 */ 9999,
/* 4274 */ 9999,
/* 4275 */ 951037,
/* 4276 */ 9999,
/* 4277 */ 9999,
/* 4278 */ 951037,
/* 4279 */ 9999,
/* 4280 */ 9999,
/* 4281 */ 9999,
/* 4282 */ 9999,
/* 4283 */ 957038,
/* 4284 */ 9999,
/* 4285 */ 960007,
/* 4286 */ 9999,
/* 4287 */ 9999,
/* 4288 */ 957018,
/* 4289 */ 9999,
/* 4290 */ 960008,
/* 4291 */ 960019,
/* 4292 */ 9999,
/* 4293 */ 960019,
/* 4294 */ 9999,
/* 4295 */ 9999,
/* 4296 */ 960019,
/* 4297 */ 9999,
/* 4298 */ 9999,
/* 4299 */ 965015,
/* 4300 */ 9999,
/* 4301 */ 9999,
/* 4302 */ 9999,
/* 4303 */ 965013,
/* 4304 */ 9999,
/* 4305 */ 9999,
/* 4306 */ 966011,
/* 4307 */ 966018,
/* 4308 */ 9999,
/* 4309 */ 9999,
/* 4310 */ 966009,
/* 4311 */ 9999,
/* 4312 */ 9999,
/* 4313 */ 967011,
/* 4314 */ 967018,
/* 4315 */ 9999,
/* 4316 */ 9999,
/* 4317 */ 967009,
/* 4318 */ 9999,
/* 4319 */ 9999,
/* 4320 */ 968011,
/* 4321 */ 968018,
/* 4322 */ 9999,
/* 4323 */ 9999,
/* 4324 */ 968008,
/* 4325 */ 9999,
/* 4326 */ 9999,
/* 4327 */ 969011,
/* 4328 */ 969018,
/* 4329 */ 9999,
/* 4330 */ 9999,
/* 4331 */ 969009,
/* 4332 */ 9999,
/* 4333 */ 9999,
/* 4334 */ 970011,
/* 4335 */ 970018,
/* 4336 */ 9999,
/* 4337 */ 9999,
/* 4338 */ 970008,
/* 4339 */ 9999,
/* 4340 */ 9999,
/* 4341 */ 971015,
/* 4342 */ 971022,
/* 4343 */ 9999,
/* 4344 */ 9999,
/* 4345 */ 971013,
/* 4346 */ 9999,
/* 4347 */ 9999,
/* 4348 */ 973011,
/* 4349 */ 973019,
/* 4350 */ 9999,
/* 4351 */ 9999,
/* 4352 */ 973009,
/* 4353 */ 9999,
/* 4354 */ 9999,
/* 4355 */ 9999,
/* 4356 */ 9999,
/* 4357 */ 9999,
/* 4358 */ 9999,
/* 4359 */ 974008,
/* 4360 */ 9999,
/* 4361 */ 9999,
/* 4362 */ 9999,
/* 4363 */ 977015,
/* 4364 */ 9999,
/* 4365 */ 9999,
/* 4366 */ 977009,
/* 4367 */ 9999,
/* 4368 */ 9999,
/* 4369 */ 9999,
/* 4370 */ 993017,
/* 4371 */ 9999,
/* 4372 */ 993011,
/* 4373 */ 9999,
/* 4374 */ 9999,
/* 4375 */ 9999,
/* 4376 */ 994017,
/* 4377 */ 9999,
/* 4378 */ 994011,
/* 4379 */ 9999,
/* 4380 */ 995005,
/* 4381 */ 9999,
/* 4382 */ 995016,
/* 4383 */ 9999,
/* 4384 */ 996005,
/* 4385 */ 9999,
/* 4386 */ 996014,
/* 4387 */ 9999,
/* 4388 */ 997005,
/* 4389 */ 9999,
/* 4390 */ 997017,
/* 4391 */ 9999,
/* 4392 */ 9999,
/* 4393 */ 9999,
/* 4394 */ 965016,
/* 4395 */ 9999,
/* 4396 */ 9999,
/* 4397 */ 965016,
/* 4398 */ 9999,
/* 4399 */ 9999,
/* 4400 */ 9999,
/* 4401 */ 966012,
/* 4402 */ 9999,
/* 4403 */ 9999,
/* 4404 */ 966012,
/* 4405 */ 9999,
/* 4406 */ 9999,
/* 4407 */ 9999,
/* 4408 */ 967012,
/* 4409 */ 9999,
/* 4410 */ 9999,
/* 4411 */ 967012,
/* 4412 */ 9999,
/* 4413 */ 9999,
/* 4414 */ 9999,
/* 4415 */ 968012,
/* 4416 */ 9999,
/* 4417 */ 9999,
/* 4418 */ 968012,
/* 4419 */ 9999,
/* 4420 */ 9999,
/* 4421 */ 9999,
/* 4422 */ 969012,
/* 4423 */ 9999,
/* 4424 */ 9999,
/* 4425 */ 969012,
/* 4426 */ 9999,
/* 4427 */ 9999,
/* 4428 */ 9999,
/* 4429 */ 970012,
/* 4430 */ 9999,
/* 4431 */ 9999,
/* 4432 */ 970012,
/* 4433 */ 9999,
/* 4434 */ 9999,
/* 4435 */ 9999,
/* 4436 */ 971016,
/* 4437 */ 9999,
/* 4438 */ 9999,
/* 4439 */ 971016,
/* 4440 */ 9999,
/* 4441 */ 971023,
/* 4442 */ 972006,
/* 4443 */ 971028,
/* 4444 */ 9999,
/* 4445 */ 971028,
/* 4446 */ 9999,
/* 4447 */ 9999,
/* 4448 */ 971028,
/* 4449 */ 9999,
/* 4450 */ 9999,
/* 4451 */ 9999,
/* 4452 */ 972007,
/* 4453 */ 9999,
/* 4454 */ 9999,
/* 4455 */ 9999,
/* 4456 */ 972013,
/* 4457 */ 9999,
/* 4458 */ 9999,
/* 4459 */ 972013,
/* 4460 */ 9999,
/* 4461 */ 9999,
/* 4462 */ 9999,
/* 4463 */ 973012,
/* 4464 */ 9999,
/* 4465 */ 9999,
/* 4466 */ 973012,
/* 4467 */ 9999,
/* 4468 */ 973020,
/* 4469 */ 973030,
/* 4470 */ 973025,
/* 4471 */ 9999,
/* 4472 */ 973025,
/* 4473 */ 9999,
/* 4474 */ 9999,
/* 4475 */ 973025,
/* 4476 */ 9999,
/* 4477 */ 9999,
/* 4478 */ 9999,
/* 4479 */ 973031,
/* 4480 */ 9999,
/* 4481 */ 9999,
/* 4482 */ 9999,
/* 4483 */ 973037,
/* 4484 */ 9999,
/* 4485 */ 9999,
/* 4486 */ 973037,
/* 4487 */ 9999,
/* 4488 */ 9999,
/* 4489 */ 9999,
/* 4490 */ 979025,
/* 4491 */ 980006,
/* 4492 */ 992003,
/* 4493 */ 977035,
/* 4494 */ 9999,
/* 4495 */ 977035,
/* 4496 */ 9999,
/* 4497 */ 9999,
/* 4498 */ 977035,
/* 4499 */ 9999,
/* 4500 */ 9999,
/* 4501 */ 980014,
/* 4502 */ 9999,
/* 4503 */ 980010,
/* 4504 */ 9999,
/* 4505 */ 980010,
/* 4506 */ 9999,
/* 4507 */ 9999,
/* 4508 */ 981015,
/* 4509 */ 986010,
/* 4510 */ 981013,
/* 4511 */ 9999,
/* 4512 */ 981013,
/* 4513 */ 9999,
/* 4514 */ 9999,
/* 4515 */ 990013,
/* 4516 */ 990011,
/* 4517 */ 9999,
/* 4518 */ 990011,
/* 4519 */ 9999,
/* 4520 */ 9999,
/* 4521 */ 990011,
/* 4522 */ 9999,
/* 4523 */ 981017,
/* 4524 */ 9999,
/* 4525 */ 981023,
/* 4526 */ 9999,
/* 4527 */ 9999,
/* 4528 */ 9999,
/* 4529 */ 982031,
/* 4530 */ 9999,
/* 4531 */ 982024,
/* 4532 */ 9999,
/* 4533 */ 9999,
/* 4534 */ 9999,
/* 4535 */ 984034,
/* 4536 */ 9999,
/* 4537 */ 984024,
/* 4538 */ 9999,
/* 4539 */ 982032,
/* 4540 */ 9999,
/* 4541 */ 982036,
/* 4542 */ 9999,
/* 4543 */ 9999,
/* 4544 */ 982036,
/* 4545 */ 9999,
/* 4546 */ 984035,
/* 4547 */ 9999,
/* 4548 */ 984039,
/* 4549 */ 9999,
/* 4550 */ 9999,
/* 4551 */ 984039,
/* 4552 */ 9999,
/* 4553 */ 9999,
/* 4554 */ 986014,
/* 4555 */ 9999,
/* 4556 */ 986012,
/* 4557 */ 9999,
/* 4558 */ 9999,
/* 4559 */ 9999,
/* 4560 */ 987023,
/* 4561 */ 9999,
/* 4562 */ 987035,
/* 4563 */ 9999,
/* 4564 */ 987014,
/* 4565 */ 9999,
/* 4566 */ 9999,
/* 4567 */ 989016,
/* 4568 */ 9999,
/* 4569 */ 989014,
/* 4570 */ 9999,
/* 4571 */ 9999,
/* 4572 */ 989014,
/* 4573 */ 9999,
/* 4574 */ 9999,
/* 4575 */ 9999,
/* 4576 */ 992004,
/* 4577 */ 9999,
/* 4578 */ 9999,
/* 4579 */ 992004,
/* 4580 */ 9999,
/* 4581 */ 993018,
/* 4582 */ 993029,
/* 4583 */ 9999,
/* 4584 */ 993029,
/* 4585 */ 9999,
/* 4586 */ 9999,
/* 4587 */ 993029,
/* 4588 */ 9999,
/* 4589 */ 994018,
/* 4590 */ 994029,
/* 4591 */ 9999,
/* 4592 */ 994029,
/* 4593 */ 9999,
/* 4594 */ 9999,
/* 4595 */ 994029,
/* 4596 */ 9999,
/* 4597 */ 9999,
/* 4598 */ 9999,
/* 4599 */ 9999,
/* 4600 */ 1002009,
/* 4601 */ 1003005,
/* 4602 */ 9999,
/* 4603 */ 1004007,
/* 4604 */ 9999,
/* 4605 */ 9999,
/* 4606 */ 1000027,
/* 4607 */ 9999,
/* 4608 */ 9999,
/* 4609 */ 1003010,
/* 4610 */ 9999,
/* 4611 */ 1003008,
/* 4612 */ 9999,
/* 4613 */ 9999,
/* 4614 */ 1003008,
/* 4615 */ 9999,
/* 4616 */ 1004008,
/* 4617 */ 1004021,
/* 4618 */ 9999,
/* 4619 */ 1004021,
/* 4620 */ 9999,
/* 4621 */ 9999,
/* 4622 */ 1004021,
/* 4623 */ 9999,
/* 4624 */ 9999,
/* 4625 */ 1009015,
/* 4626 */ 9999,
/* 4627 */ 9999,
/* 4628 */ 9999,
/* 4629 */ 1009013,
/* 4630 */ 9999,
/* 4631 */ 9999,
/* 4632 */ 1010009,
/* 4633 */ 1010016,
/* 4634 */ 9999,
/* 4635 */ 9999,
/* 4636 */ 1010007,
/* 4637 */ 9999,
/* 4638 */ 9999,
/* 4639 */ 1011011,
/* 4640 */ 1011018,
/* 4641 */ 9999,
/* 4642 */ 9999,
/* 4643 */ 1011009,
/* 4644 */ 9999,
/* 4645 */ 1012005,
/* 4646 */ 9999,
/* 4647 */ 1012015,
/* 4648 */ 9999,
/* 4649 */ 1013005,
/* 4650 */ 1013028,
/* 4651 */ 1013035,
/* 4652 */ 9999,
/* 4653 */ 9999,
/* 4654 */ 1013005,
/* 4655 */ 9999,
/* 4656 */ 9999,
/* 4657 */ 1014013,
/* 4658 */ 1014020,
/* 4659 */ 9999,
/* 4660 */ 9999,
/* 4661 */ 1014011,
/* 4662 */ 9999,
/* 4663 */ 9999,
/* 4664 */ 1015013,
/* 4665 */ 1015020,
/* 4666 */ 9999,
/* 4667 */ 9999,
/* 4668 */ 1015007,
/* 4669 */ 9999,
/* 4670 */ 9999,
/* 4671 */ 1016013,
/* 4672 */ 1016020,
/* 4673 */ 9999,
/* 4674 */ 9999,
/* 4675 */ 1016007,
/* 4676 */ 9999,
/* 4677 */ 9999,
/* 4678 */ 1017013,
/* 4679 */ 1017020,
/* 4680 */ 9999,
/* 4681 */ 9999,
/* 4682 */ 1017007,
/* 4683 */ 9999,
/* 4684 */ 9999,
/* 4685 */ 1018013,
/* 4686 */ 1018020,
/* 4687 */ 9999,
/* 4688 */ 9999,
/* 4689 */ 1018007,
/* 4690 */ 9999,
/* 4691 */ 9999,
/* 4692 */ 1019013,
/* 4693 */ 1019020,
/* 4694 */ 9999,
/* 4695 */ 9999,
/* 4696 */ 1019009,
/* 4697 */ 9999,
/* 4698 */ 9999,
/* 4699 */ 9999,
/* 4700 */ 1020024,
/* 4701 */ 1020031,
/* 4702 */ 9999,
/* 4703 */ 9999,
/* 4704 */ 1020010,
/* 4705 */ 9999,
/* 4706 */ 9999,
/* 4707 */ 1021013,
/* 4708 */ 1021020,
/* 4709 */ 9999,
/* 4710 */ 9999,
/* 4711 */ 1021008,
/* 4712 */ 9999,
/* 4713 */ 9999,
/* 4714 */ 1022013,
/* 4715 */ 1022020,
/* 4716 */ 9999,
/* 4717 */ 9999,
/* 4718 */ 1022009,
/* 4719 */ 9999,
/* 4720 */ 9999,
/* 4721 */ 1023013,
/* 4722 */ 1023020,
/* 4723 */ 9999,
/* 4724 */ 9999,
/* 4725 */ 1023008,
/* 4726 */ 9999,
/* 4727 */ 9999,
/* 4728 */ 1024019,
/* 4729 */ 1024026,
/* 4730 */ 1024035,
/* 4731 */ 9999,
/* 4732 */ 1024013,
/* 4733 */ 9999,
/* 4734 */ 9999,
/* 4735 */ 1025012,
/* 4736 */ 1025019,
/* 4737 */ 1025037,
/* 4738 */ 9999,
/* 4739 */ 9999,
/* 4740 */ 1025010,
/* 4741 */ 9999,
/* 4742 */ 9999,
/* 4743 */ 9999,
/* 4744 */ 1027015,
/* 4745 */ 9999,
/* 4746 */ 9999,
/* 4747 */ 9999,
/* 4748 */ 1027009,
/* 4749 */ 9999,
/* 4750 */ 9999,
/* 4751 */ 9999,
/* 4752 */ 1029015,
/* 4753 */ 9999,
/* 4754 */ 1029030,
/* 4755 */ 9999,
/* 4756 */ 9999,
/* 4757 */ 9999,
/* 4758 */ 1029009,
/* 4759 */ 9999,
/* 4760 */ 9999,
/* 4761 */ 9999,
/* 4762 */ 1031015,
/* 4763 */ 9999,
/* 4764 */ 1031030,
/* 4765 */ 9999,
/* 4766 */ 1031046,
/* 4767 */ 9999,
/* 4768 */ 9999,
/* 4769 */ 9999,
/* 4770 */ 1031009,
/* 4771 */ 9999,
/* 4772 */ 9999,
/* 4773 */ 9999,
/* 4774 */ 1034015,
/* 4775 */ 9999,
/* 4776 */ 1034030,
/* 4777 */ 9999,
/* 4778 */ 1034046,
/* 4779 */ 9999,
/* 4780 */ 1034062,
/* 4781 */ 9999,
/* 4782 */ 9999,
/* 4783 */ 9999,
/* 4784 */ 1034009,
/* 4785 */ 9999,
/* 4786 */ 9999,
/* 4787 */ 1038013,
/* 4788 */ 1038020,
/* 4789 */ 9999,
/* 4790 */ 9999,
/* 4791 */ 1038011,
/* 4792 */ 9999,
/* 4793 */ 9999,
/* 4794 */ 1041013,
/* 4795 */ 1041020,
/* 4796 */ 9999,
/* 4797 */ 9999,
/* 4798 */ 1041010,
/* 4799 */ 9999,
/* 4800 */ 9999,
/* 4801 */ 1044011,
/* 4802 */ 1044018,
/* 4803 */ 1045019,
/* 4804 */ 9999,
/* 4805 */ 9999,
/* 4806 */ 1044009,
/* 4807 */ 9999,
/* 4808 */ 9999,
/* 4809 */ 1046012,
/* 4810 */ 1046019,
/* 4811 */ 9999,
/* 4812 */ 9999,
/* 4813 */ 1046010,
/* 4814 */ 9999,
/* 4815 */ 9999,
/* 4816 */ 1049013,
/* 4817 */ 1049020,
/* 4818 */ 9999,
/* 4819 */ 9999,
/* 4820 */ 1049011,
/* 4821 */ 9999,
/* 4822 */ 9999,
/* 4823 */ 1051012,
/* 4824 */ 1051019,
/* 4825 */ 9999,
/* 4826 */ 9999,
/* 4827 */ 1051010,
/* 4828 */ 9999,
/* 4829 */ 9999,
/* 4830 */ 1053011,
/* 4831 */ 1053018,
/* 4832 */ 9999,
/* 4833 */ 1053009,
/* 4834 */ 9999,
/* 4835 */ 1061005,
/* 4836 */ 9999,
/* 4837 */ 1061013,
/* 4838 */ 9999,
/* 4839 */ 1062005,
/* 4840 */ 9999,
/* 4841 */ 1062014,
/* 4842 */ 9999,
/* 4843 */ 1063005,
/* 4844 */ 9999,
/* 4845 */ 1063016,
/* 4846 */ 9999,
/* 4847 */ 1064005,
/* 4848 */ 9999,
/* 4849 */ 1064017,
/* 4850 */ 9999,
/* 4851 */ 9999,
/* 4852 */ 9999,
/* 4853 */ 1009016,
/* 4854 */ 9999,
/* 4855 */ 9999,
/* 4856 */ 1009016,
/* 4857 */ 9999,
/* 4858 */ 9999,
/* 4859 */ 9999,
/* 4860 */ 1010010,
/* 4861 */ 9999,
/* 4862 */ 9999,
/* 4863 */ 1010010,
/* 4864 */ 9999,
/* 4865 */ 9999,
/* 4866 */ 9999,
/* 4867 */ 1011012,
/* 4868 */ 9999,
/* 4869 */ 9999,
/* 4870 */ 1011012,
/* 4871 */ 9999,
/* 4872 */ 9999,
/* 4873 */ 9999,
/* 4874 */ 1013015,
/* 4875 */ 9999,
/* 4876 */ 9999,
/* 4877 */ 9999,
/* 4878 */ 1013025,
/* 4879 */ 9999,
/* 4880 */ 9999,
/* 4881 */ 9999,
/* 4882 */ 1013029,
/* 4883 */ 9999,
/* 4884 */ 9999,
/* 4885 */ 1013029,
/* 4886 */ 9999,
/* 4887 */ 9999,
/* 4888 */ 9999,
/* 4889 */ 1014014,
/* 4890 */ 9999,
/* 4891 */ 9999,
/* 4892 */ 1014014,
/* 4893 */ 9999,
/* 4894 */ 9999,
/* 4895 */ 9999,
/* 4896 */ 1015014,
/* 4897 */ 9999,
/* 4898 */ 9999,
/* 4899 */ 1015014,
/* 4900 */ 9999,
/* 4901 */ 9999,
/* 4902 */ 9999,
/* 4903 */ 1016014,
/* 4904 */ 9999,
/* 4905 */ 9999,
/* 4906 */ 1016014,
/* 4907 */ 9999,
/* 4908 */ 9999,
/* 4909 */ 9999,
/* 4910 */ 1017014,
/* 4911 */ 9999,
/* 4912 */ 9999,
/* 4913 */ 1017014,
/* 4914 */ 9999,
/* 4915 */ 9999,
/* 4916 */ 9999,
/* 4917 */ 1018014,
/* 4918 */ 9999,
/* 4919 */ 9999,
/* 4920 */ 1018014,
/* 4921 */ 9999,
/* 4922 */ 9999,
/* 4923 */ 9999,
/* 4924 */ 1019014,
/* 4925 */ 9999,
/* 4926 */ 9999,
/* 4927 */ 1019014,
/* 4928 */ 9999,
/* 4929 */ 9999,
/* 4930 */ 9999,
/* 4931 */ 1020025,
/* 4932 */ 9999,
/* 4933 */ 9999,
/* 4934 */ 1020025,
/* 4935 */ 9999,
/* 4936 */ 9999,
/* 4937 */ 9999,
/* 4938 */ 1021014,
/* 4939 */ 9999,
/* 4940 */ 9999,
/* 4941 */ 1021014,
/* 4942 */ 9999,
/* 4943 */ 9999,
/* 4944 */ 9999,
/* 4945 */ 1022014,
/* 4946 */ 9999,
/* 4947 */ 9999,
/* 4948 */ 1022014,
/* 4949 */ 9999,
/* 4950 */ 9999,
/* 4951 */ 9999,
/* 4952 */ 1023014,
/* 4953 */ 9999,
/* 4954 */ 9999,
/* 4955 */ 1023014,
/* 4956 */ 9999,
/* 4957 */ 9999,
/* 4958 */ 9999,
/* 4959 */ 1024020,
/* 4960 */ 9999,
/* 4961 */ 9999,
/* 4962 */ 1024020,
/* 4963 */ 9999,
/* 4964 */ 9999,
/* 4965 */ 9999,
/* 4966 */ 1024036,
/* 4967 */ 9999,
/* 4968 */ 9999,
/* 4969 */ 9999,
/* 4970 */ 1024040,
/* 4971 */ 9999,
/* 4972 */ 9999,
/* 4973 */ 9999,
/* 4974 */ 1025013,
/* 4975 */ 9999,
/* 4976 */ 9999,
/* 4977 */ 1025013,
/* 4978 */ 9999,
/* 4979 */ 9999,
/* 4980 */ 1025042,
/* 4981 */ 1025038,
/* 4982 */ 9999,
/* 4983 */ 1025038,
/* 4984 */ 9999,
/* 4985 */ 9999,
/* 4986 */ 1025038,
/* 4987 */ 9999,
/* 4988 */ 9999,
/* 4989 */ 9999,
/* 4990 */ 1038014,
/* 4991 */ 9999,
/* 4992 */ 9999,
/* 4993 */ 1038014,
/* 4994 */ 9999,
/* 4995 */ 1038021,
/* 4996 */ 1040007,
/* 4997 */ 1038021,
/* 4998 */ 9999,
/* 4999 */ 1038021,
/* 5000 */ 9999,
/* 5001 */ 9999,
/* 5002 */ 1038021,
/* 5003 */ 9999,
/* 5004 */ 1038023,
/* 5005 */ 9999,
/* 5006 */ 1038028,
/* 5007 */ 9999,
/* 5008 */ 1039015,
/* 5009 */ 1039025,
/* 5010 */ 9999,
/* 5011 */ 1039015,
/* 5012 */ 9999,
/* 5013 */ 9999,
/* 5014 */ 9999,
/* 5015 */ 1039016,
/* 5016 */ 9999,
/* 5017 */ 9999,
/* 5018 */ 9999,
/* 5019 */ 1039020,
/* 5020 */ 9999,
/* 5021 */ 9999,
/* 5022 */ 9999,
/* 5023 */ 1040008,
/* 5024 */ 9999,
/* 5025 */ 9999,
/* 5026 */ 1040008,
/* 5027 */ 9999,
/* 5028 */ 9999,
/* 5029 */ 9999,
/* 5030 */ 1041014,
/* 5031 */ 9999,
/* 5032 */ 9999,
/* 5033 */ 1041014,
/* 5034 */ 9999,
/* 5035 */ 1041021,
/* 5036 */ 1043007,
/* 5037 */ 1041021,
/* 5038 */ 9999,
/* 5039 */ 1041021,
/* 5040 */ 9999,
/* 5041 */ 9999,
/* 5042 */ 1041021,
/* 5043 */ 9999,
/* 5044 */ 1041023,
/* 5045 */ 9999,
/* 5046 */ 1041028,
/* 5047 */ 9999,
/* 5048 */ 1042015,
/* 5049 */ 1042025,
/* 5050 */ 9999,
/* 5051 */ 1042015,
/* 5052 */ 9999,
/* 5053 */ 9999,
/* 5054 */ 9999,
/* 5055 */ 1042016,
/* 5056 */ 9999,
/* 5057 */ 9999,
/* 5058 */ 9999,
/* 5059 */ 1042020,
/* 5060 */ 9999,
/* 5061 */ 9999,
/* 5062 */ 9999,
/* 5063 */ 1043008,
/* 5064 */ 9999,
/* 5065 */ 9999,
/* 5066 */ 1043008,
/* 5067 */ 9999,
/* 5068 */ 9999,
/* 5069 */ 9999,
/* 5070 */ 1044012,
/* 5071 */ 9999,
/* 5072 */ 9999,
/* 5073 */ 1044012,
/* 5074 */ 9999,
/* 5075 */ 1044019,
/* 5076 */ 9999,
/* 5077 */ 1044024,
/* 5078 */ 9999,
/* 5079 */ 1044024,
/* 5080 */ 9999,
/* 5081 */ 9999,
/* 5082 */ 1044024,
/* 5083 */ 9999,
/* 5084 */ 9999,
/* 5085 */ 9999,
/* 5086 */ 1046013,
/* 5087 */ 9999,
/* 5088 */ 9999,
/* 5089 */ 1046013,
/* 5090 */ 9999,
/* 5091 */ 1046021,
/* 5092 */ 1048006,
/* 5093 */ 1046027,
/* 5094 */ 9999,
/* 5095 */ 1046027,
/* 5096 */ 9999,
/* 5097 */ 9999,
/* 5098 */ 1046027,
/* 5099 */ 9999,
/* 5100 */ 9999,
/* 5101 */ 9999,
/* 5102 */ 1048007,
/* 5103 */ 9999,
/* 5104 */ 9999,
/* 5105 */ 1048007,
/* 5106 */ 9999,
/* 5107 */ 9999,
/* 5108 */ 9999,
/* 5109 */ 1049014,
/* 5110 */ 9999,
/* 5111 */ 9999,
/* 5112 */ 1049014,
/* 5113 */ 9999,
/* 5114 */ 1049022,
/* 5115 */ 1050006,
/* 5116 */ 1049028,
/* 5117 */ 9999,
/* 5118 */ 1049028,
/* 5119 */ 9999,
/* 5120 */ 9999,
/* 5121 */ 1049028,
/* 5122 */ 9999,
/* 5123 */ 9999,
/* 5124 */ 9999,
/* 5125 */ 1050007,
/* 5126 */ 9999,
/* 5127 */ 9999,
/* 5128 */ 1050007,
/* 5129 */ 9999,
/* 5130 */ 9999,
/* 5131 */ 9999,
/* 5132 */ 1051013,
/* 5133 */ 9999,
/* 5134 */ 9999,
/* 5135 */ 1051013,
/* 5136 */ 9999,
/* 5137 */ 1051021,
/* 5138 */ 1052006,
/* 5139 */ 1051027,
/* 5140 */ 9999,
/* 5141 */ 1051027,
/* 5142 */ 9999,
/* 5143 */ 9999,
/* 5144 */ 1051027,
/* 5145 */ 9999,
/* 5146 */ 9999,
/* 5147 */ 9999,
/* 5148 */ 1052007,
/* 5149 */ 9999,
/* 5150 */ 9999,
/* 5151 */ 1052007,
/* 5152 */ 9999,
/* 5153 */ 9999,
/* 5154 */ 9999,
/* 5155 */ 1053012,
/* 5156 */ 9999,
/* 5157 */ 9999,
/* 5158 */ 1053012,
/* 5159 */ 9999,
/* 5160 */ 1053020,
/* 5161 */ 1060006,
/* 5162 */ 1053020,
/* 5163 */ 9999,
/* 5164 */ 1053020,
/* 5165 */ 9999,
/* 5166 */ 9999,
/* 5167 */ 1053020,
/* 5168 */ 9999,
/* 5169 */ 1054006,
/* 5170 */ 9999,
/* 5171 */ 1054011,
/* 5172 */ 9999,
/* 5173 */ 1059008,
/* 5174 */ 1059018,
/* 5175 */ 9999,
/* 5176 */ 1059008,
/* 5177 */ 9999,
/* 5178 */ 9999,
/* 5179 */ 9999,
/* 5180 */ 1059009,
/* 5181 */ 9999,
/* 5182 */ 9999,
/* 5183 */ 9999,
/* 5184 */ 1059013,
/* 5185 */ 9999,
/* 5186 */ 9999,
/* 5187 */ 9999,
/* 5188 */ 1060007,
/* 5189 */ 9999,
/* 5190 */ 9999,
/* 5191 */ 9999,
/* 5192 */ 1060011,
/* 5193 */ 9999,
/* 5194 */ 1069003,
/* 5195 */ 9999,
/* 5196 */ 1070005,
/* 5197 */ 1072003,
/* 5198 */ 1074003,
/* 5199 */ 1075005,
/* 5200 */ 1086003,
/* 5201 */ 9999,
/* 5202 */ 1067032,
/* 5203 */ 9999,
/* 5204 */ 1099005,
/* 5205 */ 9999,
/* 5206 */ 1099020,
/* 5207 */ 1101019,
/* 5208 */ 9999,
/* 5209 */ 1099010,
/* 5210 */ 9999,
/* 5211 */ 1070007,
/* 5212 */ 9999,
/* 5213 */ 1070015,
/* 5214 */ 9999,
/* 5215 */ 1071009,
/* 5216 */ 9999,
/* 5217 */ 9999,
/* 5218 */ 1071014,
/* 5219 */ 9999,
/* 5220 */ 9999,
/* 5221 */ 1071014,
/* 5222 */ 9999,
/* 5223 */ 1072004,
/* 5224 */ 9999,
/* 5225 */ 1072009,
/* 5226 */ 9999,
/* 5227 */ 9999,
/* 5228 */ 1072009,
/* 5229 */ 9999,
/* 5230 */ 9999,
/* 5231 */ 9999,
/* 5232 */ 1074006,
/* 5233 */ 9999,
/* 5234 */ 9999,
/* 5235 */ 1074006,
/* 5236 */ 9999,
/* 5237 */ 9999,
/* 5238 */ 1075011,
/* 5239 */ 1075040,
/* 5240 */ 9999,
/* 5241 */ 9999,
/* 5242 */ 1075007,
/* 5243 */ 9999,
/* 5244 */ 9999,
/* 5245 */ 1075007,
/* 5246 */ 9999,
/* 5247 */ 9999,
/* 5248 */ 9999,
/* 5249 */ 1075018,
/* 5250 */ 9999,
/* 5251 */ 9999,
/* 5252 */ 9999,
/* 5253 */ 1075026,
/* 5254 */ 9999,
/* 5255 */ 9999,
/* 5256 */ 9999,
/* 5257 */ 1075037,
/* 5258 */ 9999,
/* 5259 */ 9999,
/* 5260 */ 1087005,
/* 5261 */ 1088005,
/* 5262 */ 1089005,
/* 5263 */ 1090005,
/* 5264 */ 1086006,
/* 5265 */ 9999,
/* 5266 */ 1086006,
/* 5267 */ 9999,
/* 5268 */ 9999,
/* 5269 */ 1086006,
/* 5270 */ 9999,
/* 5271 */ 1087007,
/* 5272 */ 9999,
/* 5273 */ 1087015,
/* 5274 */ 9999,
/* 5275 */ 9999,
/* 5276 */ 1087015,
/* 5277 */ 9999,
/* 5278 */ 1088006,
/* 5279 */ 9999,
/* 5280 */ 1088011,
/* 5281 */ 9999,
/* 5282 */ 9999,
/* 5283 */ 1088011,
/* 5284 */ 9999,
/* 5285 */ 9999,
/* 5286 */ 9999,
/* 5287 */ 1089008,
/* 5288 */ 9999,
/* 5289 */ 9999,
/* 5290 */ 1089008,
/* 5291 */ 9999,
/* 5292 */ 9999,
/* 5293 */ 1090011,
/* 5294 */ 1090040,
/* 5295 */ 9999,
/* 5296 */ 9999,
/* 5297 */ 1090007,
/* 5298 */ 9999,
/* 5299 */ 9999,
/* 5300 */ 1090007,
/* 5301 */ 9999,
/* 5302 */ 9999,
/* 5303 */ 9999,
/* 5304 */ 1090018,
/* 5305 */ 9999,
/* 5306 */ 9999,
/* 5307 */ 9999,
/* 5308 */ 1090026,
/* 5309 */ 9999,
/* 5310 */ 9999,
/* 5311 */ 9999,
/* 5312 */ 1090037,
/* 5313 */ 9999,
/* 5314 */ 1101021,
/* 5315 */ 9999,
/* 5316 */ 1101026,
/* 5317 */ 9999,
/* 5318 */ 9999,
/* 5319 */ 1101026,
/* 5320 */ 9999,
/* 5321 */ 9999,
/* 5322 */ 9999,
/* 5323 */ 1105018,
/* 5324 */ 9999,
/* 5325 */ 9999,
/* 5326 */ 9999,
/* 5327 */ 9999,
/* 5328 */ 1108028,
/* 5329 */ 9999,
/* 5330 */ 1114007,
/* 5331 */ 9999,
/* 5332 */ 9999,
/* 5333 */ 1108008,
/* 5334 */ 9999,
/* 5335 */ 1114008,
/* 5336 */ 1114019,
/* 5337 */ 9999,
/* 5338 */ 1114019,
/* 5339 */ 9999,
/* 5340 */ 9999,
/* 5341 */ 1114019,
/* 5342 */ 9999,
/* 5343 */ 9999,
/* 5344 */ 1118016,
/* 5345 */ 9999,
/* 5346 */ 9999,
/* 5347 */ 9999,
/* 5348 */ 1118013,
/* 5349 */ 9999,
/* 5350 */ 9999,
/* 5351 */ 1119016,
/* 5352 */ 1119023,
/* 5353 */ 9999,
/* 5354 */ 9999,
/* 5355 */ 1119012,
/* 5356 */ 9999,
/* 5357 */ 9999,
/* 5358 */ 1120016,
/* 5359 */ 1120023,
/* 5360 */ 9999,
/* 5361 */ 9999,
/* 5362 */ 1120014,
/* 5363 */ 9999,
/* 5364 */ 9999,
/* 5365 */ 1121016,
/* 5366 */ 1121023,
/* 5367 */ 9999,
/* 5368 */ 9999,
/* 5369 */ 9999,
/* 5370 */ 1121010,
/* 5371 */ 9999,
/* 5372 */ 9999,
/* 5373 */ 9999,
/* 5374 */ 1118017,
/* 5375 */ 9999,
/* 5376 */ 9999,
/* 5377 */ 1118017,
/* 5378 */ 9999,
/* 5379 */ 9999,
/* 5380 */ 9999,
/* 5381 */ 1119017,
/* 5382 */ 9999,
/* 5383 */ 9999,
/* 5384 */ 1119017,
/* 5385 */ 9999,
/* 5386 */ 9999,
/* 5387 */ 9999,
/* 5388 */ 1120017,
/* 5389 */ 9999,
/* 5390 */ 9999,
/* 5391 */ 1120017,
/* 5392 */ 9999,
/* 5393 */ 9999,
/* 5394 */ 9999,
/* 5395 */ 1121017,
/* 5396 */ 9999,
/* 5397 */ 9999,
/* 5398 */ 1121017,
/* 5399 */ 9999,
/* 5400 */ 9999,
/* 5401 */ 9999,
/* 5402 */ 1129005,
/* 5403 */ 9999,
/* 5404 */ 1131009,
/* 5405 */ 9999,
/* 5406 */ 9999,
/* 5407 */ 1126016,
/* 5408 */ 9999,
/* 5409 */ 9999,
/* 5410 */ 1129011,
/* 5411 */ 9999,
/* 5412 */ 1129007,
/* 5413 */ 9999,
/* 5414 */ 9999,
/* 5415 */ 1129007,
/* 5416 */ 9999,
/* 5417 */ 1131010,
/* 5418 */ 1131024,
/* 5419 */ 9999,
/* 5420 */ 1131024,
/* 5421 */ 9999,
/* 5422 */ 9999,
/* 5423 */ 1131024,
/* 5424 */ 9999,
/* 5425 */ 9999,
/* 5426 */ 9999,
/* 5427 */ 1135017,
/* 5428 */ 9999,
/* 5429 */ 9999,
/* 5430 */ 1135011,
/* 5431 */ 9999,
/* 5432 */ 9999,
/* 5433 */ 9999,
/* 5434 */ 1136015,
/* 5435 */ 9999,
/* 5436 */ 9999,
/* 5437 */ 1136009,
/* 5438 */ 9999,
/* 5439 */ 9999,
/* 5440 */ 9999,
/* 5441 */ 1137016,
/* 5442 */ 9999,
/* 5443 */ 9999,
/* 5444 */ 1137010,
/* 5445 */ 9999,
/* 5446 */ 9999,
/* 5447 */ 9999,
/* 5448 */ 1138017,
/* 5449 */ 9999,
/* 5450 */ 9999,
/* 5451 */ 1138011,
/* 5452 */ 9999,
/* 5453 */ 9999,
/* 5454 */ 9999,
/* 5455 */ 1139017,
/* 5456 */ 9999,
/* 5457 */ 9999,
/* 5458 */ 9999,
/* 5459 */ 1139011,
/* 5460 */ 9999,
/* 5461 */ 9999,
/* 5462 */ 9999,
/* 5463 */ 1140017,
/* 5464 */ 1140027,
/* 5465 */ 1141030,
/* 5466 */ 9999,
/* 5467 */ 9999,
/* 5468 */ 1140011,
/* 5469 */ 9999,
/* 5470 */ 9999,
/* 5471 */ 9999,
/* 5472 */ 1142015,
/* 5473 */ 9999,
/* 5474 */ 9999,
/* 5475 */ 1142009,
/* 5476 */ 9999,
/* 5477 */ 9999,
/* 5478 */ 9999,
/* 5479 */ 1144006,
/* 5480 */ 9999,
/* 5481 */ 9999,
/* 5482 */ 1143009,
/* 5483 */ 9999,
/* 5484 */ 9999,
/* 5485 */ 1146011,
/* 5486 */ 1146018,
/* 5487 */ 9999,
/* 5488 */ 9999,
/* 5489 */ 1146009,
/* 5490 */ 9999,
/* 5491 */ 1148005,
/* 5492 */ 9999,
/* 5493 */ 1148013,
/* 5494 */ 9999,
/* 5495 */ 1149005,
/* 5496 */ 9999,
/* 5497 */ 1149014,
/* 5498 */ 9999,
/* 5499 */ 1150005,
/* 5500 */ 9999,
/* 5501 */ 1150016,
/* 5502 */ 9999,
/* 5503 */ 1151005,
/* 5504 */ 9999,
/* 5505 */ 1151017,
/* 5506 */ 9999,
/* 5507 */ 9999,
/* 5508 */ 9999,
/* 5509 */ 1140029,
/* 5510 */ 9999,
/* 5511 */ 9999,
/* 5512 */ 9999,
/* 5513 */ 1140031,
/* 5514 */ 9999,
/* 5515 */ 1144007,
/* 5516 */ 1144020,
/* 5517 */ 9999,
/* 5518 */ 1144020,
/* 5519 */ 9999,
/* 5520 */ 9999,
/* 5521 */ 1144020,
/* 5522 */ 9999,
/* 5523 */ 9999,
/* 5524 */ 9999,
/* 5525 */ 1146012,
/* 5526 */ 9999,
/* 5527 */ 9999,
/* 5528 */ 1146012,
/* 5529 */ 9999,
/* 5530 */ 1146019,
/* 5531 */ 1147005,
/* 5532 */ 1146024,
/* 5533 */ 9999,
/* 5534 */ 1146024,
/* 5535 */ 9999,
/* 5536 */ 9999,
/* 5537 */ 1146024,
/* 5538 */ 9999,
/* 5539 */ 9999,
/* 5540 */ 9999,
/* 5541 */ 1147006,
/* 5542 */ 9999,
/* 5543 */ 9999,
/* 5544 */ 9999,
/* 5545 */ 1147012,
/* 5546 */ 9999,
/* 5547 */ 9999,
/* 5548 */ 1147012,
/* 5549 */ 9999,
/* 5550 */ 1156003,
/* 5551 */ 1161003,
/* 5552 */ 9999,
/* 5553 */ 1181003,
/* 5554 */ 1187003,
/* 5555 */ 1193003,
/* 5556 */ 9999,
/* 5557 */ 1155003,
/* 5558 */ 9999,
/* 5559 */ 9999,
/* 5560 */ 9999,
/* 5561 */ 1156007,
/* 5562 */ 9999,
/* 5563 */ 9999,
/* 5564 */ 9999,
/* 5565 */ 1157010,
/* 5566 */ 9999,
/* 5567 */ 9999,
/* 5568 */ 9999,
/* 5569 */ 1158007,
/* 5570 */ 9999,
/* 5571 */ 1162007,
/* 5572 */ 9999,
/* 5573 */ 1162012,
/* 5574 */ 9999,
/* 5575 */ 1162012,
/* 5576 */ 9999,
/* 5577 */ 9999,
/* 5578 */ 9999,
/* 5579 */ 1163019,
/* 5580 */ 9999,
/* 5581 */ 9999,
/* 5582 */ 1163007,
/* 5583 */ 9999,
/* 5584 */ 1163007,
/* 5585 */ 9999,
/* 5586 */ 9999,
/* 5587 */ 1164011,
/* 5588 */ 9999,
/* 5589 */ 1164007,
/* 5590 */ 9999,
/* 5591 */ 1164007,
/* 5592 */ 9999,
/* 5593 */ 9999,
/* 5594 */ 1165011,
/* 5595 */ 9999,
/* 5596 */ 1165007,
/* 5597 */ 9999,
/* 5598 */ 1165007,
/* 5599 */ 9999,
/* 5600 */ 1166007,
/* 5601 */ 1166017,
/* 5602 */ 1166026,
/* 5603 */ 1166012,
/* 5604 */ 9999,
/* 5605 */ 1166012,
/* 5606 */ 9999,
/* 5607 */ 1169007,
/* 5608 */ 1169012,
/* 5609 */ 9999,
/* 5610 */ 1169012,
/* 5611 */ 9999,
/* 5612 */ 1172007,
/* 5613 */ 9999,
/* 5614 */ 1172012,
/* 5615 */ 9999,
/* 5616 */ 1172012,
/* 5617 */ 9999,
/* 5618 */ 1175007,
/* 5619 */ 1175015,
/* 5620 */ 9999,
/* 5621 */ 1175015,
/* 5622 */ 9999,
/* 5623 */ 9999,
/* 5624 */ 1175015,
/* 5625 */ 9999,
/* 5626 */ 9999,
/* 5627 */ 9999,
/* 5628 */ 1166018,
/* 5629 */ 9999,
/* 5630 */ 9999,
/* 5631 */ 9999,
/* 5632 */ 1166023,
/* 5633 */ 9999,
/* 5634 */ 1181007,
/* 5635 */ 9999,
/* 5636 */ 1181013,
/* 5637 */ 9999,
/* 5638 */ 1182007,
/* 5639 */ 9999,
/* 5640 */ 1182012,
/* 5641 */ 9999,
/* 5642 */ 9999,
/* 5643 */ 1183016,
/* 5644 */ 9999,
/* 5645 */ 1183014,
/* 5646 */ 9999,
/* 5647 */ 1187007,
/* 5648 */ 9999,
/* 5649 */ 1187013,
/* 5650 */ 9999,
/* 5651 */ 1188007,
/* 5652 */ 9999,
/* 5653 */ 1188012,
/* 5654 */ 9999,
/* 5655 */ 9999,
/* 5656 */ 1189016,
/* 5657 */ 9999,
/* 5658 */ 1189014,
/* 5659 */ 9999,
/* 5660 */ 9999,
/* 5661 */ 1189014,
/* 5662 */ 9999,
/* 5663 */ 9999,
/* 5664 */ 1193009,
/* 5665 */ 1194006,
/* 5666 */ 9999,
/* 5667 */ 9999,
/* 5668 */ 1193005,
/* 5669 */ 9999,
/* 5670 */ 9999,
/* 5671 */ 1193005,
/* 5672 */ 9999,
/* 5673 */ 1194008,
/* 5674 */ 1194018,
/* 5675 */ 1194008,
/* 5676 */ 9999,
/* 5677 */ 1194008,
/* 5678 */ 9999,
/* 5679 */ 9999,
/* 5680 */ 1194008,
/* 5681 */ 9999,
/* 5682 */ 9999,
/* 5683 */ 9999,
/* 5684 */ 1194009,
/* 5685 */ 9999,
/* 5686 */ 9999,
/* 5687 */ 9999,
/* 5688 */ 1194013,
/* 5689 */ 9999,
/* 5690 */ 9999,
/* 5691 */ 9999,
/* 5692 */ 9999,
/* 5693 */ 1201031,
/* 5694 */ 9999,
/* 5695 */ 1204007,
/* 5696 */ 9999,
/* 5697 */ 9999,
/* 5698 */ 1201011,
/* 5699 */ 9999,
/* 5700 */ 1204008,
/* 5701 */ 1204022,
/* 5702 */ 9999,
/* 5703 */ 1204022,
/* 5704 */ 9999,
/* 5705 */ 9999,
/* 5706 */ 1204022,
/* 5707 */ 9999,
/* 5708 */ 9999,
/* 5709 */ 1208012,
/* 5710 */ 1208019,
/* 5711 */ 9999,
/* 5712 */ 9999,
/* 5713 */ 1208009,
/* 5714 */ 9999,
/* 5715 */ 9999,
/* 5716 */ 1209011,
/* 5717 */ 1209019,
/* 5718 */ 9999,
/* 5719 */ 9999,
/* 5720 */ 1209009,
/* 5721 */ 9999,
/* 5722 */ 1210005,
/* 5723 */ 9999,
/* 5724 */ 1210014,
/* 5725 */ 9999,
/* 5726 */ 1211005,
/* 5727 */ 9999,
/* 5728 */ 1211016,
/* 5729 */ 9999,
/* 5730 */ 1212005,
/* 5731 */ 9999,
/* 5732 */ 1212017,
/* 5733 */ 9999,
/* 5734 */ 9999,
/* 5735 */ 9999,
/* 5736 */ 1208013,
/* 5737 */ 9999,
/* 5738 */ 9999,
/* 5739 */ 1208013,
/* 5740 */ 9999,
/* 5741 */ 9999,
/* 5742 */ 9999,
/* 5743 */ 1209012,
/* 5744 */ 9999,
/* 5745 */ 9999,
/* 5746 */ 1209012,
/* 5747 */ 9999,
/* 5748 */ 1209020,
/* 5749 */ 1209030,
/* 5750 */ 1209025,
/* 5751 */ 9999,
/* 5752 */ 1209025,
/* 5753 */ 9999,
/* 5754 */ 9999,
/* 5755 */ 1209025,
/* 5756 */ 9999,
/* 5757 */ 9999,
/* 5758 */ 9999,
/* 5759 */ 1209031,
/* 5760 */ 9999,
/* 5761 */ 9999,
/* 5762 */ 9999,
/* 5763 */ 1209037,
/* 5764 */ 9999,
/* 5765 */ 9999,
/* 5766 */ 1209037,
/* 5767 */ 9999,
/* 5768 */ 9999,
/* 5769 */ 9999,
/* 5770 */ 1219004,
/* 5771 */ 9999,
/* 5772 */ 9999,
/* 5773 */ 1216008,
/* 5774 */ 9999,
/* 5775 */ 9999,
/* 5776 */ 1219013,
/* 5777 */ 9999,
/* 5778 */ 1219013,
/* 5779 */ 9999,
/* 5780 */ 1220007,
/* 5781 */ 1220013,
/* 5782 */ 9999,
/* 5783 */ 1220013,
/* 5784 */ 9999,
/* 5785 */ 9999,
/* 5786 */ 1221007,
/* 5787 */ 9999,
/* 5788 */ 1221007,
/* 5789 */ 9999,
/* 5790 */ 9999,
/* 5791 */ 1222007,
/* 5792 */ 9999,
/* 5793 */ 1222007,
/* 5794 */ 9999,
/* 5795 */ 9999,
/* 5796 */ 1223007,
/* 5797 */ 9999,
/* 5798 */ 1223007,
/* 5799 */ 9999,
/* 5800 */ 9999,
/* 5801 */ 1224007,
/* 5802 */ 9999,
/* 5803 */ 1224007,
/* 5804 */ 9999,
/* 5805 */ 9999,
/* 5806 */ 1225011,
/* 5807 */ 9999,
/* 5808 */ 9999,
/* 5809 */ 1225007,
/* 5810 */ 9999,
/* 5811 */ 1225007,
/* 5812 */ 9999,
/* 5813 */ 9999,
/* 5814 */ 1228011,
/* 5815 */ 9999,
/* 5816 */ 1228007,
/* 5817 */ 9999,
/* 5818 */ 1228007,
/* 5819 */ 9999,
/* 5820 */ 9999,
/* 5821 */ 1228007,
/* 5822 */ 9999,
/* 5823 */ 9999,
/* 5824 */ 9999,
/* 5825 */ 1236018,
/* 5826 */ 9999,
/* 5827 */ 9999,
/* 5828 */ 9999,
/* 5829 */ 1239008,
/* 5830 */ 9999,
/* 5831 */ 9999,
/* 5832 */ 9999,
/* 5833 */ 1240009,
/* 5834 */ 9999,
/* 5835 */ 9999,
/* 5836 */ 9999,
/* 5837 */ 1241008,
/* 5838 */ 9999,
/* 5839 */ 9999,
/* 5840 */ 9999,
/* 5841 */ 1242009,
/* 5842 */ 9999,
/* 5843 */ 9999,
/* 5844 */ 9999,
/* 5845 */ 1243007,
/* 5846 */ 9999,
/* 5847 */ 9999,
/* 5848 */ 9999,
/* 5849 */ 1244010,
/* 5850 */ 9999,
/* 5851 */ 9999,
/* 5852 */ 9999,
/* 5853 */ 1245013,
/* 5854 */ 9999,
/* 5855 */ 9999,
/* 5856 */ 1250010,
/* 5857 */ 1250024,
/* 5858 */ 9999,
/* 5859 */ 9999,
/* 5860 */ 9999,
/* 5861 */ 1267003,
/* 5862 */ 1275004,
/* 5863 */ 9999,
/* 5864 */ 9999,
/* 5865 */ 9999,
/* 5866 */ 1249015,
/* 5867 */ 9999,
/* 5868 */ 1267005,
/* 5869 */ 1267021,
/* 5870 */ 1269005,
/* 5871 */ 1269025,
/* 5872 */ 1271005,
/* 5873 */ 9999,
/* 5874 */ 1267013,
/* 5875 */ 9999,
/* 5876 */ 9999,
/* 5877 */ 1267013,
/* 5878 */ 9999,
/* 5879 */ 9999,
/* 5880 */ 1267026,
/* 5881 */ 9999,
/* 5882 */ 1267022,
/* 5883 */ 9999,
/* 5884 */ 9999,
/* 5885 */ 1267022,
/* 5886 */ 9999,
/* 5887 */ 9999,
/* 5888 */ 9999,
/* 5889 */ 1269010,
/* 5890 */ 9999,
/* 5891 */ 9999,
/* 5892 */ 1269010,
/* 5893 */ 9999,
/* 5894 */ 9999,
/* 5895 */ 1269031,
/* 5896 */ 9999,
/* 5897 */ 1269027,
/* 5898 */ 9999,
/* 5899 */ 9999,
/* 5900 */ 1269027,
/* 5901 */ 9999,
/* 5902 */ 9999,
/* 5903 */ 1271010,
/* 5904 */ 1271026,
/* 5905 */ 1273007,
/* 5906 */ 1273027,
/* 5907 */ 1271006,
/* 5908 */ 9999,
/* 5909 */ 1271006,
/* 5910 */ 9999,
/* 5911 */ 9999,
/* 5912 */ 1271006,
/* 5913 */ 9999,
/* 5914 */ 9999,
/* 5915 */ 1271031,
/* 5916 */ 9999,
/* 5917 */ 1271027,
/* 5918 */ 9999,
/* 5919 */ 9999,
/* 5920 */ 1271027,
/* 5921 */ 9999,
/* 5922 */ 9999,
/* 5923 */ 9999,
/* 5924 */ 1273012,
/* 5925 */ 9999,
/* 5926 */ 9999,
/* 5927 */ 1273012,
/* 5928 */ 9999,
/* 5929 */ 9999,
/* 5930 */ 1273033,
/* 5931 */ 9999,
/* 5932 */ 1273029,
/* 5933 */ 9999,
/* 5934 */ 9999,
/* 5935 */ 1273029,
/* 5936 */ 9999,
/* 5937 */ 9999,
/* 5938 */ 9999,
/* 5939 */ 9999,
/* 5940 */ 1275005,
/* 5941 */ 9999,
/* 5942 */ 9999,
/* 5943 */ 1275005,
/* 5944 */ 9999,
/* 5945 */ 9999,
/* 5946 */ 1279010,
/* 5947 */ 9999,
/* 5948 */ 9999,
/* 5949 */ 1286003,
/* 5950 */ 1294004,
/* 5951 */ 9999,
/* 5952 */ 9999,
/* 5953 */ 9999,
/* 5954 */ 1278016,
/* 5955 */ 9999,
/* 5956 */ 1286005,
/* 5957 */ 1286021,
/* 5958 */ 1288005,
/* 5959 */ 1288025,
/* 5960 */ 1290005,
/* 5961 */ 9999,
/* 5962 */ 1286013,
/* 5963 */ 9999,
/* 5964 */ 9999,
/* 5965 */ 1286013,
/* 5966 */ 9999,
/* 5967 */ 9999,
/* 5968 */ 1286026,
/* 5969 */ 9999,
/* 5970 */ 1286022,
/* 5971 */ 9999,
/* 5972 */ 9999,
/* 5973 */ 1286022,
/* 5974 */ 9999,
/* 5975 */ 9999,
/* 5976 */ 9999,
/* 5977 */ 1288010,
/* 5978 */ 9999,
/* 5979 */ 9999,
/* 5980 */ 1288010,
/* 5981 */ 9999,
/* 5982 */ 9999,
/* 5983 */ 1288031,
/* 5984 */ 9999,
/* 5985 */ 1288027,
/* 5986 */ 9999,
/* 5987 */ 9999,
/* 5988 */ 1288027,
/* 5989 */ 9999,
/* 5990 */ 9999,
/* 5991 */ 1290010,
/* 5992 */ 1290026,
/* 5993 */ 1292007,
/* 5994 */ 1292027,
/* 5995 */ 1290006,
/* 5996 */ 9999,
/* 5997 */ 1290006,
/* 5998 */ 9999,
/* 5999 */ 9999,
/* 6000 */ 1290006,
/* 6001 */ 9999,
/* 6002 */ 9999,
/* 6003 */ 1290031,
/* 6004 */ 9999,
/* 6005 */ 1290027,
/* 6006 */ 9999,
/* 6007 */ 9999,
/* 6008 */ 1290027,
/* 6009 */ 9999,
/* 6010 */ 9999,
/* 6011 */ 9999,
/* 6012 */ 1292012,
/* 6013 */ 9999,
/* 6014 */ 9999,
/* 6015 */ 1292012,
/* 6016 */ 9999,
/* 6017 */ 9999,
/* 6018 */ 1292033,
/* 6019 */ 9999,
/* 6020 */ 1292029,
/* 6021 */ 9999,
/* 6022 */ 9999,
/* 6023 */ 1292029,
/* 6024 */ 9999,
/* 6025 */ 9999,
/* 6026 */ 9999,
/* 6027 */ 9999,
/* 6028 */ 1294005,
/* 6029 */ 9999,
/* 6030 */ 9999,
/* 6031 */ 1294005,
/* 6032 */ 9999,
/* 6033 */ 9999,
/* 6034 */ 1298010,
/* 6035 */ 1298024,
/* 6036 */ 9999,
/* 6037 */ 9999,
/* 6038 */ 9999,
/* 6039 */ 9999,
/* 6040 */ 1297016,
/* 6041 */ 9999,
/* 6042 */ 9999,
/* 6043 */ 1331010,
/* 6044 */ 9999,
/* 6045 */ 9999,
/* 6046 */ 1331040,
/* 6047 */ 9999,
/* 6048 */ 9999,
/* 6049 */ 1330016,
/* 6050 */ 9999,
/* 6051 */ 1347003,
/* 6052 */ 9999,
/* 6053 */ 1362003,
/* 6054 */ 9999,
/* 6055 */ 9999,
/* 6056 */ 1346016,
/* 6057 */ 9999,
/* 6058 */ 9999,
/* 6059 */ 9999,
/* 6060 */ 1362004,
/* 6061 */ 9999,
/* 6062 */ 1362004,
/* 6063 */ 9999,
/* 6064 */ 9999,
/* 6065 */ 1362004,
/* 6066 */ 9999,
/* 6067 */ 1377003,
/* 6068 */ 9999,
/* 6069 */ 1394003,
/* 6070 */ 9999,
/* 6071 */ 9999,
/* 6072 */ 1376029,
/* 6073 */ 9999,
/* 6074 */ 9999,
/* 6075 */ 9999,
/* 6076 */ 1394004,
/* 6077 */ 9999,
/* 6078 */ 1394004,
/* 6079 */ 9999,
/* 6080 */ 9999,
/* 6081 */ 1394004,
/* 6082 */ 9999,
/* 6083 */ 1414003,
/* 6084 */ 9999,
/* 6085 */ 1426003,
/* 6086 */ 9999,
/* 6087 */ 9999,
/* 6088 */ 1413016,
/* 6089 */ 9999,
/* 6090 */ 9999,
/* 6091 */ 9999,
/* 6092 */ 1426004,
/* 6093 */ 9999,
/* 6094 */ 1426004,
/* 6095 */ 9999,
/* 6096 */ 9999,
/* 6097 */ 1426004,
/* 6098 */ 9999,
/* 6099 */ 1440003,
/* 6100 */ 9999,
/* 6101 */ 9999,
/* 6102 */ 1440006,
/* 6103 */ 9999,
/* 6104 */ 9999,
/* 6105 */ 9999,
/* 6106 */ 1451012,
/* 6107 */ 9999,
/* 6108 */ 1451025,
/* 6109 */ 9999,
/* 6110 */ 1451045,
/* 6111 */ 9999,
/* 6112 */ 1451006,
/* 6113 */ 9999,
/* 6114 */ 9999,
/* 6115 */ 9999,
/* 6116 */ 1466012,
/* 6117 */ 9999,
/* 6118 */ 1466025,
/* 6119 */ 9999,
/* 6120 */ 1466006,
/* 6121 */ 9999,
/* 6122 */ 9999,
/* 6123 */ 9999,
/* 6124 */ 1475013,
/* 6125 */ 9999,
/* 6126 */ 1475027,
/* 6127 */ 9999,
/* 6128 */ 1475041,
/* 6129 */ 9999,
/* 6130 */ 1476007,
/* 6131 */ 9999,
/* 6132 */ 1475007,
/* 6133 */ 9999,
/* 6134 */ 9999,
/* 6135 */ 9999,
/* 6136 */ 1502016,
/* 6137 */ 9999,
/* 6138 */ 9999,
/* 6139 */ 1512005,
/* 6140 */ 1524005,
/* 6141 */ 9999,
/* 6142 */ 9999,
/* 6143 */ 1502010,
/* 6144 */ 9999,
/* 6145 */ 9999,
/* 6146 */ 9999,
/* 6147 */ 9999,
/* 6148 */ 1573009,
/* 6149 */ 9999,
/* 6150 */ 9999,
/* 6151 */ 9999,
/* 6152 */ 9999,
/* 6153 */ 1577012,
/* 6154 */ 9999,
/* 6155 */ 9999,
/* 6156 */ 1581008,
/* 6157 */ 9999,
/* 6158 */ 9999,
/* 6159 */ 1581033,
/* 6160 */ 9999,
/* 6161 */ 9999,
/* 6162 */ 9999,
/* 6163 */ 1581006,
/* 6164 */ 9999,
/* 6165 */ 9999,
/* 6166 */ 9999,
/* 6167 */ 1590015,
/* 6168 */ 9999,
/* 6169 */ 1590028,
/* 6170 */ 9999,
/* 6171 */ 1590009,
/* 6172 */ 9999,
/* 6173 */ 1601005,
/* 6174 */ 9999,
/* 6175 */ 1601009,
/* 6176 */ 9999,
/* 6177 */ 9999,
/* 6178 */ 1605012,
/* 6179 */ 9999,
/* 6180 */ 9999,
/* 6181 */ 1605010,
/* 6182 */ 9999,
/* 6183 */ 9999,
/* 6184 */ 9999,
/* 6185 */ 9999,
/* 6186 */ 1618010,
/* 6187 */ 9999,
/* 6188 */ 9999,
/* 6189 */ 1512012,
/* 6190 */ 9999,
/* 6191 */ 1514007,
/* 6192 */ 1512010,
/* 6193 */ 9999,
/* 6194 */ 1512010,
/* 6195 */ 9999,
/* 6196 */ 9999,
/* 6197 */ 1512010,
/* 6198 */ 9999,
/* 6199 */ 1512014,
/* 6200 */ 9999,
/* 6201 */ 1512018,
/* 6202 */ 9999,
/* 6203 */ 1512027,
/* 6204 */ 9999,
/* 6205 */ 1512033,
/* 6206 */ 9999,
/* 6207 */ 1514027,
/* 6208 */ 1514009,
/* 6209 */ 9999,
/* 6210 */ 1514009,
/* 6211 */ 9999,
/* 6212 */ 9999,
/* 6213 */ 1514009,
/* 6214 */ 9999,
/* 6215 */ 9999,
/* 6216 */ 9999,
/* 6217 */ 1525007,
/* 6218 */ 9999,
/* 6219 */ 1524013,
/* 6220 */ 9999,
/* 6221 */ 9999,
/* 6222 */ 1524013,
/* 6223 */ 9999,
/* 6224 */ 1525027,
/* 6225 */ 1525009,
/* 6226 */ 9999,
/* 6227 */ 1525009,
/* 6228 */ 9999,
/* 6229 */ 9999,
/* 6230 */ 1525009,
/* 6231 */ 9999,
/* 6232 */ 9999,
/* 6233 */ 1627007,
/* 6234 */ 9999,
/* 6235 */ 9999,
/* 6236 */ 1627003,
/* 6237 */ 9999,
/* 6238 */ 1627025,
/* 6239 */ 1627009,
/* 6240 */ 9999,
/* 6241 */ 1627009,
/* 6242 */ 9999,
/* 6243 */ 9999,
/* 6244 */ 1627009,
/* 6245 */ 9999,
/* 6246 */ 9999,
/* 6247 */ 1632007,
/* 6248 */ 1637007,
/* 6249 */ 9999,
/* 6250 */ 9999,
/* 6251 */ 1631004,
/* 6252 */ 9999,
/* 6253 */ 1632027,
/* 6254 */ 1632009,
/* 6255 */ 9999,
/* 6256 */ 1632009,
/* 6257 */ 9999,
/* 6258 */ 9999,
/* 6259 */ 1633018,
/* 6260 */ 1633016,
/* 6261 */ 9999,
/* 6262 */ 1633016,
/* 6263 */ 9999,
/* 6264 */ 9999,
/* 6265 */ 1633016,
/* 6266 */ 9999,
/* 6267 */ 1637025,
/* 6268 */ 1637009,
/* 6269 */ 9999,
/* 6270 */ 1637009,
/* 6271 */ 9999,
/* 6272 */ 9999,
/* 6273 */ 1637009,
/* 6274 */ 9999,
/* 6275 */ 9999,
/* 6276 */ 9999,
/* 6277 */ 1673010,
/* 6278 */ 9999,
/* 6279 */ 9999,
/* 6280 */ 9999,
/* 6281 */ 1675014,
/* 6282 */ 9999,
/* 6283 */ 9999,
/* 6284 */ 9999,
/* 6285 */ 1677011,
/* 6286 */ 9999,
/* 6287 */ 9999,
/* 6288 */ 9999,
/* 6289 */ 1679013,
/* 6290 */ 9999,
/* 6291 */ 9999,
/* 6292 */ 9999,
/* 6293 */ 1681014,
/* 6294 */ 9999,
/* 6295 */ 9999,
/* 6296 */ 9999,
/* 6297 */ 1683013,
/* 6298 */ 9999,
/* 6299 */ 1685005,
/* 6300 */ 9999,
/* 6301 */ 1685011,
/* 6302 */ 9999,
/* 6303 */ 9999,
/* 6304 */ 1687009,
/* 6305 */ 9999,
/* 6306 */ 9999,
/* 6307 */ 1687005,
/* 6308 */ 9999,
/* 6309 */ 9999,
/* 6310 */ 9999,
/* 6311 */ 1690009,
/* 6312 */ 9999,
/* 6313 */ 1731005,
/* 6314 */ 1731020,
/* 6315 */ 9999,
/* 6316 */ 9999,
/* 6317 */ 1731013,
/* 6318 */ 9999,
/* 6319 */ 1787005,
/* 6320 */ 1787020,
/* 6321 */ 9999,
/* 6322 */ 9999,
/* 6323 */ 1814009,
/* 6324 */ 9999,
/* 6325 */ 9999,
/* 6326 */ 1787013,
/* 6327 */ 9999,
/* 6328 */ 9999,
/* 6329 */ 9999,
/* 6330 */ 1849009,
/* 6331 */ 9999,
/* 6332 */ 9999,
/* 6333 */ 1842009,
/* 6334 */ 9999,
/* 6335 */ 1868005,
/* 6336 */ 9999,
/* 6337 */ 9999,
/* 6338 */ 9999,
/* 6339 */ 1868013,
/* 6340 */ 9999,
/* 6341 */ 9999,
/* 6342 */ 9999,
/* 6343 */ 1731021,
/* 6344 */ 9999,
/* 6345 */ 9999,
/* 6346 */ 9999,
/* 6347 */ 1731033,
/* 6348 */ 9999,
/* 6349 */ 9999,
/* 6350 */ 9999,
/* 6351 */ 1787021,
/* 6352 */ 9999,
/* 6353 */ 9999,
/* 6354 */ 9999,
/* 6355 */ 1787033,
/* 6356 */ 9999,
/* 6357 */ 9999,
/* 6358 */ 9999,
/* 6359 */ 1878007,
/* 6360 */ 9999,
/* 6361 */ 1923005,
/* 6362 */ 9999,
/* 6363 */ 9999,
/* 6364 */ 9999,
/* 6365 */ 1923010,
/* 6366 */ 9999,
/* 6367 */ 1929005,
/* 6368 */ 1929020,
/* 6369 */ 9999,
/* 6370 */ 9999,
/* 6371 */ 1929013,
/* 6372 */ 9999,
/* 6373 */ 9999,
/* 6374 */ 9999,
/* 6375 */ 1929021,
/* 6376 */ 9999,
/* 6377 */ 9999,
/* 6378 */ 9999,
/* 6379 */ 1929033,
/* 6380 */ 9999,
/* 6381 */ 1971005,
/* 6382 */ 9999,
/* 6383 */ 9999,
/* 6384 */ 1971010,
/* 6385 */ 9999,
/* 6386 */ 1973005,
/* 6387 */ 9999,
/* 6388 */ 9999,
/* 6389 */ 1973010,
/* 6390 */ 9999,
/* 6391 */ 9999,
/* 6392 */ 9999,
/* 6393 */ 1975018,
/* 6394 */ 9999,
/* 6395 */ 1975035,
/* 6396 */ 9999,
/* 6397 */ 9999,
/* 6398 */ 1975012,
/* 6399 */ 9999,
/* 6400 */ 9999,
/* 6401 */ 9999,
/* 6402 */ 1977018,
/* 6403 */ 9999,
/* 6404 */ 1977035,
/* 6405 */ 9999,
/* 6406 */ 9999,
/* 6407 */ 1977012,
/* 6408 */ 9999,
/* 6409 */ 1979005,
/* 6410 */ 9999,
/* 6411 */ 1979013,
/* 6412 */ 9999,
/* 6413 */ 9999,
/* 6414 */ 1984018,
/* 6415 */ 9999,
/* 6416 */ 1984016,
/* 6417 */ 9999,
/* 6418 */ 9999,
/* 6419 */ 1986018,
/* 6420 */ 9999,
/* 6421 */ 1986016,
/* 6422 */ 9999,
/* 6423 */ 9999,
/* 6424 */ 1988009,
/* 6425 */ 9999,
/* 6426 */ 1988005,
/* 6427 */ 9999,
/* 6428 */ 9999,
/* 6429 */ 1990009,
/* 6430 */ 9999,
/* 6431 */ 1990005,
/* 6432 */ 9999,
/* 6433 */ 9999,
/* 6434 */ 1992009,
/* 6435 */ 9999,
/* 6436 */ 1992005,
/* 6437 */ 9999,
/* 6438 */ 9999,
/* 6439 */ 1994009,
/* 6440 */ 9999,
/* 6441 */ 1994005,
/* 6442 */ 9999,
/* 6443 */ 9999,
/* 6444 */ 1996009,
/* 6445 */ 9999,
/* 6446 */ 9999,
/* 6447 */ 1996005,
/* 6448 */ 9999,
/* 6449 */ 1998005,
/* 6450 */ 9999,
/* 6451 */ 1998010,
/* 6452 */ 9999,
/* 6453 */ 2003005,
/* 6454 */ 9999,
/* 6455 */ 2003021,
/* 6456 */ 9999,
/* 6457 */ 2003010,
/* 6458 */ 9999,
/* 6459 */ 2005005,
/* 6460 */ 9999,
/* 6461 */ 2005010,
/* 6462 */ 9999,
/* 6463 */ 9999,
/* 6464 */ 2010009,
/* 6465 */ 9999,
/* 6466 */ 2010027,
/* 6467 */ 9999,
/* 6468 */ 2010005,
/* 6469 */ 9999,
/* 6470 */ 2012005,
/* 6471 */ 9999,
/* 6472 */ 2012010,
/* 6473 */ 9999,
/* 6474 */ 2018005,
/* 6475 */ 9999,
/* 6476 */ 2018021,
/* 6477 */ 9999,
/* 6478 */ 2018010,
/* 6479 */ 9999,
/* 6480 */ 2020005,
/* 6481 */ 9999,
/* 6482 */ 2020021,
/* 6483 */ 9999,
/* 6484 */ 2020010,
/* 6485 */ 9999,
/* 6486 */ 2022005,
/* 6487 */ 9999,
/* 6488 */ 2022021,
/* 6489 */ 9999,
/* 6490 */ 2022010,
/* 6491 */ 9999,
/* 6492 */ 2024005,
/* 6493 */ 9999,
/* 6494 */ 2024010,
/* 6495 */ 9999,
/* 6496 */ 2029005,
/* 6497 */ 9999,
/* 6498 */ 2029021,
/* 6499 */ 9999,
/* 6500 */ 2029010,
/* 6501 */ 9999,
/* 6502 */ 2031005,
/* 6503 */ 9999,
/* 6504 */ 2031021,
/* 6505 */ 9999,
/* 6506 */ 2031010,
/* 6507 */ 9999,
/* 6508 */ 2033005,
/* 6509 */ 9999,
/* 6510 */ 2033010,
/* 6511 */ 9999,
/* 6512 */ 2038005,
/* 6513 */ 9999,
/* 6514 */ 2038027,
/* 6515 */ 9999,
/* 6516 */ 2038010,
/* 6517 */ 9999,
/* 6518 */ 2040005,
/* 6519 */ 9999,
/* 6520 */ 2040027,
/* 6521 */ 9999,
/* 6522 */ 2040010,
/* 6523 */ 9999,
/* 6524 */ 2042005,
/* 6525 */ 9999,
/* 6526 */ 2042010,
/* 6527 */ 9999,
/* 6528 */ 2051005,
/* 6529 */ 9999,
/* 6530 */ 2051021,
/* 6531 */ 9999,
/* 6532 */ 2051010,
/* 6533 */ 9999,
/* 6534 */ 2053005,
/* 6535 */ 9999,
/* 6536 */ 2053021,
/* 6537 */ 9999,
/* 6538 */ 2053010,
/* 6539 */ 9999,
/* 6540 */ 2055005,
/* 6541 */ 9999,
/* 6542 */ 2055023,
/* 6543 */ 9999,
/* 6544 */ 2055010,
/* 6545 */ 9999,
/* 6546 */ 2057005,
/* 6547 */ 9999,
/* 6548 */ 2057023,
/* 6549 */ 9999,
/* 6550 */ 2057010,
/* 6551 */ 9999,
/* 6552 */ 2059005,
/* 6553 */ 9999,
/* 6554 */ 2059023,
/* 6555 */ 9999,
/* 6556 */ 2059010,
/* 6557 */ 9999,
/* 6558 */ 2061005,
/* 6559 */ 9999,
/* 6560 */ 2061023,
/* 6561 */ 9999,
/* 6562 */ 2061010,
/* 6563 */ 9999,
/* 6564 */ 2063005,
/* 6565 */ 9999,
/* 6566 */ 2063010,
/* 6567 */ 9999,
/* 6568 */ 2068005,
/* 6569 */ 9999,
/* 6570 */ 2068021,
/* 6571 */ 9999,
/* 6572 */ 2068010,
/* 6573 */ 9999,
/* 6574 */ 2070005,
/* 6575 */ 9999,
/* 6576 */ 2070010,
/* 6577 */ 9999,
/* 6578 */ 2075005,
/* 6579 */ 9999,
/* 6580 */ 2075022,
/* 6581 */ 9999,
/* 6582 */ 2075011,
/* 6583 */ 9999,
/* 6584 */ 2077005,
/* 6585 */ 9999,
/* 6586 */ 2077010,
/* 6587 */ 9999,
/* 6588 */ 2082005,
/* 6589 */ 9999,
/* 6590 */ 2082022,
/* 6591 */ 9999,
/* 6592 */ 2082011,
/* 6593 */ 9999,
/* 6594 */ 2084005,
/* 6595 */ 9999,
/* 6596 */ 2084011,
/* 6597 */ 9999,
/* 6598 */ 2089005,
/* 6599 */ 9999,
/* 6600 */ 2089025,
/* 6601 */ 9999,
/* 6602 */ 2089011,
/* 6603 */ 9999,
/* 6604 */ 2091005,
/* 6605 */ 9999,
/* 6606 */ 2091011,
/* 6607 */ 9999,
/* 6608 */ 2096005,
/* 6609 */ 9999,
/* 6610 */ 2096024,
/* 6611 */ 9999,
/* 6612 */ 2096011,
/* 6613 */ 9999,
/* 6614 */ 2098005,
/* 6615 */ 9999,
/* 6616 */ 2098011,
/* 6617 */ 9999,
/* 6618 */ 2103003,
/* 6619 */ 9999,
/* 6620 */ 2103020,
/* 6621 */ 9999,
/* 6622 */ 2103034,
/* 6623 */ 9999,
/* 6624 */ 2103009,
/* 6625 */ 9999,
/* 6626 */ 2153005,
/* 6627 */ 9999,
/* 6628 */ 2153011,
/* 6629 */ 9999,
/* 6630 */ 2161005,
/* 6631 */ 9999,
/* 6632 */ 2161020,
/* 6633 */ 9999,
/* 6634 */ 2161010,
/* 6635 */ 9999,
/* 6636 */ 2163005,
/* 6637 */ 9999,
/* 6638 */ 2163027,
/* 6639 */ 9999,
/* 6640 */ 2163010,
/* 6641 */ 9999,
/* 6642 */ 2165005,
/* 6643 */ 9999,
/* 6644 */ 2165027,
/* 6645 */ 9999,
/* 6646 */ 2165010,
/* 6647 */ 9999,
/* 6648 */ 2167005,
/* 6649 */ 9999,
/* 6650 */ 2167028,
/* 6651 */ 9999,
/* 6652 */ 2167010,
/* 6653 */ 9999,
/* 6654 */ 2169005,
/* 6655 */ 9999,
/* 6656 */ 2169027,
/* 6657 */ 9999,
/* 6658 */ 2169010,
/* 6659 */ 9999,
/* 6660 */ 2171005,
/* 6661 */ 9999,
/* 6662 */ 2171027,
/* 6663 */ 9999,
/* 6664 */ 2171010,
/* 6665 */ 9999,
/* 6666 */ 2173005,
/* 6667 */ 9999,
/* 6668 */ 2173027,
/* 6669 */ 9999,
/* 6670 */ 2173010,
/* 6671 */ 9999,
/* 6672 */ 2175005,
/* 6673 */ 9999,
/* 6674 */ 2175026,
/* 6675 */ 9999,
/* 6676 */ 2175010,
/* 6677 */ 9999,
/* 6678 */ 2177005,
/* 6679 */ 9999,
/* 6680 */ 2177030,
/* 6681 */ 9999,
/* 6682 */ 2177010,
/* 6683 */ 9999,
/* 6684 */ 2179005,
/* 6685 */ 9999,
/* 6686 */ 2179030,
/* 6687 */ 9999,
/* 6688 */ 2179010,
/* 6689 */ 9999,
/* 6690 */ 2181005,
/* 6691 */ 9999,
/* 6692 */ 2181027,
/* 6693 */ 9999,
/* 6694 */ 2181010,
/* 6695 */ 9999,
/* 6696 */ 2183005,
/* 6697 */ 9999,
/* 6698 */ 2183011,
/* 6699 */ 9999,
/* 6700 */ 2188003,
/* 6701 */ 9999,
/* 6702 */ 2188017,
/* 6703 */ 9999,
/* 6704 */ 2188006,
/* 6705 */ 9999,
/* 6706 */ 2201005,
/* 6707 */ 9999,
/* 6708 */ 2201011,
/* 6709 */ 9999,
/* 6710 */ 2212003,
/* 6711 */ 9999,
/* 6712 */ 2210003,
/* 6713 */ 9999,
/* 6714 */ 2212005,
/* 6715 */ 2239003,
/* 6716 */ 9999,
/* 6717 */ 2212011,
/* 6718 */ 9999,
/* 6719 */ 9999,
/* 6720 */ 2212011,
/* 6721 */ 9999,
/* 6722 */ 9999,
/* 6723 */ 2239009,
/* 6724 */ 2239005,
/* 6725 */ 9999,
/* 6726 */ 2239005,
/* 6727 */ 9999,
/* 6728 */ 9999,
/* 6729 */ 2239005,
0
};
/* only for BIGHASH (see art.c)
extern int DHITS[];
int DHITS[6731];
*/
int TABLE[1421][496];
void init_dirsets() {
TABLE[474][0] = 1;
TABLE[474][379] = 1;
TABLE[474][432] = 1;
TABLE[474][351] = 1;
TABLE[474][349] = 1;
TABLE[474][450] = 1;
TABLE[474][387] = 1;
TABLE[474][371] = 1;
TABLE[474][347] = 1;
TABLE[474][292] = 1;
TABLE[474][331] = 1;
TABLE[474][261] = 1;
TABLE[474][417] = 1;
TABLE[474][308] = 1;
TABLE[474][357] = 1;
TABLE[474][327] = 1;
TABLE[474][421] = 1;
TABLE[474][449] = 1;
TABLE[474][448] = 1;
TABLE[474][340] = 1;
TABLE[474][298] = 1;
TABLE[474][335] = 1;
TABLE[474][446] = 1;
TABLE[474][264] = 1;
TABLE[474][267] = 1;
TABLE[474][263] = 1;
TABLE[474][271] = 1;
TABLE[474][272] = 1;
TABLE[474][273] = 1;
TABLE[474][473] = 1;
TABLE[474][435] = 1;
TABLE[474][333] = 1;
TABLE[474][408] = 1;
TABLE[474][329] = 1;
TABLE[474][45] = 1;
TABLE[474][43] = 1;
TABLE[474][40] = 1;
TABLE[474][256] = 1;
TABLE[474][457] = 1;
TABLE[474][344] = 1;
TABLE[474][259] = 1;
TABLE[1][0] = 1;
TABLE[1][450] = 1;
TABLE[1][349] = 1;
TABLE[1][351] = 1;
TABLE[1][432] = 1;
TABLE[1][379] = 1;
TABLE[1][473] = 1;
TABLE[1][273] = 1;
TABLE[1][272] = 1;
TABLE[1][271] = 1;
TABLE[1][263] = 1;
TABLE[1][267] = 1;
TABLE[1][264] = 1;
TABLE[1][446] = 1;
TABLE[1][335] = 1;
TABLE[1][298] = 1;
TABLE[1][340] = 1;
TABLE[1][448] = 1;
TABLE[1][449] = 1;
TABLE[1][421] = 1;
TABLE[1][327] = 1;
TABLE[1][357] = 1;
TABLE[1][308] = 1;
TABLE[1][417] = 1;
TABLE[1][261] = 1;
TABLE[1][331] = 1;
TABLE[1][292] = 1;
TABLE[1][347] = 1;
TABLE[1][371] = 1;
TABLE[1][387] = 1;
TABLE[1][43] = 1;
TABLE[1][45] = 1;
TABLE[1][329] = 1;
TABLE[1][408] = 1;
TABLE[1][333] = 1;
TABLE[1][435] = 1;
TABLE[1][259] = 1;
TABLE[1][344] = 1;
TABLE[1][457] = 1;
TABLE[1][256] = 1;
TABLE[1][40] = 1;
TABLE[475][0] = 1;
TABLE[475][379] = 1;
TABLE[475][432] = 1;
TABLE[475][351] = 1;
TABLE[475][349] = 1;
TABLE[475][450] = 1;
TABLE[475][387] = 1;
TABLE[475][371] = 1;
TABLE[475][347] = 1;
TABLE[475][292] = 1;
TABLE[475][331] = 1;
TABLE[475][261] = 1;
TABLE[475][417] = 1;
TABLE[475][308] = 1;
TABLE[475][357] = 1;
TABLE[475][327] = 1;
TABLE[475][421] = 1;
TABLE[475][449] = 1;
TABLE[475][448] = 1;
TABLE[475][340] = 1;
TABLE[475][298] = 1;
TABLE[475][335] = 1;
TABLE[475][446] = 1;
TABLE[475][264] = 1;
TABLE[475][267] = 1;
TABLE[475][263] = 1;
TABLE[475][271] = 1;
TABLE[475][272] = 1;
TABLE[475][273] = 1;
TABLE[475][473] = 1;
TABLE[475][435] = 1;
TABLE[475][333] = 1;
TABLE[475][408] = 1;
TABLE[475][329] = 1;
TABLE[475][45] = 1;
TABLE[475][43] = 1;
TABLE[475][40] = 1;
TABLE[475][256] = 1;
TABLE[475][457] = 1;
TABLE[475][344] = 1;
TABLE[475][259] = 1;
TABLE[1019][0] = 1;
TABLE[2][379] = 1;
TABLE[3][432] = 1;
TABLE[4][351] = 1;
TABLE[5][387] = 1;
TABLE[6][371] = 1;
TABLE[7][347] = 1;
TABLE[8][292] = 1;
TABLE[9][331] = 1;
TABLE[10][261] = 1;
TABLE[11][450] = 1;
TABLE[11][349] = 1;
TABLE[11][351] = 1;
TABLE[11][432] = 1;
TABLE[11][379] = 1;
TABLE[11][473] = 1;
TABLE[11][273] = 1;
TABLE[11][272] = 1;
TABLE[11][271] = 1;
TABLE[11][263] = 1;
TABLE[11][267] = 1;
TABLE[11][264] = 1;
TABLE[11][446] = 1;
TABLE[11][335] = 1;
TABLE[11][298] = 1;
TABLE[11][340] = 1;
TABLE[11][448] = 1;
TABLE[11][449] = 1;
TABLE[11][421] = 1;
TABLE[11][327] = 1;
TABLE[11][357] = 1;
TABLE[11][308] = 1;
TABLE[11][417] = 1;
TABLE[11][261] = 1;
TABLE[11][331] = 1;
TABLE[11][292] = 1;
TABLE[11][347] = 1;
TABLE[11][371] = 1;
TABLE[11][387] = 1;
TABLE[11][0] = 1;
TABLE[11][43] = 1;
TABLE[11][45] = 1;
TABLE[11][329] = 1;
TABLE[11][408] = 1;
TABLE[11][333] = 1;
TABLE[11][435] = 1;
TABLE[11][259] = 1;
TABLE[11][344] = 1;
TABLE[11][457] = 1;
TABLE[11][256] = 1;
TABLE[11][40] = 1;
TABLE[12][417] = 1;
TABLE[13][308] = 1;
TABLE[14][357] = 1;
TABLE[15][327] = 1;
TABLE[16][421] = 1;
TABLE[17][449] = 1;
TABLE[18][448] = 1;
TABLE[19][340] = 1;
TABLE[20][298] = 1;
TABLE[21][335] = 1;
TABLE[22][261] = 1;
TABLE[23][446] = 1;
TABLE[24][446] = 1;
TABLE[25][473] = 1;
TABLE[25][273] = 1;
TABLE[25][272] = 1;
TABLE[25][271] = 1;
TABLE[25][263] = 1;
TABLE[25][267] = 1;
TABLE[25][264] = 1;
TABLE[26][446] = 1;
TABLE[27][446] = 1;
TABLE[28][349] = 1;
TABLE[29][450] = 1;
TABLE[476][58] = 1;
TABLE[477][325] = 1;
TABLE[1020][257] = 1;
TABLE[478][59] = 1;
TABLE[1021][125] = 1;
TABLE[30][36] = 1;
TABLE[31][36] = 1;
TABLE[32][43] = 1;
TABLE[32][45] = 1;
TABLE[32][259] = 1;
TABLE[32][344] = 1;
TABLE[32][457] = 1;
TABLE[32][256] = 1;
TABLE[32][40] = 1;
TABLE[33][43] = 1;
TABLE[33][45] = 1;
TABLE[33][259] = 1;
TABLE[33][344] = 1;
TABLE[33][457] = 1;
TABLE[33][256] = 1;
TABLE[33][40] = 1;
TABLE[34][45] = 1;
TABLE[34][43] = 1;
TABLE[34][40] = 1;
TABLE[34][256] = 1;
TABLE[34][457] = 1;
TABLE[34][344] = 1;
TABLE[34][259] = 1;
TABLE[35][45] = 1;
TABLE[35][43] = 1;
TABLE[35][40] = 1;
TABLE[35][256] = 1;
TABLE[35][457] = 1;
TABLE[35][344] = 1;
TABLE[35][259] = 1;
TABLE[36][45] = 1;
TABLE[36][43] = 1;
TABLE[36][40] = 1;
TABLE[36][256] = 1;
TABLE[36][457] = 1;
TABLE[36][344] = 1;
TABLE[36][259] = 1;
TABLE[37][45] = 1;
TABLE[37][43] = 1;
TABLE[37][40] = 1;
TABLE[37][256] = 1;
TABLE[37][457] = 1;
TABLE[37][344] = 1;
TABLE[37][259] = 1;
TABLE[38][45] = 1;
TABLE[38][43] = 1;
TABLE[38][40] = 1;
TABLE[38][256] = 1;
TABLE[38][457] = 1;
TABLE[38][344] = 1;
TABLE[38][259] = 1;
TABLE[39][45] = 1;
TABLE[39][43] = 1;
TABLE[39][40] = 1;
TABLE[39][256] = 1;
TABLE[39][457] = 1;
TABLE[39][344] = 1;
TABLE[39][259] = 1;
TABLE[40][43] = 1;
TABLE[40][45] = 1;
TABLE[40][259] = 1;
TABLE[40][344] = 1;
TABLE[40][457] = 1;
TABLE[40][256] = 1;
TABLE[40][40] = 1;
TABLE[41][43] = 1;
TABLE[42][45] = 1;
TABLE[43][40] = 1;
TABLE[43][256] = 1;
TABLE[43][457] = 1;
TABLE[43][344] = 1;
TABLE[43][259] = 1;
TABLE[44][40] = 1;
TABLE[45][256] = 1;
TABLE[46][457] = 1;
TABLE[47][344] = 1;
TABLE[48][259] = 1;
TABLE[49][40] = 1;
TABLE[49][256] = 1;
TABLE[49][457] = 1;
TABLE[49][344] = 1;
TABLE[49][259] = 1;
TABLE[50][40] = 1;
TABLE[50][256] = 1;
TABLE[50][457] = 1;
TABLE[50][344] = 1;
TABLE[50][259] = 1;
TABLE[51][40] = 1;
TABLE[51][256] = 1;
TABLE[51][457] = 1;
TABLE[51][344] = 1;
TABLE[51][259] = 1;
TABLE[52][43] = 1;
TABLE[52][45] = 1;
TABLE[52][259] = 1;
TABLE[52][344] = 1;
TABLE[52][457] = 1;
TABLE[52][256] = 1;
TABLE[52][40] = 1;
TABLE[53][40] = 1;
TABLE[53][256] = 1;
TABLE[53][457] = 1;
TABLE[53][344] = 1;
TABLE[53][259] = 1;
TABLE[54][40] = 1;
TABLE[54][256] = 1;
TABLE[54][457] = 1;
TABLE[54][344] = 1;
TABLE[54][259] = 1;
TABLE[55][36] = 1;
TABLE[55][418] = 1;
TABLE[55][316] = 1;
TABLE[55][299] = 1;
TABLE[55][40] = 1;
TABLE[55][457] = 1;
TABLE[55][344] = 1;
TABLE[55][259] = 1;
TABLE[55][256] = 1;
TABLE[55][366] = 1;
TABLE[479][366] = 1;
TABLE[1022][40] = 1;
TABLE[1022][256] = 1;
TABLE[1022][457] = 1;
TABLE[1022][344] = 1;
TABLE[1022][259] = 1;
TABLE[1022][316] = 1;
TABLE[1022][299] = 1;
TABLE[1022][36] = 1;
TABLE[1022][418] = 1;
TABLE[480][256] = 1;
TABLE[1023][40] = 1;
TABLE[1023][256] = 1;
TABLE[1023][457] = 1;
TABLE[1023][344] = 1;
TABLE[1023][259] = 1;
TABLE[1023][316] = 1;
TABLE[1023][299] = 1;
TABLE[1023][36] = 1;
TABLE[1023][418] = 1;
TABLE[481][259] = 1;
TABLE[481][344] = 1;
TABLE[481][457] = 1;
TABLE[481][256] = 1;
TABLE[481][40] = 1;
TABLE[1024][316] = 1;
TABLE[1024][299] = 1;
TABLE[1024][36] = 1;
TABLE[1024][418] = 1;
TABLE[482][316] = 1;
TABLE[483][299] = 1;
TABLE[1025][36] = 1;
TABLE[1025][418] = 1;
TABLE[484][36] = 1;
TABLE[485][418] = 1;
TABLE[486][43] = 1;
TABLE[487][45] = 1;
TABLE[1026][470] = 1;
TABLE[1026][398] = 1;
TABLE[1026][294] = 1;
TABLE[1026][59] = 1;
TABLE[1026][44] = 1;
TABLE[1026][334] = 1;
TABLE[490][470] = 1;
TABLE[491][398] = 1;
TABLE[492][398] = 1;
TABLE[493][398] = 1;
TABLE[494][398] = 1;
TABLE[495][294] = 1;
TABLE[1029][59] = 1;
TABLE[1029][44] = 1;
TABLE[1029][334] = 1;
TABLE[489][43] = 1;
TABLE[489][45] = 1;
TABLE[489][259] = 1;
TABLE[489][344] = 1;
TABLE[489][457] = 1;
TABLE[489][256] = 1;
TABLE[489][40] = 1;
TABLE[1028][93] = 1;
TABLE[488][44] = 1;
TABLE[1027][45] = 1;
TABLE[1027][43] = 1;
TABLE[1027][40] = 1;
TABLE[1027][256] = 1;
TABLE[1027][457] = 1;
TABLE[1027][344] = 1;
TABLE[1027][259] = 1;
TABLE[1027][93] = 1;
TABLE[56][355] = 1;
TABLE[496][58] = 1;
TABLE[499][44] = 1;
TABLE[499][334] = 1;
TABLE[499][366] = 1;
TABLE[499][256] = 1;
TABLE[499][259] = 1;
TABLE[499][344] = 1;
TABLE[499][457] = 1;
TABLE[499][40] = 1;
TABLE[499][299] = 1;
TABLE[499][316] = 1;
TABLE[499][418] = 1;
TABLE[499][36] = 1;
TABLE[1030][366] = 1;
TABLE[1030][256] = 1;
TABLE[1030][259] = 1;
TABLE[1030][344] = 1;
TABLE[1030][457] = 1;
TABLE[1030][40] = 1;
TABLE[1030][299] = 1;
TABLE[1030][316] = 1;
TABLE[1030][418] = 1;
TABLE[1030][36] = 1;
TABLE[1030][59] = 1;
TABLE[497][44] = 1;
TABLE[498][334] = 1;
TABLE[57][367] = 1;
TABLE[57][356] = 1;
TABLE[500][367] = 1;
TABLE[501][356] = 1;
TABLE[502][58] = 1;
TABLE[1031][319] = 1;
TABLE[1031][375] = 1;
TABLE[1031][302] = 1;
TABLE[1031][274] = 1;
TABLE[1031][259] = 1;
TABLE[1031][256] = 1;
TABLE[503][274] = 1;
TABLE[503][302] = 1;
TABLE[503][375] = 1;
TABLE[503][319] = 1;
TABLE[1032][259] = 1;
TABLE[1032][256] = 1;
TABLE[504][259] = 1;
TABLE[505][256] = 1;
TABLE[506][399] = 1;
TABLE[1033][59] = 1;
TABLE[58][259] = 1;
TABLE[58][256] = 1;
TABLE[58][319] = 1;
TABLE[58][375] = 1;
TABLE[58][302] = 1;
TABLE[58][274] = 1;
TABLE[507][274] = 1;
TABLE[507][302] = 1;
TABLE[507][375] = 1;
TABLE[507][319] = 1;
TABLE[1034][259] = 1;
TABLE[1034][256] = 1;
TABLE[508][259] = 1;
TABLE[509][256] = 1;
TABLE[510][399] = 1;
TABLE[1035][91] = 1;
TABLE[1035][470] = 1;
TABLE[1035][293] = 1;
TABLE[1035][44] = 1;
TABLE[1035][259] = 1;
TABLE[1035][59] = 1;
TABLE[59][399] = 1;
TABLE[511][44] = 1;
TABLE[1036][59] = 1;
TABLE[60][43] = 1;
TABLE[60][45] = 1;
TABLE[60][259] = 1;
TABLE[60][344] = 1;
TABLE[60][457] = 1;
TABLE[60][256] = 1;
TABLE[60][40] = 1;
TABLE[512][44] = 1;
TABLE[1037][257] = 1;
TABLE[1037][123] = 1;
TABLE[514][257] = 1;
TABLE[515][123] = 1;
TABLE[513][47] = 1;
TABLE[1038][44] = 1;
TABLE[1038][59] = 1;
TABLE[61][474] = 1;
TABLE[61][475] = 1;
TABLE[61][43] = 1;
TABLE[61][45] = 1;
TABLE[61][33] = 1;
TABLE[61][126] = 1;
TABLE[61][58] = 1;
TABLE[61][383] = 1;
TABLE[61][386] = 1;
TABLE[61][258] = 1;
TABLE[61][40] = 1;
TABLE[61][36] = 1;
TABLE[61][394] = 1;
TABLE[61][344] = 1;
TABLE[61][457] = 1;
TABLE[61][257] = 1;
TABLE[61][259] = 1;
TABLE[61][256] = 1;
TABLE[62][366] = 1;
TABLE[62][352] = 1;
TABLE[62][445] = 1;
TABLE[62][301] = 1;
TABLE[62][313] = 1;
TABLE[62][330] = 1;
TABLE[62][468] = 1;
TABLE[62][424] = 1;
TABLE[62][256] = 1;
TABLE[62][259] = 1;
TABLE[62][257] = 1;
TABLE[62][457] = 1;
TABLE[62][344] = 1;
TABLE[62][394] = 1;
TABLE[62][36] = 1;
TABLE[62][40] = 1;
TABLE[62][258] = 1;
TABLE[62][386] = 1;
TABLE[62][383] = 1;
TABLE[62][58] = 1;
TABLE[62][126] = 1;
TABLE[62][33] = 1;
TABLE[62][45] = 1;
TABLE[62][43] = 1;
TABLE[62][475] = 1;
TABLE[62][474] = 1;
TABLE[62][123] = 1;
TABLE[63][59] = 1;
TABLE[63][44] = 1;
TABLE[63][259] = 1;
TABLE[63][344] = 1;
TABLE[63][457] = 1;
TABLE[63][256] = 1;
TABLE[63][40] = 1;
TABLE[63][45] = 1;
TABLE[63][61] = 1;
TABLE[63][43] = 1;
TABLE[517][40] = 1;
TABLE[517][256] = 1;
TABLE[517][457] = 1;
TABLE[517][344] = 1;
TABLE[517][259] = 1;
TABLE[517][43] = 1;
TABLE[518][45] = 1;
TABLE[519][61] = 1;
TABLE[520][43] = 1;
TABLE[520][45] = 1;
TABLE[520][259] = 1;
TABLE[520][344] = 1;
TABLE[520][457] = 1;
TABLE[520][256] = 1;
TABLE[520][40] = 1;
TABLE[516][43] = 1;
TABLE[1039][40] = 1;
TABLE[1039][256] = 1;
TABLE[1039][457] = 1;
TABLE[1039][344] = 1;
TABLE[1039][259] = 1;
TABLE[524][40] = 1;
TABLE[1040][59] = 1;
TABLE[1040][44] = 1;
TABLE[521][386] = 1;
TABLE[522][383] = 1;
TABLE[523][390] = 1;
TABLE[64][387] = 1;
TABLE[526][44] = 1;
TABLE[1042][123] = 1;
TABLE[525][44] = 1;
TABLE[1041][123] = 1;
TABLE[527][125] = 1;
TABLE[527][325] = 1;
TABLE[527][324] = 1;
TABLE[527][449] = 1;
TABLE[527][363] = 1;
TABLE[527][364] = 1;
TABLE[527][365] = 1;
TABLE[527][439] = 1;
TABLE[527][423] = 1;
TABLE[527][368] = 1;
TABLE[527][295] = 1;
TABLE[527][293] = 1;
TABLE[527][322] = 1;
TABLE[527][291] = 1;
TABLE[527][380] = 1;
TABLE[527][345] = 1;
TABLE[527][349] = 1;
TABLE[527][430] = 1;
TABLE[527][434] = 1;
TABLE[527][388] = 1;
TABLE[527][436] = 1;
TABLE[527][329] = 1;
TABLE[527][408] = 1;
TABLE[527][355] = 1;
TABLE[527][399] = 1;
TABLE[527][306] = 1;
TABLE[527][373] = 1;
TABLE[527][43] = 1;
TABLE[527][367] = 1;
TABLE[527][356] = 1;
TABLE[527][276] = 1;
TABLE[527][433] = 1;
TABLE[527][376] = 1;
TABLE[527][310] = 1;
TABLE[527][264] = 1;
TABLE[527][267] = 1;
TABLE[527][263] = 1;
TABLE[527][271] = 1;
TABLE[527][272] = 1;
TABLE[527][273] = 1;
TABLE[527][473] = 1;
TABLE[1043][125] = 1;
TABLE[65][325] = 1;
TABLE[66][324] = 1;
TABLE[67][373] = 1;
TABLE[67][306] = 1;
TABLE[68][373] = 1;
TABLE[68][306] = 1;
TABLE[69][449] = 1;
TABLE[70][363] = 1;
TABLE[71][364] = 1;
TABLE[72][365] = 1;
TABLE[72][43] = 1;
TABLE[73][356] = 1;
TABLE[73][367] = 1;
TABLE[74][439] = 1;
TABLE[75][439] = 1;
TABLE[76][439] = 1;
TABLE[77][439] = 1;
TABLE[78][423] = 1;
TABLE[79][368] = 1;
TABLE[80][295] = 1;
TABLE[81][293] = 1;
TABLE[82][293] = 1;
TABLE[83][322] = 1;
TABLE[84][291] = 1;
TABLE[85][380] = 1;
TABLE[86][345] = 1;
TABLE[87][349] = 1;
TABLE[88][276] = 1;
TABLE[89][430] = 1;
TABLE[90][434] = 1;
TABLE[91][388] = 1;
TABLE[92][436] = 1;
TABLE[93][329] = 1;
TABLE[94][408] = 1;
TABLE[95][355] = 1;
TABLE[96][399] = 1;
TABLE[97][310] = 1;
TABLE[97][376] = 1;
TABLE[97][433] = 1;
TABLE[98][473] = 1;
TABLE[98][273] = 1;
TABLE[98][272] = 1;
TABLE[98][271] = 1;
TABLE[98][263] = 1;
TABLE[98][267] = 1;
TABLE[98][264] = 1;
TABLE[528][58] = 1;
TABLE[1044][257] = 1;
TABLE[529][58] = 1;
TABLE[1045][43] = 1;
TABLE[1045][45] = 1;
TABLE[1045][259] = 1;
TABLE[1045][344] = 1;
TABLE[1045][457] = 1;
TABLE[1045][256] = 1;
TABLE[1045][40] = 1;
TABLE[530][373] = 1;
TABLE[531][306] = 1;
TABLE[532][58] = 1;
TABLE[1046][43] = 1;
TABLE[1046][45] = 1;
TABLE[1046][259] = 1;
TABLE[1046][344] = 1;
TABLE[1046][457] = 1;
TABLE[1046][256] = 1;
TABLE[1046][40] = 1;
TABLE[533][373] = 1;
TABLE[534][306] = 1;
TABLE[535][58] = 1;
TABLE[1047][43] = 1;
TABLE[1047][45] = 1;
TABLE[1047][259] = 1;
TABLE[1047][344] = 1;
TABLE[1047][457] = 1;
TABLE[1047][256] = 1;
TABLE[1047][40] = 1;
TABLE[536][58] = 1;
TABLE[1048][43] = 1;
TABLE[1048][45] = 1;
TABLE[1048][259] = 1;
TABLE[1048][344] = 1;
TABLE[1048][457] = 1;
TABLE[1048][256] = 1;
TABLE[1048][40] = 1;
TABLE[537][44] = 1;
TABLE[1049][59] = 1;
TABLE[538][58] = 1;
TABLE[1050][43] = 1;
TABLE[1050][45] = 1;
TABLE[1050][259] = 1;
TABLE[1050][344] = 1;
TABLE[1050][457] = 1;
TABLE[1050][256] = 1;
TABLE[1050][40] = 1;
TABLE[539][58] = 1;
TABLE[1051][43] = 1;
TABLE[1051][45] = 1;
TABLE[1051][259] = 1;
TABLE[1051][344] = 1;
TABLE[1051][457] = 1;
TABLE[1051][256] = 1;
TABLE[1051][40] = 1;
TABLE[540][43] = 1;
TABLE[1052][365] = 1;
TABLE[541][58] = 1;
TABLE[1053][43] = 1;
TABLE[1053][45] = 1;
TABLE[1053][259] = 1;
TABLE[1053][344] = 1;
TABLE[1053][457] = 1;
TABLE[1053][256] = 1;
TABLE[1053][40] = 1;
TABLE[542][58] = 1;
TABLE[1054][45] = 1;
TABLE[1054][43] = 1;
TABLE[1054][40] = 1;
TABLE[1054][256] = 1;
TABLE[1054][457] = 1;
TABLE[1054][344] = 1;
TABLE[1054][259] = 1;
TABLE[1054][59] = 1;
TABLE[544][43] = 1;
TABLE[544][45] = 1;
TABLE[544][259] = 1;
TABLE[544][344] = 1;
TABLE[544][457] = 1;
TABLE[544][256] = 1;
TABLE[544][40] = 1;
TABLE[1056][59] = 1;
TABLE[543][44] = 1;
TABLE[1055][45] = 1;
TABLE[1055][43] = 1;
TABLE[1055][40] = 1;
TABLE[1055][256] = 1;
TABLE[1055][457] = 1;
TABLE[1055][344] = 1;
TABLE[1055][259] = 1;
TABLE[1055][59] = 1;
TABLE[545][58] = 1;
TABLE[1057][45] = 1;
TABLE[1057][43] = 1;
TABLE[1057][40] = 1;
TABLE[1057][256] = 1;
TABLE[1057][457] = 1;
TABLE[1057][344] = 1;
TABLE[1057][259] = 1;
TABLE[1057][59] = 1;
TABLE[547][43] = 1;
TABLE[547][45] = 1;
TABLE[547][259] = 1;
TABLE[547][344] = 1;
TABLE[547][457] = 1;
TABLE[547][256] = 1;
TABLE[547][40] = 1;
TABLE[1059][59] = 1;
TABLE[546][44] = 1;
TABLE[1058][45] = 1;
TABLE[1058][43] = 1;
TABLE[1058][40] = 1;
TABLE[1058][256] = 1;
TABLE[1058][457] = 1;
TABLE[1058][344] = 1;
TABLE[1058][259] = 1;
TABLE[1058][59] = 1;
TABLE[548][58] = 1;
TABLE[1060][45] = 1;
TABLE[1060][43] = 1;
TABLE[1060][40] = 1;
TABLE[1060][256] = 1;
TABLE[1060][457] = 1;
TABLE[1060][344] = 1;
TABLE[1060][259] = 1;
TABLE[549][44] = 1;
TABLE[1061][59] = 1;
TABLE[550][58] = 1;
TABLE[1062][43] = 1;
TABLE[1062][45] = 1;
TABLE[1062][259] = 1;
TABLE[1062][344] = 1;
TABLE[1062][457] = 1;
TABLE[1062][256] = 1;
TABLE[1062][40] = 1;
TABLE[551][58] = 1;
TABLE[1063][43] = 1;
TABLE[1063][45] = 1;
TABLE[1063][259] = 1;
TABLE[1063][344] = 1;
TABLE[1063][457] = 1;
TABLE[1063][256] = 1;
TABLE[1063][40] = 1;
TABLE[552][58] = 1;
TABLE[1064][43] = 1;
TABLE[1064][45] = 1;
TABLE[1064][259] = 1;
TABLE[1064][344] = 1;
TABLE[1064][457] = 1;
TABLE[1064][256] = 1;
TABLE[1064][40] = 1;
TABLE[553][58] = 1;
TABLE[1065][43] = 1;
TABLE[1065][45] = 1;
TABLE[1065][259] = 1;
TABLE[1065][344] = 1;
TABLE[1065][457] = 1;
TABLE[1065][256] = 1;
TABLE[1065][40] = 1;
TABLE[554][43] = 1;
TABLE[554][45] = 1;
TABLE[554][259] = 1;
TABLE[554][344] = 1;
TABLE[554][457] = 1;
TABLE[554][256] = 1;
TABLE[554][40] = 1;
TABLE[1066][43] = 1;
TABLE[1066][45] = 1;
TABLE[1066][259] = 1;
TABLE[1066][344] = 1;
TABLE[1066][457] = 1;
TABLE[1066][256] = 1;
TABLE[1066][40] = 1;
TABLE[555][58] = 1;
TABLE[1067][126] = 1;
TABLE[1067][43] = 1;
TABLE[1067][45] = 1;
TABLE[1067][259] = 1;
TABLE[1067][344] = 1;
TABLE[1067][457] = 1;
TABLE[1067][256] = 1;
TABLE[1067][40] = 1;
TABLE[1067][59] = 1;
TABLE[556][45] = 1;
TABLE[556][43] = 1;
TABLE[556][126] = 1;
TABLE[556][40] = 1;
TABLE[556][256] = 1;
TABLE[556][457] = 1;
TABLE[556][344] = 1;
TABLE[556][259] = 1;
TABLE[1068][59] = 1;
TABLE[557][44] = 1;
TABLE[1069][59] = 1;
TABLE[558][58] = 1;
TABLE[1070][43] = 1;
TABLE[1070][45] = 1;
TABLE[1070][259] = 1;
TABLE[1070][344] = 1;
TABLE[1070][457] = 1;
TABLE[1070][256] = 1;
TABLE[1070][40] = 1;
TABLE[559][58] = 1;
TABLE[1071][43] = 1;
TABLE[1071][45] = 1;
TABLE[1071][259] = 1;
TABLE[1071][344] = 1;
TABLE[1071][457] = 1;
TABLE[1071][256] = 1;
TABLE[1071][40] = 1;
TABLE[560][37] = 1;
TABLE[1072][59] = 1;
TABLE[561][58] = 1;
TABLE[1073][43] = 1;
TABLE[1073][45] = 1;
TABLE[1073][259] = 1;
TABLE[1073][344] = 1;
TABLE[1073][457] = 1;
TABLE[1073][256] = 1;
TABLE[1073][40] = 1;
TABLE[562][37] = 1;
TABLE[1074][59] = 1;
TABLE[563][58] = 1;
TABLE[1075][36] = 1;
TABLE[1075][59] = 1;
TABLE[565][36] = 1;
TABLE[1077][59] = 1;
TABLE[564][44] = 1;
TABLE[1076][36] = 1;
TABLE[1076][59] = 1;
TABLE[566][58] = 1;
TABLE[1078][36] = 1;
TABLE[1078][59] = 1;
TABLE[568][36] = 1;
TABLE[1080][59] = 1;
TABLE[567][44] = 1;
TABLE[1079][36] = 1;
TABLE[1079][59] = 1;
TABLE[569][58] = 1;
TABLE[1081][36] = 1;
TABLE[1081][59] = 1;
TABLE[571][36] = 1;
TABLE[1083][59] = 1;
TABLE[570][44] = 1;
TABLE[1082][36] = 1;
TABLE[1082][59] = 1;
TABLE[99][274] = 1;
TABLE[100][302] = 1;
TABLE[101][375] = 1;
TABLE[102][319] = 1;
TABLE[103][43] = 1;
TABLE[103][45] = 1;
TABLE[103][259] = 1;
TABLE[103][344] = 1;
TABLE[103][457] = 1;
TABLE[103][256] = 1;
TABLE[103][40] = 1;
TABLE[104][43] = 1;
TABLE[104][45] = 1;
TABLE[104][259] = 1;
TABLE[104][344] = 1;
TABLE[104][457] = 1;
TABLE[104][256] = 1;
TABLE[104][40] = 1;
TABLE[572][259] = 1;
TABLE[572][344] = 1;
TABLE[572][457] = 1;
TABLE[572][256] = 1;
TABLE[572][40] = 1;
TABLE[573][40] = 1;
TABLE[573][256] = 1;
TABLE[573][457] = 1;
TABLE[573][344] = 1;
TABLE[573][259] = 1;
TABLE[1084][259] = 1;
TABLE[1084][344] = 1;
TABLE[1084][457] = 1;
TABLE[1084][256] = 1;
TABLE[1084][318] = 1;
TABLE[1084][40] = 1;
TABLE[1084][287] = 1;
TABLE[1084][44] = 1;
TABLE[1084][59] = 1;
TABLE[574][40] = 1;
TABLE[574][256] = 1;
TABLE[574][457] = 1;
TABLE[574][344] = 1;
TABLE[574][259] = 1;
TABLE[1085][318] = 1;
TABLE[1085][40] = 1;
TABLE[1085][287] = 1;
TABLE[1085][44] = 1;
TABLE[1085][59] = 1;
TABLE[575][318] = 1;
TABLE[1086][40] = 1;
TABLE[1086][287] = 1;
TABLE[1086][44] = 1;
TABLE[1086][59] = 1;
TABLE[579][40] = 1;
TABLE[1087][287] = 1;
TABLE[1087][44] = 1;
TABLE[1087][59] = 1;
TABLE[576][321] = 1;
TABLE[577][306] = 1;
TABLE[578][373] = 1;
TABLE[587][287] = 1;
TABLE[1092][44] = 1;
TABLE[1092][59] = 1;
TABLE[580][259] = 1;
TABLE[580][344] = 1;
TABLE[580][457] = 1;
TABLE[580][256] = 1;
TABLE[580][40] = 1;
TABLE[1088][45] = 1;
TABLE[1088][43] = 1;
TABLE[1088][256] = 1;
TABLE[1088][457] = 1;
TABLE[1088][344] = 1;
TABLE[1088][259] = 1;
TABLE[1088][318] = 1;
TABLE[1088][40] = 1;
TABLE[1088][287] = 1;
TABLE[1088][44] = 1;
TABLE[1088][59] = 1;
TABLE[581][43] = 1;
TABLE[581][45] = 1;
TABLE[581][259] = 1;
TABLE[581][344] = 1;
TABLE[581][457] = 1;
TABLE[581][256] = 1;
TABLE[581][40] = 1;
TABLE[1089][318] = 1;
TABLE[1089][40] = 1;
TABLE[1089][287] = 1;
TABLE[1089][44] = 1;
TABLE[1089][59] = 1;
TABLE[582][318] = 1;
TABLE[1090][40] = 1;
TABLE[1090][287] = 1;
TABLE[1090][44] = 1;
TABLE[1090][59] = 1;
TABLE[586][40] = 1;
TABLE[1091][287] = 1;
TABLE[1091][44] = 1;
TABLE[1091][59] = 1;
TABLE[583][321] = 1;
TABLE[584][306] = 1;
TABLE[585][373] = 1;
TABLE[588][43] = 1;
TABLE[588][45] = 1;
TABLE[588][259] = 1;
TABLE[588][344] = 1;
TABLE[588][457] = 1;
TABLE[588][256] = 1;
TABLE[588][40] = 1;
TABLE[1093][44] = 1;
TABLE[1093][59] = 1;
TABLE[105][43] = 1;
TABLE[105][45] = 1;
TABLE[105][259] = 1;
TABLE[105][344] = 1;
TABLE[105][457] = 1;
TABLE[105][256] = 1;
TABLE[105][40] = 1;
TABLE[106][126] = 1;
TABLE[589][44] = 1;
TABLE[1094][126] = 1;
TABLE[1094][43] = 1;
TABLE[1094][45] = 1;
TABLE[1094][259] = 1;
TABLE[1094][344] = 1;
TABLE[1094][457] = 1;
TABLE[1094][256] = 1;
TABLE[1094][40] = 1;
TABLE[1094][59] = 1;
TABLE[590][44] = 1;
TABLE[1095][126] = 1;
TABLE[1095][43] = 1;
TABLE[1095][45] = 1;
TABLE[1095][259] = 1;
TABLE[1095][344] = 1;
TABLE[1095][457] = 1;
TABLE[1095][256] = 1;
TABLE[1095][40] = 1;
TABLE[1095][59] = 1;
TABLE[107][276] = 1;
TABLE[591][58] = 1;
TABLE[1096][43] = 1;
TABLE[1096][45] = 1;
TABLE[1096][259] = 1;
TABLE[1096][344] = 1;
TABLE[1096][457] = 1;
TABLE[1096][256] = 1;
TABLE[1096][40] = 1;
TABLE[108][371] = 1;
TABLE[592][325] = 1;
TABLE[592][373] = 1;
TABLE[592][429] = 1;
TABLE[592][372] = 1;
TABLE[592][315] = 1;
TABLE[592][451] = 1;
TABLE[592][283] = 1;
TABLE[592][434] = 1;
TABLE[592][403] = 1;
TABLE[592][419] = 1;
TABLE[592][382] = 1;
TABLE[592][314] = 1;
TABLE[592][465] = 1;
TABLE[592][430] = 1;
TABLE[592][395] = 1;
TABLE[592][322] = 1;
TABLE[592][311] = 1;
TABLE[592][406] = 1;
TABLE[592][291] = 1;
TABLE[592][364] = 1;
TABLE[592][436] = 1;
TABLE[592][348] = 1;
TABLE[592][349] = 1;
TABLE[592][360] = 1;
TABLE[592][303] = 1;
TABLE[592][467] = 1;
TABLE[592][466] = 1;
TABLE[592][378] = 1;
TABLE[592][455] = 1;
TABLE[592][312] = 1;
TABLE[592][375] = 1;
TABLE[592][374] = 1;
TABLE[592][354] = 1;
TABLE[592][399] = 1;
TABLE[592][367] = 1;
TABLE[592][356] = 1;
TABLE[592][433] = 1;
TABLE[592][376] = 1;
TABLE[592][310] = 1;
TABLE[592][264] = 1;
TABLE[592][267] = 1;
TABLE[592][263] = 1;
TABLE[592][271] = 1;
TABLE[592][272] = 1;
TABLE[592][273] = 1;
TABLE[592][473] = 1;
TABLE[1097][125] = 1;
TABLE[109][325] = 1;
TABLE[110][373] = 1;
TABLE[111][356] = 1;
TABLE[111][367] = 1;
TABLE[112][429] = 1;
TABLE[113][372] = 1;
TABLE[114][315] = 1;
TABLE[115][451] = 1;
TABLE[116][283] = 1;
TABLE[117][434] = 1;
TABLE[118][434] = 1;
TABLE[119][434] = 1;
TABLE[120][403] = 1;
TABLE[121][419] = 1;
TABLE[122][382] = 1;
TABLE[123][314] = 1;
TABLE[124][465] = 1;
TABLE[125][430] = 1;
TABLE[126][395] = 1;
TABLE[127][322] = 1;
TABLE[128][311] = 1;
TABLE[129][406] = 1;
TABLE[130][291] = 1;
TABLE[131][364] = 1;
TABLE[132][436] = 1;
TABLE[133][348] = 1;
TABLE[134][349] = 1;
TABLE[135][360] = 1;
TABLE[136][303] = 1;
TABLE[137][467] = 1;
TABLE[138][466] = 1;
TABLE[139][378] = 1;
TABLE[140][455] = 1;
TABLE[141][312] = 1;
TABLE[142][375] = 1;
TABLE[143][374] = 1;
TABLE[144][354] = 1;
TABLE[145][310] = 1;
TABLE[145][376] = 1;
TABLE[145][433] = 1;
TABLE[146][399] = 1;
TABLE[147][473] = 1;
TABLE[147][273] = 1;
TABLE[147][272] = 1;
TABLE[147][271] = 1;
TABLE[147][263] = 1;
TABLE[147][267] = 1;
TABLE[147][264] = 1;
TABLE[593][58] = 1;
TABLE[1098][257] = 1;
TABLE[594][58] = 1;
TABLE[1099][43] = 1;
TABLE[1099][45] = 1;
TABLE[1099][259] = 1;
TABLE[1099][344] = 1;
TABLE[1099][457] = 1;
TABLE[1099][256] = 1;
TABLE[1099][40] = 1;
TABLE[595][58] = 1;
TABLE[1100][259] = 1;
TABLE[1100][344] = 1;
TABLE[1100][457] = 1;
TABLE[1100][256] = 1;
TABLE[1100][40] = 1;
TABLE[1100][45] = 1;
TABLE[1100][43] = 1;
TABLE[596][58] = 1;
TABLE[1101][259] = 1;
TABLE[1101][344] = 1;
TABLE[1101][457] = 1;
TABLE[1101][256] = 1;
TABLE[1101][40] = 1;
TABLE[1101][45] = 1;
TABLE[1101][43] = 1;
TABLE[597][58] = 1;
TABLE[1102][43] = 1;
TABLE[1102][45] = 1;
TABLE[1102][259] = 1;
TABLE[1102][344] = 1;
TABLE[1102][457] = 1;
TABLE[1102][256] = 1;
TABLE[1102][40] = 1;
TABLE[598][58] = 1;
TABLE[1103][43] = 1;
TABLE[1103][45] = 1;
TABLE[1103][259] = 1;
TABLE[1103][344] = 1;
TABLE[1103][457] = 1;
TABLE[1103][256] = 1;
TABLE[1103][40] = 1;
TABLE[599][58] = 1;
TABLE[1104][43] = 1;
TABLE[1104][45] = 1;
TABLE[1104][259] = 1;
TABLE[1104][344] = 1;
TABLE[1104][457] = 1;
TABLE[1104][256] = 1;
TABLE[1104][40] = 1;
TABLE[600][58] = 1;
TABLE[1105][43] = 1;
TABLE[1105][45] = 1;
TABLE[1105][259] = 1;
TABLE[1105][344] = 1;
TABLE[1105][457] = 1;
TABLE[1105][256] = 1;
TABLE[1105][40] = 1;
TABLE[601][58] = 1;
TABLE[1106][40] = 1;
TABLE[1106][256] = 1;
TABLE[1106][457] = 1;
TABLE[1106][344] = 1;
TABLE[1106][259] = 1;
TABLE[602][58] = 1;
TABLE[1107][43] = 1;
TABLE[603][58] = 1;
TABLE[1108][43] = 1;
TABLE[1108][45] = 1;
TABLE[1108][259] = 1;
TABLE[1108][344] = 1;
TABLE[1108][457] = 1;
TABLE[1108][256] = 1;
TABLE[1108][40] = 1;
TABLE[604][58] = 1;
TABLE[1109][43] = 1;
TABLE[1109][45] = 1;
TABLE[1109][259] = 1;
TABLE[1109][344] = 1;
TABLE[1109][457] = 1;
TABLE[1109][256] = 1;
TABLE[1109][40] = 1;
TABLE[605][58] = 1;
TABLE[1110][43] = 1;
TABLE[1110][45] = 1;
TABLE[1110][259] = 1;
TABLE[1110][344] = 1;
TABLE[1110][457] = 1;
TABLE[1110][256] = 1;
TABLE[1110][40] = 1;
TABLE[606][58] = 1;
TABLE[1111][43] = 1;
TABLE[1111][45] = 1;
TABLE[1111][259] = 1;
TABLE[1111][344] = 1;
TABLE[1111][457] = 1;
TABLE[1111][256] = 1;
TABLE[1111][40] = 1;
TABLE[607][43] = 1;
TABLE[607][45] = 1;
TABLE[607][259] = 1;
TABLE[607][344] = 1;
TABLE[607][457] = 1;
TABLE[607][256] = 1;
TABLE[607][40] = 1;
TABLE[1112][59] = 1;
TABLE[608][58] = 1;
TABLE[1113][43] = 1;
TABLE[1113][45] = 1;
TABLE[1113][259] = 1;
TABLE[1113][344] = 1;
TABLE[1113][457] = 1;
TABLE[1113][256] = 1;
TABLE[1113][40] = 1;
TABLE[609][58] = 1;
TABLE[1114][43] = 1;
TABLE[1114][45] = 1;
TABLE[1114][259] = 1;
TABLE[1114][344] = 1;
TABLE[1114][457] = 1;
TABLE[1114][256] = 1;
TABLE[1114][40] = 1;
TABLE[610][58] = 1;
TABLE[1115][43] = 1;
TABLE[1115][45] = 1;
TABLE[1115][259] = 1;
TABLE[1115][344] = 1;
TABLE[1115][457] = 1;
TABLE[1115][256] = 1;
TABLE[1115][40] = 1;
TABLE[611][58] = 1;
TABLE[1116][43] = 1;
TABLE[1116][45] = 1;
TABLE[1116][259] = 1;
TABLE[1116][344] = 1;
TABLE[1116][457] = 1;
TABLE[1116][256] = 1;
TABLE[1116][40] = 1;
TABLE[612][58] = 1;
TABLE[1117][43] = 1;
TABLE[1117][45] = 1;
TABLE[1117][259] = 1;
TABLE[1117][344] = 1;
TABLE[1117][457] = 1;
TABLE[1117][256] = 1;
TABLE[1117][40] = 1;
TABLE[613][58] = 1;
TABLE[1118][43] = 1;
TABLE[1118][45] = 1;
TABLE[1118][259] = 1;
TABLE[1118][344] = 1;
TABLE[1118][457] = 1;
TABLE[1118][256] = 1;
TABLE[1118][40] = 1;
TABLE[614][58] = 1;
TABLE[1119][275] = 1;
TABLE[1119][43] = 1;
TABLE[1119][45] = 1;
TABLE[1119][259] = 1;
TABLE[1119][344] = 1;
TABLE[1119][457] = 1;
TABLE[1119][256] = 1;
TABLE[1119][40] = 1;
TABLE[1119][59] = 1;
TABLE[615][45] = 1;
TABLE[615][43] = 1;
TABLE[615][275] = 1;
TABLE[615][40] = 1;
TABLE[615][256] = 1;
TABLE[615][457] = 1;
TABLE[615][344] = 1;
TABLE[615][259] = 1;
TABLE[1120][59] = 1;
TABLE[616][58] = 1;
TABLE[1121][43] = 1;
TABLE[1121][45] = 1;
TABLE[1121][259] = 1;
TABLE[1121][344] = 1;
TABLE[1121][457] = 1;
TABLE[1121][256] = 1;
TABLE[1121][40] = 1;
TABLE[617][58] = 1;
TABLE[1122][36] = 1;
TABLE[1122][59] = 1;
TABLE[619][36] = 1;
TABLE[1124][59] = 1;
TABLE[618][44] = 1;
TABLE[1123][36] = 1;
TABLE[1123][59] = 1;
TABLE[620][58] = 1;
TABLE[1125][36] = 1;
TABLE[1125][59] = 1;
TABLE[622][36] = 1;
TABLE[1127][59] = 1;
TABLE[621][44] = 1;
TABLE[1126][36] = 1;
TABLE[1126][59] = 1;
TABLE[623][58] = 1;
TABLE[1128][126] = 1;
TABLE[1128][43] = 1;
TABLE[1128][45] = 1;
TABLE[1128][259] = 1;
TABLE[1128][344] = 1;
TABLE[1128][457] = 1;
TABLE[1128][256] = 1;
TABLE[1128][40] = 1;
TABLE[1128][59] = 1;
TABLE[624][45] = 1;
TABLE[624][43] = 1;
TABLE[624][126] = 1;
TABLE[624][40] = 1;
TABLE[624][256] = 1;
TABLE[624][457] = 1;
TABLE[624][344] = 1;
TABLE[624][259] = 1;
TABLE[1129][59] = 1;
TABLE[625][58] = 1;
TABLE[1130][45] = 1;
TABLE[1130][43] = 1;
TABLE[1130][40] = 1;
TABLE[1130][256] = 1;
TABLE[1130][457] = 1;
TABLE[1130][344] = 1;
TABLE[1130][259] = 1;
TABLE[1130][59] = 1;
TABLE[628][43] = 1;
TABLE[628][45] = 1;
TABLE[628][259] = 1;
TABLE[628][344] = 1;
TABLE[628][457] = 1;
TABLE[628][256] = 1;
TABLE[628][40] = 1;
TABLE[1132][59] = 1;
TABLE[626][44] = 1;
TABLE[627][124] = 1;
TABLE[1131][45] = 1;
TABLE[1131][43] = 1;
TABLE[1131][40] = 1;
TABLE[1131][256] = 1;
TABLE[1131][457] = 1;
TABLE[1131][344] = 1;
TABLE[1131][259] = 1;
TABLE[1131][59] = 1;
TABLE[629][58] = 1;
TABLE[1133][43] = 1;
TABLE[1133][45] = 1;
TABLE[1133][259] = 1;
TABLE[1133][344] = 1;
TABLE[1133][457] = 1;
TABLE[1133][256] = 1;
TABLE[1133][40] = 1;
TABLE[630][58] = 1;
TABLE[1134][43] = 1;
TABLE[1134][45] = 1;
TABLE[1134][259] = 1;
TABLE[1134][344] = 1;
TABLE[1134][457] = 1;
TABLE[1134][256] = 1;
TABLE[1134][40] = 1;
TABLE[631][58] = 1;
TABLE[1135][43] = 1;
TABLE[1135][45] = 1;
TABLE[1135][259] = 1;
TABLE[1135][344] = 1;
TABLE[1135][457] = 1;
TABLE[1135][256] = 1;
TABLE[1135][40] = 1;
TABLE[632][58] = 1;
TABLE[1136][43] = 1;
TABLE[1136][45] = 1;
TABLE[1136][259] = 1;
TABLE[1136][344] = 1;
TABLE[1136][457] = 1;
TABLE[1136][256] = 1;
TABLE[1136][40] = 1;
TABLE[633][58] = 1;
TABLE[1137][43] = 1;
TABLE[1137][45] = 1;
TABLE[1137][259] = 1;
TABLE[1137][344] = 1;
TABLE[1137][457] = 1;
TABLE[1137][256] = 1;
TABLE[1137][40] = 1;
TABLE[634][58] = 1;
TABLE[1138][43] = 1;
TABLE[1138][45] = 1;
TABLE[1138][259] = 1;
TABLE[1138][344] = 1;
TABLE[1138][457] = 1;
TABLE[1138][256] = 1;
TABLE[1138][40] = 1;
TABLE[635][58] = 1;
TABLE[1139][43] = 1;
TABLE[1139][45] = 1;
TABLE[1139][259] = 1;
TABLE[1139][344] = 1;
TABLE[1139][457] = 1;
TABLE[1139][256] = 1;
TABLE[1139][40] = 1;
TABLE[636][58] = 1;
TABLE[1140][43] = 1;
TABLE[1140][45] = 1;
TABLE[1140][259] = 1;
TABLE[1140][344] = 1;
TABLE[1140][457] = 1;
TABLE[1140][256] = 1;
TABLE[1140][40] = 1;
TABLE[637][58] = 1;
TABLE[1141][36] = 1;
TABLE[148][43] = 1;
TABLE[148][45] = 1;
TABLE[148][259] = 1;
TABLE[148][344] = 1;
TABLE[148][457] = 1;
TABLE[148][256] = 1;
TABLE[148][40] = 1;
TABLE[149][275] = 1;
TABLE[638][44] = 1;
TABLE[1142][275] = 1;
TABLE[1142][43] = 1;
TABLE[1142][45] = 1;
TABLE[1142][259] = 1;
TABLE[1142][344] = 1;
TABLE[1142][457] = 1;
TABLE[1142][256] = 1;
TABLE[1142][40] = 1;
TABLE[1142][59] = 1;
TABLE[639][44] = 1;
TABLE[1143][275] = 1;
TABLE[1143][43] = 1;
TABLE[1143][45] = 1;
TABLE[1143][259] = 1;
TABLE[1143][344] = 1;
TABLE[1143][457] = 1;
TABLE[1143][256] = 1;
TABLE[1143][40] = 1;
TABLE[1143][59] = 1;
TABLE[150][43] = 1;
TABLE[150][45] = 1;
TABLE[150][259] = 1;
TABLE[150][344] = 1;
TABLE[150][457] = 1;
TABLE[150][256] = 1;
TABLE[150][40] = 1;
TABLE[151][126] = 1;
TABLE[640][44] = 1;
TABLE[1144][126] = 1;
TABLE[1144][43] = 1;
TABLE[1144][45] = 1;
TABLE[1144][259] = 1;
TABLE[1144][344] = 1;
TABLE[1144][457] = 1;
TABLE[1144][256] = 1;
TABLE[1144][40] = 1;
TABLE[1144][59] = 1;
TABLE[641][44] = 1;
TABLE[1145][126] = 1;
TABLE[1145][43] = 1;
TABLE[1145][45] = 1;
TABLE[1145][259] = 1;
TABLE[1145][344] = 1;
TABLE[1145][457] = 1;
TABLE[1145][256] = 1;
TABLE[1145][40] = 1;
TABLE[1145][59] = 1;
TABLE[152][347] = 1;
TABLE[642][325] = 1;
TABLE[642][382] = 1;
TABLE[642][388] = 1;
TABLE[642][364] = 1;
TABLE[642][349] = 1;
TABLE[642][471] = 1;
TABLE[642][343] = 1;
TABLE[642][399] = 1;
TABLE[642][367] = 1;
TABLE[642][356] = 1;
TABLE[642][333] = 1;
TABLE[642][447] = 1;
TABLE[642][433] = 1;
TABLE[642][376] = 1;
TABLE[642][310] = 1;
TABLE[642][264] = 1;
TABLE[642][267] = 1;
TABLE[642][263] = 1;
TABLE[642][271] = 1;
TABLE[642][272] = 1;
TABLE[642][273] = 1;
TABLE[642][473] = 1;
TABLE[1146][125] = 1;
TABLE[153][325] = 1;
TABLE[154][382] = 1;
TABLE[155][356] = 1;
TABLE[155][367] = 1;
TABLE[156][388] = 1;
TABLE[157][364] = 1;
TABLE[158][349] = 1;
TABLE[159][447] = 1;
TABLE[159][333] = 1;
TABLE[160][471] = 1;
TABLE[161][343] = 1;
TABLE[162][399] = 1;
TABLE[163][310] = 1;
TABLE[163][376] = 1;
TABLE[163][433] = 1;
TABLE[164][473] = 1;
TABLE[164][273] = 1;
TABLE[164][272] = 1;
TABLE[164][271] = 1;
TABLE[164][263] = 1;
TABLE[164][267] = 1;
TABLE[164][264] = 1;
TABLE[643][58] = 1;
TABLE[1147][257] = 1;
TABLE[644][58] = 1;
TABLE[1148][43] = 1;
TABLE[1148][45] = 1;
TABLE[1148][259] = 1;
TABLE[1148][344] = 1;
TABLE[1148][457] = 1;
TABLE[1148][256] = 1;
TABLE[1148][40] = 1;
TABLE[645][58] = 1;
TABLE[1149][43] = 1;
TABLE[1149][45] = 1;
TABLE[1149][259] = 1;
TABLE[1149][344] = 1;
TABLE[1149][457] = 1;
TABLE[1149][256] = 1;
TABLE[1149][40] = 1;
TABLE[646][58] = 1;
TABLE[1150][43] = 1;
TABLE[1150][45] = 1;
TABLE[1150][259] = 1;
TABLE[1150][344] = 1;
TABLE[1150][457] = 1;
TABLE[1150][256] = 1;
TABLE[1150][40] = 1;
TABLE[647][58] = 1;
TABLE[1151][45] = 1;
TABLE[1151][43] = 1;
TABLE[1151][40] = 1;
TABLE[1151][256] = 1;
TABLE[1151][457] = 1;
TABLE[1151][344] = 1;
TABLE[1151][259] = 1;
TABLE[1151][59] = 1;
TABLE[650][43] = 1;
TABLE[650][45] = 1;
TABLE[650][259] = 1;
TABLE[650][344] = 1;
TABLE[650][457] = 1;
TABLE[650][256] = 1;
TABLE[650][40] = 1;
TABLE[1153][59] = 1;
TABLE[648][124] = 1;
TABLE[649][44] = 1;
TABLE[1152][45] = 1;
TABLE[1152][43] = 1;
TABLE[1152][40] = 1;
TABLE[1152][256] = 1;
TABLE[1152][457] = 1;
TABLE[1152][344] = 1;
TABLE[1152][259] = 1;
TABLE[1152][59] = 1;
TABLE[651][447] = 1;
TABLE[652][333] = 1;
TABLE[653][58] = 1;
TABLE[1154][36] = 1;
TABLE[654][58] = 1;
TABLE[1155][43] = 1;
TABLE[1155][45] = 1;
TABLE[1155][259] = 1;
TABLE[1155][344] = 1;
TABLE[1155][457] = 1;
TABLE[1155][256] = 1;
TABLE[1155][40] = 1;
TABLE[655][58] = 1;
TABLE[1156][259] = 1;
TABLE[1156][344] = 1;
TABLE[1156][457] = 1;
TABLE[1156][256] = 1;
TABLE[1156][40] = 1;
TABLE[1156][45] = 1;
TABLE[1156][43] = 1;
TABLE[165][435] = 1;
TABLE[165][333] = 1;
TABLE[165][408] = 1;
TABLE[165][329] = 1;
TABLE[165][45] = 1;
TABLE[165][43] = 1;
TABLE[165][40] = 1;
TABLE[165][256] = 1;
TABLE[165][457] = 1;
TABLE[165][344] = 1;
TABLE[165][259] = 1;
TABLE[657][43] = 1;
TABLE[657][45] = 1;
TABLE[657][259] = 1;
TABLE[657][344] = 1;
TABLE[657][457] = 1;
TABLE[657][256] = 1;
TABLE[657][40] = 1;
TABLE[1158][435] = 1;
TABLE[1158][333] = 1;
TABLE[1158][408] = 1;
TABLE[1158][329] = 1;
TABLE[656][47] = 1;
TABLE[1157][45] = 1;
TABLE[1157][43] = 1;
TABLE[1157][40] = 1;
TABLE[1157][256] = 1;
TABLE[1157][457] = 1;
TABLE[1157][344] = 1;
TABLE[1157][259] = 1;
TABLE[1157][435] = 1;
TABLE[1157][333] = 1;
TABLE[1157][408] = 1;
TABLE[1157][329] = 1;
TABLE[658][435] = 1;
TABLE[659][333] = 1;
TABLE[660][408] = 1;
TABLE[661][329] = 1;
TABLE[662][325] = 1;
TABLE[662][373] = 1;
TABLE[662][314] = 1;
TABLE[662][297] = 1;
TABLE[662][412] = 1;
TABLE[662][444] = 1;
TABLE[662][416] = 1;
TABLE[662][471] = 1;
TABLE[662][472] = 1;
TABLE[662][332] = 1;
TABLE[662][413] = 1;
TABLE[662][296] = 1;
TABLE[662][377] = 1;
TABLE[662][460] = 1;
TABLE[662][427] = 1;
TABLE[662][317] = 1;
TABLE[662][349] = 1;
TABLE[662][399] = 1;
TABLE[662][264] = 1;
TABLE[662][267] = 1;
TABLE[662][263] = 1;
TABLE[662][271] = 1;
TABLE[662][272] = 1;
TABLE[662][273] = 1;
TABLE[662][473] = 1;
TABLE[662][433] = 1;
TABLE[662][376] = 1;
TABLE[662][310] = 1;
TABLE[662][45] = 1;
TABLE[662][43] = 1;
TABLE[662][40] = 1;
TABLE[662][256] = 1;
TABLE[662][457] = 1;
TABLE[662][344] = 1;
TABLE[662][259] = 1;
TABLE[1159][125] = 1;
TABLE[664][287] = 1;
TABLE[1161][450] = 1;
TABLE[1161][349] = 1;
TABLE[1161][351] = 1;
TABLE[1161][432] = 1;
TABLE[1161][379] = 1;
TABLE[1161][473] = 1;
TABLE[1161][273] = 1;
TABLE[1161][272] = 1;
TABLE[1161][271] = 1;
TABLE[1161][263] = 1;
TABLE[1161][267] = 1;
TABLE[1161][264] = 1;
TABLE[1161][446] = 1;
TABLE[1161][335] = 1;
TABLE[1161][298] = 1;
TABLE[1161][340] = 1;
TABLE[1161][448] = 1;
TABLE[1161][449] = 1;
TABLE[1161][421] = 1;
TABLE[1161][327] = 1;
TABLE[1161][357] = 1;
TABLE[1161][308] = 1;
TABLE[1161][417] = 1;
TABLE[1161][261] = 1;
TABLE[1161][331] = 1;
TABLE[1161][292] = 1;
TABLE[1161][347] = 1;
TABLE[1161][371] = 1;
TABLE[1161][387] = 1;
TABLE[1161][43] = 1;
TABLE[1161][45] = 1;
TABLE[1161][329] = 1;
TABLE[1161][408] = 1;
TABLE[1161][333] = 1;
TABLE[1161][435] = 1;
TABLE[1161][259] = 1;
TABLE[1161][344] = 1;
TABLE[1161][457] = 1;
TABLE[1161][256] = 1;
TABLE[1161][40] = 1;
TABLE[1161][0] = 1;
TABLE[663][325] = 1;
TABLE[663][373] = 1;
TABLE[663][314] = 1;
TABLE[663][297] = 1;
TABLE[663][412] = 1;
TABLE[663][444] = 1;
TABLE[663][416] = 1;
TABLE[663][471] = 1;
TABLE[663][472] = 1;
TABLE[663][332] = 1;
TABLE[663][413] = 1;
TABLE[663][296] = 1;
TABLE[663][377] = 1;
TABLE[663][460] = 1;
TABLE[663][427] = 1;
TABLE[663][317] = 1;
TABLE[663][349] = 1;
TABLE[663][399] = 1;
TABLE[663][264] = 1;
TABLE[663][267] = 1;
TABLE[663][263] = 1;
TABLE[663][271] = 1;
TABLE[663][272] = 1;
TABLE[663][273] = 1;
TABLE[663][473] = 1;
TABLE[663][433] = 1;
TABLE[663][376] = 1;
TABLE[663][310] = 1;
TABLE[663][45] = 1;
TABLE[663][43] = 1;
TABLE[663][40] = 1;
TABLE[663][256] = 1;
TABLE[663][457] = 1;
TABLE[663][344] = 1;
TABLE[663][259] = 1;
TABLE[1160][125] = 1;
TABLE[166][325] = 1;
TABLE[167][43] = 1;
TABLE[167][45] = 1;
TABLE[167][259] = 1;
TABLE[167][344] = 1;
TABLE[167][457] = 1;
TABLE[167][256] = 1;
TABLE[167][40] = 1;
TABLE[168][373] = 1;
TABLE[169][314] = 1;
TABLE[170][297] = 1;
TABLE[171][412] = 1;
TABLE[172][444] = 1;
TABLE[173][416] = 1;
TABLE[174][471] = 1;
TABLE[175][472] = 1;
TABLE[176][332] = 1;
TABLE[177][413] = 1;
TABLE[178][296] = 1;
TABLE[179][377] = 1;
TABLE[180][460] = 1;
TABLE[181][427] = 1;
TABLE[182][427] = 1;
TABLE[183][317] = 1;
TABLE[184][317] = 1;
TABLE[185][349] = 1;
TABLE[186][399] = 1;
TABLE[187][473] = 1;
TABLE[187][273] = 1;
TABLE[187][272] = 1;
TABLE[187][271] = 1;
TABLE[187][263] = 1;
TABLE[187][267] = 1;
TABLE[187][264] = 1;
TABLE[188][310] = 1;
TABLE[188][376] = 1;
TABLE[188][433] = 1;
TABLE[665][58] = 1;
TABLE[1162][257] = 1;
TABLE[666][58] = 1;
TABLE[1163][43] = 1;
TABLE[1163][45] = 1;
TABLE[1163][259] = 1;
TABLE[1163][344] = 1;
TABLE[1163][457] = 1;
TABLE[1163][256] = 1;
TABLE[1163][40] = 1;
TABLE[667][58] = 1;
TABLE[1164][43] = 1;
TABLE[1164][45] = 1;
TABLE[1164][259] = 1;
TABLE[1164][344] = 1;
TABLE[1164][457] = 1;
TABLE[1164][256] = 1;
TABLE[1164][40] = 1;
TABLE[668][307] = 1;
TABLE[1165][58] = 1;
TABLE[1165][43] = 1;
TABLE[1165][45] = 1;
TABLE[1165][259] = 1;
TABLE[1165][344] = 1;
TABLE[1165][457] = 1;
TABLE[1165][256] = 1;
TABLE[1165][40] = 1;
TABLE[669][58] = 1;
TABLE[1166][43] = 1;
TABLE[1166][45] = 1;
TABLE[1166][259] = 1;
TABLE[1166][344] = 1;
TABLE[1166][457] = 1;
TABLE[1166][256] = 1;
TABLE[1166][40] = 1;
TABLE[670][37] = 1;
TABLE[1167][59] = 1;
TABLE[671][58] = 1;
TABLE[1168][43] = 1;
TABLE[1168][45] = 1;
TABLE[1168][259] = 1;
TABLE[1168][344] = 1;
TABLE[1168][457] = 1;
TABLE[1168][256] = 1;
TABLE[1168][40] = 1;
TABLE[672][40] = 1;
TABLE[1169][59] = 1;
TABLE[673][58] = 1;
TABLE[1170][43] = 1;
TABLE[1170][45] = 1;
TABLE[1170][259] = 1;
TABLE[1170][344] = 1;
TABLE[1170][457] = 1;
TABLE[1170][256] = 1;
TABLE[1170][40] = 1;
TABLE[674][404] = 1;
TABLE[1171][59] = 1;
TABLE[675][58] = 1;
TABLE[1172][43] = 1;
TABLE[1172][45] = 1;
TABLE[1172][259] = 1;
TABLE[1172][344] = 1;
TABLE[1172][457] = 1;
TABLE[1172][256] = 1;
TABLE[1172][40] = 1;
TABLE[676][58] = 1;
TABLE[1173][43] = 1;
TABLE[1173][45] = 1;
TABLE[1173][259] = 1;
TABLE[1173][344] = 1;
TABLE[1173][457] = 1;
TABLE[1173][256] = 1;
TABLE[1173][40] = 1;
TABLE[677][58] = 1;
TABLE[1174][43] = 1;
TABLE[1174][45] = 1;
TABLE[1174][259] = 1;
TABLE[1174][344] = 1;
TABLE[1174][457] = 1;
TABLE[1174][256] = 1;
TABLE[1174][40] = 1;
TABLE[678][58] = 1;
TABLE[1175][43] = 1;
TABLE[1175][45] = 1;
TABLE[1175][259] = 1;
TABLE[1175][344] = 1;
TABLE[1175][457] = 1;
TABLE[1175][256] = 1;
TABLE[1175][40] = 1;
TABLE[679][58] = 1;
TABLE[1176][259] = 1;
TABLE[1176][344] = 1;
TABLE[1176][457] = 1;
TABLE[1176][256] = 1;
TABLE[1176][40] = 1;
TABLE[1176][45] = 1;
TABLE[1176][43] = 1;
TABLE[680][58] = 1;
TABLE[1177][43] = 1;
TABLE[1177][45] = 1;
TABLE[1177][259] = 1;
TABLE[1177][344] = 1;
TABLE[1177][457] = 1;
TABLE[1177][256] = 1;
TABLE[1177][40] = 1;
TABLE[681][58] = 1;
TABLE[1178][43] = 1;
TABLE[1178][45] = 1;
TABLE[1178][259] = 1;
TABLE[1178][344] = 1;
TABLE[1178][457] = 1;
TABLE[1178][256] = 1;
TABLE[1178][40] = 1;
TABLE[682][58] = 1;
TABLE[1179][43] = 1;
TABLE[1179][45] = 1;
TABLE[1179][259] = 1;
TABLE[1179][344] = 1;
TABLE[1179][457] = 1;
TABLE[1179][256] = 1;
TABLE[1179][40] = 1;
TABLE[683][58] = 1;
TABLE[1180][36] = 1;
TABLE[684][58] = 1;
TABLE[1181][43] = 1;
TABLE[1181][45] = 1;
TABLE[1181][259] = 1;
TABLE[1181][344] = 1;
TABLE[1181][457] = 1;
TABLE[1181][256] = 1;
TABLE[1181][40] = 1;
TABLE[685][58] = 1;
TABLE[1182][43] = 1;
TABLE[1182][45] = 1;
TABLE[1182][259] = 1;
TABLE[1182][344] = 1;
TABLE[1182][457] = 1;
TABLE[1182][256] = 1;
TABLE[1182][40] = 1;
TABLE[686][58] = 1;
TABLE[1183][274] = 1;
TABLE[687][58] = 1;
TABLE[1184][45] = 1;
TABLE[1184][43] = 1;
TABLE[1184][40] = 1;
TABLE[1184][256] = 1;
TABLE[1184][457] = 1;
TABLE[1184][344] = 1;
TABLE[1184][259] = 1;
TABLE[1184][59] = 1;
TABLE[690][43] = 1;
TABLE[690][45] = 1;
TABLE[690][259] = 1;
TABLE[690][344] = 1;
TABLE[690][457] = 1;
TABLE[690][256] = 1;
TABLE[690][40] = 1;
TABLE[1186][59] = 1;
TABLE[688][44] = 1;
TABLE[689][124] = 1;
TABLE[1185][45] = 1;
TABLE[1185][43] = 1;
TABLE[1185][40] = 1;
TABLE[1185][256] = 1;
TABLE[1185][457] = 1;
TABLE[1185][344] = 1;
TABLE[1185][259] = 1;
TABLE[1185][59] = 1;
TABLE[189][45] = 1;
TABLE[189][43] = 1;
TABLE[189][40] = 1;
TABLE[189][256] = 1;
TABLE[189][457] = 1;
TABLE[189][344] = 1;
TABLE[189][259] = 1;
TABLE[693][43] = 1;
TABLE[693][45] = 1;
TABLE[693][259] = 1;
TABLE[693][344] = 1;
TABLE[693][457] = 1;
TABLE[693][256] = 1;
TABLE[693][40] = 1;
TABLE[691][44] = 1;
TABLE[692][124] = 1;
TABLE[1187][45] = 1;
TABLE[1187][43] = 1;
TABLE[1187][40] = 1;
TABLE[1187][256] = 1;
TABLE[1187][457] = 1;
TABLE[1187][344] = 1;
TABLE[1187][259] = 1;
TABLE[190][261] = 1;
TABLE[191][417] = 1;
TABLE[694][58] = 1;
TABLE[1189][123] = 1;
TABLE[695][125] = 1;
TABLE[695][325] = 1;
TABLE[695][59] = 1;
TABLE[695][358] = 1;
TABLE[695][387] = 1;
TABLE[695][339] = 1;
TABLE[695][431] = 1;
TABLE[695][45] = 1;
TABLE[695][43] = 1;
TABLE[695][349] = 1;
TABLE[695][355] = 1;
TABLE[695][399] = 1;
TABLE[695][276] = 1;
TABLE[695][40] = 1;
TABLE[695][256] = 1;
TABLE[695][457] = 1;
TABLE[695][344] = 1;
TABLE[695][259] = 1;
TABLE[695][433] = 1;
TABLE[695][376] = 1;
TABLE[695][310] = 1;
TABLE[695][264] = 1;
TABLE[695][267] = 1;
TABLE[695][263] = 1;
TABLE[695][271] = 1;
TABLE[695][272] = 1;
TABLE[695][273] = 1;
TABLE[695][473] = 1;
TABLE[1190][125] = 1;
TABLE[192][325] = 1;
TABLE[193][59] = 1;
TABLE[193][276] = 1;
TABLE[194][358] = 1;
TABLE[195][387] = 1;
TABLE[196][339] = 1;
TABLE[197][339] = 1;
TABLE[198][431] = 1;
TABLE[199][43] = 1;
TABLE[199][45] = 1;
TABLE[199][259] = 1;
TABLE[199][344] = 1;
TABLE[199][457] = 1;
TABLE[199][256] = 1;
TABLE[199][40] = 1;
TABLE[200][349] = 1;
TABLE[201][355] = 1;
TABLE[202][310] = 1;
TABLE[202][376] = 1;
TABLE[202][433] = 1;
TABLE[203][399] = 1;
TABLE[204][473] = 1;
TABLE[204][273] = 1;
TABLE[204][272] = 1;
TABLE[204][271] = 1;
TABLE[204][263] = 1;
TABLE[204][267] = 1;
TABLE[204][264] = 1;
TABLE[696][58] = 1;
TABLE[1191][257] = 1;
TABLE[699][276] = 1;
TABLE[1194][59] = 1;
TABLE[697][58] = 1;
TABLE[1192][43] = 1;
TABLE[1192][45] = 1;
TABLE[1192][259] = 1;
TABLE[1192][344] = 1;
TABLE[1192][457] = 1;
TABLE[1192][256] = 1;
TABLE[1192][40] = 1;
TABLE[698][44] = 1;
TABLE[1193][276] = 1;
TABLE[1193][59] = 1;
TABLE[700][58] = 1;
TABLE[1195][346] = 1;
TABLE[1195][439] = 1;
TABLE[1195][281] = 1;
TABLE[1195][59] = 1;
TABLE[702][281] = 1;
TABLE[702][439] = 1;
TABLE[702][346] = 1;
TABLE[1197][59] = 1;
TABLE[701][44] = 1;
TABLE[1196][346] = 1;
TABLE[1196][439] = 1;
TABLE[1196][281] = 1;
TABLE[1196][59] = 1;
TABLE[703][58] = 1;
TABLE[1198][36] = 1;
TABLE[704][308] = 1;
TABLE[1199][58] = 1;
TABLE[1199][36] = 1;
TABLE[705][58] = 1;
TABLE[1200][36] = 1;
TABLE[706][308] = 1;
TABLE[1201][58] = 1;
TABLE[1201][36] = 1;
TABLE[707][58] = 1;
TABLE[1202][36] = 1;
TABLE[708][58] = 1;
TABLE[1203][45] = 1;
TABLE[1203][43] = 1;
TABLE[1203][40] = 1;
TABLE[1203][256] = 1;
TABLE[1203][457] = 1;
TABLE[1203][344] = 1;
TABLE[1203][259] = 1;
TABLE[1203][59] = 1;
TABLE[710][43] = 1;
TABLE[710][45] = 1;
TABLE[710][259] = 1;
TABLE[710][344] = 1;
TABLE[710][457] = 1;
TABLE[710][256] = 1;
TABLE[710][40] = 1;
TABLE[1205][59] = 1;
TABLE[709][44] = 1;
TABLE[1204][45] = 1;
TABLE[1204][43] = 1;
TABLE[1204][40] = 1;
TABLE[1204][256] = 1;
TABLE[1204][457] = 1;
TABLE[1204][344] = 1;
TABLE[1204][259] = 1;
TABLE[1204][59] = 1;
TABLE[711][58] = 1;
TABLE[1206][257] = 1;
TABLE[712][58] = 1;
TABLE[1207][45] = 1;
TABLE[1207][43] = 1;
TABLE[1207][40] = 1;
TABLE[1207][256] = 1;
TABLE[1207][457] = 1;
TABLE[1207][344] = 1;
TABLE[1207][259] = 1;
TABLE[1207][59] = 1;
TABLE[715][43] = 1;
TABLE[715][45] = 1;
TABLE[715][259] = 1;
TABLE[715][344] = 1;
TABLE[715][457] = 1;
TABLE[715][256] = 1;
TABLE[715][40] = 1;
TABLE[1209][59] = 1;
TABLE[713][124] = 1;
TABLE[714][44] = 1;
TABLE[1208][45] = 1;
TABLE[1208][43] = 1;
TABLE[1208][40] = 1;
TABLE[1208][256] = 1;
TABLE[1208][457] = 1;
TABLE[1208][344] = 1;
TABLE[1208][259] = 1;
TABLE[1208][59] = 1;
TABLE[205][308] = 1;
TABLE[716][325] = 1;
TABLE[716][363] = 1;
TABLE[716][380] = 1;
TABLE[716][322] = 1;
TABLE[716][295] = 1;
TABLE[716][428] = 1;
TABLE[716][358] = 1;
TABLE[716][431] = 1;
TABLE[716][410] = 1;
TABLE[716][349] = 1;
TABLE[716][355] = 1;
TABLE[716][399] = 1;
TABLE[716][433] = 1;
TABLE[716][376] = 1;
TABLE[716][310] = 1;
TABLE[716][264] = 1;
TABLE[716][267] = 1;
TABLE[716][263] = 1;
TABLE[716][271] = 1;
TABLE[716][272] = 1;
TABLE[716][273] = 1;
TABLE[716][473] = 1;
TABLE[1210][125] = 1;
TABLE[206][325] = 1;
TABLE[207][363] = 1;
TABLE[208][380] = 1;
TABLE[209][322] = 1;
TABLE[210][295] = 1;
TABLE[211][428] = 1;
TABLE[212][358] = 1;
TABLE[213][431] = 1;
TABLE[214][410] = 1;
TABLE[215][349] = 1;
TABLE[216][355] = 1;
TABLE[217][399] = 1;
TABLE[218][310] = 1;
TABLE[218][376] = 1;
TABLE[218][433] = 1;
TABLE[219][473] = 1;
TABLE[219][273] = 1;
TABLE[219][272] = 1;
TABLE[219][271] = 1;
TABLE[219][263] = 1;
TABLE[219][267] = 1;
TABLE[219][264] = 1;
TABLE[717][58] = 1;
TABLE[1211][257] = 1;
TABLE[718][58] = 1;
TABLE[1212][100] = 1;
TABLE[1212][43] = 1;
TABLE[1212][45] = 1;
TABLE[1212][259] = 1;
TABLE[1212][344] = 1;
TABLE[1212][457] = 1;
TABLE[1212][256] = 1;
TABLE[1212][40] = 1;
TABLE[719][100] = 1;
TABLE[1213][43] = 1;
TABLE[1213][45] = 1;
TABLE[1213][259] = 1;
TABLE[1213][344] = 1;
TABLE[1213][457] = 1;
TABLE[1213][256] = 1;
TABLE[1213][40] = 1;
TABLE[720][58] = 1;
TABLE[1214][100] = 1;
TABLE[1214][43] = 1;
TABLE[1214][45] = 1;
TABLE[1214][259] = 1;
TABLE[1214][344] = 1;
TABLE[1214][457] = 1;
TABLE[1214][256] = 1;
TABLE[1214][40] = 1;
TABLE[721][100] = 1;
TABLE[1215][43] = 1;
TABLE[1215][45] = 1;
TABLE[1215][259] = 1;
TABLE[1215][344] = 1;
TABLE[1215][457] = 1;
TABLE[1215][256] = 1;
TABLE[1215][40] = 1;
TABLE[722][58] = 1;
TABLE[1216][43] = 1;
TABLE[1216][45] = 1;
TABLE[1216][259] = 1;
TABLE[1216][344] = 1;
TABLE[1216][457] = 1;
TABLE[1216][256] = 1;
TABLE[1216][40] = 1;
TABLE[723][58] = 1;
TABLE[1217][45] = 1;
TABLE[1217][43] = 1;
TABLE[1217][40] = 1;
TABLE[1217][256] = 1;
TABLE[1217][457] = 1;
TABLE[1217][344] = 1;
TABLE[1217][259] = 1;
TABLE[1217][59] = 1;
TABLE[725][43] = 1;
TABLE[725][45] = 1;
TABLE[725][259] = 1;
TABLE[725][344] = 1;
TABLE[725][457] = 1;
TABLE[725][256] = 1;
TABLE[725][40] = 1;
TABLE[1219][59] = 1;
TABLE[724][44] = 1;
TABLE[1218][45] = 1;
TABLE[1218][43] = 1;
TABLE[1218][40] = 1;
TABLE[1218][256] = 1;
TABLE[1218][457] = 1;
TABLE[1218][344] = 1;
TABLE[1218][259] = 1;
TABLE[1218][59] = 1;
TABLE[726][58] = 1;
TABLE[1220][45] = 1;
TABLE[1220][43] = 1;
TABLE[1220][40] = 1;
TABLE[1220][256] = 1;
TABLE[1220][457] = 1;
TABLE[1220][344] = 1;
TABLE[1220][259] = 1;
TABLE[1220][59] = 1;
TABLE[728][43] = 1;
TABLE[728][45] = 1;
TABLE[728][259] = 1;
TABLE[728][344] = 1;
TABLE[728][457] = 1;
TABLE[728][256] = 1;
TABLE[728][40] = 1;
TABLE[1222][59] = 1;
TABLE[727][44] = 1;
TABLE[1221][45] = 1;
TABLE[1221][43] = 1;
TABLE[1221][40] = 1;
TABLE[1221][256] = 1;
TABLE[1221][457] = 1;
TABLE[1221][344] = 1;
TABLE[1221][259] = 1;
TABLE[1221][59] = 1;
TABLE[729][58] = 1;
TABLE[1223][346] = 1;
TABLE[1223][439] = 1;
TABLE[1223][281] = 1;
TABLE[1223][59] = 1;
TABLE[731][281] = 1;
TABLE[731][439] = 1;
TABLE[731][346] = 1;
TABLE[1225][59] = 1;
TABLE[730][44] = 1;
TABLE[1224][346] = 1;
TABLE[1224][439] = 1;
TABLE[1224][281] = 1;
TABLE[1224][59] = 1;
TABLE[732][91] = 1;
TABLE[733][93] = 1;
TABLE[734][58] = 1;
TABLE[1226][45] = 1;
TABLE[1226][43] = 1;
TABLE[1226][40] = 1;
TABLE[1226][256] = 1;
TABLE[1226][457] = 1;
TABLE[1226][344] = 1;
TABLE[1226][259] = 1;
TABLE[1226][59] = 1;
TABLE[736][43] = 1;
TABLE[736][45] = 1;
TABLE[736][259] = 1;
TABLE[736][344] = 1;
TABLE[736][457] = 1;
TABLE[736][256] = 1;
TABLE[736][40] = 1;
TABLE[1228][59] = 1;
TABLE[735][44] = 1;
TABLE[1227][45] = 1;
TABLE[1227][43] = 1;
TABLE[1227][40] = 1;
TABLE[1227][256] = 1;
TABLE[1227][457] = 1;
TABLE[1227][344] = 1;
TABLE[1227][259] = 1;
TABLE[1227][59] = 1;
TABLE[737][58] = 1;
TABLE[1229][45] = 1;
TABLE[1229][43] = 1;
TABLE[1229][40] = 1;
TABLE[1229][256] = 1;
TABLE[1229][457] = 1;
TABLE[1229][344] = 1;
TABLE[1229][259] = 1;
TABLE[1229][59] = 1;
TABLE[739][43] = 1;
TABLE[739][45] = 1;
TABLE[739][259] = 1;
TABLE[739][344] = 1;
TABLE[739][457] = 1;
TABLE[739][256] = 1;
TABLE[739][40] = 1;
TABLE[1231][59] = 1;
TABLE[738][44] = 1;
TABLE[1230][45] = 1;
TABLE[1230][43] = 1;
TABLE[1230][40] = 1;
TABLE[1230][256] = 1;
TABLE[1230][457] = 1;
TABLE[1230][344] = 1;
TABLE[1230][259] = 1;
TABLE[1230][59] = 1;
TABLE[740][58] = 1;
TABLE[1232][45] = 1;
TABLE[1232][43] = 1;
TABLE[1232][40] = 1;
TABLE[1232][256] = 1;
TABLE[1232][457] = 1;
TABLE[1232][344] = 1;
TABLE[1232][259] = 1;
TABLE[1232][59] = 1;
TABLE[743][43] = 1;
TABLE[743][45] = 1;
TABLE[743][259] = 1;
TABLE[743][344] = 1;
TABLE[743][457] = 1;
TABLE[743][256] = 1;
TABLE[743][40] = 1;
TABLE[1234][59] = 1;
TABLE[741][124] = 1;
TABLE[742][44] = 1;
TABLE[1233][45] = 1;
TABLE[1233][43] = 1;
TABLE[1233][40] = 1;
TABLE[1233][256] = 1;
TABLE[1233][457] = 1;
TABLE[1233][344] = 1;
TABLE[1233][259] = 1;
TABLE[1233][59] = 1;
TABLE[220][281] = 1;
TABLE[221][439] = 1;
TABLE[222][346] = 1;
TABLE[744][44] = 1;
TABLE[1235][93] = 1;
TABLE[745][44] = 1;
TABLE[1236][93] = 1;
TABLE[746][44] = 1;
TABLE[1237][93] = 1;
TABLE[223][44] = 1;
TABLE[224][44] = 1;
TABLE[225][44] = 1;
TABLE[226][43] = 1;
TABLE[226][45] = 1;
TABLE[226][259] = 1;
TABLE[226][344] = 1;
TABLE[226][457] = 1;
TABLE[226][256] = 1;
TABLE[226][40] = 1;
TABLE[227][342] = 1;
TABLE[228][342] = 1;
TABLE[229][342] = 1;
TABLE[230][342] = 1;
TABLE[747][373] = 1;
TABLE[1238][44] = 1;
TABLE[1238][346] = 1;
TABLE[1238][439] = 1;
TABLE[1238][281] = 1;
TABLE[1238][59] = 1;
TABLE[748][373] = 1;
TABLE[1239][44] = 1;
TABLE[1239][346] = 1;
TABLE[1239][439] = 1;
TABLE[1239][281] = 1;
TABLE[1239][59] = 1;
TABLE[749][373] = 1;
TABLE[1240][44] = 1;
TABLE[1240][346] = 1;
TABLE[1240][439] = 1;
TABLE[1240][281] = 1;
TABLE[1240][59] = 1;
TABLE[231][331] = 1;
TABLE[750][325] = 1;
TABLE[750][399] = 1;
TABLE[750][433] = 1;
TABLE[750][376] = 1;
TABLE[750][310] = 1;
TABLE[750][264] = 1;
TABLE[750][267] = 1;
TABLE[750][263] = 1;
TABLE[750][271] = 1;
TABLE[750][272] = 1;
TABLE[750][273] = 1;
TABLE[750][473] = 1;
TABLE[1241][125] = 1;
TABLE[232][325] = 1;
TABLE[233][310] = 1;
TABLE[233][376] = 1;
TABLE[233][433] = 1;
TABLE[234][399] = 1;
TABLE[235][473] = 1;
TABLE[235][273] = 1;
TABLE[235][272] = 1;
TABLE[235][271] = 1;
TABLE[235][263] = 1;
TABLE[235][267] = 1;
TABLE[235][264] = 1;
TABLE[751][58] = 1;
TABLE[1242][257] = 1;
TABLE[236][310] = 1;
TABLE[237][376] = 1;
TABLE[238][433] = 1;
TABLE[752][58] = 1;
TABLE[1243][42] = 1;
TABLE[1243][59] = 1;
TABLE[754][42] = 1;
TABLE[756][42] = 1;
TABLE[1246][59] = 1;
TABLE[753][44] = 1;
TABLE[1244][42] = 1;
TABLE[1244][59] = 1;
TABLE[755][44] = 1;
TABLE[1245][42] = 1;
TABLE[1245][59] = 1;
TABLE[757][58] = 1;
TABLE[1247][42] = 1;
TABLE[1247][399] = 1;
TABLE[1247][355] = 1;
TABLE[1247][408] = 1;
TABLE[1247][329] = 1;
TABLE[1247][436] = 1;
TABLE[1247][388] = 1;
TABLE[1247][434] = 1;
TABLE[1247][430] = 1;
TABLE[1247][349] = 1;
TABLE[1247][345] = 1;
TABLE[1247][380] = 1;
TABLE[1247][291] = 1;
TABLE[1247][322] = 1;
TABLE[1247][293] = 1;
TABLE[1247][295] = 1;
TABLE[1247][368] = 1;
TABLE[1247][423] = 1;
TABLE[1247][439] = 1;
TABLE[1247][365] = 1;
TABLE[1247][364] = 1;
TABLE[1247][363] = 1;
TABLE[1247][449] = 1;
TABLE[1247][324] = 1;
TABLE[1247][325] = 1;
TABLE[1247][473] = 1;
TABLE[1247][273] = 1;
TABLE[1247][272] = 1;
TABLE[1247][271] = 1;
TABLE[1247][263] = 1;
TABLE[1247][267] = 1;
TABLE[1247][264] = 1;
TABLE[1247][310] = 1;
TABLE[1247][376] = 1;
TABLE[1247][433] = 1;
TABLE[1247][276] = 1;
TABLE[1247][356] = 1;
TABLE[1247][367] = 1;
TABLE[1247][43] = 1;
TABLE[1247][373] = 1;
TABLE[1247][306] = 1;
TABLE[1247][125] = 1;
TABLE[1247][354] = 1;
TABLE[1247][374] = 1;
TABLE[1247][375] = 1;
TABLE[1247][312] = 1;
TABLE[1247][455] = 1;
TABLE[1247][378] = 1;
TABLE[1247][466] = 1;
TABLE[1247][467] = 1;
TABLE[1247][303] = 1;
TABLE[1247][360] = 1;
TABLE[1247][348] = 1;
TABLE[1247][406] = 1;
TABLE[1247][311] = 1;
TABLE[1247][395] = 1;
TABLE[1247][465] = 1;
TABLE[1247][314] = 1;
TABLE[1247][382] = 1;
TABLE[1247][419] = 1;
TABLE[1247][403] = 1;
TABLE[1247][283] = 1;
TABLE[1247][451] = 1;
TABLE[1247][315] = 1;
TABLE[1247][372] = 1;
TABLE[1247][429] = 1;
TABLE[1247][343] = 1;
TABLE[1247][471] = 1;
TABLE[1247][447] = 1;
TABLE[1247][333] = 1;
TABLE[1247][317] = 1;
TABLE[1247][427] = 1;
TABLE[1247][460] = 1;
TABLE[1247][377] = 1;
TABLE[1247][296] = 1;
TABLE[1247][413] = 1;
TABLE[1247][332] = 1;
TABLE[1247][472] = 1;
TABLE[1247][416] = 1;
TABLE[1247][444] = 1;
TABLE[1247][412] = 1;
TABLE[1247][297] = 1;
TABLE[1247][45] = 1;
TABLE[1247][259] = 1;
TABLE[1247][344] = 1;
TABLE[1247][457] = 1;
TABLE[1247][256] = 1;
TABLE[1247][40] = 1;
TABLE[1247][431] = 1;
TABLE[1247][339] = 1;
TABLE[1247][387] = 1;
TABLE[1247][358] = 1;
TABLE[1247][59] = 1;
TABLE[1247][410] = 1;
TABLE[1247][428] = 1;
TABLE[1247][328] = 1;
TABLE[1247][397] = 1;
TABLE[1247][422] = 1;
TABLE[1247][454] = 1;
TABLE[1247][359] = 1;
TABLE[1247][425] = 1;
TABLE[1247][326] = 1;
TABLE[1247][350] = 1;
TABLE[1247][463] = 1;
TABLE[1247][305] = 1;
TABLE[1247][409] = 1;
TABLE[1247][318] = 1;
TABLE[1247][389] = 1;
TABLE[1247][352] = 1;
TABLE[1247][405] = 1;
TABLE[1247][284] = 1;
TABLE[1247][435] = 1;
TABLE[761][42] = 1;
TABLE[1249][399] = 1;
TABLE[1249][355] = 1;
TABLE[1249][408] = 1;
TABLE[1249][329] = 1;
TABLE[1249][436] = 1;
TABLE[1249][388] = 1;
TABLE[1249][434] = 1;
TABLE[1249][430] = 1;
TABLE[1249][349] = 1;
TABLE[1249][345] = 1;
TABLE[1249][380] = 1;
TABLE[1249][291] = 1;
TABLE[1249][322] = 1;
TABLE[1249][293] = 1;
TABLE[1249][295] = 1;
TABLE[1249][368] = 1;
TABLE[1249][423] = 1;
TABLE[1249][439] = 1;
TABLE[1249][365] = 1;
TABLE[1249][364] = 1;
TABLE[1249][363] = 1;
TABLE[1249][449] = 1;
TABLE[1249][324] = 1;
TABLE[1249][325] = 1;
TABLE[1249][473] = 1;
TABLE[1249][273] = 1;
TABLE[1249][272] = 1;
TABLE[1249][271] = 1;
TABLE[1249][263] = 1;
TABLE[1249][267] = 1;
TABLE[1249][264] = 1;
TABLE[1249][310] = 1;
TABLE[1249][376] = 1;
TABLE[1249][433] = 1;
TABLE[1249][276] = 1;
TABLE[1249][356] = 1;
TABLE[1249][367] = 1;
TABLE[1249][43] = 1;
TABLE[1249][373] = 1;
TABLE[1249][306] = 1;
TABLE[1249][125] = 1;
TABLE[1249][354] = 1;
TABLE[1249][374] = 1;
TABLE[1249][375] = 1;
TABLE[1249][312] = 1;
TABLE[1249][455] = 1;
TABLE[1249][378] = 1;
TABLE[1249][466] = 1;
TABLE[1249][467] = 1;
TABLE[1249][303] = 1;
TABLE[1249][360] = 1;
TABLE[1249][348] = 1;
TABLE[1249][406] = 1;
TABLE[1249][311] = 1;
TABLE[1249][395] = 1;
TABLE[1249][465] = 1;
TABLE[1249][314] = 1;
TABLE[1249][382] = 1;
TABLE[1249][419] = 1;
TABLE[1249][403] = 1;
TABLE[1249][283] = 1;
TABLE[1249][451] = 1;
TABLE[1249][315] = 1;
TABLE[1249][372] = 1;
TABLE[1249][429] = 1;
TABLE[1249][343] = 1;
TABLE[1249][471] = 1;
TABLE[1249][447] = 1;
TABLE[1249][333] = 1;
TABLE[1249][317] = 1;
TABLE[1249][427] = 1;
TABLE[1249][460] = 1;
TABLE[1249][377] = 1;
TABLE[1249][296] = 1;
TABLE[1249][413] = 1;
TABLE[1249][332] = 1;
TABLE[1249][472] = 1;
TABLE[1249][416] = 1;
TABLE[1249][444] = 1;
TABLE[1249][412] = 1;
TABLE[1249][297] = 1;
TABLE[1249][45] = 1;
TABLE[1249][259] = 1;
TABLE[1249][344] = 1;
TABLE[1249][457] = 1;
TABLE[1249][256] = 1;
TABLE[1249][40] = 1;
TABLE[1249][431] = 1;
TABLE[1249][339] = 1;
TABLE[1249][387] = 1;
TABLE[1249][358] = 1;
TABLE[1249][59] = 1;
TABLE[1249][410] = 1;
TABLE[1249][428] = 1;
TABLE[1249][328] = 1;
TABLE[1249][397] = 1;
TABLE[1249][422] = 1;
TABLE[1249][454] = 1;
TABLE[1249][359] = 1;
TABLE[1249][425] = 1;
TABLE[1249][326] = 1;
TABLE[1249][350] = 1;
TABLE[1249][463] = 1;
TABLE[1249][305] = 1;
TABLE[1249][409] = 1;
TABLE[1249][318] = 1;
TABLE[1249][389] = 1;
TABLE[1249][352] = 1;
TABLE[1249][405] = 1;
TABLE[1249][284] = 1;
TABLE[1249][435] = 1;
TABLE[758][36] = 1;
TABLE[758][259] = 1;
TABLE[758][344] = 1;
TABLE[758][457] = 1;
TABLE[758][256] = 1;
TABLE[758][40] = 1;
TABLE[758][257] = 1;
TABLE[1248][44] = 1;
TABLE[1248][59] = 1;
TABLE[759][44] = 1;
TABLE[760][59] = 1;
TABLE[762][58] = 1;
TABLE[1250][42] = 1;
TABLE[1250][59] = 1;
TABLE[768][42] = 1;
TABLE[1253][59] = 1;
TABLE[763][40] = 1;
TABLE[763][256] = 1;
TABLE[763][457] = 1;
TABLE[763][344] = 1;
TABLE[763][259] = 1;
TABLE[1251][36] = 1;
TABLE[764][36] = 1;
TABLE[765][36] = 1;
TABLE[766][36] = 1;
TABLE[767][44] = 1;
TABLE[1252][42] = 1;
TABLE[1252][59] = 1;
TABLE[239][36] = 1;
TABLE[240][40] = 1;
TABLE[240][256] = 1;
TABLE[240][457] = 1;
TABLE[240][344] = 1;
TABLE[240][259] = 1;
TABLE[241][257] = 1;
TABLE[242][292] = 1;
TABLE[769][125] = 1;
TABLE[769][325] = 1;
TABLE[769][371] = 1;
TABLE[769][59] = 1;
TABLE[769][423] = 1;
TABLE[769][443] = 1;
TABLE[769][414] = 1;
TABLE[769][337] = 1;
TABLE[769][469] = 1;
TABLE[769][365] = 1;
TABLE[769][370] = 1;
TABLE[769][399] = 1;
TABLE[769][276] = 1;
TABLE[769][264] = 1;
TABLE[769][267] = 1;
TABLE[769][263] = 1;
TABLE[769][271] = 1;
TABLE[769][272] = 1;
TABLE[769][273] = 1;
TABLE[769][473] = 1;
TABLE[1254][125] = 1;
TABLE[243][325] = 1;
TABLE[244][371] = 1;
TABLE[245][59] = 1;
TABLE[245][276] = 1;
TABLE[246][423] = 1;
TABLE[247][443] = 1;
TABLE[248][414] = 1;
TABLE[249][337] = 1;
TABLE[250][469] = 1;
TABLE[251][365] = 1;
TABLE[252][370] = 1;
TABLE[253][399] = 1;
TABLE[254][473] = 1;
TABLE[254][273] = 1;
TABLE[254][272] = 1;
TABLE[254][271] = 1;
TABLE[254][263] = 1;
TABLE[254][267] = 1;
TABLE[254][264] = 1;
TABLE[770][58] = 1;
TABLE[1255][257] = 1;
TABLE[771][58] = 1;
TABLE[1256][36] = 1;
TABLE[774][276] = 1;
TABLE[1259][59] = 1;
TABLE[772][58] = 1;
TABLE[1257][43] = 1;
TABLE[1257][45] = 1;
TABLE[1257][259] = 1;
TABLE[1257][344] = 1;
TABLE[1257][457] = 1;
TABLE[1257][256] = 1;
TABLE[1257][40] = 1;
TABLE[773][44] = 1;
TABLE[1258][276] = 1;
TABLE[1258][59] = 1;
TABLE[775][58] = 1;
TABLE[1260][45] = 1;
TABLE[1260][43] = 1;
TABLE[1260][40] = 1;
TABLE[1260][256] = 1;
TABLE[1260][457] = 1;
TABLE[1260][344] = 1;
TABLE[1260][259] = 1;
TABLE[1260][59] = 1;
TABLE[777][43] = 1;
TABLE[777][45] = 1;
TABLE[777][259] = 1;
TABLE[777][344] = 1;
TABLE[777][457] = 1;
TABLE[777][256] = 1;
TABLE[777][40] = 1;
TABLE[1262][59] = 1;
TABLE[776][44] = 1;
TABLE[1261][45] = 1;
TABLE[1261][43] = 1;
TABLE[1261][40] = 1;
TABLE[1261][256] = 1;
TABLE[1261][457] = 1;
TABLE[1261][344] = 1;
TABLE[1261][259] = 1;
TABLE[1261][59] = 1;
TABLE[778][58] = 1;
TABLE[1263][276] = 1;
TABLE[1263][59] = 1;
TABLE[780][276] = 1;
TABLE[1265][59] = 1;
TABLE[779][44] = 1;
TABLE[1264][276] = 1;
TABLE[1264][59] = 1;
TABLE[781][58] = 1;
TABLE[1266][45] = 1;
TABLE[1266][43] = 1;
TABLE[1266][40] = 1;
TABLE[1266][256] = 1;
TABLE[1266][457] = 1;
TABLE[1266][344] = 1;
TABLE[1266][259] = 1;
TABLE[1266][59] = 1;
TABLE[783][43] = 1;
TABLE[783][45] = 1;
TABLE[783][259] = 1;
TABLE[783][344] = 1;
TABLE[783][457] = 1;
TABLE[783][256] = 1;
TABLE[783][40] = 1;
TABLE[1268][59] = 1;
TABLE[782][44] = 1;
TABLE[1267][45] = 1;
TABLE[1267][43] = 1;
TABLE[1267][40] = 1;
TABLE[1267][256] = 1;
TABLE[1267][457] = 1;
TABLE[1267][344] = 1;
TABLE[1267][259] = 1;
TABLE[1267][59] = 1;
TABLE[785][439] = 1;
TABLE[785][36] = 1;
TABLE[785][369] = 1;
TABLE[785][346] = 1;
TABLE[1269][59] = 1;
TABLE[784][44] = 1;
TABLE[787][439] = 1;
TABLE[787][36] = 1;
TABLE[787][369] = 1;
TABLE[787][346] = 1;
TABLE[1270][59] = 1;
TABLE[786][44] = 1;
TABLE[789][439] = 1;
TABLE[789][36] = 1;
TABLE[789][369] = 1;
TABLE[789][346] = 1;
TABLE[1271][59] = 1;
TABLE[788][44] = 1;
TABLE[791][439] = 1;
TABLE[791][36] = 1;
TABLE[791][369] = 1;
TABLE[791][346] = 1;
TABLE[1272][59] = 1;
TABLE[790][44] = 1;
TABLE[255][439] = 1;
TABLE[256][439] = 1;
TABLE[257][36] = 1;
TABLE[258][36] = 1;
TABLE[259][36] = 1;
TABLE[260][36] = 1;
TABLE[261][369] = 1;
TABLE[262][346] = 1;
TABLE[792][294] = 1;
TABLE[1273][44] = 1;
TABLE[793][47] = 1;
TABLE[794][407] = 1;
TABLE[795][47] = 1;
TABLE[796][407] = 1;
TABLE[263][327] = 1;
TABLE[797][436] = 1;
TABLE[797][358] = 1;
TABLE[797][325] = 1;
TABLE[797][399] = 1;
TABLE[797][433] = 1;
TABLE[797][376] = 1;
TABLE[797][310] = 1;
TABLE[797][264] = 1;
TABLE[797][267] = 1;
TABLE[797][263] = 1;
TABLE[797][271] = 1;
TABLE[797][272] = 1;
TABLE[797][273] = 1;
TABLE[797][473] = 1;
TABLE[1274][125] = 1;
TABLE[264][436] = 1;
TABLE[265][358] = 1;
TABLE[266][325] = 1;
TABLE[267][399] = 1;
TABLE[268][310] = 1;
TABLE[268][376] = 1;
TABLE[268][433] = 1;
TABLE[269][473] = 1;
TABLE[269][273] = 1;
TABLE[269][272] = 1;
TABLE[269][271] = 1;
TABLE[269][263] = 1;
TABLE[269][267] = 1;
TABLE[269][264] = 1;
TABLE[798][58] = 1;
TABLE[1275][36] = 1;
TABLE[1275][59] = 1;
TABLE[800][36] = 1;
TABLE[1277][59] = 1;
TABLE[799][44] = 1;
TABLE[1276][36] = 1;
TABLE[1276][59] = 1;
TABLE[801][58] = 1;
TABLE[1278][346] = 1;
TABLE[1278][439] = 1;
TABLE[1278][281] = 1;
TABLE[1278][59] = 1;
TABLE[803][281] = 1;
TABLE[803][439] = 1;
TABLE[803][346] = 1;
TABLE[1280][59] = 1;
TABLE[802][44] = 1;
TABLE[1279][346] = 1;
TABLE[1279][439] = 1;
TABLE[1279][281] = 1;
TABLE[1279][59] = 1;
TABLE[804][58] = 1;
TABLE[1281][257] = 1;
TABLE[270][357] = 1;
TABLE[805][328] = 1;
TABLE[805][310] = 1;
TABLE[805][349] = 1;
TABLE[805][325] = 1;
TABLE[805][358] = 1;
TABLE[805][399] = 1;
TABLE[805][433] = 1;
TABLE[805][376] = 1;
TABLE[805][264] = 1;
TABLE[805][267] = 1;
TABLE[805][263] = 1;
TABLE[805][271] = 1;
TABLE[805][272] = 1;
TABLE[805][273] = 1;
TABLE[805][473] = 1;
TABLE[1282][125] = 1;
TABLE[271][328] = 1;
TABLE[272][310] = 1;
TABLE[273][349] = 1;
TABLE[274][325] = 1;
TABLE[275][358] = 1;
TABLE[276][399] = 1;
TABLE[277][310] = 1;
TABLE[277][376] = 1;
TABLE[277][433] = 1;
TABLE[278][473] = 1;
TABLE[278][273] = 1;
TABLE[278][272] = 1;
TABLE[278][271] = 1;
TABLE[278][263] = 1;
TABLE[278][267] = 1;
TABLE[278][264] = 1;
TABLE[806][58] = 1;
TABLE[1283][36] = 1;
TABLE[1283][59] = 1;
TABLE[808][36] = 1;
TABLE[1285][59] = 1;
TABLE[807][44] = 1;
TABLE[1284][36] = 1;
TABLE[1284][59] = 1;
TABLE[809][58] = 1;
TABLE[1286][45] = 1;
TABLE[1286][43] = 1;
TABLE[1286][40] = 1;
TABLE[1286][256] = 1;
TABLE[1286][457] = 1;
TABLE[1286][344] = 1;
TABLE[1286][259] = 1;
TABLE[1286][59] = 1;
TABLE[812][43] = 1;
TABLE[812][45] = 1;
TABLE[812][259] = 1;
TABLE[812][344] = 1;
TABLE[812][457] = 1;
TABLE[812][256] = 1;
TABLE[812][40] = 1;
TABLE[1288][59] = 1;
TABLE[810][124] = 1;
TABLE[811][44] = 1;
TABLE[1287][45] = 1;
TABLE[1287][43] = 1;
TABLE[1287][40] = 1;
TABLE[1287][256] = 1;
TABLE[1287][457] = 1;
TABLE[1287][344] = 1;
TABLE[1287][259] = 1;
TABLE[1287][59] = 1;
TABLE[813][58] = 1;
TABLE[1289][257] = 1;
TABLE[814][58] = 1;
TABLE[1290][346] = 1;
TABLE[1290][439] = 1;
TABLE[1290][281] = 1;
TABLE[1290][59] = 1;
TABLE[816][281] = 1;
TABLE[816][439] = 1;
TABLE[816][346] = 1;
TABLE[1292][59] = 1;
TABLE[815][44] = 1;
TABLE[1291][346] = 1;
TABLE[1291][439] = 1;
TABLE[1291][281] = 1;
TABLE[1291][59] = 1;
TABLE[279][449] = 1;
TABLE[817][325] = 1;
TABLE[817][388] = 1;
TABLE[817][406] = 1;
TABLE[817][382] = 1;
TABLE[817][349] = 1;
TABLE[817][433] = 1;
TABLE[817][376] = 1;
TABLE[817][310] = 1;
TABLE[817][399] = 1;
TABLE[817][367] = 1;
TABLE[817][356] = 1;
TABLE[817][264] = 1;
TABLE[817][267] = 1;
TABLE[817][263] = 1;
TABLE[817][271] = 1;
TABLE[817][272] = 1;
TABLE[817][273] = 1;
TABLE[817][473] = 1;
TABLE[1293][125] = 1;
TABLE[280][325] = 1;
TABLE[281][356] = 1;
TABLE[281][367] = 1;
TABLE[282][388] = 1;
TABLE[283][406] = 1;
TABLE[284][382] = 1;
TABLE[285][349] = 1;
TABLE[286][310] = 1;
TABLE[286][376] = 1;
TABLE[286][433] = 1;
TABLE[287][399] = 1;
TABLE[288][473] = 1;
TABLE[288][273] = 1;
TABLE[288][272] = 1;
TABLE[288][271] = 1;
TABLE[288][263] = 1;
TABLE[288][267] = 1;
TABLE[288][264] = 1;
TABLE[818][58] = 1;
TABLE[1294][257] = 1;
TABLE[819][58] = 1;
TABLE[1295][43] = 1;
TABLE[1295][45] = 1;
TABLE[1295][259] = 1;
TABLE[1295][344] = 1;
TABLE[1295][457] = 1;
TABLE[1295][256] = 1;
TABLE[1295][40] = 1;
TABLE[820][58] = 1;
TABLE[1296][43] = 1;
TABLE[1296][45] = 1;
TABLE[1296][259] = 1;
TABLE[1296][344] = 1;
TABLE[1296][457] = 1;
TABLE[1296][256] = 1;
TABLE[1296][40] = 1;
TABLE[821][58] = 1;
TABLE[1297][43] = 1;
TABLE[1297][45] = 1;
TABLE[1297][259] = 1;
TABLE[1297][344] = 1;
TABLE[1297][457] = 1;
TABLE[1297][256] = 1;
TABLE[1297][40] = 1;
TABLE[822][58] = 1;
TABLE[1298][45] = 1;
TABLE[1298][43] = 1;
TABLE[1298][40] = 1;
TABLE[1298][256] = 1;
TABLE[1298][457] = 1;
TABLE[1298][344] = 1;
TABLE[1298][259] = 1;
TABLE[1298][59] = 1;
TABLE[825][43] = 1;
TABLE[825][45] = 1;
TABLE[825][259] = 1;
TABLE[825][344] = 1;
TABLE[825][457] = 1;
TABLE[825][256] = 1;
TABLE[825][40] = 1;
TABLE[1300][59] = 1;
TABLE[823][124] = 1;
TABLE[824][44] = 1;
TABLE[1299][45] = 1;
TABLE[1299][43] = 1;
TABLE[1299][40] = 1;
TABLE[1299][256] = 1;
TABLE[1299][457] = 1;
TABLE[1299][344] = 1;
TABLE[1299][259] = 1;
TABLE[1299][59] = 1;
TABLE[289][421] = 1;
TABLE[826][325] = 1;
TABLE[826][463] = 1;
TABLE[826][350] = 1;
TABLE[826][326] = 1;
TABLE[826][324] = 1;
TABLE[826][430] = 1;
TABLE[826][425] = 1;
TABLE[826][349] = 1;
TABLE[826][359] = 1;
TABLE[826][454] = 1;
TABLE[826][422] = 1;
TABLE[826][397] = 1;
TABLE[826][433] = 1;
TABLE[826][376] = 1;
TABLE[826][310] = 1;
TABLE[826][399] = 1;
TABLE[826][264] = 1;
TABLE[826][267] = 1;
TABLE[826][263] = 1;
TABLE[826][271] = 1;
TABLE[826][272] = 1;
TABLE[826][273] = 1;
TABLE[826][473] = 1;
TABLE[1301][125] = 1;
TABLE[290][325] = 1;
TABLE[291][463] = 1;
TABLE[292][350] = 1;
TABLE[293][326] = 1;
TABLE[294][324] = 1;
TABLE[295][430] = 1;
TABLE[296][425] = 1;
TABLE[297][349] = 1;
TABLE[298][359] = 1;
TABLE[299][454] = 1;
TABLE[300][422] = 1;
TABLE[301][397] = 1;
TABLE[302][310] = 1;
TABLE[302][376] = 1;
TABLE[302][433] = 1;
TABLE[303][399] = 1;
TABLE[304][473] = 1;
TABLE[304][273] = 1;
TABLE[304][272] = 1;
TABLE[304][271] = 1;
TABLE[304][263] = 1;
TABLE[304][267] = 1;
TABLE[304][264] = 1;
TABLE[827][58] = 1;
TABLE[1302][257] = 1;
TABLE[828][58] = 1;
TABLE[1303][36] = 1;
TABLE[829][58] = 1;
TABLE[1304][36] = 1;
TABLE[830][58] = 1;
TABLE[1305][36] = 1;
TABLE[831][58] = 1;
TABLE[1306][43] = 1;
TABLE[1306][45] = 1;
TABLE[1306][259] = 1;
TABLE[1306][344] = 1;
TABLE[1306][457] = 1;
TABLE[1306][256] = 1;
TABLE[1306][40] = 1;
TABLE[832][58] = 1;
TABLE[1307][43] = 1;
TABLE[1307][45] = 1;
TABLE[1307][259] = 1;
TABLE[1307][344] = 1;
TABLE[1307][457] = 1;
TABLE[1307][256] = 1;
TABLE[1307][40] = 1;
TABLE[833][58] = 1;
TABLE[1308][45] = 1;
TABLE[1308][43] = 1;
TABLE[1308][40] = 1;
TABLE[1308][256] = 1;
TABLE[1308][457] = 1;
TABLE[1308][344] = 1;
TABLE[1308][259] = 1;
TABLE[1308][59] = 1;
TABLE[836][43] = 1;
TABLE[836][45] = 1;
TABLE[836][259] = 1;
TABLE[836][344] = 1;
TABLE[836][457] = 1;
TABLE[836][256] = 1;
TABLE[836][40] = 1;
TABLE[1310][59] = 1;
TABLE[834][44] = 1;
TABLE[835][124] = 1;
TABLE[1309][45] = 1;
TABLE[1309][43] = 1;
TABLE[1309][40] = 1;
TABLE[1309][256] = 1;
TABLE[1309][457] = 1;
TABLE[1309][344] = 1;
TABLE[1309][259] = 1;
TABLE[1309][59] = 1;
TABLE[837][58] = 1;
TABLE[1311][45] = 1;
TABLE[1311][43] = 1;
TABLE[1311][40] = 1;
TABLE[1311][256] = 1;
TABLE[1311][457] = 1;
TABLE[1311][344] = 1;
TABLE[1311][259] = 1;
TABLE[1311][59] = 1;
TABLE[840][43] = 1;
TABLE[840][45] = 1;
TABLE[840][259] = 1;
TABLE[840][344] = 1;
TABLE[840][457] = 1;
TABLE[840][256] = 1;
TABLE[840][40] = 1;
TABLE[1313][59] = 1;
TABLE[838][124] = 1;
TABLE[839][44] = 1;
TABLE[1312][45] = 1;
TABLE[1312][43] = 1;
TABLE[1312][40] = 1;
TABLE[1312][256] = 1;
TABLE[1312][457] = 1;
TABLE[1312][344] = 1;
TABLE[1312][259] = 1;
TABLE[1312][59] = 1;
TABLE[853][259] = 1;
TABLE[1319][59] = 1;
TABLE[841][91] = 1;
TABLE[850][470] = 1;
TABLE[851][293] = 1;
TABLE[1317][44] = 1;
TABLE[1317][259] = 1;
TABLE[1317][59] = 1;
TABLE[842][36] = 1;
TABLE[844][418] = 1;
TABLE[846][418] = 1;
TABLE[843][43] = 1;
TABLE[843][45] = 1;
TABLE[843][259] = 1;
TABLE[843][344] = 1;
TABLE[843][457] = 1;
TABLE[843][256] = 1;
TABLE[843][40] = 1;
TABLE[1314][398] = 1;
TABLE[1314][91] = 1;
TABLE[1314][470] = 1;
TABLE[1314][293] = 1;
TABLE[1314][44] = 1;
TABLE[1314][259] = 1;
TABLE[1314][59] = 1;
TABLE[845][43] = 1;
TABLE[845][45] = 1;
TABLE[845][259] = 1;
TABLE[845][344] = 1;
TABLE[845][457] = 1;
TABLE[845][256] = 1;
TABLE[845][40] = 1;
TABLE[1315][398] = 1;
TABLE[1315][91] = 1;
TABLE[1315][470] = 1;
TABLE[1315][293] = 1;
TABLE[1315][44] = 1;
TABLE[1315][259] = 1;
TABLE[1315][59] = 1;
TABLE[847][398] = 1;
TABLE[848][398] = 1;
TABLE[849][398] = 1;
TABLE[1316][91] = 1;
TABLE[1316][470] = 1;
TABLE[1316][293] = 1;
TABLE[1316][44] = 1;
TABLE[1316][259] = 1;
TABLE[1316][59] = 1;
TABLE[852][44] = 1;
TABLE[1318][259] = 1;
TABLE[1318][59] = 1;
TABLE[854][325] = 1;
TABLE[854][463] = 1;
TABLE[854][350] = 1;
TABLE[854][326] = 1;
TABLE[854][324] = 1;
TABLE[854][430] = 1;
TABLE[854][425] = 1;
TABLE[854][349] = 1;
TABLE[854][359] = 1;
TABLE[854][454] = 1;
TABLE[854][422] = 1;
TABLE[854][397] = 1;
TABLE[854][433] = 1;
TABLE[854][376] = 1;
TABLE[854][310] = 1;
TABLE[854][399] = 1;
TABLE[854][264] = 1;
TABLE[854][267] = 1;
TABLE[854][263] = 1;
TABLE[854][271] = 1;
TABLE[854][272] = 1;
TABLE[854][273] = 1;
TABLE[854][473] = 1;
TABLE[1320][399] = 1;
TABLE[1320][310] = 1;
TABLE[1320][376] = 1;
TABLE[1320][433] = 1;
TABLE[1320][397] = 1;
TABLE[1320][422] = 1;
TABLE[1320][454] = 1;
TABLE[1320][359] = 1;
TABLE[1320][349] = 1;
TABLE[1320][425] = 1;
TABLE[1320][430] = 1;
TABLE[1320][324] = 1;
TABLE[1320][326] = 1;
TABLE[1320][350] = 1;
TABLE[1320][463] = 1;
TABLE[1320][325] = 1;
TABLE[1320][473] = 1;
TABLE[1320][273] = 1;
TABLE[1320][272] = 1;
TABLE[1320][271] = 1;
TABLE[1320][263] = 1;
TABLE[1320][267] = 1;
TABLE[1320][264] = 1;
TABLE[1320][125] = 1;
TABLE[855][261] = 1;
TABLE[1321][399] = 1;
TABLE[1321][310] = 1;
TABLE[1321][376] = 1;
TABLE[1321][433] = 1;
TABLE[1321][397] = 1;
TABLE[1321][422] = 1;
TABLE[1321][454] = 1;
TABLE[1321][359] = 1;
TABLE[1321][349] = 1;
TABLE[1321][425] = 1;
TABLE[1321][430] = 1;
TABLE[1321][324] = 1;
TABLE[1321][326] = 1;
TABLE[1321][350] = 1;
TABLE[1321][463] = 1;
TABLE[1321][325] = 1;
TABLE[1321][473] = 1;
TABLE[1321][273] = 1;
TABLE[1321][272] = 1;
TABLE[1321][271] = 1;
TABLE[1321][263] = 1;
TABLE[1321][267] = 1;
TABLE[1321][264] = 1;
TABLE[1321][125] = 1;
TABLE[305][448] = 1;
TABLE[856][352] = 1;
TABLE[1322][123] = 1;
TABLE[857][325] = 1;
TABLE[857][352] = 1;
TABLE[857][389] = 1;
TABLE[857][363] = 1;
TABLE[857][365] = 1;
TABLE[857][322] = 1;
TABLE[857][291] = 1;
TABLE[857][318] = 1;
TABLE[857][409] = 1;
TABLE[857][305] = 1;
TABLE[857][430] = 1;
TABLE[857][434] = 1;
TABLE[857][388] = 1;
TABLE[857][276] = 1;
TABLE[857][295] = 1;
TABLE[857][439] = 1;
TABLE[857][423] = 1;
TABLE[857][368] = 1;
TABLE[857][345] = 1;
TABLE[857][436] = 1;
TABLE[857][329] = 1;
TABLE[857][408] = 1;
TABLE[857][349] = 1;
TABLE[857][355] = 1;
TABLE[857][399] = 1;
TABLE[857][433] = 1;
TABLE[857][376] = 1;
TABLE[857][310] = 1;
TABLE[857][367] = 1;
TABLE[857][356] = 1;
TABLE[857][306] = 1;
TABLE[857][373] = 1;
TABLE[857][264] = 1;
TABLE[857][267] = 1;
TABLE[857][263] = 1;
TABLE[857][271] = 1;
TABLE[857][272] = 1;
TABLE[857][273] = 1;
TABLE[857][473] = 1;
TABLE[1323][125] = 1;
TABLE[306][325] = 1;
TABLE[307][352] = 1;
TABLE[308][389] = 1;
TABLE[309][356] = 1;
TABLE[309][367] = 1;
TABLE[310][373] = 1;
TABLE[310][306] = 1;
TABLE[311][363] = 1;
TABLE[312][365] = 1;
TABLE[313][322] = 1;
TABLE[314][291] = 1;
TABLE[315][318] = 1;
TABLE[316][409] = 1;
TABLE[317][305] = 1;
TABLE[318][430] = 1;
TABLE[319][434] = 1;
TABLE[320][388] = 1;
TABLE[321][276] = 1;
TABLE[322][295] = 1;
TABLE[323][439] = 1;
TABLE[324][439] = 1;
TABLE[325][439] = 1;
TABLE[326][439] = 1;
TABLE[327][423] = 1;
TABLE[328][368] = 1;
TABLE[329][345] = 1;
TABLE[330][436] = 1;
TABLE[331][329] = 1;
TABLE[332][408] = 1;
TABLE[333][349] = 1;
TABLE[334][355] = 1;
TABLE[335][399] = 1;
TABLE[336][310] = 1;
TABLE[336][376] = 1;
TABLE[336][433] = 1;
TABLE[337][473] = 1;
TABLE[337][273] = 1;
TABLE[337][272] = 1;
TABLE[337][271] = 1;
TABLE[337][263] = 1;
TABLE[337][267] = 1;
TABLE[337][264] = 1;
TABLE[858][58] = 1;
TABLE[1324][257] = 1;
TABLE[859][58] = 1;
TABLE[1325][43] = 1;
TABLE[1325][45] = 1;
TABLE[1325][259] = 1;
TABLE[1325][344] = 1;
TABLE[1325][457] = 1;
TABLE[1325][256] = 1;
TABLE[1325][40] = 1;
TABLE[860][58] = 1;
TABLE[1326][43] = 1;
TABLE[1326][45] = 1;
TABLE[1326][259] = 1;
TABLE[1326][344] = 1;
TABLE[1326][457] = 1;
TABLE[1326][256] = 1;
TABLE[1326][40] = 1;
TABLE[861][373] = 1;
TABLE[862][306] = 1;
TABLE[863][58] = 1;
TABLE[1327][43] = 1;
TABLE[1327][61] = 1;
TABLE[1327][45] = 1;
TABLE[1327][40] = 1;
TABLE[1327][256] = 1;
TABLE[1327][457] = 1;
TABLE[1327][344] = 1;
TABLE[1327][259] = 1;
TABLE[1327][59] = 1;
TABLE[864][58] = 1;
TABLE[1328][43] = 1;
TABLE[1328][61] = 1;
TABLE[1328][45] = 1;
TABLE[1328][40] = 1;
TABLE[1328][256] = 1;
TABLE[1328][457] = 1;
TABLE[1328][344] = 1;
TABLE[1328][259] = 1;
TABLE[1328][59] = 1;
TABLE[865][58] = 1;
TABLE[1329][43] = 1;
TABLE[1329][61] = 1;
TABLE[1329][45] = 1;
TABLE[1329][40] = 1;
TABLE[1329][256] = 1;
TABLE[1329][457] = 1;
TABLE[1329][344] = 1;
TABLE[1329][259] = 1;
TABLE[1329][59] = 1;
TABLE[866][58] = 1;
TABLE[1330][43] = 1;
TABLE[1330][61] = 1;
TABLE[1330][45] = 1;
TABLE[1330][40] = 1;
TABLE[1330][256] = 1;
TABLE[1330][457] = 1;
TABLE[1330][344] = 1;
TABLE[1330][259] = 1;
TABLE[1330][59] = 1;
TABLE[867][58] = 1;
TABLE[1331][43] = 1;
TABLE[1331][61] = 1;
TABLE[1331][45] = 1;
TABLE[1331][40] = 1;
TABLE[1331][256] = 1;
TABLE[1331][457] = 1;
TABLE[1331][344] = 1;
TABLE[1331][259] = 1;
TABLE[1331][59] = 1;
TABLE[868][58] = 1;
TABLE[1332][43] = 1;
TABLE[1332][45] = 1;
TABLE[1332][259] = 1;
TABLE[1332][344] = 1;
TABLE[1332][457] = 1;
TABLE[1332][256] = 1;
TABLE[1332][40] = 1;
TABLE[869][58] = 1;
TABLE[1333][43] = 1;
TABLE[1333][61] = 1;
TABLE[1333][45] = 1;
TABLE[1333][40] = 1;
TABLE[1333][256] = 1;
TABLE[1333][457] = 1;
TABLE[1333][344] = 1;
TABLE[1333][259] = 1;
TABLE[1333][59] = 1;
TABLE[870][58] = 1;
TABLE[1334][43] = 1;
TABLE[1334][61] = 1;
TABLE[1334][45] = 1;
TABLE[1334][40] = 1;
TABLE[1334][256] = 1;
TABLE[1334][457] = 1;
TABLE[1334][344] = 1;
TABLE[1334][259] = 1;
TABLE[1334][59] = 1;
TABLE[871][58] = 1;
TABLE[1335][43] = 1;
TABLE[1335][61] = 1;
TABLE[1335][45] = 1;
TABLE[1335][40] = 1;
TABLE[1335][256] = 1;
TABLE[1335][457] = 1;
TABLE[1335][344] = 1;
TABLE[1335][259] = 1;
TABLE[1335][59] = 1;
TABLE[872][58] = 1;
TABLE[1336][43] = 1;
TABLE[1336][61] = 1;
TABLE[1336][45] = 1;
TABLE[1336][40] = 1;
TABLE[1336][256] = 1;
TABLE[1336][457] = 1;
TABLE[1336][344] = 1;
TABLE[1336][259] = 1;
TABLE[1336][59] = 1;
TABLE[873][58] = 1;
TABLE[1337][43] = 1;
TABLE[1337][61] = 1;
TABLE[1337][45] = 1;
TABLE[1337][40] = 1;
TABLE[1337][256] = 1;
TABLE[1337][457] = 1;
TABLE[1337][344] = 1;
TABLE[1337][259] = 1;
TABLE[1337][59] = 1;
TABLE[874][58] = 1;
TABLE[1338][43] = 1;
TABLE[1338][61] = 1;
TABLE[1338][45] = 1;
TABLE[1338][40] = 1;
TABLE[1338][256] = 1;
TABLE[1338][457] = 1;
TABLE[1338][344] = 1;
TABLE[1338][259] = 1;
TABLE[1338][44] = 1;
TABLE[1338][59] = 1;
TABLE[875][44] = 1;
TABLE[876][59] = 1;
TABLE[877][58] = 1;
TABLE[1339][45] = 1;
TABLE[1339][43] = 1;
TABLE[1339][40] = 1;
TABLE[1339][256] = 1;
TABLE[1339][457] = 1;
TABLE[1339][344] = 1;
TABLE[1339][259] = 1;
TABLE[878][44] = 1;
TABLE[1340][59] = 1;
TABLE[879][58] = 1;
TABLE[1341][43] = 1;
TABLE[1341][45] = 1;
TABLE[1341][126] = 1;
TABLE[1341][33] = 1;
TABLE[1341][259] = 1;
TABLE[1341][344] = 1;
TABLE[1341][457] = 1;
TABLE[1341][256] = 1;
TABLE[1341][40] = 1;
TABLE[1341][59] = 1;
TABLE[885][45] = 1;
TABLE[885][43] = 1;
TABLE[885][40] = 1;
TABLE[885][256] = 1;
TABLE[885][457] = 1;
TABLE[885][344] = 1;
TABLE[885][259] = 1;
TABLE[885][33] = 1;
TABLE[885][126] = 1;
TABLE[1343][59] = 1;
TABLE[880][43] = 1;
TABLE[880][45] = 1;
TABLE[880][259] = 1;
TABLE[880][344] = 1;
TABLE[880][457] = 1;
TABLE[880][256] = 1;
TABLE[880][40] = 1;
TABLE[883][126] = 1;
TABLE[883][33] = 1;
TABLE[881][126] = 1;
TABLE[882][33] = 1;
TABLE[884][44] = 1;
TABLE[1342][43] = 1;
TABLE[1342][45] = 1;
TABLE[1342][126] = 1;
TABLE[1342][33] = 1;
TABLE[1342][259] = 1;
TABLE[1342][344] = 1;
TABLE[1342][457] = 1;
TABLE[1342][256] = 1;
TABLE[1342][40] = 1;
TABLE[1342][59] = 1;
TABLE[886][58] = 1;
TABLE[1344][43] = 1;
TABLE[1344][45] = 1;
TABLE[1344][126] = 1;
TABLE[1344][33] = 1;
TABLE[1344][259] = 1;
TABLE[1344][344] = 1;
TABLE[1344][457] = 1;
TABLE[1344][256] = 1;
TABLE[1344][40] = 1;
TABLE[1344][59] = 1;
TABLE[892][45] = 1;
TABLE[892][43] = 1;
TABLE[892][40] = 1;
TABLE[892][256] = 1;
TABLE[892][457] = 1;
TABLE[892][344] = 1;
TABLE[892][259] = 1;
TABLE[892][33] = 1;
TABLE[892][126] = 1;
TABLE[1346][59] = 1;
TABLE[887][43] = 1;
TABLE[887][45] = 1;
TABLE[887][259] = 1;
TABLE[887][344] = 1;
TABLE[887][457] = 1;
TABLE[887][256] = 1;
TABLE[887][40] = 1;
TABLE[890][126] = 1;
TABLE[890][33] = 1;
TABLE[888][126] = 1;
TABLE[889][33] = 1;
TABLE[891][44] = 1;
TABLE[1345][43] = 1;
TABLE[1345][45] = 1;
TABLE[1345][126] = 1;
TABLE[1345][33] = 1;
TABLE[1345][259] = 1;
TABLE[1345][344] = 1;
TABLE[1345][457] = 1;
TABLE[1345][256] = 1;
TABLE[1345][40] = 1;
TABLE[1345][59] = 1;
TABLE[893][58] = 1;
TABLE[1347][43] = 1;
TABLE[1347][45] = 1;
TABLE[1347][259] = 1;
TABLE[1347][344] = 1;
TABLE[1347][457] = 1;
TABLE[1347][256] = 1;
TABLE[1347][40] = 1;
TABLE[894][43] = 1;
TABLE[894][45] = 1;
TABLE[894][259] = 1;
TABLE[894][344] = 1;
TABLE[894][457] = 1;
TABLE[894][256] = 1;
TABLE[894][40] = 1;
TABLE[1348][43] = 1;
TABLE[1348][45] = 1;
TABLE[1348][259] = 1;
TABLE[1348][344] = 1;
TABLE[1348][457] = 1;
TABLE[1348][256] = 1;
TABLE[1348][40] = 1;
TABLE[895][58] = 1;
TABLE[1349][36] = 1;
TABLE[1349][59] = 1;
TABLE[897][36] = 1;
TABLE[1351][59] = 1;
TABLE[896][44] = 1;
TABLE[1350][36] = 1;
TABLE[1350][59] = 1;
TABLE[898][58] = 1;
TABLE[1352][36] = 1;
TABLE[1352][59] = 1;
TABLE[900][36] = 1;
TABLE[1354][59] = 1;
TABLE[899][44] = 1;
TABLE[1353][36] = 1;
TABLE[1353][59] = 1;
TABLE[901][58] = 1;
TABLE[1355][36] = 1;
TABLE[1355][59] = 1;
TABLE[903][36] = 1;
TABLE[1357][59] = 1;
TABLE[902][44] = 1;
TABLE[1356][36] = 1;
TABLE[1356][59] = 1;
TABLE[904][58] = 1;
TABLE[1358][43] = 1;
TABLE[1358][45] = 1;
TABLE[1358][126] = 1;
TABLE[1358][33] = 1;
TABLE[1358][259] = 1;
TABLE[1358][344] = 1;
TABLE[1358][457] = 1;
TABLE[1358][256] = 1;
TABLE[1358][40] = 1;
TABLE[1358][310] = 1;
TABLE[1358][376] = 1;
TABLE[1358][433] = 1;
TABLE[1358][399] = 1;
TABLE[1358][355] = 1;
TABLE[1358][349] = 1;
TABLE[1358][408] = 1;
TABLE[1358][329] = 1;
TABLE[1358][436] = 1;
TABLE[1358][345] = 1;
TABLE[1358][368] = 1;
TABLE[1358][423] = 1;
TABLE[1358][439] = 1;
TABLE[1358][295] = 1;
TABLE[1358][276] = 1;
TABLE[1358][388] = 1;
TABLE[1358][434] = 1;
TABLE[1358][430] = 1;
TABLE[1358][305] = 1;
TABLE[1358][409] = 1;
TABLE[1358][318] = 1;
TABLE[1358][291] = 1;
TABLE[1358][322] = 1;
TABLE[1358][365] = 1;
TABLE[1358][363] = 1;
TABLE[1358][389] = 1;
TABLE[1358][352] = 1;
TABLE[1358][325] = 1;
TABLE[1358][473] = 1;
TABLE[1358][273] = 1;
TABLE[1358][272] = 1;
TABLE[1358][271] = 1;
TABLE[1358][263] = 1;
TABLE[1358][267] = 1;
TABLE[1358][264] = 1;
TABLE[1358][373] = 1;
TABLE[1358][306] = 1;
TABLE[1358][356] = 1;
TABLE[1358][367] = 1;
TABLE[1358][125] = 1;
TABLE[911][45] = 1;
TABLE[911][43] = 1;
TABLE[911][40] = 1;
TABLE[911][256] = 1;
TABLE[911][457] = 1;
TABLE[911][344] = 1;
TABLE[911][259] = 1;
TABLE[911][33] = 1;
TABLE[911][126] = 1;
TABLE[1359][310] = 1;
TABLE[1359][376] = 1;
TABLE[1359][433] = 1;
TABLE[1359][399] = 1;
TABLE[1359][355] = 1;
TABLE[1359][349] = 1;
TABLE[1359][408] = 1;
TABLE[1359][329] = 1;
TABLE[1359][436] = 1;
TABLE[1359][345] = 1;
TABLE[1359][368] = 1;
TABLE[1359][423] = 1;
TABLE[1359][439] = 1;
TABLE[1359][295] = 1;
TABLE[1359][276] = 1;
TABLE[1359][388] = 1;
TABLE[1359][434] = 1;
TABLE[1359][430] = 1;
TABLE[1359][305] = 1;
TABLE[1359][409] = 1;
TABLE[1359][318] = 1;
TABLE[1359][291] = 1;
TABLE[1359][322] = 1;
TABLE[1359][365] = 1;
TABLE[1359][363] = 1;
TABLE[1359][389] = 1;
TABLE[1359][352] = 1;
TABLE[1359][325] = 1;
TABLE[1359][473] = 1;
TABLE[1359][273] = 1;
TABLE[1359][272] = 1;
TABLE[1359][271] = 1;
TABLE[1359][263] = 1;
TABLE[1359][267] = 1;
TABLE[1359][264] = 1;
TABLE[1359][373] = 1;
TABLE[1359][306] = 1;
TABLE[1359][356] = 1;
TABLE[1359][367] = 1;
TABLE[1359][125] = 1;
TABLE[905][43] = 1;
TABLE[905][45] = 1;
TABLE[905][259] = 1;
TABLE[905][344] = 1;
TABLE[905][457] = 1;
TABLE[905][256] = 1;
TABLE[905][40] = 1;
TABLE[908][126] = 1;
TABLE[908][33] = 1;
TABLE[906][126] = 1;
TABLE[907][33] = 1;
TABLE[909][44] = 1;
TABLE[910][59] = 1;
TABLE[338][43] = 1;
TABLE[338][45] = 1;
TABLE[338][259] = 1;
TABLE[338][344] = 1;
TABLE[338][457] = 1;
TABLE[338][256] = 1;
TABLE[338][40] = 1;
TABLE[339][43] = 1;
TABLE[339][45] = 1;
TABLE[339][259] = 1;
TABLE[339][344] = 1;
TABLE[339][457] = 1;
TABLE[339][256] = 1;
TABLE[339][40] = 1;
TABLE[912][259] = 1;
TABLE[912][344] = 1;
TABLE[912][457] = 1;
TABLE[912][256] = 1;
TABLE[912][40] = 1;
TABLE[913][43] = 1;
TABLE[913][45] = 1;
TABLE[913][259] = 1;
TABLE[913][344] = 1;
TABLE[913][457] = 1;
TABLE[913][256] = 1;
TABLE[913][40] = 1;
TABLE[1360][45] = 1;
TABLE[1360][43] = 1;
TABLE[1360][256] = 1;
TABLE[1360][457] = 1;
TABLE[1360][344] = 1;
TABLE[1360][259] = 1;
TABLE[1360][318] = 1;
TABLE[1360][40] = 1;
TABLE[1360][287] = 1;
TABLE[1360][44] = 1;
TABLE[1360][59] = 1;
TABLE[914][43] = 1;
TABLE[914][45] = 1;
TABLE[914][259] = 1;
TABLE[914][344] = 1;
TABLE[914][457] = 1;
TABLE[914][256] = 1;
TABLE[914][40] = 1;
TABLE[1361][318] = 1;
TABLE[1361][40] = 1;
TABLE[1361][287] = 1;
TABLE[1361][44] = 1;
TABLE[1361][59] = 1;
TABLE[915][318] = 1;
TABLE[1362][40] = 1;
TABLE[1362][287] = 1;
TABLE[1362][44] = 1;
TABLE[1362][59] = 1;
TABLE[919][40] = 1;
TABLE[1363][287] = 1;
TABLE[1363][44] = 1;
TABLE[1363][59] = 1;
TABLE[916][321] = 1;
TABLE[917][306] = 1;
TABLE[918][373] = 1;
TABLE[927][287] = 1;
TABLE[1368][44] = 1;
TABLE[1368][59] = 1;
TABLE[920][259] = 1;
TABLE[920][344] = 1;
TABLE[920][457] = 1;
TABLE[920][256] = 1;
TABLE[920][40] = 1;
TABLE[1364][45] = 1;
TABLE[1364][43] = 1;
TABLE[1364][256] = 1;
TABLE[1364][457] = 1;
TABLE[1364][344] = 1;
TABLE[1364][259] = 1;
TABLE[1364][318] = 1;
TABLE[1364][40] = 1;
TABLE[1364][287] = 1;
TABLE[1364][44] = 1;
TABLE[1364][59] = 1;
TABLE[921][43] = 1;
TABLE[921][45] = 1;
TABLE[921][259] = 1;
TABLE[921][344] = 1;
TABLE[921][457] = 1;
TABLE[921][256] = 1;
TABLE[921][40] = 1;
TABLE[1365][318] = 1;
TABLE[1365][40] = 1;
TABLE[1365][287] = 1;
TABLE[1365][44] = 1;
TABLE[1365][59] = 1;
TABLE[922][318] = 1;
TABLE[1366][40] = 1;
TABLE[1366][287] = 1;
TABLE[1366][44] = 1;
TABLE[1366][59] = 1;
TABLE[926][40] = 1;
TABLE[1367][287] = 1;
TABLE[1367][44] = 1;
TABLE[1367][59] = 1;
TABLE[923][321] = 1;
TABLE[924][306] = 1;
TABLE[925][373] = 1;
TABLE[928][43] = 1;
TABLE[928][45] = 1;
TABLE[928][259] = 1;
TABLE[928][344] = 1;
TABLE[928][457] = 1;
TABLE[928][256] = 1;
TABLE[928][40] = 1;
TABLE[1369][44] = 1;
TABLE[1369][59] = 1;
TABLE[340][261] = 1;
TABLE[341][340] = 1;
TABLE[929][325] = 1;
TABLE[929][382] = 1;
TABLE[929][309] = 1;
TABLE[929][465] = 1;
TABLE[1370][125] = 1;
TABLE[342][325] = 1;
TABLE[343][382] = 1;
TABLE[344][309] = 1;
TABLE[345][465] = 1;
TABLE[930][58] = 1;
TABLE[1371][257] = 1;
TABLE[931][58] = 1;
TABLE[1372][43] = 1;
TABLE[1372][45] = 1;
TABLE[1372][259] = 1;
TABLE[1372][344] = 1;
TABLE[1372][457] = 1;
TABLE[1372][256] = 1;
TABLE[1372][40] = 1;
TABLE[932][58] = 1;
TABLE[1373][274] = 1;
TABLE[1373][302] = 1;
TABLE[1373][375] = 1;
TABLE[1373][319] = 1;
TABLE[933][58] = 1;
TABLE[1374][43] = 1;
TABLE[1374][45] = 1;
TABLE[1374][259] = 1;
TABLE[1374][344] = 1;
TABLE[1374][457] = 1;
TABLE[1374][256] = 1;
TABLE[1374][40] = 1;
TABLE[346][335] = 1;
TABLE[934][58] = 1;
TABLE[1375][123] = 1;
TABLE[935][449] = 1;
TABLE[935][324] = 1;
TABLE[935][465] = 1;
TABLE[935][306] = 1;
TABLE[935][284] = 1;
TABLE[935][405] = 1;
TABLE[935][349] = 1;
TABLE[935][355] = 1;
TABLE[935][399] = 1;
TABLE[935][433] = 1;
TABLE[935][376] = 1;
TABLE[935][310] = 1;
TABLE[935][264] = 1;
TABLE[935][267] = 1;
TABLE[935][263] = 1;
TABLE[935][271] = 1;
TABLE[935][272] = 1;
TABLE[935][273] = 1;
TABLE[935][473] = 1;
TABLE[1376][125] = 1;
TABLE[347][449] = 1;
TABLE[348][324] = 1;
TABLE[349][465] = 1;
TABLE[350][306] = 1;
TABLE[351][306] = 1;
TABLE[352][306] = 1;
TABLE[353][284] = 1;
TABLE[354][405] = 1;
TABLE[355][349] = 1;
TABLE[356][355] = 1;
TABLE[357][399] = 1;
TABLE[358][310] = 1;
TABLE[358][376] = 1;
TABLE[358][433] = 1;
TABLE[359][473] = 1;
TABLE[359][273] = 1;
TABLE[359][272] = 1;
TABLE[359][271] = 1;
TABLE[359][263] = 1;
TABLE[359][267] = 1;
TABLE[359][264] = 1;
TABLE[936][456] = 1;
TABLE[937][45] = 1;
TABLE[938][124] = 1;
TABLE[938][334] = 1;
TABLE[938][42] = 1;
TABLE[1377][59] = 1;
TABLE[939][58] = 1;
TABLE[1378][45] = 1;
TABLE[1378][43] = 1;
TABLE[1378][40] = 1;
TABLE[1378][256] = 1;
TABLE[1378][457] = 1;
TABLE[1378][344] = 1;
TABLE[1378][259] = 1;
TABLE[1378][59] = 1;
TABLE[942][43] = 1;
TABLE[942][45] = 1;
TABLE[942][259] = 1;
TABLE[942][344] = 1;
TABLE[942][457] = 1;
TABLE[942][256] = 1;
TABLE[942][40] = 1;
TABLE[1380][59] = 1;
TABLE[940][44] = 1;
TABLE[941][124] = 1;
TABLE[1379][45] = 1;
TABLE[1379][43] = 1;
TABLE[1379][40] = 1;
TABLE[1379][256] = 1;
TABLE[1379][457] = 1;
TABLE[1379][344] = 1;
TABLE[1379][259] = 1;
TABLE[1379][59] = 1;
TABLE[360][42] = 1;
TABLE[360][334] = 1;
TABLE[360][124] = 1;
TABLE[943][42] = 1;
TABLE[944][334] = 1;
TABLE[945][124] = 1;
TABLE[946][40] = 1;
TABLE[946][256] = 1;
TABLE[946][457] = 1;
TABLE[946][344] = 1;
TABLE[946][259] = 1;
TABLE[947][60] = 1;
TABLE[948][91] = 1;
TABLE[949][123] = 1;
TABLE[952][40] = 1;
TABLE[952][256] = 1;
TABLE[952][457] = 1;
TABLE[952][344] = 1;
TABLE[952][259] = 1;
TABLE[953][40] = 1;
TABLE[953][256] = 1;
TABLE[953][457] = 1;
TABLE[953][344] = 1;
TABLE[953][259] = 1;
TABLE[954][40] = 1;
TABLE[954][256] = 1;
TABLE[954][457] = 1;
TABLE[954][344] = 1;
TABLE[954][259] = 1;
TABLE[955][259] = 1;
TABLE[955][344] = 1;
TABLE[955][457] = 1;
TABLE[955][256] = 1;
TABLE[955][40] = 1;
TABLE[1381][398] = 1;
TABLE[950][45] = 1;
TABLE[951][456] = 1;
TABLE[956][36] = 1;
TABLE[957][43] = 1;
TABLE[957][45] = 1;
TABLE[957][259] = 1;
TABLE[957][344] = 1;
TABLE[957][457] = 1;
TABLE[957][256] = 1;
TABLE[957][40] = 1;
TABLE[958][288] = 1;
TABLE[959][36] = 1;
TABLE[960][43] = 1;
TABLE[960][45] = 1;
TABLE[960][259] = 1;
TABLE[960][344] = 1;
TABLE[960][457] = 1;
TABLE[960][256] = 1;
TABLE[960][40] = 1;
TABLE[961][288] = 1;
TABLE[1382][91] = 1;
TABLE[1382][42] = 1;
TABLE[1382][334] = 1;
TABLE[1382][124] = 1;
TABLE[1382][59] = 1;
TABLE[965][91] = 1;
TABLE[1384][42] = 1;
TABLE[1384][334] = 1;
TABLE[1384][124] = 1;
TABLE[1384][59] = 1;
TABLE[964][124] = 1;
TABLE[964][44] = 1;
TABLE[1383][93] = 1;
TABLE[962][124] = 1;
TABLE[963][44] = 1;
TABLE[361][298] = 1;
TABLE[966][435] = 1;
TABLE[966][349] = 1;
TABLE[966][399] = 1;
TABLE[966][433] = 1;
TABLE[966][376] = 1;
TABLE[966][310] = 1;
TABLE[966][264] = 1;
TABLE[966][267] = 1;
TABLE[966][263] = 1;
TABLE[966][271] = 1;
TABLE[966][272] = 1;
TABLE[966][273] = 1;
TABLE[966][473] = 1;
TABLE[1385][125] = 1;
TABLE[362][435] = 1;
TABLE[363][349] = 1;
TABLE[364][399] = 1;
TABLE[365][310] = 1;
TABLE[365][376] = 1;
TABLE[365][433] = 1;
TABLE[366][473] = 1;
TABLE[366][273] = 1;
TABLE[366][272] = 1;
TABLE[366][271] = 1;
TABLE[366][263] = 1;
TABLE[366][267] = 1;
TABLE[366][264] = 1;
TABLE[967][58] = 1;
TABLE[1386][36] = 1;
TABLE[968][58] = 1;
TABLE[1387][45] = 1;
TABLE[1387][43] = 1;
TABLE[1387][40] = 1;
TABLE[1387][256] = 1;
TABLE[1387][457] = 1;
TABLE[1387][344] = 1;
TABLE[1387][259] = 1;
TABLE[1387][59] = 1;
TABLE[971][43] = 1;
TABLE[971][45] = 1;
TABLE[971][259] = 1;
TABLE[971][344] = 1;
TABLE[971][457] = 1;
TABLE[971][256] = 1;
TABLE[971][40] = 1;
TABLE[1389][59] = 1;
TABLE[969][124] = 1;
TABLE[970][44] = 1;
TABLE[1388][45] = 1;
TABLE[1388][43] = 1;
TABLE[1388][40] = 1;
TABLE[1388][256] = 1;
TABLE[1388][457] = 1;
TABLE[1388][344] = 1;
TABLE[1388][259] = 1;
TABLE[1388][59] = 1;
TABLE[367][450] = 1;
TABLE[972][257] = 1;
TABLE[973][36] = 1;
TABLE[974][40] = 1;
TABLE[975][41] = 1;
TABLE[976][124] = 1;
TABLE[977][59] = 1;
TABLE[978][91] = 1;
TABLE[979][91] = 1;
TABLE[1390][59] = 1;
TABLE[368][261] = 1;
TABLE[369][264] = 1;
TABLE[370][267] = 1;
TABLE[371][263] = 1;
TABLE[372][271] = 1;
TABLE[373][272] = 1;
TABLE[374][273] = 1;
TABLE[375][473] = 1;
TABLE[376][446] = 1;
TABLE[987][264] = 1;
TABLE[987][267] = 1;
TABLE[987][263] = 1;
TABLE[987][271] = 1;
TABLE[987][272] = 1;
TABLE[987][273] = 1;
TABLE[987][473] = 1;
TABLE[1398][44] = 1;
TABLE[1398][41] = 1;
TABLE[980][58] = 1;
TABLE[1391][258] = 1;
TABLE[1391][61] = 1;
TABLE[1391][44] = 1;
TABLE[1391][41] = 1;
TABLE[981][258] = 1;
TABLE[1392][61] = 1;
TABLE[1392][44] = 1;
TABLE[1392][41] = 1;
TABLE[982][61] = 1;
TABLE[1393][44] = 1;
TABLE[1393][41] = 1;
TABLE[986][44] = 1;
TABLE[1397][44] = 1;
TABLE[1397][41] = 1;
TABLE[983][58] = 1;
TABLE[1394][258] = 1;
TABLE[1394][61] = 1;
TABLE[1394][44] = 1;
TABLE[1394][41] = 1;
TABLE[984][258] = 1;
TABLE[1395][61] = 1;
TABLE[1395][44] = 1;
TABLE[1395][41] = 1;
TABLE[985][61] = 1;
TABLE[1396][44] = 1;
TABLE[1396][41] = 1;
TABLE[988][44] = 1;
TABLE[1399][41] = 1;
TABLE[377][446] = 1;
TABLE[996][264] = 1;
TABLE[996][267] = 1;
TABLE[996][263] = 1;
TABLE[996][271] = 1;
TABLE[996][272] = 1;
TABLE[996][273] = 1;
TABLE[996][473] = 1;
TABLE[1407][44] = 1;
TABLE[1407][41] = 1;
TABLE[989][58] = 1;
TABLE[1400][258] = 1;
TABLE[1400][61] = 1;
TABLE[1400][44] = 1;
TABLE[1400][41] = 1;
TABLE[990][258] = 1;
TABLE[1401][61] = 1;
TABLE[1401][44] = 1;
TABLE[1401][41] = 1;
TABLE[991][61] = 1;
TABLE[1402][44] = 1;
TABLE[1402][41] = 1;
TABLE[995][44] = 1;
TABLE[1406][44] = 1;
TABLE[1406][41] = 1;
TABLE[992][58] = 1;
TABLE[1403][258] = 1;
TABLE[1403][61] = 1;
TABLE[1403][44] = 1;
TABLE[1403][41] = 1;
TABLE[993][258] = 1;
TABLE[1404][61] = 1;
TABLE[1404][44] = 1;
TABLE[1404][41] = 1;
TABLE[994][61] = 1;
TABLE[1405][44] = 1;
TABLE[1405][41] = 1;
TABLE[997][44] = 1;
TABLE[1408][41] = 1;
TABLE[378][446] = 1;
TABLE[379][446] = 1;
TABLE[380][264] = 1;
TABLE[380][267] = 1;
TABLE[380][263] = 1;
TABLE[380][271] = 1;
TABLE[380][272] = 1;
TABLE[380][273] = 1;
TABLE[380][473] = 1;
TABLE[998][44] = 1;
TABLE[1409][59] = 1;
TABLE[381][264] = 1;
TABLE[381][267] = 1;
TABLE[381][263] = 1;
TABLE[381][271] = 1;
TABLE[381][272] = 1;
TABLE[381][273] = 1;
TABLE[381][473] = 1;
TABLE[999][44] = 1;
TABLE[1410][59] = 1;
TABLE[382][264] = 1;
TABLE[382][267] = 1;
TABLE[382][263] = 1;
TABLE[382][271] = 1;
TABLE[382][272] = 1;
TABLE[382][273] = 1;
TABLE[382][473] = 1;
TABLE[1000][44] = 1;
TABLE[1411][59] = 1;
TABLE[383][474] = 1;
TABLE[383][475] = 1;
TABLE[383][43] = 1;
TABLE[383][45] = 1;
TABLE[383][33] = 1;
TABLE[383][126] = 1;
TABLE[383][58] = 1;
TABLE[383][383] = 1;
TABLE[383][386] = 1;
TABLE[383][258] = 1;
TABLE[383][40] = 1;
TABLE[383][36] = 1;
TABLE[383][394] = 1;
TABLE[383][344] = 1;
TABLE[383][457] = 1;
TABLE[383][257] = 1;
TABLE[383][259] = 1;
TABLE[383][256] = 1;
TABLE[384][366] = 1;
TABLE[385][366] = 1;
TABLE[386][352] = 1;
TABLE[387][445] = 1;
TABLE[388][301] = 1;
TABLE[389][313] = 1;
TABLE[390][330] = 1;
TABLE[391][468] = 1;
TABLE[392][123] = 1;
TABLE[393][424] = 1;
TABLE[394][424] = 1;
TABLE[1004][304] = 1;
TABLE[1413][323] = 1;
TABLE[1413][125] = 1;
TABLE[1001][43] = 1;
TABLE[1001][45] = 1;
TABLE[1001][259] = 1;
TABLE[1001][344] = 1;
TABLE[1001][457] = 1;
TABLE[1001][256] = 1;
TABLE[1001][40] = 1;
TABLE[1002][36] = 1;
TABLE[1003][366] = 1;
TABLE[1003][352] = 1;
TABLE[1003][445] = 1;
TABLE[1003][301] = 1;
TABLE[1003][313] = 1;
TABLE[1003][330] = 1;
TABLE[1003][468] = 1;
TABLE[1003][424] = 1;
TABLE[1003][256] = 1;
TABLE[1003][259] = 1;
TABLE[1003][257] = 1;
TABLE[1003][457] = 1;
TABLE[1003][344] = 1;
TABLE[1003][394] = 1;
TABLE[1003][36] = 1;
TABLE[1003][40] = 1;
TABLE[1003][258] = 1;
TABLE[1003][386] = 1;
TABLE[1003][383] = 1;
TABLE[1003][58] = 1;
TABLE[1003][126] = 1;
TABLE[1003][33] = 1;
TABLE[1003][45] = 1;
TABLE[1003][43] = 1;
TABLE[1003][475] = 1;
TABLE[1003][474] = 1;
TABLE[1003][123] = 1;
TABLE[1412][304] = 1;
TABLE[1412][323] = 1;
TABLE[1412][125] = 1;
TABLE[1006][323] = 1;
TABLE[1415][125] = 1;
TABLE[1005][366] = 1;
TABLE[1005][352] = 1;
TABLE[1005][445] = 1;
TABLE[1005][301] = 1;
TABLE[1005][313] = 1;
TABLE[1005][330] = 1;
TABLE[1005][468] = 1;
TABLE[1005][424] = 1;
TABLE[1005][256] = 1;
TABLE[1005][259] = 1;
TABLE[1005][257] = 1;
TABLE[1005][457] = 1;
TABLE[1005][344] = 1;
TABLE[1005][394] = 1;
TABLE[1005][36] = 1;
TABLE[1005][40] = 1;
TABLE[1005][258] = 1;
TABLE[1005][386] = 1;
TABLE[1005][383] = 1;
TABLE[1005][58] = 1;
TABLE[1005][126] = 1;
TABLE[1005][33] = 1;
TABLE[1005][45] = 1;
TABLE[1005][43] = 1;
TABLE[1005][475] = 1;
TABLE[1005][474] = 1;
TABLE[1005][123] = 1;
TABLE[1414][125] = 1;
TABLE[395][123] = 1;
TABLE[1007][366] = 1;
TABLE[1007][352] = 1;
TABLE[1007][445] = 1;
TABLE[1007][301] = 1;
TABLE[1007][313] = 1;
TABLE[1007][330] = 1;
TABLE[1007][468] = 1;
TABLE[1007][424] = 1;
TABLE[1007][256] = 1;
TABLE[1007][259] = 1;
TABLE[1007][257] = 1;
TABLE[1007][457] = 1;
TABLE[1007][344] = 1;
TABLE[1007][394] = 1;
TABLE[1007][36] = 1;
TABLE[1007][40] = 1;
TABLE[1007][258] = 1;
TABLE[1007][386] = 1;
TABLE[1007][383] = 1;
TABLE[1007][58] = 1;
TABLE[1007][126] = 1;
TABLE[1007][33] = 1;
TABLE[1007][45] = 1;
TABLE[1007][43] = 1;
TABLE[1007][475] = 1;
TABLE[1007][474] = 1;
TABLE[1007][123] = 1;
TABLE[1416][125] = 1;
TABLE[396][123] = 1;
TABLE[1008][473] = 1;
TABLE[1008][273] = 1;
TABLE[1008][272] = 1;
TABLE[1008][271] = 1;
TABLE[1008][263] = 1;
TABLE[1008][267] = 1;
TABLE[1008][264] = 1;
TABLE[1009][440] = 1;
TABLE[1417][424] = 1;
TABLE[1417][468] = 1;
TABLE[1417][330] = 1;
TABLE[1417][313] = 1;
TABLE[1417][301] = 1;
TABLE[1417][445] = 1;
TABLE[1417][352] = 1;
TABLE[1417][366] = 1;
TABLE[1417][123] = 1;
TABLE[1417][474] = 1;
TABLE[1417][475] = 1;
TABLE[1417][43] = 1;
TABLE[1417][45] = 1;
TABLE[1417][33] = 1;
TABLE[1417][126] = 1;
TABLE[1417][58] = 1;
TABLE[1417][383] = 1;
TABLE[1417][386] = 1;
TABLE[1417][258] = 1;
TABLE[1417][40] = 1;
TABLE[1417][36] = 1;
TABLE[1417][394] = 1;
TABLE[1417][344] = 1;
TABLE[1417][457] = 1;
TABLE[1417][257] = 1;
TABLE[1417][259] = 1;
TABLE[1417][256] = 1;
TABLE[1417][125] = 1;
TABLE[1010][366] = 1;
TABLE[1010][352] = 1;
TABLE[1010][445] = 1;
TABLE[1010][301] = 1;
TABLE[1010][313] = 1;
TABLE[1010][330] = 1;
TABLE[1010][468] = 1;
TABLE[1010][424] = 1;
TABLE[1010][256] = 1;
TABLE[1010][259] = 1;
TABLE[1010][257] = 1;
TABLE[1010][457] = 1;
TABLE[1010][344] = 1;
TABLE[1010][394] = 1;
TABLE[1010][36] = 1;
TABLE[1010][40] = 1;
TABLE[1010][258] = 1;
TABLE[1010][386] = 1;
TABLE[1010][383] = 1;
TABLE[1010][58] = 1;
TABLE[1010][126] = 1;
TABLE[1010][33] = 1;
TABLE[1010][45] = 1;
TABLE[1010][43] = 1;
TABLE[1010][475] = 1;
TABLE[1010][474] = 1;
TABLE[1010][123] = 1;
TABLE[1418][125] = 1;
TABLE[397][256] = 1;
TABLE[398][259] = 1;
TABLE[399][257] = 1;
TABLE[400][457] = 1;
TABLE[401][344] = 1;
TABLE[402][394] = 1;
TABLE[403][36] = 1;
TABLE[404][40] = 1;
TABLE[405][258] = 1;
TABLE[406][256] = 1;
TABLE[406][259] = 1;
TABLE[406][257] = 1;
TABLE[406][457] = 1;
TABLE[406][344] = 1;
TABLE[406][394] = 1;
TABLE[406][36] = 1;
TABLE[406][40] = 1;
TABLE[406][258] = 1;
TABLE[407][256] = 1;
TABLE[407][259] = 1;
TABLE[407][257] = 1;
TABLE[407][457] = 1;
TABLE[407][344] = 1;
TABLE[407][394] = 1;
TABLE[407][36] = 1;
TABLE[407][40] = 1;
TABLE[407][258] = 1;
TABLE[408][258] = 1;
TABLE[409][256] = 1;
TABLE[409][259] = 1;
TABLE[409][257] = 1;
TABLE[409][457] = 1;
TABLE[409][344] = 1;
TABLE[409][394] = 1;
TABLE[409][36] = 1;
TABLE[409][40] = 1;
TABLE[409][258] = 1;
TABLE[1011][46] = 1;
TABLE[1012][495] = 1;
TABLE[1013][46] = 1;
TABLE[1014][495] = 1;
TABLE[410][258] = 1;
TABLE[411][258] = 1;
TABLE[411][40] = 1;
TABLE[411][36] = 1;
TABLE[411][394] = 1;
TABLE[411][344] = 1;
TABLE[411][457] = 1;
TABLE[411][257] = 1;
TABLE[411][259] = 1;
TABLE[411][256] = 1;
TABLE[412][256] = 1;
TABLE[412][259] = 1;
TABLE[412][257] = 1;
TABLE[412][457] = 1;
TABLE[412][344] = 1;
TABLE[412][394] = 1;
TABLE[412][36] = 1;
TABLE[412][40] = 1;
TABLE[412][258] = 1;
TABLE[1015][46] = 1;
TABLE[1016][495] = 1;
TABLE[413][258] = 1;
TABLE[413][40] = 1;
TABLE[413][36] = 1;
TABLE[413][394] = 1;
TABLE[413][344] = 1;
TABLE[413][457] = 1;
TABLE[413][257] = 1;
TABLE[413][259] = 1;
TABLE[413][256] = 1;
TABLE[414][258] = 1;
TABLE[414][40] = 1;
TABLE[414][36] = 1;
TABLE[414][394] = 1;
TABLE[414][344] = 1;
TABLE[414][457] = 1;
TABLE[414][257] = 1;
TABLE[414][259] = 1;
TABLE[414][256] = 1;
TABLE[415][386] = 1;
TABLE[416][383] = 1;
TABLE[417][256] = 1;
TABLE[417][259] = 1;
TABLE[417][257] = 1;
TABLE[417][457] = 1;
TABLE[417][344] = 1;
TABLE[417][394] = 1;
TABLE[417][36] = 1;
TABLE[417][40] = 1;
TABLE[417][258] = 1;
TABLE[418][474] = 1;
TABLE[419][475] = 1;
TABLE[420][43] = 1;
TABLE[421][45] = 1;
TABLE[422][33] = 1;
TABLE[423][126] = 1;
TABLE[424][58] = 1;
TABLE[425][256] = 1;
TABLE[425][259] = 1;
TABLE[425][257] = 1;
TABLE[425][457] = 1;
TABLE[425][344] = 1;
TABLE[425][394] = 1;
TABLE[425][36] = 1;
TABLE[425][40] = 1;
TABLE[425][258] = 1;
TABLE[425][386] = 1;
TABLE[425][383] = 1;
TABLE[426][474] = 1;
TABLE[426][475] = 1;
TABLE[426][43] = 1;
TABLE[426][45] = 1;
TABLE[426][33] = 1;
TABLE[426][126] = 1;
TABLE[426][58] = 1;
TABLE[426][383] = 1;
TABLE[426][386] = 1;
TABLE[426][258] = 1;
TABLE[426][40] = 1;
TABLE[426][36] = 1;
TABLE[426][394] = 1;
TABLE[426][344] = 1;
TABLE[426][457] = 1;
TABLE[426][257] = 1;
TABLE[426][259] = 1;
TABLE[426][256] = 1;
TABLE[427][474] = 1;
TABLE[427][475] = 1;
TABLE[427][43] = 1;
TABLE[427][45] = 1;
TABLE[427][33] = 1;
TABLE[427][126] = 1;
TABLE[427][58] = 1;
TABLE[427][383] = 1;
TABLE[427][386] = 1;
TABLE[427][258] = 1;
TABLE[427][40] = 1;
TABLE[427][36] = 1;
TABLE[427][394] = 1;
TABLE[427][344] = 1;
TABLE[427][457] = 1;
TABLE[427][257] = 1;
TABLE[427][259] = 1;
TABLE[427][256] = 1;
TABLE[428][40] = 1;
TABLE[429][256] = 1;
TABLE[429][259] = 1;
TABLE[429][257] = 1;
TABLE[429][457] = 1;
TABLE[429][344] = 1;
TABLE[429][394] = 1;
TABLE[429][36] = 1;
TABLE[429][40] = 1;
TABLE[429][258] = 1;
TABLE[429][386] = 1;
TABLE[429][383] = 1;
TABLE[429][58] = 1;
TABLE[429][126] = 1;
TABLE[429][33] = 1;
TABLE[429][45] = 1;
TABLE[429][43] = 1;
TABLE[429][475] = 1;
TABLE[429][474] = 1;
TABLE[430][256] = 1;
TABLE[430][259] = 1;
TABLE[430][257] = 1;
TABLE[430][457] = 1;
TABLE[430][344] = 1;
TABLE[430][394] = 1;
TABLE[430][36] = 1;
TABLE[430][258] = 1;
TABLE[430][386] = 1;
TABLE[430][383] = 1;
TABLE[430][58] = 1;
TABLE[430][126] = 1;
TABLE[430][33] = 1;
TABLE[430][45] = 1;
TABLE[430][43] = 1;
TABLE[430][475] = 1;
TABLE[430][474] = 1;
TABLE[430][40] = 1;
TABLE[431][256] = 1;
TABLE[431][259] = 1;
TABLE[431][257] = 1;
TABLE[431][457] = 1;
TABLE[431][344] = 1;
TABLE[431][394] = 1;
TABLE[431][36] = 1;
TABLE[431][258] = 1;
TABLE[431][386] = 1;
TABLE[431][383] = 1;
TABLE[431][58] = 1;
TABLE[431][126] = 1;
TABLE[431][33] = 1;
TABLE[431][45] = 1;
TABLE[431][43] = 1;
TABLE[431][475] = 1;
TABLE[431][474] = 1;
TABLE[431][40] = 1;
TABLE[432][256] = 1;
TABLE[432][259] = 1;
TABLE[432][257] = 1;
TABLE[432][457] = 1;
TABLE[432][344] = 1;
TABLE[432][394] = 1;
TABLE[432][36] = 1;
TABLE[432][258] = 1;
TABLE[432][386] = 1;
TABLE[432][383] = 1;
TABLE[432][58] = 1;
TABLE[432][126] = 1;
TABLE[432][33] = 1;
TABLE[432][45] = 1;
TABLE[432][43] = 1;
TABLE[432][475] = 1;
TABLE[432][474] = 1;
TABLE[432][40] = 1;
TABLE[433][40] = 1;
TABLE[433][474] = 1;
TABLE[433][475] = 1;
TABLE[433][43] = 1;
TABLE[433][45] = 1;
TABLE[433][33] = 1;
TABLE[433][126] = 1;
TABLE[433][58] = 1;
TABLE[433][383] = 1;
TABLE[433][386] = 1;
TABLE[433][258] = 1;
TABLE[433][36] = 1;
TABLE[433][394] = 1;
TABLE[433][344] = 1;
TABLE[433][457] = 1;
TABLE[433][257] = 1;
TABLE[433][259] = 1;
TABLE[433][256] = 1;
TABLE[434][40] = 1;
TABLE[434][474] = 1;
TABLE[434][475] = 1;
TABLE[434][43] = 1;
TABLE[434][45] = 1;
TABLE[434][33] = 1;
TABLE[434][126] = 1;
TABLE[434][58] = 1;
TABLE[434][383] = 1;
TABLE[434][386] = 1;
TABLE[434][258] = 1;
TABLE[434][36] = 1;
TABLE[434][394] = 1;
TABLE[434][344] = 1;
TABLE[434][457] = 1;
TABLE[434][257] = 1;
TABLE[434][259] = 1;
TABLE[434][256] = 1;
TABLE[435][40] = 1;
TABLE[435][474] = 1;
TABLE[435][475] = 1;
TABLE[435][43] = 1;
TABLE[435][45] = 1;
TABLE[435][33] = 1;
TABLE[435][126] = 1;
TABLE[435][58] = 1;
TABLE[435][383] = 1;
TABLE[435][386] = 1;
TABLE[435][258] = 1;
TABLE[435][36] = 1;
TABLE[435][394] = 1;
TABLE[435][344] = 1;
TABLE[435][457] = 1;
TABLE[435][257] = 1;
TABLE[435][259] = 1;
TABLE[435][256] = 1;
TABLE[436][256] = 1;
TABLE[436][259] = 1;
TABLE[436][257] = 1;
TABLE[436][457] = 1;
TABLE[436][344] = 1;
TABLE[436][394] = 1;
TABLE[436][36] = 1;
TABLE[436][258] = 1;
TABLE[436][386] = 1;
TABLE[436][383] = 1;
TABLE[436][58] = 1;
TABLE[436][126] = 1;
TABLE[436][33] = 1;
TABLE[436][45] = 1;
TABLE[436][43] = 1;
TABLE[436][475] = 1;
TABLE[436][474] = 1;
TABLE[436][40] = 1;
TABLE[437][256] = 1;
TABLE[437][259] = 1;
TABLE[437][257] = 1;
TABLE[437][457] = 1;
TABLE[437][344] = 1;
TABLE[437][394] = 1;
TABLE[437][36] = 1;
TABLE[437][258] = 1;
TABLE[437][386] = 1;
TABLE[437][383] = 1;
TABLE[437][58] = 1;
TABLE[437][126] = 1;
TABLE[437][33] = 1;
TABLE[437][45] = 1;
TABLE[437][43] = 1;
TABLE[437][475] = 1;
TABLE[437][474] = 1;
TABLE[437][40] = 1;
TABLE[438][256] = 1;
TABLE[438][259] = 1;
TABLE[438][257] = 1;
TABLE[438][457] = 1;
TABLE[438][344] = 1;
TABLE[438][394] = 1;
TABLE[438][36] = 1;
TABLE[438][258] = 1;
TABLE[438][386] = 1;
TABLE[438][383] = 1;
TABLE[438][58] = 1;
TABLE[438][126] = 1;
TABLE[438][33] = 1;
TABLE[438][45] = 1;
TABLE[438][43] = 1;
TABLE[438][475] = 1;
TABLE[438][474] = 1;
TABLE[438][40] = 1;
TABLE[439][40] = 1;
TABLE[439][474] = 1;
TABLE[439][475] = 1;
TABLE[439][43] = 1;
TABLE[439][45] = 1;
TABLE[439][33] = 1;
TABLE[439][126] = 1;
TABLE[439][58] = 1;
TABLE[439][383] = 1;
TABLE[439][386] = 1;
TABLE[439][258] = 1;
TABLE[439][36] = 1;
TABLE[439][394] = 1;
TABLE[439][344] = 1;
TABLE[439][457] = 1;
TABLE[439][257] = 1;
TABLE[439][259] = 1;
TABLE[439][256] = 1;
TABLE[440][40] = 1;
TABLE[440][474] = 1;
TABLE[440][475] = 1;
TABLE[440][43] = 1;
TABLE[440][45] = 1;
TABLE[440][33] = 1;
TABLE[440][126] = 1;
TABLE[440][58] = 1;
TABLE[440][383] = 1;
TABLE[440][386] = 1;
TABLE[440][258] = 1;
TABLE[440][36] = 1;
TABLE[440][394] = 1;
TABLE[440][344] = 1;
TABLE[440][457] = 1;
TABLE[440][257] = 1;
TABLE[440][259] = 1;
TABLE[440][256] = 1;
TABLE[441][40] = 1;
TABLE[441][474] = 1;
TABLE[441][475] = 1;
TABLE[441][43] = 1;
TABLE[441][45] = 1;
TABLE[441][33] = 1;
TABLE[441][126] = 1;
TABLE[441][58] = 1;
TABLE[441][383] = 1;
TABLE[441][386] = 1;
TABLE[441][258] = 1;
TABLE[441][36] = 1;
TABLE[441][394] = 1;
TABLE[441][344] = 1;
TABLE[441][457] = 1;
TABLE[441][257] = 1;
TABLE[441][259] = 1;
TABLE[441][256] = 1;
TABLE[442][40] = 1;
TABLE[442][474] = 1;
TABLE[442][475] = 1;
TABLE[442][43] = 1;
TABLE[442][45] = 1;
TABLE[442][33] = 1;
TABLE[442][126] = 1;
TABLE[442][58] = 1;
TABLE[442][383] = 1;
TABLE[442][386] = 1;
TABLE[442][258] = 1;
TABLE[442][36] = 1;
TABLE[442][394] = 1;
TABLE[442][344] = 1;
TABLE[442][457] = 1;
TABLE[442][257] = 1;
TABLE[442][259] = 1;
TABLE[442][256] = 1;
TABLE[443][40] = 1;
TABLE[443][474] = 1;
TABLE[443][475] = 1;
TABLE[443][43] = 1;
TABLE[443][45] = 1;
TABLE[443][33] = 1;
TABLE[443][126] = 1;
TABLE[443][58] = 1;
TABLE[443][383] = 1;
TABLE[443][386] = 1;
TABLE[443][258] = 1;
TABLE[443][36] = 1;
TABLE[443][394] = 1;
TABLE[443][344] = 1;
TABLE[443][457] = 1;
TABLE[443][257] = 1;
TABLE[443][259] = 1;
TABLE[443][256] = 1;
TABLE[444][40] = 1;
TABLE[444][474] = 1;
TABLE[444][475] = 1;
TABLE[444][43] = 1;
TABLE[444][45] = 1;
TABLE[444][33] = 1;
TABLE[444][126] = 1;
TABLE[444][58] = 1;
TABLE[444][383] = 1;
TABLE[444][386] = 1;
TABLE[444][258] = 1;
TABLE[444][36] = 1;
TABLE[444][394] = 1;
TABLE[444][344] = 1;
TABLE[444][457] = 1;
TABLE[444][257] = 1;
TABLE[444][259] = 1;
TABLE[444][256] = 1;
TABLE[445][40] = 1;
TABLE[445][474] = 1;
TABLE[445][475] = 1;
TABLE[445][43] = 1;
TABLE[445][45] = 1;
TABLE[445][33] = 1;
TABLE[445][126] = 1;
TABLE[445][58] = 1;
TABLE[445][383] = 1;
TABLE[445][386] = 1;
TABLE[445][258] = 1;
TABLE[445][36] = 1;
TABLE[445][394] = 1;
TABLE[445][344] = 1;
TABLE[445][457] = 1;
TABLE[445][257] = 1;
TABLE[445][259] = 1;
TABLE[445][256] = 1;
TABLE[446][256] = 1;
TABLE[446][259] = 1;
TABLE[446][257] = 1;
TABLE[446][457] = 1;
TABLE[446][344] = 1;
TABLE[446][394] = 1;
TABLE[446][36] = 1;
TABLE[446][258] = 1;
TABLE[446][386] = 1;
TABLE[446][383] = 1;
TABLE[446][58] = 1;
TABLE[446][126] = 1;
TABLE[446][33] = 1;
TABLE[446][45] = 1;
TABLE[446][43] = 1;
TABLE[446][475] = 1;
TABLE[446][474] = 1;
TABLE[446][40] = 1;
TABLE[447][256] = 1;
TABLE[447][259] = 1;
TABLE[447][257] = 1;
TABLE[447][457] = 1;
TABLE[447][344] = 1;
TABLE[447][394] = 1;
TABLE[447][36] = 1;
TABLE[447][258] = 1;
TABLE[447][386] = 1;
TABLE[447][383] = 1;
TABLE[447][58] = 1;
TABLE[447][126] = 1;
TABLE[447][33] = 1;
TABLE[447][45] = 1;
TABLE[447][43] = 1;
TABLE[447][475] = 1;
TABLE[447][474] = 1;
TABLE[447][40] = 1;
TABLE[448][40] = 1;
TABLE[448][474] = 1;
TABLE[448][475] = 1;
TABLE[448][43] = 1;
TABLE[448][45] = 1;
TABLE[448][33] = 1;
TABLE[448][126] = 1;
TABLE[448][58] = 1;
TABLE[448][383] = 1;
TABLE[448][386] = 1;
TABLE[448][258] = 1;
TABLE[448][36] = 1;
TABLE[448][394] = 1;
TABLE[448][344] = 1;
TABLE[448][457] = 1;
TABLE[448][257] = 1;
TABLE[448][259] = 1;
TABLE[448][256] = 1;
TABLE[449][40] = 1;
TABLE[449][474] = 1;
TABLE[449][475] = 1;
TABLE[449][43] = 1;
TABLE[449][45] = 1;
TABLE[449][33] = 1;
TABLE[449][126] = 1;
TABLE[449][58] = 1;
TABLE[449][383] = 1;
TABLE[449][386] = 1;
TABLE[449][258] = 1;
TABLE[449][36] = 1;
TABLE[449][394] = 1;
TABLE[449][344] = 1;
TABLE[449][457] = 1;
TABLE[449][257] = 1;
TABLE[449][259] = 1;
TABLE[449][256] = 1;
TABLE[450][256] = 1;
TABLE[450][259] = 1;
TABLE[450][257] = 1;
TABLE[450][457] = 1;
TABLE[450][344] = 1;
TABLE[450][394] = 1;
TABLE[450][36] = 1;
TABLE[450][258] = 1;
TABLE[450][386] = 1;
TABLE[450][383] = 1;
TABLE[450][58] = 1;
TABLE[450][126] = 1;
TABLE[450][33] = 1;
TABLE[450][45] = 1;
TABLE[450][43] = 1;
TABLE[450][475] = 1;
TABLE[450][474] = 1;
TABLE[450][40] = 1;
TABLE[451][256] = 1;
TABLE[451][259] = 1;
TABLE[451][257] = 1;
TABLE[451][457] = 1;
TABLE[451][344] = 1;
TABLE[451][394] = 1;
TABLE[451][36] = 1;
TABLE[451][258] = 1;
TABLE[451][386] = 1;
TABLE[451][383] = 1;
TABLE[451][58] = 1;
TABLE[451][126] = 1;
TABLE[451][33] = 1;
TABLE[451][45] = 1;
TABLE[451][43] = 1;
TABLE[451][475] = 1;
TABLE[451][474] = 1;
TABLE[451][40] = 1;
TABLE[452][40] = 1;
TABLE[452][474] = 1;
TABLE[452][475] = 1;
TABLE[452][43] = 1;
TABLE[452][45] = 1;
TABLE[452][33] = 1;
TABLE[452][126] = 1;
TABLE[452][58] = 1;
TABLE[452][383] = 1;
TABLE[452][386] = 1;
TABLE[452][258] = 1;
TABLE[452][36] = 1;
TABLE[452][394] = 1;
TABLE[452][344] = 1;
TABLE[452][457] = 1;
TABLE[452][257] = 1;
TABLE[452][259] = 1;
TABLE[452][256] = 1;
TABLE[453][40] = 1;
TABLE[453][474] = 1;
TABLE[453][475] = 1;
TABLE[453][43] = 1;
TABLE[453][45] = 1;
TABLE[453][33] = 1;
TABLE[453][126] = 1;
TABLE[453][58] = 1;
TABLE[453][383] = 1;
TABLE[453][386] = 1;
TABLE[453][258] = 1;
TABLE[453][36] = 1;
TABLE[453][394] = 1;
TABLE[453][344] = 1;
TABLE[453][457] = 1;
TABLE[453][257] = 1;
TABLE[453][259] = 1;
TABLE[453][256] = 1;
TABLE[454][256] = 1;
TABLE[454][259] = 1;
TABLE[454][257] = 1;
TABLE[454][457] = 1;
TABLE[454][344] = 1;
TABLE[454][394] = 1;
TABLE[454][36] = 1;
TABLE[454][258] = 1;
TABLE[454][386] = 1;
TABLE[454][383] = 1;
TABLE[454][58] = 1;
TABLE[454][126] = 1;
TABLE[454][33] = 1;
TABLE[454][45] = 1;
TABLE[454][43] = 1;
TABLE[454][475] = 1;
TABLE[454][474] = 1;
TABLE[454][40] = 1;
TABLE[455][256] = 1;
TABLE[455][259] = 1;
TABLE[455][257] = 1;
TABLE[455][457] = 1;
TABLE[455][344] = 1;
TABLE[455][394] = 1;
TABLE[455][36] = 1;
TABLE[455][258] = 1;
TABLE[455][386] = 1;
TABLE[455][383] = 1;
TABLE[455][58] = 1;
TABLE[455][126] = 1;
TABLE[455][33] = 1;
TABLE[455][45] = 1;
TABLE[455][43] = 1;
TABLE[455][475] = 1;
TABLE[455][474] = 1;
TABLE[455][40] = 1;
TABLE[456][40] = 1;
TABLE[456][474] = 1;
TABLE[456][475] = 1;
TABLE[456][43] = 1;
TABLE[456][45] = 1;
TABLE[456][33] = 1;
TABLE[456][126] = 1;
TABLE[456][58] = 1;
TABLE[456][383] = 1;
TABLE[456][386] = 1;
TABLE[456][258] = 1;
TABLE[456][36] = 1;
TABLE[456][394] = 1;
TABLE[456][344] = 1;
TABLE[456][457] = 1;
TABLE[456][257] = 1;
TABLE[456][259] = 1;
TABLE[456][256] = 1;
TABLE[457][256] = 1;
TABLE[457][259] = 1;
TABLE[457][257] = 1;
TABLE[457][457] = 1;
TABLE[457][344] = 1;
TABLE[457][394] = 1;
TABLE[457][36] = 1;
TABLE[457][258] = 1;
TABLE[457][386] = 1;
TABLE[457][383] = 1;
TABLE[457][58] = 1;
TABLE[457][126] = 1;
TABLE[457][33] = 1;
TABLE[457][45] = 1;
TABLE[457][43] = 1;
TABLE[457][475] = 1;
TABLE[457][474] = 1;
TABLE[457][40] = 1;
TABLE[458][256] = 1;
TABLE[458][259] = 1;
TABLE[458][257] = 1;
TABLE[458][457] = 1;
TABLE[458][344] = 1;
TABLE[458][394] = 1;
TABLE[458][36] = 1;
TABLE[458][258] = 1;
TABLE[458][386] = 1;
TABLE[458][383] = 1;
TABLE[458][58] = 1;
TABLE[458][126] = 1;
TABLE[458][33] = 1;
TABLE[458][45] = 1;
TABLE[458][43] = 1;
TABLE[458][475] = 1;
TABLE[458][474] = 1;
TABLE[458][40] = 1;
TABLE[459][258] = 1;
TABLE[459][40] = 1;
TABLE[459][36] = 1;
TABLE[459][394] = 1;
TABLE[459][344] = 1;
TABLE[459][457] = 1;
TABLE[459][257] = 1;
TABLE[459][259] = 1;
TABLE[459][256] = 1;
TABLE[460][258] = 1;
TABLE[460][40] = 1;
TABLE[460][36] = 1;
TABLE[460][394] = 1;
TABLE[460][344] = 1;
TABLE[460][457] = 1;
TABLE[460][257] = 1;
TABLE[460][259] = 1;
TABLE[460][256] = 1;
TABLE[461][258] = 1;
TABLE[461][40] = 1;
TABLE[461][36] = 1;
TABLE[461][394] = 1;
TABLE[461][344] = 1;
TABLE[461][457] = 1;
TABLE[461][257] = 1;
TABLE[461][259] = 1;
TABLE[461][256] = 1;
TABLE[462][258] = 1;
TABLE[462][40] = 1;
TABLE[462][36] = 1;
TABLE[462][394] = 1;
TABLE[462][344] = 1;
TABLE[462][457] = 1;
TABLE[462][257] = 1;
TABLE[462][259] = 1;
TABLE[462][256] = 1;
TABLE[463][258] = 1;
TABLE[463][40] = 1;
TABLE[463][36] = 1;
TABLE[463][394] = 1;
TABLE[463][344] = 1;
TABLE[463][457] = 1;
TABLE[463][257] = 1;
TABLE[463][259] = 1;
TABLE[463][256] = 1;
TABLE[464][258] = 1;
TABLE[464][40] = 1;
TABLE[464][36] = 1;
TABLE[464][394] = 1;
TABLE[464][344] = 1;
TABLE[464][457] = 1;
TABLE[464][257] = 1;
TABLE[464][259] = 1;
TABLE[464][256] = 1;
TABLE[465][258] = 1;
TABLE[465][40] = 1;
TABLE[465][36] = 1;
TABLE[465][394] = 1;
TABLE[465][344] = 1;
TABLE[465][457] = 1;
TABLE[465][257] = 1;
TABLE[465][259] = 1;
TABLE[465][256] = 1;
TABLE[466][258] = 1;
TABLE[466][40] = 1;
TABLE[466][36] = 1;
TABLE[466][394] = 1;
TABLE[466][344] = 1;
TABLE[466][457] = 1;
TABLE[466][257] = 1;
TABLE[466][259] = 1;
TABLE[466][256] = 1;
TABLE[467][258] = 1;
TABLE[467][40] = 1;
TABLE[467][36] = 1;
TABLE[467][394] = 1;
TABLE[467][344] = 1;
TABLE[467][457] = 1;
TABLE[467][257] = 1;
TABLE[467][259] = 1;
TABLE[467][256] = 1;
TABLE[468][258] = 1;
TABLE[468][40] = 1;
TABLE[468][36] = 1;
TABLE[468][394] = 1;
TABLE[468][344] = 1;
TABLE[468][457] = 1;
TABLE[468][257] = 1;
TABLE[468][259] = 1;
TABLE[468][256] = 1;
TABLE[469][258] = 1;
TABLE[469][40] = 1;
TABLE[469][36] = 1;
TABLE[469][394] = 1;
TABLE[469][344] = 1;
TABLE[469][457] = 1;
TABLE[469][257] = 1;
TABLE[469][259] = 1;
TABLE[469][256] = 1;
TABLE[470][40] = 1;
TABLE[470][474] = 1;
TABLE[470][475] = 1;
TABLE[470][43] = 1;
TABLE[470][45] = 1;
TABLE[470][33] = 1;
TABLE[470][126] = 1;
TABLE[470][58] = 1;
TABLE[470][383] = 1;
TABLE[470][386] = 1;
TABLE[470][258] = 1;
TABLE[470][36] = 1;
TABLE[470][394] = 1;
TABLE[470][344] = 1;
TABLE[470][457] = 1;
TABLE[470][257] = 1;
TABLE[470][259] = 1;
TABLE[470][256] = 1;
TABLE[471][474] = 1;
TABLE[471][475] = 1;
TABLE[471][43] = 1;
TABLE[471][45] = 1;
TABLE[471][33] = 1;
TABLE[471][126] = 1;
TABLE[471][58] = 1;
TABLE[471][383] = 1;
TABLE[471][386] = 1;
TABLE[471][258] = 1;
TABLE[471][40] = 1;
TABLE[471][36] = 1;
TABLE[471][394] = 1;
TABLE[471][344] = 1;
TABLE[471][457] = 1;
TABLE[471][257] = 1;
TABLE[471][259] = 1;
TABLE[471][256] = 1;
TABLE[472][256] = 1;
TABLE[472][259] = 1;
TABLE[472][257] = 1;
TABLE[472][457] = 1;
TABLE[472][344] = 1;
TABLE[472][394] = 1;
TABLE[472][36] = 1;
TABLE[472][40] = 1;
TABLE[472][258] = 1;
TABLE[472][386] = 1;
TABLE[472][383] = 1;
TABLE[472][58] = 1;
TABLE[472][126] = 1;
TABLE[472][33] = 1;
TABLE[472][45] = 1;
TABLE[472][43] = 1;
TABLE[472][475] = 1;
TABLE[472][474] = 1;
TABLE[473][41] = 1;
TABLE[473][474] = 1;
TABLE[473][475] = 1;
TABLE[473][43] = 1;
TABLE[473][45] = 1;
TABLE[473][33] = 1;
TABLE[473][126] = 1;
TABLE[473][58] = 1;
TABLE[473][383] = 1;
TABLE[473][386] = 1;
TABLE[473][258] = 1;
TABLE[473][40] = 1;
TABLE[473][36] = 1;
TABLE[473][394] = 1;
TABLE[473][344] = 1;
TABLE[473][457] = 1;
TABLE[473][257] = 1;
TABLE[473][259] = 1;
TABLE[473][256] = 1;
TABLE[1018][256] = 1;
TABLE[1018][259] = 1;
TABLE[1018][257] = 1;
TABLE[1018][457] = 1;
TABLE[1018][344] = 1;
TABLE[1018][394] = 1;
TABLE[1018][36] = 1;
TABLE[1018][40] = 1;
TABLE[1018][258] = 1;
TABLE[1018][386] = 1;
TABLE[1018][383] = 1;
TABLE[1018][58] = 1;
TABLE[1018][126] = 1;
TABLE[1018][33] = 1;
TABLE[1018][45] = 1;
TABLE[1018][43] = 1;
TABLE[1018][475] = 1;
TABLE[1018][474] = 1;
TABLE[1420][41] = 1;
TABLE[1017][44] = 1;
TABLE[1419][41] = 1;
}

extern int yydirset(int,int);
int yydirset(int i,int j)
   /* int i,j; */
{
   return TABLE[i][j];
}
char * yyprintname(int n)
   /* int n; */
{
   if (n <= 50000)
      switch(n) {
         case 1: return "YYSTART"; break;
         case 6: return "file"; break;
         case 10: return "Subphrase"; break;
         case 18: return "resource"; break;
         case 144: return "Subphrase"; break;
         case 148: return "Subphrase"; break;
         case 156: return "Subphrase"; break;
         case 163: return "res_ref"; break;
         case 173: return "cexpr"; break;
         case 189: return "cexpr2"; break;
         case 223: return "cexpr3"; break;
         case 237: return "cexpr4"; break;
         case 259: return "dice_val"; break;
         case 286: return "dice_only"; break;
         case 300: return "gear_entry"; break;
         case 310: return "Subphrase"; break;
         case 320: return "Subphrase"; break;
         case 328: return "Subphrase"; break;
         case 335: return "Subphrase"; break;
         case 346: return "Subphrase"; break;
         case 355: return "Subphrase"; break;
         case 368: return "Subphrase"; break;
         case 417: return "Subphrase"; break;
         case 426: return "Subphrase"; break;
         case 433: return "gear_desc"; break;
         case 441: return "Subphrase"; break;
         case 445: return "Subphrase"; break;
         case 454: return "Subphrase"; break;
         case 462: return "glyph_entry"; break;
         case 471: return "Subphrase"; break;
         case 479: return "Subphrase"; break;
         case 486: return "Subphrase"; break;
         case 493: return "Subphrase"; break;
         case 501: return "Subphrase"; break;
         case 509: return "glyph_desc"; break;
         case 515: return "Subphrase"; break;
         case 522: return "Subphrase"; break;
         case 530: return "Subphrase"; break;
         case 538: return "event_desc"; break;
         case 546: return "Subphrase"; break;
         case 555: return "event_entry"; break;
         case 561: return "Subphrase"; break;
         case 570: return "Subphrase"; break;
         case 579: return "Subphrase"; break;
         case 588: return "sep_expr"; break;
         case 592: return "sep_stat"; break;
         case 596: return "mval"; break;
         case 601: return "Subphrase"; break;
         case 621: return "Subphrase"; break;
         case 628: return "Subphrase"; break;
         case 637: return "Subphrase"; break;
         case 652: return "monster_def"; break;
         case 663: return "Subphrase"; break;
         case 672: return "Subphrase"; break;
         case 680: return "Subphrase"; break;
         case 688: return "mon_entry"; break;
         case 932: return "Subphrase"; break;
         case 939: return "Subphrase"; break;
         case 946: return "Subphrase"; break;
         case 954: return "Subphrase"; break;
         case 961: return "Subphrase"; break;
         case 969: return "Subphrase"; break;
         case 976: return "Subphrase"; break;
         case 983: return "Subphrase"; break;
         case 992: return "Subphrase"; break;
         case 999: return "Subphrase"; break;
         case 1006: return "Subphrase"; break;
         case 1013: return "Subphrase"; break;
         case 1020: return "Subphrase"; break;
         case 1027: return "Subphrase"; break;
         case 1036: return "Subphrase"; break;
         case 1043: return "Subphrase"; break;
         case 1050: return "Subphrase"; break;
         case 1059: return "Subphrase"; break;
         case 1066: return "Subphrase"; break;
         case 1073: return "Subphrase"; break;
         case 1082: return "Subphrase"; break;
         case 1089: return "Subphrase"; break;
         case 1096: return "Subphrase"; break;
         case 1103: return "Subphrase"; break;
         case 1110: return "Subphrase"; break;
         case 1119: return "Subphrase"; break;
         case 1126: return "Subphrase"; break;
         case 1134: return "Subphrase"; break;
         case 1143: return "Subphrase"; break;
         case 1150: return "Subphrase"; break;
         case 1157: return "Subphrase"; break;
         case 1164: return "Subphrase"; break;
         case 1171: return "Subphrase"; break;
         case 1178: return "Subphrase"; break;
         case 1185: return "Subphrase"; break;
         case 1194: return "Subphrase"; break;
         case 1201: return "Subphrase"; break;
         case 1208: return "Subphrase"; break;
         case 1217: return "Subphrase"; break;
         case 1224: return "Subphrase"; break;
         case 1231: return "Subphrase"; break;
         case 1240: return "Subphrase"; break;
         case 1247: return "color"; break;
         case 1266: return "attack_entry"; break;
         case 1283: return "Subphrase"; break;
         case 1295: return "Subphrase"; break;
         case 1302: return "Subphrase"; break;
         case 1309: return "Subphrase"; break;
         case 1319: return "Subphrase"; break;
         case 1331: return "Subphrase"; break;
         case 1343: return "Subphrase"; break;
         case 1350: return "Subphrase"; break;
         case 1357: return "Subphrase"; break;
         case 1364: return "Subphrase"; break;
         case 1374: return "Subphrase"; break;
         case 1386: return "Subphrase"; break;
         case 1393: return "mflag_entry"; break;
         case 1404: return "Subphrase"; break;
         case 1411: return "Subphrase"; break;
         case 1418: return "attrib_entry"; break;
         case 1424: return "Subphrase"; break;
         case 1431: return "item_def"; break;
         case 1441: return "Subphrase"; break;
         case 1449: return "item_entry"; break;
         case 1715: return "Subphrase"; break;
         case 1722: return "Subphrase"; break;
         case 1729: return "Subphrase"; break;
         case 1736: return "Subphrase"; break;
         case 1743: return "Subphrase"; break;
         case 1750: return "Subphrase"; break;
         case 1757: return "Subphrase"; break;
         case 1764: return "Subphrase"; break;
         case 1771: return "Subphrase"; break;
         case 1778: return "Subphrase"; break;
         case 1785: return "Subphrase"; break;
         case 1792: return "Subphrase"; break;
         case 1799: return "Subphrase"; break;
         case 1806: return "Subphrase"; break;
         case 1813: return "Subphrase"; break;
         case 1820: return "Subphrase"; break;
         case 1827: return "Subphrase"; break;
         case 1834: return "Subphrase"; break;
         case 1841: return "Subphrase"; break;
         case 1848: return "Subphrase"; break;
         case 1855: return "Subphrase"; break;
         case 1862: return "Subphrase"; break;
         case 1869: return "Subphrase"; break;
         case 1877: return "Subphrase"; break;
         case 1884: return "Subphrase"; break;
         case 1891: return "Subphrase"; break;
         case 1900: return "Subphrase"; break;
         case 1907: return "Subphrase"; break;
         case 1914: return "Subphrase"; break;
         case 1923: return "Subphrase"; break;
         case 1930: return "Subphrase"; break;
         case 1937: return "Subphrase"; break;
         case 1945: return "Subphrase"; break;
         case 1952: return "Subphrase"; break;
         case 1961: return "Subphrase"; break;
         case 1972: return "Subphrase"; break;
         case 1979: return "Subphrase"; break;
         case 1986: return "Subphrase"; break;
         case 1993: return "Subphrase"; break;
         case 2000: return "Subphrase"; break;
         case 2007: return "Subphrase"; break;
         case 2014: return "Subphrase"; break;
         case 2021: return "Subphrase"; break;
         case 2028: return "Subphrase"; break;
         case 2035: return "arm_entry"; break;
         case 2046: return "Subphrase"; break;
         case 2053: return "Subphrase"; break;
         case 2060: return "iflag_entry"; break;
         case 2071: return "Subphrase"; break;
         case 2078: return "Subphrase"; break;
         case 2085: return "feature_def"; break;
         case 2095: return "Subphrase"; break;
         case 2103: return "feat_entry"; break;
         case 2176: return "Subphrase"; break;
         case 2183: return "Subphrase"; break;
         case 2190: return "Subphrase"; break;
         case 2197: return "Subphrase"; break;
         case 2204: return "Subphrase"; break;
         case 2211: return "Subphrase"; break;
         case 2220: return "Subphrase"; break;
         case 2231: return "Subphrase"; break;
         case 2239: return "Subphrase"; break;
         case 2246: return "Subphrase"; break;
         case 2253: return "Subphrase"; break;
         case 2260: return "effect_def"; break;
         case 2272: return "Subphrase"; break;
         case 2281: return "Subphrase"; break;
         case 2288: return "Subphrase"; break;
         case 2304: return "Subphrase"; break;
         case 2312: return "Subphrase"; break;
         case 2324: return "Subphrase"; break;
         case 2332: return "eff_entry"; break;
         case 2486: return "Subphrase"; break;
         case 2493: return "Subphrase"; break;
         case 2500: return "Subphrase"; break;
         case 2507: return "Subphrase"; break;
         case 2514: return "Subphrase"; break;
         case 2521: return "Subphrase"; break;
         case 2528: return "Subphrase"; break;
         case 2535: return "Subphrase"; break;
         case 2544: return "Subphrase"; break;
         case 2551: return "Subphrase"; break;
         case 2558: return "Subphrase"; break;
         case 2565: return "Subphrase"; break;
         case 2572: return "Subphrase"; break;
         case 2579: return "Subphrase"; break;
         case 2586: return "Subphrase"; break;
         case 2593: return "Subphrase"; break;
         case 2600: return "Subphrase"; break;
         case 2607: return "Subphrase"; break;
         case 2614: return "Subphrase"; break;
         case 2621: return "Subphrase"; break;
         case 2628: return "Subphrase"; break;
         case 2635: return "Subphrase"; break;
         case 2642: return "Subphrase"; break;
         case 2649: return "Subphrase"; break;
         case 2658: return "Subphrase"; break;
         case 2669: return "numlist"; break;
         case 2673: return "Subphrase"; break;
         case 2682: return "Subphrase"; break;
         case 2693: return "object_entry"; break;
         case 2697: return "race_def"; break;
         case 2706: return "Subphrase"; break;
         case 2714: return "Subphrase"; break;
         case 2722: return "race_entry"; break;
         case 2807: return "Subphrase"; break;
         case 2814: return "Subphrase"; break;
         case 2825: return "Subphrase"; break;
         case 2832: return "Subphrase"; break;
         case 2839: return "Subphrase"; break;
         case 2846: return "Subphrase"; break;
         case 2855: return "Subphrase"; break;
         case 2862: return "Subphrase"; break;
         case 2869: return "Subphrase"; break;
         case 2876: return "Subphrase"; break;
         case 2883: return "Subphrase"; break;
         case 2890: return "Subphrase"; break;
         case 2897: return "Subphrase"; break;
         case 2904: return "Subphrase"; break;
         case 2913: return "Subphrase"; break;
         case 2920: return "Subphrase"; break;
         case 2927: return "Subphrase"; break;
         case 2934: return "Subphrase"; break;
         case 2943: return "Subphrase"; break;
         case 2954: return "class_def"; break;
         case 2962: return "Subphrase"; break;
         case 2970: return "class_entry"; break;
         case 3063: return "Subphrase"; break;
         case 3070: return "Subphrase"; break;
         case 3077: return "Subphrase"; break;
         case 3084: return "Subphrase"; break;
         case 3091: return "Subphrase"; break;
         case 3098: return "Subphrase"; break;
         case 3105: return "Subphrase"; break;
         case 3112: return "Subphrase"; break;
         case 3122: return "Subphrase"; break;
         case 3129: return "Subphrase"; break;
         case 3136: return "Subphrase"; break;
         case 3146: return "Subphrase"; break;
         case 3153: return "Subphrase"; break;
         case 3160: return "Subphrase"; break;
         case 3169: return "Subphrase"; break;
         case 3176: return "Subphrase"; break;
         case 3180: return "Subphrase"; break;
         case 3184: return "Subphrase"; break;
         case 3191: return "Subphrase"; break;
         case 3200: return "Subphrase"; break;
         case 3207: return "Subphrase"; break;
         case 3214: return "Subphrase"; break;
         case 3223: return "Subphrase"; break;
         case 3230: return "Subphrase"; break;
         case 3237: return "Subphrase"; break;
         case 3246: return "Subphrase"; break;
         case 3257: return "special_ability"; break;
         case 3287: return "Subphrase"; break;
         case 3294: return "Subphrase"; break;
         case 3301: return "Subphrase"; break;
         case 3308: return "abil_param"; break;
         case 3325: return "abil_level"; break;
         case 3360: return "Subphrase"; break;
         case 3367: return "Subphrase"; break;
         case 3374: return "Subphrase"; break;
         case 3381: return "dungeon_def"; break;
         case 3389: return "Subphrase"; break;
         case 3397: return "dun_entry"; break;
         case 3416: return "Subphrase"; break;
         case 3423: return "dconst_entry"; break;
         case 3443: return "Subphrase"; break;
         case 3450: return "Subphrase"; break;
         case 3469: return "Subphrase"; break;
         case 3476: return "Subphrase"; break;
         case 3483: return "Subphrase"; break;
         case 3490: return "Subphrase"; break;
         case 3501: return "Subphrase"; break;
         case 3509: return "Subphrase"; break;
         case 3517: return "Subphrase"; break;
         case 3524: return "Subphrase"; break;
         case 3535: return "Subphrase"; break;
         case 3543: return "Subphrase"; break;
         case 3564: return "Subphrase"; break;
         case 3571: return "list_entry"; break;
         case 3583: return "artifact_def"; break;
         case 3591: return "Subphrase"; break;
         case 3599: return "arti_entry"; break;
         case 3675: return "Subphrase"; break;
         case 3682: return "Subphrase"; break;
         case 3689: return "Subphrase"; break;
         case 3700: return "Subphrase"; break;
         case 3707: return "Subphrase"; break;
         case 3714: return "Subphrase"; break;
         case 3721: return "Subphrase"; break;
         case 3730: return "Subphrase"; break;
         case 3737: return "Subphrase"; break;
         case 3744: return "Subphrase"; break;
         case 3753: return "Subphrase"; break;
         case 3760: return "Subphrase"; break;
         case 3767: return "Subphrase"; break;
         case 3776: return "Subphrase"; break;
         case 3783: return "Subphrase"; break;
         case 3792: return "Subphrase"; break;
         case 3796: return "Subphrase"; break;
         case 3805: return "Subphrase"; break;
         case 3809: return "Subphrase"; break;
         case 3818: return "Subphrase"; break;
         case 3822: return "Subphrase"; break;
         case 3831: return "Subphrase"; break;
         case 3835: return "art_power"; break;
         case 3887: return "Subphrase"; break;
         case 3895: return "Subphrase"; break;
         case 3903: return "Subphrase"; break;
         case 3911: return "domain_def"; break;
         case 3921: return "Subphrase"; break;
         case 3929: return "domain_entry"; break;
         case 3962: return "Subphrase"; break;
         case 3969: return "Subphrase"; break;
         case 3978: return "Subphrase"; break;
         case 3985: return "Subphrase"; break;
         case 3992: return "Subphrase"; break;
         case 4001: return "Subphrase"; break;
         case 4008: return "Subphrase"; break;
         case 4015: return "god_def"; break;
         case 4023: return "Subphrase"; break;
         case 4031: return "god_entry"; break;
         case 4076: return "Subphrase"; break;
         case 4083: return "Subphrase"; break;
         case 4092: return "Subphrase"; break;
         case 4099: return "Subphrase"; break;
         case 4106: return "Subphrase"; break;
         case 4115: return "Subphrase"; break;
         case 4126: return "Subphrase"; break;
         case 4133: return "Subphrase"; break;
         case 4140: return "Subphrase"; break;
         case 4149: return "Subphrase"; break;
         case 4156: return "terrain_def"; break;
         case 4164: return "Subphrase"; break;
         case 4172: return "terrain_entry"; break;
         case 4224: return "Subphrase"; break;
         case 4231: return "Subphrase"; break;
         case 4238: return "Subphrase"; break;
         case 4245: return "Subphrase"; break;
         case 4252: return "Subphrase"; break;
         case 4259: return "Subphrase"; break;
         case 4268: return "Subphrase"; break;
         case 4279: return "region_def"; break;
         case 4289: return "Subphrase"; break;
         case 4297: return "region_entry"; break;
         case 4391: return "Subphrase"; break;
         case 4398: return "Subphrase"; break;
         case 4405: return "Subphrase"; break;
         case 4412: return "Subphrase"; break;
         case 4419: return "Subphrase"; break;
         case 4426: return "Subphrase"; break;
         case 4433: return "Subphrase"; break;
         case 4440: return "Subphrase"; break;
         case 4449: return "Subphrase"; break;
         case 4460: return "Subphrase"; break;
         case 4467: return "Subphrase"; break;
         case 4476: return "Subphrase"; break;
         case 4487: return "Subphrase"; break;
         case 4499: return "Subphrase"; break;
         case 4522: return "Subphrase"; break;
         case 4538: return "Subphrase"; break;
         case 4545: return "Subphrase"; break;
         case 4552: return "Subphrase"; break;
         case 4573: return "Subphrase"; break;
         case 4580: return "Subphrase"; break;
         case 4588: return "Subphrase"; break;
         case 4596: return "template_def"; break;
         case 4607: return "Subphrase"; break;
         case 4615: return "Subphrase"; break;
         case 4623: return "template_entry"; break;
         case 4850: return "Subphrase"; break;
         case 4857: return "Subphrase"; break;
         case 4864: return "Subphrase"; break;
         case 4871: return "Subphrase"; break;
         case 4879: return "Subphrase"; break;
         case 4886: return "Subphrase"; break;
         case 4893: return "Subphrase"; break;
         case 4900: return "Subphrase"; break;
         case 4907: return "Subphrase"; break;
         case 4914: return "Subphrase"; break;
         case 4921: return "Subphrase"; break;
         case 4928: return "Subphrase"; break;
         case 4935: return "Subphrase"; break;
         case 4942: return "Subphrase"; break;
         case 4949: return "Subphrase"; break;
         case 4956: return "Subphrase"; break;
         case 4963: return "Subphrase"; break;
         case 4971: return "Subphrase"; break;
         case 4978: return "Subphrase"; break;
         case 4987: return "Subphrase"; break;
         case 4994: return "Subphrase"; break;
         case 5003: return "Subphrase"; break;
         case 5012: return "Subphrase"; break;
         case 5020: return "Subphrase"; break;
         case 5027: return "Subphrase"; break;
         case 5034: return "Subphrase"; break;
         case 5043: return "Subphrase"; break;
         case 5052: return "Subphrase"; break;
         case 5060: return "Subphrase"; break;
         case 5067: return "Subphrase"; break;
         case 5074: return "Subphrase"; break;
         case 5083: return "Subphrase"; break;
         case 5090: return "Subphrase"; break;
         case 5099: return "Subphrase"; break;
         case 5106: return "Subphrase"; break;
         case 5113: return "Subphrase"; break;
         case 5122: return "Subphrase"; break;
         case 5129: return "Subphrase"; break;
         case 5136: return "Subphrase"; break;
         case 5145: return "Subphrase"; break;
         case 5152: return "Subphrase"; break;
         case 5159: return "Subphrase"; break;
         case 5168: return "Subphrase"; break;
         case 5177: return "Subphrase"; break;
         case 5185: return "Subphrase"; break;
         case 5193: return "temp_attack_entry"; break;
         case 5210: return "Subphrase"; break;
         case 5222: return "Subphrase"; break;
         case 5229: return "Subphrase"; break;
         case 5236: return "Subphrase"; break;
         case 5246: return "Subphrase"; break;
         case 5258: return "Subphrase"; break;
         case 5270: return "Subphrase"; break;
         case 5277: return "Subphrase"; break;
         case 5284: return "Subphrase"; break;
         case 5291: return "Subphrase"; break;
         case 5301: return "Subphrase"; break;
         case 5313: return "Subphrase"; break;
         case 5320: return "quest_def"; break;
         case 5324: return "flavor_def"; break;
         case 5334: return "Subphrase"; break;
         case 5342: return "flavor_entry"; break;
         case 5371: return "Subphrase"; break;
         case 5378: return "Subphrase"; break;
         case 5385: return "Subphrase"; break;
         case 5392: return "Subphrase"; break;
         case 5399: return "encounter_def"; break;
         case 5408: return "Subphrase"; break;
         case 5416: return "Subphrase"; break;
         case 5424: return "encounter_entry"; break;
         case 5506: return "Subphrase"; break;
         case 5514: return "Subphrase"; break;
         case 5522: return "Subphrase"; break;
         case 5529: return "Subphrase"; break;
         case 5538: return "Subphrase"; break;
         case 5549: return "encounter_part"; break;
         case 5558: return "Subphrase"; break;
         case 5570: return "Subphrase"; break;
         case 5625: return "Subphrase"; break;
         case 5633: return "Subphrase"; break;
         case 5646: return "Subphrase"; break;
         case 5662: return "Subphrase"; break;
         case 5672: return "Subphrase"; break;
         case 5681: return "Subphrase"; break;
         case 5689: return "behaviour_def"; break;
         case 5699: return "Subphrase"; break;
         case 5707: return "behaviour_entry"; break;
         case 5733: return "Subphrase"; break;
         case 5740: return "Subphrase"; break;
         case 5747: return "Subphrase"; break;
         case 5756: return "Subphrase"; break;
         case 5767: return "text_def"; break;
         case 5774: return "Subphrase"; break;
         case 5822: return "prototype"; break;
         case 5826: return "type_name"; break;
         case 5854: return "s_prototype"; break;
         case 5867: return "Subphrase"; break;
         case 5878: return "Subphrase"; break;
         case 5886: return "Subphrase"; break;
         case 5893: return "Subphrase"; break;
         case 5901: return "Subphrase"; break;
         case 5913: return "Subphrase"; break;
         case 5921: return "Subphrase"; break;
         case 5928: return "Subphrase"; break;
         case 5936: return "Subphrase"; break;
         case 5944: return "sg_prototype"; break;
         case 5955: return "Subphrase"; break;
         case 5966: return "Subphrase"; break;
         case 5974: return "Subphrase"; break;
         case 5981: return "Subphrase"; break;
         case 5989: return "Subphrase"; break;
         case 6001: return "Subphrase"; break;
         case 6009: return "Subphrase"; break;
         case 6016: return "Subphrase"; break;
         case 6024: return "Subphrase"; break;
         case 6032: return "s_declaration"; break;
         case 6041: return "s_object_decl"; break;
         case 6050: return "g_declaration"; break;
         case 6057: return "Subphrase"; break;
         case 6066: return "l_declaration"; break;
         case 6073: return "Subphrase"; break;
         case 6082: return "r_declaration"; break;
         case 6089: return "Subphrase"; break;
         case 6098: return "statement"; break;
         case 6187: return "Subphrase"; break;
         case 6198: return "Subphrase"; break;
         case 6206: return "Subphrase"; break;
         case 6214: return "Subphrase"; break;
         case 6223: return "Subphrase"; break;
         case 6231: return "block"; break;
         case 6237: return "Subphrase"; break;
         case 6245: return "routine"; break;
         case 6252: return "Subphrase"; break;
         case 6266: return "Subphrase"; break;
         case 6274: return "fund_expr"; break;
         case 6340: return "Subphrase"; break;
         case 6348: return "Subphrase"; break;
         case 6356: return "lvalue"; break;
         case 6372: return "Subphrase"; break;
         case 6380: return "expr10"; break;
         case 6412: return "expr20"; break;
         case 6452: return "expr30"; break;
         case 6462: return "expr40"; break;
         case 6473: return "expr50"; break;
         case 6495: return "expr60"; break;
         case 6511: return "expr70"; break;
         case 6527: return "expr80"; break;
         case 6567: return "expr90"; break;
         case 6577: return "expr100"; break;
         case 6587: return "expr110"; break;
         case 6597: return "expr120"; break;
         case 6607: return "expr130"; break;
         case 6617: return "expr140"; break;
         case 6629: return "expr150"; break;
         case 6699: return "expr"; break;
         case 6709: return "param_list"; break;
         case 6713: return "Subphrase"; break;
         case 6721: return "Subphrase"; break;
   }
   else 
      switch(n-50000) {
         case 495: return "MEMBER_OP"; break;
         case 494: return "ELLIPSIS"; break;
         case 493: return "ASSIGN_SUB"; break;
         case 492: return "ASSIGN_RSHIFT"; break;
         case 491: return "ASSIGN_LSHIFT"; break;
         case 490: return "ASSIGN_NEG"; break;
         case 489: return "ASSIGN_OR"; break;
         case 488: return "ASSIGN_AND"; break;
         case 487: return "ASSIGN_MOD"; break;
         case 486: return "ASSIGN_DIV"; break;
         case 485: return "ASSIGN_ADD"; break;
         case 484: return "ASSIGN_MULT"; break;
         case 483: return "OR_OP"; break;
         case 482: return "AND_OP"; break;
         case 481: return "NE_OP"; break;
         case 480: return "EQ_OP"; break;
         case 479: return "LE_OP"; break;
         case 478: return "GE_OP"; break;
         case 477: return "RSHIFT_OP"; break;
         case 476: return "LSHIFT_OP"; break;
         case 475: return "DECREMENT_OP"; break;
         case 474: return "INCREMENT_OP"; break;
         case 473: return "WORD_RECT"; break;
         case 472: return "YVAL"; break;
         case 471: return "XVAL"; break;
         case 470: return "WITH"; break;
         case 469: return "WIELD"; break;
         case 468: return "WHILE"; break;
         case 467: return "WEIGHT_LIM"; break;
         case 466: return "WEIGHT_MOD"; break;
         case 465: return "WEIGHT"; break;
         case 464: return "WEEK"; break;
         case 463: return "WALLS"; break;
         case 462: return "VAR"; break;
         case 461: return "VALUE"; break;
         case 460: return "TVAL"; break;
         case 459: return "TYPE"; break;
         case 458: return "TURNS"; break;
         case 457: return "WORD_TRUE"; break;
         case 456: return "TO"; break;
         case 455: return "TIMEOUT"; break;
         case 454: return "TILES"; break;
         case 453: return "TILE"; break;
         case 452: return "THROWING"; break;
         case 451: return "THREAT"; break;
         case 450: return "TEXT"; break;
         case 449: return "TERRAIN"; break;
         case 448: return "WORD_TEMPLATE"; break;
         case 447: return "WORD_TARGET"; break;
         case 446: return "SYSTEM"; break;
         case 445: return "SWITCH"; break;
         case 444: return "SVAL"; break;
         case 443: return "SUSTAINS"; break;
         case 442: return "STORE"; break;
         case 441: return "STOCK"; break;
         case 440: return "STATIC"; break;
         case 439: return "STATI"; break;
         case 438: return "STARTING"; break;
         case 437: return "START"; break;
         case 436: return "SPELLS"; break;
         case 435: return "SPELL"; break;
         case 434: return "SPEED"; break;
         case 433: return "SPECIALS"; break;
         case 432: return "SLOT"; break;
         case 431: return "SKILLS"; break;
         case 430: return "SIZE"; break;
         case 429: return "SDMG"; break;
         case 428: return "SAVES"; break;
         case 427: return "RVAL"; break;
         case 426: return "ROUTINE"; break;
         case 425: return "ROOMTYPES"; break;
         case 424: return "RETURN"; break;
         case 423: return "RESISTS"; break;
         case 422: return "REGIONS"; break;
         case 421: return "REGION"; break;
         case 420: return "RATING"; break;
         case 419: return "RANGE"; break;
         case 418: return "RANDOM"; break;
         case 417: return "RACE"; break;
         case 416: return "QVAL"; break;
         case 415: return "QUEST"; break;
         case 414: return "QUALITIES"; break;
         case 413: return "PVAL"; break;
         case 412: return "PURPOSE"; break;
         case 411: return "PROP"; break;
         case 410: return "PROFICIENCIES"; break;
         case 409: return "POWER"; break;
         case 408: return "POISON"; break;
         case 407: return "PER"; break;
         case 406: return "PENALTY"; break;
         case 405: return "PARTS"; break;
         case 404: return "PARTIAL"; break;
         case 403: return "PARRY"; break;
         case 402: return "PARAMS"; break;
         case 401: return "OPTION"; break;
         case 400: return "OR"; break;
         case 399: return "ON"; break;
         case 398: return "OF"; break;
         case 397: return "OBJECTS"; break;
         case 396: return "OBJECT"; break;
         case 395: return "NUTRITION"; break;
         case 394: return "WORD_NULL"; break;
         case 393: return "NPC"; break;
         case 392: return "NORMAL"; break;
         case 391: return "NONE"; break;
         case 390: return "NEAR"; break;
         case 389: return "MTYPE"; break;
         case 388: return "MOVE"; break;
         case 387: return "MONSTER"; break;
         case 386: return "WORD_MIN"; break;
         case 385: return "MEMBER"; break;
         case 384: return "MELEE"; break;
         case 383: return "WORD_MAX"; break;
         case 382: return "MATERIAL"; break;
         case 381: return "MAP"; break;
         case 380: return "MANA"; break;
         case 379: return "MODULE"; break;
         case 378: return "MAX_SIZE"; break;
         case 377: return "LVAL"; break;
         case 376: return "LISTS"; break;
         case 375: return "LIGHT"; break;
         case 374: return "LIFESPAN"; break;
         case 373: return "WORD_LEVEL"; break;
         case 372: return "LDMG"; break;
         case 371: return "ITEM"; break;
         case 370: return "INVOKE"; break;
         case 369: return "INNATE"; break;
         case 368: return "IMMUNE"; break;
         case 367: return "IMAGE"; break;
         case 366: return "IF"; break;
         case 365: return "HIT"; break;
         case 364: return "HITPOINTS"; break;
         case 363: return "HITDICE"; break;
         case 362: return "HEAVY"; break;
         case 361: return "GUILD"; break;
         case 360: return "GROUP"; break;
         case 359: return "GRID"; break;
         case 358: return "GRANTS"; break;
         case 357: return "GOD"; break;
         case 356: return "GLYPH"; break;
         case 355: return "GEAR"; break;
         case 354: return "FUEL"; break;
         case 353: return "FROM"; break;
         case 352: return "FOR"; break;
         case 351: return "WORD_FILE"; break;
         case 350: return "FLOOR"; break;
         case 349: return "FLAGS"; break;
         case 348: return "FIRES"; break;
         case 347: return "FEATURE"; break;
         case 346: return "FEAT"; break;
         case 345: return "FEATS"; break;
         case 344: return "WORD_FALSE"; break;
         case 343: return "FACTOR"; break;
         case 342: return "EVERY"; break;
         case 341: return "EVENT"; break;
         case 340: return "FLAVOR"; break;
         case 339: return "FAVORED"; break;
         case 338: return "EVAL"; break;
         case 337: return "EQUIP"; break;
         case 336: return "EXPORT"; break;
         case 335: return "WORD_ENCOUNTER"; break;
         case 334: return "ELSE"; break;
         case 333: return "EFFECT"; break;
         case 332: return "DVAL"; break;
         case 331: return "DUNGEON"; break;
         case 330: return "DO"; break;
         case 329: return "DISEASE"; break;
         case 328: return "DOMAINS"; break;
         case 327: return "DOMAIN"; break;
         case 326: return "DOOR"; break;
         case 325: return "WORD_DESC"; break;
         case 324: return "DEPTH"; break;
         case 323: return "DEFAULT"; break;
         case 322: return "DEF"; break;
         case 321: return "WORD_DC"; break;
         case 320: return "DAY"; break;
         case 319: return "DARK"; break;
         case 318: return "DMG"; break;
         case 317: return "CVAL"; break;
         case 316: return "CURSED"; break;
         case 315: return "CRIT"; break;
         case 314: return "COST"; break;
         case 313: return "CONTINUE"; break;
         case 312: return "CTYPE"; break;
         case 311: return "COVERAGE"; break;
         case 310: return "CONSTANTS"; break;
         case 309: return "WORD_COLOR"; break;
         case 308: return "CLASS"; break;
         case 307: return "CHANCE"; break;
         case 306: return "WORD_CR"; break;
         case 305: return "CASTER"; break;
         case 304: return "CASE"; break;
         case 303: return "CAPACITY"; break;
         case 302: return "BRIGHT"; break;
         case 301: return "BREAK"; break;
         case 300: return "BRAWL"; break;
         case 299: return "BLESSED"; break;
         case 298: return "BEHAVIOUR"; break;
         case 297: return "BASE"; break;
         case 296: return "AVAL"; break;
         case 295: return "ATTACK"; break;
         case 294: return "AT"; break;
         case 293: return "AS"; break;
         case 292: return "ARTIFACT"; break;
         case 291: return "ARM"; break;
         case 290: return "ARCHERY"; break;
         case 289: return "APTITUDES"; break;
         case 288: return "WORD_ANY"; break;
         case 287: return "AND"; break;
         case 286: return "ALSO"; break;
         case 285: return "ALL"; break;
         case 284: return "ALIGN"; break;
         case 283: return "ACC"; break;
         case 282: return "ABS"; break;
         case 281: return "ABILITY"; break;
         case 280: return "REGNAME"; break;
         case 279: return "OPCODE"; break;
         case 278: return "STYPE"; break;
         case 277: return "DIRECTION"; break;
         case 276: return "ATTRIBUTE"; break;
         case 275: return "WEP_TYPE"; break;
         case 274: return "COLOR"; break;
         case 273: return "STRING"; break;
         case 272: return "RID"; break;
         case 271: return "HTEXT"; break;
         case 270: return "UINT32"; break;
         case 269: return "UINT16"; break;
         case 268: return "UINT8"; break;
         case 267: return "INT32"; break;
         case 266: return "INT16"; break;
         case 265: return "INT8"; break;
         case 264: return "VOID"; break;
         case 263: return "HOBJ"; break;
         case 262: return "SCOPE_OPER"; break;
         case 261: return "NOTDONE"; break;
         case 260: return "MAPAREA"; break;
         case 259: return "CHAR_CONST"; break;
         case 258: return "IDENT"; break;
         case 257: return "LITERAL"; break;
         case 256: return "NUMBER"; break;
   }
   return "<character>";
}
