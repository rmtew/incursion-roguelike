#line 1 "lang/Grammar.acc"

/* GRAMMAR.ACC -- See the Incursion LICENSE file for copyright information.



     Contains the BNF+ grammar for the IncursionScript compiler,

	 along with semantic actions that essentially compose the bulk

	 of the compiler. Designed to be fed into the ACCENT compiler

	 compiler system to produce yygram.cpp.

*/

#include "Incursion.h"
#include <setjmp.h>

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

//extern LocationInfo __Tiles__[];

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

/* function should return a value; 'void' return type assumed */
#pragma warning (disable : 4508)

int yyerror(const char*msg) {
   Errors++;
   printf("\"%s\" (%d): %s\n", preprocFilename, yypos, msg);
   longjmp(jb,1);
}

void yywarning(const char*msg) {
   printf("\"%s\" (%d): %s\n", preprocFilename,  yypos, msg);
}


extern "C" int yywrap() {
   return 1;
}



# line 150 "src/yygram.cpp"
#include "yygram.h"

int YYSTART ()
{
   switch(yyselect()) {
   case 479: {
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
#line 190 "lang/Grammar.acc"
 currMap = 0; MemFuncID = 0; MemVarID = 0; 
# line 170 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 480: {
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
#line 194 "lang/Grammar.acc"
 theModule->Name = name; 
# line 200 "src/yygram.cpp"
      } break;
   case 3: {
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 195 "lang/Grammar.acc"
 theModule->Slot = (int16)n; 
# line 208 "src/yygram.cpp"
      } break;
   case 4: {
      get_lexval();
      get_lexval();
      fname = yylval;
      get_lexval();
#line 196 "lang/Grammar.acc"
 theModule->FName = fname; 
# line 217 "src/yygram.cpp"
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
      case 481: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      cexpr(&n);
#line 220 "lang/Grammar.acc"
 CompilerFlags |= n; 
# line 299 "src/yygram.cpp"
      } break;
   case 29: {
      get_lexval();
      get_lexval();
      nam = yylval;
      get_lexval();
      switch (yyselect()) {
      case 482: {
         get_lexval();
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      text = yylval;
      switch (yyselect()) {
      case 483: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 222 "lang/Grammar.acc"
 theRes = theTxt; theTxt->Name = nam; theTxt->Desc = text; theTxt++; 
# line 324 "src/yygram.cpp"
      } break;
   }
return 0;
}

int res_ref (rID *res)
{
   YYSTYPE lit;
   YYSTYPE n;
   switch(yyselect()) {
   case 30: {
      get_lexval();
      get_lexval();
      lit = yylval;
#line 226 "lang/Grammar.acc"
 PurgeStrings();
                         *res = FIND(theModule->GetText(lit)); 
                         if (!*res) {
                           if (Unresolved.GetLength())
                             Unresolved += ", ";
                           Unresolved += Capitalize(theModule->GetText(lit),true);
                           *res = FIND("unimplemented");
                           }
                        
# line 349 "src/yygram.cpp"
      } break;
   case 31: {
      get_lexval();
      cexpr3(&n);
#line 235 "lang/Grammar.acc"
 *res = n; 
# line 356 "src/yygram.cpp"
      } break;
   }
return 0;
}

int cexpr (YYSTYPE *val)
{
   YYSTYPE x;
   YYSTYPE y;
   YYSTYPE val2;
   switch(yyselect()) {
   case 32: {
      cexpr(&x);
      get_lexval();
      cexpr2(&y);
#line 239 "lang/Grammar.acc"
 *val = x+y; 
# line 374 "src/yygram.cpp"
      } break;
   case 33: {
      cexpr(&x);
      get_lexval();
      cexpr2(&y);
#line 240 "lang/Grammar.acc"
 *val = x-y; 
# line 382 "src/yygram.cpp"
      } break;
   case 34: {
      cexpr2(&val2);
#line 241 "lang/Grammar.acc"
 *val = val2; 
# line 388 "src/yygram.cpp"
      } break;
   }
return 0;
}

int cexpr2 (YYSTYPE *val)
{
   YYSTYPE x;
   YYSTYPE y;
   YYSTYPE val2;
   switch(yyselect()) {
   case 35: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 245 "lang/Grammar.acc"
 *val = x * y; 
# line 406 "src/yygram.cpp"
      } break;
   case 36: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 246 "lang/Grammar.acc"
 *val = x / y; 
# line 414 "src/yygram.cpp"
      } break;
   case 37: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 247 "lang/Grammar.acc"
 *val = x % y; 
# line 422 "src/yygram.cpp"
      } break;
   case 38: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 248 "lang/Grammar.acc"
 *val = x | y; 
# line 430 "src/yygram.cpp"
      } break;
   case 39: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 249 "lang/Grammar.acc"
 *val = x & y; 
# line 438 "src/yygram.cpp"
      } break;
   case 40: {
      cexpr3(&val2);
#line 251 "lang/Grammar.acc"
 *val = val2; 
# line 444 "src/yygram.cpp"
      } break;
   }
return 0;
}

int cexpr3 (YYSTYPE *val)
{
   YYSTYPE val2;
   YYSTYPE n;
   YYSTYPE val3;
   switch(yyselect()) {
   case 41: {
      get_lexval();
      cexpr4(&val2);
#line 255 "lang/Grammar.acc"
 *val = val2; 
# line 461 "src/yygram.cpp"
      } break;
   case 42: {
      get_lexval();
      cexpr4(&n);
#line 256 "lang/Grammar.acc"
 *val = -n; 
# line 468 "src/yygram.cpp"
      } break;
   case 43: {
      cexpr4(&val3);
#line 257 "lang/Grammar.acc"
 *val = val3; 
# line 474 "src/yygram.cpp"
      } break;
   }
return 0;
}

int cexpr4 (YYSTYPE *val)
{
   YYSTYPE val2;
   YYSTYPE val3;
   YYSTYPE val4;
   switch(yyselect()) {
   case 44: {
      get_lexval();
      cexpr(&val2);
      get_lexval();
#line 261 "lang/Grammar.acc"
 * val = val2; 
# line 492 "src/yygram.cpp"
      } break;
   case 45: {
      get_lexval();
      val3 = yylval;
#line 262 "lang/Grammar.acc"
 *val = val3; 
# line 499 "src/yygram.cpp"
      } break;
   case 46: {
      get_lexval();
#line 263 "lang/Grammar.acc"
 *val = 1; 
# line 505 "src/yygram.cpp"
      } break;
   case 47: {
      get_lexval();
#line 264 "lang/Grammar.acc"
 *val = 0; 
# line 511 "src/yygram.cpp"
      } break;
   case 48: {
      get_lexval();
      val4 = yylval;
#line 265 "lang/Grammar.acc"
 * val = val4; 
# line 518 "src/yygram.cpp"
      } break;
   }
return 0;
}

int dice_val (Dice *d)
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
#line 270 "lang/Grammar.acc"
 d->Number = (int8)num; d->Sides = (int8)sid; d->Bonus = (int8)bon; 
# line 539 "src/yygram.cpp"
      } break;
   case 50: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
      get_lexval();
      get_lexval();
      cexpr4(&bon);
#line 272 "lang/Grammar.acc"
 d->Number = (int8)num; d->Sides = (int8)sid; d->Bonus = (int8)-bon; 
# line 550 "src/yygram.cpp"
      } break;
   case 51: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
#line 274 "lang/Grammar.acc"
 d->Number = (int8)num; d->Sides = (int8)sid; d->Bonus = 0; 
# line 558 "src/yygram.cpp"
      } break;
   case 52: {
      cexpr3(&x);
#line 276 "lang/Grammar.acc"
 d->Number = 0; d->Sides = 0; d->Bonus = (int8)x; 
# line 564 "src/yygram.cpp"
      } break;
   }
return 0;
}

int dice_only (Dice *d)
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
#line 281 "lang/Grammar.acc"
 d->Number = (int8)num; d->Sides = (int8)sid; d->Bonus = (int8)bon; 
# line 584 "src/yygram.cpp"
      } break;
   case 54: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
#line 283 "lang/Grammar.acc"
 d->Number = (int8)num; d->Sides = (int8)sid; d->Bonus = 0; 
# line 592 "src/yygram.cpp"
      } break;
   }
return 0;
}

int gear_entry (int16 *chance, Dice *amt, rID *iID, rID *eID, uint16 *spec, uint32 *qual, uint32 *fl, hCode *cond)
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
#line 288 "lang/Grammar.acc"

    int16 q; q=0;
    *chance = 0;
    *spec = 0;
    *qual = *fl = 0;
    *cond = 0;
    *iID = *eID = 0;
    amt->Set(1,1,0); 
# line 620 "src/yygram.cpp"
      switch (yyselect()) {
      case 484: {
         get_lexval();
         get_lexval();
         sep_expr(&hc);
         get_lexval();
#line 296 "lang/Grammar.acc"
 *cond = hc; 
# line 629 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 485: {
         get_lexval();
         n = yylval;
         get_lexval();
#line 297 "lang/Grammar.acc"
 *chance = (int16)n; 
# line 640 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 486: {
         dice_only(&dv);
#line 298 "lang/Grammar.acc"
 *amt = yyDice(dv); 
# line 649 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 487: {
         get_lexval();
#line 299 "lang/Grammar.acc"
 *fl |= GF_CURSED; 
# line 658 "src/yygram.cpp"
         } break;
      case 488: {
         get_lexval();
#line 300 "lang/Grammar.acc"
 *fl |= GF_BLESSED; 
# line 664 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 489: {
         res_ref(&rr);
#line 301 "lang/Grammar.acc"
 *iID = rr; 
# line 673 "src/yygram.cpp"
         } break;
      case 490: {
         get_lexval();
         cexpr3(&n);
#line 302 "lang/Grammar.acc"
 *iID = n; *fl |= GF_RANDOM_ITEM; 
# line 680 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 491: {
         get_lexval();
         get_lexval();
         n = yylval;
#line 303 "lang/Grammar.acc"
 *spec = (uint16)n; 
# line 691 "src/yygram.cpp"
         } break;
      case 492: {
         get_lexval();
         get_lexval();
         n = yylval;
#line 303 "lang/Grammar.acc"
 *spec = (uint16)-n; 
# line 699 "src/yygram.cpp"
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 495: {
            get_lexval();
            get_lexval();
            while (1) {
               switch (yyselect()) {
               case 494: {
                  cexpr(&wq);
                  switch (yyselect()) {
                  case 493: {
                     get_lexval();
                     } break;
                  default: ;
                  }
#line 304 "lang/Grammar.acc"
 if (q >= 4)
                                   yyerror("Max 4 qualities in gear lists!");
                                else
                                  ((uint8*)qual)[q++] = (uint8)wq;
# line 723 "src/yygram.cpp"
                  } break;
               default: goto yy3;
               }
            }
            yy3: ;
            get_lexval();
            } break;
         case 496: {
            get_lexval();
            res_ref(&rr);
#line 308 "lang/Grammar.acc"
 *eID = rr; 
# line 736 "src/yygram.cpp"
            } break;
         case 497: {
            get_lexval();
            get_lexval();
            cexpr3(&a);
            get_lexval();
            cexpr3(&b);
#line 310 "lang/Grammar.acc"
 *eID = a + b*256; *fl |= GF_RANDOM_EFFECT; 
# line 746 "src/yygram.cpp"
            } break;
         case 498: {
            get_lexval();
            get_lexval();
            cexpr3(&b);
#line 312 "lang/Grammar.acc"
 *eID = b*256; *fl |= GF_RANDOM_EFFECT; 
# line 754 "src/yygram.cpp"
            } break;
         case 499: {
            get_lexval();
            get_lexval();
#line 314 "lang/Grammar.acc"
 *eID = 0; *fl |= GF_RANDOM_EFFECT; 
# line 761 "src/yygram.cpp"
            } break;
         case 500: {
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
#line 320 "lang/Grammar.acc"
 int16 is_else = 0; 
# line 794 "src/yygram.cpp"
      get_lexval();
      switch (yyselect()) {
      case 501: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      while (1) {
         switch (yyselect()) {
         case 504: {
            gear_entry(&chance, &amt, &iID, &eID, &spec, &qual, &fl, &cond);
#line 323 "lang/Grammar.acc"
 theRes->AddEquip((uint8)chance,amt,iID,eID,(int8)spec,(uint8*)(&qual),(uint8)(fl | is_else),cond); 
# line 808 "src/yygram.cpp"
            switch (yyselect()) {
            case 502: {
               get_lexval();
#line 324 "lang/Grammar.acc"
 is_else = 0; 
# line 814 "src/yygram.cpp"
               } break;
            case 503: {
               get_lexval();
#line 324 "lang/Grammar.acc"
 is_else = GF_ELSE; 
# line 820 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy4;
         }
      }
      yy4: ;
      gear_entry(&chance, &amt, &iID, &eID, &spec, &qual, &fl, &cond);
#line 326 "lang/Grammar.acc"
 theRes->AddEquip((uint8)chance,amt,iID,eID,(int8)spec,(uint8*)(&qual),(uint8)(fl | is_else),cond); 
# line 832 "src/yygram.cpp"
      get_lexval();
      } break;
   }
return 0;
}

int glyph_entry (uint16 *img)
{
   YYSTYPE col;
   YYSTYPE n;
   YYSTYPE back;
   switch(yyselect()) {
   case 57: {
      switch (yyselect()) {
      case 505: {
         get_lexval();
         } break;
      case 506: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 507: {
         get_lexval();
         } break;
      default: ;
      }
#line 330 "lang/Grammar.acc"
 *img = GLYPH_FORE(WHITE); 
# line 863 "src/yygram.cpp"
      switch (yyselect()) {
      case 508: {
         color(&col);
#line 331 "lang/Grammar.acc"
 *img = (uint16)GLYPH_FORE(col); 
# line 869 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 509: {
         get_lexval();
         n = yylval;
#line 332 "lang/Grammar.acc"
 *img |= n; 
# line 879 "src/yygram.cpp"
         } break;
      case 510: {
         get_lexval();
         n = yylval;
#line 333 "lang/Grammar.acc"
 *img |= n; 
# line 886 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 511: {
         get_lexval();
         color(&back);
#line 335 "lang/Grammar.acc"
 *img |= GLYPH_BACK(back); 
# line 896 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      } break;
   }
return 0;
}

int glyph_desc (uint16 *img)
{
   YYSTYPE col;
   YYSTYPE n;
   YYSTYPE back;
   switch(yyselect()) {
   case 58: {
#line 339 "lang/Grammar.acc"
 *img = GLYPH_FORE(WHITE); 
# line 915 "src/yygram.cpp"
      switch (yyselect()) {
      case 512: {
         color(&col);
#line 340 "lang/Grammar.acc"
 *img = (uint16)GLYPH_FORE(col); 
# line 921 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 513: {
         get_lexval();
         n = yylval;
#line 341 "lang/Grammar.acc"
 *img |= n; 
# line 931 "src/yygram.cpp"
         } break;
      case 514: {
         get_lexval();
         n = yylval;
#line 342 "lang/Grammar.acc"
 *img |= n; 
# line 938 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 515: {
         get_lexval();
         color(&back);
#line 343 "lang/Grammar.acc"
 *img |= GLYPH_BACK(back); 
# line 948 "src/yygram.cpp"
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
         case 516: {
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
#line 351 "lang/Grammar.acc"
 int16 EvList[32]; int16 i, cEvent = 0; 
# line 989 "src/yygram.cpp"
      cexpr(&n);
#line 352 "lang/Grammar.acc"
 EvList[cEvent++] = (int16)n; theEvent = n; 
# line 993 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 517: {
            get_lexval();
            cexpr(&n);
#line 353 "lang/Grammar.acc"
 EvList[cEvent++] = (int16)n; 
# line 1001 "src/yygram.cpp"
            } break;
         default: goto yy6;
         }
      }
      yy6: ;
      switch (yyselect()) {
      case 519: {
         get_lexval();
         lv = yylval;
#line 354 "lang/Grammar.acc"
 for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(-EvList[i],lv); 
# line 1014 "src/yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 518: {
               get_lexval();
               get_lexval();
               lv = yylval;
#line 356 "lang/Grammar.acc"
 for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(-EvList[i],lv); 
# line 1024 "src/yygram.cpp"
               } break;
            default: goto yy7;
            }
         }
         yy7: ;
         } break;
      case 520: {
         routine(&vc);
#line 361 "lang/Grammar.acc"
 hCode hc = theCodeSeg.Add(&vc);
                            theCodeSeg.Generate(HALT);
                            for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(EvList[i],hc);
                            theEvent = 0; 
# line 1039 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      } break;
   }
return 0;
}

int sep_expr (hCode *hc)
{
   PExp ex;
   switch(yyselect()) {
   case 61: {
#line 370 "lang/Grammar.acc"
 theEvent = EV_CONDITION; 
# line 1055 "src/yygram.cpp"
      expr(&ex);
#line 372 "lang/Grammar.acc"
 *hc = theCodeSeg.Add(ex.Code);
      theCodeSeg.Generate(RET,ex.Storage,ex.Value); 
# line 1060 "src/yygram.cpp"
#line 374 "lang/Grammar.acc"
 theEvent = 0; 
# line 1063 "src/yygram.cpp"
      } break;
   }
return 0;
}

int sep_stat (hCode *hc)
{
   VBlock st;
   switch(yyselect()) {
   case 62: {
#line 378 "lang/Grammar.acc"
 theEvent = EV_CONDITION; 
# line 1076 "src/yygram.cpp"
      statement(&st);
#line 380 "lang/Grammar.acc"
 *hc = theCodeSeg.Add(&st);
      theCodeSeg.Generate(RET,0,0); 
# line 1081 "src/yygram.cpp"
#line 382 "lang/Grammar.acc"
 theEvent = 0; 
# line 1084 "src/yygram.cpp"
      } break;
   }
return 0;
}

int mval (MVal *mv)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 63: {
#line 386 "lang/Grammar.acc"
 mv->BType = mv->Bound = 0; 
# line 1097 "src/yygram.cpp"
      switch (yyselect()) {
      case 522: {
         switch (yyselect()) {
         case 521: {
            get_lexval();
            } break;
         default: ;
         }
         cexpr4(&n);
#line 387 "lang/Grammar.acc"
 mv->Value = n;  mv->VType = MVAL_ADD; 
# line 1109 "src/yygram.cpp"
         } break;
      case 523: {
         get_lexval();
         cexpr4(&n);
#line 388 "lang/Grammar.acc"
 mv->Value = -n; mv->VType = MVAL_ADD; 
# line 1116 "src/yygram.cpp"
         } break;
      case 524: {
         get_lexval();
         cexpr4(&n);
#line 389 "lang/Grammar.acc"
 mv->Value = n;  mv->VType = MVAL_SET; 
# line 1123 "src/yygram.cpp"
         } break;
      case 525: {
         cexpr3(&n);
         get_lexval();
#line 390 "lang/Grammar.acc"
 mv->Value = n;  mv->VType = MVAL_PERCENT; 
# line 1130 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 529: {
         get_lexval();
         switch (yyselect()) {
         case 526: {
            get_lexval();
            cexpr4(&n);
#line 392 "lang/Grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_MIN; 
# line 1143 "src/yygram.cpp"
            } break;
         case 527: {
            get_lexval();
            cexpr4(&n);
#line 393 "lang/Grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_MAX; 
# line 1150 "src/yygram.cpp"
            } break;
         case 528: {
            get_lexval();
            cexpr4(&n);
#line 394 "lang/Grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_NEAR; 
# line 1157 "src/yygram.cpp"
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
#line 399 "lang/Grammar.acc"
 theMon->Attr[0] = theMon->Attr[1] =
                       theMon->Attr[2] = theMon->Attr[3] =
                         theMon->Attr[4] = theMon->Attr[5] =
                         0; CurrAttk = 0; CurrFeat = 0; theRes = theMon; 
# line 1184 "src/yygram.cpp"
      get_lexval();
      name = yylval;
#line 403 "lang/Grammar.acc"
 theMon->Name = name; 
# line 1189 "src/yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 404 "lang/Grammar.acc"
 theMon->MType[0] = n; 
# line 1194 "src/yygram.cpp"
      switch (yyselect()) {
      case 531: {
         get_lexval();
         cexpr3(&n2);
#line 405 "lang/Grammar.acc"
 theMon->MType[1] = n2; 
# line 1201 "src/yygram.cpp"
         switch (yyselect()) {
         case 530: {
            get_lexval();
            cexpr3(&n3);
#line 406 "lang/Grammar.acc"
 theMon->MType[2] = n3; 
# line 1208 "src/yygram.cpp"
            } break;
         default: ;
         }
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 532: {
            mon_entry();
            } break;
         default: goto yy8;
         }
      }
      yy8: ;
      get_lexval();
#line 408 "lang/Grammar.acc"
 theMon++; 
# line 1228 "src/yygram.cpp"
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
      case 533: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 411 "lang/Grammar.acc"
 theMon->Desc = lv; 
# line 1262 "src/yygram.cpp"
      } break;
   case 66: {
      get_lexval();
      switch (yyselect()) {
      case 534: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 412 "lang/Grammar.acc"
 theMon->Depth = (int8)n; 
# line 1276 "src/yygram.cpp"
      } break;
   case 67: {
      switch (yyselect()) {
      case 535: {
         get_lexval();
         } break;
      case 536: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 537: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 413 "lang/Grammar.acc"
 theMon->CR = (int8)n; 
# line 1298 "src/yygram.cpp"
      } break;
   case 68: {
      switch (yyselect()) {
      case 538: {
         get_lexval();
         } break;
      case 539: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 540: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n2);
      get_lexval();
      cexpr3(&n1);
      get_lexval();
#line 415 "lang/Grammar.acc"
 if (n2 != 1 || n1 > 8 || n1 < 2)
                                        yyerror("Fractional CR error!");
                                      theMon->CR = (int8)((-n1) + 2); 
# line 1324 "src/yygram.cpp"
      } break;
   case 69: {
      get_lexval();
      switch (yyselect()) {
      case 541: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
#line 418 "lang/Grammar.acc"
 theMon->Terrains |= n; 
# line 1337 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 542: {
            get_lexval();
            cexpr(&n);
#line 419 "lang/Grammar.acc"
 theMon->Terrains |= n; 
# line 1345 "src/yygram.cpp"
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
      case 543: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 420 "lang/Grammar.acc"
 theMon->HitDice = (int16)n; 
# line 1365 "src/yygram.cpp"
      } break;
   case 71: {
      get_lexval();
      switch (yyselect()) {
      case 544: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 421 "lang/Grammar.acc"
 theMon->HitDice = (int16)n; 
                                       theMon->SetFlag(M_FIXED_HP); 
# line 1380 "src/yygram.cpp"
      } break;
   case 72: {
      switch (yyselect()) {
      case 545: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      switch (yyselect()) {
      case 546: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 423 "lang/Grammar.acc"
 theMon->Hit = (int8)n; 
# line 1400 "src/yygram.cpp"
      } break;
   case 73: {
      glyph_entry(&img);
#line 424 "lang/Grammar.acc"
 theMon->Image = img; 
# line 1406 "src/yygram.cpp"
      } break;
   case 74: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      get_lexval();
#line 426 "lang/Grammar.acc"
 if (!theMon->GainStati((uint8)st)) yyerror("too many Stati in monster"); 
# line 1416 "src/yygram.cpp"
      } break;
   case 75: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      get_lexval();
#line 428 "lang/Grammar.acc"
 if (!theMon->GainStati((uint8)st,(int16)stV)) yyerror("too many Stati in monster"); 
# line 1428 "src/yygram.cpp"
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
#line 430 "lang/Grammar.acc"
 if (!theMon->GainStati((uint8)st,(int16)stV,(int16)stM)) 
        yyerror("too many Stati in monster"); 
# line 1443 "src/yygram.cpp"
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
#line 433 "lang/Grammar.acc"
 if (!theMon->GainStati((uint8)st,(int16)stV,(int16)stM,rr)) 
        yyerror("too many Stati in monster"); 
# line 1460 "src/yygram.cpp"
      } break;
   case 78: {
      get_lexval();
      switch (yyselect()) {
      case 547: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 549: {
            cexpr3(&res);
#line 436 "lang/Grammar.acc"
 theMon->Res |= res; 
# line 1476 "src/yygram.cpp"
            switch (yyselect()) {
            case 548: {
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
      case 550: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 552: {
            cexpr3(&imm);
#line 437 "lang/Grammar.acc"
 theMon->Imm |= imm; 
# line 1504 "src/yygram.cpp"
            switch (yyselect()) {
            case 551: {
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
      case 553: {
         get_lexval();
         } break;
      default: ;
      }
      attack_entry();
      while (1) {
         switch (yyselect()) {
         case 554: {
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
#line 439 "lang/Grammar.acc"
 for(TMonster*m=firstMon;m!=theMon;m++)
                                         if(stricmp(theModule->GetText(l),theModule->GetText(m->Name))==0)
                                           { memcpy(theMon,m,sizeof(TMonster));
                                             goto DoneCopy; }
                                       Fatal("Monster '%s' not found!",theModule->GetText(l));
                                       DoneCopy:; 
# line 1551 "src/yygram.cpp"
      } break;
   case 82: {
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 445 "lang/Grammar.acc"
 if (n > (theMon - firstMon) || n < 0)
                                         Fatal("Bad number in Monster:as!");
                                       memcpy(theMon,&(firstMon[n]),sizeof(TMonster)); 
# line 1561 "src/yygram.cpp"
      } break;
   case 83: {
      get_lexval();
      switch (yyselect()) {
      case 555: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 448 "lang/Grammar.acc"
 theMon->Def = (int8)n; 
# line 1575 "src/yygram.cpp"
      } break;
   case 84: {
      get_lexval();
      switch (yyselect()) {
      case 556: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 449 "lang/Grammar.acc"
 theMon->Arm = (int8)n; 
# line 1589 "src/yygram.cpp"
      } break;
   case 85: {
      get_lexval();
      switch (yyselect()) {
      case 557: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 450 "lang/Grammar.acc"
 theMon->Mana = (int16)n; 
# line 1603 "src/yygram.cpp"
      } break;
   case 86: {
      get_lexval();
      switch (yyselect()) {
      case 558: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 559: {
            cexpr3(&n);
            get_lexval();
#line 451 "lang/Grammar.acc"
 theMon->Feats[CurrFeat++] = (uint16)n; 
    if (CurrFeat > 16) Fatal("Too many feats for '%s' (max 16)",
                                theModule->QTextSeg + theMon->Name);
    
# line 1623 "src/yygram.cpp"
            } break;
         default: goto yy13;
         }
      }
      yy13: ;
      cexpr3(&n);
      get_lexval();
#line 455 "lang/Grammar.acc"
 theMon->Feats[CurrFeat++] = (uint16)n; 
    if (CurrFeat > 16) Fatal("Too many feats for '%s' (max 16)",
                                theModule->QTextSeg + theMon->Name);
    
# line 1636 "src/yygram.cpp"
      } break;
   case 87: {
      get_lexval();
      switch (yyselect()) {
      case 560: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 561: {
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
         case 562: {
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
      case 563: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 461 "lang/Grammar.acc"
 theMon->Size = (int8)n; 
# line 1683 "src/yygram.cpp"
      } break;
   case 90: {
      get_lexval();
      switch (yyselect()) {
      case 564: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 565: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 462 "lang/Grammar.acc"
 theMon->Spd = (int8)((n-100)/5); 
# line 1703 "src/yygram.cpp"
      } break;
   case 91: {
      get_lexval();
      switch (yyselect()) {
      case 566: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 567: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 463 "lang/Grammar.acc"
 theMon->Mov = (int8)((n-100)/5); 
# line 1723 "src/yygram.cpp"
      } break;
   case 92: {
      get_lexval();
      switch (yyselect()) {
      case 568: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 570: {
            res_ref(&rr);
#line 464 "lang/Grammar.acc"
 theMon->SetFlag(M_INNATE);
                                       theMon->AddResID(AN_INNATE, rr); 
# line 1740 "src/yygram.cpp"
            switch (yyselect()) {
            case 569: {
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
      case 571: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 573: {
            res_ref(&rr);
#line 467 "lang/Grammar.acc"
 theMon->AddResID(AN_DISEASE, rr); 
# line 1768 "src/yygram.cpp"
            switch (yyselect()) {
            case 572: {
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
      case 574: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 576: {
            res_ref(&rr);
#line 469 "lang/Grammar.acc"
 theMon->AddResID(AN_POISON, rr); 
# line 1796 "src/yygram.cpp"
            switch (yyselect()) {
            case 575: {
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

int color (YYSTYPE *col)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 99: {
      get_lexval();
      n = yylval;
#line 478 "lang/Grammar.acc"
 *col = n; 
# line 1835 "src/yygram.cpp"
      } break;
   case 100: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 479 "lang/Grammar.acc"
 *col = n | 8; 
# line 1843 "src/yygram.cpp"
      } break;
   case 101: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 480 "lang/Grammar.acc"
 *col = n | 8; 
# line 1851 "src/yygram.cpp"
      } break;
   case 102: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 481 "lang/Grammar.acc"
 *col = n & ~8; 
# line 1859 "src/yygram.cpp"
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
#line 485 "lang/Grammar.acc"
 uint8 dt = AD_NORM; dam.Set(0,0,0);
                                 theMon->Attk[CurrAttk].u.a.Dmg.Set(0,0,0); 
# line 1881 "src/yygram.cpp"
      cexpr3(&at);
      get_lexval();
      switch (yyselect()) {
      case 577: {
         dice_only(&dam);
#line 488 "lang/Grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg = yyDice(dam); 
# line 1889 "src/yygram.cpp"
         } break;
      case 578: {
         cexpr4(&chance);
         get_lexval();
#line 489 "lang/Grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg.Bonus = (int8)chance; 
# line 1896 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 579: {
         cexpr4(&n);
#line 490 "lang/Grammar.acc"
 dt = (uint8)n; 
# line 1905 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 580: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 584: {
         get_lexval();
         switch (yyselect()) {
         case 581: {
            get_lexval();
            } break;
         case 582: {
            get_lexval();
            } break;
         case 583: {
            get_lexval();
            } break;
         default: printf("???\n"); exit(1);
         }
         cexpr3(&dc);
#line 493 "lang/Grammar.acc"
 theMon->Attk[CurrAttk].u.a.DC = (int8)dc; 
# line 1933 "src/yygram.cpp"
         get_lexval();
         } break;
      default: ;
      }
#line 494 "lang/Grammar.acc"
 theMon->Attk[CurrAttk].AType = (int8)at;
                                 theMon->Attk[CurrAttk].DType = dt;
                                 /*

                                 if (at == A_ABIL)

                                   theMon->Attk[CurrAttk].u.a.DC = max(

                                        theMon->Attk[CurrAttk].u.a.DC, 1);

                                        */
                                 CurrAttk++;
                                 if (CurrAttk >= 32)
                                   yyerror("> 32 attacks on a single monster!");
                               
# line 1954 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 592: {
            get_lexval();
#line 505 "lang/Grammar.acc"
 dt = AD_NORM; dam2.Set(0,0,0); theMon->Attk[CurrAttk].u.a.Dmg.Set(0,0,0); 
# line 1961 "src/yygram.cpp"
            switch (yyselect()) {
            case 585: {
               dice_only(&dam2);
#line 506 "lang/Grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg = yyDice(dam2); 
# line 1967 "src/yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 586: {
               cexpr3(&n2);
#line 507 "lang/Grammar.acc"
 dt = (uint8)n2; 
# line 1976 "src/yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 587: {
               get_lexval();
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 591: {
               get_lexval();
               switch (yyselect()) {
               case 588: {
                  get_lexval();
                  } break;
               case 589: {
                  get_lexval();
                  } break;
               case 590: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&dc);
#line 509 "lang/Grammar.acc"
 theMon->Attk[CurrAttk].u.a.DC = (int8)dc; 
# line 2004 "src/yygram.cpp"
               get_lexval();
               } break;
            default: ;
            }
#line 510 "lang/Grammar.acc"
 theMon->Attk[CurrAttk].AType = A_ALSO;
                                 theMon->Attk[CurrAttk].DType = dt;
                                 CurrAttk++;
                                 if (CurrAttk >= 12)
                                   Fatal("> 12 attacks on a single monster!");
                               
# line 2016 "src/yygram.cpp"
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
#line 517 "lang/Grammar.acc"
 theMon->Attk[CurrAttk].AType = (int8)at;
                                  theMon->Attk[CurrAttk].u.xID = rr; 
# line 2030 "src/yygram.cpp"
      switch (yyselect()) {
      case 593: {
         cexpr3(&dt);
#line 519 "lang/Grammar.acc"
 theMon->Attk[CurrAttk].DType = (int8)dt; 
# line 2036 "src/yygram.cpp"
         } break;
      default: ;
      }
#line 520 "lang/Grammar.acc"

                                 CurrAttk++;
                                 if (CurrAttk >= 32)
                                   yyerror("> 32 attacks on a single monster!");
                               
# line 2046 "src/yygram.cpp"
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
      case 594: {
         get_lexval();
         } break;
      default: ;
      }
#line 528 "lang/Grammar.acc"
 theMon->SetFlag((uint16)mf); 
# line 2066 "src/yygram.cpp"
      } break;
   case 106: {
      get_lexval();
      cexpr3(&mf);
      switch (yyselect()) {
      case 595: {
         get_lexval();
         } break;
      default: ;
      }
#line 529 "lang/Grammar.acc"
 theMon->UnsetFlag((uint16)mf); 
# line 2079 "src/yygram.cpp"
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
      case 596: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&v);
#line 533 "lang/Grammar.acc"
 theMon->Attr[at] = (int8)v; 
# line 2102 "src/yygram.cpp"
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
#line 537 "lang/Grammar.acc"
 theRes = theItem; 
                               theItem->u.w.Crit = 2; 
                               theItem->u.w.Threat = 1; 
                               theItem->hp = 15; 
# line 2120 "src/yygram.cpp"
      get_lexval();
      name = yylval;
#line 541 "lang/Grammar.acc"
 theItem->Name = name; 
# line 2125 "src/yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 542 "lang/Grammar.acc"
 theItem->IType = (int16)n; 
# line 2130 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 597: {
            item_entry();
            } break;
         default: goto yy20;
         }
      }
      yy20: ;
      get_lexval();
#line 543 "lang/Grammar.acc"
 theItem++; 
# line 2144 "src/yygram.cpp"
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
      case 598: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 547 "lang/Grammar.acc"
 theItem->Desc = lv; 
# line 2173 "src/yygram.cpp"
      } break;
   case 110: {
      get_lexval();
      switch (yyselect()) {
      case 599: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 548 "lang/Grammar.acc"
 theItem->Level = (int8)n; 
# line 2187 "src/yygram.cpp"
      } break;
   case 111: {
      glyph_entry(&img);
#line 549 "lang/Grammar.acc"
 theItem->Image = img; 
# line 2193 "src/yygram.cpp"
      } break;
   case 112: {
      get_lexval();
      switch (yyselect()) {
      case 600: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&dv);
      get_lexval();
#line 550 "lang/Grammar.acc"
 theItem->u.w.SDmg = yyDice(dv); 
# line 2207 "src/yygram.cpp"
      } break;
   case 113: {
      get_lexval();
      switch (yyselect()) {
      case 601: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&dv);
      get_lexval();
#line 551 "lang/Grammar.acc"
 theItem->u.w.LDmg = yyDice(dv); 
# line 2221 "src/yygram.cpp"
      } break;
   case 114: {
      get_lexval();
      switch (yyselect()) {
      case 602: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 552 "lang/Grammar.acc"
 theItem->u.w.Crit = (int8)n; 
# line 2235 "src/yygram.cpp"
      } break;
   case 115: {
      get_lexval();
      switch (yyselect()) {
      case 603: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 553 "lang/Grammar.acc"
 theItem->u.w.Threat = (int8)n; 
# line 2249 "src/yygram.cpp"
      } break;
   case 116: {
      get_lexval();
      switch (yyselect()) {
      case 604: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 554 "lang/Grammar.acc"
 theItem->u.w.Acc = (int8)n; 
# line 2263 "src/yygram.cpp"
      } break;
   case 117: {
      get_lexval();
      switch (yyselect()) {
      case 605: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 555 "lang/Grammar.acc"
 theItem->u.w.Spd = (int8)n; 
# line 2277 "src/yygram.cpp"
      } break;
   case 118: {
      get_lexval();
      switch (yyselect()) {
      case 606: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr4(&n);
      get_lexval();
      get_lexval();
#line 556 "lang/Grammar.acc"
 theItem->u.w.Spd = (int8)(n > 0 ? ((n-100)/5) : n/5); 
# line 2292 "src/yygram.cpp"
      } break;
   case 119: {
      get_lexval();
      switch (yyselect()) {
      case 607: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      cexpr4(&n);
      get_lexval();
      get_lexval();
#line 557 "lang/Grammar.acc"
 theItem->u.w.Spd = (int8)(n/5); 
# line 2308 "src/yygram.cpp"
      } break;
   case 120: {
      get_lexval();
      switch (yyselect()) {
      case 608: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 558 "lang/Grammar.acc"
 theItem->u.w.ParryMod = (int8)n; 
# line 2322 "src/yygram.cpp"
      } break;
   case 121: {
      get_lexval();
      switch (yyselect()) {
      case 609: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 559 "lang/Grammar.acc"
 theItem->u.w.RangeInc = (int8)n; 
# line 2336 "src/yygram.cpp"
      } break;
   case 122: {
      get_lexval();
      switch (yyselect()) {
      case 610: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&m);
      get_lexval();
#line 560 "lang/Grammar.acc"
 theItem->Material = (int8)m; 
# line 2350 "src/yygram.cpp"
      } break;
   case 123: {
      get_lexval();
      switch (yyselect()) {
      case 611: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
#line 561 "lang/Grammar.acc"
 theItem->Cost = n * 100; 
# line 2363 "src/yygram.cpp"
      switch (yyselect()) {
      case 612: {
         cexpr3(&unit);
#line 562 "lang/Grammar.acc"
 switch (unit)
                                         { case PLATINUM: theItem->Cost *= 5; break;
                                           case GOLD:     break;
                                           case ELECTRUM: theItem->Cost /= 2; break;
                                           case SILVER:   theItem->Cost /= 10; break;
                                           case COPPER:   theItem->Cost /= 100; break;
                                           
                                         }
                                     
# line 2377 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      } break;
   case 124: {
      get_lexval();
      switch (yyselect()) {
      case 613: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 572 "lang/Grammar.acc"
 theItem->Weight = (int16)n; 
    /* ww: according to the SRD, one inch (thickness) of iron has 30

     * hit point. We assume that the canonical long sword (at 4 lbs = 40

     * weight) is 1/2 inch thick. So a base weight of 80 gets you 30 hit

     * points. */
                               theItem->hp = (uint16)((n * 30) / 80); 
                               if (theItem->hp < 1) theItem->hp = 1; 
# line 2404 "src/yygram.cpp"
      } break;
   case 125: {
      get_lexval();
      switch (yyselect()) {
      case 614: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 579 "lang/Grammar.acc"
 theItem->Size = (int8)n; 
# line 2418 "src/yygram.cpp"
      } break;
   case 126: {
      get_lexval();
      switch (yyselect()) {
      case 615: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 580 "lang/Grammar.acc"
 theItem->Nutrition = (int8)n; 
# line 2432 "src/yygram.cpp"
      } break;
   case 127: {
      get_lexval();
      switch (yyselect()) {
      case 616: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 581 "lang/Grammar.acc"
 theItem->u.a.Def = (int8)n; 
# line 2446 "src/yygram.cpp"
      } break;
   case 128: {
      get_lexval();
      switch (yyselect()) {
      case 617: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 582 "lang/Grammar.acc"
 theItem->u.a.Cov = (int8)n; 
# line 2460 "src/yygram.cpp"
      } break;
   case 129: {
      get_lexval();
      switch (yyselect()) {
      case 618: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 583 "lang/Grammar.acc"
 theItem->u.a.Penalty = (int8)n; 
# line 2474 "src/yygram.cpp"
      } break;
   case 130: {
      get_lexval();
      switch (yyselect()) {
      case 619: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 620: {
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
      case 621: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 585 "lang/Grammar.acc"
 theItem->hp = (uint16)n; 
# line 2507 "src/yygram.cpp"
      } break;
   case 132: {
      get_lexval();
      switch (yyselect()) {
      case 622: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 624: {
            res_ref(&rr);
#line 586 "lang/Grammar.acc"
 theItem->AddResID(AN_SPELLS, rr); 
# line 2523 "src/yygram.cpp"
            switch (yyselect()) {
            case 623: {
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
      case 625: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 627: {
            res_ref(&rr);
#line 588 "lang/Grammar.acc"
 theItem->AddResID(AN_FIRES, rr); 
# line 2551 "src/yygram.cpp"
            switch (yyselect()) {
            case 626: {
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
      case 628: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 629: {
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
      case 630: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 633: {
            cexpr3(&n);
            switch (yyselect()) {
            case 631: {
               get_lexval();
               } break;
            case 632: {
               get_lexval();
               } break;
            default: ;
            }
#line 591 "lang/Grammar.acc"
 theItem->Group |= n; 
# line 2607 "src/yygram.cpp"
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
      case 634: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 592 "lang/Grammar.acc"
 theItem->u.c.Capacity = (int8)n; 
# line 2627 "src/yygram.cpp"
      } break;
   case 137: {
      get_lexval();
      switch (yyselect()) {
      case 635: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 593 "lang/Grammar.acc"
 theItem->u.c.WeightLim = (int16)n; 
# line 2641 "src/yygram.cpp"
      } break;
   case 138: {
      get_lexval();
      switch (yyselect()) {
      case 636: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 594 "lang/Grammar.acc"
 theItem->u.c.WeightMod = (int8)n; 
# line 2655 "src/yygram.cpp"
      } break;
   case 139: {
      get_lexval();
      switch (yyselect()) {
      case 637: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 595 "lang/Grammar.acc"
 theItem->u.c.MaxSize = (int8)n; 
# line 2669 "src/yygram.cpp"
      } break;
   case 140: {
      get_lexval();
      switch (yyselect()) {
      case 638: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 596 "lang/Grammar.acc"
 theItem->u.c.Timeout = (int8)n; 
# line 2683 "src/yygram.cpp"
      } break;
   case 141: {
      get_lexval();
      switch (yyselect()) {
      case 639: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 597 "lang/Grammar.acc"
 theItem->u.c.CType = (int8)n; 
# line 2697 "src/yygram.cpp"
      } break;
   case 142: {
      get_lexval();
      get_lexval();
      switch (yyselect()) {
      case 640: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 598 "lang/Grammar.acc"
 theItem->u.l.LightRange = (int8)n; 
# line 2712 "src/yygram.cpp"
      } break;
   case 143: {
      get_lexval();
      switch (yyselect()) {
      case 641: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 599 "lang/Grammar.acc"
 theItem->u.l.Lifespan = (int16)n; 
# line 2726 "src/yygram.cpp"
      } break;
   case 144: {
      get_lexval();
      switch (yyselect()) {
      case 642: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 600 "lang/Grammar.acc"
 theItem->u.l.Fuel = rr; 
# line 2740 "src/yygram.cpp"
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
      case 643: {
         get_lexval();
         } break;
      default: ;
      }
#line 607 "lang/Grammar.acc"
 if (theItem->u.a.Arm[0] == 0) theItem->u.a.Arm[0] = (int8)n;
                            if (theItem->u.a.Arm[1] == 0) theItem->u.a.Arm[1] = (int8)n;
                            if (theItem->u.a.Arm[2] == 0) theItem->u.a.Arm[2] = (int8)n;
                         
# line 2773 "src/yygram.cpp"
      } break;
   case 149: {
      get_lexval();
      wt = yylval;
      cexpr3(&n);
      switch (yyselect()) {
      case 644: {
         get_lexval();
         } break;
      default: ;
      }
#line 611 "lang/Grammar.acc"
 theItem->u.a.Arm[wt] = (int8)n; 
# line 2787 "src/yygram.cpp"
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
      case 645: {
         get_lexval();
         } break;
      default: ;
      }
#line 615 "lang/Grammar.acc"
 theItem->SetFlag((uint16)itf); 
# line 2807 "src/yygram.cpp"
      } break;
   case 151: {
      get_lexval();
      cexpr3(&itf);
      switch (yyselect()) {
      case 646: {
         get_lexval();
         } break;
      default: ;
      }
#line 616 "lang/Grammar.acc"
 theItem->UnsetFlag((uint16)itf); 
# line 2820 "src/yygram.cpp"
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
#line 622 "lang/Grammar.acc"
 theRes = theFeat; theFeat->Name = name; theRes = theFeat; 
# line 2837 "src/yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 623 "lang/Grammar.acc"
 theFeat->FType = (uint8)n; 
# line 2842 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 647: {
            feat_entry();
            } break;
         default: goto yy26;
         }
      }
      yy26: ;
      get_lexval();
#line 624 "lang/Grammar.acc"
 theFeat++; 
# line 2856 "src/yygram.cpp"
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
      case 648: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 628 "lang/Grammar.acc"
 theFeat->Desc = lv; 
# line 2885 "src/yygram.cpp"
      } break;
   case 154: {
      get_lexval();
      switch (yyselect()) {
      case 649: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&m);
      get_lexval();
#line 629 "lang/Grammar.acc"
 theFeat->Material = (int8)m; 
# line 2899 "src/yygram.cpp"
      } break;
   case 155: {
      glyph_entry(&img);
#line 630 "lang/Grammar.acc"
 theFeat->Image = img; 
# line 2905 "src/yygram.cpp"
      } break;
   case 156: {
      get_lexval();
      switch (yyselect()) {
      case 650: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mov);
      get_lexval();
      get_lexval();
#line 631 "lang/Grammar.acc"
 theFeat->MoveMod = (int8)((mov-100)/5); 
# line 2920 "src/yygram.cpp"
      } break;
   case 157: {
      get_lexval();
      switch (yyselect()) {
      case 651: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 632 "lang/Grammar.acc"
 theFeat->hp = (uint16)n; 
# line 2934 "src/yygram.cpp"
      } break;
   case 158: {
      get_lexval();
      switch (yyselect()) {
      case 652: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 655: {
            cexpr3(&n);
            switch (yyselect()) {
            case 653: {
               get_lexval();
               } break;
            case 654: {
               get_lexval();
               } break;
            default: ;
            }
#line 633 "lang/Grammar.acc"
 theFeat->Flags |= n; 
# line 2959 "src/yygram.cpp"
            } break;
         default: goto yy27;
         }
      }
      yy27: ;
      get_lexval();
      } break;
   case 159: {
      switch (yyselect()) {
      case 656: {
         get_lexval();
         } break;
      case 657: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 658: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 634 "lang/Grammar.acc"
 theFeat->xID = rr; 
# line 2987 "src/yygram.cpp"
      } break;
   case 160: {
      get_lexval();
      switch (yyselect()) {
      case 659: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 635 "lang/Grammar.acc"
 theFeat->xval = (int16)n; 
# line 3001 "src/yygram.cpp"
      } break;
   case 161: {
      get_lexval();
      switch (yyselect()) {
      case 660: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&d);
      get_lexval();
#line 636 "lang/Grammar.acc"
 theFeat->Factor = d; 
# line 3015 "src/yygram.cpp"
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
#line 642 "lang/Grammar.acc"
 theRes = theEff; theEff->ef.sval = NOSAVE; 
# line 3038 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 662: {
            cexpr3(&n);
#line 643 "lang/Grammar.acc"
 if (theEff->Sources[2])
                                        theEff->Sources[3] = (int8)n;
                                      else if (theEff->Sources[1])
                                        theEff->Sources[2] = (int8)n;
                                      else if (theEff->Sources[0])
                                        theEff->Sources[1] = (int8)n; 
                                      else
                                        theEff->Sources[0] = (int8)n; 
# line 3052 "src/yygram.cpp"
            switch (yyselect()) {
            case 661: {
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
      case 663: {
         get_lexval();
         } break;
      case 664: {
         get_lexval();
         } break;
      case 665: {
         get_lexval();
#line 653 "lang/Grammar.acc"
 theEff->ef.aval = AR_POISON; 
# line 3075 "src/yygram.cpp"
         } break;
      case 666: {
         get_lexval();
#line 654 "lang/Grammar.acc"
 theEff->ef.aval = AR_DISEASE; 
# line 3081 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      name = yylval;
#line 655 "lang/Grammar.acc"
 theEff->Name = name; 
# line 3089 "src/yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 656 "lang/Grammar.acc"
 theValsIdx = 0; 
                                      theEff->ef.eval = (int8)n; 
# line 3095 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 667: {
            eff_entry();
            } break;
         default: goto yy29;
         }
      }
      yy29: ;
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 669: {
            get_lexval();
            cexpr3(&n);
#line 659 "lang/Grammar.acc"
  Annotation *a; EffectValues tmp; int32 t;
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
                               a->u.ef.eval = (int8)n;
                             
# line 3133 "src/yygram.cpp"
            get_lexval();
            while (1) {
               switch (yyselect()) {
               case 668: {
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
#line 681 "lang/Grammar.acc"
 theEff++; 
# line 3152 "src/yygram.cpp"
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
      case 670: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 685 "lang/Grammar.acc"
 theEff->Desc = lv; 
# line 3181 "src/yygram.cpp"
      } break;
   case 167: {
      cexpr(&sc);
      get_lexval();
#line 686 "lang/Grammar.acc"
 theEff->Schools |= sc; 
# line 3188 "src/yygram.cpp"
      } break;
   case 168: {
      get_lexval();
      switch (yyselect()) {
      case 671: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 687 "lang/Grammar.acc"
 theEff->Level = (int8)n; 
# line 3202 "src/yygram.cpp"
      } break;
   case 169: {
      get_lexval();
      switch (yyselect()) {
      case 672: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 688 "lang/Grammar.acc"
 theEff->ManaCost = (uint8)n; 
# line 3216 "src/yygram.cpp"
      } break;
   case 170: {
      get_lexval();
      switch (yyselect()) {
      case 673: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 674: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 675: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 689 "lang/Grammar.acc"
 theEff->BaseChance = (uint8)n; 
# line 3242 "src/yygram.cpp"
      } break;
   case 171: {
      get_lexval();
      switch (yyselect()) {
      case 676: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
#line 690 "lang/Grammar.acc"
 theEff->Purpose = n; 
# line 3255 "src/yygram.cpp"
      switch (yyselect()) {
      case 677: {
         get_lexval();
         cexpr(&n2);
         get_lexval();
#line 691 "lang/Grammar.acc"
 theEff->Purpose |= n2<<16; 
# line 3263 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      } break;
   case 172: {
      get_lexval();
      switch (yyselect()) {
      case 678: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&sv);
      switch (yyselect()) {
      case 679: {
         get_lexval();
#line 692 "lang/Grammar.acc"
 theEff->SetFlag(EF_PARTIAL); 
# line 3283 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
#line 693 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->sval = (int8)sv; 
# line 3290 "src/yygram.cpp"
      } break;
   case 173: {
      get_lexval();
      switch (yyselect()) {
      case 680: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 694 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->qval = (int8)n; 
# line 3304 "src/yygram.cpp"
      } break;
   case 174: {
      get_lexval();
      switch (yyselect()) {
      case 681: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 695 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->xval = (uint8)n; 
# line 3318 "src/yygram.cpp"
      } break;
   case 175: {
      get_lexval();
      switch (yyselect()) {
      case 682: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 696 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->yval = (int16)n; 
# line 3332 "src/yygram.cpp"
      } break;
   case 176: {
      get_lexval();
      switch (yyselect()) {
      case 683: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 697 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->dval = (int8)n; 
# line 3346 "src/yygram.cpp"
      } break;
   case 177: {
      get_lexval();
      switch (yyselect()) {
      case 684: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&d);
      get_lexval();
#line 698 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->pval = d; 
# line 3360 "src/yygram.cpp"
      } break;
   case 178: {
      get_lexval();
      switch (yyselect()) {
      case 685: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 699 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->aval = (int8)n; 
# line 3374 "src/yygram.cpp"
      } break;
   case 179: {
      get_lexval();
      switch (yyselect()) {
      case 686: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 700 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->lval = (int8)n; 
# line 3388 "src/yygram.cpp"
      } break;
   case 180: {
      get_lexval();
      switch (yyselect()) {
      case 687: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 701 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->tval = (int8)n; 
# line 3402 "src/yygram.cpp"
      } break;
   case 181: {
      get_lexval();
      switch (yyselect()) {
      case 688: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 702 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->rval = rr; 
# line 3416 "src/yygram.cpp"
      } break;
   case 182: {
      get_lexval();
      switch (yyselect()) {
      case 689: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 703 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->rval = n; 
# line 3430 "src/yygram.cpp"
      } break;
   case 183: {
      get_lexval();
      switch (yyselect()) {
      case 690: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 704 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->cval = (int8)n; 
# line 3444 "src/yygram.cpp"
      } break;
   case 184: {
      get_lexval();
      switch (yyselect()) {
      case 691: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      n = yylval;
      get_lexval();
#line 705 "lang/Grammar.acc"
 ESTABLISH_VALS; theVals->cval = (int8)n; 
# line 3459 "src/yygram.cpp"
      } break;
   case 185: {
      get_lexval();
      switch (yyselect()) {
      case 692: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 695: {
            cexpr3(&n);
#line 706 "lang/Grammar.acc"
 theEff->SetFlag((uint16)n); 
# line 3475 "src/yygram.cpp"
            switch (yyselect()) {
            case 693: {
               get_lexval();
               } break;
            case 694: {
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

int numlist (YYSTYPE *val)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 189: {
#line 713 "lang/Grammar.acc"
 *val = 0; 
# line 3512 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 698: {
            cexpr3(&n);
#line 714 "lang/Grammar.acc"
 *val |= n; 
# line 3519 "src/yygram.cpp"
            switch (yyselect()) {
            case 696: {
               get_lexval();
               } break;
            case 697: {
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
#line 723 "lang/Grammar.acc"
 theRes = theRace; theRace->Name = name;
                                    theRace->BaseRace = 0; 
# line 3561 "src/yygram.cpp"
      switch (yyselect()) {
      case 699: {
         get_lexval();
         res_ref(&base);
#line 725 "lang/Grammar.acc"
 TRace tr; 
                                    theRace->BaseRace = base;                                        
                                    if (TRACE(base) > theRace)
                                      Fatal("Subrace definition before def of base race!"); 
# line 3571 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 700: {
            race_entry();
            } break;
         default: goto yy34;
         }
      }
      yy34: ;
      get_lexval();
#line 730 "lang/Grammar.acc"
 theRace++; 
# line 3588 "src/yygram.cpp"
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
      case 701: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 734 "lang/Grammar.acc"
 theRace->Desc = lv; 
# line 3623 "src/yygram.cpp"
      } break;
   case 193: {
      while (1) {
         switch (yyselect()) {
         case 704: {
            get_lexval();
            at = yylval;
            switch (yyselect()) {
            case 702: {
               get_lexval();
               } break;
            default: ;
            }
            cexpr(&v);
#line 735 "lang/Grammar.acc"
 theRace->AttrAdj[at] = (int8)v; 
# line 3640 "src/yygram.cpp"
            switch (yyselect()) {
            case 703: {
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
      case 705: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 707: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 706: {
               get_lexval();
               } break;
            default: ;
            }
#line 738 "lang/Grammar.acc"
 theRace->AddAbility((int8)AbT,(int16)Abil,p,(int8)l1,(int8)l2); 
# line 3674 "src/yygram.cpp"
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
      case 708: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 739 "lang/Grammar.acc"
 theRace->mID = rr; 
# line 3694 "src/yygram.cpp"
      } break;
   case 196: {
      get_lexval();
      switch (yyselect()) {
      case 709: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 710: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
      res_ref(&rr2);
      get_lexval();
#line 741 "lang/Grammar.acc"
 theRace->FavouredClass[0] = rr;
                                  theRace->FavouredClass[1] = rr2; 
# line 3717 "src/yygram.cpp"
      } break;
   case 197: {
      get_lexval();
      switch (yyselect()) {
      case 711: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 712: {
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
#line 744 "lang/Grammar.acc"
 theRace->FavouredClass[0] = rr;
                                  theRace->FavouredClass[1] = rr2; 
                                  theRace->FavouredClass[2] = rr3; 
# line 3743 "src/yygram.cpp"
      } break;
   case 198: {
      get_lexval();
      switch (yyselect()) {
      case 713: {
         get_lexval();
         } break;
      default: ;
      }
#line 747 "lang/Grammar.acc"
int8 skn = 0;
# line 3755 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 715: {
            cexpr3(&sk);
#line 748 "lang/Grammar.acc"
 theRace->Skills[skn++] = (int8)sk; 
# line 3762 "src/yygram.cpp"
            switch (yyselect()) {
            case 714: {
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
      case 716: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 749 "lang/Grammar.acc"
 switch(n) {
                                         case MALE_NAMES  : theRace-> MNames = lv; break;
                                         case FEMALE_NAMES: theRace-> FNames = lv; break;
                                         case FAMILY_NAMES: theRace-> SNames = lv; break;
                                         default: yyerror("Illegal name const in race!");
                                         } 
# line 3794 "src/yygram.cpp"
      } break;
   case 200: {
      get_lexval();
      switch (yyselect()) {
      case 717: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 720: {
            cexpr3(&n);
            switch (yyselect()) {
            case 718: {
               get_lexval();
               } break;
            case 719: {
               get_lexval();
               } break;
            default: ;
            }
#line 755 "lang/Grammar.acc"
 theRace->SetFlag((uint16)n); 
# line 3819 "src/yygram.cpp"
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
#line 763 "lang/Grammar.acc"
 theRes = theClass; theClass->Name = name; 
# line 3853 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 721: {
            class_entry();
            } break;
         default: goto yy39;
         }
      }
      yy39: ;
      get_lexval();
#line 764 "lang/Grammar.acc"
 theClass++; 
# line 3867 "src/yygram.cpp"
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
      case 722: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 768 "lang/Grammar.acc"
 theClass->Desc = lv; 
# line 3902 "src/yygram.cpp"
      } break;
   case 207: {
      get_lexval();
      switch (yyselect()) {
      case 723: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 724: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 769 "lang/Grammar.acc"
 theClass->HitDie = (uint8)n; 
# line 3922 "src/yygram.cpp"
      } break;
   case 208: {
      get_lexval();
      switch (yyselect()) {
      case 725: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 726: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 770 "lang/Grammar.acc"
 theClass->ManaDie = (uint8)n; 
# line 3942 "src/yygram.cpp"
      } break;
   case 209: {
      get_lexval();
      switch (yyselect()) {
      case 727: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&x);
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 771 "lang/Grammar.acc"
 theClass->DefMod = (uint8)n;
                                             if (x != 1) yyerror("class defense mod must by 1 per N."); 
# line 3959 "src/yygram.cpp"
      } break;
   case 210: {
      get_lexval();
      switch (yyselect()) {
      case 728: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 730: {
            cexpr3(&typ);
            cexpr3(&l);
#line 774 "lang/Grammar.acc"
 theClass->AttkVal[typ] = (uint8)l; 
# line 3976 "src/yygram.cpp"
            switch (yyselect()) {
            case 729: {
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
      case 731: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 733: {
            cexpr3(&typ);
            cexpr3(&l);
#line 776 "lang/Grammar.acc"
 theClass->Saves[typ] = (uint8)l; 
# line 4005 "src/yygram.cpp"
            switch (yyselect()) {
            case 732: {
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
      case 734: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 736: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 735: {
               get_lexval();
               } break;
            default: ;
            }
#line 778 "lang/Grammar.acc"
 theClass->AddAbility((int8)AbT,(int16)Abil,p,(int8)l1,(int8)l2); 
# line 4039 "src/yygram.cpp"
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
      case 737: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      cexpr3(&sp);
      switch (yyselect()) {
      case 738: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
#line 779 "lang/Grammar.acc"
 theClass->SkillPoints = (uint8)sp; int8 skn = 0; 
# line 4064 "src/yygram.cpp"
      switch (yyselect()) {
      case 739: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 741: {
            cexpr3(&sk);
#line 780 "lang/Grammar.acc"
 theClass->Skills[skn++] = (uint8)sk; 
# line 4077 "src/yygram.cpp"
            switch (yyselect()) {
            case 740: {
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
      case 742: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 744: {
            cexpr3(&pr);
#line 782 "lang/Grammar.acc"
 theClass->Proficiencies |= pr; 
# line 4105 "src/yygram.cpp"
            switch (yyselect()) {
            case 743: {
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
      case 745: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 748: {
            cexpr3(&n);
            switch (yyselect()) {
            case 746: {
               get_lexval();
               } break;
            case 747: {
               get_lexval();
               } break;
            default: ;
            }
#line 783 "lang/Grammar.acc"
 theClass->SetFlag((uint16)n); 
# line 4142 "src/yygram.cpp"
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

int special_ability (YYSTYPE *AbT, YYSTYPE *Abil, YYSTYPE *p, YYSTYPE *l1, YYSTYPE *l2)
{
   YYSTYPE ca;
   YYSTYPE xp;
   YYSTYPE xl1;
   YYSTYPE xl2;
   YYSTYPE st;
   YYSTYPE ft;
   switch(yyselect()) {
   case 220: {
#line 791 "lang/Grammar.acc"
 *p = 0; 
# line 4178 "src/yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&ca);
      switch (yyselect()) {
      case 749: {
         abil_param(&xp);
#line 791 "lang/Grammar.acc"
 *p = xp; 
# line 4187 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 792 "lang/Grammar.acc"
 *AbT = AB_ABILITY; *Abil = ca; *l1 =xl1; *l2 = xl2; 
# line 4196 "src/yygram.cpp"
      } break;
   case 221: {
#line 793 "lang/Grammar.acc"
 *p = 0; 
# line 4201 "src/yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&st);
      switch (yyselect()) {
      case 750: {
         abil_param(&xp);
#line 793 "lang/Grammar.acc"
 *p = xp; 
# line 4210 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 794 "lang/Grammar.acc"
 *AbT = AB_STATI;   *Abil = st; *l1 = xl1; *l2 = xl2; 
# line 4219 "src/yygram.cpp"
      } break;
   case 222: {
#line 795 "lang/Grammar.acc"
 *p = 0; 
# line 4224 "src/yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&ft);
      switch (yyselect()) {
      case 751: {
         abil_param(&xp);
#line 795 "lang/Grammar.acc"
 *p = xp; 
# line 4233 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 796 "lang/Grammar.acc"
 *AbT = AB_FEAT;    *Abil = ft; *l1 = xl1; *l2 = xl2; 
# line 4242 "src/yygram.cpp"
      } break;
   }
return 0;
}

int abil_param (YYSTYPE *p)
{
   YYSTYPE pa;
   rID rr;
   YYSTYPE pa2;
   switch(yyselect()) {
   case 223: {
      get_lexval();
      cexpr3(&pa);
#line 800 "lang/Grammar.acc"
 *p = pa; 
# line 4259 "src/yygram.cpp"
      } break;
   case 224: {
      get_lexval();
      res_ref(&rr);
#line 801 "lang/Grammar.acc"
 *p = rr; 
# line 4266 "src/yygram.cpp"
      } break;
   case 225: {
      get_lexval();
      cexpr3(&pa);
      get_lexval();
      cexpr3(&pa2);
#line 802 "lang/Grammar.acc"
 *p = pa | (pa2 << 16); 
# line 4275 "src/yygram.cpp"
      } break;
   }
return 0;
}

int abil_level (YYSTYPE *l1, YYSTYPE *l2)
{
   YYSTYPE xl1;
   YYSTYPE xl2;
   switch(yyselect()) {
   case 226: {
      cexpr3(&xl1);
      switch (yyselect()) {
      case 752: {
         get_lexval();
         } break;
      default: ;
      }
#line 807 "lang/Grammar.acc"
 *l1 = xl1; *l2 = 0; 
# line 4296 "src/yygram.cpp"
      } break;
   case 227: {
      get_lexval();
      cexpr3(&xl2);
      get_lexval();
#line 809 "lang/Grammar.acc"
 *l1 = 1;  *l2 = xl2; 
# line 4304 "src/yygram.cpp"
      } break;
   case 228: {
      get_lexval();
      get_lexval();
#line 811 "lang/Grammar.acc"
 *l1 = 1;  *l2 = 1; 
# line 4311 "src/yygram.cpp"
      } break;
   case 229: {
      get_lexval();
      get_lexval();
      get_lexval();
      get_lexval();
      cexpr3(&xl1);
      switch (yyselect()) {
      case 753: {
         get_lexval();
         } break;
      default: ;
      }
#line 813 "lang/Grammar.acc"
 *l1 = xl1; *l2 = 1; 
# line 4327 "src/yygram.cpp"
      } break;
   case 230: {
      get_lexval();
      cexpr3(&xl2);
      get_lexval();
      get_lexval();
      get_lexval();
      cexpr3(&xl1);
      switch (yyselect()) {
      case 754: {
         get_lexval();
         } break;
      default: ;
      }
#line 815 "lang/Grammar.acc"
 *l1 = xl1; *l2 = xl2; 
# line 4344 "src/yygram.cpp"
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
#line 819 "lang/Grammar.acc"
 theRes = theDgn; theDgn->Name = name; 
# line 4360 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 755: {
            dun_entry();
            } break;
         default: goto yy46;
         }
      }
      yy46: ;
      get_lexval();
#line 820 "lang/Grammar.acc"
 theDgn++; 
# line 4374 "src/yygram.cpp"
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
      case 756: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 824 "lang/Grammar.acc"
 theDgn->Desc = lv; 
# line 4397 "src/yygram.cpp"
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
      case 757: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 759: {
            get_lexval();
            cexpr3(&con);
            cexpr3(&val);
#line 832 "lang/Grammar.acc"
 theRes->AddConstant((int8)con,val); 
# line 4437 "src/yygram.cpp"
            switch (yyselect()) {
            case 758: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         case 761: {
            get_lexval();
            cexpr3(&con);
            res_ref(&rr);
#line 833 "lang/Grammar.acc"
 theRes->AddConstant((int8)con,rr); 
# line 4451 "src/yygram.cpp"
            switch (yyselect()) {
            case 760: {
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
      case 762: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 766: {
            get_lexval();
            cexpr4(&lis);
#line 836 "lang/Grammar.acc"
 TempListLoc = 0; 
# line 4480 "src/yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 763: {
                  list_entry();
                  } break;
               default: goto yy49;
               }
            }
            yy49: ;
            switch (yyselect()) {
            case 764: {
               get_lexval();
               } break;
            case 765: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
#line 837 "lang/Grammar.acc"
 TempList[TempListLoc++] = 0;
        TempList[TempListLoc++] = 0;
        TempList[TempListLoc++] = 0;
        ASSERT(TempListLoc < 1023);
        theRes->AddList((int8)lis,TempList); 
# line 4505 "src/yygram.cpp"
            } break;
         default: goto yy48;
         }
      }
      yy48: ;
      } break;
   case 238: {
      get_lexval();
      switch (yyselect()) {
      case 767: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 773: {
#line 844 "lang/Grammar.acc"
 int16 Chance = 100; 
# line 4525 "src/yygram.cpp"
            get_lexval();
            switch (yyselect()) {
            case 768: {
               cexpr4(&n);
               get_lexval();
#line 844 "lang/Grammar.acc"
 Chance = (int16)n; 
# line 4533 "src/yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 769: {
               res_ref(&rr);
               get_lexval();
               get_lexval();
               cexpr3(&n);
#line 845 "lang/Grammar.acc"
 theRes->AddSpecial(rr,Chance,(int16)n); 
# line 4545 "src/yygram.cpp"
               } break;
            case 770: {
               res_ref(&rr);
               get_lexval();
               cexpr3(&n);
               get_lexval();
#line 846 "lang/Grammar.acc"
 theRes->AddSpecial(rr,Chance,(int16)n); 
# line 4554 "src/yygram.cpp"
               } break;
            case 771: {
               res_ref(&rr);
               get_lexval();
               get_lexval();
               dice_only(&dv);
#line 847 "lang/Grammar.acc"
 theRes->AddSpecial(rr,Chance,yyDice(dv)); 
# line 4563 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 772: {
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
#line 852 "lang/Grammar.acc"
 TempList[TempListLoc++] = rr; ASSERT(TempListLoc < 1023); 
# line 4594 "src/yygram.cpp"
      } break;
   case 240: {
      cexpr4(&n);
#line 853 "lang/Grammar.acc"
 TempList[TempListLoc++] = n;  ASSERT(TempListLoc < 1023); 
# line 4600 "src/yygram.cpp"
      } break;
   case 241: {
      get_lexval();
      text = yylval;
#line 854 "lang/Grammar.acc"
 TempList[TempListLoc++] = text; ASSERT(TempListLoc < 1023); 
# line 4607 "src/yygram.cpp"
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
#line 858 "lang/Grammar.acc"
 theRes = theArti; theArti->Name = name; 
# line 4623 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 774: {
            arti_entry();
            } break;
         default: goto yy51;
         }
      }
      yy51: ;
      get_lexval();
#line 859 "lang/Grammar.acc"
 theArti++; 
# line 4637 "src/yygram.cpp"
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
      case 775: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 863 "lang/Grammar.acc"
 theRes = theArti; theArti->Desc = lv; 
# line 4665 "src/yygram.cpp"
      } break;
   case 244: {
      get_lexval();
      switch (yyselect()) {
      case 776: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 864 "lang/Grammar.acc"
 theArti->iID = rr; 
# line 4679 "src/yygram.cpp"
      } break;
   case 245: {
      while (1) {
         switch (yyselect()) {
         case 779: {
            get_lexval();
            at = yylval;
            switch (yyselect()) {
            case 777: {
               get_lexval();
               } break;
            default: ;
            }
            cexpr3(&v);
#line 865 "lang/Grammar.acc"
 theArti->AttrAdjLow[at] = (int8)v; 
# line 4696 "src/yygram.cpp"
            switch (yyselect()) {
            case 778: {
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
      case 780: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 782: {
            cexpr3(&res);
#line 866 "lang/Grammar.acc"
 theArti->Resists |= res; 
# line 4724 "src/yygram.cpp"
            switch (yyselect()) {
            case 781: {
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
      case 783: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 785: {
            get_lexval();
            at = yylval;
#line 867 "lang/Grammar.acc"
 theArti->Sustains |= 1 << (at-1); 
# line 4753 "src/yygram.cpp"
            switch (yyselect()) {
            case 784: {
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
      case 786: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 788: {
            cexpr3(&qu);
#line 868 "lang/Grammar.acc"
 theArti->Qualities |= qu; 
# line 4781 "src/yygram.cpp"
            switch (yyselect()) {
            case 787: {
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
#line 870 "lang/Grammar.acc"
 ArtPowerType = AN_ART_EQUIP; 
# line 4799 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 790: {
            art_power();
            switch (yyselect()) {
            case 789: {
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
#line 871 "lang/Grammar.acc"
 ArtPowerType = AN_ART_WIELD; 
# line 4822 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 792: {
            art_power();
            switch (yyselect()) {
            case 791: {
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
#line 872 "lang/Grammar.acc"
 ArtPowerType = AN_ART_HIT;   
# line 4845 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 794: {
            art_power();
            switch (yyselect()) {
            case 793: {
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
#line 873 "lang/Grammar.acc"
 ArtPowerType = AN_ART_INVOKE;
# line 4868 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 796: {
            art_power();
            switch (yyselect()) {
            case 795: {
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
#line 880 "lang/Grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_STATI,st,(int16)p,NULL); 
# line 4915 "src/yygram.cpp"
      } break;
   case 256: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
#line 882 "lang/Grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_STATI,st,0,NULL); 
# line 4924 "src/yygram.cpp"
      } break;
   case 257: {
      res_ref(&rr);
      switch (yyselect()) {
      case 797: {
         get_lexval();
         get_lexval();
         } break;
      default: ;
      }
#line 884 "lang/Grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_EFFECT,rr,0,NULL); 
# line 4937 "src/yygram.cpp"
      } break;
   case 258: {
      res_ref(&rr);
      get_lexval();
      dice_val(&dv);
      get_lexval();
#line 886 "lang/Grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_EFFECT,rr,0,&yyDice(dv)); 
# line 4946 "src/yygram.cpp"
      } break;
   case 259: {
      res_ref(&rr);
      cexpr3(&n);
      switch (yyselect()) {
      case 798: {
         get_lexval();
         } break;
      case 799: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
#line 888 "lang/Grammar.acc"
 Dice d; d.Set(0,(int8)n,1);
        theArti->AddPower((int8)ArtPowerType,AB_EFFECT,rr,0,&d); 
# line 4964 "src/yygram.cpp"
      } break;
   case 260: {
      res_ref(&rr);
      cexpr3(&n);
      switch (yyselect()) {
      case 800: {
         get_lexval();
         } break;
      case 801: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
#line 891 "lang/Grammar.acc"
 Dice d; d.Set(0,(int8)n,7);
        theArti->AddPower((int8)ArtPowerType,AB_EFFECT,rr,0,&d); 
# line 4982 "src/yygram.cpp"
      } break;
   case 261: {
      get_lexval();
      res_ref(&rr);
#line 894 "lang/Grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_INNATE,rr,0,NULL); 
# line 4989 "src/yygram.cpp"
      } break;
   case 262: {
      get_lexval();
      cexpr3(&ft);
#line 896 "lang/Grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_FEAT,ft,0,NULL); 
# line 4996 "src/yygram.cpp"
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
#line 900 "lang/Grammar.acc"
 theRes = theDom; theDom->Name = name; theDom->DType = (int8)type; 
# line 5015 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 802: {
            domain_entry();
            } break;
         default: goto yy60;
         }
      }
      yy60: ;
      get_lexval();
#line 902 "lang/Grammar.acc"
 theDom++; 
# line 5029 "src/yygram.cpp"
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
#line 906 "lang/Grammar.acc"
 int8 sp = 0; 
# line 5048 "src/yygram.cpp"
      get_lexval();
      switch (yyselect()) {
      case 803: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 805: {
            res_ref(&rr);
#line 907 "lang/Grammar.acc"
 theDom->Spells[min(sp,8)] = rr; sp++; 
# line 5062 "src/yygram.cpp"
            switch (yyselect()) {
            case 804: {
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
#line 908 "lang/Grammar.acc"
 if (sp > 9) 
      { yyerror(Format("Error: Domain %s has %d spells; must have at most nine.",
        theModule->QTextSeg + theDom->Name,sp)); } 
# line 5079 "src/yygram.cpp"
      } break;
   case 265: {
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
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 807: {
               get_lexval();
               } break;
            default: ;
            }
#line 912 "lang/Grammar.acc"
 theDom->AddAbility((int8)AbT,(int16)Abil,p,(int8)l1,(int8)l2); 
# line 5101 "src/yygram.cpp"
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
      case 809: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 913 "lang/Grammar.acc"
 theDom->Desc = lv; 
# line 5122 "src/yygram.cpp"
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
#line 919 "lang/Grammar.acc"
 theRes = theGod; theGod->Name = name; 
# line 5147 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 810: {
            god_entry();
            } break;
         default: goto yy63;
         }
      }
      yy63: ;
      get_lexval();
#line 921 "lang/Grammar.acc"
 theGod++; 
# line 5161 "src/yygram.cpp"
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
#line 925 "lang/Grammar.acc"
 int8 dm = 0; 
# line 5181 "src/yygram.cpp"
      get_lexval();
      switch (yyselect()) {
      case 811: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 813: {
            res_ref(&rr);
#line 926 "lang/Grammar.acc"
 theGod->Domains[min(dm,5)] = rr; dm++; 
# line 5195 "src/yygram.cpp"
            switch (yyselect()) {
            case 812: {
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
#line 927 "lang/Grammar.acc"
 if (dm < 3 || dm > 6) 
      { yyerror(Format("Error: God %s has %d domains; must have between 3 and 6.",
        theModule->QTextSeg + theGod->Name,dm)); } 
# line 5212 "src/yygram.cpp"
      } break;
   case 272: {
      get_lexval();
      get_lexval();
      } break;
   case 273: {
      get_lexval();
      switch (yyselect()) {
      case 814: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 817: {
            cexpr3(&n);
            switch (yyselect()) {
            case 815: {
               get_lexval();
               } break;
            case 816: {
               get_lexval();
               } break;
            default: ;
            }
#line 931 "lang/Grammar.acc"
 theGod->SetFlag((uint16)n); 
# line 5241 "src/yygram.cpp"
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
      case 818: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 932 "lang/Grammar.acc"
 theGod->Desc = lv; 
# line 5262 "src/yygram.cpp"
      } break;
   case 275: {
      get_lexval();
      switch (yyselect()) {
      case 819: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 821: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 820: {
               get_lexval();
               } break;
            default: ;
            }
#line 934 "lang/Grammar.acc"
 theGod->AddAbility((int8)AbT,(int16)Abil,p,(int8)l1,(int8)l2); 
# line 5284 "src/yygram.cpp"
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
#line 940 "lang/Grammar.acc"
 theRes = theTer; 
                                      theTer->Name = name; 
# line 5316 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 822: {
            terrain_entry();
            } break;
         default: goto yy67;
         }
      }
      yy67: ;
      get_lexval();
#line 943 "lang/Grammar.acc"
 theTer++; 
# line 5330 "src/yygram.cpp"
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
      case 823: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 947 "lang/Grammar.acc"
 theTer->Desc = lv; 
# line 5358 "src/yygram.cpp"
      } break;
   case 281: {
      glyph_entry(&img);
#line 948 "lang/Grammar.acc"
 theTer->Image = img; 
# line 5364 "src/yygram.cpp"
      } break;
   case 282: {
      get_lexval();
      switch (yyselect()) {
      case 824: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mov);
      get_lexval();
      get_lexval();
#line 949 "lang/Grammar.acc"
 theTer->MoveMod = (int8)((mov-100)/5); 
# line 5379 "src/yygram.cpp"
      } break;
   case 283: {
      get_lexval();
      switch (yyselect()) {
      case 825: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&pen);
      get_lexval();
#line 950 "lang/Grammar.acc"
 theTer->Penalty = (int8)pen; 
# line 5393 "src/yygram.cpp"
      } break;
   case 284: {
      get_lexval();
      switch (yyselect()) {
      case 826: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mat);
      get_lexval();
#line 951 "lang/Grammar.acc"
 theTer->Material = (int8)mat; 
# line 5407 "src/yygram.cpp"
      } break;
   case 285: {
      get_lexval();
      switch (yyselect()) {
      case 827: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 830: {
            cexpr3(&n);
            switch (yyselect()) {
            case 828: {
               get_lexval();
               } break;
            case 829: {
               get_lexval();
               } break;
            default: ;
            }
#line 952 "lang/Grammar.acc"
 theTer->SetFlag((uint16)n); 
# line 5432 "src/yygram.cpp"
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
#line 959 "lang/Grammar.acc"
 theRes = theReg; theReg->Name = name; theReg->SetFlag((uint16)regflag); 
    theReg->Size = SZ_COLLOSAL; 
# line 5467 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 831: {
            region_entry();
            } break;
         default: goto yy69;
         }
      }
      yy69: ;
      get_lexval();
#line 962 "lang/Grammar.acc"
 theReg++; 
# line 5481 "src/yygram.cpp"
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
      case 832: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 966 "lang/Grammar.acc"
 theReg->Desc = lv; 
# line 5512 "src/yygram.cpp"
      } break;
   case 291: {
      get_lexval();
      switch (yyselect()) {
      case 833: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 967 "lang/Grammar.acc"
 theReg->Walls = rr; 
# line 5526 "src/yygram.cpp"
      } break;
   case 292: {
      get_lexval();
      switch (yyselect()) {
      case 834: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 968 "lang/Grammar.acc"
 theReg->Floor = rr; 
# line 5540 "src/yygram.cpp"
      } break;
   case 293: {
      get_lexval();
      switch (yyselect()) {
      case 835: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 969 "lang/Grammar.acc"
 theReg->Door = rr; 
# line 5554 "src/yygram.cpp"
      } break;
   case 294: {
      get_lexval();
      switch (yyselect()) {
      case 836: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 970 "lang/Grammar.acc"
 theReg->Depth = (int8)n; 
# line 5568 "src/yygram.cpp"
      } break;
   case 295: {
      get_lexval();
      switch (yyselect()) {
      case 837: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 971 "lang/Grammar.acc"
 theReg->Size = (int8)n; 
# line 5582 "src/yygram.cpp"
      } break;
   case 296: {
      get_lexval();
      switch (yyselect()) {
      case 838: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 841: {
            cexpr3(&n);
#line 972 "lang/Grammar.acc"
 theReg->RoomTypes |= BIT(n); 
# line 5598 "src/yygram.cpp"
            switch (yyselect()) {
            case 839: {
               get_lexval();
               } break;
            case 840: {
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
      case 842: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 845: {
            cexpr3(&n);
            switch (yyselect()) {
            case 843: {
               get_lexval();
               } break;
            case 844: {
               get_lexval();
               } break;
            default: ;
            }
#line 974 "lang/Grammar.acc"
 theReg->SetFlag((uint16)n); 
# line 5638 "src/yygram.cpp"
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
#line 975 "lang/Grammar.acc"
 theReg->sx   = yyMapSize[currMap] % 256; 
                                    theReg->sy   = yyMapSize[currMap++] / 256;
                                    theReg->Grid = mg; 
# line 5656 "src/yygram.cpp"
      } break;
   case 299: {
      get_lexval();
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 858: {
#line 978 "lang/Grammar.acc"
 rID tID, xID, xID2; Glyph Img; uint8 fl; 
                                    tID = xID = xID2 = 0; Img = 0; fl = 0; 
# line 5667 "src/yygram.cpp"
            get_lexval();
            ch = yylval;
            get_lexval();
            res_ref(&rr);
#line 980 "lang/Grammar.acc"
 tID = rr; 
# line 5674 "src/yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 846: {
                  get_lexval();
                  cexpr(&n);
                  get_lexval();
#line 981 "lang/Grammar.acc"
 fl |= n; 
# line 5683 "src/yygram.cpp"
                  } break;
               case 855: {
                  get_lexval();
                  switch (yyselect()) {
                  case 847: {
                     res_ref(&rr);
#line 982 "lang/Grammar.acc"
 xID = rr; 
# line 5692 "src/yygram.cpp"
                     } break;
                  case 849: {
                     get_lexval();
                     get_lexval();
                     switch (yyselect()) {
                     case 848: {
                        cexpr(&v);
#line 983 "lang/Grammar.acc"
 xID = v; 
# line 5702 "src/yygram.cpp"
                        } break;
                     default: ;
                     }
#line 984 "lang/Grammar.acc"
 fl |= TILE_RANDOM | TILE_ITEM; 
# line 5708 "src/yygram.cpp"
                     } break;
                  case 851: {
                     get_lexval();
                     get_lexval();
                     switch (yyselect()) {
                     case 850: {
                        cexpr(&v);
#line 985 "lang/Grammar.acc"
 xID = v; 
# line 5718 "src/yygram.cpp"
                        } break;
                     default: ;
                     }
#line 986 "lang/Grammar.acc"
 fl |= TILE_RANDOM | TILE_MONSTER; 
# line 5724 "src/yygram.cpp"
                     } break;
                  default: printf("???\n"); exit(1);
                  }
                  switch (yyselect()) {
                  case 852: {
                     get_lexval();
                     res_ref(&rr2);
#line 987 "lang/Grammar.acc"
 xID2 = rr2; 
# line 5734 "src/yygram.cpp"
                     } break;
                  case 853: {
                     get_lexval();
                     get_lexval();
                     cexpr(&v);
                     get_lexval();
                     cexpr(&v2);
#line 989 "lang/Grammar.acc"
 fl |= TILE_RANDOM_EFF; xID2 = v+v2*256; 
# line 5744 "src/yygram.cpp"
                     } break;
                  case 854: {
                     get_lexval();
                     cexpr(&v);
#line 990 "lang/Grammar.acc"
 xID2 = v; 
# line 5751 "src/yygram.cpp"
                     } break;
                  default: ;
                  }
                  } break;
               case 856: {
                  get_lexval();
                  glyph_desc(&g);
#line 991 "lang/Grammar.acc"
 Img = g; 
# line 5761 "src/yygram.cpp"
                  } break;
               default: goto yy73;
               }
            }
            yy73: ;
#line 992 "lang/Grammar.acc"
 theReg->AddTile((char)ch,Img,fl,tID,xID,xID2); 
# line 5769 "src/yygram.cpp"
            switch (yyselect()) {
            case 857: {
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
         case 859: {
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
         case 860: {
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
#line 1002 "lang/Grammar.acc"
 theRes = theTemp; theTemp->Name = name; CurrFeat = 0; CurrAttk = 0; 
# line 5834 "src/yygram.cpp"
      get_lexval();
      cexpr3(&ty);
#line 1003 "lang/Grammar.acc"
 theTemp->TType = (uint16)ty; 
# line 5839 "src/yygram.cpp"
      switch (yyselect()) {
      case 861: {
         get_lexval();
         cexpr3(&mt);
#line 1004 "lang/Grammar.acc"
 theTemp->ForMType = mt; 
# line 5846 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 862: {
            template_entry();
            } break;
         default: goto yy76;
         }
      }
      yy76: ;
      get_lexval();
#line 1006 "lang/Grammar.acc"
 theTemp++; 
# line 5863 "src/yygram.cpp"
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
      case 863: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 1010 "lang/Grammar.acc"
 theTemp->Desc = lv; 
# line 5897 "src/yygram.cpp"
      } break;
   case 307: {
      get_lexval();
      switch (yyselect()) {
      case 864: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mt);
      get_lexval();
#line 1011 "lang/Grammar.acc"
 theTemp->ForMType = mt; 
# line 5911 "src/yygram.cpp"
      } break;
   case 308: {
      get_lexval();
      switch (yyselect()) {
      case 865: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mt);
      get_lexval();
#line 1012 "lang/Grammar.acc"
 theTemp->AddMType = mt; 
# line 5925 "src/yygram.cpp"
      } break;
   case 309: {
      glyph_entry(&img);
#line 1013 "lang/Grammar.acc"
 theTemp->NewImage = img; 
# line 5931 "src/yygram.cpp"
      } break;
   case 310: {
      switch (yyselect()) {
      case 866: {
         get_lexval();
         } break;
      case 867: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 868: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1014 "lang/Grammar.acc"
 theTemp->CR = mv; 
# line 5953 "src/yygram.cpp"
      } break;
   case 311: {
      get_lexval();
      switch (yyselect()) {
      case 869: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1015 "lang/Grammar.acc"
 theTemp->HitDice = mv; 
# line 5967 "src/yygram.cpp"
      } break;
   case 312: {
      get_lexval();
      switch (yyselect()) {
      case 870: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1016 "lang/Grammar.acc"
 theTemp->Hit = mv; 
# line 5981 "src/yygram.cpp"
      } break;
   case 313: {
      get_lexval();
      switch (yyselect()) {
      case 871: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1017 "lang/Grammar.acc"
 theTemp->Def = mv; 
# line 5995 "src/yygram.cpp"
      } break;
   case 314: {
      get_lexval();
      switch (yyselect()) {
      case 872: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1018 "lang/Grammar.acc"
 theTemp->Arm = mv; 
# line 6009 "src/yygram.cpp"
      } break;
   case 315: {
      get_lexval();
      switch (yyselect()) {
      case 873: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 1019 "lang/Grammar.acc"
 theTemp->DmgMod = (int8)n; 
# line 6023 "src/yygram.cpp"
      } break;
   case 316: {
      get_lexval();
      switch (yyselect()) {
      case 874: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1020 "lang/Grammar.acc"
 theTemp->Power = mv; 
# line 6037 "src/yygram.cpp"
      } break;
   case 317: {
      get_lexval();
      get_lexval();
      switch (yyselect()) {
      case 875: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1021 "lang/Grammar.acc"
 theTemp->CasterLev = mv; 
# line 6052 "src/yygram.cpp"
      } break;
   case 318: {
      get_lexval();
      switch (yyselect()) {
      case 876: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1022 "lang/Grammar.acc"
 theTemp->Size = mv; 
# line 6066 "src/yygram.cpp"
      } break;
   case 319: {
      get_lexval();
      switch (yyselect()) {
      case 877: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1023 "lang/Grammar.acc"
 theTemp->Spd = mv; 
# line 6080 "src/yygram.cpp"
      } break;
   case 320: {
      get_lexval();
      switch (yyselect()) {
      case 878: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1024 "lang/Grammar.acc"
 theTemp->Mov = mv; 
# line 6094 "src/yygram.cpp"
      } break;
   case 321: {
      get_lexval();
      at = yylval;
      switch (yyselect()) {
      case 879: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      switch (yyselect()) {
      case 880: {
         get_lexval();
         } break;
      case 881: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
#line 1025 "lang/Grammar.acc"
 theTemp->Attr[at] = mv; 
# line 6117 "src/yygram.cpp"
      } break;
   case 322: {
      get_lexval();
      switch (yyselect()) {
      case 882: {
         get_lexval();
         } break;
      default: ;
      }
      temp_attack_entry();
      while (1) {
         switch (yyselect()) {
         case 883: {
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
#line 1029 "lang/Grammar.acc"
 if (!theTemp->GainStati((uint8)st)) yyerror("too many Stati in template"); 
# line 6148 "src/yygram.cpp"
      } break;
   case 324: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      get_lexval();
#line 1031 "lang/Grammar.acc"
 if (!theTemp->GainStati((uint8)st,(int16)stV)) yyerror("too many Stati in template"); 
# line 6160 "src/yygram.cpp"
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
#line 1033 "lang/Grammar.acc"
 if (!theTemp->GainStati((uint8)st,(int16)stV,(int16)stM)) 
        yyerror("too many Stati in template"); 
# line 6175 "src/yygram.cpp"
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
#line 1036 "lang/Grammar.acc"
 if (!theTemp->GainStati((uint8)st,(int16)stV,(int16)stM,rr)) 
        yyerror("too many Stati in template"); 
# line 6192 "src/yygram.cpp"
      } break;
   case 327: {
      get_lexval();
      switch (yyselect()) {
      case 884: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 890: {
            switch (yyselect()) {
            case 885: {
               cexpr3(&res);
#line 1039 "lang/Grammar.acc"
 theTemp->AddRes |= res; 
# line 6210 "src/yygram.cpp"
               } break;
            case 888: {
               switch (yyselect()) {
               case 886: {
                  get_lexval();
                  } break;
               case 887: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&res);
#line 1040 "lang/Grammar.acc"
 theTemp->SubRes |= res; 
# line 6225 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 889: {
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
      case 891: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 897: {
            switch (yyselect()) {
            case 892: {
               cexpr3(&res);
#line 1042 "lang/Grammar.acc"
 theTemp->AddImm |= res; 
# line 6258 "src/yygram.cpp"
               } break;
            case 895: {
               switch (yyselect()) {
               case 893: {
                  get_lexval();
                  } break;
               case 894: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&res);
#line 1043 "lang/Grammar.acc"
 theTemp->SubImm |= res; 
# line 6273 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 896: {
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
      case 898: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 899: {
            cexpr3(&n);
            get_lexval();
#line 1045 "lang/Grammar.acc"
 theTemp->NewFeats[CurrFeat++] = (uint16)n; 
# line 6305 "src/yygram.cpp"
            } break;
         default: goto yy80;
         }
      }
      yy80: ;
      cexpr3(&n);
      get_lexval();
#line 1046 "lang/Grammar.acc"
 theTemp->NewFeats[CurrFeat++] = (uint16)n; 
# line 6315 "src/yygram.cpp"
      } break;
   case 330: {
      get_lexval();
      switch (yyselect()) {
      case 900: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 902: {
            res_ref(&rr);
#line 1047 "lang/Grammar.acc"
 theTemp->AddsFlag(M_INNATE);
                                       theTemp->AddResID(AN_INNATE, rr); 
# line 6332 "src/yygram.cpp"
            switch (yyselect()) {
            case 901: {
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
      case 903: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 905: {
            res_ref(&rr);
#line 1050 "lang/Grammar.acc"
 theTemp->AddResID(AN_DISEASE, rr); 
# line 6360 "src/yygram.cpp"
            switch (yyselect()) {
            case 904: {
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
      case 906: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 908: {
            res_ref(&rr);
#line 1052 "lang/Grammar.acc"
 theTemp->AddResID(AN_POISON, rr); 
# line 6388 "src/yygram.cpp"
            switch (yyselect()) {
            case 907: {
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
      case 909: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 916: {
            switch (yyselect()) {
            case 910: {
               cexpr3(&fl);
#line 1055 "lang/Grammar.acc"
 if (fl < 0) 
                                         theTemp->Flags[(-fl)/8] |= 1 << ((-fl)%8);
                                       else
                                         theTemp->AddFlags[fl/8] |= 1 << (fl % 8); 
                                     
# line 6422 "src/yygram.cpp"
               } break;
            case 913: {
               switch (yyselect()) {
               case 911: {
                  get_lexval();
                  } break;
               case 912: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&fl);
#line 1060 "lang/Grammar.acc"
 theTemp->SubFlags[fl/8] |= 1 << (fl % 8); 
# line 6437 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 914: {
               get_lexval();
               } break;
            case 915: {
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
#line 1068 "lang/Grammar.acc"
 uint8 dt = AD_NORM; dam.Set(0,0,0);
                                 theTemp->NewAttk[CurrAttk].u.a.Dmg.Set(0,0,0);
# line 6488 "src/yygram.cpp"
      cexpr3(&at);
      get_lexval();
      switch (yyselect()) {
      case 917: {
         dice_only(&dam);
#line 1071 "lang/Grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam); 
# line 6496 "src/yygram.cpp"
         } break;
      case 918: {
         cexpr3(&chance);
         get_lexval();
#line 1072 "lang/Grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg.Bonus = (int8)chance; 
# line 6503 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 919: {
         cexpr3(&n);
#line 1073 "lang/Grammar.acc"
 dt = (uint8)n; 
# line 6512 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 920: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 924: {
         get_lexval();
         switch (yyselect()) {
         case 921: {
            get_lexval();
            } break;
         case 922: {
            get_lexval();
            } break;
         case 923: {
            get_lexval();
            } break;
         default: printf("???\n"); exit(1);
         }
         cexpr3(&dc);
#line 1076 "lang/Grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.DC = (int8)dc; 
# line 6540 "src/yygram.cpp"
         get_lexval();
         } break;
      default: ;
      }
#line 1077 "lang/Grammar.acc"
 theTemp->NewAttk[CurrAttk].AType = (int8)at;
                                 theTemp->NewAttk[CurrAttk].DType = dt;
                                 /*

                                 if (at == A_ABIL)

                                   theTemp->NewAttk[CurrAttk].u.a.DC = max(

                                        theTemp->NewAttk[CurrAttk].u.a.DC, 1);

                                        */
                                 CurrAttk++;
                                 if (CurrAttk >= 16)
                                   yyerror("> 16 attacks on a single template!");
                               
# line 6561 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 932: {
            get_lexval();
#line 1088 "lang/Grammar.acc"
 dt = AD_NORM; theTemp->NewAttk[CurrAttk].u.a.Dmg.Set(0,0,0); dam2.Set(0,0,0); 
# line 6568 "src/yygram.cpp"
            switch (yyselect()) {
            case 925: {
               dice_only(&dam2);
#line 1089 "lang/Grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam2); 
# line 6574 "src/yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 926: {
               cexpr3(&n2);
#line 1090 "lang/Grammar.acc"
 dt = (uint8)n2; 
# line 6583 "src/yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 927: {
               get_lexval();
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 931: {
               get_lexval();
               switch (yyselect()) {
               case 928: {
                  get_lexval();
                  } break;
               case 929: {
                  get_lexval();
                  } break;
               case 930: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&dc);
#line 1092 "lang/Grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.DC = (int8)dc; 
# line 6611 "src/yygram.cpp"
               get_lexval();
               } break;
            default: ;
            }
#line 1093 "lang/Grammar.acc"
 theTemp->NewAttk[CurrAttk].AType = A_ALSO;
                                 theTemp->NewAttk[CurrAttk].DType = dt;
                                 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam2);
                                 CurrAttk++;
                                 if (CurrAttk >= 16)
                                   Fatal("> 16 attacks on a single monster!");
                               
# line 6624 "src/yygram.cpp"
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
#line 1101 "lang/Grammar.acc"
 theTemp->NewAttk[CurrAttk].AType = (int8)at;
                                  theTemp->NewAttk[CurrAttk].u.xID = rr; 
# line 6638 "src/yygram.cpp"
      switch (yyselect()) {
      case 933: {
         cexpr3(&dt);
#line 1103 "lang/Grammar.acc"
 theTemp->NewAttk[CurrAttk].DType = (int8)dt; 
# line 6644 "src/yygram.cpp"
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
#line 1110 "lang/Grammar.acc"
 theRes = theFlavor; 
                                         theFlavor->Name = name;
                                         theFlavor->IType = (int8)it;
                                         theFlavor->Material = -1;
                                         theFlavor->Color = -1;
                                         theFlavor->Weight = 20; 
# line 6681 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 934: {
            flavor_entry();
            } break;
         default: goto yy86;
         }
      }
      yy86: ;
      get_lexval();
#line 1116 "lang/Grammar.acc"
 theFlavor++; 
# line 6695 "src/yygram.cpp"
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
      case 935: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 1120 "lang/Grammar.acc"
 theFlavor->Desc     = lv; 
# line 6720 "src/yygram.cpp"
      } break;
   case 343: {
      get_lexval();
      switch (yyselect()) {
      case 936: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 1121 "lang/Grammar.acc"
 theFlavor->Material = (int8)n; 
# line 6734 "src/yygram.cpp"
      } break;
   case 344: {
      get_lexval();
      switch (yyselect()) {
      case 937: {
         get_lexval();
         } break;
      default: ;
      }
      color(&col);
      get_lexval();
#line 1122 "lang/Grammar.acc"
 theFlavor->Color    = (int8)col; 
# line 6748 "src/yygram.cpp"
      } break;
   case 345: {
      get_lexval();
      switch (yyselect()) {
      case 938: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
      get_lexval();
#line 1123 "lang/Grammar.acc"
 theFlavor->Weight   = (int8)(n/5); 
# line 6763 "src/yygram.cpp"
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
#line 1128 "lang/Grammar.acc"
 theRes = theEnc; 
                     theEnc->Weight = 10; 
# line 6779 "src/yygram.cpp"
      get_lexval();
      name = yylval;
#line 1130 "lang/Grammar.acc"
 theEnc->Name = name; 
# line 6784 "src/yygram.cpp"
      switch (yyselect()) {
      case 939: {
         get_lexval();
         cexpr3(&n);
#line 1131 "lang/Grammar.acc"
 theEnc->Terrain |= n; 
# line 6791 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 940: {
            encounter_entry();
            } break;
         default: goto yy87;
         }
      }
      yy87: ;
      get_lexval();
#line 1134 "lang/Grammar.acc"
 theEnc++; 
# line 6808 "src/yygram.cpp"
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
#line 1137 "lang/Grammar.acc"
 theEnc->Terrain |= n; 
# line 6826 "src/yygram.cpp"
      } break;
   case 348: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1138 "lang/Grammar.acc"
 theEnc->Depth = (int16)n; 
# line 6835 "src/yygram.cpp"
      } break;
   case 349: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1139 "lang/Grammar.acc"
 theEnc->Weight = (int16)n; 
# line 6844 "src/yygram.cpp"
      } break;
   case 350: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1140 "lang/Grammar.acc"
 theEnc->minCR = theEnc->maxCR = (int16)n; 
# line 6853 "src/yygram.cpp"
      } break;
   case 351: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
      get_lexval();
#line 1141 "lang/Grammar.acc"
 theEnc->minCR = (int16)n; theEnc->maxCR = 36; 
# line 6863 "src/yygram.cpp"
      } break;
   case 352: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      switch (yyselect()) {
      case 941: {
         get_lexval();
         } break;
      case 942: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      cexpr3(&n2);
      get_lexval();
#line 1143 "lang/Grammar.acc"
 theEnc->minCR = (int16)n; theEnc->maxCR = (int16)n2; 
# line 6882 "src/yygram.cpp"
      } break;
   case 353: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1144 "lang/Grammar.acc"
 theEnc->Align = (int16)n; 
# line 6891 "src/yygram.cpp"
      } break;
   case 354: {
      get_lexval();
      get_lexval();
#line 1145 "lang/Grammar.acc"
 currPart = 0; 
# line 6898 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 943: {
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
      case 944: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 947: {
            cexpr3(&n);
#line 1148 "lang/Grammar.acc"
 theEnc->SetFlag((uint16)n); 
# line 6924 "src/yygram.cpp"
            switch (yyselect()) {
            case 945: {
               get_lexval();
               } break;
            case 946: {
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
#line 1157 "lang/Grammar.acc"
 ASSERT(currPart < MAX_PARTS); 
# line 6968 "src/yygram.cpp"
      switch (yyselect()) {
      case 948: {
         get_lexval();
         } break;
      case 949: {
         get_lexval();
#line 1159 "lang/Grammar.acc"
 theEnc->Parts[currPart].Flags |= EP_ELSE; 
# line 6977 "src/yygram.cpp"
         } break;
      case 950: {
         get_lexval();
#line 1160 "lang/Grammar.acc"
 theEnc->Parts[currPart].Flags |= EP_OR; 
# line 6983 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
#line 1162 "lang/Grammar.acc"
 theEnc->Parts[currPart].Weight = 10; 
# line 6989 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 951: {
            cexpr4(&n);
            get_lexval();
#line 1164 "lang/Grammar.acc"
 theEnc->Parts[currPart].Chance = (uint8)n; 
# line 6997 "src/yygram.cpp"
            } break;
         case 952: {
            get_lexval();
            get_lexval();
            cexpr4(&n);
            get_lexval();
            get_lexval();
#line 1165 "lang/Grammar.acc"
 theEnc->Parts[currPart].minCR = (uint8)n; 
# line 7007 "src/yygram.cpp"
            } break;
         case 953: {
            get_lexval();
            cexpr4(&n);
            get_lexval();
#line 1166 "lang/Grammar.acc"
 theEnc->Parts[currPart].Weight = (uint8)n; 
# line 7015 "src/yygram.cpp"
            } break;
         case 954: {
            get_lexval();
            sep_expr(&hc);
            get_lexval();
#line 1167 "lang/Grammar.acc"
 theEnc->Parts[currPart].Condition = hc; 
# line 7023 "src/yygram.cpp"
            } break;
         case 957: {
            cexpr4(&n);
            switch (yyselect()) {
            case 955: {
               get_lexval();
               } break;
            case 956: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            cexpr4(&n2);
#line 1168 "lang/Grammar.acc"
 theEnc->Parts[currPart].Amt.Number = (int8)n;
                                      theEnc->Parts[currPart].Amt.Bonus  = (int8)n2;
                                      theEnc->Parts[currPart].Amt.Sides  = 0; 
# line 7041 "src/yygram.cpp"
            } break;
         case 958: {
            cexpr4(&n);
#line 1171 "lang/Grammar.acc"
 theEnc->Parts[currPart].Amt.Number = (int8)n;
                                      theEnc->Parts[currPart].Amt.Bonus  = (int8)n;
                                      theEnc->Parts[currPart].Amt.Sides  = 0; 
# line 7049 "src/yygram.cpp"
            } break;
         case 959: {
            cexpr4(&n);
            get_lexval();
#line 1174 "lang/Grammar.acc"
 theEnc->Parts[currPart].Amt.Number = (int8)n;
                                      theEnc->Parts[currPart].Amt.Bonus  = (int8)(n+50);
                                      theEnc->Parts[currPart].Amt.Sides  = 0; 
# line 7058 "src/yygram.cpp"
            } break;
         case 960: {
            dice_only(&d);
#line 1177 "lang/Grammar.acc"
 theEnc->Parts[currPart].Amt        = d; 
# line 7064 "src/yygram.cpp"
            } break;
         default: goto yy90;
         }
      }
      yy90: ;
      get_lexval();
      switch (yyselect()) {
      case 961: {
         res_ref(&rr);
#line 1183 "lang/Grammar.acc"
 theEnc->Parts[currPart].xID = rr; 
# line 7076 "src/yygram.cpp"
         } break;
      case 962: {
         cexpr3(&n);
#line 1184 "lang/Grammar.acc"
 theEnc->Parts[currPart].xID = n; 
# line 7082 "src/yygram.cpp"
         } break;
      case 963: {
         get_lexval();
         cexpr3(&n);
#line 1185 "lang/Grammar.acc"
 theEnc->Parts[currPart].xID = n;
                                      theEnc->Parts[currPart].Flags |= EP_ANYMON; 
# line 7090 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 964: {
         res_ref(&rr);
#line 1189 "lang/Grammar.acc"
 theEnc->Parts[currPart].xID2 = rr; 
# line 7099 "src/yygram.cpp"
         } break;
      case 965: {
         cexpr3(&n);
#line 1190 "lang/Grammar.acc"
 theEnc->Parts[currPart].xID2 = n; 
# line 7105 "src/yygram.cpp"
         } break;
      case 966: {
         get_lexval();
         cexpr3(&n);
#line 1191 "lang/Grammar.acc"
 theEnc->Parts[currPart].xID = n;
                                      theEnc->Parts[currPart].Flags |= EP_ANYTEMP; 
# line 7113 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 970: {
         get_lexval();
         cexpr3(&n);
#line 1195 "lang/Grammar.acc"
 theEnc->Parts[currPart].Flags |= n; 
# line 7123 "src/yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 969: {
               switch (yyselect()) {
               case 967: {
                  get_lexval();
                  } break;
               case 968: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&n);
#line 1196 "lang/Grammar.acc"
 theEnc->Parts[currPart].Flags |= n; 
# line 7139 "src/yygram.cpp"
               } break;
            default: goto yy91;
            }
         }
         yy91: ;
         get_lexval();
         } break;
      default: ;
      }
#line 1199 "lang/Grammar.acc"
 currPart++; 
# line 7151 "src/yygram.cpp"
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
#line 1203 "lang/Grammar.acc"
 theRes = theBev; 
                                           theBev->Name = name;
                                           theBev->Conditions = c; 
# line 7172 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 971: {
            behaviour_entry();
            } break;
         default: goto yy92;
         }
      }
      yy92: ;
      get_lexval();
#line 1206 "lang/Grammar.acc"
 theBev++; 
# line 7186 "src/yygram.cpp"
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
      case 972: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 1210 "lang/Grammar.acc"
 theBev->spID = rr; 
# line 7209 "src/yygram.cpp"
      } break;
   case 363: {
      get_lexval();
      switch (yyselect()) {
      case 973: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 976: {
            cexpr3(&n);
            switch (yyselect()) {
            case 974: {
               get_lexval();
               } break;
            case 975: {
               get_lexval();
               } break;
            default: ;
            }
#line 1211 "lang/Grammar.acc"
 theBev->SetFlag((uint16)n); 
# line 7234 "src/yygram.cpp"
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
#line 1218 "lang/Grammar.acc"
 theRes = theText;
                         theText->Name = name;
                         cText = 0; 
# line 7270 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 977: {
            get_lexval();
            text = yylval;
#line 1221 "lang/Grammar.acc"
 TempList[cText++] = text; 
# line 7278 "src/yygram.cpp"
            } break;
         case 978: {
            res_ref(&rr);
#line 1222 "lang/Grammar.acc"
 TempList[cText++] = rr; 
# line 7284 "src/yygram.cpp"
            } break;
         case 979: {
            get_lexval();
#line 1223 "lang/Grammar.acc"
 TempList[cText++] = TC_LPAREN; 
# line 7290 "src/yygram.cpp"
            } break;
         case 980: {
            get_lexval();
#line 1224 "lang/Grammar.acc"
 TempList[cText++] = TC_RPAREN; 
# line 7296 "src/yygram.cpp"
            } break;
         case 981: {
            get_lexval();
#line 1225 "lang/Grammar.acc"
 TempList[cText++] = TC_CHOICE; 
# line 7302 "src/yygram.cpp"
            } break;
         case 982: {
            get_lexval();
#line 1226 "lang/Grammar.acc"
 TempList[cText++] = TC_TERM; 
# line 7308 "src/yygram.cpp"
            } break;
         case 983: {
            get_lexval();
            sep_expr(&hc);
            get_lexval();
            get_lexval();
#line 1228 "lang/Grammar.acc"
 TempList[cText++] = TC_CASE;
                      TempList[cText++] = hc; 
# line 7318 "src/yygram.cpp"
            } break;
         case 984: {
            get_lexval();
            sep_stat(&hc);
            get_lexval();
#line 1231 "lang/Grammar.acc"
 TempList[cText++] = TC_ACTION;
                      TempList[cText++] = hc; 
# line 7327 "src/yygram.cpp"
            } break;
         default: goto yy94;
         }
      }
      yy94: ;
      get_lexval();
#line 1233 "lang/Grammar.acc"
 TempList[cText] = 0;
             theText->AddList(TEXT_LIST,TempList);
             theText++; 
# line 7338 "src/yygram.cpp"
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

int type_name (YYSTYPE *ty)
{
   switch(yyselect()) {
   case 369: {
      get_lexval();
#line 1241 "lang/Grammar.acc"
 *ty = DT_VOID; 
# line 7361 "src/yygram.cpp"
      } break;
   case 370: {
      get_lexval();
#line 1242 "lang/Grammar.acc"
 *ty = DT_BOOL; 
# line 7367 "src/yygram.cpp"
      } break;
   case 371: {
      get_lexval();
#line 1243 "lang/Grammar.acc"
 *ty = DT_UINT8; 
# line 7373 "src/yygram.cpp"
      } break;
   case 372: {
      get_lexval();
#line 1244 "lang/Grammar.acc"
 *ty = DT_UINT16; 
# line 7379 "src/yygram.cpp"
      } break;
   case 373: {
      get_lexval();
#line 1245 "lang/Grammar.acc"
 *ty = DT_INT8; 
# line 7385 "src/yygram.cpp"
      } break;
   case 374: {
      get_lexval();
#line 1246 "lang/Grammar.acc"
 *ty = DT_INT16; 
# line 7391 "src/yygram.cpp"
      } break;
   case 375: {
      get_lexval();
#line 1247 "lang/Grammar.acc"
 *ty = DT_INT32; 
# line 7397 "src/yygram.cpp"
      } break;
   case 376: {
      get_lexval();
#line 1248 "lang/Grammar.acc"
 *ty = DT_HOBJ; 
# line 7403 "src/yygram.cpp"
      } break;
   case 377: {
      get_lexval();
#line 1249 "lang/Grammar.acc"
 *ty = DT_HTEXT; 
# line 7409 "src/yygram.cpp"
      } break;
   case 378: {
      get_lexval();
#line 1250 "lang/Grammar.acc"
 *ty = DT_RID; 
# line 7415 "src/yygram.cpp"
      } break;
   case 379: {
      get_lexval();
#line 1251 "lang/Grammar.acc"
 *ty = DT_STRING; 
# line 7421 "src/yygram.cpp"
      } break;
   case 380: {
      get_lexval();
#line 1252 "lang/Grammar.acc"
 *ty = DT_RECT; 
# line 7427 "src/yygram.cpp"
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
   case 381: {
#line 1256 "lang/Grammar.acc"
 BSysFunc *b; 
# line 7448 "src/yygram.cpp"
      get_lexval();
      type_name(&ty);
      cexpr3(&ot);
      get_lexval();
      get_lexval();
      fn = yylval;
      get_lexval();
#line 1258 "lang/Grammar.acc"
 if (is_res(ot)) {
        if (theSymTab.GetBinding(fn,0,0,RES_FUNC))
          if (GETBIND_RESFUNC(fn)->RType == ot)
            yyerror(Format("Resource member function '%s' declared for same resource type twice!",theSymTab[fn]));
        b = new BResFunc((int16)MemFuncID++,(int8)ty,(int8)ot);
        theSymTab.Bind(fn,b);
        }
      else {
        if (theSymTab.GetBinding(fn,0,0,MEM_FUNC))
          yyerror(Format("System member function '%s' declared twice!",theSymTab[fn]));
        /*else if (theSymTab.GetBinding(fn,1,0,ANY))

          yyerror(Format("Identifier conflict: '%s'.", theSymTab[fn]));*/
        b = new BMemFunc((int16)MemFuncID++,(int8)ty,(int8)ot);
        theSymTab.Bind(fn,b); 
      }
    
# line 7474 "src/yygram.cpp"
      switch (yyselect()) {
      case 992: {
         type_name(&ty_1);
         switch (yyselect()) {
         case 985: {
            get_lexval();
            cexpr3(&ot_1);
#line 1274 "lang/Grammar.acc"
 b->ParamOType[0] = (int8)ot_1; 
# line 7484 "src/yygram.cpp"
            } break;
         default: ;
         }
#line 1275 "lang/Grammar.acc"
 b->ParamType[0] = (int8)ty_1; b->ParamCount++; 
# line 7490 "src/yygram.cpp"
         switch (yyselect()) {
         case 986: {
            get_lexval();
            paramname = yylval;
            } break;
         default: ;
         }
         switch (yyselect()) {
         case 987: {
            get_lexval();
            cexpr(&def);
#line 1276 "lang/Grammar.acc"
 b->Default[0] = (int8)def; b->HasDefault |= BIT(2); 
# line 7504 "src/yygram.cpp"
            } break;
         default: ;
         }
         while (1) {
            switch (yyselect()) {
            case 991: {
               get_lexval();
               type_name(&ty_n);
               switch (yyselect()) {
               case 988: {
                  get_lexval();
                  cexpr3(&ot_n);
#line 1278 "lang/Grammar.acc"
 b->ParamOType[b->ParamCount] = (int8)ot_n; 
# line 7519 "src/yygram.cpp"
                  } break;
               default: ;
               }
#line 1279 "lang/Grammar.acc"
 b->ParamType[b->ParamCount] = (int8)ty_n; b->ParamCount++; 
# line 7525 "src/yygram.cpp"
               switch (yyselect()) {
               case 989: {
                  get_lexval();
                  paramname = yylval;
                  } break;
               default: ;
               }
               switch (yyselect()) {
               case 990: {
                  get_lexval();
                  cexpr(&def);
#line 1280 "lang/Grammar.acc"
 b->Default[b->ParamCount] = (int8)def; b->HasDefault |= BIT(1+b->ParamCount); 
# line 7539 "src/yygram.cpp"
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
      case 993: {
         get_lexval();
         get_lexval();
#line 1282 "lang/Grammar.acc"
 b->isVarParam = true; 
# line 7557 "src/yygram.cpp"
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
   case 382: {
#line 1285 "lang/Grammar.acc"
 BSysFunc *b; 
# line 7582 "src/yygram.cpp"
      get_lexval();
      type_name(&ty);
      get_lexval();
      fn = yylval;
      get_lexval();
#line 1287 "lang/Grammar.acc"
 if (theSymTab.GetBinding(fn,0,0,SYS_FUNC))
        yyerror(Format("System global function '%s' declared twice!",theSymTab[fn]));
      else if (theSymTab.GetBinding(fn,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[fn]));
      b = new BSysFunc((int16)MemFuncID++,(int8)ty);
      theSymTab.Bind(fn,b); 
# line 7595 "src/yygram.cpp"
      switch (yyselect()) {
      case 1001: {
         type_name(&ty_1);
         switch (yyselect()) {
         case 994: {
            get_lexval();
            cexpr3(&ot_1);
#line 1293 "lang/Grammar.acc"
 b->ParamOType[0] = (int8)ot_1; 
# line 7605 "src/yygram.cpp"
            } break;
         default: ;
         }
#line 1294 "lang/Grammar.acc"
 b->ParamType[0] = (int8)ty_1; b->ParamCount++; 
# line 7611 "src/yygram.cpp"
         switch (yyselect()) {
         case 995: {
            get_lexval();
            paramname = yylval;
            } break;
         default: ;
         }
         switch (yyselect()) {
         case 996: {
            get_lexval();
            cexpr(&def);
#line 1295 "lang/Grammar.acc"
 b->Default[0] = (int8)def; b->HasDefault |= BIT(2); 
# line 7625 "src/yygram.cpp"
            } break;
         default: ;
         }
         while (1) {
            switch (yyselect()) {
            case 1000: {
               get_lexval();
               type_name(&ty_n);
               switch (yyselect()) {
               case 997: {
                  get_lexval();
                  cexpr3(&ot_n);
#line 1297 "lang/Grammar.acc"
 b->ParamOType[b->ParamCount] = (int8)ot_n; 
# line 7640 "src/yygram.cpp"
                  } break;
               default: ;
               }
#line 1298 "lang/Grammar.acc"
 b->ParamType[b->ParamCount] = (int8)ty_n; b->ParamCount++; 
# line 7646 "src/yygram.cpp"
               switch (yyselect()) {
               case 998: {
                  get_lexval();
                  paramname = yylval;
                  } break;
               default: ;
               }
               switch (yyselect()) {
               case 999: {
                  get_lexval();
                  cexpr(&def);
#line 1299 "lang/Grammar.acc"
 b->Default[b->ParamCount] = (int8)def; b->HasDefault |= BIT(1+b->ParamCount); 
# line 7660 "src/yygram.cpp"
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
      case 1002: {
         get_lexval();
         get_lexval();
#line 1301 "lang/Grammar.acc"
 b->isVarParam = true; 
# line 7678 "src/yygram.cpp"
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
   case 383: {
#line 1304 "lang/Grammar.acc"
 BMemVar *b; BResMem *b2; 
# line 7698 "src/yygram.cpp"
      get_lexval();
      type_name(&ty);
      cexpr3(&ot);
      get_lexval();
      get_lexval();
      vn = yylval;
      get_lexval();
#line 1306 "lang/Grammar.acc"
 
      if (is_res(ot)) {
        if (theSymTab.GetBinding(vn,0,RES_MEM))
          if (((BResMem*)theSymTab.GetBinding(vn,0,0,RES_MEM))->RType == ot)
            yyerror(Format("System resource member '%s' declared twice!",theSymTab[vn]));
        b2 = new BResMem();
        b2->varid = (int16)MemVarID++;
        b2->VarType = (int8)ty;
        b2->RType = (int8)ot;
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
        b->varid = (int16)MemVarID++; 
        b->VarType = (int8)ty; 
        b->OType = (int8)ot;
        b->xID   = 0;
        b->Event = 1;
        b->type  = MEM_VAR;
        theSymTab.Bind(vn,b); 
        }
    
# line 7736 "src/yygram.cpp"
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
   case 384: {
#line 1337 "lang/Grammar.acc"
 BSysObj *b; 
# line 7751 "src/yygram.cpp"
      get_lexval();
      cexpr3(&ot);
      get_lexval();
      oname = yylval;
      get_lexval();
      cexpr3(&oid);
      get_lexval();
#line 1339 "lang/Grammar.acc"
 if (theSymTab.GetBinding(oname,0,0,SYS_OBJ))
        yyerror(Format("System object '%s' declared twice!",theSymTab[oname]));
      else if (theSymTab.GetBinding(oname,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[oname]));
      b = new BSysObj();
      b->ObjNum = (int16)oid;
      b->OType  = (int8)ot;
      b->xID   = 0;
      b->Event = 0;
      b->type  = SYS_OBJ;
      theSymTab.Bind(oname,b); 
# line 7771 "src/yygram.cpp"
      } break;
   }
return 0;
}

int g_declaration ()
{
   YYSTYPE ty;
   YYSTYPE vn;
   switch(yyselect()) {
   case 385: {
#line 1353 "lang/Grammar.acc"
 BGlobalVar *b; 
# line 7785 "src/yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1355 "lang/Grammar.acc"
 if (ty == DT_STRING)
        Error("Global variables of type String cannot be created!");
      if (theSymTab.GetBinding(vn,0,0,GLOB_VAR))
        Error("Global variable '%s' declared twice!",theSymTab[vn]);
      else if (theSymTab.GetBinding(vn,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[vn]));
      b = new BGlobalVar();
      b->VarType = (int8)ty; 
      b->Address = HeapHead++;
      b->xID     = 0;
      b->Event   = 0;
      
      b->type    = GLOB_VAR;
      theSymTab.Bind(vn,b); 
# line 7804 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1003: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1370 "lang/Grammar.acc"
 if (theSymTab.GetBinding(vn,0,0,GLOB_VAR))
        Error("Global variable '%s' declared twice!",theSymTab[vn]);
      else if (theSymTab.GetBinding(vn,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[vn]));
      b = new BGlobalVar();
      b->VarType = (int8)ty; 
      b->Address = HeapHead++;
      b->xID     = 0;
      b->Event   = 0;
      b->type    = GLOB_VAR;
      theSymTab.Bind(vn,b); 
# line 7823 "src/yygram.cpp"
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

int l_declaration (VBlock *code)
{
   YYSTYPE ty;
   YYSTYPE vn;
   switch(yyselect()) {
   case 386: {
#line 1383 "lang/Grammar.acc"
 BLocalVar *b; int16 i; String scope_id; 
# line 7843 "src/yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1385 "lang/Grammar.acc"
 b = new BLocalVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      ASSERT(theEvent);
      b->Event = (int16)theEvent;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,LOC_VAR))
        Error("Local variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = (int8)ty; 
      b->StackOffset = (int16)++StackHead;
      b->type    = LOC_VAR;
      if (ty == DT_STRING) {
        i = AllocString();
        LockString(i);
        code->Generate(MOV,RT_MEMORY,-(b->StackOffset),RT_CONSTANT,-i);
        code->Generate(ESTR,RT_CONSTANT,-i);
        }
      theSymTab.Bind(vn,b); 
# line 7864 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1004: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1402 "lang/Grammar.acc"
 b = new BLocalVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      ASSERT(theEvent);
      b->Event = (int16)theEvent;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,LOC_VAR))
        Error("Local variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = (int8)ty; 
      b->StackOffset = (int16)++StackHead;
      b->type    = LOC_VAR;
      if (ty == DT_STRING) {
        i = AllocString();
        LockString(i);
        code->Generate(MOV,RT_MEMORY,-(b->StackOffset),RT_CONSTANT,-i);
        code->Generate(ESTR,RT_CONSTANT,-i);
        }
      theSymTab.Bind(vn,b); 
# line 7888 "src/yygram.cpp"
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
   case 387: {
#line 1420 "lang/Grammar.acc"
 BResVar *b; String scope_id; 
# line 7908 "src/yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1422 "lang/Grammar.acc"
 if (ty == DT_STRING)
        yyerror("Resource variables of type String cannot be allocated.");
      b = new BResVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      b->Event = 0;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,RES_VAR))
        Error("Resource variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = (int8)ty; 
      b->Address = HeapHead++;
      b->type    = RES_VAR;
      theSymTab.Bind(vn,b); 
# line 7924 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1005: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1434 "lang/Grammar.acc"
 b = new BResVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      b->Event = 0;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,RES_VAR))
        Error("Resource variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = (int8)ty; 
      b->Address = HeapHead++;
      b->type    = RES_VAR;
      theSymTab.Bind(vn,b); 
# line 7941 "src/yygram.cpp"
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

int statement (VBlock *st)
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
   case 388: {
      expr(&ex);
      get_lexval();
#line 1448 "lang/Grammar.acc"

      st->Add(ex.Code);
      if (ex.Storage == RT_REGISTER) {
        st->TrimGratuitousMOV();
        FreeRegister((int16)ex.Value);
        }
      if (ex.Type == DT_STRING && ex.Storage == RT_CONSTANT)
        if (ex.Value < -1)
          FreeString((int16)-ex.Value);
    
# line 7980 "src/yygram.cpp"
      } break;
   case 389: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
      get_lexval();
      statement(&st2);
#line 1459 "lang/Grammar.acc"

      st->Add(ex.Code);
      /* Here, we compute the address to include the size

         of the JUMP opcode below, which may be one or two

         words, depending upon if it needs data space. */
      st->Generate(JFAL, ex.Storage, ex.Value, 
        RT_CONSTANT, st1.GetSize()+ (st2.GetSize()+1> 511?
          3 : 2));
      if (ex.Storage == RT_REGISTER)
        FreeRegister((int16)ex.Value);                  
      st->Add(&st1);                            
      st->Generate(JUMP, RT_CONSTANT, st2.GetSize()+1);
      st->Add(&st2);
    
# line 8007 "src/yygram.cpp"
      } break;
   case 390: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
#line 1474 "lang/Grammar.acc"

      st->Add(ex.Code);
      st->Generate(JFAL, ex.Storage, ex.Value, 
        RT_CONSTANT, st1.GetSize()+1);
      if (ex.Storage == RT_REGISTER)
        FreeRegister((int16)ex.Value);
      st->Add(&st1);
    
# line 8024 "src/yygram.cpp"
      } break;
   case 391: {
      get_lexval();
      get_lexval();
      expr(&ex1);
      get_lexval();
      expr(&ex2);
      get_lexval();
      expr(&ex3);
      get_lexval();
      statement(&st1);
#line 1484 "lang/Grammar.acc"

      int16 jumpback;
      ex1.Code->TrimGratuitousMOV();
      ex3.Code->TrimGratuitousMOV();
      st->Add(ex1.Code);
      if (ex1.Storage == RT_REGISTER)
        FreeRegister((int16)ex1.Value);
      jumpback = (int16)st->GetSize();
      st->Add(ex2.Code);
      /* Here again, we compensate for the fact that the lower JUMP

         opcode may be either one or two words; there has to be a

         less kludgy way to do this... */
      st->Generate(JFAL, ex2.Storage,ex2.Value,
        RT_CONSTANT,st1.GetSize() + (ex3.Code ? ex3.Code->GetSize() : 0) + 
        (st1.GetSize() + (ex3.Code ? ex3.Code->GetSize() : 0) + 
                         (ex2.Code ? ex2.Code->GetSize() : 0) > 511 ? 3 : 2));
      if (ex2.Storage == RT_REGISTER)
        FreeRegister((int16)ex2.Value);
      st1.ProcessContinues();
      st->Add(&st1);
      st->Add(ex3.Code);
      if (ex3.Storage == RT_REGISTER)
        FreeRegister((int16)ex3.Value);
      st->Generate(JUMP,RT_CONSTANT,jumpback - st->GetSize());
    
# line 8064 "src/yygram.cpp"
      } break;
   case 392: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      get_lexval();
#line 1510 "lang/Grammar.acc"

        int16 CaseAddr[256], DefaultAddr;
        int32 CaseVal[256];
        VBlock *CaseCode[256], *DefaultCode;
        int8 CaseCount, i;
        CaseCount = 0;
        DefaultAddr = 0;
        CaseAddr[0] = 0; 
      
# line 8082 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1009: {
            get_lexval();
            switch (yyselect()) {
            case 1006: {
               cexpr(&val);
               } break;
            case 1007: {
               res_ref(&rr);
#line 1519 "lang/Grammar.acc"
 val = rr; 
# line 8095 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            get_lexval();
#line 1520 "lang/Grammar.acc"
 CaseCode[CaseCount] = new VBlock; 
# line 8102 "src/yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 1008: {
#line 1521 "lang/Grammar.acc"
 st_n.Clear(); 
# line 8108 "src/yygram.cpp"
                  statement(&st_n);
#line 1521 "lang/Grammar.acc"
 CaseCode[CaseCount]->Add(&st_n);  
# line 8112 "src/yygram.cpp"
                  } break;
               default: goto yy101;
               }
            }
            yy101: ;
#line 1522 "lang/Grammar.acc"

        /* We know where *this* case ends, which is synonimous with where

           the *next* case starts... */
        CaseAddr[CaseCount + 1] = CaseAddr[CaseCount] + (int16)CaseCode[CaseCount]->GetSize();
        CaseVal[CaseCount] = val;
        CaseCount++;  
      
# line 8127 "src/yygram.cpp"
            } break;
         default: goto yy100;
         }
      }
      yy100: ;
      switch (yyselect()) {
      case 1011: {
         get_lexval();
         get_lexval();
#line 1530 "lang/Grammar.acc"
 DefaultCode = new VBlock; 
# line 8139 "src/yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 1010: {
#line 1531 "lang/Grammar.acc"
 st_n.Clear(); 
# line 8145 "src/yygram.cpp"
               statement(&st_n);
#line 1531 "lang/Grammar.acc"
 DefaultCode->Add(&st_n);  
# line 8149 "src/yygram.cpp"
               } break;
            default: goto yy102;
            }
         }
         yy102: ;
#line 1532 "lang/Grammar.acc"
 
        DefaultAddr = CaseAddr[CaseCount]; 
        CaseAddr[CaseCount] = DefaultAddr + (int16)DefaultCode->GetSize(); 
      
# line 8160 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
#line 1539 "lang/Grammar.acc"

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
          FreeRegister((int16)ex.Value);

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
        
      
# line 8206 "src/yygram.cpp"
      } break;
   case 393: {
      get_lexval();
      get_lexval();
#line 1580 "lang/Grammar.acc"

        st->Generate(JBRK);
      
# line 8215 "src/yygram.cpp"
      } break;
   case 394: {
      get_lexval();
      get_lexval();
#line 1584 "lang/Grammar.acc"

        st->Generate(CONT);
      
# line 8224 "src/yygram.cpp"
      } break;
   case 395: {
      get_lexval();
      statement(&st1);
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      get_lexval();
#line 1588 "lang/Grammar.acc"

        st->Add(&st1);
        st->Add(ex.Code);
        st->Generate(JTRU,ex.Storage,ex.Value,
          RT_CONSTANT, -st->GetSize());
        if (ex.Storage == RT_REGISTER)
          FreeRegister((int16)ex.Value);
      
# line 8243 "src/yygram.cpp"
      } break;
   case 396: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
#line 1597 "lang/Grammar.acc"

        st->Add(ex.Code);
        st->Generate(JFAL,ex.Storage,ex.Value,
          RT_CONSTANT, st1.GetSize()+
            (st->GetSize() > 511 ? 3 : 2));
        st->Add(&st1);
        st->Generate(JUMP,RT_CONSTANT, - st->GetSize());
        if (ex.Storage == RT_REGISTER)
          FreeRegister((int16)ex.Value);
      
# line 8262 "src/yygram.cpp"
      } break;
   case 397: {
      block(&bl);
#line 1608 "lang/Grammar.acc"
 
        st->Add(&bl); 
      
# line 8270 "src/yygram.cpp"
      } break;
   case 398: {
      get_lexval();
      expr(&ex);
      get_lexval();
#line 1612 "lang/Grammar.acc"

        st->Add(ex.Code);
        if (StackHead) {
          st->Generate(MOV,RT_REGISTER,0,ex.Storage,ex.Value);
          st->Generate(DEC,RT_REGISTER,63,RT_CONSTANT,StackHead);
          st->Generate(RET,RT_REGISTER,0);
          }
        else
          st->Generate(RET,ex.Storage,ex.Value);
        if (ex.Storage == RT_REGISTER)
          FreeRegister((int16)ex.Value);
      
# line 8289 "src/yygram.cpp"
      } break;
   case 399: {
      get_lexval();
      get_lexval();
#line 1625 "lang/Grammar.acc"

        if (StackHead)
          st->Generate(DEC,RT_REGISTER,63,RT_CONSTANT,StackHead);
        st->Generate(RET);
      
# line 8300 "src/yygram.cpp"
      } break;
   }
return 0;
}

int block (VBlock *bl)
{
   VBlock st;
   switch(yyselect()) {
   case 400: {
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 1012: {
#line 1633 "lang/Grammar.acc"
 st.Clear(); 
# line 8317 "src/yygram.cpp"
            statement(&st);
#line 1633 "lang/Grammar.acc"
 bl->Add(&st); 
# line 8321 "src/yygram.cpp"
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

int routine (VBlock *bl)
{
   VBlock code;
   VBlock st;
   switch(yyselect()) {
   case 401: {
#line 1637 "lang/Grammar.acc"
 VBlock decl_code; StackHead = 0; ClearStrings(); 
# line 8341 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 1013: {
#line 1638 "lang/Grammar.acc"
 code.Clear(); 
# line 8348 "src/yygram.cpp"
            l_declaration(&code);
#line 1638 "lang/Grammar.acc"
 decl_code.Add(&code); 
# line 8352 "src/yygram.cpp"
            } break;
         case 1014: {
            get_lexval();
            r_declaration();
            } break;
         default: goto yy104;
         }
      }
      yy104: ;
#line 1640 "lang/Grammar.acc"
 if (StackHead)
          bl->Generate(INC,RT_REGISTER,63,RT_CONSTANT,StackHead);
        bl->Add(&decl_code); 
# line 8366 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1015: {
#line 1643 "lang/Grammar.acc"
 st.Clear(); 
# line 8372 "src/yygram.cpp"
            statement(&st);
#line 1643 "lang/Grammar.acc"
 bl->Add(&st); 
# line 8376 "src/yygram.cpp"
            } break;
         default: goto yy105;
         }
      }
      yy105: ;
      get_lexval();
#line 1644 "lang/Grammar.acc"
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
      
# line 8400 "src/yygram.cpp"
      } break;
   }
return 0;
}

int fund_expr (PExp *ex)
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
   case 402: {
      get_lexval();
      n = yylval;
#line 1680 "lang/Grammar.acc"
 ex->Type = DT_INT32; ex->Storage = RT_CONSTANT; ex->Value = n; ex->Code = NULL; 
# line 8425 "src/yygram.cpp"
      } break;
   case 403: {
      get_lexval();
      n = yylval;
#line 1682 "lang/Grammar.acc"
 ex->Type = DT_INT32; ex->Storage = RT_CONSTANT; ex->Value = n; ex->Code = NULL; 
# line 8432 "src/yygram.cpp"
      } break;
   case 404: {
      get_lexval();
      lv = yylval;
#line 1684 "lang/Grammar.acc"
 ex->Type = DT_HTEXT; ex->Storage = RT_CONSTANT; ex->Value = lv; ex->Code = NULL; 
# line 8439 "src/yygram.cpp"
      } break;
   case 405: {
      get_lexval();
#line 1686 "lang/Grammar.acc"
 ex->Type = DT_BOOL; ex->Storage = RT_CONSTANT; ex->Value = 1; ex->Code = NULL; 
# line 8445 "src/yygram.cpp"
      } break;
   case 406: {
      get_lexval();
#line 1688 "lang/Grammar.acc"
 ex->Type = DT_BOOL; ex->Storage = RT_CONSTANT; ex->Value = 0; ex->Code = NULL; 
# line 8451 "src/yygram.cpp"
      } break;
   case 407: {
      get_lexval();
#line 1690 "lang/Grammar.acc"
 ex->Type = DT_UNKNOWN; ex->Storage = RT_CONSTANT; ex->Value = 0; ex->Code = NULL; 
# line 8457 "src/yygram.cpp"
      } break;
   case 408: {
      res_ref(&rr);
#line 1692 "lang/Grammar.acc"
 ex->Type = DT_RID;   ex->Storage = RT_CONSTANT; ex->Value = rr; ex->Code = NULL; 
# line 8463 "src/yygram.cpp"
      } break;
   case 409: {
      get_lexval();
      expr(&ex2);
      get_lexval();
#line 1694 "lang/Grammar.acc"
 ex->Code = ex2.Code; 
      ex->Type = ex2.Type; ex->Storage = ex2.Storage; ex->Value = ex2.Value; 
# line 8472 "src/yygram.cpp"
      } break;
   case 410: {
      get_lexval();
      id = yylval;
#line 1697 "lang/Grammar.acc"
 BGlobalVar *b; BLocalVar *b2; BSysObj *b3; BResVar *b4; rID resID;
      resID = FIND(theModule->GetText(theRes->Name));
      ASSERT(resID);
      ASSERT(theEvent);
      if (b2 = GETBIND_LOCALVAR(id,resID,(int16)theEvent))
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
    
# line 8518 "src/yygram.cpp"
      } break;
   case 411: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 1016: {
         get_lexval();
         } break;
      case 1017: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      id = yylval;
#line 1738 "lang/Grammar.acc"

      BMemVar *b;
	  BResMem *b2;
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
            FreeRegister((int16)obj.Value);
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
            FreeRegister((int16)obj.Value);

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
    
# line 8590 "src/yygram.cpp"
      } break;
   case 412: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 1018: {
         get_lexval();
         } break;
      case 1019: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      fn = yylval;
#line 1795 "lang/Grammar.acc"
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
      
# line 8632 "src/yygram.cpp"
      get_lexval();
      param_list(b, fn, &bl, &narg);
      get_lexval();
#line 1822 "lang/Grammar.acc"

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
      
# line 8665 "src/yygram.cpp"
      } break;
   case 413: {
      get_lexval();
      fn = yylval;
#line 1850 "lang/Grammar.acc"
 BSysFunc *b;
        if (!(b = GETBIND_SYSFUNC(fn)))
          yyerror(Format("Expected: global function name before '(' (%s).",
            theSymTab[fn]));
        ex->Code = new VBlock;
      
# line 8677 "src/yygram.cpp"
      get_lexval();
      param_list(b, fn, &bl, &narg);
      get_lexval();
#line 1857 "lang/Grammar.acc"

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
      
# line 8700 "src/yygram.cpp"
      } break;
   case 414: {
      fund_expr(&rect);
      get_lexval();
      get_lexval();
      id = yylval;
#line 1876 "lang/Grammar.acc"

      if (rect.Type != DT_RECT)
        yyerror("Type mismatch: '@' operator used on non-Rect target.");
      *ex = CodeRectMember(&rect,(int16)id);
      
    
# line 8714 "src/yygram.cpp"
      } break;
   }
return 0;
}

int lvalue (LExp *lv)
{
   YYSTYPE id;
   LExp rect;
   PExp obj;
   switch(yyselect()) {
   case 415: {
      get_lexval();
      id = yylval;
#line 1886 "lang/Grammar.acc"
 BGlobalVar *b; BLocalVar *b2; BResVar *b3; rID resID;
      resID = FIND(theModule->GetText(theRes->Name));
      ASSERT(resID);
      ASSERT(theEvent);
      
      if (b2 = GETBIND_LOCALVAR(id,resID,(int16)theEvent))
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
    
# line 8774 "src/yygram.cpp"
      } break;
   case 416: {
      lvalue(&rect);
      get_lexval();
      get_lexval();
      id = yylval;
#line 1931 "lang/Grammar.acc"

      if (rect.Type != DT_RECT)
        yyerror("Type mismatch: '@' operator used on non-Rect target.");
      *lv = CodeRectLVal(&rect,(int16)id);
    
# line 8787 "src/yygram.cpp"
      } break;
   case 417: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 1020: {
         get_lexval();
         } break;
      case 1021: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      id = yylval;
#line 1937 "lang/Grammar.acc"

      int8 i;
	  BMemVar *b;
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
          FreeRegister((int16)obj.Value);
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
    
# line 8841 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr10 (PExp *ex)
{
   LExp lv;
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 418: {
      lvalue(&lv);
      get_lexval();
#line 1980 "lang/Grammar.acc"
 *ex = CodeAssignment('i',lv,NULL); 
# line 8858 "src/yygram.cpp"
      } break;
   case 419: {
      lvalue(&lv);
      get_lexval();
#line 1982 "lang/Grammar.acc"
 *ex = CodeAssignment('d',lv,NULL); 
# line 8865 "src/yygram.cpp"
      } break;
   case 420: {
      get_lexval();
      get_lexval();
      expr150(&ex1);
      get_lexval();
      expr150(&ex2);
      get_lexval();
#line 1984 "lang/Grammar.acc"
 *ex = CodeOperator('m',ex1,ex2); 
# line 8876 "src/yygram.cpp"
      } break;
   case 421: {
      get_lexval();
      get_lexval();
      expr150(&ex1);
      get_lexval();
      expr150(&ex2);
      get_lexval();
#line 1986 "lang/Grammar.acc"
 *ex = CodeOperator('M',ex1,ex2); 
# line 8887 "src/yygram.cpp"
      } break;
   case 422: {
      fund_expr(&ex1);
#line 1988 "lang/Grammar.acc"
 *ex = ex1; 
# line 8893 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr20 (PExp *ex)
{
   LExp lv;
   PExp ex1;
   switch(yyselect()) {
   case 423: {
      get_lexval();
      lvalue(&lv);
#line 1993 "lang/Grammar.acc"
 *ex = CodeAssignment('I',lv,NULL); 
# line 8909 "src/yygram.cpp"
      } break;
   case 424: {
      get_lexval();
      lvalue(&lv);
#line 1995 "lang/Grammar.acc"
 *ex = CodeAssignment('D',lv,NULL); 
# line 8916 "src/yygram.cpp"
      } break;
   case 425: {
      get_lexval();
      expr20(&ex1);
#line 1997 "lang/Grammar.acc"
 *ex = ex1; 
# line 8923 "src/yygram.cpp"
      } break;
   case 426: {
      get_lexval();
      expr20(&ex1);
#line 1999 "lang/Grammar.acc"
 *ex = CodeOperator('-',ex1,ex1); 
# line 8930 "src/yygram.cpp"
      } break;
   case 427: {
      get_lexval();
      expr20(&ex1);
#line 2001 "lang/Grammar.acc"
 *ex = CodeOperator('!',ex1,ex1); 
# line 8937 "src/yygram.cpp"
      } break;
   case 428: {
      get_lexval();
      expr20(&ex1);
#line 2003 "lang/Grammar.acc"
 *ex = CodeOperator('~',ex1,ex1); 
# line 8944 "src/yygram.cpp"
      } break;
   case 429: {
      get_lexval();
      expr20(&ex1);
      get_lexval();
#line 2005 "lang/Grammar.acc"
 *ex = CodeOperator(':',ex1,ex1); 
# line 8952 "src/yygram.cpp"
      } break;
   case 430: {
      expr10(&ex1);
#line 2007 "lang/Grammar.acc"
 *ex = ex1; 
# line 8958 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr30 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 431: {
      expr20(&ex1);
      get_lexval();
      expr20(&ex2);
#line 2012 "lang/Grammar.acc"
 *ex = CodeOperator('d',ex1,ex2); 
# line 8975 "src/yygram.cpp"
      } break;
   case 432: {
      expr20(&ex1);
#line 2014 "lang/Grammar.acc"
 *ex = ex1; 
# line 8981 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr40 (PExp *ex)
{
   YYSTYPE tn;
   PExp ex1;
   switch(yyselect()) {
   case 433: {
      get_lexval();
      type_name(&tn);
      get_lexval();
      expr40(&ex1);
#line 2019 "lang/Grammar.acc"
 *ex = ex1; ex->Type = (int8)tn; 
# line 8999 "src/yygram.cpp"
      } break;
   case 434: {
      expr30(&ex1);
#line 2021 "lang/Grammar.acc"
 *ex = ex1; 
# line 9005 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr50 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 435: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 2027 "lang/Grammar.acc"
 *ex = CodeOperator('*',ex1,ex2); 
# line 9022 "src/yygram.cpp"
      } break;
   case 436: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 2029 "lang/Grammar.acc"
 *ex = CodeOperator('/',ex1,ex2); 
# line 9030 "src/yygram.cpp"
      } break;
   case 437: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 2031 "lang/Grammar.acc"
 *ex = CodeOperator('%',ex1,ex2); 
# line 9038 "src/yygram.cpp"
      } break;
   case 438: {
      expr40(&ex1);
#line 2033 "lang/Grammar.acc"
 *ex = ex1; 
# line 9044 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr60 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 439: {
      expr60(&ex1);
      get_lexval();
      expr50(&ex2);
#line 2038 "lang/Grammar.acc"
 *ex = CodeOperator('+',ex1,ex2); 
# line 9061 "src/yygram.cpp"
      } break;
   case 440: {
      expr60(&ex1);
      get_lexval();
      expr50(&ex2);
#line 2040 "lang/Grammar.acc"
 *ex = CodeOperator('-',ex1,ex2); 
# line 9069 "src/yygram.cpp"
      } break;
   case 441: {
      expr50(&ex1);
#line 2042 "lang/Grammar.acc"
 *ex = ex1; 
# line 9075 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr70 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 442: {
      expr70(&ex1);
      get_lexval();
      expr60(&ex2);
#line 2047 "lang/Grammar.acc"
 *ex = CodeOperator(LSHIFT_OP,ex1,ex2); 
# line 9092 "src/yygram.cpp"
      } break;
   case 443: {
      expr70(&ex1);
      get_lexval();
      expr60(&ex2);
#line 2049 "lang/Grammar.acc"
 *ex = CodeOperator(RSHIFT_OP,ex1,ex2); 
# line 9100 "src/yygram.cpp"
      } break;
   case 444: {
      expr60(&ex1);
#line 2051 "lang/Grammar.acc"
 *ex = ex1; 
# line 9106 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr80 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 445: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2060 "lang/Grammar.acc"
 *ex = CodeOperator('<',ex1,ex2); 
# line 9123 "src/yygram.cpp"
      } break;
   case 446: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2062 "lang/Grammar.acc"
 *ex = CodeOperator('>',ex1,ex2); 
# line 9131 "src/yygram.cpp"
      } break;
   case 447: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2064 "lang/Grammar.acc"
 *ex = CodeOperator(GE_OP,ex1,ex2); 
# line 9139 "src/yygram.cpp"
      } break;
   case 448: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2066 "lang/Grammar.acc"
 *ex = CodeOperator(LE_OP,ex1,ex2); 
# line 9147 "src/yygram.cpp"
      } break;
   case 449: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2068 "lang/Grammar.acc"
 *ex = CodeOperator(EQ_OP,ex1,ex2); 
# line 9155 "src/yygram.cpp"
      } break;
   case 450: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2070 "lang/Grammar.acc"
 *ex = CodeOperator(NE_OP,ex1,ex2); 
# line 9163 "src/yygram.cpp"
      } break;
   case 451: {
      expr70(&ex1);
#line 2072 "lang/Grammar.acc"
 *ex = ex1; 
# line 9169 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr90 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 452: {
      expr90(&ex1);
      get_lexval();
      expr80(&ex2);
#line 2077 "lang/Grammar.acc"
 *ex = CodeOperator('&',ex1,ex2); 
# line 9186 "src/yygram.cpp"
      } break;
   case 453: {
      expr80(&ex1);
#line 2079 "lang/Grammar.acc"
 *ex = ex1; 
# line 9192 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr100 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 454: {
      expr100(&ex1);
      get_lexval();
      expr90(&ex2);
#line 2084 "lang/Grammar.acc"
 *ex = CodeOperator('^',ex1,ex2); 
# line 9209 "src/yygram.cpp"
      } break;
   case 455: {
      expr90(&ex1);
#line 2086 "lang/Grammar.acc"
 *ex = ex1; 
# line 9215 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr110 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 456: {
      expr110(&ex1);
      get_lexval();
      expr100(&ex2);
#line 2091 "lang/Grammar.acc"
 *ex = CodeOperator('|',ex1,ex2); 
# line 9232 "src/yygram.cpp"
      } break;
   case 457: {
      expr100(&ex1);
#line 2093 "lang/Grammar.acc"
 *ex = ex1; 
# line 9238 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr120 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 458: {
      expr120(&ex1);
      get_lexval();
      expr110(&ex2);
#line 2098 "lang/Grammar.acc"
 *ex = CodeOperator(AND_OP,ex1,ex2); 
# line 9255 "src/yygram.cpp"
      } break;
   case 459: {
      expr110(&ex1);
#line 2100 "lang/Grammar.acc"
 *ex = ex1; 
# line 9261 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr130 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 460: {
      expr130(&ex1);
      get_lexval();
      expr120(&ex2);
#line 2105 "lang/Grammar.acc"
 *ex = CodeOperator(OR_OP,ex1,ex2); 
# line 9278 "src/yygram.cpp"
      } break;
   case 461: {
      expr120(&ex1);
#line 2107 "lang/Grammar.acc"
 *ex = ex1; 
# line 9284 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr140 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   PExp ex3;
   switch(yyselect()) {
   case 462: {
      expr130(&ex1);
      get_lexval();
      expr(&ex2);
      get_lexval();
      expr140(&ex3);
#line 2112 "lang/Grammar.acc"

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
          FreeString((int16)-ex2.Value);
        if (ex3.Storage == RT_CONSTANT && ex3.Type == DT_STRING && ex3.Value < -1)
          FreeString((int16)-ex3.Value);
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
        FreeRegister((int16)ex2.Value);
      if (ex3.Storage == RT_REGISTER)
        FreeRegister((int16)ex3.Value);
    
# line 9352 "src/yygram.cpp"
      } break;
   case 463: {
      expr130(&ex1);
#line 2162 "lang/Grammar.acc"
 *ex = ex1; 
# line 9358 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr150 (PExp *ex)
{
   LExp lv;
   PExp ex2;
   PExp ex1;
   switch(yyselect()) {
   case 464: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2170 "lang/Grammar.acc"
 *ex = CodeAssignment('=',lv,&ex2); 
# line 9376 "src/yygram.cpp"
      } break;
   case 465: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2172 "lang/Grammar.acc"
 *ex = CodeAssignment(ASSIGN_ADD,lv,&ex2); 
# line 9384 "src/yygram.cpp"
      } break;
   case 466: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2174 "lang/Grammar.acc"
 *ex = CodeAssignment(ASSIGN_SUB,lv,&ex2); 
# line 9392 "src/yygram.cpp"
      } break;
   case 467: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2176 "lang/Grammar.acc"
 *ex = CodeAssignment(ASSIGN_MULT,lv,&ex2); 
# line 9400 "src/yygram.cpp"
      } break;
   case 468: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2178 "lang/Grammar.acc"
 *ex = CodeAssignment(ASSIGN_DIV,lv,&ex2); 
# line 9408 "src/yygram.cpp"
      } break;
   case 469: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2180 "lang/Grammar.acc"
 *ex = CodeAssignment(ASSIGN_MOD,lv,&ex2); 
# line 9416 "src/yygram.cpp"
      } break;
   case 470: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2182 "lang/Grammar.acc"
 *ex = CodeAssignment(ASSIGN_AND,lv,&ex2); 
# line 9424 "src/yygram.cpp"
      } break;
   case 471: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2184 "lang/Grammar.acc"
 *ex = CodeAssignment(ASSIGN_OR,lv,&ex2); 
# line 9432 "src/yygram.cpp"
      } break;
   case 472: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2186 "lang/Grammar.acc"
 *ex = CodeAssignment(ASSIGN_LSHIFT,lv,&ex2); 
# line 9440 "src/yygram.cpp"
      } break;
   case 473: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2188 "lang/Grammar.acc"
 *ex = CodeAssignment(ASSIGN_RSHIFT,lv,&ex2); 
# line 9448 "src/yygram.cpp"
      } break;
   case 474: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2190 "lang/Grammar.acc"
 *ex = CodeAssignment(ASSIGN_NEG,lv,&ex2); 
# line 9456 "src/yygram.cpp"
      } break;
   case 475: {
      expr140(&ex1);
#line 2192 "lang/Grammar.acc"
 *ex = ex1; 
# line 9462 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 476: {
      expr(&ex1);
      get_lexval();
      expr150(&ex2);
#line 2197 "lang/Grammar.acc"

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
    
# line 9490 "src/yygram.cpp"
      } break;
   case 477: {
      expr150(&ex1);
#line 2210 "lang/Grammar.acc"
 *ex = ex1; 
# line 9496 "src/yygram.cpp"
      } break;
   }
return 0;
}

int param_list (pBSysFunc b, YYSTYPE fn, VBlock *bl, int *narg)
{
   PExp p;
   switch(yyselect()) {
   case 478: {
#line 2218 "lang/Grammar.acc"
 *narg = 0; int16 i,j; bl->Clear();
    int16 PStorage[10]; int32 PValue[10]; 
# line 9510 "src/yygram.cpp"
      switch (yyselect()) {
      case 1023: {
         expr150(&p);
#line 2221 "lang/Grammar.acc"

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
            FreeString((int16)-p.Value);
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
    
# line 9541 "src/yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 1022: {
               get_lexval();
               expr150(&p);
#line 2248 "lang/Grammar.acc"

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
              FreeString((int16)-p.Value);
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
    
# line 9582 "src/yygram.cpp"
               } break;
            default: goto yy106;
            }
         }
         yy106: ;
         } break;
      default: ;
      }
#line 2283 "lang/Grammar.acc"

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
        bl->Generate(PUSH,(int8)PStorage[i],PValue[i]);
        if (PStorage[i] == RT_REGISTER)
          FreeRegister((int16)PValue[i]);
        }
      *narg = max(*narg,b->ParamCount);
      Done:;
      
# line 9611 "src/yygram.cpp"
      } break;
   }
return 0;
}

extern YYSTYPE yylval;
YYSTYPE yylval;
/* GentleFlag = no */

typedef struct LEXELEMSTRUCT {
   YYSTYPE val;
   long pos;
   long sym;
   char * text;
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
   p->next = 0;
   cur_lexelem = p;
}

void get_lexval() {
   extern int FREE_LEXELEMS;
   LEXELEM *p;
   yylval = cur_lexelem->val;
   yypos = cur_lexelem->pos;
   p = cur_lexelem;
   cur_lexelem = cur_lexelem->next;
   free(p);
}

extern int c_length;
int c_length = 6749;
extern int yygrammar[];
int yygrammar[] = {
0,
/* 1 */ 0,
/* 2 */ 6,
/* 3 */ 50000,
/* 4 */ -1,
/* 5 */ 479,
/* 6 */ 0,
/* 7 */ 10,
/* 8 */ -6,
/* 9 */ 1,
/* 10 */ 15,
/* 11 */ 18,
/* 12 */ 10,
/* 13 */ -10,
/* 14 */ 480,
/* 15 */ 0,
/* 16 */ -10,
/* 17 */ 1024,
/* 18 */ 24,
/* 19 */ 50380,
/* 20 */ 50257,
/* 21 */ 50059,
/* 22 */ -18,
/* 23 */ 2,
/* 24 */ 30,
/* 25 */ 50433,
/* 26 */ 223,
/* 27 */ 50059,
/* 28 */ -18,
/* 29 */ 3,
/* 30 */ 36,
/* 31 */ 50352,
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
/* 109 */ 5874,
/* 110 */ -18,
/* 111 */ 23,
/* 112 */ 116,
/* 113 */ 5964,
/* 114 */ -18,
/* 115 */ 24,
/* 116 */ 120,
/* 117 */ 6070,
/* 118 */ -18,
/* 119 */ 25,
/* 120 */ 124,
/* 121 */ 6052,
/* 122 */ -18,
/* 123 */ 26,
/* 124 */ 128,
/* 125 */ 6061,
/* 126 */ -18,
/* 127 */ 27,
/* 128 */ 134,
/* 129 */ 50350,
/* 130 */ 144,
/* 131 */ 173,
/* 132 */ -18,
/* 133 */ 28,
/* 134 */ 0,
/* 135 */ 50451,
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
/* 147 */ 481,
/* 148 */ 153,
/* 149 */ 50326,
/* 150 */ 50058,
/* 151 */ -148,
/* 152 */ 482,
/* 153 */ 0,
/* 154 */ -148,
/* 155 */ 1025,
/* 156 */ 160,
/* 157 */ 50059,
/* 158 */ -156,
/* 159 */ 483,
/* 160 */ 0,
/* 161 */ -156,
/* 162 */ 1026,
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
/* 248 */ 50458,
/* 249 */ -237,
/* 250 */ 46,
/* 251 */ 255,
/* 252 */ 50345,
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
/* 311 */ 50367,
/* 312 */ 50040,
/* 313 */ 588,
/* 314 */ 50041,
/* 315 */ -310,
/* 316 */ 484,
/* 317 */ 0,
/* 318 */ -310,
/* 319 */ 1027,
/* 320 */ 325,
/* 321 */ 50256,
/* 322 */ 50037,
/* 323 */ -320,
/* 324 */ 485,
/* 325 */ 0,
/* 326 */ -320,
/* 327 */ 1028,
/* 328 */ 332,
/* 329 */ 286,
/* 330 */ -328,
/* 331 */ 486,
/* 332 */ 0,
/* 333 */ -328,
/* 334 */ 1029,
/* 335 */ 339,
/* 336 */ 50317,
/* 337 */ -335,
/* 338 */ 487,
/* 339 */ 343,
/* 340 */ 50300,
/* 341 */ -335,
/* 342 */ 488,
/* 343 */ 0,
/* 344 */ -335,
/* 345 */ 1030,
/* 346 */ 350,
/* 347 */ 163,
/* 348 */ -346,
/* 349 */ 489,
/* 350 */ 0,
/* 351 */ 50419,
/* 352 */ 223,
/* 353 */ -346,
/* 354 */ 490,
/* 355 */ 360,
/* 356 */ 50043,
/* 357 */ 50256,
/* 358 */ -355,
/* 359 */ 491,
/* 360 */ 365,
/* 361 */ 50045,
/* 362 */ 50256,
/* 363 */ -355,
/* 364 */ 492,
/* 365 */ 0,
/* 366 */ -355,
/* 367 */ 1031,
/* 368 */ 376,
/* 369 */ 50471,
/* 370 */ 50091,
/* 371 */ 417,
/* 372 */ 50093,
/* 373 */ 368,
/* 374 */ -368,
/* 375 */ 495,
/* 376 */ 382,
/* 377 */ 50399,
/* 378 */ 163,
/* 379 */ 368,
/* 380 */ -368,
/* 381 */ 496,
/* 382 */ 391,
/* 383 */ 50399,
/* 384 */ 50419,
/* 385 */ 223,
/* 386 */ 50457,
/* 387 */ 223,
/* 388 */ 368,
/* 389 */ -368,
/* 390 */ 497,
/* 391 */ 398,
/* 392 */ 50399,
/* 393 */ 50419,
/* 394 */ 223,
/* 395 */ 368,
/* 396 */ -368,
/* 397 */ 498,
/* 398 */ 404,
/* 399 */ 50399,
/* 400 */ 50419,
/* 401 */ 368,
/* 402 */ -368,
/* 403 */ 499,
/* 404 */ 414,
/* 405 */ 50295,
/* 406 */ 50040,
/* 407 */ 223,
/* 408 */ 50044,
/* 409 */ 223,
/* 410 */ 50041,
/* 411 */ 368,
/* 412 */ -368,
/* 413 */ 500,
/* 414 */ 0,
/* 415 */ -368,
/* 416 */ 1034,
/* 417 */ 423,
/* 418 */ 173,
/* 419 */ 426,
/* 420 */ 417,
/* 421 */ -417,
/* 422 */ 494,
/* 423 */ 0,
/* 424 */ -417,
/* 425 */ 1033,
/* 426 */ 430,
/* 427 */ 50044,
/* 428 */ -426,
/* 429 */ 493,
/* 430 */ 0,
/* 431 */ -426,
/* 432 */ 1032,
/* 433 */ 0,
/* 434 */ 50356,
/* 435 */ 441,
/* 436 */ 445,
/* 437 */ 300,
/* 438 */ 50059,
/* 439 */ -433,
/* 440 */ 56,
/* 441 */ 0,
/* 442 */ 50058,
/* 443 */ -441,
/* 444 */ 501,
/* 445 */ 451,
/* 446 */ 300,
/* 447 */ 454,
/* 448 */ 445,
/* 449 */ -445,
/* 450 */ 504,
/* 451 */ 0,
/* 452 */ -445,
/* 453 */ 1035,
/* 454 */ 458,
/* 455 */ 50044,
/* 456 */ -454,
/* 457 */ 502,
/* 458 */ 0,
/* 459 */ 50335,
/* 460 */ -454,
/* 461 */ 503,
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
/* 472 */ 50368,
/* 473 */ -471,
/* 474 */ 505,
/* 475 */ 0,
/* 476 */ 50357,
/* 477 */ -471,
/* 478 */ 506,
/* 479 */ 483,
/* 480 */ 50058,
/* 481 */ -479,
/* 482 */ 507,
/* 483 */ 0,
/* 484 */ -479,
/* 485 */ 1036,
/* 486 */ 490,
/* 487 */ 1247,
/* 488 */ -486,
/* 489 */ 508,
/* 490 */ 0,
/* 491 */ -486,
/* 492 */ 1037,
/* 493 */ 497,
/* 494 */ 50259,
/* 495 */ -493,
/* 496 */ 509,
/* 497 */ 0,
/* 498 */ 50256,
/* 499 */ -493,
/* 500 */ 510,
/* 501 */ 506,
/* 502 */ 50400,
/* 503 */ 1247,
/* 504 */ -501,
/* 505 */ 511,
/* 506 */ 0,
/* 507 */ -501,
/* 508 */ 1038,
/* 509 */ 0,
/* 510 */ 515,
/* 511 */ 522,
/* 512 */ 530,
/* 513 */ -509,
/* 514 */ 58,
/* 515 */ 519,
/* 516 */ 1247,
/* 517 */ -515,
/* 518 */ 512,
/* 519 */ 0,
/* 520 */ -515,
/* 521 */ 1039,
/* 522 */ 526,
/* 523 */ 50259,
/* 524 */ -522,
/* 525 */ 513,
/* 526 */ 0,
/* 527 */ 50256,
/* 528 */ -522,
/* 529 */ 514,
/* 530 */ 535,
/* 531 */ 50400,
/* 532 */ 1247,
/* 533 */ -530,
/* 534 */ 515,
/* 535 */ 0,
/* 536 */ -530,
/* 537 */ 1040,
/* 538 */ 0,
/* 539 */ 50400,
/* 540 */ 50342,
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
/* 551 */ 516,
/* 552 */ 0,
/* 553 */ -546,
/* 554 */ 1041,
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
/* 566 */ 517,
/* 567 */ 0,
/* 568 */ -561,
/* 569 */ 1042,
/* 570 */ 575,
/* 571 */ 50257,
/* 572 */ 579,
/* 573 */ -570,
/* 574 */ 519,
/* 575 */ 0,
/* 576 */ 6265,
/* 577 */ -570,
/* 578 */ 520,
/* 579 */ 585,
/* 580 */ 50047,
/* 581 */ 50257,
/* 582 */ 579,
/* 583 */ -579,
/* 584 */ 518,
/* 585 */ 0,
/* 586 */ -579,
/* 587 */ 1043,
/* 588 */ 0,
/* 589 */ 6719,
/* 590 */ -588,
/* 591 */ 61,
/* 592 */ 0,
/* 593 */ 6118,
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
/* 605 */ 522,
/* 606 */ 611,
/* 607 */ 50045,
/* 608 */ 237,
/* 609 */ -601,
/* 610 */ 523,
/* 611 */ 616,
/* 612 */ 50061,
/* 613 */ 237,
/* 614 */ -601,
/* 615 */ 524,
/* 616 */ 0,
/* 617 */ 223,
/* 618 */ 50037,
/* 619 */ -601,
/* 620 */ 525,
/* 621 */ 625,
/* 622 */ 50043,
/* 623 */ -621,
/* 624 */ 521,
/* 625 */ 0,
/* 626 */ -621,
/* 627 */ 1044,
/* 628 */ 634,
/* 629 */ 50040,
/* 630 */ 637,
/* 631 */ 50041,
/* 632 */ -628,
/* 633 */ 529,
/* 634 */ 0,
/* 635 */ -628,
/* 636 */ 1045,
/* 637 */ 642,
/* 638 */ 50387,
/* 639 */ 237,
/* 640 */ -637,
/* 641 */ 526,
/* 642 */ 647,
/* 643 */ 50384,
/* 644 */ 237,
/* 645 */ -637,
/* 646 */ 527,
/* 647 */ 0,
/* 648 */ 50391,
/* 649 */ 237,
/* 650 */ -637,
/* 651 */ 528,
/* 652 */ 0,
/* 653 */ 50388,
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
/* 668 */ 531,
/* 669 */ 0,
/* 670 */ -663,
/* 671 */ 1047,
/* 672 */ 677,
/* 673 */ 50044,
/* 674 */ 223,
/* 675 */ -672,
/* 676 */ 530,
/* 677 */ 0,
/* 678 */ -672,
/* 679 */ 1046,
/* 680 */ 685,
/* 681 */ 688,
/* 682 */ 680,
/* 683 */ -680,
/* 684 */ 532,
/* 685 */ 0,
/* 686 */ -680,
/* 687 */ 1048,
/* 688 */ 695,
/* 689 */ 50326,
/* 690 */ 932,
/* 691 */ 50257,
/* 692 */ 50059,
/* 693 */ -688,
/* 694 */ 65,
/* 695 */ 702,
/* 696 */ 50325,
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
/* 719 */ 50450,
/* 720 */ 976,
/* 721 */ 173,
/* 722 */ 983,
/* 723 */ 50059,
/* 724 */ -688,
/* 725 */ 69,
/* 726 */ 733,
/* 727 */ 50364,
/* 728 */ 992,
/* 729 */ 173,
/* 730 */ 50059,
/* 731 */ -688,
/* 732 */ 70,
/* 733 */ 740,
/* 734 */ 50365,
/* 735 */ 999,
/* 736 */ 173,
/* 737 */ 50059,
/* 738 */ -688,
/* 739 */ 71,
/* 740 */ 748,
/* 741 */ 1006,
/* 742 */ 50366,
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
/* 753 */ 50440,
/* 754 */ 50091,
/* 755 */ 223,
/* 756 */ 50093,
/* 757 */ 50059,
/* 758 */ -688,
/* 759 */ 74,
/* 760 */ 770,
/* 761 */ 50440,
/* 762 */ 50091,
/* 763 */ 223,
/* 764 */ 50044,
/* 765 */ 223,
/* 766 */ 50093,
/* 767 */ 50059,
/* 768 */ -688,
/* 769 */ 75,
/* 770 */ 782,
/* 771 */ 50440,
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
/* 783 */ 50440,
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
/* 797 */ 50424,
/* 798 */ 1020,
/* 799 */ 1027,
/* 800 */ 50059,
/* 801 */ -688,
/* 802 */ 78,
/* 803 */ 810,
/* 804 */ 50369,
/* 805 */ 1043,
/* 806 */ 1050,
/* 807 */ 50059,
/* 808 */ -688,
/* 809 */ 79,
/* 810 */ 818,
/* 811 */ 50296,
/* 812 */ 1066,
/* 813 */ 1266,
/* 814 */ 1073,
/* 815 */ 50059,
/* 816 */ -688,
/* 817 */ 80,
/* 818 */ 824,
/* 819 */ 50294,
/* 820 */ 50257,
/* 821 */ 50059,
/* 822 */ -688,
/* 823 */ 81,
/* 824 */ 830,
/* 825 */ 50294,
/* 826 */ 223,
/* 827 */ 50059,
/* 828 */ -688,
/* 829 */ 82,
/* 830 */ 837,
/* 831 */ 50323,
/* 832 */ 1082,
/* 833 */ 173,
/* 834 */ 50059,
/* 835 */ -688,
/* 836 */ 83,
/* 837 */ 844,
/* 838 */ 50292,
/* 839 */ 1089,
/* 840 */ 173,
/* 841 */ 50059,
/* 842 */ -688,
/* 843 */ 84,
/* 844 */ 851,
/* 845 */ 50381,
/* 846 */ 1096,
/* 847 */ 173,
/* 848 */ 50059,
/* 849 */ -688,
/* 850 */ 85,
/* 851 */ 859,
/* 852 */ 50346,
/* 853 */ 1103,
/* 854 */ 1110,
/* 855 */ 223,
/* 856 */ 50059,
/* 857 */ -688,
/* 858 */ 86,
/* 859 */ 866,
/* 860 */ 50350,
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
/* 873 */ 50431,
/* 874 */ 1143,
/* 875 */ 223,
/* 876 */ 50059,
/* 877 */ -688,
/* 878 */ 89,
/* 879 */ 887,
/* 880 */ 50435,
/* 881 */ 1150,
/* 882 */ 223,
/* 883 */ 1157,
/* 884 */ 50059,
/* 885 */ -688,
/* 886 */ 90,
/* 887 */ 895,
/* 888 */ 50389,
/* 889 */ 1164,
/* 890 */ 223,
/* 891 */ 1171,
/* 892 */ 50059,
/* 893 */ -688,
/* 894 */ 91,
/* 895 */ 902,
/* 896 */ 50437,
/* 897 */ 1178,
/* 898 */ 1185,
/* 899 */ 50059,
/* 900 */ -688,
/* 901 */ 92,
/* 902 */ 909,
/* 903 */ 50330,
/* 904 */ 1201,
/* 905 */ 1208,
/* 906 */ 50059,
/* 907 */ -688,
/* 908 */ 93,
/* 909 */ 916,
/* 910 */ 50409,
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
/* 929 */ 6102,
/* 930 */ -688,
/* 931 */ 98,
/* 932 */ 936,
/* 933 */ 50058,
/* 934 */ -932,
/* 935 */ 533,
/* 936 */ 0,
/* 937 */ -932,
/* 938 */ 1049,
/* 939 */ 943,
/* 940 */ 50058,
/* 941 */ -939,
/* 942 */ 534,
/* 943 */ 0,
/* 944 */ -939,
/* 945 */ 1050,
/* 946 */ 950,
/* 947 */ 50374,
/* 948 */ -946,
/* 949 */ 535,
/* 950 */ 0,
/* 951 */ 50307,
/* 952 */ -946,
/* 953 */ 536,
/* 954 */ 958,
/* 955 */ 50058,
/* 956 */ -954,
/* 957 */ 537,
/* 958 */ 0,
/* 959 */ -954,
/* 960 */ 1051,
/* 961 */ 965,
/* 962 */ 50374,
/* 963 */ -961,
/* 964 */ 538,
/* 965 */ 0,
/* 966 */ 50307,
/* 967 */ -961,
/* 968 */ 539,
/* 969 */ 973,
/* 970 */ 50058,
/* 971 */ -969,
/* 972 */ 540,
/* 973 */ 0,
/* 974 */ -969,
/* 975 */ 1052,
/* 976 */ 980,
/* 977 */ 50058,
/* 978 */ -976,
/* 979 */ 541,
/* 980 */ 0,
/* 981 */ -976,
/* 982 */ 1053,
/* 983 */ 989,
/* 984 */ 50044,
/* 985 */ 173,
/* 986 */ 983,
/* 987 */ -983,
/* 988 */ 542,
/* 989 */ 0,
/* 990 */ -983,
/* 991 */ 1054,
/* 992 */ 996,
/* 993 */ 50058,
/* 994 */ -992,
/* 995 */ 543,
/* 996 */ 0,
/* 997 */ -992,
/* 998 */ 1055,
/* 999 */ 1003,
/* 1000 */ 50058,
/* 1001 */ -999,
/* 1002 */ 544,
/* 1003 */ 0,
/* 1004 */ -999,
/* 1005 */ 1056,
/* 1006 */ 1010,
/* 1007 */ 50043,
/* 1008 */ -1006,
/* 1009 */ 545,
/* 1010 */ 0,
/* 1011 */ -1006,
/* 1012 */ 1057,
/* 1013 */ 1017,
/* 1014 */ 50058,
/* 1015 */ -1013,
/* 1016 */ 546,
/* 1017 */ 0,
/* 1018 */ -1013,
/* 1019 */ 1058,
/* 1020 */ 1024,
/* 1021 */ 50058,
/* 1022 */ -1020,
/* 1023 */ 547,
/* 1024 */ 0,
/* 1025 */ -1020,
/* 1026 */ 1059,
/* 1027 */ 1033,
/* 1028 */ 223,
/* 1029 */ 1036,
/* 1030 */ 1027,
/* 1031 */ -1027,
/* 1032 */ 549,
/* 1033 */ 0,
/* 1034 */ -1027,
/* 1035 */ 1061,
/* 1036 */ 1040,
/* 1037 */ 50044,
/* 1038 */ -1036,
/* 1039 */ 548,
/* 1040 */ 0,
/* 1041 */ -1036,
/* 1042 */ 1060,
/* 1043 */ 1047,
/* 1044 */ 50058,
/* 1045 */ -1043,
/* 1046 */ 550,
/* 1047 */ 0,
/* 1048 */ -1043,
/* 1049 */ 1062,
/* 1050 */ 1056,
/* 1051 */ 223,
/* 1052 */ 1059,
/* 1053 */ 1050,
/* 1054 */ -1050,
/* 1055 */ 552,
/* 1056 */ 0,
/* 1057 */ -1050,
/* 1058 */ 1064,
/* 1059 */ 1063,
/* 1060 */ 50044,
/* 1061 */ -1059,
/* 1062 */ 551,
/* 1063 */ 0,
/* 1064 */ -1059,
/* 1065 */ 1063,
/* 1066 */ 1070,
/* 1067 */ 50058,
/* 1068 */ -1066,
/* 1069 */ 553,
/* 1070 */ 0,
/* 1071 */ -1066,
/* 1072 */ 1065,
/* 1073 */ 1079,
/* 1074 */ 50044,
/* 1075 */ 1266,
/* 1076 */ 1073,
/* 1077 */ -1073,
/* 1078 */ 554,
/* 1079 */ 0,
/* 1080 */ -1073,
/* 1081 */ 1066,
/* 1082 */ 1086,
/* 1083 */ 50058,
/* 1084 */ -1082,
/* 1085 */ 555,
/* 1086 */ 0,
/* 1087 */ -1082,
/* 1088 */ 1067,
/* 1089 */ 1093,
/* 1090 */ 50058,
/* 1091 */ -1089,
/* 1092 */ 556,
/* 1093 */ 0,
/* 1094 */ -1089,
/* 1095 */ 1068,
/* 1096 */ 1100,
/* 1097 */ 50058,
/* 1098 */ -1096,
/* 1099 */ 557,
/* 1100 */ 0,
/* 1101 */ -1096,
/* 1102 */ 1069,
/* 1103 */ 1107,
/* 1104 */ 50058,
/* 1105 */ -1103,
/* 1106 */ 558,
/* 1107 */ 0,
/* 1108 */ -1103,
/* 1109 */ 1070,
/* 1110 */ 1116,
/* 1111 */ 223,
/* 1112 */ 50044,
/* 1113 */ 1110,
/* 1114 */ -1110,
/* 1115 */ 559,
/* 1116 */ 0,
/* 1117 */ -1110,
/* 1118 */ 1071,
/* 1119 */ 1123,
/* 1120 */ 50058,
/* 1121 */ -1119,
/* 1122 */ 560,
/* 1123 */ 0,
/* 1124 */ -1119,
/* 1125 */ 1072,
/* 1126 */ 1131,
/* 1127 */ 1393,
/* 1128 */ 1126,
/* 1129 */ -1126,
/* 1130 */ 561,
/* 1131 */ 0,
/* 1132 */ -1126,
/* 1133 */ 1073,
/* 1134 */ 1140,
/* 1135 */ 50044,
/* 1136 */ 1418,
/* 1137 */ 1134,
/* 1138 */ -1134,
/* 1139 */ 562,
/* 1140 */ 0,
/* 1141 */ -1134,
/* 1142 */ 1074,
/* 1143 */ 1147,
/* 1144 */ 50058,
/* 1145 */ -1143,
/* 1146 */ 563,
/* 1147 */ 0,
/* 1148 */ -1143,
/* 1149 */ 1075,
/* 1150 */ 1154,
/* 1151 */ 50058,
/* 1152 */ -1150,
/* 1153 */ 564,
/* 1154 */ 0,
/* 1155 */ -1150,
/* 1156 */ 1076,
/* 1157 */ 1161,
/* 1158 */ 50037,
/* 1159 */ -1157,
/* 1160 */ 565,
/* 1161 */ 0,
/* 1162 */ -1157,
/* 1163 */ 1077,
/* 1164 */ 1168,
/* 1165 */ 50058,
/* 1166 */ -1164,
/* 1167 */ 566,
/* 1168 */ 0,
/* 1169 */ -1164,
/* 1170 */ 1078,
/* 1171 */ 1175,
/* 1172 */ 50037,
/* 1173 */ -1171,
/* 1174 */ 567,
/* 1175 */ 0,
/* 1176 */ -1171,
/* 1177 */ 1079,
/* 1178 */ 1182,
/* 1179 */ 50058,
/* 1180 */ -1178,
/* 1181 */ 568,
/* 1182 */ 0,
/* 1183 */ -1178,
/* 1184 */ 1080,
/* 1185 */ 1191,
/* 1186 */ 163,
/* 1187 */ 1194,
/* 1188 */ 1185,
/* 1189 */ -1185,
/* 1190 */ 570,
/* 1191 */ 0,
/* 1192 */ -1185,
/* 1193 */ 1082,
/* 1194 */ 1198,
/* 1195 */ 50044,
/* 1196 */ -1194,
/* 1197 */ 569,
/* 1198 */ 0,
/* 1199 */ -1194,
/* 1200 */ 1081,
/* 1201 */ 1205,
/* 1202 */ 50058,
/* 1203 */ -1201,
/* 1204 */ 571,
/* 1205 */ 0,
/* 1206 */ -1201,
/* 1207 */ 1083,
/* 1208 */ 1214,
/* 1209 */ 163,
/* 1210 */ 1217,
/* 1211 */ 1208,
/* 1212 */ -1208,
/* 1213 */ 573,
/* 1214 */ 0,
/* 1215 */ -1208,
/* 1216 */ 1085,
/* 1217 */ 1221,
/* 1218 */ 50044,
/* 1219 */ -1217,
/* 1220 */ 572,
/* 1221 */ 0,
/* 1222 */ -1217,
/* 1223 */ 1084,
/* 1224 */ 1228,
/* 1225 */ 50058,
/* 1226 */ -1224,
/* 1227 */ 574,
/* 1228 */ 0,
/* 1229 */ -1224,
/* 1230 */ 1086,
/* 1231 */ 1237,
/* 1232 */ 163,
/* 1233 */ 1240,
/* 1234 */ 1231,
/* 1235 */ -1231,
/* 1236 */ 576,
/* 1237 */ 0,
/* 1238 */ -1231,
/* 1239 */ 1088,
/* 1240 */ 1244,
/* 1241 */ 50044,
/* 1242 */ -1240,
/* 1243 */ 575,
/* 1244 */ 0,
/* 1245 */ -1240,
/* 1246 */ 1087,
/* 1247 */ 1251,
/* 1248 */ 50275,
/* 1249 */ -1247,
/* 1250 */ 99,
/* 1251 */ 1256,
/* 1252 */ 50303,
/* 1253 */ 50275,
/* 1254 */ -1247,
/* 1255 */ 100,
/* 1256 */ 1261,
/* 1257 */ 50376,
/* 1258 */ 50275,
/* 1259 */ -1247,
/* 1260 */ 101,
/* 1261 */ 0,
/* 1262 */ 50320,
/* 1263 */ 50275,
/* 1264 */ -1247,
/* 1265 */ 102,
/* 1266 */ 1276,
/* 1267 */ 223,
/* 1268 */ 50353,
/* 1269 */ 1283,
/* 1270 */ 1295,
/* 1271 */ 1302,
/* 1272 */ 1309,
/* 1273 */ 1331,
/* 1274 */ -1266,
/* 1275 */ 103,
/* 1276 */ 0,
/* 1277 */ 223,
/* 1278 */ 50353,
/* 1279 */ 163,
/* 1280 */ 1386,
/* 1281 */ -1266,
/* 1282 */ 104,
/* 1283 */ 1287,
/* 1284 */ 286,
/* 1285 */ -1283,
/* 1286 */ 577,
/* 1287 */ 1292,
/* 1288 */ 237,
/* 1289 */ 50037,
/* 1290 */ -1283,
/* 1291 */ 578,
/* 1292 */ 0,
/* 1293 */ -1283,
/* 1294 */ 1089,
/* 1295 */ 1299,
/* 1296 */ 237,
/* 1297 */ -1295,
/* 1298 */ 579,
/* 1299 */ 0,
/* 1300 */ -1295,
/* 1301 */ 1090,
/* 1302 */ 1306,
/* 1303 */ 50319,
/* 1304 */ -1302,
/* 1305 */ 580,
/* 1306 */ 0,
/* 1307 */ -1302,
/* 1308 */ 1091,
/* 1309 */ 1316,
/* 1310 */ 50040,
/* 1311 */ 1319,
/* 1312 */ 223,
/* 1313 */ 50041,
/* 1314 */ -1309,
/* 1315 */ 584,
/* 1316 */ 0,
/* 1317 */ -1309,
/* 1318 */ 1092,
/* 1319 */ 1323,
/* 1320 */ 50322,
/* 1321 */ -1319,
/* 1322 */ 581,
/* 1323 */ 1327,
/* 1324 */ 50307,
/* 1325 */ -1319,
/* 1326 */ 582,
/* 1327 */ 0,
/* 1328 */ 50374,
/* 1329 */ -1319,
/* 1330 */ 583,
/* 1331 */ 1340,
/* 1332 */ 50288,
/* 1333 */ 1343,
/* 1334 */ 1350,
/* 1335 */ 1357,
/* 1336 */ 1364,
/* 1337 */ 1331,
/* 1338 */ -1331,
/* 1339 */ 592,
/* 1340 */ 0,
/* 1341 */ -1331,
/* 1342 */ 1097,
/* 1343 */ 1347,
/* 1344 */ 286,
/* 1345 */ -1343,
/* 1346 */ 585,
/* 1347 */ 0,
/* 1348 */ -1343,
/* 1349 */ 1093,
/* 1350 */ 1354,
/* 1351 */ 223,
/* 1352 */ -1350,
/* 1353 */ 586,
/* 1354 */ 0,
/* 1355 */ -1350,
/* 1356 */ 1094,
/* 1357 */ 1361,
/* 1358 */ 50319,
/* 1359 */ -1357,
/* 1360 */ 587,
/* 1361 */ 0,
/* 1362 */ -1357,
/* 1363 */ 1095,
/* 1364 */ 1371,
/* 1365 */ 50040,
/* 1366 */ 1374,
/* 1367 */ 223,
/* 1368 */ 50041,
/* 1369 */ -1364,
/* 1370 */ 591,
/* 1371 */ 0,
/* 1372 */ -1364,
/* 1373 */ 1096,
/* 1374 */ 1378,
/* 1375 */ 50322,
/* 1376 */ -1374,
/* 1377 */ 588,
/* 1378 */ 1382,
/* 1379 */ 50307,
/* 1380 */ -1374,
/* 1381 */ 589,
/* 1382 */ 0,
/* 1383 */ 50374,
/* 1384 */ -1374,
/* 1385 */ 590,
/* 1386 */ 1390,
/* 1387 */ 223,
/* 1388 */ -1386,
/* 1389 */ 593,
/* 1390 */ 0,
/* 1391 */ -1386,
/* 1392 */ 1098,
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
/* 1407 */ 594,
/* 1408 */ 0,
/* 1409 */ -1404,
/* 1410 */ 1099,
/* 1411 */ 1415,
/* 1412 */ 50044,
/* 1413 */ -1411,
/* 1414 */ 595,
/* 1415 */ 0,
/* 1416 */ -1411,
/* 1417 */ 1100,
/* 1418 */ 0,
/* 1419 */ 50277,
/* 1420 */ 1424,
/* 1421 */ 223,
/* 1422 */ -1418,
/* 1423 */ 107,
/* 1424 */ 1428,
/* 1425 */ 50058,
/* 1426 */ -1424,
/* 1427 */ 596,
/* 1428 */ 0,
/* 1429 */ -1424,
/* 1430 */ 1101,
/* 1431 */ 0,
/* 1432 */ 50372,
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
/* 1445 */ 597,
/* 1446 */ 0,
/* 1447 */ -1441,
/* 1448 */ 1102,
/* 1449 */ 1456,
/* 1450 */ 50326,
/* 1451 */ 1715,
/* 1452 */ 50257,
/* 1453 */ 50059,
/* 1454 */ -1449,
/* 1455 */ 109,
/* 1456 */ 1463,
/* 1457 */ 50374,
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
/* 1468 */ 50430,
/* 1469 */ 1729,
/* 1470 */ 259,
/* 1471 */ 50059,
/* 1472 */ -1449,
/* 1473 */ 112,
/* 1474 */ 1481,
/* 1475 */ 50373,
/* 1476 */ 1736,
/* 1477 */ 259,
/* 1478 */ 50059,
/* 1479 */ -1449,
/* 1480 */ 113,
/* 1481 */ 1488,
/* 1482 */ 50316,
/* 1483 */ 1743,
/* 1484 */ 223,
/* 1485 */ 50059,
/* 1486 */ -1449,
/* 1487 */ 114,
/* 1488 */ 1495,
/* 1489 */ 50452,
/* 1490 */ 1750,
/* 1491 */ 223,
/* 1492 */ 50059,
/* 1493 */ -1449,
/* 1494 */ 115,
/* 1495 */ 1502,
/* 1496 */ 50284,
/* 1497 */ 1757,
/* 1498 */ 173,
/* 1499 */ 50059,
/* 1500 */ -1449,
/* 1501 */ 116,
/* 1502 */ 1509,
/* 1503 */ 50435,
/* 1504 */ 1764,
/* 1505 */ 173,
/* 1506 */ 50059,
/* 1507 */ -1449,
/* 1508 */ 117,
/* 1509 */ 1517,
/* 1510 */ 50435,
/* 1511 */ 1771,
/* 1512 */ 237,
/* 1513 */ 50037,
/* 1514 */ 50059,
/* 1515 */ -1449,
/* 1516 */ 118,
/* 1517 */ 1526,
/* 1518 */ 50435,
/* 1519 */ 1778,
/* 1520 */ 50043,
/* 1521 */ 237,
/* 1522 */ 50037,
/* 1523 */ 50059,
/* 1524 */ -1449,
/* 1525 */ 119,
/* 1526 */ 1533,
/* 1527 */ 50404,
/* 1528 */ 1785,
/* 1529 */ 173,
/* 1530 */ 50059,
/* 1531 */ -1449,
/* 1532 */ 120,
/* 1533 */ 1540,
/* 1534 */ 50420,
/* 1535 */ 1792,
/* 1536 */ 173,
/* 1537 */ 50059,
/* 1538 */ -1449,
/* 1539 */ 121,
/* 1540 */ 1547,
/* 1541 */ 50383,
/* 1542 */ 1799,
/* 1543 */ 223,
/* 1544 */ 50059,
/* 1545 */ -1449,
/* 1546 */ 122,
/* 1547 */ 1555,
/* 1548 */ 50315,
/* 1549 */ 1806,
/* 1550 */ 223,
/* 1551 */ 1813,
/* 1552 */ 50059,
/* 1553 */ -1449,
/* 1554 */ 123,
/* 1555 */ 1562,
/* 1556 */ 50466,
/* 1557 */ 1820,
/* 1558 */ 223,
/* 1559 */ 50059,
/* 1560 */ -1449,
/* 1561 */ 124,
/* 1562 */ 1569,
/* 1563 */ 50431,
/* 1564 */ 1827,
/* 1565 */ 223,
/* 1566 */ 50059,
/* 1567 */ -1449,
/* 1568 */ 125,
/* 1569 */ 1576,
/* 1570 */ 50396,
/* 1571 */ 1834,
/* 1572 */ 223,
/* 1573 */ 50059,
/* 1574 */ -1449,
/* 1575 */ 126,
/* 1576 */ 1583,
/* 1577 */ 50323,
/* 1578 */ 1841,
/* 1579 */ 223,
/* 1580 */ 50059,
/* 1581 */ -1449,
/* 1582 */ 127,
/* 1583 */ 1590,
/* 1584 */ 50312,
/* 1585 */ 1848,
/* 1586 */ 223,
/* 1587 */ 50059,
/* 1588 */ -1449,
/* 1589 */ 128,
/* 1590 */ 1597,
/* 1591 */ 50407,
/* 1592 */ 1855,
/* 1593 */ 223,
/* 1594 */ 50059,
/* 1595 */ -1449,
/* 1596 */ 129,
/* 1597 */ 1604,
/* 1598 */ 50292,
/* 1599 */ 1862,
/* 1600 */ 1869,
/* 1601 */ 50059,
/* 1602 */ -1449,
/* 1603 */ 130,
/* 1604 */ 1611,
/* 1605 */ 50365,
/* 1606 */ 1877,
/* 1607 */ 223,
/* 1608 */ 50059,
/* 1609 */ -1449,
/* 1610 */ 131,
/* 1611 */ 1618,
/* 1612 */ 50437,
/* 1613 */ 1884,
/* 1614 */ 1891,
/* 1615 */ 50059,
/* 1616 */ -1449,
/* 1617 */ 132,
/* 1618 */ 1625,
/* 1619 */ 50349,
/* 1620 */ 1907,
/* 1621 */ 1914,
/* 1622 */ 50059,
/* 1623 */ -1449,
/* 1624 */ 133,
/* 1625 */ 1632,
/* 1626 */ 50350,
/* 1627 */ 1930,
/* 1628 */ 1937,
/* 1629 */ 50059,
/* 1630 */ -1449,
/* 1631 */ 134,
/* 1632 */ 1639,
/* 1633 */ 50361,
/* 1634 */ 1945,
/* 1635 */ 1952,
/* 1636 */ 50059,
/* 1637 */ -1449,
/* 1638 */ 135,
/* 1639 */ 1646,
/* 1640 */ 50304,
/* 1641 */ 1972,
/* 1642 */ 223,
/* 1643 */ 50059,
/* 1644 */ -1449,
/* 1645 */ 136,
/* 1646 */ 1653,
/* 1647 */ 50468,
/* 1648 */ 1979,
/* 1649 */ 223,
/* 1650 */ 50059,
/* 1651 */ -1449,
/* 1652 */ 137,
/* 1653 */ 1660,
/* 1654 */ 50467,
/* 1655 */ 1986,
/* 1656 */ 223,
/* 1657 */ 50059,
/* 1658 */ -1449,
/* 1659 */ 138,
/* 1660 */ 1667,
/* 1661 */ 50379,
/* 1662 */ 1993,
/* 1663 */ 223,
/* 1664 */ 50059,
/* 1665 */ -1449,
/* 1666 */ 139,
/* 1667 */ 1674,
/* 1668 */ 50456,
/* 1669 */ 2000,
/* 1670 */ 223,
/* 1671 */ 50059,
/* 1672 */ -1449,
/* 1673 */ 140,
/* 1674 */ 1681,
/* 1675 */ 50313,
/* 1676 */ 2007,
/* 1677 */ 223,
/* 1678 */ 50059,
/* 1679 */ -1449,
/* 1680 */ 141,
/* 1681 */ 1689,
/* 1682 */ 50376,
/* 1683 */ 50420,
/* 1684 */ 2014,
/* 1685 */ 223,
/* 1686 */ 50059,
/* 1687 */ -1449,
/* 1688 */ 142,
/* 1689 */ 1696,
/* 1690 */ 50375,
/* 1691 */ 2021,
/* 1692 */ 223,
/* 1693 */ 50059,
/* 1694 */ -1449,
/* 1695 */ 143,
/* 1696 */ 1703,
/* 1697 */ 50355,
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
/* 1712 */ 6102,
/* 1713 */ -1449,
/* 1714 */ 147,
/* 1715 */ 1719,
/* 1716 */ 50058,
/* 1717 */ -1715,
/* 1718 */ 598,
/* 1719 */ 0,
/* 1720 */ -1715,
/* 1721 */ 1103,
/* 1722 */ 1726,
/* 1723 */ 50058,
/* 1724 */ -1722,
/* 1725 */ 599,
/* 1726 */ 0,
/* 1727 */ -1722,
/* 1728 */ 1104,
/* 1729 */ 1733,
/* 1730 */ 50058,
/* 1731 */ -1729,
/* 1732 */ 600,
/* 1733 */ 0,
/* 1734 */ -1729,
/* 1735 */ 1105,
/* 1736 */ 1740,
/* 1737 */ 50058,
/* 1738 */ -1736,
/* 1739 */ 601,
/* 1740 */ 0,
/* 1741 */ -1736,
/* 1742 */ 1106,
/* 1743 */ 1747,
/* 1744 */ 50058,
/* 1745 */ -1743,
/* 1746 */ 602,
/* 1747 */ 0,
/* 1748 */ -1743,
/* 1749 */ 1107,
/* 1750 */ 1754,
/* 1751 */ 50058,
/* 1752 */ -1750,
/* 1753 */ 603,
/* 1754 */ 0,
/* 1755 */ -1750,
/* 1756 */ 1108,
/* 1757 */ 1761,
/* 1758 */ 50058,
/* 1759 */ -1757,
/* 1760 */ 604,
/* 1761 */ 0,
/* 1762 */ -1757,
/* 1763 */ 1109,
/* 1764 */ 1768,
/* 1765 */ 50058,
/* 1766 */ -1764,
/* 1767 */ 605,
/* 1768 */ 0,
/* 1769 */ -1764,
/* 1770 */ 1110,
/* 1771 */ 1775,
/* 1772 */ 50058,
/* 1773 */ -1771,
/* 1774 */ 606,
/* 1775 */ 0,
/* 1776 */ -1771,
/* 1777 */ 1111,
/* 1778 */ 1782,
/* 1779 */ 50058,
/* 1780 */ -1778,
/* 1781 */ 607,
/* 1782 */ 0,
/* 1783 */ -1778,
/* 1784 */ 1112,
/* 1785 */ 1789,
/* 1786 */ 50058,
/* 1787 */ -1785,
/* 1788 */ 608,
/* 1789 */ 0,
/* 1790 */ -1785,
/* 1791 */ 1113,
/* 1792 */ 1796,
/* 1793 */ 50058,
/* 1794 */ -1792,
/* 1795 */ 609,
/* 1796 */ 0,
/* 1797 */ -1792,
/* 1798 */ 1114,
/* 1799 */ 1803,
/* 1800 */ 50058,
/* 1801 */ -1799,
/* 1802 */ 610,
/* 1803 */ 0,
/* 1804 */ -1799,
/* 1805 */ 1115,
/* 1806 */ 1810,
/* 1807 */ 50058,
/* 1808 */ -1806,
/* 1809 */ 611,
/* 1810 */ 0,
/* 1811 */ -1806,
/* 1812 */ 1116,
/* 1813 */ 1817,
/* 1814 */ 223,
/* 1815 */ -1813,
/* 1816 */ 612,
/* 1817 */ 0,
/* 1818 */ -1813,
/* 1819 */ 1117,
/* 1820 */ 1824,
/* 1821 */ 50058,
/* 1822 */ -1820,
/* 1823 */ 613,
/* 1824 */ 0,
/* 1825 */ -1820,
/* 1826 */ 1118,
/* 1827 */ 1831,
/* 1828 */ 50058,
/* 1829 */ -1827,
/* 1830 */ 614,
/* 1831 */ 0,
/* 1832 */ -1827,
/* 1833 */ 1119,
/* 1834 */ 1838,
/* 1835 */ 50058,
/* 1836 */ -1834,
/* 1837 */ 615,
/* 1838 */ 0,
/* 1839 */ -1834,
/* 1840 */ 1120,
/* 1841 */ 1845,
/* 1842 */ 50058,
/* 1843 */ -1841,
/* 1844 */ 616,
/* 1845 */ 0,
/* 1846 */ -1841,
/* 1847 */ 1121,
/* 1848 */ 1852,
/* 1849 */ 50058,
/* 1850 */ -1848,
/* 1851 */ 617,
/* 1852 */ 0,
/* 1853 */ -1848,
/* 1854 */ 1122,
/* 1855 */ 1859,
/* 1856 */ 50058,
/* 1857 */ -1855,
/* 1858 */ 618,
/* 1859 */ 0,
/* 1860 */ -1855,
/* 1861 */ 1123,
/* 1862 */ 1866,
/* 1863 */ 50058,
/* 1864 */ -1862,
/* 1865 */ 619,
/* 1866 */ 0,
/* 1867 */ -1862,
/* 1868 */ 1124,
/* 1869 */ 1874,
/* 1870 */ 2035,
/* 1871 */ 1869,
/* 1872 */ -1869,
/* 1873 */ 620,
/* 1874 */ 0,
/* 1875 */ -1869,
/* 1876 */ 1125,
/* 1877 */ 1881,
/* 1878 */ 50058,
/* 1879 */ -1877,
/* 1880 */ 621,
/* 1881 */ 0,
/* 1882 */ -1877,
/* 1883 */ 1126,
/* 1884 */ 1888,
/* 1885 */ 50058,
/* 1886 */ -1884,
/* 1887 */ 622,
/* 1888 */ 0,
/* 1889 */ -1884,
/* 1890 */ 1127,
/* 1891 */ 1897,
/* 1892 */ 163,
/* 1893 */ 1900,
/* 1894 */ 1891,
/* 1895 */ -1891,
/* 1896 */ 624,
/* 1897 */ 0,
/* 1898 */ -1891,
/* 1899 */ 1129,
/* 1900 */ 1904,
/* 1901 */ 50044,
/* 1902 */ -1900,
/* 1903 */ 623,
/* 1904 */ 0,
/* 1905 */ -1900,
/* 1906 */ 1128,
/* 1907 */ 1911,
/* 1908 */ 50058,
/* 1909 */ -1907,
/* 1910 */ 625,
/* 1911 */ 0,
/* 1912 */ -1907,
/* 1913 */ 1130,
/* 1914 */ 1920,
/* 1915 */ 163,
/* 1916 */ 1923,
/* 1917 */ 1914,
/* 1918 */ -1914,
/* 1919 */ 627,
/* 1920 */ 0,
/* 1921 */ -1914,
/* 1922 */ 1132,
/* 1923 */ 1927,
/* 1924 */ 50044,
/* 1925 */ -1923,
/* 1926 */ 626,
/* 1927 */ 0,
/* 1928 */ -1923,
/* 1929 */ 1131,
/* 1930 */ 1934,
/* 1931 */ 50058,
/* 1932 */ -1930,
/* 1933 */ 628,
/* 1934 */ 0,
/* 1935 */ -1930,
/* 1936 */ 1133,
/* 1937 */ 1942,
/* 1938 */ 2060,
/* 1939 */ 1937,
/* 1940 */ -1937,
/* 1941 */ 629,
/* 1942 */ 0,
/* 1943 */ -1937,
/* 1944 */ 1134,
/* 1945 */ 1949,
/* 1946 */ 50058,
/* 1947 */ -1945,
/* 1948 */ 630,
/* 1949 */ 0,
/* 1950 */ -1945,
/* 1951 */ 1135,
/* 1952 */ 1958,
/* 1953 */ 223,
/* 1954 */ 1961,
/* 1955 */ 1952,
/* 1956 */ -1952,
/* 1957 */ 633,
/* 1958 */ 0,
/* 1959 */ -1952,
/* 1960 */ 1137,
/* 1961 */ 1965,
/* 1962 */ 50044,
/* 1963 */ -1961,
/* 1964 */ 631,
/* 1965 */ 1969,
/* 1966 */ 50124,
/* 1967 */ -1961,
/* 1968 */ 632,
/* 1969 */ 0,
/* 1970 */ -1961,
/* 1971 */ 1136,
/* 1972 */ 1976,
/* 1973 */ 50058,
/* 1974 */ -1972,
/* 1975 */ 634,
/* 1976 */ 0,
/* 1977 */ -1972,
/* 1978 */ 1138,
/* 1979 */ 1983,
/* 1980 */ 50058,
/* 1981 */ -1979,
/* 1982 */ 635,
/* 1983 */ 0,
/* 1984 */ -1979,
/* 1985 */ 1139,
/* 1986 */ 1990,
/* 1987 */ 50058,
/* 1988 */ -1986,
/* 1989 */ 636,
/* 1990 */ 0,
/* 1991 */ -1986,
/* 1992 */ 1140,
/* 1993 */ 1997,
/* 1994 */ 50058,
/* 1995 */ -1993,
/* 1996 */ 637,
/* 1997 */ 0,
/* 1998 */ -1993,
/* 1999 */ 1141,
/* 2000 */ 2004,
/* 2001 */ 50058,
/* 2002 */ -2000,
/* 2003 */ 638,
/* 2004 */ 0,
/* 2005 */ -2000,
/* 2006 */ 1142,
/* 2007 */ 2011,
/* 2008 */ 50058,
/* 2009 */ -2007,
/* 2010 */ 639,
/* 2011 */ 0,
/* 2012 */ -2007,
/* 2013 */ 1143,
/* 2014 */ 2018,
/* 2015 */ 50058,
/* 2016 */ -2014,
/* 2017 */ 640,
/* 2018 */ 0,
/* 2019 */ -2014,
/* 2020 */ 1144,
/* 2021 */ 2025,
/* 2022 */ 50058,
/* 2023 */ -2021,
/* 2024 */ 641,
/* 2025 */ 0,
/* 2026 */ -2021,
/* 2027 */ 1145,
/* 2028 */ 2032,
/* 2029 */ 50058,
/* 2030 */ -2028,
/* 2031 */ 642,
/* 2032 */ 0,
/* 2033 */ -2028,
/* 2034 */ 1146,
/* 2035 */ 2040,
/* 2036 */ 223,
/* 2037 */ 2046,
/* 2038 */ -2035,
/* 2039 */ 148,
/* 2040 */ 0,
/* 2041 */ 50276,
/* 2042 */ 223,
/* 2043 */ 2053,
/* 2044 */ -2035,
/* 2045 */ 149,
/* 2046 */ 2050,
/* 2047 */ 50044,
/* 2048 */ -2046,
/* 2049 */ 643,
/* 2050 */ 0,
/* 2051 */ -2046,
/* 2052 */ 1147,
/* 2053 */ 2057,
/* 2054 */ 50044,
/* 2055 */ -2053,
/* 2056 */ 644,
/* 2057 */ 0,
/* 2058 */ -2053,
/* 2059 */ 1148,
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
/* 2074 */ 645,
/* 2075 */ 0,
/* 2076 */ -2071,
/* 2077 */ 1149,
/* 2078 */ 2082,
/* 2079 */ 50044,
/* 2080 */ -2078,
/* 2081 */ 646,
/* 2082 */ 0,
/* 2083 */ -2078,
/* 2084 */ 1150,
/* 2085 */ 0,
/* 2086 */ 50348,
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
/* 2099 */ 647,
/* 2100 */ 0,
/* 2101 */ -2095,
/* 2102 */ 1151,
/* 2103 */ 2110,
/* 2104 */ 50326,
/* 2105 */ 2176,
/* 2106 */ 50257,
/* 2107 */ 50059,
/* 2108 */ -2103,
/* 2109 */ 153,
/* 2110 */ 2117,
/* 2111 */ 50383,
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
/* 2122 */ 50389,
/* 2123 */ 2190,
/* 2124 */ 223,
/* 2125 */ 50037,
/* 2126 */ 50059,
/* 2127 */ -2103,
/* 2128 */ 156,
/* 2129 */ 2136,
/* 2130 */ 50365,
/* 2131 */ 2197,
/* 2132 */ 223,
/* 2133 */ 50059,
/* 2134 */ -2103,
/* 2135 */ 157,
/* 2136 */ 2143,
/* 2137 */ 50350,
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
/* 2151 */ 50472,
/* 2152 */ 2246,
/* 2153 */ 173,
/* 2154 */ 50059,
/* 2155 */ -2103,
/* 2156 */ 160,
/* 2157 */ 2164,
/* 2158 */ 50344,
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
/* 2173 */ 6102,
/* 2174 */ -2103,
/* 2175 */ 164,
/* 2176 */ 2180,
/* 2177 */ 50058,
/* 2178 */ -2176,
/* 2179 */ 648,
/* 2180 */ 0,
/* 2181 */ -2176,
/* 2182 */ 1152,
/* 2183 */ 2187,
/* 2184 */ 50058,
/* 2185 */ -2183,
/* 2186 */ 649,
/* 2187 */ 0,
/* 2188 */ -2183,
/* 2189 */ 1153,
/* 2190 */ 2194,
/* 2191 */ 50058,
/* 2192 */ -2190,
/* 2193 */ 650,
/* 2194 */ 0,
/* 2195 */ -2190,
/* 2196 */ 1154,
/* 2197 */ 2201,
/* 2198 */ 50058,
/* 2199 */ -2197,
/* 2200 */ 651,
/* 2201 */ 0,
/* 2202 */ -2197,
/* 2203 */ 1155,
/* 2204 */ 2208,
/* 2205 */ 50058,
/* 2206 */ -2204,
/* 2207 */ 652,
/* 2208 */ 0,
/* 2209 */ -2204,
/* 2210 */ 1156,
/* 2211 */ 2217,
/* 2212 */ 223,
/* 2213 */ 2220,
/* 2214 */ 2211,
/* 2215 */ -2211,
/* 2216 */ 655,
/* 2217 */ 0,
/* 2218 */ -2211,
/* 2219 */ 1158,
/* 2220 */ 2224,
/* 2221 */ 50124,
/* 2222 */ -2220,
/* 2223 */ 653,
/* 2224 */ 2228,
/* 2225 */ 50044,
/* 2226 */ -2220,
/* 2227 */ 654,
/* 2228 */ 0,
/* 2229 */ -2220,
/* 2230 */ 1157,
/* 2231 */ 2235,
/* 2232 */ 50448,
/* 2233 */ -2231,
/* 2234 */ 656,
/* 2235 */ 0,
/* 2236 */ 50334,
/* 2237 */ -2231,
/* 2238 */ 657,
/* 2239 */ 2243,
/* 2240 */ 50058,
/* 2241 */ -2239,
/* 2242 */ 658,
/* 2243 */ 0,
/* 2244 */ -2239,
/* 2245 */ 1159,
/* 2246 */ 2250,
/* 2247 */ 50058,
/* 2248 */ -2246,
/* 2249 */ 659,
/* 2250 */ 0,
/* 2251 */ -2246,
/* 2252 */ 1160,
/* 2253 */ 2257,
/* 2254 */ 50058,
/* 2255 */ -2253,
/* 2256 */ 660,
/* 2257 */ 0,
/* 2258 */ -2253,
/* 2259 */ 1161,
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
/* 2277 */ 662,
/* 2278 */ 0,
/* 2279 */ -2272,
/* 2280 */ 1163,
/* 2281 */ 2285,
/* 2282 */ 50047,
/* 2283 */ -2281,
/* 2284 */ 661,
/* 2285 */ 0,
/* 2286 */ -2281,
/* 2287 */ 1162,
/* 2288 */ 2292,
/* 2289 */ 50436,
/* 2290 */ -2288,
/* 2291 */ 663,
/* 2292 */ 2296,
/* 2293 */ 50334,
/* 2294 */ -2288,
/* 2295 */ 664,
/* 2296 */ 2300,
/* 2297 */ 50409,
/* 2298 */ -2288,
/* 2299 */ 665,
/* 2300 */ 0,
/* 2301 */ 50330,
/* 2302 */ -2288,
/* 2303 */ 666,
/* 2304 */ 2309,
/* 2305 */ 2332,
/* 2306 */ 2304,
/* 2307 */ -2304,
/* 2308 */ 667,
/* 2309 */ 0,
/* 2310 */ -2304,
/* 2311 */ 1164,
/* 2312 */ 2321,
/* 2313 */ 50288,
/* 2314 */ 223,
/* 2315 */ 50123,
/* 2316 */ 2324,
/* 2317 */ 50125,
/* 2318 */ 2312,
/* 2319 */ -2312,
/* 2320 */ 669,
/* 2321 */ 0,
/* 2322 */ -2312,
/* 2323 */ 1166,
/* 2324 */ 2329,
/* 2325 */ 2332,
/* 2326 */ 2324,
/* 2327 */ -2324,
/* 2328 */ 668,
/* 2329 */ 0,
/* 2330 */ -2324,
/* 2331 */ 1165,
/* 2332 */ 2339,
/* 2333 */ 50326,
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
/* 2345 */ 50374,
/* 2346 */ 2493,
/* 2347 */ 223,
/* 2348 */ 50059,
/* 2349 */ -2332,
/* 2350 */ 168,
/* 2351 */ 2358,
/* 2352 */ 50315,
/* 2353 */ 2500,
/* 2354 */ 223,
/* 2355 */ 50059,
/* 2356 */ -2332,
/* 2357 */ 169,
/* 2358 */ 2367,
/* 2359 */ 50298,
/* 2360 */ 2507,
/* 2361 */ 2514,
/* 2362 */ 223,
/* 2363 */ 2521,
/* 2364 */ 50059,
/* 2365 */ -2332,
/* 2366 */ 170,
/* 2367 */ 2375,
/* 2368 */ 50413,
/* 2369 */ 2528,
/* 2370 */ 173,
/* 2371 */ 2535,
/* 2372 */ 50059,
/* 2373 */ -2332,
/* 2374 */ 171,
/* 2375 */ 2383,
/* 2376 */ 50445,
/* 2377 */ 2544,
/* 2378 */ 173,
/* 2379 */ 2551,
/* 2380 */ 50059,
/* 2381 */ -2332,
/* 2382 */ 172,
/* 2383 */ 2390,
/* 2384 */ 50417,
/* 2385 */ 2558,
/* 2386 */ 173,
/* 2387 */ 50059,
/* 2388 */ -2332,
/* 2389 */ 173,
/* 2390 */ 2397,
/* 2391 */ 50472,
/* 2392 */ 2565,
/* 2393 */ 173,
/* 2394 */ 50059,
/* 2395 */ -2332,
/* 2396 */ 174,
/* 2397 */ 2404,
/* 2398 */ 50473,
/* 2399 */ 2572,
/* 2400 */ 173,
/* 2401 */ 50059,
/* 2402 */ -2332,
/* 2403 */ 175,
/* 2404 */ 2411,
/* 2405 */ 50333,
/* 2406 */ 2579,
/* 2407 */ 173,
/* 2408 */ 50059,
/* 2409 */ -2332,
/* 2410 */ 176,
/* 2411 */ 2418,
/* 2412 */ 50414,
/* 2413 */ 2586,
/* 2414 */ 259,
/* 2415 */ 50059,
/* 2416 */ -2332,
/* 2417 */ 177,
/* 2418 */ 2425,
/* 2419 */ 50297,
/* 2420 */ 2593,
/* 2421 */ 173,
/* 2422 */ 50059,
/* 2423 */ -2332,
/* 2424 */ 178,
/* 2425 */ 2432,
/* 2426 */ 50378,
/* 2427 */ 2600,
/* 2428 */ 173,
/* 2429 */ 50059,
/* 2430 */ -2332,
/* 2431 */ 179,
/* 2432 */ 2439,
/* 2433 */ 50461,
/* 2434 */ 2607,
/* 2435 */ 173,
/* 2436 */ 50059,
/* 2437 */ -2332,
/* 2438 */ 180,
/* 2439 */ 2446,
/* 2440 */ 50428,
/* 2441 */ 2614,
/* 2442 */ 163,
/* 2443 */ 50059,
/* 2444 */ -2332,
/* 2445 */ 181,
/* 2446 */ 2453,
/* 2447 */ 50428,
/* 2448 */ 2621,
/* 2449 */ 173,
/* 2450 */ 50059,
/* 2451 */ -2332,
/* 2452 */ 182,
/* 2453 */ 2460,
/* 2454 */ 50318,
/* 2455 */ 2628,
/* 2456 */ 173,
/* 2457 */ 50059,
/* 2458 */ -2332,
/* 2459 */ 183,
/* 2460 */ 2467,
/* 2461 */ 50318,
/* 2462 */ 2635,
/* 2463 */ 50275,
/* 2464 */ 50059,
/* 2465 */ -2332,
/* 2466 */ 184,
/* 2467 */ 2474,
/* 2468 */ 50350,
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
/* 2479 */ 6102,
/* 2480 */ -2332,
/* 2481 */ 187,
/* 2482 */ 0,
/* 2483 */ 3423,
/* 2484 */ -2332,
/* 2485 */ 188,
/* 2486 */ 2490,
/* 2487 */ 50058,
/* 2488 */ -2486,
/* 2489 */ 670,
/* 2490 */ 0,
/* 2491 */ -2486,
/* 2492 */ 1167,
/* 2493 */ 2497,
/* 2494 */ 50058,
/* 2495 */ -2493,
/* 2496 */ 671,
/* 2497 */ 0,
/* 2498 */ -2493,
/* 2499 */ 1168,
/* 2500 */ 2504,
/* 2501 */ 50058,
/* 2502 */ -2500,
/* 2503 */ 672,
/* 2504 */ 0,
/* 2505 */ -2500,
/* 2506 */ 1169,
/* 2507 */ 2511,
/* 2508 */ 50308,
/* 2509 */ -2507,
/* 2510 */ 673,
/* 2511 */ 0,
/* 2512 */ -2507,
/* 2513 */ 1170,
/* 2514 */ 2518,
/* 2515 */ 50058,
/* 2516 */ -2514,
/* 2517 */ 674,
/* 2518 */ 0,
/* 2519 */ -2514,
/* 2520 */ 1171,
/* 2521 */ 2525,
/* 2522 */ 50037,
/* 2523 */ -2521,
/* 2524 */ 675,
/* 2525 */ 0,
/* 2526 */ -2521,
/* 2527 */ 1172,
/* 2528 */ 2532,
/* 2529 */ 50058,
/* 2530 */ -2528,
/* 2531 */ 676,
/* 2532 */ 0,
/* 2533 */ -2528,
/* 2534 */ 1173,
/* 2535 */ 2541,
/* 2536 */ 50040,
/* 2537 */ 173,
/* 2538 */ 50041,
/* 2539 */ -2535,
/* 2540 */ 677,
/* 2541 */ 0,
/* 2542 */ -2535,
/* 2543 */ 1174,
/* 2544 */ 2548,
/* 2545 */ 50058,
/* 2546 */ -2544,
/* 2547 */ 678,
/* 2548 */ 0,
/* 2549 */ -2544,
/* 2550 */ 1175,
/* 2551 */ 2555,
/* 2552 */ 50405,
/* 2553 */ -2551,
/* 2554 */ 679,
/* 2555 */ 0,
/* 2556 */ -2551,
/* 2557 */ 1176,
/* 2558 */ 2562,
/* 2559 */ 50058,
/* 2560 */ -2558,
/* 2561 */ 680,
/* 2562 */ 0,
/* 2563 */ -2558,
/* 2564 */ 1177,
/* 2565 */ 2569,
/* 2566 */ 50058,
/* 2567 */ -2565,
/* 2568 */ 681,
/* 2569 */ 0,
/* 2570 */ -2565,
/* 2571 */ 1178,
/* 2572 */ 2576,
/* 2573 */ 50058,
/* 2574 */ -2572,
/* 2575 */ 682,
/* 2576 */ 0,
/* 2577 */ -2572,
/* 2578 */ 1179,
/* 2579 */ 2583,
/* 2580 */ 50058,
/* 2581 */ -2579,
/* 2582 */ 683,
/* 2583 */ 0,
/* 2584 */ -2579,
/* 2585 */ 1180,
/* 2586 */ 2590,
/* 2587 */ 50058,
/* 2588 */ -2586,
/* 2589 */ 684,
/* 2590 */ 0,
/* 2591 */ -2586,
/* 2592 */ 1181,
/* 2593 */ 2597,
/* 2594 */ 50058,
/* 2595 */ -2593,
/* 2596 */ 685,
/* 2597 */ 0,
/* 2598 */ -2593,
/* 2599 */ 1182,
/* 2600 */ 2604,
/* 2601 */ 50058,
/* 2602 */ -2600,
/* 2603 */ 686,
/* 2604 */ 0,
/* 2605 */ -2600,
/* 2606 */ 1183,
/* 2607 */ 2611,
/* 2608 */ 50058,
/* 2609 */ -2607,
/* 2610 */ 687,
/* 2611 */ 0,
/* 2612 */ -2607,
/* 2613 */ 1184,
/* 2614 */ 2618,
/* 2615 */ 50058,
/* 2616 */ -2614,
/* 2617 */ 688,
/* 2618 */ 0,
/* 2619 */ -2614,
/* 2620 */ 1185,
/* 2621 */ 2625,
/* 2622 */ 50058,
/* 2623 */ -2621,
/* 2624 */ 689,
/* 2625 */ 0,
/* 2626 */ -2621,
/* 2627 */ 1186,
/* 2628 */ 2632,
/* 2629 */ 50058,
/* 2630 */ -2628,
/* 2631 */ 690,
/* 2632 */ 0,
/* 2633 */ -2628,
/* 2634 */ 1187,
/* 2635 */ 2639,
/* 2636 */ 50058,
/* 2637 */ -2635,
/* 2638 */ 691,
/* 2639 */ 0,
/* 2640 */ -2635,
/* 2641 */ 1188,
/* 2642 */ 2646,
/* 2643 */ 50058,
/* 2644 */ -2642,
/* 2645 */ 692,
/* 2646 */ 0,
/* 2647 */ -2642,
/* 2648 */ 1189,
/* 2649 */ 2655,
/* 2650 */ 223,
/* 2651 */ 2658,
/* 2652 */ 2649,
/* 2653 */ -2649,
/* 2654 */ 695,
/* 2655 */ 0,
/* 2656 */ -2649,
/* 2657 */ 1191,
/* 2658 */ 2662,
/* 2659 */ 50044,
/* 2660 */ -2658,
/* 2661 */ 693,
/* 2662 */ 2666,
/* 2663 */ 50124,
/* 2664 */ -2658,
/* 2665 */ 694,
/* 2666 */ 0,
/* 2667 */ -2658,
/* 2668 */ 1190,
/* 2669 */ 0,
/* 2670 */ 2673,
/* 2671 */ -2669,
/* 2672 */ 189,
/* 2673 */ 2679,
/* 2674 */ 223,
/* 2675 */ 2682,
/* 2676 */ 2673,
/* 2677 */ -2673,
/* 2678 */ 698,
/* 2679 */ 0,
/* 2680 */ -2673,
/* 2681 */ 1193,
/* 2682 */ 2686,
/* 2683 */ 50044,
/* 2684 */ -2682,
/* 2685 */ 696,
/* 2686 */ 2690,
/* 2687 */ 50124,
/* 2688 */ -2682,
/* 2689 */ 697,
/* 2690 */ 0,
/* 2691 */ -2682,
/* 2692 */ 1192,
/* 2693 */ 0,
/* 2694 */ 50261,
/* 2695 */ -2693,
/* 2696 */ 190,
/* 2697 */ 0,
/* 2698 */ 50418,
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
/* 2710 */ 699,
/* 2711 */ 0,
/* 2712 */ -2706,
/* 2713 */ 1194,
/* 2714 */ 2719,
/* 2715 */ 2722,
/* 2716 */ 2714,
/* 2717 */ -2714,
/* 2718 */ 700,
/* 2719 */ 0,
/* 2720 */ -2714,
/* 2721 */ 1195,
/* 2722 */ 2729,
/* 2723 */ 50326,
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
/* 2735 */ 50359,
/* 2736 */ 2839,
/* 2737 */ 2846,
/* 2738 */ 50059,
/* 2739 */ -2722,
/* 2740 */ 194,
/* 2741 */ 2748,
/* 2742 */ 50388,
/* 2743 */ 2862,
/* 2744 */ 163,
/* 2745 */ 50059,
/* 2746 */ -2722,
/* 2747 */ 195,
/* 2748 */ 2758,
/* 2749 */ 50340,
/* 2750 */ 2869,
/* 2751 */ 2876,
/* 2752 */ 163,
/* 2753 */ 50044,
/* 2754 */ 163,
/* 2755 */ 50059,
/* 2756 */ -2722,
/* 2757 */ 196,
/* 2758 */ 2770,
/* 2759 */ 50340,
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
/* 2771 */ 50432,
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
/* 2785 */ 50350,
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
/* 2804 */ 6102,
/* 2805 */ -2722,
/* 2806 */ 204,
/* 2807 */ 2811,
/* 2808 */ 50058,
/* 2809 */ -2807,
/* 2810 */ 701,
/* 2811 */ 0,
/* 2812 */ -2807,
/* 2813 */ 1196,
/* 2814 */ 2822,
/* 2815 */ 50277,
/* 2816 */ 2825,
/* 2817 */ 173,
/* 2818 */ 2832,
/* 2819 */ 2814,
/* 2820 */ -2814,
/* 2821 */ 704,
/* 2822 */ 0,
/* 2823 */ -2814,
/* 2824 */ 1199,
/* 2825 */ 2829,
/* 2826 */ 50058,
/* 2827 */ -2825,
/* 2828 */ 702,
/* 2829 */ 0,
/* 2830 */ -2825,
/* 2831 */ 1197,
/* 2832 */ 2836,
/* 2833 */ 50044,
/* 2834 */ -2832,
/* 2835 */ 703,
/* 2836 */ 0,
/* 2837 */ -2832,
/* 2838 */ 1198,
/* 2839 */ 2843,
/* 2840 */ 50058,
/* 2841 */ -2839,
/* 2842 */ 705,
/* 2843 */ 0,
/* 2844 */ -2839,
/* 2845 */ 1200,
/* 2846 */ 2852,
/* 2847 */ 3257,
/* 2848 */ 2855,
/* 2849 */ 2846,
/* 2850 */ -2846,
/* 2851 */ 707,
/* 2852 */ 0,
/* 2853 */ -2846,
/* 2854 */ 1202,
/* 2855 */ 2859,
/* 2856 */ 50044,
/* 2857 */ -2855,
/* 2858 */ 706,
/* 2859 */ 0,
/* 2860 */ -2855,
/* 2861 */ 1201,
/* 2862 */ 2866,
/* 2863 */ 50058,
/* 2864 */ -2862,
/* 2865 */ 708,
/* 2866 */ 0,
/* 2867 */ -2862,
/* 2868 */ 1203,
/* 2869 */ 2873,
/* 2870 */ 50309,
/* 2871 */ -2869,
/* 2872 */ 709,
/* 2873 */ 0,
/* 2874 */ -2869,
/* 2875 */ 1204,
/* 2876 */ 2880,
/* 2877 */ 50058,
/* 2878 */ -2876,
/* 2879 */ 710,
/* 2880 */ 0,
/* 2881 */ -2876,
/* 2882 */ 1205,
/* 2883 */ 2887,
/* 2884 */ 50309,
/* 2885 */ -2883,
/* 2886 */ 711,
/* 2887 */ 0,
/* 2888 */ -2883,
/* 2889 */ 1206,
/* 2890 */ 2894,
/* 2891 */ 50058,
/* 2892 */ -2890,
/* 2893 */ 712,
/* 2894 */ 0,
/* 2895 */ -2890,
/* 2896 */ 1207,
/* 2897 */ 2901,
/* 2898 */ 50058,
/* 2899 */ -2897,
/* 2900 */ 713,
/* 2901 */ 0,
/* 2902 */ -2897,
/* 2903 */ 1208,
/* 2904 */ 2910,
/* 2905 */ 223,
/* 2906 */ 2913,
/* 2907 */ 2904,
/* 2908 */ -2904,
/* 2909 */ 715,
/* 2910 */ 0,
/* 2911 */ -2904,
/* 2912 */ 1210,
/* 2913 */ 2917,
/* 2914 */ 50044,
/* 2915 */ -2913,
/* 2916 */ 714,
/* 2917 */ 0,
/* 2918 */ -2913,
/* 2919 */ 1209,
/* 2920 */ 2924,
/* 2921 */ 50058,
/* 2922 */ -2920,
/* 2923 */ 716,
/* 2924 */ 0,
/* 2925 */ -2920,
/* 2926 */ 1211,
/* 2927 */ 2931,
/* 2928 */ 50058,
/* 2929 */ -2927,
/* 2930 */ 717,
/* 2931 */ 0,
/* 2932 */ -2927,
/* 2933 */ 1212,
/* 2934 */ 2940,
/* 2935 */ 223,
/* 2936 */ 2943,
/* 2937 */ 2934,
/* 2938 */ -2934,
/* 2939 */ 720,
/* 2940 */ 0,
/* 2941 */ -2934,
/* 2942 */ 1214,
/* 2943 */ 2947,
/* 2944 */ 50124,
/* 2945 */ -2943,
/* 2946 */ 718,
/* 2947 */ 2951,
/* 2948 */ 50044,
/* 2949 */ -2943,
/* 2950 */ 719,
/* 2951 */ 0,
/* 2952 */ -2943,
/* 2953 */ 1213,
/* 2954 */ 0,
/* 2955 */ 50309,
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
/* 2966 */ 721,
/* 2967 */ 0,
/* 2968 */ -2962,
/* 2969 */ 1215,
/* 2970 */ 2977,
/* 2971 */ 50326,
/* 2972 */ 3063,
/* 2973 */ 50257,
/* 2974 */ 50059,
/* 2975 */ -2970,
/* 2976 */ 206,
/* 2977 */ 2985,
/* 2978 */ 50364,
/* 2979 */ 3070,
/* 2980 */ 3077,
/* 2981 */ 223,
/* 2982 */ 50059,
/* 2983 */ -2970,
/* 2984 */ 207,
/* 2985 */ 2993,
/* 2986 */ 50381,
/* 2987 */ 3084,
/* 2988 */ 3091,
/* 2989 */ 223,
/* 2990 */ 50059,
/* 2991 */ -2970,
/* 2992 */ 208,
/* 2993 */ 3002,
/* 2994 */ 50323,
/* 2995 */ 3098,
/* 2996 */ 223,
/* 2997 */ 50047,
/* 2998 */ 223,
/* 2999 */ 50059,
/* 3000 */ -2970,
/* 3001 */ 209,
/* 3002 */ 3009,
/* 3003 */ 50296,
/* 3004 */ 3105,
/* 3005 */ 3112,
/* 3006 */ 50059,
/* 3007 */ -2970,
/* 3008 */ 210,
/* 3009 */ 3016,
/* 3010 */ 50429,
/* 3011 */ 3129,
/* 3012 */ 3136,
/* 3013 */ 50059,
/* 3014 */ -2970,
/* 3015 */ 211,
/* 3016 */ 3023,
/* 3017 */ 50359,
/* 3018 */ 3153,
/* 3019 */ 3160,
/* 3020 */ 50059,
/* 3021 */ -2970,
/* 3022 */ 212,
/* 3023 */ 3033,
/* 3024 */ 50432,
/* 3025 */ 3176,
/* 3026 */ 223,
/* 3027 */ 3180,
/* 3028 */ 3184,
/* 3029 */ 3191,
/* 3030 */ 50059,
/* 3031 */ -2970,
/* 3032 */ 213,
/* 3033 */ 3040,
/* 3034 */ 50411,
/* 3035 */ 3207,
/* 3036 */ 3214,
/* 3037 */ 50059,
/* 3038 */ -2970,
/* 3039 */ 214,
/* 3040 */ 3047,
/* 3041 */ 50350,
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
/* 3060 */ 6102,
/* 3061 */ -2970,
/* 3062 */ 219,
/* 3063 */ 3067,
/* 3064 */ 50058,
/* 3065 */ -3063,
/* 3066 */ 722,
/* 3067 */ 0,
/* 3068 */ -3063,
/* 3069 */ 1216,
/* 3070 */ 3074,
/* 3071 */ 50058,
/* 3072 */ -3070,
/* 3073 */ 723,
/* 3074 */ 0,
/* 3075 */ -3070,
/* 3076 */ 1217,
/* 3077 */ 3081,
/* 3078 */ 50100,
/* 3079 */ -3077,
/* 3080 */ 724,
/* 3081 */ 0,
/* 3082 */ -3077,
/* 3083 */ 1218,
/* 3084 */ 3088,
/* 3085 */ 50058,
/* 3086 */ -3084,
/* 3087 */ 725,
/* 3088 */ 0,
/* 3089 */ -3084,
/* 3090 */ 1219,
/* 3091 */ 3095,
/* 3092 */ 50100,
/* 3093 */ -3091,
/* 3094 */ 726,
/* 3095 */ 0,
/* 3096 */ -3091,
/* 3097 */ 1220,
/* 3098 */ 3102,
/* 3099 */ 50058,
/* 3100 */ -3098,
/* 3101 */ 727,
/* 3102 */ 0,
/* 3103 */ -3098,
/* 3104 */ 1221,
/* 3105 */ 3109,
/* 3106 */ 50058,
/* 3107 */ -3105,
/* 3108 */ 728,
/* 3109 */ 0,
/* 3110 */ -3105,
/* 3111 */ 1222,
/* 3112 */ 3119,
/* 3113 */ 223,
/* 3114 */ 223,
/* 3115 */ 3122,
/* 3116 */ 3112,
/* 3117 */ -3112,
/* 3118 */ 730,
/* 3119 */ 0,
/* 3120 */ -3112,
/* 3121 */ 1224,
/* 3122 */ 3126,
/* 3123 */ 50044,
/* 3124 */ -3122,
/* 3125 */ 729,
/* 3126 */ 0,
/* 3127 */ -3122,
/* 3128 */ 1223,
/* 3129 */ 3133,
/* 3130 */ 50058,
/* 3131 */ -3129,
/* 3132 */ 731,
/* 3133 */ 0,
/* 3134 */ -3129,
/* 3135 */ 1225,
/* 3136 */ 3143,
/* 3137 */ 223,
/* 3138 */ 223,
/* 3139 */ 3146,
/* 3140 */ 3136,
/* 3141 */ -3136,
/* 3142 */ 733,
/* 3143 */ 0,
/* 3144 */ -3136,
/* 3145 */ 1227,
/* 3146 */ 3150,
/* 3147 */ 50044,
/* 3148 */ -3146,
/* 3149 */ 732,
/* 3150 */ 0,
/* 3151 */ -3146,
/* 3152 */ 1226,
/* 3153 */ 3157,
/* 3154 */ 50058,
/* 3155 */ -3153,
/* 3156 */ 734,
/* 3157 */ 0,
/* 3158 */ -3153,
/* 3159 */ 1228,
/* 3160 */ 3166,
/* 3161 */ 3257,
/* 3162 */ 3169,
/* 3163 */ 3160,
/* 3164 */ -3160,
/* 3165 */ 736,
/* 3166 */ 0,
/* 3167 */ -3160,
/* 3168 */ 1230,
/* 3169 */ 3173,
/* 3170 */ 50044,
/* 3171 */ -3169,
/* 3172 */ 735,
/* 3173 */ 0,
/* 3174 */ -3169,
/* 3175 */ 1229,
/* 3176 */ 0,
/* 3177 */ 50091,
/* 3178 */ -3176,
/* 3179 */ 737,
/* 3180 */ 0,
/* 3181 */ 50093,
/* 3182 */ -3180,
/* 3183 */ 738,
/* 3184 */ 3188,
/* 3185 */ 50058,
/* 3186 */ -3184,
/* 3187 */ 739,
/* 3188 */ 0,
/* 3189 */ -3184,
/* 3190 */ 1231,
/* 3191 */ 3197,
/* 3192 */ 223,
/* 3193 */ 3200,
/* 3194 */ 3191,
/* 3195 */ -3191,
/* 3196 */ 741,
/* 3197 */ 0,
/* 3198 */ -3191,
/* 3199 */ 1233,
/* 3200 */ 3204,
/* 3201 */ 50044,
/* 3202 */ -3200,
/* 3203 */ 740,
/* 3204 */ 0,
/* 3205 */ -3200,
/* 3206 */ 1232,
/* 3207 */ 3211,
/* 3208 */ 50058,
/* 3209 */ -3207,
/* 3210 */ 742,
/* 3211 */ 0,
/* 3212 */ -3207,
/* 3213 */ 1234,
/* 3214 */ 3220,
/* 3215 */ 223,
/* 3216 */ 3223,
/* 3217 */ 3214,
/* 3218 */ -3214,
/* 3219 */ 744,
/* 3220 */ 0,
/* 3221 */ -3214,
/* 3222 */ 1236,
/* 3223 */ 3227,
/* 3224 */ 50044,
/* 3225 */ -3223,
/* 3226 */ 743,
/* 3227 */ 0,
/* 3228 */ -3223,
/* 3229 */ 1235,
/* 3230 */ 3234,
/* 3231 */ 50058,
/* 3232 */ -3230,
/* 3233 */ 745,
/* 3234 */ 0,
/* 3235 */ -3230,
/* 3236 */ 1237,
/* 3237 */ 3243,
/* 3238 */ 223,
/* 3239 */ 3246,
/* 3240 */ 3237,
/* 3241 */ -3237,
/* 3242 */ 748,
/* 3243 */ 0,
/* 3244 */ -3237,
/* 3245 */ 1239,
/* 3246 */ 3250,
/* 3247 */ 50124,
/* 3248 */ -3246,
/* 3249 */ 746,
/* 3250 */ 3254,
/* 3251 */ 50044,
/* 3252 */ -3246,
/* 3253 */ 747,
/* 3254 */ 0,
/* 3255 */ -3246,
/* 3256 */ 1238,
/* 3257 */ 3267,
/* 3258 */ 50282,
/* 3259 */ 50091,
/* 3260 */ 223,
/* 3261 */ 3287,
/* 3262 */ 50093,
/* 3263 */ 50295,
/* 3264 */ 3325,
/* 3265 */ -3257,
/* 3266 */ 220,
/* 3267 */ 3277,
/* 3268 */ 50440,
/* 3269 */ 50091,
/* 3270 */ 223,
/* 3271 */ 3294,
/* 3272 */ 50093,
/* 3273 */ 50295,
/* 3274 */ 3325,
/* 3275 */ -3257,
/* 3276 */ 221,
/* 3277 */ 0,
/* 3278 */ 50347,
/* 3279 */ 50091,
/* 3280 */ 223,
/* 3281 */ 3301,
/* 3282 */ 50093,
/* 3283 */ 50295,
/* 3284 */ 3325,
/* 3285 */ -3257,
/* 3286 */ 222,
/* 3287 */ 3291,
/* 3288 */ 3308,
/* 3289 */ -3287,
/* 3290 */ 749,
/* 3291 */ 0,
/* 3292 */ -3287,
/* 3293 */ 1240,
/* 3294 */ 3298,
/* 3295 */ 3308,
/* 3296 */ -3294,
/* 3297 */ 750,
/* 3298 */ 0,
/* 3299 */ -3294,
/* 3300 */ 1241,
/* 3301 */ 3305,
/* 3302 */ 3308,
/* 3303 */ -3301,
/* 3304 */ 751,
/* 3305 */ 0,
/* 3306 */ -3301,
/* 3307 */ 1242,
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
/* 3331 */ 50343,
/* 3332 */ 223,
/* 3333 */ 50374,
/* 3334 */ -3325,
/* 3335 */ 227,
/* 3336 */ 3341,
/* 3337 */ 50343,
/* 3338 */ 50374,
/* 3339 */ -3325,
/* 3340 */ 228,
/* 3341 */ 3350,
/* 3342 */ 50343,
/* 3343 */ 50374,
/* 3344 */ 50439,
/* 3345 */ 50295,
/* 3346 */ 223,
/* 3347 */ 3367,
/* 3348 */ -3325,
/* 3349 */ 229,
/* 3350 */ 0,
/* 3351 */ 50343,
/* 3352 */ 223,
/* 3353 */ 50374,
/* 3354 */ 50439,
/* 3355 */ 50295,
/* 3356 */ 223,
/* 3357 */ 3374,
/* 3358 */ -3325,
/* 3359 */ 230,
/* 3360 */ 3364,
/* 3361 */ 50374,
/* 3362 */ -3360,
/* 3363 */ 752,
/* 3364 */ 0,
/* 3365 */ -3360,
/* 3366 */ 1243,
/* 3367 */ 3371,
/* 3368 */ 50374,
/* 3369 */ -3367,
/* 3370 */ 753,
/* 3371 */ 0,
/* 3372 */ -3367,
/* 3373 */ 1244,
/* 3374 */ 3378,
/* 3375 */ 50374,
/* 3376 */ -3374,
/* 3377 */ 754,
/* 3378 */ 0,
/* 3379 */ -3374,
/* 3380 */ 1245,
/* 3381 */ 0,
/* 3382 */ 50332,
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
/* 3393 */ 755,
/* 3394 */ 0,
/* 3395 */ -3389,
/* 3396 */ 1246,
/* 3397 */ 3404,
/* 3398 */ 50326,
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
/* 3413 */ 6102,
/* 3414 */ -3397,
/* 3415 */ 235,
/* 3416 */ 3420,
/* 3417 */ 50058,
/* 3418 */ -3416,
/* 3419 */ 756,
/* 3420 */ 0,
/* 3421 */ -3416,
/* 3422 */ 1247,
/* 3423 */ 3430,
/* 3424 */ 50311,
/* 3425 */ 3443,
/* 3426 */ 3450,
/* 3427 */ 50059,
/* 3428 */ -3423,
/* 3429 */ 236,
/* 3430 */ 3436,
/* 3431 */ 50377,
/* 3432 */ 3483,
/* 3433 */ 3490,
/* 3434 */ -3423,
/* 3435 */ 237,
/* 3436 */ 0,
/* 3437 */ 50434,
/* 3438 */ 3517,
/* 3439 */ 3524,
/* 3440 */ 50059,
/* 3441 */ -3423,
/* 3442 */ 238,
/* 3443 */ 3447,
/* 3444 */ 50058,
/* 3445 */ -3443,
/* 3446 */ 757,
/* 3447 */ 0,
/* 3448 */ -3443,
/* 3449 */ 1248,
/* 3450 */ 3458,
/* 3451 */ 50042,
/* 3452 */ 223,
/* 3453 */ 223,
/* 3454 */ 3469,
/* 3455 */ 3450,
/* 3456 */ -3450,
/* 3457 */ 759,
/* 3458 */ 3466,
/* 3459 */ 50042,
/* 3460 */ 223,
/* 3461 */ 163,
/* 3462 */ 3476,
/* 3463 */ 3450,
/* 3464 */ -3450,
/* 3465 */ 761,
/* 3466 */ 0,
/* 3467 */ -3450,
/* 3468 */ 1251,
/* 3469 */ 3473,
/* 3470 */ 50044,
/* 3471 */ -3469,
/* 3472 */ 758,
/* 3473 */ 0,
/* 3474 */ -3469,
/* 3475 */ 1249,
/* 3476 */ 3480,
/* 3477 */ 50044,
/* 3478 */ -3476,
/* 3479 */ 760,
/* 3480 */ 0,
/* 3481 */ -3476,
/* 3482 */ 1250,
/* 3483 */ 3487,
/* 3484 */ 50058,
/* 3485 */ -3483,
/* 3486 */ 762,
/* 3487 */ 0,
/* 3488 */ -3483,
/* 3489 */ 1252,
/* 3490 */ 3498,
/* 3491 */ 50042,
/* 3492 */ 237,
/* 3493 */ 3501,
/* 3494 */ 3509,
/* 3495 */ 3490,
/* 3496 */ -3490,
/* 3497 */ 766,
/* 3498 */ 0,
/* 3499 */ -3490,
/* 3500 */ 1254,
/* 3501 */ 3506,
/* 3502 */ 3571,
/* 3503 */ 3501,
/* 3504 */ -3501,
/* 3505 */ 763,
/* 3506 */ 0,
/* 3507 */ -3501,
/* 3508 */ 1253,
/* 3509 */ 3513,
/* 3510 */ 50044,
/* 3511 */ -3509,
/* 3512 */ 764,
/* 3513 */ 0,
/* 3514 */ 50059,
/* 3515 */ -3509,
/* 3516 */ 765,
/* 3517 */ 3521,
/* 3518 */ 50058,
/* 3519 */ -3517,
/* 3520 */ 767,
/* 3521 */ 0,
/* 3522 */ -3517,
/* 3523 */ 1255,
/* 3524 */ 3532,
/* 3525 */ 50042,
/* 3526 */ 3535,
/* 3527 */ 3543,
/* 3528 */ 3564,
/* 3529 */ 3524,
/* 3530 */ -3524,
/* 3531 */ 773,
/* 3532 */ 0,
/* 3533 */ -3524,
/* 3534 */ 1258,
/* 3535 */ 3540,
/* 3536 */ 237,
/* 3537 */ 50037,
/* 3538 */ -3535,
/* 3539 */ 768,
/* 3540 */ 0,
/* 3541 */ -3535,
/* 3542 */ 1256,
/* 3543 */ 3550,
/* 3544 */ 163,
/* 3545 */ 50295,
/* 3546 */ 50374,
/* 3547 */ 223,
/* 3548 */ -3543,
/* 3549 */ 769,
/* 3550 */ 3557,
/* 3551 */ 163,
/* 3552 */ 50295,
/* 3553 */ 223,
/* 3554 */ 50374,
/* 3555 */ -3543,
/* 3556 */ 770,
/* 3557 */ 0,
/* 3558 */ 163,
/* 3559 */ 50295,
/* 3560 */ 50374,
/* 3561 */ 286,
/* 3562 */ -3543,
/* 3563 */ 771,
/* 3564 */ 3568,
/* 3565 */ 50044,
/* 3566 */ -3564,
/* 3567 */ 772,
/* 3568 */ 0,
/* 3569 */ -3564,
/* 3570 */ 1257,
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
/* 3584 */ 50293,
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
/* 3595 */ 774,
/* 3596 */ 0,
/* 3597 */ -3591,
/* 3598 */ 1259,
/* 3599 */ 3606,
/* 3600 */ 50326,
/* 3601 */ 3675,
/* 3602 */ 50257,
/* 3603 */ 50059,
/* 3604 */ -3599,
/* 3605 */ 243,
/* 3606 */ 3613,
/* 3607 */ 50372,
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
/* 3619 */ 50424,
/* 3620 */ 3714,
/* 3621 */ 3721,
/* 3622 */ 50059,
/* 3623 */ -3599,
/* 3624 */ 246,
/* 3625 */ 3632,
/* 3626 */ 50444,
/* 3627 */ 3737,
/* 3628 */ 3744,
/* 3629 */ 50059,
/* 3630 */ -3599,
/* 3631 */ 247,
/* 3632 */ 3639,
/* 3633 */ 50415,
/* 3634 */ 3760,
/* 3635 */ 3767,
/* 3636 */ 50059,
/* 3637 */ -3599,
/* 3638 */ 248,
/* 3639 */ 3646,
/* 3640 */ 50338,
/* 3641 */ 50353,
/* 3642 */ 3783,
/* 3643 */ 50059,
/* 3644 */ -3599,
/* 3645 */ 249,
/* 3646 */ 3653,
/* 3647 */ 50470,
/* 3648 */ 50353,
/* 3649 */ 3796,
/* 3650 */ 50059,
/* 3651 */ -3599,
/* 3652 */ 250,
/* 3653 */ 3660,
/* 3654 */ 50366,
/* 3655 */ 50353,
/* 3656 */ 3809,
/* 3657 */ 50059,
/* 3658 */ -3599,
/* 3659 */ 251,
/* 3660 */ 3667,
/* 3661 */ 50371,
/* 3662 */ 50353,
/* 3663 */ 3822,
/* 3664 */ 50059,
/* 3665 */ -3599,
/* 3666 */ 252,
/* 3667 */ 3671,
/* 3668 */ 538,
/* 3669 */ -3599,
/* 3670 */ 253,
/* 3671 */ 0,
/* 3672 */ 6102,
/* 3673 */ -3599,
/* 3674 */ 254,
/* 3675 */ 3679,
/* 3676 */ 50058,
/* 3677 */ -3675,
/* 3678 */ 775,
/* 3679 */ 0,
/* 3680 */ -3675,
/* 3681 */ 1260,
/* 3682 */ 3686,
/* 3683 */ 50058,
/* 3684 */ -3682,
/* 3685 */ 776,
/* 3686 */ 0,
/* 3687 */ -3682,
/* 3688 */ 1261,
/* 3689 */ 3697,
/* 3690 */ 50277,
/* 3691 */ 3700,
/* 3692 */ 223,
/* 3693 */ 3707,
/* 3694 */ 3689,
/* 3695 */ -3689,
/* 3696 */ 779,
/* 3697 */ 0,
/* 3698 */ -3689,
/* 3699 */ 1264,
/* 3700 */ 3704,
/* 3701 */ 50058,
/* 3702 */ -3700,
/* 3703 */ 777,
/* 3704 */ 0,
/* 3705 */ -3700,
/* 3706 */ 1262,
/* 3707 */ 3711,
/* 3708 */ 50044,
/* 3709 */ -3707,
/* 3710 */ 778,
/* 3711 */ 0,
/* 3712 */ -3707,
/* 3713 */ 1263,
/* 3714 */ 3718,
/* 3715 */ 50058,
/* 3716 */ -3714,
/* 3717 */ 780,
/* 3718 */ 0,
/* 3719 */ -3714,
/* 3720 */ 1265,
/* 3721 */ 3727,
/* 3722 */ 223,
/* 3723 */ 3730,
/* 3724 */ 3721,
/* 3725 */ -3721,
/* 3726 */ 782,
/* 3727 */ 0,
/* 3728 */ -3721,
/* 3729 */ 1267,
/* 3730 */ 3734,
/* 3731 */ 50044,
/* 3732 */ -3730,
/* 3733 */ 781,
/* 3734 */ 0,
/* 3735 */ -3730,
/* 3736 */ 1266,
/* 3737 */ 3741,
/* 3738 */ 50058,
/* 3739 */ -3737,
/* 3740 */ 783,
/* 3741 */ 0,
/* 3742 */ -3737,
/* 3743 */ 1268,
/* 3744 */ 3750,
/* 3745 */ 50277,
/* 3746 */ 3753,
/* 3747 */ 3744,
/* 3748 */ -3744,
/* 3749 */ 785,
/* 3750 */ 0,
/* 3751 */ -3744,
/* 3752 */ 1270,
/* 3753 */ 3757,
/* 3754 */ 50044,
/* 3755 */ -3753,
/* 3756 */ 784,
/* 3757 */ 0,
/* 3758 */ -3753,
/* 3759 */ 1269,
/* 3760 */ 3764,
/* 3761 */ 50058,
/* 3762 */ -3760,
/* 3763 */ 786,
/* 3764 */ 0,
/* 3765 */ -3760,
/* 3766 */ 1271,
/* 3767 */ 3773,
/* 3768 */ 223,
/* 3769 */ 3776,
/* 3770 */ 3767,
/* 3771 */ -3767,
/* 3772 */ 788,
/* 3773 */ 0,
/* 3774 */ -3767,
/* 3775 */ 1273,
/* 3776 */ 3780,
/* 3777 */ 50044,
/* 3778 */ -3776,
/* 3779 */ 787,
/* 3780 */ 0,
/* 3781 */ -3776,
/* 3782 */ 1272,
/* 3783 */ 3789,
/* 3784 */ 3835,
/* 3785 */ 3792,
/* 3786 */ 3783,
/* 3787 */ -3783,
/* 3788 */ 790,
/* 3789 */ 0,
/* 3790 */ -3783,
/* 3791 */ 1274,
/* 3792 */ 0,
/* 3793 */ 50044,
/* 3794 */ -3792,
/* 3795 */ 789,
/* 3796 */ 3802,
/* 3797 */ 3835,
/* 3798 */ 3805,
/* 3799 */ 3796,
/* 3800 */ -3796,
/* 3801 */ 792,
/* 3802 */ 0,
/* 3803 */ -3796,
/* 3804 */ 1275,
/* 3805 */ 0,
/* 3806 */ 50044,
/* 3807 */ -3805,
/* 3808 */ 791,
/* 3809 */ 3815,
/* 3810 */ 3835,
/* 3811 */ 3818,
/* 3812 */ 3809,
/* 3813 */ -3809,
/* 3814 */ 794,
/* 3815 */ 0,
/* 3816 */ -3809,
/* 3817 */ 1276,
/* 3818 */ 0,
/* 3819 */ 50044,
/* 3820 */ -3818,
/* 3821 */ 793,
/* 3822 */ 3828,
/* 3823 */ 3835,
/* 3824 */ 3831,
/* 3825 */ 3822,
/* 3826 */ -3822,
/* 3827 */ 796,
/* 3828 */ 0,
/* 3829 */ -3822,
/* 3830 */ 1277,
/* 3831 */ 0,
/* 3832 */ 50044,
/* 3833 */ -3831,
/* 3834 */ 795,
/* 3835 */ 3844,
/* 3836 */ 50440,
/* 3837 */ 50091,
/* 3838 */ 223,
/* 3839 */ 50044,
/* 3840 */ 223,
/* 3841 */ 50093,
/* 3842 */ -3835,
/* 3843 */ 255,
/* 3844 */ 3851,
/* 3845 */ 50440,
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
/* 3858 */ 50343,
/* 3859 */ 259,
/* 3860 */ 50459,
/* 3861 */ -3835,
/* 3862 */ 258,
/* 3863 */ 3870,
/* 3864 */ 163,
/* 3865 */ 223,
/* 3866 */ 3895,
/* 3867 */ 50321,
/* 3868 */ -3835,
/* 3869 */ 259,
/* 3870 */ 3877,
/* 3871 */ 163,
/* 3872 */ 223,
/* 3873 */ 3903,
/* 3874 */ 50465,
/* 3875 */ -3835,
/* 3876 */ 260,
/* 3877 */ 3882,
/* 3878 */ 50370,
/* 3879 */ 163,
/* 3880 */ -3835,
/* 3881 */ 261,
/* 3882 */ 0,
/* 3883 */ 50347,
/* 3884 */ 223,
/* 3885 */ -3835,
/* 3886 */ 262,
/* 3887 */ 3892,
/* 3888 */ 50295,
/* 3889 */ 50279,
/* 3890 */ -3887,
/* 3891 */ 797,
/* 3892 */ 0,
/* 3893 */ -3887,
/* 3894 */ 1278,
/* 3895 */ 3899,
/* 3896 */ 50047,
/* 3897 */ -3895,
/* 3898 */ 798,
/* 3899 */ 0,
/* 3900 */ 50408,
/* 3901 */ -3895,
/* 3902 */ 799,
/* 3903 */ 3907,
/* 3904 */ 50047,
/* 3905 */ -3903,
/* 3906 */ 800,
/* 3907 */ 0,
/* 3908 */ 50408,
/* 3909 */ -3903,
/* 3910 */ 801,
/* 3911 */ 0,
/* 3912 */ 50328,
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
/* 3925 */ 802,
/* 3926 */ 0,
/* 3927 */ -3921,
/* 3928 */ 1279,
/* 3929 */ 3936,
/* 3930 */ 50437,
/* 3931 */ 3962,
/* 3932 */ 3969,
/* 3933 */ 50059,
/* 3934 */ -3929,
/* 3935 */ 264,
/* 3936 */ 3943,
/* 3937 */ 50359,
/* 3938 */ 3985,
/* 3939 */ 3992,
/* 3940 */ 50059,
/* 3941 */ -3929,
/* 3942 */ 265,
/* 3943 */ 3950,
/* 3944 */ 50326,
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
/* 3959 */ 6102,
/* 3960 */ -3929,
/* 3961 */ 269,
/* 3962 */ 3966,
/* 3963 */ 50058,
/* 3964 */ -3962,
/* 3965 */ 803,
/* 3966 */ 0,
/* 3967 */ -3962,
/* 3968 */ 1280,
/* 3969 */ 3975,
/* 3970 */ 163,
/* 3971 */ 3978,
/* 3972 */ 3969,
/* 3973 */ -3969,
/* 3974 */ 805,
/* 3975 */ 0,
/* 3976 */ -3969,
/* 3977 */ 1282,
/* 3978 */ 3982,
/* 3979 */ 50044,
/* 3980 */ -3978,
/* 3981 */ 804,
/* 3982 */ 0,
/* 3983 */ -3978,
/* 3984 */ 1281,
/* 3985 */ 3989,
/* 3986 */ 50058,
/* 3987 */ -3985,
/* 3988 */ 806,
/* 3989 */ 0,
/* 3990 */ -3985,
/* 3991 */ 1283,
/* 3992 */ 3998,
/* 3993 */ 3257,
/* 3994 */ 4001,
/* 3995 */ 3992,
/* 3996 */ -3992,
/* 3997 */ 808,
/* 3998 */ 0,
/* 3999 */ -3992,
/* 4000 */ 1285,
/* 4001 */ 4005,
/* 4002 */ 50044,
/* 4003 */ -4001,
/* 4004 */ 807,
/* 4005 */ 0,
/* 4006 */ -4001,
/* 4007 */ 1284,
/* 4008 */ 4012,
/* 4009 */ 50058,
/* 4010 */ -4008,
/* 4011 */ 809,
/* 4012 */ 0,
/* 4013 */ -4008,
/* 4014 */ 1286,
/* 4015 */ 0,
/* 4016 */ 50358,
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
/* 4027 */ 810,
/* 4028 */ 0,
/* 4029 */ -4023,
/* 4030 */ 1287,
/* 4031 */ 4038,
/* 4032 */ 50329,
/* 4033 */ 4076,
/* 4034 */ 4083,
/* 4035 */ 50059,
/* 4036 */ -4031,
/* 4037 */ 271,
/* 4038 */ 4043,
/* 4039 */ 50311,
/* 4040 */ 50059,
/* 4041 */ -4031,
/* 4042 */ 272,
/* 4043 */ 4050,
/* 4044 */ 50350,
/* 4045 */ 4099,
/* 4046 */ 4106,
/* 4047 */ 50059,
/* 4048 */ -4031,
/* 4049 */ 273,
/* 4050 */ 4057,
/* 4051 */ 50326,
/* 4052 */ 4126,
/* 4053 */ 50257,
/* 4054 */ 50059,
/* 4055 */ -4031,
/* 4056 */ 274,
/* 4057 */ 4064,
/* 4058 */ 50359,
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
/* 4073 */ 6102,
/* 4074 */ -4031,
/* 4075 */ 278,
/* 4076 */ 4080,
/* 4077 */ 50058,
/* 4078 */ -4076,
/* 4079 */ 811,
/* 4080 */ 0,
/* 4081 */ -4076,
/* 4082 */ 1288,
/* 4083 */ 4089,
/* 4084 */ 163,
/* 4085 */ 4092,
/* 4086 */ 4083,
/* 4087 */ -4083,
/* 4088 */ 813,
/* 4089 */ 0,
/* 4090 */ -4083,
/* 4091 */ 1290,
/* 4092 */ 4096,
/* 4093 */ 50044,
/* 4094 */ -4092,
/* 4095 */ 812,
/* 4096 */ 0,
/* 4097 */ -4092,
/* 4098 */ 1289,
/* 4099 */ 4103,
/* 4100 */ 50058,
/* 4101 */ -4099,
/* 4102 */ 814,
/* 4103 */ 0,
/* 4104 */ -4099,
/* 4105 */ 1291,
/* 4106 */ 4112,
/* 4107 */ 223,
/* 4108 */ 4115,
/* 4109 */ 4106,
/* 4110 */ -4106,
/* 4111 */ 817,
/* 4112 */ 0,
/* 4113 */ -4106,
/* 4114 */ 1293,
/* 4115 */ 4119,
/* 4116 */ 50124,
/* 4117 */ -4115,
/* 4118 */ 815,
/* 4119 */ 4123,
/* 4120 */ 50044,
/* 4121 */ -4115,
/* 4122 */ 816,
/* 4123 */ 0,
/* 4124 */ -4115,
/* 4125 */ 1292,
/* 4126 */ 4130,
/* 4127 */ 50058,
/* 4128 */ -4126,
/* 4129 */ 818,
/* 4130 */ 0,
/* 4131 */ -4126,
/* 4132 */ 1294,
/* 4133 */ 4137,
/* 4134 */ 50058,
/* 4135 */ -4133,
/* 4136 */ 819,
/* 4137 */ 0,
/* 4138 */ -4133,
/* 4139 */ 1295,
/* 4140 */ 4146,
/* 4141 */ 3257,
/* 4142 */ 4149,
/* 4143 */ 4140,
/* 4144 */ -4140,
/* 4145 */ 821,
/* 4146 */ 0,
/* 4147 */ -4140,
/* 4148 */ 1297,
/* 4149 */ 4153,
/* 4150 */ 50044,
/* 4151 */ -4149,
/* 4152 */ 820,
/* 4153 */ 0,
/* 4154 */ -4149,
/* 4155 */ 1296,
/* 4156 */ 0,
/* 4157 */ 50450,
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
/* 4168 */ 822,
/* 4169 */ 0,
/* 4170 */ -4164,
/* 4171 */ 1298,
/* 4172 */ 4179,
/* 4173 */ 50326,
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
/* 4184 */ 50389,
/* 4185 */ 4231,
/* 4186 */ 223,
/* 4187 */ 50037,
/* 4188 */ 50059,
/* 4189 */ -4172,
/* 4190 */ 282,
/* 4191 */ 4198,
/* 4192 */ 50407,
/* 4193 */ 4238,
/* 4194 */ 223,
/* 4195 */ 50059,
/* 4196 */ -4172,
/* 4197 */ 283,
/* 4198 */ 4205,
/* 4199 */ 50383,
/* 4200 */ 4245,
/* 4201 */ 223,
/* 4202 */ 50059,
/* 4203 */ -4172,
/* 4204 */ 284,
/* 4205 */ 4212,
/* 4206 */ 50350,
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
/* 4221 */ 6102,
/* 4222 */ -4172,
/* 4223 */ 288,
/* 4224 */ 4228,
/* 4225 */ 50058,
/* 4226 */ -4224,
/* 4227 */ 823,
/* 4228 */ 0,
/* 4229 */ -4224,
/* 4230 */ 1299,
/* 4231 */ 4235,
/* 4232 */ 50058,
/* 4233 */ -4231,
/* 4234 */ 824,
/* 4235 */ 0,
/* 4236 */ -4231,
/* 4237 */ 1300,
/* 4238 */ 4242,
/* 4239 */ 50058,
/* 4240 */ -4238,
/* 4241 */ 825,
/* 4242 */ 0,
/* 4243 */ -4238,
/* 4244 */ 1301,
/* 4245 */ 4249,
/* 4246 */ 50058,
/* 4247 */ -4245,
/* 4248 */ 826,
/* 4249 */ 0,
/* 4250 */ -4245,
/* 4251 */ 1302,
/* 4252 */ 4256,
/* 4253 */ 50058,
/* 4254 */ -4252,
/* 4255 */ 827,
/* 4256 */ 0,
/* 4257 */ -4252,
/* 4258 */ 1303,
/* 4259 */ 4265,
/* 4260 */ 223,
/* 4261 */ 4268,
/* 4262 */ 4259,
/* 4263 */ -4259,
/* 4264 */ 830,
/* 4265 */ 0,
/* 4266 */ -4259,
/* 4267 */ 1305,
/* 4268 */ 4272,
/* 4269 */ 50124,
/* 4270 */ -4268,
/* 4271 */ 828,
/* 4272 */ 4276,
/* 4273 */ 50044,
/* 4274 */ -4268,
/* 4275 */ 829,
/* 4276 */ 0,
/* 4277 */ -4268,
/* 4278 */ 1304,
/* 4279 */ 0,
/* 4280 */ 50422,
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
/* 4293 */ 831,
/* 4294 */ 0,
/* 4295 */ -4289,
/* 4296 */ 1306,
/* 4297 */ 4304,
/* 4298 */ 50326,
/* 4299 */ 4391,
/* 4300 */ 50257,
/* 4301 */ 50059,
/* 4302 */ -4297,
/* 4303 */ 290,
/* 4304 */ 4311,
/* 4305 */ 50464,
/* 4306 */ 4398,
/* 4307 */ 163,
/* 4308 */ 50059,
/* 4309 */ -4297,
/* 4310 */ 291,
/* 4311 */ 4318,
/* 4312 */ 50351,
/* 4313 */ 4405,
/* 4314 */ 163,
/* 4315 */ 50059,
/* 4316 */ -4297,
/* 4317 */ 292,
/* 4318 */ 4325,
/* 4319 */ 50327,
/* 4320 */ 4412,
/* 4321 */ 163,
/* 4322 */ 50059,
/* 4323 */ -4297,
/* 4324 */ 293,
/* 4325 */ 4332,
/* 4326 */ 50325,
/* 4327 */ 4419,
/* 4328 */ 173,
/* 4329 */ 50059,
/* 4330 */ -4297,
/* 4331 */ 294,
/* 4332 */ 4339,
/* 4333 */ 50431,
/* 4334 */ 4426,
/* 4335 */ 173,
/* 4336 */ 50059,
/* 4337 */ -4297,
/* 4338 */ 295,
/* 4339 */ 4346,
/* 4340 */ 50426,
/* 4341 */ 4433,
/* 4342 */ 4440,
/* 4343 */ 50059,
/* 4344 */ -4297,
/* 4345 */ 296,
/* 4346 */ 4353,
/* 4347 */ 50350,
/* 4348 */ 4460,
/* 4349 */ 4467,
/* 4350 */ 50059,
/* 4351 */ -4297,
/* 4352 */ 297,
/* 4353 */ 4360,
/* 4354 */ 50360,
/* 4355 */ 50058,
/* 4356 */ 50260,
/* 4357 */ 50059,
/* 4358 */ -4297,
/* 4359 */ 298,
/* 4360 */ 4367,
/* 4361 */ 50455,
/* 4362 */ 50058,
/* 4363 */ 4487,
/* 4364 */ 50059,
/* 4365 */ -4297,
/* 4366 */ 299,
/* 4367 */ 4373,
/* 4368 */ 50423,
/* 4369 */ 50058,
/* 4370 */ 4580,
/* 4371 */ -4297,
/* 4372 */ 300,
/* 4373 */ 4379,
/* 4374 */ 50398,
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
/* 4388 */ 6102,
/* 4389 */ -4297,
/* 4390 */ 304,
/* 4391 */ 4395,
/* 4392 */ 50058,
/* 4393 */ -4391,
/* 4394 */ 832,
/* 4395 */ 0,
/* 4396 */ -4391,
/* 4397 */ 1307,
/* 4398 */ 4402,
/* 4399 */ 50058,
/* 4400 */ -4398,
/* 4401 */ 833,
/* 4402 */ 0,
/* 4403 */ -4398,
/* 4404 */ 1308,
/* 4405 */ 4409,
/* 4406 */ 50058,
/* 4407 */ -4405,
/* 4408 */ 834,
/* 4409 */ 0,
/* 4410 */ -4405,
/* 4411 */ 1309,
/* 4412 */ 4416,
/* 4413 */ 50058,
/* 4414 */ -4412,
/* 4415 */ 835,
/* 4416 */ 0,
/* 4417 */ -4412,
/* 4418 */ 1310,
/* 4419 */ 4423,
/* 4420 */ 50058,
/* 4421 */ -4419,
/* 4422 */ 836,
/* 4423 */ 0,
/* 4424 */ -4419,
/* 4425 */ 1311,
/* 4426 */ 4430,
/* 4427 */ 50058,
/* 4428 */ -4426,
/* 4429 */ 837,
/* 4430 */ 0,
/* 4431 */ -4426,
/* 4432 */ 1312,
/* 4433 */ 4437,
/* 4434 */ 50058,
/* 4435 */ -4433,
/* 4436 */ 838,
/* 4437 */ 0,
/* 4438 */ -4433,
/* 4439 */ 1313,
/* 4440 */ 4446,
/* 4441 */ 223,
/* 4442 */ 4449,
/* 4443 */ 4440,
/* 4444 */ -4440,
/* 4445 */ 841,
/* 4446 */ 0,
/* 4447 */ -4440,
/* 4448 */ 1315,
/* 4449 */ 4453,
/* 4450 */ 50044,
/* 4451 */ -4449,
/* 4452 */ 839,
/* 4453 */ 4457,
/* 4454 */ 50124,
/* 4455 */ -4449,
/* 4456 */ 840,
/* 4457 */ 0,
/* 4458 */ -4449,
/* 4459 */ 1314,
/* 4460 */ 4464,
/* 4461 */ 50058,
/* 4462 */ -4460,
/* 4463 */ 842,
/* 4464 */ 0,
/* 4465 */ -4460,
/* 4466 */ 1316,
/* 4467 */ 4473,
/* 4468 */ 223,
/* 4469 */ 4476,
/* 4470 */ 4467,
/* 4471 */ -4467,
/* 4472 */ 845,
/* 4473 */ 0,
/* 4474 */ -4467,
/* 4475 */ 1318,
/* 4476 */ 4480,
/* 4477 */ 50124,
/* 4478 */ -4476,
/* 4479 */ 843,
/* 4480 */ 4484,
/* 4481 */ 50044,
/* 4482 */ -4476,
/* 4483 */ 844,
/* 4484 */ 0,
/* 4485 */ -4476,
/* 4486 */ 1317,
/* 4487 */ 4496,
/* 4488 */ 50259,
/* 4489 */ 50058,
/* 4490 */ 163,
/* 4491 */ 4499,
/* 4492 */ 4573,
/* 4493 */ 4487,
/* 4494 */ -4487,
/* 4495 */ 858,
/* 4496 */ 0,
/* 4497 */ -4487,
/* 4498 */ 1324,
/* 4499 */ 4506,
/* 4500 */ 50091,
/* 4501 */ 173,
/* 4502 */ 50093,
/* 4503 */ 4499,
/* 4504 */ -4499,
/* 4505 */ 846,
/* 4506 */ 4513,
/* 4507 */ 50471,
/* 4508 */ 4522,
/* 4509 */ 4552,
/* 4510 */ 4499,
/* 4511 */ -4499,
/* 4512 */ 855,
/* 4513 */ 4519,
/* 4514 */ 50294,
/* 4515 */ 509,
/* 4516 */ 4499,
/* 4517 */ -4499,
/* 4518 */ 856,
/* 4519 */ 0,
/* 4520 */ -4499,
/* 4521 */ 1322,
/* 4522 */ 4526,
/* 4523 */ 163,
/* 4524 */ -4522,
/* 4525 */ 847,
/* 4526 */ 4532,
/* 4527 */ 50419,
/* 4528 */ 50372,
/* 4529 */ 4538,
/* 4530 */ -4522,
/* 4531 */ 849,
/* 4532 */ 0,
/* 4533 */ 50419,
/* 4534 */ 50388,
/* 4535 */ 4545,
/* 4536 */ -4522,
/* 4537 */ 851,
/* 4538 */ 4542,
/* 4539 */ 173,
/* 4540 */ -4538,
/* 4541 */ 848,
/* 4542 */ 0,
/* 4543 */ -4538,
/* 4544 */ 1319,
/* 4545 */ 4549,
/* 4546 */ 173,
/* 4547 */ -4545,
/* 4548 */ 850,
/* 4549 */ 0,
/* 4550 */ -4545,
/* 4551 */ 1320,
/* 4552 */ 4557,
/* 4553 */ 50399,
/* 4554 */ 163,
/* 4555 */ -4552,
/* 4556 */ 852,
/* 4557 */ 4565,
/* 4558 */ 50399,
/* 4559 */ 50419,
/* 4560 */ 173,
/* 4561 */ 50457,
/* 4562 */ 173,
/* 4563 */ -4552,
/* 4564 */ 853,
/* 4565 */ 4570,
/* 4566 */ 50399,
/* 4567 */ 173,
/* 4568 */ -4552,
/* 4569 */ 854,
/* 4570 */ 0,
/* 4571 */ -4552,
/* 4572 */ 1321,
/* 4573 */ 4577,
/* 4574 */ 50044,
/* 4575 */ -4573,
/* 4576 */ 857,
/* 4577 */ 0,
/* 4578 */ -4573,
/* 4579 */ 1323,
/* 4580 */ 4585,
/* 4581 */ 4297,
/* 4582 */ 4580,
/* 4583 */ -4580,
/* 4584 */ 859,
/* 4585 */ 0,
/* 4586 */ -4580,
/* 4587 */ 1325,
/* 4588 */ 4593,
/* 4589 */ 2693,
/* 4590 */ 4588,
/* 4591 */ -4588,
/* 4592 */ 860,
/* 4593 */ 0,
/* 4594 */ -4588,
/* 4595 */ 1326,
/* 4596 */ 0,
/* 4597 */ 50449,
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
/* 4608 */ 50353,
/* 4609 */ 223,
/* 4610 */ -4607,
/* 4611 */ 861,
/* 4612 */ 0,
/* 4613 */ -4607,
/* 4614 */ 1327,
/* 4615 */ 4620,
/* 4616 */ 4623,
/* 4617 */ 4615,
/* 4618 */ -4615,
/* 4619 */ 862,
/* 4620 */ 0,
/* 4621 */ -4615,
/* 4622 */ 1328,
/* 4623 */ 4630,
/* 4624 */ 50326,
/* 4625 */ 4850,
/* 4626 */ 50257,
/* 4627 */ 50059,
/* 4628 */ -4623,
/* 4629 */ 306,
/* 4630 */ 4637,
/* 4631 */ 50353,
/* 4632 */ 4857,
/* 4633 */ 223,
/* 4634 */ 50059,
/* 4635 */ -4623,
/* 4636 */ 307,
/* 4637 */ 4644,
/* 4638 */ 50390,
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
/* 4656 */ 50364,
/* 4657 */ 4886,
/* 4658 */ 596,
/* 4659 */ 50059,
/* 4660 */ -4623,
/* 4661 */ 311,
/* 4662 */ 4669,
/* 4663 */ 50366,
/* 4664 */ 4893,
/* 4665 */ 596,
/* 4666 */ 50059,
/* 4667 */ -4623,
/* 4668 */ 312,
/* 4669 */ 4676,
/* 4670 */ 50323,
/* 4671 */ 4900,
/* 4672 */ 596,
/* 4673 */ 50059,
/* 4674 */ -4623,
/* 4675 */ 313,
/* 4676 */ 4683,
/* 4677 */ 50292,
/* 4678 */ 4907,
/* 4679 */ 596,
/* 4680 */ 50059,
/* 4681 */ -4623,
/* 4682 */ 314,
/* 4683 */ 4690,
/* 4684 */ 50319,
/* 4685 */ 4914,
/* 4686 */ 173,
/* 4687 */ 50059,
/* 4688 */ -4623,
/* 4689 */ 315,
/* 4690 */ 4697,
/* 4691 */ 50410,
/* 4692 */ 4921,
/* 4693 */ 596,
/* 4694 */ 50059,
/* 4695 */ -4623,
/* 4696 */ 316,
/* 4697 */ 4705,
/* 4698 */ 50306,
/* 4699 */ 50374,
/* 4700 */ 4928,
/* 4701 */ 596,
/* 4702 */ 50059,
/* 4703 */ -4623,
/* 4704 */ 317,
/* 4705 */ 4712,
/* 4706 */ 50431,
/* 4707 */ 4935,
/* 4708 */ 596,
/* 4709 */ 50059,
/* 4710 */ -4623,
/* 4711 */ 318,
/* 4712 */ 4719,
/* 4713 */ 50435,
/* 4714 */ 4942,
/* 4715 */ 596,
/* 4716 */ 50059,
/* 4717 */ -4623,
/* 4718 */ 319,
/* 4719 */ 4726,
/* 4720 */ 50389,
/* 4721 */ 4949,
/* 4722 */ 596,
/* 4723 */ 50059,
/* 4724 */ -4623,
/* 4725 */ 320,
/* 4726 */ 4733,
/* 4727 */ 50277,
/* 4728 */ 4956,
/* 4729 */ 596,
/* 4730 */ 4963,
/* 4731 */ -4623,
/* 4732 */ 321,
/* 4733 */ 4741,
/* 4734 */ 50296,
/* 4735 */ 4971,
/* 4736 */ 5193,
/* 4737 */ 4978,
/* 4738 */ 50059,
/* 4739 */ -4623,
/* 4740 */ 322,
/* 4741 */ 4749,
/* 4742 */ 50440,
/* 4743 */ 50091,
/* 4744 */ 223,
/* 4745 */ 50093,
/* 4746 */ 50059,
/* 4747 */ -4623,
/* 4748 */ 323,
/* 4749 */ 4759,
/* 4750 */ 50440,
/* 4751 */ 50091,
/* 4752 */ 223,
/* 4753 */ 50044,
/* 4754 */ 223,
/* 4755 */ 50093,
/* 4756 */ 50059,
/* 4757 */ -4623,
/* 4758 */ 324,
/* 4759 */ 4771,
/* 4760 */ 50440,
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
/* 4772 */ 50440,
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
/* 4786 */ 50424,
/* 4787 */ 4987,
/* 4788 */ 4994,
/* 4789 */ 50059,
/* 4790 */ -4623,
/* 4791 */ 327,
/* 4792 */ 4799,
/* 4793 */ 50369,
/* 4794 */ 5027,
/* 4795 */ 5034,
/* 4796 */ 50059,
/* 4797 */ -4623,
/* 4798 */ 328,
/* 4799 */ 4807,
/* 4800 */ 50346,
/* 4801 */ 5067,
/* 4802 */ 5074,
/* 4803 */ 223,
/* 4804 */ 50059,
/* 4805 */ -4623,
/* 4806 */ 329,
/* 4807 */ 4814,
/* 4808 */ 50437,
/* 4809 */ 5083,
/* 4810 */ 5090,
/* 4811 */ 50059,
/* 4812 */ -4623,
/* 4813 */ 330,
/* 4814 */ 4821,
/* 4815 */ 50330,
/* 4816 */ 5106,
/* 4817 */ 5113,
/* 4818 */ 50059,
/* 4819 */ -4623,
/* 4820 */ 331,
/* 4821 */ 4828,
/* 4822 */ 50409,
/* 4823 */ 5129,
/* 4824 */ 5136,
/* 4825 */ 50059,
/* 4826 */ -4623,
/* 4827 */ 332,
/* 4828 */ 4834,
/* 4829 */ 50350,
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
/* 4847 */ 6102,
/* 4848 */ -4623,
/* 4849 */ 337,
/* 4850 */ 4854,
/* 4851 */ 50058,
/* 4852 */ -4850,
/* 4853 */ 863,
/* 4854 */ 0,
/* 4855 */ -4850,
/* 4856 */ 1329,
/* 4857 */ 4861,
/* 4858 */ 50058,
/* 4859 */ -4857,
/* 4860 */ 864,
/* 4861 */ 0,
/* 4862 */ -4857,
/* 4863 */ 1330,
/* 4864 */ 4868,
/* 4865 */ 50058,
/* 4866 */ -4864,
/* 4867 */ 865,
/* 4868 */ 0,
/* 4869 */ -4864,
/* 4870 */ 1331,
/* 4871 */ 4875,
/* 4872 */ 50374,
/* 4873 */ -4871,
/* 4874 */ 866,
/* 4875 */ 0,
/* 4876 */ 50307,
/* 4877 */ -4871,
/* 4878 */ 867,
/* 4879 */ 4883,
/* 4880 */ 50058,
/* 4881 */ -4879,
/* 4882 */ 868,
/* 4883 */ 0,
/* 4884 */ -4879,
/* 4885 */ 1332,
/* 4886 */ 4890,
/* 4887 */ 50058,
/* 4888 */ -4886,
/* 4889 */ 869,
/* 4890 */ 0,
/* 4891 */ -4886,
/* 4892 */ 1333,
/* 4893 */ 4897,
/* 4894 */ 50058,
/* 4895 */ -4893,
/* 4896 */ 870,
/* 4897 */ 0,
/* 4898 */ -4893,
/* 4899 */ 1334,
/* 4900 */ 4904,
/* 4901 */ 50058,
/* 4902 */ -4900,
/* 4903 */ 871,
/* 4904 */ 0,
/* 4905 */ -4900,
/* 4906 */ 1335,
/* 4907 */ 4911,
/* 4908 */ 50058,
/* 4909 */ -4907,
/* 4910 */ 872,
/* 4911 */ 0,
/* 4912 */ -4907,
/* 4913 */ 1336,
/* 4914 */ 4918,
/* 4915 */ 50058,
/* 4916 */ -4914,
/* 4917 */ 873,
/* 4918 */ 0,
/* 4919 */ -4914,
/* 4920 */ 1337,
/* 4921 */ 4925,
/* 4922 */ 50058,
/* 4923 */ -4921,
/* 4924 */ 874,
/* 4925 */ 0,
/* 4926 */ -4921,
/* 4927 */ 1338,
/* 4928 */ 4932,
/* 4929 */ 50058,
/* 4930 */ -4928,
/* 4931 */ 875,
/* 4932 */ 0,
/* 4933 */ -4928,
/* 4934 */ 1339,
/* 4935 */ 4939,
/* 4936 */ 50058,
/* 4937 */ -4935,
/* 4938 */ 876,
/* 4939 */ 0,
/* 4940 */ -4935,
/* 4941 */ 1340,
/* 4942 */ 4946,
/* 4943 */ 50058,
/* 4944 */ -4942,
/* 4945 */ 877,
/* 4946 */ 0,
/* 4947 */ -4942,
/* 4948 */ 1341,
/* 4949 */ 4953,
/* 4950 */ 50058,
/* 4951 */ -4949,
/* 4952 */ 878,
/* 4953 */ 0,
/* 4954 */ -4949,
/* 4955 */ 1342,
/* 4956 */ 4960,
/* 4957 */ 50058,
/* 4958 */ -4956,
/* 4959 */ 879,
/* 4960 */ 0,
/* 4961 */ -4956,
/* 4962 */ 1343,
/* 4963 */ 4967,
/* 4964 */ 50044,
/* 4965 */ -4963,
/* 4966 */ 880,
/* 4967 */ 0,
/* 4968 */ 50059,
/* 4969 */ -4963,
/* 4970 */ 881,
/* 4971 */ 4975,
/* 4972 */ 50058,
/* 4973 */ -4971,
/* 4974 */ 882,
/* 4975 */ 0,
/* 4976 */ -4971,
/* 4977 */ 1344,
/* 4978 */ 4984,
/* 4979 */ 50044,
/* 4980 */ 5193,
/* 4981 */ 4978,
/* 4982 */ -4978,
/* 4983 */ 883,
/* 4984 */ 0,
/* 4985 */ -4978,
/* 4986 */ 1345,
/* 4987 */ 4991,
/* 4988 */ 50058,
/* 4989 */ -4987,
/* 4990 */ 884,
/* 4991 */ 0,
/* 4992 */ -4987,
/* 4993 */ 1346,
/* 4994 */ 5000,
/* 4995 */ 5003,
/* 4996 */ 5020,
/* 4997 */ 4994,
/* 4998 */ -4994,
/* 4999 */ 890,
/* 5000 */ 0,
/* 5001 */ -4994,
/* 5002 */ 1348,
/* 5003 */ 5007,
/* 5004 */ 223,
/* 5005 */ -5003,
/* 5006 */ 885,
/* 5007 */ 0,
/* 5008 */ 5012,
/* 5009 */ 223,
/* 5010 */ -5003,
/* 5011 */ 888,
/* 5012 */ 5016,
/* 5013 */ 50126,
/* 5014 */ -5012,
/* 5015 */ 886,
/* 5016 */ 0,
/* 5017 */ 50033,
/* 5018 */ -5012,
/* 5019 */ 887,
/* 5020 */ 5024,
/* 5021 */ 50044,
/* 5022 */ -5020,
/* 5023 */ 889,
/* 5024 */ 0,
/* 5025 */ -5020,
/* 5026 */ 1347,
/* 5027 */ 5031,
/* 5028 */ 50058,
/* 5029 */ -5027,
/* 5030 */ 891,
/* 5031 */ 0,
/* 5032 */ -5027,
/* 5033 */ 1349,
/* 5034 */ 5040,
/* 5035 */ 5043,
/* 5036 */ 5060,
/* 5037 */ 5034,
/* 5038 */ -5034,
/* 5039 */ 897,
/* 5040 */ 0,
/* 5041 */ -5034,
/* 5042 */ 1351,
/* 5043 */ 5047,
/* 5044 */ 223,
/* 5045 */ -5043,
/* 5046 */ 892,
/* 5047 */ 0,
/* 5048 */ 5052,
/* 5049 */ 223,
/* 5050 */ -5043,
/* 5051 */ 895,
/* 5052 */ 5056,
/* 5053 */ 50126,
/* 5054 */ -5052,
/* 5055 */ 893,
/* 5056 */ 0,
/* 5057 */ 50033,
/* 5058 */ -5052,
/* 5059 */ 894,
/* 5060 */ 5064,
/* 5061 */ 50044,
/* 5062 */ -5060,
/* 5063 */ 896,
/* 5064 */ 0,
/* 5065 */ -5060,
/* 5066 */ 1350,
/* 5067 */ 5071,
/* 5068 */ 50058,
/* 5069 */ -5067,
/* 5070 */ 898,
/* 5071 */ 0,
/* 5072 */ -5067,
/* 5073 */ 1352,
/* 5074 */ 5080,
/* 5075 */ 223,
/* 5076 */ 50044,
/* 5077 */ 5074,
/* 5078 */ -5074,
/* 5079 */ 899,
/* 5080 */ 0,
/* 5081 */ -5074,
/* 5082 */ 1353,
/* 5083 */ 5087,
/* 5084 */ 50058,
/* 5085 */ -5083,
/* 5086 */ 900,
/* 5087 */ 0,
/* 5088 */ -5083,
/* 5089 */ 1354,
/* 5090 */ 5096,
/* 5091 */ 163,
/* 5092 */ 5099,
/* 5093 */ 5090,
/* 5094 */ -5090,
/* 5095 */ 902,
/* 5096 */ 0,
/* 5097 */ -5090,
/* 5098 */ 1356,
/* 5099 */ 5103,
/* 5100 */ 50044,
/* 5101 */ -5099,
/* 5102 */ 901,
/* 5103 */ 0,
/* 5104 */ -5099,
/* 5105 */ 1355,
/* 5106 */ 5110,
/* 5107 */ 50058,
/* 5108 */ -5106,
/* 5109 */ 903,
/* 5110 */ 0,
/* 5111 */ -5106,
/* 5112 */ 1357,
/* 5113 */ 5119,
/* 5114 */ 163,
/* 5115 */ 5122,
/* 5116 */ 5113,
/* 5117 */ -5113,
/* 5118 */ 905,
/* 5119 */ 0,
/* 5120 */ -5113,
/* 5121 */ 1359,
/* 5122 */ 5126,
/* 5123 */ 50044,
/* 5124 */ -5122,
/* 5125 */ 904,
/* 5126 */ 0,
/* 5127 */ -5122,
/* 5128 */ 1358,
/* 5129 */ 5133,
/* 5130 */ 50058,
/* 5131 */ -5129,
/* 5132 */ 906,
/* 5133 */ 0,
/* 5134 */ -5129,
/* 5135 */ 1360,
/* 5136 */ 5142,
/* 5137 */ 163,
/* 5138 */ 5145,
/* 5139 */ 5136,
/* 5140 */ -5136,
/* 5141 */ 908,
/* 5142 */ 0,
/* 5143 */ -5136,
/* 5144 */ 1362,
/* 5145 */ 5149,
/* 5146 */ 50044,
/* 5147 */ -5145,
/* 5148 */ 907,
/* 5149 */ 0,
/* 5150 */ -5145,
/* 5151 */ 1361,
/* 5152 */ 5156,
/* 5153 */ 50058,
/* 5154 */ -5152,
/* 5155 */ 909,
/* 5156 */ 0,
/* 5157 */ -5152,
/* 5158 */ 1363,
/* 5159 */ 5165,
/* 5160 */ 5168,
/* 5161 */ 5185,
/* 5162 */ 5159,
/* 5163 */ -5159,
/* 5164 */ 916,
/* 5165 */ 0,
/* 5166 */ -5159,
/* 5167 */ 1364,
/* 5168 */ 5172,
/* 5169 */ 223,
/* 5170 */ -5168,
/* 5171 */ 910,
/* 5172 */ 0,
/* 5173 */ 5177,
/* 5174 */ 223,
/* 5175 */ -5168,
/* 5176 */ 913,
/* 5177 */ 5181,
/* 5178 */ 50126,
/* 5179 */ -5177,
/* 5180 */ 911,
/* 5181 */ 0,
/* 5182 */ 50033,
/* 5183 */ -5177,
/* 5184 */ 912,
/* 5185 */ 5189,
/* 5186 */ 50044,
/* 5187 */ -5185,
/* 5188 */ 914,
/* 5189 */ 0,
/* 5190 */ 50059,
/* 5191 */ -5185,
/* 5192 */ 915,
/* 5193 */ 5203,
/* 5194 */ 223,
/* 5195 */ 50353,
/* 5196 */ 5210,
/* 5197 */ 5222,
/* 5198 */ 5229,
/* 5199 */ 5236,
/* 5200 */ 5258,
/* 5201 */ -5193,
/* 5202 */ 338,
/* 5203 */ 0,
/* 5204 */ 223,
/* 5205 */ 50353,
/* 5206 */ 163,
/* 5207 */ 5313,
/* 5208 */ -5193,
/* 5209 */ 339,
/* 5210 */ 5214,
/* 5211 */ 286,
/* 5212 */ -5210,
/* 5213 */ 917,
/* 5214 */ 5219,
/* 5215 */ 223,
/* 5216 */ 50037,
/* 5217 */ -5210,
/* 5218 */ 918,
/* 5219 */ 0,
/* 5220 */ -5210,
/* 5221 */ 1365,
/* 5222 */ 5226,
/* 5223 */ 223,
/* 5224 */ -5222,
/* 5225 */ 919,
/* 5226 */ 0,
/* 5227 */ -5222,
/* 5228 */ 1366,
/* 5229 */ 5233,
/* 5230 */ 50319,
/* 5231 */ -5229,
/* 5232 */ 920,
/* 5233 */ 0,
/* 5234 */ -5229,
/* 5235 */ 1367,
/* 5236 */ 5243,
/* 5237 */ 50040,
/* 5238 */ 5246,
/* 5239 */ 223,
/* 5240 */ 50041,
/* 5241 */ -5236,
/* 5242 */ 924,
/* 5243 */ 0,
/* 5244 */ -5236,
/* 5245 */ 1368,
/* 5246 */ 5250,
/* 5247 */ 50322,
/* 5248 */ -5246,
/* 5249 */ 921,
/* 5250 */ 5254,
/* 5251 */ 50307,
/* 5252 */ -5246,
/* 5253 */ 922,
/* 5254 */ 0,
/* 5255 */ 50374,
/* 5256 */ -5246,
/* 5257 */ 923,
/* 5258 */ 5267,
/* 5259 */ 50288,
/* 5260 */ 5270,
/* 5261 */ 5277,
/* 5262 */ 5284,
/* 5263 */ 5291,
/* 5264 */ 5258,
/* 5265 */ -5258,
/* 5266 */ 932,
/* 5267 */ 0,
/* 5268 */ -5258,
/* 5269 */ 1373,
/* 5270 */ 5274,
/* 5271 */ 286,
/* 5272 */ -5270,
/* 5273 */ 925,
/* 5274 */ 0,
/* 5275 */ -5270,
/* 5276 */ 1369,
/* 5277 */ 5281,
/* 5278 */ 223,
/* 5279 */ -5277,
/* 5280 */ 926,
/* 5281 */ 0,
/* 5282 */ -5277,
/* 5283 */ 1370,
/* 5284 */ 5288,
/* 5285 */ 50319,
/* 5286 */ -5284,
/* 5287 */ 927,
/* 5288 */ 0,
/* 5289 */ -5284,
/* 5290 */ 1371,
/* 5291 */ 5298,
/* 5292 */ 50040,
/* 5293 */ 5301,
/* 5294 */ 223,
/* 5295 */ 50041,
/* 5296 */ -5291,
/* 5297 */ 931,
/* 5298 */ 0,
/* 5299 */ -5291,
/* 5300 */ 1372,
/* 5301 */ 5305,
/* 5302 */ 50322,
/* 5303 */ -5301,
/* 5304 */ 928,
/* 5305 */ 5309,
/* 5306 */ 50307,
/* 5307 */ -5301,
/* 5308 */ 929,
/* 5309 */ 0,
/* 5310 */ 50374,
/* 5311 */ -5301,
/* 5312 */ 930,
/* 5313 */ 5317,
/* 5314 */ 223,
/* 5315 */ -5313,
/* 5316 */ 933,
/* 5317 */ 0,
/* 5318 */ -5313,
/* 5319 */ 1374,
/* 5320 */ 0,
/* 5321 */ 50261,
/* 5322 */ -5320,
/* 5323 */ 340,
/* 5324 */ 0,
/* 5325 */ 50341,
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
/* 5338 */ 934,
/* 5339 */ 0,
/* 5340 */ -5334,
/* 5341 */ 1375,
/* 5342 */ 5349,
/* 5343 */ 50326,
/* 5344 */ 5371,
/* 5345 */ 50257,
/* 5346 */ 50059,
/* 5347 */ -5342,
/* 5348 */ 342,
/* 5349 */ 5356,
/* 5350 */ 50383,
/* 5351 */ 5378,
/* 5352 */ 223,
/* 5353 */ 50059,
/* 5354 */ -5342,
/* 5355 */ 343,
/* 5356 */ 5363,
/* 5357 */ 50310,
/* 5358 */ 5385,
/* 5359 */ 1247,
/* 5360 */ 50059,
/* 5361 */ -5342,
/* 5362 */ 344,
/* 5363 */ 0,
/* 5364 */ 50466,
/* 5365 */ 5392,
/* 5366 */ 223,
/* 5367 */ 50037,
/* 5368 */ 50059,
/* 5369 */ -5342,
/* 5370 */ 345,
/* 5371 */ 5375,
/* 5372 */ 50058,
/* 5373 */ -5371,
/* 5374 */ 935,
/* 5375 */ 0,
/* 5376 */ -5371,
/* 5377 */ 1376,
/* 5378 */ 5382,
/* 5379 */ 50058,
/* 5380 */ -5378,
/* 5381 */ 936,
/* 5382 */ 0,
/* 5383 */ -5378,
/* 5384 */ 1377,
/* 5385 */ 5389,
/* 5386 */ 50058,
/* 5387 */ -5385,
/* 5388 */ 937,
/* 5389 */ 0,
/* 5390 */ -5385,
/* 5391 */ 1378,
/* 5392 */ 5396,
/* 5393 */ 50058,
/* 5394 */ -5392,
/* 5395 */ 938,
/* 5396 */ 0,
/* 5397 */ -5392,
/* 5398 */ 1379,
/* 5399 */ 0,
/* 5400 */ 50336,
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
/* 5412 */ 939,
/* 5413 */ 0,
/* 5414 */ -5408,
/* 5415 */ 1380,
/* 5416 */ 5421,
/* 5417 */ 5424,
/* 5418 */ 5416,
/* 5419 */ -5416,
/* 5420 */ 940,
/* 5421 */ 0,
/* 5422 */ -5416,
/* 5423 */ 1381,
/* 5424 */ 5431,
/* 5425 */ 50450,
/* 5426 */ 50058,
/* 5427 */ 173,
/* 5428 */ 50059,
/* 5429 */ -5424,
/* 5430 */ 347,
/* 5431 */ 5438,
/* 5432 */ 50325,
/* 5433 */ 50058,
/* 5434 */ 223,
/* 5435 */ 50059,
/* 5436 */ -5424,
/* 5437 */ 348,
/* 5438 */ 5445,
/* 5439 */ 50466,
/* 5440 */ 50058,
/* 5441 */ 223,
/* 5442 */ 50059,
/* 5443 */ -5424,
/* 5444 */ 349,
/* 5445 */ 5452,
/* 5446 */ 50307,
/* 5447 */ 50058,
/* 5448 */ 223,
/* 5449 */ 50059,
/* 5450 */ -5424,
/* 5451 */ 350,
/* 5452 */ 5460,
/* 5453 */ 50307,
/* 5454 */ 50058,
/* 5455 */ 223,
/* 5456 */ 50043,
/* 5457 */ 50059,
/* 5458 */ -5424,
/* 5459 */ 351,
/* 5460 */ 5469,
/* 5461 */ 50307,
/* 5462 */ 50058,
/* 5463 */ 223,
/* 5464 */ 5506,
/* 5465 */ 223,
/* 5466 */ 50059,
/* 5467 */ -5424,
/* 5468 */ 352,
/* 5469 */ 5476,
/* 5470 */ 50285,
/* 5471 */ 50058,
/* 5472 */ 223,
/* 5473 */ 50059,
/* 5474 */ -5424,
/* 5475 */ 353,
/* 5476 */ 5483,
/* 5477 */ 50406,
/* 5478 */ 50058,
/* 5479 */ 5514,
/* 5480 */ 50059,
/* 5481 */ -5424,
/* 5482 */ 354,
/* 5483 */ 5490,
/* 5484 */ 50350,
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
/* 5503 */ 6102,
/* 5504 */ -5424,
/* 5505 */ 359,
/* 5506 */ 5510,
/* 5507 */ 50457,
/* 5508 */ -5506,
/* 5509 */ 941,
/* 5510 */ 0,
/* 5511 */ 50045,
/* 5512 */ -5506,
/* 5513 */ 942,
/* 5514 */ 5519,
/* 5515 */ 5549,
/* 5516 */ 5514,
/* 5517 */ -5514,
/* 5518 */ 943,
/* 5519 */ 0,
/* 5520 */ -5514,
/* 5521 */ 1382,
/* 5522 */ 5526,
/* 5523 */ 50058,
/* 5524 */ -5522,
/* 5525 */ 944,
/* 5526 */ 0,
/* 5527 */ -5522,
/* 5528 */ 1383,
/* 5529 */ 5535,
/* 5530 */ 223,
/* 5531 */ 5538,
/* 5532 */ 5529,
/* 5533 */ -5529,
/* 5534 */ 947,
/* 5535 */ 0,
/* 5536 */ -5529,
/* 5537 */ 1385,
/* 5538 */ 5542,
/* 5539 */ 50044,
/* 5540 */ -5538,
/* 5541 */ 945,
/* 5542 */ 5546,
/* 5543 */ 50124,
/* 5544 */ -5538,
/* 5545 */ 946,
/* 5546 */ 0,
/* 5547 */ -5538,
/* 5548 */ 1384,
/* 5549 */ 0,
/* 5550 */ 5558,
/* 5551 */ 5570,
/* 5552 */ 50399,
/* 5553 */ 5633,
/* 5554 */ 5646,
/* 5555 */ 5662,
/* 5556 */ -5549,
/* 5557 */ 360,
/* 5558 */ 5562,
/* 5559 */ 50042,
/* 5560 */ -5558,
/* 5561 */ 948,
/* 5562 */ 5566,
/* 5563 */ 50335,
/* 5564 */ -5558,
/* 5565 */ 949,
/* 5566 */ 0,
/* 5567 */ 50124,
/* 5568 */ -5558,
/* 5569 */ 950,
/* 5570 */ 5576,
/* 5571 */ 237,
/* 5572 */ 50037,
/* 5573 */ 5570,
/* 5574 */ -5570,
/* 5575 */ 951,
/* 5576 */ 5585,
/* 5577 */ 50060,
/* 5578 */ 50307,
/* 5579 */ 237,
/* 5580 */ 50043,
/* 5581 */ 50062,
/* 5582 */ 5570,
/* 5583 */ -5570,
/* 5584 */ 952,
/* 5585 */ 5592,
/* 5586 */ 50091,
/* 5587 */ 237,
/* 5588 */ 50093,
/* 5589 */ 5570,
/* 5590 */ -5570,
/* 5591 */ 953,
/* 5592 */ 5599,
/* 5593 */ 50123,
/* 5594 */ 588,
/* 5595 */ 50125,
/* 5596 */ 5570,
/* 5597 */ -5570,
/* 5598 */ 954,
/* 5599 */ 5606,
/* 5600 */ 237,
/* 5601 */ 5625,
/* 5602 */ 237,
/* 5603 */ 5570,
/* 5604 */ -5570,
/* 5605 */ 957,
/* 5606 */ 5611,
/* 5607 */ 237,
/* 5608 */ 5570,
/* 5609 */ -5570,
/* 5610 */ 958,
/* 5611 */ 5617,
/* 5612 */ 237,
/* 5613 */ 50043,
/* 5614 */ 5570,
/* 5615 */ -5570,
/* 5616 */ 959,
/* 5617 */ 5622,
/* 5618 */ 286,
/* 5619 */ 5570,
/* 5620 */ -5570,
/* 5621 */ 960,
/* 5622 */ 0,
/* 5623 */ -5570,
/* 5624 */ 1386,
/* 5625 */ 5629,
/* 5626 */ 50045,
/* 5627 */ -5625,
/* 5628 */ 955,
/* 5629 */ 0,
/* 5630 */ 50457,
/* 5631 */ -5625,
/* 5632 */ 956,
/* 5633 */ 5637,
/* 5634 */ 163,
/* 5635 */ -5633,
/* 5636 */ 961,
/* 5637 */ 5641,
/* 5638 */ 223,
/* 5639 */ -5633,
/* 5640 */ 962,
/* 5641 */ 0,
/* 5642 */ 50289,
/* 5643 */ 223,
/* 5644 */ -5633,
/* 5645 */ 963,
/* 5646 */ 5650,
/* 5647 */ 163,
/* 5648 */ -5646,
/* 5649 */ 964,
/* 5650 */ 5654,
/* 5651 */ 223,
/* 5652 */ -5646,
/* 5653 */ 965,
/* 5654 */ 5659,
/* 5655 */ 50289,
/* 5656 */ 223,
/* 5657 */ -5646,
/* 5658 */ 966,
/* 5659 */ 0,
/* 5660 */ -5646,
/* 5661 */ 1387,
/* 5662 */ 5669,
/* 5663 */ 50091,
/* 5664 */ 223,
/* 5665 */ 5672,
/* 5666 */ 50093,
/* 5667 */ -5662,
/* 5668 */ 970,
/* 5669 */ 0,
/* 5670 */ -5662,
/* 5671 */ 1389,
/* 5672 */ 5678,
/* 5673 */ 5681,
/* 5674 */ 223,
/* 5675 */ 5672,
/* 5676 */ -5672,
/* 5677 */ 969,
/* 5678 */ 0,
/* 5679 */ -5672,
/* 5680 */ 1388,
/* 5681 */ 5685,
/* 5682 */ 50124,
/* 5683 */ -5681,
/* 5684 */ 967,
/* 5685 */ 0,
/* 5686 */ 50044,
/* 5687 */ -5681,
/* 5688 */ 968,
/* 5689 */ 0,
/* 5690 */ 50299,
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
/* 5703 */ 971,
/* 5704 */ 0,
/* 5705 */ -5699,
/* 5706 */ 1390,
/* 5707 */ 5714,
/* 5708 */ 50436,
/* 5709 */ 5733,
/* 5710 */ 163,
/* 5711 */ 50059,
/* 5712 */ -5707,
/* 5713 */ 362,
/* 5714 */ 5721,
/* 5715 */ 50350,
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
/* 5730 */ 6102,
/* 5731 */ -5707,
/* 5732 */ 366,
/* 5733 */ 5737,
/* 5734 */ 50058,
/* 5735 */ -5733,
/* 5736 */ 972,
/* 5737 */ 0,
/* 5738 */ -5733,
/* 5739 */ 1391,
/* 5740 */ 5744,
/* 5741 */ 50058,
/* 5742 */ -5740,
/* 5743 */ 973,
/* 5744 */ 0,
/* 5745 */ -5740,
/* 5746 */ 1392,
/* 5747 */ 5753,
/* 5748 */ 223,
/* 5749 */ 5756,
/* 5750 */ 5747,
/* 5751 */ -5747,
/* 5752 */ 976,
/* 5753 */ 0,
/* 5754 */ -5747,
/* 5755 */ 1394,
/* 5756 */ 5760,
/* 5757 */ 50124,
/* 5758 */ -5756,
/* 5759 */ 974,
/* 5760 */ 5764,
/* 5761 */ 50044,
/* 5762 */ -5756,
/* 5763 */ 975,
/* 5764 */ 0,
/* 5765 */ -5756,
/* 5766 */ 1393,
/* 5767 */ 0,
/* 5768 */ 50451,
/* 5769 */ 50257,
/* 5770 */ 5774,
/* 5771 */ 50059,
/* 5772 */ -5767,
/* 5773 */ 367,
/* 5774 */ 5779,
/* 5775 */ 50257,
/* 5776 */ 5774,
/* 5777 */ -5774,
/* 5778 */ 977,
/* 5779 */ 5784,
/* 5780 */ 163,
/* 5781 */ 5774,
/* 5782 */ -5774,
/* 5783 */ 978,
/* 5784 */ 5789,
/* 5785 */ 50040,
/* 5786 */ 5774,
/* 5787 */ -5774,
/* 5788 */ 979,
/* 5789 */ 5794,
/* 5790 */ 50041,
/* 5791 */ 5774,
/* 5792 */ -5774,
/* 5793 */ 980,
/* 5794 */ 5799,
/* 5795 */ 50124,
/* 5796 */ 5774,
/* 5797 */ -5774,
/* 5798 */ 981,
/* 5799 */ 5804,
/* 5800 */ 50059,
/* 5801 */ 5774,
/* 5802 */ -5774,
/* 5803 */ 982,
/* 5804 */ 5812,
/* 5805 */ 50091,
/* 5806 */ 588,
/* 5807 */ 50093,
/* 5808 */ 50058,
/* 5809 */ 5774,
/* 5810 */ -5774,
/* 5811 */ 983,
/* 5812 */ 5819,
/* 5813 */ 50091,
/* 5814 */ 592,
/* 5815 */ 50093,
/* 5816 */ 5774,
/* 5817 */ -5774,
/* 5818 */ 984,
/* 5819 */ 0,
/* 5820 */ -5774,
/* 5821 */ 1395,
/* 5822 */ 0,
/* 5823 */ 50261,
/* 5824 */ -5822,
/* 5825 */ 368,
/* 5826 */ 5830,
/* 5827 */ 50264,
/* 5828 */ -5826,
/* 5829 */ 369,
/* 5830 */ 5834,
/* 5831 */ 50274,
/* 5832 */ -5826,
/* 5833 */ 370,
/* 5834 */ 5838,
/* 5835 */ 50268,
/* 5836 */ -5826,
/* 5837 */ 371,
/* 5838 */ 5842,
/* 5839 */ 50269,
/* 5840 */ -5826,
/* 5841 */ 372,
/* 5842 */ 5846,
/* 5843 */ 50265,
/* 5844 */ -5826,
/* 5845 */ 373,
/* 5846 */ 5850,
/* 5847 */ 50266,
/* 5848 */ -5826,
/* 5849 */ 374,
/* 5850 */ 5854,
/* 5851 */ 50267,
/* 5852 */ -5826,
/* 5853 */ 375,
/* 5854 */ 5858,
/* 5855 */ 50263,
/* 5856 */ -5826,
/* 5857 */ 376,
/* 5858 */ 5862,
/* 5859 */ 50271,
/* 5860 */ -5826,
/* 5861 */ 377,
/* 5862 */ 5866,
/* 5863 */ 50272,
/* 5864 */ -5826,
/* 5865 */ 378,
/* 5866 */ 5870,
/* 5867 */ 50273,
/* 5868 */ -5826,
/* 5869 */ 379,
/* 5870 */ 0,
/* 5871 */ 50474,
/* 5872 */ -5826,
/* 5873 */ 380,
/* 5874 */ 0,
/* 5875 */ 50447,
/* 5876 */ 5826,
/* 5877 */ 223,
/* 5878 */ 50262,
/* 5879 */ 50258,
/* 5880 */ 50040,
/* 5881 */ 5887,
/* 5882 */ 5956,
/* 5883 */ 50041,
/* 5884 */ 50059,
/* 5885 */ -5874,
/* 5886 */ 381,
/* 5887 */ 5895,
/* 5888 */ 5826,
/* 5889 */ 5898,
/* 5890 */ 5906,
/* 5891 */ 5913,
/* 5892 */ 5921,
/* 5893 */ -5887,
/* 5894 */ 992,
/* 5895 */ 0,
/* 5896 */ -5887,
/* 5897 */ 1403,
/* 5898 */ 5903,
/* 5899 */ 50058,
/* 5900 */ 223,
/* 5901 */ -5898,
/* 5902 */ 985,
/* 5903 */ 0,
/* 5904 */ -5898,
/* 5905 */ 1396,
/* 5906 */ 5910,
/* 5907 */ 50258,
/* 5908 */ -5906,
/* 5909 */ 986,
/* 5910 */ 0,
/* 5911 */ -5906,
/* 5912 */ 1397,
/* 5913 */ 5918,
/* 5914 */ 50061,
/* 5915 */ 173,
/* 5916 */ -5913,
/* 5917 */ 987,
/* 5918 */ 0,
/* 5919 */ -5913,
/* 5920 */ 1398,
/* 5921 */ 5930,
/* 5922 */ 50044,
/* 5923 */ 5826,
/* 5924 */ 5933,
/* 5925 */ 5941,
/* 5926 */ 5948,
/* 5927 */ 5921,
/* 5928 */ -5921,
/* 5929 */ 991,
/* 5930 */ 0,
/* 5931 */ -5921,
/* 5932 */ 1402,
/* 5933 */ 5938,
/* 5934 */ 50058,
/* 5935 */ 223,
/* 5936 */ -5933,
/* 5937 */ 988,
/* 5938 */ 0,
/* 5939 */ -5933,
/* 5940 */ 1399,
/* 5941 */ 5945,
/* 5942 */ 50258,
/* 5943 */ -5941,
/* 5944 */ 989,
/* 5945 */ 0,
/* 5946 */ -5941,
/* 5947 */ 1400,
/* 5948 */ 5953,
/* 5949 */ 50061,
/* 5950 */ 173,
/* 5951 */ -5948,
/* 5952 */ 990,
/* 5953 */ 0,
/* 5954 */ -5948,
/* 5955 */ 1401,
/* 5956 */ 5961,
/* 5957 */ 50044,
/* 5958 */ 50495,
/* 5959 */ -5956,
/* 5960 */ 993,
/* 5961 */ 0,
/* 5962 */ -5956,
/* 5963 */ 1404,
/* 5964 */ 0,
/* 5965 */ 50447,
/* 5966 */ 5826,
/* 5967 */ 50258,
/* 5968 */ 50040,
/* 5969 */ 5975,
/* 5970 */ 6044,
/* 5971 */ 50041,
/* 5972 */ 50059,
/* 5973 */ -5964,
/* 5974 */ 382,
/* 5975 */ 5983,
/* 5976 */ 5826,
/* 5977 */ 5986,
/* 5978 */ 5994,
/* 5979 */ 6001,
/* 5980 */ 6009,
/* 5981 */ -5975,
/* 5982 */ 1001,
/* 5983 */ 0,
/* 5984 */ -5975,
/* 5985 */ 1412,
/* 5986 */ 5991,
/* 5987 */ 50058,
/* 5988 */ 223,
/* 5989 */ -5986,
/* 5990 */ 994,
/* 5991 */ 0,
/* 5992 */ -5986,
/* 5993 */ 1405,
/* 5994 */ 5998,
/* 5995 */ 50258,
/* 5996 */ -5994,
/* 5997 */ 995,
/* 5998 */ 0,
/* 5999 */ -5994,
/* 6000 */ 1406,
/* 6001 */ 6006,
/* 6002 */ 50061,
/* 6003 */ 173,
/* 6004 */ -6001,
/* 6005 */ 996,
/* 6006 */ 0,
/* 6007 */ -6001,
/* 6008 */ 1407,
/* 6009 */ 6018,
/* 6010 */ 50044,
/* 6011 */ 5826,
/* 6012 */ 6021,
/* 6013 */ 6029,
/* 6014 */ 6036,
/* 6015 */ 6009,
/* 6016 */ -6009,
/* 6017 */ 1000,
/* 6018 */ 0,
/* 6019 */ -6009,
/* 6020 */ 1411,
/* 6021 */ 6026,
/* 6022 */ 50058,
/* 6023 */ 223,
/* 6024 */ -6021,
/* 6025 */ 997,
/* 6026 */ 0,
/* 6027 */ -6021,
/* 6028 */ 1408,
/* 6029 */ 6033,
/* 6030 */ 50258,
/* 6031 */ -6029,
/* 6032 */ 998,
/* 6033 */ 0,
/* 6034 */ -6029,
/* 6035 */ 1409,
/* 6036 */ 6041,
/* 6037 */ 50061,
/* 6038 */ 173,
/* 6039 */ -6036,
/* 6040 */ 999,
/* 6041 */ 0,
/* 6042 */ -6036,
/* 6043 */ 1410,
/* 6044 */ 6049,
/* 6045 */ 50044,
/* 6046 */ 50495,
/* 6047 */ -6044,
/* 6048 */ 1002,
/* 6049 */ 0,
/* 6050 */ -6044,
/* 6051 */ 1413,
/* 6052 */ 0,
/* 6053 */ 50447,
/* 6054 */ 5826,
/* 6055 */ 223,
/* 6056 */ 50262,
/* 6057 */ 50258,
/* 6058 */ 50059,
/* 6059 */ -6052,
/* 6060 */ 383,
/* 6061 */ 0,
/* 6062 */ 50447,
/* 6063 */ 223,
/* 6064 */ 50258,
/* 6065 */ 50481,
/* 6066 */ 223,
/* 6067 */ 50059,
/* 6068 */ -6061,
/* 6069 */ 384,
/* 6070 */ 0,
/* 6071 */ 5826,
/* 6072 */ 50258,
/* 6073 */ 6077,
/* 6074 */ 50059,
/* 6075 */ -6070,
/* 6076 */ 385,
/* 6077 */ 6083,
/* 6078 */ 50044,
/* 6079 */ 50258,
/* 6080 */ 6077,
/* 6081 */ -6077,
/* 6082 */ 1003,
/* 6083 */ 0,
/* 6084 */ -6077,
/* 6085 */ 1414,
/* 6086 */ 0,
/* 6087 */ 5826,
/* 6088 */ 50258,
/* 6089 */ 6093,
/* 6090 */ 50059,
/* 6091 */ -6086,
/* 6092 */ 386,
/* 6093 */ 6099,
/* 6094 */ 50044,
/* 6095 */ 50258,
/* 6096 */ 6093,
/* 6097 */ -6093,
/* 6098 */ 1004,
/* 6099 */ 0,
/* 6100 */ -6093,
/* 6101 */ 1415,
/* 6102 */ 0,
/* 6103 */ 5826,
/* 6104 */ 50258,
/* 6105 */ 6109,
/* 6106 */ 50059,
/* 6107 */ -6102,
/* 6108 */ 387,
/* 6109 */ 6115,
/* 6110 */ 50044,
/* 6111 */ 50258,
/* 6112 */ 6109,
/* 6113 */ -6109,
/* 6114 */ 1005,
/* 6115 */ 0,
/* 6116 */ -6109,
/* 6117 */ 1416,
/* 6118 */ 6123,
/* 6119 */ 6719,
/* 6120 */ 50059,
/* 6121 */ -6118,
/* 6122 */ 388,
/* 6123 */ 6133,
/* 6124 */ 50367,
/* 6125 */ 50040,
/* 6126 */ 6719,
/* 6127 */ 50041,
/* 6128 */ 6118,
/* 6129 */ 50335,
/* 6130 */ 6118,
/* 6131 */ -6118,
/* 6132 */ 389,
/* 6133 */ 6141,
/* 6134 */ 50367,
/* 6135 */ 50040,
/* 6136 */ 6719,
/* 6137 */ 50041,
/* 6138 */ 6118,
/* 6139 */ -6118,
/* 6140 */ 390,
/* 6141 */ 6153,
/* 6142 */ 50353,
/* 6143 */ 50040,
/* 6144 */ 6719,
/* 6145 */ 50059,
/* 6146 */ 6719,
/* 6147 */ 50059,
/* 6148 */ 6719,
/* 6149 */ 50041,
/* 6150 */ 6118,
/* 6151 */ -6118,
/* 6152 */ 391,
/* 6153 */ 6164,
/* 6154 */ 50446,
/* 6155 */ 50040,
/* 6156 */ 6719,
/* 6157 */ 50041,
/* 6158 */ 50123,
/* 6159 */ 6207,
/* 6160 */ 6234,
/* 6161 */ 50125,
/* 6162 */ -6118,
/* 6163 */ 392,
/* 6164 */ 6169,
/* 6165 */ 50302,
/* 6166 */ 50059,
/* 6167 */ -6118,
/* 6168 */ 393,
/* 6169 */ 6174,
/* 6170 */ 50314,
/* 6171 */ 50059,
/* 6172 */ -6118,
/* 6173 */ 394,
/* 6174 */ 6184,
/* 6175 */ 50331,
/* 6176 */ 6118,
/* 6177 */ 50469,
/* 6178 */ 50040,
/* 6179 */ 6719,
/* 6180 */ 50041,
/* 6181 */ 50059,
/* 6182 */ -6118,
/* 6183 */ 395,
/* 6184 */ 6192,
/* 6185 */ 50469,
/* 6186 */ 50040,
/* 6187 */ 6719,
/* 6188 */ 50041,
/* 6189 */ 6118,
/* 6190 */ -6118,
/* 6191 */ 396,
/* 6192 */ 6196,
/* 6193 */ 6251,
/* 6194 */ -6118,
/* 6195 */ 397,
/* 6196 */ 6202,
/* 6197 */ 50425,
/* 6198 */ 6719,
/* 6199 */ 50059,
/* 6200 */ -6118,
/* 6201 */ 398,
/* 6202 */ 0,
/* 6203 */ 50425,
/* 6204 */ 50059,
/* 6205 */ -6118,
/* 6206 */ 399,
/* 6207 */ 6215,
/* 6208 */ 50305,
/* 6209 */ 6218,
/* 6210 */ 50058,
/* 6211 */ 6226,
/* 6212 */ 6207,
/* 6213 */ -6207,
/* 6214 */ 1009,
/* 6215 */ 0,
/* 6216 */ -6207,
/* 6217 */ 1418,
/* 6218 */ 6222,
/* 6219 */ 173,
/* 6220 */ -6218,
/* 6221 */ 1006,
/* 6222 */ 0,
/* 6223 */ 163,
/* 6224 */ -6218,
/* 6225 */ 1007,
/* 6226 */ 6231,
/* 6227 */ 6118,
/* 6228 */ 6226,
/* 6229 */ -6226,
/* 6230 */ 1008,
/* 6231 */ 0,
/* 6232 */ -6226,
/* 6233 */ 1417,
/* 6234 */ 6240,
/* 6235 */ 50324,
/* 6236 */ 50058,
/* 6237 */ 6243,
/* 6238 */ -6234,
/* 6239 */ 1011,
/* 6240 */ 0,
/* 6241 */ -6234,
/* 6242 */ 1420,
/* 6243 */ 6248,
/* 6244 */ 6118,
/* 6245 */ 6243,
/* 6246 */ -6243,
/* 6247 */ 1010,
/* 6248 */ 0,
/* 6249 */ -6243,
/* 6250 */ 1419,
/* 6251 */ 0,
/* 6252 */ 50123,
/* 6253 */ 6257,
/* 6254 */ 50125,
/* 6255 */ -6251,
/* 6256 */ 400,
/* 6257 */ 6262,
/* 6258 */ 6118,
/* 6259 */ 6257,
/* 6260 */ -6257,
/* 6261 */ 1012,
/* 6262 */ 0,
/* 6263 */ -6257,
/* 6264 */ 1421,
/* 6265 */ 0,
/* 6266 */ 50123,
/* 6267 */ 6272,
/* 6268 */ 6286,
/* 6269 */ 50125,
/* 6270 */ -6265,
/* 6271 */ 401,
/* 6272 */ 6277,
/* 6273 */ 6086,
/* 6274 */ 6272,
/* 6275 */ -6272,
/* 6276 */ 1013,
/* 6277 */ 6283,
/* 6278 */ 50441,
/* 6279 */ 6102,
/* 6280 */ 6272,
/* 6281 */ -6272,
/* 6282 */ 1014,
/* 6283 */ 0,
/* 6284 */ -6272,
/* 6285 */ 1422,
/* 6286 */ 6291,
/* 6287 */ 6118,
/* 6288 */ 6286,
/* 6289 */ -6286,
/* 6290 */ 1015,
/* 6291 */ 0,
/* 6292 */ -6286,
/* 6293 */ 1423,
/* 6294 */ 6298,
/* 6295 */ 50256,
/* 6296 */ -6294,
/* 6297 */ 402,
/* 6298 */ 6302,
/* 6299 */ 50259,
/* 6300 */ -6294,
/* 6301 */ 403,
/* 6302 */ 6306,
/* 6303 */ 50257,
/* 6304 */ -6294,
/* 6305 */ 404,
/* 6306 */ 6310,
/* 6307 */ 50458,
/* 6308 */ -6294,
/* 6309 */ 405,
/* 6310 */ 6314,
/* 6311 */ 50345,
/* 6312 */ -6294,
/* 6313 */ 406,
/* 6314 */ 6318,
/* 6315 */ 50395,
/* 6316 */ -6294,
/* 6317 */ 407,
/* 6318 */ 6322,
/* 6319 */ 163,
/* 6320 */ -6294,
/* 6321 */ 408,
/* 6322 */ 6328,
/* 6323 */ 50040,
/* 6324 */ 6719,
/* 6325 */ 50041,
/* 6326 */ -6294,
/* 6327 */ 409,
/* 6328 */ 6332,
/* 6329 */ 50258,
/* 6330 */ -6294,
/* 6331 */ 410,
/* 6332 */ 6338,
/* 6333 */ 6294,
/* 6334 */ 6360,
/* 6335 */ 50258,
/* 6336 */ -6294,
/* 6337 */ 411,
/* 6338 */ 6347,
/* 6339 */ 6294,
/* 6340 */ 6368,
/* 6341 */ 50258,
/* 6342 */ 50040,
/* 6343 */ 6729,
/* 6344 */ 50041,
/* 6345 */ -6294,
/* 6346 */ 412,
/* 6347 */ 6354,
/* 6348 */ 50258,
/* 6349 */ 50040,
/* 6350 */ 6729,
/* 6351 */ 50041,
/* 6352 */ -6294,
/* 6353 */ 413,
/* 6354 */ 0,
/* 6355 */ 6294,
/* 6356 */ 50064,
/* 6357 */ 50258,
/* 6358 */ -6294,
/* 6359 */ 414,
/* 6360 */ 6364,
/* 6361 */ 50046,
/* 6362 */ -6360,
/* 6363 */ 1016,
/* 6364 */ 0,
/* 6365 */ 50496,
/* 6366 */ -6360,
/* 6367 */ 1017,
/* 6368 */ 6372,
/* 6369 */ 50046,
/* 6370 */ -6368,
/* 6371 */ 1018,
/* 6372 */ 0,
/* 6373 */ 50496,
/* 6374 */ -6368,
/* 6375 */ 1019,
/* 6376 */ 6380,
/* 6377 */ 50258,
/* 6378 */ -6376,
/* 6379 */ 415,
/* 6380 */ 6386,
/* 6381 */ 6376,
/* 6382 */ 50064,
/* 6383 */ 50258,
/* 6384 */ -6376,
/* 6385 */ 416,
/* 6386 */ 0,
/* 6387 */ 6294,
/* 6388 */ 6392,
/* 6389 */ 50258,
/* 6390 */ -6376,
/* 6391 */ 417,
/* 6392 */ 6396,
/* 6393 */ 50046,
/* 6394 */ -6392,
/* 6395 */ 1020,
/* 6396 */ 0,
/* 6397 */ 50496,
/* 6398 */ -6392,
/* 6399 */ 1021,
/* 6400 */ 6405,
/* 6401 */ 6376,
/* 6402 */ 50475,
/* 6403 */ -6400,
/* 6404 */ 418,
/* 6405 */ 6410,
/* 6406 */ 6376,
/* 6407 */ 50476,
/* 6408 */ -6400,
/* 6409 */ 419,
/* 6410 */ 6419,
/* 6411 */ 50387,
/* 6412 */ 50040,
/* 6413 */ 6649,
/* 6414 */ 50044,
/* 6415 */ 6649,
/* 6416 */ 50041,
/* 6417 */ -6400,
/* 6418 */ 420,
/* 6419 */ 6428,
/* 6420 */ 50384,
/* 6421 */ 50040,
/* 6422 */ 6649,
/* 6423 */ 50044,
/* 6424 */ 6649,
/* 6425 */ 50041,
/* 6426 */ -6400,
/* 6427 */ 421,
/* 6428 */ 0,
/* 6429 */ 6294,
/* 6430 */ -6400,
/* 6431 */ 422,
/* 6432 */ 6437,
/* 6433 */ 50475,
/* 6434 */ 6376,
/* 6435 */ -6432,
/* 6436 */ 423,
/* 6437 */ 6442,
/* 6438 */ 50476,
/* 6439 */ 6376,
/* 6440 */ -6432,
/* 6441 */ 424,
/* 6442 */ 6447,
/* 6443 */ 50043,
/* 6444 */ 6432,
/* 6445 */ -6432,
/* 6446 */ 425,
/* 6447 */ 6452,
/* 6448 */ 50045,
/* 6449 */ 6432,
/* 6450 */ -6432,
/* 6451 */ 426,
/* 6452 */ 6457,
/* 6453 */ 50033,
/* 6454 */ 6432,
/* 6455 */ -6432,
/* 6456 */ 427,
/* 6457 */ 6462,
/* 6458 */ 50126,
/* 6459 */ 6432,
/* 6460 */ -6432,
/* 6461 */ 428,
/* 6462 */ 6468,
/* 6463 */ 50058,
/* 6464 */ 6432,
/* 6465 */ 50058,
/* 6466 */ -6432,
/* 6467 */ 429,
/* 6468 */ 0,
/* 6469 */ 6400,
/* 6470 */ -6432,
/* 6471 */ 430,
/* 6472 */ 6478,
/* 6473 */ 6432,
/* 6474 */ 50100,
/* 6475 */ 6432,
/* 6476 */ -6472,
/* 6477 */ 431,
/* 6478 */ 0,
/* 6479 */ 6432,
/* 6480 */ -6472,
/* 6481 */ 432,
/* 6482 */ 6489,
/* 6483 */ 50040,
/* 6484 */ 5826,
/* 6485 */ 50041,
/* 6486 */ 6482,
/* 6487 */ -6482,
/* 6488 */ 433,
/* 6489 */ 0,
/* 6490 */ 6472,
/* 6491 */ -6482,
/* 6492 */ 434,
/* 6493 */ 6499,
/* 6494 */ 6493,
/* 6495 */ 50042,
/* 6496 */ 6482,
/* 6497 */ -6493,
/* 6498 */ 435,
/* 6499 */ 6505,
/* 6500 */ 6493,
/* 6501 */ 50047,
/* 6502 */ 6482,
/* 6503 */ -6493,
/* 6504 */ 436,
/* 6505 */ 6511,
/* 6506 */ 6493,
/* 6507 */ 50037,
/* 6508 */ 6482,
/* 6509 */ -6493,
/* 6510 */ 437,
/* 6511 */ 0,
/* 6512 */ 6482,
/* 6513 */ -6493,
/* 6514 */ 438,
/* 6515 */ 6521,
/* 6516 */ 6515,
/* 6517 */ 50043,
/* 6518 */ 6493,
/* 6519 */ -6515,
/* 6520 */ 439,
/* 6521 */ 6527,
/* 6522 */ 6515,
/* 6523 */ 50045,
/* 6524 */ 6493,
/* 6525 */ -6515,
/* 6526 */ 440,
/* 6527 */ 0,
/* 6528 */ 6493,
/* 6529 */ -6515,
/* 6530 */ 441,
/* 6531 */ 6537,
/* 6532 */ 6531,
/* 6533 */ 50477,
/* 6534 */ 6515,
/* 6535 */ -6531,
/* 6536 */ 442,
/* 6537 */ 6543,
/* 6538 */ 6531,
/* 6539 */ 50478,
/* 6540 */ 6515,
/* 6541 */ -6531,
/* 6542 */ 443,
/* 6543 */ 0,
/* 6544 */ 6515,
/* 6545 */ -6531,
/* 6546 */ 444,
/* 6547 */ 6553,
/* 6548 */ 6547,
/* 6549 */ 50060,
/* 6550 */ 6531,
/* 6551 */ -6547,
/* 6552 */ 445,
/* 6553 */ 6559,
/* 6554 */ 6547,
/* 6555 */ 50062,
/* 6556 */ 6531,
/* 6557 */ -6547,
/* 6558 */ 446,
/* 6559 */ 6565,
/* 6560 */ 6547,
/* 6561 */ 50479,
/* 6562 */ 6531,
/* 6563 */ -6547,
/* 6564 */ 447,
/* 6565 */ 6571,
/* 6566 */ 6547,
/* 6567 */ 50480,
/* 6568 */ 6531,
/* 6569 */ -6547,
/* 6570 */ 448,
/* 6571 */ 6577,
/* 6572 */ 6547,
/* 6573 */ 50481,
/* 6574 */ 6531,
/* 6575 */ -6547,
/* 6576 */ 449,
/* 6577 */ 6583,
/* 6578 */ 6547,
/* 6579 */ 50482,
/* 6580 */ 6531,
/* 6581 */ -6547,
/* 6582 */ 450,
/* 6583 */ 0,
/* 6584 */ 6531,
/* 6585 */ -6547,
/* 6586 */ 451,
/* 6587 */ 6593,
/* 6588 */ 6587,
/* 6589 */ 50038,
/* 6590 */ 6547,
/* 6591 */ -6587,
/* 6592 */ 452,
/* 6593 */ 0,
/* 6594 */ 6547,
/* 6595 */ -6587,
/* 6596 */ 453,
/* 6597 */ 6603,
/* 6598 */ 6597,
/* 6599 */ 50094,
/* 6600 */ 6587,
/* 6601 */ -6597,
/* 6602 */ 454,
/* 6603 */ 0,
/* 6604 */ 6587,
/* 6605 */ -6597,
/* 6606 */ 455,
/* 6607 */ 6613,
/* 6608 */ 6607,
/* 6609 */ 50124,
/* 6610 */ 6597,
/* 6611 */ -6607,
/* 6612 */ 456,
/* 6613 */ 0,
/* 6614 */ 6597,
/* 6615 */ -6607,
/* 6616 */ 457,
/* 6617 */ 6623,
/* 6618 */ 6617,
/* 6619 */ 50483,
/* 6620 */ 6607,
/* 6621 */ -6617,
/* 6622 */ 458,
/* 6623 */ 0,
/* 6624 */ 6607,
/* 6625 */ -6617,
/* 6626 */ 459,
/* 6627 */ 6633,
/* 6628 */ 6627,
/* 6629 */ 50484,
/* 6630 */ 6617,
/* 6631 */ -6627,
/* 6632 */ 460,
/* 6633 */ 0,
/* 6634 */ 6617,
/* 6635 */ -6627,
/* 6636 */ 461,
/* 6637 */ 6645,
/* 6638 */ 6627,
/* 6639 */ 50063,
/* 6640 */ 6719,
/* 6641 */ 50058,
/* 6642 */ 6637,
/* 6643 */ -6637,
/* 6644 */ 462,
/* 6645 */ 0,
/* 6646 */ 6627,
/* 6647 */ -6637,
/* 6648 */ 463,
/* 6649 */ 6655,
/* 6650 */ 6376,
/* 6651 */ 50061,
/* 6652 */ 6637,
/* 6653 */ -6649,
/* 6654 */ 464,
/* 6655 */ 6661,
/* 6656 */ 6376,
/* 6657 */ 50486,
/* 6658 */ 6637,
/* 6659 */ -6649,
/* 6660 */ 465,
/* 6661 */ 6667,
/* 6662 */ 6376,
/* 6663 */ 50494,
/* 6664 */ 6637,
/* 6665 */ -6649,
/* 6666 */ 466,
/* 6667 */ 6673,
/* 6668 */ 6376,
/* 6669 */ 50485,
/* 6670 */ 6637,
/* 6671 */ -6649,
/* 6672 */ 467,
/* 6673 */ 6679,
/* 6674 */ 6376,
/* 6675 */ 50487,
/* 6676 */ 6637,
/* 6677 */ -6649,
/* 6678 */ 468,
/* 6679 */ 6685,
/* 6680 */ 6376,
/* 6681 */ 50488,
/* 6682 */ 6637,
/* 6683 */ -6649,
/* 6684 */ 469,
/* 6685 */ 6691,
/* 6686 */ 6376,
/* 6687 */ 50489,
/* 6688 */ 6637,
/* 6689 */ -6649,
/* 6690 */ 470,
/* 6691 */ 6697,
/* 6692 */ 6376,
/* 6693 */ 50490,
/* 6694 */ 6637,
/* 6695 */ -6649,
/* 6696 */ 471,
/* 6697 */ 6703,
/* 6698 */ 6376,
/* 6699 */ 50492,
/* 6700 */ 6637,
/* 6701 */ -6649,
/* 6702 */ 472,
/* 6703 */ 6709,
/* 6704 */ 6376,
/* 6705 */ 50493,
/* 6706 */ 6637,
/* 6707 */ -6649,
/* 6708 */ 473,
/* 6709 */ 6715,
/* 6710 */ 6376,
/* 6711 */ 50491,
/* 6712 */ 6637,
/* 6713 */ -6649,
/* 6714 */ 474,
/* 6715 */ 0,
/* 6716 */ 6637,
/* 6717 */ -6649,
/* 6718 */ 475,
/* 6719 */ 6725,
/* 6720 */ 6719,
/* 6721 */ 50044,
/* 6722 */ 6649,
/* 6723 */ -6719,
/* 6724 */ 476,
/* 6725 */ 0,
/* 6726 */ 6649,
/* 6727 */ -6719,
/* 6728 */ 477,
/* 6729 */ 0,
/* 6730 */ 6733,
/* 6731 */ -6729,
/* 6732 */ 478,
/* 6733 */ 6738,
/* 6734 */ 6649,
/* 6735 */ 6741,
/* 6736 */ -6733,
/* 6737 */ 1023,
/* 6738 */ 0,
/* 6739 */ -6733,
/* 6740 */ 1425,
/* 6741 */ 6747,
/* 6742 */ 50044,
/* 6743 */ 6649,
/* 6744 */ 6741,
/* 6745 */ -6741,
/* 6746 */ 1022,
/* 6747 */ 0,
/* 6748 */ -6741,
/* 6749 */ 1424,
0
};
extern int yyannotation[];
int yyannotation[] = {
0,
/* 1 */ 0,
/* 2 */ 0,
/* 3 */ 50000,
/* 4 */ -1,
/* 5 */ 0,
/* 6 */ 0,
/* 7 */ 1,
/* 8 */ -6,
/* 9 */ 1,
/* 10 */ 15,
/* 11 */ 1,
/* 12 */ 1,
/* 13 */ -10,
/* 14 */ 1,
/* 15 */ 0,
/* 16 */ -10,
/* 17 */ 2,
/* 18 */ 24,
/* 19 */ 50380,
/* 20 */ 50257,
/* 21 */ 50059,
/* 22 */ -18,
/* 23 */ 1,
/* 24 */ 30,
/* 25 */ 50433,
/* 26 */ 1,
/* 27 */ 50059,
/* 28 */ -18,
/* 29 */ 2,
/* 30 */ 36,
/* 31 */ 50352,
/* 32 */ 50257,
/* 33 */ 50059,
/* 34 */ -18,
/* 35 */ 3,
/* 36 */ 40,
/* 37 */ 1,
/* 38 */ -18,
/* 39 */ 4,
/* 40 */ 44,
/* 41 */ 1,
/* 42 */ -18,
/* 43 */ 5,
/* 44 */ 48,
/* 45 */ 1,
/* 46 */ -18,
/* 47 */ 6,
/* 48 */ 52,
/* 49 */ 1,
/* 50 */ -18,
/* 51 */ 7,
/* 52 */ 56,
/* 53 */ 1,
/* 54 */ -18,
/* 55 */ 8,
/* 56 */ 60,
/* 57 */ 1,
/* 58 */ -18,
/* 59 */ 9,
/* 60 */ 64,
/* 61 */ 1,
/* 62 */ -18,
/* 63 */ 10,
/* 64 */ 68,
/* 65 */ 1,
/* 66 */ -18,
/* 67 */ 11,
/* 68 */ 72,
/* 69 */ 1,
/* 70 */ -18,
/* 71 */ 12,
/* 72 */ 76,
/* 73 */ 1,
/* 74 */ -18,
/* 75 */ 13,
/* 76 */ 80,
/* 77 */ 1,
/* 78 */ -18,
/* 79 */ 14,
/* 80 */ 84,
/* 81 */ 1,
/* 82 */ -18,
/* 83 */ 15,
/* 84 */ 88,
/* 85 */ 1,
/* 86 */ -18,
/* 87 */ 16,
/* 88 */ 92,
/* 89 */ 1,
/* 90 */ -18,
/* 91 */ 17,
/* 92 */ 96,
/* 93 */ 1,
/* 94 */ -18,
/* 95 */ 18,
/* 96 */ 100,
/* 97 */ 1,
/* 98 */ -18,
/* 99 */ 19,
/* 100 */ 104,
/* 101 */ 1,
/* 102 */ -18,
/* 103 */ 20,
/* 104 */ 108,
/* 105 */ 1,
/* 106 */ -18,
/* 107 */ 21,
/* 108 */ 112,
/* 109 */ 1,
/* 110 */ -18,
/* 111 */ 22,
/* 112 */ 116,
/* 113 */ 1,
/* 114 */ -18,
/* 115 */ 23,
/* 116 */ 120,
/* 117 */ 1,
/* 118 */ -18,
/* 119 */ 24,
/* 120 */ 124,
/* 121 */ 1,
/* 122 */ -18,
/* 123 */ 25,
/* 124 */ 128,
/* 125 */ 1,
/* 126 */ -18,
/* 127 */ 26,
/* 128 */ 134,
/* 129 */ 50350,
/* 130 */ 1,
/* 131 */ 1,
/* 132 */ -18,
/* 133 */ 27,
/* 134 */ 0,
/* 135 */ 50451,
/* 136 */ 50257,
/* 137 */ 50123,
/* 138 */ 1,
/* 139 */ 50257,
/* 140 */ 1,
/* 141 */ 50125,
/* 142 */ -18,
/* 143 */ 28,
/* 144 */ 0,
/* 145 */ 50058,
/* 146 */ -144,
/* 147 */ 1,
/* 148 */ 153,
/* 149 */ 50326,
/* 150 */ 50058,
/* 151 */ -148,
/* 152 */ 1,
/* 153 */ 0,
/* 154 */ -148,
/* 155 */ 2,
/* 156 */ 160,
/* 157 */ 50059,
/* 158 */ -156,
/* 159 */ 1,
/* 160 */ 0,
/* 161 */ -156,
/* 162 */ 2,
/* 163 */ 168,
/* 164 */ 50036,
/* 165 */ 50257,
/* 166 */ -163,
/* 167 */ 1,
/* 168 */ 0,
/* 169 */ 50036,
/* 170 */ 1,
/* 171 */ -163,
/* 172 */ 2,
/* 173 */ 179,
/* 174 */ 1,
/* 175 */ 50043,
/* 176 */ 1,
/* 177 */ -173,
/* 178 */ 1,
/* 179 */ 185,
/* 180 */ 1,
/* 181 */ 50045,
/* 182 */ 1,
/* 183 */ -173,
/* 184 */ 2,
/* 185 */ 0,
/* 186 */ 1,
/* 187 */ -173,
/* 188 */ 3,
/* 189 */ 195,
/* 190 */ 1,
/* 191 */ 50042,
/* 192 */ 1,
/* 193 */ -189,
/* 194 */ 1,
/* 195 */ 201,
/* 196 */ 1,
/* 197 */ 50047,
/* 198 */ 1,
/* 199 */ -189,
/* 200 */ 2,
/* 201 */ 207,
/* 202 */ 1,
/* 203 */ 50037,
/* 204 */ 1,
/* 205 */ -189,
/* 206 */ 3,
/* 207 */ 213,
/* 208 */ 1,
/* 209 */ 50124,
/* 210 */ 1,
/* 211 */ -189,
/* 212 */ 4,
/* 213 */ 219,
/* 214 */ 1,
/* 215 */ 50038,
/* 216 */ 1,
/* 217 */ -189,
/* 218 */ 5,
/* 219 */ 0,
/* 220 */ 1,
/* 221 */ -189,
/* 222 */ 6,
/* 223 */ 228,
/* 224 */ 50043,
/* 225 */ 1,
/* 226 */ -223,
/* 227 */ 1,
/* 228 */ 233,
/* 229 */ 50045,
/* 230 */ 1,
/* 231 */ -223,
/* 232 */ 2,
/* 233 */ 0,
/* 234 */ 1,
/* 235 */ -223,
/* 236 */ 3,
/* 237 */ 243,
/* 238 */ 50040,
/* 239 */ 1,
/* 240 */ 50041,
/* 241 */ -237,
/* 242 */ 1,
/* 243 */ 247,
/* 244 */ 50256,
/* 245 */ -237,
/* 246 */ 2,
/* 247 */ 251,
/* 248 */ 50458,
/* 249 */ -237,
/* 250 */ 3,
/* 251 */ 255,
/* 252 */ 50345,
/* 253 */ -237,
/* 254 */ 4,
/* 255 */ 0,
/* 256 */ 50259,
/* 257 */ -237,
/* 258 */ 5,
/* 259 */ 267,
/* 260 */ 1,
/* 261 */ 50100,
/* 262 */ 1,
/* 263 */ 50043,
/* 264 */ 1,
/* 265 */ -259,
/* 266 */ 1,
/* 267 */ 276,
/* 268 */ 1,
/* 269 */ 50100,
/* 270 */ 1,
/* 271 */ 50043,
/* 272 */ 50045,
/* 273 */ 1,
/* 274 */ -259,
/* 275 */ 2,
/* 276 */ 282,
/* 277 */ 1,
/* 278 */ 50100,
/* 279 */ 1,
/* 280 */ -259,
/* 281 */ 3,
/* 282 */ 0,
/* 283 */ 1,
/* 284 */ -259,
/* 285 */ 4,
/* 286 */ 294,
/* 287 */ 1,
/* 288 */ 50100,
/* 289 */ 1,
/* 290 */ 50043,
/* 291 */ 1,
/* 292 */ -286,
/* 293 */ 1,
/* 294 */ 0,
/* 295 */ 1,
/* 296 */ 50100,
/* 297 */ 1,
/* 298 */ -286,
/* 299 */ 2,
/* 300 */ 0,
/* 301 */ 1,
/* 302 */ 1,
/* 303 */ 1,
/* 304 */ 1,
/* 305 */ 1,
/* 306 */ 1,
/* 307 */ 1,
/* 308 */ -300,
/* 309 */ 1,
/* 310 */ 317,
/* 311 */ 50367,
/* 312 */ 50040,
/* 313 */ 1,
/* 314 */ 50041,
/* 315 */ -310,
/* 316 */ 1,
/* 317 */ 0,
/* 318 */ -310,
/* 319 */ 2,
/* 320 */ 325,
/* 321 */ 50256,
/* 322 */ 50037,
/* 323 */ -320,
/* 324 */ 1,
/* 325 */ 0,
/* 326 */ -320,
/* 327 */ 2,
/* 328 */ 332,
/* 329 */ 1,
/* 330 */ -328,
/* 331 */ 1,
/* 332 */ 0,
/* 333 */ -328,
/* 334 */ 2,
/* 335 */ 339,
/* 336 */ 50317,
/* 337 */ -335,
/* 338 */ 1,
/* 339 */ 343,
/* 340 */ 50300,
/* 341 */ -335,
/* 342 */ 2,
/* 343 */ 0,
/* 344 */ -335,
/* 345 */ 3,
/* 346 */ 350,
/* 347 */ 1,
/* 348 */ -346,
/* 349 */ 1,
/* 350 */ 0,
/* 351 */ 50419,
/* 352 */ 1,
/* 353 */ -346,
/* 354 */ 2,
/* 355 */ 360,
/* 356 */ 50043,
/* 357 */ 50256,
/* 358 */ -355,
/* 359 */ 1,
/* 360 */ 365,
/* 361 */ 50045,
/* 362 */ 50256,
/* 363 */ -355,
/* 364 */ 2,
/* 365 */ 0,
/* 366 */ -355,
/* 367 */ 3,
/* 368 */ 376,
/* 369 */ 50471,
/* 370 */ 50091,
/* 371 */ 1,
/* 372 */ 50093,
/* 373 */ 1,
/* 374 */ -368,
/* 375 */ 1,
/* 376 */ 382,
/* 377 */ 50399,
/* 378 */ 1,
/* 379 */ 1,
/* 380 */ -368,
/* 381 */ 2,
/* 382 */ 391,
/* 383 */ 50399,
/* 384 */ 50419,
/* 385 */ 1,
/* 386 */ 50457,
/* 387 */ 1,
/* 388 */ 1,
/* 389 */ -368,
/* 390 */ 3,
/* 391 */ 398,
/* 392 */ 50399,
/* 393 */ 50419,
/* 394 */ 1,
/* 395 */ 1,
/* 396 */ -368,
/* 397 */ 4,
/* 398 */ 404,
/* 399 */ 50399,
/* 400 */ 50419,
/* 401 */ 1,
/* 402 */ -368,
/* 403 */ 5,
/* 404 */ 414,
/* 405 */ 50295,
/* 406 */ 50040,
/* 407 */ 1,
/* 408 */ 50044,
/* 409 */ 1,
/* 410 */ 50041,
/* 411 */ 1,
/* 412 */ -368,
/* 413 */ 6,
/* 414 */ 0,
/* 415 */ -368,
/* 416 */ 7,
/* 417 */ 423,
/* 418 */ 1,
/* 419 */ 1,
/* 420 */ 1,
/* 421 */ -417,
/* 422 */ 1,
/* 423 */ 0,
/* 424 */ -417,
/* 425 */ 2,
/* 426 */ 430,
/* 427 */ 50044,
/* 428 */ -426,
/* 429 */ 1,
/* 430 */ 0,
/* 431 */ -426,
/* 432 */ 2,
/* 433 */ 0,
/* 434 */ 50356,
/* 435 */ 1,
/* 436 */ 1,
/* 437 */ 1,
/* 438 */ 50059,
/* 439 */ -433,
/* 440 */ 1,
/* 441 */ 0,
/* 442 */ 50058,
/* 443 */ -441,
/* 444 */ 1,
/* 445 */ 451,
/* 446 */ 1,
/* 447 */ 1,
/* 448 */ 1,
/* 449 */ -445,
/* 450 */ 1,
/* 451 */ 0,
/* 452 */ -445,
/* 453 */ 2,
/* 454 */ 458,
/* 455 */ 50044,
/* 456 */ -454,
/* 457 */ 1,
/* 458 */ 0,
/* 459 */ 50335,
/* 460 */ -454,
/* 461 */ 2,
/* 462 */ 0,
/* 463 */ 1,
/* 464 */ 1,
/* 465 */ 1,
/* 466 */ 1,
/* 467 */ 1,
/* 468 */ 50059,
/* 469 */ -462,
/* 470 */ 1,
/* 471 */ 475,
/* 472 */ 50368,
/* 473 */ -471,
/* 474 */ 1,
/* 475 */ 0,
/* 476 */ 50357,
/* 477 */ -471,
/* 478 */ 2,
/* 479 */ 483,
/* 480 */ 50058,
/* 481 */ -479,
/* 482 */ 1,
/* 483 */ 0,
/* 484 */ -479,
/* 485 */ 2,
/* 486 */ 490,
/* 487 */ 1,
/* 488 */ -486,
/* 489 */ 1,
/* 490 */ 0,
/* 491 */ -486,
/* 492 */ 2,
/* 493 */ 497,
/* 494 */ 50259,
/* 495 */ -493,
/* 496 */ 1,
/* 497 */ 0,
/* 498 */ 50256,
/* 499 */ -493,
/* 500 */ 2,
/* 501 */ 506,
/* 502 */ 50400,
/* 503 */ 1,
/* 504 */ -501,
/* 505 */ 1,
/* 506 */ 0,
/* 507 */ -501,
/* 508 */ 2,
/* 509 */ 0,
/* 510 */ 1,
/* 511 */ 1,
/* 512 */ 1,
/* 513 */ -509,
/* 514 */ 1,
/* 515 */ 519,
/* 516 */ 1,
/* 517 */ -515,
/* 518 */ 1,
/* 519 */ 0,
/* 520 */ -515,
/* 521 */ 2,
/* 522 */ 526,
/* 523 */ 50259,
/* 524 */ -522,
/* 525 */ 1,
/* 526 */ 0,
/* 527 */ 50256,
/* 528 */ -522,
/* 529 */ 2,
/* 530 */ 535,
/* 531 */ 50400,
/* 532 */ 1,
/* 533 */ -530,
/* 534 */ 1,
/* 535 */ 0,
/* 536 */ -530,
/* 537 */ 2,
/* 538 */ 0,
/* 539 */ 50400,
/* 540 */ 50342,
/* 541 */ 1,
/* 542 */ 1,
/* 543 */ 50059,
/* 544 */ -538,
/* 545 */ 1,
/* 546 */ 552,
/* 547 */ 50044,
/* 548 */ 1,
/* 549 */ 1,
/* 550 */ -546,
/* 551 */ 1,
/* 552 */ 0,
/* 553 */ -546,
/* 554 */ 2,
/* 555 */ 0,
/* 556 */ 1,
/* 557 */ 1,
/* 558 */ 1,
/* 559 */ -555,
/* 560 */ 1,
/* 561 */ 567,
/* 562 */ 50044,
/* 563 */ 1,
/* 564 */ 1,
/* 565 */ -561,
/* 566 */ 1,
/* 567 */ 0,
/* 568 */ -561,
/* 569 */ 2,
/* 570 */ 575,
/* 571 */ 50257,
/* 572 */ 1,
/* 573 */ -570,
/* 574 */ 1,
/* 575 */ 0,
/* 576 */ 1,
/* 577 */ -570,
/* 578 */ 2,
/* 579 */ 585,
/* 580 */ 50047,
/* 581 */ 50257,
/* 582 */ 1,
/* 583 */ -579,
/* 584 */ 1,
/* 585 */ 0,
/* 586 */ -579,
/* 587 */ 2,
/* 588 */ 0,
/* 589 */ 1,
/* 590 */ -588,
/* 591 */ 1,
/* 592 */ 0,
/* 593 */ 1,
/* 594 */ -592,
/* 595 */ 1,
/* 596 */ 0,
/* 597 */ 1,
/* 598 */ 1,
/* 599 */ -596,
/* 600 */ 1,
/* 601 */ 606,
/* 602 */ 1,
/* 603 */ 1,
/* 604 */ -601,
/* 605 */ 1,
/* 606 */ 611,
/* 607 */ 50045,
/* 608 */ 1,
/* 609 */ -601,
/* 610 */ 2,
/* 611 */ 616,
/* 612 */ 50061,
/* 613 */ 1,
/* 614 */ -601,
/* 615 */ 3,
/* 616 */ 0,
/* 617 */ 1,
/* 618 */ 50037,
/* 619 */ -601,
/* 620 */ 4,
/* 621 */ 625,
/* 622 */ 50043,
/* 623 */ -621,
/* 624 */ 1,
/* 625 */ 0,
/* 626 */ -621,
/* 627 */ 2,
/* 628 */ 634,
/* 629 */ 50040,
/* 630 */ 1,
/* 631 */ 50041,
/* 632 */ -628,
/* 633 */ 1,
/* 634 */ 0,
/* 635 */ -628,
/* 636 */ 2,
/* 637 */ 642,
/* 638 */ 50387,
/* 639 */ 1,
/* 640 */ -637,
/* 641 */ 1,
/* 642 */ 647,
/* 643 */ 50384,
/* 644 */ 1,
/* 645 */ -637,
/* 646 */ 2,
/* 647 */ 0,
/* 648 */ 50391,
/* 649 */ 1,
/* 650 */ -637,
/* 651 */ 3,
/* 652 */ 0,
/* 653 */ 50388,
/* 654 */ 50257,
/* 655 */ 50058,
/* 656 */ 1,
/* 657 */ 1,
/* 658 */ 50123,
/* 659 */ 1,
/* 660 */ 50125,
/* 661 */ -652,
/* 662 */ 1,
/* 663 */ 669,
/* 664 */ 50044,
/* 665 */ 1,
/* 666 */ 1,
/* 667 */ -663,
/* 668 */ 1,
/* 669 */ 0,
/* 670 */ -663,
/* 671 */ 2,
/* 672 */ 677,
/* 673 */ 50044,
/* 674 */ 1,
/* 675 */ -672,
/* 676 */ 1,
/* 677 */ 0,
/* 678 */ -672,
/* 679 */ 2,
/* 680 */ 685,
/* 681 */ 1,
/* 682 */ 1,
/* 683 */ -680,
/* 684 */ 1,
/* 685 */ 0,
/* 686 */ -680,
/* 687 */ 2,
/* 688 */ 695,
/* 689 */ 50326,
/* 690 */ 1,
/* 691 */ 50257,
/* 692 */ 50059,
/* 693 */ -688,
/* 694 */ 1,
/* 695 */ 702,
/* 696 */ 50325,
/* 697 */ 1,
/* 698 */ 1,
/* 699 */ 50059,
/* 700 */ -688,
/* 701 */ 2,
/* 702 */ 709,
/* 703 */ 1,
/* 704 */ 1,
/* 705 */ 1,
/* 706 */ 50059,
/* 707 */ -688,
/* 708 */ 3,
/* 709 */ 718,
/* 710 */ 1,
/* 711 */ 1,
/* 712 */ 1,
/* 713 */ 50047,
/* 714 */ 1,
/* 715 */ 50059,
/* 716 */ -688,
/* 717 */ 4,
/* 718 */ 726,
/* 719 */ 50450,
/* 720 */ 1,
/* 721 */ 1,
/* 722 */ 1,
/* 723 */ 50059,
/* 724 */ -688,
/* 725 */ 5,
/* 726 */ 733,
/* 727 */ 50364,
/* 728 */ 1,
/* 729 */ 1,
/* 730 */ 50059,
/* 731 */ -688,
/* 732 */ 6,
/* 733 */ 740,
/* 734 */ 50365,
/* 735 */ 1,
/* 736 */ 1,
/* 737 */ 50059,
/* 738 */ -688,
/* 739 */ 7,
/* 740 */ 748,
/* 741 */ 1,
/* 742 */ 50366,
/* 743 */ 1,
/* 744 */ 1,
/* 745 */ 50059,
/* 746 */ -688,
/* 747 */ 8,
/* 748 */ 752,
/* 749 */ 1,
/* 750 */ -688,
/* 751 */ 9,
/* 752 */ 760,
/* 753 */ 50440,
/* 754 */ 50091,
/* 755 */ 1,
/* 756 */ 50093,
/* 757 */ 50059,
/* 758 */ -688,
/* 759 */ 10,
/* 760 */ 770,
/* 761 */ 50440,
/* 762 */ 50091,
/* 763 */ 1,
/* 764 */ 50044,
/* 765 */ 1,
/* 766 */ 50093,
/* 767 */ 50059,
/* 768 */ -688,
/* 769 */ 11,
/* 770 */ 782,
/* 771 */ 50440,
/* 772 */ 50091,
/* 773 */ 1,
/* 774 */ 50044,
/* 775 */ 1,
/* 776 */ 50044,
/* 777 */ 1,
/* 778 */ 50093,
/* 779 */ 50059,
/* 780 */ -688,
/* 781 */ 12,
/* 782 */ 796,
/* 783 */ 50440,
/* 784 */ 50091,
/* 785 */ 1,
/* 786 */ 50044,
/* 787 */ 1,
/* 788 */ 50044,
/* 789 */ 1,
/* 790 */ 50044,
/* 791 */ 1,
/* 792 */ 50093,
/* 793 */ 50059,
/* 794 */ -688,
/* 795 */ 13,
/* 796 */ 803,
/* 797 */ 50424,
/* 798 */ 1,
/* 799 */ 1,
/* 800 */ 50059,
/* 801 */ -688,
/* 802 */ 14,
/* 803 */ 810,
/* 804 */ 50369,
/* 805 */ 1,
/* 806 */ 1,
/* 807 */ 50059,
/* 808 */ -688,
/* 809 */ 15,
/* 810 */ 818,
/* 811 */ 50296,
/* 812 */ 1,
/* 813 */ 1,
/* 814 */ 1,
/* 815 */ 50059,
/* 816 */ -688,
/* 817 */ 16,
/* 818 */ 824,
/* 819 */ 50294,
/* 820 */ 50257,
/* 821 */ 50059,
/* 822 */ -688,
/* 823 */ 17,
/* 824 */ 830,
/* 825 */ 50294,
/* 826 */ 1,
/* 827 */ 50059,
/* 828 */ -688,
/* 829 */ 18,
/* 830 */ 837,
/* 831 */ 50323,
/* 832 */ 1,
/* 833 */ 1,
/* 834 */ 50059,
/* 835 */ -688,
/* 836 */ 19,
/* 837 */ 844,
/* 838 */ 50292,
/* 839 */ 1,
/* 840 */ 1,
/* 841 */ 50059,
/* 842 */ -688,
/* 843 */ 20,
/* 844 */ 851,
/* 845 */ 50381,
/* 846 */ 1,
/* 847 */ 1,
/* 848 */ 50059,
/* 849 */ -688,
/* 850 */ 21,
/* 851 */ 859,
/* 852 */ 50346,
/* 853 */ 1,
/* 854 */ 1,
/* 855 */ 1,
/* 856 */ 50059,
/* 857 */ -688,
/* 858 */ 22,
/* 859 */ 866,
/* 860 */ 50350,
/* 861 */ 1,
/* 862 */ 1,
/* 863 */ 50059,
/* 864 */ -688,
/* 865 */ 23,
/* 866 */ 872,
/* 867 */ 1,
/* 868 */ 1,
/* 869 */ 50059,
/* 870 */ -688,
/* 871 */ 24,
/* 872 */ 879,
/* 873 */ 50431,
/* 874 */ 1,
/* 875 */ 1,
/* 876 */ 50059,
/* 877 */ -688,
/* 878 */ 25,
/* 879 */ 887,
/* 880 */ 50435,
/* 881 */ 1,
/* 882 */ 1,
/* 883 */ 1,
/* 884 */ 50059,
/* 885 */ -688,
/* 886 */ 26,
/* 887 */ 895,
/* 888 */ 50389,
/* 889 */ 1,
/* 890 */ 1,
/* 891 */ 1,
/* 892 */ 50059,
/* 893 */ -688,
/* 894 */ 27,
/* 895 */ 902,
/* 896 */ 50437,
/* 897 */ 1,
/* 898 */ 1,
/* 899 */ 50059,
/* 900 */ -688,
/* 901 */ 28,
/* 902 */ 909,
/* 903 */ 50330,
/* 904 */ 1,
/* 905 */ 1,
/* 906 */ 50059,
/* 907 */ -688,
/* 908 */ 29,
/* 909 */ 916,
/* 910 */ 50409,
/* 911 */ 1,
/* 912 */ 1,
/* 913 */ 50059,
/* 914 */ -688,
/* 915 */ 30,
/* 916 */ 920,
/* 917 */ 1,
/* 918 */ -688,
/* 919 */ 31,
/* 920 */ 924,
/* 921 */ 1,
/* 922 */ -688,
/* 923 */ 32,
/* 924 */ 928,
/* 925 */ 1,
/* 926 */ -688,
/* 927 */ 33,
/* 928 */ 0,
/* 929 */ 1,
/* 930 */ -688,
/* 931 */ 34,
/* 932 */ 936,
/* 933 */ 50058,
/* 934 */ -932,
/* 935 */ 1,
/* 936 */ 0,
/* 937 */ -932,
/* 938 */ 2,
/* 939 */ 943,
/* 940 */ 50058,
/* 941 */ -939,
/* 942 */ 1,
/* 943 */ 0,
/* 944 */ -939,
/* 945 */ 2,
/* 946 */ 950,
/* 947 */ 50374,
/* 948 */ -946,
/* 949 */ 1,
/* 950 */ 0,
/* 951 */ 50307,
/* 952 */ -946,
/* 953 */ 2,
/* 954 */ 958,
/* 955 */ 50058,
/* 956 */ -954,
/* 957 */ 1,
/* 958 */ 0,
/* 959 */ -954,
/* 960 */ 2,
/* 961 */ 965,
/* 962 */ 50374,
/* 963 */ -961,
/* 964 */ 1,
/* 965 */ 0,
/* 966 */ 50307,
/* 967 */ -961,
/* 968 */ 2,
/* 969 */ 973,
/* 970 */ 50058,
/* 971 */ -969,
/* 972 */ 1,
/* 973 */ 0,
/* 974 */ -969,
/* 975 */ 2,
/* 976 */ 980,
/* 977 */ 50058,
/* 978 */ -976,
/* 979 */ 1,
/* 980 */ 0,
/* 981 */ -976,
/* 982 */ 2,
/* 983 */ 989,
/* 984 */ 50044,
/* 985 */ 1,
/* 986 */ 1,
/* 987 */ -983,
/* 988 */ 1,
/* 989 */ 0,
/* 990 */ -983,
/* 991 */ 2,
/* 992 */ 996,
/* 993 */ 50058,
/* 994 */ -992,
/* 995 */ 1,
/* 996 */ 0,
/* 997 */ -992,
/* 998 */ 2,
/* 999 */ 1003,
/* 1000 */ 50058,
/* 1001 */ -999,
/* 1002 */ 1,
/* 1003 */ 0,
/* 1004 */ -999,
/* 1005 */ 2,
/* 1006 */ 1010,
/* 1007 */ 50043,
/* 1008 */ -1006,
/* 1009 */ 1,
/* 1010 */ 0,
/* 1011 */ -1006,
/* 1012 */ 2,
/* 1013 */ 1017,
/* 1014 */ 50058,
/* 1015 */ -1013,
/* 1016 */ 1,
/* 1017 */ 0,
/* 1018 */ -1013,
/* 1019 */ 2,
/* 1020 */ 1024,
/* 1021 */ 50058,
/* 1022 */ -1020,
/* 1023 */ 1,
/* 1024 */ 0,
/* 1025 */ -1020,
/* 1026 */ 2,
/* 1027 */ 1033,
/* 1028 */ 1,
/* 1029 */ 1,
/* 1030 */ 1,
/* 1031 */ -1027,
/* 1032 */ 1,
/* 1033 */ 0,
/* 1034 */ -1027,
/* 1035 */ 2,
/* 1036 */ 1040,
/* 1037 */ 50044,
/* 1038 */ -1036,
/* 1039 */ 1,
/* 1040 */ 0,
/* 1041 */ -1036,
/* 1042 */ 2,
/* 1043 */ 1047,
/* 1044 */ 50058,
/* 1045 */ -1043,
/* 1046 */ 1,
/* 1047 */ 0,
/* 1048 */ -1043,
/* 1049 */ 2,
/* 1050 */ 1056,
/* 1051 */ 1,
/* 1052 */ 1,
/* 1053 */ 1,
/* 1054 */ -1050,
/* 1055 */ 1,
/* 1056 */ 0,
/* 1057 */ -1050,
/* 1058 */ 2,
/* 1059 */ 1063,
/* 1060 */ 50044,
/* 1061 */ -1059,
/* 1062 */ 1,
/* 1063 */ 0,
/* 1064 */ -1059,
/* 1065 */ 2,
/* 1066 */ 1070,
/* 1067 */ 50058,
/* 1068 */ -1066,
/* 1069 */ 1,
/* 1070 */ 0,
/* 1071 */ -1066,
/* 1072 */ 2,
/* 1073 */ 1079,
/* 1074 */ 50044,
/* 1075 */ 1,
/* 1076 */ 1,
/* 1077 */ -1073,
/* 1078 */ 1,
/* 1079 */ 0,
/* 1080 */ -1073,
/* 1081 */ 2,
/* 1082 */ 1086,
/* 1083 */ 50058,
/* 1084 */ -1082,
/* 1085 */ 1,
/* 1086 */ 0,
/* 1087 */ -1082,
/* 1088 */ 2,
/* 1089 */ 1093,
/* 1090 */ 50058,
/* 1091 */ -1089,
/* 1092 */ 1,
/* 1093 */ 0,
/* 1094 */ -1089,
/* 1095 */ 2,
/* 1096 */ 1100,
/* 1097 */ 50058,
/* 1098 */ -1096,
/* 1099 */ 1,
/* 1100 */ 0,
/* 1101 */ -1096,
/* 1102 */ 2,
/* 1103 */ 1107,
/* 1104 */ 50058,
/* 1105 */ -1103,
/* 1106 */ 1,
/* 1107 */ 0,
/* 1108 */ -1103,
/* 1109 */ 2,
/* 1110 */ 1116,
/* 1111 */ 1,
/* 1112 */ 50044,
/* 1113 */ 1,
/* 1114 */ -1110,
/* 1115 */ 1,
/* 1116 */ 0,
/* 1117 */ -1110,
/* 1118 */ 2,
/* 1119 */ 1123,
/* 1120 */ 50058,
/* 1121 */ -1119,
/* 1122 */ 1,
/* 1123 */ 0,
/* 1124 */ -1119,
/* 1125 */ 2,
/* 1126 */ 1131,
/* 1127 */ 1,
/* 1128 */ 1,
/* 1129 */ -1126,
/* 1130 */ 1,
/* 1131 */ 0,
/* 1132 */ -1126,
/* 1133 */ 2,
/* 1134 */ 1140,
/* 1135 */ 50044,
/* 1136 */ 1,
/* 1137 */ 1,
/* 1138 */ -1134,
/* 1139 */ 1,
/* 1140 */ 0,
/* 1141 */ -1134,
/* 1142 */ 2,
/* 1143 */ 1147,
/* 1144 */ 50058,
/* 1145 */ -1143,
/* 1146 */ 1,
/* 1147 */ 0,
/* 1148 */ -1143,
/* 1149 */ 2,
/* 1150 */ 1154,
/* 1151 */ 50058,
/* 1152 */ -1150,
/* 1153 */ 1,
/* 1154 */ 0,
/* 1155 */ -1150,
/* 1156 */ 2,
/* 1157 */ 1161,
/* 1158 */ 50037,
/* 1159 */ -1157,
/* 1160 */ 1,
/* 1161 */ 0,
/* 1162 */ -1157,
/* 1163 */ 2,
/* 1164 */ 1168,
/* 1165 */ 50058,
/* 1166 */ -1164,
/* 1167 */ 1,
/* 1168 */ 0,
/* 1169 */ -1164,
/* 1170 */ 2,
/* 1171 */ 1175,
/* 1172 */ 50037,
/* 1173 */ -1171,
/* 1174 */ 1,
/* 1175 */ 0,
/* 1176 */ -1171,
/* 1177 */ 2,
/* 1178 */ 1182,
/* 1179 */ 50058,
/* 1180 */ -1178,
/* 1181 */ 1,
/* 1182 */ 0,
/* 1183 */ -1178,
/* 1184 */ 2,
/* 1185 */ 1191,
/* 1186 */ 1,
/* 1187 */ 1,
/* 1188 */ 1,
/* 1189 */ -1185,
/* 1190 */ 1,
/* 1191 */ 0,
/* 1192 */ -1185,
/* 1193 */ 2,
/* 1194 */ 1198,
/* 1195 */ 50044,
/* 1196 */ -1194,
/* 1197 */ 1,
/* 1198 */ 0,
/* 1199 */ -1194,
/* 1200 */ 2,
/* 1201 */ 1205,
/* 1202 */ 50058,
/* 1203 */ -1201,
/* 1204 */ 1,
/* 1205 */ 0,
/* 1206 */ -1201,
/* 1207 */ 2,
/* 1208 */ 1214,
/* 1209 */ 1,
/* 1210 */ 1,
/* 1211 */ 1,
/* 1212 */ -1208,
/* 1213 */ 1,
/* 1214 */ 0,
/* 1215 */ -1208,
/* 1216 */ 2,
/* 1217 */ 1221,
/* 1218 */ 50044,
/* 1219 */ -1217,
/* 1220 */ 1,
/* 1221 */ 0,
/* 1222 */ -1217,
/* 1223 */ 2,
/* 1224 */ 1228,
/* 1225 */ 50058,
/* 1226 */ -1224,
/* 1227 */ 1,
/* 1228 */ 0,
/* 1229 */ -1224,
/* 1230 */ 2,
/* 1231 */ 1237,
/* 1232 */ 1,
/* 1233 */ 1,
/* 1234 */ 1,
/* 1235 */ -1231,
/* 1236 */ 1,
/* 1237 */ 0,
/* 1238 */ -1231,
/* 1239 */ 2,
/* 1240 */ 1244,
/* 1241 */ 50044,
/* 1242 */ -1240,
/* 1243 */ 1,
/* 1244 */ 0,
/* 1245 */ -1240,
/* 1246 */ 2,
/* 1247 */ 1251,
/* 1248 */ 50275,
/* 1249 */ -1247,
/* 1250 */ 1,
/* 1251 */ 1256,
/* 1252 */ 50303,
/* 1253 */ 50275,
/* 1254 */ -1247,
/* 1255 */ 2,
/* 1256 */ 1261,
/* 1257 */ 50376,
/* 1258 */ 50275,
/* 1259 */ -1247,
/* 1260 */ 3,
/* 1261 */ 0,
/* 1262 */ 50320,
/* 1263 */ 50275,
/* 1264 */ -1247,
/* 1265 */ 4,
/* 1266 */ 1276,
/* 1267 */ 1,
/* 1268 */ 50353,
/* 1269 */ 1,
/* 1270 */ 1,
/* 1271 */ 1,
/* 1272 */ 1,
/* 1273 */ 1,
/* 1274 */ -1266,
/* 1275 */ 1,
/* 1276 */ 0,
/* 1277 */ 1,
/* 1278 */ 50353,
/* 1279 */ 1,
/* 1280 */ 1,
/* 1281 */ -1266,
/* 1282 */ 2,
/* 1283 */ 1287,
/* 1284 */ 1,
/* 1285 */ -1283,
/* 1286 */ 1,
/* 1287 */ 1292,
/* 1288 */ 1,
/* 1289 */ 50037,
/* 1290 */ -1283,
/* 1291 */ 2,
/* 1292 */ 0,
/* 1293 */ -1283,
/* 1294 */ 3,
/* 1295 */ 1299,
/* 1296 */ 1,
/* 1297 */ -1295,
/* 1298 */ 1,
/* 1299 */ 0,
/* 1300 */ -1295,
/* 1301 */ 2,
/* 1302 */ 1306,
/* 1303 */ 50319,
/* 1304 */ -1302,
/* 1305 */ 1,
/* 1306 */ 0,
/* 1307 */ -1302,
/* 1308 */ 2,
/* 1309 */ 1316,
/* 1310 */ 50040,
/* 1311 */ 1,
/* 1312 */ 1,
/* 1313 */ 50041,
/* 1314 */ -1309,
/* 1315 */ 1,
/* 1316 */ 0,
/* 1317 */ -1309,
/* 1318 */ 2,
/* 1319 */ 1323,
/* 1320 */ 50322,
/* 1321 */ -1319,
/* 1322 */ 1,
/* 1323 */ 1327,
/* 1324 */ 50307,
/* 1325 */ -1319,
/* 1326 */ 2,
/* 1327 */ 0,
/* 1328 */ 50374,
/* 1329 */ -1319,
/* 1330 */ 3,
/* 1331 */ 1340,
/* 1332 */ 50288,
/* 1333 */ 1,
/* 1334 */ 1,
/* 1335 */ 1,
/* 1336 */ 1,
/* 1337 */ 1,
/* 1338 */ -1331,
/* 1339 */ 1,
/* 1340 */ 0,
/* 1341 */ -1331,
/* 1342 */ 2,
/* 1343 */ 1347,
/* 1344 */ 1,
/* 1345 */ -1343,
/* 1346 */ 1,
/* 1347 */ 0,
/* 1348 */ -1343,
/* 1349 */ 2,
/* 1350 */ 1354,
/* 1351 */ 1,
/* 1352 */ -1350,
/* 1353 */ 1,
/* 1354 */ 0,
/* 1355 */ -1350,
/* 1356 */ 2,
/* 1357 */ 1361,
/* 1358 */ 50319,
/* 1359 */ -1357,
/* 1360 */ 1,
/* 1361 */ 0,
/* 1362 */ -1357,
/* 1363 */ 2,
/* 1364 */ 1371,
/* 1365 */ 50040,
/* 1366 */ 1,
/* 1367 */ 1,
/* 1368 */ 50041,
/* 1369 */ -1364,
/* 1370 */ 1,
/* 1371 */ 0,
/* 1372 */ -1364,
/* 1373 */ 2,
/* 1374 */ 1378,
/* 1375 */ 50322,
/* 1376 */ -1374,
/* 1377 */ 1,
/* 1378 */ 1382,
/* 1379 */ 50307,
/* 1380 */ -1374,
/* 1381 */ 2,
/* 1382 */ 0,
/* 1383 */ 50374,
/* 1384 */ -1374,
/* 1385 */ 3,
/* 1386 */ 1390,
/* 1387 */ 1,
/* 1388 */ -1386,
/* 1389 */ 1,
/* 1390 */ 0,
/* 1391 */ -1386,
/* 1392 */ 2,
/* 1393 */ 1398,
/* 1394 */ 1,
/* 1395 */ 1,
/* 1396 */ -1393,
/* 1397 */ 1,
/* 1398 */ 0,
/* 1399 */ 50126,
/* 1400 */ 1,
/* 1401 */ 1,
/* 1402 */ -1393,
/* 1403 */ 2,
/* 1404 */ 1408,
/* 1405 */ 50044,
/* 1406 */ -1404,
/* 1407 */ 1,
/* 1408 */ 0,
/* 1409 */ -1404,
/* 1410 */ 2,
/* 1411 */ 1415,
/* 1412 */ 50044,
/* 1413 */ -1411,
/* 1414 */ 1,
/* 1415 */ 0,
/* 1416 */ -1411,
/* 1417 */ 2,
/* 1418 */ 0,
/* 1419 */ 50277,
/* 1420 */ 1,
/* 1421 */ 1,
/* 1422 */ -1418,
/* 1423 */ 1,
/* 1424 */ 1428,
/* 1425 */ 50058,
/* 1426 */ -1424,
/* 1427 */ 1,
/* 1428 */ 0,
/* 1429 */ -1424,
/* 1430 */ 2,
/* 1431 */ 0,
/* 1432 */ 50372,
/* 1433 */ 50257,
/* 1434 */ 50058,
/* 1435 */ 1,
/* 1436 */ 50123,
/* 1437 */ 1,
/* 1438 */ 50125,
/* 1439 */ -1431,
/* 1440 */ 1,
/* 1441 */ 1446,
/* 1442 */ 1,
/* 1443 */ 1,
/* 1444 */ -1441,
/* 1445 */ 1,
/* 1446 */ 0,
/* 1447 */ -1441,
/* 1448 */ 2,
/* 1449 */ 1456,
/* 1450 */ 50326,
/* 1451 */ 1,
/* 1452 */ 50257,
/* 1453 */ 50059,
/* 1454 */ -1449,
/* 1455 */ 1,
/* 1456 */ 1463,
/* 1457 */ 50374,
/* 1458 */ 1,
/* 1459 */ 1,
/* 1460 */ 50059,
/* 1461 */ -1449,
/* 1462 */ 2,
/* 1463 */ 1467,
/* 1464 */ 1,
/* 1465 */ -1449,
/* 1466 */ 3,
/* 1467 */ 1474,
/* 1468 */ 50430,
/* 1469 */ 1,
/* 1470 */ 1,
/* 1471 */ 50059,
/* 1472 */ -1449,
/* 1473 */ 4,
/* 1474 */ 1481,
/* 1475 */ 50373,
/* 1476 */ 1,
/* 1477 */ 1,
/* 1478 */ 50059,
/* 1479 */ -1449,
/* 1480 */ 5,
/* 1481 */ 1488,
/* 1482 */ 50316,
/* 1483 */ 1,
/* 1484 */ 1,
/* 1485 */ 50059,
/* 1486 */ -1449,
/* 1487 */ 6,
/* 1488 */ 1495,
/* 1489 */ 50452,
/* 1490 */ 1,
/* 1491 */ 1,
/* 1492 */ 50059,
/* 1493 */ -1449,
/* 1494 */ 7,
/* 1495 */ 1502,
/* 1496 */ 50284,
/* 1497 */ 1,
/* 1498 */ 1,
/* 1499 */ 50059,
/* 1500 */ -1449,
/* 1501 */ 8,
/* 1502 */ 1509,
/* 1503 */ 50435,
/* 1504 */ 1,
/* 1505 */ 1,
/* 1506 */ 50059,
/* 1507 */ -1449,
/* 1508 */ 9,
/* 1509 */ 1517,
/* 1510 */ 50435,
/* 1511 */ 1,
/* 1512 */ 1,
/* 1513 */ 50037,
/* 1514 */ 50059,
/* 1515 */ -1449,
/* 1516 */ 10,
/* 1517 */ 1526,
/* 1518 */ 50435,
/* 1519 */ 1,
/* 1520 */ 50043,
/* 1521 */ 1,
/* 1522 */ 50037,
/* 1523 */ 50059,
/* 1524 */ -1449,
/* 1525 */ 11,
/* 1526 */ 1533,
/* 1527 */ 50404,
/* 1528 */ 1,
/* 1529 */ 1,
/* 1530 */ 50059,
/* 1531 */ -1449,
/* 1532 */ 12,
/* 1533 */ 1540,
/* 1534 */ 50420,
/* 1535 */ 1,
/* 1536 */ 1,
/* 1537 */ 50059,
/* 1538 */ -1449,
/* 1539 */ 13,
/* 1540 */ 1547,
/* 1541 */ 50383,
/* 1542 */ 1,
/* 1543 */ 1,
/* 1544 */ 50059,
/* 1545 */ -1449,
/* 1546 */ 14,
/* 1547 */ 1555,
/* 1548 */ 50315,
/* 1549 */ 1,
/* 1550 */ 1,
/* 1551 */ 1,
/* 1552 */ 50059,
/* 1553 */ -1449,
/* 1554 */ 15,
/* 1555 */ 1562,
/* 1556 */ 50466,
/* 1557 */ 1,
/* 1558 */ 1,
/* 1559 */ 50059,
/* 1560 */ -1449,
/* 1561 */ 16,
/* 1562 */ 1569,
/* 1563 */ 50431,
/* 1564 */ 1,
/* 1565 */ 1,
/* 1566 */ 50059,
/* 1567 */ -1449,
/* 1568 */ 17,
/* 1569 */ 1576,
/* 1570 */ 50396,
/* 1571 */ 1,
/* 1572 */ 1,
/* 1573 */ 50059,
/* 1574 */ -1449,
/* 1575 */ 18,
/* 1576 */ 1583,
/* 1577 */ 50323,
/* 1578 */ 1,
/* 1579 */ 1,
/* 1580 */ 50059,
/* 1581 */ -1449,
/* 1582 */ 19,
/* 1583 */ 1590,
/* 1584 */ 50312,
/* 1585 */ 1,
/* 1586 */ 1,
/* 1587 */ 50059,
/* 1588 */ -1449,
/* 1589 */ 20,
/* 1590 */ 1597,
/* 1591 */ 50407,
/* 1592 */ 1,
/* 1593 */ 1,
/* 1594 */ 50059,
/* 1595 */ -1449,
/* 1596 */ 21,
/* 1597 */ 1604,
/* 1598 */ 50292,
/* 1599 */ 1,
/* 1600 */ 1,
/* 1601 */ 50059,
/* 1602 */ -1449,
/* 1603 */ 22,
/* 1604 */ 1611,
/* 1605 */ 50365,
/* 1606 */ 1,
/* 1607 */ 1,
/* 1608 */ 50059,
/* 1609 */ -1449,
/* 1610 */ 23,
/* 1611 */ 1618,
/* 1612 */ 50437,
/* 1613 */ 1,
/* 1614 */ 1,
/* 1615 */ 50059,
/* 1616 */ -1449,
/* 1617 */ 24,
/* 1618 */ 1625,
/* 1619 */ 50349,
/* 1620 */ 1,
/* 1621 */ 1,
/* 1622 */ 50059,
/* 1623 */ -1449,
/* 1624 */ 25,
/* 1625 */ 1632,
/* 1626 */ 50350,
/* 1627 */ 1,
/* 1628 */ 1,
/* 1629 */ 50059,
/* 1630 */ -1449,
/* 1631 */ 26,
/* 1632 */ 1639,
/* 1633 */ 50361,
/* 1634 */ 1,
/* 1635 */ 1,
/* 1636 */ 50059,
/* 1637 */ -1449,
/* 1638 */ 27,
/* 1639 */ 1646,
/* 1640 */ 50304,
/* 1641 */ 1,
/* 1642 */ 1,
/* 1643 */ 50059,
/* 1644 */ -1449,
/* 1645 */ 28,
/* 1646 */ 1653,
/* 1647 */ 50468,
/* 1648 */ 1,
/* 1649 */ 1,
/* 1650 */ 50059,
/* 1651 */ -1449,
/* 1652 */ 29,
/* 1653 */ 1660,
/* 1654 */ 50467,
/* 1655 */ 1,
/* 1656 */ 1,
/* 1657 */ 50059,
/* 1658 */ -1449,
/* 1659 */ 30,
/* 1660 */ 1667,
/* 1661 */ 50379,
/* 1662 */ 1,
/* 1663 */ 1,
/* 1664 */ 50059,
/* 1665 */ -1449,
/* 1666 */ 31,
/* 1667 */ 1674,
/* 1668 */ 50456,
/* 1669 */ 1,
/* 1670 */ 1,
/* 1671 */ 50059,
/* 1672 */ -1449,
/* 1673 */ 32,
/* 1674 */ 1681,
/* 1675 */ 50313,
/* 1676 */ 1,
/* 1677 */ 1,
/* 1678 */ 50059,
/* 1679 */ -1449,
/* 1680 */ 33,
/* 1681 */ 1689,
/* 1682 */ 50376,
/* 1683 */ 50420,
/* 1684 */ 1,
/* 1685 */ 1,
/* 1686 */ 50059,
/* 1687 */ -1449,
/* 1688 */ 34,
/* 1689 */ 1696,
/* 1690 */ 50375,
/* 1691 */ 1,
/* 1692 */ 1,
/* 1693 */ 50059,
/* 1694 */ -1449,
/* 1695 */ 35,
/* 1696 */ 1703,
/* 1697 */ 50355,
/* 1698 */ 1,
/* 1699 */ 1,
/* 1700 */ 50059,
/* 1701 */ -1449,
/* 1702 */ 36,
/* 1703 */ 1707,
/* 1704 */ 1,
/* 1705 */ -1449,
/* 1706 */ 37,
/* 1707 */ 1711,
/* 1708 */ 1,
/* 1709 */ -1449,
/* 1710 */ 38,
/* 1711 */ 0,
/* 1712 */ 1,
/* 1713 */ -1449,
/* 1714 */ 39,
/* 1715 */ 1719,
/* 1716 */ 50058,
/* 1717 */ -1715,
/* 1718 */ 1,
/* 1719 */ 0,
/* 1720 */ -1715,
/* 1721 */ 2,
/* 1722 */ 1726,
/* 1723 */ 50058,
/* 1724 */ -1722,
/* 1725 */ 1,
/* 1726 */ 0,
/* 1727 */ -1722,
/* 1728 */ 2,
/* 1729 */ 1733,
/* 1730 */ 50058,
/* 1731 */ -1729,
/* 1732 */ 1,
/* 1733 */ 0,
/* 1734 */ -1729,
/* 1735 */ 2,
/* 1736 */ 1740,
/* 1737 */ 50058,
/* 1738 */ -1736,
/* 1739 */ 1,
/* 1740 */ 0,
/* 1741 */ -1736,
/* 1742 */ 2,
/* 1743 */ 1747,
/* 1744 */ 50058,
/* 1745 */ -1743,
/* 1746 */ 1,
/* 1747 */ 0,
/* 1748 */ -1743,
/* 1749 */ 2,
/* 1750 */ 1754,
/* 1751 */ 50058,
/* 1752 */ -1750,
/* 1753 */ 1,
/* 1754 */ 0,
/* 1755 */ -1750,
/* 1756 */ 2,
/* 1757 */ 1761,
/* 1758 */ 50058,
/* 1759 */ -1757,
/* 1760 */ 1,
/* 1761 */ 0,
/* 1762 */ -1757,
/* 1763 */ 2,
/* 1764 */ 1768,
/* 1765 */ 50058,
/* 1766 */ -1764,
/* 1767 */ 1,
/* 1768 */ 0,
/* 1769 */ -1764,
/* 1770 */ 2,
/* 1771 */ 1775,
/* 1772 */ 50058,
/* 1773 */ -1771,
/* 1774 */ 1,
/* 1775 */ 0,
/* 1776 */ -1771,
/* 1777 */ 2,
/* 1778 */ 1782,
/* 1779 */ 50058,
/* 1780 */ -1778,
/* 1781 */ 1,
/* 1782 */ 0,
/* 1783 */ -1778,
/* 1784 */ 2,
/* 1785 */ 1789,
/* 1786 */ 50058,
/* 1787 */ -1785,
/* 1788 */ 1,
/* 1789 */ 0,
/* 1790 */ -1785,
/* 1791 */ 2,
/* 1792 */ 1796,
/* 1793 */ 50058,
/* 1794 */ -1792,
/* 1795 */ 1,
/* 1796 */ 0,
/* 1797 */ -1792,
/* 1798 */ 2,
/* 1799 */ 1803,
/* 1800 */ 50058,
/* 1801 */ -1799,
/* 1802 */ 1,
/* 1803 */ 0,
/* 1804 */ -1799,
/* 1805 */ 2,
/* 1806 */ 1810,
/* 1807 */ 50058,
/* 1808 */ -1806,
/* 1809 */ 1,
/* 1810 */ 0,
/* 1811 */ -1806,
/* 1812 */ 2,
/* 1813 */ 1817,
/* 1814 */ 1,
/* 1815 */ -1813,
/* 1816 */ 1,
/* 1817 */ 0,
/* 1818 */ -1813,
/* 1819 */ 2,
/* 1820 */ 1824,
/* 1821 */ 50058,
/* 1822 */ -1820,
/* 1823 */ 1,
/* 1824 */ 0,
/* 1825 */ -1820,
/* 1826 */ 2,
/* 1827 */ 1831,
/* 1828 */ 50058,
/* 1829 */ -1827,
/* 1830 */ 1,
/* 1831 */ 0,
/* 1832 */ -1827,
/* 1833 */ 2,
/* 1834 */ 1838,
/* 1835 */ 50058,
/* 1836 */ -1834,
/* 1837 */ 1,
/* 1838 */ 0,
/* 1839 */ -1834,
/* 1840 */ 2,
/* 1841 */ 1845,
/* 1842 */ 50058,
/* 1843 */ -1841,
/* 1844 */ 1,
/* 1845 */ 0,
/* 1846 */ -1841,
/* 1847 */ 2,
/* 1848 */ 1852,
/* 1849 */ 50058,
/* 1850 */ -1848,
/* 1851 */ 1,
/* 1852 */ 0,
/* 1853 */ -1848,
/* 1854 */ 2,
/* 1855 */ 1859,
/* 1856 */ 50058,
/* 1857 */ -1855,
/* 1858 */ 1,
/* 1859 */ 0,
/* 1860 */ -1855,
/* 1861 */ 2,
/* 1862 */ 1866,
/* 1863 */ 50058,
/* 1864 */ -1862,
/* 1865 */ 1,
/* 1866 */ 0,
/* 1867 */ -1862,
/* 1868 */ 2,
/* 1869 */ 1874,
/* 1870 */ 1,
/* 1871 */ 1,
/* 1872 */ -1869,
/* 1873 */ 1,
/* 1874 */ 0,
/* 1875 */ -1869,
/* 1876 */ 2,
/* 1877 */ 1881,
/* 1878 */ 50058,
/* 1879 */ -1877,
/* 1880 */ 1,
/* 1881 */ 0,
/* 1882 */ -1877,
/* 1883 */ 2,
/* 1884 */ 1888,
/* 1885 */ 50058,
/* 1886 */ -1884,
/* 1887 */ 1,
/* 1888 */ 0,
/* 1889 */ -1884,
/* 1890 */ 2,
/* 1891 */ 1897,
/* 1892 */ 1,
/* 1893 */ 1,
/* 1894 */ 1,
/* 1895 */ -1891,
/* 1896 */ 1,
/* 1897 */ 0,
/* 1898 */ -1891,
/* 1899 */ 2,
/* 1900 */ 1904,
/* 1901 */ 50044,
/* 1902 */ -1900,
/* 1903 */ 1,
/* 1904 */ 0,
/* 1905 */ -1900,
/* 1906 */ 2,
/* 1907 */ 1911,
/* 1908 */ 50058,
/* 1909 */ -1907,
/* 1910 */ 1,
/* 1911 */ 0,
/* 1912 */ -1907,
/* 1913 */ 2,
/* 1914 */ 1920,
/* 1915 */ 1,
/* 1916 */ 1,
/* 1917 */ 1,
/* 1918 */ -1914,
/* 1919 */ 1,
/* 1920 */ 0,
/* 1921 */ -1914,
/* 1922 */ 2,
/* 1923 */ 1927,
/* 1924 */ 50044,
/* 1925 */ -1923,
/* 1926 */ 1,
/* 1927 */ 0,
/* 1928 */ -1923,
/* 1929 */ 2,
/* 1930 */ 1934,
/* 1931 */ 50058,
/* 1932 */ -1930,
/* 1933 */ 1,
/* 1934 */ 0,
/* 1935 */ -1930,
/* 1936 */ 2,
/* 1937 */ 1942,
/* 1938 */ 1,
/* 1939 */ 1,
/* 1940 */ -1937,
/* 1941 */ 1,
/* 1942 */ 0,
/* 1943 */ -1937,
/* 1944 */ 2,
/* 1945 */ 1949,
/* 1946 */ 50058,
/* 1947 */ -1945,
/* 1948 */ 1,
/* 1949 */ 0,
/* 1950 */ -1945,
/* 1951 */ 2,
/* 1952 */ 1958,
/* 1953 */ 1,
/* 1954 */ 1,
/* 1955 */ 1,
/* 1956 */ -1952,
/* 1957 */ 1,
/* 1958 */ 0,
/* 1959 */ -1952,
/* 1960 */ 2,
/* 1961 */ 1965,
/* 1962 */ 50044,
/* 1963 */ -1961,
/* 1964 */ 1,
/* 1965 */ 1969,
/* 1966 */ 50124,
/* 1967 */ -1961,
/* 1968 */ 2,
/* 1969 */ 0,
/* 1970 */ -1961,
/* 1971 */ 3,
/* 1972 */ 1976,
/* 1973 */ 50058,
/* 1974 */ -1972,
/* 1975 */ 1,
/* 1976 */ 0,
/* 1977 */ -1972,
/* 1978 */ 2,
/* 1979 */ 1983,
/* 1980 */ 50058,
/* 1981 */ -1979,
/* 1982 */ 1,
/* 1983 */ 0,
/* 1984 */ -1979,
/* 1985 */ 2,
/* 1986 */ 1990,
/* 1987 */ 50058,
/* 1988 */ -1986,
/* 1989 */ 1,
/* 1990 */ 0,
/* 1991 */ -1986,
/* 1992 */ 2,
/* 1993 */ 1997,
/* 1994 */ 50058,
/* 1995 */ -1993,
/* 1996 */ 1,
/* 1997 */ 0,
/* 1998 */ -1993,
/* 1999 */ 2,
/* 2000 */ 2004,
/* 2001 */ 50058,
/* 2002 */ -2000,
/* 2003 */ 1,
/* 2004 */ 0,
/* 2005 */ -2000,
/* 2006 */ 2,
/* 2007 */ 2011,
/* 2008 */ 50058,
/* 2009 */ -2007,
/* 2010 */ 1,
/* 2011 */ 0,
/* 2012 */ -2007,
/* 2013 */ 2,
/* 2014 */ 2018,
/* 2015 */ 50058,
/* 2016 */ -2014,
/* 2017 */ 1,
/* 2018 */ 0,
/* 2019 */ -2014,
/* 2020 */ 2,
/* 2021 */ 2025,
/* 2022 */ 50058,
/* 2023 */ -2021,
/* 2024 */ 1,
/* 2025 */ 0,
/* 2026 */ -2021,
/* 2027 */ 2,
/* 2028 */ 2032,
/* 2029 */ 50058,
/* 2030 */ -2028,
/* 2031 */ 1,
/* 2032 */ 0,
/* 2033 */ -2028,
/* 2034 */ 2,
/* 2035 */ 2040,
/* 2036 */ 1,
/* 2037 */ 1,
/* 2038 */ -2035,
/* 2039 */ 1,
/* 2040 */ 0,
/* 2041 */ 50276,
/* 2042 */ 1,
/* 2043 */ 1,
/* 2044 */ -2035,
/* 2045 */ 2,
/* 2046 */ 2050,
/* 2047 */ 50044,
/* 2048 */ -2046,
/* 2049 */ 1,
/* 2050 */ 0,
/* 2051 */ -2046,
/* 2052 */ 2,
/* 2053 */ 2057,
/* 2054 */ 50044,
/* 2055 */ -2053,
/* 2056 */ 1,
/* 2057 */ 0,
/* 2058 */ -2053,
/* 2059 */ 2,
/* 2060 */ 2065,
/* 2061 */ 1,
/* 2062 */ 1,
/* 2063 */ -2060,
/* 2064 */ 1,
/* 2065 */ 0,
/* 2066 */ 50126,
/* 2067 */ 1,
/* 2068 */ 1,
/* 2069 */ -2060,
/* 2070 */ 2,
/* 2071 */ 2075,
/* 2072 */ 50044,
/* 2073 */ -2071,
/* 2074 */ 1,
/* 2075 */ 0,
/* 2076 */ -2071,
/* 2077 */ 2,
/* 2078 */ 2082,
/* 2079 */ 50044,
/* 2080 */ -2078,
/* 2081 */ 1,
/* 2082 */ 0,
/* 2083 */ -2078,
/* 2084 */ 2,
/* 2085 */ 0,
/* 2086 */ 50348,
/* 2087 */ 50257,
/* 2088 */ 50058,
/* 2089 */ 1,
/* 2090 */ 50123,
/* 2091 */ 1,
/* 2092 */ 50125,
/* 2093 */ -2085,
/* 2094 */ 1,
/* 2095 */ 2100,
/* 2096 */ 1,
/* 2097 */ 1,
/* 2098 */ -2095,
/* 2099 */ 1,
/* 2100 */ 0,
/* 2101 */ -2095,
/* 2102 */ 2,
/* 2103 */ 2110,
/* 2104 */ 50326,
/* 2105 */ 1,
/* 2106 */ 50257,
/* 2107 */ 50059,
/* 2108 */ -2103,
/* 2109 */ 1,
/* 2110 */ 2117,
/* 2111 */ 50383,
/* 2112 */ 1,
/* 2113 */ 1,
/* 2114 */ 50059,
/* 2115 */ -2103,
/* 2116 */ 2,
/* 2117 */ 2121,
/* 2118 */ 1,
/* 2119 */ -2103,
/* 2120 */ 3,
/* 2121 */ 2129,
/* 2122 */ 50389,
/* 2123 */ 1,
/* 2124 */ 1,
/* 2125 */ 50037,
/* 2126 */ 50059,
/* 2127 */ -2103,
/* 2128 */ 4,
/* 2129 */ 2136,
/* 2130 */ 50365,
/* 2131 */ 1,
/* 2132 */ 1,
/* 2133 */ 50059,
/* 2134 */ -2103,
/* 2135 */ 5,
/* 2136 */ 2143,
/* 2137 */ 50350,
/* 2138 */ 1,
/* 2139 */ 1,
/* 2140 */ 50059,
/* 2141 */ -2103,
/* 2142 */ 6,
/* 2143 */ 2150,
/* 2144 */ 1,
/* 2145 */ 1,
/* 2146 */ 1,
/* 2147 */ 50059,
/* 2148 */ -2103,
/* 2149 */ 7,
/* 2150 */ 2157,
/* 2151 */ 50472,
/* 2152 */ 1,
/* 2153 */ 1,
/* 2154 */ 50059,
/* 2155 */ -2103,
/* 2156 */ 8,
/* 2157 */ 2164,
/* 2158 */ 50344,
/* 2159 */ 1,
/* 2160 */ 1,
/* 2161 */ 50059,
/* 2162 */ -2103,
/* 2163 */ 9,
/* 2164 */ 2168,
/* 2165 */ 1,
/* 2166 */ -2103,
/* 2167 */ 10,
/* 2168 */ 2172,
/* 2169 */ 1,
/* 2170 */ -2103,
/* 2171 */ 11,
/* 2172 */ 0,
/* 2173 */ 1,
/* 2174 */ -2103,
/* 2175 */ 12,
/* 2176 */ 2180,
/* 2177 */ 50058,
/* 2178 */ -2176,
/* 2179 */ 1,
/* 2180 */ 0,
/* 2181 */ -2176,
/* 2182 */ 2,
/* 2183 */ 2187,
/* 2184 */ 50058,
/* 2185 */ -2183,
/* 2186 */ 1,
/* 2187 */ 0,
/* 2188 */ -2183,
/* 2189 */ 2,
/* 2190 */ 2194,
/* 2191 */ 50058,
/* 2192 */ -2190,
/* 2193 */ 1,
/* 2194 */ 0,
/* 2195 */ -2190,
/* 2196 */ 2,
/* 2197 */ 2201,
/* 2198 */ 50058,
/* 2199 */ -2197,
/* 2200 */ 1,
/* 2201 */ 0,
/* 2202 */ -2197,
/* 2203 */ 2,
/* 2204 */ 2208,
/* 2205 */ 50058,
/* 2206 */ -2204,
/* 2207 */ 1,
/* 2208 */ 0,
/* 2209 */ -2204,
/* 2210 */ 2,
/* 2211 */ 2217,
/* 2212 */ 1,
/* 2213 */ 1,
/* 2214 */ 1,
/* 2215 */ -2211,
/* 2216 */ 1,
/* 2217 */ 0,
/* 2218 */ -2211,
/* 2219 */ 2,
/* 2220 */ 2224,
/* 2221 */ 50124,
/* 2222 */ -2220,
/* 2223 */ 1,
/* 2224 */ 2228,
/* 2225 */ 50044,
/* 2226 */ -2220,
/* 2227 */ 2,
/* 2228 */ 0,
/* 2229 */ -2220,
/* 2230 */ 3,
/* 2231 */ 2235,
/* 2232 */ 50448,
/* 2233 */ -2231,
/* 2234 */ 1,
/* 2235 */ 0,
/* 2236 */ 50334,
/* 2237 */ -2231,
/* 2238 */ 2,
/* 2239 */ 2243,
/* 2240 */ 50058,
/* 2241 */ -2239,
/* 2242 */ 1,
/* 2243 */ 0,
/* 2244 */ -2239,
/* 2245 */ 2,
/* 2246 */ 2250,
/* 2247 */ 50058,
/* 2248 */ -2246,
/* 2249 */ 1,
/* 2250 */ 0,
/* 2251 */ -2246,
/* 2252 */ 2,
/* 2253 */ 2257,
/* 2254 */ 50058,
/* 2255 */ -2253,
/* 2256 */ 1,
/* 2257 */ 0,
/* 2258 */ -2253,
/* 2259 */ 2,
/* 2260 */ 0,
/* 2261 */ 1,
/* 2262 */ 1,
/* 2263 */ 50257,
/* 2264 */ 50058,
/* 2265 */ 1,
/* 2266 */ 50123,
/* 2267 */ 1,
/* 2268 */ 50125,
/* 2269 */ 1,
/* 2270 */ -2260,
/* 2271 */ 1,
/* 2272 */ 2278,
/* 2273 */ 1,
/* 2274 */ 1,
/* 2275 */ 1,
/* 2276 */ -2272,
/* 2277 */ 1,
/* 2278 */ 0,
/* 2279 */ -2272,
/* 2280 */ 2,
/* 2281 */ 2285,
/* 2282 */ 50047,
/* 2283 */ -2281,
/* 2284 */ 1,
/* 2285 */ 0,
/* 2286 */ -2281,
/* 2287 */ 2,
/* 2288 */ 2292,
/* 2289 */ 50436,
/* 2290 */ -2288,
/* 2291 */ 1,
/* 2292 */ 2296,
/* 2293 */ 50334,
/* 2294 */ -2288,
/* 2295 */ 2,
/* 2296 */ 2300,
/* 2297 */ 50409,
/* 2298 */ -2288,
/* 2299 */ 3,
/* 2300 */ 0,
/* 2301 */ 50330,
/* 2302 */ -2288,
/* 2303 */ 4,
/* 2304 */ 2309,
/* 2305 */ 1,
/* 2306 */ 1,
/* 2307 */ -2304,
/* 2308 */ 1,
/* 2309 */ 0,
/* 2310 */ -2304,
/* 2311 */ 2,
/* 2312 */ 2321,
/* 2313 */ 50288,
/* 2314 */ 1,
/* 2315 */ 50123,
/* 2316 */ 1,
/* 2317 */ 50125,
/* 2318 */ 1,
/* 2319 */ -2312,
/* 2320 */ 1,
/* 2321 */ 0,
/* 2322 */ -2312,
/* 2323 */ 2,
/* 2324 */ 2329,
/* 2325 */ 1,
/* 2326 */ 1,
/* 2327 */ -2324,
/* 2328 */ 1,
/* 2329 */ 0,
/* 2330 */ -2324,
/* 2331 */ 2,
/* 2332 */ 2339,
/* 2333 */ 50326,
/* 2334 */ 1,
/* 2335 */ 50257,
/* 2336 */ 50059,
/* 2337 */ -2332,
/* 2338 */ 1,
/* 2339 */ 2344,
/* 2340 */ 1,
/* 2341 */ 50059,
/* 2342 */ -2332,
/* 2343 */ 2,
/* 2344 */ 2351,
/* 2345 */ 50374,
/* 2346 */ 1,
/* 2347 */ 1,
/* 2348 */ 50059,
/* 2349 */ -2332,
/* 2350 */ 3,
/* 2351 */ 2358,
/* 2352 */ 50315,
/* 2353 */ 1,
/* 2354 */ 1,
/* 2355 */ 50059,
/* 2356 */ -2332,
/* 2357 */ 4,
/* 2358 */ 2367,
/* 2359 */ 50298,
/* 2360 */ 1,
/* 2361 */ 1,
/* 2362 */ 1,
/* 2363 */ 1,
/* 2364 */ 50059,
/* 2365 */ -2332,
/* 2366 */ 5,
/* 2367 */ 2375,
/* 2368 */ 50413,
/* 2369 */ 1,
/* 2370 */ 1,
/* 2371 */ 1,
/* 2372 */ 50059,
/* 2373 */ -2332,
/* 2374 */ 6,
/* 2375 */ 2383,
/* 2376 */ 50445,
/* 2377 */ 1,
/* 2378 */ 1,
/* 2379 */ 1,
/* 2380 */ 50059,
/* 2381 */ -2332,
/* 2382 */ 7,
/* 2383 */ 2390,
/* 2384 */ 50417,
/* 2385 */ 1,
/* 2386 */ 1,
/* 2387 */ 50059,
/* 2388 */ -2332,
/* 2389 */ 8,
/* 2390 */ 2397,
/* 2391 */ 50472,
/* 2392 */ 1,
/* 2393 */ 1,
/* 2394 */ 50059,
/* 2395 */ -2332,
/* 2396 */ 9,
/* 2397 */ 2404,
/* 2398 */ 50473,
/* 2399 */ 1,
/* 2400 */ 1,
/* 2401 */ 50059,
/* 2402 */ -2332,
/* 2403 */ 10,
/* 2404 */ 2411,
/* 2405 */ 50333,
/* 2406 */ 1,
/* 2407 */ 1,
/* 2408 */ 50059,
/* 2409 */ -2332,
/* 2410 */ 11,
/* 2411 */ 2418,
/* 2412 */ 50414,
/* 2413 */ 1,
/* 2414 */ 1,
/* 2415 */ 50059,
/* 2416 */ -2332,
/* 2417 */ 12,
/* 2418 */ 2425,
/* 2419 */ 50297,
/* 2420 */ 1,
/* 2421 */ 1,
/* 2422 */ 50059,
/* 2423 */ -2332,
/* 2424 */ 13,
/* 2425 */ 2432,
/* 2426 */ 50378,
/* 2427 */ 1,
/* 2428 */ 1,
/* 2429 */ 50059,
/* 2430 */ -2332,
/* 2431 */ 14,
/* 2432 */ 2439,
/* 2433 */ 50461,
/* 2434 */ 1,
/* 2435 */ 1,
/* 2436 */ 50059,
/* 2437 */ -2332,
/* 2438 */ 15,
/* 2439 */ 2446,
/* 2440 */ 50428,
/* 2441 */ 1,
/* 2442 */ 1,
/* 2443 */ 50059,
/* 2444 */ -2332,
/* 2445 */ 16,
/* 2446 */ 2453,
/* 2447 */ 50428,
/* 2448 */ 1,
/* 2449 */ 1,
/* 2450 */ 50059,
/* 2451 */ -2332,
/* 2452 */ 17,
/* 2453 */ 2460,
/* 2454 */ 50318,
/* 2455 */ 1,
/* 2456 */ 1,
/* 2457 */ 50059,
/* 2458 */ -2332,
/* 2459 */ 18,
/* 2460 */ 2467,
/* 2461 */ 50318,
/* 2462 */ 1,
/* 2463 */ 50275,
/* 2464 */ 50059,
/* 2465 */ -2332,
/* 2466 */ 19,
/* 2467 */ 2474,
/* 2468 */ 50350,
/* 2469 */ 1,
/* 2470 */ 1,
/* 2471 */ 50059,
/* 2472 */ -2332,
/* 2473 */ 20,
/* 2474 */ 2478,
/* 2475 */ 1,
/* 2476 */ -2332,
/* 2477 */ 21,
/* 2478 */ 2482,
/* 2479 */ 1,
/* 2480 */ -2332,
/* 2481 */ 22,
/* 2482 */ 0,
/* 2483 */ 1,
/* 2484 */ -2332,
/* 2485 */ 23,
/* 2486 */ 2490,
/* 2487 */ 50058,
/* 2488 */ -2486,
/* 2489 */ 1,
/* 2490 */ 0,
/* 2491 */ -2486,
/* 2492 */ 2,
/* 2493 */ 2497,
/* 2494 */ 50058,
/* 2495 */ -2493,
/* 2496 */ 1,
/* 2497 */ 0,
/* 2498 */ -2493,
/* 2499 */ 2,
/* 2500 */ 2504,
/* 2501 */ 50058,
/* 2502 */ -2500,
/* 2503 */ 1,
/* 2504 */ 0,
/* 2505 */ -2500,
/* 2506 */ 2,
/* 2507 */ 2511,
/* 2508 */ 50308,
/* 2509 */ -2507,
/* 2510 */ 1,
/* 2511 */ 0,
/* 2512 */ -2507,
/* 2513 */ 2,
/* 2514 */ 2518,
/* 2515 */ 50058,
/* 2516 */ -2514,
/* 2517 */ 1,
/* 2518 */ 0,
/* 2519 */ -2514,
/* 2520 */ 2,
/* 2521 */ 2525,
/* 2522 */ 50037,
/* 2523 */ -2521,
/* 2524 */ 1,
/* 2525 */ 0,
/* 2526 */ -2521,
/* 2527 */ 2,
/* 2528 */ 2532,
/* 2529 */ 50058,
/* 2530 */ -2528,
/* 2531 */ 1,
/* 2532 */ 0,
/* 2533 */ -2528,
/* 2534 */ 2,
/* 2535 */ 2541,
/* 2536 */ 50040,
/* 2537 */ 1,
/* 2538 */ 50041,
/* 2539 */ -2535,
/* 2540 */ 1,
/* 2541 */ 0,
/* 2542 */ -2535,
/* 2543 */ 2,
/* 2544 */ 2548,
/* 2545 */ 50058,
/* 2546 */ -2544,
/* 2547 */ 1,
/* 2548 */ 0,
/* 2549 */ -2544,
/* 2550 */ 2,
/* 2551 */ 2555,
/* 2552 */ 50405,
/* 2553 */ -2551,
/* 2554 */ 1,
/* 2555 */ 0,
/* 2556 */ -2551,
/* 2557 */ 2,
/* 2558 */ 2562,
/* 2559 */ 50058,
/* 2560 */ -2558,
/* 2561 */ 1,
/* 2562 */ 0,
/* 2563 */ -2558,
/* 2564 */ 2,
/* 2565 */ 2569,
/* 2566 */ 50058,
/* 2567 */ -2565,
/* 2568 */ 1,
/* 2569 */ 0,
/* 2570 */ -2565,
/* 2571 */ 2,
/* 2572 */ 2576,
/* 2573 */ 50058,
/* 2574 */ -2572,
/* 2575 */ 1,
/* 2576 */ 0,
/* 2577 */ -2572,
/* 2578 */ 2,
/* 2579 */ 2583,
/* 2580 */ 50058,
/* 2581 */ -2579,
/* 2582 */ 1,
/* 2583 */ 0,
/* 2584 */ -2579,
/* 2585 */ 2,
/* 2586 */ 2590,
/* 2587 */ 50058,
/* 2588 */ -2586,
/* 2589 */ 1,
/* 2590 */ 0,
/* 2591 */ -2586,
/* 2592 */ 2,
/* 2593 */ 2597,
/* 2594 */ 50058,
/* 2595 */ -2593,
/* 2596 */ 1,
/* 2597 */ 0,
/* 2598 */ -2593,
/* 2599 */ 2,
/* 2600 */ 2604,
/* 2601 */ 50058,
/* 2602 */ -2600,
/* 2603 */ 1,
/* 2604 */ 0,
/* 2605 */ -2600,
/* 2606 */ 2,
/* 2607 */ 2611,
/* 2608 */ 50058,
/* 2609 */ -2607,
/* 2610 */ 1,
/* 2611 */ 0,
/* 2612 */ -2607,
/* 2613 */ 2,
/* 2614 */ 2618,
/* 2615 */ 50058,
/* 2616 */ -2614,
/* 2617 */ 1,
/* 2618 */ 0,
/* 2619 */ -2614,
/* 2620 */ 2,
/* 2621 */ 2625,
/* 2622 */ 50058,
/* 2623 */ -2621,
/* 2624 */ 1,
/* 2625 */ 0,
/* 2626 */ -2621,
/* 2627 */ 2,
/* 2628 */ 2632,
/* 2629 */ 50058,
/* 2630 */ -2628,
/* 2631 */ 1,
/* 2632 */ 0,
/* 2633 */ -2628,
/* 2634 */ 2,
/* 2635 */ 2639,
/* 2636 */ 50058,
/* 2637 */ -2635,
/* 2638 */ 1,
/* 2639 */ 0,
/* 2640 */ -2635,
/* 2641 */ 2,
/* 2642 */ 2646,
/* 2643 */ 50058,
/* 2644 */ -2642,
/* 2645 */ 1,
/* 2646 */ 0,
/* 2647 */ -2642,
/* 2648 */ 2,
/* 2649 */ 2655,
/* 2650 */ 1,
/* 2651 */ 1,
/* 2652 */ 1,
/* 2653 */ -2649,
/* 2654 */ 1,
/* 2655 */ 0,
/* 2656 */ -2649,
/* 2657 */ 2,
/* 2658 */ 2662,
/* 2659 */ 50044,
/* 2660 */ -2658,
/* 2661 */ 1,
/* 2662 */ 2666,
/* 2663 */ 50124,
/* 2664 */ -2658,
/* 2665 */ 2,
/* 2666 */ 0,
/* 2667 */ -2658,
/* 2668 */ 3,
/* 2669 */ 0,
/* 2670 */ 1,
/* 2671 */ -2669,
/* 2672 */ 1,
/* 2673 */ 2679,
/* 2674 */ 1,
/* 2675 */ 1,
/* 2676 */ 1,
/* 2677 */ -2673,
/* 2678 */ 1,
/* 2679 */ 0,
/* 2680 */ -2673,
/* 2681 */ 2,
/* 2682 */ 2686,
/* 2683 */ 50044,
/* 2684 */ -2682,
/* 2685 */ 1,
/* 2686 */ 2690,
/* 2687 */ 50124,
/* 2688 */ -2682,
/* 2689 */ 2,
/* 2690 */ 0,
/* 2691 */ -2682,
/* 2692 */ 3,
/* 2693 */ 0,
/* 2694 */ 50261,
/* 2695 */ -2693,
/* 2696 */ 1,
/* 2697 */ 0,
/* 2698 */ 50418,
/* 2699 */ 50257,
/* 2700 */ 1,
/* 2701 */ 50123,
/* 2702 */ 1,
/* 2703 */ 50125,
/* 2704 */ -2697,
/* 2705 */ 1,
/* 2706 */ 2711,
/* 2707 */ 50058,
/* 2708 */ 1,
/* 2709 */ -2706,
/* 2710 */ 1,
/* 2711 */ 0,
/* 2712 */ -2706,
/* 2713 */ 2,
/* 2714 */ 2719,
/* 2715 */ 1,
/* 2716 */ 1,
/* 2717 */ -2714,
/* 2718 */ 1,
/* 2719 */ 0,
/* 2720 */ -2714,
/* 2721 */ 2,
/* 2722 */ 2729,
/* 2723 */ 50326,
/* 2724 */ 1,
/* 2725 */ 50257,
/* 2726 */ 50059,
/* 2727 */ -2722,
/* 2728 */ 1,
/* 2729 */ 2734,
/* 2730 */ 1,
/* 2731 */ 50059,
/* 2732 */ -2722,
/* 2733 */ 2,
/* 2734 */ 2741,
/* 2735 */ 50359,
/* 2736 */ 1,
/* 2737 */ 1,
/* 2738 */ 50059,
/* 2739 */ -2722,
/* 2740 */ 3,
/* 2741 */ 2748,
/* 2742 */ 50388,
/* 2743 */ 1,
/* 2744 */ 1,
/* 2745 */ 50059,
/* 2746 */ -2722,
/* 2747 */ 4,
/* 2748 */ 2758,
/* 2749 */ 50340,
/* 2750 */ 1,
/* 2751 */ 1,
/* 2752 */ 1,
/* 2753 */ 50044,
/* 2754 */ 1,
/* 2755 */ 50059,
/* 2756 */ -2722,
/* 2757 */ 5,
/* 2758 */ 2770,
/* 2759 */ 50340,
/* 2760 */ 1,
/* 2761 */ 1,
/* 2762 */ 1,
/* 2763 */ 50044,
/* 2764 */ 1,
/* 2765 */ 50044,
/* 2766 */ 1,
/* 2767 */ 50059,
/* 2768 */ -2722,
/* 2769 */ 6,
/* 2770 */ 2777,
/* 2771 */ 50432,
/* 2772 */ 1,
/* 2773 */ 1,
/* 2774 */ 50059,
/* 2775 */ -2722,
/* 2776 */ 7,
/* 2777 */ 2784,
/* 2778 */ 1,
/* 2779 */ 1,
/* 2780 */ 50257,
/* 2781 */ 50059,
/* 2782 */ -2722,
/* 2783 */ 8,
/* 2784 */ 2791,
/* 2785 */ 50350,
/* 2786 */ 1,
/* 2787 */ 1,
/* 2788 */ 50059,
/* 2789 */ -2722,
/* 2790 */ 9,
/* 2791 */ 2795,
/* 2792 */ 1,
/* 2793 */ -2722,
/* 2794 */ 10,
/* 2795 */ 2799,
/* 2796 */ 1,
/* 2797 */ -2722,
/* 2798 */ 11,
/* 2799 */ 2803,
/* 2800 */ 1,
/* 2801 */ -2722,
/* 2802 */ 12,
/* 2803 */ 0,
/* 2804 */ 1,
/* 2805 */ -2722,
/* 2806 */ 13,
/* 2807 */ 2811,
/* 2808 */ 50058,
/* 2809 */ -2807,
/* 2810 */ 1,
/* 2811 */ 0,
/* 2812 */ -2807,
/* 2813 */ 2,
/* 2814 */ 2822,
/* 2815 */ 50277,
/* 2816 */ 1,
/* 2817 */ 1,
/* 2818 */ 1,
/* 2819 */ 1,
/* 2820 */ -2814,
/* 2821 */ 1,
/* 2822 */ 0,
/* 2823 */ -2814,
/* 2824 */ 2,
/* 2825 */ 2829,
/* 2826 */ 50058,
/* 2827 */ -2825,
/* 2828 */ 1,
/* 2829 */ 0,
/* 2830 */ -2825,
/* 2831 */ 2,
/* 2832 */ 2836,
/* 2833 */ 50044,
/* 2834 */ -2832,
/* 2835 */ 1,
/* 2836 */ 0,
/* 2837 */ -2832,
/* 2838 */ 2,
/* 2839 */ 2843,
/* 2840 */ 50058,
/* 2841 */ -2839,
/* 2842 */ 1,
/* 2843 */ 0,
/* 2844 */ -2839,
/* 2845 */ 2,
/* 2846 */ 2852,
/* 2847 */ 1,
/* 2848 */ 1,
/* 2849 */ 1,
/* 2850 */ -2846,
/* 2851 */ 1,
/* 2852 */ 0,
/* 2853 */ -2846,
/* 2854 */ 2,
/* 2855 */ 2859,
/* 2856 */ 50044,
/* 2857 */ -2855,
/* 2858 */ 1,
/* 2859 */ 0,
/* 2860 */ -2855,
/* 2861 */ 2,
/* 2862 */ 2866,
/* 2863 */ 50058,
/* 2864 */ -2862,
/* 2865 */ 1,
/* 2866 */ 0,
/* 2867 */ -2862,
/* 2868 */ 2,
/* 2869 */ 2873,
/* 2870 */ 50309,
/* 2871 */ -2869,
/* 2872 */ 1,
/* 2873 */ 0,
/* 2874 */ -2869,
/* 2875 */ 2,
/* 2876 */ 2880,
/* 2877 */ 50058,
/* 2878 */ -2876,
/* 2879 */ 1,
/* 2880 */ 0,
/* 2881 */ -2876,
/* 2882 */ 2,
/* 2883 */ 2887,
/* 2884 */ 50309,
/* 2885 */ -2883,
/* 2886 */ 1,
/* 2887 */ 0,
/* 2888 */ -2883,
/* 2889 */ 2,
/* 2890 */ 2894,
/* 2891 */ 50058,
/* 2892 */ -2890,
/* 2893 */ 1,
/* 2894 */ 0,
/* 2895 */ -2890,
/* 2896 */ 2,
/* 2897 */ 2901,
/* 2898 */ 50058,
/* 2899 */ -2897,
/* 2900 */ 1,
/* 2901 */ 0,
/* 2902 */ -2897,
/* 2903 */ 2,
/* 2904 */ 2910,
/* 2905 */ 1,
/* 2906 */ 1,
/* 2907 */ 1,
/* 2908 */ -2904,
/* 2909 */ 1,
/* 2910 */ 0,
/* 2911 */ -2904,
/* 2912 */ 2,
/* 2913 */ 2917,
/* 2914 */ 50044,
/* 2915 */ -2913,
/* 2916 */ 1,
/* 2917 */ 0,
/* 2918 */ -2913,
/* 2919 */ 2,
/* 2920 */ 2924,
/* 2921 */ 50058,
/* 2922 */ -2920,
/* 2923 */ 1,
/* 2924 */ 0,
/* 2925 */ -2920,
/* 2926 */ 2,
/* 2927 */ 2931,
/* 2928 */ 50058,
/* 2929 */ -2927,
/* 2930 */ 1,
/* 2931 */ 0,
/* 2932 */ -2927,
/* 2933 */ 2,
/* 2934 */ 2940,
/* 2935 */ 1,
/* 2936 */ 1,
/* 2937 */ 1,
/* 2938 */ -2934,
/* 2939 */ 1,
/* 2940 */ 0,
/* 2941 */ -2934,
/* 2942 */ 2,
/* 2943 */ 2947,
/* 2944 */ 50124,
/* 2945 */ -2943,
/* 2946 */ 1,
/* 2947 */ 2951,
/* 2948 */ 50044,
/* 2949 */ -2943,
/* 2950 */ 2,
/* 2951 */ 0,
/* 2952 */ -2943,
/* 2953 */ 3,
/* 2954 */ 0,
/* 2955 */ 50309,
/* 2956 */ 50257,
/* 2957 */ 50123,
/* 2958 */ 1,
/* 2959 */ 50125,
/* 2960 */ -2954,
/* 2961 */ 1,
/* 2962 */ 2967,
/* 2963 */ 1,
/* 2964 */ 1,
/* 2965 */ -2962,
/* 2966 */ 1,
/* 2967 */ 0,
/* 2968 */ -2962,
/* 2969 */ 2,
/* 2970 */ 2977,
/* 2971 */ 50326,
/* 2972 */ 1,
/* 2973 */ 50257,
/* 2974 */ 50059,
/* 2975 */ -2970,
/* 2976 */ 1,
/* 2977 */ 2985,
/* 2978 */ 50364,
/* 2979 */ 1,
/* 2980 */ 1,
/* 2981 */ 1,
/* 2982 */ 50059,
/* 2983 */ -2970,
/* 2984 */ 2,
/* 2985 */ 2993,
/* 2986 */ 50381,
/* 2987 */ 1,
/* 2988 */ 1,
/* 2989 */ 1,
/* 2990 */ 50059,
/* 2991 */ -2970,
/* 2992 */ 3,
/* 2993 */ 3002,
/* 2994 */ 50323,
/* 2995 */ 1,
/* 2996 */ 1,
/* 2997 */ 50047,
/* 2998 */ 1,
/* 2999 */ 50059,
/* 3000 */ -2970,
/* 3001 */ 4,
/* 3002 */ 3009,
/* 3003 */ 50296,
/* 3004 */ 1,
/* 3005 */ 1,
/* 3006 */ 50059,
/* 3007 */ -2970,
/* 3008 */ 5,
/* 3009 */ 3016,
/* 3010 */ 50429,
/* 3011 */ 1,
/* 3012 */ 1,
/* 3013 */ 50059,
/* 3014 */ -2970,
/* 3015 */ 6,
/* 3016 */ 3023,
/* 3017 */ 50359,
/* 3018 */ 1,
/* 3019 */ 1,
/* 3020 */ 50059,
/* 3021 */ -2970,
/* 3022 */ 7,
/* 3023 */ 3033,
/* 3024 */ 50432,
/* 3025 */ 1,
/* 3026 */ 1,
/* 3027 */ 1,
/* 3028 */ 1,
/* 3029 */ 1,
/* 3030 */ 50059,
/* 3031 */ -2970,
/* 3032 */ 8,
/* 3033 */ 3040,
/* 3034 */ 50411,
/* 3035 */ 1,
/* 3036 */ 1,
/* 3037 */ 50059,
/* 3038 */ -2970,
/* 3039 */ 9,
/* 3040 */ 3047,
/* 3041 */ 50350,
/* 3042 */ 1,
/* 3043 */ 1,
/* 3044 */ 50059,
/* 3045 */ -2970,
/* 3046 */ 10,
/* 3047 */ 3051,
/* 3048 */ 1,
/* 3049 */ -2970,
/* 3050 */ 11,
/* 3051 */ 3055,
/* 3052 */ 1,
/* 3053 */ -2970,
/* 3054 */ 12,
/* 3055 */ 3059,
/* 3056 */ 1,
/* 3057 */ -2970,
/* 3058 */ 13,
/* 3059 */ 0,
/* 3060 */ 1,
/* 3061 */ -2970,
/* 3062 */ 14,
/* 3063 */ 3067,
/* 3064 */ 50058,
/* 3065 */ -3063,
/* 3066 */ 1,
/* 3067 */ 0,
/* 3068 */ -3063,
/* 3069 */ 2,
/* 3070 */ 3074,
/* 3071 */ 50058,
/* 3072 */ -3070,
/* 3073 */ 1,
/* 3074 */ 0,
/* 3075 */ -3070,
/* 3076 */ 2,
/* 3077 */ 3081,
/* 3078 */ 50100,
/* 3079 */ -3077,
/* 3080 */ 1,
/* 3081 */ 0,
/* 3082 */ -3077,
/* 3083 */ 2,
/* 3084 */ 3088,
/* 3085 */ 50058,
/* 3086 */ -3084,
/* 3087 */ 1,
/* 3088 */ 0,
/* 3089 */ -3084,
/* 3090 */ 2,
/* 3091 */ 3095,
/* 3092 */ 50100,
/* 3093 */ -3091,
/* 3094 */ 1,
/* 3095 */ 0,
/* 3096 */ -3091,
/* 3097 */ 2,
/* 3098 */ 3102,
/* 3099 */ 50058,
/* 3100 */ -3098,
/* 3101 */ 1,
/* 3102 */ 0,
/* 3103 */ -3098,
/* 3104 */ 2,
/* 3105 */ 3109,
/* 3106 */ 50058,
/* 3107 */ -3105,
/* 3108 */ 1,
/* 3109 */ 0,
/* 3110 */ -3105,
/* 3111 */ 2,
/* 3112 */ 3119,
/* 3113 */ 1,
/* 3114 */ 1,
/* 3115 */ 1,
/* 3116 */ 1,
/* 3117 */ -3112,
/* 3118 */ 1,
/* 3119 */ 0,
/* 3120 */ -3112,
/* 3121 */ 2,
/* 3122 */ 3126,
/* 3123 */ 50044,
/* 3124 */ -3122,
/* 3125 */ 1,
/* 3126 */ 0,
/* 3127 */ -3122,
/* 3128 */ 2,
/* 3129 */ 3133,
/* 3130 */ 50058,
/* 3131 */ -3129,
/* 3132 */ 1,
/* 3133 */ 0,
/* 3134 */ -3129,
/* 3135 */ 2,
/* 3136 */ 3143,
/* 3137 */ 1,
/* 3138 */ 1,
/* 3139 */ 1,
/* 3140 */ 1,
/* 3141 */ -3136,
/* 3142 */ 1,
/* 3143 */ 0,
/* 3144 */ -3136,
/* 3145 */ 2,
/* 3146 */ 3150,
/* 3147 */ 50044,
/* 3148 */ -3146,
/* 3149 */ 1,
/* 3150 */ 0,
/* 3151 */ -3146,
/* 3152 */ 2,
/* 3153 */ 3157,
/* 3154 */ 50058,
/* 3155 */ -3153,
/* 3156 */ 1,
/* 3157 */ 0,
/* 3158 */ -3153,
/* 3159 */ 2,
/* 3160 */ 3166,
/* 3161 */ 1,
/* 3162 */ 1,
/* 3163 */ 1,
/* 3164 */ -3160,
/* 3165 */ 1,
/* 3166 */ 0,
/* 3167 */ -3160,
/* 3168 */ 2,
/* 3169 */ 3173,
/* 3170 */ 50044,
/* 3171 */ -3169,
/* 3172 */ 1,
/* 3173 */ 0,
/* 3174 */ -3169,
/* 3175 */ 2,
/* 3176 */ 0,
/* 3177 */ 50091,
/* 3178 */ -3176,
/* 3179 */ 1,
/* 3180 */ 0,
/* 3181 */ 50093,
/* 3182 */ -3180,
/* 3183 */ 1,
/* 3184 */ 3188,
/* 3185 */ 50058,
/* 3186 */ -3184,
/* 3187 */ 1,
/* 3188 */ 0,
/* 3189 */ -3184,
/* 3190 */ 2,
/* 3191 */ 3197,
/* 3192 */ 1,
/* 3193 */ 1,
/* 3194 */ 1,
/* 3195 */ -3191,
/* 3196 */ 1,
/* 3197 */ 0,
/* 3198 */ -3191,
/* 3199 */ 2,
/* 3200 */ 3204,
/* 3201 */ 50044,
/* 3202 */ -3200,
/* 3203 */ 1,
/* 3204 */ 0,
/* 3205 */ -3200,
/* 3206 */ 2,
/* 3207 */ 3211,
/* 3208 */ 50058,
/* 3209 */ -3207,
/* 3210 */ 1,
/* 3211 */ 0,
/* 3212 */ -3207,
/* 3213 */ 2,
/* 3214 */ 3220,
/* 3215 */ 1,
/* 3216 */ 1,
/* 3217 */ 1,
/* 3218 */ -3214,
/* 3219 */ 1,
/* 3220 */ 0,
/* 3221 */ -3214,
/* 3222 */ 2,
/* 3223 */ 3227,
/* 3224 */ 50044,
/* 3225 */ -3223,
/* 3226 */ 1,
/* 3227 */ 0,
/* 3228 */ -3223,
/* 3229 */ 2,
/* 3230 */ 3234,
/* 3231 */ 50058,
/* 3232 */ -3230,
/* 3233 */ 1,
/* 3234 */ 0,
/* 3235 */ -3230,
/* 3236 */ 2,
/* 3237 */ 3243,
/* 3238 */ 1,
/* 3239 */ 1,
/* 3240 */ 1,
/* 3241 */ -3237,
/* 3242 */ 1,
/* 3243 */ 0,
/* 3244 */ -3237,
/* 3245 */ 2,
/* 3246 */ 3250,
/* 3247 */ 50124,
/* 3248 */ -3246,
/* 3249 */ 1,
/* 3250 */ 3254,
/* 3251 */ 50044,
/* 3252 */ -3246,
/* 3253 */ 2,
/* 3254 */ 0,
/* 3255 */ -3246,
/* 3256 */ 3,
/* 3257 */ 3267,
/* 3258 */ 50282,
/* 3259 */ 50091,
/* 3260 */ 1,
/* 3261 */ 1,
/* 3262 */ 50093,
/* 3263 */ 50295,
/* 3264 */ 1,
/* 3265 */ -3257,
/* 3266 */ 1,
/* 3267 */ 3277,
/* 3268 */ 50440,
/* 3269 */ 50091,
/* 3270 */ 1,
/* 3271 */ 1,
/* 3272 */ 50093,
/* 3273 */ 50295,
/* 3274 */ 1,
/* 3275 */ -3257,
/* 3276 */ 2,
/* 3277 */ 0,
/* 3278 */ 50347,
/* 3279 */ 50091,
/* 3280 */ 1,
/* 3281 */ 1,
/* 3282 */ 50093,
/* 3283 */ 50295,
/* 3284 */ 1,
/* 3285 */ -3257,
/* 3286 */ 3,
/* 3287 */ 3291,
/* 3288 */ 1,
/* 3289 */ -3287,
/* 3290 */ 1,
/* 3291 */ 0,
/* 3292 */ -3287,
/* 3293 */ 2,
/* 3294 */ 3298,
/* 3295 */ 1,
/* 3296 */ -3294,
/* 3297 */ 1,
/* 3298 */ 0,
/* 3299 */ -3294,
/* 3300 */ 2,
/* 3301 */ 3305,
/* 3302 */ 1,
/* 3303 */ -3301,
/* 3304 */ 1,
/* 3305 */ 0,
/* 3306 */ -3301,
/* 3307 */ 2,
/* 3308 */ 3313,
/* 3309 */ 50044,
/* 3310 */ 1,
/* 3311 */ -3308,
/* 3312 */ 1,
/* 3313 */ 3318,
/* 3314 */ 50044,
/* 3315 */ 1,
/* 3316 */ -3308,
/* 3317 */ 2,
/* 3318 */ 0,
/* 3319 */ 50044,
/* 3320 */ 1,
/* 3321 */ 50044,
/* 3322 */ 1,
/* 3323 */ -3308,
/* 3324 */ 3,
/* 3325 */ 3330,
/* 3326 */ 1,
/* 3327 */ 1,
/* 3328 */ -3325,
/* 3329 */ 1,
/* 3330 */ 3336,
/* 3331 */ 50343,
/* 3332 */ 1,
/* 3333 */ 50374,
/* 3334 */ -3325,
/* 3335 */ 2,
/* 3336 */ 3341,
/* 3337 */ 50343,
/* 3338 */ 50374,
/* 3339 */ -3325,
/* 3340 */ 3,
/* 3341 */ 3350,
/* 3342 */ 50343,
/* 3343 */ 50374,
/* 3344 */ 50439,
/* 3345 */ 50295,
/* 3346 */ 1,
/* 3347 */ 1,
/* 3348 */ -3325,
/* 3349 */ 4,
/* 3350 */ 0,
/* 3351 */ 50343,
/* 3352 */ 1,
/* 3353 */ 50374,
/* 3354 */ 50439,
/* 3355 */ 50295,
/* 3356 */ 1,
/* 3357 */ 1,
/* 3358 */ -3325,
/* 3359 */ 5,
/* 3360 */ 3364,
/* 3361 */ 50374,
/* 3362 */ -3360,
/* 3363 */ 1,
/* 3364 */ 0,
/* 3365 */ -3360,
/* 3366 */ 2,
/* 3367 */ 3371,
/* 3368 */ 50374,
/* 3369 */ -3367,
/* 3370 */ 1,
/* 3371 */ 0,
/* 3372 */ -3367,
/* 3373 */ 2,
/* 3374 */ 3378,
/* 3375 */ 50374,
/* 3376 */ -3374,
/* 3377 */ 1,
/* 3378 */ 0,
/* 3379 */ -3374,
/* 3380 */ 2,
/* 3381 */ 0,
/* 3382 */ 50332,
/* 3383 */ 50257,
/* 3384 */ 50123,
/* 3385 */ 1,
/* 3386 */ 50125,
/* 3387 */ -3381,
/* 3388 */ 1,
/* 3389 */ 3394,
/* 3390 */ 1,
/* 3391 */ 1,
/* 3392 */ -3389,
/* 3393 */ 1,
/* 3394 */ 0,
/* 3395 */ -3389,
/* 3396 */ 2,
/* 3397 */ 3404,
/* 3398 */ 50326,
/* 3399 */ 1,
/* 3400 */ 50257,
/* 3401 */ 50059,
/* 3402 */ -3397,
/* 3403 */ 1,
/* 3404 */ 3408,
/* 3405 */ 1,
/* 3406 */ -3397,
/* 3407 */ 2,
/* 3408 */ 3412,
/* 3409 */ 1,
/* 3410 */ -3397,
/* 3411 */ 3,
/* 3412 */ 0,
/* 3413 */ 1,
/* 3414 */ -3397,
/* 3415 */ 4,
/* 3416 */ 3420,
/* 3417 */ 50058,
/* 3418 */ -3416,
/* 3419 */ 1,
/* 3420 */ 0,
/* 3421 */ -3416,
/* 3422 */ 2,
/* 3423 */ 3430,
/* 3424 */ 50311,
/* 3425 */ 1,
/* 3426 */ 1,
/* 3427 */ 50059,
/* 3428 */ -3423,
/* 3429 */ 1,
/* 3430 */ 3436,
/* 3431 */ 50377,
/* 3432 */ 1,
/* 3433 */ 1,
/* 3434 */ -3423,
/* 3435 */ 2,
/* 3436 */ 0,
/* 3437 */ 50434,
/* 3438 */ 1,
/* 3439 */ 1,
/* 3440 */ 50059,
/* 3441 */ -3423,
/* 3442 */ 3,
/* 3443 */ 3447,
/* 3444 */ 50058,
/* 3445 */ -3443,
/* 3446 */ 1,
/* 3447 */ 0,
/* 3448 */ -3443,
/* 3449 */ 2,
/* 3450 */ 3458,
/* 3451 */ 50042,
/* 3452 */ 1,
/* 3453 */ 1,
/* 3454 */ 1,
/* 3455 */ 1,
/* 3456 */ -3450,
/* 3457 */ 1,
/* 3458 */ 3466,
/* 3459 */ 50042,
/* 3460 */ 1,
/* 3461 */ 1,
/* 3462 */ 1,
/* 3463 */ 1,
/* 3464 */ -3450,
/* 3465 */ 2,
/* 3466 */ 0,
/* 3467 */ -3450,
/* 3468 */ 3,
/* 3469 */ 3473,
/* 3470 */ 50044,
/* 3471 */ -3469,
/* 3472 */ 1,
/* 3473 */ 0,
/* 3474 */ -3469,
/* 3475 */ 2,
/* 3476 */ 3480,
/* 3477 */ 50044,
/* 3478 */ -3476,
/* 3479 */ 1,
/* 3480 */ 0,
/* 3481 */ -3476,
/* 3482 */ 2,
/* 3483 */ 3487,
/* 3484 */ 50058,
/* 3485 */ -3483,
/* 3486 */ 1,
/* 3487 */ 0,
/* 3488 */ -3483,
/* 3489 */ 2,
/* 3490 */ 3498,
/* 3491 */ 50042,
/* 3492 */ 1,
/* 3493 */ 1,
/* 3494 */ 1,
/* 3495 */ 1,
/* 3496 */ -3490,
/* 3497 */ 1,
/* 3498 */ 0,
/* 3499 */ -3490,
/* 3500 */ 2,
/* 3501 */ 3506,
/* 3502 */ 1,
/* 3503 */ 1,
/* 3504 */ -3501,
/* 3505 */ 1,
/* 3506 */ 0,
/* 3507 */ -3501,
/* 3508 */ 2,
/* 3509 */ 3513,
/* 3510 */ 50044,
/* 3511 */ -3509,
/* 3512 */ 1,
/* 3513 */ 0,
/* 3514 */ 50059,
/* 3515 */ -3509,
/* 3516 */ 2,
/* 3517 */ 3521,
/* 3518 */ 50058,
/* 3519 */ -3517,
/* 3520 */ 1,
/* 3521 */ 0,
/* 3522 */ -3517,
/* 3523 */ 2,
/* 3524 */ 3532,
/* 3525 */ 50042,
/* 3526 */ 1,
/* 3527 */ 1,
/* 3528 */ 1,
/* 3529 */ 1,
/* 3530 */ -3524,
/* 3531 */ 1,
/* 3532 */ 0,
/* 3533 */ -3524,
/* 3534 */ 2,
/* 3535 */ 3540,
/* 3536 */ 1,
/* 3537 */ 50037,
/* 3538 */ -3535,
/* 3539 */ 1,
/* 3540 */ 0,
/* 3541 */ -3535,
/* 3542 */ 2,
/* 3543 */ 3550,
/* 3544 */ 1,
/* 3545 */ 50295,
/* 3546 */ 50374,
/* 3547 */ 1,
/* 3548 */ -3543,
/* 3549 */ 1,
/* 3550 */ 3557,
/* 3551 */ 1,
/* 3552 */ 50295,
/* 3553 */ 1,
/* 3554 */ 50374,
/* 3555 */ -3543,
/* 3556 */ 2,
/* 3557 */ 0,
/* 3558 */ 1,
/* 3559 */ 50295,
/* 3560 */ 50374,
/* 3561 */ 1,
/* 3562 */ -3543,
/* 3563 */ 3,
/* 3564 */ 3568,
/* 3565 */ 50044,
/* 3566 */ -3564,
/* 3567 */ 1,
/* 3568 */ 0,
/* 3569 */ -3564,
/* 3570 */ 2,
/* 3571 */ 3575,
/* 3572 */ 1,
/* 3573 */ -3571,
/* 3574 */ 1,
/* 3575 */ 3579,
/* 3576 */ 1,
/* 3577 */ -3571,
/* 3578 */ 2,
/* 3579 */ 0,
/* 3580 */ 50257,
/* 3581 */ -3571,
/* 3582 */ 3,
/* 3583 */ 0,
/* 3584 */ 50293,
/* 3585 */ 50257,
/* 3586 */ 50123,
/* 3587 */ 1,
/* 3588 */ 50125,
/* 3589 */ -3583,
/* 3590 */ 1,
/* 3591 */ 3596,
/* 3592 */ 1,
/* 3593 */ 1,
/* 3594 */ -3591,
/* 3595 */ 1,
/* 3596 */ 0,
/* 3597 */ -3591,
/* 3598 */ 2,
/* 3599 */ 3606,
/* 3600 */ 50326,
/* 3601 */ 1,
/* 3602 */ 50257,
/* 3603 */ 50059,
/* 3604 */ -3599,
/* 3605 */ 1,
/* 3606 */ 3613,
/* 3607 */ 50372,
/* 3608 */ 1,
/* 3609 */ 1,
/* 3610 */ 50059,
/* 3611 */ -3599,
/* 3612 */ 2,
/* 3613 */ 3618,
/* 3614 */ 1,
/* 3615 */ 50059,
/* 3616 */ -3599,
/* 3617 */ 3,
/* 3618 */ 3625,
/* 3619 */ 50424,
/* 3620 */ 1,
/* 3621 */ 1,
/* 3622 */ 50059,
/* 3623 */ -3599,
/* 3624 */ 4,
/* 3625 */ 3632,
/* 3626 */ 50444,
/* 3627 */ 1,
/* 3628 */ 1,
/* 3629 */ 50059,
/* 3630 */ -3599,
/* 3631 */ 5,
/* 3632 */ 3639,
/* 3633 */ 50415,
/* 3634 */ 1,
/* 3635 */ 1,
/* 3636 */ 50059,
/* 3637 */ -3599,
/* 3638 */ 6,
/* 3639 */ 3646,
/* 3640 */ 50338,
/* 3641 */ 50353,
/* 3642 */ 1,
/* 3643 */ 50059,
/* 3644 */ -3599,
/* 3645 */ 7,
/* 3646 */ 3653,
/* 3647 */ 50470,
/* 3648 */ 50353,
/* 3649 */ 1,
/* 3650 */ 50059,
/* 3651 */ -3599,
/* 3652 */ 8,
/* 3653 */ 3660,
/* 3654 */ 50366,
/* 3655 */ 50353,
/* 3656 */ 1,
/* 3657 */ 50059,
/* 3658 */ -3599,
/* 3659 */ 9,
/* 3660 */ 3667,
/* 3661 */ 50371,
/* 3662 */ 50353,
/* 3663 */ 1,
/* 3664 */ 50059,
/* 3665 */ -3599,
/* 3666 */ 10,
/* 3667 */ 3671,
/* 3668 */ 1,
/* 3669 */ -3599,
/* 3670 */ 11,
/* 3671 */ 0,
/* 3672 */ 1,
/* 3673 */ -3599,
/* 3674 */ 12,
/* 3675 */ 3679,
/* 3676 */ 50058,
/* 3677 */ -3675,
/* 3678 */ 1,
/* 3679 */ 0,
/* 3680 */ -3675,
/* 3681 */ 2,
/* 3682 */ 3686,
/* 3683 */ 50058,
/* 3684 */ -3682,
/* 3685 */ 1,
/* 3686 */ 0,
/* 3687 */ -3682,
/* 3688 */ 2,
/* 3689 */ 3697,
/* 3690 */ 50277,
/* 3691 */ 1,
/* 3692 */ 1,
/* 3693 */ 1,
/* 3694 */ 1,
/* 3695 */ -3689,
/* 3696 */ 1,
/* 3697 */ 0,
/* 3698 */ -3689,
/* 3699 */ 2,
/* 3700 */ 3704,
/* 3701 */ 50058,
/* 3702 */ -3700,
/* 3703 */ 1,
/* 3704 */ 0,
/* 3705 */ -3700,
/* 3706 */ 2,
/* 3707 */ 3711,
/* 3708 */ 50044,
/* 3709 */ -3707,
/* 3710 */ 1,
/* 3711 */ 0,
/* 3712 */ -3707,
/* 3713 */ 2,
/* 3714 */ 3718,
/* 3715 */ 50058,
/* 3716 */ -3714,
/* 3717 */ 1,
/* 3718 */ 0,
/* 3719 */ -3714,
/* 3720 */ 2,
/* 3721 */ 3727,
/* 3722 */ 1,
/* 3723 */ 1,
/* 3724 */ 1,
/* 3725 */ -3721,
/* 3726 */ 1,
/* 3727 */ 0,
/* 3728 */ -3721,
/* 3729 */ 2,
/* 3730 */ 3734,
/* 3731 */ 50044,
/* 3732 */ -3730,
/* 3733 */ 1,
/* 3734 */ 0,
/* 3735 */ -3730,
/* 3736 */ 2,
/* 3737 */ 3741,
/* 3738 */ 50058,
/* 3739 */ -3737,
/* 3740 */ 1,
/* 3741 */ 0,
/* 3742 */ -3737,
/* 3743 */ 2,
/* 3744 */ 3750,
/* 3745 */ 50277,
/* 3746 */ 1,
/* 3747 */ 1,
/* 3748 */ -3744,
/* 3749 */ 1,
/* 3750 */ 0,
/* 3751 */ -3744,
/* 3752 */ 2,
/* 3753 */ 3757,
/* 3754 */ 50044,
/* 3755 */ -3753,
/* 3756 */ 1,
/* 3757 */ 0,
/* 3758 */ -3753,
/* 3759 */ 2,
/* 3760 */ 3764,
/* 3761 */ 50058,
/* 3762 */ -3760,
/* 3763 */ 1,
/* 3764 */ 0,
/* 3765 */ -3760,
/* 3766 */ 2,
/* 3767 */ 3773,
/* 3768 */ 1,
/* 3769 */ 1,
/* 3770 */ 1,
/* 3771 */ -3767,
/* 3772 */ 1,
/* 3773 */ 0,
/* 3774 */ -3767,
/* 3775 */ 2,
/* 3776 */ 3780,
/* 3777 */ 50044,
/* 3778 */ -3776,
/* 3779 */ 1,
/* 3780 */ 0,
/* 3781 */ -3776,
/* 3782 */ 2,
/* 3783 */ 3789,
/* 3784 */ 1,
/* 3785 */ 1,
/* 3786 */ 1,
/* 3787 */ -3783,
/* 3788 */ 1,
/* 3789 */ 0,
/* 3790 */ -3783,
/* 3791 */ 2,
/* 3792 */ 0,
/* 3793 */ 50044,
/* 3794 */ -3792,
/* 3795 */ 1,
/* 3796 */ 3802,
/* 3797 */ 1,
/* 3798 */ 1,
/* 3799 */ 1,
/* 3800 */ -3796,
/* 3801 */ 1,
/* 3802 */ 0,
/* 3803 */ -3796,
/* 3804 */ 2,
/* 3805 */ 0,
/* 3806 */ 50044,
/* 3807 */ -3805,
/* 3808 */ 1,
/* 3809 */ 3815,
/* 3810 */ 1,
/* 3811 */ 1,
/* 3812 */ 1,
/* 3813 */ -3809,
/* 3814 */ 1,
/* 3815 */ 0,
/* 3816 */ -3809,
/* 3817 */ 2,
/* 3818 */ 0,
/* 3819 */ 50044,
/* 3820 */ -3818,
/* 3821 */ 1,
/* 3822 */ 3828,
/* 3823 */ 1,
/* 3824 */ 1,
/* 3825 */ 1,
/* 3826 */ -3822,
/* 3827 */ 1,
/* 3828 */ 0,
/* 3829 */ -3822,
/* 3830 */ 2,
/* 3831 */ 0,
/* 3832 */ 50044,
/* 3833 */ -3831,
/* 3834 */ 1,
/* 3835 */ 3844,
/* 3836 */ 50440,
/* 3837 */ 50091,
/* 3838 */ 1,
/* 3839 */ 50044,
/* 3840 */ 1,
/* 3841 */ 50093,
/* 3842 */ -3835,
/* 3843 */ 1,
/* 3844 */ 3851,
/* 3845 */ 50440,
/* 3846 */ 50091,
/* 3847 */ 1,
/* 3848 */ 50093,
/* 3849 */ -3835,
/* 3850 */ 2,
/* 3851 */ 3856,
/* 3852 */ 1,
/* 3853 */ 1,
/* 3854 */ -3835,
/* 3855 */ 3,
/* 3856 */ 3863,
/* 3857 */ 1,
/* 3858 */ 50343,
/* 3859 */ 1,
/* 3860 */ 50459,
/* 3861 */ -3835,
/* 3862 */ 4,
/* 3863 */ 3870,
/* 3864 */ 1,
/* 3865 */ 1,
/* 3866 */ 1,
/* 3867 */ 50321,
/* 3868 */ -3835,
/* 3869 */ 5,
/* 3870 */ 3877,
/* 3871 */ 1,
/* 3872 */ 1,
/* 3873 */ 1,
/* 3874 */ 50465,
/* 3875 */ -3835,
/* 3876 */ 6,
/* 3877 */ 3882,
/* 3878 */ 50370,
/* 3879 */ 1,
/* 3880 */ -3835,
/* 3881 */ 7,
/* 3882 */ 0,
/* 3883 */ 50347,
/* 3884 */ 1,
/* 3885 */ -3835,
/* 3886 */ 8,
/* 3887 */ 3892,
/* 3888 */ 50295,
/* 3889 */ 50279,
/* 3890 */ -3887,
/* 3891 */ 1,
/* 3892 */ 0,
/* 3893 */ -3887,
/* 3894 */ 2,
/* 3895 */ 3899,
/* 3896 */ 50047,
/* 3897 */ -3895,
/* 3898 */ 1,
/* 3899 */ 0,
/* 3900 */ 50408,
/* 3901 */ -3895,
/* 3902 */ 2,
/* 3903 */ 3907,
/* 3904 */ 50047,
/* 3905 */ -3903,
/* 3906 */ 1,
/* 3907 */ 0,
/* 3908 */ 50408,
/* 3909 */ -3903,
/* 3910 */ 2,
/* 3911 */ 0,
/* 3912 */ 50328,
/* 3913 */ 50257,
/* 3914 */ 50058,
/* 3915 */ 1,
/* 3916 */ 50123,
/* 3917 */ 1,
/* 3918 */ 50125,
/* 3919 */ -3911,
/* 3920 */ 1,
/* 3921 */ 3926,
/* 3922 */ 1,
/* 3923 */ 1,
/* 3924 */ -3921,
/* 3925 */ 1,
/* 3926 */ 0,
/* 3927 */ -3921,
/* 3928 */ 2,
/* 3929 */ 3936,
/* 3930 */ 50437,
/* 3931 */ 1,
/* 3932 */ 1,
/* 3933 */ 50059,
/* 3934 */ -3929,
/* 3935 */ 1,
/* 3936 */ 3943,
/* 3937 */ 50359,
/* 3938 */ 1,
/* 3939 */ 1,
/* 3940 */ 50059,
/* 3941 */ -3929,
/* 3942 */ 2,
/* 3943 */ 3950,
/* 3944 */ 50326,
/* 3945 */ 1,
/* 3946 */ 50257,
/* 3947 */ 50059,
/* 3948 */ -3929,
/* 3949 */ 3,
/* 3950 */ 3954,
/* 3951 */ 1,
/* 3952 */ -3929,
/* 3953 */ 4,
/* 3954 */ 3958,
/* 3955 */ 1,
/* 3956 */ -3929,
/* 3957 */ 5,
/* 3958 */ 0,
/* 3959 */ 1,
/* 3960 */ -3929,
/* 3961 */ 6,
/* 3962 */ 3966,
/* 3963 */ 50058,
/* 3964 */ -3962,
/* 3965 */ 1,
/* 3966 */ 0,
/* 3967 */ -3962,
/* 3968 */ 2,
/* 3969 */ 3975,
/* 3970 */ 1,
/* 3971 */ 1,
/* 3972 */ 1,
/* 3973 */ -3969,
/* 3974 */ 1,
/* 3975 */ 0,
/* 3976 */ -3969,
/* 3977 */ 2,
/* 3978 */ 3982,
/* 3979 */ 50044,
/* 3980 */ -3978,
/* 3981 */ 1,
/* 3982 */ 0,
/* 3983 */ -3978,
/* 3984 */ 2,
/* 3985 */ 3989,
/* 3986 */ 50058,
/* 3987 */ -3985,
/* 3988 */ 1,
/* 3989 */ 0,
/* 3990 */ -3985,
/* 3991 */ 2,
/* 3992 */ 3998,
/* 3993 */ 1,
/* 3994 */ 1,
/* 3995 */ 1,
/* 3996 */ -3992,
/* 3997 */ 1,
/* 3998 */ 0,
/* 3999 */ -3992,
/* 4000 */ 2,
/* 4001 */ 4005,
/* 4002 */ 50044,
/* 4003 */ -4001,
/* 4004 */ 1,
/* 4005 */ 0,
/* 4006 */ -4001,
/* 4007 */ 2,
/* 4008 */ 4012,
/* 4009 */ 50058,
/* 4010 */ -4008,
/* 4011 */ 1,
/* 4012 */ 0,
/* 4013 */ -4008,
/* 4014 */ 2,
/* 4015 */ 0,
/* 4016 */ 50358,
/* 4017 */ 50257,
/* 4018 */ 50123,
/* 4019 */ 1,
/* 4020 */ 50125,
/* 4021 */ -4015,
/* 4022 */ 1,
/* 4023 */ 4028,
/* 4024 */ 1,
/* 4025 */ 1,
/* 4026 */ -4023,
/* 4027 */ 1,
/* 4028 */ 0,
/* 4029 */ -4023,
/* 4030 */ 2,
/* 4031 */ 4038,
/* 4032 */ 50329,
/* 4033 */ 1,
/* 4034 */ 1,
/* 4035 */ 50059,
/* 4036 */ -4031,
/* 4037 */ 1,
/* 4038 */ 4043,
/* 4039 */ 50311,
/* 4040 */ 50059,
/* 4041 */ -4031,
/* 4042 */ 2,
/* 4043 */ 4050,
/* 4044 */ 50350,
/* 4045 */ 1,
/* 4046 */ 1,
/* 4047 */ 50059,
/* 4048 */ -4031,
/* 4049 */ 3,
/* 4050 */ 4057,
/* 4051 */ 50326,
/* 4052 */ 1,
/* 4053 */ 50257,
/* 4054 */ 50059,
/* 4055 */ -4031,
/* 4056 */ 4,
/* 4057 */ 4064,
/* 4058 */ 50359,
/* 4059 */ 1,
/* 4060 */ 1,
/* 4061 */ 50059,
/* 4062 */ -4031,
/* 4063 */ 5,
/* 4064 */ 4068,
/* 4065 */ 1,
/* 4066 */ -4031,
/* 4067 */ 6,
/* 4068 */ 4072,
/* 4069 */ 1,
/* 4070 */ -4031,
/* 4071 */ 7,
/* 4072 */ 0,
/* 4073 */ 1,
/* 4074 */ -4031,
/* 4075 */ 8,
/* 4076 */ 4080,
/* 4077 */ 50058,
/* 4078 */ -4076,
/* 4079 */ 1,
/* 4080 */ 0,
/* 4081 */ -4076,
/* 4082 */ 2,
/* 4083 */ 4089,
/* 4084 */ 1,
/* 4085 */ 1,
/* 4086 */ 1,
/* 4087 */ -4083,
/* 4088 */ 1,
/* 4089 */ 0,
/* 4090 */ -4083,
/* 4091 */ 2,
/* 4092 */ 4096,
/* 4093 */ 50044,
/* 4094 */ -4092,
/* 4095 */ 1,
/* 4096 */ 0,
/* 4097 */ -4092,
/* 4098 */ 2,
/* 4099 */ 4103,
/* 4100 */ 50058,
/* 4101 */ -4099,
/* 4102 */ 1,
/* 4103 */ 0,
/* 4104 */ -4099,
/* 4105 */ 2,
/* 4106 */ 4112,
/* 4107 */ 1,
/* 4108 */ 1,
/* 4109 */ 1,
/* 4110 */ -4106,
/* 4111 */ 1,
/* 4112 */ 0,
/* 4113 */ -4106,
/* 4114 */ 2,
/* 4115 */ 4119,
/* 4116 */ 50124,
/* 4117 */ -4115,
/* 4118 */ 1,
/* 4119 */ 4123,
/* 4120 */ 50044,
/* 4121 */ -4115,
/* 4122 */ 2,
/* 4123 */ 0,
/* 4124 */ -4115,
/* 4125 */ 3,
/* 4126 */ 4130,
/* 4127 */ 50058,
/* 4128 */ -4126,
/* 4129 */ 1,
/* 4130 */ 0,
/* 4131 */ -4126,
/* 4132 */ 2,
/* 4133 */ 4137,
/* 4134 */ 50058,
/* 4135 */ -4133,
/* 4136 */ 1,
/* 4137 */ 0,
/* 4138 */ -4133,
/* 4139 */ 2,
/* 4140 */ 4146,
/* 4141 */ 1,
/* 4142 */ 1,
/* 4143 */ 1,
/* 4144 */ -4140,
/* 4145 */ 1,
/* 4146 */ 0,
/* 4147 */ -4140,
/* 4148 */ 2,
/* 4149 */ 4153,
/* 4150 */ 50044,
/* 4151 */ -4149,
/* 4152 */ 1,
/* 4153 */ 0,
/* 4154 */ -4149,
/* 4155 */ 2,
/* 4156 */ 0,
/* 4157 */ 50450,
/* 4158 */ 50257,
/* 4159 */ 50123,
/* 4160 */ 1,
/* 4161 */ 50125,
/* 4162 */ -4156,
/* 4163 */ 1,
/* 4164 */ 4169,
/* 4165 */ 1,
/* 4166 */ 1,
/* 4167 */ -4164,
/* 4168 */ 1,
/* 4169 */ 0,
/* 4170 */ -4164,
/* 4171 */ 2,
/* 4172 */ 4179,
/* 4173 */ 50326,
/* 4174 */ 1,
/* 4175 */ 50257,
/* 4176 */ 50059,
/* 4177 */ -4172,
/* 4178 */ 1,
/* 4179 */ 4183,
/* 4180 */ 1,
/* 4181 */ -4172,
/* 4182 */ 2,
/* 4183 */ 4191,
/* 4184 */ 50389,
/* 4185 */ 1,
/* 4186 */ 1,
/* 4187 */ 50037,
/* 4188 */ 50059,
/* 4189 */ -4172,
/* 4190 */ 3,
/* 4191 */ 4198,
/* 4192 */ 50407,
/* 4193 */ 1,
/* 4194 */ 1,
/* 4195 */ 50059,
/* 4196 */ -4172,
/* 4197 */ 4,
/* 4198 */ 4205,
/* 4199 */ 50383,
/* 4200 */ 1,
/* 4201 */ 1,
/* 4202 */ 50059,
/* 4203 */ -4172,
/* 4204 */ 5,
/* 4205 */ 4212,
/* 4206 */ 50350,
/* 4207 */ 1,
/* 4208 */ 1,
/* 4209 */ 50059,
/* 4210 */ -4172,
/* 4211 */ 6,
/* 4212 */ 4216,
/* 4213 */ 1,
/* 4214 */ -4172,
/* 4215 */ 7,
/* 4216 */ 4220,
/* 4217 */ 1,
/* 4218 */ -4172,
/* 4219 */ 8,
/* 4220 */ 0,
/* 4221 */ 1,
/* 4222 */ -4172,
/* 4223 */ 9,
/* 4224 */ 4228,
/* 4225 */ 50058,
/* 4226 */ -4224,
/* 4227 */ 1,
/* 4228 */ 0,
/* 4229 */ -4224,
/* 4230 */ 2,
/* 4231 */ 4235,
/* 4232 */ 50058,
/* 4233 */ -4231,
/* 4234 */ 1,
/* 4235 */ 0,
/* 4236 */ -4231,
/* 4237 */ 2,
/* 4238 */ 4242,
/* 4239 */ 50058,
/* 4240 */ -4238,
/* 4241 */ 1,
/* 4242 */ 0,
/* 4243 */ -4238,
/* 4244 */ 2,
/* 4245 */ 4249,
/* 4246 */ 50058,
/* 4247 */ -4245,
/* 4248 */ 1,
/* 4249 */ 0,
/* 4250 */ -4245,
/* 4251 */ 2,
/* 4252 */ 4256,
/* 4253 */ 50058,
/* 4254 */ -4252,
/* 4255 */ 1,
/* 4256 */ 0,
/* 4257 */ -4252,
/* 4258 */ 2,
/* 4259 */ 4265,
/* 4260 */ 1,
/* 4261 */ 1,
/* 4262 */ 1,
/* 4263 */ -4259,
/* 4264 */ 1,
/* 4265 */ 0,
/* 4266 */ -4259,
/* 4267 */ 2,
/* 4268 */ 4272,
/* 4269 */ 50124,
/* 4270 */ -4268,
/* 4271 */ 1,
/* 4272 */ 4276,
/* 4273 */ 50044,
/* 4274 */ -4268,
/* 4275 */ 2,
/* 4276 */ 0,
/* 4277 */ -4268,
/* 4278 */ 3,
/* 4279 */ 0,
/* 4280 */ 50422,
/* 4281 */ 50257,
/* 4282 */ 50058,
/* 4283 */ 1,
/* 4284 */ 50123,
/* 4285 */ 1,
/* 4286 */ 50125,
/* 4287 */ -4279,
/* 4288 */ 1,
/* 4289 */ 4294,
/* 4290 */ 1,
/* 4291 */ 1,
/* 4292 */ -4289,
/* 4293 */ 1,
/* 4294 */ 0,
/* 4295 */ -4289,
/* 4296 */ 2,
/* 4297 */ 4304,
/* 4298 */ 50326,
/* 4299 */ 1,
/* 4300 */ 50257,
/* 4301 */ 50059,
/* 4302 */ -4297,
/* 4303 */ 1,
/* 4304 */ 4311,
/* 4305 */ 50464,
/* 4306 */ 1,
/* 4307 */ 1,
/* 4308 */ 50059,
/* 4309 */ -4297,
/* 4310 */ 2,
/* 4311 */ 4318,
/* 4312 */ 50351,
/* 4313 */ 1,
/* 4314 */ 1,
/* 4315 */ 50059,
/* 4316 */ -4297,
/* 4317 */ 3,
/* 4318 */ 4325,
/* 4319 */ 50327,
/* 4320 */ 1,
/* 4321 */ 1,
/* 4322 */ 50059,
/* 4323 */ -4297,
/* 4324 */ 4,
/* 4325 */ 4332,
/* 4326 */ 50325,
/* 4327 */ 1,
/* 4328 */ 1,
/* 4329 */ 50059,
/* 4330 */ -4297,
/* 4331 */ 5,
/* 4332 */ 4339,
/* 4333 */ 50431,
/* 4334 */ 1,
/* 4335 */ 1,
/* 4336 */ 50059,
/* 4337 */ -4297,
/* 4338 */ 6,
/* 4339 */ 4346,
/* 4340 */ 50426,
/* 4341 */ 1,
/* 4342 */ 1,
/* 4343 */ 50059,
/* 4344 */ -4297,
/* 4345 */ 7,
/* 4346 */ 4353,
/* 4347 */ 50350,
/* 4348 */ 1,
/* 4349 */ 1,
/* 4350 */ 50059,
/* 4351 */ -4297,
/* 4352 */ 8,
/* 4353 */ 4360,
/* 4354 */ 50360,
/* 4355 */ 50058,
/* 4356 */ 50260,
/* 4357 */ 50059,
/* 4358 */ -4297,
/* 4359 */ 9,
/* 4360 */ 4367,
/* 4361 */ 50455,
/* 4362 */ 50058,
/* 4363 */ 1,
/* 4364 */ 50059,
/* 4365 */ -4297,
/* 4366 */ 10,
/* 4367 */ 4373,
/* 4368 */ 50423,
/* 4369 */ 50058,
/* 4370 */ 1,
/* 4371 */ -4297,
/* 4372 */ 11,
/* 4373 */ 4379,
/* 4374 */ 50398,
/* 4375 */ 50058,
/* 4376 */ 1,
/* 4377 */ -4297,
/* 4378 */ 12,
/* 4379 */ 4383,
/* 4380 */ 1,
/* 4381 */ -4297,
/* 4382 */ 13,
/* 4383 */ 4387,
/* 4384 */ 1,
/* 4385 */ -4297,
/* 4386 */ 14,
/* 4387 */ 0,
/* 4388 */ 1,
/* 4389 */ -4297,
/* 4390 */ 15,
/* 4391 */ 4395,
/* 4392 */ 50058,
/* 4393 */ -4391,
/* 4394 */ 1,
/* 4395 */ 0,
/* 4396 */ -4391,
/* 4397 */ 2,
/* 4398 */ 4402,
/* 4399 */ 50058,
/* 4400 */ -4398,
/* 4401 */ 1,
/* 4402 */ 0,
/* 4403 */ -4398,
/* 4404 */ 2,
/* 4405 */ 4409,
/* 4406 */ 50058,
/* 4407 */ -4405,
/* 4408 */ 1,
/* 4409 */ 0,
/* 4410 */ -4405,
/* 4411 */ 2,
/* 4412 */ 4416,
/* 4413 */ 50058,
/* 4414 */ -4412,
/* 4415 */ 1,
/* 4416 */ 0,
/* 4417 */ -4412,
/* 4418 */ 2,
/* 4419 */ 4423,
/* 4420 */ 50058,
/* 4421 */ -4419,
/* 4422 */ 1,
/* 4423 */ 0,
/* 4424 */ -4419,
/* 4425 */ 2,
/* 4426 */ 4430,
/* 4427 */ 50058,
/* 4428 */ -4426,
/* 4429 */ 1,
/* 4430 */ 0,
/* 4431 */ -4426,
/* 4432 */ 2,
/* 4433 */ 4437,
/* 4434 */ 50058,
/* 4435 */ -4433,
/* 4436 */ 1,
/* 4437 */ 0,
/* 4438 */ -4433,
/* 4439 */ 2,
/* 4440 */ 4446,
/* 4441 */ 1,
/* 4442 */ 1,
/* 4443 */ 1,
/* 4444 */ -4440,
/* 4445 */ 1,
/* 4446 */ 0,
/* 4447 */ -4440,
/* 4448 */ 2,
/* 4449 */ 4453,
/* 4450 */ 50044,
/* 4451 */ -4449,
/* 4452 */ 1,
/* 4453 */ 4457,
/* 4454 */ 50124,
/* 4455 */ -4449,
/* 4456 */ 2,
/* 4457 */ 0,
/* 4458 */ -4449,
/* 4459 */ 3,
/* 4460 */ 4464,
/* 4461 */ 50058,
/* 4462 */ -4460,
/* 4463 */ 1,
/* 4464 */ 0,
/* 4465 */ -4460,
/* 4466 */ 2,
/* 4467 */ 4473,
/* 4468 */ 1,
/* 4469 */ 1,
/* 4470 */ 1,
/* 4471 */ -4467,
/* 4472 */ 1,
/* 4473 */ 0,
/* 4474 */ -4467,
/* 4475 */ 2,
/* 4476 */ 4480,
/* 4477 */ 50124,
/* 4478 */ -4476,
/* 4479 */ 1,
/* 4480 */ 4484,
/* 4481 */ 50044,
/* 4482 */ -4476,
/* 4483 */ 2,
/* 4484 */ 0,
/* 4485 */ -4476,
/* 4486 */ 3,
/* 4487 */ 4496,
/* 4488 */ 50259,
/* 4489 */ 50058,
/* 4490 */ 1,
/* 4491 */ 1,
/* 4492 */ 1,
/* 4493 */ 1,
/* 4494 */ -4487,
/* 4495 */ 1,
/* 4496 */ 0,
/* 4497 */ -4487,
/* 4498 */ 2,
/* 4499 */ 4506,
/* 4500 */ 50091,
/* 4501 */ 1,
/* 4502 */ 50093,
/* 4503 */ 1,
/* 4504 */ -4499,
/* 4505 */ 1,
/* 4506 */ 4513,
/* 4507 */ 50471,
/* 4508 */ 1,
/* 4509 */ 1,
/* 4510 */ 1,
/* 4511 */ -4499,
/* 4512 */ 2,
/* 4513 */ 4519,
/* 4514 */ 50294,
/* 4515 */ 1,
/* 4516 */ 1,
/* 4517 */ -4499,
/* 4518 */ 3,
/* 4519 */ 0,
/* 4520 */ -4499,
/* 4521 */ 4,
/* 4522 */ 4526,
/* 4523 */ 1,
/* 4524 */ -4522,
/* 4525 */ 1,
/* 4526 */ 4532,
/* 4527 */ 50419,
/* 4528 */ 50372,
/* 4529 */ 1,
/* 4530 */ -4522,
/* 4531 */ 2,
/* 4532 */ 0,
/* 4533 */ 50419,
/* 4534 */ 50388,
/* 4535 */ 1,
/* 4536 */ -4522,
/* 4537 */ 3,
/* 4538 */ 4542,
/* 4539 */ 1,
/* 4540 */ -4538,
/* 4541 */ 1,
/* 4542 */ 0,
/* 4543 */ -4538,
/* 4544 */ 2,
/* 4545 */ 4549,
/* 4546 */ 1,
/* 4547 */ -4545,
/* 4548 */ 1,
/* 4549 */ 0,
/* 4550 */ -4545,
/* 4551 */ 2,
/* 4552 */ 4557,
/* 4553 */ 50399,
/* 4554 */ 1,
/* 4555 */ -4552,
/* 4556 */ 1,
/* 4557 */ 4565,
/* 4558 */ 50399,
/* 4559 */ 50419,
/* 4560 */ 1,
/* 4561 */ 50457,
/* 4562 */ 1,
/* 4563 */ -4552,
/* 4564 */ 2,
/* 4565 */ 4570,
/* 4566 */ 50399,
/* 4567 */ 1,
/* 4568 */ -4552,
/* 4569 */ 3,
/* 4570 */ 0,
/* 4571 */ -4552,
/* 4572 */ 4,
/* 4573 */ 4577,
/* 4574 */ 50044,
/* 4575 */ -4573,
/* 4576 */ 1,
/* 4577 */ 0,
/* 4578 */ -4573,
/* 4579 */ 2,
/* 4580 */ 4585,
/* 4581 */ 1,
/* 4582 */ 1,
/* 4583 */ -4580,
/* 4584 */ 1,
/* 4585 */ 0,
/* 4586 */ -4580,
/* 4587 */ 2,
/* 4588 */ 4593,
/* 4589 */ 1,
/* 4590 */ 1,
/* 4591 */ -4588,
/* 4592 */ 1,
/* 4593 */ 0,
/* 4594 */ -4588,
/* 4595 */ 2,
/* 4596 */ 0,
/* 4597 */ 50449,
/* 4598 */ 50257,
/* 4599 */ 50058,
/* 4600 */ 1,
/* 4601 */ 1,
/* 4602 */ 50123,
/* 4603 */ 1,
/* 4604 */ 50125,
/* 4605 */ -4596,
/* 4606 */ 1,
/* 4607 */ 4612,
/* 4608 */ 50353,
/* 4609 */ 1,
/* 4610 */ -4607,
/* 4611 */ 1,
/* 4612 */ 0,
/* 4613 */ -4607,
/* 4614 */ 2,
/* 4615 */ 4620,
/* 4616 */ 1,
/* 4617 */ 1,
/* 4618 */ -4615,
/* 4619 */ 1,
/* 4620 */ 0,
/* 4621 */ -4615,
/* 4622 */ 2,
/* 4623 */ 4630,
/* 4624 */ 50326,
/* 4625 */ 1,
/* 4626 */ 50257,
/* 4627 */ 50059,
/* 4628 */ -4623,
/* 4629 */ 1,
/* 4630 */ 4637,
/* 4631 */ 50353,
/* 4632 */ 1,
/* 4633 */ 1,
/* 4634 */ 50059,
/* 4635 */ -4623,
/* 4636 */ 2,
/* 4637 */ 4644,
/* 4638 */ 50390,
/* 4639 */ 1,
/* 4640 */ 1,
/* 4641 */ 50059,
/* 4642 */ -4623,
/* 4643 */ 3,
/* 4644 */ 4648,
/* 4645 */ 1,
/* 4646 */ -4623,
/* 4647 */ 4,
/* 4648 */ 4655,
/* 4649 */ 1,
/* 4650 */ 1,
/* 4651 */ 1,
/* 4652 */ 50059,
/* 4653 */ -4623,
/* 4654 */ 5,
/* 4655 */ 4662,
/* 4656 */ 50364,
/* 4657 */ 1,
/* 4658 */ 1,
/* 4659 */ 50059,
/* 4660 */ -4623,
/* 4661 */ 6,
/* 4662 */ 4669,
/* 4663 */ 50366,
/* 4664 */ 1,
/* 4665 */ 1,
/* 4666 */ 50059,
/* 4667 */ -4623,
/* 4668 */ 7,
/* 4669 */ 4676,
/* 4670 */ 50323,
/* 4671 */ 1,
/* 4672 */ 1,
/* 4673 */ 50059,
/* 4674 */ -4623,
/* 4675 */ 8,
/* 4676 */ 4683,
/* 4677 */ 50292,
/* 4678 */ 1,
/* 4679 */ 1,
/* 4680 */ 50059,
/* 4681 */ -4623,
/* 4682 */ 9,
/* 4683 */ 4690,
/* 4684 */ 50319,
/* 4685 */ 1,
/* 4686 */ 1,
/* 4687 */ 50059,
/* 4688 */ -4623,
/* 4689 */ 10,
/* 4690 */ 4697,
/* 4691 */ 50410,
/* 4692 */ 1,
/* 4693 */ 1,
/* 4694 */ 50059,
/* 4695 */ -4623,
/* 4696 */ 11,
/* 4697 */ 4705,
/* 4698 */ 50306,
/* 4699 */ 50374,
/* 4700 */ 1,
/* 4701 */ 1,
/* 4702 */ 50059,
/* 4703 */ -4623,
/* 4704 */ 12,
/* 4705 */ 4712,
/* 4706 */ 50431,
/* 4707 */ 1,
/* 4708 */ 1,
/* 4709 */ 50059,
/* 4710 */ -4623,
/* 4711 */ 13,
/* 4712 */ 4719,
/* 4713 */ 50435,
/* 4714 */ 1,
/* 4715 */ 1,
/* 4716 */ 50059,
/* 4717 */ -4623,
/* 4718 */ 14,
/* 4719 */ 4726,
/* 4720 */ 50389,
/* 4721 */ 1,
/* 4722 */ 1,
/* 4723 */ 50059,
/* 4724 */ -4623,
/* 4725 */ 15,
/* 4726 */ 4733,
/* 4727 */ 50277,
/* 4728 */ 1,
/* 4729 */ 1,
/* 4730 */ 1,
/* 4731 */ -4623,
/* 4732 */ 16,
/* 4733 */ 4741,
/* 4734 */ 50296,
/* 4735 */ 1,
/* 4736 */ 1,
/* 4737 */ 1,
/* 4738 */ 50059,
/* 4739 */ -4623,
/* 4740 */ 17,
/* 4741 */ 4749,
/* 4742 */ 50440,
/* 4743 */ 50091,
/* 4744 */ 1,
/* 4745 */ 50093,
/* 4746 */ 50059,
/* 4747 */ -4623,
/* 4748 */ 18,
/* 4749 */ 4759,
/* 4750 */ 50440,
/* 4751 */ 50091,
/* 4752 */ 1,
/* 4753 */ 50044,
/* 4754 */ 1,
/* 4755 */ 50093,
/* 4756 */ 50059,
/* 4757 */ -4623,
/* 4758 */ 19,
/* 4759 */ 4771,
/* 4760 */ 50440,
/* 4761 */ 50091,
/* 4762 */ 1,
/* 4763 */ 50044,
/* 4764 */ 1,
/* 4765 */ 50044,
/* 4766 */ 1,
/* 4767 */ 50093,
/* 4768 */ 50059,
/* 4769 */ -4623,
/* 4770 */ 20,
/* 4771 */ 4785,
/* 4772 */ 50440,
/* 4773 */ 50091,
/* 4774 */ 1,
/* 4775 */ 50044,
/* 4776 */ 1,
/* 4777 */ 50044,
/* 4778 */ 1,
/* 4779 */ 50044,
/* 4780 */ 1,
/* 4781 */ 50093,
/* 4782 */ 50059,
/* 4783 */ -4623,
/* 4784 */ 21,
/* 4785 */ 4792,
/* 4786 */ 50424,
/* 4787 */ 1,
/* 4788 */ 1,
/* 4789 */ 50059,
/* 4790 */ -4623,
/* 4791 */ 22,
/* 4792 */ 4799,
/* 4793 */ 50369,
/* 4794 */ 1,
/* 4795 */ 1,
/* 4796 */ 50059,
/* 4797 */ -4623,
/* 4798 */ 23,
/* 4799 */ 4807,
/* 4800 */ 50346,
/* 4801 */ 1,
/* 4802 */ 1,
/* 4803 */ 1,
/* 4804 */ 50059,
/* 4805 */ -4623,
/* 4806 */ 24,
/* 4807 */ 4814,
/* 4808 */ 50437,
/* 4809 */ 1,
/* 4810 */ 1,
/* 4811 */ 50059,
/* 4812 */ -4623,
/* 4813 */ 25,
/* 4814 */ 4821,
/* 4815 */ 50330,
/* 4816 */ 1,
/* 4817 */ 1,
/* 4818 */ 50059,
/* 4819 */ -4623,
/* 4820 */ 26,
/* 4821 */ 4828,
/* 4822 */ 50409,
/* 4823 */ 1,
/* 4824 */ 1,
/* 4825 */ 50059,
/* 4826 */ -4623,
/* 4827 */ 27,
/* 4828 */ 4834,
/* 4829 */ 50350,
/* 4830 */ 1,
/* 4831 */ 1,
/* 4832 */ -4623,
/* 4833 */ 28,
/* 4834 */ 4838,
/* 4835 */ 1,
/* 4836 */ -4623,
/* 4837 */ 29,
/* 4838 */ 4842,
/* 4839 */ 1,
/* 4840 */ -4623,
/* 4841 */ 30,
/* 4842 */ 4846,
/* 4843 */ 1,
/* 4844 */ -4623,
/* 4845 */ 31,
/* 4846 */ 0,
/* 4847 */ 1,
/* 4848 */ -4623,
/* 4849 */ 32,
/* 4850 */ 4854,
/* 4851 */ 50058,
/* 4852 */ -4850,
/* 4853 */ 1,
/* 4854 */ 0,
/* 4855 */ -4850,
/* 4856 */ 2,
/* 4857 */ 4861,
/* 4858 */ 50058,
/* 4859 */ -4857,
/* 4860 */ 1,
/* 4861 */ 0,
/* 4862 */ -4857,
/* 4863 */ 2,
/* 4864 */ 4868,
/* 4865 */ 50058,
/* 4866 */ -4864,
/* 4867 */ 1,
/* 4868 */ 0,
/* 4869 */ -4864,
/* 4870 */ 2,
/* 4871 */ 4875,
/* 4872 */ 50374,
/* 4873 */ -4871,
/* 4874 */ 1,
/* 4875 */ 0,
/* 4876 */ 50307,
/* 4877 */ -4871,
/* 4878 */ 2,
/* 4879 */ 4883,
/* 4880 */ 50058,
/* 4881 */ -4879,
/* 4882 */ 1,
/* 4883 */ 0,
/* 4884 */ -4879,
/* 4885 */ 2,
/* 4886 */ 4890,
/* 4887 */ 50058,
/* 4888 */ -4886,
/* 4889 */ 1,
/* 4890 */ 0,
/* 4891 */ -4886,
/* 4892 */ 2,
/* 4893 */ 4897,
/* 4894 */ 50058,
/* 4895 */ -4893,
/* 4896 */ 1,
/* 4897 */ 0,
/* 4898 */ -4893,
/* 4899 */ 2,
/* 4900 */ 4904,
/* 4901 */ 50058,
/* 4902 */ -4900,
/* 4903 */ 1,
/* 4904 */ 0,
/* 4905 */ -4900,
/* 4906 */ 2,
/* 4907 */ 4911,
/* 4908 */ 50058,
/* 4909 */ -4907,
/* 4910 */ 1,
/* 4911 */ 0,
/* 4912 */ -4907,
/* 4913 */ 2,
/* 4914 */ 4918,
/* 4915 */ 50058,
/* 4916 */ -4914,
/* 4917 */ 1,
/* 4918 */ 0,
/* 4919 */ -4914,
/* 4920 */ 2,
/* 4921 */ 4925,
/* 4922 */ 50058,
/* 4923 */ -4921,
/* 4924 */ 1,
/* 4925 */ 0,
/* 4926 */ -4921,
/* 4927 */ 2,
/* 4928 */ 4932,
/* 4929 */ 50058,
/* 4930 */ -4928,
/* 4931 */ 1,
/* 4932 */ 0,
/* 4933 */ -4928,
/* 4934 */ 2,
/* 4935 */ 4939,
/* 4936 */ 50058,
/* 4937 */ -4935,
/* 4938 */ 1,
/* 4939 */ 0,
/* 4940 */ -4935,
/* 4941 */ 2,
/* 4942 */ 4946,
/* 4943 */ 50058,
/* 4944 */ -4942,
/* 4945 */ 1,
/* 4946 */ 0,
/* 4947 */ -4942,
/* 4948 */ 2,
/* 4949 */ 4953,
/* 4950 */ 50058,
/* 4951 */ -4949,
/* 4952 */ 1,
/* 4953 */ 0,
/* 4954 */ -4949,
/* 4955 */ 2,
/* 4956 */ 4960,
/* 4957 */ 50058,
/* 4958 */ -4956,
/* 4959 */ 1,
/* 4960 */ 0,
/* 4961 */ -4956,
/* 4962 */ 2,
/* 4963 */ 4967,
/* 4964 */ 50044,
/* 4965 */ -4963,
/* 4966 */ 1,
/* 4967 */ 0,
/* 4968 */ 50059,
/* 4969 */ -4963,
/* 4970 */ 2,
/* 4971 */ 4975,
/* 4972 */ 50058,
/* 4973 */ -4971,
/* 4974 */ 1,
/* 4975 */ 0,
/* 4976 */ -4971,
/* 4977 */ 2,
/* 4978 */ 4984,
/* 4979 */ 50044,
/* 4980 */ 1,
/* 4981 */ 1,
/* 4982 */ -4978,
/* 4983 */ 1,
/* 4984 */ 0,
/* 4985 */ -4978,
/* 4986 */ 2,
/* 4987 */ 4991,
/* 4988 */ 50058,
/* 4989 */ -4987,
/* 4990 */ 1,
/* 4991 */ 0,
/* 4992 */ -4987,
/* 4993 */ 2,
/* 4994 */ 5000,
/* 4995 */ 1,
/* 4996 */ 1,
/* 4997 */ 1,
/* 4998 */ -4994,
/* 4999 */ 1,
/* 5000 */ 0,
/* 5001 */ -4994,
/* 5002 */ 2,
/* 5003 */ 5007,
/* 5004 */ 1,
/* 5005 */ -5003,
/* 5006 */ 1,
/* 5007 */ 0,
/* 5008 */ 1,
/* 5009 */ 1,
/* 5010 */ -5003,
/* 5011 */ 2,
/* 5012 */ 5016,
/* 5013 */ 50126,
/* 5014 */ -5012,
/* 5015 */ 1,
/* 5016 */ 0,
/* 5017 */ 50033,
/* 5018 */ -5012,
/* 5019 */ 2,
/* 5020 */ 5024,
/* 5021 */ 50044,
/* 5022 */ -5020,
/* 5023 */ 1,
/* 5024 */ 0,
/* 5025 */ -5020,
/* 5026 */ 2,
/* 5027 */ 5031,
/* 5028 */ 50058,
/* 5029 */ -5027,
/* 5030 */ 1,
/* 5031 */ 0,
/* 5032 */ -5027,
/* 5033 */ 2,
/* 5034 */ 5040,
/* 5035 */ 1,
/* 5036 */ 1,
/* 5037 */ 1,
/* 5038 */ -5034,
/* 5039 */ 1,
/* 5040 */ 0,
/* 5041 */ -5034,
/* 5042 */ 2,
/* 5043 */ 5047,
/* 5044 */ 1,
/* 5045 */ -5043,
/* 5046 */ 1,
/* 5047 */ 0,
/* 5048 */ 1,
/* 5049 */ 1,
/* 5050 */ -5043,
/* 5051 */ 2,
/* 5052 */ 5056,
/* 5053 */ 50126,
/* 5054 */ -5052,
/* 5055 */ 1,
/* 5056 */ 0,
/* 5057 */ 50033,
/* 5058 */ -5052,
/* 5059 */ 2,
/* 5060 */ 5064,
/* 5061 */ 50044,
/* 5062 */ -5060,
/* 5063 */ 1,
/* 5064 */ 0,
/* 5065 */ -5060,
/* 5066 */ 2,
/* 5067 */ 5071,
/* 5068 */ 50058,
/* 5069 */ -5067,
/* 5070 */ 1,
/* 5071 */ 0,
/* 5072 */ -5067,
/* 5073 */ 2,
/* 5074 */ 5080,
/* 5075 */ 1,
/* 5076 */ 50044,
/* 5077 */ 1,
/* 5078 */ -5074,
/* 5079 */ 1,
/* 5080 */ 0,
/* 5081 */ -5074,
/* 5082 */ 2,
/* 5083 */ 5087,
/* 5084 */ 50058,
/* 5085 */ -5083,
/* 5086 */ 1,
/* 5087 */ 0,
/* 5088 */ -5083,
/* 5089 */ 2,
/* 5090 */ 5096,
/* 5091 */ 1,
/* 5092 */ 1,
/* 5093 */ 1,
/* 5094 */ -5090,
/* 5095 */ 1,
/* 5096 */ 0,
/* 5097 */ -5090,
/* 5098 */ 2,
/* 5099 */ 5103,
/* 5100 */ 50044,
/* 5101 */ -5099,
/* 5102 */ 1,
/* 5103 */ 0,
/* 5104 */ -5099,
/* 5105 */ 2,
/* 5106 */ 5110,
/* 5107 */ 50058,
/* 5108 */ -5106,
/* 5109 */ 1,
/* 5110 */ 0,
/* 5111 */ -5106,
/* 5112 */ 2,
/* 5113 */ 5119,
/* 5114 */ 1,
/* 5115 */ 1,
/* 5116 */ 1,
/* 5117 */ -5113,
/* 5118 */ 1,
/* 5119 */ 0,
/* 5120 */ -5113,
/* 5121 */ 2,
/* 5122 */ 5126,
/* 5123 */ 50044,
/* 5124 */ -5122,
/* 5125 */ 1,
/* 5126 */ 0,
/* 5127 */ -5122,
/* 5128 */ 2,
/* 5129 */ 5133,
/* 5130 */ 50058,
/* 5131 */ -5129,
/* 5132 */ 1,
/* 5133 */ 0,
/* 5134 */ -5129,
/* 5135 */ 2,
/* 5136 */ 5142,
/* 5137 */ 1,
/* 5138 */ 1,
/* 5139 */ 1,
/* 5140 */ -5136,
/* 5141 */ 1,
/* 5142 */ 0,
/* 5143 */ -5136,
/* 5144 */ 2,
/* 5145 */ 5149,
/* 5146 */ 50044,
/* 5147 */ -5145,
/* 5148 */ 1,
/* 5149 */ 0,
/* 5150 */ -5145,
/* 5151 */ 2,
/* 5152 */ 5156,
/* 5153 */ 50058,
/* 5154 */ -5152,
/* 5155 */ 1,
/* 5156 */ 0,
/* 5157 */ -5152,
/* 5158 */ 2,
/* 5159 */ 5165,
/* 5160 */ 1,
/* 5161 */ 1,
/* 5162 */ 1,
/* 5163 */ -5159,
/* 5164 */ 1,
/* 5165 */ 0,
/* 5166 */ -5159,
/* 5167 */ 2,
/* 5168 */ 5172,
/* 5169 */ 1,
/* 5170 */ -5168,
/* 5171 */ 1,
/* 5172 */ 0,
/* 5173 */ 1,
/* 5174 */ 1,
/* 5175 */ -5168,
/* 5176 */ 2,
/* 5177 */ 5181,
/* 5178 */ 50126,
/* 5179 */ -5177,
/* 5180 */ 1,
/* 5181 */ 0,
/* 5182 */ 50033,
/* 5183 */ -5177,
/* 5184 */ 2,
/* 5185 */ 5189,
/* 5186 */ 50044,
/* 5187 */ -5185,
/* 5188 */ 1,
/* 5189 */ 0,
/* 5190 */ 50059,
/* 5191 */ -5185,
/* 5192 */ 2,
/* 5193 */ 5203,
/* 5194 */ 1,
/* 5195 */ 50353,
/* 5196 */ 1,
/* 5197 */ 1,
/* 5198 */ 1,
/* 5199 */ 1,
/* 5200 */ 1,
/* 5201 */ -5193,
/* 5202 */ 1,
/* 5203 */ 0,
/* 5204 */ 1,
/* 5205 */ 50353,
/* 5206 */ 1,
/* 5207 */ 1,
/* 5208 */ -5193,
/* 5209 */ 2,
/* 5210 */ 5214,
/* 5211 */ 1,
/* 5212 */ -5210,
/* 5213 */ 1,
/* 5214 */ 5219,
/* 5215 */ 1,
/* 5216 */ 50037,
/* 5217 */ -5210,
/* 5218 */ 2,
/* 5219 */ 0,
/* 5220 */ -5210,
/* 5221 */ 3,
/* 5222 */ 5226,
/* 5223 */ 1,
/* 5224 */ -5222,
/* 5225 */ 1,
/* 5226 */ 0,
/* 5227 */ -5222,
/* 5228 */ 2,
/* 5229 */ 5233,
/* 5230 */ 50319,
/* 5231 */ -5229,
/* 5232 */ 1,
/* 5233 */ 0,
/* 5234 */ -5229,
/* 5235 */ 2,
/* 5236 */ 5243,
/* 5237 */ 50040,
/* 5238 */ 1,
/* 5239 */ 1,
/* 5240 */ 50041,
/* 5241 */ -5236,
/* 5242 */ 1,
/* 5243 */ 0,
/* 5244 */ -5236,
/* 5245 */ 2,
/* 5246 */ 5250,
/* 5247 */ 50322,
/* 5248 */ -5246,
/* 5249 */ 1,
/* 5250 */ 5254,
/* 5251 */ 50307,
/* 5252 */ -5246,
/* 5253 */ 2,
/* 5254 */ 0,
/* 5255 */ 50374,
/* 5256 */ -5246,
/* 5257 */ 3,
/* 5258 */ 5267,
/* 5259 */ 50288,
/* 5260 */ 1,
/* 5261 */ 1,
/* 5262 */ 1,
/* 5263 */ 1,
/* 5264 */ 1,
/* 5265 */ -5258,
/* 5266 */ 1,
/* 5267 */ 0,
/* 5268 */ -5258,
/* 5269 */ 2,
/* 5270 */ 5274,
/* 5271 */ 1,
/* 5272 */ -5270,
/* 5273 */ 1,
/* 5274 */ 0,
/* 5275 */ -5270,
/* 5276 */ 2,
/* 5277 */ 5281,
/* 5278 */ 1,
/* 5279 */ -5277,
/* 5280 */ 1,
/* 5281 */ 0,
/* 5282 */ -5277,
/* 5283 */ 2,
/* 5284 */ 5288,
/* 5285 */ 50319,
/* 5286 */ -5284,
/* 5287 */ 1,
/* 5288 */ 0,
/* 5289 */ -5284,
/* 5290 */ 2,
/* 5291 */ 5298,
/* 5292 */ 50040,
/* 5293 */ 1,
/* 5294 */ 1,
/* 5295 */ 50041,
/* 5296 */ -5291,
/* 5297 */ 1,
/* 5298 */ 0,
/* 5299 */ -5291,
/* 5300 */ 2,
/* 5301 */ 5305,
/* 5302 */ 50322,
/* 5303 */ -5301,
/* 5304 */ 1,
/* 5305 */ 5309,
/* 5306 */ 50307,
/* 5307 */ -5301,
/* 5308 */ 2,
/* 5309 */ 0,
/* 5310 */ 50374,
/* 5311 */ -5301,
/* 5312 */ 3,
/* 5313 */ 5317,
/* 5314 */ 1,
/* 5315 */ -5313,
/* 5316 */ 1,
/* 5317 */ 0,
/* 5318 */ -5313,
/* 5319 */ 2,
/* 5320 */ 0,
/* 5321 */ 50261,
/* 5322 */ -5320,
/* 5323 */ 1,
/* 5324 */ 0,
/* 5325 */ 50341,
/* 5326 */ 50257,
/* 5327 */ 50058,
/* 5328 */ 1,
/* 5329 */ 50123,
/* 5330 */ 1,
/* 5331 */ 50125,
/* 5332 */ -5324,
/* 5333 */ 1,
/* 5334 */ 5339,
/* 5335 */ 1,
/* 5336 */ 1,
/* 5337 */ -5334,
/* 5338 */ 1,
/* 5339 */ 0,
/* 5340 */ -5334,
/* 5341 */ 2,
/* 5342 */ 5349,
/* 5343 */ 50326,
/* 5344 */ 1,
/* 5345 */ 50257,
/* 5346 */ 50059,
/* 5347 */ -5342,
/* 5348 */ 1,
/* 5349 */ 5356,
/* 5350 */ 50383,
/* 5351 */ 1,
/* 5352 */ 1,
/* 5353 */ 50059,
/* 5354 */ -5342,
/* 5355 */ 2,
/* 5356 */ 5363,
/* 5357 */ 50310,
/* 5358 */ 1,
/* 5359 */ 1,
/* 5360 */ 50059,
/* 5361 */ -5342,
/* 5362 */ 3,
/* 5363 */ 0,
/* 5364 */ 50466,
/* 5365 */ 1,
/* 5366 */ 1,
/* 5367 */ 50037,
/* 5368 */ 50059,
/* 5369 */ -5342,
/* 5370 */ 4,
/* 5371 */ 5375,
/* 5372 */ 50058,
/* 5373 */ -5371,
/* 5374 */ 1,
/* 5375 */ 0,
/* 5376 */ -5371,
/* 5377 */ 2,
/* 5378 */ 5382,
/* 5379 */ 50058,
/* 5380 */ -5378,
/* 5381 */ 1,
/* 5382 */ 0,
/* 5383 */ -5378,
/* 5384 */ 2,
/* 5385 */ 5389,
/* 5386 */ 50058,
/* 5387 */ -5385,
/* 5388 */ 1,
/* 5389 */ 0,
/* 5390 */ -5385,
/* 5391 */ 2,
/* 5392 */ 5396,
/* 5393 */ 50058,
/* 5394 */ -5392,
/* 5395 */ 1,
/* 5396 */ 0,
/* 5397 */ -5392,
/* 5398 */ 2,
/* 5399 */ 0,
/* 5400 */ 50336,
/* 5401 */ 50257,
/* 5402 */ 1,
/* 5403 */ 50123,
/* 5404 */ 1,
/* 5405 */ 50125,
/* 5406 */ -5399,
/* 5407 */ 1,
/* 5408 */ 5413,
/* 5409 */ 50058,
/* 5410 */ 1,
/* 5411 */ -5408,
/* 5412 */ 1,
/* 5413 */ 0,
/* 5414 */ -5408,
/* 5415 */ 2,
/* 5416 */ 5421,
/* 5417 */ 1,
/* 5418 */ 1,
/* 5419 */ -5416,
/* 5420 */ 1,
/* 5421 */ 0,
/* 5422 */ -5416,
/* 5423 */ 2,
/* 5424 */ 5431,
/* 5425 */ 50450,
/* 5426 */ 50058,
/* 5427 */ 1,
/* 5428 */ 50059,
/* 5429 */ -5424,
/* 5430 */ 1,
/* 5431 */ 5438,
/* 5432 */ 50325,
/* 5433 */ 50058,
/* 5434 */ 1,
/* 5435 */ 50059,
/* 5436 */ -5424,
/* 5437 */ 2,
/* 5438 */ 5445,
/* 5439 */ 50466,
/* 5440 */ 50058,
/* 5441 */ 1,
/* 5442 */ 50059,
/* 5443 */ -5424,
/* 5444 */ 3,
/* 5445 */ 5452,
/* 5446 */ 50307,
/* 5447 */ 50058,
/* 5448 */ 1,
/* 5449 */ 50059,
/* 5450 */ -5424,
/* 5451 */ 4,
/* 5452 */ 5460,
/* 5453 */ 50307,
/* 5454 */ 50058,
/* 5455 */ 1,
/* 5456 */ 50043,
/* 5457 */ 50059,
/* 5458 */ -5424,
/* 5459 */ 5,
/* 5460 */ 5469,
/* 5461 */ 50307,
/* 5462 */ 50058,
/* 5463 */ 1,
/* 5464 */ 1,
/* 5465 */ 1,
/* 5466 */ 50059,
/* 5467 */ -5424,
/* 5468 */ 6,
/* 5469 */ 5476,
/* 5470 */ 50285,
/* 5471 */ 50058,
/* 5472 */ 1,
/* 5473 */ 50059,
/* 5474 */ -5424,
/* 5475 */ 7,
/* 5476 */ 5483,
/* 5477 */ 50406,
/* 5478 */ 50058,
/* 5479 */ 1,
/* 5480 */ 50059,
/* 5481 */ -5424,
/* 5482 */ 8,
/* 5483 */ 5490,
/* 5484 */ 50350,
/* 5485 */ 1,
/* 5486 */ 1,
/* 5487 */ 50059,
/* 5488 */ -5424,
/* 5489 */ 9,
/* 5490 */ 5494,
/* 5491 */ 1,
/* 5492 */ -5424,
/* 5493 */ 10,
/* 5494 */ 5498,
/* 5495 */ 1,
/* 5496 */ -5424,
/* 5497 */ 11,
/* 5498 */ 5502,
/* 5499 */ 1,
/* 5500 */ -5424,
/* 5501 */ 12,
/* 5502 */ 0,
/* 5503 */ 1,
/* 5504 */ -5424,
/* 5505 */ 13,
/* 5506 */ 5510,
/* 5507 */ 50457,
/* 5508 */ -5506,
/* 5509 */ 1,
/* 5510 */ 0,
/* 5511 */ 50045,
/* 5512 */ -5506,
/* 5513 */ 2,
/* 5514 */ 5519,
/* 5515 */ 1,
/* 5516 */ 1,
/* 5517 */ -5514,
/* 5518 */ 1,
/* 5519 */ 0,
/* 5520 */ -5514,
/* 5521 */ 2,
/* 5522 */ 5526,
/* 5523 */ 50058,
/* 5524 */ -5522,
/* 5525 */ 1,
/* 5526 */ 0,
/* 5527 */ -5522,
/* 5528 */ 2,
/* 5529 */ 5535,
/* 5530 */ 1,
/* 5531 */ 1,
/* 5532 */ 1,
/* 5533 */ -5529,
/* 5534 */ 1,
/* 5535 */ 0,
/* 5536 */ -5529,
/* 5537 */ 2,
/* 5538 */ 5542,
/* 5539 */ 50044,
/* 5540 */ -5538,
/* 5541 */ 1,
/* 5542 */ 5546,
/* 5543 */ 50124,
/* 5544 */ -5538,
/* 5545 */ 2,
/* 5546 */ 0,
/* 5547 */ -5538,
/* 5548 */ 3,
/* 5549 */ 0,
/* 5550 */ 1,
/* 5551 */ 1,
/* 5552 */ 50399,
/* 5553 */ 1,
/* 5554 */ 1,
/* 5555 */ 1,
/* 5556 */ -5549,
/* 5557 */ 1,
/* 5558 */ 5562,
/* 5559 */ 50042,
/* 5560 */ -5558,
/* 5561 */ 1,
/* 5562 */ 5566,
/* 5563 */ 50335,
/* 5564 */ -5558,
/* 5565 */ 2,
/* 5566 */ 0,
/* 5567 */ 50124,
/* 5568 */ -5558,
/* 5569 */ 3,
/* 5570 */ 5576,
/* 5571 */ 1,
/* 5572 */ 50037,
/* 5573 */ 1,
/* 5574 */ -5570,
/* 5575 */ 1,
/* 5576 */ 5585,
/* 5577 */ 50060,
/* 5578 */ 50307,
/* 5579 */ 1,
/* 5580 */ 50043,
/* 5581 */ 50062,
/* 5582 */ 1,
/* 5583 */ -5570,
/* 5584 */ 2,
/* 5585 */ 5592,
/* 5586 */ 50091,
/* 5587 */ 1,
/* 5588 */ 50093,
/* 5589 */ 1,
/* 5590 */ -5570,
/* 5591 */ 3,
/* 5592 */ 5599,
/* 5593 */ 50123,
/* 5594 */ 1,
/* 5595 */ 50125,
/* 5596 */ 1,
/* 5597 */ -5570,
/* 5598 */ 4,
/* 5599 */ 5606,
/* 5600 */ 1,
/* 5601 */ 1,
/* 5602 */ 1,
/* 5603 */ 1,
/* 5604 */ -5570,
/* 5605 */ 5,
/* 5606 */ 5611,
/* 5607 */ 1,
/* 5608 */ 1,
/* 5609 */ -5570,
/* 5610 */ 6,
/* 5611 */ 5617,
/* 5612 */ 1,
/* 5613 */ 50043,
/* 5614 */ 1,
/* 5615 */ -5570,
/* 5616 */ 7,
/* 5617 */ 5622,
/* 5618 */ 1,
/* 5619 */ 1,
/* 5620 */ -5570,
/* 5621 */ 8,
/* 5622 */ 0,
/* 5623 */ -5570,
/* 5624 */ 9,
/* 5625 */ 5629,
/* 5626 */ 50045,
/* 5627 */ -5625,
/* 5628 */ 1,
/* 5629 */ 0,
/* 5630 */ 50457,
/* 5631 */ -5625,
/* 5632 */ 2,
/* 5633 */ 5637,
/* 5634 */ 1,
/* 5635 */ -5633,
/* 5636 */ 1,
/* 5637 */ 5641,
/* 5638 */ 1,
/* 5639 */ -5633,
/* 5640 */ 2,
/* 5641 */ 0,
/* 5642 */ 50289,
/* 5643 */ 1,
/* 5644 */ -5633,
/* 5645 */ 3,
/* 5646 */ 5650,
/* 5647 */ 1,
/* 5648 */ -5646,
/* 5649 */ 1,
/* 5650 */ 5654,
/* 5651 */ 1,
/* 5652 */ -5646,
/* 5653 */ 2,
/* 5654 */ 5659,
/* 5655 */ 50289,
/* 5656 */ 1,
/* 5657 */ -5646,
/* 5658 */ 3,
/* 5659 */ 0,
/* 5660 */ -5646,
/* 5661 */ 4,
/* 5662 */ 5669,
/* 5663 */ 50091,
/* 5664 */ 1,
/* 5665 */ 1,
/* 5666 */ 50093,
/* 5667 */ -5662,
/* 5668 */ 1,
/* 5669 */ 0,
/* 5670 */ -5662,
/* 5671 */ 2,
/* 5672 */ 5678,
/* 5673 */ 1,
/* 5674 */ 1,
/* 5675 */ 1,
/* 5676 */ -5672,
/* 5677 */ 1,
/* 5678 */ 0,
/* 5679 */ -5672,
/* 5680 */ 2,
/* 5681 */ 5685,
/* 5682 */ 50124,
/* 5683 */ -5681,
/* 5684 */ 1,
/* 5685 */ 0,
/* 5686 */ 50044,
/* 5687 */ -5681,
/* 5688 */ 2,
/* 5689 */ 0,
/* 5690 */ 50299,
/* 5691 */ 50257,
/* 5692 */ 50058,
/* 5693 */ 1,
/* 5694 */ 50123,
/* 5695 */ 1,
/* 5696 */ 50125,
/* 5697 */ -5689,
/* 5698 */ 1,
/* 5699 */ 5704,
/* 5700 */ 1,
/* 5701 */ 1,
/* 5702 */ -5699,
/* 5703 */ 1,
/* 5704 */ 0,
/* 5705 */ -5699,
/* 5706 */ 2,
/* 5707 */ 5714,
/* 5708 */ 50436,
/* 5709 */ 1,
/* 5710 */ 1,
/* 5711 */ 50059,
/* 5712 */ -5707,
/* 5713 */ 1,
/* 5714 */ 5721,
/* 5715 */ 50350,
/* 5716 */ 1,
/* 5717 */ 1,
/* 5718 */ 50059,
/* 5719 */ -5707,
/* 5720 */ 2,
/* 5721 */ 5725,
/* 5722 */ 1,
/* 5723 */ -5707,
/* 5724 */ 3,
/* 5725 */ 5729,
/* 5726 */ 1,
/* 5727 */ -5707,
/* 5728 */ 4,
/* 5729 */ 0,
/* 5730 */ 1,
/* 5731 */ -5707,
/* 5732 */ 5,
/* 5733 */ 5737,
/* 5734 */ 50058,
/* 5735 */ -5733,
/* 5736 */ 1,
/* 5737 */ 0,
/* 5738 */ -5733,
/* 5739 */ 2,
/* 5740 */ 5744,
/* 5741 */ 50058,
/* 5742 */ -5740,
/* 5743 */ 1,
/* 5744 */ 0,
/* 5745 */ -5740,
/* 5746 */ 2,
/* 5747 */ 5753,
/* 5748 */ 1,
/* 5749 */ 1,
/* 5750 */ 1,
/* 5751 */ -5747,
/* 5752 */ 1,
/* 5753 */ 0,
/* 5754 */ -5747,
/* 5755 */ 2,
/* 5756 */ 5760,
/* 5757 */ 50124,
/* 5758 */ -5756,
/* 5759 */ 1,
/* 5760 */ 5764,
/* 5761 */ 50044,
/* 5762 */ -5756,
/* 5763 */ 2,
/* 5764 */ 0,
/* 5765 */ -5756,
/* 5766 */ 3,
/* 5767 */ 0,
/* 5768 */ 50451,
/* 5769 */ 50257,
/* 5770 */ 1,
/* 5771 */ 50059,
/* 5772 */ -5767,
/* 5773 */ 1,
/* 5774 */ 5779,
/* 5775 */ 50257,
/* 5776 */ 1,
/* 5777 */ -5774,
/* 5778 */ 1,
/* 5779 */ 5784,
/* 5780 */ 1,
/* 5781 */ 1,
/* 5782 */ -5774,
/* 5783 */ 2,
/* 5784 */ 5789,
/* 5785 */ 50040,
/* 5786 */ 1,
/* 5787 */ -5774,
/* 5788 */ 3,
/* 5789 */ 5794,
/* 5790 */ 50041,
/* 5791 */ 1,
/* 5792 */ -5774,
/* 5793 */ 4,
/* 5794 */ 5799,
/* 5795 */ 50124,
/* 5796 */ 1,
/* 5797 */ -5774,
/* 5798 */ 5,
/* 5799 */ 5804,
/* 5800 */ 50059,
/* 5801 */ 1,
/* 5802 */ -5774,
/* 5803 */ 6,
/* 5804 */ 5812,
/* 5805 */ 50091,
/* 5806 */ 1,
/* 5807 */ 50093,
/* 5808 */ 50058,
/* 5809 */ 1,
/* 5810 */ -5774,
/* 5811 */ 7,
/* 5812 */ 5819,
/* 5813 */ 50091,
/* 5814 */ 1,
/* 5815 */ 50093,
/* 5816 */ 1,
/* 5817 */ -5774,
/* 5818 */ 8,
/* 5819 */ 0,
/* 5820 */ -5774,
/* 5821 */ 9,
/* 5822 */ 0,
/* 5823 */ 50261,
/* 5824 */ -5822,
/* 5825 */ 1,
/* 5826 */ 5830,
/* 5827 */ 50264,
/* 5828 */ -5826,
/* 5829 */ 1,
/* 5830 */ 5834,
/* 5831 */ 50274,
/* 5832 */ -5826,
/* 5833 */ 2,
/* 5834 */ 5838,
/* 5835 */ 50268,
/* 5836 */ -5826,
/* 5837 */ 3,
/* 5838 */ 5842,
/* 5839 */ 50269,
/* 5840 */ -5826,
/* 5841 */ 4,
/* 5842 */ 5846,
/* 5843 */ 50265,
/* 5844 */ -5826,
/* 5845 */ 5,
/* 5846 */ 5850,
/* 5847 */ 50266,
/* 5848 */ -5826,
/* 5849 */ 6,
/* 5850 */ 5854,
/* 5851 */ 50267,
/* 5852 */ -5826,
/* 5853 */ 7,
/* 5854 */ 5858,
/* 5855 */ 50263,
/* 5856 */ -5826,
/* 5857 */ 8,
/* 5858 */ 5862,
/* 5859 */ 50271,
/* 5860 */ -5826,
/* 5861 */ 9,
/* 5862 */ 5866,
/* 5863 */ 50272,
/* 5864 */ -5826,
/* 5865 */ 10,
/* 5866 */ 5870,
/* 5867 */ 50273,
/* 5868 */ -5826,
/* 5869 */ 11,
/* 5870 */ 0,
/* 5871 */ 50474,
/* 5872 */ -5826,
/* 5873 */ 12,
/* 5874 */ 0,
/* 5875 */ 50447,
/* 5876 */ 1,
/* 5877 */ 1,
/* 5878 */ 50262,
/* 5879 */ 50258,
/* 5880 */ 50040,
/* 5881 */ 1,
/* 5882 */ 1,
/* 5883 */ 50041,
/* 5884 */ 50059,
/* 5885 */ -5874,
/* 5886 */ 1,
/* 5887 */ 5895,
/* 5888 */ 1,
/* 5889 */ 1,
/* 5890 */ 1,
/* 5891 */ 1,
/* 5892 */ 1,
/* 5893 */ -5887,
/* 5894 */ 1,
/* 5895 */ 0,
/* 5896 */ -5887,
/* 5897 */ 2,
/* 5898 */ 5903,
/* 5899 */ 50058,
/* 5900 */ 1,
/* 5901 */ -5898,
/* 5902 */ 1,
/* 5903 */ 0,
/* 5904 */ -5898,
/* 5905 */ 2,
/* 5906 */ 5910,
/* 5907 */ 50258,
/* 5908 */ -5906,
/* 5909 */ 1,
/* 5910 */ 0,
/* 5911 */ -5906,
/* 5912 */ 2,
/* 5913 */ 5918,
/* 5914 */ 50061,
/* 5915 */ 1,
/* 5916 */ -5913,
/* 5917 */ 1,
/* 5918 */ 0,
/* 5919 */ -5913,
/* 5920 */ 2,
/* 5921 */ 5930,
/* 5922 */ 50044,
/* 5923 */ 1,
/* 5924 */ 1,
/* 5925 */ 1,
/* 5926 */ 1,
/* 5927 */ 1,
/* 5928 */ -5921,
/* 5929 */ 1,
/* 5930 */ 0,
/* 5931 */ -5921,
/* 5932 */ 2,
/* 5933 */ 5938,
/* 5934 */ 50058,
/* 5935 */ 1,
/* 5936 */ -5933,
/* 5937 */ 1,
/* 5938 */ 0,
/* 5939 */ -5933,
/* 5940 */ 2,
/* 5941 */ 5945,
/* 5942 */ 50258,
/* 5943 */ -5941,
/* 5944 */ 1,
/* 5945 */ 0,
/* 5946 */ -5941,
/* 5947 */ 2,
/* 5948 */ 5953,
/* 5949 */ 50061,
/* 5950 */ 1,
/* 5951 */ -5948,
/* 5952 */ 1,
/* 5953 */ 0,
/* 5954 */ -5948,
/* 5955 */ 2,
/* 5956 */ 5961,
/* 5957 */ 50044,
/* 5958 */ 50495,
/* 5959 */ -5956,
/* 5960 */ 1,
/* 5961 */ 0,
/* 5962 */ -5956,
/* 5963 */ 2,
/* 5964 */ 0,
/* 5965 */ 50447,
/* 5966 */ 1,
/* 5967 */ 50258,
/* 5968 */ 50040,
/* 5969 */ 1,
/* 5970 */ 1,
/* 5971 */ 50041,
/* 5972 */ 50059,
/* 5973 */ -5964,
/* 5974 */ 1,
/* 5975 */ 5983,
/* 5976 */ 1,
/* 5977 */ 1,
/* 5978 */ 1,
/* 5979 */ 1,
/* 5980 */ 1,
/* 5981 */ -5975,
/* 5982 */ 1,
/* 5983 */ 0,
/* 5984 */ -5975,
/* 5985 */ 2,
/* 5986 */ 5991,
/* 5987 */ 50058,
/* 5988 */ 1,
/* 5989 */ -5986,
/* 5990 */ 1,
/* 5991 */ 0,
/* 5992 */ -5986,
/* 5993 */ 2,
/* 5994 */ 5998,
/* 5995 */ 50258,
/* 5996 */ -5994,
/* 5997 */ 1,
/* 5998 */ 0,
/* 5999 */ -5994,
/* 6000 */ 2,
/* 6001 */ 6006,
/* 6002 */ 50061,
/* 6003 */ 1,
/* 6004 */ -6001,
/* 6005 */ 1,
/* 6006 */ 0,
/* 6007 */ -6001,
/* 6008 */ 2,
/* 6009 */ 6018,
/* 6010 */ 50044,
/* 6011 */ 1,
/* 6012 */ 1,
/* 6013 */ 1,
/* 6014 */ 1,
/* 6015 */ 1,
/* 6016 */ -6009,
/* 6017 */ 1,
/* 6018 */ 0,
/* 6019 */ -6009,
/* 6020 */ 2,
/* 6021 */ 6026,
/* 6022 */ 50058,
/* 6023 */ 1,
/* 6024 */ -6021,
/* 6025 */ 1,
/* 6026 */ 0,
/* 6027 */ -6021,
/* 6028 */ 2,
/* 6029 */ 6033,
/* 6030 */ 50258,
/* 6031 */ -6029,
/* 6032 */ 1,
/* 6033 */ 0,
/* 6034 */ -6029,
/* 6035 */ 2,
/* 6036 */ 6041,
/* 6037 */ 50061,
/* 6038 */ 1,
/* 6039 */ -6036,
/* 6040 */ 1,
/* 6041 */ 0,
/* 6042 */ -6036,
/* 6043 */ 2,
/* 6044 */ 6049,
/* 6045 */ 50044,
/* 6046 */ 50495,
/* 6047 */ -6044,
/* 6048 */ 1,
/* 6049 */ 0,
/* 6050 */ -6044,
/* 6051 */ 2,
/* 6052 */ 0,
/* 6053 */ 50447,
/* 6054 */ 1,
/* 6055 */ 1,
/* 6056 */ 50262,
/* 6057 */ 50258,
/* 6058 */ 50059,
/* 6059 */ -6052,
/* 6060 */ 1,
/* 6061 */ 0,
/* 6062 */ 50447,
/* 6063 */ 1,
/* 6064 */ 50258,
/* 6065 */ 50481,
/* 6066 */ 1,
/* 6067 */ 50059,
/* 6068 */ -6061,
/* 6069 */ 1,
/* 6070 */ 0,
/* 6071 */ 1,
/* 6072 */ 50258,
/* 6073 */ 1,
/* 6074 */ 50059,
/* 6075 */ -6070,
/* 6076 */ 1,
/* 6077 */ 6083,
/* 6078 */ 50044,
/* 6079 */ 50258,
/* 6080 */ 1,
/* 6081 */ -6077,
/* 6082 */ 1,
/* 6083 */ 0,
/* 6084 */ -6077,
/* 6085 */ 2,
/* 6086 */ 0,
/* 6087 */ 1,
/* 6088 */ 50258,
/* 6089 */ 1,
/* 6090 */ 50059,
/* 6091 */ -6086,
/* 6092 */ 1,
/* 6093 */ 6099,
/* 6094 */ 50044,
/* 6095 */ 50258,
/* 6096 */ 1,
/* 6097 */ -6093,
/* 6098 */ 1,
/* 6099 */ 0,
/* 6100 */ -6093,
/* 6101 */ 2,
/* 6102 */ 0,
/* 6103 */ 1,
/* 6104 */ 50258,
/* 6105 */ 1,
/* 6106 */ 50059,
/* 6107 */ -6102,
/* 6108 */ 1,
/* 6109 */ 6115,
/* 6110 */ 50044,
/* 6111 */ 50258,
/* 6112 */ 1,
/* 6113 */ -6109,
/* 6114 */ 1,
/* 6115 */ 0,
/* 6116 */ -6109,
/* 6117 */ 2,
/* 6118 */ 6123,
/* 6119 */ 1,
/* 6120 */ 50059,
/* 6121 */ -6118,
/* 6122 */ 1,
/* 6123 */ 6133,
/* 6124 */ 50367,
/* 6125 */ 50040,
/* 6126 */ 1,
/* 6127 */ 50041,
/* 6128 */ 1,
/* 6129 */ 50335,
/* 6130 */ 1,
/* 6131 */ -6118,
/* 6132 */ 2,
/* 6133 */ 6141,
/* 6134 */ 50367,
/* 6135 */ 50040,
/* 6136 */ 1,
/* 6137 */ 50041,
/* 6138 */ 1,
/* 6139 */ -6118,
/* 6140 */ 3,
/* 6141 */ 6153,
/* 6142 */ 50353,
/* 6143 */ 50040,
/* 6144 */ 1,
/* 6145 */ 50059,
/* 6146 */ 1,
/* 6147 */ 50059,
/* 6148 */ 1,
/* 6149 */ 50041,
/* 6150 */ 1,
/* 6151 */ -6118,
/* 6152 */ 4,
/* 6153 */ 6164,
/* 6154 */ 50446,
/* 6155 */ 50040,
/* 6156 */ 1,
/* 6157 */ 50041,
/* 6158 */ 50123,
/* 6159 */ 1,
/* 6160 */ 1,
/* 6161 */ 50125,
/* 6162 */ -6118,
/* 6163 */ 5,
/* 6164 */ 6169,
/* 6165 */ 50302,
/* 6166 */ 50059,
/* 6167 */ -6118,
/* 6168 */ 6,
/* 6169 */ 6174,
/* 6170 */ 50314,
/* 6171 */ 50059,
/* 6172 */ -6118,
/* 6173 */ 7,
/* 6174 */ 6184,
/* 6175 */ 50331,
/* 6176 */ 1,
/* 6177 */ 50469,
/* 6178 */ 50040,
/* 6179 */ 1,
/* 6180 */ 50041,
/* 6181 */ 50059,
/* 6182 */ -6118,
/* 6183 */ 8,
/* 6184 */ 6192,
/* 6185 */ 50469,
/* 6186 */ 50040,
/* 6187 */ 1,
/* 6188 */ 50041,
/* 6189 */ 1,
/* 6190 */ -6118,
/* 6191 */ 9,
/* 6192 */ 6196,
/* 6193 */ 1,
/* 6194 */ -6118,
/* 6195 */ 10,
/* 6196 */ 6202,
/* 6197 */ 50425,
/* 6198 */ 1,
/* 6199 */ 50059,
/* 6200 */ -6118,
/* 6201 */ 11,
/* 6202 */ 0,
/* 6203 */ 50425,
/* 6204 */ 50059,
/* 6205 */ -6118,
/* 6206 */ 12,
/* 6207 */ 6215,
/* 6208 */ 50305,
/* 6209 */ 1,
/* 6210 */ 50058,
/* 6211 */ 1,
/* 6212 */ 1,
/* 6213 */ -6207,
/* 6214 */ 1,
/* 6215 */ 0,
/* 6216 */ -6207,
/* 6217 */ 2,
/* 6218 */ 6222,
/* 6219 */ 1,
/* 6220 */ -6218,
/* 6221 */ 1,
/* 6222 */ 0,
/* 6223 */ 1,
/* 6224 */ -6218,
/* 6225 */ 2,
/* 6226 */ 6231,
/* 6227 */ 1,
/* 6228 */ 1,
/* 6229 */ -6226,
/* 6230 */ 1,
/* 6231 */ 0,
/* 6232 */ -6226,
/* 6233 */ 2,
/* 6234 */ 6240,
/* 6235 */ 50324,
/* 6236 */ 50058,
/* 6237 */ 1,
/* 6238 */ -6234,
/* 6239 */ 1,
/* 6240 */ 0,
/* 6241 */ -6234,
/* 6242 */ 2,
/* 6243 */ 6248,
/* 6244 */ 1,
/* 6245 */ 1,
/* 6246 */ -6243,
/* 6247 */ 1,
/* 6248 */ 0,
/* 6249 */ -6243,
/* 6250 */ 2,
/* 6251 */ 0,
/* 6252 */ 50123,
/* 6253 */ 1,
/* 6254 */ 50125,
/* 6255 */ -6251,
/* 6256 */ 1,
/* 6257 */ 6262,
/* 6258 */ 1,
/* 6259 */ 1,
/* 6260 */ -6257,
/* 6261 */ 1,
/* 6262 */ 0,
/* 6263 */ -6257,
/* 6264 */ 2,
/* 6265 */ 0,
/* 6266 */ 50123,
/* 6267 */ 1,
/* 6268 */ 1,
/* 6269 */ 50125,
/* 6270 */ -6265,
/* 6271 */ 1,
/* 6272 */ 6277,
/* 6273 */ 1,
/* 6274 */ 1,
/* 6275 */ -6272,
/* 6276 */ 1,
/* 6277 */ 6283,
/* 6278 */ 50441,
/* 6279 */ 1,
/* 6280 */ 1,
/* 6281 */ -6272,
/* 6282 */ 2,
/* 6283 */ 0,
/* 6284 */ -6272,
/* 6285 */ 3,
/* 6286 */ 6291,
/* 6287 */ 1,
/* 6288 */ 1,
/* 6289 */ -6286,
/* 6290 */ 1,
/* 6291 */ 0,
/* 6292 */ -6286,
/* 6293 */ 2,
/* 6294 */ 6298,
/* 6295 */ 50256,
/* 6296 */ -6294,
/* 6297 */ 1,
/* 6298 */ 6302,
/* 6299 */ 50259,
/* 6300 */ -6294,
/* 6301 */ 2,
/* 6302 */ 6306,
/* 6303 */ 50257,
/* 6304 */ -6294,
/* 6305 */ 3,
/* 6306 */ 6310,
/* 6307 */ 50458,
/* 6308 */ -6294,
/* 6309 */ 4,
/* 6310 */ 6314,
/* 6311 */ 50345,
/* 6312 */ -6294,
/* 6313 */ 5,
/* 6314 */ 6318,
/* 6315 */ 50395,
/* 6316 */ -6294,
/* 6317 */ 6,
/* 6318 */ 6322,
/* 6319 */ 1,
/* 6320 */ -6294,
/* 6321 */ 7,
/* 6322 */ 6328,
/* 6323 */ 50040,
/* 6324 */ 1,
/* 6325 */ 50041,
/* 6326 */ -6294,
/* 6327 */ 8,
/* 6328 */ 6332,
/* 6329 */ 50258,
/* 6330 */ -6294,
/* 6331 */ 9,
/* 6332 */ 6338,
/* 6333 */ 1,
/* 6334 */ 1,
/* 6335 */ 50258,
/* 6336 */ -6294,
/* 6337 */ 10,
/* 6338 */ 6347,
/* 6339 */ 1,
/* 6340 */ 1,
/* 6341 */ 50258,
/* 6342 */ 50040,
/* 6343 */ 1,
/* 6344 */ 50041,
/* 6345 */ -6294,
/* 6346 */ 11,
/* 6347 */ 6354,
/* 6348 */ 50258,
/* 6349 */ 50040,
/* 6350 */ 1,
/* 6351 */ 50041,
/* 6352 */ -6294,
/* 6353 */ 12,
/* 6354 */ 0,
/* 6355 */ 1,
/* 6356 */ 50064,
/* 6357 */ 50258,
/* 6358 */ -6294,
/* 6359 */ 13,
/* 6360 */ 6364,
/* 6361 */ 50046,
/* 6362 */ -6360,
/* 6363 */ 1,
/* 6364 */ 0,
/* 6365 */ 50496,
/* 6366 */ -6360,
/* 6367 */ 2,
/* 6368 */ 6372,
/* 6369 */ 50046,
/* 6370 */ -6368,
/* 6371 */ 1,
/* 6372 */ 0,
/* 6373 */ 50496,
/* 6374 */ -6368,
/* 6375 */ 2,
/* 6376 */ 6380,
/* 6377 */ 50258,
/* 6378 */ -6376,
/* 6379 */ 1,
/* 6380 */ 6386,
/* 6381 */ 1,
/* 6382 */ 50064,
/* 6383 */ 50258,
/* 6384 */ -6376,
/* 6385 */ 2,
/* 6386 */ 0,
/* 6387 */ 1,
/* 6388 */ 1,
/* 6389 */ 50258,
/* 6390 */ -6376,
/* 6391 */ 3,
/* 6392 */ 6396,
/* 6393 */ 50046,
/* 6394 */ -6392,
/* 6395 */ 1,
/* 6396 */ 0,
/* 6397 */ 50496,
/* 6398 */ -6392,
/* 6399 */ 2,
/* 6400 */ 6405,
/* 6401 */ 1,
/* 6402 */ 50475,
/* 6403 */ -6400,
/* 6404 */ 1,
/* 6405 */ 6410,
/* 6406 */ 1,
/* 6407 */ 50476,
/* 6408 */ -6400,
/* 6409 */ 2,
/* 6410 */ 6419,
/* 6411 */ 50387,
/* 6412 */ 50040,
/* 6413 */ 1,
/* 6414 */ 50044,
/* 6415 */ 1,
/* 6416 */ 50041,
/* 6417 */ -6400,
/* 6418 */ 3,
/* 6419 */ 6428,
/* 6420 */ 50384,
/* 6421 */ 50040,
/* 6422 */ 1,
/* 6423 */ 50044,
/* 6424 */ 1,
/* 6425 */ 50041,
/* 6426 */ -6400,
/* 6427 */ 4,
/* 6428 */ 0,
/* 6429 */ 1,
/* 6430 */ -6400,
/* 6431 */ 5,
/* 6432 */ 6437,
/* 6433 */ 50475,
/* 6434 */ 1,
/* 6435 */ -6432,
/* 6436 */ 1,
/* 6437 */ 6442,
/* 6438 */ 50476,
/* 6439 */ 1,
/* 6440 */ -6432,
/* 6441 */ 2,
/* 6442 */ 6447,
/* 6443 */ 50043,
/* 6444 */ 1,
/* 6445 */ -6432,
/* 6446 */ 3,
/* 6447 */ 6452,
/* 6448 */ 50045,
/* 6449 */ 1,
/* 6450 */ -6432,
/* 6451 */ 4,
/* 6452 */ 6457,
/* 6453 */ 50033,
/* 6454 */ 1,
/* 6455 */ -6432,
/* 6456 */ 5,
/* 6457 */ 6462,
/* 6458 */ 50126,
/* 6459 */ 1,
/* 6460 */ -6432,
/* 6461 */ 6,
/* 6462 */ 6468,
/* 6463 */ 50058,
/* 6464 */ 1,
/* 6465 */ 50058,
/* 6466 */ -6432,
/* 6467 */ 7,
/* 6468 */ 0,
/* 6469 */ 1,
/* 6470 */ -6432,
/* 6471 */ 8,
/* 6472 */ 6478,
/* 6473 */ 1,
/* 6474 */ 50100,
/* 6475 */ 1,
/* 6476 */ -6472,
/* 6477 */ 1,
/* 6478 */ 0,
/* 6479 */ 1,
/* 6480 */ -6472,
/* 6481 */ 2,
/* 6482 */ 6489,
/* 6483 */ 50040,
/* 6484 */ 1,
/* 6485 */ 50041,
/* 6486 */ 1,
/* 6487 */ -6482,
/* 6488 */ 1,
/* 6489 */ 0,
/* 6490 */ 1,
/* 6491 */ -6482,
/* 6492 */ 2,
/* 6493 */ 6499,
/* 6494 */ 1,
/* 6495 */ 50042,
/* 6496 */ 1,
/* 6497 */ -6493,
/* 6498 */ 1,
/* 6499 */ 6505,
/* 6500 */ 1,
/* 6501 */ 50047,
/* 6502 */ 1,
/* 6503 */ -6493,
/* 6504 */ 2,
/* 6505 */ 6511,
/* 6506 */ 1,
/* 6507 */ 50037,
/* 6508 */ 1,
/* 6509 */ -6493,
/* 6510 */ 3,
/* 6511 */ 0,
/* 6512 */ 1,
/* 6513 */ -6493,
/* 6514 */ 4,
/* 6515 */ 6521,
/* 6516 */ 1,
/* 6517 */ 50043,
/* 6518 */ 1,
/* 6519 */ -6515,
/* 6520 */ 1,
/* 6521 */ 6527,
/* 6522 */ 1,
/* 6523 */ 50045,
/* 6524 */ 1,
/* 6525 */ -6515,
/* 6526 */ 2,
/* 6527 */ 0,
/* 6528 */ 1,
/* 6529 */ -6515,
/* 6530 */ 3,
/* 6531 */ 6537,
/* 6532 */ 1,
/* 6533 */ 50477,
/* 6534 */ 1,
/* 6535 */ -6531,
/* 6536 */ 1,
/* 6537 */ 6543,
/* 6538 */ 1,
/* 6539 */ 50478,
/* 6540 */ 1,
/* 6541 */ -6531,
/* 6542 */ 2,
/* 6543 */ 0,
/* 6544 */ 1,
/* 6545 */ -6531,
/* 6546 */ 3,
/* 6547 */ 6553,
/* 6548 */ 1,
/* 6549 */ 50060,
/* 6550 */ 1,
/* 6551 */ -6547,
/* 6552 */ 1,
/* 6553 */ 6559,
/* 6554 */ 1,
/* 6555 */ 50062,
/* 6556 */ 1,
/* 6557 */ -6547,
/* 6558 */ 2,
/* 6559 */ 6565,
/* 6560 */ 1,
/* 6561 */ 50479,
/* 6562 */ 1,
/* 6563 */ -6547,
/* 6564 */ 3,
/* 6565 */ 6571,
/* 6566 */ 1,
/* 6567 */ 50480,
/* 6568 */ 1,
/* 6569 */ -6547,
/* 6570 */ 4,
/* 6571 */ 6577,
/* 6572 */ 1,
/* 6573 */ 50481,
/* 6574 */ 1,
/* 6575 */ -6547,
/* 6576 */ 5,
/* 6577 */ 6583,
/* 6578 */ 1,
/* 6579 */ 50482,
/* 6580 */ 1,
/* 6581 */ -6547,
/* 6582 */ 6,
/* 6583 */ 0,
/* 6584 */ 1,
/* 6585 */ -6547,
/* 6586 */ 7,
/* 6587 */ 6593,
/* 6588 */ 1,
/* 6589 */ 50038,
/* 6590 */ 1,
/* 6591 */ -6587,
/* 6592 */ 1,
/* 6593 */ 0,
/* 6594 */ 1,
/* 6595 */ -6587,
/* 6596 */ 2,
/* 6597 */ 6603,
/* 6598 */ 1,
/* 6599 */ 50094,
/* 6600 */ 1,
/* 6601 */ -6597,
/* 6602 */ 1,
/* 6603 */ 0,
/* 6604 */ 1,
/* 6605 */ -6597,
/* 6606 */ 2,
/* 6607 */ 6613,
/* 6608 */ 1,
/* 6609 */ 50124,
/* 6610 */ 1,
/* 6611 */ -6607,
/* 6612 */ 1,
/* 6613 */ 0,
/* 6614 */ 1,
/* 6615 */ -6607,
/* 6616 */ 2,
/* 6617 */ 6623,
/* 6618 */ 1,
/* 6619 */ 50483,
/* 6620 */ 1,
/* 6621 */ -6617,
/* 6622 */ 1,
/* 6623 */ 0,
/* 6624 */ 1,
/* 6625 */ -6617,
/* 6626 */ 2,
/* 6627 */ 6633,
/* 6628 */ 1,
/* 6629 */ 50484,
/* 6630 */ 1,
/* 6631 */ -6627,
/* 6632 */ 1,
/* 6633 */ 0,
/* 6634 */ 1,
/* 6635 */ -6627,
/* 6636 */ 2,
/* 6637 */ 6645,
/* 6638 */ 1,
/* 6639 */ 50063,
/* 6640 */ 1,
/* 6641 */ 50058,
/* 6642 */ 1,
/* 6643 */ -6637,
/* 6644 */ 1,
/* 6645 */ 0,
/* 6646 */ 1,
/* 6647 */ -6637,
/* 6648 */ 2,
/* 6649 */ 6655,
/* 6650 */ 1,
/* 6651 */ 50061,
/* 6652 */ 1,
/* 6653 */ -6649,
/* 6654 */ 1,
/* 6655 */ 6661,
/* 6656 */ 1,
/* 6657 */ 50486,
/* 6658 */ 1,
/* 6659 */ -6649,
/* 6660 */ 2,
/* 6661 */ 6667,
/* 6662 */ 1,
/* 6663 */ 50494,
/* 6664 */ 1,
/* 6665 */ -6649,
/* 6666 */ 3,
/* 6667 */ 6673,
/* 6668 */ 1,
/* 6669 */ 50485,
/* 6670 */ 1,
/* 6671 */ -6649,
/* 6672 */ 4,
/* 6673 */ 6679,
/* 6674 */ 1,
/* 6675 */ 50487,
/* 6676 */ 1,
/* 6677 */ -6649,
/* 6678 */ 5,
/* 6679 */ 6685,
/* 6680 */ 1,
/* 6681 */ 50488,
/* 6682 */ 1,
/* 6683 */ -6649,
/* 6684 */ 6,
/* 6685 */ 6691,
/* 6686 */ 1,
/* 6687 */ 50489,
/* 6688 */ 1,
/* 6689 */ -6649,
/* 6690 */ 7,
/* 6691 */ 6697,
/* 6692 */ 1,
/* 6693 */ 50490,
/* 6694 */ 1,
/* 6695 */ -6649,
/* 6696 */ 8,
/* 6697 */ 6703,
/* 6698 */ 1,
/* 6699 */ 50492,
/* 6700 */ 1,
/* 6701 */ -6649,
/* 6702 */ 9,
/* 6703 */ 6709,
/* 6704 */ 1,
/* 6705 */ 50493,
/* 6706 */ 1,
/* 6707 */ -6649,
/* 6708 */ 10,
/* 6709 */ 6715,
/* 6710 */ 1,
/* 6711 */ 50491,
/* 6712 */ 1,
/* 6713 */ -6649,
/* 6714 */ 11,
/* 6715 */ 0,
/* 6716 */ 1,
/* 6717 */ -6649,
/* 6718 */ 12,
/* 6719 */ 6725,
/* 6720 */ 1,
/* 6721 */ 50044,
/* 6722 */ 1,
/* 6723 */ -6719,
/* 6724 */ 1,
/* 6725 */ 0,
/* 6726 */ 1,
/* 6727 */ -6719,
/* 6728 */ 2,
/* 6729 */ 0,
/* 6730 */ 1,
/* 6731 */ -6729,
/* 6732 */ 1,
/* 6733 */ 6738,
/* 6734 */ 1,
/* 6735 */ 1,
/* 6736 */ -6733,
/* 6737 */ 1,
/* 6738 */ 0,
/* 6739 */ -6733,
/* 6740 */ 2,
/* 6741 */ 6747,
/* 6742 */ 50044,
/* 6743 */ 1,
/* 6744 */ 1,
/* 6745 */ -6741,
/* 6746 */ 1,
/* 6747 */ 0,
/* 6748 */ -6741,
/* 6749 */ 2,
0
};
extern int yycoordinate[];
int yycoordinate[] = {
0,
/* 1 */ 9999,
/* 2 */ 189005,
/* 3 */ 9999,
/* 4 */ 9999,
/* 5 */ 189005,
/* 6 */ 9999,
/* 7 */ 191003,
/* 8 */ 9999,
/* 9 */ 190003,
/* 10 */ 9999,
/* 11 */ 191004,
/* 12 */ 191011,
/* 13 */ 9999,
/* 14 */ 191011,
/* 15 */ 9999,
/* 16 */ 9999,
/* 17 */ 191011,
/* 18 */ 9999,
/* 19 */ 9999,
/* 20 */ 9999,
/* 21 */ 9999,
/* 22 */ 9999,
/* 23 */ 194010,
/* 24 */ 9999,
/* 25 */ 9999,
/* 26 */ 195010,
/* 27 */ 9999,
/* 28 */ 9999,
/* 29 */ 195008,
/* 30 */ 9999,
/* 31 */ 9999,
/* 32 */ 9999,
/* 33 */ 9999,
/* 34 */ 9999,
/* 35 */ 196013,
/* 36 */ 9999,
/* 37 */ 197005,
/* 38 */ 9999,
/* 39 */ 197015,
/* 40 */ 9999,
/* 41 */ 198005,
/* 42 */ 9999,
/* 43 */ 198012,
/* 44 */ 9999,
/* 45 */ 199005,
/* 46 */ 9999,
/* 47 */ 199015,
/* 48 */ 9999,
/* 49 */ 200005,
/* 50 */ 9999,
/* 51 */ 200016,
/* 52 */ 9999,
/* 53 */ 201005,
/* 54 */ 9999,
/* 55 */ 201015,
/* 56 */ 9999,
/* 57 */ 202005,
/* 58 */ 9999,
/* 59 */ 202013,
/* 60 */ 9999,
/* 61 */ 203005,
/* 62 */ 9999,
/* 63 */ 203014,
/* 64 */ 9999,
/* 65 */ 204005,
/* 66 */ 9999,
/* 67 */ 204012,
/* 68 */ 9999,
/* 69 */ 205005,
/* 70 */ 9999,
/* 71 */ 205013,
/* 72 */ 9999,
/* 73 */ 206005,
/* 74 */ 9999,
/* 75 */ 206011,
/* 76 */ 9999,
/* 77 */ 207005,
/* 78 */ 9999,
/* 79 */ 207014,
/* 80 */ 9999,
/* 81 */ 208005,
/* 82 */ 9999,
/* 83 */ 208014,
/* 84 */ 9999,
/* 85 */ 209005,
/* 86 */ 9999,
/* 87 */ 209015,
/* 88 */ 9999,
/* 89 */ 210005,
/* 90 */ 9999,
/* 91 */ 210016,
/* 92 */ 9999,
/* 93 */ 211005,
/* 94 */ 9999,
/* 95 */ 211014,
/* 96 */ 9999,
/* 97 */ 212005,
/* 98 */ 9999,
/* 99 */ 212017,
/* 100 */ 9999,
/* 101 */ 213005,
/* 102 */ 9999,
/* 103 */ 213017,
/* 104 */ 9999,
/* 105 */ 214005,
/* 106 */ 9999,
/* 107 */ 214013,
/* 108 */ 9999,
/* 109 */ 215005,
/* 110 */ 9999,
/* 111 */ 215015,
/* 112 */ 9999,
/* 113 */ 216005,
/* 114 */ 9999,
/* 115 */ 216016,
/* 116 */ 9999,
/* 117 */ 217005,
/* 118 */ 9999,
/* 119 */ 217017,
/* 120 */ 9999,
/* 121 */ 218005,
/* 122 */ 9999,
/* 123 */ 218017,
/* 124 */ 9999,
/* 125 */ 219005,
/* 126 */ 9999,
/* 127 */ 219017,
/* 128 */ 9999,
/* 129 */ 9999,
/* 130 */ 220011,
/* 131 */ 220017,
/* 132 */ 9999,
/* 133 */ 220009,
/* 134 */ 9999,
/* 135 */ 9999,
/* 136 */ 9999,
/* 137 */ 9999,
/* 138 */ 221027,
/* 139 */ 9999,
/* 140 */ 221058,
/* 141 */ 9999,
/* 142 */ 9999,
/* 143 */ 221008,
/* 144 */ 9999,
/* 145 */ 9999,
/* 146 */ 9999,
/* 147 */ 220012,
/* 148 */ 9999,
/* 149 */ 9999,
/* 150 */ 9999,
/* 151 */ 9999,
/* 152 */ 221036,
/* 153 */ 9999,
/* 154 */ 9999,
/* 155 */ 221036,
/* 156 */ 9999,
/* 157 */ 9999,
/* 158 */ 9999,
/* 159 */ 221059,
/* 160 */ 9999,
/* 161 */ 9999,
/* 162 */ 221059,
/* 163 */ 9999,
/* 164 */ 9999,
/* 165 */ 9999,
/* 166 */ 9999,
/* 167 */ 226005,
/* 168 */ 9999,
/* 169 */ 9999,
/* 170 */ 235009,
/* 171 */ 9999,
/* 172 */ 235005,
/* 173 */ 9999,
/* 174 */ 239005,
/* 175 */ 9999,
/* 176 */ 239018,
/* 177 */ 9999,
/* 178 */ 239009,
/* 179 */ 9999,
/* 180 */ 240005,
/* 181 */ 9999,
/* 182 */ 240018,
/* 183 */ 9999,
/* 184 */ 240009,
/* 185 */ 9999,
/* 186 */ 241005,
/* 187 */ 9999,
/* 188 */ 241010,
/* 189 */ 9999,
/* 190 */ 245005,
/* 191 */ 9999,
/* 192 */ 245019,
/* 193 */ 9999,
/* 194 */ 245010,
/* 195 */ 9999,
/* 196 */ 246005,
/* 197 */ 9999,
/* 198 */ 246019,
/* 199 */ 9999,
/* 200 */ 246010,
/* 201 */ 9999,
/* 202 */ 247005,
/* 203 */ 9999,
/* 204 */ 247019,
/* 205 */ 9999,
/* 206 */ 247010,
/* 207 */ 9999,
/* 208 */ 248005,
/* 209 */ 9999,
/* 210 */ 248019,
/* 211 */ 9999,
/* 212 */ 248010,
/* 213 */ 9999,
/* 214 */ 249005,
/* 215 */ 9999,
/* 216 */ 249019,
/* 217 */ 9999,
/* 218 */ 249010,
/* 219 */ 9999,
/* 220 */ 251005,
/* 221 */ 9999,
/* 222 */ 251010,
/* 223 */ 9999,
/* 224 */ 9999,
/* 225 */ 255009,
/* 226 */ 9999,
/* 227 */ 255005,
/* 228 */ 9999,
/* 229 */ 9999,
/* 230 */ 256009,
/* 231 */ 9999,
/* 232 */ 256005,
/* 233 */ 9999,
/* 234 */ 257005,
/* 235 */ 9999,
/* 236 */ 257010,
/* 237 */ 9999,
/* 238 */ 9999,
/* 239 */ 261009,
/* 240 */ 9999,
/* 241 */ 9999,
/* 242 */ 261005,
/* 243 */ 9999,
/* 244 */ 9999,
/* 245 */ 9999,
/* 246 */ 262010,
/* 247 */ 9999,
/* 248 */ 9999,
/* 249 */ 9999,
/* 250 */ 263013,
/* 251 */ 9999,
/* 252 */ 9999,
/* 253 */ 9999,
/* 254 */ 264014,
/* 255 */ 9999,
/* 256 */ 9999,
/* 257 */ 9999,
/* 258 */ 265014,
/* 259 */ 9999,
/* 260 */ 269005,
/* 261 */ 9999,
/* 262 */ 269021,
/* 263 */ 9999,
/* 264 */ 269037,
/* 265 */ 9999,
/* 266 */ 269010,
/* 267 */ 9999,
/* 268 */ 271005,
/* 269 */ 9999,
/* 270 */ 271021,
/* 271 */ 9999,
/* 272 */ 9999,
/* 273 */ 271041,
/* 274 */ 9999,
/* 275 */ 271010,
/* 276 */ 9999,
/* 277 */ 273005,
/* 278 */ 9999,
/* 279 */ 273021,
/* 280 */ 9999,
/* 281 */ 273010,
/* 282 */ 9999,
/* 283 */ 275005,
/* 284 */ 9999,
/* 285 */ 275010,
/* 286 */ 9999,
/* 287 */ 280005,
/* 288 */ 9999,
/* 289 */ 280021,
/* 290 */ 9999,
/* 291 */ 280037,
/* 292 */ 9999,
/* 293 */ 280010,
/* 294 */ 9999,
/* 295 */ 282005,
/* 296 */ 9999,
/* 297 */ 282021,
/* 298 */ 9999,
/* 299 */ 282010,
/* 300 */ 9999,
/* 301 */ 296003,
/* 302 */ 297003,
/* 303 */ 298003,
/* 304 */ 299003,
/* 305 */ 301003,
/* 306 */ 303003,
/* 307 */ 304003,
/* 308 */ 9999,
/* 309 */ 288003,
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
/* 364 */ 303044,
/* 365 */ 9999,
/* 366 */ 9999,
/* 367 */ 303044,
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
/* 1273 */ 505003,
/* 1274 */ 9999,
/* 1275 */ 485032,
/* 1276 */ 9999,
/* 1277 */ 517005,
/* 1278 */ 9999,
/* 1279 */ 517020,
/* 1280 */ 519019,
/* 1281 */ 9999,
/* 1282 */ 517010,
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
/* 1333 */ 506005,
/* 1334 */ 507005,
/* 1335 */ 508005,
/* 1336 */ 509005,
/* 1337 */ 505006,
/* 1338 */ 9999,
/* 1339 */ 505006,
/* 1340 */ 9999,
/* 1341 */ 9999,
/* 1342 */ 505006,
/* 1343 */ 9999,
/* 1344 */ 506007,
/* 1345 */ 9999,
/* 1346 */ 506015,
/* 1347 */ 9999,
/* 1348 */ 9999,
/* 1349 */ 506015,
/* 1350 */ 9999,
/* 1351 */ 507006,
/* 1352 */ 9999,
/* 1353 */ 507011,
/* 1354 */ 9999,
/* 1355 */ 9999,
/* 1356 */ 507011,
/* 1357 */ 9999,
/* 1358 */ 9999,
/* 1359 */ 9999,
/* 1360 */ 508008,
/* 1361 */ 9999,
/* 1362 */ 9999,
/* 1363 */ 508008,
/* 1364 */ 9999,
/* 1365 */ 9999,
/* 1366 */ 509011,
/* 1367 */ 509040,
/* 1368 */ 9999,
/* 1369 */ 9999,
/* 1370 */ 509007,
/* 1371 */ 9999,
/* 1372 */ 9999,
/* 1373 */ 509007,
/* 1374 */ 9999,
/* 1375 */ 9999,
/* 1376 */ 9999,
/* 1377 */ 509018,
/* 1378 */ 9999,
/* 1379 */ 9999,
/* 1380 */ 9999,
/* 1381 */ 509026,
/* 1382 */ 9999,
/* 1383 */ 9999,
/* 1384 */ 9999,
/* 1385 */ 509037,
/* 1386 */ 9999,
/* 1387 */ 519021,
/* 1388 */ 9999,
/* 1389 */ 519026,
/* 1390 */ 9999,
/* 1391 */ 9999,
/* 1392 */ 519026,
/* 1393 */ 9999,
/* 1394 */ 528003,
/* 1395 */ 528015,
/* 1396 */ 9999,
/* 1397 */ 528008,
/* 1398 */ 9999,
/* 1399 */ 9999,
/* 1400 */ 529009,
/* 1401 */ 529020,
/* 1402 */ 9999,
/* 1403 */ 529005,
/* 1404 */ 9999,
/* 1405 */ 9999,
/* 1406 */ 9999,
/* 1407 */ 528016,
/* 1408 */ 9999,
/* 1409 */ 9999,
/* 1410 */ 528016,
/* 1411 */ 9999,
/* 1412 */ 9999,
/* 1413 */ 9999,
/* 1414 */ 529021,
/* 1415 */ 9999,
/* 1416 */ 9999,
/* 1417 */ 529021,
/* 1418 */ 9999,
/* 1419 */ 9999,
/* 1420 */ 533017,
/* 1421 */ 533024,
/* 1422 */ 9999,
/* 1423 */ 533011,
/* 1424 */ 9999,
/* 1425 */ 9999,
/* 1426 */ 9999,
/* 1427 */ 533018,
/* 1428 */ 9999,
/* 1429 */ 9999,
/* 1430 */ 533018,
/* 1431 */ 9999,
/* 1432 */ 9999,
/* 1433 */ 9999,
/* 1434 */ 9999,
/* 1435 */ 542009,
/* 1436 */ 9999,
/* 1437 */ 543009,
/* 1438 */ 9999,
/* 1439 */ 9999,
/* 1440 */ 537006,
/* 1441 */ 9999,
/* 1442 */ 543010,
/* 1443 */ 543019,
/* 1444 */ 9999,
/* 1445 */ 543019,
/* 1446 */ 9999,
/* 1447 */ 9999,
/* 1448 */ 543019,
/* 1449 */ 9999,
/* 1450 */ 9999,
/* 1451 */ 547015,
/* 1452 */ 9999,
/* 1453 */ 9999,
/* 1454 */ 9999,
/* 1455 */ 547013,
/* 1456 */ 9999,
/* 1457 */ 9999,
/* 1458 */ 548017,
/* 1459 */ 548024,
/* 1460 */ 9999,
/* 1461 */ 9999,
/* 1462 */ 548015,
/* 1463 */ 9999,
/* 1464 */ 549005,
/* 1465 */ 9999,
/* 1466 */ 549015,
/* 1467 */ 9999,
/* 1468 */ 9999,
/* 1469 */ 550010,
/* 1470 */ 550017,
/* 1471 */ 9999,
/* 1472 */ 9999,
/* 1473 */ 550008,
/* 1474 */ 9999,
/* 1475 */ 9999,
/* 1476 */ 551010,
/* 1477 */ 551017,
/* 1478 */ 9999,
/* 1479 */ 9999,
/* 1480 */ 551008,
/* 1481 */ 9999,
/* 1482 */ 9999,
/* 1483 */ 552010,
/* 1484 */ 552017,
/* 1485 */ 9999,
/* 1486 */ 9999,
/* 1487 */ 552008,
/* 1488 */ 9999,
/* 1489 */ 9999,
/* 1490 */ 553012,
/* 1491 */ 553019,
/* 1492 */ 9999,
/* 1493 */ 9999,
/* 1494 */ 553010,
/* 1495 */ 9999,
/* 1496 */ 9999,
/* 1497 */ 554009,
/* 1498 */ 554016,
/* 1499 */ 9999,
/* 1500 */ 9999,
/* 1501 */ 554007,
/* 1502 */ 9999,
/* 1503 */ 9999,
/* 1504 */ 555011,
/* 1505 */ 555018,
/* 1506 */ 9999,
/* 1507 */ 9999,
/* 1508 */ 555009,
/* 1509 */ 9999,
/* 1510 */ 9999,
/* 1511 */ 556011,
/* 1512 */ 556018,
/* 1513 */ 9999,
/* 1514 */ 9999,
/* 1515 */ 9999,
/* 1516 */ 556009,
/* 1517 */ 9999,
/* 1518 */ 9999,
/* 1519 */ 557011,
/* 1520 */ 9999,
/* 1521 */ 557022,
/* 1522 */ 9999,
/* 1523 */ 9999,
/* 1524 */ 9999,
/* 1525 */ 557009,
/* 1526 */ 9999,
/* 1527 */ 9999,
/* 1528 */ 558011,
/* 1529 */ 558018,
/* 1530 */ 9999,
/* 1531 */ 9999,
/* 1532 */ 558009,
/* 1533 */ 9999,
/* 1534 */ 9999,
/* 1535 */ 559011,
/* 1536 */ 559018,
/* 1537 */ 9999,
/* 1538 */ 9999,
/* 1539 */ 559009,
/* 1540 */ 9999,
/* 1541 */ 9999,
/* 1542 */ 560014,
/* 1543 */ 560021,
/* 1544 */ 9999,
/* 1545 */ 9999,
/* 1546 */ 560012,
/* 1547 */ 9999,
/* 1548 */ 9999,
/* 1549 */ 561010,
/* 1550 */ 561017,
/* 1551 */ 562005,
/* 1552 */ 9999,
/* 1553 */ 9999,
/* 1554 */ 561008,
/* 1555 */ 9999,
/* 1556 */ 9999,
/* 1557 */ 572012,
/* 1558 */ 572019,
/* 1559 */ 9999,
/* 1560 */ 9999,
/* 1561 */ 572010,
/* 1562 */ 9999,
/* 1563 */ 9999,
/* 1564 */ 579010,
/* 1565 */ 579017,
/* 1566 */ 9999,
/* 1567 */ 9999,
/* 1568 */ 579008,
/* 1569 */ 9999,
/* 1570 */ 9999,
/* 1571 */ 580015,
/* 1572 */ 580022,
/* 1573 */ 9999,
/* 1574 */ 9999,
/* 1575 */ 580013,
/* 1576 */ 9999,
/* 1577 */ 9999,
/* 1578 */ 581009,
/* 1579 */ 581016,
/* 1580 */ 9999,
/* 1581 */ 9999,
/* 1582 */ 581007,
/* 1583 */ 9999,
/* 1584 */ 9999,
/* 1585 */ 582014,
/* 1586 */ 582021,
/* 1587 */ 9999,
/* 1588 */ 9999,
/* 1589 */ 582012,
/* 1590 */ 9999,
/* 1591 */ 9999,
/* 1592 */ 583013,
/* 1593 */ 583020,
/* 1594 */ 9999,
/* 1595 */ 9999,
/* 1596 */ 583011,
/* 1597 */ 9999,
/* 1598 */ 9999,
/* 1599 */ 584009,
/* 1600 */ 584016,
/* 1601 */ 9999,
/* 1602 */ 9999,
/* 1603 */ 584007,
/* 1604 */ 9999,
/* 1605 */ 9999,
/* 1606 */ 585015,
/* 1607 */ 585022,
/* 1608 */ 9999,
/* 1609 */ 9999,
/* 1610 */ 585013,
/* 1611 */ 9999,
/* 1612 */ 9999,
/* 1613 */ 586012,
/* 1614 */ 586019,
/* 1615 */ 9999,
/* 1616 */ 9999,
/* 1617 */ 586010,
/* 1618 */ 9999,
/* 1619 */ 9999,
/* 1620 */ 588011,
/* 1621 */ 588018,
/* 1622 */ 9999,
/* 1623 */ 9999,
/* 1624 */ 588009,
/* 1625 */ 9999,
/* 1626 */ 9999,
/* 1627 */ 590011,
/* 1628 */ 590018,
/* 1629 */ 9999,
/* 1630 */ 9999,
/* 1631 */ 590009,
/* 1632 */ 9999,
/* 1633 */ 9999,
/* 1634 */ 591011,
/* 1635 */ 591018,
/* 1636 */ 9999,
/* 1637 */ 9999,
/* 1638 */ 591009,
/* 1639 */ 9999,
/* 1640 */ 9999,
/* 1641 */ 592014,
/* 1642 */ 592021,
/* 1643 */ 9999,
/* 1644 */ 9999,
/* 1645 */ 592012,
/* 1646 */ 9999,
/* 1647 */ 9999,
/* 1648 */ 593016,
/* 1649 */ 593023,
/* 1650 */ 9999,
/* 1651 */ 9999,
/* 1652 */ 593014,
/* 1653 */ 9999,
/* 1654 */ 9999,
/* 1655 */ 594016,
/* 1656 */ 594023,
/* 1657 */ 9999,
/* 1658 */ 9999,
/* 1659 */ 594014,
/* 1660 */ 9999,
/* 1661 */ 9999,
/* 1662 */ 595014,
/* 1663 */ 595021,
/* 1664 */ 9999,
/* 1665 */ 9999,
/* 1666 */ 595012,
/* 1667 */ 9999,
/* 1668 */ 9999,
/* 1669 */ 596013,
/* 1670 */ 596020,
/* 1671 */ 9999,
/* 1672 */ 9999,
/* 1673 */ 596011,
/* 1674 */ 9999,
/* 1675 */ 9999,
/* 1676 */ 597011,
/* 1677 */ 597018,
/* 1678 */ 9999,
/* 1679 */ 9999,
/* 1680 */ 597009,
/* 1681 */ 9999,
/* 1682 */ 9999,
/* 1683 */ 9999,
/* 1684 */ 598017,
/* 1685 */ 598024,
/* 1686 */ 9999,
/* 1687 */ 9999,
/* 1688 */ 598009,
/* 1689 */ 9999,
/* 1690 */ 9999,
/* 1691 */ 599014,
/* 1692 */ 599021,
/* 1693 */ 9999,
/* 1694 */ 9999,
/* 1695 */ 599012,
/* 1696 */ 9999,
/* 1697 */ 9999,
/* 1698 */ 600010,
/* 1699 */ 600017,
/* 1700 */ 9999,
/* 1701 */ 9999,
/* 1702 */ 600008,
/* 1703 */ 9999,
/* 1704 */ 601005,
/* 1705 */ 9999,
/* 1706 */ 601016,
/* 1707 */ 9999,
/* 1708 */ 602005,
/* 1709 */ 9999,
/* 1710 */ 602014,
/* 1711 */ 9999,
/* 1712 */ 603005,
/* 1713 */ 9999,
/* 1714 */ 603017,
/* 1715 */ 9999,
/* 1716 */ 9999,
/* 1717 */ 9999,
/* 1718 */ 547016,
/* 1719 */ 9999,
/* 1720 */ 9999,
/* 1721 */ 547016,
/* 1722 */ 9999,
/* 1723 */ 9999,
/* 1724 */ 9999,
/* 1725 */ 548018,
/* 1726 */ 9999,
/* 1727 */ 9999,
/* 1728 */ 548018,
/* 1729 */ 9999,
/* 1730 */ 9999,
/* 1731 */ 9999,
/* 1732 */ 550011,
/* 1733 */ 9999,
/* 1734 */ 9999,
/* 1735 */ 550011,
/* 1736 */ 9999,
/* 1737 */ 9999,
/* 1738 */ 9999,
/* 1739 */ 551011,
/* 1740 */ 9999,
/* 1741 */ 9999,
/* 1742 */ 551011,
/* 1743 */ 9999,
/* 1744 */ 9999,
/* 1745 */ 9999,
/* 1746 */ 552011,
/* 1747 */ 9999,
/* 1748 */ 9999,
/* 1749 */ 552011,
/* 1750 */ 9999,
/* 1751 */ 9999,
/* 1752 */ 9999,
/* 1753 */ 553013,
/* 1754 */ 9999,
/* 1755 */ 9999,
/* 1756 */ 553013,
/* 1757 */ 9999,
/* 1758 */ 9999,
/* 1759 */ 9999,
/* 1760 */ 554010,
/* 1761 */ 9999,
/* 1762 */ 9999,
/* 1763 */ 554010,
/* 1764 */ 9999,
/* 1765 */ 9999,
/* 1766 */ 9999,
/* 1767 */ 555012,
/* 1768 */ 9999,
/* 1769 */ 9999,
/* 1770 */ 555012,
/* 1771 */ 9999,
/* 1772 */ 9999,
/* 1773 */ 9999,
/* 1774 */ 556012,
/* 1775 */ 9999,
/* 1776 */ 9999,
/* 1777 */ 556012,
/* 1778 */ 9999,
/* 1779 */ 9999,
/* 1780 */ 9999,
/* 1781 */ 557012,
/* 1782 */ 9999,
/* 1783 */ 9999,
/* 1784 */ 557012,
/* 1785 */ 9999,
/* 1786 */ 9999,
/* 1787 */ 9999,
/* 1788 */ 558012,
/* 1789 */ 9999,
/* 1790 */ 9999,
/* 1791 */ 558012,
/* 1792 */ 9999,
/* 1793 */ 9999,
/* 1794 */ 9999,
/* 1795 */ 559012,
/* 1796 */ 9999,
/* 1797 */ 9999,
/* 1798 */ 559012,
/* 1799 */ 9999,
/* 1800 */ 9999,
/* 1801 */ 9999,
/* 1802 */ 560015,
/* 1803 */ 9999,
/* 1804 */ 9999,
/* 1805 */ 560015,
/* 1806 */ 9999,
/* 1807 */ 9999,
/* 1808 */ 9999,
/* 1809 */ 561011,
/* 1810 */ 9999,
/* 1811 */ 9999,
/* 1812 */ 561011,
/* 1813 */ 9999,
/* 1814 */ 562007,
/* 1815 */ 9999,
/* 1816 */ 562012,
/* 1817 */ 9999,
/* 1818 */ 9999,
/* 1819 */ 562012,
/* 1820 */ 9999,
/* 1821 */ 9999,
/* 1822 */ 9999,
/* 1823 */ 572013,
/* 1824 */ 9999,
/* 1825 */ 9999,
/* 1826 */ 572013,
/* 1827 */ 9999,
/* 1828 */ 9999,
/* 1829 */ 9999,
/* 1830 */ 579011,
/* 1831 */ 9999,
/* 1832 */ 9999,
/* 1833 */ 579011,
/* 1834 */ 9999,
/* 1835 */ 9999,
/* 1836 */ 9999,
/* 1837 */ 580016,
/* 1838 */ 9999,
/* 1839 */ 9999,
/* 1840 */ 580016,
/* 1841 */ 9999,
/* 1842 */ 9999,
/* 1843 */ 9999,
/* 1844 */ 581010,
/* 1845 */ 9999,
/* 1846 */ 9999,
/* 1847 */ 581010,
/* 1848 */ 9999,
/* 1849 */ 9999,
/* 1850 */ 9999,
/* 1851 */ 582015,
/* 1852 */ 9999,
/* 1853 */ 9999,
/* 1854 */ 582015,
/* 1855 */ 9999,
/* 1856 */ 9999,
/* 1857 */ 9999,
/* 1858 */ 583014,
/* 1859 */ 9999,
/* 1860 */ 9999,
/* 1861 */ 583014,
/* 1862 */ 9999,
/* 1863 */ 9999,
/* 1864 */ 9999,
/* 1865 */ 584010,
/* 1866 */ 9999,
/* 1867 */ 9999,
/* 1868 */ 584010,
/* 1869 */ 9999,
/* 1870 */ 584017,
/* 1871 */ 584025,
/* 1872 */ 9999,
/* 1873 */ 584025,
/* 1874 */ 9999,
/* 1875 */ 9999,
/* 1876 */ 584025,
/* 1877 */ 9999,
/* 1878 */ 9999,
/* 1879 */ 9999,
/* 1880 */ 585016,
/* 1881 */ 9999,
/* 1882 */ 9999,
/* 1883 */ 585016,
/* 1884 */ 9999,
/* 1885 */ 9999,
/* 1886 */ 9999,
/* 1887 */ 586013,
/* 1888 */ 9999,
/* 1889 */ 9999,
/* 1890 */ 586013,
/* 1891 */ 9999,
/* 1892 */ 586021,
/* 1893 */ 587006,
/* 1894 */ 586027,
/* 1895 */ 9999,
/* 1896 */ 586027,
/* 1897 */ 9999,
/* 1898 */ 9999,
/* 1899 */ 586027,
/* 1900 */ 9999,
/* 1901 */ 9999,
/* 1902 */ 9999,
/* 1903 */ 587007,
/* 1904 */ 9999,
/* 1905 */ 9999,
/* 1906 */ 587007,
/* 1907 */ 9999,
/* 1908 */ 9999,
/* 1909 */ 9999,
/* 1910 */ 588012,
/* 1911 */ 9999,
/* 1912 */ 9999,
/* 1913 */ 588012,
/* 1914 */ 9999,
/* 1915 */ 588020,
/* 1916 */ 589006,
/* 1917 */ 588026,
/* 1918 */ 9999,
/* 1919 */ 588026,
/* 1920 */ 9999,
/* 1921 */ 9999,
/* 1922 */ 588026,
/* 1923 */ 9999,
/* 1924 */ 9999,
/* 1925 */ 9999,
/* 1926 */ 589007,
/* 1927 */ 9999,
/* 1928 */ 9999,
/* 1929 */ 589007,
/* 1930 */ 9999,
/* 1931 */ 9999,
/* 1932 */ 9999,
/* 1933 */ 590012,
/* 1934 */ 9999,
/* 1935 */ 9999,
/* 1936 */ 590012,
/* 1937 */ 9999,
/* 1938 */ 590019,
/* 1939 */ 590029,
/* 1940 */ 9999,
/* 1941 */ 590029,
/* 1942 */ 9999,
/* 1943 */ 9999,
/* 1944 */ 590029,
/* 1945 */ 9999,
/* 1946 */ 9999,
/* 1947 */ 9999,
/* 1948 */ 591012,
/* 1949 */ 9999,
/* 1950 */ 9999,
/* 1951 */ 591012,
/* 1952 */ 9999,
/* 1953 */ 591020,
/* 1954 */ 591030,
/* 1955 */ 591025,
/* 1956 */ 9999,
/* 1957 */ 591025,
/* 1958 */ 9999,
/* 1959 */ 9999,
/* 1960 */ 591025,
/* 1961 */ 9999,
/* 1962 */ 9999,
/* 1963 */ 9999,
/* 1964 */ 591031,
/* 1965 */ 9999,
/* 1966 */ 9999,
/* 1967 */ 9999,
/* 1968 */ 591035,
/* 1969 */ 9999,
/* 1970 */ 9999,
/* 1971 */ 591035,
/* 1972 */ 9999,
/* 1973 */ 9999,
/* 1974 */ 9999,
/* 1975 */ 592015,
/* 1976 */ 9999,
/* 1977 */ 9999,
/* 1978 */ 592015,
/* 1979 */ 9999,
/* 1980 */ 9999,
/* 1981 */ 9999,
/* 1982 */ 593017,
/* 1983 */ 9999,
/* 1984 */ 9999,
/* 1985 */ 593017,
/* 1986 */ 9999,
/* 1987 */ 9999,
/* 1988 */ 9999,
/* 1989 */ 594017,
/* 1990 */ 9999,
/* 1991 */ 9999,
/* 1992 */ 594017,
/* 1993 */ 9999,
/* 1994 */ 9999,
/* 1995 */ 9999,
/* 1996 */ 595015,
/* 1997 */ 9999,
/* 1998 */ 9999,
/* 1999 */ 595015,
/* 2000 */ 9999,
/* 2001 */ 9999,
/* 2002 */ 9999,
/* 2003 */ 596014,
/* 2004 */ 9999,
/* 2005 */ 9999,
/* 2006 */ 596014,
/* 2007 */ 9999,
/* 2008 */ 9999,
/* 2009 */ 9999,
/* 2010 */ 597012,
/* 2011 */ 9999,
/* 2012 */ 9999,
/* 2013 */ 597012,
/* 2014 */ 9999,
/* 2015 */ 9999,
/* 2016 */ 9999,
/* 2017 */ 598018,
/* 2018 */ 9999,
/* 2019 */ 9999,
/* 2020 */ 598018,
/* 2021 */ 9999,
/* 2022 */ 9999,
/* 2023 */ 9999,
/* 2024 */ 599015,
/* 2025 */ 9999,
/* 2026 */ 9999,
/* 2027 */ 599015,
/* 2028 */ 9999,
/* 2029 */ 9999,
/* 2030 */ 9999,
/* 2031 */ 600011,
/* 2032 */ 9999,
/* 2033 */ 9999,
/* 2034 */ 600011,
/* 2035 */ 9999,
/* 2036 */ 607003,
/* 2037 */ 607014,
/* 2038 */ 9999,
/* 2039 */ 607008,
/* 2040 */ 9999,
/* 2041 */ 9999,
/* 2042 */ 611018,
/* 2043 */ 611028,
/* 2044 */ 9999,
/* 2045 */ 611012,
/* 2046 */ 9999,
/* 2047 */ 9999,
/* 2048 */ 9999,
/* 2049 */ 607015,
/* 2050 */ 9999,
/* 2051 */ 9999,
/* 2052 */ 607015,
/* 2053 */ 9999,
/* 2054 */ 9999,
/* 2055 */ 9999,
/* 2056 */ 611029,
/* 2057 */ 9999,
/* 2058 */ 9999,
/* 2059 */ 611029,
/* 2060 */ 9999,
/* 2061 */ 615005,
/* 2062 */ 615018,
/* 2063 */ 9999,
/* 2064 */ 615010,
/* 2065 */ 9999,
/* 2066 */ 9999,
/* 2067 */ 616009,
/* 2068 */ 616021,
/* 2069 */ 9999,
/* 2070 */ 616005,
/* 2071 */ 9999,
/* 2072 */ 9999,
/* 2073 */ 9999,
/* 2074 */ 615019,
/* 2075 */ 9999,
/* 2076 */ 9999,
/* 2077 */ 615019,
/* 2078 */ 9999,
/* 2079 */ 9999,
/* 2080 */ 9999,
/* 2081 */ 616022,
/* 2082 */ 9999,
/* 2083 */ 9999,
/* 2084 */ 616022,
/* 2085 */ 9999,
/* 2086 */ 9999,
/* 2087 */ 9999,
/* 2088 */ 9999,
/* 2089 */ 623007,
/* 2090 */ 9999,
/* 2091 */ 624007,
/* 2092 */ 9999,
/* 2093 */ 9999,
/* 2094 */ 621009,
/* 2095 */ 9999,
/* 2096 */ 624008,
/* 2097 */ 624017,
/* 2098 */ 9999,
/* 2099 */ 624017,
/* 2100 */ 9999,
/* 2101 */ 9999,
/* 2102 */ 624017,
/* 2103 */ 9999,
/* 2104 */ 9999,
/* 2105 */ 628016,
/* 2106 */ 9999,
/* 2107 */ 9999,
/* 2108 */ 9999,
/* 2109 */ 628014,
/* 2110 */ 9999,
/* 2111 */ 9999,
/* 2112 */ 629015,
/* 2113 */ 629022,
/* 2114 */ 9999,
/* 2115 */ 9999,
/* 2116 */ 629013,
/* 2117 */ 9999,
/* 2118 */ 630005,
/* 2119 */ 9999,
/* 2120 */ 630015,
/* 2121 */ 9999,
/* 2122 */ 9999,
/* 2123 */ 631010,
/* 2124 */ 631017,
/* 2125 */ 9999,
/* 2126 */ 9999,
/* 2127 */ 9999,
/* 2128 */ 631008,
/* 2129 */ 9999,
/* 2130 */ 9999,
/* 2131 */ 632015,
/* 2132 */ 632022,
/* 2133 */ 9999,
/* 2134 */ 9999,
/* 2135 */ 632013,
/* 2136 */ 9999,
/* 2137 */ 9999,
/* 2138 */ 633011,
/* 2139 */ 633019,
/* 2140 */ 9999,
/* 2141 */ 9999,
/* 2142 */ 633009,
/* 2143 */ 9999,
/* 2144 */ 634005,
/* 2145 */ 634028,
/* 2146 */ 634035,
/* 2147 */ 9999,
/* 2148 */ 9999,
/* 2149 */ 634005,
/* 2150 */ 9999,
/* 2151 */ 9999,
/* 2152 */ 635010,
/* 2153 */ 635017,
/* 2154 */ 9999,
/* 2155 */ 9999,
/* 2156 */ 635008,
/* 2157 */ 9999,
/* 2158 */ 9999,
/* 2159 */ 636012,
/* 2160 */ 636019,
/* 2161 */ 9999,
/* 2162 */ 9999,
/* 2163 */ 636010,
/* 2164 */ 9999,
/* 2165 */ 637005,
/* 2166 */ 9999,
/* 2167 */ 637014,
/* 2168 */ 9999,
/* 2169 */ 638005,
/* 2170 */ 9999,
/* 2171 */ 638016,
/* 2172 */ 9999,
/* 2173 */ 639005,
/* 2174 */ 9999,
/* 2175 */ 639017,
/* 2176 */ 9999,
/* 2177 */ 9999,
/* 2178 */ 9999,
/* 2179 */ 628017,
/* 2180 */ 9999,
/* 2181 */ 9999,
/* 2182 */ 628017,
/* 2183 */ 9999,
/* 2184 */ 9999,
/* 2185 */ 9999,
/* 2186 */ 629016,
/* 2187 */ 9999,
/* 2188 */ 9999,
/* 2189 */ 629016,
/* 2190 */ 9999,
/* 2191 */ 9999,
/* 2192 */ 9999,
/* 2193 */ 631011,
/* 2194 */ 9999,
/* 2195 */ 9999,
/* 2196 */ 631011,
/* 2197 */ 9999,
/* 2198 */ 9999,
/* 2199 */ 9999,
/* 2200 */ 632016,
/* 2201 */ 9999,
/* 2202 */ 9999,
/* 2203 */ 632016,
/* 2204 */ 9999,
/* 2205 */ 9999,
/* 2206 */ 9999,
/* 2207 */ 633012,
/* 2208 */ 9999,
/* 2209 */ 9999,
/* 2210 */ 633012,
/* 2211 */ 9999,
/* 2212 */ 633020,
/* 2213 */ 633030,
/* 2214 */ 633025,
/* 2215 */ 9999,
/* 2216 */ 633025,
/* 2217 */ 9999,
/* 2218 */ 9999,
/* 2219 */ 633025,
/* 2220 */ 9999,
/* 2221 */ 9999,
/* 2222 */ 9999,
/* 2223 */ 633031,
/* 2224 */ 9999,
/* 2225 */ 9999,
/* 2226 */ 9999,
/* 2227 */ 633037,
/* 2228 */ 9999,
/* 2229 */ 9999,
/* 2230 */ 633037,
/* 2231 */ 9999,
/* 2232 */ 9999,
/* 2233 */ 9999,
/* 2234 */ 634016,
/* 2235 */ 9999,
/* 2236 */ 9999,
/* 2237 */ 9999,
/* 2238 */ 634025,
/* 2239 */ 9999,
/* 2240 */ 9999,
/* 2241 */ 9999,
/* 2242 */ 634029,
/* 2243 */ 9999,
/* 2244 */ 9999,
/* 2245 */ 634029,
/* 2246 */ 9999,
/* 2247 */ 9999,
/* 2248 */ 9999,
/* 2249 */ 635011,
/* 2250 */ 9999,
/* 2251 */ 9999,
/* 2252 */ 635011,
/* 2253 */ 9999,
/* 2254 */ 9999,
/* 2255 */ 9999,
/* 2256 */ 636013,
/* 2257 */ 9999,
/* 2258 */ 9999,
/* 2259 */ 636013,
/* 2260 */ 9999,
/* 2261 */ 643003,
/* 2262 */ 652003,
/* 2263 */ 9999,
/* 2264 */ 9999,
/* 2265 */ 656007,
/* 2266 */ 9999,
/* 2267 */ 658007,
/* 2268 */ 9999,
/* 2269 */ 659003,
/* 2270 */ 9999,
/* 2271 */ 642037,
/* 2272 */ 9999,
/* 2273 */ 643005,
/* 2274 */ 651005,
/* 2275 */ 643010,
/* 2276 */ 9999,
/* 2277 */ 643010,
/* 2278 */ 9999,
/* 2279 */ 9999,
/* 2280 */ 643010,
/* 2281 */ 9999,
/* 2282 */ 9999,
/* 2283 */ 9999,
/* 2284 */ 651006,
/* 2285 */ 9999,
/* 2286 */ 9999,
/* 2287 */ 651006,
/* 2288 */ 9999,
/* 2289 */ 9999,
/* 2290 */ 9999,
/* 2291 */ 652008,
/* 2292 */ 9999,
/* 2293 */ 9999,
/* 2294 */ 9999,
/* 2295 */ 652017,
/* 2296 */ 9999,
/* 2297 */ 9999,
/* 2298 */ 9999,
/* 2299 */ 653012,
/* 2300 */ 9999,
/* 2301 */ 9999,
/* 2302 */ 9999,
/* 2303 */ 654013,
/* 2304 */ 9999,
/* 2305 */ 658008,
/* 2306 */ 658016,
/* 2307 */ 9999,
/* 2308 */ 658016,
/* 2309 */ 9999,
/* 2310 */ 9999,
/* 2311 */ 658016,
/* 2312 */ 9999,
/* 2313 */ 9999,
/* 2314 */ 659009,
/* 2315 */ 9999,
/* 2316 */ 680008,
/* 2317 */ 9999,
/* 2318 */ 659007,
/* 2319 */ 9999,
/* 2320 */ 659007,
/* 2321 */ 9999,
/* 2322 */ 9999,
/* 2323 */ 659007,
/* 2324 */ 9999,
/* 2325 */ 680009,
/* 2326 */ 680017,
/* 2327 */ 9999,
/* 2328 */ 680017,
/* 2329 */ 9999,
/* 2330 */ 9999,
/* 2331 */ 680017,
/* 2332 */ 9999,
/* 2333 */ 9999,
/* 2334 */ 685015,
/* 2335 */ 9999,
/* 2336 */ 9999,
/* 2337 */ 9999,
/* 2338 */ 685013,
/* 2339 */ 9999,
/* 2340 */ 686005,
/* 2341 */ 9999,
/* 2342 */ 9999,
/* 2343 */ 686009,
/* 2344 */ 9999,
/* 2345 */ 9999,
/* 2346 */ 687016,
/* 2347 */ 687023,
/* 2348 */ 9999,
/* 2349 */ 9999,
/* 2350 */ 687014,
/* 2351 */ 9999,
/* 2352 */ 9999,
/* 2353 */ 688010,
/* 2354 */ 688017,
/* 2355 */ 9999,
/* 2356 */ 9999,
/* 2357 */ 688008,
/* 2358 */ 9999,
/* 2359 */ 9999,
/* 2360 */ 689010,
/* 2361 */ 689020,
/* 2362 */ 689027,
/* 2363 */ 689037,
/* 2364 */ 9999,
/* 2365 */ 9999,
/* 2366 */ 689008,
/* 2367 */ 9999,
/* 2368 */ 9999,
/* 2369 */ 690013,
/* 2370 */ 690020,
/* 2371 */ 691007,
/* 2372 */ 9999,
/* 2373 */ 9999,
/* 2374 */ 690011,
/* 2375 */ 9999,
/* 2376 */ 9999,
/* 2377 */ 692010,
/* 2378 */ 692017,
/* 2379 */ 692027,
/* 2380 */ 9999,
/* 2381 */ 9999,
/* 2382 */ 692008,
/* 2383 */ 9999,
/* 2384 */ 9999,
/* 2385 */ 694010,
/* 2386 */ 694017,
/* 2387 */ 9999,
/* 2388 */ 9999,
/* 2389 */ 694008,
/* 2390 */ 9999,
/* 2391 */ 9999,
/* 2392 */ 695010,
/* 2393 */ 695017,
/* 2394 */ 9999,
/* 2395 */ 9999,
/* 2396 */ 695008,
/* 2397 */ 9999,
/* 2398 */ 9999,
/* 2399 */ 696010,
/* 2400 */ 696017,
/* 2401 */ 9999,
/* 2402 */ 9999,
/* 2403 */ 696008,
/* 2404 */ 9999,
/* 2405 */ 9999,
/* 2406 */ 697010,
/* 2407 */ 697017,
/* 2408 */ 9999,
/* 2409 */ 9999,
/* 2410 */ 697008,
/* 2411 */ 9999,
/* 2412 */ 9999,
/* 2413 */ 698010,
/* 2414 */ 698017,
/* 2415 */ 9999,
/* 2416 */ 9999,
/* 2417 */ 698008,
/* 2418 */ 9999,
/* 2419 */ 9999,
/* 2420 */ 699010,
/* 2421 */ 699017,
/* 2422 */ 9999,
/* 2423 */ 9999,
/* 2424 */ 699008,
/* 2425 */ 9999,
/* 2426 */ 9999,
/* 2427 */ 700010,
/* 2428 */ 700017,
/* 2429 */ 9999,
/* 2430 */ 9999,
/* 2431 */ 700008,
/* 2432 */ 9999,
/* 2433 */ 9999,
/* 2434 */ 701010,
/* 2435 */ 701017,
/* 2436 */ 9999,
/* 2437 */ 9999,
/* 2438 */ 701008,
/* 2439 */ 9999,
/* 2440 */ 9999,
/* 2441 */ 702010,
/* 2442 */ 702017,
/* 2443 */ 9999,
/* 2444 */ 9999,
/* 2445 */ 702008,
/* 2446 */ 9999,
/* 2447 */ 9999,
/* 2448 */ 703010,
/* 2449 */ 703017,
/* 2450 */ 9999,
/* 2451 */ 9999,
/* 2452 */ 703008,
/* 2453 */ 9999,
/* 2454 */ 9999,
/* 2455 */ 704010,
/* 2456 */ 704017,
/* 2457 */ 9999,
/* 2458 */ 9999,
/* 2459 */ 704008,
/* 2460 */ 9999,
/* 2461 */ 9999,
/* 2462 */ 705010,
/* 2463 */ 9999,
/* 2464 */ 9999,
/* 2465 */ 9999,
/* 2466 */ 705008,
/* 2467 */ 9999,
/* 2468 */ 9999,
/* 2469 */ 706011,
/* 2470 */ 706018,
/* 2471 */ 9999,
/* 2472 */ 9999,
/* 2473 */ 706009,
/* 2474 */ 9999,
/* 2475 */ 708005,
/* 2476 */ 9999,
/* 2477 */ 708014,
/* 2478 */ 9999,
/* 2479 */ 709005,
/* 2480 */ 9999,
/* 2481 */ 709017,
/* 2482 */ 9999,
/* 2483 */ 710005,
/* 2484 */ 9999,
/* 2485 */ 710016,
/* 2486 */ 9999,
/* 2487 */ 9999,
/* 2488 */ 9999,
/* 2489 */ 685016,
/* 2490 */ 9999,
/* 2491 */ 9999,
/* 2492 */ 685016,
/* 2493 */ 9999,
/* 2494 */ 9999,
/* 2495 */ 9999,
/* 2496 */ 687017,
/* 2497 */ 9999,
/* 2498 */ 9999,
/* 2499 */ 687017,
/* 2500 */ 9999,
/* 2501 */ 9999,
/* 2502 */ 9999,
/* 2503 */ 688011,
/* 2504 */ 9999,
/* 2505 */ 9999,
/* 2506 */ 688011,
/* 2507 */ 9999,
/* 2508 */ 9999,
/* 2509 */ 9999,
/* 2510 */ 689016,
/* 2511 */ 9999,
/* 2512 */ 9999,
/* 2513 */ 689016,
/* 2514 */ 9999,
/* 2515 */ 9999,
/* 2516 */ 9999,
/* 2517 */ 689021,
/* 2518 */ 9999,
/* 2519 */ 9999,
/* 2520 */ 689021,
/* 2521 */ 9999,
/* 2522 */ 9999,
/* 2523 */ 9999,
/* 2524 */ 689038,
/* 2525 */ 9999,
/* 2526 */ 9999,
/* 2527 */ 689038,
/* 2528 */ 9999,
/* 2529 */ 9999,
/* 2530 */ 9999,
/* 2531 */ 690014,
/* 2532 */ 9999,
/* 2533 */ 9999,
/* 2534 */ 690014,
/* 2535 */ 9999,
/* 2536 */ 9999,
/* 2537 */ 691013,
/* 2538 */ 9999,
/* 2539 */ 9999,
/* 2540 */ 691009,
/* 2541 */ 9999,
/* 2542 */ 9999,
/* 2543 */ 691009,
/* 2544 */ 9999,
/* 2545 */ 9999,
/* 2546 */ 9999,
/* 2547 */ 692011,
/* 2548 */ 9999,
/* 2549 */ 9999,
/* 2550 */ 692011,
/* 2551 */ 9999,
/* 2552 */ 9999,
/* 2553 */ 9999,
/* 2554 */ 692035,
/* 2555 */ 9999,
/* 2556 */ 9999,
/* 2557 */ 692035,
/* 2558 */ 9999,
/* 2559 */ 9999,
/* 2560 */ 9999,
/* 2561 */ 694011,
/* 2562 */ 9999,
/* 2563 */ 9999,
/* 2564 */ 694011,
/* 2565 */ 9999,
/* 2566 */ 9999,
/* 2567 */ 9999,
/* 2568 */ 695011,
/* 2569 */ 9999,
/* 2570 */ 9999,
/* 2571 */ 695011,
/* 2572 */ 9999,
/* 2573 */ 9999,
/* 2574 */ 9999,
/* 2575 */ 696011,
/* 2576 */ 9999,
/* 2577 */ 9999,
/* 2578 */ 696011,
/* 2579 */ 9999,
/* 2580 */ 9999,
/* 2581 */ 9999,
/* 2582 */ 697011,
/* 2583 */ 9999,
/* 2584 */ 9999,
/* 2585 */ 697011,
/* 2586 */ 9999,
/* 2587 */ 9999,
/* 2588 */ 9999,
/* 2589 */ 698011,
/* 2590 */ 9999,
/* 2591 */ 9999,
/* 2592 */ 698011,
/* 2593 */ 9999,
/* 2594 */ 9999,
/* 2595 */ 9999,
/* 2596 */ 699011,
/* 2597 */ 9999,
/* 2598 */ 9999,
/* 2599 */ 699011,
/* 2600 */ 9999,
/* 2601 */ 9999,
/* 2602 */ 9999,
/* 2603 */ 700011,
/* 2604 */ 9999,
/* 2605 */ 9999,
/* 2606 */ 700011,
/* 2607 */ 9999,
/* 2608 */ 9999,
/* 2609 */ 9999,
/* 2610 */ 701011,
/* 2611 */ 9999,
/* 2612 */ 9999,
/* 2613 */ 701011,
/* 2614 */ 9999,
/* 2615 */ 9999,
/* 2616 */ 9999,
/* 2617 */ 702011,
/* 2618 */ 9999,
/* 2619 */ 9999,
/* 2620 */ 702011,
/* 2621 */ 9999,
/* 2622 */ 9999,
/* 2623 */ 9999,
/* 2624 */ 703011,
/* 2625 */ 9999,
/* 2626 */ 9999,
/* 2627 */ 703011,
/* 2628 */ 9999,
/* 2629 */ 9999,
/* 2630 */ 9999,
/* 2631 */ 704011,
/* 2632 */ 9999,
/* 2633 */ 9999,
/* 2634 */ 704011,
/* 2635 */ 9999,
/* 2636 */ 9999,
/* 2637 */ 9999,
/* 2638 */ 705011,
/* 2639 */ 9999,
/* 2640 */ 9999,
/* 2641 */ 705011,
/* 2642 */ 9999,
/* 2643 */ 9999,
/* 2644 */ 9999,
/* 2645 */ 706012,
/* 2646 */ 9999,
/* 2647 */ 9999,
/* 2648 */ 706012,
/* 2649 */ 9999,
/* 2650 */ 706019,
/* 2651 */ 707005,
/* 2652 */ 706024,
/* 2653 */ 9999,
/* 2654 */ 706024,
/* 2655 */ 9999,
/* 2656 */ 9999,
/* 2657 */ 706024,
/* 2658 */ 9999,
/* 2659 */ 9999,
/* 2660 */ 9999,
/* 2661 */ 707006,
/* 2662 */ 9999,
/* 2663 */ 9999,
/* 2664 */ 9999,
/* 2665 */ 707012,
/* 2666 */ 9999,
/* 2667 */ 9999,
/* 2668 */ 707012,
/* 2669 */ 9999,
/* 2670 */ 714003,
/* 2671 */ 9999,
/* 2672 */ 713015,
/* 2673 */ 9999,
/* 2674 */ 714004,
/* 2675 */ 714029,
/* 2676 */ 714009,
/* 2677 */ 9999,
/* 2678 */ 714009,
/* 2679 */ 9999,
/* 2680 */ 9999,
/* 2681 */ 714009,
/* 2682 */ 9999,
/* 2683 */ 9999,
/* 2684 */ 9999,
/* 2685 */ 714030,
/* 2686 */ 9999,
/* 2687 */ 9999,
/* 2688 */ 9999,
/* 2689 */ 714036,
/* 2690 */ 9999,
/* 2691 */ 9999,
/* 2692 */ 714036,
/* 2693 */ 9999,
/* 2694 */ 9999,
/* 2695 */ 9999,
/* 2696 */ 720009,
/* 2697 */ 9999,
/* 2698 */ 9999,
/* 2699 */ 9999,
/* 2700 */ 725005,
/* 2701 */ 9999,
/* 2702 */ 730007,
/* 2703 */ 9999,
/* 2704 */ 9999,
/* 2705 */ 723006,
/* 2706 */ 9999,
/* 2707 */ 9999,
/* 2708 */ 725011,
/* 2709 */ 9999,
/* 2710 */ 725007,
/* 2711 */ 9999,
/* 2712 */ 9999,
/* 2713 */ 725007,
/* 2714 */ 9999,
/* 2715 */ 730008,
/* 2716 */ 730017,
/* 2717 */ 9999,
/* 2718 */ 730017,
/* 2719 */ 9999,
/* 2720 */ 9999,
/* 2721 */ 730017,
/* 2722 */ 9999,
/* 2723 */ 9999,
/* 2724 */ 734014,
/* 2725 */ 9999,
/* 2726 */ 9999,
/* 2727 */ 9999,
/* 2728 */ 734012,
/* 2729 */ 9999,
/* 2730 */ 735004,
/* 2731 */ 9999,
/* 2732 */ 9999,
/* 2733 */ 735004,
/* 2734 */ 9999,
/* 2735 */ 9999,
/* 2736 */ 737012,
/* 2737 */ 737019,
/* 2738 */ 9999,
/* 2739 */ 9999,
/* 2740 */ 737010,
/* 2741 */ 9999,
/* 2742 */ 9999,
/* 2743 */ 739013,
/* 2744 */ 739020,
/* 2745 */ 9999,
/* 2746 */ 9999,
/* 2747 */ 739011,
/* 2748 */ 9999,
/* 2749 */ 9999,
/* 2750 */ 740013,
/* 2751 */ 740022,
/* 2752 */ 740029,
/* 2753 */ 9999,
/* 2754 */ 740045,
/* 2755 */ 9999,
/* 2756 */ 9999,
/* 2757 */ 740011,
/* 2758 */ 9999,
/* 2759 */ 9999,
/* 2760 */ 743013,
/* 2761 */ 743022,
/* 2762 */ 743029,
/* 2763 */ 9999,
/* 2764 */ 743045,
/* 2765 */ 9999,
/* 2766 */ 743062,
/* 2767 */ 9999,
/* 2768 */ 9999,
/* 2769 */ 743011,
/* 2770 */ 9999,
/* 2771 */ 9999,
/* 2772 */ 747012,
/* 2773 */ 748007,
/* 2774 */ 9999,
/* 2775 */ 9999,
/* 2776 */ 747010,
/* 2777 */ 9999,
/* 2778 */ 749005,
/* 2779 */ 749015,
/* 2780 */ 9999,
/* 2781 */ 9999,
/* 2782 */ 9999,
/* 2783 */ 749010,
/* 2784 */ 9999,
/* 2785 */ 9999,
/* 2786 */ 755011,
/* 2787 */ 755019,
/* 2788 */ 9999,
/* 2789 */ 9999,
/* 2790 */ 755009,
/* 2791 */ 9999,
/* 2792 */ 756005,
/* 2793 */ 9999,
/* 2794 */ 756013,
/* 2795 */ 9999,
/* 2796 */ 757005,
/* 2797 */ 9999,
/* 2798 */ 757016,
/* 2799 */ 9999,
/* 2800 */ 758005,
/* 2801 */ 9999,
/* 2802 */ 758014,
/* 2803 */ 9999,
/* 2804 */ 759005,
/* 2805 */ 9999,
/* 2806 */ 759017,
/* 2807 */ 9999,
/* 2808 */ 9999,
/* 2809 */ 9999,
/* 2810 */ 734015,
/* 2811 */ 9999,
/* 2812 */ 9999,
/* 2813 */ 734015,
/* 2814 */ 9999,
/* 2815 */ 9999,
/* 2816 */ 735019,
/* 2817 */ 735026,
/* 2818 */ 736003,
/* 2819 */ 735013,
/* 2820 */ 9999,
/* 2821 */ 735013,
/* 2822 */ 9999,
/* 2823 */ 9999,
/* 2824 */ 735013,
/* 2825 */ 9999,
/* 2826 */ 9999,
/* 2827 */ 9999,
/* 2828 */ 735020,
/* 2829 */ 9999,
/* 2830 */ 9999,
/* 2831 */ 735020,
/* 2832 */ 9999,
/* 2833 */ 9999,
/* 2834 */ 9999,
/* 2835 */ 736004,
/* 2836 */ 9999,
/* 2837 */ 9999,
/* 2838 */ 736004,
/* 2839 */ 9999,
/* 2840 */ 9999,
/* 2841 */ 9999,
/* 2842 */ 737013,
/* 2843 */ 9999,
/* 2844 */ 9999,
/* 2845 */ 737013,
/* 2846 */ 9999,
/* 2847 */ 737020,
/* 2848 */ 737054,
/* 2849 */ 737034,
/* 2850 */ 9999,
/* 2851 */ 737034,
/* 2852 */ 9999,
/* 2853 */ 9999,
/* 2854 */ 737034,
/* 2855 */ 9999,
/* 2856 */ 9999,
/* 2857 */ 9999,
/* 2858 */ 737055,
/* 2859 */ 9999,
/* 2860 */ 9999,
/* 2861 */ 737055,
/* 2862 */ 9999,
/* 2863 */ 9999,
/* 2864 */ 9999,
/* 2865 */ 739014,
/* 2866 */ 9999,
/* 2867 */ 9999,
/* 2868 */ 739014,
/* 2869 */ 9999,
/* 2870 */ 9999,
/* 2871 */ 9999,
/* 2872 */ 740018,
/* 2873 */ 9999,
/* 2874 */ 9999,
/* 2875 */ 740018,
/* 2876 */ 9999,
/* 2877 */ 9999,
/* 2878 */ 9999,
/* 2879 */ 740023,
/* 2880 */ 9999,
/* 2881 */ 9999,
/* 2882 */ 740023,
/* 2883 */ 9999,
/* 2884 */ 9999,
/* 2885 */ 9999,
/* 2886 */ 743018,
/* 2887 */ 9999,
/* 2888 */ 9999,
/* 2889 */ 743018,
/* 2890 */ 9999,
/* 2891 */ 9999,
/* 2892 */ 9999,
/* 2893 */ 743023,
/* 2894 */ 9999,
/* 2895 */ 9999,
/* 2896 */ 743023,
/* 2897 */ 9999,
/* 2898 */ 9999,
/* 2899 */ 9999,
/* 2900 */ 747013,
/* 2901 */ 9999,
/* 2902 */ 9999,
/* 2903 */ 747013,
/* 2904 */ 9999,
/* 2905 */ 748009,
/* 2906 */ 748059,
/* 2907 */ 748014,
/* 2908 */ 9999,
/* 2909 */ 748014,
/* 2910 */ 9999,
/* 2911 */ 9999,
/* 2912 */ 748014,
/* 2913 */ 9999,
/* 2914 */ 9999,
/* 2915 */ 9999,
/* 2916 */ 748060,
/* 2917 */ 9999,
/* 2918 */ 9999,
/* 2919 */ 748060,
/* 2920 */ 9999,
/* 2921 */ 9999,
/* 2922 */ 9999,
/* 2923 */ 749016,
/* 2924 */ 9999,
/* 2925 */ 9999,
/* 2926 */ 749016,
/* 2927 */ 9999,
/* 2928 */ 9999,
/* 2929 */ 9999,
/* 2930 */ 755012,
/* 2931 */ 9999,
/* 2932 */ 9999,
/* 2933 */ 755012,
/* 2934 */ 9999,
/* 2935 */ 755020,
/* 2936 */ 755030,
/* 2937 */ 755025,
/* 2938 */ 9999,
/* 2939 */ 755025,
/* 2940 */ 9999,
/* 2941 */ 9999,
/* 2942 */ 755025,
/* 2943 */ 9999,
/* 2944 */ 9999,
/* 2945 */ 9999,
/* 2946 */ 755031,
/* 2947 */ 9999,
/* 2948 */ 9999,
/* 2949 */ 9999,
/* 2950 */ 755037,
/* 2951 */ 9999,
/* 2952 */ 9999,
/* 2953 */ 755037,
/* 2954 */ 9999,
/* 2955 */ 9999,
/* 2956 */ 9999,
/* 2957 */ 9999,
/* 2958 */ 764007,
/* 2959 */ 9999,
/* 2960 */ 9999,
/* 2961 */ 763007,
/* 2962 */ 9999,
/* 2963 */ 764008,
/* 2964 */ 764018,
/* 2965 */ 9999,
/* 2966 */ 764018,
/* 2967 */ 9999,
/* 2968 */ 9999,
/* 2969 */ 764018,
/* 2970 */ 9999,
/* 2971 */ 9999,
/* 2972 */ 768015,
/* 2973 */ 9999,
/* 2974 */ 9999,
/* 2975 */ 9999,
/* 2976 */ 768013,
/* 2977 */ 9999,
/* 2978 */ 9999,
/* 2979 */ 769013,
/* 2980 */ 769020,
/* 2981 */ 769027,
/* 2982 */ 9999,
/* 2983 */ 9999,
/* 2984 */ 769011,
/* 2985 */ 9999,
/* 2986 */ 9999,
/* 2987 */ 770010,
/* 2988 */ 770017,
/* 2989 */ 770024,
/* 2990 */ 9999,
/* 2991 */ 9999,
/* 2992 */ 770008,
/* 2993 */ 9999,
/* 2994 */ 9999,
/* 2995 */ 771009,
/* 2996 */ 771016,
/* 2997 */ 9999,
/* 2998 */ 771030,
/* 2999 */ 9999,
/* 3000 */ 9999,
/* 3001 */ 771007,
/* 3002 */ 9999,
/* 3003 */ 9999,
/* 3004 */ 773012,
/* 3005 */ 774007,
/* 3006 */ 9999,
/* 3007 */ 9999,
/* 3008 */ 773010,
/* 3009 */ 9999,
/* 3010 */ 9999,
/* 3011 */ 775011,
/* 3012 */ 776007,
/* 3013 */ 9999,
/* 3014 */ 9999,
/* 3015 */ 775009,
/* 3016 */ 9999,
/* 3017 */ 9999,
/* 3018 */ 777012,
/* 3019 */ 777019,
/* 3020 */ 9999,
/* 3021 */ 9999,
/* 3022 */ 777010,
/* 3023 */ 9999,
/* 3024 */ 9999,
/* 3025 */ 779012,
/* 3026 */ 779018,
/* 3027 */ 779029,
/* 3028 */ 780007,
/* 3029 */ 780014,
/* 3030 */ 9999,
/* 3031 */ 9999,
/* 3032 */ 779010,
/* 3033 */ 9999,
/* 3034 */ 9999,
/* 3035 */ 781019,
/* 3036 */ 782007,
/* 3037 */ 9999,
/* 3038 */ 9999,
/* 3039 */ 781017,
/* 3040 */ 9999,
/* 3041 */ 9999,
/* 3042 */ 783011,
/* 3043 */ 783019,
/* 3044 */ 9999,
/* 3045 */ 9999,
/* 3046 */ 783009,
/* 3047 */ 9999,
/* 3048 */ 784005,
/* 3049 */ 9999,
/* 3050 */ 784013,
/* 3051 */ 9999,
/* 3052 */ 785005,
/* 3053 */ 9999,
/* 3054 */ 785014,
/* 3055 */ 9999,
/* 3056 */ 786005,
/* 3057 */ 9999,
/* 3058 */ 786016,
/* 3059 */ 9999,
/* 3060 */ 787005,
/* 3061 */ 9999,
/* 3062 */ 787017,
/* 3063 */ 9999,
/* 3064 */ 9999,
/* 3065 */ 9999,
/* 3066 */ 768016,
/* 3067 */ 9999,
/* 3068 */ 9999,
/* 3069 */ 768016,
/* 3070 */ 9999,
/* 3071 */ 9999,
/* 3072 */ 9999,
/* 3073 */ 769014,
/* 3074 */ 9999,
/* 3075 */ 9999,
/* 3076 */ 769014,
/* 3077 */ 9999,
/* 3078 */ 9999,
/* 3079 */ 9999,
/* 3080 */ 769021,
/* 3081 */ 9999,
/* 3082 */ 9999,
/* 3083 */ 769021,
/* 3084 */ 9999,
/* 3085 */ 9999,
/* 3086 */ 9999,
/* 3087 */ 770011,
/* 3088 */ 9999,
/* 3089 */ 9999,
/* 3090 */ 770011,
/* 3091 */ 9999,
/* 3092 */ 9999,
/* 3093 */ 9999,
/* 3094 */ 770018,
/* 3095 */ 9999,
/* 3096 */ 9999,
/* 3097 */ 770018,
/* 3098 */ 9999,
/* 3099 */ 9999,
/* 3100 */ 9999,
/* 3101 */ 771010,
/* 3102 */ 9999,
/* 3103 */ 9999,
/* 3104 */ 771010,
/* 3105 */ 9999,
/* 3106 */ 9999,
/* 3107 */ 9999,
/* 3108 */ 773013,
/* 3109 */ 9999,
/* 3110 */ 9999,
/* 3111 */ 773013,
/* 3112 */ 9999,
/* 3113 */ 774008,
/* 3114 */ 774020,
/* 3115 */ 774069,
/* 3116 */ 774013,
/* 3117 */ 9999,
/* 3118 */ 774013,
/* 3119 */ 9999,
/* 3120 */ 9999,
/* 3121 */ 774013,
/* 3122 */ 9999,
/* 3123 */ 9999,
/* 3124 */ 9999,
/* 3125 */ 774070,
/* 3126 */ 9999,
/* 3127 */ 9999,
/* 3128 */ 774070,
/* 3129 */ 9999,
/* 3130 */ 9999,
/* 3131 */ 9999,
/* 3132 */ 775012,
/* 3133 */ 9999,
/* 3134 */ 9999,
/* 3135 */ 775012,
/* 3136 */ 9999,
/* 3137 */ 776008,
/* 3138 */ 776020,
/* 3139 */ 776067,
/* 3140 */ 776013,
/* 3141 */ 9999,
/* 3142 */ 776013,
/* 3143 */ 9999,
/* 3144 */ 9999,
/* 3145 */ 776013,
/* 3146 */ 9999,
/* 3147 */ 9999,
/* 3148 */ 9999,
/* 3149 */ 776068,
/* 3150 */ 9999,
/* 3151 */ 9999,
/* 3152 */ 776068,
/* 3153 */ 9999,
/* 3154 */ 9999,
/* 3155 */ 9999,
/* 3156 */ 777013,
/* 3157 */ 9999,
/* 3158 */ 9999,
/* 3159 */ 777013,
/* 3160 */ 9999,
/* 3161 */ 777020,
/* 3162 */ 777054,
/* 3163 */ 777034,
/* 3164 */ 9999,
/* 3165 */ 777034,
/* 3166 */ 9999,
/* 3167 */ 9999,
/* 3168 */ 777034,
/* 3169 */ 9999,
/* 3170 */ 9999,
/* 3171 */ 9999,
/* 3172 */ 777055,
/* 3173 */ 9999,
/* 3174 */ 9999,
/* 3175 */ 777055,
/* 3176 */ 9999,
/* 3177 */ 9999,
/* 3178 */ 9999,
/* 3179 */ 779013,
/* 3180 */ 9999,
/* 3181 */ 9999,
/* 3182 */ 9999,
/* 3183 */ 779030,
/* 3184 */ 9999,
/* 3185 */ 9999,
/* 3186 */ 9999,
/* 3187 */ 780008,
/* 3188 */ 9999,
/* 3189 */ 9999,
/* 3190 */ 780008,
/* 3191 */ 9999,
/* 3192 */ 780016,
/* 3193 */ 780068,
/* 3194 */ 780021,
/* 3195 */ 9999,
/* 3196 */ 780021,
/* 3197 */ 9999,
/* 3198 */ 9999,
/* 3199 */ 780021,
/* 3200 */ 9999,
/* 3201 */ 9999,
/* 3202 */ 9999,
/* 3203 */ 780069,
/* 3204 */ 9999,
/* 3205 */ 9999,
/* 3206 */ 780069,
/* 3207 */ 9999,
/* 3208 */ 9999,
/* 3209 */ 9999,
/* 3210 */ 781020,
/* 3211 */ 9999,
/* 3212 */ 9999,
/* 3213 */ 781020,
/* 3214 */ 9999,
/* 3215 */ 782008,
/* 3216 */ 782054,
/* 3217 */ 782013,
/* 3218 */ 9999,
/* 3219 */ 782013,
/* 3220 */ 9999,
/* 3221 */ 9999,
/* 3222 */ 782013,
/* 3223 */ 9999,
/* 3224 */ 9999,
/* 3225 */ 9999,
/* 3226 */ 782055,
/* 3227 */ 9999,
/* 3228 */ 9999,
/* 3229 */ 782055,
/* 3230 */ 9999,
/* 3231 */ 9999,
/* 3232 */ 9999,
/* 3233 */ 783012,
/* 3234 */ 9999,
/* 3235 */ 9999,
/* 3236 */ 783012,
/* 3237 */ 9999,
/* 3238 */ 783020,
/* 3239 */ 783030,
/* 3240 */ 783025,
/* 3241 */ 9999,
/* 3242 */ 783025,
/* 3243 */ 9999,
/* 3244 */ 9999,
/* 3245 */ 783025,
/* 3246 */ 9999,
/* 3247 */ 9999,
/* 3248 */ 9999,
/* 3249 */ 783031,
/* 3250 */ 9999,
/* 3251 */ 9999,
/* 3252 */ 9999,
/* 3253 */ 783037,
/* 3254 */ 9999,
/* 3255 */ 9999,
/* 3256 */ 783037,
/* 3257 */ 9999,
/* 3258 */ 9999,
/* 3259 */ 9999,
/* 3260 */ 791027,
/* 3261 */ 791038,
/* 3262 */ 9999,
/* 3263 */ 9999,
/* 3264 */ 791076,
/* 3265 */ 9999,
/* 3266 */ 791003,
/* 3267 */ 9999,
/* 3268 */ 9999,
/* 3269 */ 9999,
/* 3270 */ 793029,
/* 3271 */ 793040,
/* 3272 */ 9999,
/* 3273 */ 9999,
/* 3274 */ 793078,
/* 3275 */ 9999,
/* 3276 */ 793005,
/* 3277 */ 9999,
/* 3278 */ 9999,
/* 3279 */ 9999,
/* 3280 */ 795029,
/* 3281 */ 795040,
/* 3282 */ 9999,
/* 3283 */ 9999,
/* 3284 */ 795078,
/* 3285 */ 9999,
/* 3286 */ 795005,
/* 3287 */ 9999,
/* 3288 */ 791039,
/* 3289 */ 9999,
/* 3290 */ 791048,
/* 3291 */ 9999,
/* 3292 */ 9999,
/* 3293 */ 791048,
/* 3294 */ 9999,
/* 3295 */ 793041,
/* 3296 */ 9999,
/* 3297 */ 793050,
/* 3298 */ 9999,
/* 3299 */ 9999,
/* 3300 */ 793050,
/* 3301 */ 9999,
/* 3302 */ 795041,
/* 3303 */ 9999,
/* 3304 */ 795050,
/* 3305 */ 9999,
/* 3306 */ 9999,
/* 3307 */ 795050,
/* 3308 */ 9999,
/* 3309 */ 9999,
/* 3310 */ 800007,
/* 3311 */ 9999,
/* 3312 */ 800003,
/* 3313 */ 9999,
/* 3314 */ 9999,
/* 3315 */ 801009,
/* 3316 */ 9999,
/* 3317 */ 801005,
/* 3318 */ 9999,
/* 3319 */ 9999,
/* 3320 */ 802009,
/* 3321 */ 9999,
/* 3322 */ 802024,
/* 3323 */ 9999,
/* 3324 */ 802005,
/* 3325 */ 9999,
/* 3326 */ 806003,
/* 3327 */ 806015,
/* 3328 */ 9999,
/* 3329 */ 806008,
/* 3330 */ 9999,
/* 3331 */ 9999,
/* 3332 */ 808011,
/* 3333 */ 9999,
/* 3334 */ 9999,
/* 3335 */ 808009,
/* 3336 */ 9999,
/* 3337 */ 9999,
/* 3338 */ 9999,
/* 3339 */ 9999,
/* 3340 */ 810009,
/* 3341 */ 9999,
/* 3342 */ 9999,
/* 3343 */ 9999,
/* 3344 */ 9999,
/* 3345 */ 9999,
/* 3346 */ 812034,
/* 3347 */ 812046,
/* 3348 */ 9999,
/* 3349 */ 812009,
/* 3350 */ 9999,
/* 3351 */ 9999,
/* 3352 */ 814011,
/* 3353 */ 9999,
/* 3354 */ 9999,
/* 3355 */ 9999,
/* 3356 */ 814046,
/* 3357 */ 814058,
/* 3358 */ 9999,
/* 3359 */ 814009,
/* 3360 */ 9999,
/* 3361 */ 9999,
/* 3362 */ 9999,
/* 3363 */ 806025,
/* 3364 */ 9999,
/* 3365 */ 9999,
/* 3366 */ 806025,
/* 3367 */ 9999,
/* 3368 */ 9999,
/* 3369 */ 9999,
/* 3370 */ 812056,
/* 3371 */ 9999,
/* 3372 */ 9999,
/* 3373 */ 812056,
/* 3374 */ 9999,
/* 3375 */ 9999,
/* 3376 */ 9999,
/* 3377 */ 814068,
/* 3378 */ 9999,
/* 3379 */ 9999,
/* 3380 */ 814068,
/* 3381 */ 9999,
/* 3382 */ 9999,
/* 3383 */ 9999,
/* 3384 */ 9999,
/* 3385 */ 820007,
/* 3386 */ 9999,
/* 3387 */ 9999,
/* 3388 */ 819009,
/* 3389 */ 9999,
/* 3390 */ 820008,
/* 3391 */ 820016,
/* 3392 */ 9999,
/* 3393 */ 820016,
/* 3394 */ 9999,
/* 3395 */ 9999,
/* 3396 */ 820016,
/* 3397 */ 9999,
/* 3398 */ 9999,
/* 3399 */ 824015,
/* 3400 */ 9999,
/* 3401 */ 9999,
/* 3402 */ 9999,
/* 3403 */ 824013,
/* 3404 */ 9999,
/* 3405 */ 825005,
/* 3406 */ 9999,
/* 3407 */ 825016,
/* 3408 */ 9999,
/* 3409 */ 826005,
/* 3410 */ 9999,
/* 3411 */ 826014,
/* 3412 */ 9999,
/* 3413 */ 827005,
/* 3414 */ 9999,
/* 3415 */ 827017,
/* 3416 */ 9999,
/* 3417 */ 9999,
/* 3418 */ 9999,
/* 3419 */ 824016,
/* 3420 */ 9999,
/* 3421 */ 9999,
/* 3422 */ 824016,
/* 3423 */ 9999,
/* 3424 */ 9999,
/* 3425 */ 831013,
/* 3426 */ 832005,
/* 3427 */ 9999,
/* 3428 */ 9999,
/* 3429 */ 831011,
/* 3430 */ 9999,
/* 3431 */ 9999,
/* 3432 */ 835011,
/* 3433 */ 836005,
/* 3434 */ 9999,
/* 3435 */ 835009,
/* 3436 */ 9999,
/* 3437 */ 9999,
/* 3438 */ 843014,
/* 3439 */ 844005,
/* 3440 */ 9999,
/* 3441 */ 9999,
/* 3442 */ 843012,
/* 3443 */ 9999,
/* 3444 */ 9999,
/* 3445 */ 9999,
/* 3446 */ 831014,
/* 3447 */ 9999,
/* 3448 */ 9999,
/* 3449 */ 831014,
/* 3450 */ 9999,
/* 3451 */ 9999,
/* 3452 */ 832011,
/* 3453 */ 832023,
/* 3454 */ 832077,
/* 3455 */ 832007,
/* 3456 */ 9999,
/* 3457 */ 832007,
/* 3458 */ 9999,
/* 3459 */ 9999,
/* 3460 */ 833011,
/* 3461 */ 833023,
/* 3462 */ 833076,
/* 3463 */ 833007,
/* 3464 */ 9999,
/* 3465 */ 833007,
/* 3466 */ 9999,
/* 3467 */ 9999,
/* 3468 */ 833007,
/* 3469 */ 9999,
/* 3470 */ 9999,
/* 3471 */ 9999,
/* 3472 */ 832078,
/* 3473 */ 9999,
/* 3474 */ 9999,
/* 3475 */ 832078,
/* 3476 */ 9999,
/* 3477 */ 9999,
/* 3478 */ 9999,
/* 3479 */ 833077,
/* 3480 */ 9999,
/* 3481 */ 9999,
/* 3482 */ 833077,
/* 3483 */ 9999,
/* 3484 */ 9999,
/* 3485 */ 9999,
/* 3486 */ 835012,
/* 3487 */ 9999,
/* 3488 */ 9999,
/* 3489 */ 835012,
/* 3490 */ 9999,
/* 3491 */ 9999,
/* 3492 */ 836011,
/* 3493 */ 836044,
/* 3494 */ 836058,
/* 3495 */ 836007,
/* 3496 */ 9999,
/* 3497 */ 836007,
/* 3498 */ 9999,
/* 3499 */ 9999,
/* 3500 */ 836007,
/* 3501 */ 9999,
/* 3502 */ 836045,
/* 3503 */ 836054,
/* 3504 */ 9999,
/* 3505 */ 836054,
/* 3506 */ 9999,
/* 3507 */ 9999,
/* 3508 */ 836054,
/* 3509 */ 9999,
/* 3510 */ 9999,
/* 3511 */ 9999,
/* 3512 */ 836059,
/* 3513 */ 9999,
/* 3514 */ 9999,
/* 3515 */ 9999,
/* 3516 */ 836063,
/* 3517 */ 9999,
/* 3518 */ 9999,
/* 3519 */ 9999,
/* 3520 */ 843015,
/* 3521 */ 9999,
/* 3522 */ 9999,
/* 3523 */ 843015,
/* 3524 */ 9999,
/* 3525 */ 9999,
/* 3526 */ 844035,
/* 3527 */ 845007,
/* 3528 */ 848011,
/* 3529 */ 844007,
/* 3530 */ 9999,
/* 3531 */ 844007,
/* 3532 */ 9999,
/* 3533 */ 9999,
/* 3534 */ 844007,
/* 3535 */ 9999,
/* 3536 */ 844037,
/* 3537 */ 9999,
/* 3538 */ 9999,
/* 3539 */ 844042,
/* 3540 */ 9999,
/* 3541 */ 9999,
/* 3542 */ 844042,
/* 3543 */ 9999,
/* 3544 */ 845011,
/* 3545 */ 9999,
/* 3546 */ 9999,
/* 3547 */ 845037,
/* 3548 */ 9999,
/* 3549 */ 845017,
/* 3550 */ 9999,
/* 3551 */ 846011,
/* 3552 */ 9999,
/* 3553 */ 846026,
/* 3554 */ 9999,
/* 3555 */ 9999,
/* 3556 */ 846017,
/* 3557 */ 9999,
/* 3558 */ 847011,
/* 3559 */ 9999,
/* 3560 */ 9999,
/* 3561 */ 847037,
/* 3562 */ 9999,
/* 3563 */ 847017,
/* 3564 */ 9999,
/* 3565 */ 9999,
/* 3566 */ 9999,
/* 3567 */ 848012,
/* 3568 */ 9999,
/* 3569 */ 9999,
/* 3570 */ 848012,
/* 3571 */ 9999,
/* 3572 */ 852005,
/* 3573 */ 9999,
/* 3574 */ 852011,
/* 3575 */ 9999,
/* 3576 */ 853005,
/* 3577 */ 9999,
/* 3578 */ 853010,
/* 3579 */ 9999,
/* 3580 */ 9999,
/* 3581 */ 9999,
/* 3582 */ 854011,
/* 3583 */ 9999,
/* 3584 */ 9999,
/* 3585 */ 9999,
/* 3586 */ 9999,
/* 3587 */ 859007,
/* 3588 */ 9999,
/* 3589 */ 9999,
/* 3590 */ 858010,
/* 3591 */ 9999,
/* 3592 */ 859008,
/* 3593 */ 859017,
/* 3594 */ 9999,
/* 3595 */ 859017,
/* 3596 */ 9999,
/* 3597 */ 9999,
/* 3598 */ 859017,
/* 3599 */ 9999,
/* 3600 */ 9999,
/* 3601 */ 863015,
/* 3602 */ 9999,
/* 3603 */ 9999,
/* 3604 */ 9999,
/* 3605 */ 863013,
/* 3606 */ 9999,
/* 3607 */ 9999,
/* 3608 */ 864010,
/* 3609 */ 864017,
/* 3610 */ 9999,
/* 3611 */ 9999,
/* 3612 */ 864008,
/* 3613 */ 9999,
/* 3614 */ 865005,
/* 3615 */ 9999,
/* 3616 */ 9999,
/* 3617 */ 865005,
/* 3618 */ 9999,
/* 3619 */ 9999,
/* 3620 */ 866013,
/* 3621 */ 866020,
/* 3622 */ 9999,
/* 3623 */ 9999,
/* 3624 */ 866011,
/* 3625 */ 9999,
/* 3626 */ 9999,
/* 3627 */ 867014,
/* 3628 */ 867021,
/* 3629 */ 9999,
/* 3630 */ 9999,
/* 3631 */ 867012,
/* 3632 */ 9999,
/* 3633 */ 9999,
/* 3634 */ 868015,
/* 3635 */ 868022,
/* 3636 */ 9999,
/* 3637 */ 9999,
/* 3638 */ 868013,
/* 3639 */ 9999,
/* 3640 */ 9999,
/* 3641 */ 9999,
/* 3642 */ 870049,
/* 3643 */ 9999,
/* 3644 */ 9999,
/* 3645 */ 870009,
/* 3646 */ 9999,
/* 3647 */ 9999,
/* 3648 */ 9999,
/* 3649 */ 871049,
/* 3650 */ 9999,
/* 3651 */ 9999,
/* 3652 */ 871009,
/* 3653 */ 9999,
/* 3654 */ 9999,
/* 3655 */ 9999,
/* 3656 */ 872049,
/* 3657 */ 9999,
/* 3658 */ 9999,
/* 3659 */ 872007,
/* 3660 */ 9999,
/* 3661 */ 9999,
/* 3662 */ 9999,
/* 3663 */ 873049,
/* 3664 */ 9999,
/* 3665 */ 9999,
/* 3666 */ 873010,
/* 3667 */ 9999,
/* 3668 */ 874005,
/* 3669 */ 9999,
/* 3670 */ 874014,
/* 3671 */ 9999,
/* 3672 */ 875005,
/* 3673 */ 9999,
/* 3674 */ 875017,
/* 3675 */ 9999,
/* 3676 */ 9999,
/* 3677 */ 9999,
/* 3678 */ 863016,
/* 3679 */ 9999,
/* 3680 */ 9999,
/* 3681 */ 863016,
/* 3682 */ 9999,
/* 3683 */ 9999,
/* 3684 */ 9999,
/* 3685 */ 864011,
/* 3686 */ 9999,
/* 3687 */ 9999,
/* 3688 */ 864011,
/* 3689 */ 9999,
/* 3690 */ 9999,
/* 3691 */ 865020,
/* 3692 */ 865027,
/* 3693 */ 865076,
/* 3694 */ 865014,
/* 3695 */ 9999,
/* 3696 */ 865014,
/* 3697 */ 9999,
/* 3698 */ 9999,
/* 3699 */ 865014,
/* 3700 */ 9999,
/* 3701 */ 9999,
/* 3702 */ 9999,
/* 3703 */ 865021,
/* 3704 */ 9999,
/* 3705 */ 9999,
/* 3706 */ 865021,
/* 3707 */ 9999,
/* 3708 */ 9999,
/* 3709 */ 9999,
/* 3710 */ 865077,
/* 3711 */ 9999,
/* 3712 */ 9999,
/* 3713 */ 865077,
/* 3714 */ 9999,
/* 3715 */ 9999,
/* 3716 */ 9999,
/* 3717 */ 866014,
/* 3718 */ 9999,
/* 3719 */ 9999,
/* 3720 */ 866014,
/* 3721 */ 9999,
/* 3722 */ 866021,
/* 3723 */ 866062,
/* 3724 */ 866026,
/* 3725 */ 9999,
/* 3726 */ 866026,
/* 3727 */ 9999,
/* 3728 */ 9999,
/* 3729 */ 866026,
/* 3730 */ 9999,
/* 3731 */ 9999,
/* 3732 */ 9999,
/* 3733 */ 866063,
/* 3734 */ 9999,
/* 3735 */ 9999,
/* 3736 */ 866063,
/* 3737 */ 9999,
/* 3738 */ 9999,
/* 3739 */ 9999,
/* 3740 */ 867015,
/* 3741 */ 9999,
/* 3742 */ 9999,
/* 3743 */ 867015,
/* 3744 */ 9999,
/* 3745 */ 9999,
/* 3746 */ 867074,
/* 3747 */ 867030,
/* 3748 */ 9999,
/* 3749 */ 867030,
/* 3750 */ 9999,
/* 3751 */ 9999,
/* 3752 */ 867030,
/* 3753 */ 9999,
/* 3754 */ 9999,
/* 3755 */ 9999,
/* 3756 */ 867075,
/* 3757 */ 9999,
/* 3758 */ 9999,
/* 3759 */ 867075,
/* 3760 */ 9999,
/* 3761 */ 9999,
/* 3762 */ 9999,
/* 3763 */ 868016,
/* 3764 */ 9999,
/* 3765 */ 9999,
/* 3766 */ 868016,
/* 3767 */ 9999,
/* 3768 */ 868023,
/* 3769 */ 868064,
/* 3770 */ 868028,
/* 3771 */ 9999,
/* 3772 */ 868028,
/* 3773 */ 9999,
/* 3774 */ 9999,
/* 3775 */ 868028,
/* 3776 */ 9999,
/* 3777 */ 9999,
/* 3778 */ 9999,
/* 3779 */ 868065,
/* 3780 */ 9999,
/* 3781 */ 9999,
/* 3782 */ 868065,
/* 3783 */ 9999,
/* 3784 */ 870050,
/* 3785 */ 870060,
/* 3786 */ 870058,
/* 3787 */ 9999,
/* 3788 */ 870058,
/* 3789 */ 9999,
/* 3790 */ 9999,
/* 3791 */ 870058,
/* 3792 */ 9999,
/* 3793 */ 9999,
/* 3794 */ 9999,
/* 3795 */ 870061,
/* 3796 */ 9999,
/* 3797 */ 871050,
/* 3798 */ 871060,
/* 3799 */ 871058,
/* 3800 */ 9999,
/* 3801 */ 871058,
/* 3802 */ 9999,
/* 3803 */ 9999,
/* 3804 */ 871058,
/* 3805 */ 9999,
/* 3806 */ 9999,
/* 3807 */ 9999,
/* 3808 */ 871061,
/* 3809 */ 9999,
/* 3810 */ 872050,
/* 3811 */ 872060,
/* 3812 */ 872058,
/* 3813 */ 9999,
/* 3814 */ 872058,
/* 3815 */ 9999,
/* 3816 */ 9999,
/* 3817 */ 872058,
/* 3818 */ 9999,
/* 3819 */ 9999,
/* 3820 */ 9999,
/* 3821 */ 872061,
/* 3822 */ 9999,
/* 3823 */ 873050,
/* 3824 */ 873060,
/* 3825 */ 873058,
/* 3826 */ 9999,
/* 3827 */ 873058,
/* 3828 */ 9999,
/* 3829 */ 9999,
/* 3830 */ 873058,
/* 3831 */ 9999,
/* 3832 */ 9999,
/* 3833 */ 9999,
/* 3834 */ 873061,
/* 3835 */ 9999,
/* 3836 */ 9999,
/* 3837 */ 9999,
/* 3838 */ 879015,
/* 3839 */ 9999,
/* 3840 */ 879030,
/* 3841 */ 9999,
/* 3842 */ 9999,
/* 3843 */ 879009,
/* 3844 */ 9999,
/* 3845 */ 9999,
/* 3846 */ 9999,
/* 3847 */ 881015,
/* 3848 */ 9999,
/* 3849 */ 9999,
/* 3850 */ 881009,
/* 3851 */ 9999,
/* 3852 */ 883005,
/* 3853 */ 883017,
/* 3854 */ 9999,
/* 3855 */ 883011,
/* 3856 */ 9999,
/* 3857 */ 885005,
/* 3858 */ 9999,
/* 3859 */ 885023,
/* 3860 */ 9999,
/* 3861 */ 9999,
/* 3862 */ 885011,
/* 3863 */ 9999,
/* 3864 */ 887005,
/* 3865 */ 887017,
/* 3866 */ 887027,
/* 3867 */ 9999,
/* 3868 */ 9999,
/* 3869 */ 887011,
/* 3870 */ 9999,
/* 3871 */ 890005,
/* 3872 */ 890017,
/* 3873 */ 890027,
/* 3874 */ 9999,
/* 3875 */ 9999,
/* 3876 */ 890011,
/* 3877 */ 9999,
/* 3878 */ 9999,
/* 3879 */ 893012,
/* 3880 */ 9999,
/* 3881 */ 893010,
/* 3882 */ 9999,
/* 3883 */ 9999,
/* 3884 */ 895010,
/* 3885 */ 9999,
/* 3886 */ 895008,
/* 3887 */ 9999,
/* 3888 */ 9999,
/* 3889 */ 9999,
/* 3890 */ 9999,
/* 3891 */ 883019,
/* 3892 */ 9999,
/* 3893 */ 9999,
/* 3894 */ 883019,
/* 3895 */ 9999,
/* 3896 */ 9999,
/* 3897 */ 9999,
/* 3898 */ 887028,
/* 3899 */ 9999,
/* 3900 */ 9999,
/* 3901 */ 9999,
/* 3902 */ 887034,
/* 3903 */ 9999,
/* 3904 */ 9999,
/* 3905 */ 9999,
/* 3906 */ 890028,
/* 3907 */ 9999,
/* 3908 */ 9999,
/* 3909 */ 9999,
/* 3910 */ 890034,
/* 3911 */ 9999,
/* 3912 */ 9999,
/* 3913 */ 9999,
/* 3914 */ 9999,
/* 3915 */ 899038,
/* 3916 */ 9999,
/* 3917 */ 901007,
/* 3918 */ 9999,
/* 3919 */ 9999,
/* 3920 */ 899018,
/* 3921 */ 9999,
/* 3922 */ 901008,
/* 3923 */ 901019,
/* 3924 */ 9999,
/* 3925 */ 901019,
/* 3926 */ 9999,
/* 3927 */ 9999,
/* 3928 */ 901019,
/* 3929 */ 9999,
/* 3930 */ 9999,
/* 3931 */ 907010,
/* 3932 */ 907017,
/* 3933 */ 9999,
/* 3934 */ 9999,
/* 3935 */ 906003,
/* 3936 */ 9999,
/* 3937 */ 9999,
/* 3938 */ 911012,
/* 3939 */ 911019,
/* 3940 */ 9999,
/* 3941 */ 9999,
/* 3942 */ 911010,
/* 3943 */ 9999,
/* 3944 */ 9999,
/* 3945 */ 913015,
/* 3946 */ 9999,
/* 3947 */ 9999,
/* 3948 */ 9999,
/* 3949 */ 913013,
/* 3950 */ 9999,
/* 3951 */ 914005,
/* 3952 */ 9999,
/* 3953 */ 914014,
/* 3954 */ 9999,
/* 3955 */ 915005,
/* 3956 */ 9999,
/* 3957 */ 915016,
/* 3958 */ 9999,
/* 3959 */ 916005,
/* 3960 */ 9999,
/* 3961 */ 916017,
/* 3962 */ 9999,
/* 3963 */ 9999,
/* 3964 */ 9999,
/* 3965 */ 907011,
/* 3966 */ 9999,
/* 3967 */ 9999,
/* 3968 */ 907011,
/* 3969 */ 9999,
/* 3970 */ 907018,
/* 3971 */ 907072,
/* 3972 */ 907024,
/* 3973 */ 9999,
/* 3974 */ 907024,
/* 3975 */ 9999,
/* 3976 */ 9999,
/* 3977 */ 907024,
/* 3978 */ 9999,
/* 3979 */ 9999,
/* 3980 */ 9999,
/* 3981 */ 907073,
/* 3982 */ 9999,
/* 3983 */ 9999,
/* 3984 */ 907073,
/* 3985 */ 9999,
/* 3986 */ 9999,
/* 3987 */ 9999,
/* 3988 */ 911013,
/* 3989 */ 9999,
/* 3990 */ 9999,
/* 3991 */ 911013,
/* 3992 */ 9999,
/* 3993 */ 911020,
/* 3994 */ 911054,
/* 3995 */ 911034,
/* 3996 */ 9999,
/* 3997 */ 911034,
/* 3998 */ 9999,
/* 3999 */ 9999,
/* 4000 */ 911034,
/* 4001 */ 9999,
/* 4002 */ 9999,
/* 4003 */ 9999,
/* 4004 */ 911055,
/* 4005 */ 9999,
/* 4006 */ 9999,
/* 4007 */ 911055,
/* 4008 */ 9999,
/* 4009 */ 9999,
/* 4010 */ 9999,
/* 4011 */ 913016,
/* 4012 */ 9999,
/* 4013 */ 9999,
/* 4014 */ 913016,
/* 4015 */ 9999,
/* 4016 */ 9999,
/* 4017 */ 9999,
/* 4018 */ 9999,
/* 4019 */ 920007,
/* 4020 */ 9999,
/* 4021 */ 9999,
/* 4022 */ 919012,
/* 4023 */ 9999,
/* 4024 */ 920008,
/* 4025 */ 920016,
/* 4026 */ 9999,
/* 4027 */ 920016,
/* 4028 */ 9999,
/* 4029 */ 9999,
/* 4030 */ 920016,
/* 4031 */ 9999,
/* 4032 */ 9999,
/* 4033 */ 926011,
/* 4034 */ 926018,
/* 4035 */ 9999,
/* 4036 */ 9999,
/* 4037 */ 925003,
/* 4038 */ 9999,
/* 4039 */ 9999,
/* 4040 */ 9999,
/* 4041 */ 9999,
/* 4042 */ 930013,
/* 4043 */ 9999,
/* 4044 */ 9999,
/* 4045 */ 931011,
/* 4046 */ 931018,
/* 4047 */ 9999,
/* 4048 */ 9999,
/* 4049 */ 931009,
/* 4050 */ 9999,
/* 4051 */ 9999,
/* 4052 */ 932015,
/* 4053 */ 9999,
/* 4054 */ 9999,
/* 4055 */ 9999,
/* 4056 */ 932013,
/* 4057 */ 9999,
/* 4058 */ 9999,
/* 4059 */ 933012,
/* 4060 */ 933019,
/* 4061 */ 9999,
/* 4062 */ 9999,
/* 4063 */ 933010,
/* 4064 */ 9999,
/* 4065 */ 935005,
/* 4066 */ 9999,
/* 4067 */ 935014,
/* 4068 */ 9999,
/* 4069 */ 936005,
/* 4070 */ 9999,
/* 4071 */ 936016,
/* 4072 */ 9999,
/* 4073 */ 937005,
/* 4074 */ 9999,
/* 4075 */ 937017,
/* 4076 */ 9999,
/* 4077 */ 9999,
/* 4078 */ 9999,
/* 4079 */ 926012,
/* 4080 */ 9999,
/* 4081 */ 9999,
/* 4082 */ 926012,
/* 4083 */ 9999,
/* 4084 */ 926019,
/* 4085 */ 926074,
/* 4086 */ 926025,
/* 4087 */ 9999,
/* 4088 */ 926025,
/* 4089 */ 9999,
/* 4090 */ 9999,
/* 4091 */ 926025,
/* 4092 */ 9999,
/* 4093 */ 9999,
/* 4094 */ 9999,
/* 4095 */ 926075,
/* 4096 */ 9999,
/* 4097 */ 9999,
/* 4098 */ 926075,
/* 4099 */ 9999,
/* 4100 */ 9999,
/* 4101 */ 9999,
/* 4102 */ 931012,
/* 4103 */ 9999,
/* 4104 */ 9999,
/* 4105 */ 931012,
/* 4106 */ 9999,
/* 4107 */ 931019,
/* 4108 */ 931029,
/* 4109 */ 931024,
/* 4110 */ 9999,
/* 4111 */ 931024,
/* 4112 */ 9999,
/* 4113 */ 9999,
/* 4114 */ 931024,
/* 4115 */ 9999,
/* 4116 */ 9999,
/* 4117 */ 9999,
/* 4118 */ 931030,
/* 4119 */ 9999,
/* 4120 */ 9999,
/* 4121 */ 9999,
/* 4122 */ 931036,
/* 4123 */ 9999,
/* 4124 */ 9999,
/* 4125 */ 931036,
/* 4126 */ 9999,
/* 4127 */ 9999,
/* 4128 */ 9999,
/* 4129 */ 932016,
/* 4130 */ 9999,
/* 4131 */ 9999,
/* 4132 */ 932016,
/* 4133 */ 9999,
/* 4134 */ 9999,
/* 4135 */ 9999,
/* 4136 */ 933013,
/* 4137 */ 9999,
/* 4138 */ 9999,
/* 4139 */ 933013,
/* 4140 */ 9999,
/* 4141 */ 933020,
/* 4142 */ 933054,
/* 4143 */ 933034,
/* 4144 */ 9999,
/* 4145 */ 933034,
/* 4146 */ 9999,
/* 4147 */ 9999,
/* 4148 */ 933034,
/* 4149 */ 9999,
/* 4150 */ 9999,
/* 4151 */ 9999,
/* 4152 */ 933055,
/* 4153 */ 9999,
/* 4154 */ 9999,
/* 4155 */ 933055,
/* 4156 */ 9999,
/* 4157 */ 9999,
/* 4158 */ 9999,
/* 4159 */ 9999,
/* 4160 */ 942007,
/* 4161 */ 9999,
/* 4162 */ 9999,
/* 4163 */ 940020,
/* 4164 */ 9999,
/* 4165 */ 942008,
/* 4166 */ 942020,
/* 4167 */ 9999,
/* 4168 */ 942020,
/* 4169 */ 9999,
/* 4170 */ 9999,
/* 4171 */ 942020,
/* 4172 */ 9999,
/* 4173 */ 9999,
/* 4174 */ 947015,
/* 4175 */ 9999,
/* 4176 */ 9999,
/* 4177 */ 9999,
/* 4178 */ 947013,
/* 4179 */ 9999,
/* 4180 */ 948005,
/* 4181 */ 9999,
/* 4182 */ 948015,
/* 4183 */ 9999,
/* 4184 */ 9999,
/* 4185 */ 949010,
/* 4186 */ 949017,
/* 4187 */ 9999,
/* 4188 */ 9999,
/* 4189 */ 9999,
/* 4190 */ 949008,
/* 4191 */ 9999,
/* 4192 */ 9999,
/* 4193 */ 950013,
/* 4194 */ 950020,
/* 4195 */ 9999,
/* 4196 */ 9999,
/* 4197 */ 950011,
/* 4198 */ 9999,
/* 4199 */ 9999,
/* 4200 */ 951014,
/* 4201 */ 951021,
/* 4202 */ 9999,
/* 4203 */ 9999,
/* 4204 */ 951012,
/* 4205 */ 9999,
/* 4206 */ 9999,
/* 4207 */ 952011,
/* 4208 */ 952019,
/* 4209 */ 9999,
/* 4210 */ 9999,
/* 4211 */ 952009,
/* 4212 */ 9999,
/* 4213 */ 953005,
/* 4214 */ 9999,
/* 4215 */ 953016,
/* 4216 */ 9999,
/* 4217 */ 954005,
/* 4218 */ 9999,
/* 4219 */ 954014,
/* 4220 */ 9999,
/* 4221 */ 955005,
/* 4222 */ 9999,
/* 4223 */ 955017,
/* 4224 */ 9999,
/* 4225 */ 9999,
/* 4226 */ 9999,
/* 4227 */ 947016,
/* 4228 */ 9999,
/* 4229 */ 9999,
/* 4230 */ 947016,
/* 4231 */ 9999,
/* 4232 */ 9999,
/* 4233 */ 9999,
/* 4234 */ 949011,
/* 4235 */ 9999,
/* 4236 */ 9999,
/* 4237 */ 949011,
/* 4238 */ 9999,
/* 4239 */ 9999,
/* 4240 */ 9999,
/* 4241 */ 950014,
/* 4242 */ 9999,
/* 4243 */ 9999,
/* 4244 */ 950014,
/* 4245 */ 9999,
/* 4246 */ 9999,
/* 4247 */ 9999,
/* 4248 */ 951015,
/* 4249 */ 9999,
/* 4250 */ 9999,
/* 4251 */ 951015,
/* 4252 */ 9999,
/* 4253 */ 9999,
/* 4254 */ 9999,
/* 4255 */ 952012,
/* 4256 */ 9999,
/* 4257 */ 9999,
/* 4258 */ 952012,
/* 4259 */ 9999,
/* 4260 */ 952020,
/* 4261 */ 952030,
/* 4262 */ 952025,
/* 4263 */ 9999,
/* 4264 */ 952025,
/* 4265 */ 9999,
/* 4266 */ 9999,
/* 4267 */ 952025,
/* 4268 */ 9999,
/* 4269 */ 9999,
/* 4270 */ 9999,
/* 4271 */ 952031,
/* 4272 */ 9999,
/* 4273 */ 9999,
/* 4274 */ 9999,
/* 4275 */ 952037,
/* 4276 */ 9999,
/* 4277 */ 9999,
/* 4278 */ 952037,
/* 4279 */ 9999,
/* 4280 */ 9999,
/* 4281 */ 9999,
/* 4282 */ 9999,
/* 4283 */ 958038,
/* 4284 */ 9999,
/* 4285 */ 961007,
/* 4286 */ 9999,
/* 4287 */ 9999,
/* 4288 */ 958018,
/* 4289 */ 9999,
/* 4290 */ 961008,
/* 4291 */ 961019,
/* 4292 */ 9999,
/* 4293 */ 961019,
/* 4294 */ 9999,
/* 4295 */ 9999,
/* 4296 */ 961019,
/* 4297 */ 9999,
/* 4298 */ 9999,
/* 4299 */ 966015,
/* 4300 */ 9999,
/* 4301 */ 9999,
/* 4302 */ 9999,
/* 4303 */ 966013,
/* 4304 */ 9999,
/* 4305 */ 9999,
/* 4306 */ 967011,
/* 4307 */ 967018,
/* 4308 */ 9999,
/* 4309 */ 9999,
/* 4310 */ 967009,
/* 4311 */ 9999,
/* 4312 */ 9999,
/* 4313 */ 968011,
/* 4314 */ 968018,
/* 4315 */ 9999,
/* 4316 */ 9999,
/* 4317 */ 968009,
/* 4318 */ 9999,
/* 4319 */ 9999,
/* 4320 */ 969011,
/* 4321 */ 969018,
/* 4322 */ 9999,
/* 4323 */ 9999,
/* 4324 */ 969008,
/* 4325 */ 9999,
/* 4326 */ 9999,
/* 4327 */ 970011,
/* 4328 */ 970018,
/* 4329 */ 9999,
/* 4330 */ 9999,
/* 4331 */ 970009,
/* 4332 */ 9999,
/* 4333 */ 9999,
/* 4334 */ 971011,
/* 4335 */ 971018,
/* 4336 */ 9999,
/* 4337 */ 9999,
/* 4338 */ 971008,
/* 4339 */ 9999,
/* 4340 */ 9999,
/* 4341 */ 972015,
/* 4342 */ 972022,
/* 4343 */ 9999,
/* 4344 */ 9999,
/* 4345 */ 972013,
/* 4346 */ 9999,
/* 4347 */ 9999,
/* 4348 */ 974011,
/* 4349 */ 974019,
/* 4350 */ 9999,
/* 4351 */ 9999,
/* 4352 */ 974009,
/* 4353 */ 9999,
/* 4354 */ 9999,
/* 4355 */ 9999,
/* 4356 */ 9999,
/* 4357 */ 9999,
/* 4358 */ 9999,
/* 4359 */ 975008,
/* 4360 */ 9999,
/* 4361 */ 9999,
/* 4362 */ 9999,
/* 4363 */ 978015,
/* 4364 */ 9999,
/* 4365 */ 9999,
/* 4366 */ 978009,
/* 4367 */ 9999,
/* 4368 */ 9999,
/* 4369 */ 9999,
/* 4370 */ 994017,
/* 4371 */ 9999,
/* 4372 */ 994011,
/* 4373 */ 9999,
/* 4374 */ 9999,
/* 4375 */ 9999,
/* 4376 */ 995017,
/* 4377 */ 9999,
/* 4378 */ 995011,
/* 4379 */ 9999,
/* 4380 */ 996005,
/* 4381 */ 9999,
/* 4382 */ 996016,
/* 4383 */ 9999,
/* 4384 */ 997005,
/* 4385 */ 9999,
/* 4386 */ 997014,
/* 4387 */ 9999,
/* 4388 */ 998005,
/* 4389 */ 9999,
/* 4390 */ 998017,
/* 4391 */ 9999,
/* 4392 */ 9999,
/* 4393 */ 9999,
/* 4394 */ 966016,
/* 4395 */ 9999,
/* 4396 */ 9999,
/* 4397 */ 966016,
/* 4398 */ 9999,
/* 4399 */ 9999,
/* 4400 */ 9999,
/* 4401 */ 967012,
/* 4402 */ 9999,
/* 4403 */ 9999,
/* 4404 */ 967012,
/* 4405 */ 9999,
/* 4406 */ 9999,
/* 4407 */ 9999,
/* 4408 */ 968012,
/* 4409 */ 9999,
/* 4410 */ 9999,
/* 4411 */ 968012,
/* 4412 */ 9999,
/* 4413 */ 9999,
/* 4414 */ 9999,
/* 4415 */ 969012,
/* 4416 */ 9999,
/* 4417 */ 9999,
/* 4418 */ 969012,
/* 4419 */ 9999,
/* 4420 */ 9999,
/* 4421 */ 9999,
/* 4422 */ 970012,
/* 4423 */ 9999,
/* 4424 */ 9999,
/* 4425 */ 970012,
/* 4426 */ 9999,
/* 4427 */ 9999,
/* 4428 */ 9999,
/* 4429 */ 971012,
/* 4430 */ 9999,
/* 4431 */ 9999,
/* 4432 */ 971012,
/* 4433 */ 9999,
/* 4434 */ 9999,
/* 4435 */ 9999,
/* 4436 */ 972016,
/* 4437 */ 9999,
/* 4438 */ 9999,
/* 4439 */ 972016,
/* 4440 */ 9999,
/* 4441 */ 972023,
/* 4442 */ 973006,
/* 4443 */ 972028,
/* 4444 */ 9999,
/* 4445 */ 972028,
/* 4446 */ 9999,
/* 4447 */ 9999,
/* 4448 */ 972028,
/* 4449 */ 9999,
/* 4450 */ 9999,
/* 4451 */ 9999,
/* 4452 */ 973007,
/* 4453 */ 9999,
/* 4454 */ 9999,
/* 4455 */ 9999,
/* 4456 */ 973013,
/* 4457 */ 9999,
/* 4458 */ 9999,
/* 4459 */ 973013,
/* 4460 */ 9999,
/* 4461 */ 9999,
/* 4462 */ 9999,
/* 4463 */ 974012,
/* 4464 */ 9999,
/* 4465 */ 9999,
/* 4466 */ 974012,
/* 4467 */ 9999,
/* 4468 */ 974020,
/* 4469 */ 974030,
/* 4470 */ 974025,
/* 4471 */ 9999,
/* 4472 */ 974025,
/* 4473 */ 9999,
/* 4474 */ 9999,
/* 4475 */ 974025,
/* 4476 */ 9999,
/* 4477 */ 9999,
/* 4478 */ 9999,
/* 4479 */ 974031,
/* 4480 */ 9999,
/* 4481 */ 9999,
/* 4482 */ 9999,
/* 4483 */ 974037,
/* 4484 */ 9999,
/* 4485 */ 9999,
/* 4486 */ 974037,
/* 4487 */ 9999,
/* 4488 */ 9999,
/* 4489 */ 9999,
/* 4490 */ 980025,
/* 4491 */ 981006,
/* 4492 */ 993003,
/* 4493 */ 978035,
/* 4494 */ 9999,
/* 4495 */ 978035,
/* 4496 */ 9999,
/* 4497 */ 9999,
/* 4498 */ 978035,
/* 4499 */ 9999,
/* 4500 */ 9999,
/* 4501 */ 981014,
/* 4502 */ 9999,
/* 4503 */ 981010,
/* 4504 */ 9999,
/* 4505 */ 981010,
/* 4506 */ 9999,
/* 4507 */ 9999,
/* 4508 */ 982015,
/* 4509 */ 987010,
/* 4510 */ 982013,
/* 4511 */ 9999,
/* 4512 */ 982013,
/* 4513 */ 9999,
/* 4514 */ 9999,
/* 4515 */ 991013,
/* 4516 */ 991011,
/* 4517 */ 9999,
/* 4518 */ 991011,
/* 4519 */ 9999,
/* 4520 */ 9999,
/* 4521 */ 991011,
/* 4522 */ 9999,
/* 4523 */ 982017,
/* 4524 */ 9999,
/* 4525 */ 982023,
/* 4526 */ 9999,
/* 4527 */ 9999,
/* 4528 */ 9999,
/* 4529 */ 983031,
/* 4530 */ 9999,
/* 4531 */ 983024,
/* 4532 */ 9999,
/* 4533 */ 9999,
/* 4534 */ 9999,
/* 4535 */ 985034,
/* 4536 */ 9999,
/* 4537 */ 985024,
/* 4538 */ 9999,
/* 4539 */ 983032,
/* 4540 */ 9999,
/* 4541 */ 983036,
/* 4542 */ 9999,
/* 4543 */ 9999,
/* 4544 */ 983036,
/* 4545 */ 9999,
/* 4546 */ 985035,
/* 4547 */ 9999,
/* 4548 */ 985039,
/* 4549 */ 9999,
/* 4550 */ 9999,
/* 4551 */ 985039,
/* 4552 */ 9999,
/* 4553 */ 9999,
/* 4554 */ 987014,
/* 4555 */ 9999,
/* 4556 */ 987012,
/* 4557 */ 9999,
/* 4558 */ 9999,
/* 4559 */ 9999,
/* 4560 */ 988023,
/* 4561 */ 9999,
/* 4562 */ 988035,
/* 4563 */ 9999,
/* 4564 */ 988014,
/* 4565 */ 9999,
/* 4566 */ 9999,
/* 4567 */ 990016,
/* 4568 */ 9999,
/* 4569 */ 990014,
/* 4570 */ 9999,
/* 4571 */ 9999,
/* 4572 */ 990014,
/* 4573 */ 9999,
/* 4574 */ 9999,
/* 4575 */ 9999,
/* 4576 */ 993004,
/* 4577 */ 9999,
/* 4578 */ 9999,
/* 4579 */ 993004,
/* 4580 */ 9999,
/* 4581 */ 994018,
/* 4582 */ 994029,
/* 4583 */ 9999,
/* 4584 */ 994029,
/* 4585 */ 9999,
/* 4586 */ 9999,
/* 4587 */ 994029,
/* 4588 */ 9999,
/* 4589 */ 995018,
/* 4590 */ 995029,
/* 4591 */ 9999,
/* 4592 */ 995029,
/* 4593 */ 9999,
/* 4594 */ 9999,
/* 4595 */ 995029,
/* 4596 */ 9999,
/* 4597 */ 9999,
/* 4598 */ 9999,
/* 4599 */ 9999,
/* 4600 */ 1003009,
/* 4601 */ 1004005,
/* 4602 */ 9999,
/* 4603 */ 1005007,
/* 4604 */ 9999,
/* 4605 */ 9999,
/* 4606 */ 1001027,
/* 4607 */ 9999,
/* 4608 */ 9999,
/* 4609 */ 1004010,
/* 4610 */ 9999,
/* 4611 */ 1004008,
/* 4612 */ 9999,
/* 4613 */ 9999,
/* 4614 */ 1004008,
/* 4615 */ 9999,
/* 4616 */ 1005008,
/* 4617 */ 1005021,
/* 4618 */ 9999,
/* 4619 */ 1005021,
/* 4620 */ 9999,
/* 4621 */ 9999,
/* 4622 */ 1005021,
/* 4623 */ 9999,
/* 4624 */ 9999,
/* 4625 */ 1010015,
/* 4626 */ 9999,
/* 4627 */ 9999,
/* 4628 */ 9999,
/* 4629 */ 1010013,
/* 4630 */ 9999,
/* 4631 */ 9999,
/* 4632 */ 1011009,
/* 4633 */ 1011016,
/* 4634 */ 9999,
/* 4635 */ 9999,
/* 4636 */ 1011007,
/* 4637 */ 9999,
/* 4638 */ 9999,
/* 4639 */ 1012011,
/* 4640 */ 1012018,
/* 4641 */ 9999,
/* 4642 */ 9999,
/* 4643 */ 1012009,
/* 4644 */ 9999,
/* 4645 */ 1013005,
/* 4646 */ 9999,
/* 4647 */ 1013015,
/* 4648 */ 9999,
/* 4649 */ 1014005,
/* 4650 */ 1014028,
/* 4651 */ 1014035,
/* 4652 */ 9999,
/* 4653 */ 9999,
/* 4654 */ 1014005,
/* 4655 */ 9999,
/* 4656 */ 9999,
/* 4657 */ 1015013,
/* 4658 */ 1015020,
/* 4659 */ 9999,
/* 4660 */ 9999,
/* 4661 */ 1015011,
/* 4662 */ 9999,
/* 4663 */ 9999,
/* 4664 */ 1016013,
/* 4665 */ 1016020,
/* 4666 */ 9999,
/* 4667 */ 9999,
/* 4668 */ 1016007,
/* 4669 */ 9999,
/* 4670 */ 9999,
/* 4671 */ 1017013,
/* 4672 */ 1017020,
/* 4673 */ 9999,
/* 4674 */ 9999,
/* 4675 */ 1017007,
/* 4676 */ 9999,
/* 4677 */ 9999,
/* 4678 */ 1018013,
/* 4679 */ 1018020,
/* 4680 */ 9999,
/* 4681 */ 9999,
/* 4682 */ 1018007,
/* 4683 */ 9999,
/* 4684 */ 9999,
/* 4685 */ 1019013,
/* 4686 */ 1019020,
/* 4687 */ 9999,
/* 4688 */ 9999,
/* 4689 */ 1019007,
/* 4690 */ 9999,
/* 4691 */ 9999,
/* 4692 */ 1020013,
/* 4693 */ 1020020,
/* 4694 */ 9999,
/* 4695 */ 9999,
/* 4696 */ 1020009,
/* 4697 */ 9999,
/* 4698 */ 9999,
/* 4699 */ 9999,
/* 4700 */ 1021024,
/* 4701 */ 1021031,
/* 4702 */ 9999,
/* 4703 */ 9999,
/* 4704 */ 1021010,
/* 4705 */ 9999,
/* 4706 */ 9999,
/* 4707 */ 1022013,
/* 4708 */ 1022020,
/* 4709 */ 9999,
/* 4710 */ 9999,
/* 4711 */ 1022008,
/* 4712 */ 9999,
/* 4713 */ 9999,
/* 4714 */ 1023013,
/* 4715 */ 1023020,
/* 4716 */ 9999,
/* 4717 */ 9999,
/* 4718 */ 1023009,
/* 4719 */ 9999,
/* 4720 */ 9999,
/* 4721 */ 1024013,
/* 4722 */ 1024020,
/* 4723 */ 9999,
/* 4724 */ 9999,
/* 4725 */ 1024008,
/* 4726 */ 9999,
/* 4727 */ 9999,
/* 4728 */ 1025019,
/* 4729 */ 1025026,
/* 4730 */ 1025035,
/* 4731 */ 9999,
/* 4732 */ 1025013,
/* 4733 */ 9999,
/* 4734 */ 9999,
/* 4735 */ 1026012,
/* 4736 */ 1026019,
/* 4737 */ 1026037,
/* 4738 */ 9999,
/* 4739 */ 9999,
/* 4740 */ 1026010,
/* 4741 */ 9999,
/* 4742 */ 9999,
/* 4743 */ 9999,
/* 4744 */ 1028015,
/* 4745 */ 9999,
/* 4746 */ 9999,
/* 4747 */ 9999,
/* 4748 */ 1028009,
/* 4749 */ 9999,
/* 4750 */ 9999,
/* 4751 */ 9999,
/* 4752 */ 1030015,
/* 4753 */ 9999,
/* 4754 */ 1030030,
/* 4755 */ 9999,
/* 4756 */ 9999,
/* 4757 */ 9999,
/* 4758 */ 1030009,
/* 4759 */ 9999,
/* 4760 */ 9999,
/* 4761 */ 9999,
/* 4762 */ 1032015,
/* 4763 */ 9999,
/* 4764 */ 1032030,
/* 4765 */ 9999,
/* 4766 */ 1032046,
/* 4767 */ 9999,
/* 4768 */ 9999,
/* 4769 */ 9999,
/* 4770 */ 1032009,
/* 4771 */ 9999,
/* 4772 */ 9999,
/* 4773 */ 9999,
/* 4774 */ 1035015,
/* 4775 */ 9999,
/* 4776 */ 1035030,
/* 4777 */ 9999,
/* 4778 */ 1035046,
/* 4779 */ 9999,
/* 4780 */ 1035062,
/* 4781 */ 9999,
/* 4782 */ 9999,
/* 4783 */ 9999,
/* 4784 */ 1035009,
/* 4785 */ 9999,
/* 4786 */ 9999,
/* 4787 */ 1039013,
/* 4788 */ 1039020,
/* 4789 */ 9999,
/* 4790 */ 9999,
/* 4791 */ 1039011,
/* 4792 */ 9999,
/* 4793 */ 9999,
/* 4794 */ 1042013,
/* 4795 */ 1042020,
/* 4796 */ 9999,
/* 4797 */ 9999,
/* 4798 */ 1042010,
/* 4799 */ 9999,
/* 4800 */ 9999,
/* 4801 */ 1045011,
/* 4802 */ 1045018,
/* 4803 */ 1046019,
/* 4804 */ 9999,
/* 4805 */ 9999,
/* 4806 */ 1045009,
/* 4807 */ 9999,
/* 4808 */ 9999,
/* 4809 */ 1047012,
/* 4810 */ 1047019,
/* 4811 */ 9999,
/* 4812 */ 9999,
/* 4813 */ 1047010,
/* 4814 */ 9999,
/* 4815 */ 9999,
/* 4816 */ 1050013,
/* 4817 */ 1050020,
/* 4818 */ 9999,
/* 4819 */ 9999,
/* 4820 */ 1050011,
/* 4821 */ 9999,
/* 4822 */ 9999,
/* 4823 */ 1052012,
/* 4824 */ 1052019,
/* 4825 */ 9999,
/* 4826 */ 9999,
/* 4827 */ 1052010,
/* 4828 */ 9999,
/* 4829 */ 9999,
/* 4830 */ 1054011,
/* 4831 */ 1054018,
/* 4832 */ 9999,
/* 4833 */ 1054009,
/* 4834 */ 9999,
/* 4835 */ 1062005,
/* 4836 */ 9999,
/* 4837 */ 1062013,
/* 4838 */ 9999,
/* 4839 */ 1063005,
/* 4840 */ 9999,
/* 4841 */ 1063014,
/* 4842 */ 9999,
/* 4843 */ 1064005,
/* 4844 */ 9999,
/* 4845 */ 1064016,
/* 4846 */ 9999,
/* 4847 */ 1065005,
/* 4848 */ 9999,
/* 4849 */ 1065017,
/* 4850 */ 9999,
/* 4851 */ 9999,
/* 4852 */ 9999,
/* 4853 */ 1010016,
/* 4854 */ 9999,
/* 4855 */ 9999,
/* 4856 */ 1010016,
/* 4857 */ 9999,
/* 4858 */ 9999,
/* 4859 */ 9999,
/* 4860 */ 1011010,
/* 4861 */ 9999,
/* 4862 */ 9999,
/* 4863 */ 1011010,
/* 4864 */ 9999,
/* 4865 */ 9999,
/* 4866 */ 9999,
/* 4867 */ 1012012,
/* 4868 */ 9999,
/* 4869 */ 9999,
/* 4870 */ 1012012,
/* 4871 */ 9999,
/* 4872 */ 9999,
/* 4873 */ 9999,
/* 4874 */ 1014015,
/* 4875 */ 9999,
/* 4876 */ 9999,
/* 4877 */ 9999,
/* 4878 */ 1014025,
/* 4879 */ 9999,
/* 4880 */ 9999,
/* 4881 */ 9999,
/* 4882 */ 1014029,
/* 4883 */ 9999,
/* 4884 */ 9999,
/* 4885 */ 1014029,
/* 4886 */ 9999,
/* 4887 */ 9999,
/* 4888 */ 9999,
/* 4889 */ 1015014,
/* 4890 */ 9999,
/* 4891 */ 9999,
/* 4892 */ 1015014,
/* 4893 */ 9999,
/* 4894 */ 9999,
/* 4895 */ 9999,
/* 4896 */ 1016014,
/* 4897 */ 9999,
/* 4898 */ 9999,
/* 4899 */ 1016014,
/* 4900 */ 9999,
/* 4901 */ 9999,
/* 4902 */ 9999,
/* 4903 */ 1017014,
/* 4904 */ 9999,
/* 4905 */ 9999,
/* 4906 */ 1017014,
/* 4907 */ 9999,
/* 4908 */ 9999,
/* 4909 */ 9999,
/* 4910 */ 1018014,
/* 4911 */ 9999,
/* 4912 */ 9999,
/* 4913 */ 1018014,
/* 4914 */ 9999,
/* 4915 */ 9999,
/* 4916 */ 9999,
/* 4917 */ 1019014,
/* 4918 */ 9999,
/* 4919 */ 9999,
/* 4920 */ 1019014,
/* 4921 */ 9999,
/* 4922 */ 9999,
/* 4923 */ 9999,
/* 4924 */ 1020014,
/* 4925 */ 9999,
/* 4926 */ 9999,
/* 4927 */ 1020014,
/* 4928 */ 9999,
/* 4929 */ 9999,
/* 4930 */ 9999,
/* 4931 */ 1021025,
/* 4932 */ 9999,
/* 4933 */ 9999,
/* 4934 */ 1021025,
/* 4935 */ 9999,
/* 4936 */ 9999,
/* 4937 */ 9999,
/* 4938 */ 1022014,
/* 4939 */ 9999,
/* 4940 */ 9999,
/* 4941 */ 1022014,
/* 4942 */ 9999,
/* 4943 */ 9999,
/* 4944 */ 9999,
/* 4945 */ 1023014,
/* 4946 */ 9999,
/* 4947 */ 9999,
/* 4948 */ 1023014,
/* 4949 */ 9999,
/* 4950 */ 9999,
/* 4951 */ 9999,
/* 4952 */ 1024014,
/* 4953 */ 9999,
/* 4954 */ 9999,
/* 4955 */ 1024014,
/* 4956 */ 9999,
/* 4957 */ 9999,
/* 4958 */ 9999,
/* 4959 */ 1025020,
/* 4960 */ 9999,
/* 4961 */ 9999,
/* 4962 */ 1025020,
/* 4963 */ 9999,
/* 4964 */ 9999,
/* 4965 */ 9999,
/* 4966 */ 1025036,
/* 4967 */ 9999,
/* 4968 */ 9999,
/* 4969 */ 9999,
/* 4970 */ 1025040,
/* 4971 */ 9999,
/* 4972 */ 9999,
/* 4973 */ 9999,
/* 4974 */ 1026013,
/* 4975 */ 9999,
/* 4976 */ 9999,
/* 4977 */ 1026013,
/* 4978 */ 9999,
/* 4979 */ 9999,
/* 4980 */ 1026042,
/* 4981 */ 1026038,
/* 4982 */ 9999,
/* 4983 */ 1026038,
/* 4984 */ 9999,
/* 4985 */ 9999,
/* 4986 */ 1026038,
/* 4987 */ 9999,
/* 4988 */ 9999,
/* 4989 */ 9999,
/* 4990 */ 1039014,
/* 4991 */ 9999,
/* 4992 */ 9999,
/* 4993 */ 1039014,
/* 4994 */ 9999,
/* 4995 */ 1039021,
/* 4996 */ 1041007,
/* 4997 */ 1039021,
/* 4998 */ 9999,
/* 4999 */ 1039021,
/* 5000 */ 9999,
/* 5001 */ 9999,
/* 5002 */ 1039021,
/* 5003 */ 9999,
/* 5004 */ 1039023,
/* 5005 */ 9999,
/* 5006 */ 1039028,
/* 5007 */ 9999,
/* 5008 */ 1040015,
/* 5009 */ 1040025,
/* 5010 */ 9999,
/* 5011 */ 1040015,
/* 5012 */ 9999,
/* 5013 */ 9999,
/* 5014 */ 9999,
/* 5015 */ 1040016,
/* 5016 */ 9999,
/* 5017 */ 9999,
/* 5018 */ 9999,
/* 5019 */ 1040020,
/* 5020 */ 9999,
/* 5021 */ 9999,
/* 5022 */ 9999,
/* 5023 */ 1041008,
/* 5024 */ 9999,
/* 5025 */ 9999,
/* 5026 */ 1041008,
/* 5027 */ 9999,
/* 5028 */ 9999,
/* 5029 */ 9999,
/* 5030 */ 1042014,
/* 5031 */ 9999,
/* 5032 */ 9999,
/* 5033 */ 1042014,
/* 5034 */ 9999,
/* 5035 */ 1042021,
/* 5036 */ 1044007,
/* 5037 */ 1042021,
/* 5038 */ 9999,
/* 5039 */ 1042021,
/* 5040 */ 9999,
/* 5041 */ 9999,
/* 5042 */ 1042021,
/* 5043 */ 9999,
/* 5044 */ 1042023,
/* 5045 */ 9999,
/* 5046 */ 1042028,
/* 5047 */ 9999,
/* 5048 */ 1043015,
/* 5049 */ 1043025,
/* 5050 */ 9999,
/* 5051 */ 1043015,
/* 5052 */ 9999,
/* 5053 */ 9999,
/* 5054 */ 9999,
/* 5055 */ 1043016,
/* 5056 */ 9999,
/* 5057 */ 9999,
/* 5058 */ 9999,
/* 5059 */ 1043020,
/* 5060 */ 9999,
/* 5061 */ 9999,
/* 5062 */ 9999,
/* 5063 */ 1044008,
/* 5064 */ 9999,
/* 5065 */ 9999,
/* 5066 */ 1044008,
/* 5067 */ 9999,
/* 5068 */ 9999,
/* 5069 */ 9999,
/* 5070 */ 1045012,
/* 5071 */ 9999,
/* 5072 */ 9999,
/* 5073 */ 1045012,
/* 5074 */ 9999,
/* 5075 */ 1045019,
/* 5076 */ 9999,
/* 5077 */ 1045024,
/* 5078 */ 9999,
/* 5079 */ 1045024,
/* 5080 */ 9999,
/* 5081 */ 9999,
/* 5082 */ 1045024,
/* 5083 */ 9999,
/* 5084 */ 9999,
/* 5085 */ 9999,
/* 5086 */ 1047013,
/* 5087 */ 9999,
/* 5088 */ 9999,
/* 5089 */ 1047013,
/* 5090 */ 9999,
/* 5091 */ 1047021,
/* 5092 */ 1049006,
/* 5093 */ 1047027,
/* 5094 */ 9999,
/* 5095 */ 1047027,
/* 5096 */ 9999,
/* 5097 */ 9999,
/* 5098 */ 1047027,
/* 5099 */ 9999,
/* 5100 */ 9999,
/* 5101 */ 9999,
/* 5102 */ 1049007,
/* 5103 */ 9999,
/* 5104 */ 9999,
/* 5105 */ 1049007,
/* 5106 */ 9999,
/* 5107 */ 9999,
/* 5108 */ 9999,
/* 5109 */ 1050014,
/* 5110 */ 9999,
/* 5111 */ 9999,
/* 5112 */ 1050014,
/* 5113 */ 9999,
/* 5114 */ 1050022,
/* 5115 */ 1051006,
/* 5116 */ 1050028,
/* 5117 */ 9999,
/* 5118 */ 1050028,
/* 5119 */ 9999,
/* 5120 */ 9999,
/* 5121 */ 1050028,
/* 5122 */ 9999,
/* 5123 */ 9999,
/* 5124 */ 9999,
/* 5125 */ 1051007,
/* 5126 */ 9999,
/* 5127 */ 9999,
/* 5128 */ 1051007,
/* 5129 */ 9999,
/* 5130 */ 9999,
/* 5131 */ 9999,
/* 5132 */ 1052013,
/* 5133 */ 9999,
/* 5134 */ 9999,
/* 5135 */ 1052013,
/* 5136 */ 9999,
/* 5137 */ 1052021,
/* 5138 */ 1053006,
/* 5139 */ 1052027,
/* 5140 */ 9999,
/* 5141 */ 1052027,
/* 5142 */ 9999,
/* 5143 */ 9999,
/* 5144 */ 1052027,
/* 5145 */ 9999,
/* 5146 */ 9999,
/* 5147 */ 9999,
/* 5148 */ 1053007,
/* 5149 */ 9999,
/* 5150 */ 9999,
/* 5151 */ 1053007,
/* 5152 */ 9999,
/* 5153 */ 9999,
/* 5154 */ 9999,
/* 5155 */ 1054012,
/* 5156 */ 9999,
/* 5157 */ 9999,
/* 5158 */ 1054012,
/* 5159 */ 9999,
/* 5160 */ 1054020,
/* 5161 */ 1061006,
/* 5162 */ 1054020,
/* 5163 */ 9999,
/* 5164 */ 1054020,
/* 5165 */ 9999,
/* 5166 */ 9999,
/* 5167 */ 1054020,
/* 5168 */ 9999,
/* 5169 */ 1055006,
/* 5170 */ 9999,
/* 5171 */ 1055011,
/* 5172 */ 9999,
/* 5173 */ 1060008,
/* 5174 */ 1060018,
/* 5175 */ 9999,
/* 5176 */ 1060008,
/* 5177 */ 9999,
/* 5178 */ 9999,
/* 5179 */ 9999,
/* 5180 */ 1060009,
/* 5181 */ 9999,
/* 5182 */ 9999,
/* 5183 */ 9999,
/* 5184 */ 1060013,
/* 5185 */ 9999,
/* 5186 */ 9999,
/* 5187 */ 9999,
/* 5188 */ 1061007,
/* 5189 */ 9999,
/* 5190 */ 9999,
/* 5191 */ 9999,
/* 5192 */ 1061011,
/* 5193 */ 9999,
/* 5194 */ 1070003,
/* 5195 */ 9999,
/* 5196 */ 1071005,
/* 5197 */ 1073003,
/* 5198 */ 1075003,
/* 5199 */ 1076005,
/* 5200 */ 1088003,
/* 5201 */ 9999,
/* 5202 */ 1068032,
/* 5203 */ 9999,
/* 5204 */ 1101005,
/* 5205 */ 9999,
/* 5206 */ 1101020,
/* 5207 */ 1103019,
/* 5208 */ 9999,
/* 5209 */ 1101010,
/* 5210 */ 9999,
/* 5211 */ 1071007,
/* 5212 */ 9999,
/* 5213 */ 1071015,
/* 5214 */ 9999,
/* 5215 */ 1072009,
/* 5216 */ 9999,
/* 5217 */ 9999,
/* 5218 */ 1072014,
/* 5219 */ 9999,
/* 5220 */ 9999,
/* 5221 */ 1072014,
/* 5222 */ 9999,
/* 5223 */ 1073004,
/* 5224 */ 9999,
/* 5225 */ 1073009,
/* 5226 */ 9999,
/* 5227 */ 9999,
/* 5228 */ 1073009,
/* 5229 */ 9999,
/* 5230 */ 9999,
/* 5231 */ 9999,
/* 5232 */ 1075006,
/* 5233 */ 9999,
/* 5234 */ 9999,
/* 5235 */ 1075006,
/* 5236 */ 9999,
/* 5237 */ 9999,
/* 5238 */ 1076011,
/* 5239 */ 1076040,
/* 5240 */ 9999,
/* 5241 */ 9999,
/* 5242 */ 1076007,
/* 5243 */ 9999,
/* 5244 */ 9999,
/* 5245 */ 1076007,
/* 5246 */ 9999,
/* 5247 */ 9999,
/* 5248 */ 9999,
/* 5249 */ 1076018,
/* 5250 */ 9999,
/* 5251 */ 9999,
/* 5252 */ 9999,
/* 5253 */ 1076026,
/* 5254 */ 9999,
/* 5255 */ 9999,
/* 5256 */ 9999,
/* 5257 */ 1076037,
/* 5258 */ 9999,
/* 5259 */ 9999,
/* 5260 */ 1089005,
/* 5261 */ 1090005,
/* 5262 */ 1091005,
/* 5263 */ 1092005,
/* 5264 */ 1088006,
/* 5265 */ 9999,
/* 5266 */ 1088006,
/* 5267 */ 9999,
/* 5268 */ 9999,
/* 5269 */ 1088006,
/* 5270 */ 9999,
/* 5271 */ 1089007,
/* 5272 */ 9999,
/* 5273 */ 1089015,
/* 5274 */ 9999,
/* 5275 */ 9999,
/* 5276 */ 1089015,
/* 5277 */ 9999,
/* 5278 */ 1090006,
/* 5279 */ 9999,
/* 5280 */ 1090011,
/* 5281 */ 9999,
/* 5282 */ 9999,
/* 5283 */ 1090011,
/* 5284 */ 9999,
/* 5285 */ 9999,
/* 5286 */ 9999,
/* 5287 */ 1091008,
/* 5288 */ 9999,
/* 5289 */ 9999,
/* 5290 */ 1091008,
/* 5291 */ 9999,
/* 5292 */ 9999,
/* 5293 */ 1092011,
/* 5294 */ 1092040,
/* 5295 */ 9999,
/* 5296 */ 9999,
/* 5297 */ 1092007,
/* 5298 */ 9999,
/* 5299 */ 9999,
/* 5300 */ 1092007,
/* 5301 */ 9999,
/* 5302 */ 9999,
/* 5303 */ 9999,
/* 5304 */ 1092018,
/* 5305 */ 9999,
/* 5306 */ 9999,
/* 5307 */ 9999,
/* 5308 */ 1092026,
/* 5309 */ 9999,
/* 5310 */ 9999,
/* 5311 */ 9999,
/* 5312 */ 1092037,
/* 5313 */ 9999,
/* 5314 */ 1103021,
/* 5315 */ 9999,
/* 5316 */ 1103026,
/* 5317 */ 9999,
/* 5318 */ 9999,
/* 5319 */ 1103026,
/* 5320 */ 9999,
/* 5321 */ 9999,
/* 5322 */ 9999,
/* 5323 */ 1107018,
/* 5324 */ 9999,
/* 5325 */ 9999,
/* 5326 */ 9999,
/* 5327 */ 9999,
/* 5328 */ 1110028,
/* 5329 */ 9999,
/* 5330 */ 1116007,
/* 5331 */ 9999,
/* 5332 */ 9999,
/* 5333 */ 1110008,
/* 5334 */ 9999,
/* 5335 */ 1116008,
/* 5336 */ 1116019,
/* 5337 */ 9999,
/* 5338 */ 1116019,
/* 5339 */ 9999,
/* 5340 */ 9999,
/* 5341 */ 1116019,
/* 5342 */ 9999,
/* 5343 */ 9999,
/* 5344 */ 1120016,
/* 5345 */ 9999,
/* 5346 */ 9999,
/* 5347 */ 9999,
/* 5348 */ 1120013,
/* 5349 */ 9999,
/* 5350 */ 9999,
/* 5351 */ 1121016,
/* 5352 */ 1121023,
/* 5353 */ 9999,
/* 5354 */ 9999,
/* 5355 */ 1121012,
/* 5356 */ 9999,
/* 5357 */ 9999,
/* 5358 */ 1122016,
/* 5359 */ 1122023,
/* 5360 */ 9999,
/* 5361 */ 9999,
/* 5362 */ 1122014,
/* 5363 */ 9999,
/* 5364 */ 9999,
/* 5365 */ 1123016,
/* 5366 */ 1123023,
/* 5367 */ 9999,
/* 5368 */ 9999,
/* 5369 */ 9999,
/* 5370 */ 1123010,
/* 5371 */ 9999,
/* 5372 */ 9999,
/* 5373 */ 9999,
/* 5374 */ 1120017,
/* 5375 */ 9999,
/* 5376 */ 9999,
/* 5377 */ 1120017,
/* 5378 */ 9999,
/* 5379 */ 9999,
/* 5380 */ 9999,
/* 5381 */ 1121017,
/* 5382 */ 9999,
/* 5383 */ 9999,
/* 5384 */ 1121017,
/* 5385 */ 9999,
/* 5386 */ 9999,
/* 5387 */ 9999,
/* 5388 */ 1122017,
/* 5389 */ 9999,
/* 5390 */ 9999,
/* 5391 */ 1122017,
/* 5392 */ 9999,
/* 5393 */ 9999,
/* 5394 */ 9999,
/* 5395 */ 1123017,
/* 5396 */ 9999,
/* 5397 */ 9999,
/* 5398 */ 1123017,
/* 5399 */ 9999,
/* 5400 */ 9999,
/* 5401 */ 9999,
/* 5402 */ 1131005,
/* 5403 */ 9999,
/* 5404 */ 1133009,
/* 5405 */ 9999,
/* 5406 */ 9999,
/* 5407 */ 1128016,
/* 5408 */ 9999,
/* 5409 */ 9999,
/* 5410 */ 1131011,
/* 5411 */ 9999,
/* 5412 */ 1131007,
/* 5413 */ 9999,
/* 5414 */ 9999,
/* 5415 */ 1131007,
/* 5416 */ 9999,
/* 5417 */ 1133010,
/* 5418 */ 1133024,
/* 5419 */ 9999,
/* 5420 */ 1133024,
/* 5421 */ 9999,
/* 5422 */ 9999,
/* 5423 */ 1133024,
/* 5424 */ 9999,
/* 5425 */ 9999,
/* 5426 */ 9999,
/* 5427 */ 1137017,
/* 5428 */ 9999,
/* 5429 */ 9999,
/* 5430 */ 1137011,
/* 5431 */ 9999,
/* 5432 */ 9999,
/* 5433 */ 9999,
/* 5434 */ 1138015,
/* 5435 */ 9999,
/* 5436 */ 9999,
/* 5437 */ 1138009,
/* 5438 */ 9999,
/* 5439 */ 9999,
/* 5440 */ 9999,
/* 5441 */ 1139016,
/* 5442 */ 9999,
/* 5443 */ 9999,
/* 5444 */ 1139010,
/* 5445 */ 9999,
/* 5446 */ 9999,
/* 5447 */ 9999,
/* 5448 */ 1140017,
/* 5449 */ 9999,
/* 5450 */ 9999,
/* 5451 */ 1140011,
/* 5452 */ 9999,
/* 5453 */ 9999,
/* 5454 */ 9999,
/* 5455 */ 1141017,
/* 5456 */ 9999,
/* 5457 */ 9999,
/* 5458 */ 9999,
/* 5459 */ 1141011,
/* 5460 */ 9999,
/* 5461 */ 9999,
/* 5462 */ 9999,
/* 5463 */ 1142017,
/* 5464 */ 1142027,
/* 5465 */ 1143030,
/* 5466 */ 9999,
/* 5467 */ 9999,
/* 5468 */ 1142011,
/* 5469 */ 9999,
/* 5470 */ 9999,
/* 5471 */ 9999,
/* 5472 */ 1144015,
/* 5473 */ 9999,
/* 5474 */ 9999,
/* 5475 */ 1144009,
/* 5476 */ 9999,
/* 5477 */ 9999,
/* 5478 */ 9999,
/* 5479 */ 1146006,
/* 5480 */ 9999,
/* 5481 */ 9999,
/* 5482 */ 1145009,
/* 5483 */ 9999,
/* 5484 */ 9999,
/* 5485 */ 1148011,
/* 5486 */ 1148018,
/* 5487 */ 9999,
/* 5488 */ 9999,
/* 5489 */ 1148009,
/* 5490 */ 9999,
/* 5491 */ 1150005,
/* 5492 */ 9999,
/* 5493 */ 1150013,
/* 5494 */ 9999,
/* 5495 */ 1151005,
/* 5496 */ 9999,
/* 5497 */ 1151014,
/* 5498 */ 9999,
/* 5499 */ 1152005,
/* 5500 */ 9999,
/* 5501 */ 1152016,
/* 5502 */ 9999,
/* 5503 */ 1153005,
/* 5504 */ 9999,
/* 5505 */ 1153017,
/* 5506 */ 9999,
/* 5507 */ 9999,
/* 5508 */ 9999,
/* 5509 */ 1142029,
/* 5510 */ 9999,
/* 5511 */ 9999,
/* 5512 */ 9999,
/* 5513 */ 1142031,
/* 5514 */ 9999,
/* 5515 */ 1146007,
/* 5516 */ 1146020,
/* 5517 */ 9999,
/* 5518 */ 1146020,
/* 5519 */ 9999,
/* 5520 */ 9999,
/* 5521 */ 1146020,
/* 5522 */ 9999,
/* 5523 */ 9999,
/* 5524 */ 9999,
/* 5525 */ 1148012,
/* 5526 */ 9999,
/* 5527 */ 9999,
/* 5528 */ 1148012,
/* 5529 */ 9999,
/* 5530 */ 1148019,
/* 5531 */ 1149005,
/* 5532 */ 1148024,
/* 5533 */ 9999,
/* 5534 */ 1148024,
/* 5535 */ 9999,
/* 5536 */ 9999,
/* 5537 */ 1148024,
/* 5538 */ 9999,
/* 5539 */ 9999,
/* 5540 */ 9999,
/* 5541 */ 1149006,
/* 5542 */ 9999,
/* 5543 */ 9999,
/* 5544 */ 9999,
/* 5545 */ 1149012,
/* 5546 */ 9999,
/* 5547 */ 9999,
/* 5548 */ 1149012,
/* 5549 */ 9999,
/* 5550 */ 1158003,
/* 5551 */ 1163003,
/* 5552 */ 9999,
/* 5553 */ 1183003,
/* 5554 */ 1189003,
/* 5555 */ 1195003,
/* 5556 */ 9999,
/* 5557 */ 1157003,
/* 5558 */ 9999,
/* 5559 */ 9999,
/* 5560 */ 9999,
/* 5561 */ 1158007,
/* 5562 */ 9999,
/* 5563 */ 9999,
/* 5564 */ 9999,
/* 5565 */ 1159010,
/* 5566 */ 9999,
/* 5567 */ 9999,
/* 5568 */ 9999,
/* 5569 */ 1160007,
/* 5570 */ 9999,
/* 5571 */ 1164007,
/* 5572 */ 9999,
/* 5573 */ 1164012,
/* 5574 */ 9999,
/* 5575 */ 1164012,
/* 5576 */ 9999,
/* 5577 */ 9999,
/* 5578 */ 9999,
/* 5579 */ 1165019,
/* 5580 */ 9999,
/* 5581 */ 9999,
/* 5582 */ 1165007,
/* 5583 */ 9999,
/* 5584 */ 1165007,
/* 5585 */ 9999,
/* 5586 */ 9999,
/* 5587 */ 1166011,
/* 5588 */ 9999,
/* 5589 */ 1166007,
/* 5590 */ 9999,
/* 5591 */ 1166007,
/* 5592 */ 9999,
/* 5593 */ 9999,
/* 5594 */ 1167011,
/* 5595 */ 9999,
/* 5596 */ 1167007,
/* 5597 */ 9999,
/* 5598 */ 1167007,
/* 5599 */ 9999,
/* 5600 */ 1168007,
/* 5601 */ 1168017,
/* 5602 */ 1168026,
/* 5603 */ 1168012,
/* 5604 */ 9999,
/* 5605 */ 1168012,
/* 5606 */ 9999,
/* 5607 */ 1171007,
/* 5608 */ 1171012,
/* 5609 */ 9999,
/* 5610 */ 1171012,
/* 5611 */ 9999,
/* 5612 */ 1174007,
/* 5613 */ 9999,
/* 5614 */ 1174012,
/* 5615 */ 9999,
/* 5616 */ 1174012,
/* 5617 */ 9999,
/* 5618 */ 1177007,
/* 5619 */ 1177015,
/* 5620 */ 9999,
/* 5621 */ 1177015,
/* 5622 */ 9999,
/* 5623 */ 9999,
/* 5624 */ 1177015,
/* 5625 */ 9999,
/* 5626 */ 9999,
/* 5627 */ 9999,
/* 5628 */ 1168018,
/* 5629 */ 9999,
/* 5630 */ 9999,
/* 5631 */ 9999,
/* 5632 */ 1168023,
/* 5633 */ 9999,
/* 5634 */ 1183007,
/* 5635 */ 9999,
/* 5636 */ 1183013,
/* 5637 */ 9999,
/* 5638 */ 1184007,
/* 5639 */ 9999,
/* 5640 */ 1184012,
/* 5641 */ 9999,
/* 5642 */ 9999,
/* 5643 */ 1185016,
/* 5644 */ 9999,
/* 5645 */ 1185014,
/* 5646 */ 9999,
/* 5647 */ 1189007,
/* 5648 */ 9999,
/* 5649 */ 1189013,
/* 5650 */ 9999,
/* 5651 */ 1190007,
/* 5652 */ 9999,
/* 5653 */ 1190012,
/* 5654 */ 9999,
/* 5655 */ 9999,
/* 5656 */ 1191016,
/* 5657 */ 9999,
/* 5658 */ 1191014,
/* 5659 */ 9999,
/* 5660 */ 9999,
/* 5661 */ 1191014,
/* 5662 */ 9999,
/* 5663 */ 9999,
/* 5664 */ 1195009,
/* 5665 */ 1196006,
/* 5666 */ 9999,
/* 5667 */ 9999,
/* 5668 */ 1195005,
/* 5669 */ 9999,
/* 5670 */ 9999,
/* 5671 */ 1195005,
/* 5672 */ 9999,
/* 5673 */ 1196008,
/* 5674 */ 1196018,
/* 5675 */ 1196008,
/* 5676 */ 9999,
/* 5677 */ 1196008,
/* 5678 */ 9999,
/* 5679 */ 9999,
/* 5680 */ 1196008,
/* 5681 */ 9999,
/* 5682 */ 9999,
/* 5683 */ 9999,
/* 5684 */ 1196009,
/* 5685 */ 9999,
/* 5686 */ 9999,
/* 5687 */ 9999,
/* 5688 */ 1196013,
/* 5689 */ 9999,
/* 5690 */ 9999,
/* 5691 */ 9999,
/* 5692 */ 9999,
/* 5693 */ 1203031,
/* 5694 */ 9999,
/* 5695 */ 1206007,
/* 5696 */ 9999,
/* 5697 */ 9999,
/* 5698 */ 1203011,
/* 5699 */ 9999,
/* 5700 */ 1206008,
/* 5701 */ 1206022,
/* 5702 */ 9999,
/* 5703 */ 1206022,
/* 5704 */ 9999,
/* 5705 */ 9999,
/* 5706 */ 1206022,
/* 5707 */ 9999,
/* 5708 */ 9999,
/* 5709 */ 1210012,
/* 5710 */ 1210019,
/* 5711 */ 9999,
/* 5712 */ 9999,
/* 5713 */ 1210009,
/* 5714 */ 9999,
/* 5715 */ 9999,
/* 5716 */ 1211011,
/* 5717 */ 1211019,
/* 5718 */ 9999,
/* 5719 */ 9999,
/* 5720 */ 1211009,
/* 5721 */ 9999,
/* 5722 */ 1212005,
/* 5723 */ 9999,
/* 5724 */ 1212014,
/* 5725 */ 9999,
/* 5726 */ 1213005,
/* 5727 */ 9999,
/* 5728 */ 1213016,
/* 5729 */ 9999,
/* 5730 */ 1214005,
/* 5731 */ 9999,
/* 5732 */ 1214017,
/* 5733 */ 9999,
/* 5734 */ 9999,
/* 5735 */ 9999,
/* 5736 */ 1210013,
/* 5737 */ 9999,
/* 5738 */ 9999,
/* 5739 */ 1210013,
/* 5740 */ 9999,
/* 5741 */ 9999,
/* 5742 */ 9999,
/* 5743 */ 1211012,
/* 5744 */ 9999,
/* 5745 */ 9999,
/* 5746 */ 1211012,
/* 5747 */ 9999,
/* 5748 */ 1211020,
/* 5749 */ 1211030,
/* 5750 */ 1211025,
/* 5751 */ 9999,
/* 5752 */ 1211025,
/* 5753 */ 9999,
/* 5754 */ 9999,
/* 5755 */ 1211025,
/* 5756 */ 9999,
/* 5757 */ 9999,
/* 5758 */ 9999,
/* 5759 */ 1211031,
/* 5760 */ 9999,
/* 5761 */ 9999,
/* 5762 */ 9999,
/* 5763 */ 1211037,
/* 5764 */ 9999,
/* 5765 */ 9999,
/* 5766 */ 1211037,
/* 5767 */ 9999,
/* 5768 */ 9999,
/* 5769 */ 9999,
/* 5770 */ 1221004,
/* 5771 */ 9999,
/* 5772 */ 9999,
/* 5773 */ 1218008,
/* 5774 */ 9999,
/* 5775 */ 9999,
/* 5776 */ 1221013,
/* 5777 */ 9999,
/* 5778 */ 1221013,
/* 5779 */ 9999,
/* 5780 */ 1222007,
/* 5781 */ 1222013,
/* 5782 */ 9999,
/* 5783 */ 1222013,
/* 5784 */ 9999,
/* 5785 */ 9999,
/* 5786 */ 1223007,
/* 5787 */ 9999,
/* 5788 */ 1223007,
/* 5789 */ 9999,
/* 5790 */ 9999,
/* 5791 */ 1224007,
/* 5792 */ 9999,
/* 5793 */ 1224007,
/* 5794 */ 9999,
/* 5795 */ 9999,
/* 5796 */ 1225007,
/* 5797 */ 9999,
/* 5798 */ 1225007,
/* 5799 */ 9999,
/* 5800 */ 9999,
/* 5801 */ 1226007,
/* 5802 */ 9999,
/* 5803 */ 1226007,
/* 5804 */ 9999,
/* 5805 */ 9999,
/* 5806 */ 1227011,
/* 5807 */ 9999,
/* 5808 */ 9999,
/* 5809 */ 1227007,
/* 5810 */ 9999,
/* 5811 */ 1227007,
/* 5812 */ 9999,
/* 5813 */ 9999,
/* 5814 */ 1230011,
/* 5815 */ 9999,
/* 5816 */ 1230007,
/* 5817 */ 9999,
/* 5818 */ 1230007,
/* 5819 */ 9999,
/* 5820 */ 9999,
/* 5821 */ 1230007,
/* 5822 */ 9999,
/* 5823 */ 9999,
/* 5824 */ 9999,
/* 5825 */ 1238018,
/* 5826 */ 9999,
/* 5827 */ 9999,
/* 5828 */ 9999,
/* 5829 */ 1241008,
/* 5830 */ 9999,
/* 5831 */ 9999,
/* 5832 */ 9999,
/* 5833 */ 1242008,
/* 5834 */ 9999,
/* 5835 */ 9999,
/* 5836 */ 9999,
/* 5837 */ 1243009,
/* 5838 */ 9999,
/* 5839 */ 9999,
/* 5840 */ 9999,
/* 5841 */ 1244010,
/* 5842 */ 9999,
/* 5843 */ 9999,
/* 5844 */ 9999,
/* 5845 */ 1245008,
/* 5846 */ 9999,
/* 5847 */ 9999,
/* 5848 */ 9999,
/* 5849 */ 1246009,
/* 5850 */ 9999,
/* 5851 */ 9999,
/* 5852 */ 9999,
/* 5853 */ 1247009,
/* 5854 */ 9999,
/* 5855 */ 9999,
/* 5856 */ 9999,
/* 5857 */ 1248008,
/* 5858 */ 9999,
/* 5859 */ 9999,
/* 5860 */ 9999,
/* 5861 */ 1249009,
/* 5862 */ 9999,
/* 5863 */ 9999,
/* 5864 */ 9999,
/* 5865 */ 1250007,
/* 5866 */ 9999,
/* 5867 */ 9999,
/* 5868 */ 9999,
/* 5869 */ 1251010,
/* 5870 */ 9999,
/* 5871 */ 9999,
/* 5872 */ 9999,
/* 5873 */ 1252013,
/* 5874 */ 9999,
/* 5875 */ 9999,
/* 5876 */ 1257010,
/* 5877 */ 1257024,
/* 5878 */ 9999,
/* 5879 */ 9999,
/* 5880 */ 9999,
/* 5881 */ 1274003,
/* 5882 */ 1282004,
/* 5883 */ 9999,
/* 5884 */ 9999,
/* 5885 */ 9999,
/* 5886 */ 1256015,
/* 5887 */ 9999,
/* 5888 */ 1274005,
/* 5889 */ 1274021,
/* 5890 */ 1276005,
/* 5891 */ 1276025,
/* 5892 */ 1278005,
/* 5893 */ 9999,
/* 5894 */ 1274013,
/* 5895 */ 9999,
/* 5896 */ 9999,
/* 5897 */ 1274013,
/* 5898 */ 9999,
/* 5899 */ 9999,
/* 5900 */ 1274026,
/* 5901 */ 9999,
/* 5902 */ 1274022,
/* 5903 */ 9999,
/* 5904 */ 9999,
/* 5905 */ 1274022,
/* 5906 */ 9999,
/* 5907 */ 9999,
/* 5908 */ 9999,
/* 5909 */ 1276010,
/* 5910 */ 9999,
/* 5911 */ 9999,
/* 5912 */ 1276010,
/* 5913 */ 9999,
/* 5914 */ 9999,
/* 5915 */ 1276031,
/* 5916 */ 9999,
/* 5917 */ 1276027,
/* 5918 */ 9999,
/* 5919 */ 9999,
/* 5920 */ 1276027,
/* 5921 */ 9999,
/* 5922 */ 9999,
/* 5923 */ 1278010,
/* 5924 */ 1278026,
/* 5925 */ 1280007,
/* 5926 */ 1280027,
/* 5927 */ 1278006,
/* 5928 */ 9999,
/* 5929 */ 1278006,
/* 5930 */ 9999,
/* 5931 */ 9999,
/* 5932 */ 1278006,
/* 5933 */ 9999,
/* 5934 */ 9999,
/* 5935 */ 1278031,
/* 5936 */ 9999,
/* 5937 */ 1278027,
/* 5938 */ 9999,
/* 5939 */ 9999,
/* 5940 */ 1278027,
/* 5941 */ 9999,
/* 5942 */ 9999,
/* 5943 */ 9999,
/* 5944 */ 1280012,
/* 5945 */ 9999,
/* 5946 */ 9999,
/* 5947 */ 1280012,
/* 5948 */ 9999,
/* 5949 */ 9999,
/* 5950 */ 1280033,
/* 5951 */ 9999,
/* 5952 */ 1280029,
/* 5953 */ 9999,
/* 5954 */ 9999,
/* 5955 */ 1280029,
/* 5956 */ 9999,
/* 5957 */ 9999,
/* 5958 */ 9999,
/* 5959 */ 9999,
/* 5960 */ 1282005,
/* 5961 */ 9999,
/* 5962 */ 9999,
/* 5963 */ 1282005,
/* 5964 */ 9999,
/* 5965 */ 9999,
/* 5966 */ 1286010,
/* 5967 */ 9999,
/* 5968 */ 9999,
/* 5969 */ 1293003,
/* 5970 */ 1301004,
/* 5971 */ 9999,
/* 5972 */ 9999,
/* 5973 */ 9999,
/* 5974 */ 1285016,
/* 5975 */ 9999,
/* 5976 */ 1293005,
/* 5977 */ 1293021,
/* 5978 */ 1295005,
/* 5979 */ 1295025,
/* 5980 */ 1297005,
/* 5981 */ 9999,
/* 5982 */ 1293013,
/* 5983 */ 9999,
/* 5984 */ 9999,
/* 5985 */ 1293013,
/* 5986 */ 9999,
/* 5987 */ 9999,
/* 5988 */ 1293026,
/* 5989 */ 9999,
/* 5990 */ 1293022,
/* 5991 */ 9999,
/* 5992 */ 9999,
/* 5993 */ 1293022,
/* 5994 */ 9999,
/* 5995 */ 9999,
/* 5996 */ 9999,
/* 5997 */ 1295010,
/* 5998 */ 9999,
/* 5999 */ 9999,
/* 6000 */ 1295010,
/* 6001 */ 9999,
/* 6002 */ 9999,
/* 6003 */ 1295031,
/* 6004 */ 9999,
/* 6005 */ 1295027,
/* 6006 */ 9999,
/* 6007 */ 9999,
/* 6008 */ 1295027,
/* 6009 */ 9999,
/* 6010 */ 9999,
/* 6011 */ 1297010,
/* 6012 */ 1297026,
/* 6013 */ 1299007,
/* 6014 */ 1299027,
/* 6015 */ 1297006,
/* 6016 */ 9999,
/* 6017 */ 1297006,
/* 6018 */ 9999,
/* 6019 */ 9999,
/* 6020 */ 1297006,
/* 6021 */ 9999,
/* 6022 */ 9999,
/* 6023 */ 1297031,
/* 6024 */ 9999,
/* 6025 */ 1297027,
/* 6026 */ 9999,
/* 6027 */ 9999,
/* 6028 */ 1297027,
/* 6029 */ 9999,
/* 6030 */ 9999,
/* 6031 */ 9999,
/* 6032 */ 1299012,
/* 6033 */ 9999,
/* 6034 */ 9999,
/* 6035 */ 1299012,
/* 6036 */ 9999,
/* 6037 */ 9999,
/* 6038 */ 1299033,
/* 6039 */ 9999,
/* 6040 */ 1299029,
/* 6041 */ 9999,
/* 6042 */ 9999,
/* 6043 */ 1299029,
/* 6044 */ 9999,
/* 6045 */ 9999,
/* 6046 */ 9999,
/* 6047 */ 9999,
/* 6048 */ 1301005,
/* 6049 */ 9999,
/* 6050 */ 9999,
/* 6051 */ 1301005,
/* 6052 */ 9999,
/* 6053 */ 9999,
/* 6054 */ 1305010,
/* 6055 */ 1305024,
/* 6056 */ 9999,
/* 6057 */ 9999,
/* 6058 */ 9999,
/* 6059 */ 9999,
/* 6060 */ 1304016,
/* 6061 */ 9999,
/* 6062 */ 9999,
/* 6063 */ 1338010,
/* 6064 */ 9999,
/* 6065 */ 9999,
/* 6066 */ 1338040,
/* 6067 */ 9999,
/* 6068 */ 9999,
/* 6069 */ 1337016,
/* 6070 */ 9999,
/* 6071 */ 1354003,
/* 6072 */ 9999,
/* 6073 */ 1369003,
/* 6074 */ 9999,
/* 6075 */ 9999,
/* 6076 */ 1353016,
/* 6077 */ 9999,
/* 6078 */ 9999,
/* 6079 */ 9999,
/* 6080 */ 1369004,
/* 6081 */ 9999,
/* 6082 */ 1369004,
/* 6083 */ 9999,
/* 6084 */ 9999,
/* 6085 */ 1369004,
/* 6086 */ 9999,
/* 6087 */ 1384003,
/* 6088 */ 9999,
/* 6089 */ 1401003,
/* 6090 */ 9999,
/* 6091 */ 9999,
/* 6092 */ 1383029,
/* 6093 */ 9999,
/* 6094 */ 9999,
/* 6095 */ 9999,
/* 6096 */ 1401004,
/* 6097 */ 9999,
/* 6098 */ 1401004,
/* 6099 */ 9999,
/* 6100 */ 9999,
/* 6101 */ 1401004,
/* 6102 */ 9999,
/* 6103 */ 1421003,
/* 6104 */ 9999,
/* 6105 */ 1433003,
/* 6106 */ 9999,
/* 6107 */ 9999,
/* 6108 */ 1420016,
/* 6109 */ 9999,
/* 6110 */ 9999,
/* 6111 */ 9999,
/* 6112 */ 1433004,
/* 6113 */ 9999,
/* 6114 */ 1433004,
/* 6115 */ 9999,
/* 6116 */ 9999,
/* 6117 */ 1433004,
/* 6118 */ 9999,
/* 6119 */ 1447003,
/* 6120 */ 9999,
/* 6121 */ 9999,
/* 6122 */ 1447006,
/* 6123 */ 9999,
/* 6124 */ 9999,
/* 6125 */ 9999,
/* 6126 */ 1458012,
/* 6127 */ 9999,
/* 6128 */ 1458025,
/* 6129 */ 9999,
/* 6130 */ 1458045,
/* 6131 */ 9999,
/* 6132 */ 1458006,
/* 6133 */ 9999,
/* 6134 */ 9999,
/* 6135 */ 9999,
/* 6136 */ 1473012,
/* 6137 */ 9999,
/* 6138 */ 1473025,
/* 6139 */ 9999,
/* 6140 */ 1473006,
/* 6141 */ 9999,
/* 6142 */ 9999,
/* 6143 */ 9999,
/* 6144 */ 1482013,
/* 6145 */ 9999,
/* 6146 */ 1482027,
/* 6147 */ 9999,
/* 6148 */ 1482041,
/* 6149 */ 9999,
/* 6150 */ 1483007,
/* 6151 */ 9999,
/* 6152 */ 1482007,
/* 6153 */ 9999,
/* 6154 */ 9999,
/* 6155 */ 9999,
/* 6156 */ 1509016,
/* 6157 */ 9999,
/* 6158 */ 9999,
/* 6159 */ 1519005,
/* 6160 */ 1530005,
/* 6161 */ 9999,
/* 6162 */ 9999,
/* 6163 */ 1509010,
/* 6164 */ 9999,
/* 6165 */ 9999,
/* 6166 */ 9999,
/* 6167 */ 9999,
/* 6168 */ 1579009,
/* 6169 */ 9999,
/* 6170 */ 9999,
/* 6171 */ 9999,
/* 6172 */ 9999,
/* 6173 */ 1583012,
/* 6174 */ 9999,
/* 6175 */ 9999,
/* 6176 */ 1587008,
/* 6177 */ 9999,
/* 6178 */ 9999,
/* 6179 */ 1587033,
/* 6180 */ 9999,
/* 6181 */ 9999,
/* 6182 */ 9999,
/* 6183 */ 1587006,
/* 6184 */ 9999,
/* 6185 */ 9999,
/* 6186 */ 9999,
/* 6187 */ 1596015,
/* 6188 */ 9999,
/* 6189 */ 1596028,
/* 6190 */ 9999,
/* 6191 */ 1596009,
/* 6192 */ 9999,
/* 6193 */ 1607005,
/* 6194 */ 9999,
/* 6195 */ 1607009,
/* 6196 */ 9999,
/* 6197 */ 9999,
/* 6198 */ 1611012,
/* 6199 */ 9999,
/* 6200 */ 9999,
/* 6201 */ 1611010,
/* 6202 */ 9999,
/* 6203 */ 9999,
/* 6204 */ 9999,
/* 6205 */ 9999,
/* 6206 */ 1624010,
/* 6207 */ 9999,
/* 6208 */ 9999,
/* 6209 */ 1519012,
/* 6210 */ 9999,
/* 6211 */ 1521007,
/* 6212 */ 1519010,
/* 6213 */ 9999,
/* 6214 */ 1519010,
/* 6215 */ 9999,
/* 6216 */ 9999,
/* 6217 */ 1519010,
/* 6218 */ 9999,
/* 6219 */ 1519014,
/* 6220 */ 9999,
/* 6221 */ 1519018,
/* 6222 */ 9999,
/* 6223 */ 1519027,
/* 6224 */ 9999,
/* 6225 */ 1519033,
/* 6226 */ 9999,
/* 6227 */ 1521027,
/* 6228 */ 1521009,
/* 6229 */ 9999,
/* 6230 */ 1521009,
/* 6231 */ 9999,
/* 6232 */ 9999,
/* 6233 */ 1521009,
/* 6234 */ 9999,
/* 6235 */ 9999,
/* 6236 */ 9999,
/* 6237 */ 1531007,
/* 6238 */ 9999,
/* 6239 */ 1530013,
/* 6240 */ 9999,
/* 6241 */ 9999,
/* 6242 */ 1530013,
/* 6243 */ 9999,
/* 6244 */ 1531027,
/* 6245 */ 1531009,
/* 6246 */ 9999,
/* 6247 */ 1531009,
/* 6248 */ 9999,
/* 6249 */ 9999,
/* 6250 */ 1531009,
/* 6251 */ 9999,
/* 6252 */ 9999,
/* 6253 */ 1633007,
/* 6254 */ 9999,
/* 6255 */ 9999,
/* 6256 */ 1633003,
/* 6257 */ 9999,
/* 6258 */ 1633025,
/* 6259 */ 1633009,
/* 6260 */ 9999,
/* 6261 */ 1633009,
/* 6262 */ 9999,
/* 6263 */ 9999,
/* 6264 */ 1633009,
/* 6265 */ 9999,
/* 6266 */ 9999,
/* 6267 */ 1638007,
/* 6268 */ 1643007,
/* 6269 */ 9999,
/* 6270 */ 9999,
/* 6271 */ 1637004,
/* 6272 */ 9999,
/* 6273 */ 1638027,
/* 6274 */ 1638009,
/* 6275 */ 9999,
/* 6276 */ 1638009,
/* 6277 */ 9999,
/* 6278 */ 9999,
/* 6279 */ 1639018,
/* 6280 */ 1639016,
/* 6281 */ 9999,
/* 6282 */ 1639016,
/* 6283 */ 9999,
/* 6284 */ 9999,
/* 6285 */ 1639016,
/* 6286 */ 9999,
/* 6287 */ 1643025,
/* 6288 */ 1643009,
/* 6289 */ 9999,
/* 6290 */ 1643009,
/* 6291 */ 9999,
/* 6292 */ 9999,
/* 6293 */ 1643009,
/* 6294 */ 9999,
/* 6295 */ 9999,
/* 6296 */ 9999,
/* 6297 */ 1679010,
/* 6298 */ 9999,
/* 6299 */ 9999,
/* 6300 */ 9999,
/* 6301 */ 1681014,
/* 6302 */ 9999,
/* 6303 */ 9999,
/* 6304 */ 9999,
/* 6305 */ 1683011,
/* 6306 */ 9999,
/* 6307 */ 9999,
/* 6308 */ 9999,
/* 6309 */ 1685013,
/* 6310 */ 9999,
/* 6311 */ 9999,
/* 6312 */ 9999,
/* 6313 */ 1687014,
/* 6314 */ 9999,
/* 6315 */ 9999,
/* 6316 */ 9999,
/* 6317 */ 1689013,
/* 6318 */ 9999,
/* 6319 */ 1691005,
/* 6320 */ 9999,
/* 6321 */ 1691011,
/* 6322 */ 9999,
/* 6323 */ 9999,
/* 6324 */ 1693009,
/* 6325 */ 9999,
/* 6326 */ 9999,
/* 6327 */ 1693005,
/* 6328 */ 9999,
/* 6329 */ 9999,
/* 6330 */ 9999,
/* 6331 */ 1696009,
/* 6332 */ 9999,
/* 6333 */ 1737005,
/* 6334 */ 1737020,
/* 6335 */ 9999,
/* 6336 */ 9999,
/* 6337 */ 1737013,
/* 6338 */ 9999,
/* 6339 */ 1794005,
/* 6340 */ 1794020,
/* 6341 */ 9999,
/* 6342 */ 9999,
/* 6343 */ 1821009,
/* 6344 */ 9999,
/* 6345 */ 9999,
/* 6346 */ 1794013,
/* 6347 */ 9999,
/* 6348 */ 9999,
/* 6349 */ 9999,
/* 6350 */ 1856009,
/* 6351 */ 9999,
/* 6352 */ 9999,
/* 6353 */ 1849009,
/* 6354 */ 9999,
/* 6355 */ 1875005,
/* 6356 */ 9999,
/* 6357 */ 9999,
/* 6358 */ 9999,
/* 6359 */ 1875013,
/* 6360 */ 9999,
/* 6361 */ 9999,
/* 6362 */ 9999,
/* 6363 */ 1737021,
/* 6364 */ 9999,
/* 6365 */ 9999,
/* 6366 */ 9999,
/* 6367 */ 1737033,
/* 6368 */ 9999,
/* 6369 */ 9999,
/* 6370 */ 9999,
/* 6371 */ 1794021,
/* 6372 */ 9999,
/* 6373 */ 9999,
/* 6374 */ 9999,
/* 6375 */ 1794033,
/* 6376 */ 9999,
/* 6377 */ 9999,
/* 6378 */ 9999,
/* 6379 */ 1885007,
/* 6380 */ 9999,
/* 6381 */ 1930005,
/* 6382 */ 9999,
/* 6383 */ 9999,
/* 6384 */ 9999,
/* 6385 */ 1930010,
/* 6386 */ 9999,
/* 6387 */ 1936005,
/* 6388 */ 1936020,
/* 6389 */ 9999,
/* 6390 */ 9999,
/* 6391 */ 1936013,
/* 6392 */ 9999,
/* 6393 */ 9999,
/* 6394 */ 9999,
/* 6395 */ 1936021,
/* 6396 */ 9999,
/* 6397 */ 9999,
/* 6398 */ 9999,
/* 6399 */ 1936033,
/* 6400 */ 9999,
/* 6401 */ 1979005,
/* 6402 */ 9999,
/* 6403 */ 9999,
/* 6404 */ 1979010,
/* 6405 */ 9999,
/* 6406 */ 1981005,
/* 6407 */ 9999,
/* 6408 */ 9999,
/* 6409 */ 1981010,
/* 6410 */ 9999,
/* 6411 */ 9999,
/* 6412 */ 9999,
/* 6413 */ 1983018,
/* 6414 */ 9999,
/* 6415 */ 1983035,
/* 6416 */ 9999,
/* 6417 */ 9999,
/* 6418 */ 1983012,
/* 6419 */ 9999,
/* 6420 */ 9999,
/* 6421 */ 9999,
/* 6422 */ 1985018,
/* 6423 */ 9999,
/* 6424 */ 1985035,
/* 6425 */ 9999,
/* 6426 */ 9999,
/* 6427 */ 1985012,
/* 6428 */ 9999,
/* 6429 */ 1987005,
/* 6430 */ 9999,
/* 6431 */ 1987013,
/* 6432 */ 9999,
/* 6433 */ 9999,
/* 6434 */ 1992018,
/* 6435 */ 9999,
/* 6436 */ 1992016,
/* 6437 */ 9999,
/* 6438 */ 9999,
/* 6439 */ 1994018,
/* 6440 */ 9999,
/* 6441 */ 1994016,
/* 6442 */ 9999,
/* 6443 */ 9999,
/* 6444 */ 1996009,
/* 6445 */ 9999,
/* 6446 */ 1996005,
/* 6447 */ 9999,
/* 6448 */ 9999,
/* 6449 */ 1998009,
/* 6450 */ 9999,
/* 6451 */ 1998005,
/* 6452 */ 9999,
/* 6453 */ 9999,
/* 6454 */ 2000009,
/* 6455 */ 9999,
/* 6456 */ 2000005,
/* 6457 */ 9999,
/* 6458 */ 9999,
/* 6459 */ 2002009,
/* 6460 */ 9999,
/* 6461 */ 2002005,
/* 6462 */ 9999,
/* 6463 */ 9999,
/* 6464 */ 2004009,
/* 6465 */ 9999,
/* 6466 */ 9999,
/* 6467 */ 2004005,
/* 6468 */ 9999,
/* 6469 */ 2006005,
/* 6470 */ 9999,
/* 6471 */ 2006010,
/* 6472 */ 9999,
/* 6473 */ 2011005,
/* 6474 */ 9999,
/* 6475 */ 2011021,
/* 6476 */ 9999,
/* 6477 */ 2011010,
/* 6478 */ 9999,
/* 6479 */ 2013005,
/* 6480 */ 9999,
/* 6481 */ 2013010,
/* 6482 */ 9999,
/* 6483 */ 9999,
/* 6484 */ 2018009,
/* 6485 */ 9999,
/* 6486 */ 2018027,
/* 6487 */ 9999,
/* 6488 */ 2018005,
/* 6489 */ 9999,
/* 6490 */ 2020005,
/* 6491 */ 9999,
/* 6492 */ 2020010,
/* 6493 */ 9999,
/* 6494 */ 2026005,
/* 6495 */ 9999,
/* 6496 */ 2026021,
/* 6497 */ 9999,
/* 6498 */ 2026010,
/* 6499 */ 9999,
/* 6500 */ 2028005,
/* 6501 */ 9999,
/* 6502 */ 2028021,
/* 6503 */ 9999,
/* 6504 */ 2028010,
/* 6505 */ 9999,
/* 6506 */ 2030005,
/* 6507 */ 9999,
/* 6508 */ 2030021,
/* 6509 */ 9999,
/* 6510 */ 2030010,
/* 6511 */ 9999,
/* 6512 */ 2032005,
/* 6513 */ 9999,
/* 6514 */ 2032010,
/* 6515 */ 9999,
/* 6516 */ 2037005,
/* 6517 */ 9999,
/* 6518 */ 2037021,
/* 6519 */ 9999,
/* 6520 */ 2037010,
/* 6521 */ 9999,
/* 6522 */ 2039005,
/* 6523 */ 9999,
/* 6524 */ 2039021,
/* 6525 */ 9999,
/* 6526 */ 2039010,
/* 6527 */ 9999,
/* 6528 */ 2041005,
/* 6529 */ 9999,
/* 6530 */ 2041010,
/* 6531 */ 9999,
/* 6532 */ 2046005,
/* 6533 */ 9999,
/* 6534 */ 2046027,
/* 6535 */ 9999,
/* 6536 */ 2046010,
/* 6537 */ 9999,
/* 6538 */ 2048005,
/* 6539 */ 9999,
/* 6540 */ 2048027,
/* 6541 */ 9999,
/* 6542 */ 2048010,
/* 6543 */ 9999,
/* 6544 */ 2050005,
/* 6545 */ 9999,
/* 6546 */ 2050010,
/* 6547 */ 9999,
/* 6548 */ 2059005,
/* 6549 */ 9999,
/* 6550 */ 2059021,
/* 6551 */ 9999,
/* 6552 */ 2059010,
/* 6553 */ 9999,
/* 6554 */ 2061005,
/* 6555 */ 9999,
/* 6556 */ 2061021,
/* 6557 */ 9999,
/* 6558 */ 2061010,
/* 6559 */ 9999,
/* 6560 */ 2063005,
/* 6561 */ 9999,
/* 6562 */ 2063023,
/* 6563 */ 9999,
/* 6564 */ 2063010,
/* 6565 */ 9999,
/* 6566 */ 2065005,
/* 6567 */ 9999,
/* 6568 */ 2065023,
/* 6569 */ 9999,
/* 6570 */ 2065010,
/* 6571 */ 9999,
/* 6572 */ 2067005,
/* 6573 */ 9999,
/* 6574 */ 2067023,
/* 6575 */ 9999,
/* 6576 */ 2067010,
/* 6577 */ 9999,
/* 6578 */ 2069005,
/* 6579 */ 9999,
/* 6580 */ 2069023,
/* 6581 */ 9999,
/* 6582 */ 2069010,
/* 6583 */ 9999,
/* 6584 */ 2071005,
/* 6585 */ 9999,
/* 6586 */ 2071010,
/* 6587 */ 9999,
/* 6588 */ 2076005,
/* 6589 */ 9999,
/* 6590 */ 2076021,
/* 6591 */ 9999,
/* 6592 */ 2076010,
/* 6593 */ 9999,
/* 6594 */ 2078005,
/* 6595 */ 9999,
/* 6596 */ 2078010,
/* 6597 */ 9999,
/* 6598 */ 2083005,
/* 6599 */ 9999,
/* 6600 */ 2083022,
/* 6601 */ 9999,
/* 6602 */ 2083011,
/* 6603 */ 9999,
/* 6604 */ 2085005,
/* 6605 */ 9999,
/* 6606 */ 2085010,
/* 6607 */ 9999,
/* 6608 */ 2090005,
/* 6609 */ 9999,
/* 6610 */ 2090022,
/* 6611 */ 9999,
/* 6612 */ 2090011,
/* 6613 */ 9999,
/* 6614 */ 2092005,
/* 6615 */ 9999,
/* 6616 */ 2092011,
/* 6617 */ 9999,
/* 6618 */ 2097005,
/* 6619 */ 9999,
/* 6620 */ 2097025,
/* 6621 */ 9999,
/* 6622 */ 2097011,
/* 6623 */ 9999,
/* 6624 */ 2099005,
/* 6625 */ 9999,
/* 6626 */ 2099011,
/* 6627 */ 9999,
/* 6628 */ 2104005,
/* 6629 */ 9999,
/* 6630 */ 2104024,
/* 6631 */ 9999,
/* 6632 */ 2104011,
/* 6633 */ 9999,
/* 6634 */ 2106005,
/* 6635 */ 9999,
/* 6636 */ 2106011,
/* 6637 */ 9999,
/* 6638 */ 2111003,
/* 6639 */ 9999,
/* 6640 */ 2111020,
/* 6641 */ 9999,
/* 6642 */ 2111034,
/* 6643 */ 9999,
/* 6644 */ 2111009,
/* 6645 */ 9999,
/* 6646 */ 2161005,
/* 6647 */ 9999,
/* 6648 */ 2161011,
/* 6649 */ 9999,
/* 6650 */ 2169005,
/* 6651 */ 9999,
/* 6652 */ 2169020,
/* 6653 */ 9999,
/* 6654 */ 2169010,
/* 6655 */ 9999,
/* 6656 */ 2171005,
/* 6657 */ 9999,
/* 6658 */ 2171027,
/* 6659 */ 9999,
/* 6660 */ 2171010,
/* 6661 */ 9999,
/* 6662 */ 2173005,
/* 6663 */ 9999,
/* 6664 */ 2173027,
/* 6665 */ 9999,
/* 6666 */ 2173010,
/* 6667 */ 9999,
/* 6668 */ 2175005,
/* 6669 */ 9999,
/* 6670 */ 2175028,
/* 6671 */ 9999,
/* 6672 */ 2175010,
/* 6673 */ 9999,
/* 6674 */ 2177005,
/* 6675 */ 9999,
/* 6676 */ 2177027,
/* 6677 */ 9999,
/* 6678 */ 2177010,
/* 6679 */ 9999,
/* 6680 */ 2179005,
/* 6681 */ 9999,
/* 6682 */ 2179027,
/* 6683 */ 9999,
/* 6684 */ 2179010,
/* 6685 */ 9999,
/* 6686 */ 2181005,
/* 6687 */ 9999,
/* 6688 */ 2181027,
/* 6689 */ 9999,
/* 6690 */ 2181010,
/* 6691 */ 9999,
/* 6692 */ 2183005,
/* 6693 */ 9999,
/* 6694 */ 2183026,
/* 6695 */ 9999,
/* 6696 */ 2183010,
/* 6697 */ 9999,
/* 6698 */ 2185005,
/* 6699 */ 9999,
/* 6700 */ 2185030,
/* 6701 */ 9999,
/* 6702 */ 2185010,
/* 6703 */ 9999,
/* 6704 */ 2187005,
/* 6705 */ 9999,
/* 6706 */ 2187030,
/* 6707 */ 9999,
/* 6708 */ 2187010,
/* 6709 */ 9999,
/* 6710 */ 2189005,
/* 6711 */ 9999,
/* 6712 */ 2189027,
/* 6713 */ 9999,
/* 6714 */ 2189010,
/* 6715 */ 9999,
/* 6716 */ 2191005,
/* 6717 */ 9999,
/* 6718 */ 2191011,
/* 6719 */ 9999,
/* 6720 */ 2196003,
/* 6721 */ 9999,
/* 6722 */ 2196017,
/* 6723 */ 9999,
/* 6724 */ 2196006,
/* 6725 */ 9999,
/* 6726 */ 2209005,
/* 6727 */ 9999,
/* 6728 */ 2209011,
/* 6729 */ 9999,
/* 6730 */ 2220003,
/* 6731 */ 9999,
/* 6732 */ 2218003,
/* 6733 */ 9999,
/* 6734 */ 2220005,
/* 6735 */ 2247003,
/* 6736 */ 9999,
/* 6737 */ 2220011,
/* 6738 */ 9999,
/* 6739 */ 9999,
/* 6740 */ 2220011,
/* 6741 */ 9999,
/* 6742 */ 9999,
/* 6743 */ 2247009,
/* 6744 */ 2247005,
/* 6745 */ 9999,
/* 6746 */ 2247005,
/* 6747 */ 9999,
/* 6748 */ 9999,
/* 6749 */ 2247005,
0
};
/* only for BIGHASH (see art.c)
extern int DHITS[];
int DHITS[6751];
*/
int TABLE[1426][497];
void init_dirsets() {
TABLE[479][0] = 1;
TABLE[479][380] = 1;
TABLE[479][433] = 1;
TABLE[479][352] = 1;
TABLE[479][350] = 1;
TABLE[479][451] = 1;
TABLE[479][388] = 1;
TABLE[479][372] = 1;
TABLE[479][348] = 1;
TABLE[479][293] = 1;
TABLE[479][332] = 1;
TABLE[479][261] = 1;
TABLE[479][418] = 1;
TABLE[479][309] = 1;
TABLE[479][358] = 1;
TABLE[479][328] = 1;
TABLE[479][422] = 1;
TABLE[479][450] = 1;
TABLE[479][449] = 1;
TABLE[479][341] = 1;
TABLE[479][299] = 1;
TABLE[479][336] = 1;
TABLE[479][447] = 1;
TABLE[479][264] = 1;
TABLE[479][274] = 1;
TABLE[479][268] = 1;
TABLE[479][269] = 1;
TABLE[479][265] = 1;
TABLE[479][266] = 1;
TABLE[479][267] = 1;
TABLE[479][263] = 1;
TABLE[479][271] = 1;
TABLE[479][272] = 1;
TABLE[479][273] = 1;
TABLE[479][474] = 1;
TABLE[479][436] = 1;
TABLE[479][334] = 1;
TABLE[479][409] = 1;
TABLE[479][330] = 1;
TABLE[479][45] = 1;
TABLE[479][43] = 1;
TABLE[479][40] = 1;
TABLE[479][256] = 1;
TABLE[479][458] = 1;
TABLE[479][345] = 1;
TABLE[479][259] = 1;
TABLE[1][0] = 1;
TABLE[1][451] = 1;
TABLE[1][350] = 1;
TABLE[1][352] = 1;
TABLE[1][433] = 1;
TABLE[1][380] = 1;
TABLE[1][474] = 1;
TABLE[1][273] = 1;
TABLE[1][272] = 1;
TABLE[1][271] = 1;
TABLE[1][263] = 1;
TABLE[1][267] = 1;
TABLE[1][266] = 1;
TABLE[1][265] = 1;
TABLE[1][269] = 1;
TABLE[1][268] = 1;
TABLE[1][274] = 1;
TABLE[1][264] = 1;
TABLE[1][447] = 1;
TABLE[1][336] = 1;
TABLE[1][299] = 1;
TABLE[1][341] = 1;
TABLE[1][449] = 1;
TABLE[1][450] = 1;
TABLE[1][422] = 1;
TABLE[1][328] = 1;
TABLE[1][358] = 1;
TABLE[1][309] = 1;
TABLE[1][418] = 1;
TABLE[1][261] = 1;
TABLE[1][332] = 1;
TABLE[1][293] = 1;
TABLE[1][348] = 1;
TABLE[1][372] = 1;
TABLE[1][388] = 1;
TABLE[1][43] = 1;
TABLE[1][45] = 1;
TABLE[1][330] = 1;
TABLE[1][409] = 1;
TABLE[1][334] = 1;
TABLE[1][436] = 1;
TABLE[1][259] = 1;
TABLE[1][345] = 1;
TABLE[1][458] = 1;
TABLE[1][256] = 1;
TABLE[1][40] = 1;
TABLE[480][0] = 1;
TABLE[480][380] = 1;
TABLE[480][433] = 1;
TABLE[480][352] = 1;
TABLE[480][350] = 1;
TABLE[480][451] = 1;
TABLE[480][388] = 1;
TABLE[480][372] = 1;
TABLE[480][348] = 1;
TABLE[480][293] = 1;
TABLE[480][332] = 1;
TABLE[480][261] = 1;
TABLE[480][418] = 1;
TABLE[480][309] = 1;
TABLE[480][358] = 1;
TABLE[480][328] = 1;
TABLE[480][422] = 1;
TABLE[480][450] = 1;
TABLE[480][449] = 1;
TABLE[480][341] = 1;
TABLE[480][299] = 1;
TABLE[480][336] = 1;
TABLE[480][447] = 1;
TABLE[480][264] = 1;
TABLE[480][274] = 1;
TABLE[480][268] = 1;
TABLE[480][269] = 1;
TABLE[480][265] = 1;
TABLE[480][266] = 1;
TABLE[480][267] = 1;
TABLE[480][263] = 1;
TABLE[480][271] = 1;
TABLE[480][272] = 1;
TABLE[480][273] = 1;
TABLE[480][474] = 1;
TABLE[480][436] = 1;
TABLE[480][334] = 1;
TABLE[480][409] = 1;
TABLE[480][330] = 1;
TABLE[480][45] = 1;
TABLE[480][43] = 1;
TABLE[480][40] = 1;
TABLE[480][256] = 1;
TABLE[480][458] = 1;
TABLE[480][345] = 1;
TABLE[480][259] = 1;
TABLE[1024][0] = 1;
TABLE[2][380] = 1;
TABLE[3][433] = 1;
TABLE[4][352] = 1;
TABLE[5][388] = 1;
TABLE[6][372] = 1;
TABLE[7][348] = 1;
TABLE[8][293] = 1;
TABLE[9][332] = 1;
TABLE[10][261] = 1;
TABLE[11][451] = 1;
TABLE[11][350] = 1;
TABLE[11][352] = 1;
TABLE[11][433] = 1;
TABLE[11][380] = 1;
TABLE[11][474] = 1;
TABLE[11][273] = 1;
TABLE[11][272] = 1;
TABLE[11][271] = 1;
TABLE[11][263] = 1;
TABLE[11][267] = 1;
TABLE[11][266] = 1;
TABLE[11][265] = 1;
TABLE[11][269] = 1;
TABLE[11][268] = 1;
TABLE[11][274] = 1;
TABLE[11][264] = 1;
TABLE[11][447] = 1;
TABLE[11][336] = 1;
TABLE[11][299] = 1;
TABLE[11][341] = 1;
TABLE[11][449] = 1;
TABLE[11][450] = 1;
TABLE[11][422] = 1;
TABLE[11][328] = 1;
TABLE[11][358] = 1;
TABLE[11][309] = 1;
TABLE[11][418] = 1;
TABLE[11][261] = 1;
TABLE[11][332] = 1;
TABLE[11][293] = 1;
TABLE[11][348] = 1;
TABLE[11][372] = 1;
TABLE[11][388] = 1;
TABLE[11][0] = 1;
TABLE[11][43] = 1;
TABLE[11][45] = 1;
TABLE[11][330] = 1;
TABLE[11][409] = 1;
TABLE[11][334] = 1;
TABLE[11][436] = 1;
TABLE[11][259] = 1;
TABLE[11][345] = 1;
TABLE[11][458] = 1;
TABLE[11][256] = 1;
TABLE[11][40] = 1;
TABLE[12][418] = 1;
TABLE[13][309] = 1;
TABLE[14][358] = 1;
TABLE[15][328] = 1;
TABLE[16][422] = 1;
TABLE[17][450] = 1;
TABLE[18][449] = 1;
TABLE[19][341] = 1;
TABLE[20][299] = 1;
TABLE[21][336] = 1;
TABLE[22][261] = 1;
TABLE[23][447] = 1;
TABLE[24][447] = 1;
TABLE[25][474] = 1;
TABLE[25][273] = 1;
TABLE[25][272] = 1;
TABLE[25][271] = 1;
TABLE[25][263] = 1;
TABLE[25][267] = 1;
TABLE[25][266] = 1;
TABLE[25][265] = 1;
TABLE[25][269] = 1;
TABLE[25][268] = 1;
TABLE[25][274] = 1;
TABLE[25][264] = 1;
TABLE[26][447] = 1;
TABLE[27][447] = 1;
TABLE[28][350] = 1;
TABLE[29][451] = 1;
TABLE[481][58] = 1;
TABLE[482][326] = 1;
TABLE[1025][257] = 1;
TABLE[483][59] = 1;
TABLE[1026][125] = 1;
TABLE[30][36] = 1;
TABLE[31][36] = 1;
TABLE[32][43] = 1;
TABLE[32][45] = 1;
TABLE[32][259] = 1;
TABLE[32][345] = 1;
TABLE[32][458] = 1;
TABLE[32][256] = 1;
TABLE[32][40] = 1;
TABLE[33][43] = 1;
TABLE[33][45] = 1;
TABLE[33][259] = 1;
TABLE[33][345] = 1;
TABLE[33][458] = 1;
TABLE[33][256] = 1;
TABLE[33][40] = 1;
TABLE[34][45] = 1;
TABLE[34][43] = 1;
TABLE[34][40] = 1;
TABLE[34][256] = 1;
TABLE[34][458] = 1;
TABLE[34][345] = 1;
TABLE[34][259] = 1;
TABLE[35][45] = 1;
TABLE[35][43] = 1;
TABLE[35][40] = 1;
TABLE[35][256] = 1;
TABLE[35][458] = 1;
TABLE[35][345] = 1;
TABLE[35][259] = 1;
TABLE[36][45] = 1;
TABLE[36][43] = 1;
TABLE[36][40] = 1;
TABLE[36][256] = 1;
TABLE[36][458] = 1;
TABLE[36][345] = 1;
TABLE[36][259] = 1;
TABLE[37][45] = 1;
TABLE[37][43] = 1;
TABLE[37][40] = 1;
TABLE[37][256] = 1;
TABLE[37][458] = 1;
TABLE[37][345] = 1;
TABLE[37][259] = 1;
TABLE[38][45] = 1;
TABLE[38][43] = 1;
TABLE[38][40] = 1;
TABLE[38][256] = 1;
TABLE[38][458] = 1;
TABLE[38][345] = 1;
TABLE[38][259] = 1;
TABLE[39][45] = 1;
TABLE[39][43] = 1;
TABLE[39][40] = 1;
TABLE[39][256] = 1;
TABLE[39][458] = 1;
TABLE[39][345] = 1;
TABLE[39][259] = 1;
TABLE[40][43] = 1;
TABLE[40][45] = 1;
TABLE[40][259] = 1;
TABLE[40][345] = 1;
TABLE[40][458] = 1;
TABLE[40][256] = 1;
TABLE[40][40] = 1;
TABLE[41][43] = 1;
TABLE[42][45] = 1;
TABLE[43][40] = 1;
TABLE[43][256] = 1;
TABLE[43][458] = 1;
TABLE[43][345] = 1;
TABLE[43][259] = 1;
TABLE[44][40] = 1;
TABLE[45][256] = 1;
TABLE[46][458] = 1;
TABLE[47][345] = 1;
TABLE[48][259] = 1;
TABLE[49][40] = 1;
TABLE[49][256] = 1;
TABLE[49][458] = 1;
TABLE[49][345] = 1;
TABLE[49][259] = 1;
TABLE[50][40] = 1;
TABLE[50][256] = 1;
TABLE[50][458] = 1;
TABLE[50][345] = 1;
TABLE[50][259] = 1;
TABLE[51][40] = 1;
TABLE[51][256] = 1;
TABLE[51][458] = 1;
TABLE[51][345] = 1;
TABLE[51][259] = 1;
TABLE[52][43] = 1;
TABLE[52][45] = 1;
TABLE[52][259] = 1;
TABLE[52][345] = 1;
TABLE[52][458] = 1;
TABLE[52][256] = 1;
TABLE[52][40] = 1;
TABLE[53][40] = 1;
TABLE[53][256] = 1;
TABLE[53][458] = 1;
TABLE[53][345] = 1;
TABLE[53][259] = 1;
TABLE[54][40] = 1;
TABLE[54][256] = 1;
TABLE[54][458] = 1;
TABLE[54][345] = 1;
TABLE[54][259] = 1;
TABLE[55][36] = 1;
TABLE[55][419] = 1;
TABLE[55][317] = 1;
TABLE[55][300] = 1;
TABLE[55][40] = 1;
TABLE[55][458] = 1;
TABLE[55][345] = 1;
TABLE[55][259] = 1;
TABLE[55][256] = 1;
TABLE[55][367] = 1;
TABLE[484][367] = 1;
TABLE[1027][40] = 1;
TABLE[1027][256] = 1;
TABLE[1027][458] = 1;
TABLE[1027][345] = 1;
TABLE[1027][259] = 1;
TABLE[1027][317] = 1;
TABLE[1027][300] = 1;
TABLE[1027][36] = 1;
TABLE[1027][419] = 1;
TABLE[485][256] = 1;
TABLE[1028][40] = 1;
TABLE[1028][256] = 1;
TABLE[1028][458] = 1;
TABLE[1028][345] = 1;
TABLE[1028][259] = 1;
TABLE[1028][317] = 1;
TABLE[1028][300] = 1;
TABLE[1028][36] = 1;
TABLE[1028][419] = 1;
TABLE[486][259] = 1;
TABLE[486][345] = 1;
TABLE[486][458] = 1;
TABLE[486][256] = 1;
TABLE[486][40] = 1;
TABLE[1029][317] = 1;
TABLE[1029][300] = 1;
TABLE[1029][36] = 1;
TABLE[1029][419] = 1;
TABLE[487][317] = 1;
TABLE[488][300] = 1;
TABLE[1030][36] = 1;
TABLE[1030][419] = 1;
TABLE[489][36] = 1;
TABLE[490][419] = 1;
TABLE[491][43] = 1;
TABLE[492][45] = 1;
TABLE[1031][471] = 1;
TABLE[1031][399] = 1;
TABLE[1031][295] = 1;
TABLE[1031][59] = 1;
TABLE[1031][44] = 1;
TABLE[1031][335] = 1;
TABLE[495][471] = 1;
TABLE[496][399] = 1;
TABLE[497][399] = 1;
TABLE[498][399] = 1;
TABLE[499][399] = 1;
TABLE[500][295] = 1;
TABLE[1034][59] = 1;
TABLE[1034][44] = 1;
TABLE[1034][335] = 1;
TABLE[494][43] = 1;
TABLE[494][45] = 1;
TABLE[494][259] = 1;
TABLE[494][345] = 1;
TABLE[494][458] = 1;
TABLE[494][256] = 1;
TABLE[494][40] = 1;
TABLE[1033][93] = 1;
TABLE[493][44] = 1;
TABLE[1032][45] = 1;
TABLE[1032][43] = 1;
TABLE[1032][40] = 1;
TABLE[1032][256] = 1;
TABLE[1032][458] = 1;
TABLE[1032][345] = 1;
TABLE[1032][259] = 1;
TABLE[1032][93] = 1;
TABLE[56][356] = 1;
TABLE[501][58] = 1;
TABLE[504][44] = 1;
TABLE[504][335] = 1;
TABLE[504][367] = 1;
TABLE[504][256] = 1;
TABLE[504][259] = 1;
TABLE[504][345] = 1;
TABLE[504][458] = 1;
TABLE[504][40] = 1;
TABLE[504][300] = 1;
TABLE[504][317] = 1;
TABLE[504][419] = 1;
TABLE[504][36] = 1;
TABLE[1035][367] = 1;
TABLE[1035][256] = 1;
TABLE[1035][259] = 1;
TABLE[1035][345] = 1;
TABLE[1035][458] = 1;
TABLE[1035][40] = 1;
TABLE[1035][300] = 1;
TABLE[1035][317] = 1;
TABLE[1035][419] = 1;
TABLE[1035][36] = 1;
TABLE[1035][59] = 1;
TABLE[502][44] = 1;
TABLE[503][335] = 1;
TABLE[57][368] = 1;
TABLE[57][357] = 1;
TABLE[505][368] = 1;
TABLE[506][357] = 1;
TABLE[507][58] = 1;
TABLE[1036][320] = 1;
TABLE[1036][376] = 1;
TABLE[1036][303] = 1;
TABLE[1036][275] = 1;
TABLE[1036][259] = 1;
TABLE[1036][256] = 1;
TABLE[508][275] = 1;
TABLE[508][303] = 1;
TABLE[508][376] = 1;
TABLE[508][320] = 1;
TABLE[1037][259] = 1;
TABLE[1037][256] = 1;
TABLE[509][259] = 1;
TABLE[510][256] = 1;
TABLE[511][400] = 1;
TABLE[1038][59] = 1;
TABLE[58][259] = 1;
TABLE[58][256] = 1;
TABLE[58][320] = 1;
TABLE[58][376] = 1;
TABLE[58][303] = 1;
TABLE[58][275] = 1;
TABLE[512][275] = 1;
TABLE[512][303] = 1;
TABLE[512][376] = 1;
TABLE[512][320] = 1;
TABLE[1039][259] = 1;
TABLE[1039][256] = 1;
TABLE[513][259] = 1;
TABLE[514][256] = 1;
TABLE[515][400] = 1;
TABLE[1040][91] = 1;
TABLE[1040][471] = 1;
TABLE[1040][294] = 1;
TABLE[1040][44] = 1;
TABLE[1040][259] = 1;
TABLE[1040][59] = 1;
TABLE[59][400] = 1;
TABLE[516][44] = 1;
TABLE[1041][59] = 1;
TABLE[60][43] = 1;
TABLE[60][45] = 1;
TABLE[60][259] = 1;
TABLE[60][345] = 1;
TABLE[60][458] = 1;
TABLE[60][256] = 1;
TABLE[60][40] = 1;
TABLE[517][44] = 1;
TABLE[1042][257] = 1;
TABLE[1042][123] = 1;
TABLE[519][257] = 1;
TABLE[520][123] = 1;
TABLE[518][47] = 1;
TABLE[1043][44] = 1;
TABLE[1043][59] = 1;
TABLE[61][475] = 1;
TABLE[61][476] = 1;
TABLE[61][43] = 1;
TABLE[61][45] = 1;
TABLE[61][33] = 1;
TABLE[61][126] = 1;
TABLE[61][58] = 1;
TABLE[61][384] = 1;
TABLE[61][387] = 1;
TABLE[61][258] = 1;
TABLE[61][40] = 1;
TABLE[61][36] = 1;
TABLE[61][395] = 1;
TABLE[61][345] = 1;
TABLE[61][458] = 1;
TABLE[61][257] = 1;
TABLE[61][259] = 1;
TABLE[61][256] = 1;
TABLE[62][367] = 1;
TABLE[62][353] = 1;
TABLE[62][446] = 1;
TABLE[62][302] = 1;
TABLE[62][314] = 1;
TABLE[62][331] = 1;
TABLE[62][469] = 1;
TABLE[62][425] = 1;
TABLE[62][256] = 1;
TABLE[62][259] = 1;
TABLE[62][257] = 1;
TABLE[62][458] = 1;
TABLE[62][345] = 1;
TABLE[62][395] = 1;
TABLE[62][36] = 1;
TABLE[62][40] = 1;
TABLE[62][258] = 1;
TABLE[62][387] = 1;
TABLE[62][384] = 1;
TABLE[62][58] = 1;
TABLE[62][126] = 1;
TABLE[62][33] = 1;
TABLE[62][45] = 1;
TABLE[62][43] = 1;
TABLE[62][476] = 1;
TABLE[62][475] = 1;
TABLE[62][123] = 1;
TABLE[63][59] = 1;
TABLE[63][44] = 1;
TABLE[63][259] = 1;
TABLE[63][345] = 1;
TABLE[63][458] = 1;
TABLE[63][256] = 1;
TABLE[63][40] = 1;
TABLE[63][45] = 1;
TABLE[63][61] = 1;
TABLE[63][43] = 1;
TABLE[522][40] = 1;
TABLE[522][256] = 1;
TABLE[522][458] = 1;
TABLE[522][345] = 1;
TABLE[522][259] = 1;
TABLE[522][43] = 1;
TABLE[523][45] = 1;
TABLE[524][61] = 1;
TABLE[525][43] = 1;
TABLE[525][45] = 1;
TABLE[525][259] = 1;
TABLE[525][345] = 1;
TABLE[525][458] = 1;
TABLE[525][256] = 1;
TABLE[525][40] = 1;
TABLE[521][43] = 1;
TABLE[1044][40] = 1;
TABLE[1044][256] = 1;
TABLE[1044][458] = 1;
TABLE[1044][345] = 1;
TABLE[1044][259] = 1;
TABLE[529][40] = 1;
TABLE[1045][59] = 1;
TABLE[1045][44] = 1;
TABLE[526][387] = 1;
TABLE[527][384] = 1;
TABLE[528][391] = 1;
TABLE[64][388] = 1;
TABLE[531][44] = 1;
TABLE[1047][123] = 1;
TABLE[530][44] = 1;
TABLE[1046][123] = 1;
TABLE[532][125] = 1;
TABLE[532][326] = 1;
TABLE[532][325] = 1;
TABLE[532][450] = 1;
TABLE[532][364] = 1;
TABLE[532][365] = 1;
TABLE[532][366] = 1;
TABLE[532][440] = 1;
TABLE[532][424] = 1;
TABLE[532][369] = 1;
TABLE[532][296] = 1;
TABLE[532][294] = 1;
TABLE[532][323] = 1;
TABLE[532][292] = 1;
TABLE[532][381] = 1;
TABLE[532][346] = 1;
TABLE[532][350] = 1;
TABLE[532][431] = 1;
TABLE[532][435] = 1;
TABLE[532][389] = 1;
TABLE[532][437] = 1;
TABLE[532][330] = 1;
TABLE[532][409] = 1;
TABLE[532][356] = 1;
TABLE[532][400] = 1;
TABLE[532][307] = 1;
TABLE[532][374] = 1;
TABLE[532][43] = 1;
TABLE[532][368] = 1;
TABLE[532][357] = 1;
TABLE[532][277] = 1;
TABLE[532][434] = 1;
TABLE[532][377] = 1;
TABLE[532][311] = 1;
TABLE[532][264] = 1;
TABLE[532][274] = 1;
TABLE[532][268] = 1;
TABLE[532][269] = 1;
TABLE[532][265] = 1;
TABLE[532][266] = 1;
TABLE[532][267] = 1;
TABLE[532][263] = 1;
TABLE[532][271] = 1;
TABLE[532][272] = 1;
TABLE[532][273] = 1;
TABLE[532][474] = 1;
TABLE[1048][125] = 1;
TABLE[65][326] = 1;
TABLE[66][325] = 1;
TABLE[67][374] = 1;
TABLE[67][307] = 1;
TABLE[68][374] = 1;
TABLE[68][307] = 1;
TABLE[69][450] = 1;
TABLE[70][364] = 1;
TABLE[71][365] = 1;
TABLE[72][366] = 1;
TABLE[72][43] = 1;
TABLE[73][357] = 1;
TABLE[73][368] = 1;
TABLE[74][440] = 1;
TABLE[75][440] = 1;
TABLE[76][440] = 1;
TABLE[77][440] = 1;
TABLE[78][424] = 1;
TABLE[79][369] = 1;
TABLE[80][296] = 1;
TABLE[81][294] = 1;
TABLE[82][294] = 1;
TABLE[83][323] = 1;
TABLE[84][292] = 1;
TABLE[85][381] = 1;
TABLE[86][346] = 1;
TABLE[87][350] = 1;
TABLE[88][277] = 1;
TABLE[89][431] = 1;
TABLE[90][435] = 1;
TABLE[91][389] = 1;
TABLE[92][437] = 1;
TABLE[93][330] = 1;
TABLE[94][409] = 1;
TABLE[95][356] = 1;
TABLE[96][400] = 1;
TABLE[97][311] = 1;
TABLE[97][377] = 1;
TABLE[97][434] = 1;
TABLE[98][474] = 1;
TABLE[98][273] = 1;
TABLE[98][272] = 1;
TABLE[98][271] = 1;
TABLE[98][263] = 1;
TABLE[98][267] = 1;
TABLE[98][266] = 1;
TABLE[98][265] = 1;
TABLE[98][269] = 1;
TABLE[98][268] = 1;
TABLE[98][274] = 1;
TABLE[98][264] = 1;
TABLE[533][58] = 1;
TABLE[1049][257] = 1;
TABLE[534][58] = 1;
TABLE[1050][43] = 1;
TABLE[1050][45] = 1;
TABLE[1050][259] = 1;
TABLE[1050][345] = 1;
TABLE[1050][458] = 1;
TABLE[1050][256] = 1;
TABLE[1050][40] = 1;
TABLE[535][374] = 1;
TABLE[536][307] = 1;
TABLE[537][58] = 1;
TABLE[1051][43] = 1;
TABLE[1051][45] = 1;
TABLE[1051][259] = 1;
TABLE[1051][345] = 1;
TABLE[1051][458] = 1;
TABLE[1051][256] = 1;
TABLE[1051][40] = 1;
TABLE[538][374] = 1;
TABLE[539][307] = 1;
TABLE[540][58] = 1;
TABLE[1052][43] = 1;
TABLE[1052][45] = 1;
TABLE[1052][259] = 1;
TABLE[1052][345] = 1;
TABLE[1052][458] = 1;
TABLE[1052][256] = 1;
TABLE[1052][40] = 1;
TABLE[541][58] = 1;
TABLE[1053][43] = 1;
TABLE[1053][45] = 1;
TABLE[1053][259] = 1;
TABLE[1053][345] = 1;
TABLE[1053][458] = 1;
TABLE[1053][256] = 1;
TABLE[1053][40] = 1;
TABLE[542][44] = 1;
TABLE[1054][59] = 1;
TABLE[543][58] = 1;
TABLE[1055][43] = 1;
TABLE[1055][45] = 1;
TABLE[1055][259] = 1;
TABLE[1055][345] = 1;
TABLE[1055][458] = 1;
TABLE[1055][256] = 1;
TABLE[1055][40] = 1;
TABLE[544][58] = 1;
TABLE[1056][43] = 1;
TABLE[1056][45] = 1;
TABLE[1056][259] = 1;
TABLE[1056][345] = 1;
TABLE[1056][458] = 1;
TABLE[1056][256] = 1;
TABLE[1056][40] = 1;
TABLE[545][43] = 1;
TABLE[1057][366] = 1;
TABLE[546][58] = 1;
TABLE[1058][43] = 1;
TABLE[1058][45] = 1;
TABLE[1058][259] = 1;
TABLE[1058][345] = 1;
TABLE[1058][458] = 1;
TABLE[1058][256] = 1;
TABLE[1058][40] = 1;
TABLE[547][58] = 1;
TABLE[1059][45] = 1;
TABLE[1059][43] = 1;
TABLE[1059][40] = 1;
TABLE[1059][256] = 1;
TABLE[1059][458] = 1;
TABLE[1059][345] = 1;
TABLE[1059][259] = 1;
TABLE[1059][59] = 1;
TABLE[549][43] = 1;
TABLE[549][45] = 1;
TABLE[549][259] = 1;
TABLE[549][345] = 1;
TABLE[549][458] = 1;
TABLE[549][256] = 1;
TABLE[549][40] = 1;
TABLE[1061][59] = 1;
TABLE[548][44] = 1;
TABLE[1060][45] = 1;
TABLE[1060][43] = 1;
TABLE[1060][40] = 1;
TABLE[1060][256] = 1;
TABLE[1060][458] = 1;
TABLE[1060][345] = 1;
TABLE[1060][259] = 1;
TABLE[1060][59] = 1;
TABLE[550][58] = 1;
TABLE[1062][45] = 1;
TABLE[1062][43] = 1;
TABLE[1062][40] = 1;
TABLE[1062][256] = 1;
TABLE[1062][458] = 1;
TABLE[1062][345] = 1;
TABLE[1062][259] = 1;
TABLE[1062][59] = 1;
TABLE[552][43] = 1;
TABLE[552][45] = 1;
TABLE[552][259] = 1;
TABLE[552][345] = 1;
TABLE[552][458] = 1;
TABLE[552][256] = 1;
TABLE[552][40] = 1;
TABLE[1064][59] = 1;
TABLE[551][44] = 1;
TABLE[1063][45] = 1;
TABLE[1063][43] = 1;
TABLE[1063][40] = 1;
TABLE[1063][256] = 1;
TABLE[1063][458] = 1;
TABLE[1063][345] = 1;
TABLE[1063][259] = 1;
TABLE[1063][59] = 1;
TABLE[553][58] = 1;
TABLE[1065][45] = 1;
TABLE[1065][43] = 1;
TABLE[1065][40] = 1;
TABLE[1065][256] = 1;
TABLE[1065][458] = 1;
TABLE[1065][345] = 1;
TABLE[1065][259] = 1;
TABLE[554][44] = 1;
TABLE[1066][59] = 1;
TABLE[555][58] = 1;
TABLE[1067][43] = 1;
TABLE[1067][45] = 1;
TABLE[1067][259] = 1;
TABLE[1067][345] = 1;
TABLE[1067][458] = 1;
TABLE[1067][256] = 1;
TABLE[1067][40] = 1;
TABLE[556][58] = 1;
TABLE[1068][43] = 1;
TABLE[1068][45] = 1;
TABLE[1068][259] = 1;
TABLE[1068][345] = 1;
TABLE[1068][458] = 1;
TABLE[1068][256] = 1;
TABLE[1068][40] = 1;
TABLE[557][58] = 1;
TABLE[1069][43] = 1;
TABLE[1069][45] = 1;
TABLE[1069][259] = 1;
TABLE[1069][345] = 1;
TABLE[1069][458] = 1;
TABLE[1069][256] = 1;
TABLE[1069][40] = 1;
TABLE[558][58] = 1;
TABLE[1070][43] = 1;
TABLE[1070][45] = 1;
TABLE[1070][259] = 1;
TABLE[1070][345] = 1;
TABLE[1070][458] = 1;
TABLE[1070][256] = 1;
TABLE[1070][40] = 1;
TABLE[559][43] = 1;
TABLE[559][45] = 1;
TABLE[559][259] = 1;
TABLE[559][345] = 1;
TABLE[559][458] = 1;
TABLE[559][256] = 1;
TABLE[559][40] = 1;
TABLE[1071][43] = 1;
TABLE[1071][45] = 1;
TABLE[1071][259] = 1;
TABLE[1071][345] = 1;
TABLE[1071][458] = 1;
TABLE[1071][256] = 1;
TABLE[1071][40] = 1;
TABLE[560][58] = 1;
TABLE[1072][126] = 1;
TABLE[1072][43] = 1;
TABLE[1072][45] = 1;
TABLE[1072][259] = 1;
TABLE[1072][345] = 1;
TABLE[1072][458] = 1;
TABLE[1072][256] = 1;
TABLE[1072][40] = 1;
TABLE[1072][59] = 1;
TABLE[561][45] = 1;
TABLE[561][43] = 1;
TABLE[561][126] = 1;
TABLE[561][40] = 1;
TABLE[561][256] = 1;
TABLE[561][458] = 1;
TABLE[561][345] = 1;
TABLE[561][259] = 1;
TABLE[1073][59] = 1;
TABLE[562][44] = 1;
TABLE[1074][59] = 1;
TABLE[563][58] = 1;
TABLE[1075][43] = 1;
TABLE[1075][45] = 1;
TABLE[1075][259] = 1;
TABLE[1075][345] = 1;
TABLE[1075][458] = 1;
TABLE[1075][256] = 1;
TABLE[1075][40] = 1;
TABLE[564][58] = 1;
TABLE[1076][43] = 1;
TABLE[1076][45] = 1;
TABLE[1076][259] = 1;
TABLE[1076][345] = 1;
TABLE[1076][458] = 1;
TABLE[1076][256] = 1;
TABLE[1076][40] = 1;
TABLE[565][37] = 1;
TABLE[1077][59] = 1;
TABLE[566][58] = 1;
TABLE[1078][43] = 1;
TABLE[1078][45] = 1;
TABLE[1078][259] = 1;
TABLE[1078][345] = 1;
TABLE[1078][458] = 1;
TABLE[1078][256] = 1;
TABLE[1078][40] = 1;
TABLE[567][37] = 1;
TABLE[1079][59] = 1;
TABLE[568][58] = 1;
TABLE[1080][36] = 1;
TABLE[1080][59] = 1;
TABLE[570][36] = 1;
TABLE[1082][59] = 1;
TABLE[569][44] = 1;
TABLE[1081][36] = 1;
TABLE[1081][59] = 1;
TABLE[571][58] = 1;
TABLE[1083][36] = 1;
TABLE[1083][59] = 1;
TABLE[573][36] = 1;
TABLE[1085][59] = 1;
TABLE[572][44] = 1;
TABLE[1084][36] = 1;
TABLE[1084][59] = 1;
TABLE[574][58] = 1;
TABLE[1086][36] = 1;
TABLE[1086][59] = 1;
TABLE[576][36] = 1;
TABLE[1088][59] = 1;
TABLE[575][44] = 1;
TABLE[1087][36] = 1;
TABLE[1087][59] = 1;
TABLE[99][275] = 1;
TABLE[100][303] = 1;
TABLE[101][376] = 1;
TABLE[102][320] = 1;
TABLE[103][43] = 1;
TABLE[103][45] = 1;
TABLE[103][259] = 1;
TABLE[103][345] = 1;
TABLE[103][458] = 1;
TABLE[103][256] = 1;
TABLE[103][40] = 1;
TABLE[104][43] = 1;
TABLE[104][45] = 1;
TABLE[104][259] = 1;
TABLE[104][345] = 1;
TABLE[104][458] = 1;
TABLE[104][256] = 1;
TABLE[104][40] = 1;
TABLE[577][259] = 1;
TABLE[577][345] = 1;
TABLE[577][458] = 1;
TABLE[577][256] = 1;
TABLE[577][40] = 1;
TABLE[578][40] = 1;
TABLE[578][256] = 1;
TABLE[578][458] = 1;
TABLE[578][345] = 1;
TABLE[578][259] = 1;
TABLE[1089][259] = 1;
TABLE[1089][345] = 1;
TABLE[1089][458] = 1;
TABLE[1089][256] = 1;
TABLE[1089][319] = 1;
TABLE[1089][40] = 1;
TABLE[1089][288] = 1;
TABLE[1089][44] = 1;
TABLE[1089][59] = 1;
TABLE[579][40] = 1;
TABLE[579][256] = 1;
TABLE[579][458] = 1;
TABLE[579][345] = 1;
TABLE[579][259] = 1;
TABLE[1090][319] = 1;
TABLE[1090][40] = 1;
TABLE[1090][288] = 1;
TABLE[1090][44] = 1;
TABLE[1090][59] = 1;
TABLE[580][319] = 1;
TABLE[1091][40] = 1;
TABLE[1091][288] = 1;
TABLE[1091][44] = 1;
TABLE[1091][59] = 1;
TABLE[584][40] = 1;
TABLE[1092][288] = 1;
TABLE[1092][44] = 1;
TABLE[1092][59] = 1;
TABLE[581][322] = 1;
TABLE[582][307] = 1;
TABLE[583][374] = 1;
TABLE[592][288] = 1;
TABLE[1097][44] = 1;
TABLE[1097][59] = 1;
TABLE[585][259] = 1;
TABLE[585][345] = 1;
TABLE[585][458] = 1;
TABLE[585][256] = 1;
TABLE[585][40] = 1;
TABLE[1093][45] = 1;
TABLE[1093][43] = 1;
TABLE[1093][256] = 1;
TABLE[1093][458] = 1;
TABLE[1093][345] = 1;
TABLE[1093][259] = 1;
TABLE[1093][319] = 1;
TABLE[1093][40] = 1;
TABLE[1093][288] = 1;
TABLE[1093][44] = 1;
TABLE[1093][59] = 1;
TABLE[586][43] = 1;
TABLE[586][45] = 1;
TABLE[586][259] = 1;
TABLE[586][345] = 1;
TABLE[586][458] = 1;
TABLE[586][256] = 1;
TABLE[586][40] = 1;
TABLE[1094][319] = 1;
TABLE[1094][40] = 1;
TABLE[1094][288] = 1;
TABLE[1094][44] = 1;
TABLE[1094][59] = 1;
TABLE[587][319] = 1;
TABLE[1095][40] = 1;
TABLE[1095][288] = 1;
TABLE[1095][44] = 1;
TABLE[1095][59] = 1;
TABLE[591][40] = 1;
TABLE[1096][288] = 1;
TABLE[1096][44] = 1;
TABLE[1096][59] = 1;
TABLE[588][322] = 1;
TABLE[589][307] = 1;
TABLE[590][374] = 1;
TABLE[593][43] = 1;
TABLE[593][45] = 1;
TABLE[593][259] = 1;
TABLE[593][345] = 1;
TABLE[593][458] = 1;
TABLE[593][256] = 1;
TABLE[593][40] = 1;
TABLE[1098][44] = 1;
TABLE[1098][59] = 1;
TABLE[105][43] = 1;
TABLE[105][45] = 1;
TABLE[105][259] = 1;
TABLE[105][345] = 1;
TABLE[105][458] = 1;
TABLE[105][256] = 1;
TABLE[105][40] = 1;
TABLE[106][126] = 1;
TABLE[594][44] = 1;
TABLE[1099][126] = 1;
TABLE[1099][43] = 1;
TABLE[1099][45] = 1;
TABLE[1099][259] = 1;
TABLE[1099][345] = 1;
TABLE[1099][458] = 1;
TABLE[1099][256] = 1;
TABLE[1099][40] = 1;
TABLE[1099][59] = 1;
TABLE[595][44] = 1;
TABLE[1100][126] = 1;
TABLE[1100][43] = 1;
TABLE[1100][45] = 1;
TABLE[1100][259] = 1;
TABLE[1100][345] = 1;
TABLE[1100][458] = 1;
TABLE[1100][256] = 1;
TABLE[1100][40] = 1;
TABLE[1100][59] = 1;
TABLE[107][277] = 1;
TABLE[596][58] = 1;
TABLE[1101][43] = 1;
TABLE[1101][45] = 1;
TABLE[1101][259] = 1;
TABLE[1101][345] = 1;
TABLE[1101][458] = 1;
TABLE[1101][256] = 1;
TABLE[1101][40] = 1;
TABLE[108][372] = 1;
TABLE[597][326] = 1;
TABLE[597][374] = 1;
TABLE[597][430] = 1;
TABLE[597][373] = 1;
TABLE[597][316] = 1;
TABLE[597][452] = 1;
TABLE[597][284] = 1;
TABLE[597][435] = 1;
TABLE[597][404] = 1;
TABLE[597][420] = 1;
TABLE[597][383] = 1;
TABLE[597][315] = 1;
TABLE[597][466] = 1;
TABLE[597][431] = 1;
TABLE[597][396] = 1;
TABLE[597][323] = 1;
TABLE[597][312] = 1;
TABLE[597][407] = 1;
TABLE[597][292] = 1;
TABLE[597][365] = 1;
TABLE[597][437] = 1;
TABLE[597][349] = 1;
TABLE[597][350] = 1;
TABLE[597][361] = 1;
TABLE[597][304] = 1;
TABLE[597][468] = 1;
TABLE[597][467] = 1;
TABLE[597][379] = 1;
TABLE[597][456] = 1;
TABLE[597][313] = 1;
TABLE[597][376] = 1;
TABLE[597][375] = 1;
TABLE[597][355] = 1;
TABLE[597][400] = 1;
TABLE[597][368] = 1;
TABLE[597][357] = 1;
TABLE[597][434] = 1;
TABLE[597][377] = 1;
TABLE[597][311] = 1;
TABLE[597][264] = 1;
TABLE[597][274] = 1;
TABLE[597][268] = 1;
TABLE[597][269] = 1;
TABLE[597][265] = 1;
TABLE[597][266] = 1;
TABLE[597][267] = 1;
TABLE[597][263] = 1;
TABLE[597][271] = 1;
TABLE[597][272] = 1;
TABLE[597][273] = 1;
TABLE[597][474] = 1;
TABLE[1102][125] = 1;
TABLE[109][326] = 1;
TABLE[110][374] = 1;
TABLE[111][357] = 1;
TABLE[111][368] = 1;
TABLE[112][430] = 1;
TABLE[113][373] = 1;
TABLE[114][316] = 1;
TABLE[115][452] = 1;
TABLE[116][284] = 1;
TABLE[117][435] = 1;
TABLE[118][435] = 1;
TABLE[119][435] = 1;
TABLE[120][404] = 1;
TABLE[121][420] = 1;
TABLE[122][383] = 1;
TABLE[123][315] = 1;
TABLE[124][466] = 1;
TABLE[125][431] = 1;
TABLE[126][396] = 1;
TABLE[127][323] = 1;
TABLE[128][312] = 1;
TABLE[129][407] = 1;
TABLE[130][292] = 1;
TABLE[131][365] = 1;
TABLE[132][437] = 1;
TABLE[133][349] = 1;
TABLE[134][350] = 1;
TABLE[135][361] = 1;
TABLE[136][304] = 1;
TABLE[137][468] = 1;
TABLE[138][467] = 1;
TABLE[139][379] = 1;
TABLE[140][456] = 1;
TABLE[141][313] = 1;
TABLE[142][376] = 1;
TABLE[143][375] = 1;
TABLE[144][355] = 1;
TABLE[145][311] = 1;
TABLE[145][377] = 1;
TABLE[145][434] = 1;
TABLE[146][400] = 1;
TABLE[147][474] = 1;
TABLE[147][273] = 1;
TABLE[147][272] = 1;
TABLE[147][271] = 1;
TABLE[147][263] = 1;
TABLE[147][267] = 1;
TABLE[147][266] = 1;
TABLE[147][265] = 1;
TABLE[147][269] = 1;
TABLE[147][268] = 1;
TABLE[147][274] = 1;
TABLE[147][264] = 1;
TABLE[598][58] = 1;
TABLE[1103][257] = 1;
TABLE[599][58] = 1;
TABLE[1104][43] = 1;
TABLE[1104][45] = 1;
TABLE[1104][259] = 1;
TABLE[1104][345] = 1;
TABLE[1104][458] = 1;
TABLE[1104][256] = 1;
TABLE[1104][40] = 1;
TABLE[600][58] = 1;
TABLE[1105][259] = 1;
TABLE[1105][345] = 1;
TABLE[1105][458] = 1;
TABLE[1105][256] = 1;
TABLE[1105][40] = 1;
TABLE[1105][45] = 1;
TABLE[1105][43] = 1;
TABLE[601][58] = 1;
TABLE[1106][259] = 1;
TABLE[1106][345] = 1;
TABLE[1106][458] = 1;
TABLE[1106][256] = 1;
TABLE[1106][40] = 1;
TABLE[1106][45] = 1;
TABLE[1106][43] = 1;
TABLE[602][58] = 1;
TABLE[1107][43] = 1;
TABLE[1107][45] = 1;
TABLE[1107][259] = 1;
TABLE[1107][345] = 1;
TABLE[1107][458] = 1;
TABLE[1107][256] = 1;
TABLE[1107][40] = 1;
TABLE[603][58] = 1;
TABLE[1108][43] = 1;
TABLE[1108][45] = 1;
TABLE[1108][259] = 1;
TABLE[1108][345] = 1;
TABLE[1108][458] = 1;
TABLE[1108][256] = 1;
TABLE[1108][40] = 1;
TABLE[604][58] = 1;
TABLE[1109][43] = 1;
TABLE[1109][45] = 1;
TABLE[1109][259] = 1;
TABLE[1109][345] = 1;
TABLE[1109][458] = 1;
TABLE[1109][256] = 1;
TABLE[1109][40] = 1;
TABLE[605][58] = 1;
TABLE[1110][43] = 1;
TABLE[1110][45] = 1;
TABLE[1110][259] = 1;
TABLE[1110][345] = 1;
TABLE[1110][458] = 1;
TABLE[1110][256] = 1;
TABLE[1110][40] = 1;
TABLE[606][58] = 1;
TABLE[1111][40] = 1;
TABLE[1111][256] = 1;
TABLE[1111][458] = 1;
TABLE[1111][345] = 1;
TABLE[1111][259] = 1;
TABLE[607][58] = 1;
TABLE[1112][43] = 1;
TABLE[608][58] = 1;
TABLE[1113][43] = 1;
TABLE[1113][45] = 1;
TABLE[1113][259] = 1;
TABLE[1113][345] = 1;
TABLE[1113][458] = 1;
TABLE[1113][256] = 1;
TABLE[1113][40] = 1;
TABLE[609][58] = 1;
TABLE[1114][43] = 1;
TABLE[1114][45] = 1;
TABLE[1114][259] = 1;
TABLE[1114][345] = 1;
TABLE[1114][458] = 1;
TABLE[1114][256] = 1;
TABLE[1114][40] = 1;
TABLE[610][58] = 1;
TABLE[1115][43] = 1;
TABLE[1115][45] = 1;
TABLE[1115][259] = 1;
TABLE[1115][345] = 1;
TABLE[1115][458] = 1;
TABLE[1115][256] = 1;
TABLE[1115][40] = 1;
TABLE[611][58] = 1;
TABLE[1116][43] = 1;
TABLE[1116][45] = 1;
TABLE[1116][259] = 1;
TABLE[1116][345] = 1;
TABLE[1116][458] = 1;
TABLE[1116][256] = 1;
TABLE[1116][40] = 1;
TABLE[612][43] = 1;
TABLE[612][45] = 1;
TABLE[612][259] = 1;
TABLE[612][345] = 1;
TABLE[612][458] = 1;
TABLE[612][256] = 1;
TABLE[612][40] = 1;
TABLE[1117][59] = 1;
TABLE[613][58] = 1;
TABLE[1118][43] = 1;
TABLE[1118][45] = 1;
TABLE[1118][259] = 1;
TABLE[1118][345] = 1;
TABLE[1118][458] = 1;
TABLE[1118][256] = 1;
TABLE[1118][40] = 1;
TABLE[614][58] = 1;
TABLE[1119][43] = 1;
TABLE[1119][45] = 1;
TABLE[1119][259] = 1;
TABLE[1119][345] = 1;
TABLE[1119][458] = 1;
TABLE[1119][256] = 1;
TABLE[1119][40] = 1;
TABLE[615][58] = 1;
TABLE[1120][43] = 1;
TABLE[1120][45] = 1;
TABLE[1120][259] = 1;
TABLE[1120][345] = 1;
TABLE[1120][458] = 1;
TABLE[1120][256] = 1;
TABLE[1120][40] = 1;
TABLE[616][58] = 1;
TABLE[1121][43] = 1;
TABLE[1121][45] = 1;
TABLE[1121][259] = 1;
TABLE[1121][345] = 1;
TABLE[1121][458] = 1;
TABLE[1121][256] = 1;
TABLE[1121][40] = 1;
TABLE[617][58] = 1;
TABLE[1122][43] = 1;
TABLE[1122][45] = 1;
TABLE[1122][259] = 1;
TABLE[1122][345] = 1;
TABLE[1122][458] = 1;
TABLE[1122][256] = 1;
TABLE[1122][40] = 1;
TABLE[618][58] = 1;
TABLE[1123][43] = 1;
TABLE[1123][45] = 1;
TABLE[1123][259] = 1;
TABLE[1123][345] = 1;
TABLE[1123][458] = 1;
TABLE[1123][256] = 1;
TABLE[1123][40] = 1;
TABLE[619][58] = 1;
TABLE[1124][276] = 1;
TABLE[1124][43] = 1;
TABLE[1124][45] = 1;
TABLE[1124][259] = 1;
TABLE[1124][345] = 1;
TABLE[1124][458] = 1;
TABLE[1124][256] = 1;
TABLE[1124][40] = 1;
TABLE[1124][59] = 1;
TABLE[620][45] = 1;
TABLE[620][43] = 1;
TABLE[620][276] = 1;
TABLE[620][40] = 1;
TABLE[620][256] = 1;
TABLE[620][458] = 1;
TABLE[620][345] = 1;
TABLE[620][259] = 1;
TABLE[1125][59] = 1;
TABLE[621][58] = 1;
TABLE[1126][43] = 1;
TABLE[1126][45] = 1;
TABLE[1126][259] = 1;
TABLE[1126][345] = 1;
TABLE[1126][458] = 1;
TABLE[1126][256] = 1;
TABLE[1126][40] = 1;
TABLE[622][58] = 1;
TABLE[1127][36] = 1;
TABLE[1127][59] = 1;
TABLE[624][36] = 1;
TABLE[1129][59] = 1;
TABLE[623][44] = 1;
TABLE[1128][36] = 1;
TABLE[1128][59] = 1;
TABLE[625][58] = 1;
TABLE[1130][36] = 1;
TABLE[1130][59] = 1;
TABLE[627][36] = 1;
TABLE[1132][59] = 1;
TABLE[626][44] = 1;
TABLE[1131][36] = 1;
TABLE[1131][59] = 1;
TABLE[628][58] = 1;
TABLE[1133][126] = 1;
TABLE[1133][43] = 1;
TABLE[1133][45] = 1;
TABLE[1133][259] = 1;
TABLE[1133][345] = 1;
TABLE[1133][458] = 1;
TABLE[1133][256] = 1;
TABLE[1133][40] = 1;
TABLE[1133][59] = 1;
TABLE[629][45] = 1;
TABLE[629][43] = 1;
TABLE[629][126] = 1;
TABLE[629][40] = 1;
TABLE[629][256] = 1;
TABLE[629][458] = 1;
TABLE[629][345] = 1;
TABLE[629][259] = 1;
TABLE[1134][59] = 1;
TABLE[630][58] = 1;
TABLE[1135][45] = 1;
TABLE[1135][43] = 1;
TABLE[1135][40] = 1;
TABLE[1135][256] = 1;
TABLE[1135][458] = 1;
TABLE[1135][345] = 1;
TABLE[1135][259] = 1;
TABLE[1135][59] = 1;
TABLE[633][43] = 1;
TABLE[633][45] = 1;
TABLE[633][259] = 1;
TABLE[633][345] = 1;
TABLE[633][458] = 1;
TABLE[633][256] = 1;
TABLE[633][40] = 1;
TABLE[1137][59] = 1;
TABLE[631][44] = 1;
TABLE[632][124] = 1;
TABLE[1136][45] = 1;
TABLE[1136][43] = 1;
TABLE[1136][40] = 1;
TABLE[1136][256] = 1;
TABLE[1136][458] = 1;
TABLE[1136][345] = 1;
TABLE[1136][259] = 1;
TABLE[1136][59] = 1;
TABLE[634][58] = 1;
TABLE[1138][43] = 1;
TABLE[1138][45] = 1;
TABLE[1138][259] = 1;
TABLE[1138][345] = 1;
TABLE[1138][458] = 1;
TABLE[1138][256] = 1;
TABLE[1138][40] = 1;
TABLE[635][58] = 1;
TABLE[1139][43] = 1;
TABLE[1139][45] = 1;
TABLE[1139][259] = 1;
TABLE[1139][345] = 1;
TABLE[1139][458] = 1;
TABLE[1139][256] = 1;
TABLE[1139][40] = 1;
TABLE[636][58] = 1;
TABLE[1140][43] = 1;
TABLE[1140][45] = 1;
TABLE[1140][259] = 1;
TABLE[1140][345] = 1;
TABLE[1140][458] = 1;
TABLE[1140][256] = 1;
TABLE[1140][40] = 1;
TABLE[637][58] = 1;
TABLE[1141][43] = 1;
TABLE[1141][45] = 1;
TABLE[1141][259] = 1;
TABLE[1141][345] = 1;
TABLE[1141][458] = 1;
TABLE[1141][256] = 1;
TABLE[1141][40] = 1;
TABLE[638][58] = 1;
TABLE[1142][43] = 1;
TABLE[1142][45] = 1;
TABLE[1142][259] = 1;
TABLE[1142][345] = 1;
TABLE[1142][458] = 1;
TABLE[1142][256] = 1;
TABLE[1142][40] = 1;
TABLE[639][58] = 1;
TABLE[1143][43] = 1;
TABLE[1143][45] = 1;
TABLE[1143][259] = 1;
TABLE[1143][345] = 1;
TABLE[1143][458] = 1;
TABLE[1143][256] = 1;
TABLE[1143][40] = 1;
TABLE[640][58] = 1;
TABLE[1144][43] = 1;
TABLE[1144][45] = 1;
TABLE[1144][259] = 1;
TABLE[1144][345] = 1;
TABLE[1144][458] = 1;
TABLE[1144][256] = 1;
TABLE[1144][40] = 1;
TABLE[641][58] = 1;
TABLE[1145][43] = 1;
TABLE[1145][45] = 1;
TABLE[1145][259] = 1;
TABLE[1145][345] = 1;
TABLE[1145][458] = 1;
TABLE[1145][256] = 1;
TABLE[1145][40] = 1;
TABLE[642][58] = 1;
TABLE[1146][36] = 1;
TABLE[148][43] = 1;
TABLE[148][45] = 1;
TABLE[148][259] = 1;
TABLE[148][345] = 1;
TABLE[148][458] = 1;
TABLE[148][256] = 1;
TABLE[148][40] = 1;
TABLE[149][276] = 1;
TABLE[643][44] = 1;
TABLE[1147][276] = 1;
TABLE[1147][43] = 1;
TABLE[1147][45] = 1;
TABLE[1147][259] = 1;
TABLE[1147][345] = 1;
TABLE[1147][458] = 1;
TABLE[1147][256] = 1;
TABLE[1147][40] = 1;
TABLE[1147][59] = 1;
TABLE[644][44] = 1;
TABLE[1148][276] = 1;
TABLE[1148][43] = 1;
TABLE[1148][45] = 1;
TABLE[1148][259] = 1;
TABLE[1148][345] = 1;
TABLE[1148][458] = 1;
TABLE[1148][256] = 1;
TABLE[1148][40] = 1;
TABLE[1148][59] = 1;
TABLE[150][43] = 1;
TABLE[150][45] = 1;
TABLE[150][259] = 1;
TABLE[150][345] = 1;
TABLE[150][458] = 1;
TABLE[150][256] = 1;
TABLE[150][40] = 1;
TABLE[151][126] = 1;
TABLE[645][44] = 1;
TABLE[1149][126] = 1;
TABLE[1149][43] = 1;
TABLE[1149][45] = 1;
TABLE[1149][259] = 1;
TABLE[1149][345] = 1;
TABLE[1149][458] = 1;
TABLE[1149][256] = 1;
TABLE[1149][40] = 1;
TABLE[1149][59] = 1;
TABLE[646][44] = 1;
TABLE[1150][126] = 1;
TABLE[1150][43] = 1;
TABLE[1150][45] = 1;
TABLE[1150][259] = 1;
TABLE[1150][345] = 1;
TABLE[1150][458] = 1;
TABLE[1150][256] = 1;
TABLE[1150][40] = 1;
TABLE[1150][59] = 1;
TABLE[152][348] = 1;
TABLE[647][326] = 1;
TABLE[647][383] = 1;
TABLE[647][389] = 1;
TABLE[647][365] = 1;
TABLE[647][350] = 1;
TABLE[647][472] = 1;
TABLE[647][344] = 1;
TABLE[647][400] = 1;
TABLE[647][368] = 1;
TABLE[647][357] = 1;
TABLE[647][334] = 1;
TABLE[647][448] = 1;
TABLE[647][434] = 1;
TABLE[647][377] = 1;
TABLE[647][311] = 1;
TABLE[647][264] = 1;
TABLE[647][274] = 1;
TABLE[647][268] = 1;
TABLE[647][269] = 1;
TABLE[647][265] = 1;
TABLE[647][266] = 1;
TABLE[647][267] = 1;
TABLE[647][263] = 1;
TABLE[647][271] = 1;
TABLE[647][272] = 1;
TABLE[647][273] = 1;
TABLE[647][474] = 1;
TABLE[1151][125] = 1;
TABLE[153][326] = 1;
TABLE[154][383] = 1;
TABLE[155][357] = 1;
TABLE[155][368] = 1;
TABLE[156][389] = 1;
TABLE[157][365] = 1;
TABLE[158][350] = 1;
TABLE[159][448] = 1;
TABLE[159][334] = 1;
TABLE[160][472] = 1;
TABLE[161][344] = 1;
TABLE[162][400] = 1;
TABLE[163][311] = 1;
TABLE[163][377] = 1;
TABLE[163][434] = 1;
TABLE[164][474] = 1;
TABLE[164][273] = 1;
TABLE[164][272] = 1;
TABLE[164][271] = 1;
TABLE[164][263] = 1;
TABLE[164][267] = 1;
TABLE[164][266] = 1;
TABLE[164][265] = 1;
TABLE[164][269] = 1;
TABLE[164][268] = 1;
TABLE[164][274] = 1;
TABLE[164][264] = 1;
TABLE[648][58] = 1;
TABLE[1152][257] = 1;
TABLE[649][58] = 1;
TABLE[1153][43] = 1;
TABLE[1153][45] = 1;
TABLE[1153][259] = 1;
TABLE[1153][345] = 1;
TABLE[1153][458] = 1;
TABLE[1153][256] = 1;
TABLE[1153][40] = 1;
TABLE[650][58] = 1;
TABLE[1154][43] = 1;
TABLE[1154][45] = 1;
TABLE[1154][259] = 1;
TABLE[1154][345] = 1;
TABLE[1154][458] = 1;
TABLE[1154][256] = 1;
TABLE[1154][40] = 1;
TABLE[651][58] = 1;
TABLE[1155][43] = 1;
TABLE[1155][45] = 1;
TABLE[1155][259] = 1;
TABLE[1155][345] = 1;
TABLE[1155][458] = 1;
TABLE[1155][256] = 1;
TABLE[1155][40] = 1;
TABLE[652][58] = 1;
TABLE[1156][45] = 1;
TABLE[1156][43] = 1;
TABLE[1156][40] = 1;
TABLE[1156][256] = 1;
TABLE[1156][458] = 1;
TABLE[1156][345] = 1;
TABLE[1156][259] = 1;
TABLE[1156][59] = 1;
TABLE[655][43] = 1;
TABLE[655][45] = 1;
TABLE[655][259] = 1;
TABLE[655][345] = 1;
TABLE[655][458] = 1;
TABLE[655][256] = 1;
TABLE[655][40] = 1;
TABLE[1158][59] = 1;
TABLE[653][124] = 1;
TABLE[654][44] = 1;
TABLE[1157][45] = 1;
TABLE[1157][43] = 1;
TABLE[1157][40] = 1;
TABLE[1157][256] = 1;
TABLE[1157][458] = 1;
TABLE[1157][345] = 1;
TABLE[1157][259] = 1;
TABLE[1157][59] = 1;
TABLE[656][448] = 1;
TABLE[657][334] = 1;
TABLE[658][58] = 1;
TABLE[1159][36] = 1;
TABLE[659][58] = 1;
TABLE[1160][43] = 1;
TABLE[1160][45] = 1;
TABLE[1160][259] = 1;
TABLE[1160][345] = 1;
TABLE[1160][458] = 1;
TABLE[1160][256] = 1;
TABLE[1160][40] = 1;
TABLE[660][58] = 1;
TABLE[1161][259] = 1;
TABLE[1161][345] = 1;
TABLE[1161][458] = 1;
TABLE[1161][256] = 1;
TABLE[1161][40] = 1;
TABLE[1161][45] = 1;
TABLE[1161][43] = 1;
TABLE[165][436] = 1;
TABLE[165][334] = 1;
TABLE[165][409] = 1;
TABLE[165][330] = 1;
TABLE[165][45] = 1;
TABLE[165][43] = 1;
TABLE[165][40] = 1;
TABLE[165][256] = 1;
TABLE[165][458] = 1;
TABLE[165][345] = 1;
TABLE[165][259] = 1;
TABLE[662][43] = 1;
TABLE[662][45] = 1;
TABLE[662][259] = 1;
TABLE[662][345] = 1;
TABLE[662][458] = 1;
TABLE[662][256] = 1;
TABLE[662][40] = 1;
TABLE[1163][436] = 1;
TABLE[1163][334] = 1;
TABLE[1163][409] = 1;
TABLE[1163][330] = 1;
TABLE[661][47] = 1;
TABLE[1162][45] = 1;
TABLE[1162][43] = 1;
TABLE[1162][40] = 1;
TABLE[1162][256] = 1;
TABLE[1162][458] = 1;
TABLE[1162][345] = 1;
TABLE[1162][259] = 1;
TABLE[1162][436] = 1;
TABLE[1162][334] = 1;
TABLE[1162][409] = 1;
TABLE[1162][330] = 1;
TABLE[663][436] = 1;
TABLE[664][334] = 1;
TABLE[665][409] = 1;
TABLE[666][330] = 1;
TABLE[667][326] = 1;
TABLE[667][374] = 1;
TABLE[667][315] = 1;
TABLE[667][298] = 1;
TABLE[667][413] = 1;
TABLE[667][445] = 1;
TABLE[667][417] = 1;
TABLE[667][472] = 1;
TABLE[667][473] = 1;
TABLE[667][333] = 1;
TABLE[667][414] = 1;
TABLE[667][297] = 1;
TABLE[667][378] = 1;
TABLE[667][461] = 1;
TABLE[667][428] = 1;
TABLE[667][318] = 1;
TABLE[667][350] = 1;
TABLE[667][400] = 1;
TABLE[667][264] = 1;
TABLE[667][274] = 1;
TABLE[667][268] = 1;
TABLE[667][269] = 1;
TABLE[667][265] = 1;
TABLE[667][266] = 1;
TABLE[667][267] = 1;
TABLE[667][263] = 1;
TABLE[667][271] = 1;
TABLE[667][272] = 1;
TABLE[667][273] = 1;
TABLE[667][474] = 1;
TABLE[667][434] = 1;
TABLE[667][377] = 1;
TABLE[667][311] = 1;
TABLE[667][45] = 1;
TABLE[667][43] = 1;
TABLE[667][40] = 1;
TABLE[667][256] = 1;
TABLE[667][458] = 1;
TABLE[667][345] = 1;
TABLE[667][259] = 1;
TABLE[1164][125] = 1;
TABLE[669][288] = 1;
TABLE[1166][451] = 1;
TABLE[1166][350] = 1;
TABLE[1166][352] = 1;
TABLE[1166][433] = 1;
TABLE[1166][380] = 1;
TABLE[1166][474] = 1;
TABLE[1166][273] = 1;
TABLE[1166][272] = 1;
TABLE[1166][271] = 1;
TABLE[1166][263] = 1;
TABLE[1166][267] = 1;
TABLE[1166][266] = 1;
TABLE[1166][265] = 1;
TABLE[1166][269] = 1;
TABLE[1166][268] = 1;
TABLE[1166][274] = 1;
TABLE[1166][264] = 1;
TABLE[1166][447] = 1;
TABLE[1166][336] = 1;
TABLE[1166][299] = 1;
TABLE[1166][341] = 1;
TABLE[1166][449] = 1;
TABLE[1166][450] = 1;
TABLE[1166][422] = 1;
TABLE[1166][328] = 1;
TABLE[1166][358] = 1;
TABLE[1166][309] = 1;
TABLE[1166][418] = 1;
TABLE[1166][261] = 1;
TABLE[1166][332] = 1;
TABLE[1166][293] = 1;
TABLE[1166][348] = 1;
TABLE[1166][372] = 1;
TABLE[1166][388] = 1;
TABLE[1166][43] = 1;
TABLE[1166][45] = 1;
TABLE[1166][330] = 1;
TABLE[1166][409] = 1;
TABLE[1166][334] = 1;
TABLE[1166][436] = 1;
TABLE[1166][259] = 1;
TABLE[1166][345] = 1;
TABLE[1166][458] = 1;
TABLE[1166][256] = 1;
TABLE[1166][40] = 1;
TABLE[1166][0] = 1;
TABLE[668][326] = 1;
TABLE[668][374] = 1;
TABLE[668][315] = 1;
TABLE[668][298] = 1;
TABLE[668][413] = 1;
TABLE[668][445] = 1;
TABLE[668][417] = 1;
TABLE[668][472] = 1;
TABLE[668][473] = 1;
TABLE[668][333] = 1;
TABLE[668][414] = 1;
TABLE[668][297] = 1;
TABLE[668][378] = 1;
TABLE[668][461] = 1;
TABLE[668][428] = 1;
TABLE[668][318] = 1;
TABLE[668][350] = 1;
TABLE[668][400] = 1;
TABLE[668][264] = 1;
TABLE[668][274] = 1;
TABLE[668][268] = 1;
TABLE[668][269] = 1;
TABLE[668][265] = 1;
TABLE[668][266] = 1;
TABLE[668][267] = 1;
TABLE[668][263] = 1;
TABLE[668][271] = 1;
TABLE[668][272] = 1;
TABLE[668][273] = 1;
TABLE[668][474] = 1;
TABLE[668][434] = 1;
TABLE[668][377] = 1;
TABLE[668][311] = 1;
TABLE[668][45] = 1;
TABLE[668][43] = 1;
TABLE[668][40] = 1;
TABLE[668][256] = 1;
TABLE[668][458] = 1;
TABLE[668][345] = 1;
TABLE[668][259] = 1;
TABLE[1165][125] = 1;
TABLE[166][326] = 1;
TABLE[167][43] = 1;
TABLE[167][45] = 1;
TABLE[167][259] = 1;
TABLE[167][345] = 1;
TABLE[167][458] = 1;
TABLE[167][256] = 1;
TABLE[167][40] = 1;
TABLE[168][374] = 1;
TABLE[169][315] = 1;
TABLE[170][298] = 1;
TABLE[171][413] = 1;
TABLE[172][445] = 1;
TABLE[173][417] = 1;
TABLE[174][472] = 1;
TABLE[175][473] = 1;
TABLE[176][333] = 1;
TABLE[177][414] = 1;
TABLE[178][297] = 1;
TABLE[179][378] = 1;
TABLE[180][461] = 1;
TABLE[181][428] = 1;
TABLE[182][428] = 1;
TABLE[183][318] = 1;
TABLE[184][318] = 1;
TABLE[185][350] = 1;
TABLE[186][400] = 1;
TABLE[187][474] = 1;
TABLE[187][273] = 1;
TABLE[187][272] = 1;
TABLE[187][271] = 1;
TABLE[187][263] = 1;
TABLE[187][267] = 1;
TABLE[187][266] = 1;
TABLE[187][265] = 1;
TABLE[187][269] = 1;
TABLE[187][268] = 1;
TABLE[187][274] = 1;
TABLE[187][264] = 1;
TABLE[188][311] = 1;
TABLE[188][377] = 1;
TABLE[188][434] = 1;
TABLE[670][58] = 1;
TABLE[1167][257] = 1;
TABLE[671][58] = 1;
TABLE[1168][43] = 1;
TABLE[1168][45] = 1;
TABLE[1168][259] = 1;
TABLE[1168][345] = 1;
TABLE[1168][458] = 1;
TABLE[1168][256] = 1;
TABLE[1168][40] = 1;
TABLE[672][58] = 1;
TABLE[1169][43] = 1;
TABLE[1169][45] = 1;
TABLE[1169][259] = 1;
TABLE[1169][345] = 1;
TABLE[1169][458] = 1;
TABLE[1169][256] = 1;
TABLE[1169][40] = 1;
TABLE[673][308] = 1;
TABLE[1170][58] = 1;
TABLE[1170][43] = 1;
TABLE[1170][45] = 1;
TABLE[1170][259] = 1;
TABLE[1170][345] = 1;
TABLE[1170][458] = 1;
TABLE[1170][256] = 1;
TABLE[1170][40] = 1;
TABLE[674][58] = 1;
TABLE[1171][43] = 1;
TABLE[1171][45] = 1;
TABLE[1171][259] = 1;
TABLE[1171][345] = 1;
TABLE[1171][458] = 1;
TABLE[1171][256] = 1;
TABLE[1171][40] = 1;
TABLE[675][37] = 1;
TABLE[1172][59] = 1;
TABLE[676][58] = 1;
TABLE[1173][43] = 1;
TABLE[1173][45] = 1;
TABLE[1173][259] = 1;
TABLE[1173][345] = 1;
TABLE[1173][458] = 1;
TABLE[1173][256] = 1;
TABLE[1173][40] = 1;
TABLE[677][40] = 1;
TABLE[1174][59] = 1;
TABLE[678][58] = 1;
TABLE[1175][43] = 1;
TABLE[1175][45] = 1;
TABLE[1175][259] = 1;
TABLE[1175][345] = 1;
TABLE[1175][458] = 1;
TABLE[1175][256] = 1;
TABLE[1175][40] = 1;
TABLE[679][405] = 1;
TABLE[1176][59] = 1;
TABLE[680][58] = 1;
TABLE[1177][43] = 1;
TABLE[1177][45] = 1;
TABLE[1177][259] = 1;
TABLE[1177][345] = 1;
TABLE[1177][458] = 1;
TABLE[1177][256] = 1;
TABLE[1177][40] = 1;
TABLE[681][58] = 1;
TABLE[1178][43] = 1;
TABLE[1178][45] = 1;
TABLE[1178][259] = 1;
TABLE[1178][345] = 1;
TABLE[1178][458] = 1;
TABLE[1178][256] = 1;
TABLE[1178][40] = 1;
TABLE[682][58] = 1;
TABLE[1179][43] = 1;
TABLE[1179][45] = 1;
TABLE[1179][259] = 1;
TABLE[1179][345] = 1;
TABLE[1179][458] = 1;
TABLE[1179][256] = 1;
TABLE[1179][40] = 1;
TABLE[683][58] = 1;
TABLE[1180][43] = 1;
TABLE[1180][45] = 1;
TABLE[1180][259] = 1;
TABLE[1180][345] = 1;
TABLE[1180][458] = 1;
TABLE[1180][256] = 1;
TABLE[1180][40] = 1;
TABLE[684][58] = 1;
TABLE[1181][259] = 1;
TABLE[1181][345] = 1;
TABLE[1181][458] = 1;
TABLE[1181][256] = 1;
TABLE[1181][40] = 1;
TABLE[1181][45] = 1;
TABLE[1181][43] = 1;
TABLE[685][58] = 1;
TABLE[1182][43] = 1;
TABLE[1182][45] = 1;
TABLE[1182][259] = 1;
TABLE[1182][345] = 1;
TABLE[1182][458] = 1;
TABLE[1182][256] = 1;
TABLE[1182][40] = 1;
TABLE[686][58] = 1;
TABLE[1183][43] = 1;
TABLE[1183][45] = 1;
TABLE[1183][259] = 1;
TABLE[1183][345] = 1;
TABLE[1183][458] = 1;
TABLE[1183][256] = 1;
TABLE[1183][40] = 1;
TABLE[687][58] = 1;
TABLE[1184][43] = 1;
TABLE[1184][45] = 1;
TABLE[1184][259] = 1;
TABLE[1184][345] = 1;
TABLE[1184][458] = 1;
TABLE[1184][256] = 1;
TABLE[1184][40] = 1;
TABLE[688][58] = 1;
TABLE[1185][36] = 1;
TABLE[689][58] = 1;
TABLE[1186][43] = 1;
TABLE[1186][45] = 1;
TABLE[1186][259] = 1;
TABLE[1186][345] = 1;
TABLE[1186][458] = 1;
TABLE[1186][256] = 1;
TABLE[1186][40] = 1;
TABLE[690][58] = 1;
TABLE[1187][43] = 1;
TABLE[1187][45] = 1;
TABLE[1187][259] = 1;
TABLE[1187][345] = 1;
TABLE[1187][458] = 1;
TABLE[1187][256] = 1;
TABLE[1187][40] = 1;
TABLE[691][58] = 1;
TABLE[1188][275] = 1;
TABLE[692][58] = 1;
TABLE[1189][45] = 1;
TABLE[1189][43] = 1;
TABLE[1189][40] = 1;
TABLE[1189][256] = 1;
TABLE[1189][458] = 1;
TABLE[1189][345] = 1;
TABLE[1189][259] = 1;
TABLE[1189][59] = 1;
TABLE[695][43] = 1;
TABLE[695][45] = 1;
TABLE[695][259] = 1;
TABLE[695][345] = 1;
TABLE[695][458] = 1;
TABLE[695][256] = 1;
TABLE[695][40] = 1;
TABLE[1191][59] = 1;
TABLE[693][44] = 1;
TABLE[694][124] = 1;
TABLE[1190][45] = 1;
TABLE[1190][43] = 1;
TABLE[1190][40] = 1;
TABLE[1190][256] = 1;
TABLE[1190][458] = 1;
TABLE[1190][345] = 1;
TABLE[1190][259] = 1;
TABLE[1190][59] = 1;
TABLE[189][45] = 1;
TABLE[189][43] = 1;
TABLE[189][40] = 1;
TABLE[189][256] = 1;
TABLE[189][458] = 1;
TABLE[189][345] = 1;
TABLE[189][259] = 1;
TABLE[698][43] = 1;
TABLE[698][45] = 1;
TABLE[698][259] = 1;
TABLE[698][345] = 1;
TABLE[698][458] = 1;
TABLE[698][256] = 1;
TABLE[698][40] = 1;
TABLE[696][44] = 1;
TABLE[697][124] = 1;
TABLE[1192][45] = 1;
TABLE[1192][43] = 1;
TABLE[1192][40] = 1;
TABLE[1192][256] = 1;
TABLE[1192][458] = 1;
TABLE[1192][345] = 1;
TABLE[1192][259] = 1;
TABLE[190][261] = 1;
TABLE[191][418] = 1;
TABLE[699][58] = 1;
TABLE[1194][123] = 1;
TABLE[700][125] = 1;
TABLE[700][326] = 1;
TABLE[700][59] = 1;
TABLE[700][359] = 1;
TABLE[700][388] = 1;
TABLE[700][340] = 1;
TABLE[700][432] = 1;
TABLE[700][45] = 1;
TABLE[700][43] = 1;
TABLE[700][350] = 1;
TABLE[700][356] = 1;
TABLE[700][400] = 1;
TABLE[700][277] = 1;
TABLE[700][40] = 1;
TABLE[700][256] = 1;
TABLE[700][458] = 1;
TABLE[700][345] = 1;
TABLE[700][259] = 1;
TABLE[700][434] = 1;
TABLE[700][377] = 1;
TABLE[700][311] = 1;
TABLE[700][264] = 1;
TABLE[700][274] = 1;
TABLE[700][268] = 1;
TABLE[700][269] = 1;
TABLE[700][265] = 1;
TABLE[700][266] = 1;
TABLE[700][267] = 1;
TABLE[700][263] = 1;
TABLE[700][271] = 1;
TABLE[700][272] = 1;
TABLE[700][273] = 1;
TABLE[700][474] = 1;
TABLE[1195][125] = 1;
TABLE[192][326] = 1;
TABLE[193][59] = 1;
TABLE[193][277] = 1;
TABLE[194][359] = 1;
TABLE[195][388] = 1;
TABLE[196][340] = 1;
TABLE[197][340] = 1;
TABLE[198][432] = 1;
TABLE[199][43] = 1;
TABLE[199][45] = 1;
TABLE[199][259] = 1;
TABLE[199][345] = 1;
TABLE[199][458] = 1;
TABLE[199][256] = 1;
TABLE[199][40] = 1;
TABLE[200][350] = 1;
TABLE[201][356] = 1;
TABLE[202][311] = 1;
TABLE[202][377] = 1;
TABLE[202][434] = 1;
TABLE[203][400] = 1;
TABLE[204][474] = 1;
TABLE[204][273] = 1;
TABLE[204][272] = 1;
TABLE[204][271] = 1;
TABLE[204][263] = 1;
TABLE[204][267] = 1;
TABLE[204][266] = 1;
TABLE[204][265] = 1;
TABLE[204][269] = 1;
TABLE[204][268] = 1;
TABLE[204][274] = 1;
TABLE[204][264] = 1;
TABLE[701][58] = 1;
TABLE[1196][257] = 1;
TABLE[704][277] = 1;
TABLE[1199][59] = 1;
TABLE[702][58] = 1;
TABLE[1197][43] = 1;
TABLE[1197][45] = 1;
TABLE[1197][259] = 1;
TABLE[1197][345] = 1;
TABLE[1197][458] = 1;
TABLE[1197][256] = 1;
TABLE[1197][40] = 1;
TABLE[703][44] = 1;
TABLE[1198][277] = 1;
TABLE[1198][59] = 1;
TABLE[705][58] = 1;
TABLE[1200][347] = 1;
TABLE[1200][440] = 1;
TABLE[1200][282] = 1;
TABLE[1200][59] = 1;
TABLE[707][282] = 1;
TABLE[707][440] = 1;
TABLE[707][347] = 1;
TABLE[1202][59] = 1;
TABLE[706][44] = 1;
TABLE[1201][347] = 1;
TABLE[1201][440] = 1;
TABLE[1201][282] = 1;
TABLE[1201][59] = 1;
TABLE[708][58] = 1;
TABLE[1203][36] = 1;
TABLE[709][309] = 1;
TABLE[1204][58] = 1;
TABLE[1204][36] = 1;
TABLE[710][58] = 1;
TABLE[1205][36] = 1;
TABLE[711][309] = 1;
TABLE[1206][58] = 1;
TABLE[1206][36] = 1;
TABLE[712][58] = 1;
TABLE[1207][36] = 1;
TABLE[713][58] = 1;
TABLE[1208][45] = 1;
TABLE[1208][43] = 1;
TABLE[1208][40] = 1;
TABLE[1208][256] = 1;
TABLE[1208][458] = 1;
TABLE[1208][345] = 1;
TABLE[1208][259] = 1;
TABLE[1208][59] = 1;
TABLE[715][43] = 1;
TABLE[715][45] = 1;
TABLE[715][259] = 1;
TABLE[715][345] = 1;
TABLE[715][458] = 1;
TABLE[715][256] = 1;
TABLE[715][40] = 1;
TABLE[1210][59] = 1;
TABLE[714][44] = 1;
TABLE[1209][45] = 1;
TABLE[1209][43] = 1;
TABLE[1209][40] = 1;
TABLE[1209][256] = 1;
TABLE[1209][458] = 1;
TABLE[1209][345] = 1;
TABLE[1209][259] = 1;
TABLE[1209][59] = 1;
TABLE[716][58] = 1;
TABLE[1211][257] = 1;
TABLE[717][58] = 1;
TABLE[1212][45] = 1;
TABLE[1212][43] = 1;
TABLE[1212][40] = 1;
TABLE[1212][256] = 1;
TABLE[1212][458] = 1;
TABLE[1212][345] = 1;
TABLE[1212][259] = 1;
TABLE[1212][59] = 1;
TABLE[720][43] = 1;
TABLE[720][45] = 1;
TABLE[720][259] = 1;
TABLE[720][345] = 1;
TABLE[720][458] = 1;
TABLE[720][256] = 1;
TABLE[720][40] = 1;
TABLE[1214][59] = 1;
TABLE[718][124] = 1;
TABLE[719][44] = 1;
TABLE[1213][45] = 1;
TABLE[1213][43] = 1;
TABLE[1213][40] = 1;
TABLE[1213][256] = 1;
TABLE[1213][458] = 1;
TABLE[1213][345] = 1;
TABLE[1213][259] = 1;
TABLE[1213][59] = 1;
TABLE[205][309] = 1;
TABLE[721][326] = 1;
TABLE[721][364] = 1;
TABLE[721][381] = 1;
TABLE[721][323] = 1;
TABLE[721][296] = 1;
TABLE[721][429] = 1;
TABLE[721][359] = 1;
TABLE[721][432] = 1;
TABLE[721][411] = 1;
TABLE[721][350] = 1;
TABLE[721][356] = 1;
TABLE[721][400] = 1;
TABLE[721][434] = 1;
TABLE[721][377] = 1;
TABLE[721][311] = 1;
TABLE[721][264] = 1;
TABLE[721][274] = 1;
TABLE[721][268] = 1;
TABLE[721][269] = 1;
TABLE[721][265] = 1;
TABLE[721][266] = 1;
TABLE[721][267] = 1;
TABLE[721][263] = 1;
TABLE[721][271] = 1;
TABLE[721][272] = 1;
TABLE[721][273] = 1;
TABLE[721][474] = 1;
TABLE[1215][125] = 1;
TABLE[206][326] = 1;
TABLE[207][364] = 1;
TABLE[208][381] = 1;
TABLE[209][323] = 1;
TABLE[210][296] = 1;
TABLE[211][429] = 1;
TABLE[212][359] = 1;
TABLE[213][432] = 1;
TABLE[214][411] = 1;
TABLE[215][350] = 1;
TABLE[216][356] = 1;
TABLE[217][400] = 1;
TABLE[218][311] = 1;
TABLE[218][377] = 1;
TABLE[218][434] = 1;
TABLE[219][474] = 1;
TABLE[219][273] = 1;
TABLE[219][272] = 1;
TABLE[219][271] = 1;
TABLE[219][263] = 1;
TABLE[219][267] = 1;
TABLE[219][266] = 1;
TABLE[219][265] = 1;
TABLE[219][269] = 1;
TABLE[219][268] = 1;
TABLE[219][274] = 1;
TABLE[219][264] = 1;
TABLE[722][58] = 1;
TABLE[1216][257] = 1;
TABLE[723][58] = 1;
TABLE[1217][100] = 1;
TABLE[1217][43] = 1;
TABLE[1217][45] = 1;
TABLE[1217][259] = 1;
TABLE[1217][345] = 1;
TABLE[1217][458] = 1;
TABLE[1217][256] = 1;
TABLE[1217][40] = 1;
TABLE[724][100] = 1;
TABLE[1218][43] = 1;
TABLE[1218][45] = 1;
TABLE[1218][259] = 1;
TABLE[1218][345] = 1;
TABLE[1218][458] = 1;
TABLE[1218][256] = 1;
TABLE[1218][40] = 1;
TABLE[725][58] = 1;
TABLE[1219][100] = 1;
TABLE[1219][43] = 1;
TABLE[1219][45] = 1;
TABLE[1219][259] = 1;
TABLE[1219][345] = 1;
TABLE[1219][458] = 1;
TABLE[1219][256] = 1;
TABLE[1219][40] = 1;
TABLE[726][100] = 1;
TABLE[1220][43] = 1;
TABLE[1220][45] = 1;
TABLE[1220][259] = 1;
TABLE[1220][345] = 1;
TABLE[1220][458] = 1;
TABLE[1220][256] = 1;
TABLE[1220][40] = 1;
TABLE[727][58] = 1;
TABLE[1221][43] = 1;
TABLE[1221][45] = 1;
TABLE[1221][259] = 1;
TABLE[1221][345] = 1;
TABLE[1221][458] = 1;
TABLE[1221][256] = 1;
TABLE[1221][40] = 1;
TABLE[728][58] = 1;
TABLE[1222][45] = 1;
TABLE[1222][43] = 1;
TABLE[1222][40] = 1;
TABLE[1222][256] = 1;
TABLE[1222][458] = 1;
TABLE[1222][345] = 1;
TABLE[1222][259] = 1;
TABLE[1222][59] = 1;
TABLE[730][43] = 1;
TABLE[730][45] = 1;
TABLE[730][259] = 1;
TABLE[730][345] = 1;
TABLE[730][458] = 1;
TABLE[730][256] = 1;
TABLE[730][40] = 1;
TABLE[1224][59] = 1;
TABLE[729][44] = 1;
TABLE[1223][45] = 1;
TABLE[1223][43] = 1;
TABLE[1223][40] = 1;
TABLE[1223][256] = 1;
TABLE[1223][458] = 1;
TABLE[1223][345] = 1;
TABLE[1223][259] = 1;
TABLE[1223][59] = 1;
TABLE[731][58] = 1;
TABLE[1225][45] = 1;
TABLE[1225][43] = 1;
TABLE[1225][40] = 1;
TABLE[1225][256] = 1;
TABLE[1225][458] = 1;
TABLE[1225][345] = 1;
TABLE[1225][259] = 1;
TABLE[1225][59] = 1;
TABLE[733][43] = 1;
TABLE[733][45] = 1;
TABLE[733][259] = 1;
TABLE[733][345] = 1;
TABLE[733][458] = 1;
TABLE[733][256] = 1;
TABLE[733][40] = 1;
TABLE[1227][59] = 1;
TABLE[732][44] = 1;
TABLE[1226][45] = 1;
TABLE[1226][43] = 1;
TABLE[1226][40] = 1;
TABLE[1226][256] = 1;
TABLE[1226][458] = 1;
TABLE[1226][345] = 1;
TABLE[1226][259] = 1;
TABLE[1226][59] = 1;
TABLE[734][58] = 1;
TABLE[1228][347] = 1;
TABLE[1228][440] = 1;
TABLE[1228][282] = 1;
TABLE[1228][59] = 1;
TABLE[736][282] = 1;
TABLE[736][440] = 1;
TABLE[736][347] = 1;
TABLE[1230][59] = 1;
TABLE[735][44] = 1;
TABLE[1229][347] = 1;
TABLE[1229][440] = 1;
TABLE[1229][282] = 1;
TABLE[1229][59] = 1;
TABLE[737][91] = 1;
TABLE[738][93] = 1;
TABLE[739][58] = 1;
TABLE[1231][45] = 1;
TABLE[1231][43] = 1;
TABLE[1231][40] = 1;
TABLE[1231][256] = 1;
TABLE[1231][458] = 1;
TABLE[1231][345] = 1;
TABLE[1231][259] = 1;
TABLE[1231][59] = 1;
TABLE[741][43] = 1;
TABLE[741][45] = 1;
TABLE[741][259] = 1;
TABLE[741][345] = 1;
TABLE[741][458] = 1;
TABLE[741][256] = 1;
TABLE[741][40] = 1;
TABLE[1233][59] = 1;
TABLE[740][44] = 1;
TABLE[1232][45] = 1;
TABLE[1232][43] = 1;
TABLE[1232][40] = 1;
TABLE[1232][256] = 1;
TABLE[1232][458] = 1;
TABLE[1232][345] = 1;
TABLE[1232][259] = 1;
TABLE[1232][59] = 1;
TABLE[742][58] = 1;
TABLE[1234][45] = 1;
TABLE[1234][43] = 1;
TABLE[1234][40] = 1;
TABLE[1234][256] = 1;
TABLE[1234][458] = 1;
TABLE[1234][345] = 1;
TABLE[1234][259] = 1;
TABLE[1234][59] = 1;
TABLE[744][43] = 1;
TABLE[744][45] = 1;
TABLE[744][259] = 1;
TABLE[744][345] = 1;
TABLE[744][458] = 1;
TABLE[744][256] = 1;
TABLE[744][40] = 1;
TABLE[1236][59] = 1;
TABLE[743][44] = 1;
TABLE[1235][45] = 1;
TABLE[1235][43] = 1;
TABLE[1235][40] = 1;
TABLE[1235][256] = 1;
TABLE[1235][458] = 1;
TABLE[1235][345] = 1;
TABLE[1235][259] = 1;
TABLE[1235][59] = 1;
TABLE[745][58] = 1;
TABLE[1237][45] = 1;
TABLE[1237][43] = 1;
TABLE[1237][40] = 1;
TABLE[1237][256] = 1;
TABLE[1237][458] = 1;
TABLE[1237][345] = 1;
TABLE[1237][259] = 1;
TABLE[1237][59] = 1;
TABLE[748][43] = 1;
TABLE[748][45] = 1;
TABLE[748][259] = 1;
TABLE[748][345] = 1;
TABLE[748][458] = 1;
TABLE[748][256] = 1;
TABLE[748][40] = 1;
TABLE[1239][59] = 1;
TABLE[746][124] = 1;
TABLE[747][44] = 1;
TABLE[1238][45] = 1;
TABLE[1238][43] = 1;
TABLE[1238][40] = 1;
TABLE[1238][256] = 1;
TABLE[1238][458] = 1;
TABLE[1238][345] = 1;
TABLE[1238][259] = 1;
TABLE[1238][59] = 1;
TABLE[220][282] = 1;
TABLE[221][440] = 1;
TABLE[222][347] = 1;
TABLE[749][44] = 1;
TABLE[1240][93] = 1;
TABLE[750][44] = 1;
TABLE[1241][93] = 1;
TABLE[751][44] = 1;
TABLE[1242][93] = 1;
TABLE[223][44] = 1;
TABLE[224][44] = 1;
TABLE[225][44] = 1;
TABLE[226][43] = 1;
TABLE[226][45] = 1;
TABLE[226][259] = 1;
TABLE[226][345] = 1;
TABLE[226][458] = 1;
TABLE[226][256] = 1;
TABLE[226][40] = 1;
TABLE[227][343] = 1;
TABLE[228][343] = 1;
TABLE[229][343] = 1;
TABLE[230][343] = 1;
TABLE[752][374] = 1;
TABLE[1243][44] = 1;
TABLE[1243][347] = 1;
TABLE[1243][440] = 1;
TABLE[1243][282] = 1;
TABLE[1243][59] = 1;
TABLE[753][374] = 1;
TABLE[1244][44] = 1;
TABLE[1244][347] = 1;
TABLE[1244][440] = 1;
TABLE[1244][282] = 1;
TABLE[1244][59] = 1;
TABLE[754][374] = 1;
TABLE[1245][44] = 1;
TABLE[1245][347] = 1;
TABLE[1245][440] = 1;
TABLE[1245][282] = 1;
TABLE[1245][59] = 1;
TABLE[231][332] = 1;
TABLE[755][326] = 1;
TABLE[755][400] = 1;
TABLE[755][434] = 1;
TABLE[755][377] = 1;
TABLE[755][311] = 1;
TABLE[755][264] = 1;
TABLE[755][274] = 1;
TABLE[755][268] = 1;
TABLE[755][269] = 1;
TABLE[755][265] = 1;
TABLE[755][266] = 1;
TABLE[755][267] = 1;
TABLE[755][263] = 1;
TABLE[755][271] = 1;
TABLE[755][272] = 1;
TABLE[755][273] = 1;
TABLE[755][474] = 1;
TABLE[1246][125] = 1;
TABLE[232][326] = 1;
TABLE[233][311] = 1;
TABLE[233][377] = 1;
TABLE[233][434] = 1;
TABLE[234][400] = 1;
TABLE[235][474] = 1;
TABLE[235][273] = 1;
TABLE[235][272] = 1;
TABLE[235][271] = 1;
TABLE[235][263] = 1;
TABLE[235][267] = 1;
TABLE[235][266] = 1;
TABLE[235][265] = 1;
TABLE[235][269] = 1;
TABLE[235][268] = 1;
TABLE[235][274] = 1;
TABLE[235][264] = 1;
TABLE[756][58] = 1;
TABLE[1247][257] = 1;
TABLE[236][311] = 1;
TABLE[237][377] = 1;
TABLE[238][434] = 1;
TABLE[757][58] = 1;
TABLE[1248][42] = 1;
TABLE[1248][59] = 1;
TABLE[759][42] = 1;
TABLE[761][42] = 1;
TABLE[1251][59] = 1;
TABLE[758][44] = 1;
TABLE[1249][42] = 1;
TABLE[1249][59] = 1;
TABLE[760][44] = 1;
TABLE[1250][42] = 1;
TABLE[1250][59] = 1;
TABLE[762][58] = 1;
TABLE[1252][42] = 1;
TABLE[1252][400] = 1;
TABLE[1252][356] = 1;
TABLE[1252][409] = 1;
TABLE[1252][330] = 1;
TABLE[1252][437] = 1;
TABLE[1252][389] = 1;
TABLE[1252][435] = 1;
TABLE[1252][431] = 1;
TABLE[1252][350] = 1;
TABLE[1252][346] = 1;
TABLE[1252][381] = 1;
TABLE[1252][292] = 1;
TABLE[1252][323] = 1;
TABLE[1252][294] = 1;
TABLE[1252][296] = 1;
TABLE[1252][369] = 1;
TABLE[1252][424] = 1;
TABLE[1252][440] = 1;
TABLE[1252][366] = 1;
TABLE[1252][365] = 1;
TABLE[1252][364] = 1;
TABLE[1252][450] = 1;
TABLE[1252][325] = 1;
TABLE[1252][326] = 1;
TABLE[1252][474] = 1;
TABLE[1252][273] = 1;
TABLE[1252][272] = 1;
TABLE[1252][271] = 1;
TABLE[1252][263] = 1;
TABLE[1252][267] = 1;
TABLE[1252][266] = 1;
TABLE[1252][265] = 1;
TABLE[1252][269] = 1;
TABLE[1252][268] = 1;
TABLE[1252][274] = 1;
TABLE[1252][264] = 1;
TABLE[1252][311] = 1;
TABLE[1252][377] = 1;
TABLE[1252][434] = 1;
TABLE[1252][277] = 1;
TABLE[1252][357] = 1;
TABLE[1252][368] = 1;
TABLE[1252][43] = 1;
TABLE[1252][374] = 1;
TABLE[1252][307] = 1;
TABLE[1252][125] = 1;
TABLE[1252][355] = 1;
TABLE[1252][375] = 1;
TABLE[1252][376] = 1;
TABLE[1252][313] = 1;
TABLE[1252][456] = 1;
TABLE[1252][379] = 1;
TABLE[1252][467] = 1;
TABLE[1252][468] = 1;
TABLE[1252][304] = 1;
TABLE[1252][361] = 1;
TABLE[1252][349] = 1;
TABLE[1252][407] = 1;
TABLE[1252][312] = 1;
TABLE[1252][396] = 1;
TABLE[1252][466] = 1;
TABLE[1252][315] = 1;
TABLE[1252][383] = 1;
TABLE[1252][420] = 1;
TABLE[1252][404] = 1;
TABLE[1252][284] = 1;
TABLE[1252][452] = 1;
TABLE[1252][316] = 1;
TABLE[1252][373] = 1;
TABLE[1252][430] = 1;
TABLE[1252][344] = 1;
TABLE[1252][472] = 1;
TABLE[1252][448] = 1;
TABLE[1252][334] = 1;
TABLE[1252][318] = 1;
TABLE[1252][428] = 1;
TABLE[1252][461] = 1;
TABLE[1252][378] = 1;
TABLE[1252][297] = 1;
TABLE[1252][414] = 1;
TABLE[1252][333] = 1;
TABLE[1252][473] = 1;
TABLE[1252][417] = 1;
TABLE[1252][445] = 1;
TABLE[1252][413] = 1;
TABLE[1252][298] = 1;
TABLE[1252][45] = 1;
TABLE[1252][259] = 1;
TABLE[1252][345] = 1;
TABLE[1252][458] = 1;
TABLE[1252][256] = 1;
TABLE[1252][40] = 1;
TABLE[1252][432] = 1;
TABLE[1252][340] = 1;
TABLE[1252][388] = 1;
TABLE[1252][359] = 1;
TABLE[1252][59] = 1;
TABLE[1252][411] = 1;
TABLE[1252][429] = 1;
TABLE[1252][329] = 1;
TABLE[1252][398] = 1;
TABLE[1252][423] = 1;
TABLE[1252][455] = 1;
TABLE[1252][360] = 1;
TABLE[1252][426] = 1;
TABLE[1252][327] = 1;
TABLE[1252][351] = 1;
TABLE[1252][464] = 1;
TABLE[1252][306] = 1;
TABLE[1252][410] = 1;
TABLE[1252][319] = 1;
TABLE[1252][390] = 1;
TABLE[1252][353] = 1;
TABLE[1252][406] = 1;
TABLE[1252][285] = 1;
TABLE[1252][436] = 1;
TABLE[766][42] = 1;
TABLE[1254][400] = 1;
TABLE[1254][356] = 1;
TABLE[1254][409] = 1;
TABLE[1254][330] = 1;
TABLE[1254][437] = 1;
TABLE[1254][389] = 1;
TABLE[1254][435] = 1;
TABLE[1254][431] = 1;
TABLE[1254][350] = 1;
TABLE[1254][346] = 1;
TABLE[1254][381] = 1;
TABLE[1254][292] = 1;
TABLE[1254][323] = 1;
TABLE[1254][294] = 1;
TABLE[1254][296] = 1;
TABLE[1254][369] = 1;
TABLE[1254][424] = 1;
TABLE[1254][440] = 1;
TABLE[1254][366] = 1;
TABLE[1254][365] = 1;
TABLE[1254][364] = 1;
TABLE[1254][450] = 1;
TABLE[1254][325] = 1;
TABLE[1254][326] = 1;
TABLE[1254][474] = 1;
TABLE[1254][273] = 1;
TABLE[1254][272] = 1;
TABLE[1254][271] = 1;
TABLE[1254][263] = 1;
TABLE[1254][267] = 1;
TABLE[1254][266] = 1;
TABLE[1254][265] = 1;
TABLE[1254][269] = 1;
TABLE[1254][268] = 1;
TABLE[1254][274] = 1;
TABLE[1254][264] = 1;
TABLE[1254][311] = 1;
TABLE[1254][377] = 1;
TABLE[1254][434] = 1;
TABLE[1254][277] = 1;
TABLE[1254][357] = 1;
TABLE[1254][368] = 1;
TABLE[1254][43] = 1;
TABLE[1254][374] = 1;
TABLE[1254][307] = 1;
TABLE[1254][125] = 1;
TABLE[1254][355] = 1;
TABLE[1254][375] = 1;
TABLE[1254][376] = 1;
TABLE[1254][313] = 1;
TABLE[1254][456] = 1;
TABLE[1254][379] = 1;
TABLE[1254][467] = 1;
TABLE[1254][468] = 1;
TABLE[1254][304] = 1;
TABLE[1254][361] = 1;
TABLE[1254][349] = 1;
TABLE[1254][407] = 1;
TABLE[1254][312] = 1;
TABLE[1254][396] = 1;
TABLE[1254][466] = 1;
TABLE[1254][315] = 1;
TABLE[1254][383] = 1;
TABLE[1254][420] = 1;
TABLE[1254][404] = 1;
TABLE[1254][284] = 1;
TABLE[1254][452] = 1;
TABLE[1254][316] = 1;
TABLE[1254][373] = 1;
TABLE[1254][430] = 1;
TABLE[1254][344] = 1;
TABLE[1254][472] = 1;
TABLE[1254][448] = 1;
TABLE[1254][334] = 1;
TABLE[1254][318] = 1;
TABLE[1254][428] = 1;
TABLE[1254][461] = 1;
TABLE[1254][378] = 1;
TABLE[1254][297] = 1;
TABLE[1254][414] = 1;
TABLE[1254][333] = 1;
TABLE[1254][473] = 1;
TABLE[1254][417] = 1;
TABLE[1254][445] = 1;
TABLE[1254][413] = 1;
TABLE[1254][298] = 1;
TABLE[1254][45] = 1;
TABLE[1254][259] = 1;
TABLE[1254][345] = 1;
TABLE[1254][458] = 1;
TABLE[1254][256] = 1;
TABLE[1254][40] = 1;
TABLE[1254][432] = 1;
TABLE[1254][340] = 1;
TABLE[1254][388] = 1;
TABLE[1254][359] = 1;
TABLE[1254][59] = 1;
TABLE[1254][411] = 1;
TABLE[1254][429] = 1;
TABLE[1254][329] = 1;
TABLE[1254][398] = 1;
TABLE[1254][423] = 1;
TABLE[1254][455] = 1;
TABLE[1254][360] = 1;
TABLE[1254][426] = 1;
TABLE[1254][327] = 1;
TABLE[1254][351] = 1;
TABLE[1254][464] = 1;
TABLE[1254][306] = 1;
TABLE[1254][410] = 1;
TABLE[1254][319] = 1;
TABLE[1254][390] = 1;
TABLE[1254][353] = 1;
TABLE[1254][406] = 1;
TABLE[1254][285] = 1;
TABLE[1254][436] = 1;
TABLE[763][36] = 1;
TABLE[763][259] = 1;
TABLE[763][345] = 1;
TABLE[763][458] = 1;
TABLE[763][256] = 1;
TABLE[763][40] = 1;
TABLE[763][257] = 1;
TABLE[1253][44] = 1;
TABLE[1253][59] = 1;
TABLE[764][44] = 1;
TABLE[765][59] = 1;
TABLE[767][58] = 1;
TABLE[1255][42] = 1;
TABLE[1255][59] = 1;
TABLE[773][42] = 1;
TABLE[1258][59] = 1;
TABLE[768][40] = 1;
TABLE[768][256] = 1;
TABLE[768][458] = 1;
TABLE[768][345] = 1;
TABLE[768][259] = 1;
TABLE[1256][36] = 1;
TABLE[769][36] = 1;
TABLE[770][36] = 1;
TABLE[771][36] = 1;
TABLE[772][44] = 1;
TABLE[1257][42] = 1;
TABLE[1257][59] = 1;
TABLE[239][36] = 1;
TABLE[240][40] = 1;
TABLE[240][256] = 1;
TABLE[240][458] = 1;
TABLE[240][345] = 1;
TABLE[240][259] = 1;
TABLE[241][257] = 1;
TABLE[242][293] = 1;
TABLE[774][125] = 1;
TABLE[774][326] = 1;
TABLE[774][372] = 1;
TABLE[774][59] = 1;
TABLE[774][424] = 1;
TABLE[774][444] = 1;
TABLE[774][415] = 1;
TABLE[774][338] = 1;
TABLE[774][470] = 1;
TABLE[774][366] = 1;
TABLE[774][371] = 1;
TABLE[774][400] = 1;
TABLE[774][277] = 1;
TABLE[774][264] = 1;
TABLE[774][274] = 1;
TABLE[774][268] = 1;
TABLE[774][269] = 1;
TABLE[774][265] = 1;
TABLE[774][266] = 1;
TABLE[774][267] = 1;
TABLE[774][263] = 1;
TABLE[774][271] = 1;
TABLE[774][272] = 1;
TABLE[774][273] = 1;
TABLE[774][474] = 1;
TABLE[1259][125] = 1;
TABLE[243][326] = 1;
TABLE[244][372] = 1;
TABLE[245][59] = 1;
TABLE[245][277] = 1;
TABLE[246][424] = 1;
TABLE[247][444] = 1;
TABLE[248][415] = 1;
TABLE[249][338] = 1;
TABLE[250][470] = 1;
TABLE[251][366] = 1;
TABLE[252][371] = 1;
TABLE[253][400] = 1;
TABLE[254][474] = 1;
TABLE[254][273] = 1;
TABLE[254][272] = 1;
TABLE[254][271] = 1;
TABLE[254][263] = 1;
TABLE[254][267] = 1;
TABLE[254][266] = 1;
TABLE[254][265] = 1;
TABLE[254][269] = 1;
TABLE[254][268] = 1;
TABLE[254][274] = 1;
TABLE[254][264] = 1;
TABLE[775][58] = 1;
TABLE[1260][257] = 1;
TABLE[776][58] = 1;
TABLE[1261][36] = 1;
TABLE[779][277] = 1;
TABLE[1264][59] = 1;
TABLE[777][58] = 1;
TABLE[1262][43] = 1;
TABLE[1262][45] = 1;
TABLE[1262][259] = 1;
TABLE[1262][345] = 1;
TABLE[1262][458] = 1;
TABLE[1262][256] = 1;
TABLE[1262][40] = 1;
TABLE[778][44] = 1;
TABLE[1263][277] = 1;
TABLE[1263][59] = 1;
TABLE[780][58] = 1;
TABLE[1265][45] = 1;
TABLE[1265][43] = 1;
TABLE[1265][40] = 1;
TABLE[1265][256] = 1;
TABLE[1265][458] = 1;
TABLE[1265][345] = 1;
TABLE[1265][259] = 1;
TABLE[1265][59] = 1;
TABLE[782][43] = 1;
TABLE[782][45] = 1;
TABLE[782][259] = 1;
TABLE[782][345] = 1;
TABLE[782][458] = 1;
TABLE[782][256] = 1;
TABLE[782][40] = 1;
TABLE[1267][59] = 1;
TABLE[781][44] = 1;
TABLE[1266][45] = 1;
TABLE[1266][43] = 1;
TABLE[1266][40] = 1;
TABLE[1266][256] = 1;
TABLE[1266][458] = 1;
TABLE[1266][345] = 1;
TABLE[1266][259] = 1;
TABLE[1266][59] = 1;
TABLE[783][58] = 1;
TABLE[1268][277] = 1;
TABLE[1268][59] = 1;
TABLE[785][277] = 1;
TABLE[1270][59] = 1;
TABLE[784][44] = 1;
TABLE[1269][277] = 1;
TABLE[1269][59] = 1;
TABLE[786][58] = 1;
TABLE[1271][45] = 1;
TABLE[1271][43] = 1;
TABLE[1271][40] = 1;
TABLE[1271][256] = 1;
TABLE[1271][458] = 1;
TABLE[1271][345] = 1;
TABLE[1271][259] = 1;
TABLE[1271][59] = 1;
TABLE[788][43] = 1;
TABLE[788][45] = 1;
TABLE[788][259] = 1;
TABLE[788][345] = 1;
TABLE[788][458] = 1;
TABLE[788][256] = 1;
TABLE[788][40] = 1;
TABLE[1273][59] = 1;
TABLE[787][44] = 1;
TABLE[1272][45] = 1;
TABLE[1272][43] = 1;
TABLE[1272][40] = 1;
TABLE[1272][256] = 1;
TABLE[1272][458] = 1;
TABLE[1272][345] = 1;
TABLE[1272][259] = 1;
TABLE[1272][59] = 1;
TABLE[790][440] = 1;
TABLE[790][36] = 1;
TABLE[790][370] = 1;
TABLE[790][347] = 1;
TABLE[1274][59] = 1;
TABLE[789][44] = 1;
TABLE[792][440] = 1;
TABLE[792][36] = 1;
TABLE[792][370] = 1;
TABLE[792][347] = 1;
TABLE[1275][59] = 1;
TABLE[791][44] = 1;
TABLE[794][440] = 1;
TABLE[794][36] = 1;
TABLE[794][370] = 1;
TABLE[794][347] = 1;
TABLE[1276][59] = 1;
TABLE[793][44] = 1;
TABLE[796][440] = 1;
TABLE[796][36] = 1;
TABLE[796][370] = 1;
TABLE[796][347] = 1;
TABLE[1277][59] = 1;
TABLE[795][44] = 1;
TABLE[255][440] = 1;
TABLE[256][440] = 1;
TABLE[257][36] = 1;
TABLE[258][36] = 1;
TABLE[259][36] = 1;
TABLE[260][36] = 1;
TABLE[261][370] = 1;
TABLE[262][347] = 1;
TABLE[797][295] = 1;
TABLE[1278][44] = 1;
TABLE[798][47] = 1;
TABLE[799][408] = 1;
TABLE[800][47] = 1;
TABLE[801][408] = 1;
TABLE[263][328] = 1;
TABLE[802][437] = 1;
TABLE[802][359] = 1;
TABLE[802][326] = 1;
TABLE[802][400] = 1;
TABLE[802][434] = 1;
TABLE[802][377] = 1;
TABLE[802][311] = 1;
TABLE[802][264] = 1;
TABLE[802][274] = 1;
TABLE[802][268] = 1;
TABLE[802][269] = 1;
TABLE[802][265] = 1;
TABLE[802][266] = 1;
TABLE[802][267] = 1;
TABLE[802][263] = 1;
TABLE[802][271] = 1;
TABLE[802][272] = 1;
TABLE[802][273] = 1;
TABLE[802][474] = 1;
TABLE[1279][125] = 1;
TABLE[264][437] = 1;
TABLE[265][359] = 1;
TABLE[266][326] = 1;
TABLE[267][400] = 1;
TABLE[268][311] = 1;
TABLE[268][377] = 1;
TABLE[268][434] = 1;
TABLE[269][474] = 1;
TABLE[269][273] = 1;
TABLE[269][272] = 1;
TABLE[269][271] = 1;
TABLE[269][263] = 1;
TABLE[269][267] = 1;
TABLE[269][266] = 1;
TABLE[269][265] = 1;
TABLE[269][269] = 1;
TABLE[269][268] = 1;
TABLE[269][274] = 1;
TABLE[269][264] = 1;
TABLE[803][58] = 1;
TABLE[1280][36] = 1;
TABLE[1280][59] = 1;
TABLE[805][36] = 1;
TABLE[1282][59] = 1;
TABLE[804][44] = 1;
TABLE[1281][36] = 1;
TABLE[1281][59] = 1;
TABLE[806][58] = 1;
TABLE[1283][347] = 1;
TABLE[1283][440] = 1;
TABLE[1283][282] = 1;
TABLE[1283][59] = 1;
TABLE[808][282] = 1;
TABLE[808][440] = 1;
TABLE[808][347] = 1;
TABLE[1285][59] = 1;
TABLE[807][44] = 1;
TABLE[1284][347] = 1;
TABLE[1284][440] = 1;
TABLE[1284][282] = 1;
TABLE[1284][59] = 1;
TABLE[809][58] = 1;
TABLE[1286][257] = 1;
TABLE[270][358] = 1;
TABLE[810][329] = 1;
TABLE[810][311] = 1;
TABLE[810][350] = 1;
TABLE[810][326] = 1;
TABLE[810][359] = 1;
TABLE[810][400] = 1;
TABLE[810][434] = 1;
TABLE[810][377] = 1;
TABLE[810][264] = 1;
TABLE[810][274] = 1;
TABLE[810][268] = 1;
TABLE[810][269] = 1;
TABLE[810][265] = 1;
TABLE[810][266] = 1;
TABLE[810][267] = 1;
TABLE[810][263] = 1;
TABLE[810][271] = 1;
TABLE[810][272] = 1;
TABLE[810][273] = 1;
TABLE[810][474] = 1;
TABLE[1287][125] = 1;
TABLE[271][329] = 1;
TABLE[272][311] = 1;
TABLE[273][350] = 1;
TABLE[274][326] = 1;
TABLE[275][359] = 1;
TABLE[276][400] = 1;
TABLE[277][311] = 1;
TABLE[277][377] = 1;
TABLE[277][434] = 1;
TABLE[278][474] = 1;
TABLE[278][273] = 1;
TABLE[278][272] = 1;
TABLE[278][271] = 1;
TABLE[278][263] = 1;
TABLE[278][267] = 1;
TABLE[278][266] = 1;
TABLE[278][265] = 1;
TABLE[278][269] = 1;
TABLE[278][268] = 1;
TABLE[278][274] = 1;
TABLE[278][264] = 1;
TABLE[811][58] = 1;
TABLE[1288][36] = 1;
TABLE[1288][59] = 1;
TABLE[813][36] = 1;
TABLE[1290][59] = 1;
TABLE[812][44] = 1;
TABLE[1289][36] = 1;
TABLE[1289][59] = 1;
TABLE[814][58] = 1;
TABLE[1291][45] = 1;
TABLE[1291][43] = 1;
TABLE[1291][40] = 1;
TABLE[1291][256] = 1;
TABLE[1291][458] = 1;
TABLE[1291][345] = 1;
TABLE[1291][259] = 1;
TABLE[1291][59] = 1;
TABLE[817][43] = 1;
TABLE[817][45] = 1;
TABLE[817][259] = 1;
TABLE[817][345] = 1;
TABLE[817][458] = 1;
TABLE[817][256] = 1;
TABLE[817][40] = 1;
TABLE[1293][59] = 1;
TABLE[815][124] = 1;
TABLE[816][44] = 1;
TABLE[1292][45] = 1;
TABLE[1292][43] = 1;
TABLE[1292][40] = 1;
TABLE[1292][256] = 1;
TABLE[1292][458] = 1;
TABLE[1292][345] = 1;
TABLE[1292][259] = 1;
TABLE[1292][59] = 1;
TABLE[818][58] = 1;
TABLE[1294][257] = 1;
TABLE[819][58] = 1;
TABLE[1295][347] = 1;
TABLE[1295][440] = 1;
TABLE[1295][282] = 1;
TABLE[1295][59] = 1;
TABLE[821][282] = 1;
TABLE[821][440] = 1;
TABLE[821][347] = 1;
TABLE[1297][59] = 1;
TABLE[820][44] = 1;
TABLE[1296][347] = 1;
TABLE[1296][440] = 1;
TABLE[1296][282] = 1;
TABLE[1296][59] = 1;
TABLE[279][450] = 1;
TABLE[822][326] = 1;
TABLE[822][389] = 1;
TABLE[822][407] = 1;
TABLE[822][383] = 1;
TABLE[822][350] = 1;
TABLE[822][434] = 1;
TABLE[822][377] = 1;
TABLE[822][311] = 1;
TABLE[822][400] = 1;
TABLE[822][368] = 1;
TABLE[822][357] = 1;
TABLE[822][264] = 1;
TABLE[822][274] = 1;
TABLE[822][268] = 1;
TABLE[822][269] = 1;
TABLE[822][265] = 1;
TABLE[822][266] = 1;
TABLE[822][267] = 1;
TABLE[822][263] = 1;
TABLE[822][271] = 1;
TABLE[822][272] = 1;
TABLE[822][273] = 1;
TABLE[822][474] = 1;
TABLE[1298][125] = 1;
TABLE[280][326] = 1;
TABLE[281][357] = 1;
TABLE[281][368] = 1;
TABLE[282][389] = 1;
TABLE[283][407] = 1;
TABLE[284][383] = 1;
TABLE[285][350] = 1;
TABLE[286][311] = 1;
TABLE[286][377] = 1;
TABLE[286][434] = 1;
TABLE[287][400] = 1;
TABLE[288][474] = 1;
TABLE[288][273] = 1;
TABLE[288][272] = 1;
TABLE[288][271] = 1;
TABLE[288][263] = 1;
TABLE[288][267] = 1;
TABLE[288][266] = 1;
TABLE[288][265] = 1;
TABLE[288][269] = 1;
TABLE[288][268] = 1;
TABLE[288][274] = 1;
TABLE[288][264] = 1;
TABLE[823][58] = 1;
TABLE[1299][257] = 1;
TABLE[824][58] = 1;
TABLE[1300][43] = 1;
TABLE[1300][45] = 1;
TABLE[1300][259] = 1;
TABLE[1300][345] = 1;
TABLE[1300][458] = 1;
TABLE[1300][256] = 1;
TABLE[1300][40] = 1;
TABLE[825][58] = 1;
TABLE[1301][43] = 1;
TABLE[1301][45] = 1;
TABLE[1301][259] = 1;
TABLE[1301][345] = 1;
TABLE[1301][458] = 1;
TABLE[1301][256] = 1;
TABLE[1301][40] = 1;
TABLE[826][58] = 1;
TABLE[1302][43] = 1;
TABLE[1302][45] = 1;
TABLE[1302][259] = 1;
TABLE[1302][345] = 1;
TABLE[1302][458] = 1;
TABLE[1302][256] = 1;
TABLE[1302][40] = 1;
TABLE[827][58] = 1;
TABLE[1303][45] = 1;
TABLE[1303][43] = 1;
TABLE[1303][40] = 1;
TABLE[1303][256] = 1;
TABLE[1303][458] = 1;
TABLE[1303][345] = 1;
TABLE[1303][259] = 1;
TABLE[1303][59] = 1;
TABLE[830][43] = 1;
TABLE[830][45] = 1;
TABLE[830][259] = 1;
TABLE[830][345] = 1;
TABLE[830][458] = 1;
TABLE[830][256] = 1;
TABLE[830][40] = 1;
TABLE[1305][59] = 1;
TABLE[828][124] = 1;
TABLE[829][44] = 1;
TABLE[1304][45] = 1;
TABLE[1304][43] = 1;
TABLE[1304][40] = 1;
TABLE[1304][256] = 1;
TABLE[1304][458] = 1;
TABLE[1304][345] = 1;
TABLE[1304][259] = 1;
TABLE[1304][59] = 1;
TABLE[289][422] = 1;
TABLE[831][326] = 1;
TABLE[831][464] = 1;
TABLE[831][351] = 1;
TABLE[831][327] = 1;
TABLE[831][325] = 1;
TABLE[831][431] = 1;
TABLE[831][426] = 1;
TABLE[831][350] = 1;
TABLE[831][360] = 1;
TABLE[831][455] = 1;
TABLE[831][423] = 1;
TABLE[831][398] = 1;
TABLE[831][434] = 1;
TABLE[831][377] = 1;
TABLE[831][311] = 1;
TABLE[831][400] = 1;
TABLE[831][264] = 1;
TABLE[831][274] = 1;
TABLE[831][268] = 1;
TABLE[831][269] = 1;
TABLE[831][265] = 1;
TABLE[831][266] = 1;
TABLE[831][267] = 1;
TABLE[831][263] = 1;
TABLE[831][271] = 1;
TABLE[831][272] = 1;
TABLE[831][273] = 1;
TABLE[831][474] = 1;
TABLE[1306][125] = 1;
TABLE[290][326] = 1;
TABLE[291][464] = 1;
TABLE[292][351] = 1;
TABLE[293][327] = 1;
TABLE[294][325] = 1;
TABLE[295][431] = 1;
TABLE[296][426] = 1;
TABLE[297][350] = 1;
TABLE[298][360] = 1;
TABLE[299][455] = 1;
TABLE[300][423] = 1;
TABLE[301][398] = 1;
TABLE[302][311] = 1;
TABLE[302][377] = 1;
TABLE[302][434] = 1;
TABLE[303][400] = 1;
TABLE[304][474] = 1;
TABLE[304][273] = 1;
TABLE[304][272] = 1;
TABLE[304][271] = 1;
TABLE[304][263] = 1;
TABLE[304][267] = 1;
TABLE[304][266] = 1;
TABLE[304][265] = 1;
TABLE[304][269] = 1;
TABLE[304][268] = 1;
TABLE[304][274] = 1;
TABLE[304][264] = 1;
TABLE[832][58] = 1;
TABLE[1307][257] = 1;
TABLE[833][58] = 1;
TABLE[1308][36] = 1;
TABLE[834][58] = 1;
TABLE[1309][36] = 1;
TABLE[835][58] = 1;
TABLE[1310][36] = 1;
TABLE[836][58] = 1;
TABLE[1311][43] = 1;
TABLE[1311][45] = 1;
TABLE[1311][259] = 1;
TABLE[1311][345] = 1;
TABLE[1311][458] = 1;
TABLE[1311][256] = 1;
TABLE[1311][40] = 1;
TABLE[837][58] = 1;
TABLE[1312][43] = 1;
TABLE[1312][45] = 1;
TABLE[1312][259] = 1;
TABLE[1312][345] = 1;
TABLE[1312][458] = 1;
TABLE[1312][256] = 1;
TABLE[1312][40] = 1;
TABLE[838][58] = 1;
TABLE[1313][45] = 1;
TABLE[1313][43] = 1;
TABLE[1313][40] = 1;
TABLE[1313][256] = 1;
TABLE[1313][458] = 1;
TABLE[1313][345] = 1;
TABLE[1313][259] = 1;
TABLE[1313][59] = 1;
TABLE[841][43] = 1;
TABLE[841][45] = 1;
TABLE[841][259] = 1;
TABLE[841][345] = 1;
TABLE[841][458] = 1;
TABLE[841][256] = 1;
TABLE[841][40] = 1;
TABLE[1315][59] = 1;
TABLE[839][44] = 1;
TABLE[840][124] = 1;
TABLE[1314][45] = 1;
TABLE[1314][43] = 1;
TABLE[1314][40] = 1;
TABLE[1314][256] = 1;
TABLE[1314][458] = 1;
TABLE[1314][345] = 1;
TABLE[1314][259] = 1;
TABLE[1314][59] = 1;
TABLE[842][58] = 1;
TABLE[1316][45] = 1;
TABLE[1316][43] = 1;
TABLE[1316][40] = 1;
TABLE[1316][256] = 1;
TABLE[1316][458] = 1;
TABLE[1316][345] = 1;
TABLE[1316][259] = 1;
TABLE[1316][59] = 1;
TABLE[845][43] = 1;
TABLE[845][45] = 1;
TABLE[845][259] = 1;
TABLE[845][345] = 1;
TABLE[845][458] = 1;
TABLE[845][256] = 1;
TABLE[845][40] = 1;
TABLE[1318][59] = 1;
TABLE[843][124] = 1;
TABLE[844][44] = 1;
TABLE[1317][45] = 1;
TABLE[1317][43] = 1;
TABLE[1317][40] = 1;
TABLE[1317][256] = 1;
TABLE[1317][458] = 1;
TABLE[1317][345] = 1;
TABLE[1317][259] = 1;
TABLE[1317][59] = 1;
TABLE[858][259] = 1;
TABLE[1324][59] = 1;
TABLE[846][91] = 1;
TABLE[855][471] = 1;
TABLE[856][294] = 1;
TABLE[1322][44] = 1;
TABLE[1322][259] = 1;
TABLE[1322][59] = 1;
TABLE[847][36] = 1;
TABLE[849][419] = 1;
TABLE[851][419] = 1;
TABLE[848][43] = 1;
TABLE[848][45] = 1;
TABLE[848][259] = 1;
TABLE[848][345] = 1;
TABLE[848][458] = 1;
TABLE[848][256] = 1;
TABLE[848][40] = 1;
TABLE[1319][399] = 1;
TABLE[1319][91] = 1;
TABLE[1319][471] = 1;
TABLE[1319][294] = 1;
TABLE[1319][44] = 1;
TABLE[1319][259] = 1;
TABLE[1319][59] = 1;
TABLE[850][43] = 1;
TABLE[850][45] = 1;
TABLE[850][259] = 1;
TABLE[850][345] = 1;
TABLE[850][458] = 1;
TABLE[850][256] = 1;
TABLE[850][40] = 1;
TABLE[1320][399] = 1;
TABLE[1320][91] = 1;
TABLE[1320][471] = 1;
TABLE[1320][294] = 1;
TABLE[1320][44] = 1;
TABLE[1320][259] = 1;
TABLE[1320][59] = 1;
TABLE[852][399] = 1;
TABLE[853][399] = 1;
TABLE[854][399] = 1;
TABLE[1321][91] = 1;
TABLE[1321][471] = 1;
TABLE[1321][294] = 1;
TABLE[1321][44] = 1;
TABLE[1321][259] = 1;
TABLE[1321][59] = 1;
TABLE[857][44] = 1;
TABLE[1323][259] = 1;
TABLE[1323][59] = 1;
TABLE[859][326] = 1;
TABLE[859][464] = 1;
TABLE[859][351] = 1;
TABLE[859][327] = 1;
TABLE[859][325] = 1;
TABLE[859][431] = 1;
TABLE[859][426] = 1;
TABLE[859][350] = 1;
TABLE[859][360] = 1;
TABLE[859][455] = 1;
TABLE[859][423] = 1;
TABLE[859][398] = 1;
TABLE[859][434] = 1;
TABLE[859][377] = 1;
TABLE[859][311] = 1;
TABLE[859][400] = 1;
TABLE[859][264] = 1;
TABLE[859][274] = 1;
TABLE[859][268] = 1;
TABLE[859][269] = 1;
TABLE[859][265] = 1;
TABLE[859][266] = 1;
TABLE[859][267] = 1;
TABLE[859][263] = 1;
TABLE[859][271] = 1;
TABLE[859][272] = 1;
TABLE[859][273] = 1;
TABLE[859][474] = 1;
TABLE[1325][400] = 1;
TABLE[1325][311] = 1;
TABLE[1325][377] = 1;
TABLE[1325][434] = 1;
TABLE[1325][398] = 1;
TABLE[1325][423] = 1;
TABLE[1325][455] = 1;
TABLE[1325][360] = 1;
TABLE[1325][350] = 1;
TABLE[1325][426] = 1;
TABLE[1325][431] = 1;
TABLE[1325][325] = 1;
TABLE[1325][327] = 1;
TABLE[1325][351] = 1;
TABLE[1325][464] = 1;
TABLE[1325][326] = 1;
TABLE[1325][474] = 1;
TABLE[1325][273] = 1;
TABLE[1325][272] = 1;
TABLE[1325][271] = 1;
TABLE[1325][263] = 1;
TABLE[1325][267] = 1;
TABLE[1325][266] = 1;
TABLE[1325][265] = 1;
TABLE[1325][269] = 1;
TABLE[1325][268] = 1;
TABLE[1325][274] = 1;
TABLE[1325][264] = 1;
TABLE[1325][125] = 1;
TABLE[860][261] = 1;
TABLE[1326][400] = 1;
TABLE[1326][311] = 1;
TABLE[1326][377] = 1;
TABLE[1326][434] = 1;
TABLE[1326][398] = 1;
TABLE[1326][423] = 1;
TABLE[1326][455] = 1;
TABLE[1326][360] = 1;
TABLE[1326][350] = 1;
TABLE[1326][426] = 1;
TABLE[1326][431] = 1;
TABLE[1326][325] = 1;
TABLE[1326][327] = 1;
TABLE[1326][351] = 1;
TABLE[1326][464] = 1;
TABLE[1326][326] = 1;
TABLE[1326][474] = 1;
TABLE[1326][273] = 1;
TABLE[1326][272] = 1;
TABLE[1326][271] = 1;
TABLE[1326][263] = 1;
TABLE[1326][267] = 1;
TABLE[1326][266] = 1;
TABLE[1326][265] = 1;
TABLE[1326][269] = 1;
TABLE[1326][268] = 1;
TABLE[1326][274] = 1;
TABLE[1326][264] = 1;
TABLE[1326][125] = 1;
TABLE[305][449] = 1;
TABLE[861][353] = 1;
TABLE[1327][123] = 1;
TABLE[862][326] = 1;
TABLE[862][353] = 1;
TABLE[862][390] = 1;
TABLE[862][364] = 1;
TABLE[862][366] = 1;
TABLE[862][323] = 1;
TABLE[862][292] = 1;
TABLE[862][319] = 1;
TABLE[862][410] = 1;
TABLE[862][306] = 1;
TABLE[862][431] = 1;
TABLE[862][435] = 1;
TABLE[862][389] = 1;
TABLE[862][277] = 1;
TABLE[862][296] = 1;
TABLE[862][440] = 1;
TABLE[862][424] = 1;
TABLE[862][369] = 1;
TABLE[862][346] = 1;
TABLE[862][437] = 1;
TABLE[862][330] = 1;
TABLE[862][409] = 1;
TABLE[862][350] = 1;
TABLE[862][356] = 1;
TABLE[862][400] = 1;
TABLE[862][434] = 1;
TABLE[862][377] = 1;
TABLE[862][311] = 1;
TABLE[862][368] = 1;
TABLE[862][357] = 1;
TABLE[862][307] = 1;
TABLE[862][374] = 1;
TABLE[862][264] = 1;
TABLE[862][274] = 1;
TABLE[862][268] = 1;
TABLE[862][269] = 1;
TABLE[862][265] = 1;
TABLE[862][266] = 1;
TABLE[862][267] = 1;
TABLE[862][263] = 1;
TABLE[862][271] = 1;
TABLE[862][272] = 1;
TABLE[862][273] = 1;
TABLE[862][474] = 1;
TABLE[1328][125] = 1;
TABLE[306][326] = 1;
TABLE[307][353] = 1;
TABLE[308][390] = 1;
TABLE[309][357] = 1;
TABLE[309][368] = 1;
TABLE[310][374] = 1;
TABLE[310][307] = 1;
TABLE[311][364] = 1;
TABLE[312][366] = 1;
TABLE[313][323] = 1;
TABLE[314][292] = 1;
TABLE[315][319] = 1;
TABLE[316][410] = 1;
TABLE[317][306] = 1;
TABLE[318][431] = 1;
TABLE[319][435] = 1;
TABLE[320][389] = 1;
TABLE[321][277] = 1;
TABLE[322][296] = 1;
TABLE[323][440] = 1;
TABLE[324][440] = 1;
TABLE[325][440] = 1;
TABLE[326][440] = 1;
TABLE[327][424] = 1;
TABLE[328][369] = 1;
TABLE[329][346] = 1;
TABLE[330][437] = 1;
TABLE[331][330] = 1;
TABLE[332][409] = 1;
TABLE[333][350] = 1;
TABLE[334][356] = 1;
TABLE[335][400] = 1;
TABLE[336][311] = 1;
TABLE[336][377] = 1;
TABLE[336][434] = 1;
TABLE[337][474] = 1;
TABLE[337][273] = 1;
TABLE[337][272] = 1;
TABLE[337][271] = 1;
TABLE[337][263] = 1;
TABLE[337][267] = 1;
TABLE[337][266] = 1;
TABLE[337][265] = 1;
TABLE[337][269] = 1;
TABLE[337][268] = 1;
TABLE[337][274] = 1;
TABLE[337][264] = 1;
TABLE[863][58] = 1;
TABLE[1329][257] = 1;
TABLE[864][58] = 1;
TABLE[1330][43] = 1;
TABLE[1330][45] = 1;
TABLE[1330][259] = 1;
TABLE[1330][345] = 1;
TABLE[1330][458] = 1;
TABLE[1330][256] = 1;
TABLE[1330][40] = 1;
TABLE[865][58] = 1;
TABLE[1331][43] = 1;
TABLE[1331][45] = 1;
TABLE[1331][259] = 1;
TABLE[1331][345] = 1;
TABLE[1331][458] = 1;
TABLE[1331][256] = 1;
TABLE[1331][40] = 1;
TABLE[866][374] = 1;
TABLE[867][307] = 1;
TABLE[868][58] = 1;
TABLE[1332][43] = 1;
TABLE[1332][61] = 1;
TABLE[1332][45] = 1;
TABLE[1332][40] = 1;
TABLE[1332][256] = 1;
TABLE[1332][458] = 1;
TABLE[1332][345] = 1;
TABLE[1332][259] = 1;
TABLE[1332][59] = 1;
TABLE[869][58] = 1;
TABLE[1333][43] = 1;
TABLE[1333][61] = 1;
TABLE[1333][45] = 1;
TABLE[1333][40] = 1;
TABLE[1333][256] = 1;
TABLE[1333][458] = 1;
TABLE[1333][345] = 1;
TABLE[1333][259] = 1;
TABLE[1333][59] = 1;
TABLE[870][58] = 1;
TABLE[1334][43] = 1;
TABLE[1334][61] = 1;
TABLE[1334][45] = 1;
TABLE[1334][40] = 1;
TABLE[1334][256] = 1;
TABLE[1334][458] = 1;
TABLE[1334][345] = 1;
TABLE[1334][259] = 1;
TABLE[1334][59] = 1;
TABLE[871][58] = 1;
TABLE[1335][43] = 1;
TABLE[1335][61] = 1;
TABLE[1335][45] = 1;
TABLE[1335][40] = 1;
TABLE[1335][256] = 1;
TABLE[1335][458] = 1;
TABLE[1335][345] = 1;
TABLE[1335][259] = 1;
TABLE[1335][59] = 1;
TABLE[872][58] = 1;
TABLE[1336][43] = 1;
TABLE[1336][61] = 1;
TABLE[1336][45] = 1;
TABLE[1336][40] = 1;
TABLE[1336][256] = 1;
TABLE[1336][458] = 1;
TABLE[1336][345] = 1;
TABLE[1336][259] = 1;
TABLE[1336][59] = 1;
TABLE[873][58] = 1;
TABLE[1337][43] = 1;
TABLE[1337][45] = 1;
TABLE[1337][259] = 1;
TABLE[1337][345] = 1;
TABLE[1337][458] = 1;
TABLE[1337][256] = 1;
TABLE[1337][40] = 1;
TABLE[874][58] = 1;
TABLE[1338][43] = 1;
TABLE[1338][61] = 1;
TABLE[1338][45] = 1;
TABLE[1338][40] = 1;
TABLE[1338][256] = 1;
TABLE[1338][458] = 1;
TABLE[1338][345] = 1;
TABLE[1338][259] = 1;
TABLE[1338][59] = 1;
TABLE[875][58] = 1;
TABLE[1339][43] = 1;
TABLE[1339][61] = 1;
TABLE[1339][45] = 1;
TABLE[1339][40] = 1;
TABLE[1339][256] = 1;
TABLE[1339][458] = 1;
TABLE[1339][345] = 1;
TABLE[1339][259] = 1;
TABLE[1339][59] = 1;
TABLE[876][58] = 1;
TABLE[1340][43] = 1;
TABLE[1340][61] = 1;
TABLE[1340][45] = 1;
TABLE[1340][40] = 1;
TABLE[1340][256] = 1;
TABLE[1340][458] = 1;
TABLE[1340][345] = 1;
TABLE[1340][259] = 1;
TABLE[1340][59] = 1;
TABLE[877][58] = 1;
TABLE[1341][43] = 1;
TABLE[1341][61] = 1;
TABLE[1341][45] = 1;
TABLE[1341][40] = 1;
TABLE[1341][256] = 1;
TABLE[1341][458] = 1;
TABLE[1341][345] = 1;
TABLE[1341][259] = 1;
TABLE[1341][59] = 1;
TABLE[878][58] = 1;
TABLE[1342][43] = 1;
TABLE[1342][61] = 1;
TABLE[1342][45] = 1;
TABLE[1342][40] = 1;
TABLE[1342][256] = 1;
TABLE[1342][458] = 1;
TABLE[1342][345] = 1;
TABLE[1342][259] = 1;
TABLE[1342][59] = 1;
TABLE[879][58] = 1;
TABLE[1343][43] = 1;
TABLE[1343][61] = 1;
TABLE[1343][45] = 1;
TABLE[1343][40] = 1;
TABLE[1343][256] = 1;
TABLE[1343][458] = 1;
TABLE[1343][345] = 1;
TABLE[1343][259] = 1;
TABLE[1343][44] = 1;
TABLE[1343][59] = 1;
TABLE[880][44] = 1;
TABLE[881][59] = 1;
TABLE[882][58] = 1;
TABLE[1344][45] = 1;
TABLE[1344][43] = 1;
TABLE[1344][40] = 1;
TABLE[1344][256] = 1;
TABLE[1344][458] = 1;
TABLE[1344][345] = 1;
TABLE[1344][259] = 1;
TABLE[883][44] = 1;
TABLE[1345][59] = 1;
TABLE[884][58] = 1;
TABLE[1346][43] = 1;
TABLE[1346][45] = 1;
TABLE[1346][126] = 1;
TABLE[1346][33] = 1;
TABLE[1346][259] = 1;
TABLE[1346][345] = 1;
TABLE[1346][458] = 1;
TABLE[1346][256] = 1;
TABLE[1346][40] = 1;
TABLE[1346][59] = 1;
TABLE[890][45] = 1;
TABLE[890][43] = 1;
TABLE[890][40] = 1;
TABLE[890][256] = 1;
TABLE[890][458] = 1;
TABLE[890][345] = 1;
TABLE[890][259] = 1;
TABLE[890][33] = 1;
TABLE[890][126] = 1;
TABLE[1348][59] = 1;
TABLE[885][43] = 1;
TABLE[885][45] = 1;
TABLE[885][259] = 1;
TABLE[885][345] = 1;
TABLE[885][458] = 1;
TABLE[885][256] = 1;
TABLE[885][40] = 1;
TABLE[888][126] = 1;
TABLE[888][33] = 1;
TABLE[886][126] = 1;
TABLE[887][33] = 1;
TABLE[889][44] = 1;
TABLE[1347][43] = 1;
TABLE[1347][45] = 1;
TABLE[1347][126] = 1;
TABLE[1347][33] = 1;
TABLE[1347][259] = 1;
TABLE[1347][345] = 1;
TABLE[1347][458] = 1;
TABLE[1347][256] = 1;
TABLE[1347][40] = 1;
TABLE[1347][59] = 1;
TABLE[891][58] = 1;
TABLE[1349][43] = 1;
TABLE[1349][45] = 1;
TABLE[1349][126] = 1;
TABLE[1349][33] = 1;
TABLE[1349][259] = 1;
TABLE[1349][345] = 1;
TABLE[1349][458] = 1;
TABLE[1349][256] = 1;
TABLE[1349][40] = 1;
TABLE[1349][59] = 1;
TABLE[897][45] = 1;
TABLE[897][43] = 1;
TABLE[897][40] = 1;
TABLE[897][256] = 1;
TABLE[897][458] = 1;
TABLE[897][345] = 1;
TABLE[897][259] = 1;
TABLE[897][33] = 1;
TABLE[897][126] = 1;
TABLE[1351][59] = 1;
TABLE[892][43] = 1;
TABLE[892][45] = 1;
TABLE[892][259] = 1;
TABLE[892][345] = 1;
TABLE[892][458] = 1;
TABLE[892][256] = 1;
TABLE[892][40] = 1;
TABLE[895][126] = 1;
TABLE[895][33] = 1;
TABLE[893][126] = 1;
TABLE[894][33] = 1;
TABLE[896][44] = 1;
TABLE[1350][43] = 1;
TABLE[1350][45] = 1;
TABLE[1350][126] = 1;
TABLE[1350][33] = 1;
TABLE[1350][259] = 1;
TABLE[1350][345] = 1;
TABLE[1350][458] = 1;
TABLE[1350][256] = 1;
TABLE[1350][40] = 1;
TABLE[1350][59] = 1;
TABLE[898][58] = 1;
TABLE[1352][43] = 1;
TABLE[1352][45] = 1;
TABLE[1352][259] = 1;
TABLE[1352][345] = 1;
TABLE[1352][458] = 1;
TABLE[1352][256] = 1;
TABLE[1352][40] = 1;
TABLE[899][43] = 1;
TABLE[899][45] = 1;
TABLE[899][259] = 1;
TABLE[899][345] = 1;
TABLE[899][458] = 1;
TABLE[899][256] = 1;
TABLE[899][40] = 1;
TABLE[1353][43] = 1;
TABLE[1353][45] = 1;
TABLE[1353][259] = 1;
TABLE[1353][345] = 1;
TABLE[1353][458] = 1;
TABLE[1353][256] = 1;
TABLE[1353][40] = 1;
TABLE[900][58] = 1;
TABLE[1354][36] = 1;
TABLE[1354][59] = 1;
TABLE[902][36] = 1;
TABLE[1356][59] = 1;
TABLE[901][44] = 1;
TABLE[1355][36] = 1;
TABLE[1355][59] = 1;
TABLE[903][58] = 1;
TABLE[1357][36] = 1;
TABLE[1357][59] = 1;
TABLE[905][36] = 1;
TABLE[1359][59] = 1;
TABLE[904][44] = 1;
TABLE[1358][36] = 1;
TABLE[1358][59] = 1;
TABLE[906][58] = 1;
TABLE[1360][36] = 1;
TABLE[1360][59] = 1;
TABLE[908][36] = 1;
TABLE[1362][59] = 1;
TABLE[907][44] = 1;
TABLE[1361][36] = 1;
TABLE[1361][59] = 1;
TABLE[909][58] = 1;
TABLE[1363][43] = 1;
TABLE[1363][45] = 1;
TABLE[1363][126] = 1;
TABLE[1363][33] = 1;
TABLE[1363][259] = 1;
TABLE[1363][345] = 1;
TABLE[1363][458] = 1;
TABLE[1363][256] = 1;
TABLE[1363][40] = 1;
TABLE[1363][311] = 1;
TABLE[1363][377] = 1;
TABLE[1363][434] = 1;
TABLE[1363][400] = 1;
TABLE[1363][356] = 1;
TABLE[1363][350] = 1;
TABLE[1363][409] = 1;
TABLE[1363][330] = 1;
TABLE[1363][437] = 1;
TABLE[1363][346] = 1;
TABLE[1363][369] = 1;
TABLE[1363][424] = 1;
TABLE[1363][440] = 1;
TABLE[1363][296] = 1;
TABLE[1363][277] = 1;
TABLE[1363][389] = 1;
TABLE[1363][435] = 1;
TABLE[1363][431] = 1;
TABLE[1363][306] = 1;
TABLE[1363][410] = 1;
TABLE[1363][319] = 1;
TABLE[1363][292] = 1;
TABLE[1363][323] = 1;
TABLE[1363][366] = 1;
TABLE[1363][364] = 1;
TABLE[1363][390] = 1;
TABLE[1363][353] = 1;
TABLE[1363][326] = 1;
TABLE[1363][474] = 1;
TABLE[1363][273] = 1;
TABLE[1363][272] = 1;
TABLE[1363][271] = 1;
TABLE[1363][263] = 1;
TABLE[1363][267] = 1;
TABLE[1363][266] = 1;
TABLE[1363][265] = 1;
TABLE[1363][269] = 1;
TABLE[1363][268] = 1;
TABLE[1363][274] = 1;
TABLE[1363][264] = 1;
TABLE[1363][374] = 1;
TABLE[1363][307] = 1;
TABLE[1363][357] = 1;
TABLE[1363][368] = 1;
TABLE[1363][125] = 1;
TABLE[916][45] = 1;
TABLE[916][43] = 1;
TABLE[916][40] = 1;
TABLE[916][256] = 1;
TABLE[916][458] = 1;
TABLE[916][345] = 1;
TABLE[916][259] = 1;
TABLE[916][33] = 1;
TABLE[916][126] = 1;
TABLE[1364][311] = 1;
TABLE[1364][377] = 1;
TABLE[1364][434] = 1;
TABLE[1364][400] = 1;
TABLE[1364][356] = 1;
TABLE[1364][350] = 1;
TABLE[1364][409] = 1;
TABLE[1364][330] = 1;
TABLE[1364][437] = 1;
TABLE[1364][346] = 1;
TABLE[1364][369] = 1;
TABLE[1364][424] = 1;
TABLE[1364][440] = 1;
TABLE[1364][296] = 1;
TABLE[1364][277] = 1;
TABLE[1364][389] = 1;
TABLE[1364][435] = 1;
TABLE[1364][431] = 1;
TABLE[1364][306] = 1;
TABLE[1364][410] = 1;
TABLE[1364][319] = 1;
TABLE[1364][292] = 1;
TABLE[1364][323] = 1;
TABLE[1364][366] = 1;
TABLE[1364][364] = 1;
TABLE[1364][390] = 1;
TABLE[1364][353] = 1;
TABLE[1364][326] = 1;
TABLE[1364][474] = 1;
TABLE[1364][273] = 1;
TABLE[1364][272] = 1;
TABLE[1364][271] = 1;
TABLE[1364][263] = 1;
TABLE[1364][267] = 1;
TABLE[1364][266] = 1;
TABLE[1364][265] = 1;
TABLE[1364][269] = 1;
TABLE[1364][268] = 1;
TABLE[1364][274] = 1;
TABLE[1364][264] = 1;
TABLE[1364][374] = 1;
TABLE[1364][307] = 1;
TABLE[1364][357] = 1;
TABLE[1364][368] = 1;
TABLE[1364][125] = 1;
TABLE[910][43] = 1;
TABLE[910][45] = 1;
TABLE[910][259] = 1;
TABLE[910][345] = 1;
TABLE[910][458] = 1;
TABLE[910][256] = 1;
TABLE[910][40] = 1;
TABLE[913][126] = 1;
TABLE[913][33] = 1;
TABLE[911][126] = 1;
TABLE[912][33] = 1;
TABLE[914][44] = 1;
TABLE[915][59] = 1;
TABLE[338][43] = 1;
TABLE[338][45] = 1;
TABLE[338][259] = 1;
TABLE[338][345] = 1;
TABLE[338][458] = 1;
TABLE[338][256] = 1;
TABLE[338][40] = 1;
TABLE[339][43] = 1;
TABLE[339][45] = 1;
TABLE[339][259] = 1;
TABLE[339][345] = 1;
TABLE[339][458] = 1;
TABLE[339][256] = 1;
TABLE[339][40] = 1;
TABLE[917][259] = 1;
TABLE[917][345] = 1;
TABLE[917][458] = 1;
TABLE[917][256] = 1;
TABLE[917][40] = 1;
TABLE[918][43] = 1;
TABLE[918][45] = 1;
TABLE[918][259] = 1;
TABLE[918][345] = 1;
TABLE[918][458] = 1;
TABLE[918][256] = 1;
TABLE[918][40] = 1;
TABLE[1365][45] = 1;
TABLE[1365][43] = 1;
TABLE[1365][256] = 1;
TABLE[1365][458] = 1;
TABLE[1365][345] = 1;
TABLE[1365][259] = 1;
TABLE[1365][319] = 1;
TABLE[1365][40] = 1;
TABLE[1365][288] = 1;
TABLE[1365][44] = 1;
TABLE[1365][59] = 1;
TABLE[919][43] = 1;
TABLE[919][45] = 1;
TABLE[919][259] = 1;
TABLE[919][345] = 1;
TABLE[919][458] = 1;
TABLE[919][256] = 1;
TABLE[919][40] = 1;
TABLE[1366][319] = 1;
TABLE[1366][40] = 1;
TABLE[1366][288] = 1;
TABLE[1366][44] = 1;
TABLE[1366][59] = 1;
TABLE[920][319] = 1;
TABLE[1367][40] = 1;
TABLE[1367][288] = 1;
TABLE[1367][44] = 1;
TABLE[1367][59] = 1;
TABLE[924][40] = 1;
TABLE[1368][288] = 1;
TABLE[1368][44] = 1;
TABLE[1368][59] = 1;
TABLE[921][322] = 1;
TABLE[922][307] = 1;
TABLE[923][374] = 1;
TABLE[932][288] = 1;
TABLE[1373][44] = 1;
TABLE[1373][59] = 1;
TABLE[925][259] = 1;
TABLE[925][345] = 1;
TABLE[925][458] = 1;
TABLE[925][256] = 1;
TABLE[925][40] = 1;
TABLE[1369][45] = 1;
TABLE[1369][43] = 1;
TABLE[1369][256] = 1;
TABLE[1369][458] = 1;
TABLE[1369][345] = 1;
TABLE[1369][259] = 1;
TABLE[1369][319] = 1;
TABLE[1369][40] = 1;
TABLE[1369][288] = 1;
TABLE[1369][44] = 1;
TABLE[1369][59] = 1;
TABLE[926][43] = 1;
TABLE[926][45] = 1;
TABLE[926][259] = 1;
TABLE[926][345] = 1;
TABLE[926][458] = 1;
TABLE[926][256] = 1;
TABLE[926][40] = 1;
TABLE[1370][319] = 1;
TABLE[1370][40] = 1;
TABLE[1370][288] = 1;
TABLE[1370][44] = 1;
TABLE[1370][59] = 1;
TABLE[927][319] = 1;
TABLE[1371][40] = 1;
TABLE[1371][288] = 1;
TABLE[1371][44] = 1;
TABLE[1371][59] = 1;
TABLE[931][40] = 1;
TABLE[1372][288] = 1;
TABLE[1372][44] = 1;
TABLE[1372][59] = 1;
TABLE[928][322] = 1;
TABLE[929][307] = 1;
TABLE[930][374] = 1;
TABLE[933][43] = 1;
TABLE[933][45] = 1;
TABLE[933][259] = 1;
TABLE[933][345] = 1;
TABLE[933][458] = 1;
TABLE[933][256] = 1;
TABLE[933][40] = 1;
TABLE[1374][44] = 1;
TABLE[1374][59] = 1;
TABLE[340][261] = 1;
TABLE[341][341] = 1;
TABLE[934][326] = 1;
TABLE[934][383] = 1;
TABLE[934][310] = 1;
TABLE[934][466] = 1;
TABLE[1375][125] = 1;
TABLE[342][326] = 1;
TABLE[343][383] = 1;
TABLE[344][310] = 1;
TABLE[345][466] = 1;
TABLE[935][58] = 1;
TABLE[1376][257] = 1;
TABLE[936][58] = 1;
TABLE[1377][43] = 1;
TABLE[1377][45] = 1;
TABLE[1377][259] = 1;
TABLE[1377][345] = 1;
TABLE[1377][458] = 1;
TABLE[1377][256] = 1;
TABLE[1377][40] = 1;
TABLE[937][58] = 1;
TABLE[1378][275] = 1;
TABLE[1378][303] = 1;
TABLE[1378][376] = 1;
TABLE[1378][320] = 1;
TABLE[938][58] = 1;
TABLE[1379][43] = 1;
TABLE[1379][45] = 1;
TABLE[1379][259] = 1;
TABLE[1379][345] = 1;
TABLE[1379][458] = 1;
TABLE[1379][256] = 1;
TABLE[1379][40] = 1;
TABLE[346][336] = 1;
TABLE[939][58] = 1;
TABLE[1380][123] = 1;
TABLE[940][450] = 1;
TABLE[940][325] = 1;
TABLE[940][466] = 1;
TABLE[940][307] = 1;
TABLE[940][285] = 1;
TABLE[940][406] = 1;
TABLE[940][350] = 1;
TABLE[940][356] = 1;
TABLE[940][400] = 1;
TABLE[940][434] = 1;
TABLE[940][377] = 1;
TABLE[940][311] = 1;
TABLE[940][264] = 1;
TABLE[940][274] = 1;
TABLE[940][268] = 1;
TABLE[940][269] = 1;
TABLE[940][265] = 1;
TABLE[940][266] = 1;
TABLE[940][267] = 1;
TABLE[940][263] = 1;
TABLE[940][271] = 1;
TABLE[940][272] = 1;
TABLE[940][273] = 1;
TABLE[940][474] = 1;
TABLE[1381][125] = 1;
TABLE[347][450] = 1;
TABLE[348][325] = 1;
TABLE[349][466] = 1;
TABLE[350][307] = 1;
TABLE[351][307] = 1;
TABLE[352][307] = 1;
TABLE[353][285] = 1;
TABLE[354][406] = 1;
TABLE[355][350] = 1;
TABLE[356][356] = 1;
TABLE[357][400] = 1;
TABLE[358][311] = 1;
TABLE[358][377] = 1;
TABLE[358][434] = 1;
TABLE[359][474] = 1;
TABLE[359][273] = 1;
TABLE[359][272] = 1;
TABLE[359][271] = 1;
TABLE[359][263] = 1;
TABLE[359][267] = 1;
TABLE[359][266] = 1;
TABLE[359][265] = 1;
TABLE[359][269] = 1;
TABLE[359][268] = 1;
TABLE[359][274] = 1;
TABLE[359][264] = 1;
TABLE[941][457] = 1;
TABLE[942][45] = 1;
TABLE[943][124] = 1;
TABLE[943][335] = 1;
TABLE[943][42] = 1;
TABLE[1382][59] = 1;
TABLE[944][58] = 1;
TABLE[1383][45] = 1;
TABLE[1383][43] = 1;
TABLE[1383][40] = 1;
TABLE[1383][256] = 1;
TABLE[1383][458] = 1;
TABLE[1383][345] = 1;
TABLE[1383][259] = 1;
TABLE[1383][59] = 1;
TABLE[947][43] = 1;
TABLE[947][45] = 1;
TABLE[947][259] = 1;
TABLE[947][345] = 1;
TABLE[947][458] = 1;
TABLE[947][256] = 1;
TABLE[947][40] = 1;
TABLE[1385][59] = 1;
TABLE[945][44] = 1;
TABLE[946][124] = 1;
TABLE[1384][45] = 1;
TABLE[1384][43] = 1;
TABLE[1384][40] = 1;
TABLE[1384][256] = 1;
TABLE[1384][458] = 1;
TABLE[1384][345] = 1;
TABLE[1384][259] = 1;
TABLE[1384][59] = 1;
TABLE[360][42] = 1;
TABLE[360][335] = 1;
TABLE[360][124] = 1;
TABLE[948][42] = 1;
TABLE[949][335] = 1;
TABLE[950][124] = 1;
TABLE[951][40] = 1;
TABLE[951][256] = 1;
TABLE[951][458] = 1;
TABLE[951][345] = 1;
TABLE[951][259] = 1;
TABLE[952][60] = 1;
TABLE[953][91] = 1;
TABLE[954][123] = 1;
TABLE[957][40] = 1;
TABLE[957][256] = 1;
TABLE[957][458] = 1;
TABLE[957][345] = 1;
TABLE[957][259] = 1;
TABLE[958][40] = 1;
TABLE[958][256] = 1;
TABLE[958][458] = 1;
TABLE[958][345] = 1;
TABLE[958][259] = 1;
TABLE[959][40] = 1;
TABLE[959][256] = 1;
TABLE[959][458] = 1;
TABLE[959][345] = 1;
TABLE[959][259] = 1;
TABLE[960][259] = 1;
TABLE[960][345] = 1;
TABLE[960][458] = 1;
TABLE[960][256] = 1;
TABLE[960][40] = 1;
TABLE[1386][399] = 1;
TABLE[955][45] = 1;
TABLE[956][457] = 1;
TABLE[961][36] = 1;
TABLE[962][43] = 1;
TABLE[962][45] = 1;
TABLE[962][259] = 1;
TABLE[962][345] = 1;
TABLE[962][458] = 1;
TABLE[962][256] = 1;
TABLE[962][40] = 1;
TABLE[963][289] = 1;
TABLE[964][36] = 1;
TABLE[965][43] = 1;
TABLE[965][45] = 1;
TABLE[965][259] = 1;
TABLE[965][345] = 1;
TABLE[965][458] = 1;
TABLE[965][256] = 1;
TABLE[965][40] = 1;
TABLE[966][289] = 1;
TABLE[1387][91] = 1;
TABLE[1387][42] = 1;
TABLE[1387][335] = 1;
TABLE[1387][124] = 1;
TABLE[1387][59] = 1;
TABLE[970][91] = 1;
TABLE[1389][42] = 1;
TABLE[1389][335] = 1;
TABLE[1389][124] = 1;
TABLE[1389][59] = 1;
TABLE[969][124] = 1;
TABLE[969][44] = 1;
TABLE[1388][93] = 1;
TABLE[967][124] = 1;
TABLE[968][44] = 1;
TABLE[361][299] = 1;
TABLE[971][436] = 1;
TABLE[971][350] = 1;
TABLE[971][400] = 1;
TABLE[971][434] = 1;
TABLE[971][377] = 1;
TABLE[971][311] = 1;
TABLE[971][264] = 1;
TABLE[971][274] = 1;
TABLE[971][268] = 1;
TABLE[971][269] = 1;
TABLE[971][265] = 1;
TABLE[971][266] = 1;
TABLE[971][267] = 1;
TABLE[971][263] = 1;
TABLE[971][271] = 1;
TABLE[971][272] = 1;
TABLE[971][273] = 1;
TABLE[971][474] = 1;
TABLE[1390][125] = 1;
TABLE[362][436] = 1;
TABLE[363][350] = 1;
TABLE[364][400] = 1;
TABLE[365][311] = 1;
TABLE[365][377] = 1;
TABLE[365][434] = 1;
TABLE[366][474] = 1;
TABLE[366][273] = 1;
TABLE[366][272] = 1;
TABLE[366][271] = 1;
TABLE[366][263] = 1;
TABLE[366][267] = 1;
TABLE[366][266] = 1;
TABLE[366][265] = 1;
TABLE[366][269] = 1;
TABLE[366][268] = 1;
TABLE[366][274] = 1;
TABLE[366][264] = 1;
TABLE[972][58] = 1;
TABLE[1391][36] = 1;
TABLE[973][58] = 1;
TABLE[1392][45] = 1;
TABLE[1392][43] = 1;
TABLE[1392][40] = 1;
TABLE[1392][256] = 1;
TABLE[1392][458] = 1;
TABLE[1392][345] = 1;
TABLE[1392][259] = 1;
TABLE[1392][59] = 1;
TABLE[976][43] = 1;
TABLE[976][45] = 1;
TABLE[976][259] = 1;
TABLE[976][345] = 1;
TABLE[976][458] = 1;
TABLE[976][256] = 1;
TABLE[976][40] = 1;
TABLE[1394][59] = 1;
TABLE[974][124] = 1;
TABLE[975][44] = 1;
TABLE[1393][45] = 1;
TABLE[1393][43] = 1;
TABLE[1393][40] = 1;
TABLE[1393][256] = 1;
TABLE[1393][458] = 1;
TABLE[1393][345] = 1;
TABLE[1393][259] = 1;
TABLE[1393][59] = 1;
TABLE[367][451] = 1;
TABLE[977][257] = 1;
TABLE[978][36] = 1;
TABLE[979][40] = 1;
TABLE[980][41] = 1;
TABLE[981][124] = 1;
TABLE[982][59] = 1;
TABLE[983][91] = 1;
TABLE[984][91] = 1;
TABLE[1395][59] = 1;
TABLE[368][261] = 1;
TABLE[369][264] = 1;
TABLE[370][274] = 1;
TABLE[371][268] = 1;
TABLE[372][269] = 1;
TABLE[373][265] = 1;
TABLE[374][266] = 1;
TABLE[375][267] = 1;
TABLE[376][263] = 1;
TABLE[377][271] = 1;
TABLE[378][272] = 1;
TABLE[379][273] = 1;
TABLE[380][474] = 1;
TABLE[381][447] = 1;
TABLE[992][264] = 1;
TABLE[992][274] = 1;
TABLE[992][268] = 1;
TABLE[992][269] = 1;
TABLE[992][265] = 1;
TABLE[992][266] = 1;
TABLE[992][267] = 1;
TABLE[992][263] = 1;
TABLE[992][271] = 1;
TABLE[992][272] = 1;
TABLE[992][273] = 1;
TABLE[992][474] = 1;
TABLE[1403][44] = 1;
TABLE[1403][41] = 1;
TABLE[985][58] = 1;
TABLE[1396][258] = 1;
TABLE[1396][61] = 1;
TABLE[1396][44] = 1;
TABLE[1396][41] = 1;
TABLE[986][258] = 1;
TABLE[1397][61] = 1;
TABLE[1397][44] = 1;
TABLE[1397][41] = 1;
TABLE[987][61] = 1;
TABLE[1398][44] = 1;
TABLE[1398][41] = 1;
TABLE[991][44] = 1;
TABLE[1402][44] = 1;
TABLE[1402][41] = 1;
TABLE[988][58] = 1;
TABLE[1399][258] = 1;
TABLE[1399][61] = 1;
TABLE[1399][44] = 1;
TABLE[1399][41] = 1;
TABLE[989][258] = 1;
TABLE[1400][61] = 1;
TABLE[1400][44] = 1;
TABLE[1400][41] = 1;
TABLE[990][61] = 1;
TABLE[1401][44] = 1;
TABLE[1401][41] = 1;
TABLE[993][44] = 1;
TABLE[1404][41] = 1;
TABLE[382][447] = 1;
TABLE[1001][264] = 1;
TABLE[1001][274] = 1;
TABLE[1001][268] = 1;
TABLE[1001][269] = 1;
TABLE[1001][265] = 1;
TABLE[1001][266] = 1;
TABLE[1001][267] = 1;
TABLE[1001][263] = 1;
TABLE[1001][271] = 1;
TABLE[1001][272] = 1;
TABLE[1001][273] = 1;
TABLE[1001][474] = 1;
TABLE[1412][44] = 1;
TABLE[1412][41] = 1;
TABLE[994][58] = 1;
TABLE[1405][258] = 1;
TABLE[1405][61] = 1;
TABLE[1405][44] = 1;
TABLE[1405][41] = 1;
TABLE[995][258] = 1;
TABLE[1406][61] = 1;
TABLE[1406][44] = 1;
TABLE[1406][41] = 1;
TABLE[996][61] = 1;
TABLE[1407][44] = 1;
TABLE[1407][41] = 1;
TABLE[1000][44] = 1;
TABLE[1411][44] = 1;
TABLE[1411][41] = 1;
TABLE[997][58] = 1;
TABLE[1408][258] = 1;
TABLE[1408][61] = 1;
TABLE[1408][44] = 1;
TABLE[1408][41] = 1;
TABLE[998][258] = 1;
TABLE[1409][61] = 1;
TABLE[1409][44] = 1;
TABLE[1409][41] = 1;
TABLE[999][61] = 1;
TABLE[1410][44] = 1;
TABLE[1410][41] = 1;
TABLE[1002][44] = 1;
TABLE[1413][41] = 1;
TABLE[383][447] = 1;
TABLE[384][447] = 1;
TABLE[385][264] = 1;
TABLE[385][274] = 1;
TABLE[385][268] = 1;
TABLE[385][269] = 1;
TABLE[385][265] = 1;
TABLE[385][266] = 1;
TABLE[385][267] = 1;
TABLE[385][263] = 1;
TABLE[385][271] = 1;
TABLE[385][272] = 1;
TABLE[385][273] = 1;
TABLE[385][474] = 1;
TABLE[1003][44] = 1;
TABLE[1414][59] = 1;
TABLE[386][264] = 1;
TABLE[386][274] = 1;
TABLE[386][268] = 1;
TABLE[386][269] = 1;
TABLE[386][265] = 1;
TABLE[386][266] = 1;
TABLE[386][267] = 1;
TABLE[386][263] = 1;
TABLE[386][271] = 1;
TABLE[386][272] = 1;
TABLE[386][273] = 1;
TABLE[386][474] = 1;
TABLE[1004][44] = 1;
TABLE[1415][59] = 1;
TABLE[387][264] = 1;
TABLE[387][274] = 1;
TABLE[387][268] = 1;
TABLE[387][269] = 1;
TABLE[387][265] = 1;
TABLE[387][266] = 1;
TABLE[387][267] = 1;
TABLE[387][263] = 1;
TABLE[387][271] = 1;
TABLE[387][272] = 1;
TABLE[387][273] = 1;
TABLE[387][474] = 1;
TABLE[1005][44] = 1;
TABLE[1416][59] = 1;
TABLE[388][475] = 1;
TABLE[388][476] = 1;
TABLE[388][43] = 1;
TABLE[388][45] = 1;
TABLE[388][33] = 1;
TABLE[388][126] = 1;
TABLE[388][58] = 1;
TABLE[388][384] = 1;
TABLE[388][387] = 1;
TABLE[388][258] = 1;
TABLE[388][40] = 1;
TABLE[388][36] = 1;
TABLE[388][395] = 1;
TABLE[388][345] = 1;
TABLE[388][458] = 1;
TABLE[388][257] = 1;
TABLE[388][259] = 1;
TABLE[388][256] = 1;
TABLE[389][367] = 1;
TABLE[390][367] = 1;
TABLE[391][353] = 1;
TABLE[392][446] = 1;
TABLE[393][302] = 1;
TABLE[394][314] = 1;
TABLE[395][331] = 1;
TABLE[396][469] = 1;
TABLE[397][123] = 1;
TABLE[398][425] = 1;
TABLE[399][425] = 1;
TABLE[1009][305] = 1;
TABLE[1418][324] = 1;
TABLE[1418][125] = 1;
TABLE[1006][43] = 1;
TABLE[1006][45] = 1;
TABLE[1006][259] = 1;
TABLE[1006][345] = 1;
TABLE[1006][458] = 1;
TABLE[1006][256] = 1;
TABLE[1006][40] = 1;
TABLE[1007][36] = 1;
TABLE[1008][367] = 1;
TABLE[1008][353] = 1;
TABLE[1008][446] = 1;
TABLE[1008][302] = 1;
TABLE[1008][314] = 1;
TABLE[1008][331] = 1;
TABLE[1008][469] = 1;
TABLE[1008][425] = 1;
TABLE[1008][256] = 1;
TABLE[1008][259] = 1;
TABLE[1008][257] = 1;
TABLE[1008][458] = 1;
TABLE[1008][345] = 1;
TABLE[1008][395] = 1;
TABLE[1008][36] = 1;
TABLE[1008][40] = 1;
TABLE[1008][258] = 1;
TABLE[1008][387] = 1;
TABLE[1008][384] = 1;
TABLE[1008][58] = 1;
TABLE[1008][126] = 1;
TABLE[1008][33] = 1;
TABLE[1008][45] = 1;
TABLE[1008][43] = 1;
TABLE[1008][476] = 1;
TABLE[1008][475] = 1;
TABLE[1008][123] = 1;
TABLE[1417][305] = 1;
TABLE[1417][324] = 1;
TABLE[1417][125] = 1;
TABLE[1011][324] = 1;
TABLE[1420][125] = 1;
TABLE[1010][367] = 1;
TABLE[1010][353] = 1;
TABLE[1010][446] = 1;
TABLE[1010][302] = 1;
TABLE[1010][314] = 1;
TABLE[1010][331] = 1;
TABLE[1010][469] = 1;
TABLE[1010][425] = 1;
TABLE[1010][256] = 1;
TABLE[1010][259] = 1;
TABLE[1010][257] = 1;
TABLE[1010][458] = 1;
TABLE[1010][345] = 1;
TABLE[1010][395] = 1;
TABLE[1010][36] = 1;
TABLE[1010][40] = 1;
TABLE[1010][258] = 1;
TABLE[1010][387] = 1;
TABLE[1010][384] = 1;
TABLE[1010][58] = 1;
TABLE[1010][126] = 1;
TABLE[1010][33] = 1;
TABLE[1010][45] = 1;
TABLE[1010][43] = 1;
TABLE[1010][476] = 1;
TABLE[1010][475] = 1;
TABLE[1010][123] = 1;
TABLE[1419][125] = 1;
TABLE[400][123] = 1;
TABLE[1012][367] = 1;
TABLE[1012][353] = 1;
TABLE[1012][446] = 1;
TABLE[1012][302] = 1;
TABLE[1012][314] = 1;
TABLE[1012][331] = 1;
TABLE[1012][469] = 1;
TABLE[1012][425] = 1;
TABLE[1012][256] = 1;
TABLE[1012][259] = 1;
TABLE[1012][257] = 1;
TABLE[1012][458] = 1;
TABLE[1012][345] = 1;
TABLE[1012][395] = 1;
TABLE[1012][36] = 1;
TABLE[1012][40] = 1;
TABLE[1012][258] = 1;
TABLE[1012][387] = 1;
TABLE[1012][384] = 1;
TABLE[1012][58] = 1;
TABLE[1012][126] = 1;
TABLE[1012][33] = 1;
TABLE[1012][45] = 1;
TABLE[1012][43] = 1;
TABLE[1012][476] = 1;
TABLE[1012][475] = 1;
TABLE[1012][123] = 1;
TABLE[1421][125] = 1;
TABLE[401][123] = 1;
TABLE[1013][474] = 1;
TABLE[1013][273] = 1;
TABLE[1013][272] = 1;
TABLE[1013][271] = 1;
TABLE[1013][263] = 1;
TABLE[1013][267] = 1;
TABLE[1013][266] = 1;
TABLE[1013][265] = 1;
TABLE[1013][269] = 1;
TABLE[1013][268] = 1;
TABLE[1013][274] = 1;
TABLE[1013][264] = 1;
TABLE[1014][441] = 1;
TABLE[1422][425] = 1;
TABLE[1422][469] = 1;
TABLE[1422][331] = 1;
TABLE[1422][314] = 1;
TABLE[1422][302] = 1;
TABLE[1422][446] = 1;
TABLE[1422][353] = 1;
TABLE[1422][367] = 1;
TABLE[1422][123] = 1;
TABLE[1422][475] = 1;
TABLE[1422][476] = 1;
TABLE[1422][43] = 1;
TABLE[1422][45] = 1;
TABLE[1422][33] = 1;
TABLE[1422][126] = 1;
TABLE[1422][58] = 1;
TABLE[1422][384] = 1;
TABLE[1422][387] = 1;
TABLE[1422][258] = 1;
TABLE[1422][40] = 1;
TABLE[1422][36] = 1;
TABLE[1422][395] = 1;
TABLE[1422][345] = 1;
TABLE[1422][458] = 1;
TABLE[1422][257] = 1;
TABLE[1422][259] = 1;
TABLE[1422][256] = 1;
TABLE[1422][125] = 1;
TABLE[1015][367] = 1;
TABLE[1015][353] = 1;
TABLE[1015][446] = 1;
TABLE[1015][302] = 1;
TABLE[1015][314] = 1;
TABLE[1015][331] = 1;
TABLE[1015][469] = 1;
TABLE[1015][425] = 1;
TABLE[1015][256] = 1;
TABLE[1015][259] = 1;
TABLE[1015][257] = 1;
TABLE[1015][458] = 1;
TABLE[1015][345] = 1;
TABLE[1015][395] = 1;
TABLE[1015][36] = 1;
TABLE[1015][40] = 1;
TABLE[1015][258] = 1;
TABLE[1015][387] = 1;
TABLE[1015][384] = 1;
TABLE[1015][58] = 1;
TABLE[1015][126] = 1;
TABLE[1015][33] = 1;
TABLE[1015][45] = 1;
TABLE[1015][43] = 1;
TABLE[1015][476] = 1;
TABLE[1015][475] = 1;
TABLE[1015][123] = 1;
TABLE[1423][125] = 1;
TABLE[402][256] = 1;
TABLE[403][259] = 1;
TABLE[404][257] = 1;
TABLE[405][458] = 1;
TABLE[406][345] = 1;
TABLE[407][395] = 1;
TABLE[408][36] = 1;
TABLE[409][40] = 1;
TABLE[410][258] = 1;
TABLE[411][256] = 1;
TABLE[411][259] = 1;
TABLE[411][257] = 1;
TABLE[411][458] = 1;
TABLE[411][345] = 1;
TABLE[411][395] = 1;
TABLE[411][36] = 1;
TABLE[411][40] = 1;
TABLE[411][258] = 1;
TABLE[412][256] = 1;
TABLE[412][259] = 1;
TABLE[412][257] = 1;
TABLE[412][458] = 1;
TABLE[412][345] = 1;
TABLE[412][395] = 1;
TABLE[412][36] = 1;
TABLE[412][40] = 1;
TABLE[412][258] = 1;
TABLE[413][258] = 1;
TABLE[414][256] = 1;
TABLE[414][259] = 1;
TABLE[414][257] = 1;
TABLE[414][458] = 1;
TABLE[414][345] = 1;
TABLE[414][395] = 1;
TABLE[414][36] = 1;
TABLE[414][40] = 1;
TABLE[414][258] = 1;
TABLE[1016][46] = 1;
TABLE[1017][496] = 1;
TABLE[1018][46] = 1;
TABLE[1019][496] = 1;
TABLE[415][258] = 1;
TABLE[416][258] = 1;
TABLE[416][40] = 1;
TABLE[416][36] = 1;
TABLE[416][395] = 1;
TABLE[416][345] = 1;
TABLE[416][458] = 1;
TABLE[416][257] = 1;
TABLE[416][259] = 1;
TABLE[416][256] = 1;
TABLE[417][256] = 1;
TABLE[417][259] = 1;
TABLE[417][257] = 1;
TABLE[417][458] = 1;
TABLE[417][345] = 1;
TABLE[417][395] = 1;
TABLE[417][36] = 1;
TABLE[417][40] = 1;
TABLE[417][258] = 1;
TABLE[1020][46] = 1;
TABLE[1021][496] = 1;
TABLE[418][258] = 1;
TABLE[418][40] = 1;
TABLE[418][36] = 1;
TABLE[418][395] = 1;
TABLE[418][345] = 1;
TABLE[418][458] = 1;
TABLE[418][257] = 1;
TABLE[418][259] = 1;
TABLE[418][256] = 1;
TABLE[419][258] = 1;
TABLE[419][40] = 1;
TABLE[419][36] = 1;
TABLE[419][395] = 1;
TABLE[419][345] = 1;
TABLE[419][458] = 1;
TABLE[419][257] = 1;
TABLE[419][259] = 1;
TABLE[419][256] = 1;
TABLE[420][387] = 1;
TABLE[421][384] = 1;
TABLE[422][256] = 1;
TABLE[422][259] = 1;
TABLE[422][257] = 1;
TABLE[422][458] = 1;
TABLE[422][345] = 1;
TABLE[422][395] = 1;
TABLE[422][36] = 1;
TABLE[422][40] = 1;
TABLE[422][258] = 1;
TABLE[423][475] = 1;
TABLE[424][476] = 1;
TABLE[425][43] = 1;
TABLE[426][45] = 1;
TABLE[427][33] = 1;
TABLE[428][126] = 1;
TABLE[429][58] = 1;
TABLE[430][256] = 1;
TABLE[430][259] = 1;
TABLE[430][257] = 1;
TABLE[430][458] = 1;
TABLE[430][345] = 1;
TABLE[430][395] = 1;
TABLE[430][36] = 1;
TABLE[430][40] = 1;
TABLE[430][258] = 1;
TABLE[430][387] = 1;
TABLE[430][384] = 1;
TABLE[431][475] = 1;
TABLE[431][476] = 1;
TABLE[431][43] = 1;
TABLE[431][45] = 1;
TABLE[431][33] = 1;
TABLE[431][126] = 1;
TABLE[431][58] = 1;
TABLE[431][384] = 1;
TABLE[431][387] = 1;
TABLE[431][258] = 1;
TABLE[431][40] = 1;
TABLE[431][36] = 1;
TABLE[431][395] = 1;
TABLE[431][345] = 1;
TABLE[431][458] = 1;
TABLE[431][257] = 1;
TABLE[431][259] = 1;
TABLE[431][256] = 1;
TABLE[432][475] = 1;
TABLE[432][476] = 1;
TABLE[432][43] = 1;
TABLE[432][45] = 1;
TABLE[432][33] = 1;
TABLE[432][126] = 1;
TABLE[432][58] = 1;
TABLE[432][384] = 1;
TABLE[432][387] = 1;
TABLE[432][258] = 1;
TABLE[432][40] = 1;
TABLE[432][36] = 1;
TABLE[432][395] = 1;
TABLE[432][345] = 1;
TABLE[432][458] = 1;
TABLE[432][257] = 1;
TABLE[432][259] = 1;
TABLE[432][256] = 1;
TABLE[433][40] = 1;
TABLE[434][256] = 1;
TABLE[434][259] = 1;
TABLE[434][257] = 1;
TABLE[434][458] = 1;
TABLE[434][345] = 1;
TABLE[434][395] = 1;
TABLE[434][36] = 1;
TABLE[434][40] = 1;
TABLE[434][258] = 1;
TABLE[434][387] = 1;
TABLE[434][384] = 1;
TABLE[434][58] = 1;
TABLE[434][126] = 1;
TABLE[434][33] = 1;
TABLE[434][45] = 1;
TABLE[434][43] = 1;
TABLE[434][476] = 1;
TABLE[434][475] = 1;
TABLE[435][256] = 1;
TABLE[435][259] = 1;
TABLE[435][257] = 1;
TABLE[435][458] = 1;
TABLE[435][345] = 1;
TABLE[435][395] = 1;
TABLE[435][36] = 1;
TABLE[435][258] = 1;
TABLE[435][387] = 1;
TABLE[435][384] = 1;
TABLE[435][58] = 1;
TABLE[435][126] = 1;
TABLE[435][33] = 1;
TABLE[435][45] = 1;
TABLE[435][43] = 1;
TABLE[435][476] = 1;
TABLE[435][475] = 1;
TABLE[435][40] = 1;
TABLE[436][256] = 1;
TABLE[436][259] = 1;
TABLE[436][257] = 1;
TABLE[436][458] = 1;
TABLE[436][345] = 1;
TABLE[436][395] = 1;
TABLE[436][36] = 1;
TABLE[436][258] = 1;
TABLE[436][387] = 1;
TABLE[436][384] = 1;
TABLE[436][58] = 1;
TABLE[436][126] = 1;
TABLE[436][33] = 1;
TABLE[436][45] = 1;
TABLE[436][43] = 1;
TABLE[436][476] = 1;
TABLE[436][475] = 1;
TABLE[436][40] = 1;
TABLE[437][256] = 1;
TABLE[437][259] = 1;
TABLE[437][257] = 1;
TABLE[437][458] = 1;
TABLE[437][345] = 1;
TABLE[437][395] = 1;
TABLE[437][36] = 1;
TABLE[437][258] = 1;
TABLE[437][387] = 1;
TABLE[437][384] = 1;
TABLE[437][58] = 1;
TABLE[437][126] = 1;
TABLE[437][33] = 1;
TABLE[437][45] = 1;
TABLE[437][43] = 1;
TABLE[437][476] = 1;
TABLE[437][475] = 1;
TABLE[437][40] = 1;
TABLE[438][40] = 1;
TABLE[438][475] = 1;
TABLE[438][476] = 1;
TABLE[438][43] = 1;
TABLE[438][45] = 1;
TABLE[438][33] = 1;
TABLE[438][126] = 1;
TABLE[438][58] = 1;
TABLE[438][384] = 1;
TABLE[438][387] = 1;
TABLE[438][258] = 1;
TABLE[438][36] = 1;
TABLE[438][395] = 1;
TABLE[438][345] = 1;
TABLE[438][458] = 1;
TABLE[438][257] = 1;
TABLE[438][259] = 1;
TABLE[438][256] = 1;
TABLE[439][40] = 1;
TABLE[439][475] = 1;
TABLE[439][476] = 1;
TABLE[439][43] = 1;
TABLE[439][45] = 1;
TABLE[439][33] = 1;
TABLE[439][126] = 1;
TABLE[439][58] = 1;
TABLE[439][384] = 1;
TABLE[439][387] = 1;
TABLE[439][258] = 1;
TABLE[439][36] = 1;
TABLE[439][395] = 1;
TABLE[439][345] = 1;
TABLE[439][458] = 1;
TABLE[439][257] = 1;
TABLE[439][259] = 1;
TABLE[439][256] = 1;
TABLE[440][40] = 1;
TABLE[440][475] = 1;
TABLE[440][476] = 1;
TABLE[440][43] = 1;
TABLE[440][45] = 1;
TABLE[440][33] = 1;
TABLE[440][126] = 1;
TABLE[440][58] = 1;
TABLE[440][384] = 1;
TABLE[440][387] = 1;
TABLE[440][258] = 1;
TABLE[440][36] = 1;
TABLE[440][395] = 1;
TABLE[440][345] = 1;
TABLE[440][458] = 1;
TABLE[440][257] = 1;
TABLE[440][259] = 1;
TABLE[440][256] = 1;
TABLE[441][256] = 1;
TABLE[441][259] = 1;
TABLE[441][257] = 1;
TABLE[441][458] = 1;
TABLE[441][345] = 1;
TABLE[441][395] = 1;
TABLE[441][36] = 1;
TABLE[441][258] = 1;
TABLE[441][387] = 1;
TABLE[441][384] = 1;
TABLE[441][58] = 1;
TABLE[441][126] = 1;
TABLE[441][33] = 1;
TABLE[441][45] = 1;
TABLE[441][43] = 1;
TABLE[441][476] = 1;
TABLE[441][475] = 1;
TABLE[441][40] = 1;
TABLE[442][256] = 1;
TABLE[442][259] = 1;
TABLE[442][257] = 1;
TABLE[442][458] = 1;
TABLE[442][345] = 1;
TABLE[442][395] = 1;
TABLE[442][36] = 1;
TABLE[442][258] = 1;
TABLE[442][387] = 1;
TABLE[442][384] = 1;
TABLE[442][58] = 1;
TABLE[442][126] = 1;
TABLE[442][33] = 1;
TABLE[442][45] = 1;
TABLE[442][43] = 1;
TABLE[442][476] = 1;
TABLE[442][475] = 1;
TABLE[442][40] = 1;
TABLE[443][256] = 1;
TABLE[443][259] = 1;
TABLE[443][257] = 1;
TABLE[443][458] = 1;
TABLE[443][345] = 1;
TABLE[443][395] = 1;
TABLE[443][36] = 1;
TABLE[443][258] = 1;
TABLE[443][387] = 1;
TABLE[443][384] = 1;
TABLE[443][58] = 1;
TABLE[443][126] = 1;
TABLE[443][33] = 1;
TABLE[443][45] = 1;
TABLE[443][43] = 1;
TABLE[443][476] = 1;
TABLE[443][475] = 1;
TABLE[443][40] = 1;
TABLE[444][40] = 1;
TABLE[444][475] = 1;
TABLE[444][476] = 1;
TABLE[444][43] = 1;
TABLE[444][45] = 1;
TABLE[444][33] = 1;
TABLE[444][126] = 1;
TABLE[444][58] = 1;
TABLE[444][384] = 1;
TABLE[444][387] = 1;
TABLE[444][258] = 1;
TABLE[444][36] = 1;
TABLE[444][395] = 1;
TABLE[444][345] = 1;
TABLE[444][458] = 1;
TABLE[444][257] = 1;
TABLE[444][259] = 1;
TABLE[444][256] = 1;
TABLE[445][40] = 1;
TABLE[445][475] = 1;
TABLE[445][476] = 1;
TABLE[445][43] = 1;
TABLE[445][45] = 1;
TABLE[445][33] = 1;
TABLE[445][126] = 1;
TABLE[445][58] = 1;
TABLE[445][384] = 1;
TABLE[445][387] = 1;
TABLE[445][258] = 1;
TABLE[445][36] = 1;
TABLE[445][395] = 1;
TABLE[445][345] = 1;
TABLE[445][458] = 1;
TABLE[445][257] = 1;
TABLE[445][259] = 1;
TABLE[445][256] = 1;
TABLE[446][40] = 1;
TABLE[446][475] = 1;
TABLE[446][476] = 1;
TABLE[446][43] = 1;
TABLE[446][45] = 1;
TABLE[446][33] = 1;
TABLE[446][126] = 1;
TABLE[446][58] = 1;
TABLE[446][384] = 1;
TABLE[446][387] = 1;
TABLE[446][258] = 1;
TABLE[446][36] = 1;
TABLE[446][395] = 1;
TABLE[446][345] = 1;
TABLE[446][458] = 1;
TABLE[446][257] = 1;
TABLE[446][259] = 1;
TABLE[446][256] = 1;
TABLE[447][40] = 1;
TABLE[447][475] = 1;
TABLE[447][476] = 1;
TABLE[447][43] = 1;
TABLE[447][45] = 1;
TABLE[447][33] = 1;
TABLE[447][126] = 1;
TABLE[447][58] = 1;
TABLE[447][384] = 1;
TABLE[447][387] = 1;
TABLE[447][258] = 1;
TABLE[447][36] = 1;
TABLE[447][395] = 1;
TABLE[447][345] = 1;
TABLE[447][458] = 1;
TABLE[447][257] = 1;
TABLE[447][259] = 1;
TABLE[447][256] = 1;
TABLE[448][40] = 1;
TABLE[448][475] = 1;
TABLE[448][476] = 1;
TABLE[448][43] = 1;
TABLE[448][45] = 1;
TABLE[448][33] = 1;
TABLE[448][126] = 1;
TABLE[448][58] = 1;
TABLE[448][384] = 1;
TABLE[448][387] = 1;
TABLE[448][258] = 1;
TABLE[448][36] = 1;
TABLE[448][395] = 1;
TABLE[448][345] = 1;
TABLE[448][458] = 1;
TABLE[448][257] = 1;
TABLE[448][259] = 1;
TABLE[448][256] = 1;
TABLE[449][40] = 1;
TABLE[449][475] = 1;
TABLE[449][476] = 1;
TABLE[449][43] = 1;
TABLE[449][45] = 1;
TABLE[449][33] = 1;
TABLE[449][126] = 1;
TABLE[449][58] = 1;
TABLE[449][384] = 1;
TABLE[449][387] = 1;
TABLE[449][258] = 1;
TABLE[449][36] = 1;
TABLE[449][395] = 1;
TABLE[449][345] = 1;
TABLE[449][458] = 1;
TABLE[449][257] = 1;
TABLE[449][259] = 1;
TABLE[449][256] = 1;
TABLE[450][40] = 1;
TABLE[450][475] = 1;
TABLE[450][476] = 1;
TABLE[450][43] = 1;
TABLE[450][45] = 1;
TABLE[450][33] = 1;
TABLE[450][126] = 1;
TABLE[450][58] = 1;
TABLE[450][384] = 1;
TABLE[450][387] = 1;
TABLE[450][258] = 1;
TABLE[450][36] = 1;
TABLE[450][395] = 1;
TABLE[450][345] = 1;
TABLE[450][458] = 1;
TABLE[450][257] = 1;
TABLE[450][259] = 1;
TABLE[450][256] = 1;
TABLE[451][256] = 1;
TABLE[451][259] = 1;
TABLE[451][257] = 1;
TABLE[451][458] = 1;
TABLE[451][345] = 1;
TABLE[451][395] = 1;
TABLE[451][36] = 1;
TABLE[451][258] = 1;
TABLE[451][387] = 1;
TABLE[451][384] = 1;
TABLE[451][58] = 1;
TABLE[451][126] = 1;
TABLE[451][33] = 1;
TABLE[451][45] = 1;
TABLE[451][43] = 1;
TABLE[451][476] = 1;
TABLE[451][475] = 1;
TABLE[451][40] = 1;
TABLE[452][256] = 1;
TABLE[452][259] = 1;
TABLE[452][257] = 1;
TABLE[452][458] = 1;
TABLE[452][345] = 1;
TABLE[452][395] = 1;
TABLE[452][36] = 1;
TABLE[452][258] = 1;
TABLE[452][387] = 1;
TABLE[452][384] = 1;
TABLE[452][58] = 1;
TABLE[452][126] = 1;
TABLE[452][33] = 1;
TABLE[452][45] = 1;
TABLE[452][43] = 1;
TABLE[452][476] = 1;
TABLE[452][475] = 1;
TABLE[452][40] = 1;
TABLE[453][40] = 1;
TABLE[453][475] = 1;
TABLE[453][476] = 1;
TABLE[453][43] = 1;
TABLE[453][45] = 1;
TABLE[453][33] = 1;
TABLE[453][126] = 1;
TABLE[453][58] = 1;
TABLE[453][384] = 1;
TABLE[453][387] = 1;
TABLE[453][258] = 1;
TABLE[453][36] = 1;
TABLE[453][395] = 1;
TABLE[453][345] = 1;
TABLE[453][458] = 1;
TABLE[453][257] = 1;
TABLE[453][259] = 1;
TABLE[453][256] = 1;
TABLE[454][40] = 1;
TABLE[454][475] = 1;
TABLE[454][476] = 1;
TABLE[454][43] = 1;
TABLE[454][45] = 1;
TABLE[454][33] = 1;
TABLE[454][126] = 1;
TABLE[454][58] = 1;
TABLE[454][384] = 1;
TABLE[454][387] = 1;
TABLE[454][258] = 1;
TABLE[454][36] = 1;
TABLE[454][395] = 1;
TABLE[454][345] = 1;
TABLE[454][458] = 1;
TABLE[454][257] = 1;
TABLE[454][259] = 1;
TABLE[454][256] = 1;
TABLE[455][256] = 1;
TABLE[455][259] = 1;
TABLE[455][257] = 1;
TABLE[455][458] = 1;
TABLE[455][345] = 1;
TABLE[455][395] = 1;
TABLE[455][36] = 1;
TABLE[455][258] = 1;
TABLE[455][387] = 1;
TABLE[455][384] = 1;
TABLE[455][58] = 1;
TABLE[455][126] = 1;
TABLE[455][33] = 1;
TABLE[455][45] = 1;
TABLE[455][43] = 1;
TABLE[455][476] = 1;
TABLE[455][475] = 1;
TABLE[455][40] = 1;
TABLE[456][256] = 1;
TABLE[456][259] = 1;
TABLE[456][257] = 1;
TABLE[456][458] = 1;
TABLE[456][345] = 1;
TABLE[456][395] = 1;
TABLE[456][36] = 1;
TABLE[456][258] = 1;
TABLE[456][387] = 1;
TABLE[456][384] = 1;
TABLE[456][58] = 1;
TABLE[456][126] = 1;
TABLE[456][33] = 1;
TABLE[456][45] = 1;
TABLE[456][43] = 1;
TABLE[456][476] = 1;
TABLE[456][475] = 1;
TABLE[456][40] = 1;
TABLE[457][40] = 1;
TABLE[457][475] = 1;
TABLE[457][476] = 1;
TABLE[457][43] = 1;
TABLE[457][45] = 1;
TABLE[457][33] = 1;
TABLE[457][126] = 1;
TABLE[457][58] = 1;
TABLE[457][384] = 1;
TABLE[457][387] = 1;
TABLE[457][258] = 1;
TABLE[457][36] = 1;
TABLE[457][395] = 1;
TABLE[457][345] = 1;
TABLE[457][458] = 1;
TABLE[457][257] = 1;
TABLE[457][259] = 1;
TABLE[457][256] = 1;
TABLE[458][40] = 1;
TABLE[458][475] = 1;
TABLE[458][476] = 1;
TABLE[458][43] = 1;
TABLE[458][45] = 1;
TABLE[458][33] = 1;
TABLE[458][126] = 1;
TABLE[458][58] = 1;
TABLE[458][384] = 1;
TABLE[458][387] = 1;
TABLE[458][258] = 1;
TABLE[458][36] = 1;
TABLE[458][395] = 1;
TABLE[458][345] = 1;
TABLE[458][458] = 1;
TABLE[458][257] = 1;
TABLE[458][259] = 1;
TABLE[458][256] = 1;
TABLE[459][256] = 1;
TABLE[459][259] = 1;
TABLE[459][257] = 1;
TABLE[459][458] = 1;
TABLE[459][345] = 1;
TABLE[459][395] = 1;
TABLE[459][36] = 1;
TABLE[459][258] = 1;
TABLE[459][387] = 1;
TABLE[459][384] = 1;
TABLE[459][58] = 1;
TABLE[459][126] = 1;
TABLE[459][33] = 1;
TABLE[459][45] = 1;
TABLE[459][43] = 1;
TABLE[459][476] = 1;
TABLE[459][475] = 1;
TABLE[459][40] = 1;
TABLE[460][256] = 1;
TABLE[460][259] = 1;
TABLE[460][257] = 1;
TABLE[460][458] = 1;
TABLE[460][345] = 1;
TABLE[460][395] = 1;
TABLE[460][36] = 1;
TABLE[460][258] = 1;
TABLE[460][387] = 1;
TABLE[460][384] = 1;
TABLE[460][58] = 1;
TABLE[460][126] = 1;
TABLE[460][33] = 1;
TABLE[460][45] = 1;
TABLE[460][43] = 1;
TABLE[460][476] = 1;
TABLE[460][475] = 1;
TABLE[460][40] = 1;
TABLE[461][40] = 1;
TABLE[461][475] = 1;
TABLE[461][476] = 1;
TABLE[461][43] = 1;
TABLE[461][45] = 1;
TABLE[461][33] = 1;
TABLE[461][126] = 1;
TABLE[461][58] = 1;
TABLE[461][384] = 1;
TABLE[461][387] = 1;
TABLE[461][258] = 1;
TABLE[461][36] = 1;
TABLE[461][395] = 1;
TABLE[461][345] = 1;
TABLE[461][458] = 1;
TABLE[461][257] = 1;
TABLE[461][259] = 1;
TABLE[461][256] = 1;
TABLE[462][256] = 1;
TABLE[462][259] = 1;
TABLE[462][257] = 1;
TABLE[462][458] = 1;
TABLE[462][345] = 1;
TABLE[462][395] = 1;
TABLE[462][36] = 1;
TABLE[462][258] = 1;
TABLE[462][387] = 1;
TABLE[462][384] = 1;
TABLE[462][58] = 1;
TABLE[462][126] = 1;
TABLE[462][33] = 1;
TABLE[462][45] = 1;
TABLE[462][43] = 1;
TABLE[462][476] = 1;
TABLE[462][475] = 1;
TABLE[462][40] = 1;
TABLE[463][256] = 1;
TABLE[463][259] = 1;
TABLE[463][257] = 1;
TABLE[463][458] = 1;
TABLE[463][345] = 1;
TABLE[463][395] = 1;
TABLE[463][36] = 1;
TABLE[463][258] = 1;
TABLE[463][387] = 1;
TABLE[463][384] = 1;
TABLE[463][58] = 1;
TABLE[463][126] = 1;
TABLE[463][33] = 1;
TABLE[463][45] = 1;
TABLE[463][43] = 1;
TABLE[463][476] = 1;
TABLE[463][475] = 1;
TABLE[463][40] = 1;
TABLE[464][258] = 1;
TABLE[464][40] = 1;
TABLE[464][36] = 1;
TABLE[464][395] = 1;
TABLE[464][345] = 1;
TABLE[464][458] = 1;
TABLE[464][257] = 1;
TABLE[464][259] = 1;
TABLE[464][256] = 1;
TABLE[465][258] = 1;
TABLE[465][40] = 1;
TABLE[465][36] = 1;
TABLE[465][395] = 1;
TABLE[465][345] = 1;
TABLE[465][458] = 1;
TABLE[465][257] = 1;
TABLE[465][259] = 1;
TABLE[465][256] = 1;
TABLE[466][258] = 1;
TABLE[466][40] = 1;
TABLE[466][36] = 1;
TABLE[466][395] = 1;
TABLE[466][345] = 1;
TABLE[466][458] = 1;
TABLE[466][257] = 1;
TABLE[466][259] = 1;
TABLE[466][256] = 1;
TABLE[467][258] = 1;
TABLE[467][40] = 1;
TABLE[467][36] = 1;
TABLE[467][395] = 1;
TABLE[467][345] = 1;
TABLE[467][458] = 1;
TABLE[467][257] = 1;
TABLE[467][259] = 1;
TABLE[467][256] = 1;
TABLE[468][258] = 1;
TABLE[468][40] = 1;
TABLE[468][36] = 1;
TABLE[468][395] = 1;
TABLE[468][345] = 1;
TABLE[468][458] = 1;
TABLE[468][257] = 1;
TABLE[468][259] = 1;
TABLE[468][256] = 1;
TABLE[469][258] = 1;
TABLE[469][40] = 1;
TABLE[469][36] = 1;
TABLE[469][395] = 1;
TABLE[469][345] = 1;
TABLE[469][458] = 1;
TABLE[469][257] = 1;
TABLE[469][259] = 1;
TABLE[469][256] = 1;
TABLE[470][258] = 1;
TABLE[470][40] = 1;
TABLE[470][36] = 1;
TABLE[470][395] = 1;
TABLE[470][345] = 1;
TABLE[470][458] = 1;
TABLE[470][257] = 1;
TABLE[470][259] = 1;
TABLE[470][256] = 1;
TABLE[471][258] = 1;
TABLE[471][40] = 1;
TABLE[471][36] = 1;
TABLE[471][395] = 1;
TABLE[471][345] = 1;
TABLE[471][458] = 1;
TABLE[471][257] = 1;
TABLE[471][259] = 1;
TABLE[471][256] = 1;
TABLE[472][258] = 1;
TABLE[472][40] = 1;
TABLE[472][36] = 1;
TABLE[472][395] = 1;
TABLE[472][345] = 1;
TABLE[472][458] = 1;
TABLE[472][257] = 1;
TABLE[472][259] = 1;
TABLE[472][256] = 1;
TABLE[473][258] = 1;
TABLE[473][40] = 1;
TABLE[473][36] = 1;
TABLE[473][395] = 1;
TABLE[473][345] = 1;
TABLE[473][458] = 1;
TABLE[473][257] = 1;
TABLE[473][259] = 1;
TABLE[473][256] = 1;
TABLE[474][258] = 1;
TABLE[474][40] = 1;
TABLE[474][36] = 1;
TABLE[474][395] = 1;
TABLE[474][345] = 1;
TABLE[474][458] = 1;
TABLE[474][257] = 1;
TABLE[474][259] = 1;
TABLE[474][256] = 1;
TABLE[475][40] = 1;
TABLE[475][475] = 1;
TABLE[475][476] = 1;
TABLE[475][43] = 1;
TABLE[475][45] = 1;
TABLE[475][33] = 1;
TABLE[475][126] = 1;
TABLE[475][58] = 1;
TABLE[475][384] = 1;
TABLE[475][387] = 1;
TABLE[475][258] = 1;
TABLE[475][36] = 1;
TABLE[475][395] = 1;
TABLE[475][345] = 1;
TABLE[475][458] = 1;
TABLE[475][257] = 1;
TABLE[475][259] = 1;
TABLE[475][256] = 1;
TABLE[476][475] = 1;
TABLE[476][476] = 1;
TABLE[476][43] = 1;
TABLE[476][45] = 1;
TABLE[476][33] = 1;
TABLE[476][126] = 1;
TABLE[476][58] = 1;
TABLE[476][384] = 1;
TABLE[476][387] = 1;
TABLE[476][258] = 1;
TABLE[476][40] = 1;
TABLE[476][36] = 1;
TABLE[476][395] = 1;
TABLE[476][345] = 1;
TABLE[476][458] = 1;
TABLE[476][257] = 1;
TABLE[476][259] = 1;
TABLE[476][256] = 1;
TABLE[477][256] = 1;
TABLE[477][259] = 1;
TABLE[477][257] = 1;
TABLE[477][458] = 1;
TABLE[477][345] = 1;
TABLE[477][395] = 1;
TABLE[477][36] = 1;
TABLE[477][40] = 1;
TABLE[477][258] = 1;
TABLE[477][387] = 1;
TABLE[477][384] = 1;
TABLE[477][58] = 1;
TABLE[477][126] = 1;
TABLE[477][33] = 1;
TABLE[477][45] = 1;
TABLE[477][43] = 1;
TABLE[477][476] = 1;
TABLE[477][475] = 1;
TABLE[478][41] = 1;
TABLE[478][475] = 1;
TABLE[478][476] = 1;
TABLE[478][43] = 1;
TABLE[478][45] = 1;
TABLE[478][33] = 1;
TABLE[478][126] = 1;
TABLE[478][58] = 1;
TABLE[478][384] = 1;
TABLE[478][387] = 1;
TABLE[478][258] = 1;
TABLE[478][40] = 1;
TABLE[478][36] = 1;
TABLE[478][395] = 1;
TABLE[478][345] = 1;
TABLE[478][458] = 1;
TABLE[478][257] = 1;
TABLE[478][259] = 1;
TABLE[478][256] = 1;
TABLE[1023][256] = 1;
TABLE[1023][259] = 1;
TABLE[1023][257] = 1;
TABLE[1023][458] = 1;
TABLE[1023][345] = 1;
TABLE[1023][395] = 1;
TABLE[1023][36] = 1;
TABLE[1023][40] = 1;
TABLE[1023][258] = 1;
TABLE[1023][387] = 1;
TABLE[1023][384] = 1;
TABLE[1023][58] = 1;
TABLE[1023][126] = 1;
TABLE[1023][33] = 1;
TABLE[1023][45] = 1;
TABLE[1023][43] = 1;
TABLE[1023][476] = 1;
TABLE[1023][475] = 1;
TABLE[1425][41] = 1;
TABLE[1022][44] = 1;
TABLE[1424][41] = 1;
}

extern int yydirset();
int yydirset(int i, int j)
{
   return TABLE[i][j];
}
int yytransparent(int n)
{
   switch(n) {
      case 1: return 1; break;
      case 6: return 1; break;
      case 10: return 1; break;
      case 18: return 1; break;
      case 144: return 0; break;
      case 148: return 1; break;
      case 156: return 1; break;
      case 163: return 0; break;
      case 173: return 0; break;
      case 189: return 0; break;
      case 223: return 0; break;
      case 237: return 0; break;
      case 259: return 0; break;
      case 286: return 0; break;
      case 300: return 1; break;
      case 310: return 1; break;
      case 320: return 1; break;
      case 328: return 1; break;
      case 335: return 1; break;
      case 346: return 0; break;
      case 355: return 1; break;
      case 368: return 1; break;
      case 417: return 1; break;
      case 426: return 1; break;
      case 433: return 0; break;
      case 441: return 0; break;
      case 445: return 1; break;
      case 454: return 0; break;
      case 462: return 0; break;
      case 471: return 0; break;
      case 479: return 1; break;
      case 486: return 1; break;
      case 493: return 0; break;
      case 501: return 1; break;
      case 509: return 1; break;
      case 515: return 1; break;
      case 522: return 0; break;
      case 530: return 1; break;
      case 538: return 0; break;
      case 546: return 1; break;
      case 555: return 0; break;
      case 561: return 1; break;
      case 570: return 0; break;
      case 579: return 1; break;
      case 588: return 0; break;
      case 592: return 0; break;
      case 596: return 1; break;
      case 601: return 1; break;
      case 621: return 1; break;
      case 628: return 1; break;
      case 637: return 0; break;
      case 652: return 0; break;
      case 663: return 1; break;
      case 672: return 1; break;
      case 680: return 1; break;
      case 688: return 1; break;
      case 932: return 1; break;
      case 939: return 1; break;
      case 946: return 0; break;
      case 954: return 1; break;
      case 961: return 0; break;
      case 969: return 1; break;
      case 976: return 1; break;
      case 983: return 1; break;
      case 992: return 1; break;
      case 999: return 1; break;
      case 1006: return 1; break;
      case 1013: return 1; break;
      case 1020: return 1; break;
      case 1027: return 1; break;
      case 1036: return 1; break;
      case 1043: return 1; break;
      case 1050: return 1; break;
      case 1059: return 1; break;
      case 1066: return 1; break;
      case 1073: return 1; break;
      case 1082: return 1; break;
      case 1089: return 1; break;
      case 1096: return 1; break;
      case 1103: return 1; break;
      case 1110: return 1; break;
      case 1119: return 1; break;
      case 1126: return 1; break;
      case 1134: return 1; break;
      case 1143: return 1; break;
      case 1150: return 1; break;
      case 1157: return 1; break;
      case 1164: return 1; break;
      case 1171: return 1; break;
      case 1178: return 1; break;
      case 1185: return 1; break;
      case 1194: return 1; break;
      case 1201: return 1; break;
      case 1208: return 1; break;
      case 1217: return 1; break;
      case 1224: return 1; break;
      case 1231: return 1; break;
      case 1240: return 1; break;
      case 1247: return 0; break;
      case 1266: return 0; break;
      case 1283: return 1; break;
      case 1295: return 1; break;
      case 1302: return 1; break;
      case 1309: return 1; break;
      case 1319: return 0; break;
      case 1331: return 1; break;
      case 1343: return 1; break;
      case 1350: return 1; break;
      case 1357: return 1; break;
      case 1364: return 1; break;
      case 1374: return 0; break;
      case 1386: return 1; break;
      case 1393: return 0; break;
      case 1404: return 1; break;
      case 1411: return 1; break;
      case 1418: return 0; break;
      case 1424: return 1; break;
      case 1431: return 0; break;
      case 1441: return 1; break;
      case 1449: return 0; break;
      case 1715: return 1; break;
      case 1722: return 1; break;
      case 1729: return 1; break;
      case 1736: return 1; break;
      case 1743: return 1; break;
      case 1750: return 1; break;
      case 1757: return 1; break;
      case 1764: return 1; break;
      case 1771: return 1; break;
      case 1778: return 1; break;
      case 1785: return 1; break;
      case 1792: return 1; break;
      case 1799: return 1; break;
      case 1806: return 1; break;
      case 1813: return 1; break;
      case 1820: return 1; break;
      case 1827: return 1; break;
      case 1834: return 1; break;
      case 1841: return 1; break;
      case 1848: return 1; break;
      case 1855: return 1; break;
      case 1862: return 1; break;
      case 1869: return 1; break;
      case 1877: return 1; break;
      case 1884: return 1; break;
      case 1891: return 1; break;
      case 1900: return 1; break;
      case 1907: return 1; break;
      case 1914: return 1; break;
      case 1923: return 1; break;
      case 1930: return 1; break;
      case 1937: return 1; break;
      case 1945: return 1; break;
      case 1952: return 1; break;
      case 1961: return 1; break;
      case 1972: return 1; break;
      case 1979: return 1; break;
      case 1986: return 1; break;
      case 1993: return 1; break;
      case 2000: return 1; break;
      case 2007: return 1; break;
      case 2014: return 1; break;
      case 2021: return 1; break;
      case 2028: return 1; break;
      case 2035: return 0; break;
      case 2046: return 1; break;
      case 2053: return 1; break;
      case 2060: return 0; break;
      case 2071: return 1; break;
      case 2078: return 1; break;
      case 2085: return 0; break;
      case 2095: return 1; break;
      case 2103: return 0; break;
      case 2176: return 1; break;
      case 2183: return 1; break;
      case 2190: return 1; break;
      case 2197: return 1; break;
      case 2204: return 1; break;
      case 2211: return 1; break;
      case 2220: return 1; break;
      case 2231: return 0; break;
      case 2239: return 1; break;
      case 2246: return 1; break;
      case 2253: return 1; break;
      case 2260: return 1; break;
      case 2272: return 1; break;
      case 2281: return 1; break;
      case 2288: return 0; break;
      case 2304: return 1; break;
      case 2312: return 1; break;
      case 2324: return 1; break;
      case 2332: return 0; break;
      case 2486: return 1; break;
      case 2493: return 1; break;
      case 2500: return 1; break;
      case 2507: return 1; break;
      case 2514: return 1; break;
      case 2521: return 1; break;
      case 2528: return 1; break;
      case 2535: return 1; break;
      case 2544: return 1; break;
      case 2551: return 1; break;
      case 2558: return 1; break;
      case 2565: return 1; break;
      case 2572: return 1; break;
      case 2579: return 1; break;
      case 2586: return 1; break;
      case 2593: return 1; break;
      case 2600: return 1; break;
      case 2607: return 1; break;
      case 2614: return 1; break;
      case 2621: return 1; break;
      case 2628: return 1; break;
      case 2635: return 1; break;
      case 2642: return 1; break;
      case 2649: return 1; break;
      case 2658: return 1; break;
      case 2669: return 1; break;
      case 2673: return 1; break;
      case 2682: return 1; break;
      case 2693: return 0; break;
      case 2697: return 0; break;
      case 2706: return 1; break;
      case 2714: return 1; break;
      case 2722: return 1; break;
      case 2807: return 1; break;
      case 2814: return 1; break;
      case 2825: return 1; break;
      case 2832: return 1; break;
      case 2839: return 1; break;
      case 2846: return 1; break;
      case 2855: return 1; break;
      case 2862: return 1; break;
      case 2869: return 1; break;
      case 2876: return 1; break;
      case 2883: return 1; break;
      case 2890: return 1; break;
      case 2897: return 1; break;
      case 2904: return 1; break;
      case 2913: return 1; break;
      case 2920: return 1; break;
      case 2927: return 1; break;
      case 2934: return 1; break;
      case 2943: return 1; break;
      case 2954: return 0; break;
      case 2962: return 1; break;
      case 2970: return 0; break;
      case 3063: return 1; break;
      case 3070: return 1; break;
      case 3077: return 1; break;
      case 3084: return 1; break;
      case 3091: return 1; break;
      case 3098: return 1; break;
      case 3105: return 1; break;
      case 3112: return 1; break;
      case 3122: return 1; break;
      case 3129: return 1; break;
      case 3136: return 1; break;
      case 3146: return 1; break;
      case 3153: return 1; break;
      case 3160: return 1; break;
      case 3169: return 1; break;
      case 3176: return 0; break;
      case 3180: return 0; break;
      case 3184: return 1; break;
      case 3191: return 1; break;
      case 3200: return 1; break;
      case 3207: return 1; break;
      case 3214: return 1; break;
      case 3223: return 1; break;
      case 3230: return 1; break;
      case 3237: return 1; break;
      case 3246: return 1; break;
      case 3257: return 0; break;
      case 3287: return 1; break;
      case 3294: return 1; break;
      case 3301: return 1; break;
      case 3308: return 0; break;
      case 3325: return 0; break;
      case 3360: return 1; break;
      case 3367: return 1; break;
      case 3374: return 1; break;
      case 3381: return 0; break;
      case 3389: return 1; break;
      case 3397: return 0; break;
      case 3416: return 1; break;
      case 3423: return 0; break;
      case 3443: return 1; break;
      case 3450: return 1; break;
      case 3469: return 1; break;
      case 3476: return 1; break;
      case 3483: return 1; break;
      case 3490: return 1; break;
      case 3501: return 1; break;
      case 3509: return 0; break;
      case 3517: return 1; break;
      case 3524: return 1; break;
      case 3535: return 1; break;
      case 3543: return 0; break;
      case 3564: return 1; break;
      case 3571: return 0; break;
      case 3583: return 0; break;
      case 3591: return 1; break;
      case 3599: return 1; break;
      case 3675: return 1; break;
      case 3682: return 1; break;
      case 3689: return 1; break;
      case 3700: return 1; break;
      case 3707: return 1; break;
      case 3714: return 1; break;
      case 3721: return 1; break;
      case 3730: return 1; break;
      case 3737: return 1; break;
      case 3744: return 1; break;
      case 3753: return 1; break;
      case 3760: return 1; break;
      case 3767: return 1; break;
      case 3776: return 1; break;
      case 3783: return 1; break;
      case 3792: return 0; break;
      case 3796: return 1; break;
      case 3805: return 0; break;
      case 3809: return 1; break;
      case 3818: return 0; break;
      case 3822: return 1; break;
      case 3831: return 0; break;
      case 3835: return 0; break;
      case 3887: return 1; break;
      case 3895: return 0; break;
      case 3903: return 0; break;
      case 3911: return 0; break;
      case 3921: return 1; break;
      case 3929: return 0; break;
      case 3962: return 1; break;
      case 3969: return 1; break;
      case 3978: return 1; break;
      case 3985: return 1; break;
      case 3992: return 1; break;
      case 4001: return 1; break;
      case 4008: return 1; break;
      case 4015: return 0; break;
      case 4023: return 1; break;
      case 4031: return 0; break;
      case 4076: return 1; break;
      case 4083: return 1; break;
      case 4092: return 1; break;
      case 4099: return 1; break;
      case 4106: return 1; break;
      case 4115: return 1; break;
      case 4126: return 1; break;
      case 4133: return 1; break;
      case 4140: return 1; break;
      case 4149: return 1; break;
      case 4156: return 0; break;
      case 4164: return 1; break;
      case 4172: return 0; break;
      case 4224: return 1; break;
      case 4231: return 1; break;
      case 4238: return 1; break;
      case 4245: return 1; break;
      case 4252: return 1; break;
      case 4259: return 1; break;
      case 4268: return 1; break;
      case 4279: return 0; break;
      case 4289: return 1; break;
      case 4297: return 0; break;
      case 4391: return 1; break;
      case 4398: return 1; break;
      case 4405: return 1; break;
      case 4412: return 1; break;
      case 4419: return 1; break;
      case 4426: return 1; break;
      case 4433: return 1; break;
      case 4440: return 1; break;
      case 4449: return 1; break;
      case 4460: return 1; break;
      case 4467: return 1; break;
      case 4476: return 1; break;
      case 4487: return 1; break;
      case 4499: return 1; break;
      case 4522: return 0; break;
      case 4538: return 1; break;
      case 4545: return 1; break;
      case 4552: return 1; break;
      case 4573: return 1; break;
      case 4580: return 1; break;
      case 4588: return 1; break;
      case 4596: return 0; break;
      case 4607: return 1; break;
      case 4615: return 1; break;
      case 4623: return 0; break;
      case 4850: return 1; break;
      case 4857: return 1; break;
      case 4864: return 1; break;
      case 4871: return 0; break;
      case 4879: return 1; break;
      case 4886: return 1; break;
      case 4893: return 1; break;
      case 4900: return 1; break;
      case 4907: return 1; break;
      case 4914: return 1; break;
      case 4921: return 1; break;
      case 4928: return 1; break;
      case 4935: return 1; break;
      case 4942: return 1; break;
      case 4949: return 1; break;
      case 4956: return 1; break;
      case 4963: return 0; break;
      case 4971: return 1; break;
      case 4978: return 1; break;
      case 4987: return 1; break;
      case 4994: return 1; break;
      case 5003: return 0; break;
      case 5012: return 0; break;
      case 5020: return 1; break;
      case 5027: return 1; break;
      case 5034: return 1; break;
      case 5043: return 0; break;
      case 5052: return 0; break;
      case 5060: return 1; break;
      case 5067: return 1; break;
      case 5074: return 1; break;
      case 5083: return 1; break;
      case 5090: return 1; break;
      case 5099: return 1; break;
      case 5106: return 1; break;
      case 5113: return 1; break;
      case 5122: return 1; break;
      case 5129: return 1; break;
      case 5136: return 1; break;
      case 5145: return 1; break;
      case 5152: return 1; break;
      case 5159: return 1; break;
      case 5168: return 0; break;
      case 5177: return 0; break;
      case 5185: return 0; break;
      case 5193: return 0; break;
      case 5210: return 1; break;
      case 5222: return 1; break;
      case 5229: return 1; break;
      case 5236: return 1; break;
      case 5246: return 0; break;
      case 5258: return 1; break;
      case 5270: return 1; break;
      case 5277: return 1; break;
      case 5284: return 1; break;
      case 5291: return 1; break;
      case 5301: return 0; break;
      case 5313: return 1; break;
      case 5320: return 0; break;
      case 5324: return 0; break;
      case 5334: return 1; break;
      case 5342: return 0; break;
      case 5371: return 1; break;
      case 5378: return 1; break;
      case 5385: return 1; break;
      case 5392: return 1; break;
      case 5399: return 0; break;
      case 5408: return 1; break;
      case 5416: return 1; break;
      case 5424: return 0; break;
      case 5506: return 0; break;
      case 5514: return 1; break;
      case 5522: return 1; break;
      case 5529: return 1; break;
      case 5538: return 1; break;
      case 5549: return 0; break;
      case 5558: return 0; break;
      case 5570: return 1; break;
      case 5625: return 0; break;
      case 5633: return 0; break;
      case 5646: return 1; break;
      case 5662: return 1; break;
      case 5672: return 1; break;
      case 5681: return 0; break;
      case 5689: return 0; break;
      case 5699: return 1; break;
      case 5707: return 0; break;
      case 5733: return 1; break;
      case 5740: return 1; break;
      case 5747: return 1; break;
      case 5756: return 1; break;
      case 5767: return 0; break;
      case 5774: return 1; break;
      case 5822: return 0; break;
      case 5826: return 0; break;
      case 5874: return 0; break;
      case 5887: return 1; break;
      case 5898: return 1; break;
      case 5906: return 1; break;
      case 5913: return 1; break;
      case 5921: return 1; break;
      case 5933: return 1; break;
      case 5941: return 1; break;
      case 5948: return 1; break;
      case 5956: return 1; break;
      case 5964: return 0; break;
      case 5975: return 1; break;
      case 5986: return 1; break;
      case 5994: return 1; break;
      case 6001: return 1; break;
      case 6009: return 1; break;
      case 6021: return 1; break;
      case 6029: return 1; break;
      case 6036: return 1; break;
      case 6044: return 1; break;
      case 6052: return 0; break;
      case 6061: return 0; break;
      case 6070: return 0; break;
      case 6077: return 1; break;
      case 6086: return 0; break;
      case 6093: return 1; break;
      case 6102: return 0; break;
      case 6109: return 1; break;
      case 6118: return 0; break;
      case 6207: return 1; break;
      case 6218: return 0; break;
      case 6226: return 1; break;
      case 6234: return 1; break;
      case 6243: return 1; break;
      case 6251: return 0; break;
      case 6257: return 1; break;
      case 6265: return 0; break;
      case 6272: return 1; break;
      case 6286: return 1; break;
      case 6294: return 0; break;
      case 6360: return 0; break;
      case 6368: return 0; break;
      case 6376: return 0; break;
      case 6392: return 0; break;
      case 6400: return 0; break;
      case 6432: return 0; break;
      case 6472: return 0; break;
      case 6482: return 0; break;
      case 6493: return 0; break;
      case 6515: return 0; break;
      case 6531: return 0; break;
      case 6547: return 0; break;
      case 6587: return 0; break;
      case 6597: return 0; break;
      case 6607: return 0; break;
      case 6617: return 0; break;
      case 6627: return 0; break;
      case 6637: return 0; break;
      case 6649: return 0; break;
      case 6719: return 0; break;
      case 6729: return 1; break;
      case 6733: return 1; break;
      case 6741: return 1; break;
   }
   return 0;
}
char * yyprintname(int n)
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
         case 5874: return "s_prototype"; break;
         case 5887: return "Subphrase"; break;
         case 5898: return "Subphrase"; break;
         case 5906: return "Subphrase"; break;
         case 5913: return "Subphrase"; break;
         case 5921: return "Subphrase"; break;
         case 5933: return "Subphrase"; break;
         case 5941: return "Subphrase"; break;
         case 5948: return "Subphrase"; break;
         case 5956: return "Subphrase"; break;
         case 5964: return "sg_prototype"; break;
         case 5975: return "Subphrase"; break;
         case 5986: return "Subphrase"; break;
         case 5994: return "Subphrase"; break;
         case 6001: return "Subphrase"; break;
         case 6009: return "Subphrase"; break;
         case 6021: return "Subphrase"; break;
         case 6029: return "Subphrase"; break;
         case 6036: return "Subphrase"; break;
         case 6044: return "Subphrase"; break;
         case 6052: return "s_declaration"; break;
         case 6061: return "s_object_decl"; break;
         case 6070: return "g_declaration"; break;
         case 6077: return "Subphrase"; break;
         case 6086: return "l_declaration"; break;
         case 6093: return "Subphrase"; break;
         case 6102: return "r_declaration"; break;
         case 6109: return "Subphrase"; break;
         case 6118: return "statement"; break;
         case 6207: return "Subphrase"; break;
         case 6218: return "Subphrase"; break;
         case 6226: return "Subphrase"; break;
         case 6234: return "Subphrase"; break;
         case 6243: return "Subphrase"; break;
         case 6251: return "block"; break;
         case 6257: return "Subphrase"; break;
         case 6265: return "routine"; break;
         case 6272: return "Subphrase"; break;
         case 6286: return "Subphrase"; break;
         case 6294: return "fund_expr"; break;
         case 6360: return "Subphrase"; break;
         case 6368: return "Subphrase"; break;
         case 6376: return "lvalue"; break;
         case 6392: return "Subphrase"; break;
         case 6400: return "expr10"; break;
         case 6432: return "expr20"; break;
         case 6472: return "expr30"; break;
         case 6482: return "expr40"; break;
         case 6493: return "expr50"; break;
         case 6515: return "expr60"; break;
         case 6531: return "expr70"; break;
         case 6547: return "expr80"; break;
         case 6587: return "expr90"; break;
         case 6597: return "expr100"; break;
         case 6607: return "expr110"; break;
         case 6617: return "expr120"; break;
         case 6627: return "expr130"; break;
         case 6637: return "expr140"; break;
         case 6649: return "expr150"; break;
         case 6719: return "expr"; break;
         case 6729: return "param_list"; break;
         case 6733: return "Subphrase"; break;
         case 6741: return "Subphrase"; break;
   }
   else 
      switch(n-50000) {
         case 496: return "MEMBER_OP"; break;
         case 495: return "ELLIPSIS"; break;
         case 494: return "ASSIGN_SUB"; break;
         case 493: return "ASSIGN_RSHIFT"; break;
         case 492: return "ASSIGN_LSHIFT"; break;
         case 491: return "ASSIGN_NEG"; break;
         case 490: return "ASSIGN_OR"; break;
         case 489: return "ASSIGN_AND"; break;
         case 488: return "ASSIGN_MOD"; break;
         case 487: return "ASSIGN_DIV"; break;
         case 486: return "ASSIGN_ADD"; break;
         case 485: return "ASSIGN_MULT"; break;
         case 484: return "OR_OP"; break;
         case 483: return "AND_OP"; break;
         case 482: return "NE_OP"; break;
         case 481: return "EQ_OP"; break;
         case 480: return "LE_OP"; break;
         case 479: return "GE_OP"; break;
         case 478: return "RSHIFT_OP"; break;
         case 477: return "LSHIFT_OP"; break;
         case 476: return "DECREMENT_OP"; break;
         case 475: return "INCREMENT_OP"; break;
         case 474: return "WORD_RECT"; break;
         case 473: return "YVAL"; break;
         case 472: return "XVAL"; break;
         case 471: return "WITH"; break;
         case 470: return "WIELD"; break;
         case 469: return "WHILE"; break;
         case 468: return "WEIGHT_LIM"; break;
         case 467: return "WEIGHT_MOD"; break;
         case 466: return "WEIGHT"; break;
         case 465: return "WEEK"; break;
         case 464: return "WALLS"; break;
         case 463: return "VAR"; break;
         case 462: return "VALUE"; break;
         case 461: return "TVAL"; break;
         case 460: return "TYPE"; break;
         case 459: return "TURNS"; break;
         case 458: return "WORD_TRUE"; break;
         case 457: return "TO"; break;
         case 456: return "TIMEOUT"; break;
         case 455: return "TILES"; break;
         case 454: return "TILE"; break;
         case 453: return "THROWING"; break;
         case 452: return "THREAT"; break;
         case 451: return "TEXT"; break;
         case 450: return "TERRAIN"; break;
         case 449: return "WORD_TEMPLATE"; break;
         case 448: return "WORD_TARGET"; break;
         case 447: return "SYSTEM"; break;
         case 446: return "SWITCH"; break;
         case 445: return "SVAL"; break;
         case 444: return "SUSTAINS"; break;
         case 443: return "STORE"; break;
         case 442: return "STOCK"; break;
         case 441: return "STATIC"; break;
         case 440: return "STATI"; break;
         case 439: return "STARTING"; break;
         case 438: return "START"; break;
         case 437: return "SPELLS"; break;
         case 436: return "SPELL"; break;
         case 435: return "SPEED"; break;
         case 434: return "SPECIALS"; break;
         case 433: return "SLOT"; break;
         case 432: return "SKILLS"; break;
         case 431: return "SIZE"; break;
         case 430: return "SDMG"; break;
         case 429: return "SAVES"; break;
         case 428: return "RVAL"; break;
         case 427: return "ROUTINE"; break;
         case 426: return "ROOMTYPES"; break;
         case 425: return "RETURN"; break;
         case 424: return "RESISTS"; break;
         case 423: return "REGIONS"; break;
         case 422: return "REGION"; break;
         case 421: return "RATING"; break;
         case 420: return "RANGE"; break;
         case 419: return "RANDOM"; break;
         case 418: return "RACE"; break;
         case 417: return "QVAL"; break;
         case 416: return "QUEST"; break;
         case 415: return "QUALITIES"; break;
         case 414: return "PVAL"; break;
         case 413: return "PURPOSE"; break;
         case 412: return "PROP"; break;
         case 411: return "PROFICIENCIES"; break;
         case 410: return "POWER"; break;
         case 409: return "POISON"; break;
         case 408: return "PER"; break;
         case 407: return "PENALTY"; break;
         case 406: return "PARTS"; break;
         case 405: return "PARTIAL"; break;
         case 404: return "PARRY"; break;
         case 403: return "PARAMS"; break;
         case 402: return "OPTION"; break;
         case 401: return "OR"; break;
         case 400: return "ON"; break;
         case 399: return "OF"; break;
         case 398: return "OBJECTS"; break;
         case 397: return "OBJECT"; break;
         case 396: return "NUTRITION"; break;
         case 395: return "WORD_NULL"; break;
         case 394: return "NPC"; break;
         case 393: return "NORMAL"; break;
         case 392: return "NONE"; break;
         case 391: return "NEAR"; break;
         case 390: return "MTYPE"; break;
         case 389: return "MOVE"; break;
         case 388: return "MONSTER"; break;
         case 387: return "WORD_MIN"; break;
         case 386: return "MEMBER"; break;
         case 385: return "MELEE"; break;
         case 384: return "WORD_MAX"; break;
         case 383: return "MATERIAL"; break;
         case 382: return "MAP"; break;
         case 381: return "MANA"; break;
         case 380: return "MODULE"; break;
         case 379: return "MAX_SIZE"; break;
         case 378: return "LVAL"; break;
         case 377: return "LISTS"; break;
         case 376: return "LIGHT"; break;
         case 375: return "LIFESPAN"; break;
         case 374: return "WORD_LEVEL"; break;
         case 373: return "LDMG"; break;
         case 372: return "ITEM"; break;
         case 371: return "INVOKE"; break;
         case 370: return "INNATE"; break;
         case 369: return "IMMUNE"; break;
         case 368: return "IMAGE"; break;
         case 367: return "IF"; break;
         case 366: return "HIT"; break;
         case 365: return "HITPOINTS"; break;
         case 364: return "HITDICE"; break;
         case 363: return "HEAVY"; break;
         case 362: return "GUILD"; break;
         case 361: return "GROUP"; break;
         case 360: return "GRID"; break;
         case 359: return "GRANTS"; break;
         case 358: return "GOD"; break;
         case 357: return "GLYPH"; break;
         case 356: return "GEAR"; break;
         case 355: return "FUEL"; break;
         case 354: return "FROM"; break;
         case 353: return "FOR"; break;
         case 352: return "WORD_FILE"; break;
         case 351: return "FLOOR"; break;
         case 350: return "FLAGS"; break;
         case 349: return "FIRES"; break;
         case 348: return "FEATURE"; break;
         case 347: return "FEAT"; break;
         case 346: return "FEATS"; break;
         case 345: return "WORD_FALSE"; break;
         case 344: return "FACTOR"; break;
         case 343: return "EVERY"; break;
         case 342: return "EVENT"; break;
         case 341: return "FLAVOR"; break;
         case 340: return "FAVORED"; break;
         case 339: return "EVAL"; break;
         case 338: return "EQUIP"; break;
         case 337: return "EXPORT"; break;
         case 336: return "WORD_ENCOUNTER"; break;
         case 335: return "ELSE"; break;
         case 334: return "EFFECT"; break;
         case 333: return "DVAL"; break;
         case 332: return "DUNGEON"; break;
         case 331: return "DO"; break;
         case 330: return "DISEASE"; break;
         case 329: return "DOMAINS"; break;
         case 328: return "DOMAIN"; break;
         case 327: return "DOOR"; break;
         case 326: return "WORD_DESC"; break;
         case 325: return "DEPTH"; break;
         case 324: return "DEFAULT"; break;
         case 323: return "DEF"; break;
         case 322: return "WORD_DC"; break;
         case 321: return "DAY"; break;
         case 320: return "DARK"; break;
         case 319: return "DMG"; break;
         case 318: return "CVAL"; break;
         case 317: return "CURSED"; break;
         case 316: return "CRIT"; break;
         case 315: return "COST"; break;
         case 314: return "CONTINUE"; break;
         case 313: return "CTYPE"; break;
         case 312: return "COVERAGE"; break;
         case 311: return "CONSTANTS"; break;
         case 310: return "WORD_COLOR"; break;
         case 309: return "CLASS"; break;
         case 308: return "CHANCE"; break;
         case 307: return "WORD_CR"; break;
         case 306: return "CASTER"; break;
         case 305: return "CASE"; break;
         case 304: return "CAPACITY"; break;
         case 303: return "BRIGHT"; break;
         case 302: return "BREAK"; break;
         case 301: return "BRAWL"; break;
         case 300: return "BLESSED"; break;
         case 299: return "BEHAVIOUR"; break;
         case 298: return "BASE"; break;
         case 297: return "AVAL"; break;
         case 296: return "ATTACK"; break;
         case 295: return "AT"; break;
         case 294: return "AS"; break;
         case 293: return "ARTIFACT"; break;
         case 292: return "ARM"; break;
         case 291: return "ARCHERY"; break;
         case 290: return "APTITUDES"; break;
         case 289: return "WORD_ANY"; break;
         case 288: return "AND"; break;
         case 287: return "ALSO"; break;
         case 286: return "ALL"; break;
         case 285: return "ALIGN"; break;
         case 284: return "ACC"; break;
         case 283: return "ABS"; break;
         case 282: return "ABILITY"; break;
         case 281: return "REGNAME"; break;
         case 280: return "OPCODE"; break;
         case 279: return "STYPE"; break;
         case 278: return "DIRECTION"; break;
         case 277: return "ATTRIBUTE"; break;
         case 276: return "WEP_TYPE"; break;
         case 275: return "COLOR"; break;
         case 274: return "BOOL"; break;
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
   return "special_character";
}
